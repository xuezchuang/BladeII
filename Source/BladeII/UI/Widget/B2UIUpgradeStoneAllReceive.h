// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIUpgradeStoneAllReceive.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIUpgradeStoneAllReceive : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	UB2UIUpgradeStoneAllReceive(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetUpgradeStoneMailInfo(const TArray<TWeakObjectPtr<class UB2UIMailContent>>& InMailInfo);
	void SetRareUpgradeStoneMailInfo(const TArray<TWeakObjectPtr<class UB2UIMailContent>>& InMailInfo);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION()
		void OnClickOK();
	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnChangedMailCount(float ChangeValue);
	UFUNCTION()
		void OnClickMailCountDown();
	UFUNCTION()
		void OnClickMailCountUp();
	UFUNCTION()
		void OnClickMax();

	void SetMax();

	void SetTBCount(int32 InValue);
	int32 CheckReceiveCount(int32 InValue);
	void SetValueMailCountSlider(int32 InValue);
	void SetSelectedItemRefId(int32 InValue);

private:
	UPROPERTY(EditAnywhere, Category = "LotteryMail")
		int32 MaxCountUpgradeStoneMail;

	TArray<int64> MailIDs;
	int32 ReceiveCount;
	int32 RefID;
	TWeakObjectPtr<UB2Button> BTN_OK;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_MailCountDown;
	TWeakObjectPtr<UB2Button> BTN_MailCountUp;
	TWeakObjectPtr<UB2Button> BTN_Max;
	TWeakObjectPtr<USlider> S_MailCount;
	TWeakObjectPtr<UTextBlock> TB_Count;
	TWeakObjectPtr<UTextBlock> TB_Max;
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_OK;
	TWeakObjectPtr<UTextBlock> TB_SelectFullText;
	TWeakObjectPtr<UTextBlock> TB_Guide;
	TArray<TWeakObjectPtr<class UB2UISelectiveLotteryItemSlot>> Slots;

	class UB2UIMail* mailbox;
	
};
