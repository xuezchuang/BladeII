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

// �׳� CachedStageManager �� üũ�ϸ� StageManager �� ���� ���Ӹ�忡�� �ٷ� �ɸ�
#define SAFE_STAGEMANAGER_CHECK BII_CHECK( !(Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(this))) || CachedStageManager );

ABladeIICharacter* FPlayActorShowRuntimeState::GetPlayActor(class AB2StageEventDirector* OwnerSED)
{// LocalPlayerCharacter �� ĳ���ؼ� ���ٰ� ������ �� ���� �� �־ ���� ��������.. �ٵ� �ٸ� PlayActor �� �׷� �� ���� �� �ѵ�.. üũ�� �ɾ�..
	ABladeIICharacter* RetChar = NULL;// bIsForLocalPlayerCharacter ? Cast<ABladeIICharacter>(UGameplayStatics::GetLocalPlayerCharacter(OwnerSED)) : ThePlayActor;
	// PlayActor �� NULL �� �� �ִ�. ���� ��� �ټ��� ���������� ���� ShowSetting �ε� SpawnPool ������ � ������������ ���� �� ���´ٰų� �ϴ� ���. ���� validity üũ�� �ϵ���.
	//ensure(!RetChar || RetChar->IsValidObj()); // ���� ���߿� Directed �� �ƴ� (PlayActorStateMode) PlayActor �� destroy �Ǿ�����ٸ� IsValidObj �� �ƴ� �� �ִ�. ����μ� ���ÿ� ���� ���ɼ��� �ֱ� ��..
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
	{// �׾�� �ֶ� IsValidObj ����..
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
{ // �ذ��� �ϴ� �Ŵ� ���� valid �˻������ ��
	if (InMobToForget == TheDyingMob)
	{
		ForgetIt();
	}
}
ABladeIICharacter* FDelayedDyingMobCache::GetIfMatch(int32 InWaveIndex, int32 InObjIndex)
{
	if (IsValid(TheDyingMob ) && InWaveIndex == WaveIndex && InObjIndex == ObjIndex) 
	{ // �׾�� �ֶ� IsValidObj ����..
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
	//// �����Ϳ��� �� �� �� ����� ���� ��찡 �־ �����ʹ� check ��ŵ
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
				: SpriteTexture(TEXT("/Engine/EditorResources/SceneManager")) // SceneManager �ؽ��İ� ���� ���⽺���� ���� ����
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

	// ManualCameraSettings �ϳ��� �⺻���� �־��ش�.
	ManualCameraSettings.Add(FManualCameraSingleSetting());

	// ī�޶� ����. ���� ��ġ ���� ������ ���� BP Ŭ���� ����Ʈ�� ���� ����Ʈ���� �ϱ�
	DirectorCamera_0 = CreateDefaultSubobject<UCameraComponent>(TEXT("DirectorCamera_0"));
	DirectorCamera_0->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	DirectorCamera_0->bAutoActivate = false;
	DirectorCamera_0->SetRelativeLocation(FVector(50.0f, -100.0f, 100.0f));
	DirectorCamera_0->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	DirectorCamArrayRef[0] = DirectorCamera_0; // ���� ó������ array �� �����ϰ� �����Ϳ��� �����ϵ��� ������ �ȵż� �̷��� ��.
	
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

	// Ȥ�ö� ���� �̸� �ٲٸ� redirector ó���� �ȵǹǷ� ���⼭�� �˻�.
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
	// ó���� �̰� PostLoad ���� �Ϸ��� �ߴµ� Preview �� ������Ʈ���� ���ٴ� �������� �ε� ���Ŀ� �ȸ����� ����� ����.
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
	//	// MAX_DIRECTOR_CAMERA ���̸鼭 ������ �⺻ �����Ǿ� �ִ� �� ����.
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

	//// ������ ���� (WaveMob ����ϴ� ���) �̰� �Ҹ��� ���� BeginShow �� �� ���� ������ ���.

	//// ESEV_ComponentTrigger ������ ������ ������Ʈ�� BeginOverlap ���
	//if (DirectingEvent == EStageEvent::ESEV_ComponentTrigger && TriggerComponentName != NAME_None)
	//{
	//	UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(FindComponentByName(TriggerComponentName));
	//	if (PrimComp)
	//	{
	//		PrimComp->OnComponentBeginOverlap.AddDynamic(this, &AB2StageEventDirector::SelectedComponentTriggerBeginOverlapCB);
	//	}
	//}

	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && HasDirectedFromBeginningTrack() && MatineeActor && 
	//	!bCurrentlyPlaying)// ó������ �÷����ϴ� �Ŷ�� �̷� ó���� �ʿ������� �ʰ���..
	//{
	//	MatineeActor->InitInterp(); // GetGroupActorOfGroupName ������ �ʿ�..
	//	for (int32 SI = 0; SI < ShowSettings.Num(); ++SI)
	//	{
	//		FPlayActorShowSettings& CurrSS = ShowSettings[SI];
	//		if (CurrSS.PlayActorStateMode == EPlayActorStateMode::EPASM_DirectedFromBeginning &&
	//			// ���� �÷��̾� ĳ���� �� Ʈ���̸� �̷��� ó������ ������ ��ġ�� �����ϵ��� �ϴ� �� ���� ���� �ʰ�..
	//			// ��Ÿ �ٸ� Ÿ���� �÷��̾� ĳ���Ͷ�� �� ���������� ������ �̸� ������ ��� ã�Ƴ��� �� ��. PlayActor �� �ִ� ������ �ƴϹǷ�.
	//			// ���ϰ� ������ �׷� ��� ���� �Ұ�.
	//			!IsPlayActorTypeForPlayerCharacter(CurrSS.PlayActorType)
	//			)
	//		{
	//			// �÷��̾� ĳ���Ͱ� �ƴϴ� MatchingInterpGroupName �� �״�� ���.
	//			ASkeletalMeshActor* ThisGroupSKActor = Cast<ASkeletalMeshActor>(GetGroupActorOfGroupName(CurrSS.MatchingInterpGroupName));
	//			USkeletalMeshComponent* SKMeshComp = ThisGroupSKActor->GetSkeletalMeshComponent();
	//			if (ThisGroupSKActor && SKMeshComp)
	//			{ // ó������ ���̵��� �� �ش�. �ִϸ��̼ǵ� �Ƹ� �÷��� ���¿��߰���..? �װ� �ƴ� ������ �ִٸ� �ᱹ �߰� �ɼ��� �־��.
	//				if (SKMeshComp->AnimationData.AnimToPlay){
	//					// AnimToPlay ���õ� �� ����ϴ� AnimToPlay �� ����� ���ϰڴµ�..
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
	// �ߺ��ؼ� �Ҹ� �� �ִ�.

	SAFE_STAGEMANAGER_CHECK
	if (CachedStageManager)
	{
		// �� �������� StageManager �� �ʿ��� ���� ��� �־�� �Ѵ�. (�Ƹ��� ABladeIIGameMode::PreBeginPlay ������)
		CurrentStageId = CachedStageManager->GetCurrentClientStageId();
		CurrentStageDifficulty = CachedStageManager->GetStageDifficultyLevel();
	}

#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
	// �ν��Ͻ� �� �������� �� save state �о����. �о���� ������ ����� ������ ���� skip �� ����ϰ� �� �� �ִ�.
	GConfig->GetBool(*GetPlayedAtLeastOnceSectionString(), *GetPlayedAtLeastOnceKeyString(CurrentStageId), bPlayedAtLeastOnce, GB2LocalCutSceneSavedStateIni);
#endif
}

int32 AB2StageEventDirector::GetAllowedMatineeTrackPointLightNum() const
{
	TConsoleVariableData<int32>* MobileNumDynamicPointLightsCVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.MobileNumDynamicPointLights"));
	const int32 MobileNumDynamicPointLights = MobileNumDynamicPointLightsCVar ? MobileNumDynamicPointLightsCVar->GetValueOnGameThread() : 2;
	// �÷��̾� point light �� ����Ҳ��� �װ� ����.
	// ���⼭ bAllowOtherPlayerPointLight �� �߰��� �� ����.. �װ͵� �ϴ� 1 �� ġ��..
	return bHidePlayerPointLight ? MobileNumDynamicPointLights : (bAllowOtherPlayerPointLight ? (MobileNumDynamicPointLights - 2) : (MobileNumDynamicPointLights - 1));
}

void AB2StageEventDirector::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	
	if (GIsEditor && GIsPlayInEditorWorld == false)
	{
		SafetyProcessMatineeSkActors(); // BeginPlay ���� �ϴ��� ������ �������� ���⼭ �ϴ� �� ������..?

		SyncFromPreviewSKCompPlacement(); // ������ ������Ʈ�� ������ ��ġ ��ũ
	}
#endif
}

void AB2StageEventDirector::BeginDestroy()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::BeginDestroy"));

	Super::BeginDestroy();

	// ESEV_ComponentTrigger ������ ������ ������Ʈ�� BeginOverlap ��� ����
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
	
	// �ᱹ �������� Tick ��ƾ����..
	if (IsPlayingShow())
	{
		if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
		{
			for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
			{
				FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
				FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];

				// BeginSingleShow ���� �ϴ� �Ϻ� �ݺ�.
				if (CurrShowSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_ContinuouslyAdjustToPlayActor)
				{
					// Position �� ���󰡵���.. �Ϸ��� �ߴµ� ���� �� �ȵǴ±�
					AdjustSelfPlacementToPlayActor(CurrShowState.GetPlayActor(this), CurrShowSetting.ManualRelativeLocation, CurrShowSetting.ManualRelativeRotation);

					// ExportedCamera ��忡���� ExportedEventScene �� ���� �ʱ� ��ġ�� �����Ƿ� SetSESCameraMovementFromPlayActor �� �� ������ �Ѵٰ� �ص� �ǹ� ����.

					break; // �ϳ��� ó���Ѵ�. ������ ������ �ִ� �� PostEditChangeProperty ���� �����
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


	//// DirectingEvent �� ESEV_ComponentTrigger �� ��� ���� ó��.
	//// ���� �˻縦 �� �ص� �ǰ����� ���ǻ�
	//if (DirectingEvent == EStageEvent::ESEV_ComponentTrigger && TriggerComponentName != NAME_None &&
	//	bTargetIsLocalCharacter && // ���� ���� �÷����ϴ� ĳ���Ϳ� ���ؼ��� + ž�����̸� Ż������ ��
	//	SweepResult.GetComponent() == Cast<UPrimitiveComponent>(FindComponentByName(TriggerComponentName)) &&
	//	!bDirectingEventConditionMet)
	//{
	//	bDirectingEventConditionMet = true;
	//	BeginShowOrBeginCountDown();
	//}
}

void AB2StageEventDirector::CustomBeginShow()
{
	BeginShow(); // �ϴ��� �ܼ� call. �����̶��.. BeginPlay ���Ŀ��� ���� ��
}

void AB2StageEventDirector::CustomFinishShow()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::CustomFinishShow"));

	//// ���� �÷��� �������� ������� �Ҹ� �� �־�� ��. FinishShow ���� ó���ϴ� �͵� �� ����.

	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && bPCSyncToMatineeStart && bCurrentPCSyncToMatineeStart)
	//{// ��Ƽ�� PCSync �����̶�� �������� �������� ���� ���� ���̶� ������ �� ���ƾ� ��.
	//	BeginMatineeFromPCSyncToMatineeStart();
	//}

	//for (int32 SI = 0; SI < ShowStates.Num(); ++SI)
	//{
	//	FPlayActorShowRuntimeState& CurrState = ShowStates[SI];
	//	
	//	if (CurrState.bShowStarted == false)
	//	{
	//		// ���� ��� Ÿ�̸Ӱ� ���� ������ �� �����Ƿ� ����
	//		GetWorldTimerManager().ClearTimer(CurrState.ShowStartDelayTimerHandle);
	//	}
	//}

	//// �⺻�����δ� �ڵ����� Stop �� �Ҹ�.
	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor)
	//{
	//	if (MatineeActor->bIsPlaying)
	//	{
	//		MatineeActor->Stop(); // ���߿� ���������ϴ� �Ŷ�� �̰� ���ؼ� �Ǵٽ� CustomFinishShow �� ����� ��.
	//		return;
	//	}
	//}

	//APlayerCameraManager* CamManager = UGameplayStatics::GetLocalPlayerCameraManager(this);
	//if (CamManager)
	//{
	//	CamManager->StopAllCameraAnims();
	//}

	//// ��Ƽ���� ��� ���߿� ���� ���� ������ �÷��̸� ������ �����ؼ� ��ġ ��ũ ���� ���� �ǵ��� ��� �����ϵ��� ��.
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
//	if (GCheat_CompleteIgnoreEventScene) // �̰� �ƿ� �������� ���� �ʴ� ġƮ�̴�. ���� �÷��̿� ������ �� �� ������ Ư���� �׽�Ʈ�� ���� ����Ѵ�. �Ϲ����� ���� ��� ����� GCheat_SkipEventSceneCount �� ����.
//	{
//		return false;
//	}
//#endif
//
//	// �߸��� ������ ���
//	if ((OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor == NULL) ||
//		(OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera && CameraAnim == NULL))
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		if (GIsEditor)
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] %s �� ���� Overall Mode �� �´� ������ ���� ������ �����Ǿ� ���� �ʽ��ϴ�. Stage Event �� �������� �ʽ��ϴ�."), *this->GetName())
//				));
//		}
//#endif
//		return false;
//	}
//	/*
//	// ������ ���� ���
//	if (!FBladeIIBlockToSyncNetwork::GetInstance().IsConnectedToServer()
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		&& !(CachedStageManager && CachedStageManager->DevOnlyIsStandalonePlay()) // ������ ������ �÷���, ��������� Standalone ���� �÷����ϴ� ��� ��. ���� ���� ������� ����.
//#endif
//		)
//	{
//		return false;
//	}
//	*/
//
//	// ManageModeLeastSkip �� �� ��ȹ�� ����. Skip �ϴ� �� �ƴ϶� �׳� �߱�� �͸�.
//	if (CheckSkipForManageMode(this, ManageMode, false /*bManageModeLeastSkip*/) == EStageEventSkipType::SEST_CompleteIgnore){ // ���� ����ص� �ٽ� �˻��ϰ� �� ��.
//		return false;
//	}
//
//	if (IsStageSupported(CurrentStageId, CurrentStageDifficulty) == false){
//		return false;
//	}
//	
//	if (bPlayedOnceForThisStage){ // ��� �ϸ� �� �������� �ȿ��� ������ �÷��̰� ���������� �𸣰����� ���� �׷� �ʿ䰡.. ���� ������ ���� ������ ��õ ����.
//		return false;
//	}
//
//	// �ɼǿ� ���� skip ���� �� ���⼭ return false �ϴ� ������ ó������ �ʴ´�. BeginShow ���� �ٷ� CustomFinishShow �� �ϴ� ������ ó���ؾ� �� ��.
//
//	return true;
//}
//
//// ���� ��ε� ���� Event ����� �ٽ� �÷��� �����ϵ���. ��� ������ ���̹�.
//void AB2StageEventDirector::ResetAssumeNoWorldLoading()
//{
//#if !UE_BUILD_SHIPPING
//	CheckResetAssumeNoWorldLoadingCondition(this);
//#endif
//
//	// ���� �÷��� �� �� �� ���� �ܾ� �ν��� ���� �ʿ� ����. ������ �� ���ۿ��� �߰ߵǾ���.
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
//	// �÷��� �� �� �� SetPosition(0) ���� �� ��Ȥ ���ۿ��� �ִ� �ɷ� ���� �̰� ��ü�� �Ϻ��� ���� ������ �ƴ� �� ������ ��ü�� ����.
//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && MatineeActor && MatineeActor->MatineeData)
//	{
//		MatineeActor->SetPosition(0.0f);
//
//		// ���������� �����ϴ� Ʈ���� ��� ��Ȱ��ȭ�� ���� ���� ���̹Ƿ� ��� enable .. �ϸ� ó������ disable �� �͵� enable �ǰ���? �� ����� ���� �������� �����ؼ� ����ؾ�..
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

	// bLeastSkip �� ���� �б�� StageEventDirector �� �������� ���� ����. �� ����� �۵����� �ʴ� �� ��������. �ٸ� ������ ����� �� ����� Ư���� ������ ���� ���� �̻� �� �ǹ̴� ���� ��.
	// -> 2017.03 ���� ��ȹ���δ� LeastSkip �� ���� ��� �ȵǴ� �Ŵ� CompleteIgnore �� ��. ���ܸ� ��.
	return bLeastSkip ? EStageEventSkipType::SEST_LeastSkip : EStageEventSkipType::SEST_CompleteIgnore;
}

bool AB2StageEventDirector::IsAdventureModeGame(UObject* WorldContextObject)
{
	AGameMode* CurentGM = Cast<AGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// ���� StageGameMode ���� ��ӹ��� �ٸ� ���Ӹ�尡 ������ �� �߰� �˻�. �Ϲ� �������� �÷���, �� "����" ��� ���θ� ������ ��.
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
{ // ���Ӹ�� �÷��� ���� ���� �÷��̰� ����Ǵ��� ����.
	return IsStageSupportedForGameMode(InB2GM) && (CheckSkipForManageMode(InB2GM, ManageMode, false) == EStageEventSkipType::SEST_Play);
}
bool AB2StageEventDirector::IsStageSupportedForGameMode(class ABladeIIGameMode* InB2GM) const
{
	// �̰� True ���� ���� �÷��̴� �� �� �� �ִ�. �� ���ɼ��� ���� Ȯ���ϰ� üũ�Ϸ��� IsExpectedForGameMode �� ����.

	AB2StageGameModeBase* CastedSGM = Cast<AB2StageGameModeBase>(InB2GM);
	AB2StageManager* StageManagerIfAny = CastedSGM ? CastedSGM->GetStageManager() : NULL;
	if (StageManagerIfAny)
	{
		return IsStageSupported(StageManagerIfAny->GetCurrentClientStageId(), StageManagerIfAny->GetStageDifficultyLevel());
	}
	return true; // StageGameMode �� �ƴ� ���� �⺻ �����Ǵ� �ɷ� ����. ���� �������� ������ ���� �⺻ ���� ����� ��.
}
bool AB2StageEventDirector::IsStageSupported(int32 InStageId, EStageDifficulty InDifficulty) const
{
	return IsStageNumSupported(InStageId) && IsStageDifficultySupported(InDifficulty);
}

bool AB2StageEventDirector::IsStageNumSupported(int32 InStageId) const
{
	if (SupportedStages.Num() == 0)
	{
		return true; // ��� �ִ� ��� �⺻���� ��� �������� ����
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
		return true; // ��� �ִ� ��� �⺻���� ��� ���̵� ����
	}
	const int32 TestDifficultyInt = static_cast<int32>(InDifficulty); // �� ��� EStageDifficulty �� ���� ��� Int �� ���� �ߴ��� �� ����� �ȳ�. �� ���ž�� �׷� ��
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
	//		// ���������� �������� ���� ������ MobAppear �� �ϴ����� �̰� �Ǻ��� �� �ִ°� EventWidgetType �� ��.��;
	//		(DirectingEvent == EStageEvent::ESEV_MobAppear && EventWidgetType == EStageEventWidgetType::StageBegin) || 
	//		// �ٵ� ���� �� ó������ ������ �̺�Ʈ ������ ���η�..��
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
//		// Ʃ�丮�� QTE ���·� �ٷ� ��Ƽ�� ����
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
//		// <--QTE���̸� �ݹ� �����ϰ� ����
//		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerController(this)->GetPawn());
//
//		if (pPlayer && pPlayer->IsInQTEState())
//		{
//			pPlayer->AddQTEOnEndCallBack(FQTEOnEnd::CreateLambda([this]() { this->BeginShow(); }));
//
//			// Ż�� QTE�� ������
//			if (pPlayer->QTEType == EQTEType::EQT_Mount)
//				pPlayer->StopQTEMount();
//
//			// ����ó�� ��� ������� ����°� �̸� �ѹ� ����.
//			if (DirectingEvent == EStageEvent::ESEV_MobAppear)
//			{
//				// Settings �� RuntimeState ���ڸ� �����ش�.
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
//				// �÷��̾�� �ȼ���
//				pPlayer->SetHideFromEventDirector(false);
//			}
//
//			return;
//		}
//
//		// -->QTE���̸� �ݹ� �����ϰ� ���� ��
//	}
//	
//	B2_SCOPED_TIME_LOG(FString::Printf(TEXT("AB2StageEventDirector::BeginShow %s"), *GetName()));
//
//	bEnterCinematicMode = false;
//
//	// ������ �� ����!
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
//	// BP ���� Ư���� �̺�Ʈ�� �޾Ƽ� ó���ϰ��� �Ѵٸ�..
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
//		// StageClear UI �� �ѷ� ������ ���� Clear ������ �¸����� ������ UIStageClear ���� UIStageResult �� �Բ� ����ǹǷ� ó������ BeginShow ��� ���� ó���� ���� ��ȣ�� ������.
//		// �� Phase1 �� ������ EStageEvent �� �� ���� ������ ���� UI �����̹Ƿ� ���� ó���� ���� �� ������� �䱸���� ������ �ȵȴٸ� ���� EStageEvent �� ���������� �ѷ� ������ �� �ִ�.
//		
//		MyRelevantUIName = UIFName::StageClear; // �̰� EventWidgetType üũ ���� �� Ư���ϰ� ���õǴµ� Phase1 ��ü�� ��ǻ� StageClear UI �θ� �̷������ �� ����.
//		BeginStageClearScene_Phase1Class<>::GetInstance().Signal();
//
//		ApplyStageEventTimeDilation();
//		
//		// ���� ���� ������ �������� ���� ����(�¸�) ���� �ߵ��� AB2StageManager::BeginStageClearScene_Phase2 ����.
//		// �ٽ� ���� ���� BeginShow �� �ϵ��� ClearEventPhase ����.
//		ClearEventPhase = EStageClearEventPhase::ESCE_Phase2;
//
//		if (LocalB2Player)
//		{ // bForceStopPlayerAttackState ���� �ϴ� ó���ε� ������ �� ������ ���⼭ ����.. �Ѥ�
//			LocalB2Player->SignalStopAttackImmediate();
//		}
//
//		return;
//	}
//		
//	if (ShouldApplyCinematicScalabilityDuringShow())
//	{
//		if (LocalB2Player) {
//			LocalB2Player->SetupLODInfoForCinematic(); // ���� �÷��̾�� ���� ��� �Ϻΰ� �ƴϴ��� ī�޶��ũ�� ���� ���İ� ���� ������ �⺻ cinematic ������ ����
//		}
//		B2Scalability::ApplyStageEventSpecificScalabilitySetting(this, true); // ���� �÷��̾� LOD ���ð� ���������� ���� �� Ư�� scalability ��Ʈ�� ����
//	}
//
//	// Settings �� RuntimeState ���ڸ� �����ش�.
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
//		CurrentActiveCamera = LocalB2Player->TopDownCameraComponent; // �Ƹ��� �̰� ���������� ���� ���̴�..
//
//		if ((DirectingEvent != EStageEvent::ESEV_MobDying && DirectingEvent != EStageEvent::ESEV_FinishAttack))
//		{
//			/*
//			// ���� �� �������̶� ���� �� ������ ������ �̾�� �ʾƾ� ��
//			if (LocalB2Player->IsKeepAttacking() && bForceStopPlayerAttackState) // bForceStopPlayerAttackState ������ �߰��� �˻�.. ���� ���� ������ �ƴѱ� �ߴµ� �뷫 �´´�.
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
//	// ī�޶� ���ú���
//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera)
//	{
//		APlayerCameraManager* CamManager = UGameplayStatics::GetLocalPlayerCameraManager(this);
//		if (CamManager && CameraAnim)
//		{
//			// �ϴ� ��� deactivate �� �� ����..
//			DeactivateAllManualCams();
//			
//			// ������ Ȱ��ȭ �� ���� �ִٸ� ��� ����. �׷��� ��� �̷��� �ص� show ���߿� �÷��� �ϴ� �ִϸ��̼ǿ� ī�޶� �ִ��� �پ� �ִٸ� �װ� ������ �� �� ����.
//			// ���� SetStageEventShowState �� ���� ���� AnimBP �� �ش� �ִϸ��̼��� ������ �����ؾ� ��.
//			CamManager->StopAllCameraAnims(true); 
//
//			// CameraAnimApplyCamIndex �� ���� �⺻ ī�޶� �� view target ���� ����
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
//			// �ǵ��� �÷��̾� �ʿ������� ���� �Ǵ� �Ŷ�� �ٽ� ���ֱ� �ؾ� ��.
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
//				LocalB2PC->SetViewTargetWithBlend(CameraAnimViewTarget, CameraAnimInitialBlendingTime, EViewTargetBlendFunction::VTBlend_Linear); // BlendFunction �ʿ��ϸ� �����ϰ�..
//
//			// �ʿ��ϸ� CameraAnimInst �� ĳ���ؼ� ���.
//			UCameraAnimInst* CamAnimInst = CamManager->PlayCameraAnim(CameraAnim);
//			if (CamAnimInst)
//			{
//				if (bStandbyAtTheEnd)
//				{
//					CamAnimInst->bStayAtEnd = true; // ī�޶� �ִϸ��̼� ���� ���¿��� �����ϰ� �� ��. �� ��� ������ �� CustomFinishShow �� ���.
//				}
//				CamAnimInst->AddCustomNotifyActor(this); // ���� ������ �ޱ� ���� ���. ���� �� NotifyActorCustomEvent ��.
//			}
//		}
//	}
//	else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//	{
//		// Controlled Matinee �� ���� �ϴ� BeginSingleShow �� delay ���� ó���� �Ŀ� Play �� �� ��.
//
//		if (MatineeActor)
//		{
//			// MatineeActor ���� ù ����. ���ʿ� ������ �������� �Ѱ���
//			MatineeActor->FirstSkipFrameNum = MatineeFirstSkipFrames;
//			MatineeActor->bStayAtEnd = bStandbyAtTheEnd; // StandbyAtTheEnd ������ ��Ƽ�� ���� ���¿��� �����ϰ� �� ��. �� ��� ������ �� CustomFinishShow �� ���.
//			MatineeActor->bNoCinematicModeControl = true; // StageEventDirector �� ������ �̻� CinematicMode �� ���⼭ ������.
//			MatineeActor->SetPosition(0.0f, true); // �Ϻ� ��Ȱ���ϴ� ��Ƽ�� ���� ����. MatineeActor ���� bForceStartPos �� üũ�ϸ� �Ǵ� �ű� ��.
//			MatineeActor->InitInterp(); // BeginSingleShow �� ���� �׷� actor �鿡 �����ϱ� ���� Init �� �������� �ѹ� �ؾ� �ϴ� ��.
//
//#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
//			if (bDisableStageEventLights) // ����̽������� ���� �׽�Ʈ �뵵��. �Ƹ��� �ΰ��ӿ��� ���� �����ϰ� ������ ���� ����Ʈ�� ����ϴ� �κ��� ��?
//			{
//				SetAllPointLightGroupAffectWorld(false);
//			}
//			else
//#endif
//			{
//				// ����Ʈ ���ú��� �� ����.. �ٽ� �ʿ���� �� ���� ��.
//				SetAllPointLightGroupAffectWorld(true);
//			}
//
//			SelectFinalDirectorTrack(); // bPCDependentDirectorTrack �̰ų� Random �� ��� ��� ���� Director track ���� ���� �� ������ �ϳ��� ����.
//		}
//	}
//	else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
//	{
//		check(ManualCameraSettings.Num() > 0); // ��� �ϳ��� ������ ó����.
//
//		// �ϴ� ��� deactivate �� �� ����..
//		DeactivateAllManualCams();
//
//		if (LocalB2Player && LocalB2PC)
//		{
//			// ���⵵ ��ǻ� �÷��̾� �ʿ������� ���� �� ���̹Ƿ� Ȯ���� �� ����.
//			LocalB2Player->TopDownCameraComponent->Activate();
//			LocalB2PC->SetViewTargetWithBlend(LocalB2Player, 0.0f);
//		}
//
//		// ù��° ������ ī�޶�� �̵�.		
//		BeginTargetToManualCamera(0);
//	}
//
//	// BeginSingleShow ���� SimpleEventScene ���� �� ���.
//	// -> Director camera component ���� �������� �ʴ� �ɷ� ģ��. �׷��� �����ؾ� �ϴ� ��Ȳ��. ���� ��ġ���� ĳ������ ����.
//	//CachedInitialActiveCamLocation = CurrentActiveCamera->GetComponentLocation();
//	//CachedInitialActiveCamRotation = CurrentActiveCamera->GetComponentRotation();
//	
//	// ���� play actor �� show ����
//	for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
//	{
//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];
//
//		// ControlledMatinee ������ StartDelay ����.
//		if (CurrShowSetting.StartDelay > 0.0f && OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//		{
//			GetWorldTimerManager().SetTimer(CurrShowState.ShowStartDelayTimerHandle,
//				FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::DelayedShowStartTimerCB, EI),
//				CurrShowSetting.StartDelay, false);
//		}
//		else
//		{
//			// �Ｎ���� ����
//			BeginSingleShow(EI);
//		}
//	}
//
//	// ControlledMatinee �� BeginSingleShow ó�� �� Play �ٷ� ����.. �� �ƴϰ� �߰��� �÷��̾� ��ũ�� �˻�.
//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//	{
//		// ��Ÿ�� ��Ȳ�� ���� ������ ���ƾ� �� GroupActor �� ���, ������ �ʴµ��� �ұ��ϰ� �Ҹ��� ���ų� �� �� �����Ƿ� ����.
//		// ���� ��� ������ �������� ���� �÷��̾� Ŭ������ �ٸ��Ƿ� ������ ���ƾ� �� �׷�.
//		RuntimeRemoveNonDesiredGroup();
//		// RuntimeRemoveNonDesiredGroup �ϸ鼭 ������ ���� PointLight ���� ��.. ������ �̷��� ������ �� �����ϴ� �¾��� �־ �ϳ��� ����δ� �ǹ̷�.
//		HackSpareLeastPointLight();
//
//		if (CheckForPCSyncToMatineeStart() == false) // true ��� Player sync �Ŀ� ������ ��.
//		{
//			FinalBeginMatineePlay();
//		}
//
//	}
//
//	// Widget �� �߰����� ó���� �ִµ� �̰͵��� Matinee PCSync ���ÿ��� ���� Matinee play ���� �Ŀ� ó���Ǿ�� ��.
//	if (bCurrentPCSyncToMatineeStart == false)
//	{
//		DeferredBeginShowHandlingForMatineePCSync();
//	}
//
//	if (bForceStopPlayerAttackState)
//	{
//		// ���� �÷��̾� ĳ���Ͱ� ���ӻ��¸� �����ϵ��� ��õ� ���� ���� �ؼ� �ȵ���.
//		if (ShouldMaintainPlayerCharacterState() == false)
//		{
//			if (LocalB2Player)
//				LocalB2Player->SignalStopAttackImmediate(); // ���� AnimBP �� ���� ó���� ��.
//		}
//	}
//
//	SAFE_STAGEMANAGER_CHECK
//	if (bStopAllMobsWhilePlaying && CachedStageManager)
//	{
//		CachedStageManager->StopOrRestoreAllWorldMobs(true); // ���⿡ ���Ե� PlayActor �� �ƴ϶� ��� ����.
//		if (bForceCancelAttackMotionAllMob)
//			CachedStageManager->ForceCancelAttackAllWorldMobs();
//	}
//
//	if (B2GM)
//	{// Auto �� ��� �ߴܽ�Ű�� Finish ���� ����
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
//			// Matinee ��尡 �ƴ� ����� CinematicMode ����. ��Ƽ�Ͽ��� �� Ư�� ����. 4��° ���ڴ� MovementInput �� ���� ������ ����μ��� ��ü �Է¿� ���� ����.
//			LocalB2PC->SetCinematicMode(true, bHiddenPlayer, bHideHud, bDisableInput, true);
//		}
//		if(!bForceNoHidePlayer)
//			SetAllPlayerHiddenInGame(true);
//	}
//
//	// ���� ������ ��� ���⿡ �ɸ� �� �ִ�. Slomo �ɰ� ���縷 ������ ���� ���� �ߵ��� ��
//	// RuntimeRemoveNonDesiredGroup ���Ŀ� ���� ��. �ٸ� ������ disable �� �͵��� �ɷ����� �ؼ�
//	if (B2GM && B2GM->IsInPreRenderPhase() && OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
//	{
//		TemporaryDisableTracksForPreRender(); // �����ϴ� �� FinishShow ���� �ϴ� �� �ƴ϶� GameMode �� PreRender �ʿ��� ó���ؾ�.
//	}
//
//	LevelStreamingOnBegin.ExecuteStreaming(GetWorld(), LevelStreamingOnBeginFlushTH, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::LevelStreamingOnBeginFlushCB));
//	
//	// �÷��� ���� �ε����� ���ŷ �ɸ� �� �ִ� ������ ������ ������ ��½ �ε��� ������ ��.
//	if (IsPossibleAdventureModeBeginEvent())
//	{
//		UB2UIManager::GetInstance()->ReqAsyncLoadAsset(UIFName::BossStatus);
//		UB2UIManager::GetInstance()->ReqAsyncLoadAsset(UIFName::ScenarioDialog);
//	}
//
//	// ��Ÿ �ٸ� �������� ó���� ���� (StageManager)
//	StageEventSceneBeginClass<AB2StageEventDirector*, EStageEvent>::GetInstance().Signal(this, DirectingEvent);
//
//#if BII_STATS
//	FStatisticalPerfStatCollector::bIsCurrentlyInEventScene = true;
//#endif
//
//	if(bCheckPlayOne)
//		bPlayedOnceForThisStage = true; // BeginShow �� �ص� �ѹ� �÷��� �� �ɷ� ģ��.
//	bCurrentlyPlaying = true;
//
//	//
//	// ���� ���� ������ ������, �Ʒ��� ��ŵ�� �ʿ��� ���.
//	//
//
//	bool bShowSkipped = false;
//
//	// ���� ��ŵ�� �Ϸ��� �ּ��� BeginShow �� �� �Ŀ� CustomFinshShow �� �ؾ� ��. ���� ��� ���� �� ��ġ ��ũ�� �ʿ��� ��� ������.
//	// -> �ϴ� �� ��η� ���� �� ���� ���� ��.
//	if (CheckSkipForManageMode(this, ManageMode, false/*bManageModeLeastSkip*/) == EStageEventSkipType::SEST_LeastSkip){
//		CustomFinishShow();
//		bShowSkipped = true;
//	}
//
//#if !UE_BUILD_SHIPPING
//	extern int32 GCheat_SkipEventSceneCount;
//	if (GCheat_SkipEventSceneCount > 0 && !bShowSkipped){
//		if (DirectingEvent != EStageEvent::ESEV_Clear){ // Clear �̺�Ʈ�� ���� ���� �޴��� ���� �ʿ��ϴ� ����.. �׷��� UI �޴��� �ٷ� ������ �� �ִ� ����� ������ ����..
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

	// ��ü �ð��� �����Ǿ��ٸ� ���� Ÿ�̸� on. Controlled Matinee �� ��쵵 ������ �ǰ����� ��Ƽ�� ��ü ���̺��� ��� ������ �ҿ� ���� ��.
	if (IsOverallLengthConstrainted())
	{
		GetWorldTimerManager().SetTimer(OverallLengthTimerHandle, this, &AB2StageEventDirector::OverallLengthTimerCB, SpecifiedOverallLength, false);
	}

	//ABladeIIGameMode* B2GM = GetB2GameMode();
	//if (B2GM && !B2GM->IsInPreRenderPhase()) // PreRender ��Ȳ������ �ſ� ���� TimeDilation �� ���ϰ� PreRender ���� �� ���� TimeDilation �� ������ ��
	//{
	//	ApplyStageEventTimeDilation();
	//}
}

void AB2StageEventDirector::ApplyStageEventTimeDilation()
{
	//float FinalPlayRate = GlobalTimeDilation; // TODO ���⿡ ����ڰ� ������ play rate �� ������ �� ����.
	//UGameplayStatics::SetGlobalTimeDilation(this, FinalPlayRate, true); // ���⼭�� FinalPlayRate �� ��Ÿ �ٸ� ��Ҹ� �������� ���� ���� TimeDilation ���� ���.
}

void AB2StageEventDirector::BeginShowOrBeginCountDown()
{
	//if (bDirectingEventConditionMet == false) // �⺻���� ����
	//{
	//	return;
	//}

	//// [�����÷��̾���] ���� [��Ƽ�Ͼ���] �� ��ȯ�Ǳ� ��, ��ŷ ó���� ��������Ʈ�� �ִٸ�, ��ϵ� ������ �̿��Ͽ� ó��.
	//if ( ForcedMarkPassedWaypoint )
	//{
	//	ForcedMarkPassedWaypoint->ForceMarkByJumpEvent( nullptr, Cast< ABladeIIPlayer >( UGameplayStatics::GetLocalPlayerController( this )->GetPawn() ) );
	//	if ( OverallStartDelay <= 0.0f )
	//	{
	//		OverallStartDelay += 0.1f;
	//	}
	//}

	//// DependentDirectorActor �� ������ ��쿡 ���� �߰� ���� �˻�
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
			return true; // �� �ϳ��� �� �־ ���
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
	//	// ��Ÿ ����. �������� ��� ������ ������ �Ƹ� ���� �� ��.
	//	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (B2Player)
	//	{
	//		B2Player->ForceFeedback();
	//	}

	//	// �����ڸ� ������� �����ǥ ��Ƽ�� ���� ����
	//	SetUpOutSideCharacter = Inflictor;
	//	BeginShow();
	//}
}

void AB2StageEventDirector::NotifySpawnPoolSingleWaveStart(int32 WaveNum)
{
	// ���� ���� ������ ���Ǵ� SpawnPool �� �����ϸ� GameMode ���� �˰� ������ ��. ���⿡ SpawnPool ������ ������ �ʴ� �ɷ� ��.

	//int32 ParsedWaveNum = 0;
	//int32 ParsedMobIndex = 0;
	//ParseForWaveMobSelectKeyword(MobEventMobSelectKeyword, ParsedWaveNum, ParsedMobIndex);
	//
	//if (DirectingEvent == EStageEvent::ESEV_MobAppear && WaveNum == ParsedWaveNum)
	//{
	//	EssentialBeginHandlingCommon();	// �������� ���۰� �Բ��ϴ� ù wave ��� BeginPlay ������ �̰� ����� ��.
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
	//		PossibleEventTriggeringMob = SpawnedMobList[0]; // Ȥ�ö� �߸��� �����̳� �� ����� ����..
	//	}

	//	BII_CHECK(!PossibleEventTriggeringMob || PossibleEventTriggeringMob->IsValidObj());

	//	if (PossibleEventTriggeringMob && CanPlayShow()) // BeginShow ������ CanPlayShow �˻縦 �ϴµ� UIDoc ������ ���⼭ �ؾ� �ؼ� �̸� �˻�.
	//	{
	//		SyncDataToUIDoc(PossibleEventTriggeringMob);
	//		bDirectingEventConditionMet = true;
	//		BeginShowOrBeginCountDown();
	//	}
	//}	
}

void AB2StageEventDirector::NotifySpawnPoolSingleWaveComplete(int32 WaveNum)
{
	//// ���̺� ��ȣ�� ���
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
	// ���� ���� ������ ���Ǵ� SpawnPool �� �����ϸ� GameMode ���� �˰� ������ ��. ���⿡ SpawnPool ������ ������ �ʴ� �ɷ� ��.
	//
	//int32 ParsedWaveNum = 0;
	//int32 ParsedMobIndex = 0;
	//ParseForWaveMobSelectKeyword(MobEventMobSelectKeyword, ParsedWaveNum, ParsedMobIndex);

	//if (DirectingEvent == EStageEvent::ESEV_MobDying && DyingMob && WaveNum == ParsedWaveNum && MobObjIndex == ParsedMobIndex)
	//{		
	//	if (CanPlayShow()) // BeginShow ������ �ϴ� �˻��ε� UIDoc ������ ���⼭ �ؾ� �ؼ� �̸� �˻�.
	//	{
	//		BII_CHECK(!DyingMob || DyingMob->IsValidObj());
	//		SyncDataToUIDoc(DyingMob);
	//		bDirectingEventConditionMet = true;

	//		// BeginShow ó�� ���� BeginSingleShow ���� GetPlayActor �� ���� DyingMob �� �������� ��Ȳ�� ���� ���ε�, 
	//		// ���� BeginShow �� ������Ų�ٰų� �ϸ� WaveMobSelectKeyword �� ������� �Ұ��� �� ��. MonsterSpawnPool �ʿ��� �� Notify ���� DyingMob �� ��Ͽ��� ������ ���̹Ƿ�.
	//		// �׷� �� �ʿ����� ���� ����� �Ѿ���� DyingMob �� WaveNum �� MobObjIndex �� �Բ� ĳ���� �ξ��ٰ� ������ �� �ֵ��� ��..
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
	//	if (CanPlayShow()) // BeginShow ������ �ϴ� �˻��ε� UIDoc ������ ���⼭ �ؾ� �ؼ� �̸� �˻�.
	//	{			
	//		SyncDataToUIDoc(ChangedMob);
	//		bDirectingEventConditionMet = true;
	//		BeginShowOrBeginCountDown();

	//		//ChangedMob�� Stun/Freeze ���� ����
	//		ChangedMob->ClearDebuffs();
	//	}
	//}

	//// ���� ���� PlayActor �� �ϳ��� (Re-spawn �� �����ϴ�) Phase ��ȭ�� ��ġ�� ��Ȳ. ���������� �𸣰����� ���� �� �� �ʿ䰡 �ְڴ�.
	//// �� DestroyedPrevRef �� valid ���� ���� ���̴�. PlayActor �߿� �̰� ��� �ִٸ� ChangedMob ���� �ٲ��ش�.
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
	// CharacterAboutToBeDestroyed �� �ý��ۿ��� ���ŵǱ� ������. Ȥ�ö� ���⿡ ���۷����� �ִٸ� ������.
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
	//	DelayedDyingMobCache.ForgetIfMatch(CharacterAboutToBeDestroyed); // ���ʵ� ��� ���� �� �ִ�.
	//}
}

void AB2StageEventDirector::NotifyStageEventUIOpenComplete(FName InOpenedUI)
{
	if (InOpenedUI != NAME_None && InOpenedUI == MyRelevantUIName) // MyRelevantUIName �� ���� ���ÿ��� ���õ� ��.
	{
		bRelevantUIOpenComplete = true;

		// bRelevantUIOpenComplete �������� ���� ������ ���� ���� �ִٸ� �ٽ� �˻�.
		CheckShowEndCountDown();
		CheckShowEndCountDown_ControlledMatinee(bStandbyAtTheEnd); // bStandbyAtTheEnd �� StandByNoti ��� ���� �� �ۿ�..?
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
		// PlayActorRelativeTransform �� ���� �������� �� �÷��̾��� Relative �ε�, �� �ݴ밡 �ʿ��ϹǷ� inverse
		const FTransform InvRelativeTransform = FTransform(InPlayActorRelativeRotation, InPlayActorRelativeLocation).Inverse();
		const FTransform AdjustTransform = InvRelativeTransform * InPlayActor->GetTransform();

		this->SetActorLocation(AdjustTransform.GetTranslation());
		// ���� Position �� ���󰡰� ���� ���� ����.
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
	//		// �켱, Type �� ���� ĳ���͸� ������
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
	//			// BeginShow ���� LocalPC�� ���ؼ��� �ϴµ� �±�ĳ���ͳ� ��Ʈ�p �÷��̾ �� ���� ������ ���⼭�� �ѹ�.
	//			CastedPlayActorPC->SetupLODInfoForCinematic();
	//		}

	//		if (ThisPlayActor && ThisPlayActor->IsValidObj())
	//		{
	//			// �̰� ���õǸ� Dying �� ���� ���� dead anim �� destroy ó���� ������ ��. �׸��� �׷� ���� �� ������ ���� �Ŀ� ���� ���� �� �� �������� �����ϵ��� �ϰ� �� �� (RestorePlayActorForCtrlMatineeEnd)
	//			ThisPlayActor->SetPlayingDirectedStageEvent(ThisSetting.IsDirectedMode()); 

	//			// Spawn �� �ణ �߰� �ǹǷ� Snap ���� �����ϸ� ���� ������ �� �ִ� ��� ���� �ʿ��ϴٸ� ������ ����..
	//			SnapBladeIICharacterToGround(ThisPlayActor);

	//			// �ɼǿ� ���� �Ϸ� �� ������ �� �ֵ��� ��ġ/ȸ������ ����� �д�.
	//			ThisState.BackupWorldLocation = ThisPlayActor->GetActorLocation();
	//			ThisState.BackupWorldRotation = ThisPlayActor->GetActorRotation();

	//			if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera || OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
	//			{
	//				// ĳ������ Location, Rotation ���� ����
	//				if (ThisSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_BringPlayActor)
	//				{
	//					// EPAP_BringPlayActor �̸� ExportedCamera ��, ManualCamera �� ������ �ʰ� ��ġ ���� ����
	//					const FTransform RelativeTransform(ThisSetting.ManualRelativeRotation, ThisSetting.ManualRelativeLocation);
	//					const FTransform ParentToWorld = this->GetTransform();
	//					const FTransform BroughtTransform = RelativeTransform * ParentToWorld;

	//					ThisPlayActor->SetActorLocation(BroughtTransform.GetTranslation()); // Transform �� ����� �� ����ũ����.
	//					ThisPlayActor->SetActorRotation(BroughtTransform.GetRotation().Rotator());

	//					SnapBladeIICharacterToGround(ThisPlayActor); // ��.. �ᱹ snap �� �� ��� �Ϸ���..
	//				}
	//				else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
	//				{
	//					if (ThisSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_AdjustToPlayActor || ThisSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_ContinuouslyAdjustToPlayActor)
	//					{
	//						// ManualCamera ��忡���� AdjustToPlayActor �� ���� ��ü�� �ƽθ� �÷��̾� �߽����� ��ġ��Ų��. 
	//						// �ٷ� �Ʒ� ExportedCamera ������ AdjustToPlayActor �� CameraAnimApplyCamIndex �� ���� ���Ǵ� ������Ʈ �ϳ��� ����.
	//						// ContinuouslyAdjustToPlayActor ������ Tick ������ �̰� ó��.
	//						AdjustSelfPlacementToPlayActor(ThisPlayActor, ThisSetting.ManualRelativeLocation, ThisSetting.ManualRelativeRotation);
	//					}
	//				}
	//				else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera)
	//				{
	//					// �Ʒ� ����ϴ� MoveKeyFrames ���� ���� Ȯ�� ���ɼ��� ���� KeyFrame ���� ������ �ϴ��� �׳� �ʱ� ��ġ �������� ����.
	//					if (IsExportedEventSceneValid(ThisSetting.ExportedEventScene))
	//					{
	//						if (ThisSetting.PlayActorPlacement != EPlayActorPlacement::EPAP_AdjustToPlayActor && ThisSetting.PlayActorPlacement != EPlayActorPlacement::EPAP_ContinuouslyAdjustToPlayActor)
	//						{
	//							// PlayActor �� ī�޶� �������� ��ġ��Ų��.
	//							FSimpleEventSceneMoveKeyData& FirstKeyData = ThisSetting.ExportedEventScene->SimpleMoveKeyFrames[0];
	//							SetSESPlayActorMovementFromCamera(FirstKeyData, ThisPlayActor);
	//							// ī�޶� �� ���� �� �����Ƿ� �ٽ� Snap.. ��.��
	//							SnapBladeIICharacterToGround(ThisPlayActor);
	//						}

	//						// AdjustToPlayActor �� ��� ī�޶� PlayActor �������� ��ġ��Ű��
	//						// �װ� �ƴϴ��� ���� ���߱� �������� ī�޶� ��ġ ����. �ƴ� ���� �̹� �÷��̾ ī�޶� �߽����� ��ġ���� ��.
	//						SetSESCameraMovementFromPlayActor(ThisSetting.ExportedEventScene->InvSimpleMoveKeyFrames[0], ThisPlayActor);
	//					}
	//				}
	//			}
	//			else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//			{
	//				// ���⼱ ThePlayActor �� �¾��ϴ� ��� ������ ��ġ�ص� ��Ƽ�� �׷� actor �� PlayActor �� ����ϰ� �¾��� ����. ������� ���� ����.
	//				bool bSetupMatineeGroupRes = SetupSingleShowMatineeGroup(ThisSetting, ThisState);
	//				if (ThisSetting.IsDirectedMode() && !bSetupMatineeGroupRes) // InGame ���� ��Ƽ�� Ʈ���� �ʿ���� �Ŷ� SetupSingleShowMatineeGroup �� ������ �� �ִµ�, �׷��ٰ� ���⼭ ����ó���� �� �� ����.
	//				{
	//					return false;
	//				}
	//			}

	//			// Input �� ������ ���¿��� Player Character �� ���� ���¸� �״�� �������� �ִϸ��̼��� �������� �ϰ��� ����
	//			if (bDisableInput == true || (ThisPlayActor != GetB2LocalPlayerChar() && ThisPlayActor != GetB2MatchOpponent()))
	//			{
	//				if (ThisSetting.IsDirectedMode())
	//				{// StageEventShowState �� �����ϴµ� �̰� ���� ExportedCamera �� ManualCamera ���� ����ϴ� ��. AnimMontage �� �÷��� �ϸ鼭 ������ ���� ���ҵ� ������ ������ ���� state ǥ�� �������� ���ŵ� �� ����.
	//					EStageEventShowState FinalSES =
	//						(ThisSetting.StageEventStateOverride == EStageEventShowState::ESES_None || OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee) ?
	//						GetDefaultDesiredPlayActorSESState(DirectingEvent, ThisSetting.PlayActorType) : ThisSetting.StageEventStateOverride;
	//					ThisPlayActor->SetStageEventShowState(FinalSES, this);
	//				}
	//				if (ThisSetting.PlayActorStateMode != EPlayActorStateMode::EPASM_MaintainInGame)
	//				{
	//					ThisPlayActor->PreventMovementByStageEvent(); // ���� ����, Ȥ�� ���� ���߿� spawn �Ǿ����� �÷��̾� �Ѿƿ��� �ʰ�. PreventMovement �ϸ� ���ݵ� ����. ����

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
	//	// ��Ƽ�� ��忡�� AdjustToPlayActor �� ��� �����ϵ��� �����ǥ ����� �� Ȱ��. ���� ��Ƽ�Ͽ��� ã�� ���� Ʈ���̶� �������� ���� ���� �ֵ��� ��. �׷� ���� ��ü�� PlayActorStateMode �� InGame ���� ���� ��.
	//	if (InSettingForThis.IsOneOfAdjustToPlayActorPlacement())
	//	{
	//		MatineeActor->SetDynamicRelativeMoveTrackBaseActor(ThisPlayActor,
	//			FVector(0.0f, 0.0f, -1.0f * ZOffsetFromMatchingSKActorToBIICharacter(ThisPlayActor)), // ���⵵ ������ ����Ʈ���� SkeletalMeshActor �� �������� �ΰ��ӿ��� BladeIICharacter �̴� ������ ������.
	//			(InSettingForThis.PlayActorPlacement == EPlayActorPlacement::EPAP_AdjustToPlayActorLocationOnly)); // LocationOnly �ɼ� �ι�° ���ڷ�..
	//	}

	//	// ���� �� �÷��� ��Ȳ�� ���� �������� InterpGroup �̸�.
	//	const FName FinalMatchingGroupName(*GetFinalMatchingInterpGroupName(ThisPlayActor, InSettingForThis.MatchingInterpGroupName.ToString(), false));

	//	ASkeletalMeshActor* FoundGroupSKActor = Cast<ASkeletalMeshActor>(GetGroupActorOfGroupName(FinalMatchingGroupName));

	//	if (FoundGroupSKActor)
	//	{
	//		// ��Ƽ�Ͽ� ���� ���̿��͵��� ��Į�ȸ԰� ���ش�.
	//		UPrimitiveComponent* pPrimitiveComponent = Cast<UPrimitiveComponent>(FoundGroupSKActor->GetRootComponent());
	//		pPrimitiveComponent->bReceivesDecals = false;

	//		InStateForThis.CtrlMatineePuppet = FoundGroupSKActor;

	//		// CtrlMatineePuppet �� PlayActor �� ���������� ����
	//		// ��ű��� �߰��ǰų� �ؼ� �߰��� �ܰ��� ������ �ʿ䰡 ���� ������ �Ʒ� SetupControlledMatineePuppet �ʿ� �۾��� �� ��.
	//		ThisPlayActor->SetupControlledMatineePuppet(InStateForThis.CtrlMatineePuppet, 
	//			// bHidePlayerPointLight �� ���� puppet �ʿ� �������� PointLight �� �¾��ϰ� ��. �ɼǿ� ���� ���� �÷��̾� ĳ���ͷ� �����Ǵµ� ����� dynamic point light ���� ������ �ֱ� ������ �����ϴ� ��.
	//			(!bHidePlayerPointLight && 					
	//				(InStateForThis.IsForLocalPlayerCharacter() || (bAllowOtherPlayerPointLight && Cast<ABladeIIPlayer>(InStateForThis.GetPlayActor(this))))
	//				)
	//		); 

	//		if (InSettingForThis.ActorScaleOverride > 0.0f)
	//		{ // SetupControlledMatineePuppet ���� ThisPlayActor scale �� �������� �ǵ� ���� ���� �����ϰ� ���� ���. �ϴ� uniform scale �δٰ�.
	//			InStateForThis.CtrlMatineePuppet->GetSkeletalMeshComponent()->SetWorldScale3D(FVector(InSettingForThis.ActorScaleOverride, InSettingForThis.ActorScaleOverride, InSettingForThis.ActorScaleOverride));
	//		}

	//		// SetupControlledMatineePuppet ���� �� component ���� visibility ���¸� üũ�� �� �����Ƿ� SetVisibility �� SetActorHiddenInGame ���� �� ���Ŀ� ó��.

	//		if (InSettingForThis.IsDirectedMode()) // InGameStateMode �� ��Ƽ�� anim track ������� �ʴ� ��. �ٵ� �׷� ��쿡 ��Ƽ�� track �� �غ��� �ʿ䰡 �ֳ�..?
	//		{
	//			// �⺻������ ���ܳ��Ҵٰ� �����ϰ�.. ��� �� ���ٰ� ���� �� �ٽ� ���ܳ���. �̰ɷ� ����� �� �ƴ� Visibility Ʈ������ �ؾ� �� ��.
	//			SetVisibilityOfMatineePuppet(InStateForThis.CtrlMatineePuppet, InStateForThis.GetPlayActor(this), true);

	//			// ��Ȥ ���� �̵��� ���� �ִϸ��̼��� ��� AlwaysTickPoseAndRefreshBones �� �ʿ��� ��찡 �ִ�. ���� ���� ���� �Ŀ� �� ������Ʈ �� ���� �ɷ� ����
	//			InStateForThis.CtrlMatineePuppet->SetActorTickEnabled(true); // �⺻ ƽ�� ���⼭ Ȱ��ȭ��.
	//			InStateForThis.CtrlMatineePuppet->GetSkeletalMeshComponent()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	//			// ThePlayActor �� ���ܳ��� ���� �⺻�� ����ε�, PCSync ���� ������ ���� ���� ���Ŀ� ó��
	//			//TryHidePlayActorForMatineeInternal(ThisPlayActor, true);
	//			if (ThisPlayActor->bNeedSpawnCheck)
	//			{// NPCClassInfo �� bUsePreSpawnState �� ���� ������ �Ǵµ�, ���� ����� ��ģ�ٴ� �� ��ü�� PreSpawnState �� ������ ������ ������ �� �ְ�, ������ ���� ���Ŀ� spawn �� �ٽ� �Ǿ������ ������ ��. ���� false �� ��������.
	//				ThisPlayActor->bNeedSpawnCheck = false; 
	//				// �׸��� MeshComponentUpdateFlag �� ������ �־�� ������ ���°� �ٲ��.. ���� ������ ���� �ǵ���.
	//				ThisPlayActor->TemporarySetMeshComponentUpdateFlag(EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones);
	//			}
	//		}

	//		// ����� �ΰ����� ���� ����..? ö���ϰ� off ��Ű��. �̰� ���� ��ġ�ܰ迡�� ���� �ؾ� �Ǵ� ������ �������� �ʴ´�.
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
		InPuppet->GetSkeletalMeshComponent()->SetVisibility(true, true); // ���� �������� ������ ���ܳ��Ҵٰ� ����� ���� ������ ��
		InPuppet->SetActorHiddenInGame(false); // �̰� �ٽ��ε� �̰͸� ������ �� ���� ���� ���� �־ �Լ� ���� �� ���� ��..

		ActivateMatineePuppetPSCs(InPuppet, InRelevantPlayActor); // ���� ���� ��� �Ϸ��� ���� ����.
	}
	else
	{
		InPuppet->SetActorHiddenInGame(true);
	}
}

void AB2StageEventDirector::ActivateMatineePuppetPSCs(ASkeletalMeshActor* InPuppet, ABladeIICharacter* InRelevantPlayActor)
{
	// ���� Owner Actor �� �����ִ� ���ȿ� Particle System Component ���� Deactivate �� �Ǵ� ���ؼ� ���� Activate ��Ű�� ���� ��������.
	// ���� ��ƿ�� �������� �ƶ��� ����� EnsureActivateStaticallyAttachedPSCs �� ������..
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
	//			{ // HUD ����� ����� FloorRing �� ���ϰ� ������ �ʰ�..
	//				// ���� �������� ���⿣ ���ͼ� �ȵȴ�. 
	//				// FloorRingPS �� EventPuppetSetupBlacklist �� �߰��ؼ� �ƿ� Puppet �� ������ �ʰ� �Ͽ���.
	//				// �� �ڵ�� ���� ���� ���� �ڵ���.
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
	//		// ������ �̸��� ��ġ�ϴ� �׷��� ã�� �Ŀ��� �׷� �ν��Ͻ���..
	//		UInterpGroupInst* FoundGI = NULL;
	//		for (int32 GII = 0; GII < MatineeActor->GroupInst.Num(); ++GII)
	//		{
	//			if (MatineeActor->GroupInst[GII]->Group == CurrGroup) // �ᱹ �ε����� ��������?
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

	//UInterpGroupDirector* TheDirGroup = MatineeActor->MatineeData->FindDirectorGroup(); // Director Group �� Ư���ϴ� �ٷ� ���� �� �ֱ� ��
	//if (TheDirGroup == NULL)
	//{
	//	return;
	//}

	//if (TheDirGroup->InterpTracks.Num() <= 1 || (bPCDependentDirectorTrack == false && !IsValidRandomMatineeGroupNumSet()))
	//{
	//	// �� ���, �� ó�� �ɷδٰ� Enable �� ���� ���� �ְ�����, Director Track �� �ƽθ� ���� �ʴ� ��쵵 ������ �� �� �ְ����� �׳� ����.
	//	return;
	//}

	//// Ư�� ShowSetting �� ���ε� �� PlayActor �� �ƴ� ���� �ֿ������� ������ Player Character �� ������.
	//ABladeIIPlayer* PossibleHighlightedPlayer = GetShowHeroPlayerIfAny();
	//if (bPCDependentDirectorTrack && !PossibleHighlightedPlayer)
	//{ // ��ü�� �� event �� �̷� ���ݿ� ���� �ʴ� ����� ��. ���� �ƴ� ���� �ִµ� �׷� ��찡 ����� GetShowHeroPlayerIfAny �� ����.
	//	return;
	//}
	//BII_CHECK(!PossibleHighlightedPlayer || PossibleHighlightedPlayer->IsValidObj());

	//FString DesiredPostFix;
	//if (bPCDependentDirectorTrack && PossibleHighlightedPlayer)
	//{ // FinalMatchingInterpGroupName �̶�� �ǹ̿ʹ� �ణ �ٸ��� �������� �Ծ��� ������ postfix �� ���������� ��
	//	DesiredPostFix = GetFinalMatchingInterpGroupNamePC(TEXT(""), PossibleHighlightedPlayer->GetTrimmedNameFromPCClassEnum(), EPCInterpGroupNetType::PCIGNT_Local, false);
	//}
	//else if (IsValidRandomMatineeGroupNumSet())
	//{// FinalMatchingInterpGroupName �̶�� �ǹ̿ʹ� �ణ �ٸ��� �������� �Ծ��� ������ postfix �� ���������� ��
	//	DesiredPostFix = GetFinalMatchingInterpGroupNameRandom(TEXT(""), RandomlySetMatineeGroupNum, false);
	//}
	//
	//// ���� PlayerCharacter �� �´� postfix �� ���� ������ ����.
	//for (int32 DI = 0; DI < TheDirGroup->InterpTracks.Num(); ++DI)
	//{
	//	UInterpTrackDirector* CurrDirTrack = Cast<UInterpTrackDirector>(TheDirGroup->InterpTracks[DI]);
	//	if (CurrDirTrack == NULL)
	//	{
	//		continue;
	//	}

	//	if (CurrDirTrack->TrackTitle.EndsWith(DesiredPostFix)) // Postfix �� ���߸� ��. ���� prefix ���� �� ����.
	//	{
	//		CurrDirTrack->EnableTrack(true);
	//	}
	//	else
	//	{
	//		CurrDirTrack->EnableTrack(false); // ��Ÿ �������� ����. ������ DirectorTrack �� �ϳ��ۿ� �� ��.
	//	}
	//}

	// Ȥ�ö� ������ Enable �� ���� �����ٸ� �� ù��° ������.. �� ���� �ְ����� �׳� ����.
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

	//		// ���� ShowSetting �� ����� Matinee InterpGroup Ȥ�� Folder �̸�
	//		const FName FinalMatchingGroupName(*GetFinalMatchingInterpGroupName(CurrShowState.GetPlayActor(this), CurrShowSetting.MatchingInterpGroupName.ToString(), false));
	//		const FName FinalMatchingGroupNameForFolder(*GetFinalMatchingInterpGroupName(CurrShowState.GetPlayActor(this), CurrShowSetting.MatchingInterpGroupName.ToString(), true));

	//		if (CurrGroup->GroupName == FinalMatchingGroupName || 
	//			CurrGroup->GroupName == FinalMatchingGroupNameForFolder)
	//		{
	//			bFoundMatchingOne = true; // �ٷ� �´� �� �����Ƿ� �̰� �����.
	//			break;
	//		}
	//	}

	//	if (bFoundMatchingOne){
	//		continue;
	//	}

	//	bool bNotDesiredGroup = false;

	//	// ����� ������ ShowSetting �� ���� �׷� �̸��� ���� �ʾҰų�, ���� ���� ��Ȳ�� ���� �ʰų� �� ��� ��. 
	//	// ������ ���̹� �Ծ࿡ ���� Ȯ���� ������ ���ƾ� �ϴ� ������� �ѹ� �� üũ�ؼ� ��Ȱ��ȭ �ܰ�� (RuntimeDisableInterpGroup)
	//	// �׷� �̸��� ������ ���̹� ��Ģ�� �ƿ� ������ ���ٸ� ���⼭ ��Ȱ��ȭ �� ����� �ƴϴ�.

	//	for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
	//	{
	//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
	//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];

	//		// ���⼭ ThePlayActor �� �˻��ϴ� �� ���� �� ������ BladeIIPlayer ������ ���� ����. ������ �˻��ϴ� ThePlayActor �� ������ �׷��̶�� �����ؼ� �ȵ�.
	//		if (Cast<ABladeIIPlayer>(CurrShowState.GetPlayActor(this))) 
	//		{
	//			for (int32 PCI = 0; PCI < AllTrimmedPCClassEnumName.Num(); ++PCI)
	//			{
	//				for (int32 NTI = 0; NTI < (int32)EPCInterpGroupNetType::PCIGNT_End; ++NTI)
	//				{
	//					// �� ShowSetting �� PCClass �� ���� �Ծ࿡ ���� Group Ȥ�� ���� �̸�
	//					// Iteration ���߿� ������ ���� �̸��� ����������.. �ű⿡ ��Ī�Ǵ� InterpGroup �� �̹� ����� ���� �ʰ� ����� �� �͵��� ��Ȱ��ȭ �ĺ�.
	//					const FName PossiblePCGroupName(*GetFinalMatchingInterpGroupNamePC(CurrShowSetting.MatchingInterpGroupName.ToString(), AllTrimmedPCClassEnumName[PCI], static_cast<EPCInterpGroupNetType>(NTI), false));
	//					const FName PossiblePCGroupNameForFolder(*GetFinalMatchingInterpGroupNamePC(CurrShowSetting.MatchingInterpGroupName.ToString(), AllTrimmedPCClassEnumName[PCI], static_cast<EPCInterpGroupNetType>(NTI), true));

	//					// ���� ShowSetting ������ MatchingInterpGroupName �� ���̽��� �ϰ� ������ ���� ���ӿ��� ���õǾ� ������ �ʴ�.
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
	//		{ // ���� �׷� ������ �ִ� ���, ���õ��� ���� ���� �׷��� ����. �̰� Player �� �ƴϰ� �� ����.
	//			for (int32 RNI = 1; RNI <= RandomMatineeGroupNum; ++RNI)
	//			{ 
	//				if (RNI == RandomlySetMatineeGroupNum){
	//					continue; // �̹� ���� ���õ� ����. ������ �Ʒ����� bNotDesiredGroup ������ ���� �ʰ����� �̸� �ɷ���.
	//				}
	//				// �� ���� ���� ���� �����ߴ� ġ�� �װ͸� ó������. ���� �̰Ͱ� ������ ���� �׷�鵵 ���̹��� �����ֱ� �ؾ� ��.
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

	//	// ��Ÿ �ٸ� ��� �߰��Ǹ�..

	//	if (bNotDesiredGroup)
	//	{// �ش� �׷��� ��Ȱ��ȭ
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
//		// �׷�� ��ġ�ϴ� �׷� �ν��Ͻ��� ã�´�.
//		for (int32 GII = 0; GII < OwnerMatinee->GroupInst.Num(); ++GII)
//		{
//			// Disable �ϰ��� �ϴ� ���� �ڱ� �ڽ��̵��� �ƴϸ� �ڽ��� �����ϴ� ���� �׷��̵���
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
//				// �̰� �� �����̱� �ѵ�.. ������ �� ���� �� ShowSetting �� ��ϵ��� ���� �ֵ鵵 visibility �� ���ַ��� ���� Matinee InitInterp �� SkeletalMeshActor ���� ���ִ� �ڵ尡 �߰��Ǿ���.. �ٵ� ���⼭ ������ٰ� ���ֵǴ� �ֵ��� �ٽ� ���־�� ��
//				ASkeletalMeshActor* ThisGroupSKActor = Cast<ASkeletalMeshActor>(ThisGI->GetGroupActor());
//				if (ThisGroupSKActor)
//				{
//					ThisGroupSKActor->SetActorHiddenInGame(true);
//				}
//				// PC ���� ����Ʈ Ʈ���� ����� ���� ��쵵 ���� ó���� �ʿ�. SetAllPointLightGroupAffectWorld �δ� ó�� ����.
//				ALight* ThisGroupLightActor = Cast<ALight>(ThisGI->GetGroupActor());
//				UPointLightComponent* PLComp = ThisGroupLightActor ? Cast<UPointLightComponent>(ThisGroupLightActor->GetLightComponent()) : nullptr;
//				if (PLComp
//#if 0
//					// �ǵ����� ���� �߸��� ��� �Ͽ��� �¾� �Ǿ��� �������� �޲ٱ� ���� �־��� �ӽ� �÷���..
//					// �¾� ������ �Ϸ�Ǹ� bHackMyPerPCClassLightingSetupDone �� üũ�ϴ� ���̾���.
//					// �¾� �Ϸ�Ǿ��ٰ� �Ͽ� DEPRECATED ��ũ�ϰ� ����� ����.. Ȥ �𸣴� ������ ���������� ����.
//					&& bHackMyPerPCClassLightingSetupDone_DEPRECATED 
//#endif				
//					)
//				{
//					PLComp->bAffectsWorld = false;
//					PLComp->MarkRenderStateDirty();
//				}
//
//				// ������ GroupActor �� �����ϰ� �߾��µ� �� ���� Ʈ�� Disable �� ���� �ʳ� �ʹ�. �ϴ� ������ ž �ε����� ������� �Ϸ��� GroupActor �� �����ؼ� �ȵ�.
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
//		// ���� Folder ���鼭��������.. �� �ڵ尡 �ȸ��� ���� �ִ�. �ٽ� �̰ɷε�..
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
//{ // �� �����θ� MatineeActor ������ �Űܳ���.
//	if (OwnerMatinee)
//	{
//		return OwnerMatinee->FindParentGroupFolder(ChildGroup);
//	}
//	return NULL;
//}

FString AB2StageEventDirector::GetFinalMatchingInterpGroupName(class ABladeIICharacter* InPlayActor, FString RawInterpGroupName, bool bForFolderGroup)
{
//	ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(InPlayActor);
//	if (CastedPlayer) // ��Ÿ NPC ���� ��� InterpGroup �̸��� �����ؼ� �˻��� ��찡 �߻��ұ�?
//	{
//		EPCInterpGroupNetType NetType = EPCInterpGroupNetType::PCIGNT_Local;
//
//		AB2NetGameMode* NetGM = Cast<AB2NetGameMode>(GetB2GameMode());
//		if (NetGM && DirectingEvent == EStageEvent::ESEV_Begin) // ����μ� NetGameMode �� ���̵尡 ������ �Ǵ� ���� ���ۻ�.
//		{
//			ABladeIINetPlayer* CastedNetPlayer = Cast<ABladeIINetPlayer>(CastedPlayer);
//
//			NetType = (
//				((NetGM->GetPeerType() != PEER_RESTRICTED && CastedNetPlayer->GetNetStatus() == ABladeIINetPlayer::NET_MASTER) || (NetGM->GetPeerType() == PEER_RESTRICTED && CastedNetPlayer->GetNetStatus() == ABladeIINetPlayer::NET_SLAVE)) ?
//				EPCInterpGroupNetType::PCIGNT_SuperPeer : EPCInterpGroupNetType::PCIGNT_CommonPeer
//			);
//		}
//
//		// PC �� Ŭ���� ����, Net ���� peer Ÿ�� � ���� �޶���.
//		return GetFinalMatchingInterpGroupNamePC(RawInterpGroupName, CastedPlayer->GetTrimmedNameFromPCClassEnum(), NetType, bForFolderGroup);
//	}
//	else if (IsValidRandomMatineeGroupNumSet()) 
//	{// ���� flow �δ� Player �� Ʈ���� Random selection �� �� �� ��.
//		// Player �׷��� �̹� ������ suffix �� �ٴµ��ٰ� ���� �䱸���׵� ���� ������ ������ �����ϰ� ���ڴ� ����.
//		return GetFinalMatchingInterpGroupNameRandom(RawInterpGroupName, RandomlySetMatineeGroupNum, bForFolderGroup);
//	}

	return RawInterpGroupName;
}

FString AB2StageEventDirector::GetFinalMatchingInterpGroupNamePC(FString RawInterpGroupName, FString TrimmedPCClassEnumName, EPCInterpGroupNetType NetType, bool bForFolderGroup)
{
	// �꿡 ���� �׷� �̸��� �ֵ��� �ؾ�.. ���� ��� "MainPCGroup_Gladiator", "SubPCGroup_Assassin" �ϴ� ���� �ǰڴ�.
	FString RetName = RawInterpGroupName + TEXT("_") + TrimmedPCClassEnumName;

	//// �� ���� �ϴ� PVP������ ���°ɷ� �Ѵ�. (���Ŀ� �̰� �ɼ����� ������ ����)
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
{ // ���ͱ׷쵵 �̰� �� ����.
	FString RandPostFix = FString::Printf(INTERPGROUP_SUFFIX_RAND, RandNum);
	// Folder �̸��� ��� �׳� ������ prefix �� postfix �� �ٿ��� ���.
	return bForFolderGroup ? (FOLDERGROUP_PREFIX + RandPostFix) : (RawInterpGroupName + TEXT("_") + RandPostFix);
}

#if WITH_EDITOR
bool AB2StageEventDirector::IsPossibleInterpGroupNameForPC(const FString& InCheckName) const
{ // GetFinalMatchingInterpGroupNamePC �� ���� �� �ִ� �̸����� üũ.. true �����̶�� �ص� �ƴ� ���� ������ false �����̸� Ȯ���� �ƴ� ��.

	TArray<FString> AllPCClassTrimmedName;
	ABladeIIPlayer::GetAllTrimmedNameFromPCClassEnum(AllPCClassTrimmedName);

	bool bHasAtLeastOnePCName = false;
	for (const FString& ThisPCName : AllPCClassTrimmedName)
	{
		if (InCheckName.EndsWith(ThisPCName)) // GetFinalMatchingInterpGroupNamePC ������ ����.. PCClass �̸��� ���� ����.
		{
			bHasAtLeastOnePCName = true;
			break;
		}
	}

	if (!bHasAtLeastOnePCName)
	{
		return false;
	}

	// ShowSettings �� MatchingInterpGroupName ���� ����� ������..

	return true;
}
#endif

void AB2StageEventDirector::SetAllPointLightGroupAffectWorld(bool bAffectWorld)
{
	//if (!MatineeActor)
	//	return;
	//
	//const int32 AllowedMatineeTrackPointLightNum = GetAllowedMatineeTrackPointLightNum();

	//// ����� ������ ���� ���� ����ؾ� �ϴµ�.. ���⼭ ���� ����.
	//// �ֳ��ϸ� Group ���� ���������� ����ؾ� �ϴ� ��찡 �ֱ� ������
	//// ���⼭�� �ϰ������� On/Off �ϰ� ���� �׷캰 �ʿ���� �� ���� �� RuntimeDisableInterpGroup ����
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
	//		else { // ���� �Ѿ�� ��� ��.. �ϰ� �;��µ� �׷캰�� ������ ���� �� �־ �Ұ���. �Ѥ�
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
	// ����� �������� dynamic point light ���� ������ ���߱� ���� ������ Light On/Offf ��Ʈ���� �̽��ϴ� ������
	// �߸� �����Ǿ� ���� �Ϻ� ���� �¾��� �޲ٱ� ����..
	// RuntimeDisableInterpGroup ���� ��Ȱ��ȭ ���Ѿ� �� Ʈ���� point light �� ���� �Ǵµ� �̰� ������� ���� �¾����� ���� point light �� �ϳ��� ������ �ʴ� ��Ȳ�̶�� �ϳ��� �츮����..
	
//	if (bHidePlayerPointLight && MatineeActor) // bHidePlayerPointLight �� �ƴ϶�� �÷��̾� ĳ���� ����Ʈ�� ������ �־��� �״� �ϳ��� ���� ��.
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
//						bAtLeastOneTurnedOn = true; // �ϳ��� ��� ������ �� �� �ʿ� ����.
//						break;
//					}
//					if (!TheFirstEncounteredPLComp) { // �ƹ��� �� ��� �ִ� ��츦 ���� ó���� ĳ���� �д�.
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
//#if !UE_BUILD_SHIPPING // ���� �ϳ� �ѱ� ������ ��� ���.
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
		OutGroupNames.Add(InSetting.MatchingInterpGroupName); // PC �� �ƴ϶�� MatchingInterpGroupName ���� �ٷ�.

		if (RandomMatineeGroupNum > 1) // ���� �׷� �̸��� �߰��� �ִ� �ɷ�. �Ƹ��� �̰� �ִٸ� ���� �̸��� �� �� �� ������.
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
	//	MatineeActor->AddCustomNotifyActor(this); // Stop notification �� �ޱ� ���� ���. Play ������ ������ TermInterp ���� ����.
	//	if (!GIsEditor) 
	//	{ // �� SetCloseBattleMainUIClass �̺�Ʈ�� �����Ϳ��� Init-Destroy �ÿ� Subscribe/Unsubcribe �ϴµ� �����Ϳ��� Unsubscribe �� ������ ����. �Ф�
	//		// �׸��� �Ƹ� �����Ϳ��� �ʿ������� ���� ��.
	//		SetCloseBattleMainUIClass<bool>::GetInstance().Signal(false);
	//	}
	//	EnterCinematicModeForControlledMatinee(); // CinematicMode �����ؾ� ���������� �÷��̾� ĳ���� visibility �� ���õ� ��.
	//}

	//// SetVisibilityOfMatineePuppet �� ���ؼ� ȣ���Կ��� �ұ��ϰ� ���⼭�� �� �ʿ��ѵ�.. �׸��� �̰ɷε� �ȸ��� ���� ����. ���� �̹��� Ÿ�̹� ������ �ִ� �� �ѵ�..
	//for (int32 EI = 0; EI < ShowStates.Num(); ++EI)
	//{
	//	ActivateMatineePuppetPSCs(ShowStates[EI].CtrlMatineePuppet, ShowStates[EI].GetPlayActor(this));
	//}

	//// LocalPlayerSwitching �� ���⼭ �Ѵ�. PCSyncToMatineeStart �� �ִ��� �� �������� ��Ƽ�� �÷��̰� ����� ���̹Ƿ�. (���� �� Ʃ�丮�󿡼� ���)
	//// ActivateMatineePuppetPSCs ������ ���ϴ� �� �Ʒ� Switching �� ��ġ�鼭 PlayActor �� ���ư��� ��찡 �־ Hide ���ۿ� �Ϻ� ������ �߻��ϹǷ�..
	//if (bLocalPlayerSwitching) {
	//	SwitchingLastRenderingPlayer();
	//}
}

void AB2StageEventDirector::SwitchingLastRenderingPlayer()
{
	///* ----------------------------------------------------
	// !! ���� !!
	// 
	// ���� ��ɿ� ���� flow �� ���ӵǴ� ��ʶ� �Ϲ� ���� ���������� ��� ���� ��ŵ ���ο� ���� �ΰ��� flow �� �ٲ�� ���� �߻� ������ ����.
	// �̹� �׷� ���� ����� ������ �ʰ� �� ����� ����ϴ� ���� ���� �� ���� flow �� ���⿡ ���ӵǴ� �� ����̱⵵ �ѵ�..
	//
	// ������ Ʃ�丮�� ���� ��忡�� ����� ���� �ʵ��� �ϰ�,
	// �Ϲ� ���� ������������ ��� �ʿ伺�� ����� ��� ������ ManageMode �� SupportedStages ���� � ���� ��������
	// Ư�� EStageEventSkipType::SEST_LeastSkip �� ���Ǿ�� �Ѵٸ� LeastSkip �ÿ� �� ������ ���߸��� �ʵ��� �� �ʿ䰡 ����.
	//---------------------------------------------------- */
	//bool bisInTutorialGameMode = (Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this)) != nullptr);
	//checkSlow(bisInTutorialGameMode);
	//if (!bisInTutorialGameMode) {
	//	return;
	//}
	//////////////////////////////////////////////////////////

	//TArray<FPlayActorShowRuntimeState*> AllStatesOfPlayerChar;
	//GetShowStatesOfPlayerChar(AllStatesOfPlayerChar); // �÷��̾� ĳ���͵��� ShowState �� �����ؼ� ������.

	//checkSlow(ShowStates.Num() >= 2);
	//if (ShowStates.Num() < 2) {
	//	return; // �� �̻��̾�� �ٲٵ��� ����..
	//}

	//// AllStatesOfPlayerChar �ȿ����� �ε���
	//int32 CurrentLocalPlayerIndex = INDEX_NONE; // ��ü �� State �ε���
	//int32 NewLocalPlayerIndex = INDEX_NONE; // ��ü �� State �ε���

	//for (int32 SI = 0; SI < AllStatesOfPlayerChar.Num(); ++SI)
	//{
	//	FPlayActorShowRuntimeState* ThisState = AllStatesOfPlayerChar[SI];
	//	// IsForLocalPlayerCharacter �� �ش��ϴ� �� �� �ϳ��� �־�� �Ѵ�
	//	if (ThisState->IsForLocalPlayerCharacter())
	//	{
	//		CurrentLocalPlayerIndex = SI;
	//		break;
	//	}
	//}
	//// ��ü�� �÷��̾� ĳ���ʹ� �ں��� �ؼ� ���� Local �� �ƴ� �ɷ�. ���� 3 �̻��� Ʈ���� �ִٸ� ���� �ε����� �켱 ����.
	//// �������� ���� ������� �����ϰ��� �Ѵٸ� ShowSetting �� switching ���θ� ǥ���ϴ��� �ؾ߰����� �׷� �ʿ���� �ִ� ����� �� ������ �ʴ�.
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
	//	return; // �̷� ��� ���ó� �� �̻� ���� �Ұ�
	//}

	//FPlayActorShowRuntimeState* ShowStateToSwitch = AllStatesOfPlayerChar[NewLocalPlayerIndex];
	//FPlayActorShowRuntimeState* ShowStateOfOldLocalPlayer = AllStatesOfPlayerChar[CurrentLocalPlayerIndex];
	//// PlayActorAboutTobeSwitched �� ��ü�� Dummy �� spawn �� ���Ͱ� �� ���̴�.
	//ABladeIIPlayer* PlayActorAboutTobeSwitched = Cast<ABladeIIPlayer>(ShowStateToSwitch->GetPlayActor(this));
	//// �� ������ ���� ���� �÷��̾� ĳ��..
	//ABladeIIPlayer* CurrentLocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//UB2SkillRTManager* SkillRTManager = CurrentLocalB2Player ? CurrentLocalB2Player->GetSkillRTManager() : nullptr;
	//if (PlayActorAboutTobeSwitched && CurrentLocalB2Player && SkillRTManager)
	//{
	//	// ���̶� ĳ���� Class �� ���õǾ� �ִ� ��..
	//	const EPCClass NewPCClassEnum = PlayActorAboutTobeSwitched->GetCurrentPlayerClass();
	//	const EPCClass OldPCClassEnum = CurrentLocalB2Player->GetCurrentPlayerClass();
	//	if (SkillRTManager->LocalPlayerSwitching(NewPCClassEnum))
	//	{
	//		// ĳ���� �ٲٸ鼭 TutorialGameMode�� MarkBeginAsyncLoadNextPCClass�� �̺�Ʈ ����
	//		MarkBeginAsyncLoadClass<>::GetInstance().Signal();
	//		// ���� spawn ����.. ���� ���� ShowState �ٲ�ġ��.
	//		ABladeIIPlayer* NewLocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//		ShowStateToSwitch->SetPlayActor(NewLocalB2Player);
	//		// �ٸ� visibility ������ �� ���� ���Ŀ� �ؾ� �ؼ�.. ��¿ �� ���� ��� spawn �� ĳ���� Hidden ó��.
	//		NewLocalB2Player->SetHideFromEventDirector(true);
	//		
	//		// ForgetACharacter �� ��츦 ����. ������ �������� ����Ǿ� �ִ� PlayActor �� ���ŵǴ� ��Ȳ�̶� null-out �ϴ� �� ������.
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
	// SetupSingleShowMatineeGroup �� �ٸ� �������� ���������� ���� ������ �������� �ٽ� �� �־�� �� ���� ����..

	// Controlled Matinee ����� PlayActor �� ������ �ʾƾ� �� ��찡 ��������
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
	//// ���� �������ҷ��� �ѹ��� Ÿ���Ѵ�.
	//if (bEnterCinematicMode)
	//	return;

	//bEnterCinematicMode = true;

	//ABladeIIPlayerController* LocalB2PC = GetB2LocalPlayerCon();
	//if (LocalB2PC && OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//{
	//	bool bHiddenPlayer = LocalB2PC->GetPawn() && LocalB2PC->GetPawn()->bHidden;
	//	
	//	// MatineeActor �ʿ��� bNoCinematicModeControl �� ������ ���� CinematicMode ��Ʈ���� ���� �Ѵ�.
	//	LocalB2PC->SetCinematicMode(true, bHiddenPlayer, bHideHud, bDisableInput, true);
	//}
	//if(!bForceNoHidePlayer)
	//	SetAllPlayerHiddenInGame(true);
}

// bTryHidePlayer�� ������ ���� ����
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

	//		// CurrentPlayer�� MainPlayer�� SubPlayer�� ���� �÷����ϰ� �ִ� ĳ���Ϳ� ��ġ�ϸ� ��ũ ���
	//		if (CurrShowState.IsForLocalPlayerCharacter())
	//		{
	//			// ���� ���ÿ����� move Ʈ���� ã�Ƽ� ù Ű�������� ��ǥ ��������
	//			
	//			const FName FinalMatchingGroupName(*GetFinalMatchingInterpGroupName(LocalB2Player, CurrShowSetting.MatchingInterpGroupName.ToString(), false));
	//			UInterpGroupInst* CurrGroupInst = MatineeActor->FindFirstGroupInstByName(FinalMatchingGroupName.ToString()); // ��� �̸����� �ٷ� ã�� �� �ִ� ��.
	//			
	//			if (CurrGroupInst && CurrGroupInst->Group)
	//			{
	//				for (int32 TI = 0; TI < CurrGroupInst->Group->InterpTracks.Num(); ++TI)
	//				{
	//					UInterpTrackMove* MoveTrack = Cast<UInterpTrackMove>(CurrGroupInst->Group->InterpTracks[TI]);
	//					
	//					if (MoveTrack && CurrGroupInst->TrackInst.Num() > TI)
	//					{
	//						// UInterpGroup::UpdateGroup ���� InterpTracks �� TrackInst �ε��� ��ġ���Ѽ� ���.

	//						// ù Ű ������ �� ����.
	//						float KeyTime;
	//						FVector FirstKeyPosition;
	//						MoveTrack->ExternalGetKeyframePosition(CurrGroupInst->TrackInst[TI], 0, KeyTime, FirstKeyPosition, NULL, NULL);
	//						// ȸ������ �⺻ ������δ� �������� ��..
	//						//FVector FirstKeyRotEuler;
	//						//MoveTrack->ExternalGetKeyframeRotation(CurrGroupInst->TrackInst[TI], 0, KeyTime, FirstKeyRotEuler, NULL, NULL);

	//						// NavigationSystem ������ �̵�. ������ PlayerController �ε� �۵��ϴ±� ��
	//						UNavigationSystem::SimpleMoveToLocation(LocalB2Player->Controller, FirstKeyPosition);
	//						// PathFind ���� �ݹ� ���.
	//						UPathFollowingComponent* PFComp = LocalB2Player->Controller->FindComponentByClass<UPathFollowingComponent>();
	//						if (PFComp)
	//						{
	//							// SimpleMoveToLocation ���� �ѹ� üũ�ϴµ� �� ������ �ݹ��� �ȺҸ���. ��ϵ� ���� ���°� �ٸ� ������ �ְ�. ������ �̹� ���������� ���� Sync �� �ʿ䰡 ����.
	//							if (PFComp->HasReached(FirstKeyPosition, EPathFollowingReachMode::OverlapAgent) == false)
	//							{
	//								PFComp->OnRequestFinished.AddUObject(this, &AB2StageEventDirector::OnPCSyncToMatineeStartCompleted);
	//								// Sync �� ǥ��. �̰� ������ ���� ��Ƽ�� �÷��� ����.
	//								bCurrentPCSyncToMatineeStart = true;
	//							}
	//						}

	//						// ����������.. Ư�� �ð��� ������ ������ �����ϵ��� �Ѵ�. �ʹ� �ָ��� ���� �Ǹ� ���߿� ���������, ��򰡿� ������ �� �̻� ������ ���� �ʴ� �ͺ��� ������. 
	//						// �ʿ信 ���� ������ ����..
	//						const float PCSyncFallBackTimerTime = 10.0f; 
	//						GetWorldTimerManager().SetTimer(PCSyncToMatineeStartFallbackTimerHandle, this, &AB2StageEventDirector::PCSyncToMatineeStartFallbackTimerCB, PCSyncFallBackTimerTime, false);

	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//// ���� �÷����ϱ� ������ �ΰ��� ���·� �ǵ��� ���´�. SetupSingleShowMatineeGroup ���� �� �Ϻθ� �ǵ��� ����.
	//if (bCurrentPCSyncToMatineeStart)
	//{
	//	for (int32 EI = 0; EI < ShowSettings.Num(); ++EI)
	//	{
	//		FPlayActorShowSettings& CurrShowSetting = ShowSettings[EI];
	//		// �� ��� SkeletalMeshActor ���� ó������ ������ �״� ���ܼ��� �ȵǰ���.
	//		const bool bVisibleFromFirstPlace = (CurrShowSetting.PlayActorStateMode == EPlayActorStateMode::EPASM_DirectedFromBeginning);

	//		FPlayActorShowRuntimeState& CurrShowState = ShowStates[EI];
	//		ABladeIICharacter* ThisPlayActor = CurrShowState.GetPlayActor(this);
	//		if (ThisPlayActor)
	//		{
	//			// SetupSingleShowMatineeGroup ���� ���ܳ��� �͵��� �ٽ� ��� ���̵���. 

	//			bool bOneOfTempSpawnedPuppet = false;
	//			for (int32 TPI = 0; TPI < TempSpawnedPuppets.Num(); ++TPI)
	//			{
	//				if (ThisPlayActor == TempSpawnedPuppets[TPI])
	//				{
	//					bOneOfTempSpawnedPuppet = true;
	//				}
	//			}

	//			// TempSpawnedPuppet �� ������ �����ָ� �ȵ�. ���� SetStageEventShowState �� SetupSingleShowMatineeGroup ���� ������ �״�� ���־� ������ ä�� spawn ��� ���� �� �÷��� ���� ���� ��.
	//			if (bOneOfTempSpawnedPuppet || bVisibleFromFirstPlace)
	//			{
	//				ThisPlayActor->SetHideFromEventDirector(true);
	//			}
	//			else
	//			{
	//				ThisPlayActor->SetHideFromEventDirector(false); // �������� �ΰ��� ���´��..

	//				// BeginSingleShow ���� �̰� �⺻���� �� �־ �ִϸ��̼��� ������ �ʴµ� ��Ƽ�� ��忡�� StageEventShowState ������ �߿��� �� �ƴ� ��
	//				ThisPlayActor->SetStageEventShowState(EStageEventShowState::ESES_None, NULL);
	//			}
	//		}
	//		if (CurrShowState.CtrlMatineePuppet)
	//		{
	//			SetVisibilityOfMatineePuppet(CurrShowState.CtrlMatineePuppet, CurrShowState.GetPlayActor(this), bVisibleFromFirstPlace);
	//		}
	//	}

	//	// �÷��̾� ĳ���ʹ� ���̴��� FinalBeginMatineePlay �ϱ� ���� ���⼭�� CinematicMode ������ �ʿ�.
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

		// CheckForPCSyncToMatineeStart ���� ������ �� �ٽ� ��Ƽ�� �÷��̸� ���� �������´�. �ٽ� SetupSingleShowMatineeGroup ���� �� ���� ���·�
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
				// ���� �߿��� �� �ƴѵ�.. ESES_None ���� ���� StageEventShowState �� ���� �ǹ��ִ� ������ �ٽ� ������ ����. ��� ��Ƽ�� ��忡�� ū �ǹ̴� ����..
				ThisPlayActor->SetStageEventShowState(GetDefaultDesiredPlayActorSESState(DirectingEvent, CurrShowSetting.PlayActorType), this);
				
				// PlayActor �� ����� �� �Ʒ� FinalBeginMatineePlay ����
			}
			if (CurrShowState.CtrlMatineePuppet)
			{
				SetVisibilityOfMatineePuppet(CurrShowState.CtrlMatineePuppet, CurrShowState.GetPlayActor(this), true);
			}
		}

		FinalBeginMatineePlay(); // ����

		DeferredBeginShowHandlingForMatineePCSync(); // BeginShow ���� ó�� ���ߴ� �͵� ���� ó��.
	}
}

void AB2StageEventDirector::OnPCSyncToMatineeStartCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	BeginMatineeFromPCSyncToMatineeStart();
}

void AB2StageEventDirector::PCSyncToMatineeStartFallbackTimerCB()
{
	GetWorldTimerManager().ClearTimer(PCSyncToMatineeStartFallbackTimerHandle);
	BeginMatineeFromPCSyncToMatineeStart(); // ��ü�� �̹� BeginMatineeFromPCSyncToMatineeStart �� ó���Ǿ��� ���ε� �׷� ���� �ƹ� �� ���Ͼ ��.
}

ABladeIICharacter* AB2StageEventDirector::GetRelevantPlayActor(EStageEventPlayActor InType, FString& WaveMobSelectKeyword, int32 PlayerIndex)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::GetRelevantPlayActor"));

	//ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();

	//if (InType == EStageEventPlayActor::ESEE_CurrentPlayer)
	//{
	//	if (LocalB2Player)
	//	{ // �װ��ִ� �Ŷ�� ���� �Ŀ� sync �� �� �ʿ䵵 ���ڰ�.. ���ϰ� ������ �� ������ puppet ����?
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
	//		// ���� �÷����ϰ� �ִ� ĳ���Ͷ�� ���� spawn �ϰų� �� �ʿ���� �״�� �����ͼ� ���.
	//		// �׷����� �װ��ִ� �Ŷ�� ���� �Ŀ� sync �� �� �ʿ䵵 ���ڰ�.. ���ϰ� ������ �� ������ puppet ����?
	//		return LocalB2Player->IsAlive() ? LocalB2Player : LocalB2Player->SpawnMyselfAsPuppet();
	//	}
	//	else
	//	{
	//		// �ƴϸ� ������� player controller �� possess ���� ���� ä�� spawn �ؼ� ���.
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
	//else if (InType == EStageEventPlayActor::ESEE_RemoteOpponent_Current) // RemoteOpponent_Other �� �ʿ������� �߰�. ESEE_WinningPlayer_Other �����ؼ�
	//{
	//	return GetB2MatchOpponent();
	//}
	//else if (InType == EStageEventPlayActor::ESEE_TeamAllyPlayer || InType == EStageEventPlayActor::ESEE_TeamEnemyPlayer)
	//{
	//	return GetTeamPlayer(InType == EStageEventPlayActor::ESEE_TeamAllyPlayer, PlayerIndex, false, true); // ���� ��쿡 �̰� ����Ϸ��� live �� �������� �ƹ��� ���� ��. ���� �ִ� ������� ����ó�� ��Ȱ���Ѽ� �����´�.
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
	//	//	ParseForWaveMobSelectKeyword(WaveMobSelectKeyword, WaveNum, MobIndex); // ���ڿ��� ������ �ɼǿ��� ��ȣ�� ����.

	//	//	TArray<ABladeIICharacter*> InterestedWaveMobs;
	//	//	B2GM->GetSingleWaveMobList(WaveNum, InterestedWaveMobs, false); // �׾�� �༮�� �����ؼ� ������. ���� �װ� ������ Ÿ�̹��̾�� ��. �ƴ� ��� �Ʒ� DelayedDyingMobCache ������ �ֱ� �ϴ�.

	//	//	for (int32 WMI = 0; WMI < InterestedWaveMobs.Num(); ++WMI)
	//	//	{
	//	//		ABladeIICharacter* CurrMob = InterestedWaveMobs[WMI];
	//	//		if (CurrMob)
	//	//		{
	//	//			if (CurrMob->GetSpawnPoolWaveObjIndex() == MobIndex) // ��ǻ� Wave �� ���۵� ��Ȳ�� �ƴϸ� ã�� �ε����� mob �� ���� ���� �ִ�.
	//	//			{
	//	//				return CurrMob;
	//	//			}
	//	//		}
	//	//	}
	//	//	// ������ ������ ���� �ʾҴٸ� MobDying ���� delay �� �� ����� �� �ִ�. ��� dying mob �� ������ �� �ƴ����� Ʈ���Ÿ� ������ dying mob ������ �Ʒ�ó�� ������ �� ����..
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
//	// ���� ���� ĳ���ͺ��� üũ. ������ (PendingKill �� �ƴ�) ���� ĳ���Ϳ� ��ġ�Ѵٸ� �װ� ������ ����.
//	ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
//	if (LocalB2Player && LocalB2Player->IsValidObj() && LocalB2Player->GetCurrentPlayerClass() == PCClassEnum) {
//		return LocalB2Player;
//	}
//
//	// Ʃ�丮�� ���Ӹ���� �̸� �� �������� �����س��� FallbackPuppet ���� ���
//	AB2TutorialGameMode* TutorialGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(this));
//	if (TutorialGM)
//	{
//		return TutorialGM->GetFallbackPuppetPlayer(PCClassEnum);
//	}
//#if WITH_EDITOR
//	// �����Ͷ�� ���� TutorialGameMode �������� ���� ���¿��� ��� ������ �ʿ��ϹǷ�..
//	// �̰� �Ϲ����� ���Ӹ�� �������� Ȯ���ϴ� �� ����ϴ�. 
//	// ��� ��ü�� �����ϰ����� �� ��������� ���� �̷� ��ɶ����� FallbackPuppet ���� ������ ���ƾ� �ϴ� �� ����.
//	else if (GIsEditor)
//	{
//		//
//		// ���߿� �ӽ��ڵ��̹Ƿ� ũ�� �Ű澲�� �ʰ� ���⿡ �׳� �� ¥�� ��.
//		//
//
//		ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//		if (B2GM)
//		{
//			TSubclassOf<APawn> DefaultPawnClassBackup = B2GM->DefaultPawnClass;
//			// ���̵� spawn ���Ѿ� �ؼ� ��� �� Ŭ������ �ٲ����..
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
		// �� �� spawn animation �� ������ �� �ʿ䰡 ���� �� ����.. ��� �̰� �Ҹ��� �� ���� ��Ȳ�� �ƴ� ����.
		ABladeIIPlayer* NewPuppet = LocalB2Player->SpawnOtherAsPuppet(true,
			false // �±� ĳ���� �� puppet �� point light �� ������ �Ѵٸ� �߰� ���ڸ�..
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
	//// ����� PVP ��� opponent ��
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

		// TODO : �ٸ� ī�޶� set. �ʿ��Ϸ��� �𸣰�����..
	}

	return NULL;
}

void AB2StageEventDirector::CheckShowEndCountDown()
{
	if (IsOverallLengthConstrainted()) // OverallLengthConstrainted ��� ���� ��ġ�� Ÿ�̸ӿ� ���� ����� ��.
	{
		return;
	}

	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee) // Controlled Matinee �� �ٸ� ��θ� ���� ���� üũ
	{
		return;
	}

	// ��ü show �� ����Ǵ� ���ǵ��� �˻��Ѵ�.

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
		// FinishCheck �� �ʿ���� ������ ����.
		if (DoesShowRequireFinishCheck(SI) == true && ShowStates[SI].bShowFinished == false)
		{
			return;
		}
	}

	// ���������� bStandbyAtTheEnd �� üũ.. �� ���� ���� CustomFinishShow �� �ؾ�
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
		CustomFinishShow(); // ������� �Դٸ� ��. FinishShow �� �ƴ� CustomFinishShow �� ������.
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
			CurrSetting.IsDirectedMode() && // InGame State �״�θ� ���� �ð� ���� �ٸ� ������ ���� �ʴ� �̻� ���� üũ�� �� ���� ����.
			PlayActor && PlayActor->GetStageEventShowState() != EStageEventShowState::ESES_None &&
			PlayActor->GetStageEventShowState() != EStageEventShowState::ESES_Extra // Extra �� �Ƹ��� SESEndNotify ���� �ܼ��� Idle �� ������ �� ��..
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
//	if (!bCurrentlyPlaying){ // �ߺ�ó���� �� �� ����.
//		return;
//	}
//
//	// CustomFinishShow ���� ��Ȳ�̸� ��ó ó���� �ȵ� ��Ȳ�� �� �־ Ÿ�̸ӵ��� �ٽ� �ѹ� Ȯ���� Ŭ���� �Ѵ�.
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
//		WorldTimerManager.ClearTimer(PCSyncToMatineeStartFallbackTimerHandle); // ��� ���� ���� ����ϰ� ���� ���� Ÿ�̸Ӷ� ������ �̷��� ���־�� ��
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
//	//if (IsUsingMobileRendering(GetWorld())) // ĸ�� ���� ���� �� ����Ʈ ���� �������� �̷� üũ�� �ʿ��� �� ������ StageEventDirector �� �ΰ��� ����̶� �̷� üũ�� �ʿ��ұ� �ͱ⵵.
//	//{
//	//	// ����� ���������� dynamic point light ���� ������ ����. ���� ���� ����� �� ���� ���� �����ϸ鼭 Ȯ���� ���δ� ��. �ʿ��� �� ���� ���� �� �� �ٽ� ���� ��.
//	//	if (B2GM)
//	//		B2GM->DeactivateWorldMovablePointLight();
//	//}
//
//	ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
//	ABladeIIPlayerController* LocalB2PC = GetB2LocalPlayerCon();
//
//	// bStandbyPause �� ���� pause �� ���¸� ������
//	if (bStandbyAtTheEnd && bStandbyPause && UGameplayStatics::IsGamePaused(this))
//	{
//		UGameplayStatics::SetGamePaused(this, false);
//	}
//		
//	// ����Ƽ�꿡���� ���� �̷��� �̺�Ʈ��..
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
//			// ���� ��ġ�鼭 ThisState �� PlayActor �� ���ŵ� �� �ִ�.
//			if(ThisSetting.PlayActorType != EStageEventPlayActor::ESEE_SetUpOutside)
//				RestorePlayActorForCtrlMatineeEnd(ThisSetting, ThisState);
//		}
//						
//		ABladeIICharacter* ThisPlayActor = ThisState.GetPlayActor(this);
//		if (ThisPlayActor)
//		{
//			ThisPlayActor->AllowMovementByStageEvent(); // PreventMovement �ϴ� ���̽��� �ִµ� �� ��� ����.
//
//			if (ThisSetting.EndActionPlayActorAdjustType == EEndActionPlayActorAdjustType::EAPAAT_BackToOriginal)
//			{
//				// PlayActor �� ��ġ �� ȸ������ ������Ŵ.
//				ThisPlayActor->SetActorLocation(ThisState.BackupWorldLocation);
//				ThisPlayActor->SetActorRotation(ThisState.BackupWorldRotation);
//			}
//
//			// CustomFinish Ȥ�� Extra �� ��� NotifyStageEventShowEnd �� ������� �ʾ��� ��. 
//			// ���������� ���� notify ���� ���¸� ����. bStandbyAtTheEnd �� ��쵵 �ٽ� Extra �� �ǵ����� �ʿ�..
//			ThisPlayActor->SetStageEventShowState(EStageEventShowState::ESES_None, NULL);
//			ThisPlayActor->SetPlayingDirectedStageEvent(false);
//		}
//
//		ThisState.bShowFinished = true; // Ȯ���� ��ũ
//		ThisState.SetPlayActor(NULL); // ���� ��
//	}
//	ShowStates.Empty();
//	DelayedDyingMobCache.ForgetIt();
//
//	// ����� ���� ������ �־�� ��.
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
//	FinishEventWidget(); // EventWidgetEndTime ������ ��� �̹� ����Ǿ��� �� �ִ�.
//
//	if (bStopAllMobsWhilePlaying && CachedStageManager)
//	{ // BeginShow ���� Stop ���״� �͵� �ٽ� �ǻ츲.
//		CachedStageManager->StopOrRestoreAllWorldMobs(false);
//	}
//
//	if (LocalB2PC)
//	{
//		// CinematicMode ����. �Ʒ� NotifyShowOverFromDependentDirectorActor ���Ŀ� ���� �����
//		LocalB2PC->SetCinematicMode(false, false, true, true, true);
//	}
//	// ����� ���̾� ���̰�
//	if(!bForceNoHidePlayer)
//		SetAllPlayerHiddenInGame(false);
//
//	// ���� UI ������ �ִϸ��̼��� �÷����� �ʿ䰡 �� �־ �̷� ������ ��.
//	if (IsPossibleAdventureModeBeginEvent())
//	{
//		// todo : ��� �ϵ��ڵ� �κ� �ٲ��ʿ䰡 ������ =��= (��� ���·�)
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
//		// todo : ��� �ϵ��ڵ� �κ� �ٲ��ʿ䰡 ������ =��= (��� ���·�)
//
//		// pvpUI
//		UB2UIPVP1on1BattleMain* PVPBattleMainUI = UB2UIManager::GetInstance()->GetUI<UB2UIPVP1on1BattleMain>(UIFName::PVPOneOnOneBattleMain);
//		if (PVPBattleMainUI)
//		{
//			PVPBattleMainUI->OnPVPBeginEventSceneEnd();
//		}
//
//		// ������UI - �̰͵� ������ UIName �� �����.
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
//	// ������ �ٸ��� ���� ���� ������ �������� �����ش� ġ��.
//	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
//
//	// �÷��̾� TopDown ī�޶� ������ ActiveCamera ��..
//	ReturnToGamePlayCamera();
//
//	LevelStreamingOnEnd.ExecuteStreaming(GetWorld(), LevelStreamingOnEndFlushTH, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::LevelStreamingOnEndFlushCB));
//
//	// Auto ���� ����
//	if (ShouldStopAutoDuringShow() && B2GM && B2GM->IsLocalPlayerAutoBattle()) // ���� ������ ������ Auto �� �ƴϾ��� �ϴ��� �߰��� � �����ε� �ٲ�� Auto �� �������ش�.
//	{
//		B2GM->RestoreLocalPlayerAuto();
//	}
//
//#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
//	if (bPlayedAtLeastOnce == false)
//	{			
//		bPlayedAtLeastOnce = true; // �̹� ������������ �ּ��� �ѹ� �÷��� �Ͽ���.
//		GConfig->SetBool(*GetPlayedAtLeastOnceSectionString(), *GetPlayedAtLeastOnceKeyString(CurrentStageId), bPlayedAtLeastOnce, GB2LocalCutSceneSavedStateIni);
//		GConfig->Flush(false, GB2LocalCutSceneSavedStateIni);
//	}
//#endif
//
//	if (LocalB2Player) 
//	{ // ��Ÿ ���..
//		LocalB2Player->SetupLODInfoForInGame(); // ���� �÷��̾�� ���� ���� ���� ������� ������ CinematicLOD �����ϴ� �ǵ���.
//		LocalB2Player->SetInvincibleByStageEvent(false);
//	}
//	B2Scalability::ApplyStageEventSpecificScalabilitySetting(this, false); // ���� �÷��̾� LOD ���ð� ���������� ���� �� Ư�� scalability ��Ʈ�� ���� ����
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
	checkSlow(!bCurrentlyPlaying); // Ȯ���� ���� �÷��� ���� �Ŀ� �۵� ������ �͵�..

	//// BP ���� Ư���� �̺�Ʈ�� �޾Ƽ� ó���ϰ��� �Ѵٸ�..
	//if (ShowOverEventName != NAME_None)
	//{
	//	GEngine->HandleCeCommand(GetWorld(), *(ShowOverEventName.ToString()), *GLog);
	//}

	//if (IsPossibleAdventureModeBeginEvent())
	//{ // BeginShow ������ Async ��û�ϴ� �� �ִµ� �̰͵� ���� ���� �� hitch �� �����ϴ� ������ ������..

	//	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
	//	// ���⼭�� Async �ε� ��û�� ������ �ʿ䰡 �ִµ� ���⼭ ��û�� �ϸ� �ΰ��� ������ ���ư��� ���߿� �ε��� ���ư� ���̴�.
	//	// �̶� �װͱ��� ������ �� �߰� �� ������ ���� ���� ���̴� �׳� ���ش�. 
	//	// �̷� Ÿ�ֿ̹� async �ε� ��û�� �� �߰��Ǹ� ����� ���̴�.
	//	UB2UIManager::GetInstance()->ReqAsyncLoadAsset(UIFName::MobAppearBossStage);
	//}

	//ABladeIIGameMode* B2GM = GetB2GameMode();
	//if (B2GM)
	//{		
	//	if (DirectingEvent == EStageEvent::ESEV_Begin && 
	//		(B2GM->GetB2GameModeType() == EB2GameMode::PVP_Tag || B2GM->GetB2GameModeType() == EB2GameMode::PVP_Team || B2GM->GetB2GameModeType() == EB2GameMode::Guild))
	//	{// MatchGameStartEventSceneEnd �ñ׳� ���� ��� ���Ӹ��� ���..
	//		MatchGameStartEventSceneEndClass<>::GetInstance().Signal();
	//	}

	//	B2GM->MarkAnyStageEventPlayedOnce(); // ��·�� �ּ� �ϳ� �÷��� ��. ��

	//	// ���� ������ ��ٸ��� �ִ� �ٸ� DirectorActor �� �ִٸ� Notify �� ������. �ٸ� ������ �ٷ� ������ �� �� �����Ƿ� ������ ����ó���� ��ø���� �ʵ��� ���� ����.
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
	//	// SetupSingleShowMatineeGroup, EnsureHidePlayActorsForControlledMatinee ��� ���ܳ��� �� ����.
	//	if (ThisPlayActor->bIsDying && InSettingForThis.IsDirectedMode())
	//	{ // ��, ��Ƽ�Ϸ� �״� ������ ������ �ִ� �Ⱥ��̰� �� ó���� �־�� �Ѵ�.
	//		
	//		// �� Destroy �� ������ ������ �����ѵ� �̰� ���Ŀ� SpawnPool ���� ó���ؾ� �ϴ� ��Ȳ�� ���� �� �־.. �� ������. ���ʿ� Destroy ��� �⺻���� ��Ʈ �̿ܿ��� �����ؾ߰ڴ�.
	//		
	//		ThisPlayActor->SetHideFromEventDirector(true);
	//		if (ThisPlayActor->GetMesh()){				
	//			ThisPlayActor->GetMesh()->GlobalAnimRateScale = 0.0f; // �̷��� �ϸ� �ִϸ��̼��� ������ �ߴܽ�ų �� �ִ�.
	//			// �ٵ� �̷��� �ϸ� OnAnimMobDeadEnd �� ���� �ʰ� �Ǿ� Destroy �� �ȵȴ�. �Ƹ� �̰� ������ ��Ȳ�� �������� ���� ��Ȳ�� �Ŷ� ��� �ϵ� ū ������ ���� �� ������.. �ٷ� Destroy �� ���� �ʴ� �ɷ�..
	//			ThisPlayActor->SetLifeSpan(1.0f);
	//		}

	//		// �ٸ� ������δ� LobbySkeletalMeshActor �ʿ� �� �� ó�� AnimNotify �� �����ϴ� ����� ����. ���� ������ �ذ�å�� �� �ǵ� ������ �� �� �ֱ� �ؾ�..
	//					
	//		InStateForThis.SetPlayActor(NULL);
	//	}
	//	else
	//	{
	//		ABladeIIPlayer* CastedPlayerPlayActor = Cast<ABladeIIPlayer>(ThisPlayActor);
	//		if (CastedPlayerPlayActor) 
	//		{
	//			if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
	//			{ // ���� ���� ���� ����ϴ� ��忡���� ���� ������ �ٽñ� ���� ���յ� �������� �������´�.
	//				CastedPlayerPlayActor->SetUseSectionMergedMesh(true);
	//			}

	//			CastedPlayerPlayActor->SetupLODInfoForInGame(); // ���� �÷��̾ �ƴ϶� ��Ʈ��ũ �÷��̾ �±� �÷��̾ ������ �� LODInfo ����
	//		}

	//		ThisPlayActor->SetHideFromEventDirector(false);
	//		ThisPlayActor->RestoreTemporaryMeshComponentUpdateFlag(); // NeedSpawnCheck �����ϸ鼭 TickPose �����ߴ� �� ����. ���ɻ�.
	//	}

	//	if (InSettingForThis.EndActionPlayActorAdjustType == EEndActionPlayActorAdjustType::EAPAAT_AdjustToPuppet && InStateForThis.CtrlMatineePuppet) // bPutPlayActorBackToOriginalLocation �� �ƴ϶� ���� Matinee ���� ��ġ�� ��ũ
	//	{
	//		const float B2CharHeightOffset = ZOffsetFromMatchingSKActorToBIICharacter(ThisPlayActor);

	//		ThisPlayActor->SetActorLocation(InStateForThis.CtrlMatineePuppet->GetActorLocation() + FVector(0.0f, 0.0f, B2CharHeightOffset));
	//		if (Cast<ABladeIIPlayer>(ThisPlayActor))
	//		{ // �÷��̾� ĳ���Ͷ�� ȸ������ yaw �� �����´�.
	//			ThisPlayActor->SetActorRotation(FRotator(0.0f, InStateForThis.CtrlMatineePuppet->GetActorRotation().Yaw, 0.0f));
	//		}
	//		else
	//		{ // �Ϲ� ���̶�� roll �̳� pitch �� ���⿡�� ���������� non-zero ���� �� ���� ��������?
	//			ThisPlayActor->SetActorRotation(InStateForThis.CtrlMatineePuppet->GetActorRotation());
	//		}
	//	}
	//}

	//// Matinee �� ���� �ʿ� ��ġ�� �ֵ�μ� �������� �ʰ� ��. ���Ŵ� ���� ����.
	//// ���� ����� �����Ϸ��� Matinee �ʿ��� �ּ��� TermInterp �� ó���� �����̾�� �Ѵ�. �⺻���� ��뿡�� AMatineeActor::Stop ���� ���������� notify �� �� �Ŀ� TermInterp �� ó���ǹǷ� �����ϰ� �ʹ��� �� ���������� �ȵ�
	//// -> MatineePuppet (�� �׷����) ���� ��Ȱ�� �ϱ� ���� �������� �ʴ´�. ��Ȱ���� �����ϰ� �ϱ� ���� ó���� ABladeIICharacter::DoesMatineePuppetSetupFor** ����.
	//if (InStateForThis.CtrlMatineePuppet)
	//{
	//	if (InSettingForThis.PlayActorStateMode != EPlayActorStateMode::EPASM_DirectedAfterFinish)
	//	{
	//		if (ThisPlayActor)
	//		{ // ����Ʈ ���ְ� ��Ÿ PlayActor �� ���� Setup �ߴ� �� clear			
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

	CheckShowEndCountDown(); // ��ü���� ���� ���� �˻�.
}

void AB2StageEventDirector::OnControlledMatineeStop(bool bStandByNoti)
{
	bControlledMatineeFinished = true;
	CheckShowEndCountDown_ControlledMatinee(bStandByNoti); // �߰� �˻��� ������ ����.
	if (!GIsEditor)
	{ // �� SetCloseBattleMainUIClass �̺�Ʈ�� �����Ϳ��� Init-Destroy �ÿ� Subscribe/Unsubcribe �ϴµ� �����Ϳ��� Unsubscribe �� ������ ����. �Ф�
		SetCloseBattleMainUIClass<bool>::GetInstance().Signal(true);
	}
}

void AB2StageEventDirector::OnExportedCameraAnimStop()
{
	bCameraAnimFinished = true;
	CheckShowEndCountDown(); // ��ü���� ���� ���� �˻�.
}

void AB2StageEventDirector::OnAllManualCameraOver()
{
	bAllManualCameraEnded = true;
	CheckShowEndCountDown(); // ��ü���� ���� ���� �˻�.
}

void AB2StageEventDirector::ReturnToGamePlayCamera(bool bDeactivateOtherCams /*= false*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::ReturnToGamePlayCamera"));

	//ABladeIIPlayer* LocalB2Player = GetB2LocalPlayerChar();
	//ABladeIIPlayerController* LocalB2PC = GetB2LocalPlayerCon();

	//// ���⼭ �÷��̾� ī�޶� ������ blending out �� ����� ���̷��� DeactivateAllManualCams �� �ؼ� �ȵ�. 
	//// �׷��� ���ʿ��� active �� camera component �� ��� AActor::CalcCamera ���� �ܼ��� ���� ��ġ�� ����ϰ� �� ��.
	//if (bDeactivateOtherCams)
	//{
	//	DeactivateAllManualCams();
	//}

	//CurrentActiveCamera = NULL;

	//// ��Ƽ�� ��� ���� ī�޶� ���� ������ �ٸ� �� ����... ��?

	//if (LocalB2Player)
	//	LocalB2Player->TopDownCameraComponent->Activate();

	//if (ActiveCameraForceOnFinish)
	//{// ���� ���� �ƴϾ ����. �̷� ����� �ʿ��ϰ� �� ����..
	//	// �׷��ٰ� �ؼ� ���� �ȿ� �ٸ� ī�޶� �ִµ� ActiveCameraForceOnFinish �� �����ϱ� ���� ������ �ƴϴ�. ��ü�� �������� �����ϱ� ����Ҹ��� ��쿡 Ȯ���ϰ� �����ϱ� ���� ����.
	//	ActiveCameraForceOnFinish->ForceBeginActiveCamera(FinalCameraRestoreBlendTime); 
	//}
	//else
	//{
	//	// ActiveCameraForceOnFinish ���ٴ� ����� ���� �� ��ü�� �� �ٶ����ϰ� �ڿ�(?)������.
	//	// ���⼭�� FinalCameraRestoreBlendTime �� �������� ApplyActiveCameraIfAny �� blend time �������̵� �����ϵ��� ��. ���⿡�� ActiveCamera �� ���ư� ���� ������ ������ ��������.
	//	ApplyNormallyExpectedInGameCamera(GetB2GameMode(), LocalB2PC, FinalCameraRestoreBlendTime);
	//}

	//if (LocalB2PC && LocalB2PC->PlayerCameraManager)
	//{
	//	//������ ī�޶� ������Ʈ -> ��� APlayerCameraManager::SetViewTarget()�� ȣ���ϰ� �Ǹ�,
	//	//APlayerCameraManager::TransformComponent�� AttachedChild ��ġ�� �ȸ´� ������ �־(ī�޶���ġ�� TransformComponent ��ġ�� Ʋ����.) APlayerCameraManager::SetViewTarget()���� �Ϸ��� �ߴµ�
	//	//� ���̵�����Ʈ�� ������ ���� �̰��� ����ִ´�.
	//	LocalB2PC->PlayerCameraManager->UpdateCamera(0.f);
	//}
}

void AB2StageEventDirector::DeactivateAllManualCams()
{
	for (int32 CI = 0; CI < MAX_DIRECTOR_CAMERA; ++CI)
	{
		DirectorCamArrayRef[CI]->Deactivate();
	}

	// TODO �ٸ� �����ʿ� ��ġ�� ī�޶�鵵 -> ���� �� ����.
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

	//	if (NextCamera) // ���� NextCamera �� ã�� ���ϸ� ���ᰡ �Ǳ� �������� �����°� ������ ��.
	//	{
	//		if (CurrentActiveCamera && CurrentActiveCamera != LocalB2Player->TopDownCameraComponent)
	//		{
	//			// �̰� Deactivate �ؾ� �ϴ� ��Ȳ�� ���� actor ���� �ٸ� ī�޶� ������Ʈ�� ���� ���� �ϴ� �����.
	//			// ���� ���� ������ ���ǿ� CurrentActiveCamera �� �÷��̾��� TopDownCamera ��� deactivate �� ���� �ʴ´�.
	//			// �ٸ� ��찡 �� �ִٸ� �߰� �˻縦 �ϴ���, outer �˻縦 �ϴ��� �ϴ� ������..
	//			CurrentActiveCamera->Deactivate();
	//		}
	//		CurrentActiveCamera = NextCamera;
	//		CurrentActiveCamera->Activate();
	//		LocalB2PC->SetViewTargetWithBlend(this, CurrCamSetting.BlendTimeFromPrev, CurrCamSetting.BlendFunction);

	//		// BlendTime �� ���缭 ManualCameraBlendCB Ÿ�̸� ��ġ.
	//		// SetViewTargetWithBlend �� ���� �Ϸ� �ݹ��� �ִ��� �𸣰ڴµ�.. �ð� ���缭 Ÿ�̸� ������. (SetViewTargetWithBlend �� ���� �ݹ��� �ִ� �Ŷ�� ���� ����) 
	//		if (CurrCamSetting.BlendTimeFromPrev > 0.0f)
	//		{
	//			GetWorldTimerManager().SetTimer(ManualCameraBlendTimerHandle, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::ManualCameraBlendCB, CamSettingIndex), CurrCamSetting.BlendTimeFromPrev, false);
	//		}
	//		else
	//		{
	//			ManualCameraBlendCB(CamSettingIndex); // ������ �̷��� ���� ������ �ʾ�����..
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
	//	// bStandbyAtTheEnd ������� ��ȣ�� ������. CheckShowEndCountDown ���� bStandbyAtTheEnd �� ���� ó���� �� ��.
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
	CustomFinishShow(); // ���� ����.
}

void AB2StageEventDirector::ManualCameraBlendCB(int32 CamSettingIndex)
{
	GetWorldTimerManager().ClearTimer(ManualCameraBlendTimerHandle);

	if (CamSettingIndex >= 0 && CamSettingIndex < ManualCameraSettings.Num())
	{
		FManualCameraSingleSetting& CurrCamSetting = ManualCameraSettings[CamSettingIndex];

		if (CurrCamSetting.StayTime > 0.0f)
		{
			// �� ���� �ε����� ������ ä �� �� �ӹ���.
			GetWorldTimerManager().SetTimer(ManualCameraBlendTimerHandle, FTimerDelegate::CreateUObject(this, &AB2StageEventDirector::ManualCameraBlendStayCB, CamSettingIndex), CurrCamSetting.StayTime, false);
		}
		else
		{
			// ���� ī�޶��. ���� ���� ī�޶� ���ٸ� ī�޶� ��ũ�� ���� ó���� �� ��.
			BeginTargetToManualCamera(CamSettingIndex + 1);
		}
	}
}

void AB2StageEventDirector::ManualCameraBlendStayCB(int32 CamSettingIndex)
{
	GetWorldTimerManager().ClearTimer(ManualCameraBlendTimerHandle);

	// ���� ī�޶��. ���� ���� ī�޶� ���ٸ� ī�޶� ��ũ�� ���� ó���� �� ��.
	BeginTargetToManualCamera(CamSettingIndex + 1);
}
//
//void AB2StageEventDirector::SetSESPlayActorMovementFromCamera(FSimpleEventSceneMoveKeyData& InMoveKey, class ABladeIICharacter* InPlayActor)
//{
//	if (InPlayActor && CameraAnimApplyCamIndex >= 0 && CameraAnimApplyCamIndex < MAX_DIRECTOR_CAMERA)
//	{
//		// UB2SimpleEventScene::CreateFromInterpTrack ����
//
//		UCameraComponent* BaseCamComp = DirectorCamArrayRef[CameraAnimApplyCamIndex];
//		
//		// SimpleEventScene �� Ű���� CameraAnim �� ����Ǵ� ī�޶� �������� �� �����ǥ�� ����.
//		// ��, SimpleEventScene �� ���� ���� �ǹ��� �ϴܿ� �ִ� SkeletalMeshActor �� ������� ���̹Ƿ� ĸ�� ������Ʈ�� ���ݸ�ŭ �÷��ش�.
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
//		// UB2SimpleEventScene::CreateFromInterpTrack ����
//
//		UCameraComponent* BaseCamComp = DirectorCamArrayRef[CameraAnimApplyCamIndex];
//
//		// SetSESPlayActorMovementFromCamera �ʹ� �ݴ�� InPlayActor �� �������� ī�޶� ��ġ ����
//		// ĸ�� ������Ʈ offset �� ���������� �����ؾ� ��.
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
	// ���⼭ return �ϴ� �� �Ϲ����� ��뿡�� ����Ǵ� ���̰� StageEventStateOverride �� ESES_None �� �ƴϸ� �������̵� �� ��.

	if (InEvent == EStageEvent::ESEV_Begin)
	{
		return EStageEventShowState::ESES_Appearing; // PlayActor ������ ������� ������ ��
	}
	else if (InEvent == EStageEvent::ESEV_MobAppear)
	{
		if (InPlayActorType == EStageEventPlayActor::ESEE_WaveMob)
		{
			return EStageEventShowState::ESES_Appearing;
		}
		else if (IsPlayActorTypeForPlayerCharacter(InPlayActorType))
		{
			return EStageEventShowState::ESES_Extra; // �� ��� �÷��̾ ������ �ʴ��� �ٸ� �������� ī�޶� �ִϸ��̼��� ��ģ�ٰų� ���� �ʰ�.. Extra �� ������ ���� ����üũ�� ���� �ʴ´�. �ٸ� �� ������ ���� ����.
		}
	}
	else if (InEvent == EStageEvent::ESEV_MobDying)
	{
		if (InPlayActorType == EStageEventPlayActor::ESEE_WaveMob)
		{
			return EStageEventShowState::ESES_Dead; // MobDying �̶�� �ص� �ٸ� Setting �� ���� �״� ���� �� �ִ�. �׷� ��� StageEventStateOverride �� ���� state �� ����.
		}
		else if (IsPlayActorTypeForPlayerCharacter(InPlayActorType))
		{
			return EStageEventShowState::ESES_Extra; // ���������� �� ��� �÷��̾ ������ �ʴ��� �ٸ� �������� ī�޶� �ִϸ��̼��� ��ģ�ٰų� ���� �ʰ�.
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
			return EStageEventShowState::ESES_Dead; // �̷� ��찡 ��������� ������..
		}
	}

	// StageEvent �� PlayActor Ÿ�Կ� ���� �߰�..

	return EStageEventShowState::ESES_None;
}

void AB2StageEventDirector::SyncDataToUIDoc(ABladeIICharacter* InEventTriggeringMob)
{// UI ���� �� �����͵� ����.
	//BII_CHECK(!InEventTriggeringMob || InEventTriggeringMob->IsValidObj());
	//UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();	
	//if (DocBS && InEventTriggeringMob && InEventTriggeringMob->IsValidObj())
	//{
	//	UB2NPCSingleClassInfo* EventTriggerMobInfo = InEventTriggeringMob->GetMyNPCClassInfo();

	//	DocBS->SetCurAppearedMobName(EventTriggerMobInfo ? EventTriggerMobInfo->GetLocalizedName() : FText::FromString(TEXT("")));
	//	DocBS->SetCurAppearedMobNickName(EventTriggerMobInfo ? EventTriggerMobInfo->GetLocalizedNickName() : FText::FromString(TEXT("")));

	//	// ��Ÿ �ʿ��� �͵� �߰��Ǹ�.. 
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
	// StartEventWidget ���� ��.

	if (EventWidgetType > EStageEventWidgetType::None)
	{
		if (EventWidgetStartTime > 0.0f)
		{
			GetWorldTimerManager().SetTimer(StartEventWidgetTimerHandle, this, &AB2StageEventDirector::StartEventWidgetTimerCB, EventWidgetStartTime, false);
		}
		else
		{
			StartEventWidget(); // �ٷ� ����
		}
	}
}

void AB2StageEventDirector::StartEventWidget()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2StageEventDirector::StartEventWidget"));

	//switch (EventWidgetType)
	//{
	//	//
	//	// ���⼭ ������ UI ���� IsStageEventRelatedUI �ʿ��� return true ó���� �ֵ���.
	//	//

	//	case EStageEventWidgetType::StageBegin:	
	//		{
	//			if (CachedStageManager && CachedStageManager->GetIsShowTalkDialog())
	//			{
	//				// ��ȭ������ ������ ��ȭ �ý��ۺ��� ������.
	//				AB2StageGameMode* StageGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(this));
	//				if (StageGM && StageGM->GetActiveSpawnPool())
	//				{
	//					int32 GroupID = 0;
	//					MyRelevantUIName = UIFName::StageBegin; // MyRelevantUIName �� OpenUI ������ ������ ��. �Ϻ� Ư�����̽��� �����ϸ� OpenComplete �̺�Ʈ�� Open �� ���ÿ� ���ƿ� ���̴�.
	//					if(StageGM->GetActiveSpawnPool()->TryGetStartTalkSetting(GroupID))
	//						StageEventTalkStartClass<int32, bool>::GetInstance().Signal(GroupID, false);
	//					else 
	//						UB2UIManager::GetInstance()->OpenUI(UIFName::StageBegin);
	//				}
	//			}
	//			else
	//			{
	//				MyRelevantUIName = UIFName::StageBegin; // MyRelevantUIName �� OpenUI ������ ������ ��. �Ϻ� Ư�����̽��� �����ϸ� OpenComplete �̺�Ʈ�� Open �� ���ÿ� ���ƿ� ���̴�.
	//				UB2UIManager::GetInstance()->OpenUI(UIFName::StageBegin);
	//			}
	//		}
	//		break;
	//	case EStageEventWidgetType::StageClear:
	//		{
	//			// StageClear �� �� Ư���ϰ� ����ؼ� UIScene ���� ����.
	//			// Stage ���� UI �� �ѷ� �����鼭 ���⼭�� UIStageClear ��� UIStageResult �� ����. UIStageClear �� �ణ �ٸ��� ��. AB2StageEventDirector::BeginShow, AB2StageManager::BeginStageClearScene_Phase2 ����.
	//			if (GetB2GameModeType(this) == EB2GameMode::Scenario)
	//			{					
	//				MyRelevantUIName = UIFName::StageResult;
	//				UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::StageResult);
	//			} 		
	//			else if (GetB2GameModeType(this) == EB2GameMode::CounterDungeon) //EStageEventWidgetType�� ���� �߰��ұ�..
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
	//		MyRelevantUIName = UIFName::PVPResult; // ���⵵.. �ᱹ �¸�â UI �� UIScene ����
	//		//[@AKI, 170613] �� ��Ʈ��ũ���� ����� �������� ���� ���� ���¿��� ���ȭ�� �������� �ʰ� ����
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
	//		// ���� Ÿ�̸Ӹ� ��ġ���� ���� ���� FinishShow ���� �ı�
	//		GetWorldTimerManager().SetTimer(FinishEventWidgetTimerHandle, this, &AB2StageEventDirector::FinishEventWidgetTimerCB,
	//			EventWidgetEndTime - FMath::Max(EventWidgetStartTime, 0.0f), false);
	//	}
	//	else
	//	{
	//		// �� ���� �ٷ� �����ؾ� ���� �� �� ��.��;
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
	//		//Fight PSC�� ���ؼ� ���⼭ ���� �ʴ´�.
	//		//UB2UIManager::GetInstance()->CloseUI(UIFName::PVPBegin); 
	//		break;
	//		// StageClear �� �� Ư���ϰ� ����ؼ� UIScene ���� ����. ���������� StageClear ��� ���� �ٽ� �ε��ȴٰų� �� ���̹Ƿ� ����� ������ ���� ��?
	//		// ��͵� UIScene �̶� None ����..
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
	// BeginSingleShow ���� ����ϴ� ��Ȳ�� ��ġ�ؾ߰ڴ�..
	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera || OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera)
	{		
		if (
			InSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_BringPlayActor ||
			// ManualCamera �� ��� NoAdjust �� �ƴϸ� ���� ����.
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
	//	// ExportedEventScene ������ ��Ծ�����, �ƴϸ� �׳� ī�޶��ũ�� �����ؼ� ����ϱ� ���� �ǵ��� �� ���� ����.
	//	return true;
	//}

	//if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ManualCamera && InSetting.PlayActorPlacement == EPlayActorPlacement::EPAP_NoAdjust)
	//{
	//	// ������ �ΰ��� ��Ȳ ī�޶� �̷� ������ ����� �� ���� �ְڴ�.. �� ���� ���ĸ� ���� ��ġ�� �����Ǿ� �ֱ⵵ ���װ�..
	//	return true;
	//}

	return false;
}

void AB2StageEventDirector::NotifyStageEventShowOver(class ABladeIICharacter* PlayActorJustOver)
{
	// SESEndNotify �� ���Ǵ� ��Ȳ������ �ǹ̰� ����. Controlled Matinee ��忡���� �Ƹ� �Ҹ��� ���� ��. (��� NotifyActorCustomEvent)

	if (bStandbyAtTheEnd && PlayActorJustOver)
	{
		// �� ��� ������ ������ ����ϰ� �־�� �ϴ� �������� AnimBP ���� ī�޶� Anim �ߴ� ���� �� ���� �ʵ��� Extra �� SES State �� �����Ѵ�.
		// ControlledMatinee �� ��� ���� �̰� ������ ���� �Ŷ� �׳� ���� �����µ�.. �� ��� ��������..
		PlayActorJustOver->SetStageEventShowState(EStageEventShowState::ESES_Extra, this);
	}
	else
	{
		for (int32 SI = 0; SI < ShowStates.Num(); ++SI)
		{
			if (ShowStates[SI].GetPlayActor(this) == PlayActorJustOver && ShowStates[SI].bShowFinished == false)
			{
				OnSingleStageEventShowOver(SI);
				// ��ġ�ϴ� �� �ϳ��� �־�� �����̰����� Ȥ �𸣴� break ���� �� ���� ����.
			}
		}
	}
}

static bool CheckAndGetMatineeScenarioDialogEventName(FString& InOutEventString)
{ // AMatineeActor::NotifyEventTriggered ���� ���� ScenarioDialog �̺�Ʈ �Ծ࿡ �´��� üũ�ϴ� ��. 
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
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee) // �̰� �ƴ� �����.. ���� �߸��� ���� ��
	//	{
	//		OnControlledMatineeStop(false);
	//	}
	//}
	//else if (OptionalEventName == MATINEE_DONE_STAY_ACTOR_CUSTOM_EVENT)
	//{
	//	// ��� ��� ���������� �� ���·� stay.
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee && bStandbyAtTheEnd)
	//	{
	//		OnControlledMatineeStop(true);
	//	}
	//}
	//else if (OptionalEventName == CAMERAANIM_STOP_ACTOR_CUSTOM_EVENT)
	//{
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera) // ���������� �̰� �ƴ϶�� ���� �߸�..
	//	{
	//		OnExportedCameraAnimStop();
	//	}
	//}
	//else if (OptionalEventName == CAMERAANIM_DONE_STAY_ACTOR_CUSTOM_EVENT)
	//{
	//	if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera && bStandbyAtTheEnd)
	//	{
	//		// �ϴ� Stop notification �� ������. CheckShowEndCountDown ���� bStandbyAtTheEnd �� ���� �߰� üũ�� �� ��.
	//		OnExportedCameraAnimStop();
	//	}
	//}
	//else
	//{	
	//	
	//	FString EventNameString = OptionalEventName.ToString();
	//	if (CheckAndGetMatineeScenarioDialogEventName(EventNameString))
	//	{ // EventNameString ���� Ư�� prefix �� ���� ���� ��.
	//		OpenDialogClass<FName, bool>::GetInstance().Signal(FName(*EventNameString), false); // CinematicMode �� ���� BattleUI ����Ⱑ �۵��� ���̹Ƿ� ���⼱ MaintainBattleUI �� �Ű澵 ����.
	//	}
	//}
}

void AB2StageEventDirector::NotifyShowOverFromDependentDirectorActor()
{
	if (HasValidDependentDirectorActorSetting()) // �̰� ������ �ȵǾ� ������ �Ҹ��� �ʾƾ� ��.
	{
		if (bDependentDirectorActorMet == false) // ���� �ϳ� �ߺ� ȣ�� ����..?
		{
			//////////
			bDependentDirectorActorMet = true; // �ٸ� �������� bDirectingEventConditionMet �� �����ϴµ� ���⸸ �̰� �����Ѵ�.
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
	// ����� �Դٸ� bStandbyAtTheEnd ����ϴ� ��Ȳ���� �������� ������ ��� ���� ����
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
	//	//BII_CHECK(0); //TMGameMode���� LocalPlayerCharacter�� ����. SpectatorPawn��.
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
	//{ // TMGameMode ���� ���� �� �ִ�.
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
	// bDisableInput �� �ƴϸ� ���� �÷����� ������ ���ɼ��� �����Ƿ� Auto �� ����. 
	// ������ �� �� refine �� �� �ִ�. �ٸ� ���� �˻� ������ Ư�� �Ӽ����� ���� ��.
	return bDisableInput;
}

bool AB2StageEventDirector::ShouldApplyCinematicScalabilityDuringShow() const
{ // ���⼭ false �� �����ϴ� ��Ȳ�� ��ǻ� ���� �÷����� �������� ���̳� NPC �� Ư�� �ִϸ��̼��� �÷����ϱ� ������ ��.
	return (bDisableInput || bHideHud);
}

bool AB2StageEventDirector::ShouldBeInvincibleDuringShow() const
{
	return bDisableInput; // ���⵵ ���������� �Է� ������ �̺�Ʈ ���̶�� �÷����� ������ ���̹Ƿ� ���� ���·� ���� �ʰ�..
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

	//if (!InOutDefaultSKActorToSetup) { // DefaultSKActorToSetup �� GameMode �ʿ��� �����ؼ� �ش�. 
	//	return;
	//}
	//if (OverallMode != EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee || !MatineeActor || !MatineeActor->MatineeData) {
	//	return;
	//}

	//// ���⼭�� ��Ÿ ����ϴ� ParticleSystemEffect ���� �ܾ ������Ʈ�� ����. �Ѿ�� DefaultSKActorToSetup �� �ٿ� ��.
	//TArray<AActor*> AllGroupActors;
	//MatineeActor->GetControlledActors(AllGroupActors);
	//TMap<FName, UParticleSystem*> AllFoundPSList; // �Ȱ��� ���ҽ� �ߺ� ���� ���ϰ�.. ���⿡ �ܾ����.
	//for (int32 AI = 0; AI < AllGroupActors.Num(); ++AI)
	//{
	//	// ������ ������ ��ġ�� �� Emitter ���͸� GroupActor �� �ؼ� On/Off �� ���� �����.
	//	
	//	AEmitter* CastedEmitterActor = Cast<AEmitter>(AllGroupActors[AI]); 
	//	UParticleSystem* EmitterFxResouce = (CastedEmitterActor && CastedEmitterActor->GetParticleSystemComponent()) ? CastedEmitterActor->GetParticleSystemComponent()->Template : NULL;

	//	// EmitterActor �� visibility �� �����Ϸ��� ���̱⵵ ���� (���⼭ �״ٰ� �ٷ� ���ִ� �͵� �ƴϰ� �� ������ ���Ŀ� ���ִ� �ǵ� �׷��� ���� �� �� ���۵� ���⿡�� ������ ���� ���� �ְ�.. ��ġ ����)
	//	// �� �Ȱ��� ParticleSystem ���ҽ��δ� ������Ʈ ������ ��.
	//	if (EmitterFxResouce)
	//	{
	//		FName ThisPSKey(*EmitterFxResouce->GetPathName());
	//		UParticleSystem** FoundOne = AllFoundPSList.Find(ThisPSKey);
	//		if (!FoundOne){
	//			AllFoundPSList.Add(ThisPSKey, EmitterFxResouce);
	//		}
	//	}
	//}

	//// AnimControl Track ���� �� �ִϸ��̼ǵ��� AnimNotify �鵵..
	//for (UInterpGroup* ThisGroup : MatineeActor->MatineeData->InterpGroups)
	//{ // Group > Track > AnimSeq > Notify �������� ���±� ��.��
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
	//			// ������ for �� ����
	//			for (FAnimNotifyEvent& NotifyEvt : ThisAnimSeq->Notifies)
	//			{
	//				UParticleSystem* FoundPS = CheckNotifyEventForParticleSystemUse(NotifyEvt);
	//				if (FoundPS)
	//				{ // ���ο� �Ÿ� �߰�
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

	//// �ܾ���� �ɷ� pre-render �� ������Ʈ ����
	//for (TMap<FName, UParticleSystem*>::TIterator FoundPSIt(AllFoundPSList); FoundPSIt; ++FoundPSIt)
	//{
	//	UParticleSystem* ThisPS = FoundPSIt.Value();
	//	UParticleSystemComponent* SpawnedFxComp = UB2PreRenderer::SpawnDummyPSCForPreRender(ThisPS, InOutDefaultSKActorToSetup->GetRootComponent());
	//}
	//
	//// SkeletalMeshActor ���� ShowSettings �� ������� ���� �ָ� ���� ó���� ��. ShowSetting �� ����� �ֵ��� BladeIIGameMode �ʿ��� PC, NPC ���� �ܾ���鼭 Prerender �¾��� �� ó���� ���̱� ����.
	//TArray<AActor*> NotBoundActors;
	//GetMatineeGroupActorsNotBoundtoShowSetting(NotBoundActors);
	//UWorld* TheWorld = GetWorld();
	//for (AActor* ThisNotBound : NotBoundActors)
	//{
	//	ASkeletalMeshActor* CastedNotBoundSKActor = Cast<ASkeletalMeshActor>(ThisNotBound);
	//	if (CastedNotBoundSKActor && CastedNotBoundSKActor->GetSkeletalMeshComponent())
	//	{
	//		// ���⿡ ��ġ�� SkeletalMeshActor �� ���� �� �ƴ϶� ���� �����Ѵ�. Visibility �� ������ �����ؾ� �ϹǷ�..
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
	// Pre-render �� ���ư��� ���߿� ���� Ÿ�� ������(slomoe)�� ������ �� ä�� ȭ���� ��İ� �������µ�
	// ���� Ʈ�� ���� ���� slomo �� ������ ���� �ʴµ��ٰ� (�޾Ƶ� �̻��� ��) �ƽθ� 0 �ʺ��� ������ �Ǹ� ȭ���� ������ ���¿��� ���常 ���� ���� ������ ����� �߻��ϰ� �ȴ�.
	// �̸� ���� ����
	// UB2UITalkDialog ���� �������� ������ ó���� �Ͽ���

	//if (!MatineeActor || !MatineeActor->MatineeData) {
	//	return;
	//}

	//for (UInterpGroup* ThisInterpGroup : MatineeActor->MatineeData->InterpGroups)
	//{
	//	if (ThisInterpGroup)
	//	{
	//		for (UInterpTrack* ThisTrack : ThisInterpGroup->InterpTracks)
	//		{
	//			// �ϴ� ����� sound track
	//			UInterpTrackSound* CastedSoundTrack = Cast<UInterpTrackSound>(ThisTrack);
	//			if (CastedSoundTrack && !CastedSoundTrack->IsDisabled()) // �ٸ� ������ disable �� �͵��� ���� �״� �װ� �Ÿ���
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

	//ForceSetMatineePlayingPosScale(0.0f); // ÷���� �ٽ� ����.
}
void AB2StageEventDirector::ForceSetMatineePlayingPosScale(float InPositionScale)
{
	//// Matinee �� ����ϴ� ��� Ư�� ��ġ (0.0 ~ 1.0 �����Ϸ�) �� ���� �̵��ϴµ�..
	//// �ټҰ� Ư�� �������� ��������� ���..
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

	//TMap<FName, FName> AllPossibleGroupNameMap; // �� �˻��� �� �ؾ� �ؼ� TMap ���� ������.
	//for (FPlayActorShowSettings& ThisShowSetting : ShowSettings)
	//{
	//	TArray<FName> AllSupposedNameThis;
	//	// PC �� ��� �������� �ؼ� �ϳ��� ShowSetting �� ���� Matinee Group �� ����Ǿ� ���� �� ������ ������ �͵��� ��� �˻�.
	//	GetAllSupposedInterpGroupNamesForSetting(AllSupposedNameThis, ThisShowSetting);
	//	for (FName& ThisFoundName : AllSupposedNameThis)
	//	{
	//		AllPossibleGroupNameMap.Add(ThisFoundName, ThisFoundName);
	//	}
	//}

	//TArray<UInterpGroup*> AllFoundGroupNotBoundShowSetting; // � ShowSetting ���� ������� ���� InterpGroup ���
	//for (int32 GAI = 0; GAI < MatineeActor->MatineeData->InterpGroups.Num(); ++GAI)
	//{
	//	UInterpGroup* CurrGroup = MatineeActor->MatineeData->InterpGroups[GAI];
	//	if (!CurrGroup || AllPossibleGroupNameMap.Find(CurrGroup->GroupName)){
	//		continue; // Ư�� ShowSettings �� ����� �׷��� ��ŵ
	//	}
	//	// ������� ����� InterpGroup���� GroupActor ���� �̾ƿ´�. GroupActorInfos �� ���� �׷쿡 ����� ���͵��� ã�ƿ� �� ����
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
	// Matinee Event Ʈ���� ScenarioDialogEvent �� ��ϵ� �̺�Ʈ �̸����� �ܾ���� ��.
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
	//					// NotifyActorCustomEvent �� ���� �� ������ ScenarioDialogEvent �뵵���� �����ϱ� ���� prefix �� �ٰ� ������ 
	//					// �׳� �� ���¿����� EventName �� �ٷ� DialogCode �� ��.
	//					
	//					// ����ϴ� ��Ȳ���� �ٸ� ������ �ܾ�� �͵��� �ѵ� ��ĥ �� �����Ƿ� AddUnique ��.
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
	//// ������ ����Ʈ�� �۰� ������ ������ â�� ������ ī�޶� ������Ʈ �並 ���̱� ����. �⺻ ������δ� �׳� ù��° �͸� ����.
	//if(GIsEditor && ComponentClass == UCameraComponent::StaticClass())
	//{
	//	TArray<UCameraComponent*> SelectedCameraComps;
	//	UCameraComponent* SelectedCameraComponent = NULL;

	//	// ���õ� ī�޶� ������Ʈ�� �ִ��� �˻�
	//	USelection* ComponentSelection = GEditor->GetSelectedComponents();
	//	if(ComponentSelection)
	//	{
	//		ComponentSelection->GetSelectedObjects(SelectedCameraComps);
	//	}
	//	if(SelectedCameraComps.Num() > 0)
	//	{
	//		SelectedCameraComponent = SelectedCameraComps[0];
	//	}

	//	// ���� ��ɿ����� �׳� ó�� ���� �����ϴµ� ������ �Ͱ� ���� ������ �߰� �˻�
	//	UActorComponent* FoundComponent = NULL;
	//	for (UActorComponent* Component : GetComponents())
	//	{
	//		if (Component && Component->IsA(ComponentClass) && Component == SelectedCameraComponent)
	//		{
	//			FoundComponent = Component;
	//			break;
	//		}
	//	}

	//	if(FoundComponent == NULL) // ����
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
	// ��Ÿ �ٸ� NetGameMode ������ Opponent �� �߰��� �� ����.
	//AB2PVPGameMode* PVPGM = Cast<AB2PVPGameMode>(GetB2GameMode());
	//return PVPGM ? PVPGM->GetOpponent() : NULL;
	return NULL;
}

ABladeIIPlayer* AB2StageEventDirector::GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly, bool bSpawnPuppetForDead)
{
	if (AB2AsyncGameMode* AsyncGM = Cast<AB2AsyncGameMode>(GetB2GameMode()))
		return AsyncGM->GetTeamPlayer(bAllyTeam, nPlayerIndex, bLiveOnly, bSpawnPuppetForDead); // bSpawnPuppetForDead �� true �� �Ѱ��ָ� ���� �ֵ� ���� ������ ������ �� �ִ�.

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
	//// ���� ������ ���� ī�޶� ���̶���Ʈ�� ���� ���� �÷��̾� ĳ���͸� �����ϴ� ����. ���� �װ� �ݵ�� ����Ǵ� �� �ƴϴ�.
	//if (DirectingEvent == EStageEvent::ESEV_MatchResult)
	//{
	//	if (AB2TMGameMode* TMGM = Cast<AB2TMGameMode>(GetB2GameMode()))
	//		return TMGM->GetTeamPlayer(bMatchResultEventAsWinner, 0, false, true); // ���⼭�� dead �� ĳ���͸� dummy spawn �ؾ� �ϳ�..
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

	return GetB2LocalPlayerChar(); // ��.. �ᱹ�� MobAppear �̺�Ʈ���� �ʿ��ϰ� �Ǿ���.
}

#if WITH_EDITOR

void AB2StageEventDirector::RefreshPreviewSKComps(bool bRegenerateSKComp /*= true*/)
{
	//// ������ ���� �͵� ��κ��� �����ϰ� ���� ����.
	//DestroyPreviewComponents(bRegenerateSKComp);

	//UPackage* OutermostPkg = GetOutermost();
	//// ���� ��츸 �����ؾ� ��. �������Ʈ�� ������ �ֵ� ó���� ������ �νĵ� ��.
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
	//	ActorNameDisplayText->SetRelativeLocation(FVector(0.0f,0.0f,100.0f)); // �� ������
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
	//	NewInfoTextComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // ��ġ�� ������ �����ش�
	//	NewInfoTextComp->SetText(FText::FromString(CurrSC->GetName()));
	//	NewInfoTextComp->SetTextRenderColor(FColor(255, 0, 0));

	//	AnyShapeCompNameText.Add(NewInfoTextComp);
	//}

	//if(OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//{
	//	// Controlled Matinee ���� Preview �� SkeletalMeshComponent ���� �����ϴ� ��� ��ü�� �̹� ��ġ�� SkeletalMeshActor ��� ������ �߰����� ó��..
	//	SetupPreviewForControlledMatinee();
	//	return;
	//}

	//// ���ϴ� Controlled Matinee ��忡���� �ʿ���� �͵鸸 �ִ´�.

	//for(int32 SI = 0; SI < ShowSettings.Num(); ++SI)
	//{
	//	FPlayActorShowSettings& ThisSetting = ShowSettings[SI];
	//	FTransform PreviewCompTransform;
	//	USkeletalMesh* PreviewSKMesh = NULL;
	//	if(GetEssentialPreviewInfoForSetting(ThisSetting, NULL, PreviewSKMesh, PreviewCompTransform))
	//	{
	//		// ���������� PreviewSKMesh �� PreviewCompTransform �� ���õǾ��� ��.

	//		// bRegenerateSKComp �� �ƴ϶� ������ ���� ��Ȳ�̶�� ���� �����Ѵ�.
	//		USkeletalMeshComponent* NewSKComp = (bRegenerateSKComp == false && PreviewSKComps.Num() > SI) ?
	//			PreviewSKComps[SI] : NewObject<USkeletalMeshComponent>(this, *GetPreviewSKCompNameForSetting(SI), RF_Transient);

	//		if(NewSKComp)
	//		{
	//			NewSKComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//			NewSKComp->RegisterComponent();
	//			NewSKComp->SetVisibility(true);
	//			// ������ ���ӿ��� �������� ���� �ֵ������� 
	//			NewSKComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//			NewSKComp->SetHiddenInGame(true);
	//				
	//			NewSKComp->SetSkeletalMesh(PreviewSKMesh);
	//			const float ZOffsetForCharacter = NewSKComp->Bounds.BoxExtent.Z;
	//			// SetWorldTransform �� �ϱ� ���� SkeletalMesh �� BladeIICharacter �� pivot ���̷� ���� offset �� �ݿ��Ѵ�. SyncFromPreviewSKCompPlacement ���� �ϴ� ���� �ݴ�
	//			if(DoesThisSettingRequireManualPlacement(ThisSetting))
	//			{						
	//				PreviewCompTransform.SetTranslation(PreviewCompTransform.GetTranslation() + FVector(0.0f, 0.0f, -1.0f * ZOffsetForCharacter));
	//			}
	//			NewSKComp->SetWorldTransform(PreviewCompTransform);
	//			// ��Ÿ�� PlayActor �� Snapping �� �ϴ� �굵 snapping. �ٵ� PostLoad ���������� ���� �ε����� ���� ���� ������Ʈ�� �־ �׷��� �ȸ���.
	//			SnapSkeletalMeshComponentToGround(NewSKComp);

	//			if(DoesThisSettingRequireManualPlacement(ThisSetting))
	//			{
	//				const float FirstSnapZTranslation = NewSKComp->GetComponentLocation().Z - PreviewCompTransform.GetTranslation().Z;

	//				if(FMath::Abs(FirstSnapZTranslation) < KINDA_SMALL_NUMBER)
	//				{
	//					// ����� �Դٸ� �ٷ� ���� SnapSkeletalMeshComponentToGround �� �ȸ����� ��䵥
	//					// �ռ� PreviewCompTransform �� ZOffsetForCharacter �� - �� �������Ƿ� �⺻ ��ġ�� ������ ���� �Ĺ����� ���ɼ��� �� �ִ�.
	//					// �ΰ��ӿ����� ��� �������� �Ĺ��� �� �浹 ó���� ���� ���� �ö���� ������ ������ �۾��� ȥ���� �� ���� ������ ���� �ణ �÷��� �ٽ� snap �� �õ�.
	//					// �ٸ�, �� ���� snap ���� ��ȭ�� ���ٸ� ������ ���� �����ֵ��� �ϴ� ����̹Ƿ� ���� ������ �ٽ� ��������.

	//					const FVector LocationBeforeAddtionalSnap = NewSKComp->GetComponentLocation(); // ���߿� �ǵ��� �� ����� ���� ���

	//					PreviewCompTransform.SetTranslation(PreviewCompTransform.GetTranslation() + FVector(0.0f, 0.0f, 1.0f * ZOffsetForCharacter));
	//					const FVector RaisedLocationBeforeSnap = PreviewCompTransform.GetTranslation();

	//					NewSKComp->SetWorldLocation(PreviewCompTransform.GetTranslation());
	//					SnapSkeletalMeshComponentToGround(NewSKComp); // ���� snap �� location �� ���� ��ȭ�� �ִٸ� ���⼭�� ��ġ�� �����ϰ� �� ��.

	//					const FVector RaisedAndSnappedLocation = NewSKComp->GetComponentLocation();

	//					if(FMath::Abs(RaisedAndSnappedLocation.Z - RaisedLocationBeforeSnap.Z) < KINDA_SMALL_NUMBER)
	//					{
	//						// ���� snap �� �ص� ����ġ ������ �ٽ� �ǵ�����.
	//						NewSKComp->SetWorldLocation(LocationBeforeAddtionalSnap);
	//					}
	//				}
	//			}

	//			if(bRegenerateSKComp == true || PreviewSKComps.Num() <= SI) // ������ ������Ʈ�� ���� �����ϴ� ����.
	//			{
	//				PreviewSKComps.Add(NewSKComp);
	//			}

	//			// ������ comp ���� ��ġ�� ������ sync �ϱ� ���� ���� info.
	//			FSEDPreviewCompInfo NewMappingInfo;
	//			NewMappingInfo.SKComp = NewSKComp;
	//			NewMappingInfo.ShowSettingIndex = SI;
	//			PreviewMapping.Add(NewMappingInfo);
	//				
	//			// �ȳ��� �ؽ�Ʈ. ���ó� B2SpawnPoolEditingActor ǥ��
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

	//				// SpawnPoolEditingActor �ʿ��� ũ�� �����Ϸ��� �θ� MeshComp ���� ���� ����ؼ� RelativeScale �� �Կ��µ� �׳� WorldScale �� ���̸� �Ǵ� �� �ƴѰ�? ����
	//				NewInfoTextComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	//				// Most SkeletalMesh assets have their root at their feet, so multiply the box extent by 2.
	//				// Relative location takes a value before scaling, so give InvMeshScale.
	//				const float TextRenderZOffset = (NewSKComp->Bounds.BoxExtent.Z * 2.0f); // ���⿡ NewSKComp �� scale ���� ���� �ʿ䰡 ��������..
	//				FVector TextRelativeLocation = NewInfoTextComp->GetRelativeTransform().GetLocation();
	//				TextRelativeLocation.Z = TextRenderZOffset;
	//				NewInfoTextComp->SetRelativeLocation( TextRelativeLocation );
	//					
	//				FColor InfoTextRenderColor = FColor(0,255,255);

	//				// ���������� �ȳ�����.
	//				// Directing event �� ���� �ȳ� ����
	//				FString DirectingEventString;
	//				switch (DirectingEvent) // ���⼭ ���� GetTrimmedNameFromPCClassEnum ���� ���� �� �ʿ���� ������.
	//				{
	//				case EStageEvent::ESEV_Begin: DirectingEventString = TEXT("Begin"); break;
	//				case EStageEvent::ESEV_MobAppear: DirectingEventString = TEXT("MobAppear"); break;
	//				case EStageEvent::ESEV_MobDying: DirectingEventString = TEXT("MobDying"); break;
	//				case EStageEvent::ESEV_Clear: DirectingEventString = TEXT("Clear"); break;
	//				case EStageEvent::ESEV_Custom: DirectingEventString = TEXT("Custom"); break;
	//				}

	//				// PlayActor �� ���� �ȳ�����
	//				FString PlayActorString;
	//				switch (ThisSetting.PlayActorType)
	//				{
	//				case EStageEventPlayActor::ESEE_CurrentPlayer: PlayActorString = TEXT("CurrentPlayer"); break;
	//				case EStageEventPlayActor::ESEE_OtherPlayer: PlayActorString = TEXT("OtherPlayer"); break;
	//				case EStageEventPlayActor::ESEE_MainPlayer: PlayActorString = TEXT("MainPlayer"); break;
	//				case EStageEventPlayActor::ESEE_SubPlayer: PlayActorString = TEXT("SubPlayer"); break;
	//				case EStageEventPlayActor::ESEE_WaveMob: {
	//					// ������� �� �Ÿ� ��ȿ�� ������ ��.
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
	//					InfoTextRenderColor = FColor(127,255,127); // Bä�� ��� Rä���� ������ ��¦ �� ���� �� ��?
	//				}
	//				else if(DoesThisSettingRequireManualPlacement(ThisSetting) == false)
	//				{
	//					InfoTextString += TEXT("\nPreview only");
	//					InfoTextRenderColor = FColor(0,127,127); // ���� ��Ӱ�.
	//				}						

	//				NewInfoTextComp->SetText(FText::FromString(InfoTextString));
	//				NewInfoTextComp->SetTextRenderColor(InfoTextRenderColor);
	//				PreviewSKCompsInfoText.Add(NewInfoTextComp);
	//			}
	//		}
	//	}
	//}

	//// Director ī�޶� ����� ��ȣ�� �ѷ�����.
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
	//		NewInfoTextComp->SetRelativeLocation(FVector(0.0f,-10.0f,20.0f)); // ��ġ�� ������ �����ش�
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
	//	// ThisSetting �� ����� �׷���͵� ��� ����. PC �� �ƴ϶�� �����ϸ� �ϳ��� ��.
	//	EditorGetMatineeGroupActorsFromShowSetting(ThisSetting, GroupActorsForThisSetting);		

	//	for (int32 GAI = 0; GAI < GroupActorsForThisSetting.Num(); ++GAI)
	//	{
	//		ASkeletalMeshActor* CurrGroupSKActor = Cast<ASkeletalMeshActor>(GroupActorsForThisSetting[GAI]);
	//		if (CurrGroupSKActor == NULL)
	//		{
	//			continue;
	//		}

	//		// ���� �غ� �Ǿ����� ThisSetting �� �ش��ϴ� ������ �������� ã�Ƽ� CurrGroupSKActor �� ����.

	//		FTransform PreviewCompTransform;
	//		USkeletalMesh* PreviewSKMesh = NULL;
	//		// �ι�° ���ڷ� �׷� SkeletalMesh �� �Ѱܼ� PC �� ��� ThisSetting �� ������� �� �� ���� Ư�� Ŭ������ ���� ������ �������� ��.
	//		if (GetEssentialPreviewInfoForSetting(ThisSetting, CurrGroupSKActor->GetSkeletalMeshComponent()->SkeletalMesh, PreviewSKMesh, PreviewCompTransform) == false)
	//		{
	//			continue;
	//		}
	//		// �ϴ� ���⼭�� Scale �� ������.. ��� �˰��� ABladeIICharacter::SetupControlledMatineePuppet ���� �ϴ� �Ͱ� ��������..
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
	//// PC �� ��� �������� �ؼ� �ϳ��� ShowSetting �� ���� Matinee Group �� ����Ǿ� ���� �� ������ ������ �͵��� ��� �˻�.
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
	//		continue;; // InSetting �� ����� Group �� �ƴ�.
	//	}

	//	TArray<class AActor*> CurrGroupActors;
	//	// �����Ϳ����� GroupActorInfos �� ���� �׷쿡 ����� ���͵��� ã�ƿ� �� ����. -> �ƴ� ������ ���� �׳� ���ӿ����� �Ǵ� �� �ƴѰ�..?
	//	for (int32 GAI2 = 0; GAI2 < MatineeActor->GroupActorInfos.Num(); ++GAI2)
	//	{
	//		if (MatineeActor->GroupActorInfos[GAI2].ObjectName == FName(*CurrGroup->GetName()))
	//		{
	//			// OutGroupActors �� ���Ѵ�. �ϳ��� �׷쿡 ���� Actor �� ��ϵ� ��Ȳ�� �󸶳� �ֳ�?
	//			OutGroupActors += MatineeActor->GroupActorInfos[GAI2].Actors;
	//			break;
	//		}
	//	}
	//}
}

void AB2StageEventDirector::SyncFromPreviewSKCompPlacement()
{
	// PreviewMapping �� ���� �� preview component ���� ShowSetting �� ã�Ƽ� Placement ���� ������ ���� �ʿ��ϸ� ���� �����Ѵ�.

	for(int32 PCI = 0; PCI < PreviewSKComps.Num(); ++PCI)
	{
		USkeletalMeshComponent* CurrSKComp = PreviewSKComps[PCI];
		int32 ShowSettingIndex = -1;

		for(int32 PMI = 0; PMI < PreviewMapping.Num(); ++PMI)
		{
			if(PreviewMapping[PMI].SKComp == CurrSKComp) // �Ƹ� PCI �� PMI �� ��ġ�� ��
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
				// ������� �̰� �ƴ� ��Ȳ������ ���� �� ������ Sync �� �� ��츸.

				// ���⼭ ���� ���� BladeIICharacter ���� �� ���̹Ƿ� Z offset �� �� �����ؾ߰ڴ�. �̷����� �� �̽���ġ�� �ְڴµ�..
				float ZOffsetForCharacter = CurrSKComp->Bounds.BoxExtent.Z;

				// ���ݱ����� ��뿡�� ManualRelativeLocation/Rotation �� �� ������ ��Ʈ �����̹Ƿ� �� �ܼ��� GetRelativeTransform �ϸ� �ǰڴ�.
				// �ƴ� ��찡 �����, GetComponentTransform �ߴٰ� �ʿ��� ���� transform �� ������.
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
			// Ŭ������ ���س��� ������. ���� WorldSetting �� ������ GameMode �� �� ���� �ְ����� ���� �׷� �ʿ䰡..
			// Main �̳� Current �� Gladiator ��, �ƴϸ� Assassin
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
	//	// �ΰ��� ��ġ�� ����ϴ� ��� �� ������ ��ġ�� ���ٸ� ������ �����δ� ������ ��ġ�� ���´�. Default �� ������ ī�޶�� ���ֺ���?
	//	OutTransform = FTransform(FRotator(0.0f,-90.0f,0.0f), FVector(0.0f,100.0f,0.0f)) * this->GetTransform();
	//	bGotTransform = true;
	//}
	//else if (DoesThisSettingRequireManualPlacement(InSetting)) // �Ϲ����� ���� ���� ��Ȳ
	//{
	//	const FTransform RelativeTransform(InSetting.ManualRelativeRotation, InSetting.ManualRelativeLocation);
	//	OutTransform = RelativeTransform * this->GetTransform();
	//	bGotTransform = true; // �ϴ� ����
	//}
	//else if(OverallMode == EStageEventDirectorOverallMode::ESEDOM_ExportedCamera)
	//{
	//	// SimpleEventScene �� ����ϴ� ��찡 �ǰڴ�. ���ó� AdjustToPlayActor �� �����ϰ� CameraAnimApplyCamIndex �� ������ ī�޶� ���缭 ExportedEventScene �� ù Ű������ ��ġ ����.
	//	// ���� �׳� ������θ�.
	//	if (IsExportedEventSceneValid(InSetting.ExportedEventScene) && CameraAnimApplyCamIndex >= 0 && CameraAnimApplyCamIndex < MAX_DIRECTOR_CAMERA)
	//	{
	//		FSimpleEventSceneMoveKeyData& FirstKeyData = InSetting.ExportedEventScene->SimpleMoveKeyFrames[0];

	//		// SetSESPlayActorMovementFromCamera ����. �����ϰ� �ʿ��� �ٽɸ�.
	//		UCameraComponent* BaseCamComp = DirectorCamArrayRef[CameraAnimApplyCamIndex];
	//		OutTransform = FTransform(FirstKeyData.RotKey, FirstKeyData.PosKey) * BaseCamComp->GetComponentTransform();
	//		bGotTransform = true;
	//	}
	//}
	//else if (OverallMode == EStageEventDirectorOverallMode::ESEDOM_ControlledMatinee)
	//{
	//	// ControlledMatinee ���� SkeletalMeshActor ������ ���纸�� ���� �����Ŵ. ���⼭�� ���� �ʿ��ϴٸ� �ش� �׷���� ã�Ƽ� ��ġ�� Transform �� �ִ´ٰų� �غ� �� �ְڴ�.
	//	OutTransform = FTransform();
	//	bGotTransform = true;
	//}

	//if (bGotTransform == false)
	//{
	//	return false;
	//}

	//// ǥ���� transform �� �������� SkeletalMesh �� �̾ƿ���.	
	//if( IsPlayActorTypeForPlayerCharacter(InSetting.PlayActorType) )
	//{
	//	UB2PCClassInfo* PCClassInfo = GetPCClassInfoForPlayActorType(InSetting.PlayActorType); // ���⼭�� �ƿ� ������ Ÿ���� PCClass �� ������ ��. ������ �����Ͷ� � Ŭ������ ������ �𸣴� ���̹Ƿ�.

	//	if (OptionalSpecificPCClassMesh)
	//	{
	//		// �� ��� Player Character �̸鼭 �޽��� �����Ͽ����� �� Mesh �� ����ϴ� PCClassInfo ���� ������ ������.
	//		// �̰� ��ǻ� Ŭ�������� Ʈ���� ������ �ϴ� Controlled Matinee ����.
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
	//	// ������� ã�� PCClassInfo �� ������ �ɷ�..
	//	if(PCClassInfo)
	//	{
	//		OutSKMesh = PCClassInfo->BaseMeshAsset;

	//		// PCClassInfo �� ������� �����ϵ� ���� ����.
	//		if(PCClassInfo->GetBaseBPClass(this))
	//		{
	//			ABladeIIPlayer* DefaultPlayerBPObject = Cast<ABladeIIPlayer>(PCClassInfo->GetBaseBPClass(this)->GetDefaultObject());
	//			if(DefaultPlayerBPObject && DefaultPlayerBPObject->GetMesh())
	//			{
	//				// ���⼱ GetMesh()->GetComponentScale �δ� �����ϴ� ���� �ȳ���. RootComponent ���� �ؼ� relative scale �� ������ ���Ѵ�.
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
	//	if (RelevantEditorSpawnPool && IsStageSupported(EditorStageNum, EditorStageDifficulty)) // WorldSettings �� ������ EditorStageNum �� �����Ǵ� ����������.
	//	{
	//		// NPCClass enum Ȥ�� ABladeIICharacter �������Ʈ Ŭ���� �� �� �ϳ�.
	//		ENPCClass NPCClassEnum = ENPCClass::ENC_End;
	//		ENPCClassVariation NPCClassVariation = ENPCClassVariation::ENCV_Normal;
	//		TSubclassOf<ABladeIICharacter> NPCClassBP = NULL;
	//		RelevantEditorSpawnPool->EditorGetSpawnClassOfWaveMob(NPCClassEnum, NPCClassVariation, NPCClassBP, WaveNum, MobIndex);

	//		// AB2MonsterSpawnPool::SpawnWave �� �������� �켱������..
	//		if(NPCClassEnum != ENPCClass::ENC_End)
	//		{
	//			UB2NPCClassInfoBox* MobClassInfoBox = StaticFindMobClassInfoBox();
	//			if(MobClassInfoBox)
	//			{
	//				UB2NPCClassInfoBox::NPCClassPreviewAssetInfo PreviewAssetInfo = MobClassInfoBox->GetPreviewAssetInfoFromClass(NPCClassEnum, NPCClassVariation);
	//				// PreviewAssetInfo �� MaterialOverride �� ������ �ϴµ� ���⿡�� �ݿ��� �ұ� ����..
	//				OutSKMesh = PreviewAssetInfo.SKMesh;

	//				// ���⵵ NPCClassInfo �� ������� ������ ���߱�.
	//				UB2NPCSingleClassInfo* SingleMobInfo = MobClassInfoBox->GetNPCSingleClassInfo(NPCClassEnum, NPCClassVariation);
	//				if(SingleMobInfo && SingleMobInfo->GetBaseBPClass(this))
	//				{
	//					ABladeIICharacter* DefaultBPObject = Cast<ABladeIICharacter>(SingleMobInfo->GetBaseBPClass(this)->GetDefaultObject());
	//					if(DefaultBPObject && DefaultBPObject->GetMesh())
	//					{
	//						// ���⼱ GetMesh()->GetComponentScale �δ� �����ϴ� ���� �ȳ���. RootComponent ���� �ؼ� relative scale �� ������ ���Ѵ�.
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
	if(bDestroyAll) // �̰� false �� ���� �ܼ� ��ġ �̵� �� refresh �� ����ϱ� ����.
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

	//// ��Ƽ�� �׷� ���ͷ� ��ϵ� ASkeletalMeshActor ���� ShowSettings ��� ���ο� ������� collision ������ �� ����. ���� �ʿ��� ��찡 ������?
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

	//			// ActorHiddenIngame ���� ��������
	//			if (SKActor->bHidden)
	//			{
	//				// ����, ��Ȥ �ǵ�ġ �ʰ� ������ �ʴ� ���·� �ִϸ��̼��� �÷��̵ǰ� �־ ������ �Ǵ� ��찡 ����. (AnimNotify �߻�) ���� �̰͵鵵 ���� ��. DirectedFromBeginning �̸� ���� �� �ִϸ��̼� ���� ���.
	//				SKActorComp->AnimationData.bSavedPlaying = false;
	//				SKActorComp->AnimationData.bSavedLooping = false;
	//				if (!bHasAnyDirectedFromBeginningSetup)
	//				{
	//					// AnimToPlay ���۷����� Ư���� ���� �� �������� ���� �ε��� �ϰ� �Ǹ� �ʿ���� �ε��� �����ϹǷ� ����ȭ ���ɼ� �鿡���� �����ִ� �� ������
	//					// DirectedFromBeginning ���ð� ���� ���� ���ּ� �� �ȴ�.
	//					// �� SkeletalMeshActor �� ��ü StageEventDirector �� ��� �ϳ��ϰ�� DirectedFromBeginning ���� �������� �˻��Ϸ��� �� ���̰� �ؼ� 
	//					// �׳� ��ü ���忡 �׷� ������ ���� ��츸 ���. �׷��� ���� ����ϴ� �� �ƴ϶�..
	//					SKActorComp->AnimationData.AnimToPlay = nullptr;
	//				}
	//			}
	//		}
	//	}
	//}

	//// ShowSetting �̶� ������ �Ǿ��ִ� SkeletalMeshActor ���� visibility ���� �߰� ���ñ��� �� ����.
	//for (int32 SI = 0; SI < ShowSettings.Num(); ++SI)
	//{
	//	FPlayActorShowSettings& ThisSetting = ShowSettings[SI];
	//	TArray<class AActor*> GroupActorsForThisSetting;
	//	// ThisSetting �� ����� �׷���͵� ��� ����. PC �� �ƴ϶�� �����ϸ� �ϳ��� ��.
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
	//		ThisSKComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered; // �̰͵� ���� ���߿��� Ȱ���� ������Ʈ�ϵ��� ���� �ٲ��ִ� �⺻���� ����߷� ������.

	//		// �߰��� MaterialInstanceDynamic �� overriding �Ǿ� �ִٸ� �����ش�. ClearControlledMatineePuppet ���� �Ϸ��� �ǵ� ���� �� �ȸ����� ���⼭ ���� Ȯ���ϰ�..
	//		// MID �� �� ó������ ������ �ϱ� ���� �� �ƴϴ�.
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

	// ������ �̸� �Ȱ��������� Ȯ������
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
//		// ��� �ϳ��� �ֵ��� ��.
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
//		// MainPlayer �� SubPlayer �� �Ѿ� ������ �ʰ� üũ �� ���
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
//						FString::Printf(TEXT("[Warning] ������ �÷��̾� ĳ������ PlayActorType �� �ߺ��Ǿ� ��Ÿ�����ϴ�.\nIndex %d and %d"), IA, IB)
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
//			FString WarnMsg = TEXT("���� Directing Event ���������� Mob �� �������� ���� ���¿��� ���� ����� ���۵� ������ �ֽ��ϴ�. Ư���� ������ �ƴϸ� Mob Appear/Dying/Phase �� ����ϰų� Show Settings ���� Wave Mob �� �����ּ���.\n\n");
//			
//			if (DirectingEvent == EStageEvent::ESEV_ComponentTrigger)
//			{
//				WarnMsg += TEXT("Ư�� ComponentTrigger ���� �Ǽ��ϴ� �����̴� Ȯ���� �������� �ʿ��� ������ Ȯ���ϰ� ����ϼ���!\n\n");
//			}
//
//			WarnMsg += TEXT("Mob �� �����ϴ� Show Setting ��ȣ : ");
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
//				FString::Printf(TEXT("[Warning] MobDying �̺�Ʈ�� StartDelay %.2f (��)�� �����Ǿ� �ֽ��ϴ�.\n�⺻ ������ ������ Dying Mob �� �����ϴ� ��� StartDelay �� ����� ������ ������� ���� ���ɼ��� �����ϴ�.\n������ �ٸ� ����� ã�ų� Ȯ���� �׽�Ʈ�� ��ġ�� ���� �����ϴ�."), OverallStartDelay)
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
//				// ���� �ִ� Wave Num �� ����� Ȯ��
//				for (int32 SI = 0; SI < ThisSP->GetStageSettingsNum(); ++SI)
//				{
//					// SpawnPool �� TargetDifficulty �� �������� ���� ��밡���� ���̵��� ��������. 
//					// �װ� �������ڸ� �̹� ������ TargetDifficulty ���� ���� ���̵��� ���鼭 üũ�غ� �ʿ�� �ִµ� �׷��� ���ڸ� ���� ��Ȳ�� üũ�ؾ� �ؼ�.. �� �Ʒ� �޽����� �����ϴ� ������.
//					int32 ThisSPSettingDifficultyLevel = ThisSP->GetTargetDifficultyLevelBySettingIndex(SI);
//					if (IsStageSupported(ThisSP->GetTargetStageIdBySettingIndex(SI), ThisSPSettingDifficultyLevel) == false)
//					{
//						continue; // �����Ǵ� �������� ���ø�.
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
//				FString::Printf(TEXT("[Warning] Mob Event ����� ���� Spawn Pool ���Ͱ� �߰ߵ��� �ʾҽ��ϴ�."))
//				));
//#endif
//		}
//		else if (!bFoundWaveSetting)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[WARNING!] ���忡 �����ϴ� Monster Spawn Pool ���Ϳ��� ������ ��������/���̵��� ������ %d ��ȣ�� Wave Setting �� ã�� �� �����ϴ�.\n\n���̵��� ��� SpawnPool ������ ���� ���̵� ������ ���� ���̵��� ����� �� �����Ƿ� �����δ� ��� ������ ���� �ֽ��ϴ�."),
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
//			// PlayActor �������� ��ġ�� �����ϴ� �Ŷ�� ���� �� PlayActor�� �ϳ��� �־�߰ڱ�.
//			if (ShowSettings[SI].IsOneOfAdjustToPlayActorPlacement())
//			{
//				++TotalCount;
//			}
//		}
//		if (TotalCount >= 2)
//		{
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] %d ���� AdjustToPlayActor �迭 ������ �߰ߵǾ����ϴ�.\n�ϳ��� �ѵδ� ���� �����մϴ�."), TotalCount)
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
//				FString::Printf(TEXT("[����] MatineeFirstSkipFrames ������ŭ ù %d �������� ��ŵ�˴ϴ�.\n\n�����Ϳ��� ���� ����� ���� ������� ���� ������� �ٸ� ���� �����Ƿ� �ʹ� ū ���� ������� �ʴ� ���� �����ϴ�."), MatineeFirstSkipFrames)
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
//				FString::Printf(TEXT("Clear �̺�Ʈ�� ���;� �� ��Ȳ���� ��ŵ�� ������ �ִ� ManageMode �� ������ �Ǿ����ϴ�."))
//				));
//#endif
//		}
//	}
//
//	if (PropertyName == Name_RandomMatineeGroupNum)
//	{
//#if !PLATFORM_MAC
//		if(RandomMatineeGroupNum == 1) // 1 �̸� �����δ� ���� ������ ���� �ʴ� ���ε� Ȥ ���� ������ �ǵ��ϰ� �־������� �𸣴�.
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("RandomMatineeGroupNum 1 ���������� ������ ���� ���� ����� �۵����� �ʽ��ϴ�.\n���� ���� ���� ����� �ǵ��ߴٸ� 2���� ū ������ ������ �ּ���."))
//				));
//		}
//		else if (RandomMatineeGroupNum >= 2)
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("RandomMatineeGroupNum �� %d (��)�� �����߽��ϴ�.\n���� ���� ���� ����� ���ؼ��� ����ϴ� Matinee �� �� �׷���� ���⿡ ���� ������ Rand ���̹����� �����ؾ� �մϴ�.\n�ڼ��� ������ ���� �Ŵ����� �����ϼ���."),
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
//				FString::Printf(TEXT("LocalPlayerSwitching �� �մϴ�. Matinee ����� �����ϴ� ĳ���Ϳ� ������ ĳ���Ͱ� �ٸ���� ������ ���� ĳ���ͷ� ����ĳ���͸� ����Ī ��ŵ�ϴ�.\n\n���� : ���⿡ ���� ���� flow �� ����˴ϴ�. ��� ���߿� ���� �۵��ϴ� �����̳� ���Ӹ�尡 ������ �� �ֽ��ϴ�."))
//			));
//#endif
//		}
//	}
//
//	CheckAndWarnForLightSetup();
//
//	//if (
//	//	PropertyName == Name_OverallMode || PropertyName == Name_ShowSettings || // ShowSettings �߰��ÿ��� �۵��ϵ���
//	//	PropertyName == Name_bOverallMode_ExportedCamera || PropertyName == Name_bOverallMode_ControlledMatinee ||
//	//	PropertyName == Name_bOverallMode_ManualCamera || PropertyName == Name_bOverallMode_ExportedOrManualCamera
//	//	) 
//	// �� ������ �۵��ϵ��� �ؾ� �� ��.
//	{
//		SetEditConditionProperties();
//	}

	// ���� ��Ȳ���δ� ������ ������Ʈ�� ��ġ���� �Ӽ�â���� �����ؼ� ���� notify �� �����Ƿ� ���⼭ Sync �� ���ϰ� Refresh ��..
	RefreshPreviewSKComps();
}

void AB2StageEventDirector::SetEditConditionProperties()
{
	bOverallMode_ExportedCamera = false;
	bOverallMode_ControlledMatinee = false;
	bOverallMode_ManualCamera = false;
	bOverallMode_ExportedOrManualCamera = false;

	// OverallMode ���ÿ� ���� �������� �ϰ�������
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

	// �̰͵� ���� �����ֱ�.
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
		// Preview ������Ʈ�� �̵������� �� ������ ��ġ���� ��ũ
		SyncFromPreviewSKCompPlacement();
		// ���⼭�� RegenerateSKComp �� ���� �ʴ´�. PostEditMove ��Ȳ������ ���� regenerate �� �� �ʿ䵵 ����, �������� �� �̵��ø��� ���� ��Ŀ���� �Ҿ������ �����ϱ� ����.
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
//				// ���������� Light �� ����� InterpGroup �� �θ� Folder Group ���̹����� On/Off �� �Ѵ�.
//				UInterpGroup* ParentFolderGroup = FindParentGroupFolder(GroupOfThisInfo, MatineeActor);
//
//				if (ParentFolderGroup && IsPossibleInterpGroupNameForPC(ParentFolderGroup->GroupName.ToString()))
//				{
//					++LightCount_PossibleManagedGroup; // �̰ɷ� ������ �͵� �߿��� �Ƹ� �� �ϳ��� ���� ������ ����..
//				}
//				else
//				{
//					++LightCount_NoneManagedGroup;
//				}				
//			}
//		}
//	}
//	const int32 TotalLightCount = LightCount_NoneManagedGroup + LightCount_PossibleManagedGroup;
//	// LightCount_PossibleManagedGroup �� ���ÿ� ���� �����ϰ� ���õǸ� �� �̻� ���� �� �ִµ�.. �װͱ����� �� �̻� ���ϰڰ�..
//	const int32 TotalLightCount_TurnOnTogether = (LightCount_NoneManagedGroup + (LightCount_PossibleManagedGroup > 0 ? 1 : 0));
//
//	if (TotalLightCount > AllowedMatineeTrackPointLightNum)
//	{
//		if (TotalLightCount_TurnOnTogether > AllowedMatineeTrackPointLightNum)
//		{
//			// ������ ������ �ɸ��� ��Ȳ.
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(
//					TEXT("%s ���� �����ϴ� PointLight �� ����� �������� ��ü ������ �ѱ� ������ ����˴ϴ�. ���� �������� Ȯ���� �ּ���.\r\n\r\n")
//					TEXT("1. HidePlayerPointLight : %d %s\r\n")
//					TEXT("2. AllowOtherPlayerPointLight : %d\r\n")
//					TEXT("3. Matinee �� ����� PointLight Ʈ���� �ǵ���� ����Ǵ��� Ȯ���� �ּ���.\r\n")
//					TEXT("  - ������ On/Off ��� �Ǵ� �׷쿡 ��ϵ� PointLight ����  : %d\r\n")
//					TEXT("  - ������ On/Off ��� �� �Ǵ� �׷쿡 ��ϵ� PointLight ���� : %d"),
//					*GetName(), bHidePlayerPointLight, (bHidePlayerPointLight ? TEXT("") : TEXT("(Ư�� �� ���� 0 �̸鼭 �� ��� �߸� �����մϴ�)")), 
//					bAllowOtherPlayerPointLight, LightCount_PossibleManagedGroup, LightCount_NoneManagedGroup)
//			));
//#endif
//		}
//	}
}
//
//UInterpGroup* AB2StageEventDirector::GetInterpGroupOfObjectName(FName InFindGroupObjectName)
//{
//	// FInterpGroupActorInfo �� ObjectName �� ������ ã���� �ϴ� ��.
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

	// Ư�� ���� �Ǽ��ؼ� ���׸� �߱��ϴ� �����̶� Ư����.. ��ü StageEventDirector ���鼭 ���� ���� ��Ƽ� ��� ���.
	// ���� �����ص� ComponentTrigger �� ��� ������ ��찡 �ֱ� �Ҳ��� �װͺ��� �Ǽ��� �� ����..
	
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
		//FString WarnMsg = TEXT("Wave ���� �����ϴ� ���� StageEventDirector ���͵鿡 ComponentTrigger ���� ������ �Ǿ� �ֽ��ϴ�. �̷� ������ ����ϰ� �޴� �������� ���� ���� ���׸� �߱��մϴ�. ���� ���� ��ġ�� ��ȹ�ڿ��� ���� �Ͽ� SpawnPool �� ���� �����ϰų� PCSyncToMatineeStart ����� Ȱ���ϼ���.\r\n\r\n");
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