#include "BladeII.h"
#include "B2StageDataStore.h"
#include "B2ClientDataStore.h"
#include "B2StageManager.h"
#include "Event.h"
//#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"

#include "B2PCClassInfo.h"
#include "B2ChapterInfo.h"
//#include "B2UIChapterFrame.h"
//#include "B2UILobbyMain.h"
//#include "B2UIMsgPopupReward.h"


#include "Global.h"
#include "EventSubsystem.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
//#include "B2UIEnterAdventure.h"

FName GetCombineKey(const int32 key1, const EStageDifficulty key2)
{
	BLADE2_SCOPE_CYCLE_COUNTER(Global_GetCombineKey);
	//// key : {ClientStageId}_{Difficulty} ex) 1_ESD_Normal
	//static UEnum* ePtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStageDifficulty"), true);
	//FString KeyString = FString::Printf(TEXT("%d_%s"), key1, *(ePtr->GetNameStringByIndex((int32)key2)));
	//return FName(*KeyString);
	return FName(TEXT(""));
}

FB2ActData::FB2ActData(FB2OriginGetActInfoPtr Info)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FB2ActData_FB2ActData);
	ServerData = Info;
};

FB2StageData::FB2StageData(FB2OriginGetStageInfoPtr Info)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FB2ActData_FB2StageData);
	ServerData = Info;
};


FStageDataStore::FStageDataStore()
{
	RequestedActId = -1;
	RequestedActDifficulty = EStageDifficulty::ESD_None;
	RequestedServerStageId = FServerStageID();
	RequestedActIdForChapterClearReward = -1;
	RequestedDifficultyForChapterClearReward = EStageDifficulty::ESD_None;
	RequestedActIdForRepeatClearReward = -1;
	RequestedDifficultyForRepeatClearReward = EStageDifficulty::ESD_None;
		
	bChangeChapterShortCut = false;
	ShortCutSvrStageID = FServerStageID();

	bHasRequestedActInfo = false;
	bHasRequestedActInfoWithChangeSceneReserve = false;

	SubscribeEvents();
}

void FStageDataStore::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryGetActInfoClass<FB2OriginGetActInfoPtr>::GetInstance().Subscribe2(
		[this](FB2OriginGetActInfoPtr ActInfo)
	{
		this->ResponseGetActInfo(ActInfo);
	}
	));
	Issues.Add(DeliveryGetStageInfoClass<FB2OriginGetStageInfoPtr>::GetInstance().Subscribe2(
		[this](FB2OriginGetStageInfoPtr StageInfo)
	{
		this->ResponseGetStageInfo(StageInfo);
	}
	));
	Issues.Add(DeliveryGetActRepeatClearRewardClass<FB2GetReceiveActRepeatClearReward>::GetInstance().Subscribe2(
		[this](FB2GetReceiveActRepeatClearReward RepeatClearRewardInfo)
	{
		this->ResponseRepeatMissionClearReward(RepeatClearRewardInfo);
	}
	));
	Issues.Add(DeliveryGetActClearRewardClass<FB2GetReceiveActClearReward>::GetInstance().Subscribe2(
		[this](FB2GetReceiveActClearReward ClearRewardInfo)
	{
		this->ResponseChapterClearReward(ClearRewardInfo);
	}
	));
}

void FStageDataStore::UnsubscribeEvents()
{
	Issues.Empty();
}

void FStageDataStore::OnShutdownModule()
{
	UnsubscribeEvents();
}

void FStageDataStore::SetMasterData(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_SetMasterData);
	//StageGameplayDatas.Empty();
	//const TArray<b2network::B2mdStageInfoPtr>& AllmdStageInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_stage_infos_index, InMasterDatas);
	//for (const b2network::B2mdStageInfoPtr& ThisInfo : AllmdStageInfo)
	//{
	//	FStageInfoGameplayData NewElem(ThisInfo);
	//	StageGameplayDatas.Add(FServerStageID(ThisInfo->stage_id), NewElem); // 여기서 stage_id는 serverStageID
	//}

	//// Cache StageGameplayDatas Key : ServerStageID를 빠르게 검색하기위해 Map/StageDifficulty를 기준으로 새로운 키값을 만들어준다. (캐싱용)
	//ServerStageIdMappingKeys.Empty();
	//for (auto& stageInfo : StageGameplayDatas)
	//{
	//	FName hashKey = GetCombineKey(stageInfo.Value.ClientStageId, static_cast<EStageDifficulty>(stageInfo.Value.Difficulty));
	//	if (ServerStageIdMappingKeys.Contains(hashKey) == false)
	//		ServerStageIdMappingKeys.Add(hashKey, stageInfo.Key);
	//}
	//SetMDStageFixedDrops(InMasterDatas);
}

void FStageDataStore::SetMDStageFixedDrops(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_SetMDStageFixedDrops);
	/*auto& MasterDataInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_stage_fixed_drops_index, InMasterDatas);

	MD_StageFixedDrops.Empty();

	FStageFixedDropsInfo dropinfo;
	for (auto Elem : MasterDataInfo)
	{
		dropinfo.idx = Elem->idx;
		dropinfo.Iteminfo_idx = Elem->template_id;
		dropinfo.Min_Gain = Elem->min;
		dropinfo.Max_Gain = Elem->max;
		dropinfo.Rate = Elem->rate;

		MD_StageFixedDrops.Add(Elem->stage_id, dropinfo);
	}*/
}

void FStageDataStore::AddMDStageGamePlayData(FServerStageID InServerStageId, const FStageInfoGameplayData&  InStageInfoGamePlayData)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_AddMDStageGamePlayData);
	StageGameplayDatas.Add(InServerStageId, InStageInfoGamePlayData);
}

void FStageDataStore::RequestGetActInfoAndChangeChapter(int32 ActId, EStageDifficulty Difficulty, bool bShortCut, FServerStageID ShortCutServerStageID)
{
	// 일반적인 상황에서 UI Chapter 뷰 열 때 사용.
	// 데이터가 없을 시 서버에 요청하는 걸 겸함.

	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_RequestGetActInfoAndChangeChapter);
	ensure(ActId > 0);

	const int32 EventSceneActNumber = GetEventSceneChapter(ActId);;
	if (EventSceneActNumber != INDEX_NONE)
	{
		bShortCut = false;
		ActId = EventSceneActNumber;

		if (ShortCutServerStageID != 0)
			CachedServerStageIDForRequestGetActInfo = ShortCutServerStageID;
	}
	else
	{
		if (CachedServerStageIDForRequestGetActInfo != 0)
		{
			ShortCutServerStageID = CachedServerStageIDForRequestGetActInfo;
			CachedServerStageIDForRequestGetActInfo = FServerStageID();
		}
	}

	bChangeChapterShortCut = bShortCut;
	ShortCutSvrStageID = ShortCutServerStageID;
	
	if (ActId == 0)//비정상적인 방법으로 들어왔거나 서버접속이 지연되서 초기값 설정이 잘못되었거나..
		ActId = 1;

	RequestedActId = ActId; // 꼭 서버로 request 했단 의미는 아닌..
	RequestedActDifficulty = Difficulty;

	auto DocChapter = Cast<UB2UIDocChapter>(UB2UIDocHelper::GetDocChapter());
	if (DocChapter != nullptr)
	{
		DocChapter->SetCurStageDifficulty(RequestedActDifficulty);
		DocChapter->SetCurChapterNum(RequestedActId);
	}

	if (IsCachedActData(ActId, Difficulty))
	{
		ChangeChapterScene(); // 데이터가 있으니 지연 없이 바로 연다.
	}
	else
	{
		if (!bHasRequestedActInfo && !bHasRequestedActInfoWithChangeSceneReserve)
		{
			// 서버에 요청을 하면서.. Response 가 왔을 시 장면 교체에 대한 예약도 해 놓는다.
			bool bActuallyRequested = ConditionalRequestGetActInfo(ActId, Difficulty, bShortCut, ShortCutServerStageID);
			check(bActuallyRequested); // 실행 루트상 이렇게 되어야..
			if (bActuallyRequested)
			{
				bHasRequestedActInfoWithChangeSceneReserve = true;
			}
		}
	}
}

bool FStageDataStore::ConditionalRequestGetActInfo(int32 ActId, EStageDifficulty Difficulty, bool  bShortCut, FServerStageID ShortCutServerStageID)
{
	// 이걸 RequestGetActInfoAndChangeChapter 이전에 pre-fetching 으로 사용할 수 있게 하면 챕터뷰 여는 걸 추가 최적화 할 수 있다..
	// 지금은 약간의 손질이 필요.

	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_RequestGetActInfo);
	ensure(ActId > 0);

	if (ActId == 0)//비정상적인 방법으로 들어왔거나 서버접속이 지연되서 초기값 설정이 잘못되었거나..
		ActId = 1;

	if (!IsCachedActData(ActId, Difficulty) && !bHasRequestedActInfo) // 이전 요청에 대한 응답이 처리 안된 상태에서는 스킵.
	{
		bHasRequestedActInfo = true;
		RequestedActId = ActId; // 이 경우는 실제로 서버로 요청한 값이 되겠다.
		RequestedActDifficulty = Difficulty;

		//ActorID가 INDEX_NONE일 수도 있으니 여기서도 검사함
		ensure(ActId > 0);
		data_trader::Retailer::GetInstance().RequestGetActInfo(CliToSvrStageDifficulty(Difficulty), ActId);

		return true; // 실제 요청을 한 것에 대해 true 리턴.
	}
	return false;
}

void FStageDataStore::ResponseGetActInfo(FB2OriginGetActInfoPtr ActInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_ResponseGetActInfo);
	CacheActData(RequestedActId, RequestedActDifficulty, ActInfo);

	// Act관련 UIDoc 변경
	if (UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter())
	{
		if (UB2ChapterInfo* ChapterInfoTable = StaticFindChapterInfo())
		{
			FText StageDiffText = FText::FromString(FString::Printf(TEXT("Chapter_DiffColor_%d"), (int32)RequestedActDifficulty));
			DocChapter->SetCurChapterName(FText::Format(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, StageDiffText.ToString()),
					ChapterInfoTable->GetLocalizedChapterName(RequestedActId)));
		}
	}
	SetActRewardState(ActInfo);

	checkSlow(bHasRequestedActInfo);
	bHasRequestedActInfo = false;

	if (bHasRequestedActInfoWithChangeSceneReserve) // 데이터 요청하면서 챕터 뷰로의 변경도 예약했다면
	{
		bHasRequestedActInfoWithChangeSceneReserve = false;
		ChangeChapterScene();
	}
}

/** 기존 RequestGetActInfo 는 의미를 명확히 하고 기능을 구분하기 위해 대부분 RequestGetActInfoAndChangeChapter 로 대체됨.
 * 여기도 필요에 따라.. 지금은 이 RequestGetStageInfo 가 UI 전환 의미도 지니고 있음. */
void FStageDataStore::RequestGetStageInfo(FServerStageID ServerStageId, bool bInForceReqToServer)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_RequestGetStageInfo);
	// Note :	챕터(Act)데이터가 없으면 챕터를 먼저 가져오도록 하자.
	//			Act를 먼저 가져와야하는 이유는 액트 쪽에서 미리 셋팅되는 정보가 있기 때문 (반복던전 등등)
	int32 ActId = GetActIdByClientStageId(GetClientStageId(ServerStageId));
	EStageDifficulty Difficulty = GetStageDifficulty(ServerStageId);
		
	if (bInForceReqToServer && IsCachedActData(ActId, Difficulty) == false)
	{
		// 여기 응답에서 다시 RequestGetStageInfo 가 호출될 것임..
		// 대체 왜 이렇게 위험한 순환식 호출 구조가 된 건지 모르겠다.. 주의가 필요.. ㅠㅠ
		RequestGetActInfoAndChangeChapter(ActId, Difficulty, true, ServerStageId);
		return;
	}

	RequestedServerStageId = ServerStageId;

	// 이전에 FB2MessageInfoResponseGetStageInfo 가 static 데이터들로만 되어 있을 때에는 한번 받고 나면 캐싱한 걸 꺼내서 쓰기만 했으나 
	// 이제는 실행 도중 변경될 수 있는 데이터가 들어가서 다시 요청할 수 있도록 한다.
	if (!IsCachedStageData(ServerStageId) || bInForceReqToServer) {
		data_trader::Retailer::GetInstance().RequestGetStageInfo(ServerStageId);
	}
	else {
		// 여기서 반복전투 관련 처리는 하지 않는다.

		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		if (DocBS)
		{
			FRepeatBattleStateSet RepeatBattleRequestState;
			RepeatBattleRequestState.bIsOn = DocBS->GetRepeatBattleCurrentOn();
			RepeatBattleRequestState.CurrentRepeatCount = DocBS->GetRepeatBattleCurrentCount();
			RepeatBattleRequestState.bRepeatOne = DocBS->GetRepeatBattleLoopOne();
			RepeatBattleRequestState.bRepeatAll = DocBS->GetRepeatBattleLoopAll();
			RepeatBattleRequestState.bBoostOn = DocBS->GetRepeatBattleBoostOn();
			RepeatBattleRequestState.RemainingBoostCount = DocBS->GetRepeatBattleBoostRemainCount();

			GoGameStageInfoFromLobbyClass<FServerStageID, FRepeatBattleStateSet>::GetInstance().Signal(ServerStageId, FRepeatBattleStateSet());
		}
	}
}

void FStageDataStore::ResponseGetStageInfo(FB2OriginGetStageInfoPtr StageInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_ResponseGetStageInfo);
	CacheStageData(RequestedServerStageId, StageInfo);

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		FRepeatBattleStateSet RepeatBattleRequestState;
		RepeatBattleRequestState.bIsOn = DocBS->GetRepeatBattleCurrentOn();
		RepeatBattleRequestState.CurrentRepeatCount = DocBS->GetRepeatBattleCurrentCount();
		RepeatBattleRequestState.bRepeatOne = DocBS->GetRepeatBattleLoopOne();
		RepeatBattleRequestState.bRepeatAll = DocBS->GetRepeatBattleLoopAll();
		RepeatBattleRequestState.bBoostOn = DocBS->GetRepeatBattleBoostOn();
		// colosseum comment : 3배 모험 이용권
		// TODO : 기존은 누적 카운트를 리턴 했다면 현재는 보유중인 이용권 카운트를 리턴한다.
		RepeatBattleRequestState.RemainingBoostCount = StageInfo->stage_boost_ticket;
		BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::StageBoostTicket, StageInfo->stage_boost_ticket);

		const auto MaxCondition = static_cast<int32>(EStageClearCondition::MaxCondition);
		const auto ConstMask = static_cast<int32>(1);
		TArray<bool> StageClearConditions;

		for (int32 i = 0; i < MaxCondition; ++i)
		{
			const auto ClearCondition = (StageInfo->best_clear_mask & (static_cast<int32>(1) << i));
			StageClearConditions.Add(ClearCondition == 0 ? false : true);
		}
		DocBS->SetStageClearConditions(StageClearConditions);

		// 여기서 반복전투 관련 처리는 하지 않는다.
		GoGameStageInfoFromLobbyClass<FServerStageID, FRepeatBattleStateSet>::GetInstance().Signal(RequestedServerStageId, RepeatBattleRequestState);
	}
}


void FStageDataStore::SetActRewardState(FB2OriginGetActInfoPtr ActInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_SetActRewardState);
	if (ActInfo != nullptr)
	{
		CachedActStarRewards.Empty();
		// 받는 Reward 현황은 항상 최신으로 유지
		for (b2network::B2ActIdentifierPtr RewardInfo : ActInfo->has_reward_acts)
		{
			if (RewardInfo != nullptr)
			{
				FName ActKey = GetCombineKey(RewardInfo->act_id, static_cast<EStageDifficulty>(RewardInfo->difficulty));
				CachedActStarRewards.AddUnique(ActKey);
			}
		}
	}
}

void FStageDataStore::RemoveActRewardState(int32 ActID, EStageDifficulty Difficulty)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_RemoveActRewardState);
	CachedActStarRewards.Remove(GetCombineKey(ActID, Difficulty));
}

void FStageDataStore::StartStage(const FB2StartStageInfo& StartStageInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_StartStage);
	B2_SCOPED_TRACK_LOG(TEXT("FStageDataStore::StartStage"));

	BladeIIGameImpl::GetClientDataStore().StartStage(StartStageInfo);
}

void FStageDataStore::ClearStage(const FB2ClearStageInfo& ClearStageInfo, FServerStageID ServerStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_ClearStage);
	//B2_SCOPED_TRACK_LOG(TEXT("FStageDataStore::ClearStage"));

	//SetActClearStageData(ServerStageId, GET_TUPLE_DATA(FB2ResponseClearStage::clear_grade_index, ClearStageInfo));
	//CheckChangeLastClearStage(BladeIIGameImpl::GetClientDataStore().GetAccountInfo(), 
	//	GET_TUPLE_DATA(FB2ResponseClearStage::account_index, ClearStageInfo));

	//BladeIIGameImpl::GetClientDataStore().ClearStage(ClearStageInfo, ServerStageId);
}

void FStageDataStore::ChangeChapterScene()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_ChangeChapterScene);
	//if (bChangeChapterShortCut)
	//{
	//	//if (ShortCutSvrStageID == INDEX_NONE)
	//		RequestGetStageInfo(ShortCutSvrStageID, true);

	//	return;
	//}

	//auto* UIManager = UB2UIManager::GetInstance();
	//if (UIManager)
	//{
	//	if (UIManager->IsUIScene(EUIScene::LobbyMain))
	//	{
	//		auto* LobbyMain = UIManager->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	//		if (LobbyMain)
	//			LobbyMain->ChangeChaperScene(RequestedActId > 0 ? RequestedActId : 1);
	//	}
	//	else if (UIManager->IsUIScene(EUIScene::Chapter))
	//	{
	//		auto* Chapter = UIManager->GetUI<UB2UIChapterFrame>(UIFName::Chapter);
	//		if (Chapter)
	//			Chapter->ChangeChapter(RequestedActId, RequestedActDifficulty);
	//	}
	//	else if (UIManager->IsUIScene(EUIScene::AdventureModSelect))
	//	{
	//		if (auto* Chapter = UIManager->OpenUI<UB2UIChapterFrame>(UIFName::Chapter))
	//			Chapter->ChangeChapter(RequestedActId, RequestedActDifficulty);
	//	}
	//	else if (UIManager->IsUIScene(EUIScene::AdventureEnterBattleMain))
	//	{
	//		auto* AdventureSelect = UIManager->GetUI<UB2UIEnterAdventure>(UIFName::AdventureEnterBattleMain);
	//		if (AdventureSelect)
	//			AdventureSelect->ChangeChaperScene(RequestedActId > 0 ? RequestedActId : 1);
	//	}
	//	else
	//	{
	//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::Chapter);
	//	}
	//}
}

void FStageDataStore::CacheActData(int32 ActId, EStageDifficulty Difficulty, FB2OriginGetActInfoPtr ActInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_CacheActData);
	CachedActDatas.Add(GetCombineKey(ActId, Difficulty), FB2ActData(ActInfo));
}

bool FStageDataStore::IsCachedActData(int32 ActId, EStageDifficulty Difficulty)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_IsCachedActData);
	return (CachedActDatas.Find(GetCombineKey(ActId, Difficulty)) != nullptr);
}

void FStageDataStore::CacheStageData(FServerStageID ServerStageId, FB2OriginGetStageInfoPtr StageInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_CacheStageData);
	CachedStageDatas.Remove(ServerStageId); // Add 하면 덮어쓰긴 하지만 Remove 먼저 하는 게 보다 확실함. 뭔가 좀 다름.
	CachedStageDatas.Add(ServerStageId, FB2StageData(StageInfo));
}

int32 FStageDataStore::GetActIdByClientStageId(int32 ClientStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetActIdByClientStageId);
	UB2StageInfo* Info = StaticFindStageInfo();
	if (Info)
	{
		return Info->GetChapterNumByClientStageId(ClientStageId);
	}

	return 0;
}

int32 FStageDataStore::GetActIdByServerStageId(FServerStageID InServerStageId)
{
	return GetActIdByClientStageId(GetClientStageId(InServerStageId));
}

int32 FStageDataStore::GetClientStageIdOfChapterNums(int32 ChapterNum, int32 SubChapterNum)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetClientStageIdOfChapterNums);
	UB2StageInfo* Info = StaticFindStageInfo();
	if (Info)
	{
		return Info->GetClientStageIdOfChapterNums(ChapterNum, SubChapterNum);
	}

	return 0;
}

int32 FStageDataStore::GetSubChapterNumByClientStageId(int32 ClientStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetSubChapterNumByClientStageId);
	UB2StageInfo* Info = StaticFindStageInfo();
	if (Info)
	{
		return Info->GetSubChapterNumByClientStageId(ClientStageId);
	}

	return 0;
}

int32 FStageDataStore::GetSubChapterNumByServerStageId(FServerStageID InServerStageId)
{
	return GetSubChapterNumByClientStageId(GetClientStageId(InServerStageId));
}

void FStageDataStore::GetActRewardData(int32 ActId, EStageDifficulty Difficulty, TArray<FUIChapterStarReward>& OutRewardData)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetActRewardData);
	OutRewardData.Empty();
	auto* ActInfo = CachedActDatas.Find(GetCombineKey(ActId, Difficulty));
	if (ActInfo && ActInfo->ServerData)
	{
		int32 CurrentGradeStarCount = 0;
		for (auto& ClearInfo : ActInfo->ServerData->stage_clear_infos)
		{
			CurrentGradeStarCount += ClearInfo->clear_grade;
		}

		for (auto& RewardInfo : ActInfo->ServerData->reward_infos)
		{
			OutRewardData.Add(FUIChapterStarReward(RewardInfo->require_star_count, CurrentGradeStarCount, RewardInfo->received));
		}
	}
}

void FStageDataStore::GetActRewardPreviewData(int32 InChapterID, EStageDifficulty InDifficulty, int32 InRewardIndex, TArray<b2network::B2RewardPtr>& OutRewards)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetActRewardPreviewData);
	FB2ActData* ActInfo = CachedActDatas.Find(GetCombineKey(InChapterID, InDifficulty));
	if (ActInfo && ActInfo->ServerData)
	{
		for (b2network::B2ActRewardInfoPtr& ThisInfo : ActInfo->ServerData->reward_infos)
		{
			if (ThisInfo && ThisInfo->reward_index == InRewardIndex)
			{
				OutRewards = ThisInfo->rewards;
				return;
			}
		}
	}
}

bool FStageDataStore::HasAnyRewardToReceive(int32 ActID, EStageDifficulty Difficulty)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_HasAnyRewardToReceive);
	const FName ActRewardKey = GetCombineKey(ActID, Difficulty);
	int32 FindIndex = INDEX_NONE;
	if (CachedActStarRewards.Find(ActRewardKey, FindIndex))
	{
		return true;
	}

	return false;
}

bool FStageDataStore::HasAnyRewardToReceiveAllChapter(EStageDifficulty Difficulty)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_HasAnyRewardToReceiveAllChapter);
	if (UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter())
	{
		const int32 MaxActID = DocChapter->GetChapterMaxNum();
		for (int32 ActID = 1; ActID <= MaxActID; ActID++)
		{
			if (HasAnyRewardToReceive(ActID, Difficulty))
				return true;
		}
	}

	return false;
}

void FStageDataStore::GetStageRewardPreviewItems(FServerStageID ServerStageId, TArray<FB2RewardItemPreviewInfo>& OutRewardInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageRewardPreviewItems);
	OutRewardInfo.Empty();

	auto* StageInfo = CachedStageDatas.Find(ServerStageId);
	if (StageInfo && StageInfo->ServerData)
	{
		for (auto& RewardItem : StageInfo->ServerData->reward_item_infos)
		{
			int32 index = OutRewardInfo.AddDefaulted();
			if (index != INDEX_NONE)
			{
				//그룹 대표 아이콘은 가상의 아이템으로 그룹아이디와 타입 조합으로 ID를 가진다. 그렇지만 나름 ItemInfo 테이블에 등록되어 있음.
				int32 RewardRepresentativeRefId = FItemRefIDHelper::GenerateRewardRepresentativeItemID(RewardItem->group_id, RewardItem->type);

				FB2RewardItemPreviewInfo& ThisInfo = OutRewardInfo[index];

				ThisInfo.SomeCommonData.ItemRefID = RewardRepresentativeRefId;
				ThisInfo.SomeCommonData.StarGrade = RewardItem->grade;
				ThisInfo.SomeCommonData.ItemClass = SvrToCliItemClass(RewardItem->type);
				ThisInfo.SomeCommonData.PrimaryPointType = SvrToCliPrimPointType(RewardItem->main_attribute);

				//툴팁으로 보여질 추가 데이터
				ThisInfo.PrimPointMinValue = RewardItem->main_attribute_value_min;
				ThisInfo.PrimPointMaxValue = RewardItem->main_attribute_value_max;
				ThisInfo.RandomOptionCount = RewardItem->random_option_count;

				ThisInfo.MinGrade = RewardItem->grade;
				ThisInfo.MaxGrade = RewardItem->grade;
			}
		}
	}
}

void FStageDataStore::GetStageRewardPreviewItemsOfSameType(TArray<FB2RewardItemPreviewInfo>& OutRewardInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageRewardPreviewItemsOfSameType);
	TArray<FB2RewardItemPreviewInfo> TempRewardInfo;
	TempRewardInfo.Empty();

	for (int i = 0; i < OutRewardInfo.Num(); i++)
	{
		bool bIsSameType = false;

		for (int j = 0; j < TempRewardInfo.Num(); j++)
		{
			if (OutRewardInfo[i].SomeCommonData.ItemRefID == TempRewardInfo[j].SomeCommonData.ItemRefID)
			{
				bIsSameType = true;

				TempRewardInfo[j].MinGrade = (TempRewardInfo[j].MinGrade < OutRewardInfo[i].MinGrade) ? TempRewardInfo[j].MinGrade : OutRewardInfo[i].MinGrade;
				TempRewardInfo[j].MaxGrade = (TempRewardInfo[j].MaxGrade > OutRewardInfo[i].MaxGrade) ? TempRewardInfo[j].MaxGrade : OutRewardInfo[i].MaxGrade;
			}
		}

		if (!bIsSameType)
		{
			TempRewardInfo.Add(OutRewardInfo[i]);
		}
	}

	OutRewardInfo = TempRewardInfo;
}

void FStageDataStore::GetStageAetherPreviewItems(FServerStageID ServerStageId, TArray<FB2AetherItemPreviewInfo>& OutAetherInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageAetherPreviewItems);
	OutAetherInfo.Empty();

	auto* StageInfo = CachedStageDatas.Find(ServerStageId);
	if (StageInfo && StageInfo->ServerData)
	{
		for (auto& AetherItem : StageInfo->ServerData->reward_aether_infos)
		{
			int32 index = OutAetherInfo.AddDefaulted();
			if (index != INDEX_NONE)
			{
				FB2AetherItemPreviewInfo& ThisInfo = OutAetherInfo[index];

				ThisInfo.AetherType = AetherItem->type;
				ThisInfo.AetherTiers = AetherItem->tiers;	// M20에서 사용 안함 추후 팝업에 나올때 사용 (봉마, 불멸 같은거..)
				ThisInfo.AetherGrades = AetherItem->grades;

				int32 MinGrade = 99;
				int32 MaxGrade = 0;

				for (int i = 0; i < ThisInfo.AetherGrades.Num(); i++)
				{
					MinGrade = (AetherItem->grades[i] < MinGrade) ? AetherItem->grades[i] : MinGrade;
					MaxGrade = (AetherItem->grades[i]) > MaxGrade ? AetherItem->grades[i] : MaxGrade;

					ThisInfo.MinGrade = MinGrade;
					ThisInfo.MaxGrade = MaxGrade;
				}
			}
		}
	}
}

void FStageDataStore::GetStageMaterialPreviewItems(FServerStageID ServerStageId, TArray<FB2MaterialItemPreviewInfo>& OutMaterialInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageMaterialPreviewItems);
	OutMaterialInfo.Empty();

	auto* StageInfo = CachedStageDatas.Find(ServerStageId);
	if (StageInfo && StageInfo->ServerData)
	{
		for (auto& MaterialItem : StageInfo->ServerData->material_items)
		{
			int32 index = OutMaterialInfo.AddDefaulted();
			if (index != INDEX_NONE)
			{
				FB2MaterialItemPreviewInfo& ThisInfo = OutMaterialInfo[index];

				ThisInfo.ItemRefID = MaterialItem->template_id;
				ThisInfo.ItemDropCount = MaterialItem->count;
			}
		}
	}
}

void FStageDataStore::GetStageCostData(FServerStageID ServerStageId, int32& OutRepeatBattleCostRefId, int32& OutRepeatBattleCostCount, int32& OutRequireStamina)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageCostData);
	OutRepeatBattleCostRefId = OutRepeatBattleCostCount = OutRequireStamina = 0;
	auto* StageInfo = CachedStageDatas.Find(ServerStageId);
	if (StageInfo && StageInfo->ServerData)
	{
		//현재는 골드로만
		OutRepeatBattleCostRefId = FItemRefIDHelper::GetGoodsID_Gold();
		OutRepeatBattleCostCount = StageInfo->ServerData->auto_repeat_battle_cost;

		OutRequireStamina = StageInfo->ServerData->need_blade_point;
	}
}

void FStageDataStore::GetStageSuggestedCombatStats(FServerStageID ServerStageId, float& OutSuggestedAttack, float& OutSuggestedDefense)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageSuggestedCombatStats);
	FStageInfoGameplayData* ThisStageInfo = StageGameplayDatas.Find(ServerStageId);
	if (ThisStageInfo)
	{
		OutSuggestedAttack = ThisStageInfo->SuggestedAttack;
		OutSuggestedDefense = ThisStageInfo->SuggestedDefense;
	}
}

void FStageDataStore::GetStageSuggestedCombatStats(FServerStageID ServerStageId, int32& OutRecommentPower)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageSuggestedCombatStats);
	FStageInfoGameplayData* ThisStageInfo = StageGameplayDatas.Find(ServerStageId);
	if (ThisStageInfo)
	{
		OutRecommentPower = ThisStageInfo->RecommentPower;
	}
}

void FStageDataStore::GetStageRecommendAbility(FServerStageID ServerStageId, int32& OutRecommentAttack, int32& OutRecommentDefense)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageRecommendAbility);
	FStageInfoGameplayData* ThisStageInfo = StageGameplayDatas.Find(ServerStageId);
	if (ThisStageInfo)
	{
		OutRecommentAttack = ThisStageInfo->RecommendAttack;
		OutRecommentDefense = ThisStageInfo->RecommendDefense;
	}
}

FServerStageID FStageDataStore::GetServerStageID(int32 ClientStageId, EStageDifficulty Difficulty)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetServerStageID);
	FServerStageID* ServerStageID = ServerStageIdMappingKeys.Find(GetCombineKey(ClientStageId, Difficulty));
	if (ServerStageID != nullptr)
		return (*ServerStageID);

	return FServerStageID();
}

int32 FStageDataStore::GetClientStageId(FServerStageID ServerStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetClientStageId);
	FStageInfoGameplayData* ThisStageInfo = StageGameplayDatas.Find(ServerStageId);
	if (ThisStageInfo)
		return ThisStageInfo->ClientStageId;

	return 0;
}

EStageDifficulty FStageDataStore::GetStageDifficulty(FServerStageID ServerStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageDifficulty);
	FStageInfoGameplayData* ThisStageInfo = StageGameplayDatas.Find(ServerStageId);
	if (ThisStageInfo)
		return static_cast<EStageDifficulty>(ThisStageInfo->Difficulty);

	return EStageDifficulty::ESD_Normal;
}


void FStageDataStore::RequestCachedGetActInfoAndChangeChapter()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_RequestCachedGetActInfo);
	if (CachedServerStageIDForRequestGetActInfo != 0)
	{
		int32 CachedActID = GetActIdByClientStageId(GetClientStageId(CachedServerStageIDForRequestGetActInfo));
		EStageDifficulty CachedDifficulty = GetStageDifficulty(CachedServerStageIDForRequestGetActInfo);

		RequestGetActInfoAndChangeChapter(CachedActID, CachedDifficulty, true, CachedServerStageIDForRequestGetActInfo);
	}
}

int32 FStageDataStore::GetEventSceneChapter(const int32 RequestActNumber)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetEventSceneChapter);
	if (IsFirstActOpen(RequestActNumber) || IsFirstActClear(RequestActNumber))
	{
		const int32 PrevActID = RequestActNumber - 1;
		if (IsFirstActClear(PrevActID))
			return PrevActID;

		return RequestActNumber;
	}

	return INDEX_NONE;
}

bool FStageDataStore::IsFirstActClear(int32 ActNumber)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_IsFirstActClear);
	int32 LastClearStage = GetLastClearClientId(EStageDifficulty::ESD_Normal);
	if (ActNumber * 10 == LastClearStage && ActNumber > LoadClearAct())
		return true;

	return false;
}

bool FStageDataStore::IsFirstActOpen(int32 ActNumber)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_IsFirstActOpen);
	if (ActNumber == 1)	// 1은 Open 띄우지마
		return false;

	int32 LastClearStage = GetLastClearClientId(EStageDifficulty::ESD_Normal);
	int32 LastClearAct = LoadClearAct();

	bool bPrevActClear = (LastClearAct > 0) && (LastClearAct * 10 == LastClearStage);

	int32 LastOpenAct = LoadOpenAct();

	if (bPrevActClear)
	{
		if (ActNumber == LastOpenAct +1 && LastOpenAct == LoadClearAct())
			return true;
	}

	if (LastClearAct > LastOpenAct)
		SaveOpenAct(LastClearAct);
	
	return false;
}

void FStageDataStore::SaveClearAct(int32 ActNumber)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_SaveClearAct);
	GConfig->SetInt(TEXT("ChapterState"), *FString::Printf(TEXT("ClearChapterNum")), ActNumber, GB2UserSavedStateIni);
	GConfig->Flush(false, GB2UserSavedStateIni);
}

void FStageDataStore::SaveRecentlyStageInfo()
{
	BLADE2_SCOPE_CYCLE_COUNTER(SaveRecentlyStageInfo);
	
	if (RequestedServerStageId == 0)
		return;
	
	GConfig->SetInt(TEXT("ChapterState"), *FString::Printf(TEXT("RecentlyChapterNum")), RequestedActId, GB2UserSavedStateIni);

	int RequestClientStageId = GetClientStageId(RequestedServerStageId);
	GConfig->SetInt(TEXT("ChapterState"), *FString::Printf(TEXT("RecentlyStageNum")), RequestClientStageId, GB2UserSavedStateIni);
	GConfig->SetInt(TEXT("ChapterState"), *FString::Printf(TEXT("RecentlyDifficulty")), static_cast<int32>(RequestedActDifficulty), GB2UserSavedStateIni);

	GConfig->Flush(false, GB2UserSavedStateIni);
}


void FStageDataStore::SaveOpenAct(int32 ActNumber)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_SaveOpenAct);
	GConfig->SetInt(TEXT("ChapterState"), *FString::Printf(TEXT("OpenChapterNum")), ActNumber, GB2UserSavedStateIni);
	GConfig->Flush(false, GB2UserSavedStateIni);
}

void FStageDataStore::SaveOpenCahpter(int32 InChapter ,bool Instage)
{
	GConfig->SetBool(TEXT("ChapterState"), *FString::Printf(TEXT("OpenChater%d"), InChapter), Instage, GB2UserSavedStateIni);
	GConfig->Flush(false, GB2UserSavedStateIni);
}


bool FStageDataStore::LoadOpenCahpter(int32 InChapter)
{
	bool RetValue = false;
	GConfig->GetBool(TEXT("ChapterState"), *FString::Printf(TEXT("OpenChater%d"), InChapter), RetValue, GB2UserSavedStateIni);

	return RetValue;
}

void FStageDataStore::SaveCBTClear(bool InbClear)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_SaveCBTClear);
	GConfig->SetBool(TEXT("CBTState"), *FString::Printf(TEXT("CBTClear")), InbClear, GB2UserSavedStateIni);
	GConfig->Flush(false, GB2UserSavedStateIni);
}


void FStageDataStore::LoadRecentlyStageInfo(int32 & ChapterNum, int32 & StageNum, EStageDifficulty & Difficulty)
{
	BLADE2_SCOPE_CYCLE_COUNTER(LoadRecentlyStageInfo);
	
	int32 IntDifficulty = 0;
	
	GConfig->GetInt(TEXT("ChapterState"), *FString::Printf(TEXT("RecentlyChapterNum")), ChapterNum, GB2UserSavedStateIni);
	GConfig->GetInt(TEXT("ChapterState"), *FString::Printf(TEXT("RecentlyStageNum")), StageNum, GB2UserSavedStateIni);
	GConfig->GetInt(TEXT("ChapterState"), *FString::Printf(TEXT("RecentlyDifficulty")), IntDifficulty, GB2UserSavedStateIni);
	
	Difficulty = static_cast<EStageDifficulty>(IntDifficulty);

	if (StageNum == 0 || ChapterNum == 0 || Difficulty == EStageDifficulty::ESD_None)
	{
		StageNum = 1;
		ChapterNum = 1;
		Difficulty = EStageDifficulty::ESD_Normal;
	}
}

int32 FStageDataStore::LoadClearAct()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_LoadClearAct);
	int32 RetValue = 0;
	GConfig->GetInt(TEXT("ChapterState"), *FString::Printf(TEXT("ClearChapterNum")), RetValue, GB2UserSavedStateIni);
	return RetValue;
}

int32 FStageDataStore::LoadOpenAct()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_LoadOpenAct);
	int32 RetValue = 1;
	GConfig->GetInt(TEXT("ChapterState"), *FString::Printf(TEXT("OpenChapterNum")), RetValue, GB2UserSavedStateIni);
	return RetValue;
}

bool FStageDataStore::LoadCBTClear()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_LoadCBTClear);
	bool RetValue = false;
	GConfig->GetBool(TEXT("CBTState"), *FString::Printf(TEXT("CBTClear")), RetValue, GB2UserSavedStateIni);
	return RetValue;
}

int32 FStageDataStore::LoadClearDifficult()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_LoadClearDifficult);
	int32 RetValue = 1;
	GConfig->GetInt(TEXT("ChapterState"), *FString::Printf(TEXT("ClearChapterDifficult")), RetValue, GB2UserSavedStateIni);
	return RetValue;
}

#include "B2StageInfo.h"
float FStageDataStore::GetStageDifficultyScale(int32 ClientStageId, EStageDifficulty Difficulty, EB2GameMode TemporaryGameModeType)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageDifficultyScale);
	bool bUsingOnlyClientDataForBalancingTest = false;

	GConfig->GetBool(TEXT("/Script/BladeII"), TEXT("bUsingOnlyClientDataForBalancingTest"), bUsingOnlyClientDataForBalancingTest, GGameIni);

	FServerStageID* ServerStageID = ServerStageIdMappingKeys.Find(GetCombineKey(ClientStageId, Difficulty));
	// Scenario 모드에서만 서버에서 데이터를 받도록 한 것은 M15 빌드 당시의 임시이다. 서버 테이블 준비된 후에는 여기로 넘어오는 TemporaryGameModeType 을 없애야 함. 
	if (ServerStageID != nullptr && TemporaryGameModeType == EB2GameMode::Scenario)
	{
		if (FStageInfoGameplayData* ThisStageInfo = StageGameplayDatas.Find(*ServerStageID))
			return ThisStageInfo->GetStageDifficultyScale_ServerData();
	}
	else
		bUsingOnlyClientDataForBalancingTest = true; //서버로부터 데이터를 받지 못한 경우.

	if (ShouldShowGDCVersionContent() || bUsingOnlyClientDataForBalancingTest)
	{
		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
		if (StageInfoTable)
		{
			auto* ClientStageInfoData = StageInfoTable->GetInfoData(ClientStageId);
			if (ClientStageInfoData)
				return ClientStageInfoData->GetDifficultyLevel_ClientData(Difficulty);
		}
	}

	return 1.f;
}

#include "B2CombatStatEvaluator.h"
EUIBattleDifficulty FStageDataStore::GetRelativeStageDifficulty(FServerStageID ServerStageId, EPCClass MainClass, EPCClass TagClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetRelativeStageDifficulty);
	float SuggestedAttack = 0.0f;
	float SuggestedDefense = 0.0f;

	GetStageSuggestedCombatStats(ServerStageId, SuggestedAttack, SuggestedDefense);

	// 메인 태그 연합 스탯
	const float MyUnitedAttack = (MainClass == EPCClass::EPC_End ? 0 : CombatStatEval::GetPCAttack(MainClass))
		+ (TagClass == EPCClass::EPC_End ? 0 : CombatStatEval::GetPCAttack(TagClass));
	const float MyUnitedDefense = (MainClass == EPCClass::EPC_End ? 0 : CombatStatEval::GetPCDefense(MainClass))
		+ (TagClass == EPCClass::EPC_End ? 0 : CombatStatEval::GetPCDefense(TagClass));
	// 공격 방어 합동 스탯
	const float SuggestedJointStat = SuggestedAttack + SuggestedDefense;
	const float MyJointCombatStat = MyUnitedAttack + MyUnitedDefense;

	if (MyJointCombatStat >= 0.9f * SuggestedJointStat) {
		return EUIBattleDifficulty::Easy;
	}
	else if (MyJointCombatStat < 0.8f * SuggestedJointStat) {
		return EUIBattleDifficulty::Hard;
	}
	return EUIBattleDifficulty::Normal;
}

int32 FStageDataStore::GetStageExpectedClearStarGrade(FServerStageID ServerStageId, int32 CurrentPlayTime)
{ // 스테이지 종료 시 보여주는 클리어 시간에 따른 별 등급 1 ~ 3
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageExpectedClearStarGrade);
	FStageInfoGameplayData* ThisStageInfo = StageGameplayDatas.Find(ServerStageId);
	if (ThisStageInfo)
	{
		return (
			(CurrentPlayTime <= ThisStageInfo->ClearTimeGrade_High) ? STAGE_CLEAR_GRADE_STAR_3 :
			(CurrentPlayTime <= ThisStageInfo->ClearTimeGrade_Medium ? STAGE_CLEAR_GRADE_STAR_2 : STAGE_CLEAR_GRADE_STAR_1)
			);
	}
	return STAGE_CLEAR_GRADE_STAR_1;
}

int32 FStageDataStore::GetTimeLimitForStageClearGrade(FServerStageID ServerStageId, int32 InStarGrade)
{ // GetStageExpectedClearStarGrade 와 반대로 StarGrade 를 달성하기 위한 제한 PlayTime 을 리턴.
  // 0 은 조건 없는 걸로.
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetTimeLimitForStageClearGrade);
	FStageInfoGameplayData* ThisStageInfo = StageGameplayDatas.Find(ServerStageId);
	if (ThisStageInfo)
	{
		return (
			(InStarGrade >= STAGE_CLEAR_GRADE_STAR_3) ? ThisStageInfo->ClearTimeGrade_High :
			(InStarGrade >= STAGE_CLEAR_GRADE_STAR_2 ? ThisStageInfo->ClearTimeGrade_Medium : 0)
			);
	}
	return 0;
}

bool FStageDataStore::IsRepeatBattleLocked(FServerStageID ServerStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_IsRepeatBattleLocked);
	FB2StageData* StageInfo = CachedStageDatas.Find(ServerStageId);
	return (StageInfo && StageInfo->ServerData && StageInfo->ServerData->enable_auto_repeat_battle) ? false : true;
}

bool FStageDataStore::IsPossibleToDoRepeatBattle(FServerStageID ServerStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_IsPossibleToDoRepeatBattle);
	FB2StageData* StageInfo = CachedStageDatas.Find(ServerStageId);
	if (StageInfo && StageInfo->ServerData)
	{
		FB2OriginGetStageInfoPtr CachedStage = StageInfo->ServerData;

		//if (CachedStage->enable_auto_repeat_battle == false)
		//	return false;

		// 영혼검 개수는 굳이 반복 전투 아니어도 기본 전투 요건이기도 한데..
		if (CachedStage->need_blade_point > BladeIIGameImpl::GetClientDataStore().GetBladeCurPoint())
			return false;

		if (IsEnoughGoldToRepeatBattle(ServerStageId) == false)
			return false;

		//if (BladeIIGameImpl::GetClientDataStore().IsRepeatBattleLocked(ServerStageId))
		//	return false;

		return true;
	}

	return false;
}

bool FStageDataStore::IsEnoughGoldToRepeatBattle(FServerStageID ServerStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_IsEnoughGoldToRepeatBattle);
	FB2StageData* StageInfo = CachedStageDatas.Find(ServerStageId);
	if (StageInfo && StageInfo->ServerData)
	{
		FB2OriginGetStageInfoPtr CachedStage = StageInfo->ServerData;

		// 나중에 서버에서 전달받은 Valid Buff의 가격
		TArray<EStageBuffType> CurrentBuffs;
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		if (DocBS != nullptr) // DocBattleStage 내부의 Buff는 항상 최신으로 유지해야한다. [ 서버 Response 이후 바로 동기화 ]
		{
			CurrentBuffs = DocBS->GetSelectedStageBuffs();
		}

		int32 BuffCost = GetStageBuffGoldCost(ServerStageId, CurrentBuffs, true);
		if (CachedStage->auto_repeat_battle_cost + BuffCost <= BladeIIGameImpl::GetClientDataStore().GetGoldAmount())
			return true;
	}

	return false;
}

bool FStageDataStore::IsCachedStageData(FServerStageID ServerStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_IsCachedStageData);
	auto* pInfo = CachedStageDatas.Find(ServerStageId);
	return pInfo ? true : false;
}

int32 FStageDataStore::GetStageClearGradeInfo(FServerStageID ServerStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageClearGradeInfo);
	int32 ActId = GetActIdByClientStageId(GetClientStageId(ServerStageId));
	EStageDifficulty Difficulty = GetStageDifficulty(ServerStageId);

	auto* Act = CachedActDatas.Find(GetCombineKey(ActId, Difficulty));
	if (Act && Act->ServerData)
	{
		for (auto& ClearInfo : Act->ServerData->stage_clear_infos)
		{
			if (ServerStageId == ClearInfo->stage_id)
				return ClearInfo->clear_grade;
		}
	}

	return 0;
}

EUIChapterStageBonusType FStageDataStore::GetStageBonusType(int32 ClientStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageBonusType);
	UB2StageInfo* Info = StaticFindStageInfo();
	if (Info)
	{
		auto* StageInfo = Info->GetInfoData(ClientStageId);
		if (StageInfo)
		{
			return StageInfo->bIsGoldBonusStage ? EUIChapterStageBonusType::Gold : EUIChapterStageBonusType::None;
		}
	}

	return EUIChapterStageBonusType::None;
}

void FStageDataStore::SetActClearStageData(FServerStageID ServerStageId, int32 ClearGrade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_SetActClearStageData);
	int32 ActId = GetActIdByClientStageId(GetClientStageId(ServerStageId));
	EStageDifficulty Difficulty = GetStageDifficulty(ServerStageId);

	auto* Act = CachedActDatas.Find(GetCombineKey(ActId, Difficulty));

	if (Act && Act->ServerData)
	{
		for (auto& StageClearInfo : Act->ServerData->stage_clear_infos)
		{
			if (ServerStageId == StageClearInfo->stage_id)
			{
				StageClearInfo->clear_grade = FMath::Max(StageClearInfo->clear_grade, ClearGrade); // 더 낮아지지는 않는다 
				return;
			}
		}

		// 위에서 리턴되지 않았다는건 처음 클리어한 맵이라는 뜻.
		// 기존에 캐싱한 데이터에 새 클리어 맵의 정보가 없기때문에, 서버로 액트 정보 갱신 요청.
		
		CachedActDatas.Remove(GetCombineKey(ActId, Difficulty)); // 서버 업데이트를 위해 키 제거
		ConditionalRequestGetActInfo(ActId, Difficulty); 
	}
}

void FStageDataStore::CheckChangeLastClearStage(const FB2OriginAccountInfoPtr Current, const FB2OriginAccountInfoPtr Next)
{
	if (Current == nullptr || Next == nullptr)
		return;

	int32 CurrentClientStageID = 0;
	int32 NextClientStageID = 0;
	EStageDifficulty StageDifficulty = EStageDifficulty::ESD_None;

	auto CheckFunc = [this](int32 CurrentServerStageID, int32 NextServerStageID, int32* pCurrentClientStageID, int32* pNextClientStageID, EStageDifficulty* pDifficulty) 
	{
		*pCurrentClientStageID = this->GetClientStageId(FServerStageID(CurrentServerStageID));
		*pNextClientStageID = this->GetClientStageId(FServerStageID(NextServerStageID));
		*pDifficulty = this->GetStageDifficulty(FServerStageID(CurrentServerStageID));
		return *pCurrentClientStageID != *pNextClientStageID;
	};

	if(CheckFunc(Current->last_clear_stage_id_normal, Next->last_clear_stage_id_normal, &CurrentClientStageID, &NextClientStageID, &StageDifficulty))
		ChangeLastClearStageClass<EStageDifficulty, int32, int32>::GetInstance().Signal(StageDifficulty, CurrentClientStageID, NextClientStageID);

	if (CheckFunc(Current->last_clear_stage_id_hard, Next->last_clear_stage_id_hard, &CurrentClientStageID, &NextClientStageID, &StageDifficulty))
		ChangeLastClearStageClass<EStageDifficulty, int32, int32>::GetInstance().Signal(StageDifficulty, CurrentClientStageID, NextClientStageID);

	if (CheckFunc(Current->last_clear_stage_id_very_hard, Next->last_clear_stage_id_very_hard, &CurrentClientStageID, &NextClientStageID, &StageDifficulty))
		ChangeLastClearStageClass<EStageDifficulty, int32, int32>::GetInstance().Signal(StageDifficulty, CurrentClientStageID, NextClientStageID);

	if (CheckFunc(Current->last_clear_stage_id_hell, Next->last_clear_stage_id_hell, &CurrentClientStageID, &NextClientStageID, &StageDifficulty))
		ChangeLastClearStageClass<EStageDifficulty, int32, int32>::GetInstance().Signal(StageDifficulty, CurrentClientStageID, NextClientStageID);
}

int32 FStageDataStore::GetLastPlayedActId()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetLastPlayedActId);
	FB2OriginAccountInfoPtr AccountInfo = BladeIIGameImpl::GetClientDataStore().GetAccountInfo();
	if (AccountInfo == nullptr)
		return 0;

	if (StageGameplayDatas.Num() == 0)
		return 0;

	int32 lastClientStageId = GetClientStageId(GetLastPlayedServerStageId());
	return lastClientStageId > 0 ? GetActIdByClientStageId(lastClientStageId) : 1;
}

int32 FStageDataStore::GetLastPlayedClientStageId()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetLastPlayedClientStageId);
	return GetClientStageId(GetLastPlayedServerStageId());
}

EStageDifficulty FStageDataStore::GetLastPlayedStageDifficulty()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetLastPlayedStageDifficulty);
	return GetStageDifficulty(GetLastPlayedServerStageId());
}

int32 FStageDataStore::GetLastSubChapterNum(int32 ActNum)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetLastSubStageNum);
	if (UB2StageInfo* StageInfoTable = StaticFindStageInfo())
	{
		TArray<FSingleStageInfoData*> StageInfoDatas;
		StageInfoTable->GetAllInfoDataOfChapter(StageInfoDatas, ActNum);
		StageInfoDatas.Sort([](FSingleStageInfoData A, FSingleStageInfoData B) {return A.SubChapterNum > B.SubChapterNum; });
		if (StageInfoDatas.IsValidIndex(0))
			return StageInfoDatas[0]->SubChapterNum;
	}
	return INDEX_NONE;
}

FServerStageID FStageDataStore::GetLastClearServerStageIdByLastPlayedStageDifficulty()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetLastClearServerStageIdByLastPlayedStageDifficulty);
	// 가장 마지막에 플레이한 난이도의 마지막 스테이지 아이디 반환
	EStageDifficulty LastStageDifficulty = GetStageDifficulty(GetLastPlayedServerStageId());
	return GetLastClearServerStageId(LastStageDifficulty);
}

FServerStageID FStageDataStore::GetNextClearServerStageIdByLastPlayedStageDifficulty()
{
	// 가장 마지막에 플레이한 난이도의 마지막 스테이지 아이디 반환
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetNextClearServerStageIdByLastPlayedStageDifficulty);
	EStageDifficulty LastStageDifficulty = GetStageDifficulty(GetLastPlayedServerStageId());
	return GetNextClearServerStageId(LastStageDifficulty);
}

FServerStageID FStageDataStore::GetNewLastClearServerStageIdByLastPlayedStageDifficulty(int32 InChapter)
{
	EStageDifficulty StageDifficulty = GetMaxOpenedNewStageDifficulty(InChapter);

	FServerStageID ServerStageID = GetLastClearServerStageId(StageDifficulty);

	int32 ClientID = GetClientStageId(ServerStageID);

	ServerStageID = GetServerStageID(ClientID + 1, StageDifficulty);

	if(ServerStageID.Id == 0)
		ServerStageID = GetServerStageID(ClientID , StageDifficulty);
	
	return ServerStageID;

}

EStageDifficulty FStageDataStore::GetMaxOpenedStageDifficulty()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetMaxOpenedStageDifficulty);
	if (CanAdventureMode(EStageDifficulty::ESD_Hell))
		return EStageDifficulty::ESD_Hell;

	if (CanAdventureMode(EStageDifficulty::ESD_VeryHard))
		return EStageDifficulty::ESD_VeryHard;

	if (CanAdventureMode(EStageDifficulty::ESD_Hard))
		return EStageDifficulty::ESD_Hard;

	return EStageDifficulty::ESD_Normal;
}

EStageDifficulty FStageDataStore::GetMaxOpenedNewStageDifficulty(int32 InChapter)
{
	FServerStageID LastServerID;		//InChapter * 10 이 마지막스테이지라는 가정하에 계산하다

	LastServerID = GetServerStageID(InChapter * 10 , EStageDifficulty::ESD_VeryHard);

	if (LastServerID.Id && LastServerID.Id <= GetLastClearServerStageId(EStageDifficulty::ESD_VeryHard).Id)
	{
		LastServerID = GetServerStageID(((InChapter - 1) * 10) + 1, EStageDifficulty::ESD_Hell);

		if (LastServerID.Id)
			return EStageDifficulty::ESD_Hell;
		else
			return EStageDifficulty::ESD_VeryHard;
	}

	LastServerID = GetServerStageID(InChapter * 10, EStageDifficulty::ESD_Hard);

	if (LastServerID.Id && LastServerID.Id <= GetLastClearServerStageId(EStageDifficulty::ESD_Hard).Id)
	{
		LastServerID = GetServerStageID(((InChapter - 1) * 10) + 1, EStageDifficulty::ESD_VeryHard);

		if (LastServerID.Id)
			return EStageDifficulty::ESD_VeryHard;
		else
			return EStageDifficulty::ESD_Hard;
	}

	LastServerID = GetServerStageID(InChapter * 10, EStageDifficulty::ESD_Normal);

	if (LastServerID.Id && LastServerID.Id <= GetLastClearServerStageId(EStageDifficulty::ESD_Normal).Id)
	{
		LastServerID = GetServerStageID(((InChapter - 1) * 10) + 1, EStageDifficulty::ESD_Hard);
		
		if (LastServerID.Id)
			return EStageDifficulty::ESD_Hard;
		else
			return EStageDifficulty::ESD_Normal;
	}

	return EStageDifficulty::ESD_Normal;
}

bool FStageDataStore::GetNewChapterOpenState(int32 InChapter)
{
	FServerStageID LastServerID = GetServerStageID((InChapter-1) * 10, EStageDifficulty::ESD_Hell);

	return LastServerID.Id <= GetLastClearServerStageId(EStageDifficulty::ESD_Hell).Id;
}

FServerStageID FStageDataStore::GetLastPlayedServerStageId()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetLastPlayedServerStageId);
	FB2OriginAccountInfoPtr AccountInfo = BladeIIGameImpl::GetClientDataStore().GetAccountInfo();
	return AccountInfo ? FServerStageID(AccountInfo->last_play_stage_id) : FServerStageID();
}

FServerStageID FStageDataStore::GetLastClearServerStageId(EStageDifficulty InDifficulty /*= EStageDifficulty::ESD_Normal*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetLastClearServerStageId);
	if (FB2OriginAccountInfoPtr AccountInfo = BladeIIGameImpl::GetClientDataStore().GetAccountInfo())
	{
		switch (InDifficulty) {
		case EStageDifficulty::ESD_Normal:
			return FServerStageID(AccountInfo->last_clear_stage_id_normal);
		case EStageDifficulty::ESD_Hard:
			return FServerStageID(AccountInfo->last_clear_stage_id_hard);
		case EStageDifficulty::ESD_VeryHard:
			return FServerStageID(AccountInfo->last_clear_stage_id_very_hard);
		case EStageDifficulty::ESD_Hell:
			return FServerStageID(AccountInfo->last_clear_stage_id_hell);
		}

		return FServerStageID(AccountInfo->last_clear_stage_id_normal);
	}

	return FServerStageID();
}

FServerStageID FStageDataStore::GetNextClearServerStageId(EStageDifficulty InDifficulty)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetNextClearServerStageId);
	FServerStageID lastClearServerStageId = GetLastClearServerStageId(InDifficulty);

	if (lastClearServerStageId == 0)
	{
		lastClearServerStageId = GetServerStageID(1, InDifficulty);
		return lastClearServerStageId;
	}


	// 현재 난이도에 다음 스테이지 번호를 찾아준다.
	return GetServerStageID(GetClientStageId(lastClearServerStageId) + 1, GetStageDifficulty(lastClearServerStageId));
}

int32 FStageDataStore::GetLastClearClientId(EStageDifficulty InDifficulty)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetLastClearClientId);
	return GetClientStageId(GetLastClearServerStageId(InDifficulty));
}

int32 FStageDataStore::GetNextClearClientId(EStageDifficulty InDifficulty)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetNextClearClientId);
	return GetClientStageId(GetNextClearServerStageId(InDifficulty));
}

int32 FStageDataStore::GetMaxReachedActNumber(EStageDifficulty InDifficulty)
{
	int32 ActNum = 1;
	FServerStageID CheckServerStageId = GetNextClearServerStageId(InDifficulty) ;

	if (UB2StageInfo* StageInfoTable = StaticFindStageInfo())
	{
		if (IsClearedFinalClientStageId(InDifficulty))
		{
			if (auto DocChapter = Cast<UB2UIDocChapter>(UB2UIDocHelper::GetDocChapter()))
				ActNum = DocChapter->GetCurChapterNum();
		}
		else
		{
			if (FStageInfoGameplayData* GamePlayData = StageGameplayDatas.Find(CheckServerStageId))
				ActNum = GetActIdByClientStageId(GamePlayData->ClientStageId);
		}
	}

	return ActNum;
}

bool FStageDataStore::IsClearedFinalClientStageId(EStageDifficulty InCheckDifficulty /*= EStageDifficulty::ESD_Normal*/, bool IsNext /*= false*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_IsClearedFinalClientStageId);
	FServerStageID CheckServerStageId = IsNext ? GetNextClearServerStageId(InCheckDifficulty) : GetLastClearServerStageId(InCheckDifficulty);

	if (UB2StageInfo* StageInfoTable = StaticFindStageInfo())
	{
		if (FStageInfoGameplayData* GamePlayData = StageGameplayDatas.Find(CheckServerStageId))
			return (GamePlayData->ClientStageId >= StageInfoTable->GetFinalClientStageId());
	}

	return false;
}

bool FStageDataStore::IsClearedStartAdventureModeClientStageId(EStageDifficulty InCheckDifficulty /*= EStageDifficulty::ESD_Normal*/, bool IsNext /*= false*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_IsClearedStartAdventureModeClientStageId);
	if (InCheckDifficulty == EStageDifficulty::ESD_End || InCheckDifficulty == EStageDifficulty::ESD_None)
		return false;

	FServerStageID CheckServerStageId = IsNext ? GetNextClearServerStageId(InCheckDifficulty) : GetLastClearServerStageId(InCheckDifficulty);
	if (FStageInfoGameplayData* GamePlayData = StageGameplayDatas.Find(CheckServerStageId))
		return (GamePlayData->ClientStageId >= GetStartAdventureModeClientStageId());

	return false;
}

bool FStageDataStore::CanAdventureMode(EStageDifficulty InCheckDifficulty /*= EStageDifficulty::ESD_Normal*/, bool IsNext /*= false*/)
{ // 해당 난이도를 선택할 수 있는지 체크
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_CanAdventureMode);
	if (InCheckDifficulty == EStageDifficulty::ESD_None)
		return false;

	if (InCheckDifficulty == EStageDifficulty::ESD_Normal)
		return true;

	// Note : 이전 난이도에서 스테이지를 AdventureModeClientStageId이상을 클리어했을시 난이도 선택 가능
	EStageDifficulty PreDifficulty = static_cast<EStageDifficulty>((int32)InCheckDifficulty - 1);
	return IsClearedStartAdventureModeClientStageId(PreDifficulty, IsNext);
}

int32 FStageDataStore::GetStartAdventureModeClientStageId()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStartAdventureModeClientStageId);
	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	UB2ChapterInfo* ChapterInfoTable = StaticFindChapterInfo();
	if (StageInfoTable  && ChapterInfoTable)
	{
		int32 ChapterNum = StageInfoTable->GetStartAdventureModeChapterNum();
		return StageInfoTable->GetClientStageIdOfChapterNums(ChapterNum, ChapterInfoTable->GetStagesNumInChapter(ChapterNum));
	}

	return 0;
}

bool FStageDataStore::IsStageClearedOnce(FServerStageID ServerStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_IsStageClearedOnce);
	return (GetStageClearGradeInfo(ServerStageId) > 0);
}

int32 FStageDataStore::GetStageBuffGoldCost(FServerStageID ServerStageId, const TArray<EStageBuffType>& InAllSelectedType, bool bApplyBuffTicket)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageBuffGoldCost);
	int32 BuffGold = 0;

	FStageInfoGameplayData* FoundGamePlayData = StageGameplayDatas.Find(ServerStageId);
	if (FoundGamePlayData != nullptr)
	{

		for (EStageBuffType ThisBuff : InAllSelectedType)
		{
			/*
			// Buff Ticket 제작되면 구현
			if (bExceptBuffTicket && HasFreeTicket(ThisBuff))
			continue;
			*/

			switch (ThisBuff)
			{
			case EStageBuffType::SBT_IncreaseExp: BuffGold += FoundGamePlayData->ExpBuffCost; break;
			case EStageBuffType::SBT_IncreaseGold: BuffGold += FoundGamePlayData->GoldBuffCost; break;
			case EStageBuffType::SBT_AutoSkill: BuffGold += FoundGamePlayData->AutoSkillBuffCost; break;
			case EStageBuffType::SBT_BoostGameSpeed: BuffGold += FoundGamePlayData->BoostBuffCost; break;
			}
		}
	}

	return BuffGold;
}

int32 FStageDataStore::GetStageSingleBuffGoldCost(FServerStageID ServerStageId, EStageBuffType BuffType)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_GetStageSingleBuffGoldCost);
	FStageInfoGameplayData* FoundGamePlayData = StageGameplayDatas.Find(ServerStageId);
	if (FoundGamePlayData != nullptr)
	{
		switch (BuffType)
		{
		case EStageBuffType::SBT_IncreaseExp: return FoundGamePlayData->ExpBuffCost;
		case EStageBuffType::SBT_IncreaseGold: return FoundGamePlayData->GoldBuffCost;
		case EStageBuffType::SBT_AutoSkill: return FoundGamePlayData->AutoSkillBuffCost;
		case EStageBuffType::SBT_BoostGameSpeed: return FoundGamePlayData->BoostBuffCost;
		}
	}

	return 0;
}


void FStageDataStore::RequestGetChapterClearReward(int32 ChapterId, EStageDifficulty Difficulty, int32 nRewardIndex)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_RequestGetChapterClearReward);
	if (ChapterId > 0) {
		RequestedActIdForChapterClearReward = ChapterId;
		RequestedDifficultyForChapterClearReward = Difficulty;
		int32 reward_index = nRewardIndex;
		data_trader::Retailer::GetInstance().RequestGetActClearReward(CliToSvrStageDifficulty(Difficulty), ChapterId, reward_index);
	}
}

void FStageDataStore::ResponseChapterClearReward(const FB2GetReceiveActClearReward& ResponseRewardInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_ResponseChapterClearReward);
	//TArray<b2network::B2RewardPtr> ItemRewards = GET_TUPLE_DATA(FB2ResponseReceiveActClearReward::rewards_index, ResponseRewardInfo);

	//if (ItemRewards.Num() > 0)
	//{
	//	auto* RewardMailPopUp = UB2UIManager::GetInstance()->OpenRewardMailPopUp(ItemRewards);
	//	if(RewardMailPopUp == nullptr)
	//	{
	//		if (UB2UIMsgPopupReward* RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty()))
	//		{
	//			RewardPopup->AddRewardItems(ItemRewards);
	//			RewardPopup->ShowRewardPopup();
	//		}
	//	}
	//}

	//if (RequestedActIdForChapterClearReward > 0 && RequestedDifficultyForChapterClearReward != EStageDifficulty::ESD_None)
	//{
	//	FB2ActData* ActInfo = CachedActDatas.Find(GetCombineKey(RequestedActIdForChapterClearReward, RequestedDifficultyForChapterClearReward));
	//	if (ActInfo && ActInfo->ServerData)
	//	{
	//		int32 RewardedInfoIndex = GET_TUPLE_DATA(FB2ResponseReceiveActClearReward::reward_index_index, ResponseRewardInfo);

	//		for (b2network::B2ActRewardInfoPtr& RewardInfo : ActInfo->ServerData->reward_infos)
	//		{ // 이번에 보상을 받은 index 의 info 를 찾아서 받았음 표시
	//			if (RewardInfo->reward_index == RewardedInfoIndex)
	//			{
	//				RewardInfo->received = true;
	//				break;
	//			}
	//		}
	//	}

	//	// 업데이트 했으면 새로 정보 얻어와서 UIDoc 쪽에 세팅.
	//	UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter();
	//	if (DocChapter)
	//	{
	//		TArray<FUIChapterStarReward> ActRewards;
	//		GetActRewardData(RequestedActIdForChapterClearReward, RequestedDifficultyForChapterClearReward, ActRewards);
	//		
	//		bool bHasRewardableBox = false;
	//		for (const FUIChapterStarReward& StarReward : ActRewards)
	//		{
	//			if (StarReward.bAlreadyRewarded == false && StarReward.StarCurNum >= StarReward.StarMaxNum)
	//				bHasRewardableBox = true;
	//		}

	//		if (bHasRewardableBox == false)
	//			RemoveActRewardState(RequestedActIdForChapterClearReward, RequestedDifficultyForChapterClearReward);

	//		DocChapter->SetStarRewards(ActRewards);
	//	}
	//}

	//RequestedActIdForChapterClearReward = -1;
	//RequestedDifficultyForChapterClearReward = EStageDifficulty::ESD_None;
}

void FStageDataStore::RequestGetRepeatMissionClearReward(FServerStageID ServerStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_RequestGetRepeatMissionClearReward);
	int32 ActId = GetActIdByClientStageId(GetClientStageId(ServerStageId));
	EStageDifficulty Difficulty = GetStageDifficulty(ServerStageId);

	if (ActId > 0) {
		RequestedActIdForRepeatClearReward = ActId;
		RequestedDifficultyForRepeatClearReward = Difficulty;
		data_trader::Retailer::GetInstance().RequestGetActRepeatClearReward(CliToSvrStageDifficulty(Difficulty), ActId);
	}
}

void FStageDataStore::ResponseRepeatMissionClearReward(const FB2GetReceiveActRepeatClearReward& ResponseRewardInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FStageDataStore_ResponseRepeatMissionClearReward);
	//const int32 RewardGold = GET_TUPLE_DATA(FB2ResponseReceiveActRepeatClearReward::rewarded_gold_index, ResponseRewardInfo);

	//if (FB2OriginAccountInfoPtr AccountInfo = BladeIIGameImpl::GetClientDataStore().GetAccountInfo())
	//	AccountInfo->money += RewardGold;	

	//// 추가로 UI 연출을 위한 이벤트 시그널
	//LobbyReceivedRepeatClearRewardClass<int32>::GetInstance().Signal(RewardGold);

	//RequestedActIdForRepeatClearReward = -1;
	//RequestedDifficultyForRepeatClearReward = EStageDifficulty::ESD_None;
}
