// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, ACTION SQUARE
#include "BladeIIWorldSettings.h"

#if WITH_EDITOR
#include "Engine.h"
#include "Editor.h"
#include "EditorBuildUtils.h"
#endif

#include "BladeIIPlayerController.h"
#include "BladeIIPlayer.h"
#include "B2ActiveCameraActor.h"
#include "B2StageManager.h"
#include "BladeIIGameMode.h"
#include "B2MonsterSpawnPool.h"
#include "B2StageEventDirector.h"
#include "BladeIIScalabilityControl.h"

#include "FB2ErrorMessage.h"
#include "FakeConfigure.h"

#if WITH_EDITOR
void FWorldDefaultPlayCameraSettings::LoadPresetValues(FWorldDefaultPlayCameraSettings& OutResult, FString UniquePresetIDString)
{
	float LoadedPitch = 0.0f;
	GConfig->GetFloat(TEXT("WorldDefaultPlayCameraPreset"), *FString::Printf(TEXT("%s_Pitch"), *UniquePresetIDString), LoadedPitch, GGameIni);
	OutResult.bOverrideCameraBoomPitch = true; // �̰� �׳� �� ��.
	OutResult.CameraBoomPitch = LoadedPitch;

	float LoadedYaw = 0.0f;
	GConfig->GetFloat(TEXT("WorldDefaultPlayCameraPreset"), *FString::Printf(TEXT("%s_Yaw"), *UniquePresetIDString), LoadedYaw, GGameIni);
	OutResult.bOverrideCameraBoomYaw = true; // �̰� �׳� �� ��.
	OutResult.CameraBoomYaw = LoadedYaw;

	float LoadedRoll = 0.0f;
	GConfig->GetFloat(TEXT("WorldDefaultPlayCameraPreset"), *FString::Printf(TEXT("%s_Roll"), *UniquePresetIDString), LoadedRoll, GGameIni);
	OutResult.bOverrideCameraBoomRoll = true; // �̰� �׳� �� ��.
	OutResult.CameraBoomRoll = LoadedRoll;

	float LoadedArmLength = 0.0f;
	GConfig->GetFloat(TEXT("WorldDefaultPlayCameraPreset"), *FString::Printf(TEXT("%s_ArmLength"), *UniquePresetIDString), LoadedArmLength, GGameIni);
	OutResult.bOverrideCameraBoomArmLength = true; // �̰� �׳� �� ��.
	OutResult.CameraBoomArmLength = LoadedArmLength;
}
void FWorldDefaultPlayCameraSettings::ApplyPresetValues(FWorldDefaultPlayCameraSettings& InPreset)
{
	// Preset ����ÿ��� bOverride** ���� ��� ����
	bOverrideCameraBoomPitch = true;
	CameraBoomPitch = InPreset.CameraBoomPitch;
	
	bOverrideCameraBoomYaw = true;
	CameraBoomYaw = InPreset.CameraBoomYaw;

	bOverrideCameraBoomRoll = true;
	CameraBoomRoll = InPreset.CameraBoomRoll;

	bOverrideCameraBoomArmLength = true;
	CameraBoomArmLength = InPreset.CameraBoomArmLength;	

	bOverrideCameraTargetFocusOffset = true;
	CameraTargetFocusOffset = InPreset.CameraTargetFocusOffset;

	bCameraBoomLockedViewTarget = InPreset.bCameraBoomLockedViewTarget;
}
#endif

FName FCombinedUniqueSPWaveID::WaveInfoToKeyName(int32 InClientStageId, int32 InTargetDifficulty, int32 InTargetWave)
{
	// ó������ int32 ������ �������� �ʴ� �� �¾��� �� ���ŷӰ� �Ӽ�â���� Ȯ���ϱ⵵ ���ŷο���..
	return FName(
		*FString::Printf(TEXT("%d_%d_%d"), InClientStageId, InTargetDifficulty, InTargetWave)
	);
}

ABladeIIWorldSettings::ABladeIIWorldSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EditorStageNumber = 1;
	EditorStageDifficulty = (int32)EStageDifficulty::ESD_Hard;
	bDevTestScenarioMode = true;
	TransientDefaultCameraPreset = EWorldDefaultPlayCameraPreset::EWDCP_End;
}

void ABladeIIWorldSettings::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	if (LocalizedAreaNameKey != NAME_None && LocalizedAreaSubNameKey == NAME_None)
	{ // LocalizedAreaSubName �� �ڴʰ� �����ؼ� �̷��� ������ ������ �� �����ٸ� �ڵ����� �ֵ��� ��.
		LocalizedAreaSubNameKey = FName(*(LocalizedAreaNameKey.ToString() + TEXT("_Sub")));
	}

	if (GIsEditor)
	{ // �� ���� �Ǽ��ϴ� ����. �� �̻� ���� ���ϰ� World PostLoad ����
		// ���� ��Ʈ���� ���� ���� ��쿣 P �ʿ�����.
		UWorld* TheWorld = GetWorld();
		//UPackage* PMapPackage = (TheWorld && TheWorld->PersistentLevel) ? TheWorld->PersistentLevel->GetOutermost() : NULL;
		//const bool bAmIPMap = (PMapPackage && this->GetOutermost() == PMapPackage); P Map ������ üũ�Ϸ� �ߴµ� �ȸ���.
		if (TheWorld && !TheWorld->IsPlayInEditor())
		{
			AB2StageEventDirector::CheckAndWarnForComponentTriggerSetup(TheWorld);
		}
	}
#endif
}

void ABladeIIWorldSettings::NotifyBeginPlay()
{
	// BladeIIGameMode �� StageManager �� PreBeginPlay ���ٴ� ���������� ���ϰ� �ٸ� Actor ���� �ʹ� BeginPlay ���ٴ� �ռ� �Ҹ��� ����.

	Super::NotifyBeginPlay();
}

void ABladeIIWorldSettings::BeginPlay()
{
	Super::BeginPlay();
}

void ABladeIIWorldSettings::PreSave(FObjectPreSaveContext SaveContext)
{
//	Super::PreSave(SaveContext);
//#if WITH_EDITOR
//	if (GIsEditor && !IsRunningCommandlet())
//	{
//		// �ڵ����� �������ַ��� �ߴµ� ������ ���� �Ʒ��� ���� ������ ������ �ȵǴ� ���� ��Ȳ�� �ִ� �� ����.
//		// ���� �ӹ��ؼ� ���� ������ �ſ� ����ϴ� ������ �ڵ� ���� ������.
//		//UWorld* TheWorld = GetWorld();
//		//if (TheWorld && TheWorld->GetNavigationSystem() && TheWorld->GetNavigationSystem()->IsNavigationDirty())
//
//		// ��� �ڵ� ����Ǵ� �����̶� ��Ÿ �����Ǵ� �Ϻ� ���̽� ������ �ɷ�����..
//		UPackage* MapPackage = GetOutermost();
//		UWorld* TheWorld = GetWorld();
//		if (MapPackage && TheWorld && TheWorld->GetLevels().Num() > 0)
//		{
//			FString MapPackageFullPath = MapPackage->GetPathName();
//			if (!MapPackageFullPath.Contains(TEXT("Autosaves")) && 
//				!MapPackageFullPath.Contains(TEXT("LVCH_")) && 
//				!MapPackageFullPath.Contains(TEXT("Lobby")))
//			{
//				bool bWasUnattended = FApp::IsUnattended();
//				const TCHAR* CommandLineBackup = FCommandLine::Get();
//				//if (!bWasUnattended){ // �����Ϸ��� �ϴµ� �°� �޽������� �߰� �Ǵ� �װ� �� ���ַ��� �ߴµ� �̰ɷ� �ȸ���..
//				//	FCommandLine::Append(TEXT(" -UNATTENDED"));
//				//}
//
//				UE_LOG(LogEngine, Log, TEXT("Auto re-build navigation of %s at PreSave.."), *MapPackageFullPath);
//				FEditorBuildUtils::EditorBuild(GetWorld(), FBuildOptions::BuildAIPaths);
//
//				//if (!bWasUnattended){
//				//	FCommandLine::Set(CommandLineBackup);
//				//}
//			}
//		}
//
//		if (!EditorFakeConfigure::GetInstance().HavePowerToSave() && *PreviousDefaultGameMode != nullptr)
//		{
//			DefaultGameMode = PreviousDefaultGameMode;
//		}
//	}
//
//	// �÷��̾� ��ŸƮ ������üũ
//	for (FActorIterator It(GetWorld()); It; ++It)
//	{
//		APlayerStart* pPlayerStart = Cast<APlayerStart>(*It);
//
//		if(pPlayerStart &&
//			(pPlayerStart->GetActorScale3D().X != 1.0f
//			|| pPlayerStart->GetActorScale3D().Y != 1.0f
//			|| pPlayerStart->GetActorScale3D().Z != 1.0f)
//			)
//		{
//#if !PLATFORM_MAC
//			UE_LOG(LogEngine, Log, TEXT("Check!! PlayerStart Scale!!"));
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("PlayerStart�� �����ϰ��� ����Ǿ����ϴ�.\nX Y Z ��� 1.0���� �������ּ���.")));
//			pPlayerStart->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
//#endif
//		}
//	}
//
//	// SkeletalMeshComponent �鿡 MID �� overriding �Ǿ� ������ clear �ϴµ� ����� ������ �ִ�. AB2StageEventDirector::SafetyProcessMatineeSkActors
//	// �ٵ� ���ϱ� StageEventDirector ���� �ٸ� ��η� ���õǴ� �� �־ �� ��ü.. �̰� �׳� �δ°� ����..
//	for (FActorIterator It(GetWorld()); It; ++It)
//	{
//		ASkeletalMeshActor* CastedSKActor = Cast<ASkeletalMeshActor>(*It);
//		USkeletalMeshComponent* SKMeshComp = CastedSKActor ? CastedSKActor->GetSkeletalMeshComponent() : NULL;
//
//		if (CastedSKActor && SKMeshComp)
//		{
//			for (int32 MI = 0; MI < SKMeshComp->OverrideMaterials.Num(); ++MI)
//			{
//				UMaterialInstanceDynamic* OverriddenMID = Cast<UMaterialInstanceDynamic>(SKMeshComp->OverrideMaterials[MI]);
//				if (OverriddenMID)
//				{
//					SKMeshComp->SetMaterial(MI, NULL);
//				}
//			}
//		}
//	}
//#endif
}

void ABladeIIWorldSettings::InitDefaultPlayerCameraSetting()
{
	//// �̹� ����Ǳ� ������ ActiveCameraActor �� �ִٸ� DefaultPlayerCamera �� �����ؼ� �ȵ�.
	//// FActorIterator �� ���� ��� GameMode �� ���� �˻�
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//const bool bAnyActiveCameraActorActivated = (B2GM && B2GM->GetCurrentActiveActiveCameraActor());

	//if (bAnyActiveCameraActorActivated == false)
	//{
	//	// ���� ���� �ʱ� ����.
	//	ApplyDefaultPlayerCameraSetting(Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this)), 0.0f);
	//}
}

void ABladeIIWorldSettings::ApplyDefaultPlayerCameraSetting(class ABladeIIPlayerController* InB2PC, float BlendTime, EViewTargetBlendFunction BlendFunc, bool bNoSetViewTarget)
{
	//ABladeIIPlayer* B2Player = InB2PC ? Cast<ABladeIIPlayer>(InB2PC->GetPawn()) : NULL;

	//if (InB2PC && B2Player)
	//{
	//	// Override �� ���� ��� ����� Ŭ���� ����Ʈ 
	//	ABladeIIPlayer* B2PlayerDefault = Cast<ABladeIIPlayer>(B2Player->GetClass()->GetDefaultObject());

	//	// ������ override �� ���� ����
	//	if (DefaultPlayerCameraSettings.bOverrideCameraBoomPitch)
	//	{
	//		InB2PC->SetCameraBoomPitch(DefaultPlayerCameraSettings.CameraBoomPitch);
	//	}
	//	else if (B2PlayerDefault)
	//	{
	//		InB2PC->SetCameraBoomPitch(B2PlayerDefault->CameraBoom->RelativeRotation.Pitch);
	//	}
	//		
	//	if (DefaultPlayerCameraSettings.bOverrideCameraBoomYaw)
	//	{
	//		InB2PC->SetCameraBoomYaw(DefaultPlayerCameraSettings.CameraBoomYaw);
	//	}
	//	else if (B2PlayerDefault)
	//	{
	//		InB2PC->SetCameraBoomYaw(B2PlayerDefault->CameraBoom->RelativeRotation.Yaw);
	//	}

	//	if (DefaultPlayerCameraSettings.bOverrideCameraBoomRoll)
	//	{
	//		InB2PC->SetCameraBoomRoll(DefaultPlayerCameraSettings.CameraBoomRoll);
	//	}
	//	else if (B2PlayerDefault)
	//	{
	//		InB2PC->SetCameraBoomRoll(B2PlayerDefault->CameraBoom->RelativeRotation.Roll);
	//	}

	//	if (DefaultPlayerCameraSettings.bOverrideCameraBoomArmLength)
	//	{
	//		InB2PC->SetCameraBoomArmLength(GetExtraAppliedCameraDistance(
	//			DefaultPlayerCameraSettings.CameraBoomArmLength, DefaultPlayerCameraSettings.CameraBoomArmLength_Extra
	//		));
	//	}
	//	else if (B2PlayerDefault)
	//	{
	//		InB2PC->SetCameraBoomArmLength(B2PlayerDefault->CameraBoom->TargetArmLength);
	//	}

	//	if (DefaultPlayerCameraSettings.bOverrideCameraTargetFocusOffset)
	//	{
	//		InB2PC->SetCameraTargetFocusOffset(DefaultPlayerCameraSettings.CameraTargetFocusOffset);
	//	}
	//	else if (B2PlayerDefault)
	//	{
	//		InB2PC->SetCameraTargetFocusOffset(FVector::ZeroVector);
	//	}

	//	InB2PC->SetCameraBoomLockedViewTarget(DefaultPlayerCameraSettings.bCameraBoomLockedViewTarget);

	//	InB2PC->SetDefaultCameraRotator();

	//	if (!bNoSetViewTarget) // bNoSetViewTarget �� false �� �ִ� ���� CameraBoom �� �������� ������ Viewtarget �� ��� ������ �𸣴� ��Ȳ.
	//	{
	//		// Override �� ������ �⺻ �������Ʈ�� ���ǵ� top-down ī�޶�� ���ư�����.
	//		// �� �ѹ� ������ �� ����. ���� ViewTarget �� PlayerCharacter �� ��쵵 ������ �۵��ϵ��� �Ϸ��� ForceFromLastView ���.
	//		InB2PC->SetViewTargetWithBlend(B2Player, BlendTime, BlendFunc, 0.0f, false, true);
	//	}
	//}
}

void ABladeIIWorldSettings::ApplyDefaultPlayerCameraSettingForPlayerPawn(class ABladeIIPlayer* targetPlayer)
{
	targetPlayer->UpdateCameraPitch(DefaultPlayerCameraSettings.CameraBoomPitch);
	targetPlayer->UpdateCameraYaw(DefaultPlayerCameraSettings.CameraBoomYaw);
	targetPlayer->UpdateCameraDistance(DefaultPlayerCameraSettings.CameraBoomArmLength);
	targetPlayer->UpdateCameraTargetFocusOffset(DefaultPlayerCameraSettings.CameraTargetFocusOffset);
	targetPlayer->UpdateCameraBoomLockedViewTarget(DefaultPlayerCameraSettings.bCameraBoomLockedViewTarget);
}

void ABladeIIWorldSettings::ApplyPlayerLightSettingForPlayerPawn(class ABladeIIPlayer* targetPlayer)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIWorldSettings::ApplyPlayerLightSettingForPlayerPawn"));
	//
	//ABladeIIPlayer* AssumedLocalPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//// ����Ʈ�� �̷� �ֵ� ���� Local player character ��. GetLocalPlayerCharacter �� NULL �� �����ϴ� ������ ���� ���� ����.
	//if (targetPlayer && (!AssumedLocalPlayer || AssumedLocalPlayer == targetPlayer))
	//{
	//	UPointLightComponent* PlayerAttachedLight = targetPlayer ? targetPlayer->LightComponent : NULL;
	//	if (PlayerAttachedLight)
	//	{
	//		if (PlayerLightSettings.bOverrideIntensity)
	//		{
	//			PlayerAttachedLight->SetIntensity(PlayerLightSettings.Intensity);
	//		}
	//		if (PlayerLightSettings.bOverrideLightColor)
	//		{
	//			PlayerAttachedLight->SetLightColor(PlayerLightSettings.LightColor);
	//		}
	//		PlayerAttachedLight->MarkRenderStateDirty();
	//		FlushRenderingCommands(); // �̷� �� �ִ��� ���� ����.
	//	}
	//}
}

FText ABladeIIWorldSettings::GetLocalizedAreaNameText()
{
	return BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, LocalizedAreaNameKey.ToString());
}

FText ABladeIIWorldSettings::GetLocalizedAreaSubNameText()
{
	return BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, LocalizedAreaSubNameKey.ToString());
}

bool ABladeIIWorldSettings::IsDynamicShadowDisallowedWave(int32 InCurrClientStage, int32 InCurrDifficulty, int32 InCheckWaveNum) const
{
	// Static lighting ȯ�濡�� dynamic shadow �� ������ ������ �Ǵ� ��찡 �־ wave ���� �� �� �ֵ��� �ϴ� ���� ����..
	// TurnOffMeshComponentsDyamicShadowForModulated ����.
	// SpawnPool �� Difficulty ������ int �� ���⵵.. �Ф�
	FCombinedUniqueSPWaveID CheckID(InCurrClientStage, InCurrDifficulty, InCheckWaveNum);
	// �׷� Ű���� �ִ����� ����. bool Value ���� �߿����� ����.
	return (DynamicShadowExcludeWaveNameKeys.Find(CheckID.GetMyKeyName()) != nullptr);
}

#if WITH_EDITOR
FString ABladeIIWorldSettings::GetIniKeyStringPrefixOfDefaultPresetEnum(EWorldDefaultPlayCameraPreset InPresetEnum)
{	
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWorldDefaultPlayCameraPreset"), true);
	//if (EnumPtr)
	//{
	//	FString PresetEnumName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPresetEnum));

	//	// ���̹� ���� �¾ƾ� �ǰڴ�. �׷��ٰ� CheckPCClassEnumNamingConvention ó�� �� �ʿ������..
	//	if (PresetEnumName.RemoveFromStart(EWORLDDEFAULTCAMERAPRESET_COMMON_PREFIX) == true)
	//	{
	//		return PresetEnumName;
	//	}
	//}

	return TEXT("UnknownPreset");
}

void ABladeIIWorldSettings::PreEditChange(FProperty* PropertyThatWillChange)
{
	Super::PreEditChange(PropertyThatWillChange);

#if WITH_EDITOR
	const FName Name_DefaultGameMode = GET_MEMBER_NAME_CHECKED(AWorldSettings, DefaultGameMode);

	FName PropertyName = PropertyThatWillChange != NULL ? PropertyThatWillChange->GetFName() : NAME_None;

	if (PropertyName == Name_DefaultGameMode && *PreviousDefaultGameMode == nullptr)
	{
		PreviousDefaultGameMode = DefaultGameMode;
	}
#endif
}

void ABladeIIWorldSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_TransientDefaultCameraPreset = GET_MEMBER_NAME_CHECKED(ABladeIIWorldSettings, TransientDefaultCameraPreset);
	const FName Name_EditorStageNumber = GET_MEMBER_NAME_CHECKED(ABladeIIWorldSettings, EditorStageNumber);
	const FName Name_EditorStageDifficulty = GET_MEMBER_NAME_CHECKED(ABladeIIWorldSettings, EditorStageDifficulty);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	// TransientDefaultCameraPreset ���ý� Preset ����� �����.
	if(PropertyName == Name_TransientDefaultCameraPreset)
	{
		if (TransientDefaultCameraPreset != EWorldDefaultPlayCameraPreset::EWDCP_End)
		{
			// Preset �� �ε�
			FWorldDefaultPlayCameraSettings::LoadPresetValues(DefaultCameraPreset_StandardTopDown, GetIniKeyStringPrefixOfDefaultPresetEnum(EWorldDefaultPlayCameraPreset::EWDCP_StandardTopDown));
			FWorldDefaultPlayCameraSettings::LoadPresetValues(DefaultCameraPreset_StandardBackView, GetIniKeyStringPrefixOfDefaultPresetEnum(EWorldDefaultPlayCameraPreset::EWDCP_StandardBackView));

			if (TransientDefaultCameraPreset == EWorldDefaultPlayCameraPreset::EWDCP_StandardTopDown)
			{
				DefaultPlayerCameraSettings.ApplyPresetValues(DefaultCameraPreset_StandardTopDown);
			}
			else if (TransientDefaultCameraPreset == EWorldDefaultPlayCameraPreset::EWDCP_StandardBackView)
			{
				DefaultPlayerCameraSettings.ApplyPresetValues(DefaultCameraPreset_StandardBackView);
			}
		}
	}
	else
	{
		// ��Ÿ�� ��� �ϴ� EWDCP_End �� �������� �ٽ� �����ϰ� ������ ������ �����ϵ��� �Ѵ�.
		TransientDefaultCameraPreset = EWorldDefaultPlayCameraPreset::EWDCP_End;
	}

	if (PropertyName == Name_EditorStageNumber || PropertyName == Name_EditorStageDifficulty)
	{
		EditorStageNumber = FMath::Max(EditorStageNumber, 1);
		EditorStageDifficulty = FMath::Clamp(EditorStageDifficulty, (int32)EStageDifficulty::ESD_Normal, (int32)EStageDifficulty::ESD_Hell);

		bool bFoundStageNumberSetting = false;
		bool bFoundDifficultySetting = false;
		EStageDifficulty FallbackDifficultyLevel = EStageDifficulty::ESD_None;
		
		// MonsterSpawnPool �̶� StageEventDirector ������ ������Ʈ �ؾ� ��.
		for (FActorIterator ActorIt(GEditor->GetEditorWorldContext().World()); ActorIt; ++ActorIt)
		{
			AB2MonsterSpawnPool* SpawnPool = Cast<AB2MonsterSpawnPool>(*ActorIt);
			AB2StageEventDirector* EventDirector = Cast<AB2StageEventDirector>(*ActorIt);

			if (SpawnPool) 
			{
				const FMonsterSpawnPoolPerStageSettings* ThisStageSetting = SpawnPool->GetDesiredStageSetting(EditorStageNumber, (EStageDifficulty)EditorStageDifficulty);
				// ���� ������ �������� ��ȣ�� ���̵� ������ �°� ���� �������� ������Ʈ
				SpawnPool->RegenerateEditingActors();
				SpawnPool->EnsureHaveFixedSpawnTransforms();
				SpawnPool->RegenerateComponentNameDisplay();

				if (ThisStageSetting)
				{
					bFoundStageNumberSetting = true;
					if (ThisStageSetting->TargetDifficultyLevel == EditorStageDifficulty)
					{
						bFoundDifficultySetting = true;
					}
					else
					{
						FallbackDifficultyLevel = static_cast<EStageDifficulty>(ThisStageSetting->TargetDifficultyLevel);
					}
				}

			}
			else if (EventDirector)
			{
				EventDirector->RefreshPreviewSKComps(true);
			}
		}

		if (!bFoundStageNumberSetting)
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("���� ��ġ�� SpawnPool ���� �����Ϳ� StageNum %d ��(��) DifficultyLevel %d �� �ش��ϴ� Wave ������ ã�� �� �����ϴ�.\n\n�ش� Stage �� �� �������� ����� �� �����ϴ�."), 
					EditorStageNumber, EditorStageDifficulty
				)));
#endif
		}
		else if (!bFoundDifficultySetting) 
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("���� ��ġ�� SpawnPool ���� �����Ϳ� StageNum %d �� �ش��ϴ� ������ ã������ DifficultyLevel %d �� �ش��ϴ� Wave ������ ã�� ���Ͽ� ����� DifficultyLevel %d ������ ��� ����ϰ� �˴ϴ�."),
					EditorStageNumber, EditorStageDifficulty, static_cast<int32>(FallbackDifficultyLevel)
				)));
#endif
		}
	}
}

bool ABladeIIWorldSettings::CanEditChange(const FProperty* InProperty) const
{
	const FName Name_DevTestScenarioMode = GET_MEMBER_NAME_CHECKED(ABladeIIWorldSettings, bDevTestScenarioMode);

	if (InProperty)
	{
		FString PropertyName = InProperty->GetName();
		if (PropertyName == Name_DevTestScenarioMode.ToString())
		{ // �ó����� ���� �Ϲ� ���̵������� �۵���. ������ ������ ȥ�� ������ ���� ���� ���Ƶ�.
			return FCacheStageEssentialDataToKeep::IsScenarioModeAllowedDifficulty((EStageDifficulty)EditorStageDifficulty);
		}
	}

	return Super::CanEditChange(InProperty);
}
#endif 