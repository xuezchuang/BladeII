
#include "Retailer.h"
#include "B2PCClassInfo.h"
#include "Event.h"
#include "InternalEvents.h"
#include "BladeIIBlockToSyncNetwork.h"

#if WITH_EDITOR
#define BII_CHECK(expr) checkSlow((expr))
#else
#define BII_CHECK(expr) check((expr)) 
#endif

namespace data_trader
{
	Retailer& Retailer::GetInstance()
	{
		static Retailer	inst;
		return inst;
	}

	void Retailer::RequestChangeNickname(const FString& Nickname, int32 itemTemplateId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[Nickname, itemTemplateId]() {
			RequestUpdateNicknameClass<const FString&, uint32>::GetInstance().Signal(Nickname, itemTemplateId);
		});
	}

	void Retailer::RequestGetAccount()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetAccountClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetAccountItem(int32 CharacterType, int32 InventoryType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[CharacterType, InventoryType](){
			RequestGetAccountItemClass<int32, int32>::GetInstance().Signal(CharacterType, InventoryType);
		});
	}

	void Retailer::RequestGetMasterData()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetMasterDataClass<>::GetInstance().Signal();
		});
		FBladeIIBlockToSyncNetwork::GetInstance().SetIsRequestMasterData(true);
	}

	void Retailer::RequestGetLatestEventData(int64 lastUpdateTime)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[lastUpdateTime]() {
			RequestGetLatestEventDataClass<int64>::GetInstance().Signal(lastUpdateTime);
		});
	}

	void Retailer::RequestDismantleItems(const TArray<int64>& items)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[items]() {
			RequestDismantleItemsClass<const TArray<int64>&>::GetInstance().Signal(items);
		});
	}

	void Retailer::RequestSellItem(const TArray<int64>& items)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[items]() {
			RequestSellItemClass<const TArray<int64>&>::GetInstance().Signal(items);
		});
	}

	void Retailer::RequestSellConsumableItem(int64 itemId, int32 sellCount)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[itemId, sellCount]() {
			RequestSellConsumableItemClass<int64, int32>::GetInstance().Signal(itemId, sellCount);
		});
	}

	void Retailer::RequestConvertData(int32 templateId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[templateId]() {
			RequestConvertDataClass<int32>::GetInstance().Signal(templateId);
		});
	}

	void Retailer::RequestConvertItem(int32 itemId, int32 covnertType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[itemId, covnertType]() {
			RequestConvertItemClass<int32, int32>::GetInstance().Signal(itemId, covnertType);
		});
	}


	void Retailer::RequestEquipItem(int32 itemPresetId, const TArray<int64>& items)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[itemPresetId, items](){
			RequestEquipItemClass<int32, const TArray<int64>&>::GetInstance().Signal(itemPresetId, items);
		});
	}

	void Retailer::RequestUnequipItem(int32 itemPresetId, const TArray<int64>& items)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[itemPresetId, items](){
			RequestUnequipItemClass<int32, const TArray<int64>&>::GetInstance().Signal(itemPresetId, items);
		});
	}

	void Retailer::RequestLockItem(const TArray<int64>& items)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[items](){
			RequestLockItemClass<const TArray<int64>&>::GetInstance().Signal(items);
		});
	}
	
	void Retailer::RequestUnlockItem(const TArray<int64>& items)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[items](){
			RequestUnlockItemClass<const TArray<int64>&>::GetInstance().Signal(items);
		});
	}
	
	void Retailer::RequestEnhanceItem(int64 main, int32 enhanceScrollTID, bool useProtection, int32 protectionScrollTID)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[main, enhanceScrollTID, useProtection, protectionScrollTID]() {
			RequestEnhanceItemClass<int64, int32, bool, int32>::GetInstance().Signal(main, enhanceScrollTID, useProtection, protectionScrollTID);
		});
	}

	void Retailer::RequestExchangeItemEnhanceLevel(int64 item_id_1, int64 item_id_2, int32 price_type)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[item_id_1, item_id_2, price_type]() {
			RequestExchangeItemEnhanceLevelClass<int64, int64, int32>::GetInstance().Signal(item_id_1, item_id_2, price_type);
		});
	}

	void Retailer::RequestOpenSealSlotItem(int64 item_id, int32 seal_slot)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[item_id, seal_slot]() {
			RequestOpenSealSlotItemClass<int64, int32>::GetInstance().Signal(item_id, seal_slot);
		});
	}

	void Retailer::RequestCarveSealItem(int64 item_id, int32 seal_slot)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[item_id, seal_slot]() {
			RequestCarveSealItemClass<int64, int32>::GetInstance().Signal(item_id, seal_slot);
		});
	}

	void Retailer::RequestLevelupItem(int64 main, const TArray<int64>& materials, bool autoLevelup)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[main, materials, autoLevelup](){
			RequestLevelupItemClass<int64, const TArray<int64>&, bool>::GetInstance().Signal(main, materials, autoLevelup);
		});
	}

	void Retailer::RequestGetItemSurpassCost(int64 uid)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[uid](){
			RequestGetItemSurpassCostClass<int64>::GetInstance().Signal(uid);
		});
	}

	void Retailer::RequestSurpassItem(int64 main, int64 material)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[main, material](){
			RequestSurpassItemClass<int64, int64>::GetInstance().Signal(main, material);
		});
	}

	void Retailer::RequestGetItemUpgradeCost(int64 uid)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[uid](){
			RequestGetItemUpgradeCostClass<int64>::GetInstance().Signal(uid);
		});
	}

	void Retailer::RequestUpgradeItem(int64 uid)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[uid](){
			RequestUpgradeItemClass<int64>::GetInstance().Signal(uid);
		});
	}

	void Retailer::RequestGetItemComposeCost(int64 uid, int32 presetId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[uid, presetId](){
			RequestGetItemComposeCostClass<int64, int32>::GetInstance().Signal(uid, presetId);
		});
	}

	void Retailer::RequestComposeItem(int64 main, int64 sub, int32 presetId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[main, sub, presetId](){
			RequestComposeItemClass<int64, int64, int32>::GetInstance().Signal(main, sub, presetId);
		});
	}

	void Retailer::RequestChangeItemCharacterType(int64 mainUID, int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[mainUID, characterType]() {
			RequestChangeItemCharacterTypeClass<int64, int32>::GetInstance().Signal(mainUID, characterType);
		});
	}

	void Retailer::RequestGetChangeItemCharacterTypeCost(int64 mainUID)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[mainUID]() {
			RequestGetChangeItemCharacterTypeCostClass<int64>::GetInstance().Signal(mainUID);
		});
	}

	void Retailer::RequestExtendInventory(int32 CharacterType, int32 ExtendCount)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[CharacterType, ExtendCount](){
			RequestExtendInventoryClass<int32, int32>::GetInstance().Signal(CharacterType, ExtendCount);
		});
	}

	void Retailer::RequestStartStage(FServerStageID serverStageId, int32 main, int32 tag, int32 option, bool repeatBattle, bool boostMode, int32 tutorialId, int32 mainPresetId, int32 tagPresetId)
	{
		BII_CHECK(serverStageId.IsEnable());

		int32 stageRepeatMode = b2network::B2StageRepeatMode::NORMAL;
		if (repeatBattle) {
			stageRepeatMode = b2network::B2StageRepeatMode::REPEATED;
		}
		// TODO b2network::B2StageRepeatMode::CONTINUOUS

		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[serverStageId, main, tag, option, stageRepeatMode, boostMode, tutorialId, mainPresetId, tagPresetId](){
			RequestStartStageClass<FServerStageID, int32, int32, int32, int32, bool, int32, int32, int32>::GetInstance().Signal(serverStageId, main, tag, option, stageRepeatMode, boostMode, tutorialId, mainPresetId, tagPresetId);
		});
	}

	void Retailer::RequestResurrection(const FString& token, int32 seleted_buff_index)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[token, seleted_buff_index](){
			RequestResurrectionClass<const FString&, int32>::GetInstance().Signal(token, seleted_buff_index);
		});
	}

	void Retailer::RequestClearStage(FServerStageID serverStageId, const FString& token, int32 main, int32 tag, int32 cleartime, const TArray<b2network::B2KillMonsterInfoPtr>& killMonsterInfo, 
		int32 totalMonsterKillCount, bool isAllSurvival, int32 qteStartCount, int32 qteFinishCount, b2network::B2ItemSpotSaleOptionPtr b2SpotSaleOptionPtr, b2network::B2AetherSpotSaleOptionPtr b2AetherSpotSaleOptionPtr)
	{
		BII_CHECK(serverStageId.IsEnable());

		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[serverStageId, token, main, tag, cleartime, killMonsterInfo, totalMonsterKillCount, isAllSurvival, qteStartCount, qteFinishCount, b2SpotSaleOptionPtr, b2AetherSpotSaleOptionPtr](){
			RequestClearStageClass<FServerStageID, const FString&, int32, int32, int32, const TArray<b2network::B2KillMonsterInfoPtr>&, 
				int32, bool, int32, int32, b2network::B2ItemSpotSaleOptionPtr, b2network::B2AetherSpotSaleOptionPtr>::GetInstance().Signal(serverStageId, token, main, tag, cleartime, killMonsterInfo,
					totalMonsterKillCount, isAllSurvival, qteStartCount, qteFinishCount, b2SpotSaleOptionPtr, b2AetherSpotSaleOptionPtr);
		});
	}

	void Retailer::RequestFailStage(FServerStageID serverStageId, const FString& token, int32 reason, int32 playtime)
	{
		BII_CHECK(serverStageId.IsEnable());

		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[serverStageId, token, reason, playtime]() {
			RequestFailStageClass<FServerStageID, const FString&, int32, int32>::GetInstance().Signal(serverStageId, token, reason, playtime);
		});
	}

	void Retailer::RequestSweepStage(FServerStageID serverStageId, int32 main, int32 tag, int32 mainPresetId, int32 tagPresetId, int32 repeatCount)
	{
		BII_CHECK(serverStageId.IsEnable());

		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[serverStageId, main, tag, mainPresetId, tagPresetId, repeatCount]() {
			RequestSweepStageClass<FServerStageID, int32, int32, int32, int32, int32>::GetInstance().Signal(serverStageId, main, tag, mainPresetId, tagPresetId, repeatCount);
		});
	}

	void Retailer::RequestCheckBladePointCharge()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestCheckBladePointChargeClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetActInfo(int32 difficulty, int32 act_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[difficulty, act_id](){
			RequestGetActInfoClass<int32, int32>::GetInstance().Signal(difficulty, act_id);
		});
	}

	void Retailer::RequestGetStageInfo(FServerStageID serverStageId)
	{
		ensure(serverStageId.IsEnable()); 

		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[serverStageId](){
			RequestGetStageInfoClass<FServerStageID>::GetInstance().Signal(serverStageId);
		});
	}

	void Retailer::RequestGetActClearReward(int32 difficulty, int32 act_id, int32 reward_index)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[difficulty, act_id, reward_index](){
			RequestReceiveActClearRewardClass<int32, int32, int32>::GetInstance().Signal(difficulty, act_id, reward_index);
		});
	}

	void Retailer::RequestGetActRepeatClearReward(int32 difficulty, int32 act_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[difficulty, act_id](){
			RequestReceiveActRepeatClearRewardClass<int32, int32>::GetInstance().Signal(difficulty, act_id);
		});
	}

	void Retailer::RequestGetContentsModeState(int32 ContentsMode /*= 0*/)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[ContentsMode]() {
			RequestGetContentsModeStateClass<int32>::GetInstance().Signal(ContentsMode);
		});
	}

	void Retailer::RequestGetMatchStatus()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetMatchStatusClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestMatchmaking(int32 match_type, int32 main, int32 sub, bool useAdditionalCount, FString const& room_seed)
	{
		RequestMatchmakingClass<int32, int32, int32, bool, const FString&>::GetInstance().Signal(
			match_type,
			main,
			sub,
			useAdditionalCount,
			room_seed
		);
	}

	void Retailer::RequestRaidStart(int32 character_type)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[character_type]() {
			RequestStartRaidClass<int32>::GetInstance().Signal(character_type);
		});
	} 

	void Retailer::RequestRaidMatchmaking(int32 match_type, int32 character_type, int32 raid_type, int32 raid_step, b2network::B2EndpointPtr endpoint)
    {
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[match_type, character_type, raid_type, raid_step, endpoint]() {
			RequestRaidMatchmakingClass<int32, int32, int32, int32, b2network::B2EndpointPtr>::GetInstance().Signal(match_type, character_type, raid_type, raid_step, endpoint);
		});
    }

	void Retailer::RequestRoomInvitation(const FString& room_id, const FString& invitee, int32 invitation_number, int32 mapId, int32 difficulty)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[room_id, invitee, invitation_number, mapId, difficulty]() {
			RequestRoomInvitationClass<const FString&, const FString&, int32, int32, int32>::GetInstance().Signal(room_id, invitee, invitation_number, mapId, difficulty);
		});
	}

	void Retailer::RequestRequestJoinRoom(const FString& RoomId, int32 RoomType, int32 roomAccess, TArray<b2network::B2RoomPropertyPtr>& properties, 
		TArray<b2network::B2RoomCharacterPtr>& characters, TArray<b2network::B2PresetNumPtr>& presetNums)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[RoomId, RoomType, roomAccess, &properties, &characters, &presetNums]() {
			RequestJoinRoomClass<const FString&, int32, int32, TArray<b2network::B2RoomPropertyPtr>&, TArray<b2network::B2RoomCharacterPtr>&, 
				TArray<b2network::B2PresetNumPtr>&>::GetInstance().Signal(RoomId, RoomType, roomAccess, properties, characters, presetNums);
		});
	}

	void Retailer::RequestRequestJoinProperty(const FString& RoomId, TArray<b2network::B2RoomPropertyPtr>& PropertyList)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[RoomId, &PropertyList]() {
			RequestRoomPropertyClass<const FString&, TArray<b2network::B2RoomPropertyPtr>&>::GetInstance().Signal(RoomId, PropertyList);
		});
	}

	void Retailer::RequestRoomClose(const FString& RoomId, const int32 mapId, const ERoomCloseReasonType  room_reason)
	{
		if (room_reason == ERoomCloseReasonType::NONE)
			return;

		// Todo : 현재 mapId, difficulty가 Raid에 한정되어있음. (서버와 함께 타입같은게 추가되어야할듯)
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[RoomId, mapId, room_reason]() {
			RequestRoomCloseClass<const FString&, int32, int32>::GetInstance().Signal(RoomId, mapId, CliToSvrRoomCloseReasonType(room_reason));
		});
	}

	void Retailer::RequestRoomLeaveResult(const FString& RoomId, const int64 accountId, const FString& matchToken)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[RoomId, accountId, matchToken]() {
			RequestRoomLeaveResultClass<const FString&, int64,const FString&>::GetInstance().Signal(RoomId, accountId, matchToken);
		});
	}

	void Retailer::RequestRoomAccessControl(const FString& RoomId, int32 accessKey)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[RoomId, accessKey]() {
			RequestRoomAccessControlClass<const FString&, int32>::GetInstance().Signal(RoomId, accessKey);
		});
	}

	void Retailer::RequestLeaveMatch(FString const& roomId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[roomId]() {
			RequestLeaveRoomClass<const FString&>::GetInstance().Signal(roomId);
		});
	}

	void Retailer::RequestKickUser(FString const& roomId, int64 userAccountId)
	{
		RequestRoomKickClass<const FString&, int64>::GetInstance().Signal(roomId, userAccountId);
	}

	void Retailer::RequestCancelMatchmaking()
	{
		RequestCancelMatchmakingClass<>::GetInstance().Signal();
	}

	void Retailer::RequestMatchResult(int64 MyAccountId, const FString& MyToken, int32 MyResult, int32 matchType, b2network::B2RoomCharacterPtr myMain,
			b2network::B2RoomCharacterPtr myTag, b2network::B2RoomCharacterPtr opponentMain, b2network::B2RoomCharacterPtr opponentTag, const FString& roomId, bool bGiveup)
	{
		b2network::B2MatchResult My, Opponent;

		My.account_id = MyAccountId;
		My.match_token = MyToken;
		My.result = MyResult;
		My.disconnect = false;

		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[My, matchType, myMain, myTag, opponentMain, opponentTag, roomId, bGiveup](){
			RequestMatchResultClass<b2network::B2MatchResult, int32, b2network::B2RoomCharacterPtr,
				b2network::B2RoomCharacterPtr, b2network::B2RoomCharacterPtr, b2network::B2RoomCharacterPtr, const FString&, bool>::GetInstance().Signal(My, matchType, myMain, myTag,
					opponentMain, opponentTag, roomId, bGiveup);
		});
	}

	void Retailer::RequestLevelUpSkill(int32 charType, int32 skillId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[charType, skillId](){
			RequestLevelUpSkillClass<int32, int32>::GetInstance().Signal(charType, skillId);
		});
	}

	void Retailer::RequestResetSkill(int32 charType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[charType](){
			RequestResetSkillClass<int32>::GetInstance().Signal(charType);
		});
	}

	void Retailer::RequestBuySkillPoint(const TArray<b2network::B2BuySkillPointRequestInfoPtr>& buyInfos)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[buyInfos](){
			RequestBuySkillPointClass<const TArray<b2network::B2BuySkillPointRequestInfoPtr>&>::GetInstance().Signal(buyInfos);
		});
	}

	void Retailer::RequestSetUsingSkill(int32 charType, int32 presetType, int32 usingSkillId1, int32 usingSkillId2, int32 usingSkillId3)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[charType, presetType, usingSkillId1, usingSkillId2, usingSkillId3](){
			RequestSetUsingSkillClass<int32, int32, int32, int32, int32>::GetInstance().Signal(charType, presetType, usingSkillId1, usingSkillId2, usingSkillId3);
		});
	}

	void Retailer::RequestSetNickName(FString NickName)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[NickName]() {
			RequestGiveNicknameClass<const FString&>::GetInstance().Signal(NickName);
		});
	}

	void Retailer::RequestSetCharNickName(int32 SvrPCClassIndex, const FString& InNickName)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[SvrPCClassIndex, InNickName](){
			RequestSetCharNickNameClass<int32, const FString&>::GetInstance().Signal(SvrPCClassIndex, InNickName);
		});
	}

	void Retailer::RequestGetRank()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetRankClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestEnhanceRankNode(int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[characterType](){
			RequestEnhanceRankNodeClass<int32>::GetInstance().Signal(characterType);
		});
	}

	void Retailer::RequestActivateRankNode(int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[characterType](){
			RequestActivateRankNodeClass<int32>::GetInstance().Signal(characterType);
		});
	}

	void Retailer::RequestGetMailList(int32 mailCategory)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[mailCategory](){
			RequestGetMailListClass<int32>::GetInstance().Signal(mailCategory);
		});
	}

	void Retailer::RequestOpenMail(int64 mailId, int32 characterType, int32 itemTemplateId, int32 aetherType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[mailId, characterType, itemTemplateId, aetherType](){
			RequestOpenMailClass<int64, int32, int32, int32>::GetInstance().Signal(mailId, characterType, itemTemplateId, aetherType);
		});
	}

	void Retailer::RequestOpenAllMail(int32 mailCategory, int64 startId, int64 endId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[mailCategory, startId, endId](){
			RequestOpenAllMailClass<int32, int64, int64>::GetInstance().Signal(mailCategory, startId, endId);
		});
	}

	void Retailer::RequestGetNewMailCount()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetNewMailCountClass<>::GetInstance().Signal();
		});
	}
	
	
	void Retailer::RequestAskFriend(int64 account_id)
	{
		CacheSendAskFriendAccountIdClass<int64>::GetInstance().Signal(account_id);

		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[account_id]() {
			RequestAskFriendClass<int64>::GetInstance().Signal(account_id);
		});
	}

	void Retailer::RequestAcceptAskFriend(int64 account_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[account_id]() {
			RequestAcceptAskFriendClass<int64>::GetInstance().Signal(account_id);
		});
	}

	void Retailer::RequestRejectAskFriend(int64 account_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[account_id]() {
			RequestRejectAskFriendClass<int64>::GetInstance().Signal(account_id);
		});
	}

	void Retailer::RequestGetFriendList()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestGetFriendListClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestCancelAskFriend(int64 account_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[account_id]() {
			RequestCancelAskFriendClass<int64>::GetInstance().Signal(account_id);
		});
	}

	void Retailer::RequestDeleteFriend(int64 account_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[account_id]() {
			RequestDeleteFriendClass<int64>::GetInstance().Signal(account_id);
		});
	}

	void Retailer::RequestGetAllSendAskFriend()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestGetAllSendAskFriendClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetAllReceiveAskFriend()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestGetAllReceiveAskFriendClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestSendSocialPoint(TArray<int64>& ar_account_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[ar_account_id]() {
			RequestSendSocialPointClass<TArray<int64>>::GetInstance().Signal(ar_account_id);
		});
	}

	void Retailer::RequestConfirmNewFriend(bool is_all_confirm, TArray<int64>& ar_account_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[is_all_confirm, ar_account_id]() {
			RequestConfirmNewFriendClass<bool, TArray<int64>>::GetInstance().Signal(is_all_confirm, ar_account_id);
		});
	}

	void Retailer::RequestGetRecommendFriend()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestGetRecommendFriendClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestPlatformFriendInviteInfo(int32 platform_type)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[platform_type]() {
			RequestPlatformFriendInviteInfoClass<int32>::GetInstance().Signal(platform_type);
		});
	}

	void Retailer::RequestPlatformFriendInviteCountUp(int32 platform_type)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[platform_type]() {
			RequestPlatformFriendInviteCountUpClass<int32>::GetInstance().Signal(platform_type);
		});
	}

	void Retailer::RequestPlatformFriendJoinerCountUp(int32 platform_type, int32 joiner_count)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[platform_type, joiner_count]() {
			RequestPlatformFriendJoinerCountUpClass<int32, int32>::GetInstance().Signal(platform_type, joiner_count);
		});
	}

	void Retailer::RequestPlatformFriendInviteReward(int32 platform_type, int32 reward_type, int32 reward_index)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[platform_type, reward_type, reward_index]() {
			RequestPlatformFriendInviteRewardClass<int32, int32, int32>::GetInstance().Signal(platform_type, reward_type, reward_index);
		});
	}

	void Retailer::RequestFindUser(const FString& UserName, bool guild_invite_state /*= false*/)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[UserName, guild_invite_state]() {
			RequestFindUserClass<const FString&, bool>::GetInstance().Signal(UserName, guild_invite_state);
		});
	}

	void Retailer::RequestGetFriendRankings(int32 opponentType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[opponentType]() {
			RequestGetFriendRankingsClass<int32>::GetInstance().Signal(opponentType);
		});
	}

	void Retailer::RequestGetCounterDungeon()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetCounterDungeonClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetChatChannel()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetChatChannelClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestChangeChatChannel(int32 ReqChannelNum)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[ReqChannelNum](){
			RequestChangeChatChannelClass<int32>::GetInstance().Signal(ReqChannelNum);
		});
	}

	void Retailer::RequestGetQuest()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestGetQuestClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestAcceptQuest(int32 slot)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[slot]() {
			RequestAcceptQuestClass<int32>::GetInstance().Signal(slot);
		});
	}

	void Retailer::RequestReceiveQuestReward(int32 slot)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[slot]() {
			RequestReceiveQuestRewardClass<int32>::GetInstance().Signal(slot);
		});
	}

	void Retailer::RequestGetMission(int32 missionType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[missionType]() {
			RequestGetMissionClass<int32>::GetInstance().Signal(missionType);
		});
	}

	void Retailer::RequestGetFeaturedMission()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestGetFeaturedMissionClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestReceiveMissionReward(int32 missionId, bool bSyncNetwork/* = true*/)
	{
		if (bSyncNetwork)
		{
			FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
				[missionId]() {
				RequestReceiveMissionRewardClass<int32>::GetInstance().Signal(missionId);
			});
		}
		else
		{
			// 모두받기로 실행되며 UI단에서 Sync
			RequestReceiveMissionRewardClass<int32>::GetInstance().Signal(missionId);
		}
	}

	void Retailer::RequestReceiveCompleteMissionReward(int32 missionType, int32 rewardIndex)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[missionType, rewardIndex]() {
			RequestReceiveCompleteMissionRewardClass<int32, int32>::GetInstance().Signal(missionType, rewardIndex);
		});
	}

	void Retailer::RequestGetWing(int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[characterType]() {
			RequestGetWingClass<int32>::GetInstance().Signal(characterType);
		});
	}

	void Retailer::RequestEnhanceWing(int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[characterType]() {
			RequestEnhanceWingClass<int32>::GetInstance().Signal(characterType);
		});
	}

	void Retailer::RequestUpgradeWing(int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[characterType]() {
			RequestUpgradeWingClass <int32>::GetInstance().Signal(characterType);
		});
	}

	void Retailer::RequestEnhanceWingOption(int32 characterType, int32 optionIndex)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[characterType, optionIndex]() {
			RequestEnhanceWingOptionClass<int32, int32>::GetInstance().Signal(characterType, optionIndex);
		});
	}

	void Retailer::RequestGetDonationEvent()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetDonationEventClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestMakeDonation(int32 donationId, bool isBundle, bool isAgain, bool isFree, const FString& lastDonationKey)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[donationId, isBundle, isAgain, isFree, lastDonationKey]() {
			RequestMakeDonationClass<int32, bool, bool, bool, const FString&>::GetInstance().Signal(donationId, isBundle, isAgain, isFree, lastDonationKey);
		});
	}

	void Retailer::RequestDonationPointReward(int32 donationId, int32 step)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[donationId, step]() {
			RequestDonationPointRewardClass<int32, int32>::GetInstance().Signal(donationId, step);
		});
	}

	void Retailer::RequestGetFairy()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetFairyClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestLevelupFairy(int32 fairyType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[fairyType]() {
			RequestLevelupFairyClass<int32>::GetInstance().Signal(fairyType);
		});
	}

	void Retailer::RequestBlessFairy(int32 fairyType, int32 blessOptionId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[fairyType, blessOptionId]() {
			RequestBlessFairyClass<int32, int32>::GetInstance().Signal(fairyType, blessOptionId);
		});
	}

	void Retailer::RequestReceiveFairyGift(int32 fairyType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[fairyType]() {
			RequestReceiveFairyGiftClass<int32>::GetInstance().Signal(fairyType);
		});
	}

	void Retailer::RequestEnhanceTeamBattleFormation(ETMFormation FormationType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[FormationType](){
			RequestEnhanceTeamBattleFormationClass<int32>::GetInstance().Signal(CliToSvrFormationType(FormationType));
		});
	}

	void Retailer::RequestUpdateTeamBattleEntry(ETMFormation FormationType, EPCClass Slot1Character, EPCClass Slot2Character, EPCClass Slot3Character)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[FormationType, Slot1Character, Slot2Character, Slot3Character](){
			b2network::B2TeamBattleEntryPtr entryPtr = std::make_shared<b2network::B2TeamBattleEntry>();
			entryPtr->formation_type = CliToSvrFormationType(FormationType);
			entryPtr->character_slot_1 = CliToSvrPCClassType(Slot1Character);
			entryPtr->character_slot_2 = CliToSvrPCClassType(Slot2Character);
			entryPtr->character_slot_3 = CliToSvrPCClassType(Slot3Character);

			RequestUpdateTeamBattleEntryClass<b2network::B2TeamBattleEntryPtr>::GetInstance().Signal(entryPtr);
		});		
	}

	void Retailer::RequestGetTeamBattleStatus()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetTeamBattleStatusClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestTeamMatchmaking(bool bUsedAdditional)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[bUsedAdditional](){
			RequestTeamMatchmakingClass<bool>::GetInstance().Signal(bUsedAdditional);
		});
	}

	void Retailer::RequestTeamMatchResult(const FString& Token, bool bVictory, bool bGiveup)
	{
		int32 dwVictory = bVictory ? b2network::B2MatchResultType::BMRT_WIN : b2network::B2MatchResultType::BMRT_LOSE;
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[Token, dwVictory, bGiveup](){
			RequestTeamMatchResultClass<const FString&, int32, bool>::GetInstance().Signal(Token, dwVictory, bGiveup);
		});
	}

	void Retailer::RequestStartCounterDungeon(FServerStageID InServerStageNum, int32 MainSvrClass)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[InServerStageNum, MainSvrClass](){
			RequestStartCounterDungeonClass<FServerStageID, int32>::GetInstance().Signal(InServerStageNum, MainSvrClass);
		});
	}

	void Retailer::RequestFinishCounterDungeon(FServerStageID InServerStageNum, const FString& Token, bool bCompleted, bool bGiveup)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[InServerStageNum, Token, bCompleted, bGiveup](){
			RequestFinishCounterDungeonClass<FServerStageID, const FString&, bool, bool>::GetInstance().Signal(InServerStageNum, Token, bCompleted, bGiveup);
		});
	}

	void Retailer::RequestSweepCounterDungeon(FServerStageID InServerStageNum, int32 MainSvrClass)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[InServerStageNum, MainSvrClass]() {
			RequestSweepCounterDungeonClass<FServerStageID, int32>::GetInstance().Signal(InServerStageNum, MainSvrClass);
		});
	}

	void Retailer::RequestWeeklyRewardCounterDungeon(int32 RewardIndex)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[RewardIndex](){
			RequestCounterDungeonWeeklyRewardClass<int32>::GetInstance().Signal(RewardIndex);
		});
	}

	void Retailer::RequestBuyTicketCounterDungeon()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestBuyCounterDungeonTicketClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetDimension()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetDimensionClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestStartDimension(int32 difficultyLevel, int32 dungeonId, int32 MainSvrClass, int32 totalPower)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[difficultyLevel, dungeonId, MainSvrClass, totalPower]() {
			RequestStartDimensionClass<int32, int32, int32, int32>::GetInstance().Signal(difficultyLevel, dungeonId, MainSvrClass, totalPower);
		});
	}

	void Retailer::RequestFinishDimension(int32 difficultyLevel, const FString& playToken, int32 playResult, int32 playTimeMillis)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[difficultyLevel, playToken, playResult, playTimeMillis]() {
			RequestFinishDimensionClass<int32, const FString&, int32, int32>::GetInstance().Signal(difficultyLevel, playToken, playResult, playTimeMillis);
		});
	}

	void Retailer::RequestGuildAskJoinGuild(int64 GuildID)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[GuildID](){
			RequestAskJoinGuildClass<int64>::GetInstance().Signal(GuildID);
		});
	}

	void Retailer::RequestCreateGuild(const FString& GuildName, int32 JoinOption, int32 MarkIndex, int32 MarkColor, int32 MarkBgIndex, int32 MarkBGColor)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[GuildName, JoinOption, MarkIndex, MarkColor, MarkBgIndex, MarkBGColor]() {
			RequestCreateGuildClass<const FString&, int32, int32, int32, int32, int32>::GetInstance().Signal(GuildName, JoinOption, MarkIndex, MarkColor, MarkBgIndex, MarkBGColor);
		});
	}

	void Retailer::RequestGuildDetailInfo(int64 GuildID)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[GuildID](){
			RequestGetGuildDetailInfoClass<int64>::GetInstance().Signal(GuildID);
		});
	}

	void Retailer::RequestLeaveGuild()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestLeaveGuildClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetRecommendGuild()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetRecommendGuildClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestAppointSubGuildMaster(int64 AccountID)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[AccountID](){
			RequestAppointSubGuildMasterClass<int64>::GetInstance().Signal(AccountID);
		});
	}

	void Retailer::RequestDismissSubGuildMaster(int64 AccountID)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[AccountID](){
			RequestDismissSubGuildMasterClass<int64>::GetInstance().Signal(AccountID);
		});
	}

	void Retailer::RequestKickGuildMember(int64 AccountID)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[AccountID](){
			RequestKickGuildMemberClass<int64>::GetInstance().Signal(AccountID);
		});
	}

	void Retailer::RequestGuildJoinBattle()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestJoinGuildBattleClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetGuildBattleInfo()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetGuildBattleClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestStartGuildBattle(int64 AccountId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[AccountId](){
			RequestStartGuildBattleClass<int64>::GetInstance().Signal(AccountId);
		});
	}

	void Retailer::RequestSetGuildBattleEntry(int32 slot1, int32 slot2, int32 slot3, int32 slot4, int32 slot5, int32 slot6, int32 slot7, int32 slot8)
	{
		/*// jwyim GuildEntry
		int32 entry1 = Slot1;		// 1팀 앞
		int32 entry2 = Slot2;		// 1팀 중간 위
		int32 entry3 = 10001;		// 1팀 중간 아래
		int32 entry4 = 10002;		// 1팀 뒤
		int32 entry5 = Slot3;		// 2팀 앞
		int32 entry6 = Slot4;		// 2팀 중간 위
		int32 entry7 = 10002;		// 2팀 중간 아래
		int32 entry8 = 10003;		// 2팀 뒤*/

		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[slot1, slot2, slot3, slot4, slot5, slot6, slot7, slot8](){
			RequestSetGuildBattleEntryClass<int32, int32, int32, int32, int32, int32, int32, int32>::GetInstance().Signal(
				slot1, slot2, slot3, slot4, slot5, slot6, slot7, slot8);
		});
	}

	void Retailer::RequestFinishGuildBattle(int64 TargetAccountID, const FString& Token, int32 Result, int32 LocalWinRound )
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[TargetAccountID, Token, Result, LocalWinRound](){
			RequestFinishGuildBattleClass<int64,const FString&, int32, int32>::GetInstance().Signal(TargetAccountID, Token, Result, LocalWinRound);
		});
	}

	void Retailer::RequestGetGuildInviteList()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetGuildInviteListClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetGuildMemberCandidates(bool NeedFriend)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[NeedFriend](){
			RequestGetGuildMemberCandidatesClass<bool>::GetInstance().Signal(NeedFriend);
		});
	}

	void Retailer::RequestInviteGuild(int64 account_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[account_id](){
			RequestInviteGuildClass<int64>::GetInstance().Signal(account_id);
		});
	}

	void Retailer::ReqeustRejectInviteGuild(int64 account_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[account_id](){
			RequestRejectInviteGuildClass<int64>::GetInstance().Signal(account_id);
		});
	}

	void Retailer::ReqeustAcceptInviteGuild(int64 account_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[account_id](){
			RequestAcceptInviteGuildClass<int64>::GetInstance().Signal(account_id);
		});
	}

	void Retailer::RequestGuildBattleHistory()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGuildBattleHistoryClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestUpdateGuild(int32 JoinOption, int32 MarkIndex, int32 MarkColor, int32 BG, int32 BGColor)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[JoinOption, MarkIndex, MarkColor, BG, BGColor](){
			RequestUpdateGuildClass<int32, int32, int32, int32, int32>::GetInstance().Signal(JoinOption, MarkIndex, MarkColor, BG, BGColor);
		});
	}

	void Retailer::RequestUpdateGuildNotice(const FString& Notice)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[Notice](){
			RequestUpdateGuildNoticeClass<const FString&>::GetInstance().Signal(Notice);
		});
	}

	// jwyim GuildFormation
	/*void Retailer::RequestEnhanceGuildBattleFormation(int32 FormationType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[FormationType]() {
			RequestEnhanceGuildBattleFormationClass<int32>::GetInstance().Signal(FormationType);
		});
	}*/

	void Retailer::RequestGetGuildRankingList()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestGetGuildRankingListClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGuildSeasonReward()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestGuildBattleSeasonRewardClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestFindGuild(const FString& GuildName)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[GuildName](){
			RequestFindGuildClass<const FString&>::GetInstance().Signal(GuildName);
		});
	}

	void Retailer::RequestGuildBattleTurnResult()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGuildBattleTurnResultClass<>::GetInstance().Signal(); 
		});
	}

	void Retailer::RequestGuildBattleTurnReward()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGuildBattleTurnRewardClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestLevelUpGuildSkill(int32 Skill_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[Skill_id]() {
			RequestLevelUpGuildSkillClass<int32>::GetInstance().Signal(Skill_id);
		});
	}

	void Retailer::RequestBuyGuildSkill(int32 Skill_id, int32 BuyCount)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[Skill_id, BuyCount]() {
			RequestBuyGuildSkillClass<int32, int32>::GetInstance().Signal(Skill_id, BuyCount);
		});
	}

	void Retailer::RequestHandOverGuildMaster(int64 account_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[account_id]() {
			RequestHandOverGuildMasterClass<int64>::GetInstance().Signal(account_id);
		});
	}

	void Retailer::RequesttObtainGuildMaster()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestObtainGuildMasterClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetServerVersion()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetServerVersionClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetHeroTower()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetHeroTowerClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestStartHeroTower(int32 floor, int32 main_character_type, int32 tag_character_type, const FString& play_token /*= TEXT("")*/)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[floor, main_character_type, tag_character_type, play_token](){
			RequestStartHeroTowerClass<int32, int32, int32, const FString&>::GetInstance().Signal(floor, main_character_type, tag_character_type, play_token);
		});
	}

	void Retailer::RequestFinishHeroTower(int32 floor, const FString& play_token, int32 clear_time_sec, bool is_success)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[floor, play_token, clear_time_sec, is_success](){
			RequestFinishHeroTowerClass<int32, const FString&, int32, bool>::GetInstance().Signal(floor, play_token, clear_time_sec, is_success);
		});
	}

	void Retailer::RequestAbandonHeroTower(int32 floor, const FString& play_token)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[floor, play_token]() {
			RequestAbandonHeroTowerClass<int32, const FString&>::GetInstance().Signal(floor, play_token);
		});
	}

	void Retailer::RequestSweepHeroTower()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestSweepHeroTowerClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestBuyHeroTowerTicket()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestBuyHeroTowerTicketClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestDrawShopLottery(EPCClass character_type, int32 draw_price_type, bool continuous_draw, bool is_draw_again,
		const FString& last_lottery_key, const bool isFreeLottery)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[character_type, draw_price_type, continuous_draw, is_draw_again, last_lottery_key, isFreeLottery](){
			//클라->서버 데이터로 컨버팅
			RequestDrawShopLotteryClass<int32, int32, bool, bool, const FString&, bool>::GetInstance().Signal(
				CliToSvrPCClassType(character_type), 
				draw_price_type, 
				continuous_draw,
				is_draw_again,
				last_lottery_key,
				isFreeLottery);
		});
	}

	void Retailer::RequestRewardShopMileage(int32 shop_mileage_type)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[shop_mileage_type](){
			RequestRewardShopMileageClass<int32>::GetInstance().Signal(shop_mileage_type + b2network::B2ShopMileageType::LOTTERY);
		});
	}

	void Retailer::RequestGetLotteryShop()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetLotteryShopClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestBuyShopProduct(int32 productId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[productId](){
			RequestBuyShopProductClass<int32>::GetInstance().Signal(productId);
		});
	}

	void Retailer::RequestGetGeneralShop(bool ReserveScene)
	{
		ReserveStoreSceneClass<bool>::GetInstance().Signal(ReserveScene);
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[](){
			RequestGetGeneralShopClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetMagicShop()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetMagicShopClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestRenewalMagicShop()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestRenewalMagicShopClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestPurchaseMagicShop(int32 magicShopItemId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[magicShopItemId]() {
			RequestPurchaseMagicShopClass<int32>::GetInstance().Signal(magicShopItemId);
		});
	}

	void Retailer::RequestGetCollectionItem(int32 characterType, int32 equipCategory)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[characterType, equipCategory](){
			RequestGetCollectionItemClass<int32, int32>::GetInstance().Signal(characterType, equipCategory);
		});
	}

	void Retailer::RequestRewardCollectionItem(int32 itemTemplateId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[itemTemplateId](){
			RequestRewardCollectionItemClass<int32>::GetInstance().Signal(itemTemplateId);
		});
	}

	void Retailer::RequestRewardCollectionSetItem(int32 characterType, int32 grade, int32 groupId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[characterType, grade, groupId](){
			RequestRewardCollectionSetItemClass<int32, int32, int32>::GetInstance().Signal(characterType, grade, groupId);
		});
	}

	void Retailer::RequestGetMarketProduct()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetMarketProductClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestPreparePurchase(int32 product_id, const FString& market_product_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[product_id, market_product_id]() {
			RequestPreparePurchaseClass<int32, const FString&>::GetInstance().Signal(product_id, market_product_id);
		});
	}
	
	void Retailer::RequestVerifyPurchase(const FString& market_product_id, const FString& developer_payload, const FString& purchase_data, const FString& purchase_signature, bool is_restore, const FString& currency, float price)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[market_product_id, developer_payload, purchase_data, purchase_signature, is_restore, currency, price]() {
			RequestVerifyPurchaseClass<const FString&, const FString&, const FString&, const FString&, bool, const FString&, float>::GetInstance().Signal(market_product_id, developer_payload, purchase_data, purchase_signature, is_restore, currency, price);
		});
	}

	void Retailer::RequestCancelPurchase(const FString& market_product_id, const FString& developer_payload, int32 cancel_reason)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[market_product_id, developer_payload, cancel_reason]() {
			RequestCancelPurchaseClass<const FString&, const FString&, int32>::GetInstance().Signal(market_product_id, developer_payload, cancel_reason);
		});
	}

	void Retailer::RequestFinishPurchase(const FString& developer_payload)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[developer_payload]() {
			RequestFinishPurchaseClass<const FString&>::GetInstance().Signal(developer_payload);
		});
	}

	void Retailer::RequestDailyAttendance()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestDailyAttendanceClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestSetWingVisible(int32 characterType, bool isVisible)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[characterType, isVisible]() {
			RequestSetWingVisibleClass<int32, bool>::GetInstance().Signal(characterType, isVisible);
		});
	}

	void Retailer::RequestSetHelmetVisible(int32 characterType, bool isVisible)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[characterType, isVisible]() {
			RequestSetHelmetVisibleClass<int32, bool>::GetInstance().Signal(characterType, isVisible);
		});
	}

	void Retailer::RequestGetRaid()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestGetRaidClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestResetAttendanceTime()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestResetAttendanceTimeClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestResetEventAttendance(bool isOnlyAttendanceTime)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[isOnlyAttendanceTime]() {
			RequestResetEventAttendanceClass<bool>::GetInstance().Signal(isOnlyAttendanceTime);
		});
	}

	void Retailer::RequestResetDailyPlayTimeRecord()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestResetDailyPlayTimeRecordClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestResetTeamMatchPoint()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestResetTeamMatchPointClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestEnhanceRelic(int32 character_type, int32 relic_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[character_type, relic_id]() {
			RequestEnhanceRelicClass<int32, int32>::GetInstance().Signal(character_type, relic_id);
		});
	}

	void Retailer::RequestPromotionRelic(int32 character_type, int32 relic_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[character_type, relic_id]() {
			RequestPromotionRelicClass<int32, int32>::GetInstance().Signal(character_type, relic_id);
		});
	}

	void Retailer::RequestGetAccountRelic(int32 character_type)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[character_type]() {
			RequestGetAccountRelicClass<int32>::GetInstance().Signal(character_type);
		});
	}

	void Retailer::RequestRaidResult(const FString& token, int32 ranking, bool isGetBonus, bool isReward, bool bGiveup)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[token, ranking, isGetBonus, isReward, bGiveup]() {
			RequestRaidResultClass<const FString&, int32, bool, bool, bool>::GetInstance().Signal(token, ranking, isGetBonus, isReward, bGiveup);
		});
	}

	void Retailer::RequestRaidResurrection(const FString& token, int32 selectedBuffIndex)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[token, selectedBuffIndex]() {
			RequestRaidResurrectionClass<const FString&, int32>::GetInstance().Signal(token, selectedBuffIndex);
		});
	}

	void Retailer::RequestAssaultMatchmaking(const int32 character_type)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[character_type]() {
			RequestAssaultMatchmakingClass<int32>::GetInstance().Signal(character_type);
		});
	}

	void Retailer::RequestCancelAssaultMatchmaking(const FString& token)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[token]() {
			RequestCancelAssaultMatchmakingClass<const FString&>::GetInstance().Signal(token);
		});
	}

	void Retailer::RequestAssaultResult(const FString token, int32 matchResult, bool is_mvp_player, bool isGhostUser, b2network::B2AssaultBattleInfoPtr battleInfo)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[token, matchResult, is_mvp_player, isGhostUser, battleInfo]() {
			RequestAssaultResultClass<const FString&, int32, bool, bool, b2network::B2AssaultBattleInfoPtr>::GetInstance().Signal(token, matchResult, is_mvp_player, isGhostUser, battleInfo);
		});
	}

	void Retailer::RequestAssaultResurrection(const FString& token)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[token]() {
			RequestAssaultResurrectionClass<const FString&> ::GetInstance().Signal(token);
		});
	}

	void Retailer::RequestAssaultBattleStatus()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestGetAssaultBattleStatusClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetItemRandomOption()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestGetItemRandomOptionClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGenerateItemRandomOption(int64 ItemId, int32 BuyPriceType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[ItemId, BuyPriceType]() {
			RequestGenerateItemRandomOptionClass<int64, int32>::GetInstance().Signal(ItemId, BuyPriceType);
		});
	}
	void Retailer::RequestApplyItemRandomOption(int64 ItemId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[ItemId]() {
			RequestApplyItemRandomOptionClass<int64>::GetInstance().Signal(ItemId);
		});
	}

	void Retailer::RequestDiscardItemRandomOption(int64 ItemId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[ItemId]() {
			RequestDiscardItemRandomOptionClass<int64>::GetInstance().Signal(ItemId);
		});
	}

	void Retailer::RequestGetItemRandomQuality(int64 ItemId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[ItemId]() {
			RequestGetItemRandomQualityClass<int64>::GetInstance().Signal(ItemId);
		});
	}
	void Retailer::RequestGenerateItemRandomQuality(int64 ItemId, int64 MaterialItemId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[ItemId, MaterialItemId]() {
			RequestGenerateItemRandomQualityClass<int64, int64>::GetInstance().Signal(ItemId, MaterialItemId);
		});
	}
	void Retailer::RequestApplyItemRandomQuality(int64 ItemId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[ItemId]() {
			RequestApplyItemRandomQualityClass<int64>::GetInstance().Signal(ItemId);
		});
	}

	void Retailer::RequestDiscardItemRandomQuality(int64 ItemId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[ItemId]() {
			RequestDiscardItemRandomQualityClass<int64>::GetInstance().Signal(ItemId);
		});
	}

	void Retailer::RequestCheckRedDot(const TArray<int32>& hintList)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[hintList]() {
			RequestCheckRedDotClass<const TArray<int32>&>::GetInstance().Signal(hintList);
		});
	}

	void Retailer::RequestCheckRedDot()
	{
		const TArray<int32> hintList;
		Retailer::RequestCheckRedDot(hintList);
	}

	void Retailer::RequestCheckChargePoints(bool CheckBladePoint, bool CheckPvPMatchPoint, bool CheckTeamMatchPoint)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[CheckBladePoint, CheckPvPMatchPoint, CheckTeamMatchPoint]() {
			RequestCheckChargePointsClass<bool, bool, bool>::GetInstance().Signal(CheckBladePoint, CheckPvPMatchPoint, CheckTeamMatchPoint);
		});
	}

	void Retailer::RequestIncreaseDarkCrystalPaid(int32 DarkCrystalPaid)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[DarkCrystalPaid]() {
			RequestIncreaseDarkCrystalPaidClass<int32>::GetInstance().Signal(DarkCrystalPaid);
		});
	}

	void Retailer::RequestCheckPvPMatchPointCharge()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestCheckPvPMatchPointChargeClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestCheckTeamMatchPointCharge()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[]() {
			RequestCheckTeamMatchPointChargeClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestFindAccount(int32 AccountID, EPCClass PCClass1, EPCClass PCClass2, EPCClass PCClass3, int32 _item_preset_id, int32 _skill_slot)
	{

		TArray<b2network::B2FindAccountQueryPtr> FindAccountInfoArr;

		b2network::B2FindAccountQueryPtr FindAccountInfo(new b2network::B2FindAccountQuery);
		FindAccountInfo->account_id = AccountID;
		if (PCClass1 != EPCClass::EPC_End)
			FindAccountInfo->character_types.Add(CliToSvrPCClassType(PCClass1));
		if (PCClass2 != EPCClass::EPC_End)
			FindAccountInfo->character_types.Add(CliToSvrPCClassType(PCClass2));
		if (PCClass3 != EPCClass::EPC_End)
			FindAccountInfo->character_types.Add(CliToSvrPCClassType(PCClass3));

		FindAccountInfo->item_preset_id = _item_preset_id;
		FindAccountInfo->skill_slot = _skill_slot;
		
		FindAccountInfoArr.Add(FindAccountInfo);

		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[FindAccountInfoArr]() {
			RequestFindAccountClass<const TArray<b2network::B2FindAccountQueryPtr>&>::GetInstance().Signal(FindAccountInfoArr);
		});
	}

	void Retailer::RequestFindAccount(int32 AccountID, EPCClass PCClass1 /*= EPCClass::EPC_End*/, EPCClass PCClass2 /*= EPCClass::EPC_End*/, EPCClass PCClass3 /*= EPCClass::EPC_End*/, EPCClass PCClass4 /*= EPCClass::EPC_End int32 _item_preset_id = 0*/, int32 _item_preset_id, int32 _skill_slot /*= 0*/)
	{
		TArray<b2network::B2FindAccountQueryPtr> FindAccountInfoArr;

		b2network::B2FindAccountQueryPtr FindAccountInfo(new b2network::B2FindAccountQuery);
		FindAccountInfo->account_id = AccountID;
		if (PCClass1 != EPCClass::EPC_End)
			FindAccountInfo->character_types.Add(CliToSvrPCClassType(PCClass1));
		if (PCClass2 != EPCClass::EPC_End)
			FindAccountInfo->character_types.Add(CliToSvrPCClassType(PCClass2));
		if (PCClass3 != EPCClass::EPC_End)
			FindAccountInfo->character_types.Add(CliToSvrPCClassType(PCClass3));
		if (PCClass4 != EPCClass::EPC_End)
			FindAccountInfo->character_types.Add(CliToSvrPCClassType(PCClass4));

		FindAccountInfo->item_preset_id = _item_preset_id;
		FindAccountInfo->skill_slot = _skill_slot;

		FindAccountInfoArr.Add(FindAccountInfo);

		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[FindAccountInfoArr]() {
			RequestFindAccountClass<const TArray<b2network::B2FindAccountQueryPtr>&>::GetInstance().Signal(FindAccountInfoArr);
		});
	}

	void Retailer::RequestModifyItemRandomOption(int64 ItemId, int32 SlotNum, int32 RandomOptionId, float RandomOptionValue)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[ItemId, SlotNum, RandomOptionId, RandomOptionValue]() {
			RequestModifyItemRandomOptionClass<int64, int32, int32, float>::GetInstance().Signal(ItemId, SlotNum, RandomOptionId, RandomOptionValue);
		});
	}

	void Retailer::RequestModifyItemNewFlag(int32 characterType, int32 inventoryType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[characterType, inventoryType]() {
			RequestModifyItemNewFlagClass<int32, int32>::GetInstance().Signal(characterType, inventoryType);
		});
	}

	void Retailer::RequestModifyCostumeNewFlag(int32 characterType, int32 inventoryType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[characterType, inventoryType]() {
			RequestModifyCostumeNewFlagClass<int32, int32>::GetInstance().Signal(characterType, inventoryType);
		});
	}

	void Retailer::RequestStartRaid(int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[characterType]() {
			RequestStartRaidClass<int32>::GetInstance().Signal(characterType);
		});
	}

	void Retailer::RequestGetContentsOpenStatus(int32 linkKey)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
		[linkKey]() {
			RequestGetContentsOpenStatusClass<int32>::GetInstance().Signal(linkKey);
		});
	}

	void Retailer::RequestGetDuelRewardBoxInfo(int32 duelMode)
	{
		RequestGetDuelRewardBoxInfoClass<int32>::GetInstance().Signal(duelMode);
	}

	void Retailer::RequestGetDuelRewardBoxGainableList(int32 duelMode, int32 boxGrade)
	{
		RequestGetDuelRewardBoxGainableListClass<int32, int32>::GetInstance().Signal(duelMode, boxGrade);
	}

	void Retailer::RequestOpenDuelRewardBox(int32 duelMode)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[duelMode]() {
			RequestOpenDuelRewardBoxClass<int32>::GetInstance().Signal(duelMode);
		});
	}

	void Retailer::RequestReceiveAssaultMvpReward()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestReceiveAssaultMvpRewardClass<>::GetInstance().Signal();
		});
	}
	
	void Retailer::RequestEventAttendance(int32 eventId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId]() {
			RequestEventAttendanceClass<int32>::GetInstance().Signal(eventId);
		});
	}

	void Retailer::RequestBeginnerAttendance()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestBeginnerAttendanceClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestUpdateDailyPlayTime(int32 playTimeInSec)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[playTimeInSec]() {
			RequestUpdateDailyPlayTimeClass<int32>::GetInstance().Signal(playTimeInSec);
		});
	}

	void Retailer::RequestReceiveDailyPlayTimeReward()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestReceiveDailyPlayTimeRewardClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestAskGuildSupport(int32 ItemTempleteID)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[ItemTempleteID]() {
			RequestAskGuildSupportClass<int32>::GetInstance().Signal(ItemTempleteID);
		});
	}

	void Retailer::RequestGuildSupport(int64 TargetAccountId, int64 ChatId, int32 Count)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[TargetAccountId, ChatId, Count]() {
			RequestGuildSupportClass<int64, int64, int32>::GetInstance().Signal(TargetAccountId, ChatId, Count);
		});
	}

	void Retailer::RequestGetGuildChat(int32 ChatId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[ChatId]() {
			RequestGetGuildChatClass<int32>::GetInstance().Signal(ChatId);
		});
	}

	void Retailer::RequestGetAccountAether(int32 aetherType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[aetherType]() {
			RequestGetAccountAetherClass<int32>::GetInstance().Signal(aetherType);
		});
	}

	void Retailer::RequestDismantleAethers(const TArray<int64>& aether_ids)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[aether_ids]() {
			RequestDismantleAethersClass<const TArray<int64>&>::GetInstance().Signal(aether_ids);
		});
	}

	void Retailer::RequestMountAethers(const TArray<int64>& aether_ids, int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[aether_ids, characterType]() {
			RequestMountAethersClass<const TArray<int64>&, int32>::GetInstance().Signal(aether_ids, characterType);
		});
	}

	void Retailer::RequestUnmountAethers(const TArray<int64>& aether_ids, int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[aether_ids, characterType]() {
			RequestUnmountAethersClass<const TArray<int64>&, int32>::GetInstance().Signal(aether_ids, characterType);
		});
	}

	void Retailer::RequestLockAethers(const TArray<int64>& aether_ids)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[aether_ids]() {
			RequestLockAethersClass<const TArray<int64>&>::GetInstance().Signal(aether_ids);
		});
	}

	void Retailer::RequestUnlockAethers(const TArray<int64>& aether_ids)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[aether_ids]() {
			RequestUnlockAethersClass<const TArray<int64>&>::GetInstance().Signal(aether_ids);
		});
	}

	void Retailer::RequestExtendAetherInventory(int32 buyCount)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[buyCount]() {
			RequestExtendAetherInventoryClass<int32>::GetInstance().Signal(buyCount);
		});
	}

	void Retailer::RequestEnhanceAether(int64 aetherId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[aetherId]() {
			RequestEnhanceAetherClass<int64>::GetInstance().Signal(aetherId);
		});
	}

	void Retailer::RequestModifyAetherNewFlag(int32 aetherType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[aetherType]() {
			RequestModifyAetherNewFlagClass<int32>::GetInstance().Signal(aetherType);
		});
	}

	void Retailer::RequestSelectAetherDefenseOption(int32 setOptionId, int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[setOptionId, characterType]() {
			RequestSelectAetherDefenseOptionClass<int32, int32>::GetInstance().Signal(setOptionId, characterType);
		});
	}

	void Retailer::RequestSelectAetherUnitySkillOption(int32 setOptionId, int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[setOptionId, characterType]() {
			RequestSelectAetherUnitySkillOptionClass<int32, int32>::GetInstance().Signal(setOptionId, characterType);
		});
	}

	void Retailer::RequestSellAethers(const TArray<int64>& aether_ids)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[aether_ids]() {
			RequestSellAethersClass<const TArray<int64>&>::GetInstance().Signal(aether_ids);
		});
	}

	void Retailer::RequestUnitySkillMissions(int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[characterType]() {
			RequestGetUnitySkillMissionClass<int32>::GetInstance().Signal(characterType);
		});
	}

	void Retailer::RequestAwakenUnitySkillMission(int32 characterType, int32 missionId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[characterType, missionId]() {
			RequestAwakenUnitySkillMissionClass<int32, int32>::GetInstance().Signal(characterType, missionId);
		});
	}

	void Retailer::RequestAwakenUnitySkill(int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[characterType]() {
			RequestAwakenUnitySkillClass<int32>::GetInstance().Signal(characterType);
		});
	}

	void Retailer::RequestReceiveUnitySkillReward(int32 mainCharacterType, int32 unityCharacterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[mainCharacterType, unityCharacterType]() {
			RequestReceiveUnitySkillRewardClass<int32, int32>::GetInstance().Signal(mainCharacterType, unityCharacterType);
		});
	}

	void Retailer::RequestGetDuelModeInfo()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetDuelModeInfoClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetChallengeModeInfo()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetChallengeModeInfoClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestReceiveRestReward(int32 rewardId, bool isDoubleReward)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[rewardId, isDoubleReward]() {
			RequestReceiveRestRewardClass<int32, bool>::GetInstance().Signal(rewardId, isDoubleReward);
		});
	}

	void Retailer::RequestCheckModeOpen()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestCheckModeOpenClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetGuildMember()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetGuildMemberClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetHallOfFame(int32 mode, bool needUserInfo)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[mode, needUserInfo]() {
			RequestGetHallOfFameClass<int32, bool>::GetInstance().Signal(mode, needUserInfo);
		});
	}

	void Retailer::RequestPraiseTargetRanker(int32 mode, int64 accountId, int32 ranking)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[mode, accountId, ranking]() {
			RequestPraiseTargetRankerClass<int32, int64, int32>::GetInstance().Signal(mode, accountId, ranking);
		});
	}

	void Retailer::RequestGetTutorialProgress()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetTutorialProgressClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestSetTutorialProgress()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestSetTutorialProgressClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestUpdateTutorialProgress()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestUpdateTutorialProgressClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestSkipTutorialProgress()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestSkipTutorialProgressClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestDrawTutorialItem(int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[characterType]() {
			RequestDrawTutorialItemClass<int32>::GetInstance().Signal(characterType);
		});
	}

	void Retailer::RequestPrepareTutorial(int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[characterType]() {
			RequestPrepareTutorialClass<int32>::GetInstance().Signal(characterType);
		});
	}

	void Retailer::RequestGiveTutorialReward()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGiveTutorialRewardClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestCheckReachPower(const TArray<b2network::B2CharacterPowerPtr>& characterPower)
	{
		RequestCheckReachPowerClass<const TArray<b2network::B2CharacterPowerPtr>&>::GetInstance().Signal(characterPower);
	}

	void Retailer::RequestTrackingPowerDetail(int32 characterType, int32 itemPresetId, int32 skillSlot, const TArray<b2network::B2CharacterPowerSpecificPtr>& powerSpecifics)
	{
		RequestTrackingPowerDetailClass<int32, int32, int32, const TArray<b2network::B2CharacterPowerSpecificPtr>&>::GetInstance().Signal(characterType, itemPresetId, skillSlot, powerSpecifics);
	}

	void Retailer::RequestKakaoMessageProcessing(const FString& message_box_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[message_box_id]() {
			RequestKakaoMessageProcessingClass<const FString&>::GetInstance().Signal(message_box_id);
		});
	}

	void Retailer::RequestKakaoCouponUse(const FString& coupon_code)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[coupon_code]() {
			RequestKakaoCouponUseClass<const FString&>::GetInstance().Signal(coupon_code);
		});
	}

	void Retailer::RequestKakaoAccountConversionPrepare(const FString& idp_code)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[idp_code]() {
			RequestKakaoAccountConversionPrepareClass<const FString&>::GetInstance().Signal(idp_code);
		});
	}

	void Retailer::RequestKakaoAccountConversionComplete(const FString& idp_code)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[idp_code]() {
			RequestKakaoAccountConversionCompleteClass<const FString&>::GetInstance().Signal(idp_code);
		});
	}

	void Retailer::RequestSkipMatchTutorial(int32 matchType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[matchType]() {
			RequestSkipMatchTutorialClass<int32>::GetInstance().Signal(matchType);
		});
	}

	void Retailer::RequestGetEvent()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetEventClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestReceivePointEventReward(int32 eventId, int32 rewardIndex)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId, rewardIndex]() {
			RequestReceivePointEventRewardClass<int32, int32>::GetInstance().Signal(eventId, rewardIndex);
		});
	}

	void Retailer::RequestReceiveLevelUpEventReward(int32 eventId, int32 index)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId, index]() {
			RequestReceiveLevelUpEventRewardClass<int32, int32>::GetInstance().Signal(eventId, index);
		});
	}

	void Retailer::RequestReceiveStageClearEventReward(int32 eventId, int32 index)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId, index]() {
			RequestReceiveStageClearEventRewardClass<int32, int32>::GetInstance().Signal(eventId, index);
		});
	}

	void Retailer::RequestGetSeasonMission()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetSeasonMissionClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestRewardSeasonMission(int32 eventId, int32 day, int32 index)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId, day, index]() {
			RequestRewardSeasonMissionClass<int32, int32, int32>::GetInstance().Signal(eventId, day, index);
		});
	}

	void Retailer::RequestAttendSeasonMission()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestAttendSeasonMissionClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestBuyHotTimeBuff(int32 buffType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[buffType]() {
			RequestBuyHotTimeBuffClass<int32>::GetInstance().Signal(buffType);
		});
	}

	void Retailer::RequestRewardFlatRatePackageBonus()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestRewardFlatRatePackageBonusClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetPackageShop()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetPackageShopClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestGetPeriodPackageState()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetPeriodPackageStateClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestRewardLevelUpPackage(const int packageId, const int32 characterType, const int level)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[packageId, characterType, level]() {
			RequestRewardLevelUpPackageClass<int32, int32, int32>::GetInstance().Signal(packageId, characterType, level);
		});
	}

	void Retailer::RequestRewardReachLevelPackage(const int packageId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[packageId]() {
			RequestRewardReachLevelPackageClass<int32>::GetInstance().Signal(packageId);
		});
	}

	void Retailer::RequestGiveUpQuest(const int slot)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[slot]() {
			RequestGiveUpQuestClass<int32>::GetInstance().Signal(slot);
		});
	}

	void Retailer::RequestRewardTenLotteryEvent(const int eventId, const int index)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId, index]() {
			RequestRewardTenLotteryEventClass<int32, int32>::GetInstance().Signal(eventId, index);
		});
	}

	void Retailer::RequestRewardSpendGemEvent(const int eventId, const int index)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId, index]() {
			RequestRewardSpendGemEventClass<int32, int32>::GetInstance().Signal(eventId, index);
		});
	}

	void Retailer::RequestReceiveGemPurchaseEventReward(const int eventId, const int index)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId, index]() {
			RequestReceiveGemPurchaseEventRewardClass<int32, int32>::GetInstance().Signal(eventId, index);
		});
	}

	void Retailer::RequestRewardPointShopEvent(int32 eventId, int32 index, int32 buyCount/* = 1*/)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId, index, buyCount]() {
			RequestRewardPointShopEventClass<int32, int32, int32>::GetInstance().Signal(eventId, index, buyCount);
		});
	}

	void Retailer::RequestReportUser(int64 targetAccountId, int32 modeType, int32 reasonType, const FString& explain)
	{
		// to channel
		RequestReportUserViaChannelClass<int64, int32, int32, const FString&>::GetInstance().Signal(targetAccountId, modeType, reasonType, explain);

		// to session
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[targetAccountId, modeType, reasonType, explain]() {
			RequestReportUserClass<int64, int32, int32, const FString&>::GetInstance().Signal(targetAccountId, modeType, reasonType, explain);
		});
	}

	void Retailer::RequestBlockChat(int32 accountId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[accountId]() {
			RequestBlockChatClass<int64>::GetInstance().Signal(accountId);
		});
	}

	void Retailer::RequestUnblockChat(int32 accountId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[accountId]() {
			RequestUnblockChatClass<int64>::GetInstance().Signal(accountId);
		});
	}

	void Retailer::RequestGetBlockChatList()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetBlockChatListClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestClientStateReport(const FString& category, const FString& event_name, const FString& label, const FString& detail_info)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[category, event_name, label, detail_info]() {
			RequestClientStateReportClass<const FString&, const FString&, const FString&, const FString&>::GetInstance().Signal(category, event_name, label, detail_info);
		});
	}

	void Retailer::RequestGuildDonation(int32 mercenaryid, int32 donationtype)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[mercenaryid, donationtype]() {
			RequestGuildDonationClass<int32, int32>::GetInstance().Signal(mercenaryid, donationtype);
		});
	}

	void Retailer::RequestRewardTenLotterySavingsEvent(const int eventId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId]() {
			RequestRewardTenLotterySavingsEventClass<int32>::GetInstance().Signal(eventId);
		});
	}

	void Retailer::RequestRollDiceEvent(const int eventId, const bool isFreeDice)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId, isFreeDice]() {
			RequestRollDiceEventClass<const int32, const bool>::GetInstance().Signal(eventId, isFreeDice);
		});
	}

	void Retailer::RequestBuyDiceEventPoint(const int eventId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId]() {
			RequestBuyDiceEventPointClass<const int32>::GetInstance().Signal(eventId);
		});
	}

	void Retailer::RequestRewardDiceEventMileage(const int eventId, const int rewardIndex)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[eventId, rewardIndex]() {
			RequestRewardDiceEventMileageClass<const int32, const int32>::GetInstance().Signal(eventId, rewardIndex);
		});
	}

	void Retailer::RequestInvitation(const FString& invitee, const FString& room_id)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[invitee, room_id]() {
			RequestInvitationClass<const FString&, const FString&>::GetInstance().Signal(invitee, room_id);
		});
	}

	void Retailer::RequestAcceptInvitation(const int invitation_type, const FString& inviter, const FString& invitation_ref, const FString& invitee, b2network::B2RoomLocationPtr room_location)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[invitation_type, inviter, invitation_ref, invitee, room_location]() {
			RequestAcceptInvitationClass<const int32, const FString&, const FString&, const FString&, b2network::B2RoomLocationPtr>
				::GetInstance().Signal(invitation_type, inviter, invitation_ref, invitee, room_location);
		});
	}

	void Retailer::RequestDeclineInvitation(const int invitation_type, const FString& inviter, const FString& invitation_ref, const FString& invitee)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[invitation_type, inviter, invitation_ref, invitee]() {
			RequestDeclineInvitationClass<const int32, const FString&, const FString&, const FString&>
				::GetInstance().Signal(invitation_type, inviter, invitation_ref, invitee);
		});
	}

	void Retailer::RequestGetAccountCostume(int32 CharacterType, int32 InventoryType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[CharacterType, InventoryType]() {
			RequestGetAccountCostumeClass<int32, int32>::GetInstance().Signal(CharacterType, InventoryType);
		});
	}

	void Retailer::RequestEquipCostume(const TArray<int64>& CostumeIds)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[CostumeIds]() {
			RequestEquipCostumeClass<const TArray<int64>&>::GetInstance().Signal(CostumeIds);
		});
	}

	void Retailer::RequestUnequipCostume(const TArray<int64>& CostumeIds)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[CostumeIds]() {
			RequestUnequipCostumeClass<const TArray<int64>&>::GetInstance().Signal(CostumeIds);
		});
	}

	void Retailer::RequestEnhanceCostume(int64 CostumeId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[CostumeId]() {
			RequestEnhanceCostumeClass<int64>::GetInstance().Signal(CostumeId);
		});
	}

	void Retailer::RequestDismantleCostumes(const TArray<int64>& CostumeIds)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[CostumeIds]() {
			RequestDismantleCostumesClass<const TArray<int64>&>::GetInstance().Signal(CostumeIds);
		});
	}

	void Retailer::RequestLockCostumes(const TArray<int64>& CostumeIds)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[CostumeIds]() {
			RequestLockCostumesClass<const TArray<int64>&>::GetInstance().Signal(CostumeIds);
		});
	}

	void Retailer::RequestUnlockCostumes(const TArray<int64>& CostumeIds)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[CostumeIds]() {
			RequestUnlockCostumesClass<const TArray<int64>&>::GetInstance().Signal(CostumeIds);
		});
	}

	void Retailer::RequestGetCostumeShop()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetCostumeShopClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestPurchaseCostume(int32 ProductId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[ProductId]() {
			RequestPurchaseCostumeClass<int32>::GetInstance().Signal(ProductId);
		});
	}

	void Retailer::RequestSetCostumeVisible(int32 CharacterType, int32 CostumeType, bool IsVisible)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[CharacterType, CostumeType, IsVisible]() {
			RequestSetCostumeVisibleClass<int32, int32, bool>::GetInstance().Signal(CharacterType, CostumeType, IsVisible);
		});
	}

	void Retailer::RequestGetAccountTotem()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestGetAccountTotemClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestEquipTotems(const TArray<int64>& totemIds, int32 characterType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[totemIds, characterType]() {
			RequestEquipTotemsClass<const TArray<int64>&, int32>::GetInstance().Signal(totemIds, characterType);
		});
	}

	void Retailer::RequestUnequipTotems(const TArray<int64>& totemIds)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[totemIds]() {
			RequestUnequipTotemsClass<const TArray<int64>&>::GetInstance().Signal(totemIds);
		});
	}

	void Retailer::RequestLockTotems(const TArray<int64>& totemIds)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[totemIds]() {
			RequestLockTotemsClass<const TArray<int64>&>::GetInstance().Signal(totemIds);
		});
	}

	void Retailer::RequestUnlockTotems(const TArray<int64>& totemIds)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[totemIds]() {
			RequestUnlockTotemsClass<const TArray<int64>&>::GetInstance().Signal(totemIds);
		});
	}

	void Retailer::RequestSellTotems(const TArray<int64>& totemIds)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[totemIds]() {
			RequestSellTotemsClass<const TArray<int64>&>::GetInstance().Signal(totemIds);
		});
	}

	void Retailer::RequestDismantleTotems(const TArray<int64>& totemIds)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[totemIds]() {
			RequestDismantleTotemsClass<const TArray<int64>&>::GetInstance().Signal(totemIds);
		});
	}

	void Retailer::RequestRefineTotem(int64 totemId, int32 optionId, int32 costType)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[totemId, optionId, costType]() {
			RequestRefineTotemClass<int64, int32, int32>::GetInstance().Signal(totemId, optionId, costType);
		});
	}

	void Retailer::RequestModifyTotemNewFlag()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestModifyTotemNewFlagClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestAddTotemToInventory(int32 templateId)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[templateId]() {
			RequestAddTotemToInventoryClass<int32>::GetInstance().Signal(templateId);
		});
	}

	void Retailer::RequestItemForgeList()
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[]() {
			RequestItemForgeListClass<>::GetInstance().Signal();
		});
	}

	void Retailer::RequestItemForge(int64 ForgeID, int64 Quantity, int64 MainItemUID, int64 MaterialItemUID1, int64 MaterialItemUID2,int64 MaterialItemUID3,int64 MaterialItemUID4, int64 MainItemRefID, int64 MaterialItemRefID1, int64 MaterialItemRefID2, int64 MaterialItemRefID3, int64 MaterialItemRefID4)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[ForgeID, Quantity, MainItemUID, MaterialItemUID1, MaterialItemUID2, MaterialItemUID3, MaterialItemUID4, MainItemRefID, MaterialItemRefID1, MaterialItemRefID2, MaterialItemRefID3, MaterialItemRefID4] {
			RequestItemForgeClass<int64, int64, int64, int64, int64, int64, int64, int64, int64, int64, int64, int64>::GetInstance().Signal(
				ForgeID, Quantity, MainItemUID, MaterialItemUID1, MaterialItemUID2, MaterialItemUID3, MaterialItemUID4, MainItemRefID, MaterialItemRefID1, MaterialItemRefID2, MaterialItemRefID3, MaterialItemRefID4
			);
		});
	}

	void Retailer::RequestUnsealBox(int64 sealboxUID, int32 PCClass, int64 openAmount)
	{
		FBladeIIBlockToSyncNetwork::GetInstance().SetBlockTillResponse(
			[sealboxUID, PCClass, openAmount] {
			RequestUnsealBoxClass<int64, int32, int64>::GetInstance().Signal(
				sealboxUID, PCClass, openAmount
			);
		});
	}

	// REPLACEMENT POINT: RETAILER CPP REQUEST
	// NEVER DELETE LINE ABOVE
}