// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2ScrollBoxWithGridPanel.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2ScrollBoxWithGridPanel : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;

public:
	UPanelSlot* UniformGridPanelAddChild(UWidget* Content);
	UUniformGridSlot* AddChildToUniformGridSlot(UWidget* Content);

	int32 GetChildrenCount();

public:
	UB2ScrollBox* GetB2ScrollBox();
	UPanelSlot* GetGridPanelSlot();

public:
	void SetB2ScrollOffset(const float& fOffset);
	void SetUniformGridPanelClearChildren();
	void SetUniformGridPanelSlotPadding(FMargin InSlotPadding);
	void SetUniformGridPanelMinDesiredSlotHeight(float InMinDesiredSlotHeight);
	
private:
	TWeakObjectPtr<UB2ScrollBox>		X_B2ScrollBox;
	TWeakObjectPtr<UUniformGridPanel>	X_UniformGridPanel;
};
