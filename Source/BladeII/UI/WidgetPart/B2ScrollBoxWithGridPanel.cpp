// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2ScrollBoxWithGridPanel.h"

void UB2ScrollBoxWithGridPanel::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2ScrollBox, X_B2ScrollBox);
	GET_SLOT(UUniformGridPanel, X_UniformGridPanel);
}

UPanelSlot* UB2ScrollBoxWithGridPanel::UniformGridPanelAddChild(UWidget* Content)
{
	UPanelSlot* pAddPanelSlot = nullptr;

	if (Content && X_UniformGridPanel.IsValid())
	{
		pAddPanelSlot = X_UniformGridPanel->AddChild(Content);
	}

	return pAddPanelSlot;
}

UUniformGridSlot* UB2ScrollBoxWithGridPanel::AddChildToUniformGridSlot(UWidget* Content)
{
	UUniformGridSlot* pAddUniformGridSlot = nullptr;

	if (Content && X_UniformGridPanel.IsValid())
	{
		pAddUniformGridSlot = X_UniformGridPanel->AddChildToUniformGrid(Content);
	}

	return pAddUniformGridSlot;
}

int32 UB2ScrollBoxWithGridPanel::GetChildrenCount()
{
	if (X_UniformGridPanel.IsValid())
		return X_UniformGridPanel->GetSlots().Num();
	return 0;
}

UB2ScrollBox* UB2ScrollBoxWithGridPanel::GetB2ScrollBox()
{
	UB2ScrollBox* pB2ScrollBox = nullptr;

	if (X_B2ScrollBox.IsValid())
	{
		pB2ScrollBox = X_B2ScrollBox.Get();
	}

	return pB2ScrollBox;
}

UPanelSlot* UB2ScrollBoxWithGridPanel::GetGridPanelSlot()
{
	UPanelSlot* pGridPanelSlot = nullptr;

	if (X_UniformGridPanel.IsValid())
	{
		pGridPanelSlot = Cast<UPanelSlot>(X_UniformGridPanel->Slot);
	}

	return pGridPanelSlot;
}

void UB2ScrollBoxWithGridPanel::SetB2ScrollOffset(const float& fOffset)
{
	if (X_B2ScrollBox.IsValid())
	{
		X_B2ScrollBox->SetScrollOffset(fOffset);
	}
}

void UB2ScrollBoxWithGridPanel::SetUniformGridPanelClearChildren()
{
	if (X_UniformGridPanel.IsValid())
	{
		X_UniformGridPanel->ClearChildren();
	}
}

void UB2ScrollBoxWithGridPanel::SetUniformGridPanelSlotPadding(FMargin InSlotPadding)
{
	if (X_UniformGridPanel.IsValid())
	{
		X_UniformGridPanel->SetSlotPadding(InSlotPadding);
	}
}

void UB2ScrollBoxWithGridPanel::SetUniformGridPanelMinDesiredSlotHeight(float InMinDesiredSlotHeight)
{
	if (X_UniformGridPanel.IsValid())
	{
		X_UniformGridPanel->SetMinDesiredSlotHeight(InMinDesiredSlotHeight);
	}
}