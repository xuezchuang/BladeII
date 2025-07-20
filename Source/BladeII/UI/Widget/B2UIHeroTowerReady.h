// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "B2UIHeroTowerReady.generated.h"


/**
*
*/
UCLASS()
class BLADEII_API UB2UIHeroTowerReady : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()
public:
	UB2UIHeroTowerReady(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	float GetTowerPosition();
	int32 GetHerotowerEnterCost();

	void SetReadyToScroll();
	void CreatePopupBuyTryCount();
	void OnConfirmBuyTryCount();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationEvent_Anim_Visible();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationEvent_Anim_Invisible();

	FORCEINLINE EHeroTowerReadyUIState GetHeroTowerUIState() { return m_eUIState; }

private:
	void SubscribeEvent();
	void UnsubscribeEvent();

	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void OnClickBtnStartBattle();

	UFUNCTION()
		void OnClickBtnStartSweep();

	UFUNCTION()
		void OnClickBtnRanking();

	UFUNCTION()
		void OnClickBtnGoCurrentFloor();

	void HiddenFloorInfo();
	void SetFloorInfo();
	void SetFloorInfoValue(int32 nFloor);

	void ResetTowerScroll();

	int32 GetCurrentFloorIndex(); // 0 ~

	void CheckUIVisible();
	void CheckTowerInitMove();

	void OnHeroTowerSweep();
	void SendHeroTowerSweep();
	void AckHeroTowerSweep(const FB2ResponseSweepHeroTowerPtr& MsgPtr);
	void ResponseBuyHeroTowerTicket(const FB2ResponseBuyHeroTowerTicketPtr& MsgPtr);

	void OpenSweepResultPopup();

	void SetStaticText();

	void CheckAndOpenLevelUpPart();
	void ConditionalCreateLevelUpPart();
	void DestroyLevelUpPart();

	bool OnCheckAbleToSweep();

	const TMap<int, FRewardItem>& GetSweepItems();

private:
	TWeakObjectPtr<UB2Button> BTN_StartSweep;
	TWeakObjectPtr<UB2Button> BTN_StartBattle;
	TWeakObjectPtr<UB2Button> BTN_Ranking;
	TWeakObjectPtr<UB2Button> BTN_MyFloor;
	TWeakObjectPtr<UCanvasPanel> CP_BTN_StartSweep;
	TWeakObjectPtr<UCanvasPanel> CP_BTN_StartBattle;
	TWeakObjectPtr<UB2ScrollBox> SB_InputTower;

	TWeakObjectPtr<UTextBlock> TB_Floor;
	//TWeakObjectPtr<UTextBlock> TB_RecordTime;
	TWeakObjectPtr<UTextBlock>	TB_CombatPower;
	TWeakObjectPtr<UTextBlock>	TB_RecommandPower;
	TWeakObjectPtr<UHorizontalBox> HB_Rewards;
	TWeakObjectPtr<UTextBlock> TB_TodayEnterCount;

	TWeakObjectPtr<UTextBlock> STB_BattleInfo;
	TWeakObjectPtr<UTextBlock> STB_RewardInfo;
	TWeakObjectPtr<UTextBlock> STB_RecommandPower;
	TWeakObjectPtr<UTextBlock> STB_CombatPower;
	TWeakObjectPtr<UTextBlock> STB_BattleReady;
	TWeakObjectPtr<UTextBlock> STB_Sweep;
	TWeakObjectPtr<UTextBlock> STB_Raking;

	TWeakObjectPtr<class UB2UIRecommandPower> UIP_RecommendPower;

	TWeakObjectPtr<UCanvasPanel> X_CP_LevelUpPanel;

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UB2UIRewardIcon> RewardItemClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UB2UIWidget> HeroTowerScrollItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InitMoveSpeed")
		float MoveToInitFloorWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InitMoveSpeed")
		float TotalMoveToInitFloorTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InitMoveSpeed")
		float MoveToInitFloorBaseSpeed;

	UPROPERTY(Transient)
		class UB2UIStageResultLevelUp* CreatedUIP_LevelUp;

	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIStageResultLevelUp> UIP_LevelUpClass;

private:
	bool m_bIsReadyToScroll;
	float m_fStartScrollOffSet;

	bool m_bIsSetFloorInfo;

	int32 m_nScrollStartIndex;

	FTimerHandle TimeToSetFloor;
	FTimerHandle TimeSweepPopupWait;

	float m_fMoveToInitFloorStartTime;
	float m_fTowerPosForMoveToInitFloor;

	EHeroTowerReadyUIState m_eUIState;

	int32 m_nTempHeroPieceAmount;

	bool bIsSweep;
	TMap<int, FRewardItem> SweepItems;

	TArray<std::shared_ptr<class issue>> Issues;
};
