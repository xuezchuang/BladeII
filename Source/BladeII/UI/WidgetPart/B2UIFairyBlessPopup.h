// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIFairyBlessPopup.generated.h"


struct FB2FairyBlessRowInfo
{
	FText ConditionLevel;
	FText PreviewStat;
	FText PreviewStatValue;
};

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFairyBlessPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetBlessInfo(EFairyType FairyType, int32 OptionId);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	void SetActivatedBlessInfo(EFairyType FairyType, int32 OptionId);
	void SetPreviewStats(TArray<FB2FairyBlessRowInfo> InfoList);

private:
	UFUNCTION()
	void OnClickBTN_Close();

	UFUNCTION()
	void OnClickBTN_BlessIcon();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2UIFairyLevelUpStatRow> PartClass_StatRow;
	
private:
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_BelssInfo;
	
	TWeakObjectPtr<UImage> IMG_BelssIcon;	TWeakObjectPtr<UTextBlock> TB_Notice;
	TWeakObjectPtr<UB2RichTextBlock> TB_BelssName;
	TWeakObjectPtr<UTextBlock> TB_NextInfo;
	TWeakObjectPtr<UB2ScrollBox> ScrollBox;
};
