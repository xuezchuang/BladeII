// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIUserReport.generated.h"

UCLASS()
class BLADEII_API UB2UIUserReport : public UB2UIWidget
{
	GENERATED_BODY()

public:
	UB2UIUserReport(const FObjectInitializer& ObjectInitializer);

    virtual void Init() override;

	UFUNCTION()
    void OnClickReport();

	UFUNCTION()
    void OnClickCancel();

	UFUNCTION()
	void EditBoxTextChange(const FText& InText);

	UFUNCTION(BlueprintCallable, Category = "BladeII BattleMain")
	void SetCheckNumber(int32 InCheckNumber, bool IsCheck);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

public:
	void UpdatePopup(int32 InModType, int64 InAccountID, FString InNickName);

private:
	void SetReason(int32 InModType);
	void SetNickName(FString InNickName);
	void UnCheckAll();

private:
	//static Text
	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_ReportTitle;
	TWeakObjectPtr<UTextBlock> TB_Cancel;
	TWeakObjectPtr<UTextBlock> TB_Report;

	TWeakObjectPtr<UTextBlock>			TB_NickName;
	TArray<TWeakObjectPtr<UTextBlock>>	TB_ReasonArr;
	TArray<TWeakObjectPtr<UCheckBox>>	CB_ReasonArr;

	TWeakObjectPtr<UEditableText>		E_EditBox;

	TWeakObjectPtr<UB2Button>				BTN_Cancel;
	TWeakObjectPtr<UB2Button>				BTN_Report;

	int64								AccountID;
	int32								ModType;
	int32								CheckNumber;

	const int32 ModDivisionNumber = 100;

    int32 ReasonFlags;
};
