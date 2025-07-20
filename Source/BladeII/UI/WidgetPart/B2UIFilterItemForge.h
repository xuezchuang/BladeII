// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIFilterCheckBox.h"
#include "B2UIFilterItemForge.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFilterItemForge : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIFilterItemForge(const FObjectInitializer& ObjectInitializer);

	FOnCheckBoxComponentStateChanged OnCheckStateChanged;

	UPROPERTY(EditInstanceOnly, Category = FilterCount)
		int32 VisibleMaxStarGrade = 12;

	UPROPERTY(EditInstanceOnly, Category = FilterCount)
		int32 MinGradeFilter = 1;	//0篮 傈眉 急琶

	int32 ApplyAllAtStarGrade = 1;

private:
	TWeakObjectPtr<UTextBlock> TB_ItemCategory;
	TWeakObjectPtr<UTextBlock> TB_ItemGrade;

	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_All;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_Weapon;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_Protection;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_Accessory;
	TWeakObjectPtr<class UB2UIFilterCheckBox> UIP_Item;

	TArray<TWeakObjectPtr<class UB2UIFilterCheckBox>> GradeFilters;
	TArray<TWeakObjectPtr<class UB2UIFilterCheckBox>> AllFilterCheckBox;

	FText FilterTitleText;

	TArray<EItemInvenType> PastInvenTypeFilter;
	TArray<int32> PastStarGradeFilter;
		
public:
	void InitData();
	void SetMinGradeFilter();
	void SaveFilterInfo(const TCHAR* InSectionName);
	void LoadFilterInfo(const TCHAR* InSectionName);
	void ResetFilter();
	void SetFilterTitle(FText TitleText);

	void GetSelectedFilters(TArray<EItemInvenType>& OutInvenTypeFilter, TArray<int32>& OutStarGradeFilter,bool checkAllBox=false);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void AdjustCheckboxes();
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;


	UFUNCTION()
		void FilterCBChanged(bool bIsChecked);	
};
