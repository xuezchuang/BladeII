// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeIIUserWidget.h"
#include "B2UIItemDetailOption.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIItemDetailOption : public UBladeIIUserWidget
{
	GENERATED_BODY()
	
public:
	UB2UIItemDetailOption(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void DestroySelf() override;

	void SetItemData(
		const FB2Item& InItem, 
		const FB2Item& InSamePlaceItem, 
		bool InIsEquip, 
		FSlateColor InItemStatIncColor,
		FSlateColor InItemStatSameColor, 
		FSlateColor InItemStatDecColor);

protected:
	virtual void CacheAssets() override;
	
private:
	void Init();
	void UpdateStaticText();
	void BindDelegates();

	void OnClickOptionTab();
	void OnClickRandomOptionTab();
	void OnClickSealTab();
	void SelectTabs(int32 InValue);
	void UpdateOptionInfos(const TArray<FItemOption>& InOptionInfos, const TArray<FItemOption>& InSameItemOptionInfos);
	void UpdateOptionInfos(const TArray<FSealOption>& InOptionInfos, const TArray<FItemOption>& InSameItemOptionInfos);
	void UpdateOptionInfo(int32 InIndex, const FItemOption& InOptionInfo, const TArray<FItemOption>& InSameItemOptionInfos);
	void EmptySealInfo(int32 InIndex);
	class UB2DynText_ItemDetailOption* GetOptionSlot(int32 InIndex);
	void OffUIOptionInfos();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
		TSubclassOf<class UB2DynText_ItemDetailOption> OptionInfoClass;
		
private:
	const int32 OPTION = 1;
	const int32 RANDOMOPTION = 2;
	const int32 SEAL = 3;

	TArray<FItemOption> ItemOptions;
	TArray<FItemOption> ItemRandomOptions;
	TArray<FSealOption> SealOptions;
	bool IsEquip;
	int32 ItemAllowedPCClass;
	FSlateColor ItemStatIncColor;
	FSlateColor ItemStatSameColor;
	FSlateColor ItemStatDecColor;
	TArray<FItemOption> SamePlaceItemOptions;
	TArray<FItemOption> SamePlaceItemRandomOptions;
	TArray<FItemOption> SamePlaceItemSealOptions;

	TMap<int32, TWeakObjectPtr<class UB2UICommonTab>> UIP_Tabs;
	TArray<TWeakObjectPtr<class UB2DynText_ItemDetailOption>> OptionInfos;
	TWeakObjectPtr<UScrollBox> OptionScroll;
};
