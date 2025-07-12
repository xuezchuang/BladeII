// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyGameMode.h"
//#include "BladeII.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/SpectatorPawn.h"
#include "Animation/SkeletalMeshActor.h"

#include "BladeIIGameMode.h"
#include "B2UIManager_Lobby.h"
#include "B2UIManager.h"
#include "BladeIIPlayer.h"
#include "B2PCClassInfo.h"
#include "B2PCClassInfoBox.h"
#include "B2LobbyInventory.h"
#include "B2NetworkBridge.h"
#include "Event.h"
#include "B2StageManager.h"
#include "Retailer.h"
#include "CommonStruct.h"
#include "B2StageInfo.h"
#include "B2ChapterInfo.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
#include "B2UIDocBattleStage.h"
#include "B2CombatStatEvaluator.h"
#include "EngineGlobals.h"
#include "BladeIIPointCharger.h"
////#include "Matinee/MatineeActor.h"
#include "B2StageGameMode.h"
#include "B2UIChatting.h"
#include "B2Airport.h"
#include "B2UIHeroTowerReady.h"
#include "B2UIDocRaid.h"
#include "B2UIMsgPopupSelectPCClass.h"
#include "Packet.h"
#include "PacketMaker.h"
#include "ChannelManager.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIBattleScoreNotify.h"
#include "B2UIDocLobbyMatch.h"
#include "B2SkillRTManager.h"
#include "B2UIAttendanceMain.h"
#include "TutorialManager.h"
#include "B2BGMManager.h"
#include "B2DLCFrontGameMode.h"
#include "B2UICommonInvite.h"
#include "B2UIPVPCharacterSelectPage.h"
#include "B2UIMsgPopupSelectPCClass.h"
#include "B2UIMsgPopupInventoryFull.h"

//#if PLATFORM_ANDROID
//#include "Android/AndroidJNI.h"
//extern bool AndroidThunkCpp_GetCharacterInfoScene();
//#endif
#include "B2UIGuildMapMain.h"
#include "B2GuildCastleActor.h"
#include "B2UIRaidMain.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIModRewardBoxGradeInfo.h"
#include "B2UIModRewardBoxDetailInfo.h"
#include "B2UISummonItemDisplay.h"
#include "B2PreRenderer.h"
#include "B2GameLoadingProgress.h"
#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"
#include "B2PlatformAPI.h"
#include "B2GameDefine.h"


bool AB2LobbyGameMode::IsFirstLobby = true;
bool AB2LobbyGameMode::IsFirstLobbyMain = true;

AB2LobbyGameMode::AB2LobbyGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bDefaultMatineePlayedReverse(false)
	, bIsInputLeftControl(false)
{
	//PrimaryActorTick.bCanEverTick = true;

	//DefaultPawnClass = ASpectatorPawn::StaticClass(); // SpectatorPawn �� �⺻����
	//PlayerControllerClass = APlayerController::StaticClass(); // �ʿ信 ���� ���� extend �ؼ� ���..

	//DefaultPCClassEnum = EPCClass::EPC_End; // �ϴ� spawn ���� �ʴ´�.

	//GameModeToPlay = ABladeIIGameMode::StaticClass();

	//bHeroMgmtMode = false;
	//HeroMgmtSubMode = EHeroMgmtSubMode::EHMSM_Inventory;

	//SelectedChapterViewNum = 0;
	//SelectedStageDifficulty = EStageDifficulty::ESD_None;

	//LobbyUIManager = NULL;
	//LobbyInventory = NULL;
	//LobbyAirport = NULL;

	//InputHistory_CheckFlick.InitializeHistory();
	//InputHistory_IgnoreCheck.InitializeHistory();

	//if (!HasAnyFlags(RF_ClassDefaultObject) && GConfig)
	//{
	//	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("ConsiderFlickDeltaTime"), ConsiderFlickDeltaTime, GGameIni);
	//	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("ConsiderFlickScreenDeltaRatio"), ConsiderFlickScreenDeltaRatio, GGameIni);
	//	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("OddInputMinTimeInterval"), OddInputMinTimeInterval, GGameIni);
	//	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("OddInputMinPosDeltaRatio"), OddInputMinPosDeltaRatio, GGameIni);
	//}

	//bLobbyLoadingScreenCompleted = false;
	//bEnableLobbyScreenRotate = false;

	//bWaitPrerequisiteToStartForNonHost = false;
}

void AB2LobbyGameMode::BeginPlay()
{
	//B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::BeginPlay"));

	//Super::BeginPlay();	
	//
	//AB2StageManager::GetCacheStageKeepEssentialData().ClearStageBasisData();

	//// PreRenderGameMode ���Կ� ���� ù �κ� �ε� ���� ���ٸ� �ϴ� �� ���� �ٷ� Pre-render ���� ���� �ε��ϴ� ��Ȳ�� �߻��ϰ� �ȴ�.
	//const bool bMapTransitionWillBeginSoon = IsFirstTimePreRenderPending(); // �Ƹ� �̰� true �� �׳� ���⼭ return �ϴ� �� �̵������� ��

	//if (IsFirstLobby && !bMapTransitionWillBeginSoon)
	//{
	//	// �⼮ UI �� ó�� �� �� ���� �ɸ��� ���� �ߴ� ��Ȳ ������ �� ���� ������ ù �ε��ð� ������ �����ϰ� �̸� ������ �ݴ°�. ���⿡�� StayInViewport üũ�� �Ǿ� �־ CloseUI �ϴ� �� �׳� ����� ����.
	//	UB2UIAttendanceMain* atta = UB2UIManager::GetInstance()->OpenUI<UB2UIAttendanceMain>(UIFName::AttendanceMain);
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::AttendanceMain);
	//	B2GMLoadingProgCollector::StepCustom(0.4f);
	//}

	//// �Ƹ� BeginPlay ���� ������ �ʿ������� ���� ��.
	//LobbyUIManager = NewObject<UB2UIManager_Lobby>(this, *FString::Printf(TEXT("UIManager_Lobby_%s"), *this->GetName()), RF_Transient);
	//if (LobbyUIManager)
	//{
	//	// LobbyUIManager �� DJLegacy �� �Ϻ��ε� ���ο��� �� UIManager �� ����ϴ� �κ��� �־ ���� �ϳ� �̰� �����Ѵٸ� ���θ� ���캼 ��. (�Ƹ� ������ ���� ���� ��)
	//	if (LobbyUIManager->InitUIManager(this) == false)
	//	{
	//		LobbyUIManager->ConditionalBeginDestroy();
	//		LobbyUIManager = NULL;
	//	}
	//}
	//if (IsFirstLobby) // ó���� UB2UIManager_Lobby ���� LoadOnStartup ó���ϸ鼭 �ð� �ɸ��� �� �� ����.
	//{
	//	B2GMLoadingProgCollector::StepCustom(0.4f);
	//}

	//// LobbyUIManager ���� ���ķδ� ������ ������ ���� access �ϴ� ��� Event �ý����� �̿��Ͽ� �޽����� ������.
	//SetPCSelection(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());

	//CreateLobbyInventory(); // �ʿ信 ���� ������ �� ����..
	//CreateLobbyAirport();

	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	//Matinee actor���� cinematic mode�� on���� off�� ��, ������ Virtual joystick visibility�� �Ѱ� �Ǿ�����.
	//	//���ҽ����� �����ؾ� �� �� ������, �Ǽ��� ���� �� ������ ���⼭ ������ cinematic mode�� �������� �ʵ��� �Ѵ�.
	//	//UWorld::GetMatineeActors() �� �Լ��� �ִµ�, CurrentLevel�� ���ؼ��� ã�ƿͼ� �Ʒ�ó�� �ڵ�
	//	for (FActorIterator It(World); It; ++It)
	//	{
	//		auto* MatineeActor = Cast<ALevelSequenceActor >(*It);
	//		if (MatineeActor)
	//		{
	//			MatineeActor->bNoCinematicModeControl = true;
	//		}
	//	}
	//}

	//auto* PlayerController = UGameplayStatics::GetLocalPlayerController(World);
	//if (PlayerController)
	//	PlayerController->ActivateTouchInterface(nullptr);

	//LobbySceneManager.SetGameMode(this);

	////[@Woongbok, 171222][TODO] ���⼭ �ȹ޾Ƶ� �ɵ� �ϳ� ���� ������ �ʿ���.
	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{ // �ֿ� ������ ��ġ UI ������Ʈ. �κ񿡼� �� ��ġ�� �� �ִ��� Ȯ���ϰ� ������Ʈ �Ǵ� ���� �ϳ� �־��ִ� �ɷ�
	//	OnUpdateCombatPower(IntToPCClass(PCI), false);
	//}
	//	
	//if (IsFirstLobby && !bMapTransitionWillBeginSoon)
	//{ // ù �κ� ���� ��ũ ����
	//	IsFirstLobby = false;
	//}
	//check(bMapTransitionWillBeginSoon || !IsFirstLobby);
}

void AB2LobbyGameMode::PreBeginPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::PreBeginPlay"));

	Super::PreBeginPlay();
}

void AB2LobbyGameMode::StartPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::StartPlay"));
	
	SetUseCustomWaitForLoadingScreen(true); // ������ StartPlay ��ƾ���� ������ �ʵ��� ���Ƴ���.. ���⼭ �ڵ鸵�� ��.

	if (IsFirstLobby)
	{ // ������ �ʿ��� Scalability override �����ϱ� ���� ����..
		InitializeAllGameSettingData(this);

		FString Title;
		if (FParse::Value(FCommandLine::Get(), TEXT("TitleName="), Title))
		{
			UKismetSystemLibrary::SetWindowTitle(FText::FromString(Title));
		}
	}

	Super::StartPlay();

	// �κ� ���Ӹ���� ���൵�� �⺻ StartPlay �� Preload ������ ������ ���� ���� ���⼭ ���� DeferredChangeLobbyScene ����..
	if (LobbySceneManager.HasDeferredChangeLobbyScenes())
	{
		B2GMLoadingProgCollector::StepCustom(0.5f, 0.5f);
		// ���� ChangeScene �� �����ϴ� ���.. �װ� �󸶳� ������ ���� �ٸ� ����.. ��κ� �ϳ��ϱ�.. ū ������ ����.
		// �׷� UIScene �ϳ� ���� ���� �� ä�����.
		B2GMLoadingProgCollector::SetStepRate(0.9f);
	}
	else
	{
		B2GMLoadingProgCollector::StepToMax();
	}

	// �����ʼ� ���ƿ��ų� �� ������ DeferredChangeLobbyScenes �� ������ ���� open ����Ʈ�� ���� ����. �װ� �ƴ� ��츸 Title ȭ���� ����. 
	// ������ Title ȭ���� �������� ������ ���� ������� �ٲ������ �׳� �׷��� �θ� �κ�� ���ƿ� ���� �ε��ð� ���� ��.
	if (!LobbySceneManager.HasDeferredChangeLobbyScenes())
	{
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_Title);
	}
	else
	{
		//titleȭ�鿡���� ������ bgm���
		//titleȭ���� �ƴҽø� �κ� bgm���
		PlayMainLobbyBgm();
	}

	if (UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter())
	{
		if (DocChapter->GetShowChapterEventScene())
		{
			LobbySceneManager.ClearDeferredChangeLobbyScenes();
			LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_LobbyMain);
			
			FServerStageID DeferredServerStageID = DocChapter->GetDeferredServerStageID();
			if (DeferredServerStageID != 0)
			{
				FStageDataStore& StageDataStore = BladeIIGameImpl::GetStageDataStore();

				int32 ClientStageID = StageDataStore.GetClientStageId(DeferredServerStageID);
				int32 ActID = StageDataStore.GetActIdByClientStageId(ClientStageID);
				EStageDifficulty StageDifficulty = StageDataStore.GetStageDifficulty(DeferredServerStageID);

				StageDataStore.RequestGetActInfoAndChangeChapter(ActID, StageDifficulty, true, DeferredServerStageID);

				DocChapter->SetShowChapterEventScene(false);
				DocChapter->SetDeferredServerStageID(FServerStageID());
			}
		}
	}

	// �����ʿ��� �κ�� ���ƿ� �Ŀ��� �̰��� �������� ���� ���ϴ� �κ� ������� ���� �� ��. ����ȭ���̵�, �������̵�.
	LobbySceneManager.CallDeferredChangeLobbyScenes();

	if (LobbyInventory)
	{ // ���� �� �κ� ���� ���������� ���� �������� ���� �������� ���� ���� üũ. �ٷ� ������ ���� ���� �� ������ �����Ͱ� �� �� �����Ƿ� �� �������� üũ��.
		LobbyInventory->ConditionalCheckForBetterEquipSuggest();
	}

	if (!IsInPreRenderPhase() && !IsFirstTimePreRenderPending()
		&& UB2UIManager::GetInstance()->GetCurrUIScene() != EUIScene::Title)
	{ // Pre-render �� ������ ���� ��κ��� ��� (��ġ �� ���ʰ� �ƴ϶��) ���� StartPlay �� WaitForLoadingScreen ��� ���⼭..
		WaitForLoadingScreen();
	}

	// Initialize InvitationGame
	ClearInvitationGameInfo(EB2GameMode::PVP_Tag);
	ClearInvitationGameInfo(EB2GameMode::Raid);
}

void AB2LobbyGameMode::OnLoadingScreenFinished()
{
	// Ư���� ó������ ������ �ε� �������� ���ư��� ���߿� �� �� �ִ�.. �ܺ� ��⿡ ���� �˾��� �־ �� Ÿ�̹��� ����� ��.
	bLobbyLoadingScreenCompleted = true;
}

void AB2LobbyGameMode::BeginDestroy()
{
	if(GetWorld() && GetWorldTimerManager().IsTimerActive(WaitInventorySyncTimeHandler))
		GetWorldTimerManager().ClearTimer(WaitInventorySyncTimeHandler);

	Super::BeginDestroy();
}

void AB2LobbyGameMode::BattleStart()
{
	LobbySceneManager.BattleStart();
}
void AB2LobbyGameMode::SetDesiredPCClassEnum()
{
	CurrentPCClassEnum = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
}

void AB2LobbyGameMode::SubscribeEvents()
{
//	if (bEventsSubscribed)
//		return;
//
//	Super::SubscribeEvents(); // Super �ʿ� �ʿ���� �͵鵵 ������ ���� �͵� ����.
//
//	Issues.Add(StartGameStageFromLobbyClass<int32, EStageDifficulty, const TArray<EStageBuffType>&, bool>::GetInstance().Subscribe2(
//		[this](int32 i, EStageDifficulty diff, const TArray<EStageBuffType>& sbt, bool bskipstory)
//		{
//			this->RequestStartGameStageWithSelectedPC(i, diff, sbt, bskipstory);
//		}
//	));
//	Issues.Add(StartRepeatBattleStageClass<int32, EStageDifficulty>::GetInstance().Subscribe2(
//		[this](int32 i, EStageDifficulty StageDifficulty)
//		{
//			this->RequestStartRepeatBattleWithSelectedPC(i, StageDifficulty);
//		}
//	));
//	Issues.Add(LobbyLoginAndStartGameClass<const FString&, const FString&>::GetInstance().Subscribe2(
//		[this](const FString& sa, const FString& sb)
//		{
//			this->LoginAndStartGame(sa, sb);
//		}
//	));
//	Issues.Add(LobbyCreateAccountClass<const FString&, const FString&>::GetInstance().Subscribe2(
//		[this](const FString& sa, const FString& sb)
//		{
//			this->CreateAccount(sa, sb);
//		}
//	));
//	Issues.Add(LobbySetPCSelectionClass<EPCClass, EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass MainChar, EPCClass SubChar)
//		{
//			this->SetPCSelection(MainChar, SubChar);
//		}
//	));
//	Issues.Add(LobbySetMainPCClassClass<EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass MainChar)
//		{
//			this->SetMainPCClass(MainChar);
//		}
//	));
//	Issues.Add(LobbyCharacterIntroSetPCSelectionClass<EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass Char)
//		{
//			this->SetCharacterIntroModePCSelection(Char);
//		}
//	));
//	Issues.Add(LobbyProceedOnCharSelClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ProceedOnCharacterSelection();
//		}
//	));
//	Issues.Add(LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Subscribe2(
//		[this](EHeroMgmtSubMode sm)
//		{
//			this->EnterHeroMgmtMode(sm);
//		}
//	));
//	Issues.Add(LobbyQuitHeroMgmtModeClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->QuitHeroMgmtMode();
//		}
//	));
//	Issues.Add(LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass c)
//		{
//			this->SetHeroMgmtModePCSelection(c);
//		}
//	));
//	Issues.Add(LobbySetCombatEntryChapterNumClass<int32, EStageDifficulty>::GetInstance().Subscribe2(
//		[this](int32 i, EStageDifficulty d)
//		{
//			this->SetCombatEntryChapterNum(i, d);
//		}
//	));
//	Issues.Add(LobbyStartMatchClass<bool>::GetInstance().Subscribe2(
//		[this](bool IsRequestMatchMaking)
//		{
//			this->StartLobbyMatch(IsRequestMatchMaking);
//		}
//	));
//	Issues.Add(GoGameStageInfoFromLobbyClass<FServerStageID, FRepeatBattleStateSet>::GetInstance().Subscribe2(
//		[this](FServerStageID ServerStageId, FRepeatBattleStateSet RepeatBattleSet)
//		{
//			this->GoGameStageInfo(ServerStageId, RepeatBattleSet);
//		}
//	));
//	Issues.Add(GoGameStageInfoLastReachedFromLobbyClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->GoGameStageInfoLastReached();
//		}
//	));
//	Issues.Add(GameStageCheckForArinConsultingClass<FServerStageID>::GetInstance().Subscribe2(
//		[this](FServerStageID ServerStageId)
//		{
//			this->GameStageCheckForArinConsulting(ServerStageId);
//		}
//	));
//	Issues.Add(LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass pc)
//		{
//			this->UpdateCharacterParts(pc);
//		}
//	));
//	Issues.Add(LobbyChangeSceneClass<ELobbyScene>::GetInstance().Subscribe2(
//		[this](ELobbyScene ToChangeScene)
//		{
//			this->ChangeLobbyScene(ToChangeScene);
//		}
//	));
//	Issues.Add(LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Subscribe2(
//		[this](EUIScene Scene)
//		{
//			this->ChangeLobbyScene(Scene);
//		}
//	));
//	Issues.Add(LobbyChangeToChapterSceneByUISceneClass<int32>::GetInstance().Subscribe2(
//		[this](int32 ChangeChapterNum)
//		{
//			this->ChangeLobbyScene(EUIScene::Chapter, ChangeChapterNum); /* specially for chapter scene.. */
//		}
//	));
//	Issues.Add(LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Subscribe2(
//		[this](ELobbyInvenItemOpMode m)
//		{
//			this->EnterItemOpDirectingView(m);
//		}
//	));
//	Issues.Add(LobbyQuitItemOpDirectingViewClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->QuitItemOpDirectingView();
//		}
//	));
//	Issues.Add(LobbyEnterPVPMainMenuClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->EnterPVPMainMenu();
//		}
//	));
//	Issues.Add(LobbyAnyOtherUpdateOnSetAccountClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->AnyOtherUpdateOnSetAccount();
//		}
//	));
//	Issues.Add(LobbyEnterTeamMatchMenuClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->EnterTeamMatchMenu();
//		}
//	));
//	Issues.Add(LobbyEnterControlMenuClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->EnterControlMenu();
//	}
//	));	
//	Issues.Add(LobbyUpdateChapterDataClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->UpdateChapterData();
//		}
//	));
//	Issues.Add(EnterTMHeroChangeMenuClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->EnterTMChageHeroMod();
//		}
//	));
//	Issues.Add(HandleServerError910Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorNotEnoughGems();
//		}
//	));
//	Issues.Add(HandleServerError911Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorNotEnoughGolds();
//		}
//	));
//	Issues.Add(HandleServerError919Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorNotEnoughDarkCystal();
//		}
//	));
//	Issues.Add(HandleServerError932Class<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorNotEnoughFamePoint();
//	}
//	));
//	Issues.Add(HandleServerError4032Class<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorNotEnoughStageAutoClearTicket();
//	}
//	));
//	Issues.Add(HandleServerError1012Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorWrongCharacterNickname();
//		}
//	));
//	Issues.Add(HandleServerError4207Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorNotEnoughHeroPiece();
//		}
//	));
//	Issues.Add(HandleServerError7413Class<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->ErrorUserInRaidPenalty();
//		}
//	));
//	Issues.Add(HandleServerError4015ByRoomJoinClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorInventoryIsFull_RoomJoin();
//	}
//	));
//	Issues.Add(HandleServerError4015ForSelectCharacterClass<TArray<b2network::B2InventoryCountServerInfoPtr>>::GetInstance().Subscribe2(
//		[this](TArray<b2network::B2InventoryCountServerInfoPtr> InStageInfo)
//	{
//		this->ErrorInventoryIsFull_RoomJoin(InStageInfo);
//	}
//	));
//	Issues.Add(DeliveryRoomJoinErrorClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorRoomJoin();
//	}
//	)); 
//	Issues.Add(DeliveryRaidMatchmakingErrorClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorRaidMatchMaking();
//	}
//	));
//	Issues.Add(HandleServerError8005Class<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorChannelRoomClose();
//	}
//	));
//	Issues.Add(ErrorOpenWebEmptyURLClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->ErrorOpenWebEmptyURL();
//	}
//	));
//	Issues.Add(ReceivedNotifyInviteMessageClass<const FString&, const FString&, const FString&, int32, int32>::GetInstance().Subscribe2(
//		[this](const FString& Inviter, const FString& InviteKey, const FString& RoomId, int32 mapId, int32 difficulty)
//		{
//			this->NotifyRoomInviteMessage(Inviter, InviteKey, RoomId, mapId, difficulty);
//		}
//	));
//	Issues.Add(ReceivedKickUserClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->HandleNotifyKickUser();
//		}
//	));
//	Issues.Add(ReceivedNoResponsePrerequisiteToStartClass<>::GetInstance().Subscribe2(
//		[this]()
//		{
//			this->HandleNotifyNoResponsePrerequisiteToStart();
//		}
//	));
//	Issues.Add(ReceivedBattleGradeInfoClass<uint32, int32, int32>::GetInstance().Subscribe2(
//		[this](uint8 NetId, int32 BattleGrade, int32 BattleStarCount)
//		{
//			this->NetAckBattleGradeInfo(NetId, BattleGrade, BattleStarCount);
//		}
//	));
//	Issues.Add(ReceivedOtherTokenClass<uint32, const FString&>::GetInstance().Subscribe2(
//		[this](uint32 InNetId, const FString& MatchToken)
//		{
//			this->SetOtherToken(InNetId, MatchToken);
//		}
//	));
//	Issues.Add(FindAccountRequestEventClass<int64, EPCClass, EPCClass, EPCClass>::GetInstance().Subscribe2(
//		[this](int64 AccountID, EPCClass PCClass1 = EPCClass::EPC_End, EPCClass PCClass2 = EPCClass::EPC_End, EPCClass PCClass3 = EPCClass::EPC_End)
//		{
//			this->RequsetFindAccount(AccountID, PCClass1, PCClass2, PCClass3);
//		}
//	));
//	Issues.Add(LobbyHeroTransientEffectBeginClass<const int32>::GetInstance().Subscribe2(
//		[this](const int32 Index)
//		{
//			this->HeroTransientEffectBegin(Index);
//		}
//	));
//	Issues.Add(UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Subscribe2(
//		[this](EPCClass EPC, bool PopupShow)
//		{
//			this->OnUpdateCombatPower(EPC, PopupShow);
//		}
//	));
//	Issues.Add(ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Subscribe2(
//		[this](EPCClass EPC)
//		{
//			this->OnShowBattleScoreNotify(EPC);
//		}
//	));
//	Issues.Add(DeliveryOpenDuelRewardBoxClass<FB2OpenDuelRewardBoxPtr>::GetInstance().Subscribe2(
//		[this](FB2OpenDuelRewardBoxPtr RewardInfo)
//		{
//			this->ModRewardOpenPopup(RewardInfo);
//		}
//	));
//	Issues.Add(DeliveryGetDuelRewardBoxInfoClass<FB2GetDuelRewardBoxInfoPtr>::GetInstance().Subscribe2(
//		[this](FB2GetDuelRewardBoxInfoPtr BoxGradeInfo)
//		{
//			this->ModRewardBoxListInfo(BoxGradeInfo);
//		}
//	));
//	Issues.Add(DeliveryGetDuelRewardBoxGainableListClass<FB2GetDuelRewardBoxGainableListPtr>::GetInstance().Subscribe2(
//		[this](FB2GetDuelRewardBoxGainableListPtr BoxInfo)
//		{
//			this->ModRewardBoxDetailInfo(BoxInfo);
//		}
//	));
//	Issues.Add(ReceiveRoomPlayerTeamInfoClass<TMap<uint32, uint8>&>::GetInstance().Subscribe2(
//		[this](TMap<uint32, uint8>& TeamInfos)
//	{
//		this->NetReceiveMatchPlayerTeamInfo(TeamInfos);
//	}
//	));
//	Issues.Add(PlayLobbyBgmClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->PlayMainLobbyBgm();
//	}
//	));
//	Issues.Add(OnChangUISceneClass<EUIScene>::GetInstance().Subscribe2(
//		[this](EUIScene UIScene)
//	{
//		this->OnChangeUIScene(UIScene);
//	}
//	));
//	Issues.Add(OnChangUISceneBackClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->OnChangeUISceneBack();
//	}
//	));
//	Issues.Add(DeliveryResponseGetHallOfFameClass<FB2ResponseGetHallOfFamePtr>::GetInstance().Subscribe2(
//		[this](FB2ResponseGetHallOfFamePtr Message)
//	{
//		this->ResponseHallOfFameInfo(Message);
//	}
//	));
//	Issues.Add(DeliveryResponsePraiseTargetRankerClass<FB2ResponsePraiseTargetRankerPtr>::GetInstance().Subscribe2(
//		[this](FB2ResponsePraiseTargetRankerPtr Message)
//	{
//		this->ResponsePraiseTargetRanker(Message);
//	}
//	));
//
//	Issues.Add(EnterCostumePageClass<ECostumePage>::GetInstance().Subscribe2(
//	[this](ECostumePage eCostumePageType)
//	{
//		this->EnterCostumeUIScene(eCostumePageType);
//	}
//	));
//
//    Issues.Add(StartPVPReplayClass<FString, int32, int32>::GetInstance().Subscribe2(
//        [this](FString url, int32 sizeCompressed, int32 sizeUncompressed)
//        {
//            FModReplayInfo ReplayInfo{ url, sizeCompressed, sizeUncompressed };
//            StartBladeIIPVPReplay(this, ReplayInfo);
//        }
//    ));
//
//	Issues.Add(DeliveryCheckReachPowerClass<FB2ResponseCheckReachPowerPtr>::GetInstance().Subscribe2(
//		[this](FB2ResponseCheckReachPowerPtr Message)
//	{
//		this->ResponseCheckReachPower(Message);
//	}
//	));
//	Issues.Add(DeliveryReceiveFairyGiftClass<FB2ReceiveFairyGiftPtr>::GetInstance().Subscribe2(
//		[this](const FB2ReceiveFairyGiftPtr& pReceiveFairyGift)
//	{
//		this->ResponseReceiveFairyGift(pReceiveFairyGift);
//	}
//	));
//	Issues.Add(UpdateFairyGiftStateClass<>::GetInstance().Subscribe2(
//		[this]()
//	{
//		this->UpdateFairyGiftState();
//	}
//	));
//
bEventsSubscribed = true;
}

void AB2LobbyGameMode::ErrorNotEnoughGems()
{
	ShortageMGR->PopupGoToShopForGemMessage();
}

void AB2LobbyGameMode::ErrorNotEnoughGolds()
{
	ShortageMGR->PopupGoToShopForGoldMessage();
}

void AB2LobbyGameMode::ErrorNotEnoughHeroPiece()
{
	ShortageMGR->PopupGoToHeroTowerMessage();
}

void AB2LobbyGameMode::ErrorNotEnoughStageAutoClearTicket()
{
	ShortageMGR->PopupGotoShopForAutoClearTicket();
}

void AB2LobbyGameMode::ErrorNotEnoughDarkCystal()
{
	ShortageMGR->PopupGotoShopForDarkCrystalMessage();
}

void AB2LobbyGameMode::ErrorNotEnoughFamePoint()
{
	ShortageMGR->PopupGotoShopForFamePointMessage();
}

void AB2LobbyGameMode::ErrorUserInRaidPenalty()
{
	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_LobbyMain);
}

void AB2LobbyGameMode::ErrorInventoryIsFull_RoomJoin()
{
	EPCClass PCClass = EPCClass::EPC_End;
	
	if (BladeIIGameImpl::GetClientDataStore().GetWaitInventorySync()) return; // ��ũ�߿� ���� ���� ��� ����

	for (int32 i = 0; i < GetMaxPCClassNum(); i++)
	{
		if (UB2LobbyInventory::IsInventoryFull(EItemInvenType::EIIVT_United, IntToPCClass(i)))
		{
			PCClass = IntToPCClass(i);
			break;
		}
	}

	GetWorldTimerManager().ClearTimer(WaitInventorySyncTimeHandler);

	if (PCClass == EPCClass::EPC_End)
	{
		UE_LOG(LogBladeII, Log, TEXT("mismatch server-client inventroy data. need refresh"));

		GetWorldTimerManager().SetTimer(WaitInventorySyncTimeHandler, this, &AB2LobbyGameMode::ReleaseWaitInventorySync, 5.0f);

		BladeIIGameImpl::GetClientDataStore().SetWaitInventorySync(true);

		for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
		{
			BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(IntToPCClass(i));
		}

		return;
	}

	B2UIUtil::OpenInventoryFullPopup(PCClass, FMsgPopupOnClick::CreateLambda([this, PCClass]() {
		LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(PCClass);
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
		LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(EItemInvenType::EIIVT_Weapon, false);
	}));
}

void AB2LobbyGameMode::ErrorInventoryIsFull_RoomJoin(TArray<b2network::B2InventoryCountServerInfoPtr> InInvenInfo)
{
	EPCClass PCClass = EPCClass::EPC_End;

	if (BladeIIGameImpl::GetClientDataStore().GetWaitInventorySync()) return; // ��ũ�߿� ���� ���� ��� ����

	if (InInvenInfo.Num() > 0)
	{
		PCClass = SvrToCliPCClassType(InInvenInfo[0]->character_type);
	}

	if (PCClass == EPCClass::EPC_End)
	{
		UE_LOG(LogBladeII, Log, TEXT("mismatch server-client inventroy data(ErrorInventoryIsFull_RoomJoin)."));
		return;
	}

	auto InventoryPopUp = B2UIUtil::OpenInventoryFullPopup(PCClass);
	if (InventoryPopUp.IsValid() && InInvenInfo.Num() > 0)
	{
		InventoryPopUp->SetInventoryItemCount(InInvenInfo[0]->count, InInvenInfo[0]->max_count);
	}

	//B2UIUtil::OpenInventoryFullPopup(PCClass, FMsgPopupOnClick::CreateLambda([this, PCClass]() {
	//	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(PCClass);
	//	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
	//	LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(EItemInvenType::EIIVT_Weapon, false);
	//}));
}

void AB2LobbyGameMode::ReleaseWaitInventorySync()
{
	BladeIIGameImpl::GetClientDataStore().SetWaitInventorySync(false);
}

void AB2LobbyGameMode::ErrorRoomJoin()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ErrorRoomJoin"));

	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		DocRaid->ClearRaidJoinData();
		DocRaid->ClearRaidBattleData();
		DocRaid->SetIsReceiveInvite(false);
	}

	if (auto* UIRaidMain = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMain>(UIFName::Raid))
	{
		UIRaidMain->ButtonFree();
	}
}

void AB2LobbyGameMode::ErrorRaidMatchMaking()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ErrorRaidMatchMaking"));

	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		DocRaid->ClearRaidJoinData();
		DocRaid->ClearRaidBattleData();
		DocRaid->SetIsReceiveInvite(false);
	}

	if (auto* UIRaidMain = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMain>(UIFName::Raid))
	{
		UIRaidMain->ButtonFree();
	}
}

void AB2LobbyGameMode::ErrorOpenWebEmptyURL()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ErrorOpenWebEmptyURL")),
		0.f,
		true,
		true
	);
}

void AB2LobbyGameMode::ErrorChannelRoomClose()
{
	if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		if (DocRaid->GetIsRequestPartyRematch())
		{
			DocRaid->ClearPartyRetryInfo();
			DocRaid->SetIsRequestPartyRematch(false);
		}
	}

	GoToVillage();
}

void AB2LobbyGameMode::ModRewardOpenPopup(FB2OpenDuelRewardBoxPtr RewardBoxInfo)
{
	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
	{
		auto RewardPopup = UIManager->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
		RewardPopup->AddRewardItems(RewardBoxInfo->received_rewards);
		RewardPopup->ShowRewardPopup(true);
	}
}

void AB2LobbyGameMode::ModRewardBoxListInfo(FB2GetDuelRewardBoxInfoPtr BoxInfo)
{
	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
	{
		auto BoxGradeUI = UIManager->OpenUI<UB2UIModRewardBoxGradeInfo>(UIFName::ModRewardBoxList);
		int32 WinCount = 0;				//�������� ���

		if (BoxGradeUI)
		{
			for (auto El : BoxInfo->reward_boxes)
			{
				WinCount += El->win_count;
				BoxGradeUI->SetBoxGrade(BoxInfo->duel_mode,El->grade, WinCount);
			}
		}
	}
}

void AB2LobbyGameMode::ModRewardBoxDetailInfo(FB2GetDuelRewardBoxGainableListPtr BoxInfo)
{
	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
	{
		auto BoxGradeUI = UIManager->OpenUI<UB2UIModRewardBoxDetailInfo>(UIFName::ModRewardBoxDetail);

		if (BoxGradeUI)
		{
			BoxGradeUI->SetPanelClear();
			BoxGradeUI->SetRewardCount(BoxInfo->reward_item_count);
			BoxGradeUI->GradeEvent(BoxInfo->grade);
			for (int32 i = 0 ; i <BoxInfo->reward_items.Num() ; ++i)
			{
				BoxGradeUI->SetRewardIcon(BoxInfo->reward_items[i]->reward_id, BoxInfo->reward_items[i]->min_count, BoxInfo->reward_items[i]->max_count, i);
			}
		}
	}
}

void AB2LobbyGameMode::ErrorWrongCharacterNickname()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterNickNameEditWrongName")),
		0.f,
		true,
		true
		);
}

void AB2LobbyGameMode::CheckAndPopupKakaoPromotion()
{
	// ���θ�� ����. �̰� �Ϲ������� ����ϴ� BeginPlay ���� ���� ������ �ε� Ÿ�̹��� �ƴ϶� �񵿱� GMP �ε� ������ ������ ��ٷȴٰ� ����. 
	// �������� �ٷ� ���� �� �ƴϹǷ� ���� ������ ���ŷ�� �� ����.
	if (
		bLobbyLoadingScreenCompleted && 
		!IsViewPromotion() && // ���� ���� �� ������ ����.. �׳� static ���� ���� �÷��ٵ�.. 
#if PLATFORM_ANDROID && PLATFORM_IOS
		IsInitPurchases() &&	// �ξ� ���� �ʱ�ȭ �� ���θ�� �� ��� �ֵ���
#endif
		(UB2UIManager::GetInstance()->GetCurrUIScene() != EUIScene::Title) && // Ÿ��Ʋ UI �� ��� ���ƴٰ� �ٷ� �ٸ� ���� �ε��ϰ� �Ǵ� ��찡 �־ �н�.
		BladeIIGameImpl::GetClientDataStore().HasValidUserNickName() // �α��ε��� �ʾҴٸ� �Ƹ� Title UI �� �Ѿ�� ���߰����� �߰� ���� ��ġ�μ�..
		)
	{
		B2P_ShowKakaoPromotionView();
		SetViewPromotion(true);
		bEnableLobbyScreenRotate = true;
	}
}

void AB2LobbyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

//	if (IsInPreRenderPhase() || IsFirstTimePreRenderPending())
//	{ // Pre-render �� �����ؼ� ���� �� �ε� ȭ�� �ݴ� �� ������Ű�� �ǹǷ� ���⼭ ó��. �ε� ������ ���� ����
//		ConditionalWaitForLoadingScreen();
//	}
//
//	TickTimeTouchAction(DeltaSeconds);
//
//	if (LobbyUIManager && !LobbyUIManager->IsPendingKill())
//	{
//		LobbyUIManager->UpdateManual(DeltaSeconds);
//	}
//
//#if !UE_BUILD_SHIPPING
//	extern bool GTest_GenerateDummyChatMessage;
//	if (GTest_GenerateDummyChatMessage)
//	{
//		DevOnlyGenerateDummyChat(DeltaSeconds);
//	}
//#endif
//	CheckAndPopupKakaoPromotion();
}

void AB2LobbyGameMode::NotifyActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject)
{
	LobbySceneManager.OnLobbyGMActorCustomEvent(OptionalEventName, OptionalNotifyingObject);
}

void AB2LobbyGameMode::SetEnableClickEvents(bool bEnable)
{
	//auto* PlayerController = UGameplayStatics::GetLocalPlayerController(GetWorld());
	//if (PlayerController)
	//	PlayerController->bEnableClickEvents = PlayerController->bEnableTouchEvents = bEnable;
}

void AB2LobbyGameMode::RealtimeUpdateUIDoc()
{
	Super::RealtimeUpdateUIDoc();

	// �̺�Ʈ�� ���� ������ ĳġ�ؾ� �Ѵٸ� ����� �־ ���� �δ��� ���� �� ������ ���귮�� ���� ���� ������ ���� �ʵ��� �Ѵ�.
	// ���װ� ����� �ص� UI ǥ�ð� ��� �߸��Ǵ� �����̴�.
	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore ����
	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{
	//	const EPCClass PCClassEnum = IntToPCClass(PCI);
	//	const int32 PCLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(PCClassEnum);
	//	const int32 ExpPercent = (int32)BladeIIGameImpl::GetLocalCharacterData().GetCharacterExpPercent(PCClassEnum);
	//	UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCI);
	//	if (DocHero)
	//	{
	//		DocHero->SetCurrentLevel(PCLevel);
	//		DocHero->SetCurrentExpPercent(ExpPercent);
	//	}
	//}

	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	if (DocUser)
	{
		DocUser->SetGold(BladeIIGameImpl::GetClientDataStore().GetGoldAmount());
		DocUser->SetGem(BladeIIGameImpl::GetClientDataStore().GetGemAmount());
		DocUser->SetSocialPoint(BladeIIGameImpl::GetClientDataStore().GetSocialPoint());
		DocUser->SetFamePoint(BladeIIGameImpl::GetClientDataStore().GetFamePoint());
	}
}

void AB2LobbyGameMode::OnUpdateCombatPower(EPCClass UpdatedClass, bool PopupShow)
{
	if (UpdatedClass == EPCClass::EPC_End)
		return;

	UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(int32(UpdatedClass));
	if (DocHero)
	{
		// ���⼭ ���� CombatStat ������ �Լ����� ���귮�� �����Ƿ� �̺�Ʈ �������� ȣ���ϵ���
		DocHero->SetAttackPower(CombatStatEval::GetPCAttack(UpdatedClass));
		DocHero->SetDefensePower(CombatStatEval::GetPCDefense(UpdatedClass));
		DocHero->SetHealthPoint(CombatStatEval::GetPCHealth(UpdatedClass));

		// �������� �߰����� �˾� �ִϸ��̼� ó��
		int32 BeforeCP = DocHero->GetCombatPower();
		int32 AfterCP = CombatStatEval::GetPCCombatPower(UpdatedClass);
		
		DocHero->SetPreCombatPower(BeforeCP);
		DocHero->SetCombatPower(AfterCP);

		if (BeforeCP != AfterCP && BeforeCP != 0)
		{
			DocHero->SetChangedCombatPower(true);
		}

		if (PopupShow)
		{
			OnShowBattleScoreNotify(UpdatedClass);
		}
	}
}

void AB2LobbyGameMode::OnShowBattleScoreNotify(EPCClass EPC)
{ // UI ��� ���������� ��ºκи� ���� �������.
	if (EPC == EPCClass::EPC_End) return;

	UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(int32(EPC));
	if (DocHero)
	{
		int32 BeforeCP = DocHero->GetPreCombatPower();
		int32 AfterCP = DocHero->GetCombatPower();

		if (BeforeCP != AfterCP && BeforeCP != 0)
		{
			// ������ ���� �˾� ���
			UB2UIBattleScoreNotify* BattleScoreNotifyUI = UB2UIManager::GetInstance()->OpenUI<UB2UIBattleScoreNotify>(UIFName::BattleScoreNotify);
			if (BattleScoreNotifyUI != nullptr)
			{
				BattleScoreNotifyUI->SetAmount(AfterCP, BeforeCP, EPC);
			}
		}
	}
}

void AB2LobbyGameMode::NotifyRoomInviteMessage(const FString& Inviter, const FString& InviteKey, const FString& RoomId, int32 MapId, int32 Step)
{
	// ���������� �����ϱ� ���ؼ� ���� ��û�� �Ѵ�.
	data_trader::Retailer::GetInstance().RequestGetRaid();

	if (CanReceiveGameInvite(Inviter, EB2GameMode::Raid))
	{
		if(UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
		{
			DocRaid->SetRaidInvitationInfo(Inviter, RoomId, MapId, Step, FDateTime::UtcNow());
			OnReceiveInvitation(Inviter, EB2GameMode::Raid);
		}
	}
}

void AB2LobbyGameMode::SetPCSelection(EPCClass InMainPCClass, EPCClass InSubPCClass)
{
//	BII_CHECK(InMainPCClass != InSubPCClass);
	if (InMainPCClass == InSubPCClass && InMainPCClass != EPCClass::EPC_End)
	{
#if WITH_EDITOR && !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("����ĳ���Ϳ� �±� ĳ���͸� �����ϰ� ������ �� �����ϴ�. ��ǻ�Ͱ� �� �����Ѵ�."))
			));
#endif 
		return;
	}

	/*BII_CHECK(InMainPCClass != EPCClass::EPC_End);
	BII_CHECK(InSubPCClass != EPCClass::EPC_End);
	if (InMainPCClass == EPCClass::EPC_End || InSubPCClass == EPCClass::EPC_End)
		return;*/

	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetPCSelection"));

	BladeIIGameImpl::GetLocalCharacterData().SetMainPlayerClass(InMainPCClass);
	BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(InSubPCClass);

	//UI��ť��Ʈ�� MainChar�� SubChar�� �˷�����
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		DocBattle->SetIsCurPCMain(true); // �κ񿡼��� ������ ������ current �� main �� ������ ����. �ΰ��ӿ����� �޶��� �� �ִ�.
		DocBattle->SetCurPCClass(PCClassToInt(InMainPCClass));
		DocBattle->SetTagPCClass(PCClassToInt(InSubPCClass));
	}
		
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{ // ���� ���� ȭ���� ��� ������ �ʿ��� ����
		DocBS->SetBattleDifficulty(BladeIIGameImpl::GetStageDataStore().GetRelativeStageDifficulty(DocBS->GetServerStageId(), InMainPCClass, InSubPCClass)
			);
	}

	// �̰� ��ü�� UI �ʿ��� �Ҹ��� �Ŷ� �� ��ȯ�Ǵ� ���̱� �ѵ� ���ϰ� �ٽ� UI ������ ���� ����.
	LobbyPCSelectionChangedClass<EPCClass>::GetInstance().Signal(InMainPCClass);
	SetHeroMgmtModePCSelection(InMainPCClass);
	LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Signal(InMainPCClass, InSubPCClass);
}

bool AB2LobbyGameMode::SetMainPCClass(EPCClass InMainPCClass)
{
	BII_CHECK(InMainPCClass != EPCClass::EPC_End);
	if (InMainPCClass == EPCClass::EPC_End)
		return false;

	if (InMainPCClass == BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass())
		return false;

	if (InMainPCClass == BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass())
		SetPCSelection(InMainPCClass, BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
	else
		SetPCSelection(InMainPCClass, BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());

	return true;
}

bool AB2LobbyGameMode::SetMainPCClassBP(EPCClass InMainPCClass)
{
	return SetMainPCClass(InMainPCClass);	
}

void AB2LobbyGameMode::SceneForceUpdate()
{
	LobbySceneManager.ForceUpdate();
}

void AB2LobbyGameMode::SetCharacterIntroModePCSelection(EPCClass InPCClass)
{
	LobbyCharIntroSelectClass<EPCClass>::GetInstance().Signal(InPCClass);
}

void AB2LobbyGameMode::RequsetFindAccount(int64 InAccountId, EPCClass PCClass1, EPCClass PCClass2, EPCClass PCClass3)
{
	auto SomeDoc = UB2UIDocHelper::GetDocSome();

	if (!SomeDoc)
		return;

	SomeDoc->SetFindUserAccountID(InAccountId);

	data_trader::Retailer::GetInstance().RequestFindAccount(InAccountId, PCClass1, PCClass2, PCClass3);
}

void AB2LobbyGameMode::HeroTransientEffectBegin(const int32 Index)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HeroTransientEffectBegin"));

	LobbySceneManager.HeroTransientEffectBegin(Index);
}

bool AB2LobbyGameMode::HasMatchStarted() const
{
	if (GetMatchState() == MatchState::EnteringMap || GetMatchState() == MatchState::WaitingToStart)
		return false;

	return (GetMatchState() == NetMatchState::RequestMatchMaking || 
		GetMatchState() == NetMatchState::WaitToMeetCondition ||
		GetMatchState() == NetMatchState::PreparePlayers);
}

void AB2LobbyGameMode::SetMatchState(FName NewState)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetMatchState"));
	
	Super::SetMatchState(NewState);
}

void AB2LobbyGameMode::HandleSuccessToMatchmake(data_trader::ChannelPtr ch, uint32 peer_role, const TArray<FB2RoomParticipantPtr>& participants)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleSuccessToMatchmake"));

	Super::HandleSuccessToMatchmake(ch, peer_role, participants);

	LobbySceneManager.NotifySuccessMatchMake(IsHost());
}

void AB2LobbyGameMode::HandleRequestMatchMaking()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleRequestMatchMaking"));

	AllPeerInfos.Empty();
	LobbySceneManager.HandleRequestMatchMaking();

	if (auto* UIDocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		UIDocLobbyMatch->ClearRemotePlayerInfoData();

	Super::HandleRequestMatchMaking();
}

void AB2LobbyGameMode::HandleNotifyLeave(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleNotifyLeave"));

	Super::HandleNotifyLeave(NetId);
	
	if (MatchState == NetMatchState::StartResult ||
		MatchState == MatchState::LeavingMap ||
		MatchState == MatchState::Aborted)
		return;

	if (MatchState != MatchState::InProgress && MatchState != NetMatchState::PreparePlayers)
	{
		RemovePeerPlayer(NetId);
		RemotePlayerInfoData.Remove(NetId);
	}
	else if (MatchState == NetMatchState::PreparePlayers)
	{
		// 180213_YJ ���̵� ��Ī ī��Ʈ�϶� Ż�� �߰� ó��
		if (auto* UIRaidMain = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMain>(UIFName::Raid))
		{
			SendLeavePlayerOnlyRaid(NetId);
		}
	}

	LobbySceneManager.HandleNotifyLeave(NetId);
}

void AB2LobbyGameMode::SetMyPlayerInfoData(uint32 Netid, const FB2FindAccountInfoPtr& info)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetMyPlayerInfoData"));

	Super::SetMyPlayerInfoData(Netid, info);

	SetMatchState(NetMatchState::WaitToMeetCondition);

	LobbySceneManager.SetMyPlayerInfoData(Netid, info);
}

void AB2LobbyGameMode::SetRemotePlayerInfoData(uint32 Netid, int64 AccountId, const FB2FindAccountInfoPtr& info)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetRemotePlayerInfoData"));

	// ��Ƽ��(ȣ��Ʈ)�� ��� ���� ������ �������� �����ִ� ������� ���õ� ĳ���͸� �ѱ��.
	// �ش� ��Ŷ�� ���޹��� ������ ������ �������� �����ϰ�, �ڽ��� ������ ĳ���͸� ��ü �������� �˸���(��ε�ĳ����)
	if (GetPeerType() == PEER_SUPER)
	{
		packet::ByteStream	payload;

		// Host
		payload << GetMyNetId();

		// ĳ���� ����
		payload << static_cast<uint8>(RemotePlayerInfoData.Num() + 1) <<
			GetMyNetId() << (uint8)BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
		for (auto& Elem : RemotePlayerInfoData)
			payload << Elem.Key << (uint8)Elem.Value.GetMainPlayerClass();

		FString encoded_string = packet::FinalizePacket(packet::SEND_ROOM_PLAYERCLASS, GetMyNetId(), 0, packet::ALLBUTME, payload);
		data_trader::ChannelManager::GetInstance().SendMessage(Netid, encoded_string);
	}

	Super::SetRemotePlayerInfoData(Netid, AccountId, info);
	
	EmplacePeerPlayer(Netid);

	LobbySceneManager.SetRemotePlayerInfoData(Netid, info);

	if (AllPeerInfos[Netid].ReservedPCClass != EPCClass::EPC_End)
		SetRemotePlayerClass(Netid, AllPeerInfos[Netid].ReservedPCClass);
}

void AB2LobbyGameMode::SetRoomParticipant(const TArray<FB2RoomParticipantPtr>& Participants)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetRoomParticipant"));

	//BII_CHECK(Participants.Num() <= GetMaxMatchPlayerNum());

	for (auto& Participant : Participants)
	{
		if (Participant->id == GetMyNetId())
			continue;

		SetRemotePlayerInfoData(Participant->id, Participant->account_id, Participant->account_info);
	}
}

void AB2LobbyGameMode::HandleNotifyKickUser()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleNotifyKickUser"));

	LobbySceneManager.HandleNotifyKickUser();

	LeaveRoomIfJoined();
}

void AB2LobbyGameMode::HandleNotifyRoomClose(const FString& RoomToken, int32 RoomType)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleNotifyRoomClose"));

	Super::HandleNotifyRoomClose(RoomToken, RoomType);

	if (MatchState != MatchState::InProgress)
	{
		// �� Ŭ�����ϸ鼭 ��ū �缳���� ���̵忡����.
		if (RoomType == b2network::B2RoomType::BRT_Raid)
		{
			// �ڱ��ڽ� ����
			SetToken(RoomToken);
		}

		SendPrerequisiteToStart(RoomType);
	}
}

void AB2LobbyGameMode::OnHostChanged(int32 RoomType, int32 NewHostId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::OnHostChanged"));
	
	uint32 BeforeHostId = GetHostId();

	Super::OnHostChanged(RoomType, NewHostId);

	LobbySceneManager.OnHostChanged(RoomType, NewHostId);

	if (HasMatchStarted() && (GetMyNetId() == NewHostId))
	{
		// ������ �߰��������. ȣ��Ʈ�� �ƴϾ�����
		// HandleWaitToMeetCondition() ��⼭ �߰� ��������.
		IncreasePeerPlayers(GetMyNetId());

		// ��Ī�������� Host�� �������ϴ� ������ �缳�����ش�.
		for (auto PeerInfo : AllPeerInfos)
			IncreasePeerPlayers(PeerInfo.Key);
	}

	if (MatchState == NetMatchState::PreparePlayers)
	{
		// 180213_YJ ���̵� ��Ī ī��Ʈ�϶� Ż�� �߰� ó��
		if (auto* UIRaidMain = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMain>(UIFName::Raid))
		{
			SendLeavePlayerOnlyRaid(BeforeHostId);
		}
	}

	// ��ȣ��Ʈ ���� ��Ŭ�������� �����ϱ���� ��������� �÷���. �̻��� ȣ��Ʈ �ΰ�޾����� �����.
	if (bWaitPrerequisiteToStartForNonHost)
	{
		NoResponsePrerequisiteToStart();
	}
}


void AB2LobbyGameMode::HandleWaitToMeetCondition()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleWaitToMeetCondition"));

	BII_CHECK(!RoomWhereIAm.IsEmpty());
	BII_CHECK(GetMyNetId() != 0);

	IncreasePeerPlayers(GetMyNetId());
}

void AB2LobbyGameMode::HandlePreparePlayers()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandlePreparePlayers"));

	bWaitPrerequisiteToStartForNonHost = false;

	LobbySceneManager.HandlePreparePlayers();
}

void AB2LobbyGameMode::LeaveRoomIfJoined()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandlePreparePlayers"));

	LobbySceneManager.LeaveRoomIfJoined();

	// �غ�Ȯ�� Ÿ�̸� ����
	ClearWaitPrerequisiteToStartTimeHandler();

	bWaitPrerequisiteToStartForNonHost = false;

	Super::LeaveRoomIfJoined();
}


bool AB2LobbyGameMode::CanRequestRoomCloseReason(ERoomCloseReasonType RoomCloseReason)
{
	B2_SCOPED_TRACK_LOG(TEXT("B2LobbyGameMode::CanRequestRoomCloseReason"));

	if (RoomCloseReason == ERoomCloseReasonType::NONE)
		return false;

	return (RoomCloseReason == GetStartRoomCloseReason() || RoomCloseReason == GetBreakRoomCloseReason());
}

ERoomCloseReasonType AB2LobbyGameMode::GetStartRoomCloseReason()
{
	B2_SCOPED_TRACK_LOG(TEXT("B2LobbyGameMode::GetStartRoomCloseReason()"));

	return LobbySceneManager.GetStartRoomCloseReason();
}

int32 AB2LobbyGameMode::GetMatchModeMapId()
{
	return LobbySceneManager.GetMatchModeMapId();
}

int32 AB2LobbyGameMode::GetMatchModeDifficulty()
{
	return LobbySceneManager.GetMatchModeDifficulty();
}

void AB2LobbyGameMode::HandleFinishLobbyMatch()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::HandleFinishLobbyMatch"));

	// ��Ī �� ������Ʈ���� �������� �����Ͱ� ���󰡰ԵǹǷ� DocLobby�� ��Ī���� ������ ����
	if (auto* UIDocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	{
		UIDocLobbyMatch->SetPeerType(GetPeerType());
		UIDocLobbyMatch->SetRoomId(GetRoomId());
		UIDocLobbyMatch->SetHostId(GetHostId());
		UIDocLobbyMatch->SetRoomToken(GetToken());
		UIDocLobbyMatch->SetMyNetId(GetMyNetId());	
		//UIDocLobbyMatch->ClearRemotePlayerInfoData();

		for (FB2NetMatchPeerInfo& PeerInfo : ReservedPlayerClasses)
		{
			if (PeerInfo.NetId == GetMyNetId())
				UIDocLobbyMatch->SetMyStartIndex(PeerInfo.PlayerStartIndex);
		}

		UIDocLobbyMatch->MatchedRemotePlayerSkillAnims.Empty();
		for (auto& Elem : RemotePlayerInfoData)
		{
			uint8 StartIndex = 0;

			for (FB2NetMatchPeerInfo& PeerInfo : ReservedPlayerClasses)
			{
				if (Elem.Key == PeerInfo.NetId)
					StartIndex = PeerInfo.PlayerStartIndex;
			}

			UIDocLobbyMatch->SetRemotePlayerInfoData(Elem.Key, Elem.Value.GetAccountId(), Elem.Value.GetAccountInfoPtr(), Elem.Value.GetMainPlayerClass(), StartIndex);

			//////////////////////////////			
			{ // �� �ε� ���� SkillAnim �� Pre-load �� ���� ���� ����. �ش� Pre-load ������ ���� �ε��Ǵ� ���Ӹ�忡�� ��ó RemotePlayerInfoData �� ���� ��Ȳ�̶� ���⿡ ������ �д�.
				TArray<ESkillAnimType> SkillAnimTypesOfThisRemote;
				UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(this, Elem.Value.GetMainPlayerClass(),
					///// ���� ���Ӹ��� �Ƹ��� Raid �� Control �� ���ٵ� ���� �� ���� ����. �ȿ� ������ ��� �Ϻ� ��ų�� Ư���ϰ� ����ϴ� GameMode Ÿ�Կ� �ش������� ������ ������ ���µ� �� �Ž����� ����.
					EB2GameMode::Raid,
					/////
					SkillAnimTypesOfThisRemote, &Elem.Value);
				// ���� �ɷ� CombinedID �� ����� �ִ´�.
				for (ESkillAnimType ThisSkillAnimType : SkillAnimTypesOfThisRemote)
				{
					UIDocLobbyMatch->MatchedRemotePlayerSkillAnims.AddUnique(FCombinedPCSkillAnimID(Elem.Value.GetMainPlayerClass(), ThisSkillAnimType));
				}
			}
			//////////////////////////////
		}
	}	

	// ���� :	���� GameMode�� ����Ǹ� NotifyRoomLeave�� ȣ��Ǵµ� �̶� RoomId�� �����ϸ� ������ ���� ������ ��Ŷ�� ������. 
	//			�̸� �����ϱ����� �ݵ�� ���� �ʱ�ȭ�������
	ClearMatchRoom();

	LobbySceneManager.HandleFinishLobbyMatch();
}

void AB2LobbyGameMode::CheckFullPeerRoom()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CheckFullPeerRoom"));

	if (IsMatchMaxPlayerNumOver())
	{
		NoResponsePrerequisiteToStart();
	}

	return Super::CheckFullPeerRoom();
}

void AB2LobbyGameMode::SendPrerequisiteToStart(int32 RoomType)
{
	// ȣ��Ʈ�� ReservedPlayerClasses ����
	FB2PeerInfo MyPeerInfo;
	TArray<FB2PeerInfo> PeerInfos;
	
	MyPeerInfo.NetId = GetMyNetId();
	MyPeerInfo.Class = uint8(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
	MyPeerInfo.PlayerStartIndex = 1;
	
	//if (AllPeerInfos[MyPeerInfo.NetId].PlayerTeamType != ETeamType::ETT_End)
	//	MyPeerInfo.PlayerStartIndex = GetPlayerStartIndexByNetId(MyPeerInfo.NetId);
	if (RoomType == b2network::B2RoomType::BRT_Assault)
	{
		MyPeerInfo.MatchGrade = UB2UIDocHelper::GetDocControl()->GetBattleGrade();
		MyPeerInfo.MatchStar = UB2UIDocHelper::GetDocControl()->GetBattleStarCount();

		if (IsActiveControlTutorial() && AllPeerInfos.Contains(MyPeerInfo.NetId))
		{
			MyPeerInfo.PlayerTeamType = AllPeerInfos[MyPeerInfo.NetId].PlayerTeamType;
		}
	}

	PeerInfos.Add(MyPeerInfo);

	uint8 RemotePlayerStartIndex = 2;

	for (auto& Elem : RemotePlayerInfoData)
	{
		FB2PeerInfo RemotePeerInfo;
		RemotePeerInfo.NetId = Elem.Key;
		RemotePeerInfo.Class = uint8(RemotePlayerInfoData[Elem.Key].GetMainPlayerClass());
		RemotePeerInfo.PlayerStartIndex = RemotePlayerStartIndex;
		
		//if (AllPeerInfos[RemotePeerInfo.NetId].PlayerTeamType != ETeamType::ETT_End)
		//	RemotePeerInfo.PlayerStartIndex = GetPlayerStartIndexByNetId(RemotePeerInfo.NetId);
		if (RoomType == b2network::B2RoomType::BRT_Assault)
		{
			RemotePeerInfo.MatchGrade = Elem.Value.GetAccountInfoPtr()->assault_match_grade;
			RemotePeerInfo.MatchStar = Elem.Value.GetAccountInfoPtr()->assault_match_star_count;
			if (IsActiveControlTutorial() && AllPeerInfos.Contains(RemotePeerInfo.NetId))
			{
				RemotePeerInfo.PlayerTeamType = AllPeerInfos[RemotePeerInfo.NetId].PlayerTeamType;
			}
		}

		PeerInfos.Add(RemotePeerInfo);
		RemotePlayerStartIndex++;
	}
	
	if (RoomType == b2network::B2RoomType::BRT_Assault)
	{
		if (IsActiveControlTutorial())
		{
			GetTutorialControlPlayerInfo(PeerInfos);
		}
		else
		{// ��, ��ġ ����
			GetControlPlayerInfo(PeerInfos);
		}
	}

	for (FB2PeerInfo& Elem : PeerInfos)
	{
		FB2NetMatchPeerInfo NetMatchPeerInfo;
		NetMatchPeerInfo.NetId = Elem.NetId;
		NetMatchPeerInfo.Class = Elem.Class;
		NetMatchPeerInfo.PlayerStartIndex = Elem.PlayerStartIndex;
		NetMatchPeerInfo.PlayerTeamType = (uint8)Elem.PlayerTeamType;
		ReservedPlayerClasses.Add(NetMatchPeerInfo);

		if (AllPeerInfos.Contains(Elem.NetId))
		{
			AllPeerInfos[Elem.NetId].PlayerTeamType = Elem.PlayerTeamType;
		}
	}

	// �ڽ��� �����ϰ� ��Ƽ������ ������ �˸�
	packet::ByteStream	payload;
	FString	strRoomToken(GetToken());

	payload << RoomType << strRoomToken <<
		static_cast<uint8>(ReservedPlayerClasses.Num());

	for (FB2NetMatchPeerInfo& PeerInfo : ReservedPlayerClasses)
	{
		payload << PeerInfo.NetId << PeerInfo.Class << PeerInfo.PlayerStartIndex << (uint8)PeerInfo.PlayerTeamType;
	}

	FString encoded_string = packet::FinalizePacket(packet::SEND_PREREQUISITE_TO_START, GetMyNetId(), 0, packet::ALLBUTME, payload);
	data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);

	bool bStandaloneStart = false;
	const bool IsAlone = ReservedPlayerClasses.Num() == 1;

	if (IsAlone)
	{
#if !UE_BUILD_SHIPPING
		bStandaloneStart |= GetMaxMatchPlayerNum() == 1;
#endif
		bStandaloneStart |= IsForceStartMatch(GetCurrentLobbyScene());
	}

	// Ʃ�丮��
	if (IsActiveControlTutorial())
		bStandaloneStart = true;


	if (bStandaloneStart)
		SetPreparePlayers();

	else
		GetWorldTimerManager().SetTimer(WaitPrerequisiteToStartTimeHandler, this, &AB2LobbyGameMode::NoResponsePrerequisiteToStart, 3.0f);
}

void AB2LobbyGameMode::StartLobbyMatch(bool IsRequestMatchMaking)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::StartLobbyMatch"));

	ClearMatchRoom();
	SetMatchState(IsRequestMatchMaking ? NetMatchState::RequestMatchMaking : NetMatchState::WaitMatchMaking);
}

void AB2LobbyGameMode::StartFinishLobbyMatch()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::StartFinishLobbyMatch"));

	SetMatchState(NetMatchState::FinishLobbyMatch);
}

void AB2LobbyGameMode::HandleNotifyNoResponsePrerequisiteToStart()
{
	// ä�γ�����
	// LeaveRoomIfJoined �κ�� �Ŵ������� ��Īâ �ݾ��ش�.
	LeaveRoomIfJoined();

	// �˾������
	LobbySceneManager.ReceiveNoResponsePrerequisiteToStart();
}

void AB2LobbyGameMode::BreakRoom(int32 InMapId /*= 1*/, int32 InDifficulty /*= 1*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::BreakRoom"));

	if (GetMatchState() != NetMatchState::StartResult)
	{
		// ��Ī�������� ���� �������� üũ
		if (HasMatchStarted())
		{
			if (GetPeerType() == PEER_SUPER)
			{
				// ������ �� �̻� ������ ���ϰ��ϴ� ��Ŷ�� ����
				UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid();
				data_trader::Retailer::GetInstance().RequestRoomClose(GetRoomId(), InMapId, GetBreakRoomCloseReason()); // Ÿ�� ����������.

				// ��� ������ �����Ų��.
				packet::ByteStream	payload;
				FString encoded_string = packet::FinalizePacket(packet::KICK_USER, GetMyNetId(), 0, packet::ALLBUTME, payload);
				data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);				
			}
		}
	}
}

void AB2LobbyGameMode::IncreasePeerPlayers(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::IncreasePeerPlayers"));

	if (GetPeerType() != PEER_SUPER)
	{
		if(GetMyNetId() == NetId)
			EmplacePeerPlayer(NetId);
		return;
	}

	if (MatchState == MatchState::InProgress)
		return;

	EmplacePeerPlayer(NetId);
		
	// ���� ��ã���� üũ
	CheckFullPeerRoom();
}

void AB2LobbyGameMode::NetSendBattleGradeInfo(int32 BattleGrade, int32 BattleStarCount)
{
	// ���� ������ ��� ä�γ� ���
	packet::ByteStream	payload;

	payload << BattleGrade << BattleStarCount;

	FString encoded_string = packet::FinalizePacket(packet::BATTLE_GRADE_INFO, GetMyNetId(), 0, packet::ALLBUTME, payload);
	//SendMessage(encoded_string);
}

void AB2LobbyGameMode::NetSendMatchToken()
{
	packet::ByteStream	payload;
	payload << TokenToPlay;

	FString encoded_string = packet::FinalizePacket(packet::MATCH_TOKEN, GetMyNetId(), 0, packet::ALLBUTME, payload);
	//SendMessage(encoded_string);
}

void AB2LobbyGameMode::NetAckBattleGradeInfo(uint32 NetId, int32 BattleGrade, int32 BattleStarCount)
{
	UB2UIDocHelper::GetDocLobbyMatch()->SetGradeInfoData(NetId, BattleGrade, BattleStarCount);
}

void AB2LobbyGameMode::SetOtherToken(uint32 InNetId, const FString& MatchToken)
{
	if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	{
		DocLobbyMatch->SetTokenData(InNetId, MatchToken);
	}
}

void AB2LobbyGameMode::NetSendMatchPlayerTeamInfo()
{
	packet::ByteStream	payload;

	payload << AllPeerInfos.Num();

	// ĳ���� ����
	for (TPair<uint32, FLobbyMatchPeerInfo>& peerInfo : AllPeerInfos)
		payload << peerInfo.Key << (uint8)peerInfo.Value.PlayerTeamType;

	FString encoded_string = packet::FinalizePacket(packet::ROOM_PLAYER_TEAM_INFO, GetMyNetId(), 0, packet::ALL, payload);
	data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);
}

void AB2LobbyGameMode::NetReceiveMatchPlayerTeamInfo(TMap<uint32, uint8>& TeamInfos)
{
	for (TPair<uint32, uint8>& TeamInfo : TeamInfos)
	{
		EmplacePeerPlayer(TeamInfo.Key);
	}

	LobbySceneManager.ReceiveMatchPlayerTeamInfo();
}

void AB2LobbyGameMode::OnDeliveryRoomJoin(FB2ResponseRoomJoinPtr Message)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::OnDeliveryRoomJoin"));

	if (Message->room_type == b2network::B2RoomType::BRT_Raid)
	{
		OnDeliveryRoomJoin_Raid(Message);
	}
	else
	{
		Super::OnDeliveryRoomJoin(Message);
	}
}

void AB2LobbyGameMode::OnDeliveryRoomJoin_Raid(FB2ResponseRoomJoinPtr Message)
{
	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	auto* UIManager = UB2UIManager::GetInstance();

	const bool InRaidScene = UIManager->IsUIScene(EUIScene::RaidMain);
	if (InRaidScene)
	{
		if (DocRaid && DocRaid->GetIsReceiveInvite())
		{
			// ���̵� ������ ������ �ʴ븦 �޾���.
			if (auto* UIRaidMain = UIManager->GetUI<UB2UIRaidMain>(UIFName::Raid))
			{
				auto* DocBattle = UB2UIDocHelper::GetDocBattle();
				auto* UIDocRaid = UB2UIDocHelper::GetDocRaid();
				if (DocBattle && UIDocRaid)
				{
					int32 RaidStep = UIDocRaid->GetLastRaidStep();
					int32 RaidType = UIDocRaid->GetLastRaidType();
					EPCClass PcClass = IntToPCClass(DocBattle->GetCurPCClass());
					UIRaidMain->ForcedStartGame(RaidType, RaidStep, ERaidJoinType::MakeRoom, PcClass, false);
				}
			}
		}
	}
	else
	{
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_RaidMain);
	}

	auto EndPoint = std::make_shared<b2network::B2Endpoint>();
	FString NextRoomId(TEXT(""));

	const TArray<b2network::B2RoomPropertyPtr>& Properties = Message->properties;
	for (int32 PropIdx = 0; PropIdx < Properties.Num(); PropIdx++)
	{
		if (auto RoomPropPtr = Properties[PropIdx])
		{
			if (RoomPropPtr->key == TEXT("port"))
				EndPoint->port = FCString::Atoi(*RoomPropPtr->value);

			else if (RoomPropPtr->key == TEXT("address"))
				EndPoint->host = RoomPropPtr->value;

			else if (RoomPropPtr->key == TEXT("nextRoomId"))
				NextRoomId = RoomPropPtr->value;
		}
	}

	if (EndPoint && DocRaid)
		DocRaid->SetPartyRetryInfo(EndPoint, NextRoomId);

	UB2UIManager::GetInstance()->OpenUI(UIFName::RaidJoin);
}

void AB2LobbyGameMode::PlayMainLobbyBgm()
{
	PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_MainLobby);
}

void AB2LobbyGameMode::SetRemotePlayerClass(uint32 NetId, EPCClass PCClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetRemotePlayerClass"));

	// ���� ������ ���� ��Ī�� ���� �ʾҴٸ� PCClass�� ���� �ɾ�д�. (Reserved PCClass)
	if (RemotePlayerInfoData.Contains(NetId) == false)
	{
		EmplacePeerPlayer(NetId, PCClass);
		return;
	}

	Super::SetRemotePlayerClass(NetId, PCClass);

	IncreasePeerPlayers(NetId);
	LobbySceneManager.SetRemotePlayerClass(NetId, PCClass, RemotePlayerInfoData[NetId].GetAccountInfoPtr());
}

void AB2LobbyGameMode::ResponsePrerequisiteToStart(int32 RoomType, const FString& RoomToken, TArray<FB2NetMatchPeerInfo> const& PeerInfos)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ResponsePrerequisiteToStart"));

	for (FB2NetMatchPeerInfo const& PeerInfo : PeerInfos)
	{
		EmplacePeerPlayer(PeerInfo.NetId, static_cast<ETeamType>(PeerInfo.PlayerTeamType));
	}

	Super::ResponsePrerequisiteToStart(RoomType, RoomToken, PeerInfos);

	bWaitPrerequisiteToStartForNonHost = true;
}

void AB2LobbyGameMode::ResponseAckPrerequisiteToStart(uint32 InNetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ResponseAckPrerequisiteToStart"));

	BII_CHECK(GetPeerType() == PEER_SUPER);

	EmplacePeerPlayer(InNetId, true);

	Super::ResponseAckPrerequisiteToStart(InNetId);

	if (IsAlreadyStarted())
		ClearWaitPrerequisiteToStartTimeHandler();
}

void AB2LobbyGameMode::ResponseChangeRoomAccess(uint8 AccessKey)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ResponseChangeRoomAccess"));

	Super::ResponseChangeRoomAccess(AccessKey);

	LobbySceneManager.ResponseChangeRoomAccess(AccessKey);
}

void AB2LobbyGameMode::EmplacePeerPlayer(int32 InNetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EmplacePeerPlayer"));

	if (AllPeerInfos.Contains(InNetId))
		return;

	AllPeerInfos.Emplace(InNetId, FLobbyMatchPeerInfo());
}

void AB2LobbyGameMode::EmplacePeerPlayer(int32 InNetId, bool IsReady)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EmplacePeerPlayer"));

	EmplacePeerPlayer(InNetId);
	AllPeerInfos[InNetId].IsReady = IsReady;
}

void AB2LobbyGameMode::EmplacePeerPlayer(int32 InNetId, EPCClass InReservedPCClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EmplacePeerPlayer"));

	EmplacePeerPlayer(InNetId);
	AllPeerInfos[InNetId].ReservedPCClass = InReservedPCClass;
}

void AB2LobbyGameMode::EmplacePeerPlayer(int32 InNetId, ETeamType InTeamTpye)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EmplacePeerPlayer"));

	EmplacePeerPlayer(InNetId);
	AllPeerInfos[InNetId].PlayerTeamType = InTeamTpye;
}

void AB2LobbyGameMode::RemovePeerPlayer(int32 InNetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::RemovePeerPlayer"));

	if (AllPeerInfos.Contains(InNetId) == false)
		return;

	AllPeerInfos.Remove(InNetId);
}

bool AB2LobbyGameMode::CanRequestRoomClose()
{
	B2_SCOPED_TRACK_LOG(TEXT("B2LobbyGameMode::CanRequestRoomClose"));

	if (IsForceStartMatch(GetCurrentLobbyScene()))
		return true;

	return IsMatchMaxPlayerNum();
}

bool AB2LobbyGameMode::CanStartInGame()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CanStartInGame"));

	if (IsMatchMaxPlayerNum() || IsForceStartMatch(GetCurrentLobbyScene()))
	{
		bool bCanStart = true;

		for (auto PeerInfo : AllPeerInfos)
		{
			// �ڱ��ڽ��� ����
			if (PeerInfo.Key == GetMyNetId())
				continue;

			if (PeerInfo.Value.IsReady == false)
				bCanStart = false;
		}

		return bCanStart;
	}

	return false;
}

ERoomCloseReasonType AB2LobbyGameMode::GetBreakRoomCloseReason()
{
	B2_SCOPED_TRACK_LOG(TEXT("B2LobbyGameMode::GetBreakRoomCloseReason"));

	return LobbySceneManager.GetBreakRoomCloseReason();
}

int32 AB2LobbyGameMode::GetMatchPlayerNum()
{
	return AllPeerInfos.Num();
}

int32 AB2LobbyGameMode::GetMatchPlayerNum(ETeamType teamType)
{
	int32 nCount = 0;

	for (TPair<uint32, FLobbyMatchPeerInfo>& peerInfo : AllPeerInfos)
	{
		if (peerInfo.Value.PlayerTeamType == teamType)
			nCount++;
	}

	return nCount;
}

int32 AB2LobbyGameMode::GetMaxMatchPlayerNum()
{
	B2_SCOPED_TRACK_LOG(TEXT("B2LobbyGameMode::GetMaxMatchPlayerNum"));
	
	return LobbySceneManager.GetMaxMatchPlayerNum();
}

bool AB2LobbyGameMode::IsPlayBeginAction()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::IsMatchMaxPlayerNum"));

	return LobbySceneManager.IsPlayingBeginAnim();
}
void AB2LobbyGameMode::ResetPlayBeginActionState()
{
	LobbySceneManager.SetPlayingBeginAnim(true); // �ٽ� �÷��� �����ϰ� �ϴ� ��. �κ� ÷ ������ �� �ι� �ʿ��ؼ�.
}

void AB2LobbyGameMode::SendLeavePlayerOnlyRaid(uint32 NetId)
{
	// 180213_YJ ���̵� ��Ī ī��Ʈ�϶� Ż�� �߰� ó��
	if (GetPeerType() == PEER_SUPER)
	{
		/*
		#if !UE_BUILD_SHIPPING
		if (NetFakeConfigure::GetInstance().GetHateRaidPenalty()) return; // Ŀ�ǵ� ġ�� Ż�� ���� ����.
		#endif*/

		if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
			DocRaid->SetAlreadyExitUser(true);

		for (auto& Elem : RemotePlayerInfoData)
		{
			if (Elem.Key == NetId)
			{
				data_trader::Retailer::GetInstance().RequestRoomLeaveResult(GetRoomId(), Elem.Value.GetAccountId(), GetToken());
				break;
			}
		}
	}
}

bool AB2LobbyGameMode::IsMatchMaxPlayerNum()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::IsMatchMaxPlayerNum"));

	return (GetMatchPlayerNum() == GetMaxMatchPlayerNum());
}

bool AB2LobbyGameMode::IsMatchMaxPlayerNumOver()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::IsMatchMaxPlayerNumOver"));

	return (GetMatchPlayerNum() > GetMaxMatchPlayerNum());
}

bool AB2LobbyGameMode::IsForceStartMatch(ELobbyScene LobbySceneType)
{
	if (LobbySceneType == ELobbyScene::ELobbyScene_RaidMain)
	{
		auto* DocRaid = UB2UIDocHelper::GetDocRaid();
		return DocRaid && DocRaid->GetForceStartRaid();
	}

	return false;
}

ETeamType AB2LobbyGameMode::GetMyTeamType()
{
	if (AllPeerInfos.Num() == 0)
		return ETeamType::ETT_End;

	return AllPeerInfos[GetMyNetId()].PlayerTeamType;
}

bool AB2LobbyGameMode::IsCurrentUIScene(EUIScene CheckScene)
{
	return UB2UIManager::GetInstance()->IsUIScene(CheckScene);
}

bool AB2LobbyGameMode::RequestStartGameStageWithSelectedPC(int32 ClientStageId, EStageDifficulty StageDifficulty, const TArray<EStageBuffType>& StageBuffTypes, bool bSkipStoryCinematic)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::RequestStartGameStageWithSelectedPC"));

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		RepeatBattleRequestState.bRepeatOne = DocBS->GetRepeatBattleLoopOne();
		RepeatBattleRequestState.bRepeatAll = DocBS->GetRepeatBattleLoopAll();
		RepeatBattleRequestState.bBoostOn = DocBS->GetRepeatBattleBoostOn();
		RepeatBattleRequestState.RemainingBoostCount = DocBS->GetRepeatBattleBoostRemainCount();

		if (!RepeatBattleRequestState.bIsOn && (DocBS->GetRepeatBattleLoopOne() || DocBS->GetRepeatBattleLoopAll()))
		{
			RepeatBattleRequestState.bIsOn = true;
			RepeatBattleRequestState.CurrentRepeatCount = 0;
		}
		if (!DocBS->GetRepeatBattleLoopOne() && !DocBS->GetRepeatBattleLoopAll())
		{
			RepeatBattleRequestState.bIsOn = false;
			RepeatBattleRequestState.CurrentRepeatCount = 0;
		}
	}

	int32 MainPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
	int32 SubPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());

	auto& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
	StageData.RequestStartStage(ClientStageId,
								StageDifficulty,
								BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(),
								BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass(),
								StageBuffTypes,
								RepeatBattleRequestState,
								RepeatBattleRequestState.bIsOn ? true : bSkipStoryCinematic,
								MainPCPresetId,
								SubPCPresetId); // �߰� ��û �ֱ� ������ �ݺ������� �ó����� ���� �ƴ� ������ �����. RequestStartRepeatBattleWithSelectedPC ������..

	return true;
}

bool AB2LobbyGameMode::RequestStartRepeatBattleWithSelectedPC(int32 ClientStageId, EStageDifficulty StageDifficulty)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::RequestStartRepeatBattleWithSelectedPC"));

	// ������ �ݺ����� �������� ������ �ƴ� �� ó�� ��������.
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	// ���� ������ ���� �������� ������ ���� �ΰ��� ����ü�� ������ ����.
	RepeatBattleRequestState.bIsOn = true;
	RepeatBattleRequestState.CurrentRepeatCount = 0;
	if (DocBS)
	{
		RepeatBattleRequestState.bRepeatOne = DocBS->GetRepeatBattleLoopOne();
		RepeatBattleRequestState.bRepeatAll = DocBS->GetRepeatBattleLoopAll();
		RepeatBattleRequestState.bBoostOn = DocBS->GetRepeatBattleBoostOn();
		RepeatBattleRequestState.RemainingBoostCount = DocBS->GetRepeatBattleBoostRemainCount();
	}

#if !UE_BUILD_SHIPPING
	extern int32 GCheat_RepeatBattleCountOverride;
	if (GCheat_RepeatBattleCountOverride > 0)
	{
		RepeatBattleRequestState.CurrentRepeatCount = GCheat_RepeatBattleCountOverride;
		GCheat_RepeatBattleCountOverride = 0;
	}
#endif
	SyncRepeatBattleRequestStateToUIDoc();

	TArray<EStageBuffType> SelectedBuffs; // �ݺ����������� ���� ������ ������ �ִٸ� �޾ƿ�����..
	if (DocBS)
	{
		SelectedBuffs = DocBS->GetSelectedStageBuffs();
	}

	return RequestStartGameStageWithSelectedPC(ClientStageId, StageDifficulty, SelectedBuffs, true); // �߰� ��û �ֱ� ������ �ݺ������� �ó����� ���� �ƴ� ������ �����
}

void AB2LobbyGameMode::CancelOrStopRepeatBattle()
{ // �ݺ� ������ �����Ϸ��µ� �κ��丮�� ���ų� ���� �����ϰų� ���� ������ ���ϰ� �Ǿ��� ��..
	// ������ �������� ���� ��û�ϱ� ������ �ܼ��� �̰ɷ� �ݺ����� �ߴ��� ��.
	RepeatBattleRequestState.bIsOn = false;
	RepeatBattleRequestState.CurrentRepeatCount = 0;
	RepeatBattleRequestState.bRepeatAll = false;
	RepeatBattleRequestState.bRepeatOne = false;
	//RepeatBattleRequestState.MultiplyingFactor = 1;
	SyncRepeatBattleRequestStateToUIDoc();
}

void AB2LobbyGameMode::SetupForNextRepeatBattleStage(FServerStageID InServerStageId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SetupForNextRepeatBattleStage"));

	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	// ������ �ݺ����� �������� �غ� �¾�. �������� �ݺ������� ġ��� �԰� �߰� �ݺ������� �����ִ� ����
	// �ʿ��� ��κ��� �͵��� UI ���� ó���ϰ�.. ���⼱ �ݺ������� �� �̻� �������� ���ϴ� ��쿡 ���� ���� üũ�� �ϵ��� ����..		
	const bool bIsRepeatBattlePossible = StageDataStore.IsPossibleToDoRepeatBattle(InServerStageId);
	if (!bIsRepeatBattlePossible && !(StageDataStore.IsRepeatBattleLocked(InServerStageId) && RepeatBattleRequestState.bRepeatOne))
	{
		// �ϴ� �ߴܺ��� �ϰ�..
		CancelOrStopRepeatBattle();
		// �޽����� �ѷ���߰���..
		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst)
		{
			UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_CannotContinueRepeatBattle")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm,
				FMsgPopupOnClick::CreateLambda([this](){
				}),
				FMsgPopupOnClick::CreateLambda([this](){
				})
			);
		}
	}
}

void AB2LobbyGameMode::SyncRepeatBattleRequestStateToUIDoc()
{
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		DocBS->SetRepeatBattleCurrentOn(RepeatBattleRequestState.bIsOn);
		DocBS->SetRepeatBattleCurrentCount(RepeatBattleRequestState.CurrentRepeatCount);
		DocBS->SetRepeatBattleLoopAll(RepeatBattleRequestState.bRepeatAll);
		DocBS->SetRepeatBattleLoopOne(RepeatBattleRequestState.bRepeatOne);
		DocBS->SetRepeatBattleBoostOn(RepeatBattleRequestState.bBoostOn);
	}
}

void AB2LobbyGameMode::DJLegacy_ChangeLobbyUIPage(ELobbyUIPages InPageEnum)
{
	// �ᱹ�� LobbyUISwitcher �� BeginLobbyUIPage ���ε�, ���ķ� ���ʼ� ���� ��ȭ �� �ʿ��� ���� ������ ó��.
	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(InPageEnum);
}

void AB2LobbyGameMode::DJLegacy_OpenLobbySubPopup(ELobbySubPopups InPopupEnum, FDJLegacySubpopupAddPanelInfo InOptionalAddPanelInfo)
{
	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(InPopupEnum, InOptionalAddPanelInfo);
}

void AB2LobbyGameMode::DJLegacy_CloseLobbySubPopup(ELobbySubPopups InPopupEnum)
{
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(InPopupEnum);
}

UB2LobbyUserWidget* AB2LobbyGameMode::DJLegacy_FindLobbySubPopup(ELobbySubPopups InUserWidgetEnum)
{
	return LobbyUIManager ? LobbyUIManager->DJLegacy_FindLobbySubPopup(InUserWidgetEnum) : nullptr;
}
UB2LobbyUserWidget* AB2LobbyGameMode::DJLegacy_GetCurrLobbyUIPage()
{
	return LobbyUIManager ? Cast<UB2LobbyUserWidget>(LobbyUIManager->GetCurrActiveUI()) : nullptr;
}
void AB2LobbyGameMode::DJLegacy_AsyncLoadUIPage(ELobbyUIPages InLoadUIPage, bool bBlocking)
{
	if (LobbyUIManager)
		LobbyUIManager->TryAsyncLoadOpenUI(InLoadUIPage, bBlocking);
}

EPCClass AB2LobbyGameMode::GetHeroMgmtModePCClass()
{
	return LobbySceneManager.GetHeroMgmtModePCClass();
}

void AB2LobbyGameMode::EnterHeroMgmtMode(EHeroMgmtSubMode InSubMode)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterHeroMgmtMode"));

	bHeroMgmtMode = true;

	//////////////////////////////////////////////////
	// DJLegacy �� �� UI ���� �ӽ� ó��.. �ϰɷ� ���������� �������� ���� ��� DJLegacy �� ���� ��.
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	const EUIScene CurrentUIScene = UIMgrInst->GetCurrUIScene();
	if (CurrentUIScene != EUIScene::None && /*!IsDJLegacyUIScene(CurrentUIScene) && */ // UIInvenQuickSlide �� ���� ��� DJLegacyUIScene �� ���ӵ� �� ����. ��� ���� �� ���� �����丮 �����ϴ� �� ���� ���� �ֱ� �ѵ�.. � ���� �������� �ϰ� �Ǵ�..
		InSubMode != EHeroMgmtSubMode::EHMSM_End && !UIMgrInst->IsOpeningDJLegacyHeroMgmtBySceneHistory())
	{
		// Back button Ŭ�� �� (UB2UIManager::ChangeUISceneBack) ���� UIManager �ý����� ���� UIScene ���� ���ư� �� �ֵ��� ���� �����丮�� �־��ش�.
		// �׷��� Back button Ŭ������ ���⿡ �� ���� ����.
		UIMgrInst->ChangeUIScene(DJLegacyHeroMgmtPageToUIScene(InSubMode));
	}
	//////////////////////////////////////////////////

	//HeroMgmtModePCSelection = BladeIIGameImpl::GetClientDataStore().GetMainPlayerClass(); // �⺻������ �κ񿡼� ���õ� PC �� �Ѱ���.
		
	OnQuitHeroMgmtSubModeCommon(HeroMgmtSubMode); // ���� sub ��� �ݴ� ó����. InSubMode �� ������ ���� ���� ���� ���̴� �ݴ� ó���� �� ��. ������ �ȴٸ� �� �� �˻縦 ��ȭ.

	HeroMgmtSubMode = InSubMode;
	ELobbyUIPages MatchingDjLegacyUIPage = HeroMgmtSubModeToLobbyUIPage(HeroMgmtSubMode);
	if (MatchingDjLegacyUIPage != ELobbyUIPages::ELUP_End)
	{
		DJLegacy_ChangeLobbyUIPage(MatchingDjLegacyUIPage);
		LobbyInventoryViewJustOpenClass<>::GetInstance().Signal();

		// Ȥ���� �� submode ���� Ư���� ������ �ʿ��ϸ� �β��������� ���� ���⿡ if �� switch �־ ó��....
		// ������ ������ �׷��� �Ǿ� �־���. ����
	} 

	// ���� ���� ��� ���� ī�޶��ũ
	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_HeroManagement);
}

void AB2LobbyGameMode::OnChangeUIScene(EUIScene UIScene)
{
	if (BGMManager)
		BGMManager->PlayBGMByUIScene(UIScene);
}

void AB2LobbyGameMode::OnChangeUISceneBack()
{
	if (BGMManager)
	{
		BGMManager->PlayBGMByPrevUIScene();
	}
}

void AB2LobbyGameMode::QuitHeroMgmtMode()
{
	// �̰� DJLegacy �� �Ǵٺ��� ���� �ٲٴ� �� �ٸ� ��θ� ���ϰ� ����� �ΰ����� ó���� �ϰ� �Ǿ���.

	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::QuitHeroMgmtMode"));

	bHeroMgmtMode = false;

	DJLegacy_ChangeLobbyUIPage(ELobbyUIPages::ELUP_End); 
		
	OnQuitHeroMgmtSubModeCommon(HeroMgmtSubMode);

	RequestCombatPower();
}

void AB2LobbyGameMode::OnQuitHeroMgmtSubModeCommon(EHeroMgmtSubMode SubModeToClose)
{
	switch (SubModeToClose)
	{
	case EHeroMgmtSubMode::EHMSM_Inventory:
		LobbyInventoryViewJustCloseClass<>::GetInstance().Signal();
		break;
	case EHeroMgmtSubMode::EHMSM_Brevet:
		break;
	case EHeroMgmtSubMode::EHMSM_Skill:
		break;
	case EHeroMgmtSubMode::EHMSM_Wing:
		break;
	case EHeroMgmtSubMode::EHMSM_Relic:
		break;
	case EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE:
		break;
	}

	UB2Airport::FlushPendingWingVisibilityReq(); // ���� visibility on/off �� �� ��Ƽ� ������ ��û
	BladeIIGameImpl::GetClientDataStore().RequestSyncToServerShowHead(); //�Ӹ� Visible ���� ���� ����ȭ

	// ��� �����̳� ��ų ��ȭ � ���� �Ƹ��� ���� üũ
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	GameStageCheckForArinConsultingClass<FServerStageID>::GetInstance().Signal(DocBS ? DocBS->GetServerStageId() : FServerStageID());
}

void AB2LobbyGameMode::RequestCombatPower()
{
	bool bCheckCombatPower = CheckCombatPower();
	if (bCheckCombatPower)
	{
		TArray<b2network::B2CharacterPowerPtr> CharacterPowerArray;

		for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
		{
			auto* DocHero = UB2UIDocHelper::GetDocHero(i);
			if (DocHero)
			{
				auto CharacterPower = std::make_shared<b2network::B2CharacterPower>();
				CharacterPower->character_type = CliToSvrPCClassType(IntToPCClass(i));
				CharacterPower->total_power = DocHero->GetCombatPower();
				CharacterPowerArray.Add(CharacterPower);

				DocHero->SetChangedCombatPower(false);
			}
		}

		data_trader::Retailer::GetInstance().RequestCheckReachPower(CharacterPowerArray);
	}
}

bool AB2LobbyGameMode::CheckCombatPower()
{
	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		auto* DocHero = UB2UIDocHelper::GetDocHero(i);
		if (DocHero)
		{
			if (DocHero->GetChangedCombatPower())
				return true;
		}
	}

	return false;
}

void AB2LobbyGameMode::SetHeroMgmtModePCSelection(EPCClass InPCClass)
{	
	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(InPCClass);
}

class UB2LobbyInventory* AB2LobbyGameMode::GetLobbyInventory()
{
	// ������ CreateLobbyInventory ���� ������ GetLobbyInventory �� �ϴ� ���� ������..
	checkSlow(LobbyInventory);
	return LobbyInventory;
}

bool AB2LobbyGameMode::CreateLobbyInventory()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CreateLobbyInventory"));

	// LobbyInventory �� ���� �������̽����� static ������ ������ ��ü ������ �� �Ѵ�.
	LobbyInventory = NewObject<UB2LobbyInventory>(this, UB2LobbyInventory::StaticClass(), NAME_None);

	if (LobbyInventory)
	{
		LobbyInventory->InitLobbyInventory(this);
		return true;
	}
	return false;
}

UB2Airport* AB2LobbyGameMode::GetLobbyAirport()
{
	// ������ CreateLobbyAirport ���� ������ GetLobbyAirport �� �ϴ� ���� ������..
	checkSlow(LobbyAirport);
	return LobbyAirport;
}

bool AB2LobbyGameMode::CreateLobbyAirport()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CreateLobbyAirport"));

	LobbyAirport = NewObject<UB2Airport>(this, UB2Airport::StaticClass(), NAME_None);
	if (LobbyAirport)
	{
		LobbyAirport->InitAirportLobby(this);
		return true;
	}
	return false;
}

class UB2PCClassInfo* AB2LobbyGameMode::GetCurrentSelectedPCClassInfo()
{
	UB2PCClassInfoBox* TheBox = StaticFindPCClassInfoBox(this);
	return TheBox ? TheBox->GetSingleClassInfo(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass()) : NULL;
}

void AB2LobbyGameMode::UpdateChapterData()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::UpdateChapterData"));

	// ���� UI Doc �� �������� ���ٸ� ���⼭ ������ �� ��. �����Ͱ� ���� ���̹Ƿ� LobbyInventory ���� ���� Ư���ϰ� ó��.
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	if (SelectedChapterViewNum == 0)
		SelectedChapterViewNum = StageDataStore.GetLastPlayedActId();
	if (SelectedStageDifficulty == EStageDifficulty::ESD_None)
		SelectedStageDifficulty = StageDataStore.GetLastPlayedStageDifficulty();

	if (SelectedChapterViewNum == 0 || SelectedStageDifficulty == EStageDifficulty::ESD_None)
		return;

	UB2ChapterInfo* ChapterInfoTable = StaticFindChapterInfo();
	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	FSingleChapterInfoData* ChapterInfoData = ChapterInfoTable ? ChapterInfoTable->GetInfoData(SelectedChapterViewNum) : NULL;
	UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter();

	if (DocChapter && ChapterInfoTable && StageInfoTable)
	{
		int32 ChapterOpenedNum = StageDataStore.GetActIdByClientStageId(StageDataStore.GetNextClearClientId(SelectedStageDifficulty));
		int32 ChapterMaxNum = FMath::Min(ChapterInfoTable->GetAllChapterNum(), StageInfoTable->GetChapterNumByClientStageId(StageInfoTable->GetFinalClientStageId()));
		FText StageDiffText = FText::FromString(FString::Printf(TEXT("Chapter_DiffColor_%d"), (int32)SelectedStageDifficulty));

		DocChapter->SetChapterOpenedNum(ChapterMaxNum); // ��, ����μ� �׳� �ִ� é�ͱ��� ��ũ���� �� �� �ֵ��� ��. ���� �� ���������� ��� ���� ��.	// ���� : DocChapter->SetChapterOpenedNum(ChapterOpenedNum);
		DocChapter->SetChapterMaxNum(ChapterMaxNum);
		DocChapter->SetMaxOpenedStageDifficulty(ChapterMaxNum >= SelectedChapterViewNum ? StageDataStore.GetMaxOpenedStageDifficulty() : StageDataStore.GetMaxOpenedNewStageDifficulty(SelectedChapterViewNum));
		DocChapter->SetCurChapterNum(SelectedChapterViewNum);
		DocChapter->SetCurStageDifficulty(SelectedStageDifficulty);
		DocChapter->SetCurChapterName(FText::Format(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, StageDiffText.ToString()),
			ChapterInfoTable->GetLocalizedChapterName(SelectedChapterViewNum)));

		// UI������ �˷��ֱ� ���� Next�� ǥ��
		DocChapter->SetNextChapterNum(StageDataStore.GetActIdByClientStageId(StageDataStore.GetNextClearClientId(SelectedStageDifficulty)));
		DocChapter->SetNextSubChapterNum(StageInfoTable->GetSubChapterNumByClientStageId(StageDataStore.GetNextClearClientId(SelectedStageDifficulty)));
	}
}

void AB2LobbyGameMode::SetCombatEntryChapterNum(int32 InChapterNum, EStageDifficulty InDifficulty)
{
	SelectedChapterViewNum = InChapterNum;
	SelectedStageDifficulty = InDifficulty;

	UpdateChapterData();

	// é�� ���� �� ���� ��Ʈ���� ������ ���� ������ ���� ���� ��Ʈ������ ���� �� �־�� �� �� �ִ�. 
	ChangeLobbyScene(ELobbyScene::ELobbyScene_Chapter, SelectedChapterViewNum);
	LobbySceneManager.ForceUpdate();
}

void AB2LobbyGameMode::GoGameStageInfo(FServerStageID InServerStageId, FRepeatBattleStateSet& RepeatBattleSet)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::GoGameStageInfo"));

	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	// �ݺ����� �������� �Ϸ��ϰ� ������ ������ �� ����� ���ƿ´�. (AB2StageGameMode::OnEndPlayOfSingleRepeatBattleStage)
	// �ݺ������� ������ �����ؾ� �Ѵٸ� ���� �ݺ� ī��Ʈ�� �ϳ��� �־�� ��.
	BII_CHECK(!RepeatBattleSet.bIsOn || RepeatBattleSet.CurrentRepeatCount > 0);
	
	const bool bIsRepeatBattlePossible = StageDataStore.IsPossibleToDoRepeatBattle(InServerStageId);
	if (!bIsRepeatBattlePossible && !(StageDataStore.IsRepeatBattleLocked(InServerStageId) && RepeatBattleSet.bRepeatOne))
	{
		RepeatBattleRequestState = FRepeatBattleStateSet();
	}
	else
	{
		RepeatBattleRequestState = RepeatBattleSet;
	}
	if(!RepeatBattleRequestState.bIsOn)
		RepeatBattleRequestState.CurrentRepeatCount = 0;
	if (RepeatBattleRequestState.bIsOn && RepeatBattleRequestState.CurrentRepeatCount < 0){ // Ȥ�� ����
		RepeatBattleRequestState.CurrentRepeatCount = 0;
	}
	SyncRepeatBattleRequestStateToUIDoc();

	//>>==============���õ� ���������� ������. �ϴ� �׽�Ʈ�� ���� �ξ��µ� �����δ� ������ Request�ϰ� ��������� ó�� �ؾ� �ɵ�
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		DocBS->SetClientStageId(StageDataStore.GetClientStageId(InServerStageId));
		DocBS->SetStageDifficulty(StageDataStore.GetStageDifficulty(InServerStageId));
		DocBS->SetServerStageId(InServerStageId);
		
		//todo StageAreaName�� ���̺�� �����ϵ��� �����Ͽ� ���⼭ �����ϵ��� - �� �������� ��ĭ����
		DocBS->SetLocalizedAreaName(FText::FromString(FString(TEXT(""))));

		//todo �κ��丮�� ������ ������ �ֵ���.
		// 			DocBS->SetBuffItemPlayFast(FIntPoint(1000100, 10));
		// 			DocBS->SetBuffItemExpIncrease(FIntPoint(1000101, 0));
		// 			DocBS->SetBuffItemGoldIncrease(FIntPoint(1000102, 5));
		// 			DocBS->SetBuffItemAutoSkill(FIntPoint(1000103, 1));

		TArray<FB2RewardItemPreviewInfo> RewardItemInfo;
		StageDataStore.GetStageRewardPreviewItems(InServerStageId, RewardItemInfo);
		StageDataStore.GetStageRewardPreviewItemsOfSameType(RewardItemInfo);
		DocBS->SetRewardItemList(RewardItemInfo);
		DocBS->SetBossDropItemsList(StageDataStore.GetStageFixedDrops(InServerStageId.Id));

		TArray<FB2AetherItemPreviewInfo> AetherItemInfo;
		StageDataStore.GetStageAetherPreviewItems(InServerStageId, AetherItemInfo);
		DocBS->SetAetherItemList(AetherItemInfo);

		TArray<FB2MaterialItemPreviewInfo> MaterialItemInfo;
		StageDataStore.GetStageMaterialPreviewItems(InServerStageId, MaterialItemInfo);
		DocBS->SetMaterialItemList(MaterialItemInfo);

		DocBS->SetRepeatMissionReward(FIntPoint(FItemRefIDHelper::GetGoodsID_Gold(), 0));

		int32 OutRepeatBattleCostRefId, OutRepeatBattleCostCount, OutRequireStamina;
		StageDataStore.GetStageCostData(InServerStageId, OutRepeatBattleCostRefId, OutRepeatBattleCostCount, OutRequireStamina);

		DocBS->SetRepeatBattleCost(FIntPoint(OutRepeatBattleCostRefId, OutRepeatBattleCostCount));
		DocBS->SetRequiredStaminaNum(OutRequireStamina);
		
		int32 RecommendPower;
		StageDataStore.GetStageSuggestedCombatStats(InServerStageId, RecommendPower);
		DocBS->SetRecommendPower(RecommendPower);
		
		int32 RecommendAttack;
		int32 RecommendDefense;
		StageDataStore.GetStageRecommendAbility(InServerStageId, RecommendAttack, RecommendDefense);
		DocBS->SetRecommendAttack(RecommendAttack);
		DocBS->SetRecommendDefense(RecommendDefense);

		DocBS->SetBattleDifficulty(StageDataStore.GetRelativeStageDifficulty(InServerStageId, ClientDataStore.GetLocalCharacterData().GetMainPlayerClass(), ClientDataStore.GetLocalCharacterData().GetSubPlayerClass()));
	}

	GameStageCheckForArinConsulting(InServerStageId); // BattleStageInfo ������ ���� ���� �Ƹ� ���� üũ�� �Ϸ�.

	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::BattleStageInfo);
	//<<==============���õ� ���������� ������.

	if (RepeatBattleRequestState.bIsOn) // RepeatBattle ���� �������� �Ϸ� �ÿ��� �� ��Ʈ�� ���� ����.
	{
		SetupForNextRepeatBattleStage(InServerStageId);
	}
}

void AB2LobbyGameMode::GoGameStageInfoLastReached()
{
	/*int32 ChapterNumber = 0;
	int32 StageNumber = 0;
	EStageDifficulty StageDifficulty = EStageDifficulty::ESD_None;

	BladeIIGameImpl::GetStageDataStore().LoadRecentlyStageInfo(ChapterNumber, StageNumber, StageDifficulty);

	if (StageNumber == 0 || ChapterNumber == 0 || StageDifficulty == EStageDifficulty::ESD_None)
	{
	StageNumber = 1;
	ChapterNumber = 1;
	StageDifficulty = EStageDifficulty::ESD_Normal;
	}*/
	//�ֱ� �÷����� �������� ����
	FStageDataStore& StageStore = BladeIIGameImpl::GetStageDataStore();
	FServerStageID ServerStageID;
	EStageDifficulty CurDiff = EStageDifficulty::ESD_Normal;
	int32 CurChapter = 1;


	auto DocChapter = Cast<UB2UIDocChapter>(UB2UIDocHelper::GetDocChapter());
	

	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	if (StageInfoTable == nullptr)
		return;

	if (DocChapter == nullptr)
		return;
	CurChapter = DocChapter->GetCurChapterNum();
	int32 MaxChapterNum = StageInfoTable ? StageInfoTable->GetStartAdventureModeChapterNum() : 0;

	if (CurChapter <= MaxChapterNum)
	{
		if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_Normal)) // �븻 ��带 ������ ������
		{
			if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_Hard)) // �ϵ� ��带 ������ ������
			{
				if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_VeryHard)) // �ſ� ������� ������ ������
				{
					if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_Hell)) // ���� ������ ������
					{
						ServerStageID = StageStore.GetLastClearServerStageId(EStageDifficulty::ESD_Hell);
					}
					else
					{
						ServerStageID = StageStore.GetNextClearServerStageId(EStageDifficulty::ESD_Hell);
					}
				}
				else
				{
					ServerStageID = StageStore.GetNextClearServerStageId(EStageDifficulty::ESD_VeryHard);
				}
			}
			else
			{
				ServerStageID = StageStore.GetNextClearServerStageId(EStageDifficulty::ESD_Hard);
			}
		}
		else
		{
			ServerStageID = StageStore.GetNextClearServerStageId(EStageDifficulty::ESD_Normal);
		}

		
		if (StageStore.GetClientStageId(ServerStageID) > StageInfoTable->GetFinalClientStageId())
			ServerStageID = StageStore.GetServerStageID(StageInfoTable->GetFinalClientStageId() , EStageDifficulty::ESD_Hell);
		
	}
	else
	{
		ServerStageID = StageStore.GetNewLastClearServerStageIdByLastPlayedStageDifficulty(CurChapter);
	}

	CurDiff = StageStore.GetStageDifficulty(ServerStageID);
	CurChapter = StageStore.GetActIdByServerStageId(ServerStageID);

	// �ڷΰ���� ���ƿ����� �ش� é�ͷ� �����ֱ� ���� �̸� DOC����
	
	
	DocChapter->SetCurChapterNum(CurChapter);
	DocChapter->SetCurStageDifficulty(CurDiff);
	

	BladeIIGameImpl::GetStageDataStore().RequestGetStageInfo(ServerStageID, true);
}

void AB2LobbyGameMode::GameStageCheckForArinConsulting(FServerStageID InServerStageId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::GameStageCheckForArinConsulting"));

	FClientDataStore& ClientDataStorte = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	
	TArray<EPCClass> CharacterClasses;
	CharacterClasses.Add(ClientDataStorte.GetLocalCharacterData().GetMainPlayerClass());
	CharacterClasses.Add(ClientDataStorte.GetLocalCharacterData().GetSubPlayerClass());

	int32 UnitedSkillPoint = 0;
	float UnitedAttackPower = 0.0f;
	float UnitedDefensePower = 0.0f;

	for (auto CharacterClass : CharacterClasses)
	{
		if(EPCClass::EPC_End == CharacterClass)
			continue;

		UnitedAttackPower += CombatStatEval::GetPCAttack(CharacterClass, NULL);
		UnitedDefensePower += CombatStatEval::GetPCDefense(CharacterClass, NULL);
		UnitedSkillPoint += ClientDataStorte.GetLocalCharacterData().GetCharacterLeftSkillPoint(CharacterClass);
	}

	float StageSuggestedAttack = 0.0f;
	float StageSuggestedDefense = 0.0f;
	StageDataStore.GetStageSuggestedCombatStats(InServerStageId, StageSuggestedAttack, StageSuggestedDefense);

	EArinConsultPreCombat_MainSubject FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_None;
	// ��¼�� ����/���� �� Ư�� ���� ���������� ���� �����ؾ� �� ����. �Ƹ��� ���𿡼� �κ��丮�� �Ѿ �� Ŭ���� ���ÿ� �ݿ��ؾ� �� ���� ������..
	if (UnitedAttackPower < StageSuggestedAttack)
	{
		FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_Attack;
	}
	else if (UnitedDefensePower < StageSuggestedDefense)
	{
		FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_Defense;
	}
	else if (UnitedSkillPoint > 5) // ��ġ�� ������(?)
	{
		FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_Skill;
	}
	//else if (MainCharSkillPoint > 3 || SubCharSkillPoint > 3) // ���� ���� 3���� �� �� �ϳ��� 3���� Ȯ��..
	//{
	//	FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_Skill;
	//}

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		DocBS->SetArinConsultingSubject(FinalArinConsultSubject);
	}
}

void AB2LobbyGameMode::GoRaidGameMode(const FString& InvitedRoomId, EPCClass ChangeMainPCClass, int32 RaidType, int32 Step)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::GoRaidGameMode"));

	BII_CHECK(!InvitedRoomId.IsEmpty());

	// ���� �����������, ������Ī���� �����͸� ���������� �ʴ�޾�����쿣 �����ִ� �κ��� ���� �߰�.
	AllPeerInfos.Empty();
	if (auto* UIDocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		UIDocLobbyMatch->ClearRemotePlayerInfoData();

	if (UB2UIDocRaid* UIDocRaid = UB2UIDocHelper::GetDocRaid())
	{
		UIDocRaid->ClearRaidJoinData();
		UIDocRaid->ClearRaidBattleData();
		
		// ���õ� ���� �Է�
		UIDocRaid->SetLastRaidType(RaidType);
		UIDocRaid->SetLastRaidStep(Step);
		UIDocRaid->SetIsReceiveInvite(true);
		UIDocRaid->SetOpenedRaidJoinType(ERaidJoinType::MakeRoom);
	}

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		DocBattle->SetCurPCClass(PCClassToInt(ChangeMainPCClass));

	// �����͸� ���� �����ϰ� Request�� �����Ŀ� Response ���� ���� UI����.
	// (���� : �ʴ� �޾��� ��� RequestRoomMatchMaking�� ���� �ʰ� RequestJoinRoom���� �ٷ� �뿡 ����!)
	TArray<b2network::B2RoomPropertyPtr> properties;
	TArray<b2network::B2RoomCharacterPtr> characters;
	TArray<b2network::B2PresetNumPtr> presetNums;


	auto& LocalCharData = BladeIIGameImpl::GetLocalCharacterData();

	// temporary test code
	for (int i = (int)EPCClass::EPC_Gladiator; i < GetMaxPCClassNum(); ++i) 
	{
		const EPCClass PCClass = static_cast<EPCClass>(i);

		auto b2PresetNum = std::make_shared<b2network::B2PresetNum>();
		b2PresetNum->character_type = CliToSvrPCClassType(IntToPCClass(i));
		b2PresetNum->item_preset_num = static_cast<int32>(LocalCharData.GetPCClassItemPresetSlotID(PCClass));
		b2PresetNum->skill_preset_num = static_cast<int32>(LocalCharData.GetPCClassSkillSlotID(PCClass));
		presetNums.Emplace(b2PresetNum);
	}

	data_trader::Retailer::GetInstance().RequestRequestJoinRoom(InvitedRoomId, b2network::B2RoomType::BRT_Raid, 0, properties, characters, presetNums);
}

void AB2LobbyGameMode::SetItemOpDirectingSceneObjects(class ACameraActor* InCamActor)
{
	LobbySceneManager.SetItemOpDirectingSceneObjects(InCamActor);
}

void AB2LobbyGameMode::EnterItemOpDirectingView(ELobbyInvenItemOpMode InOpMode)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterItemOpDirectingView"));

	LobbySceneManager.EnterItemOpDirectingView(InOpMode); // LobbySceneManager �� ���� �غ��� ���� ����� ī�޶� ���.
}

void AB2LobbyGameMode::QuitItemOpDirectingView()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::QuitItemOpDirectingView"));

	LobbySceneManager.QuitItemOpDirectingView(); // ���� �κ��丮 ī�޶� ��� ����
}

bool AB2LobbyGameMode::IsInItemOpDirectingView()
{
	return LobbySceneManager.IsInItemOpDirectingView();
}

void AB2LobbyGameMode::EnterPVPMainMenu()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterPVPMainMenu"));

	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::PVPOneOnOne);
}

void AB2LobbyGameMode::EnterTeamMatchMenu()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterTeamMatchMenu"));

	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::TeamMatchUI);
}

void AB2LobbyGameMode::EnterControlMenu()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterControlMenu"));

	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::ControlMain);
}

void AB2LobbyGameMode::EnterTMChageHeroMod()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::EnterTMChageHeroMod"));

	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::TeamHeroChangeUI);
}

#include "B2UIStore.h"
#include "B2UISummonItem.h"
#include "B2UIMagicStore.h"
void AB2LobbyGameMode::EnterShop(int32 nWhereShop)
{ 
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	
	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	if (UIMgr)
	{ // ������ �˾��� �� ���¿��� ���� �����ϴ� ��찡 ���� �� �־ �װ� ó��.
		UIMgr->CloseAllStandaloneUIs();
	}

	switch (static_cast<EStorePageWhere>(nWhereShop))
	{
	case EStorePageWhere::Store:
	{
		auto* StoreUI = UIMgr->GetUI<UB2UIStore>(UIFName::Store);
		if (StoreUI)
			StoreUI->SetTab();
		else
			data_trader::Retailer::GetInstance().RequestGetGeneralShop();
	}
		break;
	case EStorePageWhere::EquipStore:
	{
		auto* SummonItemUI = UIMgr->GetUI<UB2UISummonItem>(UIFName::SummonItemStore);
		if (SummonItemUI)
			SummonItemUI->SetTab();
		else
			data_trader::Retailer::GetInstance().RequestGetLotteryShop();
	}
		break;
	case EStorePageWhere::PackageStore:
	{
		data_trader::Retailer::GetInstance().RequestGetPackageShop();
	}
		break;
	case EStorePageWhere::MagicStore:
	{
		auto* MagicStoreUI = UIMgr->GetUI<UB2UIMagicStore>(UIFName::MagicStore);
		if (MagicStoreUI)
			MagicStoreUI->SetItemList();
		else
			data_trader::Retailer::GetInstance().RequestGetMagicShop();
	}
		break;

	}
}

//////////////////////////////////////////////////
// ��ü�� Lobby UI ���� �Ҹ��� �Ǵ� �������� ���� ��ɵ�.

void AB2LobbyGameMode::LoginAndStartGame(const FString& InUserID, const FString& InUserPW)
{
	//DJLegacy_ChangeLobbyUIPage(ELobbyUIPages::ELUP_CharSel); // �ٷ� ���� �������� �̵�.
}

void AB2LobbyGameMode::CreateAccount(const FString& InUserID, const FString& InUserPW)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CreateAccount"));

	//if (InUserID.IsEmpty())
	//{
	//	// TODO : �������� �Է����ּ���~ �˾� �޼���
	//	UE_LOG(LogBladeII, Log, TEXT("Please enter your nickname!!"));
	//}
	//else
	//{
	//	auto B2MessageInfo = std::make_shared<b2network::FB2MessageInfoRequestSignUp>
	//	(
	//		[this, InUserID, InUserPW](FB2MessageInfoPtr messageInfo)
	//		{
	//			// ���� ������ ���������Ƿ� �ٽ� ���� �õ��� ����
	//			LoginAndStartGame(InUserID, InUserPW);
	//		},
	//		[this](FB2MessageInfoPtr messageInfo)
	//		{
	//			b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
	//			// TODO ���� �ڵ� ó��
	//		}
	//	);
	//	B2MessageInfo->name = TCHAR_TO_ANSI(*InUserID);
	//	FB2NetworkBridge::GetInstance()->SendB2Message(B2MessageInfo);
	//}
}

void AB2LobbyGameMode::ProceedOnCharacterSelection()
{ // ĳ���� ��Ʈ�� â������ ���� ����. �� ������ ������ �� �ƴѵ� �� ó���� ĳ���� ��Ʈ�ΰ� ���� �����ϴ� ���̶� �̸��� �̷��� �Ǿ���.
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::LobbyMain);
}

#include "Engine/LevelStreaming.h"
void AB2LobbyGameMode::AddHallOfFrameCharacter(const TArray<FHallOfFameCharacterInfo>& HallOfFameCharacter)
{
	LobbySceneManager.AddHallOfFrameCharacter(HallOfFameCharacter);
}

void AB2LobbyGameMode::AddHallOfCharacterAnimation(EPCClass PCClass, class UAnimationAsset* PCAnimation)
{
	LobbySceneManager.AddHallOfCharacterAnimation(PCClass, PCAnimation);
}

void AB2LobbyGameMode::AddLevelToLobbyScene(ELobbyLevelType LobbyType, class ULevelStreaming* StreamingLevel)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::AddLevelToLobbyScene"));
	if (StreamingLevel)
	{
		LobbySceneManager.AddLevelLobbyScene(LobbyType, StreamingLevel->GetWorldAssetPackageFName());
	}	
}

void AB2LobbyGameMode::ClearMatchRoom()
{
	Super::ClearMatchRoom();

	AllPeerInfos.Empty();
}

void AB2LobbyGameMode::ClearInvitationGameInfo(EB2GameMode GameModeType)
{
	Super::ClearInvitationGameInfo(GameModeType);

	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		if(auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
			DocPVP->ClearInvitationData();
	}
}

void AB2LobbyGameMode::RequestGameInvite(const FString& NaickName, EB2GameMode GameModeType)
{
	// Request to server
	Super::RequestGameInvite(NaickName, GameModeType);

	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		if (UB2UICommonInvite* InviteUI = UIManager->OpenUI<UB2UICommonInvite>(UIFName::InviteGame))
			InviteUI->OnRequestGameInvite(NaickName, GameModeType);

		UIManager->CloseUI(UIFName::RaidInviteList);
	}

	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
		{
			DocPVP->SetIsFriendshipGame(true);
			DocPVP->SetInviterForceCancel(false);
		}
	}
}

void AB2LobbyGameMode::RequestDeclineInvitation(EB2GameMode GameModeType)
{
	Super::RequestDeclineInvitation(GameModeType);

	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch();
		const bool bWaitingAccept = DocLobbyMatch && DocLobbyMatch->GetInvitationState() == EInvitationState::WaitingAccept;

		if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
			DocPVP->SetInviterForceCancel(true);
	}
}

// CallOwner = �ʴ�(����)��
void AB2LobbyGameMode::PrevAcceptGameInvite(const FString& Inviter, EB2GameMode GameModeType)
{
	// InviteGame UI���� ������ ������ ĳ���� ���� ���� ���� ���� Request�� ����
	auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();
	if (GameModeType == EB2GameMode::PVP_Tag && DocPVP)
	{
		// CharacterSelectPage �̵� �� Setting�Ǿ� �־�� ��
		DocPVP->SetIsFriendshipGame(true);
		DocPVP->SetIsInvitee(true);
		DocPVP->SetIsInviterReady(true);
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_PVPCharacterSelectPage);
		
		if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
		{
			if (auto* SelectUI = UIManager->GetUI<UB2UIPVPCharacterSelectPage>(UIFName::PVPCharacterSelectPage))
				SelectUI->OnPrevAcceptInvitation();
		}

		if(DocPVP->GetRewardBoxStatus() == nullptr)
			data_trader::Retailer::GetInstance().RequestGetMatchStatus();

	}

	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	if (GameModeType == EB2GameMode::Raid && DocRaid)
	{
		if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
		{
			auto* RaidInvitePopupUI = UIManager->OpenMsgPopup<UB2UIMsgPopupSelectPCClass>(EUIMsgPopup::SelectPCClass,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")), FText::GetEmpty(),
				0.0f,
				true,
				true,
				EUIMsgPopupButtonGroup::AcceptOrReject);

			// RequestRaidStart �̸��� ���̵� ����������, ���������� �������� ��������
			// ���̵� �ʴ�ÿ� � ĳ���͸� �����ߴ��� �˸���, ���������� ���� �Ŀ� ������.
			if (RaidInvitePopupUI != nullptr)
			{
				RaidInvitePopupUI->SetAutoCloseOnClick(false); // ���������� ó���� �Ŀ� �ݾƾ���
				RaidInvitePopupUI->AddHandler(EUIMsgPopupButton::Positive, FMsgPopupOnClick::CreateLambda([this, RaidInvitePopupUI]() {
					data_trader::Retailer::GetInstance().RequestRaidStart(CliToSvrPCClassType(RaidInvitePopupUI->GetSelectPCClass()));
				}));
			}
		}
	}
}

bool AB2LobbyGameMode::CanReceiveGameInvite(const FString& Inviter, EB2GameMode GameModeType) const
{
	if(Super::CanReceiveGameInvite(Inviter, GameModeType))
	{
		auto* DocRaid = UB2UIDocHelper::GetDocRaid();
		// �̹� �濡 �� �ִٴ� �ǹ�
		if (DocRaid && DocRaid->HasValidPartyRetryInfo())
			return false;

		return true;
	}

	return false;
}

void AB2LobbyGameMode::OnReceiveInvitation(const FString& Inviter, EB2GameMode GameModeType)
{
	if (GameModeType == EB2GameMode::PVP_Tag)
		ClearInvitationGameInfo(EB2GameMode::Raid);
	else if (GameModeType == EB2GameMode::Raid)
		ClearInvitationGameInfo(EB2GameMode::PVP_Tag);

	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		if (UB2UICommonInvite* InviteUI = UIManager->OpenUI<UB2UICommonInvite>(UIFName::InviteGame))
			InviteUI->OnReceiveInvitation(Inviter, GameModeType);

		// LobbyMain�� Hide ���¶��
		if (UIManager->GetCurrUIScene() == EUIScene::LobbyMain)
		{
			auto* DocUICondition = UB2UIDocHelper::GetDocUICondition();
			if(DocUICondition && DocUICondition->GetHideAllLobbyMainUIs())
				DocUICondition->SetHideAllLobbyMainUIs(false);
		}
	}
}

void AB2LobbyGameMode::OnInvitationSuccess(const FString& Invitee, EB2GameMode GameModeType, const FString& InvitationRef)
{
	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();
		auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch();
		auto* UIManager = UB2UIManager::GetInstance();
		if (DocPVP == nullptr || UIManager == nullptr)
			return;

		DocPVP->SetIsInviteeReady(true);
		DocPVP->SetIsInviterReady(true);
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_PVPCharacterSelectPage);

		if (auto* CharSelectUI = UIManager->GetUI<UB2UIPVPCharacterSelectPage>(UIFName::PVPCharacterSelectPage))
			CharSelectUI->OnInvitationSuccess();

		UIManager->CloseUI(UIFName::InviteGame);
		UIManager->CloseUI(UIFName::RaidInviteList);
	}
}

void AB2LobbyGameMode::OnInvitationDeclined(const FString& Invitee, EB2GameMode GameModeType)
{
	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		const FString MyNickName = BladeIIGameImpl::GetLocalCharacterData().GetUserNickName();
		const bool IsInvitee = Invitee == MyNickName;

		UB2UIManager* UIManager = UB2UIManager::GetInstance();
		if (UB2UICommonInvite* InviteUI = UIManager->GetUI<UB2UICommonInvite>(UIFName::InviteGame))
			InviteUI->OnReceiveDecline(Invitee, IsInvitee);

		if (IsInvitee)
		{
			if (auto* PVPSelectUI = UIManager->GetUI<UB2UIPVPCharacterSelectPage>(UIFName::PVPCharacterSelectPage))
				PVPSelectUI->OnInvitationDeclineByInvitee();
		}
	}
}

void AB2LobbyGameMode::ChangeLobbyScene(EUIScene ToChangeUIScene, int32 OptionalChapterNum /*= -1*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ChangeLobbyScene"));
	ChangeLobbyScene(LobbySceneManager.GetLobbyScene(ToChangeUIScene), 
		(OptionalChapterNum > 0) ? OptionalChapterNum : SelectedChapterViewNum);
}

void AB2LobbyGameMode::HideActorHasTag(FName Tag, bool bHide)
{
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		if ((*It)->ActorHasTag(Tag))
			(*It)->SetActorHiddenInGame(bHide);
	}
}

//void AB2LobbyGameMode::SetGuildMapChangeMatinee(class ALevelSequenceActor * Atype, class ALevelSequenceActor * Btype)
//{
//	if (LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_GuildMapUI
//		|| LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_Guild)
//	{
//		LobbySceneManager.SetGuildMapChangeMatinee(Atype, Btype);
//	}
//
//}

void AB2LobbyGameMode::GuildMapChangeMatineeEnd()
{
	if (LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_GuildMapUI
		|| LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_Guild)
	{
		LobbySceneManager.GuildMapChangeMatineeEnd();
	}
}


void AB2LobbyGameMode::ResponseHallOfFameInfo(FB2ResponseGetHallOfFamePtr HallOfFameInfo)
{
	if (LobbySceneManager.IsHallOfFameData(HallOfFameInfo))
	{
		LobbySceneManager.SetHallOfFameInfo(HallOfFameInfo);
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::HallOfFame);
	}
}

void AB2LobbyGameMode::ResponsePraiseTargetRanker(FB2ResponsePraiseTargetRankerPtr PraiseInfo)
{
	LobbySceneManager.SetPraiseInfo(PraiseInfo);
}

void AB2LobbyGameMode::EnterCostumeUIScene(const ECostumePage& eCostumePageType)
{
	FCostumeDataStore& CostumeDataStore = BladeIIGameImpl::GetCostumeDataStore();

	if (CostumeDataStore.GetIsOpenCostume() == false)
	{
		return;
	}

	CostumeDataStore.SetCachedCostumePageType(eCostumePageType);

	ELobbyUIPages MatchingDjLegacyUIPage = HeroMgmtSubModeToLobbyUIPage(EHeroMgmtSubMode::EHMSM_Costume);

	bool bRequetAsyncLoading = BladeIIGameImpl::bAllowUIAsyncLoading && BladeIIGameImpl::bAllowUIOnDemandAsyncLoading;

	DJLegacy_AsyncLoadUIPage(MatchingDjLegacyUIPage, bRequetAsyncLoading);
}

void AB2LobbyGameMode::ResponseCheckReachPower(FB2ResponseCheckReachPowerPtr Message)
{
	if (Message->required_tracking_targets.Num() > 0)
	{
		for (int32 i = 0; i < Message->required_tracking_targets.Num(); ++i)
		{
			EPCClass PCClass = SvrToCliPCClassType(Message->required_tracking_targets[i]);
			
			// Preset
			int32 CurrentItemPresetID = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(PCClass);
			// Skill Slot
			EItemPresetSlotType SlotType = BladeIIGameImpl::GetLocalCharacterData().GetPCClassSkillSlotID(PCClass);
			int32 SkillSlotID = static_cast<int32>(SlotType);

			TArray<b2network::B2CharacterPowerSpecificPtr> CharacterPowerSpecificArray;
			CombatStatEval::GetReuqestPCCombatPower(PCClass, CharacterPowerSpecificArray);

			data_trader::Retailer::GetInstance().RequestTrackingPowerDetail(Message->required_tracking_targets[i], CurrentItemPresetID, SkillSlotID, CharacterPowerSpecificArray);
		}
	}
}

void AB2LobbyGameMode::ResponseReceiveFairyGift(const FB2ReceiveFairyGiftPtr& pReceiveFairyGift)
{
	BladeIIGameImpl::GetClientDataStore().SetNextFairyGiftTime(pReceiveFairyGift->next_fairy_gift_time);

	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager && (pReceiveFairyGift->rewards.Num() > 0))
	{
		auto RewardPopup = UIManager->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
		if (RewardPopup != nullptr)
		{
			RewardPopup->AddRewardItems(pReceiveFairyGift->rewards);
			RewardPopup->ShowMailDescription(false);
			RewardPopup->SetJackpotReward(pReceiveFairyGift->jackpoted);
			RewardPopup->ShowRewardPopup(true);
		}
		UIManager->CloseUI(UIFName::MsgPopupSelectFairyGift);
	}
	LobbySceneManager.OnLobbyFlyFairy_AttachFX(false);
}

void AB2LobbyGameMode::UpdateFairyGiftState()
{
	const bool IsPossibleFairyGift = BladeIIGameImpl::GetClientDataStore().GetPossibleFairyGift();
	LobbySceneManager.OnLobbyFlyFairy_AttachFX(IsPossibleFairyGift);
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
// �� �ε� �ð� �����ϴ� �� �κ� �� ��ȯ �ð� �������� ���
extern void DevCheckLoadingTimeBegin();
extern void DevCheckLoadingTimeEnd();
#endif

void AB2LobbyGameMode::ChangeLobbyScene(ELobbyScene LobbyScene, int32 OptionalChapterNum /*= -1*/)
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	DevCheckLoadingTimeBegin();
#endif

	if (LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_HeroManagement ||
		LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_FairyMain ||
		LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_Guild)
	{
		if (LobbyScene != ELobbyScene::ELobbyScene_HeroManagement)
		{
			QuitHeroMgmtMode();
		}
	}

	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::ChangeLobbyScene"));
	SetTimeToIgnoreTouchEnd(-1.f);
	LobbySceneManager.ChangeLobbyScene(LobbyScene, (OptionalChapterNum > 0) ? OptionalChapterNum : SelectedChapterViewNum);	

	LobbyEndOnTouchImpulseRingClass<>::GetInstance().Signal(); // Ȯ�� ��� ��������

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	DevCheckLoadingTimeEnd();
#endif
}

/** 
 * ���ɿ� ����� �� �� �־ ���� ������Ʈ ���ڴ� �������� �ʴ´�.
 * �⺻������ �� ĳ���� Ŭ���� ���� merge �� �޽��� ���� ����, ���� ���ο� ���� refresh �� �ؾ��ϴ��� �Ǻ��� ��.
 */
void AB2LobbyGameMode::UpdateCharacterParts(EPCClass InPClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::UpdateCharacterParts"));
	LobbySceneManager.UpdateLobbyCharCompForParts(InPClass);
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
void AB2LobbyGameMode::DevForceUpdateCharacterParts(EPCClass InPCClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::DevForceUpdateCharacterParts"));
	LobbySceneManager.DevForceUpdateLobbyCharCompForParts(InPCClass);
}
#endif

void AB2LobbyGameMode::AnyOtherUpdateOnSetAccount()
{
	LobbySceneManager.ConditionalUpdateLobbyOnHeadDisplayWidget();
	// ��Ÿ �������� SetAccount ���� �� ������Ʈ �ʿ��� ����� �� ������ �߰�
}

bool AB2LobbyGameMode::SelectLobbyMainCharacter(EPCClass SelectedClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::SelectLobbyMainCharacter"));
	return LobbySceneManager.SelectLobbyMainCharacter(SelectedClass);
}
//
//void AB2LobbyGameMode::SetDefaultCameraMatinee(class ALevelSequenceActor * DefaultMatinee)
//{
//	LobbySceneManager.SetDefaultCameraMatinee(DefaultMatinee);
//}
//
//void AB2LobbyGameMode::SetChapterViewMatinee(class ALevelSequenceActor * ChapterMatinee)
//{
//	LobbySceneManager.SetChapterViewMatinee(ChapterMatinee);
//}

void AB2LobbyGameMode::UpdateNativeLobbyMatineeRef()
{ // �Ϲ������δ� ��Ʈ���� ���� �ε� �� BeginPlay �� �� ���� Matinee �� ���õ�. Ư���� ��쿡 ���� ������Ʈ�� �� ���.
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::UpdateNativeLobbyMatineeRef"));
	if (GEngine){
		GEngine->HandleCeCommand(GetWorld(), UPDATE_NATIVE_LOBBY_MATINEE_REF_EVENT, *GLog);
	}	
}

void AB2LobbyGameMode::AddLobbyCharacter(EPCClass PCClass, const FLobbyCharacter& LobbyCharacter)
{
	LobbySceneManager.AddLobbyCharacter(PCClass, LobbyCharacter);
}

void AB2LobbyGameMode::AddLobbyATypeActorInfo(EPCClass PCClass, const FLobbyCharacterInfo& LobbyCharacter)
{
	LobbySceneManager.AddLobbyATypeActorInfo(PCClass, LobbyCharacter);
}

void AB2LobbyGameMode::AddLobbyGTypeActorInfo(EPCClass PCClass, const FLobbyCharacterInfo& LobbyCharacter)
{
	LobbySceneManager.AddLobbyGTypeActorInfo(PCClass, LobbyCharacter);
}

void AB2LobbyGameMode::AddLobbyNPCCharacter(ELobbyNPCType InType, const FLobbyNPCCharacterInfo& InAddInfo)
{
	LobbySceneManager.AddLobbyNPCCharacter(InType, InAddInfo);
}

void AB2LobbyGameMode::AddLobbyMainScenePointLights(const FLobbyMainScenePointLightInfo& InAddInfo)
{
	LobbySceneManager.AddLobbyMainScenePointLights(InAddInfo);
}

void AB2LobbyGameMode::AddLobbyFairyFlyingActor(EFairyType InType, class AB2LobbyFlyingFairyDoumi* fairyactor)
{
	LobbySceneManager.AddLobbyFairyFlyingActor(InType, fairyactor);
}

//void AB2LobbyGameMode::AddSummonItemDisplayMatinee(bool bIsMultiple, bool bIsOpen,bool bIsFriendShip, class ALevelSequenceActor * Matinee)
//{
//	LobbySceneManager.AddSummonItemDisplayMatinee(bIsMultiple, bIsOpen, bIsFriendShip, Matinee);
//}
//
//void AB2LobbyGameMode::AddSummonItemDisplayMatineeResult(class ALevelSequenceActor * Matinee)
//{
//	LobbySceneManager.AddSummonItemDisplayMatineeResult(Matinee);
//}

void AB2LobbyGameMode::PlaySummonItemDisplayMatineeResult()
{
	LobbySceneManager.PlaySummonItemDisplayMatineeResult();
}

//void AB2LobbyGameMode::OnFinishedHallOfFameDisplayMatinee(class ALevelSequenceActor * Matinee)
//{
//	LobbySceneManager.OnFinishedHallOfFameDisplayMatinee(Matinee);
//}

//void AB2LobbyGameMode::OnFinishedSummonItemDisplayMatinee(ALevelSequenceActor * Matinee)
//{
//	LobbySceneManager.OnFinishedSummonItemDisplayMatinee(Matinee);
//}

void AB2LobbyGameMode::OnFinishedSummonItemOpenState(bool State)
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UISummonItemDisplay* SummonItemDisplay = pUIManager->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);

		if (SummonItemDisplay)
			return SummonItemDisplay->SetOpenCardState(State);
	}
}


void AB2LobbyGameMode::OnFinishedSummonItemDisplayResultMatinee()
{
	LobbySceneManager.OnFinishedSummonItemDisplayResultMatinee();
}

//void AB2LobbyGameMode::OnChangedToFxSummonItemDisplayMatinee(ALevelSequenceActor * Matinee)
//{
//	LobbySceneManager.OnChangedToFxSummonItemDisplayMatinee(Matinee);
//}

void AB2LobbyGameMode::NotifyOpenCard(int32 InIndex)
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UISummonItemDisplay* SummonItemDisplay = pUIManager->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);

		if (SummonItemDisplay)
			SummonItemDisplay->NotifyOpenCard(InIndex);

	}
}

int32 AB2LobbyGameMode::OpenRemainCardCount()
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UISummonItemDisplay* SummonItemDisplay = pUIManager->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);

		if (SummonItemDisplay)
			return SummonItemDisplay->GetItemsCount();
	}
	return 0;
}

void AB2LobbyGameMode::BindTouchInput(bool bBind)
{
	if (bBind == bBindTouchInput)
		return;

	//bBindTouchInput = bBind;

	//auto* PlayerController = UGameplayStatics::GetLocalPlayerController(this);
	//if (PlayerController)
	//{
	//	auto* IC = PlayerController->InputComponent;
	//	if (IC)
	//	{
	//		if (bBindTouchInput)
	//		{
	//			//PC->
	//			IC->BindAxis("MoveRight", this, &AB2LobbyGameMode::SwipeH);
	//			IC->BindAxis("MoveForward", this, &AB2LobbyGameMode::SwipeV);

	//			//IC->BindAxisKey(EKeys::MouseX, this, &AB2LobbyGameMode::CameraMouseRotateYaw);
	//			IC->BindAxisKey(EKeys::MouseX, this, &AB2LobbyGameMode::MouseX);
	//			IC->BindAxisKey(EKeys::MouseY, this, &AB2LobbyGameMode::MouseY);
	//			auto& BindLFControlStart = IC->BindKey(EKeys::LeftControl, IE_Pressed, this, &AB2LobbyGameMode::LeftControlPressed);
	//			auto& BindLFControlEnd = IC->BindKey(EKeys::LeftControl, IE_Released, this, &AB2LobbyGameMode::LeftControlReleased);
	//			BindLFControlStart.bConsumeInput = false;
	//			BindLFControlEnd.bConsumeInput = false;

	//			auto& BindViewportTouchEnventStart = IC->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AB2LobbyGameMode::ViewportPressMouseEventStart);
	//			BindViewportTouchEnventStart.bConsumeInput = false;

	//			auto& BindViewportTouchEnventEnd = IC->BindKey(EKeys::LeftMouseButton, IE_Released, this, &AB2LobbyGameMode::OnReleasedLeftMouseButton);
	//			BindViewportTouchEnventEnd.bConsumeInput = false;
	//			//<- PC

	//			auto& BindTouchStart = IC->BindTouch(EInputEvent::IE_Pressed, this, &AB2LobbyGameMode::TouchStart);
	//			BindTouchStart.bConsumeInput = false;
	//			auto& BindTouchMove = IC->BindTouch(EInputEvent::IE_Repeat, this, &AB2LobbyGameMode::TouchMove);
	//			BindTouchMove.bConsumeInput = false;
	//			auto& BindTouchEnd = IC->BindTouch(EInputEvent::IE_Released, this, &AB2LobbyGameMode::TouchEnd);
	//			BindTouchEnd.bConsumeInput = false;

	//			IC->BindAction("ZoomOut", IE_Pressed, this, &AB2LobbyGameMode::CameraZoomOut);
	//			IC->BindAction("ZoomIn", IE_Pressed, this, &AB2LobbyGameMode::CameraZoomIn);

	//			PressedInputPosition = FVector2D::ZeroVector;
	//		}
	//		else
	//		{
	//			IC->ClearActionBindings();
	//			IC->TouchBindings.Reset();
	//			IC->AxisBindings.Reset();
	//			IC->AxisKeyBindings.Reset();
	//		}			
	//	}		
	//}
}

void AB2LobbyGameMode::ViewportPressMouseEventStart()
{
	FVector2D NewMousePosition;
	
	auto* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient)
		ViewportClient->GetMousePosition(NewMousePosition);

	ViewportTouchEventStart(false, NewMousePosition);
}

void AB2LobbyGameMode::ViewportTouchEventStart(bool bInputTypeTouch, const FVector2D& InPressedPosition)
{
	bIsInputTypeTouch = bInputTypeTouch;
	PressedInputPosition = InPressedPosition;
	SetTimeToIgnoreTouchEnd(GetTimeToDecideInputClickOrTab());
	
#if 0 //-> �ذ����� ���� ������ ���Ƽ� �ᱹ �� ����. �� ������ �ذ��ϴ� �� �̾��� ���������.
	// ��� ��鿡�� OnTouchImpulseRing �� ������ �ʴ� ���� �ش� ��忡 ���� ��ġ�� ���� widget ���� �Է� �ڵ鸵�� ���� ������ �߻��ϱ� ����.
	// ���� ��� ��ũ�ѹڽ� ��.. TouchEnd ó���� ������ �� ����..
	if (!bHeroMgmtMode && LobbySceneManager.GetCurrentLobbyScene() != ELobbyScene::ELobbyScene_Chapter
		&& LobbySceneManager.GetCurrentLobbyScene() != ELobbyScene::ELobbyScene_StageInfo)
	{
		LobbyConditionalBeginOnTouchImpulseRingClass<const FVector2D&>::GetInstance().Signal(InPressedPosition);
	}
#endif
	
	InputHistory_CheckFlick.InitializeHistory(
#if !UE_BUILD_SHIPPING
		TEXT("CheckFlick")
#endif
	);
	InputHistory_CheckFlick.AddCheckPoint(ElapsedTimeTouchAction, InPressedPosition);
	InputHistory_IgnoreCheck.InitializeHistory(
#if !UE_BUILD_SHIPPING
		TEXT("IgnoreCheck")
#endif
	);
	InputHistory_IgnoreCheck.AddCheckPoint(ElapsedTimeTouchAction, InPressedPosition);
}

void AB2LobbyGameMode::CheckAndDoSwipe(const FVector2D& CurrentPos, const FVector2D& PrevPos)
{
	//*	
	FVector2D ScreenSize(1.f, 1.f);
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient != nullptr)
		ViewportClient->GetViewportSize(ScreenSize);

	FVector2D divideScreenSize(360.0f / ScreenSize.X, 1 / ScreenSize.Y);
	FVector2D Delta( (CurrentPos - PrevPos) * divideScreenSize); //�ϴ� ������ ���� Delta�� ����
	
	if (!FMath::IsNearlyZero(Delta.X))
		SwipeH(Delta.X);
	if (!FMath::IsNearlyZero(Delta.Y))
		SwipeV(Delta.Y);
	/*/
	//Update
	float DeltaX = (CurrentPos.X - PrevPos.X);
	float DeltaY = (CurrentPos.Y - PrevPos.Y);

	FVector2D ScreenSize(1.f, 1.f);
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient != nullptr)
		ViewportClient->GetViewportSize(ScreenSize);

	if (!FMath::IsNearlyZero(DeltaX))
		SwipeH(DeltaX / ScreenSize.X * 2.f);
	if (!FMath::IsNearlyZero(DeltaY))
		SwipeV(DeltaY / ScreenSize.Y);
	//*/
}

void AB2LobbyGameMode::CheckAndDoFlick(const FVector2D& CurrentPos)
{
	FVector2D ScreenSize(1.f, 1.f);
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient != nullptr)
		ViewportClient->GetViewportSize(ScreenSize);

	float OutDeltaX = 0.f;
	InputHistory_CheckFlick.AddCheckPoint(ElapsedTimeTouchAction, CurrentPos);
	if (InputHistory_CheckFlick.IsSpeedOverDeltaX(ConsiderFlickScreenDeltaRatio * ScreenSize.X, ConsiderFlickDeltaTime, OutDeltaX))
	{
		FlickH(OutDeltaX / ScreenSize.X);

		// Ȯ�������� ������ FlickH �� ���������� �Ҹ��� ���� ��찡 �ִ�. InputHistory_CheckFlick �� �ʱ�ȭ�ϴ� ���� ���� �ʾ� ���⼭ �߰� ó���� �� ��.
		InputHistory_CheckFlick.InitializeHistory(
#if !UE_BUILD_SHIPPING
			TEXT("CheckFlick")
#endif
		);
	}
}

void AB2LobbyGameMode::TouchStart(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (!CurrentTouchData.bIsPressed)
	{
		CurrentTouchData.MainFingerIndex = FingerIndex;
		CurrentTouchData.MainFingerPrevLocation = FVector2D(Location);
		CurrentTouchData.bIsPressed = true;

		ViewportTouchEventStart(true, FVector2D(Location));
	}
	else if (CurrentTouchData.MainFingerIndex != FingerIndex && !CurrentTouchData.bIsPinching)
	{
		CurrentTouchData.SubFingerIndex = FingerIndex;
		CurrentTouchData.SubFingerPrevLocation = FVector2D(Location);
		CurrentTouchData.bIsPinching = true;

		ScreenZoomCenter = FVector2D((CurrentTouchData.MainFingerPrevLocation + CurrentTouchData.SubFingerPrevLocation) * 0.5f);
	}
}

void AB2LobbyGameMode::TouchMove(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (!CurrentTouchData.bIsPinching)
	{
		if (CurrentTouchData.bIsPressed && FingerIndex == CurrentTouchData.MainFingerIndex)
		{
			InputHistory_IgnoreCheck.AddCheckPoint(ElapsedTimeTouchAction, FVector2D(Location.X, Location.Y)); // TouchMove ���� �Է��� �����ϸ鼭 ��ġ �ʴ� ������ �Է��� ���Դ� �ǴܵǸ� Swipe �� �������� �ʵ���..

			if (InputHistory_IgnoreCheck.IsLastHistoryOdd(OddInputMinTimeInterval, OddInputMinPosDeltaRatio)) {
				// ���������� ���Ǵ� ��Ȳ���� TouchEnd �� �� ������ �̷� ó���� �ʿ���µ� ����� �Է� ��Ȳ���� �׷��� ���� ���� ���� �ִ�. 
				// �ÿ���å������ TouchEnd ���� ���۽��� �Է��� ���Դ� ���� ���� Swipe �� ���� �ʵ���. InputHistory ���� ��� �ʱ�ȭ.
				UE_LOG(LogBladeII, Log, TEXT("Ignore TouchMove and skip Swipe as Odd input is detected.. InputHistories are being initialized.."));
				InputHistory_IgnoreCheck.InitializeHistory();
				InputHistory_CheckFlick.InitializeHistory();
			}
			else {
				CheckAndDoSwipe(FVector2D(Location.X, Location.Y), CurrentTouchData.MainFingerPrevLocation);
			}
			
			CurrentTouchData.MainFingerPrevLocation = FVector2D(Location);
		}		
	}
	else
	{
		FVector2D CurrentLocation(Location);

		if (CurrentTouchData.MainFingerIndex == FingerIndex)
		{
			float OriginDist = (CurrentTouchData.MainFingerPrevLocation - CurrentTouchData.SubFingerPrevLocation).Size();
			float CurrentDist = (CurrentLocation - CurrentTouchData.SubFingerPrevLocation).Size();
			
			FVector2D ScreenSize(1.f, 1.f);
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
				ViewportClient->GetViewportSize(ScreenSize);

			float Delta = (OriginDist - CurrentDist) / ScreenSize.X;

			CurrentTouchData.MainFingerPrevLocation = CurrentLocation;

			CameraZoom(Delta * 65.f);
		}
		else if (CurrentTouchData.SubFingerIndex == FingerIndex)
		{
			float OriginDist = (CurrentTouchData.MainFingerPrevLocation - CurrentTouchData.SubFingerPrevLocation).Size();
			float CurrentDist = (CurrentTouchData.MainFingerPrevLocation - CurrentLocation).Size();

			FVector2D ScreenSize(1.f, 1.f);
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
				ViewportClient->GetViewportSize(ScreenSize);

			float Delta = (OriginDist - CurrentDist) / ScreenSize.X;

			CurrentTouchData.SubFingerPrevLocation = CurrentLocation;

			CameraZoom(Delta * 65.f);
		}
	}

	LobbyMoveOnTouchImpulseRingClass<const FVector2D&>::GetInstance().Signal(FVector2D(Location));
}

void AB2LobbyGameMode::TouchEnd(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	InputHistory_IgnoreCheck.InitializeHistory(
#if !UE_BUILD_SHIPPING
		TEXT("IgnoreCheck")
#endif
	);

	if (!CurrentTouchData.bIsPressed)
		return;

	if (!CurrentTouchData.bIsPinching)
	{
		if (FingerIndex == CurrentTouchData.MainFingerIndex)
		{
			CurrentTouchData.bIsPressed = false;
			CheckAndDoFlick(FVector2D(Location));
		}
	}
	else
	{
		if (FingerIndex == CurrentTouchData.MainFingerIndex || FingerIndex == CurrentTouchData.SubFingerIndex)
		{
			CurrentTouchData.bIsPinching = false;
			CurrentTouchData.bIsPressed = false;
		}
	}

	if (bPendingSetTouchingRotateAreaInHeroMgnt)
	{
		bIsTouchingRotateAreaInHeroMgnt = bPendingSetTouchingRotateAreaInHeroMgnt = false;
	}

	LobbyEndOnTouchImpulseRingClass<>::GetInstance().Signal();
}

void AB2LobbyGameMode::OnReleasedLeftMouseButton()
{ 
	SetPressedInputInUI(false); 

	FVector2D MousePosition(0.f, 0.f);

	auto* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient)
		ViewportClient->GetMousePosition(MousePosition);

	CheckAndDoFlick(MousePosition);

	if (bPendingSetTouchingRotateAreaInHeroMgnt)
	{
		bIsTouchingRotateAreaInHeroMgnt = bPendingSetTouchingRotateAreaInHeroMgnt = false;
	}

	LobbyEndOnTouchImpulseRingClass<>::GetInstance().Signal();
}

float AB2LobbyGameMode::GetHeroTowerUITowerPosition() const
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UIHeroTowerReady* pHeroTowerReadyUI = pUIManager->GetUI<UB2UIHeroTowerReady>(UIFName::HeroTowerReady);

		if (pHeroTowerReadyUI)
			return pHeroTowerReadyUI->GetTowerPosition();
	}

	return 0.0f;
}

EHeroTowerReadyUIState AB2LobbyGameMode::GetHeroTowerUIState() const
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UIHeroTowerReady* pHeroTowerReadyUI = pUIManager->GetUI<UB2UIHeroTowerReady>(UIFName::HeroTowerReady);

		if (pHeroTowerReadyUI)
			return pHeroTowerReadyUI->GetHeroTowerUIState();
	}

	return EHeroTowerReadyUIState::None;
}

int32 AB2LobbyGameMode::GetHeroTowerStartFloor() const
{
	return UB2UIDocHelper::GetDocHeroTower()->GetStartFloor();
}

int32 AB2LobbyGameMode::GetHeroTowerMaxFloor() const
{
	return BladeIIGameImpl::GetClientDataStore().GetHeroTowerMaxFloor();
}

bool AB2LobbyGameMode::IsHeroTowerMaxFloorCleared() const
{
	return UB2UIDocHelper::GetDocHeroTower()->GetMaxClearFloor() >= BladeIIGameImpl::GetClientDataStore().GetHeroTowerMaxFloor();
}

void AB2LobbyGameMode::GuildMapInitCarmeraPosSetting(const FVector& Camera1, const FVector& Camera2, const FVector& Camera3)
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
		UB2UIGuildMapMain* pGuildMap = pUIManager->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

		if (pGuildMap)
			return pGuildMap->CameraPosSetting(Camera1.Y, Camera2.Y, Camera3.Y);
	}
}

void AB2LobbyGameMode::GuildMapFinishMatinee()
{

	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (!pUIManager)
		return;

	UB2UIGuildMapMain* pGuildMapMain = pUIManager->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

	if (!pGuildMapMain)
		return;

	pGuildMapMain->FinishMatineeSetting();
}



void AB2LobbyGameMode::GuildMapMoveScroll(ACameraActor* CameraActor)
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (!pUIManager)
		return;

	UB2UIGuildMapMain* pGuildMapMain = pUIManager->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

	if (!pGuildMapMain)
		return;

	float ScrollOffSet = pGuildMapMain->ScrollCameraSetting();

	FVector CameraLocation;

	CameraLocation =CameraActor->GetActorLocation();
	CameraActor->SetActorLocation(FVector(CameraLocation.X, ScrollOffSet, CameraLocation.Z));


}

void AB2LobbyGameMode::GuildMapDoneMoveScroll(ACameraActor* CameraActor)
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (!pUIManager)
		return;

	UB2UIGuildMapMain* pGuildMapMain = pUIManager->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

	if (!pGuildMapMain)
		return;

	float ScrollOffSet = pGuildMapMain->DoneMoveCameraSetting();

	FVector CameraLocation;

	CameraLocation = CameraActor->GetActorLocation();
	CameraActor->SetActorLocation(FVector(CameraLocation.X, ScrollOffSet, CameraLocation.Z));
}

#define SET_GUILDCASTLE_EFFECT(Index , Location)											\
	if (Index == static_cast<int32>(EUIGuildSelectEffect::End)){							\
		for (auto& El : GuildEffects)														\
		{																					\
			El->GetRootComponent()->SetVisibility(false);									\
		}																					\
		return;																				\
	}																						\
	GuildEffects[Index]->SetActorLocation(Location);										\
	GuildEffects[Index]->GetRootComponent()->SetVisibility(true);							\



void AB2LobbyGameMode::SetGuildSelectEffects(AEmitter* Maker_01, AEmitter* Maker_02, AEmitter* Maker_03, AEmitter* Maker_04)
{
	GuildEffects.Empty();

	GuildEffects.SetNum(4);
	GuildEffects[0] = Maker_01;
	GuildEffects[1] = Maker_02;
	GuildEffects[2] = Maker_03;
	GuildEffects[3] = Maker_04;

	SET_GUILDCASTLE_EFFECT(static_cast<int32>(EUIGuildSelectEffect::End), FVector(0, 0, 0));
}
void AB2LobbyGameMode::SetGuildEffectsLocation(int32 Type, FVector EffectLocation)
{
	if (Type == static_cast<int32>(EUIGuildSelectEffect::Rival))
	{
		SET_GUILDCASTLE_EFFECT(0 , EffectLocation)
	}
	else if (Type == static_cast<int32>(EUIGuildSelectEffect::Team))
	{
		SET_GUILDCASTLE_EFFECT(2, EffectLocation)
		GuildEffects[2]->GetRootComponent()->SetVisibility(EffectLocation == GuildEffects[1]->GetActorLocation() ? false : true);
	}
	else if (Type == static_cast<int32>(EUIGuildSelectEffect::MyCastle))
	{
		SET_GUILDCASTLE_EFFECT(1, EffectLocation)
	}
	else
	{
		SET_GUILDCASTLE_EFFECT(static_cast<int32>(EUIGuildSelectEffect::End), EffectLocation)
	}
}

void AB2LobbyGameMode::SelectLobbyFocusPCClass(EPCClass SelectClass)
{
	if (SelectClass == EPCClass::EPC_End)
		LobbySceneManager.ChangeLobbyScene(ELobbyScene::ELobbyScene_LobbyMain);
	
	else
		LobbySceneManager.ChangeLobbyScene(ELobbyScene::ELobbyScene_CharacterObserve);
}

EPCClass AB2LobbyGameMode::GetSuitableFocusPCClass()
{
	int32 SuitablePCClassIdx = GetMaxPCClassNum();
	int32 HighestCombatPower = 0;

	for (int32 ClassIdx = 0; ClassIdx < GetMaxPCClassNum(); ClassIdx++)
	{
		if (UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(ClassIdx))
		{
			const int32 ClassCombatPower = DocHero->GetCombatPower();
			if (ClassCombatPower > HighestCombatPower)
			{
				HighestCombatPower = ClassCombatPower;
				SuitablePCClassIdx = ClassIdx;
			}
		}
	}

	const EPCClass SuitablePCClass = IntToPCClass(SuitablePCClassIdx);
	if (SuitablePCClass == EPCClass::EPC_End)
		return EPCClass::EPC_Assassin;

	return SuitablePCClass;
}

bool AB2LobbyGameMode::IsFirstTimePreRenderPending() const
{ // ���� ��ġ �� ù ����. ������ ���� ���� ���� �� �� �� ���� ��..
	return PreRenderer && PreRenderer->IsPendingForPreRenderGM();
}

void AB2LobbyGameMode::ToggleShowLobbyWing()
{
	bTestShowLobbyWing ^= true;
	LobbySceneManager.SetUnconditionalShowWing(bTestShowLobbyWing);
}

void AB2LobbyGameMode::SetPeerTeam()
{
	if (GetPeerType() != PEER_SUPER)
		return;

	//������ ���� ����

	// ���. ȣ��Ʈ�� �Ͼ��.
	UE_LOG(LogBladeII, Log, TEXT("[SetPeerTeam] RED = %d, Bule = %d"), GetMatchPlayerNum(ETeamType::ETT_Red), GetMatchPlayerNum(ETeamType::ETT_Blue));
	NetSendMatchPlayerTeamInfo();
}

uint8 AB2LobbyGameMode::GetPlayerStartIndexByNetId(uint32 NetId, ETeamType PlayerTeamType)
{
	// ȣ��Ʈ�� �̰� ����Ѵ�.
	check(GetPeerType() == PEER_SUPER);

	uint8 nStartIndex = 1;

	for (TPair<uint32, FLobbyMatchPeerInfo>& peerInfo : AllPeerInfos)
	{
		if (peerInfo.Key == NetId)
		{
			if (PlayerTeamType == ETeamType::ETT_Red)
				return (nStartIndex * 2) - 1;
			if (PlayerTeamType == ETeamType::ETT_Blue)
				return nStartIndex * 2;
		}

		if (peerInfo.Value.PlayerTeamType == PlayerTeamType)
			nStartIndex++;
	}

	return 0;
}

void AB2LobbyGameMode::GetControlPlayerInfo(TArray<FB2PeerInfo>& InPeerInfos)
{
	InPeerInfos.Sort([](FB2PeerInfo A, FB2PeerInfo B)->bool {
		bool Result = false;
		if (A.MatchGrade < B.MatchGrade)
		{
			Result = true;
		}
		else if (A.MatchGrade == B.MatchGrade)
		{
			if (A.MatchStar > B.MatchStar)
			{
				Result = true;
			}
		}
		return Result;
	});

	if (InPeerInfos.Num() == CONTROL_GAME_MAX_PLAYERS)
	{
		if (FMath::RandBool() == true)
		{
			//[JH][todo] �ٸ���(AllPeerInfos)���� �������� �ϴ� �κ��� �־ ������ �ʿ���
			for (int i = 0; i < InPeerInfos.Num(); i++)
			{
				if (InPeerInfos.IsValidIndex(i))
				{
					bool isOdd = (i & 1) == 1; // i % 2 == 1
					if (isOdd)
					{
						InPeerInfos[i].PlayerTeamType = ETeamType::ETT_Red;
						InPeerInfos[i].PlayerStartIndex = i;
					}
					else
					{
						InPeerInfos[i].PlayerTeamType = ETeamType::ETT_Blue;
						InPeerInfos[i].PlayerStartIndex = i + 2;
					}
				}
			}
		}
		else
		{
			InPeerInfos[0].PlayerTeamType = ETeamType::ETT_Blue;
			InPeerInfos[0].PlayerStartIndex = 2;
			InPeerInfos[3].PlayerTeamType = ETeamType::ETT_Blue;
			InPeerInfos[3].PlayerStartIndex = 4;
			InPeerInfos[4].PlayerTeamType = ETeamType::ETT_Blue;
			InPeerInfos[4].PlayerStartIndex = 6;
			
			InPeerInfos[1].PlayerTeamType = ETeamType::ETT_Red;
			InPeerInfos[1].PlayerStartIndex = 1;
			InPeerInfos[2].PlayerTeamType = ETeamType::ETT_Red;
			InPeerInfos[2].PlayerStartIndex = 3;
			InPeerInfos[5].PlayerTeamType = ETeamType::ETT_Red;
			InPeerInfos[5].PlayerStartIndex = 5;
		}
	}
	else
	{
		for (int cnt = 0; cnt < InPeerInfos.Num(); cnt++)
		{
			if (cnt % 2 == 0)
			{
				InPeerInfos[cnt].PlayerTeamType = ETeamType::ETT_Red;
			} 
			else
			{
				InPeerInfos[cnt].PlayerTeamType = ETeamType::ETT_Blue;
			}
		}

		int32 RedIDX = 1;
		int32 BlueIDX = 1;
		for (int cnt = 0; cnt < InPeerInfos.Num(); cnt++)
		{
			if (InPeerInfos[cnt].PlayerTeamType == ETeamType::ETT_Red)
			{
				InPeerInfos[cnt].PlayerStartIndex = (RedIDX * 2) - 1;
				RedIDX++;
			}
			else
			{
				InPeerInfos[cnt].PlayerStartIndex = BlueIDX * 2;
				BlueIDX++;
			}
		}
	}
}

void AB2LobbyGameMode::GetTutorialControlPlayerInfo(TArray<FB2PeerInfo>& InPeerInfos)
{
	int32 RedIDX = 1;
	int32 BleuIDX = 1;
	for (FB2PeerInfo& Elem : InPeerInfos)
	{
		if (Elem.PlayerTeamType == ETeamType::ETT_Red)
		{
			Elem.PlayerStartIndex = (RedIDX * 2) - 1;
			RedIDX++;
		}
		else if (Elem.PlayerTeamType == ETeamType::ETT_Blue)
		{
			Elem.PlayerStartIndex = BleuIDX * 2;
			BleuIDX++;
		}
	}
}

void AB2LobbyGameMode::StartControlTutorial()
{
	ClearMatchRoom();

	// ��¥ �÷��̾� ����� ������ 101 ~ 108�� �����ɵ�. ���ڴ� ū�ǹ� ����
	uint32 nFakePlayerNetId = 102;
	uint32 nCurrentFakePlayerNetId = nFakePlayerNetId;

	SetPeerType(PEER_SUPER);

	// ����ũ �ݾ��̵� �ٷξռ��ڸ� ������.
	SetMyNetId(nFakePlayerNetId - 1);

	EmplacePeerPlayer(GetMyNetId());

	// ����� �ѹ� ��������
	if (AllPeerInfos.Contains(GetMyNetId()))
	{
		AllPeerInfos[GetMyNetId()].PlayerTeamType = ETeamType::ETT_Red;
	}

	const TArray<FTutorialControlBotInfo>* TutorialControlBotInfos = TutorialManager::GetInstance().GetTutorialControlBotInfos();

	for (FTutorialControlBotInfo TutorialControlBotInfo : *TutorialControlBotInfos)
	{
		//7�� ������ �ȵȴ�.
		if (nCurrentFakePlayerNetId >= nFakePlayerNetId + 7)
		{
			check(0)
			return;
		}

		// ����Ʈ ������ �ϳ� �߰�
		RemotePlayerInfoData.Add(nCurrentFakePlayerNetId);

		b2network::B2FindAccountInfoPtr pAccountInfo = std::make_shared<b2network::B2FindAccountInfo>();
		pAccountInfo->nickname = TutorialControlBotInfo.BotName;
		
		pAccountInfo->characters.Add(std::make_shared<b2network::B2CharacterServerInfo>());
		pAccountInfo->characters[0]->character_type = CliToSvrPCClassType(TutorialControlBotInfo.BotPCClass);
		
		pAccountInfo->characters[0]->skill_quick_slots.Add(std::make_shared<b2network::B2SkillQuickSlot>());
		pAccountInfo->characters[0]->skill_quick_slots[0]->using_skill_id_1 = TutorialControlBotInfo.BotSkillId;
		
		pAccountInfo->characters[0]->skills.Add(std::make_shared<b2network::B2SkillServerInfo>());
		pAccountInfo->characters[0]->skills[0]->skill_id = TutorialControlBotInfo.BotSkillId;
		pAccountInfo->characters[0]->skills[0]->skill_level = 1;

		pAccountInfo->characters[0]->level = TutorialControlBotInfo.BotLevel;

		//������ �߰�
		for (FTutorialControlBotItemInfo TutorialControlBotItemInfo : TutorialControlBotInfo.BotEquipItem)
		{
			b2network::B2ItemServerInfoPtr pItemInfo = std::make_shared<b2network::B2ItemServerInfo>();
			pItemInfo->character_type = pAccountInfo->characters[0]->character_type;
			pItemInfo->template_id = TutorialControlBotItemInfo.ItemTemplateId;

			// ��¥ ������ ���̵�(����)�� ���ø����̵� �״�ξ� �׳�(�ߺ��� �ȵǸ� ���������)
			pItemInfo->id = pItemInfo->template_id;

			pItemInfo->equip_position = TutorialControlBotItemInfo.ItemEquipPos;
			pItemInfo->amount = 1;
			pItemInfo->grade = 1;
			pItemInfo->quality = 1;
			pItemInfo->level = 1;
			// �̰ɷ� ����ü ����(���� �������� ���������� ������ ����)
			pItemInfo->md_main_attribute_value = TutorialControlBotItemInfo.ItemBaseStat;

			pAccountInfo->equip_items.Add(pItemInfo);

			b2network::B2ItemPresetPtr pItemPreset = std::make_shared<b2network::B2ItemPreset>();
			pItemPreset->character_type = pAccountInfo->characters[0]->character_type;
			pItemPreset->preset_id = 0;
			pItemPreset->equip_position = pItemInfo->equip_position;
			pItemPreset->item_id = pItemInfo->id;
			pAccountInfo->item_presets.Add(pItemPreset);
		}

		// ����Ʈ �÷��̾������� �߰�.
		RemotePlayerInfoData[nCurrentFakePlayerNetId].SetModPlayerInfo(pAccountInfo);
		RemotePlayerInfoData[nCurrentFakePlayerNetId].SetMainPlayerClass(TutorialControlBotInfo.BotPCClass);

		AllPeerInfos.Emplace(nCurrentFakePlayerNetId, FLobbyMatchPeerInfo());
		AllPeerInfos[nCurrentFakePlayerNetId].ReservedPCClass = TutorialControlBotInfo.BotPCClass;
		AllPeerInfos[nCurrentFakePlayerNetId].PlayerTeamType = TutorialControlBotInfo.BotTeamType;
		
		// ����ũ �ݾ��̵� �÷���
		nCurrentFakePlayerNetId++;
	}
	// ��¥ �÷��̾� ����� ����

	// ��Ŭ���� ��ŵ�ϱ� �׵ڷ� ����
	SendPrerequisiteToStart(b2network::B2RoomType::BRT_Assault);
}

void AB2LobbyGameMode::NoResponsePrerequisiteToStart()
{
	packet::ByteStream	payload;
	FString encoded_string = packet::FinalizePacket(packet::NO_RESPONSE_PREREQUISITE_TO_START, GetMyNetId(), 0, packet::ALL, payload);
	data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);
}

void AB2LobbyGameMode::ClearWaitPrerequisiteToStartTimeHandler()
{
	GetWorldTimerManager().ClearTimer(WaitPrerequisiteToStartTimeHandler);
}

FVector2D AB2LobbyGameMode::GetTouchingInputCenter()
{
	FVector2D Center(0.f, 0.f);

	auto* ViewportClient = GetWorld()->GetGameViewport();
	BII_CHECK(ViewportClient);
	if (ViewportClient)
	{
		/*
		ViewportClient->GetViewportSize(Center);

		Center.X *= 0.5f;
		Center.Y *= 0.5f;
		*/

		if (CurrentTouchData.bIsPinching)
		{
			Center.Y = ScreenZoomCenter.Y;
		}
		else
		{
			FVector2D MousePosition(0.f, 0.f);
			ViewportClient->GetMousePosition(MousePosition);

			Center.Y = MousePosition.Y;
		}
	}
	
	return Center;
}

//////////////////////////////////////////////////
//
// Camera work in Lobby
//

void AB2LobbyGameMode::CameraZoomIn()
{
	CameraZoom(-1.f);
}

void AB2LobbyGameMode::CameraZoomOut()
{
	CameraZoom(1.f);
}

void AB2LobbyGameMode::CameraZoom(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		LobbySceneManager.Zoom(Value, GetTouchingInputCenter());
	}		
}

void AB2LobbyGameMode::SwipeH(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		LobbySceneManager.SwipeH(-Value);
	}		
}

void AB2LobbyGameMode::SwipeV(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		LobbySceneManager.SwipeV(-Value);
	}		
}

void AB2LobbyGameMode::FlickH(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		LobbySceneManager.FlickH(Value < 0.f);
		
		SwipeH(Value);

		BeginLobbySceneFreeRotate(Value);
	}
}

void AB2LobbyGameMode::BeginLobbySceneFreeRotate(float InLastSwipeValue)
{ // ��ġ and swipe �� ������ ������ ���� ȸ�� ����.
	bDeacceleratingCharRotate = true;
	DeaccelerationCharRotate = (InLastSwipeValue < 0.f ? 1.f : -1.f) * LobbySceneManager.GetCharacterCameraRotateDeaccelerationFactor(); // �����ߴ� ����� �ݴ�� deaccelerate
	RotationSpeedCharRotate = InLastSwipeValue * LobbySceneManager.GetCharacterCameraRotateFactor(); // ���� ���� ���� �ʱⰪ. FLobbySceneManager::SwipeH ó������ ������ �ӵ��� �̾�������.
	RotationSpeedCharRotate = FMath::Clamp(RotationSpeedCharRotate, -1.0f * LobbySceneManager.GetCharacterCameraFreeRotateMaxSpeed(), LobbySceneManager.GetCharacterCameraFreeRotateMaxSpeed());
	// ī�޶� ������ �Ŷ� ĳ���� ������ �Ŷ� �������� �� �޶� �Ϻ� �������� �߰� �������� �ִµ�
	// �̷��� �� �̸��� ���� ����ϴ� ���̸� ���������ؼ� �̽����� ������ �̽��� ���ٰ� �ؼ� ū�� �� �͵� �ƴϰ� �ϴ� �̷��� ���� ��.
	if (LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_HeroManagement ||
		LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_CollectBookMain ||
		LobbySceneManager.GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_FindAccount)
	{
		RotationSpeedCharRotate *= LobbySceneManager.GetCharacterFreeRotateAdditionalScale();
	}

	UE_LOG(LogBladeII, Log, TEXT("[BeginLobbySceneFreeRotate], DeaccelerationCharRotate %.3f  RotationSpeedCharRotate %.3f"), DeaccelerationCharRotate, RotationSpeedCharRotate);
}

void AB2LobbyGameMode::FlickV(float Value)
{

}

void AB2LobbyGameMode::MouseMoved()
{
	//auto* ViewportClient = GetWorld()->GetGameViewport();

	//if (ViewportClient)
	//{
	//	FVector2D NewMousePosition;
	//	if (ViewportClient->GetMousePosition(NewMousePosition))
	//	{
	//		auto* PlayerController = UGameplayStatics::GetLocalPlayerController(this);
	//		if (bIsInputLeftControl)
	//		{
	//			FVector2D CurrentLocation(NewMousePosition);

	//			FVector2D ScreenSize(1.f, 1.f);
	//			if (ViewportClient != nullptr)
	//				ViewportClient->GetViewportSize(ScreenSize);

	//			CurrentTouchData.SubFingerPrevLocation = FVector2D(CurrentTouchData.MainFingerPrevLocation.X - ScreenSize.X * 0.5f, CurrentTouchData.MainFingerPrevLocation.Y - ScreenSize.Y * 0.5f);

	//			float OriginDist = (CurrentTouchData.MainFingerPrevLocation - CurrentTouchData.SubFingerPrevLocation).Size();
	//			float CurrentDist = (CurrentLocation - CurrentTouchData.SubFingerPrevLocation).Size();

	//			float Delta = (OriginDist - CurrentDist) / ScreenSize.X;

	//			CurrentTouchData.MainFingerPrevLocation = CurrentLocation;

	//			CameraZoom(Delta * 65.f);

	//		}
	//		else if (bPressedInputInUI || (PlayerController && PlayerController->IsInputKeyDown(EKeys::LeftMouseButton)))
	//		{
	//			CheckAndDoSwipe(NewMousePosition, PrevMousePosition);
	//		}

	//		PrevMousePosition = NewMousePosition;

	//		LobbyMoveOnTouchImpulseRingClass<const FVector2D&>::GetInstance().Signal(NewMousePosition);
	//	}
	//}
}

void AB2LobbyGameMode::MouseX(float Value)
{
	MouseMoved();
}

void AB2LobbyGameMode::MouseY(float Value)
{
	MouseMoved();
}

void AB2LobbyGameMode::LeftControlPressed()
{
	if(!bIsInputLeftControl)
	{
		auto* ViewportClient = GetWorld()->GetGameViewport();
		if (ViewportClient)
		{
			FVector2D NewMousePosition;
			if (ViewportClient->GetMousePosition(NewMousePosition))
			{
				CurrentTouchData.MainFingerPrevLocation = NewMousePosition;
				UE_LOG(LogBladeII, Log, TEXT("LeftControlPressed MainFingerPrevLocation X : %f, Y : %f"), CurrentTouchData.MainFingerPrevLocation.X, CurrentTouchData.MainFingerPrevLocation.Y);
			}
		}
	}

	bIsInputLeftControl = true;
}

void AB2LobbyGameMode::LeftControlReleased()
{
	bIsInputLeftControl = false;
}

FVector GetViewportSpacePostion(const FGeometry& Geometry, const FPointerEvent& Event)
{
	//FPointerEvent�� ScreenSpacePosition�� PC ����� ������ ��ǥ�̱� ������ ���Ӻ������� ��ȯ�� �ʿ���.
	return FVector(Geometry.Position + Geometry.AbsoluteToLocal(Event.GetScreenSpacePosition()), 0.f);
}

#include "Input/Events.h"
FReply AB2LobbyGameMode::TouchStartFromUI(const FGeometry& Geometry, const FPointerEvent& Event)
{
	UE_LOG(LogBladeII, Log, TEXT("TouchStartFromUI"));
	TouchStart(static_cast<ETouchIndex::Type>(Event.GetPointerIndex()), GetViewportSpacePostion(Geometry, Event));

	return FReply::Handled();
}

FReply AB2LobbyGameMode::TouchEndFromUI(const FGeometry& Geometry, const FPointerEvent& Event)
{
	UE_LOG(LogBladeII, Log, TEXT("TouchFromFromUI"));
	TouchEnd(static_cast<ETouchIndex::Type>(Event.GetPointerIndex()), GetViewportSpacePostion(Geometry, Event));

	return FReply::Handled();
}

FReply AB2LobbyGameMode::TouchMoveFromUI(const FGeometry& Geometry, const FPointerEvent& Event)
{
	UE_LOG(LogBladeII, Log, TEXT("TouchMoveFromUI"));
	TouchMove(static_cast<ETouchIndex::Type>(Event.GetPointerIndex()), GetViewportSpacePostion(Geometry, Event));

	return FReply::Handled();
}

bool AB2LobbyGameMode::IsIgnoreTouchEnd()
{ 
	if (TimeLimitTouchToClick < 0.f || ElapsedTimeTouchAction < TimeLimitTouchToClick)
	{
		FVector2D CurInputPosition(FLT_MAX, FLT_MAX);

		auto* ViewportClient = GetWorld()->GetGameViewport();
		check(ViewportClient);
		
		if (bIsInputTypeTouch)
		{
			if (CurrentTouchData.bIsPinching)
				return true;

			CurInputPosition = FVector2D(CurrentTouchData.MainFingerPrevLocation);
		}
		else
		{				
			ViewportClient->GetMousePosition(CurInputPosition);
		}

		FVector2D ViewportSize(1.f, 1.f);
		ViewportClient->GetViewportSize(ViewportSize);

		FVector2D OldInputPosition = PressedInputPosition;
		PressedInputPosition = FVector2D::ZeroVector;

		return (CurInputPosition - OldInputPosition).Size() / ViewportSize.X > 0.04f;
	}

	return true; 
}

void AB2LobbyGameMode::SetTimeToIgnoreTouchEnd(float LimitTime)
{
	ElapsedTimeTouchAction = 0.f;
	TimeLimitTouchToClick = LimitTime;
}

void AB2LobbyGameMode::TickTimeTouchAction(float DeltaSeconds)
{
	if (TimeLimitTouchToClick > 0.f)
	{
		ElapsedTimeTouchAction += DeltaSeconds;
		
		InputHistory_CheckFlick.AddCheckPoint(ElapsedTimeTouchAction, bIsInputTypeTouch ? CurrentTouchData.MainFingerPrevLocation : PrevMousePosition);

		if (bDeacceleratingCharRotate)
		{
			RotationSpeedCharRotate += DeaccelerationCharRotate * DeltaSeconds;

			if (RotationSpeedCharRotate * DeaccelerationCharRotate >= 0) // ���� ����� ������ �ݴ밡 �Ǹ� �ߴ�.
			{
				UE_LOG(LogBladeII, Log, TEXT("[TickTimeTouchAction] Stopping Deacceleration.. DeaccelerationCharRotate %.3f, RotationSpeedCharRotate %.3f"),
					DeaccelerationCharRotate, RotationSpeedCharRotate);

				RotationSpeedCharRotate = 0.0f;
				bDeacceleratingCharRotate = false;
				InputHistory_CheckFlick.InitializeHistory();
			}
			else
			{
				SwipeH(RotationSpeedCharRotate);
			}
		}
	}		
}

#if !UE_BUILD_SHIPPING
void AB2LobbyGameMode::DevOnlyGenerateDummyChat(float DeltaSeconds)
{
	//static float AccumulatedTime = 0.0f;

	//AccumulatedTime += DeltaSeconds;
	//if (AccumulatedTime > 0.1f)
	//{
	//	FB2ChatMessage NewMsgInfo;
	//	NewMsgInfo.SendUserNick = (FMath::RandBool() == true) ? FText::FromString(TEXT("DummyChatUser")) : FText::FromString(TEXT("DumbAss"));
	//	NewMsgInfo.MessageText = (FMath::RandBool() == true) ?
	//		FText::FromString(TEXT("Short dummy chat message.")) :
	//		FText::FromString(TEXT("This is dummy chat message.. This is dummy chat message. This is dummy chat message"));

	//	BladeIIGameImpl::GetChatStore().AddChatMessage(EB2ChatType::Channel, NewMsgInfo);

	//	// ���� ä�õ� �׽�Ʈ �� �� �ְ�..
	//	NewMsgInfo.MessageType = EB2ChatType::Channel;
	//	UpdateChatSinglePopupClass<const FB2ChatMessage&>::GetInstance().Signal(NewMsgInfo);

	//	AccumulatedTime = 0.0f;
	//}

	//static float AccumulatedTimeForPublicNoti = 0.0f;
	//AccumulatedTimeForPublicNoti += DeltaSeconds;
	//if (AccumulatedTimeForPublicNoti > 0.1f)
	//{
	//	FB2ChatMessage NewMsgInfo;
	//	NewMsgInfo.SendUserNick = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti"));
	//	NewMsgInfo.MessageText = (FMath::RandBool() == true) ?
	//		FText::FromString(TEXT("Short dummy public announcement message.")) :
	//		FText::FromString(TEXT("This is dummy public announcement message.. This is dummy public announcement message. This is dummy announcement message"));

	//	BladeIIGameImpl::GetChatStore().AddChatMessage(EB2ChatType::Notice, NewMsgInfo);

	//	// ���� ä�õ� �׽�Ʈ �� �� �ְ�..
	//	NewMsgInfo.MessageType = EB2ChatType::Notice;
	//	UpdateChatSinglePopupClass<const FB2ChatMessage&>::GetInstance().Signal(NewMsgInfo);

	//	AccumulatedTimeForPublicNoti = 0.0f;
	//}

	//UpdateChatUIClass<bool>::GetInstance().Signal(true);
}
#endif

void AB2LobbyGameMode::UpdateChatUI(bool bIsScrollEnd)
{
}

void AB2LobbyGameMode::ExtraOnPreLoadMap()
{
	if (LobbyUIManager)
	{ // �� ���� �ÿ� DJLegacy UI �� �ε��� TAsset �� Unload �ʿ�..
		LobbyUIManager->OnPreLoadMap();
	}

	// ù ������ �ε� �� �ó����� ������ ��� �����ϰ� Ÿ��Ʋ �������� ���´ٴ� ������ �־.. ������ �� ���� ������ ���� �ϳ��� ���ɼ� ����
	if (!IsFirstLobby)
	{
		AB2DLCFrontGameMode::ClearSelectedFrontTitleMovieInfo();
	}
}

void AB2LobbyGameMode::OnPlatformPause()
{
	// 1. �ʴ� ���� - �ٸ� �׼ǵ��� �ʿ��ϸ� �Լ��� ������
	const TArray<EB2GameMode> InvGameModeList{ EB2GameMode::PVP_Tag, EB2GameMode::Raid };

	for (const auto GameModeType : InvGameModeList)
	{
		if (HasGameInvitation(GameModeType))
			RequestDeclineInvitation(GameModeType);
	}
}

void AB2LobbyGameMode::FInputHistory::InitializeHistory(
#if !UE_BUILD_SHIPPING
	const FString& OptionalLoggingHistoryName
#endif
)
{
#if !UE_BUILD_SHIPPING
	if (OptionalLoggingHistoryName.Len() > 0) // ����� �α� �Ѹ��� �����ϴ� �Ű������ε�.
	{
		UE_LOG(LogBladeII, Log, TEXT("InputHistory %s Initialize.. InputHistoryLastIndex %d"), *OptionalLoggingHistoryName, InputHistoryLastIndex);
	}
#endif

	InputHistoryLastIndex = INDEX_NONE;
	FMemory::Memzero(&InputPointHistory, sizeof(FInputHistoryElem) * MAX_INPUT_HISTORY_INDEX);
}

void AB2LobbyGameMode::FInputHistory::AddCheckPoint(float Time, const FVector2D& Position)
{
	if (InputPointHistory[InputHistoryLastIndex].CheckPointTime == Time)
		return;

	check(InputHistoryLastIndex + 1 > -1);

	InputHistoryLastIndex = (InputHistoryLastIndex + 1) % MAX_INPUT_HISTORY_INDEX;

	InputPointHistory[InputHistoryLastIndex].CheckPointTime = Time;
	InputPointHistory[InputHistoryLastIndex].CheckPointPosition = Position;	
	InputPointHistory[InputHistoryLastIndex].bInitialized = true;
}

bool AB2LobbyGameMode::FInputHistory::IsSpeedOverDeltaX(float DeltaX, float Time, float& OutDeltaX)
{
	for (int32 i = GetPrevHistoryIndexOf(InputHistoryLastIndex); i != InputHistoryLastIndex  ; i = GetPrevHistoryIndexOf(i))
	{
		if (!InputPointHistory[i].bInitialized)
			return false;

		if (InputPointHistory[InputHistoryLastIndex].CheckPointTime - InputPointHistory[i].CheckPointTime > Time)
		{
			return false;
		}
				
		if (DeltaX < FMath::Abs(InputPointHistory[InputHistoryLastIndex].CheckPointPosition.X - InputPointHistory[i].CheckPointPosition.X))
		{			
			OutDeltaX = InputPointHistory[InputHistoryLastIndex].CheckPointPosition.X - InputPointHistory[(InputHistoryLastIndex + MAX_INPUT_HISTORY_INDEX - 1) % MAX_INPUT_HISTORY_INDEX].CheckPointPosition.X;
			OutDeltaX = OutDeltaX * (1.f - (InputPointHistory[InputHistoryLastIndex].CheckPointTime - InputPointHistory[(InputHistoryLastIndex + MAX_INPUT_HISTORY_INDEX - 1) % MAX_INPUT_HISTORY_INDEX].CheckPointTime));
			return true;
		}			
	}

	return false;
}

bool AB2LobbyGameMode::FInputHistory::IsLastHistoryOdd(float MinOddTimeInterval, float MinOddPosDeltaRatio)
{
	if (InputHistoryLastIndex < 0) {
		return false; // History �� ����. �Ǻ� �Ұ�
	}

	int32 AddedOldHistoryCount = 0;
	FVector2D AddedCheckPointDelta = FVector2D(0.0f,0.0f);
	// �������� �����ϰ� ���ΰ��� delta �� ����
	for (int32 IA = GetPrevHistoryIndexOf(InputHistoryLastIndex); IA != GetNextHistoryIndexOf(InputHistoryLastIndex); IA = GetPrevHistoryIndexOf(IA))
	{
		const int32 IB = GetPrevHistoryIndexOf(IA);

		const FInputHistoryElem& InputHistoryA = InputPointHistory[IA];
		const FInputHistoryElem& InputHistoryB = InputPointHistory[IB]; // A ���� �ѹ� �� ������

		if (!InputHistoryA.bInitialized || !InputHistoryB.bInitialized) {
			continue; // �� �� ���ǹ��� ���� �־�߰ڴ�.
		}
		++AddedOldHistoryCount;

		AddedCheckPointDelta = InputHistoryA.CheckPointPosition - InputHistoryB.CheckPointPosition;
	}

	if (AddedOldHistoryCount >= 2) // ���� ���̿� ���Ϸ��� ���̴� �ƿ� ���ų� �ϳ��� �������� �� �Ұ���.
	{
		const FInputHistoryElem& LastHistory = InputPointHistory[InputHistoryLastIndex];
		const FInputHistoryElem& LastLastHistory = InputPointHistory[GetPrevHistoryIndexOf(InputHistoryLastIndex)];
		checkSlow(LastHistory.bInitialized && LastLastHistory.bInitialized);

		FVector2D LastPosDelta = LastHistory.CheckPointPosition - LastLastHistory.CheckPointPosition;
		float LastTimeDelta = FMath::Abs(LastHistory.CheckPointTime - LastLastHistory.CheckPointTime);

		if (LastTimeDelta >= MinOddTimeInterval &&
			// PosDelta �� �� ��Һ��� ����. ������ �ݴ�鼭 ���� ũ�� �̻����� Ƣ������.
			(
				(LastPosDelta.X * AddedCheckPointDelta.X < 0.0f && FMath::Abs(LastPosDelta.X) >= FMath::Abs(AddedCheckPointDelta.X * MinOddPosDeltaRatio)) ||
				(LastPosDelta.Y * AddedCheckPointDelta.Y < 0.0f && FMath::Abs(LastPosDelta.Y) >= FMath::Abs(AddedCheckPointDelta.Y * MinOddPosDeltaRatio))
			)
			)
		{
			return true;
		}
	}

	return false;
}
