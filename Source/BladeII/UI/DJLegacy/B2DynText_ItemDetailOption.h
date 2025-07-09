// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2DynTextBase.h"
#include "BladeIIPlayer.h"
#include "B2DynText_ItemDetailOption.generated.h"

/**
 * Text row set displaying item's single option (ability), be dynamically created and deployed onto item detail panel.
 */
UCLASS()
class BLADEII_API UB2DynText_ItemDetailOption : public UB2DynTextBase
{
	GENERATED_BODY()
	

protected:
		
	TWeakObjectPtr<UTextBlock> TB_OptionName; 

	/** Value text at left side, can be either just empty or the option value. */
	TWeakObjectPtr<UTextBlock> TB_OptionValueL;

	/** Value text at right side, can be either the option value or compared inc/dec delta. */
	TWeakObjectPtr<UTextBlock> TB_OptionValueR;
	
public:

	UB2DynText_ItemDetailOption(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void CacheAssets() override;

public:
	/** To be called from ItemDetails panel widget, with its relevant item data and some of properties. */
	void UpdateOptionTextSet(bool bIsForEquippedItem, int32 RowIndex, EPCClass InPCClass, const struct FItemOption* InDisplayOption, struct FItemOption* InEquippedOption,
		const FSlateColor& StatIncColor, const FSlateColor& StatSameColor, const FSlateColor& StatDecColor, bool IsAfterEvent = false);
	void UpdateOptionTextSet(FText InText);

	void UpdatePrimaryRangeOption(int32 RowIndex, EItemPrimaryPointType, int32 LowValue, int32 HighValue);

private:
	bool GetItemOptionValueBySecond(EItemOption InDisplayOption);
	bool GetItemOptionValueByPercent(EItemOption InDisplayOption);

};
