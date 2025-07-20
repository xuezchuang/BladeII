// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2LobbyUI_AutoSell.generated.h"

#define MAX_AUTOSELL_ITEM_CATEGORY 3
#define MAX_AUTOSELL_ITEM_GRADE 6

enum class EAutoSellCategoryType : uint8
{
	WEAPON = 1,
	ARMOR = 2,
	ACCESSORY = 3,
	ETHER = 4,

	End
};

UCLASS()
class BLADEII_API UB2LobbyUI_AutoSell : public UB2UIWidget, public IB2UIBackWidget
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
	FText SetCategoryLabel(int32 index);
	FText SetGradeLabel(int32 index);

	void SaveFilterInfo();
	void LoadFilterInfo();
	void ResetFilterInfos();
	int32 GetGradeFilterInfo();
	int32 GetEtherGradeFilterInfo();

	void UpdateButonState();

protected:
	UFUNCTION()
		void OnClickCategoryCheckBox(bool bCheck);
	UFUNCTION()
		void OnClickGradeCheckBox(int32 ButtonIndex);
	UFUNCTION()
		void OnClickEtherGradeCheckBox(int32 ButtonIndex);

	UFUNCTION()
		void OnClickBTN_Cancel();
	UFUNCTION()
		void OnClickBTN_AutoSellConfirm();
	UFUNCTION()
		void OnClickBTN_Release();

private:

	TWeakObjectPtr<UTextBlock> TB_AutoSellTitle;
	TWeakObjectPtr<UB2Button> BTN_Cancel;

	/* Filter */
	TWeakObjectPtr<UTextBlock> TB_ItemCategory;
	TWeakObjectPtr<UTextBlock> TB_ItemGrade;

	/* Item */
	TMap<int32, TWeakObjectPtr<class UB2UIFilterCheckBox>> Filter_Category;
	TMap<int32, TWeakObjectPtr<class UB2UIFilterCheckBox>> Filter_Grade;
	TArray<TWeakObjectPtr<class UB2UIFilterCheckBox>> CategoryArray;
	TArray<TWeakObjectPtr<class UB2UIFilterCheckBox>> GradeArray;

	/* Ether */
	TWeakObjectPtr<class UB2UIFilterCheckBox> Filter_EtherCategory;
	TMap<int32, TWeakObjectPtr<class UB2UIFilterCheckBox>> Filter_EtherGrade;
	TArray<TWeakObjectPtr<class UB2UIFilterCheckBox>> EtherGradeArray;

	TWeakObjectPtr<UTextBlock> TB_AutoSellDesc;

	TWeakObjectPtr<UB2Button> BTN_AutoSellConfirm;
	TWeakObjectPtr<UTextBlock> TB_AutoSellConfirm;

	TWeakObjectPtr<UB2Button> BTN_Release;
	TWeakObjectPtr<UTextBlock> TB_Release;

};
