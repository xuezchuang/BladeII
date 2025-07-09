
// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "B2UIEnum.generated.h"


/* OpenState of Each Widget */
UENUM(BlueprintType)
enum class EUIOpenState : uint8
{
	None			UMETA(DisplayName = "None"),
	OnOpening		UMETA(DisplayName = "OnOpening"),
	Opened			UMETA(DisplayName = "Opened"),
	OnClosing		UMETA(DisplayName = "OnClosing"),
	Closed			UMETA(DisplayName = "Closed"),
	Max,
};


/* Depth of UI's Layer. (Layer = Panel)
* The layer hierarchy representing the zorder.
* Higher number is higher hierarchy
*/
UENUM(BlueprintType)
enum class EUILayer : uint8
{
	None					UMETA(DisplayName = "None"),
	BackGroundWidgetScenes	UMETA(DisplayName = "BackGroundWidgetScenes"),
	WidgetScenes			UMETA(DisplayName = "WidgetScene"),
	MainMenu				UMETA(DisplayName = "MainMenu"),
	Header					UMETA(DisplayName = "Header"),
	Widgets					UMETA(DisplayName = "Widgets"),
	Popups					UMETA(DisplayName = "Popups"),
	Notices					UMETA(DisplayName = "Notices"),
	Movie					UMETA(DisplayName = "Movie"),
	Max,
};

/** Use this macro in UIFName namespace.
* It declares FName variable UIFName::NameID as "NameID" */
#define DECLARE_B2_UIFNAME(NameID) const FName NameID(TEXT( #NameID ));


/** A namespace meant to declare FNames to identify independent widget name entry.
* Each declared names here can be a widget page which can be registered to UIManager and be opened solely. */
namespace UIFName
{
	const FName None(NAME_None); // Specially declared for convenience.

	DECLARE_B2_UIFNAME(LogIn)
	DECLARE_B2_UIFNAME(Title)
	DECLARE_B2_UIFNAME(CharacterIntro)
	DECLARE_B2_UIFNAME(Header)
	DECLARE_B2_UIFNAME(LobbyMain)
	DECLARE_B2_UIFNAME(ScenarioDialog)
	DECLARE_B2_UIFNAME(Chapter)
	DECLARE_B2_UIFNAME(CharacterSelect)
	DECLARE_B2_UIFNAME(CharacterObserve)
	DECLARE_B2_UIFNAME(BattleStageInfo)
	DECLARE_B2_UIFNAME(BattleMain)
	DECLARE_B2_UIFNAME(TutorialBattleMain)
	DECLARE_B2_UIFNAME(BattlePause)
	DECLARE_B2_UIFNAME(BossStatus)
	DECLARE_B2_UIFNAME(CharacterSkillSet)
	DECLARE_B2_UIFNAME(MobAppearBossStage)
	DECLARE_B2_UIFNAME(MobAppearBossArea)
	DECLARE_B2_UIFNAME(Defeat)
	DECLARE_B2_UIFNAME(StageBegin)
	DECLARE_B2_UIFNAME(StageClear)
	DECLARE_B2_UIFNAME(StageResult)
	DECLARE_B2_UIFNAME(MsgPopupSimple)
	DECLARE_B2_UIFNAME(MsgPopupInput)
	DECLARE_B2_UIFNAME(MsgPopupItemReward)
	DECLARE_B2_UIFNAME(MsgPopupCenter)
	DECLARE_B2_UIFNAME(MsgPopupSelectPCClass)
	DECLARE_B2_UIFNAME(PVPOneOnOne)
	DECLARE_B2_UIFNAME(RegistSkill)
	DECLARE_B2_UIFNAME(MsgPopupItemCost)
	DECLARE_B2_UIFNAME(MsgPopupBuySkillPoint)
	DECLARE_B2_UIFNAME(MsgPopupTooltipSkillOption)
	DECLARE_B2_UIFNAME(PVPOneOnOneBattleMain)
	DECLARE_B2_UIFNAME(PVPOneOnOneFindMatch)
	DECLARE_B2_UIFNAME(PVPBegin)
	DECLARE_B2_UIFNAME(PVPResult)
	DECLARE_B2_UIFNAME(TeamMatchUI)
	DECLARE_B2_UIFNAME(TeamHeroSelectUI)
	DECLARE_B2_UIFNAME(TeamMatchFindMatch)
	DECLARE_B2_UIFNAME(TeamMatchBattle)
	DECLARE_B2_UIFNAME(ModSelect)
	DECLARE_B2_UIFNAME(ModMatchReward)
	DECLARE_B2_UIFNAME(ModMatchRewardList_Day)
	DECLARE_B2_UIFNAME(ModMatchRewardList_Week)
	DECLARE_B2_UIFNAME(ArinConsult_PreCombat)
	DECLARE_B2_UIFNAME(ArinConsult_PostDefeat)
	DECLARE_B2_UIFNAME(Chatting)
	DECLARE_B2_UIFNAME(Raid)
	DECLARE_B2_UIFNAME(RaidJoin)
	DECLARE_B2_UIFNAME(RaidInviteList)
	DECLARE_B2_UIFNAME(RaidBattle)
	DECLARE_B2_UIFNAME(Store)
	DECLARE_B2_UIFNAME(Mail)
	DECLARE_B2_UIFNAME(SelectLogin)
	DECLARE_B2_UIFNAME(MsgPopupGoTeamMatch)
	DECLARE_B2_UIFNAME(MsgPopupNeedClearStage)
	DECLARE_B2_UIFNAME(MsgPopupUseGold)
	DECLARE_B2_UIFNAME(MsgPopupUseGem)
	DECLARE_B2_UIFNAME(BrevetAddStatus)
	DECLARE_B2_UIFNAME(BrevetRankUpSuccess)
	DECLARE_B2_UIFNAME(BlockScreen)
	DECLARE_B2_UIFNAME(CounterAttackDGMain)
	DECLARE_B2_UIFNAME(CounterAttackDGSelect)
	DECLARE_B2_UIFNAME(Friend)
	DECLARE_B2_UIFNAME(QuestNotice)
	DECLARE_B2_UIFNAME(QuestDialog)
	DECLARE_B2_UIFNAME(QuestCompleteNotify)
	DECLARE_B2_UIFNAME(CounterAttackDGResult)
	DECLARE_B2_UIFNAME(MsgPopupSimpleAddComment)
	DECLARE_B2_UIFNAME(MsgPopupUserInfo)
	DECLARE_B2_UIFNAME(MsgPopupGuildCreate)
	DECLARE_B2_UIFNAME(CounterDGBattleMain)
	DECLARE_B2_UIFNAME(Mission)
	DECLARE_B2_UIFNAME(ContentsSubMain)
	DECLARE_B2_UIFNAME(AdventureModSelect)
	DECLARE_B2_UIFNAME(SelectAdventureDifficulty)
	DECLARE_B2_UIFNAME(HeroTowerReady)
	DECLARE_B2_UIFNAME(HeroTowerBattleSub)
	DECLARE_B2_UIFNAME(GuildJoinAndCreate)
	DECLARE_B2_UIFNAME(GuildMain)
	DECLARE_B2_UIFNAME(HeroTowerResult)
	DECLARE_B2_UIFNAME(GuildCreate)
	DECLARE_B2_UIFNAME(Setting)
	DECLARE_B2_UIFNAME(MyGuildMemeberInfo)
	DECLARE_B2_UIFNAME(RecommendGuildInfo)
	DECLARE_B2_UIFNAME(UserGuildMsg)
	DECLARE_B2_UIFNAME(SummonItemStore)
	DECLARE_B2_UIFNAME(CollectBookSelectClass)
	DECLARE_B2_UIFNAME(CollectBookMain)
	DECLARE_B2_UIFNAME(CollectBookWingOption)
	DECLARE_B2_UIFNAME(ToolTipTextBox)
	DECLARE_B2_UIFNAME(GuildMatchBattle)
	DECLARE_B2_UIFNAME(SummonItemResult)
	DECLARE_B2_UIFNAME(SemiSetting)
	DECLARE_B2_UIFNAME(ControlMain)
	DECLARE_B2_UIFNAME(GuildDefFormation)
	DECLARE_B2_UIFNAME(AttendanceMain)
	DECLARE_B2_UIFNAME(RaidResult)
	DECLARE_B2_UIFNAME(ControlBattleSub)
	DECLARE_B2_UIFNAME(WaitRevive)
	DECLARE_B2_UIFNAME(GuildMapUI)
	DECLARE_B2_UIFNAME(StoreHeader)
	DECLARE_B2_UIFNAME(AttendanceMsgPopup)
	DECLARE_B2_UIFNAME(ControlFindMatch)
	DECLARE_B2_UIFNAME(ControlIntroTeamInfo)
	DECLARE_B2_UIFNAME(ControlMvpView)
	DECLARE_B2_UIFNAME(ControlResult)
	DECLARE_B2_UIFNAME(GuildResult)
	DECLARE_B2_UIFNAME(GuildBattleReady)
	DECLARE_B2_UIFNAME(GuildBattleStatistics)
	DECLARE_B2_UIFNAME(CreateNickName)
	DECLARE_B2_UIFNAME(GuildInvite)
	DECLARE_B2_UIFNAME(GuildRanking)
	DECLARE_B2_UIFNAME(GuildBattleHistory)
	DECLARE_B2_UIFNAME(ChallengePopUp)
	DECLARE_B2_UIFNAME(GuildEdit)
	DECLARE_B2_UIFNAME(GuildSeasonReward)
	DECLARE_B2_UIFNAME(BattleRepeatDetail)
	DECLARE_B2_UIFNAME(HeroTowerCharacterSelect)
	DECLARE_B2_UIFNAME(HeroTowerSweepPopup)
	DECLARE_B2_UIFNAME(HeroTowerRanking)
	DECLARE_B2_UIFNAME(ItemOptionChange)
	DECLARE_B2_UIFNAME(TalkDialog)
	DECLARE_B2_UIFNAME(FindMatchPopup)
	DECLARE_B2_UIFNAME(BattleScoreNotify)
	DECLARE_B2_UIFNAME(ControlReadyToBattle)
	DECLARE_B2_UIFNAME(GoodsShortcutTooltipPopup)
	DECLARE_B2_UIFNAME(CharacterSelectPage)
	DECLARE_B2_UIFNAME(ControlAddTime)
	DECLARE_B2_UIFNAME(ControlRanking)
	DECLARE_B2_UIFNAME(FindAccountInfo)
	DECLARE_B2_UIFNAME(ControlCharacterSelectPage)
	DECLARE_B2_UIFNAME(EnterDungeonMain)
	DECLARE_B2_UIFNAME(LevelupAutoSelect)
	DECLARE_B2_UIFNAME(EnterShopMain)
	DECLARE_B2_UIFNAME(UpgradeStatNotify)
	DECLARE_B2_UIFNAME(FormationPopup)
	DECLARE_B2_UIFNAME(HeroTowerCharacterSelectPage)
	DECLARE_B2_UIFNAME(EnterBattlePage)
	DECLARE_B2_UIFNAME(RaidDefeat)
	DECLARE_B2_UIFNAME(Tutorial)
	DECLARE_B2_UIFNAME(PVPCharacterSelectPage)
	DECLARE_B2_UIFNAME(SetInfoNotify)
	DECLARE_B2_UIFNAME(RaidMvpView)
	DECLARE_B2_UIFNAME(ModRewardBoxList)
	DECLARE_B2_UIFNAME(ModRewardBoxDetail)
	DECLARE_B2_UIFNAME(MsgPopupBoxRewardConfirm)
	DECLARE_B2_UIFNAME(ItemRaffle)
	DECLARE_B2_UIFNAME(EtherMain)
	DECLARE_B2_UIFNAME(RewardMailPopup)
	DECLARE_B2_UIFNAME(EtherAutoDecomposition)
	DECLARE_B2_UIFNAME(EtherAutoSell)
	DECLARE_B2_UIFNAME(RepeatBattleCountDown)
	DECLARE_B2_UIFNAME(EtherHelp)
	DECLARE_B2_UIFNAME(GuildSupportItemPopup)
	DECLARE_B2_UIFNAME(EnhanceEther)
	DECLARE_B2_UIFNAME(GuildSkill)
	DECLARE_B2_UIFNAME(DecompositionObtainPopup)
	DECLARE_B2_UIFNAME(LobbySelectCharacter)
	DECLARE_B2_UIFNAME(GuildBattleRewardInfoList)
	DECLARE_B2_UIFNAME(MsgPopupInputNoneFocus)
	DECLARE_B2_UIFNAME(EtherInfoNotify)
	DECLARE_B2_UIFNAME(UnitySkillRewardPopup)
	DECLARE_B2_UIFNAME(EtherExtendInventory)
	DECLARE_B2_UIFNAME(RestPointMain)
	DECLARE_B2_UIFNAME(BroadcastSystemNotice)
	DECLARE_B2_UIFNAME(DecompositionPopup)
	DECLARE_B2_UIFNAME(ClientIndicator)
	DECLARE_B2_UIFNAME(MsgPopupRaidInviteNotice)
	DECLARE_B2_UIFNAME(MsgPopupSimpleInvite)
	DECLARE_B2_UIFNAME(MsgPopupRaidPenalty)
	DECLARE_B2_UIFNAME(MoviePlayer)
	DECLARE_B2_UIFNAME(Credit)
	DECLARE_B2_UIFNAME(EventMain)
	DECLARE_B2_UIFNAME(CBTClear)
	DECLARE_B2_UIFNAME(MsgPopupInventoryFull)
	DECLARE_B2_UIFNAME(MsgPopupDetailMyInfo)
	DECLARE_B2_UIFNAME(SeasonEvent)
	DECLARE_B2_UIFNAME(PackageStore)
	DECLARE_B2_UIFNAME(HotTimePopup)
	DECLARE_B2_UIFNAME(MsgPopupBuyHotTime)
	DECLARE_B2_UIFNAME(SettingGlobalSound)
	DECLARE_B2_UIFNAME(TimerNotice)
	DECLARE_B2_UIFNAME(BattleMessageNotify)
	DECLARE_B2_UIFNAME(SettingGlobalSoundTheInitial)
	DECLARE_B2_UIFNAME(MiniStorePopup)
	DECLARE_B2_UIFNAME(EtherFullPopup)
	DECLARE_B2_UIFNAME(UserReportPopup)
	DECLARE_B2_UIFNAME(ControlUserReportPopup)
	DECLARE_B2_UIFNAME(BlockUserList)
	DECLARE_B2_UIFNAME(HallOfFame)
	DECLARE_B2_UIFNAME(MagicStore)
	DECLARE_B2_UIFNAME(GuildMercenary)
	DECLARE_B2_UIFNAME(EtherLottery)
	DECLARE_B2_UIFNAME(EtherLotteryResult)
	DECLARE_B2_UIFNAME(GuildRound)
	DECLARE_B2_UIFNAME(InviteGame)
	DECLARE_B2_UIFNAME(DailyNoticePopup)
	DECLARE_B2_UIFNAME(AutoSell)
	DECLARE_B2_UIFNAME(LotteryAllReceive)
	DECLARE_B2_UIFNAME(AdventureEnterBattleMain)
	DECLARE_B2_UIFNAME(MsgPopupBuyDicePoint)
	DECLARE_B2_UIFNAME(LotteryAllReceiveResult)
	DECLARE_B2_UIFNAME(DiceEventMileageRewardPopup)
	DECLARE_B2_UIFNAME(FairyMain)
	DECLARE_B2_UIFNAME(PointShopLimitedEventPopup)
	DECLARE_B2_UIFNAME(CounterDungeonSweepPopup)
	DECLARE_B2_UIFNAME(FairyLevelUpPopup)
	DECLARE_B2_UIFNAME(FairyBlessPopup)
	DECLARE_B2_UIFNAME(FairyBlessConfirmPopup)
	DECLARE_B2_UIFNAME(FairyLevelUpResultPopup)
	DECLARE_B2_UIFNAME(AutoClearResultPopup)
	DECLARE_B2_UIFNAME(CostumeItemInfoPopup)
	DECLARE_B2_UIFNAME(CostumeItemInfoPopupLeft)
	DECLARE_B2_UIFNAME(CostumePackageDetailPopup)
	DECLARE_B2_UIFNAME(DonationEvent)
	DECLARE_B2_UIFNAME(DonationResultPopup)
	DECLARE_B2_UIFNAME(DonationRewardInfoPopup)
	DECLARE_B2_UIFNAME(CostumeRaffle)
	DECLARE_B2_UIFNAME(MsgEnhancementChangeMode)
	DECLARE_B2_UIFNAME(DimensionBattle)
	DECLARE_B2_UIFNAME(TotemInfoPopup)
	DECLARE_B2_UIFNAME(TotemInfoPopupLeft)
	DECLARE_B2_UIFNAME(TotemSmeltingPopup)
	DECLARE_B2_UIFNAME(TotemSellAllPopup)
	DECLARE_B2_UIFNAME(TotemDecompositionAllPopup)
	DECLARE_B2_UIFNAME(UpgradeStoneAllReceive)
	DECLARE_B2_UIFNAME(ItemHeroChangeMain)
	DECLARE_B2_UIFNAME(DimensionMain)
	DECLARE_B2_UIFNAME(DimensionSelectPage)
	DECLARE_B2_UIFNAME(DimensionWeeklyRewardInfoPopup)
	DECLARE_B2_UIFNAME(DimensionWeeklyRewardPopup)
	DECLARE_B2_UIFNAME(DimensionResult)
	DECLARE_B2_UIFNAME(TotemRaffle)
	DECLARE_B2_UIFNAME(SealMain)
	DECLARE_B2_UIFNAME(CostumeDecompositonPopup)
	DECLARE_B2_UIFNAME(CostumeEnhancePopup)
	DECLARE_B2_UIFNAME(MsgPopupSelectFairyGift)
	DECLARE_B2_UIFNAME(ConvertItemPopup)
	DECLARE_B2_UIFNAME(ItemQualityChange)
	DECLARE_B2_UIFNAME(HammerChangePopup)
	DECLARE_B2_UIFNAME(ItemForge)
	DECLARE_B2_UIFNAME(SealBoxResult)
	DECLARE_B2_UIFNAME(SealBoxResultSimple)
	DECLARE_B2_UIFNAME(SealBoxRewardRate)
}


/** A pop-up message showing. */
UENUM(BlueprintType)
enum class EUIMsgPopup : uint8
{
	None = 0,
	Simple						UMETA(DisplayName = "Simple"),
	InputNoneFocus				UMETA(DisplayName = "InputNoneFocus"),
	Input						UMETA(DisplayName = "Input"),
	ItemReward					UMETA(DisplayName = "ItemReward"),
	Center						UMETA(DisplayName = "Center"),
	ItemCostConfirm				UMETA(DisplayName = "ItemCostConfirm"),
	BuySkillPoint				UMETA(DisplayName = "BuySkillPoin"),
	TooltipSkillOption			UMETA(DisplayName = "TooltipSkillOption"),
	GoTeamMatch					UMETA(DisplayName = "GoTeamMatch"),
	NeedClearStage				UMETA(DisplayName = "NeedClearStage"),
	UseGold						UMETA(DisplayName = "UseGold"),
	UseGem						UMETA(DisplayName = "UseGem"),
	SimpleAddComment			UMETA(DisplayName = "SimpleAddComment"),
	UserInfo					UMETA(DisplayName = "UserInfo"),
	SelectPCClass				UMETA(DisplayName = "SelectPCClass"),
	RaidInviteNotice			UMETA(DisplayName = "RaidInviteNotice"),
	SimpleInvite				UMETA(DisplayName = "SimpleInvite"),
	RaidPenalty					UMETA(DisplayName = "RaidPenalty"),
	InventoryFull				UMETA(DisplayName = "InventoryFull"),
	DetailMyInfo				UMETA(DisplayName = "DetailMyInfo"),
	BuyHotTime					UMETA(DisplayName = "BuyHotTime"),
	BattleMessageNotify			UMETA(DisplayName = "BattleMessageNotify"),
	MsgPopupBuyDicePoint		UMETA(DisplayName = "MsgPopupBuyDicePoint")
};

/** Buttons type of simple pop-up */
UENUM(BlueprintType)
enum class EUIMsgPopupButtonGroup : uint8
{
	None					UMETA(DisplayName = "None"),
	Yes						UMETA(DisplayName = "Yes"),
	No						UMETA(DisplayName = "No"),
	YesOrNo					UMETA(DisplayName = "YesOrNo"),
	Confirm					UMETA(DisplayName = "Confirm"),
	Cancel					UMETA(DisplayName = "Cancel"),
	ConfirmOrCancel			UMETA(DisplayName = "ConfirmOrCancel"),
	AcceptOrReject			UMETA(DisplayName = "AcceptOrReject"),
	WatchOrExit				UMETA(DisplayName = "WatchOrExit"),
	MatchingContinue		UMETA(DisplayName = "MatchingContinue"),
	Reconnect				UMETA(DisplayName = "Reconnect"),
	ReconnectOrGoToTitle	UMETA(DisplayName = "Reconnect Or GOTOTitle"),
	GotoLastChapter			UMETA(DisplayName = "GotoLastChapter"),
	GotoLobby				UMETA(DisplayName = "GotoLobby")
};

UENUM()
enum class EPopUpPriority : uint8
{
	Base = 0,

	// server message
	Server_Message_Default,
	Server_Message_GoToLobby,
	Server_Message_GoToTitle,
	Server_Message_DLCMap,
};


/** The name of the scene is made of multiple widgets combined.(For convenience) */
UENUM(BlueprintType)
enum class EUIScene : uint8
{
	None = 0,
	LogIn						UMETA(DisplayName = "LogIn"),
	Title						UMETA(DisplayName = "Title"),
	CharacterIntro				UMETA(DisplayName = "CharacterIntro"),
	LobbyMain					UMETA(DisplayName = "LobbyMain"),
	LobbyMainSelectCharacter	UMETA(DisplayName = "LobbyMainSelectCharacter"),
	CharacterObserve			UMETA(DisplayName = "CharacterObserve"),
	//>> Those below are dummy UIScene entry provided for connection between DJLegacy UI (extending UB2LobbyUserWidget) and UIManager system
	// See utils DJLegacyHeroMgmtPageToUIScene, UISceneToDJLegacyHeroMgmt, OpenDJLegacyHeroMgmtPageForUIScene and IsDJLegacyUIScene
	Inventory					UMETA(DisplayName = "Inventory"),
	Skill						UMETA(DisplayName = "Skill"),
	Brevet						UMETA(DisplayName = "Brevet"),
	Wing						UMETA(DisplayName = "Wing"),
	Relic						UMETA(DisplayName = "Relic"),
	Costume						UMETA(DisplayName = "Costume"),
	Totem						UMETA(DisplayName = "Totem"),
	RelicManagement				UMETA(DisplayName = "RelicManagement"),
	//<<
	Chapter						UMETA(DisplayName = "Chapter"),
	BattleStageInfo				UMETA(DisplayName = "BattleStageInfo"),
	BattleMain					UMETA(DisplayName = "BattleMain"),
	TutorialBattleMain			UMETA(DisplayName = "TutorialBattleMain"),
	StageClear					UMETA(DisplayName = "StageClear"),
	StageReward					UMETA(DisplayName = "StageReward"),
	StageResult					UMETA(DisplayName = "StageResult"),
	PVPOneOnOne					UMETA(DisplayName = "PVP_1on1"),
	PVPOneOnOneBattleMain		UMETA(DisplayName = "PVP_1on1BattleMain"),
	PVPOneOnOneFindMatch		UMETA(DisplayName = "PVP_1on1FindMatch"),
	PVPResult					UMETA(DisplayName = "PVP_Result"),
	TeamMatchUI					UMETA(DisplayName = "TeamMatch"),
	TeamHeroChangeUI			UMETA(DisplayName = "TeamHeroChangeUI"),
	TeamMatchFindMatch			UMETA(DisplayName = "TeamMatchFindMatch"),
	TeamMatchBattle				UMETA(DisplayName = "TeamMatchBattle"),
	ModSelect					UMETA(DisplayName = "ModSelect"),
	RaidMain					UMETA(DisplayName = "RaidMain"),
	RaidBattle					UMETA(DisplayName = "RaidBattle"),
	RaidResult					UMETA(DisplayName = "RaidResult"),
	Store						UMETA(DisplayName = "Store"),
	Mail						UMETA(DisplayName = "Mail"),
	SelectLogin					UMETA(DisplayName = "SelectLogin"),
	CounterAttackDGMain			UMETA(DisplayName = "CounterAttackDGMain"),
	CounterAttackDGSelect		UMETA(DisplayName = "CounterAttackDGSelect"),
	Friend						UMETA(DisplayName = "Friend"),
	CounterAttackDGResult		UMETA(DisplayName = "CounterAttackDGResult"),
	CounterDGBattleMain			UMETA(DisplayName = "CounterDGBattleMain"),
	Mission						UMETA(DisplayName = "Mission"),
	ContentsSubMain				UMETA(DisplayName = "ContentsSubMain"),
	AdventureModSelect			UMETA(DisplayName = "AdventureModSelect"),
	SelectAdventureDifficulty	UMETA(DisplayName = "SelectAdventureDifficulty"),
	HeroTowerReady				UMETA(DisplayName = "HeroTowerReady"),
	HeroTowerBattle				UMETA(DisplayName = "HeroTowerBattle"),
	GuildJoinAndCreate			UMETA(DisplayName = "GuildJoinAndCreate"),
	GuildMain					UMETA(DisplayName = "GuildMain"),
	HeroTowerResult				UMETA(DisplayName = "HeroTowerResult"),
	SummonItemStore				UMETA(DisplayName = "SummonItemStore"),
	CollectBookSelectClass		UMETA(DisplayName = "CollectBookSelectClass"),
	CollectBookMain				UMETA(DisplayName = "CollectBookMain"),
	GuildMatchBattle			UMETA(DisplayName = "GuildMatchBattle"),
	SummonItemResult			UMETA(DisplayName = "SummonItemResult"),
	ControlMain					UMETA(DisplayName = "ControlMain"),
	ControlBattle				UMETA(DisplayName = "ControlBattle"),
	ControlMvpView				UMETA(DisplayName = "ControlMvpView"),
	ControlResult				UMETA(DisplayName = "ControlResult"),
	GuildMapUI					UMETA(DisplayName = "GuildMapUI"),
	ControlFindMatch			UMETA(DisplayName = "ControlFindMatch"),
	GuildResult					UMETA(DisplayName = "GuildResult"),
	GuildBattleReady			UMETA(DisplayName = "GuildBattleReady"),
	GeneralLotteryResult		UMETA(DisplayName = "GeneralLotteryResult"),
	CharacterSelectPage			UMETA(DisplayName = "CharacterSelectPage"),
	FindAccountInfo				UMETA(DisplayName = "FindAccountInfo"),
	ControlCharacterSelectPage	UMETA(DisplayName = "ControlCharacterSelectPage"),
	EnterDungeonMain			UMETA(DisplayName = "EnterDungeonMain"),
	EnterShopMain				UMETA(DisplayName = "EnterShopMain"),
	LotteryStore				UMETA(DisplayName = "LotteryStore"),
	HeroTowerCharacterSelectPage UMETA(DisplayName = "HeroTowerCharacterSelectPage"),
	EnterBattleMain				UMETA(DisplayName = "EnterBattleMain"),
	PVPCharacterSelectPage		UMETA(DisplayName = "PVPCharacterSelectPage"),
	EtherMain					UMETA(DisplayName = "EtherMain"),
	RestPointMain				UMETA(DisplayName = "RestPointMain"),
	EventMain					UMETA(DisplayName = "EventMain"),
	SeasonEvent					UMETA(DisplayName = "SeasonEvent"),
	PackageStore				UMETA(DisplayName = "PackageStore"),
	HallOfFame			    	UMETA(DisplayName = "HallOfFame"),
	MagicStore					UMETA(DisplayName = "MagicStore"),
	GuildMercenary				UMETA(DisplayName = "GuildMercenary"),
	AdventureEnterBattleMain	UMETA(DisplayName = "AdventureEnterBattleMain"),
	FairyMain					UMETA(DisplayName = "FairyMain"),
	DonationEvent				UMETA(DisplayName = "DonationEvent"),
	DimensionBattle				UMETA(DisplayName = "DimensionBattle"),
	DimensionMain				UMETA(DisplayName = "DimensionMain"),
	DimensionSelectPage			UMETA(DisplayName = "DimensionSelectPage"),
	DimensionResult				UMETA(DisplayName = "DimensionResult"),
	ItemForge					UMETA(DisplayName = "ItemForge"),
	SealBoxResult				UMETA(DisplayName = "SealBoxResult"),
	SealBoxResultSimple			UMETA(DisplayName = "SealBoxResultSimple"),
	SealBoxRewardRate			UMETA(DisplayName = "SealBoxRewardRate"),
	Max,
};

/** See if InUIScene is one of dummy UIScene provide for DJLegacy compatibility
* Also check for DJLegacyHeroMgmtPageToUIScene, UISceneToDJLegacyHeroMgmt, and OpenDJLegacyH	eroMgmtPageForUIScene */
FORCEINLINE bool IsDJLegacyUIScene(EUIScene InUIScene)
{
	// Don't forget for new entry
	checkSlow(static_cast<int32>(EUIScene::RelicManagement) + 1 == static_cast<int32>(EUIScene::Chapter));
	return (InUIScene == EUIScene::Inventory) || (InUIScene == EUIScene::Skill) || (InUIScene == EUIScene::Brevet) || (InUIScene == EUIScene::Wing) || (InUIScene == EUIScene::Relic) || (InUIScene == EUIScene::Costume) || (InUIScene == EUIScene::Totem) || (InUIScene == EUIScene::RelicManagement);
}

/* When the switch UIScene represents the type of transition. */
UENUM(BlueprintType)
enum class EUIChangeUISceneTransitionType : uint8
{
	NO_CANCEL		UMETA(DisplayName = "NoCancel"),	//체인지시 모든 연출을 정상적으로 해준다.
	CANCEL_CLOSE	UMETA(DisplayName = "CancelClose"),	//체인지시 클로즈 연출만 씹는다.
	CANCEL_OPEN		UMETA(DisplayName = "CancelOpen"),	//체인지시 오픈 연출만 씹는다.
	CANCEL_ALL		UMETA(DisplayName = "CancelAll"),	//체인지시 모든 연출을 씹는다.
	PREOPEN			UMETA(DisplayName = "Preopen"),		//체인지시 다음에 뜰 UI를 미리 띄워놓는다. 마치 팝업인거 마냥
};


/* Type of the document. */
UENUM(BlueprintType)
enum class EUIDocType : uint8
{
	None,
	Some					UMETA(DisplayName = "Some"),
	User					UMETA(DisplayName = "UserDoc"),
	UICondition				UMETA(DisplayName = "UIConditionDoc"), //This purpose is to hold miscellaneous UI-related data.
	Inventory				UMETA(DisplayName = "InventoryDoc"),
	Item					UMETA(DisplayName = "ItemDoc"),
	Hero					UMETA(DisplayName = "HeroDoc"),
	Equip					UMETA(DisplayName = "EquipDoc"),
	Skill					UMETA(DisplayName = "SkillDoc"),
	Chapter					UMETA(DisplayName = "ChapterDoc"),
	Battle					UMETA(DisplayName = "BattleDoc"),
	BattleStage				UMETA(DisplayName = "BattleStageDoc"),
	ScenarioDialog			UMETA(DisplayName = "ScenarioDialog"),
	Mode					UMETA(DisplayName = "ModeDoc"),
	PVP1on1Battle			UMETA(DisplayName = "PVP1on1BattleDoc"),
	TeamMatch				UMETA(DisplayName = "TeamMatch"),
	CounterAttack			UMETA(DisplayName = "CounterAttack"),
	Raid					UMETA(DisplayName = "Raid"),
	HeroTower				UMETA(DisplayName = "HeroTower"),
	Guild					UMETA(DisplayName = "Guild"),
	Store					UMETA(DisplayName = "Store"),
	Control					UMETA(DisplayName = "Control"),
	LobbyMatch				UMETA(DisplayName = "LobbyMatch"),
	Friend					UMETA(DisplayName = "Friend"),
	Donation				UMETA(DisplayName = "Donation"),
	Dimension				UMETA(DisplayName = "Dimension"),
	Max
};

/* Type of search. */
UENUM(BlueprintType)
enum class EUISearchType : uint8
{
	First			UMETA(DisplayName = "First"),		//첨으로(First)
	Forward			UMETA(DisplayName = "Forward"),		//앞으로(Next)
	Reverse			UMETA(DisplayName = "Reverse"),		//뒤로(Prev)
	End				UMETA(DisplayName = "End"),			//끝으로(End)
};

/* Type of StageBonus. */
UENUM(BlueprintType)
enum class EUIChapterStageBonusType : uint8
{
	None			UMETA(DisplayName = "None"),
	Gold			UMETA(DisplayName = "Gold"),
	Exp				UMETA(DisplayName = "Exp"),
	Item			UMETA(DisplayName = "Item"),
};

/* Type of StageCleare state. */
UENUM(BlueprintType)
enum class EUIChapterStageState : uint8
{
	NonCleared = 0		UMETA(DisplayName = "NonCleared"),
	NextDest = 1		UMETA(DisplayName = "NextDest"), //NonCleared but next destination.
	Cleared = 2			UMETA(DisplayName = "Cleared"),
};

/* Type of ChapterStarReward state. */
UENUM(BlueprintType)
enum class EUIChapterStarRewardState : uint8
{
	NotReached		UMETA(DisplayName = "NotReached"),
	Reached			UMETA(DisplayName = "Reached"),
	Rewarded		UMETA(DisplayName = "Rewarded"),
};

/* Battle Diffidulty Type. */
UENUM(BlueprintType)
enum class EUIBattleDifficulty : uint8
{
	Easy		UMETA(DisplayName = "Easy"),
	Normal		UMETA(DisplayName = "Normal"),
	Hard		UMETA(DisplayName = "Hard"),
};

/* When the select, what kind of mode. */
UENUM(BlueprintType)
enum class EUIHeroSelectModeType : uint8
{
	None				UMETA(DisplayName = "None"),
	SelectOwnerMain		UMETA(DisplayName = "SelectOwnerMain"),
	SelectOwnerSub		UMETA(DisplayName = "SelectOwnerSub"),
	SelectTargetMain	UMETA(DisplayName = "SelectTargetMain"),
	SelectTargetSub		UMETA(DisplayName = "SelectTargetSub"),
};

/* Whether main and sub recognized and the Hero is selected.*/
UENUM(BlueprintType)
enum class EUIHeroSelectType : uint8
{
	None		UMETA(DisplayName = "None"),
	Main		UMETA(DisplayName = "Main"),
	Sub			UMETA(DisplayName = "Sub"),
	Mod			UMETA(DisplayName = "Mod"),
};

/* CounterAttack Dungeon Day Type*/

UENUM(BlueprintType)
enum class EUICounterDayType : uint8
{
	None		UMETA(DisplayName = "None"),
	Sunday = 1	UMETA(DisplayName = "Sunday"),
	Monday		UMETA(DisplayName = "Monday"),
	Tuesday		UMETA(DisplayName = "Tuesday"),
	Wednesday	UMETA(DisplayName = "Wednesday"),
	Thursday	UMETA(DisplayName = "Thursday"),
	Friday		UMETA(DisplayName = "Friday"),
	Saturday	UMETA(DisplayName = "Saturday"),
	Max
};

UENUM(BlueprintType)
enum class EUIHeaderResourceType : uint8
{
	None				UMETA(DisplayName = "None"),
	Gem					UMETA(DisplayName = "Gem"),
	Gold				UMETA(DisplayName = "Gold"),
	BladePoint			UMETA(DisplayName = "BladePoint"),
	SocialPoint			UMETA(DisplayName = "SocialPoint"),
	HeroTowerTicket		UMETA(DisplayName = "HeroTowerTicket"),
	HeroPiece			UMETA(DisplayName = "HeroPiece"),
	Brevet				UMETA(DisplayName = "Brevet"),
	AdvancedBrevet		UMETA(DisplayName = "AdvancedBrevet"),
	TagMatchTicket		UMETA(DisplayName = "TagMatchTicket"),
	TeamMatchTicket		UMETA(DisplayName = "TeamMatchTicket"),
	ControlMatchTicket	UMETA(DisplayName = "ControlMatchTicket"),
	DarkCrystal			UMETA(DisplayName = "DarkCrystal"),
	EtherPiece			UMETA(DisplayName = "EtherPiece"),
	FamePoint			UMETA(DisplayName = "FamePoint"),
	DimensionPiece			UMETA(DisplayName = "DimensionPiece"),
	Max
};

const int32 Accuracy_Perfect = 0;
const int32 Accuracy_Great = 1;
const int32 Accuracy_Good = 2;
const int32 Accuracy_Bad = 3;
const int32 Accuracy_Fail = 4;

UENUM(BlueprintType)
enum class ESummonItemTabMenu : uint8
{
	GemBox,
	FriendShipBox,
	Costume,
};



/** Use this macro in GoodsTypeFName namespace.
* It declares GoodsTypeFName variable GoodsTypeFName::NameID as "NameID" */
#define DECLARE_B2_GoodsTypeFName(NameID) const FName NameID(TEXT( #NameID ));

namespace GoodsTypeFName
{
	const FName None(NAME_None);

	DECLARE_B2_GoodsTypeFName(GOLD)
	DECLARE_B2_GoodsTypeFName(GEM)
	DECLARE_B2_GoodsTypeFName(ACHIEVEMENT_POINT)
	DECLARE_B2_GoodsTypeFName(SOUL_BLADE)
	DECLARE_B2_GoodsTypeFName(UPGRADE_STONE_WEAPON)
	DECLARE_B2_GoodsTypeFName(ADVANCED_UPGRADE_STONE_WEAPON)
	DECLARE_B2_GoodsTypeFName(UPGRADE_STONE_ARMOR)
	DECLARE_B2_GoodsTypeFName(ADVANCED_UPGRADE_STONE_ARMOR)
	DECLARE_B2_GoodsTypeFName(UPGRADE_STONE_ACCESSORY)
	DECLARE_B2_GoodsTypeFName(ADVANCED_UPGRADE_STONE_ACCESSORY)
	DECLARE_B2_GoodsTypeFName(STEEL_HAMMER)
	DECLARE_B2_GoodsTypeFName(ADVANCED_STEEL_HAMMER)
	DECLARE_B2_GoodsTypeFName(PLATINUM_HAMMER)
	DECLARE_B2_GoodsTypeFName(ADVANCED_PLATINUM_HAMMER)
	DECLARE_B2_GoodsTypeFName(UPGRADE_WING_ENHANCE_INGREDIENT)
	DECLARE_B2_GoodsTypeFName(UPGRADE_WING_EVOLVE_INGREDIENT)
	DECLARE_B2_GoodsTypeFName(UPGRADE_WING_PROP_OPTION_ENHANCE_INGREDIENT)
	DECLARE_B2_GoodsTypeFName(BREVET_STONE)
	DECLARE_B2_GoodsTypeFName(ADVANCED_BREVET_STONE)
	DECLARE_B2_GoodsTypeFName(PIECE_OF_HERO)
	DECLARE_B2_GoodsTypeFName(FRIENDSHIP_POINT)
	DECLARE_B2_GoodsTypeFName(SELECT_UPGRADE_STONE_TICKET)
	DECLARE_B2_GoodsTypeFName(EXP)
	DECLARE_B2_GoodsTypeFName(DARK_CRYSTAL)
	DECLARE_B2_GoodsTypeFName(WEAPON_INGREDIENT)
	DECLARE_B2_GoodsTypeFName(ARMOR_INGREDIENT)
	DECLARE_B2_GoodsTypeFName(ACCESSORY_INGREDIENT)
	DECLARE_B2_GoodsTypeFName(ALL_INGREDIENT)
	DECLARE_B2_GoodsTypeFName(MVP_KEY)
	DECLARE_B2_GoodsTypeFName(HOTTIME_BUFF_TICKET)

	DECLARE_B2_GoodsTypeFName(NORMAL_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_WIZARD_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_ASSASSIN_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_FIGHTER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_GLADIATOR_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_WIZARD_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_ASSASSIN_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_FIGHTER_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_GLADIATOR_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_WIZARD_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_ASSASSIN_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_FIGHTER_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(NORMAL_GLADIATOR_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(SPECIAL_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_WIZARD_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_ASSASSIN_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_FIGHTER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_GLADIATOR_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_WIZARD_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_ASSASSIN_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_FIGHTER_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_GLADIATOR_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_WIZARD_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_ASSASSIN_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_FIGHTER_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_GLADIATOR_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(PREMIUM_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_WIZARD_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_ASSASSIN_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_FIGHTER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_GLADIATOR_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_WIZARD_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_ASSASSIN_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_FIGHTER_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_GLADIATOR_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_WIZARD_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_ASSASSIN_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_FIGHTER_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_GLADIATOR_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(SIGNATURE_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_WIZARD_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_ASSASSIN_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_FIGHTER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_GLADIATOR_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_WIZARD_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_ASSASSIN_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_FIGHTER_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_GLADIATOR_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_WIZARD_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_ASSASSIN_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_FIGHTER_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_GLADIATOR_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(PLATINUM_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(PLATINUM_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(PLATINUM_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_GLADIATOR_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_GLADIATOR_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_GLADIATOR_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_ASSASSIN_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_ASSASSIN_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_ASSASSIN_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_WIZARD_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_WIZARD_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_WIZARD_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_FIGHTER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_FIGHTER_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_FIGHTER_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_GLADIATOR_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_GLADIATOR_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_GLADIATOR_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_ASSASSIN_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_ASSASSIN_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_ASSASSIN_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_WIZARD_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_WIZARD_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_WIZARD_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_FIGHTER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_FIGHTER_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_FIGHTER_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_GLADIATOR_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_GLADIATOR_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_GLADIATOR_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_ASSASSIN_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_ASSASSIN_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_ASSASSIN_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_WIZARD_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_WIZARD_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_WIZARD_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_FIGHTER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_FIGHTER_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_FIGHTER_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(GRADE_4_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(GRADE_5_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(GRADE_6_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(GRADE_7_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_7_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_7_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(GRADE_8_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_8_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_8_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(GRADE_9_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_9_ARMOR_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_9_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(ESSENCE_WEAPON)
	DECLARE_B2_GoodsTypeFName(ESSENCE_PROTECTION)
	DECLARE_B2_GoodsTypeFName(ESSENCE_ACCESSORY)

	//정수 뽑기권 타입 추가
	DECLARE_B2_GoodsTypeFName(SPECIAL_WEAPON_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_ARMOR_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(SPECIAL_ACCESSORY_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_WEAPON_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_ARMOR_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(PREMIUM_ACCESSORY_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_WEAPON_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_ARMOR_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(SIGNATURE_ACCESSORY_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_2_WEAPON_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_2_ARMOR_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_2_ACCESSORY_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_3_WEAPON_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_3_ARMOR_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_3_ACCESSORY_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_WEAPON_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_ARMOR_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_ACCESSORY_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_WEAPON_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_ARMOR_ESSENCE_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_ACCESSORY_ESSENCE_TICKET)

	//정수 추가
	DECLARE_B2_GoodsTypeFName(GRADE_2_GLADIATOR_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_GLADIATOR_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_GLADIATOR_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_GLADIATOR_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_2_ASSASSIN_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_ASSASSIN_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_ASSASSIN_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_ASSASSIN_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_2_WIZARD_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_WIZARD_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_WIZARD_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_WIZARD_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_2_FIGHTER_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_FIGHTER_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_FIGHTER_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_FIGHTER_WEAPON_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_2_GLADIATOR_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_GLADIATOR_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_GLADIATOR_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_GLADIATOR_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_2_ASSASSIN_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_ASSASSIN_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_ASSASSIN_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_ASSASSIN_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_2_WIZARD_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_WIZARD_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_WIZARD_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_WIZARD_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_2_FIGHTER_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_FIGHTER_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_FIGHTER_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_FIGHTER_ARMOR_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_2_GLADIATOR_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_GLADIATOR_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_GLADIATOR_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_GLADIATOR_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_2_ASSASSIN_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_ASSASSIN_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_ASSASSIN_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_ASSASSIN_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_2_WIZARD_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_WIZARD_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_WIZARD_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_WIZARD_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_2_FIGHTER_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_3_FIGHTER_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_4_FIGHTER_ACCESSORY_ESSECNE)
	DECLARE_B2_GoodsTypeFName(GRADE_5_FIGHTER_ACCESSORY_ESSECNE)

	DECLARE_B2_GoodsTypeFName(BOSSPIECE_ID_1)
	DECLARE_B2_GoodsTypeFName(BOSSPIECE_ID_2)
	DECLARE_B2_GoodsTypeFName(BOSSPIECE_ID_3)
	DECLARE_B2_GoodsTypeFName(BOSSPIECE_ID_4)
	DECLARE_B2_GoodsTypeFName(BOSSPIECE_ID_5)
	DECLARE_B2_GoodsTypeFName(BOSSPIECE_ID_6)

	DECLARE_B2_GoodsTypeFName(ETHER_PIECE)
	DECLARE_B2_GoodsTypeFName(STAGE_BOOST_TICKET)
	DECLARE_B2_GoodsTypeFName(EXP_BOOST_TICKET)
	DECLARE_B2_GoodsTypeFName(GOLD_BOOST_TICKET)
	DECLARE_B2_GoodsTypeFName(TENLOTTERY_TICKET)

	DECLARE_B2_GoodsTypeFName(RANDOM_ACCESSORY)
	DECLARE_B2_GoodsTypeFName(RANDOM_BONUS_ACCESSORY)
	DECLARE_B2_GoodsTypeFName(RANDOM_ESSECNE)

	DECLARE_B2_GoodsTypeFName(HONER_POINT)

	DECLARE_B2_GoodsTypeFName(GRADE_1_TIER_1_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_1_TIER_2_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_1_TIER_3_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_1_TIER_4_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_2_TIER_1_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_2_TIER_2_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_2_TIER_3_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_2_TIER_4_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_3_TIER_1_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_3_TIER_2_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_3_TIER_3_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_3_TIER_4_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_TIER_1_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_TIER_2_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_TIER_3_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_4_TIER_4_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_TIER_1_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_TIER_2_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_TIER_3_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_TIER_4_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_TIER_1_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_TIER_2_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_TIER_3_SELECT_ETHER_WEAPON_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_TIER_4_SELECT_ETHER_WEAPON_TICKET)

	DECLARE_B2_GoodsTypeFName(DETERMINATIONOFGUARD)

	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_GROWTH_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_GROWTH_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_GROWTH_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_7_SELECT_GROWTH_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_8_SELECT_GROWTH_ACCESSORY_TICKET)

	DECLARE_B2_GoodsTypeFName(GRADE_4_SELECT_DUEL_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_5_SELECT_DUEL_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_6_SELECT_DUEL_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_7_SELECT_DUEL_ACCESSORY_TICKET)
	DECLARE_B2_GoodsTypeFName(GRADE_8_SELECT_DUEL_ACCESSORY_TICKET)

	// 페어리 레벨업 재료
	DECLARE_B2_GoodsTypeFName(FAIRY_LEVELUP_MATERIAL)

	DECLARE_B2_GoodsTypeFName(STAGEAUTOCLEAR)
}

UENUM(BlueprintType)
enum class EHotTimeEventType :uint8
{
	Event,		//이벤트
	Product,	//구매
	All,		//전체
};

UENUM()
enum class ECostumePage : uint8
{
	Inven,
	Store,
	Max,
};

UENUM()
enum class EEventSlotPointConsumeBTNState : uint8
{
	None,
	Decrease,
	Increase,
};

UENUM()
enum class EItemForgeSlot : uint8
{
	MainMaterialItem,
	Sub1stMaterialItem,
	Sub2ndMaterialItem,
	Sub3rdMaterialItem,
	Sub4thMaterialItem
};

UENUM()
enum class EItemForgeResultType : uint8
{
	Fail,
	Success,
	HugeSuccess,
	HugeSuccessReward,
	FailRefund
};