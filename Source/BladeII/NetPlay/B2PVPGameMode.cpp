// Fill out your copyright notice in the Description page of Project Settings.


#include "B2PVPGameMode.h"

#if WITH_EDITOR
#include "Editor.h"
#endif
#include "B2StageManager.h"
#include "BladeIIPlayer.h"
#include "BladeIINetPlayer.h"
#include "Event.h"
#include "Packet.h"
#include "PacketMaker.h"
#include "Serializer.h"
#include "ChannelManager.h"
#include "B2SkillRTManager.h"
#include "BladeIIPlayerController.h"
//#include "B2UIManager.h"
//#include "B2UIPVP1on1BattleMain.h"
//#include "B2UIPVPModFindingMatch.h"
#include "B2UIDocHelper.h"
#include "BladeIIPlayerAIController.h"
////#include "Matinee/MatineeActor.h"
#include "B2CommonSoundInfo.h"
#include "B2ModPlayerInfoData.h"
#include "Retailer.h"
#include "B2PCClassInfoBox.h"
#include "B2CombatStatEvaluator.h"
#include "B2GameLoadingProgress.h"
//#include "B2UICommonInvite.h"
//#include "B2UIPVPResult.h"

#include "FakeConfigure.h"
#include "BladeIIGameImpl.h"
//#include "B2AutoAIController.h"
#include "Engine/DamageEvents.h"
#include "../BladeII/BladeIIUtil.h"

DEFINE_LOG_CATEGORY_STATIC(LogPVPGameMode, Log, All);

#if WITH_EDITOR
bool GbEditorPVPGamePreview = false;
EPCClass GEditorPVPPreviewMyClassMain = EPCClass::EPC_Gladiator;
EPCClass GEditorPVPPreviewMyClassTag = EPCClass::EPC_Assassin;
EPCClass GEditorPVPPreviewRemoteClassMain = EPCClass::EPC_Assassin;
EPCClass GEditorPVPPreviewRemoteClassTag = EPCClass::EPC_Gladiator;
#endif

AB2PVPGameMode::AB2PVPGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRemotePawnClass	= ABladeIINetPlayer::StaticClass();	
	//PVPAIController			= nullptr;

	bDeferAutoBeginAuto = true; // TryAutoBeginAutoAtBeginning 妮阑 BeginPlay 俊 窍瘤 臼绊 捞饶俊 流立 妮.

	bRequestResult = false;
}

void AB2PVPGameMode::BeginPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::BeginPlay"));

	PlayerAutoBattleType = GetLocalSavedAutoType();

	Super::BeginPlay();

	PreBuildTagCharMesh(); // PreloadAnyNecessaryInfo 己拜牢单 捞率辑 捞 扁瓷捞 蝶肺 备泅登绢 乐绊 PreloadAnyNecessaryInfo 啊 坷滚扼捞爹 登绢 乐瘤档 臼绊 秦辑 利寸茄 矫痢俊 蝶肺 荐青.

	StageBeginClass<>::GetInstance().Signal(); // 捞霸 泅犁 窍绰 扒 傈捧 UI 甫 咯绰 扒单 Stage 鄂 捞抚捞 芭浇府搁 促弗 捞亥飘肺 官层辑 镜 荐 乐摆促. 酒抗 促弗 捞亥飘啊 鞘夸且 荐档 乐绊.	
}

void AB2PVPGameMode::BeginDestroy()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::BeginDestroy"));

	//if (PVPAIController)
	//{
	//	PVPAIController->Destroy();
	//	PVPAIController = nullptr;
	//}

	Super::BeginDestroy();
}

void AB2PVPGameMode::Destroyed()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::Destroyed"));

	//if (PVPAIController)
	//{
	//	PVPAIController->Destroy();
	//	PVPAIController = nullptr;
	//}

	Super::Destroyed();
}

void AB2PVPGameMode::HandleMatchHasStarted()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleMatchHasStarted"));
//
//	BII_CHECK(GetLocalController());
//	auto MyPawn = Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn());
//	BII_CHECK(MyPawn);
//	
//	if (GetPeerType() == PEER_RESTRICTED)
//	{
//		BII_CHECK(GetOpponent());
//		BII_CHECK(GetOpponent()->GetNetId() != 0);
//		PrepareSlaveNetGame(GetOpponent()->GetNetId());
//	}
//	else
//	{
//		BII_CHECK(GetLocalController());
//#if !UE_BUILD_SHIPPING
//		if (NetFakeConfigure::GetInstance().GetNumToPlayRaid() == 1)
//		{
//			PrepareNetGame(0);
//		}
//		else
//#endif
//		{
//			PrepareNetGame(GetOpponent()->GetNetId());
//		}
//	}
//
//	MyPawn->EnableInput(GetLocalController());
//	MyPawn->SetTeamNum(IsLeftLocalPlayer() ? int32(ETeamType::ETT_Red) : int32(ETeamType::ETT_Blue));
//	MyPawn->FloorRingColor = FloorRingColors[IsLeftLocalPlayer() ? int32(ETeamType::ETT_Red) : int32(ETeamType::ETT_Blue)];
//	MyPawn->ChangeFloorRingPS(MyPawn->FloorRingPS);
//
//	// Make sure level streaming is up to date before triggering NotifyMatchStarted
//	GEngine->BlockTillLevelStreamingCompleted(GetWorld());
//
//	BII_CHECK(GetWorldSettings());
//
//	if (MyPawn)
//	{
//		MyPawn->SetActorHiddenInGame(false);
//		MyPawn->SetMovementDisable(false);
//	}
//
//	// First fire BeginPlay, if we haven't already in waiting to start match
//	GetWorldSettings()->NotifyBeginPlay();
//
//	// Then fire off match started
//	GetWorldSettings()->NotifyMatchStarted();
//
//#if BII_STATS // PerfStat 篮 MatchStart 矫痢何磐 葛栏扁 矫累.
//	PerfStatCollector.OnBegin(this);
//#endif

}

void AB2PVPGameMode::MatchGameStartEventSceneEnd()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::MatchGameStartEventSceneEnd"));

	//if (GetPeerType() == PEER_STANDALONE)
	//{
	//	PVPAIController = SpawnAIController(FVector::ZeroVector, FRotator::ZeroRotator);
	//	PVPAIController->Possess(GetOpponent());
	//	PVPAIController->BeginAutoPlay();
	//}

	//TryAutoBeginAutoAtBeginning(); // PVP 绰 PostBeginPlay 矫痢俊 LocalPlayer 啊 绝澜. 咯扁辑 茄锅 歹 秦 林绢具 窃.
}

void AB2PVPGameMode::NotifyInGameReady(uint32 InNetId, const uint8 ReceiverType)
{
	//packet::ByteStream	payload;
	//payload << InNetId;
	//FString encoded_string = packet::FinalizePacket(packet::SYNC_READY_TO_STARTGAME, GetMyNetId(), 0, ReceiverType, payload);
	//data_trader::ChannelManager::GetInstance().SendMessage(GetHostId(), encoded_string);
}

void AB2PVPGameMode::CheckInGameReady(float DeltaSeconds)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::CheckInGameReady"));
	
	//Peer 啊 slave带 master 带 InGameReady甫 焊郴具 促澜栏肺 逞绢啊霸 荐沥
	const float IntervalTime = 5.0f;
	const int32 LimitSendCount = 4;

	InGameReadyTime += DeltaSeconds;

	if (IntervalTime <= InGameReadyTime)
	{
		// ai搁 弊成 矫累
		if (GetPeerType() == PEER_NETSTANDALONE || GetPeerType() == PEER_STANDALONE)
		{
			HandleSyncStartGame();
			return;
		}

		NotifyInGameReady(GetMyNetId(), packet::ALL);
		
		InGameReadyTime = 0.0f;
		++InGameReadySendPacketCount;
	}

	if (InGameReadySendPacketCount >= LimitSendCount)
	{
		ClosePreLoadingScreenClass<EPreLoadingScreenType>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin);
		SetPeerType(PEER_NETSTANDALONE);
		LeaveEnemyBeforeInProgress = true;
		HandleSyncStartGame();
	}
}

bool CheckAllReadyPeer(const TMap<uint32, bool>& CheckMap)
{
	for (auto& Elem : CheckMap)
	{
		if (Elem.Value == false)
			return false;
	}
	return true;
}

void AB2PVPGameMode::HandleSyncReadyToStartGame(uint32 InNetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleSyncReadyToStartGame"));

	//if (GetPeerType() == PEER_SUPER)
	//{
	//	ReadyCheck[InNetId] = true;
	//	if (CheckAllReadyPeer(ReadyCheck))
	//	{
	//		packet::ByteStream	payload;
	//		FString encoded_string = packet::FinalizePacket(packet::SEND_SYNC_STARTGAME, this->GetMyNetId(), 0, packet::ALL, payload);
	//		data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);
	//	}
	//}
}

void AB2PVPGameMode::HandleSyncStartGame()
{
	if(SceneManager.GetState() != EPVPMatchState::Battle)
		SceneManager.SetState(EPVPMatchState::Battle);
}

void AB2PVPGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SceneManager.Tick(DeltaSeconds);

	//RealtimeUpdateUIDoc俊辑 康旷 酒捞能阑 官操绰 何盒捞 乐绰单 弊吧 救官操扁 困秦 捞 临阑 眠啊窃(秦榜 酒捞能捞 昏力 登绰 捞酱啊 乐澜)
//	if (GetMatchState() == NetMatchState::FinishBattle || GetMatchState() == NetMatchState::StartResult) return;

	if (SceneManager.GetState() == EPVPMatchState::PreBattle)
	{
		CheckInGameReady(DeltaSeconds);
	}

	if (LeaveEnemyBeforeInProgress && SceneManager.GetState() == EPVPMatchState::Battle)
	{
		LeaveEnemyInProgress();
		LeaveEnemyBeforeInProgress = false;
	}

	if (GetMatchState() != NetMatchState::ExchangeInfos && GetMatchState() != NetMatchState::MatchOtherPlayer)
		return;

	ErrorHandlingTimeExchangeInfo += DeltaSeconds;
	if (ErrorHandlingTimeExchangeInfo > 10.f)
	{
		//辑滚俊 捞惑捞 乐芭唱
		//惑措 peer啊 唱啊辑 沥焊啊 坷瘤 臼芭唱..
		//郴啊 聚阑 pause矫难躇促带瘤..
		bRepeatRequestMatchByError = true;
#if !UE_BUILD_DEBUG
		LeaveRoomIfJoined();
#endif

		if (IsValidFriendshipGame())
		{
			ReturnToPVPMainMenu();
			ClearInvitationGameInfo(EB2GameMode::PVP_Tag);
		}
		else
		{
			SetMatchState(NetMatchState::RequestMatchMaking);
		}
	}
}

void AB2PVPGameMode::InitializeUpdateUIDoc()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::InitializeUpdateUIDoc"));

	//auto* DocBattleRival = UB2UIDocHelper::GetDocPVP1on1Rival();
	//if (DocBattleRival)
	//{
	//	if (Opponent != nullptr)
	//	{
	//		DocBattleRival->SetCurrPCHealth(Opponent->Health);
	//		DocBattleRival->SetCurrPCMaxHealth(Opponent->MaxHealth);

	//		DocBattleRival->SetIsCurPCMain(Opponent->AmIMainCharacter());
	//		DocBattleRival->SetCurPCLevel(Opponent->GetCurCharacterLevel());
	//		DocBattleRival->SetCurPCClass(PCClassToInt(Opponent->CurrentPlayingClass));
	//		DocBattleRival->SetTagPCClass(PCClassToInt(Opponent->CurrentPlayingClass == Opponent->MainPlayerClass ? Opponent->SubPlayerClass : Opponent->MainPlayerClass));
	//	}
	//}
}

void AB2PVPGameMode::RealtimeUpdateUIDoc()
{
////	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::RealtimeUpdateUIDoc"));
//	//RealtimeUpdateUIDoc俊辑 康旷 酒捞能阑 官操绰 何盒捞 乐绰单 弊吧 救官操扁 困秦 捞 临阑 眠啊窃(秦榜 酒捞能捞 昏力 登绰 捞酱啊 乐澜)
//	if (GetMatchState() == NetMatchState::FinishBattle || GetMatchState() == NetMatchState::StartResult) return;
//
//	Super::RealtimeUpdateUIDoc();
//
//	{
//		auto* DocBattleRival = UB2UIDocHelper::GetDocPVP1on1Rival();
//		if (!DocBattleRival)
//			return;
//
//		if (!GetOpponent())
//			return;
//
//		auto* SkillManager = GetRemoteSkillRTManager(GetOpponent()->GetNetId());
//
//		if (SkillManager)
//		{
//			DocBattleRival->SetTagPCHealth(SkillManager->GetInactiveCharacterHealth());
//			DocBattleRival->SetTagPCMaxHealth(SkillManager->GetInactiveCharacterMaxHealth());
//			if (SkillManager->GetInactiveCharacterHealth() <= 0.f)
//				DocBattleRival->SetIsTagPossible(false);
//		}
//
//		if (Opponent != nullptr)
//		{
//			DocBattleRival->SetCurrPCHealth(Opponent->Health);
//			DocBattleRival->SetCurrPCMaxHealth(Opponent->MaxHealth);
//			DocBattleRival->SetCurrPCShield(Opponent->GetShield());
//			DocBattleRival->SetCurPCLevel(Opponent->GetCurCharacterLevel());
//			DocBattleRival->SetCurPCClass(PCClassToInt(Opponent->CurrentPlayingClass));
//			DocBattleRival->SetTagPCClass(PCClassToInt(Opponent->CurrentPlayingClass == Opponent->MainPlayerClass ? Opponent->SubPlayerClass : Opponent->MainPlayerClass));
//			DocBattleRival->SetCurrPCInCriticalCondition(Opponent->IsInCriticalCondition());
//		}
//	}
}

void AB2PVPGameMode::ReturnToFindAccount()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::ReturnToFindAccount"));
	//	
	//if (Opponent)
	//{
	//	if (RemotePlayerInfoData.Contains(Opponent->GetNetId()))
	//	{
	//		FB2FindAccountInfoPtr RemoteFindAccountInfoPtr = RemotePlayerInfoData[Opponent->GetNetId()].GetAccountInfoPtr();

	//		auto DocSome = UB2UIDocHelper::GetDocSome();

	//		if (DocSome)
	//			DocSome->SetFindUserAccountID(RemoteFindAccountInfoPtr->account_id);

	//		BladeIIGameImpl::GetClientDataStore().SetOtherUserInfo(RemoteFindAccountInfoPtr);

	//		FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
	//			// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
	//			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//			if (UIMgrInst) {
	//				UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//				UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterBattleMain);
	//				UIMgrInst->ArtificialAddUIHistory(EUIScene::PVPOneOnOne);
	//			}
	//			LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_FindAccount);
	//			data_trader::Retailer::GetInstance().RequestGetMatchStatus();
	//		});
	//	}		
	//}

	//OpenBladeIILobbyCommon(this); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
}

void AB2PVPGameMode::RequestRematch()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::RequestRematch"));

	//StartBladeIIPVPGame(Cast<AGameMode>(GetWorld()->GetAuthGameMode()));
}

void AB2PVPGameMode::ReturnToPVPMainMenu()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::ReturnToPVPMainMenu"));

	//if (FLobbySceneManager::IsExistDeferredRegistChangeLobbyScene() == false)
	//{
	//	FLobbySceneManager::DeferredRegistChangeLobbyScene(
	//		[this]()
	//	{
	//		// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
	//		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//		if (UIMgrInst) {
	//			UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//			UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterBattleMain);
	//		}
	//		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ElobbyScene_PVPMain);

	//		if(this->IsValidFriendshipGame() == false)
	//			data_trader::Retailer::GetInstance().RequestGetMatchStatus();
	//	}
	//	);
	//}

	//OpenBladeIILobbyCommon(this); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
}

void AB2PVPGameMode::SendRequestMatchResult()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SendRequestMatchResult"));

	//// 模急傈篮 match搬苞 焊尘 鞘夸绝澜
	//if (IsValidFriendshipGame())
	//	return;

	//// 沥惑利牢 霸烙 场唱绊 搬苞肺 啊瘤臼绊,
	//// 霸烙 吝埃俊 盲澄捞 谗扁芭唱 老矫沥瘤肺 唱艾阑版快俊 搬苞 贸府甫 焊郴扁 困秦 静绰 窃荐.

	//if (GetMatchState() != NetMatchState::StartResult)
	//{
	//	//Todo 唱 唱啊夸~ 菩哦
	//	if (GetPeerType() == PEER_SUPER || GetPeerType() == PEER_RESTRICTED)
	//		LeaveRoomIfJoined();
	//	else if (TokenToPlay.Len())
	//	{
	//		if (!bRequestResult)
	//		{
	//			int32 matchType = GetMatchType();
	//			auto myMainCharater = std::make_shared<b2network::B2RoomCharacter>();
	//			auto myTagCharater = std::make_shared<b2network::B2RoomCharacter>();
	//			auto opponentMainCharater = std::make_shared<b2network::B2RoomCharacter>();
	//			auto opponentTagCharater = std::make_shared<b2network::B2RoomCharacter>();

	//			auto& DataStore = BladeIIGameImpl::GetClientDataStore();
	//			ICharacterDataStore* MyData = &DataStore.GetLocalCharacterData();

	//			/////////////////////////////////////////////////////////////////////////////////////////////////
	//			//For Crash Test
	//			FB2ModPlayerInfo* OpponentData = nullptr;
	//			if (Opponent)
	//				OpponentData = &GetRemotePlayerInfoData(Opponent->GetNetId());
	//			/////////////////////////////////////////////////////////////////////////////////////////////////

	//			if (OpponentData != nullptr && MyData != nullptr)
	//			{
	//				EPCClass MyMain = MyData->GetMainPlayerClass();
	//				EPCClass MySub = MyData->GetSubPlayerClass();
	//				EPCClass OppMain = OpponentData->GetMainPlayerClass();
	//				EPCClass OppSub = OpponentData->GetSubPlayerClass();

	//				myMainCharater->character_level = MyData->GetCharacterLevel(MyMain);
	//				myMainCharater->character_power = CombatStatEval::GetPCCombatPower(MyMain, MyData);
	//				myMainCharater->character_type = CliToSvrPCClassType(MyMain);

	//				myTagCharater->character_level = MyData->GetCharacterLevel(MySub);
	//				myTagCharater->character_power = CombatStatEval::GetPCCombatPower(MySub, MyData);
	//				myTagCharater->character_type = CliToSvrPCClassType(MySub);

	//				opponentMainCharater->character_level = OpponentData->GetCharacterLevel(OppMain);
	//				opponentMainCharater->character_power = CombatStatEval::GetPCCombatPower(OppMain, OpponentData);
	//				opponentMainCharater->character_type = CliToSvrPCClassType(OppMain);

	//				opponentTagCharater->character_level = OpponentData->GetCharacterLevel(OppSub);
	//				opponentTagCharater->character_power = CombatStatEval::GetPCCombatPower(OppSub, OpponentData);
	//				opponentTagCharater->character_type = CliToSvrPCClassType(OppSub);
	//			}

	//			data_trader::Retailer::GetInstance().RequestMatchResult(BladeIIGameImpl::GetClientDataStore().GetAccountId(), TokenToPlay, MatchResult_Lose,
	//				matchType, myMainCharater, myTagCharater, opponentMainCharater, opponentTagCharater, GetRoomId(), true);

	//			bRequestResult = true;
	//		}
	//	}
	//}
}

void AB2PVPGameMode::OpenPopupGotoPVPMainMenu()
{
	FText txtComment = BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, FString::FromInt(6516));

	//UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//	BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
	//	txtComment,
	//	0.f,
	//	true,
	//	true,
	//	EUIMsgPopupButtonGroup::Confirm,
	//	FMsgPopupOnClick::CreateLambda([this]() {
	//	SendRequestMatchResult();
	//	ReturnToPVPMainMenu();
	//})
	//);
}

void AB2PVPGameMode::EnterShop(int32 nWhereShop)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::EnterShop"));

	//FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
	//	// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促.
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst) {
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterBattleMain);
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::PVPOneOnOne);
	//	}
	//});

	//switch (static_cast<EStorePageWhere>(nWhereShop))
	//{
	//case EStorePageWhere::Store:
	//	data_trader::Retailer::GetInstance().RequestGetGeneralShop();
	//	break;
	//case EStorePageWhere::EquipStore:
	//	data_trader::Retailer::GetInstance().RequestGetLotteryShop();
	//	break;
	//case EStorePageWhere::PackageStore:
	//	data_trader::Retailer::GetInstance().RequestGetPackageShop();
	//	break;
	//case EStorePageWhere::MagicStore:
	//	data_trader::Retailer::GetInstance().RequestGetMagicShop();
	//	break;

	//}
	//OpenBladeIILobbyCommon(this); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
}

void AB2PVPGameMode::PreBeginPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::PreBeginPlay"));

	//auto* GameRule = new PVPGameRule;
	//SetGameRule(GameRule);

	//Super::PreBeginPlay();

	//PlayPVPBGM(EPVPBGMPlayContext::PVPBGMCTX_Standby); // 贸澜俊绰 措扁 惑怕老 巴.. PVP 葛靛 BeginPlay 阂府绰 鸥捞怪捞 粱 词栏骨肺 咯扁辑 矫累.

	//SetStartTimeCount(false);
	//bIsTutorial = false;
	//HostMatchState = EPVPMatchState::NONE;
	//TutorialDebuffValue = 0.0f;
	//InGameReadyTime = 2.5f;
	//InGameReadySendPacketCount = 0;
	//LeaveEnemyBeforeInProgress = false;
}

void AB2PVPGameMode::SetDesiredPCClassEnum()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetDesiredPCClassEnum"));
	
	Super::SetDesiredPCClassEnum();
}

void AB2PVPGameMode::SubscribeEvents()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SubscribeEvents"));

	//if (bEventsSubscribed)
	//	return;

	//Super::SubscribeEvents();

	//// ReceivedOtherNetIdClass狼 版快 何葛巴阑 荤侩窍瘤 臼绊 蝶肺 敬促.
	//Issues.Add(ReceivedOtherNetIdClass<uint32, int64, const FB2FindAccountInfoPtr&>::GetInstance().Subscribe2(
	//	[this](uint32 NetId, int64 AccountId, const FB2FindAccountInfoPtr& AccountInfo)
	//	{
	//		UE_LOG(LogPVPGameMode, Log, TEXT("ReceivedOtherNetId Event Invoked..."));
	//		packet::ByteStream	payload;
	//		payload << this->GetMyNetId();
	//		FString encoded_string = packet::FinalizePacket(packet::BEGIN_HANDSHAKING, this->GetMyNetId(), 0, packet::ALLBUTME, payload);
	//		data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);

	//		this->SetRemotePlayerInfoData(NetId, AccountId, AccountInfo);
	//		this->SetMatchState(NetMatchState::ExchangeInfos);
	//		UE_LOG(LogPVPGameMode, Log, TEXT("Leave - ReceivedOtherNetId Event..."));
	//	}
	//));
	//Issues.Add(ReceivedExchangeInfomationsClass<uint32, EPCClass, EPCClass, uint32, uint32, uint32, const FGuid&, int64>::GetInstance().Subscribe2(
	//	[this](uint32 NetId, EPCClass Main, EPCClass Tag, uint32 Grade, uint32 Star, uint32 ContinuousWins, const FGuid& Token, const int64& AccountId)
	//	{
	//		this->SetOpponentToken(Token);
	//		this->SetOpponentAccountId(AccountId);

	//		auto* DocRival = UB2UIDocHelper::GetDocPVP1on1Rival();
	//		if (DocRival)
	//		{
	//			DocRival->SetUserAccountID(AccountId);
	//			DocRival->SetTagMatchGrade(Grade);
	//			DocRival->SetTagMatchGradeStar(Star);
	//			DocRival->SetTagMatchCurrContinuousWins(ContinuousWins);
	//		}

	//		this->GetRemotePlayerInfoData(NetId).SetMainPlayerClass(Main);
	//		this->GetRemotePlayerInfoData(NetId).SetSubPlayerClass(Tag);
	//		this->SpawnRemotePlayer(NetId, Main, Tag);

	//		packet::ByteStream	payload;
	//		FString encoded_string = packet::FinalizePacket(packet::ACK_EXCHANGE_INFORMATIONS, this->GetMyNetId(), 0, packet::ALLBUTME, payload);
	//		data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);
	//	}
	//));
	//Issues.Add(ReceivedStandalonePlayClass<FB2TagMatchMakingPtr>::GetInstance().Subscribe2(
	//	[this](FB2TagMatchMakingPtr MatchInfo)
	//	{
	//		UE_LOG(LogPVPGameMode, Log, TEXT("AI Match"));
	//		this->PlayMatchWithAI(MatchInfo);
	//	}
	//));
	//Issues.Add(ReceivedAckExchangeInformationsClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->SetMatchState(NetMatchState::DoneToExchange);
	//	}
	//));
	//Issues.Add(ReturnToFindAccountClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->ReturnToFindAccount();
	//	}
	//));
	//Issues.Add(ReturnToPVPMainMenuClass<bool>::GetInstance().Subscribe2(
	//	[this](bool bSendMatchResult)
	//	{
	//		if (bSendMatchResult)
	//			this->SendRequestMatchResult();

	//		this->ReturnToPVPMainMenu();
	//	}
	//));
	//Issues.Add(MatchGameStartEventSceneEndClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->MatchGameStartEventSceneEnd();
	//	}
	//));
	//Issues.Add(HandleServerError901Class<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->HandleRequestMatchMaking();
	//	}
	//));
	//Issues.Add(HandleServerError6516Class<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->OpenPopupGotoPVPMainMenu();
	//	}
	//));
	//Issues.Add(DeliveryTagMatchResultClass<FB2TagMatchResult>::GetInstance().Subscribe2(
	//	[this](FB2TagMatchResult Result)
	//	{
	//		this->HandleResponseMatchResult(Result);
	//	}
	//));
	//Issues.Add(ReturnToPVPRematchClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->RequestRematch();
	//	}
	//));
	//Issues.Add(PlayerTaggedClass<ABladeIIPlayer*, ABladeIIPlayer*>::GetInstance().Subscribe2(
	//	[this](ABladeIIPlayer* Old, ABladeIIPlayer* New)
	//	{
	//		this->SetupNetwork(Old, New);
	//	}
	//));
	//Issues.Add(TagPlayerSpawnedClass<ABladeIIPlayer*>::GetInstance().Subscribe2(
	//	[this](ABladeIIPlayer* NewPlayer)
	//	{
	//		this->SetNetStatus(NewPlayer);
	//	}
	//));
	//Issues.Add(DeliveryMatchMakingClass<FB2TagMatchMakingPtr>::GetInstance().Subscribe2(
	//	[this](FB2TagMatchMakingPtr MatchInfo)
	//	{
	//		this->DeliveryMatchMaking(MatchInfo);
	//	}
	//));
	//Issues.Add(DeliveryCancelMatchmakingClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->HandleResponseCancelMatch();
	//	}
	//));
	//Issues.Add(ReceivedSyncReadyToStartGameClass<uint32>::GetInstance().Subscribe2(
	//	[this](uint32 NetId)
	//	{
	//		this->HandleSyncReadyToStartGame(NetId);
	//	}
	//));
	//Issues.Add(ReceivedSyncStartGameClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->HandleSyncStartGame();
	//}
	//));
	//Issues.Add(DeliveryGetMatchStatusClass<FB2TagMatchStatus>::GetInstance().Subscribe2(
	//	[this](const FB2TagMatchStatus& TagMatchStatus)
	//	{
	//		this->ResponseGetMatchStatus(TagMatchStatus);
	//	}
	//));

	//Issues.Add(StageEventDirectorFinishShowEventClass<int32>::GetInstance().Subscribe2(
	//	[this](int32 EventType)
	//{
	//	this->NotifyStageEventDirector_FinishShow(EventType);
	//}
	//));
	///*
	//Issues.Add(ReceivedRoomLeaveResponseClass<>::GetInstance().Subscribe2(
	//	[this]
	//{
	//	this->ReturnToPVPMainMenu();
	//}
	//));
	//*/

	//bEventsSubscribed = true;
}

void AB2PVPGameMode::SetRoomParticipant(const TArray<FB2RoomParticipantPtr>& Participants)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetRoomParticipant"));

	BII_CHECK(Participants.Num() <= 1);

	if (Participants.Num() > 0)
	{
		if (GetMyNetId() != Participants[0]->id)
		{
			SetRemotePlayerInfoData(Participants[0]->id, Participants[0]->account_id, Participants[0]->account_info);
		}
	}
}

ERoomCloseReasonType AB2PVPGameMode::GetStartRoomCloseReason()
{
	return ERoomCloseReasonType::START_PVP;
}

void AB2PVPGameMode::OnHostChanged(int32 RoomType, int32 NewHostId)
{
	//PVP俊辑绰 Peer甫 Super肺 函版窍搁 救登绰单 Parents啊 弊 开且阑 窍绊 乐扁 锭巩俊
	//厚绢乐绰 皋家靛甫 父惦.
}

bool AB2PVPGameMode::CanTakeDamage()
{
	return SceneManager.GetState() == EPVPMatchState::Battle;
}

void AB2PVPGameMode::ForceTagWhenPartnerDie(AActor* actor)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::ForceTagWhenPartnerDie"));

	//auto*	Player = Cast<ABladeIINetPlayer>(actor);
	//BII_CHECK(Player);

	//if (Player->GetNetStatus() == ABladeIINetPlayer::NET_MASTER)
	//{
	//	ABladeIIPlayerController*	B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//	ABladeIIPlayer*				B2Player = Cast<ABladeIIPlayer>(B2PC->GetPawn());

	//	if (B2Player)
	//	{
	//		auto* SkillRTManager = B2Player->GetSkillRTManager();
	//		SkillRTManager->TagToOther((int32)ETagAttackMode::EAM_DownAttack, EAttackState::ECS_None);
	//	}
	//}
	//else if (Player->GetNetStatus() == ABladeIINetPlayer::NET_SLAVE)
	//{
	//	auto* Controller = Cast<ABladeIIPlayerAIController>(Player->GetController());

	//	if (Controller)
	//	{
	//		auto SkillRTManager = GetRemoteSkillRTManager(Player->GetNetId());
	//		SkillRTManager->ForceSetSkillCoolTimerWhenAIPlayerTagging(static_cast<int32>(ETagAttackMode::EAM_DownAttack), EAttackState::ECS_None);
	//	}

	//	RemotePlayerTagToOther(Player, static_cast<int32>(ETagAttackMode::EAM_DownAttack));
	//}
}

bool AB2PVPGameMode::TagPlayerCharacter(AActor* actor)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::TagPlayerCharacter"));

	bool	Ret = false;
	//auto*	Player = Cast<ABladeIINetPlayer>(actor);
	//BII_CHECK(Player);

	//if (Player->GetNetStatus() == ABladeIINetPlayer::NET_MASTER)
	//{
	//	ABladeIIPlayerController*	B2PC			= Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//	ABladeIIPlayer*				B2Player		= Cast<ABladeIIPlayer>(B2PC->GetPawn());

	//	if (B2Player)
	//	{
	//		auto* SkillRTManager = B2Player->GetSkillRTManager();
	//		
	//		Ret = SkillRTManager ? SkillRTManager->TagToOther(
	//			(int32)B2Player->TagAttackMode, 
	//			B2Player->TagAttackMode == ETagAttackMode::EAM_Disable ? EAttackState(EAttackState::ECS_None) : B2Player->GetAttackState()) : false;

	//		B2Player->SetTeamNum(int32(ETeamType::ETT_Red));
	//	}
	//}
	//else if (Player->GetNetStatus() == ABladeIINetPlayer::NET_SLAVE)
	//{
	//	auto* Controller = Cast<ABladeIIPlayerAIController>(Player->GetController());

	//	if (Controller)
	//	{
	//		GetRemoteSkillRTManager(Player->GetNetId())->ForceSetSkillCoolTimerWhenAIPlayerTagging(
	//			static_cast<int32>(Player->TagAttackMode),
	//			Player->TagAttackMode == ETagAttackMode::EAM_Disable ? EAttackState(EAttackState::ECS_None) : Player->GetAttackState()
	//		);

	//		Player->SetTeamNum(int32(ETeamType::ETT_Blue));
	//	}

	//	Ret = RemotePlayerTagToOther(
	//		Player,
	//		static_cast<int32>(Player->TagAttackMode));
	//}

	return Ret;
}

void AB2PVPGameMode::SetMatchResult(int32 MyResult)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetMatchResult"));

	BII_CHECK(GetPeerType() != PEER_RESTRICTED);

	//SetMatchState(NetMatchState::StartResult);

	////EnablePlayerFloorRing(false);

	//if (GetPeerType() != PEER_RESTRICTED)
	//{
	//	HostMatchState = SceneManager.GetState();
	//}

	//if (GetPeerType() == PEER_SUPER)//if (GetPeerType() != PEER_RESTRICTED)
	//{
	//	ENetMatchResult battleResult(ENetMatchResult::LocalWin);

	//	packet::ByteStream	payload;
	//	payload << static_cast<uint8>(MyResult);
	//	payload << static_cast<uint8>(HostMatchState);
	//	FString encoded_string = packet::FinalizePacket(packet::NOTIFYRESULT, GetMyNetId(), 0, packet::ALLBUTME, payload);
	//	data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);
	//}

	//HandleNotifyResult(SvrToCliNetMatchResult(MyResult), static_cast<uint8>(HostMatchState));
}

void AB2PVPGameMode::StartPVPGame()
{
	StartEventScene_GameBegin();

	PlayPVPBGM(EPVPBGMPlayContext::PVPBGMCTX_Combat, true);

	InitializeUpdateUIDoc();
}

ENetMatchResult AB2PVPGameMode::GetMatchResult() const
{
	//*
	//[@AKI, 170322] 弊成 官肺 逞败档 瞪巴 鞍酒辑 促矫 荐沥窃
	UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome)
		return DocSome->GetLocalPlayerMatchResult();

	return ENetMatchResult::LocalLose;
	/*/
	//[@AKI, 170322] 盔贰 内靛 扁馆栏肺 累己茄 内靛. 困俊 return DocSome->GetLocalPlayerMatchResult(); 内靛肺 利侩 秦档 巩力啊 绝绢 焊咯辑 Legacy肺 巢败 初澜
	//趣矫 困俊 内靛啊 巩力啊 乐促搁 酒贰 内靛甫 荤侩 窍搁 凳.
	UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome)
	{
		ENetMatchResult battleResult(ENetMatchResult::LocalWin);
		if (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin)
			battleResult = ENetMatchResult::LocalWin;
		else
			battleResult = (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalLose) ? ENetMatchResult::LocalLose : ENetMatchResult::Draw;
	}
	return battleResult;
	//*/
}

void AB2PVPGameMode::NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector)
{
	//if (EndedDirector && EndedDirector->GetDirectingEvent() == EStageEvent::ESEV_Begin && EndedDirector->GetMyRelevantUIName() == UIFName::PVPBegin)
	//{
	//	if (GetLocalPlayer())
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("AB2PVPGameMode::NotifyStageEventSceneEnd"));
	//		GetLocalPlayer()->ForceSendAnimStateIndex();
	//	}
	//}
}

void AB2PVPGameMode::SettingResultInfo()
{
	//FB2TagMatchResult Result = ResultInfo;

	//UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	//UB2UIDocPVP1on1Rival* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();

	//if (DocSome == nullptr)
	//	return;

	//if (DocPVP == nullptr)
	//	return;

	//auto RewardBoxStatus = GET_TUPLE_DATA(FB2ResponseTagMatchResult::reward_box_status_index, Result);

	//// 冠胶 搬苞技泼窍扁傈俊. 盔贰波尔 厚背
	//if (DocPVP->GetRewardBoxStatus()
	//	&& !DocPVP->GetRewardBoxStatus()->is_reward_state
	//	&& RewardBoxStatus && RewardBoxStatus->is_reward_state)
	//{
	//	DocPVP->SetCanViewModRewardBoxConfirm(true);
	//	DocPVP->SetIsInviteeReady(false);
	//	DocPVP->SetIsInviterReady(false);
	//	DocPVP->SetIsInvitee(false);
	//}

	//// 模急傈俊辑绰 搬苞 盎脚窍瘤 臼澜
	//if (IsValidFriendshipGame() == false)
	//{
	//	DocPVP->SetRewardBoxStatus(RewardBoxStatus);

	//	auto MyTagMatchStatusPtr = GET_TUPLE_DATA(FB2ResponseTagMatchResult::status_index, Result);
	//	auto MyRanking = GET_TUPLE_DATA(FB2ResponseTagMatchResult::ranking_index, Result);

	//	if (MyTagMatchStatusPtr.get() && MyRanking.get())
	//	{
	//		auto LowRankerSvrPtr = GET_TUPLE_DATA(FB2ResponseTagMatchResult::prev_ranker_index, Result);
	//		auto* BattleDoc = UB2UIDocHelper::GetDocBattle();

	//		if (BattleDoc)
	//		{
	//			BattleDoc->SetTagMatchGrade(MyTagMatchStatusPtr->battle_grade);
	//			BattleDoc->SetTagMatchGradeStar(MyTagMatchStatusPtr->battle_star_count);
	//			BattleDoc->SetTagMatchPrevContinuousWins(BattleDoc->GetTagMatchPrevContinuousWins());
	//			BattleDoc->SetTagMatchCurrContinuousWins(MyTagMatchStatusPtr->continue_win);
	//			BattleDoc->SetTagMatchPrevTopContinuousWins(BattleDoc->GetTagMatchCurrTopContinuousWins());
	//			BattleDoc->SetTagMatchCurrTopContinuousWins(MyTagMatchStatusPtr->continue_win_max);

	//			//Todo 珐欧 函悼
	//			DocSome->SetRankingChanged((BattleDoc->GetTagMatchRank() != 0 ? BattleDoc->GetTagMatchRank() : MyRanking->total_ranker_count) - MyRanking->ranking);

	//			BattleDoc->SetTagMatchRankPercentage(MyRanking->total_ranker_count > 0 ? 100.f * MyRanking->ranking / (float)MyRanking->total_ranker_count : 100);
	//			BattleDoc->SetTagMatchRank(MyRanking->ranking);

	//			FModRankerInfo MyRankerInfo;
	//			MyRankerInfo.Grade = BattleDoc->GetTagMatchGrade();
	//			MyRankerInfo.GradeStar = BattleDoc->GetTagMatchGradeStar();
	//			MyRankerInfo.Rank = MyRanking->ranking;
	//			MyRankerInfo.UserName = BladeIIGameImpl::GetClientDataStore().GetUserNickName();

	//			MyRankerInfo.Entry.Add(IntToPCClass(BattleDoc->GetCurPCClass()));
	//			MyRankerInfo.Entry.Add(IntToPCClass(BattleDoc->GetTagPCClass()));

	//			auto* MainCharDoc = UB2UIDocHelper::GetDocHero(BattleDoc->GetCurPCClass());
	//			auto* TagCharDoc = UB2UIDocHelper::GetDocHero(BattleDoc->GetTagPCClass());
	//			if (MainCharDoc && TagCharDoc)
	//			{
	//				MyRankerInfo.TotalCombatPower = MainCharDoc->GetCombatPower() + TagCharDoc->GetCombatPower();

	//				//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
	//				//MyRankerInfo.EntryLevels.Add(MainCharDoc->GetCurrentLevel());
	//				//MyRankerInfo.EntryLevels.Add(TagCharDoc->GetCurrentLevel());
	//				FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	//				MyRankerInfo.EntryLevels.Add(CharacterDataStore.GetCharacterLevel(MainCharDoc->GetHeroClass()));
	//				MyRankerInfo.EntryLevels.Add(CharacterDataStore.GetCharacterLevel(TagCharDoc->GetHeroClass()));
	//			}

	//			DocSome->SetMyRankerInfo(MyRankerInfo);

	//			if (LowRankerSvrPtr.get() && LowRankerSvrPtr->account_id > -1)
	//			{
	//				FModRankerInfo OtherRankerInfo;
	//				OtherRankerInfo.Grade = LowRankerSvrPtr->battle_grade;
	//				OtherRankerInfo.GradeStar = LowRankerSvrPtr->battle_star_count;
	//				OtherRankerInfo.Rank = MyRanking->ranking + 1;
	//				OtherRankerInfo.UserName = LowRankerSvrPtr->nickname;

	//				for (auto& EntryChar : LowRankerSvrPtr->entries)
	//				{
	//					OtherRankerInfo.Entry.Add(SvrToCliPCClassType(EntryChar.character_type));
	//					OtherRankerInfo.EntryLevels.Add(EntryChar.level);
	//				}

	//				OtherRankerInfo.TotalCombatPower = LowRankerSvrPtr->total_power;		// jwyim power

	//				DocSome->SetOtherRankerInfo(OtherRankerInfo);
	//			}
	//		}
	//	}

	//	auto MyRewards = GET_TUPLE_DATA(FB2ResponseTagMatchResult::rewards_index, Result);
	//	for (auto Reward : MyRewards)
	//	{
	//		BladeIIGameImpl::GetClientDataStore().ApplyRewardInfo(Reward);
	//	}
	//	DocSome->SetRewards(MyRewards);
	//}

	//auto* AIController = Cast<ABladeIIPlayerAIController>(Opponent->GetController());
	//if (AIController)
	//	AIController->StopAutoPlay();
	//
	//StartEventScene_MatchResult(DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin); // Match 辆丰俊 蝶弗 楷免阑 剁款促. GoToVillage 殿篮 搬苞 楷免 UI 滚瓢俊 
}

void AB2PVPGameMode::ResponseGetMatchStatus(const FB2TagMatchStatus& TagMatchStatus)
{
	/*UB2UIDocBattle*			BattleDoc = UB2UIDocHelper::GetDocBattle();
	UB2UIDocPVP1on1Rival*	PvPDoc = UB2UIDocHelper::GetDocPVP1on1Rival();

	if (!PvPDoc || !BattleDoc)
		return;

	auto RewardBoxStatus = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::reward_box_status_index, TagMatchStatus);

	PvPDoc->SetRewardBoxStatus(RewardBoxStatus);

	auto MyRanking = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::ranking_index, TagMatchStatus);
	BattleDoc->SetTagMatchRankPercentage(MyRanking->total_ranker_count > 0 ? 100.f * MyRanking->ranking / (float)MyRanking->total_ranker_count : 100);
	BattleDoc->SetTagMatchRank(MyRanking->ranking);

	auto MatchStatus = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::match_status_index, TagMatchStatus);

	BattleDoc->SetTagMatchGrade(MatchStatus->battle_grade);
	BattleDoc->SetTagMatchBeforeGrade(MatchStatus->battle_grade);
	BattleDoc->SetTagMatchGradeStar(MatchStatus->battle_star_count);
	BattleDoc->SetTagMatchBeforeGradeStar(MatchStatus->battle_star_count);
	BattleDoc->SetTagMatchCurrContinuousWins(MatchStatus->continue_win);
	BattleDoc->SetTagMatchCurrTopContinuousWins(MatchStatus->continue_win_max);

	auto RankingReward = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::ranking_reward_index, TagMatchStatus);
	auto HallofFame = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::hall_of_fames_index, TagMatchStatus);

	if (RankingReward)
	{
		PvPDoc->SetIsTagReward(true);
		PvPDoc->SetTagReward(RankingReward);
	}
	else
		PvPDoc->SetIsTagReward(false);

	auto RankingList = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::rankers_index, TagMatchStatus);
	auto BeginRank = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::from_ranking_index, TagMatchStatus);

	PvPDoc->RankerInfoContainer.SetRankerList(RankingList, BeginRank);

	auto RewardRemainTime = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::settlement_time_index, TagMatchStatus);
	PvPDoc->SetTagRewardTime(RewardRemainTime);

	auto TodayReward = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::today_reward_index, TagMatchStatus);
	PvPDoc->SetTagTodayReward(TodayReward);

	auto LastReward = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::last_reward_index, TagMatchStatus);
	PvPDoc->SetTagLastReward(LastReward);

	auto ParticipationCount = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::today_participation_count_index, TagMatchStatus);
	PvPDoc->SetTagParticipationCount(ParticipationCount);

	auto CurrentMatchPoint = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::match_point_index, TagMatchStatus);
	PvPDoc->SetMatchPoint(CurrentMatchPoint);

	auto CurrentAdditionalMatchCount = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::additional_match_count_index, TagMatchStatus);
	PvPDoc->SetAdditionalMatchCount(CurrentAdditionalMatchCount);

	auto NextChargeSecond = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::match_point_next_charge_second_index, TagMatchStatus);
	PvPDoc->SetNextChargeSecond(NextChargeSecond);

	auto CurrentGold = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::current_gold_index, TagMatchStatus);
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, CurrentGold);

	auto CurrentGem = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::current_gem_index, TagMatchStatus);
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, CurrentGem);*/
}

void AB2PVPGameMode::HandleResponseMatchResult(FB2TagMatchResult Result)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleResponseMatchResult"));
	
	ResultInfo = Result;

	SceneManager.ShowResult();
}

void AB2PVPGameMode::HandleResponseCancelMatch()
{
	ReturnToPVPMainMenu();
}

void AB2PVPGameMode::NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadGuy)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::NotifyPlayerDeadEnd"));
}

void AB2PVPGameMode::NotifyCharacterDead(ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::NotifyCharacterDead"));

	//Super::NotifyCharacterDead(TargetCharacter, PawnInstigator, DamageCauser);

	//if (GetMatchState() == NetMatchState::LeaveEnemyInBattle)
	//	return;

	//auto* Victim = Cast<ABladeIINetPlayer>(TargetCharacter);
	//BII_CHECK(Victim);

	//auto* SkillRTManager = Victim->GetSkillRTManager();
	//BII_CHECK(SkillRTManager);

	//if (SkillRTManager && SkillRTManager->IsTagPossibleAtDeadMoment())
	//{
	//	this->ForceTagWhenPartnerDie(Victim);
	//	return;
	//}

	//ABladeIICharacter* Inflictor;

	//// 捧荤眉老荐档 乐绢辑 笛促眉农
	//if (PawnInstigator)
	//	Inflictor = Cast<ABladeIICharacter>(PawnInstigator);
	//else
	//	Inflictor = Cast<ABladeIICharacter>(DamageCauser);

	////捧荤眉老锭绰 弊 Owner甫 啊廉咳
	//if (DamageCauser && !Inflictor)
	//	Inflictor = Cast<ABladeIICharacter>(DamageCauser->GetOwner());

	////付公府 傍拜(Slow Motion)
	//if (Inflictor)
	//	StartEventScene_FinishAttack(Inflictor);

	//// SetMatchResult俊辑 秦拎辑 咯扁俊 捞吧 持栏搁 救登绰单....
	//// 秦榜 钎矫甫 付公府 傍拜窍扁 傈何磐 焊咯林妨搁...捞犯霸 持绢拎具 窃....
	//UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	//if (DocSome)
	//{
	//	auto MyPlayer = Cast<ABladeIIPlayer>(GetLocalController()->GetPawn());
	//	BII_CHECK(MyPlayer);
	//	DocSome->SetLocalPlayerMatchResult(SvrToCliNetMatchResult(TargetCharacter == MyPlayer ? MatchResult_Lose : MatchResult_Win));
	//}

	//SetMatchState(NetMatchState::FinishBattle);
}

void AB2PVPGameMode::EndMatchByDie()
{
	ENetMatchResult result(ENetMatchResult::LocalLose);

	if (UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome())
	{
		result = DocSome->GetLocalPlayerMatchResult();
	}

	if (GetPeerType() == PEER_RESTRICTED)
		return;

	SetMatchResult(static_cast<int32>(result));
}

void AB2PVPGameMode::SetStartTimeCount(bool InStartCount)
{
	SceneManager.SetStartTimeCount(InStartCount);

	if (InStartCount == true)
	{
		if (GetLocalPlayer())
		{
			GetLocalPlayer()->SetUnityTick(true);
		}

		if (Opponent)
		{
			Opponent->SetUnityTick(true);
		}
	}
}

void AB2PVPGameMode::StopPlayers()
{
	//auto* LocalB2Player = Cast<ABladeIINetPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));

	//auto* NetMyPlayer = Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn());
	//auto* NetOppent = Cast<ABladeIINetPlayer>(GetOpponent());

	//BII_CHECK(LocalB2Player);
	//BII_CHECK(NetMyPlayer);
	//BII_CHECK(NetOppent);

	//// StopPlayers 俊 立辟窍绰 版快绰 惑措狼 楷搬捞 谗绢柳 版快, 鸥烙坷滚肺 PVP啊 碍力肺 辆丰等 版快啊 乐栏哥,
	//// 秦寸 惑怕俊急 付瘤阜 坷配 鸥涝阑 肺拿俊 历厘 饶 烙矫肺 坷配 敲饭捞 悼累阑 沥瘤矫难 促澜 版扁俊 坷配 鸥涝捞 蜡瘤登霸 茄促. 
	//SaveLocalSavedAutoType();

	//if (LocalB2Player->IsInAutoPlay())
	//{
	//	LocalB2Player->StopAutoPlay();
	//}
	//	
	//NetMyPlayer->StopAutoPlay();
	//NetMyPlayer->DisableInput(GetLocalController());
	//NetMyPlayer->SetForceComboEnd();
	//NetMyPlayer->SeEnableTakeDamageForNet(false);
	//NetMyPlayer->StopRepeatingInput(true);

	//NetOppent->SeEnableTakeDamageForNet(false);
 //	NetOppent->SetForceComboEnd();
	//NetOppent->StopAutoPlay();
	//NetOppent->StopRepeatingInput(true);
	//
	//if (NetMyPlayer->GetAutoAIController())
	//{
	//	NetMyPlayer->GetAutoAIController()->StopAutoPlay();
	//}

	//if (NetOppent->GetAutoAIController())
	//{
	//	NetOppent->GetAutoAIController()->StopAutoPlay();
	//}
}

int32 AB2PVPGameMode::GetMatchType()
{
	auto matchType = b2network::B2MatchType::AI;

	auto* BattleDoc = UB2UIDocHelper::GetDocBattle();
	if (BattleDoc)
		matchType = BattleDoc->GetIsAIMe() ? b2network::B2MatchType::AI : b2network::B2MatchType::PLAYER;
	
	return static_cast<int32>(matchType);
}

void AB2PVPGameMode::EndMatchByBattleTime()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::EndMatchByBattleTime"));
//
//#if !UE_BUILD_SHIPPING
//	if (NetFakeConfigure::GetInstance().GetSkipEndGameByTime())
//		return;
//#endif
//
//	StopPlayers();
//
//	if (MatchState != MatchState::InProgress)
//		return;
//
//	if (GetPeerType() == PEER_RESTRICTED)
//		return;
//	
//	BII_CHECK(GetLocalController());
//
//	auto MyPlayer = Cast<ABladeIIPlayer>(GetLocalController()->GetPawn());
//	BII_CHECK(MyPlayer);
//
//	auto MySRTManager = MyPlayer->GetSkillRTManager();
//	BII_CHECK(MySRTManager);
//
//	auto OpponentSRTManager = GetRemoteSkillRTManager(Opponent->GetNetId());
//	BII_CHECK(OpponentSRTManager);
//
//	float MyTotalHPRatio	   = (MyPlayer->Health / MyPlayer->MaxHealth) + (MySRTManager->GetInactiveCharacterHealth() / MySRTManager->GetInactiveCharacterMaxHealth());
//	float OpponentTotalHPRatio = (Opponent->Health / Opponent->MaxHealth) + (OpponentSRTManager->GetInactiveCharacterHealth() / OpponentSRTManager->GetInactiveCharacterMaxHealth());
//	
//	if (FMath::IsNearlyEqual(MyTotalHPRatio, OpponentTotalHPRatio))
//	{
//		SetMatchResult(MatchResult_Draw);
//	}
//	else
//	{
//		if (MyTotalHPRatio > OpponentTotalHPRatio)
//		{
//			SetMatchResult(MatchResult_Win);
//		}
//		else
//		{
//			SetMatchResult(MatchResult_Lose);
//		}
//	}
}

void AB2PVPGameMode::SetupUIScene()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetupUIScene"));

	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::PVPOneOnOneFindMatch);
}

bool AB2PVPGameMode::IsGlobalTimeDilationEnabled(const AActor* RelativeActor /*= NULL*/)
{
	// World Matinee 搁 Timedilation 利侩
	if (RelativeActor && Cast<AB2StageEventDirector>(RelativeActor))
		return true;

	return Super::IsGlobalTimeDilationEnabled(RelativeActor);
}

void AB2PVPGameMode::SetMatchState(FName NewState)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetMatchState"));

	Super::SetMatchState(NewState);

#if WITH_EDITOR
	if (GIsEditor && GbEditorPVPGamePreview && 
		(MatchState == MatchState::WaitingToStart || MatchState == NetMatchState::RequestMatchMaking))
	{
		HandleEditorPVPPreviewSetup();
	}
#endif
}

//PVP率俊辑 AutoAIController甫 肺爹窍绰 泅惑捞 乐绢 啊惑皋家靛肺 倒妨辑 B2PVPGameMode俊辑 坷滚扼捞靛 窃
void AB2PVPGameMode::SetPlayerAutoType(int32 InAuto) // 老矫利牢 On/Off 啊 酒囱 泅 霸烙葛靛俊辑 蜡瘤登绰 auto 惑怕.
{
	//if (PlayerAutoBattleType != InAuto)
	//{
	//	PlayerAutoBattleType = InAuto;
	//	SaveLocalSavedAutoType();
	//}

	//// 捞吧 妮 窍绰 惑炔俊辑绰 楷免 殿俊 狼秦 烙矫利栏肺 Auto 啊 波柳促芭唱 窍绰 扒 公矫茄促. 弊巴鳖瘤 积阿窍妨搁 粱 汗棱窍绊 弊凡 鞘夸鳖瘤绰 绝阑 淀窃.
	//auto* LocalB2Player = Cast<ABladeIINetPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//auto* DocBattle		= UB2UIDocHelper::GetDocBattle();

	//BII_CHECK(LocalB2Player);

	//if (DocBattle)
	//{
	//	DocBattle->SetInAutoPlay(PlayerAutoBattleType);
	//}

	//if (PlayerAutoBattleType == AutoBattleType::Attack || PlayerAutoBattleType == AutoBattleType::Skill)
	//{
	//	LocalB2Player->StartAutoPlay();
	//}
	//else
	//{
	//	if(LocalB2Player->IsInAutoPlay())
	//		LocalB2Player->StopAutoPlay();
	//}
}

TArray<EPCClass> AB2PVPGameMode::GetPCClassesToPreLoad()
{
	TArray<EPCClass> RetArray;
	// 角力 唱客 惑措规狼 急琶茄 努贰胶父 肺爹窍档废 窍绰 霸 亮促. 弥措 4 努贰胶啊 唱棵 荐 乐瘤父 利绢档 泅犁 急琶俊 蝶扼 肺爹窍霸 窍搁 乞闭利栏肺 矫埃捞 临绢甸霸 瞪 巴.
	// 窜, 瘤陛栏肺急 PVP 绰 概莫等 沥焊甫 捞 矫痢 捞饶俊唱 舅 荐 乐绢辑 阂啊..
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}
	return RetArray;
}

TArray<FCombinedPCSkillAnimID> AB2PVPGameMode::GetPCSkillAnimsToPreLoad()
{
	return TArray<FCombinedPCSkillAnimID> {};	// 老馆利牢 Preload矫痢俊 Load矫 Matching Scene 傈券锭 Flush 蜡惯
}

void AB2PVPGameMode::SetupPreRenderObjects()
{
	// 扁夯 PreRender 鸥捞怪俊绰 酒公巴档 窍瘤 臼绰促. 概莫 饶 皋牢 饭骇捞 胶飘府怪 登骨肺 弊锭辑具 角青.
}

void AB2PVPGameMode::PreBuildTagCharMesh()
{
	// AB2StageGameModeBase::PreBuildTagCharMesh 客 吝汗等 内靛牢单
	// 惑加 备炼 惑 弊率窍绊绰 冻绢廉 乐栏搁辑 NetGameMode 俊辑 捞 扁瓷捞 鞘夸茄 镑捞 PVP 观俊 绝绊 秦辑..
	// 措脚 琴缴 扁瓷篮 傍烹登霸 哗 初澜.

	// B2CompositeMeshCache 狼 RooSet 贸府甫 荤侩窍霸 登搁 利绢档 肺拿 某腐磐俊 措秦辑绰 捞扒 蜡疙公角茄单..
	// 弊巴焊促 PVP 绰 惑措规 怕弊 某腐磐 皋浆甫 Prebuild 且 荐 乐栏搁 亮促.
	//PrebuildLocalTagCharMesh();
}

#if WITH_EDITOR
void AB2PVPGameMode::HandleEditorPVPPreviewSetup()
{
	if (!GIsEditor){ // 俊叼磐 歹喉玭
		return;
	}

	//// 老瞒利栏肺 楷免 技泼阑 困秦 弥家茄狼 牢霸烙 敲肺快 悸诀捞 啊瓷窍档废 窃. 绢蠢 沥档狼 霸烙敲饭捞啊 登搁 亮瘤父 弊霸 林 格利篮 酒丛.
	//
	//APlayerController* CandidatePC = NULL;
	//for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	//{
	//	APlayerController* PlayerController = Iterator->Get();
	//	if (PlayerController && PlayerController->GetPawn() == NULL)
	//	{
	//		CandidatePC = PlayerController;
	//		break;
	//	}
	//}

	//if (CandidatePC)
	//{
	//	SceneManager.SetGameMode(this);
	//	SceneManager.ChangeScene(true);

	//	CurrentPCClassEnum = GEditorPVPPreviewMyClassMain; 
	//	BladeIIGameImpl::GetLocalCharacterData().SetMainPlayerClass(CurrentPCClassEnum);
	//	BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(GEditorPVPPreviewMyClassTag); // 怕弊 某腐磐绰 DataStore 技泼等 吧肺 登绰 扒啊.
	//			
	//	RestartNetPlayer(CandidatePC, TEXT("P1"));
	//	ABladeIINetPlayer* LocalPlayer = Cast<ABladeIINetPlayer>(CandidatePC->GetPawn());
	//	if (LocalPlayer)
	//	{
	//		const uint32 FakeMyNetId		= 3242323;
	//		const uint32 FakeRemoteNetId	= 0x00FFFF00;
	//		LocalPlayer->SetNetId(FakeMyNetId); // check 救 吧府霸 酒公芭唱 持绢淋
	//		SetLocalController(CandidatePC);
	//		
	//		FB2ModPlayerInfo& MyInfo = RemotePlayerInfoData.Emplace(FakeRemoteNetId);
	//		BladeIIGameImpl::GetClientDataStore().GetRandomCharacterData(MyInfo);


	//		GetRemotePlayerInfoData(FakeRemoteNetId).SetMainPlayerClass(CurrentPCClassEnum);
	//		GetRemotePlayerInfoData(FakeRemoteNetId).SetSubPlayerClass(GEditorPVPPreviewMyClassTag);

	//		SpawnRemotePlayer(FakeRemoteNetId, CurrentPCClassEnum, GEditorPVPPreviewMyClassTag);
	//		RestartRemotePlayer(TEXT("P2"));

	//		SetRoomId(TEXT("EditorPreviewRoom"));
	//					
	//		SceneManager.SetState(EPVPMatchState::Battle); // 救俊辑 MatchState 档 痢橇窃.
	//	}
	//}
}

void AB2PVPGameMode::HandleEditorPVPPreviewRemoteDead()
{
	if (!GIsEditor){ // 俊叼磐 歹喉玭
		return;
	}
	UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome)
	{
		DocSome->SetLocalPlayerMatchResult(ENetMatchResult::LocalWin);
	}
	StartEventScene_MatchResult(true);
}
#endif

void AB2PVPGameMode::PrepareNetGame(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::PrepareNetGame"));
//
//#if !UE_BUILD_SHIPPING
//	if (NetFakeConfigure::GetInstance().GetNumToPlayRaid() == 1)
//	{
//		BII_CHECK(GetPeerType() != PEER_RESTRICTED);
//		BII_CHECK(GetMyNetId() != NetId);
//		BII_CHECK(GetLocalController());
//
//		Super::PrepareNetGame(NetId);
//
//		RestartNetPlayer(GetLocalController(), TEXT("P1"));
//	}
//	else
//#endif
//	{
//		BII_CHECK(GetPeerType() != PEER_RESTRICTED);
//		BII_CHECK(GetMyNetId() != NetId);
//		BII_CHECK(GetLocalController());
//		BII_CHECK(GetOpponent());
//
//		Super::PrepareNetGame(NetId);
//
//		RestartNetPlayer(GetLocalController(), TEXT("P1"));
//		RestartRemotePlayer(TEXT("P2"));
//	}
//
//	//UI困摹 急沥
//	bIsLeftLocalPlayer = true;
//
//	GetLocalPlayer()->SetTeamNum(static_cast<int32>(ETeamType::ETT_Red));
//	GetOpponent()->SetTeamNum(static_cast<int32>(ETeamType::ETT_Blue));
}

void AB2PVPGameMode::PrepareSlaveNetGame(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::PrepareSlaveNetGame"));

	BII_CHECK(GetPeerType() != PEER_SUPER);
	BII_CHECK(NetId != GetMyNetId());
	BII_CHECK(GetLocalController());
	BII_CHECK(GetOpponent());

	Super::PrepareSlaveNetGame(NetId);

	RestartNetPlayer(GetLocalController(), TEXT("P2"));
	RestartRemotePlayer(TEXT("P1"));

	//UI困摹 急沥
	bIsLeftLocalPlayer = false;

	GetLocalPlayer()->SetTeamNum(static_cast<int32>(ETeamType::ETT_Blue));
	GetOpponent()->SetTeamNum(static_cast<int32>(ETeamType::ETT_Red));
}

void AB2PVPGameMode::SetOpponent(ABladeIINetPlayer* opponent)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetOpponent"));

	BII_CHECK(opponent);
	Opponent = opponent;
}

ABladeIINetPlayer* AB2PVPGameMode::GetLocalPlayer() const
{
	return Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn());
}

void AB2PVPGameMode::HandleRequestMatchMaking()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleRequestMatchMaking"));
//	auto* localController = GetLocalController();
//	BII_CHECK(localController);
//	if (!bRepeatRequestMatchByError)
//	{
//		localController->PlayerCameraManager->SetManualCameraFade(1.f, FLinearColor::Black, false);
//		bRepeatRequestMatchByError = false;
//	}
//
//	if (auto* BattleDoc = UB2UIDocHelper::GetDocBattle())
//	{
//		BattleDoc->SetIsAIMe(false);
//	}
//
//	localController->SetVirtualJoystickVisibility(false);
//		
//	int32 matchType = b2network::B2MatchMakingType::BMMT_PVP_RANK;
//
//	// TODO 泅犁 急琶等 某腐磐 鸥涝阑 持绢林技夸
//	int32 mainCharacterType = CliToSvrPCClassType(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
//	int32 tagCharacterType = CliToSvrPCClassType(BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());
//	bool bUsedAdditionalPoint = false; // 概莫 萍南捞 0 俺老锭 眠啊 犁拳甫 荤侩且 巴牢瘤 犬牢 170421YJ
//	auto* DocPvP = UB2UIDocHelper::GetDocPVP1on1Rival();
//	if (DocPvP)	bUsedAdditionalPoint = DocPvP->GetUsedAdditionalPoint();
//
//	
//	if (IsValidFriendshipGame())
//	{
//		auto* DocMatch = UB2UIDocHelper::GetDocLobbyMatch();
//		const auto& InvitationInfo = DocMatch->GetInvitationInfo();
//
//		const FString& RoomID = InvitationInfo.RoomInfoPtr->room_id;
//		const int32 RoomAccessType = 1; // PrivateRoom
//		RequestJoinRoomBridgeClass<const FString&, int32, int32>::GetInstance().Signal(RoomID, 0, RoomAccessType);
//	}
//
//	else
//	{
//#if !UE_BUILD_SHIPPING
//		if (NetFakeConfigure::GetInstance().IsPVPRoomName())
//			data_trader::Retailer::GetInstance().RequestMatchmaking(matchType, mainCharacterType, tagCharacterType, bUsedAdditionalPoint, NetFakeConfigure::GetInstance().GetPVPRoomName());
//		else
//#endif
//			data_trader::Retailer::GetInstance().RequestMatchmaking(matchType, mainCharacterType, tagCharacterType, bUsedAdditionalPoint);
//	}
//
//
//	SceneManager.SetGameMode(this);
//	SceneManager.SetState(EPVPMatchState::Finding);
//
//	Super::HandleRequestMatchMaking();
//
//	// 概莫 Scene 傈券 捞饶 Local 某腐磐 SkillAnim 肺靛
//	TryAsyncLoadSkillAnims(&BladeIIGameImpl::GetLocalCharacterData());
}

void AB2PVPGameMode::HandleMatchIsWaitingToStart()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleMatchIsWaitingToStart"));

	Super::HandleMatchIsWaitingToStart();
}

void AB2PVPGameMode::HandleMatchOtherPlayer()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleMatchOtherPlayer"));

	//auto* PVPSceneFindMatchWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::PVPOneOnOneFindMatch);
	//if (PVPSceneFindMatchWidget)
	//{
	//	PVPSceneFindMatchWidget->SetEnableCancel(false); //辑滚胶抛捞飘 部捞瘤 臼档废 秒家给窍霸
	//}

	//ErrorHandlingTimeExchangeInfo = 0.f;

	//Super::HandleMatchOtherPlayer();
}

void AB2PVPGameMode::HandleExchangeInfos()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleExchangeInfos"));

	//// now just send character classes...
	//auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	//BII_CHECK(DocBattle);

	//auto& DataStore = BladeIIGameImpl::GetLocalCharacterData();

	//FGuid MyTokenToPlay;
	//FString Temp = TokenToPlay.Replace(TEXT("-"), TEXT(""));
	//auto TempCharArray = *Temp;

	//if (MyTokenToPlay.ImportTextItem(TempCharArray, 0, nullptr, nullptr) == false)
	//{
	//	// 模急傈篮 Token 鞘夸 X
	//	check(IsValidFriendshipGame());
	//}

	//packet::ByteStream	payload;
	//payload << GetMyNetId()
	//	<< DataStore.GetMainPlayerClass() << DataStore.GetSubPlayerClass()
	//	<< DocBattle->GetTagMatchGrade() << DocBattle->GetTagMatchGradeStar() << DocBattle->GetTagMatchCurrContinuousWins()
	//	<< MyTokenToPlay << BladeIIGameImpl::GetClientDataStore().GetAccountId();

	//FString encoded_string = packet::FinalizePacket(packet::EXCHANGE_INFORMATIONS, GetMyNetId(), 0, packet::ALLBUTME, payload);
	//SendMessage(encoded_string);
}

void AB2PVPGameMode::HandleDoneToExchange()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleDoneToExchange"));

	if (GetPeerType() == PEER_SUPER)
	{
		ReadyCheck.Empty();
		ReadyCheck.Add(GetMyNetId(), false);
		for (auto& Elem : RemotePlayerInfoData)
		{
			ReadyCheck.Add(Elem.Key, false);
		}
	}

	SceneManager.SetState(EPVPMatchState::Ready);
}

void AB2PVPGameMode::HandleStartResult()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleStartResult"));

	BII_CHECK(GetOpponent());
	//HandleFinishBattle()苞 HandleStartResult()荤捞俊 AI啊 悼累窍绰 巩力啊 乐绢
	//困摹甫 HandleFinishBattle()肺 函版
	/*
	auto* AIController = Cast<ABladeIIPlayerAIController>(Opponent->GetController());
	if (AIController)
		AIController->StopAutoPlay();
	*/

	//StopPauseMenuClass<>::GetInstance().Signal();

	Super::HandleStartResult();
}

void AB2PVPGameMode::HandleLeaveEnemy()
{
	LeaveEnemyInProgress();
}

void AB2PVPGameMode::HandleFinishBattle()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleFinishBattle"));
	Super::HandleFinishBattle();

	BII_CHECK(GetOpponent());

	//HandleFinishBattle()苞 HandleStartResult()荤捞俊 AI啊 悼累窍绰 巩力啊 乐绢
	//困摹甫 HandleFinishBattle()肺 函版
	auto* AIController = Cast<ABladeIIPlayerAIController>(Opponent->GetController());
	if (AIController)
		AIController->StopAutoPlay();

	SceneManager.SetState(EPVPMatchState::BattleEnd);
}

void AB2PVPGameMode::SetMyPlayerInfoData(uint32 NetId, const FB2FindAccountInfoPtr& AccountInfo)
{
	check(NetId != 0);
	Super::SetMyPlayerInfoData(NetId, AccountInfo);
//
//#if !UE_BUILD_SHIPPING
//	if (NetFakeConfigure::GetInstance().GetNumToPlayRaid() == 1)
//	{
//		PrepareMyPlayer(NetId);
//		SceneManager.SetState(EPVPMatchState::Battle);
//	}
//	else
//#endif
//	{
//		SetMatchState(NetMatchState::MatchOtherPlayer);
//	}
}

void AB2PVPGameMode::SetRemotePlayerInfoData(uint32 NetId, int64 AccountId, const FB2FindAccountInfoPtr& AccountInfo)
{
	Super::SetRemotePlayerInfoData(NetId, AccountId, AccountInfo);
}

void AB2PVPGameMode::HandleNotifyResult(ENetMatchResult Ret, uint8 InState)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleNotifyResult"));

	//if (GetMatchState() != NetMatchState::StartResult)
	//	SetMatchState(NetMatchState::StartResult);

	//// 搬苞 沥焊甫 UI 率俊 技泼窍绰 何盒篮 备泅 沥府登搁 茄 镑栏肺
	//UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	//if (DocSome)
	//	DocSome->SetLocalPlayerMatchResult(Ret);

	//if (IsValidFriendshipGame())
	//{
	//	SceneManager.ShowResult();
	//	return;
	//}

	//if (!bRequestResult)
	//{
	//	HostMatchState = static_cast<EPVPMatchState>(InState);

	//	int32 matchType = GetMatchType();
	//	auto myMainCharater = std::make_shared<b2network::B2RoomCharacter>();
	//	auto myTagCharater = std::make_shared<b2network::B2RoomCharacter>();
	//	auto opponentMainCharater = std::make_shared<b2network::B2RoomCharacter>();
	//	auto opponentTagCharater = std::make_shared<b2network::B2RoomCharacter>();

	//	auto& DataStore = BladeIIGameImpl::GetClientDataStore();
	//	ICharacterDataStore* MyData = &DataStore.GetLocalCharacterData();

	//	/////////////////////////////////////////////////////////////////////////////////////////////////
	//	//For Crash Test
	//	FB2ModPlayerInfo* OpponentData = nullptr;
	//	if (Opponent)
	//		OpponentData = &GetRemotePlayerInfoData(Opponent->GetNetId());
	//	/////////////////////////////////////////////////////////////////////////////////////////////////

	//	if (OpponentData != nullptr && MyData != nullptr)
	//	{
	//		EPCClass MyMain = MyData->GetMainPlayerClass();
	//		EPCClass MySub = MyData->GetSubPlayerClass();
	//		EPCClass OppMain = OpponentData->GetMainPlayerClass();
	//		EPCClass OppSub = OpponentData->GetSubPlayerClass();

	//		myMainCharater->character_level = MyData->GetCharacterLevel(MyMain);
	//		myMainCharater->character_power = CombatStatEval::GetPCCombatPower(MyMain, MyData);
	//		myMainCharater->character_type = CliToSvrPCClassType(MyMain);

	//		myTagCharater->character_level = MyData->GetCharacterLevel(MySub);
	//		myTagCharater->character_power = CombatStatEval::GetPCCombatPower(MySub, MyData);
	//		myTagCharater->character_type = CliToSvrPCClassType(MySub);

	//		opponentMainCharater->character_level = OpponentData->GetCharacterLevel(OppMain);
	//		opponentMainCharater->character_power = CombatStatEval::GetPCCombatPower(OppMain, OpponentData);
	//		opponentMainCharater->character_type = CliToSvrPCClassType(OppMain);

	//		opponentTagCharater->character_level = OpponentData->GetCharacterLevel(OppSub);
	//		opponentTagCharater->character_power = CombatStatEval::GetPCCombatPower(OppSub, OpponentData);
	//		opponentTagCharater->character_type = CliToSvrPCClassType(OppSub);
	//	}

	//	data_trader::Retailer::GetInstance().RequestMatchResult(BladeIIGameImpl::GetClientDataStore().GetAccountId(), TokenToPlay, static_cast<uint8>(Ret),
	//		matchType, myMainCharater, myTagCharater, opponentMainCharater, opponentTagCharater, GetRoomId());

	//	bRequestResult = true;

	//	ABladeIIPlayer* pLocalPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));

	//	if (pLocalPlayer)
	//	{
	//		pLocalPlayer->StopRepeatingInput(true);
	//	}
	//}
}

void AB2PVPGameMode::LeaveEnemyInProgress()
{
	if (GetMatchState() == NetMatchState::StartResult || GetMatchState() == NetMatchState::FinishBattle)
		return;

	//StopPlayers();

	//if (GetOpponent()->bIsDying)
	//{
	//	SetMatchResult(MatchResult_Win);
	//}
	//else if (GetLocalPlayer()->bIsDying)
	//{
	//	SetMatchResult(MatchResult_Lose);
	//}
	//else
	//{
	//	UB2UIPVP1on1BattleMain* PVPSceneBattleMainWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVP1on1BattleMain>(UIFName::PVPOneOnOneBattleMain);

	//	if (PVPSceneBattleMainWidget)
	//	{
	//		PVPSceneBattleMainWidget->SetLeaveEnemyText(ESlateVisibility::Visible);

	//		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToLeaveEnemy);
	//		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(
	//			TimeToLeaveEnemy
	//			, this
	//			, &AB2PVPGameMode::LeaveOpponentPlayer
	//			, 0.1f
	//			, false
	//			, 2.5f);
	//	}
	//	else
	//	{
	//		LeaveOpponentPlayer();
	//	}
	//	data_trader::Retailer::GetInstance().RequestRoomLeaveResult(GetRoomId(), GetOpponent()->GetAccountId(), OpponentTokenToPlay);
	//}
}

void AB2PVPGameMode::HandleNotifyLeave(uint32 NetId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleNotifyLeave"));

	SetPeerType(PEER_NETSTANDALONE);

	bool bWaitNotifyResult = (MatchState == NetMatchState::StartResult || MatchState == NetMatchState::FinishBattle) && !bRequestResult;

	if (MatchState == MatchState::InProgress || bWaitNotifyResult)
	{
		SetMatchState(NetMatchState::LeaveEnemyInBattle);
		//LeaveEnemyInProgress();
	}
	else
	{
		LeaveEnemyBeforeInProgress = true;
	}
}

void AB2PVPGameMode::HandleCharacterDead(ABladeIICharacter* DeadGuy, uint32 NetId, float KillingDamage)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::HandleCharacterDead"));

	BII_CHECK(GetLocalController());
	DeadGuy->Die(KillingDamage, FDamageEvent(), GetLocalController(), GetLocalController()->GetPawn());
}

void AB2PVPGameMode::PlayMatchWithAI(FB2TagMatchMakingPtr InMatchInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::PlayMatchWithAI"));

	//auto* PVPSceneFindMatchWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::PVPOneOnOneFindMatch);
	//if (PVPSceneFindMatchWidget)
	//{
	//	PVPSceneFindMatchWidget->SetEnableCancel(false); //辑滚胶抛捞飘 部捞瘤 臼档废 秒家给窍霸
	//}

	//SetPeerType(PEER_STANDALONE);
	//SetRoomId(TEXT("GetOutHere.IWannaStayAlone"));
	//PrepareMyPlayer(0xffffffff);

	//FB2OpponentMatchInfoDataPtr MatchInfo = InMatchInfo->ai_opponent;

	//checkf(MatchInfo.get() && MatchInfo->account.get() && MatchInfo->account->characters.Num() >= 2, TEXT("Missing some data.. probably due to network connection.."));
	//if (MatchInfo && MatchInfo->account && MatchInfo->account->characters.Num() >= 2)
	//{
	//	bIsTutorial = MatchInfo->debuff_value > 0;
	//	TutorialDebuffValue = MatchInfo->debuff_value * 0.01f;

	//	EPCClass MainCharClass = SvrToCliPCClassType(MatchInfo->account->characters[0]->character_type);
	//	EPCClass SubCharClass = SvrToCliPCClassType(MatchInfo->account->characters[1]->character_type);

	//	const uint32 FakeRemoteNetId = 0x0000FFFF;
	//	SetRemotePlayerInfoData(FakeRemoteNetId, 0, MatchInfo->account);
	//	GetRemotePlayerInfoData(FakeRemoteNetId).SetMainPlayerClass(MainCharClass);
	//	GetRemotePlayerInfoData(FakeRemoteNetId).SetSubPlayerClass(SubCharClass);

	//	auto* DocRival = UB2UIDocHelper::GetDocPVP1on1Rival();
	//	if (DocRival && MatchInfo.get() && MatchInfo->match_status.get())
	//	{
	//		DocRival->SetTagMatchGrade(MatchInfo->match_status->battle_grade);
	//		DocRival->SetTagMatchGradeStar(MatchInfo->match_status->battle_star_count);
	//		DocRival->SetTagMatchCurrContinuousWins(MatchInfo->match_status->continue_win);
	//	}

	//	SpawnRemotePlayer(FakeRemoteNetId, MainCharClass, SubCharClass);

	//	auto* BattleDoc = UB2UIDocHelper::GetDocBattle();
	//	
	//	if (BattleDoc && MatchInfo->account->account_id == BladeIIGameImpl::GetClientDataStore().GetAccountId())
	//	{
	//		BattleDoc->SetIsAIMe(true);
	//	}

	//}

	//SceneManager.SetState(EPVPMatchState::Ready);
}

void AB2PVPGameMode::LeaveOpponentPlayer()
{
	SetMatchResult(MatchResult_Win);
}

void AB2PVPGameMode::SpawnRemotePlayer(uint32 NetId, EPCClass MainClass, EPCClass SubClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SpawnRemotePlayer"));

	//FString TagName		= GetPeerType() == PEER_RESTRICTED ? TEXT("P1") : TEXT("P2");

	//AActor* StartSpot = FindPlayerStart(NULL, TagName);
	//BII_CHECK(StartSpot);

	//FActorSpawnParameters SpawnParam;
	//SpawnParam.Owner = nullptr;
	//SpawnParam.bDeferConstruction = true; 
	//SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//FTransform SpawnTransform = StartSpot->GetTransform();

	//auto* RemotePlayer = Cast<ABladeIINetPlayer>(AB2NetGameMode::SpawnRemotePlayer(GetWorld(), MainClass, SpawnTransform, SpawnParam));
	//BII_CHECK(RemotePlayer);

	//RemotePlayer->CurrentPlayingClass = RemotePlayer->MainPlayerClass = MainClass;
	//RemotePlayer->SubPlayerClass = SubClass;
	//RemotePlayer->SetNetId(NetId);
	//RemotePlayer->DisableComponentsSimulatePhysics();
	//RemotePlayer->SetActorHiddenInGame(true);
	//if(GetPeerType() == PEER_NETSTANDALONE)
	//	RemotePlayer->SetTeamNum(!IsLeftLocalPlayer() ? int32(ETeamType::ETT_Red) : int32(ETeamType::ETT_Blue));
	//else
	//	RemotePlayer->SetTeamNum(GetPeerType() == PEER_RESTRICTED ? int32(ETeamType::ETT_Red) : int32(ETeamType::ETT_Blue));
	//RemotePlayer->FinishSpawning(SpawnTransform);

	//if (bIsTutorial)
	//{
	//	if (auto* InPlayer = Cast<ABladeIICharacter>(GetLocalPlayer()))
	//	{
	//		RemotePlayer->SetPvPTutorialHandicap(InPlayer, TutorialDebuffValue);
	//	}
	//}

	//SetOpponent(RemotePlayer);

	//auto* DocBattleRival = UB2UIDocHelper::GetDocPVP1on1Rival();
	//if (DocBattleRival)
	//{
	//	DocBattleRival->SetCurPCClass(PCClassToInt(RemotePlayer->CurrentPlayingClass));
	//	DocBattleRival->SetCurPCLevel(RemotePlayer->GetCurCharacterLevel());
	//	DocBattleRival->SetTagPCClass(PCClassToInt(RemotePlayer->SubPlayerClass));

	//	auto* FindingMatchUI = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::PVPOneOnOneFindMatch);
	//	if (FindingMatchUI)
	//	{
	//		SceneManager.SetRightEntry(&GetRemotePlayerInfoData(NetId), MainClass, SubClass);
	//		FindingMatchUI->FindMatch(true);
	//	}
	//}

	//// Matching 惑措 SkillAnim AsyncLoad
	//TryAsyncLoadSkillAnims(&GetRemotePlayerInfoData(NetId));
}

bool AB2PVPGameMode::RemotePlayerTagToOther(ABladeIINetPlayer* player, int32 TagAttackType)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::RemotePlayerTagToOther"));

	//BII_CHECK(player);

	//if (GetMatchState() == NetMatchState::StartResult)
	//	return false;
	////*
	//EPCClass NewSpawnClass = player->GetTagPlayerClass();

	//ABladeIINetPlayer* OldPlayerBackup = player;

	//auto NewPlayer = Cast<ABladeIINetPlayer>(DynamicSpawnNewPlayerForTag(player, NewSpawnClass));
	//BII_CHECK(NewPlayer);

	//NewPlayer->SetSpawnMotionState(static_cast<EPCSpawnMotionState>(TagAttackType + static_cast<int32>(EPCSpawnMotionState::EPST_Tag)));

	//FTransform PrevTransform = OldPlayerBackup->GetTransform();
	//FTransform NewTransform(
	//	PrevTransform.GetRotation(),
	//	PrevTransform.GetTranslation() + FVector(0.0f, 0.0f, 1.0f), // 混娄 困俊辑
	//	PrevTransform.GetScale3D()
	//);

	//// when the AI controller possess remote player, after new player is spawned 
	//// the controller must possess new player to control it...

	//// transfer OldPlayerBackup data to NewPlayer...
	//NewPlayer->CurrentPlayingClass	= NewSpawnClass;
	//NewPlayer->MainPlayerClass		= OldPlayerBackup->MainPlayerClass;
	//NewPlayer->SubPlayerClass		= OldPlayerBackup->SubPlayerClass;
	//NewPlayer->SetNetStatus(OldPlayerBackup->GetNetStatus());
	//NewPlayer->SetNetId(OldPlayerBackup->GetNetId());
	//if (GetPeerType() == PEER_NETSTANDALONE)
	//	NewPlayer->SetTeamNum(!IsLeftLocalPlayer() ? int32(ETeamType::ETT_Red) : int32(ETeamType::ETT_Blue));
	//else
	//	NewPlayer->SetTeamNum(GetPeerType() == PEER_RESTRICTED ? int32(ETeamType::ETT_Red) : int32(ETeamType::ETT_Blue));

	//NewPlayer->FinishSpawning(NewTransform);//咯扁辑 眉仿阑 老窜 啊垫 眉框(ABladeIICharacter::InitializeCombatStats)
	//NewPlayer->NotifyTagSpawn(OldPlayerBackup);

	//if (bIsTutorial)
	//{
	//	if (auto* InPlayer = Cast<ABladeIICharacter>(GetLocalPlayer()))
	//	{
	//		NewPlayer->SetPvPTutorialHandicap(InPlayer, TutorialDebuffValue);
	//	}
	//}

	////[@AKI, 170327] Tag秦辑 HP Bar俊 HP 菜 蛮 惑怕肺 唱坷绰 泅惑 荐沥窍扁 困秦 第肺 猾 内靛
	////NotifyTagSpawn阑 沁阑锭 眉仿阑 啊垫 盲款 惑怕肺 倒妨林绰单
	////盔贰 眉仿栏肺 倒妨林绰 内靛啊 OnPlayerChangedByTag()救俊 FInactivePlayerStateCache::SetToNewCommingPlayer()俊辑 窃
	//GetRemoteSkillRTManager(NewPlayer->GetNetId())->SetCurrentOwnerPlayer(NewPlayer);
	//GetRemoteSkillRTManager(NewPlayer->GetNetId())->OnPlayerChangedByTag(OldPlayerBackup, NewPlayer); //咯扁辑 CurrentHP甫 罐酒辑 NewPlayer俊 泅犁 眉仿阑 倒妨淋
	//GetRemoteSkillRTManager(NewPlayer->GetNetId())->SetSkillCoolTimeTimer(EAttackState::ECS_TagOtherChar);

	//SwitchOpponent(NewPlayer);

	////PvPAI俊辑 Tag矫 巩力肺 牢秦 SwitchOpponent(NewPlayer); 促澜俊 AIController 内靛啊 客具 凳
	//auto* Controller = Cast<ABladeIIPlayerAIController>(OldPlayerBackup->GetController());
	//if (Controller)
	//{
	//	Controller->Possess(NewPlayer);
	//	Controller->BeginAutoPlay();
	//}

	//OldPlayerBackup->Role = ROLE_Authority;
	//GetWorld()->DestroyActor(OldPlayerBackup);

	return true;
} 

ABladeIIPlayer* AB2PVPGameMode::DynamicSpawnNewPlayerForTag(ABladeIINetPlayer* Requester, EPCClass ClassToSpawn)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::DynamicSpawnNewPlayerForTag"));

	BII_CHECK(Requester)
	BII_CHECK(ClassToSpawn != EPCClass::EPC_End);

	FTransform PrevTransform = Requester->GetTransform();
	FTransform NewTransform(
		PrevTransform.GetRotation(),
		PrevTransform.GetTranslation() + FVector(0.0f, 0.0f, 50.0f), // 混娄 困俊辑
		PrevTransform.GetScale3D()
	);

	Requester->SetActorEnableCollision(false); // 瘤陛 磊府俊 spawn 捞 登聪 collision 阑 波 霖促.

	// 馆靛矫 DeferContruction 阑 窍绊 Possess 饶 FinishSpawning 阑.
	ABladeIIPlayer* NewSpawned = SpawnTagPlayer(Requester, ClassToSpawn, NewTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, true);
	
	if (NewSpawned)
		NewSpawned->SetTeamNum(IsLeftLocalPlayer() ? int32(ETeamType::ETT_Red) : int32(ETeamType::ETT_Blue));

	BII_CHECK(NewSpawned);

	return NewSpawned;
}

ABladeIIPlayer* AB2PVPGameMode::SpawnTagPlayer(
	ABladeIINetPlayer* Requester,
	EPCClass InClass,
	const FTransform& SpawnTransform,
	ESpawnActorCollisionHandlingMethod CollisionHandlingOverride /*= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn*/,
	bool bDeferConstruction /*= true*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SpawnTagPlayer"));

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner							= nullptr;
	SpawnParam.bDeferConstruction				= bDeferConstruction; // 捞吧 true 肺 窍搁 捞芭 龋免 捞饶俊 FinishSpawning 阑 蝶肺 阂矾林绢具 窍绰 锅芭肺框捞 乐瘤父 老馆利牢 霸烙敲饭捞 惑炔俊辑狼 肺拿 敲饭捞绢 某腐磐扼搁 BeginPlay 矫痢俊 PlayerController 俊 狼秦 Possess 等 惑怕咯具 且 巴捞骨肺 鞘夸窃.
	SpawnParam.SpawnCollisionHandlingOverride	= CollisionHandlingOverride;

	auto* World			= Requester->GetWorld();

	BII_CHECK(World);
	ABladeIIPlayer* NewSpawned(AB2NetGameMode::SpawnRemotePlayer(World, InClass, SpawnTransform, SpawnParam));
	
	if (NewSpawned)
		NewSpawned->SetTeamNum(IsLeftLocalPlayer() ? int32(ETeamType::ETT_Red) : int32(ETeamType::ETT_Blue));
	
	BII_CHECK(NewSpawned);
	
	return NewSpawned;
}

ABladeIIPlayer* AB2PVPGameMode::SpawnRemoteTagAsPuppet(bool bPossessByAIController)
{ 
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SpawnRemoteTagAsPuppet"));

	// ABladeIIPlayer::SpawnOtherAsPuppet 府葛飘 敲饭捞绢 滚傈
	BII_CHECK(IsValid(Opponent));

	auto* OldOpponent = Opponent;
	FTransform CurrTransform = Opponent->GetTransform();
	const float AsideDistance = Opponent->GetCapsuleComponent() ? Opponent->GetCapsuleComponent()->GetScaledCapsuleRadius() : 50.0f;
	FTransform NewTransform(
		CurrTransform.GetRotation(),
		CurrTransform.GetTranslation() + FVector(AsideDistance, AsideDistance, 50.0f), // 粱 糠俊辑 spawn 矫挪促.
		CurrTransform.GetScale3D()
	);

	EPCClass NewSpawnClass = Opponent->GetTagPlayerClass();
	if (NewSpawnClass != EPCClass::EPC_End)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = nullptr;
		SpawnParam.bDeferConstruction = true;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		auto* RemoteTagActor = Cast<ABladeIINetPlayer>(AB2NetGameMode::SpawnRemotePlayer(GetWorld(), NewSpawnClass, NewTransform, SpawnParam));
		
		RemoteTagActor->SetNetId(Opponent->GetNetId()); // 捞吧 持绢林绢具 某教等 皋浆甫 (乐促搁) 利例洒 啊廉坷霸 等促.
		RemoteTagActor->FinishSpawning(NewTransform); //<- BeginPlay甫 龋免窍咯 厘厚殿阑 嘎苗涝霸 茄促.

		SetOpponent(OldOpponent);

		return RemoteTagActor;		
	}
	BII_CHECK(0);

	return NULL;
}

void AB2PVPGameMode::GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum, bool bOnlyLiveMobs)
{
//	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::GetAllWorldEnemyList"));

	//auto* LocalPlayer = Cast<ABladeIICharacter>(GetLocalController()->GetPawn());
	//auto* OpponentPlayer = Cast<ABladeIICharacter>(GetOpponent());

	//if (IsValid(LocalPlayer) && (!bOnlyLiveMobs || LocalPlayer->IsAlive()) && LocalPlayer->CanBeTarget() && LocalPlayer->GetTeamNum() != MyTeamNum)
	//	OutList.Add(LocalPlayer);
	//
	//if (IsValid(OpponentPlayer) && (!bOnlyLiveMobs || OpponentPlayer->IsAlive()) && OpponentPlayer->CanBeTarget() /*&& OpponentPlayer->GetTeamNum() != MyTeamNum*/)
	//	OutList.Add(OpponentPlayer);
}

void AB2PVPGameMode::SwitchOpponent(ABladeIINetPlayer* opponent)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SwitchOpponent"));

	SetOpponent(opponent);
}

void AB2PVPGameMode::RestartRemotePlayer(FString const& TagName)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::RestartRemotePlayer"));

	auto* opponent		= GetOpponent();
	BII_CHECK(opponent);
	
	auto* start			= MyFindPlayerStart(TagName);
	BII_CHECK(start);

	FRotator StartRotation(ForceInit);
	StartRotation.Yaw		= start->GetActorRotation().Yaw;
	FVector StartLocation	= start->GetActorLocation();

	opponent->TeleportTo(StartLocation, StartRotation);

	opponent->SetActorHiddenInGame(true);
}

ABladeIIPlayerAIController* AB2PVPGameMode::SpawnAIController(FVector const& SpawnLocation, FRotator const& SpawnRotation)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SpawnAIController"));

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator						= NULL;
	SpawnInfo.SpawnCollisionHandlingOverride	= ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.ObjectFlags					   |= RF_Transient;	// We never want to save player controllers into a map
	SpawnInfo.bDeferConstruction				= true;

	auto* NewAC = GetWorld()->SpawnActor<ABladeIIPlayerAIController>(ABladeIIPlayerAIController::StaticClass(), SpawnLocation, SpawnRotation, SpawnInfo);
	BII_CHECK(NewAC);

	UGameplayStatics::FinishSpawningActor(NewAC, FTransform(SpawnRotation, SpawnLocation));
	return NewAC;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	PVP Scene management
//
#include "Engine/LevelStreaming.h"
#include "Engine/LevelScriptActor.h"
#include "Animation/AnimationAsset.h"
#include "B2PCClassInfo.h"
#include "B2PCClassInfoBox.h"
//#include "Matinee/InterpGroup.h"
//#include "Matinee/InterpGroupInst.h"
//#include "Matinee/InterpTrackMove.h"
#include "Animation/SkeletalMeshActor.h"

//
//void AB2PVPGameMode::SetMatchLobbyResources(
//	class ALevelSequenceActor * InMatinee,
//	const FMatchingAnims& Gladiator,
//	const FMatchingAnims& Assassin,
//	const FMatchingAnims& Wizard,
//	const FMatchingAnims& Fighter
//)
//{
//	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetMatchLobbyResource"));
//
//	SceneManager.SetMatchLobbyResources(InMatinee, Gladiator, Assassin, Wizard, Fighter);
//}

void AB2PVPGameMode::StartMatchResult()
{
	SettingResultInfo();
}

void AB2PVPGameMode::SetMatchLobbyCharacterLocationZ(float GladiatorZ, float AssassinZ, float WizardZ, float FighterZ)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetMatchLobbyCharacterLocationZ"));

	SceneManager.SetMatchLobbyCharacterLocationZ(GladiatorZ, AssassinZ, WizardZ, FighterZ);
}

void AB2PVPGameMode::SetMatchLobbyLevel(ULevelStreaming* PackageToLoad)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetMatchLobbyLevel"));

	SceneManager.SetMatchLobbyLevelName(PackageToLoad->GetWorldAssetPackageFName());
}

void AB2PVPGameMode::SetMatchBattleLevel(ULevelStreaming* PackageToLoad)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetMatchBattleLevel"));

	SceneManager.SetMatchBattleLevelName(PackageToLoad->GetWorldAssetPackageFName());
}

void AB2PVPGameMode::PlayPVPBGM(EPVPBGMPlayContext InContext, bool bForceStopCurrent)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::PlayPVPBGM"));

	//if (bForceStopCurrent)
	//	StopBGM();

	//if (StaticFindCommonSoundInfo(this))
	//{
	//	ECommonSoundID MatchingSoundID = ECommonSoundID::ECSID_End;
	//	switch (InContext)
	//	{
	//	case EPVPBGMPlayContext::PVPBGMCTX_Standby: MatchingSoundID = ECommonSoundID::ECSID_BGM_PVPStandby; break;
	//	case EPVPBGMPlayContext::PVPBGMCTX_Combat: MatchingSoundID = ECommonSoundID::ECSID_BGM_PVPCombat; break;
	//	case EPVPBGMPlayContext::PVPBGMCTX_Victory: MatchingSoundID = ECommonSoundID::ECSID_BGM_PVPVictory; break;
	//	case EPVPBGMPlayContext::PVPBGMCTX_Lose: MatchingSoundID = ECommonSoundID::ECSID_BGM_Lose; break;
	//	}

	//	PlayBGMByCommonSoundID(MatchingSoundID);
	//}
}

void AB2PVPGameMode::StartEventScene_MatchResult(bool HasLocalPlayerWon)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::StartEventScene_MatchResult"));

	if (HasLocalPlayerWon)
	{
		PlayPVPBGM(EPVPBGMPlayContext::PVPBGMCTX_Victory, true);
	}
	else
	{ // 酒丛 脸阑 订 傲 吝窜?
		PlayPVPBGM(EPVPBGMPlayContext::PVPBGMCTX_Lose, true);
	}

	//[@AKI, 170613] 距 匙飘况农俊辑 搬苞甫 辑滚俊辑 罐瘤 臼篮 惑怕俊辑 搬苞拳搁 焊咯林瘤 臼霸 函版
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::PVPResult);

	Super::StartEventScene_MatchResult(HasLocalPlayerWon);
}

void FPVPMatchSceneManager::SetGameMode(class AB2PVPGameMode* InGameMode)
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetGameMode"));

	GameMode = InGameMode; 	

	if (GameMode)
	{ // 饭骇 府家胶 匙捞萍宏 饭欺繁胶 某教 鸥捞怪.
		GameMode->RecacheLevelResources();
	}
}

void FPVPMatchSceneManager::SetMatchLobbyResources(
	class ALevelSequenceActor * InMatinee,
	const FMatchingAnims& Gladiator,
	const FMatchingAnims& Assassin,
	const FMatchingAnims& Wizard,
	const FMatchingAnims& Fighter
)
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetMatchLobbyResources"));

	//AnimArray.Empty(4);
	//AnimArray.Add(Gladiator);
	//AnimArray.Add(Assassin);
	//AnimArray.Add(Wizard);
	//AnimArray.Add(Fighter);

	//LobbyMatinee = InMatinee; 

	//if (LobbyMatinee)
	//	ABladeIIGameMode::SetCameraMatineeCam(LobbyMatinee, 0.f);
}

void FPVPMatchSceneManager::SetMatchLobbyCharacterLocationZ(float GladiatorZ, float AssassinZ, float WizardZ, float FighterZ)
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetMatchLobbyCharacterLocationZ"));

	CharacterLocationZ.Empty(4);
	CharacterLocationZ.Add(GladiatorZ);
	CharacterLocationZ.Add(AssassinZ);
	CharacterLocationZ.Add(WizardZ);
	CharacterLocationZ.Add(FighterZ);
}

void FPVPMatchSceneManager::SetState(EPVPMatchState InState)
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetState"));

	BII_CHECK(GameMode);

	CurrentState = InState;

	switch (InState)
	{
	case EPVPMatchState::Finding:
		SetFinding();
		break;
	case EPVPMatchState::Ready:
		SetReady();
		break;
	case EPVPMatchState::InGameReady:
		SetInGameReady();
		break;
	case EPVPMatchState::PreBattle:
		SetPreBattle();
		break;
	case EPVPMatchState::Battle:
		SetBattle();
		break;
	//[@AKI, 170418] 绢蠢 茄率捞 磷绢辑 场车阑 版快 磷篮 纳腐磐甫 啊廉客辑 秦榜 钎矫甫 弊府扁 困秦 眠啊
	// M16 老措老&评措傈俺急(冠扁怕) 5page 曼炼
	case EPVPMatchState::BattleEnd:
		SetBattleEnd();
		break;
	case EPVPMatchState::EndMatchByTime:
		SetEndMatchByTime();
		break;
	default:
		break;
	}
		
	BII_CHECK(GameMode);
	auto* LocalController = GameMode->GetLocalController();
	if (LocalController)
	{
		LocalController->SetVirtualJoystickVisibility(InState == EPVPMatchState::Battle);
	}
}

void FPVPMatchSceneManager::Tick(float DeltaSeconds)
{
	//if (CurrentState == EPVPMatchState::Ready)
	//{
	//	ElapsedWaitingReadyToMatchTime += DeltaSeconds;

	//	auto* PVPSceneFindMatchWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::PVPOneOnOneFindMatch);
	//	if (PVPSceneFindMatchWidget)
	//	{
	//		PVPSceneFindMatchWidget->SetCount(FMath::Min(WaitingReadyToMatchTime - ElapsedWaitingReadyToMatchTime + 0.5f, 5.f));
	//	}

	//	if (ElapsedWaitingReadyToMatchTime > WaitingReadyToMatchTime)
	//		SetState(EPVPMatchState::InGameReady);
	//}
	//else if (CurrentState == EPVPMatchState::Battle)
	//{
	//	if (IsStartTimeCount && LeftTimeBattle > 0.f)
	//	{
	//		LeftTimeBattle = FMath::Max<double>(TimeLimitBattle - (FApp::GetCurrentTime() - TimeBeginPlay), 0.0);

	//		if (LeftTimeBattle == 0.0)
	//		{
	//			//if (!GIsPlayInEditorWorld)
	//				SetState(EPVPMatchState::EndMatchByTime);
	//		}
	//	}

	//	if (auto* PVPSceneBattleMainWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVP1on1BattleMain>(UIFName::PVPOneOnOneBattleMain))
	//	{
	//		PVPSceneBattleMainWidget->SetLeftTime(FMath::CeilToInt(LeftTimeBattle));
	//	}
	//}
}

void FPVPMatchSceneManager::SetFinding()
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetFinding"));

	//ChangeScene(false);
	//BII_CHECK(GameMode);

	//B2GMLoadingProgCollector::StepToMax(); // 肺爹 柳青档. PVP 绰 粱 漂荐茄 荤沥捞 乐绰单 开矫唱 捞 码辑 菜 盲框.
	//GameMode->SetupUIScene();
	//
	//if (LobbyMatinee)
	//{
	//	float KeyTime = ABladeIIGameMode::GetMatineePositionByEventName(LobbyMatinee, TEXT("pvpLobby"));
	//	LobbyMatinee->SetPosition(KeyTime); 
	//	LobbyMatinee->Play();		

	//	ABladeIIGameMode::SetCameraMatineeCam(LobbyMatinee, KeyTime);
	//	LobbyMatinee->Pause();		
	//}	
	//
	//if (SpawnedActors.Num() == 0)
	//{
	//	auto& DataStore = BladeIIGameImpl::GetClientDataStore();
	//	SetLeftEntry(&DataStore.GetLocalCharacterData(), DataStore.GetLocalCharacterData().GetMainPlayerClass(), DataStore.GetLocalCharacterData().GetSubPlayerClass());

	//	auto* LocalController = GameMode->GetLocalController();
	//	BII_CHECK(LocalController);

	//	LocalController->ClientSetCameraFade_Implementation(true, FColor::Black, FVector2D(1.f, 0.f), 1.f, false);
	//}
	//else //捞惑捞 积败 促矫 概莫府涅胶飘甫 朝赴 版快
	//{
	//	for (int32 idx = 2; idx < SpawnedActors.Num(); ++idx)
	//	{
	//		SpawnedActors[idx]->ConditionalBeginDestroy();
	//	}

	//	if (SpawnedActors.IsValidIndex(2))
	//		SpawnedActors.RemoveAt(2, SpawnedActors.Num() - 2);
	//}

	//auto* PVPSceneFindMatchWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::PVPOneOnOneFindMatch);
	//if (PVPSceneFindMatchWidget)
	//{
	//	PVPSceneFindMatchWidget->SetEnableCancel(true);
	//}
}

void FPVPMatchSceneManager::SetReady()
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetReady"));

	//WaitingReadyToMatchTime = 4.5f; //temporary
	//ElapsedWaitingReadyToMatchTime = 0.f;

	//auto* PVPSceneFindMatchWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::PVPOneOnOneFindMatch);
	//if (PVPSceneFindMatchWidget)
	//{
	//	PVPSceneFindMatchWidget->SetReady(WaitingReadyToMatchTime);
	//}

	////澜.. 蝶肺 哗扁绰 蓖满篮 累诀捞绊 杭俘档 累栏聪 弊成 窍靛内靛肺..
	//BII_CHECK(SpawnedActors.Num() == 4 && AnimArray.Num() == 4);
	//if (SpawnedActors.Num() == 4 && AnimArray.Num() == 4)
	//{
	//	auto& DataStore = BladeIIGameImpl::GetClientDataStore();
	//	SpawnedActors[0]->GetSkeletalMeshComponent()->PlayAnimation(AnimArray[PCClassToInt(DataStore.GetLocalCharacterData().GetMainPlayerClass())].Ready_L, false);
	//	SpawnedActors[1]->GetSkeletalMeshComponent()->PlayAnimation(AnimArray[PCClassToInt(DataStore.GetLocalCharacterData().GetSubPlayerClass())].Ready_L, false);

	//	SpawnedActors[2]->GetSkeletalMeshComponent()->PlayAnimation(AnimArray[PCClassToInt(RemoteMainClass)].Ready_R, false);
	//	SpawnedActors[3]->GetSkeletalMeshComponent()->PlayAnimation(AnimArray[PCClassToInt(RemoteSubClass)].Ready_R, false);
	//}	
}

void FPVPMatchSceneManager::SetInGameReady()
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetInGameReady"));

	//if (LobbyMatinee)
	//{
	//	LobbyMatinee->Stop();
	//}

	//for (auto* SpawnedActor : SpawnedActors)
	//{
	//	SpawnedActor->Destroy();
	//}
	//SpawnedActors.Empty();

	//ChangeScene(true);

	//// 扁夯 霸烙葛靛 风凭俊辑 BeginPlay 矫痢俊 窍绰 老何 悼累阑 角力 鞘夸茄 饭骇捞 胶飘府怪 登绊 沥焊啊 盲况柳 促澜 促矫 荐青.
	//GameMode->B2GamePreloadMatineeRefAssets();
	//if (GameMode->ShouldSetupPreRender())
	//{
	//	GameMode->DelayedSetupPreRender();
	//}

	//OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::PVP_Tag);

	//if (GameMode)
	//	GameMode->SetMatchState(MatchState::InProgress);

	//SetState(EPVPMatchState::PreBattle);
}

void FPVPMatchSceneManager::SetPreBattle()
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetPreBattle"));

	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::PVPOneOnOneBattleMain);

	//TimeLimitBattle = LeftTimeBattle = 90.f;

	//auto* PVPSceneWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVP1on1BattleMain>(UIFName::PVPOneOnOneBattleMain);
	//if (PVPSceneWidget && GameMode)
	//{
	//	PVPSceneWidget->SetLocalPlayerToLeft(GameMode->IsLeftLocalPlayer());
	//	PVPSceneWidget->SetLeftTime(TimeLimitBattle);
	//}
}

void FPVPMatchSceneManager::SetBattle()
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetBattle"));

	ClosePreLoadingScreenClass<EPreLoadingScreenType>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin);
	
	if(GameMode)
		GameMode->StartPVPGame();
}

void FPVPMatchSceneManager::SetBattleEnd()
{
	ENetMatchResult result(ENetMatchResult::LocalLose);
	//if (auto* DocSome = UB2UIDocHelper::GetDocSome())
	//{
	//	result = DocSome->GetLocalPlayerMatchResult();
	//}

	//if (auto* PVPSceneWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVP1on1BattleMain>(UIFName::PVPOneOnOneBattleMain))
	//{
	//	//秦榜 酒捞能栏肺 官层林扁
	//	if (CurrentState == EPVPMatchState::BattleEnd)
	//	{
	//		PVPSceneWidget->SetDeathPortrait(result);
	//	}
	//		
	//	//[@SameLine, 180508] 咯扁辑 龋免窍搁 Dead付萍聪 吝俊 response甫 罐澜 Dead付萍聪 场唱绊 龋免
	//	//GameMode->EndMatchByDie();
	//}

	//if (result == ENetMatchResult::LocalWin)
	//{
	//	auto* DocBattleRival = UB2UIDocHelper::GetDocPVP1on1Rival();
	//	if (DocBattleRival)
	//	{
	//		DocBattleRival->SetCurrPCHealth(0.0f);
	//		DocBattleRival->SetTagPCHealth(0.0f);
	//	}
	//}
	//else if (result == ENetMatchResult::LocalLose)
	//{
	//	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	//	if (DocBattle)
	//	{
	//		DocBattle->SetCurrPCHealth(0.0f);
	//		DocBattle->SetTagPCHealth(0.0f);
	//	}
	//}
	//else if (result == ENetMatchResult::Draw)
	//{
	//	auto* DocBattleRival = UB2UIDocHelper::GetDocPVP1on1Rival();
	//	if (DocBattleRival)
	//	{
	//		DocBattleRival->SetCurrPCHealth(0.0f);
	//		DocBattleRival->SetTagPCHealth(0.0f);
	//	}

	//	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	//	if (DocBattle)
	//	{
	//		DocBattle->SetCurrPCHealth(0.0f);
	//		DocBattle->SetTagPCHealth(0.0f);
	//	}
	//}
}

void FPVPMatchSceneManager::SetStartTimeCount(bool InStart)
{
	//IsStartTimeCount = InStart;

	//if(IsStartTimeCount)
	//	TimeBeginPlay = FApp::GetCurrentTime();
	//
	//
}

void FPVPMatchSceneManager::ShowResult()
{
	ShowResultUiImage();
}

void FPVPMatchSceneManager::ShowResultUiImage()
{
	/*if (auto* PVPSceneWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVP1on1BattleMain>(UIFName::PVPOneOnOneBattleMain))
	{
		ENetMatchResult result(ENetMatchResult::LocalLose);

		if (auto* DocSome = UB2UIDocHelper::GetDocSome())
			result = DocSome->GetLocalPlayerMatchResult();

		PVPSceneWidget->ShowResultImage(result);
	}*/
}

void FPVPMatchSceneManager::SetEndMatchByTime()
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetEndMatchByTime"));

	check(GameMode);
	GameMode->EndMatchByBattleTime();
}

void FPVPMatchSceneManager::ChangeScene(bool bStateFromReadyToBattle)
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::ChangeScene"));
//
//#if WITH_EDITOR
//	if (GIsEditor && GbEditorPVPGamePreview){
//		if (FindingMatchLevelPackageName == NAME_None || BattleMatchLevelPackageName == NAME_None){
//			return;
//		}
//	}
//#endif
//
//	BII_CHECK(FindingMatchLevelPackageName != NAME_None);
//	BII_CHECK(BattleMatchLevelPackageName != NAME_None);
//
//	FName ToUnloadLevel = bStateFromReadyToBattle ? FindingMatchLevelPackageName : BattleMatchLevelPackageName;
//	FName ToLoadLevel = !bStateFromReadyToBattle ? FindingMatchLevelPackageName : BattleMatchLevelPackageName;
//
//	auto* World = GameMode->GetWorld();
//	if (!World)
//		return;
//
//	bool OldMatchStarted = World->bMatchStarted;
//	World->bMatchStarted = false;
//	bool OldForceVisibleStreamingLevels = World->bShouldForceVisibleStreamingLevels;
//	World->bShouldForceVisibleStreamingLevels = false;
//
//	{
//		ULevelStreaming* StreamingLevel = World->GetLevelStreamingForPackageName(ToUnloadLevel);
//
//		BII_CHECK(StreamingLevel);
//
//		if (StreamingLevel)
//		{
//			StreamingLevel->bShouldBeVisible = false;
//			StreamingLevel->bShouldBeLoaded = false;
//		}
//	}
//
//	//SimpleDirectionalLight regist 巩力肺 RemoveFromWorld甫 刚历 阂矾具凳.
//	World->FlushLevelStreaming();
//	B2GMLoadingProgCollector::StepCustom(0.5f); // 肺爹 柳青档. 夸 PVP 狼 漂喊茄 备籍牢单.. 促弗 葛靛甸捞 措眉肺 捞繁 侥牢 扒 酒聪绊..
//
//	{
//		ULevelStreaming* StreamingLevel = World->GetLevelStreamingForPackageName(ToLoadLevel);
//
//		BII_CHECK(StreamingLevel);
//
//		if (StreamingLevel)
//		{
//			StreamingLevel->bShouldBeVisible = true;
//			StreamingLevel->bShouldBeLoaded = true;
//
//			if (!StreamingLevel->GetLoadedLevel())
//			{
//				StreamingLevel->bShouldBlockOnLoad = true;
//			}
//		}	
//	}
//
//	World->FlushLevelStreaming();
//	World->bMatchStarted = OldMatchStarted;
//	World->bShouldForceVisibleStreamingLevels = OldForceVisibleStreamingLevels;
//
//	if (GameMode)
//	{ // 饭骇 府家胶 匙捞萍宏 饭欺繁胶 某教 鸥捞怪.
//		GameMode->RecacheLevelResources();
//	}
//
//	GameMode->UpdateManagedActorList(); // 饭骇 胶飘府怪栏肺 牢秦 甘捞 货肺 棵扼坷搁 Actor 府胶飘甫 诀单捞飘 秦 林绢具 窃.
//
//	FlushRenderingCommands();
}

void FPVPMatchSceneManager::SetLeftEntry(ICharacterDataStore* PlayerCharacterDataStore, EPCClass MainClass, EPCClass SubClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetLeftEntry"));

	//check(PlayerCharacterDataStore);
	//check(SpawnedActors.Num() == 0);
	//	
	//ASkeletalMeshActor* EntryActor = NULL;

	//FMatchingAnims* MatchingAnimInfo = NULL;
	//
	//MatchingAnimInfo = AnimArray.IsValidIndex(PCClassToInt(MainClass)) ? &AnimArray[PCClassToInt(MainClass)] : NULL;

	//if (!MatchingAnimInfo)
	//	return;

	//EntryActor = SpawnPuppetActor(TEXT("Left_Main"), LobbyMatinee, MainClass, PlayerCharacterDataStore, MatchingAnimInfo->Idle);
	//if (EntryActor)
	//{
	//	FVector NewLocation = EntryActor->GetActorLocation();
	//	NewLocation.Z = CharacterLocationZ[PCClassToInt(MainClass)];
	//	EntryActor->SetActorLocation(NewLocation);
	//	SpawnedActors.Add(EntryActor);
	//}

	//MatchingAnimInfo = AnimArray.IsValidIndex(PCClassToInt(SubClass)) ? &AnimArray[PCClassToInt(SubClass)] : NULL;

	//if (!MatchingAnimInfo)
	//	return;

	//EntryActor = SpawnPuppetActor(TEXT("Left_Sub"), LobbyMatinee, SubClass, PlayerCharacterDataStore, MatchingAnimInfo->Idle);
	//if (EntryActor)
	//{
	//	FVector NewLocation = EntryActor->GetActorLocation();
	//	NewLocation.Z = CharacterLocationZ[PCClassToInt(SubClass)];
	//	EntryActor->SetActorLocation(NewLocation);
	//	SpawnedActors.Add(EntryActor);
	//}

	//if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	//{
	//	DocBattle->SetUserNickName(PlayerCharacterDataStore->GetUserNickName());
	//}
}

void FPVPMatchSceneManager::SetRightEntry(ICharacterDataStore* PlayerCharacterDataStore, EPCClass MainClass, EPCClass SubClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("FPVPMatchSceneManager::SetRightEntry"));

	//check(PlayerCharacterDataStore);
	//check(SpawnedActors.Num() == 2);

	//ASkeletalMeshActor* EntryActor = NULL;

	//FMatchingAnims* MatchingAnimInfo = NULL;

	//MatchingAnimInfo = AnimArray.IsValidIndex(PCClassToInt(MainClass)) ? &AnimArray[PCClassToInt(MainClass)] : NULL;

	//if (!MatchingAnimInfo)
	//	return;

	//EntryActor = SpawnPuppetActor(TEXT("Right_Main"), LobbyMatinee, MainClass, PlayerCharacterDataStore, MatchingAnimInfo->Idle);
	//if (EntryActor)
	//{
	//	FVector NewLocation = EntryActor->GetActorLocation();
	//	NewLocation.Z = CharacterLocationZ[PCClassToInt(MainClass)];
	//	EntryActor->SetActorLocation(NewLocation);
	//	SpawnedActors.Add(EntryActor);
	//}

	//MatchingAnimInfo = AnimArray.IsValidIndex(PCClassToInt(SubClass)) ? &AnimArray[PCClassToInt(SubClass)] : NULL;

	//if (!MatchingAnimInfo)
	//	return;

	//EntryActor = SpawnPuppetActor(TEXT("Right_Sub"), LobbyMatinee, SubClass, PlayerCharacterDataStore, MatchingAnimInfo->Idle);
	//if (EntryActor)
	//{
	//	FVector NewLocation = EntryActor->GetActorLocation();
	//	NewLocation.Z = CharacterLocationZ[PCClassToInt(MainClass)];
	//	EntryActor->SetActorLocation(NewLocation);
	//	SpawnedActors.Add(EntryActor);
	//}		

	//RemoteMainClass	= MainClass;
	//RemoteSubClass	= SubClass;

	//int32 PowerValue = 0;
	//int32 AttackPower = 0;
	//int32 Defense = 0;

	//if (GameMode && GameMode->IsPvPTutorial() && GameMode->GetLocalPlayer())
	//{
	//	auto& LocalCharacterData = BladeIIGameImpl::GetLocalCharacterData();
	//	PowerValue = CombatStatEval::GetPCCombatPower(LocalCharacterData.GetMainPlayerClass(), &LocalCharacterData);
	//	PowerValue += CombatStatEval::GetPCCombatPower(LocalCharacterData.GetSubPlayerClass(), &LocalCharacterData);
	//	PowerValue *= GameMode->GetDebuffValue();

	//	AttackPower = CombatStatEval::GetPCAttack(LocalCharacterData.GetMainPlayerClass(), &LocalCharacterData);
	//	AttackPower += CombatStatEval::GetPCAttack(LocalCharacterData.GetSubPlayerClass(), &LocalCharacterData);
	//	AttackPower *= GameMode->GetDebuffValue();

	//	Defense = CombatStatEval::GetPCDefense(LocalCharacterData.GetMainPlayerClass(), &LocalCharacterData);
	//	Defense += CombatStatEval::GetPCDefense(LocalCharacterData.GetSubPlayerClass(), &LocalCharacterData);
	//	Defense *= GameMode->GetDebuffValue();
	//}
	//else
	//{
	//	PowerValue = CombatStatEval::GetPCCombatPower(MainClass, PlayerCharacterDataStore);
	//	PowerValue += CombatStatEval::GetPCCombatPower(SubClass, PlayerCharacterDataStore);

	//	AttackPower = CombatStatEval::GetPCAttack(MainClass, PlayerCharacterDataStore);
	//	AttackPower += CombatStatEval::GetPCAttack(SubClass, PlayerCharacterDataStore);

	//	Defense = CombatStatEval::GetPCDefense(MainClass, PlayerCharacterDataStore);
	//	Defense += CombatStatEval::GetPCDefense(SubClass, PlayerCharacterDataStore);
	//}

	//auto* DocRival = UB2UIDocHelper::GetDocPVP1on1Rival();
	//if (DocRival)
	//{
	//	DocRival->SetUserNickName(PlayerCharacterDataStore->GetUserNickName());
	//}
	//		
	//auto* FindingMatchUI = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::PVPOneOnOneFindMatch);
	//if (FindingMatchUI)
	//{
	//	if (DocRival)
	//		FindingMatchUI->SetUserData(false, PlayerCharacterDataStore->GetUserNickName(), PlayerCharacterDataStore->GetUserLevel(), DocRival->GetTagMatchGrade(), DocRival->GetTagMatchGradeStar(), DocRival->GetTagMatchCurrContinuousWins(), PowerValue,
	//		TEXT(""), 0, 0, 0, 0, AttackPower, Defense);
	//	else
	//		FindingMatchUI->SetUserData(false, PlayerCharacterDataStore->GetUserNickName(), PlayerCharacterDataStore->GetUserLevel(), 10, 2, 4, 1044,
	//		TEXT(""), 0, 0, 0, 0, AttackPower, Defense);
	//}
}

void AB2PVPGameMode::SetOpponentToken(const FGuid& Token)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetOpponentToken"));

	OpponentTokenToPlay = Token.ToString(EGuidFormats::DigitsWithHyphens).ToLower();	
}

void AB2PVPGameMode::DelayedSetupPreRender()
{
	ABladeIIGameMode::SetupPreRenderObjects(); // 漂喊洒 促弗 扁瓷阑 静妨绰 扒 酒聪绊 距埃 鸥捞怪 促福霸 秦辑 角青窍妨绊.
}

void AB2PVPGameMode::NotifyStageEventDirector_FinishShow(int32 EventType)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::NotifyStageEventDirector_FinishShow"));

	//EStageEvent eStageEvent = (EStageEvent)EventType;

	//switch (eStageEvent)
	//{
	//case EStageEvent::ESEV_FinishAttack:
	//	{
	//		EndMatchByDie();
	//	}break;
	//}
}

bool AB2PVPGameMode::IsValidFriendshipGame()
{
	bool HasValidInvitation = false;
	if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		HasValidInvitation = DocLobbyMatch->HasValidInvitation();

	bool IsFriendshipGame = false;
	//if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
	//	IsFriendshipGame = DocPVP->GetIsFriendshipGame();

	return HasValidInvitation && IsFriendshipGame;
}

bool AB2PVPGameMode::CanReceiveGameInvite(const FString& Inviter, EB2GameMode GameModeType) const
{
	if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	{
		const EInvitationState InvitationState = DocLobbyMatch->GetInvitationState();
		if (InvitationState == EInvitationState::Success)
		{
			const FB2InvitationInfo& InvitationInfo = DocLobbyMatch->GetInvitationInfo();
			if (InvitationInfo.Inviter == Inviter || InvitationInfo.Invitee == Inviter)
				return true;

		}
	}

	return false;
}

void AB2PVPGameMode::ClearInvitationGameInfo(EB2GameMode GameModeType)
{
	Super::ClearInvitationGameInfo(GameModeType);

	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
			DocPVP->ClearInvitationData();
	}
}

void AB2PVPGameMode::RequestGameInvite(const FString& NaickName, EB2GameMode GameModeType)
{
	// Request to server
	Super::RequestGameInvite(NaickName, GameModeType);
	/*if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		if (UB2UICommonInvite* InviteUI = UIManager->OpenUI<UB2UICommonInvite>(UIFName::InviteGame))
			InviteUI->OnRequestGameInvite(NaickName, GameModeType);
	}

	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		if (auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival())
			DocPVP->SetIsFriendshipGame(true);
	}

	LeaveRoomIfJoined();*/
}

void AB2PVPGameMode::PrevAcceptGameInvite(const FString& Inviter, EB2GameMode GameModeType)
{
	//// PVP葛靛俊辑 荐遏篮 官肺 GameStart甫 狼固
	//auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();
	//if (GameModeType == EB2GameMode::PVP_Tag && DocPVP)
	//{
	//	// CharacterSelectPage 捞悼 傈 Setting登绢 乐绢具 窃
	//	DocPVP->SetIsFriendshipGame(true);
	//	DocPVP->SetIsInvitee(true);
	//	DocPVP->SetIsInviterReady(true);

	//	RequestAcceptInvitation(GameModeType);
	//}
}

void AB2PVPGameMode::RequestDeclineInvitation(EB2GameMode GameModeType)
{
	Super::RequestDeclineInvitation(GameModeType);

	//if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	//{
	//	if (UB2UIPVPResult* ResultUI = UIManager->GetUI<UB2UIPVPResult>(UIFName::PVPResult))
	//		ResultUI->PreventRematch();
	//}
}

void AB2PVPGameMode::OnReceiveInvitation(const FString& Inviter, EB2GameMode GameModeType)
{
	//if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	//{
	//	if (UB2UICommonInvite* InviteUI = UIManager->OpenUI<UB2UICommonInvite>(UIFName::InviteGame))
	//		InviteUI->OnReceiveInvitation(Inviter, GameModeType);
	//}

	LeaveRoomIfJoined();
}

void AB2PVPGameMode::OnInvitationSuccess(const FString& Invitee, EB2GameMode GameModeType, const FString& InvitationRef)
{
	if (GetMatchState() == NetMatchState::StartResult)
	{
		RequestRematch();
	}
}

void AB2PVPGameMode::OnInvitationDeclined(const FString& Inviter, EB2GameMode GameModeType)
{
	//if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	//{
	//	if (UB2UICommonInvite* InviteUI = UIManager->GetUI<UB2UICommonInvite>(UIFName::InviteGame))
	//		InviteUI->OnReceiveDecline(Inviter);

	//	// 惑措规捞 芭例窍搁 Rematch 阜澜
	//	if (UB2UIPVPResult* ResultUI = UIManager->GetUI<UB2UIPVPResult>(UIFName::PVPResult))
	//		ResultUI->PreventRematch();
	//}
}

void AB2PVPGameMode::NotifyUIWidgetOpenComplete(FName InOpenedUI)
{
	Super::NotifyUIWidgetOpenComplete(InOpenedUI);

	if (InOpenedUI == UIFName::PVPResult)
		ShowCachedCompleteQuestNotify();
}

void AB2PVPGameMode::SetupNetwork(ABladeIIPlayer* Old, ABladeIIPlayer* New)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetupNetwork"));

	auto*	OldNetworkPlayer = Cast<ABladeIINetPlayer>(Old);
	auto*	NewNetworkPlayer = Cast<ABladeIINetPlayer>(New);

	if (OldNetworkPlayer && NewNetworkPlayer)
	{
		NewNetworkPlayer->SetNetId(OldNetworkPlayer->GetNetId());
	}
}

void AB2PVPGameMode::SetNetStatus(ABladeIIPlayer* New)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PVPGameMode::SetNetStatus"));

	auto*	NewNetworkPlayer = Cast<ABladeIINetPlayer>(New);
	check(NewNetworkPlayer);

	NewNetworkPlayer->SetNetStatus(ABladeIINetPlayer::NET_MASTER);
}


void AB2PVPGameMode::DeliveryMatchMaking(FB2TagMatchMakingPtr MatchInfo)
{
	if (!MatchInfo) return;

	if (auto* DocPvpMatch = UB2UIDocHelper::GetDocPVP1on1Rival())
	{
		int32 MatchPoint = MatchInfo->match_point;
		int32 AdditionalCount = MatchInfo->addadditional_match_count;

		DocPvpMatch->SetMatchPoint(MatchPoint);
		DocPvpMatch->SetAdditionalMatchCount(AdditionalCount);

		/*PvPDoc->SetMyMainPCLevel();
		PvPDoc->SetMyMainPCPower();
		PvPDoc->SetMyMainPCClass();

		PvPDoc->SetMySubPCLevel();
		PvPDoc->SetMySubPCPower();
		PvPDoc->SetMySubPCClass();

		PvPDoc->SetOpponentMainPCLevel(MatchInfo->);
		PvPDoc->SetOpponentMainPCPower();
		PvPDoc->SetOpponentMainPCClass();

		PvPDoc->SetOpponentSubPCLevel();
		PvPDoc->SetOpponentSubPCPower();
		PvPDoc->SetOpponentSubPCClass();*/

				
		//DocPvpMatch->SetUserAccountID(MatchInfo->ai_opponent->account->account_id);
		
	}

	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, MatchInfo->current_gem);

	GuildBuffSetttingClass<const TArray<b2network::B2GuildSkillPtr>&>::GetInstance().Signal(MatchInfo->guild_skills);
}
