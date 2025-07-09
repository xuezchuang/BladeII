// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2DynItemIcon_GuildSupport.h"
#include "B2Inventory.h"
#include "B2ItemInfo.h"
#include "BladeIIUtil.h"

UB2DynItemIcon_GuildSupport::UB2DynItemIcon_GuildSupport(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2DynItemIcon_GuildSupport::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_ItemLevel);

	StarGradeImages.SetNum(FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE);
	GET_SLOT_BYNAME(UImage, StarGradeImages[0], Star_1);
	GET_SLOT_BYNAME(UImage, StarGradeImages[1], Star_2);
	GET_SLOT_BYNAME(UImage, StarGradeImages[2], Star_3);
	GET_SLOT_BYNAME(UImage, StarGradeImages[3], Star_4);
	GET_SLOT_BYNAME(UImage, StarGradeImages[4], Star_5);
	GET_SLOT_BYNAME(UImage, StarGradeImages[5], Star_6);

	GET_SLOT(UImage, IMG_GradeBG);
	GET_SLOT(UImage, MainIconImage);
	GET_SLOT(UImage, IMG_ExtraSlotEffect);

	GET_SLOT(UImage, EnhanceIngredSelectionImage_01);
	GET_SLOT(UImage, EnhanceIngredSelectionImage_02);

	GET_SLOT(UTextBlock, TB_ItemLevel);
	GET_SLOT(UTextBlock, TB_EnhanceLevel);
	GET_SLOT(UImage, IMG_GradeBG);
	GET_SLOT(UImage, IMG_SelectFx);
}

void UB2DynItemIcon_GuildSupport::UpdateItemData(const FB2Item& InItem)
{
	//Super::UpdateItemData(InItem);

	// 클라이언트 아이템 리소스 테이블
	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(InItem.ItemRefID) : NULL;
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();

	// UMG 쪽에 배치한 widget 들 업데이트

	// 장착 아이템이나 정수만 별성에 따른 차이..
	SetBackgroundImageFromGrade((IsEquipmentItem(InItem) || IsEssenceItem(InItem)) ? InItem.StarGrade : 1, (InItem.InventoryType == EItemInvenType::EIIVT_Consumables));

	if (MainIconImage.IsValid())
	{
		MainIconImage->SetBrushFromMaterial(ThisItemInfo ? ThisItemInfo->GetIconMaterial(AllItemInfo) : NULL);
	}

	SetBackgroundStarGrade(InItem.StarGrade, ItemMiscInfo ? ItemMiscInfo->GetItemStarGradeImage((InItem.SurpassCount > 0)) : NULL);
	
	if (IMG_ExtraSlotEffect.IsValid())
	{
		IMG_ExtraSlotEffect->SetVisibility((ThisItemInfo && ThisItemInfo->bHasExtraSlotEffect) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	if (TB_ItemLevel.IsValid())
	{
		TB_ItemLevel->SetVisibility(ESlateVisibility::Hidden);
	}

	if (TB_EnhanceLevel.IsValid())
	{
		TB_EnhanceLevel->SetVisibility(ESlateVisibility::Hidden);
	}

	SetSelectIcon(false);
}

void UB2DynItemIcon_GuildSupport::SetBackgroundStarGrade(int32 InItemGrade, UMaterialInterface* StarMtrl)
{
	for (int32 II = 0; II < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; ++II)
	{
		if (StarGradeImages.IsValidIndex(II) == false)
			continue;

		auto& CurrSGImage = StarGradeImages[II];
		if (CurrSGImage.IsValid())
		{
			CurrSGImage->SetVisibility(InItemGrade >= II + 1 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
			if (StarMtrl)
			{
				CurrSGImage->SetBrushFromMaterial(StarMtrl);
			}
		}
	}
}

void UB2DynItemIcon_GuildSupport::SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables)
{
	ensure(!bForConsumables); // 만일 이게 들어왔다면 소모품을 장착하려고 했다기보단 아마도 뭔가 더미 데이터로 표시하려는데 값이 잘못 세팅되어 있다거나.. 걍 신경쓰지 않았던 값일 듯..?

	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (IMG_GradeBG.IsValid())
	{
		bool bSimpleBG = true;
		bool bCunsumable = false;
		UMaterialInterface* BGMtrl = ItemMiscInfo ? ItemMiscInfo->GetItemIconBGMtrl(InItemGrade, bSimpleBG, bCunsumable) : NULL;
		if (BGMtrl)
		{
			IMG_GradeBG->SetBrushFromMaterial(BGMtrl);
		}
	}
}

void UB2DynItemIcon_GuildSupport::SetSelectIcon(bool bIsSelected)
{
	ESlateVisibility Visible = bIsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;

	if (EnhanceIngredSelectionImage_01.IsValid())
		EnhanceIngredSelectionImage_01->SetVisibility(Visible);
	if (EnhanceIngredSelectionImage_02.IsValid())
		EnhanceIngredSelectionImage_02->SetVisibility(Visible);
}