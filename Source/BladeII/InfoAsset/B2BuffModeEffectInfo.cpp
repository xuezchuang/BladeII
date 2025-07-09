// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#include "BladeII.h"
#include "B2BuffModeEffectInfo.h"
#include "BladeIICharacter.h"
#include "Kismet/GameplayStatics.h"
//#include "B2DamageEffectInfo.h"
#include "Event.h"
#include "BladeIIUtil.h"

#define RESTORE_BUFF_MODE_TEXT_EFFECT_3DFX 0 // Ȥ �ٽ� ��� ������ �𸣴� ���ܵδ� ��. �̰� 1 �� ���� ����ʿ� �ּ��� ���� �͵� ������Ŵ. ���� ���ҽ� ���õ�.

bool FB2BuffEffectData::MarkLoadedAssetRootSet()
{
	bool bAddedAnyone = false;
	if (ParticleSytem.IsValid()) // ���⼭ �ε��� ������ �ʰ� Root �� �ֱ⸸.
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
	// �� ���⼭ �ε� �� �ص� �̹� Valid �� ���·� ���� ���� �ִ�.
	LoadedBasicParticleSytem = InSetup.ParticleSytem.Get();
	if (bLoadEndPS && !InSetup.EndParticleSytem.IsValid() && !InSetup.EndParticleSytem.IsNull())
	{
		InLoadManager.LoadSynchronous(InSetup.EndParticleSytem);		
	}
	LoadedEndParticleSytem = InSetup.EndParticleSytem.Get();
}

// ���� �������� ���� ShouldUnloadAllAssetOnLevelTransition ����
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
	// LoadEditorData ���� �ϴ� �Ͱ� ���� �ƶ�. �̰� �� �� �ܼ��� ����..

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
//	extern bool gPreloadCombatEffectResources; // �̰� ���ɿ� ������ �󸶳� �⿩�ϴ��� �׽�Ʈ�� ����
//	if (!gPreloadCombatEffectResources) {
//		return;
//	}
//#endif
//
//#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] �ϴ� iOS �� �����ϰ� �ٸ� �÷����鿡����
//	if (GIsEditor) {
//		return; // �����͸� �ؼ� �ȵ� �� ���� �ѵ� �ε� �ð� �����̶� �� �ɸ� �״� �׳� ����. �׸��� ���� LoadedPtr �� serialize �� ������ �����̶� �� Ȯ���� ���� ��
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
//	// ResurrectBuffTextUIEffectSetup �̰� �״� �� �ʿ� ���� �� ���Ƽ� �н�.. ��;
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
	// �̸����Ͱ� �� �߱������ε� �⺻�����δ� Unload �� �ǵ��� �����.
	// �ϴٺ��� �ʿ��� ���� RootSet �� ���Խ�Ű�� �͵�.. ������ �ʿ��� �͵� �ִ��� �ε��Ǿ� ���� ������ ������ ���ؼ�.
	if (!ShouldUnloadAllAssetOnLevelTransition())
	{
		ConditionalMarkLoadedAssetsRootSet();
	}
	// �ٵ� ĳ���� ���ϰ� ����.. �װ� ������ ��. ��Ʈ���̸� ������ ������ �ٽ� �ε� �� �� ����.
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
	//	// ���⼭ �ε��ϴ� �� �ƴ�. �ε��� �͸� ��ũ

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
	//	{ // ���� �ǹ̺��� "���"���� "�Ϻ�" ���� ���� ������ �ʰ� �ϳ��� AddToRoot �Ǹ� ����. ���� �������� �׸� ö���� ī��Ʈ�� �ʿ��� �� �ƴ�.
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
	if (Target == NULL || Target->GetMesh() == NULL || !BuffMaterialParams.Contains(BuffType))
	{
		return false;
	}
	
	Target->ResetEmissive();
	EmissiveOverride(Target, BuffMaterialParams[BuffType]);
	Target->bEmissiveActiveByBuff = true;

	return true;
}

void UB2BuffModeEffectInfo::ResetBuffMaterial(class ABladeIICharacter* Target)
{
	Target->ResetEmissive();
	Target->bEmissiveActiveByBuff = false;
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
	// ����� ���ٸ� �߰� �Ķ���Ͱ� ���µ� ������ Break �ΰ� � ���� �߰� �Ķ���Ͱ� ���� ����..
	if (InTarget == NULL || InTarget->GetMesh() == NULL){
		return;
	}	
	//SpawnTextInner2D(InTarget, BreakTextUIEffectSetup, LoadedBreakTextUIEffectSetup);
}

// FloatingDamageNum ������ ��. �� ����� �� �𸣴� ���ܴ� �д�.
//void UB2BuffModeEffectInfo::SpawnCriticalHitTextEffect(class ABladeIICharacter* InTarget)
//{
//	// ����� ���ٸ� �߰� �Ķ���Ͱ� ���µ� ������ critical hit �ΰ� � ���� �߰� �Ķ���Ͱ� ���� ����.. (EDamageNumberType)
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
	if (Target == NULL || Target->GetMesh() == NULL)
	{
		return;
	}

	for (int32 i = 0; i < Target->GetMesh()->GetNumMaterials(); ++i)
	{
		// Set Dynamic Material data
		UMaterialInterface* MaterialInterface = Target->GetOriginalMaterial(i);
		if (MaterialInterface)
		{
			UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);
			if (!DynamicMaterial) //Is it already a UMaterialInstanceDynamic
			{
				DynamicMaterial = Target->GetMesh()->CreateAndSetMaterialInstanceDynamic(i);
			}

			if (Data.ColorParamName != NAME_None)
			{
				DynamicMaterial->SetVectorParameterValue(Data.ColorParamName, Data.ColorParamValue);
			}

			for (int32 j = 0; j < Data.ScalarMaterialParams.Num(); ++j)
			{
				const auto& Param = Data.ScalarMaterialParams[j];
				DynamicMaterial->SetScalarParameterValue(Param.MaterialParamName, Param.MaterialParamValue);
			}
		}
	}
}

void UB2BuffModeEffectInfo::SpawnTextInner(class ABladeIICharacter* Target, class UParticleSystem* PST)
{
	if (PST == NULL || Target == NULL || Target->GetCapsuleComponent() == NULL)
	{
		return;
	}

	// Text effects should be bound to this, because it need to be coordinated with damage effect
	APlayerController* PC =  Cast<APlayerController>(Target->GetController());
	ULocalPlayer* const LP = PC ? PC->GetLocalPlayer() : nullptr;
	if (LP && LP->ViewportClient)
	{
		// get the projection data
		FSceneViewProjectionData ProjectionData;
		if (LP->GetProjectionData(LP->ViewportClient->Viewport,ProjectionData))
		{
			FMatrix const ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
			FMatrix const InvViewProjectionMatrix = ViewProjectionMatrix.InverseFast();

			FVector CameraRight = InvViewProjectionMatrix.TransformVector(FVector(Target->bValueForTextEffect ? 1.0f : -1.f, 0.0f, 0.0f)).GetSafeNormal();

			float RandDirX = FMath::FRandRange(-1.0f, 1.0f);
			float RandDirY = FMath::FRandRange(-1.0f, 1.0f);
			FVector2D RandDir(RandDirX, RandDirY);
			RandDir.Normalize();

			FVector LocOffsetForText = TextLeftRightOffset * CameraRight + Target->GetActorLocation();
			LocOffsetForText.X += TextRandomOffset * RandDir.X;
			LocOffsetForText.Y += TextRandomOffset * RandDir.Y;

			FPlane Result = ViewProjectionMatrix.TransformFVector4(FVector4(LocOffsetForText, 1.f));
			if (Result.W > 0.0f)
			{
				// the result of this will be x and y coords in -1..1 projection space
				const float RHW = 1.0f / Result.W;
				FPlane PosInScreenSpace = FPlane(Result.X * RHW, Result.Y * RHW, Result.Z * RHW, Result.W);

				const FVector4 RayStartProjectionSpace = FVector4(PosInScreenSpace.X, PosInScreenSpace.Y, 1.0f, 1.0f);
				const FVector4 RayEndProjectionSpace = FVector4(PosInScreenSpace.X, PosInScreenSpace.Y, 0.5f, 1.0f);

				const FVector4 HGRayStartWorldSpace = InvViewProjectionMatrix.TransformFVector4(RayStartProjectionSpace);
				const FVector4 HGRayEndWorldSpace = InvViewProjectionMatrix.TransformFVector4(RayEndProjectionSpace);
				FVector RayStartWorldSpace(HGRayStartWorldSpace.X, HGRayStartWorldSpace.Y, HGRayStartWorldSpace.Z);
				FVector RayEndWorldSpace(HGRayEndWorldSpace.X, HGRayEndWorldSpace.Y, HGRayEndWorldSpace.Z);
				// divide vectors by W to undo any projection and get the 3-space coordinate 
				if (HGRayStartWorldSpace.W != 0.0f)
				{
					RayStartWorldSpace /= HGRayStartWorldSpace.W;
				}
				if (HGRayEndWorldSpace.W != 0.0f)
				{
					RayEndWorldSpace /= HGRayEndWorldSpace.W;
				}
				const FVector RayDirWorldSpace = (RayEndWorldSpace - RayStartWorldSpace).GetSafeNormal();

				UGameplayStatics::SpawnEmitterAtLocation(Target, PST, LocOffsetForText - Target->GetCapsuleComponent()->GetScaledCapsuleRadius() * RayDirWorldSpace);
				Target->bValueForTextEffect = !Target->bValueForTextEffect;
			}

		}
	}
}

//void UB2BuffModeEffectInfo::SpawnTextInner2D(class ABladeIICharacter* Target, FFloatingAbnormalEffectSetupInfo& EffectCreateInfo, FLoadedFloatingAbnormalEffectSetupInfo& LoadedHolderRef)
//{
//	// �������� ������ DJLegacy �� UIManager ���� �ϰ� �Ǵµ� ������ ����Ʈ ��� ������ �� �ڵ��о��� �״�� ���� ���� ���⿡ ��.
//
//	// �̹� �ε��Ǿ� �ִ� ��Ȳ�̸� ���⼭ �ð��� �ɸ����� ���� ����.
//	LoadedHolderRef = FLoadedFloatingAbnormalEffectSetupInfo(EffectCreateInfo, InfoLoadManager);
//
//	if (LoadedHolderRef.LoadedTextEffectTexture) // ��͵��� �� �ִµ� ��ݾ��� �� �Ƕ��Ⱑ �����ؼ� ���.
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
//			// ���� �ε����� �ʿ� ����.
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