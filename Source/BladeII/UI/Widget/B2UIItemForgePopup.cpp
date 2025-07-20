// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIItemForgePopup.h"
#include "B2DynItemIcon_ItemForge.h"
#include "B2UIItemForgeIngredientList.h"
#include "Event.h"
#include "BladeIIGameImpl.h"
#include "B2ClientDataStore.h"
#include "B2LobbyInventory.h"

#include "B2LobbyGameMode.h"
#include "B2LobbySceneManager.h"




void UB2UIItemForgePopup::Init()
{
	Super::Init();
	//DataSetInInven.SetDataSet(1	, EItemForgeSlot::MainMaterialItem);
	//DataSetInInven.HoldList.Empty();

	SubscribeEvent();
}


void UB2UIItemForgePopup::InitializeForgePopup(EPCClass CharType,int32 FID, EItemForgeSlot SType,int32 Makecount, int32 MainItem,int32 MainItemCount,class UB2UIItemForgeIngredientList* MainUI, int32 enhanceLevel, int32 itemLevel)
{
	ItemForgeIngredientUI = MainUI;
	ItemForgeDataSet.SetDataSet(CharType,FID, SType,Makecount,MainItem,MainItemCount);
	ItemForgeDataSet.HoldList.Empty();

	CachedCharacterStore = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	requiredItemEnhanceLevel = enhanceLevel;
	requiredItemLevel = itemLevel;

	Init();
}

void UB2UIItemForgePopup::SubscribeEvent()
{
	

	//if (bSubscribed == false)
	//{
	//	UnSubscribeEvent();

	//
	//	Issues.Add(DeliverySelectItemForgeClass<FB2Item&>::GetInstance().Subscribe2(
	//		[this](FB2Item& SelectItem)
	//	{
	//		this->SelectedItem = SelectItem;
	//		//UE_LOG(LogTemp, Log, TEXT("Checking Log : %d"), this->SelectedItem.ItemRefID);

	//		for (int i = 0; i < this->SB_Item->GetChildrenCount(); i++)
	//		{
	//			UB2DynItemIcon_ItemForge* icon =  Cast<UB2DynItemIcon_ItemForge>(this->SB_Item->GetChildAt(i));
	//			icon->SetOV_SelectedVisibility(ESlateVisibility::Hidden);
	//		}
	//		
	//		//UE_LOG(LogTemp, Log, TEXT("Event System :: RefID :%d UID : %d"),SelectedItem.ItemRefID,SelectedItem.InstanceUID);

	//		BTN_Select->SetIsEnabled(true);
	//	}
	//	));

	//	bSubscribed = true;
	//}

}

void UB2UIItemForgePopup::UnSubscribeEvent()
{
	DeliverySelectItemForgeClass<FB2Item&>::GetInstance().Unsubscribe(Itemforge_SelectTicket);
}

void UB2UIItemForgePopup::CacheAssets()
{
	Super::CacheAssets();

	/*for (auto& elem : OutList)
	{
		UE_LOG(LogTemp, Log, TEXT("RefID : %d, UID : %d"), elem.ItemRefID,elem.InstanceUID);
	}*/

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UB2ScrollBox, SB_Item);
	GET_SLOT(UB2ScrollBox, SB_ItemTargets);
	GET_SLOT(UB2Button, BTN_Cancel);
	GET_SLOT(UB2Button, BTN_Select);
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIItemForgePopup::OnClickBTNSelect);
	GET_SLOT(UTextBlock, TB_Confirm);

	// 抛捞喉 单捞磐 肺靛
	TArray<FItemForgeMaterialTransInfo> MD = BladeIIGameImpl::GetClientDataStore().MD_ItemForgeMaterialTransInfo;

	// Factory ID 技泼
	int32 TempFID = ItemForgeDataSet.FactoryID;

	// Factory ID甫 扁馆栏肺 鞘磐傅
	TArray<FItemForgeMaterialTransInfo> MD_Filtered = MD.FilterByPredicate(
		[TempFID](FItemForgeMaterialTransInfo master_data)
	{
		return master_data.forgeID == TempFID;

	}
	);

	// Main Item Setting 扁夯利栏肺 Main Item捞 鞘荐瘤父 眉农
	if (ItemForgeDataSet.MainItemRefID != 0 && ItemForgeDataSet.MainItemCount > 0)
	{
		TargetData MainItemData(ItemForgeDataSet.MainItemRefID, ItemForgeDataSet.MainItemCount * ItemForgeDataSet.MakeCount);
		ItemForgeDataSet.ResourceItem.Add(MainItemData);
	}

	// 鞘夸茄 单捞磐甫 Slot 扁馆栏肺 眠免
	for (auto& MD_Raw : MD_Filtered)
	{
		int32 TempRefID = 0;
		int32 TempCount = 0;
		switch (ItemForgeDataSet.SlotType)
		{
		case EItemForgeSlot::MainMaterialItem:
		{
			TempRefID = MD_Raw.mainMaterialItemID;
			TempCount = MD_Raw.mainMaterialItemQuantity;
			break;
		}

		case EItemForgeSlot::Sub1stMaterialItem:
		{
			TempRefID = MD_Raw.material1stItemID;
			TempCount = MD_Raw.material1stItemQuantity;
			break;
		}

		case EItemForgeSlot::Sub2ndMaterialItem:
		{
			TempRefID = MD_Raw.material2ndItemID;
			TempCount = MD_Raw.material2ndItemQuantity;
			break;
		}

		case EItemForgeSlot::Sub3rdMaterialItem:
		{
			TempRefID = MD_Raw.material3rdItemID;
			TempCount = MD_Raw.material3rdItemQuantity;
			break;
		}

		case EItemForgeSlot::Sub4thMaterialItem:
		{
			TempRefID = MD_Raw.material4thItemID;
			TempCount = MD_Raw.material4thItemQuantity;
			break;
		}

		default:
			UE_LOG(LogTemp, Error, TEXT("EItemForgeSlot Not Setted"));
			break;
		}

		if (TempRefID == 0)
			continue;

		TempCount *= ItemForgeDataSet.MakeCount;

		TargetData TempTargetData(TempRefID, TempCount);

		ItemForgeDataSet.ResourceItem.Add(TempTargetData);
	}

	if (ItemForgeDataSet.ResourceItem.Num() > 0)
	{
		// RefID 扁馆栏肺 ItemClass 眠免
		EItemClass EIC = FItemRefIDHelper::ExtractItemClassFromRefIDForItemForge(ItemForgeDataSet.ResourceItem.GetData()->RefID);
		// ItemClass甫 扁馆栏肺 InvenType 眠免
		EItemInvenType EIIT = GetItemInvenTypeFromClass(EIC);

		//UE_LOG(LogTemp, Log, TEXT("EIC : %d, EIIT : %d"),(int)EIC,(int)EIIT);

		// IvenType捞 家葛前老 版快 "措眉" 咆胶飘 厘厚老 版快 "急琶" 咆胶飘
		if (EIIT == EItemInvenType::EIIVT_Consumables)
		{
			TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemForgePopUpText_Replacement")));
		}
		else
		{
			TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Select")));
		}
	}

	/*UE_LOG(LogTemp, Log, TEXT("MD"));
	for (auto& t : MD)
	{
		UE_LOG(LogTemp, Log, TEXT("%d %d %d"), t.forgeID, t.mainMaterialItemID, t.mainMaterialItemQuantity);
	}

	UE_LOG(LogTemp, Log, TEXT("Temptest %d"), TempFID);
	for (auto& t : temptest)
	{
		UE_LOG(LogTemp, Log, TEXT("%d %d %d"),t.forgeID,t.mainMaterialItemID,t.mainMaterialItemQuantity);
	}
	*/

	// 抛捞喉狼 措眉 酒捞袍 扁馆栏肺 Icon 积己
	for (auto& ResourceItemElem : ItemForgeDataSet.ResourceItem)
	{
		UB2DynItemIcon_ItemForge* ResourceItemIcon = CreateWidget<UB2DynItemIcon_ItemForge>(GetOwningPlayer(), result);
		ResourceItemIcon->Initialize();
		ResourceItemIcon->NativeConstruct();

		// 犁丰肺 焊咯林绰 酒捞袍捞扁 锭巩俊 EIFI_TargetItem栏肺 汲沥
		ResourceItemIcon->InitItemForgeSlot(ItemForgeDataSet.SlotType, EItemForgeItemIconUsage::EIFI_TargetItem, ItemForgeDataSet.FactoryID, requiredItemEnhanceLevel,requiredItemLevel);

		// B2UI俊辑 UpdateItemData甫 刚历 角青窍扁 锭巩俊 公炼扒 Init 饶 UpdateTargetItemData秦具 窃.
		ResourceItemIcon->UpdateTargetItemData(ResourceItemElem.RefID, ResourceItemElem.Count);
		SB_ItemTargets->AddChild(ResourceItemIcon);
	}

	//力累 吝牢 鸥涝 喊肺 牢亥 教农甫 嘎苗 拎具 凳. 惑困 Class俊辑 急攫 秦林绰 巴捞 歹 救傈窍促绊 魄窜窍咯 惑困俊辑 柳青 窃.
	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(ItemForgeDataSet.CharacterType);

	// 泅犁 某腐磐狼 单捞磐 肺靛
	CachedCharacterStore = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	if (CachedCharacterStore == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CachedCharacterStroe Error!!!!!!!!!"));
	}

	// 蜡历 单捞磐 肺靛
	FPCItemContainer& UserItmes = CachedCharacterStore->GetUserAllItems();

	// 某腐磐 鸥涝俊 蝶扼 牢亥配府 肺靛
	auto& UserItemsByChar = UserItmes.CharacterItemContainers[PCClassToInt(ItemForgeDataSet.CharacterType)];

	for (auto& ResourceItemElem : ItemForgeDataSet.ResourceItem)
	{
		// 酒捞袍 Type俊 蝶扼 历厘且 Map
		TMap<int64, FB2Item>* ItemMap = nullptr;

		// RefID肺 InvenType 眠免
		EItemClass EIC = FItemRefIDHelper::ExtractItemClassFromRefIDForItemForge(ResourceItemElem.RefID);
		EItemInvenType EIIT = GetItemInvenTypeFromClass(EIC);
		/*UE_LOG(LogTemp, Log, TEXT("HoldList Checking RefID : %d EItemClass: %d EItemInvenType: %d"),
			ResourceItemElem.RefID,(int)EIC,(int)EIIT);*/

		switch (EIIT)
		{
		case EItemInvenType::EIIVT_Weapon:
		{
			ItemMap = &(UserItemsByChar.StoredWeapons);
			break;
		}
		case EItemInvenType::EIIVT_Protection:
		{
			ItemMap = &(UserItemsByChar.StoredProtections);
			break;
		}
		case EItemInvenType::EIIVT_Accessory:
		{
			ItemMap = &(UserItemsByChar.StoredAccessries);
			break;
		}
		case EItemInvenType::EIIVT_Consumables:
		{
			ItemMap = &(UserItmes.ConsumableItems);
			break;
		}
		case EItemInvenType::EIIVT_CostumeWeapon:
		{
			ItemMap = &(UserItemsByChar.StoredCostumeWeapons);
			break;
		}

		case EItemInvenType::EIIVT_CostumeArmor:
		{
			ItemMap = &(UserItemsByChar.StoredCostumeArmors);
			break;
		}

		default:
			UE_LOG(LogTemp, Log, TEXT("ItemMap Load Error : ItemForge // Input Data : %d"), (int)EIIT);

		}

		TArray<int32> tempUIDs;

		// 泅犁 Resource RefID甫 扁馆栏肺 鞍篮 RefID 葛滴 肺靛
		if (ItemMap != nullptr)
		{
			for (auto& StoredItemElem : *ItemMap)
			{
				if (StoredItemElem.Value.ItemRefID == ResourceItemElem.RefID)
				{
					if (StoredItemElem.Value.Level >= requiredItemLevel && StoredItemElem.Value.EnhanceLevel >= requiredItemEnhanceLevel)
					{
						ItemForgeDataSet.HoldList.Add(StoredItemElem.Value);
						if (StoredItemElem.Value.bIsLocked == true)
							tempUIDs.Add(StoredItemElem.Value.InstanceUID);

						//UE_LOG(LogTemp, Log, TEXT("RefID : %d UID : %d Added"), StoredItemElem.Value.ItemRefID, StoredItemElem.Value.InstanceUID);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Item Map Nullptr"));
		}


		// 厘厚吝牢 牧抛捞呈 茄 锅 犬牢 AllEuippedInfo AllItemPresets
		// FindItem (UID)
		// 泅犁 橇府悸狼 厘馒等 酒捞袍 吝 refid啊 鞍篮吧 茫酒辑 免仿 茄促.
		bool alreadyFound = false;
		for (auto& elem : UserItemsByChar.AllItemPresets)
		{
			for (auto& elem2 : elem.Value.EquippedItemID)
			{
				UE_LOG(LogTemp, Log, TEXT("key(slotID) %d /// uid : %d"), elem.Key, elem2);
				FB2Item* tempas = UserItemsByChar.FindItem(elem2);
				if (tempas != nullptr)
				{
					if (tempas->ItemRefID == ResourceItemElem.RefID)
					{
						for (int32 i = 0; i < tempUIDs.Num(); i++)
						{
							if (tempas->InstanceUID == tempUIDs[i]) continue;

							ItemForgeDataSet.HoldList.Add((*tempas));
							UE_LOG(LogTemp, Log, TEXT("RefID %d"), tempas->ItemRefID);
						}


						if (tempUIDs.Num() == 0)
						{
							ItemForgeDataSet.HoldList.Add((*tempas));
							tempUIDs.Add(tempas->InstanceUID);
							UE_LOG(LogTemp, Log, TEXT("RefID %d"), tempas->ItemRefID);
						}
					}
				}
			}
		}
	}

	bool IsConsumableItem = false;
	{
		EItemClass EIC = FItemRefIDHelper::ExtractItemClassFromRefIDForItemForge(ItemForgeDataSet.MainItemRefID);
		EItemInvenType EIIT = GetItemInvenTypeFromClass(EIC);
		if (EIIT == EItemInvenType::EIIVT_Consumables) 
			IsConsumableItem = true;
	}
	

	if (!IsConsumableItem)
	{
		// 内胶片 酒捞袍篮 力寇 灯澜. 父距 力累家俊 内胶片捞 甸绢哎 版快 内胶片档 厘馒 咯何 颇厩 秦具 凳.
		TArray<const FB2Item*> EquipedItems;
		{
			TArray<int64> UIDs;

			for (auto& Preset : UserItemsByChar.AllItemPresets)
			{
				for (auto& UID : Preset.Value.EquippedItemID)
				{
					if (UID > 0)
					{
						UIDs.AddUnique(UID);
					}
				}
			}

			for (auto& UID : UIDs)
			{
				const FB2Item* EquipedItem = UserItemsByChar.FindItem(UID);
				if (EquipedItem != nullptr)
				{
					EquipedItems.Add(EquipedItem);
				}
			}
		}
	}


	// itemforgedataset.holdlist 甫 扁馆栏肺 add
	for (auto& HoldListElem : ItemForgeDataSet.HoldList)
	{
		UB2DynItemIcon_ItemForge* DynItemIconItemForge = CreateWidget<UB2DynItemIcon_ItemForge>(GetOwningPlayer(), result);
		DynItemIconItemForge->Initialize();
		DynItemIconItemForge->NativeConstruct();
		DynItemIconItemForge->InitItemForgeSlot(ItemForgeDataSet.SlotType, EItemForgeItemIconUsage::EIFI_InIvenItem, ItemForgeDataSet.FactoryID,requiredItemEnhanceLevel,requiredItemLevel);
		DynItemIconItemForge->UpdateItemData(HoldListElem);

		SB_Item->AddChild(DynItemIconItemForge);			
	}

	BTN_Select->SetIsEnabled(false);
	
}

void UB2UIItemForgePopup::BindDelegates()
{
	Super::BindDelegates();
	//UE_LOG(LogTemp, Log, TEXT("Check bind"));
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2UIItemForgePopup::OnClickBTN_Cancel);
}

void UB2UIItemForgePopup::UpdateStaticText()
{
	Super::UpdateStaticText();
	// 急琶 or 措眉 咯何 魄喊 窍 档废 父甸绢具凳.
	//TB_Title->SetText(FText::FromString(TEXT("急琶")));
	//TB_Confirm->SetText(FText::FromString(TEXT("急琶")));
	
}

void UB2UIItemForgePopup::OnClickBTN_Cancel()
{
	UnSubscribeEvent();
	RemoveFromParent();
}

void UB2UIItemForgePopup::OnClickBTNSelect()
{
	//if (!SelectedItem.IsEmpty())
	//{
	//	//UE_LOG(LogTemp, Log, TEXT("select btn clicked"));
	//	// 惑困 菩澄 class俊 持绢临 急琶茄 酒捞袍狼 鞘夸 俺荐甫 持绢拎具凳栏肺 
	//	// ItemForgeDataSet俊辑 TArray 俊辑 perdicate栏肺 急琶茄 id客 refid啊 鞍篮 蔼狼 count蔼阑 倒妨 林档废 且 巴

	//	bool CheckEnd = false;
	//	int32 Count = -1;
	//	for (auto& elem : ItemForgeDataSet.ResourceItem)
	//	{
	//		if (CheckEnd)
	//			break;

	//		if (elem.RefID == SelectedItem.ItemRefID)
	//		{
	//			CheckEnd = true;
	//			Count = elem.Count;
	//		}
	//	}


	//	if (Count < 0)
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("ItemForge Select Button Count Error"));
	//	}
	//	else
	//	{
	//		if (ItemForgeDataSet.MakeCount != 0)
	//		{
	//			Count /= ItemForgeDataSet.MakeCount;
	//		}
	//		
	//		
	//		// UE_LOG(LogTemp, Log, TEXT("UID : %d, Count : %d"),SelectedItem.InstanceUID,Count);
	//		// selecteditem苞 count 蔼 event system栏肺 技泼 秦 林扁.

	//		ItemForgeIngredientUI->SetAnotherMaterialItem(SelectedItem, Count);
	//	}



	//	OnClickBTN_Cancel(); //瘤快扁 困秦
	//}
	//else
	//{
	//	// 犁丰甫 急琶窍绞矫坷 扑诀 哆霸
	//}
	
}
