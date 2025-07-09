// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#include "BladeII.h"
#include "Camera/CameraComponent.h"
//#include "Matinee/InterpData.h"
//#include "Matinee/InterpGroup.h"
//#include "Matinee/InterpGroupInst.h"
//#include "Matinee/InterpTrackMove.h"
//#include "Matinee/InterpTrackSound.h"
//#include "Matinee/InterpGroupDirector.h"
//#include "Matinee/InterpTrackDirector.h"
//#include "Matinee/InterpTrackEvent.h"
////#include "Matinee/MatineeActor.h"
//#include "Matinee/InterpTrackAnimControl.h"
#include "Animation/SkeletalMeshActor.h"
#include "Engine/SkeletalMesh.h"
#include "Engine.h"
#if WITH_EDITOR
#include "Editor.h"
#include "FileHelpers.h"
#include "Components/TextRenderComponent.h"
#endif
#include "B2StageEventDirector.h"
#include "B2StageManager.h"
//#include "B2MonsterSpawnPool.h"
#include "BladeIIGameMode.h"
#include "B2StageGameMode.h"
#include "B2NetGameMode.h"
//#include "B2PVPGameMode.h"
//#include "B2TMGameMode.h"
//#include "B2RaidGameMode.h"
#include "BladeIIPlayer.h"
#include "BladeIIPlayerController.h"
//#include "BladeIINetPlayer.h"
//#include "Camera/PlayerCameraManager.h"
//#include "Camera/CameraAnim.h"
//#include "Camera/CameraAnimInst.h"
//#include "B2SkillRTManager.h"
#include "B2PCClassInfo.h"
#include "B2PCClassInfoBox.h"
#include "B2NPCClassInfo.h"
//#include "B2NPCClassInfoBox.h"
//#include "B2ActiveCameraActor.h"
//#include "BladeIIWorldSettings.h"
//#include "B2UIGuildRound.h"
#include "Event.h"
//#include "B2UIManager.h"
//#include "B2UIBattleMain.h"
//#include "B2UIPVP1on1BattleMain.h"
//#include "B2UITeamMatchBattle.h"
#include "B2UIDocHelper.h"
//#include "B2UIStageResult.h"
//#include "B2UIPVPBegin.h"
//#include "B2UIHeroTowerResult.h"
#include "Particles/Emitter.h"
#include "BladeIIScalabilityControl.h"
//#include "B2PreRenderer.h"
//#include "B2TutorialGameMode.h"
#include "B2AutoWayPoint.h"
#include "FB2ErrorMessage.h"
#include "BladeIIBlockToSyncNetwork.h"

// 그냥 CachedStageManager 를 체크하면 StageManager 가 없는 게임모드에서 바로 걸림
#define SAFE_STAGEMANAGER_CHECK BII_CHECK( !(Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(this))) || CachedStageManager );

ABladeIICharacter* FPlayActorShowRuntimeState::GetPlayActor(class AB2StageEventDirector* OwnerSED)
{// LocalPlayerCharacter 를 캐싱해서 쓰다가 문제가 된 적이 좀 있어서 안전 차원에서.. 근데 다른 PlayActor 도 그럴 수 있을 법 한데.. 체크라도 걸어볼까나..
	ABladeIICharacter* RetChar = NULL;// bIsForLocalPlayerCharacter ? Cast<ABladeIICharacter>(UGameplayStatics::GetLocalPlayerCharacter(OwnerSED)) : ThePlayActor;
	// PlayActor 는 NULL 일 수 있다. 예를 들어 다수의 스테이지를 위한 ShowSetting 인데 SpawnPool 에서는 어떤 스테이지에서 몹이 덜 나온다거나 하는 경우. 따라서 validity 체크만 하도록.
	//ensure(!RetChar || RetChar->IsValidObj()); // 연출 도중에 Directed 가 아닌 (PlayActorStateMode) PlayActor 가 destroy 되어버린다면 IsValidObj 가 아닐 수 있다. 현재로선 세팅에 따라 가능성이 있긴 함..
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
	{// 죽어가는 애라도 IsValidObj 여야..
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
{ // 잊고자 하는 거니 굳이 valid 검사까지는 ㅋ
	if (InMobToForget == TheDyingMob)
	{
		ForgetIt();
	}
}
ABladeIICharacter* FDelayedDyingMobCache::GetIfMatch(int32 InWaveIndex, int32 InObjIndex)
{
	if (IsValid(TheDyingMob ) && InWaveIndex == WaveIndex && InObjIndex == ObjIndex) 
	{ // 죽어가는 애라도 IsValidObj 여야..
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
	//// 에디터에서 맵 열 때 여기로 오는 경우가 있어서 에디터는 check 스킵
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
				: SpriteTexture(TEXT("/Engine/EditorResources/SceneManager")) // SceneManager 텍스쳐가 뭔가 연출스럽게 생김 ㄷㄷ
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

	// ManualCameraSettings 하나는 기본으로 넣어준다.
	ManualCameraSettings.Add(FManualCameraSingleSetting());

	// 카메라 생성. 상세한 위치 등의 조절은 하위 BP 클래스 뷰포트나 레벨 뷰포트에서 하긔
	DirectorCamera_0 = CreateDefaultSubobject<UCameraComponent>(TEXT("DirectorCamera_0"));
	DirectorCamera_0->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	DirectorCamera_0->bAutoActivate = false;
	DirectorCamera_0->SetRelativeLocation(FVector(50.0f, -100.0f, 100.0f));
	DirectorCamera_0->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	DirectorCamArrayRef[0] = DirectorCamera_0; // 단지 처음부터 array 로 선언하고 에디터에서 편집하도록 노출이 안돼서 이렇게 함.
	
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


#if WITH_EDITOR
	SetEditConditionProperties();

	// 혹시라도 레벨 이름 바꾸면 redirector 처리가 안되므로 여기서도 검사.
	UWorld* TheWorld = GetWorld();
	LevelStreamingOnBegin.EditorCheckForProperSetting(TheWorld);
	LevelStreamingOnEnd.EditorCheckForProperSetting(TheWorld);

	CheckAndWarnForLightSetup();
#endif
}

void AB2StageEventDirector::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
#if WITH_EDITOR
	// 처음에 이걸 PostLoad 에서 하려고 했는데 Preview 용 컴포넌트들의 땅바닥 스냅핑이 로딩 직후에 안먹혀서 여기다 넣음.
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
	//	// MAX_DIRECTOR_CAMERA 줄이면서 이전에 기본 생성되어 있던 거 제거.
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

	//// 설정에 따라 (WaveMob 사용하는 경우) 이게 불리기 전에 BeginShow 가 될 수도 있음을 명심.

	//// ESEV_ComponentTrigger 용으로 지정한 컴포넌트의 BeginOverlap 등록
	//if (DirectingEvent == EStageEvent::ESEV_ComponentTrigger && TriggerComponentName != NAME_None)
	//{
	//	UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(FindComponentByName(TriggerComponentName));
	//	if (PrimComp)
	//	{
	//		PrimComp->OnComponentBeginOverlap.AddDynamic(this, &AB2StageEventDirector::SelectedComponentTriggerBeginOverlapCB);
	//	}
	//}

	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && HasDirectedFromBeginningTrack() && MatineeActor && 
	//	!bCurrentlyPlaying)// 처음부터 플레이하는 거라면 이런 처리가 필요하지도 않겠지..
	//{
	//	MatineeActor->InitInterp(); // GetGroupActorOfGroupName 이전에 필요..
	//	for (int32 SI = 0; SI < ShowSettings.Num(); ++SI)
	//	{
	//		FPlayActorShowSettings& CurrSS = ShowSettings[SI];
	//		if (CurrSS.PlayActorStateMode == EPlayActorStateMode::EPASM_DirectedFromBeginning &&
	//			// 로컬 플레이어 캐릭터 용 트랙이면 이렇게 처음부터 고정된 위치에 존재하도록 하는 게 말이 되지 않고..
	//			// 기타 다른 타입의 플레이어 캐릭터라면 이 시점에서는 가능한 이름 조합을 모두 찾아내야 할 것. PlayActor 가 있는 시점이 아니므로.
	//			// 여하간 지금은 그런 경우 지원 불가.
	//			!IsPlayActorTypeForPlayerCharacter(CurrSS.PlayActorType)
	//			)
	//		{
	//			// 플레이어 캐릭터가 아니니 MatchingInterpGroupName 을 그대로 사용.
	//			ASkeletalMeshActor* ThisGroupSKActor = Cast<ASkeletalMeshActor>(GetGroupActorOfGroupName(CurrSS.MatchingInterpGroupName));
	//			USkeletalMeshComponent* SKMeshComp = ThisGroupSKActor->GetSkeletalMeshComponent();
	//			if (ThisGroupSKActor && SKMeshComp)
	//			{ // 처음부터 보이도록 켜 준다. 애니메이션도 아마 플레이 상태여야겠지..? 그게 아닌 경우까지 있다면 결국 추가 옵션을 주어야.
	//				if (SKMeshComp->AnimationData.AnimToPlay){
	//					// AnimToPlay 세팅된 걸 사용하니 AnimToPlay 를 비우지 못하겠는데..
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
	// 중복해서 불릴 수 있다.

	SAFE_STAGEMANAGER_CHECK
	if (CachedStageManager)
	{
		// 이 시점에서 StageManager 는 필요한 값을 들고 있어야 한다. (아마도 ABladeIIGameMode::PreBeginPlay 시점에)
		CurrentStageId = CachedStageManager->GetCurrentClientStageId();
		CurrentStageDifficulty = CachedStageManager->GetStageDifficultyLevel();
	}

#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
	// 인스턴스 및 스테이지 별 save state 읽어들임. 읽어들인 변수와 사용자 설정에 따라 skip 을 사용하게 될 수 있다.
	GConfig->GetBool(*GetPlayedAtLeastOnceSectionString(), *GetPlayedAtLeastOnceKeyString(CurrentStageId), bPlayedAtLeastOnce, GB2LocalCutSceneSavedStateIni);
#endif
}

int32 AB2StageEventDirector::GetAllowedMatineeTrackPointLightNum() const
{
	TConsoleVariableData<int32>* MobileNumDynamicPointLightsCVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.MobileNumDynamicPointLights"));
	const int32 MobileNumDynamicPointLights = MobileNumDynamicPointLightsCVar ? MobileNumDynamicPointLightsCVar->GetValueOnGameThread() : 2;
	// 플레이어 point light 를 사용할꺼면 그거 빼야.
	// 여기서 bAllowOtherPlayerPointLight 가 추가로 들어갈 껀데.. 그것도 일단 1 로 치자..
	return bHidePlayerPointLight ? MobileNumDynamicPointLights : (bAllowOtherPlayerPointLight ? (MobileNumDynamicPointLights - 2) : (MobileNumDynamicPointLights - 1));
}

void AB2StageEventDirector::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	
	if (GIsEditor && GIsPlayInEditorWorld == false)
	{
		SafetyProcessMatineeSkActors(); // BeginPlay 에서 하느니 에디터 전용으로 여기서 하는 게 낫겠지..?

		SyncFromPreviewSKCompPlacement(); // 프리뷰 컴포넌트로 편집한 위치 싱크
	}
#endif
}

void AB2StageEventDirector::BeginDestroy()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::BeginDestroy"));

	Super::BeginDestroy();

	// ESEV_ComponentTrigger 용으로 지정한 컴포넌트의 BeginOverlap 등록 해제
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
	
	// 결국 독자적인 Tick 루틴으로..
	if (IsPlayingShow())
	{
		if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
		{
			for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
			{
				FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
				FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];

				// BeginSingleShow 에서 하는 일부 반복.
				if (CurrShowSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_ContinuouslyAdjustToPlayActor)
				{
					// Position 만 따라가도록.. 하려고 했는데 뭔가 잘 안되는군
					AdjustSelfPlacementToPlayActor(CurrShowState.GetPlayActor(this), CurrShowSetting.ManualRelativeLocation, CurrShowSetting.ManualRelativeRotation);

					// ExportedCamera 모드에서는 ExportedEventScene 에 단지 초기 위치만 있으므로 SetSESCameraMovementFromPlayActor 를 매 프레임 한다고 해도 의미 없음.

					break; // 하나만 처리한다. 여러개 세팅이 있는 건 PostEditChangeProperty 에서 경고함
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


	//// DirectingEvent 가 ESEV_ComponentTrigger 인 경우 시작 처리.
	//// 굳이 검사를 안 해도 되겠지만 예의상
	//if (DirectingEvent == EStageEvent::ESEV_ComponentTrigger && TriggerComponentName != NAME_None &&
	//	bTargetIsLocalCharacter && // 물론 현재 플레이하는 캐릭터에 대해서만 + 탑승중이면 탈것으로 비교
	//	SweepResult.GetComponent() == Cast<UPrimitiveComponent>(FindComponentByName(TriggerComponentName)) &&
	//	!bDirectingEventConditionMet)
	//{
	//	bDirectingEventConditionMet = true;
	//	BeginShowOrBeginCountDown();
	//}
}

void AB2StageEventDirector::CustomBeginShow()
{
	BeginShow(); // 일단은 단순 call. 조건이라면.. BeginPlay 이후에만 가능 ㅋ
}

void AB2StageEventDirector::CustomFinishShow()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::CustomFinishShow"));

	//// 현재 플레이 도중인지 상관없이 불릴 수 있어야 함. FinishShow 에서 처리하는 것도 좀 있음.

	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && bPCSyncToMatineeStart && bCurrentPCSyncToMatineeStart)
	//{// 마티니 PCSync 도중이라면 실질적인 시작조차 하지 않은 것이라 시작을 해 놓아야 함.
	//	BeginMatineeFromPCSyncToMatineeStart();
	//}

	//for (int32 SI = 0; SI < ShowStates.Num(); ++SI)
	//{
	//	FPlayActorShowRuntimeState& CurrState = ShowStates[SI];
	//	
	//	if (CurrState.bShowStarted == false)
	//	{
	//		// 시작 대기 타이머가 켜진 상태일 수 있으므로 제거
	//		GetWorldTimerManager().ClearTimer(CurrState.ShowStartDelayTimerHandle);
	//	}
	//}

	//// 기본적으로는 자동으로 Stop 이 불림.
	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor)
	//{
	//	if (MatineeActor->bIsPlaying)
	//	{
	//		MatineeActor->Stop(); // 도중에 강제종료하는 거라면 이걸 통해서 또다시 CustomFinishShow 가 실행될 것.
	//		return;
	//	}
	//}

	//APlayerCameraManager* CamManager = UGameplayStatics::GetLocalPlayerCameraManager(this);
	//if (CamManager)
	//{
	//	CamManager->StopAllCameraAnims();
	//}

	//// 마티니의 경우 도중에 끝날 수도 있으니 플레이를 끝으로 세팅해서 위치 싱크 같은 것이 의도한 대로 동작하도록 함.
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
//	if (GCheat_CompleteIgnoreEventScene) // 이건 아예 시작조차 하지 않는 치트이다. 게임 플레이에 지장이 갈 수 있으며 특별한 테스트를 위해 사용한다. 일반적인 연출 장면 통과는 GCheat_SkipEventSceneCount 을 참고.
//	{
//		return false;
//	}
//#endif
//
//	// 잘못된 세팅의 경우
//	if ((OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor == NULL) ||
//		(OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera && CameraAnim == NULL))
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		if (GIsEditor)
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] %s 에 현재 Overall Mode 에 맞는 적절한 메인 에셋이 설정되어 있지 않습니다. Stage Event 를 실행하지 않습니다."), *this->GetName())
//				));
//		}
//#endif
//		return false;
//	}
//	/*
//	// 연결이 끊긴 경우
//	if (!FBladeIIBlockToSyncNetwork::GetInstance().IsConnectedToServer()
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		&& !(CachedStageManager && CachedStageManager->DevOnlyIsStandalonePlay()) // 에디터 프리뷰 플레이, 명시적으로 Standalone 모드로 플레이하는 경우 등. 연결 여부 상관없이 지속.
//#endif
//		)
//	{
//		return false;
//	}
//	*/
//
//	// ManageModeLeastSkip 은 현 계획상 없다. Skip 하는 게 아니라 그냥 쌩까는 것만.
//	if (CheckSkipForManageMode(this, ManageMode, false /*bManageModeLeastSkip*/) == EStageEventSkipType::SEST_CompleteIgnore){ // 여길 통과해도 다시 검사하게 될 것.
//		return false;
//	}
//
//	if (IsStageSupported(CurrentStageId, CurrentStageDifficulty) == false){
//		return false;
//	}
//	
//	if (bPlayedOnceForThisStage){ // 어떻게 하면 한 스테이지 안에서 여러번 플레이가 가능할지도 모르겠지만 굳이 그럴 필요가.. 괜히 위험할 수도 있으니 원천 봉쇄.
//		return false;
//	}
//
//	// 옵션에 따른 skip 같은 건 여기서 return false 하는 식으로 처리하지 않는다. BeginShow 직후 바로 CustomFinishShow 를 하는 식으로 처리해야 할 것.
//
//	return true;
//}
//
//// 월드 재로딩 없이 Event 장면을 다시 플레이 가능하도록. 모두 동일한 네이밍.
//void AB2StageEventDirector::ResetAssumeNoWorldLoading()
//{
//#if !UE_BUILD_SHIPPING
//	CheckResetAssumeNoWorldLoadingCondition(this);
//#endif
//
//	// 아직 플레이 안 된 걸 굳이 긁어 부스럼 만들 필요 없지. 실제로 좀 부작용이 발견되었음.
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
//	// 플레이 안 된 걸 SetPosition(0) 했을 때 간혹 부작용이 있던 걸로 봐서 이거 자체가 완벽한 리셋 수단은 아닌 듯 하지만 대체로 먹힘.
//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor && MatineeActor->MatineeData)
//	{
//		MatineeActor->SetPosition(0.0f);
//
//		// 선택적으로 등잗하는 트랙의 경우 비활성화된 것이 있을 것이므로 모두 enable .. 하면 처음부터 disable 한 것도 enable 되겠지? 이 기능을 쓰는 레벨에선 조심해서 사용해야..
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
	if (InManageMode == EStageEventManageMode::SEMM_Always){
		return EStageEventSkipType::SEST_Play;
	}

	AB2StageGameMode* StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	AB2StageManager* StageMgr = StageGM ? StageGM->GetStageManager() : NULL;

	const bool bIsStageScenarioMode = StageMgr ? StageMgr->IsScenarioMode() : false;

	if (IsAdventureModeGame(WorldContextObject))
	{
		if ((InManageMode == EStageEventManageMode::SEMM_ScenarioOnly && bIsStageScenarioMode) ||
			(InManageMode == EStageEventManageMode::SEMM_RegularStageOnly && !bIsStageScenarioMode) ||
			InManageMode == EStageEventManageMode::SEMM_AlwaysForAdventure)
		{
			return EStageEventSkipType::SEST_Play;
		}
	}

	// bLeastSkip 에 따른 분기는 StageEventDirector 의 내부적인 동작 차이. 본 기능이 작동하지 않는 건 마찬가지. 다른 곳에서 사용할 때 비슷한 특별한 사정이 있지 않은 이상 별 의미는 없을 것.
	// -> 2017.03 기준 기획으로는 LeastSkip 은 없고 사용 안되는 거는 CompleteIgnore 일 뿐. 남겨만 둠.
	return bLeastSkip ? EStageEventSkipType::SEST_LeastSkip : EStageEventSkipType::SEST_CompleteIgnore;
}

bool AB2StageEventDirector::IsAdventureModeGame(UObject* WorldContextObject)
{
	AGameMode* CurentGM = Cast<AGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// 추후 StageGameMode 에서 상속받은 다른 게임모드가 들어왔을 때 추가 검사. 일반 스테이지 플레이, 즉 "모험" 모드 여부를 보고자 함.
	return (Cast<AB2StageGameMode>(CurentGM) != NULL);
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
{ // 게임모드 플레이 도중 실제 플레이가 예상되는지 여부.
	return IsStageSupportedForGameMode(InB2GM) && (CheckSkipForManageMode(InB2GM, ManageMode, false) == EStageEventSkipType::SEST_Play);
}
bool AB2StageEventDirector::IsStageSupportedForGameMode(class ABladeIIGameMode* InB2GM) const
{
	// 이게 True 여도 실제 플레이는 안 될 수 있다. 그 가능성을 보다 확실하게 체크하려면 IsExpectedForGameMode 를 봐야.

	AB2StageGameModeBase* CastedSGM = Cast<AB2StageGameModeBase>(InB2GM);
	AB2StageManager* StageManagerIfAny = CastedSGM ? CastedSGM->GetStageManager() : NULL;
	if (StageManagerIfAny)
	{
		return IsStageSupported(StageManagerIfAny->GetCurrentClientStageId(), StageManagerIfAny->GetStageDifficultyLevel());
	}
	return true; // StageGameMode 가 아닌 경우는 기본 지원되는 걸로 본다. 별도 스테이지 개념이 없이 기본 동작 취급인 것.
}
bool AB2StageEventDirector::IsStageSupported(int32 InStageId, EStageDifficulty InDifficulty) const
{
	return IsStageNumSupported(InStageId) && IsStageDifficultySupported(InDifficulty);
}

bool AB2StageEventDirector::IsStageNumSupported(int32 InStageId) const
{
	if (SupportedStages.Num() == 0)
	{
		return true; // 비어 있는 경우 기본으로 모든 스테이지 지원
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
		return true; // 비어 있는 경우 기본으로 모든 난이도 지원
	}
	const int32 TestDifficultyInt = static_cast<int32>(InDifficulty); // 왜 어디선 EStageDifficulty 를 씨고 어디선 Int 를 쓰고 했는지 잘 기억이 안남. 걍 정신없어서 그런 듯
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
	//		// 실질적으로 스테이지 시작 연출을 MobAppear 로 하는지라 이걸 판별할 수 있는게 EventWidgetType 임 ㅡ.ㅡ;
	//		(DirectingEvent == EStageEvent::ESEV_MobAppear && EventWidgetType == EStageEventWidgetType::StageBegin) || 
	//		// 근데 이젠 걍 처음으로 나오는 이벤트 씬인지 여부로..ㅋ
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
//		// 튜토리얼때 QTE 상태로 바로 마티니 진행
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
//		// <--QTE중이면 콜백 예약하고 리턴
//		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerController(this)->GetPawn());
//
//		if (pPlayer && pPlayer->IsInQTEState())
//		{
//			pPlayer->AddQTEOnEndCallBack(FQTEOnEnd::CreateLambda([this]() { this->BeginShow(); }));
//
//			// 탈것 QTE면 내리기
//			if (pPlayer->QTEType == EQTEType::EQT_Mount)
//				pPlayer->StopQTEMount();
//
//			// 예외처리 쇼세팅 기반으로 숨기는건 미리 한번 해줌.
//			if (DirectingEvent == EStageEvent::ESEV_MobAppear)
//			{
//				// Settings 와 RuntimeState 숫자를 맞춰준다.
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
//				// 플레이어는 안숨김
//				pPlayer->SetHideFromEventDirector(false);
//			}
//
//			return;
//		}
//
//		// -->QTE중이면 콜백 예약하고 리턴 끝
//	}
//	
//	B2_SCOPED_TIME_LOG(FString::Printf(TEXT("AB2StageEventDirector::BeginShow %s"), *GetName()));
//
//	bEnterCinematicMode = false;
//
//	// 나빼고 다 스톱!
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
//	// BP 에서 특별히 이벤트를 받아서 처리하고자 한다면..
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
//		// StageClear UI 가 둘로 나뉘고 기존 Clear 에서의 승리포즈 연출은 UIStageClear 후의 UIStageResult 와 함께 진행되므로 처음에는 BeginShow 대신 별도 처리를 위한 신호를 보낸다.
//		// 이 Phase1 도 별도의 EStageEvent 로 할 수도 있지만 거의 UI 위주이므로 따로 처리해 보고 현 기능으로 요구사항 감당이 안된다면 추후 EStageEvent 도 마찬가지로 둘로 나눠볼 수 있다.
//		
//		MyRelevantUIName = UIFName::StageClear; // 이건 EventWidgetType 체크 없이 좀 특수하게 세팅되는데 Phase1 자체가 사실상 StageClear UI 로만 이루어지는 걸 감안.
//		BeginStageClearScene_Phase1Class<>::GetInstance().Signal();
//
//		ApplyStageEventTimeDilation();
//		
//		// 여길 통해 세팅한 실질적인 최종 종료(승리) 연출 발동은 AB2StageManager::BeginStageClearScene_Phase2 참고.
//		// 다시 들어올 때는 BeginShow 를 하도록 ClearEventPhase 세팅.
//		ClearEventPhase = EStageClearEventPhase::ESCE_Phase2;
//
//		if (LocalB2Player)
//		{ // bForceStopPlayerAttackState 에서 하는 처리인데 리턴을 해 버리니 여기서 따로.. ㅡㅡ
//			LocalB2Player->SignalStopAttackImmediate();
//		}
//
//		return;
//	}
//		
//	if (ShouldApplyCinematicScalabilityDuringShow())
//	{
//		if (LocalB2Player) {
//			LocalB2Player->SetupLODInfoForCinematic(); // 로컬 플레이어는 연출 장면 일부가 아니더라도 카메라워크에 의해 스쳐갈 수도 있으니 기본 cinematic 용으로 세팅
//		}
//		B2Scalability::ApplyStageEventSpecificScalabilitySetting(this, true); // 로컬 플레이어 LOD 세팅과 마찬가지로 연출 용 특별 scalability 컨트롤 적용
//	}
//
//	// Settings 와 RuntimeState 숫자를 맞춰준다.
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
//		CurrentActiveCamera = LocalB2Player->TopDownCameraComponent; // 아마도 이걸 유지하지는 않을 것이다..
//
//		if ((DirectingEvent != EStageEvent::ESEV_MobDying && DirectingEvent != EStageEvent::ESEV_FinishAttack))
//		{
//			/*
//			// 연출 전 공격중이라도 연출 끝 시점에 공격을 이어가지 않아야 함
//			if (LocalB2Player->IsKeepAttacking() && bForceStopPlayerAttackState) // bForceStopPlayerAttackState 조건을 추가로 검사.. 원래 이쪽 목적이 아닌긴 했는데 대략 맞는다.
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
//	// 카메라 세팅부터
//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera)
//	{
//		APlayerCameraManager* CamManager = UGameplayStatics::GetLocalPlayerCameraManager(this);
//		if (CamManager && CameraAnim)
//		{
//			// 일단 모두 deactivate 를 해 놓고..
//			DeactivateAllManualCams();
//			
//			// 이전에 활성화 된 것이 있다면 모두 종료. 그런데 사실 이렇게 해도 show 도중에 플레이 하는 애니메이션에 카메라 애님이 붙어 있다면 그게 적용이 될 수 있음.
//			// 따라서 SetStageEventShowState 에 의해 들어가는 AnimBP 의 해당 애니메이션을 적절히 세팅해야 함.
//			CamManager->StopAllCameraAnims(true); 
//
//			// CameraAnimApplyCamIndex 에 따라 기본 카메라 및 view target 액터 선택
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
//			// 의도가 플레이어 쪽에서부터 블렌딩 되는 거라면 다시 켜주긴 해야 함.
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
//				LocalB2PC->SetViewTargetWithBlend(CameraAnimViewTarget, CameraAnimInitialBlendingTime, EViewTargetBlendFunction::VTBlend_Linear); // BlendFunction 필요하면 설정하게..
//
//			// 필요하면 CameraAnimInst 를 캐싱해서 사용.
//			UCameraAnimInst* CamAnimInst = CamManager->PlayCameraAnim(CameraAnim);
//			if (CamAnimInst)
//			{
//				if (bStandbyAtTheEnd)
//				{
//					CamAnimInst->bStayAtEnd = true; // 카메라 애니메이션 종료 상태에서 유지하게 될 것. 이 경우 끝내는 건 CustomFinishShow 를 사용.
//				}
//				CamAnimInst->AddCustomNotifyActor(this); // 종료 통지를 받기 위한 등록. 종료 시 NotifyActorCustomEvent 로.
//			}
//		}
//	}
//	else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//	{
//		// Controlled Matinee 의 경우는 하단 BeginSingleShow 를 delay 없이 처리한 후에 Play 를 할 것.
//
//		if (MatineeActor)
//		{
//			// MatineeActor 와의 첫 조우. 이쪽에 설정한 변수들을 넘겨줌
//			MatineeActor->FirstSkipFrameNum = MatineeFirstSkipFrames;
//			MatineeActor->bStayAtEnd = bStandbyAtTheEnd; // StandbyAtTheEnd 에서는 마티니 종료 상태에서 유지하게 될 것. 이 경우 끝내는 건 CustomFinishShow 를 사용.
//			MatineeActor->bNoCinematicModeControl = true; // StageEventDirector 에 연결한 이상 CinematicMode 는 여기서 조절함.
//			MatineeActor->SetPosition(0.0f, true); // 일부 재활용하는 마티니 씬이 있음. MatineeActor 쪽의 bForceStartPos 를 체크하면 되는 거긴 함.
//			MatineeActor->InitInterp(); // BeginSingleShow 를 통해 그룹 actor 들에 접근하기 전에 Init 을 수동으로 한번 해야 하는 듯.
//
//#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
//			if (bDisableStageEventLights) // 디바이스에서의 성능 테스트 용도임. 아마도 인게임에서 거의 유일하게 별도의 동적 라이트를 사용하는 부분일 듯?
//			{
//				SetAllPointLightGroupAffectWorld(false);
//			}
//			else
//#endif
//			{
//				// 라이트 세팅부터 켜 놓고.. 다시 필요없는 건 꺼야 함.
//				SetAllPointLightGroupAffectWorld(true);
//			}
//
//			SelectFinalDirectorTrack(); // bPCDependentDirectorTrack 이거나 Random 인 경우 등에서 여러 Director track 들이 있을 때 적절한 하나를 선택.
//		}
//	}
//	else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
//	{
//		check(ManualCameraSettings.Num() > 0); // 적어도 하나는 남도록 처리함.
//
//		// 일단 모두 deactivate 를 해 놓고..
//		DeactivateAllManualCams();
//
//		if (LocalB2Player && LocalB2PC)
//		{
//			// 여기도 사실상 플레이어 쪽에서부터 블렌딩 될 것이므로 확실히 켜 주자.
//			LocalB2Player->TopDownCameraComponent->Activate();
//			LocalB2PC->SetViewTargetWithBlend(LocalB2Player, 0.0f);
//		}
//
//		// 첫번째 설정한 카메라로 이동.		
//		BeginTargetToManualCamera(0);
//	}
//
//	// BeginSingleShow 에서 SimpleEventScene 세팅 시 사용.
//	// -> Director camera component 들은 움직이지 않는 걸로 친다. 그렇게 가정해야 하는 상황임. 따라서 위치값을 캐싱하지 않음.
//	//CachedInitialActiveCamLocation = CurrentActiveCamera->GetComponentLocation();
//	//CachedInitialActiveCamRotation = CurrentActiveCamera->GetComponentRotation();
//	
//	// 개별 play actor 별 show 시작
//	for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
//	{
//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];
//
//		// ControlledMatinee 에서는 StartDelay 없음.
//		if (CurrShowSetting.StartDelay > 0.0f && OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//		{
//			GetWorldTimerManager().SetTimer(CurrShowState.ShowStartDelayTimerHandle,
//				FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::DelayedShowStartTimerCB, EI),
//				CurrShowSetting.StartDelay, false);
//		}
//		else
//		{
//			// 즉석에서 시작
//			BeginSingleShow(EI);
//		}
//	}
//
//	// ControlledMatinee 는 BeginSingleShow 처리 후 Play 바로 시작.. 은 아니고 추가로 플레이어 싱크를 검사.
//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//	{
//		// 런타임 상황에 의해 보이지 말아야 할 GroupActor 의 경우, 보이지 않는데도 불구하고 소리가 나거나 할 수 있으므로 제거.
//		// 예를 들어 만들어는 놓았으나 현재 플레이어 클래스와 다르므로 나오지 말아야 할 그룹.
//		RuntimeRemoveNonDesiredGroup();
//		// RuntimeRemoveNonDesiredGroup 하면서 사용되지 않을 PointLight 끄는 게.. 예전에 이렇게 안했을 때 동작하던 셋업이 있어서 하나는 살려두는 의미로.
//		HackSpareLeastPointLight();
//
//		if (CheckForPCSyncToMatineeStart() == false) // true 라면 Player sync 후에 시작할 것.
//		{
//			FinalBeginMatineePlay();
//		}
//
//	}
//
//	// Widget 등 추가적인 처리가 있는데 이것들은 Matinee PCSync 사용시에는 본격 Matinee play 시작 후에 처리되어야 함.
//	if (bCurrentPCSyncToMatineeStart == false)
//	{
//		DeferredBeginShowHandlingForMatineePCSync();
//	}
//
//	if (bForceStopPlayerAttackState)
//	{
//		// 현재 플레이어 캐릭터가 게임상태를 유지하도록 명시된 경우면 물론 해선 안되지.
//		if (ShouldMaintainPlayerCharacterState() == false)
//		{
//			if (LocalB2Player)
//				LocalB2Player->SignalStopAttackImmediate(); // 이후 AnimBP 에 의해 처리될 것.
//		}
//	}
//
//	SAFE_STAGEMANAGER_CHECK
//	if (bStopAllMobsWhilePlaying && CachedStageManager)
//	{
//		CachedStageManager->StopOrRestoreAllWorldMobs(true); // 여기에 포함된 PlayActor 뿐 아니라 모두 정지.
//		if (bForceCancelAttackMotionAllMob)
//			CachedStageManager->ForceCancelAttackAllWorldMobs();
//	}
//
//	if (B2GM)
//	{// Auto 는 잠시 중단시키고 Finish 에서 복구
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
//			// Matinee 모드가 아닌 경우의 CinematicMode 세팅. 마티니에선 좀 특별 관리. 4번째 인자는 MovementInput 에 대한 거지만 현재로서는 전체 입력에 대해 영향.
//			LocalB2PC->SetCinematicMode(true, bHiddenPlayer, bHideHud, bDisableInput, true);
//		}
//		if(!bForceNoHidePlayer)
//			SetAllPlayerHiddenInGame(true);
//	}
//
//	// 시작 연출인 경우 여기에 걸릴 수 있다. Slomo 걸고 차양막 가리는 동안 사운드 발동을 끔
//	// RuntimeRemoveNonDesiredGroup 이후에 들어가야 함. 다른 이유로 disable 된 것들을 걸러내야 해서
//	if (B2GM && B2GM->IsInPreRenderPhase() && OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//	{
//		TemporaryDisableTracksForPreRender(); // 복구하는 건 FinishShow 에서 하는 게 아니라 GameMode 나 PreRender 쪽에서 처리해야.
//	}
//
//	LevelStreamingOnBegin.ExecuteStreaming(GetWorld(), LevelStreamingOnBeginFlushTH, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::LevelStreamingOnBeginFlushCB));
//	
//	// 플레이 도중 로딩으로 블로킹 걸릴 수 있는 꺼리를 적절한 시점에 슬쩍 로딩해 보고자 함.
//	if (IsPossibleAdventureModeBeginEvent())
//	{
//		UB2UIManager::GetInstance()->ReqAsyncLoadAsset(UIFName::BossStatus);
//		UB2UIManager::GetInstance()->ReqAsyncLoadAsset(UIFName::ScenarioDialog);
//	}
//
//	// 기타 다른 곳에서의 처리를 위해 (StageManager)
//	StageEventSceneBeginClass<AB2StageEventDirector*, EStageEvent>::GetInstance().Signal(this, DirectingEvent);
//
//#if BII_STATS
//	FStatisticalPerfStatCollector::bIsCurrentlyInEventScene = true;
//#endif
//
//	if(bCheckPlayOne)
//		bPlayedOnceForThisStage = true; // BeginShow 만 해도 한번 플레이 한 걸로 친다.
//	bCurrentlyPlaying = true;
//
//	//
//	// 정식 시작 절차는 끝났고, 아래는 스킵이 필요한 경우.
//	//
//
//	bool bShowSkipped = false;
//
//	// 정식 스킵을 하려면 최소한 BeginShow 는 한 후에 CustomFinshShow 를 해야 함. 연출 장면 종료 후 위치 싱크가 필요한 경우 때문에.
//	// -> 일단 이 경로로 가게 될 일은 없을 것.
//	if (CheckSkipForManageMode(this, ManageMode, false/*bManageModeLeastSkip*/) == EStageEventSkipType::SEST_LeastSkip){
//		CustomFinishShow();
//		bShowSkipped = true;
//	}
//
//#if !UE_BUILD_SHIPPING
//	extern int32 GCheat_SkipEventSceneCount;
//	if (GCheat_SkipEventSceneCount > 0 && !bShowSkipped){
//		if (DirectingEvent != EStageEvent::ESEV_Clear){ // Clear 이벤트는 게임 종료 메뉴를 위해 필요하다 보니.. 그래도 UI 메뉴만 바로 보여줄 수 있는 방법을 생각해 보자..
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

	// 전체 시간이 지정되었다면 종료 타이머 on. Controlled Matinee 의 경우도 적용은 되겠지만 마티니 전체 길이보다 길면 어차피 소용 없을 것.
	if (IsOverallLengthConstrainted())
	{
		GetWorldTimerManager().SetTimer(OverallLengthTimerHandle, this, &AB2StageEventDirector::OverallLengthTimerCB, SpecifiedOverallLength, false);
	}

	//ABladeIIGameMode* B2GM = GetB2GameMode();
	//if (B2GM && !B2GM->IsInPreRenderPhase()) // PreRender 상황에서는 매우 적은 TimeDilation 을 가하고 PreRender 끝날 때 이쪽 TimeDilation 을 복구할 것
	//{
	//	ApplyStageEventTimeDilation();
	//}
}

void AB2StageEventDirector::ApplyStageEventTimeDilation()
{
	//float FinalPlayRate = GlobalTimeDilation; // TODO 여기에 사용자가 선택한 play rate 가 곱해질 수 있음.
	//UGameplayStatics::SetGlobalTimeDilation(this, FinalPlayRate, true); // 여기서는 FinalPlayRate 를 여타 다른 요소를 감안하지 않은 실질 TimeDilation 으로 취급.
}

void AB2StageEventDirector::BeginShowOrBeginCountDown()
{
	//if (bDirectingEventConditionMet == false) // 기본적인 조건
	//{
	//	return;
	//}

	//// [게임플레이액터] 에서 [마티니액터] 로 전환되기 전, 마킹 처리할 웨이포인트가 있다면, 등록된 참조를 이용하여 처리.
	//if ( ForcedMarkPassedWaypoint )
	//{
	//	ForcedMarkPassedWaypoint->ForceMarkByJumpEvent( nullptr, Cast< ABladeIIPlayer >( UGameplayStatics::GetLocalPlayerController( this )->GetPawn() ) );
	//	if ( OverallStartDelay <= 0.0f )
	//	{
	//		OverallStartDelay += 0.1f;
	//	}
	//}

	//// DependentDirectorActor 가 설정된 경우에 대한 추가 조건 검사
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
			return true; // 단 하나만 들어가 있어도 통과
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
	//	// 막타 진동. 스테이지 모드 보스몹 진동은 아마 따로 들어갈 듯.
	//	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (B2Player)
	//	{
	//		B2Player->ForceFeedback();
	//	}

	//	// 가해자를 대상으로 상대좌표 마티니 연출 시작
	//	SetUpOutSideCharacter = Inflictor;
	//	BeginShow();
	//}
}

void AB2StageEventDirector::NotifySpawnPoolSingleWaveStart(int32 WaveNum)
{
	// 단일 레벨 내에서 사용되는 SpawnPool 은 일정하며 GameMode 에서 알고 있으면 됨. 여기에 SpawnPool 정보를 보내지 않는 걸로 함.

	//int32 ParsedWaveNum = 0;
	//int32 ParsedMobIndex = 0;
	//ParseForWaveMobSelectKeyword(MobEventMobSelectKeyword, ParsedWaveNum, ParsedMobIndex);
	//
	//if (DirectingEvent == EStageEvent::ESEV_MobAppear && WaveNum == ParsedWaveNum)
	//{
	//	EssentialBeginHandlingCommon();	// 스테이지 시작과 함께하는 첫 wave 라면 BeginPlay 이전에 이게 실행될 것.
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
	//		PossibleEventTriggeringMob = SpawnedMobList[0]; // 혹시라도 잘못된 설정이나 한 경우의 폴백..
	//	}

	//	BII_CHECK(!PossibleEventTriggeringMob || PossibleEventTriggeringMob->IsValidObj());

	//	if (PossibleEventTriggeringMob && CanPlayShow()) // BeginShow 에서도 CanPlayShow 검사를 하는데 UIDoc 세팅을 여기서 해야 해서 미리 검사.
	//	{
	//		SyncDataToUIDoc(PossibleEventTriggeringMob);
	//		bDirectingEventConditionMet = true;
	//		BeginShowOrBeginCountDown();
	//	}
	//}	
}

void AB2StageEventDirector::NotifySpawnPoolSingleWaveComplete(int32 WaveNum)
{
	//// 웨이브 번호만 사용
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
	// 단일 레벨 내에서 사용되는 SpawnPool 은 일정하며 GameMode 에서 알고 있으면 됨. 여기에 SpawnPool 정보를 보내지 않는 걸로 함.
	//
	//int32 ParsedWaveNum = 0;
	//int32 ParsedMobIndex = 0;
	//ParseForWaveMobSelectKeyword(MobEventMobSelectKeyword, ParsedWaveNum, ParsedMobIndex);

	//if (DirectingEvent == EStageEvent::ESEV_MobDying && DyingMob && WaveNum == ParsedWaveNum && MobObjIndex == ParsedMobIndex)
	//{		
	//	if (CanPlayShow()) // BeginShow 에서도 하는 검사인데 UIDoc 세팅을 여기서 해야 해서 미리 검사.
	//	{
	//		BII_CHECK(!DyingMob || DyingMob->IsValidObj());
	//		SyncDataToUIDoc(DyingMob);
	//		bDirectingEventConditionMet = true;

	//		// BeginShow 처리 도중 BeginSingleShow 에서 GetPlayActor 를 통해 DyingMob 을 가져오는 상황이 있을 것인데, 
	//		// 만일 BeginShow 를 지연시킨다거나 하면 WaveMobSelectKeyword 만 가지고는 불가능 할 것. MonsterSpawnPool 쪽에서 이 Notify 직후 DyingMob 을 목록에서 제거할 것이므로.
	//		// 그런 게 필요할지 몰라서 여기로 넘어오는 DyingMob 을 WaveNum 과 MobObjIndex 와 함께 캐싱해 두었다가 꺼내올 수 있도록 함..
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
	//	if (CanPlayShow()) // BeginShow 에서도 하는 검사인데 UIDoc 세팅을 여기서 해야 해서 미리 검사.
	//	{			
	//		SyncDataToUIDoc(ChangedMob);
	//		bDirectingEventConditionMet = true;
	//		BeginShowOrBeginCountDown();

	//		//ChangedMob의 Stun/Freeze 상태 해제
	//		ChangedMob->ClearDebuffs();
	//	}
	//}

	//// 연출 도중 PlayActor 중 하나가 (Re-spawn 을 동반하는) Phase 변화를 거치는 상황. 가능한지는 모르겠지만 대비는 해 둘 필요가 있겠다.
	//// 이 DestroyedPrevRef 는 valid 하지 않을 것이다. PlayActor 중에 이걸 들고 있다면 ChangedMob 으로 바꿔준다.
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
	// CharacterAboutToBeDestroyed 는 시스템에서 제거되기 직전임. 혹시라도 여기에 레퍼런스가 있다면 날린다.
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
	//	DelayedDyingMobCache.ForgetIfMatch(CharacterAboutToBeDestroyed); // 이쪽도 들고 있을 수 있다.
	//}
}

void AB2StageEventDirector::NotifyStageEventUIOpenComplete(FName InOpenedUI)
{
	if (InOpenedUI != NAME_None && InOpenedUI == MyRelevantUIName) // MyRelevantUIName 은 실제 사용시에나 세팅될 것.
	{
		bRelevantUIOpenComplete = true;

		// bRelevantUIOpenComplete 조건으로 인해 끝내지 못한 것이 있다면 다시 검사.
		CheckShowEndCountDown();
		CheckShowEndCountDown_ControlledMatinee(bStandbyAtTheEnd); // bStandbyAtTheEnd 면 StandByNoti 라고 믿을 수 밖에..?
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
		// PlayActorRelativeTransform 은 나를 기준으로 한 플레이어의 Relative 인데, 그 반대가 필요하므로 inverse
		const FTransform InvRelativeTransform = FTransform(InPlayActorRelativeRotation, InPlayActorRelativeLocation).Inverse();
		const FTransform AdjustTransform = InvRelativeTransform * InPlayActor->GetTransform();

		this->SetActorLocation(AdjustTransform.GetTranslation());
		// 가끔 Position 만 따라가고 싶을 때가 있음.
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
	//		// 우선, Type 에 따라 캐릭터를 가져옴
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
	//			// BeginShow 에서 LocalPC에 대해서는 하는데 태그캐릭터나 네트웤 플레이어가 들어갈 수도 있으니 여기서도 한번.
	//			CastedPlayActorPC->SetupLODInfoForCinematic();
	//		}

	//		if (ThisPlayActor && ThisPlayActor->IsValidObj())
	//		{
	//			// 이게 세팅되면 Dying 인 경우라도 실제 dead anim 과 destroy 처리가 지연될 것. 그리고 그런 경우는 이 연출이 끝난 후에 상태 복구 시 꺼 버리든지 제거하든지 하게 될 것 (RestorePlayActorForCtrlMatineeEnd)
	//			ThisPlayActor->SetPlayingDirectedStageEvent(ThisSetting.IsDirectedMode()); 

	//			// Spawn 시 약간 뜨게 되므로 Snap 부터 수행하며 만일 실제로 떠 있는 장면 등이 필요하다면 생각해 보자..
	//			SnapBladeIICharacterToGround(ThisPlayActor);

	//			// 옵션에 따라 완료 후 복원할 수 있도록 위치/회전값을 백업해 둔다.
	//			ThisState.BackupWorldLocation = ThisPlayActor->GetActorLocation();
	//			ThisState.BackupWorldRotation = ThisPlayActor->GetActorRotation();

	//			if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera || OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
	//			{
	//				// 캐릭터의 Location, Rotation 직접 세팅
	//				if (ThisSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_BringPlayActor)
	//				{
	//					// EPAP_BringPlayActor 이면 ExportedCamera 든, ManualCamera 든 가리지 않고 위치 직접 세팅
	//					const FTransform RelativeTransform(ThisSetting.ManualRelativeRotation, ThisSetting.ManualRelativeLocation);
	//					const FTransform ParentToWorld = this->GetTransform();
	//					const FTransform BroughtTransform = RelativeTransform * ParentToWorld;

	//					ThisPlayActor->SetActorLocation(BroughtTransform.GetTranslation()); // Transform 을 사용한 건 훼이크였다.
	//					ThisPlayActor->SetActorRotation(BroughtTransform.GetRotation().Rotator());

	//					SnapBladeIICharacterToGround(ThisPlayActor); // 음.. 결국 snap 은 해 줘야 하려나..
	//				}
	//				else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
	//				{
	//					if (ThisSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_AdjustToPlayActor || ThisSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_ContinuouslyAdjustToPlayActor)
	//					{
	//						// ManualCamera 모드에서의 AdjustToPlayActor 는 액터 전체를 아싸리 플레이어 중심으로 위치시킨다. 
	//						// 바로 아래 ExportedCamera 에서의 AdjustToPlayActor 는 CameraAnimApplyCamIndex 에 따라 사용되는 컴포넌트 하나만 조정.
	//						// ContinuouslyAdjustToPlayActor 에서는 Tick 에서도 이걸 처리.
	//						AdjustSelfPlacementToPlayActor(ThisPlayActor, ThisSetting.ManualRelativeLocation, ThisSetting.ManualRelativeRotation);
	//					}
	//				}
	//				else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera)
	//				{
	//					// 아래 사용하는 MoveKeyFrames 들은 추후 확장 가능성을 위해 KeyFrame 으로 했으며 일단은 그냥 초기 위치 가져오기 위함.
	//					if (IsExportedEventSceneValid(ThisSetting.ExportedEventScene))
	//					{
	//						if (ThisSetting.PlayActorPlacement != EPlayActorPlacement::EPAP_AdjustToPlayActor && ThisSetting.PlayActorPlacement != EPlayActorPlacement::EPAP_ContinuouslyAdjustToPlayActor)
	//						{
	//							// PlayActor 를 카메라 기준으로 위치시킨다.
	//							FSimpleEventSceneMoveKeyData& FirstKeyData = ThisSetting.ExportedEventScene->SimpleMoveKeyFrames[0];
	//							SetSESPlayActorMovementFromCamera(FirstKeyData, ThisPlayActor);
	//							// 카메라가 떠 있을 수 있으므로 다시 Snap.. ㅡ.ㅡ
	//							SnapBladeIICharacterToGround(ThisPlayActor);
	//						}

	//						// AdjustToPlayActor 인 경우 카메라를 PlayActor 기준으로 위치시키며
	//						// 그게 아니더라도 높이 맞추기 차원에서 카메라 위치 조정. 아닌 경우는 이미 플레이어가 카메라 중심으로 위치했을 것.
	//						SetSESCameraMovementFromPlayActor(ThisSetting.ExportedEventScene->InvSimpleMoveKeyFrames[0], ThisPlayActor);
	//					}
	//				}
	//			}
	//			else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//			{
	//				// 여기선 ThePlayActor 를 셋업하는 대신 기존에 설치해둔 마티니 그룹 actor 를 PlayActor 와 비슷하게 셋업할 것임. 길어지니 따로 뺀다.
	//				bool bSetupMatineeGroupRes = SetupSingleShowMatineeGroup(ThisSetting, ThisState);
	//				if (ThisSetting.IsDirectedMode() && !bSetupMatineeGroupRes) // InGame 모드면 마티니 트랙이 필요없는 거라 SetupSingleShowMatineeGroup 이 실패할 수 있는데, 그렇다고 여기서 실패처리를 할 건 없지.
	//				{
	//					return false;
	//				}
	//			}

	//			// Input 이 가능한 상태에서 Player Character 는 게임 상태를 그대로 가져가야 애니메이션이 나오든지 하겠지 낄낄
	//			if (bDisableInput == true || (ThisPlayActor != GetB2LocalPlayerChar() && ThisPlayActor != GetB2MatchOpponent()))
	//			{
	//				if (ThisSetting.IsDirectedMode())
	//				{// StageEventShowState 를 세팅하는데 이건 이전 ExportedCamera 랑 ManualCamera 에서 사용하던 거. AnimMontage 를 플레이 하면서 전투를 막는 역할도 했으나 지금은 거의 state 표시 수준으로 제거될 수 있음.
	//					EStageEventShowState FinalSES =
	//						(ThisSetting.StageEventStateOverride == EStageEventShowState::ESES_None || OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee) ?
	//						GetDefaultDesiredPlayActorSESState(DirectingEvent, ThisSetting.PlayActorType) : ThisSetting.StageEventStateOverride;
	//					ThisPlayActor->SetStageEventShowState(FinalSES, this);
	//				}
	//				if (ThisSetting.PlayActorStateMode != EPlayActorStateMode::EPASM_MaintainInGame)
	//				{
	//					ThisPlayActor->PreventMovementByStageEvent(); // 숨은 와중, 혹은 연출 도중에 spawn 되었더라도 플레이어 쫓아오지 않게. PreventMovement 하면 공격도 못함. 개꿀

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
	//	// 마티니 모드에서 AdjustToPlayActor 도 사용 가능하도록 상대좌표 기능을 좀 활용. 설령 마티니에서 찾지 못한 트랙이라도 기준으로 삼을 수는 있도록 함. 그런 경우는 대체로 PlayActorStateMode 를 InGame 으로 놓을 듯.
	//	if (InSettingForThis.IsOneOfAdjustToPlayActorPlacement())
	//	{
	//		MatineeActor->SetDynamicRelativeMoveTrackBaseActor(ThisPlayActor,
	//			FVector(0.0f, 0.0f, -1.0f * ZOffsetFromMatchingSKActorToBIICharacter(ThisPlayActor)), // 여기도 에디터 뷰포트에선 SkeletalMeshActor 로 보겠지만 인게임에선 BladeIICharacter 이니 오프셋 맞춰줌.
	//			(InSettingForThis.PlayActorPlacement == EPlayActorPlacement::EPAP_AdjustToPlayActorLocationOnly)); // LocationOnly 옵션 두번째 인자로..
	//	}

	//	// 설정 및 플레이 상황에 따른 최종적인 InterpGroup 이름.
	//	const FName FinalMatchingGroupName(*GetFinalMatchingInterpGroupName(ThisPlayActor, InSettingForThis.MatchingInterpGroupName.ToString(), false));

	//	ASkeletalMeshActor* FoundGroupSKActor = Cast<ASkeletalMeshActor>(GetGroupActorOfGroupName(FinalMatchingGroupName));

	//	if (FoundGroupSKActor)
	//	{
	//		// 마티니에 사용될 스켈엑터들은 데칼안먹게 해준다.
	//		UPrimitiveComponent* pPrimitiveComponent = Cast<UPrimitiveComponent>(FoundGroupSKActor->GetRootComponent());
	//		pPrimitiveComponent->bReceivesDecals = false;

	//		InStateForThis.CtrlMatineePuppet = FoundGroupSKActor;

	//		// CtrlMatineePuppet 을 PlayActor 와 마찬가지로 분장
	//		// 장신구가 추가되거나 해서 추가로 외관을 맞춰줄 필요가 있을 때에는 아래 SetupControlledMatineePuppet 쪽에 작업을 할 것.
	//		ThisPlayActor->SetupControlledMatineePuppet(InStateForThis.CtrlMatineePuppet, 
	//			// bHidePlayerPointLight 에 따라 puppet 쪽에 마찬가지 PointLight 를 셋업하게 함. 옵션에 따라 로컬 플레이어 캐릭터로 한정되는데 모바일 dynamic point light 개수 제한이 있기 때문에 주의하는 것.
	//			(!bHidePlayerPointLight && 					
	//				(InStateForThis.IsForLocalPlayerCharacter() || (bAllowOtherPlayerPointLight && Cast<ABladeIIPlayer>(InStateForThis.GetPlayActor(this))))
	//				)
	//		); 

	//		if (InSettingForThis.ActorScaleOverride > 0.0f)
	//		{ // SetupControlledMatineePuppet 에서 ThisPlayActor scale 을 적용했을 건데 따로 직접 지정하고 싶을 경우. 일단 uniform scale 로다가.
	//			InStateForThis.CtrlMatineePuppet->GetSkeletalMeshComponent()->SetWorldScale3D(FVector(InSettingForThis.ActorScaleOverride, InSettingForThis.ActorScaleOverride, InSettingForThis.ActorScaleOverride));
	//		}

	//		// SetupControlledMatineePuppet 에서 각 component 들의 visibility 상태를 체크할 수 있으므로 SetVisibility 나 SetActorHiddenInGame 등은 그 이후에 처리.

	//		if (InSettingForThis.IsDirectedMode()) // InGameStateMode 면 마티니 anim track 사용하지 않는 것. 근데 그런 경우에 마티니 track 을 준비할 필요가 있나..?
	//		{
	//			// 기본적으로 숨겨놓았다고 가정하고.. 잠깐 켜 놨다가 종료 후 다시 숨겨놓음. 이걸로 등록한 게 아님 Visibility 트랙으로 해야 할 듯.
	//			SetVisibilityOfMatineePuppet(InStateForThis.CtrlMatineePuppet, InStateForThis.GetPlayActor(this), true);

	//			// 간혹 상하 이동이 심한 애니메이션의 경우 AlwaysTickPoseAndRefreshBones 가 필요한 경우가 있다. 물론 연출 종료 후엔 젤 업데이트 빈도 낮은 걸로 복구
	//			InStateForThis.CtrlMatineePuppet->SetActorTickEnabled(true); // 기본 틱도 여기서 활성화를.
	//			InStateForThis.CtrlMatineePuppet->GetSkeletalMeshComponent()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	//			// ThePlayActor 를 숨겨놓을 가장 기본적 장소인데, PCSync 등의 건으로 인해 따로 이후에 처리
	//			//TryHidePlayActorForMatineeInternal(ThisPlayActor, true);
	//			if (ThisPlayActor->bNeedSpawnCheck)
	//			{// NPCClassInfo 의 bUsePreSpawnState 에 따라 세팅이 되는데, 연출 장면을 거친다는 거 자체가 PreSpawnState 를 지나는 것으로 간주할 수 있고, 오히려 연출 이후에 spawn 이 다시 되어버리니 문제가 됨. 따라서 false 로 돌려놓음.
	//				ThisPlayActor->bNeedSpawnCheck = false; 
	//				// 그리고 MeshComponentUpdateFlag 를 세팅해 주어야 실제로 상태가 바뀐다.. 연출 끝나면 도로 되돌림.
	//				ThisPlayActor->TemporarySetMeshComponentUpdateFlag(EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones);
	//			}
	//		}

	//		// 얘들이 인공지능 길을 막네..? 철저하게 off 시키자. 이건 원래 배치단계에서 부터 해야 되는 것으로 복구하지 않는다.
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
		InPuppet->GetSkeletalMeshComponent()->SetVisibility(true, true); // 편집 목적으로 완전히 숨겨놓았다가 까먹을 수도 있으니 ㅋ
		InPuppet->SetActorHiddenInGame(false); // 이게 핵심인데 이것만 가지고 다 되지 않을 때가 있어서 함수 따로 빼 놓은 거..

		ActivateMatineePuppetPSCs(InPuppet, InRelevantPlayActor); // 실은 거의 요거 하려고 따로 뺐음.
	}
	else
	{
		InPuppet->SetActorHiddenInGame(true);
	}
}

void AB2StageEventDirector::ActivateMatineePuppetPSCs(ASkeletalMeshActor* InPuppet, ABladeIICharacter* InRelevantPlayActor)
{
	// 뭔가 Owner Actor 가 숨어있는 동안에 Particle System Component 들이 Deactivate 가 되는 듯해서 따로 Activate 시키기 위해 만들어놓음.
	// 전역 유틸로 마찬가지 맥락의 기능이 EnsureActivateStaticallyAttachedPSCs 로 제공됨..
	//if (InPuppet)
	//{
	//	ABladeIIPlayer* CastedPlayActorPlayer = Cast<ABladeIIPlayer>(InRelevantPlayActor);
	//	TArray<UActorComponent*> AllPSC = InPuppet->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//	for (UActorComponent* ThisComp : AllPSC)
	//	{
	//		UParticleSystemComponent* ThisPSComp = Cast<UParticleSystemComponent>(ThisComp);
	//		if (ThisPSComp)
	//		{
	//			if (bHideHud && CastedPlayActorPlayer && CastedPlayActorPlayer->FloorRingPS == ThisPSComp->Template/* && GetB2GameModeType(GetWorld()) != EB2GameMode::PVP_Tag*/)
	//			{ // HUD 숨기는 경우라면 FloorRing 은 여하간 보이지 않게..
	//				// 지금 예상으로 여기엔 들어와선 안된다. 
	//				// FloorRingPS 를 EventPuppetSetupBlacklist 에 추가해서 아예 Puppet 에 붙이지 않게 하였음.
	//				// 이 코드는 옛날 옛적 땜질 코드임.
	//				checkSlow(0); 
	//				ThisPSComp->Deactivate();
	//				ThisPSComp->SetHiddenInGame(true);
	//			}
	//			else
	//			{
	//				EnsureActivatePSCInternal(ThisPSComp);
	//			}
	//		}
	//	}
	//}
}

AActor* AB2StageEventDirector::GetGroupActorOfGroupName(const FName& InGroupName)
{
	//if (!MatineeActor || !MatineeActor->MatineeData){
	//	return NULL;
	//}

	//for (int32 GI = 0; GI < MatineeActor->MatineeData->InterpGroups.Num(); ++GI)
	//{
	//	UInterpGroup* CurrGroup = MatineeActor->MatineeData->InterpGroups[GI];

	//	if (CurrGroup->GroupName == InGroupName)
	//	{
	//		// 설정한 이름과 일치하는 그룹을 찾은 후에는 그룹 인스턴스를..
	//		UInterpGroupInst* FoundGI = NULL;
	//		for (int32 GII = 0; GII < MatineeActor->GroupInst.Num(); ++GII)
	//		{
	//			if (MatineeActor->GroupInst[GII]->Group == CurrGroup) // 결국 인덱스가 같으려나?
	//			{
	//				FoundGI = MatineeActor->GroupInst[GII];
	//				return FoundGI ? FoundGI->GroupActor : NULL;
	//			}
	//		}
	//	}
	//}
	return NULL;
}

void AB2StageEventDirector::SelectFinalDirectorTrack()
{
	//if (OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee || MatineeActor == NULL || MatineeActor->MatineeData == NULL)
	//{
	//	return;
	//}

	//UInterpGroupDirector* TheDirGroup = MatineeActor->MatineeData->FindDirectorGroup(); // Director Group 은 특별하니 바로 얻어올 수 있군 ㅋ
	//if (TheDirGroup == NULL)
	//{
	//	return;
	//}

	//if (TheDirGroup->InterpTracks.Num() <= 1 || (bPCDependentDirectorTrack == false && !IsValidRandomMatineeGroupNumSet()))
	//{
	//	// 이 경우, 걍 처음 걸로다가 Enable 해 버릴 수도 있겠지만, Director Track 을 아싸리 쓰지 않는 경우도 생각해 볼 수 있겠으니 그냥 리턴.
	//	return;
	//}

	//// 특정 ShowSetting 에 바인딩 된 PlayActor 가 아닌 현재 주연격으로 나오는 Player Character 에 따라서임.
	//ABladeIIPlayer* PossibleHighlightedPlayer = GetShowHeroPlayerIfAny();
	//if (bPCDependentDirectorTrack && !PossibleHighlightedPlayer)
	//{ // 대체로 이 event 가 이런 성격에 맞지 않는 경우일 것. 물론 아닐 수도 있는데 그런 경우가 생기면 GetShowHeroPlayerIfAny 를 손질.
	//	return;
	//}
	//BII_CHECK(!PossibleHighlightedPlayer || PossibleHighlightedPlayer->IsValidObj());

	//FString DesiredPostFix;
	//if (bPCDependentDirectorTrack && PossibleHighlightedPlayer)
	//{ // FinalMatchingInterpGroupName 이라는 의미와는 약간 다르고 마찬가지 규약을 적용한 postfix 만 가져오려는 거
	//	DesiredPostFix = GetFinalMatchingInterpGroupNamePC(TEXT(""), PossibleHighlightedPlayer->GetTrimmedNameFromPCClassEnum(), EPCInterpGroupNetType::PCIGNT_Local, false);
	//}
	//else if (IsValidRandomMatineeGroupNumSet())
	//{// FinalMatchingInterpGroupName 이라는 의미와는 약간 다르고 마찬가지 규약을 적용한 postfix 만 가져오려는 거
	//	DesiredPostFix = GetFinalMatchingInterpGroupNameRandom(TEXT(""), RandomlySetMatineeGroupNum, false);
	//}
	//
	//// 현재 PlayerCharacter 에 맞는 postfix 를 가진 것으로 선택.
	//for (int32 DI = 0; DI < TheDirGroup->InterpTracks.Num(); ++DI)
	//{
	//	UInterpTrackDirector* CurrDirTrack = Cast<UInterpTrackDirector>(TheDirGroup->InterpTracks[DI]);
	//	if (CurrDirTrack == NULL)
	//	{
	//		continue;
	//	}

	//	if (CurrDirTrack->TrackTitle.EndsWith(DesiredPostFix)) // Postfix 만 맞추면 됨. 공통 prefix 같은 거 없음.
	//	{
	//		CurrDirTrack->EnableTrack(true);
	//	}
	//	else
	//	{
	//		CurrDirTrack->EnableTrack(false); // 여타 나머지는 끈다. 어차피 DirectorTrack 은 하나밖에 못 씀.
	//	}
	//}

	// 혹시라도 위에서 Enable 된 것이 없었다면 걍 첫번째 것으로.. 할 수도 있겠지만 그냥 두자.
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

	//		// 현재 ShowSetting 과 연결된 Matinee InterpGroup 혹은 Folder 이름
	//		const FName FinalMatchingGroupName(*GetFinalMatchingInterpGroupName(CurrShowState.GetPlayActor(this), CurrShowSetting.MatchingInterpGroupName.ToString(), false));
	//		const FName FinalMatchingGroupNameForFolder(*GetFinalMatchingInterpGroupName(CurrShowState.GetPlayActor(this), CurrShowSetting.MatchingInterpGroupName.ToString(), true));

	//		if (CurrGroup->GroupName == FinalMatchingGroupName || 
	//			CurrGroup->GroupName == FinalMatchingGroupNameForFolder)
	//		{
	//			bFoundMatchingOne = true; // 바로 맞는 게 있으므로 이건 통과함.
	//			break;
	//		}
	//	}

	//	if (bFoundMatchingOne){
	//		continue;
	//	}

	//	bool bNotDesiredGroup = false;

	//	// 여기로 왔으면 ShowSetting 을 통해 그룹 이름을 넣지 않았거나, 현재 게임 상황에 맞지 않거나 한 경우 등. 
	//	// 정해진 네이밍 규약에 의해 확실히 나오지 말아야 하는 경우인지 한번 더 체크해서 비활성화 단계로 (RuntimeDisableInterpGroup)
	//	// 그룹 이름이 정해진 네이밍 규칙과 아예 관련이 없다면 여기서 비활성화 할 대상이 아니다.

	//	for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
	//	{
	//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
	//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];

	//		// 여기서 ThePlayActor 를 검사하는 건 단지 이 세팅이 BladeIIPlayer 용인지 보기 위함. 실제로 검사하는 ThePlayActor 와 연관된 그룹이라면 제거해선 안됨.
	//		if (Cast<ABladeIIPlayer>(CurrShowState.GetPlayActor(this))) 
	//		{
	//			for (int32 PCI = 0; PCI < AllTrimmedPCClassEnumName.Num(); ++PCI)
	//			{
	//				for (int32 NTI = 0; NTI < (int32)EPCInterpGroupNetType::PCIGNT_End; ++NTI)
	//				{
	//					// 이 ShowSetting 과 PCClass 에 따라 규약에 들어가는 Group 혹은 폴더 이름
	//					// Iteration 도중에 실제로 사용될 이름도 나오겠지만.. 거기에 매칭되는 InterpGroup 은 이미 여기로 오지 않고 여기로 온 것들은 비활성화 후보.
	//					const FName PossiblePCGroupName(*GetFinalMatchingInterpGroupNamePC(CurrShowSetting.MatchingInterpGroupName.ToString(), AllTrimmedPCClassEnumName[PCI], static_cast<EPCInterpGroupNetType>(NTI), false));
	//					const FName PossiblePCGroupNameForFolder(*GetFinalMatchingInterpGroupNamePC(CurrShowSetting.MatchingInterpGroupName.ToString(), AllTrimmedPCClassEnumName[PCI], static_cast<EPCInterpGroupNetType>(NTI), true));

	//					// 현재 ShowSetting 에서의 MatchingInterpGroupName 을 베이스로 하고 있지만 현재 게임에서 선택되어 있지는 않다.
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
	//		{ // 랜덤 그룹 선택이 있는 경우, 선택되지 못한 랜덤 그룹을 제거. 이건 Player 는 아니고 몹 연출.
	//			for (int32 RNI = 1; RNI <= RandomMatineeGroupNum; ++RNI)
	//			{ 
	//				if (RNI == RandomlySetMatineeGroupNum){
	//					continue; // 이번 랜덤 선택된 거임. 어차피 아래에서 bNotDesiredGroup 선택이 되지 않겠지만 미리 걸러냄.
	//				}
	//				// 각 랜덤 별로 폴더 구성했다 치고 그것만 처리하자. 물론 이것과 별개로 하위 그룹들도 네이밍을 맞춰주긴 해야 함.
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

	//	// 기타 다른 경우 추가되면..

	//	if (bNotDesiredGroup)
	//	{// 해당 그룹을 비활성화
	//		RuntimeDisableInterpGroup(CurrGroup, MatineeActor);
	//	}
	//}
}

void AB2StageEventDirector::RuntimeDisableInterpGroup(class UInterpGroup* GroupToDisable, class AMatineeActor* OwnerMatinee)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::RuntimeRemoveGroupActorFromInterpGroup"));

//	if (GroupToDisable && OwnerMatinee)
//	{
//		TArray<UInterpGroupInst*> arMatchingGI;
//		// 그룹과 일치하는 그룹 인스턴스를 찾는다.
//		for (int32 GII = 0; GII < OwnerMatinee->GroupInst.Num(); ++GII)
//		{
//			// Disable 하고자 하는 것이 자기 자신이든지 아니면 자신을 포함하는 폴더 그룹이든지
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
//				// 이거 좀 낭비이긴 한데.. 에디터 맵 저장 시 ShowSetting 에 등록되지 않은 애들도 visibility 를 꺼주려다 보니 Matinee InitInterp 시 SkeletalMeshActor 들을 켜주는 코드가 추가되었고.. 근데 여기서 쓸모없다고 간주되는 애들은 다시 꺼주어야 ㅋ
//				ASkeletalMeshActor* ThisGroupSKActor = Cast<ASkeletalMeshActor>(ThisGI->GetGroupActor());
//				if (ThisGroupSKActor)
//				{
//					ThisGroupSKActor->SetActorHiddenInGame(true);
//				}
//				// PC 별로 라이트 트랙들 만들어 놓은 경우도 별도 처리가 필요. SetAllPointLightGroupAffectWorld 로는 처리 못함.
//				ALight* ThisGroupLightActor = Cast<ALight>(ThisGI->GetGroupActor());
//				UPointLightComponent* PLComp = ThisGroupLightActor ? Cast<UPointLightComponent>(ThisGroupLightActor->GetLightComponent()) : nullptr;
//				if (PLComp
//#if 0
//					// 의도하지 않은 잘못된 기능 하에서 셋업 되었던 라이팅을 메꾸기 위해 넣었던 임시 플래그..
//					// 셋업 수정이 완료되면 bHackMyPerPCClassLightingSetupDone 을 체크하는 식이었음.
//					// 셋업 완료되었다고 하여 DEPRECATED 마크하고 기능은 제거.. 혹 모르니 완전히 제거하지는 않음.
//					&& bHackMyPerPCClassLightingSetupDone_DEPRECATED 
//#endif				
//					)
//				{
//					PLComp->bAffectsWorld = false;
//					PLComp->MarkRenderStateDirty();
//				}
//
//				// 예전에 GroupActor 를 제거하고 했었는데 걍 하위 트랙 Disable 로 되지 않나 싶다. 일단 영웅의 탑 로딩없는 재시작을 하려면 GroupActor 는 제거해선 안됨.
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
//		// 뭔가 Folder 들어가면서부터인지.. 위 코드가 안먹힐 때가 있다. 다시 이걸로도..
//		for (UInterpTrack* CurrTrack : GroupToDisable->InterpTracks)
//		{
//			if (CurrTrack)
//			{
//				CurrTrack->EnableTrack(false);
//			}
//		}
//	}
}

//UInterpGroup* AB2StageEventDirector::FindParentGroupFolder(UInterpGroup* ChildGroup, class AMatineeActor* OwnerMatinee) const
//{ // 실 구현부를 MatineeActor 쪽으로 옮겨놓음.
//	if (OwnerMatinee)
//	{
//		return OwnerMatinee->FindParentGroupFolder(ChildGroup);
//	}
//	return NULL;
//}

FString AB2StageEventDirector::GetFinalMatchingInterpGroupName(class ABladeIICharacter* InPlayActor, FString RawInterpGroupName, bool bForFolderGroup)
{
//	ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(InPlayActor);
//	if (CastedPlayer) // 기타 NPC 들의 경우 InterpGroup 이름을 변경해서 검색할 경우가 발생할까?
//	{
//		EPCInterpGroupNetType NetType = EPCInterpGroupNetType::PCIGNT_Local;
//
//		AB2NetGameMode* NetGM = Cast<AB2NetGameMode>(GetB2GameMode());
//		if (NetGM && DirectingEvent == EStageEvent::ESEV_Begin) // 현재로선 NetGameMode 의 사이드가 문제가 되는 경우는 시작뿐.
//		{
//			ABladeIINetPlayer* CastedNetPlayer = Cast<ABladeIINetPlayer>(CastedPlayer);
//
//			NetType = (
//				((NetGM->GetPeerType() != PEER_RESTRICTED && CastedNetPlayer->GetNetStatus() == ABladeIINetPlayer::NET_MASTER) || (NetGM->GetPeerType() == PEER_RESTRICTED && CastedNetPlayer->GetNetStatus() == ABladeIINetPlayer::NET_SLAVE)) ?
//				EPCInterpGroupNetType::PCIGNT_SuperPeer : EPCInterpGroupNetType::PCIGNT_CommonPeer
//			);
//		}
//
//		// PC 는 클래스 선택, Net 게임 peer 타입 등에 따라 달라짐.
//		return GetFinalMatchingInterpGroupNamePC(RawInterpGroupName, CastedPlayer->GetTrimmedNameFromPCClassEnum(), NetType, bForFolderGroup);
//	}
//	else if (IsValidRandomMatineeGroupNumSet()) 
//	{// 현재 flow 로는 Player 용 트랙은 Random selection 이 안 될 것.
//		// Player 그룹은 이미 별도의 suffix 가 붙는데다가 실제 요구사항도 몹이 나오는 연출을 랜덤하게 하자는 것임.
//		return GetFinalMatchingInterpGroupNameRandom(RawInterpGroupName, RandomlySetMatineeGroupNum, bForFolderGroup);
//	}

	return RawInterpGroupName;
}

FString AB2StageEventDirector::GetFinalMatchingInterpGroupNamePC(FString RawInterpGroupName, FString TrimmedPCClassEnumName, EPCInterpGroupNetType NetType, bool bForFolderGroup)
{
	// 룰에 맞춰 그룹 이름을 넣도록 해야.. 예를 들어 "MainPCGroup_Gladiator", "SubPCGroup_Assassin" 하는 식이 되겠다.
	FString RetName = RawInterpGroupName + TEXT("_") + TrimmedPCClassEnumName;

	//// 이 룰은 일단 PVP에서만 쓰는걸로 한다. (추후에 이건 옵션으로 빼도록 하자)
	//if (AB2PVPGameMode* PVPGM = Cast<AB2PVPGameMode>(GetB2GameMode()))
	//{
	//	if (NetType == EPCInterpGroupNetType::PCIGNT_SuperPeer){
	//		RetName += FString(TEXT("_")) + FString(INTERPGROUP_SUFFIX_SUPERPEER);
	//	}
	//	else if (NetType == EPCInterpGroupNetType::PCIGNT_CommonPeer){
	//		RetName += FString(TEXT("_")) + FString(INTERPGROUP_SUFFIX_COMMONPEER);
	//	}
	//}
	//
	//if (bForFolderGroup)
	//{
	//	RetName = FOLDERGROUP_PREFIX + RetName;
	//}
	return RetName;
}

FString AB2StageEventDirector::GetFinalMatchingInterpGroupNameRandom(FString RawInterpGroupName, int32 RandNum, bool bForFolderGroup)
{ // 디렉터그룹도 이걸 쓸 것임.
	FString RandPostFix = FString::Printf(INTERPGROUP_SUFFIX_RAND, RandNum);
	// Folder 이름인 경우 그냥 정해진 prefix 와 postfix 만 붙여서 사용.
	return bForFolderGroup ? (FOLDERGROUP_PREFIX + RandPostFix) : (RawInterpGroupName + TEXT("_") + RandPostFix);
}

#if WITH_EDITOR
bool AB2StageEventDirector::IsPossibleInterpGroupNameForPC(const FString& InCheckName) const
{ // GetFinalMatchingInterpGroupNamePC 로 나올 수 있는 이름인지 체크.. true 리턴이라고 해도 아닐 수는 있지만 false 리턴이면 확실히 아닌 거.

	TArray<FString> AllPCClassTrimmedName;
	ABladeIIPlayer::GetAllTrimmedNameFromPCClassEnum(AllPCClassTrimmedName);

	bool bHasAtLeastOnePCName = false;
	for (const FString& ThisPCName : AllPCClassTrimmedName)
	{
		if (InCheckName.EndsWith(ThisPCName)) // GetFinalMatchingInterpGroupNamePC 구현에 따라.. PCClass 이름이 끝에 붙음.
		{
			bHasAtLeastOnePCName = true;
			break;
		}
	}

	if (!bHasAtLeastOnePCName)
	{
		return false;
	}

	// ShowSettings 의 MatchingInterpGroupName 까지 보고는 싶지만..

	return true;
}
#endif

void AB2StageEventDirector::SetAllPointLightGroupAffectWorld(bool bAffectWorld)
{
	//if (!MatineeActor)
	//	return;
	//
	//const int32 AllowedMatineeTrackPointLightNum = GetAllowedMatineeTrackPointLightNum();

	//// 모바일 렌더러 개수 제한 고려해야 하는데.. 여기서 하지 못함.
	//// 왜냐하면 Group 별로 선택적으로 사용해야 하는 경우가 있기 때문에
	//// 여기서는 일괄적으로 On/Off 하고 개별 그룹별 필요없는 거 끄는 건 RuntimeDisableInterpGroup 에서
	//int32 UsedLightCount = 0;
	//for (int32 GII = 0; GII < MatineeActor->GroupInst.Num(); ++GII)
	//{
	//	ALight* pLight = Cast<ALight>(MatineeActor->GroupInst[GII]->GroupActor);		
	//	ULightComponent* TheLightComp = pLight ? pLight->GetLightComponent() : nullptr;
	//	if (TheLightComp)
	//	{
	//		const bool bPrevAffectsWorld = TheLightComp->bAffectsWorld;
	//		if (UsedLightCount < AllowedMatineeTrackPointLightNum) {
	//			TheLightComp->bAffectsWorld = bAffectWorld;
	//		}
	//		else { // 제한 넘어가면 허용 안.. 하고 싶었는데 그룹별로 지정해 놓은 게 있어서 불가함. ㅡㅡ
	//			TheLightComp->bAffectsWorld = bAffectWorld;
	//		}

	//		if (bPrevAffectsWorld != TheLightComp->bAffectsWorld){
	//			TheLightComp->MarkRenderStateDirty();
	//		}

	//		++UsedLightCount;
	//	}
	//}
}

void AB2StageEventDirector::HackSpareLeastPointLight()
{
	// 모바일 렌더링의 dynamic point light 개수 제한을 맞추기 위한 적절한 Light On/Offf 컨트롤을 미스하던 시절에
	// 잘못 설정되어 버린 일부 연출 셋업을 메꾸기 위함..
	// RuntimeDisableInterpGroup 에서 비활성화 시켜야 할 트랙의 point light 를 끄게 되는데 이걸 고려하지 못한 셋업으로 인해 point light 가 하나도 나오지 않는 상황이라면 하나는 살리도록..
	
//	if (bHidePlayerPointLight && MatineeActor) // bHidePlayerPointLight 가 아니라면 플레이어 캐릭터 라이트를 전달해 주었을 테니 하나는 있을 것.
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
//						bAtLeastOneTurnedOn = true; // 하나는 살아 있으니 더 볼 필요 없음.
//						break;
//					}
//					if (!TheFirstEncounteredPLComp) { // 아무도 안 살아 있는 경우를 위해 처음껄 캐싱해 둔다.
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
//#if !UE_BUILD_SHIPPING // 뭔가 하나 켜긴 하지만 경고 대상.
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
		OutGroupNames.Add(InSetting.MatchingInterpGroupName); // PC 가 아니라면 MatchingInterpGroupName 으로 바로.

		if (RandomMatineeGroupNum > 1) // 랜덤 그룹 이름은 추가로 넣는 걸로. 아마도 이게 있다면 원래 이름은 안 쓸 거 같지만.
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
	//	MatineeActor->AddCustomNotifyActor(this); // Stop notification 을 받기 위해 등록. Play 이전에 넣으면 TermInterp 에서 빠짐.
	//	if (!GIsEditor) 
	//	{ // 요 SetCloseBattleMainUIClass 이벤트를 에디터에서 Init-Destroy 시에 Subscribe/Unsubcribe 하는데 에디터에서 Unsubscribe 가 빠지기 쉬움. ㅠㅠ
	//		// 그리고 아마 에디터에선 필요하지도 않을 듯.
	//		SetCloseBattleMainUIClass<bool>::GetInstance().Signal(false);
	//	}
	//	EnterCinematicModeForControlledMatinee(); // CinematicMode 진입해야 실질적으로 플레이어 캐릭터 visibility 가 세팅될 것.
	//}

	//// SetVisibilityOfMatineePuppet 을 통해서 호출함에도 불구하고 여기서도 또 필요한듯.. 그리고 이걸로도 안먹힐 때가 있음. 뭔가 미묘한 타이밍 문제가 있는 듯 한데..
	//for (int32 EI = 0; EI < ShowStates.Num(); ++EI)
	//{
	//	ActivateMatineePuppetPSCs(ShowStates[EI].CtrlMatineePuppet, ShowStates[EI].GetPlayActor(this));
	//}

	//// LocalPlayerSwitching 을 여기서 한다. PCSyncToMatineeStart 가 있더라도 이 시점에는 마티니 플레이가 진행될 것이므로. (현재 뉴 튜토리얼에서 사용)
	//// ActivateMatineePuppetPSCs 이전에 못하는 건 아래 Switching 을 거치면서 PlayActor 가 날아가는 경우가 있어서 Hide 동작에 일부 문제가 발생하므로..
	//if (bLocalPlayerSwitching) {
	//	SwitchingLastRenderingPlayer();
	//}
}

void AB2StageEventDirector::SwitchingLastRenderingPlayer()
{
	///* ----------------------------------------------------
	// !! 주의 !!
	// 
	// 연출 기능에 게임 flow 가 종속되는 사례라 일반 모험 스테이지의 경우 연출 스킵 여부에 따라 인게임 flow 가 바뀌는 버그 발생 소지가 있음.
	// 이미 그런 식의 사용이 없지도 않고 이 기능을 사용하는 연출 성격 상 게임 flow 가 연출에 종속되는 게 사실이기도 한데..
	//
	// 당장은 튜토리얼 외의 모드에서 사용을 하지 않도록 하고,
	// 일반 모험 스테이지에서 사용 필요성이 생기면 사용 조건을 ManageMode 나 SupportedStages 여부 등에 따라 한정짓고
	// 특히 EStageEventSkipType::SEST_LeastSkip 이 사용되어야 한다면 LeastSkip 시에 이 동작을 빠뜨리지 않도록 할 필요가 있음.
	//---------------------------------------------------- */
	//bool bisInTutorialGameMode = (Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this)) != nullptr);
	//checkSlow(bisInTutorialGameMode);
	//if (!bisInTutorialGameMode) {
	//	return;
	//}
	//////////////////////////////////////////////////////////

	//TArray<FPlayActorShowRuntimeState*> AllStatesOfPlayerChar;
	//GetShowStatesOfPlayerChar(AllStatesOfPlayerChar); // 플레이어 캐릭터들의 ShowState 만 선별해서 가져옴.

	//checkSlow(ShowStates.Num() >= 2);
	//if (ShowStates.Num() < 2) {
	//	return; // 둘 이상이어야 바꾸든지 하지..
	//}

	//// AllStatesOfPlayerChar 안에서의 인덱스
	//int32 CurrentLocalPlayerIndex = INDEX_NONE; // 교체 전 State 인덱스
	//int32 NewLocalPlayerIndex = INDEX_NONE; // 교체 후 State 인덱스

	//for (int32 SI = 0; SI < AllStatesOfPlayerChar.Num(); ++SI)
	//{
	//	FPlayActorShowRuntimeState* ThisState = AllStatesOfPlayerChar[SI];
	//	// IsForLocalPlayerCharacter 에 해당하는 건 단 하나만 있어야 한다
	//	if (ThisState->IsForLocalPlayerCharacter())
	//	{
	//		CurrentLocalPlayerIndex = SI;
	//		break;
	//	}
	//}
	//// 교체할 플레이어 캐릭터는 뒤부터 해서 현재 Local 이 아닌 걸로. 만일 3 이상의 트랙이 있다면 뒤쪽 인덱스가 우선 순위.
	//// 범용적인 정식 사용으로 구현하고자 한다면 ShowSetting 에 switching 여부를 표시하던지 해야겠지만 그럴 필요까지 있는 기능인 것 같지는 않다.
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
	//	return; // 이런 경우 역시나 더 이상 진행 불가
	//}

	//FPlayActorShowRuntimeState* ShowStateToSwitch = AllStatesOfPlayerChar[NewLocalPlayerIndex];
	//FPlayActorShowRuntimeState* ShowStateOfOldLocalPlayer = AllStatesOfPlayerChar[CurrentLocalPlayerIndex];
	//// PlayActorAboutTobeSwitched 는 대체로 Dummy 로 spawn 한 액터가 될 것이다.
	//ABladeIIPlayer* PlayActorAboutTobeSwitched = Cast<ABladeIIPlayer>(ShowStateToSwitch->GetPlayActor(this));
	//// 곧 퇴장할 현재 로컬 플레이어 캐릭..
	//ABladeIIPlayer* CurrentLocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//UB2SkillRTManager* SkillRTManager = CurrentLocalB2Player ? CurrentLocalB2Player->GetSkillRTManager() : nullptr;
	//if (PlayActorAboutTobeSwitched && CurrentLocalB2Player && SkillRTManager)
	//{
	//	// 더미라도 캐릭터 Class 는 세팅되어 있는 거..
	//	const EPCClass NewPCClassEnum = PlayActorAboutTobeSwitched->GetCurrentPlayerClass();
	//	const EPCClass OldPCClassEnum = CurrentLocalB2Player->GetCurrentPlayerClass();
	//	if (SkillRTManager->LocalPlayerSwitching(NewPCClassEnum))
	//	{
	//		// 캐릭터 바꾸면서 TutorialGameMode에 MarkBeginAsyncLoadNextPCClass에 이벤트 날림
	//		MarkBeginAsyncLoadClass<>::GetInstance().Signal();
	//		// 새로 spawn 성공.. 이제 본격 ShowState 바꿔치기.
	//		ABladeIIPlayer* NewLocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//		ShowStateToSwitch->SetPlayActor(NewLocalB2Player);
	//		// 다른 visibility 세팅이 다 끝난 이후에 해야 해서.. 어쩔 수 없이 방금 spawn 한 캐릭터 Hidden 처리.
	//		NewLocalB2Player->SetHideFromEventDirector(true);
	//		
	//		// ForgetACharacter 의 경우를 참고. 실제로 이전까지 연결되어 있던 PlayActor 는 제거되는 상황이라 null-out 하는 게 적절함.
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
	// SetupSingleShowMatineeGroup 등 다른 곳에서도 세팅하지만 종종 꼬여서 마지막에 다시 해 주어야 할 때가 있음..

	// Controlled Matinee 모드의 PlayActor 를 숨기지 않아야 할 경우가 있으려나
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
	//// 여긴 여러번불려도 한번만 타게한다.
	//if (bEnterCinematicMode)
	//	return;

	//bEnterCinematicMode = true;

	//ABladeIIPlayerController* LocalB2PC = GetB2LocalPlayerCon();
	//if (LocalB2PC && OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//{
	//	bool bHiddenPlayer = LocalB2PC->GetPawn() && LocalB2PC->GetPawn()->bHidden;
	//	
	//	// MatineeActor 쪽에는 bNoCinematicModeControl 을 세팅해 놓고 CinematicMode 컨트롤을 직접 한다.
	//	LocalB2PC->SetCinematicMode(true, bHiddenPlayer, bHideHud, bDisableInput, true);
	//}
	//if(!bForceNoHidePlayer)
	//	SetAllPlayerHiddenInGame(true);
}

// bTryHidePlayer로 숨길지 말지 결정
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

	//		// CurrentPlayer건 MainPlayer건 SubPlayer건 현재 플레이하고 있는 캐릭터와 일치하면 싱크 대상
	//		if (CurrShowState.IsForLocalPlayerCharacter())
	//		{
	//			// 현재 세팅에서의 move 트랙을 찾아서 첫 키프레임을 목표 지점으로
	//			
	//			const FName FinalMatchingGroupName(*GetFinalMatchingInterpGroupName(LocalB2Player, CurrShowSetting.MatchingInterpGroupName.ToString(), false));
	//			UInterpGroupInst* CurrGroupInst = MatineeActor->FindFirstGroupInstByName(FinalMatchingGroupName.ToString()); // 요건 이름으로 바로 찾을 수 있는 듯.
	//			
	//			if (CurrGroupInst && CurrGroupInst->Group)
	//			{
	//				for (int32 TI = 0; TI < CurrGroupInst->Group->InterpTracks.Num(); ++TI)
	//				{
	//					UInterpTrackMove* MoveTrack = Cast<UInterpTrackMove>(CurrGroupInst->Group->InterpTracks[TI]);
	//					
	//					if (MoveTrack && CurrGroupInst->TrackInst.Num() > TI)
	//					{
	//						// UInterpGroup::UpdateGroup 보면 InterpTracks 과 TrackInst 인덱스 일치시켜서 사용.

	//						// 첫 키 프레임 값 빼옴.
	//						float KeyTime;
	//						FVector FirstKeyPosition;
	//						MoveTrack->ExternalGetKeyframePosition(CurrGroupInst->TrackInst[TI], 0, KeyTime, FirstKeyPosition, NULL, NULL);
	//						// 회전값은 기본 기능으로는 못맞춰줄 듯..
	//						//FVector FirstKeyRotEuler;
	//						//MoveTrack->ExternalGetKeyframeRotation(CurrGroupInst->TrackInst[TI], 0, KeyTime, FirstKeyRotEuler, NULL, NULL);

	//						// NavigationSystem 빌려서 이동. 다행히 PlayerController 로도 작동하는군 ㅋ
	//						UNavigationSystem::SimpleMoveToLocation(LocalB2Player->Controller, FirstKeyPosition);
	//						// PathFind 종료 콜백 등록.
	//						UPathFollowingComponent* PFComp = LocalB2Player->Controller->FindComponentByClass<UPathFollowingComponent>();
	//						if (PFComp)
	//						{
	//							// SimpleMoveToLocation 에서 한번 체크하는데 그 때에는 콜백이 안불린다. 등록도 안한 상태고 다른 문제도 있고. 어차피 이미 도달했으면 굳이 Sync 할 필요가 없음.
	//							if (PFComp->HasReached(FirstKeyPosition, EPathFollowingReachMode::OverlapAgent) == false)
	//							{
	//								PFComp->OnRequestFinished.AddUObject(this, &AB2StageEventDirector::OnPCSyncToMatineeStartCompleted);
	//								// Sync 중 표시. 이게 끝나면 본격 마티니 플레이 시작.
	//								bCurrentPCSyncToMatineeStart = true;
	//							}
	//						}

	//						// 마지막으로.. 특정 시간이 지나면 무조건 시작하도록 한다. 너무 멀리서 오게 되면 도중에 끊기겠지만, 어딘가에 막혀서 더 이상 진행이 되지 않는 것보단 낫겠지. 
	//						// 필요에 따라 변수로 노출..
	//						const float PCSyncFallBackTimerTime = 10.0f; 
	//						GetWorldTimerManager().SetTimer(PCSyncToMatineeStartFallbackTimerHandle, this, &AB2StageEventDirector::PCSyncToMatineeStartFallbackTimerCB, PCSyncFallBackTimerTime, false);

	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//// 실제 플레이하기 전까지 인게임 상태로 되돌려 놓는다. SetupSingleShowMatineeGroup 에서 한 일부를 되돌려 놓음.
	//if (bCurrentPCSyncToMatineeStart)
	//{
	//	for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
	//	{
	//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
	//		// 이 경우 SkeletalMeshActor 들은 처음부터 보여질 테니 숨겨서도 안되겠지.
	//		const bool bVisibleFromFirstPlace = (CurrShowSetting.PlayActorStateMode == EPlayActorStateMode::EPASM_DirectedFromBeginning);

	//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];
	//		ABladeIICharacter* ThisPlayActor = CurrShowState.GetPlayActor(this);
	//		if (ThisPlayActor)
	//		{
	//			// SetupSingleShowMatineeGroup 에서 숨겨놓은 것들을 다시 잠시 보이도록. 

	//			bool bOneOfTempSpawnedPuppet = false;
	//			for (int32 TPI = 0; TPI < TempSpawnedPuppets.Num(); ++TPI)
	//			{
	//				if (ThisPlayActor == TempSpawnedPuppets[TPI])
	//				{
	//					bOneOfTempSpawnedPuppet = true;
	//				}
	//			}

	//			// TempSpawnedPuppet 은 어차피 보여주면 안됨. 또한 SetStageEventShowState 도 SetupSingleShowMatineeGroup 에서 세팅한 그대로 놔둬야 숨겨진 채로 spawn 모션 같은 게 플레이 되지 않을 것.
	//			if (bOneOfTempSpawnedPuppet || bVisibleFromFirstPlace)
	//			{
	//				ThisPlayActor->SetHideFromEventDirector(true);
	//			}
	//			else
	//			{
	//				ThisPlayActor->SetHideFromEventDirector(false); // 나머지는 인게임 상태대로..

	//				// BeginSingleShow 에서 이게 기본으로 들어가 있어서 애니메이션이 나오지 않는데 마티니 모드에선 StageEventShowState 세팅이 중요한 게 아님 ㅋ
	//				ThisPlayActor->SetStageEventShowState(EStageEventShowState::ESES_None, NULL);
	//			}
	//		}
	//		if (CurrShowState.CtrlMatineePuppet)
	//		{
	//			SetVisibilityOfMatineePuppet(CurrShowState.CtrlMatineePuppet, CurrShowState.GetPlayActor(this), bVisibleFromFirstPlace);
	//		}
	//	}

	//	// 플레이어 캐릭터는 보이더라도 FinalBeginMatineePlay 하기 전에 여기서도 CinematicMode 진입이 필요.
	//	EnterCinematicModeForControlledMatinee();

	//	return true;
	//}

	return false;
}

void AB2StageEventDirector::BeginMatineeFromPCSyncToMatineeStart()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::BeginMatineeFromPCSyncToMatineeStart"));

	if (bCurrentPCSyncToMatineeStart)
	{
		bCurrentPCSyncToMatineeStart = false;

		// CheckForPCSyncToMatineeStart 에서 세팅한 거 다시 마티니 플레이를 위해 돌려놓는다. 다시 SetupSingleShowMatineeGroup 에서 해 놓은 상태로
		for (int32 EI = 0; EI < ShowStates.Num(); ++EI)
		{
			FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
			FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];

			if (!CurrShowSetting.IsDirectedMode())
			{
				continue;
			}

			ABladeIICharacter* ThisPlayActor = CurrShowState.GetPlayActor(this);
			if (IsValid(ThisPlayActor))
			{
				// 별로 중요한 건 아닌데.. ESES_None 으로 놓은 StageEventShowState 도 나름 의미있는 값으로 다시 세팅해 놓자. 사실 마티니 모드에선 큰 의미는 없다..
				ThisPlayActor->SetStageEventShowState(GetDefaultDesiredPlayActorSESState(DirectingEvent, CurrShowSetting.PlayActorType), this);
				
				// PlayActor 들 숨기는 건 아래 FinalBeginMatineePlay 에서
			}
			if (CurrShowState.CtrlMatineePuppet)
			{
				SetVisibilityOfMatineePuppet(CurrShowState.CtrlMatineePuppet, CurrShowState.GetPlayActor(this), true);
			}
		}

		FinalBeginMatineePlay(); // 시작

		DeferredBeginShowHandlingForMatineePCSync(); // BeginShow 에서 처리 못했던 것들 마저 처리.
	}
}

void AB2StageEventDirector::OnPCSyncToMatineeStartCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	BeginMatineeFromPCSyncToMatineeStart();
}

void AB2StageEventDirector::PCSyncToMatineeStartFallbackTimerCB()
{
	GetWorldTimerManager().ClearTimer(PCSyncToMatineeStartFallbackTimerHandle);
	BeginMatineeFromPCSyncToMatineeStart(); // 대체로 이미 BeginMatineeFromPCSyncToMatineeStart 가 처리되었을 것인데 그런 경우면 아무 일 안일어날 것.
}

ABladeIICharacter* AB2StageEventDirector::GetRelevantPlayActor(EStageEventPlayActor InType, FString& WaveMobSelectKeyword, int32 PlayerIndex)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::GetRelevantPlayActor"));

	//ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();

	//if (InType == EStageEventPlayActor::ESEE_CurrentPlayer)
	//{
	//	if (LocalB2Player)
	//	{ // 죽고있는 거라면 연출 후에 sync 를 할 필요도 없겠고.. 여하간 위험할 수 있으니 puppet 으로?
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
	//		// 현재 플레이하고 있는 캐릭터라면 새로 spawn 하거나 할 필요없이 그대로 가져와서 사용.
	//		// 그렇지만 죽고있는 거라면 연출 후에 sync 를 할 필요도 없겠고.. 여하간 위험할 수 있으니 puppet 으로?
	//		return LocalB2Player->IsAlive() ? LocalB2Player : LocalB2Player->SpawnMyselfAsPuppet();
	//	}
	//	else
	//	{
	//		// 아니면 상대쪽을 player controller 에 possess 되지 않은 채로 spawn 해서 사용.
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
	//else if (InType == EStageEventPlayActor::ESEE_RemoteOpponent_Current) // RemoteOpponent_Other 는 필요해지면 추가. ESEE_WinningPlayer_Other 참고해서
	//{
	//	return GetB2MatchOpponent();
	//}
	//else if (InType == EStageEventPlayActor::ESEE_TeamAllyPlayer || InType == EStageEventPlayActor::ESEE_TeamEnemyPlayer)
	//{
	//	return GetTeamPlayer(InType == EStageEventPlayActor::ESEE_TeamAllyPlayer, PlayerIndex, false, true); // 지는 경우에 이걸 사용하려면 live 만 가져오면 아무도 없을 것. 죽은 애는 껍데기라도 좀비처럼 부활시켜서 데려온다.
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
	//	//	ParseForWaveMobSelectKeyword(WaveMobSelectKeyword, WaveNum, MobIndex); // 문자열로 지정한 옵션에서 번호들 빼옴.

	//	//	TArray<ABladeIICharacter*> InterestedWaveMobs;
	//	//	B2GM->GetSingleWaveMobList(WaveNum, InterestedWaveMobs, false); // 죽어가는 녀석도 포함해서 가져옴. 물론 그게 가능한 타이밍이어야 함. 아닌 경우 아래 DelayedDyingMobCache 찬스가 있긴 하다.

	//	//	for (int32 WMI = 0; WMI < InterestedWaveMobs.Num(); ++WMI)
	//	//	{
	//	//		ABladeIICharacter* CurrMob = InterestedWaveMobs[WMI];
	//	//		if (CurrMob)
	//	//		{
	//	//			if (CurrMob->GetSpawnPoolWaveObjIndex() == MobIndex) // 사실상 Wave 가 시작된 상황이 아니면 찾는 인덱스의 mob 이 없을 수도 있다.
	//	//			{
	//	//				return CurrMob;
	//	//			}
	//	//		}
	//	//	}
	//	//	// 위에서 리턴을 하지 않았다면 MobDying 에서 delay 를 준 경우일 수 있다. 모든 dying mob 이 가능한 건 아니지만 트리거를 유발한 dying mob 까지는 아래처럼 가져올 수 있음..
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
//	// 현재 로컬 캐릭터부터 체크. 여전히 (PendingKill 이 아닌) 로컬 캐릭터와 일치한다면 그걸 가져다 쓴다.
//	ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
//	if (LocalB2Player && LocalB2Player->IsValidObj() && LocalB2Player->GetCurrentPlayerClass() == PCClassEnum) {
//		return LocalB2Player;
//	}
//
//	// 튜토리얼 게임모드라면 미리 이 목적으로 생성해놓은 FallbackPuppet 들을 사용
//	AB2TutorialGameMode* TutorialGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this));
//	if (TutorialGM)
//	{
//		return TutorialGM->GetFallbackPuppetPlayer(PCClassEnum);
//	}
//#if WITH_EDITOR
//	// 에디터라면 아직 TutorialGameMode 설정하지 않은 상태에서 기능 점검이 필요하므로..
//	// 이걸 일반적인 게임모드 차원으로 확장하는 건 곤란하다. 
//	// 기능 자체야 동작하겠지만 잘 사용하지도 않을 이런 기능때문에 FallbackPuppet 들을 생성해 놓아야 하는 게 낭비.
//	else if (GIsEditor)
//	{
//		//
//		// 개발용 임시코드이므로 크게 신경쓰지 않고 여기에 그냥 막 짜는 거.
//		//
//
//		ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//		if (B2GM)
//		{
//			TSubclassOf<APawn> DefaultPawnClassBackup = B2GM->DefaultPawnClass;
//			// 더미들 spawn 시켜야 해서 잠깐 딴 클래스로 바꿔놓고..
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
		// 이 때 spawn animation 은 없도록 할 필요가 있을 거 같음.. 적어도 이게 불리는 게 시작 상황은 아닐 듯함.
		ABladeIIPlayer* NewPuppet = LocalB2Player->SpawnOtherAsPuppet(true,
			false // 태그 캐릭터 쪽 puppet 의 point light 가 켜져야 한다면 추가 인자를..
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
	//// 현재는 PVP 모드 opponent 만
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

		// TODO : 다른 카메라 set. 필요하려나 모르겠지만..
	}

	return NULL;
}

void AB2StageEventDirector::CheckShowEndCountDown()
{
	if (IsOverallLengthConstrainted()) // OverallLengthConstrainted 라면 따로 설치한 타이머에 의해 종료될 것.
	{
		return;
	}

	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee) // Controlled Matinee 는 다른 경로를 통해 종료 체크
	{
		return;
	}

	// 전체 show 가 종료되는 조건들을 검사한다.

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
		// FinishCheck 가 필요없는 세팅이 있음.
		if (DoesShowRequireFinishCheck(SI) == true && ShowStates[SI].bShowFinished == false)
		{
			return;
		}
	}

	// 마지막으로 bStandbyAtTheEnd 를 체크.. 이 경우는 물론 CustomFinishShow 를 해야
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
		CustomFinishShow(); // 여기까지 왔다면 끝. FinishShow 가 아닌 CustomFinishShow 로 끝낸다.
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
			CurrSetting.IsDirectedMode() && // InGame State 그대로면 딱히 시간 등의 다른 조건을 주지 않는 이상 종료 체크를 할 수가 없음.
			PlayActor && PlayActor->GetStageEventShowState() != EStageEventShowState::ESES_None &&
			PlayActor->GetStageEventShowState() != EStageEventShowState::ESES_Extra // Extra 는 아마도 SESEndNotify 없이 단순히 Idle 을 돌리게 될 것..
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
//	if (!bCurrentlyPlaying){ // 중복처리가 될 수 있음.
//		return;
//	}
//
//	// CustomFinishShow 같은 상황이면 미처 처리가 안된 상황일 수 있어서 타이머들을 다시 한번 확실히 클리어 한다.
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
//		WorldTimerManager.ClearTimer(PCSyncToMatineeStartFallbackTimerHandle); // 얘는 실제 연출 기능하고 따로 도는 타이머라 더더욱 이렇게 꺼주어야 함
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
//	//if (IsUsingMobileRendering(GetWorld())) // 캡쳐 영상 만들 때 라이트 끄지 않으려고 이런 체크가 필요한 것 같은데 StageEventDirector 는 인게임 기능이라 이런 체크가 필요할까 싶기도.
//	//{
//	//	// 모바일 렌더러에서 dynamic point light 개수 제한을 위해. 연출 동안 사용한 게 있을 꺼라 종료하면서 확실히 꺼두는 거. 필요한 건 다음 연출 들어갈 때 다시 켜질 것.
//	//	if (B2GM)
//	//		B2GM->DeactivateWorldMovablePointLight();
//	//}
//
//	ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
//	ABladeIIPlayerController* LocalB2PC = GetB2LocalPlayerCon();
//
//	// bStandbyPause 에 의해 pause 된 상태를 돌려줌
//	if (bStandbyAtTheEnd && bStandbyPause && UGameplayStatics::IsGamePaused(this))
//	{
//		UGameplayStatics::SetGamePaused(this, false);
//	}
//		
//	// 네이티브에서는 이제 이렇게 이벤트를..
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
//			// 여길 거치면서 ThisState 의 PlayActor 가 제거될 수 있다.
//			if(ThisSetting.PlayActorType != EStageEventPlayActor::ESEE_SetUpOutside)
//				RestorePlayActorForCtrlMatineeEnd(ThisSetting, ThisState);
//		}
//						
//		ABladeIICharacter* ThisPlayActor = ThisState.GetPlayActor(this);
//		if (ThisPlayActor)
//		{
//			ThisPlayActor->AllowMovementByStageEvent(); // PreventMovement 하는 케이스가 있는데 걍 모두 복구.
//
//			if (ThisSetting.EndActionPlayActorAdjustType == EEndActionPlayActorAdjustType::EAPAAT_BackToOriginal)
//			{
//				// PlayActor 의 위치 및 회전값을 복원시킴.
//				ThisPlayActor->SetActorLocation(ThisState.BackupWorldLocation);
//				ThisPlayActor->SetActorRotation(ThisState.BackupWorldRotation);
//			}
//
//			// CustomFinish 혹은 Extra 인 경우 NotifyStageEventShowEnd 가 실행되지 않았을 것. 
//			// 이쪽으로의 종료 notify 없이 상태만 종료. bStandbyAtTheEnd 인 경우도 다시 Extra 로 되돌리니 필요..
//			ThisPlayActor->SetStageEventShowState(EStageEventShowState::ESES_None, NULL);
//			ThisPlayActor->SetPlayingDirectedStageEvent(false);
//		}
//
//		ThisState.bShowFinished = true; // 확실한 마크
//		ThisState.SetPlayActor(NULL); // 퇴장 ㅋ
//	}
//	ShowStates.Empty();
//	DelayedDyingMobCache.ForgetIt();
//
//	// 얘들은 직접 제거해 주어야 함.
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
//	FinishEventWidget(); // EventWidgetEndTime 세팅의 경우 이미 종료되었을 수 있다.
//
//	if (bStopAllMobsWhilePlaying && CachedStageManager)
//	{ // BeginShow 에서 Stop 시켰던 것들 다시 되살림.
//		CachedStageManager->StopOrRestoreAllWorldMobs(false);
//	}
//
//	if (LocalB2PC)
//	{
//		// CinematicMode 종료. 아래 NotifyShowOverFromDependentDirectorActor 이후에 들어가면 개곤란
//		LocalB2PC->SetCinematicMode(false, false, true, true, true);
//	}
//	// 숨겼던 블레이어 보이게
//	if(!bForceNoHidePlayer)
//		SetAllPlayerHiddenInGame(false);
//
//	// 전투 UI 오프닝 애니메이션을 플레이할 필요가 좀 있어서 이런 뻘짓을 함.
//	if (IsPossibleAdventureModeBeginEvent())
//	{
//		// todo : 요기 하드코딩 부분 바꿀필요가 있음요 =ㅇ= (상속 형태로)
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
//		// todo : 요기 하드코딩 부분 바꿀필요가 있음요 =ㅇ= (상속 형태로)
//
//		// pvpUI
//		UB2UIPVP1on1BattleMain* PVPBattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIPVP1on1BattleMain>(UIFName::PVPOneOnOneBattleMain);
//		if (PVPBattleMainUI)
//		{
//			PVPBattleMainUI->OnPVPBeginEventSceneEnd();
//		}
//
//		// 팀대전UI - 이것도 동일한 UIName 을 사용함.
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
//	// 이전에 다르게 들어갔을 수도 있지만 정상으로 돌려준다 치자.
//	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
//
//	// 플레이어 TopDown 카메라 내지는 ActiveCamera 로..
//	ReturnToGamePlayCamera();
//
//	LevelStreamingOnEnd.ExecuteStreaming(GetWorld(), LevelStreamingOnEndFlushTH, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::LevelStreamingOnEndFlushCB));
//
//	// Auto 상태 복구
//	if (ShouldStopAutoDuringShow() && B2GM && B2GM->IsLocalPlayerAutoBattle()) // 설령 시작할 때에는 Auto 가 아니었다 하더라도 중간에 어떤 사유로든 바뀌면 Auto 를 복구해준다.
//	{
//		B2GM->RestoreLocalPlayerAuto();
//	}
//
//#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
//	if (bPlayedAtLeastOnce == false)
//	{			
//		bPlayedAtLeastOnce = true; // 이번 스테이지에서 최소한 한번 플레이 하였음.
//		GConfig->SetBool(*GetPlayedAtLeastOnceSectionString(), *GetPlayedAtLeastOnceKeyString(CurrentStageId), bPlayedAtLeastOnce, GB2LocalCutSceneSavedStateIni);
//		GConfig->Flush(false, GB2LocalCutSceneSavedStateIni);
//	}
//#endif
//
//	if (LocalB2Player) 
//	{ // 기타 등등..
//		LocalB2Player->SetupLODInfoForInGame(); // 로컬 플레이어는 연출 참여 여부 상관없이 무조건 CinematicLOD 설정하니 되돌림.
//		LocalB2Player->SetInvincibleByStageEvent(false);
//	}
//	B2Scalability::ApplyStageEventSpecificScalabilitySetting(this, false); // 로컬 플레이어 LOD 세팅과 마찬가지로 연출 용 특별 scalability 컨트롤 적용 리셋
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
	checkSlow(!bCurrentlyPlaying); // 확실히 끝난 플래그 들어온 후에 작동 가능한 것들..

	//// BP 에서 특별히 이벤트를 받아서 처리하고자 한다면..
	//if (ShowOverEventName != NAME_None)
	//{
	//	GEngine->HandleCeCommand(GetWorld(), *(ShowOverEventName.ToString()), *GLog);
	//}

	//if (IsPossibleAdventureModeBeginEvent())
	//{ // BeginShow 에서도 Async 요청하는 게 있는데 이것도 보스 등장 시 hitch 를 유발하니 적당한 시점에..

	//	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
	//	// 여기서의 Async 로딩 요청은 주의할 필요가 있는데 여기서 요청을 하면 인게임 전투가 돌아가는 와중에 로딩이 돌아갈 것이다.
	//	// 이땐 그것까지 생각을 못 했고 이 정도는 정말 작은 것이니 그냥 봐준다. 
	//	// 이런 타이밍에 async 로딩 요청이 더 추가되면 곤란할 것이다.
	//	UB2UIManager::GetInstance()->ReqAsyncLoadAsset(UIFName::MobAppearBossStage);
	//}

	//ABladeIIGameMode* B2GM = GetB2GameMode();
	//if (B2GM)
	//{		
	//	if (DirectingEvent == EStageEvent::ESEV_Begin && 
	//		(B2GM->GetB2GameModeType() == EB2GameMode::PVP_Tag || B2GM->GetB2GameModeType() == EB2GameMode::PVP_Team || B2GM->GetB2GameModeType() == EB2GameMode::Guild))
	//	{// MatchGameStartEventSceneEnd 시그널 적용 대상 게임모드들 모두..
	//		MatchGameStartEventSceneEndClass<>::GetInstance().Signal();
	//	}

	//	B2GM->MarkAnyStageEventPlayedOnce(); // 어쨌든 최소 하나 플레이 됨. ㅋ

	//	// 내가 끝나길 기다리고 있는 다른 DirectorActor 가 있다면 Notify 를 날린다. 다른 곳에서 바로 시작이 될 수 있으므로 여기의 종료처리와 중첩되지 않도록 끝에 가서.
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
	//	// SetupSingleShowMatineeGroup, EnsureHidePlayActorsForControlledMatinee 등에서 숨겨놓은 거 복구.
	//	if (ThisPlayActor->bIsDying && InSettingForThis.IsDirectedMode())
	//	{ // 단, 마티니로 죽는 연출이 나오는 애는 안보이게 좀 처리해 주어야 한다.
	//		
	//		// 걍 Destroy 로 제거해 버리면 간단한데 이거 이후에 SpawnPool 에서 처리해야 하는 상황이 나올 수 있어서.. 좀 위험함. 애초에 Destroy 라면 기본적인 루트 이외에는 조심해야겠다.
	//		
	//		ThisPlayActor->SetHideFromEventDirector(true);
	//		if (ThisPlayActor->GetMesh()){				
	//			ThisPlayActor->GetMesh()->GlobalAnimRateScale = 0.0f; // 이렇게 하면 애니메이션을 완전히 중단시킬 수 있다.
	//			// 근데 이렇게 하면 OnAnimMobDeadEnd 로 가지 않게 되어 Destroy 가 안된다. 아마 이게 나오는 상황은 스테이지 종료 상황일 거라 어떻게 하든 큰 문제는 없을 거 같은데.. 바로 Destroy 만 하지 않는 걸로..
	//			ThisPlayActor->SetLifeSpan(1.0f);
	//		}

	//		// 다른 방법으로는 LobbySkeletalMeshActor 쪽에 한 것 처럼 AnimNotify 만 제거하는 방법이 있음. 가장 원만한 해결책이 될 건데 구현을 좀 해 주긴 해야..
	//					
	//		InStateForThis.SetPlayActor(NULL);
	//	}
	//	else
	//	{
	//		ABladeIIPlayer* CastedPlayerPlayActor = Cast<ABladeIIPlayer>(ThisPlayActor);
	//		if (CastedPlayerPlayActor) 
	//		{
	//			if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
	//			{ // 섹션 병합 모델을 사용하는 모드에서는 연출 끝나고 다시금 섹션 병합된 버전으로 돌려놓는다.
	//				CastedPlayerPlayActor->SetUseSectionMergedMesh(true);
	//			}

	//			CastedPlayerPlayActor->SetupLODInfoForInGame(); // 로컬 플레이어만 아니라 네트워크 플레이어나 태그 플레이어도 있으니 다 LODInfo 리셋
	//		}

	//		ThisPlayActor->SetHideFromEventDirector(false);
	//		ThisPlayActor->RestoreTemporaryMeshComponentUpdateFlag(); // NeedSpawnCheck 해제하면서 TickPose 강제했던 거 해제. 성능상.
	//	}

	//	if (InSettingForThis.EndActionPlayActorAdjustType == EEndActionPlayActorAdjustType::EAPAAT_AdjustToPuppet && InStateForThis.CtrlMatineePuppet) // bPutPlayActorBackToOriginalLocation 이 아니란 얘기는 Matinee 종료 위치로 싱크
	//	{
	//		const float B2CharHeightOffset = ZOffsetFromMatchingSKActorToBIICharacter(ThisPlayActor);

	//		ThisPlayActor->SetActorLocation(InStateForThis.CtrlMatineePuppet->GetActorLocation() + FVector(0.0f, 0.0f, B2CharHeightOffset));
	//		if (Cast<ABladeIIPlayer>(ThisPlayActor))
	//		{ // 플레이어 캐릭터라면 회전값은 yaw 만 가져온다.
	//			ThisPlayActor->SetActorRotation(FRotator(0.0f, InStateForThis.CtrlMatineePuppet->GetActorRotation().Yaw, 0.0f));
	//		}
	//		else
	//		{ // 일반 몹이라면 roll 이나 pitch 도 연출에서 연속적으로 non-zero 값을 쓸 일이 있을지도?
	//			ThisPlayActor->SetActorRotation(InStateForThis.CtrlMatineePuppet->GetActorRotation());
	//		}
	//	}
	//}

	//// Matinee 를 위해 맵에 배치된 애들로서 보이지만 않게 둠. 제거는 하지 말고.
	//// 만일 얘들을 제거하려면 Matinee 쪽에서 최소한 TermInterp 가 처리된 다음이어야 한다. 기본적인 사용에서 AMatineeActor::Stop 에서 이쪽으로의 notify 가 온 후에 TermInterp 가 처리되므로 제거하고 싶더라도 이 시점에서는 안됨
	//// -> MatineePuppet (즉 그룹액터) 들을 재활용 하기 위해 제거하지 않는다. 재활용을 가능하게 하기 위한 처리가 ABladeIICharacter::DoesMatineePuppetSetupFor** 들임.
	//if (InStateForThis.CtrlMatineePuppet)
	//{
	//	if (InSettingForThis.PlayActorStateMode != EPlayActorStateMode::EPASM_DirectedAfterFinish)
	//	{
	//		if (ThisPlayActor)
	//		{ // 라이트 꺼주고 기타 PlayActor 에 따라 Setup 했던 거 clear			
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

	CheckShowEndCountDown(); // 전체적인 종료 조건 검사.
}

void AB2StageEventDirector::OnControlledMatineeStop(bool bStandByNoti)
{
	bControlledMatineeFinished = true;
	CheckShowEndCountDown_ControlledMatinee(bStandByNoti); // 추가 검사할 조건이 있음.
	if (!GIsEditor)
	{ // 요 SetCloseBattleMainUIClass 이벤트를 에디터에서 Init-Destroy 시에 Subscribe/Unsubcribe 하는데 에디터에서 Unsubscribe 가 빠지기 쉬움. ㅠㅠ
		SetCloseBattleMainUIClass<bool>::GetInstance().Signal(true);
	}
}

void AB2StageEventDirector::OnExportedCameraAnimStop()
{
	bCameraAnimFinished = true;
	CheckShowEndCountDown(); // 전체적인 종료 조건 검사.
}

void AB2StageEventDirector::OnAllManualCameraOver()
{
	bAllManualCameraEnded = true;
	CheckShowEndCountDown(); // 전체적인 종료 조건 검사.
}

void AB2StageEventDirector::ReturnToGamePlayCamera(bool bDeactivateOtherCams /*= false*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::ReturnToGamePlayCamera"));

	//ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
	//ABladeIIPlayerController* LocalB2PC = GetB2LocalPlayerCon();

	//// 여기서 플레이어 카메라 쪽으로 blending out 을 제대로 먹이려면 DeactivateAllManualCams 를 해선 안됨. 
	//// 그러면 이쪽에서 active 한 camera component 가 없어서 AActor::CalcCamera 에서 단순히 액터 위치를 사용하게 될 것.
	//if (bDeactivateOtherCams)
	//{
	//	DeactivateAllManualCams();
	//}

	//CurrentActiveCamera = NULL;

	//// 마티니 사용 시의 카메라 복귀 구현은 다를 수 있을... 까?

	//if (LocalB2Player)
	//	LocalB2Player->TopDownCameraComponent->Activate();

	//if (ActiveCameraForceOnFinish)
	//{// 영역 안이 아니어도 적용. 이런 사용이 필요하게 될 수도..
	//	// 그렇다고 해서 영역 안에 다른 카메라가 있는데 ActiveCameraForceOnFinish 로 강제하기 위한 목적은 아니다. 대체로 영역으로 적용하기 곤란할만한 경우에 확실하게 적용하기 위한 목적.
	//	ActiveCameraForceOnFinish->ForceBeginActiveCamera(FinalCameraRestoreBlendTime); 
	//}
	//else
	//{
	//	// ActiveCameraForceOnFinish 보다는 여기로 오는 게 대체로 더 바람직하고 자연(?)스럽다.
	//	// 여기서도 FinalCameraRestoreBlendTime 이 먹히도록 ApplyActiveCameraIfAny 에 blend time 오버라이딩 가능하도록 함. 연출에서 ActiveCamera 로 돌아갈 때의 블렌딩은 연출쪽 설정으로.
	//	ApplyNormallyExpectedInGameCamera(GetB2GameMode(), LocalB2PC, FinalCameraRestoreBlendTime);
	//}

	//if (LocalB2PC && LocalB2PC->PlayerCameraManager)
	//{
	//	//강제로 카메라 업데이트 -> 사실 APlayerCameraManager::SetViewTarget()을 호출하게 되면,
	//	//APlayerCameraManager::TransformComponent의 AttachedChild 위치가 안맞는 문제가 있어서(카메라위치랑 TransformComponent 위치가 틀어짐.) APlayerCameraManager::SetViewTarget()에서 하려고 했는데
	//	//어떤 사이드이펙트가 생길지 몰라서 이곳에 집어넣는다.
	//	LocalB2PC->PlayerCameraManager->UpdateCamera(0.f);
	//}
}

void AB2StageEventDirector::DeactivateAllManualCams()
{
	for (int32 CI = 0; CI < MAX_DIRECTOR_CAMERA; ++CI)
	{
		DirectorCamArrayRef[CI]->Deactivate();
	}

	// TODO 다른 액터쪽에 설치할 카메라들도 -> 없을 거 같다.
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

	//	if (NextCamera) // 만일 NextCamera 를 찾지 못하면 종료가 되기 전까지는 현상태가 유지될 것.
	//	{
	//		if (CurrentActiveCamera && CurrentActiveCamera != LocalB2Player->TopDownCameraComponent)
	//		{
	//			// 이걸 Deactivate 해야 하는 상황은 같은 actor 안의 다른 카메라 컴포넌트들 간에 블렌딩 하는 경우임.
	//			// 따라서 여기 들어오는 조건에 CurrentActiveCamera 가 플레이어의 TopDownCamera 라면 deactivate 를 하지 않는다.
	//			// 다른 경우가 또 있다면 추가 검사를 하던지, outer 검사를 하던지 하는 식으로..
	//			CurrentActiveCamera->Deactivate();
	//		}
	//		CurrentActiveCamera = NextCamera;
	//		CurrentActiveCamera->Activate();
	//		LocalB2PC->SetViewTargetWithBlend(this, CurrCamSetting.BlendTimeFromPrev, CurrCamSetting.BlendFunction);

	//		// BlendTime 에 맞춰서 ManualCameraBlendCB 타이머 설치.
	//		// SetViewTargetWithBlend 에 대한 완료 콜백이 있는지 모르겠는데.. 시간 맞춰서 타이머 세팅함. (SetViewTargetWithBlend 에 대한 콜백이 있는 거라면 뻘짓 ㄷㄷ) 
	//		if (CurrCamSetting.BlendTimeFromPrev > 0.0f)
	//		{
	//			GetWorldTimerManager().SetTimer(ManualCameraBlendTimerHandle, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::ManualCameraBlendCB, CamSettingIndex), CurrCamSetting.BlendTimeFromPrev, false);
	//		}
	//		else
	//		{
	//			ManualCameraBlendCB(CamSettingIndex); // 가급적 이렇게 쓰고 싶지는 않았지만..
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
	//	// bStandbyAtTheEnd 상관없이 신호를 보낸다. CheckShowEndCountDown 에서 bStandbyAtTheEnd 에 따른 처리를 할 것.
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
	CustomFinishShow(); // 강제 종료.
}

void AB2StageEventDirector::ManualCameraBlendCB(int32 CamSettingIndex)
{
	GetWorldTimerManager().ClearTimer(ManualCameraBlendTimerHandle);

	if (CamSettingIndex >= 0 && CamSettingIndex < ManualCameraSettings.Num())
	{
		FManualCameraSingleSetting& CurrCamSetting = ManualCameraSettings[CamSettingIndex];

		if (CurrCamSetting.StayTime > 0.0f)
		{
			// 이 경우는 인덱스를 유지한 채 좀 더 머문다.
			GetWorldTimerManager().SetTimer(ManualCameraBlendTimerHandle, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::ManualCameraBlendStayCB, CamSettingIndex), CurrCamSetting.StayTime, false);
		}
		else
		{
			// 다음 카메라로. 만일 다음 카메라가 없다면 카메라 워크는 종료 처리가 될 것.
			BeginTargetToManualCamera(CamSettingIndex + 1);
		}
	}
}

void AB2StageEventDirector::ManualCameraBlendStayCB(int32 CamSettingIndex)
{
	GetWorldTimerManager().ClearTimer(ManualCameraBlendTimerHandle);

	// 다음 카메라로. 만일 다음 카메라가 없다면 카메라 워크는 종료 처리가 될 것.
	BeginTargetToManualCamera(CamSettingIndex + 1);
}
//
//void AB2StageEventDirector::SetSESPlayActorMovementFromCamera(FSimpleEventSceneMoveKeyData& InMoveKey, class ABladeIICharacter* InPlayActor)
//{
//	if (InPlayActor && CameraAnimApplyCamIndex >= 0 && CameraAnimApplyCamIndex < MAX_DIRECTOR_CAMERA)
//	{
//		// UB2SimpleEventScene::CreateFromInterpTrack 참고
//
//		UCameraComponent* BaseCamComp = DirectorCamArrayRef[CameraAnimApplyCamIndex];
//		
//		// SimpleEventScene 의 키값을 CameraAnim 이 적용되는 카메라를 기준으로 한 상대좌표로 간주.
//		// 단, SimpleEventScene 을 찍을 때는 피벗이 하단에 있는 SkeletalMeshActor 를 사용했을 것이므로 캡슐 컴포넌트의 절반만큼 올려준다.
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
//		// UB2SimpleEventScene::CreateFromInterpTrack 참고
//
//		UCameraComponent* BaseCamComp = DirectorCamArrayRef[CameraAnimApplyCamIndex];
//
//		// SetSESPlayActorMovementFromCamera 와는 반대로 InPlayActor 를 기준으로 카메라 위치 세팅
//		// 캡슐 컴포넌트 offset 은 마찬가지로 감안해야 함.
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
	// 여기서 return 하는 건 일반적인 사용에서 예상되는 값이고 StageEventStateOverride 가 ESES_None 이 아니면 오버라이딩 될 것.

	if (InEvent == EStageEvent::ESEV_Begin)
	{
		return EStageEventShowState::ESES_Appearing; // PlayActor 종류에 상관없이 공통일 듯
	}
	else if (InEvent == EStageEvent::ESEV_MobAppear)
	{
		if (InPlayActorType == EStageEventPlayActor::ESEE_WaveMob)
		{
			return EStageEventShowState::ESES_Appearing;
		}
		else if (IsPlayActorTypeForPlayerCharacter(InPlayActorType))
		{
			return EStageEventShowState::ESES_Extra; // 이 경우 플레이어가 보이지 않더라도 다른 동작으로 카메라 애니메이션을 망친다거나 하지 않게.. Extra 인 세팅은 따로 종료체크를 하지 않는다. 다른 게 끝나면 같이 끝남.
		}
	}
	else if (InEvent == EStageEvent::ESEV_MobDying)
	{
		if (InPlayActorType == EStageEventPlayActor::ESEE_WaveMob)
		{
			return EStageEventShowState::ESES_Dead; // MobDying 이라고 해도 다른 Setting 의 몹이 죽는 것일 수 있다. 그런 경우 StageEventStateOverride 로 직접 state 를 지정.
		}
		else if (IsPlayActorTypeForPlayerCharacter(InPlayActorType))
		{
			return EStageEventShowState::ESES_Extra; // 마찬가지로 이 경우 플레이어가 보이지 않더라도 다른 동작으로 카메라 애니메이션을 망친다거나 하지 않게.
		}
	}
	else if (InEvent == EStageEvent::ESEV_Clear)
	{
		if (InPlayActorType == EStageEventPlayActor::ESEE_MainPlayer || InPlayActorType == EStageEventPlayActor::ESEE_CurrentPlayer || InPlayActorType == EStageEventPlayActor::ESEE_OtherPlayer)
		{
			return EStageEventShowState::ESES_VictoryMain;
		}
		else if (InPlayActorType == EStageEventPlayActor::ESEE_SubPlayer)
		{
			return EStageEventShowState::ESES_VictorySub;
		}
		else if (InPlayActorType == EStageEventPlayActor::ESEE_WaveMob)
		{
			return EStageEventShowState::ESES_Dead; // 이런 경우가 예상되지는 않지만..
		}
	}

	// StageEvent 나 PlayActor 타입에 따라 추가..

	return EStageEventShowState::ESES_None;
}

void AB2StageEventDirector::SyncDataToUIDoc(ABladeIICharacter* InEventTriggeringMob)
{// UI 에서 쓸 데이터들 세팅.
	//BII_CHECK(!InEventTriggeringMob || InEventTriggeringMob->IsValidObj());
	//UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();	
	//if (DocBS && InEventTriggeringMob && InEventTriggeringMob->IsValidObj())
	//{
	//	UB2NPCSingleClassInfo* EventTriggerMobInfo = InEventTriggeringMob->GetMyNPCClassInfo();

	//	DocBS->SetCurAppearedMobName(EventTriggerMobInfo ? EventTriggerMobInfo->GetLocalizedName() : FText::FromString(TEXT("")));
	//	DocBS->SetCurAppearedMobNickName(EventTriggerMobInfo ? EventTriggerMobInfo->GetLocalizedNickName() : FText::FromString(TEXT("")));

	//	// 기타 필요한 것들 추가되면.. 
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
	// StartEventWidget 메인 격.

	if (EventWidgetType > EStageEventWidgetType::None)
	{
		if (EventWidgetStartTime > 0.0f)
		{
			GetWorldTimerManager().SetTimer(StartEventWidgetTimerHandle, this, &AB2StageEventDirector::StartEventWidgetTimerCB, EventWidgetStartTime, false);
		}
		else
		{
			StartEventWidget(); // 바로 시작
		}
	}
}

void AB2StageEventDirector::StartEventWidget()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::StartEventWidget"));

	//switch (EventWidgetType)
	//{
	//	//
	//	// 여기서 열리는 UI 들은 IsStageEventRelatedUI 쪽에서 return true 처리해 주도록.
	//	//

	//	case EStageEventWidgetType::StageBegin:	
	//		{
	//			if (CachedStageManager && CachedStageManager->GetIsShowTalkDialog())
	//			{
	//				// 대화설정이 있으면 대화 시스템부터 노출함.
	//				AB2StageGameMode* StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(this));
	//				if (StageGM && StageGM->GetActiveSpawnPool())
	//				{
	//					int32 GroupID = 0;
	//					MyRelevantUIName = UIFName::StageBegin; // MyRelevantUIName 을 OpenUI 이전에 세팅할 것. 일부 특수케이스를 제외하면 OpenComplete 이벤트가 Open 과 동시에 날아올 것이다.
	//					if(StageGM->GetActiveSpawnPool()->TryGetStartTalkSetting(GroupID))
	//						StageEventTalkStartClass<int32, bool>::GetInstance().Signal(GroupID, false);
	//					else 
	//						UB2UIManager::GetInstance()->OpenUI(UIFName::StageBegin);
	//				}
	//			}
	//			else
	//			{
	//				MyRelevantUIName = UIFName::StageBegin; // MyRelevantUIName 을 OpenUI 이전에 세팅할 것. 일부 특수케이스를 제외하면 OpenComplete 이벤트가 Open 과 동시에 날아올 것이다.
	//				UB2UIManager::GetInstance()->OpenUI(UIFName::StageBegin);
	//			}
	//		}
	//		break;
	//	case EStageEventWidgetType::StageClear:
	//		{
	//			// StageClear 만 좀 특별하게 취급해서 UIScene 으로 간다.
	//			// Stage 종료 UI 가 둘로 나뉘면서 여기서는 UIStageClear 대신 UIStageResult 를 연다. UIStageClear 는 약간 다르게 들어감. AB2StageEventDirector::BeginShow, AB2StageManager::BeginStageClearScene_Phase2 참고.
	//			if (GetB2GameModeType(this) == EB2GameMode::Scenario)
	//			{					
	//				MyRelevantUIName = UIFName::StageResult;
	//				UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::StageResult);
	//			} 		
	//			else if (GetB2GameModeType(this) == EB2GameMode::CounterDungeon) //EStageEventWidgetType을 따로 추가할까..
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
	//		MyRelevantUIName = UIFName::PVPResult; // 여기도.. 결국 승리창 UI 는 UIScene 으로
	//		//[@AKI, 170613] 약 네트워크에서 결과를 서버에서 받지 않은 상태에서 결과화면 보여주지 않게 변경
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
	//		// 종료 타이머를 설치하지 않은 경우는 FinishShow 에서 파괴
	//		GetWorldTimerManager().SetTimer(FinishEventWidgetTimerHandle, this, &AB2StageEventDirector::FinishEventWidgetTimerCB,
	//			EventWidgetEndTime - FMath::Max(EventWidgetStartTime, 0.0f), false);
	//	}
	//	else
	//	{
	//		// 이 경우는 바로 제거해야 말이 될 듯 ㅡ.ㅡ;
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
	//		//Fight PSC로 인해서 여기서 닫지 않는다.
	//		//UB2UIManager::GetInstance()->CloseUI(UIFName::PVPBegin); 
	//		break;
	//		// StageClear 만 좀 특별하게 취급해서 UIScene 으로 간다. 실질적으로 StageClear 라면 맵이 다시 로딩된다거나 할 것이므로 여기로 오지는 않을 듯?
	//		// 요것도 UIScene 이라 None 으로..
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
	// BeginSingleShow 에서 사용하는 상황과 일치해야겠다..
	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera || OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
	{		
		if (
			InSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_BringPlayActor ||
			// ManualCamera 인 경우 NoAdjust 만 아니면 직접 세팅.
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
	//	// ExportedEventScene 세팅을 까먹었든지, 아니면 그냥 카메라워크만 추출해서 사용하기 위해 의도한 걸 수도 있음.
	//	return true;
	//}

	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera && InSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_NoAdjust)
	//{
	//	// 간단한 인게임 상황 카메라에 이런 설정을 사용해 볼 수도 있겠다.. 몹 등작 직후면 거의 위치가 고정되어 있기도 할테고..
	//	return true;
	//}

	return false;
}

void AB2StageEventDirector::NotifyStageEventShowOver(class ABladeIICharacter* PlayActorJustOver)
{
	// SESEndNotify 가 사용되는 상황에서만 의미가 있음. Controlled Matinee 모드에서는 아마 불리지 않을 것. (대신 NotifyActorCustomEvent)

	if (bStandbyAtTheEnd && PlayActorJustOver)
	{
		// 이 경우 어차피 끝까지 대기하고 있어야 하니 쓸데없이 AnimBP 에서 카메라 Anim 중단 같은 거 하지 않도록 Extra 로 SES State 를 유지한다.
		// ControlledMatinee 의 경우 물론 이게 먹히지 않을 거라 그냥 멈춰 버리는데.. 뭐 방법 있으려나..
		PlayActorJustOver->SetStageEventShowState(EStageEventShowState::ESES_Extra, this);
	}
	else
	{
		for (int32 SI = 0; SI < ShowStates.Num(); ++SI)
		{
			if (ShowStates[SI].GetPlayActor(this) == PlayActorJustOver && ShowStates[SI].bShowFinished == false)
			{
				OnSingleStageEventShowOver(SI);
				// 일치하는 게 하나만 있어야 정상이겠지만 혹 모르니 break 없이 다 돌고 본다.
			}
		}
	}
}

static bool CheckAndGetMatineeScenarioDialogEventName(FString& InOutEventString)
{ // AMatineeActor::NotifyEventTriggered 에서 보낸 ScenarioDialog 이벤트 규약에 맞는지 체크하는 것. 
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
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee) // 이게 아닌 경우라면.. 뭔가 잘못된 것일 듯
	//	{
	//		OnControlledMatineeStop(false);
	//	}
	//}
	//else if (OptionalEventName == MATINEE_DONE_STAY_ACTOR_CUSTOM_EVENT)
	//{
	//	// 방금 장면 종료했으나 이 상태로 stay.
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && bStandbyAtTheEnd)
	//	{
	//		OnControlledMatineeStop(true);
	//	}
	//}
	//else if (OptionalEventName == CAMERAANIM_STOP_ACTOR_CUSTOM_EVENT)
	//{
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera) // 마찬가지로 이게 아니라면 뭔가 잘못..
	//	{
	//		OnExportedCameraAnimStop();
	//	}
	//}
	//else if (OptionalEventName == CAMERAANIM_DONE_STAY_ACTOR_CUSTOM_EVENT)
	//{
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera && bStandbyAtTheEnd)
	//	{
	//		// 일단 Stop notification 을 보낸다. CheckShowEndCountDown 에서 bStandbyAtTheEnd 에 대한 추가 체크를 할 것.
	//		OnExportedCameraAnimStop();
	//	}
	//}
	//else
	//{	
	//	
	//	FString EventNameString = OptionalEventName.ToString();
	//	if (CheckAndGetMatineeScenarioDialogEventName(EventNameString))
	//	{ // EventNameString 에서 특정 prefix 가 빠져 있을 것.
	//		OpenDialogClass<FName, bool>::GetInstance().Signal(FName(*EventNameString), false); // CinematicMode 에 따른 BattleUI 숨기기가 작동할 것이므로 여기선 MaintainBattleUI 를 신경쓸 없다.
	//	}
	//}
}

void AB2StageEventDirector::NotifyShowOverFromDependentDirectorActor()
{
	if (HasValidDependentDirectorActorSetting()) // 이게 세팅이 안되어 있으면 불리지 않아야 함.
	{
		if (bDependentDirectorActorMet == false) // 만에 하나 중복 호출 방지..?
		{
			//////////
			bDependentDirectorActorMet = true; // 다른 곳에서는 bDirectingEventConditionMet 을 세팅하는데 여기만 이걸 세팅한다.
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
	// 여기로 왔다면 bStandbyAtTheEnd 사용하는 상황에서 실질적인 장면들이 방금 끝난 상태
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
	//	//BII_CHECK(0); //TMGameMode에서 LocalPlayerCharacter는 없음. SpectatorPawn임.
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
	//{ // TMGameMode 에선 없을 수 있다.
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
	// bDisableInput 이 아니면 뭔가 플레이의 연속일 가능성이 높으므로 Auto 를 유지. 
	// 조건이 좀 더 refine 될 수 있다. 다른 조건 검사 내지는 특정 속성값에 따라서 등.
	return bDisableInput;
}

bool AB2StageEventDirector::ShouldApplyCinematicScalabilityDuringShow() const
{ // 여기서 false 를 리턴하는 상황은 사실상 게임 플레이의 연속으로 몹이나 NPC 의 특정 애니메이션을 플레이하기 위함일 것.
	return (bDisableInput || bHideHud);
}

bool AB2StageEventDirector::ShouldBeInvincibleDuringShow() const
{
	return bDisableInput; // 여기도 마찬가지로 입력 가능한 이벤트 씬이라면 플레이의 연속일 것이므로 무적 상태로 들어가지 않게..
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

	//if (!InOutDefaultSKActorToSetup) { // DefaultSKActorToSetup 은 GameMode 쪽에서 생성해서 준다. 
	//	return;
	//}
	//if (OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee || !MatineeActor || !MatineeActor->MatineeData) {
	//	return;
	//}

	//// 여기서는 기타 사용하는 ParticleSystemEffect 들을 긁어서 컴포넌트로 생성. 넘어온 DefaultSKActorToSetup 에 붙여 줌.
	//TArray<AActor*> AllGroupActors;
	//MatineeActor->GetControlledActors(AllGroupActors);
	//TMap<FName, UParticleSystem*> AllFoundPSList; // 똑같은 리소스 중복 생성 안하게.. 여기에 긁어모음.
	//for (int32 AI = 0; AI < AllGroupActors.Num(); ++AI)
	//{
	//	// 보통은 레벨에 설치해 둔 Emitter 액터를 GroupActor 로 해서 On/Off 해 가며 사용함.
	//	
	//	AEmitter* CastedEmitterActor = Cast<AEmitter>(AllGroupActors[AI]); 
	//	UParticleSystem* EmitterFxResouce = (CastedEmitterActor && CastedEmitterActor->GetParticleSystemComponent()) ? CastedEmitterActor->GetParticleSystemComponent()->Template : NULL;

	//	// EmitterActor 의 visibility 를 제어하려면 꼬이기도 쉽고 (여기서 켰다가 바로 꺼주는 것도 아니고 몇 프레임 이후에 꺼주는 건데 그러다 보면 그 새 시작된 연출에서 실제로 켜질 수도 있고.. 골치 아픔)
	//	// 걍 똑같은 ParticleSystem 리소스로다 컴포넌트 생성해 줌.
	//	if (EmitterFxResouce)
	//	{
	//		FName ThisPSKey(*EmitterFxResouce->GetPathName());
	//		UParticleSystem** FoundOne = AllFoundPSList.Find(ThisPSKey);
	//		if (!FoundOne){
	//			AllFoundPSList.Add(ThisPSKey, EmitterFxResouce);
	//		}
	//	}
	//}

	//// AnimControl Track 으로 들어간 애니메이션들의 AnimNotify 들도..
	//for (UInterpGroup* ThisGroup : MatineeActor->MatineeData->InterpGroups)
	//{ // Group > Track > AnimSeq > Notify 마구마구 들어가는군 ㅠ.ㅠ
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
	//			// 마지막 for 문 ㄷㄷ
	//			for (FAnimNotifyEvent& NotifyEvt : ThisAnimSeq->Notifies)
	//			{
	//				UParticleSystem* FoundPS = CheckNotifyEventForParticleSystemUse(NotifyEvt);
	//				if (FoundPS)
	//				{ // 새로운 거면 추가
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

	//// 긁어모은 걸로 pre-render 용 컴포넌트 생성
	//for (TMap<FName, UParticleSystem*>::TIterator FoundPSIt(AllFoundPSList); FoundPSIt; ++FoundPSIt)
	//{
	//	UParticleSystem* ThisPS = FoundPSIt.Value();
	//	UParticleSystemComponent* SpawnedFxComp = UB2PreRenderer::SpawnDummyPSCForPreRender(ThisPS, InOutDefaultSKActorToSetup->GetRootComponent());
	//}
	//
	//// SkeletalMeshActor 들은 ShowSettings 에 연결되지 않은 애만 따로 처리해 줌. ShowSetting 에 연결된 애들은 BladeIIGameMode 쪽에서 PC, NPC 정보 긁어오면서 Prerender 셋업할 때 처리될 것이기 때문.
	//TArray<AActor*> NotBoundActors;
	//GetMatineeGroupActorsNotBoundtoShowSetting(NotBoundActors);
	//UWorld* TheWorld = GetWorld();
	//for (AActor* ThisNotBound : NotBoundActors)
	//{
	//	ASkeletalMeshActor* CastedNotBoundSKActor = Cast<ASkeletalMeshActor>(ThisNotBound);
	//	if (CastedNotBoundSKActor && CastedNotBoundSKActor->GetSkeletalMeshComponent())
	//	{
	//		// 여기에 설치된 SkeletalMeshActor 를 쓰는 게 아니라 새로 생성한다. Visibility 를 별도로 제어해야 하므로..
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
	// Pre-render 가 돌아가는 와중에 월드 타임 스케일(slomoe)이 적용이 된 채로 화면이 까맣게 가려지는데
	// 사운드 트랙 같은 경우는 slomo 의 영향을 받지 않는데다가 (받아도 이상할 듯) 아싸리 0 초부터 시작이 되면 화면은 가려진 상태에서 사운드만 먼저 나와 버리는 비극이 발생하게 된다.
	// 이를 막기 위함
	// UB2UITalkDialog 에도 마찬가지 목적의 처리를 하였음

	//if (!MatineeActor || !MatineeActor->MatineeData) {
	//	return;
	//}

	//for (UInterpGroup* ThisInterpGroup : MatineeActor->MatineeData->InterpGroups)
	//{
	//	if (ThisInterpGroup)
	//	{
	//		for (UInterpTrack* ThisTrack : ThisInterpGroup->InterpTracks)
	//		{
	//			// 일단 대상은 sound track
	//			UInterpTrackSound* CastedSoundTrack = Cast<UInterpTrackSound>(ThisTrack);
	//			if (CastedSoundTrack && !CastedSoundTrack->IsDisabled()) // 다른 이유로 disable 된 것들이 있을 테니 그건 거르고
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

	//ForceSetMatineePlayingPosScale(0.0f); // 첨부터 다시 시작.
}
void AB2StageEventDirector::ForceSetMatineePlayingPosScale(float InPositionScale)
{
	//// Matinee 를 사용하는 경우 특정 위치 (0.0 ~ 1.0 스케일로) 로 강제 이동하는데..
	//// 다소간 특수 목적으로 만들었음을 명시..
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

	//TMap<FName, FName> AllPossibleGroupNameMap; // 걍 검색을 좀 해야 해서 TMap 으로 구성함.
	//for (FPlayActorShowSettings& ThisShowSetting : ShowSettings)
	//{
	//	TArray<FName> AllSupposedNameThis;
	//	// PC 의 경우 종류별로 해서 하나의 ShowSetting 이 여러 Matinee Group 에 연결되어 있을 수 있으니 가능한 것들을 모두 검사.
	//	GetAllSupposedInterpGroupNamesForSetting(AllSupposedNameThis, ThisShowSetting);
	//	for (FName& ThisFoundName : AllSupposedNameThis)
	//	{
	//		AllPossibleGroupNameMap.Add(ThisFoundName, ThisFoundName);
	//	}
	//}

	//TArray<UInterpGroup*> AllFoundGroupNotBoundShowSetting; // 어떤 ShowSetting 과도 연결되지 않은 InterpGroup 목록
	//for (int32 GAI = 0; GAI < MatineeActor->MatineeData->InterpGroups.Num(); ++GAI)
	//{
	//	UInterpGroup* CurrGroup = MatineeActor->MatineeData->InterpGroups[GAI];
	//	if (!CurrGroup || AllPossibleGroupNameMap.Find(CurrGroup->GroupName)){
	//		continue; // 특정 ShowSettings 과 연결된 그룹은 스킵
	//	}
	//	// 여기까지 통과한 InterpGroup에서 GroupActor 들을 뽑아온다. GroupActorInfos 를 통해 그룹에 연결된 액터들을 찾아올 수 있음
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
	// Matinee Event 트랙에 ScenarioDialogEvent 로 등록된 이벤트 이름들을 긁어오는 것.
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
	//					// NotifyActorCustomEvent 를 통해 올 때에는 ScenarioDialogEvent 용도인지 구분하기 위한 prefix 가 붙게 되지만 
	//					// 그냥 이 상태에서는 EventName 이 바로 DialogCode 가 됨.
	//					
	//					// 사용하는 상황에서 다른 데에서 긁어온 것들을 한데 합칠 수 있으므로 AddUnique 함.
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
	//// 에디터 뷰포트에 작게 나오는 프리뷰 창에 선택한 카메라 컴포넌트 뷰를 보이기 위함. 기본 기능으로는 그냥 첫번째 것만 나옴.
	//if(GIsEditor && ComponentClass == UCameraComponent::StaticClass())
	//{
	//	TArray<UCameraComponent*> SelectedCameraComps;
	//	UCameraComponent* SelectedCameraComponent = NULL;

	//	// 선택된 카메라 컴포넌트가 있는지 검사
	//	USelection* ComponentSelection = GEditor->GetSelectedComponents();
	//	if(ComponentSelection)
	//	{
	//		ComponentSelection->GetSelectedObjects(SelectedCameraComps);
	//	}
	//	if(SelectedCameraComps.Num() > 0)
	//	{
	//		SelectedCameraComponent = SelectedCameraComps[0];
	//	}

	//	// 원래 기능에서는 그냥 처음 것을 리턴하는데 선택한 것과 같은 것인지 추가 검사
	//	UActorComponent* FoundComponent = NULL;
	//	for (UActorComponent* Component : GetComponents())
	//	{
	//		if (Component && Component->IsA(ComponentClass) && Component == SelectedCameraComponent)
	//		{
	//			FoundComponent = Component;
	//			break;
	//		}
	//	}

	//	if(FoundComponent == NULL) // 폴백
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
	// 여타 다른 NetGameMode 에서의 Opponent 가 추가될 수 있음.
	//AB2PVPGameMode* PVPGM = Cast<AB2PVPGameMode>(GetB2GameMode());
	//return PVPGM ? PVPGM->GetOpponent() : NULL;
	return NULL;
}

ABladeIIPlayer* AB2StageEventDirector::GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly, bool bSpawnPuppetForDead)
{
	if (AB2AsyncGameMode* AsyncGM = Cast<AB2AsyncGameMode>(GetB2GameMode()))
		return AsyncGM->GetTeamPlayer(bAllyTeam, nPlayerIndex, bLiveOnly, bSpawnPuppetForDead); // bSpawnPuppetForDead 를 true 로 넘겨주면 죽은 애도 연출 용으로 데려올 수 있다.

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
	//// 현재 설정에 따라 카메라 하이라이트를 받을 만한 플레이어 캐릭터를 리턴하는 것임. 물론 그게 반드시 보장되는 건 아니다.
	//if (DirectingEvent == EStageEvent::ESEV_MatchResult)
	//{
	//	if (AB2TMGameMode* TMGM = Cast<AB2TMGameMode>(GetB2GameMode()))
	//		return TMGM->GetTeamPlayer(bMatchResultEventAsWinner, 0, false, true); // 여기서도 dead 인 캐릭터를 dummy spawn 해야 하나..
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

	return GetB2LocalPlayerChar(); // 음.. 결국은 MobAppear 이벤트에서 필요하게 되었군.
}

#if WITH_EDITOR

void AB2StageEventDirector::RefreshPreviewSKComps(bool bRegenerateSKComp /*= true*/)
{
	//// 기존에 만든 것들 대부분을 제거하고 새로 생성.
	//DestroyPreviewComponents(bRegenerateSKComp);

	//UPackage* OutermostPkg = GetOutermost();
	//// 맵인 경우만 생성해야 함. 블루프린트로 저장한 애도 처음엔 맵으로 인식될 것.
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
	//	ActorNameDisplayText->SetRelativeLocation(FVector(0.0f,0.0f,100.0f)); // 좀 높여서
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
	//	NewInfoTextComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // 위치는 적당히 맞춰준다
	//	NewInfoTextComp->SetText(FText::FromString(CurrSC->GetName()));
	//	NewInfoTextComp->SetTextRenderColor(FColor(255, 0, 0));

	//	AnyShapeCompNameText.Add(NewInfoTextComp);
	//}

	//if(OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//{
	//	// Controlled Matinee 에선 Preview 용 SkeletalMeshComponent 들을 생성하는 대신 대체로 이미 배치된 SkeletalMeshActor 들과 관련한 추가적인 처리..
	//	SetupPreviewForControlledMatinee();
	//	return;
	//}

	//// 이하는 Controlled Matinee 모드에서는 필요없는 것들만 넣는다.

	//for(int32 SI = 0; SI < ShowSettings.Num(); ++SI)
	//{
	//	FPlayActorShowSettings& ThisSetting = ShowSettings[SI];
	//	FTransform PreviewCompTransform;
	//	USkeletalMesh* PreviewSKMesh = NULL;
	//	if(GetEssentialPreviewInfoForSetting(ThisSetting, NULL, PreviewSKMesh, PreviewCompTransform))
	//	{
	//		// 성공했으면 PreviewSKMesh 와 PreviewCompTransform 이 세팅되었을 것.

	//		// bRegenerateSKComp 가 아니라도 기존에 없는 상황이라면 새로 생성한다.
	//		USkeletalMeshComponent* NewSKComp = (bRegenerateSKComp == false && PreviewSKComps.Num() > SI) ?
	//			PreviewSKComps[SI] : NewObject<USkeletalMeshComponent>(this, *GetPreviewSKCompNameForSetting(SI), RF_Transient);

	//		if(NewSKComp)
	//		{
	//			NewSKComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//			NewSKComp->RegisterComponent();
	//			NewSKComp->SetVisibility(true);
	//			// 어차피 게임에선 생기지도 않을 애들이지만 
	//			NewSKComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//			NewSKComp->SetHiddenInGame(true);
	//				
	//			NewSKComp->SetSkeletalMesh(PreviewSKMesh);
	//			const float ZOffsetForCharacter = NewSKComp->Bounds.BoxExtent.Z;
	//			// SetWorldTransform 을 하기 전에 SkeletalMesh 와 BladeIICharacter 간 pivot 차이로 인한 offset 을 반영한다. SyncFromPreviewSKCompPlacement 에서 하는 것의 반대
	//			if(DoesThisSettingRequireManualPlacement(ThisSetting))
	//			{						
	//				PreviewCompTransform.SetTranslation(PreviewCompTransform.GetTranslation() + FVector(0.0f, 0.0f, -1.0f * ZOffsetForCharacter));
	//			}
	//			NewSKComp->SetWorldTransform(PreviewCompTransform);
	//			// 런타임 PlayActor 도 Snapping 을 하니 얘도 snapping. 근데 PostLoad 시점에서는 아직 로딩되지 않은 월드 오브젝트가 있어서 그런지 안먹힘.
	//			SnapSkeletalMeshComponentToGround(NewSKComp);

	//			if(DoesThisSettingRequireManualPlacement(ThisSetting))
	//			{
	//				const float FirstSnapZTranslation = NewSKComp->GetComponentLocation().Z - PreviewCompTransform.GetTranslation().Z;

	//				if(FMath::Abs(FirstSnapZTranslation) < KINDA_SMALL_NUMBER)
	//				{
	//					// 여기로 왔다면 바로 앞의 SnapSkeletalMeshComponentToGround 가 안먹혔단 얘긴데
	//					// 앞서 PreviewCompTransform 에 ZOffsetForCharacter 를 - 로 가했으므로 기본 위치가 낮으면 땅에 파묻혔을 가능성이 꽤 있다.
	//					// 인게임에서는 어느 정도까지 파묻힌 건 충돌 처리로 인해 위로 올라오게 되지만 에디터 작업에 혼선을 줄 수도 있으니 위로 약간 올려서 다시 snap 을 시도.
	//					// 다만, 이 때에 snap 전후 변화가 없다면 여전히 땅에 묻혀있든지 하는 경우이므로 이전 값으로 다시 돌려놓음.

	//					const FVector LocationBeforeAddtionalSnap = NewSKComp->GetComponentLocation(); // 나중에 되돌릴 때 사용을 위해 백업

	//					PreviewCompTransform.SetTranslation(PreviewCompTransform.GetTranslation() + FVector(0.0f, 0.0f, 1.0f * ZOffsetForCharacter));
	//					const FVector RaisedLocationBeforeSnap = PreviewCompTransform.GetTranslation();

	//					NewSKComp->SetWorldLocation(PreviewCompTransform.GetTranslation());
	//					SnapSkeletalMeshComponentToGround(NewSKComp); // 새로 snap 된 location 에 뭔가 변화가 있다면 여기서의 위치를 유지하게 될 것.

	//					const FVector RaisedAndSnappedLocation = NewSKComp->GetComponentLocation();

	//					if(FMath::Abs(RaisedAndSnappedLocation.Z - RaisedLocationBeforeSnap.Z) < KINDA_SMALL_NUMBER)
	//					{
	//						// 새로 snap 을 해도 신통치 않으니 다시 되돌린다.
	//						NewSKComp->SetWorldLocation(LocationBeforeAddtionalSnap);
	//					}
	//				}
	//			}

	//			if(bRegenerateSKComp == true || PreviewSKComps.Num() <= SI) // 위에서 컴포넌트를 새로 생성하는 조건.
	//			{
	//				PreviewSKComps.Add(NewSKComp);
	//			}

	//			// 프리뷰 comp 에서 위치를 역으로 sync 하기 위한 맵핑 info.
	//			FSEDPreviewCompInfo NewMappingInfo;
	//			NewMappingInfo.SKComp = NewSKComp;
	//			NewMappingInfo.ShowSettingIndex = SI;
	//			PreviewMapping.Add(NewMappingInfo);
	//				
	//			// 안내용 텍스트. 역시나 B2SpawnPoolEditingActor 표절
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

	//				// SpawnPoolEditingActor 쪽에선 크기 유지하려고 부모 MeshComp 따라 새로 계산해서 RelativeScale 로 먹였는데 그냥 WorldScale 로 먹이면 되는 거 아닌가? ㄷㄷ
	//				NewInfoTextComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	//				// Most SkeletalMesh assets have their root at their feet, so multiply the box extent by 2.
	//				// Relative location takes a value before scaling, so give InvMeshScale.
	//				const float TextRenderZOffset = (NewSKComp->Bounds.BoxExtent.Z * 2.0f); // 여기에 NewSKComp 쪽 scale 까지 먹일 필요가 있으려나..
	//				FVector TextRelativeLocation = NewInfoTextComp->GetRelativeTransform().GetLocation();
	//				TextRelativeLocation.Z = TextRenderZOffset;
	//				NewInfoTextComp->SetRelativeLocation( TextRelativeLocation );
	//					
	//				FColor InfoTextRenderColor = FColor(0,255,255);

	//				// 마지막으로 안내문구.
	//				// Directing event 에 따른 안내 문자
	//				FString DirectingEventString;
	//				switch (DirectingEvent) // 여기서 굳이 GetTrimmedNameFromPCClassEnum 같은 짓을 할 필요까진 없겠지.
	//				{
	//				case EStageEvent::ESEV_Begin: DirectingEventString = TEXT("Begin"); break;
	//				case EStageEvent::ESEV_MobAppear: DirectingEventString = TEXT("MobAppear"); break;
	//				case EStageEvent::ESEV_MobDying: DirectingEventString = TEXT("MobDying"); break;
	//				case EStageEvent::ESEV_Clear: DirectingEventString = TEXT("Clear"); break;
	//				case EStageEvent::ESEV_Custom: DirectingEventString = TEXT("Custom"); break;
	//				}

	//				// PlayActor 에 따른 안내문자
	//				FString PlayActorString;
	//				switch (ThisSetting.PlayActorType)
	//				{
	//				case EStageEventPlayActor::ESEE_CurrentPlayer: PlayActorString = TEXT("CurrentPlayer"); break;
	//				case EStageEventPlayActor::ESEE_OtherPlayer: PlayActorString = TEXT("OtherPlayer"); break;
	//				case EStageEventPlayActor::ESEE_MainPlayer: PlayActorString = TEXT("MainPlayer"); break;
	//				case EStageEventPlayActor::ESEE_SubPlayer: PlayActorString = TEXT("SubPlayer"); break;
	//				case EStageEventPlayActor::ESEE_WaveMob: {
	//					// 여기까지 온 거면 유효한 설정일 것.
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
	//					InfoTextRenderColor = FColor(127,255,127); // B채널 대신 R채널이 들어갔으니 살짝 더 밝기는 할 듯?
	//				}
	//				else if(DoesThisSettingRequireManualPlacement(ThisSetting) == false)
	//				{
	//					InfoTextString += TEXT("\nPreview only");
	//					InfoTextRenderColor = FColor(0,127,127); // 보다 어둡게.
	//				}						

	//				NewInfoTextComp->SetText(FText::FromString(InfoTextString));
	//				NewInfoTextComp->SetTextRenderColor(InfoTextRenderColor);
	//				PreviewSKCompsInfoText.Add(NewInfoTextComp);
	//			}
	//		}
	//	}
	//}

	//// Director 카메라 상단의 번호도 뿌려보자.
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
	//		NewInfoTextComp->SetRelativeLocation(FVector(0.0f,-10.0f,20.0f)); // 위치는 적당히 맞춰준다
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
	//	// ThisSetting 과 연결된 그룹액터들 모두 모음. PC 가 아니라면 웬만하면 하나일 것.
	//	EditorGetMatineeGroupActorsFromShowSetting(ThisSetting, GroupActorsForThisSetting);		

	//	for (int32 GAI = 0; GAI < GroupActorsForThisSetting.Num(); ++GAI)
	//	{
	//		ASkeletalMeshActor* CurrGroupSKActor = Cast<ASkeletalMeshActor>(GroupActorsForThisSetting[GAI]);
	//		if (CurrGroupSKActor == NULL)
	//		{
	//			continue;
	//		}

	//		// 이제 준비가 되었으니 ThisSetting 에 해당하는 적절한 스케일을 찾아서 CurrGroupSKActor 에 설정.

	//		FTransform PreviewCompTransform;
	//		USkeletalMesh* PreviewSKMesh = NULL;
	//		// 두번째 인자로 그룹 SkeletalMesh 를 넘겨서 PC 의 경우 ThisSetting 만 가지고는 알 수 없는 특정 클래스에 대한 정보를 얻어오도록 함.
	//		if (GetEssentialPreviewInfoForSetting(ThisSetting, CurrGroupSKActor->GetSkeletalMeshComponent()->SkeletalMesh, PreviewSKMesh, PreviewCompTransform) == false)
	//		{
	//			continue;
	//		}
	//		// 일단 여기서는 Scale 만 세팅을.. 사실 알고보면 ABladeIICharacter::SetupControlledMatineePuppet 에서 하는 것과 마찬가지..
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
	//// PC 의 경우 종류별로 해서 하나의 ShowSetting 이 여러 Matinee Group 에 연결되어 있을 수 있으니 가능한 것들을 모두 검사.
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
	//		continue;; // InSetting 과 연결된 Group 은 아님.
	//	}

	//	TArray<class AActor*> CurrGroupActors;
	//	// 에디터에서는 GroupActorInfos 를 통해 그룹에 연결된 액터들을 찾아올 수 있음. -> 아니 에디터 말고 그냥 게임에서도 되는 거 아닌가..?
	//	for (int32 GAI2 = 0; GAI2 < MatineeActor->GroupActorInfos.Num(); ++GAI2)
	//	{
	//		if (MatineeActor->GroupActorInfos[GAI2].ObjectName == FName(*CurrGroup->GetName()))
	//		{
	//			// OutGroupActors 에 더한다. 하나의 그룹에 여러 Actor 가 등록된 상황이 얼마나 있나?
	//			OutGroupActors += MatineeActor->GroupActorInfos[GAI2].Actors;
	//			break;
	//		}
	//	}
	//}
}

void AB2StageEventDirector::SyncFromPreviewSKCompPlacement()
{
	// PreviewMapping 을 통해 각 preview component 별로 ShowSetting 을 찾아서 Placement 등의 설정에 따라 필요하면 값을 세팅한다.

	for(int32 PCI = 0; PCI < PreviewSKComps.Num(); ++PCI)
	{
		USkeletalMeshComponent* CurrSKComp = PreviewSKComps[PCI];
		int32 ShowSettingIndex = -1;

		for(int32 PMI = 0; PMI < PreviewMapping.Num(); ++PMI)
		{
			if(PreviewMapping[PMI].SKComp == CurrSKComp) // 아마 PCI 랑 PMI 가 일치할 것
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
				// 프리뷰는 이게 아닌 상황에서도 나올 수 있지만 Sync 는 이 경우만.

				// 여기서 빼낸 값은 BladeIICharacter 용이 될 것이므로 Z offset 을 좀 감안해야겠다. 이래저래 좀 미스매치가 있겠는데..
				float ZOffsetForCharacter = CurrSKComp->Bounds.BoxExtent.Z;

				// 지금까지의 사용에서 ManualRelativeLocation/Rotation 은 이 액터의 루트 기준이므로 걍 단순히 GetRelativeTransform 하면 되겠다.
				// 아닌 경우가 생기면, GetComponentTransform 했다가 필요한 기준 transform 을 빼도록.
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
	if( IsPlayActorTypeForPlayerCharacter(InType) )
	{
		UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox();
		if(PCClassInfoBox)
		{
			// 클래스를 정해놓고 가져옴. 물론 WorldSetting 에 설정한 GameMode 를 볼 수도 있겠지만 굳이 그럴 필요가..
			// Main 이나 Current 면 Gladiator 로, 아니면 Assassin
			EPCClass PCClassType = EPCClass::EPC_End;
			switch (InType)
			{
				case EStageEventPlayActor::ESEE_MainPlayer :
				case EStageEventPlayActor::ESEE_CurrentPlayer :
				case EStageEventPlayActor::ESEE_WinningPlayer_Current :
				case EStageEventPlayActor::ESEE_TeamAllyPlayer :
				case EStageEventPlayActor::ESEE_TeamWinningPlayer :
				case EStageEventPlayActor::ESEE_RaidPlayer:
				case EStageEventPlayActor::ESEE_WinningRaidPlayer:
				case EStageEventPlayActor::ESEE_DefeatedRaidPlayer:
					PCClassType = EPCClass::EPC_Gladiator;
					break;
				case EStageEventPlayActor::ESEE_SubPlayer :
				case EStageEventPlayActor::ESEE_OtherPlayer :
				case EStageEventPlayActor::ESEE_WinningPlayer_Other:
				case EStageEventPlayActor::ESEE_TeamEnemyPlayer:
					PCClassType = EPCClass::EPC_Assassin;
					break;
				case EStageEventPlayActor::ESEE_RemoteOpponent_Current:
					PCClassType = EPCClass::EPC_Fighter;
					break;
				default :
					PCClassType = EPCClass::EPC_Wizard;
					break;
			}

			if (PCClassType != EPCClass::EPC_End)
				return PCClassInfoBox->GetSingleClassInfo(PCClassType);
		}
	}
	return NULL;
}

bool AB2StageEventDirector::GetEssentialPreviewInfoForSetting(FPlayActorShowSettings& InSetting, class USkeletalMesh* OptionalSpecificPCClassMesh, class USkeletalMesh*& OutSKMesh, FTransform& OutTransform)
{
	//bool bGotTransform = false;

	//if(DoesThisSettingHaveNoFixedPlacement(InSetting))
	//{
	//	// 인게임 위치를 사용하는 경우 등 정해진 위치가 없다면 프리뷰 상으로는 적당한 위치에 놓는다. Default 로 세팅한 카메라와 마주보게?
	//	OutTransform = FTransform(FRotator(0.0f,-90.0f,0.0f), FVector(0.0f,100.0f,0.0f)) * this->GetTransform();
	//	bGotTransform = true;
	//}
	//else if (DoesThisSettingRequireManualPlacement(InSetting)) // 일반적인 수동 설정 상황
	//{
	//	const FTransform RelativeTransform(InSetting.ManualRelativeRotation, InSetting.ManualRelativeLocation);
	//	OutTransform = RelativeTransform * this->GetTransform();
	//	bGotTransform = true; // 일단 성공
	//}
	//else if(OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera)
	//{
	//	// SimpleEventScene 을 사용하는 경우가 되겠다. 역시나 AdjustToPlayActor 는 무시하고 CameraAnimApplyCamIndex 로 선택한 카메라에 맞춰서 ExportedEventScene 의 첫 키값으로 위치 산정.
	//	// 여긴 그냥 프리뷰로만.
	//	if (IsExportedEventSceneValid(InSetting.ExportedEventScene) && CameraAnimApplyCamIndex >= 0 && CameraAnimApplyCamIndex < MAX_DIRECTOR_CAMERA)
	//	{
	//		FSimpleEventSceneMoveKeyData& FirstKeyData = InSetting.ExportedEventScene->SimpleMoveKeyFrames[0];

	//		// SetSESPlayActorMovementFromCamera 참고. 심플하게 필요한 핵심만.
	//		UCameraComponent* BaseCamComp = DirectorCamArrayRef[CameraAnimApplyCamIndex];
	//		OutTransform = FTransform(FirstKeyData.RotKey, FirstKeyData.PosKey) * BaseCamComp->GetComponentTransform();
	//		bGotTransform = true;
	//	}
	//}
	//else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//{
	//	// ControlledMatinee 에서 SkeletalMeshActor 스케일 맞춰보기 위해 통과시킴. 여기서도 물론 필요하다면 해당 그룹액터 찾아서 배치된 Transform 을 넣는다거나 해볼 수 있겠다.
	//	OutTransform = FTransform();
	//	bGotTransform = true;
	//}

	//if (bGotTransform == false)
	//{
	//	return false;
	//}

	//// 표시할 transform 이 생겼으니 SkeletalMesh 모델 뽑아오기.	
	//if( IsPlayActorTypeForPlayerCharacter(InSetting.PlayActorType) )
	//{
	//	UB2PCClassInfo* PCClassInfo = GetPCClassInfoForPlayActorType(InSetting.PlayActorType); // 여기서는 아예 정해진 타입의 PCClass 를 가져올 것. 어차피 에디터라 어떤 클래스가 나올지 모르는 일이므로.

	//	if (OptionalSpecificPCClassMesh)
	//	{
	//		// 이 경우 Player Character 이면서 메쉬를 지정하였으니 그 Mesh 를 사용하는 PCClassInfo 에서 정보를 가져옴.
	//		// 이건 사실상 클래스별로 트랙을 만들어야 하는 Controlled Matinee 용임.
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
	//	// 여기까지 찾은 PCClassInfo 에 세팅한 걸로..
	//	if(PCClassInfo)
	//	{
	//		OutSKMesh = PCClassInfo->BaseMeshAsset;

	//		// PCClassInfo 를 기반으로 스케일도 맞춰 보자.
	//		if(PCClassInfo->GetBaseBPClass(this))
	//		{
	//			ABladeIIPlayer* DefaultPlayerBPObject = Cast<ABladeIIPlayer>(PCClassInfo->GetBaseBPClass(this)->GetDefaultObject());
	//			if(DefaultPlayerBPObject && DefaultPlayerBPObject->GetMesh())
	//			{
	//				// 여기선 GetMesh()->GetComponentScale 로는 예상하는 값이 안나옴. RootComponent 부터 해서 relative scale 을 가져다 곱한다.
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
	//	if (RelevantEditorSpawnPool && IsStageSupported(EditorStageNum, EditorStageDifficulty)) // WorldSettings 에 설정한 EditorStageNum 이 지원되는 설정에서만.
	//	{
	//		// NPCClass enum 혹은 ABladeIICharacter 블루프린트 클래스 둘 중 하나.
	//		ENPCClass NPCClassEnum = ENPCClass::ENC_End;
	//		ENPCClassVariation NPCClassVariation = ENPCClassVariation::ENCV_Normal;
	//		TSubclassOf<ABladeIICharacter> NPCClassBP = NULL;
	//		RelevantEditorSpawnPool->EditorGetSpawnClassOfWaveMob(NPCClassEnum, NPCClassVariation, NPCClassBP, WaveNum, MobIndex);

	//		// AB2MonsterSpawnPool::SpawnWave 와 마찬가지 우선순위로..
	//		if(NPCClassEnum != ENPCClass::ENC_End)
	//		{
	//			UB2NPCClassInfoBox* MobClassInfoBox = StaticFindMobClassInfoBox();
	//			if(MobClassInfoBox)
	//			{
	//				UB2NPCClassInfoBox::NPCClassPreviewAssetInfo PreviewAssetInfo = MobClassInfoBox->GetPreviewAssetInfoFromClass(NPCClassEnum, NPCClassVariation);
	//				// PreviewAssetInfo 에 MaterialOverride 도 나오긴 하는데 여기에도 반영을 할까 말까..
	//				OutSKMesh = PreviewAssetInfo.SKMesh;

	//				// 여기도 NPCClassInfo 를 기반으로 스케일 맞추기.
	//				UB2NPCSingleClassInfo* SingleMobInfo = MobClassInfoBox->GetNPCSingleClassInfo(NPCClassEnum, NPCClassVariation);
	//				if(SingleMobInfo && SingleMobInfo->GetBaseBPClass(this))
	//				{
	//					ABladeIICharacter* DefaultBPObject = Cast<ABladeIICharacter>(SingleMobInfo->GetBaseBPClass(this)->GetDefaultObject());
	//					if(DefaultBPObject && DefaultBPObject->GetMesh())
	//					{
	//						// 여기선 GetMesh()->GetComponentScale 로는 예상하는 값이 안나옴. RootComponent 부터 해서 relative scale 을 가져다 곱한다.
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
	if(bDestroyAll) // 이게 false 인 것은 단순 위치 이동 시 refresh 에 사용하기 위함.
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

	//// 마티니 그룹 액터로 등록된 ASkeletalMeshActor 들은 ShowSettings 등록 여부에 상관없이 collision 무조건 꺼 놓음. 설마 필요한 경우가 있을까?
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

	//			// ActorHiddenIngame 설정 됬을때만
	//			if (SKActor->bHidden)
	//			{
	//				// 또한, 간혹 의도치 않게 보이지 않는 상태로 애니메이션이 플레이되고 있어서 문제가 되는 경우가 있음. (AnimNotify 발생) 따라서 이것들도 없애 줌. DirectedFromBeginning 이면 시작 시 애니메이션 직접 재생.
	//				SKActorComp->AnimationData.bSavedPlaying = false;
	//				SKActorComp->AnimationData.bSavedLooping = false;
	//				if (!bHasAnyDirectedFromBeginningSetup)
	//				{
	//					// AnimToPlay 레퍼런스는 특히나 연출 용 데이터의 선별 로딩을 하게 되면 필요없는 로딩을 유발하므로 최적화 가능성 면에서라도 없애주는 게 좋은데
	//					// DirectedFromBeginning 세팅과 얽힌 경우는 없애선 안 된다.
	//					// 이 SkeletalMeshActor 가 전체 StageEventDirector 중 어느 하나하고라도 DirectedFromBeginning 으로 엮었는지 검사하려니 좀 버겁고 해서 
	//					// 그냥 전체 월드에 그런 세팅이 없는 경우만 비움. 그렇게 많이 사용하는 건 아니라서..
	//					SKActorComp->AnimationData.AnimToPlay = nullptr;
	//				}
	//			}
	//		}
	//	}
	//}

	//// ShowSetting 이랑 연결이 되어있는 SkeletalMeshActor 들은 visibility 등의 추가 세팅까지 해 놓음.
	//for (int32 SI = 0; SI < ShowSettings.Num(); ++SI)
	//{
	//	FPlayActorShowSettings& ThisSetting = ShowSettings[SI];
	//	TArray<class AActor*> GroupActorsForThisSetting;
	//	// ThisSetting 과 연결된 그룹액터들 모두 모음. PC 가 아니라면 웬만하면 하나일 것.
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
	//		ThisSKComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered; // 이것도 연출 도중에는 활발히 업데이트하도록 값을 바꿔주니 기본값은 떨어뜨려 놓도록.

	//		// 추가로 MaterialInstanceDynamic 이 overriding 되어 있다면 날려준다. ClearControlledMatineePuppet 에서 하려는 건데 뭔가 잘 안먹혀서 여기서 보다 확실하게..
	//		// MID 란 게 처음부터 저장을 하기 위한 게 아니다.
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

	// 나머지 이름 똑같아지도록 확인차원
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
//		// 적어도 하나는 있도록 함.
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
//		// MainPlayer 나 SubPlayer 가 둘씩 나오지 않게 체크 및 경고
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
//						FString::Printf(TEXT("[Warning] 동일한 플레이어 캐릭터의 PlayActorType 이 중복되어 나타났습니다.\nIndex %d and %d"), IA, IB)
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
//			FString WarnMsg = TEXT("현재 Directing Event 설정에서는 Mob 이 등장하지 않은 상태에서 연출 장면이 시작될 위험이 있습니다. 특별한 이유가 아니면 Mob Appear/Dying/Phase 를 사용하거나 Show Settings 에서 Wave Mob 을 없애주세요.\n\n");
//			
//			if (DirectingEvent == EStageEvent::ESEV_ComponentTrigger)
//			{
//				WarnMsg += TEXT("특히 ComponentTrigger 자주 실수하는 설정이니 확실히 문제없고 필요한 곳에만 확인하고 사용하세요!\n\n");
//			}
//
//			WarnMsg += TEXT("Mob 이 등장하는 Show Setting 번호 : ");
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
//				FString::Printf(TEXT("[Warning] MobDying 이벤트에 StartDelay %.2f (이)가 설정되어 있습니다.\n기본 지원은 되지만 Dying Mob 이 등장하는 경우 StartDelay 가 길수록 동작이 보장되지 않을 가능성이 높습니다.\n가급적 다른 방법을 찾거나 확실한 테스트를 거치는 것이 좋습니다."), OverallStartDelay)
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
//				// 실제 있는 Wave Num 을 썼는지 확인
//				for (int32 SI = 0; SI < ThisSP->GetStageSettingsNum(); ++SI)
//				{
//					// SpawnPool 의 TargetDifficulty 는 낮을수록 실제 사용가능한 난이도가 많아진다. 
//					// 그걸 감안하자면 이번 세팅의 TargetDifficulty 보다 높은 난이도로 가면서 체크해볼 필요는 있는데 그렇게 하자면 여러 상황을 체크해야 해서.. 걍 아래 메시지를 보강하는 정도만.
//					int32 ThisSPSettingDifficultyLevel = ThisSP->GetTargetDifficultyLevelBySettingIndex(SI);
//					if (IsStageSupported(ThisSP->GetTargetStageIdBySettingIndex(SI), ThisSPSettingDifficultyLevel) == false)
//					{
//						continue; // 지원되는 스테이지 세팅만.
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
//				FString::Printf(TEXT("[Warning] Mob Event 사용을 위한 Spawn Pool 액터가 발견되지 않았습니다."))
//				));
//#endif
//		}
//		else if (!bFoundWaveSetting)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[WARNING!] 월드에 존재하는 Monster Spawn Pool 액터에서 지정한 스테이지/난이도와 지정한 %d 번호의 Wave Setting 을 찾을 수 없습니다.\n\n난이도의 경우 SpawnPool 에서는 낮은 난이도 설정이 높은 난이도를 대신할 수 있으므로 실제로는 사용 가능할 수도 있습니다."),
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
//			// PlayActor 기준으로 위치를 세팅하는 거라면 물론 그 PlayActor는 하나만 있어야겠긔.
//			if (ShowSettings[SI].IsOneOfAdjustToPlayActorPlacement())
//			{
//				++TotalCount;
//			}
//		}
//		if (TotalCount >= 2)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] %d 개의 AdjustToPlayActor 계열 세팅이 발견되었습니다.\n하나만 켜두는 것을 권장합니다."), TotalCount)
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
//				FString::Printf(TEXT("[주의] MatineeFirstSkipFrames 설정만큼 첫 %d 프레임이 스킵됩니다.\n\n에디터에서 보는 모습과 최종 릴리즈에서 보는 모습과는 다를 수도 있으므로 너무 큰 값은 사용하지 않는 것이 좋습니다."), MatineeFirstSkipFrames)
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
//				FString::Printf(TEXT("Clear 이벤트가 나와야 할 상황에서 스킵될 소지가 있는 ManageMode 로 설정이 되었습니다."))
//				));
//#endif
//		}
//	}
//
//	if (PropertyName == Name_RandomMatineeGroupNum)
//	{
//#if !PLATFORM_MAC
//		if(RandomMatineeGroupNum == 1) // 1 이면 실제로는 랜덤 동작을 하지 않는 것인데 혹 랜덤 동작을 의도하고 넣었을지도 모르니.
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("RandomMatineeGroupNum 1 설정에서는 실제로 랜덤 연출 기능이 작동하지 않습니다.\n만일 랜덤 연출 기능을 의도했다면 2보다 큰 값으로 설정해 주세요."))
//				));
//		}
//		else if (RandomMatineeGroupNum >= 2)
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("RandomMatineeGroupNum 을 %d (으)로 설정했습니다.\n실제 랜덤 연출 기능을 위해서는 사용하는 Matinee 의 각 그룹들을 여기에 맞춰 적절한 Rand 네이밍으로 설정해야 합니다.\n자세한 사항은 관련 매뉴얼을 참고하세요."),
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
//				FString::Printf(TEXT("LocalPlayerSwitching 을 켭니다. Matinee 재생시 시작하는 캐릭터와 끝나는 캐릭터가 다른경우 마지막 연출 캐릭터로 로컬캐릭터를 스위칭 시킵니다.\n\n주의 : 연출에 의해 게임 flow 가 제어됩니다. 기능 개발에 따라 작동하는 조건이나 게임모드가 한정될 수 있습니다."))
//			));
//#endif
//		}
//	}
//
//	CheckAndWarnForLightSetup();
//
//	//if (
//	//	PropertyName == Name_OverallMode || PropertyName == Name_ShowSettings || // ShowSettings 추가시에도 작동하도록
//	//	PropertyName == Name_bOverallMode_ExportedCamera || PropertyName == Name_bOverallMode_ControlledMatinee ||
//	//	PropertyName == Name_bOverallMode_ManualCamera || PropertyName == Name_bOverallMode_ExportedOrManualCamera
//	//	) 
//	// 걍 언제나 작동하도록 해야 할 듯.
//	{
//		SetEditConditionProperties();
//	}

	// 현재 상황으로는 프리뷰 컴포넌트의 위치값을 속성창에서 변경해서 오는 notify 가 없으므로 여기서 Sync 는 못하고 Refresh 만..
	RefreshPreviewSKComps();
}

void AB2StageEventDirector::SetEditConditionProperties()
{
	bOverallMode_ExportedCamera = false;
	bOverallMode_ControlledMatinee = false;
	bOverallMode_ManualCamera = false;
	bOverallMode_ExportedOrManualCamera = false;

	// OverallMode 선택에 따라 나머지는 일괄적으로
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

	// 이것도 여기 끼워넣긔.
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
		// Preview 컴포넌트를 이동시켰을 수 있으니 위치부터 싱크
		SyncFromPreviewSKCompPlacement();
		// 여기서는 RegenerateSKComp 를 하지 않는다. PostEditMove 상황에서는 딱히 regenerate 를 할 필요도 없고, 무엇보다 매 이동시마다 선택 포커스를 잃어버리면 불편하기 때문.
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
//				// 실질적으로 Light 를 등록한 InterpGroup 은 부모 Folder Group 네이밍으로 On/Off 를 한다.
//				UInterpGroup* ParentFolderGroup = FindParentGroupFolder(GroupOfThisInfo, MatineeActor);
//
//				if (ParentFolderGroup && IsPossibleInterpGroupNameForPC(ParentFolderGroup->GroupName.ToString()))
//				{
//					++LightCount_PossibleManagedGroup; // 이걸로 더해진 것들 중에는 아마 단 하나만 켜질 것으로 예상..
//				}
//				else
//				{
//					++LightCount_NoneManagedGroup;
//				}				
//			}
//		}
//	}
//	const int32 TotalLightCount = LightCount_NoneManagedGroup + LightCount_PossibleManagedGroup;
//	// LightCount_PossibleManagedGroup 도 세팅에 따라서 엉뚱하게 세팅되면 둘 이상 켜질 수 있는데.. 그것까지는 더 이상 못하겠고..
//	const int32 TotalLightCount_TurnOnTogether = (LightCount_NoneManagedGroup + (LightCount_PossibleManagedGroup > 0 ? 1 : 0));
//
//	if (TotalLightCount > AllowedMatineeTrackPointLightNum)
//	{
//		if (TotalLightCount_TurnOnTogether > AllowedMatineeTrackPointLightNum)
//		{
//			// 실제로 문제가 될만한 상황.
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(
//					TEXT("%s 에서 제어하는 PointLight 가 모바일 렌더링의 전체 제한을 넘길 것으로 예상됩니다. 다음 설정들을 확인해 주세요.\r\n\r\n")
//					TEXT("1. HidePlayerPointLight : %d %s\r\n")
//					TEXT("2. AllowOtherPlayerPointLight : %d\r\n")
//					TEXT("3. Matinee 에 등록한 PointLight 트랙이 의도대로 제어되는지 확인해 주세요.\r\n")
//					TEXT("  - 선별적 On/Off 제어가 되는 그룹에 등록된 PointLight 개수  : %d\r\n")
//					TEXT("  - 선별적 On/Off 제어가 안 되는 그룹에 등록된 PointLight 개수 : %d"),
//					*GetName(), bHidePlayerPointLight, (bHidePlayerPointLight ? TEXT("") : TEXT("(특히 이 값이 0 이면서 이 경고가 뜨면 위험합니다)")), 
//					bAllowOtherPlayerPointLight, LightCount_PossibleManagedGroup, LightCount_NoneManagedGroup)
//			));
//#endif
//		}
//	}
}
//
//UInterpGroup* AB2StageEventDirector::GetInterpGroupOfObjectName(FName InFindGroupObjectName)
//{
//	// FInterpGroupActorInfo 의 ObjectName 을 가지고 찾고자 하는 거.
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
	if (!InWorld) {
		return;
	}

	// 특히 자주 실수해서 버그를 야기하는 설정이라 특별히.. 전체 StageEventDirector 돌면서 문제 설정 모아서 경고 띄움.
	// 몹이 등장해도 ComponentTrigger 로 사용 가능한 경우가 있긴 할껀데 그것보단 실수가 넘 많네..
	
	TArray<AB2StageEventDirector*> FoundProblematicSED;

	for (FActorIterator ActorIt(InWorld); ActorIt; ++ActorIt)
	{
		AB2StageEventDirector* CastedSED = Cast<AB2StageEventDirector>(*ActorIt);

		if (CastedSED && CastedSED->DirectingEvent == EStageEvent::ESEV_ComponentTrigger)
		{
			bool bHasMobSetting = false;
			for (int32 SI = 0; SI < CastedSED->ShowSettings.Num(); ++SI)
			{
				if (CastedSED->ShowSettings[SI].PlayActorType == EStageEventPlayActor::ESEE_WaveMob)
				{
					bHasMobSetting = true;
					break;
				}
			}
			if (bHasMobSetting)
			{
				FoundProblematicSED.Add(CastedSED);
			}
		}
	}

	if (FoundProblematicSED.Num() > 0)
	{
#if !PLATFORM_MAC
		//FString WarnMsg = TEXT("Wave 몹이 등장하는 다음 StageEventDirector 액터들에 ComponentTrigger 시작 설정이 되어 있습니다. 이런 설정은 빈번하게 겪는 스테이지 연출 진행 버그를 야기합니다. 연출 시작 위치는 기획자와의 협의 하에 SpawnPool 을 통해 제어하거나 PCSyncToMatineeStart 기능을 활용하세요.\r\n\r\n");
		FString WarnMsg = TEXT("Wave");
		
		for (int32 SI = 0; SI < FoundProblematicSED.Num(); ++SI)
		{
			AB2StageEventDirector* ThisOne = FoundProblematicSED[SI];
			WarnMsg += FString::Printf(TEXT("%d. %s\r\n"), SI+1, *ThisOne->GetName());
		}

		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMsg));
#endif
	}
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