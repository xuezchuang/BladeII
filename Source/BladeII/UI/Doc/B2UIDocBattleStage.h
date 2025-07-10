// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "CommonStruct.h"
#include "B2ClientDataStore.h"
#include "B2UIDocBattleStage.generated.h"

/* The document represents the current stage to start the battle. */
UCLASS()
class BLADEII_API UB2UIDocBattleStage : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocBattleStage(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocBattleStage();

	DECLARE_UIDOC_FIELD(FText, CurAppearedMobName)
	DECLARE_UIDOC_FIELD(FText, CurAppearedMobNickName)
	DECLARE_UIDOC_FIELD(int32, ClientStageId)
	DECLARE_UIDOC_FIELD(EStageDifficulty, StageDifficulty)
	DECLARE_UIDOC_FIELD(FServerStageID, ServerStageId)

	DECLARE_UIDOC_FIELD(FText, LocalizedAreaName)
	DECLARE_UIDOC_FIELD(FText, LocalizedAreaSubName)

	// UI selection for LobbyGameMode, stage start selection for StageGameMode
	DECLARE_UIDOC_FIELD(TArray<EStageBuffType>, SelectedStageBuffs)

	/* Preview Item */
	//Reward Items
	DECLARE_UIDOC_FIELD(TArray<FB2RewardItemPreviewInfo>, RewardItemList)
	//BossDrop Items
	DECLARE_UIDOC_FIELD(FStageFixedDropsInfo*, BossDropItemsList)
	//Aether Items
	DECLARE_UIDOC_FIELD(TArray<FB2AetherItemPreviewInfo>, AetherItemList)
	//Aether Items
	DECLARE_UIDOC_FIELD(TArray<FB2MaterialItemPreviewInfo>, MaterialItemList)

	/* Reward Item */
	DECLARE_UIDOC_FIELD(int32, StageClearMultipleValue) 
	//Stage Clear Reward Items( max num = 2 )
	DECLARE_UIDOC_FIELD(TArray<FB2Item>, StageClearRewardItemList)
	//Stage Clear Ether Items
	DECLARE_UIDOC_FIELD(TArray<FB2Item>, StageClearEtherItemsList)
	//Stage Clear Material Items
	DECLARE_UIDOC_FIELD(TArray<FB2Item>, StageClearMaterialItemsList)
	//Stage Clear Sold Items
	DECLARE_UIDOC_FIELD(TArray<FB2Item>, StageClearSoldItemsList)
	//Stage Result BossDrop Items
	DECLARE_UIDOC_FIELD(TArray<FB2Item>, StageResultBossDropItemsList)

	DECLARE_UIDOC_FIELD(int32, EndGoldBuffRate)
	DECLARE_UIDOC_FIELD(int32, EndExpBuffRate)

	DECLARE_UIDOC_FIELD(int32, StartGoldBuffRate)
	DECLARE_UIDOC_FIELD(int32, StartExpBuffRate)

	//RepeatMission
	DECLARE_UIDOC_FIELD(int32, RepeatMissionCurNum)
	DECLARE_UIDOC_FIELD(int32, RepeatMissionMaxNum)
	DECLARE_UIDOC_FIELD(FIntPoint, RepeatMissionReward)		// X=ItemRefID, Y=Reward Count

	//RepeatBattle
	DECLARE_UIDOC_FIELD(FIntPoint, RepeatBattleCost)		// X=ItemRefID, Y=Cost Count

	DECLARE_UIDOC_FIELD(EUIBattleDifficulty, BattleDifficulty)
	DECLARE_UIDOC_FIELD(int32, RequiredStaminaNum)

	DECLARE_UIDOC_FIELD(int32, StagePlayTime) // Time in second, not including event scene play time.
	DECLARE_UIDOC_FIELD(int32, StageClearGrade) // Being set when a stage is just cleared, it actually depends on StagePlayTime.
	DECLARE_UIDOC_FIELD(bool, AllServival)
	DECLARE_UIDOC_FIELD(TArray<bool>, StageClearConditions)

	DECLARE_UIDOC_FIELD(bool, RepeatBattleCurrentOn)
	DECLARE_UIDOC_FIELD(int32, RepeatBattleCurrentCount)
	DECLARE_UIDOC_FIELD(bool, RepeatBattleLoopOne)
	DECLARE_UIDOC_FIELD(bool, RepeatBattleLoopAll)
	DECLARE_UIDOC_FIELD(bool, RepeatBattleBoostOn)
	DECLARE_UIDOC_FIELD(int64, RepeatBattleBoostRemainCount)
	DECLARE_UIDOC_FIELD(int64, RepeatBattleBoostMaxCount)

	DECLARE_UIDOC_FIELD(float, SuggestedAttackPower)
	DECLARE_UIDOC_FIELD(float, SuggestedDefensePower)
	DECLARE_UIDOC_FIELD(int32, RecommendPower)
	DECLARE_UIDOC_FIELD(int32, RecommendAttack)
	DECLARE_UIDOC_FIELD(int32, RecommendDefense)

	DECLARE_UIDOC_FIELD(int32, StageRewardGold)
	DECLARE_UIDOC_FIELD(int32, StageQTEGold)
	DECLARE_UIDOC_FIELD(int32, StageDropGold)
	DECLARE_UIDOC_FIELD(int32, StageTotalGold)
	DECLARE_UIDOC_FIELD(int32, StageSoldGold)

	DECLARE_UIDOC_FIELD(EArinConsultPreCombat_MainSubject, ArinConsultingSubject)

	// Tutorial Check
	DECLARE_UIDOC_FIELD(int32, ProcessableTutorialID);
};
