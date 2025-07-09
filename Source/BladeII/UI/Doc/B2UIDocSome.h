// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "CommonStruct.h"
#include "B2UIDocSome.generated.h"

/* This is the place where you can put "SOME" fucking data that you have no idea of where to put shits in. */
UCLASS()
class BLADEII_API UB2UIDocSome : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocSome(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocSome();

	virtual void Init() override;
	virtual void Destroy() override;

	void SetPraiseInfo(const TArray<b2network::B2HallOfFamePraisePtr>& PraiseInfo);
	void SetPraiseInfo(const b2network::B2HallOfFamePraisePtr& PraiseInfo);

	bool GetPraiseInfo(EHallOfFameMode InHallOfFame) const;

	// Whether local player recently won NetGame (PVP) match, or remote opponent won, or draw.
	DECLARE_UIDOC_FIELD(ENetMatchResult, LocalPlayerMatchResult)
	DECLARE_UIDOC_FIELD(int32, RankingChanged)

	DECLARE_UIDOC_FIELD(TArray<b2network::B2RewardPtr>, Rewards)

	//DECLARE_UIDOC_STRUCT_FIELD(FModRankerInfo, MyRankerInfo)
	//DECLARE_UIDOC_STRUCT_FIELD(FModRankerInfo, OtherRankerInfo)

	DECLARE_UIDOC_FIELD(TArray<bool>, OutAutoSellCatergoryFilter)
	DECLARE_UIDOC_FIELD(int32, OutAutoSellGradeFilter)
	DECLARE_UIDOC_FIELD(bool, OutAutoSellEtherCatergoryFilter)
	DECLARE_UIDOC_FIELD(int32, OutAutoSellEtherGradeFilter)

	DECLARE_UIDOC_FIELD(bool, IsCharacterObserveScene)
	DECLARE_UIDOC_FIELD(bool, OnClickHallOfFame)
	DECLARE_UIDOC_FIELD(bool, OnClickUserToHallOf)
	DECLARE_UIDOC_FIELD(EHallOfFameMode, HallOfFameMode)
	DECLARE_UIDOC_FIELD(EPCClass, CurrentBestCharacter)

	DECLARE_UIDOC_FIELD(int32, CurrentChatChannel)

	DECLARE_UIDOC_FIELD(int32, CounterDGMainRewardItemId)
	DECLARE_UIDOC_FIELD(int32, CounterDGMainRewardItemNum)

	DECLARE_UIDOC_FIELD(int32, CounterDGSubRewardItem01Id)
	DECLARE_UIDOC_FIELD(int32, CounterDGSubRewardItem01Num)

	DECLARE_UIDOC_FIELD(int32, CounterDGSubRewardItem02Id)
	DECLARE_UIDOC_FIELD(int32, CounterDGSubRewardItem02Num)

	DECLARE_UIDOC_FIELD(int32, CompleteMissionType)

	DECLARE_UIDOC_FIELD(EPCClass, CollectBookSelectClass)
	
	/** To be converted to/from EWingIngredAcqElemType */
	DECLARE_UIDOC_FIELD(int32, WingIngredAcqPopupReason)

	DECLARE_UIDOC_FIELD(MailTabIndex, OpenedMailTab)

	DECLARE_UIDOC_FIELD(int64, FindUserAccountID)

	DECLARE_UIDOC_FIELD(bool, bModeOpenNoticeRedDotCheck)

	DECLARE_UIDOC_FIELD(bool, IsOpenRaid)
	DECLARE_UIDOC_FIELD(bool, IsOpenAssault)
	DECLARE_UIDOC_FIELD(int32, GuildBattleState)

	DECLARE_UIDOC_FIELD(int64, SendReportUserAccountID)

private:
	TMap<EHallOfFameMode, bool> EnablePraise;

public:
    // @ReplayMode
    FString ReplayFileName;
    //FModReplayInfo ReplayInfo;
};
