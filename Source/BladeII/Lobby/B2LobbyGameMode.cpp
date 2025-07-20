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
//#include "B2UISummonItemDisplay.h"
#include "B2PreRenderer.h"
#include "B2GameLoadingProgress.h"
#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"
#include "B2PlatformAPI.h"
#include "B2GameDefine.h"
#include "../UI/Widget/B2UISummonItemDisplay.h"


bool AB2LobbyGameMode::IsFirstLobby = true;
bool AB2LobbyGameMode::IsFirstLobbyMain = true;

AB2LobbyGameMode::AB2LobbyGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bDefaultMatineePlayedReverse(false)
	, bIsInputLeftControl(false)
{
	//PrimaryActorTick.bCanEverTick = true;

	//DefaultPawnClass = ASpectatorPawn::StaticClass(); // SpectatorPawn 阑 扁夯栏肺
	//PlayerControllerClass = APlayerController::StaticClass(); // 鞘夸俊 蝶扼 蝶肺 extend 秦辑 荤侩..

	//DefaultPCClassEnum = EPCClass::EPC_End; // 老窜 spawn 窍瘤 臼绰促.

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

	//// PreRenderGameMode 档涝俊 蝶扼 霉 肺厚 肺爹 流饶 喊促弗 窍绰 老 绝捞 官肺 Pre-render 傈侩 甘阑 肺爹窍绰 惑炔捞 惯积窍霸 等促.
	//const bool bMapTransitionWillBeginSoon = IsFirstTimePreRenderPending(); // 酒付 捞霸 true 搁 弊成 咯扁辑 return 窍绰 霸 捞垫老瘤档 せ

	//if (IsFirstLobby && !bMapTransitionWillBeginSoon)
	//{
	//	// 免籍 UI 啊 贸澜 躲 锭 发捞 吧府绰 巴捞 哆绰 惑炔 锭巩俊 救 亮酒 焊咯辑 霉 肺爹矫埃 刘啊甫 皑荐窍绊 固府 凯菌促 摧绰芭. 咯扁俊绰 StayInViewport 眉农啊 登绢 乐绢辑 CloseUI 窍绰 霸 弊成 见扁绰 巴烙.
	//	UB2UIAttendanceMain* atta = UB2UIManager::GetInstance()->OpenUI<UB2UIAttendanceMain>(UIFName::AttendanceMain);
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::AttendanceMain);
	//	B2GMLoadingProgCollector::StepCustom(0.4f);
	//}

	//// 酒付 BeginPlay 矫痢 捞傈俊 鞘夸窍瘤绰 臼阑 巴.
	//LobbyUIManager = NewObject<UB2UIManager_Lobby>(this, *FString::Printf(TEXT("UIManager_Lobby_%s"), *this->GetName()), RF_Transient);
	//if (LobbyUIManager)
	//{
	//	// LobbyUIManager 绰 DJLegacy 狼 老何牢单 郴何俊辑 脚 UIManager 甫 荤侩窍绰 何盒捞 乐绢辑 父俊 窍唱 捞吧 力芭茄促搁 郴何甫 混旗杭 巴. (酒付 力芭且 老捞 绝阑 淀)
	//	if (LobbyUIManager->InitUIManager(this) == false)
	//	{
	//		LobbyUIManager->ConditionalBeginDestroy();
	//		LobbyUIManager = NULL;
	//	}
	//}
	//if (IsFirstLobby) // 贸澜俊 UB2UIManager_Lobby 俊辑 LoadOnStartup 贸府窍搁辑 矫埃 吧府绰 霸 粱 乐澜.
	//{
	//	B2GMLoadingProgCollector::StepCustom(0.4f);
	//}

	//// LobbyUIManager 积己 捞饶肺绰 啊鞭利 捞率阑 流立 access 窍绰 措脚 Event 矫胶袍阑 捞侩窍咯 皋矫瘤甫 焊辰促.
	//SetPCSelection(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());

	//CreateLobbyInventory(); // 鞘夸俊 蝶扼 矫痢阑 粱 炼例..
	//CreateLobbyAirport();

	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	//Matinee actor俊辑 cinematic mode啊 on俊辑 off瞪 锭, 公炼扒 Virtual joystick visibility甫 难霸 登绢乐澜.
	//	//府家胶俊辑 汲沥秦具 且 巴 鞍篮单, 角荐啊 乐阑 荐 乐栏聪 咯扁辑 碍力肺 cinematic mode肺 柳涝窍瘤 臼档废 茄促.
	//	//UWorld::GetMatineeActors() 捞 窃荐啊 乐绰单, CurrentLevel俊 措秦辑父 茫酒客辑 酒贰贸烦 内爹
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

	////[@Woongbok, 171222][TODO] 咯扁辑 救罐酒档 瞪淀 窍唱 肺流 八刘捞 鞘夸窃.
	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{ // 林夸 傈捧仿 荐摹 UI 诀单捞飘. 肺厚俊辑 粱 初摹绰 芭 乐歹扼档 犬角窍霸 诀单捞飘 登绰 矫痢 窍唱 持绢林绰 吧肺
	//	OnUpdateCombatPower(IntToPCClass(PCI), false);
	//}
	//	
	//if (IsFirstLobby && !bMapTransitionWillBeginSoon)
	//{ // 霉 肺厚 柳涝 付农 府悸
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
	
	SetUseCustomWaitForLoadingScreen(true); // 惑困窜 StartPlay 风凭俊辑 摧洒瘤 臼档废 阜酒初绊.. 咯扁辑 勤甸傅且 芭.

	if (IsFirstLobby)
	{ // 惑困窜 率俊辑 Scalability override 利侩窍扁 傈俊 甸绢啊具..
		InitializeAllGameSettingData(this);

		FString Title;
		if (FParse::Value(FCommandLine::Get(), TEXT("TitleName="), Title))
		{
			UKismetSystemLibrary::SetWindowTitle(FText::FromString(Title));
		}
	}

	Super::StartPlay();

	// 肺厚 霸烙葛靛狼 柳青档绰 扁夯 StartPlay 狼 Preload 俊辑绰 喊杭老 绝阑 波绊 咯扁辑 巢篮 DeferredChangeLobbyScene 沥档..
	if (LobbySceneManager.HasDeferredChangeLobbyScenes())
	{
		B2GMLoadingProgCollector::StepCustom(0.5f, 0.5f);
		// 角力 ChangeScene 阑 悼馆窍绰 版快.. 弊霸 倔付唱 瞪瘤俊 蝶扼 促甫 搏单.. 措何盒 窍唱聪鳖.. 奴 胶跑阑 林磊.
		// 弊烦 UIScene 窍唱 凯绊 芭狼 菜 盲快摆瘤.
		B2GMLoadingProgCollector::SetStepRate(0.9f);
	}
	else
	{
		B2GMLoadingProgCollector::StepToMax();
	}

	// 傈捧甘辑 倒酒坷芭唱 且 锭俊绰 DeferredChangeLobbyScenes 肺 抗距秦 初篮 open 府胶飘啊 乐阑 巴烙. 弊霸 酒匆 版快父 Title 拳搁阑 楷促. 
	// 绢瞒乔 Title 拳搁捞 凯府歹扼档 抗距秦 初篮 厘搁栏肺 官差摆瘤父 弊成 弊犯霸 滴搁 肺厚肺 倒酒棵 锭狼 肺爹矫埃 扯厚啊 凳.
	if (!LobbySceneManager.HasDeferredChangeLobbyScenes())
	{
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_Title);
	}
	else
	{
		//title拳搁俊辑绰 喊俺狼 bgm荤侩
		//title拳搁捞 酒匆矫父 肺厚 bgm荤侩
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

	// 傈捧甘俊辑 肺厚肺 倒酒柯 饶俊绰 捞巴狼 角青栏肺 角力 盔窍绰 肺厚 厘搁栏肺 啊霸 瞪 巴. 皋牢拳搁捞电, 傈捧甘捞电.
	LobbySceneManager.CallDeferredChangeLobbyScenes();

	if (LobbyInventory)
	{ // 傈捧 饶 肺厚 柳涝 矫痢俊辑狼 捞傈 胶抛捞瘤 嚼垫 酒捞袍狼 厘馒 鼻厘 眉农. 官肺 困俊辑 厘馒 鼻厘 矫 曼绊且 单捞磐啊 甸绢哎 荐 乐栏骨肺 捞 矫痢俊辑 眉农窃.
		LobbyInventory->ConditionalCheckForBetterEquipSuggest();
	}

	if (!IsInPreRenderPhase() && !IsFirstTimePreRenderPending()
		&& UB2UIManager::GetInstance()->GetCurrUIScene() != EUIScene::Title)
	{ // Pre-render 客 爵洒瘤 臼篮 措何盒狼 版快 (汲摹 饶 弥檬啊 酒聪扼搁) 惑困 StartPlay 狼 WaitForLoadingScreen 措脚 咯扁辑..
		WaitForLoadingScreen();
	}

	// Initialize InvitationGame
	ClearInvitationGameInfo(EB2GameMode::PVP_Tag);
	ClearInvitationGameInfo(EB2GameMode::Raid);
}

void AB2LobbyGameMode::OnLoadingScreenFinished()
{
	// 漂喊洒 贸府窍瘤 臼栏搁 肺爹 悼康惑捞 倒酒啊绰 客吝俊 躲 荐 乐绰.. 寇何 葛碘俊 狼茄 扑诀捞 乐绢辑 捞 鸥捞怪阑 扁废秦 狄.
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
//	Super::SubscribeEvents(); // Super 率俊 鞘夸绝绰 巴甸档 乐瘤父 救且 巴档 绝瘤.
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
	
	if (BladeIIGameImpl::GetClientDataStore().GetWaitInventorySync()) return; // 教农吝俊 犁瞒 吭阑 版快 府畔

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

	if (BladeIIGameImpl::GetClientDataStore().GetWaitInventorySync()) return; // 教农吝俊 犁瞒 吭阑 版快 府畔

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
		int32 WinCount = 0;				//穿利栏肺 拌魂

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
	// 橇肺葛记 昆轰. 捞扒 老馆利栏肺 荤侩窍绰 BeginPlay 幅狼 皋牢 静饭靛 肺爹 鸥捞怪捞 酒聪扼 厚悼扁 GMP 肺爹 康惑捞 场唱辨 扁促啡促啊 剁款促. 
	// 浚柳俊辑 官肺 剁快绰 霸 酒聪骨肺 皋牢 静饭靛 喉肺欧捞 救 冈塞.
	if (
		bLobbyLoadingScreenCompleted && 
		!IsViewPromotion() && // 弥檬 备泅 矫 父甸绢敌 炼扒.. 弊成 static 傈开 函荐 敲贰辟单.. 
#if PLATFORM_ANDROID && PLATFORM_IOS
		IsInitPurchases() &&	// 牢聚 搬力 檬扁拳 饶 橇肺葛记 轰 剁况 林档废
#endif
		(UB2UIManager::GetInstance()->GetCurrUIScene() != EUIScene::Title) && // 鸥捞撇 UI 甫 泪矫 胶闷促啊 官肺 促弗 甘阑 肺爹窍霸 登绰 版快啊 乐绢辑 菩胶.
		BladeIIGameImpl::GetClientDataStore().HasValidUserNickName() // 肺弊牢登瘤 臼疽促搁 酒付 Title UI 甫 逞绢啊瘤 给沁摆瘤父 眠啊 救傈 厘摹肺辑..
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
//	{ // Pre-render 客 包访秦辑 皋牢 甘 肺爹 拳搁 摧绰 吧 瘤楷矫虐霸 登骨肺 咯扁辑 贸府. 肺爹 槐臂捞 楷厘 瞒盔
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

	// 捞亥飘甫 咯矾 镑俊辑 某摹秦具 茄促搁 咯扁促 持绢辑 滚弊 何淬阑 临老 荐 乐瘤父 楷魂樊捞 腹篮 巴篮 啊鞭利 持瘤 臼档废 茄促.
	// 滚弊啊 积变促 秦档 UI 钎矫啊 泪矫 肋给登绰 沥档捞促.
	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
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
		// 咯扁辑 静绰 CombatStat 掘绢坷绰 窃荐甸篮 楷魂樊捞 腹栏骨肺 捞亥飘 矫痢俊父 龋免窍档废
		DocHero->SetAttackPower(CombatStatEval::GetPCAttack(UpdatedClass));
		DocHero->SetDefensePower(CombatStatEval::GetPCDefense(UpdatedClass));
		DocHero->SetHealthPoint(CombatStatEval::GetPCHealth(UpdatedClass));

		// 傈捧仿篮 眠啊利牢 扑诀 局聪皋捞记 贸府
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
{ // UI 免仿 矫痢锭巩俊 免仿何盒父 蝶肺 父甸菌澜.
	if (EPC == EPCClass::EPC_End) return;

	UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(int32(EPC));
	if (DocHero)
	{
		int32 BeforeCP = DocHero->GetPreCombatPower();
		int32 AfterCP = DocHero->GetCombatPower();

		if (BeforeCP != AfterCP && BeforeCP != 0)
		{
			// 傈捧仿 函版 扑诀 免仿
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
	// 曼咯沥焊甫 盎脚窍扁 困秦辑 刚历 夸没阑 茄促.
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
			FString::Printf(TEXT("皋牢某腐磐客 怕弊 某腐磐甫 悼老窍霸 汲沥且 荐 绝嚼聪促. 哪腔磐啊 梆 气惯茄促."))
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

	//UI档钮刚飘俊 MainChar客 SubChar甫 舅妨具窃
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		DocBattle->SetIsCurPCMain(true); // 肺厚俊辑狼 急琶篮 攫力唱 current 啊 main 牢 巴栏肺 埃林. 牢霸烙俊辑父 崔扼龙 荐 乐促.
		DocBattle->SetCurPCClass(PCClassToInt(InMainPCClass));
		DocBattle->SetTagPCClass(PCClassToInt(InSubPCClass));
	}
		
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{ // 傈捧 涝厘 拳搁老 版快 技泼捞 鞘夸茄 沥焊
		DocBS->SetBattleDifficulty(BladeIIGameImpl::GetStageDataStore().GetRelativeStageDifficulty(DocBS->GetServerStageId(), InMainPCClass, InSubPCClass)
			);
	}

	// 捞霸 措眉肺 UI 率俊辑 阂府绰 芭扼 粱 鉴券登绰 烩捞变 茄单 咯窍埃 促矫 UI 率栏肺 函版 烹瘤.
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
		// 180213_YJ 饭捞靛 概莫 墨款飘老锭 呕林 眠啊 贸府
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

	// 颇萍厘(龋胶飘)老 版快 泅犁 立加茄 蜡历俊霸 泅犁乐绰 荤恩甸狼 急琶等 某腐磐甫 逞变促.
	// 秦寸 菩哦阑 傈崔罐篮 蜡历绰 立加茄 蜡历甸阑 悸泼窍绊, 磊脚狼 急琶茄 某腐磐甫 傈眉 蜡历俊霸 舅赴促(宏肺靛某胶泼)
	if (GetPeerType() == PEER_SUPER)
	{
		packet::ByteStream	payload;

		// Host
		payload << GetMyNetId();

		// 某腐磐 沥焊
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
		// 冯 努肺令窍搁辑 配奴 犁汲沥篮 饭捞靛俊辑父.
		if (RoomType == b2network::B2RoomType::BRT_Raid)
		{
			// 磊扁磊脚 悸泼
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
		// 郴波档 眠啊秦拎具窃. 龋胶飘啊 酒聪菌栏搁
		// HandleWaitToMeetCondition() 夸扁辑 眠啊 给沁阑淀.
		IncreasePeerPlayers(GetMyNetId());

		// 概莫苞沥俊辑 Host啊 啊廉具窍绰 沥焊甫 犁汲沥秦霖促.
		for (auto PeerInfo : AllPeerInfos)
			IncreasePeerPlayers(PeerInfo.Key);
	}

	if (MatchState == NetMatchState::PreparePlayers)
	{
		// 180213_YJ 饭捞靛 概莫 墨款飘老锭 呕林 眠啊 贸府
		if (auto* UIRaidMain = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMain>(UIFName::Raid))
		{
			SendLeavePlayerOnlyRaid(BeforeHostId);
		}
	}

	// 厚龋胶飘 蜡历 冯努肺令捞饶 矫累窍扁鳖瘤 措扁吝牢瘤 敲贰弊. 捞荤捞 龋胶飘 牢拌罐疽栏搁 规秉促.
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

	// 霖厚犬牢 鸥捞赣 力芭
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

	// 概莫 饶 饭骇胶飘府怪 栏肺牢秦 单捞磐啊 朝扼啊霸登骨肺 DocLobby俊 概莫包访 沥焊甫 历厘
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
			{ // 甘 肺爹 档吝 SkillAnim 狼 Pre-load 俊 静扁 困茄 沥焊. 秦寸 Pre-load 矫痢俊 货肺 肺爹登绰 霸烙葛靛俊辑 固贸 RemotePlayerInfoData 啊 绝绰 惑炔捞扼 咯扁俊 历厘秦 敌促.
				TArray<ESkillAnimType> SkillAnimTypesOfThisRemote;
				UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(this, Elem.Value.GetMainPlayerClass(),
					///// 促澜 霸烙葛靛绰 酒付档 Raid 唱 Control 捞 瞪刨单 购瘤 舅 荐啊 绝焙. 救俊 甸绢啊焊搁 荤角 老何 胶懦阑 漂喊窍霸 荤侩窍绰 GameMode 鸥涝俊 秦寸窍瘤父 臼栏搁 巩力绰 绝绰单 粱 芭浇府变 窍瘤.
					EB2GameMode::Raid,
					/////
					SkillAnimTypesOfThisRemote, &Elem.Value);
				// 葛篮 吧肺 CombinedID 甫 父甸绢 持绰促.
				for (ESkillAnimType ThisSkillAnimType : SkillAnimTypesOfThisRemote)
				{
					UIDocLobbyMatch->MatchedRemotePlayerSkillAnims.AddUnique(FCombinedPCSkillAnimID(Elem.Value.GetMainPlayerClass(), ThisSkillAnimType));
				}
			}
			//////////////////////////////
		}
	}	

	// 林狼 :	捞饶 GameMode啊 函版登搁 NotifyRoomLeave啊 龋免登绰单 捞锭 RoomId啊 粮犁窍搁 辑滚俊 冯阑 唱啊绰 菩哦阑 焊辰促. 
	//			捞甫 规瘤窍扁困秦 馆靛矫 冯阑 檬扁拳秦拎具窃
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
	// 龋胶飘侩 ReservedPlayerClasses 汲沥
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
		{// 评, 困摹 技泼
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

	// 磊脚阑 力寇窍绊 颇萍盔俊霸 矫累阑 舅覆
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

	// 譬配府倔
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
	// 盲澄唱啊扁
	// LeaveRoomIfJoined 肺厚纠 概聪历俊辑 概莫芒 摧酒霖促.
	LeaveRoomIfJoined();

	// 扑诀剁快扁侩
	LobbySceneManager.ReceiveNoResponsePrerequisiteToStart();
}

void AB2LobbyGameMode::BreakRoom(int32 InMapId /*= 1*/, int32 InDifficulty /*= 1*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::BreakRoom"));

	if (GetMatchState() != NetMatchState::StartResult)
	{
		// 概莫苞沥俊辑 规阑 饼巴牢瘤 眉农
		if (HasMatchStarted())
		{
			if (GetPeerType() == PEER_SUPER)
			{
				// 辑滚肺 歹 捞惑 甸绢坷瘤 给窍霸窍绰 菩哦阑 傈价
				UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid();
				data_trader::Retailer::GetInstance().RequestRoomClose(GetRoomId(), InMapId, GetBreakRoomCloseReason()); // 鸥涝 函版秦林磊.

				// 葛电 蜡历甫 碍硼矫挪促.
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
		
	// 冯捞 菜茫绰瘤 眉农
	CheckFullPeerRoom();
}

void AB2LobbyGameMode::NetSendBattleGradeInfo(int32 BattleGrade, int32 BattleStarCount)
{
	// 郴波 痢飞傈 殿鞭 盲澄郴 规价
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

	// 某腐磐 沥焊
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
			// 饭捞靛 其捞瘤 郴俊辑 檬措甫 罐疽澜.
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

	// 酒流 辑滚肺 何磐 概莫捞 登瘤 臼疽促搁 PCClass甫 抗距 吧绢敌促. (Reserved PCClass)
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
			// 磊扁磊脚篮 力寇
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
	LobbySceneManager.SetPlayingBeginAnim(true); // 促矫 敲饭捞 啊瓷窍霸 窍绰 芭. 肺厚 梅 柳涝且 锭 滴锅 鞘夸秦辑.
}

void AB2LobbyGameMode::SendLeavePlayerOnlyRaid(uint32 NetId)
{
	// 180213_YJ 饭捞靛 概莫 墨款飘老锭 呕林 眠啊 贸府
	if (GetPeerType() == PEER_SUPER)
	{
		/*
		#if !UE_BUILD_SHIPPING
		if (NetFakeConfigure::GetInstance().GetHateRaidPenalty()) return; // 目盖靛 摹搁 呕林 力力 公矫.
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
								SubPCPresetId); // 眠啊 夸没 乐扁 傈俊绰 馆汗傈捧急 矫唱府坷 楷免 酒囱 剪滚傈 楷免肺. RequestStartRepeatBattleWithSelectedPC 俊辑档..

	return true;
}

bool AB2LobbyGameMode::RequestStartRepeatBattleWithSelectedPC(int32 ClientStageId, EStageDifficulty StageDifficulty)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::RequestStartRepeatBattleWithSelectedPC"));

	// 阿阿狼 馆汗傈捧 胶抛捞瘤 柳涝捞 酒囱 盖 贸澜 柳涝痢烙.
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	// 蔼阑 技泼茄 促澜 辑滚俊辑 览翠捞 坷搁 牢霸烙 备炼眉肺 傈崔且 巴烙.
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

	TArray<EStageBuffType> SelectedBuffs; // 馆汗傈捧俊辑档 滚橇 酒捞袍 急琶捞 乐促搁 罐酒坷档废..
	if (DocBS)
	{
		SelectedBuffs = DocBS->GetSelectedStageBuffs();
	}

	return RequestStartGameStageWithSelectedPC(ClientStageId, StageDifficulty, SelectedBuffs, true); // 眠啊 夸没 乐扁 傈俊绰 馆汗傈捧急 矫唱府坷 楷免 酒囱 剪滚傈 楷免肺
}

void AB2LobbyGameMode::CancelOrStopRepeatBattle()
{ // 馆汗 傈捧肺 涝厘窍妨绰单 牢亥配府啊 瞒芭唱 购啊 何练窍芭唱 殿狼 捞蜡肺 给窍霸 登菌阑 锭..
	// 郴瘤绰 胶抛捞瘤 涝厘 夸没窍扁 傈俊绰 窜鉴洒 捞吧肺 馆汗傈捧 吝窜捞 凳.
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

	// 阿阿狼 馆汗傈捧 胶抛捞瘤 霖厚 悸诀. 捞傈俊档 馆汗傈捧甫 摹风绊 吭绊 眠啊 馆汗傈捧档 巢酒乐绰 惑怕
	// 鞘夸茄 措何盒狼 巴甸篮 UI 俊辑 贸府窍绊.. 咯扁急 馆汗傈捧甫 歹 捞惑 柳青窍瘤 给窍绰 版快俊 措茄 荤傈 眉农甫 窍档废 窍磊..		
	const bool bIsRepeatBattlePossible = StageDataStore.IsPossibleToDoRepeatBattle(InServerStageId);
	if (!bIsRepeatBattlePossible && !(StageDataStore.IsRepeatBattleLocked(InServerStageId) && RepeatBattleRequestState.bRepeatOne))
	{
		// 老窜 吝窜何磐 窍绊..
		CancelOrStopRepeatBattle();
		// 皋矫瘤扼档 谎妨拎具摆瘤..
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
	// 搬惫篮 LobbyUISwitcher 狼 BeginLobbyUIPage 妮牢单, 傈饶肺 捞率辑 惑怕 函拳 殿 鞘夸茄 巴捞 乐栏搁 贸府.
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
	// DJLegacy 客 脚 UI 楷悼 烙矫 贸府.. 老吧肺 积阿沁栏唱 康旷包府 葛靛绰 拌加 DJLegacy 肺 巢阑 淀.
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	const EUIScene CurrentUIScene = UIMgrInst->GetCurrUIScene();
	if (CurrentUIScene != EUIScene::None && /*!IsDJLegacyUIScene(CurrentUIScene) && */ // UIInvenQuickSlide 客 鞍篮 版快 DJLegacyUIScene 捞 楷加瞪 荐 乐澜. 绢痘霸 焊搁 弊 版快绰 洒胶配府 公矫窍绰 霸 唱阑 荐档 乐变 茄单.. 绢恫 版快绰 坷悼累阑 窍霸 登聪..
		InSubMode != EHeroMgmtSubMode::EHMSM_End && !UIMgrInst->IsOpeningDJLegacyHeroMgmtBySceneHistory())
	{
		// Back button 努腐 矫 (UB2UIManager::ChangeUISceneBack) 喊档 UIManager 矫胶袍狼 捞傈 UIScene 栏肺 倒酒哎 荐 乐档废 蝶肺 洒胶配府甫 持绢霖促.
		// 弊矾唱 Back button 努腐栏肺 咯扁俊 柯 版快绰 力寇.
		UIMgrInst->ChangeUIScene(DJLegacyHeroMgmtPageToUIScene(InSubMode));
	}
	//////////////////////////////////////////////////

	//HeroMgmtModePCSelection = BladeIIGameImpl::GetClientDataStore().GetMainPlayerClass(); // 扁夯利栏肺 肺厚俊辑 急琶等 PC 甫 逞败淋.
		
	OnQuitHeroMgmtSubModeCommon(HeroMgmtSubMode); // 捞傈 sub 葛靛 摧绰 贸府档. InSubMode 客 鞍歹扼档 酒流 货肺 坷锹 傈捞聪 摧绰 贸府甫 秦 航. 巩力啊 等促搁 粱 歹 八荤甫 碍拳.

	HeroMgmtSubMode = InSubMode;
	ELobbyUIPages MatchingDjLegacyUIPage = HeroMgmtSubModeToLobbyUIPage(HeroMgmtSubMode);
	if (MatchingDjLegacyUIPage != ELobbyUIPages::ELUP_End)
	{
		DJLegacy_ChangeLobbyUIPage(MatchingDjLegacyUIPage);
		LobbyInventoryViewJustOpenClass<>::GetInstance().Signal();

		// 趣咯唱 阿 submode 喊肺 漂荐茄 悼累捞 鞘夸窍搁 何掺矾况窍瘤 富绊 咯扁俊 if 唱 switch 持绢辑 贸府....
		// 捞傈俊 角力肺 弊犯霸 登绢 乐菌促. 偿偿
	} 

	// 康旷 包府 葛靛 傈侩 墨皋扼况农
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
	// 捞霸 DJLegacy 啊 登促焊聪 角力 官操绰 扒 促弗 版肺甫 烹窍绊 咯扁绰 何啊利牢 贸府甫 窍霸 登菌澜.

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

	UB2Airport::FlushPendingWingVisibilityReq(); // 朝俺 visibility on/off 茄 芭 葛酒辑 辑滚肺 夸没
	BladeIIGameImpl::GetClientDataStore().RequestSyncToServerShowHead(); //赣府 Visible 包访 辑滚 悼扁拳

	// 厘厚 厘馒捞唱 胶懦 碍拳 殿俊 蝶扼 酒赴狼 炼攫档 眉农
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
	// 啊鞭利 CreateLobbyInventory 矫痢 捞傈俊 GetLobbyInventory 甫 窍绰 老捞 绝档废..
	checkSlow(LobbyInventory);
	return LobbyInventory;
}

bool AB2LobbyGameMode::CreateLobbyInventory()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CreateLobbyInventory"));

	// LobbyInventory 狼 腹篮 牢磐其捞胶甸捞 static 捞瘤父 咯傈洒 按眉 积己阑 粱 茄促.
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
	// 啊鞭利 CreateLobbyAirport 矫痢 捞傈俊 GetLobbyAirport 甫 窍绰 老捞 绝档废..
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

	// 父老 UI Doc 俊 酒捞袍捞 甸绢埃促搁 咯扁辑 持瘤绰 富 巴. 单捞磐啊 腹阑 巴捞骨肺 LobbyInventory 鞍篮 单辑 漂荐窍霸 贸府.
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

		DocChapter->SetChapterOpenedNum(ChapterMaxNum); // 磊, 泅犁肺急 弊成 弥措 茅磐鳖瘤 胶农费篮 且 荐 乐档废 窃. 拱沸 阿 胶抛捞瘤绰 泪败 乐阑 巴.	// 扁粮 : DocChapter->SetChapterOpenedNum(ChapterOpenedNum);
		DocChapter->SetChapterMaxNum(ChapterMaxNum);
		DocChapter->SetMaxOpenedStageDifficulty(ChapterMaxNum >= SelectedChapterViewNum ? StageDataStore.GetMaxOpenedStageDifficulty() : StageDataStore.GetMaxOpenedNewStageDifficulty(SelectedChapterViewNum));
		DocChapter->SetCurChapterNum(SelectedChapterViewNum);
		DocChapter->SetCurStageDifficulty(SelectedStageDifficulty);
		DocChapter->SetCurChapterName(FText::Format(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, StageDiffText.ToString()),
			ChapterInfoTable->GetLocalizedChapterName(SelectedChapterViewNum)));

		// UI惑栏肺 舅妨林扁 困秦 Next档 钎矫
		DocChapter->SetNextChapterNum(StageDataStore.GetActIdByClientStageId(StageDataStore.GetNextClearClientId(SelectedStageDifficulty)));
		DocChapter->SetNextSubChapterNum(StageInfoTable->GetSubChapterNumByClientStageId(StageDataStore.GetNextClearClientId(SelectedStageDifficulty)));
	}
}

void AB2LobbyGameMode::SetCombatEntryChapterNum(int32 InChapterNum, EStageDifficulty InDifficulty)
{
	SelectedChapterViewNum = InChapterNum;
	SelectedStageDifficulty = InDifficulty;

	UpdateChapterData();

	// 茅磐 函版 矫 鞍篮 胶飘府怪 饭骇俊 弓绰 窜困俊 蝶扼 饭骇 胶飘府怪阑 货肺 秦 林绢具 且 荐 乐促. 
	ChangeLobbyScene(ELobbyScene::ELobbyScene_Chapter, SelectedChapterViewNum);
	LobbySceneManager.ForceUpdate();
}

void AB2LobbyGameMode::GoGameStageInfo(FServerStageID InServerStageId, FRepeatBattleStateSet& RepeatBattleSet)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::GoGameStageInfo"));

	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	// 馆汗傈捧 胶抛捞瘤 肯丰窍绊 咯傈洒 巢疽阑 锭 咯扁肺 倒酒柯促. (AB2StageGameMode::OnEndPlayOfSingleRepeatBattleStage)
	// 馆汗傈捧甫 咯傈洒 柳青秦具 茄促搁 巢篮 馆汗 墨款飘啊 窍唱绰 乐绢具 窃.
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
	if (RepeatBattleRequestState.bIsOn && RepeatBattleRequestState.CurrentRepeatCount < 0){ // 趣矫 隔扼辑
		RepeatBattleRequestState.CurrentRepeatCount = 0;
	}
	SyncRepeatBattleRequestStateToUIDoc();

	//>>==============急琶等 胶抛捞瘤肺 焊辰促. 老窜 抛胶飘聪 咯扁 滴菌绰单 角力肺绰 辑滚肺 Request窍绊 览翠吭阑锭 贸府 秦具 瞪淀
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		DocBS->SetClientStageId(StageDataStore.GetClientStageId(InServerStageId));
		DocBS->SetStageDifficulty(StageDataStore.GetStageDifficulty(InServerStageId));
		DocBS->SetServerStageId(InServerStageId);
		
		//todo StageAreaName阑 抛捞喉肺 包府窍档废 荐沥窍咯 咯扁辑 技泼窍档废 - 弊 傈鳖瘤绰 后沫栏肺
		DocBS->SetLocalizedAreaName(FText::FromString(FString(TEXT(""))));

		//todo 牢亥配府甫 第廉辑 唱坷搁 持档废.
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

	GameStageCheckForArinConsulting(InServerStageId); // BattleStageInfo 其捞瘤 凯扁 傈俊 酒赴 炼攫 眉农档 肯丰.

	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::BattleStageInfo);
	//<<==============急琶等 胶抛捞瘤肺 焊辰促.

	if (RepeatBattleRequestState.bIsOn) // RepeatBattle 档吝 胶抛捞瘤 肯丰 矫俊档 捞 风飘甫 烹秦 焊晨.
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
	//弥辟 敲饭捞茄 胶抛捞瘤 沥焊
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
		if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_Normal)) // 畴富 葛靛甫 场鳖瘤 柳青窃
		{
			if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_Hard)) // 窍靛 葛靛甫 场鳖瘤 柳青窃
			{
				if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_VeryHard)) // 概快 绢妨框阑 场鳖瘤 柳青窃
				{
					if (StageStore.IsClearedFinalClientStageId(EStageDifficulty::ESD_Hell)) // 秋阑 场鳖瘤 柳青窃
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

	// 第肺啊扁肺 倒酒吭阑锭 秦寸 茅磐肺 焊咯林扁 困秦 固府 DOC技泼
	
	
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
	// 绢录搁 皋牢/辑宏 吝 漂洒 穿啊 冻绢瘤绰瘤 蝶肺 历厘秦具 且 荐档. 酒赴狼 炼攫俊辑 牢亥配府肺 逞绢哎 锭 努贰胶 急琶俊 馆康秦具 且 荐档 乐栏聪..
	if (UnitedAttackPower < StageSuggestedAttack)
	{
		FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_Attack;
	}
	else if (UnitedDefensePower < StageSuggestedDefense)
	{
		FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_Defense;
	}
	else if (UnitedSkillPoint > 5) // 荐摹甫 割栏肺(?)
	{
		FinalArinConsultSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_Skill;
	}
	//else if (MainCharSkillPoint > 3 || SubCharSkillPoint > 3) // 笛捞 钦媚 3暗瘤 笛 吝 窍唱父 3暗瘤 犬牢..
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

	// 规阑 父甸菌阑锭唱, 狐弗概莫锭绰 单捞磐甫 朝妨林瘤父 檬措罐疽阑版快浚 朝妨林绰 何盒捞 绝绢 眠啊.
	AllPeerInfos.Empty();
	if (auto* UIDocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		UIDocLobbyMatch->ClearRemotePlayerInfoData();

	if (UB2UIDocRaid* UIDocRaid = UB2UIDocHelper::GetDocRaid())
	{
		UIDocRaid->ClearRaidJoinData();
		UIDocRaid->ClearRaidBattleData();
		
		// 急琶等 沥焊 涝仿
		UIDocRaid->SetLastRaidType(RaidType);
		UIDocRaid->SetLastRaidStep(Step);
		UIDocRaid->SetIsReceiveInvite(true);
		UIDocRaid->SetOpenedRaidJoinType(ERaidJoinType::MakeRoom);
	}

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		DocBattle->SetCurPCClass(PCClassToInt(ChangeMainPCClass));

	// 单捞磐甫 刚历 技泼窍绊 Request甫 焊辰饶俊 Response 罐篮 捞饶 UI函版.
	// (林狼 : 檬措 罐疽阑 版快 RequestRoomMatchMaking阑 窍瘤 臼绊 RequestJoinRoom栏肺 官肺 冯俊 涝厘!)
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

	LobbySceneManager.EnterItemOpDirectingView(InOpMode); // LobbySceneManager 甫 烹秦 霖厚秦 初篮 楷免侩 墨皋扼 轰肺.
}

void AB2LobbyGameMode::QuitItemOpDirectingView()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::QuitItemOpDirectingView"));

	LobbySceneManager.QuitItemOpDirectingView(); // 扁粮 牢亥配府 墨皋扼 轰肺 汗蓖
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
	{ // 磊肋茄 扑诀捞 夺 惑怕俊辑 惑痢 柳涝窍绰 版快啊 积辨 荐 乐绢辑 弊芭 贸府.
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
// 措眉肺 Lobby UI 俊辑 阂府霸 登绰 辑滚客狼 楷拌 扁瓷甸.

void AB2LobbyGameMode::LoginAndStartGame(const FString& InUserID, const FString& InUserPW)
{
	//DJLegacy_ChangeLobbyUIPage(ELobbyUIPages::ELUP_CharSel); // 官肺 促澜 其捞瘤肺 捞悼.
}

void AB2LobbyGameMode::CreateAccount(const FString& InUserID, const FString& InUserPW)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2LobbyGameMode::CreateAccount"));

	//if (InUserID.IsEmpty())
	//{
	//	// TODO : 拌沥疙阑 涝仿秦林技夸~ 扑诀 皋技瘤
	//	UE_LOG(LogBladeII, Log, TEXT("Please enter your nickname!!"));
	//}
	//else
	//{
	//	auto B2MessageInfo = std::make_shared<b2network::FB2MessageInfoRequestSignUp>
	//	(
	//		[this, InUserID, InUserPW](FB2MessageInfoPtr messageInfo)
	//		{
	//			// 拌沥 积己捞 己傍沁栏骨肺 促矫 立加 矫档甫 窍磊
	//			LoginAndStartGame(InUserID, InUserPW);
	//		},
	//		[this](FB2MessageInfoPtr messageInfo)
	//		{
	//			b2network::EB2ResultCode errorCode = messageInfo->GetErrorCode();
	//			// TODO 俊矾 内靛 贸府
	//		}
	//	);
	//	B2MessageInfo->name = TCHAR_TO_ANSI(*InUserID);
	//	FB2NetworkBridge::GetInstance()->SendB2Message(B2MessageInfo);
	//}
}

void AB2LobbyGameMode::ProceedOnCharacterSelection()
{ // 某腐磐 牢飘肺 芒俊辑狼 急琶 捞饶. 够 角力肺 急琶茄 扒 酒囱单 傲 贸澜俊 某腐磐 牢飘肺啊 购啊 急琶窍绰 烩捞扼 捞抚捞 捞犯霸 登菌澜.
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

// CallOwner = 檬措(荐脚)磊
void AB2LobbyGameMode::PrevAcceptGameInvite(const FString& Inviter, EB2GameMode GameModeType)
{
	// InviteGame UI俊辑 荐遏篮 沁瘤父 某腐磐 急琶 捞饶 角力 荐遏 Request甫 焊晨
	auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();
	if (GameModeType == EB2GameMode::PVP_Tag && DocPVP)
	{
		// CharacterSelectPage 捞悼 傈 Setting登绢 乐绢具 窃
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

			// RequestRaidStart 捞抚篮 饭捞靛 矫累捞瘤父, 辑滚评俊辑 橇肺配妮 犁荤侩栏肺
			// 饭捞靛 檬措矫俊 绢恫 某腐磐甫 急琶沁绰瘤 舅府绊, 府胶迄胶甫 罐篮 饶俊 冯炼牢.
			if (RaidInvitePopupUI != nullptr)
			{
				RaidInvitePopupUI->SetAutoCloseOnClick(false); // 府胶迄胶甫 贸府茄 饶俊 摧酒具窃
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
		// 捞固 规俊 甸绢啊 乐促绰 狼固
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

		// LobbyMain捞 Hide 惑怕扼搁
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
// 甘 肺爹 矫埃 螟沥窍带 芭 肺厚 纠 傈券 矫埃 螟沥俊档 荤侩
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

	LobbyEndOnTouchImpulseRingClass<>::GetInstance().Signal(); // 犬牢 荤混 瞒盔俊辑

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	DevCheckLoadingTimeEnd();
#endif
}

/** 
 * 己瓷俊 零距捞 瞪 荐 乐绢辑 碍力 诀单捞飘 牢磊绰 力傍窍瘤 臼绰促.
 * 扁夯利栏肺 阿 某腐磐 努贰胶 喊肺 merge 茄 皋浆绰 厘馒 惑怕, 朝俺 咯何俊 蝶扼 refresh 甫 秦具窍绰瘤 魄喊阑 窃.
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
	// 扁鸥 辑滚俊辑 SetAccount 吭阑 锭 诀单捞飘 鞘夸茄 棱促茄 芭 乐栏搁 眠啊
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
{ // 老馆利栏肺绰 胶飘府怪 饭骇 肺靛 矫 BeginPlay 甫 且 锭俊 Matinee 啊 技泼凳. 漂荐茄 版快俊 流立 诀单捞飘且 锭 荤侩.
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
	
#if 0 //-> 秦搬窍瘤 给茄 巩力啊 腹酒辑 搬惫 波 滚覆. 捞 巩力甫 秦搬窍绰 磊 固揪甫 掘阑瘤绢促.
	// 割割 厘搁俊辑 OnTouchImpulseRing 阑 焊捞瘤 臼绰 巴篮 秦寸 葛靛俊 腹捞 汲摹秦 初篮 widget 甸狼 涝仿 勤甸傅苞 爵囚 巩力啊 惯积窍扁 锭巩.
	// 抗甫 甸绢 胶农费冠胶 殿.. TouchEnd 贸府俊 巩力啊 粱 乐澜..
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
	FVector2D Delta( (CurrentPos - PrevPos) * divideScreenSize); //老窜 厚啦肺 刚历 Delta甫 沥窃
	
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

		// 犬角窍瘤绰 臼篮单 FlickH 啊 瘤加利栏肺 阂府绰 淀茄 版快啊 乐促. InputHistory_CheckFlick 阑 檬扁拳窍绰 镑捞 腹瘤 臼酒 咯扁辑 眠啊 贸府甫 秦 航.
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
			InputHistory_IgnoreCheck.AddCheckPoint(ElapsedTimeTouchAction, FVector2D(Location.X, Location.Y)); // TouchMove 悼救 涝仿阑 历厘窍搁辑 盔摹 臼绰 荐霖狼 涝仿捞 甸绢吭促 魄窜登搁 Swipe 阑 角青窍瘤 臼档废..

			if (InputHistory_IgnoreCheck.IsLastHistoryOdd(OddInputMinTimeInterval, OddInputMinPosDeltaRatio)) {
				// 沥惑利栏肺 扁措登绰 惑炔俊辑 TouchEnd 啊 肋 甸绢坷搁 捞繁 贸府啊 鞘夸绝绰单 葛官老 涝仿 惑炔俊辑 弊犯瘤 臼阑 锭啊 辆辆 乐促. 
				// 泵咯瘤氓捞瘤父 TouchEnd 绝捞 鞭累胶繁 涝仿捞 甸绢吭促 酵阑 锭俊 Swipe 阑 窍瘤 臼档废. InputHistory 甸篮 葛滴 檬扁拳.
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
{ // 霸烙 汲摹 饶 霉 角青. 郴瘤绰 肺拿 惑怕 府悸 饶 茄 锅 乐阑 老..
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

	//评沥焊 技泼 救窃

	// 规价. 龋胶飘铂 瓜绢扼.
	UE_LOG(LogBladeII, Log, TEXT("[SetPeerTeam] RED = %d, Bule = %d"), GetMatchPlayerNum(ETeamType::ETT_Red), GetMatchPlayerNum(ETeamType::ETT_Blue));
	NetSendMatchPlayerTeamInfo();
}

uint8 AB2LobbyGameMode::GetPlayerStartIndexByNetId(uint32 NetId, ETeamType PlayerTeamType)
{
	// 龋胶飘父 捞芭 结具茄促.
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
			//[JH][todo] 促弗镑(AllPeerInfos)俊辑 评技泼阑 窍绰 何盒捞 乐绢辑 沥府啊 鞘夸窃
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

	// 啊楼 敲饭捞绢 父甸扁 唱器窃 101 ~ 108肺 汲沥瞪淀. 箭磊绰 奴狼固 绝澜
	uint32 nFakePlayerNetId = 102;
	uint32 nCurrentFakePlayerNetId = nFakePlayerNetId;

	SetPeerType(PEER_SUPER);

	// 妊捞农 齿酒捞叼 官肺菊箭磊甫 郴铂肺.
	SetMyNetId(nFakePlayerNetId - 1);

	EmplacePeerPlayer(GetMyNetId());

	// 饭靛肺 茄锅 碍力汲沥
	if (AllPeerInfos.Contains(GetMyNetId()))
	{
		AllPeerInfos[GetMyNetId()].PlayerTeamType = ETeamType::ETT_Red;
	}

	const TArray<FTutorialControlBotInfo>* TutorialControlBotInfos = TutorialManager::GetInstance().GetTutorialControlBotInfos();

	for (FTutorialControlBotInfo TutorialControlBotInfo : *TutorialControlBotInfos)
	{
		//7俺 逞栏搁 救等促.
		if (nCurrentFakePlayerNetId >= nFakePlayerNetId + 7)
		{
			check(0)
			return;
		}

		// 府葛飘 单捞磐 窍唱 眠啊
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

		//酒捞袍 眠啊
		for (FTutorialControlBotItemInfo TutorialControlBotItemInfo : TutorialControlBotInfo.BotEquipItem)
		{
			b2network::B2ItemServerInfoPtr pItemInfo = std::make_shared<b2network::B2ItemServerInfo>();
			pItemInfo->character_type = pAccountInfo->characters[0]->character_type;
			pItemInfo->template_id = TutorialControlBotItemInfo.ItemTemplateId;

			// 啊楼 酒捞袍 酒捞叼(辑滚)绰 袍敲复酒捞叼 弊措肺靖 弊成(吝汗父 救登搁 惑包绝摆瘤)
			pItemInfo->id = pItemInfo->template_id;

			pItemInfo->equip_position = TutorialControlBotItemInfo.ItemEquipPos;
			pItemInfo->amount = 1;
			pItemInfo->grade = 1;
			pItemInfo->quality = 1;
			pItemInfo->level = 1;
			// 捞吧肺 傍规眉 汲沥(盔贰 辑滚俊辑 酒捞袍沥焊 罐阑锭 罐澜)
			pItemInfo->md_main_attribute_value = TutorialControlBotItemInfo.ItemBaseStat;

			pAccountInfo->equip_items.Add(pItemInfo);

			b2network::B2ItemPresetPtr pItemPreset = std::make_shared<b2network::B2ItemPreset>();
			pItemPreset->character_type = pAccountInfo->characters[0]->character_type;
			pItemPreset->preset_id = 0;
			pItemPreset->equip_position = pItemInfo->equip_position;
			pItemPreset->item_id = pItemInfo->id;
			pAccountInfo->item_presets.Add(pItemPreset);
		}

		// 府葛飘 敲饭捞绢牢器俊 眠啊.
		RemotePlayerInfoData[nCurrentFakePlayerNetId].SetModPlayerInfo(pAccountInfo);
		RemotePlayerInfoData[nCurrentFakePlayerNetId].SetMainPlayerClass(TutorialControlBotInfo.BotPCClass);

		AllPeerInfos.Emplace(nCurrentFakePlayerNetId, FLobbyMatchPeerInfo());
		AllPeerInfos[nCurrentFakePlayerNetId].ReservedPCClass = TutorialControlBotInfo.BotPCClass;
		AllPeerInfos[nCurrentFakePlayerNetId].PlayerTeamType = TutorialControlBotInfo.BotTeamType;
		
		// 其捞农 齿酒捞叼 棵妨淋
		nCurrentFakePlayerNetId++;
	}
	// 啊楼 敲饭捞绢 父甸扁 辆丰

	// 冯努肺令 胶诺窍备 弊第肺 楷搬
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
{ // 磐摹 and swipe 饶 皑加阑 悼馆茄 磊蜡 雀傈 矫累.
	bDeacceleratingCharRotate = true;
	DeaccelerationCharRotate = (InLastSwipeValue < 0.f ? 1.f : -1.f) * LobbySceneManager.GetCharacterCameraRotateDeaccelerationFactor(); // 炼累沁带 规氢苞 馆措肺 deaccelerate
	RotationSpeedCharRotate = InLastSwipeValue * LobbySceneManager.GetCharacterCameraRotateFactor(); // 皑加 矫累 矫狼 檬扁蔼. FLobbySceneManager::SwipeH 贸府俊辑 悼老茄 加档肺 捞绢瘤档废.
	RotationSpeedCharRotate = FMath::Clamp(RotationSpeedCharRotate, -1.0f * LobbySceneManager.GetCharacterCameraFreeRotateMaxSpeed(), LobbySceneManager.GetCharacterCameraFreeRotateMaxSpeed());
	// 墨皋扼 倒府绰 芭尔 某腐磐 倒府绰 芭尔 胶纳老捞 粱 崔扼辑 老何 纠俊辑绰 眠啊 胶纳老阑 林绰单
	// 捞犯霸 纠 捞抚阑 流立 疙矫窍绰 侥捞搁 备龙备龙秦辑 固胶唱扁 奖瘤父 固胶啊 抄促绊 秦辑 奴老 朝 巴档 酒聪绊 窍聪 捞犯霸 啊绰 芭.
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
	//FPointerEvent狼 ScreenSpacePosition篮 PC 葛聪磐 傍埃狼 谅钎捞扁 锭巩俊 霸烙轰器肺狼 傈券捞 鞘夸窃.
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

			if (RotationSpeedCharRotate * DeaccelerationCharRotate >= 0) // 皑加 搬苞肺 规氢捞 馆措啊 登搁 吝窜.
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

	//	// 茄临 盲泼档 抛胶飘 且 荐 乐霸..
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

	//	// 茄临 盲泼档 抛胶飘 且 荐 乐霸..
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
	{ // 甘 郴副 矫俊 DJLegacy UI 俊 肺爹等 TAsset 甸 Unload 鞘夸..
		LobbyUIManager->OnPreLoadMap();
	}

	// 霉 傈捧甘 肺爹 矫 矫唱府坷 悼康惑 措脚 钧蹲窍霸 鸥捞撇 悼康惑捞 唱柯促绰 力焊啊 乐绢辑.. 捞蜡甫 舅 荐啊 绝瘤父 购啊 窍唱狼 啊瓷己 力芭
	if (!IsFirstLobby)
	{
		AB2DLCFrontGameMode::ClearSelectedFrontTitleMovieInfo();
	}
}

void AB2LobbyGameMode::OnPlatformPause()
{
	// 1. 檬措 芭例 - 促弗 咀记甸捞 鞘夸窍搁 窃荐肺 哗具窃
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
	if (OptionalLoggingHistoryName.Len() > 0) // 扳荤扳荤 肺弊 谎副瘤 瘤沥窍绰 概俺函荐肺档.
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
		return false; // History 啊 绝澜. 魄喊 阂啊
	}

	int32 AddedOldHistoryCount = 0;
	FVector2D AddedCheckPointDelta = FVector2D(0.0f,0.0f);
	// 付瘤阜波 力寇窍绊 辑肺埃狼 delta 甫 歹窃
	for (int32 IA = GetPrevHistoryIndexOf(InputHistoryLastIndex); IA != GetNextHistoryIndexOf(InputHistoryLastIndex); IA = GetPrevHistoryIndexOf(IA))
	{
		const int32 IB = GetPrevHistoryIndexOf(IA);

		const FInputHistoryElem& InputHistoryA = InputPointHistory[IA];
		const FInputHistoryElem& InputHistoryB = InputPointHistory[IB]; // A 俊辑 茄锅 歹 捞傈波

		if (!InputHistoryA.bInitialized || !InputHistoryB.bInitialized) {
			continue; // 笛 促 蜡狼固茄 蔼捞 乐绢具摆促.
		}
		++AddedOldHistoryCount;

		AddedCheckPointDelta = InputHistoryA.CheckPointPosition - InputHistoryB.CheckPointPosition;
	}

	if (AddedOldHistoryCount >= 2) // 捞傈 眠捞客 厚背窍妨绰 巴捞聪 酒抗 绝芭唱 窍唱父 啊瘤绊辑绰 厚背 阂啊窃.
	{
		const FInputHistoryElem& LastHistory = InputPointHistory[InputHistoryLastIndex];
		const FInputHistoryElem& LastLastHistory = InputPointHistory[GetPrevHistoryIndexOf(InputHistoryLastIndex)];
		checkSlow(LastHistory.bInitialized && LastLastHistory.bInitialized);

		FVector2D LastPosDelta = LastHistory.CheckPointPosition - LastLastHistory.CheckPointPosition;
		float LastTimeDelta = FMath::Abs(LastHistory.CheckPointTime - LastLastHistory.CheckPointTime);

		if (LastTimeDelta >= MinOddTimeInterval &&
			// PosDelta 绰 阿 夸家喊肺 夯促. 规氢捞 馆措搁辑 老沥 农扁 捞惑栏肺 脾菌绰瘤.
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
