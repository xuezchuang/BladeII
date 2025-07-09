// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIStore.h"
#include "B2UIMiniStorePopup.generated.h"


UCLASS()
class BLADEII_API UB2UIMiniStorePopup : public UB2UIWidget
{
	GENERATED_BODY()

protected:

	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:

	UB2UIMiniStorePopup(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SubscribeEvents();
	void UnsubscribeEvents();

	void SetProductData();

protected:

	UFUNCTION()
		void SubmitConfirmToBuyGem();
	UFUNCTION()
		void CloseConfirmToBuyPopupGemPopup();

	UFUNCTION()
		void OpenGuestWarningPopup();
	UFUNCTION()
		void CloseGuestWarningPopup();
	UFUNCTION()
		void BuyGuestWarningPopup();

	UFUNCTION()
		void OnClickBTN_Close();

private:

	void SetProductListItem(const FStoreProductData* ProductItem);
	void OpenPopupConfirmToBuyItem(int32 ProductId);
	void ConditionalCreateConfirmToBuyPopup_Gem();
	void SetBonusData_ConfirmBuy(bool bShowBonus, EStoreItemType BonusItemType, int32 InItemCount, int32 InBonusCount);
	void CompletedPurchaseBuyGem(bool success, const FB2ResponseBuyShopResultInfoPtr GetProduct);

	void DestroyConfirmToBuyPopup_Gem();
	void ConfirmToBuyGem();

	void ConditionalCreateGuestWarningPopup();

	ESTORE_SLOT_WIDGET_TYPE GetProductWidget(int32 ProductId);
	UMaterialInstance* GetProductIcon(int32 ProductId, int32 OptionalItemId);
	ESTORE_SLOT_EFFECT_TYPE GetProductIconEffect(int32 ProductId);
	FText GetProductName(int32 ProductId, int32 OptionalItemId);
	void GetBonusIconAndName(EStoreItemType BonusType, UMaterialInstance*& OutBonusIcon, FText& OutBonusName, int32 OptionalItemTemplateId);

public:

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<UB2UIStoreItemSlot> DefaultItemSlot;

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<UB2UIStoreProductIconData> ProductIconData;

	UPROPERTY(Transient)
		class UB2UIBackWidgetBase* Created_UIP_ConfirmToBuyPopup_Gem;
	UPROPERTY(Transient)
		UB2UIMsgPopupGuestWarning* Created_UIP_GuestWarningPopup;

	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIBackWidgetBase> UIP_ConfirmToBuyPopup_Gem_Class;
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<UB2UIMsgPopupGuestWarning> UIP_GuestWarningPopup_Class;

private:

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UHorizontalBox> HB_ItemList;
	TWeakObjectPtr<UB2Button> BTN_Close;

	TWeakObjectPtr<UCanvasPanel> X_CP_ConfirmToBuyPopup_Gem_Panel;
	TWeakObjectPtr<UCanvasPanel> X_CP_GuestWarningPopup_Panel;

	TArray<TWeakObjectPtr<UB2UIWidgetBase>> ItemSlots;

private:

	TArray<std::shared_ptr<issue>> Issues;
	int32 SelectedProductIdToBuy;

};
