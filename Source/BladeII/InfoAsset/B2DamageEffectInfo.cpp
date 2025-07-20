// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#include "B2DamageEffectInfo.h"
#include "BladeIICharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Animation/SkeletalMeshActor.h"
#include "BladeIIGameMode.h"
#include "BladeIIUtil.h"
#include "UObject/ObjectSaveContext.h"
//#include "B2UIDocHelper.h"

/** 咯扁 俊悸 备己俊 嘎苗辑.. FStreamableManager 扼档 荤侩茄 肺爹内靛肺 官操扁 困秦 鞭炼窃. */
template<class TAssetClass>
static TAssetClass* LoadAndCacheDamageEffectInfoTAssetArrayCommon(FStreamableManager& InLoadManager, 
	TSoftObjectPtr<TAssetClass>& InTAsset, int32 AssetArrayIndex, TMap<int32, TAssetClass*>& InLoadedPtrMap)
{
	check(AssetArrayIndex >= 0);

	// DamageEffectInfo 傈侩烙. Async 肺爹 救 且 芭. 窜瘤 促弗 Async 肺爹阑 规秦窍瘤 臼扁 困秦 FStreamableManager 荤侩篮 鞘荐.

	TAssetClass*& LoadedMapElem = InLoadedPtrMap.FindOrAdd(AssetArrayIndex);
	if (LoadedMapElem && InTAsset.IsValid()) {
		return LoadedMapElem; // 捞固 肺爹登绢 乐绰 纳捞胶
	}
		
	if (!InTAsset.IsValid() && !InTAsset.IsNull())
	{ // TAsset 肺爹. InTAsset 磊眉狼 LoadSynchronous() 肺 窍搁 促弗 async loading 捞 flush 啊 登绰 巩力啊 乐绢辑 牢霸烙 傈捧 吝俊 劝惯洒 荤侩瞪 捞扒 漂洒唱 FStreamableManager 荤侩捞 吝夸窍促.
		InLoadManager.LoadSynchronous(InTAsset);
	}

	TAssetClass* Loaded = InTAsset.Get();
	LoadedMapElem = Loaded;
	
	return Loaded;
}
/** 夸扒 窜刀栏肺 粮犁窍绰 芭 侩档 */
template<class TAssetClass>
static TAssetClass* LoadAndCacheDamageEffectInfoTAssetSingleCommon(FStreamableManager& InLoadManager,
	TSoftObjectPtr<TAssetClass>& InTAsset, TAssetClass*& InLoadedPtr)
{
	// DamageEffectInfo 傈侩烙. Async 肺爹 救 且 芭. 窜瘤 促弗 Async 肺爹阑 规秦窍瘤 臼扁 困秦 FStreamableManager 荤侩篮 鞘荐.

	if (InLoadedPtr && InTAsset.IsValid()) {
		return InLoadedPtr; // 捞固 肺爹登绢 乐绰 纳捞胶
	}

	if (!InTAsset.IsValid() && !InTAsset.IsNull())
	{ // TAsset 肺爹. InTAsset 磊眉狼 LoadSynchronous() 肺 窍搁 促弗 async loading 捞 flush 啊 登绰 巩力啊 乐绢辑 牢霸烙 傈捧 吝俊 劝惯洒 荤侩瞪 捞扒 漂洒唱 FStreamableManager 荤侩捞 吝夸窍促.
		InLoadManager.LoadSynchronous(InTAsset);
	}

	TAssetClass* Loaded = InTAsset.Get();
	InLoadedPtr = Loaded;

	return Loaded;
}

template<class TAssetClass>
static void PreloadAndCacheTAssetArrayCommon(FStreamableManager& InLoadManager, TArray<TSoftObjectPtr<TAssetClass>>& InAssetArray, TMap<int32, TAssetClass*>& InLoadedPtrMap)
{
	InLoadedPtrMap.Empty(); // 接冠窍霸 梅何磐 矫累窍绰 芭..
	for (int32 AssetIndex = 0; AssetIndex < InAssetArray.Num(); ++AssetIndex)
	{
		TSoftObjectPtr<TAssetClass>& ThisTAsset = InAssetArray[AssetIndex];
		LoadAndCacheDamageEffectInfoTAssetArrayCommon<TAssetClass>(InLoadManager, ThisTAsset, AssetIndex, InLoadedPtrMap);
	}
	checkSlow(InAssetArray.Num() == InLoadedPtrMap.Num()); // Null 捞绢档 眠啊登摆瘤..
}

// 流立 曼炼窍瘤 富绊 ShouldUnloadAllAssetOnLevelTransition 栏肺
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
	//if (Target == nullptr)
	//	return false;

	//if (Target->IsBuffActive(EBuffType::Debuff_Burning))
	//{
	//	auto* DocBattle = UB2UIDocHelper::GetDocBattle();

	//	if (DocBattle && DocBattle->GetIsCenematicMode())
	//		return false;
	//}

	return true;
}

void UB2DamageEffectInfo::SpawnDamageEffect(EDamageWeapon WeaponType, EDamageElement ElementType, class ABladeIICharacter* Target, EHitDirection HitDir, float RandomAngle, FRotator HitActorRot)
{
	//if (!IsEffectSpawnable(Target))
	//{
	//	return;
	//}

	//UParticleSystem* PSTemplate = NULL;
	//ABladeIIPlayer* PC = Cast<ABladeIIPlayer>(Target);
	//FName DamageSocketName = Target->DamageSocketName;

	//if (PC)
	//{
	//	if (PC->IsApplyGuarding())
	//	{
	//		PSTemplate = LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, GuardPST, LoadedGuardPST);
	//		DamageSocketName = GuardDamageSocketName.IsNone() ? Target->DamageSocketName : GuardDamageSocketName;
	//	}
	//	else if (PC->IsBuffActive(EBuffType::Buff_ReflectDamage))
	//	{
	//		PSTemplate = LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, ReflectDamagePST, LoadedReflectDamagePST);
	//		DamageSocketName = ReflectDamageSocketName.IsNone() ? Target->DamageSocketName : ReflectDamageSocketName;
	//	}
	//	else if (PC->IsBuffActive(EBuffType::Buff_Shield))
	//	{
	//		PSTemplate = LoadAndCacheDamageEffectInfoTAssetSingleCommon<UParticleSystem>(InfoLoadManager, ShieldPST, LoadedShieldPST);
	//		DamageSocketName = ShieldDamageSocketName.IsNone() ? Target->DamageSocketName : ShieldDamageSocketName;
	//	}
	//}

	//if (PSTemplate == NULL)
	//{
	//	switch (WeaponType)
	//	{
	//	case EDamageWeapon::EDW_Blade:
	//	{
	//		PSTemplate = GetWeaponPSTCommon((PC != NULL) ? BladeWeaponPSTForPC : BladeWeaponPST, ElementType, 
	//			(PC != NULL) ? LoadedBladeWeaponPSTForPC : LoadedBladeWeaponPST);
	//	}
	//	break;
	//	case EDamageWeapon::EDW_Hammer:
	//	{
	//		PSTemplate = GetWeaponPSTCommon((PC != NULL) ? HammerWeaponPSTForPC : HammerWeaponPST, ElementType,
	//			(PC != NULL) ? LoadedHammerWeaponPSTForPC : LoadedHammerWeaponPST);
	//	}
	//	break;
	//	case EDamageWeapon::EDW_Magic:
	//	{
	//		PSTemplate = GetWeaponPSTCommon((PC != NULL) ? MagicWeaponPSTForPC : MagicWeaponPST, ElementType,
	//			(PC != NULL) ? LoadedMagicWeaponPSTForPC : LoadedMagicWeaponPST);
	//	}
	//	break;
	//	case EDamageWeapon::EDW_End:
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//SpawnDamageEffectInner(PSTemplate, Target, HitDir, RandomAngle, HitActorRot, DamageSocketName);
}

void UB2DamageEffectInfo::SpawnDamageEffectInner(UParticleSystem* PSTemplate, class ABladeIICharacter* Target, EHitDirection HitDir, float RandomAngle, FRotator HitActorRot, FName DamageSocketName)
{
//#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
//	if (bDisableDamageEffectParticle) {
//		return;
//	}
//#endif
//
//	if (Target == NULL || Target->GetMesh() == NULL || PSTemplate == NULL || (MaxDamageEffectNum > 0 && DamageEffectComps.Num() >= MaxDamageEffectNum))
//	{
//		return;
//	}
//
//	FTransform DamageSocketTransform = Target->GetMesh()->GetSocketTransform(DamageSocketName);
//		
//	FVector2D ScreenPos;
//
//	// Optimization to reduce hit effect
//	if (MaxDamageEffectNum > 0)
//	{
//		if (const APlayerController* PC = 0)//Cast<APlayerController*>(Target->GetController()))
//		{
//			ULocalPlayer* const LP = PC ? PC->GetLocalPlayer() : nullptr;
//			if (LP && LP->ViewportClient)
//			{
//				// get the projection data
//				FSceneViewProjectionData ProjectionData;
//				if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
//				{
//					FMatrix const ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
//					int32 ViewSizeX = 1280, ViewSizeY = 720;
//					PC->GetViewportSize(ViewSizeX, ViewSizeY);
//					FSceneView::ProjectWorldToScreen(DamageSocketTransform.GetTranslation(), FIntRect(0, 0, ViewSizeX, ViewSizeY)/*ProjectionData.GetConstrainedViewRect()*/, ViewProjectionMatrix, ScreenPos);
//
//					for (int32 i = 0; i < DamageEffectScreenPos.Num(); ++i)
//					{
//						const FVector2D& PrevPos = DamageEffectScreenPos[i];
//						if (FVector2D::DistSquared(PrevPos, ScreenPos) < FMath::Square(DamageEffectRange))
//						{
//							// Kill previous one
//							if (DamageEffectComps[i])
//							{
//								DamageEffectComps[i]->DestroyComponent();
//								OnParticleSystemFinished(DamageEffectComps[i]);
//							}
//							else
//							{
//								DamageEffectComps.RemoveAt(i);
//								DamageEffectScreenPos.RemoveAt(i);
//							}
//							break;
//						}
//					}
//				}
//			}
//			else
//			{
//				// Culling
//				return;
//			}
//		}
//	}
//
//	FRotator DamageRot = HitActorRot;
//	if (HitDir == EHitDirection::EHD_None) // Use Default Value
//	{
//		DamageRot = DamageSocketTransform.Rotator();
//
//		if (bUseRandomRotation)
//		{
//			DamageRot.Yaw += FMath::FRandRange(-RandRange, RandRange);
//		}
//	}
//	else if (HitDir != EHitDirection::EHD_HitActor) // 
//	{
//		switch (HitDir)
//		{
//		case EHitDirection::EHD_NegX:
//			DamageRot.Yaw += 180.f;
//			break;
//		case EHitDirection::EHD_PosY:
//			DamageRot.Yaw += 90.f;
//			break;
//		case EHitDirection::EHD_NegY:
//			DamageRot.Yaw -= 90.f;
//			break;
//		default:
//			break;
//		}
//		if (RandomAngle > 0.f)
//		{
//			DamageRot.Yaw += FMath::FRandRange(-RandomAngle, RandomAngle);
//		}
//	}
//
//	// 180档 倒妨临仇甸
//	ABladeIIPlayer* PC = Cast<ABladeIIPlayer>(Target);
//
//	if (PC && (PC->IsApplyGuarding() || PC->IsBuffActive(EBuffType::Buff_ReflectDamage)))
//	{
//		DamageRot.Yaw += 180;
//	}
//
//	UParticleSystemComponent* NewComponent =
//		UGameplayStatics::SpawnEmitterAtLocation(Target, PSTemplate, DamageSocketTransform.GetTranslation(), DamageRot, true);
//
//	if (NewComponent && MaxDamageEffectNum > 0)
//	{
//		NewComponent->OnSystemFinished.AddUniqueDynamic(this, &UB2DamageEffectInfo::OnParticleSystemFinished);
//
//		// Screen Pos data handling
//		int32 Idx = DamageEffectComps.Add(NewComponent);
//		if (Idx != INDEX_NONE)
//		{
//			if (DamageEffectScreenPos.Num() <= Idx)
//			{
//				DamageEffectScreenPos.AddDefaulted(Idx + 1 - DamageEffectScreenPos.Num());
//			}
//			DamageEffectScreenPos[Idx] = ScreenPos;
//		}
//	}
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
	//if (Target && Target->CurrentOverridenMatType == ECharacterMaterialOverrideType::ECMO_None)
	//{
	//	if (Target->IsBuffActive(EBuffType::Buff_Shield))
	//		return;

	//	checkSlow(HitFlashColorParamValues.Num() == (int32)EDamageElement::EDE_End && HitFlashColorParamValuesForPC.Num() == (int32)EDamageElement::EDE_End)
	//	int32 DamageColorIdx = 0;
	//	// Check State for Damage Effect, priority Frozen > Burning > Poison
	//	if (Target->IsBuffActive(EBuffType::Debuff_Freezing))
	//	{
	//		DamageColorIdx = 1;
	//	}
	//	else if (Target->IsBuffActive(EBuffType::Debuff_Burning))
	//	{
	//		DamageColorIdx = 2;
	//	}
	//	else if (Target->IsBuffActive(EBuffType::Debuff_Poison))
	//	{
	//		DamageColorIdx = 3;
	//	}
	//	ABladeIIPlayer* PC = Cast<ABladeIIPlayer>(Target);
	//	// Hit Flash Effect
	//	Target->FlashForDuration(HitFlashColorParamName, (PC != NULL) ? HitFlashColorParamValuesForPC[DamageColorIdx] : HitFlashColorParamValues[DamageColorIdx],
	//		HitFlashIntensityParamName, (PC != NULL) ? HitFlashIntensityParamValueForPC : HitFlashIntensityParamValue, (PC != NULL) ? HitFlashDurationForPC : HitFlashDuration);
	//}
}

static void SpawnPreRenderParticleComponentsCommon(ASkeletalMeshActor* InOutOwnerSKActor, UParticleSystem* InFxTemplate)
{
	if (InOutOwnerSKActor && InFxTemplate) {
		// UGameplayStatics::SpawnEmitterAttached 措脚俊 夸吧肺.
		//UB2PreRenderer::SpawnDummyPSCForPreRender(InFxTemplate, InOutOwnerSKActor->GetRootComponent());
	}
}
// BladeIIGameMode 狼 PreRender 风凭狼 老何肺.. InfoAsset 吝俊辑 牢霸烙 己瓷俊 康氢阑 固摹绰 吧 榜扼辑 鞍捞 贸府.
void UB2DamageEffectInfo::SetupPreRenderSKMeshActorForFX(ASkeletalMeshActor* InOutSKActorToSetup)
{
	if (!InOutSKActorToSetup) { // SkeletalMeshActor 绰 GameMode 率俊辑 积己秦辑 霖促. Pre-render 扁夯 困摹绰 弊率俊辑 包府窍骨肺.
		return;
	}

	// 窍唱笛 窍唱笛..
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
			// 咯扁肺 逞绢柯 InOutSkActorsToSetup 狼 俺荐绰 GetDesiredPreRenderDummyNumForMtrlOverride 父怒阑 狼档..
			if (InOutSkActorsToSetup.IsValidIndex(ActualLoadedMI)) // InOutSkActorsToSetup 乐绰 措肺 CharacterMaterialOverrides 窍唱究 技泼秦 淋.
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
		if (!ThisMatOverride.ToString().IsEmpty()){ // 角力 肺爹鳖瘤 窍绰 扒 咯扁辑 绢匡府瘤绰 臼绊..
			++MI;
		}
	}
	return MI;
}

/** For preventing in-game loading spike, possibly for specific platform only */
void UB2DamageEffectInfo::PreloadAndCacheAllTAsset()
{
#if !UE_BUILD_SHIPPING
	extern bool gPreloadCombatEffectResources; // 捞霸 己瓷俊 角力肺 倔付唱 扁咯窍绰瘤 抛胶飘甫 困秦
	if (!gPreloadCombatEffectResources) {
		return;
	}
#endif

#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] 老窜 iOS 绰 力寇窍绊 促弗 敲阀汽甸俊辑父
	if (GIsEditor) {
		return; // 俊叼磐搁 秦辑 救瞪 芭 绝变 茄单 肺爹 矫埃 炼陛捞扼档 歹 吧副 抛聪 弊成 府畔. 弊府绊 宝洒 LoadedPtr 肺 serialize 瞪 咯瘤甫 炼陛捞扼档 歹 犬角洒 瞒窜 せ
	}

	B2_SCOPED_TRACK_LOG(TEXT("UB2DamageEffectInfo::PreloadAndCacheAllTAsset"));

	// TAsset 俊辑 肺爹秦辑 Loaded~~ 俊 持绢辑 GC 规瘤.
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
	// 捞抚何磐啊 粱 吝备抄规牢单 扁夯利栏肺绰 Unload 啊 狼档茄 扁瓷烙.
	// 窍促焊聪 鞘夸茄 巴阑 RootSet 俊 器窃矫虐绰 巴档.. 绢瞒乔 鞘夸茄 巴甸 弥措茄 肺爹登绢 乐阑 捞码捞 利例且 淀秦辑.

	if(!ShouldUnloadAllAssetOnLevelTransition())
	{ // 弊促瘤 绢匡府瘤 臼瘤父 泅 备炼肺辑绰 捞码捞 利例茄 厘家老 淀..
		ConditionalMarkLoadedAssetsRootSet();
	}

	// 辟单 某浆绰 咯窍埃 厚款促.. 弊霸 救傈且 淀. 风飘悸捞搁 绢瞒乔 促澜俊 促矫 肺爹 救 且 巴烙.
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
		// 咯扁辑 肺爹窍绰 芭 酒丛. 肺爹等 巴父 付农

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
		{// 函荐 狼固何磐 "葛滴"牢瘤 "老何" 牢瘤 迭洒 乐瘤绰 臼绊 窍唱父 AddToRoot 登搁 技泼. 泅犁 荤侩格利捞 弊府 枚历茄 墨款飘啊 鞘夸茄 扒 酒丛.
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
	//const int32 CastedIndex = static_cast<int32>(MatType);
	//if (Target && CharacterMaterialOverrides.Num() > CastedIndex)
	//{
	//	// Cache old materials and set to new material
	//	Target->OverrideMaterial(
	//		LoadAndCacheDamageEffectInfoTAssetArrayCommon<UMaterialInterface>(InfoLoadManager, CharacterMaterialOverrides[CastedIndex], CastedIndex, LoadedCharacterMaterialOverrides),
	//		MatType);
	//}
}

void UB2DamageEffectInfo::PlayHitSound(UObject* WorldContextObject, ECharacterType CharType, FVector Location, bool IsAboradMob /*= false*/)
{
//#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
//	if (bDisableDamageEffectSound) {
//		return;
//	}
//#endif
//
//	int32 CueIndex = INDEX_NONE;
//	switch (CharType)
//	{
//	case ECharacterType::ECT_Assassin:
//		CueIndex = (int32)EHitSoundCase::EHit_Assassin;
//		break;
//	case ECharacterType::ECT_Gladiator:
//		CueIndex = (int32)EHitSoundCase::EHit_Gladiator;
//		break;
//	case ECharacterType::ECT_Wizard:
//		CueIndex = (int32)EHitSoundCase::EHit_Wizard;
//		break;
//	case ECharacterType::ECT_Fighter:
//		CueIndex = (int32)EHitSoundCase::EHit_Fighter;
//		break;
//	case ECharacterType::ECT_MeleeMob:
//	case ECharacterType::ECT_RangeMob:
//	case ECharacterType::ECT_MiddleMob:
//		{
//			if (IsAboradMob)
//			{
//				CueIndex = (int32)EHitSoundCase::EHit_AboardMob;
//			}
//			else
//			{
//				CueIndex = (int32)EHitSoundCase::EHit_Mob;
//			}
//		}
//		break;
//	case ECharacterType::ECT_BossMob:
//		CueIndex = (int32)EHitSoundCase::EHit_Boss;
//		break;
//	default:
//		break;
//	}
//
//	if (CueIndex >= 0 && HitSoundCues.Num() > CueIndex)
//	{
//		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, HitSoundCues[CueIndex], Location);
	//}
}

void UB2DamageEffectInfo::PlayGuardHitSound(UObject* WorldContextObject, FVector Location)
{
	//int32 CueIndex = (int32)EHitSoundCase::EHit_Guard;
	//
	//if (HitSoundCues.IsValidIndex(CueIndex))
	//{
	//	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, HitSoundCues[CueIndex], Location);
	//}
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