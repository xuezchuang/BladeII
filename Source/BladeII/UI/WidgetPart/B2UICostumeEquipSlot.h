// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "../../BladeII/BladeIIGameMode.h"
#include "B2UICostumeEquipSlot.generated.h"

UCLASS()
class BLADEII_API UB2UICostumeEquipSlotList : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

public:
	UFUNCTION(BlueprintCallable, Category = "B2UICostumeEquipSlotList")
	void InitCostumeEquipSlotList_BP();

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

private:
	void UpdateVisibility(const bool& bIsVisible);

private:
	TArray<issue_ptr> Issues;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


UCLASS()
class BLADEII_API UB2UICostumeEquipSlot : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "B2UICostumeEquipSlot")
	void InitCostumeEquipSlot_BP();

private:
	void InitEquipSlot();

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

private:
	void UpdateFromSelectionPCClass(EPCClass SelectionPCClass);
	void UpdateFromCurrentPCClass();
	void UpdateFromFindAccountClass(EPCClass InPCClass);

private:
	void UpdateEquipSlotFromArrayFB2Item(const TArray<FB2Item>& rArrayItem);

private:
	void UpdateIMGItemIcon(const FB2Item& InItem);
	void UpdateIMGItemGrade(const uint8& StarGrade);
	void UpdateBTNEquipSlot(const bool& IsEnable);
	void UpdateEnhanceLevelText(const int32& nEnhanceLevel);

protected:
	UFUNCTION()
	void OnClickBTN_EquipSlot();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECostumeEquipPlace eCostumeEquipPlace;
	
private:
	TWeakObjectPtr<UB2Button>						BTN_EquipSlot;

	TWeakObjectPtr<UWidgetSwitcher>					WS_EquipSlotIMG;

	TWeakObjectPtr<UB2Image>						IMG_ItemIcon;
	TWeakObjectPtr<UB2Image>						IMG_ItemGrade;

	TWeakObjectPtr<UTextBlock>						TB_EnhanceLevel;
private:
	TArray<issue_ptr> Issues;

private:
	FB2Item CachedEquipB2ItemInfo;
};
