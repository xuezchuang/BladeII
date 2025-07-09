// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2LobbySceneManager.h"
#include "B2UISingleModSelect.generated.h"

//DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSingleModSelectClickDelgate, UB2UISingleModSelect*, ClickedMod);
DECLARE_DYNAMIC_DELEGATE(FOnSingleModSelectClickDelgate);

/*
 * Single placed widget (as button) for ModSelect UI page.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISingleModSelect : public UB2UIWidgetBase
{
	GENERATED_BODY()

	// Setup here depend on this info.
	ELobbyModSceneElem MyMod;

	// Whether the user fulfilled certain condition to play this mod..
	uint32 bIsLocked : 1;

public:
	
	UB2UISingleModSelect(const FObjectInitializer& ObjectInitializer);

		
protected:

	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	//=================================================== Btn
	UFUNCTION()
	virtual void OnClickBtnArea();

public:
	
	FOnSingleModSelectClickDelgate OnBtnAreaClickDelegate;

	void SetMyModInfo(ELobbyModSceneElem InModEnum, bool bInLocked);

	FText GetModNameText(ELobbyModSceneElem InModEnum);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayButtonSound();

	bool bPressed;
	double TimeInputStarted;

protected:

	TWeakObjectPtr<UB2Button> BTN_Area;
	TWeakObjectPtr<UTextBlock> TB_ModName;

	// It can be changed..
	TWeakObjectPtr<UImage> IMG_ModMain;
	//TWeakObjectPtr<UImage> IMG_ModLocked;


	UPROPERTY(EditAnywhere, Category = "BladeII SingleModSelect")
	UMaterialInterface* ModImageMaterial;

	UPROPERTY(EditAnywhere, Category = "BladeII SingleModSelect")
	UMaterialInterface* LockedImageMaterial;
};
