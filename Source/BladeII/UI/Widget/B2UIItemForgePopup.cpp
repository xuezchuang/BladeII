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

	// 테이블 데이터 로드
	TArray<FItemForgeMaterialTransInfo> MD = BladeIIGameImpl::GetClientDataStore().MD_ItemForgeMaterialTransInfo;

	// Factory ID 세팅
	int32 TempFID = ItemForgeDataSet.FactoryID;

	// Factory ID를 기반으로 필터링
	TArray<FItemForgeMaterialTransInfo> MD_Filtered = MD.FilterByPredicate(
		[TempFID](FItemForgeMaterialTransInfo master_data)
	{
		return master_data.forgeID == TempFID;

	}
	);

	// Main Item Setting 기본적으로 Main Item이 필수지만 체크
	if (ItemForgeDataSet.MainItemRefID != 0 && ItemForgeDataSet.MainItemCount > 0)
	{
		TargetData MainItemData(ItemForgeDataSet.MainItemRefID, ItemForgeDataSet.MainItemCount * ItemForgeDataSet.MakeCount);
		ItemForgeDataSet.ResourceItem.Add(MainItemData);
	}

	// 필요한 데이터를 Slot 기반으로 추출
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
		// RefID 기반으로 ItemClass 추출
		EItemClass EIC = FItemRefIDHelper::ExtractItemClassFromRefIDForItemForge(ItemForgeDataSet.ResourceItem.GetData()->RefID);
		// ItemClass를 기반으로 InvenType 추출
		EItemInvenType EIIT = GetItemInvenTypeFromClass(EIC);

		//UE_LOG(LogTemp, Log, TEXT("EIC : %d, EIIT : %d"),(int)EIC,(int)EIIT);

		// IvenType이 소모품일 경우 "대체" 텍스트 장비일 경우 "선택" 텍스트
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

	// 테이블의 대체 아이템 기반으로 Icon 생성
	for (auto& ResourceItemElem : ItemForgeDataSet.ResourceItem)
	{
		UB2DynItemIcon_ItemForge* ResourceItemIcon = CreateWidget<UB2DynItemIcon_ItemForge>(GetOwningPlayer(), result);
		ResourceItemIcon->Initialize();
		ResourceItemIcon->NativeConstruct();

		// 재료로 보여주는 아이템이기 때문에 EIFI_TargetItem으로 설정
		ResourceItemIcon->InitItemForgeSlot(ItemForgeDataSet.SlotType, EItemForgeItemIconUsage::EIFI_TargetItem, ItemForgeDataSet.FactoryID, requiredItemEnhanceLevel,requiredItemLevel);

		// B2UI에서 UpdateItemData를 먼저 실행하기 때문에 무조건 Init 후 UpdateTargetItemData해야 함.
		ResourceItemIcon->UpdateTargetItemData(ResourceItemElem.RefID, ResourceItemElem.Count);
		SB_ItemTargets->AddChild(ResourceItemIcon);
	}

	//제작 중인 타입 별로 인벤 싱크를 맞춰 줘야 됨. 상위 Class에서 선언 해주는 것이 더 안전하다고 판단하여 상위에서 진행 함.
	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(ItemForgeDataSet.CharacterType);

	// 현재 캐릭터의 데이터 로드
	CachedCharacterStore = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	if (CachedCharacterStore == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CachedCharacterStroe Error!!!!!!!!!"));
	}

	// 유저 데이터 로드
	FPCItemContainer& UserItmes = CachedCharacterStore->GetUserAllItems();

	// 캐릭터 타입에 따라 인벤토리 로드
	auto& UserItemsByChar = UserItmes.CharacterItemContainers[PCClassToInt(ItemForgeDataSet.CharacterType)];

	for (auto& ResourceItemElem : ItemForgeDataSet.ResourceItem)
	{
		// 아이템 Type에 따라 저장할 Map
		TMap<int64, FB2Item>* ItemMap = nullptr;

		// RefID로 InvenType 추출
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

		// 현재 Resource RefID를 기반으로 같은 RefID 모두 로드
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


		// 장비중인 컨테이너 한 번 확인 AllEuippedInfo AllItemPresets
		// FindItem (UID)
		// 현재 프리셋의 장착된 아이템 중 refid가 같은걸 찾아서 출력 한다.
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
		// 코스튬 아이템은 제외 됐음. 만약 제작소에 코스튬이 들어갈 경우 코스튬도 장착 여부 파악 해야 됨.
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


	// itemforgedataset.holdlist 를 기반으로 add
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
	// 선택 or 대체 여부 판별 하 도록 만들어야됨.
	//TB_Title->SetText(FText::FromString(TEXT("선택")));
	//TB_Confirm->SetText(FText::FromString(TEXT("선택")));
	
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
	//	// 상위 패널 class에 넣어줄 선택한 아이템의 필요 개수를 넣어줘야됨으로 
	//	// ItemForgeDataSet에서 TArray 에서 perdicate으로 선택한 id와 refid가 같은 값의 count값을 돌려 주도록 할 것

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
	//		// selecteditem과 count 값 event system으로 세팅 해 주기.

	//		ItemForgeIngredientUI->SetAnotherMaterialItem(SelectedItem, Count);
	//	}



	//	OnClickBTN_Cancel(); //지우기 위해
	//}
	//else
	//{
	//	// 재료를 선택하십시오 팝업 뜨게
	//}
	
}
