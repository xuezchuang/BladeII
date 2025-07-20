// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "../../DataStore/B2TotemData.h"
#include "../B2RichTextBlock.h"
#include "B2UI_TotemSmeltingBonus.generated.h"

/**
 *
 */
UCLASS()
class BLADEII_API UB2UI_TotemSmeltingBonus : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;

public:
	void InitTotemSmeltingBonus(const FB2Totem& InTotem);

private:
	void UpdateSmeltingBonusEffectText(const int32& SmeltingLevel, struct FB2TotemBonusOptionInfo& BonusOptionInfo);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor ApplySetTextColor;

private:
	TWeakObjectPtr<UB2RichTextBlock>			RTB_SmeltingBonusTitle;

	TArray<TWeakObjectPtr<class UB2DynText_Multiple>>	ArraySmeltingBonusText;
};
