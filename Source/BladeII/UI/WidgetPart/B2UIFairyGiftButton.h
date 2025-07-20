// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIFairyGiftButton.generated.h"

DECLARE_DELEGATE_OneParam(FFairyGiftButtonOnClick, EFairyType)

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFairyGiftButton : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	void SetFairyGiftBtnHandler(const FFairyGiftButtonOnClick& InBtnHandler);

	void SetSelectState(bool bSelected);
	const bool GetIsSelected() { return IsSelected; }

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	
	void UpdateFairyImage();

	UFUNCTION()
		void OnClickBTN_Fairy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		EFairyType								FairyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
		TSoftObjectPtr<class UMaterialInterface>		FairyImage;

private:
	FFairyGiftButtonOnClick			OnClickBtnHandler;

	bool							IsSelected;

	TWeakObjectPtr<UImage>			IMG_On;
	TWeakObjectPtr<UImage>			IMG_Off;

	TWeakObjectPtr<UB2Button>		BTN_Fairy;
	TWeakObjectPtr<UWidgetSwitcher>	WS_Image;
};
