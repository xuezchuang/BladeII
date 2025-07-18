// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIItemForgePopup.h"
#include "B2UIItemForgeIngredientList.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIItemForgeIngredientList : public UB2UIWidgetBase
{
	GENERATED_BODY()
	/*
private:
	FB2Item FindItemByRefID(int32 RefID);
	*/
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void UpdateStaticText() override;

	UFUNCTION()
		void OnClickBTN_Select();
	UFUNCTION()
		void OnClickBTN_Confirm();

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetOwnerItemForgeUI(class UB2UIItemForge* OwnerUI, EItemForgeSlot IngredientSlot_, int32 materialID, int32 forgeID_, EPCClass pcClass, int32 pleaseForgeThisMany, int32 FirstRequiredQuantity = 1, int32 requiredEnhanceLevel=0, int32 requiredItemLevel=0);
	
	EItemForgeSlot GetIngredientSlot() { return IngredientSlot; }

	void SetIconMaterial(UMaterialInterface* IconMaterial);
	void InitStarGrade(int32 StarGrade);
	void SetStarGrade(const int32 StarGrade, const bool IsSurpass);
	void HideStarGrade();
	void SetQuantity(bool isReady = false);

	void SetAnotherMaterialItem(FB2Item setAsThis, int32 newRequiredQuantity);
	void RecalculateQuantity(int32 thisMuchPlease, bool uiOnly=false);

	int32 GetRequiredQuantity();

	FB2Item FindItemByRefID(int32 RefID);

protected:
	class UB2UIItemForge* ItemForgeMainUI;

	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UB2UIItemForgePopup> result;

private:
	TWeakObjectPtr<UImage> IMG_SelectedFrame;
	TWeakObjectPtr<UImage> IMG_ItemIcon;
	TWeakObjectPtr<UScaleBox>	SB_StarGrade;
	TWeakObjectPtr<class UB2UIStarGradePart> StarGradePart;
	TWeakObjectPtr<UTextBlock> TB_ItemName;

	TWeakObjectPtr<UTextBlock> TB_Quantity;

	TWeakObjectPtr<UTextBlock> TB_Select;
	TWeakObjectPtr<UB2Button> BTN_Select;
	TWeakObjectPtr<UHorizontalBox> HB_ButtonBox;

	TWeakObjectPtr<UB2Button> BTN_Confirm;

protected:
	class FLocalCharacterData* CachedCharacterStore;

private:
	int32 forgeID;
	EItemForgeSlot IngredientSlot;
	FB2Item SelectedItem;
	int32 singleRequiredQuantity;
	int32 originalRequirement;
	int32 firstMaterialID;
	int32 requiredQuantity;
	EPCClass slotClass;
	int32 mainWantsThisMany;

	int32 mainItemEnhanceLevel;	//강화 레벨
	int32 mainItemLevel;		//레벨업 레벨

	bool isConsumable;
};
