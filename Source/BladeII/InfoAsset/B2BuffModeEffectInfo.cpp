// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square


#include "B2BuffModeEffectInfo.h"
//#include "BladeII.h"
#include "BladeIICharacter.h"
#include "Kismet/GameplayStatics.h"
//#include "B2DamageEffectInfo.h"
#include "Event.h"
#include "BladeIIUtil.h"
#include "GameFramework/PlayerController.h"

#define RESTORE_BUFF_MODE_TEXT_EFFECT_3DFX 0 // 趣 促矫 娟扁 唱棵瘤 葛福聪 巢败滴绰 芭. 捞芭 1 肺 初绊 庆歹率俊 林籍媚 初篮 巴档 汗备矫糯. 拱沸 府家胶 技泼档.

bool FB2BuffEffectData::MarkLoadedAssetRootSet()
{
	bool bAddedAnyone = false;
	if (ParticleSytem.IsValid()) // 咯扁辑 肺爹阑 窍瘤绰 臼绊 Root 俊 持扁父.
	{
		ParticleSytem->AddToRoot();
		bAddedAnyone = true;
	}
	if (EndParticleSytem.IsValid())
	{
		EndParticleSytem->AddToRoot();
		bAddedAnyone = true;
	}
	return bAddedAnyone;
}

FB2LoadedBuffEffectData::FB2LoadedBuffEffectData(FStreamableManager& InLoadManager, FB2BuffEffectData& InSetup, bool bLoadBasicPS, bool bLoadEndPS)
{
	if (bLoadBasicPS && !InSetup.ParticleSytem.IsValid() && !InSetup.ParticleSytem.IsNull())
	{
		InLoadManager.LoadSynchronous(InSetup.ParticleSytem);
	}
	// 怖 咯扁辑 肺爹 救 秦档 捞固 Valid 茄 惑怕肺 甸绢棵 荐档 乐促.
	LoadedBasicParticleSytem = InSetup.ParticleSytem.Get();
	if (bLoadEndPS && !InSetup.EndParticleSytem.IsValid() && !InSetup.EndParticleSytem.IsNull())
	{
		InLoadManager.LoadSynchronous(InSetup.EndParticleSytem);		
	}
	LoadedEndParticleSytem = InSetup.EndParticleSytem.Get();
}

// 流立 曼炼窍瘤 富绊 ShouldUnloadAllAssetOnLevelTransition 栏肺
bool UB2BuffModeEffectInfo::bRootsetOnLoad = false;
bool UB2BuffModeEffectInfo::bAssetsRooted = false;
UB2BuffModeEffectInfo::UB2BuffModeEffectInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TextLeftRightOffset = 90.f;
	TextRandomOffset = 30.f;

	if (GConfig)
	{
		GConfig->GetBool(TEXT("/Script/BladeII"), TEXT("BuffModeEffectInfoRootsetOnLoad"), bRootsetOnLoad, GGameIni);
	}
}

void UB2BuffModeEffectInfo::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	EditorLoadAllTAsset();
#endif
}

#if WITH_EDITOR
void UB2BuffModeEffectInfo::EditorLoadAllTAsset()
{
	// LoadEditorData 俊辑 窍绰 巴苞 鞍篮 钙遏. 捞扒 粱 歹 窜鉴茄 备炼..

	if (!GIsEditor){
		return;
	}
	
	//for (FFloatingAbnormalEffectSetupInfo& ThisInfo : ResurrectBuffTextUIEffectSetup)
	//{
	//	ThisInfo.EditorOnlySimpleLoad();
	//}
}
#endif

/** For preventing in-game loading spike, possibly for specific platform only */
void UB2BuffModeEffectInfo::PreloadAndCacheAllTAsset()
{
//#if !UE_BUILD_SHIPPING
	//extern bool gPreloadCombatEffectResources; // 捞霸 己瓷俊 角力肺 倔付唱 扁咯窍绰瘤 抛胶飘甫 困秦
//	if (!gPreloadCombatEffectResources) {
//		return;
//	}
//#endif
//
//#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] 老窜 iOS 绰 力寇窍绊 促弗 敲阀汽甸俊辑父
//	if (GIsEditor) {
//		return; // 俊叼磐搁 秦辑 救瞪 芭 绝变 茄单 肺爹 矫埃 炼陛捞扼档 歹 吧副 抛聪 弊成 府畔. 弊府绊 宝洒 LoadedPtr 肺 serialize 瞪 咯瘤甫 炼陛捞扼档 歹 犬角洒 瞒窜 せ
//	}
//
//	B2_SCOPED_TRACK_LOG(TEXT("UB2BuffModeEffectInfo::PreloadAndCacheAllTAsset"));
//
//	LoadedCharacterBuffEffects.Empty();
//	for (TMap<EBuffType, FB2BuffEffectData>::TIterator ItCharBuffFx(CharacterBuffEffects); ItCharBuffFx; ++ItCharBuffFx)
//	{
//		FB2LoadedBuffEffectData& NewLoadedHolder = LoadedCharacterBuffEffects.FindOrAdd(ItCharBuffFx.Key());
//		NewLoadedHolder = FB2LoadedBuffEffectData(InfoLoadManager, ItCharBuffFx.Value(), true, true);
//	}	
//
//	// ResurrectBuffTextUIEffectSetup 捞扒 弊蹿 且 鞘夸 绝阑 芭 鞍酒辑 菩胶.. ぞ;
//
//	LoadedBreakTextUIEffectSetup = FLoadedFloatingAbnormalEffectSetupInfo(BreakTextUIEffectSetup, InfoLoadManager);
//
//	LoadedCharacterBuffTextUIEffectSetup.Empty();
//	for (TMap<EBuffType, FFloatingAbnormalEffectSetupInfo>::TIterator ItCharBuffTextUIEffect(CharacterBuffTextUIEffectSetup); ItCharBuffTextUIEffect; ++ItCharBuffTextUIEffect)
//	{
//		FLoadedFloatingAbnormalEffectSetupInfo& NewLoadedHolder = LoadedCharacterBuffTextUIEffectSetup.FindOrAdd(ItCharBuffTextUIEffect.Key());
//		NewLoadedHolder = FLoadedFloatingAbnormalEffectSetupInfo(ItCharBuffTextUIEffect.Value(), InfoLoadManager);
//	}
//	checkSlow(CharacterBuffTextUIEffectSetup.Num() == LoadedCharacterBuffTextUIEffectSetup.Num());
//#endif
}

void UB2BuffModeEffectInfo::ConditionalUnloadAllOrPreserve()
{
	// 捞抚何磐啊 粱 吝备抄规牢单 扁夯利栏肺绰 Unload 啊 狼档茄 扁瓷烙.
	// 窍促焊聪 鞘夸茄 巴阑 RootSet 俊 器窃矫虐绰 巴档.. 绢瞒乔 鞘夸茄 巴甸 弥措茄 肺爹登绢 乐阑 捞码捞 利例且 淀秦辑.
	if (!ShouldUnloadAllAssetOnLevelTransition())
	{
		ConditionalMarkLoadedAssetsRootSet();
	}
	// 辟单 某浆绰 咯窍埃 厚款促.. 弊霸 救傈且 淀. 风飘悸捞搁 绢瞒乔 促澜俊 促矫 肺爹 救 且 巴烙.
	UnloadAllTAssetCache();
}

void UB2BuffModeEffectInfo::UnloadAllTAssetCache()
{
	//LoadedCharacterBuffEffects.Empty();
	//LoadedResurrectBuffTextUIEffectSetup.Empty();
	//LoadedBreakTextUIEffectSetup.ClearLoaded();
	//LoadedCharacterBuffTextUIEffectSetup.Empty();
}

void UB2BuffModeEffectInfo::ConditionalMarkLoadedAssetsRootSet()
{
	//if (!ShouldUnloadAllAssetOnLevelTransition())
	//{
	//	// 咯扁辑 肺爹窍绰 芭 酒丛. 肺爹等 巴父 付农

	//	int32 AddedCount = 0;
	//	for (TMap<EBuffType, FB2BuffEffectData>::TIterator CBFIt(CharacterBuffEffects); CBFIt; ++CBFIt)
	//	{
	//		if (CBFIt.Value().MarkLoadedAssetRootSet())
	//		{
	//			++AddedCount;
	//		}
	//	}
	//	for (FFloatingAbnormalEffectSetupInfo& ThisInfo : ResurrectBuffTextUIEffectSetup)
	//	{
	//		if (ThisInfo.MarkLoadedAssetRootSet())
	//		{
	//			++AddedCount;
	//		}
	//	}
	//	if (BreakTextUIEffectSetup.MarkLoadedAssetRootSet())
	//	{
	//		++AddedCount;
	//	}

	//	for (TMap<EBuffType, FFloatingAbnormalEffectSetupInfo>::TIterator CBTFIt(CharacterBuffTextUIEffectSetup); CBTFIt; ++CBTFIt)
	//	{
	//		if (CBTFIt.Value().MarkLoadedAssetRootSet())
	//		{
	//			++AddedCount;
	//		}
	//	}

	//	if (AddedCount > 0)
	//	{ // 函荐 狼固何磐 "葛滴"牢瘤 "老何" 牢瘤 迭洒 乐瘤绰 臼绊 窍唱父 AddToRoot 登搁 技泼. 泅犁 荤侩格利捞 弊府 枚历茄 墨款飘啊 鞘夸茄 扒 酒丛.
	//		bAssetsRooted = true;
	//	}
	//}
}

bool UB2BuffModeEffectInfo::ShouldUnloadAllAssetOnLevelTransition()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING]
	return true;
#else
	return !bRootsetOnLoad;
#endif
}

bool UB2BuffModeEffectInfo::ApplyBuffMaterial(EBuffType BuffType, class ABladeIICharacter* Target)
{
	//if (Target == NULL || Target->GetMesh() == NULL || !BuffMaterialParams.Contains(BuffType))
	//{
	//	return false;
	//}
	//
	//Target->ResetEmissive();
	//EmissiveOverride(Target, BuffMaterialParams[BuffType]);
	//Target->bEmissiveActiveByBuff = true;

	return true;
}

void UB2BuffModeEffectInfo::ResetBuffMaterial(class ABladeIICharacter* Target)
{
	//Target->ResetEmissive();
	//Target->bEmissiveActiveByBuff = false;
}

UParticleSystemComponent* UB2BuffModeEffectInfo::CharacterBuffEffect(EBuffType BuffType, class ABladeIICharacter* Target, bool bEndEffect)
{
	/*const bool IsValidTarget = Target && Target->GetMesh();
	const bool IsValidBuff	 = BuffType != EBuffType::Buff_None && BuffType != EBuffType::Buff_End;

	if (IsValidTarget && IsValidBuff)
	{
		auto* EffectTemplate = CharacterBuffEffects.Find(BuffType);
		auto* ParticleObj = GetOrLoadBuffEffect(BuffType, bEndEffect);

		if (EffectTemplate && ParticleObj)
		{
			auto* RootComp = Target->GetRootComponent();
			auto* OwnerMesh = Target->GetMesh();

			const FName SocketName = EffectTemplate->SocketName;

			if (EffectTemplate->bAttach)
			{
				if (EffectTemplate->SocketName == NAME_None)
					return UGameplayStatics::SpawnEmitterAttached(ParticleObj, RootComp);
				else
					return UGameplayStatics::SpawnEmitterAttached(ParticleObj, OwnerMesh, SocketName);
			}
			else
			{
				if (EffectTemplate->SocketName == NAME_None)
				{
					return UGameplayStatics::SpawnEmitterAtLocation(GWorld, ParticleObj, Target->GetActorLocation());
				}
				else
				{
					FTransform SocketTransform = OwnerMesh->GetSocketTransform(SocketName, RTS_World);
					FVector SpawnLocation = EffectTemplate->bStickGround ? GetGroundHitLocation(SocketTransform.GetLocation(), Target) : SocketTransform.GetLocation();
					return UGameplayStatics::SpawnEmitterAtLocation(GWorld, ParticleObj, SpawnLocation);
				}
			}
			
		}
	}*/

	return nullptr;
}

UParticleSystem* UB2BuffModeEffectInfo::GetOrLoadBuffEffect(EBuffType BuffType, bool IsEndEffect)
{
	if (FB2BuffEffectData* BuffEffectTemplate = CharacterBuffEffects.Find(BuffType))
	{
		FB2LoadedBuffEffectData& LoadedData = LoadedCharacterBuffEffects.FindOrAdd(BuffType);
		LoadedData = FB2LoadedBuffEffectData(InfoLoadManager, *BuffEffectTemplate, !IsEndEffect, IsEndEffect);
		UParticleSystem* ParticleObj = IsEndEffect ? LoadedData.LoadedEndParticleSytem : LoadedData.LoadedBasicParticleSytem;

		return ParticleObj;
	}

	return nullptr;

}

void UB2BuffModeEffectInfo::SpawnCharacterBuffTextEffect(EBuffType BuffType, class ABladeIICharacter* Target)
{
	//if (Target == NULL || Target->GetMesh() == NULL || BuffType == EBuffType::Buff_End || BuffType == EBuffType::Buff_None)
	//{
	//	return;
	//}

	//FFloatingAbnormalEffectSetupInfo* pInfoData = CharacterBuffTextUIEffectSetup.Find(BuffType);
	//if (pInfoData)
	//{
	//	FLoadedFloatingAbnormalEffectSetupInfo* LoadedHolder = &(LoadedCharacterBuffTextUIEffectSetup.FindOrAdd(BuffType));
	//	SpawnTextInner2D(Target, *pInfoData, *LoadedHolder);
	//}	
}

void UB2BuffModeEffectInfo::SpawnBreakTextEffect(class ABladeIICharacter* InTarget)
{
	//// 泅犁绰 喊促弗 眠啊 颇扼固磐啊 绝绰单 穿备狼 Break 牢啊 殿俊 蝶弗 眠啊 颇扼固磐啊 积辨 荐档..
	//if (InTarget == NULL || InTarget->GetMesh() == NULL){
	//	return;
	//}	
	//SpawnTextInner2D(InTarget, BreakTextUIEffectSetup, LoadedBreakTextUIEffectSetup);
}

// FloatingDamageNum 率栏肺 皑. 肚 函版瞪 瘤 葛福聪 巢败绰 敌促.
//void UB2BuffModeEffectInfo::SpawnCriticalHitTextEffect(class ABladeIICharacter* InTarget)
//{
//	// 泅犁绰 喊促弗 眠啊 颇扼固磐啊 绝绰单 穿备狼 critical hit 牢啊 殿俊 蝶弗 眠啊 颇扼固磐啊 积辨 荐档.. (EDamageNumberType)
//
//	if (InTarget == NULL || InTarget->GetMesh() == NULL)
//	{
//		return;
//	}
//
//	SpawnTextInner2D(InTarget, PlayerCriticalHitTextUIEffectSetup);
//}

void UB2BuffModeEffectInfo::EmissiveOverride(class ABladeIICharacter* Target, const FB2MaterialParamData& Data)
{
	//if (Target == NULL || Target->GetMesh() == NULL)
	//{
	//	return;
	//}

	//for (int32 i = 0; i < Target->GetMesh()->GetNumMaterials(); ++i)
	//{
	//	// Set Dynamic Material data
	//	UMaterialInterface* MaterialInterface = Target->GetOriginalMaterial(i);
	//	if (MaterialInterface)
	//	{
	//		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);
	//		if (!DynamicMaterial) //Is it already a UMaterialInstanceDynamic
	//		{
	//			DynamicMaterial = Target->GetMesh()->CreateAndSetMaterialInstanceDynamic(i);
	//		}

	//		if (Data.ColorParamName != NAME_None)
	//		{
	//			DynamicMaterial->SetVectorParameterValue(Data.ColorParamName, Data.ColorParamValue);
	//		}

	//		for (int32 j = 0; j < Data.ScalarMaterialParams.Num(); ++j)
	//		{
	//			const auto& Param = Data.ScalarMaterialParams[j];
	//			DynamicMaterial->SetScalarParameterValue(Param.MaterialParamName, Param.MaterialParamValue);
	//		}
	//	}
	//}
}

void UB2BuffModeEffectInfo::SpawnTextInner(class ABladeIICharacter* Target, class UParticleSystem* PST)
{
	//if (PST == NULL || Target == NULL || Target->GetCapsuleComponent() == NULL)
	//{
	//	return;
	//}

	//// Text effects should be bound to this, because it need to be coordinated with damage effect
	//APlayerController* PC =  Cast<APlayerController>(Target->GetController());
	//ULocalPlayer* const LP = PC ? PC->GetLocalPlayer() : nullptr;
	//if (LP && LP->ViewportClient)
	//{
	//	// get the projection data
	//	FSceneViewProjectionData ProjectionData;
	//	if (LP->GetProjectionData(LP->ViewportClient->Viewport,ProjectionData))
	//	{
	//		FMatrix const ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
	//		FMatrix const InvViewProjectionMatrix = ViewProjectionMatrix.InverseFast();

	//		FVector CameraRight = InvViewProjectionMatrix.TransformVector(FVector(Target->bValueForTextEffect ? 1.0f : -1.f, 0.0f, 0.0f)).GetSafeNormal();

	//		float RandDirX = FMath::FRandRange(-1.0f, 1.0f);
	//		float RandDirY = FMath::FRandRange(-1.0f, 1.0f);
	//		FVector2D RandDir(RandDirX, RandDirY);
	//		RandDir.Normalize();

	//		FVector LocOffsetForText = TextLeftRightOffset * CameraRight + Target->GetActorLocation();
	//		LocOffsetForText.X += TextRandomOffset * RandDir.X;
	//		LocOffsetForText.Y += TextRandomOffset * RandDir.Y;

	//		FPlane Result = ViewProjectionMatrix.TransformFVector4(FVector4(LocOffsetForText, 1.f));
	//		if (Result.W > 0.0f)
	//		{
	//			// the result of this will be x and y coords in -1..1 projection space
	//			const float RHW = 1.0f / Result.W;
	//			FPlane PosInScreenSpace = FPlane(Result.X * RHW, Result.Y * RHW, Result.Z * RHW, Result.W);

	//			const FVector4 RayStartProjectionSpace = FVector4(PosInScreenSpace.X, PosInScreenSpace.Y, 1.0f, 1.0f);
	//			const FVector4 RayEndProjectionSpace = FVector4(PosInScreenSpace.X, PosInScreenSpace.Y, 0.5f, 1.0f);

	//			const FVector4 HGRayStartWorldSpace = InvViewProjectionMatrix.TransformFVector4(RayStartProjectionSpace);
	//			const FVector4 HGRayEndWorldSpace = InvViewProjectionMatrix.TransformFVector4(RayEndProjectionSpace);
	//			FVector RayStartWorldSpace(HGRayStartWorldSpace.X, HGRayStartWorldSpace.Y, HGRayStartWorldSpace.Z);
	//			FVector RayEndWorldSpace(HGRayEndWorldSpace.X, HGRayEndWorldSpace.Y, HGRayEndWorldSpace.Z);
	//			// divide vectors by W to undo any projection and get the 3-space coordinate 
	//			if (HGRayStartWorldSpace.W != 0.0f)
	//			{
	//				RayStartWorldSpace /= HGRayStartWorldSpace.W;
	//			}
	//			if (HGRayEndWorldSpace.W != 0.0f)
	//			{
	//				RayEndWorldSpace /= HGRayEndWorldSpace.W;
	//			}
	//			const FVector RayDirWorldSpace = (RayEndWorldSpace - RayStartWorldSpace).GetSafeNormal();

	//			UGameplayStatics::SpawnEmitterAtLocation(Target, PST, LocOffsetForText - Target->GetCapsuleComponent()->GetScaledCapsuleRadius() * RayDirWorldSpace);
	//			Target->bValueForTextEffect = !Target->bValueForTextEffect;
	//		}

	//	}
	//}
}

//void UB2BuffModeEffectInfo::SpawnTextInner2D(class ABladeIICharacter* Target, FFloatingAbnormalEffectSetupInfo& EffectCreateInfo, FLoadedFloatingAbnormalEffectSetupInfo& LoadedHolderRef)
//{
//	// 角龙利牢 悼累篮 DJLegacy 狼 UIManager 俊辑 窍霸 登绰单 捞傈俊 捞棋飘 扁馆 备泅茄 芭 内靛菩静甫 弊措肺 静扁 困秦 咯扁俊 狄.
//
//	// 捞固 肺爹登绢 乐带 惑炔捞搁 咯扁辑 矫埃捞 吧府瘤绰 臼阑 巴烙.
//	LoadedHolderRef = FLoadedFloatingAbnormalEffectSetupInfo(EffectCreateInfo, InfoLoadManager);
//
//	if (LoadedHolderRef.LoadedTextEffectTexture) // 后巴甸捞 粱 乐绰单 夺陛绝捞 闰 魄锭扁啊 殿厘秦辑 愁恩.
//	{
//		CharacterAbnormalEffectClass<ABladeIICharacter*, const FLoadedFloatingAbnormalEffectSetupInfo&>::GetInstance().Signal(Target, LoadedHolderRef);
//	}
//}

void UB2BuffModeEffectInfo::SpawnResurrectTextEffect(EResurrectBuffType BuffType, class ABladeIICharacter* Target)
{
//#if RESTORE_BUFF_MODE_TEXT_EFFECT_3DFX
//	UParticleSystem* PST = NULL;
//#endif
//
//	FFloatingAbnormalEffectSetupInfo* TextEffectSetupInfo = nullptr;
//	FLoadedFloatingAbnormalEffectSetupInfo* LoadedInfoHolder = nullptr;
//
//	int32 InfoIndex = static_cast<int32>(BuffType) - 1; // Ignore EResurrectBuffType::RBT_None
//
//	switch (BuffType)
//	{
//	case EResurrectBuffType::RBT_Attack:
//	case EResurrectBuffType::RBT_Defense:
//	case EResurrectBuffType::RBT_Health:
//#if RESTORE_BUFF_MODE_TEXT_EFFECT_3DFX
//		PST = ResurrectBuffTextPST.Num() > InfoIndex ? ResurrectBuffTextPST[InfoIndex] : NULL;
//#endif
//
//		if (ResurrectBuffTextUIEffectSetup.IsValidIndex(InfoIndex))
//		{
//			TextEffectSetupInfo = &(ResurrectBuffTextUIEffectSetup[InfoIndex]);
//			// 悼老 牢郸胶肺 甘俊 立辟.
//			LoadedInfoHolder = &LoadedResurrectBuffTextUIEffectSetup.FindOrAdd(InfoIndex);
//		}
//		break;
//	case EResurrectBuffType::RBT_None:
//	case EResurrectBuffType::RBT_End:
//		break;
//	default:
//		break;
//	}
//
//#if RESTORE_BUFF_MODE_TEXT_EFFECT_3DFX
//	SpawnTextInner(Target, PST);
//#endif
//
//	if (TextEffectSetupInfo && LoadedInfoHolder)
//	{
//		SpawnTextInner2D(Target, *TextEffectSetupInfo, *LoadedInfoHolder);
//	}
}

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2BuffModeEffectInfo::CleanupOnPreSave()
{
	UnloadAllTAssetCache();
}
void UB2BuffModeEffectInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	CleanupOnPreSave();
}
#endif