// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UICommonComboBoxSlot.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICommonComboBoxSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetMainText(FText InText);
protected:
	UFUNCTION()
	void OnClickBTN_Select();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII ComboBox")
	TArray<UMaterialInterface*>	ComboBoxTexture;		// 0 : Normal, 1 : Hovered, 2:Pressed, 3:Disabled

private:
	TWeakObjectPtr<UB2Image> IMG_BG;
	TWeakObjectPtr<UB2Button> BTN_Select;
	TWeakObjectPtr<UTextBlock> TB_MainText;
};
