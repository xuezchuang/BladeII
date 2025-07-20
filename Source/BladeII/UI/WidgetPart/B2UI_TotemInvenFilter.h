// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
////#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "../../DataStore/B2TotemData.h"
#include "B2UI_TotemInvenFilter.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UI_TotemInvenFilter : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void GetSelectedFilters(TArray<ETotemBasePrefix>& OutTotemBasePrefixFilter, TArray<ETotemGrade>& OutTotemGradeFilter, bool& IsSmelting);

public:
	void BindWarningPopup();
	bool GetAllTargetTotems(TArray<FB2Totem> &OutList);
	void SaveFilterInfo(const TCHAR* InSectionName);
	void LoadFilterInfo(const TCHAR* InSectionName);
	void ResetFilter();
	void SetFilterTitle(FText TitleText);
	void SetVisibleEssenceCheckBox(ESlateVisibility InVisibility);
	void GetFilteredItemList(TArray<FB2Totem>& OutResult, bool bSmelting, TArray<ETotemBasePrefix>& CateogryFilter, TArray<ETotemGrade>& StarGradeFilter);

private:
	FText GetGradeText(ETotemGrade TotemGrade);

protected:
	UFUNCTION()
	void FilterCBChanged(bool bIsChecked);

public:
	FOnCheckBoxComponentStateChanged OnCheckStateChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32	CategoryFiltersNumMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32	GradeFiltersNumMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32	WarningPopupStartGrade;

private:
	FText FiltetTitleText;

private:
	TWeakObjectPtr<UTextBlock>							TB_TotemCategory;
	TWeakObjectPtr<UTextBlock>							TB_TotemGrade;
	TWeakObjectPtr<UTextBlock>							TB_TotemState;

	TArray<TWeakObjectPtr<class UB2UIFilterCheckBox>>	CategoryFilters;
	TArray<TWeakObjectPtr<class UB2UIFilterCheckBox>>	GradeFilters;
	TWeakObjectPtr<class UB2UIFilterCheckBox>			SmeltingFilter;
};
