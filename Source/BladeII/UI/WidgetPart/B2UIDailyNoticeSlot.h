// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIDailyNoticeSlot.generated.h"


UCLASS()
class BLADEII_API UB2UIDailyNoticeSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetDailyNoticeData(int32 PackageID);
	void SetMoveButton(bool bIsMove);
	void SetPeriodicPackageLeftTime(int64 EndTime);

private:
	class UMaterialInstance* GetProductIcon(int32 PackageID);
	FText GetProductName(int32 PackageID);

protected:
	UFUNCTION()
		void OnClickBTN_GotoShop();

protected:
	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UIStoreProductIconData> ProductIconData;

private:
	TWeakObjectPtr<UWidgetSwitcher> WS_Active;
	TWeakObjectPtr<UB2Image> IMG_DailyNotice;
	TWeakObjectPtr<UTextBlock> TB_DailyNoticeName;

	TWeakObjectPtr<UWidgetSwitcher> WS_GotoShop;
	TWeakObjectPtr<UB2Button> BTN_GotoShop;
	TWeakObjectPtr<UTextBlock> TB_GotoShop;
	TWeakObjectPtr<UTextBlock> TB_RemainTime;

private:
	int32 CurrentPackageID;

};
