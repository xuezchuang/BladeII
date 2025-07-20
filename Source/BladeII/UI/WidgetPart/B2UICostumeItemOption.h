// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UICostumeItemOption.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UICostumeItemOption : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

public:
	void InitCostumeItemOption(const EPCClass& ItemAllowedPCClass, const TArray<FItemOption>& ArrayItemOption,
		const TArray<FItemOption>* ArrayItemOptionAtSamePlace);

private:
	bool IsSameItemOptionOfType(const TArray<FItemOption>& ArrayItemOptionAtSamePlace, FItemOption& OutFoundOption, EItemOption InSearchOption);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeltaColor")
	FLinearColor ItemStatIncColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeltaColor")
	FLinearColor ItemStatSameColor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeltaColor")
	FLinearColor ItemStatDecColor;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2DynText_ItemDetailOption>	UIItemDetailOptionClass;

private:
	TWeakObjectPtr<UB2RichTextBlock>	RTB_Title;
	TWeakObjectPtr<UVerticalBox>		VB_OptionList;
};
