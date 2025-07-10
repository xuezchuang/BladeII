// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2AutoSellEther.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2AutoSellEther : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	bool GetSendEther();
	const int32 WarningPopupStartGrade = 5;
	const int32 WarningPopupTierGrade = b2network::B2AetherTierType::ETERNAL_LIFE;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void Init() override;
	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnClickSell();
	UFUNCTION()
		void OnClickReset();
	UFUNCTION()
		void OnClickCheckBox(bool bCheck);
	void GetFiliterList(TArray<int32> &OutStartFilter, TArray<int32> &OutTierFilter, bool& IsEnhance);
	void GetSendEtherList(TArray<int64>& OutSendIDs);
	void EndLoopRequest();
	void StartTimerRequestSellEther();
	void EndTimerRequestSellEther();

	FText GetEtherGradeText(int32 Grade);
	void OpenCheckWarningPopUp(FText Parameter, class UB2UIFilterCheckBox* FilterCheckBox);

	void SaveFilterInfo();
	void LoadFilterInfo();

	TWeakObjectPtr<UB2Button>									X_BTN_Close;
	TWeakObjectPtr<UB2Button>									X_BTN_Reset;

	TMap<int32, TWeakObjectPtr<class UB2UIFilterCheckBox>>		Filter_Grade;
	TMap<int32, TWeakObjectPtr<class UB2UIFilterCheckBox>>		Filter_Tier;
	TWeakObjectPtr<class UB2UIFilterCheckBox>					UIP_Enhance;
	
	TWeakObjectPtr<UB2Button>									X_BTN_Sell;
	TWeakObjectPtr<UTextBlock>									X_TB_Sell;
	TWeakObjectPtr<UTextBlock>									X_TB_Title;
	TWeakObjectPtr<UTextBlock>									X_TB_EtherSellPriceCount;
	TWeakObjectPtr<UTextBlock>									X_TB_EtherSellPriceNumber;
	TWeakObjectPtr<UTextBlock>									X_TB_Description;
	TWeakObjectPtr<UTextBlock>									X_TB_Reset;
	TWeakObjectPtr<UTextBlock>									X_TB_SelectCount;
	TWeakObjectPtr<UTextBlock>									X_TB_SelectNumber;
	TWeakObjectPtr<UTextBlock>									X_TB_Select;
	TWeakObjectPtr<UTextBlock>									X_TB_Tier;
	

	TArray<TWeakObjectPtr<class UB2UIFilterCheckBox>>			AllFilters;

	uint32														SellEtherEventID;
	uint32														SellEtherErrorEventID;
	int32														EtherSellPriceAmount;
	TArray<int64>												FilteredEtherIDs;
	int32														SendEtherSellPrice;
	FTimerHandle												LoopRequestSellEther;
	float														RequestTime;
	float														ResponseTime;
};
