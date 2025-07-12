// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIBattleStageInfoToolTip.h"
#include "B2ItemInfo.h"
#include "BladeIIUtil.h"
#include "B2EtherInfo.h"
#include "B2EtherManager.h"
#include "B2EtherSetOptionInfo.h"
#include "B2GoodsTypeInfo.h"
#include "B2AndroidBackManager.h"

void UB2UIBattleStageInfoToolTip::Init()
{
	Super::Init();
}

void UB2UIBattleStageInfoToolTip::DestroySelf(class UB2UIManager* InUIManager)
{
	if (this->GetVisibility() == ESlateVisibility::Visible)
	{
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIBattleStageInfoToolTip::ShowRewardItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	ShowToolTip();

	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfoData = ItemInfoTable ? ItemInfoTable->GetInfoData(InRewardInfoData.SomeCommonData.ItemRefID) : NULL;

	if (IMG_InfoGoodsIcon.IsValid() && ThisItemInfoData)
	{
		IMG_InfoGoodsIcon->SetBrushFromMaterial(ThisItemInfoData->GetIconMaterial(ItemInfoTable));
	}

	if (TB_RewardItemName.IsValid() && ThisItemInfoData)
	{
		TB_RewardItemName->SetText(ThisItemInfoData->GetLocalizedName());
	}

	if (TB_RewardItemDesc.IsValid())
	{
		FString TempString = "";
		if (InRewardInfoData.SomeCommonData.ItemClass == EItemClass::EIC_Weapon)
		{
			TempString += TEXT("PreWeapon_");
		}
		else
		{
			TempString += TEXT("PreArmor_");
		}

		TempString += FText::AsNumber((InRewardInfoData.SomeCommonData.ItemRefID / 1000) % 100).ToString(); // 실제 존재하는 아이템이 아니라, 보여주기식 아이템이므로 중간 인덱스 사용

		TB_RewardItemDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_ITEMPREFIX, TempString));
	}

	if (TB_RewardItemOptionText.IsValid())
	{
		TB_RewardItemOptionText->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (InRewardInfoData.SomeCommonData.ItemClass == EItemClass::EIC_Weapon)
		{
			TB_RewardItemOptionText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemDetail_UniqueOption")));
		}
		else
		{
			TB_RewardItemOptionText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemDetail_SetOption")));
		}
	}
}

void UB2UIBattleStageInfoToolTip::ShowBossPieceInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	//ShowToolTip();

	//UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	//FSingleItemInfoData* ThisItemInfoData = ItemInfoTable ? ItemInfoTable->GetInfoData(InRewardInfoData.SomeCommonData.ItemRefID) : NULL;

	//if (IMG_InfoGoodsIcon.IsValid() && ThisItemInfoData)
	//{
	//	IMG_InfoGoodsIcon->SetBrushFromMaterial(ThisItemInfoData->GetIconMaterial(ItemInfoTable));
	//}

	//if (TB_RewardItemName.IsValid() && ThisItemInfoData)
	//{
	//	TB_RewardItemName->SetText(ThisItemInfoData->GetLocalizedName());
	//}

	//FString descStr(TEXT("Relic_PieceInfo_ACT") + FString::FormatAsNumber(InRewardInfoData.RandomOptionCount));

	////조각은 설명이 들어감. RewardItemInfo에서 TB_RewardItemOptionInfo에 넣지만 여기서 그위에 다시 덮어 씌움
	//if (TB_RewardItemDesc.IsValid())
	//{
	//	TB_RewardItemDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, descStr));
	//}

	//if (TB_RewardItemOptionText.IsValid())
	//{
	//	TB_RewardItemOptionText->SetVisibility(ESlateVisibility::Collapsed);
	//}
}

void UB2UIBattleStageInfoToolTip::ShowEtherItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	ShowToolTip();

	if (IMG_InfoGoodsIcon.IsValid())
	{
		IMG_InfoGoodsIcon->SetBrushFromMaterial(StaticFindEtherInfoTable()->GetEtherIcon(InRewardInfoData.SomeCommonData.Quality));
	}

	if (TB_RewardItemDesc.IsValid() && TB_RewardItemName.IsValid())
	{
		b2network::B2mdAetherSetOptionInfoPtr SetOptionPtr = B2EtherManager::GetInstance().GetEtherSetOptionInfoPtrByType(InRewardInfoData.SomeCommonData.Quality, 1); //tier는 1로 활용
		if (SetOptionPtr != nullptr)
		{
			if (StaticFindEtherSetOptionInfoTable() != NULL)
			{
				TB_RewardItemName->SetText(B2EtherManager::GetInstance().GetEtherName(InRewardInfoData.SomeCommonData.Quality));
				TB_RewardItemDesc->SetText(StaticFindEtherSetOptionInfoTable()->GetEtherSetOptionName(SetOptionPtr->set_option_id));
			}
		}
	}

	if (TB_RewardItemOptionText.IsValid())
	{
		TB_RewardItemOptionText->SetVisibility(ESlateVisibility::HitTestInvisible);
		TB_RewardItemOptionText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemDetail_SetOption")));
	}
}

void UB2UIBattleStageInfoToolTip::ShowMaterialItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData)
{
	ShowToolTip();

	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfoData = ItemInfoTable ? ItemInfoTable->GetInfoData(InRewardInfoData.SomeCommonData.ItemRefID) : NULL;

	if (IMG_InfoGoodsIcon.IsValid() && ThisItemInfoData)
	{
		IMG_InfoGoodsIcon->SetBrushFromMaterial(ThisItemInfoData->GetIconMaterial(ItemInfoTable));
	}

	if (TB_RewardItemName.IsValid() && ThisItemInfoData)
	{
		TB_RewardItemName->SetText(ThisItemInfoData->GetLocalizedName());
	}

	if (TB_RewardItemDesc.IsValid() && ThisItemInfoData)
	{
		const FSingleGoodsTypeInfoData* GoodsTypeInfo = StaticFindGoodsTypeInfoTable()->GetInfoData(ThisItemInfoData->FGoodsType);
		
		FText GoodsTypeText = BladeIIGetLOCText(B2LOC_CAT_TOOLTIPTEXT, *(FString::FromInt(GoodsTypeInfo->GoodsInfoStringID)));
		TB_RewardItemDesc->SetText(GoodsTypeText);
	}

	if (TB_RewardItemOptionText.IsValid())
	{
		TB_RewardItemOptionText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIBattleStageInfoToolTip::HideToolTip()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIBattleStageInfoToolTip::CloseWidgetDelegate()
{
	HideToolTip();
}

void UB2UIBattleStageInfoToolTip::CacheAssets()
{
	GET_SLOT(UImage, IMG_InfoGoodsIcon);
	GET_SLOT(UTextBlock, TB_RewardItemName);
	GET_SLOT(UTextBlock, TB_RewardItemDesc);
	GET_SLOT(UTextBlock, TB_RewardItemOptionText);
	GET_SLOT(UB2Button, BTN_HideRewardItemInfoSet);
}

void UB2UIBattleStageInfoToolTip::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_HideRewardItemInfoSet, &UB2UIBattleStageInfoToolTip::HideToolTip)
}

void UB2UIBattleStageInfoToolTip::ShowToolTip()
{
	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	this->SetVisibility(ESlateVisibility::Visible);
}