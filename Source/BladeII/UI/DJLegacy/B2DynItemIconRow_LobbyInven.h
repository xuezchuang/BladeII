// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2LobbyInventory.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2DynItemIconRow_LobbyInven.generated.h"

namespace DynItemIconRowMaxCount
{
	const int32 DefaultInven = 4;		// 牢亥配府俊辑 荤侩登绰 扁夯 浇吩 肮荐
	const int32 AutoLevelupResult = 6;	// 犁丰 饭骇诀俊 荤侩登绰 浇吩 肮荐
}

/**
 * It is supposed to be a single row of inventory item panel scroll box, containing certain number of item icons (UB2DynItemIcon_LobbyInven)
 */
UCLASS()
class BLADEII_API UB2DynItemIconRow_LobbyInven : public UB2LobbyUserWidget
{
	GENERATED_BODY()
	
protected:

	/** The main canvas panel here, where dynamically created item icons are placed. */
	UPROPERTY(Transient, BlueprintReadWrite)
	class UCanvasPanel* MainPanelNativeRef;

	/** Horizontal margin between items. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot")
	float ItemPlacementMargin;


	/** Dynamically created item icons. */
	UPROPERTY(Transient)
	TArray<class UB2DynItemIcon_LobbyInven*> CreatedItemIcons;

private:
	int32 InventoryRowSlotCount;

public:

	UB2DynItemIconRow_LobbyInven(const FObjectInitializer& ObjectInitializer);
	
	/** Create and place item icons using information in AllItemsArray. 
	 * AllItemsArray contains all items for current context. It will create icons from StartIndex ~ StartIndex + InventoryRowSlotCount. */
	void UpdateItemIcons(const TArray<FB2Item>& AllItemsArray, int32 InStartIndex, class UB2LobbyUI_InventoryMain* ContainingInvenPage);
	
	void OverrideItemIconUsage(ELobbyInvenItemIconUsage InNewUsage);

	/** It will return the allowed size by the main panel size, not the designed size of ItemIconWidgetClass */
	FVector2D GetAllowedSingleItemIconSize();

	/** Returns a valid reference of item icon, if it contains an item icon of designated instance UID. */
	class UB2DynItemIcon_LobbyInven* FindSingleItemIcon(int64 ItemInstanceUID);
	class UB2DynItemIcon_LobbyInven* FindSingleItemIconFromIndex(int32 iIndex);

	void GetAllCreatedIcons(TArray<class UB2DynItemIcon_LobbyInven*>& OutIcons);

	void DestroyItemIcons();

	virtual void DestroySelf() override;

	void SetInventorySlotMaxCount(const int32 MaxCount);

	UFUNCTION()
	void PlayEnhanceResultAnims();
};
