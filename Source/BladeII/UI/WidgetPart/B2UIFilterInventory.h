// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIFilterInventory.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFilterInventory : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIFilterInventory(const FObjectInitializer& ObjectInitializer);

	FOnCheckBoxComponentStateChanged OnCheckStateChanged;
	const int32 WarningPopupStartGrade = 5;

	UPROPERTY(EditInstanceOnly, Category = FilterCount)
	int32 VisibleMaxStarGrade = 12;

	UPROPERTY(EditInstanceOnly, Category = FilterCount)
	int32 MinGradeFilter = 1;

private:
	TWeakObjectPtr<UTextBlock> TB_ItemCategory;
	TWeakObjectPtr<UTextBlock> TB_ItemGrade;
	TWeakObjectPtr<UTextBlock> TB_Etc;

	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_Weapon;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_Protection;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_Accessory;

	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_LVUp;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_Enhance;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_Essence;

	TArray<TWeakObjectPtr<class UB2UIFilterCheckBox>> GradeFilters;

	TArray<TWeakObjectPtr<class UB2UIFilterCheckBox>> AllFilterCheckBox;

	FText FiltetTitleText;
public:
	void BindWarningPopup();
	void SetMinGradeFilter();
	bool GetAllTargetItems(TArray<FB2Item> &OutList, EPCClass PCClass = EPCClass::EPC_End);
	void SaveFilterInfo(const TCHAR* InSectionName);
	void LoadFilterInfo(const TCHAR* InSectionName);
	void ResetFilter();
	void SetFilterTitle(FText TitleText);
	void SetVisibleEssenceCheckBox(ESlateVisibility InVisibility);
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	
	void GetSelectedFilters(TArray<EItemInvenType>& OutInvenTypeFilter, TArray<int32>& OutStarGradeFilter, bool & IsLVUp, bool & IsEnhance, bool & IsEssence, bool & IsAnvil);
	
	UFUNCTION()
	void FilterCBChanged(bool bIsChecked);
};
