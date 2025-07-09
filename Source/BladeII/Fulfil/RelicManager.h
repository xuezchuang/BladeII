// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "B2MessageInfoCommonStruct.h"
#include "EventSubsystem.h"
#include "CharacterDataStore.h"

struct FMD_AncientRelicLevelElem
{
	FMD_AncientRelicLevelElem(int32 id, int32 level, FItemOption modeOption_1, FItemOption modeOption_2, FItemOption modeOption_3, int32 rate, int32 gold, int32 heropiecevalue)
		: nRelicId(id)
		, nEnhance_Level(level)
		, nModeOption_Attack(modeOption_1)
		, nModeOption_Defence(modeOption_2)
		, nModeOption_Health(modeOption_3)
		, nEnhance_Rate(rate)
		, nEnhance_Gold(gold)
		, nNeed_HeroPiece(heropiecevalue)
	{
	}

	explicit FMD_AncientRelicLevelElem()
		: nRelicId(0)
		, nEnhance_Level(0)
		, nEnhance_Rate(0)
		, nEnhance_Gold(0)
		, nNeed_HeroPiece(0)
	{
	}

	int32 nRelicId;
	int32 nEnhance_Level;
	FItemOption nModeOption_Attack;
	FItemOption nModeOption_Defence;
	FItemOption nModeOption_Health;
	int32 nEnhance_Rate;
	int32 nEnhance_Gold;
	int32 nNeed_HeroPiece;
};

struct FMD_AncientRelicGradeElem
{
	FMD_AncientRelicGradeElem(int32 id,int32 grade, FItemOption option_1, FItemOption option_2, FItemOption option_3, int32 rate, int32 gold,int32 bossid,int32 bossValue)
		: nRelicId(id)
		, nGrade(grade)
		, nOption_Attack(option_1)
		, nOption_Defence(option_2)
		, nOption_Health(option_3)
		, nPromotion_Rate(rate)
		, nPromotion_Gold(gold)
		, nPromotion_BossPiece(bossValue)
		, nPromotion_BossPieceid(bossid)
	{
	}

	explicit FMD_AncientRelicGradeElem()
		: nRelicId(0)
		, nGrade(0)
		, nPromotion_Rate(0)
		, nPromotion_Gold(0)
		, nPromotion_BossPiece(0)
		, nPromotion_BossPieceid(0)
	{
	}

	int32 nRelicId;
	int32 nGrade;
	FItemOption nOption_Attack;
	FItemOption nOption_Defence;
	FItemOption nOption_Health;
	int32 nPromotion_Rate;
	int32 nPromotion_Gold;
	int32 nPromotion_BossPiece;
	int32 nPromotion_BossPieceid;
};

struct FMD_AncientRelicSingleElem
{
	FMD_AncientRelicSingleElem()
		: nRelicId(0)
		, nOpenStage(0)
	{
	}

	FMD_AncientRelicSingleElem& operator=(const b2network::B2mdRelicInfoPtr& ServerStruct);

	int32 nRelicId;
	int32 nOpenStage;

	TMap<int32, FMD_AncientRelicGradeElem> MD_AncientRelicGrade;
	TMap<int32, FMD_AncientRelicLevelElem> MD_AncientRelicLevel;
};

//ENum 순서가 의미가 있음. M18에 고대유물 리뉴얼에 고대유물 정리를 참고하면 서버 ID와 매칭 됨.
//순서를 바꿀라면 서버랑 먼저 맞춰야 함.
enum EApplyMode : uint8
{
	EApplyMode_None,
	EApplyMode_HeroTower	= 1,
	EApplyMode_PVP_Tag		= 2,
	EApplyMode_Control		= 3, //팀 점령전인데....
	EApplyMode_Raid			= 4,
	EApplyMode_PVP_Team		= 5,
	EApplyMode_Guild		= 6,
	//EApplyMode_Assault, //도전의 탑
	EApplyMode_Total,
	//Total보다 크면 GetPC(Attack)(Defence)(Health)ByMod에서 return함으로
	//사용시 여기서 Total보다 위로 올릴것
	//EApplyMode_Scenario,
	//EApplyMode_CounterDungeon,
	//EApplyMode_Tutorial,
	//EApplyMode_Lobby,
};

class RelicManager
{
public:
	explicit RelicManager()
		: MAX_RELIC_LEVEL(10)
	{ 
		MD_AncientRelic.Empty();
		MD_RelicInfo.Empty();
	}

	void Init();

	void SubscribeEvents();
	void UnsubscribeEvents();

	TArray<b2network::B2mdRelicInfoPtr>							MD_RelicInfo;
private:
	//RelicManager(RelicManager const&) = delete;
	//RelicManager operator = (RelicManager const&) = delete;

	void ReceiveRelicMasterData(const FB2MasterDatas& InMasterDatas);
	void SetMyActiveRelicsByClass(EPCClass nClass, FB2ResponseGetAccountRelicPtr msgPtr);

public:
	//ClientDataStore Copy
	int32 GetRelicNum();
	int32 getRelicOpenStageID(int32 nRelicId);

	const FMD_AncientRelicLevelElem* getRelicLevelInfo(int32 nRelicId, int32 nLevel);
	const FMD_AncientRelicGradeElem* getRelicGradeInfo(int32 nRelicId, int32 nGrade);
	TArray<float> GenerateLevelOptionValue(int32 nRelicID, int32 nGrade, int32 nLevel);
	TArray<float> GenerateGradeOptionValue(int32 nRelicID, int32 nGrade);

	int32 ConvertModeNameToEApplyMode(EB2GameMode ModeType);

	bool GetIsReddotRelicByClass(int32 nClass);
	bool GetIsReddotByRelicId(EPCClass nClass,int32 nRelicID);

	float GetPCAttackByMode(EPCClass InPCClass, EB2GameMode ModeType, int32 nRelicId, int32 nGrade, int32 nLevel);
	float GetPCDefenseByMode(EPCClass InPCClass, EB2GameMode ModeType, int32 nRelicId, int32 nGrade, int32 nLevel);
	float GetPCHealthByMode(EPCClass InPCClass, EB2GameMode ModeType, int32 nRelicId, int32 nGrade, int32 nLevel);

	FORCEINLINE int32 GetRelicGradeMax() { return MAX_RELIC_GRADE; }
	FORCEINLINE int32 GetRelicLevelMax() { return MAX_RELIC_LEVEL; }

private:
	TMap<int32, FMD_AncientRelicSingleElem> MD_AncientRelic;

	int32 MAX_RELIC_GRADE;
	const int32 MAX_RELIC_LEVEL;

	TArray<std::shared_ptr<issue>> Issues;
};
