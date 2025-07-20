// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIItemChangePopupMode.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FItemChangePopupMode)


UENUM(BlueprintType)
enum class EUIItemChangePopupMode : uint8
{
	Option,
	Hero,
	Enhance,
	Quality
};

UCLASS()
class BLADEII_API UB2UIItemChangePopupMode : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
public:

	void AddHandler(EUIItemChangePopupMode ButtonType, const FItemChangePopupMode& Handler);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void CacheAssets() override;

	/*Allows setting the content of the text does not change here.*/
	virtual void UpdateStaticText() override;

	/* Where to bind a DelegateFunction to cached assets. */
	virtual void BindDelegates() override;


	void ItemSlotSetting(const FB2Item& ItemInfo);

	virtual void CloseWidgetDelegate() override;


	UFUNCTION(BlueprintCallable)
		void OnClickBtnOption();
	UFUNCTION(BlueprintCallable)
		void OnClickBtnEnhance();
	UFUNCTION(BlueprintCallable)
		void OnClickBtnHero();
	UFUNCTION(BlueprintCallable)
		void OnClickBtnQuality();

	UFUNCTION(BlueprintCallable)
		void OnClickDestorySelf();

	UFUNCTION(BlueprintImplementableEvent)
		void OptionButtonHiddenEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void HeroButtonHiddenEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void EnhanceButtonHiddenEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void QualityButtonHiddenEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void EnableQualityChangeBTN();
	UFUNCTION(BlueprintImplementableEvent)
		void DisableQualityChangeBTN();
		

private:
	TMap<EUIItemChangePopupMode, FItemChangePopupMode> ClickHandlerMap;
};
