// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "B2UIMsgPopupMagicStoreRefresh.generated.h"


UCLASS()
class BLADEII_API UB2UIMsgPopupMagicStoreRefresh : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void OpenPopup();
	bool IsOpened();
	void ClosePopup();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void OnPlaySuccessSound(bool bSuccess);
	
private:
	void SetRenewalCount();

private:
	TWeakObjectPtr<UTextBlock> TB_Title;

	TWeakObjectPtr<UWidgetSwitcher> WS_RemainCount;
	TWeakObjectPtr<UTextBlock> TB_RefreshDesc;
	TWeakObjectPtr<UTextBlock> TB_RefreshCount;
	TWeakObjectPtr<UTextBlock> TB_Cost;
	TWeakObjectPtr<UTextBlock> TB_EndRefresh;

	TWeakObjectPtr<UTextBlock> TB_Buy;
};
