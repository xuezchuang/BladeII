// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDailyNoticePopup.generated.h"


UCLASS()
class BLADEII_API UB2UIDailyNoticePopup : public UB2UIWidget, public IB2UIBackWidget
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

private:
	void SetDailyNoticeList();

protected:
	UFUNCTION()
		void OnClickBTN_Cancel();

public:
	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf <class UB2UIDailyNoticeSlot > DailyNoticeSlot;

private:
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_Desc;

	TWeakObjectPtr<UVerticalBox> VB_DailyNoticeList;

	TWeakObjectPtr<UB2Button> BTN_Cancel;

private:
	TArray<TWeakObjectPtr<class UB2UIDailyNoticeSlot>> ItemSlots;

};
