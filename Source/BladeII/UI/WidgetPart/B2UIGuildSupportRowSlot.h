// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "CommonStruct.h"
#include "B2UIGuildSupportRowSlot.generated.h"

/**
 * 
 */
namespace B2GuildRequestItem
{
	const int32 SlotColumnMax = 3;
}

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGuildSupportIconClickedDelegate, class UB2DynItemIcon_GuildSupport*, ItemIcon, int32, TempleteItemID);

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildSupportRowSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	void SetCategoryName(const FText &InText);
	void UpdateItemIcons(const TArray<FB2Item>& AllItemsArray, const int32 InStartIndex);
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	
	UFUNCTION()
	void OnCallbackCol_0();
	UFUNCTION()
	void OnCallbackCol_1();
	UFUNCTION()
	void OnCallbackCol_2();

public:
	FOnGuildSupportIconClickedDelegate OnDelegate;
private:
	TArray<int32> ItemIDs;
	TArray<TWeakObjectPtr<UB2Button>> Buttons;

	TArray<TWeakObjectPtr<class UB2DynItemIcon_GuildSupport>> Items;
	TWeakObjectPtr<UTextBlock> TB_ItemRowName;
};
