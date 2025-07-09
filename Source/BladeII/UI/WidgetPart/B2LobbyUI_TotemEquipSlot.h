// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2LobbyUI_TotemEquipSlot.generated.h"

/**
 * 
 */

///////////////////////////////////////////////////////////////////////////////////////
//
// UB2LobbyUI_TotemEquipSlotList
//
///////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class BLADEII_API UB2LobbyUI_TotemEquipSlotList : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxEquipSlot;

private:
	TArray<TWeakObjectPtr<class UB2LobbyUI_TotemEquipSlot>>		UIP_TotemEquipSlots;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// UB2LobbyUI_TotemEquipSlot
//
///////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class BLADEII_API UB2LobbyUI_TotemEquipSlot : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	
private:
	void InitTotemEquipSlot();

	void SubscribeEvents();
	void UnsubscribeEvents();

private:
	void UpdateFromSelectionPCClass(EPCClass SelectionPCClass);
	void UpdateFromCurrentPCClass();

	void UpdateSlotState(ETotemEquipSlotState InEquipSlotState);
	void UpdateIMGTotemIcon(const FB2Totem& InTotem);
	void UpdateIMGTotemGrade(const ETotemGrade& TotemGrade);
	void UpdateIMGTotemSmeltingLevel(const uint8& SmeltingLevel);
	void UpdateBTNEquipSlot(const bool& IsEnable);

	void UpdateEquipSlotFromArrayFB2Totem(const TArray<FB2Totem>& rArrayTotem);

protected:
	UFUNCTION()
	void OnClickBTN_EquipSlot();

protected:
	// TotemEquipPlace 로 변경 필요
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETotemEquipSlot TotemEquipPlace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETotemEquipSlotState EquipSlotState;

private:
	TWeakObjectPtr<UB2Button>						BTN_EquipSlot;

	TWeakObjectPtr<UWidgetSwitcher>					WS_EquipSlotIMG;

	TWeakObjectPtr<UB2Image>						IMG_TotemIcon;
	TWeakObjectPtr<UB2Image>						IMG_TotemGrade;

	TWeakObjectPtr<UTextBlock>						TB_SmeltingLevel;
private:
	TArray<issue_ptr> Issues;

private:
	FB2Totem CachedEquipB2TotemInfo;
};
