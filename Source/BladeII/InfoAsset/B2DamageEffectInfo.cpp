// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#include "B2DamageEffectInfo.h"
#include "BladeIICharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Animation/SkeletalMeshActor.h"
#include "BladeIIGameMode.h"
#include "BladeIIUtil.h"
#include "B2UIDocHelper.h"

/** 여기 에셋 구성에 맞춰서.. FStreamableManager 라도 사용한 로딩코드로 바꾸기 위해 급조함. */
template<class TAssetClass>
static TAssetClass* LoadAndCacheDamageEffectInfoTAssetArrayCommon(FStreamableManager& InLoadManager, 
	TSoftObjectPtr<TAssetClass>& InTAsset, int32 AssetArrayIndex, TMap<int32, TAssetClass*>& InLoadedPtrMap)
{
	check(AssetArrayIndex >= 0);

	// DamageEffectInfo 전용임. Async 로딩 안 할 거. 단지 다른 Async 로딩을 방해하지 않기 위해 FStreamableManager 사용은 필수.

	TAssetClass*& LoadedMapElem = InLoadedPtrMap.FindOrAdd(AssetArrayIndex);
	if (LoadedMapElem && InTAsset.IsValid()) {
		return LoadedMapElem; // 이미 로딩되어 있는 케이스
	}
		
	if (!InTAsset.IsValid() && !InTAsset.IsNull())
	{ // TAsset 로딩. InTAsset 자체의 LoadSynchronous() 로 하면 다른 async loading 이 flush 가 되는 문제가 있어서 인게임 전투 중에 활발히 사용될 이건 특히나 FStreamableManager 사용이 중요하다.
		InLoadManager.LoadSynchronous(InTAsset);
	}

	TAssetClass* Loaded = InTAsset.Get();
	LoadedMapElem = Loaded;
	
	return Loaded;
}
/** 요건 단독으로 존재하는 거 용도 */
template<class TAssetClass>
static TAssetClass* LoadAndCacheDamageEffectInfoTAssetSingleCommon(FStreamableManager& InLoadManager,
	TSoftObjectPtr<TAssetClass>& InTAsset, TAssetClass*& InLoadedPtr)
{
	// DamageEffectInfo 전용임. Async 로딩 안 할 거. 단지 다른 Async 로딩을 방해하지 않기 위해 FStreamableManager 사용은 필수.

	if (InLoadedPtr && InTAsset.IsValid()) {
		return InLoadedPtr; // 이미 로딩되어 있는 케이스
	}

	if (!InTAsset.IsValid() && !InTAsset.IsNull())
	{ // TAsset 로딩. InTAsset 자체의 LoadSynchronous() 로 하면 다른 async loading 이 flush 가 되는 문제가 있어서 인게임 전투 중에 활발히 사용될 이건 특히나 FStreamableManager 사용이 중요하다.
		InLoadManager.LoadSynchronous(InTAsset);
	}

	TAssetClass* Loaded = InTAsset.Get();
	InLoadedPtr = Loaded;

	return Loaded;
}

template<class TAssetClass>
static void PreloadAndCacheTAssetArrayCommon(FStreamableManager& InLoadManager, TArray<TSoftObjectPtr<TAssetClass>>& InAssetArray, TMap<int32, TAssetClass*>& InLoadedPtrMap)
{
	InLoadedPtrMap.Empty(); // 쌈박하게 첨부터 시작하는 거..
	for (int32 AssetIndex = 0; AssetIndex < InAssetArray.Num(); ++AssetIndex)
	{
		TSoftObjectPtr<TAssetClass>& ThisTAsset = InAssetArray[AssetIndex];
		LoadAndCacheDamageEffectInfoTAssetArrayCommon<TAssetClass>(InLoadManager, ThisTAsset, AssetIndex, InLoadedPtrMap);
	}
	checkSlow(InAssetArray.Num() == InLoadedPtrMap.Num()); // Null 이어도 추가되겠지..
}

// 직접 참조하지 말고 ShouldUnloadAllAssetOnLevelTransition 으로
bool UB2DamageEffectInfo::bRootsetOnLoad = false;
bool UB2DamageEffectInfo::bAssetsRooted = false;
UB2DamageEffectInfo::UB2DamageEffectInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUseRandomRotation = true;
	RandRange = 45.f;
	HitFlashDuration = 0.1f;
	HitFlashDurationForPC = 0.5f;
	DamageEffectRange = 50.f;
	MaxDamageEffectNum = 7;

	if (GConfig)
	{
		GConfig->GetBool(TEXT("/Script/BladeII"), TEXT("DamageEffectInfoRootsetOnLoad"), bRootsetOnLoad, GGameIni);
	}
}

UParticleSystem* UB2DamageEffectInfo::GetWeaponPSTCommon(TArray<TSoftObjectPtr<UParticleSystem>>& PSTAssetArray, EDamageElement ElementType, TMap<int32, UParticleSystem*>& InLoadedPtrMap)
{
	int32 CastedIndex = static_cast<int32>(ElementType);
	if (PSTAssetArray.Num() > CastedIndex)
	{
		return LoadAndCacheDamageEffectInfoTAssetArrayCommon<UParticleSystem>(InfoLoadManager, PSTAssetArray[CastedIndex], CastedIndex, InLoadedPtrMap);
	}
	return NULL;
}
bool UB2DamageEffectInfo::IsEffectSpawnable(const ABladeIICharacter* Target) const
{
	if (Target == nullptr)
		return false;

	if (Target->IsBuffActive(EBuffType::Debuff_Burning))
	{
		auto* DocBattle = UB2UIDocHelper::GetDocBattle();

		if (DocBattle && DocBattle->GetIsCenematicMode())
			return false;
	}

	return true;
}

void UB2DamageEffectInfo::SpawnDamageEffect(EDamageWeapon WeaponType, EDamageElement ElementType, class ABladeIICharacter* Target, EHitDirection HitDir, float RandomAngle, FRotator HitActorRot)
{
	if (!IsEffectSpawnable(Target))
	{
		return;
	}

	UParticleSystem* PSTemplate = NULL;
	ABladeIIPlayer* PC = Cast<ABladeIIPlayer>(Target);
	FName DamageSocketName = Target->DamageSocketName;

	if (PC)
	{
		if (PC->IsApplyGuarding())
		{
			PSTemplate = LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, GuardPST, LoadedGuardPST);
			DamageSocketName = GuardDamageSocketName.IsNone() ? Target->DamageSocketName : GuardDamageSocketName;
		}
		else if (PC->IsBuffActive(EBuffType::Buff_ReflectDamage))
		{
			PSTemplate = LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, ReflectDamagePST, LoadedReflectDamagePST);
			DamageSocketName = ReflectDamageSocketName.IsNone() ? Target->DamageSocketName : ReflectDamageSocketName;
		}
		else if (PC->IsBuffActive(EBuffType::Buff_Shield))
		{
			PSTemplate = LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, ShieldPST, LoadedShieldPST);
			DamageSocketName = ShieldDamageSocketName.IsNone() ? Target->DamageSocketName : ShieldDamageSocketName;
		}
	}

	if (PSTemplate == NULL)
	{
		switch (WeaponType)
		{
		case EDamageWeapon::EDW_Blade:
		{
			PSTemplate = GetWeaponPSTCommon((PC != NULL) ? BladeWeaponPSTForPC : BladeWeaponPST, ElementType, 
				(PC != NULL) ? LoadedBladeWeaponPSTForPC : LoadedBladeWeaponPST);
		}
		break;
		case EDamageWeapon::EDW_Hammer:
		{
			PSTemplate = GetWeaponPSTCommon((PC != NULL) ? HammerWeaponPSTForPC : HammerWeaponPST, ElementType,
				(PC != NULL) ? LoadedHammerWeaponPSTForPC : LoadedHammerWeaponPST);
		}
		break;
		case EDamageWeapon::EDW_Magic:
		{
			PSTemplate = GetWeaponPSTCommon((PC != NULL) ? MagicWeaponPSTForPC : MagicWeaponPST, ElementType,
				(PC != NULL) ? LoadedMagicWeaponPSTForPC : LoadedMagicWeaponPST);
		}
		break;
		case EDamageWeapon::EDW_End:
			break;
		default:
			break;
		}
	}

	SpawnDamageEffectInner(PSTemplate, Target, HitDir, RandomAngle, HitActorRot, DamageSocketName);
}

void UB2DamageEffectInfo::SpawnDamageEffectInner(UParticleSystem* PSTemplate, class ABladeIICharacter* Target, EHitDirection HitDir, float RandomAngle, FRotator HitActorRot, FName DamageSocketName)
{
#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
	if (bDisableDamageEffectParticle) {
		return;
	}
#endif

	if (Target == NULL || Target->GetMesh() == NULL || PSTemplate == NULL || (MaxDamageEffectNum > 0 && DamageEffectComps.Num() >= MaxDamageEffectNum))
	{
		return;
	}

	FTransform DamageSocketTransform = Target->GetMesh()->GetSocketTransform(DamageSocketName);
		
	FVector2D ScreenPos;

	// Optimization to reduce hit effect
	if (MaxDamageEffectNum > 0)
	{
		if (const APlayerController* PC = 0)//Cast<APlayerController*>(Target->GetController()))
		{
			ULocalPlayer* const LP = PC ? PC->GetLocalPlayer() : nullptr;
			if (LP && LP->ViewportClient)
			{
				// get the projection data
				FSceneViewProjectionData ProjectionData;
				if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
				{
					FMatrix const ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
					int32 ViewSizeX = 1280, ViewSizeY = 720;
					PC->GetViewportSize(ViewSizeX, ViewSizeY);
					FSceneView::ProjectWorldToScreen(DamageSocketTransform.GetTranslation(), FIntRect(0, 0, ViewSizeX, ViewSizeY)/*ProjectionData.GetConstrainedViewRect()*/, ViewProjectionMatrix, ScreenPos);

					for (int32 i = 0; i < DamageEffectScreenPos.Num(); ++i)
					{
						const FVector2D& PrevPos = DamageEffectScreenPos[i];
						if (FVector2D::DistSquared(PrevPos, ScreenPos) < FMath::Square(DamageEffectRange))
						{
							// Kill previous one
							if (DamageEffectComps[i])
							{
								DamageEffectComps[i]->DestroyComponent();
								OnParticleSystemFinished(DamageEffectComps[i]);
							}
							else
							{
								DamageEffectComps.RemoveAt(i);
								DamageEffectScreenPos.RemoveAt(i);
							}
							break;
						}
					}
				}
			}
			else
			{
				// Culling
				return;
			}
		}
	}

	FRotator DamageRot = HitActorRot;
	if (HitDir == EHitDirection::EHD_None) // Use Default Value
	{
		DamageRot = DamageSocketTransform.Rotator();

		if (bUseRandomRotation)
		{
			DamageRot.Yaw += FMath::FRandRange(-RandRange, RandRange);
		}
	}
	else if (HitDir != EHitDirection::EHD_HitActor) // 
	{
		switch (HitDir)
		{
		case EHitDirection::EHD_NegX:
			DamageRot.Yaw += 180.f;
			break;
		case EHitDirection::EHD_PosY:
			DamageRot.Yaw += 90.f;
			break;
		case EHitDirection::EHD_NegY:
			DamageRot.Yaw -= 90.f;
			break;
		default:
			break;
		}
		if (RandomAngle > 0.f)
		{
			DamageRot.Yaw += FMath::FRandRange(-RandomAngle, RandomAngle);
		}
	}

	// 180도 돌려줄놈들
	ABladeIIPlayer* PC = Cast<ABladeIIPlayer>(Target);

	if (PC && (PC->IsApplyGuarding() || PC->IsBuffActive(EBuffType::Buff_ReflectDamage)))
	{
		DamageRot.Yaw += 180;
	}

	UParticleSystemComponent* NewComponent =
		UGameplayStatics::SpawnEmitterAtLocation(Target, PSTemplate, DamageSocketTransform.GetTranslation(), DamageRot, true);

	if (NewComponent && MaxDamageEffectNum > 0)
	{
		NewComponent->OnSystemFinished.AddUniqueDynamic(this, &UB2DamageEffectInfo::OnParticleSystemFinished);

		// Screen Pos data handling
		int32 Idx = DamageEffectComps.Add(NewComponent);
		if (Idx != INDEX_NONE)
		{
			if (DamageEffectScreenPos.Num() <= Idx)
			{
				DamageEffectScreenPos.AddDefaulted(Idx + 1 - DamageEffectScreenPos.Num());
			}
			DamageEffectScreenPos[Idx] = ScreenPos;
		}
	}
}

void UB2DamageEffectInfo::OnParticleSystemFinished(UParticleSystemComponent* FinishedComponent)
{
	if (int32 Idx = DamageEffectComps.Find(FinishedComponent))
	{
		DamageEffectComps.RemoveAt(Idx);
		DamageEffectScreenPos.RemoveAt(Idx);
	}
}

void UB2DamageEffectInfo::PlayHitFlash(class ABladeIICharacter* Target)
{
	if (Target && Target->CurrentOverridenMatType == ECharacterMaterialOverrideType::ECMO_None)
	{
		if (Target->IsBuffActive(EBuffType::Buff_Shield))
			return;

		checkSlow(HitFlashColorParamValues.Num() == (int32)EDamageElement::EDE_End && HitFlashColorParamValuesForPC.Num() == (int32)EDamageElement::EDE_End)
		int32 DamageColorIdx = 0;
		// Check State for Damage Effect, priority Frozen > Burning > Poison
		if (Target->IsBuffActive(EBuffType::Debuff_Freezing))
		{
			DamageColorIdx = 1;
		}
		else if (Target->IsBuffActive(EBuffType::Debuff_Burning))
		{
			DamageColorIdx = 2;
		}
		else if (Target->IsBuffActive(EBuffType::Debuff_Poison))
		{
			DamageColorIdx = 3;
		}
		ABladeIIPlayer* PC = Cast<ABladeIIPlayer>(Target);
		// Hit Flash Effect
		Target->FlashForDuration(HitFlashColorParamName, (PC != NULL) ? HitFlashColorParamValuesForPC[DamageColorIdx] : HitFlashColorParamValues[DamageColorIdx],
			HitFlashIntensityParamName, (PC != NULL) ? HitFlashIntensityParamValueForPC : HitFlashIntensityParamValue, (PC != NULL) ? HitFlashDurationForPC : HitFlashDuration);
	}
}

static void SpawnPreRenderParticleComponentsCommon(ASkeletalMeshActor* InOutOwnerSKActor, UParticleSystem* InFxTemplate)
{
	if (InOutOwnerSKActor && InFxTemplate) {
		// UGameplayStatics::SpawnEmitterAttached 대신에 요걸로.
		//UB2PreRenderer::SpawnDummyPSCForPreRender(InFxTemplate, InOutOwnerSKActor->GetRootComponent());
	}
}
// BladeIIGameMode 의 PreRender 루틴의 일부로.. InfoAsset 중에서 인게임 성능에 영향을 미치는 걸 골라서 같이 처리.
void UB2DamageEffectInfo::SetupPreRenderSKMeshActorForFX(ASkeletalMeshActor* InOutSKActorToSetup)
{
	if (!InOutSKActorToSetup) { // SkeletalMeshActor 는 GameMode 쪽에서 생성해서 준다. Pre-render 기본 위치는 그쪽에서 관리하므로.
		return;
	}

	// 하나둘 하나둘..
	for (int32 AI = 0; AI < BladeWeaponPST.Num(); ++AI)
	{
		TSoftObjectPtr<UParticleSystem>& ThisPSTAsset = BladeWeaponPST[AI];
		SpawnPreRenderParticleComponentsCommon(InOutSKActorToSetup, 
			LoadAndCacheDamageEffectInfoTAssetArrayCommon<UParticleSystem>(InfoLoadManager, ThisPSTAsset, AI, LoadedBladeWeaponPST));
	}
	for (int32 AI = 0; AI < HammerWeaponPST.Num(); ++AI)
	{
		TSoftObjectPtr<UParticleSystem>& ThisPSTAsset = HammerWeaponPST[AI];
		SpawnPreRenderParticleComponentsCommon(InOutSKActorToSetup, 
			LoadAndCacheDamageEffectInfoTAssetArrayCommon<UParticleSystem>(InfoLoadManager, ThisPSTAsset, AI, LoadedHammerWeaponPST));
	}
	for (int32 AI = 0; AI < MagicWeaponPST.Num(); ++AI)
	{
		TSoftObjectPtr<UParticleSystem>& ThisPSTAsset = MagicWeaponPST[AI];
		SpawnPreRenderParticleComponentsCommon(InOutSKActorToSetup, 
			LoadAndCacheDamageEffectInfoTAssetArrayCommon<UParticleSystem>(InfoLoadManager, ThisPSTAsset, AI, LoadedMagicWeaponPST));
	}
	for (int32 AI = 0; AI < BladeWeaponPSTForPC.Num(); ++AI)
	{
		TSoftObjectPtr<UParticleSystem>& ThisPSTAsset = BladeWeaponPSTForPC[AI];
		SpawnPreRenderParticleComponentsCommon(InOutSKActorToSetup, 
			LoadAndCacheDamageEffectInfoTAssetArrayCommon<UParticleSystem>(InfoLoadManager, ThisPSTAsset, AI, LoadedBladeWeaponPSTForPC));
	}
	for (int32 AI = 0; AI < HammerWeaponPSTForPC.Num(); ++AI)
	{
		TSoftObjectPtr<UParticleSystem>& ThisPSTAsset = HammerWeaponPSTForPC[AI];
		SpawnPreRenderParticleComponentsCommon(InOutSKActorToSetup,
			LoadAndCacheDamageEffectInfoTAssetArrayCommon<UParticleSystem>(InfoLoadManager, ThisPSTAsset, AI, LoadedHammerWeaponPSTForPC));
	}
	for (int32 AI = 0; AI < MagicWeaponPSTForPC.Num(); ++AI)
	{
		TSoftObjectPtr<UParticleSystem>& ThisPSTAsset = MagicWeaponPSTForPC[AI];
		SpawnPreRenderParticleComponentsCommon(InOutSKActorToSetup,
			LoadAndCacheDamageEffectInfoTAssetArrayCommon<UParticleSystem>(InfoLoadManager, ThisPSTAsset, AI, LoadedMagicWeaponPSTForPC));
	}
	SpawnPreRenderParticleComponentsCommon(InOutSKActorToSetup,
		LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, GuardPST, LoadedGuardPST));
	SpawnPreRenderParticleComponentsCommon(InOutSKActorToSetup,
		LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, ReflectDamagePST, LoadedReflectDamagePST));
	SpawnPreRenderParticleComponentsCommon(InOutSKActorToSetup,
		LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, ShieldPST, LoadedShieldPST));
}
void UB2DamageEffectInfo::SetupPreRenderMaterialOverride(TArray<class ASkeletalMeshActor*>& InOutSkActorsToSetup)
{
	int32 ActualLoadedMI = 0;
	for (int32 MI = 0; MI < CharacterMaterialOverrides.Num(); ++MI)
	{
		TSoftObjectPtr<class UMaterialInterface>& ThisMatOverride = CharacterMaterialOverrides[MI];
		UMaterialInterface* ThisLoaded =
			LoadAndCacheDamageEffectInfoTAssetArrayCommon<UMaterialInterface>(InfoLoadManager, CharacterMaterialOverrides[MI], MI, LoadedCharacterMaterialOverrides);

		if (ThisLoaded)
		{
			// 여기로 넘어온 InOutSkActorsToSetup 의 개수는 GetDesiredPreRenderDummyNumForMtrlOverride 만큼을 의도..
			if (InOutSkActorsToSetup.IsValidIndex(ActualLoadedMI)) // InOutSkActorsToSetup 있는 대로 CharacterMaterialOverrides 하나씩 세팅해 줌.
			{
				ASkeletalMeshActor* ThisActor = InOutSkActorsToSetup[ActualLoadedMI];
				if (ThisActor && ThisActor->GetSkeletalMeshComponent())
				{
					ThisActor->GetSkeletalMeshComponent()->SetMaterial(0, ThisLoaded);
				}
			}
			++ActualLoadedMI;
		}
	}
}
int32 UB2DamageEffectInfo::GetDesiredPreRenderDummyNumForMtrlOverride() const
{
	int32 MI = 0;
	for (const TSoftObjectPtr<class UMaterialInterface>& ThisMatOverride : CharacterMaterialOverrides)
	{
		if (!ThisMatOverride.ToString().IsEmpty()){ // 실제 로딩까지 하는 건 여기서 어울리지는 않고..
			++MI;
		}
	}
	return MI;
}

/** For preventing in-game loading spike, possibly for specific platform only */
void UB2DamageEffectInfo::PreloadAndCacheAllTAsset()
{
#if !UE_BUILD_SHIPPING
	extern bool gPreloadCombatEffectResources; // 이게 성능에 실제로 얼마나 기여하는지 테스트를 위해
	if (!gPreloadCombatEffectResources) {
		return;
	}
#endif

#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] 일단 iOS 는 제외하고 다른 플랫폼들에서만
	if (GIsEditor) {
		return; // 에디터면 해서 안될 거 없긴 한데 로딩 시간 조금이라도 더 걸릴 테니 그냥 리턴. 그리고 괜히 LoadedPtr 로 serialize 될 여지를 조금이라도 더 확실히 차단 ㅋ
	}

	B2_SCOPED_TRACK_LOG(TEXT("UB2DamageEffectInfo::PreloadAndCacheAllTAsset"));

	// TAsset 에서 로딩해서 Loaded~~ 에 넣어서 GC 방지.
	PreloadAndCacheTAssetArrayCommon<UParticleSystem>(InfoLoadManager, BladeWeaponPST, LoadedBladeWeaponPST);
	PreloadAndCacheTAssetArrayCommon<UParticleSystem>(InfoLoadManager, HammerWeaponPST, LoadedHammerWeaponPST);
	PreloadAndCacheTAssetArrayCommon<UParticleSystem>(InfoLoadManager, MagicWeaponPST, LoadedMagicWeaponPST);
	PreloadAndCacheTAssetArrayCommon<UParticleSystem>(InfoLoadManager, BladeWeaponPSTForPC, LoadedBladeWeaponPSTForPC);
	PreloadAndCacheTAssetArrayCommon<UParticleSystem>(InfoLoadManager, HammerWeaponPSTForPC, LoadedHammerWeaponPSTForPC);
	PreloadAndCacheTAssetArrayCommon<UParticleSystem>(InfoLoadManager, MagicWeaponPSTForPC, LoadedMagicWeaponPSTForPC);
	PreloadAndCacheTAssetArrayCommon<UMaterialInterface>(InfoLoadManager, CharacterMaterialOverrides, LoadedCharacterMaterialOverrides);

	LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, GuardPST, LoadedGuardPST);
	LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, ReflectDamagePST, LoadedReflectDamagePST);
	LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, ShieldPST, LoadedShieldPST);
#endif
}

void UB2DamageEffectInfo::ConditionalUnloadAllOrPreserve()
{
	// 이름부터가 좀 중구난방인데 기본적으로는 Unload 가 의도한 기능임.
	// 하다보니 필요한 것을 RootSet 에 포함시키는 것도.. 어차피 필요한 것들 최대한 로딩되어 있을 이쯤이 적절할 듯해서.

	if(!ShouldUnloadAllAssetOnLevelTransition())
	{ // 그다지 어울리지 않지만 현 구조로서는 이쯤이 적절한 장소일 듯..
		ConditionalMarkLoadedAssetsRootSet();
	}

	// 근데 캐쉬는 여하간 비운다.. 그게 안전할 듯. 루트셋이면 어차피 다음에 다시 로딩 안 할 것임.
	UnloadAllTAssetCache();
}

void UB2DamageEffectInfo::UnloadAllTAssetCache()
{
	LoadedBladeWeaponPST.Empty();
	LoadedHammerWeaponPST.Empty();
	LoadedMagicWeaponPST.Empty();
	LoadedBladeWeaponPSTForPC.Empty();
	LoadedHammerWeaponPSTForPC.Empty();
	LoadedMagicWeaponPSTForPC.Empty();
	LoadedCharacterMaterialOverrides.Empty();

	LoadedGuardPST = nullptr;
	LoadedReflectDamagePST = nullptr;
	LoadedShieldPST = nullptr;
}

template<class TAssetClass>
static int32 ValidTAssetArrayMarkRootSetCommon(TArray<TSoftObjectPtr<TAssetClass>>& InAssetArray)
{
	int32 AddedCount = 0;
	for (int32 AssetIndex = 0; AssetIndex < InAssetArray.Num(); ++AssetIndex)
	{
		TSoftObjectPtr<TAssetClass>& ThisTAsset = InAssetArray[AssetIndex];
		if (ThisTAsset.IsValid()) {
			ThisTAsset->AddToRoot();
			++AddedCount;
		}
	}
	return AddedCount;
}
void UB2DamageEffectInfo::ConditionalMarkLoadedAssetsRootSet()
{
	if (!ShouldUnloadAllAssetOnLevelTransition())
	{
		// 여기서 로딩하는 거 아님. 로딩된 것만 마크

		int32 AddedCount = 0;
		AddedCount += ValidTAssetArrayMarkRootSetCommon<UParticleSystem>(BladeWeaponPST);
		AddedCount += ValidTAssetArrayMarkRootSetCommon<UParticleSystem>(HammerWeaponPST);
		AddedCount += ValidTAssetArrayMarkRootSetCommon<UParticleSystem>(MagicWeaponPST);
		AddedCount += ValidTAssetArrayMarkRootSetCommon<UParticleSystem>(BladeWeaponPSTForPC);
		AddedCount += ValidTAssetArrayMarkRootSetCommon<UParticleSystem>(HammerWeaponPSTForPC);
		AddedCount += ValidTAssetArrayMarkRootSetCommon<UParticleSystem>(MagicWeaponPSTForPC);
		AddedCount += ValidTAssetArrayMarkRootSetCommon<UMaterialInterface>(CharacterMaterialOverrides);

		if (GuardPST.IsValid()){
			GuardPST->AddToRoot();
			++AddedCount;
		}
		if (ReflectDamagePST.IsValid()) {
			ReflectDamagePST->AddToRoot();
			++AddedCount;
		}
		if (ShieldPST.IsValid()) {
			ShieldPST->AddToRoot();
			++AddedCount;
		}

		if (AddedCount > 0)
		{// 변수 의미부터 "모두"인지 "일부" 인지 딱히 있지는 않고 하나만 AddToRoot 되면 세팅. 현재 사용목적이 그리 철저한 카운트가 필요한 건 아님.
			bAssetsRooted = true;
		}
	}
}

bool UB2DamageEffectInfo::ShouldUnloadAllAssetOnLevelTransition()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING]
	return true;
#else
	return !bRootsetOnLoad;
#endif
}

void UB2DamageEffectInfo::OverrideCharacterMaterial(ECharacterMaterialOverrideType MatType, class ABladeIICharacter* Target)
{
	const int32 CastedIndex = static_cast<int32>(MatType);
	if (Target && CharacterMaterialOverrides.Num() > CastedIndex)
	{
		// Cache old materials and set to new material
		Target->OverrideMaterial(
			LoadAndCacheDamageEffectInfoTAssetArrayCommon<UMaterialInterface>(InfoLoadManager, CharacterMaterialOverrides[CastedIndex], CastedIndex, LoadedCharacterMaterialOverrides),
			MatType);
	}
}

void UB2DamageEffectInfo::PlayHitSound(UObject* WorldContextObject, ECharacterType CharType, FVector Location, bool IsAboradMob /*= false*/)
{
#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
	if (bDisableDamageEffectSound) {
		return;
	}
#endif

	int32 CueIndex = INDEX_NONE;
	switch (CharType)
	{
	case ECharacterType::ECT_Assassin:
		CueIndex = (int32)EHitSoundCase::EHit_Assassin;
		break;
	case ECharacterType::ECT_Gladiator:
		CueIndex = (int32)EHitSoundCase::EHit_Gladiator;
		break;
	case ECharacterType::ECT_Wizard:
		CueIndex = (int32)EHitSoundCase::EHit_Wizard;
		break;
	case ECharacterType::ECT_Fighter:
		CueIndex = (int32)EHitSoundCase::EHit_Fighter;
		break;
	case ECharacterType::ECT_MeleeMob:
	case ECharacterType::ECT_RangeMob:
	case ECharacterType::ECT_MiddleMob:
		{
			if (IsAboradMob)
			{
				CueIndex = (int32)EHitSoundCase::EHit_AboardMob;
			}
			else
			{
				CueIndex = (int32)EHitSoundCase::EHit_Mob;
			}
		}
		break;
	case ECharacterType::ECT_BossMob:
		CueIndex = (int32)EHitSoundCase::EHit_Boss;
		break;
	default:
		break;
	}

	if (CueIndex >= 0 && HitSoundCues.Num() > CueIndex)
	{
		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, HitSoundCues[CueIndex], Location);
	}
}

void UB2DamageEffectInfo::PlayGuardHitSound(UObject* WorldContextObject, FVector Location)
{
	int32 CueIndex = (int32)EHitSoundCase::EHit_Guard;
	
	if (HitSoundCues.IsValidIndex(CueIndex))
	{
		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, HitSoundCues[CueIndex], Location);
	}
}

#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
bool UB2DamageEffectInfo::bDisableDamageEffectParticle = false;
bool UB2DamageEffectInfo::bDisableDamageEffectSound = false;
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2DamageEffectInfo::CleanupOnPreSave()
{
	UnloadAllTAssetCache();
}
void UB2DamageEffectInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	CleanupOnPreSave();
}
#endif