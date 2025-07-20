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
#include "BladeIILocText.h"
#include "BladeIIUtil.h"

#if WITH_EDITOR
void FWorldDefaultPlayCameraSettings::LoadPresetValues(FWorldDefaultPlayCameraSettings& OutResult, FString UniquePresetIDString)
{
	float LoadedPitch = 0.0f;
	GConfig->GetFloat(TEXT("WorldDefaultPlayCameraPreset"), *FString::Printf(TEXT("%s_Pitch"), *UniquePresetIDString), LoadedPitch, GGameIni);
	OutResult.bOverrideCameraBoomPitch = true; // 捞扒 弊成 难 淋.
	OutResult.CameraBoomPitch = LoadedPitch;

	float LoadedYaw = 0.0f;
	GConfig->GetFloat(TEXT("WorldDefaultPlayCameraPreset"), *FString::Printf(TEXT("%s_Yaw"), *UniquePresetIDString), LoadedYaw, GGameIni);
	OutResult.bOverrideCameraBoomYaw = true; // 捞扒 弊成 难 淋.
	OutResult.CameraBoomYaw = LoadedYaw;

	float LoadedRoll = 0.0f;
	GConfig->GetFloat(TEXT("WorldDefaultPlayCameraPreset"), *FString::Printf(TEXT("%s_Roll"), *UniquePresetIDString), LoadedRoll, GGameIni);
	OutResult.bOverrideCameraBoomRoll = true; // 捞扒 弊成 难 淋.
	OutResult.CameraBoomRoll = LoadedRoll;

	float LoadedArmLength = 0.0f;
	GConfig->GetFloat(TEXT("WorldDefaultPlayCameraPreset"), *FString::Printf(TEXT("%s_ArmLength"), *UniquePresetIDString), LoadedArmLength, GGameIni);
	OutResult.bOverrideCameraBoomArmLength = true; // 捞扒 弊成 难 淋.
	OutResult.CameraBoomArmLength = LoadedArmLength;
}
void FWorldDefaultPlayCameraSettings::ApplyPresetValues(FWorldDefaultPlayCameraSettings& InPreset)
{
	// Preset 利侩矫俊绰 bOverride** 甸阑 葛滴 难淋
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
	// 贸澜何磐 int32 技泼阑 力傍窍瘤 臼绰 扒 悸诀捞 逞 锅芭酚绊 加己芒俊辑 犬牢窍扁档 锅芭肺况辑..
	return FName(
		*FString::Printf(TEXT("%d_%d_%d"), InClientStageId, InTargetDifficulty, InTargetWave)
	);
}

ABladeIIWorldSettings::ABladeIIWorldSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EditorStageNumber = 1;
	////EditorStageDifficulty = (int32)EStageDifficulty::ESD_Hard;
	bDevTestScenarioMode = true;
	TransientDefaultCameraPreset = EWorldDefaultPlayCameraPreset::EWDCP_End;
}

void ABladeIIWorldSettings::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	if (LocalizedAreaNameKey != NAME_None && LocalizedAreaSubNameKey == NAME_None)
	{ // LocalizedAreaSubName 阑 第词霸 备泅秦辑 捞犯霸 扁粮俊 汲沥茄 霸 绝菌促搁 磊悼栏肺 持档废 窃.
		LocalizedAreaSubNameKey = FName(*(LocalizedAreaNameKey.ToString() + TEXT("_Sub")));
	}

	if (GIsEditor)
	{ // 逞 磊林 角荐窍绰 汲沥. 歹 捞惑 曼瘤 给窍绊 World PostLoad 俊辑
		// 咯矾 胶飘府怪 饭骇 乐阑 版快浚 P 甘俊辑父.
		UWorld* TheWorld = GetWorld();
		//UPackage* PMapPackage = (TheWorld && TheWorld->PersistentLevel) ? TheWorld->PersistentLevel->GetOutermost() : NULL;
		//const bool bAmIPMap = (PMapPackage && this->GetOutermost() == PMapPackage); P Map 俊辑父 眉农窍妨 沁绰单 救冈塞.
		if (TheWorld && !TheWorld->IsPlayInEditor())
		{
			AB2StageEventDirector::CheckAndWarnForComponentTriggerSetup(TheWorld);
		}
	}
#endif
}

void ABladeIIWorldSettings::NotifyBeginPlay()
{
	// BladeIIGameMode 唱 StageManager 狼 PreBeginPlay 焊促绰 唱吝捞瘤父 咯窍埃 促弗 Actor 甸狼 檬馆 BeginPlay 焊促绰 菊辑 阂府绰 瘤痢.

	Super::NotifyBeginPlay();
}

void ABladeIIWorldSettings::BeginPlay()
{
	Super::BeginPlay();
}

void ABladeIIWorldSettings::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
#if WITH_EDITOR
	if (GIsEditor && !IsRunningCommandlet())
	{
		// 磊悼栏肺 呼靛秦林妨绊 沁绰单 绢瞒乔 瘤陛 酒贰客 鞍篮 炼扒捞 皑瘤啊 救登绰 滚弊 惑炔捞 乐绰 芭 鞍促.
		// 付皑 烙冠秦辑 呼靛 柄瘤搁 概快 帮鄂窍聪 公炼扒 磊悼 呼靛 倒赴促.
		//UWorld* TheWorld = GetWorld();
		//if (TheWorld && TheWorld->GetNavigationSystem() && TheWorld->GetNavigationSystem()->IsNavigationDirty())

		// 措脚 磊悼 历厘登绰 颇老捞尔 扁鸥 巩力登绰 老何 纳捞胶 沥档父 吧矾郴磊..
		UPackage* MapPackage = GetOutermost();
		UWorld* TheWorld = GetWorld();
		if (MapPackage && TheWorld && TheWorld->GetLevels().Num() > 0)
		{
			FString MapPackageFullPath = MapPackage->GetPathName();
			if (!MapPackageFullPath.Contains(TEXT("Autosaves")) && 
				!MapPackageFullPath.Contains(TEXT("LVCH_")) && 
				!MapPackageFullPath.Contains(TEXT("Lobby")))
			{
				bool bWasUnattended = FApp::IsUnattended();
				const TCHAR* CommandLineBackup = FCommandLine::Get();
				//if (!bWasUnattended){ // 历厘窍妨绊 窍绰单 柯爱 皋矫瘤甸捞 哆霸 登聪 弊吧 粱 绝局妨绊 沁绰单 捞吧沸 救冈塞..
				//	FCommandLine::Append(TEXT(" -UNATTENDED"));
				//}

				UE_LOG(LogEngine, Log, TEXT("Auto re-build navigation of %s at PreSave.."), *MapPackageFullPath);
				FEditorBuildUtils::EditorBuild(GetWorld(), FBuildOptions::BuildAIPaths);

				//if (!bWasUnattended){
				//	FCommandLine::Set(CommandLineBackup);
				//}
			}
		}

		if (!EditorFakeConfigure::GetInstance().HavePowerToSave() && *PreviousDefaultGameMode != nullptr)
		{
			DefaultGameMode = PreviousDefaultGameMode;
		}
	}

	// 敲饭捞绢 胶鸥飘 胶纳老眉农
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		APlayerStart* pPlayerStart = Cast<APlayerStart>(*It);

		if(pPlayerStart &&
			(pPlayerStart->GetActorScale3D().X != 1.0f
			|| pPlayerStart->GetActorScale3D().Y != 1.0f
			|| pPlayerStart->GetActorScale3D().Z != 1.0f)
			)
		{
#if !PLATFORM_MAC
			UE_LOG(LogEngine, Log, TEXT("Check!! PlayerStart Scale!!"));
			//FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("PlayerStart狼 胶纳老蔼捞 函版登菌嚼聪促.\nX Y Z 葛滴 1.0栏肺 汲沥秦林技夸.")));
			pPlayerStart->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
#endif
		}
	}

	// SkeletalMeshComponent 甸俊 MID 啊 overriding 登绢 乐栏搁 clear 窍绰单 楷免苞 包访捞 乐促. AB2StageEventDirector::SafetyProcessMatineeSkActors
	// 辟单 焊聪鳖 StageEventDirector 富绊档 促弗 版肺肺 技泼登绰 霸 乐绢辑 傲 傈眉.. 捞吧 弊成 滴绰霸 亮唱..
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		ASkeletalMeshActor* CastedSKActor = Cast<ASkeletalMeshActor>(*It);
		USkeletalMeshComponent* SKMeshComp = CastedSKActor ? CastedSKActor->GetSkeletalMeshComponent() : NULL;

		if (CastedSKActor && SKMeshComp)
		{
			for (int32 MI = 0; MI < SKMeshComp->OverrideMaterials.Num(); ++MI)
			{
				UMaterialInstanceDynamic* OverriddenMID = Cast<UMaterialInstanceDynamic>(SKMeshComp->OverrideMaterials[MI]);
				if (OverriddenMID)
				{
					SKMeshComp->SetMaterial(MI, NULL);
				}
			}
		}
	}
#endif
}

void ABladeIIWorldSettings::InitDefaultPlayerCameraSetting()
{
	// 捞固 利侩登扁 矫累茄 ActiveCameraActor 啊 乐促搁 DefaultPlayerCamera 甫 利侩秦急 救凳.
	// FActorIterator 甫 档绰 措脚 GameMode 甫 烹秦 八荤
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	const bool bAnyActiveCameraActorActivated = (B2GM && B2GM->GetCurrentActiveActiveCameraActor());

	if (bAnyActiveCameraActorActivated == false)
	{
		// 喉坊爹 绝绰 檬扁 利侩.
		ApplyDefaultPlayerCameraSetting(Cast<ABladeIIPlayerController>(UGameplayStatics::GetPlayerController(this, 0)), 0.0f);
	}
}

void ABladeIIWorldSettings::ApplyDefaultPlayerCameraSetting(class ABladeIIPlayerController* InB2PC, float BlendTime, EViewTargetBlendFunction BlendFunc, bool bNoSetViewTarget)
{
	ABladeIIPlayer* B2Player = InB2PC ? Cast<ABladeIIPlayer>(InB2PC->GetPawn()) : NULL;

	if (InB2PC && B2Player)
	{
		// Override 啊 绝阑 版快 荤侩且 努贰胶 叼弃飘 
		ABladeIIPlayer* B2PlayerDefault = Cast<ABladeIIPlayer>(B2Player->GetClass()->GetDefaultObject());

		// 技泼狼 override 等 蔼甸 利侩
		if (DefaultPlayerCameraSettings.bOverrideCameraBoomPitch)
		{
			InB2PC->SetCameraBoomPitch(DefaultPlayerCameraSettings.CameraBoomPitch);
		}
		else if (B2PlayerDefault)
		{
			InB2PC->SetCameraBoomPitch(B2PlayerDefault->CameraBoom->GetRelativeRotation().Pitch);
		}
			
		if (DefaultPlayerCameraSettings.bOverrideCameraBoomYaw)
		{
			InB2PC->SetCameraBoomYaw(DefaultPlayerCameraSettings.CameraBoomYaw);
		}
		else if (B2PlayerDefault)
		{
			InB2PC->SetCameraBoomYaw(B2PlayerDefault->CameraBoom->GetRelativeRotation().Yaw);
		}

		if (DefaultPlayerCameraSettings.bOverrideCameraBoomRoll)
		{
			InB2PC->SetCameraBoomRoll(DefaultPlayerCameraSettings.CameraBoomRoll);
		}
		else if (B2PlayerDefault)
		{
			InB2PC->SetCameraBoomRoll(B2PlayerDefault->CameraBoom->GetRelativeRotation().Roll);
		}

		if (DefaultPlayerCameraSettings.bOverrideCameraBoomArmLength)
		{
			InB2PC->SetCameraBoomArmLength(GetExtraAppliedCameraDistance(
				DefaultPlayerCameraSettings.CameraBoomArmLength, DefaultPlayerCameraSettings.CameraBoomArmLength_Extra
			));
		}
		else if (B2PlayerDefault)
		{
			InB2PC->SetCameraBoomArmLength(B2PlayerDefault->CameraBoom->TargetArmLength);
		}

		if (DefaultPlayerCameraSettings.bOverrideCameraTargetFocusOffset)
		{
			InB2PC->SetCameraTargetFocusOffset(DefaultPlayerCameraSettings.CameraTargetFocusOffset);
		}
		else if (B2PlayerDefault)
		{
			InB2PC->SetCameraTargetFocusOffset(FVector::ZeroVector);
		}

		InB2PC->SetCameraBoomLockedViewTarget(DefaultPlayerCameraSettings.bCameraBoomLockedViewTarget);

		InB2PC->SetDefaultCameraRotator();

		if (!bNoSetViewTarget) // bNoSetViewTarget 阑 false 肺 林绰 版快绰 CameraBoom 父 倒妨初绊 角力肺 Viewtarget 捞 绢痘霸 瞪瘤绰 葛福绰 惑炔.
		{
			// Override 啊 绝歹扼档 扁夯 喉风橇赴飘俊 沥狼等 top-down 墨皋扼肺 倒酒啊档废.
			// 磊 茄锅 喉坊爹阑 秦 焊磊. 捞傈 ViewTarget 捞 PlayerCharacter 牢 版快档 喉坊爹捞 累悼窍档废 窍妨搁 ForceFromLastView 荤侩.
			InB2PC->SetViewTargetWithBlend(B2Player, BlendTime, BlendFunc, 0.0f, false);
		}
	}
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
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	ABladeIIPlayer* AssumedLocalPlayer = PC ? Cast<ABladeIIPlayer>(PC->GetCharacter()) : nullptr;
	// 府葛飘唱 捞繁 局甸 富绊 Local player character 父. GetLocalPlayerCharacter 啊 NULL 阑 府畔窍绰 矫痢捞 唱棵 荐档 乐澜.
	if (targetPlayer && (!AssumedLocalPlayer || AssumedLocalPlayer == targetPlayer))
	{
		UPointLightComponent* PlayerAttachedLight = targetPlayer ? targetPlayer->LightComponent : NULL;
		if (PlayerAttachedLight)
		{
			if (PlayerLightSettings.bOverrideIntensity)
			{
				PlayerAttachedLight->SetIntensity(PlayerLightSettings.Intensity);
			}
			if (PlayerLightSettings.bOverrideLightColor)
			{
				PlayerAttachedLight->SetLightColor(PlayerLightSettings.LightColor);
			}
			PlayerAttachedLight->MarkRenderStateDirty();
			FlushRenderingCommands(); // 捞繁 扒 弥措茄 弧府 利侩.
		}
	}
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
	// Static lighting 券版俊辑 dynamic shadow 甫 静妨聪 巩力啊 登绰 版快啊 乐绢辑 wave 喊肺 缠 荐 乐档废 窍绰 汲沥 力傍..
	// TurnOffMeshComponentsDyamicShadowForModulated 曼绊.
	// SpawnPool 率 Difficulty 技泼捞 int 扼 咯扁档.. ばば
	FCombinedUniqueSPWaveID CheckID(InCurrClientStage, InCurrDifficulty, InCheckWaveNum);
	// 弊繁 虐蔼捞 乐绰瘤父 夯促. bool Value 蔼篮 吝夸窍瘤 臼澜.
	return (DynamicShadowExcludeWaveNameKeys.Find(CheckID.GetMyKeyName()) != nullptr);
}

#if WITH_EDITOR
FString ABladeIIWorldSettings::GetIniKeyStringPrefixOfDefaultPresetEnum(EWorldDefaultPlayCameraPreset InPresetEnum)
{	
	const UEnum* EnumPtr = StaticEnum<EWorldDefaultPlayCameraPreset>();
	if (EnumPtr)
	{
		FString PresetEnumName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPresetEnum));

		// 匙捞怪 逢捞 嘎酒具 登摆促. 弊犯促绊 CheckPCClassEnumNamingConvention 贸烦 且 鞘夸鳖瘤绰..
		if (PresetEnumName.RemoveFromStart(EWORLDDEFAULTCAMERAPRESET_COMMON_PREFIX) == true)
		{
			return PresetEnumName;
		}
	}

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

	// TransientDefaultCameraPreset 急琶矫 Preset 蔼甸肺 丹绢静扁.
	if(PropertyName == Name_TransientDefaultCameraPreset)
	{
		if (TransientDefaultCameraPreset != EWorldDefaultPlayCameraPreset::EWDCP_End)
		{
			// Preset 甸 肺靛
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
		// 扁鸥狼 版快 老窜 EWDCP_End 肺 倒妨出辑 促矫 埃祈窍霸 橇府悸 技泼捞 啊瓷窍档废 茄促.
		TransientDefaultCameraPreset = EWorldDefaultPlayCameraPreset::EWDCP_End;
	}

	if (PropertyName == Name_EditorStageNumber || PropertyName == Name_EditorStageDifficulty)
	{
		EditorStageNumber = FMath::Max(EditorStageNumber, 1);
		EditorStageDifficulty = FMath::Clamp(EditorStageDifficulty, (int32)EStageDifficulty::ESD_Normal, (int32)EStageDifficulty::ESD_Hell);

		bool bFoundStageNumberSetting = false;
		bool bFoundDifficultySetting = false;
		EStageDifficulty FallbackDifficultyLevel = EStageDifficulty::ESD_None;
		
		// MonsterSpawnPool 捞尔 StageEventDirector 橇府轰 诀单捞飘 秦具 窃.
		for (FActorIterator ActorIt(GEditor->GetEditorWorldContext().World()); ActorIt; ++ActorIt)
		{
			AB2MonsterSpawnPool* SpawnPool = Cast<AB2MonsterSpawnPool>(*ActorIt);
			AB2StageEventDirector* EventDirector = Cast<AB2StageEventDirector>(*ActorIt);

			if (SpawnPool) 
			{
				const FMonsterSpawnPoolPerStageSettings* ThisStageSetting = SpawnPool->GetDesiredStageSetting(EditorStageNumber, (EStageDifficulty)EditorStageDifficulty);
				// 泅犁 急琶茄 胶抛捞瘤 锅龋客 抄捞档 饭骇俊 嘎霸 祈笼 沥焊甸阑 诀单捞飘
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
				FString::Printf(TEXT("泅犁 汲摹等 SpawnPool 俊辑 俊叼磐侩 StageNum %d 客(苞) DifficultyLevel %d 俊 秦寸窍绰 Wave 技泼阑 茫阑 荐 绝嚼聪促.\n\n秦寸 Stage 绰 泅 饭骇俊辑 荤侩且 荐 绝嚼聪促."), 
					EditorStageNumber, EditorStageDifficulty
				)));
#endif
		}
		else if (!bFoundDifficultySetting) 
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("泅犁 汲摹等 SpawnPool 俊辑 俊叼磐侩 StageNum %d 俊 秦寸窍绰 汲沥阑 茫疽栏唱 DifficultyLevel %d 俊 秦寸窍绰 Wave 技泼阑 茫瘤 给窍咯 啊鳖款 DifficultyLevel %d 技泼阑 措脚 荤侩窍霸 邓聪促."),
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
		{ // 矫唱府坷 葛靛绰 老馆 抄捞档俊辑父 累悼窃. 俊叼磐 橇府轰 去急 规瘤甫 困秦 急琶 阜酒狄.
			return FCacheStageEssentialDataToKeep::IsScenarioModeAllowedDifficulty((EStageDifficulty)EditorStageDifficulty);
		}
	}

	return Super::CanEditChange(InProperty);
}
#endif 