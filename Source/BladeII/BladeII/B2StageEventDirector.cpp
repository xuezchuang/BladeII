// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square
#include "B2StageEventDirector.h"

#include "Camera/CameraComponent.h"
//#include "Matinee/InterpData.h"
//#include "Matinee/InterpGroup.h"
//#include "Matinee/InterpGroupInst.h"
//#include "Matinee/InterpTrackMove.h"
//#include "Matinee/InterpTrackSound.h"
//#include "Matinee/InterpGroupDirector.h"
//#include "Matinee/InterpTrackDirector.h"
//#include "Matinee/InterpTrackEvent.h"
//#include "Matinee/MatineeActor.h"
//#include "Matinee/InterpTrackAnimControl.h"
#include "Animation/SkeletalMeshActor.h"
#include "Engine/SkeletalMesh.h"
#include "Engine.h"
#if WITH_EDITOR
#include "Editor.h"
#include "FileHelpers.h"
#include "Components/TextRenderComponent.h"
#endif
#include "BladeIIUtil.h"
#include "B2UIEnum.h"
#include "B2StageManager.h"


#include "LevelSequenceActor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"

#include "MovieScene.h"
#include "MovieSceneBinding.h"
#include "IMovieScenePlayer.h"      // IMovieScenePlayer::FindBoundObjects
#include "MovieSceneSequenceID.h"


//#include "B2StageManager.h"
////#include "B2MonsterSpawnPool.h"
//#include "BladeIIGameMode.h"
//#include "B2StageGameMode.h"
#include "B2NetGameMode.h"
#include "B2PVPGameMode.h"
#include "MovieSceneObjectBindingID.h"
////#include "B2TMGameMode.h"
////#include "B2RaidGameMode.h"
//#include "BladeIIPlayer.h"
//#include "BladeIIPlayerController.h"
////#include "BladeIINetPlayer.h"
////#include "Camera/PlayerCameraManager.h"
////#include "Camera/CameraAnim.h"
////#include "Camera/CameraAnimInst.h"
////#include "B2SkillRTManager.h"
//#include "B2PCClassInfo.h"
//#include "B2PCClassInfoBox.h"
//#include "B2NPCClassInfo.h"
////#include "B2NPCClassInfoBox.h"
////#include "B2ActiveCameraActor.h"
////#include "BladeIIWorldSettings.h"
////#include "B2UIGuildRound.h"
//#include "Event.h"
////#include "B2UIManager.h"
////#include "B2UIBattleMain.h"
////#include "B2UIPVP1on1BattleMain.h"
////#include "B2UITeamMatchBattle.h"
//#include "B2UIDocHelper.h"
////#include "B2UIStageResult.h"
////#include "B2UIPVPBegin.h"
////#include "B2UIHeroTowerResult.h"
//#include "Particles/Emitter.h"
//#include "BladeIIScalabilityControl.h"
////#include "B2PreRenderer.h"
////#include "B2TutorialGameMode.h"
//#include "B2AutoWayPoint.h"
//#include "FB2ErrorMessage.h"
//#include "BladeIIBlockToSyncNetwork.h"

// 弊成 CachedStageManager 甫 眉农窍搁 StageManager 啊 绝绰 霸烙葛靛俊辑 官肺 吧覆
#define SAFE_STAGEMANAGER_CHECK BII_CHECK( !(Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(this))) || CachedStageManager );

ABladeIICharacter* FPlayActorShowRuntimeState::GetPlayActor(class AB2StageEventDirector* OwnerSED)
{// LocalPlayerCharacter 甫 某教秦辑 静促啊 巩力啊 等 利捞 粱 乐绢辑 救傈 瞒盔俊辑.. 辟单 促弗 PlayActor 档 弊凡 荐 乐阑 过 茄单.. 眉农扼档 吧绢杭鳖唱..
	ABladeIICharacter* RetChar = NULL;// bIsForLocalPlayerCharacter ? Cast<ABladeIICharacter>(UGameplayStatics::GetLocalPlayerCharacter(OwnerSED)) : ThePlayActor;
	// PlayActor 绰 NULL 老 荐 乐促. 抗甫 甸绢 促荐狼 胶抛捞瘤甫 困茄 ShowSetting 牢单 SpawnPool 俊辑绰 绢恫 胶抛捞瘤俊辑 各捞 代 唱柯促芭唱 窍绰 版快. 蝶扼辑 validity 眉农父 窍档废.
	//ensure(!RetChar || RetChar->IsValidObj()); // 楷免 档吝俊 Directed 啊 酒囱 (PlayActorStateMode) PlayActor 啊 destroy 登绢滚赴促搁 IsValidObj 啊 酒匆 荐 乐促. 泅犁肺急 技泼俊 蝶扼 啊瓷己捞 乐变 窃..
	return (IsValid(RetChar) ? RetChar : NULL);
}

void FPlayActorShowRuntimeState::SetPlayActor(class ABladeIICharacter* InPlayActor)
{
	ThePlayActor = InPlayActor;

	//bIsForLocalPlayerCharacter = (InPlayActor && UGameplayStatics::GetLocalPlayerCharacter(InPlayActor) == InPlayActor);
}

void FDelayedDyingMobCache::SetDyingMobCache(class ABladeIICharacter* InDyingMob, int32 InWaveIndex, int32 InObjIndex)
{
	if (IsValid(InDyingMob))
	{// 磷绢啊绰 局扼档 IsValidObj 咯具..
		TheDyingMob = InDyingMob;
		WaveIndex = InWaveIndex;
		ObjIndex = InObjIndex;
	}
}
void FDelayedDyingMobCache::ForgetIt()
{
	TheDyingMob = NULL;
	WaveIndex = -1;
	ObjIndex = -1;
}
void FDelayedDyingMobCache::ForgetIfMatch(class ABladeIICharacter* InMobToForget)
{ // 镭绊磊 窍绰 芭聪 被捞 valid 八荤鳖瘤绰 せ
	if (InMobToForget == TheDyingMob)
	{
		ForgetIt();
	}
}
ABladeIICharacter* FDelayedDyingMobCache::GetIfMatch(int32 InWaveIndex, int32 InObjIndex)
{
	if (IsValid(TheDyingMob ) && InWaveIndex == WaveIndex && InObjIndex == ObjIndex) 
	{ // 磷绢啊绰 局扼档 IsValidObj 咯具..
		return TheDyingMob;
	}
	return NULL;
}

ABladeIIGameMode* AB2StageEventDirector::GetB2GameMode()
{
	//if (!CachedGameMode || !CachedGameMode->IsValidObj())
	//{
	//	CachedGameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	//}
	//// 俊叼磐俊辑 甘 凯 锭 咯扁肺 坷绰 版快啊 乐绢辑 俊叼磐绰 check 胶诺
	//BII_CHECK(GIsEditor || (CachedGameMode && CachedGameMode->IsValidObj()));
	return CachedGameMode;
}

// Sets default values
AB2StageEventDirector::AB2StageEventDirector(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick	= true;
	bReplicates						= false;
	NetUpdateFrequency				= 10.0f;
	SetReplicateMovement(false);
	SetCanBeDamaged(false);
	
	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(DefaultRootComponent);

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	EditorSpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("StageEventDirectorSprite"));
	if (!IsRunningCommandlet() && (EditorSpriteComponent != nullptr))
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture;
			FName ID_B2StageEventDirector;
			FText NAME_B2StageEventDirector;
			FConstructorStatics()
				: SpriteTexture(TEXT("/Engine/EditorResources/SceneManager")) // SceneManager 咆胶媚啊 购啊 楷免胶反霸 积辫 ぇぇ
				, ID_B2StageEventDirector(TEXT("B2StageEventDirector"))
				, NAME_B2StageEventDirector(FText::FromString(TEXT("B2StageEventDirectorEditorSprite")))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		EditorSpriteComponent->Sprite = ConstructorStatics.SpriteTexture.Get();
		EditorSpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_B2StageEventDirector;
		EditorSpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_B2StageEventDirector;
		EditorSpriteComponent->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		EditorSpriteComponent->bIsScreenSizeScaled = true;
		EditorSpriteComponent->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
	}
#endif

	OverallMode							= EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee;
	bOverallMode_ExportedCamera			= false;
	bOverallMode_ControlledMatinee		= true;
	bOverallMode_ManualCamera			= false;
	bOverallMode_ExportedOrManualCamera = false;
		
	DirectingEvent						= EStageEvent::ESEV_Begin;
	ClearEventPhase						= EStageClearEventPhase::ESCE_Phase1;
	bDirectingEvent_MobEvent			= false;
	bDirectingEvent_MobDying			= false;
	bDirectingEvent_MobPhase			= false;
	bDirectingEvent_ComponentTrigger	= false;

	ManageMode							= EStageEventManageMode::SEMM_Always;
	//bManageModeLeastSkip				= false;

	bCheckPlayOne						= true;

	MobEventMobPhase					= 1;
	TriggerComponentName				= NAME_None;

	OverallStartDelay					= 0.0f;
	SpecifiedOverallLength				= 0.0f;
	bStandbyAtTheEnd					= false;
	bStandbyPause						= false;
	GlobalTimeDilation					= 1.0f;

	EventWidgetType						= EStageEventWidgetType::None;
	EventWidgetStartTime				= 0.0f;
	EventWidgetEndTime					= 0.0f;

	CachedStageManager					= NULL;
	CurrentStageId						= 1;
	CurrentStageDifficulty				= EStageDifficulty::ESD_Normal;
	
	//CameraAnim							= NULL;
	CameraAnimApplyCamIndex				= 0;
	CameraAnimInitialBlendingTime		= 0.0f;

	//MatineeActor						= NULL;

	ManualCameraSelection				= EManualCameraSelection::EDCT_DirectorActorCamera;
	FinalCameraRestoreBlendTime			= 1.0f;
	
	bPCSyncToMatineeStart				= false;
	bPCDependentDirectorTrack			= false;
	RandomMatineeGroupNum				= 0;
	bTryHidePlayer						= false;
	bForceNoHidePlayer					= false;
	bHideHud							= true;
	bDisableInput						= true;
	bHidePlayerPointLight				= false;
	bAllowOtherPlayerPointLight			= false;
	bHackMyPerPCClassLightingSetupDone_DEPRECATED = false;
	bLocalPlayerSwitching = false;

	//ActiveCameraForceOnFinish			= NULL;

	bForceStopPlayerAttackState			= true;
	bStopAllMobsWhilePlaying			= false;
	MatineeFirstSkipFrames				= 0;

	// ManualCameraSettings 窍唱绰 扁夯栏肺 持绢霖促.
	ManualCameraSettings.Add(FManualCameraSingleSetting());

	// 墨皋扼 积己. 惑技茄 困摹 殿狼 炼例篮 窍困 BP 努贰胶 轰器飘唱 饭骇 轰器飘俊辑 窍贬
	DirectorCamera_0 = CreateDefaultSubobject<UCameraComponent>(TEXT("DirectorCamera_0"));
	DirectorCamera_0->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	DirectorCamera_0->bAutoActivate = false;
	DirectorCamera_0->SetRelativeLocation(FVector(50.0f, -100.0f, 100.0f));
	DirectorCamera_0->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	DirectorCamArrayRef[0] = DirectorCamera_0; // 窜瘤 贸澜何磐 array 肺 急攫窍绊 俊叼磐俊辑 祈笼窍档废 畴免捞 救蹬辑 捞犯霸 窃.
	
	DirectorCamera_1 = CreateDefaultSubobject<UCameraComponent>(TEXT("DirectorCamera_1"));
	DirectorCamera_1->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	DirectorCamera_1->bAutoActivate = false;
	DirectorCamera_1->SetRelativeLocation(FVector(0.0f, -100.0f, 100.0f));
	DirectorCamera_1->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	DirectorCamArrayRef[1] = DirectorCamera_1;
	
	DirectorCamera_2 = CreateDefaultSubobject<UCameraComponent>(TEXT("DirectorCamera_2"));
	DirectorCamera_2->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	DirectorCamera_2->bAutoActivate = false;
	DirectorCamera_2->SetRelativeLocation(FVector(-50.0f, -100.0f, 100.0f));
	DirectorCamera_2->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	DirectorCamArrayRef[2] = DirectorCamera_2;
		
	CurrentActiveCamera				= NULL;
	SetUpOutSideCharacter			= NULL;

	bDirectingEventConditionMet		= false;
	bDependentDirectorActorMet		= false;
	bAllManualCameraEnded			= false;
	bCameraAnimFinished				= false;
	bControlledMatineeFinished		= false;
	bRelevantUIOpenComplete			= false;
	MyRelevantUIName				= NAME_None;

	RandomlySetMatineeGroupNum		= -1;
	bPlayedOnceForThisStage			= false;

	ActorNameDisplayText			= NULL;

	bCurrentPCSyncToMatineeStart	= false;

	bWasPlayerAttacking				= false;

	bMatchResultEventAsWinner		= false;

	bCurrentlyPlaying				= false;
}

void AB2StageEventDirector::PostLoad()
{
	Super::PostLoad();

//
//#if WITH_EDITOR
//	SetEditConditionProperties();
//
//	// 趣矫扼档 饭骇 捞抚 官操搁 redirector 贸府啊 救登骨肺 咯扁辑档 八荤.
//	UWorld* TheWorld = GetWorld();
//	LevelStreamingOnBegin.EditorCheckForProperSetting(TheWorld);
//	LevelStreamingOnEnd.EditorCheckForProperSetting(TheWorld);
//
//	CheckAndWarnForLightSetup();
//#endif
}

void AB2StageEventDirector::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
#if WITH_EDITOR
	// 贸澜俊 捞吧 PostLoad 俊辑 窍妨绊 沁绰单 Preview 侩 哪器惩飘甸狼 顶官蹿 胶忱俏捞 肺爹 流饶俊 救冈囚辑 咯扁促 持澜.
	if (GIsEditor)
	{
		RefreshPreviewSKComps();
	}
	
	//TArray<UActorComponent*> AllCamComp = GetComponentsByClass(UCameraComponent::StaticClass());
	//for (UActorComponent* ThisComp : AllCamComp)
	//{
	//	UCameraComponent* ThisCamComp = Cast<UCameraComponent>(ThisComp);
	//	if (!ThisCamComp)
	//		continue;

	//	bool bOneOfInstalledCam = false;
	//	for (int32 CI = 0; CI < MAX_DIRECTOR_CAMERA; ++CI)
	//	{
	//		if (DirectorCamArrayRef[CI] == ThisCamComp){
	//			bOneOfInstalledCam = true;
	//			break;
	//		}
	//	}
	//	// MAX_DIRECTOR_CAMERA 临捞搁辑 捞傈俊 扁夯 积己登绢 乐带 芭 力芭.
	//	if (!bOneOfInstalledCam){
	//		ThisCamComp->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	//		ThisCamComp->ConditionalBeginDestroy();
	//		MarkPackageDirty();
	//	}
	//}

#endif
}

// Called when the game starts or when spawned
void AB2StageEventDirector::BeginPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::BeginPlay"));

	//Super::BeginPlay();

	//// 汲沥俊 蝶扼 (WaveMob 荤侩窍绰 版快) 捞霸 阂府扁 傈俊 BeginShow 啊 瞪 荐档 乐澜阑 疙缴.

	//// ESEV_ComponentTrigger 侩栏肺 瘤沥茄 哪器惩飘狼 BeginOverlap 殿废
	//if (DirectingEvent == EStageEvent::ESEV_ComponentTrigger && TriggerComponentName != NAME_None)
	//{
	//	UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(FindComponentByName(TriggerComponentName));
	//	if (PrimComp)
	//	{
	//		PrimComp->OnComponentBeginOverlap.AddDynamic(this, &AB2StageEventDirector::SelectedComponentTriggerBeginOverlapCB);
	//	}
	//}

	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && HasDirectedFromBeginningTrack() && MatineeActor && 
	//	!bCurrentlyPlaying)// 贸澜何磐 敲饭捞窍绰 芭扼搁 捞繁 贸府啊 鞘夸窍瘤档 臼摆瘤..
	//{
	//	MatineeActor->InitInterp(); // GetGroupActorOfGroupName 捞傈俊 鞘夸..
	//	for (int32 SI = 0; SI < ShowSettings.Num(); ++SI)
	//	{
	//		FPlayActorShowSettings& CurrSS = ShowSettings[SI];
	//		if (CurrSS.PlayActorStateMode == EPlayActorStateMode::EPASM_DirectedFromBeginning &&
	//			// 肺拿 敲饭捞绢 某腐磐 侩 飘发捞搁 捞犯霸 贸澜何磐 绊沥等 困摹俊 粮犁窍档废 窍绰 霸 富捞 登瘤 臼绊..
	//			// 扁鸥 促弗 鸥涝狼 敲饭捞绢 某腐磐扼搁 捞 矫痢俊辑绰 啊瓷茄 捞抚 炼钦阑 葛滴 茫酒郴具 且 巴. PlayActor 啊 乐绰 矫痢捞 酒聪骨肺.
	//			// 咯窍埃 瘤陛篮 弊繁 版快 瘤盔 阂啊.
	//			!IsPlayActorTypeForPlayerCharacter(CurrSS.PlayActorType)
	//			)
	//		{
	//			// 敲饭捞绢 某腐磐啊 酒聪聪 MatchingInterpGroupName 阑 弊措肺 荤侩.
	//			ASkeletalMeshActor* ThisGroupSKActor = Cast<ASkeletalMeshActor>(GetGroupActorOfGroupName(CurrSS.MatchingInterpGroupName));
	//			USkeletalMeshComponent* SKMeshComp = ThisGroupSKActor->GetSkeletalMeshComponent();
	//			if (ThisGroupSKActor && SKMeshComp)
	//			{ // 贸澜何磐 焊捞档废 难 霖促. 局聪皋捞记档 酒付 敲饭捞 惑怕咯具摆瘤..? 弊霸 酒囱 版快鳖瘤 乐促搁 搬惫 眠啊 可记阑 林绢具.
	//				if (SKMeshComp->AnimationData.AnimToPlay){
	//					// AnimToPlay 技泼等 吧 荤侩窍聪 AnimToPlay 甫 厚快瘤 给窍摆绰单..
	//					SKMeshComp->PlayAnimation(SKMeshComp->AnimationData.AnimToPlay, true);
	//				}
	//				SKMeshComp->SetVisibility(true);
	//				ThisGroupSKActor->SetActorHiddenInGame(false);
	//			}
	//		}
	//	}
	//	MatineeActor->TermInterp();
	//}

	//EssentialBeginHandlingCommon();
}

void AB2StageEventDirector::EssentialBeginHandlingCommon()
{
	// 吝汗秦辑 阂副 荐 乐促.

	SAFE_STAGEMANAGER_CHECK
	if (CachedStageManager)
	{
		// 捞 矫痢俊辑 StageManager 绰 鞘夸茄 蔼阑 甸绊 乐绢具 茄促. (酒付档 ABladeIIGameMode::PreBeginPlay 矫痢俊)
		CurrentStageId = CachedStageManager->GetCurrentClientStageId();
		CurrentStageDifficulty = CachedStageManager->GetStageDifficultyLevel();
	}

//#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
//	// 牢胶畔胶 棺 胶抛捞瘤 喊 save state 佬绢甸烙. 佬绢甸牢 函荐客 荤侩磊 汲沥俊 蝶扼 skip 阑 荤侩窍霸 瞪 荐 乐促.
//	GConfig->GetBool(*GetPlayedAtLeastOnceSectionString(), *GetPlayedAtLeastOnceKeyString(CurrentStageId), bPlayedAtLeastOnce, GB2LocalCutSceneSavedStateIni);
//#endif
}

int32 AB2StageEventDirector::GetAllowedMatineeTrackPointLightNum() const
{
	TConsoleVariableData<int32>* MobileNumDynamicPointLightsCVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.MobileNumDynamicPointLights"));
	const int32 MobileNumDynamicPointLights = MobileNumDynamicPointLightsCVar ? MobileNumDynamicPointLightsCVar->GetValueOnGameThread() : 2;
	// 敲饭捞绢 point light 甫 荤侩且波搁 弊芭 哗具.
	// 咯扁辑 bAllowOtherPlayerPointLight 啊 眠啊肺 甸绢哎 搏单.. 弊巴档 老窜 1 肺 摹磊..
	return bHidePlayerPointLight ? MobileNumDynamicPointLights : (bAllowOtherPlayerPointLight ? (MobileNumDynamicPointLights - 2) : (MobileNumDynamicPointLights - 1));
}

void AB2StageEventDirector::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	
	if (GIsEditor && GIsPlayInEditorWorld == false)
	{
		SafetyProcessMatineeSkActors(); // BeginPlay 俊辑 窍蠢聪 俊叼磐 傈侩栏肺 咯扁辑 窍绰 霸 炒摆瘤..?

		SyncFromPreviewSKCompPlacement(); // 橇府轰 哪器惩飘肺 祈笼茄 困摹 教农
	}
#endif
}

void AB2StageEventDirector::BeginDestroy()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::BeginDestroy"));

	Super::BeginDestroy();

	// ESEV_ComponentTrigger 侩栏肺 瘤沥茄 哪器惩飘狼 BeginOverlap 殿废 秦力
	if (DirectingEvent == EStageEvent::ESEV_ComponentTrigger && TriggerComponentName != NAME_None)
	{
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(FindComponentByName(TriggerComponentName));
		if (PrimComp)
		{
			PrimComp->OnComponentBeginOverlap.RemoveDynamic(this, &AB2StageEventDirector::SelectedComponentTriggerBeginOverlapCB);
		}
	}


#if WITH_EDITOR
	// Need to manually destroy them..?
	DestroyPreviewComponents(true);
#endif
}

// Called every frame
void AB2StageEventDirector::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	// 搬惫 刀磊利牢 Tick 风凭栏肺..
	if (IsPlayingShow())
	{
		if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
		{
			for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
			{
				FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
				FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];

				// BeginSingleShow 俊辑 窍绰 老何 馆汗.
				if (CurrShowSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_ContinuouslyAdjustToPlayActor)
				{
					// Position 父 蝶扼啊档废.. 窍妨绊 沁绰单 购啊 肋 救登绰焙
					AdjustSelfPlacementToPlayActor(CurrShowState.GetPlayActor(this), CurrShowSetting.ManualRelativeLocation, CurrShowSetting.ManualRelativeRotation);

					// ExportedCamera 葛靛俊辑绰 ExportedEventScene 俊 窜瘤 檬扁 困摹父 乐栏骨肺 SetSESCameraMovementFromPlayActor 甫 概 橇饭烙 茄促绊 秦档 狼固 绝澜.

					break; // 窍唱父 贸府茄促. 咯矾俺 技泼捞 乐绰 扒 PostEditChangeProperty 俊辑 版绊窃
				}
			}
		}
	}
}

void AB2StageEventDirector::SelectedComponentTriggerBeginOverlapCB(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bool bTargetIsLocalCharacter = false;

	//ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//
	//if (!pPlayer)
	//	return;

	//if (pPlayer->IsQTEMounting())
	//	bTargetIsLocalCharacter = Cast<ABladeIICharacter>(OtherActor) == pPlayer->GetQTEActor();
	//else
	//	bTargetIsLocalCharacter = Cast<ABladeIIPlayer>(OtherActor) == pPlayer;


	//// DirectingEvent 啊 ESEV_ComponentTrigger 牢 版快 矫累 贸府.
	//// 被捞 八荤甫 救 秦档 登摆瘤父 抗狼惑
	//if (DirectingEvent == EStageEvent::ESEV_ComponentTrigger && TriggerComponentName != NAME_None &&
	//	bTargetIsLocalCharacter && // 拱沸 泅犁 敲饭捞窍绰 某腐磐俊 措秦辑父 + 啪铰吝捞搁 呕巴栏肺 厚背
	//	SweepResult.GetComponent() == Cast<UPrimitiveComponent>(FindComponentByName(TriggerComponentName)) &&
	//	!bDirectingEventConditionMet)
	//{
	//	bDirectingEventConditionMet = true;
	//	BeginShowOrBeginCountDown();
	//}
}

void AB2StageEventDirector::CustomBeginShow()
{
	BeginShow(); // 老窜篮 窜鉴 call. 炼扒捞扼搁.. BeginPlay 捞饶俊父 啊瓷 せ
}

void AB2StageEventDirector::CustomFinishShow()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::CustomFinishShow"));

	//// 泅犁 敲饭捞 档吝牢瘤 惑包绝捞 阂副 荐 乐绢具 窃. FinishShow 俊辑 贸府窍绰 巴档 粱 乐澜.

	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && bPCSyncToMatineeStart && bCurrentPCSyncToMatineeStart)
	//{// 付萍聪 PCSync 档吝捞扼搁 角龙利牢 矫累炼瞒 窍瘤 臼篮 巴捞扼 矫累阑 秦 初酒具 窃.
	//	BeginMatineeFromPCSyncToMatineeStart();
	//}

	//for (int32 SI = 0; SI < ShowStates.Num(); ++SI)
	//{
	//	FPlayActorShowRuntimeState& CurrState = ShowStates[SI];
	//	
	//	if (CurrState.bShowStarted == false)
	//	{
	//		// 矫累 措扁 鸥捞赣啊 难柳 惑怕老 荐 乐栏骨肺 力芭
	//		GetWorldTimerManager().ClearTimer(CurrState.ShowStartDelayTimerHandle);
	//	}
	//}

	//// 扁夯利栏肺绰 磊悼栏肺 Stop 捞 阂覆.
	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor)
	//{
	//	if (MatineeActor->bIsPlaying)
	//	{
	//		MatineeActor->Stop(); // 档吝俊 碍力辆丰窍绰 芭扼搁 捞吧 烹秦辑 肚促矫 CustomFinishShow 啊 角青瞪 巴.
	//		return;
	//	}
	//}

	//APlayerCameraManager* CamManager = UGameplayStatics::GetLocalPlayerCameraManager(this);
	//if (CamManager)
	//{
	//	CamManager->StopAllCameraAnims();
	//}

	//// 付萍聪狼 版快 档吝俊 场朝 荐档 乐栏聪 敲饭捞甫 场栏肺 技泼秦辑 困摹 教农 鞍篮 巴捞 狼档茄 措肺 悼累窍档废 窃.
	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor && MatineeActor->MatineeData && 
	//	MatineeActor->InterpPosition < MatineeActor->MatineeData->InterpLength)
	//{
	//	MatineeActor->SetPosition(MatineeActor->MatineeData->InterpLength);
	//}

	//FinishShow();
}

bool AB2StageEventDirector::CanPlayShow()
{
//#if !UE_BUILD_SHIPPING
//	extern bool GCheat_CompleteIgnoreEventScene;
//	if (GCheat_CompleteIgnoreEventScene) // 捞扒 酒抗 矫累炼瞒 窍瘤 臼绰 摹飘捞促. 霸烙 敲饭捞俊 瘤厘捞 哎 荐 乐栏哥 漂喊茄 抛胶飘甫 困秦 荤侩茄促. 老馆利牢 楷免 厘搁 烹苞绰 GCheat_SkipEventSceneCount 阑 曼绊.
//	{
//		return false;
//	}
//#endif
//
//	// 肋给等 技泼狼 版快
//	if ((OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor == NULL) ||
//		(OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera && CameraAnim == NULL))
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		if (GIsEditor)
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] %s 俊 泅犁 Overall Mode 俊 嘎绰 利例茄 皋牢 俊悸捞 汲沥登绢 乐瘤 臼嚼聪促. Stage Event 甫 角青窍瘤 臼嚼聪促."), *this->GetName())
//				));
//		}
//#endif
//		return false;
//	}
//	/*
//	// 楷搬捞 谗变 版快
//	if (!FBladeIIBlockToSyncNetwork::GetInstance().IsConnectedToServer()
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		&& !(CachedStageManager && CachedStageManager->DevOnlyIsStandalonePlay()) // 俊叼磐 橇府轰 敲饭捞, 疙矫利栏肺 Standalone 葛靛肺 敲饭捞窍绰 版快 殿. 楷搬 咯何 惑包绝捞 瘤加.
//#endif
//		)
//	{
//		return false;
//	}
//	*/
//
//	// ManageModeLeastSkip 篮 泅 拌裙惑 绝促. Skip 窍绰 霸 酒聪扼 弊成 竭鳖绰 巴父.
//	if (CheckSkipForManageMode(this, ManageMode, false /*bManageModeLeastSkip*/) == EStageEventSkipType::SEST_CompleteIgnore){ // 咯辨 烹苞秦档 促矫 八荤窍霸 瞪 巴.
//		return false;
//	}
//
//	if (IsStageSupported(CurrentStageId, CurrentStageDifficulty) == false){
//		return false;
//	}
//	
//	if (bPlayedOnceForThisStage){ // 绢痘霸 窍搁 茄 胶抛捞瘤 救俊辑 咯矾锅 敲饭捞啊 啊瓷且瘤档 葛福摆瘤父 被捞 弊凡 鞘夸啊.. 宝洒 困氰且 荐档 乐栏聪 盔玫 豪尖.
//		return false;
//	}
//
//	// 可记俊 蝶弗 skip 鞍篮 扒 咯扁辑 return false 窍绰 侥栏肺 贸府窍瘤 臼绰促. BeginShow 流饶 官肺 CustomFinishShow 甫 窍绰 侥栏肺 贸府秦具 且 巴.
//
//	return true;
//}
//
//// 岿靛 犁肺爹 绝捞 Event 厘搁阑 促矫 敲饭捞 啊瓷窍档废. 葛滴 悼老茄 匙捞怪.
//void AB2StageEventDirector::ResetAssumeNoWorldLoading()
//{
//#if !UE_BUILD_SHIPPING
//	CheckResetAssumeNoWorldLoadingCondition(this);
//#endif
//
//	// 酒流 敲饭捞 救 等 吧 被捞 避绢 何胶烦 父甸 鞘夸 绝瘤. 角力肺 粱 何累侩捞 惯斑登菌澜.
//	if (!bPlayedOnceForThisStage) {
//		return;
//	}
//	bPlayedOnceForThisStage = false;
//
//	if (MatineeActor && MatineeActor->bIsPlaying)
//	{
//		MatineeActor->Stop();
//	}
//
//	if (bCurrentlyPlaying) {
//		CustomFinishShow();
//	}	
//
//	// 敲饭捞 救 等 吧 SetPosition(0) 沁阑 锭 埃趣 何累侩捞 乐带 吧肺 毫辑 捞芭 磊眉啊 肯寒茄 府悸 荐窜篮 酒囱 淀 窍瘤父 措眉肺 冈塞.
//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor && MatineeActor->MatineeData)
//	{
//		MatineeActor->SetPosition(0.0f);
//
//		// 急琶利栏肺 殿擂窍绰 飘发狼 版快 厚劝己拳等 巴捞 乐阑 巴捞骨肺 葛滴 enable .. 窍搁 贸澜何磐 disable 茄 巴档 enable 登摆瘤? 捞 扁瓷阑 静绰 饭骇俊急 炼缴秦辑 荤侩秦具..
//		for (UInterpGroup* ThisInterpGroup : MatineeActor->MatineeData->InterpGroups)
//		{
//			if (ThisInterpGroup)
//			{
//				for (UInterpTrack* ThisTrack : ThisInterpGroup->InterpTracks)
//				{
//					if (ThisTrack) {
//						ThisTrack->EnableTrack(true);
//					}
//				}
//			}
//		}
//	}
	return false;
}

EStageEventSkipType AB2StageEventDirector::CheckSkipForManageMode(UObject* WorldContextObject, EStageEventManageMode InManageMode, bool bLeastSkip)
{
	//if (InManageMode == EStageEventManageMode::SEMM_Always){
	//	return EStageEventSkipType::SEST_Play;
	//}

	//AB2StageGameMode* StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	//AB2StageManager* StageMgr = StageGM ? StageGM->GetStageManager() : NULL;

	//const bool bIsStageScenarioMode = StageMgr ? StageMgr->IsScenarioMode() : false;

	//if (IsAdventureModeGame(WorldContextObject))
	//{
	//	if ((InManageMode == EStageEventManageMode::SEMM_ScenarioOnly && bIsStageScenarioMode) ||
	//		(InManageMode == EStageEventManageMode::SEMM_RegularStageOnly && !bIsStageScenarioMode) ||
	//		InManageMode == EStageEventManageMode::SEMM_AlwaysForAdventure)
	//	{
	//		return EStageEventSkipType::SEST_Play;
	//	}
	//}

	//// bLeastSkip 俊 蝶弗 盒扁绰 StageEventDirector 狼 郴何利牢 悼累 瞒捞. 夯 扁瓷捞 累悼窍瘤 臼绰 扒 付蛮啊瘤. 促弗 镑俊辑 荤侩且 锭 厚搅茄 漂喊茄 荤沥捞 乐瘤 臼篮 捞惑 喊 狼固绰 绝阑 巴.
	//// -> 2017.03 扁霖 扁裙栏肺绰 LeastSkip 篮 绝绊 荤侩 救登绰 芭绰 CompleteIgnore 老 挥. 巢败父 狄.
	//return bLeastSkip ? EStageEventSkipType::SEST_LeastSkip : EStageEventSkipType::SEST_CompleteIgnore;
	return EStageEventSkipType::SEST_Play;
}

bool AB2StageEventDirector::IsAdventureModeGame(UObject* WorldContextObject)
{
	//AGameMode* CurentGM = Cast<AGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	//// 眠饶 StageGameMode 俊辑 惑加罐篮 促弗 霸烙葛靛啊 甸绢吭阑 锭 眠啊 八荤. 老馆 胶抛捞瘤 敲饭捞, 溜 "葛氰" 葛靛 咯何甫 焊绊磊 窃.
	//return (Cast<AB2StageGameMode>(CurentGM) != NULL);
	return false;
}

void AB2StageEventDirector::SetVisibilityMatineePuppetPointLight(bool bVislble)
{
	check(ShowStates.Num() == ShowSettings.Num());
	for (int32 SI = 0; SI < ShowStates.Num(); ++SI)
	{
		FPlayActorShowRuntimeState& ThisState = ShowStates[SI];

		ABladeIICharacter* ThisPlayActor = ThisState.GetPlayActor(this);

		if (ThisPlayActor && ThisState.CtrlMatineePuppet)
		{
			if (ThisPlayActor)
				ThisPlayActor->SetVisibilityMatineePuppetPointLight(ThisState.CtrlMatineePuppet, bVislble);
		}
	}
}

bool AB2StageEventDirector::IsExpectedForGameMode(class ABladeIIGameMode* InB2GM)
{ // 霸烙葛靛 敲饭捞 档吝 角力 敲饭捞啊 抗惑登绰瘤 咯何.
	//return IsStageSupportedForGameMode(InB2GM) && (CheckSkipForManageMode(InB2GM, ManageMode, false) == EStageEventSkipType::SEST_Play);
	return false;
}
bool AB2StageEventDirector::IsStageSupportedForGameMode(class ABladeIIGameMode* InB2GM) const
{
	// 捞霸 True 咯档 角力 敲饭捞绰 救 瞪 荐 乐促. 弊 啊瓷己阑 焊促 犬角窍霸 眉农窍妨搁 IsExpectedForGameMode 甫 毫具.

	//AB2StageGameModeBase* CastedSGM = Cast<AB2StageGameModeBase>(InB2GM);
	//AB2StageManager* StageManagerIfAny = CastedSGM ? CastedSGM->GetStageManager() : NULL;
	//if (StageManagerIfAny)
	//{
	//	return IsStageSupported(StageManagerIfAny->GetCurrentClientStageId(), StageManagerIfAny->GetStageDifficultyLevel());
	//}
	return true; // StageGameMode 啊 酒囱 版快绰 扁夯 瘤盔登绰 吧肺 夯促. 喊档 胶抛捞瘤 俺充捞 绝捞 扁夯 悼累 秒鞭牢 巴.
}
bool AB2StageEventDirector::IsStageSupported(int32 InStageId, EStageDifficulty InDifficulty) const
{
	return IsStageNumSupported(InStageId) && IsStageDifficultySupported(InDifficulty);
}

bool AB2StageEventDirector::IsStageNumSupported(int32 InStageId) const
{
	if (SupportedStages.Num() == 0)
	{
		return true; // 厚绢 乐绰 版快 扁夯栏肺 葛电 胶抛捞瘤 瘤盔
	}
	for (int32 CurrStage : SupportedStages)
	{
		if (CurrStage == InStageId)
		{
			return true;
		}
	}
	return false;
}
bool AB2StageEventDirector::IsStageDifficultySupported(EStageDifficulty InDifficulty) const
{
	if (SupportedDifficulties.Num() == 0)
	{
		return true; // 厚绢 乐绰 版快 扁夯栏肺 葛电 抄捞档 瘤盔
	}
	const int32 TestDifficultyInt = static_cast<int32>(InDifficulty); // 恐 绢叼急 EStageDifficulty 甫 揪绊 绢叼急 Int 甫 静绊 沁绰瘤 肋 扁撅捞 救巢. 傲 沥脚绝绢辑 弊繁 淀
	for (int32 CurrDifficulty : SupportedDifficulties)
	{
		if (CurrDifficulty == TestDifficultyInt)
		{
			return true;
		}
	}
	return false;
}

bool AB2StageEventDirector::IsPossibleAdventureModeBeginEvent()
{
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	//return (
	//	IsAdventureModeGame(this) &&
	//	(
	//		DirectingEvent == EStageEvent::ESEV_Begin || 
	//		// 角龙利栏肺 胶抛捞瘤 矫累 楷免阑 MobAppear 肺 窍绰瘤扼 捞吧 魄喊且 荐 乐绰霸 EventWidgetType 烙 ぱ.ぱ;
	//		(DirectingEvent == EStageEvent::ESEV_MobAppear && EventWidgetType == EStageEventWidgetType::StageBegin) || 
	//		// 辟单 捞哩 傲 贸澜栏肺 唱坷绰 捞亥飘 纠牢瘤 咯何肺..せ
	//		(B2GM && !B2GM->HasAnyStageEventScenePlayed())
	//	)
	//	);
	return true;
}

void AB2StageEventDirector::BeginShow()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::BeginShow"));
//
//	if (CanPlayShow() == false)
//	{
//		return;
//	}
//
//	bool bisInTutorialGameMode = (Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this)) != nullptr);
//	
//	if (bisInTutorialGameMode)
//	{
//		// 譬配府倔锭 QTE 惑怕肺 官肺 付萍聪 柳青
//		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerController(this)->GetPawn());
//
//		if (pPlayer && pPlayer->IsInQTEState())
//		{
//			if (pPlayer->QTEType == EQTEType::EQT_Mount)
//			{
//				pPlayer->StopQTEMount();
//				pPlayer->SetHideFromEventDirector(true);
//			}
//		}
//	}
//	else
//	{
//		// <--QTE吝捞搁 妮归 抗距窍绊 府畔
//		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerController(this)->GetPawn());
//
//		if (pPlayer && pPlayer->IsInQTEState())
//		{
//			pPlayer->AddQTEOnEndCallBack(FQTEOnEnd::CreateLambda([this]() { this->BeginShow(); }));
//
//			// 呕巴 QTE搁 郴府扁
//			if (pPlayer->QTEType == EQTEType::EQT_Mount)
//				pPlayer->StopQTEMount();
//
//			// 抗寇贸府 碱技泼 扁馆栏肺 见扁绰扒 固府 茄锅 秦淋.
//			if (DirectingEvent == EStageEvent::ESEV_MobAppear)
//			{
//				// Settings 客 RuntimeState 箭磊甫 嘎苗霖促.
//				ShowStates.Empty();
//				for (int32 SI = 0; SI < ShowSettings.Num(); ++SI)
//				{
//					ShowStates.Add(FPlayActorShowRuntimeState());
//				}
//
//				for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
//				{
//					BeginSingleShow(EI);
//				}
//
//				EnsureHidePlayActorsForControlledMatinee();
//
//				// 敲饭捞绢绰 救见辫
//				pPlayer->SetHideFromEventDirector(false);
//			}
//
//			return;
//		}
//
//		// -->QTE吝捞搁 妮归 抗距窍绊 府畔 场
//	}
//	
//	B2_SCOPED_TIME_LOG(FString::Printf(TEXT("AB2StageEventDirector::BeginShow %s"), *GetName()));
//
//	bEnterCinematicMode = false;
//
//	// 唱哗绊 促 胶砰!
//	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	if (B2GM)
//	{
//		B2GM->StopAllStageEventDirector(this);
//	}
//
//	if (RandomMatineeGroupNum > 1)
//	{
//		RandomlySetMatineeGroupNum = FMath::RandRange(1, RandomMatineeGroupNum);
//	}
//
//	// BP 俊辑 漂喊洒 捞亥飘甫 罐酒辑 贸府窍绊磊 茄促搁..
//	if (ShowBeginEventName != NAME_None)
//	{
//		GEngine->HandleCeCommand(GetWorld(), *(ShowBeginEventName.ToString()), *GLog);
//	}
//	
//	ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
//	ABladeIIPlayerController* LocalB2PC = GetB2LocalPlayerCon();
//
//	if (DirectingEvent == EStageEvent::ESEV_Clear && ClearEventPhase == EStageClearEventPhase::ESCE_Phase1 && Cast<AB2StageGameMode>(GetB2GameMode()))
//	{
//		// StageClear UI 啊 笛肺 唱吹绊 扁粮 Clear 俊辑狼 铰府器令 楷免篮 UIStageClear 饶狼 UIStageResult 客 窃膊 柳青登骨肺 贸澜俊绰 BeginShow 措脚 喊档 贸府甫 困茄 脚龋甫 焊辰促.
//		// 捞 Phase1 档 喊档狼 EStageEvent 肺 且 荐档 乐瘤父 芭狼 UI 困林捞骨肺 蝶肺 贸府秦 焊绊 泅 扁瓷栏肺 夸备荤亲 皑寸捞 救等促搁 眠饶 EStageEvent 档 付蛮啊瘤肺 笛肺 唱床杭 荐 乐促.
//		
//		MyRelevantUIName = UIFName::StageClear; // 捞扒 EventWidgetType 眉农 绝捞 粱 漂荐窍霸 技泼登绰单 Phase1 磊眉啊 荤角惑 StageClear UI 肺父 捞风绢瘤绰 吧 皑救.
//		BeginStageClearScene_Phase1Class<>::GetInstance().Signal();
//
//		ApplyStageEventTimeDilation();
//		
//		// 咯辨 烹秦 技泼茄 角龙利牢 弥辆 辆丰(铰府) 楷免 惯悼篮 AB2StageManager::BeginStageClearScene_Phase2 曼绊.
//		// 促矫 甸绢棵 锭绰 BeginShow 甫 窍档废 ClearEventPhase 技泼.
//		ClearEventPhase = EStageClearEventPhase::ESCE_Phase2;
//
//		if (LocalB2Player)
//		{ // bForceStopPlayerAttackState 俊辑 窍绰 贸府牢单 府畔阑 秦 滚府聪 咯扁辑 蝶肺.. ぱぱ
//			LocalB2Player->SignalStopAttackImmediate();
//		}
//
//		return;
//	}
//		
//	if (ShouldApplyCinematicScalabilityDuringShow())
//	{
//		if (LocalB2Player) {
//			LocalB2Player->SetupLODInfoForCinematic(); // 肺拿 敲饭捞绢绰 楷免 厘搁 老何啊 酒聪歹扼档 墨皋扼况农俊 狼秦 胶媚哎 荐档 乐栏聪 扁夯 cinematic 侩栏肺 技泼
//		}
//		B2Scalability::ApplyStageEventSpecificScalabilitySetting(this, true); // 肺拿 敲饭捞绢 LOD 技泼苞 付蛮啊瘤肺 楷免 侩 漂喊 scalability 牧飘费 利侩
//	}
//
//	// Settings 客 RuntimeState 箭磊甫 嘎苗霖促.
//	ShowStates.Empty();
//	for (int32 SI = 0; SI < ShowSettings.Num(); ++SI)
//	{
//		ShowStates.Add(FPlayActorShowRuntimeState());
//	}
//
//	check(ShowSettings.Num() == ShowStates.Num());
//	
//	if (LocalB2Player)
//	{
//		CurrentActiveCamera = LocalB2Player->TopDownCameraComponent; // 酒付档 捞吧 蜡瘤窍瘤绰 臼阑 巴捞促..
//
//		if ((DirectingEvent != EStageEvent::ESEV_MobDying && DirectingEvent != EStageEvent::ESEV_FinishAttack))
//		{
//			/*
//			// 楷免 傈 傍拜吝捞扼档 楷免 场 矫痢俊 傍拜阑 捞绢啊瘤 臼酒具 窃
//			if (LocalB2Player->IsKeepAttacking() && bForceStopPlayerAttackState) // bForceStopPlayerAttackState 炼扒阑 眠啊肺 八荤.. 盔贰 捞率 格利捞 酒囱变 沁绰单 措帆 嘎绰促.
//			{
//				bWasPlayerAttacking = true;
//				LocalB2Player->bCachedPressedInCinematic = true;
//			}
//			*/
//		}
//		else
//		{
//			// Boss Kill event, should stop Camera Anims
//			if (LocalB2Player->CameraAnimPlayInstance)
//			{
//				LocalB2Player->CameraAnimPlayInstance->AddCustomNotifyActor(LocalB2Player);
//				LocalB2Player->RemainCameraAnims = 0;
//				LocalB2Player->CameraAnimPlayInstance->Stop(true);
//			}
//		}
//
//		if (bForceStopPlayerAttackState){
//			// Stop repeating input for attack/guard
//			LocalB2Player->StopRepeatingInput();
//		}
//	}
//
//	bAllManualCameraEnded = false;
//	bCameraAnimFinished = false;
//	bControlledMatineeFinished = false;
//	bRelevantUIOpenComplete = false;
//
//	// 墨皋扼 技泼何磐
//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera)
//	{
//		APlayerCameraManager* CamManager = UGameplayStatics::GetLocalPlayerCameraManager(this);
//		if (CamManager && CameraAnim)
//		{
//			// 老窜 葛滴 deactivate 甫 秦 初绊..
//			DeactivateAllManualCams();
//			
//			// 捞傈俊 劝己拳 等 巴捞 乐促搁 葛滴 辆丰. 弊繁单 荤角 捞犯霸 秦档 show 档吝俊 敲饭捞 窍绰 局聪皋捞记俊 墨皋扼 局丛捞 嘿绢 乐促搁 弊霸 利侩捞 瞪 荐 乐澜.
//			// 蝶扼辑 SetStageEventShowState 俊 狼秦 甸绢啊绰 AnimBP 狼 秦寸 局聪皋捞记阑 利例洒 技泼秦具 窃.
//			CamManager->StopAllCameraAnims(true); 
//
//			// CameraAnimApplyCamIndex 俊 蝶扼 扁夯 墨皋扼 棺 view target 咀磐 急琶
//			AActor* CameraAnimViewTarget = this;
//			if (CameraAnimApplyCamIndex >= 0 && CameraAnimApplyCamIndex < MAX_DIRECTOR_CAMERA)
//			{
//				CurrentActiveCamera = DirectorCamArrayRef[CameraAnimApplyCamIndex];
//			}
//			else
//			{
//				if (LocalB2Player)
//				{
//					CurrentActiveCamera = LocalB2Player->TopDownCameraComponent;
//					CameraAnimViewTarget = LocalB2Player;
//				}
//			}
//			CurrentActiveCamera->Activate();
//
//			// 狼档啊 敲饭捞绢 率俊辑何磐 喉坊爹 登绰 芭扼搁 促矫 难林变 秦具 窃.
//			if (CameraAnimInitialBlendingTime > 0.0f)
//			{
//				if (LocalB2Player && LocalB2PC)
//				{
//					LocalB2Player->TopDownCameraComponent->Activate();
//					LocalB2PC->SetViewTargetWithBlend(LocalB2Player, 0.0f);
//				}
//			}
//
//			if (LocalB2PC)
//				LocalB2PC->SetViewTargetWithBlend(CameraAnimViewTarget, CameraAnimInitialBlendingTime, EViewTargetBlendFunction::VTBlend_Linear); // BlendFunction 鞘夸窍搁 汲沥窍霸..
//
//			// 鞘夸窍搁 CameraAnimInst 甫 某教秦辑 荤侩.
//			UCameraAnimInst* CamAnimInst = CamManager->PlayCameraAnim(CameraAnim);
//			if (CamAnimInst)
//			{
//				if (bStandbyAtTheEnd)
//				{
//					CamAnimInst->bStayAtEnd = true; // 墨皋扼 局聪皋捞记 辆丰 惑怕俊辑 蜡瘤窍霸 瞪 巴. 捞 版快 场郴绰 扒 CustomFinishShow 甫 荤侩.
//				}
//				CamAnimInst->AddCustomNotifyActor(this); // 辆丰 烹瘤甫 罐扁 困茄 殿废. 辆丰 矫 NotifyActorCustomEvent 肺.
//			}
//		}
//	}
//	else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//	{
//		// Controlled Matinee 狼 版快绰 窍窜 BeginSingleShow 甫 delay 绝捞 贸府茄 饶俊 Play 甫 且 巴.
//
//		if (MatineeActor)
//		{
//			// MatineeActor 客狼 霉 炼快. 捞率俊 汲沥茄 函荐甸阑 逞败淋
//			MatineeActor->FirstSkipFrameNum = MatineeFirstSkipFrames;
//			MatineeActor->bStayAtEnd = bStandbyAtTheEnd; // StandbyAtTheEnd 俊辑绰 付萍聪 辆丰 惑怕俊辑 蜡瘤窍霸 瞪 巴. 捞 版快 场郴绰 扒 CustomFinishShow 甫 荤侩.
//			MatineeActor->bNoCinematicModeControl = true; // StageEventDirector 俊 楷搬茄 捞惑 CinematicMode 绰 咯扁辑 炼例窃.
//			MatineeActor->SetPosition(0.0f, true); // 老何 犁劝侩窍绰 付萍聪 纠捞 乐澜. MatineeActor 率狼 bForceStartPos 甫 眉农窍搁 登绰 芭变 窃.
//			MatineeActor->InitInterp(); // BeginSingleShow 甫 烹秦 弊缝 actor 甸俊 立辟窍扁 傈俊 Init 阑 荐悼栏肺 茄锅 秦具 窍绰 淀.
//
//#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
//			if (bDisableStageEventLights) // 叼官捞胶俊辑狼 己瓷 抛胶飘 侩档烙. 酒付档 牢霸烙俊辑 芭狼 蜡老窍霸 喊档狼 悼利 扼捞飘甫 荤侩窍绰 何盒老 淀?
//			{
//				SetAllPointLightGroupAffectWorld(false);
//			}
//			else
//#endif
//			{
//				// 扼捞飘 技泼何磐 难 初绊.. 促矫 鞘夸绝绰 扒 波具 窃.
//				SetAllPointLightGroupAffectWorld(true);
//			}
//
//			SelectFinalDirectorTrack(); // bPCDependentDirectorTrack 捞芭唱 Random 牢 版快 殿俊辑 咯矾 Director track 甸捞 乐阑 锭 利例茄 窍唱甫 急琶.
//		}
//	}
//	else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
//	{
//		check(ManualCameraSettings.Num() > 0); // 利绢档 窍唱绰 巢档废 贸府窃.
//
//		// 老窜 葛滴 deactivate 甫 秦 初绊..
//		DeactivateAllManualCams();
//
//		if (LocalB2Player && LocalB2PC)
//		{
//			// 咯扁档 荤角惑 敲饭捞绢 率俊辑何磐 喉坊爹 瞪 巴捞骨肺 犬角洒 难 林磊.
//			LocalB2Player->TopDownCameraComponent->Activate();
//			LocalB2PC->SetViewTargetWithBlend(LocalB2Player, 0.0f);
//		}
//
//		// 霉锅掳 汲沥茄 墨皋扼肺 捞悼.		
//		BeginTargetToManualCamera(0);
//	}
//
//	// BeginSingleShow 俊辑 SimpleEventScene 技泼 矫 荤侩.
//	// -> Director camera component 甸篮 框流捞瘤 臼绰 吧肺 模促. 弊犯霸 啊沥秦具 窍绰 惑炔烙. 蝶扼辑 困摹蔼阑 某教窍瘤 臼澜.
//	//CachedInitialActiveCamLocation = CurrentActiveCamera->GetComponentLocation();
//	//CachedInitialActiveCamRotation = CurrentActiveCamera->GetComponentRotation();
//	
//	// 俺喊 play actor 喊 show 矫累
//	for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
//	{
//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];
//
//		// ControlledMatinee 俊辑绰 StartDelay 绝澜.
//		if (CurrShowSetting.StartDelay > 0.0f && OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//		{
//			GetWorldTimerManager().SetTimer(CurrShowState.ShowStartDelayTimerHandle,
//				FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::DelayedShowStartTimerCB, EI),
//				CurrShowSetting.StartDelay, false);
//		}
//		else
//		{
//			// 溜籍俊辑 矫累
//			BeginSingleShow(EI);
//		}
//	}
//
//	// ControlledMatinee 绰 BeginSingleShow 贸府 饶 Play 官肺 矫累.. 篮 酒聪绊 眠啊肺 敲饭捞绢 教农甫 八荤.
//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//	{
//		// 繁鸥烙 惑炔俊 狼秦 焊捞瘤 富酒具 且 GroupActor 狼 版快, 焊捞瘤 臼绰单档 阂备窍绊 家府啊 唱芭唱 且 荐 乐栏骨肺 力芭.
//		// 抗甫 甸绢 父甸绢绰 初疽栏唱 泅犁 敲饭捞绢 努贰胶客 促福骨肺 唱坷瘤 富酒具 且 弊缝.
//		RuntimeRemoveNonDesiredGroup();
//		// RuntimeRemoveNonDesiredGroup 窍搁辑 荤侩登瘤 臼阑 PointLight 掺绰 霸.. 抗傈俊 捞犯霸 救沁阑 锭 悼累窍带 悸诀捞 乐绢辑 窍唱绰 混妨滴绰 狼固肺.
//		HackSpareLeastPointLight();
//
//		if (CheckForPCSyncToMatineeStart() == false) // true 扼搁 Player sync 饶俊 矫累且 巴.
//		{
//			FinalBeginMatineePlay();
//		}
//
//	}
//
//	// Widget 殿 眠啊利牢 贸府啊 乐绰单 捞巴甸篮 Matinee PCSync 荤侩矫俊绰 夯拜 Matinee play 矫累 饶俊 贸府登绢具 窃.
//	if (bCurrentPCSyncToMatineeStart == false)
//	{
//		DeferredBeginShowHandlingForMatineePCSync();
//	}
//
//	if (bForceStopPlayerAttackState)
//	{
//		// 泅犁 敲饭捞绢 某腐磐啊 霸烙惑怕甫 蜡瘤窍档废 疙矫等 版快搁 拱沸 秦急 救登瘤.
//		if (ShouldMaintainPlayerCharacterState() == false)
//		{
//			if (LocalB2Player)
//				LocalB2Player->SignalStopAttackImmediate(); // 捞饶 AnimBP 俊 狼秦 贸府瞪 巴.
//		}
//	}
//
//	SAFE_STAGEMANAGER_CHECK
//	if (bStopAllMobsWhilePlaying && CachedStageManager)
//	{
//		CachedStageManager->StopOrRestoreAllWorldMobs(true); // 咯扁俊 器窃等 PlayActor 挥 酒聪扼 葛滴 沥瘤.
//		if (bForceCancelAttackMotionAllMob)
//			CachedStageManager->ForceCancelAttackAllWorldMobs();
//	}
//
//	if (B2GM)
//	{// Auto 绰 泪矫 吝窜矫虐绊 Finish 俊辑 汗备
//		if (ShouldStopAutoDuringShow() && B2GM->IsLocalPlayerAutoBattle()){
//			B2GM->TemporaryStopLocalPlayerAuto();
//		}
//	}
//	if (LocalB2Player && ShouldBeInvincibleDuringShow()) {
//		LocalB2Player->SetInvincibleByStageEvent(true);
//	}
//
//	if (OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//	{ 
//		if (LocalB2PC)
//		{
//			bool bHiddenPlayer = LocalB2PC->GetPawn() && LocalB2PC->GetPawn()->bHidden;
//			// Matinee 葛靛啊 酒囱 版快狼 CinematicMode 技泼. 付萍聪俊急 粱 漂喊 包府. 4锅掳 牢磊绰 MovementInput 俊 措茄 芭瘤父 泅犁肺辑绰 傈眉 涝仿俊 措秦 康氢.
//			LocalB2PC->SetCinematicMode(true, bHiddenPlayer, bHideHud, bDisableInput, true);
//		}
//		if(!bForceNoHidePlayer)
//			SetAllPlayerHiddenInGame(true);
//	}
//
//	// 矫累 楷免牢 版快 咯扁俊 吧副 荐 乐促. Slomo 吧绊 瞒剧阜 啊府绰 悼救 荤款靛 惯悼阑 阐
//	// RuntimeRemoveNonDesiredGroup 捞饶俊 甸绢啊具 窃. 促弗 捞蜡肺 disable 等 巴甸阑 吧矾郴具 秦辑
//	if (B2GM && B2GM->IsInPreRenderPhase() && OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//	{
//		TemporaryDisableTracksForPreRender(); // 汗备窍绰 扒 FinishShow 俊辑 窍绰 霸 酒聪扼 GameMode 唱 PreRender 率俊辑 贸府秦具.
//	}
//
//	LevelStreamingOnBegin.ExecuteStreaming(GetWorld(), LevelStreamingOnBeginFlushTH, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::LevelStreamingOnBeginFlushCB));
//	
//	// 敲饭捞 档吝 肺爹栏肺 喉肺欧 吧副 荐 乐绰 波府甫 利例茄 矫痢俊 浇陆 肺爹秦 焊绊磊 窃.
//	if (IsPossibleAdventureModeBeginEvent())
//	{
//		UB2UIManager::GetInstance()->ReqAsyncLoadAsset(UIFName::BossStatus);
//		UB2UIManager::GetInstance()->ReqAsyncLoadAsset(UIFName::ScenarioDialog);
//	}
//
//	// 扁鸥 促弗 镑俊辑狼 贸府甫 困秦 (StageManager)
//	StageEventSceneBeginClass<AB2StageEventDirector*, EStageEvent>::GetInstance().Signal(this, DirectingEvent);
//
//#if BII_STATS
//	FStatisticalPerfStatCollector::bIsCurrentlyInEventScene = true;
//#endif
//
//	if(bCheckPlayOne)
//		bPlayedOnceForThisStage = true; // BeginShow 父 秦档 茄锅 敲饭捞 茄 吧肺 模促.
//	bCurrentlyPlaying = true;
//
//	//
//	// 沥侥 矫累 例瞒绰 场车绊, 酒贰绰 胶诺捞 鞘夸茄 版快.
//	//
//
//	bool bShowSkipped = false;
//
//	// 沥侥 胶诺阑 窍妨搁 弥家茄 BeginShow 绰 茄 饶俊 CustomFinshShow 甫 秦具 窃. 楷免 厘搁 辆丰 饶 困摹 教农啊 鞘夸茄 版快 锭巩俊.
//	// -> 老窜 捞 版肺肺 啊霸 瞪 老篮 绝阑 巴.
//	if (CheckSkipForManageMode(this, ManageMode, false/*bManageModeLeastSkip*/) == EStageEventSkipType::SEST_LeastSkip){
//		CustomFinishShow();
//		bShowSkipped = true;
//	}
//
//#if !UE_BUILD_SHIPPING
//	extern int32 GCheat_SkipEventSceneCount;
//	if (GCheat_SkipEventSceneCount > 0 && !bShowSkipped){
//		if (DirectingEvent != EStageEvent::ESEV_Clear){ // Clear 捞亥飘绰 霸烙 辆丰 皋春甫 困秦 鞘夸窍促 焊聪.. 弊贰档 UI 皋春父 官肺 焊咯临 荐 乐绰 规过阑 积阿秦 焊磊..
//			CustomFinishShow();
//			--GCheat_SkipEventSceneCount;
//		}
//	}
//#endif
}

void AB2StageEventDirector::DeferredBeginShowHandlingForMatineePCSync()
{
	// Widget.
	StartEventWidgetOrStartCountdown();

	// 傈眉 矫埃捞 瘤沥登菌促搁 辆丰 鸥捞赣 on. Controlled Matinee 狼 版快档 利侩篮 登摆瘤父 付萍聪 傈眉 辨捞焊促 辨搁 绢瞒乔 家侩 绝阑 巴.
	if (IsOverallLengthConstrainted())
	{
		GetWorldTimerManager().SetTimer(OverallLengthTimerHandle, this, &AB2StageEventDirector::OverallLengthTimerCB, SpecifiedOverallLength, false);
	}

	//ABladeIIGameMode* B2GM = GetB2GameMode();
	//if (B2GM && !B2GM->IsInPreRenderPhase()) // PreRender 惑炔俊辑绰 概快 利篮 TimeDilation 阑 啊窍绊 PreRender 场朝 锭 捞率 TimeDilation 阑 汗备且 巴
	//{
	//	ApplyStageEventTimeDilation();
	//}
}

void AB2StageEventDirector::ApplyStageEventTimeDilation()
{
	//float FinalPlayRate = GlobalTimeDilation; // TODO 咯扁俊 荤侩磊啊 急琶茄 play rate 啊 蚌秦龙 荐 乐澜.
	//UGameplayStatics::SetGlobalTimeDilation(this, FinalPlayRate, true); // 咯扁辑绰 FinalPlayRate 甫 咯鸥 促弗 夸家甫 皑救窍瘤 臼篮 角龙 TimeDilation 栏肺 秒鞭.
}

void AB2StageEventDirector::BeginShowOrBeginCountDown()
{
	//if (bDirectingEventConditionMet == false) // 扁夯利牢 炼扒
	//{
	//	return;
	//}

	//// [霸烙敲饭捞咀磐] 俊辑 [付萍聪咀磐] 肺 傈券登扁 傈, 付欧 贸府且 傀捞器牢飘啊 乐促搁, 殿废等 曼炼甫 捞侩窍咯 贸府.
	//if ( ForcedMarkPassedWaypoint )
	//{
	//	ForcedMarkPassedWaypoint->ForceMarkByJumpEvent( nullptr, Cast< ABladeIIPlayer >( UGameplayStatics::GetLocalPlayerController( this )->GetPawn() ) );
	//	if ( OverallStartDelay <= 0.0f )
	//	{
	//		OverallStartDelay += 0.1f;
	//	}
	//}

	//// DependentDirectorActor 啊 汲沥等 版快俊 措茄 眠啊 炼扒 八荤
	//if (HasValidDependentDirectorActorSetting() && bDependentDirectorActorMet == false)
	//{
	//	return;
	//}

	//if (OverallStartDelay > 0.0f)
	//{
	//	GetWorldTimerManager().SetTimer(OverallStartDelayTimerHandle, this, &AB2StageEventDirector::OverallStartDelayTimerCB, OverallStartDelay, false);
	//}
	//else
	//{
	//	BeginShow();
	//}
}

bool AB2StageEventDirector::HasValidDependentDirectorActorSetting()
{
	for (AB2StageEventDirector* CurrSED : DependentDirectorActors)
	{
		if (CurrSED)
		{
			return true; // 窜 窍唱父 甸绢啊 乐绢档 烹苞
		}
	}
	return false;
}

void AB2StageEventDirector::NotifyGameStarted()
{
	if (DirectingEvent == EStageEvent::ESEV_Begin)
	{
		bDirectingEventConditionMet = true;
		BeginShowOrBeginCountDown();
	}
}

void AB2StageEventDirector::NotifyGameClear()
{
	if (DirectingEvent == EStageEvent::ESEV_Clear)
	{
		bDirectingEventConditionMet = true;
		BeginShowOrBeginCountDown();
	}
}

void AB2StageEventDirector::NotifyMatchEnd(bool bAmIWinnder)
{
	bool IsBeginShow = false;

	switch (DirectingEvent)
	{
	case EStageEvent::ESEV_MatchResult:
		IsBeginShow = true;
		break;
	case EStageEvent::ESEV_MatchResultOnlyWinning:
		IsBeginShow = (bAmIWinnder);
		break;
	case EStageEvent::ESEV_MatchResultOnlyDefeat:
		IsBeginShow = (bAmIWinnder == false);
		break;
	}

	if (IsBeginShow)
	{
		bMatchResultEventAsWinner = bAmIWinnder;
		bDirectingEventConditionMet = true;
		BeginShowOrBeginCountDown();
	}
}

void AB2StageEventDirector::NotifyFinishAttack(ABladeIICharacter* Inflictor)
{
	//if (DirectingEvent == EStageEvent::ESEV_FinishAttack)
	//{
	//	// 阜鸥 柳悼. 胶抛捞瘤 葛靛 焊胶各 柳悼篮 酒付 蝶肺 甸绢哎 淀.
	//	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (B2Player)
	//	{
	//		B2Player->ForceFeedback();
	//	}

	//	// 啊秦磊甫 措惑栏肺 惑措谅钎 付萍聪 楷免 矫累
	//	SetUpOutSideCharacter = Inflictor;
	//	BeginShow();
	//}
}

void AB2StageEventDirector::NotifySpawnPoolSingleWaveStart(int32 WaveNum)
{
	// 窜老 饭骇 郴俊辑 荤侩登绰 SpawnPool 篮 老沥窍哥 GameMode 俊辑 舅绊 乐栏搁 凳. 咯扁俊 SpawnPool 沥焊甫 焊郴瘤 臼绰 吧肺 窃.

	//int32 ParsedWaveNum = 0;
	//int32 ParsedMobIndex = 0;
	//ParseForWaveMobSelectKeyword(MobEventMobSelectKeyword, ParsedWaveNum, ParsedMobIndex);
	//
	//if (DirectingEvent == EStageEvent::ESEV_MobAppear && WaveNum == ParsedWaveNum)
	//{
	//	EssentialBeginHandlingCommon();	// 胶抛捞瘤 矫累苞 窃膊窍绰 霉 wave 扼搁 BeginPlay 捞傈俊 捞霸 角青瞪 巴.
	//	
	//	TArray<ABladeIICharacter*> SpawnedMobList;
	//	ABladeIIGameMode* B2GM = GetB2GameMode();
	//	if (B2GM){
	//		B2GM->GetSingleWaveMobList(WaveNum, SpawnedMobList);
	//	}

	//	ABladeIICharacter* PossibleEventTriggeringMob = NULL;
	//	for (int32 MI = 0; MI < SpawnedMobList.Num(); ++MI)
	//	{
	//		if (SpawnedMobList[MI]->GetSpawnPoolWaveObjIndex() == ParsedMobIndex)
	//		{
	//			PossibleEventTriggeringMob = SpawnedMobList[MI];
	//			break;
	//		}
	//	}

	//	if (PossibleEventTriggeringMob == NULL && SpawnedMobList.Num() > 0)
	//	{
	//		PossibleEventTriggeringMob = SpawnedMobList[0]; // 趣矫扼档 肋给等 汲沥捞唱 茄 版快狼 弃归..
	//	}

	//	BII_CHECK(!PossibleEventTriggeringMob || PossibleEventTriggeringMob->IsValidObj());

	//	if (PossibleEventTriggeringMob && CanPlayShow()) // BeginShow 俊辑档 CanPlayShow 八荤甫 窍绰单 UIDoc 技泼阑 咯扁辑 秦具 秦辑 固府 八荤.
	//	{
	//		SyncDataToUIDoc(PossibleEventTriggeringMob);
	//		bDirectingEventConditionMet = true;
	//		BeginShowOrBeginCountDown();
	//	}
	//}	
}

void AB2StageEventDirector::NotifySpawnPoolSingleWaveComplete(int32 WaveNum)
{
	//// 傀捞宏 锅龋父 荤侩
	//int32 ParsedWaveNum = 0;
	//int32 ParsedMobIndex = 0;
	//ParseForWaveMobSelectKeyword(MobEventMobSelectKeyword, ParsedWaveNum, ParsedMobIndex);

	//if (DirectingEvent == EStageEvent::ESEV_MobWaveClear && WaveNum == ParsedWaveNum && CanPlayShow())
	//{
	//	bDirectingEventConditionMet = true;
	//	BeginShowOrBeginCountDown();
	//}
}

void AB2StageEventDirector::NotifySpawnPoolSingleMobDead(class ABladeIICharacter* DyingMob, int32 WaveNum, int32 MobObjIndex)
{
	// 窜老 饭骇 郴俊辑 荤侩登绰 SpawnPool 篮 老沥窍哥 GameMode 俊辑 舅绊 乐栏搁 凳. 咯扁俊 SpawnPool 沥焊甫 焊郴瘤 臼绰 吧肺 窃.
	//
	//int32 ParsedWaveNum = 0;
	//int32 ParsedMobIndex = 0;
	//ParseForWaveMobSelectKeyword(MobEventMobSelectKeyword, ParsedWaveNum, ParsedMobIndex);

	//if (DirectingEvent == EStageEvent::ESEV_MobDying && DyingMob && WaveNum == ParsedWaveNum && MobObjIndex == ParsedMobIndex)
	//{		
	//	if (CanPlayShow()) // BeginShow 俊辑档 窍绰 八荤牢单 UIDoc 技泼阑 咯扁辑 秦具 秦辑 固府 八荤.
	//	{
	//		BII_CHECK(!DyingMob || DyingMob->IsValidObj());
	//		SyncDataToUIDoc(DyingMob);
	//		bDirectingEventConditionMet = true;

	//		// BeginShow 贸府 档吝 BeginSingleShow 俊辑 GetPlayActor 甫 烹秦 DyingMob 阑 啊廉坷绰 惑炔捞 乐阑 巴牢单, 
	//		// 父老 BeginShow 甫 瘤楷矫挪促芭唱 窍搁 WaveMobSelectKeyword 父 啊瘤绊绰 阂啊瓷 且 巴. MonsterSpawnPool 率俊辑 捞 Notify 流饶 DyingMob 阑 格废俊辑 力芭且 巴捞骨肺.
	//		// 弊繁 霸 鞘夸且瘤 隔扼辑 咯扁肺 逞绢坷绰 DyingMob 阑 WaveNum 苞 MobObjIndex 客 窃膊 某教秦 滴菌促啊 波郴棵 荐 乐档废 窃..
	//		if (NeedDelayedDyingMobCache())
	//		{
	//			DelayedDyingMobCache.SetDyingMobCache(DyingMob, WaveNum, MobObjIndex);
	//		}

	//		BeginShowOrBeginCountDown();
	//	}
	//}
}

void AB2StageEventDirector::NotifySpawnPoolSingleMobPhaseChange(class ABladeIICharacter* ChangedMob, class ABladeIICharacter* DestroyedPrevRef, int32 WaveNum, int32 MobObjIndex, int32 NewPhase)
{
	//int32 ParsedWaveNum = 0;
	//int32 ParsedMobIndex = 0;
	//ParseForWaveMobSelectKeyword(MobEventMobSelectKeyword, ParsedWaveNum, ParsedMobIndex);
	//	
	//BII_CHECK(!ChangedMob || ChangedMob->IsValidObj());

	//if (DirectingEvent == EStageEvent::ESEV_MobPhase && WaveNum == ParsedWaveNum && MobObjIndex == ParsedMobIndex)
	//{
	//	if (CanPlayShow()) // BeginShow 俊辑档 窍绰 八荤牢单 UIDoc 技泼阑 咯扁辑 秦具 秦辑 固府 八荤.
	//	{			
	//		SyncDataToUIDoc(ChangedMob);
	//		bDirectingEventConditionMet = true;
	//		BeginShowOrBeginCountDown();

	//		//ChangedMob狼 Stun/Freeze 惑怕 秦力
	//		ChangedMob->ClearDebuffs();
	//	}
	//}

	//// 楷免 档吝 PlayActor 吝 窍唱啊 (Re-spawn 阑 悼馆窍绰) Phase 函拳甫 芭摹绰 惑炔. 啊瓷茄瘤绰 葛福摆瘤父 措厚绰 秦 笛 鞘夸啊 乐摆促.
	//// 捞 DestroyedPrevRef 绰 valid 窍瘤 臼阑 巴捞促. PlayActor 吝俊 捞吧 甸绊 乐促搁 ChangedMob 栏肺 官层霖促.
	//if (DestroyedPrevRef)
	//{
	//	for (int32 EI = 0; EI < ShowStates.Num(); ++EI)
	//	{
	//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];
	//		if (CurrShowState.GetPlayActor(this) == DestroyedPrevRef)
	//		{
	//			CurrShowState.SetPlayActor(ChangedMob);
	//		}
	//	}
	//}
}

void AB2StageEventDirector::NotifyForgetACharacter(class ABladeIICharacter* CharacterAboutToBeDestroyed)
{
	// CharacterAboutToBeDestroyed 绰 矫胶袍俊辑 力芭登扁 流傈烙. 趣矫扼档 咯扁俊 饭欺繁胶啊 乐促搁 朝赴促.
	//BII_CHECK(CharacterAboutToBeDestroyed && CharacterAboutToBeDestroyed->IsValidObj())
	//if (bCurrentlyPlaying)
	//{
	//	for (FPlayActorShowRuntimeState& CurrState : ShowStates)
	//	{
	//		if (CurrState.GetPlayActor(this) == CharacterAboutToBeDestroyed)
	//		{
	//			CurrState.SetPlayActor(NULL);
	//		}
	//	}
	//	DelayedDyingMobCache.ForgetIfMatch(CharacterAboutToBeDestroyed); // 捞率档 甸绊 乐阑 荐 乐促.
	//}
}

void AB2StageEventDirector::NotifyStageEventUIOpenComplete(FName InOpenedUI)
{
	if (InOpenedUI != NAME_None && InOpenedUI == MyRelevantUIName) // MyRelevantUIName 篮 角力 荤侩矫俊唱 技泼瞪 巴.
	{
		bRelevantUIOpenComplete = true;

		// bRelevantUIOpenComplete 炼扒栏肺 牢秦 场郴瘤 给茄 巴捞 乐促搁 促矫 八荤.
		CheckShowEndCountDown();
		CheckShowEndCountDown_ControlledMatinee(bStandbyAtTheEnd); // bStandbyAtTheEnd 搁 StandByNoti 扼绊 瓜阑 荐 观俊..?
	}
}

void AB2StageEventDirector::DelayedShowStartTimerCB(int32 PlayActorIdx)
{
	if (PlayActorIdx >= 0 && PlayActorIdx < ShowStates.Num())
	{		
		GetWorldTimerManager().ClearTimer(ShowStates[PlayActorIdx].ShowStartDelayTimerHandle);
		BeginSingleShow(PlayActorIdx);
	}
}

void AB2StageEventDirector::AdjustSelfPlacementToPlayActor(class ABladeIICharacter* InPlayActor, FVector& InPlayActorRelativeLocation, FRotator& InPlayActorRelativeRotation, bool bInPositionOnly /*= false*/)
{
	if (InPlayActor)
	{
		// PlayActorRelativeTransform 篮 唱甫 扁霖栏肺 茄 敲饭捞绢狼 Relative 牢单, 弊 馆措啊 鞘夸窍骨肺 inverse
		const FTransform InvRelativeTransform = FTransform(InPlayActorRelativeRotation, InPlayActorRelativeLocation).Inverse();
		const FTransform AdjustTransform = InvRelativeTransform * InPlayActor->GetTransform();

		this->SetActorLocation(AdjustTransform.GetTranslation());
		// 啊阐 Position 父 蝶扼啊绊 酵阑 锭啊 乐澜.
		if (bInPositionOnly == false)
		{
			this->SetActorRotation(AdjustTransform.GetRotation().Rotator());
		}
	}
}

bool AB2StageEventDirector::BeginSingleShow(int32 PlayActorIdx)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::BeginSingleShow"));

	check(ShowSettings.Num() == ShowStates.Num());

	//if (PlayActorIdx >= 0 && PlayActorIdx < ShowSettings.Num())
	//{
	//	FPlayActorShowSettings& ThisSetting = ShowSettings[PlayActorIdx];
	//	FPlayActorShowRuntimeState& ThisState = ShowStates[PlayActorIdx];

	//	if (ThisState.bShowStarted == false)
	//	{
	//		// 快急, Type 俊 蝶扼 某腐磐甫 啊廉咳
	//		ThisState.SetPlayActor(GetRelevantPlayActor(ThisSetting.PlayActorType, ThisSetting.WaveMobSelectKeyword, ThisSetting.PlayActorIndex));
	//		ABladeIICharacter* ThisPlayActor = ThisState.GetPlayActor(this);
	//		ABladeIIPlayer* CastedPlayActorPC = Cast<ABladeIIPlayer>(ThisPlayActor);
	//		if (CastedPlayActorPC)
	//		{ 
	//			switch (DirectingEvent)
	//			{
	//			case EStageEvent::ESEV_Clear:
	//			case EStageEvent::ESEV_MatchResult:
	//			case EStageEvent::ESEV_MatchResultOnlyWinning:
	//			case EStageEvent::ESEV_MatchResultOnlyDefeat:
	//				CastedPlayActorPC->ClearDebuffs();
	//				break;
	//			}
	//			
	//			// BeginShow 俊辑 LocalPC俊 措秦辑绰 窍绰单 怕弊某腐磐唱 匙飘焢 敲饭捞绢啊 甸绢哎 荐档 乐栏聪 咯扁辑档 茄锅.
	//			CastedPlayActorPC->SetupLODInfoForCinematic();
	//		}

	//		if (ThisPlayActor && ThisPlayActor->IsValidObj())
	//		{
	//			// 捞霸 技泼登搁 Dying 牢 版快扼档 角力 dead anim 苞 destroy 贸府啊 瘤楷瞪 巴. 弊府绊 弊繁 版快绰 捞 楷免捞 场抄 饶俊 惑怕 汗备 矫 波 滚府电瘤 力芭窍电瘤 窍霸 瞪 巴 (RestorePlayActorForCtrlMatineeEnd)
	//			ThisPlayActor->SetPlayingDirectedStageEvent(ThisSetting.IsDirectedMode()); 

	//			// Spawn 矫 距埃 哆霸 登骨肺 Snap 何磐 荐青窍哥 父老 角力肺 栋 乐绰 厘搁 殿捞 鞘夸窍促搁 积阿秦 焊磊..
	//			SnapBladeIICharacterToGround(ThisPlayActor);

	//			// 可记俊 蝶扼 肯丰 饶 汗盔且 荐 乐档废 困摹/雀傈蔼阑 归诀秦 敌促.
	//			ThisState.BackupWorldLocation = ThisPlayActor->GetActorLocation();
	//			ThisState.BackupWorldRotation = ThisPlayActor->GetActorRotation();

	//			if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera || OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
	//			{
	//				// 某腐磐狼 Location, Rotation 流立 技泼
	//				if (ThisSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_BringPlayActor)
	//				{
	//					// EPAP_BringPlayActor 捞搁 ExportedCamera 电, ManualCamera 电 啊府瘤 臼绊 困摹 流立 技泼
	//					const FTransform RelativeTransform(ThisSetting.ManualRelativeRotation, ThisSetting.ManualRelativeLocation);
	//					const FTransform ParentToWorld = this->GetTransform();
	//					const FTransform BroughtTransform = RelativeTransform * ParentToWorld;

	//					ThisPlayActor->SetActorLocation(BroughtTransform.GetTranslation()); // Transform 阑 荤侩茄 扒 妊捞农看促.
	//					ThisPlayActor->SetActorRotation(BroughtTransform.GetRotation().Rotator());

	//					SnapBladeIICharacterToGround(ThisPlayActor); // 澜.. 搬惫 snap 篮 秦 拎具 窍妨唱..
	//				}
	//				else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
	//				{
	//					if (ThisSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_AdjustToPlayActor || ThisSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_ContinuouslyAdjustToPlayActor)
	//					{
	//						// ManualCamera 葛靛俊辑狼 AdjustToPlayActor 绰 咀磐 傈眉甫 酒轿府 敲饭捞绢 吝缴栏肺 困摹矫挪促. 
	//						// 官肺 酒贰 ExportedCamera 俊辑狼 AdjustToPlayActor 绰 CameraAnimApplyCamIndex 俊 蝶扼 荤侩登绰 哪器惩飘 窍唱父 炼沥.
	//						// ContinuouslyAdjustToPlayActor 俊辑绰 Tick 俊辑档 捞吧 贸府.
	//						AdjustSelfPlacementToPlayActor(ThisPlayActor, ThisSetting.ManualRelativeLocation, ThisSetting.ManualRelativeRotation);
	//					}
	//				}
	//				else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera)
	//				{
	//					// 酒贰 荤侩窍绰 MoveKeyFrames 甸篮 眠饶 犬厘 啊瓷己阑 困秦 KeyFrame 栏肺 沁栏哥 老窜篮 弊成 檬扁 困摹 啊廉坷扁 困窃.
	//					if (IsExportedEventSceneValid(ThisSetting.ExportedEventScene))
	//					{
	//						if (ThisSetting.PlayActorPlacement != EPlayActorPlacement::EPAP_AdjustToPlayActor && ThisSetting.PlayActorPlacement != EPlayActorPlacement::EPAP_ContinuouslyAdjustToPlayActor)
	//						{
	//							// PlayActor 甫 墨皋扼 扁霖栏肺 困摹矫挪促.
	//							FSimpleEventSceneMoveKeyData& FirstKeyData = ThisSetting.ExportedEventScene->SimpleMoveKeyFrames[0];
	//							SetSESPlayActorMovementFromCamera(FirstKeyData, ThisPlayActor);
	//							// 墨皋扼啊 栋 乐阑 荐 乐栏骨肺 促矫 Snap.. ぱ.ぱ
	//							SnapBladeIICharacterToGround(ThisPlayActor);
	//						}

	//						// AdjustToPlayActor 牢 版快 墨皋扼甫 PlayActor 扁霖栏肺 困摹矫虐哥
	//						// 弊霸 酒聪歹扼档 臭捞 嘎眠扁 瞒盔俊辑 墨皋扼 困摹 炼沥. 酒囱 版快绰 捞固 敲饭捞绢啊 墨皋扼 吝缴栏肺 困摹沁阑 巴.
	//						SetSESCameraMovementFromPlayActor(ThisSetting.ExportedEventScene->InvSimpleMoveKeyFrames[0], ThisPlayActor);
	//					}
	//				}
	//			}
	//			else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//			{
	//				// 咯扁急 ThePlayActor 甫 悸诀窍绰 措脚 扁粮俊 汲摹秦敌 付萍聪 弊缝 actor 甫 PlayActor 客 厚搅窍霸 悸诀且 巴烙. 辨绢瘤聪 蝶肺 猾促.
	//				bool bSetupMatineeGroupRes = SetupSingleShowMatineeGroup(ThisSetting, ThisState);
	//				if (ThisSetting.IsDirectedMode() && !bSetupMatineeGroupRes) // InGame 葛靛搁 付萍聪 飘发捞 鞘夸绝绰 芭扼 SetupSingleShowMatineeGroup 捞 角菩且 荐 乐绰单, 弊犯促绊 咯扁辑 角菩贸府甫 且 扒 绝瘤.
	//				{
	//					return false;
	//				}
	//			}

	//			// Input 捞 啊瓷茄 惑怕俊辑 Player Character 绰 霸烙 惑怕甫 弊措肺 啊廉啊具 局聪皋捞记捞 唱坷电瘤 窍摆瘤 偿偿
	//			if (bDisableInput == true || (ThisPlayActor != GetB2LocalPlayerChar() && ThisPlayActor != GetB2MatchOpponent()))
	//			{
	//				if (ThisSetting.IsDirectedMode())
	//				{// StageEventShowState 甫 技泼窍绰单 捞扒 捞傈 ExportedCamera 尔 ManualCamera 俊辑 荤侩窍带 芭. AnimMontage 甫 敲饭捞 窍搁辑 傈捧甫 阜绰 开且档 沁栏唱 瘤陛篮 芭狼 state 钎矫 荐霖栏肺 力芭瞪 荐 乐澜.
	//					EStageEventShowState FinalSES =
	//						(ThisSetting.StageEventStateOverride == EStageEventShowState::ESES_None || OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee) ?
	//						GetDefaultDesiredPlayActorSESState(DirectingEvent, ThisSetting.PlayActorType) : ThisSetting.StageEventStateOverride;
	//					ThisPlayActor->SetStageEventShowState(FinalSES, this);
	//				}
	//				if (ThisSetting.PlayActorStateMode != EPlayActorStateMode::EPASM_MaintainInGame)
	//				{
	//					ThisPlayActor->PreventMovementByStageEvent(); // 见篮 客吝, 趣篮 楷免 档吝俊 spawn 登菌歹扼档 敲饭捞绢 卵酒坷瘤 臼霸. PreventMovement 窍搁 傍拜档 给窃. 俺曹

	//					ABladeIIPlayer* ThisPlayActorCastToPlayer = Cast<ABladeIIPlayer>(ThisPlayActor);

	//					if (ThisPlayActorCastToPlayer)
	//						ThisPlayActorCastToPlayer->StopRepeatingInput();

	//				}
	//			}

	//			ThisState.bShowStarted = true; // Mark started.
	//			ThisState.bShowFinished = false;

	//			return true;
	//		}
	//	}
	//}

	return false;
}

bool AB2StageEventDirector::SetupSingleShowMatineeGroup(FPlayActorShowSettings& InSettingForThis, FPlayActorShowRuntimeState& InStateForThis)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::SetupSingleShowMatineeGroup"));

	ABladeIICharacter* ThisPlayActor = InStateForThis.GetPlayActor(this);
	//if (MatineeActor && MatineeActor->MatineeData && ThisPlayActor)
	//{
	//	// 付萍聪 葛靛俊辑 AdjustToPlayActor 档 荤侩 啊瓷窍档废 惑措谅钎 扁瓷阑 粱 劝侩. 汲飞 付萍聪俊辑 茫瘤 给茄 飘发捞扼档 扁霖栏肺 伙阑 荐绰 乐档废 窃. 弊繁 版快绰 措眉肺 PlayActorStateMode 甫 InGame 栏肺 初阑 淀.
	//	if (InSettingForThis.IsOneOfAdjustToPlayActorPlacement())
	//	{
	//		MatineeActor->SetDynamicRelativeMoveTrackBaseActor(ThisPlayActor,
	//			FVector(0.0f, 0.0f, -1.0f * ZOffsetFromMatchingSKActorToBIICharacter(ThisPlayActor)), // 咯扁档 俊叼磐 轰器飘俊急 SkeletalMeshActor 肺 焊摆瘤父 牢霸烙俊急 BladeIICharacter 捞聪 坷橇悸 嘎苗淋.
	//			(InSettingForThis.PlayActorPlacement == EPlayActorPlacement::EPAP_AdjustToPlayActorLocationOnly)); // LocationOnly 可记 滴锅掳 牢磊肺..
	//	}

	//	// 汲沥 棺 敲饭捞 惑炔俊 蝶弗 弥辆利牢 InterpGroup 捞抚.
	//	const FName FinalMatchingGroupName(*GetFinalMatchingInterpGroupName(ThisPlayActor, InSettingForThis.MatchingInterpGroupName.ToString(), false));

	//	ASkeletalMeshActor* FoundGroupSKActor = Cast<ASkeletalMeshActor>(GetGroupActorOfGroupName(FinalMatchingGroupName));

	//	if (FoundGroupSKActor)
	//	{
	//		// 付萍聪俊 荤侩瞪 胶奶竣磐甸篮 单漠救冈霸 秦霖促.
	//		UPrimitiveComponent* pPrimitiveComponent = Cast<UPrimitiveComponent>(FoundGroupSKActor->GetRootComponent());
	//		pPrimitiveComponent->bReceivesDecals = false;

	//		InStateForThis.CtrlMatineePuppet = FoundGroupSKActor;

	//		// CtrlMatineePuppet 阑 PlayActor 客 付蛮啊瘤肺 盒厘
	//		// 厘脚备啊 眠啊登芭唱 秦辑 眠啊肺 寇包阑 嘎苗临 鞘夸啊 乐阑 锭俊绰 酒贰 SetupControlledMatineePuppet 率俊 累诀阑 且 巴.
	//		ThisPlayActor->SetupControlledMatineePuppet(InStateForThis.CtrlMatineePuppet, 
	//			// bHidePlayerPointLight 俊 蝶扼 puppet 率俊 付蛮啊瘤 PointLight 甫 悸诀窍霸 窃. 可记俊 蝶扼 肺拿 敲饭捞绢 某腐磐肺 茄沥登绰单 葛官老 dynamic point light 俺荐 力茄捞 乐扁 锭巩俊 林狼窍绰 巴.
	//			(!bHidePlayerPointLight && 					
	//				(InStateForThis.IsForLocalPlayerCharacter() || (bAllowOtherPlayerPointLight && Cast<ABladeIIPlayer>(InStateForThis.GetPlayActor(this))))
	//				)
	//		); 

	//		if (InSettingForThis.ActorScaleOverride > 0.0f)
	//		{ // SetupControlledMatineePuppet 俊辑 ThisPlayActor scale 阑 利侩沁阑 扒单 蝶肺 流立 瘤沥窍绊 酵阑 版快. 老窜 uniform scale 肺促啊.
	//			InStateForThis.CtrlMatineePuppet->GetSkeletalMeshComponent()->SetWorldScale3D(FVector(InSettingForThis.ActorScaleOverride, InSettingForThis.ActorScaleOverride, InSettingForThis.ActorScaleOverride));
	//		}

	//		// SetupControlledMatineePuppet 俊辑 阿 component 甸狼 visibility 惑怕甫 眉农且 荐 乐栏骨肺 SetVisibility 唱 SetActorHiddenInGame 殿篮 弊 捞饶俊 贸府.

	//		if (InSettingForThis.IsDirectedMode()) // InGameStateMode 搁 付萍聪 anim track 荤侩窍瘤 臼绰 巴. 辟单 弊繁 版快俊 付萍聪 track 阑 霖厚且 鞘夸啊 乐唱..?
	//		{
	//			// 扁夯利栏肺 见败初疽促绊 啊沥窍绊.. 泪瘪 难 躇促啊 辆丰 饶 促矫 见败初澜. 捞吧肺 殿废茄 霸 酒丛 Visibility 飘发栏肺 秦具 且 淀.
	//			SetVisibilityOfMatineePuppet(InStateForThis.CtrlMatineePuppet, InStateForThis.GetPlayActor(this), true);

	//			// 埃趣 惑窍 捞悼捞 缴茄 局聪皋捞记狼 版快 AlwaysTickPoseAndRefreshBones 啊 鞘夸茄 版快啊 乐促. 拱沸 楷免 辆丰 饶浚 俩 诀单捞飘 后档 撤篮 吧肺 汗备
	//			InStateForThis.CtrlMatineePuppet->SetActorTickEnabled(true); // 扁夯 平档 咯扁辑 劝己拳甫.
	//			InStateForThis.CtrlMatineePuppet->GetSkeletalMeshComponent()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	//			// ThePlayActor 甫 见败初阑 啊厘 扁夯利 厘家牢单, PCSync 殿狼 扒栏肺 牢秦 蝶肺 捞饶俊 贸府
	//			//TryHidePlayActorForMatineeInternal(ThisPlayActor, true);
	//			if (ThisPlayActor->bNeedSpawnCheck)
	//			{// NPCClassInfo 狼 bUsePreSpawnState 俊 蝶扼 技泼捞 登绰单, 楷免 厘搁阑 芭模促绰 芭 磊眉啊 PreSpawnState 甫 瘤唱绰 巴栏肺 埃林且 荐 乐绊, 坷洒妨 楷免 捞饶俊 spawn 捞 促矫 登绢滚府聪 巩力啊 凳. 蝶扼辑 false 肺 倒妨初澜.
	//				ThisPlayActor->bNeedSpawnCheck = false; 
	//				// 弊府绊 MeshComponentUpdateFlag 甫 技泼秦 林绢具 角力肺 惑怕啊 官诧促.. 楷免 场唱搁 档肺 登倒覆.
	//				ThisPlayActor->TemporarySetMeshComponentUpdateFlag(EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones);
	//			}
	//		}

	//		// 娟甸捞 牢傍瘤瓷 辨阑 阜匙..? 枚历窍霸 off 矫虐磊. 捞扒 盔贰 硅摹窜拌俊辑 何磐 秦具 登绰 巴栏肺 汗备窍瘤 臼绰促.
	//		InStateForThis.CtrlMatineePuppet->SetActorEnableCollision(false);
	//		InStateForThis.CtrlMatineePuppet->GetSkeletalMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//		return true;
	//	}
	//}

	return false;
}

void AB2StageEventDirector::SetVisibilityOfMatineePuppet(ASkeletalMeshActor* InPuppet, ABladeIICharacter* InRelevantPlayActor, bool bVisible)
{
	if (!IsValid(InPuppet))
	{
		return;
	}

	if (bVisible)
	{
		InPuppet->GetSkeletalMeshComponent()->SetVisibility(true, true); // 祈笼 格利栏肺 肯傈洒 见败初疽促啊 鳖冈阑 荐档 乐栏聪 せ
		InPuppet->SetActorHiddenInGame(false); // 捞霸 琴缴牢单 捞巴父 啊瘤绊 促 登瘤 臼阑 锭啊 乐绢辑 窃荐 蝶肺 哗 初篮 芭..

		ActivateMatineePuppetPSCs(InPuppet, InRelevantPlayActor); // 角篮 芭狼 夸芭 窍妨绊 蝶肺 话澜.
	}
	else
	{
		InPuppet->SetActorHiddenInGame(true);
	}
}

void AB2StageEventDirector::ActivateMatineePuppetPSCs(ASkeletalMeshActor* InPuppet, ABladeIICharacter* InRelevantPlayActor)
{
	// 购啊 Owner Actor 啊 见绢乐绰 悼救俊 Particle System Component 甸捞 Deactivate 啊 登绰 淀秦辑 蝶肺 Activate 矫虐扁 困秦 父甸绢初澜.
	// 傈开 蜡瓶肺 付蛮啊瘤 钙遏狼 扁瓷捞 EnsureActivateStaticallyAttachedPSCs 肺 力傍凳..
	if (InPuppet)
	{
		ABladeIIPlayer* CastedPlayActorPlayer = Cast<ABladeIIPlayer>(InRelevantPlayActor);
		TArray<UActorComponent*> AllPSC;
		InPuppet->GetComponents(AllPSC);
		for (UActorComponent* ThisComp : AllPSC)
		{
			UParticleSystemComponent* ThisPSComp = Cast<UParticleSystemComponent>(ThisComp);
			if (ThisPSComp)
			{
				if (bHideHud && CastedPlayActorPlayer && CastedPlayActorPlayer->FloorRingPS == ThisPSComp->Template/* && GetB2GameModeType(GetWorld()) != EB2GameMode::PVP_Tag*/)
				{ // HUD 见扁绰 版快扼搁 FloorRing 篮 咯窍埃 焊捞瘤 臼霸..
					// 瘤陛 抗惑栏肺 咯扁浚 甸绢客急 救等促. 
					// FloorRingPS 甫 EventPuppetSetupBlacklist 俊 眠啊秦辑 酒抗 Puppet 俊 嘿捞瘤 臼霸 窍看澜.
					// 捞 内靛绰 烤朝 烤利 东龙 内靛烙.
					checkSlow(0); 
					ThisPSComp->Deactivate();
					ThisPSComp->SetHiddenInGame(true);
				}
				else
				{
					EnsureActivatePSCInternal(ThisPSComp);
				}
			}
		}
	}
}

AActor* AB2StageEventDirector::GetGroupActorOfGroupName(const FName& InGroupName)
{
	if (!MatineeActor)
	{
		return nullptr;
	}

	ULevelSequence* LevelSequence = Cast<ULevelSequence>(MatineeActor->GetSequence());
	if (!LevelSequence)
	{
		return nullptr;
	}

	UMovieScene* MovieScene = LevelSequence->GetMovieScene();
	if (!MovieScene)
	{
		return nullptr;
	}

	ULevelSequencePlayer* SequencePlayer = MatineeActor->GetSequencePlayer();
	if (!SequencePlayer)
	{
		return nullptr;
	}

	IMovieScenePlayer& PlayerInterface = *SequencePlayer;

	const FString TargetName = InGroupName.ToString();

	// 遍历所有绑定 (旧 Matinee 的 InterpGroups -> 现在的 MovieScene Bindings)
	for (const FMovieSceneBinding& Binding : MovieScene->GetBindings())
	{
		if (Binding.GetName() == TargetName)
		{
			const FGuid& ObjectGuid = Binding.GetObjectGuid();

			// Root 序列（如果是嵌套子序列再处理 SequenceID，这里简单假设都是根）
			TArrayView<TWeakObjectPtr<UObject>> Found = PlayerInterface.FindBoundObjects(ObjectGuid, FMovieSceneSequenceID());

			for (const TWeakObjectPtr<UObject>& WeakObj : Found)
			{
				if (AActor* Actor = Cast<AActor>(WeakObj.Get()))
				{
					return Actor;
				}
			}

			// 找到了名字但没有 Actor（可能绑定的是 Component），直接返回 nullptr
			return nullptr;
		}
	}

	return nullptr;
}


void AB2StageEventDirector::SelectFinalDirectorTrack()
{
	//if (OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee || MatineeActor == NULL || MatineeActor->MatineeData == NULL)
	//{
	//	return;
	//}

	//UInterpGroupDirector* TheDirGroup = MatineeActor->MatineeData->FindDirectorGroup(); // Director Group 篮 漂喊窍聪 官肺 掘绢棵 荐 乐焙 せ
	//if (TheDirGroup == NULL)
	//{
	//	return;
	//}

	//if (TheDirGroup->InterpTracks.Num() <= 1 || (bPCDependentDirectorTrack == false && !IsValidRandomMatineeGroupNumSet()))
	//{
	//	// 捞 版快, 傲 贸澜 吧肺促啊 Enable 秦 滚副 荐档 乐摆瘤父, Director Track 阑 酒轿府 静瘤 臼绰 版快档 积阿秦 杭 荐 乐摆栏聪 弊成 府畔.
	//	return;
	//}

	//// 漂沥 ShowSetting 俊 官牢爹 等 PlayActor 啊 酒囱 泅犁 林楷拜栏肺 唱坷绰 Player Character 俊 蝶扼辑烙.
	//ABladeIIPlayer* PossibleHighlightedPlayer = GetShowHeroPlayerIfAny();
	//if (bPCDependentDirectorTrack && !PossibleHighlightedPlayer)
	//{ // 措眉肺 捞 event 啊 捞繁 己拜俊 嘎瘤 臼绰 版快老 巴. 拱沸 酒匆 荐档 乐绰单 弊繁 版快啊 积扁搁 GetShowHeroPlayerIfAny 甫 颊龙.
	//	return;
	//}
	//BII_CHECK(!PossibleHighlightedPlayer || PossibleHighlightedPlayer->IsValidObj());

	//FString DesiredPostFix;
	//if (bPCDependentDirectorTrack && PossibleHighlightedPlayer)
	//{ // FinalMatchingInterpGroupName 捞扼绰 狼固客绰 距埃 促福绊 付蛮啊瘤 痹距阑 利侩茄 postfix 父 啊廉坷妨绰 芭
	//	DesiredPostFix = GetFinalMatchingInterpGroupNamePC(TEXT(""), PossibleHighlightedPlayer->GetTrimmedNameFromPCClassEnum(), EPCInterpGroupNetType::PCIGNT_Local, false);
	//}
	//else if (IsValidRandomMatineeGroupNumSet())
	//{// FinalMatchingInterpGroupName 捞扼绰 狼固客绰 距埃 促福绊 付蛮啊瘤 痹距阑 利侩茄 postfix 父 啊廉坷妨绰 芭
	//	DesiredPostFix = GetFinalMatchingInterpGroupNameRandom(TEXT(""), RandomlySetMatineeGroupNum, false);
	//}
	//
	//// 泅犁 PlayerCharacter 俊 嘎绰 postfix 甫 啊柳 巴栏肺 急琶.
	//for (int32 DI = 0; DI < TheDirGroup->InterpTracks.Num(); ++DI)
	//{
	//	UInterpTrackDirector* CurrDirTrack = Cast<UInterpTrackDirector>(TheDirGroup->InterpTracks[DI]);
	//	if (CurrDirTrack == NULL)
	//	{
	//		continue;
	//	}

	//	if (CurrDirTrack->TrackTitle.EndsWith(DesiredPostFix)) // Postfix 父 嘎眠搁 凳. 傍烹 prefix 鞍篮 芭 绝澜.
	//	{
	//		CurrDirTrack->EnableTrack(true);
	//	}
	//	else
	//	{
	//		CurrDirTrack->EnableTrack(false); // 咯鸥 唱赣瘤绰 馋促. 绢瞒乔 DirectorTrack 篮 窍唱观俊 给 靖.
	//	}
	//}

	//// 趣矫扼档 困俊辑 Enable 等 巴捞 绝菌促搁 傲 霉锅掳 巴栏肺.. 且 荐档 乐摆瘤父 弊成 滴磊.
}

void AB2StageEventDirector::RuntimeRemoveNonDesiredGroup()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::RuntimeRemoveNonDesiredGroup"));

	//TArray<FString> AllTrimmedPCClassEnumName;
	//ABladeIIPlayer::GetAllTrimmedNameFromPCClassEnum(AllTrimmedPCClassEnumName);

	//for (int32 GI = 0; GI < MatineeActor->MatineeData->InterpGroups.Num(); ++GI)
	//{
	//	UInterpGroup* CurrGroup = MatineeActor->MatineeData->InterpGroups[GI];

	//	bool bFoundMatchingOne = false;
	//	for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
	//	{
	//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
	//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];

	//		// 泅犁 ShowSetting 苞 楷搬等 Matinee InterpGroup 趣篮 Folder 捞抚
	//		const FName FinalMatchingGroupName(*GetFinalMatchingInterpGroupName(CurrShowState.GetPlayActor(this), CurrShowSetting.MatchingInterpGroupName.ToString(), false));
	//		const FName FinalMatchingGroupNameForFolder(*GetFinalMatchingInterpGroupName(CurrShowState.GetPlayActor(this), CurrShowSetting.MatchingInterpGroupName.ToString(), true));

	//		if (CurrGroup->GroupName == FinalMatchingGroupName || 
	//			CurrGroup->GroupName == FinalMatchingGroupNameForFolder)
	//		{
	//			bFoundMatchingOne = true; // 官肺 嘎绰 霸 乐栏骨肺 捞扒 烹苞窃.
	//			break;
	//		}
	//	}

	//	if (bFoundMatchingOne){
	//		continue;
	//	}

	//	bool bNotDesiredGroup = false;

	//	// 咯扁肺 吭栏搁 ShowSetting 阑 烹秦 弊缝 捞抚阑 持瘤 臼疽芭唱, 泅犁 霸烙 惑炔俊 嘎瘤 臼芭唱 茄 版快 殿. 
	//	// 沥秦柳 匙捞怪 痹距俊 狼秦 犬角洒 唱坷瘤 富酒具 窍绰 版快牢瘤 茄锅 歹 眉农秦辑 厚劝己拳 窜拌肺 (RuntimeDisableInterpGroup)
	//	// 弊缝 捞抚捞 沥秦柳 匙捞怪 痹蘑苞 酒抗 包访捞 绝促搁 咯扁辑 厚劝己拳 且 措惑捞 酒聪促.

	//	for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
	//	{
	//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
	//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];

	//		// 咯扁辑 ThePlayActor 甫 八荤窍绰 扒 窜瘤 捞 技泼捞 BladeIIPlayer 侩牢瘤 焊扁 困窃. 角力肺 八荤窍绰 ThePlayActor 客 楷包等 弊缝捞扼搁 力芭秦急 救凳.
	//		if (Cast<ABladeIIPlayer>(CurrShowState.GetPlayActor(this))) 
	//		{
	//			for (int32 PCI = 0; PCI < AllTrimmedPCClassEnumName.Num(); ++PCI)
	//			{
	//				for (int32 NTI = 0; NTI < (int32)EPCInterpGroupNetType::PCIGNT_End; ++NTI)
	//				{
	//					// 捞 ShowSetting 苞 PCClass 俊 蝶扼 痹距俊 甸绢啊绰 Group 趣篮 弃歹 捞抚
	//					// Iteration 档吝俊 角力肺 荤侩瞪 捞抚档 唱坷摆瘤父.. 芭扁俊 概莫登绰 InterpGroup 篮 捞固 咯扁肺 坷瘤 臼绊 咯扁肺 柯 巴甸篮 厚劝己拳 饶焊.
	//					const FName PossiblePCGroupName(*GetFinalMatchingInterpGroupNamePC(CurrShowSetting.MatchingInterpGroupName.ToString(), AllTrimmedPCClassEnumName[PCI], static_cast<EPCInterpGroupNetType>(NTI), false));
	//					const FName PossiblePCGroupNameForFolder(*GetFinalMatchingInterpGroupNamePC(CurrShowSetting.MatchingInterpGroupName.ToString(), AllTrimmedPCClassEnumName[PCI], static_cast<EPCInterpGroupNetType>(NTI), true));

	//					// 泅犁 ShowSetting 俊辑狼 MatchingInterpGroupName 阑 海捞胶肺 窍绊 乐瘤父 泅犁 霸烙俊辑 急琶登绢 乐瘤绰 臼促.
	//					if (CurrGroup->GroupName == PossiblePCGroupName ||
	//						CurrGroup->GroupName == PossiblePCGroupNameForFolder)
	//					{
	//						bNotDesiredGroup = true;
	//						break;
	//					}
	//				}
	//			}
	//		}
	//		else if (IsValidRandomMatineeGroupNumSet())
	//		{ // 罚待 弊缝 急琶捞 乐绰 版快, 急琶登瘤 给茄 罚待 弊缝阑 力芭. 捞扒 Player 绰 酒聪绊 各 楷免.
	//			for (int32 RNI = 1; RNI <= RandomMatineeGroupNum; ++RNI)
	//			{ 
	//				if (RNI == RandomlySetMatineeGroupNum){
	//					continue; // 捞锅 罚待 急琶等 芭烙. 绢瞒乔 酒贰俊辑 bNotDesiredGroup 急琶捞 登瘤 臼摆瘤父 固府 吧矾晨.
	//				}
	//				// 阿 罚待 喊肺 弃歹 备己沁促 摹绊 弊巴父 贸府窍磊. 拱沸 捞巴苞 喊俺肺 窍困 弊缝甸档 匙捞怪阑 嘎苗林变 秦具 窃.
	//				const FName PossibleRandFolderName(*GetFinalMatchingInterpGroupNameRandom(CurrShowSetting.MatchingInterpGroupName.ToString(), RNI, true));
	//				if (CurrGroup->GroupName == PossibleRandFolderName)
	//				{
	//					bNotDesiredGroup = true;
	//					break;
	//				}
	//			}
	//		}

	//		if (bNotDesiredGroup)
	//		{
	//			break;
	//		}
	//	}

	//	// 扁鸥 促弗 版快 眠啊登搁..

	//	if (bNotDesiredGroup)
	//	{// 秦寸 弊缝阑 厚劝己拳
	//		RuntimeDisableInterpGroup(CurrGroup, MatineeActor);
	//	}
	//}
}

void AB2StageEventDirector::RuntimeDisableInterpGroup(class UInterpGroup* GroupToDisable, class ALevelSequenceActor * OwnerMatinee)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::RuntimeRemoveGroupActorFromInterpGroup"));

//	if (GroupToDisable && OwnerMatinee)
//	{
//		TArray<UInterpGroupInst*> arMatchingGI;
//		// 弊缝苞 老摹窍绰 弊缝 牢胶畔胶甫 茫绰促.
//		for (int32 GII = 0; GII < OwnerMatinee->GroupInst.Num(); ++GII)
//		{
//			// Disable 窍绊磊 窍绰 巴捞 磊扁 磊脚捞电瘤 酒聪搁 磊脚阑 器窃窍绰 弃歹 弊缝捞电瘤
//			UInterpGroup* ParentFolder = FindParentGroupFolder(OwnerMatinee->GroupInst[GII]->Group, OwnerMatinee);
//
//			if ( (ParentFolder && ParentFolder == GroupToDisable)
//				 || (OwnerMatinee->GroupInst[GII]->Group == GroupToDisable) )
//			{
//				arMatchingGI.Add(OwnerMatinee->GroupInst[GII]);
//			}
//		}
//
//		for (int32 GII = 0; GII < arMatchingGI.Num(); ++GII)
//		{
//			UInterpGroupInst* ThisGI = arMatchingGI[GII];
//			if (ThisGI) 
//			{
//				// 捞芭 粱 扯厚捞变 茄单.. 俊叼磐 甘 历厘 矫 ShowSetting 俊 殿废登瘤 臼篮 局甸档 visibility 甫 波林妨促 焊聪 Matinee InitInterp 矫 SkeletalMeshActor 甸阑 难林绰 内靛啊 眠啊登菌绊.. 辟单 咯扁辑 镜葛绝促绊 埃林登绰 局甸篮 促矫 波林绢具 せ
//				ASkeletalMeshActor* ThisGroupSKActor = Cast<ASkeletalMeshActor>(ThisGI->GetGroupActor());
//				if (ThisGroupSKActor)
//				{
//					ThisGroupSKActor->SetActorHiddenInGame(true);
//				}
//				// PC 喊肺 扼捞飘 飘发甸 父甸绢 初篮 版快档 喊档 贸府啊 鞘夸. SetAllPointLightGroupAffectWorld 肺绰 贸府 给窃.
//				ALight* ThisGroupLightActor = Cast<ALight>(ThisGI->GetGroupActor());
//				UPointLightComponent* PLComp = ThisGroupLightActor ? Cast<UPointLightComponent>(ThisGroupLightActor->GetLightComponent()) : nullptr;
//				if (PLComp
//#if 0
//					// 狼档窍瘤 臼篮 肋给等 扁瓷 窍俊辑 悸诀 登菌带 扼捞泼阑 皋操扁 困秦 持菌带 烙矫 敲贰弊..
//					// 悸诀 荐沥捞 肯丰登搁 bHackMyPerPCClassLightingSetupDone 阑 眉农窍绰 侥捞菌澜.
//					// 悸诀 肯丰登菌促绊 窍咯 DEPRECATED 付农窍绊 扁瓷篮 力芭.. 趣 葛福聪 肯傈洒 力芭窍瘤绰 臼澜.
//					&& bHackMyPerPCClassLightingSetupDone_DEPRECATED 
//#endif				
//					)
//				{
//					PLComp->bAffectsWorld = false;
//					PLComp->MarkRenderStateDirty();
//				}
//
//				// 抗傈俊 GroupActor 甫 力芭窍绊 沁菌绰单 傲 窍困 飘发 Disable 肺 登瘤 臼唱 酵促. 老窜 康旷狼 啪 肺爹绝绰 犁矫累阑 窍妨搁 GroupActor 绰 力芭秦急 救凳.
//				for (UInterpTrack* CurrTrack : ThisGI->Group->InterpTracks)
//				{
//					if (CurrTrack)
//					{
//						CurrTrack->EnableTrack(false);
//					}
//				}
//			}
//		}
//
//		// 购啊 Folder 甸绢啊搁辑何磐牢瘤.. 困 内靛啊 救冈鳃 锭啊 乐促. 促矫 捞吧肺档..
//		for (UInterpTrack* CurrTrack : GroupToDisable->InterpTracks)
//		{
//			if (CurrTrack)
//			{
//				CurrTrack->EnableTrack(false);
//			}
//		}
//	}
}

//UInterpGroup* AB2StageEventDirector::FindParentGroupFolder(UInterpGroup* ChildGroup, class ALevelSequenceActor * OwnerMatinee) const
//{ // 角 备泅何甫 MatineeActor 率栏肺 颗败初澜.
//	if (OwnerMatinee)
//	{
//		return OwnerMatinee->FindParentGroupFolder(ChildGroup);
//	}
//	return NULL;
//}

FString AB2StageEventDirector::GetFinalMatchingInterpGroupName(class ABladeIICharacter* InPlayActor, FString RawInterpGroupName, bool bForFolderGroup)
{
	ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(InPlayActor);
	if (CastedPlayer) // 扁鸥 NPC 甸狼 版快 InterpGroup 捞抚阑 函版秦辑 八祸且 版快啊 惯积且鳖?
	{
		EPCInterpGroupNetType NetType = EPCInterpGroupNetType::PCIGNT_Local;

		AB2NetGameMode* NetGM = Cast<AB2NetGameMode>(GetB2GameMode());
		if (NetGM && DirectingEvent == EStageEvent::ESEV_Begin) // 泅犁肺急 NetGameMode 狼 荤捞靛啊 巩力啊 登绰 版快绰 矫累挥.
		{
			ABladeIINetPlayer* CastedNetPlayer = Cast<ABladeIINetPlayer>(CastedPlayer);

			NetType = (
				((NetGM->GetPeerType() != PEER_RESTRICTED && CastedNetPlayer->GetNetStatus() == ABladeIINetPlayer::NET_MASTER) || (NetGM->GetPeerType() == PEER_RESTRICTED && CastedNetPlayer->GetNetStatus() == ABladeIINetPlayer::NET_SLAVE)) ?
				EPCInterpGroupNetType::PCIGNT_SuperPeer : EPCInterpGroupNetType::PCIGNT_CommonPeer
				);
		}

		// PC 绰 努贰胶 急琶, Net 霸烙 peer 鸥涝 殿俊 蝶扼 崔扼咙.
		return GetFinalMatchingInterpGroupNamePC(RawInterpGroupName, CastedPlayer->GetTrimmedNameFromPCClassEnum(), NetType, bForFolderGroup);
	}
	else if (IsValidRandomMatineeGroupNumSet())
	{// 泅犁 flow 肺绰 Player 侩 飘发篮 Random selection 捞 救 瞪 巴.
		// Player 弊缝篮 捞固 喊档狼 suffix 啊 嘿绰单促啊 角力 夸备荤亲档 各捞 唱坷绰 楷免阑 罚待窍霸 窍磊绰 巴烙.
		return GetFinalMatchingInterpGroupNameRandom(RawInterpGroupName, RandomlySetMatineeGroupNum, bForFolderGroup);
	}

	return RawInterpGroupName;
}

FString AB2StageEventDirector::GetFinalMatchingInterpGroupNamePC(FString RawInterpGroupName, FString TrimmedPCClassEnumName, EPCInterpGroupNetType NetType, bool bForFolderGroup)
{
	// 逢俊 嘎苗 弊缝 捞抚阑 持档废 秦具.. 抗甫 甸绢 "MainPCGroup_Gladiator", "SubPCGroup_Assassin" 窍绰 侥捞 登摆促.
	FString RetName = RawInterpGroupName + TEXT("_") + TrimmedPCClassEnumName;

	// 捞 逢篮 老窜 PVP俊辑父 静绰吧肺 茄促. (眠饶俊 捞扒 可记栏肺 哗档废 窍磊)
	if (AB2PVPGameMode* PVPGM = Cast<AB2PVPGameMode>(GetB2GameMode()))
	{
		if (NetType == EPCInterpGroupNetType::PCIGNT_SuperPeer){
			RetName += FString(TEXT("_")) + FString(INTERPGROUP_SUFFIX_SUPERPEER);
		}
		else if (NetType == EPCInterpGroupNetType::PCIGNT_CommonPeer){
			RetName += FString(TEXT("_")) + FString(INTERPGROUP_SUFFIX_COMMONPEER);
		}
	}
	
	if (bForFolderGroup)
	{
		RetName = FOLDERGROUP_PREFIX + RetName;
	}
	return RetName;
}

FString AB2StageEventDirector::GetFinalMatchingInterpGroupNameRandom(FString RawInterpGroupName, int32 RandNum, bool bForFolderGroup)
{ // 叼泛磐弊缝档 捞吧 镜 巴烙.
	FString RandPostFix = FString::Printf(INTERPGROUP_SUFFIX_RAND, RandNum);
	// Folder 捞抚牢 版快 弊成 沥秦柳 prefix 客 postfix 父 嘿咯辑 荤侩.
	return bForFolderGroup ? (FOLDERGROUP_PREFIX + RandPostFix) : (RawInterpGroupName + TEXT("_") + RandPostFix);
}

#if WITH_EDITOR
bool AB2StageEventDirector::IsPossibleInterpGroupNameForPC(const FString& InCheckName) const
{ // GetFinalMatchingInterpGroupNamePC 肺 唱棵 荐 乐绰 捞抚牢瘤 眉农.. true 府畔捞扼绊 秦档 酒匆 荐绰 乐瘤父 false 府畔捞搁 犬角洒 酒囱 芭.

	TArray<FString> AllPCClassTrimmedName;
	ABladeIIPlayer::GetAllTrimmedNameFromPCClassEnum(AllPCClassTrimmedName);

	bool bHasAtLeastOnePCName = false;
	for (const FString& ThisPCName : AllPCClassTrimmedName)
	{
		if (InCheckName.EndsWith(ThisPCName)) // GetFinalMatchingInterpGroupNamePC 备泅俊 蝶扼.. PCClass 捞抚捞 场俊 嘿澜.
		{
			bHasAtLeastOnePCName = true;
			break;
		}
	}

	if (!bHasAtLeastOnePCName)
	{
		return false;
	}

	// ShowSettings 狼 MatchingInterpGroupName 鳖瘤 焊绊绰 酵瘤父..

	return true;
}
#endif

void AB2StageEventDirector::SetAllPointLightGroupAffectWorld(bool bAffectWorld)
{
	//if (!MatineeActor)
	//	return;

	//const int32 AllowedMatineeTrackPointLightNum = GetAllowedMatineeTrackPointLightNum();

	//// 葛官老 坊歹矾 俺荐 力茄 绊妨秦具 窍绰单.. 咯扁辑 窍瘤 给窃.
	//// 恐衬窍搁 Group 喊肺 急琶利栏肺 荤侩秦具 窍绰 版快啊 乐扁 锭巩俊
	//// 咯扁辑绰 老褒利栏肺 On/Off 窍绊 俺喊 弊缝喊 鞘夸绝绰 芭 掺绰 扒 RuntimeDisableInterpGroup 俊辑
	//int32 UsedLightCount = 0;
	//for (int32 GII = 0; GII < MatineeActor->GroupInst.Num(); ++GII)
	//{
	//	ALight* pLight = Cast<ALight>(MatineeActor->GroupInst[GII]->GroupActor);
	//	ULightComponent* TheLightComp = pLight ? pLight->GetLightComponent() : nullptr;
	//	if (TheLightComp)
	//	{
	//		const bool bPrevAffectsWorld = TheLightComp->bAffectsWorld;
	//		if (UsedLightCount < AllowedMatineeTrackPointLightNum)
	//		{
	//			TheLightComp->bAffectsWorld = bAffectWorld;
	//		}
	//		else
	//		{ // 力茄 逞绢啊搁 倾侩 救.. 窍绊 酵菌绰单 弊缝喊肺 瘤沥秦 初篮 霸 乐绢辑 阂啊窃. ぱぱ
	//			TheLightComp->bAffectsWorld = bAffectWorld;
	//		}

	//		if (bPrevAffectsWorld != TheLightComp->bAffectsWorld)
	//		{
	//			TheLightComp->MarkRenderStateDirty();
	//		}

	//		++UsedLightCount;
	//	}
	//}
}

void AB2StageEventDirector::HackSpareLeastPointLight()
{
	// 葛官老 坊歹傅狼 dynamic point light 俺荐 力茄阑 嘎眠扁 困茄 利例茄 Light On/Offf 牧飘费阑 固胶窍带 矫例俊
	// 肋给 汲沥登绢 滚赴 老何 楷免 悸诀阑 皋操扁 困窃..
	// RuntimeDisableInterpGroup 俊辑 厚劝己拳 矫难具 且 飘发狼 point light 甫 掺霸 登绰单 捞吧 绊妨窍瘤 给茄 悸诀栏肺 牢秦 point light 啊 窍唱档 唱坷瘤 臼绰 惑炔捞扼搁 窍唱绰 混府档废..
	
//	if (bHidePlayerPointLight && MatineeActor) // bHidePlayerPointLight 啊 酒聪扼搁 敲饭捞绢 某腐磐 扼捞飘甫 傈崔秦 林菌阑 抛聪 窍唱绰 乐阑 巴.
//	{
//		bool bAtLeastOneTurnedOn = false;
//		UPointLightComponent* TheFirstEncounteredPLComp = nullptr;
//
//		for (int32 GroupIdx = 0; GroupIdx < MatineeActor->GroupActorInfos.Num(); GroupIdx++)
//		{
//			FInterpGroupActorInfo& Info = MatineeActor->GroupActorInfos[GroupIdx];
//			for (int32 ActorIdx = 0; ActorIdx < Info.Actors.Num(); ActorIdx++)
//			{
//				APointLight* PLActor = Cast<APointLight>(Info.Actors[ActorIdx]);
//				UPointLightComponent* PLComp = PLActor ? Cast<UPointLightComponent>(PLActor->GetLightComponent()) : nullptr;
//				if (PLComp && PLComp->Mobility == EComponentMobility::Movable)
//				{
//					if (PLComp->bAffectsWorld) {
//						bAtLeastOneTurnedOn = true; // 窍唱绰 混酒 乐栏聪 歹 杭 鞘夸 绝澜.
//						break;
//					}
//					if (!TheFirstEncounteredPLComp) { // 酒公档 救 混酒 乐绰 版快甫 困秦 贸澜铂 某教秦 敌促.
//						TheFirstEncounteredPLComp = PLComp;
//					}
//				}
//			}
//
//			if (bAtLeastOneTurnedOn) {
//				break;
//			}
//		}
//
//		if (!bAtLeastOneTurnedOn && TheFirstEncounteredPLComp)
//		{
//#if !UE_BUILD_SHIPPING // 购啊 窍唱 难变 窍瘤父 版绊 措惑.
//			UE_LOG(LogBladeII, Warning, TEXT("Unconditionally turning on the first point light.. just because of possible false event scene light setup.. of %s"), *GetName());
//#endif
//			TheFirstEncounteredPLComp->bAffectsWorld = true;
//			TheFirstEncounteredPLComp->MarkRenderStateDirty();
//		}
//	}
}


void AB2StageEventDirector::GetAllSupposedInterpGroupNamesForSetting(TArray<FName>& OutGroupNames, FPlayActorShowSettings& InSetting)
{
	if (IsPlayActorTypeForPlayerCharacter(InSetting.PlayActorType))
	{
		TArray<FString> AllTrimmedPCClassEnumName;
		ABladeIIPlayer::GetAllTrimmedNameFromPCClassEnum(AllTrimmedPCClassEnumName);

		for (int32 PCI = 0; PCI < AllTrimmedPCClassEnumName.Num(); ++PCI)
		{
			for (int32 NTI = 0; NTI < (int32)EPCInterpGroupNetType::PCIGNT_End; ++NTI)
			{
				OutGroupNames.AddUnique(
					FName(*GetFinalMatchingInterpGroupNamePC(InSetting.MatchingInterpGroupName.ToString(), AllTrimmedPCClassEnumName[PCI], static_cast<EPCInterpGroupNetType>(NTI), false))
				);
			}
		}
	}
	else
	{
		OutGroupNames.Add(InSetting.MatchingInterpGroupName); // PC 啊 酒聪扼搁 MatchingInterpGroupName 栏肺 官肺.

		if (RandomMatineeGroupNum > 1) // 罚待 弊缝 捞抚篮 眠啊肺 持绰 吧肺. 酒付档 捞霸 乐促搁 盔贰 捞抚篮 救 镜 芭 鞍瘤父.
		{
			for (int32 RI = 1; RI <= RandomMatineeGroupNum; ++RI)
			{
				OutGroupNames.AddUnique(
					FName(*GetFinalMatchingInterpGroupNameRandom(InSetting.MatchingInterpGroupName.ToString(), RI, false))
				);
			}
		}
	}
}

void AB2StageEventDirector::FinalBeginMatineePlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::FinalBeginMatineePlay"));

	//if (MatineeActor && MatineeActor->MatineeData)
	//{		
	//	EnsureHidePlayActorsForControlledMatinee();
	//	
	//	MatineeActor->Play();
	//	MatineeActor->AddCustomNotifyActor(this); // Stop notification 阑 罐扁 困秦 殿废. Play 捞傈俊 持栏搁 TermInterp 俊辑 狐咙.
	//	if (!GIsEditor) 
	//	{ // 夸 SetCloseBattleMainUIClass 捞亥飘甫 俊叼磐俊辑 Init-Destroy 矫俊 Subscribe/Unsubcribe 窍绰单 俊叼磐俊辑 Unsubscribe 啊 狐瘤扁 浆框. ばば
	//		// 弊府绊 酒付 俊叼磐俊急 鞘夸窍瘤档 臼阑 淀.
	//		SetCloseBattleMainUIClass<bool>::GetInstance().Signal(false);
	//	}
	//	EnterCinematicModeForControlledMatinee(); // CinematicMode 柳涝秦具 角龙利栏肺 敲饭捞绢 某腐磐 visibility 啊 技泼瞪 巴.
	//}

	//// SetVisibilityOfMatineePuppet 阑 烹秦辑 龋免窃俊档 阂备窍绊 咯扁辑档 肚 鞘夸茄淀.. 弊府绊 捞吧肺档 救冈鳃 锭啊 乐澜. 购啊 固功茄 鸥捞怪 巩力啊 乐绰 淀 茄单..
	//for (int32 EI = 0; EI < ShowStates.Num(); ++EI)
	//{
	//	ActivateMatineePuppetPSCs(ShowStates[EI].CtrlMatineePuppet, ShowStates[EI].GetPlayActor(this));
	//}

	//// LocalPlayerSwitching 阑 咯扁辑 茄促. PCSyncToMatineeStart 啊 乐歹扼档 捞 矫痢俊绰 付萍聪 敲饭捞啊 柳青瞪 巴捞骨肺. (泅犁 春 譬配府倔俊辑 荤侩)
	//// ActivateMatineePuppetPSCs 捞傈俊 给窍绰 扒 酒贰 Switching 阑 芭摹搁辑 PlayActor 啊 朝酒啊绰 版快啊 乐绢辑 Hide 悼累俊 老何 巩力啊 惯积窍骨肺..
	//if (bLocalPlayerSwitching) {
	//	SwitchingLastRenderingPlayer();
	//}
}

void AB2StageEventDirector::SwitchingLastRenderingPlayer()
{
	///* ----------------------------------------------------
	// !! 林狼 !!
	// 
	// 楷免 扁瓷俊 霸烙 flow 啊 辆加登绰 荤肥扼 老馆 葛氰 胶抛捞瘤狼 版快 楷免 胶诺 咯何俊 蝶扼 牢霸烙 flow 啊 官差绰 滚弊 惯积 家瘤啊 乐澜.
	// 捞固 弊繁 侥狼 荤侩捞 绝瘤档 臼绊 捞 扁瓷阑 荤侩窍绰 楷免 己拜 惑 霸烙 flow 啊 楷免俊 辆加登绰 霸 荤角捞扁档 茄单..
	//
	// 寸厘篮 譬配府倔 寇狼 葛靛俊辑 荤侩阑 窍瘤 臼档废 窍绊,
	// 老馆 葛氰 胶抛捞瘤俊辑 荤侩 鞘夸己捞 积扁搁 荤侩 炼扒阑 ManageMode 唱 SupportedStages 咯何 殿俊 蝶扼 茄沥窿绊
	// 漂洒 EStageEventSkipType::SEST_LeastSkip 捞 荤侩登绢具 茄促搁 LeastSkip 矫俊 捞 悼累阑 狐哆府瘤 臼档废 且 鞘夸啊 乐澜.
	//---------------------------------------------------- */
	//bool bisInTutorialGameMode = (Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this)) != nullptr);
	//checkSlow(bisInTutorialGameMode);
	//if (!bisInTutorialGameMode) {
	//	return;
	//}
	//////////////////////////////////////////////////////////

	//TArray<FPlayActorShowRuntimeState*> AllStatesOfPlayerChar;
	//GetShowStatesOfPlayerChar(AllStatesOfPlayerChar); // 敲饭捞绢 某腐磐甸狼 ShowState 父 急喊秦辑 啊廉咳.

	//checkSlow(ShowStates.Num() >= 2);
	//if (ShowStates.Num() < 2) {
	//	return; // 笛 捞惑捞绢具 官操电瘤 窍瘤..
	//}

	//// AllStatesOfPlayerChar 救俊辑狼 牢郸胶
	//int32 CurrentLocalPlayerIndex = INDEX_NONE; // 背眉 傈 State 牢郸胶
	//int32 NewLocalPlayerIndex = INDEX_NONE; // 背眉 饶 State 牢郸胶

	//for (int32 SI = 0; SI < AllStatesOfPlayerChar.Num(); ++SI)
	//{
	//	FPlayActorShowRuntimeState* ThisState = AllStatesOfPlayerChar[SI];
	//	// IsForLocalPlayerCharacter 俊 秦寸窍绰 扒 窜 窍唱父 乐绢具 茄促
	//	if (ThisState->IsForLocalPlayerCharacter())
	//	{
	//		CurrentLocalPlayerIndex = SI;
	//		break;
	//	}
	//}
	//// 背眉且 敲饭捞绢 某腐磐绰 第何磐 秦辑 泅犁 Local 捞 酒囱 吧肺. 父老 3 捞惑狼 飘发捞 乐促搁 第率 牢郸胶啊 快急 鉴困.
	//// 裹侩利牢 沥侥 荤侩栏肺 备泅窍绊磊 茄促搁 ShowSetting 俊 switching 咯何甫 钎矫窍带瘤 秦具摆瘤父 弊凡 鞘夸鳖瘤 乐绰 扁瓷牢 巴 鞍瘤绰 臼促.
	//for (int32 SI = AllStatesOfPlayerChar.Num() - 1; SI >= 0; --SI)
	//{
	//	FPlayActorShowRuntimeState* ThisState = AllStatesOfPlayerChar[SI];
	//	if(!ThisState->IsForLocalPlayerCharacter())
	//	{
	//		NewLocalPlayerIndex = SI;
	//		break;
	//	}
	//}
	//checkSlow(CurrentLocalPlayerIndex != INDEX_NONE && NewLocalPlayerIndex != INDEX_NONE);
	//if (CurrentLocalPlayerIndex == INDEX_NONE || NewLocalPlayerIndex == INDEX_NONE) {
	//	return; // 捞繁 版快 开矫唱 歹 捞惑 柳青 阂啊
	//}

	//FPlayActorShowRuntimeState* ShowStateToSwitch = AllStatesOfPlayerChar[NewLocalPlayerIndex];
	//FPlayActorShowRuntimeState* ShowStateOfOldLocalPlayer = AllStatesOfPlayerChar[CurrentLocalPlayerIndex];
	//// PlayActorAboutTobeSwitched 绰 措眉肺 Dummy 肺 spawn 茄 咀磐啊 瞪 巴捞促.
	//ABladeIIPlayer* PlayActorAboutTobeSwitched = Cast<ABladeIIPlayer>(ShowStateToSwitch->GetPlayActor(this));
	//// 梆 硼厘且 泅犁 肺拿 敲饭捞绢 某腐..
	//ABladeIIPlayer* CurrentLocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//UB2SkillRTManager* SkillRTManager = CurrentLocalB2Player ? CurrentLocalB2Player->GetSkillRTManager() : nullptr;
	//if (PlayActorAboutTobeSwitched && CurrentLocalB2Player && SkillRTManager)
	//{
	//	// 歹固扼档 某腐磐 Class 绰 技泼登绢 乐绰 芭..
	//	const EPCClass NewPCClassEnum = PlayActorAboutTobeSwitched->GetCurrentPlayerClass();
	//	const EPCClass OldPCClassEnum = CurrentLocalB2Player->GetCurrentPlayerClass();
	//	if (SkillRTManager->LocalPlayerSwitching(NewPCClassEnum))
	//	{
	//		// 某腐磐 官操搁辑 TutorialGameMode俊 MarkBeginAsyncLoadNextPCClass俊 捞亥飘 朝覆
	//		MarkBeginAsyncLoadClass<>::GetInstance().Signal();
	//		// 货肺 spawn 己傍.. 捞力 夯拜 ShowState 官层摹扁.
	//		ABladeIIPlayer* NewLocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//		ShowStateToSwitch->SetPlayActor(NewLocalB2Player);
	//		// 促弗 visibility 技泼捞 促 场抄 捞饶俊 秦具 秦辑.. 绢驴 荐 绝捞 规陛 spawn 茄 某腐磐 Hidden 贸府.
	//		NewLocalB2Player->SetHideFromEventDirector(true);
	//		
	//		// ForgetACharacter 狼 版快甫 曼绊. 角力肺 捞傈鳖瘤 楷搬登绢 乐带 PlayActor 绰 力芭登绰 惑炔捞扼 null-out 窍绰 霸 利例窃.
	//		ShowStateOfOldLocalPlayer->SetPlayActor(nullptr);			
	//	}
	//}
}

void AB2StageEventDirector::GetShowStatesOfPlayerChar(TArray<FPlayActorShowRuntimeState*>& OutFoundStates)
{
	for (FPlayActorShowRuntimeState& ThisState : ShowStates)
	{
		if (Cast<ABladeIIPlayer>(ThisState.GetPlayActor(this)))
		{
			OutFoundStates.Add(&ThisState);
		}
	}
}

void AB2StageEventDirector::EnsureHidePlayActorsForControlledMatinee()
{
	// SetupSingleShowMatineeGroup 殿 促弗 镑俊辑档 技泼窍瘤父 辆辆 部咯辑 付瘤阜俊 促矫 秦 林绢具 且 锭啊 乐澜..

	// Controlled Matinee 葛靛狼 PlayActor 甫 见扁瘤 臼酒具 且 版快啊 乐栏妨唱
	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	{
		for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
		{
			FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
			FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];
	
			ABladeIICharacter* ThisPlayActor = CurrShowState.GetPlayActor(this);
			if (ThisPlayActor == NULL){
				continue;
			}

			ThisPlayActor->SetHideFromEventDirector(CurrShowSetting.IsDirectedMode());

			ThisPlayActor->PreventMovementByStageEvent();
		}
	}
}

void AB2StageEventDirector::EnterCinematicModeForControlledMatinee()
{
	//// 咯变 咯矾锅阂妨档 茄锅父 鸥霸茄促.
	//if (bEnterCinematicMode)
	//	return;

	//bEnterCinematicMode = true;

	//ABladeIIPlayerController* LocalB2PC = GetB2LocalPlayerCon();
	//if (LocalB2PC && OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//{
	//	bool bHiddenPlayer = LocalB2PC->GetPawn() && LocalB2PC->GetPawn()->bHidden;
	//	
	//	// MatineeActor 率俊绰 bNoCinematicModeControl 阑 技泼秦 初绊 CinematicMode 牧飘费阑 流立 茄促.
	//	LocalB2PC->SetCinematicMode(true, bHiddenPlayer, bHideHud, bDisableInput, true);
	//}
	//if(!bForceNoHidePlayer)
	//	SetAllPlayerHiddenInGame(true);
}

// bTryHidePlayer肺 见辨瘤 富瘤 搬沥
void AB2StageEventDirector::SetAllPlayerHiddenInGame(bool bForceHide)
{
	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	ABladeIIPlayer *playerPawn = Cast<ABladeIIPlayer>(*Iterator);

	//	if (playerPawn)
	//	{
	//		bool bCurrentHidden = playerPawn->IsHidden();

	//		if (!bForceHide)
	//			playerPawn->SetHideFromEventDirector(false);
	//		else
	//			playerPawn->SetHideFromEventDirector(bCurrentHidden || bTryHidePlayer);
	//	}
	//} 
}

bool AB2StageEventDirector::CheckForPCSyncToMatineeStart()
{
	//ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();

	//if (bPCSyncToMatineeStart && LocalB2Player && MatineeActor && MatineeActor->MatineeData)
	//{
	//	for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
	//	{
	//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
	//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];

	//		// CurrentPlayer扒 MainPlayer扒 SubPlayer扒 泅犁 敲饭捞窍绊 乐绰 某腐磐客 老摹窍搁 教农 措惑
	//		if (CurrShowState.IsForLocalPlayerCharacter())
	//		{
	//			// 泅犁 技泼俊辑狼 move 飘发阑 茫酒辑 霉 虐橇饭烙阑 格钎 瘤痢栏肺
	//			
	//			const FName FinalMatchingGroupName(*GetFinalMatchingInterpGroupName(LocalB2Player, CurrShowSetting.MatchingInterpGroupName.ToString(), false));
	//			UInterpGroupInst* CurrGroupInst = MatineeActor->FindFirstGroupInstByName(FinalMatchingGroupName.ToString()); // 夸扒 捞抚栏肺 官肺 茫阑 荐 乐绰 淀.
	//			
	//			if (CurrGroupInst && CurrGroupInst->Group)
	//			{
	//				for (int32 TI = 0; TI < CurrGroupInst->Group->InterpTracks.Num(); ++TI)
	//				{
	//					UInterpTrackMove* MoveTrack = Cast<UInterpTrackMove>(CurrGroupInst->Group->InterpTracks[TI]);
	//					
	//					if (MoveTrack && CurrGroupInst->TrackInst.Num() > TI)
	//					{
	//						// UInterpGroup::UpdateGroup 焊搁 InterpTracks 苞 TrackInst 牢郸胶 老摹矫难辑 荤侩.

	//						// 霉 虐 橇饭烙 蔼 哗咳.
	//						float KeyTime;
	//						FVector FirstKeyPosition;
	//						MoveTrack->ExternalGetKeyframePosition(CurrGroupInst->TrackInst[TI], 0, KeyTime, FirstKeyPosition, NULL, NULL);
	//						// 雀傈蔼篮 扁夯 扁瓷栏肺绰 给嘎苗临 淀..
	//						//FVector FirstKeyRotEuler;
	//						//MoveTrack->ExternalGetKeyframeRotation(CurrGroupInst->TrackInst[TI], 0, KeyTime, FirstKeyRotEuler, NULL, NULL);

	//						// NavigationSystem 呼妨辑 捞悼. 促青洒 PlayerController 肺档 累悼窍绰焙 せ
	//						UNavigationSystem::SimpleMoveToLocation(LocalB2Player->Controller, FirstKeyPosition);
	//						// PathFind 辆丰 妮归 殿废.
	//						UPathFollowingComponent* PFComp = LocalB2Player->Controller->FindComponentByClass<UPathFollowingComponent>();
	//						if (PFComp)
	//						{
	//							// SimpleMoveToLocation 俊辑 茄锅 眉农窍绰单 弊 锭俊绰 妮归捞 救阂赴促. 殿废档 救茄 惑怕绊 促弗 巩力档 乐绊. 绢瞒乔 捞固 档崔沁栏搁 被捞 Sync 且 鞘夸啊 绝澜.
	//							if (PFComp->HasReached(FirstKeyPosition, EPathFollowingReachMode::OverlapAgent) == false)
	//							{
	//								PFComp->OnRequestFinished.AddUObject(this, &AB2StageEventDirector::OnPCSyncToMatineeStartCompleted);
	//								// Sync 吝 钎矫. 捞霸 场唱搁 夯拜 付萍聪 敲饭捞 矫累.
	//								bCurrentPCSyncToMatineeStart = true;
	//							}
	//						}

	//						// 付瘤阜栏肺.. 漂沥 矫埃捞 瘤唱搁 公炼扒 矫累窍档废 茄促. 呈公 钢府辑 坷霸 登搁 档吝俊 谗扁摆瘤父, 绢凋啊俊 阜囚辑 歹 捞惑 柳青捞 登瘤 臼绰 巴焊窜 炒摆瘤. 
	//						// 鞘夸俊 蝶扼 函荐肺 畴免..
	//						const float PCSyncFallBackTimerTime = 10.0f; 
	//						GetWorldTimerManager().SetTimer(PCSyncToMatineeStartFallbackTimerHandle, this, &AB2StageEventDirector::PCSyncToMatineeStartFallbackTimerCB, PCSyncFallBackTimerTime, false);

	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//// 角力 敲饭捞窍扁 傈鳖瘤 牢霸烙 惑怕肺 登倒妨 初绰促. SetupSingleShowMatineeGroup 俊辑 茄 老何甫 登倒妨 初澜.
	//if (bCurrentPCSyncToMatineeStart)
	//{
	//	for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
	//	{
	//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
	//		// 捞 版快 SkeletalMeshActor 甸篮 贸澜何磐 焊咯龙 抛聪 见败辑档 救登摆瘤.
	//		const bool bVisibleFromFirstPlace = (CurrShowSetting.PlayActorStateMode == EPlayActorStateMode::EPASM_DirectedFromBeginning);

	//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];
	//		ABladeIICharacter* ThisPlayActor = CurrShowState.GetPlayActor(this);
	//		if (ThisPlayActor)
	//		{
	//			// SetupSingleShowMatineeGroup 俊辑 见败初篮 巴甸阑 促矫 泪矫 焊捞档废. 

	//			bool bOneOfTempSpawnedPuppet = false;
	//			for (int32 TPI = 0; TPI < TempSpawnedPuppets.Num(); ++TPI)
	//			{
	//				if (ThisPlayActor == TempSpawnedPuppets[TPI])
	//				{
	//					bOneOfTempSpawnedPuppet = true;
	//				}
	//			}

	//			// TempSpawnedPuppet 篮 绢瞒乔 焊咯林搁 救凳. 肚茄 SetStageEventShowState 档 SetupSingleShowMatineeGroup 俊辑 技泼茄 弊措肺 出抵具 见败柳 盲肺 spawn 葛记 鞍篮 霸 敲饭捞 登瘤 臼阑 巴.
	//			if (bOneOfTempSpawnedPuppet || bVisibleFromFirstPlace)
	//			{
	//				ThisPlayActor->SetHideFromEventDirector(true);
	//			}
	//			else
	//			{
	//				ThisPlayActor->SetHideFromEventDirector(false); // 唱赣瘤绰 牢霸烙 惑怕措肺..

	//				// BeginSingleShow 俊辑 捞霸 扁夯栏肺 甸绢啊 乐绢辑 局聪皋捞记捞 唱坷瘤 臼绰单 付萍聪 葛靛俊急 StageEventShowState 技泼捞 吝夸茄 霸 酒丛 せ
	//				ThisPlayActor->SetStageEventShowState(EStageEventShowState::ESES_None, NULL);
	//			}
	//		}
	//		if (CurrShowState.CtrlMatineePuppet)
	//		{
	//			SetVisibilityOfMatineePuppet(CurrShowState.CtrlMatineePuppet, CurrShowState.GetPlayActor(this), bVisibleFromFirstPlace);
	//		}
	//	}

	//	// 敲饭捞绢 某腐磐绰 焊捞歹扼档 FinalBeginMatineePlay 窍扁 傈俊 咯扁辑档 CinematicMode 柳涝捞 鞘夸.
	//	EnterCinematicModeForControlledMatinee();

	//	return true;
	//}

	return false;
}

void AB2StageEventDirector::BeginMatineeFromPCSyncToMatineeStart()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::BeginMatineeFromPCSyncToMatineeStart"));

	//if (bCurrentPCSyncToMatineeStart)
	//{
	//	bCurrentPCSyncToMatineeStart = false;

	//	// CheckForPCSyncToMatineeStart 俊辑 技泼茄 芭 促矫 付萍聪 敲饭捞甫 困秦 倒妨初绰促. 促矫 SetupSingleShowMatineeGroup 俊辑 秦 初篮 惑怕肺
	//	for (int32 EI = 0; EI < ShowStates.Num(); ++EI)
	//	{
	//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
	//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];

	//		if (!CurrShowSetting.IsDirectedMode())
	//		{
	//			continue;
	//		}

	//		ABladeIICharacter* ThisPlayActor = CurrShowState.GetPlayActor(this);
	//		if (IsValid(ThisPlayActor))
	//		{
	//			// 喊肺 吝夸茄 扒 酒囱单.. ESES_None 栏肺 初篮 StageEventShowState 档 唱抚 狼固乐绰 蔼栏肺 促矫 技泼秦 初磊. 荤角 付萍聪 葛靛俊急 奴 狼固绰 绝促..
	//			ThisPlayActor->SetStageEventShowState(GetDefaultDesiredPlayActorSESState(DirectingEvent, CurrShowSetting.PlayActorType), this);
	//			
	//			// PlayActor 甸 见扁绰 扒 酒贰 FinalBeginMatineePlay 俊辑
	//		}
	//		if (CurrShowState.CtrlMatineePuppet)
	//		{
	//			SetVisibilityOfMatineePuppet(CurrShowState.CtrlMatineePuppet, CurrShowState.GetPlayActor(this), true);
	//		}
	//	}

	//	FinalBeginMatineePlay(); // 矫累

	//	DeferredBeginShowHandlingForMatineePCSync(); // BeginShow 俊辑 贸府 给沁带 巴甸 付历 贸府.
	//}
}

void AB2StageEventDirector::OnPCSyncToMatineeStartCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	BeginMatineeFromPCSyncToMatineeStart();
}

void AB2StageEventDirector::PCSyncToMatineeStartFallbackTimerCB()
{
	GetWorldTimerManager().ClearTimer(PCSyncToMatineeStartFallbackTimerHandle);
	BeginMatineeFromPCSyncToMatineeStart(); // 措眉肺 捞固 BeginMatineeFromPCSyncToMatineeStart 啊 贸府登菌阑 巴牢单 弊繁 版快搁 酒公 老 救老绢朝 巴.
}

ABladeIICharacter* AB2StageEventDirector::GetRelevantPlayActor(EStageEventPlayActor InType, FString& WaveMobSelectKeyword, int32 PlayerIndex)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::GetRelevantPlayActor"));

	//ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();

	//if (InType == EStageEventPlayActor::ESEE_CurrentPlayer)
	//{
	//	if (LocalB2Player)
	//	{ // 磷绊乐绰 芭扼搁 楷免 饶俊 sync 甫 且 鞘夸档 绝摆绊.. 咯窍埃 困氰且 荐 乐栏聪 puppet 栏肺?
	//		return LocalB2Player->IsAlive() ? LocalB2Player : LocalB2Player->SpawnMyselfAsPuppet();
	//	}
	//}
	//else if (InType == EStageEventPlayActor::ESEE_OtherPlayer)
	//{
	//	return GetOtherTagClassPuppet();
	//}
	//else if (InType == EStageEventPlayActor::ESEE_MainPlayer || InType == EStageEventPlayActor::ESEE_SubPlayer)
	//{
	//	if (
	//		LocalB2Player &&
	//		((InType == EStageEventPlayActor::ESEE_MainPlayer && LocalB2Player->AmIMainCharacter() == true) ||
	//		(InType == EStageEventPlayActor::ESEE_SubPlayer && LocalB2Player->AmIMainCharacter() == false))
	//		)
	//	{
	//		// 泅犁 敲饭捞窍绊 乐绰 某腐磐扼搁 货肺 spawn 窍芭唱 且 鞘夸绝捞 弊措肺 啊廉客辑 荤侩.
	//		// 弊犯瘤父 磷绊乐绰 芭扼搁 楷免 饶俊 sync 甫 且 鞘夸档 绝摆绊.. 咯窍埃 困氰且 荐 乐栏聪 puppet 栏肺?
	//		return LocalB2Player->IsAlive() ? LocalB2Player : LocalB2Player->SpawnMyselfAsPuppet();
	//	}
	//	else
	//	{
	//		// 酒聪搁 惑措率阑 player controller 俊 possess 登瘤 臼篮 盲肺 spawn 秦辑 荤侩.
	//		return GetOtherTagClassPuppet();
	//	}
	//}
	//else if (InType == EStageEventPlayActor::ESEE_WinningPlayer_Current || InType == EStageEventPlayActor::ESEE_WinningPlayer_Other)
	//{
	//	const bool bCurrentOne = (InType == EStageEventPlayActor::ESEE_WinningPlayer_Current);

	//	return bMatchResultEventAsWinner ?
	//		(bCurrentOne ? LocalB2Player : GetOtherTagClassPuppet()) :
	//		(bCurrentOne ? GetB2MatchOpponent() : GetRemoteOtherTagClassPuppet());
	//}
	//else if (InType == EStageEventPlayActor::ESEE_RemoteOpponent_Current) // RemoteOpponent_Other 绰 鞘夸秦瘤搁 眠啊. ESEE_WinningPlayer_Other 曼绊秦辑
	//{
	//	return GetB2MatchOpponent();
	//}
	//else if (InType == EStageEventPlayActor::ESEE_TeamAllyPlayer || InType == EStageEventPlayActor::ESEE_TeamEnemyPlayer)
	//{
	//	return GetTeamPlayer(InType == EStageEventPlayActor::ESEE_TeamAllyPlayer, PlayerIndex, false, true); // 瘤绰 版快俊 捞吧 荤侩窍妨搁 live 父 啊廉坷搁 酒公档 绝阑 巴. 磷篮 局绰 伯单扁扼档 粱厚贸烦 何劝矫难辑 单妨柯促.
	//}
	//else if (InType == EStageEventPlayActor::ESEE_TeamWinningPlayer)
	//{
	//	return GetTeamPlayer(bMatchResultEventAsWinner, PlayerIndex, false, true);
	//}
	//else if (InType == EStageEventPlayActor::ESEE_RaidPlayer)
	//{
	//	return GetRaidPlayer(PlayerIndex);
	//}
	//else if (InType == EStageEventPlayActor::ESEE_WinningRaidPlayer || InType == EStageEventPlayActor::ESEE_DefeatedRaidPlayer)
	//{	
	//	return GetRaidPlayer(PlayerIndex, false);
	//}
	//else if (InType == EStageEventPlayActor::ESEE_WaveMob)
	//{
	//	ABladeIIGameMode* B2GM = GetB2GameMode();
	//	//if (B2GM)
	//	//{
	//	//	int32 WaveNum = 0;
	//	//	int32 MobIndex = 0;
	//	//	ParseForWaveMobSelectKeyword(WaveMobSelectKeyword, WaveNum, MobIndex); // 巩磊凯肺 瘤沥茄 可记俊辑 锅龋甸 哗咳.

	//	//	TArray<ABladeIICharacter*> InterestedWaveMobs;
	//	//	B2GM->GetSingleWaveMobList(WaveNum, InterestedWaveMobs, false); // 磷绢啊绰 赤籍档 器窃秦辑 啊廉咳. 拱沸 弊霸 啊瓷茄 鸥捞怪捞绢具 窃. 酒囱 版快 酒贰 DelayedDyingMobCache 蛮胶啊 乐变 窍促.

	//	//	for (int32 WMI = 0; WMI < InterestedWaveMobs.Num(); ++WMI)
	//	//	{
	//	//		ABladeIICharacter* CurrMob = InterestedWaveMobs[WMI];
	//	//		if (CurrMob)
	//	//		{
	//	//			if (CurrMob->GetSpawnPoolWaveObjIndex() == MobIndex) // 荤角惑 Wave 啊 矫累等 惑炔捞 酒聪搁 茫绰 牢郸胶狼 mob 捞 绝阑 荐档 乐促.
	//	//			{
	//	//				return CurrMob;
	//	//			}
	//	//		}
	//	//	}
	//	//	// 困俊辑 府畔阑 窍瘤 臼疽促搁 MobDying 俊辑 delay 甫 霖 版快老 荐 乐促. 葛电 dying mob 捞 啊瓷茄 扒 酒聪瘤父 飘府芭甫 蜡惯茄 dying mob 鳖瘤绰 酒贰贸烦 啊廉棵 荐 乐澜..
	//	//	if (NeedDelayedDyingMobCache())
	//	//	{
	//	//		return DelayedDyingMobCache.GetIfMatch(WaveNum, MobIndex);
	//	//	}
	//	//}
	//}
	//else if (InType == EStageEventPlayActor::ESEE_SetUpOutside)
	//{
	//	if (SetUpOutSideCharacter)
	//	{
	//		return SetUpOutSideCharacter;
	//	}
	//}
	//else if (IsPlayActorTypeTutorialFixedClass(InType))
	//{
	//	return GetTutorialPlayActor(InType);
	//}

	return NULL;
}

ABladeIIPlayer* AB2StageEventDirector::GetTutorialPlayActor(EStageEventPlayActor InType)
{
	checkSlow(IsPlayActorTypeTutorialFixedClass(InType));

	EPCClass PCClassEnum = TutorialPlayActorTypeToPCClass(InType);
	checkSlow(PCClassEnum != EPCClass::EPC_End);
	if (PCClassEnum == EPCClass::EPC_End) {
		return nullptr;
	}
//
//	// 泅犁 肺拿 某腐磐何磐 眉农. 咯傈洒 (PendingKill 捞 酒囱) 肺拿 某腐磐客 老摹茄促搁 弊吧 啊廉促 敬促.
//	ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
//	if (LocalB2Player && LocalB2Player->IsValidObj() && LocalB2Player->GetCurrentPlayerClass() == PCClassEnum) {
//		return LocalB2Player;
//	}
//
//	// 譬配府倔 霸烙葛靛扼搁 固府 捞 格利栏肺 积己秦初篮 FallbackPuppet 甸阑 荤侩
//	AB2TutorialGameMode* TutorialGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this));
//	if (TutorialGM)
//	{
//		return TutorialGM->GetFallbackPuppetPlayer(PCClassEnum);
//	}
//#if WITH_EDITOR
//	// 俊叼磐扼搁 酒流 TutorialGameMode 汲沥窍瘤 臼篮 惑怕俊辑 扁瓷 痢八捞 鞘夸窍骨肺..
//	// 捞吧 老馆利牢 霸烙葛靛 瞒盔栏肺 犬厘窍绰 扒 帮鄂窍促. 
//	// 扁瓷 磊眉具 悼累窍摆瘤父 肋 荤侩窍瘤档 臼阑 捞繁 扁瓷锭巩俊 FallbackPuppet 甸阑 积己秦 初酒具 窍绰 霸 扯厚.
//	else if (GIsEditor)
//	{
//		//
//		// 俺惯侩 烙矫内靛捞骨肺 农霸 脚版静瘤 臼绊 咯扁俊 弊成 阜 楼绰 芭.
//		//
//
//		ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//		if (B2GM)
//		{
//			TSubclassOf<APawn> DefaultPawnClassBackup = B2GM->DefaultPawnClass;
//			// 歹固甸 spawn 矫难具 秦辑 泪瘪 谍 努贰胶肺 官层初绊..
//			B2GM->DefaultPawnClass = ABladeIITutorialFallbackPuppetPlayer::StaticClass();
//
//			FTransform NewSpawnTransform(FVector(100000.0f * FMath::FRandRange(0.1f, 2.0f), 100000.0f * FMath::FRandRange(0.1f, 2.0f), 100000.0f * FMath::FRandRange(0.1f, 2.0f)));
//			ABladeIITutorialFallbackPuppetPlayer* SpawnedFallbackPuppet = Cast<ABladeIITutorialFallbackPuppetPlayer>(
//				ABladeIIGameMode::SpawnPlayerCharAsPuppet(this, PCClassEnum, NewSpawnTransform, true));
//
//			B2GM->DefaultPawnClass = DefaultPawnClassBackup;
//
//			return SpawnedFallbackPuppet;
//		}
//	}
//#endif
	return nullptr;
}

ABladeIIPlayer* AB2StageEventDirector::GetOtherTagClassPuppet()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::GetOtherTagClassPuppet"));

	ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
		
	if (LocalB2Player)
	{
		// 捞 锭 spawn animation 篮 绝档废 且 鞘夸啊 乐阑 芭 鞍澜.. 利绢档 捞霸 阂府绰 霸 矫累 惑炔篮 酒匆 淀窃.
		ABladeIIPlayer* NewPuppet = LocalB2Player->SpawnOtherAsPuppet(true,
			false // 怕弊 某腐磐 率 puppet 狼 point light 啊 难廉具 茄促搁 眠啊 牢磊甫..
		); 
		if (NewPuppet)
		{
			TempSpawnedPuppets.Add(NewPuppet);
			return NewPuppet;
		}
	}
	BII_CHECK(0);
	return NULL;
}

ABladeIIPlayer* AB2StageEventDirector::GetRemoteOtherTagClassPuppet()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::GetRemoteOtherTagClassPuppet"));

	//AB2PVPGameMode* PVPGM = Cast<AB2PVPGameMode>(GetB2GameMode());
	//// 泅犁绰 PVP 葛靛 opponent 父
	//ABladeIIPlayer* NewPuppet = PVPGM ? PVPGM->SpawnRemoteTagAsPuppet() : NULL;
	//
	//if (NewPuppet)
	//{
	//	TempSpawnedPuppets.Add(NewPuppet);
	//	return NewPuppet;
	//}

	BII_CHECK(0);
	return NULL;
}

class UCameraComponent* AB2StageEventDirector::GetManualCameraFromSetting(FManualCameraSingleSetting& InSetting)
{
	if (InSetting.CameraIndex >= 0 && InSetting.CameraIndex < MAX_DIRECTOR_CAMERA)
	{
		if (ManualCameraSelection == EManualCameraSelection::EDCT_DirectorActorCamera)
		{
			return DirectorCamArrayRef[InSetting.CameraIndex];
		}

		// TODO : 促弗 墨皋扼 set. 鞘夸窍妨唱 葛福摆瘤父..
	}

	return NULL;
}

void AB2StageEventDirector::CheckShowEndCountDown()
{
	if (IsOverallLengthConstrainted()) // OverallLengthConstrainted 扼搁 蝶肺 汲摹茄 鸥捞赣俊 狼秦 辆丰瞪 巴.
	{
		return;
	}

	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee) // Controlled Matinee 绰 促弗 版肺甫 烹秦 辆丰 眉农
	{
		return;
	}

	// 傈眉 show 啊 辆丰登绰 炼扒甸阑 八荤茄促.

	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera && bAllManualCameraEnded == false){
		return;
	}

	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera && bCameraAnimFinished == false){
		return;
	}

	if (MyRelevantUIName != NAME_None && !bRelevantUIOpenComplete){
		return;
	}

	for (int32 SI = 0; SI < ShowStates.Num(); ++SI)
	{
		// FinishCheck 啊 鞘夸绝绰 技泼捞 乐澜.
		if (DoesShowRequireFinishCheck(SI) == true && ShowStates[SI].bShowFinished == false)
		{
			return;
		}
	}

	// 付瘤阜栏肺 bStandbyAtTheEnd 甫 眉农.. 捞 版快绰 拱沸 CustomFinishShow 甫 秦具
	if (bStandbyAtTheEnd){
		OnStartStandbyAtTheEnd();
	}
	else{
		FinishShow();
	}
}

void AB2StageEventDirector::CheckShowEndCountDown_ControlledMatinee(bool bForStandByNoti)
{
	if (OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee){
		return;
	}
	if (!bControlledMatineeFinished){
		return;
	}
	if (MyRelevantUIName != NAME_None && !bRelevantUIOpenComplete){
		return;
	}

	if (bForStandByNoti){
		check(bStandbyAtTheEnd);
		OnStartStandbyAtTheEnd();
	}
	else{
		CustomFinishShow(); // 咯扁鳖瘤 吭促搁 场. FinishShow 啊 酒囱 CustomFinishShow 肺 场辰促.
	}	
}

bool AB2StageEventDirector::DoesShowRequireFinishCheck(int32 ShowIndex)
{
	if (ShowIndex >= 0 && ShowIndex < ShowSettings.Num() && ShowIndex < ShowStates.Num())
	{
		FPlayActorShowSettings& CurrSetting = ShowSettings[ShowIndex];
		FPlayActorShowRuntimeState& CurrState = ShowStates[ShowIndex];
		ABladeIICharacter* PlayActor = CurrState.GetPlayActor(this);

		if (
			CurrSetting.IsDirectedMode() && // InGame State 弊措肺搁 迭洒 矫埃 殿狼 促弗 炼扒阑 林瘤 臼绰 捞惑 辆丰 眉农甫 且 荐啊 绝澜.
			PlayActor && PlayActor->GetStageEventShowState() != EStageEventShowState::ESES_None &&
			PlayActor->GetStageEventShowState() != EStageEventShowState::ESES_Extra // Extra 绰 酒付档 SESEndNotify 绝捞 窜鉴洒 Idle 阑 倒府霸 瞪 巴..
			)
		{
			return true;
		}
	}
	return false;
}

void AB2StageEventDirector::FinishShow()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::FinishShow"));
//
//	if (!bCurrentlyPlaying){ // 吝汗贸府啊 瞪 荐 乐澜.
//		return;
//	}
//
//	// CustomFinishShow 鞍篮 惑炔捞搁 固贸 贸府啊 救等 惑炔老 荐 乐绢辑 鸥捞赣甸阑 促矫 茄锅 犬角洒 努府绢 茄促.
//	FTimerManager& WorldTimerManager = GetWorldTimerManager();
//	if (OverallStartDelayTimerHandle.IsValid()) {
//		WorldTimerManager.ClearTimer(OverallStartDelayTimerHandle);
//	}
//	if (OverallLengthTimerHandle.IsValid()) {
//		WorldTimerManager.ClearTimer(OverallLengthTimerHandle);
//	}
//	if (ManualCameraBlendTimerHandle.IsValid()) {
//		WorldTimerManager.ClearTimer(ManualCameraBlendTimerHandle);
//	}
//	if (StartEventWidgetTimerHandle.IsValid()) {
//		WorldTimerManager.ClearTimer(StartEventWidgetTimerHandle);
//	}
//	if (FinishEventWidgetTimerHandle.IsValid()) {
//		WorldTimerManager.ClearTimer(FinishEventWidgetTimerHandle);
//	}
//	if (PCSyncToMatineeStartFallbackTimerHandle.IsValid()) {
//		WorldTimerManager.ClearTimer(PCSyncToMatineeStartFallbackTimerHandle); // 娟绰 角力 楷免 扁瓷窍绊 蝶肺 档绰 鸥捞赣扼 歹歹宽 捞犯霸 波林绢具 窃
//	}
//	if(LevelStreamingOnBeginFlushTH.IsValid()){
//		WorldTimerManager.ClearTimer(LevelStreamingOnBeginFlushTH);
//	}
//	if (LevelStreamingOnEndFlushTH.IsValid()) {
//		WorldTimerManager.ClearTimer(LevelStreamingOnEndFlushTH);
//	}
//
//	ABladeIIGameMode* B2GM = GetB2GameMode();
//	
//	//if (IsUsingMobileRendering(GetWorld())) // 母媚 康惑 父甸 锭 扼捞飘 掺瘤 臼栏妨绊 捞繁 眉农啊 鞘夸茄 巴 鞍篮单 StageEventDirector 绰 牢霸烙 扁瓷捞扼 捞繁 眉农啊 鞘夸且鳖 酵扁档.
//	//{
//	//	// 葛官老 坊歹矾俊辑 dynamic point light 俺荐 力茄阑 困秦. 楷免 悼救 荤侩茄 霸 乐阑 波扼 辆丰窍搁辑 犬角洒 波滴绰 芭. 鞘夸茄 扒 促澜 楷免 甸绢哎 锭 促矫 难龙 巴.
//	//	if (B2GM)
//	//		B2GM->DeactivateWorldMovablePointLight();
//	//}
//
//	ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
//	ABladeIIPlayerController* LocalB2PC = GetB2LocalPlayerCon();
//
//	// bStandbyPause 俊 狼秦 pause 等 惑怕甫 倒妨淋
//	if (bStandbyAtTheEnd && bStandbyPause && UGameplayStatics::IsGamePaused(this))
//	{
//		UGameplayStatics::SetGamePaused(this, false);
//	}
//		
//	// 匙捞萍宏俊辑绰 捞力 捞犯霸 捞亥飘甫..
//	StageEventSceneEndClass<AB2StageEventDirector*>::GetInstance().Signal(this);
//
//	check(ShowStates.Num() == ShowSettings.Num());
//	for (int32 SI = 0; SI < ShowStates.Num(); ++SI)
//	{
//		B2_SCOPED_TRACK_LOG_L2(TEXT("AB2StageEventDirector::FinishShow ShowState"));
//
//		FPlayActorShowSettings& ThisSetting = ShowSettings[SI];
//		FPlayActorShowRuntimeState& ThisState = ShowStates[SI];
//		
//		if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//		{
//			// 咯辨 芭摹搁辑 ThisState 狼 PlayActor 啊 力芭瞪 荐 乐促.
//			if(ThisSetting.PlayActorType != EStageEventPlayActor::ESEE_SetUpOutside)
//				RestorePlayActorForCtrlMatineeEnd(ThisSetting, ThisState);
//		}
//						
//		ABladeIICharacter* ThisPlayActor = ThisState.GetPlayActor(this);
//		if (ThisPlayActor)
//		{
//			ThisPlayActor->AllowMovementByStageEvent(); // PreventMovement 窍绰 纳捞胶啊 乐绰单 傲 葛滴 汗备.
//
//			if (ThisSetting.EndActionPlayActorAdjustType == EEndActionPlayActorAdjustType::EAPAAT_BackToOriginal)
//			{
//				// PlayActor 狼 困摹 棺 雀傈蔼阑 汗盔矫糯.
//				ThisPlayActor->SetActorLocation(ThisState.BackupWorldLocation);
//				ThisPlayActor->SetActorRotation(ThisState.BackupWorldRotation);
//			}
//
//			// CustomFinish 趣篮 Extra 牢 版快 NotifyStageEventShowEnd 啊 角青登瘤 臼疽阑 巴. 
//			// 捞率栏肺狼 辆丰 notify 绝捞 惑怕父 辆丰. bStandbyAtTheEnd 牢 版快档 促矫 Extra 肺 登倒府聪 鞘夸..
//			ThisPlayActor->SetStageEventShowState(EStageEventShowState::ESES_None, NULL);
//			ThisPlayActor->SetPlayingDirectedStageEvent(false);
//		}
//
//		ThisState.bShowFinished = true; // 犬角茄 付农
//		ThisState.SetPlayActor(NULL); // 硼厘 せ
//	}
//	ShowStates.Empty();
//	DelayedDyingMobCache.ForgetIt();
//
//	// 娟甸篮 流立 力芭秦 林绢具 窃.
//	if (GetWorld())
//	{
//		B2_SCOPED_TRACK_LOG_L2(TEXT("AB2StageEventDirector::FinishShow TempSpawnedPuppets Destroy"));
//
//		for (int32 SPI = 0; SPI < TempSpawnedPuppets.Num(); ++SPI)
//		{
//			if (TempSpawnedPuppets[SPI])
//			{
//				GetWorld()->DestroyActor(TempSpawnedPuppets[SPI]);
//			}
//		}
//	}
//	TempSpawnedPuppets.Empty();
//	
//	FinishEventWidget(); // EventWidgetEndTime 技泼狼 版快 捞固 辆丰登菌阑 荐 乐促.
//
//	if (bStopAllMobsWhilePlaying && CachedStageManager)
//	{ // BeginShow 俊辑 Stop 矫淖带 巴甸 促矫 登混覆.
//		CachedStageManager->StopOrRestoreAllWorldMobs(false);
//	}
//
//	if (LocalB2PC)
//	{
//		// CinematicMode 辆丰. 酒贰 NotifyShowOverFromDependentDirectorActor 捞饶俊 甸绢啊搁 俺帮鄂
//		LocalB2PC->SetCinematicMode(false, false, true, true, true);
//	}
//	// 见板带 喉饭捞绢 焊捞霸
//	if(!bForceNoHidePlayer)
//		SetAllPlayerHiddenInGame(false);
//
//	// 傈捧 UI 坷橇醋 局聪皋捞记阑 敲饭捞且 鞘夸啊 粱 乐绢辑 捞繁 还窿阑 窃.
//	if (IsPossibleAdventureModeBeginEvent())
//	{
//		// todo : 夸扁 窍靛内爹 何盒 官曹鞘夸啊 乐澜夸 =し= (惑加 屈怕肺)
//		UB2UIBattleMain* BattleMainUI = nullptr;
//		if (GetB2GameModeType(this->GetWorld()) == EB2GameMode::CounterDungeon)
//			BattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::CounterDGBattleMain);
//		else
//			BattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::BattleMain);
//		if (BattleMainUI)
//		{
//			if(!AB2StageManager::GetCacheStageKeepEssentialData().IsPlayInExtraLevel())
//				BattleMainUI->OnStageBeginEventSceneEnd();
//		}
//	}
//	else if (DirectingEvent == EStageEvent::ESEV_Begin && MyRelevantUIName == UIFName::PVPBegin)
//	{
//		// todo : 夸扁 窍靛内爹 何盒 官曹鞘夸啊 乐澜夸 =し= (惑加 屈怕肺)
//
//		// pvpUI
//		UB2UIPVP1on1BattleMain* PVPBattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIPVP1on1BattleMain>(UIFName::PVPOneOnOneBattleMain);
//		if (PVPBattleMainUI)
//		{
//			PVPBattleMainUI->OnPVPBeginEventSceneEnd();
//		}
//
//		// 评措傈UI - 捞巴档 悼老茄 UIName 阑 荤侩窃.
//		UB2UITeamMatchBattle* TMBattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UITeamMatchBattle>(UIFName::TeamMatchBattle);
//		if (TMBattleMainUI)
//		{
//			TMBattleMainUI->OnPVPBeginEventSceneEnd();
//		}
//		
//		if (UB2UIBattleMain* RaidBattleUI = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::RaidBattle))
//			RaidBattleUI->OnStageEventDirectorEndScene_BP();
//	}
//
//	// 捞傈俊 促福霸 甸绢艾阑 荐档 乐瘤父 沥惑栏肺 倒妨霖促 摹磊.
//	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
//
//	// 敲饭捞绢 TopDown 墨皋扼 郴瘤绰 ActiveCamera 肺..
//	ReturnToGamePlayCamera();
//
//	LevelStreamingOnEnd.ExecuteStreaming(GetWorld(), LevelStreamingOnEndFlushTH, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::LevelStreamingOnEndFlushCB));
//
//	// Auto 惑怕 汗备
//	if (ShouldStopAutoDuringShow() && B2GM && B2GM->IsLocalPlayerAutoBattle()) // 汲飞 矫累且 锭俊绰 Auto 啊 酒聪菌促 窍歹扼档 吝埃俊 绢恫 荤蜡肺电 官差搁 Auto 甫 汗备秦霖促.
//	{
//		B2GM->RestoreLocalPlayerAuto();
//	}
//
//#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
//	if (bPlayedAtLeastOnce == false)
//	{			
//		bPlayedAtLeastOnce = true; // 捞锅 胶抛捞瘤俊辑 弥家茄 茄锅 敲饭捞 窍看澜.
//		GConfig->SetBool(*GetPlayedAtLeastOnceSectionString(), *GetPlayedAtLeastOnceKeyString(CurrentStageId), bPlayedAtLeastOnce, GB2LocalCutSceneSavedStateIni);
//		GConfig->Flush(false, GB2LocalCutSceneSavedStateIni);
//	}
//#endif
//
//	if (LocalB2Player) 
//	{ // 扁鸥 殿殿..
//		LocalB2Player->SetupLODInfoForInGame(); // 肺拿 敲饭捞绢绰 楷免 曼咯 咯何 惑包绝捞 公炼扒 CinematicLOD 汲沥窍聪 登倒覆.
//		LocalB2Player->SetInvincibleByStageEvent(false);
//	}
//	B2Scalability::ApplyStageEventSpecificScalabilitySetting(this, false); // 肺拿 敲饭捞绢 LOD 技泼苞 付蛮啊瘤肺 楷免 侩 漂喊 scalability 牧飘费 利侩 府悸
//
//	if (bWasPlayerAttacking && LocalB2Player && LocalB2Player->bCachedPressedInCinematic && LocalB2Player->bEnableNormalAttack)
//		LocalB2Player->SetKeepAttacking(true);
//	
//#if BII_STATS
//	FStatisticalPerfStatCollector::bIsCurrentlyInEventScene = false;
//#endif
//
//	SetUpOutSideCharacter = nullptr;
//	MyRelevantUIName = NAME_None;
//	bMatchResultEventAsWinner = false;
//	bCurrentlyPlaying = false;
//	
//	StageEventDirectorFinishShowEventClass<int32>::GetInstance().Signal((int32)DirectingEvent);
//
//	PostFinishShow();
}

void AB2StageEventDirector::PostFinishShow()
{
	checkSlow(!bCurrentlyPlaying); // 犬角洒 场抄 敲贰弊 甸绢柯 饶俊 累悼 啊瓷茄 巴甸..

	//// BP 俊辑 漂喊洒 捞亥飘甫 罐酒辑 贸府窍绊磊 茄促搁..
	//if (ShowOverEventName != NAME_None)
	//{
	//	GEngine->HandleCeCommand(GetWorld(), *(ShowOverEventName.ToString()), *GLog);
	//}

	//if (IsPossibleAdventureModeBeginEvent())
	//{ // BeginShow 俊辑档 Async 夸没窍绰 霸 乐绰单 捞巴档 焊胶 殿厘 矫 hitch 甫 蜡惯窍聪 利寸茄 矫痢俊..

	//	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
	//	// 咯扁辑狼 Async 肺爹 夸没篮 林狼且 鞘夸啊 乐绰单 咯扁辑 夸没阑 窍搁 牢霸烙 傈捧啊 倒酒啊绰 客吝俊 肺爹捞 倒酒哎 巴捞促.
	//	// 捞订 弊巴鳖瘤 积阿阑 给 沁绊 捞 沥档绰 沥富 累篮 巴捞聪 弊成 毫霖促. 
	//	// 捞繁 鸥捞怪俊 async 肺爹 夸没捞 歹 眠啊登搁 帮鄂且 巴捞促.
	//	UB2UIManager::GetInstance()->ReqAsyncLoadAsset(UIFName::MobAppearBossStage);
	//}

	//ABladeIIGameMode* B2GM = GetB2GameMode();
	//if (B2GM)
	//{		
	//	if (DirectingEvent == EStageEvent::ESEV_Begin && 
	//		(B2GM->GetB2GameModeType() == EB2GameMode::PVP_Tag || B2GM->GetB2GameModeType() == EB2GameMode::PVP_Team || B2GM->GetB2GameModeType() == EB2GameMode::Guild))
	//	{// MatchGameStartEventSceneEnd 矫弊澄 利侩 措惑 霸烙葛靛甸 葛滴..
	//		MatchGameStartEventSceneEndClass<>::GetInstance().Signal();
	//	}

	//	B2GM->MarkAnyStageEventPlayedOnce(); // 绢路电 弥家 窍唱 敲饭捞 凳. せ

	//	// 郴啊 场唱辨 扁促府绊 乐绰 促弗 DirectorActor 啊 乐促搁 Notify 甫 朝赴促. 促弗 镑俊辑 官肺 矫累捞 瞪 荐 乐栏骨肺 咯扁狼 辆丰贸府客 吝酶登瘤 臼档废 场俊 啊辑.
	//	const TArray<AB2StageEventDirector*>& AllSEDs = B2GM->GetEventDirectorArray();

	//	for (AB2StageEventDirector* CurrSED : AllSEDs)
	//	{
	//		if (CurrSED && CurrSED != this && CurrSED->IsOneOfDependentDirectorActor(this))
	//		{
	//			CurrSED->NotifyShowOverFromDependentDirectorActor();
	//		}
	//	}
	//}
}

void AB2StageEventDirector::RestorePlayActorForCtrlMatineeEnd(FPlayActorShowSettings& InSettingForThis, FPlayActorShowRuntimeState& InStateForThis)
{
	//ABladeIICharacter* ThisPlayActor = InStateForThis.GetPlayActor(this);
	//if (ThisPlayActor)
	//{
	//	// SetupSingleShowMatineeGroup, EnsureHidePlayActorsForControlledMatinee 殿俊辑 见败初篮 芭 汗备.
	//	if (ThisPlayActor->bIsDying && InSettingForThis.IsDirectedMode())
	//	{ // 窜, 付萍聪肺 磷绰 楷免捞 唱坷绰 局绰 救焊捞霸 粱 贸府秦 林绢具 茄促.
	//		
	//		// 傲 Destroy 肺 力芭秦 滚府搁 埃窜茄单 捞芭 捞饶俊 SpawnPool 俊辑 贸府秦具 窍绰 惑炔捞 唱棵 荐 乐绢辑.. 粱 困氰窃. 局檬俊 Destroy 扼搁 扁夯利牢 风飘 捞寇俊绰 炼缴秦具摆促.
	//		
	//		ThisPlayActor->SetHideFromEventDirector(true);
	//		if (ThisPlayActor->GetMesh()){				
	//			ThisPlayActor->GetMesh()->GlobalAnimRateScale = 0.0f; // 捞犯霸 窍搁 局聪皋捞记阑 肯傈洒 吝窜矫懦 荐 乐促.
	//			// 辟单 捞犯霸 窍搁 OnAnimMobDeadEnd 肺 啊瘤 臼霸 登绢 Destroy 啊 救等促. 酒付 捞霸 唱坷绰 惑炔篮 胶抛捞瘤 辆丰 惑炔老 芭扼 绢痘霸 窍电 奴 巩力绰 绝阑 芭 鞍篮单.. 官肺 Destroy 父 窍瘤 臼绰 吧肺..
	//			ThisPlayActor->SetLifeSpan(1.0f);
	//		}

	//		// 促弗 规过栏肺绰 LobbySkeletalMeshActor 率俊 茄 巴 贸烦 AnimNotify 父 力芭窍绰 规过捞 乐澜. 啊厘 盔父茄 秦搬氓捞 瞪 扒单 备泅阑 粱 秦 林变 秦具..
	//					
	//		InStateForThis.SetPlayActor(NULL);
	//	}
	//	else
	//	{
	//		ABladeIIPlayer* CastedPlayerPlayActor = Cast<ABladeIIPlayer>(ThisPlayActor);
	//		if (CastedPlayerPlayActor) 
	//		{
	//			if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
	//			{ // 冀记 捍钦 葛胆阑 荤侩窍绰 葛靛俊辑绰 楷免 场唱绊 促矫陛 冀记 捍钦等 滚傈栏肺 倒妨初绰促.
	//				CastedPlayerPlayActor->SetUseSectionMergedMesh(true);
	//			}

	//			CastedPlayerPlayActor->SetupLODInfoForInGame(); // 肺拿 敲饭捞绢父 酒聪扼 匙飘况农 敲饭捞绢唱 怕弊 敲饭捞绢档 乐栏聪 促 LODInfo 府悸
	//		}

	//		ThisPlayActor->SetHideFromEventDirector(false);
	//		ThisPlayActor->RestoreTemporaryMeshComponentUpdateFlag(); // NeedSpawnCheck 秦力窍搁辑 TickPose 碍力沁带 芭 秦力. 己瓷惑.
	//	}

	//	if (InSettingForThis.EndActionPlayActorAdjustType == EEndActionPlayActorAdjustType::EAPAAT_AdjustToPuppet && InStateForThis.CtrlMatineePuppet) // bPutPlayActorBackToOriginalLocation 捞 酒聪鄂 娟扁绰 Matinee 辆丰 困摹肺 教农
	//	{
	//		const float B2CharHeightOffset = ZOffsetFromMatchingSKActorToBIICharacter(ThisPlayActor);

	//		ThisPlayActor->SetActorLocation(InStateForThis.CtrlMatineePuppet->GetActorLocation() + FVector(0.0f, 0.0f, B2CharHeightOffset));
	//		if (Cast<ABladeIIPlayer>(ThisPlayActor))
	//		{ // 敲饭捞绢 某腐磐扼搁 雀傈蔼篮 yaw 父 啊廉柯促.
	//			ThisPlayActor->SetActorRotation(FRotator(0.0f, InStateForThis.CtrlMatineePuppet->GetActorRotation().Yaw, 0.0f));
	//		}
	//		else
	//		{ // 老馆 各捞扼搁 roll 捞唱 pitch 档 楷免俊辑 楷加利栏肺 non-zero 蔼阑 镜 老捞 乐阑瘤档?
	//			ThisPlayActor->SetActorRotation(InStateForThis.CtrlMatineePuppet->GetActorRotation());
	//		}
	//	}
	//}

	//// Matinee 甫 困秦 甘俊 硅摹等 局甸肺辑 焊捞瘤父 臼霸 狄. 力芭绰 窍瘤 富绊.
	//// 父老 娟甸阑 力芭窍妨搁 Matinee 率俊辑 弥家茄 TermInterp 啊 贸府等 促澜捞绢具 茄促. 扁夯利牢 荤侩俊辑 ALevelSequenceActor ::Stop 俊辑 捞率栏肺狼 notify 啊 柯 饶俊 TermInterp 啊 贸府登骨肺 力芭窍绊 酵歹扼档 捞 矫痢俊辑绰 救凳
	//// -> MatineePuppet (溜 弊缝咀磐) 甸阑 犁劝侩 窍扁 困秦 力芭窍瘤 臼绰促. 犁劝侩阑 啊瓷窍霸 窍扁 困茄 贸府啊 ABladeIICharacter::DoesMatineePuppetSetupFor** 甸烙.
	//if (InStateForThis.CtrlMatineePuppet)
	//{
	//	if (InSettingForThis.PlayActorStateMode != EPlayActorStateMode::EPASM_DirectedAfterFinish)
	//	{
	//		if (ThisPlayActor)
	//		{ // 扼捞飘 波林绊 扁鸥 PlayActor 俊 蝶扼 Setup 沁带 芭 clear			
	//			ThisPlayActor->SetVisibilityMatineePuppetPointLight(InStateForThis.CtrlMatineePuppet, false);
	//			ThisPlayActor->ClearControlledMatineePuppet(InStateForThis.CtrlMatineePuppet);
	//		}

	//		SetVisibilityOfMatineePuppet(InStateForThis.CtrlMatineePuppet, InStateForThis.GetPlayActor(this), false);

	//		InStateForThis.CtrlMatineePuppet->SetActorTickEnabled(false);
	//		InStateForThis.CtrlMatineePuppet->GetSkeletalMeshComponent()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	//		InStateForThis.CtrlMatineePuppet = NULL;
	//	}
	//	else
	//	{
	//		if (InStateForThis.CtrlMatineePuppet->GetSkeletalMeshComponent()->AnimationData.AnimToPlay)
	//		{
	//			InStateForThis.CtrlMatineePuppet->GetSkeletalMeshComponent()->PlayAnimation(
	//				InStateForThis.CtrlMatineePuppet->GetSkeletalMeshComponent()->AnimationData.AnimToPlay, true);
	//		}
	//	}		
	//}
}

void AB2StageEventDirector::OnSingleStageEventShowOver(int32 ActorShowIndex)
{
	// Not probably for the Controlled Matinee mode.

	if (ActorShowIndex >= 0 && ActorShowIndex < ShowStates.Num())
	{
		FPlayActorShowRuntimeState& CurrShowState = ShowStates[ActorShowIndex];
		CurrShowState.bShowFinished = true;
	}

	CheckShowEndCountDown(); // 傈眉利牢 辆丰 炼扒 八荤.
}

void AB2StageEventDirector::OnControlledMatineeStop(bool bStandByNoti)
{
	//bControlledMatineeFinished = true;
	//CheckShowEndCountDown_ControlledMatinee(bStandByNoti); // 眠啊 八荤且 炼扒捞 乐澜.
	//if (!GIsEditor)
	//{ // 夸 SetCloseBattleMainUIClass 捞亥飘甫 俊叼磐俊辑 Init-Destroy 矫俊 Subscribe/Unsubcribe 窍绰单 俊叼磐俊辑 Unsubscribe 啊 狐瘤扁 浆框. ばば
	//	SetCloseBattleMainUIClass<bool>::GetInstance().Signal(true);
	//}
}

void AB2StageEventDirector::OnExportedCameraAnimStop()
{
	bCameraAnimFinished = true;
	CheckShowEndCountDown(); // 傈眉利牢 辆丰 炼扒 八荤.
}

void AB2StageEventDirector::OnAllManualCameraOver()
{
	bAllManualCameraEnded = true;
	CheckShowEndCountDown(); // 傈眉利牢 辆丰 炼扒 八荤.
}

void AB2StageEventDirector::ReturnToGamePlayCamera(bool bDeactivateOtherCams /*= false*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::ReturnToGamePlayCamera"));

	//ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
	//ABladeIIPlayerController* LocalB2PC = GetB2LocalPlayerCon();

	//// 咯扁辑 敲饭捞绢 墨皋扼 率栏肺 blending out 阑 力措肺 冈捞妨搁 DeactivateAllManualCams 甫 秦急 救凳. 
	//// 弊矾搁 捞率俊辑 active 茄 camera component 啊 绝绢辑 AActor::CalcCamera 俊辑 窜鉴洒 咀磐 困摹甫 荤侩窍霸 瞪 巴.
	//if (bDeactivateOtherCams)
	//{
	//	DeactivateAllManualCams();
	//}

	//CurrentActiveCamera = NULL;

	//// 付萍聪 荤侩 矫狼 墨皋扼 汗蓖 备泅篮 促甫 荐 乐阑... 鳖?

	//if (LocalB2Player)
	//	LocalB2Player->TopDownCameraComponent->Activate();

	//if (ActiveCameraForceOnFinish)
	//{// 康开 救捞 酒聪绢档 利侩. 捞繁 荤侩捞 鞘夸窍霸 瞪 荐档..
	//	// 弊犯促绊 秦辑 康开 救俊 促弗 墨皋扼啊 乐绰单 ActiveCameraForceOnFinish 肺 碍力窍扁 困茄 格利篮 酒聪促. 措眉肺 康开栏肺 利侩窍扁 帮鄂且父茄 版快俊 犬角窍霸 利侩窍扁 困茄 格利.
	//	ActiveCameraForceOnFinish->ForceBeginActiveCamera(FinalCameraRestoreBlendTime); 
	//}
	//else
	//{
	//	// ActiveCameraForceOnFinish 焊促绰 咯扁肺 坷绰 霸 措眉肺 歹 官恩流窍绊 磊楷(?)胶反促.
	//	// 咯扁辑档 FinalCameraRestoreBlendTime 捞 冈洒档废 ApplyActiveCameraIfAny 俊 blend time 坷滚扼捞爹 啊瓷窍档废 窃. 楷免俊辑 ActiveCamera 肺 倒酒哎 锭狼 喉坊爹篮 楷免率 汲沥栏肺.
	//	ApplyNormallyExpectedInGameCamera(GetB2GameMode(), LocalB2PC, FinalCameraRestoreBlendTime);
	//}

	//if (LocalB2PC && LocalB2PC->PlayerCameraManager)
	//{
	//	//碍力肺 墨皋扼 诀单捞飘 -> 荤角 APlayerCameraManager::SetViewTarget()阑 龋免窍霸 登搁,
	//	//APlayerCameraManager::TransformComponent狼 AttachedChild 困摹啊 救嘎绰 巩力啊 乐绢辑(墨皋扼困摹尔 TransformComponent 困摹啊 撇绢咙.) APlayerCameraManager::SetViewTarget()俊辑 窍妨绊 沁绰单
	//	//绢恫 荤捞靛捞棋飘啊 积辨瘤 隔扼辑 捞镑俊 笼绢持绰促.
	//	LocalB2PC->PlayerCameraManager->UpdateCamera(0.f);
	//}
}

void AB2StageEventDirector::DeactivateAllManualCams()
{
	for (int32 CI = 0; CI < MAX_DIRECTOR_CAMERA; ++CI)
	{
		DirectorCamArrayRef[CI]->Deactivate();
	}

	// TODO 促弗 咀磐率俊 汲摹且 墨皋扼甸档 -> 绝阑 芭 鞍促.
}

void AB2StageEventDirector::BeginTargetToManualCamera(int32 CamSettingIndex)
{
	//ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
	//ABladeIIPlayerController* LocalB2PC = GetB2LocalPlayerCon();
	//check(LocalB2PC && LocalB2Player && LocalB2Player->IsValidObj());

	//bool bNoMoreCameraWork = false;

	//if (CamSettingIndex >= 0 && CamSettingIndex < ManualCameraSettings.Num())
	//{
	//	FManualCameraSingleSetting& CurrCamSetting = ManualCameraSettings[CamSettingIndex];
	//	UCameraComponent* NextCamera = GetManualCameraFromSetting(CurrCamSetting);

	//	if (NextCamera) // 父老 NextCamera 甫 茫瘤 给窍搁 辆丰啊 登扁 傈鳖瘤绰 泅惑怕啊 蜡瘤瞪 巴.
	//	{
	//		if (CurrentActiveCamera && CurrentActiveCamera != LocalB2Player->TopDownCameraComponent)
	//		{
	//			// 捞吧 Deactivate 秦具 窍绰 惑炔篮 鞍篮 actor 救狼 促弗 墨皋扼 哪器惩飘甸 埃俊 喉坊爹 窍绰 版快烙.
	//			// 蝶扼辑 咯扁 甸绢坷绰 炼扒俊 CurrentActiveCamera 啊 敲饭捞绢狼 TopDownCamera 扼搁 deactivate 甫 窍瘤 臼绰促.
	//			// 促弗 版快啊 肚 乐促搁 眠啊 八荤甫 窍带瘤, outer 八荤甫 窍带瘤 窍绰 侥栏肺..
	//			CurrentActiveCamera->Deactivate();
	//		}
	//		CurrentActiveCamera = NextCamera;
	//		CurrentActiveCamera->Activate();
	//		LocalB2PC->SetViewTargetWithBlend(this, CurrCamSetting.BlendTimeFromPrev, CurrCamSetting.BlendFunction);

	//		// BlendTime 俊 嘎苗辑 ManualCameraBlendCB 鸥捞赣 汲摹.
	//		// SetViewTargetWithBlend 俊 措茄 肯丰 妮归捞 乐绰瘤 葛福摆绰单.. 矫埃 嘎苗辑 鸥捞赣 技泼窃. (SetViewTargetWithBlend 俊 措茄 妮归捞 乐绰 芭扼搁 还窿 ぇぇ) 
	//		if (CurrCamSetting.BlendTimeFromPrev > 0.0f)
	//		{
	//			GetWorldTimerManager().SetTimer(ManualCameraBlendTimerHandle, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::ManualCameraBlendCB, CamSettingIndex), CurrCamSetting.BlendTimeFromPrev, false);
	//		}
	//		else
	//		{
	//			ManualCameraBlendCB(CamSettingIndex); // 啊鞭利 捞犯霸 静绊 酵瘤绰 臼疽瘤父..
	//		}
	//	}
	//	else
	//	{
	//		bNoMoreCameraWork = true;
	//	}
	//}
	//else
	//{
	//	bNoMoreCameraWork = true;
	//}

	//if (bNoMoreCameraWork)
	//{
	//	// bStandbyAtTheEnd 惑包绝捞 脚龋甫 焊辰促. CheckShowEndCountDown 俊辑 bStandbyAtTheEnd 俊 蝶弗 贸府甫 且 巴.
	//	OnAllManualCameraOver();
	//}
}

void AB2StageEventDirector::OverallStartDelayTimerCB()
{
	GetWorldTimerManager().ClearTimer(OverallStartDelayTimerHandle);
	BeginShow();
}

void AB2StageEventDirector::OverallLengthTimerCB()
{
	GetWorldTimerManager().ClearTimer(OverallLengthTimerHandle);
	CustomFinishShow(); // 碍力 辆丰.
}

void AB2StageEventDirector::ManualCameraBlendCB(int32 CamSettingIndex)
{
	GetWorldTimerManager().ClearTimer(ManualCameraBlendTimerHandle);

	if (CamSettingIndex >= 0 && CamSettingIndex < ManualCameraSettings.Num())
	{
		FManualCameraSingleSetting& CurrCamSetting = ManualCameraSettings[CamSettingIndex];

		if (CurrCamSetting.StayTime > 0.0f)
		{
			// 捞 版快绰 牢郸胶甫 蜡瘤茄 盲 粱 歹 赣巩促.
			GetWorldTimerManager().SetTimer(ManualCameraBlendTimerHandle, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::ManualCameraBlendStayCB, CamSettingIndex), CurrCamSetting.StayTime, false);
		}
		else
		{
			// 促澜 墨皋扼肺. 父老 促澜 墨皋扼啊 绝促搁 墨皋扼 况农绰 辆丰 贸府啊 瞪 巴.
			BeginTargetToManualCamera(CamSettingIndex + 1);
		}
	}
}

void AB2StageEventDirector::ManualCameraBlendStayCB(int32 CamSettingIndex)
{
	GetWorldTimerManager().ClearTimer(ManualCameraBlendTimerHandle);

	// 促澜 墨皋扼肺. 父老 促澜 墨皋扼啊 绝促搁 墨皋扼 况农绰 辆丰 贸府啊 瞪 巴.
	BeginTargetToManualCamera(CamSettingIndex + 1);
}
//
//void AB2StageEventDirector::SetSESPlayActorMovementFromCamera(FSimpleEventSceneMoveKeyData& InMoveKey, class ABladeIICharacter* InPlayActor)
//{
//	if (InPlayActor && CameraAnimApplyCamIndex >= 0 && CameraAnimApplyCamIndex < MAX_DIRECTOR_CAMERA)
//	{
//		// UB2SimpleEventScene::CreateFromInterpTrack 曼绊
//
//		UCameraComponent* BaseCamComp = DirectorCamArrayRef[CameraAnimApplyCamIndex];
//		
//		// SimpleEventScene 狼 虐蔼阑 CameraAnim 捞 利侩登绰 墨皋扼甫 扁霖栏肺 茄 惑措谅钎肺 埃林.
//		// 窜, SimpleEventScene 阑 嘛阑 锭绰 乔哈捞 窍窜俊 乐绰 SkeletalMeshActor 甫 荤侩沁阑 巴捞骨肺 母蕉 哪器惩飘狼 例馆父怒 棵妨霖促.
//		float B2CharHeightOffset = ZOffsetFromMatchingSKActorToBIICharacter(InPlayActor);
//		const FTransform MutlpliedTransform = 
//			FTransform(InMoveKey.RotKey, InMoveKey.PosKey + FVector(0.0f, 0.0f, B2CharHeightOffset)) * BaseCamComp->GetComponentTransform();
//		
//		InPlayActor->SetActorLocation(MutlpliedTransform.GetTranslation());
//		InPlayActor->SetActorRotation(MutlpliedTransform.GetRotation());
//	}
//}
//
//void AB2StageEventDirector::SetSESCameraMovementFromPlayActor(FSimpleEventSceneMoveKeyData& InInvMoveKey, class ABladeIICharacter* InPlayActor)
//{
//	if (InPlayActor && CameraAnimApplyCamIndex >= 0 && CameraAnimApplyCamIndex < MAX_DIRECTOR_CAMERA)
//	{
//		// UB2SimpleEventScene::CreateFromInterpTrack 曼绊
//
//		UCameraComponent* BaseCamComp = DirectorCamArrayRef[CameraAnimApplyCamIndex];
//
//		// SetSESPlayActorMovementFromCamera 客绰 馆措肺 InPlayActor 甫 扁霖栏肺 墨皋扼 困摹 技泼
//		// 母蕉 哪器惩飘 offset 篮 付蛮啊瘤肺 皑救秦具 窃.
//		float B2CharHeightOffset = ZOffsetFromMatchingSKActorToBIICharacter(InPlayActor);
//		const FTransform MutlpliedTransform =
//			FTransform(InInvMoveKey.RotKey, InInvMoveKey.PosKey - FVector(0.0f, 0.0f, B2CharHeightOffset)) * InPlayActor->GetTransform();
//
//		BaseCamComp->SetWorldLocation(MutlpliedTransform.GetTranslation());
//		BaseCamComp->SetWorldRotation(MutlpliedTransform.GetRotation());
//	}
//}

bool AB2StageEventDirector::IsExportedEventSceneValid(class UB2SimpleEventScene* InEventScene)
{
	//return (InEventScene != NULL && InEventScene->SimpleMoveKeyFrames.Num() > 0 && InEventScene->InvSimpleMoveKeyFrames.Num() > 0);
	return true;
}

EStageEventShowState AB2StageEventDirector::GetDefaultDesiredPlayActorSESState(EStageEvent InEvent, EStageEventPlayActor InPlayActorType)
{
	//// 咯扁辑 return 窍绰 扒 老馆利牢 荤侩俊辑 抗惑登绰 蔼捞绊 StageEventStateOverride 啊 ESES_None 捞 酒聪搁 坷滚扼捞爹 瞪 巴.

	//if (InEvent == EStageEvent::ESEV_Begin)
	//{
	//	return EStageEventShowState::ESES_Appearing; // PlayActor 辆幅俊 惑包绝捞 傍烹老 淀
	//}
	//else if (InEvent == EStageEvent::ESEV_MobAppear)
	//{
	//	if (InPlayActorType == EStageEventPlayActor::ESEE_WaveMob)
	//	{
	//		return EStageEventShowState::ESES_Appearing;
	//	}
	//	else if (IsPlayActorTypeForPlayerCharacter(InPlayActorType))
	//	{
	//		return EStageEventShowState::ESES_Extra; // 捞 版快 敲饭捞绢啊 焊捞瘤 臼歹扼档 促弗 悼累栏肺 墨皋扼 局聪皋捞记阑 噶模促芭唱 窍瘤 臼霸.. Extra 牢 技泼篮 蝶肺 辆丰眉农甫 窍瘤 臼绰促. 促弗 霸 场唱搁 鞍捞 场巢.
	//	}
	//}
	//else if (InEvent == EStageEvent::ESEV_MobDying)
	//{
	//	if (InPlayActorType == EStageEventPlayActor::ESEE_WaveMob)
	//	{
	//		return EStageEventShowState::ESES_Dead; // MobDying 捞扼绊 秦档 促弗 Setting 狼 各捞 磷绰 巴老 荐 乐促. 弊繁 版快 StageEventStateOverride 肺 流立 state 甫 瘤沥.
	//	}
	//	else if (IsPlayActorTypeForPlayerCharacter(InPlayActorType))
	//	{
	//		return EStageEventShowState::ESES_Extra; // 付蛮啊瘤肺 捞 版快 敲饭捞绢啊 焊捞瘤 臼歹扼档 促弗 悼累栏肺 墨皋扼 局聪皋捞记阑 噶模促芭唱 窍瘤 臼霸.
	//	}
	//}
	//else if (InEvent == EStageEvent::ESEV_Clear)
	//{
	//	if (InPlayActorType == EStageEventPlayActor::ESEE_MainPlayer || InPlayActorType == EStageEventPlayActor::ESEE_CurrentPlayer || InPlayActorType == EStageEventPlayActor::ESEE_OtherPlayer)
	//	{
	//		return EStageEventShowState::ESES_VictoryMain;
	//	}
	//	else if (InPlayActorType == EStageEventPlayActor::ESEE_SubPlayer)
	//	{
	//		return EStageEventShowState::ESES_VictorySub;
	//	}
	//	else if (InPlayActorType == EStageEventPlayActor::ESEE_WaveMob)
	//	{
	//		return EStageEventShowState::ESES_Dead; // 捞繁 版快啊 抗惑登瘤绰 臼瘤父..
	//	}
	//}

	//// StageEvent 唱 PlayActor 鸥涝俊 蝶扼 眠啊..

	return EStageEventShowState::ESES_None;
}

void AB2StageEventDirector::SyncDataToUIDoc(ABladeIICharacter* InEventTriggeringMob)
{// UI 俊辑 镜 单捞磐甸 技泼.
	//BII_CHECK(!InEventTriggeringMob || InEventTriggeringMob->IsValidObj());
	//UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();	
	//if (DocBS && InEventTriggeringMob && InEventTriggeringMob->IsValidObj())
	//{
	//	UB2NPCSingleClassInfo* EventTriggerMobInfo = InEventTriggeringMob->GetMyNPCClassInfo();

	//	DocBS->SetCurAppearedMobName(EventTriggerMobInfo ? EventTriggerMobInfo->GetLocalizedName() : FText::FromString(TEXT("")));
	//	DocBS->SetCurAppearedMobNickName(EventTriggerMobInfo ? EventTriggerMobInfo->GetLocalizedNickName() : FText::FromString(TEXT("")));

	//	// 扁鸥 鞘夸茄 巴甸 眠啊登搁.. 
	//}
}

bool AB2StageEventDirector::IsStageEventRelatedUI(FName InUI)
{
	return (InUI == UIFName::StageBegin || InUI == UIFName::StageClear || InUI == UIFName::StageResult
		|| InUI == UIFName::MobAppearBossArea || InUI == UIFName::MobAppearBossStage || InUI == UIFName::PVPBegin
		|| InUI == UIFName::PVPResult || InUI == UIFName::RaidResult || InUI == UIFName::GuildResult || InUI == UIFName::ControlIntroTeamInfo
		|| InUI == UIFName::GuildRound);
}

void AB2StageEventDirector::StartEventWidgetOrStartCountdown()
{
	// StartEventWidget 皋牢 拜.

	if (EventWidgetType > EStageEventWidgetType::None)
	{
		if (EventWidgetStartTime > 0.0f)
		{
			GetWorldTimerManager().SetTimer(StartEventWidgetTimerHandle, this, &AB2StageEventDirector::StartEventWidgetTimerCB, EventWidgetStartTime, false);
		}
		else
		{
			StartEventWidget(); // 官肺 矫累
		}
	}
}

void AB2StageEventDirector::StartEventWidget()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::StartEventWidget"));

	//switch (EventWidgetType)
	//{
	//	//
	//	// 咯扁辑 凯府绰 UI 甸篮 IsStageEventRelatedUI 率俊辑 return true 贸府秦 林档废.
	//	//

	//	case EStageEventWidgetType::StageBegin:	
	//		{
	//			if (CachedStageManager && CachedStageManager->GetIsShowTalkDialog())
	//			{
	//				// 措拳汲沥捞 乐栏搁 措拳 矫胶袍何磐 畴免窃.
	//				AB2StageGameMode* StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(this));
	//				if (StageGM && StageGM->GetActiveSpawnPool())
	//				{
	//					int32 GroupID = 0;
	//					MyRelevantUIName = UIFName::StageBegin; // MyRelevantUIName 阑 OpenUI 捞傈俊 技泼且 巴. 老何 漂荐纳捞胶甫 力寇窍搁 OpenComplete 捞亥飘啊 Open 苞 悼矫俊 朝酒棵 巴捞促.
	//					if(StageGM->GetActiveSpawnPool()->TryGetStartTalkSetting(GroupID))
	//						StageEventTalkStartClass<int32, bool>::GetInstance().Signal(GroupID, false);
	//					else 
	//						UB2UIManager::GetInstance()->OpenUI(UIFName::StageBegin);
	//				}
	//			}
	//			else
	//			{
	//				MyRelevantUIName = UIFName::StageBegin; // MyRelevantUIName 阑 OpenUI 捞傈俊 技泼且 巴. 老何 漂荐纳捞胶甫 力寇窍搁 OpenComplete 捞亥飘啊 Open 苞 悼矫俊 朝酒棵 巴捞促.
	//				UB2UIManager::GetInstance()->OpenUI(UIFName::StageBegin);
	//			}
	//		}
	//		break;
	//	case EStageEventWidgetType::StageClear:
	//		{
	//			// StageClear 父 粱 漂喊窍霸 秒鞭秦辑 UIScene 栏肺 埃促.
	//			// Stage 辆丰 UI 啊 笛肺 唱吹搁辑 咯扁辑绰 UIStageClear 措脚 UIStageResult 甫 楷促. UIStageClear 绰 距埃 促福霸 甸绢皑. AB2StageEventDirector::BeginShow, AB2StageManager::BeginStageClearScene_Phase2 曼绊.
	//			if (GetB2GameModeType(this) == EB2GameMode::Scenario)
	//			{					
	//				MyRelevantUIName = UIFName::StageResult;
	//				UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::StageResult);
	//			} 		
	//			else if (GetB2GameModeType(this) == EB2GameMode::CounterDungeon) //EStageEventWidgetType阑 蝶肺 眠啊且鳖..
	//			{
	//				MyRelevantUIName = UIFName::CounterAttackDGResult;
	//				UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::CounterAttackDGResult);
	//			}
	//			else if (GetB2GameModeType(this) == EB2GameMode::HeroTower)
	//			{
	//				MyRelevantUIName = UIFName::HeroTowerResult;
	//				//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::HeroTowerResult);

	//				auto* HerotowerResult = UB2UIManager::GetInstance()->GetUI<UB2UIHeroTowerResult>(UIFName::HeroTowerResult);
	//				if (HerotowerResult)
	//				{
	//					HerotowerResult->OnShowResult();
	//				}
	//			}
	//			else if (GetB2GameModeType(this) == EB2GameMode::Dimension)
	//			{
	//				MyRelevantUIName = UIFName::DimensionResult;
	//				UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::DimensionResult);
	//			}
	//		}
	//		
	//		break;
	//	case EStageEventWidgetType::MobAppearBossStage:		
	//		MyRelevantUIName = UIFName::MobAppearBossStage;
	//		UB2UIManager::GetInstance()->OpenUI(UIFName::MobAppearBossStage); 
	//		break;
	//	case EStageEventWidgetType::MobAppearBossArea:		
	//		MyRelevantUIName = UIFName::MobAppearBossArea;
	//		UB2UIManager::GetInstance()->OpenUI(UIFName::MobAppearBossArea); 
	//		break;
	//	case EStageEventWidgetType::PVPBegin:
	//	case EStageEventWidgetType::RaidBegin:
	//		{
	//			MyRelevantUIName = UIFName::PVPBegin;
	//			auto* UI = UB2UIManager::GetInstance()->OpenUI<UB2UIPVPBegin>(UIFName::PVPBegin);
	//			if (UI && this->MatineeActor && this->MatineeActor->MatineeData)
	//			{
	//				UI->SetFightFX_PlayTime(this->MatineeActor->MatineeData->InterpLength);
	//			}
	//		}
	//		break;

	//	case EStageEventWidgetType::GuildBegin:
	//		{
	//			MyRelevantUIName = UIFName::GuildRound;
	//			auto* UI = UB2UIManager::GetInstance()->OpenUI<UB2UIGuildRound>(UIFName::GuildRound);
	//			if (UI && this->MatineeActor && this->MatineeActor->MatineeData)
	//				UI->StartRoundUI(this->MatineeActor->MatineeData->InterpLength, true);
	//		}
	//		break;

	//	case EStageEventWidgetType::GuildFinishBattle:
	//		{
	//			MyRelevantUIName = UIFName::GuildRound;
	//			auto* UI = UB2UIManager::GetInstance()->OpenUI<UB2UIGuildRound>(UIFName::GuildRound);
	//			float TimeLength = 0.0f;
	//			if (bStandbyAtTheEnd)
	//			{
	//				TimeLength = SpecifiedOverallLength;
	//			}
	//			else
	//			{
	//				if (this->MatineeActor && this->MatineeActor->MatineeData)
	//					TimeLength = this->MatineeActor->MatineeData->InterpLength;
	//			}

	//			if (UI && this->MatineeActor && this->MatineeActor->MatineeData)
	//				UI->StartRoundUI(TimeLength, false);
	//		}
	//		break;

	//	case EStageEventWidgetType::PVPResult:
	//		MyRelevantUIName = UIFName::PVPResult; // 咯扁档.. 搬惫 铰府芒 UI 绰 UIScene 栏肺
	//		//[@AKI, 170613] 距 匙飘况农俊辑 搬苞甫 辑滚俊辑 罐瘤 臼篮 惑怕俊辑 搬苞拳搁 焊咯林瘤 臼霸 函版
	//		//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::PVPResult);
	//		break;
	//	case EStageEventWidgetType::RaidResult:
	//		MyRelevantUIName = UIFName::RaidResult;
	//		UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::RaidResult);
	//		break;
	//	case EStageEventWidgetType::GuildResult:
	//		MyRelevantUIName = UIFName::RaidResult;
	//		UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::GuildResult);
	//		break;
	//	case EStageEventWidgetType::ControlIntro:
	//		MyRelevantUIName = UIFName::ControlIntroTeamInfo;
	//		UB2UIManager::GetInstance()->OpenUI(UIFName::ControlIntroTeamInfo);
	//		break;
	//	case EStageEventWidgetType::None:
	//	default:
	//		MyRelevantUIName = NAME_None;
	//		break;
	//}

	//if (EventWidgetEndTime > 0.0f)
	//{
	//	if (EventWidgetEndTime > EventWidgetStartTime)
	//	{
	//		// 辆丰 鸥捞赣甫 汲摹窍瘤 臼篮 版快绰 FinishShow 俊辑 颇鲍
	//		GetWorldTimerManager().SetTimer(FinishEventWidgetTimerHandle, this, &AB2StageEventDirector::FinishEventWidgetTimerCB,
	//			EventWidgetEndTime - FMath::Max(EventWidgetStartTime, 0.0f), false);
	//	}
	//	else
	//	{
	//		// 捞 版快绰 官肺 力芭秦具 富捞 瞪 淀 ぱ.ぱ;
	//		FinishEventWidget();
	//	}
	//}
}

void AB2StageEventDirector::FinishEventWidget()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::FinishEventWidget"));

	//switch (EventWidgetType)
	//{
	//	case EStageEventWidgetType::StageBegin:				UB2UIManager::GetInstance()->CloseUI(UIFName::StageBegin); break;
	//	case EStageEventWidgetType::MobAppearBossStage:		UB2UIManager::GetInstance()->CloseUI(UIFName::MobAppearBossStage); break;
	//	case EStageEventWidgetType::MobAppearBossArea:		UB2UIManager::GetInstance()->CloseUI(UIFName::MobAppearBossArea); break;
	//	case EStageEventWidgetType::ControlIntro:			UB2UIManager::GetInstance()->CloseUI(UIFName::ControlIntroTeamInfo); break;
	//	case EStageEventWidgetType::PVPBegin:
	//	case EStageEventWidgetType::RaidBegin:
	//	case EStageEventWidgetType::GuildBegin:
	//		//Fight PSC肺 牢秦辑 咯扁辑 摧瘤 臼绰促.
	//		//UB2UIManager::GetInstance()->CloseUI(UIFName::PVPBegin); 
	//		break;
	//		// StageClear 父 粱 漂喊窍霸 秒鞭秦辑 UIScene 栏肺 埃促. 角龙利栏肺 StageClear 扼搁 甘捞 促矫 肺爹等促芭唱 且 巴捞骨肺 咯扁肺 坷瘤绰 臼阑 淀?
	//		// 夸巴档 UIScene 捞扼 None 栏肺..
	//	case EStageEventWidgetType::StageClear:
	//	case EStageEventWidgetType::PVPResult:				
	//	case EStageEventWidgetType::RaidResult:
	//	case EStageEventWidgetType::GuildResult:
	//		UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::None); 
	//		break;			
	//	case EStageEventWidgetType::None:
	//	default:
	//		break;
	//}
}

void AB2StageEventDirector::StartEventWidgetTimerCB()
{
	GetWorldTimerManager().ClearTimer(StartEventWidgetTimerHandle);
	StartEventWidget();
}

void AB2StageEventDirector::FinishEventWidgetTimerCB()
{
	GetWorldTimerManager().ClearTimer(FinishEventWidgetTimerHandle);
	FinishEventWidget();
}

bool AB2StageEventDirector::IsPlayActorTypeForPlayerCharacter(EStageEventPlayActor InType) const
{
	switch (InType)
	{
		case EStageEventPlayActor::ESEE_CurrentPlayer :
		case EStageEventPlayActor::ESEE_OtherPlayer :
		case EStageEventPlayActor::ESEE_MainPlayer :
		case EStageEventPlayActor::ESEE_SubPlayer :
		case EStageEventPlayActor::ESEE_RemoteOpponent_Current :
		case EStageEventPlayActor::ESEE_WinningPlayer_Current :
		case EStageEventPlayActor::ESEE_WinningPlayer_Other :
		case EStageEventPlayActor::ESEE_TeamAllyPlayer :
		case EStageEventPlayActor::ESEE_TeamEnemyPlayer :
		case EStageEventPlayActor::ESEE_TeamWinningPlayer :
		case EStageEventPlayActor::ESEE_RaidPlayer:
		case EStageEventPlayActor::ESEE_WinningRaidPlayer:
		case EStageEventPlayActor::ESEE_DefeatedRaidPlayer:
		case EStageEventPlayActor::ESEE_Tutorial_Gladiator:
		case EStageEventPlayActor::ESEE_Tutorial_Assassin:
		case EStageEventPlayActor::ESEE_Tutorial_Wizard:
		case EStageEventPlayActor::ESEE_Tutorial_Fighter:
			return true;
	}

	return false;
}

bool AB2StageEventDirector::IsPlayActorTypeTutorialFixedClass(EStageEventPlayActor InType) const
{
	return (InType == EStageEventPlayActor::ESEE_Tutorial_Gladiator || InType == EStageEventPlayActor::ESEE_Tutorial_Assassin ||
		InType == EStageEventPlayActor::ESEE_Tutorial_Wizard || InType == EStageEventPlayActor::ESEE_Tutorial_Fighter);
}
EPCClass AB2StageEventDirector::TutorialPlayActorTypeToPCClass(EStageEventPlayActor InType) const
{
	switch (InType)
	{
	case EStageEventPlayActor::ESEE_Tutorial_Gladiator: return EPCClass::EPC_Gladiator;
	case EStageEventPlayActor::ESEE_Tutorial_Assassin: return EPCClass::EPC_Assassin;
	case EStageEventPlayActor::ESEE_Tutorial_Wizard: return EPCClass::EPC_Wizard;
	case EStageEventPlayActor::ESEE_Tutorial_Fighter: return EPCClass::EPC_Fighter;
	}
	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
	return EPCClass::EPC_End;
}

bool AB2StageEventDirector::DoesThisSettingRequireManualPlacement(FPlayActorShowSettings& InSetting)
{
	// BeginSingleShow 俊辑 荤侩窍绰 惑炔苞 老摹秦具摆促..
	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera || OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
	{		
		if (
			InSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_BringPlayActor ||
			// ManualCamera 牢 版快 NoAdjust 父 酒聪搁 流立 技泼.
			(OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera && InSetting.PlayActorPlacement != EPlayActorPlacement::EPAP_NoAdjust)
			)
		{
			return true;
		}
	}

	return false;
}

bool AB2StageEventDirector::DoesThisSettingHaveNoFixedPlacement(FPlayActorShowSettings& InSetting)
{
	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//{
	//	return false;
	//}

	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera && InSetting.PlayActorPlacement != EPlayActorPlacement::EPAP_BringPlayActor &&
	//	IsExportedEventSceneValid(InSetting.ExportedEventScene) == false)
	//{
	//	// ExportedEventScene 技泼阑 鳖冈菌电瘤, 酒聪搁 弊成 墨皋扼况农父 眠免秦辑 荤侩窍扁 困秦 狼档茄 吧 荐档 乐澜.
	//	return true;
	//}

	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera && InSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_NoAdjust)
	//{
	//	// 埃窜茄 牢霸烙 惑炔 墨皋扼俊 捞繁 汲沥阑 荤侩秦 杭 荐档 乐摆促.. 各 殿累 流饶搁 芭狼 困摹啊 绊沥登绢 乐扁档 且抛绊..
	//	return true;
	//}

	return false;
}

void AB2StageEventDirector::NotifyStageEventShowOver(class ABladeIICharacter* PlayActorJustOver)
{
	// SESEndNotify 啊 荤侩登绰 惑炔俊辑父 狼固啊 乐澜. Controlled Matinee 葛靛俊辑绰 酒付 阂府瘤 臼阑 巴. (措脚 NotifyActorCustomEvent)

	if (bStandbyAtTheEnd && PlayActorJustOver)
	{
		// 捞 版快 绢瞒乔 场鳖瘤 措扁窍绊 乐绢具 窍聪 镜单绝捞 AnimBP 俊辑 墨皋扼 Anim 吝窜 鞍篮 芭 窍瘤 臼档废 Extra 肺 SES State 甫 蜡瘤茄促.
		// ControlledMatinee 狼 版快 拱沸 捞霸 冈洒瘤 臼阑 芭扼 弊成 肛苗 滚府绰单.. 构 规过 乐栏妨唱..
		PlayActorJustOver->SetStageEventShowState(EStageEventShowState::ESES_Extra, this);
	}
	else
	{
		for (int32 SI = 0; SI < ShowStates.Num(); ++SI)
		{
			if (ShowStates[SI].GetPlayActor(this) == PlayActorJustOver && ShowStates[SI].bShowFinished == false)
			{
				OnSingleStageEventShowOver(SI);
				// 老摹窍绰 霸 窍唱父 乐绢具 沥惑捞摆瘤父 趣 葛福聪 break 绝捞 促 倒绊 夯促.
			}
		}
	}
}

static bool CheckAndGetMatineeScenarioDialogEventName(FString& InOutEventString)
{ // ALevelSequenceActor ::NotifyEventTriggered 俊辑 焊辰 ScenarioDialog 捞亥飘 痹距俊 嘎绰瘤 眉农窍绰 巴. 
	//if (InOutEventString.RemoveFromStart(MATINEE_OPEN_SCENARIO_DIALOG_EVENT_PREFIX))
	//{
	//	return true;
	//}
	return false;
}
void AB2StageEventDirector::NotifyActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::NotifyActorCustomEvent"));

	//Super::NotifyActorCustomEvent(OptionalEventName, OptionalNotifyingObject);

	//if (OptionalEventName == MATINEE_STOP_ACTOR_CUSTOM_EVENT)
	//{
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee) // 捞霸 酒囱 版快扼搁.. 购啊 肋给等 巴老 淀
	//	{
	//		OnControlledMatineeStop(false);
	//	}
	//}
	//else if (OptionalEventName == MATINEE_DONE_STAY_ACTOR_CUSTOM_EVENT)
	//{
	//	// 规陛 厘搁 辆丰沁栏唱 捞 惑怕肺 stay.
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && bStandbyAtTheEnd)
	//	{
	//		OnControlledMatineeStop(true);
	//	}
	//}
	//else if (OptionalEventName == CAMERAANIM_STOP_ACTOR_CUSTOM_EVENT)
	//{
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera) // 付蛮啊瘤肺 捞霸 酒聪扼搁 购啊 肋给..
	//	{
	//		OnExportedCameraAnimStop();
	//	}
	//}
	//else if (OptionalEventName == CAMERAANIM_DONE_STAY_ACTOR_CUSTOM_EVENT)
	//{
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera && bStandbyAtTheEnd)
	//	{
	//		// 老窜 Stop notification 阑 焊辰促. CheckShowEndCountDown 俊辑 bStandbyAtTheEnd 俊 措茄 眠啊 眉农甫 且 巴.
	//		OnExportedCameraAnimStop();
	//	}
	//}
	//else
	//{	
	//	
	//	FString EventNameString = OptionalEventName.ToString();
	//	if (CheckAndGetMatineeScenarioDialogEventName(EventNameString))
	//	{ // EventNameString 俊辑 漂沥 prefix 啊 狐廉 乐阑 巴.
	//		OpenDialogClass<FName, bool>::GetInstance().Signal(FName(*EventNameString), false); // CinematicMode 俊 蝶弗 BattleUI 见扁扁啊 累悼且 巴捞骨肺 咯扁急 MaintainBattleUI 甫 脚版镜 绝促.
	//	}
	//}
}

void AB2StageEventDirector::NotifyShowOverFromDependentDirectorActor()
{
	if (HasValidDependentDirectorActorSetting()) // 捞霸 技泼捞 救登绢 乐栏搁 阂府瘤 臼酒具 窃.
	{
		if (bDependentDirectorActorMet == false) // 父俊 窍唱 吝汗 龋免 规瘤..?
		{
			//////////
			bDependentDirectorActorMet = true; // 促弗 镑俊辑绰 bDirectingEventConditionMet 阑 技泼窍绰单 咯扁父 捞吧 技泼茄促.
			//////////
			BeginShowOrBeginCountDown();
		}
	}
}

bool AB2StageEventDirector::IsOneOfDependentDirectorActor(class AB2StageEventDirector* CheckSED)
{
	for (AB2StageEventDirector* CurrDep : DependentDirectorActors)
	{
		if (CurrDep && CurrDep == CheckSED)
		{
			return true;
		}
	}
	return false;
}

void AB2StageEventDirector::LevelStreamingOnBeginFlushCB()
{
	UGameplayStatics::FlushLevelStreaming(this);
	GetWorldTimerManager().ClearTimer(LevelStreamingOnBeginFlushTH);
}
void AB2StageEventDirector::LevelStreamingOnEndFlushCB()
{
	UGameplayStatics::FlushLevelStreaming(this);
	GetWorldTimerManager().ClearTimer(LevelStreamingOnEndFlushTH);
}

void AB2StageEventDirector::OnStartStandbyAtTheEnd()
{
	// 咯扁肺 吭促搁 bStandbyAtTheEnd 荤侩窍绰 惑炔俊辑 角龙利牢 厘搁甸捞 规陛 场抄 惑怕
	if (bStandbyPause)
	{
		UGameplayStatics::SetGamePaused(this, true);
	}
}

bool AB2StageEventDirector::IsPlayerCharacterOneOfPlayActor()
{
	//ABladeIIPlayer* LocalPlayerCharacter = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));

	//if (LocalPlayerCharacter == NULL)
	//{
	//	//BII_CHECK(0); //TMGameMode俊辑 LocalPlayerCharacter绰 绝澜. SpectatorPawn烙.
	//	return false;
	//}

	//for (int32 EI = 0; EI < ShowStates.Num(); ++EI)
	//{
	//	if (ShowStates[EI].GetPlayActor(this) == LocalPlayerCharacter)
	//	{
	//		return true;
	//	}
	//}

	return false;
}

bool AB2StageEventDirector::ShouldMaintainPlayerCharacterState()
{
	//ABladeIIPlayer* LocalPlayerCharacter = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));

	//if (LocalPlayerCharacter == NULL)
	//{ // TMGameMode 俊急 绝阑 荐 乐促.
	//	return false;
	//}

	//for (int32 EI = 0; EI < ShowStates.Num(); ++EI)
	//{
	//	FPlayActorShowSettings& ThisSetting = ShowSettings[EI];
	//	FPlayActorShowRuntimeState& ThisState = ShowStates[EI];
	//	
	//	if (ThisState.GetPlayActor(this) == LocalPlayerCharacter && !ThisSetting.IsDirectedMode())
	//	{
	//		return true;
	//	}
	//}
	return false;
}

bool AB2StageEventDirector::ShouldStopAutoDuringShow() const
{
	// bDisableInput 捞 酒聪搁 购啊 敲饭捞狼 楷加老 啊瓷己捞 臭栏骨肺 Auto 甫 蜡瘤. 
	// 炼扒捞 粱 歹 refine 瞪 荐 乐促. 促弗 炼扒 八荤 郴瘤绰 漂沥 加己蔼俊 蝶扼辑 殿.
	return bDisableInput;
}

bool AB2StageEventDirector::ShouldApplyCinematicScalabilityDuringShow() const
{ // 咯扁辑 false 甫 府畔窍绰 惑炔篮 荤角惑 霸烙 敲饭捞狼 楷加栏肺 各捞唱 NPC 狼 漂沥 局聪皋捞记阑 敲饭捞窍扁 困窃老 巴.
	return (bDisableInput || bHideHud);
}

bool AB2StageEventDirector::ShouldBeInvincibleDuringShow() const
{
	return bDisableInput; // 咯扁档 付蛮啊瘤肺 涝仿 啊瓷茄 捞亥飘 纠捞扼搁 敲饭捞狼 楷加老 巴捞骨肺 公利 惑怕肺 甸绢啊瘤 臼霸..
}

bool AB2StageEventDirector::HasDirectedFromBeginningTrack()
{
	for (FPlayActorShowSettings& CurSetting : ShowSettings)
	{
		if (CurSetting.PlayActorStateMode == EPlayActorStateMode::EPASM_DirectedFromBeginning){
			return true;
		}
	}
	return false;
}

void AB2StageEventDirector::SetupPreRenderSKMeshActor(class ASkeletalMeshActor* InOutDefaultSKActorToSetup, TArray<ASkeletalMeshActor*>& OutNewCreatedSKActors)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::SetupPreRenderSKMeshActor"));

	//if (!InOutDefaultSKActorToSetup) { // DefaultSKActorToSetup 篮 GameMode 率俊辑 积己秦辑 霖促. 
	//	return;
	//}
	//if (OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee || !MatineeActor || !MatineeActor->MatineeData) {
	//	return;
	//}

	//// 咯扁辑绰 扁鸥 荤侩窍绰 ParticleSystemEffect 甸阑 避绢辑 哪器惩飘肺 积己. 逞绢柯 DefaultSKActorToSetup 俊 嘿咯 淋.
	//TArray<AActor*> AllGroupActors;
	//MatineeActor->GetControlledActors(AllGroupActors);
	//TMap<FName, UParticleSystem*> AllFoundPSList; // 度鞍篮 府家胶 吝汗 积己 救窍霸.. 咯扁俊 避绢葛澜.
	//for (int32 AI = 0; AI < AllGroupActors.Num(); ++AI)
	//{
	//	// 焊烹篮 饭骇俊 汲摹秦 敌 Emitter 咀磐甫 GroupActor 肺 秦辑 On/Off 秦 啊哥 荤侩窃.
	//	
	//	AEmitter* CastedEmitterActor = Cast<AEmitter>(AllGroupActors[AI]); 
	//	UParticleSystem* EmitterFxResouce = (CastedEmitterActor && CastedEmitterActor->GetParticleSystemComponent()) ? CastedEmitterActor->GetParticleSystemComponent()->Template : NULL;

	//	// EmitterActor 狼 visibility 甫 力绢窍妨搁 部捞扁档 奖绊 (咯扁辑 淖促啊 官肺 波林绰 巴档 酒聪绊 割 橇饭烙 捞饶俊 波林绰 扒单 弊矾促 焊搁 弊 货 矫累等 楷免俊辑 角力肺 难龙 荐档 乐绊.. 榜摹 酒悄)
	//	// 傲 度鞍篮 ParticleSystem 府家胶肺促 哪器惩飘 积己秦 淋.
	//	if (EmitterFxResouce)
	//	{
	//		FName ThisPSKey(*EmitterFxResouce->GetPathName());
	//		UParticleSystem** FoundOne = AllFoundPSList.Find(ThisPSKey);
	//		if (!FoundOne){
	//			AllFoundPSList.Add(ThisPSKey, EmitterFxResouce);
	//		}
	//	}
	//}

	//// AnimControl Track 栏肺 甸绢埃 局聪皋捞记甸狼 AnimNotify 甸档..
	//for (UInterpGroup* ThisGroup : MatineeActor->MatineeData->InterpGroups)
	//{ // Group > Track > AnimSeq > Notify 付备付备 甸绢啊绰焙 ば.ば
	//	if (!ThisGroup) {
	//		continue;
	//	}
	//	for (UInterpTrack* ThisTrack : ThisGroup->InterpTracks)
	//	{
	//		UInterpTrackAnimControl* AnimControlTrack = Cast<UInterpTrackAnimControl>(ThisTrack);
	//		if (!AnimControlTrack) {
	//			continue;
	//		}
	//		for (FAnimControlTrackKey& ThisAnimKey : AnimControlTrack->AnimSeqs)
	//		{
	//			UAnimSequence* ThisAnimSeq = ThisAnimKey.GetAnimSeq();
	//			if (!ThisAnimSeq) {
	//				continue;
	//			}
	//			// 付瘤阜 for 巩 ぇぇ
	//			for (FAnimNotifyEvent& NotifyEvt : ThisAnimSeq->Notifies)
	//			{
	//				UParticleSystem* FoundPS = CheckNotifyEventForParticleSystemUse(NotifyEvt);
	//				if (FoundPS)
	//				{ // 货肺款 芭搁 眠啊
	//					FName ThisPSKey(*FoundPS->GetPathName());
	//					UParticleSystem** FoundOne = AllFoundPSList.Find(ThisPSKey);
	//					if (!FoundOne){
	//						AllFoundPSList.Add(ThisPSKey, FoundPS);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//// 避绢葛篮 吧肺 pre-render 侩 哪器惩飘 积己
	//for (TMap<FName, UParticleSystem*>::TIterator FoundPSIt(AllFoundPSList); FoundPSIt; ++FoundPSIt)
	//{
	//	UParticleSystem* ThisPS = FoundPSIt.Value();
	//	UParticleSystemComponent* SpawnedFxComp = UB2PreRenderer::SpawnDummyPSCForPreRender(ThisPS, InOutDefaultSKActorToSetup->GetRootComponent());
	//}
	//
	//// SkeletalMeshActor 甸篮 ShowSettings 俊 楷搬登瘤 臼篮 局父 蝶肺 贸府秦 淋. ShowSetting 俊 楷搬等 局甸篮 BladeIIGameMode 率俊辑 PC, NPC 沥焊 避绢坷搁辑 Prerender 悸诀且 锭 贸府瞪 巴捞扁 锭巩.
	//TArray<AActor*> NotBoundActors;
	//GetMatineeGroupActorsNotBoundtoShowSetting(NotBoundActors);
	//UWorld* TheWorld = GetWorld();
	//for (AActor* ThisNotBound : NotBoundActors)
	//{
	//	ASkeletalMeshActor* CastedNotBoundSKActor = Cast<ASkeletalMeshActor>(ThisNotBound);
	//	if (CastedNotBoundSKActor && CastedNotBoundSKActor->GetSkeletalMeshComponent())
	//	{
	//		// 咯扁俊 汲摹等 SkeletalMeshActor 甫 静绰 霸 酒聪扼 货肺 积己茄促. Visibility 甫 喊档肺 力绢秦具 窍骨肺..
	//		ASkeletalMeshActor* NewCreated = UB2PreRenderer::SpawnPreRenderMeshCommon(TheWorld);
	//		if (NewCreated)
	//		{
	//			NewCreated->GetSkeletalMeshComponent()->SetSkeletalMesh(CastedNotBoundSKActor->GetSkeletalMeshComponent()->SkeletalMesh);
	//			OutNewCreatedSKActors.Add(NewCreated);
	//		}
	//	}
	//}
}

void AB2StageEventDirector::TemporaryDisableTracksForPreRender()
{
	// Pre-render 啊 倒酒啊绰 客吝俊 岿靛 鸥烙 胶纳老(slomoe)捞 利侩捞 等 盲肺 拳搁捞 鳖改霸 啊妨瘤绰单
	// 荤款靛 飘发 鞍篮 版快绰 slomo 狼 康氢阑 罐瘤 臼绰单促啊 (罐酒档 捞惑且 淀) 酒轿府 0 檬何磐 矫累捞 登搁 拳搁篮 啊妨柳 惑怕俊辑 荤款靛父 刚历 唱客 滚府绰 厚必捞 惯积窍霸 等促.
	// 捞甫 阜扁 困窃
	// UB2UITalkDialog 俊档 付蛮啊瘤 格利狼 贸府甫 窍看澜

	//if (!MatineeActor || !MatineeActor->MatineeData) {
	//	return;
	//}

	//for (UInterpGroup* ThisInterpGroup : MatineeActor->MatineeData->InterpGroups)
	//{
	//	if (ThisInterpGroup)
	//	{
	//		for (UInterpTrack* ThisTrack : ThisInterpGroup->InterpTracks)
	//		{
	//			// 老窜 措惑篮 sound track
	//			UInterpTrackSound* CastedSoundTrack = Cast<UInterpTrackSound>(ThisTrack);
	//			if (CastedSoundTrack && !CastedSoundTrack->IsDisabled()) // 促弗 捞蜡肺 disable 等 巴甸捞 乐阑 抛聪 弊扒 芭福绊
	//			{
	//				TracksToRestoreForPreRender.Add(CastedSoundTrack);
	//				CastedSoundTrack->EnableTrack(false);
	//			}
	//		}
	//	}
	//}
}
void AB2StageEventDirector::RestoreTracksAfterPreRender()
{
	//for (UInterpTrack* ThisSavedTrack : TracksToRestoreForPreRender)
	//{
	//	if (ThisSavedTrack) {
	//		ThisSavedTrack->EnableTrack(true);
	//	}
	//}
	//TracksToRestoreForPreRender.Empty();

	//ForceSetMatineePlayingPosScale(0.0f); // 梅何磐 促矫 矫累.
}
void AB2StageEventDirector::ForceSetMatineePlayingPosScale(float InPositionScale)
{
	//// Matinee 甫 荤侩窍绰 版快 漂沥 困摹 (0.0 ~ 1.0 胶纳老肺) 肺 碍力 捞悼窍绰单..
	//// 促家埃 漂荐 格利栏肺 父甸菌澜阑 疙矫..
	//checkSlow(InPositionScale == 0.0f || (GetB2GameMode() && GetB2GameMode()->IsInPreRenderPhase()));

	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor && MatineeActor->MatineeData)
	//{
	//	float WantedPos = FMath::Clamp(InPositionScale, 0.0f, 1.0f) * MatineeActor->MatineeData->InterpLength;
	//	MatineeActor->SetPosition(WantedPos, true);
	//}
}

void AB2StageEventDirector::LoadLazyLoadedMatineeRefAssets(const TArray<FString>& ExcludeGroupNameFilters)
{
	//if (MatineeActor)
	//{
	//	MatineeActor->LoadLazyLoadedAssets(ExcludeGroupNameFilters);
	//}
}

void AB2StageEventDirector::GetMatineeGroupActorsNotBoundtoShowSetting(TArray<AActor*>& OutGroupActors)
{
	//if (OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee || !MatineeActor || !MatineeActor->MatineeData){
	//	return;
	//}

	//TMap<FName, FName> AllPossibleGroupNameMap; // 傲 八祸阑 粱 秦具 秦辑 TMap 栏肺 备己窃.
	//for (FPlayActorShowSettings& ThisShowSetting : ShowSettings)
	//{
	//	TArray<FName> AllSupposedNameThis;
	//	// PC 狼 版快 辆幅喊肺 秦辑 窍唱狼 ShowSetting 捞 咯矾 Matinee Group 俊 楷搬登绢 乐阑 荐 乐栏聪 啊瓷茄 巴甸阑 葛滴 八荤.
	//	GetAllSupposedInterpGroupNamesForSetting(AllSupposedNameThis, ThisShowSetting);
	//	for (FName& ThisFoundName : AllSupposedNameThis)
	//	{
	//		AllPossibleGroupNameMap.Add(ThisFoundName, ThisFoundName);
	//	}
	//}

	//TArray<UInterpGroup*> AllFoundGroupNotBoundShowSetting; // 绢恫 ShowSetting 苞档 楷搬登瘤 臼篮 InterpGroup 格废
	//for (int32 GAI = 0; GAI < MatineeActor->MatineeData->InterpGroups.Num(); ++GAI)
	//{
	//	UInterpGroup* CurrGroup = MatineeActor->MatineeData->InterpGroups[GAI];
	//	if (!CurrGroup || AllPossibleGroupNameMap.Find(CurrGroup->GroupName)){
	//		continue; // 漂沥 ShowSettings 苞 楷搬等 弊缝篮 胶诺
	//	}
	//	// 咯扁鳖瘤 烹苞茄 InterpGroup俊辑 GroupActor 甸阑 惶酒柯促. GroupActorInfos 甫 烹秦 弊缝俊 楷搬等 咀磐甸阑 茫酒棵 荐 乐澜
	//	TArray<class AActor*> CurrGroupActors;
	//	for (int32 GAI2 = 0; GAI2 < MatineeActor->GroupActorInfos.Num(); ++GAI2)
	//	{
	//		if (MatineeActor->GroupActorInfos[GAI2].ObjectName == FName(*CurrGroup->GetName()))
	//		{
	//			OutGroupActors += MatineeActor->GroupActorInfos[GAI2].Actors;
	//			break;
	//		}
	//	}
	//}
}

void AB2StageEventDirector::GetAllExpectedDialogCodeNames(TArray<FName>& OutAllDlgCode)
{
	// Matinee Event 飘发俊 ScenarioDialogEvent 肺 殿废等 捞亥飘 捞抚甸阑 避绢坷绰 巴.
	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor && MatineeActor->MatineeData)
	//{
	//	for (UInterpGroup* ThisGroup : MatineeActor->MatineeData->InterpGroups)
	//	{
	//		for (UInterpTrack* ThisTrack : ThisGroup->InterpTracks)
	//		{
	//			UInterpTrackEvent* CastedEventTrack = Cast<UInterpTrackEvent>(ThisTrack);
	//			if (CastedEventTrack && CastedEventTrack->bB2ScenarioDialogEvent)
	//			{
	//				for (FEventTrackKey& ThisEventKey : CastedEventTrack->EventTrack)
	//				{
	//					// NotifyActorCustomEvent 甫 烹秦 棵 锭俊绰 ScenarioDialogEvent 侩档牢瘤 备盒窍扁 困茄 prefix 啊 嘿霸 登瘤父 
	//					// 弊成 捞 惑怕俊辑绰 EventName 捞 官肺 DialogCode 啊 凳.
	//					
	//					// 荤侩窍绰 惑炔俊辑 促弗 单俊辑 避绢柯 巴甸阑 茄单 钦磨 荐 乐栏骨肺 AddUnique 窃.
	//					OutAllDlgCode.AddUnique(ThisEventKey.EventName);
	//				}
	//			}
	//		}
	//	}
	//}
}

UActorComponent* AB2StageEventDirector::FindComponentByClass(const TSubclassOf<UActorComponent> ComponentClass) const
{
#if WITH_EDITOR
	//// 俊叼磐 轰器飘俊 累霸 唱坷绰 橇府轰 芒俊 急琶茄 墨皋扼 哪器惩飘 轰甫 焊捞扁 困窃. 扁夯 扁瓷栏肺绰 弊成 霉锅掳 巴父 唱咳.
	//if(GIsEditor && ComponentClass == UCameraComponent::StaticClass())
	//{
	//	TArray<UCameraComponent*> SelectedCameraComps;
	//	UCameraComponent* SelectedCameraComponent = NULL;

	//	// 急琶等 墨皋扼 哪器惩飘啊 乐绰瘤 八荤
	//	USelection* ComponentSelection = GEditor->GetSelectedComponents();
	//	if(ComponentSelection)
	//	{
	//		ComponentSelection->GetSelectedObjects(SelectedCameraComps);
	//	}
	//	if(SelectedCameraComps.Num() > 0)
	//	{
	//		SelectedCameraComponent = SelectedCameraComps[0];
	//	}

	//	// 盔贰 扁瓷俊辑绰 弊成 贸澜 巴阑 府畔窍绰单 急琶茄 巴苞 鞍篮 巴牢瘤 眠啊 八荤
	//	UActorComponent* FoundComponent = NULL;
	//	for (UActorComponent* Component : GetComponents())
	//	{
	//		if (Component && Component->IsA(ComponentClass) && Component == SelectedCameraComponent)
	//		{
	//			FoundComponent = Component;
	//			break;
	//		}
	//	}

	//	if(FoundComponent == NULL) // 弃归
	//	{
	//		return Super::FindComponentByClass(ComponentClass);
	//	}
	//	else
	//	{
	//		return FoundComponent;
	//	}
	//}
	//else
	{
		return Super::FindComponentByClass(ComponentClass);
	}
#else
	return Super::FindComponentByClass(ComponentClass);
#endif
}

UActorComponent* AB2StageEventDirector::FindComponentByName(FName& InComponentName) const
{
	for (UActorComponent* Component : GetComponents())
	{
		if (Component && Component->GetName() == InComponentName.ToString())
		{
			return Component;
		}
	}
	return NULL;
}

ABladeIIPlayer* AB2StageEventDirector::GetB2LocalPlayerChar()
{
	//return Cast<class ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this)); 
	return NULL;
}
ABladeIIPlayerController* AB2StageEventDirector::GetB2LocalPlayerCon()
{
	//return Cast<class ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this)); 
	return NULL;
}
ABladeIINetPlayer* AB2StageEventDirector::GetB2MatchOpponent()
{
	// 咯鸥 促弗 NetGameMode 俊辑狼 Opponent 啊 眠啊瞪 荐 乐澜.
	//AB2PVPGameMode* PVPGM = Cast<AB2PVPGameMode>(GetB2GameMode());
	//return PVPGM ? PVPGM->GetOpponent() : NULL;
	return NULL;
}

ABladeIIPlayer* AB2StageEventDirector::GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly, bool bSpawnPuppetForDead)
{
	//if (AB2AsyncGameMode* AsyncGM = Cast<AB2AsyncGameMode>(GetB2GameMode()))
	//	return AsyncGM->GetTeamPlayer(bAllyTeam, nPlayerIndex, bLiveOnly, bSpawnPuppetForDead); // bSpawnPuppetForDead 甫 true 肺 逞败林搁 磷篮 局档 楷免 侩栏肺 单妨棵 荐 乐促.

	return NULL;
}

ABladeIIPlayer* AB2StageEventDirector::GetRaidPlayer(int32 nPlayerIndex, bool bLiveOnly /*= false*/)
{
	//if (AB2RaidGameMode* RaidGM = Cast<AB2RaidGameMode>(GetB2GameMode()))
	//	return RaidGM->GetRaidPlayer(nPlayerIndex, bLiveOnly ? ERaidPlayerLiveState::LiveOnly : ERaidPlayerLiveState::All);

	return NULL;
}

ABladeIIPlayer* AB2StageEventDirector::GetShowHeroPlayerIfAny()
{
	//// 泅犁 汲沥俊 蝶扼 墨皋扼 窍捞扼捞飘甫 罐阑 父茄 敲饭捞绢 某腐磐甫 府畔窍绰 巴烙. 拱沸 弊霸 馆靛矫 焊厘登绰 扒 酒聪促.
	//if (DirectingEvent == EStageEvent::ESEV_MatchResult)
	//{
	//	if (AB2TMGameMode* TMGM = Cast<AB2TMGameMode>(GetB2GameMode()))
	//		return TMGM->GetTeamPlayer(bMatchResultEventAsWinner, 0, false, true); // 咯扁辑档 dead 牢 某腐磐甫 dummy spawn 秦具 窍唱..
	//	
	//	return bMatchResultEventAsWinner ? GetB2LocalPlayerChar() : GetB2MatchOpponent();		
	//}
	//else if (DirectingEvent == EStageEvent::ESEV_MatchResultOnlyWinning || DirectingEvent == EStageEvent::ESEV_MatchResultOnlyDefeat)
	//{
	//	return GetRaidPlayer(0);
	//}
	//else if (DirectingEvent == EStageEvent::ESEV_Clear || IsPossibleAdventureModeBeginEvent())
	//{
	//	return GetB2LocalPlayerChar();
	//}

	return GetB2LocalPlayerChar(); // 澜.. 搬惫篮 MobAppear 捞亥飘俊辑 鞘夸窍霸 登菌焙.
}

#if WITH_EDITOR

void AB2StageEventDirector::RefreshPreviewSKComps(bool bRegenerateSKComp /*= true*/)
{
	//// 扁粮俊 父电 巴甸 措何盒阑 力芭窍绊 货肺 积己.
	//DestroyPreviewComponents(bRegenerateSKComp);

	//UPackage* OutermostPkg = GetOutermost();
	//// 甘牢 版快父 积己秦具 窃. 喉风橇赴飘肺 历厘茄 局档 贸澜浚 甘栏肺 牢侥瞪 巴.
	//if(OutermostPkg == NULL || FEditorFileUtils::IsMapPackageAsset(OutermostPkg->GetName()) == false)
	//{
	//	return;
	//}

	//ActorNameDisplayText = NewObject<UTextRenderComponent>(this, TEXT("ActorNameDisplay"), RF_Transient);
	//if(ActorNameDisplayText)
	//{
	//	ActorNameDisplayText->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//	ActorNameDisplayText->RegisterComponent();
	//	ActorNameDisplayText->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	ActorNameDisplayText->SetVisibility(true);
	//	ActorNameDisplayText->SetHiddenInGame(true);
	//	ActorNameDisplayText->SetHorizontalAlignment(EHTA_Center);
	//	ActorNameDisplayText->SetWorldSize(35.0f);
	//	ActorNameDisplayText->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	//	ActorNameDisplayText->SetRelativeLocation(FVector(0.0f,0.0f,100.0f)); // 粱 臭咯辑
	//	ActorNameDisplayText->SetText(FText::FromString( this->GetName() ));
	//	ActorNameDisplayText->SetTextRenderColor(FColor(255,255,0));
	//}
	//
	//TArray<UActorComponent*> AllShapeComps = GetComponentsByClass(UShapeComponent::StaticClass());
	//for (int32 SCI = 0; SCI < AllShapeComps.Num(); ++SCI)
	//{
	//	UShapeComponent* CurrSC = Cast<UShapeComponent>(AllShapeComps[SCI]);
	//	UTextRenderComponent* NewInfoTextComp = NewObject<UTextRenderComponent>(this, *FString::Printf(TEXT("AnyShapeCompNameText_%d"), SCI), RF_Transient);
	//	if (CurrSC == NULL || NewInfoTextComp == NULL)
	//	{
	//		continue;
	//	}

	//	NewInfoTextComp->AttachToComponent(CurrSC, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//	NewInfoTextComp->RegisterComponent();
	//	NewInfoTextComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	NewInfoTextComp->SetVisibility(true);
	//	NewInfoTextComp->SetHiddenInGame(true);
	//	NewInfoTextComp->SetHorizontalAlignment(EHTA_Center);
	//	NewInfoTextComp->SetWorldSize(30.0f);
	//	NewInfoTextComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	//	NewInfoTextComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // 困摹绰 利寸洒 嘎苗霖促
	//	NewInfoTextComp->SetText(FText::FromString(CurrSC->GetName()));
	//	NewInfoTextComp->SetTextRenderColor(FColor(255, 0, 0));

	//	AnyShapeCompNameText.Add(NewInfoTextComp);
	//}

	//if(OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//{
	//	// Controlled Matinee 俊急 Preview 侩 SkeletalMeshComponent 甸阑 积己窍绰 措脚 措眉肺 捞固 硅摹等 SkeletalMeshActor 甸苞 包访茄 眠啊利牢 贸府..
	//	SetupPreviewForControlledMatinee();
	//	return;
	//}

	//// 捞窍绰 Controlled Matinee 葛靛俊辑绰 鞘夸绝绰 巴甸父 持绰促.

	//for(int32 SI = 0; SI < ShowSettings.Num(); ++SI)
	//{
	//	FPlayActorShowSettings& ThisSetting = ShowSettings[SI];
	//	FTransform PreviewCompTransform;
	//	USkeletalMesh* PreviewSKMesh = NULL;
	//	if(GetEssentialPreviewInfoForSetting(ThisSetting, NULL, PreviewSKMesh, PreviewCompTransform))
	//	{
	//		// 己傍沁栏搁 PreviewSKMesh 客 PreviewCompTransform 捞 技泼登菌阑 巴.

	//		// bRegenerateSKComp 啊 酒聪扼档 扁粮俊 绝绰 惑炔捞扼搁 货肺 积己茄促.
	//		USkeletalMeshComponent* NewSKComp = (bRegenerateSKComp == false && PreviewSKComps.Num() > SI) ?
	//			PreviewSKComps[SI] : NewObject<USkeletalMeshComponent>(this, *GetPreviewSKCompNameForSetting(SI), RF_Transient);

	//		if(NewSKComp)
	//		{
	//			NewSKComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//			NewSKComp->RegisterComponent();
	//			NewSKComp->SetVisibility(true);
	//			// 绢瞒乔 霸烙俊急 积扁瘤档 臼阑 局甸捞瘤父 
	//			NewSKComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//			NewSKComp->SetHiddenInGame(true);
	//				
	//			NewSKComp->SetSkeletalMesh(PreviewSKMesh);
	//			const float ZOffsetForCharacter = NewSKComp->Bounds.BoxExtent.Z;
	//			// SetWorldTransform 阑 窍扁 傈俊 SkeletalMesh 客 BladeIICharacter 埃 pivot 瞒捞肺 牢茄 offset 阑 馆康茄促. SyncFromPreviewSKCompPlacement 俊辑 窍绰 巴狼 馆措
	//			if(DoesThisSettingRequireManualPlacement(ThisSetting))
	//			{						
	//				PreviewCompTransform.SetTranslation(PreviewCompTransform.GetTranslation() + FVector(0.0f, 0.0f, -1.0f * ZOffsetForCharacter));
	//			}
	//			NewSKComp->SetWorldTransform(PreviewCompTransform);
	//			// 繁鸥烙 PlayActor 档 Snapping 阑 窍聪 娟档 snapping. 辟单 PostLoad 矫痢俊辑绰 酒流 肺爹登瘤 臼篮 岿靛 坷宏璃飘啊 乐绢辑 弊繁瘤 救冈塞.
	//			SnapSkeletalMeshComponentToGround(NewSKComp);

	//			if(DoesThisSettingRequireManualPlacement(ThisSetting))
	//			{
	//				const float FirstSnapZTranslation = NewSKComp->GetComponentLocation().Z - PreviewCompTransform.GetTranslation().Z;

	//				if(FMath::Abs(FirstSnapZTranslation) < KINDA_SMALL_NUMBER)
	//				{
	//					// 咯扁肺 吭促搁 官肺 菊狼 SnapSkeletalMeshComponentToGround 啊 救冈躯窜 娟变单
	//					// 菊辑 PreviewCompTransform 俊 ZOffsetForCharacter 甫 - 肺 啊沁栏骨肺 扁夯 困摹啊 撤栏搁 顶俊 颇汞躯阑 啊瓷己捞 蚕 乐促.
	//					// 牢霸烙俊辑绰 绢蠢 沥档鳖瘤 颇汞腮 扒 面倒 贸府肺 牢秦 困肺 棵扼坷霸 登瘤父 俊叼磐 累诀俊 去急阑 临 荐档 乐栏聪 困肺 距埃 棵妨辑 促矫 snap 阑 矫档.
	//					// 促父, 捞 锭俊 snap 傈饶 函拳啊 绝促搁 咯傈洒 顶俊 汞囚乐电瘤 窍绰 版快捞骨肺 捞傈 蔼栏肺 促矫 倒妨初澜.

	//					const FVector LocationBeforeAddtionalSnap = NewSKComp->GetComponentLocation(); // 唱吝俊 登倒副 锭 荤侩阑 困秦 归诀

	//					PreviewCompTransform.SetTranslation(PreviewCompTransform.GetTranslation() + FVector(0.0f, 0.0f, 1.0f * ZOffsetForCharacter));
	//					const FVector RaisedLocationBeforeSnap = PreviewCompTransform.GetTranslation();

	//					NewSKComp->SetWorldLocation(PreviewCompTransform.GetTranslation());
	//					SnapSkeletalMeshComponentToGround(NewSKComp); // 货肺 snap 等 location 俊 购啊 函拳啊 乐促搁 咯扁辑狼 困摹甫 蜡瘤窍霸 瞪 巴.

	//					const FVector RaisedAndSnappedLocation = NewSKComp->GetComponentLocation();

	//					if(FMath::Abs(RaisedAndSnappedLocation.Z - RaisedLocationBeforeSnap.Z) < KINDA_SMALL_NUMBER)
	//					{
	//						// 货肺 snap 阑 秦档 脚烹摹 臼栏聪 促矫 登倒赴促.
	//						NewSKComp->SetWorldLocation(LocationBeforeAddtionalSnap);
	//					}
	//				}
	//			}

	//			if(bRegenerateSKComp == true || PreviewSKComps.Num() <= SI) // 困俊辑 哪器惩飘甫 货肺 积己窍绰 炼扒.
	//			{
	//				PreviewSKComps.Add(NewSKComp);
	//			}

	//			// 橇府轰 comp 俊辑 困摹甫 开栏肺 sync 窍扁 困茄 甘俏 info.
	//			FSEDPreviewCompInfo NewMappingInfo;
	//			NewMappingInfo.SKComp = NewSKComp;
	//			NewMappingInfo.ShowSettingIndex = SI;
	//			PreviewMapping.Add(NewMappingInfo);
	//				
	//			// 救郴侩 咆胶飘. 开矫唱 B2SpawnPoolEditingActor 钎例
	//			UTextRenderComponent* NewInfoTextComp = NewObject<UTextRenderComponent>(this, *GetPreviewInfoTextCompNameForSetting(SI), RF_Transient);
	//			if(NewInfoTextComp)
	//			{
	//				NewInfoTextComp->AttachToComponent(NewSKComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//				NewInfoTextComp->RegisterComponent();
	//				NewInfoTextComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//				NewInfoTextComp->SetVisibility(true);
	//				NewInfoTextComp->SetHiddenInGame(true);
	//				NewInfoTextComp->SetHorizontalAlignment(EHTA_Center);
	//				NewInfoTextComp->SetWorldSize(20.0f);

	//				// SpawnPoolEditingActor 率俊急 农扁 蜡瘤窍妨绊 何葛 MeshComp 蝶扼 货肺 拌魂秦辑 RelativeScale 肺 冈看绰单 弊成 WorldScale 肺 冈捞搁 登绰 芭 酒囱啊? ぇぇ
	//				NewInfoTextComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	//				// Most SkeletalMesh assets have their root at their feet, so multiply the box extent by 2.
	//				// Relative location takes a value before scaling, so give InvMeshScale.
	//				const float TextRenderZOffset = (NewSKComp->Bounds.BoxExtent.Z * 2.0f); // 咯扁俊 NewSKComp 率 scale 鳖瘤 冈老 鞘夸啊 乐栏妨唱..
	//				FVector TextRelativeLocation = NewInfoTextComp->GetRelativeTransform().GetLocation();
	//				TextRelativeLocation.Z = TextRenderZOffset;
	//				NewInfoTextComp->SetRelativeLocation( TextRelativeLocation );
	//					
	//				FColor InfoTextRenderColor = FColor(0,255,255);

	//				// 付瘤阜栏肺 救郴巩备.
	//				// Directing event 俊 蝶弗 救郴 巩磊
	//				FString DirectingEventString;
	//				switch (DirectingEvent) // 咯扁辑 被捞 GetTrimmedNameFromPCClassEnum 鞍篮 窿阑 且 鞘夸鳖柳 绝摆瘤.
	//				{
	//				case EStageEvent::ESEV_Begin: DirectingEventString = TEXT("Begin"); break;
	//				case EStageEvent::ESEV_MobAppear: DirectingEventString = TEXT("MobAppear"); break;
	//				case EStageEvent::ESEV_MobDying: DirectingEventString = TEXT("MobDying"); break;
	//				case EStageEvent::ESEV_Clear: DirectingEventString = TEXT("Clear"); break;
	//				case EStageEvent::ESEV_Custom: DirectingEventString = TEXT("Custom"); break;
	//				}

	//				// PlayActor 俊 蝶弗 救郴巩磊
	//				FString PlayActorString;
	//				switch (ThisSetting.PlayActorType)
	//				{
	//				case EStageEventPlayActor::ESEE_CurrentPlayer: PlayActorString = TEXT("CurrentPlayer"); break;
	//				case EStageEventPlayActor::ESEE_OtherPlayer: PlayActorString = TEXT("OtherPlayer"); break;
	//				case EStageEventPlayActor::ESEE_MainPlayer: PlayActorString = TEXT("MainPlayer"); break;
	//				case EStageEventPlayActor::ESEE_SubPlayer: PlayActorString = TEXT("SubPlayer"); break;
	//				case EStageEventPlayActor::ESEE_WaveMob: {
	//					// 咯扁鳖瘤 柯 芭搁 蜡瓤茄 汲沥老 巴.
	//					int32 WaveNum = 0;
	//					int32 MobIndex = 0;
	//					ParseForWaveMobSelectKeyword(ThisSetting.WaveMobSelectKeyword, WaveNum, MobIndex);
	//					PlayActorString = FString::Printf(TEXT("WaveMob %d %d"), WaveNum, MobIndex); 
	//					}break;
	//				}

	//				FString InfoTextString = FString::Printf(TEXT("SE %s\n%d: %s"), *DirectingEventString, SI, *PlayActorString);

	//				if(DoesThisSettingHaveNoFixedPlacement(ThisSetting))
	//				{
	//					InfoTextString = TEXT("NOT FIXED!\n") + InfoTextString;
	//					InfoTextRenderColor = FColor(127,255,127); // B盲澄 措脚 R盲澄捞 甸绢艾栏聪 混娄 歹 灌扁绰 且 淀?
	//				}
	//				else if(DoesThisSettingRequireManualPlacement(ThisSetting) == false)
	//				{
	//					InfoTextString += TEXT("\nPreview only");
	//					InfoTextRenderColor = FColor(0,127,127); // 焊促 绢涤霸.
	//				}						

	//				NewInfoTextComp->SetText(FText::FromString(InfoTextString));
	//				NewInfoTextComp->SetTextRenderColor(InfoTextRenderColor);
	//				PreviewSKCompsInfoText.Add(NewInfoTextComp);
	//			}
	//		}
	//	}
	//}

	//// Director 墨皋扼 惑窜狼 锅龋档 谎妨焊磊.
	//for(int32 DI = 0; DI < MAX_DIRECTOR_CAMERA; ++DI)
	//{
	//	UCameraComponent* CurrDirectorCam = DirectorCamArrayRef[DI];

	//	UTextRenderComponent* NewInfoTextComp = NewObject<UTextRenderComponent>(this, *FString::Printf(TEXT("DirectorCamNumber_%d"), DI), RF_Transient);
	//	if(NewInfoTextComp)
	//	{
	//		NewInfoTextComp->AttachToComponent(CurrDirectorCam, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//		NewInfoTextComp->RegisterComponent();
	//		NewInfoTextComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//		NewInfoTextComp->SetVisibility(true);
	//		NewInfoTextComp->SetHiddenInGame(true);
	//		NewInfoTextComp->SetHorizontalAlignment(EHTA_Center);
	//		NewInfoTextComp->SetWorldSize(25.0f);
	//		NewInfoTextComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	//		NewInfoTextComp->SetRelativeLocation(FVector(0.0f,-10.0f,20.0f)); // 困摹绰 利寸洒 嘎苗霖促
	//		NewInfoTextComp->SetText(FText::FromString(FString::FromInt(DI)));
	//		NewInfoTextComp->SetTextRenderColor(FColor(255,0,0));
	//		DirectorCameraNumberText.Add(NewInfoTextComp);
	//	}
	//}

}

void AB2StageEventDirector::SetupPreviewForControlledMatinee()
{
	//if (OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee || MatineeActor == NULL || MatineeActor->MatineeData == NULL)
	//{
	//	return;
	//}

	//for (int32 SI = 0; SI < ShowSettings.Num(); ++SI)
	//{
	//	FPlayActorShowSettings& ThisSetting = ShowSettings[SI];
	//	TArray<class AActor*> GroupActorsForThisSetting;
	//	// ThisSetting 苞 楷搬等 弊缝咀磐甸 葛滴 葛澜. PC 啊 酒聪扼搁 愧父窍搁 窍唱老 巴.
	//	EditorGetMatineeGroupActorsFromShowSetting(ThisSetting, GroupActorsForThisSetting);		

	//	for (int32 GAI = 0; GAI < GroupActorsForThisSetting.Num(); ++GAI)
	//	{
	//		ASkeletalMeshActor* CurrGroupSKActor = Cast<ASkeletalMeshActor>(GroupActorsForThisSetting[GAI]);
	//		if (CurrGroupSKActor == NULL)
	//		{
	//			continue;
	//		}

	//		// 捞力 霖厚啊 登菌栏聪 ThisSetting 俊 秦寸窍绰 利例茄 胶纳老阑 茫酒辑 CurrGroupSKActor 俊 汲沥.

	//		FTransform PreviewCompTransform;
	//		USkeletalMesh* PreviewSKMesh = NULL;
	//		// 滴锅掳 牢磊肺 弊缝 SkeletalMesh 甫 逞败辑 PC 狼 版快 ThisSetting 父 啊瘤绊绰 舅 荐 绝绰 漂沥 努贰胶俊 措茄 沥焊甫 掘绢坷档废 窃.
	//		if (GetEssentialPreviewInfoForSetting(ThisSetting, CurrGroupSKActor->GetSkeletalMeshComponent()->SkeletalMesh, PreviewSKMesh, PreviewCompTransform) == false)
	//		{
	//			continue;
	//		}
	//		// 老窜 咯扁辑绰 Scale 父 技泼阑.. 荤角 舅绊焊搁 ABladeIICharacter::SetupControlledMatineePuppet 俊辑 窍绰 巴苞 付蛮啊瘤..
	//		CurrGroupSKActor->GetSkeletalMeshComponent()->SetWorldScale3D(PreviewCompTransform.GetScale3D());
	//	}
	//}
}

void AB2StageEventDirector::EditorGetMatineeGroupActorsFromShowSetting(FPlayActorShowSettings& InSetting, TArray<AActor*>& OutGroupActors)
{
	//if (OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee || MatineeActor == NULL || MatineeActor->MatineeData == NULL)
	//{
	//	return;
	//}

	//TArray<FName> AllSupposedIGNames;
	//// PC 狼 版快 辆幅喊肺 秦辑 窍唱狼 ShowSetting 捞 咯矾 Matinee Group 俊 楷搬登绢 乐阑 荐 乐栏聪 啊瓷茄 巴甸阑 葛滴 八荤.
	//GetAllSupposedInterpGroupNamesForSetting(AllSupposedIGNames, InSetting); 

	//for (int32 GAI = 0; GAI < MatineeActor->MatineeData->InterpGroups.Num(); ++GAI)
	//{
	//	UInterpGroup* CurrGroup = MatineeActor->MatineeData->InterpGroups[GAI];

	//	bool bIsOneOfTargetGroup = false;
	//	for (int32 GNI = 0; GNI < AllSupposedIGNames.Num(); ++GNI)
	//	{
	//		if (CurrGroup->GroupName == AllSupposedIGNames[GNI])
	//		{
	//			bIsOneOfTargetGroup = true;
	//			break;
	//		}
	//	}

	//	if (bIsOneOfTargetGroup == false)
	//	{
	//		continue;; // InSetting 苞 楷搬等 Group 篮 酒丛.
	//	}

	//	TArray<class AActor*> CurrGroupActors;
	//	// 俊叼磐俊辑绰 GroupActorInfos 甫 烹秦 弊缝俊 楷搬等 咀磐甸阑 茫酒棵 荐 乐澜. -> 酒聪 俊叼磐 富绊 弊成 霸烙俊辑档 登绰 芭 酒囱啊..?
	//	for (int32 GAI2 = 0; GAI2 < MatineeActor->GroupActorInfos.Num(); ++GAI2)
	//	{
	//		if (MatineeActor->GroupActorInfos[GAI2].ObjectName == FName(*CurrGroup->GetName()))
	//		{
	//			// OutGroupActors 俊 歹茄促. 窍唱狼 弊缝俊 咯矾 Actor 啊 殿废等 惑炔捞 倔付唱 乐唱?
	//			OutGroupActors += MatineeActor->GroupActorInfos[GAI2].Actors;
	//			break;
	//		}
	//	}
	//}
}

void AB2StageEventDirector::SyncFromPreviewSKCompPlacement()
{
	// PreviewMapping 阑 烹秦 阿 preview component 喊肺 ShowSetting 阑 茫酒辑 Placement 殿狼 汲沥俊 蝶扼 鞘夸窍搁 蔼阑 技泼茄促.

	for(int32 PCI = 0; PCI < PreviewSKComps.Num(); ++PCI)
	{
		USkeletalMeshComponent* CurrSKComp = PreviewSKComps[PCI];
		int32 ShowSettingIndex = -1;

		for(int32 PMI = 0; PMI < PreviewMapping.Num(); ++PMI)
		{
			if(PreviewMapping[PMI].SKComp == CurrSKComp) // 酒付 PCI 尔 PMI 啊 老摹且 巴
			{
				ShowSettingIndex = PreviewMapping[PMI].ShowSettingIndex;
				break;
			}
		}

		if(CurrSKComp && ShowSettingIndex >= 0 && ShowSettingIndex < ShowSettings.Num())
		{
			FPlayActorShowSettings& CurrSetting = ShowSettings[ShowSettingIndex];

			if(DoesThisSettingRequireManualPlacement(CurrSetting))
			{
				// 橇府轰绰 捞霸 酒囱 惑炔俊辑档 唱棵 荐 乐瘤父 Sync 绰 捞 版快父.

				// 咯扁辑 哗辰 蔼篮 BladeIICharacter 侩捞 瞪 巴捞骨肺 Z offset 阑 粱 皑救秦具摆促. 捞贰历贰 粱 固胶概摹啊 乐摆绰单..
				float ZOffsetForCharacter = CurrSKComp->Bounds.BoxExtent.Z;

				// 瘤陛鳖瘤狼 荤侩俊辑 ManualRelativeLocation/Rotation 篮 捞 咀磐狼 风飘 扁霖捞骨肺 傲 窜鉴洒 GetRelativeTransform 窍搁 登摆促.
				// 酒囱 版快啊 积扁搁, GetComponentTransform 沁促啊 鞘夸茄 扁霖 transform 阑 哗档废.
				CurrSetting.ManualRelativeLocation = CurrSKComp->GetRelativeTransform().GetTranslation() + FVector(0.0f,0.0f,ZOffsetForCharacter);
				CurrSetting.ManualRelativeRotation = CurrSKComp->GetRelativeTransform().Rotator();
			}
		}
	}
}

FString AB2StageEventDirector::GetPreviewSKCompNameForSetting(int32 ShowSettingIndex)
{
	return FString::Printf(TEXT("PreviewSkeletalMeshComponent_%d"), ShowSettingIndex);
}

FString AB2StageEventDirector::GetPreviewInfoTextCompNameForSetting(int32 ShowSettingIndex)
{
	return FString::Printf(TEXT("PreviewInfoTextComponent_%d"), ShowSettingIndex);
}

UB2PCClassInfo* AB2StageEventDirector::GetPCClassInfoForPlayActorType(EStageEventPlayActor InType)
{
	//if( IsPlayActorTypeForPlayerCharacter(InType) )
	//{
	//	UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox();
	//	if(PCClassInfoBox)
	//	{
	//		// 努贰胶甫 沥秦初绊 啊廉咳. 拱沸 WorldSetting 俊 汲沥茄 GameMode 甫 杭 荐档 乐摆瘤父 被捞 弊凡 鞘夸啊..
	//		// Main 捞唱 Current 搁 Gladiator 肺, 酒聪搁 Assassin
	//		EPCClass PCClassType = EPCClass::EPC_End;
	//		switch (InType)
	//		{
	//			case EStageEventPlayActor::ESEE_MainPlayer :
	//			case EStageEventPlayActor::ESEE_CurrentPlayer :
	//			case EStageEventPlayActor::ESEE_WinningPlayer_Current :
	//			case EStageEventPlayActor::ESEE_TeamAllyPlayer :
	//			case EStageEventPlayActor::ESEE_TeamWinningPlayer :
	//			case EStageEventPlayActor::ESEE_RaidPlayer:
	//			case EStageEventPlayActor::ESEE_WinningRaidPlayer:
	//			case EStageEventPlayActor::ESEE_DefeatedRaidPlayer:
	//				PCClassType = EPCClass::EPC_Gladiator;
	//				break;
	//			case EStageEventPlayActor::ESEE_SubPlayer :
	//			case EStageEventPlayActor::ESEE_OtherPlayer :
	//			case EStageEventPlayActor::ESEE_WinningPlayer_Other:
	//			case EStageEventPlayActor::ESEE_TeamEnemyPlayer:
	//				PCClassType = EPCClass::EPC_Assassin;
	//				break;
	//			case EStageEventPlayActor::ESEE_RemoteOpponent_Current:
	//				PCClassType = EPCClass::EPC_Fighter;
	//				break;
	//			default :
	//				PCClassType = EPCClass::EPC_Wizard;
	//				break;
	//		}

	//		if (PCClassType != EPCClass::EPC_End)
	//			return PCClassInfoBox->GetSingleClassInfo(PCClassType);
	//	}
	//}
	return NULL;
}

bool AB2StageEventDirector::GetEssentialPreviewInfoForSetting(FPlayActorShowSettings& InSetting, class USkeletalMesh* OptionalSpecificPCClassMesh, class USkeletalMesh*& OutSKMesh, FTransform& OutTransform)
{
	//bool bGotTransform = false;

	//if(DoesThisSettingHaveNoFixedPlacement(InSetting))
	//{
	//	// 牢霸烙 困摹甫 荤侩窍绰 版快 殿 沥秦柳 困摹啊 绝促搁 橇府轰 惑栏肺绰 利寸茄 困摹俊 初绰促. Default 肺 技泼茄 墨皋扼客 付林焊霸?
	//	OutTransform = FTransform(FRotator(0.0f,-90.0f,0.0f), FVector(0.0f,100.0f,0.0f)) * this->GetTransform();
	//	bGotTransform = true;
	//}
	//else if (DoesThisSettingRequireManualPlacement(InSetting)) // 老馆利牢 荐悼 汲沥 惑炔
	//{
	//	const FTransform RelativeTransform(InSetting.ManualRelativeRotation, InSetting.ManualRelativeLocation);
	//	OutTransform = RelativeTransform * this->GetTransform();
	//	bGotTransform = true; // 老窜 己傍
	//}
	//else if(OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera)
	//{
	//	// SimpleEventScene 阑 荤侩窍绰 版快啊 登摆促. 开矫唱 AdjustToPlayActor 绰 公矫窍绊 CameraAnimApplyCamIndex 肺 急琶茄 墨皋扼俊 嘎苗辑 ExportedEventScene 狼 霉 虐蔼栏肺 困摹 魂沥.
	//	// 咯变 弊成 橇府轰肺父.
	//	if (IsExportedEventSceneValid(InSetting.ExportedEventScene) && CameraAnimApplyCamIndex >= 0 && CameraAnimApplyCamIndex < MAX_DIRECTOR_CAMERA)
	//	{
	//		FSimpleEventSceneMoveKeyData& FirstKeyData = InSetting.ExportedEventScene->SimpleMoveKeyFrames[0];

	//		// SetSESPlayActorMovementFromCamera 曼绊. 缴敲窍霸 鞘夸茄 琴缴父.
	//		UCameraComponent* BaseCamComp = DirectorCamArrayRef[CameraAnimApplyCamIndex];
	//		OutTransform = FTransform(FirstKeyData.RotKey, FirstKeyData.PosKey) * BaseCamComp->GetComponentTransform();
	//		bGotTransform = true;
	//	}
	//}
	//else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//{
	//	// ControlledMatinee 俊辑 SkeletalMeshActor 胶纳老 嘎苗焊扁 困秦 烹苞矫糯. 咯扁辑档 拱沸 鞘夸窍促搁 秦寸 弊缝咀磐 茫酒辑 硅摹等 Transform 阑 持绰促芭唱 秦杭 荐 乐摆促.
	//	OutTransform = FTransform();
	//	bGotTransform = true;
	//}

	//if (bGotTransform == false)
	//{
	//	return false;
	//}

	//// 钎矫且 transform 捞 积板栏聪 SkeletalMesh 葛胆 惶酒坷扁.	
	//if( IsPlayActorTypeForPlayerCharacter(InSetting.PlayActorType) )
	//{
	//	UB2PCClassInfo* PCClassInfo = GetPCClassInfoForPlayActorType(InSetting.PlayActorType); // 咯扁辑绰 酒抗 沥秦柳 鸥涝狼 PCClass 甫 啊廉棵 巴. 绢瞒乔 俊叼磐扼 绢恫 努贰胶啊 唱棵瘤 葛福绰 老捞骨肺.

	//	if (OptionalSpecificPCClassMesh)
	//	{
	//		// 捞 版快 Player Character 捞搁辑 皋浆甫 瘤沥窍看栏聪 弊 Mesh 甫 荤侩窍绰 PCClassInfo 俊辑 沥焊甫 啊廉咳.
	//		// 捞扒 荤角惑 努贰胶喊肺 飘发阑 父甸绢具 窍绰 Controlled Matinee 侩烙.
	//		UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox();
	//		if (PCClassInfoBox)
	//		{
	//			UB2PCClassInfo* PCClassInfoFromSKMesh = PCClassInfoBox->GetPCClassInfoForMatchingSKMesh(OptionalSpecificPCClassMesh);
	//			if (PCClassInfoFromSKMesh)
	//			{
	//				PCClassInfo = PCClassInfoFromSKMesh;
	//			}
	//		}
	//	}
	//	
	//	// 咯扁鳖瘤 茫篮 PCClassInfo 俊 技泼茄 吧肺..
	//	if(PCClassInfo)
	//	{
	//		OutSKMesh = PCClassInfo->BaseMeshAsset;

	//		// PCClassInfo 甫 扁馆栏肺 胶纳老档 嘎苗 焊磊.
	//		if(PCClassInfo->GetBaseBPClass(this))
	//		{
	//			ABladeIIPlayer* DefaultPlayerBPObject = Cast<ABladeIIPlayer>(PCClassInfo->GetBaseBPClass(this)->GetDefaultObject());
	//			if(DefaultPlayerBPObject && DefaultPlayerBPObject->GetMesh())
	//			{
	//				// 咯扁急 GetMesh()->GetComponentScale 肺绰 抗惑窍绰 蔼捞 救唱咳. RootComponent 何磐 秦辑 relative scale 阑 啊廉促 蚌茄促.
	//				FVector CompScale = 
	//					DefaultPlayerBPObject->GetRootComponent()->RelativeScale3D *
	//					(
	//						(PCClassInfo->MeshScaleOverride > 0.0f) ? 
	//						FVector(PCClassInfo->MeshScaleOverride, PCClassInfo->MeshScaleOverride, PCClassInfo->MeshScaleOverride) : 
	//						DefaultPlayerBPObject->GetMesh()->RelativeScale3D
	//					);

	//				OutTransform.SetScale3D(CompScale);
	//			}
	//		}
	//	}
	//}
	//else if(InSetting.PlayActorType == EStageEventPlayActor::ESEE_WaveMob)
	//{
	//	int32 WaveNum = 0;
	//	int32 MobIndex = 0;
	//	ParseForWaveMobSelectKeyword(InSetting.WaveMobSelectKeyword, WaveNum, MobIndex);
	//	
	//	ABladeIIWorldSettings* B2WS = Cast<ABladeIIWorldSettings>(GetWorldSettings());
	//	const int32 EditorStageNum = B2WS ? B2WS->GetEditorStageNumber() : 1;
	//	const int32 EditorStageDifficulty = B2WS ? B2WS->GetEditorStageDifficulty() : 1;

	//	AB2MonsterSpawnPool* RelevantEditorSpawnPool = EditorGetCurrentActiveSpawnPool();
	//	if (RelevantEditorSpawnPool && IsStageSupported(EditorStageNum, EditorStageDifficulty)) // WorldSettings 俊 汲沥茄 EditorStageNum 捞 瘤盔登绰 汲沥俊辑父.
	//	{
	//		// NPCClass enum 趣篮 ABladeIICharacter 喉风橇赴飘 努贰胶 笛 吝 窍唱.
	//		ENPCClass NPCClassEnum = ENPCClass::ENC_End;
	//		ENPCClassVariation NPCClassVariation = ENPCClassVariation::ENCV_Normal;
	//		TSubclassOf<ABladeIICharacter> NPCClassBP = NULL;
	//		RelevantEditorSpawnPool->EditorGetSpawnClassOfWaveMob(NPCClassEnum, NPCClassVariation, NPCClassBP, WaveNum, MobIndex);

	//		// AB2MonsterSpawnPool::SpawnWave 客 付蛮啊瘤 快急鉴困肺..
	//		if(NPCClassEnum != ENPCClass::ENC_End)
	//		{
	//			UB2NPCClassInfoBox* MobClassInfoBox = StaticFindMobClassInfoBox();
	//			if(MobClassInfoBox)
	//			{
	//				UB2NPCClassInfoBox::NPCClassPreviewAssetInfo PreviewAssetInfo = MobClassInfoBox->GetPreviewAssetInfoFromClass(NPCClassEnum, NPCClassVariation);
	//				// PreviewAssetInfo 俊 MaterialOverride 档 唱坷变 窍绰单 咯扁俊档 馆康阑 且鳖 富鳖..
	//				OutSKMesh = PreviewAssetInfo.SKMesh;

	//				// 咯扁档 NPCClassInfo 甫 扁馆栏肺 胶纳老 嘎眠扁.
	//				UB2NPCSingleClassInfo* SingleMobInfo = MobClassInfoBox->GetNPCSingleClassInfo(NPCClassEnum, NPCClassVariation);
	//				if(SingleMobInfo && SingleMobInfo->GetBaseBPClass(this))
	//				{
	//					ABladeIICharacter* DefaultBPObject = Cast<ABladeIICharacter>(SingleMobInfo->GetBaseBPClass(this)->GetDefaultObject());
	//					if(DefaultBPObject && DefaultBPObject->GetMesh())
	//					{
	//						// 咯扁急 GetMesh()->GetComponentScale 肺绰 抗惑窍绰 蔼捞 救唱咳. RootComponent 何磐 秦辑 relative scale 阑 啊廉促 蚌茄促.
	//						FVector CompScale = 
	//							DefaultBPObject->GetRootComponent()->RelativeScale3D *
	//							(
	//							(SingleMobInfo->MeshScaleOverride > 0.0f) ? 
	//							FVector(SingleMobInfo->MeshScaleOverride, SingleMobInfo->MeshScaleOverride, SingleMobInfo->MeshScaleOverride) : 
	//							DefaultBPObject->GetMesh()->RelativeScale3D
	//							);

	//						OutTransform.SetScale3D(CompScale);
	//					}
	//				}
	//			}
	//		}
	//		else if(NPCClassBP != NULL)
	//		{
	//			ABladeIICharacter* DefaultB2CharBPObject = Cast<ABladeIICharacter>(NPCClassBP);
	//			if(DefaultB2CharBPObject && DefaultB2CharBPObject->GetMesh())
	//			{
	//				OutSKMesh = DefaultB2CharBPObject->GetMesh()->SkeletalMesh;

	//				OutTransform.SetScale3D(
	//					DefaultB2CharBPObject->GetRootComponent()->RelativeScale3D * DefaultB2CharBPObject->GetMesh()->RelativeScale3D
	//					);
	//			}
	//		}
	//	}
	//}
	//	
	//if(OutSKMesh)
	//{
	//	return true;
	//}
	//
	return false;
}

void AB2StageEventDirector::DestroyPreviewComponents(bool bDestroyAll)
{
	if(bDestroyAll) // 捞霸 false 牢 巴篮 窜鉴 困摹 捞悼 矫 refresh 俊 荤侩窍扁 困窃.
	{
		for(int32 PCI = 0; PCI < PreviewSKComps.Num(); ++PCI)
		{
			if(PreviewSKComps[PCI])
			{
				PreviewSKComps[PCI]->DestroyComponent();
			}
		}
		PreviewSKComps.Empty();
	}
	
	for(int32 PCI = 0; PCI < PreviewSKCompsInfoText.Num(); ++PCI)
	{
		if(PreviewSKCompsInfoText[PCI])
		{
			PreviewSKCompsInfoText[PCI]->DestroyComponent();
		}
	}
	PreviewSKCompsInfoText.Empty();
	PreviewMapping.Empty();
	for(int32 PCI = 0; PCI < DirectorCameraNumberText.Num(); ++PCI)
	{
		if(DirectorCameraNumberText[PCI])
		{
			DirectorCameraNumberText[PCI]->DestroyComponent();
		}
	}
	DirectorCameraNumberText.Empty();
	if (ActorNameDisplayText)
	{
		ActorNameDisplayText->DestroyComponent();
		ActorNameDisplayText = NULL;
	}
	for (int32 SCI = 0; SCI < AnyShapeCompNameText.Num(); ++SCI)
	{
		if (AnyShapeCompNameText[SCI])
		{
			AnyShapeCompNameText[SCI]->DestroyComponent();
		}
	}
	AnyShapeCompNameText.Empty();
}

void AB2StageEventDirector::SafetyProcessMatineeSkActors()
{
	//if (MatineeActor == NULL || MatineeActor->MatineeData == NULL)
	//{
	//	return;
	//}

	//const bool bHasAnyDirectedFromBeginningSetup = HasAnyDirectedFromBeginningTrackInCurrentWorld(GetWorld());

	//// 付萍聪 弊缝 咀磐肺 殿废等 ASkeletalMeshActor 甸篮 ShowSettings 殿废 咯何俊 惑包绝捞 collision 公炼扒 波 初澜. 汲付 鞘夸茄 版快啊 乐阑鳖?
	//for (int32 GAI = 0; GAI < MatineeActor->GroupActorInfos.Num(); ++GAI)
	//{
	//	for (AActor* CurrGA : MatineeActor->GroupActorInfos[GAI].Actors)
	//	{
	//		ASkeletalMeshActor* SKActor = Cast<ASkeletalMeshActor>(CurrGA);
	//		USkeletalMeshComponent* SKActorComp = SKActor ? SKActor->GetSkeletalMeshComponent() : nullptr;
	//		if (SKActor && SKActorComp)
	//		{
	//			SKActor->SetActorEnableCollision(false);
	//			SKActorComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	//			// ActorHiddenIngame 汲沥 夌阑锭父
	//			if (SKActor->bHidden)
	//			{
	//				// 肚茄, 埃趣 狼档摹 臼霸 焊捞瘤 臼绰 惑怕肺 局聪皋捞记捞 敲饭捞登绊 乐绢辑 巩力啊 登绰 版快啊 乐澜. (AnimNotify 惯积) 蝶扼辑 捞巴甸档 绝局 淋. DirectedFromBeginning 捞搁 矫累 矫 局聪皋捞记 流立 犁积.
	//				SKActorComp->AnimationData.bSavedPlaying = false;
	//				SKActorComp->AnimationData.bSavedLooping = false;
	//				if (!bHasAnyDirectedFromBeginningSetup)
	//				{
	//					// AnimToPlay 饭欺繁胶绰 漂洒唱 楷免 侩 单捞磐狼 急喊 肺爹阑 窍霸 登搁 鞘夸绝绰 肺爹阑 蜡惯窍骨肺 弥利拳 啊瓷己 搁俊辑扼档 绝局林绰 霸 亮篮单
	//					// DirectedFromBeginning 技泼苞 爵腮 版快绰 绝局急 救 等促.
	//					// 捞 SkeletalMeshActor 啊 傈眉 StageEventDirector 吝 绢蠢 窍唱窍绊扼档 DirectedFromBeginning 栏肺 揩菌绰瘤 八荤窍妨聪 粱 滚疤绊 秦辑 
	//					// 弊成 傈眉 岿靛俊 弊繁 技泼捞 绝绰 版快父 厚框. 弊犯霸 腹捞 荤侩窍绰 扒 酒聪扼辑..
	//					SKActorComp->AnimationData.AnimToPlay = nullptr;
	//				}
	//			}
	//		}
	//	}
	//}

	//// ShowSetting 捞尔 楷搬捞 登绢乐绰 SkeletalMeshActor 甸篮 visibility 殿狼 眠啊 技泼鳖瘤 秦 初澜.
	//for (int32 SI = 0; SI < ShowSettings.Num(); ++SI)
	//{
	//	FPlayActorShowSettings& ThisSetting = ShowSettings[SI];
	//	TArray<class AActor*> GroupActorsForThisSetting;
	//	// ThisSetting 苞 楷搬等 弊缝咀磐甸 葛滴 葛澜. PC 啊 酒聪扼搁 愧父窍搁 窍唱老 巴.
	//	EditorGetMatineeGroupActorsFromShowSetting(ThisSetting, GroupActorsForThisSetting);

	//	for (int32 GAI = 0; GAI < GroupActorsForThisSetting.Num(); ++GAI)
	//	{
	//		ASkeletalMeshActor* CurrGroupSKActor = Cast<ASkeletalMeshActor>(GroupActorsForThisSetting[GAI]);
	//		if (CurrGroupSKActor == NULL)
	//		{
	//			continue;
	//		}

	//		CurrGroupSKActor->SetActorHiddenInGame(true);
	//		USkeletalMeshComponent* ThisSKComp = CurrGroupSKActor->GetSkeletalMeshComponent();
	//		ThisSKComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered; // 捞巴档 楷免 档吝俊绰 劝惯洒 诀单捞飘窍档废 蔼阑 官层林聪 扁夯蔼篮 冻绢哆妨 初档废.

	//		// 眠啊肺 MaterialInstanceDynamic 捞 overriding 登绢 乐促搁 朝妨霖促. ClearControlledMatineePuppet 俊辑 窍妨绰 扒单 购啊 肋 救冈囚辑 咯扁辑 焊促 犬角窍霸..
	//		// MID 鄂 霸 贸澜何磐 历厘阑 窍扁 困茄 霸 酒聪促.
	//		for (int32 MI = 0; MI < ThisSKComp->OverrideMaterials.Num(); ++MI)
	//		{
	//			UMaterialInstanceDynamic* OverriddenMID = Cast<UMaterialInstanceDynamic>(ThisSKComp->OverrideMaterials[MI]);
	//			if (OverriddenMID)
	//			{
	//				ThisSKComp->SetMaterial(MI, NULL);
	//			}
	//		}
	//	}
	//}
}

void AB2StageEventDirector::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_ManualCameraSettings = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, ManualCameraSettings);
	const FName Name_CameraIndex = GET_MEMBER_NAME_CHECKED(FManualCameraSingleSetting, CameraIndex);
	const FName Name_PlayActorType = GET_MEMBER_NAME_CHECKED(FPlayActorShowSettings, PlayActorType);
	const FName Name_PlayActorPlacement = GET_MEMBER_NAME_CHECKED(FPlayActorShowSettings, PlayActorPlacement);
	const FName Name_OverallMode = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, OverallMode);
	const FName Name_ShowSettings = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, ShowSettings);
	const FName Name_bOverallMode_ExportedCamera = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, bOverallMode_ExportedCamera);
	const FName Name_bOverallMode_ControlledMatinee = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, bOverallMode_ControlledMatinee);
	const FName Name_bOverallMode_ManualCamera = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, bOverallMode_ManualCamera);
	const FName Name_bOverallMode_ExportedOrManualCamera = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, bOverallMode_ExportedOrManualCamera);
	const FName Name_ShowOverEventName = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, ShowOverEventName);
	const FName Name_MobEventMobSelectKeyword = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, MobEventMobSelectKeyword);
	const FName Name_DirectingEvent = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, DirectingEvent);
	const FName Name_SupportedStages = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, SupportedStages);
	const FName Name_SupportedDifficulties = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, SupportedDifficulties);
	const FName Name_MatineeFirstSkipFrames = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, MatineeFirstSkipFrames);
	const FName Name_MobEventMobPhase = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, MobEventMobPhase);
	const FName Name_ManageMode = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, ManageMode);
	const FName Name_RandomMatineeGroupNum = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, RandomMatineeGroupNum);
	const FName Name_LevelToLoad = GET_MEMBER_NAME_CHECKED(FB2StageStreamingSettings, LevelToLoad);
	const FName Name_LevelToUnload = GET_MEMBER_NAME_CHECKED(FB2StageStreamingSettings, LevelToUnload);
	const FName Name_OverallStartDelay = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, OverallStartDelay);
	const FName Name_bLocalPlayerSwitching = GET_MEMBER_NAME_CHECKED(AB2StageEventDirector, bLocalPlayerSwitching);

	// 唱赣瘤 捞抚 度鞍酒瘤档废 犬牢瞒盔
	GET_MEMBER_NAME_CHECKED(FPlayActorShowSettings, bOverallMode_ExportedCamera);
	GET_MEMBER_NAME_CHECKED(FPlayActorShowSettings, bOverallMode_ControlledMatinee);
	GET_MEMBER_NAME_CHECKED(FPlayActorShowSettings, bOverallMode_ManualCamera);
	GET_MEMBER_NAME_CHECKED(FPlayActorShowSettings, bOverallMode_ExportedOrManualCamera);
//
//	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
//	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;
//
//	if (PropertyName == Name_ManualCameraSettings)
//	{
//		// 利绢档 窍唱绰 乐档废 窃.
//		if (ManualCameraSettings.Num() == 0)
//		{
//			ManualCameraSettings.Add(FManualCameraSingleSetting());
//		}
//	}
//
//	if (PropertyName == Name_CameraIndex)
//	{
//		for (int32 CI = 0; CI < ManualCameraSettings.Num(); ++CI)
//		{
//			ManualCameraSettings[CI].CameraIndex = FMath::Clamp(ManualCameraSettings[CI].CameraIndex, 0, MAX_DIRECTOR_CAMERA - 1);
//		}
//	}
//
//	if (PropertyName == Name_PlayActorType)
//	{
//		// MainPlayer 唱 SubPlayer 啊 笛究 唱坷瘤 臼霸 眉农 棺 版绊
//		for (int32 IA = 0; IA < ShowSettings.Num(); ++IA)
//		{
//			EStageEventPlayActor CurrType = ShowSettings[IA].PlayActorType;
//
//			for (int32 IB = IA + 1; IB < ShowSettings.Num(); ++IB)
//			{
//				EStageEventPlayActor CompareType = ShowSettings[IB].PlayActorType;
//
//				if (IsPlayActorTypeForPlayerCharacter(CurrType) && 
//					ShowSettings[IB].IsTeamPlayActorType() == false && 
//					ShowSettings[IB].IsRaidPlayActorType() == false &&
//					CurrType == CompareType)
//				{
//#if !PLATFORM_MAC
//					FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//						FString::Printf(TEXT("[Warning] 悼老茄 敲饭捞绢 某腐磐狼 PlayActorType 捞 吝汗登绢 唱鸥车嚼聪促.\nIndex %d and %d"), IA, IB)
//						));
//#endif
//				}
//			}
//		}
//	}
//
//	if (PropertyName == Name_PlayActorType || PropertyName == Name_DirectingEvent)
//	{
//		if (DirectingEvent != EStageEvent::ESEV_MobAppear && DirectingEvent != EStageEvent::ESEV_MobDying && DirectingEvent != EStageEvent::ESEV_MobPhase)
//		{
//#if !PLATFORM_MAC
//			FString WarnMsg = TEXT("泅犁 Directing Event 汲沥俊辑绰 Mob 捞 殿厘窍瘤 臼篮 惑怕俊辑 楷免 厘搁捞 矫累瞪 困氰捞 乐嚼聪促. 漂喊茄 捞蜡啊 酒聪搁 Mob Appear/Dying/Phase 甫 荤侩窍芭唱 Show Settings 俊辑 Wave Mob 阑 绝局林技夸.\n\n");
//			
//			if (DirectingEvent == EStageEvent::ESEV_ComponentTrigger)
//			{
//				WarnMsg += TEXT("漂洒 ComponentTrigger 磊林 角荐窍绰 汲沥捞聪 犬角洒 巩力绝绊 鞘夸茄 镑俊父 犬牢窍绊 荤侩窍技夸!\n\n");
//			}
//
//			WarnMsg += TEXT("Mob 捞 殿厘窍绰 Show Setting 锅龋 : ");
//			bool bFoundNoneDesiredSetting = false;
//			for (int32 SI = 0; SI < ShowSettings.Num(); ++SI)
//			{
//				if (ShowSettings[SI].PlayActorType == EStageEventPlayActor::ESEE_WaveMob)
//				{
//					WarnMsg += FString::Printf(TEXT("%d, "), SI);
//					bFoundNoneDesiredSetting = true;
//				}
//			}
//			if (bFoundNoneDesiredSetting)
//			{
//				FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMsg));
//			}
//#endif
//		}
//	}
//
//	if (PropertyName == Name_DirectingEvent || PropertyName == Name_OverallStartDelay)
//	{
//		if (DirectingEvent == EStageEvent::ESEV_MobDying && OverallStartDelay > 0.0f)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] MobDying 捞亥飘俊 StartDelay %.2f (捞)啊 汲沥登绢 乐嚼聪促.\n扁夯 瘤盔篮 登瘤父 Dying Mob 捞 殿厘窍绰 版快 StartDelay 啊 辨荐废 悼累捞 焊厘登瘤 臼阑 啊瓷己捞 臭嚼聪促.\n啊鞭利 促弗 规过阑 茫芭唱 犬角茄 抛胶飘甫 芭摹绰 巴捞 亮嚼聪促."), OverallStartDelay)
//				));
//#endif
//		}
//	}
//
//	if (PropertyName == Name_MobEventMobSelectKeyword)
//	{
//		bool bFoundWaveSetting = false;
//		bool bFoundSpawnPool = false;
//		int32 ParsedWaveNum = 0;
//		int32 ParsedMobIndex = 0;
//		ParseForWaveMobSelectKeyword(MobEventMobSelectKeyword, ParsedWaveNum, ParsedMobIndex);
//
//		UWorld* EditorWorld = GEditor ? GEditor->GetEditorWorldContext().World() : NULL;
//		for (FActorIterator ActorIt(EditorWorld); ActorIt; ++ActorIt)
//		{
//			AB2MonsterSpawnPool* ThisSP = Cast<AB2MonsterSpawnPool>(*ActorIt);
//			if (ThisSP != NULL)
//			{
//				bFoundSpawnPool = true;
//
//				// 角力 乐绰 Wave Num 阑 借绰瘤 犬牢
//				for (int32 SI = 0; SI < ThisSP->GetStageSettingsNum(); ++SI)
//				{
//					// SpawnPool 狼 TargetDifficulty 绰 撤阑荐废 角力 荤侩啊瓷茄 抄捞档啊 腹酒柳促. 
//					// 弊吧 皑救窍磊搁 捞锅 技泼狼 TargetDifficulty 焊促 臭篮 抄捞档肺 啊搁辑 眉农秦杭 鞘夸绰 乐绰单 弊犯霸 窍磊搁 咯矾 惑炔阑 眉农秦具 秦辑.. 傲 酒贰 皋矫瘤甫 焊碍窍绰 沥档父.
//					int32 ThisSPSettingDifficultyLevel = ThisSP->GetTargetDifficultyLevelBySettingIndex(SI);
//					if (IsStageSupported(ThisSP->GetTargetStageIdBySettingIndex(SI), ThisSPSettingDifficultyLevel) == false)
//					{
//						continue; // 瘤盔登绰 胶抛捞瘤 技泼父.
//					}
//					
//					if (ParsedWaveNum >= 0 && ParsedWaveNum < ThisSP->GetStageWaveNumBySettingIndex(SI))
//					{
//						bFoundWaveSetting = true;
//						break;
//					}
//				}
//			}
//		}
//				
//		if(!bFoundSpawnPool)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] Mob Event 荤侩阑 困茄 Spawn Pool 咀磐啊 惯斑登瘤 臼疽嚼聪促."))
//				));
//#endif
//		}
//		else if (!bFoundWaveSetting)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[WARNING!] 岿靛俊 粮犁窍绰 Monster Spawn Pool 咀磐俊辑 瘤沥茄 胶抛捞瘤/抄捞档客 瘤沥茄 %d 锅龋狼 Wave Setting 阑 茫阑 荐 绝嚼聪促.\n\n抄捞档狼 版快 SpawnPool 俊辑绰 撤篮 抄捞档 汲沥捞 臭篮 抄捞档甫 措脚且 荐 乐栏骨肺 角力肺绰 荤侩 啊瓷且 荐档 乐嚼聪促."),
//					ParsedWaveNum)
//			));
//#endif
//		}
//	}
//
//	if (PropertyName == Name_PlayActorPlacement)
//	{
//		int32 TotalCount = 0;
//		for (int32 SI = 0; SI < ShowSettings.Num(); ++SI)
//		{
//			// PlayActor 扁霖栏肺 困摹甫 技泼窍绰 芭扼搁 拱沸 弊 PlayActor绰 窍唱父 乐绢具摆贬.
//			if (ShowSettings[SI].IsOneOfAdjustToPlayActorPlacement())
//			{
//				++TotalCount;
//			}
//		}
//		if (TotalCount >= 2)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] %d 俺狼 AdjustToPlayActor 拌凯 技泼捞 惯斑登菌嚼聪促.\n窍唱父 难滴绰 巴阑 鼻厘钦聪促."), TotalCount)
//				));
//#endif
//		}
//	}
//
//	if (PropertyName == Name_SupportedStages)
//	{
//		for (int32 SI = 0; SI < SupportedStages.Num(); ++SI)
//		{
//			SupportedStages[SI] = FMath::Max(1, SupportedStages[SI]);
//		}
//	}
//	if (PropertyName == Name_SupportedDifficulties)
//	{
//		for (int32 SI = 0; SI < SupportedDifficulties.Num(); ++SI)
//		{
//			SupportedDifficulties[SI] = FMath::Clamp(SupportedDifficulties[SI], static_cast<int32>(EStageDifficulty::ESD_Normal), static_cast<int32>(EStageDifficulty::ESD_Hell));
//		}
//	}
//
//	if (PropertyName == Name_MatineeFirstSkipFrames)
//	{
//		if (MatineeFirstSkipFrames >= 2)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[林狼] MatineeFirstSkipFrames 汲沥父怒 霉 %d 橇饭烙捞 胶诺邓聪促.\n\n俊叼磐俊辑 焊绰 葛嚼苞 弥辆 副府令俊辑 焊绰 葛嚼苞绰 促甫 荐档 乐栏骨肺 呈公 奴 蔼篮 荤侩窍瘤 臼绰 巴捞 亮嚼聪促."), MatineeFirstSkipFrames)
//				));
//#endif
//		}
//	}
//
//	if (PropertyName == Name_MobEventMobPhase)
//	{
//		MobEventMobPhase = FMath::Max(MobEventMobPhase, 1);
//	}
//
//	if (PropertyName == Name_ManageMode || PropertyName == Name_DirectingEvent)
//	{
//		if (DirectingEvent == EStageEvent::ESEV_Clear && ManageMode != EStageEventManageMode::SEMM_Always)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("Clear 捞亥飘啊 唱客具 且 惑炔俊辑 胶诺瞪 家瘤啊 乐绰 ManageMode 肺 汲沥捞 登菌嚼聪促."))
//				));
//#endif
//		}
//	}
//
//	if (PropertyName == Name_RandomMatineeGroupNum)
//	{
//#if !PLATFORM_MAC
//		if(RandomMatineeGroupNum == 1) // 1 捞搁 角力肺绰 罚待 悼累阑 窍瘤 臼绰 巴牢单 趣 罚待 悼累阑 狼档窍绊 持菌阑瘤档 葛福聪.
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("RandomMatineeGroupNum 1 汲沥俊辑绰 角力肺 罚待 楷免 扁瓷捞 累悼窍瘤 臼嚼聪促.\n父老 罚待 楷免 扁瓷阑 狼档沁促搁 2焊促 奴 蔼栏肺 汲沥秦 林技夸."))
//				));
//		}
//		else if (RandomMatineeGroupNum >= 2)
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("RandomMatineeGroupNum 阑 %d (栏)肺 汲沥沁嚼聪促.\n角力 罚待 楷免 扁瓷阑 困秦辑绰 荤侩窍绰 Matinee 狼 阿 弊缝甸阑 咯扁俊 嘎苗 利例茄 Rand 匙捞怪栏肺 汲沥秦具 钦聪促.\n磊技茄 荤亲篮 包访 概春倔阑 曼绊窍技夸."),
//					RandomMatineeGroupNum)
//				));
//		}
//#endif
//	}
//
//	if (PropertyName == Name_LevelToLoad || PropertyName == Name_LevelToUnload)
//	{
//		UWorld* TheWorld = GetWorld();
//		LevelStreamingOnBegin.EditorCheckForProperSetting(TheWorld);
//		LevelStreamingOnEnd.EditorCheckForProperSetting(TheWorld);
//	}
//	
//	if (PropertyName == Name_bLocalPlayerSwitching)
//	{
//		if (bLocalPlayerSwitching)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("LocalPlayerSwitching 阑 恼聪促. Matinee 犁积矫 矫累窍绰 某腐磐客 场唱绰 某腐磐啊 促弗版快 付瘤阜 楷免 某腐磐肺 肺拿某腐磐甫 胶困莫 矫诺聪促.\n\n林狼 : 楷免俊 狼秦 霸烙 flow 啊 力绢邓聪促. 扁瓷 俺惯俊 蝶扼 累悼窍绰 炼扒捞唱 霸烙葛靛啊 茄沥瞪 荐 乐嚼聪促."))
//			));
//#endif
//		}
//	}
//
//	CheckAndWarnForLightSetup();
//
//	//if (
//	//	PropertyName == Name_OverallMode || PropertyName == Name_ShowSettings || // ShowSettings 眠啊矫俊档 累悼窍档废
//	//	PropertyName == Name_bOverallMode_ExportedCamera || PropertyName == Name_bOverallMode_ControlledMatinee ||
//	//	PropertyName == Name_bOverallMode_ManualCamera || PropertyName == Name_bOverallMode_ExportedOrManualCamera
//	//	) 
//	// 傲 攫力唱 累悼窍档废 秦具 且 淀.
//	{
//		SetEditConditionProperties();
//	}

	// 泅犁 惑炔栏肺绰 橇府轰 哪器惩飘狼 困摹蔼阑 加己芒俊辑 函版秦辑 坷绰 notify 啊 绝栏骨肺 咯扁辑 Sync 绰 给窍绊 Refresh 父..
	RefreshPreviewSKComps();
}

void AB2StageEventDirector::SetEditConditionProperties()
{
	bOverallMode_ExportedCamera = false;
	bOverallMode_ControlledMatinee = false;
	bOverallMode_ManualCamera = false;
	bOverallMode_ExportedOrManualCamera = false;

	// OverallMode 急琶俊 蝶扼 唱赣瘤绰 老褒利栏肺
	switch (OverallMode)
	{
	case EStageEventDirectorOverallMode::ESEDOM_ExportedCamera:
		bOverallMode_ExportedCamera = true;
		bOverallMode_ExportedOrManualCamera = true;
		break;
	case EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee:
		bOverallMode_ControlledMatinee = true;
		break;
	case EStageEventDirectorOverallMode::ESEDOM_ManualCamera:
		bOverallMode_ManualCamera = true;
		bOverallMode_ExportedOrManualCamera = true;
		break;
	}

	for(int32 SI = 0; SI < ShowSettings.Num(); ++SI)
	{
		FPlayActorShowSettings& CurrSetting = ShowSettings[SI];

		CurrSetting.bOverallMode_ExportedCamera = false;
		CurrSetting.bOverallMode_ControlledMatinee = false;
		CurrSetting.bOverallMode_ManualCamera = false;
		CurrSetting.bOverallMode_ExportedOrManualCamera = false;
		
		switch (OverallMode)
		{
		case EStageEventDirectorOverallMode::ESEDOM_ExportedCamera:
			CurrSetting.bOverallMode_ExportedCamera = true;
			CurrSetting.bOverallMode_ExportedOrManualCamera = true;
			break;
		case EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee:
			CurrSetting.bOverallMode_ControlledMatinee = true;
			break;
		case EStageEventDirectorOverallMode::ESEDOM_ManualCamera:
			CurrSetting.bOverallMode_ManualCamera = true;
			CurrSetting.bOverallMode_ExportedOrManualCamera = true;
			break;
		}

		CurrSetting.bPlayActorType_WaveMob = (CurrSetting.PlayActorType == EStageEventPlayActor::ESEE_WaveMob);

		CurrSetting.bUsePlayActorIndex = (CurrSetting.IsTeamPlayActorType() || CurrSetting.IsRaidPlayActorType());
	}

	// 捞巴档 咯扁 尝况持贬.
	bDirectingEvent_MobEvent = ((DirectingEvent == EStageEvent::ESEV_MobAppear) || (DirectingEvent == EStageEvent::ESEV_MobDying));
	bDirectingEvent_MobDying = (DirectingEvent == EStageEvent::ESEV_MobDying);
	bDirectingEvent_MobPhase = (DirectingEvent == EStageEvent::ESEV_MobPhase);
	bDirectingEvent_ComponentTrigger = (DirectingEvent == EStageEvent::ESEV_ComponentTrigger);
}

void AB2StageEventDirector::PostEditUndo()
{
	Super::PostEditUndo();
}
void AB2StageEventDirector::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished)
	{
		// Preview 哪器惩飘甫 捞悼矫淖阑 荐 乐栏聪 困摹何磐 教农
		SyncFromPreviewSKCompPlacement();
		// 咯扁辑绰 RegenerateSKComp 甫 窍瘤 臼绰促. PostEditMove 惑炔俊辑绰 迭洒 regenerate 甫 且 鞘夸档 绝绊, 公均焊促 概 捞悼矫付促 急琶 器目胶甫 酪绢滚府搁 阂祈窍扁 锭巩.
		RefreshPreviewSKComps(false);
	}
}

void AB2StageEventDirector::CheckAndWarnForLightSetup()
{
//	if (!MatineeActor || !MatineeActor->MatineeData)
//		return;
//
//	const int32 AllowedMatineeTrackPointLightNum = GetAllowedMatineeTrackPointLightNum();
//
//	int32 LightCount_PossibleManagedGroup = 0;
//	int32 LightCount_NoneManagedGroup = 0;
//
//	for (int32 GAI = 0; GAI < MatineeActor->GroupActorInfos.Num(); ++GAI)
//	{
//		FInterpGroupActorInfo& ThisGAI = MatineeActor->GroupActorInfos[GAI];
//		for (AActor* ThisGroupActor : ThisGAI.Actors)
//		{
//			APointLight* pLight = Cast<APointLight>(ThisGroupActor);
//			if (pLight && pLight->GetLightComponent() && pLight->GetLightComponent()->Mobility == EComponentMobility::Movable &&
//				pLight->GetLightComponent()->bAffectsWorld)
//			{
//				UInterpGroup* GroupOfThisInfo = GetInterpGroupOfObjectName(ThisGAI.ObjectName);
//				// 角龙利栏肺 Light 甫 殿废茄 InterpGroup 篮 何葛 Folder Group 匙捞怪栏肺 On/Off 甫 茄促.
//				UInterpGroup* ParentFolderGroup = FindParentGroupFolder(GroupOfThisInfo, MatineeActor);
//
//				if (ParentFolderGroup && IsPossibleInterpGroupNameForPC(ParentFolderGroup->GroupName.ToString()))
//				{
//					++LightCount_PossibleManagedGroup; // 捞吧肺 歹秦柳 巴甸 吝俊绰 酒付 窜 窍唱父 难龙 巴栏肺 抗惑..
//				}
//				else
//				{
//					++LightCount_NoneManagedGroup;
//				}				
//			}
//		}
//	}
//	const int32 TotalLightCount = LightCount_NoneManagedGroup + LightCount_PossibleManagedGroup;
//	// LightCount_PossibleManagedGroup 档 技泼俊 蝶扼辑 钧蹲窍霸 技泼登搁 笛 捞惑 难龙 荐 乐绰单.. 弊巴鳖瘤绰 歹 捞惑 给窍摆绊..
//	const int32 TotalLightCount_TurnOnTogether = (LightCount_NoneManagedGroup + (LightCount_PossibleManagedGroup > 0 ? 1 : 0));
//
//	if (TotalLightCount > AllowedMatineeTrackPointLightNum)
//	{
//		if (TotalLightCount_TurnOnTogether > AllowedMatineeTrackPointLightNum)
//		{
//			// 角力肺 巩力啊 瞪父茄 惑炔.
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(
//					TEXT("%s 俊辑 力绢窍绰 PointLight 啊 葛官老 坊歹傅狼 傈眉 力茄阑 逞辨 巴栏肺 抗惑邓聪促. 促澜 汲沥甸阑 犬牢秦 林技夸.\r\n\r\n")
//					TEXT("1. HidePlayerPointLight : %d %s\r\n")
//					TEXT("2. AllowOtherPlayerPointLight : %d\r\n")
//					TEXT("3. Matinee 俊 殿废茄 PointLight 飘发捞 狼档措肺 力绢登绰瘤 犬牢秦 林技夸.\r\n")
//					TEXT("  - 急喊利 On/Off 力绢啊 登绰 弊缝俊 殿废等 PointLight 俺荐  : %d\r\n")
//					TEXT("  - 急喊利 On/Off 力绢啊 救 登绰 弊缝俊 殿废等 PointLight 俺荐 : %d"),
//					*GetName(), bHidePlayerPointLight, (bHidePlayerPointLight ? TEXT("") : TEXT("(漂洒 捞 蔼捞 0 捞搁辑 捞 版绊啊 哆搁 困氰钦聪促)")), 
//					bAllowOtherPlayerPointLight, LightCount_PossibleManagedGroup, LightCount_NoneManagedGroup)
//			));
//#endif
//		}
//	}
}
//
//UInterpGroup* AB2StageEventDirector::GetInterpGroupOfObjectName(FName InFindGroupObjectName)
//{
//	// FInterpGroupActorInfo 狼 ObjectName 阑 啊瘤绊 茫绊磊 窍绰 芭.
//	if (MatineeActor && MatineeActor->MatineeData)
//	{
//		for (UInterpGroup* ThisGroup : MatineeActor->MatineeData->InterpGroups)
//		{
//			if (ThisGroup && ThisGroup->GetFName() == InFindGroupObjectName)
//			{
//				return ThisGroup;
//			}
//		}
//	}
//	return nullptr;
//}

void AB2StageEventDirector::CheckAndWarnForComponentTriggerSetup(UWorld* InWorld)
{
//	if (!InWorld) {
//		return;
//	}
//
//	// 漂洒 磊林 角荐秦辑 滚弊甫 具扁窍绰 汲沥捞扼 漂喊洒.. 傈眉 StageEventDirector 倒搁辑 巩力 汲沥 葛酒辑 版绊 剁框.
//	// 各捞 殿厘秦档 ComponentTrigger 肺 荤侩 啊瓷茄 版快啊 乐变 且搏单 弊巴焊窜 角荐啊 逞 腹匙..
//	
//	TArray<AB2StageEventDirector*> FoundProblematicSED;
//
//	for (FActorIterator ActorIt(InWorld); ActorIt; ++ActorIt)
//	{
//		AB2StageEventDirector* CastedSED = Cast<AB2StageEventDirector>(*ActorIt);
//
//		if (CastedSED && CastedSED->DirectingEvent == EStageEvent::ESEV_ComponentTrigger)
//		{
//			bool bHasMobSetting = false;
//			for (int32 SI = 0; SI < CastedSED->ShowSettings.Num(); ++SI)
//			{
//				if (CastedSED->ShowSettings[SI].PlayActorType == EStageEventPlayActor::ESEE_WaveMob)
//				{
//					bHasMobSetting = true;
//					break;
//				}
//			}
//			if (bHasMobSetting)
//			{
//				FoundProblematicSED.Add(CastedSED);
//			}
//		}
//	}
//
//	if (FoundProblematicSED.Num() > 0)
//	{
//#if !PLATFORM_MAC
//		//FString WarnMsg = TEXT("Wave 各捞 殿厘窍绰 促澜 StageEventDirector 咀磐甸俊 ComponentTrigger 矫累 汲沥捞 登绢 乐嚼聪促. 捞繁 汲沥篮 后锅窍霸 稗绰 胶抛捞瘤 楷免 柳青 滚弊甫 具扁钦聪促. 楷免 矫累 困摹绰 扁裙磊客狼 蛆狼 窍俊 SpawnPool 阑 烹秦 力绢窍芭唱 PCSyncToMatineeStart 扁瓷阑 劝侩窍技夸.\r\n\r\n");
//		FString WarnMsg = TEXT("Wave");
//		
//		for (int32 SI = 0; SI < FoundProblematicSED.Num(); ++SI)
//		{
//			AB2StageEventDirector* ThisOne = FoundProblematicSED[SI];
//			WarnMsg += FString::Printf(TEXT("%d. %s\r\n"), SI+1, *ThisOne->GetName());
//		}
//
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMsg));
//#endif
//	}
}
bool AB2StageEventDirector::HasAnyDirectedFromBeginningTrackInCurrentWorld(UWorld* InWorld)
{
	for (FActorIterator It(InWorld); It; ++It)
	{
		AB2StageEventDirector* CastedSED = Cast<AB2StageEventDirector>(*It);
		if (CastedSED)
		{
			if (CastedSED->HasDirectedFromBeginningTrack())
			{
				return true;
			}
		}
	}
	return false;
}
#endif
#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
bool AB2StageEventDirector::bDisableStageEventLights = false;
#endif