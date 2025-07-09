// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "CommonStruct.h"
#include "B2UIResurrectBuffIcon.generated.h"

// To propagate resurrect buff icon click event to its owning widget
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnResurrectBuffIconClickedDelgate, EResurrectBuffType, SelectedBuffType);

USTRUCT(BlueprintType)
struct FResurrectBuffIconPerTypeSetup
{
	GENERATED_USTRUCT_BODY()

	FResurrectBuffIconPerTypeSetup()
	{
		StatTextColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0));
		IconMaterial = NULL;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FSlateColor StatTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	UMaterialInterface* IconMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	UMaterialInterface* IconMaterial_NonSelected;
};

/*
 * The icon displaying resurrect buff and providing selection button as well for defeat menu 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIResurrectBuffIcon : public UB2UIWidgetBase
{
	GENERATED_BODY()

	EResurrectBuffType MyBuffType;
	int32 MyBuffAmount; // Perhaps in percent unit. 

	uint32 bIsSelected : 1;

protected:

	// We provide settings per buff type here, to enable setting up for all types of buff by using just a single widget bp.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FResurrectBuffIconPerTypeSetup SetupForAttackBuff;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FResurrectBuffIconPerTypeSetup SetupForDefenseBuff;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FResurrectBuffIconPerTypeSetup SetupForHeathBuff;


	TWeakObjectPtr<UB2Button> BTN_Area;

	TWeakObjectPtr<UImage> IMG_MainIcon; // This to be dynamically set.
	TWeakObjectPtr<UImage> IMG_Selected; // The image showing selected state.

	TWeakObjectPtr<UTextBlock> TB_BuffTypeLabel;
	TWeakObjectPtr<UTextBlock> TB_BuffAmount;

public:

	UB2UIResurrectBuffIcon(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	
	// To propagate resurrect buff icon click event to its owning widget
	FOnResurrectBuffIconClickedDelgate OnSelectedOuterDelegate;

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	void SetBuffInfo(EResurrectBuffType InBuffType, int32 InBuffAmount);

	void SetSelected(bool bSelect);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnPlaySelectAnimation_IMP(bool bIsPlay);

protected:
	void UpdateWidgets();

	UFUNCTION()
	void OnClickBtnArea();

	FORCEINLINE EResurrectBuffType GetBuffType() { return MyBuffType; }
	FORCEINLINE bool IsSelected() { return bIsSelected; }
};
