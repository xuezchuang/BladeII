// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIStore.h"
#include "B2UIEventAttendancePakgeSlot.generated.h"

UCLASS()
class BLADEII_API UB2UIEventAttendancePakgeSlot : public UB2UIWidget
{
	GENERATED_BODY()
		
public:
	virtual void Init() override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

public:
	void InitSlotState();
	void UpdateFromPackageID(int32 PackageID);
	void UpdatePackageNameAndIcon(FText ftPackageName, UMaterialInterface* pPackageIcon);
	void UpdateSlotState(bool IsBought);

	UFUNCTION()
	void OnClickBtnGotoPackageShop();

public:
	const int32 GetPackageID() { return CachedPackageID; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UIStoreProductIconData>	StoreProductData;

	//this value set from UIEventAttendance widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPACKAGE_TYPE						PackageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInterface*					PackageIcon;

private:
	TWeakObjectPtr<UB2RichTextBlock>	RTB_PackageName;
	TWeakObjectPtr<UImage>				IMG_PackageIcon;
	TWeakObjectPtr<UWidgetSwitcher>		WS_StateBuy;
	TWeakObjectPtr<UB2Button>			BTN_GotoPackageShop;

	//possiblebuy
	TWeakObjectPtr<UB2RichTextBlock>	RTB_PossibleBuy;

	//completebuy
	TWeakObjectPtr<UB2RichTextBlock>	RTB_CompleteBuy;
	TWeakObjectPtr<UImage>				IMG_CompleteBuyEffect;

private:
	int32								CachedPackageID;
};
