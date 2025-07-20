// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UISelectiveLotteryCostumeSlot.h"
#include "B2ItemInfo.h"
#include "BladeIIGameImpl.h"
#include "B2UIManager.h"


void UB2UISelectiveLotteryCostumeSlot::Init()
{
	Super::Init();

	bSelected = false;
}

void UB2UISelectiveLotteryCostumeSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_CostumeItemSlot, &UB2UISelectiveLotteryCostumeSlot::OnClickBtnArea);
}

void UB2UISelectiveLotteryCostumeSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UISelectiveLotteryCostumeSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_CostumeGrade);
	GET_SLOT(UImage, IMG_CostumeIcon);
	GET_SLOT(UImage, IMG_Reading_Glasses);
	GET_SLOT(UOverlay, O_Selected);
	GET_SLOT(UB2RichTextBlock, RTB_CostumeName);
	GET_SLOT(UB2Button, BTN_CostumeItemSlot);
}

void UB2UISelectiveLotteryCostumeSlot::BindDoc(const FB2Item& InItemData)
{
	UnbindDoc();
	SetDocStruct(InItemData);

	class UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	FSingleItemInfoData* InItemInfoData = ItemInfoTable->GetInfoData(InItemData.ItemRefID);

	SetCostumeGrade(InItemData.StarGrade);
	SetCostumeIcon(InItemInfoData->GetIconMaterial(StaticFindItemInfo()));
	SetCostumeName(InItemInfoData->GetLocalizedName(false, InItemData.Quality), InItemData.StarGrade);
}

void UB2UISelectiveLotteryCostumeSlot::UnbindDoc()
{
	BindedStruct.Empty();
}

void UB2UISelectiveLotteryCostumeSlot::SetCostumeGrade(const int32 InValue)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (IMG_CostumeGrade.IsValid())
	{
		UMaterialInterface* BGMtrl;

		BGMtrl = ItemMiscInfo ? ItemMiscInfo->GetCostumeIconBGMtrl(InValue) : NULL;

		if (BGMtrl)
		{
			IMG_CostumeGrade->SetBrushFromMaterial(BGMtrl);
		}
	}
}

void UB2UISelectiveLotteryCostumeSlot::SetCostumeIcon(UMaterialInterface* InMaterial)
{
	if (IMG_CostumeIcon.IsValid())
		IMG_CostumeIcon->SetBrushFromMaterial(InMaterial);
}

void UB2UISelectiveLotteryCostumeSlot::SetCostumeName(const FText& InText, const int32 InGrade)
{
	if (RTB_CostumeName.IsValid())
	{
		UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
		if (ItemMiscInfo)
		{
			RTB_CostumeName->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(InGrade).GetSpecifiedColor());
		}

		RTB_CostumeName->SetText(InText);
	}
}

void UB2UISelectiveLotteryCostumeSlot::SetSelected(bool InbSelected)
{
	if (O_Selected.IsValid())
	{
		O_Selected->SetVisibility(InbSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UISelectiveLotteryCostumeSlot::SetEnableReadingGlasses(bool Enable)
{
	if (IMG_Reading_Glasses.IsValid())
	{
		IMG_Reading_Glasses->SetVisibility(Enable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UISelectiveLotteryCostumeSlot::OnClickBtnArea()
{
	OnSlotCostumeClickDelgate.ExecuteIfBound(this);
}