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

/** ���� ���� ������ ���缭.. FStreamableManager �� ����� �ε��ڵ�� �ٲٱ� ���� ������. */
template<class TAssetClass>
static TAssetClass* LoadAndCacheDamageEffectInfoTAssetArrayCommon(FStreamableManager& InLoadManager, 
	TSoftObjectPtr<TAssetClass>& InTAsset, int32 AssetArrayIndex, TMap<int32, TAssetClass*>& InLoadedPtrMap)
{
	check(AssetArrayIndex >= 0);

	// DamageEffectInfo ������. Async �ε� �� �� ��. ���� �ٸ� Async �ε��� �������� �ʱ� ���� FStreamableManager ����� �ʼ�.

	TAssetClass*& LoadedMapElem = InLoadedPtrMap.FindOrAdd(AssetArrayIndex);
	if (LoadedMapElem && InTAsset.IsValid()) {
		return LoadedMapElem; // �̹� �ε��Ǿ� �ִ� ���̽�
	}
		
	if (!InTAsset.IsValid() && !InTAsset.IsNull())
	{ // TAsset �ε�. InTAsset ��ü�� LoadSynchronous() �� �ϸ� �ٸ� async loading �� flush �� �Ǵ� ������ �־ �ΰ��� ���� �߿� Ȱ���� ���� �̰� Ư���� FStreamableManager ����� �߿��ϴ�.
		InLoadManager.LoadSynchronous(InTAsset);
	}

	TAssetClass* Loaded = InTAsset.Get();
	LoadedMapElem = Loaded;
	
	return Loaded;
}
/** ��� �ܵ����� �����ϴ� �� �뵵 */
template<class TAssetClass>
static TAssetClass* LoadAndCacheDamageEffectInfoTAssetSingleCommon(FStreamableManager& InLoadManager,
	TSoftObjectPtr<TAssetClass>& InTAsset, TAssetClass*& InLoadedPtr)
{
	// DamageEffectInfo ������. Async �ε� �� �� ��. ���� �ٸ� Async �ε��� �������� �ʱ� ���� FStreamableManager ����� �ʼ�.

	if (InLoadedPtr && InTAsset.IsValid()) {
		return InLoadedPtr; // �̹� �ε��Ǿ� �ִ� ���̽�
	}

	if (!InTAsset.IsValid() && !InTAsset.IsNull())
	{ // TAsset �ε�. InTAsset ��ü�� LoadSynchronous() �� �ϸ� �ٸ� async loading �� flush �� �Ǵ� ������ �־ �ΰ��� ���� �߿� Ȱ���� ���� �̰� Ư���� FStreamableManager ����� �߿��ϴ�.
		InLoadManager.LoadSynchronous(InTAsset);
	}

	TAssetClass* Loaded = InTAsset.Get();
	InLoadedPtr = Loaded;

	return Loaded;
}

template<class TAssetClass>
static void PreloadAndCacheTAssetArrayCommon(FStreamableManager& InLoadManager, TArray<TSoftObjectPtr<TAssetClass>>& InAssetArray, TMap<int32, TAssetClass*>& InLoadedPtrMap)
{
	InLoadedPtrMap.Empty(); // �ӹ��ϰ� ÷���� �����ϴ� ��..
	for (int32 AssetIndex = 0; AssetIndex < InAssetArray.Num(); ++AssetIndex)
	{
		TSoftObjectPtr<TAssetClass>& ThisTAsset = InAssetArray[AssetIndex];
		LoadAndCacheDamageEffectInfoTAssetArrayCommon<TAssetClass>(InLoadManager, ThisTAsset, AssetIndex, InLoadedPtrMap);
	}
	checkSlow(InAssetArray.Num() == InLoadedPtrMap.Num()); // Null �̾ �߰��ǰ���..
}

// ���� �������� ���� ShouldUnloadAllAssetOnLevelTransition ����
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

	// 180�� �����ٳ��
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
		// UGameplayStatics::SpawnEmitterAttached ��ſ� ��ɷ�.
		//UB2PreRenderer::SpawnDummyPSCForPreRender(InFxTemplate, InOutOwnerSKActor->GetRootComponent());
	}
}
// BladeIIGameMode �� PreRender ��ƾ�� �Ϻη�.. InfoAsset �߿��� �ΰ��� ���ɿ� ������ ��ġ�� �� ��� ���� ó��.
void UB2DamageEffectInfo::SetupPreRenderSKMeshActorForFX(ASkeletalMeshActor* InOutSKActorToSetup)
{
	if (!InOutSKActorToSetup) { // SkeletalMeshActor �� GameMode �ʿ��� �����ؼ� �ش�. Pre-render �⺻ ��ġ�� ���ʿ��� �����ϹǷ�.
		return;
	}

	// �ϳ��� �ϳ���..
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
			// ����� �Ѿ�� InOutSkActorsToSetup �� ������ GetDesiredPreRenderDummyNumForMtrlOverride ��ŭ�� �ǵ�..
			if (InOutSkActorsToSetup.IsValidIndex(ActualLoadedMI)) // InOutSkActorsToSetup �ִ� ��� CharacterMaterialOverrides �ϳ��� ������ ��.
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
		if (!ThisMatOverride.ToString().IsEmpty()){ // ���� �ε����� �ϴ� �� ���⼭ ��︮���� �ʰ�..
			++MI;
		}
	}
	return MI;
}

/** For preventing in-game loading spike, possibly for specific platform only */
void UB2DamageEffectInfo::PreloadAndCacheAllTAsset()
{
#if !UE_BUILD_SHIPPING
	extern bool gPreloadCombatEffectResources; // �̰� ���ɿ� ������ �󸶳� �⿩�ϴ��� �׽�Ʈ�� ����
	if (!gPreloadCombatEffectResources) {
		return;
	}
#endif

#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] �ϴ� iOS �� �����ϰ� �ٸ� �÷����鿡����
	if (GIsEditor) {
		return; // �����͸� �ؼ� �ȵ� �� ���� �ѵ� �ε� �ð� �����̶� �� �ɸ� �״� �׳� ����. �׸��� ���� LoadedPtr �� serialize �� ������ �����̶� �� Ȯ���� ���� ��
	}

	B2_SCOPED_TRACK_LOG(TEXT("UB2DamageEffectInfo::PreloadAndCacheAllTAsset"));

	// TAsset ���� �ε��ؼ� Loaded~~ �� �־ GC ����.
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
	// �̸����Ͱ� �� �߱������ε� �⺻�����δ� Unload �� �ǵ��� �����.
	// �ϴٺ��� �ʿ��� ���� RootSet �� ���Խ�Ű�� �͵�.. ������ �ʿ��� �͵� �ִ��� �ε��Ǿ� ���� ������ ������ ���ؼ�.

	if(!ShouldUnloadAllAssetOnLevelTransition())
	{ // �״��� ��︮�� ������ �� �����μ��� ������ ������ ����� ��..
		ConditionalMarkLoadedAssetsRootSet();
	}

	// �ٵ� ĳ���� ���ϰ� ����.. �װ� ������ ��. ��Ʈ���̸� ������ ������ �ٽ� �ε� �� �� ����.
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
		// ���⼭ �ε��ϴ� �� �ƴ�. �ε��� �͸� ��ũ

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
		{// ���� �ǹ̺��� "���"���� "�Ϻ�" ���� ���� ������ �ʰ� �ϳ��� AddToRoot �Ǹ� ����. ���� �������� �׸� ö���� ī��Ʈ�� �ʿ��� �� �ƴ�.
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