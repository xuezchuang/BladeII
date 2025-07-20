// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIAutoSelectedItemParts.generated.h"

UENUM()
enum class EEnable_Touch : uint8
{
	NONE,
	ENABLE,
	DISABLE_OVERCOUNT,
	DISABLE_MAXLEVEL,
	DISABLE_HIGHRANK,
};

UCLASS()
class BLADEII_API UB2UIAutoSelectedItemParts : public UB2UIWidget
{
	GENERATED_BODY()

private:
	EEnable_Touch IsEnableClick();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	int32 GetItemGrade() { return ItemGrade;  }
	bool IsAutoSelected() const { return IsSelected; }
	bool IsEnableAddItem() const { return CurrentItemCount < MaxItemCount; }

	void SetData(int32 StartCount, int32 ItemCount, int32 InMaxItemCount);
	void SetUIVisibility(bool InSelected);
	void SelectPart();

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UFUNCTION()
	void OnClickBTN_Selected();

private:
	static const int32 HighRankGradeWarning = 7;
	TWeakObjectPtr<UTextBlock> TB_StarCount;
	TWeakObjectPtr<UTextBlock> TB_StarCount_UnSelect;
	TWeakObjectPtr<UTextBlock> TB_ItemCount;
	TWeakObjectPtr<UTextBlock> TB_ItemCount_UnSelect;
	TWeakObjectPtr<UTextBlock> TB_MaxItemCount;

	TWeakObjectPtr<UB2Button> BTN_Selected;
	TWeakObjectPtr<UImage>    IMG_Selected;
	TWeakObjectPtr<UImage>    IMG_SelectedCheck;

	int32 ItemGrade;
	int32 MaxItemCount;
	int32 CurrentItemCount;
	bool  IsSelected;
};
