// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "B2Inventory.h"
#include "B2DynItemIcon.generated.h"

#define MAX_STARGRADE_IMAGE FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE
#define OVER_STAR_COUNT 7

/**
 * To be dynamically created and deployed onto other widget panel, displaying an item information.
 */
UCLASS()
class BLADEII_API UB2DynItemIcon : public UBladeIIUserWidget
{
	GENERATED_BODY()
	
protected:


	/** It will affect this icon's alignment when it is dynamically placed onto some panel. 
	 * In most cases, put the exact size of overall layout. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	FVector2D NonScaledSize;

	/** Copied item data. The main stuff to be displayed through this item icon widget. */
	FB2Item NativeItemData;
	
	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* BackgroundImageNRef;

	UPROPERTY(Transient, BlueprintReadWrite)
	UImage* MainIconImageNRef;

	UPROPERTY(Transient, BlueprintReadWrite)
	UTextBlock* ItemNameTextBlockNRef;

	/** Image is prepared per each star grade. */
	TWeakObjectPtr<UImage> IMG_ExtraSlotEffect;

	TWeakObjectPtr<class UB2UIStarGradePart> StarGradePart;

public:
	UB2DynItemIcon(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void CacheAssets() override;

	/** Update the main data container here. It also updates ItemInfo data container according to the ItemRefID. */
	virtual void UpdateItemData(const FB2Item& InItem);

	virtual void SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables);

	/** Not only for displaying the star grade itself, but also for some other display depend on the star level. */
	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 GetItemStarGrade() const;

	UImage* GetStarGradeImagNRef(int32 StarIndex);
	
	/** For some Blueprint bound data */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void UpdateItemData_BP(int32 StarGrade, int32 ItemQuality, bool bIsSurpassed);

	FORCEINLINE FVector2D GetNonScaledSize() { return NonScaledSize; }
		
	/** Returns currently displaying item's instance UID. */
	FORCEINLINE int64 GetItemInstanceUID() { return NativeItemData.InstanceUID; }

public:
	/** Common utility to pop-up a item detail window in lobby.
	 * @param bIsForEquippedItem : Create from equipped item if true, from stored (inventory) item if false. */
	static class UB2LobbyUI_ItemDetail* CreateLobbyItemDetailPopupCommon(UObject* WorldContextObject, FB2Item& InItemData, bool bIsForEquippedItem);
};