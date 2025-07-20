// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2DynItemIcon.h"
#include "EventSubsystem.h"
#include "B2DynItemIcon_LobbyEquip.generated.h"

DECLARE_DELEGATE(FOnClickDetailInfo);

enum class ELobbyEquipItemIconType : uint8
{
	EEIIT_None,
	EEIIT_LobbyInven,
	EEIIT_AutoEquipCurrent, // Current equipped for auto equip window.
	EEIIT_AutoEquipSuggest, // Equip suggestion for auto equip window.
};

/**
 * Dynamically created for all equipped items, in lobby inventory page.
 */
UCLASS()
class BLADEII_API UB2DynItemIcon_LobbyEquip : public UB2DynItemIcon
{
	GENERATED_BODY()

	/** Some displayed information and features depend on it. */
	ELobbyEquipItemIconType EquipItemIconType;

protected:
	
	/** Only for AutoEquipSuggest type */
	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* IMG_StatInc_NRef;

	/** Only for AutoEquipSuggest type */
	UPROPERTY(Transient, BlueprintReadWrite)
	UTextBlock* TB_StatIncAmount_NRef;

	// Anything added later use this TWeakObjectPtr and GET_SLOT stuff.

	TWeakObjectPtr<UTextBlock> TB_ItemLevel;
	TWeakObjectPtr<UTextBlock> TB_EnhanceLevel;
	TWeakObjectPtr<UTextBlock> TB_StartGrade;
	TWeakObjectPtr<UImage> IMG_GradeBG;
	TWeakObjectPtr<UImage>	IMG_SelectFx;
	TWeakObjectPtr<UImage> IMG_Start;

public:

	UB2DynItemIcon_LobbyEquip(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void UpdateItemData(const FB2Item& InItem) override;

	virtual void SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables) override;

	virtual void DestroySelf() override;

	/** For EEIIT_AutoEquipSuggest, update text and stat inc images to show how much stat will increase by equipping the item that this icon represents.
	 * You need to send currently equipped item at the same place to CurrenEquipToCompare */
	void UpdateStatIncSigns(FB2Item& CurrenEquipToCompare);

	/** Better set before call UpdateItemData */
	void SetEquipItemIconType(ELobbyEquipItemIconType InType);

	FOnClickDetailInfo OnClickDetailInfo;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnDetailInfoButtonPressed();

	UFUNCTION(BlueprintImplementableEvent, Category = "B2DynItemIcon_LobbyEquip")
	void PlayChangeAnimEquipSlotOrCostumeSlot_BP(bool bIEquipSlotVisible);

private:
	void UpdateItemEquipSlotVisible(const bool& bIsVisibleCostumeSlot);

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

private:
	TArray<std::shared_ptr<issue>> Issues;
};
