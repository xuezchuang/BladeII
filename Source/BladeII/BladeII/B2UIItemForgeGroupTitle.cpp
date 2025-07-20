// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIItemForgeGroupTitle.h"

#include "B2UIManager.h"
#include "B2UIItemForge.h"



void UB2UIItemForgeGroupTitle::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_Info);
	GET_SLOT(UTextBlock, TB_GroupTitle);
	GET_SLOT(UTextBlock, TB_IngredientTitle);
	GET_SLOT(UOverlay, O_Group);
	GET_SLOT(UOverlay, O_Ingredient);
}

void UB2UIItemForgeGroupTitle::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Info, &UB2UIItemForgeGroupTitle::OnClickedBTNInfo);
}

void UB2UIItemForgeGroupTitle::UpdateStaticText()
{

}

void UB2UIItemForgeGroupTitle::Init()
{
	Super::Init();
}

void UB2UIItemForgeGroupTitle::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIItemForgeGroupTitle::SetOwnerItemForgeUI(class UB2UIItemForge* OwnerUI, int32 groupid, bool isIngredient, bool thisIsMainIngred, bool isItForge)
{
	ItemForgeMainUI = OwnerUI;
	GroupID = groupid;
	if (isIngredient == false)
	{
		FText name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("GroupID_%d"), GroupID));

		if (O_Group.IsValid()) O_Group->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (O_Ingredient.IsValid()) O_Ingredient->SetVisibility(ESlateVisibility::Collapsed);

		if (TB_GroupTitle.IsValid())
			TB_GroupTitle->SetText(name);
		if (TB_IngredientTitle.IsValid())
			TB_IngredientTitle->SetText(FText::GetEmpty());
	}
	else
	{
		FText name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("IngredientTitle_%d"), GroupID));
		isForgeSection = isItForge;
		if (thisIsMainIngred == true)
		{
			BTN_Info->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			BTN_Info->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (O_Group.IsValid()) O_Group->SetVisibility(ESlateVisibility::Collapsed);
		if (O_Ingredient.IsValid()) O_Ingredient->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (TB_GroupTitle.IsValid())
			TB_GroupTitle->SetText(FText::GetEmpty());
		if (TB_IngredientTitle.IsValid())
			TB_IngredientTitle->SetText(name);
	}
}

void UB2UIItemForgeGroupTitle::OnClickedBTNInfo()
{
	if (isForgeSection)
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(
			EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Alert_Forge")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
		);
	}
	else
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(
			EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Alert_Upgrade")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
		);
	}
}