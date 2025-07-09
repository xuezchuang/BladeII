// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonStruct.h"
#include "UI/MsgPopup/B2UIMsgPopupBase.h"
#include "B2UIMsgPopupBuyHotTime.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupBuyHotTime : public UB2UIMsgPopupBase
{
	GENERATED_BODY()

public:
	UB2UIMsgPopupBuyHotTime(const FObjectInitializer& ObjectInitializer);

	void SetHotTimeBuffType(EHotTimeBuffType InBuffType);
	void SetHotTimeImage(class UTexture2D* IconImg, FVector2D ImgSize);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void OnClickBtnBuy();

	UFUNCTION()
		void OnClickBtnClose();
	
protected:
	EHotTimeBuffType			BuffType;

	TWeakObjectPtr<UButton>		BTN_Close;
	TWeakObjectPtr<UButton>		BTN_Buy;
	TWeakObjectPtr<UTextBlock>	TB_Buy;
	TWeakObjectPtr<UTextBlock>	TB_Price;
	TWeakObjectPtr<UB2Image>	IMG_BuffImage;
	TWeakObjectPtr<UB2Image>	IMG_GoodsImage;
	
};
