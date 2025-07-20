// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIMsgPopupMagicStoreBuyConfirm.generated.h"


UCLASS()
class BLADEII_API UB2UIMsgPopupMagicStoreBuyConfirm : public UB2UIWidgetBase, public IB2UIBackWidget
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

	void SetBuyConfirmPopupData(int32 ProductID);

private:
	void SetItemIcon(int32 ItemRefID);

private:
	TWeakObjectPtr<UTextBlock> TB_Title;

	TWeakObjectPtr<UImage> IMG_ItemIcon;
	TWeakObjectPtr<UB2RichTextBlock> TB_Content;
	TWeakObjectPtr<class UB2UIStoreProductCost> UIP_Cost;

	TWeakObjectPtr<UTextBlock> TB_Buy;
};
