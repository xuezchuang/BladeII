#pragma once
//#include "BladeII.h"
#include "EngineMinimal.h"
#include "CommonStruct.h"
#include "BladeIIPlayer.h"

enum RedDotHint
{
	RED_DOT_ALL = 0,
	RED_DOT_EQUIP = 1,
	RED_DOT_COLLECTION = 2,
	RED_DOT_MISSION = 3,
	RED_DOT_FRIEND = 4,
	RED_DOT_ADVENTURE = 5,
	RED_DOT_DUNGEON = 6,
	RED_DOT_PVP = 7,
	RED_DOT_GUILD = 8,
	RED_DOT_SHOP = 9,
	RED_DOT_MAIL = 10,
	RED_DOT_OPTION = 11,
	RED_DOT_EVENT = 12,
	RED_DOT_SHOP_PACKAGE = 13,
	RED_DOT_MAGIC_SHOP = 14,
	RED_DOT_HALL_OF_FAME = 15,
	RED_DOT_FAIRY = 16,
	RED_DOT_DONATION = 17,
	RED_DOT_ETC = 100
};

struct FLocalNewActRedDotData
{
public:
	const static TCHAR* SECTION_NAME;

	FLocalNewActRedDotData(EStageDifficulty InDifficulty, int32 InActID, bool bNew);
	FLocalNewActRedDotData(const TCHAR* Key, const TCHAR* Value);
		
	EStageDifficulty GetStringToDifficulty(FString& DifficultyStr);
	FString GetDifficultyToString(EStageDifficulty InDifficulty);

	void SetNewAct(bool bNew);
	int32 GetUniqueKey();
	bool IsNewAct();
public: 
	static int32 GetUniqueKey(EStageDifficulty InDifficulty, int32 InActID);

private:
	EStageDifficulty Difficulty;
	int32 ActID;
	bool bNewAct;

	int32 UniqueKey;
	FString SaveLoadKey;

	void CreateKey();
	void ParseKey(const TCHAR* Key);
	void ParseValue(const TCHAR* Value);
};



class FB2RedDotManager
{
public:
	FB2RedDotManager();
	
	void SubscribeEvents();
	void UnsubscribeEvents();

	void LoadLocalData();
	
	// Attendance
	bool HadLobbyRedDot_Attendance();
	bool HasLobbyRedDot_ConnectReward();
	bool HasLobbyRedDot_EventAttendance();
	bool HasLobbyRedDot_NewUser();
	// Lobby New Mark - HeroMgmt
	bool HasLobbyRedDot_HeroMgmt();

	bool HasLobbyRedDot_HeroMgmt_Inventory();
	bool HasLobbyRedDot_HeroMgmt_Inventory(EPCClass PCClass);
	bool HasLobbyRedDot_HeroMgmt_Inventory(EPCClass PCClass, EItemInvenType TapType);
	
	bool HasLobbyRedDot_HeroMgmt_Skill();
	bool HasLobbyRedDot_HeroMgmt_Skill(EPCClass PCClass);
	bool HasLobbyRedDot_HeroMgmt_PointSkill(EPCClass PCClass);
	bool HasLobbyRedDot_HeroMgmt_PointSkill(EPCClass PCClass, bool bActiveSkill);
	bool HasLobbyReddot_HeroMgmt_UnitySkill(EPCClass PCClass);
	bool HasLobbyReddot_HeroMgmt_UnitySkill(EPCClass MainPCClass, EPCClass SubPCClass);
	bool HasLobbyReddot_HeroMgmt_UnitySkillAwakenAble(EPCClass PCClass);
	bool HasLobbyReddot_HeroMgmt_UnitySkillAwakenMission(EPCClass PCClass);

	bool HasLobbyRedDot_HeroMgmt_Brevet();
	bool HasLobbyRedDot_HeroMgmt_Brevet(EPCClass PCClass);

	bool HasLobbyRedDot_HeroMgmt_Wing();
	bool HasLobbyRedDot_HeroMgmt_Wing(EPCClass PCClass);

	bool HasLobbyRedDot_HeroMgmt_Relic();
	bool HasLobbyRedDot_HeroMgmt_Relic(EPCClass PCClass);
	bool HasLobbyRedDot_HeroMgmt_Relic(EPCClass PCClass,int32 nRelicId);

	bool HasLobbyRedDot_HeroMgmt_Ether();

	bool HasLobbyRedDot_HeroMgmt_Totem();

	// Lobby New Mark - Dungeon
	void MarkAct(EStageDifficulty StageDifficulty, int32 ActNum);
	bool HasLobbyRedDot_NewAct(EStageDifficulty StageDifficulty, int32 ActNum);
	bool HasLobbyRedDot_AllNewAct();
	
	bool HasLobbyRedDot_Adventure();

	bool HasLobbyRedDot_Dungeon();
	bool HasLobbyRedDot_Dungeon_Counter();
	bool HasLobbyRedDot_Dungeon_Hero();
	bool HasLobbyRedDot_Dungeon_Raid();
	bool HasLobbyRedDot_Dungeon_Dimension();

	// Lobby New mark - Battle

	bool HasLobbyRedDot_Battle();
	bool HasLobbyRedDot_Battle_PvP();
	bool HasLobbyRedDot_Battle_Team();
	bool HasLobbyRedDot_Battle_Control();

	// CollectBook
	bool HasLobbyRedDot_CollectBook();

	// Mission
	bool HasLobbyRedDot_Mission();

	// Guild
	bool HasLobbyRedDot_Guild();

	// Shop
	bool HasLobbyRedDot_Shop();
	bool HasLobbyRedDot_Mileage();
	bool HasLobbyRedDot_Mileage_Cash();
	bool HasLobbyRedDot_Mileage_Item();
	bool HasLobbyRedDot_FreeItem();

	// Friend
	bool HasLobbyRedDot_Friend();
	bool HasLobbyRedDot_Friend_New();
	bool HasLobbyRedDot_Friend_Request();
	bool HasLobbyRedDot_Friend_FriendPoint();

	// Mail
	bool HasLobbyRedDot_Mail();
	bool HasLobbyRedDot_Mail(MailTabIndex MailType);

	// Chat
	bool HasLobbyRedDot_Chat();

	// Setting
	bool HasLobbyRedDot_Setting();

	bool HasUnappliedItemRandomOption();

	bool HasUnappliedItemRandomQuality();

	//event
	bool HasLobbyRedDot_Event();
	bool HasLobbyRedDot_Event_Levelup();
	bool HasLobbyRedDot_Event_Point();
	bool HasLobbyRedDot_SeasonEvent();
	bool HasLobbyRedDot_Event_StageClear();
	bool HasLobbyRedDot_Event_PointConsume();
	bool HasLobbyRedDot_Event_GemPurchase();
	bool HasLobbyRedDot_Event_TenLottery();
	bool HasLobbyRedDot_Event_GemSpend();
	bool HasLobbyRedDot_Event_TenLotterySaving();
	bool HasLobbyRedDot_Event_Dice();
	
	//rest
	bool HasLobbyRedDot_RestPoint();

	//Package
	bool HasLobbyRedDot_Package_FlatRate();
	bool HasLobbyRedDot_Package_Levelup();
	bool HasLobbyRedDot_Package_ReachLevel();

	// Magic Store
	bool HasLobbyRedDot_MagicStore();

	// HallOfFame
	bool HasLobbyRedDot_HallOfFame();

	bool HasNotApplyBlessFairy();

	bool HasLobbyRedDot_Donation();

	//Costume
	bool HasLobbyRedDot_Costume();
	bool HasLobbyRedDot_CostumeInventory(EPCClass PCClass);
	bool HasLobbyRedDot_CostumeInventory(EPCClass PCClass, EItemInvenType TapType);

	// Totem
	bool HasLobbyRedDot_Totem();

// explicit

	void OnOpenedAllMails(MailTabIndex OpenedMailIdx, int32 OpenedMailCount = 0);
	void OnOpenedSingleMail(MailTabIndex OpenedMailIdx);

	const int32 GetUsedHeroTowerTicket();

public:
	static bool IsAvailableCounterDungeonReward(int32 ClearCount, int32 RewardReceived);
	static void SetArrayRedDotHint(TArray<int32>& SetArray, std::initializer_list<RedDotHint> HintArguments) throw();
	static void RequestCheckRedDot(std::initializer_list<RedDotHint> HintArguments);
	
	//Tutorial
	static bool TutorialLockRedDotVisibleCheck(int32 iLockTutorialID);

private:


	/*
	void DoCheckMarkForNewUIs(EUICategoryMarkForNew Category);
	FString GetInvenTabNewItemToCheckIniItemName(EItemInvenType InvenTab, EPCClass InPCClass);
	*/

	void ResponseLobbyCheckRedDot(FB2MessageInfoResponseCheckRedDotPtr LobbyRedDotInfo);
	void UpdateMarkRedDot(const FName& UIName);
	bool IsLevelupableSkill(int32 SkillID);
	bool HasLevelUpSkillGold(int32 SkillID);
	bool HasLevelUpSkillPoint(int32 SkillID, int32 SkillLevel);

	void UpdateRedDotState(FB2MessageInfoResponseCheckRedDotPtr NewRedDotInfo);
	void UpdateCheckModeOpen();

	void SetChangeStageClear(EStageDifficulty Difficulty, int32 CurrentStageID, int32 NextStageID);

	void CreateNewActMap();

private:
	/*
	TMap<EUICategoryMarkForNew, TArray<TWeakObjectPtr<class UB2UnitedWidgetBase>>> UIListMap;

	struct FCharNewItemData
	{
		TMultiMap<EItemInvenType, int64> NewItemUIDs;
	};
	TMap<EPCClass, FCharNewItemData> CharNewItemData;
	*/

	TWeakObjectPtr<class UB2SkillInfo>	cacheB2SkillInfo;

	FB2MessageInfoResponseCheckRedDotPtr CurrentRedDotState;

	TMap<int32, FLocalNewActRedDotData> CheckNewActs;

	TArray<std::shared_ptr<class issue>> Issues;
};