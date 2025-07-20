// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemDetailSetEffect.h"
#include "B2LobbyInventory.h"
#include "B2DynText_Multiple.h"
#include "B2UIRepeatableElements.h"



#include "B2UIStarGradePart.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

UB2LobbyUI_ItemDetailSetEffect::UB2LobbyUI_ItemDetailSetEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NonScaledSize = FVector2D(200.0f, 100.0f);
}

void UB2LobbyUI_ItemDetailSetEffect::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_SetOptionTitle);


	GET_SLOT(UOverlay, O_SetItem1);
	GET_SLOT(UB2Image, IMG_SetItem1);
	GET_SLOT(UOverlay, O_SetItem2);
	GET_SLOT(UB2Image, IMG_SetItem2);
	GET_SLOT(UOverlay, O_SetItem3);
	GET_SLOT(UB2Image, IMG_SetItem3);
	GET_SLOT(UOverlay, O_SetItem4);
	GET_SLOT(UB2Image, IMG_SetItem4);

	UIP_ItemSetEffects.AddDefaulted(MAX_ITEM_SET_EFFECT);

	GET_SLOT_BYNAME(UB2DynText_Multiple, UIP_ItemSetEffects[0], UIP_ItemSetOption1);
	GET_SLOT_BYNAME(UB2DynText_Multiple, UIP_ItemSetEffects[1], UIP_ItemSetOption2);
	GET_SLOT_BYNAME(UB2DynText_Multiple, UIP_ItemSetEffects[2], UIP_ItemSetOption3);

	GET_SLOT(UPanelWidget, P_PreFix);
	if (P_PreFix.IsValid())
		P_PreFix->SetVisibility(ESlateVisibility::Collapsed);

	GET_SLOT(UTextBlock, TB_Prefix);

	GET_SLOT(UB2UIStarGradePart, StarGradePart);

	if (StarGradePart.IsValid())
	{
		StarGradePart->Init();
		StarGradePart->SetEnableSurpass(false);
	}

}

void UB2LobbyUI_ItemDetailSetEffect::UpdateItemData(const FB2Item& InItem)
{
	// 여기서 필요 정보 가져와서 세팅.

	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	const FMD_ItemSingleElem* ItemMasterInfo = DataStore.GetItemMasterDetailInfo(InItem.ItemRefID);
	if (ItemMasterInfo)
	{
		const int32 SetUniqueKey = GetSetItemUniqueKey(ItemMasterInfo->SetGroupID, InItem.ItemRefID);
		InitSetOptionPanel(SetUniqueKey, ItemMasterInfo->ItemEquipClass, InItem);


	}
}


void UB2LobbyUI_ItemDetailSetEffect::InitSetOptionPanel(int32 InSetUniqueKey, EPCClass InPCClass, const FB2Item& InItem)
{
	if (InSetUniqueKey == 0)
		return;

	//FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	//EItemInvenType InItemInvenType = InItem.InventoryType;

	//FMD_SetInfo* SetItemInfo = DataStore.GetSetItemInfo(InPCClass, InSetUniqueKey);
	//const int32 DisplaySetOptionID = GetSetOptionID(SetItemInfo, InItemInvenType);

	//if (SetItemInfo && DisplaySetOptionID)
	//{
	//	// 상단 SetItem 
	//	if (TB_SetOptionTitle.IsValid())
	//	{
	//		TB_SetOptionTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SetEffectTitle")));
	//	}

	//	// 방어구, 장신구 Icon 및 TextLine 갯수 변경
	//	ESlateVisibility LastSetVisibility = (InItemInvenType == EItemInvenType::EIIVT_Accessory) ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible;
	//	if (O_SetItem4.IsValid())
	//		O_SetItem4->SetVisibility(LastSetVisibility);

	//	static const int32 LastSetIndex = MAX_ITEM_SET_EFFECT - 1;
	//	if (UIP_ItemSetEffects.IsValidIndex(LastSetIndex) && UIP_ItemSetEffects[LastSetIndex].IsValid())
	//		UIP_ItemSetEffects[LastSetIndex]->SetVisibility(LastSetVisibility);
	//	
	//	int32 EquipItemCount = 0;
	//	TArray<int32> SetCompositionItems = GetSetCompositionItems(SetItemInfo, InItemInvenType);
	//	for (const int32 ItemTemplateID : SetCompositionItems)
	//	{
	//		EItemClass ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(ItemTemplateID);
	//		bool bCurrentEquipItem = IsEquipSetCompisitionItem(ItemTemplateID, InPCClass);
	//		UpdateSingleCompositionIcon(ItemClass, bCurrentEquipItem);

	//		EquipItemCount += bCurrentEquipItem ? 1 : 0;
	//	}

	//	TMap<int32, int32> SetItemDatas;
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetSetItemValue(InPCClass, SetItemDatas);

	//	TArray<FB2Item> GatheredLocalEquippedItems;
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetEquippedItems(InPCClass, GatheredLocalEquippedItems);
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetEquippedCostumeItems(InPCClass, GatheredLocalEquippedItems);

	//	TArray<int32> SameGroupSetID;
	//	int GroupSetIDCount = 0;
	//	for (int i = 0; i < GatheredLocalEquippedItems.Num(); i++)
	//	{
	//		if (InItemInvenType == GatheredLocalEquippedItems[i].InventoryType
	//			&& InItem.ItemRefID % 10 == GatheredLocalEquippedItems[i].ItemRefID % 10)
	//		{
	//			if (!SameGroupSetID.Contains(GatheredLocalEquippedItems[i].SetID))
	//			{
	//				SameGroupSetID.Add(GatheredLocalEquippedItems[i].SetID);
	//			}
	//			GroupSetIDCount++;
	//		}

	//	}
	//	SameGroupSetID.Sort();

	//	TArray<bool> HighClassActived;
	//	HighClassActived.Add(false);
	//	HighClassActived.Add(false);
	//	HighClassActived.Add(false);

	//	if (SameGroupSetID.Num() > 0)
	//	{
	//		//UE_LOG(LogTemp, Log, TEXT("In"));
	//		bool IsLowClass = SameGroupSetID[0] == InItem.SetID ? true : false;
	//		
	//		if (IsLowClass)
	//		{
	//			EquipItemCount = GroupSetIDCount;
	//			//UE_LOG(LogTemp, Log, TEXT("EquipItemCount : %d"), EquipItemCount);
	//			for (int i = 1; i < SameGroupSetID.Num(); i++)
	//			{
	//				if (SetItemDatas[SameGroupSetID[i]] >= 2)
	//				{
	//					for (int j = 0; j < SetItemDatas[SameGroupSetID[i]]-1; j++)
	//					{
	//						HighClassActived[j] = true;
	//					}
	//					break;
	//				}
	//			}
	//		}
	//	}

	//	/*	for (int j = 0; j < HighClassActived.Num(); j++)
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("HighClassActived [%d] : %d"),j+1,HighClassActived[j]);
	//		}

	//		for (int i = 0; i < SameGroupSetID.Num(); i++)
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("SameGroupSetID[%d] : %d"), i + 1, SameGroupSetID[i]);
	//		}*/


	//	const FMD_ItemSetOptionElem* SetEffectInfo = DataStore.GetItemSetOptionInfo(DisplaySetOptionID);
	//	if (SetEffectInfo)
	//	{
	//		int32 index = 0;
	//		for (int32 SetItemCount = ESetItemOptionCount::FIRST_COUNT; SetItemCount <= ESetItemOptionCount::LAST_COUNT; ++SetItemCount)
	//		{
	//			FItemSetOptionData OptionData;
	//			if (SetEffectInfo->GetSetItemOption(SetItemCount, OptionData))
	//			{
	//				UpdateSetEffectTextLine(index, OptionData.SetItemOption, InPCClass, OptionData.SetItemOptionValue, 
	//					(EquipItemCount >= SetItemCount) && !(HighClassActived[index]) ? true : false);
	//				index++;
	//			}					
	//		}
	//	}

	//	// 세트 아이템 정보 출력.
	//	if (P_PreFix.IsValid())
	//		P_PreFix->SetVisibility(ESlateVisibility::HitTestInvisible);

	//	if (TB_Prefix.IsValid())
	//	{
	//		FString TempString = "";
	//		TempString += TEXT("Prefix_");
	//		TempString += FText::AsNumber(InItem.ItemRefID % 100).ToString();

	//		TB_Prefix->SetText(BladeIIGetLOCText(B2LOC_CAT_ITEMPREFIX, TempString));
	//	}

	//	if (StarGradePart.IsValid())
	//	{
	//		StarGradePart->UpdateStar(InItem);
	//	}
	//}
}

void UB2LobbyUI_ItemDetailSetEffect::UpdateSingleCompositionIcon(EItemClass InItemClass, bool bCurrentEquipItem)
{
	UMaterialInterface* SimpleIconMaterial = GetSimpleIconMaterial(InItemClass, bCurrentEquipItem);
	TWeakObjectPtr<UImage> TargetImageWidget = nullptr;

	switch (InItemClass)
	{
	case EItemClass::EIC_Helmet:
	case EItemClass::EIC_Belt:
		TargetImageWidget = IMG_SetItem1;
		break;

	case EItemClass::EIC_BodyArmor:
	case EItemClass::EIC_Necklace:
		TargetImageWidget = IMG_SetItem2;
		break;

	case EItemClass::EIC_Glove:
	case EItemClass::EIC_Ring:
		TargetImageWidget = IMG_SetItem3;
		break;

	case EItemClass::EIC_Shoes:
		TargetImageWidget = IMG_SetItem4;
		break;
	}

	if (TargetImageWidget.IsValid())
		TargetImageWidget->SetBrushFromMaterial(SimpleIconMaterial);
}

void UB2LobbyUI_ItemDetailSetEffect::UpdateSetEffectTextLine(int32 TextIndex, EItemOption OptionType, EPCClass InPCClass, float IncreaseValue, bool bCurrentApplying)
{
	if (IncreaseValue == 0.f)
		return;

	TWeakObjectPtr<UB2DynText_Multiple> SetEffectTextLine = GetSetEffectTextLine(TextIndex);
	if (SetEffectTextLine.IsValid() && SetEffectTextLine.Get() != nullptr)
	{
		FText SetCountTextFormat = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SetCountFormat"));
		FText SetCountText = FText::Format(SetCountTextFormat, FText::AsNumber(TextIndex + 2));

		FText SetOptionText = GetLOCTextOfItemOption(OptionType, InPCClass);
		FText OptionValueText = GetItemOptionValueDisplayText(OptionType, IncreaseValue, true);

		FText SetEffectTextFormat = FText::FromString(TEXT("{0} {1}"));
		FText SetEffectText = FText::Format(SetEffectTextFormat, SetOptionText, OptionValueText);

		SetEffectTextLine->SetDynText(0, SetCountText, SetEffectText);

		if (bCurrentApplying)
		{
			SetEffectTextLine->SetDynTextColor(0, ApplySetCountTextColor);
			SetEffectTextLine->SetDynTextColor(1, ApplySetEffectTextColor);
		}
	}
}

TWeakObjectPtr<UB2DynText_Multiple> UB2LobbyUI_ItemDetailSetEffect::GetSetEffectTextLine(int32 TextIndex)
{
	if (UIP_ItemSetEffects.IsValidIndex(TextIndex))
	{
		return UIP_ItemSetEffects[TextIndex];
	}

	return nullptr;
}

int32 UB2LobbyUI_ItemDetailSetEffect::GetSetOptionID(const FMD_SetInfo* InSetMasterInfo, EItemInvenType InItemInvenType)
{
	int32 DisplaySetOptionID = 0;

	if (InSetMasterInfo)
	{
		for (int32 SetOptionID : InSetMasterInfo->SetOptionIDs)
		{
			if (GetSetOptionInvenType(SetOptionID) == InItemInvenType)
			{
				DisplaySetOptionID = SetOptionID;
				break;
			}
		}
	}

	return DisplaySetOptionID;
}

TArray<int32> UB2LobbyUI_ItemDetailSetEffect::GetSetCompositionItems(const FMD_SetInfo* InSetMasterInfo, EItemInvenType InItemInvenType)
{
	TArray<int32> SetCompositionItems;

	if (InSetMasterInfo)
	{
		for (int32 ItemTemplateID : InSetMasterInfo->SetCompositionItems)
		{
			if (GetItemInvenTypeFromClass(FItemRefIDHelper::ExtractItemClassFromRefID(ItemTemplateID)) == InItemInvenType)
			{
				SetCompositionItems.Add(ItemTemplateID);
			}
		}
	}

	return SetCompositionItems;
}

bool UB2LobbyUI_ItemDetailSetEffect::IsEquipSetCompisitionItem(int32 ItemTemplateID, EPCClass InPCClass)
{
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	EItemClass SetItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(ItemTemplateID);
	EItemEquipPlace SetItemEquipPlace = GetItemEquipPlaceFromClass(SetItemClass);

	FB2Item CurrentEquipItem;
	if (UB2LobbyInventory::GetEquippedItemAtPlace(CurrentEquipItem, SetItemEquipPlace, InPCClass))
	{
		if (CurrentEquipItem.ItemRefID == ItemTemplateID)
		{
			return true;
		}
	}

	return false;
}

UMaterialInterface* UB2LobbyUI_ItemDetailSetEffect::GetSimpleIconMaterial(EItemClass InItemClass, bool bCurrentEquip)
{
	for (auto& IconInfo : SimpleIconInfos)
	{
		if (IconInfo.ItemClass == InItemClass)
		{
			return bCurrentEquip ? IconInfo.ItemSimpleIconOn : IconInfo.ItemSimpleIconOff;
		}
	}

	return nullptr;
}

