#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIMsgPopupSimple.h"
#include "B2ClientDataStore.h"
#include "B2UIMsgPopupReward.generated.h"

/**
 * 
 */

enum class ERewardState : uint8
{
	Preview = 0,
	RequestReward,
	Rewarded,

	Max
};

UCLASS()
class BLADEII_API UB2UIMsgPopupReward : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()

	TWeakObjectPtr<UImage>		IMG_Background;
	
	TWeakObjectPtr<UButton>		BTN_Confirm;
	TWeakObjectPtr<UTextBlock>	TB_Confirm;

	TWeakObjectPtr<UButton>		BTN_Close;

	TWeakObjectPtr<UTextBlock>	TB_RewardTitle;
	TWeakObjectPtr<UTextBlock>	TB_Description;

	TArray<FCommonRewardInfo>	ShowRewardList;
	TArray<TWeakObjectPtr<class UB2UIRewardIcon>> RewardSlots;

	FMsgPopupOnClick			ConfirmDelegate;

	TWeakObjectPtr<UTextBlock>	TB_MailDescription;

	TWeakObjectPtr<UCanvasPanel>	X_CP_Jackpot;
	TWeakObjectPtr<UTextBlock>		TB_Jackpot;

	int32	TotalRewardPage;
	int32	CurrentRewardPage;

	bool	bForceApplyRewardToAccount;
	bool	bOpenNickNamePopup;

	bool	bIsJackpotReward;

protected:
	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

	bool HasAnyRewardToShow();
	void UpdateRewardPage(int32 ShowRewardPage);
	void ShowNextReward();

	void ApplyCurrentPageReward(int32 RewardPage);
	void ApplyRewardToClientDataStore(const FCommonRewardInfo& RewardInfo);
	void ApplyRewardToClientDataStore(b2network::B2RewardPtr RewardPtr);
	
	void AddRewardItem_Internal(const FCommonRewardInfo& RewardInfo);

protected:

	UFUNCTION()
	void OnClickConfirm();

	UFUNCTION()
	void OnClickClose();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OpenRewardAnimationBP();

	UFUNCTION(BlueprintCallable)
	bool IsJackpotReward();

public:

	UB2UIMsgPopupReward(const FObjectInitializer& ObjectInitializer);
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void InitRewardMsgPopupText(const FText& TitleText, const FText& BtmDescText);
	void InitRewardMsgPopup(const TArray<b2network::B2RewardPtr>& RewardList);


	void AddRewardItems(const TArray<b2network::B2RewardPtr>& RewardList, bool bShowRewardNow = false);
	void AddSingleRewardItem(ERewardType RewardType, int32 Count);
	void AddSingleRewardItem(int32 ItemTemplateId, int32 Count);
	void AddSingleRewardItem(const b2network::B2RewardPtr RewardPtr);
	void AddSingleRewardItemByTemplateId(int32 ItemTemplateId, int32 Count);

	void ShowRewardPopup(bool bApplyRewardToAccount = false);
	void ShowMailDescription(bool bVisible);

	void SetConfirmCallback(const FMsgPopupOnClick& Handler);

	void UpdateDescriptionText(const FText& DescText);

	void SetOpenNickNameChangePopup();
	void SetJackpotReward(bool bJackpot);
};
