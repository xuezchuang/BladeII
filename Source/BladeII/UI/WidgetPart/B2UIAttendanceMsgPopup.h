// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIAttendanceMsgPopup.generated.h"

DECLARE_DELEGATE(FMsgPopupOnClick)

UCLASS()
class BLADEII_API UB2UIAttendanceMsgPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetConfirmCallback(const FMsgPopupOnClick& Handler);

protected:

	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	void SetRewardItem();
	FText GetAttendanceItemName(int32 nRewardId, int32 nRewardCount);

	UFUNCTION()
	void OnClickB2Button_0();
	UFUNCTION()
	void OnClickB2Button_2();

private:
	TWeakObjectPtr<UB2Button> B2Button_0;
	TWeakObjectPtr<UB2Image> B2Image_0;
	TWeakObjectPtr<UB2Image> B2Image_2;
	TWeakObjectPtr<UB2Image> B2Image_141;
	TWeakObjectPtr<UB2Image> IMG_ITEM;
	TWeakObjectPtr<UTextBlock> TXT_ItemName;
	TWeakObjectPtr<UB2Image> B2Image_3;
	TWeakObjectPtr<UB2Button> B2Button_2;
	TWeakObjectPtr<UTextBlock> TXT_Notice;
	TWeakObjectPtr<UTextBlock> TXT_Ok;

	FMsgPopupOnClick				 ConfirmDelegate;
};
