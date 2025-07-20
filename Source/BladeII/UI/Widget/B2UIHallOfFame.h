// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "../../Lobby/B2LobbySceneManager.h"
#include "../B2RichTextBlock.h"
#include "B2UIHallOfFame.generated.h"

UCLASS()
class BLADEII_API UB2UIHallOfFame : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	UB2UIHallOfFame(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void RegisterUIMarkForRedDot() final;

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void PlayBestPopupAnimation();
	UFUNCTION()
		void OnClick_PvP();
	UFUNCTION()
		void OnClick_TeamMatch();
	UFUNCTION()
		void OnClick_Controll();
	UFUNCTION()
		void OpenPraiseToolTip();
	UFUNCTION()
		void ClosePraiseToolTip();
	UFUNCTION()
		void OpenCelebrate();
	UFUNCTION()
		void CloseCelebrate();

	bool RedDotCondition_PvP();
	bool RedDotCondition_Team();
	bool RedDotCondition_Control();
	
	EHallOfFameMode GetHallOfFameMode() const;
	FText GetModeString(EHallOfFameMode InFameMode);
	bool GetIsTest() const { return IsTest; }

	void SetMenuTabSelected();
	void SetSettleState();
	void SetCelebratePopup(EHallOfFameMode InMode ,int32 InRanking, TArray<b2network::B2RewardPtr>& InRewardItem);
	void SetVisiblePraise(bool InPraiseButton, int32 InRanker);
	void SetPlayerInfo(const TArray<FRankerInfo>& Rankers);
	void SetIsTest(bool InTest) { IsTest = InTest; }
	void SetHallOfFamePraiseInfo(const b2network::B2HallOfFamePraisePtr& PraiseInfo, int32 InRanking);
	void SetHallOfFamePraiseInfo(int32 InMaxPraise, int32 InCurrentPraise, int64 InRanking = 0);
	void SetVisiblePlayerInfo(bool InVisible);
	void SetVisibleForMatinee(bool InVisible);

	void AddPlayer(const FHallOfFameCharacterInfo& PlayerInfo, bool IsVisible = true);
	void SceneForceUpdate();
	void ClearPlayerInfo();

private:
	TMap<EHallOfFameMode, bool> EnablePraise;
	TArray<TWeakObjectPtr<class UB2UICommonTab>> MenuTab;
	TArray<class UB2UIHallOfFamePlayerInfo*> PlayerInfoUI;

	bool IsTest;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
		TSubclassOf<class UB2UIHallOfFamePlayerInfo> PlayerInfoClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
		TSubclassOf<class UB2UIHallOfFameToolTip> ToolTipInfo;

	TWeakObjectPtr<UPanelWidget> P_RankerPanel;

	TWeakObjectPtr<UB2Button> BTN_CelebrateRanker;
	TWeakObjectPtr<UB2Button> BTN_OpenPraiseToolTip;
	TWeakObjectPtr<UB2Button> BTN_ClosePraiseToolTip;

	TWeakObjectPtr<UTextBlock>		TB_Celebrate;
	TWeakObjectPtr<UTextBlock>		TB_Reward;
	TWeakObjectPtr<UTextBlock>		TB_ToMail;
	TWeakObjectPtr<UTextBlock>		TB_LikeNUM;
	TWeakObjectPtr<UTextBlock>		TB_LikeText;
	TWeakObjectPtr<UTextBlock>      TB_RewardInfo;
	TWeakObjectPtr<UTextBlock>		TB_SeasonToolTip;

	TWeakObjectPtr<UB2RichTextBlock>	TB_Ranking;

	TWeakObjectPtr<UPanelWidget>		 P_MenuTab;
	TWeakObjectPtr<UPanelWidget>		 OV_LikeInfo;
	TWeakObjectPtr<UPanelWidget>		 OV_SeasonMsg;

	TWeakObjectPtr<class UB2UIHallOfFameSkipButton> UIP_HallOfFameSkipButton;

	TArray<TWeakObjectPtr<UPanelWidget>> P_CelebrateRank;
	UB2UIHallOfFameToolTip* ToolTipWidget;
};
