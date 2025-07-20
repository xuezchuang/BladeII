#include "B2ClientDataStore.h"


#include "B2StageManager.h"
#include "Event.h"
#include "B2UIManager.h"
#include "B2UILobbyMain.h"
#include "Retailer.h"
#include "B2UIDocHelper.h"
#include "B2UIChapterFrame.h"
#include "B2Inventory.h"
#include "B2StageGameMode.h"
#include "BladeIIPointCharger.h"
#include "B2SkillInfo.h"
#include "B2ChapterInfo.h"
#include "B2PCClassInfo.h"
#include "B2PCClassInfoBox.h"
#include "B2UIMsgPopupInput.h"
#include "B2BrevetInfo.h"
#include "Quest.h"
#include "B2LobbyInventory.h"
#include "B2RewardInfo.h"
#include "B2RaidInfo.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIHeader.h"
#include "B2CombatStatEvaluator.h"
#include "TutorialManager.h"
//#include "Algo/Find.h"
#include "B2UISettingGlobalSoundTheInital.h"
//
#include "HotTimeBuffManager.h"
#include "RewardEventManager.h"

#include "CommonStruct.h"
#include "BladeIIPlayer.h"
#include "Common.h"
#include "B2UIEnum.h"
#include "B2UIDocChapter.h"
#include "BladeIIUtil.h"

#include "Global.h"
#include "EventSubsystem.h"
#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"
#include "B2CodeTable.h"
#include "B2ItemInfo.h"
#include "B2LoadingImageInfo.h"
#include "B2GameInstance.h"
#include "BladeIIBlockToSyncNetwork.h"
#include "FairyManager.h"
#include "B2Version.h"
#include "B2UIDocDonation.h"

#include "B2LobbyUI_ConvertItemPopup.h"
#include "B2UIItemQualityChange.h"

inline uint8 EItemEquipPlaceToUINT(EItemEquipPlace Enum)
{
	return static_cast<uint8>(Enum);
}

inline bool CheckConsumeableItem(EItemInvenType type)
{
	return type == EItemInvenType::EIIVT_Consumables;
}

FEventInfo & FEventInfo::operator=(const b2network::B2mdEventInfo& InEventInfo)
{
	event_id = InEventInfo.event_id;
	event_category = InEventInfo.event_category;
	event_tab_name = InEventInfo.event_tab_name;
	title = InEventInfo.title;
	start_time = InEventInfo.start_time;
	end_time = InEventInfo.end_time;
	explain = InEventInfo.explain;
	link_text = InEventInfo.link_text;
	link_url = InEventInfo.link_url;
	tab_order = InEventInfo.tab_order;
	banner_url = InEventInfo.banner_url;

	return *this;
}


int64 FSummonItemSlotData::GetNextFreeTimeLeftSeconds()
{
	//if (NextFreeTime == 0)
		return 0;

	//int64 LeftSeconds = (NextFreeTime - UB2GameInstance::GetUTCNow()).GetTotalSeconds();
	//if (LeftSeconds <= 0)
	//	NextFreeTime = LeftSeconds = 0;

	//return LeftSeconds;
}

void FPerCharacterItemContainer::AddItem(const FB2Item& Item, bool CheckPreset /*= false*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_AddItem);
	if (CheckPreset && IsInCurrentItemPreset(Item.InstanceUID))
	{
		const uint32 EquipPlaceUint = EItemEquipPlaceToUINT(Item.EquipPlace);
		if (EquipPlaceUint >= 0 && EquipPlaceUint < (int32)EItemEquipPlace::EIEP_End)
		{
			FEquippedItemInfo& ThisEquipSlot = AllEuippedInfo[EquipPlaceUint];

			// 咯扁肺 甸绢坷绰 厘馒 沥焊绰 account info 己拜栏肺 捞固 厘馒等 单捞磐甫 罐绰 版快捞绊, 厘馒阑 官层摹绰 版快绰 酒丛. 厘馒 沥焊啊 滴锅 甸绢坷霸 瞪 荐绰 乐澜.
			BII_CHECK(ThisEquipSlot.bIsEquipped == false || ThisEquipSlot.EquippedItem.InstanceUID == Item.InstanceUID);

			ThisEquipSlot.bIsEquipped = true;
			ThisEquipSlot.EquippedItem = Item;
		}
		return;
	}

	//厘馒 内胶片 眉农饶 喊档 利犁
	if (CheckPreset && Item.bCurrentlyEquipped)
	{
		const uint32 iCostumeEquipPlace = static_cast<uint32>(Item.CostumeEquipPlace);
		if (iCostumeEquipPlace >= 0 && iCostumeEquipPlace < (int32)ECostumeEquipPlace::ECEP_End)
		{
			FEquippedItemInfo& rEquippedCostumeItemInfo = AllEuippedCostumeInfo.FindOrAdd(iCostumeEquipPlace);
			rEquippedCostumeItemInfo.bIsEquipped = true;
			rEquippedCostumeItemInfo.EquippedItem = Item;
			return;
		}
	}

	switch (Item.InventoryType)
	{
	case EItemInvenType::EIIVT_Weapon:
		StoredWeapons.Add(Item.InstanceUID, Item);
		break;
	case EItemInvenType::EIIVT_Protection:
		StoredProtections.Add(Item.InstanceUID, Item);
		break;
	case EItemInvenType::EIIVT_Accessory:
		StoredAccessries.Add(Item.InstanceUID, Item);
		break;
	case EItemInvenType::EIIVT_CostumeWeapon:
		StoredCostumeWeapons.Add(Item.InstanceUID, Item);
		break;
	case EItemInvenType::EIIVT_CostumeArmor:
		StoredCostumeArmors.Add(Item.InstanceUID, Item);
		break;
	default:
#if WITH_EDITOR && !PLATFORM_MAC
		//checkf(false, TEXT("Received Wrong Item.")); 酒流 肋给等 单捞磐啊 甸绢乐绰 版快啊 缴缴摹 臼霸 乐绰 淀? 农贰浆 措脚 皋矫瘤 冠胶肺 窍磊
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("货肺 眠啊窍绰 酒捞袍狼 InventoryType 捞 捞惑窍促. %d. 哪腔磐啊 梆 气惯茄促."), (int32)Item.InventoryType)
		));
#endif		
		break;
	}

	if (StoredItemCounts.Find(Item.ItemRefID))
		StoredItemCounts[Item.ItemRefID] += 1;
	else
		StoredItemCounts.Add(Item.ItemRefID, 1);
}

bool FPerCharacterItemContainer::RemoveItem(int64 ItemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_RemoveItem);
	for (auto& EquippedInfo : AllEuippedInfo)
	{
		if (EquippedInfo.bIsEquipped && ItemUID == EquippedInfo.EquippedItem.InstanceUID)
		{
			EquippedInfo.bIsEquipped = false;
			EquippedInfo.EquippedItem = FB2Item();

			return true;
		}
	}

	FB2Item *FoundItem = FindItem(ItemUID);
	if (FoundItem)
	{
		if (StoredItemCounts.Find(FoundItem->ItemRefID))
			StoredItemCounts[FoundItem->ItemRefID] -= 1;
		else
			StoredItemCounts.Add(FoundItem->ItemRefID, 0);
	}

	return StoredWeapons.Remove(ItemUID) > 0 || StoredProtections.Remove(ItemUID) > 0 || StoredAccessries.Remove(ItemUID) > 0
		|| StoredCostumeWeapons.Remove(ItemUID) > 0 || StoredCostumeArmors.Remove(ItemUID) > 0;
}

FB2Item* FPerCharacterItemContainer::FindItem(int64 ItemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_FindItem);
	FB2Item* FoundItem = StoredWeapons.Find(ItemUID);
	if (FoundItem)
		return FoundItem;

	FoundItem = StoredProtections.Find(ItemUID);
	if (FoundItem)
		return FoundItem;

	FoundItem = StoredAccessries.Find(ItemUID);
	if (FoundItem)
		return FoundItem;

	FoundItem = StoredCostumeWeapons.Find(ItemUID);
	if (FoundItem)
		return FoundItem;

	FoundItem = StoredCostumeArmors.Find(ItemUID);
	if (FoundItem)
		return FoundItem;

	//厘馒 酒捞袍
	for (int32 i = 0; i < EItemEquipPlaceToUINT(EItemEquipPlace::EIEP_End); ++i)
	{
		if (AllEuippedInfo[i].bIsEquipped && AllEuippedInfo[i].EquippedItem.InstanceUID == ItemUID)
			return &(AllEuippedInfo[i].EquippedItem);
	}
	//厘馒 内胶片
	for (auto& Elem : AllEuippedCostumeInfo)
	{
		FEquippedItemInfo& EquippedCostumeItemInfo = Elem.Value;
		if (EquippedCostumeItemInfo.bIsEquipped && EquippedCostumeItemInfo.EquippedItem.InstanceUID == ItemUID)
		{
			return &(EquippedCostumeItemInfo.EquippedItem);
		}
	}

	return NULL;
}

void FPerCharacterItemContainer::EquipItem(FB2Item* Item, bool bEquip, int32 presetID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_EquipItem);
	check(Item->bCurrentlyEquipped != bEquip);
	check(!CheckConsumeableItem(Item->InventoryType));

	FB2Item ToAddBackup = *Item; // 酒贰俊辑 Remove 唱 equip 秦力窍搁辑 单捞磐啊 檬扁拳瞪 巴捞骨肺 归诀秦 狄.

	const uint32 EquipPlaceUint = EItemEquipPlaceToUINT(Item->EquipPlace);

	if (EquipPlaceUint >= 0 && EquipPlaceUint < (int32)EItemEquipPlace::EIEP_End)
	{
		if (bEquip)
		{
			RemoveItem(Item->InstanceUID); // 捞扒 Stored 府胶飘俊辑 remove 窍妨绰 狼档捞骨肺 Equip 捞 登扁 傈俊 remove 何磐 秦 淋.

			ToAddBackup.bCurrentlyEquipped = true; // AddItem 矫啊 酒聪搁 bCurrentlyEquipped 甫 流立 技泼秦淋.
			AllEuippedInfo[EquipPlaceUint].bIsEquipped = true;
			AllEuippedInfo[EquipPlaceUint].EquippedItem = ToAddBackup;

			// update item preset
			UpdateItemPreset(presetID, ToAddBackup.EquipPlace, ToAddBackup.InstanceUID);
		}
		else
		{
			ToAddBackup.bCurrentlyEquipped = false;
			AllEuippedInfo[EquipPlaceUint].bIsEquipped = false;
			AllEuippedInfo[EquipPlaceUint].EquippedItem = FB2Item();

			// update item preset
			// AddItem俊辑 Preset俊 乐绰瘤俊 眉农窍扁 锭巩俊 AddItem 焊促 刚历 龋免秦具窃.
			UpdateItemPreset(presetID, ToAddBackup.EquipPlace, 0);

			AddItem(ToAddBackup);
		}
	}

	//内胶片 厘馒 眉农
	const uint32 iCostumeEquipPlace = static_cast<uint32>(Item->CostumeEquipPlace);
	if (iCostumeEquipPlace > (int32)ECostumeEquipPlace::ECEP_None && iCostumeEquipPlace < (int32)ECostumeEquipPlace::ECEP_End)
	{
		if (bEquip)
		{
			RemoveItem(Item->InstanceUID); // 捞扒 Stored 府胶飘俊辑 remove 窍妨绰 狼档捞骨肺 Equip 捞 登扁 傈俊 remove 何磐 秦 淋.

			ToAddBackup.bCurrentlyEquipped = true; // AddItem 矫啊 酒聪搁 bCurrentlyEquipped 甫 流立 技泼秦淋.

			FEquippedItemInfo& rEquippedCostumeItemInfo = AllEuippedCostumeInfo.FindOrAdd(iCostumeEquipPlace);
			rEquippedCostumeItemInfo.bIsEquipped = true;
			rEquippedCostumeItemInfo.EquippedItem = ToAddBackup;

			// update item preset
			UpdateItemPreset(presetID, ToAddBackup.EquipPlace, ToAddBackup.InstanceUID);
		}
		else
		{
			ToAddBackup.bCurrentlyEquipped = false;

			FEquippedItemInfo& rEquippedCostumeItemInfo = AllEuippedCostumeInfo.FindOrAdd(iCostumeEquipPlace);
			rEquippedCostumeItemInfo.bIsEquipped = false;
			rEquippedCostumeItemInfo.EquippedItem = FB2Item();

			// update item preset
			// AddItem俊辑 Preset俊 乐绰瘤俊 眉农窍扁 锭巩俊 AddItem 焊促 刚历 龋免秦具窃.
			UpdateItemPreset(presetID, ToAddBackup.EquipPlace, 0);

			AddItem(ToAddBackup);
		}
	}
}

void FPerCharacterItemContainer::ResetEquipedInfoPart(EItemEquipPlace EquipPlace)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_ResetEquipedInfoPart);
	const uint32 EquipPlaceUint = EItemEquipPlaceToUINT(EquipPlace);
	if (EquipPlaceUint >= 0 && EquipPlaceUint < (int32)EItemEquipPlace::EIEP_End)
	{
		if (AllEuippedInfo[EquipPlaceUint].bIsEquipped)
		{
			AllEuippedInfo[EquipPlaceUint].EquippedItem.bCurrentlyEquipped = false;

			bool CheckPreset = false;
			AddItem(AllEuippedInfo[EquipPlaceUint].EquippedItem, CheckPreset);
		}

		AllEuippedInfo[EquipPlaceUint].bIsEquipped = false;
		AllEuippedInfo[EquipPlaceUint].EquippedItem = FB2Item();
	}
}

void FPerCharacterItemContainer::SwitchPresetItem(FB2Item* Item)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_SwitchPresetItem);
	check(!CheckConsumeableItem(Item->InventoryType));

	FB2Item ToAddBackup = *Item;

	RemoveItem(Item->InstanceUID);

	ToAddBackup.bCurrentlyEquipped = true;
	AllEuippedInfo[EItemEquipPlaceToUINT(Item->EquipPlace)].bIsEquipped = true;
	AllEuippedInfo[EItemEquipPlaceToUINT(Item->EquipPlace)].EquippedItem = ToAddBackup;
}

void FPerCharacterItemContainer::UpdateItemMasterDataOnMD()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_UpdateItemMasterDataOnMD);
	for (auto& EquippedInfo : AllEuippedInfo)
	{
		EquippedInfo.EquippedItem.UpdateItemMasterDataOnMD();
	}
}

void FPerCharacterItemContainer::UpdateItemPreset(int32 presetID, EItemEquipPlace equipPlace, int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_UpdateItemPreset);
	if (presetID < 0)
	{
		// apply for all preset
		for (auto Preset : AllItemPresets)
		{
			Preset.Value.AddItem(equipPlace, itemUID);
		}
	}
	else
	{
		auto FoundPreset = AllItemPresets.Find(presetID);
		if (FoundPreset == nullptr)
		{
			FItemPresetInfo newPreset(presetID);
			newPreset.AddItem(equipPlace, itemUID);
			AllItemPresets.Add(presetID, newPreset);
		}
		else
		{
			FoundPreset->AddItem(equipPlace, itemUID);
		}
	}
}

bool FPerCharacterItemContainer::IsInCurrentItemPreset(int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_IsInCurrentItemPreset);
	auto FoundPreset = AllItemPresets.Find(CurrentItemPresetID);
	if (FoundPreset == nullptr)
	{
		return false;
	}
	else
	{
		return FoundPreset->IsEquippedItem(itemUID);
	}
}

bool FPerCharacterItemContainer::IsEquippedCostume(const FB2Item& Item)
{
	FEquippedItemInfo* EquippedInfo = AllEuippedCostumeInfo.Find((int32)Item.CostumeEquipPlace);
	if (EquippedInfo != nullptr)
		return EquippedInfo->bIsEquipped && EquippedInfo->EquippedItem.InstanceUID == Item.InstanceUID;

	return false;
}

bool FPerCharacterItemContainer::IsInItemPreset(int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_IsInItemPreset);
	return (GetContainedItemPresetNum(itemUID) > 0);
}

int32 FPerCharacterItemContainer::GetContainedItemPresetNum(int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_GetContainedItemPresetNum);
	int32 AllCounted = 0;
	for (auto PresetPair : AllItemPresets)
	{
		auto& Preset = PresetPair.Value;
		if (Preset.IsEquippedItem(itemUID))
		{
			++AllCounted;
		}
	}
	return AllCounted;
}

bool FPerCharacterItemContainer::IsInItemPreset(int32 presetID, int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_IsInItemPreset);
	if (AllItemPresets.Num() == 0)
	{
		return false;
	}
	else
	{
		auto* Preset = AllItemPresets.Find(presetID);
		return Preset ? Preset->IsEquippedItem(itemUID) : false;
	}

	return false;
}

bool FPerCharacterItemContainer::IsInOtherItemPreset(int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_IsInOtherItemPreset);
	if (AllItemPresets.Num() > 0)
	{
		bool IsInPreset = false;
		for (auto PresetPair : AllItemPresets)
		{
			auto& Preset = PresetPair.Value;
			// 泅犁 橇府悸 寇 厘馒吝牢 惑怕肺 眉农..
			if (Preset.PresetID != CurrentItemPresetID && Preset.IsEquippedItem(itemUID))
			{
				return true;
			}
		}
	}

	return false;
}

void FPerCharacterItemContainer::RemoveCurrentItemPreset(int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_RemoveCurrentItemPreset);
	auto FoundPreset = AllItemPresets.Find(CurrentItemPresetID);
	if (FoundPreset != nullptr)
	{
		FoundPreset->RemoveItem(itemUID);
	}
}

void FPerCharacterItemContainer::RemoveAllItemPreset(int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_RemoveAllItemPreset);
	if (AllItemPresets.Num() > 0)
	{
		for (auto PresetPair : AllItemPresets)
		{
			auto& Preset = PresetPair.Value;
			Preset.RemoveItem(itemUID);
		}
	}
}

int32 FPerCharacterItemContainer::GetContainItemCount(int32 InRefID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_GetContainItemCount);
	if (StoredItemCounts.Find(InRefID))
		return StoredItemCounts[InRefID];

	return 0;
}

void FPerCharacterItemContainer::SetCurrentItemPresetID(int32 presetID)
{
	CurrentItemPresetID = presetID;
}

int32 FPerCharacterItemContainer::GetContainsPresetID(int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPerCharacterItemContainer_GetContainsPresetID);
	if (AllItemPresets.Num() == 0)
	{
		return -1;
	}
	else
	{
		for (auto PresetPair : AllItemPresets)
		{
			auto& Preset = PresetPair.Value;
			if (Preset.IsEquippedItem(itemUID))
			{
				return Preset.PresetID;
			}
		}
	}

	return -1;
}

void FPCItemContainer::AddItem(const FB2Item& Item)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_AddItem);
	if (CheckConsumeableItem(Item.InventoryType))
		ConsumableItems.Add(Item.InstanceUID, Item);
	else
	{
		if (CharacterItemContainers.IsValidIndex(Item.AllowedPCClass))
		{
			CharacterItemContainers[Item.AllowedPCClass].AddItem(Item);
		}
	}
}

void FPCItemContainer::AddItems(const TArray<FB2Item>& Items)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_AddItems);
	for (auto& Item : Items)
	{
		AddItem(Item);
	}
}

void FPCItemContainer::AddNewAcquiredItems(const TArray<FB2Item>& NewItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_AddNewAcquiredItems);
	TArray<FB2Item> FinalAddList;
	for (const FB2Item& ThisItem : NewItems)
	{
		FB2Item ThisToAdd = ThisItem;

		if (ThisToAdd.ItemClass == EItemClass::EIC_WeaponEssence ||
			ThisToAdd.ItemClass == EItemClass::EIC_ProtectionEssence ||
			ThisToAdd.ItemClass == EItemClass::EIC_AccessoryEssence ||
			ThisToAdd.ItemClass == EItemClass::EIC_WeaponSurpassAnvil ||
			ThisToAdd.ItemClass == EItemClass::EIC_ArmorSurpassAnvil ||
			ThisToAdd.ItemClass == EItemClass::EIC_AccessorySurpassAnvil
			)
		{
			ThisToAdd.bNewlyAddedForEquipSuggest = false;				//沥荐绰 剁况林瘤 富磊
		}
		else
			ThisToAdd.bNewlyAddedForEquipSuggest = true; // 厘馒 鼻厘 眉农 侩. 眠饶 牢亥配府俊辑 CheckForBetterEquipSuggest 芭摹搁辑 府悸凳.
														 //BladeIIGameImpl::GetRedDotManager().AddNewItem(ThisToAdd); // 捞巴档 bNewlyAddedForEquipSuggest 尔 厚搅窍变 茄单 捞扒 牢亥配府 徘 UI 俊 New 钎矫窍扁 困茄 芭.
		FinalAddList.Add(ThisToAdd);
	}

	AddItems(FinalAddList);
}

void FPCItemContainer::AddNewAcquiredItem(FB2Item InItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_AddNewAcquiredItem);

	if (InItem.ItemClass == EItemClass::EIC_WeaponEssence ||
		InItem.ItemClass == EItemClass::EIC_ProtectionEssence ||
		InItem.ItemClass == EItemClass::EIC_AccessoryEssence ||
		InItem.ItemClass == EItemClass::EIC_WeaponSurpassAnvil ||
		InItem.ItemClass == EItemClass::EIC_ArmorSurpassAnvil ||
		InItem.ItemClass == EItemClass::EIC_AccessorySurpassAnvil
		)
	{
		InItem.bNewlyAddedForEquipSuggest = false;				//沥荐绰 剁况林瘤 富磊
	}
	else
	{
		InItem.bNewlyAddedForEquipSuggest = true; // 厘馒 鼻厘 眉农 侩. 眠饶 牢亥配府俊辑 CheckForBetterEquipSuggest 芭摹搁辑 府悸凳.
												  //BladeIIGameImpl::GetRedDotManager().AddNewItem(ThisToAdd); // 捞巴档 bNewlyAddedForEquipSuggest 尔 厚搅窍变 茄单 捞扒 牢亥配府 徘 UI 俊 New 钎矫窍扁 困茄 芭.
	}

	AddItem(InItem);
}

bool FPCItemContainer::RemoveItem(const int64& ToRemoveItemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_RemoveItem_int64);
	if (ConsumableItems.Remove(ToRemoveItemUID) > 0)
		return true;

	for (auto& CharacterItemContainer : CharacterItemContainers)
	{
		if (CharacterItemContainer.RemoveItem(ToRemoveItemUID))
			return true;
	}

	return false;
}

bool FPCItemContainer::RemoveItem(const FB2ItemUID& ToRemoveItemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_RemoveItem);
	return RemoveItem(ToRemoveItemUID.UID);
}

void FPCItemContainer::RemoveItems(const TArray<FB2Item>& ToRemoveItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_RemoveItems);
	for (auto& Item : ToRemoveItems)
	{
		RemoveItem(Item.InstanceUID);
	}
}

void FPCItemContainer::RemoveItems(const TArray<int64>& ToRemoveItemUIDs_int64)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_RemoveItems_int64);
	for (int64 UID : ToRemoveItemUIDs_int64)
	{
		RemoveItem(UID);
	}
}


void FPCItemContainer::DecSharedConsumableAmountOfType(int32 InRefID, int32 InDecNum) // 皑家父 矫糯
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_DecSharedConsumableAmountOfType_Decrease);
	checkSlow(InDecNum >= 0); // 澜荐肺 霖促绊 刘啊肺 贸府窍绰 芭 酒丛.

							  // 家葛前 皑家登绰 览翠狼 版快 辑滚俊辑 函版等 酒捞袍 单捞磐唱 力芭瞪 酒捞袍 浇吩 鞍篮 芭 焊郴临 波扁 锭巩俊 措眉肺绰 捞 扁瓷捞 鞘夸绝阑 巴捞促.
							  // 抗甫 甸绢 FClientDataStore::OnResponseConsumableAmountDecrease 捞吧肺 贸府啊 等促芭唱..
							  // 滚辟瘤 酒流 备泅 救等 扒瘤 酒聪搁 傲 弊繁扒瘤 隔扼档 酒囱 版快啊 乐绢辑..

	TArray<FB2Item> AllFound;
	FindConsumablesOfType(InRefID, AllFound);

	// ConsumingAmount 利篮 芭 何磐 炼例且 波扁 锭巩俊 家泼阑 茄促. 咯矾俺老 锭 滴锅掳 波 捞惑篮 弥措 箭磊咯具..
	// 辟单 氢饶肺绰 傍侩前 浇吩 俺荐 力茄捞 荤角惑 绝绢龙 芭扼 乐绢档 窍唱老 波烙.
	struct FConsumingAmountSorter
	{
		bool operator()(const FB2Item& A, const FB2Item& B) const
		{
			return A.ConsumingAmount < B.ConsumingAmount;
		}
	};
	AllFound.Sort(FConsumingAmountSorter());

	int32 RemainigDecNum = InDecNum;
	TArray<int64> ToRemoveID; // ConsumingAmount 啊 - 肺 郴妨啊辑 酒抗 力芭秦具 窍绰 巴甸 葛澜
	for (int32 CI = 0; CI < AllFound.Num(); ++CI)
	{
		FB2Item& ThisAdjust = AllFound[CI];

		if (ThisAdjust.ConsumingAmount > RemainigDecNum)
		{
			ThisAdjust.ConsumingAmount -= RemainigDecNum;
			ChangeItem(ThisAdjust);
			RemainigDecNum = 0;
			break;
		}
		else
		{ // 捞锅 波 浇吩捞 力芭凳.
			RemainigDecNum -= ThisAdjust.ConsumingAmount;
			ThisAdjust.ConsumingAmount = 0;
			ToRemoveID.Add(ThisAdjust.InstanceUID);
		}
	}

	RemoveItems(ToRemoveID);
}

void FPCItemContainer::IncSharedConsumableAmountOfType(int32 InRefID, int32 InIncNum) // 刘啊父..
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_DecSharedConsumableAmountOfType_Increase);
	checkSlow(InIncNum >= 0); // 澜荐肺 霖促绊 皑家肺 贸府窍绰 芭 酒丛.

							  // DecSharedConsumableAmountOfType 档 弊犯瘤父 捞扒 漂洒唱 流立 荤侩窍绰 惑炔捞 弊府 官恩流茄 霸 酒聪促.
							  // 酒流 东龙捞 固贸 救等 内靛甸档 乐绊 秦辑 父甸绢绰 狄..

	TArray<FB2Item> AllFound;
	FindConsumablesOfType(InRefID, AllFound);

	if (AllFound.Num() > 0)
	{ // 鞘夸俊 蝶扼 Sort 甫 秦 焊带瘤.. 辟单 扁裙俊 蝶福搁 傍侩前 浇吩付促 俺荐 力茄捞 绝阑 波扼 弊贰辑.. 傲 唱柯 芭 窍唱父 箭磊 刘啊矫虐搁 瞪 淀
		AllFound[0].ConsumingAmount += InIncNum;
		ChangeItem(AllFound[0]);
	}
	else // 酒抗 绝促啊 货肺 积扁绰 版快.. 漂洒唱 捞繁 版快扼搁 促弗 侥栏肺 甸绢客具 茄促.
	{// 恐衬窍搁 促弗 单捞磐绰 隔扼档 UID 绰 努扼捞攫飘俊辑 何咯且 荐 绝扁 锭巩捞促.
	 // 咯窍埃 烙矫肺扼档 贸府秦 霖促 摹磊. 
		ensureMsgf(0, TEXT("Shared consumables amount better not be simply increased! Server protocol must send full data struct having proper UID!"));
		FB2Item NewItemSlot;
		NewItemSlot.InventoryType = EItemInvenType::EIIVT_Consumables;
		NewItemSlot.ItemRefID = InRefID;
		NewItemSlot.ConsumingAmount = InIncNum;
		AddItem(NewItemSlot);
	}
}

FB2Item* FPCItemContainer::FindItem(const FB2ItemUID& ItemUID)
{
	return FindItem(ItemUID.UID);
}

FB2Item* FPCItemContainer::FindItem(const int64& ItemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_FindItem_int64);
	FB2Item* FoundItem = nullptr;
	for (auto& ChacaterItemContainer : CharacterItemContainers)
	{
		FoundItem = ChacaterItemContainer.FindItem(ItemUID);
		if (FoundItem)
			break;

		FoundItem = ConsumableItems.Find(ItemUID);
		if (FoundItem)
			break;
	}

	return FoundItem;
}

void FPCItemContainer::FindConsumablesOfType(int32 InConsumableTypeRefID, TArray<FB2Item>& OutResults)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_FindConsumablesOfType);
	for (TMap<int64, FB2Item>::TIterator CII(ConsumableItems); CII; ++CII)
	{
		FB2Item& ThisOne = CII.Value();
		// 焊包窃 酒捞袍 甘 虐啊 UID 牢单 漂沥 鸥涝狼 傍侩? 家葛前甸阑 茫绰 单俊绰 狼固啊 绝促. iteration 档绰 荐 观俊.. 
		if (ThisOne.ItemRefID == InConsumableTypeRefID)
		{
			OutResults.Add(ThisOne);
		}
	}
}

int32 FPCItemContainer::FindItemOfTypeCount(int32 InRefID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_FindItemOfTypeCount);
	int32 Count = 0;
	for (auto& CharacterItemContainer : CharacterItemContainers)
	{
		Count += CharacterItemContainer.GetContainItemCount(InRefID);
	}
	return Count;
}

void FPCItemContainer::EquipItems(const TArray<int64>& Items, bool bEquip, int32 presetID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_EquipItems);
	for (auto& ItemUID : Items)
	{
		FB2Item* FoundItem = FindItem(ItemUID);
		if (FoundItem && FoundItem->bCurrentlyEquipped != bEquip)
		{
			int32 PCClassIndex = FoundItem->AllowedPCClass;
			checkSlow(CharacterItemContainers.IsValidIndex(PCClassIndex));
			if (CharacterItemContainers.IsValidIndex(PCClassIndex)) {
				CharacterItemContainers[PCClassIndex].EquipItem(FoundItem, bEquip, presetID);
			}
		}
		else if (FoundItem)
		{
			int32 PCClassIndex = FoundItem->AllowedPCClass;
			checkSlow(CharacterItemContainers.IsValidIndex(PCClassIndex));
			// 促弗 橇府悸俊 厘馒等 厘厚绰 秦力 惑怕肺 函版.
			if (CharacterItemContainers.IsValidIndex(PCClassIndex)) {
				CharacterItemContainers[PCClassIndex].UpdateItemPreset(presetID, FoundItem->EquipPlace, 0);
			}
		}
	}
}

void FPCItemContainer::UnEquipItemsPresets(const TArray<int64>& Items)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_UnEquipItemsPresets);
	//const int32 MaxPreset = BladeIIGameImpl::GetClientDataStore().GetItemPresetMaxCount();
	//for (auto& ItemUID : Items)
	//{
	//	FB2Item* FoundItem = FindItem(ItemUID);
	//	if (FoundItem)
	//	{
	//		int32 PCClassIndex = FoundItem->AllowedPCClass;
	//		checkSlow(CharacterItemContainers.IsValidIndex(PCClassIndex));
	//		if (CharacterItemContainers.IsValidIndex(PCClassIndex))
	//		{
	//			for (int32 i = 0; i < MaxPreset; ++i)
	//			{
	//				if (CharacterItemContainers[PCClassIndex].IsInItemPreset(i, FoundItem->InstanceUID))
	//					CharacterItemContainers[PCClassIndex].UpdateItemPreset(i, FoundItem->EquipPlace, 0);
	//			}
	//			CharacterItemContainers[PCClassIndex].ResetEquipedInfoPart(FoundItem->EquipPlace);
	//		}
	//	}
	//}
}

void FPCItemContainer::SwitchPresetItems(const TArray<int64>& Items, EPCClass ePCClass, int32 presetID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_SwitchPresetItems);
	for (auto& ItemUID : Items)
	{
		FB2Item* FoundItem = FindItem(ItemUID);
		if (FoundItem && CharacterItemContainers.IsValidIndex(FoundItem->AllowedPCClass))
		{
			CharacterItemContainers[FoundItem->AllowedPCClass].SwitchPresetItem(FoundItem);
		}
	}
}
void FPCItemContainer::LockItems(const TArray<FB2Item>& Items, bool bLock)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_LockItems);
	for (auto& Item : Items)
	{
		FB2Item* FoundItem = FindItem(Item.InstanceUID);
		if (FoundItem)
		{
			FoundItem->bIsLocked = bLock;
		}
	}
}

void FPCItemContainer::ChangeItem(const FB2Item& ChangedItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_ChangeItem);
	RemoveItem(ChangedItem.InstanceUID);
	AddItem(ChangedItem);
}

void FPCItemContainer::ChangeItems(const TArray<FB2Item>& Items)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_ChangeItems);
	for (auto& Item : Items)
	{
		RemoveItem(Item.InstanceUID);
		AddItem(Item);
	}
}

void FPCItemContainer::ResetAllItems(EPCClass ePCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_ResetAllItems);
	if (CharacterItemContainers.IsValidIndex(PCClassToInt(ePCClass)))
	{
		for (int32 i = (int32)EItemEquipPlace::EIEP_Weapon; i < (int32)EItemEquipPlace::EIEP_End; ++i)
		{
			CharacterItemContainers[PCClassToInt(ePCClass)].ResetEquipedInfoPart((EItemEquipPlace)i);
		}
	}
}

void FPCItemContainer::UpdateItemMasterDataOnMD()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_UpdateItemMasterDataOnMD);
	for (auto& Item : ConsumableItems)
	{
		Item.Value.UpdateItemMasterDataOnMD();
	}


	for (auto& CharacterItemContainer : CharacterItemContainers)
	{
		CharacterItemContainer.UpdateItemMasterDataOnMD();
	}
}

void FPCItemContainer::UpdateItemPresets(const TArray<FB2ItemPresetInfo>& ItemPresets)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_UpdateItemPresets);
	for (auto ItemPreset : ItemPresets)
	{
		auto CharacterType = SvrToCliPCClassType(ItemPreset->character_type);
		auto EquipPlace = SvrToCliItemEquipPlace(ItemPreset->equip_position);

		if (CharacterItemContainers.IsValidIndex((int32)CharacterType))
		{
			CharacterItemContainers[(int32)CharacterType].UpdateItemPreset(ItemPreset->preset_id, EquipPlace, ItemPreset->item_id);
		}
	}
}

bool FPCItemContainer::IsInItemPresets(EPCClass ePCClass, int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_IsInItemPresets);
	int32 CastedPCClassInt = PCClassToInt(ePCClass);
	return CharacterItemContainers.IsValidIndex(CastedPCClassInt) ? CharacterItemContainers[CastedPCClassInt].IsInItemPreset(itemUID) : false;
}

bool FPCItemContainer::IsInOtherItemPresets(EPCClass ePCClass, int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_IsInOtherItemPresets);
	int32 CastedPCClassInt = PCClassToInt(ePCClass);
	return CharacterItemContainers.IsValidIndex(CastedPCClassInt) ? CharacterItemContainers[CastedPCClassInt].IsInOtherItemPreset(itemUID) : false;
}

int32 FPCItemContainer::GetContainedItemPresetNum(EPCClass ePCClass, int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_GetContainedItemPresetNum);
	int32 CastedPCClassInt = PCClassToInt(ePCClass);
	return CharacterItemContainers.IsValidIndex(CastedPCClassInt) ? CharacterItemContainers[CastedPCClassInt].GetContainedItemPresetNum(itemUID) : false;
}

bool FPCItemContainer::IsInCurrentItemPreset(EPCClass ePCClass, int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_IsInCurrentItemPreset);
	if (CharacterItemContainers.IsValidIndex(PCClassToInt(ePCClass)) == false)
		return false;

	return CharacterItemContainers[PCClassToInt(ePCClass)].IsInCurrentItemPreset(itemUID);
}

bool FPCItemContainer::IsEquippedCostume(const FB2Item& Item)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_IsEquippedCostume);
	EPCClass ePCClass = IntToPCClass(Item.AllowedPCClass);

	if (CharacterItemContainers.IsValidIndex(PCClassToInt(ePCClass)) == false)
		return false;

	return CharacterItemContainers[PCClassToInt(ePCClass)].IsEquippedCostume(Item);
}

int32 FPCItemContainer::GetCurrentItemPresetID(EPCClass ePCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_GetCurrentItemPresetID);
	if (CharacterItemContainers.IsValidIndex(PCClassToInt(ePCClass)) == false)
		return -1;

	return CharacterItemContainers[PCClassToInt(ePCClass)].CurrentItemPresetID;
}

int32 FPCItemContainer::GetContainsItemPresetID(EPCClass ePCClass, int64 itemUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_GetContainsItemPresetID);
	if (CharacterItemContainers.IsValidIndex(PCClassToInt(ePCClass)) == false)
		return -1;

	return CharacterItemContainers[PCClassToInt(ePCClass)].GetContainsPresetID(itemUID);
}

void FPCItemContainer::SetCurrentItemPresetID(EPCClass ePCClass, int32 presetID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FPCItemContainer_SetCurrentItemPresetID);

	const int32 PCClassNum = PCClassToInt(ePCClass);
	if (CharacterItemContainers.IsValidIndex(PCClassNum) == false)
		return;

	CharacterItemContainers[PCClassNum].SetCurrentItemPresetID(presetID);
}

FItemLevelupFactorMap::FItemLevelupFactorMap()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FItemLevelupFactorMap_FItemLevelupFactorMap);
	// 辑滚俊辑 单捞磐 罐扁 傈狼 扁夯 蔼.

	// 喉饭捞靛2_酒捞袍碍拳_抛捞喉.xlsm狼 itemLevelupExps 徘狼 mainItemGrade 曼绊
	TargetItemGradeToFactor.Add(41602);		// 0, 歹固
	TargetItemGradeToFactor.Add(41602);		// 1
	TargetItemGradeToFactor.Add(14992);		// 2
	TargetItemGradeToFactor.Add(5065);		// 3
	TargetItemGradeToFactor.Add(1574);		// 4
	TargetItemGradeToFactor.Add(370);		// 5
	TargetItemGradeToFactor.Add(100);		// 6
	TargetItemGradeToFactor.Add(54);		// 7
	TargetItemGradeToFactor.Add(28);		// 8
	TargetItemGradeToFactor.Add(14);		// 9
	TargetItemGradeToFactor.Add(2);			// 10
	TargetItemGradeToFactor.Add(1);			// 11 
	TargetItemGradeToFactor.Add(1);			// 12
											// 喉饭捞靛2_酒捞袍碍拳_抛捞喉.xlsm狼 itemLevelupExps 徘狼 materialItemGrade 曼绊
	IngredItemGradeToFactor.Add(0.0087);	// 0, 歹固
	IngredItemGradeToFactor.Add(0.0087);	// 1
	IngredItemGradeToFactor.Add(0.027);		// 2
	IngredItemGradeToFactor.Add(0.0839);	// 3
	IngredItemGradeToFactor.Add(0.2601);	// 4
	IngredItemGradeToFactor.Add(0.8064);	// 5
	IngredItemGradeToFactor.Add(2.5);		// 6
	IngredItemGradeToFactor.Add(7.75);		// 7
	IngredItemGradeToFactor.Add(15.5);		// 8
	IngredItemGradeToFactor.Add(31);		// 9
	IngredItemGradeToFactor.Add(145);		// 10
	IngredItemGradeToFactor.Add(215);		// 11
	IngredItemGradeToFactor.Add(345);		// 12

	for (int32 EPI = 0; EPI < (int32)EItemEquipPlace::EIEP_End; ++EPI)
	{
		// 喉饭捞靛2_酒捞袍碍拳_抛捞喉.xlsm狼 itemLevelupExps 徘狼 equipPosition 曼绊
		switch (static_cast<EItemEquipPlace>(EPI))
		{
		case EItemEquipPlace::EIEP_Weapon:
			IngredItemEquipPosToFactor.Add(0.7f);
			break;

		case EItemEquipPlace::EIEP_Helmet:
		case EItemEquipPlace::EIEP_BodyArmor:
		case EItemEquipPlace::EIEP_Glove:
		case EItemEquipPlace::EIEP_Shoes:
			IngredItemEquipPosToFactor.Add(1.2f);
			break;

		case EItemEquipPlace::EIEP_Belt:
		case EItemEquipPlace::EIEP_Necklace:
		case EItemEquipPlace::EIEP_Ring:
			IngredItemEquipPosToFactor.Add(1.1f);
			break;
		}
	}

	for (int32 ELI = 0; ELI <= 70; ++ELI)			// 喉饭捞靛2_酒捞袍碍拳_抛捞喉.xlsm狼 itemLevelupExps 徘狼 materialLevel 曼绊, 0篮 歹固
	{
		IngredItemLevelupToFactor.Add(1.0f + (float)(ELI - 1) * 0.35f);
	}

	for (int32 SCI = 0; SCI <= 40; ++SCI)			// 喉饭捞靛2_酒捞袍碍拳_抛捞喉.xlsm狼 itemLevelupExps 徘俊 surpassCount 曼绊
	{
		IngredItemSurpassCountToFactor.Add(1.0f + (float)(SCI) * 0.2f);
	}

	for (int32 IOI = 0; IOI < (int32)EItemOption::EIO_End; ++IOI)
	{ // EItemOption enum 捞 鉴瞒利牢 蔼捞绢具 角力 可记 辆幅父怒狼 element 啊 甸绢啊霸 瞪 巴.
		TargetItemOptionIncFactor.Add(static_cast<EItemOption>(IOI), FitemOptionValue(1.0f, 0.f));
	}
}

void FItemLevelupFactorMap::UpdateByServerSync(const TArray<b2network::B2mdItemLevelupExpFactorPtr>& ServerExpFactor, const TArray<b2network::B2mdItemOptionAddValuePtr>& ServerOptionFactor)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FItemLevelupFactorMap_UpdateByServerSync);
	BII_CHECK( // 辑滚客 努扼 埃 单捞磐 牢盔 ?
			   // 努扼 单捞磐甸篮 歹固蔼牢 0 何磐 持绰 巴甸捞 乐绢辑 3阑 猾促.
		(TargetItemGradeToFactor.Num() + IngredItemGradeToFactor.Num() + IngredItemEquipPosToFactor.Num() + IngredItemLevelupToFactor.Num() + IngredItemSurpassCountToFactor.Num() - 3)
		== ServerExpFactor.Num()
	);

	for (int32 SEI = 0; SEI < ServerExpFactor.Num(); ++SEI)
	{
		const b2network::B2mdItemLevelupExpFactorPtr& CurrFactor = ServerExpFactor[SEI];

		if (CurrFactor->type_name == TEXT("mainItemGrade"))
		{ // 0锅篮 歹固
			BII_CHECK(TargetItemGradeToFactor.IsValidIndex(CurrFactor->key_code));
			if (TargetItemGradeToFactor.IsValidIndex(CurrFactor->key_code))
			{
				TargetItemGradeToFactor[CurrFactor->key_code] = CurrFactor->factor;
			}
		}
		else if (CurrFactor->type_name == TEXT("materialItemGrade"))
		{ // 0锅篮 歹固
			BII_CHECK(IngredItemGradeToFactor.IsValidIndex(CurrFactor->key_code));
			if (IngredItemGradeToFactor.IsValidIndex(CurrFactor->key_code))
			{
				IngredItemGradeToFactor[CurrFactor->key_code] = CurrFactor->factor;
			}
		}
		else if (CurrFactor->type_name == TEXT("equipPosition"))
		{
			EItemEquipPlace CliEquipPlace = SvrToCliItemEquipPlace(CurrFactor->key_code);
			int32 CliEquipPlaceInt = (int32)CliEquipPlace;
			BII_CHECK(CliEquipPlace != EItemEquipPlace::EIEP_End && IngredItemEquipPosToFactor.IsValidIndex(CliEquipPlaceInt));
			if (CliEquipPlace != EItemEquipPlace::EIEP_End && IngredItemEquipPosToFactor.IsValidIndex(CliEquipPlaceInt))
			{
				IngredItemEquipPosToFactor[CliEquipPlaceInt] = CurrFactor->factor;
			}
		}
		else if (CurrFactor->type_name == TEXT("materialLevel"))
		{ // 0锅篮 歹固
			BII_CHECK(IngredItemLevelupToFactor.IsValidIndex(CurrFactor->key_code));
			if (IngredItemLevelupToFactor.IsValidIndex(CurrFactor->key_code))
			{
				IngredItemLevelupToFactor[CurrFactor->key_code] = CurrFactor->factor;
			}
		}
		else if (CurrFactor->type_name == TEXT("surpassCount"))
		{
			BII_CHECK(IngredItemSurpassCountToFactor.IsValidIndex(CurrFactor->key_code));
			if (IngredItemSurpassCountToFactor.IsValidIndex(CurrFactor->key_code))
			{
				IngredItemSurpassCountToFactor[CurrFactor->key_code] = CurrFactor->factor;
			}
		}
		else
		{
			BII_CHECK(0);
		}
	}

	// 辑滚客 努扼 埃 单捞磐 牢盔 ? .. 捞芭 瘤陛 肮荐啊 促弗单.. 努扼捞攫飘俊辑绰 蝶肺 包府窍绰 单捞磐甫 辑滚俊辑 鞍篮 单捞磐 硅凯俊 持绢辑 焊郴林绰 淀 窃. 咯扁辑 汗荤窍绰 ItemOption 率 单捞磐 捞寇俊 唱赣瘤 绢痘罢啊 荤侩窍档废 秦具.
	//ensure(TargetItemOptionIncFactor.Num() == ServerOptionFactor.Num()); 
	int32 FoundCount = 0;
	for (int32 SOI = 0; SOI < ServerOptionFactor.Num(); ++SOI)
	{
		const b2network::B2mdItemOptionAddValuePtr& CurrFactor = ServerOptionFactor[SOI];
		EItemOption CurrOption = SvrToCliOptionType(CurrFactor->option_id);
		if (CurrOption != EItemOption::EIO_End)
		{
			FitemOptionValue* ThisFactorPtr = TargetItemOptionIncFactor.Find(CurrOption);
			if (ThisFactorPtr)
			{
				++FoundCount;
				ThisFactorPtr->Add_Value = CurrFactor->add_value;
				ThisFactorPtr->Power_Factor = CurrFactor->power_factor;
			}
		}
	}
	checkSlow(FoundCount == TargetItemOptionIncFactor.Num()); // 措脚 咯扁辑 鞘夸茄 单捞磐绰 救 哗初绊 啊廉客具瘤.
}

bool FItemLevelupCostPerLevel::HasDataForInvenType(EItemInvenType InItemInvenType) const
{ // 捞 FItemLevelupCostPerLevel 阑 碍拳饭骇阑 虐肺 秦辑 甸绊 乐阑 扒单 郴何俊辑 牢亥配府 鸥涝俊 蝶扼 唱吹绢瘤绰 锐茄茄 备炼促 焊聪 捞繁 眉农啊 鞘夸
	BLADE2_SCOPE_CYCLE_COUNTER(FItemLevelupFactorMap_HasDataForInvenType);
	return (
		(InItemInvenType == EItemInvenType::EIIVT_Weapon && CostPerGrade_Weapon.Num() > 0) ||
		(InItemInvenType == EItemInvenType::EIIVT_Protection && CostPerGrade_Protection.Num() > 0) ||
		(InItemInvenType == EItemInvenType::EIIVT_Accessory && CostPerGrade_Accessory.Num() > 0)
		);
}

int32 FItemLevelupCostPerLevel::GetCostForGrade(EItemInvenType InItemInvenType, int32 InGrade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FItemLevelupFactorMap_GetCostForGrade);
	const int32 DesiredIndex = InGrade - 1;
	switch (InItemInvenType)
	{
	case EItemInvenType::EIIVT_Weapon:
		if (CostPerGrade_Weapon.IsValidIndex(DesiredIndex)) {
			return CostPerGrade_Weapon[DesiredIndex];
		}
		break;
	case EItemInvenType::EIIVT_Protection:
		if (CostPerGrade_Protection.IsValidIndex(DesiredIndex)) {
			return CostPerGrade_Protection[DesiredIndex];
		}
		break;
	case EItemInvenType::EIIVT_Accessory:
		if (CostPerGrade_Accessory.IsValidIndex(DesiredIndex)) {
			return CostPerGrade_Accessory[DesiredIndex];
		}
		break;
	}
	return 0;
}

void FItemLevelupCostPerLevel::UpdateByServerSync(const b2network::B2mdItemLevelupCostPtr& ServerItemLevelupCostStruct)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FItemLevelupFactorMap_UpdateByServerSync);
	TArray<int32>* DesiredCostArray = NULL;
	if (ServerItemLevelupCostStruct->inventory_type == b2network::B2InventoryType::Weapon) {
		DesiredCostArray = &CostPerGrade_Weapon;
	}
	else if (ServerItemLevelupCostStruct->inventory_type == b2network::B2InventoryType::Protection) {
		DesiredCostArray = &CostPerGrade_Protection;
	}
	else if (ServerItemLevelupCostStruct->inventory_type == b2network::B2InventoryType::Accessory) {
		DesiredCostArray = &CostPerGrade_Accessory;
	}

	if (DesiredCostArray)
	{
		DesiredCostArray->Empty();

		// Grade 喊肺 鉴辑措肺. level 篮 咯扁辑 眉农窍绰 荤亲捞 酒丛. 
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade1);
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade2);
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade3);
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade4);
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade5);
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade6);
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade7);
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade8);
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade9);
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade10);
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade11);
		DesiredCostArray->Add(ServerItemLevelupCostStruct->price_grade12);

		checkSlow(DesiredCostArray->Num() == FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE); // 趣矫 刘啊窍搁 鳖冈瘤 臼霸..
	}
}

void FResurrectMasterDataElement::UpdateByServerSync(const b2network::B2mdResurrectionInfoPtr& InThisServerInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FItemLevelupFactorMap_UpdateByServerSync);
	if (InThisServerInfo)
	{
		price = InThisServerInfo->price;

		BuffValueMap.Empty();

		EResurrectBuffType BuffType_01 = SvrToCliResurrectBuffType(InThisServerInfo->attribute_change1);
		if (BuffType_01 != EResurrectBuffType::RBT_None || BuffType_01 != EResurrectBuffType::RBT_End)
		{
			BuffValueMap.Add(BuffType_01, InThisServerInfo->change_value1);
		}

		EResurrectBuffType BuffType_02 = SvrToCliResurrectBuffType(InThisServerInfo->attribute_change2);
		if (BuffType_02 != EResurrectBuffType::RBT_None || BuffType_02 != EResurrectBuffType::RBT_End)
		{
			BuffValueMap.Add(BuffType_02, InThisServerInfo->change_value2);
		}

		EResurrectBuffType BuffType_03 = SvrToCliResurrectBuffType(InThisServerInfo->attribute_change3);
		if (BuffType_03 != EResurrectBuffType::RBT_None || BuffType_03 != EResurrectBuffType::RBT_End)
		{
			BuffValueMap.Add(BuffType_03, InThisServerInfo->change_value3);
		}
	}
}

int32 FResurrectMasterDataElement::GetBuffValueOfType(EResurrectBuffType InType)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FItemLevelupFactorMap_GetBuffValueOfType);
	int32* FoundValue = BuffValueMap.Find(InType);
	return FoundValue ? *FoundValue : 0;
}

FClientDataStore::FClientDataStore()
{
	//bReceivedServerMasterData = false;
	//RequestedCharNickNameSetClass = EPCClass::EPC_End;
	//MD_MinEnhanceLevelForSetEffect = 0;
	//MD_MaxEnhanceLevelForSetEffect = 0;

	//InitializeAccountInfo();

	//SubscribeEvents();
}

void FClientDataStore::InitializeAccountInfo()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_InitializeAccountInfo);
	FMemory::Memzero(AccountInfo);

	const int8 IndexMax = GetMaxPCClassNum();

	LocalCharacterData.PlayerSharedStats.Empty(IndexMax);
	LocalCharacterData.UserAllItems.CharacterItemContainers.Empty(IndexMax);

	if (LocalCharacterData.MainPlayerClass == EPCClass::EPC_End
		|| LocalCharacterData.MainPlayerClass == LocalCharacterData.SubPlayerClass)
	{
		LocalCharacterData.MainPlayerClass = EPCClass::EPC_Assassin;
		LocalCharacterData.SubPlayerClass = EPCClass::EPC_Gladiator;
	}
	LocalCharacterData.CharNames.Empty(IndexMax);
	LocalCharacterData.AllWingData.Empty(IndexMax);
	LocalCharacterData.CacheShowPlayerHeads.Empty(IndexMax);

	for (int8 PCI = 0; PCI < IndexMax; ++PCI)
	{ // Player Class 付促 鞘夸茄 单捞磐甸.
		LocalCharacterData.PlayerSharedStats.Add(FB2PlayerSharedStats());
		LocalCharacterData.UserAllItems.CharacterItemContainers.Add(FPerCharacterItemContainer());
		LocalCharacterData.CharNames.Add(FPCClassOrNickName()); // NickName 篮 隔扼档 ClassName 篮 啊瓷茄 弧府 PCClassInfo 俊辑 啊廉客具 窃. 积己磊 矫痢俊辑绰 ClassInfo 甫 茫阑 荐 绝澜.
		LocalCharacterData.AllWingData.Add(FPCWingContainer());
		LocalCharacterData.CacheShowPlayerHeads.Add(true);
	}

	TutorialCharacterData.PlayerSharedStats.Empty(IndexMax);
	TutorialCharacterData.UserAllItems.CharacterItemContainers.Empty(IndexMax);
	TutorialCharacterData.MainPlayerClass = EPCClass::EPC_Gladiator;
	TutorialCharacterData.SubPlayerClass = EPCClass::EPC_Wizard;
	TutorialCharacterData.CharNames.Empty(IndexMax);
	TutorialCharacterData.AllWingData.Empty(IndexMax);

	for (int8 PCI = 0; PCI < IndexMax; ++PCI)
	{ // Player Class 付促 鞘夸茄 单捞磐甸.
		TutorialCharacterData.PlayerSharedStats.Add(FB2PlayerSharedStats());
		TutorialCharacterData.UserAllItems.CharacterItemContainers.Add(FPerCharacterItemContainer());
		TutorialCharacterData.CharNames.Add(FPCClassOrNickName()); // NickName 篮 隔扼档 ClassName 篮 啊瓷茄 弧府 PCClassInfo 俊辑 啊廉客具 窃. 积己磊 矫痢俊辑绰 ClassInfo 甫 茫阑 荐 绝澜.
		TutorialCharacterData.AllWingData.Add(FPCWingContainer());
	}

	m_ServerVersionInfo = FString("");

	//AB2StageManager::GetCacheStageKeepEssentialData().Initialize();
}

void FClientDataStore::SubscribeEvents()
{
	UnsubscribeEvents();

	//// 弥檬 Account 葛电 沥焊.
	//Issues.Add(DeliveryAccountInfoClass<FB2WholeInfo>::GetInstance().Subscribe2(
	//	[this](const FB2WholeInfo& info)
	//{
	//	this->SetAccountInfo(info);
	//}
	//));

	//// Master Data
	//Issues.Add(DeliveryMasterDataClass<FB2MasterDatas>::GetInstance().Subscribe2(
	//	[this](const FB2MasterDatas& info)
	//{
	//	this->SetMasterDatas(info);
	//}
	//));

	//Issues.Add(DeliveryAccountItemClass<FB2AccountItems>::GetInstance().Subscribe2(
	//	[this](const FB2AccountItems& info)
	//{
	//	this->SetAccountItems(info);
	//}
	//));

	//Issues.Add(DeliveryGetAccountCostumeClass<FB2GetAccountCostumePtr>::GetInstance().Subscribe2(
	//	[this](const FB2GetAccountCostumePtr& info)
	//{
	//	this->ResponseGetAccountCostume(info);
	//}
	//));

	//Issues.Add(DeliveryGetAccountTotemClass<FB2GetAccountTotemPtr>::GetInstance().Subscribe2(
	//	[this](const FB2GetAccountTotemPtr& info)
	//{
	//	this->ResponseGetAccountTotem(info);
	//}
	//));

	//// Item 包访. Item 眠啊绰 LobbyInventory 甫 烹窍瘤 臼绊 咯扁辑 流立 罐绰单 LobbyInventory 啊 绝绰 惑炔俊辑档 Add 瞪 荐 乐扁 锭巩.
	//Issues.Add(DeliveryAddItemClass<FB2AddedItems>::GetInstance().Subscribe2(
	//	[this](const FB2AddedItems& AddedItems)
	//{
	//	this->AddItems(AddedItems);
	//}
	//));
	//Issues.Add(DeliveryAddCostumeToInventoryClass<FB2AddCostumeToInventoryPtr>::GetInstance().Subscribe2(
	//	[this](const FB2AddCostumeToInventoryPtr& pAddCostume)
	//{
	//	this->ResponseAddCostumeToInventory(pAddCostume);
	//}
	//));
	//Issues.Add(DeliveryExtendInventoryClass<FB2ExtendInventory>::GetInstance().Subscribe2(
	//	[this](const FB2ExtendInventory& ExtendInventoryInfo)
	//{
	//	this->ResponseExtendInventory(ExtendInventoryInfo);
	//}
	//));
	//Issues.Add(DeliveryGetCollectionItemClass<FB2CollectionItemPtr>::GetInstance().Subscribe2(
	//	[this](const FB2CollectionItemPtr& CollectionItem)
	//{
	//	this->SetCollectionItemData(CollectionItem);
	//}
	//));

	//Issues.Add(DeliveryLevelUpSkillClass<FB2LevelUpSkillInfo>::GetInstance().Subscribe2(
	//	[this](const FB2LevelUpSkillInfo& LevelUpSkillInfo)
	//{
	//	this->ResponseLevelUpSkill(LevelUpSkillInfo);
	//}
	//));
	//Issues.Add(DeliveryResetSkillClass<FB2ResetSkillInfo>::GetInstance().Subscribe2(
	//	[this](const FB2ResetSkillInfo& ResetSkillInfo)
	//{
	//	this->ResponseResetSkillPreset(ResetSkillInfo);
	//}
	//));
	//Issues.Add(DeliveryBuySkillPointClass<FB2BuySkillPointInfo>::GetInstance().Subscribe2(
	//	[this](const FB2BuySkillPointInfo& BuySkillPointInfo)
	//{
	//	this->ResponseBuySkillPoint(BuySkillPointInfo);
	//}
	//));
	//Issues.Add(DeliverySetUsingSkillClass<FB2SetUsingSkillInfo>::GetInstance().Subscribe2(
	//	[this](const FB2SetUsingSkillInfo& SetUsingSkillInfo)
	//{
	//	this->ResponseSetUsingSkillInfo(SetUsingSkillInfo);
	//}
	//));

	//Issues.Add(DeliveryReceiveSetHelmetVisibleClass<FB2SetHelmetVisible>::GetInstance().Subscribe2(
	//	[this](const FB2SetHelmetVisible& SetHelmetVisible)
	//{
	//	this->ResponseSetHelmetVisible(SetHelmetVisible);
	//}
	//));

	//Issues.Add(DeliverySetCostumeVisibleClass<FB2SetCostumeVisiblePtr>::GetInstance().Subscribe2(
	//	[this](const FB2SetCostumeVisiblePtr& CostumeVisible)
	//{
	//	this->ResponseSetCostumeVisible(CostumeVisible);
	//}
	//));

	//Issues.Add(DeliveryEnhanceTeamFormationClass<FB2EnhanceTeamFormation>::GetInstance().Subscribe2(
	//	[this](const FB2EnhanceTeamFormation& EnhanceResult)
	//{
	//	this->ResponseSetEnhanceTeamFormation(EnhanceResult);
	//}
	//));

	//Issues.Add(DeliveryCharacterNickNameClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->ResponseSetCharNickName();
	//}
	//));
	//Issues.Add(DeliveryUserNickNameClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->ResponseSetNickName();
	//}
	//));

	//Issues.Add(DeliveryBuyShopProductClass<FB2ResponseBuyShopProductPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseBuyShopProductPtr& BuyShopResult)
	//{
	//	this->ResponseBuyShopProduct(BuyShopResult);
	//}
	//));
	//Issues.Add(DeliveryRewardShopMileageClass<FB2ResponseRewardShopMileagePtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseRewardShopMileagePtr& MileageReward)
	//{
	//	this->ResponseRewardShopMileage(MileageReward);
	//}
	//));

	//Issues.Add(DeliveryGetItemRandomOptionClass<FB2ResponseGetItemRandomOptionPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseGetItemRandomOptionPtr& UnappliedItemOption)
	//{
	//	this->SetUnappliedItemOption(UnappliedItemOption);
	//}
	//));

	//// 	Issues.Add(DeliveryGetItemRandomQualityClass<FB2ResponseGetItemRandomQualityPtr>::GetInstance().Subscribe2(
	//// 		[this](const FB2ResponseGetItemRandomQualityPtr& UnappliedItemQuality)
	//// 	{
	//// 		this->SetUnappliedItemQuality(UnappliedItemQuality);
	//// 	}
	//// 	));

	//Issues.Add(DeliveryEventAttendanceClass<FB2ResponseEventAttendancePtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseEventAttendancePtr& EventAttendance)
	//{
	//	this->ResponseEventAttendance(EventAttendance);
	//}
	//));
	//Issues.Add(DeliveryUpdateDailyPlayTimeClass<FB2ResponseUpdateDailyPlayTimePtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseUpdateDailyPlayTimePtr& UpdateDailyPlayTime)
	//{
	//	this->ResponseUpdateDailyPlayTimeClass(UpdateDailyPlayTime);
	//}
	//));
	//Issues.Add(DeliveryDailyPlayTimeRewardClass<FB2ResponseReceiveDailyPlayTimeRewardPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseReceiveDailyPlayTimeRewardPtr& PlayTimeReward)
	//{
	//	this->ResponseDailyPlayTimeRewardClass(PlayTimeReward);
	//}
	//));

	//Issues.Add(DeliveryCheckModeOpenClass<FB2ResponseCheckModeOpenPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseCheckModeOpenPtr& CheckModeOpen)
	//{
	//	this->ResponseCheckModeOpen(CheckModeOpen);
	//}
	//));

	//Issues.Add(DeliveryGetServerVersionClass<const FString&, const FString&>::GetInstance().Subscribe2(
	//	[this](const FString& strVersion, const FString& strStage)
	//{
	//	this->ResponseGetServerVersion(strVersion, strStage);
	//}
	//));

	//Issues.Add(RequestJoinRoomBridgeClass<const FString&, int32, int32>::GetInstance().Subscribe2(
	//	[this](const FString RoomID, int32 RoomType, int32 RoomAccessType)
	//{
	//	this->BridgeRequestJoinRoom(RoomID, RoomType, RoomAccessType);
	//}
	//));

	//Issues.Add(UpdateCombatStatScaleClass<const float>::GetInstance().Subscribe2(
	//	[this](const float fCombatScale)
	//{
	//	this->UpdateCombatStateScale(fCombatScale);
	//}
	//));

	//RedDotManager.SubscribeEvents();
}

void FClientDataStore::UnsubscribeEvents()
{
	Issues.Empty();
}

static void OpenLobbyLoginLoadingScreen()
{
	//// AccountInfo 罐酒辑 捞巴历巴 肺爹窍绰 霸 唱抚 矫埃捞 吧府绰单 DLC 俊辑 肺厚 肺爹且 锭狼 肺爹 拳搁阑 捞绢辑 钎矫.
	//UB2LoadingImageInfo* LoadingImageInfo = StaticFindLoadingImageInfo();
	//// 角力 霸烙俊辑狼 AccountInfo 罐绰 扒 肺厚 皋牢拳搁 霉 柳涝(肺弊牢)俊 秦寸.
	//const TCHAR* LobbyLoginLoadingImageName = LoadingImageInfo ? *(LoadingImageInfo->GetLobbyLoginLoadingImageName()) : nullptr;
	//SetupLoadingScreenImageClass<bool, bool, bool, const TCHAR*, EB2LoadingMessageType>::GetInstance().Signal(
	//	false, false, true,
	//	LobbyLoginLoadingImageName, // 父老 null 捞扼搁 救俊辑 促弗 侥栏肺 肺爹 捞固瘤甫 波郴静霸 瞪 巴.
	//	EB2LoadingMessageType::ELMT_LobbyLoginLoading
	//);
}
static void CloseLobbyLoginLoadingScreen()
{
	//ConditionalWaitForLoadingScreenClass<>::GetInstance().Signal();
}

void FClientDataStore::SetAccountInfo(const FB2WholeInfo& InAccountInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetAccountInfo);
	B2_SCOPED_TRACK_LOG(TEXT("FClientDataStore::SetAccountInfo"));
//
//	OpenLobbyLoginLoadingScreen(); // 厚悼扁 肺爹 拳搁 剁框.
//
//	InitializeAccountInfo();
//
//	LoadLocalData();
//
//	LocalCharacterData.SetCharClassNames(StaticFindPCClassInfoBox()); // ABladeIIGameMode::PreBeginPlay 俊辑 技泼茄 捞饶啊 瞪 巴栏肺 抗惑登骨肺 促矫 技泼.
//
//	AccountInfo = GET_TUPLE_DATA(FB2ResponseWholeInfo::account_index, InAccountInfo);
//
//	if (AccountInfo->name.IsEmpty())
//	{ // 霉 角青捞电瘤, 肺弊酒眶阑 沁电瘤, 拌沥阑 朝啡电瘤 咯窍埃 货肺 积己窍绰 镑.
//#if !UE_BUILD_SHIPPING
//	  // 硅器 滚傈俊辑绰 肺弊酒眶捞唱 拌沥昏力 矫痢俊 肺拿 霸烙 历厘 惑怕甫 朝府骨肺 捞霸 鞘夸 绝阑 芭 鞍瘤父 
//	  // 俺惯 档吝俊绰 沥侥 肺弊酒眶, 拌沥昏力 例瞒 绝捞 货肺 积己窍绰 老捞 腹捞 乐绢辑 咯扁辑 茄锅 朝妨淋.
//		ClearLocalIniSavedState();
//#endif
//		TheInitialSoundLocTypeAndCreateNickName();
//	}
//	else
//	{
//		//Lobby scene logic flow甫 困茄 trick..
//		RequestedUserNickName = AccountInfo->name;
//	}
//
//	ChangeUserAccountInfoClass<>::GetInstance().Signal();
//	LobbyUpdateChapterDataClass<>::GetInstance().Signal();
//
//	// item preset. equip 焊促 刚历 贸府秦具窃
//	auto itemPresets = GET_TUPLE_DATA(FB2ResponseWholeInfo::item_presets_index, InAccountInfo);
//	LocalCharacterData.UserAllItems.UpdateItemPresets(itemPresets);
//
//	// equip
//	GetB2ItemArray(FB2ResponseWholeInfo::equips_index, InAccountInfo);
//	LocalCharacterData.UserAllItems.AddItems(B2ItemArray);
//
//	//costume equip
//	const TArray<b2network::B2CostumeServerInfoPtr>& ArrayCostumeInfo = GET_TUPLE_DATA(FB2ResponseWholeInfo::equip_costumes_index, InAccountInfo);
//	TArray<int64> EqiupCostumeIDList;
//
//	const int32 iCostumeInfoArrayMax = ArrayCostumeInfo.Num();
//	for (int32 i = 0; i < iCostumeInfoArrayMax; ++i)
//	{
//		if (ArrayCostumeInfo.IsValidIndex(i) && ArrayCostumeInfo[i])
//		{
//			AddCostumeItem(*ArrayCostumeInfo[i]);
//			EqiupCostumeIDList.Add(ArrayCostumeInfo[i]->id);
//		}
//	}
//
//	EquipItems(0, EqiupCostumeIDList, TArray<int64>());
//
//	// Totem Equip
//	const TArray<b2network::B2TotemServerInfoPtr>& ArrayTotemInfo = GET_TUPLE_DATA(FB2ResponseWholeInfo::equip_totems_index, InAccountInfo);
//	TArray<FB2Totem> EqiupTotems;
//
//	const int32 TotemInfoArrayMax = ArrayTotemInfo.Num();
//	for (int32 i = 0; i < TotemInfoArrayMax; ++i)
//	{
//		if (ArrayTotemInfo.IsValidIndex(i) && ArrayTotemInfo[i])
//		{
//			AddTotem(ArrayTotemInfo[i]);
//			EqiupTotems.Add(FB2Totem(ArrayTotemInfo[i]));
//		}
//	}
//
//	EquipTotems(EqiupTotems);
//
//	auto characters = GET_TUPLE_DATA(FB2ResponseWholeInfo::characters_index, InAccountInfo);
//	for (auto& Character : characters)
//	{
//		EPCClass CliPClass = SvrToCliPCClassType(Character->character_type);
//		const int32 CharIndex = PCClassToInt(CliPClass);
//
//		if (LocalCharacterData.PlayerSharedStats.IsValidIndex(CharIndex))
//		{
//			LocalCharacterData.PlayerSharedStats[CharIndex].BasicInfo = Character;
//			LocalCharacterData.PlayerSharedStats[CharIndex].CheckNeedRegistSkill();
//		}
//
//		if (LocalCharacterData.CharNames.IsValidIndex(CharIndex))
//			LocalCharacterData.CharNames[CharIndex].NickName = FText::FromString(Character->name);
//
//		LocalCharacterData.UpdateBeforeExpLevel(CliPClass);
//	}
//	LocalCharacterData.InitShowHeadItemCache();
//	LocalCharacterData.InitShowCostumeCache();
//
//	//弥檬 辨靛 沥焊
//	UserGuildInfo.GuildName = GET_TUPLE_DATA(FB2ResponseWholeInfo::guild_name_index, InAccountInfo);
//	UserGuildInfo.Mark_Index = GET_TUPLE_DATA(FB2ResponseWholeInfo::mark_index_index, InAccountInfo);
//	UserGuildInfo.MarkColor_Index = GET_TUPLE_DATA(FB2ResponseWholeInfo::mark_color_index, InAccountInfo);
//	UserGuildInfo.MarkBG_Index = GET_TUPLE_DATA(FB2ResponseWholeInfo::mark_bg_index_index, InAccountInfo);
//	UserGuildInfo.MarkBGColor_Index = GET_TUPLE_DATA(FB2ResponseWholeInfo::mark_bg_color_index, InAccountInfo);
//	UserGuildInfo.GuildRank = GET_TUPLE_DATA(FB2ResponseWholeInfo::my_rank_index, InAccountInfo);
//
//	auto GuildDoc = UB2UIDocHelper::GetDocGuild();
//
//	check(GuildDoc)
//
//		GuildDoc->SetLeaveGuildTime(AccountInfo->leave_guild_time);
//	SetAccountKickedGuildID(AccountInfo);
//
//	// 其绢府 惑怕 沥焊
//	const auto& InFairyStatusData = GET_TUPLE_DATA(FB2ResponseWholeInfo::fairies_index, InAccountInfo);
//	FairyManager::GetInstance().UpdateFairyStatus(InFairyStatusData);
//
//	UserGuildInfo.CurrentGuildSkill.Empty();
//
//	const TArray<b2network::B2GuildSkillPtr>& GuildSkillInfo = GET_TUPLE_DATA(FB2ResponseWholeInfo::guild_skills_index, InAccountInfo);
//
//	for (b2network::B2GuildSkillPtr GuildSkill : GuildSkillInfo)
//	{
//		UserGuildInfo.CurrentGuildSkill.Add(GuildSkill->id, GuildSkill);
//	}
//
//	//蜡拱 沥焊
//	TArray<B2RelicPtr> RelicInfos = GET_TUPLE_DATA(FB2ResponseWholeInfo::relics_index, InAccountInfo);
//	for (auto Relicitem : RelicInfos)
//	{
//		LocalCharacterData.SetRelicInfo(Relicitem);
//	}
//
//	//搬加 胶懦 沥焊
//	TArray<B2UnitySkillMissionPtr> UnitySkillInfos = GET_TUPLE_DATA(FB2ResponseWholeInfo::unitySkills_index, InAccountInfo);
//	LocalCharacterData.SetUnitySkillMissionInfo(UnitySkillInfos);
//
//	//搬加 胶懦 阿己 沥焊
//	TArray<b2network::B2UnitySkillAwakenMissionPtr> UnitySkilAwakelInfos = GET_TUPLE_DATA(FB2ResponseWholeInfo::unity_awaken_missions_index, InAccountInfo);
//	LocalCharacterData.SetUnitySkillAwakenMission(UnitySkilAwakelInfos);
//
//
//	// 肺拿 某腐磐 朝俺 沥焊. 技何 单捞磐绰 付胶磐 单捞磐啊 柯 捞饶俊 诀单捞飘 瞪 巴. 寸厘篮 坊歹傅俊 鞘夸茄 沥焊扼档.
//	SetLocalWingDataByAccountInfoClass<const TArray<b2network::B2CharacterServerInfoPtr>&, const TArray<b2network::B2WingOptionInfoPtr>&>::GetInstance().Signal(
//		GET_TUPLE_DATA(FB2ResponseWholeInfo::characters_index, InAccountInfo),
//		GET_TUPLE_DATA(FB2ResponseWholeInfo::wing_options_index, InAccountInfo)
//	);
//
//	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//	{
//		// 厘馒 酒捞袍 棺 朝俺 单捞磐俊 蝶弗 皋浆 炼钦 诀单捞飘
//		LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
//	}
//
//	const auto& EventAttendanceDatas = GET_TUPLE_DATA(FB2ResponseWholeInfo::event_attendance_statuses_index, InAccountInfo);
//	for (const auto& Data : EventAttendanceDatas)
//	{
//		EventAttendanceStatusData.event_id = Data->event_id;
//		EventAttendanceStatusData.attendance_day = Data->attendance_day;
//		EventAttendanceStatusData.last_attendance_time = Data->last_attendance_time;
//		EventAttendanceStatusData.next_attendance_time = Data->next_attendance_time;
//	}
//
//	const auto& InEventNewUserData = GET_TUPLE_DATA(FB2ResponseWholeInfo::beginner_attendance_status_index, InAccountInfo);
//
//	if (InEventNewUserData != nullptr)
//	{
//		EventNewUserStatusData.TotalAttendanceDays = InEventNewUserData->total_attendance_days;
//		EventNewUserStatusData.AttendanceDay = InEventNewUserData->attendance_day;
//		EventNewUserStatusData.LastAttendanceTime = InEventNewUserData->last_attendance_time;
//		EventNewUserStatusData.NextAttendanceTime = InEventNewUserData->next_attendance_time;
//		EventNewUserStatusData.FinishTime = InEventNewUserData->finish_time;
//		EventNewUserStatusData.IsNewUserAttendance = EventNewUserStatusData.TotalAttendanceDays > EventNewUserStatusData.AttendanceDay;
//
//	}
//
//	PlayTimeStatusData.daily_play_time_in_sec = AccountInfo->daily_play_time_in_sec;
//	PlayTimeStatusData.daily_play_time_reward_index = AccountInfo->daily_play_time_reward_index;
//	PlayTimeStatusData.daily_play_time_update_time = AccountInfo->daily_play_time_update_time;
//
//	SetConnectRewardButtonPush(false);
//
//	auto* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(UB2UIManager::GetInstance()->GetWorld()));
//	if (B2GI)
//	{
//		B2GI->SetPlayTimeInfo(PlayTimeStatusData.daily_play_time_in_sec);
//		B2GI->EnableSavePlayTime(true);
//	}
//
//	SetNextFairyGiftTime(AccountInfo->next_fairy_gift_time);
//#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_LIVE // !B2_BUILD_LEVEL_LIVE
//	// 摹飘虐 涝仿 矫 官肺 利侩登扁 困茄 例瞒捞扁俊 LIVE 俊辑绰 被捞 角青窍瘤 臼酒档 等促.
//	UpdateFairyGiftStateClass<>::GetInstance().Signal();
//#endif
//
//	//////////
//	// Account 沥焊肺 譬配府倔 沥焊 捞悼 抗沥
//	bool IsTutorialClear = GET_TUPLE_DATA(FB2ResponseWholeInfo::is_all_tutorial_finished_index, InAccountInfo);
//	if (IsTutorialClear == false)
//	{
//		if (GetAttendanceInfo().bReceiveDone == false)
//		{
//			TutorialManager& TutorialMgr = TutorialManager::GetInstance();
//			if (TutorialMgr.bTestTutorialAvailable)
//			{
//				TutorialMgr.SetTutorialAvailable(true);
//			}
//		}
//	}
//	else //葛电 譬配府倔阑 努府绢 沁促搁 譬配府倔 厚劝己拳
//	{
//		TutorialManager& TutorialMgr = TutorialManager::GetInstance();
//		TutorialMgr.SetTestTutorialAvailable(false);
//		TutorialMgr.SetTutorialAvailable(false);
//	}
//
//	//////////
//	// 扁夯 AccountInfo 罐篮 捞饶 饶加 单捞磐 夸没..
//
//	//馒侩吝牢 俊抛福 沥焊
//	SetEquippedEtherClass<const int32, const TArray<b2network::B2AetherServerInfoPtr>&>::GetInstance().Signal(
//		GET_TUPLE_DATA(FB2ResponseWholeInfo::aether_inventory_slot_count_index, InAccountInfo),
//		GET_TUPLE_DATA(FB2ResponseWholeInfo::mount_aethers_index, InAccountInfo)
//	);
//
//	//焊蜡 俊抛福 龋免 抗距
//	BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().MakeDirty();
//
//	// 傍侩 酒捞袍阑 夸没窍绰单 扁鸥 某腐磐 喊 焊包窃 酒捞袍篮 牢亥配府 拳搁俊辑 弊锭弊锭 夸没茄促.
//	SyncingInventoryData(EPCClass::EPC_End);
//
//	// 困俊辑 酒捞袍捞 函版凳俊 蝶扼 PropertyItem捞 函版瞪荐 乐促. UIDoc档 盎脚秦淋
//	if (UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser())
//		DocUser->UpdateUserData();
//
//	//肺厚某腐磐甫 困秦 碍力利栏肺 茄锅 朝覆.
//	LobbyCharIntroSelectClass<EPCClass>::GetInstance().Signal(EPCClass::EPC_End);
//	LobbyCharIntroSelectClass<EPCClass>::GetInstance().Signal(LocalCharacterData.GetMainPlayerClass());
//
//	//[@SameLine, 181220] 烙矫 葛电 某腐磐狼 内胶片 酒捞袍 沥焊甫 绢墨款飘 沥焊甫 罐绊 夸没
//	//肺厚 柳涝傈 厘馒 内胶片 沥焊甫 罐阑 喊档狼 规过捞 眠啊啊 救等促搁 request 矫痢父 炼例瞪巴栏肺 抗惑
//	this->RequestGetAccountCostumeAll();
//
//	// 配袍 沥焊 葛滴 罐扁
//	this->RequestGetAccountTotem();
//
//	//霉 版氰摹 沥焊 DOC盎脚, 傈捧仿档 鞍捞 啪铰
//	for (int32 i = 0; i < PCClassToInt(EPCClass::EPC_End); i++)
//	{
//		auto* HeroDoc = UB2UIDocHelper::GetDocHero(i);
//		if (HeroDoc)
//		{
//			EPCClass pcClass = IntToPCClass(i);
//			const int32 Combat = CombatStatEval::GetPCCombatPower(pcClass);
//			const int32 CharLevel = LocalCharacterData.GetCharacterLevel(pcClass);
//
//			//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
//			//HeroDoc->InitExp();
//			HeroDoc->SetPowers(CharLevel);
//			HeroDoc->SetPostClearPowers(CharLevel);
//			HeroDoc->SetPreCombatPower(Combat);
//			HeroDoc->SetCombatPower(Combat);
//		}
//	}
//
//	//肺厚 硅呈 沥焊.
//	LobbyBannerData.Empty();
//	LobbyBannerData = GET_TUPLE_DATA(FB2ResponseWholeInfo::rolling_banners_index, InAccountInfo);
//
//#if WITH_EDITOR
//	extern bool gbAlreadyGotAccountOnce_ForTitleScreen;
//	gbAlreadyGotAccountOnce_ForTitleScreen = true; // 俊叼磐狼 版快 鸥捞撇拳搁俊辑 促矫 夸没窍瘤 臼档废 钎矫.
//#endif
//
//												   //弥檬 譬配府倔 沥焊甫 罐酒辑 傈捧 譬配府倔 矫累 or 肺厚柳涝 眉农
//	TutorialManager::GetInstance().RequestGetTutorial();
//
//	// 萍百 沥焊 平 劝己拳
//	FBladeIIChargeBladePointCharger::GetBladeIIPointCharger().Initialize();
//	FBladeIIChargeBladePointCharger::GetPvPMatchPointCharger().Initialize();
//	FBladeIIChargeBladePointCharger::GetTeamMatchPointCharger().Initialize();
//
//	EUIScene CurrentSceneEnum = UB2UIManager::GetInstance()->GetCurrUIScene();
//
//	auto* CurrentSceneUI = UB2UIManager::GetInstance()->GetUISceneData(CurrentSceneEnum);
//
//	if (CurrentSceneUI)
//	{
//		for (auto El : CurrentSceneUI->WidgetFNames)
//		{
//			auto ElUI = UB2UIManager::GetInstance()->GetUI<UB2UnitedWidgetBase>(El);
//			if (ElUI)
//			{
//				ElUI->UpdateServercmdGetAccount();
//			}
//		}
//	}
//
//#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_LIVE // !B2_BUILD_LEVEL_LIVE
//	//Account 汲沥 且 锭父 龋免 登霸 函版
//	RequestGetServerVersion();
//#endif //B2_BUILD_LEVEL < B2_BUILD_LEVEL_LIVE 
//	CloseLobbyLoginLoadingScreen(); // 肺爹拳搁 辆丰
}

void FClientDataStore::CheckBeginCombatTutorialorEntryMainLobby()
{
	//if (HasValidUserNickName()) // 葱匙烙 积己秦具 窍绰 惑炔捞搁 Skip 瞪 巴.
	//{
	//	if (bReceivedServerMasterData)
	//	{
	//		OpenLobbyLoginLoadingScreen(); // 咯扁档 矫埃捞 粱 吧府骨肺 肺爹 拳搁阑 钎矫
	//									   // LobbyMain 柳涝 矫弊澄.
	//		DoneToGetAccountClass<>::GetInstance().Signal();
	//		CloseLobbyLoginLoadingScreen();
	//	}
	//}
}

void FClientDataStore::SetAccountItems(const FB2AccountItems& InItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetAccountItems);
	B2_SCOPED_TRACK_LOG(TEXT("FClientDataStore::SetAccountItems"));

	//GetB2ItemArray(FB2ResponseAccountItems::items_index, InItems);

	//for (FB2Item& elem : B2ItemArray)
	//{//酒.. empty array俊急 绢戮促..
	//	EPCClass ThisItemPCClass = IntToPCClass(elem.AllowedPCClass);

	//	if (elem.InventoryType == EItemInvenType::EIIVT_Consumables)
	//	{
	//		LocalCharacterData.UserAllItems.bSyncingConsumables = true;
	//	}
	//	else if (ThisItemPCClass != EPCClass::EPC_End)
	//	{
	//		if (LocalCharacterData.UserAllItems.CharacterItemContainers.IsValidIndex(elem.AllowedPCClass)) {
	//			LocalCharacterData.UserAllItems.CharacterItemContainers[elem.AllowedPCClass].bSyncingData = true;
	//		}
	//	}
	//}
	//LocalCharacterData.UserAllItems.AddItems(B2ItemArray);

	//// 困俊辑 酒捞袍捞 函版凳俊 蝶扼 PropertyItem捞 函版瞪荐 乐促. UIDoc档 盎脚秦淋
	//if (UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser())
	//	DocUser->UpdateUserData();

	//// 咯变 LobbyInventory 甫 烹窍瘤 臼绰 镑牢单 Inventory 啊 栋 乐绰 惑怕扼搁 UI 扼档 诀单捞飘 窍霸 脚龋甫 朝赴促.
	//LobbyAllItemsAddedClass<>::GetInstance().Signal();
	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();

	//// 牢亥配府 教农 捞饶俊 厘厚窃 肮荐 俊矾 扑诀阑 促矫 剁况林扁困秦
	//if (GetWaitInventorySync()) // 快急 扑诀 侩档肺 甸绢吭绰瘤 眉农..
	//{
	//	for (int32 i = 0; i < GetMaxPCClassNum(); i++)
	//	{
	//		if (LocalCharacterData.UserAllItems.CharacterItemContainers.IsValidIndex(i))
	//		{
	//			if (!LocalCharacterData.UserAllItems.CharacterItemContainers[i].bSyncingData) // 酒流 教农啊 肯傈 肯丰等 惑怕啊 酒丛
	//			{
	//				return;
	//			}
	//		}
	//	}

	//	SetWaitInventorySync(false);
	//	HandleServerError4015ByRoomJoinClass<>::GetInstance().Signal();
	//}
}

int64 FClientDataStore::GetAccountId()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetAccountId);
	if (AccountInfo)
	{
		return AccountInfo->account_id;
	}

	return 0;
}

void FClientDataStore::SetMasterDatas(const FB2MasterDatas& InMasterDatas)
{
//	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDatas);
//	B2_SCOPED_TRACK_LOG(TEXT("FClientDataStore::SetMasterDatas"));
//
//	PreSetMasterData();
//	bReceivedServerMasterData = true;
//
//	SetMasterDataImpl_ConfigData(InMasterDatas); // 0
//	SetMasterDataImpl_Skill(InMasterDatas); // 1, 2, 89
//											//LobbyAnyOtherUpdateOnSetAccountClass<>::GetInstance().Signal();		// todo : confirm and delete code
//	SetMasterDataImpl_ItemLevelupExpFactorAndOptionAddValue(InMasterDatas); // 3, 4
//	SetMasterDataImpl_ItemLevelupCost(InMasterDatas); // 5
//	SetMasterDataImpl_StageInfo(InMasterDatas); // 6, 53
//	SetMasterDataImpl_ResurrectionInfo(InMasterDatas);	// 7
//	SetMasterDataImpl_TeamBattleFormationInfo(InMasterDatas); // 8 
//	SetMasterDataImpl_RankPromotionInfo(InMasterDatas);	 // 9
//	SetMasterDataImpl_DailyMatchReward(InMasterDatas); // 10
//	SetMasterDataImpl_PVPMatchRankingReward(InMasterDatas); // 11
//	SetMasterDataImpl_TeamMatchRankingReward(InMasterDatas); // 12
//	SetMasterDataImpl_CounterDungeon(InMasterDatas); // 13
//	SetMasterDataImpl_CounterDungeonPeriod(InMasterDatas); // 14
//	SetMasterDataImpl_CounterDungeonWeekReward(InMasterDatas); // 15
//															   // B2mdMainQuestInfoPtr(16) : MissionManager俊辑 贸府	
//															   // B2mdDailyQuestInfoPtr(17) : MissionManager俊辑 贸府	
//	SetMasterDataImpl_RewardInfo(InMasterDatas);	// 18
//													// B2mdMissionInfoPtr(19)  : MissionManager俊辑 贸府
//													// B2mdSerialMissionInfoPtr(20) : MissionManager俊辑 贸府
//													// B2mdMissionCompletePointInfoPtr(21) : MissionManager俊辑 贸府
//	SetMasterDataImpl_WingEnhance(InMasterDatas);	// 22
//	SetMasterDataImpl_WingUpgrade(InMasterDatas);	// 23
//	SetMasterDataImpl_WingOptionValue(InMasterDatas);	// 24
//	SetMasterDataImpl_WingOptionOpen(InMasterDatas);	// 25
//	SetMasterDataImpl_WingOptionMaxLevel(InMasterDatas);	// 26
//	SetMasterDataImpl_HeroTowerFloor(InMasterDatas);	// 27
//														// SetMasterDataImpl_HeroTowerRetryCost(InMasterDatas); // 28
//														// B2mdRelicInfoPtr : RelicManager俊辑 贸府
//														// B2mdRelicGradeInfoPtr : RelicManager俊辑 贸府
//														// B2mdRelicLevelInfoPtr : RelicManager俊辑 贸府
//
//	SetMasterDataImpl_ShopLotteryPrice(InMasterDatas);							// 30
//	SetMasterDataImpl_ShopLotteryPossibleGradeAndSpecialGrade(InMasterDatas);	// 32, 33
//	SetMasterDataImpl_ShopProduct(InMasterDatas);								// 34
//	SetMasterDataImpl_ShopPackage(InMasterDatas);								// 35
//	SetMasterDataImpl_ShopMileageReward(InMasterDatas);							// 36
//	SetMasterDataImpl_ShopLotteryDailylimitsMaxCount(InMasterDatas);			// 37
//
//	SetMasterDataImpl_DailyAttendanceInfo(InMasterDatas);						// 39
//
//	SetMasterDataImpl_GuildBattleResultReward(InMasterDatas); // 41  ??鉴辑啊 部看唱焊匙夸..
//															  // B2mdGuildBattleResultRewardPtr(39) // ???  todo : confirm and delete code
//	SetMasterDataImpl_GuildBattleSeasonReward(InMasterDatas); // 40
//	SetMasterDataImpl_Raid(InMasterDatas);	// 41
//	SetMasterDataImpl_MonsterInfo(InMasterDatas);	// 42
//	SetMasterDataImpl_GeneralLottery(InMasterDatas); // 43
//	SetMasterDataImpl_SelectiveLottery(InMasterDatas); // 44
//
//	SetMasterDataImpl_FixedGradeLotteries(InMasterDatas); // 45
//	SetMasterDataImpl_ItemEnhancementCost(InMasterDatas);	// 46
//	SetMasterDataImpl_ItemEnhancementEffectInfo(InMasterDatas);	// 47
//	SetMasterDataImpl_AdditionalMatchPointInfos(InMasterDatas);// 48
//
//	SetMasterDataImpl_GuildSkill(InMasterDatas);					//50
//	SetMasterDataImpl_GuildExps(InMasterDatas);					//51
//																// SetMasterDataImpl_StageFixedDrop(InMasterDatas);// 53, SetMasterData(InMasterDatas) 救俊 SetMDStageFixedDrops俊辑 窍绊 乐澜 
//
//	SetMasterDatalmpl_AssaultMvpRewardInfo(InMasterDatas);		//55
//																//SetMasterDataImpl_EventAttendances(InMasterDatas); // 56	//RewardEventManager狼 ResponseLatestEventInfo肺 捞悼
//	SetMasterDataImpl_EventAttendanceReward(InMasterDatas); // 57
//	SetMasterDataImpl_PlayTimeReward(InMasterDatas); // 58
//	SetMasterDataImpl_Tutorial(InMasterDatas);	// 59
//	SetMasterDataImpl_ControlMatchRankingReward(InMasterDatas); // 58
//
//																//62, 63, 64, 65 B2EtherManager俊辑 贸府
//	SetMasterDataImpl_Guild(InMasterDatas); // 66
//
//	SetMasterDataImpl_GameResultExp(InMasterDatas);
//
//	SetMasterDataImpl_GuildBattleSchedule(InMasterDatas);	//68
//	SetMasterDataImpl_RestReward(InMasterDatas);
//	SetMasterDataImpl_DuelModeReward(InMasterDatas);
//
//	SetMasterDataImpl_Item(InMasterDatas);					// 74, 75
//
//	SetMasterDataImpl_EventInfo(InMasterDatas);				// 77
//
//	SetMasterDataImpl_KakaoInviteReward(InMasterDatas);		// 83
//	SetMasterDataImpl_HotTimeProduct(InMasterDatas);		// 84
//
//	SetMasterDataImpl_ShopCashBonus(InMasterDatas);			// 85
//	SetMasterDataImpl_ShopFundsSale(InMasterDatas);			// 86
//	SetMasterDataImpl_ShopLotterySale(InMasterDatas);		// 87
//	SetMasterDataImpl_PackageProduct(InMasterDatas);			//88
//
//	SetMasterDataImpl_DuelModeSettleSchedule(InMasterDatas);	// 90
//	SetMasterDataImpl_ShopLevelUpPackage(InMasterDatas);		// 91
//	SetMasterDataImpl_ShopReachLevelPackage(InMasterDatas);		// 92
//	SetMasterDataImpl_ShopFlatratePackageBonus(InMasterDatas);	// 93
//	SetMasterDataImpl_ShopPackageTabOrder(InMasterDatas);		// 94
//																// 95 UnitySkillOptionPtr : UnitySkillManager俊辑 贸府
//	SetMasterDataImpl_WebURL(InMasterDatas);					// 97
//
//	SetMasterDataImpl_MagicShopProduct(InMasterDatas);			// 99
//	SetMasterDataImpl_MagicShopRenewalCost(InMasterDatas);		// 100
//
//	SetMasterDataImpl_ItemlevelupFactor(InMasterDatas);			// 104
//
//	SetMasterDataImpl_PrefixSelectiveLottery(InMasterDatas);	// 105
//
//	SetMasterDataImpl_SelectItemOption(InMasterDatas);	// 106
//	SetMasterDataImpl_SurpassLevel(InMasterDatas); // 110
//	SetMasterDataImpl_ItemDismantleInfo(InMasterDatas); // 113
//
//	SetMasterDataImpl_FairyInfo(InMasterDatas);					// 114
//	SetMasterDataImpl_FairyLevelupCosts(InMasterDatas);			// 115
//	SetMasterDataImpl_FairyOpenInfo(InMasterDatas);				// 116
//
//	SetMasterDataImpl_Costume(InMasterDatas);					// FCostumeDataStore俊辑 贸府 抗沥
//
//	SetMasterDataImpl_DonationPointRankingRewardInfo(InMasterDatas); //118
//	SetMasterDataImpl_DonationBundleFixedRewardInfo(InMasterDatas);	 // 119
//	SetMasterDataImpl_DonationPointStepRewardInfo(InMasterDatas);	   //120
//
//	SetMasterDataImpl_ExtendMessage(InMasterDatas);	   //122
//	SetMasterDataImpl_CostumeEnhanceCostInfo(InMasterDatas); // ExtendMessage
//
//	SetMasterDataImpl_ConvertTableInfo(InMasterDatas); //ExtendMessage
//	SetMasterDataImpl_ConvertTableCost(InMasterDatas); //ExtendMessage
//
//	SetMasterDataImpl_LotteryShopRateQualityInfo(InMasterDatas); //ExtendMessage
//	SetMasterDataImpl_QualityTransRateInfo(InMasterDatas); //ExtendMessage
//
//	SetMasterDataImpl_ItemForgeInfo(InMasterDatas); //ExtendMessage
//	SetMasterDataImpl_ItemForgeMaterialTransInfo(InMasterDatas); //ExtendMessage
//	SetMasterDataImpl_ItemForgeLimitStatus(InMasterDatas); //ExtendMessage
//
//	SetMasterDataImpl_SealBoxInfo(InMasterDatas);	//ExtendMessage
//	SetMasterDataImpl_SealBoxRewardRateInfo(InMasterDatas);	//ExtendMessage
//
//														   // Note : Post绰 亲惑 付瘤阜俊 龋免窍档废 窍磊.
//	PostSetMasterData();
//
//	FBladeIIBlockToSyncNetwork::GetInstance().SetIsRequestMasterData(false);
//	data_trader::Retailer::GetInstance().RequestGetAccount();
}

void FClientDataStore::SetGuildAccountID(int64 GuildID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetGuildAccountID);
	AccountInfo->guild_id = GuildID;
}

void FClientDataStore::SetMyGuildInfo(FGuildInfo GuildInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMyGuildInfo);
	MyGuildData = GuildInfo;
	AccountInfo->guild_id = GuildInfo.GuildInfo->guild_id;
}

void FClientDataStore::SetMyGuildInfo(FB2ResponseGuildInfoPtr InGuildInfo)
{
	MyGuildData.GuildInfo = InGuildInfo;
	AccountInfo->guild_id = MyGuildData.GuildInfo->guild_id;
}

void FClientDataStore::UpdateMyGuildGuildMemberInfo(TArray<b2network::B2GuildMemberPtr> InChangedGuildMember)
{
	for (b2network::B2GuildMemberPtr ChangedEle : InChangedGuildMember)
	{
		for (b2network::B2GuildMemberPtr Ele : MyGuildData.GuildMemberInfo)
		{
			if (Ele->character_summary->account_id == ChangedEle->character_summary->account_id)
			{
				MyGuildData.GuildMemberInfo.Remove(Ele);
				MyGuildData.GuildMemberInfo.Add(ChangedEle);
			}
		}

		if (GetAccountId() == ChangedEle->character_summary->account_id)
		{
			UserGuildInfo.GuildRank = ChangedEle->rank;
		}
	}
}

FTimespan FClientDataStore::GetGuildMemberLastLogoutTime(int64 InMemberID)
{
	FTimespan LastLogoutTime = FTimespan::Zero();

	for (b2network::B2GuildMemberPtr Ele : MyGuildData.GuildMemberInfo)
	{
		if (Ele->character_summary->account_id == InMemberID)
		{
			LastLogoutTime = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(Ele->last_logout_time / 1000);
			break;
		}
	}

	return LastLogoutTime;
}

FTimespan FClientDataStore::GetMasterElectedTime()
{
	return FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(MyGuildData.GuildInfo->master_elected_time / 1000);
}

void FClientDataStore::SetMyGuildMemberOut(int64 AccountID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMyGuildMemberOut);
	for (auto Ele : MyGuildData.GuildMemberInfo)
	{
		if (Ele->character_summary->account_id == AccountID)
		{
			MyGuildData.GuildMemberInfo.Remove(Ele);
			break;
		}
	}
}

void FClientDataStore::SetMyGuildSubMaster(bool Type, int64 AccountID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMyGuildSubMaster);

	//true搁 何辨靛厘 烙疙, 酒聪搁 秦烙!!!
	for (auto Ele : MyGuildData.GuildMemberInfo)
	{
		if (Ele->character_summary->account_id == AccountID)
		{
			Type ? Ele->rank = b2network::B2GuildMemberRank::SUB_MASTER : Ele->rank = b2network::B2GuildMemberRank::NORMAL;
		}
	}
}

void FClientDataStore::SetOtherGuildInfo(FGuildInfo GuildInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetOtherGuildInfo);
	OtherGuildData = GuildInfo;
}

void FClientDataStore::OnShutdownModule()
{
	UnsubscribeEvents();
}

void FClientDataStore::SaveLocalData() // Save Local (Config) Data
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SaveLocalData);
	LocalCharacterData.SaveLocalData();

	//	RedDotManager.SaveMarkForNewData();
}

void FClientDataStore::LoadLocalData() // Load Local (Config) Data
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_LoadLocalData);
	LocalCharacterData.LoadLocalData();
	//RedDotManager.LoadLocalData();
}

void FClientDataStore::LoadLocalData_ResourceBound()
{ // 葛碘 Startup 窜拌俊辑绰 利例摹 臼篮 InfoAsset 郴瘤绰 MasterData 啊 鞘夸茄 config 单捞磐啊 乐绢辑 蝶肺 都绢晨. LoadLocalData 狼 老何啊 酒丛.
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_LoadLocalData_ResourceBound);
	LocalCharacterData.LoadLocalData_ResourceBound();
}

void FClientDataStore::AddNewAcquiredItems(const TArray<FB2Item>& NewItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_LoadLocalData_AddNewAcquiredItems);
	LocalCharacterData.UserAllItems.AddNewAcquiredItems(NewItems);
}

void FClientDataStore::AddNewAcquiredItem(const FB2Item& NewItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_LoadLocalData_AddNewAcquiredItem);
	LocalCharacterData.UserAllItems.AddNewAcquiredItem(NewItems);
}

void FClientDataStore::AddChangeItem(const FB2Item& NewItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_AddChangeItem);

	//LocalCharacterData.UserAllItems.RemoveItem(NewItems.InstanceUID);
	//LocalCharacterData.UserAllItems.ChangeItem(NewItems);
	//LobbyAllItemsAddedClass<>::GetInstance().Signal();
	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void FClientDataStore::AddItems(const FB2AddedItems& AddedItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_AddItems);
	LocalCharacterData.UserAllItems.AddItems(AddedItems.AddedItems);
	// 咯变 LobbyInventory 甫 烹窍瘤 臼绰 镑牢单 Inventory 啊 栋 乐绰 惑怕扼搁 UI 扼档 诀单捞飘 窍霸 脚龋甫 朝赴促.
	//LobbyItemsAddedClass<const FB2AddedItems&>::GetInstance().Signal(AddedItems);
}

void FClientDataStore::AddCostumeItem(const b2network::B2CostumeServerInfo& CostumeInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_AddCostumeItem);

	FB2Item fCostumeItem(CostumeInfo);

	LocalCharacterData.UserAllItems.AddItem(fCostumeItem);
}


void FClientDataStore::ChangeCostumeItem(const b2network::B2CostumeServerInfoPtr& CostumeInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ChangeCostumeItem);

	//FB2Item fCostumeItem(*CostumeInfo);

	//LocalCharacterData.UserAllItems.ChangeItem(fCostumeItem);

	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void FClientDataStore::ResponseExtendInventory(const FB2ExtendInventory& ExtendInventoryInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseExtendInventory);
	//int32 NewGem = GET_TUPLE_DATA(FB2ResponseExtendInventory::cash_index, ExtendInventoryInfo);
	//if (AccountInfo)
	//{
	//	AccountInfo->cash = NewGem;
	//}
	//b2network::B2CharacterServerInfoPtr ExtendedCharinfo = GET_TUPLE_DATA(FB2ResponseExtendInventory::character_index, ExtendInventoryInfo);

	//const int32 CharArrayIdx = PCClassToInt(SvrToCliPCClassType(ExtendedCharinfo->character_type));
	//if (LocalCharacterData.PlayerSharedStats.IsValidIndex(CharArrayIdx))
	//	LocalCharacterData.PlayerSharedStats[CharArrayIdx].BasicInfo = ExtendedCharinfo;

	//ABladeIIGameMode::SetHUDCenterMessage(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("LobbyInvenExtend_Notice")), 1.0f);

	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void FClientDataStore::SellItem(const FB2SoldItems& SoldItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SellItem);
	////Syncing data
	//ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseSoldItem::money_total_index, SoldItems));

	//LocalCharacterData.UserAllItems.RemoveItems(GET_TUPLE_DATA(FB2ResponseSoldItem::sell_item_ids_index, SoldItems));
}

void FClientDataStore::SellConsumableItem(const FB2SellConsumableItem& SellConsumableItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SellConsumableItem);
	////Syncing data
	//ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseSellConsumableItem::current_money_index, SellConsumableItem));

	//FB2Item MainItem;
	//MainItem = GET_TUPLE_DATA(FB2ResponseSellConsumableItem::updated_item_index, SellConsumableItem);

	//// 犁丰 酒捞袍 魄概饶 巢篮 肮荐啊 0俺 老锭
	//if (GET_TUPLE_DATA(FB2ResponseSellConsumableItem::deleted_item_id_index, SellConsumableItem))
	//{
	//	LocalCharacterData.UserAllItems.RemoveItem(GET_TUPLE_DATA(FB2ResponseSellConsumableItem::deleted_item_id_index, SellConsumableItem));
	//}
	//else
	//{
	//	LocalCharacterData.UserAllItems.ChangeItem(MainItem);
	//}
}

//函券 傈 冉荐 眉农
void FClientDataStore::ConvertData(const FB2ConvertData& ConvertData)
{
	//UI 龋免篮 B2LobbyInventory.cpp ResponseConvertData 窃荐俊辑 龋免等促.
	//咯扁急 迭洒 秦 临 老捞 绝澜.
}
//函券 磊眉
void FClientDataStore::ConvertItem(const FB2ConvertItem& ConvertItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ConvertItem);
	////Syncing data
	//ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseConvertItem::currentGold_index, ConvertItem));
	//ReplaceUserDataWithDoc(EDocUserDataType::Gem, GET_TUPLE_DATA(FB2ResponseConvertItem::currentCash_index, ConvertItem));

	///*
	//OnResponseConsumableAmountDecrease(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE,
	//-1 // 抗傈 内靛狼 儡犁肺 橇肺配妮 惑俊 粮犁窍瘤 臼栏搁 澜荐肺 逞败拎档 等促. ,
	//RefineTotemPtr->changed_dimensional_piece_items,
	//RefineTotemPtr->deleted_dimensional_piece_ids
	//);
	//OnResponseConsumableAmountIncrease
	//*/

	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void FClientDataStore::UseNickNameChangeTicket(const FB2UpdateNicknamePtr& updated)
{
	if (updated->deleted_item_id > 0)
	{
		LocalCharacterData.UserAllItems.RemoveItem(updated->deleted_item_id);
	}
	else
	{
		FB2Item MainItem;
		MainItem = updated->updated_item;
		LocalCharacterData.UserAllItems.ChangeItem(MainItem);
	}

	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void FClientDataStore::DecompositionItem(const FB2DismantleItems& DismantleItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_DecompositionItem);
	////Syncing data
	//LocalCharacterData.UserAllItems.RemoveItems(GET_TUPLE_DATA(FB2ResponseDismantleItems::dismantled_item_ids_index, DismantleItems));

	//const TArray<b2network::B2ItemServerInfoPtr> ExtractedItems = GET_TUPLE_DATA(FB2ResponseDismantleItems::extracted_items_index, DismantleItems);
	//for (int i = 0; i < ExtractedItems.Num(); ++i)
	//{
	//	OnResponseConsumableAmountIncrease(ExtractedItems[i]->template_id, ExtractedItems[i]);
	//}
}

void FClientDataStore::DecompositionCostumeItem(const FB2ResponseDismantleCostumesPtr& DismantleItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_DecompositionCostumeItem);
	//Syncing data
	LocalCharacterData.UserAllItems.RemoveItems(DismantleItems->dismantled_costume_ids);

	const TArray<b2network::B2ItemServerInfoPtr> ExtractedItems = DismantleItems->extracted_items;
	for (int i = 0; i < ExtractedItems.Num(); ++i)
	{
		OnResponseConsumableAmountIncrease(ExtractedItems[i]->template_id, ExtractedItems[i]);
	}

	if (DismantleItems->restore_item)
		OnResponseConsumableAmountIncrease(DismantleItems->restore_item->template_id, DismantleItems->restore_item);
}

void FClientDataStore::EquipItems(const FB2EquippedItems& EquipppedItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_EquipItems);

	//EquipItems(GET_TUPLE_DATA(FB2ResponseEquippedItem::preset_id_index, EquipppedItems),
	//	GET_TUPLE_DATA(FB2ResponseEquippedItem::unequiped_item_ids_index, EquipppedItems),
	//	GET_TUPLE_DATA(FB2ResponseEquippedItem::equiped_item_ids_index, EquipppedItems));
}

void FClientDataStore::EquipItems(const int32 & PresetID, const TArray<int64>& EquipedItems, const TArray<int64>& UnequipedItems)
{
	LocalCharacterData.UserAllItems.EquipItems(UnequipedItems, false, PresetID);
	LocalCharacterData.UserAllItems.EquipItems(EquipedItems, true, PresetID);
}

void FClientDataStore::UnEquipItems(const TArray<int64>& ArrayUnEquipItemID, const int32 iPresetID /*= -1*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_UnEquipItems);

	// -1 = 傈眉 橇府悸 秦力
	if (iPresetID < 0)
		LocalCharacterData.UserAllItems.UnEquipItemsPresets(ArrayUnEquipItemID);
	else
		LocalCharacterData.UserAllItems.EquipItems(ArrayUnEquipItemID, false, iPresetID);
}

void FClientDataStore::LockItems(const FB2LockedItems& LockedItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_LockItems);
	//GetB2ItemArray(FB2ResponseLockItem::locked_items_index, LockedItems);
	//LocalCharacterData.UserAllItems.LockItems(B2ItemArray, true);
}

void FClientDataStore::UnLockItems(const FB2UnlockedItems& UnlockedItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_UnLockItems);
	//GetB2ItemArray(FB2ResponseUnlockItem::unlocked_items_index, UnlockedItems);
	//LocalCharacterData.UserAllItems.LockItems(B2ItemArray, false);
}

void FClientDataStore::LevelupItem(const FB2LevelupItem& EnhancedItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_LevelupItem);
	//ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseLevelupItem::current_money_index, EnhancedItem));

	//FB2Item MainItem;
	//MainItem = GET_TUPLE_DATA(FB2ResponseLevelupItem::main_item_index, EnhancedItem);
	//LocalCharacterData.UserAllItems.ChangeItem(MainItem);
	//LocalCharacterData.UserAllItems.RemoveItems(GET_TUPLE_DATA(FB2ResponseLevelupItem::item_ids_index, EnhancedItem));

	////////////////////////////////////////////////////////////////////
	////咀记 瘤钎 肺弊 (酒捞袍 饭骇诀)
	//FString itemName(TEXT("ITEM_LEVELUP_NONE"));
	//UB2ItemInfo* IteminfoTable(StaticFindItemInfo());
	//FSingleItemInfoData* ThisItemInfo = IteminfoTable ? IteminfoTable->GetInfoData(MainItem.ItemRefID) : NULL;
	//if (ThisItemInfo)
	//	itemName = ThisItemInfo->GetLocalizedName().ToString().ToUpper();
	//B2ActionLogSender Sender;
	//Sender.setCategory(FString(TEXT("ITEM")))
	//	.setAction(FString(TEXT("LEVELUP")))
	//	.setLabel(itemName)
	//	.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	////////////////////////////////////////////////////////////////////
}

void FClientDataStore::SurpassItem(const FB2SurpassItem& SurpassedItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SurpassItem);
	//ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseSurpassItem::current_money_index, SurpassedItem));

	//FB2Item MainItem;
	//MainItem = GET_TUPLE_DATA(FB2ResponseSurpassItem::main_item_index, SurpassedItem);
	//LocalCharacterData.UserAllItems.ChangeItem(MainItem);
	//LocalCharacterData.UserAllItems.RemoveItem(GET_TUPLE_DATA(FB2ResponseSurpassItem::material_item_id_index, SurpassedItem));

	////////////////////////////////////////////////////////////////////
	////咀记 瘤钎 肺弊 (酒捞袍 檬岿)
	//FString itemName(TEXT("ITEM_SURPASS_NONE"));
	//UB2ItemInfo* IteminfoTable(StaticFindItemInfo());
	//FSingleItemInfoData* ThisItemInfo = IteminfoTable ? IteminfoTable->GetInfoData(MainItem.ItemRefID) : NULL;
	//if (ThisItemInfo)
	//	itemName = ThisItemInfo->GetLocalizedName().ToString().ToUpper();
	//B2ActionLogSender Sender;
	//Sender.setCategory(FString(TEXT("ITEM")))
	//	.setAction(FString(TEXT("SURPASS")))
	//	.setLabel(itemName)
	//	.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//////////////////////////////////////////////////////////////////
}

void FClientDataStore::UpgradeItem(const FB2UpgradeItem& UpgradedItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_UpgradeItem);
	//ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseUpgradeItem::current_money_index, UpgradedItem));

	//FB2Item MainItem;
	//MainItem = GET_TUPLE_DATA(FB2ResponseUpgradeItem::main_item_index, UpgradedItem);
	//LocalCharacterData.UserAllItems.ChangeItem(MainItem);

	//// 铰鞭籍 皑家 贸府.. 捞芭 捞饶俊 眠啊等 厚搅茄 己拜狼 览翠甸篮 辑滚俊辑 力芭等 俺荐尔 力芭等 浇吩 UID 殿 利例洒 焊郴林扁 锭巩俊 捞繁 侥栏肺 贸府 救 秦档 瞪 淀 茄单 咯扁急 流立 贸府秦 林绰 霸 粱 乐促.

	//int32 ToReduceStoneCount = GET_TUPLE_DATA(FB2ResponseUpgradeItem::use_upgrade_stone_count_index, UpgradedItem);
	//LocalCharacterData.UserAllItems.DecSharedConsumableAmountOfType(FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(MainItem, EUpgradeded::UPGRADE_AFTER), ToReduceStoneCount);
	////LocalCharacterData.UserAllItems.DecSharedConsumableAmountOfType(FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(MainItem, EUpgradeded::UPGRADE_NONE), ToReduceStoneCount);
	//{ // 捞扒 吝汗等 单捞磐牢啊.. 辟单 捞哩 傲 厚绢乐绰 芭 鞍绊..
	//	GetB2ItemArray(FB2ResponseUpgradeItem::upgrade_stone_items_index, UpgradedItem);
	//	LocalCharacterData.UserAllItems.ChangeItems(B2ItemArray);
	//}

	// 铰鞭籍 浇吩 窍唱啊 力芭等 版快老淀.. 捞固 DecSharedConsumableAmountOfType 俊辑 力芭登菌阑 巴捞促.
	//LocalCharacterData.UserAllItems.RemoveItems(GET_TUPLE_DATA(FB2ResponseUpgradeItem::deleted_item_ids_index, UpgradedItem));
}

void FClientDataStore::EnhanceChangeItem(const FB2Item& InTarget, const FB2Item& InSource)
{
	TArray<FB2Item> TargetArray;
	TArray<FB2Item> SourceArray;
	TargetArray.Add(InTarget);
	SourceArray.Add(InSource);

	LocalCharacterData.UserAllItems.ChangeItems(TargetArray);
	LocalCharacterData.UserAllItems.ChangeItems(SourceArray);
}

//int64 FClientDataStore::ComposeItem(const FB2ComposeItem& ComposedItem)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ComposeItem);
//	ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseComposeItem::current_money_index, ComposedItem));
//
//	FB2Item NewItem;
//	NewItem = GET_TUPLE_DATA(FB2ResponseComposeItem::new_composed_item_index, ComposedItem);
//	LocalCharacterData.UserAllItems.AddItem(NewItem);
//	LocalCharacterData.UserAllItems.RemoveItem(GET_TUPLE_DATA(FB2ResponseComposeItem::deleted_main_item_id_index, ComposedItem));
//	LocalCharacterData.UserAllItems.RemoveItem(GET_TUPLE_DATA(FB2ResponseComposeItem::deleted_sub_item_id_index, ComposedItem));
//
//	return NewItem.InstanceUID; // 钦己篮 酒捞袍捞 酒抗 官诧促. 牢亥配府 曼绊甫 困秦 UID 府畔
//}
//
int32 FClientDataStore::EnhanceItem(const FB2EnhanceItem& EnhancedItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_EnhanceItem);
	//ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseEnhanceItem::current_money_index, EnhancedItem));

	//FB2Item TargetItem;
	//TargetItem = GET_TUPLE_DATA(FB2ResponseEnhanceItem::enhanced_item_index, EnhancedItem);
	//LocalCharacterData.UserAllItems.ChangeItem(TargetItem);

	//{
	//	GetB2ItemArray(FB2ResponseEnhanceItem::changed_enhance_scroll_items_index, EnhancedItem);
	//	LocalCharacterData.UserAllItems.ChangeItems(B2ItemArray);
	//}
	//{
	//	GetB2ItemArray(FB2ResponseEnhanceItem::changed_protection_scroll_items_index, EnhancedItem);
	//	LocalCharacterData.UserAllItems.ChangeItems(B2ItemArray);
	//}
	////////////////////////////////////////////////////////////////////
	////咀记 瘤钎 肺弊 (酒捞袍 碍拳)
	//FString itemName(TEXT("ITEM_ENHANCE_NONE"));
	//UB2ItemInfo* IteminfoTable(StaticFindItemInfo());
	//FSingleItemInfoData* ThisItemInfo = IteminfoTable ? IteminfoTable->GetInfoData(TargetItem.ItemRefID) : NULL;
	//if (ThisItemInfo)
	//	itemName = ThisItemInfo->GetLocalizedName().ToString().ToUpper();
	//B2ActionLogSender Sender;
	//Sender.setCategory(FString("ITEM"))
	//	.setAction(FString("ENHANCE"))
	//	.setLabel(itemName)
	//	.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	////////////////////////////////////////////////////////////////////

	//LocalCharacterData.UserAllItems.RemoveItems(GET_TUPLE_DATA(FB2ResponseEnhanceItem::deleted_enhance_scroll_item_ids_index, EnhancedItem));
	//LocalCharacterData.UserAllItems.RemoveItems(GET_TUPLE_DATA(FB2ResponseEnhanceItem::deleted_protection_scroll_item_ids_index, EnhancedItem));
	//bool IsSuccessEnhanceItem = GET_TUPLE_DATA(FB2ResponseEnhanceItem::is_success_index, EnhancedItem);
	int32 DeltaEnahnceLevel = 0;// GET_TUPLE_DATA(FB2ResponseEnhanceItem::changed_enhance_level_index, EnhancedItem);

	return DeltaEnahnceLevel;
}

void FClientDataStore::ItemOptionChange(const FB2Item& OptionAppliedItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ItemOptionChange);
	LocalCharacterData.UserAllItems.ChangeItem(OptionAppliedItem);
}

void FClientDataStore::ItemQualityChange(const FB2Item& OptionAppliedItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ItemOptionChange);
	LocalCharacterData.UserAllItems.ChangeItem(OptionAppliedItem);
}

void FClientDataStore::ResetTotem()
{
	LocalCharacterData.AllTotems.Empty();
	LocalCharacterData.EquippedTotems.Empty();
}

void FClientDataStore::AddTotem(const b2network::B2TotemServerInfoPtr TotemInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_AddTotem);

	FB2Totem fTotem(TotemInfo);
	LocalCharacterData.AllTotems.Add(fTotem.ID, fTotem);
}

void FClientDataStore::EquipTotems(const TArray<FB2Totem> EquippedTotems)
{
	for (FB2Totem TotemInfo : EquippedTotems)
	{
		if (LocalCharacterData.AllTotems.Contains(TotemInfo.ID))
		{
			if (LocalCharacterData.EquippedTotems.Contains(TotemInfo.OwnerPCClass) == false)
			{
				LocalCharacterData.EquippedTotems.Add(TotemInfo.OwnerPCClass, TMap<int64, FB2Totem>());
			}

			if (LocalCharacterData.EquippedTotems[TotemInfo.OwnerPCClass].Contains(TotemInfo.ID) == false)
			{
				LocalCharacterData.EquippedTotems[TotemInfo.OwnerPCClass].Add(TotemInfo.ID, TotemInfo);
			}
		}
	}
}

void FClientDataStore::EquipTotems(const FB2EquipTotemsPtr EquipTotemsPtr, EPCClass& TargetPCClass, ETotemEquipSlot& TargetEquipSlot)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_EquipTotems);
	\
		for (int64 InstanceID : EquipTotemsPtr->unequiped_totem_ids)
		{
			if (LocalCharacterData.AllTotems.Contains(InstanceID))
			{
				if (LocalCharacterData.EquippedTotems.Contains(TargetPCClass) == false)
				{
					LocalCharacterData.EquippedTotems.Add(TargetPCClass, TMap<int64, FB2Totem>());
				}

				if (LocalCharacterData.EquippedTotems[TargetPCClass].Contains(InstanceID))
				{
					LocalCharacterData.EquippedTotems[TargetPCClass].Remove(InstanceID);
				}
			}

			LocalCharacterData.AllTotems[InstanceID].OwnerPCClass = EPCClass::EPC_End;
			LocalCharacterData.AllTotems[InstanceID].EquipSlot = ETotemEquipSlot::TOTEM_NONE;
		}

	for (int64 InstanceID : EquipTotemsPtr->equiped_totem_ids)
	{
		if (LocalCharacterData.AllTotems.Contains(InstanceID))
		{
			LocalCharacterData.AllTotems[InstanceID].OwnerPCClass = TargetPCClass;
			LocalCharacterData.AllTotems[InstanceID].EquipSlot = TargetEquipSlot;

			if (LocalCharacterData.EquippedTotems.Contains(TargetPCClass) == false)
			{
				LocalCharacterData.EquippedTotems.Add(TargetPCClass, TMap<int64, FB2Totem>());
			}

			if (LocalCharacterData.EquippedTotems[TargetPCClass].Contains(InstanceID) == false)
			{
				LocalCharacterData.EquippedTotems[TargetPCClass].Add(InstanceID, LocalCharacterData.AllTotems[InstanceID]);
			}
		}
	}
}

void FClientDataStore::UnequipTotems(const FB2UnequipTotemsPtr UnequipTotemsPtr, EPCClass& TargetPCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_UnequipTotems);

	for (int64 InstanceID : UnequipTotemsPtr->unequiped_totem_ids)
	{
		if (LocalCharacterData.AllTotems.Contains(InstanceID))
		{
			if (LocalCharacterData.EquippedTotems.Contains(TargetPCClass) == false)
			{
				LocalCharacterData.EquippedTotems.Add(TargetPCClass, TMap<int64, FB2Totem>());
			}

			if (LocalCharacterData.EquippedTotems[TargetPCClass].Contains(InstanceID))
			{
				LocalCharacterData.EquippedTotems[TargetPCClass].Remove(InstanceID);
			}

			LocalCharacterData.AllTotems[InstanceID].OwnerPCClass = EPCClass::EPC_End;
			LocalCharacterData.AllTotems[InstanceID].EquipSlot = ETotemEquipSlot::TOTEM_NONE;
		}
	}
}

void FClientDataStore::LockTotems(const FB2LockTotemsPtr LockTotemsPtr)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_LockTotems);

	for (b2network::B2TotemServerInfoPtr TotemInfo : LockTotemsPtr->locked_totems)
	{
		if (LocalCharacterData.AllTotems.Contains(TotemInfo->id))
		{
			LocalCharacterData.AllTotems[TotemInfo->id].IsLock = TotemInfo->locked;
		}

		EPCClass EquippedPCClass = SvrToCliPCClassType(TotemInfo->character_type);
		if (EquippedPCClass != EPCClass::EPC_End)
		{
			if (LocalCharacterData.EquippedTotems.Contains(EquippedPCClass) && LocalCharacterData.EquippedTotems[EquippedPCClass].Contains(TotemInfo->id))
			{
				LocalCharacterData.EquippedTotems[EquippedPCClass][TotemInfo->id].IsLock = TotemInfo->locked;
			}
		}
	}
}

void FClientDataStore::UnlockTotems(const FB2UnlockTotemsPtr UnlockTotemsPtr)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_UnlockTotems);

	for (b2network::B2TotemServerInfoPtr TotemInfo : UnlockTotemsPtr->unlocked_totems)
	{
		if (LocalCharacterData.AllTotems.Contains(TotemInfo->id))
		{
			LocalCharacterData.AllTotems[TotemInfo->id].IsLock = TotemInfo->locked;
		}

		EPCClass EquippedPCClass = SvrToCliPCClassType(TotemInfo->character_type);
		if (EquippedPCClass != EPCClass::EPC_End)
		{
			if (LocalCharacterData.EquippedTotems.Contains(EquippedPCClass) && LocalCharacterData.EquippedTotems[EquippedPCClass].Contains(TotemInfo->id))
			{
				LocalCharacterData.EquippedTotems[EquippedPCClass][TotemInfo->id].IsLock = TotemInfo->locked;
			}
		}
	}
}

void FClientDataStore::SellTotem(const FB2SellTotemsPtr SellTotemsPtr)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SellTotem);

	ReplaceUserDataWithDoc(EDocUserDataType::Gold, SellTotemsPtr->current_gold);

	for (int64 InstanceID : SellTotemsPtr->sell_totem_ids)
	{
		if (LocalCharacterData.AllTotems.Contains(InstanceID))
		{
			LocalCharacterData.AllTotems.Remove(InstanceID);
		}
	}
}

void FClientDataStore::DecompositionTotem(const FB2DismantleTotemsPtr DismantleTotemsPtr)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_DecompositionTotem);

	OnResponseConsumableAmountIncrease(DismantleTotemsPtr->current_dimensional_piece->template_id, DismantleTotemsPtr->current_dimensional_piece);
	ReplaceUserDataWithDoc(EDocUserDataType::DimensionPiece, DismantleTotemsPtr->current_dimensional_piece->amount);

	for (int64 InstanceID : DismantleTotemsPtr->dismantle_totem_ids)
	{
		if (LocalCharacterData.AllTotems.Contains(InstanceID))
		{
			LocalCharacterData.AllTotems.Remove(InstanceID);
		}
	}
}

void FClientDataStore::SmeltingTotem(const FB2RefineTotemPtr RefineTotemPtr, const int32& ServerRefineCostType)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_FB2RefineTotemPtr);

	if (ServerRefineCostType == b2network::B2TotemRefineCostType::DIMENSIONAL_PIECE)
	{
		OnResponseConsumableAmountDecrease(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE,
			-1 /* 抗傈 内靛狼 儡犁肺 橇肺配妮 惑俊 粮犁窍瘤 臼栏搁 澜荐肺 逞败拎档 等促. */,
			RefineTotemPtr->changed_dimensional_piece_items,
			RefineTotemPtr->deleted_dimensional_piece_ids
		);

		if (RefineTotemPtr->changed_dimensional_piece_items.IsValidIndex(0))
		{
			ReplaceUserDataWithDoc(EDocUserDataType::DimensionPiece, RefineTotemPtr->changed_dimensional_piece_items[0]->amount);
		}
		else if (RefineTotemPtr->deleted_dimensional_piece_ids.IsValidIndex(0))
		{
			ReplaceUserDataWithDoc(EDocUserDataType::DimensionPiece, 0);
		}
	}
	else
	{
		ReplaceUserDataWithDoc(EDocUserDataType::Gold, RefineTotemPtr->current_gold);
	}

	// 背眉
	if (LocalCharacterData.AllTotems.Contains(RefineTotemPtr->refined_totem->id))
	{
		LocalCharacterData.AllTotems.Remove(RefineTotemPtr->refined_totem->id);
		AddTotem(RefineTotemPtr->refined_totem);
	}
}

void FClientDataStore::RequestCollectionItem(EPCClass TargetClass, ECollectionType CollectionType)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_RequestCollectionItem);
	int32 SvrPCype = CliToSvrPCClassType(TargetClass);
	int32 SvrEquipCategory = CliToSvrCollectionCategory(CollectionType);

	ItemMasterData.CharacterItems[TargetClass].ClearCollectionItems(CollectionType);

	data_trader::Retailer::GetInstance().RequestGetCollectionItem(SvrPCype, SvrEquipCategory);
}

void FClientDataStore::SetCollectionItemData(FB2CollectionItemPtr ServerCollections)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetCollectionItemData);
	if (ServerCollections)
	{
		for (b2network::B2CollectionItemPtr ItemPtr : ServerCollections->collection_items)
		{
			if (ItemPtr != nullptr)
			{
				FCollectionItemData CollectionItem;
				CollectionItem = ItemPtr;
				EPCClass TargetClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(CollectionItem.ID);
				EItemClass ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(CollectionItem.ID);

				if (ItemMasterData.CharacterItems.Contains(TargetClass))
				{
					ItemMasterData.CharacterItems[TargetClass].AddCollectionItem(ItemClass, CollectionItem);
					ItemMasterData.CharacterItems[TargetClass].AddSetCollectionItem(CollectionItem);
				}
			}
		}

		for (b2network::B2CollectionSetItemPtr SetRewardPtr : ServerCollections->reward_collection_items)
		{
			if (SetRewardPtr != nullptr)
			{
				if (SetRewardPtr->group_id == b2network::B2GroupPrefix::GROWTH
					|| SetRewardPtr->group_id == b2network::B2GroupPrefix::NON_EQUIP
					//|| SetRewardPtr->group_id == b2network::B2GroupPrefix::ETERNAL
					|| SetRewardPtr->group_id == b2network::B2GroupPrefix::DUEL)
				{
					// 己厘 厘脚备, 沥荐 技飘 酒捞袍 力寇
					continue;
				}

				EPCClass TargetClass = SvrToCliPCClassType(SetRewardPtr->character_type);
				int32 SetUniqueKey = GetSetItemUniqueKey(SetRewardPtr->group_id, TargetClass, SetRewardPtr->grade);

				FCollectionItemData CollectionSet;
				CollectionSet.ID = SetUniqueKey;
				CollectionSet.CollectionState = EItemCollectionState::Enhanced;

				if (ItemMasterData.CharacterItems.Contains(TargetClass)) {
					ItemMasterData.CharacterItems[TargetClass].AddCollectionItem(EItemClass::EIC_SetItem, CollectionSet);
				}
			}
		}

		// reward_collection_items 俊 甸绢柯 Set备己 (group_id, grade, character_type)篮 焊惑 罐阑 荐 乐绰 惑炔俊辑父 甸绢柯促.
		// 捞固 焊惑阑 罐篮 技飘俊 措秦辑绰 菩哦阑 焊郴瘤 臼澜 ( 磊盔 扯厚且 鞘夸啊 绝澜 )
		// 蝶扼辑 Set狼 葛电 备己前阑 葛疽栏唱 reward_collection_items俊 甸绢坷瘤 臼篮 SetItem篮 碍力 Rewarded 贸府甫 茄促.
		for (TMap<EPCClass, FClassCollectionItemData>::TIterator It(ItemMasterData.CharacterItems); It; ++It)
		{
			FClassCollectionItemData& CollectionItemData = It.Value();
			CollectionItemData.UpdateSetCollectionState();
		}

	}
}

void FClientDataStore::SyncingInventoryData(EPCClass PCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SyncingInventoryData);
	if (PCClass == EPCClass::EPC_End)
	{ // 捞 版快 傍侩 家葛前 夸没栏肺..
		if (!LocalCharacterData.UserAllItems.bSyncingConsumables)
		{
			data_trader::Retailer::GetInstance().RequestGetAccountItem(b2network::B2CharacterType::Common, b2network::B2InventoryType::Consumables);
		}
	}
	else if (
		LocalCharacterData.UserAllItems.CharacterItemContainers.IsValidIndex(PCClassToInt(PCClass)) &&
		!LocalCharacterData.UserAllItems.CharacterItemContainers[PCClassToInt(PCClass)].bSyncingData
		)
	{
		data_trader::Retailer::GetInstance().RequestGetAccountItem(CliToSvrPCClassType(PCClass), 0);
	}
}

void FClientDataStore::BladePointCharge(const int32 InPoint, const int32 InPointMax, const int32 InLefttime)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_BladePointCharge);
	if (!AccountInfo)
		return;

	AccountInfo->blade_point = InPoint;
	AccountInfo->blade_point_max = InPointMax;
	AccountInfo->blade_point_charge_lefttime_second = InLefttime;
}

void FClientDataStore::StartStage(const FB2StartStageInfo& StartStageInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_StartStage);
	//[todo] 捞扒峦....
	//AccountInfo = GET_TUPLE_DATA(FB2ResponseStartStage::account_index, StartStageInfo);
}

void FClientDataStore::ClearStage(const FB2ClearStageInfo& ClearStageInfo, FServerStageID ServerStageId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ClearStage);
	//B2_SCOPED_TRACK_LOG(TEXT("FClientDataStore::ClearStage"));

	//auto characters = GET_TUPLE_DATA(FB2ResponseClearStage::characters_index, ClearStageInfo);

	//check(characters.Num() == 2);

	////版氰摹 沥焊
	//for (auto charItem : characters)
	//{
	//	EndGameIncreaseExp(charItem);
	//}

	//// Item
	//GetB2ItemArray(FB2ResponseClearStage::items_index, ClearStageInfo);
	//// Update Item &  BossPiece 
	//TU::CopyVectorHelper<FB2Item, b2network::B2ItemServerInfoPtr> ConvertUpdateItems(&B2ItemArray);
	//ConvertUpdateItems = GET_TUPLE_DATA(FB2ResponseClearStage::update_items_index, ClearStageInfo);

	//LocalCharacterData.UserAllItems.AddNewAcquiredItems(B2ItemArray); // 货肺 掘篮 巴捞骨肺 AddNewAcquiredItems 肺

	//																  // 蜡拱捞 坷锹登菌阑锭 眠啊.
	//const TArray<B2RelicPtr>& RelicInfos = GET_TUPLE_DATA(FB2ResponseClearStage::created_relics_index, ClearStageInfo);
	//for (auto RelicItem : RelicInfos)
	//{
	//	LocalCharacterData.SetRelicInfo(RelicItem);
	//}

	////[todo] 捞扒峦....
	//AccountInfo = GET_TUPLE_DATA(FB2ResponseClearStage::account_index, ClearStageInfo);
	//ChangeUserAccountInfoClass<>::GetInstance().Signal();
	//LobbyUpdateChapterDataClass<>::GetInstance().Signal();
}

void FClientDataStore::ClearStage(const FB2SweepStageInfo& SweepStageInfo)
{
	//auto characters = GET_TUPLE_DATA(FB2ResponseSweepStage::characters_index, SweepStageInfo);

	//check(characters.Num() == 2);

	////版氰摹 沥焊
	//for (auto charItem : characters)
	//{
	//	EndGameIncreaseExp(charItem);
	//}

	//// Item
	//GetB2ItemArray(FB2ResponseSweepStage::items_index, SweepStageInfo);
	//// Update Item &  BossPiece 
	//TU::CopyVectorHelper<FB2Item, b2network::B2ItemServerInfoPtr> ConvertUpdateItems(&B2ItemArray);
	//ConvertUpdateItems = GET_TUPLE_DATA(FB2ResponseSweepStage::update_items_index, SweepStageInfo);

	//LocalCharacterData.UserAllItems.AddNewAcquiredItems(B2ItemArray); // 货肺 掘篮 巴捞骨肺 AddNewAcquiredItems 肺

	//																  // 蜡拱捞 坷锹登菌阑锭 眠啊.
	//const TArray<B2RelicPtr>& RelicInfos = GET_TUPLE_DATA(FB2ResponseSweepStage::created_relics_index, SweepStageInfo);
	//for (auto RelicItem : RelicInfos)
	//{
	//	LocalCharacterData.SetRelicInfo(RelicItem);
	//}

	//ChangeUserAccountInfoClass<>::GetInstance().Signal();
	//LobbyUpdateChapterDataClass<>::GetInstance().Signal();
}

FB2OriginCharacterInfoPtr FClientDataStore::CheckEndGameIncreaseExp(const EPCClass PCClass, const int32 LatestLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_CheckEndGameIncreaseExp);
	B2_SCOPED_TRACK_LOG(TEXT("FClientDataStore::CheckEndGameIncreaseExp"));

	FB2OriginCharacterInfoPtr pCharInfo = nullptr;

	int32 CharacterIndex = PCClassToInt(PCClass);

	if (UB2UIDocHero* pDocHero = UB2UIDocHelper::GetDocHero(CharacterIndex))
	{
		//pDocHero->InitExp();
		FLocalCharacterData& getLocalCharData = GetLocalCharacterData();

		int32 BeforeLevel = 0;

		if (getLocalCharData.PlayerSharedStats.IsValidIndex(CharacterIndex) &&
			getLocalCharData.PlayerSharedStats[CharacterIndex].BasicInfo)
		{
			pCharInfo = getLocalCharData.PlayerSharedStats[CharacterIndex].BasicInfo;
			getLocalCharData.PlayerSharedStats[CharacterIndex].UpdateBeforeExpLevelInfo();
			BeforeLevel = pCharInfo->level;
		}

		if (LatestLevel > BeforeLevel) // 饭骇诀阑 窍瘤 臼疽促搁, 胶懦器牢飘甫 拌魂窍瘤 臼绰促.
		{
			pDocHero->SetLastAcquiredSkillPoint(LatestLevel - BeforeLevel);
		}

		//pDocHero->UpdateExp();
		pDocHero->SetPowers(BeforeLevel);
		pDocHero->SetPostClearPowers(LatestLevel);
	}

	return pCharInfo;
}

void FClientDataStore::EndGameIncreaseExp(const FB2OriginCharacterInfoPtr CharInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_EndGameIncreaseExp);
	B2_SCOPED_TRACK_LOG(TEXT("FClientDataStore::EndGameIncreaseExp_CharInfo"));

	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
	if (CharInfo == nullptr)
	{
		return;
	}

	//if (FB2OriginCharacterInfoPtr pCharInfo = CheckEndGameIncreaseExp(SvrToCliPCClassType(CharInfo->character_type), CharInfo->level))
	//{
	//	*pCharInfo = *CharInfo;
	//}

	// FB2OriginCharacterInfoPtr 饭骇沥焊 盎脚等 促澜俊 沥惑累悼窍骨肺 CheckEndGameIncreaseExp窃荐俊辑 话澜.
	//CheckNeedRegistSkill(SvrToCliPCClassType(CharInfo->character_type));
}

void FClientDataStore::EndGameIncreaseExp(const FB2LightCharacterPtr ExpInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_EndGameIncreaseExp);
	B2_SCOPED_TRACK_LOG(TEXT("FClientDataStore::EndGameIncreaseExp_ExpInfo"));

	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
	if (ExpInfo == nullptr)
	{
		return;
	}

	//if (FB2OriginCharacterInfoPtr pCharInfo = CheckEndGameIncreaseExp(SvrToCliPCClassType(ExpInfo->character_type), ExpInfo->level))
	//{
	//	pCharInfo->character_type = ExpInfo->character_type;
	//	pCharInfo->level = ExpInfo->level;
	//	pCharInfo->exp = ExpInfo->exp;
	//	pCharInfo->exp_max = ExpInfo->exp_max;
	//	pCharInfo->remain_skill_point = ExpInfo->remain_skill_point;
	//}

	// FB2OriginCharacterInfoPtr 饭骇沥焊 盎脚等 促澜俊 沥惑累悼窍骨肺 CheckEndGameIncreaseExp窃荐俊辑 话澜.
	//CheckNeedRegistSkill(SvrToCliPCClassType(ExpInfo->character_type));
}

void FClientDataStore::CheckNeedRegistSkill(EPCClass InPCClass)
{
	// 阿 胶懦 浇吩付促 脚痹 胶懦 乐绰瘤 眉农~	
	int32 CharacterIndex = PCClassToInt(InPCClass);
	const int32 SkillSlotMax = GetSkillPresetMaxCount();

	for (int32 i = 0; i < SkillSlotMax; ++i)
	{
		GetLocalCharacterData().PlayerSharedStats[CharacterIndex].CheckNeedRegistSkill(static_cast<EItemPresetSlotType>(i));
	}
}

int32 FClientDataStore::GetCharacterInvenSlotCount(EPCClass InPCClass, EItemInvenType InInvenType)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetCharacterInvenSlotCount);
	int RetSlotCount = 0;
	if (InPCClass == EPCClass::EPC_End)
		return RetSlotCount;

	if (InInvenType == EItemInvenType::EIIVT_Consumables && AccountInfo) // 家葛前篮 傍蜡登哥 沥秦柳 蔼.
		RetSlotCount = AccountInfo->inventory_consumable_slot_count;

	if (auto RelevantCharacterInfoPtr = LocalCharacterData.GetPlayerStatBasicInfo(InPCClass))
	{
		RetSlotCount = RelevantCharacterInfoPtr->inventory_item_slot_count;
	}
	/*if (auto RelevantCharacterInfoPtr = LocalCharacterData.GetPlayerStatBasicInfo(InPCClass))
	{
	switch (InInvenType)
	{
	case EItemInvenType::EIIVT_Weapon: RetSlotCount = RelevantCharacterInfoPtr->inventory_weapon_slot_count; break;
	case EItemInvenType::EIIVT_Protection: RetSlotCount = RelevantCharacterInfoPtr->inventory_armor_slot_count; break;
	case EItemInvenType::EIIVT_Accessory: RetSlotCount = RelevantCharacterInfoPtr->inventory_accessory_slot_count; break;
	}
	}*/

	return RetSlotCount;
}

const TArray<FCounterDungeonPeriod>* FClientDataStore::GetCounterDungeonPeriodData(int32 type)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetCounterDungeonPeriodData);
	return CounterDungeonPeriodData.Find(type);
}

void FClientDataStore::SetAccountKickedGuildID(const FB2OriginAccountInfoPtr Value)
{
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	check(GuildDoc)

		GuildDoc->SetKickedGuildId(Value->kicked_guild_id);
}

void FClientDataStore::SetUserGuildInfo(FUserGuildInfo GuildInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetUserGuildInfo);
	UserGuildInfo.GuildName = GuildInfo.GuildName;
	UserGuildInfo.GuildRank = GuildInfo.GuildRank;
	UserGuildInfo.Mark_Index = GuildInfo.Mark_Index;
	UserGuildInfo.MarkColor_Index = GuildInfo.MarkColor_Index;
	UserGuildInfo.MarkBG_Index = GuildInfo.MarkBG_Index;
	UserGuildInfo.MarkBGColor_Index = GuildInfo.MarkBGColor_Index;
}

void FClientDataStore::AddGold(int32 ToAdd)
{
	if (AccountInfo)
	{
		int32 Remain = MAX_GOLD - AccountInfo->money;

		if (ToAdd > Remain)
			AccountInfo->money = MAX_GOLD;
		else
			AccountInfo->money += ToAdd;
	}
}

int32 FClientDataStore::GetCounterDungeonTypeNumber(int32 InDungeonType)
{
	int32 DungeonTypeNumberCount = 0;
	for (auto El : CounterDungeonData)
	{
		if (El.DungeonType == InDungeonType)
			++DungeonTypeNumberCount;
	}

	return DungeonTypeNumberCount;
}

const TArray<FRaidMasterData>* FClientDataStore::GetRaidMasterDataList(int32 RaidId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRaidMasterDataList);
	return RaidMasterDatas.Find(RaidId);
}

FText FClientDataStore::GetLocalCharClassName(EPCClass InPCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetLocalCharClassName);
	int8 PCI = PCClassToInt(InPCClass);
	if (LocalCharacterData.CharNames.IsValidIndex(PCI))
	{
		return LocalCharacterData.CharNames[PCI].ClassName;
	}
	return FText::FromString(TEXT("UnknownPC"));
}

FText FClientDataStore::GetLocalCharClassOrNickName(EPCClass InPCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetLocalCharClassOrNickName);
	int8 PCI = PCClassToInt(InPCClass);
	if (LocalCharacterData.CharNames.IsValidIndex(PCI))
	{
		return LocalCharacterData.CharNames[PCI].GetClassOrNickName();
	}
	return FText::FromString(TEXT("UnknownPC"));
}

const TMap<int32, b2network::B2mdGuildBattleResultRewardPtr>& FClientDataStore::GetGuildBattleResultRewardArr()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetGuildBattleResultRewardArr);
	return GuildBattleResultRewardMasterData;
}

const b2network::B2mdGuildBattleResultRewardPtr FClientDataStore::GetGuildBattleResultReward(int32 medalCount)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetGuildBattleResultReward);

	b2network::B2mdGuildBattleResultRewardPtr RewardInfo;

	for (auto ResultInfo : GuildBattleResultRewardMasterData)
	{
		if (ResultInfo.Value->req_count_min <= medalCount && ResultInfo.Value->req_count_max >= medalCount)
			return ResultInfo.Value;
		else
			RewardInfo = ResultInfo.Value;
	}
	return RewardInfo;
}

const b2network::B2mdGuildBattleResultRewardPtr FClientDataStore::GetGuildBattleResultRewardGrade(int32 Grade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetGuildBattleResultRewardGrade);
	if (GuildBattleResultRewardMasterData.Contains(Grade))
		return GuildBattleResultRewardMasterData[Grade];

	return nullptr;
}

int32 FClientDataStore::GetGuildBattleResultRewardMaxGrade()
{
	int32 MaxGrade = 0;

	for (auto ResultInfo : GuildBattleResultRewardMasterData)
	{
		if (MaxGrade <= ResultInfo.Value->reward_grade)
			MaxGrade = ResultInfo.Value->reward_grade;
	}

	return MaxGrade;
}

void FClientDataStore::AddRankPromotionPoint(int32 ToAdd)
{// 抗傈俊 柳鞭器牢飘甫 犁拳 秒鞭窍带 矫例狼 儡犁. 捞扒 柳鞭籍捞 绝绰 惑怕俊辑 捞犯霸 荤侩窍霸 登搁 粱 帮鄂且 荐 乐促.
 // 橇肺配妮 磊眉啊 刘啊 荐摹父 甸绢客辑绰 帮鄂窍绊 Item 备炼眉啊 甸绢客具 窃. 努扼俊急 烙矫 贸府啊 啊瓷窍促绊 秦档 UID 绰 何咯窍瘤 给茄促.
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_AddRankPromotionPoint);
	const int32 PrevAmount = GetRankPromotionPoint();

	LocalCharacterData.UserAllItems.IncSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE, ToAdd);

	//if (PrevAmount == 0)
	//{ // 官恩流茄 惑炔篮 酒聪瘤父 货肺 积扁搁 酒捞能 诀单捞飘 秦拎具瘤 せせ
	//	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//}
}
int32 FClientDataStore::GetRankPromotionPoint()
{ // 抗傈俊 柳鞭器牢飘甫 犁拳 秒鞭窍带 矫例狼 儡犁. 窜 捞扒 弊措肺 结档 公规 
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRankPromotionPoint);
	return UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE);
}
void FClientDataStore::AddAdvancedRankPromotionPoint(int32 ToAdd)
{// 抗傈俊 柳鞭器牢飘甫 犁拳 秒鞭窍带 矫例狼 儡犁. 捞扒 柳鞭籍捞 绝绰 惑怕俊辑 捞犯霸 荤侩窍霸 登搁 粱 帮鄂且 荐 乐促.
 // 橇肺配妮 磊眉啊 刘啊 荐摹父 甸绢客辑绰 帮鄂窍绊 Item 备炼眉啊 甸绢客具 窃. 努扼俊急 烙矫 贸府啊 啊瓷窍促绊 秦档 UID 绰 何咯窍瘤 给茄促.
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_AddAdvancedRankPromotionPoint);
	const int32 PrevAmount = GetAdvancedRankPromotionPoint();

	LocalCharacterData.UserAllItems.IncSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ADVANCED_BREVET_STONE, ToAdd);

	//if (PrevAmount == 0)
	//{ // 官恩流茄 惑炔篮 酒聪瘤父 货肺 积扁搁 酒捞能 诀单捞飘 秦拎具瘤 せせ
	//	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//}
}
int32 FClientDataStore::GetAdvancedRankPromotionPoint()
{ // 抗傈俊 柳鞭器牢飘甫 犁拳 秒鞭窍带 矫例狼 儡犁. 窜 捞扒 弊措肺 结档 公规 
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetAdvancedRankPromotionPoint);
	return UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ADVANCED_BREVET_STONE);
}

void FClientDataStore::ApplyRewardInfo(b2network::B2RewardPtr RewardPtr)
{
	//if (RewardPtr)
	//{
	//	auto* RewardInfo = RewardInfoData.Find(RewardPtr->raw_reward->id);
	//	if (RewardInfo)
	//	{
	//		// 皋老捞搁 溜矫 盎脚 救窍档废 荐沥.
	//		if (RewardInfo->RewardPushType == b2network::B2RewardPushType::MAIL)
	//			return;

	//		if (RewardPtr->item)
	//		{
	//			// 家葛己 酒捞袍
	//			if (RewardPtr->item->inventory_type == b2network::B2InventoryType::Consumables)
	//			{
	//				OnResponseConsumableAmountIncrease(RewardPtr->item->template_id, RewardPtr->item);
	//			}
	//			else
	//			{
	//				// 老馆(厘厚) 酒捞袍甸篮 焊烹 蝶肺 牢亥配府肺 逞败林绰 内靛甫 荤侩窍绊 乐澜.
	//				// ClientDataStore.AddNewAcquiredItems()
	//			}
	//		}
	//		else
	//		{
	//			// 犁拳 酒捞袍
	//			AddRewardItemByType(RewardInfo->Type, RewardPtr->raw_reward->count);
	//		}
	//	}

	//	// 版氰摹
	//	if (RewardPtr->light_characters.Num() > 0)
	//	{
	//		for (auto ExpInfo : RewardPtr->light_characters)
	//		{
	//			BladeIIGameImpl::GetClientDataStore().EndGameIncreaseExp(ExpInfo);
	//		}
	//	}
	//}
}

void FClientDataStore::ApplyRewardInfo(const FCommonRewardInfo& RewardInfo)
{
	ERewardType RewardType = ERewardType::None;

	if (RewardInfo.bTemplateId)
	{
		RewardType = FItemRefIDHelper::GetRewardTypeByRefID(RewardInfo.RewardID);
	}
	else
	{
		auto* Reward = RewardInfoData.Find(RewardInfo.RewardID);
		if (Reward)
			RewardType = Reward->Type;
	}

	AddRewardItemByType(RewardType, RewardInfo.RewardAmount);

}

void FClientDataStore::ApplyRewardInfoList(const TArray<b2network::B2RewardPtr>& Rewards)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ApplyRewardInfoList);
	UB2UIDocUser* UIDocUser = UB2UIDocHelper::GetDocUser();
	for (auto Reward : Rewards)
	{
		ApplyRewardInfo(Reward);
	}
}

void FClientDataStore::AddRewardItemByType(ERewardType RewardType, int32 RewardAmount)
{
	switch (RewardType)
	{
	case ERewardType::Gold:
	{
		AddUserDataWithDoc(EDocUserDataType::Gold, RewardAmount);
		break;
	}
	case ERewardType::Gem:
	{
		AddUserDataWithDoc(EDocUserDataType::Gem, RewardAmount);
		break;
	}
	case ERewardType::BladePoint:
	{
		AddUserDataWithDoc(EDocUserDataType::BladePoint, RewardAmount);
		break;
	}
	case ERewardType::Social_Point:
	{
		AddUserDataWithDoc(EDocUserDataType::SocialPoint, RewardAmount);
		break;
	}
	case ERewardType::Dark_Crystal:
	{
		AddUserDataWithDoc(EDocUserDataType::DarkGem, RewardAmount);
		break;
	}
	case ERewardType::Hero_Piece:
	{
		AddUserDataWithDoc(EDocUserDataType::HeroPiece, RewardAmount);
		break;
	}
	case ERewardType::Rank_Promotion_Point:
	{
		AddUserDataWithDoc(EDocUserDataType::RankPromotionPoint, RewardAmount);
		break;
	}
	case ERewardType::Stage_Boost_Ticket:
	{
		AddUserDataWithDoc(EDocUserDataType::StageBoostTicket, RewardAmount);
		break;
	}
	case ERewardType::Shop_Ten_Lottery_Ticket:
	{
		AddShopTenLotteryTicket(RewardAmount);
		break;
	}
	case ERewardType::Hot_Time_Gold_Buff_Ticket:
	{
		AddGoldBuffTicket(RewardAmount);
		break;
	}
	case ERewardType::Hot_Time_Exp_Buff_Ticket:
	{
		AddExpBuffTicket(RewardAmount);
		break;
	}
	case ERewardType::Fame_Point:
	{
		AddFamePoint(RewardAmount);
		break;
	}
	case ERewardType::Stage_Auto_Clear_Ticket:
	{
		AddStageAutoClearTicket(RewardAmount);
		break;
	}
	}
}

void FClientDataStore::ReplaceUserDataWithDoc(EDocUserDataType type, int32 amount)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ReplaceUserDataWithDoc);
	//if (AccountInfo)
	//{
	//	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();

	//	switch (type)
	//	{
	//	case EDocUserDataType::Gold:
	//		SetGoldAmount(amount);
	//		if (DocUser)
	//			DocUser->SetGold(GetGoldAmount());
	//		break;

	//	case EDocUserDataType::Gem:
	//		SetGemAmount(amount);
	//		if (DocUser)
	//			DocUser->SetGem(GetGemAmount());
	//		break;

	//	case EDocUserDataType::DarkGem:
	//		SetDarkCrystal(amount);
	//		if (DocUser)
	//			DocUser->SetDarkCrystal(GetDarkCrystal());
	//		break;

	//	case EDocUserDataType::HeroPiece:
	//		if (DocUser)
	//			DocUser->SetHeroPiece(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE));
	//		break;
	//	case EDocUserDataType::DimensionPiece:
	//		if (DocUser)
	//			DocUser->SetDimensionPiece(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE));
	//		break;
	//	case EDocUserDataType::BladePoint:
	//		SetBladeCurPoint(amount);
	//		if (DocUser)
	//			DocUser->SetStaminaCur(GetBladeCurPoint());
	//		break;

	//	case EDocUserDataType::SocialPoint:
	//		SetSocialPoint(amount);
	//		if (DocUser)
	//			DocUser->SetSocialPoint(GetSocialPoint());
	//		break;
	//	case EDocUserDataType::RankPromotionPoint:
	//		if (DocUser)
	//			DocUser->SetRankPromotionPoint(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE));
	//		break;
	//	case EDocUserDataType::AdvancedRankPromotionPoint:
	//		if (DocUser)
	//			DocUser->SetRankPromotionPoint(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ADVANCED_BREVET_STONE));
	//		break;
	//	case EDocUserDataType::StageBoostTicket:
	//	{
	//		SetStageBoostTicket(amount);
	//		if (auto* DocBattleStage = UB2UIDocHelper::GetDocBattleStage())
	//			DocBattleStage->SetRepeatBattleBoostRemainCount(GetStageBoostTicket());
	//	}
	//	break;
	//	case EDocUserDataType::FamePoint:
	//		SetFamePoint(amount);
	//		if (DocUser)
	//			DocUser->SetFamePoint(GetFamePoint());
	//		break;
	//		break;
	//	case EDocUserDataType::StageAutoClearTicket:
	//		SetStageAutoClearTicket(amount);
	//		if (DocUser)
	//			DocUser->SetStageAutoClearTicket(GetStageAutoClearTicket());
	//		break;
	//	case EDocUserDataType::AllRefresh:
	//	default:
	//		// 葛电 单捞磐 盎脚
	//		DocUser->UpdateUserData();
	//		break;
	//	}
	//}
}

void FClientDataStore::AddUserDataWithDoc(EDocUserDataType type, int32 amount)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_AddUserDataWithDoc);
	//if (AccountInfo)
	//{
	//	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();

	//	switch (type)
	//	{
	//	case EDocUserDataType::Gold:
	//		AddGold(amount);
	//		if (DocUser)
	//			DocUser->SetGold(GetGoldAmount());
	//		break;

	//	case EDocUserDataType::Gem:
	//		AddGem(amount);
	//		if (DocUser)
	//			DocUser->SetGem(GetGemAmount());
	//		break;

	//	case EDocUserDataType::DarkGem:
	//		AddDarkCrystal(amount);
	//		if (DocUser)
	//			DocUser->SetDarkCrystal(GetDarkCrystal());
	//		break;

	//	case EDocUserDataType::HeroPiece:
	//		if (DocUser)
	//			DocUser->SetHeroPiece(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE));
	//		break;
	//	case EDocUserDataType::DimensionPiece:
	//		if (DocUser)
	//			DocUser->SetDimensionPiece(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE));
	//		break;
	//	case EDocUserDataType::BladePoint:
	//	{
	//		AddBladePoint(amount);

	//		// 喉饭捞靛 器牢飘绰 矫埃付促 眠啊 瞪 锭绰 局聪皋捞记阑 免仿窍瘤 臼酒具 登骨肺, 
	//		// UIheader俊辑 贸府窍瘤臼澜.
	//		UB2UIManager* pUIManager = UB2UIManager::GetInstance();
	//		if (pUIManager)
	//		{
	//			UB2UIHeader* pUI = NULL;
	//			pUI = pUIManager->GetUI<UB2UIHeader>(UIFName::Header);
	//			if (pUI) pUI->ApplyAddAmountAction(ERewardType::BladePoint);
	//		}

	//		if (DocUser)
	//			DocUser->SetStaminaCur(GetBladeCurPoint());
	//	}
	//	break;
	//	case EDocUserDataType::SocialPoint:
	//		AddSocialPoint(amount);
	//		if (DocUser)
	//			DocUser->SetSocialPoint(GetSocialPoint());
	//		break;
	//	case EDocUserDataType::RankPromotionPoint:
	//		if (DocUser)
	//			DocUser->SetRankPromotionPoint(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE));
	//		break;
	//	case EDocUserDataType::AdvancedRankPromotionPoint:
	//		if (DocUser)
	//			DocUser->SetRankPromotionPoint(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ADVANCED_BREVET_STONE));
	//		break;
	//	case EDocUserDataType::StageBoostTicket:
	//	{
	//		AddStageBoostTicket(amount);
	//		auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	//		if (DocBattleStage)
	//			DocBattleStage->SetRepeatBattleBoostRemainCount(GetStageBoostTicket());
	//	}
	//	break;
	//	case EDocUserDataType::FamePoint:
	//	{
	//		AddFamePoint(amount);
	//		if (DocUser)
	//			DocUser->SetFamePoint(GetFamePoint());
	//		break;
	//	}
	//	case EDocUserDataType::StageAutoClearTicket:
	//	{
	//		AddStageAutoClearTicket(amount);
	//		if (DocUser)
	//			DocUser->SetStageAutoClearTicket(GetStageAutoClearTicket());
	//		break;
	//	}
	//	case EDocUserDataType::AllRefresh:
	//	default:
	//		// 葛电 单捞磐 盎脚
	//		DocUser->UpdateUserData();
	//		break;
	//	}
	////}
}

//void FClientDataStore::UseSocialPoint()
//{
//	//////////////////////////////////////////////////////////////////
//	//咀记 瘤钎 肺弊 (快沥器牢飘 荤侩)
//	B2ActionLogSender Sender;
//	Sender.setCategory(FString("FRIEND"))
//		.setAction(FString("SOCIALPOINT_USE"))
//		.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
//	//////////////////////////////////////////////////////////////////
//}

int32 FClientDataStore::GetSkillLearnLevel(int32 SkillId) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSkillLearnLevel);
	auto* Skill = SkillMasterData.Find(SkillId);
	return Skill ? Skill->LearnLevel : -1;
}

int32 FClientDataStore::GetMaxSkillLevel(int32 SkillId) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMaxSkillLevel);
	auto* Skill = SkillMasterData.Find(SkillId);
	return Skill ? Skill->MaxSkillLevel : -1;
}

EPCClass FClientDataStore::GetSkillClass(int32 SkillId) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSkillClass);
	auto* Skill = SkillMasterData.Find(SkillId);
	return Skill ? Skill->UserClass : EPCClass::EPC_End;
}

int32 FClientDataStore::GetSkillMinLevelAcquiredOption(int32 SkillId, int32 OptionId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSkillMinLevelAcquiredOption);
	auto* Skill = SkillMasterData.Find(SkillId);

	int32 MinLv = INT_MAX;
	for (auto It = Skill->LevelOptionData.CreateConstIterator(); It; ++It)
	{
		if (It.Value().OptionId == OptionId && It.Key() < MinLv)
			MinLv = It.Key();
	}

	return MinLv;
}

float FClientDataStore::GetSkillPower(int32 SkillId, int32 SkillLv) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSkillPower);
	auto* Skill = SkillMasterData.Find(SkillId);

	float SkillPoser = 0;
	for (auto It = Skill->LevelOptionData.CreateConstIterator(); It; ++It)
	{
		if (It.Key() == SkillLv)
			SkillPoser += It.Value().OptionPower;
	}


	return SkillPoser;
}

int32 FClientDataStore::GetGemResetSkill()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetGemResetSkill);
	if (ConfigData)
		return ConfigData->skill_point_reset_price_gem;

	return 0;
}

int32 FClientDataStore::GetGemsBuySkillPoint()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetGemsBuySkillPoint);
	if (ConfigData)
		return ConfigData->skill_point_buy_price_gem;

	return 0;
}

int32 FClientDataStore::GetMaxSkillPointBuyable(EPCClass PCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMaxSkillPointBuyable);
	if (!ConfigData)
		return 0;

	if (auto CharInfo = LocalCharacterData.GetPlayerStatBasicInfo(PCClass))
		return (ConfigData->max_skill_point_buy_count - CharInfo->skill_point_buy_count);

	return 0;
}

int32 FClientDataStore::GetMaxSkillPoint()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMaxSkillPoint);
	if (!ConfigData)
		return 0;

	return ConfigData->max_skill_point_buy_count;
}

int32 FClientDataStore::GetMaxRaidCountPerDay()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMaxRaidCountPerDay);
	if (ConfigData)
		return ConfigData->max_raid_count_per_day;

	return 0;
}

int32 FClientDataStore::GetLimitOpenStageDifficulty()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetLimitOpenStageDifficulty);
	//[180712_YJ] 譬配府倔篮 绊妨窍瘤 臼澜.
	if (ConfigData)
		return ConfigData->max_open_stage_difficulty;

	return 1;
}

int32 FClientDataStore::GetMaxCounterDungeonTicketCount()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMaxCounterDungeonTicketCount);
	if (ConfigData)
		return ConfigData->counter_dungeon_daily_enter_count_max;

	return 0;
}

int32 FClientDataStore::GetMaxHeroTowerTicketCount()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMaxCounterDungeonTicketCount);
	if (ConfigData)
		return ConfigData->hero_tower_free_ticket_count_max;

	return 0;
}

int32 FClientDataStore::GetCurrentHeroTowerTicketCount()
{
	int32 CurrentHeroTowerTicket = GetMaxHeroTowerTicketCount() - GetRedDotManager().GetUsedHeroTowerTicket();

	UB2UIDocHeroTower* pDocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (pDocHerotower && pDocHerotower->m_ptrGetHeroTower)
	{
		CurrentHeroTowerTicket = pDocHerotower->m_ptrGetHeroTower->possible_try_count;
	}

	if (CurrentHeroTowerTicket < 0)
	{
		CurrentHeroTowerTicket = 0;
	}

	return CurrentHeroTowerTicket;
}

int32 FClientDataStore::GetRaidMaxStep(int32 RaidType)
{
	int32 MaxStep = 1;

	TArray<FRaidMasterData>& RaidInfos = RaidMasterDatas[RaidType];
	for (int32 i = 0; i < RaidInfos.Num(); ++i)
	{
		if (RaidInfos[i].RaidStep > MaxStep)
			MaxStep = RaidInfos[i].RaidStep;
	}

	return MaxStep;
}

void FClientDataStore::GetRaidMainReward(int32 RaidType, int32& RewardID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRaidRewardItem);
	int32 StepIdx = 1;
	RewardID = 0;

	if (RaidMasterDatas.Contains(RaidType) == false)
		return;

	if (RaidMasterDatas[RaidType].IsValidIndex(StepIdx) == false)
		return;

	FRaidMasterData RaidData = RaidMasterDatas[RaidType][StepIdx];
	RewardID = RaidData.MainRewardID;
}

FText FClientDataStore::GetRaidName(int32 RaidId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRaidName);
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString::Printf(TEXT("Raid_Name%d"), RaidId));
}

const FSkillMasterData* FClientDataStore::GetSkillMasterData(int32 SkillId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSkillMasterData);
	return SkillMasterData.Find(SkillId);
}

FB2EnhanceItemCost FClientDataStore::GetEnhanceCostData(int32 InEnhanceLevel)
{
	for (auto& EnhanceInfo : MD_EnhanceItemCostData)
	{
		if (EnhanceInfo.Value.EnhanceLevel == InEnhanceLevel)
		{
			return EnhanceInfo.Value;
		}
	}
	return FB2EnhanceItemCost();
}

bool FClientDataStore::GetPossibleFairyGift() const
{
	// 府坷啊 凯啡绰啊?
	if (FairyManager::GetInstance().GetIsOpenedFairy(EFairyType::FAIRY_RIO) == false)
	{
		return false;
	}

	// 胶抛捞瘤 惑俊辑 凯啡歹扼档 其绢府 GetFairy 登扁 傈捞扼搁 酒流 焊惑 裙垫捞 阂啊瓷窍促.
	FB2FairyStatusInfo StatusInfo;
	if (FairyManager::GetInstance().GetFairyInfo(EFairyType::FAIRY_RIO, StatusInfo))
	{
		if (StatusInfo.bClearStage == false)
		{
			return false;
		}
	}

	const auto& RemainTime = FDateTime::FromUnixTimestamp(NextFairyGiftTime / 1000) - UB2GameInstance::GetUTCNow();
	auto TimePeriod = RemainTime.GetTicks() < 0.0f;

	return TimePeriod;
}

void FClientDataStore::GetDiffSkillOption(int32 SkillId, int32 OldLevel, int32 NewLevel, TArray<FSkillOptionData>& DiffOptions)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetDiffSkillOption);
	DiffOptions.Empty();

	auto* SkillMD = GetSkillMasterData(SkillId);
	if (!SkillMD)
		return;

	TArray<FSkillOptionData> OldOptions;
	SkillMD->LevelOptionData.MultiFind(OldLevel, OldOptions);

	TArray<FSkillOptionData> NewOptions;
	SkillMD->LevelOptionData.MultiFind(NewLevel, NewOptions);

	DiffOptions.Empty();

	for (auto& NewOption : NewOptions)
	{
		bool bFound = false;
		for (auto& OldOption : OldOptions)
		{
			if (NewOption.OptionId == OldOption.OptionId)
			{
				bFound = true;
				if (NewOption.OptionValue != OldOption.OptionValue)
				{
					NewOption.OptionValue -= OldOption.OptionValue;
					DiffOptions.Add(NewOption);
				}
			}
		}

		if (!bFound)
			DiffOptions.Add(NewOption);
	}
}

void FClientDataStore::GetRandomCharacterData(FB2ModPlayerInfo& ModPlayerInfoData)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRandomCharacterData);
	BII_CHECK(AccountInfo);

	ModPlayerInfoData.SetNickName(AccountInfo->name);

	for (auto Char : LocalCharacterData.PlayerSharedStats)
	{
		TArray<FB2Item> EquippedItems;

		LocalCharacterData.GetEquippedItems(SvrToCliPCClassType(Char.BasicInfo->character_type), EquippedItems);
		ModPlayerInfoData.AddModCharInfo(Char.BasicInfo, EquippedItems);
	}
}

float FClientDataStore::GetAttackDamageBonusByGuildFormation(ETMFormation Formation, int32 FormationLV, int32 Position) const
{
	return GetBuffGuildFormation(Formation, FormationLV, b2network::B2BattleFormaionBuffType::BTBFBT_Attack, Position);
}

float FClientDataStore::GetReduceDamageBonusByGuildFormation(ETMFormation Formation, int32 FormationLV, int32 Position) const
{
	return GetBuffGuildFormation(Formation, FormationLV, b2network::B2BattleFormaionBuffType::BTBFBT_Defense, Position);
}

int32 FClientDataStore::GetUpgradeGuildFormationCost(ETMFormation Formation, int32 NextLv)
{
	// jwyim GuildFormation
	return 0;
	/*
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetUpgradeGuildFormationCost);
	auto* FormationData = GuildMatchFormationMasterData.Find(CliToSvrFormationType(Formation));
	//BII_CHECK(FormationData);
	if (!FormationData)
	return 0;

	auto* FormationLVData = FormationData->LevelInfo.Find(NextLv);
	BII_CHECK(FormationLVData);
	if (!FormationLVData)
	return 0;

	return FormationLVData->EnhanceGoldCost;*/
}
int32  FClientDataStore::GetMaxLevelTeamFormation(ETMFormation Formation) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMaxLevelTeamFormation);

	if (auto* FormationData = TeamMatchFormationMasterData.Find(CliToSvrFormationType(Formation)))
	{
		return FormationData->MaxLevel;
	}
	return 20;
}

float FClientDataStore::GetBuffTeamFormation(ETMFormation Formation, int32 FormationLV, int32 BuffType, int32 Position) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBuffTeamFormation);
	auto* FormationData = TeamMatchFormationMasterData.Find(CliToSvrFormationType(Formation));
	//BII_CHECK(FormationData);
	if (!FormationData)
		return 0;

	auto* FormationLVData = FormationData->LevelInfo.Find(FormationLV);

	if (!FormationLVData)
		return 0;

	// TODO formation buff
	int32 FormationPosition = Position;
	auto* FormationBuff = FormationLVData->BuffInfo.Find(FormationPosition);
	if (!FormationBuff)
		return 0;

	if (FormationBuff->BuffType1 == BuffType)
		return FormationBuff->BuffValue1;

	if (FormationBuff->BuffType2 == BuffType)
		return FormationBuff->BuffValue2;

	return 0;
}

float FClientDataStore::GetBuffGuildFormation(ETMFormation Formation, int32 FormationLV, int32 BuffType, int32 Position) const
{
	// jwyim GuildFormation
	//BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBuffGuildFormation);
	//auto* FormationData = GuildMatchFormationMasterData.Find(CliToSvrFormationType(Formation));
	////BII_CHECK(FormationData);
	//if (!FormationData)
	//	return 0;

	//auto* FormationLVData = FormationData->LevelInfo.Find(FormationLV);

	//if (!FormationLVData)
	//	return 0;

	//// TODO formation buff
	//int32 FormationPosition = Position;
	//auto* FormationBuff = FormationLVData->BuffInfo.Find(FormationPosition);
	//if (!FormationBuff)
	//	return 0;

	//if (FormationBuff->BuffType1 == BuffType)
	//	return FormationBuff->BuffValue1;

	//if (FormationBuff->BuffType2 == BuffType)
	//	return FormationBuff->BuffValue2;

	return 0;
}

float FClientDataStore::GetAttackDamageBonusByTeamFormation(ETMFormation Formation, int32 FormationLV, int32 Position) const
{
	return GetBuffTeamFormation(Formation, FormationLV, b2network::B2BattleFormaionBuffType::BTBFBT_Attack, Position);
}

float FClientDataStore::GetReduceDamageBonusByTeamFormation(ETMFormation Formation, int32 FormationLV, int32 Position) const
{
	return GetBuffTeamFormation(Formation, FormationLV, b2network::B2BattleFormaionBuffType::BTBFBT_Defense, Position);
}

bool FClientDataStore::IsExistFormationLevel(ETMFormation Formation, int32 FormationLV) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_IsExistFormationLevel);
	auto* FormationData = TeamMatchFormationMasterData.Find(CliToSvrFormationType(Formation));
	if (!FormationData)
		return false;

	auto* FormationLVData = FormationData->LevelInfo.Find(FormationLV);
	if (!FormationLVData)
		return false;

	return true;
}

int32 FClientDataStore::GetUpgradeFormationCost(ETMFormation Formation, int32 NextFormationLV) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetUpgradeFormationCost);
	auto* FormationData = TeamMatchFormationMasterData.Find(CliToSvrFormationType(Formation));
	BII_CHECK(FormationData);
	if (!FormationData)
		return 0;

	auto* FormationLVData = FormationData->LevelInfo.Find(NextFormationLV);
	BII_CHECK(FormationLVData);
	if (!FormationLVData)
		return 0;

	return FormationLVData->EnhanceGoldCost;
}

FAttendanceDayData FClientDataStore::GetAttendanceDayInfo(int32 InType, int32 InValue)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetAttendanceDayInfo);

	if (m_AttendanceDayDatas.Contains(InType) && m_AttendanceDayDatas[InType].IsValidIndex(InValue))
	{
		return m_AttendanceDayDatas[InType][InValue];
	}

	return FAttendanceDayData();
}

int32 FClientDataStore::GetAttendance_day()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetAttendance_day);
	if (AccountInfo)
	{
		return AccountInfo->attendance_day;
	}

	return 0;
}

int32 FClientDataStore::GetExpectedExpOfRaidGame(int32 RaidType, int32 RaidStep)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetExpectedExpOfRaidGame);
	FRaidExpMDSingleData* FoundElem = MD_RaidExpData.Find(FRaidExpMDSingleData::GetKey(RaidType, RaidStep));
	return FoundElem ? FoundElem->ExpectedExp : 0;
}

const TMap<int32, FItemPresetInfo> FClientDataStore::GetAllPresetItem(EPCClass PCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetAllPresetItem);
	const int8 PCI = PCClassToInt(PCClass);
	TMap<int32, FItemPresetInfo> PresetItems;

	if (LocalCharacterData.UserAllItems.CharacterItemContainers.IsValidIndex(PCI))
	{
		const FPerCharacterItemContainer& ThisCharContainer = LocalCharacterData.UserAllItems.CharacterItemContainers[PCI];

		PresetItems = ThisCharContainer.AllItemPresets;
	}

	return PresetItems;
}

bool FClientDataStore::HasUnappliedItemOption()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_HasUnappliedItemOption);
	return bHasUnappliedItemOption && GetRedDotManager().HasUnappliedItemRandomOption();
}

bool FClientDataStore::GetItemOptionUnappliedItem(FB2Item& UnappliedItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemOptionUnappliedItem);
	if (UB2LobbyInventory::FindStoredItem(UnappliedItem, UnAppliedItemUniqueInstanceID) ||
		UB2LobbyInventory::FindEquippedItem(UnappliedItem, UnAppliedItemUniqueInstanceID))
		return true;

	return false;
}

void FClientDataStore::SetUnappliedItemOption(FB2ResponseGetItemRandomOptionPtr ServerUnappliedItemOptions)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetUnappliedItemOption);
	if (ServerUnappliedItemOptions)
	{
		bHasUnappliedItemOption = true;

		UnAppliedOptions.Empty();
		UnAppliedItemUniqueInstanceID = ServerUnappliedItemOptions->item_id;

		EItemOption ItemOption1 = SvrToCliOptionType(ServerUnappliedItemOptions->random_option_id1);
		EItemOption ItemOption2 = SvrToCliOptionType(ServerUnappliedItemOptions->random_option_id2);
		EItemOption ItemOption3 = SvrToCliOptionType(ServerUnappliedItemOptions->random_option_id3);

		UnAppliedOptions.Add(FItemOption(ItemOption1, ServerUnappliedItemOptions->random_option_value1));
		UnAppliedOptions.Add(FItemOption(ItemOption2, ServerUnappliedItemOptions->random_option_value2));
		UnAppliedOptions.Add(FItemOption(ItemOption3, ServerUnappliedItemOptions->random_option_value3));

		// 泅 惑怕俊辑 InstanceID啊 绢恫 Class狼 item牢瘤 葛抚
		for (int32 ClassIdx = 0; ClassIdx < GetMaxPCClassNum(); ClassIdx++)
			SyncingInventoryData(IntToPCClass(ClassIdx));
	}
}

void FClientDataStore::ClearUnappliedItemOptionInfo()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ClearUnappliedItemOptionInfo);
	UnAppliedOptions.Empty();
	UnAppliedItemUniqueInstanceID = 0;
	bHasUnappliedItemOption = false;
}

bool FClientDataStore::HasUnappliedItemQuality()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_HasUnappliedItemQuality);
	return bHasUnappliedItemQuality && GetRedDotManager().HasUnappliedItemRandomQuality();
	//return bHasUnappliedItemQuality;
}

bool FClientDataStore::GetItemQualityUnappliedItem(FB2Item& UnappliedItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemQualityUnappliedItem);
	if (UB2LobbyInventory::FindStoredItem(UnappliedItem, UnAppliedItem.InstanceUID) ||
		UB2LobbyInventory::FindEquippedItem(UnappliedItem, UnAppliedItem.InstanceUID))
		return true;

	return false;
}

void FClientDataStore::SetUnappliedItemQuality(FB2ResponseGetItemRandomQualityPtr ServerUnappliedItemQuality)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetUnappliedItemQuality);
	if (ServerUnappliedItemQuality)
	{
		bHasUnappliedItemQuality = true;

		// 		UnAppliedOptions.Empty();
		// 		UnAppliedItemUniqueInstanceID = ServerUnappliedItemQuality->item_id;
		// 
		// 		EItemOption ItemOption1 = SvrToCliOptionType(ServerUnappliedItemOptions->random_option_id1);
		// 
		// 		UnAppliedOptions.Add(FItemOption(ItemOption1, ServerUnappliedItemOptions->random_option_value1));
		// 
		// 		// 泅 惑怕俊辑 InstanceID啊 绢恫 Class狼 item牢瘤 葛抚
		// 		for (int32 ClassIdx = 0; ClassIdx < GetMaxPCClassNum(); ClassIdx++)
		// 			SyncingInventoryData(IntToPCClass(ClassIdx));
	}
}

void FClientDataStore::ClearUnappliedItemQualityInfo()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ClearUnappliedItemQualityInfo);
	UnAppliedItem.Empty();
	bHasUnappliedItemQuality = false;
}

void FClientDataStore::UpdateCombatStateScale(const float fScale)
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	extern float gLocalPCCombatStatScale;
	gLocalPCCombatStatScale = fScale;
#endif
}

bool FClientDataStore::GetEventAttendanceRewardData(TArray<FEventAttendanceReward>& AttendanceRewards, const int32 eventId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetEventAttendanceRewardData);
	if (nullptr == EventAttendanceRewardData.Find(eventId))
		return false;

	AttendanceRewards = EventAttendanceRewardData[eventId];
	return true;
}

void FClientDataStore::SetDailyAttendanceInfo(const FB2ReceiveDailyAttendance& AttendanceInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseDailyAttendance);
	//bool isAttendance = GET_TUPLE_DATA(FB2ResponseDailyAttendance::is_attendance_index, AttendanceInfo);
	//int32 AttendanceDay = GET_TUPLE_DATA(FB2ResponseDailyAttendance::attendance_day_index, AttendanceInfo);
	//int32 AttendanceType = GET_TUPLE_DATA(FB2ResponseDailyAttendance::attendance_type_index, AttendanceInfo);
	//b2network::B2RewardPtr reward = GET_TUPLE_DATA(FB2ResponseDailyAttendance::reward_index, AttendanceInfo);

	//FAttendanceRewardInfo ServerAttendance;
	//ServerAttendance.bIsAttendance = isAttendance;
	//ServerAttendance.nAttendanceDay = AttendanceDay;
	//ServerAttendance.nAttendance_type = AttendanceType;
	//ServerAttendance.nAttendance_Id = reward->raw_reward->id;
	//ServerAttendance.nAttendance_count = reward->raw_reward->count;
	//ServerAttendance.DailyAttendanceRewardItem = reward;

	//ServerAttendance.bReceiveDone = true;

	//m_AttendanceRewardInfo = ServerAttendance;
}

void FClientDataStore::SetNewUserAttendanceInfo(int32 InAttendanceDay, int64 InNextAttendanceTime)
{
	EventNewUserStatusData.AttendanceDay = InAttendanceDay;
	EventNewUserStatusData.LastAttendanceTime = EventNewUserStatusData.NextAttendanceTime;
	EventNewUserStatusData.NextAttendanceTime = InNextAttendanceTime;
}

bool FClientDataStore::IsReturnUser() const
{
	return m_AttendanceRewardInfo.nAttendance_type == 2;
}

void FClientDataStore::ResponseCheckModeOpen(const FB2ResponseCheckModeOpenPtr& CheckModeOpen)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseCheckModeOpen);
	/*if (CheckModeOpen)
	{
		auto SomeDoc = UB2UIDocHelper::GetDocSome();
		if (SomeDoc)
		{
			TArray<b2network::B2MatchOpenPeriodInfoPtr> CurrentDayOpenTimes;

			bool bIsRaidOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(CheckModeOpen->raid_open_period_infos, CurrentDayOpenTimes);
			bool bIsAssultOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(CheckModeOpen->assault_open_period_infos, CurrentDayOpenTimes);

			SomeDoc->SetIsOpenRaid(bIsRaidOpen);
			SomeDoc->SetIsOpenAssault(bIsAssultOpen);
			SomeDoc->SetGuildBattleState(CheckModeOpen->guild_battle_state);
		}
		UB2UIDocRaid* pDocRaid = UB2UIDocHelper::GetDocRaid();
		if (pDocRaid)
			pDocRaid->SetPeriodInfos(CheckModeOpen->raid_open_period_infos);

		UB2UIDocControl* pDocControl = UB2UIDocHelper::GetDocControl();
		if (pDocControl)
			pDocControl->SetPeriodInfos(CheckModeOpen->assault_open_period_infos);
	}*/
}

void FClientDataStore::RequestGetServerVersion()
{
	data_trader::Retailer::GetInstance().RequestGetServerVersion();
}

void FClientDataStore::ResponseGetServerVersion(const FString& strVersion, const FString& strStage)
{
	FText TextFotmat = FText::FromString(TEXT("{0}.{1}"));
	FText TextServerVersion = FText::Format(TextFotmat, FText::FromString(strVersion), FText::FromString(strStage));

	m_ServerVersionInfo = TextServerVersion.ToString();

	// 	if (UB2UIManager* UIMgrInst = UB2UIManager::GetInstance())
	// 	{
	// 		UIMgrInst->SetMainWidgetServerVersionText(TextServerVersion);
	// 	}
}

void FClientDataStore::TheInitialSoundLocTypeAndCreateNickName()
{
	if (UB2UIManager* pUIManagerInstance = UB2UIManager::GetInstance())
	{
		pUIManagerInstance->OpenUI<UB2UISettingGlobalSoundTheInital>(UIFName::SettingGlobalSoundTheInitial);
	}
}

void FClientDataStore::CheckOpenCreateNickName()
{
	if (HasValidUserNickName() == false)
	{
		UB2UIManager::GetInstance()->OpenUI(UIFName::CreateNickName);
	}
}

int32 FClientDataStore::GetBrevetNodeNeedHonerPoint(int32 BrevetRank, int32 NodeIndex) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBrevetNodeNeedHonerPoint);
	const FBrevetNodeInfo* BrevetNodeData = GetBrevetNodeInfo(BrevetRank, NodeIndex);

	if (!BrevetNodeData)
		return 0;

	return BrevetNodeData->NeedRankPromotionPoint;
}

int32 FClientDataStore::GetBrevetNodeTotalAtk(int32 BrevetRank, int32 NodeIndex) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBrevetNodeTotalAtk);
	const FBrevetNodeInfo* BrevetNodeData = GetBrevetNodeInfo(BrevetRank, NodeIndex);

	if (!BrevetNodeData)
		return 0;

	return BrevetNodeData->TotalAtk;
}

int32 FClientDataStore::GetBrevetNodeTotalDef(int32 BrevetRank, int32 NodeIndex) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBrevetNodeTotalDef);
	const FBrevetNodeInfo* BrevetNodeData = GetBrevetNodeInfo(BrevetRank, NodeIndex);

	if (!BrevetNodeData)
		return 0;

	return BrevetNodeData->TotalDef;
}

int32 FClientDataStore::GetBrevetNodeTotalHp(int32 BrevetRank, int32 NodeIndex) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBrevetNodeTotalHp);
	const FBrevetNodeInfo* BrevetNodeData = GetBrevetNodeInfo(BrevetRank, NodeIndex);

	if (!BrevetNodeData)
		return 0;

	return BrevetNodeData->TotalHp;
}



int32 FClientDataStore::GetBrevetNodeTotalStatus(EItemOption OptionType, int32 BrevetRank, int32 NodeIndex) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBrevetNodeTotalStatus);
	switch (OptionType)
	{
	case EItemOption::EIO_Offense_IncAttack:
		return GetBrevetNodeTotalAtk(BrevetRank, NodeIndex);
		break;
	case EItemOption::EIO_Defense_IncDefense:
		return GetBrevetNodeTotalDef(BrevetRank, NodeIndex);
		break;
	case EItemOption::EIO_Health_IncHealth:
		return GetBrevetNodeTotalHp(BrevetRank, NodeIndex);
		break;
	default:
		return 0;
		break;
	}
}

const FBrevetNodeInfo* FClientDataStore::GetBrevetNodeInfo(int32 BrevetRank, int32 NodeIndex) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBrevetNodeInfo);
	//const FBrevetRankInfo* BrevetRankData = BrevetMasterData.Find(BrevetRank);

	//if (BrevetRankData)
	//	return BrevetRankData->BrevetNodeInfo.Find(NodeIndex);

	return NULL;
}

const int32 FClientDataStore::GetBrevetMaxRank() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMaxRank);

	return BrevetMasterData.Num();
}

int32 FClientDataStore::GetBrevetNodeCount(int32 BrevetRank) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBrevetNodeCount);
	if (BrevetRank == 0)
		BrevetRank = 1;

	const FBrevetRankInfo* BrevetRankData = BrevetMasterData.Find(BrevetRank);

	if (BrevetRankData)
		return BrevetRankData->BrevetNodeInfo.Num();

	return 0;
}

int32 FClientDataStore::GetBrevetRankByOpenLevel(int32 OpenLevelMin, int32 OpenLevelMax) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBrevetRankByOpenLevel);
	int32 nBrevetRank = 0;

	for (const TPair<int32, FBrevetRankInfo>& ElemBMD : BrevetMasterData)
	{
		for (const TPair<int32, FBrevetNodeInfo>& ElemBNI : ElemBMD.Value.BrevetNodeInfo)
		{
			if (ElemBNI.Value.OpenLevel >= OpenLevelMin && ElemBNI.Value.OpenLevel <= OpenLevelMax)
			{
				if (ElemBMD.Value.BrevetRank >= nBrevetRank)
					nBrevetRank = ElemBMD.Value.BrevetRank;
			}

			// 眉农且备埃 逞菌栏搁 八荤沁带芭 府畔
			if (ElemBNI.Value.OpenLevel > OpenLevelMax)
				return nBrevetRank;
		}
	}

	return nBrevetRank;
}

int32 FClientDataStore::GetMaxBrevetRank()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMaxBrevetRank);
	if (BrevetMasterData.Num() == 0)
		return 0;

	TArray<int32> RankList;
	if (BrevetMasterData.GetKeys(RankList) == 0)
		return 0;

	RankList.Sort([](const int32 A, const int32 B) { return A > B; });
	return RankList[0];
}

int32 FClientDataStore::GetRewardID(ERewardType RewardType) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRewardID);
	TArray<int32> RewardIDList;
	if (RewardInfoData.GetKeys(RewardIDList) > 0)
	{
		for (int32 RewardID : RewardIDList)
		{
			auto* RewardInfo = RewardInfoData.Find(RewardID);
			if (RewardInfo && RewardInfo->Type == RewardType)
			{
				return RewardID;
			}
		}
	}

	return 0;
}

ERewardType FClientDataStore::GetRewardType(int32 RewardId) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRewardID_int);
	auto* RewardInfo = RewardInfoData.Find(RewardId);

	if (RewardInfo == nullptr)
		return ERewardType::None;

	return RewardInfo->Type;
}

int32 FClientDataStore::GetRewardItemId(int32 RewardId) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRewardItemId);
	auto* RewardInfo = RewardInfoData.Find(RewardId);

	if (RewardInfo == nullptr)
		return 0;

	return FItemRefIDHelper::GetRewardItemId(RewardInfo->Type, RewardInfo->RewardId);
}


int32 FClientDataStore::GetRewardPushType(int32 RewardId) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRewardPushType);
	auto* RewardInfo = RewardInfoData.Find(RewardId);

	if (RewardInfo == nullptr)
		return -1;
	return RewardInfo->RewardPushType;
}

int32 FClientDataStore::GetRewardIDKeyValue(int32 iItemTemplateId) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRewardIDKeyValue);
	auto* RewardInfo = StaticFindRewardInfo();
	if (!RewardInfo)
	{
		return -1;
	}

	TArray<int32> RewardIDList;
	if (RewardInfoData.GetKeys(RewardIDList) <= 0)
	{
		return -1;
	}

	for (int32 RewardID : RewardIDList)
	{
		if (FClientDataStore::GetRewardItemId(RewardID) == iItemTemplateId)
		{
			return RewardID;
		}
	}

	return -1;
}

FText FClientDataStore::GetRewardName(int32 RewardId) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRewardName);
	auto* RewardInfo = StaticFindRewardInfo();
	if (RewardInfo)
	{
		return RewardInfo->GetNameText(RewardId);
	}

	return FText::FromString(TEXT("Not found"));
}

FClassCollectionItemData* FClientDataStore::GetClassCollectionItems(EPCClass TargetClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetClassCollectionItems);
	return ItemMasterData.CharacterItems.Find(TargetClass);
}

bool FClientDataStore::GetSetItemUniqueKeys(EPCClass TargetClass, TArray<int32>& UniqueKeys)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSetItemUniqueKeys);
	FClassCollectionItemData* ClassItemData = GetClassCollectionItems(TargetClass);
	if (ClassItemData)
	{
		ClassItemData->SetItemMasterInfos.GetKeys(UniqueKeys);
	}

	return UniqueKeys.Num() > 0 ? true : false;
}

FMD_SetInfo* FClientDataStore::GetSetItemInfo(EPCClass TargetClass, int32 SetUniqueKey)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSetItemInfo);
	FClassCollectionItemData* ClassItemData = GetClassCollectionItems(TargetClass);
	if (ClassItemData)
	{
		return ClassItemData->SetItemMasterInfos.Find(SetUniqueKey);
	}

	return nullptr;
}

int32 FClientDataStore::GetTotalItemCount(EPCClass TargetClass, EItemClass ItemClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetTotalItemCount);
	FClassCollectionItemData* ClassItemData = GetClassCollectionItems(TargetClass);
	if (ClassItemData)
	{
		if (ItemClass == EItemClass::EIC_SetItem)
		{
			int32 count = 0;

			for (auto Item : ClassItemData->SetItemMasterInfos)
			{
				// 1 ~ 9 : 老馆 厘厚, 10 ~ : 内胶片 栏肺 沥狼
				// b2network::B2GroupPrefix::COLD (Index:10) 何磐 内胶片 酒捞袍捞扁 锭巩俊 荐樊俊 器窃矫虐瘤 臼绰促.
				int32 GroupID = Item.Value.SetGroupID;
				if (GroupID != b2network::B2GroupPrefix::GROWTH
					&& GroupID != b2network::B2GroupPrefix::NON_EQUIP
					&& GroupID != b2network::B2GroupPrefix::ETERNAL
					&& GroupID != b2network::B2GroupPrefix::DUEL
					&& GroupID < b2network::B2GroupPrefix::COLD)
				{
					count++;
				}
			}

			return count;
		}

		else if (ClassItemData->ItemMasterInfos.Find(ItemClass))
		{
			return ClassItemData->ItemMasterInfos[ItemClass].Num();
		}
	}

	return 0;
}

int32 FClientDataStore::GetCollectionItemCount(EPCClass TargetClass, EItemClass ItemClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetCollectionItemCount);
	FClassCollectionItemData* ClassItemData = GetClassCollectionItems(TargetClass);
	if (ClassItemData)
	{
		return ClassItemData->GetCollectionItemCount(ItemClass);
	}

	return 0;
}

int32 FClientDataStore::GetEnhancedCollectionItemCount(EPCClass TargetClass, EItemClass ItemClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetNewCollectionItemCount);
	FClassCollectionItemData* ClassItemData = GetClassCollectionItems(TargetClass);
	if (ClassItemData)
	{
		return ClassItemData->GetEnhancedCollectionItemCount(ItemClass);
	}
	return 0;
}

bool FClientDataStore::HasEnhancedCollectionItem(EPCClass TargetClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_HasNewCollectionItem);
	const int32 BeginIdx = static_cast<int32>(EItemClass::EIC_Weapon);
	const int32 EndIdx = static_cast<int32>(EItemClass::EIC_Ring);

	for (int32 Idx = BeginIdx; Idx <= EndIdx; Idx++)
	{
		if (GetEnhancedCollectionItemCount(TargetClass, static_cast<EItemClass>(Idx)) > 0)
			return true;
	}

	if (GetEnhancedCollectionItemCount(TargetClass, EItemClass::EIC_SetItem))
		return true;

	return false;
}

bool FClientDataStore::GetClassItemTemplateIDs(TArray<int32>& OutTemplateIDs, EPCClass TargetClass, EItemClass TargetItem, int32 OptionalGroupID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetClassItemTemplateIDs);
	FClassCollectionItemData* ClassItemData = GetClassCollectionItems(TargetClass);
	if (ClassItemData)
	{
		if (TargetItem == EItemClass::EIC_SetItem)
		{
			TArray<int32> SetUniqueKeys;
			ClassItemData->SetItemMasterInfos.GetKeys(SetUniqueKeys);

			for (int32 SetUniqueKey : SetUniqueKeys)
			{
				int32 SetGroupID = GetSetGroupIDFromUniqueKey(SetUniqueKey);
				if (OptionalGroupID == 0 || SetGroupID == OptionalGroupID)
				{
					OutTemplateIDs.Add(SetUniqueKey);
				}
			}
		}

		else
		{
			TArray<FMD_ItemSingleElem>* ItemElements = ClassItemData->ItemMasterInfos.Find(TargetItem);
			if (ItemElements != nullptr)
			{
				for (auto& SingleItem : *ItemElements)
				{
					if (OptionalGroupID == 0 || OptionalGroupID == SingleItem.SetGroupID)
						OutTemplateIDs.Add(SingleItem.ItemTemplateID);
				}
			}
		}
	}

	return OutTemplateIDs.Num() > 0 ? true : false;
}

const FMD_ItemSingleElem* FClientDataStore::GetItemMasterDetailInfo(int32 ItemTemplateID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemMasterDetailInfo);
	EPCClass TargetClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(ItemTemplateID); // 搬惫 捞犯霸 RefID 痹距阑 嘎眠瘤 臼栏搁 救登霸 登绰焙 ぬ.ぬ
	EItemClass TargetItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(ItemTemplateID);
	if (TargetClass == EPCClass::EPC_End || IsEssenceItemClass(TargetItemClass)) // 沥荐 酒捞袍捞 PCClass 喊肺 唱吹绢瘤变 窍瘤父 NonEquipItems 栏肺 盒幅登绢辑.. 球哎府瘤父 咯瓢
	{
		return ItemMasterData.NonEquipItems.Find(ItemTemplateID);
	}
	else if (IsAnvilItemClass(TargetItemClass))
	{
		return ItemMasterData.NonEquipItems.Find(ItemTemplateID);
	}
	else
	{
		FClassCollectionItemData* ClassItemData = GetClassCollectionItems(TargetClass);

		if (ClassItemData)
			return ClassItemData->GetSingleItemInfo(ItemTemplateID);
	}

	return nullptr;
}

const FMD_ItemSetOptionElem* FClientDataStore::GetItemSetOptionInfo(int32 SetOptionID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemSetOptionInfo);
	return ItemMasterData.SetItemOptions.Find(SetOptionID);
}

const FCollectionItemData* FClientDataStore::GetCollectionItem(EItemClass ItemClass, int32 ID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetCollectionItem);
	EPCClass TargetClass;
	if (ItemClass == EItemClass::EIC_SetItem)
		TargetClass = GetSetCharacterClassFromUniqueKey(ID);
	else
		TargetClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(ID);

	FClassCollectionItemData* ClassItemData = GetClassCollectionItems(TargetClass);
	if (ClassItemData)
		return ClassItemData->GetCollectionItemInfo(ItemClass, ID);

	return nullptr;
}

EPCClass FClientDataStore::GetGeneralLotteryCharClass(int32 LotteryId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetGeneralLotteryCharClass);
	auto* LotteryData = GetGeneralLotteryInfo(LotteryId);
	if (LotteryData)
	{
		return LotteryData->CharClass;
	}

	return EPCClass::EPC_End;
}

FB2LotteryTicketInfo* FClientDataStore::GetGeneralLotteryInfo(int32 LotteryId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetGeneralLotteryInfo);
	auto* LotteryData = MD_GeneralLottery.Find(LotteryId);
	if (LotteryData)
	{
		return LotteryData;
	}

	return nullptr;
}

FB2LotteryTicketInfo* FClientDataStore::GetSelelctiveLotteryInfo(int32 LotteryId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSelelctiveLotteryInfo);
	auto* LotteryData = MD_SelectiveLottery.Find(LotteryId);
	if (LotteryData)
	{
		return LotteryData;
	}

	return nullptr;
}

FB2LotteryTicketInfo* FClientDataStore::GetFixedLotteryInfo(int32 LotteryId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetFixedLotteryInfo);
	auto* LotteryData = MD_FixedGradeLottery.Find(LotteryId);
	if (LotteryData)
	{
		return LotteryData;
	}

	return nullptr;
}

FB2LotteryTicketInfo* FClientDataStore::GetPrefixSelelctiveLotteryInfo(int32 LotteryId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(GetPrefixSelelctiveLotteryInfo);
	auto* LotteryData = MD_PrefixSelectiveLottery.Find(LotteryId);
	if (LotteryData)
	{
		return LotteryData;
	}

	return nullptr;
}

int32 FClientDataStore::GetGeneralLotteryItemTemplateId(int32 iLotteryId)
{
	return FItemRefIDHelper::GetGeneralLotteryRefID(iLotteryId);
}

int32 FClientDataStore::GetSelectiveLotteryItemTemplateId(int32 iLotteryId)
{
	return FItemRefIDHelper::GetSelectiveLotteryRefID(iLotteryId);
}

int32 FClientDataStore::GetFixedGradeLotteryItemTemplateId(int32 iLotteryId)
{
	return FItemRefIDHelper::GetFixedGradeLotteryRefID(iLotteryId);
}

int32 FClientDataStore::GetSelectiveEtherLotteryItemTemplateId(int32 iLotteryId)
{
	return FItemRefIDHelper::GetSelectiveEtherLotteryRefID(iLotteryId);
}

int32 FClientDataStore::GetPrefixSelectiveLotteryItemTemplateId(int32 iLotteryId)
{
	return FItemRefIDHelper::GetPrefixSelectiveLotteryRefID(iLotteryId);
}

int32 FClientDataStore::GetAssaultMvpPointToOpenReward()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetAssaultMvpPointToOpenReward);
	if (ConfigData)
		return ConfigData->assault_mvp_point_to_open_reward;

	return 0;
}

void FClientDataStore::GetDuelModeSettleSchedule(int32 in_mode_type, int32 in_ranking_settle_state, int32& out_day, int32& out_end_hour, int32& out_end_min)
{
	for (b2network::B2mdDuelModeSettleSchedulePtr ScheduleInfo : MD_DuelModeSettleSchedules)
	{
		if (ScheduleInfo->mode_type == in_mode_type && ScheduleInfo->ranking_settle_state == in_ranking_settle_state)
		{
			out_day = ScheduleInfo->day;
			out_end_hour = ScheduleInfo->end_hour;
			out_end_min = ScheduleInfo->end_min;
			return;
		}
	}
}

void FClientDataStore::SetOtherUserInfo(b2network::B2FindAccountInfoPtr InPlayerInfo)
{
	OtherUserInfo = InPlayerInfo;
}

FB2ModPlayerInfo& FClientDataStore::GetOtherUserInfo()
{
	return OtherUserInfo;
}

void FClientDataStore::OnReceiveCollectionReward(EPCClass TargetClass, int32 ItemTemplateID, b2network::B2RewardPtr RewardPtr)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_OnReceiveCollectionReward);
	if (ItemTemplateID && RewardPtr)
	{
		FClassCollectionItemData* ClassItemData = GetClassCollectionItems(TargetClass);
		if (ClassItemData)
		{
			FCollectionItemData* CollectionItemInfo = ClassItemData->GetCollectionItemInfo(ItemTemplateID);
			if (CollectionItemInfo)
			{
				// getcollectionitem 罐栏搁 绢楼乔 rewarded肺 登绢 乐瘤父 犬牢 瞒盔俊辑
				CollectionItemInfo->CollectionState = EItemCollectionState::Rewarded;
				CollectionItemInfo->bNewItem = false;

				EItemClass ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(ItemTemplateID);
				CollectionRewardActionLog(TargetClass, ItemClass);
			}
		}
	}
}

void FClientDataStore::OnReceiveSetCollectionReward(EPCClass TargetClass, int32 SetUniqueKey, b2network::B2RewardPtr RewardPtr)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_OnReceiveSetCollectionReward);
	if (SetUniqueKey && RewardPtr)
	{
		FClassCollectionItemData* ClassItemData = GetClassCollectionItems(TargetClass);
		if (ClassItemData)
		{
			FCollectionItemData* CollectionSetInfo = ClassItemData->GetCollectionItemInfo(EItemClass::EIC_SetItem, SetUniqueKey);
			if (CollectionSetInfo)
			{
				// getcollectionitem 罐栏搁 绢楼乔 rewarded肺 登绢 乐瘤父 犬牢 瞒盔俊辑
				CollectionSetInfo->CollectionState = EItemCollectionState::Rewarded;
				CollectionSetInfo->bNewItem = false;

				CollectionRewardActionLog(TargetClass, EItemClass::EIC_SetItem);
			}
		}
	}
}

void FClientDataStore::CollectionRewardActionLog(EPCClass InPCClass, EItemClass InItemClass)
{
	//////////////////////////////////////////////////////////////////
	//咀记 瘤钎 肺弊 (档皑 崔己)
	FText TabName;
	switch (InItemClass)
	{
	case EItemClass::EIC_Weapon:
		TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_WeaponTab")));
		break;
	case EItemClass::EIC_Helmet:
		TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_HelmetTab")));
		break;
	case EItemClass::EIC_BodyArmor:
		TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_BodyArmorTab")));
		break;
	case EItemClass::EIC_Glove:
		TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_GloveTab")));
		break;
	case EItemClass::EIC_Shoes:
		TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_ShoesTab")));
		break;
	case EItemClass::EIC_Belt:
		TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_BeltTab")));
		break;
	case EItemClass::EIC_Necklace:
		TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_NecklaceTab")));
		break;
	case EItemClass::EIC_Ring:
		TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_RingTab")));
		break;
	case EItemClass::EIC_SetItem:
		TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_SetTab")));
		break;
	default:
		TabName = FText::FromString(TEXT("COLLECTION_PARTS_NONE"));
		break;
	}

	B2ActionLogSender Sender;
	Sender.setCategory(FString("COLLECTION"))
		.setAction(FString("GET_SET"))
		.setLabel(B2ActionLogType<EPCClass>::GetName(InPCClass))
		.setActionAttr1(TabName.ToString().ToUpper())
		.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//////////////////////////////////////////////////////////////////
}

const FMonsterInfo* FClientDataStore::GetMonsterInfo(const ENPCClass NpcClass, const ENPCClassVariation NpcGrade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMonsterInfo);
	int32 NPCClassSvrInt32 = static_cast<int32>(NpcClass);
	int32 NPCClassGradeSvrInt32 = static_cast<int32>(NpcGrade);

	if (MD_MonterListData.Contains(NPCClassGradeSvrInt32) == false)
		return nullptr;

	const auto& MonsterMap = MD_MonterListData[NPCClassGradeSvrInt32];

	return MonsterMap.Find(NPCClassSvrInt32);
}

#include "B2NPCSensitiveInfo.h"
void FClientDataStore::GetMonsterGradeFactor(ENPCClass InClass, UB2NPCSingleClassInfo* InClassInfo, float& OutHPScale, float& OutAttackDefenseLevelScale, float& OutMaxArmorOverride)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMonsterGradeFactor);
	bool bUsingOnlyClientDataForBalancingTest = false;
	GConfig->GetBool(TEXT("/Script/BladeII"), TEXT("bUsingOnlyClientDataForBalancingTest"), bUsingOnlyClientDataForBalancingTest, GGameIni);

	OutHPScale = 0.f;
	OutAttackDefenseLevelScale = 0.f;
	OutMaxArmorOverride = 0.f;

	auto* MobInfo = GetMonsterInfo(InClass, InClassInfo->ClassVariation);
	if (MobInfo)
	{
		OutAttackDefenseLevelScale = MobInfo->AttackDefenseLevelScale;
		OutHPScale = MobInfo->HPScale;
		OutMaxArmorOverride = MobInfo->MaxArmorOverride;
	}
	else
		bUsingOnlyClientDataForBalancingTest = true; //辑滚肺何磐 单捞磐甫 罐瘤 给茄 版快.

	if (ShouldShowGDCVersionContent() || bUsingOnlyClientDataForBalancingTest)
	{
		// 荤角惑 NPCClassInfo 狼 老何牢 SensitiveInfo. 傈券 档吝浚 绝阑 荐 乐栏聪 check 甫 吧瘤 富 巴.
		UB2NPCSensitiveInfo* NPCSensitiveInfo = StaticFindNPCSensitiveInfoTable();
		// 傈捧 瓷仿摹甸		
		FSingleNPCSensitiveData* ThisSensitiveInfo = NPCSensitiveInfo ? NPCSensitiveInfo->GetInfoData(InClass, InClassInfo) : NULL;

		if (ThisSensitiveInfo) // 霖厚登搁 SensitiveInfo 俊辑 啊廉坷档废 官槽促. 酒贰 促弗 割割 荐摹甸档
		{
			OutAttackDefenseLevelScale = ThisSensitiveInfo->AttackDefenseLevelScale;
			OutHPScale = ThisSensitiveInfo->HPScale;
			OutMaxArmorOverride = ThisSensitiveInfo->MaxArmorOverride;
		}
	}
}

void FClientDataStore::PreSetMasterData()
{

}

void FClientDataStore::PostSetMasterData()
{
	// 酒捞袍 付胶磐 单捞磐 罐扁 傈俊 AccountInfo 烹秦 馒侩 酒捞袍 单捞磐甫 罐疽阑 巴捞骨肺 捞 矫痢俊辑 付胶磐 单捞磐甫 扁馆栏肺 单捞磐甫 盎脚窃.
	LocalCharacterData.UserAllItems.UpdateItemMasterDataOnMD();

	// 扁鸥 捞繁历繁 SetAccount 矫 鞘夸茄 肺厚 诀单捞飘甸 咯扁俊
	LobbyAnyOtherUpdateOnSetAccountClass<>::GetInstance().Signal();

	// 朝俺 付胶磐 单捞磐 罐扁 傈俊 AccountInfo 烹秦 朝俺 单捞磐甫 罐疽阑 巴捞骨肺 捞 矫痢俊辑 付胶磐 单捞磐甫 扁馆栏肺 扁鸥 朝俺 单捞磐甫 盎脚窃.
	UpdateWingCombatStatsOnMDClass<>::GetInstance().Signal();
	// 埃趣 AccountInfo 俊 器窃等 朝俺 单捞磐啊 肯傈摹 臼篮 芭 鞍酒 (可记 穿遏?) 巩力啊 登搁 捞 矫痢俊辑 老褒 夸没阑.. 
	//BeginReqForLobbyInitWingDataClass<>::GetInstance().Signal();

	// 林夸 傈捧仿 荐摹 UI 诀单捞飘. 付胶磐 单捞磐 罐篮 贸澜俊 诀单捞飘 秦 临 鞘夸啊 乐促.
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
		UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(IntToPCClass(PCI), false);

	LoadLocalData_ResourceBound(); // InfoAsset 郴瘤绰 MasterData 甫 鞘夸肺 窍绰 Local (config) Data 肺爹
}

void FClientDataStore::SetMasterDataImpl_ConfigData(const FB2MasterDatas& InMasterDatas)
{
	//ConfigData = GET_TUPLE_DATA(FB2ResponseMasterDatas::config_data_index, InMasterDatas);
}

void FClientDataStore::SetMasterDataImpl_Skill(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_Skill);
	//SkillMasterData.Empty();
	//auto mdSkillInfos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_skill_infos_index, InMasterDatas);
	//auto mdSkillOptionInfos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_skill_options_index, InMasterDatas);
	//auto mdSkillLevelUpCosts = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_skill_levelups_index, InMasterDatas);

	//auto* ClassInfoBox = StaticFindPCClassInfoBox();
	//auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;

	//for (auto mdSkillInfo : mdSkillInfos)
	//{
	//	auto* SkillClientData = AllSkillInfo->GetSingleInfoOfID(mdSkillInfo->skill_id);
	//	bool bIsActive = SkillClientData ? SkillClientData->PassiveType == EPassiveType::EPassiveType_None : false;
	//	FSkillMasterData Data(mdSkillInfo->skill_id, mdSkillInfo->learn_level, mdSkillInfo->max_skill_level, mdSkillInfo->level_up_cost_factor, SvrToCliPCClassType(mdSkillInfo->character_type), bIsActive);
	//	for (auto mdSkillOptionInfo : mdSkillOptionInfos)
	//	{
	//		if (mdSkillOptionInfo->skill_id == Data.SkillId)
	//		{
	//			Data.LevelOptionData.Add(mdSkillOptionInfo->skill_level, FSkillOptionData(mdSkillOptionInfo->option_id, mdSkillOptionInfo->option_value, mdSkillOptionInfo->power));
	//		}
	//	}

	//	for (auto mdSkillLevelUpCost : mdSkillLevelUpCosts)
	//	{
	//		if (mdSkillLevelUpCost->skill_id == Data.SkillId)
	//		{
	//			Data.LevelUpPointData.Add(mdSkillLevelUpCost->skill_level, mdSkillLevelUpCost->req_skill_point);
	//		}
	//	}
	//	SkillMasterData.Add(Data.SkillId, Data);
	//}
}

void FClientDataStore::SetMasterDataImpl_ItemLevelupExpFactorAndOptionAddValue(const FB2MasterDatas& InMasterDatas)
{
	//// 酒捞袍 碍拳 橇府轰俊辑 荤侩窍绰 单捞磐甸
	//BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ItemLevelupExpFactorAndOptionAddValue);
	//ItemLevelupFactorMap.UpdateByServerSync(
	//	GET_TUPLE_DATA(FB2ResponseMasterDatas::md_item_levelup_exp_factors_index, InMasterDatas),
	//	GET_TUPLE_DATA(FB2ResponseMasterDatas::md_item_option_add_values_index, InMasterDatas));
}

void FClientDataStore::SetMasterDataImpl_ItemLevelupCost(const FB2MasterDatas& InMasterDatas)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ItemLevelupCost);
	//ItemLevelupCostMap.Empty();
	//// 酒捞袍 碍拳 厚侩 橇府轰俊 荤侩窍绰 单捞磐. 捞扒 老馆 碍拳啊 酒聪扼 磊悼 碍拳俊辑父.
	//const TArray<b2network::B2mdItemLevelupCostPtr>& AllItemLevelupPriceData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_item_levelup_costs_index, InMasterDatas);
	//for (const b2network::B2mdItemLevelupCostPtr& ThisPricePerLevel : AllItemLevelupPriceData)
	//{
	//	// enhane level 虐俊 蝶弗 阿 element 喊肺 Inventory type 俊 蝶弗 备盒捞 肚茄 乐栏骨肺 扁粮俊 眠啊等 巴何磐 茫酒航.
	//	FItemLevelupCostPerLevel* FoundCostPerLevel = ItemLevelupCostMap.Find(ThisPricePerLevel->level);
	//	if (FoundCostPerLevel)
	//	{ // 扁粮俊 眠啊等 巴捞扼绊 秦档 捞锅 inventory type 俊 措秦急 单捞磐啊 绝阑 巴栏肺 扁措.
	//		checkSlow(!FoundCostPerLevel->HasDataForInvenType(SvrToCliItemInvenType(ThisPricePerLevel->inventory_type)));
	//		FoundCostPerLevel->UpdateByServerSync(ThisPricePerLevel);
	//	}
	//	else
	//	{ // 秦寸 碍拳 饭骇俊 措秦辑绰 贸澜 甸绢柯 芭.
	//		FItemLevelupCostPerLevel NewCostElem;
	//		NewCostElem.UpdateByServerSync(ThisPricePerLevel);
	//		ItemLevelupCostMap.Add(ThisPricePerLevel->level, NewCostElem);
	//	}
	//}
}

void FClientDataStore::SetMasterDataImpl_StageInfo(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_StageInfo);
	BladeIIGameImpl::GetStageDataStore().SetMasterData(InMasterDatas);
}

void FClientDataStore::SetMasterDataImpl_ResurrectionInfo(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ResurrectionInfo);
	//const TArray<b2network::B2mdResurrectionInfoPtr>& AllResurrectMasterInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_resurrection_infos_index, InMasterDatas);
	//for (const b2network::B2mdResurrectionInfoPtr& ThisInfo : AllResurrectMasterInfo)
	//{
	//	if (ThisInfo)
	//	{
	//		FResurrectMasterDataElement NewElem;
	//		NewElem.UpdateByServerSync(ThisInfo);

	//		EResurrectGameModeType ModeType = (EResurrectGameModeType)ThisInfo->mode_type;
	//		if (ResurrectMasterData.Contains(ModeType) == false)
	//		{
	//			ResurrectMasterData.Add(ModeType, FModeResurrectMasterDataElement(ThisInfo->count, NewElem));
	//		}
	//		else
	//		{
	//			ResurrectMasterData[ModeType].ResurrectElementList.Add(ThisInfo->count, NewElem);
	//		}
	//	}
	//}

}

void FClientDataStore::SetMasterDataImpl_TeamBattleFormationInfo(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_TeamBattleFormationInfo);
	//const TArray<b2network::B2mdTeamBattleFormationInfoPtr>& AllTeamBattleFormationInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_formation_infos_index, InMasterDatas);
	//for (auto& FormationInfo : AllTeamBattleFormationInfo)
	//{
	//	FTeamMatchFormationLevelInfo LevelInfo;

	//	LevelInfo.EnhanceGoldCost = FormationInfo->enhance_gold_cost;

	//	// TODO formation buff
	//	for (auto FormationBuff : FormationInfo->buffs)
	//	{
	//		FTeamMatchFormationBuffInfo BuffInfo;
	//		BuffInfo.Id = FormationBuff->id;
	//		BuffInfo.BuffType1 = FormationBuff->buff_type1;
	//		BuffInfo.BuffValue1 = FormationBuff->buff_value1;
	//		BuffInfo.BuffType2 = FormationBuff->buff_type2;
	//		BuffInfo.BuffValue2 = FormationBuff->buff_value2;

	//		if (!LevelInfo.BuffInfo.Contains(BuffInfo.Id))
	//		{
	//			LevelInfo.BuffInfo.Add(BuffInfo.Id, BuffInfo);
	//		}
	//	}

	//	if (!TeamMatchFormationMasterData.Contains(FormationInfo->formation_type))
	//	{
	//		TeamMatchFormationMasterData.Add(FormationInfo->formation_type, FTeamMatchFormationTypeInfo(FormationInfo->formation_type));
	//	}

	//	FTeamMatchFormationTypeInfo* TypeInfo = TeamMatchFormationMasterData.Find(FormationInfo->formation_type);
	//	BII_CHECK(TypeInfo);
	//	if (TypeInfo)
	//	{
	//		TypeInfo->MaxLevel = FormationInfo->formation_level;
	//		TypeInfo->LevelInfo.Add(FormationInfo->formation_level, LevelInfo);
	//	}
	//}
}

void FClientDataStore::SetMasterDataImpl_RankPromotionInfo(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_RankPromotionInfo);
	//// Brevet Master Data
	//const TArray<b2network::B2mdRankInfoPtr>& AllRankInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_rank_infos_index, InMasterDatas);

	//int32 BrevetNodeInfoStatusTotalAtk = 0;
	//int32 BrevetNodeInfoStatusTotalDef = 0;
	//int32 BrevetNodeInfoStatusTotalHp = 0;

	//for (auto& RankInfo : AllRankInfo)
	//{
	//	FBrevetNodeInfo BrevetNodeInfo;

	//	BrevetNodeInfo.OpenLevel = RankInfo->open_level;
	//	BrevetNodeInfo.StatusType = RankInfo->attribute_change;
	//	BrevetNodeInfo.StatusValue = RankInfo->change_value;
	//	BrevetNodeInfo.NeedRankPromotionPoint = RankInfo->need_rank_promotion_point;
	//	BrevetNodeInfo.NeedActiveGold = RankInfo->need_activate_gold;
	//	BrevetNodeInfo.Rate = RankInfo->rate;


	//	// No Include Current Node
	//	switch (EBrevetStatusType(BrevetNodeInfo.StatusType))
	//	{
	//	case EBrevetStatusType::EBST_Atk:
	//		BrevetNodeInfoStatusTotalAtk += BrevetNodeInfo.StatusValue;
	//		break;
	//	case EBrevetStatusType::EBST_Def:
	//		BrevetNodeInfoStatusTotalDef += BrevetNodeInfo.StatusValue;
	//		break;
	//	case EBrevetStatusType::EBST_Hp:
	//		BrevetNodeInfoStatusTotalHp += BrevetNodeInfo.StatusValue;
	//		break;
	//	default:
	//		break;
	//	}

	//	BrevetNodeInfo.TotalAtk = BrevetNodeInfoStatusTotalAtk;
	//	BrevetNodeInfo.TotalDef = BrevetNodeInfoStatusTotalDef;
	//	BrevetNodeInfo.TotalHp = BrevetNodeInfoStatusTotalHp;

	//	if (!BrevetMasterData.Contains(RankInfo->rank))
	//	{
	//		BrevetMasterData.Add(RankInfo->rank, FBrevetRankInfo(RankInfo->rank));
	//	}

	//	FBrevetRankInfo* BrevetRankInfo = BrevetMasterData.Find(RankInfo->rank);
	//	BII_CHECK(BrevetRankInfo);
	//	if (BrevetRankInfo)
	//	{
	//		if (BrevetRankInfo->BrevetRank == 0)
	//			BrevetRankInfo->BrevetRank = RankInfo->rank;

	//		BrevetRankInfo->BrevetNodeInfo.Add(RankInfo->node_index, BrevetNodeInfo);
	//	}
	//}
}

void FClientDataStore::SetMasterDataImpl_DailyMatchReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_DailyMatchReward);
	//const TArray<b2network::B2mdDailyMatchRewardPtr>& AllModDaliyData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_daily_match_rewards_index, InMasterDatas);
	//ModDailyData.Empty();

	//for (auto& DaliyData : AllModDaliyData)
	//{
	//	FModDailyReward DailyRewardData;

	//	DailyRewardData.Match_Type = DaliyData->match_type;
	//	DailyRewardData.Require_Count = DaliyData->require_count;
	//	DailyRewardData.Reward_BladePoint = DaliyData->reward_bladepoint;
	//	DailyRewardData.Reward_Gold = DaliyData->reward_gold;
	//	DailyRewardData.Reward_Gem = DaliyData->reward_gem;
	//	DailyRewardData.Reward_ItemId = DaliyData->reward_item_template_id;

	//	ModDailyData.Add(DailyRewardData);
	//}
}

void FClientDataStore::SetMasterDataImpl_PVPMatchRankingReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_PVPMatchRankingReward);
	//const TArray<b2network::B2mdMatchRankingRewardPtr>& PvPRankingRewardData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_pvp_ranking_rewards_index, InMasterDatas);
	//PvPWeeklyRewardData.Empty();

	//for (auto& WeeklyData : PvPRankingRewardData)
	//{
	//	FModWeeklyReward WeeklyRewardData;

	//	WeeklyRewardData.Grade = WeeklyData->grade;
	//	WeeklyRewardData.Ranking_Max = WeeklyData->ranking_max;
	//	WeeklyRewardData.Ranking_Min = WeeklyData->ranking_min;
	//	WeeklyRewardData.rewardItems = WeeklyData->rewards;
	//	//WeeklyRewardData.Reward_Gem = WeeklyData->reward_gem;

	//	PvPWeeklyRewardData.Add(WeeklyRewardData);
	//}
}

void FClientDataStore::SetMasterDataImpl_TeamMatchRankingReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_TeamMatchRankingReward);
	//const TArray<b2network::B2mdMatchRankingRewardPtr>& TeamMatchRewardData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_team_ranking_rewards_index, InMasterDatas);
	//TeamMatchWeeklyRewardData.Empty();

	//for (auto& WeeklyData : TeamMatchRewardData)
	//{
	//	FModWeeklyReward WeeklyRewardData;

	//	WeeklyRewardData.Grade = WeeklyData->grade;
	//	WeeklyRewardData.Ranking_Max = WeeklyData->ranking_max;
	//	WeeklyRewardData.Ranking_Min = WeeklyData->ranking_min;
	//	WeeklyRewardData.rewardItems = WeeklyData->rewards;
	//	//WeeklyRewardData.Reward_Gem = WeeklyData->reward_gem;

	//	TeamMatchWeeklyRewardData.Add(WeeklyRewardData);
	//}
}

void FClientDataStore::SetMasterDataImpl_ControlMatchRankingReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ControlMatchRankingReward);
	//const TArray<b2network::B2mdMatchRankingRewardPtr>& ControlMatchRewardData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_assault_ranking_rewards_index, InMasterDatas);
	//ControlWeeklyRewardData.Empty();

	//for (auto& WeeklyData : ControlMatchRewardData)
	//{
	//	FModWeeklyReward WeeklyRewardData;

	//	WeeklyRewardData.Grade = WeeklyData->grade;
	//	WeeklyRewardData.Ranking_Max = WeeklyData->ranking_max;
	//	WeeklyRewardData.Ranking_Min = WeeklyData->ranking_min;
	//	WeeklyRewardData.rewardItems = WeeklyData->rewards;
	//	//WeeklyRewardData.Reward_Gem = WeeklyData->reward_gem;

	//	ControlWeeklyRewardData.Add(WeeklyRewardData);
	//}
}

void FClientDataStore::SetMasterDataImpl_CounterDungeon(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_CounterDungeon);
	//const TArray<b2network::B2mdCounterDungeonPtr>& CounterDungeonInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_counter_dungeons_index, InMasterDatas);
	//CounterDungeonData.Empty();

	//for (auto& CDData : CounterDungeonInfo)
	//{
	//	BladeIIGameImpl::GetStageDataStore().AddMDStageGamePlayData(FServerStageID(CDData->dungeon_id), FStageInfoGameplayData(CDData));

	//	FCounterDungeon CounterDungeon;
	//	CounterDungeon.ServerStageId = FServerStageID(CDData->dungeon_id);
	//	CounterDungeon.DungeonType = CDData->dungeon_type;
	//	CounterDungeon.Difficulty = CDData->difficulty;
	//	CounterDungeon.DifficultyLevel = CDData->difficulty_level;
	//	CounterDungeon.DifficultyScale = CDData->difficulty_scale;
	//	CounterDungeon.RecommendCombat = CDData->recommend_power;	// jwyim power
	//	CounterDungeon.RecommendAttack = CDData->recommend_attack;
	//	CounterDungeon.RecommendDefense = CDData->recommend_defense;

	//	for (auto& Ele : CDData->rewards)
	//	{
	//		// TODO fix (min ~ max)
	//		CounterDungeon.RewardId.Add(FRewardItem(Ele->id, Ele->min_count, Ele->max_count));
	//	}

	//	CounterDungeonData.Add(CounterDungeon);
	//}
}

void FClientDataStore::SetMasterDataImpl_CounterDungeonPeriod(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_CounterDungeonPeriod);
	//const TArray<b2network::B2mdCounterDungeonPeriodPtr>& CounterDungeonPeriodInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_counter_dungeon_periods_index, InMasterDatas);
	//CounterDungeonPeriodData.Empty();

	//for (auto& CDPeriodData : CounterDungeonPeriodInfo)
	//{
	//	FCounterDungeonPeriod CounterDungeonPeriod;
	//	CounterDungeonPeriod.DungeonType = CDPeriodData->dungeon_type;
	//	CounterDungeonPeriod.DayOfWeek = CDPeriodData->day_of_week;
	//	CounterDungeonPeriod.BeginHour = CDPeriodData->begin_hour;
	//	CounterDungeonPeriod.BeginMin = CDPeriodData->begin_min;
	//	CounterDungeonPeriod.EndHour = CDPeriodData->end_hour;
	//	CounterDungeonPeriod.EndMin = CDPeriodData->end_min;
	//	// TODO remove characterType
	//	CounterDungeonPeriod.CharacterType = EPCClass::EPC_Gladiator;

	//	if (CounterDungeonPeriodData.Contains(CounterDungeonPeriod.DungeonType))
	//		CounterDungeonPeriodData[CounterDungeonPeriod.DungeonType].Add(CounterDungeonPeriod);
	//	else
	//	{
	//		TArray<FCounterDungeonPeriod> dungeonperiod;
	//		dungeonperiod.Add(CounterDungeonPeriod);
	//		CounterDungeonPeriodData.Add(CounterDungeonPeriod.DungeonType, dungeonperiod);
	//	}
	//}
}

void FClientDataStore::SetMasterDataImpl_CounterDungeonWeekReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_CounterDungeonWeekReward);
	//const TArray<b2network::B2mdCounterDungeonWeekRewardPtr>& CounterDungeonWeekReward = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_counter_dungeon_week_rewards_index, InMasterDatas);
	//CounterDungeonRewardData.Empty();

	//for (auto& CDReward : CounterDungeonWeekReward)
	//{
	//	FCounterDungeunWeekReward WeekData;

	//	WeekData.RequireCount = CDReward->require_count;

	//	for (auto& ItemReward : CDReward->rewards)
	//	{
	//		WeekData.RewardItem.Add(FRewardItem(ItemReward->raw_reward->id, ItemReward->raw_reward->count));
	//	}

	//	CounterDungeonRewardData.Add(WeekData);
	//}
	//// 趣矫葛福聪 焊惑 俺荐啊 利篮巴何磐 沥纺阑 秦凛聪促...
	//CounterDungeonRewardData.Sort([this](const FCounterDungeunWeekReward& count1, const FCounterDungeunWeekReward& count2)->bool {
	//	if (count1.RequireCount > count2.RequireCount) return false; else return true; });
}

void FClientDataStore::SetMasterDataImpl_RewardInfo(const FB2MasterDatas& InMasterDatas)
{
	//BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_RewardInfo);
	//RewardInfoData.Empty();
	//auto RewardData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_reward_infos_index, InMasterDatas);
	//for (auto RewardInfo : RewardData)
	//{
	//	RewardInfoData.Add(RewardInfo->id, FRewardData((ERewardType)RewardInfo->reward_type, RewardInfo->reward_id, RewardInfo->reward_push_type));
	//}
}

void FClientDataStore::SetMasterDataImpl_WingEnhance(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_WingEnhance);
	//WingMD_Enhance.Empty();
	//const TArray<b2network::B2mdWingEnhancePtr>& WingEnhanceInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_wing_enhance_infos_index, InMasterDatas);
	//for (const b2network::B2mdWingEnhancePtr& ThisInfo : WingEnhanceInfo)
	//{
	//	FWingMD_EnhanceSingleElem NewCliMDElem;
	//	NewCliMDElem = ThisInfo;
	//	WingMD_Enhance.Add(NewCliMDElem.GetMyKey(), NewCliMDElem);
	//}
}

void FClientDataStore::SetMasterDataImpl_WingUpgrade(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_WingUpgrade);
	//WingMD_Evolve.Empty();
	//const TArray<b2network::B2mdWingUpgradePtr>& WingUpgradeInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_wing_upgrade_infos_index, InMasterDatas);
	//for (const b2network::B2mdWingUpgradePtr& ThisInfo : WingUpgradeInfo)
	//{
	//	FWingMD_EvolveSingleElem NewCliMDElem;
	//	NewCliMDElem = ThisInfo;
	//	WingMD_Evolve.Add(NewCliMDElem.GetMyKey(), NewCliMDElem);
	//}
}

void FClientDataStore::SetMasterDataImpl_WingOptionValue(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_WingOptionValue);
	//WingMD_PropOption.Empty();
	//const TArray<b2network::B2mdWingOptionValuePtr>& WingPropOptionInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_wing_option_value_infos_index, InMasterDatas);
	//for (const b2network::B2mdWingOptionValuePtr& ThisInfo : WingPropOptionInfo)
	//{
	//	FWingMD_PropOptionSingleElem NewCliMDElem;
	//	NewCliMDElem = ThisInfo;
	//	WingMD_PropOption.Add(NewCliMDElem.GetMyKey(), NewCliMDElem);
	//}
}

void FClientDataStore::SetMasterDataImpl_WingOptionOpen(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_WingOptionOpen);
	//WingMD_PropOptionOpenCondition.Empty();
	//const TArray<b2network::B2mdWingOptionOpenPtr>& WingPropOptionOpenInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_wing_option_open_infos_index, InMasterDatas);
	//for (const b2network::B2mdWingOptionOpenPtr& ThisInfo : WingPropOptionOpenInfo)
	//{
	//	FWingMD_PropOptionOpenSingleElem NewCliMDElem;
	//	NewCliMDElem = ThisInfo;
	//	WingMD_PropOptionOpenCondition.Add(NewCliMDElem.GetMyKey(), NewCliMDElem);
	//}
}

void FClientDataStore::SetMasterDataImpl_WingOptionMaxLevel(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_WingOptionMaxLevel);
	//WingMD_PropOptionMaxLevel.Empty();
	//const TArray<b2network::B2mdWingOptionMaxLevelPtr>& WingPropOptionMaxLevelInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_wing_option_max_level_infos_index, InMasterDatas);
	//for (const b2network::B2mdWingOptionMaxLevelPtr& ThisInfo : WingPropOptionMaxLevelInfo)
	//{
	//	FWingMD_PropOptionMaxLevelSingleElem NewCliMDElem;
	//	NewCliMDElem = ThisInfo;
	//	WingMD_PropOptionMaxLevel.Add(NewCliMDElem.GetMyKey(), NewCliMDElem);
	//}
}

void FClientDataStore::SetMasterDataImpl_HeroTowerFloor(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_HeroTowerFloor);
	//MD_HeroTowerFloor.Empty();
	//const TArray<b2network::B2mdHeroTowerFloorPtr>& HeroTowerFloorInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_hero_tower_floors_index, InMasterDatas);
	//for (const b2network::B2mdHeroTowerFloorPtr& ThisInfo : HeroTowerFloorInfo)
	//{
	//	FMD_HeroTowerFloorSingleElem NewCliMDElem;
	//	NewCliMDElem = ThisInfo;
	//	MD_HeroTowerFloor.Add(NewCliMDElem.nFloor, NewCliMDElem);
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopLotteryPrice(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopLotteryPrice);
	//auto& SummonItemData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_lottery_prices_index, InMasterDatas);
	//MD_Store_SummonItem_ListData.Empty(SummonItemData.Num());
	//for (auto& Elem : SummonItemData)
	//{
	//	uint32 HashKey = FSummonItemSlotData::GetHashKey(int32(ESummonItemTabMenu::GemBox), Elem->is_continuous, Elem->is_again);
	//	MD_Store_SummonItem_ListData.Add(HashKey, FSummonItemSlotData(HashKey, Elem->gem, Elem->calculated_gem));

	//	HashKey = FSummonItemSlotData::GetHashKey(int32(ESummonItemTabMenu::FriendShipBox), Elem->is_continuous, Elem->is_again);
	//	MD_Store_SummonItem_ListData.Add(HashKey, FSummonItemSlotData(HashKey, Elem->social_point, 0));
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopLotteryPossibleGradeAndSpecialGrade(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopLotteryPossibleGradeAndSpecialGrade);
	//auto& SummonItemSpecialGradeRange = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_lottery_special_grades_index, InMasterDatas);
	//int32 FixMinGrade = SummonItemSpecialGradeRange.Num() > 0 ? SummonItemSpecialGradeRange[0]->min_grade : 0;

	//auto& SummonItemGradeRange = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_lottery_possible_grades_index, InMasterDatas);
	//MD_Store_SummonItem_GradeRange.Empty(SummonItemGradeRange.Num());
	//for (auto& Elem : SummonItemGradeRange)
	//{
	//	MD_Store_SummonItem_GradeRange.Add(FSummonItemGradeRange::GetHashKey(Elem->draw_price_type - b2network::B2EquipCategory::WEAPON), FSummonItemGradeRange(Elem->min_grade, Elem->max_grade, FixMinGrade > 0 ? FixMinGrade : Elem->min_grade));
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopLotteryDailylimitsMaxCount(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopLotteryDailylimitsMaxCount);
	//auto& data = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_lottery_daily_limits_index, InMasterDatas);
	////模备惑痢 混荐乐绰 弥措摹甫 罐酒可聪促 
	//for (auto& Elem : data)
	//{
	//	uint32 HashKey = FSummonItemSlotData::GetHashKey(int32(ESummonItemTabMenu::FriendShipBox), Elem->is_continuous, false);
	//	if (MD_Store_SummonItem_ListData.Contains(HashKey))
	//	{
	//		MD_Store_SummonItem_ListData[HashKey].SetLimitdBuyCount(Elem->limit_count);
	//	}
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopProduct(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopProduct);
	//auto& StoreProductData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_products_index, InMasterDatas);

	//MD_Store_GeneralShopProductData.Empty();
	//MD_Store_CategoryOrderData.Empty();

	//for (auto& Elem : StoreProductData)
	//{
	//	FStoreProductData Data;

	//	Data.StoreProductId = Elem->id;

	//	Data.StoreCategory = static_cast<int32>(Elem->shop_type - b2network::B2ShopType::SHOP_PACKAGE);
	//	Data.StoreCategoryOrder = Elem->shop_order;

	//	Data.GoodsCategory = Data.StoreCategory == static_cast<int32>(EStoreCategory::Goods) ?
	//		static_cast<int32>(Elem->product_type - b2network::B2ShopProductType::BLADE_POINT) : -1;
	//	Data.GoodsCategoryOrder = Elem->product_order;

	//	Data.Product.ProductId = Elem->product_id;
	//	Data.Product.ProductType = static_cast<int32>(Elem->product_type - b2network::B2ShopProductType::BLADE_POINT);
	//	Data.Product.ProductCount = Elem->product_count;

	//	Data.Bonus.ProductId = Elem->bonus_product_id;
	//	Data.Bonus.ProductType = static_cast<int32>(Elem->bonus_product_type - b2network::B2ShopProductType::BLADE_POINT);
	//	Data.Bonus.ProductCount = Elem->bonus_product_count;

	//	Data.CalculatedBonusCount = Elem->calculated_bonus_count;
	//	Data.CalculatedPrice = Elem->calculated_price;

	//	Data.CostType = static_cast<int32>(Elem->buy_price_type - b2network::B2ShopBuyPriceType::GEM);
	//	Data.Cost = Elem->price;

	//	Data.RestrictType = Elem->restric_type;
	//	Data.RestrictCount = Elem->buy_limit_count;
	//	Data.PayPeriod = 0;

	//	MD_Store_GeneralShopProductData.Add(Elem->id, Data);

	//	if (Elem->shop_type != b2network::B2ShopType::SHOP_PACKAGE) // 菩虐瘤 徘狼 家泼篮 蝶肺 付胶磐单捞磐肺 罐绊乐澜. 惑痢俊辑绰 力寇
	//		MD_Store_CategoryOrderData.Add(Data.StoreCategoryOrder, Data.StoreCategory);
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopPackage(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopPackage);
	//auto& StorePackageData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_packages_index, InMasterDatas);
	//for (auto& PackageElem : StorePackageData)
	//{
	//	FPackageData Data;
	//	Data.PackageCategory = PackageElem->category;
	//	Data.PackageTabType = PackageElem->tab_type;
	//	Data.PackagePeriod = PackageElem->pay_period;
	//	Data.PackagePeriodBonusID = PackageElem->period_bonus_reward_id;
	//	Data.PackagePeriodBonusCount = PackageElem->period_bonus_reward_count;

	//	MD_Store_PackageData.Add(PackageElem->id, Data);
	//}
}

void FClientDataStore::SetMasterDataImpl_PackageProduct(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopPackage);
	//MD_Store_PackageProductData.Empty();
	//auto& StorePackageData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_package_products_index, InMasterDatas);
	//for (auto& PackageElem : StorePackageData)
	//{
	//	FPackageProductData Data;
	//	Data.PackageIndex = PackageElem->index;
	//	Data.PackageProductId = PackageElem->product->product_id;
	//	Data.PackageProductType = static_cast<int32>(PackageElem->product->product_type - b2network::B2ShopProductType::BLADE_POINT);
	//	Data.PackageProductCount = PackageElem->product->product_count;

	//	MD_Store_PackageProductData.Add(PackageElem->package_id, Data);
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopMileageReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopMileageReward);
	//MD_Store_Mileage_RewardData.Empty();
	//auto& StoreMileageRewardData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_mileage_rewards_index, InMasterDatas);
	//for (auto& Elem : StoreMileageRewardData)
	//{
	//	uint32 HashKey = FStoreMileageData::GetHashkey(Elem->mileage_type - b2network::B2ShopMileageType::LOTTERY, Elem->index - 1);
	//	FStoreMileageData Data(HashKey);
	//	Data.MaxMileage = Elem->mileage;

	//	for (auto& Reward : Elem->rewards)
	//	{
	//		Data.RewardIds.Add(FRewardItem(Reward->raw_reward->id, Reward->raw_reward->count));
	//	}

	//	MD_Store_Mileage_RewardData.Add(HashKey, Data);
	//}
}

void FClientDataStore::SetMasterDataImpl_CollectionReward(const FB2MasterDatas& InMasterDatas)
{

}

void FClientDataStore::SetMasterDataImpl_DailyAttendanceInfo(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_DailyAttendanceInfo);
	//m_AttendanceDayDatas.Empty();
	//const TArray<b2network::B2mdDailyAttendanceInfoPtr>& DailyAttendanceInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_daily_atttendances_index, InMasterDatas);

	//for (auto& attenddaydata : DailyAttendanceInfo)
	//{
	//	if (!m_AttendanceDayDatas.Contains(attenddaydata->attendance_type))
	//	{
	//		m_AttendanceDayDatas.Add(attenddaydata->attendance_type, TArray<FAttendanceDayData>());
	//	}
	//	FAttendanceDayData NewAttendDayData;
	//	NewAttendDayData.nAttendanceType = attenddaydata->attendance_type;
	//	NewAttendDayData.nDay = attenddaydata->day;
	//	NewAttendDayData.nRewardId = attenddaydata->reward_id;
	//	NewAttendDayData.nRewardCount = attenddaydata->reward_count;
	//	m_AttendanceDayDatas[attenddaydata->attendance_type].Add(NewAttendDayData);
	//}
}

// jwyim GuildFormation
//void FClientDataStore::SetMasterDataImpl_GuildBattleFormation(const FB2MasterDatas& InMasterDatas)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_GuildBattleFormation);
//	GuildMatchFormationMasterData.Empty();
//	const TArray<b2network::B2mdGuildBattleFormationPtr>& GuildBattleFormationPtr = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_guild_battle_formations_index, InMasterDatas);
//
//	for (auto& guildFormationData : GuildBattleFormationPtr)
//	{
//		FTeamMatchFormationLevelInfo LevelInfo;
//
//		LevelInfo.EnhanceGoldCost = guildFormationData->enhance_gold_cost;
//
//		// TODO formation buff
//		for (auto FormationBuff : guildFormationData->buffs)
//		{
//			FTeamMatchFormationBuffInfo BuffInfo;
//			BuffInfo.Id = FormationBuff->id;
//			BuffInfo.BuffType1 = FormationBuff->buff_type1;
//			BuffInfo.BuffValue1 = FormationBuff->buff_value1;
//			BuffInfo.BuffType2 = FormationBuff->buff_type2;
//			BuffInfo.BuffValue2 = FormationBuff->buff_value2;
//
//			if (!LevelInfo.BuffInfo.Contains(BuffInfo.Id))
//			{
//				LevelInfo.BuffInfo.Add(BuffInfo.Id, BuffInfo);
//			}
//		}
//
//		if (!GuildMatchFormationMasterData.Contains(guildFormationData->formation_type))
//		{
//			GuildMatchFormationMasterData.Add(guildFormationData->formation_type, FTeamMatchFormationTypeInfo(guildFormationData->formation_type));
//		}
//
//		FTeamMatchFormationTypeInfo* TypeInfo = GuildMatchFormationMasterData.Find(guildFormationData->formation_type);
//		BII_CHECK(TypeInfo);
//		if (TypeInfo)
//		{
//			TypeInfo->LevelInfo.Add(guildFormationData->formation_level, LevelInfo);
//		}
//	}
//}

void FClientDataStore::SetMasterDataImpl_GuildBattleResultReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_GuildBattleResultReward);
	//GuildBattleResultRewardMasterData.Empty();

	//const TArray<b2network::B2mdGuildBattleResultRewardPtr>& GuildReward = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_guild_battle_result_rewards_index, InMasterDatas);

	//for (auto& RewardInfo : GuildReward)
	//{
	//	GuildBattleResultRewardMasterData.Add(RewardInfo->reward_grade, RewardInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_GuildBattleSeasonReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_GuildBattleSeasonReward);
	//GuildSeasonRewardInfo.Empty();
	//const TArray<b2network::B2mdGuildBattleSeasonRewardPtr>& GuildSeasonReward = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_guild_battle_season_rewards_index, InMasterDatas);

	//for (auto& SeasonInfo : GuildSeasonReward)
	//{
	//	GuildSeasonRewardInfo.Add(SeasonInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_Raid(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_Raid);
	//auto* RaidInfoTable = StaticFindRaidInfo();

	//FRaidMasterData RaidInfo;

	//RaidMasterDatas.Empty();
	//auto& MasterData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_raid_infos_index, InMasterDatas);

	//int32 OldRaidId = -1;

	//for (auto& RaidEle : MasterData)
	//{
	//	RaidInfo.clear();
	//	RaidInfo.RaidType = RaidEle->raid_type;
	//	RaidInfo.RaidStep = RaidEle->step;
	//	RaidInfo.DifficultyScale = RaidEle->difficulty_scale;
	//	RaidInfo.RaidName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString::Printf(TEXT("Raid_Name%d"), RaidInfo.RaidType));
	//	RaidInfo.RaidDesc = BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString::Printf(TEXT("Raid_Desc%d"), RaidInfo.RaidType));
	//	RaidInfo.RecommendCombatPoint = RaidEle->recommend_power;
	//	RaidInfo.RecommendAttack = RaidEle->recommend_attack;
	//	RaidInfo.RecommendDefense = RaidEle->recommend_defense;

	//	RaidInfo.StepRewardID = RaidEle->reward_id;
	//	RaidInfo.StepRewardMinCount = RaidEle->reward_min_count;
	//	RaidInfo.StepRewardMaxCount = RaidEle->reward_max_count;
	//	RaidInfo.RewardEquipItemMinGrade = RaidEle->reward_item_min_grade;
	//	RaidInfo.RewardEquipItemMaxGrade = RaidEle->reward_item_max_grade;

	//	RaidInfo.MainRewardID = RaidEle->main_reward_id;
	//	RaidInfo.MainRewardMinCount = RaidEle->main_reward_min_count;
	//	RaidInfo.MainRewardMaxCount = RaidEle->main_reward_max_count;

	//	if (RaidMasterDatas.Contains(RaidInfo.RaidType) == false)
	//		RaidMasterDatas.Add(RaidInfo.RaidType, TArray<FRaidMasterData>());

	//	if (RaidMasterDatas.Contains(RaidInfo.RaidType))
	//		RaidMasterDatas[RaidInfo.RaidType].Add(RaidInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_MonsterInfo(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_MonsterInfo);
	//auto& MonsterInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_monster_infos_index, InMasterDatas);
	//MD_MonterListData.Empty();
	//UEnum *EnumNpcClass = nullptr;
	//UEnum *EnumGradeClass = nullptr;
	//for (auto& Elem : MonsterInfo)
	//{
	//	EnumNpcClass = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENPCClass"), true);
	//	EnumGradeClass = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENPCClassVariation"), true);

	//	if (!EnumNpcClass || !EnumGradeClass)
	//		continue;

	//	int32 NpcClass = EnumNpcClass->GetIndexByName(FName(*Elem->symbol));
	//	int32 NpcGrade = EnumGradeClass->GetIndexByName(FName(*Elem->grade));
	//	if (MD_MonterListData.Contains(NpcGrade) == false)
	//		MD_MonterListData.Add(NpcGrade, TMap<int32, FMonsterInfo>());
	//	FMonsterInfo EachMonster;
	//	EachMonster.Id = Elem->id;
	//	EachMonster.NameIndex = -1;
	//	EachMonster.NpcClass = NpcClass;
	//	EachMonster.Grade = NpcGrade;
	//	EachMonster.MonsterType = 0;
	//	EachMonster.Desc = Elem->desc;

	//	EachMonster.HPScale = SvrToCliBalancingScaleFactor(Elem->hp_scale);
	//	EachMonster.AttackDefenseLevelScale = SvrToCliBalancingScaleFactor(Elem->attack_defense_level_scale);
	//	EachMonster.MaxArmorOverride = SvrToCliBalancingScaleFactor(Elem->max_armor_override);

	//	MD_MonterListData[NpcGrade].Add(NpcClass, EachMonster);
	//}
}

void FClientDataStore::SetMasterDataImpl_GeneralLottery(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_GeneralLottery);
	//auto& GeneralLotteryInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_general_lotteries_index, InMasterDatas);
	//MD_GeneralLottery.Empty();

	//for (auto& InfoElem : GeneralLotteryInfo)
	//{
	//	MD_GeneralLottery.Add(InfoElem->id, InfoElem);
	//}
}

void FClientDataStore::SetMasterDataImpl_SelectiveLottery(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_SelectiveLottery);
	//auto& SelectiveLotteryInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_selective_lotteries_index, InMasterDatas);
	//MD_SelectiveLottery.Empty();

	//for (auto& InfoElem : SelectiveLotteryInfo)
	//{
	//	MD_SelectiveLottery.Add(InfoElem->id, InfoElem);
	//}
}

void FClientDataStore::SetMasterDataImpl_PrefixSelectiveLottery(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(SetMasterDataImpl_PrefixSelectiveLottery);
	//auto& SelectiveLotteryInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_prefix_selective_lotteries_index, InMasterDatas);
	//MD_PrefixSelectiveLottery.Empty();

	//for (auto& InfoElem : SelectiveLotteryInfo)
	//{
	//	MD_PrefixSelectiveLottery.Add(InfoElem->id, InfoElem);
	//}

}

void FClientDataStore::SetMasterDataImpl_FixedGradeLotteries(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_FixedGradeLotteries);
	//auto& FixedLotteryInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_fixed_grade_lotteries_index, InMasterDatas);
	//MD_FixedGradeLottery.Empty();

	//for (auto& InfoElem : FixedLotteryInfo)
	//{
	//	MD_FixedGradeLottery.Add(InfoElem->id, InfoElem);
	//}
}

void FClientDataStore::SetMasterDataImpl_ItemEnhancementCost(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ItemEnhancementCost);
	//auto& MasterDataInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_item_enhance_costs_index, InMasterDatas);
	//MD_EnhanceItemCostData.Empty();

	//for (const auto& mdItemEnhanceCost : MasterDataInfo)
	//{
	//	if (MD_EnhanceItemCostData.Contains(mdItemEnhanceCost->enhance_level) == false)
	//	{
	//		FB2EnhanceItemCost EnhanceItemCost;
	//		EnhanceItemCost.UpdateByServerSync(mdItemEnhanceCost);
	//		MD_EnhanceItemCostData.Add(mdItemEnhanceCost->enhance_level, EnhanceItemCost);
	//	}
	//	else
	//	{
	//		MD_EnhanceItemCostData[mdItemEnhanceCost->enhance_level].UpdateByServerSync(mdItemEnhanceCost);
	//	}
	//}
}

void FClientDataStore::SetMasterDataImpl_ItemEnhancementEffectInfo(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ItemEnhancementEffectInfo);
	//auto& MasterDataInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_item_enhance_effect_infos_index, InMasterDatas);
	//MD_EnhanceItemEffectData.Empty();
	//for (const auto& mdItemEnhanceEffect : MasterDataInfo)
	//{
	//	if (MD_EnhanceItemEffectData.Contains(mdItemEnhanceEffect->enhance_level) == false)
	//	{
	//		FItemEnhanceEffectInfo EnhanceItemEffect;
	//		EnhanceItemEffect.UpdateByServerSync(mdItemEnhanceEffect);
	//		MD_EnhanceItemEffectData.Add(mdItemEnhanceEffect->enhance_level, EnhanceItemEffect);
	//	}
	//	else
	//	{
	//		MD_EnhanceItemEffectData[mdItemEnhanceEffect->enhance_level].UpdateByServerSync(mdItemEnhanceEffect);
	//	}
	//}

	//if (auto* FindPredicate = MasterDataInfo.FindByPredicate([](const b2network::B2mdItemEnhanceEffectInfoPtr EffectPtr) { return (EffectPtr && EffectPtr->set_effect != 0); }))
	//	MD_MinEnhanceLevelForSetEffect = (*FindPredicate)->enhance_level;

	//TArray<int32> keys;
	//MD_EnhanceItemEffectData.GetKeys(keys);
	//keys.Sort([](const int32 A, const int32 B) { return A > B; });
	//if (keys.IsValidIndex(0))
	//	MD_MaxEnhanceLevelForSetEffect = keys[0];
}

void FClientDataStore::SetMasterDataImpl_AdditionalMatchPointInfos(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_AdditionalMatchPointInfos);
	//auto& MasterDataInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_additional_match_cost_infos_index, InMasterDatas);
	//MD_AdditonalMatchCostData.Empty();

	//for (const auto& mdItemEnhanceCost : MasterDataInfo)
	//{
	//	MD_AdditonalMatchCostData.Add(mdItemEnhanceCost->cost);
	//}
}

void FClientDataStore::SetMasterDataImpl_StageFixedDrop(const FB2MasterDatas& InMasterDatas)
{
	//BladeIIGameImpl::GetStageDataStore().SetMasterData(InMasterDatas) 救俊 SetMDStageFixedDrops俊辑 窍绊 乐澜
}

void FClientDataStore::SetMasterDatalmpl_AssaultMvpRewardInfo(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDatalmpl_AssaultMvpRewardInfo);
	//auto& MasterDataInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_assault_mvp_reward_infos_index, InMasterDatas);

	//auto ControlDoc = UB2UIDocHelper::GetDocControl();

	//if (ControlDoc)
	//{
	//	ControlDoc->SetAssaultMvpRewardInfo(MasterDataInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_EventAttendanceReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_EventAttendanceReward);
	//auto& MasterDataInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_event_attendance_rewards_index, InMasterDatas);
	//EventAttendanceRewardData.Empty();
	//for (auto& RewardData : MasterDataInfo)
	//{
	//	FEventAttendanceReward EventAttendanceReward;
	//	EventAttendanceReward.event_id = RewardData->event_id;
	//	EventAttendanceReward.day = RewardData->day;
	//	EventAttendanceReward.reward_id = RewardData->reward_id;
	//	EventAttendanceReward.reward_count = RewardData->reward_count;

	//	if (nullptr == EventAttendanceRewardData.Find(RewardData->event_id))
	//	{
	//		EventAttendanceRewardData.Add(RewardData->event_id, TArray<FEventAttendanceReward>());
	//	}
	//	EventAttendanceRewardData[RewardData->event_id].Add(EventAttendanceReward);
	//}
}

void FClientDataStore::SetMasterDataImpl_PlayTimeReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_PlayTimeReward);
	//PlayTimeRewradData.Reset();
	//auto& MasterDataInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_daily_play_time_rewards_index, InMasterDatas);
	//for (auto& RewardData : MasterDataInfo)
	//{
	//	FPlayTimeRewrad PlayTimeReward;
	//	PlayTimeReward.reward_index = RewardData->reward_index;
	//	PlayTimeReward.play_time_inmin = RewardData->play_time_inmin;
	//	PlayTimeReward.reward_id = RewardData->reward_id;
	//	PlayTimeReward.reward_count = RewardData->reward_count;
	//	PlayTimeRewradData.Add(PlayTimeReward);
	//}
}

void FClientDataStore::SetMasterDataImpl_GameResultExp(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_GameResultExp);
	//MD_RaidExpData.Empty();

	//const TArray<b2network::B2mdRaidExpPtr>& AllRaidExpData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_raid_exp_index, InMasterDatas);
	//for (const b2network::B2mdRaidExpPtr& ThisSvrData : AllRaidExpData)
	//{
	//	FRaidExpMDSingleData NewMDDataElem;
	//	NewMDDataElem.UpdateByServerSync(ThisSvrData);
	//	MD_RaidExpData.Add(NewMDDataElem.GetMyKey(), NewMDDataElem);
	//}
}

void FClientDataStore::SetMasterDataImpl_Tutorial(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_Tutorial);
	//const TArray<b2network::B2mdTutorialInfoPtr>& AllTutorialInfos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_tutorial_infos_index, InMasterDatas);
	//TutorialManager::GetInstance().SetTutorialMasterData(AllTutorialInfos);
}

void FClientDataStore::SetMasterDataImpl_GuildExps(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_GuildExps);
	//const TArray<b2network::B2mdGuildExpPtr>& GuildExpData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_guild_exps_index, InMasterDatas);

	//MD_GuildExps.Empty();

	//for (const b2network::B2mdGuildExpPtr& GuildExp : GuildExpData)
	//{
	//	MD_GuildExps.Add(GuildExp->level, GuildExp);
	//}
}

void FClientDataStore::SetMasterDataImpl_GuildSkill(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_GuildSkill);
	//const TArray<b2network::B2mdGuildSkillPtr>& GuildSkills = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_guild_skills_index, InMasterDatas);

	//GuildSkillInfoMasterData.Empty();

	//for (auto& GuildSkill : GuildSkills)
	//{
	//	GuildSkillInfoMasterData.Add(FGuildSkillHashKey::GetHashKey(GuildSkill->id, GuildSkill->level), GuildSkill);
	//}
}

void FClientDataStore::SetMasterDataImpl_Guild(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_Guild);
	BladeIIGameImpl::GetGuildDataStore().SetMasterData(InMasterDatas);
}

void FClientDataStore::SetMasterDataImpl_GuildBattleSchedule(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_GuildBattleSchedule);
	//GuildBattleSchedule = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_guild_battle_schedule_infos_index, InMasterDatas);

	//GuildBattleSchedule.Sort([](b2network::B2mdGuildBattleScheduleInfoPtr Ele1, b2network::B2mdGuildBattleScheduleInfoPtr Ele2)->bool {
	//	if (Ele1->turn < Ele2->turn) return true; else return false;
	//});

}

void FClientDataStore::SetMasterDataImpl_RestReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_RestReward);
	//const TArray<b2network::B2mdRestRewardPtr>& RestReward = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_rest_rewards_index, InMasterDatas);

	//MD_RestRewardData.Empty();

	//for (auto& RewardItem : RestReward)
	//{
	//	MD_RestRewardData.Add(RewardItem);
	//}
}

void FClientDataStore::SetMasterDataImpl_DuelModeReward(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_DuelModeReward);
	//const TArray<b2network::B2mdDuelModeRewardPtr>& DualReward = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_duel_mode_reward_index, InMasterDatas);

	//MD_DuelModeRewardInfo.Empty();

	//for (auto& Rewardinfo : DualReward)
	//{
	//	MD_DuelModeRewardInfo.Add(Rewardinfo->duel_mode, Rewardinfo->rewards);
	//}

}

void FClientDataStore::SetMasterDataImpl_Item(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_Item);
	//ItemMasterData.InitItemMasterData();

	//const TArray<b2network::B2mdItemInfoPtr>& ItemInfos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_item_infos_index, InMasterDatas);
	//for (b2network::B2mdItemInfoPtr ItemInfo : ItemInfos)
	//{
	//	FMD_ItemSingleElem SingleItemInfo;
	//	SingleItemInfo = ItemInfo;

	//	ItemMasterData.AddItemMasterData(SingleItemInfo);
	//}

	//const TArray<b2network::B2mdCostumeInfoPtr>& ArrayMDCostumeInfo = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_costume_infos_index, InMasterDatas);
	//const int32 iArrayMax = ArrayMDCostumeInfo.Num();
	//for (int32 i = 0; i < iArrayMax; ++i)
	//{
	//	if (ArrayMDCostumeInfo.IsValidIndex(i))
	//	{
	//		FMD_ItemSingleElem SingleItemInfo;
	//		SingleItemInfo.InitCostumeItemInfo(ArrayMDCostumeInfo[i]);
	//		ItemMasterData.AddItemMasterData(SingleItemInfo);
	//	}
	//}

	//ItemMasterData.CharacterItems[EPCClass::EPC_Gladiator].SortAllItemMasterInfo();
	//ItemMasterData.CharacterItems[EPCClass::EPC_Assassin].SortAllItemMasterInfo();
	//ItemMasterData.CharacterItems[EPCClass::EPC_Wizard].SortAllItemMasterInfo();
	//ItemMasterData.CharacterItems[EPCClass::EPC_Fighter].SortAllItemMasterInfo();

	//const TArray<b2network::B2mdItemSetPtr>& ItemSetInfos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_item_set_infos_index, InMasterDatas);
	//for (b2network::B2mdItemSetPtr SetOptionInfo : ItemSetInfos)
	//{
	//	FMD_ItemSetOptionElem SetItemAbility;
	//	SetItemAbility = SetOptionInfo;
	//	ItemMasterData.SetItemOptions.Add(SetItemAbility.SetOptionID, SetItemAbility);
	//}

	//// move to PostSetMasterData()
	////// 酒捞袍 付胶磐 单捞磐 罐扁 傈俊 AccountInfo 烹秦 馒侩 酒捞袍 单捞磐甫 罐疽阑 巴捞骨肺 捞 矫痢俊辑 付胶磐 单捞磐甫 扁馆栏肺 单捞磐甫 盎脚窃.
	////LocalCharacterData.UserAllItems.UpdateItemMasterDataOnMD();

	////for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	////{ // 林夸 傈捧仿 荐摹 UI 诀单捞飘. 付胶磐 单捞磐 罐篮 贸澜俊 诀单捞飘 秦 临 鞘夸啊 乐促.
	////	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(IntToPCClass(PCI), false);
	////}
}

void FClientDataStore::SetMasterDataImpl_EventInfo(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_EventInfo);

	RewardEventManager::GetInstance().InitEventFromMD(InMasterDatas);
}

void FClientDataStore::SetMasterDataImpl_KakaoInviteReward(const FB2MasterDatas& InMasterDatas)
{
	//const TArray<b2network::B2mdKakaoFriendInviteRewardPtr>& KakaoFriendInfos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_kakao_friend_invite_rewards_index, InMasterDatas);

	//MD_KakaoFriendInviteRewardInfo.Empty();

	//for (auto& KakaoFriendInfo : KakaoFriendInfos)
	//{
	//	if (MD_KakaoFriendInviteRewardInfo.Contains(KakaoFriendInfo->invite_reward_type) == false)
	//		MD_KakaoFriendInviteRewardInfo.Add(KakaoFriendInfo->invite_reward_type, TArray<b2network::B2mdKakaoFriendInviteRewardPtr>());
	//	MD_KakaoFriendInviteRewardInfo[KakaoFriendInfo->invite_reward_type].Add(KakaoFriendInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_HotTimeEvent(const FB2MasterDatas& InMasterDatas, const FEventInfo& HotTimeEventInfo)
{
	//FClientDataStore -> RewardEventManager肺 捞包

	//const TArray<b2network::B2mdHotTimeEventPtr>& ArrHotTimeEvent = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_hot_time_events_index, InMasterDatas);

	//TArray<FHotTimeEventDetailInfo> DetailInfos;
	//DetailInfos.SetNum(ArrHotTimeEvent.Num());

	//for (int32 i = 0; i < DetailInfos.Num(); ++i)
	//{
	//	DetailInfos[i].event_id = ArrHotTimeEvent[i]->event_id;
	//	DetailInfos[i].index = ArrHotTimeEvent[i]->index;
	//	DetailInfos[i].day_of_week = ArrHotTimeEvent[i]->day_of_week;
	//	DetailInfos[i].buff_type = SvrToCliHotTimeBuffType(ArrHotTimeEvent[i]->buff_type);
	//	DetailInfos[i].begin_hour = ArrHotTimeEvent[i]->begin_hour;
	//	DetailInfos[i].begin_min = ArrHotTimeEvent[i]->begin_min;
	//	DetailInfos[i].end_hour = ArrHotTimeEvent[i]->end_hour;
	//	DetailInfos[i].end_min = ArrHotTimeEvent[i]->end_min;
	//	DetailInfos[i].increase_rate = ArrHotTimeEvent[i]->increase_rate;
	//}
	//HotTimeBuffManager::GetInstance().SetHotTimeEventInfo(HotTimeEventInfo, DetailInfos);
}

void FClientDataStore::SetMasterDataImpl_HotTimeProduct(const FB2MasterDatas & InMasterDatas)
{
	//const TArray<b2network::B2mdHotTimeProductPtr>& ArrHotTimeProducts = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_hot_time_products_index, InMasterDatas);

	//TArray<FHotTimeProductBuffInfo> HotTimeProducts;
	//HotTimeProducts.SetNum(ArrHotTimeProducts.Num());

	//for (int32 i = 0; i < HotTimeProducts.Num(); ++i)
	//{
	//	HotTimeProducts[i].buff_type = SvrToCliHotTimeBuffType(ArrHotTimeProducts[i]->buff_type);
	//	HotTimeProducts[i].period_hour = ArrHotTimeProducts[i]->period_hour;
	//	HotTimeProducts[i].price = ArrHotTimeProducts[i]->price;
	//	HotTimeProducts[i].increaseRate = ArrHotTimeProducts[i]->increaseRate;
	//}

	//HotTimeBuffManager::GetInstance().SetHotTimeProductBuffInfo(HotTimeProducts);
}

void FClientDataStore::SetMasterDataImpl_ShopCashBonus(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopCashBonus);

	//auto& ShopCashBonusData = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_cash_bonus_infos_index, InMasterDatas);
	//MD_Store_CashBonusData.Empty();

	//for (auto& Elem : ShopCashBonusData)
	//{
	//	bool bIsInclude = UB2GameInstance::IncludeTimeNow(Elem->start_time, Elem->end_time);
	//	if (Elem->increase_rate > 0 && bIsInclude)
	//	{
	//		FStoreCashBonusData Data;
	//		Data.StoreProductId = Elem->id;
	//		Data.GoodsCategory = GetStoreProductData(Elem->id)->GoodsCategory;
	//		Data.IncreaseRate = Elem->increase_rate;
	//		Data.StartTime = Elem->start_time;
	//		Data.EndTime = Elem->end_time;

	//		MD_Store_CashBonusData.Add(Data.StoreProductId, Data);
	//	}
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopFundsSale(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopFundsSale);

	//auto& ShopFundsSale = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_funds_sale_infos_index, InMasterDatas);
	//MD_Store_FundsSaleData.Empty();

	//for (auto& Elem : ShopFundsSale)
	//{
	//	bool bIsInclude = UB2GameInstance::IncludeTimeNow(Elem->start_time, Elem->end_time);
	//	if (Elem->discount_rate > 0 && bIsInclude)
	//	{
	//		FStoreFundsSale Data;
	//		Data.StoreProductId = Elem->id;
	//		Data.StoreCategory = GetStoreProductData(Elem->id)->StoreCategory;
	//		Data.GoodsCategory = GetStoreProductData(Elem->id)->GoodsCategory;
	//		Data.DiscountRate = Elem->discount_rate;
	//		Data.StartTime = Elem->start_time;
	//		Data.EndTime = Elem->end_time;

	//		MD_Store_FundsSaleData.Add(Data.StoreProductId, Data);
	//	}
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopLotterySale(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopLotterySale);

	//auto& ShopLotterySale = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_lottery_sale_infos_index, InMasterDatas);
	//MD_Store_LotterySaleData.Empty();

	//for (auto& Elem : ShopLotterySale)
	//{
	//	bool bIsInclude = UB2GameInstance::IncludeTimeNow(Elem->start_time, Elem->end_time);
	//	if (Elem->discount_rate > 0 && bIsInclude)
	//	{
	//		uint32 HashKey = FSummonItemLotterySale::GetHashKey(Elem->draw_price_type - b2network::B2ShopDrawPriceType::GEM, Elem->is_continuous, false);
	//		MD_Store_LotterySaleData.Add(HashKey, FSummonItemLotterySale(Elem->discount_rate, Elem->start_time, Elem->end_time));

	//		HashKey = FSummonItemLotterySale::GetHashKey(Elem->draw_price_type - b2network::B2ShopDrawPriceType::GEM, Elem->is_continuous, true);
	//		MD_Store_LotterySaleData.Add(HashKey, FSummonItemLotterySale(Elem->discount_rate, Elem->start_time, Elem->end_time));
	//	}
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopLevelUpPackage(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopLevelUpPackage);

	//const TArray<FB2ShopLevelUpPackagePtr>& PackageLevel = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_level_up_packages_index, InMasterDatas);
	//MD_Store_LevelUpPackageData.Empty();

	//for (auto& Each : PackageLevel)
	//{
	//	MD_Store_LevelUpPackageData.Add(Each->package_id, Each);
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopFlatratePackageBonus(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopReachLevelPackage);

	//const TArray<FB2ShopFlatRatePackageBonusPtr>& FlatRatePackageBonus = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_flat_rate_package_bonus_index, InMasterDatas);
	//MD_Store_FlatRatePackageBonusData.Empty();

	//for (auto& Elem : FlatRatePackageBonus)
	//{
	//	MD_Store_FlatRatePackageBonusData.Add(Elem->index, Elem);
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopReachLevelPackage(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ShopReachLevelPackage);

	//const TArray<FB2ShopReachLevelPackagePtr>& ReachLevelPackage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_reach_level_packages_index, InMasterDatas);
	//MD_Store_ReachLevelPackageData.Empty();

	//for (auto& Elem : ReachLevelPackage)
	//{
	//	MD_Store_ReachLevelPackageData.Add(Elem->package_id, Elem);
	//}
}

void FClientDataStore::SetMasterDataImpl_DuelModeSettleSchedule(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_DuelModeSettleSchedule);

	//MD_DuelModeSettleSchedules.Empty();

	//auto& DuelModeSettleSchedules = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_duel_mode_settle_schedules_index, InMasterDatas);

	//for (auto& Elem : DuelModeSettleSchedules)
	//{
	//	MD_DuelModeSettleSchedules.Add(Elem);
	//}
}

void FClientDataStore::SetMasterDataImpl_ShopPackageTabOrder(const FB2MasterDatas& InMasterDatas)
{
	//const TArray<FB2ShopPackageTabPtr>& PackageTabOrder = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_shop_package_tab_infos_index, InMasterDatas);
	//MD_Store_PackageOrderData.Empty();

	//for (auto& Each : PackageTabOrder)
	//{
	//	MD_Store_PackageOrderData.Add(Each);
	//}

	//MD_Store_PackageOrderData.Sort([](const FB2ShopPackageTabPtr& Data1, const FB2ShopPackageTabPtr& Data2)->bool {
	//	return Data1->sort_order < Data2->sort_order;
	//});
}

void FClientDataStore::SetMasterDataImpl_WebURL(const FB2MasterDatas& InMasterDatas)
{
	//const TArray<b2network::B2mdWebUrlPtr>& URLs = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_web_urls_index, InMasterDatas);
	//MD_WebURL.Empty();

	//for (auto& Each : URLs)
	//{
	//	MD_WebURL.Add(Each->key, Each->url);
	//}
}

void FClientDataStore::SetMasterDataImpl_MagicShopProduct(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_MagicShopProduct);

	//auto& MagicShopProduct = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_magic_shop_product_index, InMasterDatas);
	//MD_MagicShopProduct.Empty();

	//for (auto& Elem : MagicShopProduct)
	//{
	//	FMagicShopProduct Data;
	//	Data.MagicShopID = Elem->id;
	//	Data.MagicShopGroupType = Elem->magic_shop_group_type;
	//	Data.ProductType = static_cast<int32>(Elem->product_type - b2network::B2ShopProductType::BLADE_POINT);
	//	Data.ProductDetail = Elem->product_detail;
	//	Data.ProductCount = Elem->product_count;
	//	Data.PriceType = static_cast<int32>(Elem->price_type - b2network::B2ShopBuyPriceType::GEM);
	//	Data.Price = Elem->price;

	//	MD_MagicShopProduct.Add(Data.MagicShopID, Data);
	//}
}

void FClientDataStore::SetMasterDataImpl_MagicShopRenewalCost(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_MagicShopRenewalCost);

	//auto& MagicShopRenewalCost = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_magic_shop_renewal_cost_index, InMasterDatas);
	//MD_MagicShopRenewalCost.Empty();

	//for (auto& Elem : MagicShopRenewalCost)
	//{
	//	FMagicShopRenewalCost Data;
	//	Data.RenewalCount = Elem->renewal_count;
	//	Data.GemCost = Elem->gem_cost;

	//	MD_MagicShopRenewalCost.Add(Data.RenewalCount, Data);
	//}
}

void FClientDataStore::SetMasterDataImpl_ItemlevelupFactor(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_ItemlevelupFactor);

	//auto& ServerLevelupFactor = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_item_levelup_factors_index, InMasterDatas);

	//const int32 MaxLevel = ServerLevelupFactor.Num();

	//MD_ItemLevelupPrimaryFactor.Empty();
	//MD_ItemLevelupPrimaryFactor.AddDefaulted(MaxLevel);

	//for (int32 LevelIndex = 0; LevelIndex < MaxLevel; LevelIndex++)
	//{
	//	auto& ServerFactor = ServerLevelupFactor[LevelIndex];

	//	FItemLevelupFactor Factor;
	//	Factor.ItemLevel = ServerFactor->level;
	//	Factor.LevelupFactor = static_cast<float>(ServerFactor->factor);

	//	MD_ItemLevelupPrimaryFactor[LevelIndex] = Factor;
	//}
}

void FClientDataStore::SetMasterDataImpl_SelectItemOption(const FB2MasterDatas& InMasterDatas)
{
	//auto& SelectItemOptions = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_select_item_option_index, InMasterDatas);

	//MD_SelectItemOption.Empty();

	//for (auto Item : SelectItemOptions)
	//{
	//	FSelectItemOption NewOption;
	//	NewOption.OptionId = Item->option_id;
	//	NewOption.Grade = Item->grade;
	//	NewOption.EquipPosition = Item->equip_position;
	//	NewOption.Prob = Item->prob;
	//	NewOption.DecimalPoint = Item->decimal_point;
	//	NewOption.MinValue = Item->min;
	//	NewOption.MaxValue = Item->max;

	//	int32 KeyID = NewOption.GetMyKey();

	//	if (MD_SelectItemOption.Contains(KeyID) == false)
	//		MD_SelectItemOption.Add(KeyID, TArray<FSelectItemOption>());

	//	if (MD_SelectItemOption.Contains(KeyID))
	//	{
	//		TArray<FSelectItemOption>* Array = MD_SelectItemOption.Find(KeyID);
	//		if (Array)
	//			(*Array).Add(NewOption);
	//	}
	//}
}

void FClientDataStore::SetMasterDataImpl_SurpassLevel(const FB2MasterDatas& InMasterDatas)
{
	//MD_SurpassLevelData.Empty();

	//auto& SurpassLevels = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_item_surpass_levels_index, InMasterDatas);

	//for (auto& SurpassLevel : SurpassLevels)
	//{
	//	FItemSurpassLevel InSurpassLevel;
	//	InSurpassLevel.Grade = SurpassLevel->grade;
	//	InSurpassLevel.IncreaseLevel = SurpassLevel->increase_level;
	//	InSurpassLevel.MaxSurpassLevel = SurpassLevel->max_surpass_level;

	//	MD_SurpassLevelData.Add(InSurpassLevel);
	//}
}

void FClientDataStore::SetMasterDataImpl_ItemDismantleInfo(const FB2MasterDatas& InMasterDatas)
{
	//auto& ItemDismatleInfos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_item_dismantle_infos_index, InMasterDatas);

	//MD_ItemDismantleInfo.Empty();

	//for (auto& Elem : ItemDismatleInfos)
	//{
	//	FItemDismantleInfo Data;
	//	Data.ItemInvenType = SvrToCliItemInvenType(Elem->inventory_type);
	//	Data.Grade = Elem->grade;
	//	if (Elem->material1 != 0)
	//	{
	//		Data.Matrial.Add(Elem->material1);
	//		Data.MatrialAmount.Add(Elem->material_amount1);
	//	}
	//	if (Elem->material2 != 0)
	//	{
	//		Data.Matrial.Add(Elem->material2);
	//		Data.MatrialAmount.Add(Elem->material_amount2);
	//	}
	//	if (Elem->material3 != 0)
	//	{
	//		Data.Matrial.Add(Elem->material3);
	//		Data.MatrialAmount.Add(Elem->material_amount3);
	//	}

	//	if (Elem->material1 != 0 || Elem->material2 != 0 || Elem->material2 != 0)
	//	{
	//		MD_ItemDismantleInfo.Add(Data.ItemInvenType, Data);
	//	}
	//}
}

void FClientDataStore::SetMasterDataImpl_FairyInfo(const FB2MasterDatas& InMasterDatas)
{
	//auto& FairyInfos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_fairy_infos_index, InMasterDatas);

	//MD_FairyInfoData.Empty();

	//EFairyType FairyType = EFairyType::FAIRY_NONE;
	//for (auto& Elem : FairyInfos)
	//{
	//	FairyType = SvrToCliFairyType(Elem->fairy_type);

	//	if (MD_FairyInfoData.Contains(FairyType) == false)
	//	{
	//		MD_FairyInfoData.Add(FairyType, TArray<FMD_FairyInfo>());
	//	}

	//	MD_FairyInfoData[FairyType].Add(Elem);
	//}
}

void FClientDataStore::SetMasterDataImpl_FairyLevelupCosts(const FB2MasterDatas& InMasterDatas)
{
	//auto& FairyInfos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_fairy_levelup_costs_index, InMasterDatas);

	//MD_FairyLevelUpCostData.Empty();

	//for (auto& Elem : FairyInfos)
	//{
	//	MD_FairyLevelUpCostData.Add(Elem->level, Elem);
	//}
}

void FClientDataStore::SetMasterDataImpl_FairyOpenInfo(const FB2MasterDatas& InMasterDatas)
{
	//auto& FairyInfos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_fairy_open_infos_index, InMasterDatas);

	//MD_FairyOpenInfoData.Empty();

	//EFairyType FairyType = EFairyType::FAIRY_NONE;
	//for (auto& Elem : FairyInfos)
	//{
	//	FairyType = SvrToCliFairyType(Elem->fairy_type);
	//	MD_FairyOpenInfoData.Add(FairyType, Elem);
	//}
}

void FClientDataStore::SetMasterDataImpl_Costume(const FB2MasterDatas& InMasterDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_SetMasterDataImpl_Costume);

	BladeIIGameImpl::GetCostumeDataStore().SetMasterData(InMasterDatas);
}

void FClientDataStore::SetMasterDataImpl_DonationPointRankingRewardInfo(const FB2MasterDatas& InMasterDatas)
{
	//auto& Infos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_donation_point_ranking_reward_infos_index, InMasterDatas);
	//UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	//if (DocDonation)
	//{
	//	DocDonation->SetMDDonationPointRankingRewardInfo(Infos);
	//}
}

void FClientDataStore::SetMasterDataImpl_DonationBundleFixedRewardInfo(const FB2MasterDatas& InMasterDatas)
{
	//auto& Infos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_donation_bundle_fixed_reward_infos_index, InMasterDatas);
	//UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	//if (DocDonation)
	//{
	//	DocDonation->SetMDDonationBundleFixedRewardInfo(Infos);
	//}
}

void FClientDataStore::SetMasterDataImpl_DonationPointStepRewardInfo(const FB2MasterDatas& InMasterDatas)
{
	//auto& Infos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_donation_point_step_reward_infos_index, InMasterDatas);
	//UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	//if (DocDonation)
	//{
	//	DocDonation->SetMDDonationPointStepRewardInfo(Infos);
	//}
}

void FClientDataStore::SetMasterDataImpl_ExtendMessage(const FB2MasterDatas& InMasterDatas)
{
	//BladeIIGameImpl::GetTotemDataStore().SetMasterData(InMasterDatas);
}

void FClientDataStore::SetMasterDataImpl_CostumeEnhanceCostInfo(const FB2MasterDatas& InMasterDatas)
{
	//FB2ExtendMessagePtr	ExtendMessage;
	//ExtendMessage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_extend_massages_index, InMasterDatas);

	//MD_CustumeEnhanceCostInfo.Empty();

	//for (const b2network::B2mdCostumeEnhanceCostPtr& ThisInfo : ExtendMessage->md_costume_enhance_cost)
	//{
	//	FCostumeEnhanceCostInfo NewInfo;

	//	NewInfo.Grade = ThisInfo->grade;
	//	NewInfo.EnhanceLevel = ThisInfo->enhance_level;
	//	NewInfo.MoneyCost = ThisInfo->money_cost;
	//	NewInfo.WeaponMaterialId = ThisInfo->weapon_material_id;
	//	NewInfo.WeaponAmount = ThisInfo->weapon_amount;
	//	NewInfo.WeaponRestoreAmount = ThisInfo->weapon_restore_amount;
	//	NewInfo.ArmorMaterialId = ThisInfo->armor_material_id;
	//	NewInfo.ArmorAmount = ThisInfo->armor_amount;
	//	NewInfo.ArmorRestoreAmount = ThisInfo->armor_restore_amount;

	//	MD_CustumeEnhanceCostInfo.Add(NewInfo.GetMyKey(), NewInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_ConvertTableInfo(const FB2MasterDatas& InMasterDatas)
{
	/*
	auto& ItemDismatleInfos = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_item_dismantle_infos_index, InMasterDatas);

	MD_ItemDismantleInfo.Empty();

	for (auto& Elem : ItemDismatleInfos)
	{
	FItemDismantleInfo Data;
	Data.ItemInvenType = SvrToCliItemInvenType(Elem->inventory_type);
	Data.Grade = Elem->grade;
	if (Elem->material1 != 0)
	{
	Data.Matrial.Add(Elem->material1);
	Data.MatrialAmount.Add(Elem->material_amount1);
	}
	if (Elem->material2 != 0)
	{
	Data.Matrial.Add(Elem->material2);
	Data.MatrialAmount.Add(Elem->material_amount2);
	}
	if (Elem->material3 != 0)
	{
	Data.Matrial.Add(Elem->material3);
	Data.MatrialAmount.Add(Elem->material_amount3);
	}

	if (Elem->material1 != 0 || Elem->material2 != 0 || Elem->material2 != 0)
	{
	MD_ItemDismantleInfo.Add(Data.ItemInvenType, Data);
	}
	}
	*/
	//FB2ExtendMessagePtr	ExtendMessage;
	//ExtendMessage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_extend_massages_index, InMasterDatas);

	//MD_ExchangeSystemInfo.Empty();

	//for (const b2network::B2mdExchangeSystemInfoPtr& ThisInfo : ExtendMessage->md_exchange_system_info)
	//{
	//	FConvertTableInfo NewInfo;

	//	NewInfo.tableId = ThisInfo->id;
	//	NewInfo.beforeConvertItemID = ThisInfo->template_id;
	//	NewInfo.beforeConvertItemQuantity = ThisInfo->request_count;
	//	NewInfo.afterConvertItemID = ThisInfo->exchange_template_id;
	//	NewInfo.afterConvertItemQuantity = ThisInfo->result_count;

	//	MD_ExchangeSystemInfo.Add(NewInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_LotteryShopRateQualityInfo(const FB2MasterDatas& InMasterDatas)
{
	//FB2ExtendMessagePtr	ExtendMessage;
	//ExtendMessage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_extend_massages_index, InMasterDatas);

	//MD_LotteryShopRateQualityInfo.Empty();

	//for (const b2network::B2mdLotteryShopRateQualityInfoPtr& ThisInfo : ExtendMessage->md_lottery_shop_rate_quality_info)
	//{
	//	FLotteryShopRateQualityInfo NewInfo;

	//	switch (ThisInfo->equip_category)
	//	{
	//	case b2network::B2EquipCategory::WEAPON: NewInfo.equipCategory = EEquipCategoryType::WEAPON; break;
	//	case b2network::B2EquipCategory::ARMOR: NewInfo.equipCategory = EEquipCategoryType::ARMOR; break;
	//	case b2network::B2EquipCategory::ACCESSORY: NewInfo.equipCategory = EEquipCategoryType::ACCESSORY; break;
	//	}
	//	NewInfo.quality = ThisInfo->quality;
	//	NewInfo.rate = ThisInfo->rate;
	//	NewInfo.qualityIncLevel = ThisInfo->quality_inc_level;

	//	MD_LotteryShopRateQualityInfo.Add(NewInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_QualityTransRateInfo(const FB2MasterDatas& InMasterDatas)
{
	//FB2ExtendMessagePtr	ExtendMessage;
	//ExtendMessage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_extend_massages_index, InMasterDatas);

	//MD_QualityTransRateInfo.Empty();

	//for (const b2network::B2mdQualityTransRateInfoPtr& ThisInfo : ExtendMessage->md_quality_trans_rate_info)
	//{
	//	FQualityTransRateInfo NewInfo;

	//	NewInfo.templateID = ThisInfo->template_id;
	//	NewInfo.equipCategory = ThisInfo->equip_category;
	//	NewInfo.qualityValue = ThisInfo->quality;
	//	NewInfo.transRate = ThisInfo->trans_rate;
	//	NewInfo.transCondQuality = ThisInfo->trans_cond_quality;

	//	MD_QualityTransRateInfo.Add(NewInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_ConvertTableCost(const FB2MasterDatas& InMasterDatas)
{
	//FB2ExtendMessagePtr	ExtendMessage;
	//ExtendMessage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_extend_massages_index, InMasterDatas);

	//MD_ExchangeSystemCost.Empty();

	//for (const b2network::B2mdExchangeSystemCostPtr& ThisInfo : ExtendMessage->md_exchange_system_cost)
	//{
	//	FConvertCostInfo NewInfo;

	//	NewInfo.tableId = ThisInfo->exchange_id;
	//	NewInfo.count = ThisInfo->count;
	//	NewInfo.goldPrice = ThisInfo->gold_cost;
	//	NewInfo.crystalPrice = ThisInfo->gem_cost;

	//	MD_ExchangeSystemCost.Add(NewInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_ItemForgeInfo(const FB2MasterDatas& InMasterDatas)
{
	//FB2ExtendMessagePtr	ExtendMessage;
	//ExtendMessage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_extend_massages_index, InMasterDatas);

	//MD_ItemForgeInfo.Empty();

	//for (const b2network::B2mdFactoryInfoPtr& ThisInfo : ExtendMessage->md_factory_info)
	//{
	//	FItemForgeInfo NewInfo;

	//	NewInfo.forgeID = ThisInfo->id;
	//	switch (ThisInfo->factory_type)
	//	{
	//	case 1: NewInfo.forgeType = EItemForgeType::Forge; break;
	//	case 2: NewInfo.forgeType = EItemForgeType::Upgrade; break;
	//	}
	//	NewInfo.forgeTab = ThisInfo->factory_tab;
	//	NewInfo.groupID = ThisInfo->group_id;
	//	NewInfo.groupSort = ThisInfo->group_sort;
	//	NewInfo.itemSort = ThisInfo->item_sort;
	//	NewInfo.limitID = ThisInfo->limit_id;
	//	switch (ThisInfo->character_type)
	//	{
	//	case 1: NewInfo.PCClassType = EPCClass::EPC_Gladiator; break;
	//	case 2: NewInfo.PCClassType = EPCClass::EPC_Assassin; break;
	//	case 3: NewInfo.PCClassType = EPCClass::EPC_Wizard; break;
	//	case 4: NewInfo.PCClassType = EPCClass::EPC_Fighter; break;
	//	default:NewInfo.PCClassType = EPCClass::EPC_End; break;
	//	}

	//	NewInfo.goldPrice = ThisInfo->gold_cost;
	//	NewInfo.gemPrice = ThisInfo->gem_cost;
	//	NewInfo.darkCrystalPrice = ThisInfo->dark_crystal_cost;

	//	NewInfo.successRate = ThisInfo->success_rate;
	//	NewInfo.hugeSuccessRate = ThisInfo->huge_success_rate;
	//	NewInfo.failRate = ThisInfo->fail_rate;

	//	NewInfo.hugeSuccessAdditionalItemID = ThisInfo->huge_success_add_item_template_id;
	//	NewInfo.hugeSuccessAdditionalItemQuantity = ThisInfo->huge_success_add_item_count;
	//	NewInfo.resultItemID = ThisInfo->result_item_template_id;
	//	NewInfo.resultItemQuantity = ThisInfo->result_item_count;

	//	NewInfo.mainMaterialItemID = ThisInfo->main_material_template_id;
	//	NewInfo.mainMaterialItemEnhance = ThisInfo->main_material_cond_reinforce;
	//	NewInfo.mainMaterialItemLevel = ThisInfo->main_material_cond_level;
	//	NewInfo.mainMaterialItemQuantity = ThisInfo->main_material_count;
	//	NewInfo.mainMaterialItemRefundRate = ThisInfo->main_material_fail_refund_rate;

	//	NewInfo.material1stItemID = ThisInfo->material_template_id1;
	//	NewInfo.material1stItemQuantity = ThisInfo->material_count1;
	//	NewInfo.material1stItemRefundRate = ThisInfo->material_fail_refund_rate1;
	//	NewInfo.material2ndItemID = ThisInfo->material_template_id2;
	//	NewInfo.material2ndItemQuantity = ThisInfo->material_count2;
	//	NewInfo.material2ndItemRefundRate = ThisInfo->material_fail_refund_rate2;
	//	NewInfo.material3rdItemID = ThisInfo->material_template_id3;
	//	NewInfo.material3rdItemQuantity = ThisInfo->material_count3;
	//	NewInfo.material3rdItemRefundRate = ThisInfo->material_fail_refund_rate3;
	//	NewInfo.material4thItemID = ThisInfo->material_template_id4;
	//	NewInfo.material4thItemQuantity = ThisInfo->material_count4;
	//	NewInfo.material4thItemRefundRate = ThisInfo->material_fail_refund_rate4;


	//	switch (ThisInfo->visible_type)
	//	{
	//	case 1: NewInfo.visibleType = EItemForgeListVisibleType::Disabled; break;
	//	case 2: NewInfo.visibleType = EItemForgeListVisibleType::Enabled; break;
	//	case 3: NewInfo.visibleType = EItemForgeListVisibleType::Limited; break;
	//	}

	//	NewInfo.previewOnly = false;

	//	MD_ItemForgeInfo.Add(NewInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_ItemForgeMaterialTransInfo(const FB2MasterDatas& InMasterDatas)
{
	//FB2ExtendMessagePtr	ExtendMessage;
	//ExtendMessage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_extend_massages_index, InMasterDatas);

	//MD_ItemForgeMaterialTransInfo.Empty();

	//for (const b2network::B2mdFactoryMaterialTransInfoPtr& ThisInfo : ExtendMessage->md_factory_material_trans_info)
	//{
	//	FItemForgeMaterialTransInfo NewInfo;

	//	NewInfo.forgeID = ThisInfo->factory_id;
	//	NewInfo.mainMaterialItemID = ThisInfo->trans_main_material_template_id;
	//	NewInfo.mainMaterialItemQuantity = ThisInfo->trans_main_material_count;
	//	NewInfo.material1stItemID = ThisInfo->trans_material_template_id1;
	//	NewInfo.material1stItemQuantity = ThisInfo->trans_material_count1;
	//	NewInfo.material2ndItemID = ThisInfo->trans_material_template_id2;
	//	NewInfo.material2ndItemQuantity = ThisInfo->trans_material_count2;
	//	NewInfo.material3rdItemID = ThisInfo->trans_material_template_id3;
	//	NewInfo.material3rdItemQuantity = ThisInfo->trans_material_count3;
	//	NewInfo.material4thItemID = ThisInfo->trans_material_template_id4;
	//	NewInfo.material4thItemQuantity = ThisInfo->trans_material_count4;

	//	MD_ItemForgeMaterialTransInfo.Add(NewInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_ItemForgeLimitStatus(const FB2MasterDatas& InMasterDatas)
{
	FB2ExtendMessagePtr	ExtendMessage;
	//ExtendMessage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_extend_massages_index, InMasterDatas);

	//MD_ItemForgeLimitInfo.Empty();

	//for (const b2network::B2mdFactoryLimitInfoPtr& ThisInfo : ExtendMessage->md_factory_limit_info)
	//{
	//	FItemForgeLimitInfo NewInfo;

	//	NewInfo.limitID = ThisInfo->id;
	//	NewInfo.limitQuantity = ThisInfo->limit_count;

	//	switch (ThisInfo->limit_type)
	//	{
	//	case 1: NewInfo.limitType = EItemForgeLimitType::NumberOfTry; break;
	//	case 2: NewInfo.limitType = EItemForgeLimitType::NumberOfSuccess; break;
	//	}

	//	switch (ThisInfo->limit_time_type)
	//	{
	//	case 5: NewInfo.limitTimeType = EItemForgePeriodLimitType::None; break;
	//	case 1: NewInfo.limitTimeType = EItemForgePeriodLimitType::Once; break;
	//	case 2: NewInfo.limitTimeType = EItemForgePeriodLimitType::Day; break;
	//	case 3: NewInfo.limitTimeType = EItemForgePeriodLimitType::Week; break;
	//	case 4: NewInfo.limitTimeType = EItemForgePeriodLimitType::Month; break;
	//	}

	//	MD_ItemForgeLimitInfo.Add(NewInfo);
	//}
}

void FClientDataStore::SetMasterDataImpl_SealBoxInfo(const FB2MasterDatas& InMasterDatas)
{
	FB2ExtendMessagePtr	ExtendMessage;
	//ExtendMessage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_extend_massages_index, InMasterDatas);

	//MD_SealBoxInfo.Empty();

	//for (const b2network::B2mdSealBoxInfoPtr& ThisInfo : ExtendMessage->md_seal_box_info)
	//{
	//	FSealBoxInfo NewInfo;

	//	NewInfo.sealboxRefID = ThisInfo->seal_box_template_id;
	//	NewInfo.rewardSlot1 = ThisInfo->reward_slot1;
	//	NewInfo.rewardSlot2 = ThisInfo->reward_slot2;
	//	NewInfo.rewardSlot3 = ThisInfo->reward_slot3;
	//	NewInfo.rewardSlot4 = ThisInfo->reward_slot4;
	//	NewInfo.rewardSlot5 = ThisInfo->reward_slot5;
	//	NewInfo.isRateVisible = ThisInfo->is_rate_visible;

	//	MD_SealBoxInfo.Add(NewInfo);
	//}
}


void FClientDataStore::SetMasterDataImpl_SealBoxRewardRateInfo(const FB2MasterDatas& InMasterDatas)
{
	FB2ExtendMessagePtr	ExtendMessage;
	//ExtendMessage = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_extend_massages_index, InMasterDatas);

	//MD_SealBoxRewardRateInfo.Empty();

	//for (const b2network::B2mdSealBoxRewardRateInfoPtr& ThisInfo : ExtendMessage->md_seal_box_reward_rate_info)
	//{
	//	FSealBoxRewardRateInfo NewInfo;

	//	NewInfo.sealboxRewardID = ThisInfo->seal_box_reward_id;
	//	NewInfo.itemType= ThisInfo->reward_type;
	//	NewInfo.itemRefID = ThisInfo->template_id;
	//	NewInfo.minQuantity = ThisInfo->min_amount;
	//	NewInfo.maxQuantity = ThisInfo->max_amount;

	//	NewInfo.starGrade = ThisInfo->grade;
	//	NewInfo.groupID = ThisInfo->group_id;
	//	NewInfo.equipCategory = ThisInfo->equip_category;
	//	NewInfo.equipPlace = ThisInfo->equip_position;
	//	NewInfo.quality = ThisInfo->quality;
	//	NewInfo.enhanceLevel = ThisInfo->enhance_level;
	//	NewInfo.surpassCount = ThisInfo->surpass_count;

	//	NewInfo.rewardRate = ThisInfo->reward_rate;

	//	MD_SealBoxRewardRateInfo.Add(NewInfo);
	//}
}


bool FClientDataStore::GetFairyOpenInfoAll(TArray<FMD_FairyOpenInfo>& OutList)
{
	OutList.Empty();
	if (MD_FairyOpenInfoData.Num() < 1)
		return false;

	TArray<EFairyType> Keys;
	MD_FairyOpenInfoData.GetKeys(Keys);
	for (const auto Each : Keys)
	{
		FMD_FairyOpenInfo* FoundPtr = MD_FairyOpenInfoData.Find(Each);
		if (FoundPtr)
		{
			OutList.Add(*FoundPtr);
		}
	}
	return OutList.Num() > 0;
}

bool FClientDataStore::GetFairyOpenInfo(EFairyType Target, FMD_FairyOpenInfo& OutData)
{
	FMD_FairyOpenInfo* FoundPtr = MD_FairyOpenInfoData.Find(Target);
	if (FoundPtr)
	{
		OutData = *FoundPtr;
		return true;
	}

	return false;
}

bool FClientDataStore::GetFairyInfoAll(EFairyType Target, TArray<FMD_FairyInfo>& OutList)
{
	OutList.Empty();
	if (MD_FairyInfoData.Num() < 1)
		return false;

	TArray<EFairyType> Keys;
	MD_FairyInfoData.GetKeys(Keys);
	TArray<FMD_FairyInfo>* FoundPtr = MD_FairyInfoData.Find(Target);
	if (FoundPtr)
	{
		for (FMD_FairyInfo& Each : *FoundPtr)
		{
			OutList.Add(Each);
		}
	}
	return OutList.Num() > 0;
}

bool FClientDataStore::GetFairyInfo(EFairyType Target, int32 OptionId, int32 BlessLevel, FMD_FairyInfo& OutData)
{
	TArray<FMD_FairyInfo>* FoundPtr = MD_FairyInfoData.Find(Target);
	if (FoundPtr)
	{
		for (FMD_FairyInfo& Each : *FoundPtr)
		{
			if (Each.OptionId == OptionId && Each.BlessLevel == BlessLevel)
			{
				OutData = Each;
				return true;
			}
		}
	}
	return false;
}

bool FClientDataStore::GetFairyLevelUpCostAll(TArray<FMD_FairyLevelUpCost>& OutList)
{
	OutList.Empty();
	if (MD_FairyLevelUpCostData.Num() < 1)
		return false;

	TArray<int32> Keys;
	MD_FairyLevelUpCostData.GetKeys(Keys);
	for (const auto Each : Keys)
	{
		FMD_FairyLevelUpCost* FoundPtr = MD_FairyLevelUpCostData.Find(Each);
		if (FoundPtr)
		{
			OutList.Add(*FoundPtr);
		}
	}
	return OutList.Num() > 0;
}

bool FClientDataStore::GetFairyLevelUpCost(int32 TargetLevel, FMD_FairyLevelUpCost& OutData)
{
	FMD_FairyLevelUpCost* FoundPtr = MD_FairyLevelUpCostData.Find(TargetLevel);
	if (FoundPtr)
	{
		OutData = *FoundPtr;
		return true;
	}
	return false;
}

void FClientDataStore::GetKakaoFriendInviteRewardInfo(const int32 RewardType, TArray<b2network::B2mdKakaoFriendInviteRewardPtr>& OutList)
{
	if (MD_KakaoFriendInviteRewardInfo.Contains(RewardType) == false)
		return;

	for (auto& KakaoFriendInfo : MD_KakaoFriendInviteRewardInfo[RewardType])
	{
		OutList.Add(KakaoFriendInfo);
	}
}

FString FClientDataStore::GetWebURL(FString URLKey)
{
	FString Result = FString(TEXT(""));

	if (MD_WebURL.Contains(URLKey))
	{
		Result = MD_WebURL[URLKey];
	}

	return Result;
}

FString FClientDataStore::GetWebURL(EUIScene CurScene)
{
	FString Result;
	FString URLKey;

	switch (CurScene)
	{
	case EUIScene::Inventory:
		URLKey = FString(TEXT("GameGuide_EquipmentInfo"));
		break;
	case EUIScene::Chapter:
		URLKey = FString(TEXT("GameGuide_AdventureInfo"));
		break;
	case EUIScene::PVPOneOnOne:
		URLKey = FString(TEXT("GameGuide_PvpMatch"));
		break;
	case EUIScene::TeamMatchUI:
		URLKey = FString(TEXT("GameGuide_TeamMatch"));
		break;
	case EUIScene::ControlMain:
		URLKey = FString(TEXT("GameGuide_TeamControl"));
		break;
	case EUIScene::Skill:
		URLKey = FString(TEXT("GameGuide_HeroSkill"));
		break;
	case EUIScene::Brevet:
		URLKey = FString(TEXT("GameGuide_HeroPromotion"));
		break;
	case EUIScene::Wing:
		URLKey = FString(TEXT("GameGuide_HeroWing"));
		break;
	case EUIScene::Relic:
		URLKey = FString(TEXT("GameGuide_HeroRelic"));
		break;
	case EUIScene::EtherMain:
		URLKey = FString(TEXT("GameGuide_HeroEther"));
		break;
	case EUIScene::CounterAttackDGMain:
		URLKey = FString(TEXT("GameGuide_CounterDungeon"));
		break;
	case EUIScene::HeroTowerReady:
		URLKey = FString(TEXT("GameGuide_HeroTower"));
		break;
	case EUIScene::RaidMain:
		URLKey = FString(TEXT("GameGuide_Raid"));
		break;
	case EUIScene::GuildMain:
		URLKey = FString(TEXT("GameGuide_Guild"));
		break;
	case EUIScene::SummonItemStore:
		URLKey = FString(TEXT("GameGuide_Store"));
		break;
	case EUIScene::Store:
		URLKey = FString(TEXT("GameGuide_Goods"));
		break;
	case EUIScene::Mission:
		URLKey = FString(TEXT("GameGuide_Achievement"));
		break;
	case EUIScene::Friend:
		URLKey = FString(TEXT("GameGuide_Friend"));
		break;
	case EUIScene::RestPointMain:
		URLKey = FString(TEXT("GameGuide_BreaktimeReward"));
		break;
	case EUIScene::CollectBookSelectClass:
		URLKey = FString(TEXT("GameGuide_Guidebook"));
		break;
	case EUIScene::MagicStore:
		URLKey = FString(TEXT("GameGuide_MagicStore"));
		break;
	case EUIScene::HallOfFame:
		URLKey = FString(TEXT("GameGuide_HallofFame"));
		break;
	case EUIScene::DonationEvent:
		URLKey = FString(TEXT("GameGuide_DonationEvent"));
		break;
	case EUIScene::DimensionMain:
		URLKey = FString(TEXT("GameGuide_Dimension"));
		break;
	case EUIScene::Totem:
		URLKey = FString(TEXT("GameGuide_Totem"));
		break;
	case EUIScene::Costume:
		URLKey = FString(TEXT("GameGuide_Costume"));
		break;
	default:
		URLKey = FString(TEXT("GameGuide_List"));
		break;
	}

	if (MD_WebURL.Contains(URLKey))
	{
		Result = MD_WebURL[URLKey];
	}

	return Result;
}

float FClientDataStore::GetItemLevelupFactor_TargetItemGrade(int32 InGrade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemLevelupFactor_TargetItemGrade);
	int32 ClampedGrade = FMath::Clamp(InGrade, 0, ItemLevelupFactorMap.TargetItemGradeToFactor.Num() - 1);
	return ItemLevelupFactorMap.TargetItemGradeToFactor[ClampedGrade];
}

float FClientDataStore::GetItemLevelupFactor_IngredItemGrade(int32 InGrade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemLevelupFactor_IngredItemGrade);
	int32 ClampedGrade = FMath::Clamp(InGrade, 0, ItemLevelupFactorMap.IngredItemGradeToFactor.Num() - 1);
	return ItemLevelupFactorMap.IngredItemGradeToFactor[ClampedGrade];
}

float FClientDataStore::GetItemLevelupFactor_IngredItemEquipPlace(EItemEquipPlace InEquipPlace)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemLevelupFactor_IngredItemEquipPlace);
	int32 EquipPlaceInt = static_cast<int32>(InEquipPlace);
	if (EquipPlaceInt >= 0 && EquipPlaceInt < ItemLevelupFactorMap.IngredItemEquipPosToFactor.Num())
	{
		return ItemLevelupFactorMap.IngredItemEquipPosToFactor[EquipPlaceInt];
	}
	return 0.0f; // 捞扒 裹困 观捞搁 漏绝捞 0.
}

float FClientDataStore::GetItemLevelupFactor_IngredItemEnhLv(int32 InEnhanceLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemLevelupFactor_IngredItemEnhLv);
	int32 ClampedLevel = FMath::Clamp(InEnhanceLevel, 0, ItemLevelupFactorMap.IngredItemLevelupToFactor.Num() - 1);
	return ItemLevelupFactorMap.IngredItemLevelupToFactor[ClampedLevel];
}

float FClientDataStore::GetItemLevelupFactor_IngredItemSurpCount(int32 InSurpassCount)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemLevelupFactor_IngredItemSurpCount);
	int32 ClampedCount = FMath::Clamp(InSurpassCount, 0, ItemLevelupFactorMap.IngredItemSurpassCountToFactor.Num() - 1);
	return ItemLevelupFactorMap.IngredItemSurpassCountToFactor[ClampedCount];
}

float FClientDataStore::GetItemLevelupFactor_PerOptionInc(EItemOption InOption)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemLevelupFactor_PerOptionInc);
	FItemLevelupFactorMap::FitemOptionValue* FoundFactorPtr = ItemLevelupFactorMap.TargetItemOptionIncFactor.Find(InOption);
	return FoundFactorPtr->Add_Value ? FoundFactorPtr->Add_Value : 0.0f;
}

float FClientDataStore::GetOptionWeight(EItemOption InOption)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetOptionWeight);
	FItemLevelupFactorMap::FitemOptionValue* FoundFactorPtr = ItemLevelupFactorMap.TargetItemOptionIncFactor.Find(InOption);
	return FoundFactorPtr ? FoundFactorPtr->Power_Factor : 0.f;
}

int32 FClientDataStore::GetItemLevelupCostSingleFactor(int32 InEnhanceLevel, EItemInvenType InInvenType, int32 InStarGrade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemLevelupCostSingleFactor);
	FItemLevelupCostPerLevel* CostMapForThisLevel = ItemLevelupCostMap.Find(InEnhanceLevel);
	if (CostMapForThisLevel)
	{
		return CostMapForThisLevel->GetCostForGrade(InInvenType, InStarGrade);
	}
	return 0;
}

float FClientDataStore::GetItemLevelupFactor_PrimaryIncrement(int32 ItemLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(GetItemLevelupFactor_PrimaryIncrement);
	const int32 LevelIndex = ItemLevel - 1;

	if (MD_ItemLevelupPrimaryFactor.IsValidIndex(LevelIndex))
	{
		const auto& LevelupFactorInfo = MD_ItemLevelupPrimaryFactor[LevelIndex];
		if (LevelupFactorInfo.ItemLevel == ItemLevel)
			return LevelupFactorInfo.LevelupFactor;
	}

	return 0.f;
}

bool FClientDataStore::GetInventoryExtendReferenceData(int32& OutCashPerSingleExtend, int32& OutSlotCountPerSingleExtend, int32& OutMaxAllowedSlotCount)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetInventoryExtendReferenceData);
	if (ConfigData)
	{
		OutCashPerSingleExtend = ConfigData->need_cash_for_extend_inventory;
		OutSlotCountPerSingleExtend = ConfigData->increase_inventory_slot_per_buy_count;
		OutMaxAllowedSlotCount = ConfigData->max_inventory_slot_count;
		return true;
	}
	return false;
}

int32 FClientDataStore::GetEnhanceItem_NextStepRate(int32 InEnhanceLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetEnhanceItem_NextStepRate);
	int32 NextEnhanceLevel = InEnhanceLevel + 1;
	if (MD_EnhanceItemCostData.Contains(NextEnhanceLevel) == false)
		return 0;

	return MD_EnhanceItemCostData[NextEnhanceLevel].Rate;
}

int32 FClientDataStore::GetEnhanceItem_NextStepScrollCost(int32 InEnhanceLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetEnhanceItem_NextStepScrollCost);
	int32 NextEnhanceLevel = InEnhanceLevel + 1;
	if (MD_EnhanceItemCostData.Contains(NextEnhanceLevel) == false)
		return 0;

	return MD_EnhanceItemCostData[NextEnhanceLevel].ScrollCost;
}

int32 FClientDataStore::GetEnhanceItem_NextStepProtectionScrollCost(int32 InEnhanceLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetEnhanceItem_NextStepProtectionScrollCost);
	int32 NextEnhanceLevel = InEnhanceLevel + 1;
	if (MD_EnhanceItemCostData.Contains(NextEnhanceLevel) == false)
		return 0;

	return MD_EnhanceItemCostData[NextEnhanceLevel].ProtectionScrollCost;
}

int32 FClientDataStore::GetEnhanceItem_NextStepMoneyCost(int32 InEnhanceLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetEnhanceItem_NextStepMoneyCost);
	int32 NextEnhanceLevel = InEnhanceLevel + 1;
	if (MD_EnhanceItemCostData.Contains(NextEnhanceLevel) == false)
		return 0;

	return MD_EnhanceItemCostData[NextEnhanceLevel].MoneyCost;
}

bool FClientDataStore::HasEnhanceEffectAuraColor(int32 InEnhanceLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_HasEnhanceEffectAuraColor);
	return GetEnhanceEffectAuraColor(InEnhanceLevel) != FLinearColor::Transparent;
}

FLinearColor FClientDataStore::GetEnhanceEffectAuraColor(int32 InEnhanceLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetEnhanceEffectAuraColor);
	if (MD_EnhanceItemEffectData.Contains(InEnhanceLevel) == false)
		return FLinearColor::Transparent;

	return MD_EnhanceItemEffectData[InEnhanceLevel].AuraColor;
}

int32 FClientDataStore::GetMinEnhanceLevelForSetEffect()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMinEnhanceLevelForSetEffect);
	// 技飘瓤苞啊 啊厘 贸澜 利侩登绰 矫痢(碍拳饭骇) 馆券茄促
	return MD_MinEnhanceLevelForSetEffect;
}

int32 FClientDataStore::GetMaxEnhanceLevelForSetEffect()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMaxEnhanceLevelForSetEffect);

	return MD_MaxEnhanceLevelForSetEffect;
}

double FClientDataStore::GetEnhanceItemSetEffect(int32 InEnhanceLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetEnhanceItemSetEffect);
	if (MD_EnhanceItemEffectData.Num() == 0)
		return 0.f;

	if (MD_EnhanceItemEffectData.Contains(InEnhanceLevel) == false)
		return 0.f;

	return MD_EnhanceItemEffectData[InEnhanceLevel].SetEffect;
}

int32 FClientDataStore::GetResurrectionCost(EResurrectGameModeType ModeType, int32 ResurrectCount)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetResurrectionCost);
	if (ResurrectMasterData.Contains(ModeType) == false)
		return 0;

	FModeResurrectMasterDataElement MasterDataEle = ResurrectMasterData[ModeType];
	if (MasterDataEle.ResurrectElementList.Contains(ResurrectCount) == false)
		return 0;

	return MasterDataEle.ResurrectElementList[ResurrectCount].price;
}

void FClientDataStore::GetAppliedResurrectBuff(EResurrectGameModeType ModeType, const TMap<EResurrectBuffType, int32>& InSelectedBuffCounts, TMap<EResurrectBuffType, int32>& OutAppliedBuffValues, bool bGetNextExpectedValues)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetAppliedResurrectBuff);
	for (auto& ThisBuffCountSet : InSelectedBuffCounts)
	{
		if (ThisBuffCountSet.Key == EResurrectBuffType::RBT_None || ThisBuffCountSet.Key == EResurrectBuffType::RBT_End) {
			continue;
		}

		// 滚橇 冉荐甫 虐肺 秦辑 付胶磐 单捞磐俊辑 蔼阑 啊廉咳
		// 促澜 滚橇 急琶 矫 抗惑 荐摹扼搁 窍唱 歹秦辑. 捞固 弥措 荐摹俊 档崔茄 惑怕扼搁 酒公巴档 唱坷瘤 臼阑 巴.
		int32 ResurrectCount = (bGetNextExpectedValues ? ThisBuffCountSet.Value + 1 : ThisBuffCountSet.Value);
		if (ResurrectMasterData.Contains(ModeType) && ResurrectMasterData[ModeType].ResurrectElementList.Contains(ResurrectCount))
		{
			FResurrectMasterDataElement ResurrectData = ResurrectMasterData[ModeType].ResurrectElementList[ResurrectCount];

			int32 ThisValue = ResurrectData.GetBuffValueOfType(ThisBuffCountSet.Key);
			if (ThisValue > 0)
				OutAppliedBuffValues.Add(ThisBuffCountSet.Key, ThisValue);
		}
	}
}

FString FClientDataStore::GetBrevetRankNameKey(int32 BrevetRank)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBrevetRankNameKey);
	//UB2BrevetInfo* Info = StaticFindBrevetInfo();

	//if (Info)
	//{
	//	return Info->GetBrevetRankNameKey(BrevetRank);
	//}

	return TEXT("");
}

UMaterialInterface* FClientDataStore::GetBrevetRankIconMaterial(int32 BrevetRank, bool isBigSize)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBrevetRankIconMaterial);
	//UB2BrevetInfo* Info = StaticFindBrevetInfo();

	//if (Info)
	//{
	//	return Info->GetBrevetRankIconMaterial(BrevetRank, isBigSize);
	//}

	return 0;
}
int32 FClientDataStore::GetBrevetGoodsID(int32 BrevetRank)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetBrevetGoodsIconMaterial);

	bool checkNormal = (BrevetRank <= GetMaximumNormalBrevetGade());
	
	if (checkNormal)	return FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE;
	else				return FItemRefIDHelper::ITEM_REF_ID_ADVANCED_BREVET_STONE;
}

FString FClientDataStore::GetRewardNameKey(int32 RewardIndex)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRewardNameKey);
	//UB2RewardInfo* Info = StaticFindRewardInfo();

	//if (Info)
	//{
	//	return Info->GetRewardNameKey(RewardIndex);
	//}

	return TEXT("");
}

UMaterialInterface* FClientDataStore::GetRewardIconMaterial(int32 RewardIndex)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRewardIconMaterial);
	//UB2RewardInfo* Info = StaticFindRewardInfo();

	//if (Info)
	//{
	//	return Info->GetRewardIconMaterial(RewardIndex);
	//}

	return 0;
}

const FWingMD_EnhanceSingleElem* FClientDataStore::GetWingMD_Enhance(int32 InWingGrade, int32 InWingEnhanceLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetWingMD_Enhance);
	return WingMD_Enhance.Find(FWingMD_EnhanceSingleElem::GetKey(InWingGrade, InWingEnhanceLevel));
}
const FWingMD_EvolveSingleElem* FClientDataStore::GetWingMD_Evolve(int32 InWingGrade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetWingMD_Evolve);
	return WingMD_Evolve.Find(FWingMD_EvolveSingleElem::GetKey(InWingGrade));
}
const FWingMD_PropOptionSingleElem* FClientDataStore::GetWingMD_PropOption(EPCClass InPCClass, int32 InOptionIndex, int32 InOptionLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetWingMD_PropOption);
	return WingMD_PropOption.Find(FWingMD_PropOptionSingleElem::GetKey(InPCClass, InOptionIndex, InOptionLevel));
}
const FWingMD_PropOptionSingleElem* FClientDataStore::GetWingMD_PropOption(const FB2Wing& InWing, int32 InOptionIndex)
{
	const FWingPropOption* FoundOptionOfIndex = InWing.GetPropOptionPtr(InOptionIndex);
	if (FoundOptionOfIndex)
	{
		return GetWingMD_PropOption(IntToPCClass(InWing.OwnerPCClass), InOptionIndex, FoundOptionOfIndex->OptionLevel);
	}
	return NULL;
}
const FWingMD_PropOptionOpenSingleElem* FClientDataStore::GetWingMD_PropOptionOpenCondition(EPCClass InPCClass, int32 InOptionIndex)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetWingMD_PropOptionOpenCondition);
	return WingMD_PropOptionOpenCondition.Find(FWingMD_PropOptionOpenSingleElem::GetKey(InPCClass, InOptionIndex));
}
const FWingMD_PropOptionMaxLevelSingleElem* FClientDataStore::GetWingMD_PropOptionMaxLevel(EPCClass InPCClass, int32 InOptionIndex, int32 InWingGrade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetWingMD_PropOptionMaxLevel);
	return WingMD_PropOptionMaxLevel.Find(FWingMD_PropOptionMaxLevelSingleElem::GetKey(InPCClass, InOptionIndex, InWingGrade));
}

bool FClientDataStore::GetWingEvolveRefData(int32 InTargetGrade, int32& OutRequiredItemCount, int32& OutRequiredGoldCost, float& OutSuccessRate, int32& OutMaxFailPoint)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetWingEvolveRefData);
	const FWingMD_EvolveSingleElem* WantedMD = GetWingMD_Evolve(InTargetGrade);
	if (WantedMD)
	{
		// RequiredItemRefID 绰 老窜 捞率 define 阑 静绊.. FItemRefIDHelper::GetWingEvolveResourceRefID()
		OutRequiredItemCount = WantedMD->RequiredItemCount;
		OutRequiredGoldCost = WantedMD->RequiredGoldCost;
		OutSuccessRate = WantedMD->SuccessRate;
		OutMaxFailPoint = WantedMD->MaxFailPoint;
		return true;
	}
	return false;
}
bool FClientDataStore::GetWingEvolveRefData(const FB2Wing& InCurrentWing, int32& OutRequiredItemCount, int32& OutRequiredGoldCost, float& OutSuccessRate, int32& OutMaxFailPoint)
{
	// InCurrentWing 俊辑 促澜 grade 肺狼 曼绊 单捞磐啊 凳. 弥措 荐摹俊 档崔沁栏搁 false 府畔且 巴.
	return GetWingEvolveRefData(InCurrentWing.EvolutionGrade, OutRequiredItemCount, OutRequiredGoldCost, OutSuccessRate, OutMaxFailPoint);
}
bool FClientDataStore::GetWingEnhanceRefData(int32 InTargetGrade, int32 InTargetEnhanceLevel, int32& OutRequiredItemCount, int32& OutRequiredGoldCost, int32& OutEnhancePointToAcquire, int32& OutRequiredEnhancePoint)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetWingEnhanceRefData);
	const FWingMD_EnhanceSingleElem* WantedMD = GetWingMD_Enhance(InTargetGrade, InTargetEnhanceLevel);
	if (WantedMD)
	{
		// RequiredItemRefID 绰 老窜 捞率 define 阑 静绊.. FItemRefIDHelper::GetWingEnhanceResourceRefID()
		OutRequiredItemCount = WantedMD->RequiredItemCount;
		OutRequiredGoldCost = WantedMD->RequiredGoldCost;
		OutEnhancePointToAcquire = WantedMD->EnhancePointToAcquirePerTrial;
		OutRequiredEnhancePoint = WantedMD->RequiredEnhancePoint;
		return true;
	}
	return false;
}
bool FClientDataStore::GetWingEnhanceRefData(const FB2Wing& InCurrentWing, int32& OutRequiredItemCount, int32& OutRequiredGoldCost, int32& OutEnhancePointToAcquire, int32& OutRequiredEnhancePoint)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetWingEnhanceRefData);
	// EnhanceLevel 父 促澜 饭骇肺 啊绰 巴俊 措茄 曼绊 单捞磐
	return GetWingEnhanceRefData(InCurrentWing.EvolutionGrade, InCurrentWing.EnhanceLevel, OutRequiredItemCount, OutRequiredGoldCost, OutEnhancePointToAcquire, OutRequiredEnhancePoint);
}
bool FClientDataStore::GetWingPropOptionRefData(EPCClass InPCClass, int32 InOptionIndex, int32 InTargetOptionLevel, int32& OutRequiredItemCount)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetWingPropOptionRefData);
	const FWingMD_PropOptionSingleElem* WantedMD = GetWingMD_PropOption(InPCClass, InOptionIndex, InTargetOptionLevel);
	if (WantedMD)
	{
		// RequiredItemRefID 绰 老窜 捞率 define 阑 静绊.. ITEM_REF_ID_WING_PROP_OPTION_ENHANCE_INGREDIENT
		OutRequiredItemCount = WantedMD->RequiredItemCount;
		return true;
	}
	return false;
}
bool FClientDataStore::GetWingPropOptionRefData(const FB2Wing& InCurrentWing, int32 InOptionIndex, int32& OutRequiredItemCount)
{
	const FWingPropOption* PropOptionOfIndex = InCurrentWing.GetPropOptionPtr(InOptionIndex);
	if (PropOptionOfIndex)
	{ // 秦寸 可记狼 促澜 饭骇肺狼 曼绊 单捞磐.
		return GetWingPropOptionRefData(IntToPCClass(InCurrentWing.OwnerPCClass), InOptionIndex, PropOptionOfIndex->OptionLevel, OutRequiredItemCount);
	}
	return false;
}

int32 FClientDataStore::GetMaxDailyTryCountHeroTower()
{
	// colosseum comment
	//return MD_HeroTowerRetryCost.Num();
	return 0;
}

int32 FClientDataStore::GetMaxDailyFreeTryCountHeroTower()
{
	// 酒贰 抛捞喉捞 力芭等促窃.. 沥焊甫 啊廉棵单啊 绝澜
	return 3;

	// 	int32 FreeTryCount = 0;
	// 
	// 	const int32 MaxTryCount = GetMaxDailyTryCountHeroTower();
	// 	for (int32 TryCount = 1; TryCount <= MaxTryCount; TryCount++)
	// 	{
	// 		if (GetHeroTowerRetryCost(TryCount) == 0)
	// 			FreeTryCount++;
	// 		else
	// 			return FreeTryCount;
	// 	}
	// 
	// 	return 0;
}

int32 FClientDataStore::GetHeroTowerRetryCost(int32 TryCount)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetHeroTowerRetryCost);
	// colosseum comment
	//FMD_HeroTowerRetryCostSingleElem* HeroTowerRetryCostSingleElem = MD_HeroTowerRetryCost.Find(TryCount);
	//return HeroTowerRetryCostSingleElem->nGemCost;
	int32 HERO_TOWER_FREE_TRY_COUNT = 3;
	return TryCount * 10 * (TryCount - HERO_TOWER_FREE_TRY_COUNT);
}

int32 FClientDataStore::GetHeroTowerMapId(int32 nFloor)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetHeroTowerMapId);
	FMD_HeroTowerFloorSingleElem* HeroTowerFloorSingleElem = MD_HeroTowerFloor.Find(nFloor);

	if (HeroTowerFloorSingleElem)
		return HeroTowerFloorSingleElem->nMapId;

	return 0;
}

float FClientDataStore::GetHeroTowerLevel(int32 nFloor)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetHeroTowerLevel);
	FMD_HeroTowerFloorSingleElem* HeroTowerFloorSingleElem = MD_HeroTowerFloor.Find(nFloor);

	if (HeroTowerFloorSingleElem)
		return HeroTowerFloorSingleElem->nDifficultyScale / 100.0f;

	return 0.0f;
}

int32 FClientDataStore::GetHeroTowerLimitedTime(int32 nFloor)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetHeroTowerLimitedTime);
	FMD_HeroTowerFloorSingleElem* HeroTowerFloorSingleElem = MD_HeroTowerFloor.Find(nFloor);

	if (HeroTowerFloorSingleElem)
		return HeroTowerFloorSingleElem->nClearLimitTime;

	return 0;
}


int32 FClientDataStore::GetHeroTowerRecommendedCombat(int32 nFloor)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetHeroTowerRecommendedCombat);
	FMD_HeroTowerFloorSingleElem* HeroTowerFloorSingleElem = MD_HeroTowerFloor.Find(nFloor);

	if (HeroTowerFloorSingleElem)
		return HeroTowerFloorSingleElem->nRecommendCombat;

	return 0;
}

void FClientDataStore::GetHeroTowerRecommendAbility(int32 nFloor, int32& OutRecommendAttack, int32& OutRecommendDefense)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetHeroTowerRecommendedCombat);
	FMD_HeroTowerFloorSingleElem* HeroTowerFloorSingleElem = MD_HeroTowerFloor.Find(nFloor);

	if (HeroTowerFloorSingleElem)
	{
		OutRecommendAttack = HeroTowerFloorSingleElem->RecommendAttack;
		OutRecommendDefense = HeroTowerFloorSingleElem->RecommendDefense;
	}
}

TArray<FRewardItem>* FClientDataStore::GetHeroTowerRewards(int32 nFloor, bool IsSweep)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetHeroTowerRewards);
	if (auto* HeroTowerFloorSingleElem = MD_HeroTowerFloor.Find(nFloor))
	{
		return IsSweep ? &HeroTowerFloorSingleElem->rewards : &HeroTowerFloorSingleElem->first_rewards;
	}
	return nullptr;
}

int32 FClientDataStore::GetHeroTowerMaxFloor()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetHeroTowerMaxFloor);
	int32 nDebugNum = MD_HeroTowerFloor.Num();
	return nDebugNum;
}

int32 FClientDataStore::GetHeroTowerSweepRewardCountPerFloor()
{
	return 1;

	// TODO
	//return ConfigData->????
}

int32 FClientDataStore::GetItemLevelUpCost(const EItemInvenType InItemType, const int32 InGrade, const int32 InLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemLevelUpCost);
	FItemLevelupCostPerLevel* FoundCostPerLevel = ItemLevelupCostMap.Find(InLevel);
	if (FoundCostPerLevel == nullptr)
		return 0;

	return FoundCostPerLevel->GetCostForGrade(InItemType, InGrade);
}

const FCostumeEnhanceCostInfo* FClientDataStore::GetCostumeEnhanceCost(int32 TargetGrade, int32 TargetLevel)
{
	return MD_CustumeEnhanceCostInfo.Find(FCostumeEnhanceCostInfo::GetKey(TargetGrade, TargetLevel));
}

const bool FClientDataStore::GetCurrentModeOpenTimes(TArray<b2network::B2MatchOpenPeriodInfoPtr> AllTimeData, TArray<b2network::B2MatchOpenPeriodInfoPtr>& OutTimeData)
{
	FDateTime DateNow = UB2GameInstance::GetKoreaTimeNow();
	int32 nCurrentDayofweek = UB2GameInstance::ConvertToSvrDayOfWeek();

	OutTimeData.Empty();
	for (auto TimeInfo : AllTimeData)
	{
		if (TimeInfo->day_of_week != nCurrentDayofweek)  // 鞍篮 夸老父 拌魂秦具窃
			continue;

		OutTimeData.Add(TimeInfo);
	}

	for (auto TimeInfo : OutTimeData)
	{
		int32 TimeOpen = (TimeInfo->begin_hour * 60) + TimeInfo->begin_min;
		int32 TimeClose = (TimeInfo->end_hour * 60) + TimeInfo->end_min;
		int32 nTestHour = DateNow.GetHour();
		int32 TimeNow = (nTestHour * 60) + DateNow.GetMinute();

		if (FMath::IsWithin(TimeNow, TimeOpen, TimeClose))
		{
			return true;
		}
	}

	return false;
}

const bool FClientDataStore::GetRaidModeOpenTimes(TArray<b2network::B2MatchOpenPeriodInfoPtr> AllTimeData, int32 RaidType, TArray<b2network::B2MatchOpenPeriodInfoPtr>& OutTimeData)
{
//	FDateTime DateNow = UB2GameInstance::GetKoreaTimeNow();
//	int32 nCurrentDayofweek = UB2GameInstance::ConvertToSvrDayOfWeek();
//
//	OutTimeData.Empty();
//
//	for (auto TimeInfo : AllTimeData)
//	{
//		if (TimeInfo->raid_type != RaidType) // 鞍篮 饭捞靛父 厚背.
//			continue;
//		if (TimeInfo->day_of_week != nCurrentDayofweek)  // 鞍篮 夸老父 拌魂秦具窃
//			continue;
//
//		OutTimeData.Add(TimeInfo);
//	}
//
//	for (auto TimeInfo : OutTimeData)
//	{
//		int32 TimeOpen = (TimeInfo->begin_hour * 60) + TimeInfo->begin_min;
//		int32 TimeClose = (TimeInfo->end_hour * 60) + TimeInfo->end_min;
//		int32 nTestHour = DateNow.GetHour();
//		int32 TimeNow = (nTestHour * 60) + DateNow.GetMinute();
//
//		if (FMath::IsWithin(TimeNow, TimeOpen, TimeClose))
//		{
//			return true;
//		}
//	}

	return false;
}

const bool FClientDataStore::GetIsRaidBossOpen(TArray<b2network::B2MatchOpenPeriodInfoPtr> AllTimeData, int32 RaidType, TArray<int32>& OpenDayData)
{
	OpenDayData.Empty();

	int32 nCurrentDayofweek = UB2GameInstance::ConvertToSvrDayOfWeek();
	bool bIsOpen = false;

	for (auto TimeInfo : AllTimeData)
	{
		if (TimeInfo->raid_type != RaidType) // 鞍篮 饭捞靛父 厚背.
			continue;

		if (!OpenDayData.Contains(TimeInfo->day_of_week))
			OpenDayData.Add(TimeInfo->day_of_week); // 坷锹登绰 朝楼.
	}

	for (auto DayData : OpenDayData)
	{
		if (nCurrentDayofweek == DayData)
		{
			bIsOpen = true;
			break;
		}
	}

	return bIsOpen;
}

void FClientDataStore::ResponseGetQuestReward(const TArray<b2network::B2RewardPtr>& RewardInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseGetQuestReward);
	//UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();

	//UB2UIManager* pUIManager = UB2UIManager::GetInstance();
	//UB2UIHeader* pUI = NULL;

	//if (pUIManager)
	//{
	//	pUI = pUIManager->GetUI<UB2UIHeader>(UIFName::Header);
	//}
	//for (const auto& Elment : RewardInfo)
	//{
	//	ERewardType RewardType = BladeIIGameImpl::GetClientDataStore().GetRewardType(Elment->raw_reward->id);

	//	switch (RewardType)
	//	{
	//	case ERewardType::Gold:
	//		AddGold(Elment->raw_reward->count);
	//		if (DocUser) DocUser->SetGold(GetGoldAmount());
	//		if (pUI) pUI->ApplyAddAmountAction(ERewardType::Gold);
	//		break;
	//	case ERewardType::Gem:
	//		AddGem(Elment->raw_reward->count);
	//		if (DocUser) DocUser->SetGem(GetGemAmount());
	//		if (pUI) pUI->ApplyAddAmountAction(ERewardType::Gem);
	//		break;
	//	case ERewardType::BladePoint:
	//		AddBladePoint(Elment->raw_reward->count);
	//		if (DocUser) DocUser->SetStaminaCur(GetBladeCurPoint());
	//		if (pUI) pUI->ApplyAddAmountAction(ERewardType::BladePoint);
	//		break;
	//	case ERewardType::Rank_Promotion_Point:
	//		AddRankPromotionPoint(Elment->raw_reward->count);
	//		if (DocUser) DocUser->SetRankPromotionPoint(GetRankPromotionPoint());
	//		if (pUI) pUI->ApplyAddAmountAction(ERewardType::Rank_Promotion_Point);
	//		break;
	//	case ERewardType::Fame_Point:
	//		AddFamePoint(Elment->raw_reward->count);
	//		if (DocUser) DocUser->SetFamePoint(GetFamePoint());
	//		if (pUI) pUI->ApplyAddAmountAction(ERewardType::Fame_Point);
	//		break;
	//	case ERewardType::Item:
	//	{
	//		for (auto& CharItemCon : GetLocalCharacterData().GetUserAllItems().CharacterItemContainers)
	//		{
	//			CharItemCon.MakeDirty();
	//		}
	//	}
	//	break;
	//	}
	//}
}

//void FClientDataStore::OnReceiveEnhanceWing(const FB2EnhanceWing& EnhanceWingData)
//{ // 扁鸥 览翠 贸府绰 UB2Airport::OnReceiveEnhanceWing 俊辑
//	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_OnReceiveEnhanceWing);
//	ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseEnhanceWing::current_money_index, EnhanceWingData));
//
//	OnResponseConsumableAmountDecrease(FItemRefIDHelper::GetWingEnhanceResourceRefID(),
//		GET_TUPLE_DATA(FB2ResponseEnhanceWing::material_item_count_index, EnhanceWingData),
//		GET_TUPLE_DATA(FB2ResponseEnhanceWing::changed_material_items_index, EnhanceWingData),
//		GET_TUPLE_DATA(FB2ResponseEnhanceWing::deleted_item_ids_index, EnhanceWingData)
//	);
//}
//void FClientDataStore::OnReceiveUpgradeWing(const FB2UpgradeWing& UpgradeWingData)
//{ // 扁鸥 览翠 贸府绰 UB2Airport::OnReceiveUpgradeWing 俊辑
//	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_OnReceiveUpgradeWing);
//	ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseUpgradeWing::current_money_index, UpgradeWingData));
//
//	OnResponseConsumableAmountDecrease(FItemRefIDHelper::GetWingEvolveResourceRefID(),
//		GET_TUPLE_DATA(FB2ResponseUpgradeWing::material_item_count_index, UpgradeWingData),
//		GET_TUPLE_DATA(FB2ResponseUpgradeWing::changed_material_items_index, UpgradeWingData),
//		GET_TUPLE_DATA(FB2ResponseUpgradeWing::deleted_item_ids_index, UpgradeWingData)
//	);
//}
//void FClientDataStore::OnReceiveEnhanceWingOption(const FB2EnhanceWingOption& EnhanceWingOptionData)
//{// 扁鸥 览翠 贸府绰 UB2Airport::OnReceiveEnhanceWingOption 俊辑
//	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_OnReceiveEnhanceWingOption);
//	OnResponseConsumableAmountDecrease(FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID(),
//		GET_TUPLE_DATA(FB2ResponseEnhanceWingOption::material_item_count_index, EnhanceWingOptionData),
//		GET_TUPLE_DATA(FB2ResponseEnhanceWingOption::changed_material_items_index, EnhanceWingOptionData),
//		GET_TUPLE_DATA(FB2ResponseEnhanceWingOption::deleted_item_ids_index, EnhanceWingOptionData)
//	);
//}

// ConsumedAmount 绰 抗傈 内靛狼 儡犁牢单.. 橇肺配妮 惑俊辑 迭洒 绝栏搁 傲 付捞呈胶 蔼栏肺 逞败临 巴.
void FClientDataStore::OnResponseConsumableAmountDecrease(int32 InConsumableRefID, int32 ConsumedAmount, const TArray<b2network::B2ItemServerInfoPtr>& ChangedConsumableData, const TArray<int64>& DeletedConsumableIDs)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_OnResponseConsumableAmountDecrease);
#if !UE_BUILD_SHIPPING
	const int32 AmountBefore = UB2LobbyInventory::GetSharedConsumableAmountOfType(InConsumableRefID); // 捞繁 牢磐其捞胶绰 FPCItemContainer 率俊档 付访秦 初栏搁.. 亮阑扼唱?
#endif

																									  // GetB2ItemArray 概农肺 曼绊
	TArray<FB2Item>	B2ItemArray;
	TU::CopyVectorHelper<FB2Item, b2network::B2ItemServerInfoPtr> SvrToCliItemConvertHelper(&B2ItemArray);
	SvrToCliItemConvertHelper = ChangedConsumableData;
	for (const FB2Item& ThisChangeItem : B2ItemArray)
	{
		LocalCharacterData.UserAllItems.ChangeItem(ThisChangeItem);
	}

#if !UE_BUILD_SHIPPING
	// 酒付档..? 捞霸 嘎瘤 臼绰 芭扼搁 购啊 咯扁 甸绢坷绰 蔼阑 肋 捞秦 给茄 芭老 淀.
	const int32 AmountAfter = UB2LobbyInventory::GetSharedConsumableAmountOfType(InConsumableRefID);
	// 朝俺 磊悼 碍拳俊辑 犁丰啊 阜魄俊 冻绢脸阑 锭 咯扁俊 吧府绰单.. 迭洒 弊 促澜 搬苞啊 肋给登瘤绰 臼绊..
	ensure(ConsumedAmount <= 0 || AmountAfter + ConsumedAmount == AmountBefore); // ConsumedAmount 甫 迭洒 逞败林瘤 给且 锭俊绰 傲 菩胶..
#endif

	LocalCharacterData.UserAllItems.RemoveItems(DeletedConsumableIDs);
}

void FClientDataStore::OnResponseConsumableAmountIncrease(int32 InConsumableRefID, const b2network::B2ItemServerInfoPtr& ChangedConsumableData)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_OnResponseConsumableAmountIncrease);
	TArray<FB2Item>	B2HaveItemArray;
	UB2LobbyInventory::GetSharedConsumableOfType(B2HaveItemArray, InConsumableRefID);

	FB2Item NewConsumableData;
	NewConsumableData = ChangedConsumableData;

	if (B2HaveItemArray.Num() == 0)
	{
		LocalCharacterData.UserAllItems.AddItem(NewConsumableData);
	}
	else
	{
		LocalCharacterData.UserAllItems.ChangeItem(NewConsumableData);
	}
}

void FLocalCharacterData::GetRegistredTargetSlotSkills(EPCClass InCharacterClass, EItemPresetSlotType InSlotType, TArray<int32>& OutList) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetRegistredTargetSlotSkills);
	OutList.Empty();

	if (PlayerSharedStats.IsValidIndex(PCClassToInt(InCharacterClass)))
	{
		auto CharInfo = PlayerSharedStats[PCClassToInt(InCharacterClass)].BasicInfo;

		EItemPresetSlotType RealSlotType = GetPCClassSkillSlotID(InCharacterClass);
		if (InSlotType != EItemPresetSlotType::EIPS_End)
			RealSlotType = InSlotType;
		const int32 SkillSlotID = static_cast<int32>(RealSlotType);

		if (CharInfo && CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID))
		{
			auto SkillInfo = CharInfo->skill_quick_slots[SkillSlotID];
			if (SkillInfo)
			{
				OutList.Add(SkillInfo->using_skill_id_1);

				// 吝汗等 ID 啊 甸绢客辑 粱 眉农.. 单捞磐啊 肋给等 芭变 茄单.
				OutList.Add((SkillInfo->using_skill_id_1 != SkillInfo->using_skill_id_2) ? SkillInfo->using_skill_id_2 : SKILL_INVALID_ID);

				OutList.Add(
					(SkillInfo->using_skill_id_1 != SkillInfo->using_skill_id_3 && SkillInfo->using_skill_id_2 != SkillInfo->using_skill_id_3) ?
					SkillInfo->using_skill_id_3 : SKILL_INVALID_ID
				);
			}
		}
	}
}

void FLocalCharacterData::GetUnregistredTargetSlotSkills(EPCClass InCharacterClass, EItemPresetSlotType InSlotType, TArray<int32>& OutList) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetUnregistredTargetSlotSkills);
	OutList.Empty();

	if (auto CharInfo = GetPlayerStatBasicInfo(InCharacterClass))
	{
		const int32 SkillSlotID = static_cast<int32>(InSlotType);

		if (CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID))
		{
			auto SkillInfo = CharInfo->skill_quick_slots[SkillSlotID];

			for (auto skill : CharInfo->skills)
			{
				auto* SkillMsterInfo = BladeIIGameImpl::GetClientDataStore().GetSkillMasterData(skill->skill_id);
				if (SkillMsterInfo && SkillMsterInfo->bActiveSkill && CharInfo->level >= SkillMsterInfo->LearnLevel)
				{
					if (skill->skill_id != SkillInfo->using_skill_id_1 && skill->skill_id != SkillInfo->using_skill_id_2 && skill->skill_id != SkillInfo->using_skill_id_3)
						OutList.Add(skill->skill_id);
				}
			}
		}
	}
}


#include "B2PCClassInfoBox.h"
#include "B2SkillInfo.h"

void FClientDataStore::RequestSetNickName(FString NickName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_RequestSetNickName);
	RequestedUserNickName = NickName;
	data_trader::Retailer::GetInstance().RequestSetNickName(NickName);
}

void FClientDataStore::ResponseSetNickName()
{ // 捞傈俊 RequestSetNickName 阑 茄 惑炔俊辑 客具..
  /*if (AccountInfo)
  {
  AccountInfo->name = RequestedUserNickName;
  }*/
}

void FClientDataStore::RequestLevelUpSkill(int32 SkillId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_RequestLevelUpSkill);
	auto* ClassInfoBox = StaticFindPCClassInfoBox();
	auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;
	check(AllSkillInfo);
	auto* SkillInfo = AllSkillInfo ? AllSkillInfo->GetSingleInfoOfID(SkillId) : nullptr;

	if (SkillInfo)
	{
		// TODO skill quick slot. preset 篮 唱吝备泅
		data_trader::Retailer::GetInstance().RequestLevelUpSkill(CliToSvrPCClassType(SkillInfo->UserClass), SkillId);
	}
}

void FClientDataStore::ResponseLevelUpSkill(const FB2LevelUpSkillInfo& LevelUpSkillInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseLevelUpSkill);
	//ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseLevelUpSkill::current_gold_index, LevelUpSkillInfo));

	//// TODO skill quick slot
	//int32 CharType = GET_TUPLE_DATA(FB2ResponseLevelUpSkill::character_type_index, LevelUpSkillInfo);
	//EPCClass InPCClass = SvrToCliPCClassType(CharType);
	//EItemPresetSlotType SlotType = LocalCharacterData.GetPCClassSkillSlotID(InPCClass);
	//int32 SkillSlotID = static_cast<int32>(SlotType);

	//if (auto SkillInfo = LocalCharacterData.GetPlayerStatBasicSkillInfo(InPCClass, SkillSlotID))
	//{
	//	int32 nLeftSkillPoint = GET_TUPLE_DATA(FB2ResponseLevelUpSkill::left_skill_point_index, LevelUpSkillInfo);
	//	LocalCharacterData.SetCharacterLeftSkillPoint(InPCClass, nLeftSkillPoint);

	//	auto LevelUpSkill = GET_TUPLE_DATA(FB2ResponseLevelUpSkill::skill_index, LevelUpSkillInfo);
	//	float SkillPower = GetSkillPower(LevelUpSkill->skill_id, LevelUpSkill->skill_level);
	//	int32 EquipSkillId = 0;

	//	//////////////////////////////////////////////////////////////////
	//	//咀记 瘤钎 肺弊 (某腐磐 胶懦 碍拳)
	//	B2ActionLogSender Sender;
	//	Sender.setCategory(FString("CHARACTER"))
	//		.setAction(FString("SKILL_ENHANCE"))
	//		.setLabel(B2ActionLogType<EPCClass>::GetName(InPCClass))
	//		.setActionAttr1(GetSkillLocalizedTextKey(ESkillTextDataType::SkillName, LevelUpSkill->skill_id))
	//		.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	//	//////////////////////////////////////////////////////////////////

	//	if (LevelUpSkill->skill_id == SkillInfo->using_skill_id_1)
	//		EquipSkillId = SkillInfo->using_skill_id_1;
	//	else if (LevelUpSkill->skill_id == SkillInfo->using_skill_id_2)
	//		EquipSkillId = SkillInfo->using_skill_id_2;
	//	else if (LevelUpSkill->skill_id == SkillInfo->using_skill_id_3)
	//		EquipSkillId = SkillInfo->using_skill_id_3;

	//	auto* ClassInfoBox = StaticFindPCClassInfoBox();
	//	auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;

	//	auto* SkillSingleInfo = AllSkillInfo->GetSingleInfoOfID(LevelUpSkill->skill_id);

	//	// TODO skill quick slot
	//	auto CharInfo = LocalCharacterData.GetPlayerStatBasicInfo(InPCClass);
	//	for (auto Skill : CharInfo->skills)
	//	{
	//		if (Skill->skill_id == LevelUpSkill->skill_id)
	//		{

	//			if (EquipSkillId || SkillSingleInfo->PassiveType != EPassiveType::EPassiveType_None)
	//				SkillInfo->skill_total_power += SkillPower - GetSkillPower(Skill->skill_id, Skill->skill_level);

	//			if (Skill->skill_level != LevelUpSkill->skill_level)
	//			{
	//				LobbyHeroTransientEffectBeginClass<const int32>::GetInstance().Signal(0);
	//			}

	//			Skill->skill_level = LevelUpSkill->skill_level;
	//			break;
	//		}
	//	}

	//	auto SkillQuickSlots = GET_TUPLE_DATA(FB2ResponseLevelUpSkill::skill_quick_slots_index, LevelUpSkillInfo);
	//	for (auto skillQuickSlot : SkillQuickSlots)
	//	{
	//		auto slotNum = skillQuickSlot->slot;
	//		if (CharInfo->skill_quick_slots.IsValidIndex(slotNum))
	//		{
	//			CharInfo->skill_quick_slots[slotNum] = skillQuickSlot;
	//		}
	//	}

	//	ChangeSkillPointClass<>::GetInstance().Signal();
	//	ChangeSkillLevelClass<int32>::GetInstance().Signal(LevelUpSkill->skill_id);

	//	// 傈捧仿 函版 扑诀 免仿 170405_YJ
	//	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(InPCClass, true);
	//}
}

void FClientDataStore::RequestResetSkillPreset(EPCClass SelectedClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_RequestResetSkillPreset);
	data_trader::Retailer::GetInstance().RequestResetSkill(CliToSvrPCClassType(SelectedClass));
}

void FClientDataStore::ResponseResetSkillPreset(const FB2ResetSkillInfo& ResetSkillInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseResetSkillPreset);
	//ReplaceUserDataWithDoc(EDocUserDataType::Gem, GET_TUPLE_DATA(FB2ResponseResetSkill::current_gem_index, ResetSkillInfo));

	//// TODO skill quick slot
	//int32 charType = GET_TUPLE_DATA(FB2ResponseResetSkill::character_type_index, ResetSkillInfo);

	//int32 nLeftSkillPoint = GET_TUPLE_DATA(FB2ResponseResetSkill::left_skill_point_index, ResetSkillInfo);

	//LocalCharacterData.SetCharacterLeftSkillPoint(SvrToCliPCClassType(charType), nLeftSkillPoint);

	//if (auto CharInfo = LocalCharacterData.GetPlayerStatBasicInfo(SvrToCliPCClassType(charType)))
	//{
	//	auto SkillQuickSlots = GET_TUPLE_DATA(FB2ResponseResetSkill::skill_quick_slots_index, ResetSkillInfo);
	//	for (auto skillQuickSlot : SkillQuickSlots)
	//	{
	//		auto slotNum = skillQuickSlot->slot;
	//		if (CharInfo->skill_quick_slots.IsValidIndex(slotNum))
	//		{
	//			CharInfo->skill_quick_slots[slotNum] = skillQuickSlot;
	//		}
	//	}

	//	auto serverSkills = GET_TUPLE_DATA(FB2ResponseResetSkill::skills_index, ResetSkillInfo);
	//	for (auto Skill : CharInfo->skills)
	//	{
	//		for (auto serverSkill : serverSkills)
	//		{
	//			if (Skill->skill_id == serverSkill->skill_id)
	//			{
	//				if (Skill->skill_level != serverSkill->skill_level)
	//				{
	//					Skill->skill_level = serverSkill->skill_level;
	//					ChangeSkillLevelClass<int32>::GetInstance().Signal(Skill->skill_id);
	//				}
	//				break;
	//			}
	//		}
	//	}

	//	ChangeSkillPointClass<>::GetInstance().Signal();

	//	// 傈捧仿 函版 扑诀 免仿 170405_YJ
	//	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(SvrToCliPCClassType(charType), true);
	//}
}

void FClientDataStore::RequestBuySkillPoint(TArray<int32>& NumSkillPointToBuy)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_RequestBuySkillPoint);
	TArray<b2network::B2BuySkillPointRequestInfoPtr> buySkillPointRequest;

	BII_CHECK(GetMaxPCClassNum() == NumSkillPointToBuy.Num());

	for (int32 i = 0; i < NumSkillPointToBuy.Num(); ++i)
	{
		if (NumSkillPointToBuy[i] > 0)
		{
			auto buyInfo = std::make_shared<b2network::B2BuySkillPointRequestInfo>();
			buyInfo->character_type = CliToSvrPCClassType(IntToPCClass(i));
			// TODO skill quick slot
			buyInfo->skill_point_buy_count = NumSkillPointToBuy[i];
			buySkillPointRequest.Add(buyInfo);
		}
	}

	data_trader::Retailer::GetInstance().RequestBuySkillPoint(buySkillPointRequest);
}

void FClientDataStore::ResponseBuySkillPoint(const FB2BuySkillPointInfo& BuySkillPointInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseBuySkillPoint);
	//ReplaceUserDataWithDoc(EDocUserDataType::Gem, GET_TUPLE_DATA(FB2ResponseBuySkillPoint::left_gem_index, BuySkillPointInfo));

	////preset俊 措茄 备泅捞 绝栏骨肺 唱吝俊 眠啊登搁 荐沥登绢具 且 淀.
	//auto buyResults = GET_TUPLE_DATA(FB2ResponseBuySkillPoint::buy_results_index, BuySkillPointInfo);
	//for (b2network::B2BuySkillPointResponseInfoPtr buyResult : buyResults)
	//{
	//	int32 charType = buyResult->character_type;
	//	EPCClass PCClass = SvrToCliPCClassType(charType);

	//	bool IsDifferentSkillPoiont = false;

	//	if (LocalCharacterData.GetCharacterLeftSkillPoint(PCClass) != buyResult->left_skill_point)
	//	{
	//		IsDifferentSkillPoiont = true;
	//	}

	//	LocalCharacterData.SetCharacterLeftSkillPoint(PCClass, buyResult->left_skill_point);

	//	if (IsDifferentSkillPoiont)
	//	{
	//		LobbyHeroTransientEffectBeginClass<const int32>::GetInstance().Signal(0);
	//	}

	//	auto CharInfo = LocalCharacterData.GetPlayerStatBasicInfo(PCClass);

	//	if (CharInfo)
	//		CharInfo->skill_point_buy_count = buyResult->total_skill_point_buy_count;
	//}

	//ABladeIIGameMode::SetHUDCenterMessage(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("SkillInfo_PurchaseComplete")), 1.0f);

	//ChangeSkillPointClass<>::GetInstance().Signal();
}

void FClientDataStore::RequestSetUsingSkill(EPCClass SelectedClass, int32 PresetType, int32 UsingSkill1, int32 UsingSkill2, int32 UsingSkill3)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_RequestSetUsingSkill);
	data_trader::Retailer::GetInstance().RequestSetUsingSkill(CliToSvrPCClassType(SelectedClass), PresetType, UsingSkill1, UsingSkill2, UsingSkill3);
}

void FClientDataStore::ResponseSetUsingSkillInfo(const FB2SetUsingSkillInfo& SetUsingSkillInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseSetUsingSkillInfo);
	//int32 charType = GET_TUPLE_DATA(FB2ResponseSetUsingSkill::character_type_index, SetUsingSkillInfo);
	//auto SkillPresetServerInfo = GET_TUPLE_DATA(FB2ResponseSetUsingSkill::skill_quick_slot_index, SetUsingSkillInfo);

	//if (SkillPresetServerInfo)
	//{
	//	int32 SkillSlotID = SkillPresetServerInfo->slot;
	//	if (auto SkillInfo = LocalCharacterData.GetPlayerStatBasicSkillInfo(SvrToCliPCClassType(charType), SkillSlotID))
	//	{
	//		if (auto CharInfo = LocalCharacterData.GetPlayerStatBasicInfo(SvrToCliPCClassType(charType)))
	//		{
	//			if (CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID))
	//			{
	//				TArray<b2network::B2SkillServerInfoPtr> TemporaryOldSkills(CharInfo->skills);
	//				int32 TemporaryOldLeftSkillPoint = SkillInfo->left_skill_point;

	//				CharInfo->skill_quick_slots[SkillSlotID] = SkillPresetServerInfo;

	//				CharInfo->skills = TemporaryOldSkills;
	//				CharInfo->skill_quick_slots[SkillSlotID]->left_skill_point = TemporaryOldLeftSkillPoint;
	//			}
	//		}
	//	}
	//}

	//RegistSkillClass<>::GetInstance().Signal();
}

void FClientDataStore::RequestSetCharNickName(EPCClass InCharClass, const FText& InNickName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_RequestSetCharNickName);
	if (InCharClass != EPCClass::EPC_End)
	{
		RequestedCharNickNameSetClass = InCharClass;
		RequestedCharNickNameSetNameText = InNickName;

		// 瘤陛 粱 巩力啊 乐绢辑 酒流 沥侥 俺烹 给窃. 咯傈洒 努扼 瞒盔俊辑父 技泼.
		data_trader::Retailer::GetInstance().RequestSetCharNickName(CliToSvrPCClassType(InCharClass), InNickName.ToString());
	}
}

void FClientDataStore::RequestSyncToServerShowHead()
{
	TArray<EPCClass> SyncClasses;
	LocalCharacterData.GetSyncShowHeadItemClass(SyncClasses);

	for (EPCClass PCClass : SyncClasses)
	{
		data_trader::Retailer::GetInstance().RequestSetHelmetVisible(CliToSvrPCClassType(PCClass), LocalCharacterData.IsShowHeadItem(PCClass));
	}
}

void FClientDataStore::RequestSyncToServerCostumeVisible()
{
	TArray<TPair<EPCClass, ECostumeEquipPlace>> SyncCostumeEquips;
	LocalCharacterData.GetSyncCostumeVisibleParts(SyncCostumeEquips);

	for (TPair<EPCClass, ECostumeEquipPlace>& Pair : SyncCostumeEquips)
	{
		data_trader::Retailer::GetInstance().RequestSetCostumeVisible(
			CliToSvrPCClassType(Pair.Key), CliToSvrItemEquipPlace(Pair.Value),
			LocalCharacterData.IsShowCostume(Pair.Key, Pair.Value));
	}
}

void FClientDataStore::RequestGetAccountCostumeAll()
{
	const int32 iPcClassEnd = GetMaxPCClassNum();
	for (int i = (int)EPCClass::EPC_Gladiator; i < iPcClassEnd; ++i)
	{
		const EPCClass PcClass = static_cast<EPCClass>(i);

		this->RequestGetAccountCostume(PcClass, EItemInvenType::EIIVT_CostumeWeapon);
		this->RequestGetAccountCostume(PcClass, EItemInvenType::EIIVT_CostumeArmor);
	}
}

void FClientDataStore::RequestGetAccountCostume(const EPCClass& PcClass, const EItemInvenType& ItemInvenType)
{
	const int32 iServerPcClass = CliToSvrPCClassType(PcClass);
	const int32 iServerItemInvenType = CliToSvrItemInvenType(ItemInvenType);

	data_trader::Retailer::GetInstance().RequestGetAccountCostume(iServerPcClass, iServerItemInvenType);
}

void FClientDataStore::RequestGetAccountTotem()
{
	data_trader::Retailer::GetInstance().RequestGetAccountTotem();
}

void FClientDataStore::ResponseSetCharNickName()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseSetCharNickName);
	//if (RequestedCharNickNameSetClass != EPCClass::EPC_End)
	//{ // 览翠篮 傲 铰牢捞骨肺 捞傈俊 夸没茄 沥焊肺 技泼.
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().SetCharNickName(RequestedCharNickNameSetClass, RequestedCharNickNameSetNameText);
	//	UpdateLobbyCharOnHeadDisplayClass<>::GetInstance().Signal(); // Response 吭阑 锭 捞吧 何福霸 秦具.
	//	LobbyCharObserveCloseNickEditClass<>::GetInstance().Signal(); // 俊叼飘 芒档 捞 矫痢俊 摧绰促.
	//}

	RequestedCharNickNameSetClass = EPCClass::EPC_End;
	RequestedCharNickNameSetNameText = FText::FromString(TEXT(""));
}

void FClientDataStore::ResponseRewardShopMileage(const FB2ResponseRewardShopMileagePtr& MileageReward)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseRewardShopMileage);
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		if (MileageReward->received_reward_index == 0)
			DocStore->SetMileagePoint(MileageReward->shop_mileage_type - b2network::B2ShopMileageType::LOTTERY, 0);
		DocStore->SetMileageReceivedIndex(MileageReward->shop_mileage_type - b2network::B2ShopMileageType::LOTTERY, MileageReward->received_reward_index - 1);
	}

	// void UB2UIMsgPopupReward::ShowRewardPopup(bool bApplyRewardToAccount /* optional = false */)
	// 困窃荐俊辑 可记蔼 true林搁 扑诀 摧鳃锭 焊惑 利侩窃
	//ApplyRewardInfoList(MileageReward->received_rewards);

	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager && (MileageReward->received_rewards.Num() > 0 && !UB2UIManager::GetInstance()->OpenRewardMailPopUp(MileageReward->received_rewards)))
	{
		auto RewardPopup = UIManager->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
		if (RewardPopup != nullptr)
		{
			RewardPopup->AddRewardItems(MileageReward->received_rewards);
			RewardPopup->ShowRewardPopup(true);
		}
	}
}


void FClientDataStore::ResponseEventAttendance(const FB2ResponseEventAttendancePtr& EventAttendance)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseEventAttendance);
	EventAttendanceStatusData.attendance_day = EventAttendance->attendance_day;
	EventAttendanceStatusData.next_attendance_time = EventAttendance->next_attendance_time;
	ReceiveEventAttendanceRewardClass<int32, int32, b2network::B2RewardPtr>::GetInstance().Signal(EventAttendance->eventId,
		EventAttendance->attendance_day, EventAttendance->reward);
}

void FClientDataStore::ResponseUpdateDailyPlayTimeClass(const FB2ResponseUpdateDailyPlayTimePtr& UpdateDailyPlayTime)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseUpdateDailyPlayTimeClass);
	if (PushConnectRewardButton)
	{
		data_trader::Retailer::GetInstance().RequestReceiveDailyPlayTimeReward();
	}
	PushConnectRewardButton = false;
	PlayTimeStatusData.daily_play_time_in_sec = UpdateDailyPlayTime->daily_play_time_in_sec;
	PlayTimeStatusData.daily_play_time_reward_index = UpdateDailyPlayTime->daily_play_time_reward_index;

	UB2GameInstance::SetPlayTimeInfo(PlayTimeStatusData.daily_play_time_in_sec);
}

void FClientDataStore::ResponseDailyPlayTimeRewardClass(const FB2ResponseReceiveDailyPlayTimeRewardPtr& PlayTimeReward)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseDailyPlayTimeRewardClass);
	//PlayTimeStatusData.daily_play_time_in_sec = PlayTimeReward->daily_play_time_in_sec;
	//PlayTimeStatusData.daily_play_time_reward_index = PlayTimeReward->daily_play_time_reward_index;
	//ReceivePlayTimeRewardClass<const TArray<b2network::B2RewardPtr>&>::GetInstance().Signal(PlayTimeReward->rewards);
}

void FClientDataStore::ResponseSetHelmetVisible(const FB2SetHelmetVisible & SetHelmetVisible)
{
//	int32 charType = GET_TUPLE_DATA(FB2ResponseSetHelmetVisible::character_type_index, SetHelmetVisible);
//	bool Visible = GET_TUPLE_DATA(FB2ResponseSetHelmetVisible::is_visible_index, SetHelmetVisible);
//	LocalCharacterData.SetShowHeadItem(SvrToCliPCClassType(charType), Visible);
}

void FClientDataStore::ResponseSetCostumeVisible(const FB2SetCostumeVisiblePtr & CostumeVisible)
{
	EPCClass CharClass = SvrToCliPCClassType(CostumeVisible->character_type);
	ECostumeEquipPlace CostumeEquipPlace = SvrToCliCostumeEquipPlace(CostumeVisible->costume_type);
	bool Visible = CostumeVisible->is_visible;

	LocalCharacterData.SetServerCostumeVisible(CharClass, CostumeEquipPlace, CostumeVisible->is_visible);
}

void FClientDataStore::ResponseSetEnhanceTeamFormation(const FB2EnhanceTeamFormation& EnhanceResult)
{
	//if (AccountInfo)
	//{
	//	AccountInfo->money = GET_TUPLE_DATA(FB2ResponseEnhanceTeamFormation::current_gold_index, EnhanceResult);
	//}
}

void FClientDataStore::BridgeRequestJoinRoom(const FString& RoomID, int32 RoomType, int32 RoomAccessType)
{
	TArray<b2network::B2RoomPropertyPtr> properties;
	TArray<b2network::B2RoomCharacterPtr> characters;
	TArray<b2network::B2PresetNumPtr> presetNums;

	// temporary test code

	const int32 PCClassMax = GetMaxPCClassNum();
	for (int PCIndex = PCClassToInt(EPCClass::EPC_Gladiator); PCIndex < GetMaxPCClassNum(); ++PCIndex)
	{
		const EPCClass PCClass = IntToPCClass(PCIndex);

		auto b2PresetNum = std::make_shared<b2network::B2PresetNum>();
		b2PresetNum->character_type = CliToSvrPCClassType(PCClass);
		b2PresetNum->item_preset_num = CliToSvrPresetID(LocalCharacterData.GetPCClassItemPresetSlotID(PCClass));
		b2PresetNum->skill_preset_num = CliToSvrPresetID(LocalCharacterData.GetPCClassSkillSlotID(PCClass));
		presetNums.Emplace(b2PresetNum);
	}

	/*
	data_trader::RequestJoinRoomClass<const FString&, int32, int32, TArray<b2network::B2RoomPropertyPtr>&,
	TArray<b2network::B2RoomCharacterPtr>&, TArray<b2network::B2PresetNumPtr>&>::GetInstance()
	.Signal(RoomID, RoomType, 0, properties, characters, presetNums);
	*/

	data_trader::Retailer::GetInstance().RequestRequestJoinRoom(RoomID, RoomType, RoomAccessType, properties, characters, presetNums);
}

void FClientDataStore::ResponseGetAccountCostume(const FB2GetAccountCostumePtr& info)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseGetAccountCostume);

	if (info == nullptr)
	{
		return;
	}

	TArray<b2network::B2CostumeServerInfoPtr>& ArrayCostumeInfo = info->costumes;

	const int32 iCostumeInfoArrayMax = ArrayCostumeInfo.Num();
	for (int32 i = 0; i < iCostumeInfoArrayMax; ++i)
	{
		if (ArrayCostumeInfo.IsValidIndex(i) && ArrayCostumeInfo[i])
		{
			AddCostumeItem(*ArrayCostumeInfo[i]);
		}
	}

	SuccesGetAccountCostumeAllClass<>::GetInstance().Signal();
}

void FClientDataStore::ResponseGetAccountTotem(const FB2GetAccountTotemPtr& info)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseGetAccountTotem);

	if (info == nullptr)
	{
		return;
	}

	ResetTotem();

	TArray<b2network::B2TotemServerInfoPtr>& ArrayTotemInfo = info->totems;

	const int32 iTotemInfoArrayMax = ArrayTotemInfo.Num();
	for (int32 i = 0; i < iTotemInfoArrayMax; ++i)
	{
		if (ArrayTotemInfo.IsValidIndex(i) && ArrayTotemInfo[i])
		{
			AddTotem(ArrayTotemInfo[i]);

			EPCClass OwnerClassType = SvrToCliPCClassType(ArrayTotemInfo[i]->character_type);

			if (OwnerClassType != EPCClass::EPC_End)
			{
				if (LocalCharacterData.EquippedTotems.Contains(OwnerClassType) == false)
				{
					LocalCharacterData.EquippedTotems.Add(OwnerClassType, TMap<int64, FB2Totem>());
				}

				LocalCharacterData.EquippedTotems[OwnerClassType].Add(ArrayTotemInfo[i]->id, ArrayTotemInfo[i]);
			}
		}
	}

	SuccesGetAccountTotemClass<>::GetInstance().Signal();
}

void FClientDataStore::ResponseAddCostumeToInventory(const FB2AddCostumeToInventoryPtr& pAddCostume)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_AddCostume);

	if (pAddCostume && pAddCostume->costume)
	{
		AddCostumeItem(*pAddCostume->costume);
	}
}

void FClientDataStore::GetStoreProductList(int32 StoreCategory, int32 GoodsCategory, TArray<FStoreProductData>& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetStoreProductList);

	OutList.Empty();

	for (auto& ShopProductData : MD_Store_GeneralShopProductData)
	{
		FStoreProductData ShopItem = ShopProductData.Value;
		if (ShopItem.StoreCategory == StoreCategory && ShopItem.GoodsCategory == GoodsCategory)
		{
			OutList.Add(ShopItem);
		}
	}

	OutList.Sort([](const FStoreProductData& Data1, const FStoreProductData& Data2)->bool {
		return Data1.GoodsCategoryOrder < Data2.GoodsCategoryOrder;
	});
}

void FClientDataStore::GetStoreCategoryOrder(TMap<int32, int32>& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetStoreCategoryOrder);

	OutList.Empty();

	for (auto& CategoryOrderData : MD_Store_CategoryOrderData)
	{
		OutList.Add(CategoryOrderData.Key, CategoryOrderData.Value);
	}

	OutList.KeySort([](const int32& Data1, const int32& Data2)->bool {
		return Data1 < Data2;
	});
}

const FStoreProductData* FClientDataStore::GetStoreProductData(int32 ProductId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetStoreProductData);

	return MD_Store_GeneralShopProductData.Find(ProductId);
}

const FStoreCashBonusData* FClientDataStore::GetStoreCashBonusData(int32 ProductId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetStoreCashBonusData);

	return MD_Store_CashBonusData.Find(ProductId);
}

bool FClientDataStore::GetStoreCashBonusGoodsCategory(int32 GoodsCategory)
{
	for (auto& CashBonusData : MD_Store_CashBonusData)
	{
		FStoreCashBonusData CashBonus = CashBonusData.Value;
		if (CashBonus.GoodsCategory == GoodsCategory)
			return true;
	}

	return false;
}

const FStoreFundsSale* FClientDataStore::GetStoreFundsSaleData(int32 ProductId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetStoreFundsSaleData);

	return MD_Store_FundsSaleData.Find(ProductId);
}

bool FClientDataStore::GetStoreFundsSaleStoreCategory(int32 StoreCategory)
{
	for (auto& FundsSaleData : MD_Store_FundsSaleData)
	{
		FStoreFundsSale FundsSale = FundsSaleData.Value;
		if (FundsSale.StoreCategory == StoreCategory)
			return true;
	}

	return false;
}

bool FClientDataStore::GetStoreFundsSaleGoodsCategory(int32 GoodsCategory)
{
	for (auto& FundsSaleData : MD_Store_FundsSaleData)
	{
		FStoreFundsSale FundsSale = FundsSaleData.Value;
		if (FundsSale.GoodsCategory == GoodsCategory)
			return true;
	}

	return false;
}

const FSummonItemLotterySale* FClientDataStore::GetSummonItemLotterySaleData(int32 HashKey)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSummonItemLotterySaleData);

	return MD_Store_LotterySaleData.Find(HashKey);
}

void FClientDataStore::GetPackageList(int32 PackageCategory, TArray<FStoreProductData>& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetPackageList);

	OutList.Empty();

	for (auto& ShopProductData : MD_Store_GeneralShopProductData)
	{
		//if (ShopProductData.StoreCategory == (b2network::B2ShopType::SHOP_PACKAGE-1) && ShopProductData.GoodsCategory == b2network::B2ShopProductType::PACKAGE)
		FStoreProductData& ShopItem = ShopProductData.Value;
		if (ShopItem.StoreCategory == 0 && ShopItem.GoodsCategory == -1)
		{
			if (MD_Store_PackageData.Contains(ShopItem.Product.ProductId))
			{
				const FPackageData* PackageData = GetPackageData(ShopItem.Product.ProductId);
				if (PackageData != nullptr)
				{
					if (PackageData->PackageTabType == PackageCategory)
					{
						ShopItem.PayPeriod = PackageData->PackagePeriod;
						OutList.Add(ShopItem);
					}
				}
			}
		}
	}

	OutList.Sort([](const FStoreProductData& Data1, const FStoreProductData& Data2)->bool {
		return Data1.GoodsCategoryOrder < Data2.GoodsCategoryOrder;
	});
}

void FClientDataStore::GetDailyNoticePackageList(TArray<FStoreProductData>& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetDailyNoticePackageList);

	OutList.Empty();

	for (auto& ShopProductData : MD_Store_GeneralShopProductData)
	{
		FStoreProductData& ShopItem = ShopProductData.Value;
		if (ShopItem.StoreCategory == 0 && ShopItem.GoodsCategory == -1 && ShopItem.RestrictType == 0)
		{
			if (MD_Store_PackageData.Contains(ShopItem.Product.ProductId))
			{
				const FPackageData* PackageData = GetPackageData(ShopItem.Product.ProductId);
				if (PackageData)
				{
					if (PackageData->PackagePeriod > 0)
					{
						ShopItem.PayPeriod = PackageData->PackagePeriod;
						OutList.Add(ShopItem);
					}
				}
			}
		}
	}

	OutList.Sort([](const FStoreProductData& Data1, const FStoreProductData& Data2)->bool {
		return Data1.StoreProductId < Data2.StoreProductId;
	});
}

const FPackageData* FClientDataStore::GetPackageData(int32 PackageID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetPackageData);

	return MD_Store_PackageData.Find(PackageID);
}

void FClientDataStore::GetPackageProductList(int32 PackageId, TArray<FPackageProductData>& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetPackageProductList);

	OutList.Empty();
	MD_Store_PackageProductData.MultiFind(PackageId, OutList);

	OutList.Sort([](const FPackageProductData& Data1, const FPackageProductData& Data2)->bool
		{
			return Data1.PackageIndex < Data2.PackageIndex;
		});
}
//
void FClientDataStore::GetPackageLevelUpList(int32 PackageId, TArray<FB2ShopLevelUpPackagePtr>& OutList)
{
	OutList.Empty();
	MD_Store_LevelUpPackageData.MultiFind(PackageId, OutList);

	OutList.StableSort([](const FB2ShopLevelUpPackagePtr& Data1, const FB2ShopLevelUpPackagePtr& Data2)->bool {
		return Data1->package_id < Data2->package_id;
	});

	OutList.StableSort([](const FB2ShopLevelUpPackagePtr& Data1, const FB2ShopLevelUpPackagePtr& Data2)->bool {
		return Data1->level < Data2->level;
	});
};

void FClientDataStore::GetPackageReachLevelList(int32 PackageId, TArray<FB2ShopReachLevelPackagePtr>& OutList)
{
	OutList.Empty();
	MD_Store_ReachLevelPackageData.MultiFind(PackageId, OutList);

	OutList.StableSort([](const FB2ShopReachLevelPackagePtr& Data1, const FB2ShopReachLevelPackagePtr& Data2)->bool {
		return Data1->package_id < Data2->package_id;
	});
};

void FClientDataStore::GetFlatratePackageBonusList(int32 Index, TArray<FB2ShopFlatRatePackageBonusPtr>& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetFlatratePackageBonusList);

	OutList.Empty();
	MD_Store_FlatRatePackageBonusData.MultiFind(Index, OutList);
}

int32 FClientDataStore::GetItemTemplateIdFromMDProductData(const FProductData& ProductData) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemTemplateIdFromMDProductData);
	EStoreItemType ProductItemType = static_cast<EStoreItemType>(ProductData.ProductType);

	return FItemRefIDHelper::GetProductDataItemId(ProductItemType, ProductData.ProductId);
}

int32 FClientDataStore::GetItemTemplateIdFromMDProductData(const int32 ProductID, const int32 ProductType) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemTemplateIdFromMDProductData);
	EStoreItemType ProductItemType = static_cast<EStoreItemType>(ProductType);

	return FItemRefIDHelper::GetProductDataItemId(ProductItemType, ProductID);
}

void FClientDataStore::ResponseBuyShopProduct(const FB2ResponseBuyShopProductPtr& BuyShopProduct)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseBuyShopProduct);
	// Used
	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	switch (BuyShopProduct->buy_price_type)
	{
	case b2network::B2ShopBuyPriceType::SOCIAL_POINT:
		SetSocialPoint(GetSocialPoint() - BuyShopProduct->price);
		if (DocUser)
			DocUser->SetSocialPoint(GetSocialPoint());
		break;

	case b2network::B2ShopBuyPriceType::GEM:
		SetGemAmount(GetGemAmount() - BuyShopProduct->price);
		if (DocUser)
			DocUser->SetGem(GetGemAmount());
		break;

	case b2network::B2ShopBuyPriceType::DARK_CRYSTAL:
		SetDarkCrystal(GetDarkCrystal() - BuyShopProduct->price);
		if (DocUser)
			DocUser->SetDarkCrystal(GetDarkCrystal());
		break;

	case b2network::B2ShopBuyPriceType::FAME_POINT:
		SetFamePoint(GetFamePoint() - BuyShopProduct->price);
		if (DocUser)
			DocUser->SetFamePoint(GetFamePoint());
		break;

	case b2network::B2ShopBuyPriceType::GOLD:
		SetGoldAmount(GetGoldAmount() - BuyShopProduct->price);
		if (DocUser)
			DocUser->SetGold(GetGoldAmount());
		break;

	case b2network::B2ShopBuyPriceType::CASH:
		break;
	}

	// Updated
	ResponseBuyShopResultInfo(BuyShopProduct->buy_result_info);

	FText BuyPriceText = (BuyShopProduct->buy_price_type == b2network::B2ShopBuyPriceType::DARK_CRYSTAL || BuyShopProduct->buy_price_type == b2network::B2ShopBuyPriceType::GOLD) ?
		BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bought_MailProduct")) : BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bought_Product"));

	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BuyPriceText,
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
		);
}

void FClientDataStore::ResponseBuyShopResultInfo(const FB2ResponseBuyShopResultInfoPtr BuyShopResult)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseBuyShopResultInfo);
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		// 付老府瘤
		DocStore->SetMileagePoint(BuyShopResult->mileage_type - b2network::B2ShopMileageType::LOTTERY, BuyShopResult->current_mileage);

		// 备概 冉荐
		for (auto Restrict : BuyShopResult->shop_buy_manages)
		{
			DocStore->SetReceivedRestrictCount(Restrict->product_id, Restrict->buy_count);
		}

		// 扁埃力 巢篮 矫埃
		if (BuyShopResult->periodic_package_state->package_id != 0)
		{
			DocStore->SetReceivedPeriodicPackageLeftTime(BuyShopResult->periodic_package_state->package_id, BuyShopResult->periodic_package_state->end_pay_time);
		}

		// 饭骇诀 菩虐瘤
		for (auto LevelUpMission : BuyShopResult->level_up_package_missions)
		{
			if (LevelUpMission->package_id != 0)
			{
				DocStore->SetReceivedLevelupPacakgeMissions(LevelUpMission->package_id, LevelUpMission);
			}
		}

		// 饭骇 崔己 拌距 菩虐瘤
		if (BuyShopResult->reach_level_package_mission)
		{
			if (BuyShopResult->reach_level_package_mission->package_id != 0)
			{
				DocStore->SetReceivedReachLevelPacakgeMissions(BuyShopResult->reach_level_package_mission->package_id, BuyShopResult->reach_level_package_mission);
			}
		}

		// 岿沥咀
		if (BuyShopResult->flat_rate_package_bonus_state)
		{
			if (BuyShopResult->flat_rate_package_bonus_state->current_index != 0)
			{
				DocStore->SetReceivedFlatratePackageBonusState(BuyShopResult->flat_rate_package_bonus_state);
			}
		}
	}

	// Package Goto Mail
	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	if (BuyShopResult->buy_product_type != b2network::B2ShopProductType::PACKAGE)
	{
		for (auto& Product : BuyShopResult->add_products)
		{
			switch (Product->product_type)
			{
			case b2network::B2ShopProductType::BLADE_POINT:
				AddBladePoint(Product->product_count);
				if (DocUser)
					DocUser->SetStaminaCur(GetBladeCurPoint());
				break;

			case b2network::B2ShopProductType::GEM:
				AddGem(Product->product_count);
				if (DocUser)
					DocUser->SetGem(GetGemAmount());
				break;

			case b2network::B2ShopProductType::GOLD:
				AddGold(Product->product_count);
				if (DocUser)
					DocUser->SetGold(GetGoldAmount());
				break;

			case b2network::B2ShopProductType::DARK_CRYSTAL:
				AddDarkCrystal(Product->product_count);
				if (DocUser)
					DocUser->SetDarkCrystal(GetDarkCrystal());
				break;


			default:
				// 唱赣瘤绰 快祈窃栏肺 埃促窃.
				break;
			}
		}
	}
}

void FClientDataStore::ResponseBuyMagicShopProduct(const FB2ResponsePurchaseMagicShopPtr& MagicShopPtr)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_ResponseBuyMagicShopProduct);
	// Used
	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	switch (MagicShopPtr->buy_price_type)
	{
	case b2network::B2ShopBuyPriceType::GEM:
		SetGemAmount(GetGemAmount() - MagicShopPtr->price);
		if (DocUser)
			DocUser->SetGem(GetGemAmount());
		break;

	case b2network::B2ShopBuyPriceType::SOCIAL_POINT:
		SetSocialPoint(GetSocialPoint() - MagicShopPtr->price);
		if (DocUser)
			DocUser->SetSocialPoint(GetSocialPoint());
		break;

	case b2network::B2ShopBuyPriceType::DARK_CRYSTAL:
		SetDarkCrystal(GetDarkCrystal() - MagicShopPtr->price);
		if (DocUser)
			DocUser->SetDarkCrystal(GetDarkCrystal());
		break;

	case b2network::B2ShopBuyPriceType::CASH:
		break;

	case b2network::B2ShopBuyPriceType::GOLD:
		SetGoldAmount(GetGoldAmount() - MagicShopPtr->price);
		if (DocUser)
			DocUser->SetGold(GetGoldAmount());
		break;
	}

	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bought_MailProduct")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
		);
}

int32 CliToSvrStageBuffTypeComposit(const TArray<EStageBuffType>& AllCliSBT)
{
	int32 RetVal = 0;
	for (EStageBuffType ThisType : AllCliSBT)
	{ // b2network::B2StageBuffFlag 曼绊.
		RetVal |= CliToSvrStageBuffType(ThisType);
	}
	return RetVal;
}

FMD_ItemSingleElem& FMD_ItemSingleElem::operator=(const b2network::B2mdItemInfoPtr& ItemMasterInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FMD_ItemSingleElem_operator);
	if (ItemMasterInfo)
	{
		ItemTemplateID = ItemMasterInfo->md_template_id;
		ItemEquipClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(ItemTemplateID);
		SetGroupID = ItemMasterInfo->md_group_id;

		ItemInvenType = SvrToCliItemInvenType(ItemMasterInfo->md_inventory_type);
		ItemEquipPlace = SvrToCliItemEquipPlace(ItemMasterInfo->md_equip_position);
		SetOptionID = ItemMasterInfo->md_set_id;
		ItemClass = SvrToCliItemClass(ItemMasterInfo->md_type);
		MaxLevel = ItemMasterInfo->md_max_levelup;

		PrimaryType = SvrToCliPrimPointType(ItemMasterInfo->md_main_attribute);
		ItemFactorLevel = ItemMasterInfo->md_level_factor;

		if (ItemMasterInfo->md_unique_option_id1 != b2network::B2ItemOption::BIO_None)
			UniqueOptions.Add(FItemOption(SvrToCliOptionType(ItemMasterInfo->md_unique_option_id1), ItemMasterInfo->md_unique_option_value1));
		if (ItemMasterInfo->md_unique_option_id2 != b2network::B2ItemOption::BIO_None)
			UniqueOptions.Add(FItemOption(SvrToCliOptionType(ItemMasterInfo->md_unique_option_id2), ItemMasterInfo->md_unique_option_value2));
		if (ItemMasterInfo->md_unique_option_id3 != b2network::B2ItemOption::BIO_None)
			UniqueOptions.Add(FItemOption(SvrToCliOptionType(ItemMasterInfo->md_unique_option_id3), ItemMasterInfo->md_unique_option_value3));
		if (ItemMasterInfo->md_unique_option_id4 != b2network::B2ItemOption::BIO_None)
			UniqueOptions.Add(FItemOption(SvrToCliOptionType(ItemMasterInfo->md_unique_option_id4), ItemMasterInfo->md_unique_option_value4));

		//UniqueSkillID = ItemMasterInfo->md_unique_skill_id;
		MaxGrade = ItemMasterInfo->md_max_grade;
		ItemGrade = ItemMasterInfo->md_grade;

		RandomOptCount = ItemMasterInfo->md_random_option_count;
		NextGradeItemTemplateID = ItemMasterInfo->md_next_grade_template_id;
		BuyPrice = ItemMasterInfo->md_buy_price;
		SellPrice = ItemMasterInfo->md_sell_price;

		SealSlot = ItemMasterInfo->seal_slot;
		SealSlotOpen = ItemMasterInfo->seal_slot_open;
		SellAble = ItemMasterInfo->is_saleable;
		// 酒捞袍 鸥涝喊肺 悼累 啊瓷 咯何甫 沥狼窍绰 敲贰弊甸捞 辑滚窜俊 甸绢吭栏聪 鞘夸俊 蝶扼 啊廉客辑 荤侩. 弊 悼救 努扼捞攫飘 磊眉利牢 炼扒俊 蝶扼 悼累 鞘磐傅阑 窍看澜.
		//is_stackable
		//is_equipable
		//is_levelupable
		//is_enhanceable
		//is_upgradable
		//is_composable
		//is_surpassable
	}

	return *this;
}

FMD_ItemSingleElem& FMD_ItemSingleElem::InitCostumeItemInfo(const b2network::B2mdCostumeInfoPtr& ItemMasterInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FMD_ItemSingleElemInitCostumeItemInfo);
	if (ItemMasterInfo)
	{
		ItemTemplateID = ItemMasterInfo->md_template_id;
		ItemEquipClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(ItemTemplateID);
		SetGroupID = ItemMasterInfo->md_group_id;

		ItemInvenType = SvrToCliItemInvenType(ItemMasterInfo->md_inventory_type);
		//costume use other place ItemEquipPlace = SvrToCliItemEquipPlace(ItemMasterInfo->md_equip_position);
		CostumeEquipPlace = SvrToCliCostumeEquipPlace(ItemMasterInfo->md_equip_position);
		SetOptionID = ItemMasterInfo->md_set_id;
		ItemClass = SvrToCliItemClass(ItemMasterInfo->md_type);
		MaxLevel = ItemMasterInfo->md_max_levelup;

		PrimaryType = SvrToCliPrimPointType(ItemMasterInfo->md_main_attribute);
		ItemFactorLevel = ItemMasterInfo->md_level_factor;

		if (ItemMasterInfo->md_unique_option_id1 != b2network::B2ItemOption::BIO_None)
			UniqueOptions.Add(FItemOption(SvrToCliOptionType(ItemMasterInfo->md_unique_option_id1), ItemMasterInfo->md_unique_option_value1));
		if (ItemMasterInfo->md_unique_option_id2 != b2network::B2ItemOption::BIO_None)
			UniqueOptions.Add(FItemOption(SvrToCliOptionType(ItemMasterInfo->md_unique_option_id2), ItemMasterInfo->md_unique_option_value2));
		if (ItemMasterInfo->md_unique_option_id3 != b2network::B2ItemOption::BIO_None)
			UniqueOptions.Add(FItemOption(SvrToCliOptionType(ItemMasterInfo->md_unique_option_id3), ItemMasterInfo->md_unique_option_value3));
		if (ItemMasterInfo->md_unique_option_id4 != b2network::B2ItemOption::BIO_None)
			UniqueOptions.Add(FItemOption(SvrToCliOptionType(ItemMasterInfo->md_unique_option_id4), ItemMasterInfo->md_unique_option_value4));

		MaxGrade = ItemMasterInfo->md_max_grade;
		ItemGrade = ItemMasterInfo->md_grade;

		RandomOptCount = ItemMasterInfo->md_random_option_count;
		NextGradeItemTemplateID = ItemMasterInfo->md_next_grade_template_id;
		BuyPrice = ItemMasterInfo->md_buy_price;
		SellPrice = ItemMasterInfo->md_sell_price;
		SellAble = ItemMasterInfo->is_saleable;
	}

	return *this;
}

void FClassCollectionItemData::InitClassItemMasterData()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_InitClassItemMasterData);
	int32 ItemClassBegin = static_cast<int32>(EItemClass::EIC_Weapon);
	int32 ItemClassEnd = static_cast<int32>(EItemClass::EIC_CostumeArmor);

	for (int32 Idx = ItemClassBegin; Idx <= ItemClassEnd; Idx++)
	{
		EItemClass AddItemClass = static_cast<EItemClass>(Idx);
		TArray<FMD_ItemSingleElem> ItemArray;
		ItemMasterInfos.Add(AddItemClass, ItemArray);
	}
}

FORCEINLINE void FClassCollectionItemData::AddItem(const FMD_ItemSingleElem& ItemElem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_AddItem);
	ItemMasterInfos[ItemElem.ItemClass].Add(ItemElem);
}

void FClassCollectionItemData::AddSetItemInfo(const FMD_ItemSingleElem& ItemElem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_AddSetItemInfo);
	int32 SetUniqueKey = GetSetItemUniqueKey(ItemElem.SetGroupID, ItemElem.ItemTemplateID);

	FMD_SetInfo* SetInfo = SetItemMasterInfos.Find(SetUniqueKey);
	if (SetInfo)
	{
		SetInfo->SetCompositionItems.Add(ItemElem.ItemTemplateID);
		SetInfo->SetOptionIDs.AddUnique(ItemElem.SetOptionID);
	}
	else
	{
		FMD_SetInfo NewSetInfo;
		NewSetInfo.SetUniqueKey = SetUniqueKey;
		NewSetInfo.SetGroupID = ItemElem.SetGroupID;
		NewSetInfo.SetGrade = ItemElem.ItemGrade;
		NewSetInfo.TargetClass = ItemElem.ItemEquipClass;
		NewSetInfo.SetCompositionItems.Add(ItemElem.ItemTemplateID);

		SetItemMasterInfos.Add(SetUniqueKey, NewSetInfo);
	}
}

void FClassCollectionItemData::AddCollectionItem(EItemClass ItemClass, const FCollectionItemData& ItemData)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_AddCollectionItem);
	TArray<FCollectionItemData>& Collections = CollectionItems.FindOrAdd(ItemClass);
	FCollectionItemData* FindedCollectionItem = GetCollectionItemInfo(ItemClass, ItemData.ID);
	if (FindedCollectionItem)
	{
		FindedCollectionItem->bNewItem = ItemData.bNewItem;
		FindedCollectionItem->CollectionState = ItemData.CollectionState;
	}
	else
	{
		Collections.Add(ItemData);
	}
}

void FClassCollectionItemData::AddSetCollectionItem(const FCollectionItemData& ItemData)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_AddSetCollectionItem);
	const FMD_ItemSingleElem* ItemElemPtr = GetSingleItemInfo(ItemData.ID);
	if (ItemElemPtr && ItemElemPtr->SetGroupID)
	{
		if (ItemElemPtr->SetGroupID == b2network::B2GroupPrefix::GROWTH
			|| ItemElemPtr->SetGroupID == b2network::B2GroupPrefix::NON_EQUIP
			//|| ItemElemPtr->SetGroupID == b2network::B2GroupPrefix::ETERNAL
			|| ItemElemPtr->SetGroupID == b2network::B2GroupPrefix::DUEL)
			return;

		const int32 SetUniqueKey = GetSetItemUniqueKey(ItemElemPtr->SetGroupID, ItemElemPtr->ItemTemplateID);
		TArray<int32>& SetCompositionItems = CollectedSetCompositionItems.FindOrAdd(SetUniqueKey);
		SetCompositionItems.AddUnique(ItemElemPtr->ItemTemplateID);
	}
}

void FClassCollectionItemData::SortAllItemMasterInfo()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_SortAllItemMasterInfo);
	TArray<EItemClass> ItemClassKeys;
	ItemMasterInfos.GenerateKeyArray(ItemClassKeys);

	for (auto ItemClass : ItemClassKeys)
	{
		TArray<FMD_ItemSingleElem>* FindedItemMasterInfo = ItemMasterInfos.Find(ItemClass);
		if (FindedItemMasterInfo == nullptr)
			continue;


		FindedItemMasterInfo->Sort([](const FMD_ItemSingleElem& ItemInfo1, const FMD_ItemSingleElem& ItemInfo2) {
			int32 SortKey1 = ItemInfo1.SetGroupID * 100 + ItemInfo1.ItemGrade;
			int32 SortKey2 = ItemInfo2.SetGroupID * 100 + ItemInfo2.ItemGrade;

			return SortKey1 > SortKey2 ? false : true;
		});

	}
}

void FClassCollectionItemData::UpdateSetCollectionState()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_UpdateSetCollectionState);
	for (TMap<int32, TArray<int32>>::TIterator It(CollectedSetCompositionItems); It; ++It)
	{
		const int32 SetUniqueKey = It.Key();
		const TArray<int32>& SetCompositionItems = It.Value();

		if (GetCollectionItemInfo(EItemClass::EIC_SetItem, SetUniqueKey) == nullptr)
		{
			const FMD_SetInfo* SetInfo = SetItemMasterInfos.Find(SetUniqueKey);
			if (SetInfo && SetInfo->SetCompositionItems.Num() == SetCompositionItems.Num())
			{
				FCollectionItemData RewardedSetItemData;
				RewardedSetItemData.bNewItem = false;
				RewardedSetItemData.CollectionState = EItemCollectionState::Rewarded;
				RewardedSetItemData.ID = SetUniqueKey;

				AddCollectionItem(EItemClass::EIC_SetItem, RewardedSetItemData);
			}
		}
	}
}

FMD_ItemSingleElem* FClassCollectionItemData::GetSingleItemInfo(int32 ItemTemplateID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_GetSingleItemInfo);
	EItemClass ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(ItemTemplateID);

	TArray<FMD_ItemSingleElem>* ClassItemList = ItemMasterInfos.Find(ItemClass);
	if (ClassItemList != nullptr)
	{
		TArray<FMD_ItemSingleElem>& ItemElems = *ClassItemList;
		// need to convert binary search
		for (int32 Idx = 0; Idx < ItemElems.Num(); Idx++)
		{
			if (ItemElems[Idx].ItemTemplateID == ItemTemplateID)
				return &ItemElems[Idx];
		}
	}

	return nullptr;
}

FCollectionItemData* FClassCollectionItemData::GetCollectionItemInfo(EItemClass ItemClass, int32 ID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_GetCollectionItemInfo);
	TArray<FCollectionItemData>* CollectionItemList = CollectionItems.Find(ItemClass);
	if (CollectionItemList != nullptr)
	{
		TArray<FCollectionItemData>& Collections = *CollectionItemList;

		// need to convert binary search
		for (int32 Idx = 0; Idx < Collections.Num(); Idx++)
		{
			if (Collections[Idx].ID == ID)
				return &Collections[Idx];
		}
	}
	return nullptr;
}


FCollectionItemData* FClassCollectionItemData::GetCollectionItemInfo(int32 ItemTemplateID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_GetCollectionItemInfo);
	EItemClass ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(ItemTemplateID);
	return GetCollectionItemInfo(ItemClass, ItemTemplateID);
}

FCollectionItemData* FClassCollectionItemData::GetSetCollectionItemInfo(int32 SetUniqueID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_GetSetCollectionItemInfo);
	return GetCollectionItemInfo(EItemClass::EIC_SetItem, SetUniqueID);
}

int32 FClassCollectionItemData::GetSetCompositionItems(int32 SetUniqueKey, TArray<int32>& OutCompositionItems)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_GetSetCompositionItems);
	TArray<int32>* SetCompositions = CollectedSetCompositionItems.Find(SetUniqueKey);
	if (SetCompositions != nullptr)
		OutCompositionItems = *SetCompositions;

	return OutCompositionItems.Num();
}

int32 FClassCollectionItemData::GetSetCollectionCount()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_GetSetCollectionCount);
	TArray<int32> SetKeys;
	SetItemMasterInfos.GetKeys(SetKeys);

	int32 SetCollectionCount = 0;

	TArray<int32> ItemTemplateIDs;
	for (int32 SetUniqueKey : SetKeys)
	{
		if (GetSetCompositionItems(SetUniqueKey, ItemTemplateIDs) > 0)
		{
			SetCollectionCount++;
			ItemTemplateIDs.Empty();
		}
	}

	return SetCollectionCount;
}

int32 FClassCollectionItemData::GetCollectionItemCount(EItemClass ItemClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_GetCollectionItemCount);
	TArray<FCollectionItemData>* CurrentCollections = CollectionItems.Find(ItemClass);
	if (CurrentCollections != nullptr)
	{
		return CurrentCollections->Num();
	}

	return 0;
}

int32 FClassCollectionItemData::GetEnhancedCollectionItemCount(EItemClass ItemClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_GetNewCollectionItemCount);
	int32 NewItem = 0;
	TArray<FCollectionItemData>* CurrentCollections = CollectionItems.Find(ItemClass);
	if (CurrentCollections != nullptr)
	{
		for (auto& ItemData : *CurrentCollections)
		{
			if (ItemData.ID != 0 && ItemData.CollectionState == EItemCollectionState::Enhanced)
				NewItem++;
		}
	}

	return NewItem;
}

void FClassCollectionItemData::ClearCollectionItems(ECollectionType CollectionType)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_ClearCollectionItems);
	TArray<EItemClass> ItemClassList = GetItemClassListByCollectionType(CollectionType);
	for (EItemClass ItemClass : ItemClassList)
		CollectionItems.Remove(ItemClass);
}

void FClassCollectionItemData::ClearAllCollectionItems()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClassCollectionItemData_ClearAllCollectionItems);
	ClearCollectionItems(ECollectionType::Weapon);
	ClearCollectionItems(ECollectionType::Armor);
	ClearCollectionItems(ECollectionType::Accessory);
}

void FItemMasterData::InitItemMasterData()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FItemMasterData_InitItemMasterData);
	CharacterItems.Add(EPCClass::EPC_Gladiator, FClassCollectionItemData());
	CharacterItems.Add(EPCClass::EPC_Assassin, FClassCollectionItemData());
	CharacterItems.Add(EPCClass::EPC_Wizard, FClassCollectionItemData());
	CharacterItems.Add(EPCClass::EPC_Fighter, FClassCollectionItemData());

	CharacterItems[EPCClass::EPC_Gladiator].InitClassItemMasterData();
	CharacterItems[EPCClass::EPC_Assassin].InitClassItemMasterData();
	CharacterItems[EPCClass::EPC_Wizard].InitClassItemMasterData();
	CharacterItems[EPCClass::EPC_Fighter].InitClassItemMasterData();

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
}

void FItemMasterData::AddItemMasterData(const FMD_ItemSingleElem& ItemInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FItemMasterData_AddItemMasterData);
	if (ItemInfo.ItemEquipClass == EPCClass::EPC_End ||
		IsEssenceItemClass(ItemInfo.ItemClass) // 沥荐 酒捞袍捞 EquipClass 啊 厘馒 啊瓷茄 单捞磐肺 甸绢客辑 流立 抗寇 贸府.
		)
	{
		NonEquipItems.Add(ItemInfo.ItemTemplateID, ItemInfo);
	}
	else if (IsAnvilItemClass(ItemInfo.ItemClass))
	{
		NonEquipItems.Add(ItemInfo.ItemTemplateID, ItemInfo);
	}
	else
	{
		CharacterItems[ItemInfo.ItemEquipClass].AddItem(ItemInfo);
		CharacterItems[ItemInfo.ItemEquipClass].AddSetItemInfo(ItemInfo);
		UniqueGroupIDList.AddUnique(ItemInfo.SetGroupID);
		if (ItemInfo.CostumeEquipPlace != ECostumeEquipPlace::ECEP_None)
			CostumeUniqueGroupIDList.AddUnique(ItemInfo.SetGroupID);
	}
}

FMD_ItemSetOptionElem::FMD_ItemSetOptionElem()
	: SetOptionID(0)
{
	SetOptions.Empty();
}

FMD_ItemSetOptionElem& FMD_ItemSetOptionElem::operator=(const b2network::B2mdItemSetPtr& SetOptionMasterInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FMD_ItemSetOptionElem_operator);

	//EItemOption::EIO_End
	if (SetOptionMasterInfo)
	{
		SetOptionID = SetOptionMasterInfo->set_id;
		SetOptions.Add(FItemSetOptionData(2, SvrToCliOptionType(SetOptionMasterInfo->set_option_id_set2), SetOptionMasterInfo->set_value_set2));
		SetOptions.Add(FItemSetOptionData(3, SvrToCliOptionType(SetOptionMasterInfo->set_option_id_set3), SetOptionMasterInfo->set_value_set3));
		SetOptions.Add(FItemSetOptionData(4, SvrToCliOptionType(SetOptionMasterInfo->set_option_id_set4), SetOptionMasterInfo->set_value_set4));
	}
	return *this;
}

const bool FMD_ItemSetOptionElem::GetSetItemOption(int32 InSetItemCount, FItemSetOptionData & SetOptionData) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FMD_ItemSetOptionElem_GetSetItemOption);
	const FItemSetOptionData* FoundData = SetOptions.FindByPredicate([InSetItemCount](FItemSetOptionData & InSetOptionData) { return InSetOptionData.SetItemCount == InSetItemCount; });
	if (FoundData)
		SetOptionData = *FoundData;
	return FoundData != nullptr;
}

const void FMD_ItemSetOptionElem::GetAllSetItemOption(TArray<FItemSetOptionData>& SetOptionData, int32 SetItemCount) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FMD_ItemSetOptionElem_GetAllSetItemOption);
	if (SetOptions.Num() > 0)
	{
		SetOptionData = SetOptions;
		if (SetItemCount > 0)
			SetOptionData.RemoveAll([SetItemCount](FItemSetOptionData& OptionData)
				{
					return OptionData.SetItemCount > SetItemCount;
				});
	}

}

const bool FMD_ItemSetOptionElem::HaveSetItemOption() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FMD_ItemSetOptionElem_HaveSetItemOption);
	if (SetOptions.Num() > 0)
	{
		FItemSetOptionData data;
		if (GetSetItemOption(2, data))
			return data.SetItemOptionValue != 0;
	}
	return false;
}

bool FCollectionItemData::operator==(const FCollectionItemData& Other)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FCollectionItemData_operator_Equal);
	return ID == Other.ID;
}

FCollectionItemData& FCollectionItemData::operator=(const b2network::B2CollectionItemPtr CollectionItem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FCollectionItemData_operator);
	if (CollectionItem)
	{
		ID = CollectionItem->item_template_id;
		CollectionState = SvrToClientItemCollectionState(CollectionItem->collect_state);
		bNewItem = CollectionItem->is_new_item;
	}

	return *this;
}

FMD_FairyInfo::FMD_FairyInfo(const b2network::B2mdFairyInfoPtr ServerStruct)
{
	if (ServerStruct)
	{
		FairyType = SvrToCliFairyType(ServerStruct->fairy_type);
		OptionId = ServerStruct->option_id;
		BlessLevel = ServerStruct->bless_level;
		OptionValue = ServerStruct->option_value;
		Level = ServerStruct->level;
	}
}

FMD_FairyInfo& FMD_FairyInfo::operator=(const b2network::B2mdFairyInfoPtr ServerStruct)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FMD_FairyInfo_operator);
	if (ServerStruct)
	{
		FairyType = SvrToCliFairyType(ServerStruct->fairy_type);
		OptionId = ServerStruct->option_id;
		BlessLevel = ServerStruct->bless_level;
		OptionValue = ServerStruct->option_value;
		Level = ServerStruct->level;
	}
	return *this;
}

FMD_FairyLevelUpCost::FMD_FairyLevelUpCost(const b2network::B2mdFairyLevelupCostPtr ServerStruct)
{
	if (ServerStruct)
	{
		Level = ServerStruct->level;
		MaterialCost = ServerStruct->material_cost;
	}
}

FMD_FairyLevelUpCost& FMD_FairyLevelUpCost::operator=(const b2network::B2mdFairyLevelupCostPtr ServerStruct)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FMD_FairyLevelUpCost_operator);
	if (ServerStruct)
	{
		Level = ServerStruct->level;
		MaterialCost = ServerStruct->material_cost;
	}
	return *this;
}

FMD_FairyOpenInfo::FMD_FairyOpenInfo(const b2network::B2mdFairyOpenInfoPtr ServerStruct)
{
	if (ServerStruct)
	{
		FairyType = SvrToCliFairyType(ServerStruct->fairy_type);
		StageId = ServerStruct->stage_id;
	}
}

FMD_FairyOpenInfo& FMD_FairyOpenInfo::operator=(const b2network::B2mdFairyOpenInfoPtr ServerStruct)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FMD_FairyOpenInfo_operator);
	if (ServerStruct)
	{
		FairyType = SvrToCliFairyType(ServerStruct->fairy_type);
		StageId = ServerStruct->stage_id;
	}
	return *this;
}

void FB2EnhanceItemCost::UpdateByServerSync(const b2network::B2mdItemEnhanceCostPtr& ServerData)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FCollectionItemData_UpdateByServerSync);
	EnhanceLevel = ServerData->enhance_level;
	Rate = ServerData->enhance_rate;
	ScrollCost = ServerData->enhance_scroll_cost;
	ProtectionScrollCost = ServerData->protection_scroll_cost;
	MoneyCost = ServerData->money_cost;
	CostDarkCrystalForExchange = ServerData->exchange_cost_dark_crystal;
	CostGemCrystalForExchange = ServerData->exchange_cost_gem;
}

void FItemEnhanceEffectInfo::UpdateByServerSync(const b2network::B2mdItemEnhanceEffectInfoPtr& ServerData)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FCollectionItemData_UpdateByServerSync);
	SetEffect = ServerData->set_effect;
	AuraColor = FLinearColor(ServerData->channel_red,
		ServerData->channel_green,
		ServerData->channel_blue,
		ServerData->channel_alpha);
}

void FDuelModeExpMDSingleData::UpdateByServerSync(const b2network::B2mdDuelModeRewardPtr& InDataElem)
{
	//// 流立 曼炼侩篮 酒聪绊 捞 element 甫 甸绊 乐阑 甘狼 虐蔼. GameMode 客 MatchResult 滴 俺甫 炼钦茄促. 犬牢 瞒 鞘夸窍搁 荤侩.
	//KeyGameMode = SvrDuelModeToCliGameMode(InDataElem->duel_mode);
	//checkSlow(KeyGameMode != EB2GameMode::Unknown); // 父老 捞霸 吧府霸登搁 努扼 GameMode 俊 秦寸窍绰 货肺款 DuelMode 啊 沥狼登菌阑 淀.. SvrDuelModeToCliGameMode 甘俏 犬牢.
	//												//KeyMatchResult = SvrToCliNetMatchResult(InDataElem->match_result);

	//												//ExpectedExp = InDataElem->exp;
	//ExpectedExp = 0;
}

int32 FDuelModeExpMDSingleData::GetCombinedIntKey(EB2GameMode InGameMode, ENetMatchResult InResult)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FCollectionItemData_GetCombinedIntKey);
	return static_cast<int32>(InGameMode) * 10 + static_cast<int32>(InResult);
}

void FRaidExpMDSingleData::UpdateByServerSync(const b2network::B2mdRaidExpPtr& InDataElem)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FCollectionItemData_UpdateByServerSync);
	RaidType = InDataElem->raid_type;
	RaidStep = InDataElem->step; // 流立 曼炼侩篮 酒聪绊 捞 element 甫 甸绊 乐阑 甘狼 虐蔼. 犬牢 瞒 鞘夸窍搁 荤侩.
	ExpectedExp = InDataElem->exp;

	RaidExpKey = FRaidExpMDSingleData::GetKey(RaidType, RaidStep);
}

void FModRankerInfoContainer::SetRankerList(const TArray<b2network::B2MatchRankerPtr>& RankerList, int32 BeginRank)
{
	RankingList.Empty();

	for (auto RankerSvrInfo : RankerList)
	{
		FModRankerInfo RankerCliInfo;
		RankerCliInfo.UserName = RankerSvrInfo->nickname;
		RankerCliInfo.Grade = RankerSvrInfo->battle_grade;
		RankerCliInfo.GradeStar = RankerSvrInfo->battle_star_count;
		RankerCliInfo.Rank = BeginRank++;
		RankerCliInfo.TotalCombatPower = RankerSvrInfo->total_power;			// jwyim power
		RankerCliInfo.ClearTimeSec = RankerSvrInfo->clear_time;
		RankerCliInfo.BestFloor = RankerSvrInfo->best_floor;
		RankerCliInfo.AccountID = RankerSvrInfo->account_id;

		FModReplayInfo ReplayInfo;
		ReplayInfo.Url = RankerSvrInfo->pvp_replay_url;
		ReplayInfo.SizeCompressed = RankerSvrInfo->replay_size_compressed;
		ReplayInfo.SizeUncompressed = RankerSvrInfo->replay_size_uncompressed;

		RankerCliInfo.ReplayInfo = ReplayInfo;

		for (auto EntryInfo : RankerSvrInfo->entries)
		{
			RankerCliInfo.Entry.Add(SvrToCliPCClassType(EntryInfo.character_type));
			RankerCliInfo.EntryLevels.Add(EntryInfo.level);
		}

		RankingList.Add(RankerCliInfo);
	}
	OnRankingListChanged.Broadcast(RankingList);
}

const FMagicShopProduct* FClientDataStore::GetMagicShopProductData(int32 ProductID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMagicShopProductData);

	return MD_MagicShopProduct.Find(ProductID);
}

const FMagicShopRenewalCost* FClientDataStore::GetMagicShopRenewalCostData(int32 RenewalCount)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetMagicShopRenewalCostData);

	return MD_MagicShopRenewalCost.Find(RenewalCount);
}

const TArray<FSelectItemOption>* FClientDataStore::GetSelectItemOptionData(int32 EquipType, int32 Grade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSelectItemOptionData);

	FSelectItemOption NewItemOption;
	NewItemOption.EquipPosition = EquipType;
	NewItemOption.Grade = Grade;
	int32 KeyID = NewItemOption.GetMyKey();

	return MD_SelectItemOption.Find(KeyID);
}

FItemSurpassLevel FClientDataStore::GetSurpassLevelData(int32 InGrade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSurpassLevelData);

	for (auto SurpassLevel : MD_SurpassLevelData)
	{
		if (SurpassLevel.Grade == InGrade)
		{
			return SurpassLevel;
		}
	}

	return FItemSurpassLevel();
}

bool FClientDataStore::GetItemDismantleInfoData(EItemInvenType InvenType, int32 Grade, FItemDismantleInfo& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemDismantleInfoData);

	TArray<FItemDismantleInfo> DismantleInfos;
	DismantleInfos.Empty();

	MD_ItemDismantleInfo.MultiFind(InvenType, DismantleInfos);

	for (int32 i = 0; i < DismantleInfos.Num(); ++i)
	{
		if (DismantleInfos[i].Grade == Grade)
		{
			OutList = DismantleInfos[i];
			return true;
		}
	}

	return false;
}

int32 FClientDataStore::GetConvertTable(int32 beforeConvertItemID, FConvertTableInfo& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetConvertTable);

	for (int32 i = 0; i < MD_ExchangeSystemInfo.Num(); i++)
	{
		if (MD_ExchangeSystemInfo[i].beforeConvertItemID == beforeConvertItemID)
		{
			//OutList = MD_ExchangeSystemInfo[i];
			OutList.tableId = MD_ExchangeSystemInfo[i].tableId;
			OutList.beforeConvertItemID = MD_ExchangeSystemInfo[i].beforeConvertItemID;
			OutList.beforeConvertItemQuantity = MD_ExchangeSystemInfo[i].beforeConvertItemQuantity;
			OutList.afterConvertItemID = MD_ExchangeSystemInfo[i].afterConvertItemID;
			OutList.afterConvertItemQuantity = MD_ExchangeSystemInfo[i].afterConvertItemQuantity;
			return OutList.tableId;
		}
	}

	return -1;	//厚沥惑 搬苞.
}

bool FClientDataStore::GetConvertCost(int32 tableID, int32 count, FConvertCostInfo& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetConvertCost);

	for (int32 i = 0; i < MD_ExchangeSystemCost.Num(); ++i)
	{
		if (MD_ExchangeSystemCost[i].tableId == tableID)
		{
			if (MD_ExchangeSystemCost[i].count == count)
			{
				OutList = MD_ExchangeSystemCost[i];
				return true;
			}
		}
	}

	return false;
}

void FClientDataStore::GetQualityTable(TArray<FQualityTransRateInfo>& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetQualityTable);

	for (int32 i = 0; i < MD_QualityTransRateInfo.Num(); i++)
	{
		OutList.Add(MD_QualityTransRateInfo[i]);
	}
}

int32 FClientDataStore::GetMaxQuality(int64 hammerID)
{
	for (int32 i = MD_QualityTransRateInfo.Num() - 1; i >= 0; i--)
	{
		if (MD_QualityTransRateInfo[i].templateID == hammerID && MD_QualityTransRateInfo[i].transRate > 0)
		{
			return MD_QualityTransRateInfo[i].qualityValue;
		}
	}
	return 0;
}
int32 FClientDataStore::GetMinQuality(int64 hammerID)
{
	for (int32 i = 0; i < MD_QualityTransRateInfo.Num(); i++)
	{
		if (MD_QualityTransRateInfo[i].templateID == hammerID && MD_QualityTransRateInfo[i].transRate > 0)
		{
			return MD_QualityTransRateInfo[i].qualityValue;
		}
	}
	return 0;
}

void FClientDataStore::GetItemForgeTable(EItemForgeType forgeType_, EPCClass SelectedClass, TArray<FItemForgeInfo>& OutList, TArray<FItemForgePeriodicStatus>& periodicTable)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemForgeTable);

	for (int32 i = 0; i < MD_ItemForgeInfo.Num(); i++)
	{
		if (MD_ItemForgeInfo[i].forgeType == forgeType_ && (MD_ItemForgeInfo[i].visibleType == EItemForgeListVisibleType::Enabled || MD_ItemForgeInfo[i].visibleType == EItemForgeListVisibleType::Limited))
		{
			if (MD_ItemForgeInfo[i].visibleType == EItemForgeListVisibleType::Limited)
			{
				FDateTime DateNow = UB2GameInstance::GetLocalNow();

				for (int32 j = 0; j < periodicTable.Num(); j++)
				{
					if (MD_ItemForgeInfo[i].forgeID == periodicTable[j].forgeID)
					{
						FDateTime previewStart = periodicTable[j].previewTime;
						FDateTime ItemShowStart = periodicTable[j].startTime;
						FDateTime ItemShowEnd = periodicTable[j].endTime;

						if (DateNow.GetTicks() >= previewStart.GetTicks() && DateNow.GetTicks() <= ItemShowEnd.GetTicks())
						{
							if (DateNow.GetTicks() < ItemShowStart.GetTicks())
								MD_ItemForgeInfo[i].previewOnly = true;
							else
								MD_ItemForgeInfo[i].previewOnly = false;

							OutList.Add(MD_ItemForgeInfo[i]);
						}
					}
				}
			}
			else
			{
				OutList.Add(MD_ItemForgeInfo[i]);
			}
		}
	}
}

void FClientDataStore::GetItemForgeTable(EItemForgeType forgeType_, EPCClass SelectedClass, TArray<FItemForgeInfo>& OutList, TArray<FItemForgePeriodicStatus>& periodicTable, TArray<EItemInvenType> filterInven, TArray<int32> filterStarGrade)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemForgeTable);

	for (int32 i = 0; i < MD_ItemForgeInfo.Num(); i++)
	{
		if (MD_ItemForgeInfo[i].forgeType == forgeType_ && (MD_ItemForgeInfo[i].visibleType == EItemForgeListVisibleType::Enabled || MD_ItemForgeInfo[i].visibleType == EItemForgeListVisibleType::Limited))
		{
			for (int32 l = 0; l < filterInven.Num(); l++)
			{
				if(filterInven[l] == GetItemInvenTypeFromClass(FItemRefIDHelper::ExtractItemClassFromRefID(MD_ItemForgeInfo[i].resultItemID)))
				{
					if (filterInven[l] == EItemInvenType::EIIVT_End)
					{
						if (MD_ItemForgeInfo[i].visibleType == EItemForgeListVisibleType::Limited)
						{
							FDateTime DateNow = UB2GameInstance::GetLocalNow();

							for (int32 j = 0; j < periodicTable.Num(); j++)
							{
								if (MD_ItemForgeInfo[i].forgeID == periodicTable[j].forgeID)
								{
									FDateTime previewStart = periodicTable[j].previewTime;
									FDateTime ItemShowStart = periodicTable[j].startTime;
									FDateTime ItemShowEnd = periodicTable[j].endTime;

									if (DateNow.GetTicks() >= previewStart.GetTicks() && DateNow.GetTicks() <= ItemShowEnd.GetTicks())
									{
										if (DateNow.GetTicks() < ItemShowStart.GetTicks())
											MD_ItemForgeInfo[i].previewOnly = true;
										else
											MD_ItemForgeInfo[i].previewOnly = false;

										OutList.Add(MD_ItemForgeInfo[i]);
									}
								}
							}
						}
						else
						{
							OutList.Add(MD_ItemForgeInfo[i]);
						}
					}
					else {
						for (int32 k = 0; k < filterStarGrade.Num(); k++)
						{
							if (filterStarGrade[k] == FItemRefIDHelper::ExtractItemStarGradeFromRefID(MD_ItemForgeInfo[i].resultItemID))
							{
								if (MD_ItemForgeInfo[i].visibleType == EItemForgeListVisibleType::Limited)
								{
									FDateTime DateNow = UB2GameInstance::GetLocalNow();

									for (int32 j = 0; j < periodicTable.Num(); j++)
									{
										if (MD_ItemForgeInfo[i].forgeID == periodicTable[j].forgeID)
										{
											FDateTime previewStart = periodicTable[j].previewTime;
											FDateTime ItemShowStart = periodicTable[j].startTime;
											FDateTime ItemShowEnd = periodicTable[j].endTime;

											if (DateNow.GetTicks() >= previewStart.GetTicks() && DateNow.GetTicks() <= ItemShowEnd.GetTicks())
											{
												if (DateNow.GetTicks() < ItemShowStart.GetTicks())
													MD_ItemForgeInfo[i].previewOnly = true;
												else
													MD_ItemForgeInfo[i].previewOnly = false;

												OutList.Add(MD_ItemForgeInfo[i]);
											}
										}
									}
								}
								else
								{
									OutList.Add(MD_ItemForgeInfo[i]);
								}
							}
						}
					}
				}
			}
		}
	}
}

void FClientDataStore::GetItemForgeLimitTable(int32 limitID, FItemForgeLimitInfo& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemForgeLimitTable);

	for (int32 i = 0; i < MD_ItemForgeLimitInfo.Num(); i++)
	{
		if (MD_ItemForgeLimitInfo[i].limitID == limitID)
		{
			OutList = MD_ItemForgeLimitInfo[i];
		}
	}
}

void FClientDataStore::GetItemForgeMaterialTransInfo(int32 forgeID_, TArray<FItemForgeMaterialTransInfo>& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetItemForgeMaterialTransInfo);

	for (int32 i = 0; i < MD_ItemForgeMaterialTransInfo.Num(); i++)
	{
		if (MD_ItemForgeMaterialTransInfo[i].forgeID == forgeID_)
		{
			OutList.Add(MD_ItemForgeMaterialTransInfo[i]);
		}
	}
}

FSealBoxInfo FClientDataStore::GetSealBoxInfo(int32 sealboxRefID_)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSealboxInfo);

	for (int32 i = 0; i < MD_SealBoxInfo.Num(); i++)
	{
		if (sealboxRefID_ == MD_SealBoxInfo[i].sealboxRefID)
		{
			return MD_SealBoxInfo[i];
		}
	}

	FSealBoxInfo nullInfo;
	return nullInfo;
}

void FClientDataStore::GetSealBoxRewardRateInfo(int32 rewardID, TArray<FSealBoxRewardRateInfo>& OutList)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FClientDataStore_GetSealBoxRewardRateInfo);

	for (int32 i = 0; i < MD_SealBoxRewardRateInfo.Num(); i++)
	{
		if (MD_SealBoxRewardRateInfo[i].sealboxRewardID == rewardID)
		{
			OutList.Add(MD_SealBoxRewardRateInfo[i]);
		}
	}
}

bool FClientDataStore::DoesSealboxHaveEquipment(int32 rewardID)
{
	for (int32 i = 0; i < MD_SealBoxRewardRateInfo.Num(); i++)
	{
		if (MD_SealBoxRewardRateInfo[i].sealboxRewardID == rewardID)
		{
			if (MD_SealBoxRewardRateInfo[i].starGrade > 0)
				return true;
		}
	}

	return false;
}