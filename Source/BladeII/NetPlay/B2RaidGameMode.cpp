
#include "B2RaidGameMode.h"
//#include "BehaviorTree/BehaviorTree.h"
#include "B2NPCClassInfoBox.h"
#include "BladeIINetPlayer.h"
#include "BladeIIPlayerController.h"
#include "BladeIIAIController.h"
#include "B2UIManager.h"
#include "BladeIINetCharacter.h"
//#include "ChannelManager.h"
#include "Event.h"
#include "Packet.h"
#include "PacketMaker.h"
#include "Serializer.h"
#include "Retailer.h"
//#include "B2UIRaidBattle.h"
#include "B2UIDocHelper.h"
//#include "B2UIManager.h"
//#include "B2UIRaidJoin.h"
#include "B2UIDocLobbyMatch.h"
#include "B2StageEventDirector.h"
#include "B2CombatStatEvaluator.h"
//#include "B2UIGameInviteList.h"
#include "B2StageManager.h"
#include "B2RaidInfo.h"
#include "Algo/Count.h"
#include "BladeIIRaidNetCharacter.h"
#include "B2UIRaidMvpView.h"
////#include "Matinee/MatineeActor.h"
//#include "Matinee/InterpGroup.h"
//#include "Matinee/InterpGroupInst.h"
//#include "Matinee/InterpTrackMove.h"
//#include "B2FloatingHPBar.h"
#include "B2GameInstance.h"
#include "B2ServerResultCodeTable.h"


#include "FB2ErrorMessage.h"
#include "FakeConfigure.h"
#include "BladeIIGameImpl.h"

DEFINE_LOG_CATEGORY_STATIC(LogRaidGameMode, Log, All);

bool g_IsEditorRaidGamePreview = false;
EPCClass g_EditorRaidPreviewPCClassMain[4] = { EPCClass::EPC_Gladiator, EPCClass::EPC_Assassin, EPCClass::EPC_Wizard, EPCClass::EPC_Fighter };

EPCClass GetRaidSubPCClassForDev(EPCClass PCClass)
{
	// 俺惯侩 窃荐肺 PCClass客 嘎瘤臼篮 霉锅掳 PCClass甫 府畔
	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		EPCClass CheckPCClass = IntToPCClass(i);
		if (PCClass != CheckPCClass)
			return CheckPCClass;
	}

	return EPCClass::EPC_End;
}

FRaidMatchSceneManager::FRaidMatchSceneManager()
	: CurrentState(ERaidMatchSceneState::None), GameMode(nullptr)
{
	CheckAllLoadingPeerTryCount = 0;
	CheckAllPeerReadyBeginPlayTime = 0.0f;
	CheckAllPeerReadyLimitTime = 0.0f;
	CheckAllPeerReadyLeftTime = 0.0f;

	BattleBeginTime = 0;
	BattleLimitTime = 300;
	BattleLeftTime = 300;

	FailCountBeginPlayTime = 0.0f;
	FailCountLimitTime = 0.0f;
	FailCountLeftTime = 0.0f;

}

// ======================= FRaidMatchSceneManager
void FRaidMatchSceneManager::SetGameMode(class AB2RaidGameMode* InGameMode)
{
	GameMode = InGameMode;

	if (GameMode)
	{ // 饭骇 府家胶 匙捞萍宏 饭欺繁胶 某教 鸥捞怪.
		GameMode->RecacheLevelResources();
	}
}

void FRaidMatchSceneManager::SetState(ERaidMatchSceneState InState)
{
	BII_CHECK(GameMode);
	CurrentState = InState;

	switch (InState)
	{
	case ERaidMatchSceneState::Loading:
		SetLoading();
		break;
	case ERaidMatchSceneState::WaitMatchPlayer:
		SetWaitMatchPlayer();
		break;
	case ERaidMatchSceneState::PreparePlayers:
		SetPreparePlayers();
		break;
	case ERaidMatchSceneState::Battle:
		SetBattle();
		break;
	case ERaidMatchSceneState::EndMatchByTime:
		SetEndMatchByTime();
		break;
	default:
		break;
	}

	BII_CHECK(GameMode);
	auto* LocalController = GameMode->GetLocalController();
	if (LocalController)
		LocalController->SetVirtualJoystickVisibility(InState == ERaidMatchSceneState::Battle);
}

void FRaidMatchSceneManager::Tick(float DeltaSeconds)
{
	// State Tick
	//switch (CurrentState)
	//{
	//case ERaidMatchSceneState::WaitMatchPlayer:
	//case ERaidMatchSceneState::PreparePlayers:
	//	{
	//		// 甘函版 捞饶 概莫捞 力措肺夌绰瘤 眉农
	//		if (IsHost() && GameMode)
	//		{
	//			if (CheckAllPeerReadyLimitTime != 0.0f)
	//			{
	//				CheckAllPeerReadyLeftTime = (CheckAllPeerReadyLeftTime > 0.0f) ? FMath::Max<double>(CheckAllPeerReadyLimitTime - (FApp::GetCurrentTime() - CheckAllPeerReadyBeginPlayTime), 0.0) : 0.0f;
	//				CheckAllPeerReadyLeftTime = FMath::Clamp<double>(CheckAllPeerReadyLeftTime, 0.0f, CheckAllPeerReadyLimitTime);

	//				if (CheckAllPeerReadyLeftTime <= 0.0f)
	//				{
	//					if (CheckAllLoadingPeerTryCount < FRaidMatchSceneManager::MAX_CHECKREADY_TRY_COUNT)
	//					{
	//						GameMode->CheckAllPeerReady(CheckAllLoadingPeerTryCount);
	//						CheckAllLoadingPeerTryCount++;
	//					}
	//					else
	//					{
	//						// 老沥矫埃(CHECK_READY_TIME * MAX_CHECKREADY_TRY_COUNT) 悼救 雀脚捞 绝栏搁 弊成 霸烙胶鸥飘
	//						GameMode->ForceStartGame();
	//					}

	//					StartCheckAllPeerReady();
	//				}
	//			}
	//		}
	//	}
	//	break;
	//case ERaidMatchSceneState::Battle:
	//	{
	//		ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(GameMode));
	//		
	//		if (B2PC && B2PC->IsInCinematicMode())
	//		{
	//			BattleBeginTime += int64(DeltaSeconds * ETimespan::TicksPerSecond);
	//		}

	//		FTimespan BeginTimeStamp = UB2GameInstance::GetUTCNow() - FDateTime(BattleBeginTime);
	//		int32 BeinTimeSec = static_cast<int32>(BeginTimeStamp.GetTotalSeconds());

	//		BattleLeftTime = (BattleLeftTime > 0) ? FMath::Max<int32>(BattleLimitTime - BeinTimeSec, 0) : 0;
	//	
	//		if (BattleLeftTime <= 0)
	//		{
	//			BattleLeftTime = 0;
	//			
	//			if (IsHost())
	//			{
	//				GameMode->SendNotifyTimeEnd();
	//			}
	//		}

	//		if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//			DocRaid->SetRaidBattleTime(BattleLeftTime);

	//		if (FailCountBeginPlayTime != 0.f)
	//		{
	//			bool IsForceStopTimer = (GameMode && GameMode->IsAllPlayerDead() == false);
	//			FailCountLeftTime = (FailCountLeftTime > 0.0f) ? FMath::Max<double>(FailCountLimitTime - (FApp::GetCurrentTime() - FailCountBeginPlayTime), 0.0) : 0.0f;
	//			FailCountLeftTime = FMath::Clamp<double>(FailCountLeftTime, 0.0f, FailCountLimitTime);

	//			if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//				DocRaid->SetFailCountDownTime(IsForceStopTimer ? 0 : FMath::CeilToInt(FailCountLeftTime));

	//			if (IsForceStopTimer || FailCountLeftTime <= 0.0f)
	//			{
	//				StopFailCountDown();

	//				if (IsForceStopTimer == false && GameMode)
	//					GameMode->OnFinishRaidFailCountDown();
	//			}
	//		}
	//	}
	//	break;
	//}
	
}

bool FRaidMatchSceneManager::IsHost()
{
	return (GameMode && GameMode->IsHost());
}

void FRaidMatchSceneManager::OnHostChanged(int32 RoomType, int32 NewHostId)
{
	switch (CurrentState)
	{
	case ERaidMatchSceneState::Loading:
	case ERaidMatchSceneState::WaitMatchPlayer:
	case ERaidMatchSceneState::PreparePlayers:
		{
			BII_CHECK(GameMode);
			if (IsHost())
			{
				GameMode->SetMatchState(NetMatchState::LobbyMatchToGameMode);
				StartCheckAllPeerReady(true);
			}
		}
		break;
	case ERaidMatchSceneState::Battle:
	{
		SendStartTimeByHost();
	}
	break;
	}
}

void FRaidMatchSceneManager::StartFailCountDown()
{
	//FailCountBeginPlayTime = FApp::GetCurrentTime();
	//FailCountLimitTime = FailCountLeftTime = FRaidMatchSceneManager::FAIL_COUNTDOWN;
}

void FRaidMatchSceneManager::StopFailCountDown()
{
	FailCountBeginPlayTime = 0.f;
	FailCountLimitTime = 0.f; 
	FailCountLeftTime = 0.f;
}

void FRaidMatchSceneManager::StartCheckAllPeerReady(bool IsClearTryCount /*= false*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("FRaidMatchSceneManager::StartCheckAllPeerReady"));
	//
	//if (IsHost() == false)
	//	return;

	//CheckAllPeerReadyBeginPlayTime = FApp::GetCurrentTime();
	//CheckAllPeerReadyLimitTime = CheckAllPeerReadyLeftTime = FRaidMatchSceneManager::CHECK_READY_TIME;

	//if (IsClearTryCount)
	//	CheckAllLoadingPeerTryCount = 0;
}

void FRaidMatchSceneManager::SetLoading()
{
	// 饭捞靛 包访 贸府(概莫犬牢, 某腐磐 积己, 矫累悼扁拳) 甫 困秦 促矫 肺爹拳搁阑 焊咯霖促.
	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::Raid);
}

void FRaidMatchSceneManager::SetWaitMatchPlayer()
{
	if (IsHost())
		StartCheckAllPeerReady(true);
}

void FRaidMatchSceneManager::SetPreparePlayers()
{
	if (IsHost())
		StartCheckAllPeerReady(true);
}

void FRaidMatchSceneManager::SetBattle()
{
	BattleLimitTime = BattleLeftTime = FRaidMatchSceneManager::BATTLE_TIME;
	BattleBeginTime = UB2GameInstance::GetUTCNow().GetTicks();

	SendStartTimeByHost();

	// 肺爹拳搁 瘤况霖促.
	ClosePreLoadingScreenClass<EPreLoadingScreenType>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin);
}

void FRaidMatchSceneManager::SetBattleBeginTime(int64 time)
{
	BattleBeginTime = time;
}

void FRaidMatchSceneManager::SetEndMatchByTime()
{
	BII_CHECK(GameMode);

	//auto* RaidBattle = UB2UIManager::GetInstance()->GetUI<UB2UIRaidBattle>(UIFName::RaidBattle);
	//if (!RaidBattle)
	//	return;

	//StopDefeatMenuClass<>::GetInstance().Signal();
	//UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	//if (DocSome)
	//{
	//	RaidBattle->ShowResultImage(ENetMatchResult::LocalLose);
	//}

	//if (GameMode)
	//{
	//	GameMode->EndTimePlayerBossStop();
	//}
}

void FRaidMatchSceneManager::SendStartTimeByHost()
{
	BII_CHECK(GameMode);
	
	if (!IsHost()) return;

	TArray<FB2RoomPropertyPtr> properties;
	auto b2RoomProperty = std::make_shared<b2network::B2RoomProperty>();
	b2RoomProperty->key = "StartTime";
	b2RoomProperty->value = FString::Printf(TEXT("%lld"), BattleBeginTime);
	properties.Add(b2RoomProperty);

	data_trader::Retailer::GetInstance().RequestRequestJoinProperty(GameMode->GetRoomId(), properties);
	GameMode->SyncUserBattleStartTime();
}

// ======================= AB2RaidGameMode 
AB2RaidGameMode::AB2RaidGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = true;
	//bWaitingForResult = false;
	//DefaultRemotePawnClass = ABladeIINetPlayer::StaticClass();
	//MobNetIdCounter = 0x0000FFFF;
	//MaxMatchPlayerNum = AB2RaidGameMode::MAX_PLAYERS;
	//TokenToPlay.Empty();
	//AllPeerInfos.Empty();
	//CacheOtherPeerNetIds.Empty();
	//LastRequestedStageBuffType = EResurrectBuffType::RBT_None;
	//bRequestResult = false;

	////if (!GMinimalDLCFrontMode)
	////{ // DLC Front 葛靛 府家胶肺爹 弥措茄 力芭

	////  // 敲饭捞绢 纳腐磐侩 眉仿官+备福扁酿鸥烙侩 UI努贰胶
	////  // BP俊辑 犁汲沥且荐档乐澜
	////	FString FloatingResurrectionBarWidgetClassPath;
	////	GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("FloatingResurrectionBarWidgetClass"), FloatingResurrectionBarWidgetClassPath, GGameIni);
	////	static ConstructorHelpers::FClassFinder<UB2FloatingHPBar> FloatingBarWidgetClass(*FloatingResurrectionBarWidgetClassPath);
	////	if (FloatingBarWidgetClass.Class != NULL)
	////	{
	////		FloatingResurrectionBarWidgetClass = FloatingBarWidgetClass.Class;
	////	}
	////}
}

void AB2RaidGameMode::BeginPlay()
{
	Super::BeginPlay();

	//StageBeginClass<>::GetInstance().Signal();

	if (ABladeIIPlayer* B2Player = GetRaidPlayer(0))
	{
		B2Player->SetUnityTick(true);
	}
}

void AB2RaidGameMode::Tick(float delta)
{
	Super::Tick(delta);

	SceneManager.Tick(delta);
}

void AB2RaidGameMode::BeginDestroy()
{
	/*if (IsValid(RaidBoss) && !RaidBoss->IsPendingKill())
	{
		RaidBoss->Role = ROLE_Authority;
		RaidBoss->Destroy();
	}

	AllPeerInfos.Empty();
	CacheOtherPeerNetIds.Empty();

	if (auto* UIDocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		UIDocLobbyMatch->ClearRemotePlayerInfoData();*/

	Super::BeginDestroy();
}

void AB2RaidGameMode::Destroyed()
{
	//if (IsValid(RaidBoss) && !RaidBoss->IsPendingKill())
	//{
	//	RaidBoss->Role = ROLE_Authority;
	//	RaidBoss->Destroy();
	//}

	//AllPeerInfos.Empty();
	//CacheOtherPeerNetIds.Empty();

	//if (auto* UIDocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	//	UIDocLobbyMatch->ClearRemotePlayerInfoData();

	Super::Destroyed();
}

bool AB2RaidGameMode::HasMatchStarted() const
{
	return !(GetMatchState() == MatchState::EnteringMap || 
		GetMatchState() == MatchState::WaitingToStart ||
		GetMatchState() == NetMatchState::RequestMatchMaking ||
		GetMatchState() == NetMatchState::LobbyMatchToGameMode ||
		GetMatchState() == NetMatchState::WaitToMeetCondition ||
		GetMatchState() == NetMatchState::PreparePlayers);
}

void AB2RaidGameMode::SetMatchState(FName NewState)
{	
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetMatchState"));

	if (MatchState == NewState)
		return;

	Super::SetMatchState(NewState);
}

void AB2RaidGameMode::GoToVillage()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::GoToVillage"));

	LeaveRoomIfJoined();
}

void AB2RaidGameMode::HandleRequestMatchMaking()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleRequestMatchMaking"));

	UE_LOG(LogRaidGameMode, Log, TEXT("\t [B2RaidGameMode] HandleRequestMatchMaking Start"));

	ClearMatchRoom();

	MobNetIdCounter = 0x0000FFFF;
	AllPeerInfos.Empty();
	CacheOtherPeerNetIds.Empty();
	LastRequestedStageBuffType = EResurrectBuffType::RBT_None;
	
	auto* localController = GetLocalController();
	BII_CHECK(localController);
	localController->SetVirtualJoystickVisibility(false);
	
	SceneManager.SetGameMode(this);
	SceneManager.SetState(ERaidMatchSceneState::Loading);

	// 林狼 : 肺厚 概莫狼 版快 何葛 龋免窍搁 救凳!!!!
	// Super::HandleRequestMatchMaking();	

	SetMatchState(NetMatchState::LobbyMatchToGameMode);
}

void AB2RaidGameMode::PreBeginPlay()
{
	UE_LOG(LogRaidGameMode, Log, TEXT("\t [B2RaidGameMode] PreBeginPlay Start"));

	//auto* GameRule = new RaidGameRule;
	//SetGameRule(GameRule);

	//Super::PreBeginPlay();

	//PlayRaidBGM(ERaidBGMPlayContext::BGMCTX_Standby);
}

void AB2RaidGameMode::SubscribeEvents()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SubscribeEvents"));

	if (bEventsSubscribed)
		return;

	//Super::SubscribeEvents();

	//CAPTURE_UOBJECT(AB2RaidGameMode);
	//
	//Issues.Add(data_trader::ChannelDisconnectedEventClass<int32>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ChannelDisconnectedEvent, int32 errorCode)
	//		Capture->OnChannelDisconnectedEvent();
	//	END_CAPTURE_OBJECT()
	//	));

	//Issues.Add(ReceivedRaidNotifyResultClass<bool, uint32, TArray<TPair<int32, int32>> const &>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedRaidNotifyResult, bool IsSuccess, uint32 BonusPlayer, const TArray<TPair<int32, int32>> &Contributions)
	//		Capture->HandleNotifyRaidResult(IsSuccess, BonusPlayer, Contributions);
	//	END_CAPTURE_OBJECT()
	//	));

	//Issues.Add(TargetDamagedClass<ABladeIICharacter*, int32, float>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(TargetDamaged, ABladeIICharacter* c, int32 i, float f)
	//		Capture->NotifyTargetDamaged(c, i, f);
	//	END_CAPTURE_OBJECT()
	//	));

	//Issues.Add(StageEventDirectorFinishShowEventClass<int32>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(StageEventDirectorFinishShowEvent, int32 EventType)
	//		Capture->NotifyStageEventDirector_FinishShow(EventType);
	//	END_CAPTURE_OBJECT()
	//	));
	//						  
	//Issues.Add(ReturnToRaidMainClass<>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReturnToRaidMainClass)
	//	Capture->LeaveRoomIfJoined();
	//		END_CAPTURE_OBJECT()
	//	));

	//Issues.Add(ReceivedRaidResultClass<FB2ResponseRaidResultPtr>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedRaidResult, const FB2ResponseRaidResultPtr &Rewards)
	//	Capture->HandleResponseRaidResult(Rewards);
	//	END_CAPTURE_OBJECT()
	//	));

	//Issues.Add(HandleServerError4015InRaidClass<>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(HandleServerError4015InRaid)
	//		Capture->HandleResponseRaidResultInvenFull();
	//	END_CAPTURE_OBJECT()
	//	));
	//
	//Issues.Add(BornAgainChangedPlayerClass<int32, ABladeIIPlayer*, ABladeIIPlayer*>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(BornAgainChangedPlayer, int32 NetId, ABladeIIPlayer* Old, ABladeIIPlayer* New)
	//		Capture->HandleBornAgainChangedPlayer(NetId, Old, New);
	//	END_CAPTURE_OBJECT()
	//	));

	//Issues.Add(ReceivedSyncReadyToStartGameClass<uint32>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedSyncReadyToStartGame, uint32 NetId)
	//		Capture->HandleSyncReadyToStartGame(NetId);
	//	END_CAPTURE_OBJECT()
	//	));

	//Issues.Add(ReceivedSyncStartGameClass<>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedSyncStartGame)
	//		Capture->SetMatchState(MatchState::InProgress);
	//	END_CAPTURE_OBJECT()
	//	));

	//Issues.Add(ReceiveQteEnableClass<uint32, uint8>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceiveQteEnable, uint32 NetId, uint8 TeamNum)
	//		Capture->NetReceiveQTEEnable(NetId, TeamNum);
	//	END_CAPTURE_OBJECT()
	//	));

	//Issues.Add(ReceivedRaidNotifyTimeEndClass<uint32>::GetInstance().Subscribe2(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ReceivedRaidNotifyTimeEnd, uint32 NetId)
	//		Capture->HandleNotifyRaidTimeEnd(NetId);
	//	END_CAPTURE_OBJECT()
	//	));

	bEventsSubscribed = true;
}

void AB2RaidGameMode::SetupUIScene()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::RaidBattle);
}

void AB2RaidGameMode::NotifyTargetDamaged(class ABladeIICharacter* InNotifyOwner, int32 SuccessiveComboNum, float DamageAmount)
{
	UE_LOG(LogRaidGameMode, Log, TEXT("\t [B2RaidGameMode] NotifyTargetDamaged Start"));
	//单固瘤 拌魂阑 Master俊辑 窍扁锭巩俊 歹捞惑 荤侩窍瘤 臼澜
	if(GetPeerType() == PEER_RESTRICTED)
		return;

	if (GetMatchState() == NetMatchState::StartResult)
		return;

	if (InNotifyOwner == nullptr)
		return;

	int32 NetId = 0;

	// 刚历 磊扁磊脚牢瘤 犬牢
	if (InNotifyOwner == GetLocalController()->GetPawn())
	{
		NetId = GetMyNetId();
	}
	else
	{
		NetId = GetPeerNetId(InNotifyOwner);
	}

	if (AllPeerInfos.Contains(NetId) == false)
		return;

	AllPeerInfos[NetId].AttackDamage += DamageAmount;
}

void AB2RaidGameMode::NotifyStageEventDirector_FinishShow(int32 EventType)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::NotifyStageEventDirector_FinishShow"));
	//UE_LOG(LogRaidGameMode, Log, TEXT("\t [B2RaidGameMode] NotifyStageEventDirector_FinishShow Start"));

	//switch ((EStageEvent)EventType)
	//{
	//case EStageEvent::ESEV_MobDying:
	//	{
	//		// If PeerType isn't PEER_RESTRICTED, Handling Raid to succeed...
	//		if (GetPeerType() != PEER_RESTRICTED)
	//			SetStartResultState(true);
	//	}
	//	break;
	//case EStageEvent::ESEV_MobAppear:
	//	{
	//		// 弥檬绰 HandleMatchHasStarted俊辑 秦林绊 咯变 2其捞令何磐
	//		CheckPromoteNetStatus();
	//	}
	//	break;
	//}
}

void AB2RaidGameMode::NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadGuy)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::NotifyPlayerDeadEnd"));

	SetPlayerDead(GetPeerNetId(DeadGuy), true);

	bool IsAllDead = IsAllPlayerDead();
	
	// 葛滴 促 磷篮 版快绰 辆丰 墨款飘 矫累
	if (IsAllDead)
		SceneManager.StartFailCountDown();
	
	// 郴啊 磷菌阑 版快 混酒朝 荐 乐栏骨肺 扑诀 坷锹
	if (IsAllDead || DeadGuy == GetLocalController()->GetPawn())
		StartDefeatMenuClass<EB2GameMode>::GetInstance().Signal(EB2GameMode::Raid);
}

void AB2RaidGameMode::NotifyCharacterDead(ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::NotifyCharacterDead"));

	Super::NotifyCharacterDead(TargetCharacter, PawnInstigator, DamageCauser);

	// Check to kill the raid boss...
	if (TargetCharacter == RaidBoss)
	{
		ABladeIICharacter* Inflictor;

		// 捧荤眉老荐档 乐绢辑 笛促眉农
		if (PawnInstigator)
			Inflictor = Cast<ABladeIICharacter>(PawnInstigator);
		else
			Inflictor = Cast<ABladeIICharacter>(DamageCauser);

		if (Inflictor)
			StartEventScene_FinishAttack(Inflictor);
	}

	if (TargetCharacter->GetNPCClass() == ENPCClass::ENC_Boss_Sargon_P2)
	{
		PlayRaidBGM(ERaidBGMPlayContext::BGMCTX_SargonDie);
	}
}

float AB2RaidGameMode::GetCurrentStageDifficultyScale()
{
	auto* UIDocRaid = UB2UIDocHelper::GetDocRaid();
	BII_CHECK(UIDocRaid);

	const auto* mdRaid = BladeIIGameImpl::GetClientDataStore().GetRaidMasterDataList(UIDocRaid->GetLastRaidType());
	//BII_CHECK(mdRaid->Num() == 3);
	if (mdRaid == nullptr) 
		return 0.0;

	int32 DifficultyIdx = FMath::Max(UIDocRaid->GetLastRaidStep() - 1, 0);
	BII_CHECK(mdRaid->IsValidIndex(DifficultyIdx));

	auto RewardInfo = (*mdRaid)[DifficultyIdx];

	return static_cast<float>(RewardInfo.DifficultyScale) / 100.0f;
}

//ABladeIICharacter* AB2RaidGameMode::SpawnCharacter(UB2NPCClassInfoBox* InfoBox, ENPCClass InClass, ENPCClassVariation InVariation, FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters)
//{
//	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SpawnCharacter"));
//
//	//BII_CHECK(InfoBox);
//	//BII_CHECK(GetPeerType() != PEER_NONE);
//	//
//	//ABladeIICharacter*	NewSpawned = nullptr;
//
//	//NewSpawned = InfoBox->SpawnNPCClassInfoNetCharacter(GetWorld(), InClass, InVariation, SpawnTransform, SpawnParameters);
//	//auto* NetCharacter = Cast<ABladeIINetCharacter>(NewSpawned);
//	//if (NetCharacter)
//	//{
//	//	auto CharacterRole = GetPeerType() == PEER_RESTRICTED ? ABladeIINetCharacter::NET_SLAVE : ABladeIINetCharacter::NET_MASTER;
//
//	//	NetCharacter->SetNetId(++MobNetIdCounter);
//	//	NetCharacter->SetNetStatus(CharacterRole);
//	//	NetCharacter->GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
//	//}
//
//	//if (NewSpawned && NewSpawned->IsBossMob()/* && MobNetIdCounter == 0x00010000*/)
//	//{
//	//	RestartAllPlayers();
//
//	//	if (RaidBoss)
//	//	{
//	//		RaidBoss->Destroy();
//	//		RaidBoss = nullptr;
//	//	}
//
//	//	RaidBoss = Cast<ABladeIINetCharacter>(NewSpawned);
//
//	//	// 荤福帮 2其捞令 傈侩宏陛
//	//	if (RaidBoss->GetNPCClass() == ENPCClass::ENC_Boss_Sargon_P2)
//	//	{
//	//		PlayRaidBGM(ERaidBGMPlayContext::BGMCTX_SargonPhaseChange);
//	//	}
//	//}		
//
//	//return NewSpawned;
//	return NULL;
//}

ACharacter* AB2RaidGameMode::GetNearestPlayerCharacter(FVector Location, bool bSkipByCustomLogic)
{
	ABladeIIPlayer* ReturnPlayer	= nullptr;
	//float			MinDistanceSQ	= BIG_NUMBER;
	// 
	//if (!GetWorld())
	//	return ReturnPlayer;

	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	auto* Player = Cast<ABladeIINetPlayer>(*Iterator);
	//	if (!Player || Player->IsAlive() == false)
	//		continue;

	//	const float ThisDistanceSQ = (Player->GetActorLocation() - Location).SizeSquared();

	//	if (ThisDistanceSQ < MinDistanceSQ)
	//	{
	//		ReturnPlayer = Player;
	//		MinDistanceSQ = ThisDistanceSQ;
	//	}
	//}

	return ReturnPlayer;
}

void AB2RaidGameMode::GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum, bool bOnlyLiveMobs)
{
	GetAllWorldMobList(OutList, bOnlyLiveMobs);
}

void AB2RaidGameMode::GetAllWorldMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs)
{
	if (!(IsValid(RaidBoss) && !RaidBoss->bIsDying))
		return;

	OutList.Add(RaidBoss);
}

void AB2RaidGameMode::GetAllMyTeamCharList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum, bool bOnlyLive)
{
	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	auto* Player = Cast<ABladeIINetPlayer>(*Iterator);

	//	if (!Player)
	//		continue;

	//	if (Player->IsPendingKill())
	//		continue;

	//	if (bOnlyLive && !Player->IsAlive())
	//		continue;

	//	OutList.Add(Player);
	//}

	//// 评滚橇 吧副牢盔 急沥矫 悼扁拳甫困秦 齿酒捞叼鉴辑肺 沥纺
	//OutList.Sort([](const ABladeIICharacter& A, const ABladeIICharacter& B) { return B.GetNetId() > A.GetNetId(); });
}

void AB2RaidGameMode::SetMyPlayerInfoData(uint32 NetId, const FB2FindAccountInfoPtr& info)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetMyPlayerInfoData"));

	Super::SetMyPlayerInfoData(NetId, info);
}

void AB2RaidGameMode::SetRemotePlayerInfoData(uint32 Netid, int64 AccountId, const FB2FindAccountInfoPtr& info)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetRemotePlayerInfoData"));

	if (AllPeerInfos.Contains(Netid) == false)
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("RemovePeerPlayer - AllPeerInfos Not Exist (NetID).")));
#endif
		return;
	}

	Super::SetRemotePlayerInfoData(Netid, AccountId, info);

	EmplacePeerPlayer(Netid);

	if (AllPeerInfos[Netid].ReservedPCClass != EPCClass::EPC_End)
		SetRemotePlayerClass(Netid, AllPeerInfos[Netid].ReservedPCClass);
}

void AB2RaidGameMode::SetRoomParticipant(const TArray<FB2RoomParticipantPtr>& Participants)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetRoomParticipant"));

	BII_CHECK(Participants.Num() <= MaxMatchPlayerNum);

	for (auto& Participant : Participants)
	{
		if (Participant->id == GetMyNetId())
			continue;

		SetRemotePlayerInfoData(Participant->id, Participant->account_id, Participant->account_info);
	}
}

void AB2RaidGameMode::StartEventScene_MatchResult(bool HasLocalPlayerWon)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::StartEventScene_MatchResult"));

	if (HasLocalPlayerWon)
	{
		PlayRaidBGM(ERaidBGMPlayContext::BGMCTX_Victory, true);
	}
	else
	{ 
		//PlayRaidBGM(ERaidBGMPlayContext::BGMCTX_Defeat, true);
	}

	Super::StartEventScene_MatchResult(HasLocalPlayerWon);
}

void AB2RaidGameMode::SetResurrectPlayerPosition(ABladeIIPlayer* pBladePlayer)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetResurrectPlayer"));
		
	auto* PlayerController = GetLocalController();
	auto* pPlayer = Cast<ABladeIINetPlayer>(pBladePlayer);

	if (!pPlayer)
	{
		// 何劝 困摹 : 胶迄困摹		
		if (PlayerController == nullptr || PlayerController->GetPawn() == nullptr)
			return;

		pPlayer = Cast<ABladeIINetPlayer>(PlayerController->GetPawn());
	}

	if (!pPlayer)
		return;

	const float ZOffset = 50.f;
	// 官蹿苞 混娄 芭府甫 剁况辑
	const FVector FloatedLocatoin = pPlayer->GetActorLocation() + FVector(0.f, 0.f, ZOffset);

	pPlayer->SetActorLocationAndRotation(FloatedLocatoin, pPlayer->GetActorRotation(), true);
	pPlayer->SetLastResurrectLocation(FloatedLocatoin);
}

bool AB2RaidGameMode::RequestResurrect(EResurrectBuffType InResurrectBuffType)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::RequestResurrect"));

	int32 ServerResurrectBuffType = CliToSvrResurrectBuffType(InResurrectBuffType);
	if (ServerResurrectBuffType != b2network::B2ResurrectBuffType::BRBT_None)
	{
		LastRequestedStageBuffType = InResurrectBuffType;
		data_trader::Retailer::GetInstance().RequestRaidResurrection(GetToken(), ServerResurrectBuffType);

		return true;
	}
	return false;
}

void AB2RaidGameMode::RemotePlayerResurrect(uint32 NetId, FVector pos, FRotator rot, TMap<EResurrectBuffType, uint8>& BuffInfos)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::RemotePlayerResurrect"));

	Super::RemotePlayerResurrect(NetId, pos, rot, BuffInfos);

	SetPlayerDead(NetId, false);

	// UIDoc俊促啊 蜡历喊 滚橇瓤苞 悸泼 (UB2UITeamEntryCharacter UI钎矫, Spawn矫 滚橇 瓤苞俊 荤侩)
	if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		if (NetId == GetMyNetId())
		{
			DocBattle->SetCurResurrectBuffAttackCountByIndex(UB2UIDocBattle::MySlotIndex, BuffInfos.Contains(EResurrectBuffType::RBT_Attack) ? BuffInfos[EResurrectBuffType::RBT_Attack] : 0);
			DocBattle->SetCurResurrectBuffDefenseCountByIndex(UB2UIDocBattle::MySlotIndex, BuffInfos.Contains(EResurrectBuffType::RBT_Defense) ? BuffInfos[EResurrectBuffType::RBT_Defense] : 0);
			DocBattle->SetCurResurrectBuffHealthCountByIndex(UB2UIDocBattle::MySlotIndex, BuffInfos.Contains(EResurrectBuffType::RBT_Health) ? BuffInfos[EResurrectBuffType::RBT_Health] : 0);
		}
		else
		{
			if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
			{
				int32 BattleIndex = DocRaid->GetBattleSlotIndex(NetId);
				if (BattleIndex != -1)
				{
					DocBattle->SetCurResurrectBuffAttackCountByIndex(BattleIndex, (BuffInfos.Contains(EResurrectBuffType::RBT_Attack) ? BuffInfos[EResurrectBuffType::RBT_Attack] : 0));
					DocBattle->SetCurResurrectBuffDefenseCountByIndex(BattleIndex, (BuffInfos.Contains(EResurrectBuffType::RBT_Defense) ? BuffInfos[EResurrectBuffType::RBT_Defense] : 0));
					DocBattle->SetCurResurrectBuffHealthCountByIndex(BattleIndex, (BuffInfos.Contains(EResurrectBuffType::RBT_Health) ? BuffInfos[EResurrectBuffType::RBT_Health] : 0));
				}
			}
		}

		// 何劝矫 UIDOC盎脚
		AActor* pTarget = FindPawnByNetId(NetId);
		if (!pTarget) return;
		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(pTarget);
		if (pPlayer)
			HandleHealth(NetId, pPlayer->GetHealth(), NetId);

	}
}

void AB2RaidGameMode::LeaveRoomIfJoined()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::LeaveRoomIfJoined"));

	if (GetWorldTimerManager().IsTimerActive(BattleTimeSyncTimer))
		GetWorldTimerManager().ClearTimer(BattleTimeSyncTimer);

	if (GetPeerType() == PEER_STANDALONE)
		return;

	if (GetRoomId().IsEmpty())
	{
		// RoomID甫 给罐疽阑矫俊档 Level阑 函版秦霖促.
		HandleNotifyRoomLeave();
		return;
	}

	data_trader::Retailer::GetInstance().RequestLeaveMatch(GetRoomId());

	SetRoomId(TEXT(""));
}

void AB2RaidGameMode::HandleResponseRaidResult(const FB2ResponseRaidResultPtr& RaidResultReward)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleResponseRaidResult"));

	//UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	//if (DocSome == nullptr)
	//	return;

	//bool IsSuccess = false;
	//IsSuccess = (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin);

	//if (GetMatchState() != NetMatchState::StartResult)
	//	SetMatchState(NetMatchState::StartResult);

	//if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//	DocRaid->SetRaidResultReward(RaidResultReward);

	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	//迄促见辫
	//	if (ABladeIICharacter* pPawn = Cast<ABladeIICharacter>(*Iterator))
	//	{
	//		pPawn->SetActorHiddenInGame(true);
	//	}

	//	if (ABladeIINetPlayer* pNetPawn = Cast<ABladeIINetPlayer>(*Iterator))
	//	{
	//		pNetPawn->SetMovementDisable(true);
	//		pNetPawn->DisableComponentsSimulatePhysics();
	//		pNetPawn->DestroyResurrectionZoneActor();
	//	}
	//}

	//for (auto ThisInfo : RaidResultReward->light_characters)
	//{
	//	BladeIIGameImpl::GetClientDataStore().EndGameIncreaseExp(ThisInfo);
	//}

	//StartEventScene_MatchResult(IsSuccess);

	////牢亥俊 持扁
	//auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	//TArray<FB2Item>				AddedItems;
	//for (auto& SvrItem : RaidResultReward->items)
	//{
	//	int32 AddedIndex = AddedItems.AddDefaulted();
	//	check(AddedIndex != INDEX_NONE);
	//	AddedItems[AddedIndex] = SvrItem;
	//}

	//ClientDataStore.AddNewAcquiredItems(AddedItems);

	//// 抄捞档喊 焊惑 贸府(厘脚备沥荐)
	//if (RaidResultReward->step_clear_reward)
	//{
	//	ClientDataStore.ApplyRewardInfo(RaidResultReward->step_clear_reward);
	//}

	////ChangeUserAccountInfoClass<>::GetInstance().Signal();	// 泅犁 犁拳 沥焊绰 救坷绰单 眠啊啊 等促搁 捞巴栏肺 盎脚阑..

	//// todo delete : EUIScene::RaidResult 纠篮 鸥捞赣 悸泼捞 登绢具窃... (官肺 龋免窍搁 救凳)
	//////[@AKI, 170613] 距 匙飘况农俊辑 搬苞甫 辑滚俊辑 罐瘤 臼篮 惑怕俊辑 搬苞拳搁 焊咯林瘤 臼霸 函版
	////UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::RaidResult);
}

void AB2RaidGameMode::HandleResponseRaidResultInvenFull()
{
	/*auto& CharItemContainers = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().CharacterItemContainers;
	for (auto& CharItemContainer : CharItemContainers)
	{
		CharItemContainer.MakeDirty();
	}

	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(IntToPCClass(i));
	}
	UB2UIManager::GetInstance()->GoToLobbyPopup(B2ResultCode::FAILURE_NOT_ENOUGH_INVENTORY_LEFT_SLOT);*/
}

void AB2RaidGameMode::TryGetContributionList(TArray<TPair<int32, int32>> &ContributionArray)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::TryGetContributionList"));

	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		int32 TotalAttackDamage = GetTotalAttackDamage();
		if (auto* B2Player = GetRaidPlayer(0))
		{
			TPair<int32, int32> Each;
			Each.Key = GetMyNetId();
			Each.Value = DocRaid->GetRaidContributionByIndex(DocRaid->GetBattleSlotIndex(GetMyNetId()));
			ContributionArray.Add(Each);
		}
		for (int32 i = 0; i < RemotePlayerInfoData.Num(); ++i)
		{
			int32 NetID = 0;
			if (auto* B2Player = GetRaidPlayer(i + 1, ERaidPlayerLiveState::All, &NetID))
			{
				TPair<int32, int32> Each;
				Each.Key = NetID;
				Each.Value = DocRaid->GetRaidContributionByIndex(DocRaid->GetBattleSlotIndex(NetID));
				ContributionArray.Add(Each);
			}
		}
	}
}

bool AB2RaidGameMode::CheckRemotePlayer()
{
	return RemotePlayerInfoData.Num() == 0;
}

void AB2RaidGameMode::ErrorReturnToRaidMainMenu(int32 ErrorCode)
{
	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		DocRaid->SetRaidErrorCode(ErrorCode);
		DocRaid->SetReturnToPageType(ERaidGotoWhere::RaidMain);
	}

	GoToVillage();
}

void AB2RaidGameMode::CheckPromoteNetStatus()
{
	//ensure(RaidBoss);

	//if (RaidBoss && RaidBoss->IsValidObj())
	//{
	//	if (GetPeerType() == PEER_SUPER)
	//		RaidBoss->PromoteNetStatus();

	//	RaidBoss->StartPlay();
	//}
	//else
	//{
	//	//[FIXME][@AKI, 180103] 瘤陛篮 Debugging 侩栏肺 焊咯林瘤父 唱吝浚 Shipping栏肺 皑轿酒具窃.
	//	if (!RaidBoss)
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RaidBoss is nullptr"));
	//	if (!RaidBoss->IsValidObj())
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RaidBoss->IsValidObj() is false"));
	//}
}

void AB2RaidGameMode::RestartAllPlayers()
{
	//FString TagName = FString::Printf(TEXT("P%d"), AllPeerInfos[GetMyNetId()].PlayerStartIdx);

	//// AllPeerInfos[GetMyNetId()].PeerPlayer 绰 亲惑 澄牢芭鞍促. 肺拿敲饭捞绢 爱促靖.
	//ABladeIINetPlayer* B2Player = Cast<ABladeIINetPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//
	//if (B2Player && B2Player->IsAlive())
	//	RestartNetPlayer(GetLocalController(), TagName);

	//for (auto& Elem : RemotePlayerInfoData)
	//	RestartRemotePlayer(Elem.Key);
}

int32 AB2RaidGameMode::GetRankingNetID(int32 Rank)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::GetRankingNetID"));

	Rank = FMath::Clamp<int32>(Rank, 1, RemotePlayerInfoData.Num() + 1);
	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		TArray<TPair<int32, int32>> Contributions;
		TryGetContributionList(Contributions);
		Contributions.Sort([](const TPair<int32, int32>& A, const TPair<int32, int32>& B) {
			return A.Value > B.Value;
		});

		return Contributions.IsValidIndex(Rank - 1) ? Contributions[Rank - 1].Key : -1;
	}

	return -1;
}

int32 AB2RaidGameMode::GetMyRanking()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::GetMyRanking"));

	int ResultRank = 1;
	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		int32 TotalAttackDamage = GetTotalAttackDamage();
		int32 MyContribution = 0;

		for (int32 i = 0; i < RemotePlayerInfoData.Num() + 1; ++i)
		{
			int32 NetID = DocRaid->GetNetIdByIndex(i);
			if (NetID == GetMyNetId())
			{
				MyContribution = DocRaid->GetRaidContributionByIndex(DocRaid->GetBattleSlotIndex(NetID));
				continue;
			}
			int32 EachContribution = DocRaid->GetRaidContributionByIndex(DocRaid->GetBattleSlotIndex(NetID));
			if (EachContribution > MyContribution)
			{
				++ResultRank;
			}
		}
	}
	//[TODO_YJ] 珐欧捞 5啊 唱客, 焊惑阑 给罐绰 惑炔捞 积辫.. 
	// 200扒 沥档 惯积沁绰单 5捞惑狼 箭磊绰 唱坷瘤 臼酒 RemotePlayerInfoData俊 磊扁磊脚捞 甸绢埃霸 酒囱啊 眠螟吝.
	// 老窜 4 捞惑老订 4肺 绊沥秦辑 焊郴档废窍绊.. 眠利阑 拌加秦具窃.
	//BII_CHECK(!(ResultRank > 4));

	return FMath::Min(ResultRank, 4);
}

int32 AB2RaidGameMode::GetBonusPlayerNetID(uint32 BonusNum)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::GetBonusPlayerNetID"));

	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		TArray<TPair<int32, int32>> InfoArray;
		TryGetContributionList(InfoArray);
		InfoArray.Sort([](const TPair<int32, int32>& A, const TPair<int32, int32>& B) {
			return A.Value < B.Value;
		});

		for (int i = 0; i < InfoArray.Num(); ++i)
		{
			uint32 LeftSide = InfoArray[i].Value;
			uint32 RightSide = 0;
			for (int j = 0; j < i; ++j)
			{
				LeftSide += InfoArray[j].Value;
				RightSide += InfoArray[j].Value;
			}

			LeftSide *= 10;
			RightSide *= 10;

			if (LeftSide >= BonusNum && RightSide < BonusNum)
				return InfoArray[i].Key;
		}
	}

	return -1;
}

FVector AB2RaidGameMode::GetActorPositionFromMatineeActor(int32 PlayerIndex, bool IsWin)
{
	//ALevelSequenceActor * MatineeActor = IsWin ? SuccessResultMatinee : FailResultMatinee;

	//if (!MatineeActor)
		return FVector();

	//if (!MatineeActor->bIsPlaying)
	//	MatineeActor->InitInterp();

	//FString TagName = FString::Printf(TEXT("PC%d"), PlayerIndex);
	//FTransform SpawnTransform;
	//FVector ActorLocation;
	//for (auto* InterpGroup : MatineeActor->GroupInst)
	//{
	//	if (InterpGroup->Group == nullptr)
	//		continue;

	//	FString GroupName = InterpGroup->Group->GroupName.ToString();
	//	if (GroupName.StartsWith(TagName) && InterpGroup->GroupActor)
	//		ActorLocation = InterpGroup->GroupActor->GetActorLocation();
	//}

	//if (!MatineeActor->bIsPlaying)
	//	MatineeActor->TermInterp();

	//return ActorLocation;
}

void AB2RaidGameMode::HandleNotifyRaidResult(bool IsSuccess, uint32 BonusPlayer, const TArray<TPair<int32, int32>> &Contributions)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleNotifyRaidResult"));

	//if (GetMatchState() != NetMatchState::StartResult)
	//	SetMatchState(NetMatchState::StartResult);

	//StopDefeatMenuClass<>::GetInstance().Signal();

	//const bool IsBonusPlayer = BonusPlayer == GetMyNetId();
	//bool IsContribution = false;

	//if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//{
	//	for (int32 i = 0; i < Contributions.Num(); ++i)
	//	{
	//		const TPair<int32, int32> &Each = Contributions[i];

	//		int32 Index = DocRaid->GetBattleSlotIndex(Each.Key);
	//		if (Index < 0)
	//			continue;

	//		if (AllPeerInfos.Contains(Each.Key))
	//		{
	//			DocRaid->SetPCExitMainGameByIndex(Index, AllPeerInfos[Each.Key].IsExitMainGame);
	//			//DocRaid->SetAccountIdByIndex(Index, AllPeerInfos[Each.Key].PeerPlayer->GetAccountId());
	//		}

	//		DocRaid->SetRaidContributionByIndex(Index, Contributions.IsValidIndex(Index) ? Each.Value : 0);
	//		DocRaid->SetAlreadyExitUser(true);

	//		if (GetMyNetId() == Each.Key && Each.Value > 1)
	//			IsContribution = true;
	//	}
	//	DocRaid->SetRaidBonusPlayerNetID(BonusPlayer);

	//	DocRaid->SetRaidEndBattleTime((int32)(FRaidMatchSceneManager::BATTLE_TIME - DocRaid->GetRaidBattleTime()));
	//}

	//if (UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome())
	//	DocSome->SetLocalPlayerMatchResult(IsSuccess ? ENetMatchResult::LocalWin : ENetMatchResult::LocalLose);

	//bRequestResult = true;
	//data_trader::Retailer::GetInstance().RequestRaidResult(GetToken(), GetMyRanking(), IsBonusPlayer, (IsSuccess && IsContribution));
}

void AB2RaidGameMode::HandleBornAgainChangedPlayer(int32 InNetId, ABladeIIPlayer* oldPlayer, ABladeIIPlayer* newPlayer)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleBornAgainChangedPlayer"));

	//auto*	OldNetworkPlayer = Cast<ABladeIINetPlayer>(oldPlayer);
	//auto*	NewNetworkPlayer = Cast<ABladeIINetPlayer>(newPlayer);

	//if (NewNetworkPlayer)
	//{
	//	NewNetworkPlayer->SetNetId(InNetId);
	//	EmplacePeerPlayer(InNetId, NewNetworkPlayer);
	//	SetPlayerDead(InNetId, false);
	//}

	//if (InNetId == GetMyNetId())
	//{
	//	StopDefeatMenuClass<>::GetInstance().Signal();
	//}
}

void AB2RaidGameMode::HandleSyncReadyToStartGame(uint32 InNetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleSyncReadyToStartGame"));

	BII_CHECK(GetPeerType() == PEER_SUPER);

	if (AllPeerInfos.Contains(InNetId))
		AllPeerInfos[InNetId].IsFinishReady = true;

	//int32 ReadyCount = Algo::CountIf(AllPeerInfos, [](const TPair<uint32, FRaidPeerInfo>& PeerInfo) {return (PeerInfo.Value.IsFinishReady || !PeerInfo.Value.IsExitMainGame); });
	int32 ReadyCount = Algo::CountIf(AllPeerInfos, [](const TPair<uint32, FRaidPeerInfo>& PeerInfo) {return (PeerInfo.Value.IsFinishReady); });

	if (ReadyCount == AllPeerInfos.Num())
		SendStartGamePacket();
}

void AB2RaidGameMode::HandleNotifyRaidTimeEnd(uint32 InNetID)
{
	SceneManager.SetState(ERaidMatchSceneState::EndMatchByTime);
}

void AB2RaidGameMode::HandleNotifyRoomLeave()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleNotifyRoomLeave"));

	auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	if (RaidDoc)
	{
		RaidDoc->SetIsRequestPartyRematch(false);

		switch (RaidDoc->GetReturnToPageType())
		{
		case ERaidGotoWhere::Lobby:			
			ReturnToLobby(); 
			break;

		case ERaidGotoWhere::RaidMain:
		case ERaidGotoWhere::Rematch:
			ReturnToRaidMainMenu();
			break;

		case ERaidGotoWhere::PartyRematch:
			RaidDoc->SetIsRequestPartyRematch(true);
			ReturnToRaidMainMenu();
			break;
		}

		if (RaidDoc->GetIsRequestPartyRematch() == false)
			ClearPartyRetryInfo();

		if (!bRequestResult) // 霸烙吝 唱啊扁肺 甸绢吭阑 版快
		{
			bRequestResult = true;
			bool bIsGiveUp = RaidDoc->GetRaidErrorCode() == INDEX_NONE ? true : false; // 俊矾 惑炔栏肺 唱哎锭绰 器扁啊 酒丛.
			data_trader::Retailer::GetInstance().RequestRaidResult(GetToken(), GetMyRanking(), false, false, bIsGiveUp);
		}
	
	}
}

void AB2RaidGameMode::HandleNotifyRoomClose(const FString& RoomToken, int32 RoomType)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleNotifyRoomClose"));
}

bool AB2RaidGameMode::CanReceiveGameInvite(const FString& Inviter, EB2GameMode GameModeType) const
{
	return false;
}

void AB2RaidGameMode::HandleNotifyLeave(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleNotifyLeave"));

	//if (MatchState == NetMatchState::StartResult || 
	//	MatchState == MatchState::LeavingMap ||
	//	MatchState == MatchState::Aborted)
	//	return;	

	//if (MatchState == MatchState::InProgress)
	//{
	//	if (APawn* LeavePlayer = AB2NetGameMode::FindPawnByNetId(NetId))
	//	{
	//		// Role must be ROLE_Authority to delete actor from the world...
	//		LeavePlayer->Role = ROLE_Authority;
	//		GetWorld()->DestroyActor(LeavePlayer);
	//	}

	//	if (AllPeerInfos.Contains(NetId))
	//	{
	//		AllPeerInfos[NetId].IsExitMainGame = true;
	//		AllPeerInfos[NetId].PeerPlayer = nullptr;
	//		SendLeavePlayer(NetId); // 辑滚俊 唱埃 蜡历 贸府 夸没
	//	}

	//	// UI俊 唱埃 蜡历甫 钎矫
	//	if (auto* RaidBattleUI = UB2UIManager::GetInstance()->GetUI<UB2UIRaidBattle>(UIFName::RaidBattle))
	//	{
	//		if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//		{
	//			int32 nSlotIndex = DocRaid->GetBattleSlotIndex(NetId);
	//			RaidBattleUI->SetDisableSlot(nSlotIndex -1);
	//			DocRaid->SetPCExitMainGameByIndex(nSlotIndex, true);
	//			DocRaid->SetAlreadyExitUser(true);
	//			DocRaid->SetRaidContributionByIndex(nSlotIndex, 0);
	//		}
	//	}
	//}
	//else
	//{
	//	// 180213_YJ 肺爹吝老锭 呕林 眠啊 贸府
	//	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//	{
	//		int32 nSlotIndex = DocRaid->GetBattleSlotIndex(NetId);
	//		DocRaid->SetPCExitMainGameByIndex(nSlotIndex, true);
	//		DocRaid->SetAlreadyExitUser(true);
	//		DocRaid->SetPCClassByIndex(nSlotIndex, EPCClass::EPC_End);
	//		DocRaid->SetRaidContributionByIndex(nSlotIndex, 0);
	//	}

	//	if (AllPeerInfos.Contains(NetId))
	//	{
	//		SendLeavePlayer(NetId); // 辑滚俊 唱埃 蜡历 贸府 夸没
	//	}

	//	RemovePeerPlayer(NetId);
	//	RemotePlayerInfoData.Remove(NetId);
	//	MaxMatchPlayerNum = AllPeerInfos.Num();
	//}
}

void AB2RaidGameMode::HandleHealth(uint32 OwnerId, float Health, uint32 DamageCauserId)
{
	Super::HandleHealth(OwnerId, Health, DamageCauserId);

	if (MatchState == MatchState::InProgress)
	{
		if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
		{
			UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
			if (DocBattle)
			{
				if (OwnerId == GetMyNetId())
				{
					DocBattle->SetCurrPCHealth(Health);
				}
				//促弗蜡历 老锭(TMap捞 HashMap捞扼绊 UE Docs俊 乐栏聪 瓜绊)
				else if (RemotePlayerInfoData.Contains(OwnerId))
				{
					//[todo]瘤陛 备炼甫 救扒靛府绊 For巩阑 弥家拳 窍促 焊聪 捞犯霸 瞪荐 观俊 绝菌澜...
					int32 i = 1;
					for (auto& remotePlayer : RemotePlayerInfoData)
					{
						if (OwnerId == remotePlayer.Key)
						{
							DocRaid->SetPCHealthByIndex(i, Health);
							break;
						}
						i++;
					}
				}
				//焊胶老锭
				else if (OwnerId == RaidBoss->GetNetId())
				{
					//焊胶甫 锭赴霸 唱老锭
					if (DamageCauserId == GetMyNetId())
					{
						BossHealthAndDamage(0, Health, DamageCauserId);
						RevalueContribution(); //RemotePlayerInfoData俊 措秦 for巩 茄锅 倒绊 乐澜
					}
					//焊胶甫 锭赴霸 巢老锭(TMap捞 HashMap捞扼绊 UE Docs俊 乐栏聪 瓜绊)
					else if (RemotePlayerInfoData.Contains(DamageCauserId))
					{
						//[todo]瘤陛 备炼甫 救扒靛府绊 For巩阑 弥家拳 窍促 焊聪 捞犯霸 瞪荐 观俊 绝菌澜...
						int32 i = 1;
						for (auto& remotePlayer : RemotePlayerInfoData)
						{
							if (remotePlayer.Key == DamageCauserId)
							{
								BossHealthAndDamage(i, Health, DamageCauserId);
								break;
							}
							i++;
						}
						RevalueContribution();//RemotePlayerInfoData俊 措秦 for巩 茄锅 倒绊 乐澜
					}
					else if (OwnerId == DamageCauserId) //Healing
					{
						DocBattle->SetBossHPCurrent(Health);
					}
				}
			}
		}
	}
}

void AB2RaidGameMode::RevalueContribution()
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (!DocBattle) return;
	UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid();
	if (!DocRaid) return;

	//焊胶甫 锭赴霸 唱老锭
	//扁咯档绰 傈何 盎脚 秦拎具 窃
	int32 i = 1;
	DocRaid->SetRaidContributionByIndex(0, GetContribution(GetMyNetId()));
	for (auto& remotePlayer : RemotePlayerInfoData)
	{
		DocRaid->SetRaidContributionByIndex(i, GetContribution(remotePlayer.Key));
		i++;
	}
}

void AB2RaidGameMode::BossHealthAndDamage(uint32 _PlayerIDX, float _Health, uint32 _DamageCauserID)
{
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (!DocBattle) return;
	UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid();
	if (!DocRaid) return;

	float preHealth = DocBattle->GetBossHPCurrent();
	float damage = preHealth - _Health;
	DocBattle->SetBossHPCurrent(_Health);
	//雀汗 老荐档 乐栏聪鳖, 拱沸 雀汗老订 咯扁俊 救甸绢 坷摆瘤父
	if (damage > 0.0f)
	{
		if(AllPeerInfos.Contains(_DamageCauserID) )
			AllPeerInfos[_DamageCauserID].AttackDamage += damage;
	}
}

void AB2RaidGameMode::HandleMatchHasStarted()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleMatchHasStarted"));
//
//	SceneManager.SetState(ERaidMatchSceneState::Battle);
//
//	ensure(GetLocalController());
//	ensure(AllPeerInfos.Contains(GetMyNetId()));
//	ensure(AllPeerInfos[GetMyNetId()].PlayerStartIdx != 0);
//	
//	RestartAllPlayers();
//	
//	auto MyPawn = Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn());
//	BII_CHECK(MyPawn);
//
//	MyPawn->EnableInput(GetLocalController());
//
//	Super::HandleMatchHasStarted();
//
//#if WITH_EDITOR && !PLATFORM_MAC
//	if (RaidBoss == nullptr)
//	{
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//			FString::Printf(TEXT("饭捞靛 甘俊 焊胶啊 汲沥登瘤 臼疽嚼聪促. Map颇老俊 RaidBoss甫 汲沥秦林技夸."))
//		));
//	}
//#endif //WITH_EDITOR
//
//	CheckPromoteNetStatus();
//	
//	if (RaidBoss && RaidBoss->GetNPCClass() == ENPCClass::ENC_Boss_Sargon)
//	{
//		PlayRaidBGM(ERaidBGMPlayContext::BGMCTX_SargonCombat, true);
//	}
//	else
//	{
//		// 叼弃飘. 泅犁 窍尔父靖
//		PlayRaidBGM(ERaidBGMPlayContext::BGMCTX_Combat, true);
//	}
//
//	InitializeUpdateUIDoc();
//
//	AB2StageManager::GetCacheStageKeepEssentialData().SetClearResurrection(EResurrectGameModeType::EResurrectType_Raid, AB2RaidGameMode::MAX_RESURRECTION_COUNT);
//
//
//	TryAsyncLoadSkillAnims(&BladeIIGameImpl::GetLocalCharacterData());
//
//	for (auto& RemotePlayerData : RemotePlayerInfoData)
//		TryAsyncLoadSkillAnims(&RemotePlayerData.Value);
}

void AB2RaidGameMode::HandleStartResult()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleStartResult"));

	//Super::HandleStartResult();

	//StopDefeatMenuClass<>::GetInstance().Signal();
	//StopPauseMenuClass<>::GetInstance().Signal();
	//UB2UIManager::GetInstance()->CloseAllMsgPopups();
}

void AB2RaidGameMode::HandleNotifyUpdateRoomProperty(const TArray<b2network::B2RoomPropertyPtr>& PropertyList)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleNotifyUpdateRoomProperty"));
	for (auto each : PropertyList)
	{
		FString KeyName = each->key;
		if (KeyName.Equals(TEXT("StartTime")))
		{
			SceneManager.SetBattleBeginTime(FCString::Atoi64(*(each->value)));
			break;
		}
	}
}

void AB2RaidGameMode::HandleNotifyResult(ENetMatchResult Ret, uint8 InState)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleStartResult"));

	if (GetMatchState() != NetMatchState::StartResult)
		SetMatchState(NetMatchState::StartResult);

	StartEventScene_MatchResult(Ret == ENetMatchResult::LocalWin); // Match 辆丰俊 蝶弗 楷免阑 剁款促.
}

void AB2RaidGameMode::OnHostChanged(int32 RoomType, int32 NewHostId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::OnHostChanged"));
	uint32 BeforeHostId = GetHostId();

	Super::OnHostChanged(RoomType, NewHostId);

	if (GetMyNetId() == NewHostId)
	{
		//郴啊 规厘捞 登菌栏聪 Lobby俊辑 Doc俊 汲沥等 单捞磐甫 刚历 官槽促.
		auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch();
		if (DocLobbyMatch)
		{
			DocLobbyMatch->SetPeerType(PEER_SUPER);
			DocLobbyMatch->SetHostId(NewHostId);
		}
		SceneManager.OnHostChanged(RoomType, NewHostId);

		if (MatchState == MatchState::InProgress)
		{
			if (RaidBoss)
				RaidBoss->PromoteNetStatus();
			if (AllPeerInfos.Contains(BeforeHostId))
				SendLeavePlayer(BeforeHostId); // 辑滚俊 唱埃 蜡历 贸府 夸没
		
		}
		else if (MatchState == NetMatchState::WaitToMeetCondition)
		{
			if (AllPeerInfos.Contains(BeforeHostId))
				SendLeavePlayer(BeforeHostId); // 辑滚俊 唱埃 蜡历 贸府 夸没

			AllPeerInfos.Remove(BeforeHostId); // 辑滚俊 唱埃 蜡历 贸府 夸没(泅犁 Player Spwan傈 捞绢辑 Player甫 力芭 且 鞘夸绰 绝澜)
			RemotePlayerInfoData.Remove(BeforeHostId); // 辑滚俊 唱埃 蜡历 贸府 夸没(泅犁 Player Spwan傈 捞绢辑 Player甫 力芭 且 鞘夸绰 绝澜)

			int32 ReadyCount = Algo::CountIf(AllPeerInfos, [](const TPair<uint32, FRaidPeerInfo>& PeerInfo) {return PeerInfo.Value.IsFinishReady; });
			if (ReadyCount == AllPeerInfos.Num())
				SendStartGamePacket();
		}
	}
}

void AB2RaidGameMode::HandleLobbyMatchToGameMode()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleLobbyMatchToGameMode"));

	// LobbyGameMode Data -> RaidGameMode Data
	// 肺厚霸烙葛靛肺何磐 悸泼等 郴侩阑 饭捞靛俊 利犁秦霖促. 
	if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	{		
		SetPeerType(DocLobbyMatch->GetPeerType());
		SetRoomId(DocLobbyMatch->GetRoomId());
		SetHostId(DocLobbyMatch->GetHostId());
		SetToken(DocLobbyMatch->GetRoomToken());

		// 磊扁磊脚 悸泼
		SetMyPlayerInfoData(DocLobbyMatch->GetMyNetId(), FB2FindAccountInfoPtr());
		IncreasePeerPlayers(GetMyNetId());
		AllPeerInfos[GetMyNetId()].IsConnectedMap = true;

		// 蜡历悸泼
		for (int32 i = 0; i < DocLobbyMatch->GetRemotePlayerInfoDataCount(); ++i)
		{
			int32 NetId = DocLobbyMatch->GetRemoteNetIdByIndex(i);
			EPCClass ReservedPCClass = DocLobbyMatch->GetReservedRemotePCClassByIndex(i);
			
			if (ReservedPCClass != EPCClass::EPC_End)
				EmplacePeerPlayer(NetId, ReservedPCClass);

			SetRemotePlayerInfoData(NetId, DocLobbyMatch->GetRemoteAccountIdByIndex(i), DocLobbyMatch->GetRemoteAccountInfoByIndex(i));
		}
	}

	SetMatchState(NetMatchState::WaitToMeetCondition);
}

void AB2RaidGameMode::HandleWaitToMeetCondition()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandleWaitToMeetCondition"));

	BII_CHECK(!RoomWhereIAm.IsEmpty());
	BII_CHECK(GetMyNetId() != 0);

	SceneManager.SetState(ERaidMatchSceneState::WaitMatchPlayer);

	if (GetPeerType() == PEER_SUPER && IsStandaloneGame())
	{
		SetPreparePlayers();
	}
}

void AB2RaidGameMode::HandlePreparePlayers()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::HandlePreparePlayers"));

	SceneManager.SetState(ERaidMatchSceneState::PreparePlayers);

	// 郴 某腐磐客 颇萍盔 某腐磐 霖厚
	PrepareHostPlayer();
	
	for (auto& Elem : RemotePlayerInfoData)
		PrepareRemotePlayer(Elem.Key);

	SendSyncReadyToGame();
}

void AB2RaidGameMode::IncreasePeerPlayers(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::IncreasePeerPlayers"));

	if (MatchState == MatchState::InProgress)
		return;

	EmplacePeerPlayer(NetId);

	if (AllPeerInfos.Contains(NetId) && AllPeerInfos[NetId].PlayerStartIdx < 1)
	{
		AllPeerInfos[NetId].PlayerStartIdx = GetNextPlayStartIdx(NetId == GetMyNetId());
	}
}

void AB2RaidGameMode::SetRemotePlayerClass(uint32 NetId, EPCClass PCClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetRemotePlayerClass"));

	// 酒流 辑滚肺 何磐 概莫捞 登瘤 臼疽促搁 PCClass甫 抗距 吧绢敌促. (Reserved PCClass)
	if (RemotePlayerInfoData.Contains(NetId) == false)
	{
		EmplacePeerPlayer(NetId, PCClass);
		return;
	}

	Super::SetRemotePlayerClass(NetId, PCClass);

	IncreasePeerPlayers(NetId);
}

void AB2RaidGameMode::ResponsePrerequisiteToStart(int32 RoomType, const FString& RoomToken, TArray<FB2NetMatchPeerInfo> const& NetMatchPeerInfos)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::ResponsePrerequisiteToStart"));

	if (GetPeerType() != PEER_SUPER)
	{
		// Remove PeerInfo
		TArray<uint32> RemainNetIdList;
		for (auto& Peer : NetMatchPeerInfos)
			RemainNetIdList.Add(Peer.NetId);

		RemoveAllButRemainPeerList(RemainNetIdList);

		// Set Player StartIdx
		for (auto& PeerInfo : NetMatchPeerInfos)
		{
			if (PeerInfo.NetId == GetMyNetId())
				EmplacePeerPlayer(GetMyNetId());

			ensure(AllPeerInfos.Contains(PeerInfo.NetId));
			if (AllPeerInfos.Contains(PeerInfo.NetId))
				AllPeerInfos[PeerInfo.NetId].PlayerStartIdx = PeerInfo.PlayerStartIndex;
		}
	}

	Super::ResponsePrerequisiteToStart(RoomType, RoomToken, NetMatchPeerInfos);
}

void AB2RaidGameMode::ResponseAckPrerequisiteToStart(uint32 InNetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::ResponseAckPrerequisiteToStart"));

	UE_LOG(LogBladeII, Log, TEXT("AB2RaidGameMode::ResponseAckPrerequisiteToStart NetId : %d MyNetId : %d"), InNetId, GetMyNetId());

	BII_CHECK(GetPeerType() == PEER_SUPER);

	if (AllPeerInfos.Contains(InNetId))
		AllPeerInfos[InNetId].IsConnectedMap = true;
	else
		UE_LOG(LogBladeII, Log, TEXT("AB2RaidGameMode::ResponseAckPrerequisiteToStart AllPeerInfos Not Exist : %d"), InNetId);

	Super::ResponseAckPrerequisiteToStart(InNetId);
}

void AB2RaidGameMode::PrepareHostPlayer()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::PrepareHostPlayer"));

	auto* PlayerController = GetLocalController();
	if (PlayerController->GetPawn() != NULL)
	{
		PlayerController->SetPawn(nullptr);
	}

	BII_CHECK(AllPeerInfos.Contains(GetMyNetId()));
	FString TagName = FString::Printf(TEXT("P%d"), AllPeerInfos[GetMyNetId()].PlayerStartIdx);
	RestartNetPlayer(PlayerController, TagName);

	auto MyPawn = Cast<ABladeIINetPlayer>(PlayerController->GetPawn());
	BII_CHECK(MyPawn)

	MyPawn->SetNetId(GetMyNetId());
	MyPawn->DisableInput(PlayerController);
	MyPawn->SetMovementDisable(true);
	MyPawn->SetActorHiddenInGame(true);
	MyPawn->SetTagEnable(false);
	SetLocalController(PlayerController);

	// RAID AspectRatioAdaptiveFOV, 20180716
	// 漂沥 葛靛俊辑狼 傈捧甘 叼磊牢 荤沥栏肺 AspectRatioAdaptiveFOV 甫 荤侩窍霸 凳.
	// 荤侩登绰 饭骇苞 墨皋扼 轰啊 犬角洒 窍唱肺 绊沥登绢 乐瘤 臼促搁 ActiveCameraActor 殿 甘 汲沥阑 烹秦 甸绢啊绰 巴捞 亮澜.
	//MyPawn->TopDownCameraComponent->SetAspectRatioAdaptiveFOV(true);

	//auto  Asset = GetGameRule()->GetBehaviorTreeAsset();
	//if (!Asset.IsEmpty())
	//	MyPawn->BehaviorTree = LoadObject<UBehaviorTree>(NULL, *Asset);
}

void AB2RaidGameMode::PrepareRemotePlayer(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::PrepareRemotePlayer"));

	ensure(AllPeerInfos.Contains(NetId));

	if (AllPeerInfos.Contains(NetId) == false)
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("PrepareRemotePlayer - AllPeerInfos Not Exist (NetID).")));
#endif
		return;
	}

	// 龋胶飘啊 AB2RaidGameMode::CheckAllPeerReady 滴锅焊郴绰 纳捞胶 惯斑(府葛飘敲饭捞绢 滴付府 积己窃)
	// 夸没 咯矾锅窍绰扒 沥惑纳捞胶 鞍栏聪 览翠贸府扼档 茄锅父 窍磊.
	if (AllPeerInfos.Contains(NetId) && AllPeerInfos[NetId].PeerPlayer)
	{
		return;
	}

	FString TagName = FString::Printf(TEXT("P%d"), AllPeerInfos[NetId].PlayerStartIdx);
	AActor* StartSpot = FindPlayerStart(NULL, TagName);
	BII_CHECK(StartSpot);
	
	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = nullptr;
	SpawnParam.bDeferConstruction = true;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FTransform SpawnTransform = StartSpot->GetTransform();
	auto* RemotePlayer = Cast<ABladeIINetPlayer>(AB2NetGameMode::SpawnRemotePlayer(GetWorld(), RemotePlayerInfoData[NetId].GetMainPlayerClass(), SpawnTransform, SpawnParam));
	BII_CHECK(RemotePlayer);

	RemotePlayer->CurrentPlayingClass = RemotePlayer->MainPlayerClass = RemotePlayerInfoData[NetId].GetMainPlayerClass();
	RemotePlayer->SetNetStatus(ABladeIINetPlayer::NET_SLAVE);
	RemotePlayer->SetNetId(NetId);
	RemotePlayer->DisableComponentsSimulatePhysics();
	RemotePlayer->SetActorHiddenInGame(true);
	RemotePlayer->SetTagEnable(false);
	RemotePlayer->FinishSpawning(SpawnTransform);
	
	EmplacePeerPlayer(NetId, RemotePlayer);
}

// 困摹 犁汲沥
void AB2RaidGameMode::RestartRemotePlayer(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::RestartRemotePlayer"));

	if (AllPeerInfos.Contains(NetId) == false)
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("RestartRemotePlayer - AllPeerInfos Not Exist (NetID).")));
#endif
		return;
	}

	// 秦寸 蜡历啊 盲澄 栋唱搁 澄老荐 乐澜
	//BII_CHECK(AllPeerInfos[NetId].PeerPlayer);
	BII_CHECK(AllPeerInfos[NetId].PlayerStartIdx != 0);

	auto*	OtherPlayer = AllPeerInfos[NetId].PeerPlayer;
	uint8	PlayerStartIdx = FMath::Clamp<uint8>(AllPeerInfos[NetId].PlayerStartIdx, 1, 4);
	auto	TagName = FString::Printf(TEXT("P%d"), PlayerStartIdx);
	auto*	start		= MyFindPlayerStart(TagName);
	BII_CHECK(start);

	if (start)
	{
		FRotator StartRotation(ForceInit);
		StartRotation.Yaw = start->GetActorRotation().Yaw;
		FVector StartLocation = start->GetActorLocation();

		if (OtherPlayer)
		{
			OtherPlayer->TeleportTo(StartLocation, StartRotation);
			OtherPlayer->SetActorHiddenInGame(false);
		}
	}
}

void AB2RaidGameMode::ResponseChangeRoomAccess(uint8 AccessKey)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::ResponseChangeRoomAccess"));

	//if (auto* RaidJoinUI = UB2UIManager::GetInstance()->GetUI<UB2UIRaidJoin>(UIFName::RaidJoin))
	//	RaidJoinUI->ChangeQuickStartRoom();
}

void AB2RaidGameMode::SetEndMatchByTime()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetEndMatchByTime"));
//	
//	if(GetWorldTimerManager().IsTimerActive(BattleTimeSyncTimer))
//		GetWorldTimerManager().ClearTimer(BattleTimeSyncTimer);
//
//	if (GetPeerType() == PEER_RESTRICTED)
//		return;
//
//#if !UE_BUILD_SHIPPING
//	if (NetFakeConfigure::GetInstance().GetSkipEndGameByTime())
//		return;
//#endif
//
//	if (MatchState != MatchState::InProgress)
//		return;
//
//	// 刚历 Wave啊 努府绢 登菌阑 锭绰 鸥烙酒眶 公矫..
//	if (bWaitingForResult == true)
//		return;
//
//	SetStartResultState(false);
}

void AB2RaidGameMode::PlayRaidBGM(ERaidBGMPlayContext InContext, bool bForceStopCurrent /*= true*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::PlayRaidBGM"));

	//if (bForceStopCurrent){
	//	StopBGM();
	//}

	//if (StaticFindCommonSoundInfo(this))
	//{
	//	ECommonSoundID MatchingSoundID = ECommonSoundID::ECSID_End;
	//	switch (InContext)
	//	{
	//	case ERaidBGMPlayContext::BGMCTX_Standby: MatchingSoundID = ECommonSoundID::ECSID_BGM_RaidStandby; break;
	//	case ERaidBGMPlayContext::BGMCTX_Combat: MatchingSoundID = ECommonSoundID::ECSID_BGM_RaidCombat; break;
	//	case ERaidBGMPlayContext::BGMCTX_Victory: MatchingSoundID = ECommonSoundID::ECSID_BGM_RaidVictory; break;
	//	case ERaidBGMPlayContext::BGMCTX_Defeat: MatchingSoundID = ECommonSoundID::ECSID_BGM_RaidDefeat; break; // 老窜 脸阑 锭档 惑措规 铰府 楷免捞 唱坷骨肺 付蛮啊瘤肺..
	//	case ERaidBGMPlayContext::BGMCTX_SargonCombat: MatchingSoundID = ECommonSoundID::ECSID_BGM_RaidSargonCombat; break;
	//	case ERaidBGMPlayContext::BGMCTX_SargonPhaseChange: MatchingSoundID = ECommonSoundID::ECSID_BGM_RaidSargonPhaseChange; break;
	//	case ERaidBGMPlayContext::BGMCTX_SargonDie: MatchingSoundID = ECommonSoundID::ECSID_BGM_RaidSargonDie; break;
	//	}
	//	PlayBGMByCommonSoundID(MatchingSoundID);
	//}
}

void AB2RaidGameMode::EmplacePeerPlayer(int32 InNetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::EmplacePeerPlayer"));

	if (AllPeerInfos.Contains(InNetId))
		return;

	AllPeerInfos.Emplace(InNetId, FRaidPeerInfo());
}

void AB2RaidGameMode::EmplacePeerPlayer(int32 InNetId, ABladeIINetPlayer* NewNetPlayer)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::EmplacePeerPlayer"));

	if(AllPeerInfos.Contains(InNetId) == false)
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("EmplacePeerPlayer - AllPeerInfos Not Exist (NetID).")));
#endif
		return;
	}

	EmplacePeerPlayer(InNetId);

	auto* OldPlayer = AllPeerInfos[InNetId].PeerPlayer;

	// Check ExistPlayer
	if (OldPlayer == NewNetPlayer)
		return;

	// Remove OldPlayer
	if (OldPlayer != nullptr && CacheOtherPeerNetIds.Contains(OldPlayer))
		CacheOtherPeerNetIds.Remove(OldPlayer);

	// Add NewPlayer
	AllPeerInfos[InNetId].PeerPlayer = NewNetPlayer;
	if (InNetId != GetMyNetId() && NewNetPlayer != nullptr)
		CacheOtherPeerNetIds.Emplace(NewNetPlayer, InNetId);
}

void AB2RaidGameMode::EmplacePeerPlayer(int32 InNetId, EPCClass PCClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::EmplacePeerPlayer"));

//	if (AllPeerInfos.Contains(InNetId))
//	{
//#if !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("EmplacePeerPlayer PCClass - AllPeerInfos Not Exist (NetID).")));
//#endif
//		return;
//	}
//
//	EmplacePeerPlayer(InNetId);
//	AllPeerInfos[InNetId].ReservedPCClass = PCClass;
//
//	if (AllPeerInfos[InNetId].IsExitMainGame)
//	{
//		if (APawn* LeavePlayer = AB2NetGameMode::FindPawnByNetId(InNetId))
//		{
//			// Role must be ROLE_Authority to delete actor from the world...
//			LeavePlayer->Role = ROLE_Authority;
//			GetWorld()->DestroyActor(LeavePlayer);
//		}
//	}
}

void AB2RaidGameMode::RemovePeerPlayer(int32 InNetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::RemovePeerPlayer"));

	if (AllPeerInfos.Contains(InNetId) == false)
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("RemovePeerPlayer - AllPeerInfos Not Exist (NetID).")));
#endif
		return;
	}

	if (InNetId != GetMyNetId())
	{
		if (auto* PeerPlayer = AllPeerInfos[InNetId].PeerPlayer)
			CacheOtherPeerNetIds.Remove(PeerPlayer);
	}

	AllPeerInfos.Remove(InNetId);
}

void AB2RaidGameMode::SetStartResultState(bool IsSuccess)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetStartResultState"));

	//BII_CHECK(GetPeerType() == PEER_SUPER);

	//// 泅犁 搬苞 柳青吝捞搁 公矫....
	//if (GetMatchState() == NetMatchState::StartResult)
	//{
	//	UE_LOG(LogRaidGameMode, Log, TEXT("\t [B2RaidGameMode] Already StartResult"));
	//	return;
	//}
	//SetMatchState(NetMatchState::StartResult); // call HandleStartResult
	////SetResultRaidContribution();

	//uint32 BonusNum = FMath::RandRange(1, 1000);
	//int32 BonusPlaerNetID = GetBonusPlayerNetID(BonusNum);
	//packet::ByteStream	payload;
	//payload << IsSuccess;
	//payload << BonusPlaerNetID;

	//if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//{
	//	payload << static_cast<uint8>(RemotePlayerInfoData.Num() + 1);

	//	payload << GetMyNetId() << DocRaid->GetRaidContributionByIndex(0);

	//	for (int32 i = 0; i < RemotePlayerInfoData.Num(); ++i)
	//		payload << DocRaid->GetNetIdByIndex(i + 1) << DocRaid->GetRaidContributionByIndex(i+1);
	//}

	//FString encoded_string = packet::FinalizePacket(packet::RAID_NOTIFYRESULT, GetMyNetId(), 0, packet::ALL, payload);
	//data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);
}

void AB2RaidGameMode::InitializeUpdateUIDoc()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::InitializeUpdateUIDoc"));
	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		DocRaid->ClearRaidBattleData();

		auto* B2LocalPlayer = GetRaidPlayer(0);
		DocRaid->SetNetIdByIndex(0, GetMyNetId());
		DocRaid->SetPCHealthByIndex(0, B2LocalPlayer->Health);
		DocRaid->SetCharNameByIndex(0, DocRaid->GetJoinSlotCharNameByIndex(DocRaid->GetMySlotIdx()));
		DocRaid->SetPCClassByIndex(0, DocRaid->GetJoinSlotPCClassByIndex(DocRaid->GetMySlotIdx()));
		DocRaid->SetPCLevelByIndex(0, DocRaid->GetJoinSlotCharLevelByIndex(DocRaid->GetMySlotIdx()));
		DocRaid->SetIsMyGuildMemberByIndex(0, DocRaid->GetJoinSlotIsMyGuildMemberByIndex(DocRaid->GetMySlotIdx()));
		
		int32 NetID = 0;
		for (int32 i = 0; i < RemotePlayerInfoData.Num(); ++i)
		{
			if (auto* B2Player = Cast<ABladeIINetPlayer>(GetRaidPlayer(i + 1, ERaidPlayerLiveState::All, &NetID)))
			{
				DocRaid->SetNetIdByIndex(i + 1, NetID);
				DocRaid->SetCharNameByIndex(i + 1, FText::FromString(RemotePlayerInfoData[NetID].GetUserNickName()));
				DocRaid->SetPCClassByIndex(i + 1, B2Player->CurrentPlayingClass);
				DocRaid->SetPCLevelByIndex(i + 1, B2Player->GetPlayerLevel());
				DocRaid->SetPCHealthByIndex(i + 1, B2Player->Health);
				DocRaid->SetPCMaxHealthByIndex(i + 1, B2Player->MaxHealth);
				DocRaid->SetAccountIdByIndex(i + 1, B2Player->GetAccountId());
				DocRaid->SetIsMyGuildMemberByIndex(i + 1, DocRaid->GetJoinSlotIsMyGuildMemberByIndex(DocRaid->GetJoinSlotIndex(NetID)));
			}
		}
	}

	//if (auto* RaidBattleUI = UB2UIManager::GetInstance()->GetUI<UB2UIRaidBattle>(UIFName::RaidBattle))
		//RaidBattleUI->InitEntryData();

}

void AB2RaidGameMode::RealtimeUpdateUIDoc()
{
	Super::RealtimeUpdateUIDoc();

	if (MatchState == MatchState::InProgress)
	{		
		if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
		{
			int32 TotalAttackDamage = GetTotalAttackDamage();
			
			auto* B2LocalPlayer = GetRaidPlayer(0, ERaidPlayerLiveState::All);
			if (B2LocalPlayer && GetPeerType() == PEER_SUPER)
			{
				DocRaid->SetPCHealthByIndex(0, B2LocalPlayer->Health);
				DocRaid->SetRaidContributionByIndex(0, GetContribution(GetMyNetId(), TotalAttackDamage));
			}

			for (int32 i = 0; i < RemotePlayerInfoData.Num(); ++i)
			{
				int32 NetID = 0;
				if (auto* B2Player = GetRaidPlayer(i + 1, ERaidPlayerLiveState::All, &NetID))
				{
					DocRaid->SetPCMaxHealthByIndex(i + 1, B2Player->MaxHealth);
					DocRaid->SetPCShieldByIndex(i + 1, B2Player->GetShield());
					DocRaid->SetPCInCriticalConditionByIndex(i + 1, B2Player->IsInCriticalCondition());
					if (GetPeerType() == PEER_SUPER)
					{
						DocRaid->SetPCHealthByIndex(i + 1, B2Player->Health);
						DocRaid->SetRaidContributionByIndex(i + 1, GetContribution(NetID, TotalAttackDamage));
					}
				}
			}
		}
	}
}

void AB2RaidGameMode::OnChannelDisconnectedEvent()
{
	if(IsLoadingState())
		ErrorReturnToRaidMainMenu(AB2RaidGameMode::CHANNEL_DISCONNECTED);
}

int32 AB2RaidGameMode::GetMatchPlayerNum()
{
	return AllPeerInfos.Num();
}

int32 AB2RaidGameMode::GetMaxMatchPlayerNum()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::GetMaxMatchPlayerNum"));
	return MaxMatchPlayerNum;
}

bool AB2RaidGameMode::IsMatchMaxPlayerNum()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::IsMatchMaxPlayerNum"));

	return (GetMatchPlayerNum() == GetMaxMatchPlayerNum());
}

void AB2RaidGameMode::GetDelayConnectPeerList(TArray<uint32>& OutPeerList)
{
	OutPeerList.Empty();

	for (auto& PeerInfo : AllPeerInfos)
	{
		if (PeerInfo.Key == GetMyNetId())
			continue;

		if (PeerInfo.Value.IsConnectedMap == false)
			OutPeerList.Add(PeerInfo.Key);
	}
}

void AB2RaidGameMode::GetDelayReadyPeerList(TArray<uint32>& OutPeerList)
{	
	OutPeerList.Empty();

	for (auto& PeerInfo : AllPeerInfos)
	{
		if (PeerInfo.Key == GetMyNetId())
			continue;

		if (PeerInfo.Value.IsFinishReady == false)
			OutPeerList.Add(PeerInfo.Key);
	}
}

void AB2RaidGameMode::RemoveDelayPeerList(const TArray<uint32>& RemovePeerList)
{
	for (auto NetId : RemovePeerList)
	{
		RemovePeerPlayer(NetId);
		RemoveRemotePlayer(NetId);
	}

	MaxMatchPlayerNum = AllPeerInfos.Num();
}

void AB2RaidGameMode::RemoveAllButRemainPeerList(const TArray<uint32>& RemainPeerList)
{
	TSet<uint32> RemainPeerSet(RemainPeerList);
	TArray<uint32> RemovePeerList;

	for (auto& PeerInfo : AllPeerInfos)
	{
		int32 NetId = PeerInfo.Key;
		if (RemainPeerSet.Contains(NetId) == false)
			RemovePeerList.Add(NetId);
	}

	RemoveDelayPeerList(RemovePeerList);
}

void AB2RaidGameMode::SetPlayerDead(const uint32 NetId, bool IsDead)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetPlayerDead"));

	if (AllPeerInfos.Contains(NetId) == false)
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("RemovePeerPlayer - AllPeerInfos Not Exist (NetID).")));
#endif
		return;
	}

	AllPeerInfos[NetId].IsDead = IsDead;
}

bool AB2RaidGameMode::IsAllPlayerDead()
{	
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::IsAllPlayerDead"));

	// 唱啊瘤臼篮 蜡历甸吝俊 葛滴 磷菌绰瘤 眉农
	for (auto& PeerInfo : AllPeerInfos)
	{
		if (PeerInfo.Value.IsExitMainGame == false && 
			PeerInfo.Value.IsDead == false)
			return false;
	}

	return true;
}

void AB2RaidGameMode::StartMatchResult()
{
	SetEndMatchByTime();
}

int32 AB2RaidGameMode::GetMyJoinSlotIndex()
{
	int32 i = 0;
	for (auto& PeerInfo : AllPeerInfos)
	{
		if (PeerInfo.Key == GetMyNetId())
			return i;

		i++;
	}

	return 0;
}

int32 AB2RaidGameMode::GetJoinSlotIndex(const int32 NetId)
{		
	int32 SlotIdx = 0;
	for (auto& Elem : AllPeerInfos)
	{
		if (Elem.Key == NetId)
			break;

		SlotIdx++;
	}

	return SlotIdx;
}

uint8 AB2RaidGameMode::GetNextPlayStartIdx(bool IsMyStartIdx)
{
	// 林狼 : 付萍聪狼 PC Index绰 1何磐 矫累!!
	// 林狼 : 磊扁磊脚篮 公炼扒 1锅 浇吩
	if (IsMyStartIdx)
		return 1;

	uint8 PCPlayStartIdx = 1;
	for (auto& peerInfo2 : AllPeerInfos)
	{
		if (peerInfo2.Value.PlayerStartIdx > PCPlayStartIdx)
			PCPlayStartIdx = peerInfo2.Value.PlayerStartIdx;
	}

	PCPlayStartIdx+=1;

	return FMath::Clamp<uint8>(PCPlayStartIdx, 1, 4);
}

int32 AB2RaidGameMode::GetPeerNetId(ABladeIICharacter* B2Character)
{
	if (CacheOtherPeerNetIds.Contains(B2Character))
		return CacheOtherPeerNetIds[B2Character];

	if (B2Character == Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn()))
		return GetMyNetId();

	return -1;
}

bool AB2RaidGameMode::CanStartInGame()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::CanStartInGame"));

	if(IsMatchMaxPlayerNum() || IsForceStartMatch())
	{
		for (auto& PeerInfo : AllPeerInfos)
		{
			// 磊扁磊脚阑 眉农窍瘤富酒具且锭绰 菩胶秦霖促.
			if (PeerInfo.Key == GetMyNetId())
				continue;

			if (PeerInfo.Value.IsConnectedMap == false)
				return false;
		}

		return true;
	}

	return false;
}

void AB2RaidGameMode::SendResurrectHelper(const ABladeIINetPlayer* player)
{
	if (player == nullptr || player->GetNetId() == 0) return;

	//if (auto* RaidBattleUI = UB2UIManager::GetInstance()->GetUI<UB2UIRaidBattle>(UIFName::RaidBattle))
	//{
	//	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//	{
	//		for (int32 i = 0; i < RemotePlayerInfoData.Num() + 1; ++i)
	//		{
	//			int32 NetID = DocRaid->GetNetIdByIndex(i);
	//			if (NetID == player->GetNetId())
	//			{
	//				RaidBattleUI->SetResurrectHelperNotice(DocRaid->GetCharNameByIndex(i));
	//				break;
	//			}
	//		}
	//	}
	//}
}

void AB2RaidGameMode::CheckAllPeerReady(int32 CurCheckCount)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::CheckAllPeerReady()"));

	UE_LOG(LogRaidGameMode, Log, TEXT("\t [B2RaidGameMode] CheckAllPeerReady Start"));

	BII_CHECK(GetPeerType() == PEER_SUPER);

	if (IsStandaloneGame())
		return;

	BII_CHECK(AllPeerInfos.Num() != 0);
	ensure(RemotePlayerInfoData.Num() != 0);

	// 饭捞靛 傈侩 函荐
	uint8 TempPlayerTeamType = static_cast<uint8>(ETeamType::ETT_End);

	// 磊脚阑 力寇窍绊 颇萍盔俊霸 霖厚 脚龋甫 傈崔 (龋胶飘 扁霖栏肺 某腐磐 胶鸥飘困摹 悼扁拳)
	packet::ByteStream	payload;
	FString strToken(GetToken());
	payload << static_cast<int32>(b2network::B2RoomType::BRT_Raid) << strToken <<
		static_cast<uint8>(RemotePlayerInfoData.Num() + 1) <<
		GetMyNetId() << BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass() << AllPeerInfos[GetMyNetId()].PlayerStartIdx << TempPlayerTeamType;
	
	for (auto& Elem : RemotePlayerInfoData)
	{
		ensure(AllPeerInfos.Contains(Elem.Key));
		if (AllPeerInfos.Contains(Elem.Key))
			payload << Elem.Key << RemotePlayerInfoData[Elem.Key].GetMainPlayerClass() << AllPeerInfos[Elem.Key].PlayerStartIdx << TempPlayerTeamType;
	}

	FString encoded_string = packet::FinalizePacket(packet::SEND_PREREQUISITE_TO_START, GetMyNetId(), 0, packet::ALLBUTME, payload);

	if (CurCheckCount == 0)
	{
		// 霖厚等 某腐磐 檬扁拳
		for (auto& PeerInfo : AllPeerInfos)
			PeerInfo.Value.IsConnectedMap = false;

		//data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);
	}
	else
	{
		// 霖厚
		for (auto& PeerInfo : AllPeerInfos)
		{
			//if (PeerInfo.Value.IsConnectedMap == false)
				//data_trader::ChannelManager::GetInstance().SendMessage(static_cast<int32>(PeerInfo.Key), encoded_string);
		}
	}
}

void AB2RaidGameMode::SendSyncReadyToGame()
{
	if (GetPeerType() == PEER_SUPER)
	{
		HandleSyncReadyToStartGame(GetMyNetId());
		return;
	}

	packet::ByteStream	payload;
	payload << GetMyNetId();
	FString encoded_string = packet::FinalizePacket(packet::SYNC_READY_TO_STARTGAME, GetMyNetId(), 0, packet::ALLBUTME, payload);
	//data_trader::ChannelManager::GetInstance().SendMessage(GetHostId(), encoded_string);
}

void AB2RaidGameMode::ForceStartGame()
{
	// Note : 老沥 矫埃 掉饭捞 登菌阑矫 掉饭捞等 蜡历 昏力窍绊 霸烙 矫累
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::ForceStartGame()"));

	BII_CHECK(GetPeerType() == PEER_SUPER);

	if (MatchState == NetMatchState::WaitToMeetCondition)
	{
		// 掉饭捞等 蜡历 昏力
		TArray<uint32> DelayNetIdList;
		GetDelayConnectPeerList(DelayNetIdList);
		RemoveDelayPeerList(DelayNetIdList);

		if (RemotePlayerInfoData.Num() == 0)
		{
			SetPreparePlayers();
		}
		else
		{
			CheckAllPeerReady(0);
		}
	}
	else if (MatchState == NetMatchState::PreparePlayers)
	{
		// 掉饭捞等 蜡历 昏力
		TArray<uint32> DelayNetIdList;
		GetDelayReadyPeerList(DelayNetIdList);
		RemoveDelayPeerList(DelayNetIdList);

		// 霸烙 矫累
		SendStartGamePacket();
	}
}

int32 AB2RaidGameMode::GetTotalAttackDamage()
{
	int32 TotalDamage = 0;

	for (auto& PeerData : AllPeerInfos)
	{
		// 霸烙郴俊辑 唱埃 蜡历绰 扁咯档拌魂 救窃
		if (PeerData.Value.IsExitMainGame)
			continue;
		TotalDamage += PeerData.Value.AttackDamage;
	}

	return TotalDamage;
}

int32 AB2RaidGameMode::GetAttackDamage(int32 NetId)
{
	if (AllPeerInfos.Contains(NetId) == false)
		return 0;

	return AllPeerInfos[NetId].AttackDamage;
}

uint8 AB2RaidGameMode::GetContribution(int32 NetId, int32 PeerTotalAttackDamage)
{
	// Note : 扁咯档 = (郴 傍拜 单固瘤 / 颇萍盔 傍拜单固瘤 醚钦) * 100
	if (PeerTotalAttackDamage == 0)
		PeerTotalAttackDamage = GetTotalAttackDamage();

	// 傍拜茄 利捞 绝栏搁 扁咯档绰 0
	if (PeerTotalAttackDamage == 0)
		return 0;

	int32 AttackDamage = GetAttackDamage(NetId);
	UE_LOG(LogRaidGameMode, Log, TEXT("==========================Start===================================="));
	UE_LOG(LogRaidGameMode, Log, TEXT("\t AttackDamge : [%d], PeerTotalAttackDamage : [%d]"), AttackDamage, PeerTotalAttackDamage);
	int32 contribution = FMath::RoundToInt((AttackDamage / (float)FMath::Max(PeerTotalAttackDamage, 1)) * 100);
	UE_LOG(LogRaidGameMode, Log, TEXT("\t Contribution [%d]"), contribution);
	UE_LOG(LogRaidGameMode, Log, TEXT("===================================================================\n"));
	return  FMath::RoundToInt((AttackDamage / (float)FMath::Max(PeerTotalAttackDamage, 1)) * 100);
}

void AB2RaidGameMode::SetResultRaidContribution()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetResultRaidContribution"));

	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{

		int32 AttackDamage = 0;
		float Friction = 0.0f;
		float TotalFriction = 0.0f;

		int32 TotalAttackDamage = GetTotalAttackDamage();
		if (auto* B2Player = GetRaidPlayer(0))
		{
			DocRaid->SetRaidContributionByIndex(0, B2Player ? GetContribution(GetMyNetId(), TotalAttackDamage) : 0);
		}
		for (int32 i = 0; i < RemotePlayerInfoData.Num(); ++i)
		{
			int32 NetID = 0;
			if (auto* B2Player = GetRaidPlayer(i + 1, ERaidPlayerLiveState::All, &NetID))
			{
				DocRaid->SetRaidContributionByIndex(i + 1, B2Player ? GetContribution(NetID, TotalAttackDamage) : 0);
			}
		}
	}
}

bool AB2RaidGameMode::SetupEditorRaidDefaultInfo()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetupEditorRaidDefaultInfo"));
//
//#if UE_BUILD_SHIPPING
//	return false;
//#endif //UE_BUILD_SHIPPING
//
//	if (g_IsEditorRaidGamePreview == false)
//		return false;
//	
//	APlayerController* CandidatePCController = NULL;
//	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
//	{
//		APlayerController* PlayerController = Iterator->Get();
//		if (PlayerController && PlayerController->GetPawn() == NULL)
//		{
//			CandidatePCController = PlayerController;
//			break;
//		}
//	}
//
//	if (CandidatePCController)
//	{
//		// FakeMyNetId甫 父甸绢霖促. (啊扼肺..)
//		SetRoomId(TEXT("EditorPreviewRoom"));
//		SetLocalController(CandidatePCController);
//		CurrentPCClassEnum = g_EditorRaidPreviewPCClassMain[0];
//
//		BladeIIGameImpl::GetLocalCharacterData().SetMainPlayerClass(CurrentPCClassEnum);
//		BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(GetRaidSubPCClassForDev(CurrentPCClassEnum));
//
//		// 碍力肺 烙狼 悸泼!!
//		const uint32 FakeMyNetId = (FDateTime::UtcNow().ToUnixTimestamp() % 200000) + 3000000;
//		SetMyNetId(FakeMyNetId);	
//		
//		OnHostChanged(0, FakeMyNetId);
//
//		EmplacePeerPlayer(FakeMyNetId, Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn()));
//		IncreasePeerPlayers(FakeMyNetId);
//		
//		BII_CHECK(BladeIIGameImpl::GetClientDataStore().ExistAccountInfo());
//
//		for (uint32 i = 0; i < 3; ++i)
//		{
//			const uint32 FakeRemoteNetId = FakeMyNetId + (i+1);
//			FB2ModPlayerInfo& MyInfo = RemotePlayerInfoData.Emplace(FakeRemoteNetId);
//
//			BladeIIGameImpl::GetClientDataStore().GetRandomCharacterData(MyInfo);
//			MyInfo.SetMainPlayerClass(g_EditorRaidPreviewPCClassMain[i + 1]);
//			MyInfo.SetSubPlayerClass(GetRaidSubPCClassForDev(g_EditorRaidPreviewPCClassMain[i + 1]));
//			
//			IncreasePeerPlayers(FakeRemoteNetId); 
//		}
//	
//		SetPreparePlayers();
//	}

	return true;
}

void AB2RaidGameMode::SetupEditorRaidPreviewRemoteDead()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SetupEditorRaidPreviewRemoteDead"));

#if UE_BUILD_SHIPPING
	return;
#endif //UE_BUILD_SHIPPING

	if (UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome())
		DocSome->SetLocalPlayerMatchResult(ENetMatchResult::LocalWin);
	
	StartEventScene_MatchResult(true);
}

void AB2RaidGameMode::SendLeavePlayer(uint32 NetId)
{
//	B2_SCOPED_TRACK_LOG(TEXT("AB2RaidGameMode::SendLeavePlayer"));
//	if (GetPeerType() == PEER_SUPER)
//	{
//#if !UE_BUILD_SHIPPING
//		if (NetFakeConfigure::GetInstance().GetHateRaidPenalty()) return; // 目盖靛 摹搁 呕林 力力 公矫.
//#endif
//		int32 PlayerAccountID = 0;
//		if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
//		{
//			PlayerAccountID = DocLobbyMatch->GetRemoteAccountId(NetId);
//		}
//
//		if(PlayerAccountID != 0)
//			data_trader::Retailer::GetInstance().RequestRoomLeaveResult(GetRoomId(), PlayerAccountID, GetToken());
//	}
}

void AB2RaidGameMode::NetReceiveQTEEnable(uint32 NetId, uint8 TeamNum)
{
	if (RaidBoss && RaidBoss->GetNetId() == NetId)
	{
		RaidBoss->ApplyQTEEnable(ETeamType(TeamNum));
	}
}

bool AB2RaidGameMode::IsLoadingState()
{
	switch (SceneManager.GetState())
	{
		case ERaidMatchSceneState::Loading:
		case ERaidMatchSceneState::WaitMatchPlayer:
		case ERaidMatchSceneState::PreparePlayers:
			return true;
	}
	return false;
}

void AB2RaidGameMode::SyncUserBattleStartTime()
{
	if (!IsHost()) return;

	GetWorldTimerManager().ClearTimer(BattleTimeSyncTimer);
	GetWorldTimerManager().SetTimer(BattleTimeSyncTimer, this, &AB2RaidGameMode::BattleStartTimeCheck, 5.0f, false);
}

void AB2RaidGameMode::BattleStartTimeCheck()
{
	GetWorldTimerManager().ClearTimer(BattleTimeSyncTimer);
	SceneManager.SendStartTimeByHost();
}

void AB2RaidGameMode::SetAddFriendButtonForRaidResult()
{
	//UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	//UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid();
	//if (DocSome == nullptr || DocRaid == nullptr)
	//	return;

	//bool IsSuccess = false;
	//IsSuccess = (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin);

	//UB2UIRaidMvpView* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMvpView>(UIFName::RaidMvpView);
	//APlayerController* PCControl = UGameplayStatics::GetLocalPlayerController(this);
	//if (pUI == nullptr || PCControl == nullptr)
	//	return;

	//int32 MvpNetId = IsSuccess ? GetRankingNetID(1) : -1;

	//if (ABladeIINetPlayer* B2Player = Cast<ABladeIINetPlayer>(GetRaidPlayer(0)))
	//{
	//	ICharacterDataStore* pDataStore = B2Player->GetCharacterDataStore();
	//	int32 RaidContribution = DocRaid->GetRaidContributionByIndex(DocRaid->GetBattleSlotIndex(B2Player->GetNetId()));

	//	if(ResultLocationArray[0] != nullptr)
	//		pUI->AddPlayer(ResultLocationArray[0], B2Player, B2Player->GetNetId() == MvpNetId, RaidContribution);
	//}

	//int32 MatineePCIndex = 2;
	//for (auto PeerInfo : AllPeerInfos)
	//{
	//	if (PeerInfo.Value.IsExitMainGame)
	//	{
	//		++MatineePCIndex;
	//		continue;
	//	}

	//	ABladeIINetPlayer* NetPlayer = PeerInfo.Value.PeerPlayer;
	//	if (NetPlayer && !NetPlayer->IsPendingKill()) //isPendingKill篮 啊厚瘤 拿发记俊 甸绢啊绰 惑怕老锭 八荤烙
	//	{
	//		if (NetPlayer->GetNetId() == GetMyNetId())
	//			continue;

	//		int32 RaidContribution = DocRaid->GetRaidContributionByIndex(DocRaid->GetBattleSlotIndex(NetPlayer->GetNetId()));

	//		if (ResultLocationArray[MatineePCIndex - 1] != nullptr)
	//			pUI->AddPlayer(ResultLocationArray[MatineePCIndex - 1], NetPlayer, NetPlayer->GetNetId() == MvpNetId, RaidContribution);

	//		++MatineePCIndex;
	//	}
	//}
}

void AB2RaidGameMode::ReturnToRaidMainMenu()
{
	//FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
	//	// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst) {
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterDungeonMain);
	//	}
	//	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_RaidMain);
	//});
	//OpenBladeIILobbyCommon(this); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
}

void AB2RaidGameMode::ReturnToLobby()
{
	//FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
	//	// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
	//	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_LobbyMain);
	//});			
	//OpenBladeIILobbyCommon(this); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
}

//void AB2RaidGameMode::SetMatchLobbyResources(class ALevelSequenceActor * IsSuccessMatinee, 
//	class ALevelSequenceActor * InFailMatinee,
//	AActor* ResultActor01,
//	AActor* ResultActor02,
//	AActor* ResultActor03,
//	AActor* ResultActor04)
//{
//	SuccessResultMatinee = IsSuccessMatinee;
//	FailResultMatinee = InFailMatinee;
//
//	// 171213_YJ
//	// 饭捞靛狼 版快 铰府,角菩老锭 付萍聪啊 犁积捞 凳.
//	// 弊繁单 阿 某腐磐喊肺 4镑俊辑 促弗 局聪皋捞记阑 犁积吝捞扼,
//	// 某腐磐 捞抚 免仿侩 UI甫 困茄 绊沥利牢 困摹甫 掘扁 塞甸菌澜.
//	// 弊贰辑 困摹蔼阑 绊沥茄 歹固 咀磐甫 4俺 汲摹秦辑 啊廉坷霸窃.
//	ResultLocationArray.Empty();
//	ResultLocationArray.Add(ResultActor01);
//	ResultLocationArray.Add(ResultActor02);
//	ResultLocationArray.Add(ResultActor03);
//	ResultLocationArray.Add(ResultActor04);
//}

void AB2RaidGameMode::OnMatineeEnd_StartEvent()
{
	SceneManager.SetBattleBeginTime(UB2GameInstance::GetUTCNow().GetTicks());
	BattleStartTimeCheck();
}

ABladeIIPlayer* AB2RaidGameMode::GetRaidPlayer(int32 BattlePlayerIndex, ERaidPlayerLiveState LiveState /*= ERaidPlayerLiveState::All*/, int32* outNetId /*= nullptr*/)
{
	ABladeIIPlayer* B2Player = nullptr;

	//if (BattlePlayerIndex == 0)
	//{
	//	B2Player = Cast<ABladeIIPlayer>(GetLocalController()->GetPawn());
	//	if (outNetId)
	//		*outNetId = GetMyNetId();
	//}
	//else
	//{
	//	int32 i = 1; 
	//	for (auto& PeerInfo : AllPeerInfos)
	//	{
	//		if (PeerInfo.Key == GetMyNetId())
	//			continue;

	//		if (BattlePlayerIndex == i)
	//		{
	//			if (PeerInfo.Value.IsExitMainGame == false)
	//			{
	//				B2Player = PeerInfo.Value.PeerPlayer;
	//				if (outNetId)
	//					*outNetId = PeerInfo.Key;
	//			}

	//			break;
	//		}

	//		i++;
	//	}
	//}

	//if (B2Player == nullptr)
	//	return nullptr;

	//if (B2Player->IsValidObj() == false)
	//	return nullptr;

	//switch (LiveState)
	//{
	//case ERaidPlayerLiveState::All:
	//	break;
	//case ERaidPlayerLiveState::LiveOnly:
	//	if (B2Player->IsAlive() == false)
	//		return nullptr;
	//	break;
	//case ERaidPlayerLiveState::DefeatOnly:
	//	if (B2Player->IsAlive())
	//		return nullptr;
	//	break;
	//}

	return B2Player;
}

int32 AB2RaidGameMode::GetClientStageId()
{
	//if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//{		
	//	if (auto* RaidInfoTable = StaticFindRaidInfo())
	//		return RaidInfoTable->GetClientStageIdByRaidMapId(DocRaid->GetLastRaidType(), DocRaid->GetLastRaidStep());
	//}

	return 1;
}

int32 AB2RaidGameMode::GetRaidStep()
{
	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
		return DocRaid->GetLastRaidStep();

	return 1;
}

void AB2RaidGameMode::NotifyUIWidgetOpenComplete(FName InOpenedUI)
{
	Super::NotifyUIWidgetOpenComplete(InOpenedUI);

	if (InOpenedUI == UIFName::RaidResult)
		ShowCachedCompleteQuestNotify();
}

void AB2RaidGameMode::OnFinishRaidFailCountDown()
{
	if (GetPeerType() == PEER_RESTRICTED)
		return;
	
	SetStartResultState(false);
}

void AB2RaidGameMode::NotifySpawnPoolComplete(class AB2MonsterSpawnPool* CompletedSpawnPool)
{
	Super::NotifySpawnPoolComplete(CompletedSpawnPool);

	bWaitingForResult = true;
}

#include "B2CombatStatEvaluator.h"

float AB2RaidGameMode::GetTotalAttack(ICharacterDataStore* CharacterDataStore)
{
	const auto MainPCClass = CharacterDataStore->GetMainPlayerClass();

	float Attack = CombatStatEval::GetPCAttack(MainPCClass, CharacterDataStore);
	float ModSpecializeAttack = CombatStatEval::GetPCAttackByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);

	float TotalAttack = Attack + ModSpecializeAttack;
	TotalAttack = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(MainPCClass))), TotalAttack);

	return TotalAttack;
}

float AB2RaidGameMode::GetTotalDefense(ICharacterDataStore* CharacterDataStore)
{
	const auto MainPCClass = CharacterDataStore->GetMainPlayerClass();

	float Defense = CombatStatEval::GetPCDefense(MainPCClass, CharacterDataStore);
	float ModSpecializeDefense = CombatStatEval::GetPCDefenseByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);

	float TotalDefense = Defense + ModSpecializeDefense;
	TotalDefense = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(MainPCClass))), TotalDefense);

	return TotalDefense;
}

bool AB2RaidGameMode::CanTakeDamage()
{
	return SceneManager.GetState() == ERaidMatchSceneState::Battle;
}

float AB2RaidGameMode::GetModifiedBossmobMaxHealth()
{
	const int32 PlayerNum = GetMatchPlayerNum();
	const float ReduceRateByPlayer = static_cast<float>(PlayerNum) / AB2RaidGameMode::MAX_PLAYERS;

	return ReduceRateByPlayer;
}

void AB2RaidGameMode::GatherEquippedSkillAnimIDs(ICharacterDataStore* DataStore, TArray<FCombinedPCSkillAnimID>& OutEquippedSkills)
{
	// Raid绰 MainClass父 SkillAnim Load
	//OutEquippedSkills.Append(GetEquippedSkillAnimIDs(DataStore->GetMainPlayerClass(), DataStore));
}

void AB2RaidGameMode::NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector)
{
	if (SceneManager.GetState() == ERaidMatchSceneState::Battle)
	{
		if (RaidBoss && RaidBoss->IsReservedAttackAfterSpecialAction())
		{
			RaidBoss->SetReservedAttackAfterSpecialAction(false);
			RaidBoss->AllowMovementByStageEvent();
			RaidBoss->SetSignalAttack(true);
		}
	}
}

void AB2RaidGameMode::EndTimePlayerBossStop()
{
	//// 郴某腐磐 肛勉
	//auto* PlayerController = GetLocalController();
	//auto MyPawn = Cast<ABladeIINetPlayer>(PlayerController->GetPawn());
	//if (PlayerController)
	//{
	//	if (MyPawn)
	//	{
	//		MyPawn->DisableInput(PlayerController);
	//		MyPawn->SetInvincibleByStageEvent(true); // 趣矫唱 平单固瘤甸绢客辑 磷阑吧 规瘤..
	//	}

	//	ABladeIIPlayerController* pPC = Cast<ABladeIIPlayerController>(PlayerController);
	//	if (pPC)
	//	{
	//		pPC->SetStageEndNonCombatState(true);
	//		pPC->StopAutoPlay();
	//	}
	//}

	//// 焊胶 肛勉
	//if (RaidBoss && RaidBoss->IsValidObj() && !RaidBoss->IsPendingKill())
	//{
	//	ABladeIIAIController* pAC = Cast<ABladeIIAIController>(RaidBoss->GetController());
	//	if (pAC)
	//		pAC->StopAutoPlay();
	//}
}

void AB2RaidGameMode::SendNotifyTimeEnd()
{
	/*if (!IsHost()) return;

	packet::ByteStream	payload;
	FString encoded_string = packet::FinalizePacket(packet::RAID_NOTIFYTIMEEND, GetMyNetId(), 0, packet::ALL, payload);
	data_trader::ChannelManager::GetInstance().SendMessage(data_trader::ChannelManager::ROOM_All, encoded_string);*/
}

bool AB2RaidGameMode::IsStandaloneGame()
{
	if (GetMaxMatchPlayerNum() == 1 || AllPeerInfos.Num() == 1)
		return true;

	return false;
}

bool AB2RaidGameMode::IsForceStartMatch()
{
	if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
		return DocRaid->GetIsForceStartGame();

	return false;
}

void AB2RaidGameMode::ClearPartyRetryInfo()
{
	if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		DocRaid->SetIsForceStartGame(false);
		DocRaid->ClearPartyRetryInfo();
		DocRaid->SetIsRequestPartyRematch(false);
	}
}
