// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIRaidResult.generated.h"


/**
 * 
 */


struct FRaidResultRankingInfo
{
	int32 NetID;
	int32 Contribution;
	int32 Level;
	EPCClass PCClass;
	bool IsExitGame;
	FText CharName;
	int64 AccountID;
	bool IsMyGuildMan;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2RaidResult : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

	enum class ERaidRankSlot : uint8
	{
		Slot_0,
		Slot_1,
		Slot_2,
		Slot_3,
		Max,
	};
public:
	virtual void Init() override;
	virtual void NativeConstruct() override;

	void ShowResult();
protected: 
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void OnClicked_Bonus();
	UFUNCTION()
	void OnClicked_Lobby();
	UFUNCTION()
	void OnClicked_Raid();
	UFUNCTION()
	void OnClicked_Retry();
	UFUNCTION()
	void OnClicked_PartyRetry();
	UFUNCTION()
	void OnClicked_SkipRoulette();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayBonusItemAnim(int32 nStep);

	UFUNCTION(BlueprintCallable, Category = BladeII)
	void FinishBonusItemAnim();
	UFUNCTION(BlueprintCallable)
	void OnReadyToRoulette();
private:
	void OnFinishedSpinRoulette();

	void SetResultHudInfo();
	void SetRouletteInfo(const TArray<FRaidResultRankingInfo> &IndexArray);
	void SetVisibleBonusButton(bool bIsvisible);
	void SetResultSlotInfo(const TArray<FRaidResultRankingInfo> &IndexArray);

	void SetCloseTimeON();
	void OnLevelUpFXFinish();

	TArray<FRaidResultRankingInfo> GetRankArrayInfo();
	
	void CheckAndOpenLevelUpPart();
public:
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TArray<UTexture2D*>	BossImages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TArray<FLinearColor>	RouletteColors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BladeII)
		int32 BonusItemNumber;
private:
	TWeakObjectPtr<UHorizontalBox> X_BottomButton;
	TWeakObjectPtr<UB2Button> BTN_Bonus;
	TWeakObjectPtr<UB2Button> BTN_Lobby;
	TWeakObjectPtr<UB2Button> BTN_Raid;
	TWeakObjectPtr<UB2Button> BTN_Retry;
	TWeakObjectPtr<UB2Button> BTN_PartyRetry;
	TWeakObjectPtr<UB2Button> BTN_SkipRoulette;
	
	TWeakObjectPtr<UB2RichTextBlock> RTB_Close;
	TWeakObjectPtr<UCanvasPanel> CP_Bottom;

	TWeakObjectPtr<UTextBlock> RTB_RaidName;
	TWeakObjectPtr<UB2RichTextBlock> RTB_ClearDifficulty;
	TWeakObjectPtr<UCanvasPanel> CP_LeftStatePanel;

	TWeakObjectPtr<UTextBlock> RTB_PlayTime;
	TWeakObjectPtr<UCanvasPanel> CP_PlayTime;

	TWeakObjectPtr<UTextBlock> RTB_GuideContribution;
	TWeakObjectPtr<UOverlay> CP_Contribution;

	TWeakObjectPtr<UB2RichTextBlock> RTB_GuideBonus;
	TWeakObjectPtr<class UB2UIRaidRoulette> Roulette;
	TWeakObjectPtr<UPanelWidget> CP_Roulette;

	TWeakObjectPtr<class UB2UIRewardIcon> BonusItem;

	TArray<TWeakObjectPtr<class UB2UIRaidResultSlot>> RankSlots;

	TWeakObjectPtr<class UB2UIStageResultLevelUp> UIP_LevelUp;

	TWeakObjectPtr<UTextBlock> STB_GotoLobby;
	TWeakObjectPtr<UTextBlock> STB_GotoRaid;
	TWeakObjectPtr<UTextBlock> STB_ReTry;
	TWeakObjectPtr<UTextBlock> STB_PartyRetry;
	TWeakObjectPtr<UTextBlock> STB_ViewBonus;

	TWeakObjectPtr<UImage> IMG_Boss;
	
	FTimerHandle	TimeToAutoMoveStage;

	int32 CloseTimeCount;
	bool IsStartCloseTimer;
	float DeltaTimeCount;
};