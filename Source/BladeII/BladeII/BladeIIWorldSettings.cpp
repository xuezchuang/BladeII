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
	OutResult.bOverrideCameraBoomPitch = true; // 이건 그냥 켜 줌.
	OutResult.CameraBoomPitch = LoadedPitch;

	float LoadedYaw = 0.0f;
	GConfig->GetFloat(TEXT("WorldDefaultPlayCameraPreset"), *FString::Printf(TEXT("%s_Yaw"), *UniquePresetIDString), LoadedYaw, GGameIni);
	OutResult.bOverrideCameraBoomYaw = true; // 이건 그냥 켜 줌.
	OutResult.CameraBoomYaw = LoadedYaw;

	float LoadedRoll = 0.0f;
	GConfig->GetFloat(TEXT("WorldDefaultPlayCameraPreset"), *FString::Printf(TEXT("%s_Roll"), *UniquePresetIDString), LoadedRoll, GGameIni);
	OutResult.bOverrideCameraBoomRoll = true; // 이건 그냥 켜 줌.
	OutResult.CameraBoomRoll = LoadedRoll;

	float LoadedArmLength = 0.0f;
	GConfig->GetFloat(TEXT("WorldDefaultPlayCameraPreset"), *FString::Printf(TEXT("%s_ArmLength"), *UniquePresetIDString), LoadedArmLength, GGameIni);
	OutResult.bOverrideCameraBoomArmLength = true; // 이건 그냥 켜 줌.
	OutResult.CameraBoomArmLength = LoadedArmLength;
}
void FWorldDefaultPlayCameraSettings::ApplyPresetValues(FWorldDefaultPlayCameraSettings& InPreset)
{
	// Preset 적용시에는 bOverride** 들을 모두 켜줌
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
	// 처음부터 int32 세팅을 제공하지 않는 건 셋업이 넘 번거롭고 속성창에서 확인하기도 번거로워서..
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
	{ // LocalizedAreaSubName 을 뒤늦게 구현해서 이렇게 기존에 설정한 게 없었다면 자동으로 넣도록 함.
		LocalizedAreaSubNameKey = FName(*(LocalizedAreaNameKey.ToString() + TEXT("_Sub")));
	}

	if (GIsEditor)
	{ // 넘 자주 실수하는 설정. 더 이상 참지 못하고 World PostLoad 에서
		// 여러 스트리밍 레벨 있을 경우엔 P 맵에서만.
		UWorld* TheWorld = GetWorld();
		//UPackage* PMapPackage = (TheWorld && TheWorld->PersistentLevel) ? TheWorld->PersistentLevel->GetOutermost() : NULL;
		//const bool bAmIPMap = (PMapPackage && this->GetOutermost() == PMapPackage); P Map 에서만 체크하려 했는데 안먹힘.
		if (TheWorld && !TheWorld->IsPlayInEditor())
		{
			AB2StageEventDirector::CheckAndWarnForComponentTriggerSetup(TheWorld);
		}
	}
#endif
}

void ABladeIIWorldSettings::NotifyBeginPlay()
{
	// BladeIIGameMode 나 StageManager 의 PreBeginPlay 보다는 나중이지만 여하간 다른 Actor 들의 초반 BeginPlay 보다는 앞서 불리는 지점.

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
//		// 자동으로 빌드해주려고 했는데 어차피 지금 아래와 같은 조건이 감지가 안되는 버그 상황이 있는 거 같다.
//		// 마감 임박해서 빌드 깨지면 매우 곤란하니 무조건 자동 빌드 돌린다.
//		//UWorld* TheWorld = GetWorld();
//		//if (TheWorld && TheWorld->GetNavigationSystem() && TheWorld->GetNavigationSystem()->IsNavigationDirty())
//
//		// 대신 자동 저장되는 파일이랑 기타 문제되는 일부 케이스 정도만 걸러내자..
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
//				//if (!bWasUnattended){ // 저장하려고 하는데 온갖 메시지들이 뜨게 되니 그걸 좀 없애려고 했는데 이걸론 안먹힘..
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
//	// 플레이어 스타트 스케일체크
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
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("PlayerStart의 스케일값이 변경되었습니다.\nX Y Z 모두 1.0으로 설정해주세요.")));
//			pPlayerStart->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
//#endif
//		}
//	}
//
//	// SkeletalMeshComponent 들에 MID 가 overriding 되어 있으면 clear 하는데 연출과 관련이 있다. AB2StageEventDirector::SafetyProcessMatineeSkActors
//	// 근데 보니까 StageEventDirector 말고도 다른 경로로 세팅되는 게 있어서 걍 전체.. 이걸 그냥 두는게 좋나..
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
	//// 이미 적용되기 시작한 ActiveCameraActor 가 있다면 DefaultPlayerCamera 를 적용해선 안됨.
	//// FActorIterator 를 도는 대신 GameMode 를 통해 검사
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//const bool bAnyActiveCameraActorActivated = (B2GM && B2GM->GetCurrentActiveActiveCameraActor());

	//if (bAnyActiveCameraActorActivated == false)
	//{
	//	// 블렌딩 없는 초기 적용.
	//	ApplyDefaultPlayerCameraSetting(Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this)), 0.0f);
	//}
}

void ABladeIIWorldSettings::ApplyDefaultPlayerCameraSetting(class ABladeIIPlayerController* InB2PC, float BlendTime, EViewTargetBlendFunction BlendFunc, bool bNoSetViewTarget)
{
	//ABladeIIPlayer* B2Player = InB2PC ? Cast<ABladeIIPlayer>(InB2PC->GetPawn()) : NULL;

	//if (InB2PC && B2Player)
	//{
	//	// Override 가 없을 경우 사용할 클래스 디폴트 
	//	ABladeIIPlayer* B2PlayerDefault = Cast<ABladeIIPlayer>(B2Player->GetClass()->GetDefaultObject());

	//	// 세팅의 override 된 값들 적용
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

	//	if (!bNoSetViewTarget) // bNoSetViewTarget 을 false 로 주는 경우는 CameraBoom 만 돌려놓고 실제로 Viewtarget 이 어떻게 될지는 모르는 상황.
	//	{
	//		// Override 가 없더라도 기본 블루프린트에 정의된 top-down 카메라로 돌아가도록.
	//		// 자 한번 블렌딩을 해 보자. 이전 ViewTarget 이 PlayerCharacter 인 경우도 블렌딩이 작동하도록 하려면 ForceFromLastView 사용.
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
	//// 리모트나 이런 애들 말고 Local player character 만. GetLocalPlayerCharacter 가 NULL 을 리턴하는 시점이 나올 수도 있음.
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
	//		FlushRenderingCommands(); // 이런 건 최대한 빨리 적용.
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
	// Static lighting 환경에서 dynamic shadow 를 쓰려니 문제가 되는 경우가 있어서 wave 별로 끌 수 있도록 하는 설정 제공..
	// TurnOffMeshComponentsDyamicShadowForModulated 참고.
	// SpawnPool 쪽 Difficulty 세팅이 int 라 여기도.. ㅠㅠ
	FCombinedUniqueSPWaveID CheckID(InCurrClientStage, InCurrDifficulty, InCheckWaveNum);
	// 그런 키값이 있는지만 본다. bool Value 값은 중요하지 않음.
	return (DynamicShadowExcludeWaveNameKeys.Find(CheckID.GetMyKeyName()) != nullptr);
}

#if WITH_EDITOR
FString ABladeIIWorldSettings::GetIniKeyStringPrefixOfDefaultPresetEnum(EWorldDefaultPlayCameraPreset InPresetEnum)
{	
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWorldDefaultPlayCameraPreset"), true);
	//if (EnumPtr)
	//{
	//	FString PresetEnumName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPresetEnum));

	//	// 네이밍 룰이 맞아야 되겠다. 그렇다고 CheckPCClassEnumNamingConvention 처럼 할 필요까지는..
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

	// TransientDefaultCameraPreset 선택시 Preset 값들로 덮어쓰기.
	if(PropertyName == Name_TransientDefaultCameraPreset)
	{
		if (TransientDefaultCameraPreset != EWorldDefaultPlayCameraPreset::EWDCP_End)
		{
			// Preset 들 로드
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
		// 기타의 경우 일단 EWDCP_End 로 돌려놔서 다시 간편하게 프리셋 세팅이 가능하도록 한다.
		TransientDefaultCameraPreset = EWorldDefaultPlayCameraPreset::EWDCP_End;
	}

	if (PropertyName == Name_EditorStageNumber || PropertyName == Name_EditorStageDifficulty)
	{
		EditorStageNumber = FMath::Max(EditorStageNumber, 1);
		EditorStageDifficulty = FMath::Clamp(EditorStageDifficulty, (int32)EStageDifficulty::ESD_Normal, (int32)EStageDifficulty::ESD_Hell);

		bool bFoundStageNumberSetting = false;
		bool bFoundDifficultySetting = false;
		EStageDifficulty FallbackDifficultyLevel = EStageDifficulty::ESD_None;
		
		// MonsterSpawnPool 이랑 StageEventDirector 프리뷰 업데이트 해야 함.
		for (FActorIterator ActorIt(GEditor->GetEditorWorldContext().World()); ActorIt; ++ActorIt)
		{
			AB2MonsterSpawnPool* SpawnPool = Cast<AB2MonsterSpawnPool>(*ActorIt);
			AB2StageEventDirector* EventDirector = Cast<AB2StageEventDirector>(*ActorIt);

			if (SpawnPool) 
			{
				const FMonsterSpawnPoolPerStageSettings* ThisStageSetting = SpawnPool->GetDesiredStageSetting(EditorStageNumber, (EStageDifficulty)EditorStageDifficulty);
				// 현재 선택한 스테이지 번호와 난이도 레벨에 맞게 편집 정보들을 업데이트
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
				FString::Printf(TEXT("현재 설치된 SpawnPool 에서 에디터용 StageNum %d 와(과) DifficultyLevel %d 에 해당하는 Wave 세팅을 찾을 수 없습니다.\n\n해당 Stage 는 현 레벨에서 사용할 수 없습니다."), 
					EditorStageNumber, EditorStageDifficulty
				)));
#endif
		}
		else if (!bFoundDifficultySetting) 
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("현재 설치된 SpawnPool 에서 에디터용 StageNum %d 에 해당하는 설정을 찾았으나 DifficultyLevel %d 에 해당하는 Wave 세팅을 찾지 못하여 가까운 DifficultyLevel %d 세팅을 대신 사용하게 됩니다."),
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
		{ // 시나리오 모드는 일반 난이도에서만 작동함. 에디터 프리뷰 혼선 방지를 위해 선택 막아둠.
			return FCacheStageEssentialDataToKeep::IsScenarioModeAllowedDifficulty((EStageDifficulty)EditorStageDifficulty);
		}
	}

	return Super::CanEditChange(InProperty);
}
#endif 