#pragma once

#ifndef __B2_STAGE_DATA_STORE_H__
#define __B2_STAGE_DATA_STORE_H__
//#include "BladeII.h"
#include "EngineMinimal.h"
#include "CommonStruct.h"

//+[TASK][B2CLT-2098]<SameLine> BladeII.h Dependency Remove
#include "B2UIDocChapter.h"
#include "B2ClientDataStore.h"

enum class EAdventureActType: uint8
{
	Act1_5,
	Act6,
};
struct FB2ActData
{
	FB2ActData(FB2OriginGetActInfoPtr Info);
	FB2OriginGetActInfoPtr ServerData;
};

struct FB2StageData
{
	FB2StageData(FB2OriginGetStageInfoPtr Info);
	FB2OriginGetStageInfoPtr ServerData;
};

/** Per-stage sensitive info which can affect game-play in some way, sync from server */
struct FStageInfoGameplayData
{
	FStageInfoGameplayData(const b2network::B2mdStageInfoPtr& InServerinfoPtr)
	{
		FMemory::Memzero(this, sizeof(FStageInfoGameplayData));
		if (InServerinfoPtr)
		{
			Difficulty = InServerinfoPtr->difficulty;
			ClientStageId = InServerinfoPtr->map_id;	// 捞霸 努扼捞攫飘俊辑 荤侩窍绰 stageId捞促. (辑滚俊辑绰 map_id扼绊 逞绢咳)

			ClearTimeGrade_High = InServerinfoPtr->clear_time_grade_high;
			ClearTimeGrade_Medium = InServerinfoPtr->clear_time_grade_medium;
			SuggestedAttack = InServerinfoPtr->recommend_power;
			SuggestedDefense = InServerinfoPtr->recommend_power;		// jwyim power
			RecommentPower = InServerinfoPtr->recommend_power;
			RecommendAttack = InServerinfoPtr->recommend_attack;
			RecommendDefense = InServerinfoPtr->recommend_defense;
			ExpBuffCost = InServerinfoPtr->gold_cost_exp_buff;
			GoldBuffCost = InServerinfoPtr->gold_cost_gold_buff;
			AutoSkillBuffCost = InServerinfoPtr->gold_cost_auto_skill_buff;
			BoostBuffCost = InServerinfoPtr->gold_cost_boost_buff;

			//闺繁教夸家.
			DifficultyScale = SvrToCliBalancingScaleFactor(InServerinfoPtr->difficulty_scale);
		}
	}

	FStageInfoGameplayData(const b2network::B2mdCounterDungeonPtr& InCounterDGServerInfoPtr)
	{
		FMemory::Memzero(this, sizeof(FStageInfoGameplayData));
		if (InCounterDGServerInfoPtr)
		{
			Difficulty = InCounterDGServerInfoPtr->difficulty;
			ClientStageId = InCounterDGServerInfoPtr->dungeon_id;
			RecommentPower = InCounterDGServerInfoPtr->recommend_power;
			SuggestedAttack = InCounterDGServerInfoPtr->recommend_power;
			SuggestedDefense = InCounterDGServerInfoPtr->recommend_power;		// jwyim power
		}
	}

	int32 Difficulty;
	int32 ClientStageId;

	int32 ClearTimeGrade_High;
	int32 ClearTimeGrade_Medium;
	int32 SuggestedAttack;
	int32 SuggestedDefense;
	int32 RecommentPower;
	int32 RecommendAttack;
	int32 RecommendDefense;
	int32 ExpBuffCost;
	int32 GoldBuffCost;
	int32 AutoSkillBuffCost;
	int32 BoostBuffCost;

	float DifficultyScale;

	float GetStageDifficultyScale_ServerData()
	{
		return DifficultyScale;
	}
};


class FStageDataStore
{
public:
	FStageDataStore();

	// Use only reference variable.
	FStageDataStore(FStageDataStore const&) = delete;
	FStageDataStore& operator=(FStageDataStore const&) = delete;

	// Interface
public:
	void				SubscribeEvents();
	void				UnsubscribeEvents();
	void				OnShutdownModule();

	void				SetMasterData(const FB2MasterDatas& InMasterDatas);
	void				AddMDStageGamePlayData(FServerStageID, const FStageInfoGameplayData&);
	void				SetMDStageFixedDrops(const FB2MasterDatas& InMasterDatas);

	/** The master, main interface that you use when you enter lobby chapter view. */
	void				RequestGetActInfoAndChangeChapter(int32 ActNum, EStageDifficulty Difficulty, bool bShortCut = false, FServerStageID ShortCutServerStageID = FServerStageID());
	/** The subset of RequestGetActInfoAndChangeChapter. We might refine it to be called prior to RequestGetActInfoAndChangeChapter, as pre-fetching of the act info. */
	bool				ConditionalRequestGetActInfo(int32 ActNum, EStageDifficulty Difficulty, bool bShortCut = false, FServerStageID ShortCutServerStageID = FServerStageID());
	FORCEINLINE bool	IsActInfoRequestPending() const { return bHasRequestedActInfo; }
	void				ResponseGetActInfo(FB2OriginGetActInfoPtr);
	void				RequestGetStageInfo(FServerStageID ServerStageId, bool bInForceReqToServer = false);
	void				ResponseGetStageInfo(FB2OriginGetStageInfoPtr);

	void				StartStage(const FB2StartStageInfo&);
	void				ClearStage(const FB2ClearStageInfo&, FServerStageID);

	void				RequestCachedGetActInfoAndChangeChapter();
	bool				IsCachedStageData(FServerStageID ServerStageId);
	bool				IsCachedActData(int32, EStageDifficulty);

	FServerStageID		GetLastPlayedServerStageId();
	int32				GetLastPlayedActId();
	int32				GetLastPlayedClientStageId();
	EStageDifficulty	GetLastPlayedStageDifficulty();
	int32				GetLastSubChapterNum(int32 ActNum);

	FServerStageID		GetLastClearServerStageIdByLastPlayedStageDifficulty();	// 付瘤阜 努府绢茄 抄狼档狼 付瘤阜 胶抛捞瘤 酒捞叼甫 馆券
	FServerStageID		GetNextClearServerStageIdByLastPlayedStageDifficulty();	// 付瘤阜 努府绢茄 抄狼档狼 付瘤阜 胶抛捞瘤 酒捞叼+ 1阑 馆券 

	FServerStageID		GetNewLastClearServerStageIdByLastPlayedStageDifficulty(int32 InChapter);	//6茅磐捞饶 付瘤阜 努府绢茄 抄档狼 付瘤阜 胶抛捞瘤 +1, 歹 饼霸 绝栏搁 付瘤阜 努府绢茄 胶抛捞瘤
	
	EStageDifficulty	GetMaxOpenedStageDifficulty();							//扁粮 1~5抄捞档 林绰 窃荐
	EStageDifficulty	GetMaxOpenedNewStageDifficulty(int32 InChapter);		//6~捞饶 抄捞档 林绰 窃荐
	bool				GetNewChapterOpenState(int32 InChapter);				//6何磐 坷锹捞 等扒瘤 救等扒瘤
	FServerStageID		GetLastClearServerStageId(EStageDifficulty InDifficulty);
	FServerStageID		GetNextClearServerStageId(EStageDifficulty InDifficulty);
	
	int32				GetLastClearClientId(EStageDifficulty InDifficulty);
	int32				GetNextClearClientId(EStageDifficulty InDifficulty);

	int32				GetMaxReachedActNumber(EStageDifficulty InDifficulty);

	bool				IsClearedFinalClientStageId(EStageDifficulty CheckDifficulty = EStageDifficulty::ESD_Normal, bool IsNext = false);
	bool				IsClearedStartAdventureModeClientStageId(EStageDifficulty CheckDifficulty = EStageDifficulty::ESD_Normal, bool IsNext = false);
	
	bool				CanAdventureMode(EStageDifficulty CheckDifficulty = EStageDifficulty::ESD_Normal, bool IsNext = false);
	bool				IsStageClearedOnce(FServerStageID InServerStageId);

	FServerStageID		GetServerStageID(int32 ClientStageId, EStageDifficulty Difficulty);
	int32				GetClientStageId(FServerStageID ServerStageId);
	EStageDifficulty	GetStageDifficulty(FServerStageID ServerStageId);

	int32				GetStartAdventureModeClientStageId();
	int32				GetActIdByClientStageId(int32 InClientStageId);
	int32				GetActIdByServerStageId(FServerStageID InServerStageId);
	int32				GetClientStageIdOfChapterNums(int32, int32);
	int32				GetSubChapterNumByClientStageId(int32);
	int32				GetSubChapterNumByServerStageId(FServerStageID InServerStageId);
	void				GetActRewardData(int32, EStageDifficulty, TArray<FUIChapterStarReward>&);
	void				GetActRewardPreviewData(int32 InChapterID, EStageDifficulty InDifficulty, int32 InRewardIndex, TArray<b2network::B2RewardPtr>& OutRewards);
	bool				HasAnyRewardToReceive(int32 ActID, EStageDifficulty Difficulty);
	bool				HasAnyRewardToReceiveAllChapter(EStageDifficulty Difficulty);
	void				SetActRewardState(FB2OriginGetActInfoPtr ActInfo);
	void				RemoveActRewardState(int32 ActID, EStageDifficulty Difficulty);
	int32				GetStageClearGradeInfo(FServerStageID ServerStageId);
	EUIChapterStageBonusType GetStageBonusType(int32 ClientStageId);
	void				GetStageRewardPreviewItems(FServerStageID, TArray<FB2RewardItemPreviewInfo>&);
	void				GetStageRewardPreviewItemsOfSameType(TArray<FB2RewardItemPreviewInfo>&);
	void				GetStageAetherPreviewItems(FServerStageID, TArray<FB2AetherItemPreviewInfo>&);
	void				GetStageMaterialPreviewItems(FServerStageID, TArray<FB2MaterialItemPreviewInfo>&);
	void				GetStageCostData(FServerStageID ServerStageId, int32& OutRepeatBattleCostRefId, int32& OutRepeatBattleCostCount, int32& OutRequireStamina);
	void				GetStageSuggestedCombatStats(FServerStageID ServerStageId, float& OutSuggestedAttack, float& OutSuggestedDefense);
	void				GetStageSuggestedCombatStats(FServerStageID ServerStageId, int32& OutRecommentPower);
	void				GetStageRecommendAbility(FServerStageID ServerStageId, int32& OutRecommentAttack, int32& OutRecommentDefense);
	bool				IsFirstActClear(int32 ActNumber);
	bool				IsFirstActOpen(int32 ActNumber);
	void				SaveClearAct(int32 ActNumber);
	void				SaveOpenAct(int32 ActNumber);

	void				SaveOpenCahpter(int32 InChapter ,bool Instage);
	bool				LoadOpenCahpter(int32 InChapter);

	void				SaveRecentlyStageInfo();
	void				SaveCBTClear(bool InbClear);
	void				LoadRecentlyStageInfo(int32& ChapterNum, int32& StageNum, EStageDifficulty& Difficulty);
	int32				LoadClearAct();
	int32				LoadOpenAct();
	bool				LoadCBTClear();
	//[@AKI, 170908] B2BUG-704 包访 眠啊 窃. 704 荐沥阑 困秦辑绰 LoadClearAct客 LoadOpenAct甫 荐沥秦具 窃
	int32				LoadClearDifficult();
	int32				GetEventSceneChapter(const int32 RequestActNumber);

	float				GetStageDifficultyScale(int32 ClientStageId, EStageDifficulty Difficulty, EB2GameMode TemporaryGameModeType);
	EUIBattleDifficulty GetRelativeStageDifficulty(FServerStageID ServerStageId, EPCClass MainClass, EPCClass TagClass);
	int32				GetStageExpectedClearStarGrade(FServerStageID ServerStageId, int32 CurrentPlayTime);
	int32				GetTimeLimitForStageClearGrade(FServerStageID ServerStageId, int32 InStarGrade); // The other way of GetStageExpectedClearStarGrade

	bool				IsRepeatBattleLocked(FServerStageID ServerStageId);
	bool				IsPossibleToDoRepeatBattle(FServerStageID ServerStageId); // More condition than IsRepeatBattleLocked
	bool				IsEnoughGoldToRepeatBattle(FServerStageID ServerStageId);

	int32				GetStageBuffGoldCost(FServerStageID ServerStageId, const TArray<EStageBuffType>& InAllSelectedType, bool bApplyBuffTicket = false);
	int32				GetStageSingleBuffGoldCost(FServerStageID ServerStageId, EStageBuffType BuffType);

	FORCEINLINE FStageFixedDropsInfo* GetStageFixedDrops(int32 stageIdx) { return MD_StageFixedDrops.Find(stageIdx); };

	void				RequestGetChapterClearReward(int32 ChapterId, EStageDifficulty Difficulty, int32 nRewardIndex);
	void				ResponseChapterClearReward(const FB2GetReceiveActClearReward& ResponseRewardInfo);
	void				RequestGetRepeatMissionClearReward(FServerStageID ServerStageId);
	void				ResponseRepeatMissionClearReward(const FB2GetReceiveActRepeatClearReward& ResponseRewardInfo);

	FORCEINLINE void	SetAdventureActType(EAdventureActType InType) { SelectActButtonType = InType; }
	FORCEINLINE EAdventureActType	GetAdventureActType() { return SelectActButtonType; };

private:
	void				CacheActData(int32, EStageDifficulty, FB2OriginGetActInfoPtr);
	void				CacheStageData(FServerStageID, FB2OriginGetStageInfoPtr);
	void				ChangeChapterScene();
	void				SetActClearStageData(FServerStageID ServerStageId, int32 ClearGrade);
	void				CheckChangeLastClearStage(const FB2OriginAccountInfoPtr Current, const FB2OriginAccountInfoPtr Next);

public:
	const int32 OriginalChapterDivision = 10000;

	// member
private:
	TMap<FName, FB2ActData>							CachedActDatas;	// Cached for Act Data / Key : {ActId}_{Difficulty}
	TMap<FServerStageID, FB2StageData>				CachedStageDatas; // Cached for each start stage req/response. We also got stage master data, see StageGameplayData. / Key : ServerStageId

	TMap<FName, FServerStageID>						ServerStageIdMappingKeys; // Key is pair<map_id, difficulty> / Value is ServerStageId
	TMap<FServerStageID, FStageInfoGameplayData>	StageGameplayDatas; // Key is serverStageId (stage number)

	bool											bChangeChapterShortCut;
	FServerStageID									ShortCutSvrStageID;
	FServerStageID									CachedServerStageIDForRequestGetActInfo;
	
	TMap<int32, FStageFixedDropsInfo>				MD_StageFixedDrops;


	TArray<FName>									CachedActStarRewards;


	EAdventureActType								SelectActButtonType;

	// Requested Backup's
private:
	int32				RequestedActId;
	EStageDifficulty	RequestedActDifficulty;
	FServerStageID		RequestedServerStageId;
	int32				RequestedActIdForChapterClearReward;
	EStageDifficulty	RequestedDifficultyForChapterClearReward;
	int32				RequestedActIdForRepeatClearReward;
	EStageDifficulty	RequestedDifficultyForRepeatClearReward;

	bool				bHasRequestedActInfo;
	bool				bHasRequestedActInfoWithChangeSceneReserve;

	TArray<std::shared_ptr<class issue>> Issues;
};

#endif // __B2_STAGE_DATA_STORE_H__