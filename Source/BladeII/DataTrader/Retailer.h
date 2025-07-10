#pragma once
//#include "BladeII.h"
#include <memory>
#include "B2MessageInfoCommonStruct.h"
#include "CommonStruct.h"

namespace data_trader
{
	class Retailer
	{
	public:
		static 	Retailer& GetInstance();

		void	RequestChangeNickname(const FString&, int32);
		void	RequestGetAccount();
		void	RequestGetAccountItem(int32, int32);
		void	RequestGetMasterData();
		void	RequestGetLatestEventData(int64);
		void	RequestDismantleItems(const TArray<int64>&);
		void	RequestSellItem(const TArray<int64>&);
		void	RequestSellConsumableItem(int64, int32);
		void	RequestConvertData(int32);
		void	RequestConvertItem(int32, int32);
		void	RequestEquipItem(int32, const TArray<int64>&);
		void	RequestUnequipItem(int32, const TArray<int64>&);
		void	ResponseUnequipItem(const FB2UnequippedItems&);
		void	RequestLockItem(const TArray<int64>&);
		void	RequestUnlockItem(const TArray<int64>&);
		void	RequestEnhanceItem(int64, int32, bool, int32);
		void	RequestExchangeItemEnhanceLevel(int64, int64, int32);
		void	RequestOpenSealSlotItem(int64, int32);
		void	RequestCarveSealItem(int64, int32);
		void	RequestLevelupItem(int64, const TArray<int64>&, bool);
		void	RequestGetItemSurpassCost(int64);
		void	RequestSurpassItem(int64, int64);
		void	RequestGetItemUpgradeCost(int64);
		void	RequestUpgradeItem(int64);
		void	RequestGetItemComposeCost(int64, int32);
		void	RequestComposeItem(int64, int64, int32);
		void	RequestChangeItemCharacterType(int64, int32);
		void	RequestGetChangeItemCharacterTypeCost(int64);
		void	RequestExtendInventory(int32, int32);
		void	RequestStartStage(FServerStageID, int32, int32, int32, bool, bool, int32, int32, int32);
		void	RequestResurrection(const FString&, int32);
		void	RequestClearStage(FServerStageID, const FString&, int32, int32, int32, const TArray<b2network::B2KillMonsterInfoPtr>&, int32, bool, int32, int32, b2network::B2ItemSpotSaleOptionPtr, b2network::B2AetherSpotSaleOptionPtr b2AetherSpotSaleOptionPtr);
		void	RequestFailStage(FServerStageID, const FString&, int32, int32);
		void	RequestSweepStage(FServerStageID, int32, int32, int32, int32, int32);
		void	RequestCheckBladePointCharge();
		void	RequestGetActInfo(int32, int32);
		void	RequestGetStageInfo(FServerStageID);
		void	RequestGetActClearReward(int32, int32, int32);
		void	RequestGetActRepeatClearReward(int32, int32);
		void	RequestGetContentsModeState(int32 ContentsMode = 0);
		void	RequestGetMatchStatus();
		void	RequestMatchmaking(int32, int32, int32, bool, FString const& room_seed=TEXT(""));
		void	RequestLeaveMatch(FString const&);
		void	RequestKickUser(FString const&, int64);
		void	RequestCancelMatchmaking();
		void	RequestMatchResult(int64, const FString&,int32, int32, b2network::B2RoomCharacterPtr, b2network::B2RoomCharacterPtr, b2network::B2RoomCharacterPtr, b2network::B2RoomCharacterPtr, const FString&, bool bGiveup = false);
		void	RequestEnhanceTeamBattleFormation(ETMFormation);
		void	RequestUpdateTeamBattleEntry(ETMFormation, EPCClass, EPCClass, EPCClass);
		void	RequestGetTeamBattleStatus();
		void	RequestTeamMatchmaking(bool);
		void	RequestTeamMatchResult(const FString&, bool, bool bGiveup = false);
		void	RequestRaidStart(int32);
		void	RequestRaidMatchmaking(int32, int32, int32, int32, b2network::B2EndpointPtr);
		void	RequestRoomInvitation(const FString&, const FString&, int32,int32, int32);
		void	RequestRequestJoinRoom(const FString&, int32, int32, TArray<b2network::B2RoomPropertyPtr>&, TArray<b2network::B2RoomCharacterPtr>&, TArray<b2network::B2PresetNumPtr>&);
		void	RequestRequestJoinProperty(const FString&, TArray<b2network::B2RoomPropertyPtr>&);
		void	RequestRoomClose(const FString&, const int32 mapId, const ERoomCloseReasonType reason);
		void	RequestRoomLeaveResult(const FString&, const int64, const FString&);
		void	RequestRoomAccessControl(const FString&, int32);		
		void	RequestLevelUpSkill(int32, int32);
		void	RequestResetSkill(int32);
		void	RequestBuySkillPoint(const TArray<b2network::B2BuySkillPointRequestInfoPtr>&);
		void	RequestSetUsingSkill(int32, int32, int32, int32, int32);
		void	RequestSetNickName(FString);
		void	RequestSetCharNickName(int32, const FString&);
		void	RequestGetRank();
		void	RequestEnhanceRankNode(int32);
		void	RequestActivateRankNode(int32);
		void	RequestGetMailList(int32);
		void	RequestOpenMail(int64, int32, int32, int32);
		void	RequestOpenAllMail(int32, int64, int64);
		void	RequestGetNewMailCount();
		void	RequestAskFriend(int64);
		void	RequestAcceptAskFriend(int64);
		void	RequestRejectAskFriend(int64);
		void	RequestGetFriendList();
		void	RequestCancelAskFriend(int64);
		void	RequestDeleteFriend(int64);
		void	RequestGetAllSendAskFriend();
		void	RequestGetAllReceiveAskFriend();
		void	RequestSendSocialPoint(TArray<int64>&);
		void	RequestConfirmNewFriend(bool, TArray<int64>&);
		void	RequestGetRecommendFriend();
		void	RequestPlatformFriendInviteInfo(int32);
		void	RequestPlatformFriendInviteCountUp(int32);
		void	RequestPlatformFriendJoinerCountUp(int32, int32);
		void	RequestPlatformFriendInviteReward(int32, int32, int32);
		void	RequestFindUser(const FString&, bool guild_invite_state = false);
		void	RequestGetFriendRankings(int32 opponentType);
		void	RequestFindGuild(const FString& GuildName);
		void	RequestGetCounterDungeon();
		void	RequestGetChatChannel();
		void	RequestChangeChatChannel(int32);
		void	RequestGetQuest();
		void	RequestAcceptQuest(int32 slot);
		void	RequestReceiveQuestReward(int32 slot);
		void	RequestGetMission(int32);
		void	RequestGetFeaturedMission();
		void	RequestReceiveMissionReward(int32 slot, bool bSyncNetwork = true);
		void	RequestReceiveCompleteMissionReward(int32 slot, int32 rewardIndex);
		void	RequestGetWing(int32);
		void	RequestEnhanceWing(int32);
		void	RequestUpgradeWing(int32);
		void	RequestEnhanceWingOption(int32, int32);
		void	RequestGetDonationEvent();
		void	RequestMakeDonation(int32, bool, bool, bool, const FString&);
		void	RequestDonationPointReward(int32, int32);
		void	RequestGetFairy();
		void	RequestLevelupFairy(int32);
		void	RequestBlessFairy(int32, int32);
		void	RequestReceiveFairyGift(int32);
		void	RequestStartCounterDungeon(FServerStageID InServerStageNum, int32 MainSvrClass);
		void	RequestFinishCounterDungeon(FServerStageID, const FString&, bool, bool bGiveup = false);
		void	RequestSweepCounterDungeon(FServerStageID InServerStageNum, int32 MainSvrClass);
		void	RequestWeeklyRewardCounterDungeon(int32);
		void	RequestBuyTicketCounterDungeon();
		void	RequestGetDimension();
		void	RequestStartDimension(int32 difficultyLevel, int32 dungeonId, int32 MainSvrClass, int32 totalPower);
		void	RequestFinishDimension(int32 difficultyLevel, const FString& playToken, int32 playResult, int32 playTimeMillis);
		void	RequestGuildAskJoinGuild(int64);
		void	RequestCreateGuild(const FString&, int32, int32, int32, int32, int32);
		void	RequestGuildDetailInfo(int64);
		void	RequestLeaveGuild();
		void	RequestGetRecommendGuild();
		void	RequestAppointSubGuildMaster(int64);
		void	RequestDismissSubGuildMaster(int64);
		void	RequestKickGuildMember(int64);
		void	RequestGuildJoinBattle();
		void	RequestGetGuildBattleInfo();
		void	RequestStartGuildBattle(int64);
		void	RequestSetGuildBattleEntry(int32, int32, int32, int32, int32, int32, int32, int32);
		void	RequestFinishGuildBattle(int64,const FString&, int32, int32);
		void	RequestGetGuildInviteList();
		void	RequestGetGuildMemberCandidates(bool);
		void	RequestInviteGuild(int64 account_id);
		void	ReqeustRejectInviteGuild(int64 account_id);
		void	ReqeustAcceptInviteGuild(int64 account_id);
		void	RequestGuildBattleHistory();
		void	RequestUpdateGuild(int32, int32, int32, int32, int32);
		void	RequestUpdateGuildNotice(const FString&);
		void	RequestGetGuildRankingList();
		void	RequestGuildSeasonReward();
		void	RequestGuildBattleTurnResult();
		void	RequestGuildBattleTurnReward();
		void	RequestLevelUpGuildSkill(int32 Skill_id);
		void	RequestBuyGuildSkill(int32 Skill_id, int32 BuyCount);
		void	RequestHandOverGuildMaster(int64 account_id);
		void	RequesttObtainGuildMaster();
		void	RequestGetServerVersion();

		void	RequestGetHeroTower();
		void	RequestStartHeroTower(int32 floor, int32 main_character_type, int32 tag_character_type, const FString& play_token = TEXT(""));
		void	RequestFinishHeroTower(int32 floor, const FString& play_token, int32 clear_time_sec, bool is_success);
		void	RequestAbandonHeroTower(int32 floor, const FString& play_token);
		void	RequestSweepHeroTower();
		void	RequestBuyHeroTowerTicket();
		void	RequestDrawShopLottery(EPCClass character_type, int32 draw_price_type, bool continuous_draw, bool is_draw_again, const FString& last_lottery_key, const bool isFreeLottery);
		void	RequestRewardShopMileage(int32 shop_mileage_type);
		void	RequestGetLotteryShop();
		void	RequestBuyShopProduct(int32 productId);
		void	RequestGetGeneralShop(bool ReserveScene = true);
		void	RequestGetMagicShop();
		void	RequestRenewalMagicShop();
		void	RequestPurchaseMagicShop(int32 magicShopItemId);
		void	RequestGetCollectionItem(int32 characterType, int32 equipCategory);
		void	RequestRewardCollectionItem(int32 itemTemplateId);
		void	RequestRewardCollectionSetItem(int32 characterType, int32 grade, int32 groupId);
		void	RequestGetMarketProduct();
		void	RequestPreparePurchase(int32 product_id, const FString& market_product_id);
		void	RequestVerifyPurchase(const FString& market_product_id, const FString& developer_payload, const FString& purchase_data, const FString& purchase_signature, bool is_restore, const FString& currency, float price);
		void	RequestCancelPurchase(const FString& market_product_id, const FString& developer_payload, int32 cancel_reason);
		void	RequestFinishPurchase(const FString& developer_payload);
		void	RequestDailyAttendance();
		void	RequestSetWingVisible(int32 characterType, bool isVisible);
		void	RequestSetHelmetVisible(int32 characterType, bool isVisible);
		void	RequestGetRaid();
		void	RequestResetAttendanceTime();
		void	RequestResetEventAttendance(bool isOnlyAttendanceTime);
		void	RequestResetDailyPlayTimeRecord();
		void	RequestResetTeamMatchPoint();
		void	RequestEnhanceRelic(int32 characterType, int32 relic_id);
		void	RequestPromotionRelic(int32 characterType, int32 relic_id);
		void	RequestGetAccountRelic(int32 characterType);
		void	RequestRaidResult(const FString&, int32, bool, bool, bool bGiveup = false);
		void	RequestRaidResurrection(const FString&, int32);
		void	RequestAssaultMatchmaking(const int32 character_type);
		void	RequestCancelAssaultMatchmaking(const FString&);
		void	RequestAssaultResult(const FString token, int matchResult, bool is_mvp_player, bool isGhostUser, b2network::B2AssaultBattleInfoPtr battleInfo);
		void	RequestAssaultResurrection(const FString&);
		void	RequestAssaultBattleStatus();
		void	RequestGetItemRandomOption();
		void	RequestGenerateItemRandomOption(int64 ItemId, int32 BuyPriceType);
		void	RequestApplyItemRandomOption(int64 ItemId);
		void	RequestDiscardItemRandomOption(int64 ItemId);
		void	RequestGetItemRandomQuality(int64 ItemId);
		void	RequestGenerateItemRandomQuality(int64 ItemId, int64 MaterialItemId);
		void	RequestApplyItemRandomQuality(int64 ItemId);
		void	RequestDiscardItemRandomQuality(int64 ItemId);
		void	RequestCheckRedDot();
		void	RequestCheckRedDot(const TArray<int32>&);
		void	RequestCheckChargePoints(bool CheckBladePoint, bool CheckPvPMatchPoint, bool CheckTeamMatchPoint);
		void	RequestIncreaseDarkCrystalPaid(int32 DarkCrystalPaid);
		void	RequestCheckPvPMatchPointCharge();
		void	RequestCheckTeamMatchPointCharge();
		void	RequestFindAccount(int32 AccountID, EPCClass PCClass1 = EPCClass::EPC_End, EPCClass PCClass2 = EPCClass::EPC_End, EPCClass PCClass3= EPCClass::EPC_End, int32 _item_preset_id = 0, int32 _skill_slot = 0);//const TArray<b2network::B2FindAccountQueryPtr>& FindAccountInfo);
		void	RequestFindAccount(int32 AccountID, EPCClass PCClass1, EPCClass PCClass2, EPCClass PCClass3, EPCClass PCClass4, int32 _item_preset_id, int32 _skill_slot);//const TArray<b2network::B2FindAccountQueryPtr>& FindAccountInfo);
		void	RequestModifyItemRandomOption(int64 ItemId, int32 SlotNum, int32 RandomOptionId, float RandomOptionValue);
		void	RequestModifyItemNewFlag(int32 characterType, int32 inventoryType);
		void	RequestModifyCostumeNewFlag(int32 characterType, int32 inventoryType);
		void	RequestStartRaid(int32 characterType);
		void	RequestGetContentsOpenStatus(int32 linkKey);
		void	RequestGetDuelRewardBoxInfo(int32 duelMode);
		void	RequestGetDuelRewardBoxGainableList(int32 duelMode, int32 boxGrade);
		void	RequestOpenDuelRewardBox(int32 duelMode);
		void	RequestReceiveAssaultMvpReward();
		void	RequestEventAttendance(int32 eventId);
		void	RequestBeginnerAttendance();
		void	RequestUpdateDailyPlayTime(int32 playTimeInSec);
		void	RequestReceiveDailyPlayTimeReward();
		
		void	RequestAskGuildSupport(int32 ItemTempleteID);
		void	RequestGuildSupport(int64 TargetAccountId, int64 ChatId, int32 Count);
		void	RequestGetGuildChat(int32 ChatId);

		void	RequestGetAccountAether(int32 aetherType);
		void	RequestDismantleAethers(const TArray<int64>& aether_ids);
		void	RequestMountAethers(const TArray<int64>& aether_ids, int32 characterType);
		void	RequestUnmountAethers(const TArray<int64>& aether_ids, int32 characterType);
		void	RequestLockAethers(const TArray<int64>& aether_ids);
		void	RequestUnlockAethers(const TArray<int64>& aether_ids);
		void	RequestExtendAetherInventory(int32 buyCount);
		void	RequestEnhanceAether(int64 aetherId);
		void	RequestModifyAetherNewFlag(int32 aetherType);
		void	RequestSelectAetherDefenseOption(int32 setOptionId, int32 characterType);
		void	RequestSelectAetherUnitySkillOption(int32 setOptionId, int32 characterType);
		void	RequestSellAethers(const TArray<int64>& aether_ids);
		void	RequestUnitySkillMissions(int32 characterType);
		void	RequestReceiveUnitySkillReward(int32 mainCharacterType, int32 unityCharacterType);
		void	RequestAwakenUnitySkillMission(int32 characterType, int32 missionId);
		void	RequestAwakenUnitySkill(int32 characterType);
		void	RequestGetDuelModeInfo();
		void	RequestGetChallengeModeInfo();
		void	RequestReceiveRestReward(int32 rewardId, bool isDoubleReward);
		void	RequestCheckModeOpen();
		void	RequestGetGuildMember();
		void	RequestGetHallOfFame(int32 mode, bool needUserInfo);
		void	RequestPraiseTargetRanker(int32 mode, int64 accountId, int32 ranking);

		void	RequestGetTutorialProgress();
		void	RequestSetTutorialProgress();
		void	RequestUpdateTutorialProgress();
		void	RequestSkipTutorialProgress();
		void	RequestDrawTutorialItem(int32 characterType);
		void	RequestPrepareTutorial(int32 characterType);
		void	RequestGiveTutorialReward();
		void	RequestCheckReachPower(const TArray<b2network::B2CharacterPowerPtr>& characterPower);
		void	RequestTrackingPowerDetail(int32 characterType, int32 itemPresetId, int32 skillSlot, const TArray<b2network::B2CharacterPowerSpecificPtr>& powerSpecifics);

		void	RequestKakaoMessageProcessing(const FString& message_box_id);
		void	RequestKakaoCouponUse(const FString& coupon_code);
		void	RequestKakaoAccountConversionPrepare(const FString& idp_code);
		void	RequestKakaoAccountConversionComplete(const FString& idp_code);
		void	RequestSkipMatchTutorial(int32 matchType);
		void	RequestGetEvent();
		void	RequestReceivePointEventReward(int32 eventId, int32 rewardIndex);
		void	RequestReceiveLevelUpEventReward(int32 eventId, int32 index); 
		void	RequestReceiveStageClearEventReward(int32 eventId, int32 index);
		void	RequestGetSeasonMission();
		void	RequestRewardSeasonMission(int32 eventId, int32 day, int32 index);
		void	RequestAttendSeasonMission();
		void	RequestBuyHotTimeBuff(int32 buffType);
		void	RequestRewardPointShopEvent(int32 eventId, int32 index, int32 buyCount = 1);
		void	RequestReportUser(int64 targetAccountId, int32 modeType, int32 reasonType, const FString& explain);

		void	RequestBlockChat(int32 accountId);
		void	RequestUnblockChat(int32 accountId);
		void	RequestGetBlockChatList();

		void	RequestClientStateReport(const FString& category, const FString& event_name, const FString& label, const FString& detail_info);

		void	RequestRewardFlatRatePackageBonus();
		void	RequestGetPackageShop();
		void	RequestGetPeriodPackageState();
		void	RequestRewardLevelUpPackage(const int packageId, const int32 characterType, const int level);
		void	RequestRewardReachLevelPackage(const int packageId);
		void	RequestGiveUpQuest(const int slot);
		void	RequestRewardTenLotteryEvent(const int eventId, const int index);
		void	RequestRewardSpendGemEvent(const int eventId, const int index);
		void	RequestReceiveGemPurchaseEventReward(const int eventId, const int index);
		void	RequestGuildDonation(int32 mercenaryid, int32 donationtype);
		void	RequestRewardTenLotterySavingsEvent(const int eventId);
		void	RequestRollDiceEvent(const int eventId, const bool isFreeDice);
		void	RequestBuyDiceEventPoint(const int eventId);
		void	RequestRewardDiceEventMileage(const int eventId, const int rewardIndex);

		void    RequestInvitation(const FString& invitee, const FString& room_id);
		void    RequestAcceptInvitation(const int invitation_type, const FString& inviter, const FString& invitation_ref, const FString& invitee, b2network::B2RoomLocationPtr room_location);
		void    RequestDeclineInvitation(const int invitation_type, const FString& inviter, const FString& invitation_ref, const FString& invitee);

		void	RequestGetAccountCostume(int32 characterType, int32 inventoryType);
		void	RequestEquipCostume(const TArray<int64>& costumeIds);
		void	RequestUnequipCostume(const TArray<int64>& costumeIds);
		void	RequestEnhanceCostume(int64 costumeId);
		void	RequestDismantleCostumes(const TArray<int64>& costumeIds);
		void	RequestLockCostumes(const TArray<int64>& costumeIds);
		void	RequestUnlockCostumes(const TArray<int64>& costumeIds);
		void	RequestGetCostumeShop();
		void	RequestPurchaseCostume(int32 productId);
		void	RequestSetCostumeVisible(int32 characterType, int32 costumeType, bool isVisible);

		// totem
		void	RequestGetAccountTotem();
		void	RequestEquipTotems(const TArray<int64>& totemIds, int32 characterType);
		void	RequestUnequipTotems(const TArray<int64>& totemIds);
		void	RequestLockTotems(const TArray<int64>& totemIds);
		void	RequestUnlockTotems(const TArray<int64>& totemIds);
		void	RequestSellTotems(const TArray<int64>& totemIds);
		void	RequestDismantleTotems(const TArray<int64>& totemIds);
		void	RequestRefineTotem(int64 totemId, int32 optionId, int32 costType);
		void	RequestModifyTotemNewFlag();
		void	RequestAddTotemToInventory(int32 templateId);

		//ItemForge
		void	RequestItemForgeList();
		void	RequestItemForge(int64 ForgeID, int64 Quantity, int64 MainItemUID, int64 MaterialItemUID1, int64 MaterialItemUID2, int64 MaterialItemUID3, int64 MaterialItemUID4, int64 MainItemRefID, int64 MaterialItemRefID1, int64 MaterialItemRefID2, int64 MaterialItemRefID3, int64 MaterialItemRefID4);

		//SealBox
		void	RequestUnsealBox(int64 sealboxUID, int32 PCClass, int64 openAmount);

		// REPLACEMENT POINT: RETAILER HEADER REQUEST
		// NEVER DELETE LINE ABOVE

	private:
		Retailer()
		{
		}

		~Retailer()
		{
		}

		Retailer(Retailer const&)				= delete;
		Retailer& operator = (Retailer const&)	= delete;
	};

	using RetailerPtr = std::shared_ptr<Retailer>;
}