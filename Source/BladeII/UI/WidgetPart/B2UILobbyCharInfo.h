// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UILobbyCharInfo.generated.h"


#define DECLARE_ITEM_SLOT(Name)					\
	TWeakObjectPtr<UB2Button> BTN_##Name;		\
	TWeakObjectPtr<UImage> IMG_Selected_##Name;	\
	TWeakObjectPtr<UTextBlock> TB_##Name;

UCLASS()
class BLADEII_API UB2UILobbyCharInfo : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2DynItemIcon_LobbyEquip> EquipItemIconWidgetClass;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;

	void SetInvetorySlot();

	void OpenCharInfo(EPCClass PCClassToShow);

	void CloseCharInfo();

	UB2Button* GetBGButton();

protected:
	UFUNCTION()
	void OnClickBTN_Helmet();
	UFUNCTION()
	void OnClickBTN_BodyArmor();
	UFUNCTION()
	void OnClickBTN_Glove();
	UFUNCTION()
	void OnClickBTN_Shoes();
	UFUNCTION()
	void OnClickBTN_Weapon();
	UFUNCTION()
	void OnClickBTN_Belt();
	UFUNCTION()
	void OnClickBTN_Necklace();
	UFUNCTION()
	void OnClickBTN_Ring();

	UFUNCTION(BlueprintImplementableEvent)
		void OnItemClickedAnimStart(EItemEquipPlace EquipPlace);

	UFUNCTION()
	void OnClickBTN_ToggleCostume();
	UFUNCTION()
	void OnClickBTN_DetailStat();
	UFUNCTION()
	void ClosePopupItemInfo();

	UFUNCTION(BlueprintImplementableEvent)
		void OnOpenAnim();

	UFUNCTION(BlueprintImplementableEvent)
		void OnCloseAnim();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void OnFinishedCloseAnim();

	void ShowPopupItemInfo(const FB2Item& ItemToShow);
	void ShowPopupCharStat();
	void ClosePopupCharStat();

	void SetShowCostume(bool bToSet);

private:
	TWeakObjectPtr<UB2Button> BTN_BG;

	DECLARE_ITEM_SLOT(Helmet);
	DECLARE_ITEM_SLOT(BodyArmor);
	DECLARE_ITEM_SLOT(Glove);
	DECLARE_ITEM_SLOT(Shoes);
	DECLARE_ITEM_SLOT(Weapon);
	DECLARE_ITEM_SLOT(Belt);
	DECLARE_ITEM_SLOT(Necklace);
	DECLARE_ITEM_SLOT(Ring);
	
	TWeakObjectPtr<UTextBlock> TB_CharName;
	TWeakObjectPtr<UTextBlock> TB_CharLevel;
	TWeakObjectPtr<UTextBlock> TB_CharExp;
	TWeakObjectPtr<UImage> IMG_CharTitle;
	TWeakObjectPtr<UTextBlock> TB_CharTitle;
	TWeakObjectPtr<UProgressBar> PB_CharExp;
	TWeakObjectPtr<UTextBlock> TB_Static_HP;
	TWeakObjectPtr<UTextBlock> TB_HP;
	TWeakObjectPtr<UTextBlock> TB_Static_ATK;
	TWeakObjectPtr<UTextBlock> TB_ATK;
	TWeakObjectPtr<UTextBlock> TB_Static_DEF;
	TWeakObjectPtr<UTextBlock> TB_DEF;
	TWeakObjectPtr<UB2Button> BTN_ToggleCostume;
	TWeakObjectPtr<UB2Button> BTN_DetailStat;

	TWeakObjectPtr<UB2Button> BTN_ItemDetailBG;
	TWeakObjectPtr<UB2Button> BTN_CharDetailBG;
	
	TWeakObjectPtr<class UB2LobbyUI_ItemDetailPostOp> UIP_ItemDetail;
	TWeakObjectPtr<class UB2LobbyUI_CharStatDetail> UIP_CharDetail;

	TWeakObjectPtr<UTextBlock> TB_ToggleCostume;

	EPCClass CurrentPCClass;
	TMap<EItemEquipPlace, FB2Item> EquippedItems;
	TMap<EItemEquipPlace, UCanvasPanel*> ItemCanvasPanels;
	TMap<EItemEquipPlace, UImage*> SelectedImages;
	TMap<EItemEquipPlace, UTextBlock*> SlotEquipNames;

	TMap<EItemEquipPlace, class UB2DynItemIcon_LobbyEquip*> DynIcons;

	bool bShowCostume;

	EItemEquipPlace CurrentViewEquipPlace;
};

#undef DECLARE_ITEM_SLOT