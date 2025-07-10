// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIItemForgeListButton.h"

#include "B2UIManager.h"
#include "B2UIItemForge.h"


void UB2UIItemForgeListButton::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_ListTitle);
	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UB2Image, IMG_Selected);
}

void UB2UIItemForgeListButton::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UIItemForgeListButton::OnClickBTN_Area);
}

void UB2UIItemForgeListButton::UpdateStaticText()
{

}

void UB2UIItemForgeListButton::Init()
{
	Super::Init();
	YouHaveBeenChosen(false);
}

void UB2UIItemForgeListButton::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIItemForgeListButton::OnClickBTN_Area()
{
	if (ItemForgeMainUI)
		ItemForgeMainUI->OnClickedForgeList(this);
}

void UB2UIItemForgeListButton::SetOwnerItemForgeUI(class UB2UIItemForge* OwnerUI, int32 listID)
{
	ItemForgeMainUI = OwnerUI;
	ListID = listID;

	FText name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("FactoryID_%d"), ListID));
	
	if (TB_ListTitle.IsValid())
		TB_ListTitle->SetText(name);
}

void UB2UIItemForgeListButton::YouHaveBeenChosen(bool selected)
{
	if (IMG_Selected.IsValid())
		IMG_Selected->SetVisibility(selected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}