// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "EventSubsystem.h"

#include "B2UIHotTimeBuffPopup.generated.h"


USTRUCT(BlueprintType)
struct FB2HotTimeBuffUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	EHotTimeEventType EventType;

	UPROPERTY(EditAnywhere)
	EHotTimeBuffType BuffType;

	UPROPERTY(EditAnywhere)
	class UTexture2D* IconImg;

	UPROPERTY(EditAnywhere)
	FVector2D ImgSize;

	UPROPERTY(EditAnywhere)
	FString TitleLOCKey;

	UPROPERTY(EditAnywhere)
	FString DescriptionLOCKey;

	static const FB2HotTimeBuffUIData* GetData(const TArray<FB2HotTimeBuffUIData>& InBuffDatas, EHotTimeEventType InEventType, EHotTimeBuffType InBuffType);
};

/**
 *
 */
UCLASS()
class BLADEII_API UB2UIHotTimeBuffPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void CloseWidgetDelegate() override;
	virtual void UpdateStaticText() override;

	virtual void BindDelegates() override;

	void SetPartUIData(EHotTimeEventType InEventType, EHotTimeBuffType InBuffType);

public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void Init() override;

	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	void UpdateUIParts();

	UFUNCTION()
	void OnCancleBtnClick();
	UFUNCTION()
	void OnLinkBtnClick();

protected:
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TArray<FB2HotTimeBuffUIData> PartUIDatas;

	TArray<TWeakObjectPtr<class UB2UIHotTimeBuffPart>> HotTimeUIParts;

	FTimerHandle UpdateTimeHandle;

	TArray<issue_ptr> Issues;

	TWeakObjectPtr<UButton> BTN_Cancle;
	TWeakObjectPtr<UTextBlock> TB_Title;

	TWeakObjectPtr<UTextBlock> TB_HotTimePeriodTitle;
	TWeakObjectPtr<UTextBlock> TB_HotTimePeriod;
	TWeakObjectPtr<UTextBlock> TB_HotTimeEventEnd;

	TWeakObjectPtr<UButton> BTN_HotTimeLink;
	TWeakObjectPtr<UTextBlock> TB_HotTimeBuffDescription;
	TWeakObjectPtr<UTextBlock> TB_HotTimeLinkDescription;

	TWeakObjectPtr<UHorizontalBox> HB_PartBox;

	TWeakObjectPtr<class UB2UIHotTimeBuffPart> UIP_HottimeEventGold;
	TWeakObjectPtr<class UB2UIHotTimeBuffPart> UIP_HottimeEventExp;
	TWeakObjectPtr<class UB2UIHotTimeBuffPart> UIP_HottimeProductGold;
	TWeakObjectPtr<class UB2UIHotTimeBuffPart> UIP_HottimeProductExp;
};
