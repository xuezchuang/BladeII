// Fill out your copyright notice in the Description page of Project Settings.
#include "B2DynItemIcon_LobbyEquip.h"
//#include "BladeII.h"
#include "B2Inventory.h"
#include "B2ItemInfo.h"
#include "BladeIIUtil.h"
#include "B2UIUtil.h"
#include "Event.h"

UB2DynItemIcon_LobbyEquip::UB2DynItemIcon_LobbyEquip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EquipItemIconType = ELobbyEquipItemIconType::EEIIT_LobbyInven;

	IMG_StatInc_NRef = NULL;
	TB_StatIncAmount_NRef = NULL;
}

void UB2DynItemIcon_LobbyEquip::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_ItemLevel);
	GET_SLOT(UTextBlock, TB_EnhanceLevel);
	GET_SLOT(UTextBlock, TB_StartGrade);
	
	GET_SLOT(UImage, IMG_GradeBG);
	GET_SLOT(UImage, IMG_SelectFx);
	GET_SLOT(UImage, IMG_Start); 

	SubscribeEvents();
}

void UB2DynItemIcon_LobbyEquip::UpdateItemData(const FB2Item& InItem)
{
	Super::UpdateItemData(InItem);

	if (TB_ItemLevel.IsValid())
	{
		TB_ItemLevel->SetText(
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::FromString(FString::FromInt(NativeItemData.Level)))
			);
	}

	if (TB_EnhanceLevel.IsValid())
	{
		TB_EnhanceLevel->SetText(
			NativeItemData.EnhanceLevel > 0 ? FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_EnhanceLevel"))), FText::FromString(FString::FromInt(NativeItemData.EnhanceLevel))) : FText()
		);
	}
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (ItemMiscInfo)
	{
		if (IMG_Start.IsValid())
		{
			UMaterialInterface* StarMtrl = ItemMiscInfo ? ItemMiscInfo->GetItemStarGradeImage(InItem.SurpassCount > 0) : NULL;
			if (StarMtrl)
				IMG_Start->SetBrushFromMaterial(StarMtrl);
		}

		if (TB_StartGrade.IsValid())
		{
			TB_StartGrade->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(InItem.StarGrade));
			TB_StartGrade->SetText(FText::AsNumber(InItem.StarGrade));
		}
	}
}

void UB2DynItemIcon_LobbyEquip::SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables)
{
	ensure(!bForConsumables); // 父老 捞霸 甸绢吭促搁 家葛前阑 厘馒窍妨绊 沁促扁焊窜 酒付档 购啊 歹固 单捞磐肺 钎矫窍妨绰单 蔼捞 肋给 技泼登绢 乐促芭唱.. 傲 脚版静瘤 臼疽带 蔼老 淀..?

	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (IMG_GradeBG.IsValid())
	{
		UMaterialInterface* BGMtrl = ItemMiscInfo ? ItemMiscInfo->GetEquipIconBGMtrl(InItemGrade) : NULL;
		if (BGMtrl)
		{
			IMG_GradeBG->SetBrushFromMaterial(BGMtrl);
		}
	}
}

void UB2DynItemIcon_LobbyEquip::DestroySelf()
{
	UnsubscribeEvents();

	Super::DestroySelf();
}

void UB2DynItemIcon_LobbyEquip::UpdateStatIncSigns(FB2Item& CurrenEquipToCompare)
{
	if (EquipItemIconType == ELobbyEquipItemIconType::EEIIT_AutoEquipSuggest &&  // 厘馒鼻厘侩 酒捞能牢 版快俊 措秦辑父 厚背窍绰 捞固瘤 剁框.
		NativeItemData.InstanceUID > 0 && NativeItemData.ItemRefID > 0 // ID 甸捞 技泼登菌绰瘤 犬牢 瞒盔俊辑..
		//CurrenEquipToCompare.EquipPlace == NativeItemData.EquipPlace && CurrenEquipToCompare.bCurrentlyEquipped // 角力肺 鞍篮 困摹俊 厘馒登绢 乐绰 巴牢瘤.. 绰 后 巴苞 厚背且 荐档 乐栏骨肺 弊成 滴磊.
		) 
	{
		int32 PrimPointInc = B2UIUtil::DifferenceItemPrimaryPoint(NativeItemData, CurrenEquipToCompare);
		if (PrimPointInc > 0) // PrimPointInc 啊 0 捞窍扼搁 购啊 肋给等 芭
		{
			if (IMG_StatInc_NRef){ 
				IMG_StatInc_NRef->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			if (TB_StatIncAmount_NRef){
				TB_StatIncAmount_NRef->SetVisibility(ESlateVisibility::HitTestInvisible);
				TB_StatIncAmount_NRef->SetText(FText::FromString(FString::Printf(
					TEXT("+%d"), PrimPointInc
					)));
			}
		}
	}
}

void UB2DynItemIcon_LobbyEquip::SetEquipItemIconType(ELobbyEquipItemIconType InType)
{
	EquipItemIconType = InType;


	if (IMG_SelectFx.IsValid())
		IMG_SelectFx->SetVisibility(InType == ELobbyEquipItemIconType::EEIIT_AutoEquipCurrent ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (EquipItemIconType != ELobbyEquipItemIconType::EEIIT_AutoEquipSuggest)
	{ // AutoEquipSuggest 扼搁 UpdateStatIncSigns 甫 烹秦 技泼秦 林绢具 窃.
		if (IMG_StatInc_NRef){
			IMG_StatInc_NRef->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TB_StatIncAmount_NRef){
			TB_StatIncAmount_NRef->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	//woong
}

void UB2DynItemIcon_LobbyEquip::OnDetailInfoButtonPressed()
{
	// 泅犁 NativeItemData 甫 荤侩秦辑 技何 沥焊芒 扑诀.
	// UB2DynItemIcon_LobbyInven::OnDetailInfoButtonPressed 苞 付蛮啊瘤肺..
	OnClickDetailInfo.ExecuteIfBound();

	if (EquipItemIconType != ELobbyEquipItemIconType::EEIIT_LobbyInven)
	{ // 磊悼 厘馒 芒捞搁 贸澜何磐 喘府瘤 臼档废 窍绰霸 亮阑 巴.
		return;
	}

	CreateLobbyItemDetailPopupCommon(GetOwningPlayer(), NativeItemData, true); // Equip 侩栏肺 detail 芒 积己
}

void UB2DynItemIcon_LobbyEquip::UpdateItemEquipSlotVisible(const bool& bIsVisibleCostumeSlot)
{
	const bool bIsItemEquipSlotVisible = bIsVisibleCostumeSlot ? false : true;

	PlayChangeAnimEquipSlotOrCostumeSlot_BP(bIsItemEquipSlotVisible);
}

void UB2DynItemIcon_LobbyEquip::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(CostumeEquipSlotVisibleClass<bool>::GetInstance().Subscribe2(
	[this](bool IsVisible)
	{
		this->UpdateItemEquipSlotVisible(IsVisible);
	}
	));
}

void UB2DynItemIcon_LobbyEquip::UnsubscribeEvents()
{
	Issues.Empty();
}