// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "../Widget/B2Image.h"
#include "B2LobbyUI_ItemDetailSetEffect.generated.h"

USTRUCT(BlueprintType)
struct FSetItemSimpleIcon
{
	GENERATED_USTRUCT_BODY()

	FSetItemSimpleIcon() : ItemClass(EItemClass::EIC_End), ItemSimpleIconOn(nullptr), ItemSimpleIconOff(nullptr) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemClass ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ItemSimpleIconOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ItemSimpleIconOff;


};

/**
 * A tiny sub window reside in item detail view only for currently equipped protections (having set effect).
 */
UCLASS()
class BLADEII_API UB2LobbyUI_ItemDetailSetEffect : public UB2LobbyUserWidget
{
	GENERATED_BODY()

	virtual void CacheAssets() override;
	
public:

	void InitSetOptionPanel(int32 InSetUniqueKey, EPCClass InPCClass, const FB2Item& InItem);

	void UpdateSefEffectText(int32 SetOptionID);

	void UpdateSingleCompositionIcon(EItemClass InItemClass, bool bCurrentEquipItem);

	void UpdateSetEffectTextLine(int32 TextIndex, EItemOption OptionType, EPCClass InPCClass, float IncreaseValue, bool bCurrentApplying);


protected:

	TWeakObjectPtr<class UB2DynText_Multiple> GetSetEffectTextLine(int32 InSetCount);
	int32	GetSetOptionID(const struct FMD_SetInfo* InSetMasterInfo, EItemInvenType InItemInvenType);
	
	TArray<int32>	GetSetCompositionItems(const struct FMD_SetInfo* InSetMasterInfo, EItemInvenType InItemInvenType);
	bool	IsEquipSetCompisitionItem(int32 ItemTemplateID, EPCClass InPCClass);
	
	UMaterialInterface* GetSimpleIconMaterial(EItemClass InItemClass, bool bCurrentEquip);
protected:

	/** It will affect this icon's alignment when it is dynamically placed onto some panel.
	* In most cases, put the exact size of overall layout. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	FVector2D NonScaledSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemIcon")
	TArray<FSetItemSimpleIcon>	SimpleIconInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor ApplySetCountTextColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor ApplySetEffectTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII RelicUI")
	TSubclassOf<class UUserWidget> StarItem;

	TWeakObjectPtr<UTextBlock> TB_SetOptionTitle;

	TWeakObjectPtr<UOverlay> O_SetItem1;
	TWeakObjectPtr<UB2Image> IMG_SetItem1;
	TWeakObjectPtr<UOverlay> O_SetItem2;
	TWeakObjectPtr<UB2Image> IMG_SetItem2;
	TWeakObjectPtr<UOverlay> O_SetItem3;
	TWeakObjectPtr<UB2Image> IMG_SetItem3;
	TWeakObjectPtr<UOverlay> O_SetItem4;
	TWeakObjectPtr<UB2Image> IMG_SetItem4;
	
	TWeakObjectPtr<UPanelWidget> P_PreFix;
	TWeakObjectPtr<UTextBlock> TB_Prefix;

	TWeakObjectPtr<class UB2UIStarGradePart> StarGradePart;

	TArray<TWeakObjectPtr<class UB2DynText_Multiple>> UIP_ItemSetEffects;

public:
	UB2LobbyUI_ItemDetailSetEffect(const FObjectInitializer& ObjectInitializer);

	void UpdateItemData(const FB2Item& InItem);

	FORCEINLINE FVector2D GetNonScaledSize() { return NonScaledSize; }
};
