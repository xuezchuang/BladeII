// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UICostumeItemDetailSetEffect.h"

#include "BladeIIUtil.h"
#include "B2UIStarGradePart.h"
#include "BladeIIGameImpl.h"
#include "B2LobbyInventory.h"
#include "B2DynText_Multiple.h"

const int32 MAX_COSTUME_SET_EFFECT = 2;

void UB2UICostumeItemDetailSetEffect::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, RTB_SetOptionTitle);

	GET_SLOT(UB2RichTextBlock, RTB_Prefix);

	GET_SLOT(UB2UIStarGradePart, UIP_StarGradePart);
	if (UIP_StarGradePart.IsValid())
	{
		UIP_StarGradePart->Init();
		UIP_StarGradePart->SetEnableSurpass(false);
	}

	ArraySetEffectText.AddDefaulted(MAX_COSTUME_SET_EFFECT);
	const int32 iArrayCostumeSetEffectTextMax = ArraySetEffectText.Num();
	for (int32 i = 0; i < iArrayCostumeSetEffectTextMax; ++i)
	{
		if (ArraySetEffectText.IsValidIndex(i))
		{
			ArraySetEffectText[i] = GetCachedWidget<UB2DynText_Multiple>(FName(*FString::Printf(TEXT("UIP_ItemSetOption%d"), i)));
		}
	}

	const int32 iArrayCostumeSetItemIconInfoMax = ArrayCostumeSetItemIconInfo.Num();
	for (int32 i = 0; i < iArrayCostumeSetItemIconInfoMax; ++i)
	{
		if (ArrayCostumeSetItemIconInfo.IsValidIndex(i))
		{
			ArrayCostumeSetItemIconInfo[i].IMG_Icon = GetCachedWidget<UB2Image>(FName(*FString::Printf(TEXT("IMG_SetItem%d"), i)));
		}
	}
}

void UB2UICostumeItemDetailSetEffect::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (RTB_SetOptionTitle.IsValid())
	{
		RTB_SetOptionTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SetEffectTitle")));
	}
}

void UB2UICostumeItemDetailSetEffect::InitCostumeItemDetailSetEffect(const FB2Item& InItem)
{
	this->Init();

	FClientDataStore& fClientDataStore = BladeIIGameImpl::GetClientDataStore();

	const FMD_ItemSingleElem* pItemMasterInfo = fClientDataStore.GetItemMasterDetailInfo(InItem.ItemRefID);
	if (pItemMasterInfo)
	{
		const EPCClass ItemEquipPCClass = pItemMasterInfo->ItemEquipClass;
		const int32 iSetUniqueKey = GetSetItemUniqueKey(pItemMasterInfo->SetGroupID, InItem.ItemRefID);
		const FMD_SetInfo* pSetItemInfo = fClientDataStore.GetSetItemInfo(ItemEquipPCClass, iSetUniqueKey);
		if (pSetItemInfo)
		{
			UpdatePrefixText(pItemMasterInfo->SetGroupID);
			UpdateStarGradePart(InItem);
			UpdateSetEffectIcon(ItemEquipPCClass, pSetItemInfo);
			UpdateSetEffectText(ItemEquipPCClass, pItemMasterInfo->SetOptionID, pSetItemInfo);
		}
	}
}

void UB2UICostumeItemDetailSetEffect::UpdatePrefixText(const int32& iSetGroupID)
{
	if (RTB_Prefix.IsValid() == false)
	{
		return;
	}

	FString TempString = "";
	TempString += TEXT("Prefix_");
	TempString += FText::AsNumber(iSetGroupID).ToString();

	RTB_Prefix->SetText(BladeIIGetLOCText(B2LOC_CAT_ITEMPREFIX, TempString));
}

void UB2UICostumeItemDetailSetEffect::UpdateStarGradePart(const FB2Item& InItem)
{
	if (UIP_StarGradePart.IsValid())
	{
		UIP_StarGradePart->UpdateStar(InItem);
	}
}

void UB2UICostumeItemDetailSetEffect::UpdateSetEffectIcon(const EPCClass& InPCClass, const FMD_SetInfo* pSetItemInfo)
{
	if (pSetItemInfo == nullptr)
	{
		return;
	}

	TArray<FB2Item> ArryEquipCostume;
	UB2LobbyInventory::GetAllEquippedCostumeItem(ArryEquipCostume, InPCClass);

	for (int32 ItemTemplateID : pSetItemInfo->SetCompositionItems)
	{
		bool IsEquipped = false;
		const int32 iArrayMax = ArryEquipCostume.Num();
		for (int32 i = 0; i < iArrayMax; ++i)
		{
			if (ArryEquipCostume.IsValidIndex(i) && ArryEquipCostume[i].ItemRefID == ItemTemplateID)
			{
				IsEquipped = true;
				break;
			}
		}

		EItemClass SetItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(ItemTemplateID);
		SetItemIconImg(SetItemClass, IsEquipped);
	}
}

void UB2UICostumeItemDetailSetEffect::UpdateSetEffectText(const EPCClass& InPCClass, const int32 iSetOptionID, const FMD_SetInfo* pSetItemInfo)
{
	const FMD_ItemSetOptionElem* pSetOptionInfo = BladeIIGameImpl::GetClientDataStore().GetItemSetOptionInfo(iSetOptionID);
	if (pSetOptionInfo)
	{
		TArray<FB2Item> ArryEquipCostume;
		UB2LobbyInventory::GetAllEquippedCostumeItem(ArryEquipCostume, InPCClass);

		int32 SetItemEquipCount = 0;
		for (int32 ItemTemplateID : pSetItemInfo->SetCompositionItems)
		{
			const int32 iArrayMax = ArryEquipCostume.Num();
			for (int32 i = 0; i < iArrayMax; ++i)
			{
				if (ArryEquipCostume.IsValidIndex(i) && ArryEquipCostume[i].ItemRefID == ItemTemplateID)
					++SetItemEquipCount;
			}
		}


		int32 iSetEffectTextIndex = 0;
		for (int32 SetItemCount = ESetItemOptionCount::FIRST_COUNT; SetItemCount < ESetItemOptionCount::LAST_COUNT; ++SetItemCount)
		{
			FItemSetOptionData OptionData;
			pSetOptionInfo->GetSetItemOption(SetItemCount, OptionData);

			if (ArraySetEffectText.IsValidIndex(iSetEffectTextIndex))
			{
				FText SetCountTextFormat = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SetCountFormat"));
				FText SetCountText = FText::Format(SetCountTextFormat, FText::AsNumber(iSetEffectTextIndex + 2));

				FText SetOptionText = GetLOCTextOfItemOption(OptionData.SetItemOption, InPCClass);
				FText OptionValueText = GetItemOptionValueDisplayText(OptionData.SetItemOption, OptionData.SetItemOptionValue, true);

				FText SetEffectTextFormat = FText::FromString(TEXT("{0} {1}"));
				FText SetEffectText = FText::Format(SetEffectTextFormat, SetOptionText, OptionValueText);

				ArraySetEffectText[iSetEffectTextIndex]->SetDynText(0, SetCountText, SetEffectText);
				if (SetItemCount <= SetItemEquipCount)
				{
					ArraySetEffectText[iSetEffectTextIndex]->SetDynTextColor(0, ApplySetTextColor);
					ArraySetEffectText[iSetEffectTextIndex]->SetDynTextColor(1, ApplySetTextColor);
				}
			}

			iSetEffectTextIndex++;
		}
	}
}

void UB2UICostumeItemDetailSetEffect::SetItemIconImg(const EItemClass& InItemClass, const bool& IsEquipped)
{
	const int32 iArrayMax = ArrayCostumeSetItemIconInfo.Num();
	for (int32 i = 0; i < iArrayMax; ++i)
	{
		if (ArrayCostumeSetItemIconInfo.IsValidIndex(i) == false)
		{
			continue;
		}

		if (ArrayCostumeSetItemIconInfo[i].ItemClass == InItemClass)
		{
			UMaterialInterface* pIconMaterial = nullptr;

			if (IsEquipped == true)
			{
				pIconMaterial = ArrayCostumeSetItemIconInfo[i].ItemSimpleIconOn;
			}
			else
			{
				pIconMaterial = ArrayCostumeSetItemIconInfo[i].ItemSimpleIconOff;
			}

			if (ArrayCostumeSetItemIconInfo[i].IMG_Icon.IsValid())
			{
				ArrayCostumeSetItemIconInfo[i].IMG_Icon->SetBrushFromMaterial(pIconMaterial);
			}
			break;
		}
	}
}
