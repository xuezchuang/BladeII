// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UICostumePageStore.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BLADEII_API UB2UICostumePageStore : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;


	void RefreshPage();
#if WITH_EDITOR
	void AddDevSlot(int32 AddCount);
#endif

protected:
	class UMaterialInstance* GetProductIcon(int32 PackageID, int32 OptionalItemId);
	FText GetProductName(int32 PackageID, int32 OptionalItemId);
	FText GetProductDescription(int32 PackageID);
	
	void RemoveAllChild();

	void RequestCashPurchase(int32 ProductID);

	void OpenPackageDetailPopUp(int32 ProductID);
	void ClosePackageDetailPopup();
	void RefreshPackageDetailPopup();

	void ProcessPurchase(int32 ProductID);

	bool CheckPackageInClass(EPCClass CurrentClass, const FStoreProductData& StoreProductData);

	void FilterAndSortPackageList(EPCClass InCurrentClass, TArray<FStoreProductData> & FilterList);

	void CompletedPurchaseBuyCash(bool success, const FB2ResponseBuyShopResultInfoPtr GetProduct);

	void ConditionalCreateGuestWarningPopup();
	void DestroyGuestWarningPopup();

	void OpenSuccessBuyPopUp();
	void OpenFailBuyPopUp(bool bIsInvenFull = false);

	virtual void CacheAssets() override;


	UFUNCTION()
		void OpenGuestWarningPopup();
	UFUNCTION()
		void CloseGuestWarningPopup();
	UFUNCTION()
		void BuyGuestWarningPopup();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		int32 RowSlotCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		FVector2D SlotPadding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		float IconSizeY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TSubclassOf<class UB2UICostumeStoreItemSlot> CostumeSlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TSubclassOf<class UB2UIStoreProductIconData> ProductIconData;

	UPROPERTY(Transient)
		class UB2UIMsgPopupGuestWarning* GuestWarningPopup;
	
	UPROPERTY(Transient)
		class UB2UICostumePackageDetail* CostumePacakageDetail;

private:
	int32 StoreProductID;
	TArray<issue_ptr> Issues;
	EPCClass SelectedPCClass;

	FDelegateHandle RefreshHandler;

	TWeakObjectPtr<class UB2ScrollBoxWithGridPanel>	UIP_ScrollBoxWithGridPanelBP;
};
