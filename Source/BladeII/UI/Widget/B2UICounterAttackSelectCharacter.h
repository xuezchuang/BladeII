#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "../B2RichTextBlock.h"
#include "B2UICounterAttackSelectCharacter.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICounterAttackSelectCharacter : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	~UB2UICounterAttackSelectCharacter();

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	//virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	void SetSelectCharacter();
	void SetEnterCount(int32 Count, int32 MaxCount);
	void SetMaxDifficulty();

	const FString DayDivision(EUICounterDayType Day);
	const FString DungeonTypeDivision(int32 DungeonType);

	void OnChangedCurPCClass(class UB2UIDocBase* Sender, int32 CurPCClass, int32 PrevCurPCClass);
	void OnChangedTagPCClass(class UB2UIDocBase* Sender, int32 TagPCClass, int32 PrevTagPCClass);
	void OnHighestUpdate(class UB2UIDocBase* Sender, int32 CurLv, int32 PreLv);

	void ChangeDifficulty(int32 InDifficulty);
	void RefreshRecommendStats(int32 RecommandPower, int32 RecommendAttack, int32 RecommendDefense);

	void OnClickEntry(EPCClass InEntry);

	void SweepButtonState(int32 InHighest);

	//void RepeatBattleBoostOnChanged(class UB2UIDocBase* Sender, bool newFactor, bool oldFactor);

	bool CheckBuyTicket();
	void SendBuyTicket();
	bool CheckAndInvokeArinConsulting();
	bool SetArinCheck();
	void StartBattle();
	void OnCounterDungeonSweep();

	const TMap<int, FRewardItem>& GetSweepItems();

	void AckCounterrSweep(const FB2SweepCounterDungeonInfo& MsgPtr);
	void OpenSweepResultPopup(const FB2SweepCounterDungeonInfo& MsgPtr);

	void SubscribeEvent();
	void UnsubscribeEvent();
	

	virtual void OnOpen(bool RightNow = false) override;

	UFUNCTION()
	void OnClickBtnGoToHeroMgmt();

	UFUNCTION()
	void OnClickSweepBattle();

	void SetPreRewardList();
	void AddPreRewardListItem(int32 rewardid, int32 rewardcount, int32 rewardcountMax);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UB2UIRewardIcon>			RewardInfoClass;
protected:
	TWeakObjectPtr<class UB2UIPVPEntryHero>		UIP_Entry;

	TWeakObjectPtr<UTextBlock>					TB_RecommandPower;
	TWeakObjectPtr<UTextBlock>					TB_CombatPower;

	TWeakObjectPtr<UTextBlock>					TB_DailyEnter;

	TWeakObjectPtr<UButton>						BTN_ChangeHero;
	TWeakObjectPtr<UButton>						BTN_GoToHeroMgmt;
	TWeakObjectPtr<UButton>						BTN_Sweep;

	TWeakObjectPtr<UTextBlock>					TB_SweepButton;

	TWeakObjectPtr<UTextBlock>					STB_DungeonLv;
	TWeakObjectPtr<UTextBlock>					STB_EquipManagement;
	TWeakObjectPtr<UTextBlock>					STB_RecommandPower;
	TWeakObjectPtr<UTextBlock>					STB_CombatPower;
	TWeakObjectPtr<UTextBlock>					STB_EntenHero;
	TWeakObjectPtr<UTextBlock>					STB_RewardInfo;
	TWeakObjectPtr<UTextBlock>					STB_DailyEnter;
	TWeakObjectPtr<UB2RichTextBlock>			STB_JoinExplanation;

	TWeakObjectPtr<UTextBlock>					TB_MonsterName;
	TWeakObjectPtr<UTextBlock>					TB_MonsterDesc;
	TWeakObjectPtr<UTextBlock>					TB_BestScoreDesc;

	TWeakObjectPtr<UHorizontalBox>				List_Reward;

	TArray<TWeakObjectPtr<class UB2UIPVPEntryHero>>							UIP_EntryArr;

	TWeakObjectPtr<class UB2UIDungeonDifficulty>					UIP_DungeonLevelofDifficulty;

	TWeakObjectPtr<class UB2UIStartButton>							UIP_StartBT;

	TWeakObjectPtr<class UB2UIRecommandPower>						UIP_StageRecommendPower;
	TWeakObjectPtr<class UB2UIFairyLinkButton>	UIP_FairyLinkButton;

	TMap<int, FRewardItem> SweepItems;
private:
	uint32 DeliveryBuyTicket;
	int32	nCurrentCombatPower;
	int32 CurrentAttack;
	int32 CurrentDefense;
	int32 BestLevel;
	FServerStageID BestLevelServerStageID;

	uint32 DeliverySweepCounterTicket;
};