// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIPEtherSetToolTip.generated.h"

/**
 * 
 */
struct FEtherSetSlotInfo
{
	TWeakObjectPtr<UTextBlock>	TB_Name;
	TWeakObjectPtr<UTextBlock>	TB_Value;
	TWeakObjectPtr<UPanelWidget> PW_Slot;

	void SetText(FString InNameKey, FString InOptionKey, float InValue1, bool IsVisible, float InValue2 = 0.0f);
};

UCLASS()
class UB2UIPEtherSetToolTip : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetInfo(int32 InSetID, EPCClass InPCClass);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

private:
	UFUNCTION()
		void OnClickClose();

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TArray<FEtherSetSlotInfo> EtherSetOptionInfos;
};
