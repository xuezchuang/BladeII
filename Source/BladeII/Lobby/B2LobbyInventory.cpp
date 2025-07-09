// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
//#include "B2UIManager_Lobby.h"
#include "B2NetworkBridge.h"
#include "Event.h"
#include "B2UIDocHelper.h"
#include "OnlineWholesaler.h"
#include "Retailer.h"
#include "BladeIIPlayer.h"
#include "B2PCClassInfo.h"
//#include "B2DynItemIcon.h"
//#include "B2AcqItemEquipSuggest.h"
//#include "B2LobbyUI_SellItemPopup.h"
//#include "B2UIDecompositionObtainPopup.h"
//#include "B2UIInventorySetInfoNotify.h"
#include "B2CombatStatEvaluator.h"
#include "B2Airport.h"
//#include "B2UIShortagePopupManager.h"
#include "TutorialManager.h"
#include "Algo/Copy.h"
//#include "B2UIClientIndicator.h"
#include "B2ItemSortInfo.h"
#include "B2UIUtil.h"
//#include "B2LobbyUI_InventoryMain.h"
//#include "B2UI_TotemInfoPopup.h"


#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"
//#include "B2UILevelupAutoSelect.h"
#include "B2MessageInfoCommonStruct.h"
//#include "B2UIItemChangePopupMode.h"
//#include "B2UIItemOptionChange.h"
//#include "B2UIItemQualityChange.h"
//#include "B2UISealBoxResult.h"
//#include "B2UISealBoxResultSimple.h"
//#include "B2LobbyUI_ItemHeroChangeMain.h"
//#include "B2UIBackWidgetBase.h"
#include "B2UnitedWidgetBase.h"
#include "B2ItemInfo.h"
//#include "B2UIHeader.h"

//#include "B2LobbyUI_ConvertItemPopup.h"

FPerCharacterItemContainer* FInvenPCItemAccessor::GetMyItemStore()
{
	auto& DataStore = BladeIIGameImpl::GetLocalCharacterData();

	int32 StoreArrayIndex = PCClassToInt(MyPCClass);
	if (StoreArrayIndex < DataStore.GetUserAllItems().CharacterItemContainers.Num())
	{
		return &(DataStore.GetUserAllItems().CharacterItemContainers[StoreArrayIndex]);
	}
	return NULL;
}

FEquippedItemInfo* FInvenPCItemAccessor::GetMyEquippedStore()
{
	FPerCharacterItemContainer* MyContainer = GetMyItemStore();
	return MyContainer ? MyContainer->AllEuippedInfo : NULL;
}

const TMap<int32, FEquippedItemInfo>* FInvenPCItemAccessor::GetMyEquippedCostumeStore()
{
	FPerCharacterItemContainer* MyContainer = GetMyItemStore();
	return MyContainer ? &(MyContainer->AllEuippedCostumeInfo) : NULL;
}

int32 FInvenPCItemAccessor::GetAllItemMapCount()
{
	FPerCharacterItemContainer* MyContainer = GetMyItemStore();	
	return MyContainer ? MyContainer->StoredWeapons.Num() + MyContainer->StoredProtections.Num() + MyContainer->StoredAccessries.Num() : 0;
}

TMap<int64, FB2Item>* FInvenPCItemAccessor::GetItemMapForInvenType(EItemInvenType InItemInvenType)
{
	FPerCharacterItemContainer* MyContainer = GetMyItemStore();
	if (!MyContainer)
		return NULL;

	switch (InItemInvenType)
	{
	case EItemInvenType::EIIVT_Weapon: return &(MyContainer->StoredWeapons);
	case EItemInvenType::EIIVT_Protection: return &(MyContainer->StoredProtections);
	case EItemInvenType::EIIVT_Accessory: return &(MyContainer->StoredAccessries);
	case EItemInvenType::EIIVT_CostumeWeapon:	return &(MyContainer->StoredCostumeWeapons);
	case EItemInvenType::EIIVT_CostumeArmor:	return &(MyContainer->StoredCostumeArmors);
	}
	return NULL;
}

bool FInvenPCItemAccessor::FindEquippedItem(FB2Item& OutFound, EItemEquipPlace InEquipPlace)
{
	FEquippedItemInfo* AllEuippedInfo = GetMyEquippedStore();
	if (!AllEuippedInfo)
		return false;

	if (InEquipPlace < EItemEquipPlace::EIEP_End)
	{
		if (AllEuippedInfo[(int32)InEquipPlace].bIsEquipped)
		{
			OutFound = AllEuippedInfo[(int32)InEquipPlace].EquippedItem;
			return true;
		}
	}
	return false;
}

bool FInvenPCItemAccessor::FindEquippedCostume(FB2Item& OutFound, ECostumeEquipPlace InEquipPlace)
{
	if (InEquipPlace >= ECostumeEquipPlace::ECEP_End)
		return false;

	const TMap<int32, FEquippedItemInfo>* pEquippedCostumeInfo = GetMyEquippedCostumeStore();
	if (!pEquippedCostumeInfo)
		return false;

	for (auto& Elem : *pEquippedCostumeInfo)
	{
		const FEquippedItemInfo& EquippedCostumeItemInfo = Elem.Value;

		if (EquippedCostumeItemInfo.EquippedItem.CostumeEquipPlace == InEquipPlace && EquippedCostumeItemInfo.bIsEquipped)
		{
			OutFound = EquippedCostumeItemInfo.EquippedItem;
			return true;
		}
	}
	return false;
}

bool FInvenPCItemAccessor::FindEquippedItemByID(FB2Item& OutFound, int64 InstanceUID)
{
	FEquippedItemInfo* AllEuippedInfo = GetMyEquippedStore();
	if (!AllEuippedInfo)
		return false;

	for (int32 EI = 0; EI < (int32)EItemEquipPlace::EIEP_End; ++EI)
	{
		// 설령 InstanceUID 가 맞다고 하더라도 Equipped 되지 않은 상태라면 valid 하지 않을 것.
		if (AllEuippedInfo[EI].EquippedItem.InstanceUID == InstanceUID && AllEuippedInfo[EI].bIsEquipped)
		{
			OutFound = AllEuippedInfo[EI].EquippedItem;
			return true;
		}
	}
	return false;
}

FB2Item* FInvenPCItemAccessor::FindEquippedItemPtrByID(int64 InstanceUID)
{
	FEquippedItemInfo* AllEuippedInfo = GetMyEquippedStore();
	if (!AllEuippedInfo)
		return NULL;

	for (int32 EI = 0; EI < (int32)EItemEquipPlace::EIEP_End; ++EI)
	{
		// 설령 InstanceUID 가 맞다고 하더라도 Equipped 되지 않은 상태라면 valid 하지 않을 것.
		if (AllEuippedInfo[EI].EquippedItem.InstanceUID == InstanceUID && AllEuippedInfo[EI].bIsEquipped)
		{
			return &(AllEuippedInfo[EI].EquippedItem);
		}
	}
	return NULL;
}

bool FInvenPCItemAccessor::IsEquipped(EItemEquipPlace InEquipPlace)
{
	FEquippedItemInfo* AllEuippedInfo = GetMyEquippedStore();
	if (!AllEuippedInfo)
		return false;

	if (InEquipPlace < EItemEquipPlace::EIEP_End)
	{
		return AllEuippedInfo[(int32)InEquipPlace].bIsEquipped;
	}
	return false;
}

void FInvenPCItemAccessor::GetAllEquippedItems(TArray<FB2Item>& OutList)
{
	FEquippedItemInfo* AllEuippedInfo = GetMyEquippedStore();
	if (AllEuippedInfo)
	{
		for (int32 EI = 0; EI < (int32)EItemEquipPlace::EIEP_End; ++EI)
		{
			if (AllEuippedInfo[EI].bIsEquipped)
			{
				OutList.Add(AllEuippedInfo[EI].EquippedItem);
			}
		}
	}
}

void FInvenPCItemAccessor::GetAllEquippedItems(TArray<FB2Item>& OutList, EItemInvenType InItemInvenType)
{
	bool UnChecked = InItemInvenType == EItemInvenType::EIIVT_End;
	FEquippedItemInfo* AllEuippedInfo = GetMyEquippedStore();
	if (AllEuippedInfo)
	{
		for (int32 EI = 0; EI < (int32)EItemEquipPlace::EIEP_End; ++EI)
		{
			if (AllEuippedInfo[EI].bIsEquipped)
			{
				if(UnChecked || AllEuippedInfo[EI].EquippedItem.InventoryType == InItemInvenType)
					OutList.Add(AllEuippedInfo[EI].EquippedItem);
			}
		}
	}
}

void FInvenPCItemAccessor::GetAllEquippedCostumeItems(TArray<FB2Item>& OutList)
{
	const TMap<int32, FEquippedItemInfo>* pEquippedCostumeInfo = GetMyEquippedCostumeStore();
	if (pEquippedCostumeInfo)
	{
		for (auto& Elem : *pEquippedCostumeInfo)
		{
			const FEquippedItemInfo& EquippedCostumeItemInfo = Elem.Value;

			if (EquippedCostumeItemInfo.bIsEquipped)
			{
				OutList.Add(EquippedCostumeItemInfo.EquippedItem);
			}
		}
	}
}

AB2LobbyGameMode* UB2LobbyInventory::TransientLobbyGM = NULL;
TArray<FInvenPCItemAccessor> UB2LobbyInventory::PCItems;
int32 UB2LobbyInventory::ItemLevelupIngredLimit = 20; // @TODO BII_SERVER 걍 정해진 값인데 서버에서 오거나..

UB2LobbyInventory::UB2LobbyInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_None;
	ItemOpTargetItemID = 0;
	PendingEnhanceProtectionItemRefID = 0;
	bFreezeitemOpModeIngredSelection = false;
	OnCommitItemOpHeroMgmtPCBackup = EPCClass::EPC_End;
	OnCommitItemOpInvenPageTabBackup = EItemInvenType::EIIVT_End;
	CustomBackupInvenPageTab = EItemInvenType::EIIVT_End;
	CurrentBaseItemOpGoldCost = 0;
	CurrentUpgradeStoneCost = 0;
	CurrentEnhanceItemSuccessRate = 0;
	CurrentEnhanceItemScrollCost = 0;
	CurrentEnhanceItemProtectionScrollCost = 0;
	MainInvenViewCurrentTab = EItemInvenType::EIIVT_Weapon;
	AutoItemLevelupPhase = EAutoItemLevelupPhase::AEPhase_None;
	AutoItemLevelupTotalCost = 0;
	//CreatedUIP_CharSelectPopup = nullptr;
#if WITH_EDITOR
	bSelectedNoForDummyDataSuggestion = false;
	bItemOpProgressSceneTestMode = false;
#endif
}

TMap<int64, FB2Item>& UB2LobbyInventory::GetConsumablesList()
{
	return BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().ConsumableItems;
}

void UB2LobbyInventory::BeginDestroy()
{
	TransientLobbyGM = NULL;

	ResetAllIngredsList(); // 일반적인 게임 플로우라면 필요는 없을 건데 에디터의 경우등 확실히 비워주면 좋을 것.
	ResetEnhanceItemDatas();
	GhostItemOpIngreds.Empty();

	UnsubscribeEvents();

	Super::BeginDestroy();
}

void UB2LobbyInventory::StaticInit()
{
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		PCItems.Add(FInvenPCItemAccessor(IntToPCClass(PCI))); // 각 PC 별로 DataStore 에 접근하는 객체를 만들어 두어야 함.
	}
}

void UB2LobbyInventory::InitLobbyInventory(class AB2LobbyGameMode* InLobbyGM)
{
	TransientLobbyGM = InLobbyGM;
	check(TransientLobbyGM); // 객체 생성은 LobbyGameMode 에서만

	SubscribeEvents();
}

void UB2LobbyInventory::SubscribeEvents()
{
	if (bEventSubscribed)
		return;

	CAPTURE_UOBJECT(UB2LobbyInventory);

	// 서버 요청 이벤트들..
	LobbyReqEquipItemsTicket = LobbyReqEquipItemsClass<const TArray<int64>&>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqEquipItems, const TArray<int64>& i)
			Capture->ReqForEquipItems(i);
		END_CAPTURE_OBJECT()
	);
	LobbyReqUnEquipItemsTicket = LobbyReqUnEquipItemsClass<const TArray<int64>&>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqUnEquipItems, const TArray<int64>& i)
			Capture->ReqForUnEquipItems(i);
		END_CAPTURE_OBJECT()
	);
	LobbyReqUnEquipItemsOtherPresetTicket = LobbyReqUnEquipItemsOtherPresetClass<int32, const TArray<int64>&>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqUnEquipItemsOtherPreset, const int32 PresetID, const TArray<int64>& i)
		Capture->ReqForUnEquipItems(PresetID, i);
		END_CAPTURE_OBJECT()
	);

	LobbyReqToggleItemLockTicket = LobbyReqToggleItemLockClass<int64, bool>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqToggleItemLock, int64 i, bool bLock)
			Capture->ReqForToggleItemLock(i, bLock);
		END_CAPTURE_OBJECT()
	);
	LobbyReqSellItemTicket= LobbyReqSellItemClass<int64, bool>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqSellItem, int64 i, bool bIsTotem)
			Capture->ReqForSellItem(i, bIsTotem);
		END_CAPTURE_OBJECT()
	);
	LobbyReqSellItemsTicket= LobbyReqSellItemsClass<const TArray<int64>&, bool>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqSellItems, const TArray<int64>& ia, bool bIsTotem)
			Capture->ReqForSellItems(ia, bIsTotem);
		END_CAPTURE_OBJECT()
	);
	LobbyReqSellConsumableItemTicket = LobbyReqSellConsumableItemClass<int64, int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqSellConsumableItem, int64 i, int32 j)
			Capture->ReqForSellComsumableItem(i, j);
		END_CAPTURE_OBJECT()
	);

	LobbyReqConvertDataTicket = LobbyReqConvertDataClass<int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqConvertData, int32 templateId_)
			Capture->ReqForConvertData(templateId_);
	END_CAPTURE_OBJECT()
	);
	LobbyReqConvertItemTicket = LobbyReqConvertItemClass<int32, int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqConvertItem, int32 id, int32 convertType_)
			Capture->ReqForConvertItem(id, convertType_);
	END_CAPTURE_OBJECT()
	);

	LobbyReqDecompositionItemsTicket = LobbyReqDecompositionItemsClass<const TArray<int64>&, bool>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqDecompositionItems, const TArray<int64>& ia, bool bIsTotem)
		Capture->ReqForDecompositionItems(ia, bIsTotem);
	END_CAPTURE_OBJECT()
	);
	LobbyReqItemLevelupWithCurrentSelectionTicket= LobbyReqItemLevelupWithCurrentSelectionClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqItemLevelupWithCurrentSelection)
			Capture->ReqForItemLevelupWithCurrentSelection();
		END_CAPTURE_OBJECT()
	);
	
	LobbyReqEnhanceItemTicket = LobbyReqEnhanceItemClass<>::GetInstance().Subscribe(
	USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqEnhanceItem)
		Capture->ReqForEnhanceItem();
	END_CAPTURE_OBJECT()
		);

	LobbyReqUpgradeItemTicket= LobbyReqUpgradeItemClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqUpgradeItem)
			Capture->ReqForUpgradeItem();
		END_CAPTURE_OBJECT()
	);
	LobbyReqComposeItemWithCurrentSelectionTicket= LobbyReqComposeItemWithCurrentSelectionClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqComposeItemWithCurrentSelection)
			Capture->ReqForComposeItemWithCurrentSelection();
		END_CAPTURE_OBJECT()
	);
	LobbyReqSurpassItemWithCurrentSelectionTicket= LobbyReqSurpassItemWithCurrentSelectionClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqSurpassItemWithCurrentSelection)
			Capture->ReqForSurpassItemWithCurrentSelection();
		END_CAPTURE_OBJECT()
	);
	LobbyReqAutoItemLevelupTicket = LobbyReqAutoItemLevelupClass<const TArray<FB2Item>&, bool>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqAutoItemLevelup, const TArray<FB2Item>& items, bool b)
		Capture->BeginAutoLevelupItem(items, b);
		END_CAPTURE_OBJECT()
	);
	LobbyReqSmeltingTotemTicket = LobbyReqSmeltingTotemClass<int64, int32, int32, bool>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyReqSmeltingTotem, int64 InstanceID, int32 ServerOptionID, int32 ServerRefineCostType, bool LastRefine)
		Capture->ReqForSmeltingTotem(InstanceID, ServerOptionID, ServerRefineCostType, LastRefine);
	END_CAPTURE_OBJECT()
	);

	// 서버에서 온 응답을 이곳이 아닌 ClientDataStore 에서 직접 처리할 때 여기로 보내는 이벤트
	LobbyAllItemsAddedTicket= LobbyAllItemsAddedClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyAllItemsAdded)
			Capture->OnAllItemsAddedAtStore();
		END_CAPTURE_OBJECT()
	);
	LobbyItemsAddedTicket= LobbyItemsAddedClass<const FB2AddedItems&>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyItemsAdded, const FB2AddedItems& items)
			Capture->OnItemsAddedAtStore(items);
		END_CAPTURE_OBJECT()
	);
	EnterItemLevelupModeTicket= EnterItemLevelupModeClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(EnterItemLevelupMode, int64 i)
			Capture->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_Levelup, i);
		END_CAPTURE_OBJECT()
	);
	EnterItemUpgradeModeTicket= EnterItemUpgradeModeClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(EnterItemUpgradeMode, int64 i)
			Capture->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_Upgrade, i);
		END_CAPTURE_OBJECT()
	);
	EnterItemCompositionModeTicket= EnterItemCompositionModeClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(EnterItemCompositionMode, int64 i)
			Capture->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_Composition, i);
		END_CAPTURE_OBJECT()
	);
	EnterItemSurpassModeTicket= EnterItemSurpassModeClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(EnterItemSurpassMode, int64 i)
			Capture->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_Surpass, i);
		END_CAPTURE_OBJECT()
	);
	EnterItemEnhanceModeTicket = EnterItemEnhanceModeClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(EnterItemEnhanceMode, int64 i)
		// Backup을 해준다.
		Capture->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_Enhancement, i, EItemInvenType::EIIVT_Consumables, true);
		END_CAPTURE_OBJECT()
		);
	EnterItemCostumeEnhanceModeTicket = EnterItemCostumeEnhanceModeClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(EnterItemCostumeEnhanceMode, int64 i)
		Capture->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume, i, EItemInvenType::EIIVT_Consumables, false);
		Capture->OnReqForItemOpCommon();

	END_CAPTURE_OBJECT()
		);
	EnterTotemSmeltingModeTicket = EnterTotemSmeltingModeClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(EnterTotemSmeltingMode, int64 i)
		Capture->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_TotemSmelting, i);
	END_CAPTURE_OBJECT()
		);
	LobbyInventoryViewJustOpenTicket= LobbyInventoryViewJustOpenClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyInventoryViewJustOpen)
			Capture->OnLobbyInventoryViewOpenUp();
		END_CAPTURE_OBJECT()
	);
	LobbyInventoryViewJustCloseTicket= LobbyInventoryViewJustCloseClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyInventoryViewJustClose)
			Capture->OnLobbyInventoryViewClose();
		END_CAPTURE_OBJECT()
	);
	QuitItemOpModeTicket= QuitItemOpModeClass<bool>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyInventoryViewJustClose, bool b)
			Capture->QuitItemOpMode(b);
		END_CAPTURE_OBJECT()
	);
	SelectForItemLevelupIngredTicket= SelectForItemLevelupIngredClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SelectForItemLevelupIngred, int64 i)
			Capture->SelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_Levelup);
		END_CAPTURE_OBJECT()
	);
	UnSelectForItemLevelupIngredTicket= UnSelectForItemLevelupIngredClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UnSelectForItemLevelupIngred, int64 i)
			Capture->UnSelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_Levelup);
		END_CAPTURE_OBJECT()
	);	
	SelectForComposeIngredTicket= SelectForComposeIngredClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SelectForComposeIngred, int64 i)
			Capture->SelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_Composition);
		END_CAPTURE_OBJECT()
	);
	UnSelectForComposeIngredTicket= UnSelectForComposeIngredClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UnSelectForComposeIngred, int64 i)
			Capture->UnSelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_Composition);
		END_CAPTURE_OBJECT()
	);
	SelectForSurpassIngredTicket= SelectForSurpassIngredClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SelectForSurpassIngred, int64 i)
			Capture->SelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_Surpass);
		END_CAPTURE_OBJECT()
	);
	UnSelectForSurpassIngredTicket= UnSelectForSurpassIngredClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UnSelectForSurpassIngred, int64 i)
			Capture->UnSelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_Surpass);
		END_CAPTURE_OBJECT()
	);
	SelectForEnhanceIngredTicket = SelectForEnhanceIngredClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SelectForEnhanceIngred, int64 i)
		Capture->SelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_Enhancement);
	END_CAPTURE_OBJECT()
		);
	UnSelectForEnhanceIngredTicket = UnSelectForEnhanceIngredClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UnSelectForEnhanceIngred, int64 i)
		Capture->UnSelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_Enhancement);
	END_CAPTURE_OBJECT()
		);

	SelectForEnhanceChangeIngredTicket = SelectForEnhanceChangeIngredClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SelectForEnhanceChangeIngred, int64 i)
		Capture->SelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_EnhancementChange);
	END_CAPTURE_OBJECT()
		);
	UnSelectForEnhanceChangeIngredTicket = UnSelectForEnhanceChangeIngredClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UnSelectForEnhanceChangeIngred, int64 i)
		Capture->UnSelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_EnhancementChange);
	END_CAPTURE_OBJECT()

		);
	SelectForEnhanceProtectionItemTicket = SelectForEnhanceProtectionItemClass<int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SelectForEnhanceProtectionItem, int32 i)
		Capture->SetSelectEnhanceProtectionItemRefId(i);
	END_CAPTURE_OBJECT()
		);

	SelectForItemHammerTicket = SelectForItemHammerClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SelectForItemHammer, int64 i)
		Capture->SelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_Hammer);
	END_CAPTURE_OBJECT()
		);
	UnSelectForItemHammerTicket = UnSelectForItemHammerClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UnSelectForItemHammer, int64 i)
		Capture->UnSelectForIngred(i, ELobbyInvenItemOpMode::EIVIOP_Hammer);
	END_CAPTURE_OBJECT()
		);

	DeliveryEquipItemTicket= DeliveryEquipItemClass<FB2EquippedItems>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryEquipItem, const FB2EquippedItems& EquipedItems)
			Capture->EquippedItems(EquipedItems);
		END_CAPTURE_OBJECT()
	);
	DeliveryUnequipItemTicket= DeliveryUnequipItemClass<FB2UnequippedItems>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryUnequipItem, const FB2UnequippedItems& UnEquippedItems)
			Capture->UnEquippedItems(UnEquippedItems);
		END_CAPTURE_OBJECT()
	);
	DeliveryLockItemTicket= DeliveryLockItemClass<FB2LockedItems>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryLockItem, const FB2LockedItems& LockedItems)
			Capture->LockedItems(LockedItems);
		END_CAPTURE_OBJECT()
	);
	DeliveryUnlockItemTicket= DeliveryUnlockItemClass<FB2UnlockedItems>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryUnlockItem, const FB2UnlockedItems& UnlockedItems)
			Capture->UnlockedItems(UnlockedItems);
		END_CAPTURE_OBJECT()
	);
	DeliverySellItemTicket = DeliverySellItemClass<FB2SoldItems>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliverySellItem, const FB2SoldItems& SoldItems)
			Capture->ResponseSellItems(SoldItems);
		END_CAPTURE_OBJECT()
	);
	DeliverySellItemErrorTicket = DeliverySellItemErrorClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliverySellItemError)
			Capture->SellItemsEndLoop();
		END_CAPTURE_OBJECT()
	);
	DeliverySellConsumableItemTicket = DeliverySellConsumableItemClass<FB2SellConsumableItem>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliverySellConsumableItem, const FB2SellConsumableItem& SellConsumableItem)
			Capture->SellConsumableItem(SellConsumableItem);
		END_CAPTURE_OBJECT()
	);
	DeliveryConvertDataTicket = DeliveryConvertDataClass<FB2ConvertData>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryConvertData, const FB2ConvertData& ConvertData)
		Capture->ResponseConvertData(ConvertData);
	END_CAPTURE_OBJECT()
		);
	DeliveryConvertItemTicket = DeliveryConvertItemClass<FB2ConvertItem>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryConvertData, const FB2ConvertItem& ConvertItem)
		Capture->ResponseConvertItem(ConvertItem);
	END_CAPTURE_OBJECT()
		);
	DeliveryDismantleItemsTicket = DeliveryDismantleItemsClass<FB2DismantleItems>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryDismantleItems, const FB2DismantleItems& DismanttleItems)
			Capture->ResponseDismantleItems(DismanttleItems);
		END_CAPTURE_OBJECT()
	);
	DeliveryDismantleItemsErrorTicket = DeliveryDismantleItemsErrorClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryDismantleItemsError)
			Capture->DismantleItemsEndLoop();
		END_CAPTURE_OBJECT()
	);
	DeliveryItemLevelupTicket= DeliveryItemLevelupClass<FB2LevelupItem>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryItemLevelup, const FB2LevelupItem& LevelupItemInfo)
			Capture->LevelUpItem(LevelupItemInfo);
		END_CAPTURE_OBJECT()
	);
	DeliveryEnhanceItemTicket = DeliveryEnhanceItemClass<FB2EnhanceItem>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryEnhanceItem, const FB2EnhanceItem& EnhancedItemInfo)
			Capture->EnhanceItem(EnhancedItemInfo);
	END_CAPTURE_OBJECT()
		);
	DeliverySurpassItemTicket= DeliverySurpassItemClass<FB2SurpassItem>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliverySurpassItem, const FB2SurpassItem& SurpassedItemInfo)
			Capture->SurpassedItem(SurpassedItemInfo);
		END_CAPTURE_OBJECT()
	);
	DeliveryUpgradeItemTicket= DeliveryUpgradeItemClass<FB2UpgradeItem>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryUpgradeItem, const FB2UpgradeItem& UpgradedItemInfo)
			Capture->UpgradedItem(UpgradedItemInfo);
		END_CAPTURE_OBJECT()
	);
	DeliveryComposeItemTicket= DeliveryComposeItemClass<FB2ComposeItem>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryComposeItem, const FB2ComposeItem& ComposedItemInfo)
			Capture->ComposedItem(ComposedItemInfo);
		END_CAPTURE_OBJECT()
	);
	DeliveryGetItemUpgradeCostTicket= DeliveryGetItemUpgradeCostClass<FB2GetItemUpgradeCost>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetItemUpgradeCost, const FB2GetItemUpgradeCost& UpgradeCost)
			Capture->ItemUpgradeCostDelivered(UpgradeCost);
		END_CAPTURE_OBJECT()
	);
	DeliveryGetItemComposeCostTicket= DeliveryGetItemComposeCostClass<FB2GetItemComposeCost>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetItemComposeCost, const FB2GetItemComposeCost& ComposeCost)
			Capture->ItemComposeCostDelivered(ComposeCost);
		END_CAPTURE_OBJECT()
	);
	DeliveryGetItemSurpassCostTicket= DeliveryGetItemSurpassCostClass<FB2GetItemSurpassCost>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetItemSurpassCost, const FB2GetItemSurpassCost& SurpassCost)
			Capture->ItemSurpassCostDelivered(SurpassCost);
		END_CAPTURE_OBJECT()
	);

	SelectForLevelUpGradeTicket = SelectForLevelUpGradeClass<int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SelectForLevelUpGrade, const int32 StarGrade)
			Capture->BeginAutoSelectGradeItems(StarGrade);
		END_CAPTURE_OBJECT()
		);
	UpdateItemPresetTicket = UpdateItemPresetClass<EPCClass, EItemPresetSlotType>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UpdateItemPreset, const EPCClass InPCClass, const EItemPresetSlotType InPresetSlot)
		Capture->ChangePreset(InPCClass, InPresetSlot);
		END_CAPTURE_OBJECT()
		);

	Issues.Add(DeliveryEquipCostumeClass<FB2EquipCostumePtr>::GetInstance().Subscribe2(
	[Capture](const FB2EquipCostumePtr& EquipCostumePtr)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseEquipCostumeItems(EquipCostumePtr);
		}
	}
	));
	Issues.Add(DeliveryUnequipCostumeClass<FB2UnequipCostumePtr>::GetInstance().Subscribe2(
		[Capture](const FB2UnequipCostumePtr& UnEquipCostumePtr)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseUnEquipCostumeItems(UnEquipCostumePtr);
		}
	}
	));

	Issues.Add(DeliveryGetChangeItemCharacterTypeCostClass<FB2ResponseGetChangeItemCharacterTypeCostPtr>::GetInstance().Subscribe2(
		[Capture](const FB2ResponseGetChangeItemCharacterTypeCostPtr& GetChangeItemCharacterTypeCostPtr)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseGetChangeItemCharacterTypeCost(GetChangeItemCharacterTypeCostPtr);
		}
	}
	));
	
	Issues.Add(LobbyReqToggleTotemLockClass<int64, bool>::GetInstance().Subscribe2(
		[Capture](int64 InstanceID, bool bLock)
	{
		if (Capture.IsValid())
		{
			Capture->ReqForToggleTotemLock(InstanceID, bLock);
		}
	}
	));

	Issues.Add(DeliveryLockTotemsClass<FB2LockTotemsPtr>::GetInstance().Subscribe2(
		[Capture](const FB2LockTotemsPtr& LockTotemsPtr)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseLockTotems(LockTotemsPtr);
		}
	}
	));
	Issues.Add(DeliveryUnlockTotemsClass<FB2UnlockTotemsPtr>::GetInstance().Subscribe2(
		[Capture](const FB2UnlockTotemsPtr& UnlockTotemsPtr)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseUnlockTotems(UnlockTotemsPtr);
		}
	}
	));
	Issues.Add(DeliverySellTotemsClass<FB2SellTotemsPtr>::GetInstance().Subscribe2(
		[Capture](const FB2SellTotemsPtr& SellTotemsPtr)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseSellTotems(SellTotemsPtr);
		}
	}
	));
	Issues.Add(DeliveryDismantleTotemsClass<FB2DismantleTotemsPtr>::GetInstance().Subscribe2(
		[Capture](const FB2DismantleTotemsPtr& DismantleTotemsPtr)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseDismantleTotems(DismantleTotemsPtr);
		}
	}
	));
	Issues.Add(DeliveryRefineTotemClass<FB2RefineTotemPtr>::GetInstance().Subscribe2(
		[Capture](const FB2RefineTotemPtr& RefineTotemPtr)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseRefineTotem(RefineTotemPtr);
		}
	}
	));
	Issues.Add(LobbyReqSmeltingCompleteTotemClass<>::GetInstance().Subscribe2(
		[Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->ShowTotemRefineOpFx();
		}
	}
	));
	Issues.Add(DeliveryAddTotemToInventoryClass<FB2AddTotemToInventoryPtr>::GetInstance().Subscribe2(
		[Capture](const FB2AddTotemToInventoryPtr& AddTotemToInventoryPtr)
	{
		if (Capture.IsValid())
		{
			Capture->ResponseAddTotemToInventory(AddTotemToInventoryPtr);
		}
	}
	));
	Issues.Add(DeliveryEnhanceCostumeClass<FB2ResponseEnhanceCostumePtr>::GetInstance().Subscribe2(
		[Capture](const FB2ResponseEnhanceCostumePtr& msg)
	{
		if (Capture.IsValid())
		{
			Capture->CostumeEnhanceItem(msg);
		}
	}
	));

	Issues.Add(SealBoxSelectCharClass<int32>::GetInstance().Subscribe2(
		[this](int32 PCClass)
	{
		this->ResponseSealBoxSelectChar(PCClass);
	}
	));

	Issues.Add(DeliveryUnsealBoxClass<FB2ResponseUnsealBoxPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseUnsealBoxPtr& UnsealBoxPtr)
	{
		this->ResponseUnsealBox(UnsealBoxPtr);
	}
	));

	bEventSubscribed = true;
}

void UB2LobbyInventory::UnsubscribeEvents()
{
	if (!bEventSubscribed)
		return;

	Issues.Empty();

	LobbyReqEquipItemsClass<const TArray<int64>&>::GetInstance().Unsubscribe(LobbyReqEquipItemsTicket);
	LobbyReqUnEquipItemsClass<const TArray<int64>&>::GetInstance().Unsubscribe(LobbyReqUnEquipItemsTicket);
	LobbyReqUnEquipItemsOtherPresetClass<int32, const TArray<int64>&>::GetInstance().Unsubscribe(LobbyReqUnEquipItemsOtherPresetTicket);
	LobbyReqToggleItemLockClass<int64, bool>::GetInstance().Unsubscribe(LobbyReqToggleItemLockTicket);
	LobbyReqSellItemClass<int64, bool>::GetInstance().Unsubscribe(LobbyReqSellItemTicket);
	LobbyReqSellItemsClass<const TArray<int64>&, bool>::GetInstance().Unsubscribe(LobbyReqSellItemsTicket);
	LobbyReqDecompositionItemsClass<const TArray<int64>&, bool>::GetInstance().Unsubscribe(LobbyReqDecompositionItemsTicket);
	LobbyReqSellConsumableItemClass<int64, int32>::GetInstance().Unsubscribe(LobbyReqSellConsumableItemTicket);
	LobbyReqConvertDataClass<int32>::GetInstance().Unsubscribe(LobbyReqConvertDataTicket);
	LobbyReqConvertItemClass<int32, int32>::GetInstance().Unsubscribe(LobbyReqConvertItemTicket);
	LobbyReqItemLevelupWithCurrentSelectionClass<>::GetInstance().Unsubscribe(LobbyReqItemLevelupWithCurrentSelectionTicket);	
	LobbyReqEnhanceItemClass<>::GetInstance().Unsubscribe(LobbyReqEnhanceItemTicket);
	LobbyReqUpgradeItemClass<>::GetInstance().Unsubscribe(LobbyReqUpgradeItemTicket);
	LobbyReqComposeItemWithCurrentSelectionClass<>::GetInstance().Unsubscribe(LobbyReqComposeItemWithCurrentSelectionTicket);
	LobbyReqSurpassItemWithCurrentSelectionClass<>::GetInstance().Unsubscribe(LobbyReqSurpassItemWithCurrentSelectionTicket);
	LobbyReqAutoItemLevelupClass<const TArray<FB2Item>&, bool>::GetInstance().Unsubscribe(LobbyReqAutoItemLevelupTicket);
	LobbyReqSmeltingTotemClass<int64, int32, int32, bool>::GetInstance().Unsubscribe(LobbyReqSmeltingTotemTicket);
	LobbyAllItemsAddedClass<>::GetInstance().Unsubscribe(LobbyAllItemsAddedTicket);
	EnterItemLevelupModeClass<int64>::GetInstance().Unsubscribe(EnterItemLevelupModeTicket);
	EnterItemUpgradeModeClass<int64>::GetInstance().Unsubscribe(EnterItemUpgradeModeTicket);
	EnterItemCompositionModeClass<int64>::GetInstance().Unsubscribe(EnterItemCompositionModeTicket);
	EnterItemSurpassModeClass<int64>::GetInstance().Unsubscribe(EnterItemSurpassModeTicket);
	EnterItemEnhanceModeClass<int64>::GetInstance().Unsubscribe(EnterItemEnhanceModeTicket);
	EnterItemCostumeEnhanceModeClass<int64>::GetInstance().Unsubscribe(EnterItemCostumeEnhanceModeTicket);
	EnterTotemSmeltingModeClass<int64>::GetInstance().Unsubscribe(EnterTotemSmeltingModeTicket);
	LobbyInventoryViewJustOpenClass<>::GetInstance().Unsubscribe(LobbyInventoryViewJustOpenTicket);
	LobbyInventoryViewJustCloseClass<>::GetInstance().Unsubscribe(LobbyInventoryViewJustCloseTicket);
	QuitItemOpModeClass<bool>::GetInstance().Unsubscribe(QuitItemOpModeTicket);
	SelectForItemLevelupIngredClass<int64>::GetInstance().Unsubscribe(SelectForItemLevelupIngredTicket);
	UnSelectForItemLevelupIngredClass<int64>::GetInstance().Unsubscribe(UnSelectForItemLevelupIngredTicket);
	SelectForComposeIngredClass<int64>::GetInstance().Unsubscribe(SelectForComposeIngredTicket);
	UnSelectForComposeIngredClass<int64>::GetInstance().Unsubscribe(UnSelectForComposeIngredTicket);
	SelectForSurpassIngredClass<int64>::GetInstance().Unsubscribe(SelectForSurpassIngredTicket);
	UnSelectForSurpassIngredClass<int64>::GetInstance().Unsubscribe(UnSelectForSurpassIngredTicket);
	SelectForEnhanceIngredClass<int64>::GetInstance().Unsubscribe(SelectForEnhanceIngredTicket);
	UnSelectForEnhanceIngredClass<int64>::GetInstance().Unsubscribe(UnSelectForEnhanceIngredTicket);
	SelectForItemHammerClass<int64>::GetInstance().Unsubscribe(SelectForItemHammerTicket);
	UnSelectForItemHammerClass<int64>::GetInstance().Unsubscribe(UnSelectForItemHammerTicket);
	DeliveryEquipItemClass<FB2EquippedItems>::GetInstance().Unsubscribe(DeliveryEquipItemTicket);
	DeliveryUnequipItemClass<FB2UnequippedItems>::GetInstance().Unsubscribe(DeliveryUnequipItemTicket);
	DeliveryLockItemClass<FB2LockedItems>::GetInstance().Unsubscribe(DeliveryLockItemTicket);
	DeliveryUnlockItemClass<FB2UnlockedItems>::GetInstance().Unsubscribe(DeliveryUnlockItemTicket);
	DeliverySellItemClass<FB2SoldItems>::GetInstance().Unsubscribe(DeliverySellItemTicket);
	DeliverySellItemErrorClass<>::GetInstance().Unsubscribe(DeliverySellItemErrorTicket);
	DeliverySellConsumableItemClass<FB2SellConsumableItem>::GetInstance().Unsubscribe(DeliverySellConsumableItemTicket);
	DeliveryConvertDataClass<FB2ConvertData>::GetInstance().Unsubscribe(LobbyReqConvertDataTicket);
	DeliveryConvertItemClass<FB2ConvertItem>::GetInstance().Unsubscribe(LobbyReqConvertItemTicket);
	DeliveryDismantleItemsClass<FB2DismantleItems>::GetInstance().Unsubscribe(DeliveryDismantleItemsTicket);
	DeliveryDismantleItemsErrorClass<>::GetInstance().Unsubscribe(DeliveryDismantleItemsErrorTicket);
	DeliveryItemLevelupClass<FB2LevelupItem>::GetInstance().Unsubscribe(DeliveryItemLevelupTicket);
	DeliveryEnhanceItemClass<FB2EnhanceItem>::GetInstance().Unsubscribe(DeliveryEnhanceItemTicket);	
	DeliverySurpassItemClass<FB2SurpassItem>::GetInstance().Unsubscribe(DeliverySurpassItemTicket);
	DeliveryUpgradeItemClass<FB2UpgradeItem>::GetInstance().Unsubscribe(DeliveryUpgradeItemTicket);
	DeliveryComposeItemClass<FB2ComposeItem>::GetInstance().Unsubscribe(DeliveryComposeItemTicket);
	DeliveryGetItemUpgradeCostClass<FB2GetItemUpgradeCost>::GetInstance().Unsubscribe(DeliveryGetItemUpgradeCostTicket);
	DeliveryGetItemComposeCostClass<FB2GetItemComposeCost>::GetInstance().Unsubscribe(DeliveryGetItemComposeCostTicket);
	DeliveryGetItemSurpassCostClass<FB2GetItemSurpassCost>::GetInstance().Unsubscribe(DeliveryGetItemSurpassCostTicket);
	SelectForLevelUpGradeClass<int32>::GetInstance().Unsubscribe(SelectForLevelUpGradeTicket);
	UpdateItemPresetClass<EPCClass, EItemPresetSlotType>::GetInstance().Unsubscribe(UpdateItemPresetTicket);
	bEventSubscribed = false;
}

void UB2LobbyInventory::OnLobbyInventoryViewOpenUp()
{
	// UI lobby inventory 페이지 오픈 시 처리할 것들. Init 때와는 다르다.
}

void UB2LobbyInventory::OnLobbyInventoryViewClose()
{
	// UI lobby inventory 페이지 닫을 시 처리할 것들. Destroy 때와는 다르다.

	QuitItemOpMode(false);
}

void UB2LobbyInventory::OnAllItemsAddedAtStore()
{ 
#if WITH_EDITOR
	// 비어 있을 경우 테스트에 편하도록 더미 데이터 생성해주는 대화창을 열어줌.
	//CheckForGenerateDummyDataSuggestion(); 아 됐다. 서버 초기화가 자주 되어서 은근 불편. 필요하면 알아서 치트키 문의해서 쓰던지 하겠지..
#endif
	// 이 경우 데이터 핸들링은 ClientDataStore 에서 된 상태로 여기서는 UI 쪽 업데이트 신호만 날려준다.
	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
}

void UB2LobbyInventory::OnItemsAddedAtStore(const FB2AddedItems& AddedItems)
{
	// 이 경우 데이터 핸들링은 ClientDataStore 에서 된 상태로 여기서는 UI 쪽 업데이트 신호만 날려준다.
	// 개별 아이템 업데이트 신호는 있으나 추가하는 것에 대해서는 아님. 모두 업데이트.
	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
}

bool UB2LobbyInventory::IsIngredsAllItemEquiped(EPCClass PCClass, EItemInvenType InvenType)
{
	TArray<int32> StarGradeFilter;
	const int32 GradeMax = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
	for (int32 Grade = 1; Grade <= GradeMax; ++Grade)
	{
		StarGradeFilter.Add(Grade);
	}

	TArray<EItemInvenType> InvenTypeFilter;
	InvenTypeFilter.Add(InvenType);

	TArray<FB2Item> OutFilteredList;
	// 장착, 잠금, 프리셋 되어 있는거 제외.
	GetFilteredItemList(OutFilteredList, false, false, false, InvenTypeFilter, StarGradeFilter);

	return OutFilteredList.Num() < 1;
}

bool UB2LobbyInventory::IsIngredsAllItemLock(EPCClass PCClass, EItemInvenType InvenType)
{
	TArray<int32> StarGradeFilter;
	const int32 GradeMax = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
	for (int32 Grade = 1; Grade <= GradeMax; ++Grade)
	{
		StarGradeFilter.Add(Grade);
	}

	TArray<EItemInvenType> InvenTypeFilter;
	InvenTypeFilter.Add(InvenType);

	TArray<FB2Item> OutFilteredList;
	// 잠금, 프리셋 되어 있는거 제외.
	GetFilteredItemList(OutFilteredList, false, false, false, InvenTypeFilter, StarGradeFilter);

	return OutFilteredList.Num() < 1;
}

bool UB2LobbyInventory::HasIngredsSameGrade(EPCClass PCClass, EItemInvenType InvenType)
{
	TArray<EItemInvenType> InvenTypeFilter;
	InvenTypeFilter.Add(InvenType);

	const int32 GradeMax = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
	for (int32 Grade = 1; Grade <= GradeMax; ++Grade)
	{
		TArray<int32> StarGradeFilter;
		StarGradeFilter.Add(Grade);

		TArray<FB2Item> OutFilteredList;
		// 장착 및 잠금, 프리셋 되어 있는거 제외.
		GetFilteredItemList(OutFilteredList, false, false, false, InvenTypeFilter, StarGradeFilter);

		if (OutFilteredList.Num() > 1)
			return true;
	}

	return false;
}

bool UB2LobbyInventory::HasPossibleLevelupItem(EPCClass PCClass, EItemInvenType InvenType)
{
	TArray<int32> StarGradeFilter;
	const int32 GradeMax = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
	for (int32 Grade = 1; Grade <= GradeMax; ++Grade)
	{
		StarGradeFilter.Add(Grade);
	}

	TArray<EItemInvenType> InvenTypeFilter;
	InvenTypeFilter.Add(InvenType);

	TArray<FB2Item> OutFilteredList;
	// 장착 및 잠금, 프리셋 되어 있는거 제외.
	GetFilteredItemList(OutFilteredList, false, false, false, InvenTypeFilter, StarGradeFilter);

	return OutFilteredList.Num() > 0;
}

bool UB2LobbyInventory::IsIngredsAllMaxLevel(EPCClass PCClass, EItemInvenType InvenType)
{
	TArray<int32> StarGradeFilter;
	const int32 GradeMax = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
	for (int32 Grade = 1; Grade <= GradeMax; ++Grade)
	{
		StarGradeFilter.Add(Grade);
	}

	TArray<EItemInvenType> InvenTypeFilter;
	InvenTypeFilter.Add(InvenType);

	TArray<FB2Item> OutFilteredList;
	// 장착 및 잠금, 프리셋 되어 있는거 제외.
	GetFilteredItemList(OutFilteredList, false, false, false, InvenTypeFilter, StarGradeFilter);

	for (const FB2Item &Each : OutFilteredList)
	{
		if (Each.Level < Each.MaxLevel)
			return false;
	}

	return true;
}

int32 UB2LobbyInventory::HaveEnoughHammer(const FB2Item& TargetItem) const
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	TArray<FQualityTransRateInfo> QualityRateTable;
	CliDataStore.GetQualityTable(QualityRateTable);

	int64 tempID = 0;
	int32 amount = 0;

	for (int32 i = 0; i < QualityRateTable.Num(); i++)
	{
		if (TargetItem.Quality >= QualityRateTable[i].transCondQuality)
		{
			if (tempID != QualityRateTable[i].templateID)
			{
				tempID = QualityRateTable[i].templateID;
				amount += GetSharedConsumableAmountOfType(tempID);
			}
		}
	}
	return amount;
}
int32 UB2LobbyInventory::HowMannyHammer(int64 HammerID) const
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	TArray<FQualityTransRateInfo> QualityRateTable;
	CliDataStore.GetQualityTable(QualityRateTable);

	return GetSharedConsumableAmountOfType(HammerID);
}

#if !UE_BUILD_SHIPPING
void UB2LobbyInventory::DevOnlyGenerateDummyItemData(bool bRequestToServer)
{
	// 기존에 여기에 추가했던 bRequestToServer 가 더 이상 안먹히는 관계로 DevOnlyRequestDummyItems 를 대신 사용
	if (bRequestToServer)
	{
		DevOnlyRequestDummyItems(30, 1000100, 0.3f);
		return;
	}

	if (!bRequestToServer) // 클라이언트 전용 기능의 경우, 인벤토리 기능 확인 차원이니 기존 것들 비우자.
	{
		for (int32 IPI = 0; IPI < GetMaxPCClassNum(); ++IPI)
		{
			// 실제 ClientDataStore 에 있는 데이터를 제거하는 것으로서 실제 기능에는 이런 코드를 넣으면 안됨.
			TMap<int64, FB2Item>* WeaponMap = PCItems[IPI].GetItemMapForInvenType(EItemInvenType::EIIVT_Weapon);
			if (WeaponMap)
				WeaponMap->Empty();
			TMap<int64, FB2Item>* ProtectionMap = PCItems[IPI].GetItemMapForInvenType(EItemInvenType::EIIVT_Protection);
			if (ProtectionMap)
				ProtectionMap->Empty();
			TMap<int64, FB2Item>* AccessoryMap = PCItems[IPI].GetItemMapForInvenType(EItemInvenType::EIIVT_Accessory);
			if (AccessoryMap)
				AccessoryMap->Empty();
		}
		GetConsumablesList().Empty();
	}

	for (int32 IPI = 0; IPI < GetMaxPCClassNum(); ++IPI)
	{
		// 구질구질하지만 테스트 기능이니 그냥 간다.
		EPCClass CurrPCClass = ItemAllowedPCClassEnum(IPI);

		// 여기서 ItemRefID 숫자 채워넣는 로직은 단순히 구현 당시 더미 데이터에 맞춘 것임.

		// 아래 더할 아이템 중에서 장착할 거 몇개를 무작위로 빼 온다. 서버에 요청하는 상황이면 안될 것.
		TArray<int64> IDsToEquip;

		const int WeaponNum = FMath::RandRange(24, 36); // 전체 개수
		const int WeaponEquipIndex = FMath::RandRange(0, WeaponNum - 1); // 그 중 장착할 거 인덱스
		for (int32 NI = 0; NI < WeaponNum; ++NI)
		{
			const int32 StarGrade = (NI % 3) + 1;

			FB2Item NewWeapon;
			NewWeapon.InstanceUID = 1230000 + IPI * 1000 + NI; // 사용되는 상황에서 다른 아이템은 없을 것이므로 여기서만이라도 겹치지 않게 적당히 넣어줌
			// 자리수 별로 의미가 있음. 어차피 지금은 더미 데이터지만 맞춰본다.
			NewWeapon.ItemRefID = 1000000 + 100000 * IPI + 0 + 100 * StarGrade;

			if (bRequestToServer) // 서버에 요청한다면 지금까지 정보로 충분함. 서버 테스트 커맨드 사용.
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewWeapon.ItemRefID, StarGrade));
				continue;
			}

			// 이하는 진짜 가라 데이터 낄낄
			DevOnlyLoadItemClientTestData(NewWeapon);
			NewWeapon.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;
			NewWeapon.SetPrimaryPoint((float)StarGrade * 100.0f + FMath::FRandRange(0.0f, 100.0f));
			NewWeapon.Level = FMath::RandRange(1, NewWeapon.MaxLevel - 1); // 강화 레벨은 보여지는 중요 정보 중 하나이니 무작위로 넣어주자 ㅋ
			NewWeapon.Quality = FMath::RandRange(1, MAX_ITEM_QUALITY); // 품질도 보여지는 정보.
			
			// 특정 조건에서만 사용 가능한 합성, 승급, 초월 테스트를 위해 특별 수치 세팅.
			if (FMath::RandRange(1, 10) == 1)
			{
				NewWeapon.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewWeapon.Level = NewWeapon.MaxLevel;
			}
			else if (FMath::RandRange(1, 10) == 1)
			{ // 초월 재료
				NewWeapon.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewWeapon.Level = 1;
			}
			else if (FMath::RandRange(1, 4) == 1)
			{
				NewWeapon.Level = NewWeapon.MaxLevel;
			}

			// 능력치 옵션 적당히 넣어줌 ㅋ
			int32 IntrinsicOptionNum = FMath::RandRange(0, MAX_ITEM_INTRINSIC_OPTION);
			int32 AllOptionRange = (int32)EItemOption::EIO_Skill_IncSkill5Damage - (int32)EItemOption::EIO_Offense_IncAttack;
			for (int32 AI = 0; AI < IntrinsicOptionNum; ++AI)
			{
				FItemOption NewOption;
				NewOption.OptionType = (EItemOption)FMath::RandRange((int32)EItemOption::EIO_Offense_IncAttack + (AllOptionRange * AI / MAX_ITEM_INTRINSIC_OPTION),
					(int32)EItemOption::EIO_Offense_IncAttack + (AllOptionRange * (AI + 1) / MAX_ITEM_INTRINSIC_OPTION));
				NewOption.RawOptionAmount = FMath::FRandRange(1.0f, 99.0f);
				NewWeapon.IntrinsicOptions.Add(NewOption);
			}
			int32 RandOptionNum = FMath::RandRange(1, MAX_ITEM_RANDOM_OPTION);
			for (int32 AI = 0; AI < RandOptionNum; ++AI)
			{
				FItemOption NewOption;
				NewOption.OptionType = (EItemOption)FMath::RandRange((int32)EItemOption::EIO_Offense_IncAttack + (AllOptionRange * AI / MAX_ITEM_RANDOM_OPTION),
					(int32)EItemOption::EIO_Offense_IncAttack + (AllOptionRange * (AI + 1) / MAX_ITEM_RANDOM_OPTION));
				NewOption.RawOptionAmount = FMath::FRandRange(1.0f, 99.0f);
				NewWeapon.RandomOptions.Add(NewOption);
			}

			ReAddStoredItem(NewWeapon, true, CurrPCClass, false);

			if (NI == WeaponEquipIndex)
			{
				IDsToEquip.Add(NewWeapon.InstanceUID);
			}
		}

		// 방어구들
		const int ProtectionNum = FMath::RandRange(36, 48); // 전체 개수
		const int ProtectionEquipIndex = FMath::RandRange(0, ProtectionNum - 1); // 그 중 장착할 거 인덱스
		for (int32 NI = 0; NI < ProtectionNum; ++NI)
		{
			const int32 ItemClassID = (NI % 3) + 1; // 방어구 종류
			const int32 StarGrade = (NI % 3) + 1;

			FB2Item NewProtection;
			NewProtection.InstanceUID = 56340000 + IPI * 1000 + NI;
			NewProtection.ItemRefID = 1000000 + 100000 * IPI + 1000 * ItemClassID + 100 * StarGrade;

			if (bRequestToServer) // 서버에 요청한다면 지금까지 정보로 충분함. 서버 테스트 커맨드 사용.
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewProtection.ItemRefID, StarGrade));
				continue;
			}

			DevOnlyLoadItemClientTestData(NewProtection);
			NewProtection.PrimaryPointType = EItemPrimaryPointType::EIPP_Defense;
			NewProtection.SetPrimaryPoint((float)StarGrade * 100.0f + FMath::FRandRange(0.0f, 100.0f));
			NewProtection.Level = FMath::RandRange(1, NewProtection.MaxLevel - 1); // 강화 레벨은 보여지는 중요 정보 중 하나이니 무작위로 넣어주자 ㅋ
			NewProtection.Quality = FMath::RandRange(1, MAX_ITEM_QUALITY); // 품질도 보여지는 정보.
			
			// 특정 조건에서만 사용 가능한 합성, 승급, 초월 테스트를 위해 특별 수치 세팅.
			if (FMath::RandRange(1, 10) == 1)
			{
				NewProtection.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewProtection.Level = NewProtection.MaxLevel;
			}
			else if (FMath::RandRange(1, 10) == 1)
			{ // 초월 재료
				NewProtection.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewProtection.Level = 1;
			}
			else if (FMath::RandRange(1, 4) == 1)
			{
				NewProtection.Level = NewProtection.MaxLevel;
			}

			// 능력치 옵션 적당히 넣어줌 ㅋ
			int32 IntrinsicOptionNum = FMath::RandRange(0, MAX_ITEM_INTRINSIC_OPTION);
			int32 AllOptionRange = (int32)EItemOption::EIO_Defense_ResistCritical - (int32)EItemOption::EIO_Health_ProbableHPAbsorption;
			for (int32 AI = 0; AI < IntrinsicOptionNum; ++AI)
			{
				FItemOption NewOption;
				NewOption.OptionType = (EItemOption)FMath::RandRange((int32)EItemOption::EIO_Health_ProbableHPAbsorption + (AllOptionRange * AI / MAX_ITEM_INTRINSIC_OPTION),
					(int32)EItemOption::EIO_Health_ProbableHPAbsorption + (AllOptionRange * (AI + 1) / MAX_ITEM_INTRINSIC_OPTION));
				NewOption.RawOptionAmount = FMath::FRandRange(1.0f, 99.0f);
				NewProtection.IntrinsicOptions.Add(NewOption);
			}
			int32 RandOptionNum = FMath::RandRange(1, MAX_ITEM_RANDOM_OPTION);
			for (int32 AI = 0; AI < RandOptionNum; ++AI)
			{
				FItemOption NewOption;
				NewOption.OptionType = (EItemOption)FMath::RandRange((int32)EItemOption::EIO_Health_ProbableHPAbsorption + (AllOptionRange * AI / MAX_ITEM_RANDOM_OPTION),
					(int32)EItemOption::EIO_Health_ProbableHPAbsorption + (AllOptionRange * (AI + 1) / MAX_ITEM_RANDOM_OPTION));
				NewOption.RawOptionAmount = FMath::FRandRange(1.0f, 99.0f);
				NewProtection.RandomOptions.Add(NewOption);
			}

			ReAddStoredItem(NewProtection, true, CurrPCClass, false);

			if (NI == ProtectionEquipIndex)
			{
				IDsToEquip.Add(NewProtection.InstanceUID);
			}
		}

		// 장신구들
		const int AccessoryNum = FMath::RandRange(36, 48); // 전체 개수
		const int AccessoryEquipIndex = FMath::RandRange(0, AccessoryNum - 1); // 그 중 장착할 거 인덱스
		for (int32 NI = 0; NI < AccessoryNum; ++NI)
		{
			const int32 ItemClassID = (NI % 3) + 5; // 장신구 종류
			const int32 StarGrade = (NI % 3) + 1;

			FB2Item NewAccessory;
			NewAccessory.InstanceUID = 98230000 + IPI * 1000 + NI;
			NewAccessory.ItemRefID = 1000000 + 100000 * IPI + 1000 * ItemClassID + 100 * StarGrade;

			if (bRequestToServer) // 서버에 요청한다면 지금까지 정보로 충분함. 서버 테스트 커맨드 사용.
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewAccessory.ItemRefID, StarGrade));
				continue;
			}

			DevOnlyLoadItemClientTestData(NewAccessory);
			NewAccessory.PrimaryPointType = EItemPrimaryPointType::EIPP_Health; // 장신구는 기본 능력치가 체력인건가.
			NewAccessory.SetPrimaryPoint((float)StarGrade * 100.0f + FMath::FRandRange(0.0f, 100.0f));
			NewAccessory.Level = FMath::RandRange(1, NewAccessory.MaxLevel - 1); // 강화 레벨은 보여지는 중요 정보 중 하나이니 무작위로 넣어주자 ㅋ
			NewAccessory.Quality = FMath::RandRange(1, MAX_ITEM_QUALITY); // 품질도 보여지는 정보.
			
			// 특정 조건에서만 사용 가능한 합성, 승급, 초월 테스트를 위해 특별 수치 세팅.
			if (FMath::RandRange(1, 10) == 1)
			{
				NewAccessory.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewAccessory.Level = NewAccessory.MaxLevel;
			}
			else if (FMath::RandRange(1, 10) == 1)
			{ // 초월 재료
				NewAccessory.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewAccessory.Level = 1;
			}
			else if (FMath::RandRange(1, 4) == 1)
			{
				NewAccessory.Level = NewAccessory.MaxLevel;
			}

			// 능력치 옵션 적당히 넣어줌 ㅋ
			int32 IntrinsicOptionNum = FMath::RandRange(0, MAX_ITEM_INTRINSIC_OPTION);
			int32 AllOptionRange = (int32)EItemOption::EIO_Misc_DecTagCooltime - (int32)EItemOption::EIO_Offense_IncAttack;
			for (int32 AI = 0; AI < IntrinsicOptionNum; ++AI)
			{
				FItemOption NewOption;
				NewOption.OptionType = (EItemOption)FMath::RandRange((int32)EItemOption::EIO_Offense_IncAttack + (AllOptionRange * AI / MAX_ITEM_INTRINSIC_OPTION),
					(int32)EItemOption::EIO_Offense_IncAttack + (AllOptionRange * (AI + 1) / MAX_ITEM_INTRINSIC_OPTION));
				NewOption.RawOptionAmount = FMath::FRandRange(1.0f, 99.0f);
				NewAccessory.IntrinsicOptions.Add(NewOption);
			}
			int32 RandOptionNum = FMath::RandRange(1, MAX_ITEM_RANDOM_OPTION);
			for (int32 AI = 0; AI < RandOptionNum; ++AI)
			{
				FItemOption NewOption;
				NewOption.OptionType = (EItemOption)FMath::RandRange((int32)EItemOption::EIO_Offense_IncAttack + (AllOptionRange * AI / MAX_ITEM_RANDOM_OPTION),
					(int32)EItemOption::EIO_Offense_IncAttack + (AllOptionRange * (AI + 1) / MAX_ITEM_RANDOM_OPTION));
				NewOption.RawOptionAmount = FMath::FRandRange(1.0f, 99.0f);
				NewAccessory.RandomOptions.Add(NewOption);
			}

			ReAddStoredItem(NewAccessory, true, CurrPCClass, false);

			if (NI == AccessoryEquipIndex)
			{
				IDsToEquip.Add(NewAccessory.InstanceUID);
			}
		}

		// 정수 아이템도
		int32 WeaponEssenceNum = 4;
		for (int32 WEI = 0; WEI < WeaponEssenceNum; ++WEI)
		{
			const int32 StarGrade = WEI + 2;

			FB2Item NewWeaponEssence;
			NewWeaponEssence.InstanceUID = 7349579324000 + IPI * 1000 + WEI; // 사용되는 상황에서 다른 아이템은 없을 것이므로 여기서만이라도 겹치지 않게 적당히 넣어줌
			// 정수 아이템은 2000000 번대
			NewWeaponEssence.ItemRefID = 2000000 + (100000 * IPI) + (0 * 1000) + (StarGrade * 100);

			if (bRequestToServer)
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewWeaponEssence.ItemRefID, StarGrade));
				continue;
			}			
			DevOnlyLoadItemClientTestData(NewWeaponEssence);
			ReAddStoredItem(NewWeaponEssence, true, CurrPCClass, false);
		}
		int32 ProtectionEssenceNum = 4;
		for (int32 PEI = 0; PEI < ProtectionEssenceNum; ++PEI)
		{
			const int32 StarGrade = PEI + 2;
			FB2Item NewProtectionEssence;
			NewProtectionEssence.InstanceUID = 802364934000 + IPI * 1000 + PEI; 
			NewProtectionEssence.ItemRefID = 2000000 + (100000 * IPI) + (1 * 1000) + (StarGrade * 100);
			if (bRequestToServer)
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewProtectionEssence.ItemRefID, StarGrade));
				continue;
			}
			DevOnlyLoadItemClientTestData(NewProtectionEssence);
			ReAddStoredItem(NewProtectionEssence, true, CurrPCClass, false);
		}
		int32 AccessoryEssenceNum = 4;
		for (int32 AEI = 0; AEI < AccessoryEssenceNum; ++AEI)
		{
			const int32 StarGrade = AEI + 2;
			FB2Item NewAccessoryEssence;
			NewAccessoryEssence.InstanceUID = 52117932000 + IPI * 1000 + AEI;
			NewAccessoryEssence.ItemRefID = 2000000 + (100000 * IPI) + (2 * 1000) + (StarGrade * 100);
			if (bRequestToServer)
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewAccessoryEssence.ItemRefID, StarGrade));
				continue;
			}
			DevOnlyLoadItemClientTestData(NewAccessoryEssence);
			ReAddStoredItem(NewAccessoryEssence, true, CurrPCClass, false);
		}

		// 위에서 각 종류별 무작위로 고른 것들을 장착하는데 서버에서 요청하는 상황이면 이 시점에 처리가 아마도 안되었을테니 장착까지는 안함 ㅋ
		if (!bRequestToServer)
		{
			// TODO 아이템 프리셋 (일단 0번 프리셋 사용)
			BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().EquipItems(IDsToEquip, true, 0);
		}
	}

	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
}
#endif


void UB2LobbyInventory::ReqForEquipItems(const TArray<int64>& UIDsToEquip)
{
	// TODO 아이템 프리셋
	auto ePCClass = GetInventorySelectedPCClass();
	int32 currentItemPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(ePCClass);

	data_trader::Retailer::GetInstance().RequestEquipItem(currentItemPresetId, UIDsToEquip);
}

void UB2LobbyInventory::ReqForUnEquipItems(const TArray<int64>& UIDsToUnEquip)
{
	// TODO 아이템 프리셋
	auto ePCClass = GetInventorySelectedPCClass();
	int32 currentItemPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(ePCClass);

	data_trader::Retailer::GetInstance().RequestUnequipItem(currentItemPresetId, UIDsToUnEquip);
}

void UB2LobbyInventory::ReqForUnEquipItems(const int32 PresetID, const TArray<int64>& UIDsToUnEquip)
{
	data_trader::Retailer::GetInstance().RequestUnequipItem(PresetID, UIDsToUnEquip);
}

void UB2LobbyInventory::ReqForToggleItemLock(int64 ItemInstanceUID, bool bLock)
{
	TArray<int64> ItemInstanceUIDArray;
	ItemInstanceUIDArray.Add(ItemInstanceUID);

	if (bLock)
		data_trader::Retailer::GetInstance().RequestLockItem(ItemInstanceUIDArray);
	else
		data_trader::Retailer::GetInstance().RequestUnlockItem(ItemInstanceUIDArray);
}

void UB2LobbyInventory::ReqForSellItem(int64 ItemInstanceUID, bool bIsTotem)
{
	TArray<int64> ItemInstanceUIDArray;
	ItemInstanceUIDArray.Add(ItemInstanceUID);
	ItemSellLoopType = EItemLoopType::EI_Normal;
	bUseTotem = bIsTotem;
	if (bUseTotem)
	{
		data_trader::Retailer::GetInstance().RequestSellTotems(ItemInstanceUIDArray);
	}
	else
	{
		data_trader::Retailer::GetInstance().RequestSellItem(ItemInstanceUIDArray);
	}
}

void UB2LobbyInventory::ReqForSellItems(const TArray<int64>& UIDsToSell, bool bIsTotem)
{
	SellItemUIDs = UIDsToSell;
	GetSellItemGold = 0;
	bUseTotem = bIsTotem;
	SendSellItem();
}

void UB2LobbyInventory::ReqForConvertData(int32 templateId_)
{
	templateUID = templateId_;
	templateId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().FindItem(templateUID)->ItemRefID;
	SendConvertData();
}

void UB2LobbyInventory::ReqForConvertItem(int32 ItemID, int32 ConvertType_)
{
	convertItemID = ItemID;
	convertType = ConvertType_;
	SendConvertItem();
}
void UB2LobbyInventory::ReqForDecompositionItems(const TArray<int64>& UIDsToDecomposition, bool bIsTotem)
{
	DecompositionItemUIDs = UIDsToDecomposition;
	GetDismantleItems.Empty();
	GetDismantleItemsKey.Empty();
	bUseTotem = bIsTotem;

	SendDecompositionItem();
}

void UB2LobbyInventory::ReqForSellComsumableItem(int64 ItemInstanceUID, int32 ItemSellCount)
{
	data_trader::Retailer::GetInstance().RequestSellConsumableItem(ItemInstanceUID, ItemSellCount);
}

void UB2LobbyInventory::ReqForItemLevelupWithCurrentSelection()
{
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup)
	{
		TArray<int64> AllSelectedIngreds;
		GetAllSelectedItemOpIngreds(AllSelectedIngreds);
		if (AllSelectedIngreds.Num() > 0){ 
			data_trader::Retailer::GetInstance().RequestLevelupItem(ItemOpTargetItemID, AllSelectedIngreds, false);
			OnReqForItemOpCommon();
		}
	}
}

void UB2LobbyInventory::ReqForEnhanceItem()
{
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Enhancement)
	{
		TArray<FB2Item> AllSelectedIngreds;
		GetAllSelectedItemOpIngreds(AllSelectedIngreds);

		if (AllSelectedIngreds.Num() <= 0)
			return;

		CheckUsableEnhanceProtectionItem();

		data_trader::Retailer::GetInstance().RequestEnhanceItem(
			ItemOpTargetItemID, AllSelectedIngreds[0].ItemRefID, IsSelectedEnhanceProtectionItem(), PendingEnhanceProtectionItemRefID);

		OnReqForItemOpCommon();
	}
}

void UB2LobbyInventory::ReqForUpgradeItem()
{
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Upgrade)
	{
		data_trader::Retailer::GetInstance().RequestUpgradeItem(ItemOpTargetItemID);
		OnReqForItemOpCommon();
	}
}

void UB2LobbyInventory::ReqForComposeItemWithCurrentSelection()
{
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Composition)
	{
		TArray<int64> AllSelectedIngreds;
		GetAllSelectedItemOpIngreds(AllSelectedIngreds);
		if (AllSelectedIngreds.Num() > 0){
			auto ePCClass = GetInventorySelectedPCClass();
			int32 currentItemPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(ePCClass);
			data_trader::Retailer::GetInstance().RequestComposeItem(ItemOpTargetItemID, AllSelectedIngreds[0], currentItemPresetId);
			OnReqForItemOpCommon();
		}
	}
}

void UB2LobbyInventory::ReqForSurpassItemWithCurrentSelection()
{
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass)
	{
		TArray<int64> AllSelectedIngreds;
		GetAllSelectedItemOpIngreds(AllSelectedIngreds);
		if (AllSelectedIngreds.Num() > 0){
			data_trader::Retailer::GetInstance().RequestSurpassItem(ItemOpTargetItemID, AllSelectedIngreds[0]);
			OnReqForItemOpCommon();
		}
	}
}

void UB2LobbyInventory::ReqForSmeltingTotem(int64 TotemInstanceID, int32 ServerOptionID, int32 ServerRefineCostType, bool LastRefine)
{
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_TotemSmelting)
	{
		CachedTotemRefineCostType = ServerRefineCostType;
		data_trader::Retailer::GetInstance().RequestRefineTotem(TotemInstanceID, ServerOptionID, ServerRefineCostType);

		if (LastRefine)
		{
			OnReqForItemOpCommon();
		}
	}
}

void UB2LobbyInventory::ReqForToggleTotemLock(int64 TotemInstanceUID, bool bLock)
{
	TArray<int64> TotemInstanceUIDArray;
	TotemInstanceUIDArray.Add(TotemInstanceUID);

	if (bLock)
	{
		data_trader::Retailer::GetInstance().RequestLockTotems(TotemInstanceUIDArray);
	}
	else
	{
		data_trader::Retailer::GetInstance().RequestUnlockTotems(TotemInstanceUIDArray);
	}
}

void UB2LobbyInventory::OnReqForItemOpCommon()
{
	GhostItemOpIngreds.Empty(); // 리스트 비워지기 전에 UI 연출에 쓰기 위해 재료 정보만 슬쩍 저장
	GetAllSelectedItemOpIngreds(GhostItemOpIngreds);
	//ResetAllIngredsList(); -> 여기서 재료 마크 없애는 게 안전하긴 한데 UI 연출 관련 요청이 있어서 하지 않는다.

	// Request 를 보내고 나서야 CachedItemOpTargetData 를 캐싱해 놓는다. 이전까지는 CachedItemOpTargetData 를 사용하면 안됨.
	GetItemOPTargetItemData(CachedItemOpTargetData, false); // 여기서 true 를 주면 CachedItemOpTargetData 를 CachedItemOpTargetData 에 넣으려고 하겠지 ㅋ
}

void UB2LobbyInventory::ReqForExtendInventory(EPCClass InPCClass, EItemInvenType InvenType, int32 ExtendCount)
{
	int32 SvrPCClass = CliToSvrPCClassType(InPCClass);
	int32 SvrInvenType = CliToSvrItemInvenType(InvenType);

	if (
		SvrPCClass != b2network::B2CharacterType::None &&
		(SvrInvenType == b2network::B2InventoryType::Weapon || SvrInvenType == b2network::B2InventoryType::Protection || SvrInvenType == b2network::B2InventoryType::Accessory) &&
		ExtendCount > 0
		)
	{
		// colosseum comment : equipable inventory combine
		//data_trader::Retailer::GetInstance().RequestExtendInventory(SvrPCClass, SvrInvenType, ExtendCount);
		data_trader::Retailer::GetInstance().RequestExtendInventory(SvrPCClass, ExtendCount);
	}
}

EPCClass UB2LobbyInventory::GetInventorySelectedPCClass() 
{	
	if (TransientLobbyGM)
	{// 로비에선 인벤토리는 영웅관리 창 선택 PC 를 기본으로. 물론 어디까지나 기본값
		return TransientLobbyGM->GetHeroMgmtModePCClass();
	}

	// GetLocalPlayerCharacter 를 사용할 수 있다면 인게임에서 BladeIIPlayer 의 GetCurrentPlayerClass 를 사용해 볼 수도.
	// 여하간 이게 사용되는 상황에서는 PCClass 를 직접 지정해 줄 수 있을 테니 가급적 직접 지정을..

	return EPCClass::EPC_End;
}

FInvenPCItemAccessor* UB2LobbyInventory::GetPCItemAccessor(EPCClass InPCClass)
{
	int32 InvenPCIdx = GetInvenPCIndexFromPCClass(InPCClass);
	return (PCItems.IsValidIndex(InvenPCIdx)) ? &(PCItems[InvenPCIdx]) : NULL;
}

TMap<int64, FB2Item>* UB2LobbyInventory::GetStoredItemMapPtr(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	TMap<int64, FB2Item>* RetList = NULL;

	if (InItemInvenType == EItemInvenType::EIIVT_Consumables) // 소모품은 모든 PC 간에 공유됨.
	{
		RetList = &GetConsumablesList();
	}
	else
	{
		int32 InvenPCIdx = GetInvenPCIndexFromPCClass(FinalPCClass);
		RetList = (InvenPCIdx < GetMaxPCClassNum()) ? PCItems[InvenPCIdx].GetItemMapForInvenType(InItemInvenType) : NULL;
	}

	return RetList;
}

void UB2LobbyInventory::GetStoredItemList(TArray<FB2Item>& OutResult, EItemInvenType InItemInvenType, EPCClass InPCClass, bool bSort, int32 Grade)
{
	TMap<int64, FB2Item>* InternalMap = GetStoredItemMapPtr(InItemInvenType, InPCClass);
	// UI 등에서 단순 리스트로 사용하기 위해 TArray 에 넣어주는 기능임
	if (InternalMap)
	{
		for (auto& CurrPair : *InternalMap)
		{
			if (Grade != -1 && CurrPair.Value.StarGrade != Grade)
			{
				continue;
			}
			OutResult.Add(CurrPair.Value);
		}

		if (bSort)
		{
			SortItemListForStoredItemView(InItemInvenType, OutResult);
		}
	}
}

void UB2LobbyInventory::GetStoredPendingItemList(TArray<FB2Item>& OutResult)
{
	OutResult.Append(PendingSelectItemOpIngreds);
}

void UB2LobbyInventory::SortItemListForStoredItemView(EItemInvenType InInvenType, TArray<FB2Item>& InOutList)
{
#if UE_BUILD_DEBUG
	for (const FB2Item& ThisItemCheck : InOutList)
	{// InOutList 각 element 들의 InventoryType 이 일치해야 함.
		checkSlow(ThisItemCheck.InventoryType == InInvenType);
	}
#endif

	if (InInvenType != EItemInvenType::EIIVT_Consumables) 
	{
		// 인벤토리에 진열하기 위한 소팅.
		// 강화 < 레벨 < 별등급 < 잠금 < (일시적)새거 순
		// 명시적 요구사항은 아니었지만 위 조건들이 모두 같은 경우를 위해 토탈 전투력을 베이스로..
		struct FInvItemCombatPowerSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return A.Power > B.Power;
			}
		};
		InOutList.StableSort(FInvItemCombatPowerSorter()); // 이어지는 아래 소팅에서도 이전 소팅을 보존하기 위해 StableSort 로.

		struct FInvItemEnhanceLevelSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return A.EnhanceLevel > B.EnhanceLevel;
			}
		};
		InOutList.StableSort(FInvItemEnhanceLevelSorter());

		struct FInvItemLevelSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return A.Level > B.Level;
			}
		};
		InOutList.StableSort(FInvItemLevelSorter());

		struct FInvItemStarGradeSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return A.StarGrade > B.StarGrade;
			}
		};
		InOutList.StableSort(FInvItemStarGradeSorter());

		struct FInvItemLockSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return (int32)A.bIsLocked > (int32)B.bIsLocked;
			}
		};
		InOutList.StableSort(FInvItemLockSorter());
		
		// 그리고 새로 얻은 아이템을 일시적으로 제일 앞에 놓아주어야 한다는데..
		struct FInvItemNewSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return (A.IsNew && !B.IsNew) ? true : false;
			}
		};
		InOutList.StableSort(FInvItemNewSorter());
		

		// 큰 타입에 따른 정렬도 하는데 보관함에 장착 아이템 외에 정수 아이템도 들어오기 때문. 이건 새로 들어오더라도 뒤에 넣어줄까나..
		struct FInvItemTypeSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return (IsEquipmentItem(A) && !IsEquipmentItem(B)) ? true : false;
			}
		};
		InOutList.StableSort(FInvItemTypeSorter());

		//InstanceUID는 아이템이 지급된 순서로 튜토리얼용 아이템이 가장 나중에 지급되기 때문에 해당 조건으로 정렬
		if (TutorialManager::GetInstance().HasAnyTutorial())
		{
			struct FInvItemTypeSorter
			{
				bool operator()(const FB2Item& A, const FB2Item& B) const
				{
					return (int32)A.InstanceUID > (int32)B.InstanceUID;
				}
			};
			InOutList.StableSort(FInvItemTypeSorter());
		}

	}
	else
	{ // 소모품은 좀 다른 기준으로, 전투력이니 레벨이니 이런 거 없으니.. 대체로 타입에 따라 정해짐.

		// 아래에 이러질 타입 정렬도 그렇지만 가장 기본은 테이블에 등록된 RefID 순이다. 
		// 타입별 정렬에서도 무기/방어구/장신구 중 어디에 적용되는 재료냐에 따라 정렬이 되어야 하는데 모두 일일이 명시하지 못하고 RefID 로 한다. 
		// 물론 정렬 순서에 따라 RefID 가 정해져 있어야 한다.
		struct FConsumablesTypeSorter_RefID
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return (A.ItemRefID < B.ItemRefID);
			}
		};
		InOutList.StableSort(FConsumablesTypeSorter_RefID());  // 이어지는 아래 소팅에서도 이전 소팅을 보존하기 위해 StableSort 로.

		struct FConsumablesTypeSorter_SortValue
		{
			UB2ItemSortInfo* ItemSortInfo;
			FConsumablesTypeSorter_SortValue()
			{
				//ItemSortInfo = StaticFindItemSortInfo();
			}
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				if (ItemSortInfo == nullptr)
					return false;

				int32 ASortValue = ItemSortInfo->GetItemSortValue(A.ItemRefID);
				int32 BSortValue = ItemSortInfo->GetItemSortValue(B.ItemRefID);

				//2가지 경우 A sort value > 0이고 B가 -1 인경우
				//둘다 있는 경우 A.SortValue > B.SortValue

				if (ASortValue > 0 && BSortValue == INDEX_NONE)
					return true;
				else if (ASortValue > 0 && BSortValue > 0)
					return ASortValue > BSortValue;
				else
					return false;
			}
		};
		InOutList.StableSort(FConsumablesTypeSorter_SortValue());

		struct FFConsumablesTypeSorter_New
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return (A.IsNew && !B.IsNew) ? true : false;
			}
		};
		InOutList.StableSort(FFConsumablesTypeSorter_New());
	}
}

void UB2LobbyInventory::GetStoredItemList_ItemOpModeFilter(TArray<FB2Item>& OutResult, EItemInvenType InItemInvenType, EPCClass InPCClass, bool bSort, int32 Grade)
{
	GetStoredItemList(OutResult, InItemInvenType, InPCClass, bSort, Grade);

	FB2Item OpTarget;
	if (!GetItemOPTargetItemData(OpTarget)){
		return;
	}

	for (int32 II = 0; II < OutResult.Num(); ++II)
	{
		const FB2Item& CurrCheck = OutResult[II];
		bool bRemoveCurrentCheck = false;

		// IsEnable** 들에서 UID 체크를 하긴 할건데, Upgrade 의 경우 자신을 보여주게 되는지는 모르겠다. 일단 어느 경우든 자신은 안 나오도록.
		bRemoveCurrentCheck = (CurrCheck.InstanceUID == OpTarget.InstanceUID);

		switch (CurrentItemOpMode)
		{
		case ELobbyInvenItemOpMode::EIVIOP_Levelup:
			bRemoveCurrentCheck |= !IsEnableItemLevelupIngred(CurrCheck); // 선택 가능한 것만 보여주기.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Upgrade:
			bRemoveCurrentCheck |= !(IsItemUpgradeAllowed(CurrCheck) || IsItemAllowedCompositeAndSurpass(CurrCheck)); // 승급의 경우는 조금 특이한데, 승급 재료란 건 없고, 승급 타겟이 될 수 있는 애들만 보여준다.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Composition:
			bRemoveCurrentCheck |= !IsEnableComposeIngred(CurrCheck); // 선택 가능한 것만 보여주기.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Surpass:
			bRemoveCurrentCheck |= !IsEnableSurpassIngred(CurrCheck); // 선택 가능한 것만 보여주기.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
			bRemoveCurrentCheck |= !IsVisibilityEhanceIngred(CurrCheck); // 선택 가능한 것만 보여주기.
			break;			
		case ELobbyInvenItemOpMode::EIVIOP_EnhancementChange:
			bRemoveCurrentCheck |= !IsEnableEhanceChangeIngred(CurrCheck); // 선택 가능한 것만 보여주기.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_HeroChange:
			bRemoveCurrentCheck = false;
			break;
		}

		if (bRemoveCurrentCheck)
		{
			OutResult.RemoveAt(II);
			--II;
		}
	}
}

void UB2LobbyInventory::GetMergedItemList(TArray<FB2Item>& OutResult, bool bIncludeEquipped, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	for (int32 ITI = 0; ITI < (int32)EItemInvenType::EIIVT_End; ++ITI)
	{
		TArray<FB2Item> CurrList; 
		GetStoredItemList(CurrList, (EItemInvenType)ITI, FinalPCClass, false); // 여기서 소팅해 봐야 어차피 더할 거 필요가 없지.
		OutResult.Append(CurrList);
	}

	if (bIncludeEquipped)
	{
		FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);
		if (CurrPCItemAccessor)
		{
			TArray<FB2Item> AllEquipped;
			CurrPCItemAccessor->GetAllEquippedItems(AllEquipped);
			OutResult.Append(AllEquipped);
		}
	}
}

void UB2LobbyInventory::GetFilteredItemList(TArray<FB2Item>& OutResult, bool bIncludeEquipped, bool bIncludeLocked, bool bIncludeEquippedPreset, TArray<EItemInvenType>& InvenTypeFilter, 
											TArray<int32>& StarGradeFilter, EPCClass InPCClass, bool bIncludeLevelUp, bool bIncludeEnhanced, bool bIncludeEssence, bool bIncludeAnvil)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	TArray<FB2Item> MergedList;
	GetMergedItemList(MergedList, bIncludeEquipped, FinalPCClass); // 일단 filter 외의 조건에서 전체 합쳐진 아이템 목록.

	//TArray 쓰면 Resize 일어날때 Lambda 받은 std::function이 메모리 깨짐
	std::list<std::function<bool(FB2Item)>> FIlters;
	// 차례대로 필터 추가
	FIlters.emplace_back(
		[&InvenTypeFilter](FB2Item CurrItem)
	{
		for (EItemInvenType CurrInvenType : InvenTypeFilter) // InvenTypeFilter 에 있는 element 중 어느 하나와 일치하면 포함.
		{
			if (CurrItem.InventoryType == CurrInvenType)
			{
				return true;
			}
		}
		return false;
	});

	FIlters.emplace_back(
		[&StarGradeFilter](FB2Item CurrItem)
	{
		for (int32 CurrStarGrade : StarGradeFilter) // StarGradeFilter 에 있는 element 중 어느 하나와 일치하면 포함.
		{
			if (CurrItem.StarGrade == CurrStarGrade)
			{
				return true;
			}
		}
		return false;
	});
	
	// Lock 된 것을 걸러내는 것은 자동판매나 자동강화 같은 기능에서 사용.
	if (!bIncludeLocked)
	{
		FIlters.emplace_back(
			[](FB2Item CurrItem)
		{
			return !CurrItem.bIsLocked;
		});
	}
	
	if (!bIncludeEquippedPreset)
	{
		FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
		FPCItemContainer &ItemContainer = CliDataStore.GetLocalCharacterData().GetUserAllItems();

		FIlters.emplace_back(
			[&ItemContainer](FB2Item CurrItem)
		{
			if (ItemContainer.IsInItemPresets(IntToPCClass(CurrItem.AllowedPCClass), CurrItem.InstanceUID) == false)
				return true;
			return false;
		});
	}

	if (!bIncludeLevelUp)
	{
		FIlters.emplace_back(
			[](FB2Item CurrItem)
		{
			if (!IsEssenceItem(CurrItem))
			{
				return (CurrItem.Level <= 1);
			}
			return true;
		});
	}

	if (!bIncludeEnhanced)
	{
		FIlters.emplace_back(
			[](FB2Item CurrItem)
		{
			if (!IsEssenceItem(CurrItem))
			{
				return (CurrItem.EnhanceLevel == 0);
			}
			return true;
		});
	}

	if (!bIncludeEssence)
	{
		FIlters.emplace_back(
			[](FB2Item CurrItem)
		{
			return !IsEssenceItem(CurrItem);
		});
	}

	// 초월재료(모루) 예외처리
	if (!bIncludeAnvil)
	{
		FIlters.emplace_back(
			[](FB2Item CurrItem)
		{
			return !IsAnvilItem(CurrItem);
		});
	}

	for (auto &Filter : FIlters)
	{
		if( MergedList.Num() == 0)
			break;
		MergedList = MergedList.FilterByPredicate(Filter);
	}
	
	OutResult = MergedList; // 결과 리스트로 전달.
}

void UB2LobbyInventory::GetStoredItemListForEquipPlace(TArray<FB2Item>& OutResult, EItemEquipPlace InEquipPlace, EPCClass InPCClass /*= EPCClass::EPC_End*/)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	TArray<FB2Item> MergedList;
	GetMergedItemList(MergedList, false, FinalPCClass); // InEquipPlace 에 따라 InvenType 이 있긴 한데.. 클라에 그걸 맵핑하는 함수를 두는 것도 뭐하고 해서 걍 일단 MergedList 얻어온 후에..

	if (MergedList.Num() > 0)
	{
		OutResult = MergedList.FilterByPredicate( // InEquipPlace 와 일치하는 걸로만 골라낸다.
			[&InEquipPlace](FB2Item CurrItem)
			{ // 정수, 초월재료(모루) 예외처리
				if (CurrItem.EquipPlace == InEquipPlace && !IsEssenceItem(CurrItem) && !IsAnvilItem(CurrItem))
				{
					return true;
				}
				return false;
			}
		);
	}
}

int32 UB2LobbyInventory::GetItemContainerSize(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	int32 RetSlotCount = BladeIIGameImpl::GetClientDataStore().GetCharacterInvenSlotCount(FinalPCClass, InItemInvenType);
	
	return RetSlotCount;
}

/* 캐릭터 부위별 인벤토리 */
int32 UB2LobbyInventory::GetStoredItemNumInSharedSlot(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	if (InItemInvenType == EItemInvenType::EIIVT_Consumables)
	{
		return GetConsumablesList().Num(); // PCClass 간 공유.
	}
	else if (InItemInvenType != EItemInvenType::EIIVT_End)
	{
		int32 InvenPCIdx = GetInvenPCIndexFromPCClass(FinalPCClass);
		if (InvenPCIdx >= 0 && InvenPCIdx < GetMaxPCClassNum())
		{
			FInvenPCItemAccessor& PCList = PCItems[InvenPCIdx];
			TMap<int64, FB2Item>* ItemMap = PCList.GetItemMapForInvenType(InItemInvenType);

			// 장착중인 장비도 포함.
			TArray<FB2Item> EquippedList;
			PCList.GetAllEquippedItems(EquippedList, InItemInvenType);

			int32 HasCount = EquippedList.Num();
			if (ItemMap)
				HasCount += ItemMap->Num();
			return HasCount;
		}
	}
	return 0;
}

/* 캐릭터 통합 인벤토리 */
int UB2LobbyInventory::GetStoredItemNumInUnitedSlot(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	if (InItemInvenType == EItemInvenType::EIIVT_Consumables)
	{
		return GetConsumablesList().Num(); // PCClass 간 공유.
	}
	else if (InItemInvenType != EItemInvenType::EIIVT_End)
	{
		int32 InvenPCIdx = GetInvenPCIndexFromPCClass(FinalPCClass);
		if (InvenPCIdx >= 0 && InvenPCIdx < GetMaxPCClassNum())
		{
			FInvenPCItemAccessor& PCList = PCItems[InvenPCIdx];
			int32 ItemCount = PCList.GetAllItemMapCount();

			// 장착중인 장비도 포함.
			TArray<FB2Item> EquippedList;
			PCList.GetAllEquippedItems(EquippedList);

			int32 HasCount = EquippedList.Num();
			HasCount += ItemCount;

			return HasCount;
		}
	}
	return 0;
}

int UB2LobbyInventory::GetStoredCostomeItemNumInUnitedSlot(EPCClass InPCClass)
{
	int32 HasCount = 0;
	TArray<FB2Item> CurrPCItemList;
	
	GetStoredItemList_ItemOpModeFilter(CurrPCItemList, EItemInvenType::EIIVT_CostumeArmor, InPCClass, false);
	GetStoredItemList_ItemOpModeFilter(CurrPCItemList, EItemInvenType::EIIVT_CostumeWeapon, InPCClass, false);
	
	HasCount = CurrPCItemList.Num();

	int32 InvenPCIdx = GetInvenPCIndexFromPCClass(InPCClass);
	if (InvenPCIdx >= 0 && InvenPCIdx < GetMaxPCClassNum())
	{
		FInvenPCItemAccessor& PCList = PCItems[InvenPCIdx];
		int32 ItemCount = PCList.GetAllItemMapCount();

		// 장착중인 장비도 포함.
		TArray<FB2Item> EquippedList;
		PCList.GetAllEquippedCostumeItems(EquippedList);
		HasCount += EquippedList.Num();
	}

	return HasCount;
}

void UB2LobbyInventory::GetAllTotemList(TArray<FB2Totem>& OutResult)
{
	OutResult.Empty();
	TMap<int64, FB2Totem> AllTotems;
	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetAllTotems(AllTotems);

	for (TPair<int64, FB2Totem>& Elem : AllTotems)
	{
		OutResult.Add(Elem.Value);
	}
}

void UB2LobbyInventory::GetEquippedTotemList(TArray<FB2Totem>& OutResult, EPCClass InPCClass)
{
	OutResult.Empty();
	TMap<int64, FB2Totem> EquippedTotems;
	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetEquippedTotems(InPCClass, EquippedTotems);

	for (TPair<int64, FB2Totem>& Elem : EquippedTotems)
	{
		OutResult.Add(Elem.Value);
	}
}

void UB2LobbyInventory::GetUnEquippedTotemList(TArray<FB2Totem>& OutResult)
{
	OutResult.Empty();
	TMap<int64, FB2Totem> AllTotems;
	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetAllTotems(AllTotems);

	for (TPair<int64, FB2Totem>& Elem : AllTotems)
	{
		if (Elem.Value.IsEquipped() == true)
		{
			continue;
		}

		OutResult.Add(Elem.Value);
	}
}

bool UB2LobbyInventory::NewAddStoredItem(FB2Item& InNewItem, bool bForceAdd, EPCClass InPCClass)
{
	InNewItem.bNewlyAddedForEquipSuggest = true; // 거의 이게 목적일 듯..

	return ReAddStoredItem(InNewItem, bForceAdd, InPCClass, true); // 기능상 여기서는 OptionalInsertIndex 는 필요없겠다.
}

bool UB2LobbyInventory::ReAddStoredItem(FB2Item& InNewItem, bool bForceAdd, EPCClass InPCClass, bool bUpdateUIDocImmediately /*= true*/)
{ // 여기에 InPCClass 인자가 과연 필요할까. InNewItem.AllowedPCClass 를 사용할수도
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	if (InNewItem.InventoryType == EItemInvenType::EIIVT_End)
	{
		return false;
	}

	if (!bForceAdd && IsInventoryFull(InNewItem.InventoryType, FinalPCClass))
	{
		return false;
	}

	bool bRetAdded = false;
	// 현재 설정에서의 리스트를 가져와서 더함.
	TMap<int64, FB2Item>* MapToAdd = GetStoredItemMapPtr(InNewItem.InventoryType, FinalPCClass);
	if (MapToAdd)
	{
		InNewItem.bCurrentlyEquipped = false; // 여기다 넣는다는 건 적어도 장착하지는 않는 것.
		MapToAdd->Add(InNewItem.InstanceUID, InNewItem);
		bRetAdded = true;
	}

	if (bUpdateUIDocImmediately)
	{ // 관련 UI Doc 데이터 업데이트.. 를 하지 않는 경우는 ReAdd 를 사용한 다른 동작
		LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	}

	return bRetAdded;
}

bool UB2LobbyInventory::InternalIsInventoryFull(bool IsOver, EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	if (InItemInvenType == EItemInvenType::EIIVT_End)
	{
		return true;
	}

	// 현재 타입에서 가능한 아이템 보관함 개수와 실제 아이템 개수
	int32 ContainerSize = GetItemContainerSize(InItemInvenType, FinalPCClass);
	int32 ItemNumInCurrentSlot = GetStoredItemNumInUnitedSlot(InItemInvenType, FinalPCClass);

	if (IsOver)
	{
		if (ItemNumInCurrentSlot > ContainerSize) 
			return true;
	}
	else
	{
		if (ItemNumInCurrentSlot >= ContainerSize)
			return true;
	}

	return false;
}

void UB2LobbyInventory::GetSharedConsumableOfType(TArray<FB2Item>& OutResult, int32 InRefID)
{
	// Sort 는 아마 의미가 없을 듯 하지만.. 아님 소모품 전용으로 sort 기준을 세워서 처리하게 될 수도. -> ConsumingAmount 로 하면 될 듯.
	BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().FindConsumablesOfType(InRefID, OutResult);
}

int32 UB2LobbyInventory::GetSharedConsumableAmountOfType(int32 InRefID)
{
	TArray<FB2Item> AllFound;
	GetSharedConsumableOfType(AllFound, InRefID);
	int32 RetAmount = 0;
	for (const FB2Item& ThisItem : AllFound)
	{
		RetAmount += ThisItem.ConsumingAmount;
	}
	return RetAmount;
}

#if !UE_BUILD_SHIPPING
void UB2LobbyInventory::DevOnlyGenerateDummyConsumable(int32 InRefID, int32 InAmount)
{
	if (InAmount <= 0){
		return;
	}

	// 1000개당 하나씩 만들어보자.. 아이템별로 다른지는 모르겠지만..
	const int32 AllowedAmountPerSingleItem = 1000;
	int32 GenerateItemNum = ((InAmount + AllowedAmountPerSingleItem-1) / AllowedAmountPerSingleItem);
	TArray<FB2Item> GeneratedData;
	int32 RemainingAmount = InAmount;
	for (int32 GI = 0; GI < GenerateItemNum; ++GI)
	{
		int32 ThisAmount = FMath::Min(RemainingAmount, AllowedAmountPerSingleItem);
		RemainingAmount = FMath::Max(0, RemainingAmount - ThisAmount);
		FB2Item NewConsumableData;
		// UID 겹치면 안되는데 어차피 치트기능이니 ㅋ
		NewConsumableData.InstanceUID = 438402842 + FMath::RandRange(1, INT_MAX) + FMath::RandRange(1, INT_MAX); 
		NewConsumableData.ItemRefID = InRefID;
		NewConsumableData.InventoryType = EItemInvenType::EIIVT_Consumables;
		NewConsumableData.ConsumingAmount = ThisAmount;

		GeneratedData.Add(NewConsumableData);
	}

	for (FB2Item& ThisToAdd : GeneratedData)
	{ // 정식으로 할 거 같으면 기존에 있는 거에 수량 같이 합체해야 할 수도.. 그건 어차피 서버에서 오는 거엔 처리가 되나..
		ReAddStoredItem(ThisToAdd, true, EPCClass::EPC_End, false);
	}
	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
}
#endif

bool UB2LobbyInventory::IsInventoryFull(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	return InternalIsInventoryFull(false, InItemInvenType, InPCClass);
}

bool UB2LobbyInventory::IsInventoryOverLimit(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	return InternalIsInventoryFull(true, InItemInvenType, InPCClass);
}

void UB2LobbyInventory::GetPCClassesNeedToCleanInventory(TArray<EPCClass>& OutClassList)
{
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		EPCClass CurrClass = IntToPCClass(PCI);
		if (IsInventoryFull(EItemInvenType::EIIVT_United, CurrClass))
		{
			OutClassList.Add(CurrClass);
		}
	}
}

bool UB2LobbyInventory::IsContainsAllPreset(const FB2Item& InItem)
{
	FPCItemContainer &ItemContainer = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems();
	return ItemContainer.IsInItemPresets(IntToPCClass(InItem.AllowedPCClass), InItem.InstanceUID);
}

bool UB2LobbyInventory::IsInOtherItemPresets(const FB2Item& InItem)
{
	FPCItemContainer &ItemContainer = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems();
	return ItemContainer.IsInOtherItemPresets(IntToPCClass(InItem.AllowedPCClass), InItem.InstanceUID);
}

void UB2LobbyInventory::SetChangeNewItemTofalseTap(EPCClass PCClass, EItemInvenType InvenType)
{
	TMap<int64, FB2Item>* InternalMap = GetStoredItemMapPtr(InvenType, PCClass);

	checkSlow(InternalMap);
	if (InternalMap)
	{
		for (auto El : *InternalMap)
		{
			if (El.Value.IsNew)
			{
				SetChangeNewItemTofalse(El.Key);
			}
		}
	}
}

void UB2LobbyInventory::SetChangeNewItemTofalse(int64 ItemInstanceUID)
{
	//아이템 New정보 false로 변경
	FB2Item* FoundPtr = FindFromAllPtr(ItemInstanceUID);
	if (FoundPtr)
	{
		FoundPtr->SetNewItemState();
	}
}

bool UB2LobbyInventory::FindStoredItem(FB2Item& OutFoundItem, int64 ItemInstanceUID)
{
	FB2Item* FoundPtr = FindStoredItemPtr(ItemInstanceUID);
	if (FoundPtr)
	{
		OutFoundItem = *FoundPtr;
		return true;
	}
	return false;
}

bool UB2LobbyInventory::FindEquippedItem(FB2Item& OutFoundItem, int64 ItemInstanceUID)
{
	FB2Item* FoundPtr = FindEquippedItemPtr(ItemInstanceUID);
	if (FoundPtr)
	{
		OutFoundItem = *FoundPtr;
		return true;
	}
	return false;
}

bool UB2LobbyInventory::FindFromAll(FB2Item& OutFoundItem, int64 ItemInstanceUID)
{
	FB2Item* Item = FindFromAllPtr(ItemInstanceUID);	
	if (Item)
	{
		OutFoundItem = *Item;
		return true;
	}

	return false;
}

FB2Item* UB2LobbyInventory::FindStoredItemPtr(int64 ItemInstanceUID)
{
	FB2Item* Item = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().FindItem(ItemInstanceUID);
	if (Item && !Item->bCurrentlyEquipped)
		return Item;

	return NULL;
}

FB2Item* UB2LobbyInventory::FindEquippedItemPtr(int64 ItemInstanceUID)
{
	FB2Item* Item = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().FindItem(ItemInstanceUID);
	if (Item && Item->bCurrentlyEquipped)
		return Item;

	return NULL;
}

FB2Item* UB2LobbyInventory::FindFromAllPtr(int64 ItemInstanceUID)
{
	return BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().FindItem(ItemInstanceUID);
}

int32 UB2LobbyInventory::GetAvailableAutoSelectItemCount(int32 StarGrade)
{
	FB2Item EnhanceTargetItem;
	if (GetItemOPTargetItemData(EnhanceTargetItem) == false)
		return 0;

	TArray<EItemInvenType> InvenTypeFilter;
	InvenTypeFilter.Add(EnhanceTargetItem.InventoryType);

	TArray<int32> StarGradeFilter;
	StarGradeFilter.Add(StarGrade);

	TArray<FB2Item> AllTargetItems;
	// 장착 및 잠금, 프리셋 있는거 제외.
	UB2LobbyInventory::GetFilteredItemList(AllTargetItems, false, false, false, InvenTypeFilter, StarGradeFilter);

	// 현재 레벨 업 대상이 되는 아이템만 있을 경우..
	if(EnhanceTargetItem.StarGrade == StarGrade && AllTargetItems.Num() == 1)
		return 0;
	return AllTargetItems.Num();
}

bool UB2LobbyInventory::UpdateSingleItemData(int64 ItemInstanceUID, FB2Item& InDataToUpdate)
{
	FB2Item* FoundItemPtr = FindFromAllPtr(ItemInstanceUID);
	if (!FoundItemPtr)
		return false; // 해당 아이템 못찾음.

	// InDataToUpdate 는 사실상 능력치 등의 데이터만 사용하기 위함으로, 아이템의 기초적인 정보는 일치해야 함.
	if (InDataToUpdate.InstanceUID != FoundItemPtr->InstanceUID || InDataToUpdate.ItemRefID != FoundItemPtr->ItemRefID || InDataToUpdate.ItemClass != FoundItemPtr->ItemClass
		|| InDataToUpdate.AllowedPCClass != FoundItemPtr->AllowedPCClass || InDataToUpdate.InventoryType != FoundItemPtr->InventoryType || InDataToUpdate.EquipPlace != FoundItemPtr->EquipPlace)
	{
		return false;
	}

	bool bWasEquipped = FoundItemPtr->bCurrentlyEquipped; // 장착 상태도 여기서 변경대상은 아님. 이건 return false 까지 하진 않는다.

	*FoundItemPtr = InDataToUpdate;
	FoundItemPtr->bCurrentlyEquipped = bWasEquipped; // 아마도 마찬가지여야겠지만 ㅋ

	if (FoundItemPtr->bCurrentlyEquipped)
	{
		// 관련 UI Doc 데이터 업데이트. 아직은 장착된 것에 대해서만 있음.
		UB2UIDocEquip* DocEquip = UB2UIDocHelper::GetDocEquip(FoundItemPtr->AllowedPCClass, (int32)FoundItemPtr->EquipPlace);
		if (DocEquip)
		{
			DocEquip->UpdateData(InDataToUpdate);
		}
	}
	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Signal(ItemInstanceUID); // 변경된 하나만 찝어서 업데이트. 추가나 제거시에는 이걸 사용하지 않는다.

	return true;
}

void UB2LobbyInventory::EquippedItems(const FB2EquippedItems& InEquippedItems)
{
	//int32 PresetID = GET_TUPLE_DATA(FB2ResponseEquippedItem::preset_id_index, InEquippedItems);
	//auto EquipItemUIDList = GET_TUPLE_DATA(FB2ResponseEquippedItem::equiped_item_ids_index, InEquippedItems);
	//auto UnequipItemUIDList = GET_TUPLE_DATA(FB2ResponseEquippedItem::unequiped_item_ids_index, InEquippedItems);
	//EquippedItems(PresetID, EquipItemUIDList, UnequipItemUIDList);
}

void UB2LobbyInventory::EquippedItems(const int32& PresetID, const TArray<int64>& EquipedItems, const TArray<int64>& UnequipedItems, bool UpdateBattleScore)
{
	//TArray<EPCClass> PCPartUpdateSignalClass; // 외관상으로 변경되는 아이템을 장착한 PC 클래스들 모두 모음. 각 클래스에다 신호를 보내기 위해
	//TArray<EPCClass> AnyEquipStateChangedClass; // 외관 장착 파트 변경되는 경우가 아니라도 전투력 반영은 될 수 있으니 클래스 따로 모음
	//bool bIsCostumeItem = false;

	//TArray<const FB2Item*> B2ItemPtrArray;
	//for (auto EquippedItemUID : EquipedItems)
	//{
	//	const FB2Item* EquippedItem = FindFromAllPtr(EquippedItemUID);
	//	if (EquippedItem != nullptr)
	//	{
	//		B2ItemPtrArray.Emplace(EquippedItem);

	//		if (GetPCPartForItemEquipPlace(EquippedItem->EquipPlace) != EPCPart::EPCPT_End)
	//		{ // 3D 메쉬 업데이트 시그널을 위한 것이니 장착한 것이 PC 파트 중 하나인 것에 한해.
	//			if (ItemAllowedPCClassEnum(EquippedItem->AllowedPCClass) == EPCClass::EPC_End) {
	//				// EPC_End 면 장착해도 표시가 안되는 아이템이거나 할 텐데 일단 모르니 모든 클래스를 돌도록 하자.
	//				for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI) {
	//					PCPartUpdateSignalClass.AddUnique(IntToPCClass(PCI));
	//				}
	//			}
	//			else {
	//				PCPartUpdateSignalClass.AddUnique(ItemAllowedPCClassEnum(EquippedItem->AllowedPCClass));
	//			}
	//		}
	//		else if (IsCostumeEquipPlace(EquippedItem->CostumeEquipPlace))
	//		{
	//			PCPartUpdateSignalClass.AddUnique(ItemAllowedPCClassEnum(EquippedItem->AllowedPCClass));
	//			bIsCostumeItem = true;
	//		}

	//		AnyEquipStateChangedClass.AddUnique(ItemAllowedPCClassEnum(EquippedItem->AllowedPCClass));

	//	}

	//}

	//TMap<EPCClass, TMap<int32, int32>> BeforeSetItemCounts;
	//TMap<EPCClass, TMap<int32, int32>> AfterSetItemCounts;

	//for (EPCClass& CurrUpdateClass : AnyEquipStateChangedClass)
	//{
	//	BeforeSetItemCounts.Add(CurrUpdateClass);
	//	AfterSetItemCounts.Add(CurrUpdateClass);
	//}

	//for (EPCClass& CurrUpdateClass : AnyEquipStateChangedClass)
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetSetItemValue(CurrUpdateClass, BeforeSetItemCounts[CurrUpdateClass]);

	//// EquipItems!!!!!!!
	//BladeIIGameImpl::GetClientDataStore().EquipItems(PresetID, EquipedItems, UnequipedItems);

	//for (EPCClass& CurrUpdateClass : AnyEquipStateChangedClass)
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetSetItemValue(CurrUpdateClass, AfterSetItemCounts[CurrUpdateClass]);

	//for (auto EquippedItemUID : EquipedItems)
	//{
	//	if (const FB2Item* EquippedItem = FindFromAllPtr(EquippedItemUID))
	//	{
	//		// 관련 UI Doc 데이터 업데이트.
	//		if (UB2UIDocEquip* DocEquip = UB2UIDocHelper::GetDocEquip(EquippedItem->AllowedPCClass, static_cast<int32>(EquippedItem->EquipPlace)))
	//			DocEquip->DoEquip(*EquippedItem);

	//		if (EquippedItem->IsNew)
	//		{
	//			SetChangeNewItemTofalse(EquippedItemUID);
	//		}
	//	}
	//}

	//// 장착 아이템 세부 정보 창이 열려 있다면 장착 장비 뷰를 막을 것이므로 닫는다.
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);

	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();

	//for (EPCClass& CurrUpdateClass : PCPartUpdateSignalClass)
	//{ // 해당 클래스의 3D 메쉬를 새로운 장착 상태 반영해서 업데이트
	//	LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(CurrUpdateClass);
	//}
	///*for (EPCClass& CurrUpdateClass : AnyEquipStateChangedClass)
	//{
	//}*/

	//if (UpdateBattleScore)
	//{
	//	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//	{
	//		EPCClass PCClassEnum = IntToPCClass(PCI);
	//		if (!PCPartUpdateSignalClass.Contains(PCClassEnum) && !AnyEquipStateChangedClass.Contains(PCClassEnum)) continue;

	//		UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(PCClassEnum, true);
	//	}
	//}

	////메시 업데이트 후, 애니메이션플레이.
	//for (auto EquippedItem : B2ItemPtrArray)
	//{
	//	EItemEquipPlace eItemEquipPlace = EquippedItem->EquipPlace;

	//	// 장착 슬롯 UI effect 도
	//	LobbyPlayUIEquipEffectClass<EItemEquipPlace>::GetInstance().Signal(eItemEquipPlace);

	//	//코스튬 부위일 경우 애니메이션 재생 부위 지정
	//	if (IsCostumeEquipPlace(EquippedItem->CostumeEquipPlace))
	//	{
	//		if (EquippedItem->CostumeEquipPlace == ECostumeEquipPlace::ECEP_Weapon)
	//		{
	//			eItemEquipPlace = EItemEquipPlace::EIEP_Weapon;
	//		}
	//		else
	//		{
	//			eItemEquipPlace = EItemEquipPlace::EIEP_BodyArmor;
	//		}
	//	}

	//	LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Signal(ItemAllowedPCClassEnum(EquippedItem->AllowedPCClass), eItemEquipPlace);
	//}

	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
	//
	//for (auto& UID : EquipedItems)
	//	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Signal(UID);

	//for (auto SetItemInfo : BeforeSetItemCounts)
	//	NoticeSetItemEffect(BeforeSetItemCounts[SetItemInfo.Key], AfterSetItemCounts[SetItemInfo.Key], bIsCostumeItem);
}

void UB2LobbyInventory::UnEquippedItems(const FB2UnequippedItems& UnEquippedItems)
{	
	//const TArray<int64>& ArrayUnEquipItemID = GET_TUPLE_DATA(FB2ResponseUnequippedItem::unequiped_item_ids_index, UnEquippedItems);
	//const int32 iPresetID = GET_TUPLE_DATA(FB2ResponseUnequippedItem::preset_id_index, UnEquippedItems);

	//UnEquippedItems_New(ArrayUnEquipItemID, iPresetID);
}

void UB2LobbyInventory::UnEquippedItems_New(const TArray<int64>& ArrayUnEquipItemID, const int32 iPresetID /*= -1*/)
{
	//TArray<EPCClass> PCPartUpdateSignalClass; // 외관상으로 변경되는 아이템을 장착한 PC 클래스들 모두 모음. 각 클래스에다 신호를 보내기 위해
	//TArray<EPCClass> AnyEquipStateChangedClass; // 외관 장착 파트 변경되는 경우가 아니라도 전투력 반영은 될 수 있으니 클래스 따로 모음
	//bool bIsCostumeItem = false;

	//for (auto UnequippedItemUID : ArrayUnEquipItemID)
	//{
	//	const FB2Item* UnequippedItem = FindFromAllPtr(UnequippedItemUID);
	//	if (UnequippedItem != nullptr)
	//	{
	//		if (GetPCPartForItemEquipPlace(UnequippedItem->EquipPlace) != EPCPart::EPCPT_End)
	//		{ // 3D 메쉬 업데이트 시그널을 위한 것이니 장착한 것이 PC 파트 중 하나인 것에 한해.
	//			if (ItemAllowedPCClassEnum(UnequippedItem->AllowedPCClass) == EPCClass::EPC_End) {
	//				// EPC_End 면 장착해도 표시가 안되는 아이템이거나 할 텐데 일단 모르니 모든 클래스를 돌도록 하자.
	//				for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI) {
	//					PCPartUpdateSignalClass.AddUnique(IntToPCClass(PCI));
	//				}
	//			}
	//			else {
	//				PCPartUpdateSignalClass.AddUnique(ItemAllowedPCClassEnum(UnequippedItem->AllowedPCClass));
	//			}
	//		}
	//		if (IsCostumeEquipPlace(UnequippedItem->CostumeEquipPlace))
	//		{
	//			PCPartUpdateSignalClass.AddUnique(ItemAllowedPCClassEnum(UnequippedItem->AllowedPCClass));
	//			bIsCostumeItem = true;
	//		}

	//		AnyEquipStateChangedClass.AddUnique(ItemAllowedPCClassEnum(UnequippedItem->AllowedPCClass));
	//	}
	//}

	//TMap<EPCClass, TMap<int32, int32>> BeforeSetItemCounts;
	//TMap<EPCClass, TMap<int32, int32>> AfterSetItemCounts;
	//for (EPCClass& CurrUpdateClass : AnyEquipStateChangedClass)
	//{
	//	BeforeSetItemCounts.Add(CurrUpdateClass);
	//	AfterSetItemCounts.Add(CurrUpdateClass);
	//}

	//for (EPCClass& CurrUpdateClass : AnyEquipStateChangedClass)
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetSetItemValue(CurrUpdateClass, BeforeSetItemCounts[CurrUpdateClass]);

	//// UnEquipItems!! 리뉴얼 예정
	//BladeIIGameImpl::GetClientDataStore().UnEquipItems(ArrayUnEquipItemID, iPresetID);

	//for (EPCClass& CurrUpdateClass : AnyEquipStateChangedClass)
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetSetItemValue(CurrUpdateClass, AfterSetItemCounts[CurrUpdateClass]);

	//for (auto UnequippedItemUID : ArrayUnEquipItemID)
	//{
	//	if (const FB2Item* UnequippedItem = FindFromAllPtr(UnequippedItemUID))
	//	{
	//		// 관련 UI Doc 데이터업데이트.
	//		if (UB2UIDocEquip* DocEquip = UB2UIDocHelper::GetDocEquip(UnequippedItem->AllowedPCClass, static_cast<int32>(UnequippedItem->EquipPlace)))
	//			DocEquip->RemoveEquip();
	//	}
	//}

	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();

	//for (EPCClass& CurrUpdateClass : PCPartUpdateSignalClass)
	//{ // 해당 클래스의 3D 메쉬를 새로운 장착 상태 반영해서 업데이트
	//	LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(CurrUpdateClass);
	//}
	///*for (EPCClass& CurrUpdateClass : AnyEquipStateChangedClass)
	//{
	//}*/

	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{
	//	EPCClass PCClassEnum = IntToPCClass(PCI);
	//	if (!PCPartUpdateSignalClass.Contains(PCClassEnum) && !AnyEquipStateChangedClass.Contains(PCClassEnum)) continue;

	//	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(PCClassEnum, true);
	//}

	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();

	//for (auto SetItemInfo : BeforeSetItemCounts)
	//	NoticeSetItemEffect(BeforeSetItemCounts[SetItemInfo.Key], AfterSetItemCounts[SetItemInfo.Key], bIsCostumeItem);

	//for (auto& UID : ArrayUnEquipItemID)
	//	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Signal(UID);
}

bool UB2LobbyInventory::GetEquippedItemAtPlace(FB2Item& OutFoundItem, EItemEquipPlace InEquipPlace, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);
	if (CurrPCItemAccessor)
	{
		return CurrPCItemAccessor->FindEquippedItem(OutFoundItem, InEquipPlace);
	}
	return false;
}

bool UB2LobbyInventory::GetEquippedCostumeAtPlace(FB2Item& OutFoundItem, ECostumeEquipPlace InEquipPlace, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);
	if (CurrPCItemAccessor)
	{
		return CurrPCItemAccessor->FindEquippedCostume(OutFoundItem, InEquipPlace);
	}
	return false;
}

void UB2LobbyInventory::GetAllEquippedItem(TArray<FB2Item>& OutList, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);

	if (CurrPCItemAccessor)
	{
		CurrPCItemAccessor->GetAllEquippedItems(OutList);
	}
}

void UB2LobbyInventory::GetAllEquippedCostumeItem(TArray<FB2Item>& OutList, EPCClass InPCClass /*= EPCClass::EPC_End*/)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);

	if (CurrPCItemAccessor)
	{
		CurrPCItemAccessor->GetAllEquippedCostumeItems(OutList);
	}
}

void UB2LobbyInventory::ConditionalCheckForBetterEquipSuggest()
{
	//bool bIgnoreSuggest = false;

	//// 1. 반복 전투 도중에는 새로 얻은 아이템에 대한 장착권장을 무시
	//// 2. Tutorial도중에는 장착권장 무시
	//if (TransientLobbyGM && TransientLobbyGM->GetRepeatBattleRequestState().bIsOn ||
	//	TutorialManager::GetInstance().HasAnyTutorial())
	//	bIgnoreSuggest = true;

	//if (bIgnoreSuggest)
	//{
	//	IgnoreBetterEquipSuggest();
	//}
	//else
	//{
	//	CheckForBetterEquipSuggest();
	//}
}

void UB2LobbyInventory::CheckForBetterEquipSuggest()
{
	// For all PC, and all possible EquipPlace.
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		for (int32 EQI = 0; EQI < (int32)EItemEquipPlace::EIEP_End; ++EQI)
		{
			if (CheckForBetterEquipSuggestInner(static_cast<EItemEquipPlace>(EQI), IntToPCClass(PCI)))
			{ // 이번 CheckForBetterEquipSuggestInner 에서 좋은 장비가 있어서 권장 대화상자를 열었다면 대화상자 중복해서 열지 않게 리턴. 그쪽에서 다시 여길 call 할 것. 
				return;
			}
		}
	}	
}

bool UB2LobbyInventory::CheckForBetterEquipSuggestInner(EItemEquipPlace InEquipPlace, EPCClass InPCClass)
{
	//FB2Item EquippedOne;
	//const bool bHadEquipped = GetEquippedItemAtPlace(EquippedOne, InEquipPlace, InPCClass);
	////if (!bHadEquipped)
	////{ // 만일 기존 장착된 게 아예 없다면 권장을 안할 수도 있고.. 걍 젤 존걸로 권장할 수도.
	////	return false; // 이렇게 하면 권장 안 하는 걸로.
	////}

	//TArray<FB2Item> StoredList;
	//GetStoredItemListForEquipPlace(StoredList, InEquipPlace, InPCClass);
	//
	//// bNewlyAddedForEquipSuggest 체크된 걸로만 다시 필터링
	//TArray<FB2Item> NewlyAddedList = StoredList.FilterByPredicate( 
	//	[](FB2Item CurrItem)
	//	{
	//		if (CurrItem.bNewlyAddedForEquipSuggest){
	//			return true;
	//		}
	//		return false;
	//	}
	//);
	//
	//struct FItemStarGradeSorter // 일차적으로 PrimaryPoint 로 비교할 거지만 혹시라도 동일한 게 있을 때 기본적인 StarGrade 라도 볼 수 있게 기준을 좀 더 두자..
	//{
	//	bool operator()(const FB2Item& A, const FB2Item& B) const
	//	{
	//		return A.StarGrade > B.StarGrade;
	//	}
	//};
	//NewlyAddedList.StableSort(FItemStarGradeSorter());
	//struct FPrimaryPointSorter // PrimaryPoint 젤 존 거 순서대로 비교하기 위해 소팅.
	//{
	//	bool operator()(const FB2Item& A, const FB2Item& B) const
	//	{
	//		return A.GetPrimaryPoint() > B.GetPrimaryPoint();
	//	}
	//};
	//NewlyAddedList.StableSort(FPrimaryPointSorter());

	//if (NewlyAddedList.Num() > 0)
	//{
	//	bool bOpenedDialog = false;

	//	for (FB2Item& CurrCadid : NewlyAddedList)
	//	{ // PrimaryPoint 순으로 소팅하였으니 가장 앞에꺼 사용하면 됨. 단, Type 까지 비교를.. 하는 게 좋겠지..?
	//		if (!bHadEquipped || (CurrCadid.PrimaryPointType == EquippedOne.PrimaryPointType 
	//			&& (B2UIUtil::DifferenceItemPrimaryPoint(CurrCadid, EquippedOne) >= 1)))
	//		{
	//			DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_AcqItemEquipSuggest, FDJLegacySubpopupAddPanelInfo());

	//			check(TransientLobbyGM); // 없으면 바보.
	//			UB2AcqItemEquipSuggest* AcqItemEquipSuggestPopup = Cast<UB2AcqItemEquipSuggest>(TransientLobbyGM->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_AcqItemEquipSuggest));
	//							
	//			if (AcqItemEquipSuggestPopup)
	//			{
	//				AcqItemEquipSuggestPopup->UpdateData(bHadEquipped, EquippedOne, CurrCadid,
	//					// 선택 이후 CheckForBetterEquipSuggest 를 다시 콜할 수 있도록.
	//					[this](){CheckForBetterEquipSuggest(); }
	//				);
	//			}

	//			bOpenedDialog = true;
	//			break;
	//		}
	//	}

	//	for (FB2Item& CurrNew : NewlyAddedList)
	//	{ // 더 좋은 게 있었든 없었든 체크는 하였으니 이번 리스트에서는 bNewlyAddedForEquipSuggest 를 모두 reset
	//		CurrNew.bNewlyAddedForEquipSuggest = false;
	//		UpdateSingleItemData(CurrNew.InstanceUID, CurrNew);
	//	}
	//			
	//	return bOpenedDialog; // 이번에 대화상자를 열었다면 true 를 리턴해서 상위단에서 중복해서 열지 않게 한다. 어차피 체크는 다 할 것임.
	//}

	return false;
}

void UB2LobbyInventory::IgnoreBetterEquipSuggest()
{
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{// 걍 모든 아이템 돌면서 bNewlyAddedForEquipSuggest 리셋..
		TArray<FB2Item> ThisList;
		GetMergedItemList(ThisList, true, IntToPCClass(PCI));
		for (FB2Item& ThisItem : ThisList)
		{
			if (ThisItem.bNewlyAddedForEquipSuggest)
			{
				ThisItem.bNewlyAddedForEquipSuggest = false;
				UpdateSingleItemData(ThisItem.InstanceUID, ThisItem);
			}
		}
	}
}

void UB2LobbyInventory::GetEquipSuggestion(TArray<int64>& OutSuggestedIDs, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	for (int32 EQI = 0; EQI < (int32)EItemEquipPlace::EIEP_End; ++EQI)
	{ // 장착되지 않은 곳에 대해서도 검색을.
		FB2Item EquippedOne;
		const bool bHadEquipped = GetEquippedItemAtPlace(EquippedOne, static_cast<EItemEquipPlace>(EQI), FinalPCClass);

		// 동일 위치에서 더 나은 포인트를 올릴 수 있는 것을 찾는다.
		TArray<FB2Item> StoredListAtPlace;
		GetStoredItemListForEquipPlace(StoredListAtPlace, static_cast<EItemEquipPlace>(EQI), FinalPCClass);
		
		float BestScoreSoFar = -1.0f;
		int64 CurrentBestUID = 0;
		for (FB2Item& CurrStored : StoredListAtPlace)
		{
			if ((!bHadEquipped || (B2UIUtil::DifferenceItemPrimaryPoint(CurrStored, EquippedOne) >= 1)) && // 장착된 것이 없다면 단순히 젤 쎈 걸로.
				(!bHadEquipped || CurrStored.PrimaryPointType == EquippedOne.PrimaryPointType) &&
				CurrStored.GetPrimaryPoint() > BestScoreSoFar)
			{
				BestScoreSoFar = CurrStored.GetPrimaryPoint();
				CurrentBestUID = CurrStored.InstanceUID;
			}
		}

		if (CurrentBestUID > 0) // 이번 위치에 장착된 것보다 (혹은 비어있는 칸에) 더 좋은 것을 찾음.
		{
			OutSuggestedIDs.Add(CurrentBestUID);
		}
	}
}

//float UB2LobbyInventory::GatherPrimPointFromList(EItemPrimaryPointType InPrimPointType, const TArray<FB2Item>& InItemList, const FB2Wing* InWing)
//{
//	EItemOption AssociatedOption = CombatStatEval::GetPrimPointIncOption(InPrimPointType);
//	if (InPrimPointType == EItemPrimaryPointType::EIPP_End || AssociatedOption == EItemOption::EIO_End){
//		return 0.0f;
//	}
//
//	float AllGathered = 0.0f;
//	for (int32 II = 0; II < InItemList.Num(); ++II)
//	{
//		const FB2Item& CurrCheck = InItemList[II];
//
//		if (CurrCheck.PrimaryPointType == InPrimPointType){
//			AllGathered += CurrCheck.GetPrimaryPoint(); // 기본 prim point 더함. 여기서 옵션값까지 카운트하지는 않는다. 기본 PrimaryPointType 이 InPrimPointType 과 다른 경우 어차피 아래 옵션값 취합이 필요하기 때문에.
//		}
//
//		// 해당 prim point 의 증가옵션이 있다면 그것도 더한다.
//		// AllGathered += FMath::Max(GetEffectiveItemOptionValue(AssociatedOption, CurrCheck), 0.0f); // 단순 증가 옵션. 더함. 근데 음수가 나오는 경우도 있나..?
//	}
//	// Wing 도 여기서 추가.
//	float AdditionalWingValue = 0.0f;
//	if (InWing)
//	{
//		AdditionalWingValue = (InPrimPointType == EItemPrimaryPointType::EIPP_Attack) ? InWing->GetAttackPoint() :
//			(InPrimPointType == EItemPrimaryPointType::EIPP_Defense ? InWing->GetDefensePoint() : InWing->GetHealthPoint());
//	}
//
//	return AllGathered + AdditionalWingValue;
//}

//float UB2LobbyInventory::GetEquippedPrimPointLocalPlayer(EItemPrimaryPointType InPrimPointType, EPCClass InPCClass)
//{
//	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.
//
//	TArray<FB2Item> AllEquipped;
//	GetAllEquippedItem(AllEquipped, FinalPCClass);
//
//	// Wing 도 아이템과 유사한 시스템이고 옵션 타입이 같다 보니 여기서 처리.
//	bool bFoundWingData = false;
//	FB2Wing AdditionalWingData;
//	bFoundWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(FinalPCClass, AdditionalWingData);
//
//	return GatherPrimPointFromList(InPrimPointType, AllEquipped, (bFoundWingData ? &AdditionalWingData : NULL));
//}

//void UB2LobbyInventory::GetAllEquippedPrimPointsLocalPlayer(TMap<EItemPrimaryPointType, float>& OutAllPrimPoints, EPCClass InPCClass)
//{
//	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.
//
//	for (int32 PTI = 0; PTI < (int32)EItemPrimaryPointType::EIPP_End; ++PTI)
//	{
//		float ThisTypePoint = GetEquippedPrimPointLocalPlayer((EItemPrimaryPointType)PTI, FinalPCClass);
//		if (ThisTypePoint > 0.0f) // 0 이하면 현재 장착에선 없는 거
//		{
//			OutAllPrimPoints.Add((EItemPrimaryPointType)PTI, ThisTypePoint);
//		}
//	}
//}

//float UB2LobbyInventory::GetEquippedOptionPointLocalPlayer(EItemOption InOptionToCheck, EPCClass InPCClass)
//{
//	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.
//
//	TArray<FB2Item> AllEquipped;
//	GetAllEquippedItem(AllEquipped, FinalPCClass);
//
//	// Wing 도 아이템과 유사한 시스템이고 옵션 타입이 같다 보니 여기서 처리.
//	bool bFoundWingData = false;
//	FB2Wing AdditionalWingData;
//	bFoundWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(FinalPCClass, AdditionalWingData);
//
//	return GetEquippedOptionPoint(AllEquipped, (bFoundWingData ? &AdditionalWingData : NULL), InOptionToCheck);
//}

//float UB2LobbyInventory::GetEquippedOptionPoint(const TArray<FB2Item>& AllEquipped, const FB2Wing* InWing, EItemOption InOptionToCheck)
//{
//	TArray<float> AllFoundRawValues;
//	for (auto& CurrEquip : AllEquipped)
//	{ // 고유 옵션이든 기냥 옵션이든 모두.
//		for (auto& CurrOption : CurrEquip.IntrinsicOptions)
//		{
//			if (CurrOption.OptionType == InOptionToCheck)
//			{
//				AllFoundRawValues.Add(CurrOption.RawOptionAmount);
//			}
//		}
//		for (auto& CurrOption : CurrEquip.RandomOptions)
//		{
//			if (CurrOption.OptionType == InOptionToCheck)
//			{
//				AllFoundRawValues.Add(CurrOption.RawOptionAmount);
//			}
//		}
//	}
//
//	// Wing 도 아이템과 유사한 시스템이고 옵션 타입이 같다 보니 여기서 처리.
//	if (InWing)
//	{
//		const FWingPropOption* ThisPropOption = InWing->GetPropOptionPtr(InOptionToCheck); 
//		if (ThisPropOption && ThisPropOption->bIsOpen)
//		{
//			AllFoundRawValues.Add(ThisPropOption->RawOptionAmount);
//		}
//	}
//	
//	// AllFoundRawValues 가 없더라도 GetEffectiveItemOptionValue 로 넘긴다. 없는 경우에 대한 기본값을 옵션 타입에 따라 리턴해 줄 것.
//	return GetEffectiveItemOptionValue(InOptionToCheck, AllFoundRawValues);
//}

/* 좀 복잡해져서 일단 없애고.. 혹시 필요해지면 Wing 데이터도 고려하도록.
void UB2LobbyInventory::GetAllEquippedOptionPointsLocalPlayer(TMap<EItemOption, float>& OutAllOptions, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 지정 시 현재 선택된 걸로.

	TArray<FB2Item> AllEquipped;
	GetAllEquippedItem(AllEquipped, FinalPCClass);

	// 종류별 Option 들 중간 취합
	TMap<EItemOption, TArray<float>> IntermOptionMap;

	for (FB2Item& CurrEquip : AllEquipped)
	{ // 고유 옵션이든 기냥 옵션이든 모두.
		for (FItemOption& CurrOption : CurrEquip.IntrinsicOptions)
		{
			float NewValue = CurrOption.RawOptionAmount;
			TArray<float>* ExistingValueArray = IntermOptionMap.Find(CurrOption.OptionType);
			if (ExistingValueArray)
			{ // 여기선 단순히 값을 넣기만 하고 모두 모은 후 최종적인 값으로 다시 가공할 것.
				ExistingValueArray->Add(NewValue);
			}
			else
			{
				TArray<float> NewArray;
				NewArray.Add(NewValue);
				IntermOptionMap.Add(CurrOption.OptionType, NewArray);
			}
		}
		for (FItemOption& CurrOption : CurrEquip.RandomOptions)
		{
			float NewValue = CurrOption.RawOptionAmount;
			TArray<float>* ExistingValueArray = IntermOptionMap.Find(CurrOption.OptionType);
			if (ExistingValueArray)
			{ // 여기선 단순히 값을 넣기만 하고 모두 모은 후 최종적인 값으로 다시 가공할 것.
				ExistingValueArray->Add(NewValue);
			}
			else
			{
				TArray<float> NewArray;
				NewArray.Add(NewValue);
				IntermOptionMap.Add(CurrOption.OptionType, NewArray);
			}
		}
	}

	for (auto& IntermElem : IntermOptionMap)
	{ // 모아놓은 걸로 최종적인 가공.
		float FinalScaled = GetEffectiveItemOptionValue(IntermElem.Key, IntermElem.Value);
		OutAllOptions.Add(IntermElem.Key, FinalScaled);
	}
}*/

void UB2LobbyInventory::EnterItemOpMode(ELobbyInvenItemOpMode InMode, int64 TargetItemID, EItemInvenType OpenInventoryType /*= EItemInvenType::EIIVT_End*/, bool IsBackupRollBackInventoryTab /*= false*/)
{
	BII_CHECK(AutoItemLevelupPhase == EAutoItemLevelupPhase::AEPhase_None);
	if (AutoItemLevelupPhase != EAutoItemLevelupPhase::AEPhase_None){
		return;
	}

	//ResetAllIngredsList(); // 확실히..
	//ResetEnhanceItemDatas();

	//GhostItemOpIngreds.Empty();
	//CachedItemOpTargetData = FB2Item(); // 서버로 요청 보낸 이후에나 캐싱 됨.
	//OnCommitItemOpHeroMgmtPCBackup = EPCClass::EPC_End;
	//OnCommitItemOpInvenPageTabBackup = EItemInvenType::EIIVT_End;
	//CurrentItemOpMode = InMode;
	//ItemOpTargetItemID = TargetItemID;
	//bFreezeitemOpModeIngredSelection = false;

	//if (IsBackupRollBackInventoryTab && TransientLobbyGM && TransientLobbyGM->DJLegacy_GetLobbyUIManager())
	//	CustomBackupInvenPageTab = TransientLobbyGM->DJLegacy_GetLobbyUIManager()->GetInventoryTabSelection();

	//// 생성된 Popup widget 을 UIP 처럼 배치해 넣을 패널 정보를 만들어준다.
	//UB2LobbyUI_InventoryMain* InventoryUI = TransientLobbyGM ? Cast<UB2LobbyUI_InventoryMain>(TransientLobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
	//UPanelWidget* PopupAddPanel = InventoryUI ? InventoryUI->GetItemDetailPopupParentPanel(true) : nullptr;
	//
	//// 기타 나머지 모드의 재료 선택 상태도.

	//// Mode 랑 TargetID 등 세팅부터 하고 UI 를 열어야 함.
	//switch (CurrentItemOpMode)
	//{
	//case ELobbyInvenItemOpMode::EIVIOP_None: 
	//	// Do nothing
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Levelup:
	//	{	
	//		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemLevelUpMain, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));
	//	}
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Upgrade:
	//	{
	//		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemUpgradeMain, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));
	//	}
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Composition:
	//	{
	//		auto ePCClass = GetInventorySelectedPCClass();
	//		int32 currentItemPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(ePCClass);
	//		data_trader::Retailer::GetInstance().RequestGetItemComposeCost(ItemOpTargetItemID, currentItemPresetId);

	//		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemComposeMain, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));
	//	}
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Surpass:
	//	{
	//		data_trader::Retailer::GetInstance().RequestGetItemSurpassCost(ItemOpTargetItemID);

	//		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemSurpassMain, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));
	//	}
	//	break;

	//case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
	//	{		
	//		SetCurEnhanceItemData(ItemOpTargetItemID);

	//		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceMain, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));
	//	}
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_EnhancementChange:
	//{
	//	FB2Item TargetItemData;
	//	if (FindFromAll(TargetItemData, ItemOpTargetItemID))
	//	{
	//		if (TargetItemData.bIsLocked)
	//		{
	//			int32 TargetItemUID = TargetItemData.InstanceUID;
	//			OpenPopupItemLock(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhancementExchange_Title")), TargetItemUID);

	//			CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_None;
	//			return;
	//		}
	//		else
	//		{
	//			DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceChangeMain, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));
	//		}
	//	}
	//}
	//break;

	//case ELobbyInvenItemOpMode::EIVIOP_HeroChange:
	//{
	//	//CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_None;
	//	FB2Item TargetItemData;
	//	if (FindFromAll(TargetItemData, ItemOpTargetItemID))
	//	{	
	//			UB2LobbyUI_ItemHeroChangeMain* HeroChangeMain = UB2UIManager::GetInstance()->OpenUI<UB2LobbyUI_ItemHeroChangeMain>(UIFName::ItemHeroChangeMain);

	//			if (HeroChangeMain)
	//			{
	//				HeroChangeMain->InitBase();
	//				HeroChangeMain->UpdateItemIcons();
	//				HeroChangeMain->SetInActive(static_cast<EPCClass>(TargetItemData.AllowedPCClass));
	//				HeroChangeMain->SetCostInfo(DarkCrystalCost, ChangeItem);
	//			}
	//			CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_None;
	//	}

	//	return; //전체 팝업이라 뒤에 바꿔줄필요없다
	//	//DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemHeroChangeMain, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));
	//}
	//break;
	//case ELobbyInvenItemOpMode::EIVIOP_EnhancementChangeMode:
	//{

	//	CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_None;
	//	FB2Item TargetItemData;
	//	if (FindFromAll(TargetItemData, ItemOpTargetItemID))
	//	{

	//		UB2UIItemChangePopupMode* PopUpUI = UB2UIManager::GetInstance()->OpenUI<UB2UIItemChangePopupMode>(UIFName::MsgEnhancementChangeMode);
	//		check(PopUpUI)
	//		PopUpUI->ItemSlotSetting(TargetItemData);
	//		//강화전환
	//		PopUpUI->AddHandler(EUIItemChangePopupMode::Enhance, FItemChangePopupMode::CreateLambda([this]()
	//		{	this->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_EnhancementChange, ItemOpTargetItemID); }));

	//		//옵션 변경
	//		PopUpUI->AddHandler(EUIItemChangePopupMode::Option, FItemChangePopupMode::CreateLambda([TargetItemData]()
	//		{	UB2UIItemOptionChange* ItemOptionChangeWidget = UB2UIManager::GetInstance()->OpenUI<UB2UIItemOptionChange>(UIFName::ItemOptionChange);
	//		if (ItemOptionChangeWidget)
	//		{
	//			ItemOptionChangeWidget->SetTargetItem(TargetItemData);
	//		}
	//		}));
	//		
	//		//영웅전환
	//		PopUpUI->AddHandler(EUIItemChangePopupMode::Hero, FItemChangePopupMode::CreateLambda([this, TargetItemData]()
	//		{	
	//			if (TargetItemData.bIsLocked)
	//			{
	//				int32 TargetItemUID = TargetItemData.InstanceUID;
	//				OpenPopupItemLock(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ExchangeHero_Hero")), TargetItemUID);

	//				CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_None;
	//				return;
	//			}
	//			else
	//				data_trader::Retailer::GetInstance().RequestGetChangeItemCharacterTypeCost(ItemOpTargetItemID);
	//		}));

	//		
	//		//등급 변경
	//		PopUpUI->AddHandler(EUIItemChangePopupMode::Quality, FItemChangePopupMode::CreateLambda([TargetItemData]()
	//		{
	//			DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_QualityChange, FDJLegacySubpopupAddPanelInfo());
	//			check(TransientLobbyGM); // 없으면 바보.
	//			UB2UIItemQualityChange* ItemQualityChangeWidget = Cast<UB2UIItemQualityChange>(TransientLobbyGM->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_QualityChange));
	//			if (ItemQualityChangeWidget)
	//			{
	//				ItemQualityChangeWidget->SetTargetItem((FB2Item&)TargetItemData, TransientLobbyGM);
	//				data_trader::Retailer::GetInstance().RequestGetItemRandomQuality(TargetItemData.InstanceUID);
	//			}
	//		}
	//		));
	//		
	//	}
	//	return;
	//	//woong
	//	/*UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::EnhancementChangeMode,
	//				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
	//				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_HigherGradeThanEnhanceTarget"))),
	//				0.f,
	//				true,
	//				true,
	//				EUIMsgPopupButtonGroup::EnhancementChangeMode,
	//				FMsgPopupOnClick::CreateLambda([this]() {
	//				this->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_EnhancementChange, ItemOpTargetItemID);
	//			})
	//			);*/
	//}
	//case ELobbyInvenItemOpMode::EIVIOP_TotemSmelting:
	//{
	//	OpenTotemSmeltingInfoPopupClass<int64>::GetInstance().Signal(TargetItemID);
	//}
	//break;
	//case ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume:
	//{
	//	SetCurEnhanceItemData(ItemOpTargetItemID);

	//	//DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceMain, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));
	//}
	//break;
	//	// Note :	이제부터는 강화 비용(Cost)관련해서 서버에 요청하지않고, 기존에 받아두었던 마스터데이터에서 참조하도록 변경 (2017-04)
	//	//			기존 것도 서버/클라 작업자가 날잡고 한번에 변경하면 좋겠는데....
	//}


	//if (OpenInventoryType == EItemInvenType::EIIVT_End)
	//{
	//	// 타겟 아이템과 같은 타입의 탭으로 강제 선택. 
	//	// QuitItemOpMode 에서도 비슷한 동작을 하는데 여기서 탭선택 자체를 잠근 건 풀어주어야 한다.
	//	FB2Item TargetItemData;
	//	if (FindFromAll(TargetItemData, ItemOpTargetItemID))
	//	{
	//		LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(TargetItemData.InventoryType, true);
	//	}
	//}
	//else
	//{
	//	LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(OpenInventoryType, true);
	//}

	//// 보관함 아이템 세부 정보 창이 열려 있다면 닫는다. 보관함에서 재료를 꺼내야 하기 때문에.
	//// 전환모드(전체 팝업 창)일 경우 닫아주지 않는다
	//
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_StoredItemDetail);
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);
	//ChangeInvenItemOpModeClass<ELobbyInvenItemOpMode>::GetInstance().Signal(CurrentItemOpMode);
	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal(); // 인벤토리 보관함 뷰 업데이트 시에는 Target 아이템은 보이지 않게 될 것.
	//
}

void UB2LobbyInventory::QuitItemOpMode(bool bQuitFromOpSuccess)
{
//	// 모드를 종료하기 전, 이전 Op 모드 성공 이후라면 연달아서 Op 모드를 지속할 수 있으므로 업그레이드 된 아이템 데이터 등을 보관해 둔다.
//	FB2Item PrevSuccessTargetData;
//	const bool bGotPrevItemOpTarget = GetItemOPTargetItemData(PrevSuccessTargetData, false);
//	const ELobbyInvenItemOpMode JustFinishedOpMode = CurrentItemOpMode;
//	
//	TArray<FB2Item> PrevSelectedIngredItems;
//	GetAllGhostIngreds(PrevSelectedIngredItems);
//
//	bool IsReturnTotemInventory = (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_TotemSmelting ? true : false);
//
//	CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_None;
//	ItemOpTargetItemID = 0;
//	bFreezeitemOpModeIngredSelection = false;
//	CurrentBaseItemOpGoldCost = 0;
//	GhostItemOpIngreds.Empty(); // GhostItemOpIngreds 는 따로 비움 처리
//	CachedItemOpTargetData = FB2Item();
//#if WITH_EDITOR
//	bItemOpProgressSceneTestMode = false;
//#endif
//
//	ResetAllIngredsList();
//	ResetEnhanceItemDatas();
//
//	// 열려 있을지 모를 아이템 강화 관련 메뉴들을 모두 닫는다. (바로 아래에서 다시 열릴 수도 있지만 ㅋ)
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemLevelUpMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemUpgradeMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemComposeMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemSurpassMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceChangeMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemHeroChangeMain);
//		
//	LobbyInvenHideItemOpModeGuideTextClass<>::GetInstance().Signal(); // 가이드 텍스트도 표시되어 있을 수 있으므로 닫음.
//
//	if (bQuitFromOpSuccess && // 그냥 확인차원에서 QuitItemOpMode 를 콜하는 경우도 있음. 실제 Op mode 성공 시 불리는 경우의 처리들.
//		TransientLobbyGM && TransientLobbyGM->IsValidObj() && TransientLobbyGM->IsHeroMgmtMode()) 
//	{
//		// 강화 연출 및 성공 페이지 등이 popup 이 아닌 별도 페이지로 되어 있으므로 인벤토리로 돌아옴.
//		// 언제나 하더라도 딱히 문제는 안되는데 필요 없을때에도 계속 이게 실행되면 보기 사나움 ㅋ
//		// 영웅관리 모드가 아니라면 이미 DJLegacy UI page 는 닫힌 상태로 메인 로비창으로 나와 있을 것이므로 다시 진입하면 안됨.
//
//		ELobbyUIPages NextPage;
//		switch (JustFinishedOpMode)
//		{
//		case ELobbyInvenItemOpMode::EIVIOP_TotemSmelting:
//			NextPage = ELobbyUIPages::ELUP_Totem;
//			break;
//		case ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume:
//			NextPage = ELobbyUIPages::ELUP_Costume;
//			break;
//		default:
//			NextPage = ELobbyUIPages::ELUP_InventoryMain;
//			break;
//		}
//
//		DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(NextPage);
//
//		// LobbyGM 쪽 연출용 뷰 세팅도 종료. 
//		LobbyQuitItemOpDirectingViewClass<>::GetInstance().Signal();
//		
//		// 인벤토리 UI를 새로 여는데 이전 탭을 유지하게 좀 처리.
//		if (OnCommitItemOpHeroMgmtPCBackup != EPCClass::EPC_End)
//			TransientLobbyGM->SetHeroMgmtModePCSelection(OnCommitItemOpHeroMgmtPCBackup);
//
//		//if (OnCommitItemOpInvenPageTabBackup != EItemInvenType::EIIVT_End)
//		{// EnterItemOpMode 시 탭선택 잠근 거 풀어주는 것도 포함. 따라서 설령 OnCommitItemOpInvenPageTabBackup 이 예상과 다르게 들어가 있어도 이건 bool 인자를 false 로 해서 꼭 불러준다.
//			LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(OnCommitItemOpInvenPageTabBackup, false);
//		}
//
//		if (IsReturnTotemInventory)
//		{
//			// 토템에서 돌아온 상태라면 다시 창을 띄워주도록 하자.
//			ReturnTotemInventoryClass<int64>::GetInstance().Signal(PrevSuccessTargetData.InstanceUID);
//		}
//		else
//		{
//			if (bGotPrevItemOpTarget)
//			{
//				// 강화/레벨업이 계속 가능하다면 창을 띄워주도록 하자.
//
//				if (JustFinishedOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup && IsItemLevelupAllowed(PrevSuccessTargetData))
//				{
//					EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_Levelup, PrevSuccessTargetData.InstanceUID);
//				}
//				else if (JustFinishedOpMode == ELobbyInvenItemOpMode::EIVIOP_Enhancement && IsItemEnhanceAllowed(PrevSuccessTargetData))
//				{
//					// 강화의 경우 재료들도 미리 넣어준다. 
//					EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_Enhancement, PrevSuccessTargetData.InstanceUID, EItemInvenType::EIIVT_Consumables);
//
//					if (PrevSelectedIngredItems.Num() > 0)
//					{
//						// 999개가 넘으면 아이템 InstanceUID가 여러개일수 있으므로 RefID기준으로 다시 계산을 해준다.
//						const int32 EnhanceIngredRefId = PrevSelectedIngredItems[0].ItemRefID;
//						if (GetSharedConsumableAmountOfType(EnhanceIngredRefId) > 0)
//						{
//							TArray<FB2Item> IngredsItems;
//							GetSharedConsumableOfType(IngredsItems, EnhanceIngredRefId);
//
//							if (IngredsItems.Num() > 0)
//								SelectForIngred(IngredsItems.Last().InstanceUID, ELobbyInvenItemOpMode::EIVIOP_Enhancement);
//						}
//					}
//				}
//				else if (JustFinishedOpMode == ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume)
//				{
//					OnCallBackEnhanceCostumeInvenPageClass<const FB2Item&>::GetInstance().Signal(PrevSuccessTargetData);
//				}
//				else
//				{ // 기타 다른 경우라면 걍 세부 정보 창 다시 띄우는 걸로. 합성이나 승급을 해야 할 상황이라도 둘 중 하나를 선택해야 하니 걍 세부창으로 ㅋ
//					UB2DynItemIcon::CreateLobbyItemDetailPopupCommon(TransientLobbyGM, PrevSuccessTargetData, PrevSuccessTargetData.bCurrentlyEquipped);
//				}
//			}
//		}
//	}
//	else
//	{	
//		// 탭선택 잠긴 거 풀어주는 건 어떤 상황에서든
//		LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(CustomBackupInvenPageTab, false);
//		CustomBackupInvenPageTab = EItemInvenType::EIIVT_End;
//	}
//
//	OnCommitItemOpInvenPageTabBackup = EItemInvenType::EIIVT_End;
//	OnCommitItemOpHeroMgmtPCBackup = EPCClass::EPC_End;
//
//	ChangeInvenItemOpModeClass<ELobbyInvenItemOpMode>::GetInstance().Signal(CurrentItemOpMode);
}

bool UB2LobbyInventory::GetItemOPTargetItemData(FB2Item& OutData, bool bGetCachedNonUpdatedData) const
{
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None)
	{
		return false;
	}

	if (bGetCachedNonUpdatedData)
	{ // Item OP req 동작 자체는 한 상황에서 업데이트 하기 이전 값 가져옴.
		if (CachedItemOpTargetData.ItemRefID > 0 && CachedItemOpTargetData.InstanceUID > 0)
		{
			OutData = CachedItemOpTargetData;
			return true;
		}
	}
	else 
	{
		if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_TotemSmelting)
		{
			TMap<int64, FB2Totem> AllTotems;
			BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetAllTotems(AllTotems);

			if (AllTotems.Contains(ItemOpTargetItemID))
			{
				AllTotems[ItemOpTargetItemID].GetItemFormat(OutData);
				return true;
			}
		}
		else
		{
			if (FindFromAll(OutData, ItemOpTargetItemID)) // 강화 및 승급 등의 타겟은 장착 아이템일 수도 있다.
			{
				return true;
			}
		}
	}

	return false;
}

bool UB2LobbyInventory::IsItemOpTargetItem(const FB2Item& InCheckItem)
{
	return (CurrentItemOpMode != ELobbyInvenItemOpMode::EIVIOP_None && InCheckItem.InstanceUID == ItemOpTargetItemID);
}

bool UB2LobbyInventory::IsIngredsFull()
{
	switch (CurrentItemOpMode)
	{
	case ELobbyInvenItemOpMode::EIVIOP_Levelup:
		return PendingSelectItemOpIngreds.Num() >= ItemLevelupIngredLimit;
	case ELobbyInvenItemOpMode::EIVIOP_Composition:
	case ELobbyInvenItemOpMode::EIVIOP_Surpass:
	case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
		return PendingSelectItemOpIngreds.Num() >= 1;
		break;
	}

	return false;
}

int32 UB2LobbyInventory::GetCurrentItemLevelUpGoldCost() const
{
	int32 LevelUpCost = 0;
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	for (const FB2Item &Each : PendingSelectItemOpIngreds)
	{
		LevelUpCost += DataStore.GetItemLevelUpCost(Each.InventoryType, Each.StarGrade, Each.Level);
	}
	return LevelUpCost;
}

int32 UB2LobbyInventory::GetCurrentItemOpGoldCost() const
{ 
	// 레벨업에서는 재료 레벨별로 계산됨.
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup)
	{
		return GetCurrentItemLevelUpGoldCost();
	}

	return CurrentBaseItemOpGoldCost;
}

bool UB2LobbyInventory::CanAffordCurrentItemOp() const
{ // 금전적인 부분을 보는 거. 강화 메뉴 등에서 이게 false 인 경우 실행 버튼 클릭은 가능하지만 사용자 현질 유도 팝업을 띄울 것임.
	return (
		CurrentItemOpMode != ELobbyInvenItemOpMode::EIVIOP_None &&
		BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= GetCurrentItemOpGoldCost()
		);
}

bool UB2LobbyInventory::IsMaxItemLevelUpExpectedByCurrentSelection()
{
	FB2Item EnhanceTargetItem;
	if (GetItemOPTargetItemData(EnhanceTargetItem))
	{
		// 이건 클라이언트 계산임. 물론 최종적으로는 서버에서 강화를 해서 내려줄 것이지만 이건 클라이언트에서 할 필요가 좀 있음.
		int32 NewLevel = GetLevel_PreviewItemLevelup(EnhanceTargetItem.Exp, EnhanceTargetItem, PendingSelectItemOpIngreds, true);

		if (NewLevel >= EnhanceTargetItem.MaxLevel)
			return true;
	}
	return false;
}

int32 UB2LobbyInventory::GetOpTargetItemLevel()
{
	FB2Item EnhanceTargetItem;
	if (GetItemOPTargetItemData(EnhanceTargetItem))
	{
		// 이건 클라이언트 계산임. 물론 최종적으로는 서버에서 강화를 해서 내려줄 것이지만 이건 클라이언트에서 할 필요가 좀 있음.
		return GetLevel_PreviewItemLevelup(EnhanceTargetItem.Exp, EnhanceTargetItem, PendingSelectItemOpIngreds, true);
	}
	return 0;
}

bool UB2LobbyInventory::IsHigherGradeThanEnhanceTarget(const int32 InCheckStarGrade)
{
	FB2Item EnhanceTargetItem;
	if (GetItemOPTargetItemData(EnhanceTargetItem))
	{
		if (InCheckStarGrade > EnhanceTargetItem.StarGrade)
			return true;
	}
	return false;
}

bool UB2LobbyInventory::IsHigherGradeThanEnhanceTarget(FB2Item& InCheckItem)
{
	FB2Item EnhanceTargetItem;
	if (GetItemOPTargetItemData(EnhanceTargetItem))
	{
		if (InCheckItem.StarGrade > EnhanceTargetItem.StarGrade)
			return true;
	}
	return false;
}

bool UB2LobbyInventory::IsEnableItemLevelupIngred(const FB2Item& IngredCandidate)
{
	FB2Item EnhanceTargetItem;
	if (GetItemOPTargetItemData(EnhanceTargetItem))
	{ // 레벨업시 모루 예외처리
		if (IngredCandidate.ItemClass == EItemClass::EIC_WeaponSurpassAnvil ||
			IngredCandidate.ItemClass == EItemClass::EIC_ArmorSurpassAnvil ||
			IngredCandidate.ItemClass == EItemClass::EIC_AccessorySurpassAnvil)
		return false;
		
		return IsPossibleItemLevelupIngred(EnhanceTargetItem, IngredCandidate);
	}
	return false;
}

bool UB2LobbyInventory::IsEnableAutoSelectLevelupItem(const FB2Item& IngredCandidate)
{
	/*FB2Item EnhanceTargetItem;
	if (GetItemOPTargetItemData(EnhanceTargetItem))
	{
		auto* UIAutoLevelUp = UB2UIManager::GetInstance()->GetUI<UB2UILevelupAutoSelect>(UIFName::LevelupAutoSelect);

		if (UIAutoLevelUp && !UIAutoLevelUp->IsEnableAutoSelect(IngredCandidate))
		{
				return false;
		}

		if (IsPossibleItemLevelupIngred(EnhanceTargetItem, IngredCandidate))
		{
			if (!IngredCandidate.bIsLocked)
			{
				FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
				FPCItemContainer &ItemContainer = CliDataStore.GetLocalCharacterData().GetUserAllItems();
				return !ItemContainer.IsInItemPresets(IntToPCClass(EnhanceTargetItem.AllowedPCClass), IngredCandidate.InstanceUID);
			}
		}
	}*/
	return false;
}

bool UB2LobbyInventory::IsPossibleItemLevelupIngred(const FB2Item& EnhanceTarget, const FB2Item& IngredCandidate)
{
	return (EnhanceTarget.InstanceUID != IngredCandidate.InstanceUID &&
		EnhanceTarget.InventoryType == IngredCandidate.InventoryType /*&& !IngredCandidate.bIsLocked*/); // 잠긴 것도 재료가 되는 건 아닌데, 최소한 선택 시도는 되어야 함. (메시지 박스가 뜨도록.)
}

bool UB2LobbyInventory::SelectForIngred(int64 ItemInstanceUID, ELobbyInvenItemOpMode ItemOpMode, bool InForceMenuUpdate)
{
	//if (bFreezeitemOpModeIngredSelection || CurrentItemOpMode != ItemOpMode || ItemInstanceUID == ItemOpTargetItemID) // 현재 타겟은 ingredient 가 되어선 안되겠지
	//	return false;

	//// 장착 장비가 아닌 보관함에서만 찾는다.
	//FB2Item FoundItem;
	//if (!FindStoredItem(FoundItem, ItemInstanceUID))
	//	return false;

	//if (// 모드별 합당한 재료인지 체크
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup && !IsEnableItemLevelupIngred(FoundItem)) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Composition && !IsEnableComposeIngred(FoundItem)) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass && !IsEnableSurpassIngred(FoundItem)) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Enhancement && !IsEnableEhanceIngred(FoundItem) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_EnhancementChange && !IsEnableEhanceChangeIngred(FoundItem)))
	//	)
	//{
	//	return false;
	//}

	//// 선택된것이 있으면 Swap할것인지 추가 선택을 막을것인지 결정
	//switch (ItemOpMode)
	//{
	//	// Swap SelectItem
	//case ELobbyInvenItemOpMode::EIVIOP_Composition:
	//case ELobbyInvenItemOpMode::EIVIOP_Surpass:
	//case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
	//case ELobbyInvenItemOpMode::EIVIOP_EnhancementChange:
	//	{
	//		// 기존에 선택한 것은 지워준다.
	//		if (PendingSelectItemOpIngreds.Num() > 0)
	//			UnSelectForIngred(PendingSelectItemOpIngreds[0].InstanceUID, ItemOpMode);
	//	}
	//	break;
	//default:
	//	{
	//		if (IsIngredsFull())
	//			return false;
	//	}
	//	break;
	//}	

	//FoundItem.bSelectedAsIngred = true;
	//PendingSelectItemOpIngreds.Add(FoundItem);

	//// 데이터 갱신
	//UpdateSingleItemData(ItemInstanceUID, FoundItem); // 내부에서 UI 쪽 데이터 갱신 요청도 할 것.

	//if (InForceMenuUpdate)
	//{
	//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // 강화창은 따로 업데이트	
	//}
	return true;
}

void UB2LobbyInventory::UnSelectForIngred(int64 ItemInstanceUID, ELobbyInvenItemOpMode ItemOpMode)
{
	//if (bFreezeitemOpModeIngredSelection || CurrentItemOpMode != ItemOpMode)
	//	return;

	//FB2Item FoundItem;
	//// 장착 장비가 아닌 보관함에서만 찾는다.
	//if (FindStoredItem(FoundItem, ItemInstanceUID))
	//{
	//	FoundItem.bSelectedAsIngred = false;

	//	// 리스트에서 제거
	//	for (int32 RI = 0; RI < PendingSelectItemOpIngreds.Num(); ++RI)
	//	{
	//		if (PendingSelectItemOpIngreds[RI].InstanceUID == ItemInstanceUID)
	//		{
	//			PendingSelectItemOpIngreds.RemoveAt(RI);
	//			break;
	//		}
	//	}

	//	// 데이터 갱신
	//	UpdateSingleItemData(ItemInstanceUID, FoundItem);  // 내부에서 UI 쪽 데이터 갱신 요청도 할 것.

	//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // 강화창은 따로 업데이트
	//}
}

void UB2LobbyInventory::UnSelectForIngred(const TArray<int64>& InInstanceUID, ELobbyInvenItemOpMode ItemOpMode)
{
	if (bFreezeitemOpModeIngredSelection || CurrentItemOpMode != ItemOpMode)
		return;

	for (int32 i = 0; i < InInstanceUID.Num(); ++i)
	{
		FB2Item FoundItem;
		// 장착 장비가 아닌 보관함에서만 찾는다.
		if (FindStoredItem(FoundItem, InInstanceUID[i]))
		{
			FoundItem.bSelectedAsIngred = false;

			// 리스트에서 제거
			for (int32 RI = 0; RI < PendingSelectItemOpIngreds.Num(); ++RI)
			{
				if (PendingSelectItemOpIngreds[RI].InstanceUID == InInstanceUID[i])
				{
					PendingSelectItemOpIngreds.RemoveAt(RI);
					break;
				}
			}

			// 데이터 갱신
			UpdateSingleItemData(InInstanceUID[i], FoundItem);  // 내부에서 UI 쪽 데이터 갱신 요청도 할 것.
		}
	}
	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // 강화창은 따로 업데이트
}

void UB2LobbyInventory::AutoItemUnselect(int64 InGrade)
{

}

void UB2LobbyInventory::GetAllSelectedItemOpIngreds(TArray<int64>& OutIngredsID)
{
	for (FB2Item& CurrIngred : PendingSelectItemOpIngreds)
	{
		OutIngredsID.Add(CurrIngred.InstanceUID);
	}
}

void UB2LobbyInventory::GetAllSelectedItemOpIngreds(TArray<FB2Item>& OutIngredsData)
{
	for (FB2Item& CurrIngred : PendingSelectItemOpIngreds)
	{
		OutIngredsData.Add(CurrIngred);
	}
}

void UB2LobbyInventory::GetAvailableHammers(FB2Item& TargetItem, TArray<int64>& OutputList)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	TArray<FQualityTransRateInfo> QualityRateTable;
	CliDataStore.GetQualityTable(QualityRateTable);

	int64 tempID = 0;

	for (int32 i = 0; i < QualityRateTable.Num(); i++)
	{
		if (TargetItem.Quality >= QualityRateTable[i].transCondQuality && CliDataStore.GetMaxQuality(QualityRateTable[i].templateID) != TargetItem.Quality )
		{
			if (tempID != QualityRateTable[i].templateID)
			{
				OutputList.Add(QualityRateTable[i].templateID);
				tempID = QualityRateTable[i].templateID;
			}
		}
	}
}

void UB2LobbyInventory::ResetAllIngredsList()
{
	//PendingSelectItemOpIngreds.Empty(); // 리스트는 비우고.

	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{ // 이게 좀 느려진다 싶으면 PendingSelectItemOpIngreds 비우기 전에 해당 ID 만 찾아서 리셋하는 걸 해볼 수도.. 그게 더 느리려나 ㄷㄷ
	//	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(IntToPCClass(PCI));
	//	if (!CurrPCItemAccessor)
	//		continue;

	//	TMap<int64, FB2Item>* WeaponMap = CurrPCItemAccessor->GetItemMapForInvenType(EItemInvenType::EIIVT_Weapon);
	//	TMap<int64, FB2Item>* ProtectionMap = CurrPCItemAccessor->GetItemMapForInvenType(EItemInvenType::EIIVT_Protection);
	//	TMap<int64, FB2Item>* AccessoryMap = CurrPCItemAccessor->GetItemMapForInvenType(EItemInvenType::EIIVT_Accessory);

	//	if (WeaponMap)
	//	{
	//		for (auto& CurrWeapon : *WeaponMap)
	//		{
	//			CurrWeapon.Value.bSelectedAsIngred = false;
	//		}
	//	}
	//	if (ProtectionMap)
	//	{
	//		for (auto& CurrProtection : *ProtectionMap)
	//		{
	//			CurrProtection.Value.bSelectedAsIngred = false;
	//		}
	//	}
	//	if (AccessoryMap)
	//	{
	//		for (auto& CurrAccessory : *AccessoryMap)
	//		{
	//			CurrAccessory.Value.bSelectedAsIngred = false;
	//		}
	//	}
	//}
	//for (auto& CurrConsumables : GetConsumablesList())
	//{
	//	CurrConsumables.Value.bSelectedAsIngred = false;
	//}

	//if (TransientLobbyGM) // 이걸 검사하는 이유는 BeginDestroy 시점에 불리 때에 UI 단 업데이트를 막기 위해서임. 일반적인 경로는 아니지만 여튼 위험함.
	//{ // 관련 UI Doc 데이터 업데이트.
	//	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // 레벨업창도 열려 있다면..
	//}
}

void UB2LobbyInventory::ResetEnhanceItemDatas()
{
	CurrentEnhanceItemSuccessRate = 0;
	CurrentEnhanceItemScrollCost = 0;
	CurrentEnhanceItemProtectionScrollCost = 0;
	DeltaEnhanceLevelForResultPresent = 0;
}


bool UB2LobbyInventory::IsSelectableEnhanceProtectionItem()
{
	return (GetEnhanceProtectionItemCount() != 0 && GetEnhanceProtectionItemCount() >= CurrentEnhanceItemProtectionScrollCost);
}

bool UB2LobbyInventory::IsSelectedEnhanceProtectionItem()
{
	return (PendingEnhanceProtectionItemRefID != 0);
}

void UB2LobbyInventory::SetSelectEnhanceProtectionItemRefId(int32 ProtectionItemRefID)
{
	PendingEnhanceProtectionItemRefID = ProtectionItemRefID;
}

bool UB2LobbyInventory::GetSelectEnhanceProtectionItem(FB2Item& OutData)
{
	if (IsSelectedEnhanceProtectionItem() == false)
		return false;

	TArray<FB2Item> AllFound;
	GetSharedConsumableOfType(AllFound, PendingEnhanceProtectionItemRefID);

	if (AllFound.Num() == 0)
		return false;
	
	OutData = AllFound[0];
	return true;
}

int32 UB2LobbyInventory::GetEnhanceProtectionItemCount()
{
	return GetSharedConsumableAmountOfType(FItemRefIDHelper::GetEnhanceProtectionItemRefId()); // 보호권 타입 추가에 따라 추후 다시..
}

void UB2LobbyInventory::GetAllGhostIngreds(TArray<FB2Item>& OutGhostData)
{
	for (FB2Item& CurrIngred : GhostItemOpIngreds)
	{
		OutGhostData.Add(CurrIngred);
	}
}

void UB2LobbyInventory::GetAllGhostIngreds(TArray<int64>& OutGhostData)
{
	for (FB2Item& CurrIngred : GhostItemOpIngreds)
	{
		OutGhostData.Add(CurrIngred.InstanceUID);
	}
}

int32 UB2LobbyInventory::HowManyUpgradeStoneForCurrentTarget() const
{
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Upgrade)
	{
		FB2Item UpgradeTargetItem;
		if (GetItemOPTargetItemData(UpgradeTargetItem, false))
		{
			int32 UpgradeStoneRefID = FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(UpgradeTargetItem);
			return GetSharedConsumableAmountOfType(UpgradeStoneRefID);
		}
	}
	return 0;
}

bool UB2LobbyInventory::HasEnoughUpgradeStoneForCurrentTarget() const
{
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Upgrade)
	{
		return (HowManyUpgradeStoneForCurrentTarget() >= GetCurrentUpgradeStoneCost());
	}
	return false;
}

bool UB2LobbyInventory::IsEnableComposeIngred(const FB2Item& Ingred)
{
	FB2Item ComposeTargetItem;
	if (GetItemOPTargetItemData(ComposeTargetItem))
	{
		return (ComposeTargetItem.InstanceUID != Ingred.InstanceUID && 
			ComposeTargetItem.InventoryType == Ingred.InventoryType &&
			ComposeTargetItem.StarGrade == Ingred.StarGrade && 
			// 레벨이 같거나 // 초월이거나(현재6성)
			(ComposeTargetItem.Level == Ingred.Level || IsItemAllowedCompositeAndSurpass(Ingred))
			);
	}
	return false;
}

bool UB2LobbyInventory::IsEnableSurpassIngred(const FB2Item& Ingred)
{
	FB2Item SurpassTargetItem;

	if (GetItemOPTargetItemData(SurpassTargetItem))
	{ 
		//황금모루 예외처리
		if (Ingred.ItemClass == EItemClass::EIC_WeaponSurpassAnvil ||
			Ingred.ItemClass == EItemClass::EIC_ArmorSurpassAnvil ||
			Ingred.ItemClass == EItemClass::EIC_AccessorySurpassAnvil
			)
		{
			return (
				(SurpassTargetItem.InstanceUID != Ingred.InstanceUID) &&
				(FItemRefIDHelper::ExtractItemVariationFromRefID(SurpassTargetItem.ItemRefID) == (FItemRefIDHelper::ExtractItemVariationFromRefID(Ingred.ItemRefID)))
				);
		}
		
		// 11성 이상의 장비는 11성 이상으로 만 초월하고
		// 이하는 이전의 6성 부터 필터링 할 수 있도록 제한 추가
		uint16 Minimum_Surpass_Star_Grade = (SurpassTargetItem.StarGrade >= FItemGradeInfo::MINIMUM_ADVANCED_SURPASS_STAR_GRADE ?
			FItemGradeInfo::MINIMUM_ADVANCED_SURPASS_STAR_GRADE : FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE);		

		return (SurpassTargetItem.InstanceUID != Ingred.InstanceUID &&
			Ingred.StarGrade >= Minimum_Surpass_Star_Grade &&			// 변수 추가하여 교체
			SurpassTargetItem.ItemClass == Ingred.ItemClass && // Item Slot으로 추정. 큰 분류만이 아닌 구체 클래스까지 일치함.
			(FItemRefIDHelper::ExtractItemVariationFromRefID(SurpassTargetItem.ItemRefID) == FItemRefIDHelper::ExtractItemVariationFromRefID(Ingred.ItemRefID)) // 마지막으로 동일 계열 variation 이어야.
			);
		
	}
	return false;
}

bool UB2LobbyInventory::IsVisibilityEhanceIngred(const FB2Item& Ingred)
{
	FB2Item EnhanceTargetItem;
	if (GetItemOPTargetItemData(EnhanceTargetItem))
	{
		if (EnhanceTargetItem.InstanceUID == Ingred.InstanceUID)
			return false;

		if (Ingred.InventoryType != EItemInvenType::EIIVT_Consumables)
			return false;

		if (Ingred.ItemClass != EItemClass::EIC_EnhaceScroll &&
			Ingred.ItemClass != EItemClass::EIC_BlessedEnhanceScroll &&
			Ingred.ItemClass != EItemClass::EIC_ProtectionScroll)
			return false;

		// todo : 추후에 스크롤이 여러개가 되면 타입으로 빼도록 하자! 
		int32 ScrollRefId = FItemRefIDHelper::GetEnhanceScrollRefId(EnhanceTargetItem);
		int32 BlessScrollRefId = FItemRefIDHelper::GetBlessEnhanceScrollRefId(EnhanceTargetItem);

		return (ScrollRefId == Ingred.ItemRefID || BlessScrollRefId == Ingred.ItemRefID || Ingred.ItemClass == EItemClass::EIC_ProtectionScroll);
	}

	return false;
}

bool UB2LobbyInventory::IsEnableEhanceIngred(const FB2Item& Ingred)
{
	FB2Item EnhanceTargetItem;
	if (GetItemOPTargetItemData(EnhanceTargetItem))
	{
		if (EnhanceTargetItem.InstanceUID == Ingred.InstanceUID)
			return false;

		if (Ingred.InventoryType != EItemInvenType::EIIVT_Consumables)
			return false;
			
		if (Ingred.ItemClass != EItemClass::EIC_EnhaceScroll && 
			Ingred.ItemClass != EItemClass::EIC_BlessedEnhanceScroll)
			return false;

		// todo : 추후에 스크롤이 여러개가 되면 타입으로 빼도록 하자! 
		int32 ScrollRefId = FItemRefIDHelper::GetEnhanceScrollRefId(EnhanceTargetItem);
		int32 BlessScrollRefId = FItemRefIDHelper::GetBlessEnhanceScrollRefId(EnhanceTargetItem);

		return (ScrollRefId == Ingred.ItemRefID || BlessScrollRefId == Ingred.ItemRefID);
	}

	return false;
}

bool UB2LobbyInventory::IsEnableEhanceChangeIngred(const FB2Item& Ingred)
{
	FB2Item EnhanceTargetItem;
	if (GetItemOPTargetItemData(EnhanceTargetItem))
	{
		if (EnhanceTargetItem.InventoryType == Ingred.InventoryType && IsEquipmentItemByType(Ingred))
		{
			return true;
		}
	}

	return false;
}

void UB2LobbyInventory::CheckUsableEnhanceProtectionItem()
{
	if (PendingEnhanceProtectionItemRefID == 0)
		return;

	if (CurrentEnhanceItemProtectionScrollCost == 0)
		PendingEnhanceProtectionItemRefID = 0;

	if (GetSharedConsumableAmountOfType(PendingEnhanceProtectionItemRefID) < CurrentEnhanceItemProtectionScrollCost)
		PendingEnhanceProtectionItemRefID = 0;

	FB2Item TargetItemData;
	if (FindFromAll(TargetItemData, ItemOpTargetItemID))
	{
		if (IsItemSafeEnhanceLevel(TargetItemData))
			PendingEnhanceProtectionItemRefID = 0;
	}
}

void UB2LobbyInventory::LevelUpItem(const FB2LevelupItem& EnhancedItemInfo)
{
	BladeIIGameImpl::GetClientDataStore().LevelupItem(EnhancedItemInfo);

	//bool bJackPot = GET_TUPLE_DATA(FB2ResponseLevelupItem::jackpoted_index, EnhancedItemInfo);
	//bool bGreateJackPot = GET_TUPLE_DATA(FB2ResponseLevelupItem::great_jackpoted_index, EnhancedItemInfo);

	//if (bGreateJackPot)
	//	CachedItemLevelupJackpotType = EItemLevelUpJackpotType::ER_GreateJackpot;
	//else if (bJackPot)
	//	CachedItemLevelupJackpotType = EItemLevelUpJackpotType::ER_Jackpot;
	//else 
	//	CachedItemLevelupJackpotType = EItemLevelUpJackpotType::ER_Normal;

	//if (AutoItemLevelupPhase != EAutoItemLevelupPhase::AEPhase_None)
	//{
	//	OnResponseOfSingleAutoItemLevelupList(EnhancedItemInfo);
	//}
	//else
	//{
	//	OnItemOpResponseCommon(ELobbyInvenItemOpMode::EIVIOP_Levelup);
	//}

	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyInventory::SurpassedItem(const FB2SurpassItem& SurpassedItemInfo)
{
	BladeIIGameImpl::GetClientDataStore().SurpassItem(SurpassedItemInfo);

	OnItemOpResponseCommon(ELobbyInvenItemOpMode::EIVIOP_Surpass);
}

void UB2LobbyInventory::UpgradedItem(const FB2UpgradeItem& UpgradedItemInfo)
{
	BladeIIGameImpl::GetClientDataStore().UpgradeItem(UpgradedItemInfo);

	OnItemOpResponseCommon(ELobbyInvenItemOpMode::EIVIOP_Upgrade);
}

void UB2LobbyInventory::ComposedItem(const FB2ComposeItem& ComposedItemInfo)
{
	//합성된 아이템을 없애기 전에 어떤 프리셋에 장착되어 있는지 체크한다
	//FB2Item NewItem;
	//NewItem = GET_TUPLE_DATA(FB2ResponseComposeItem::new_composed_item_index, ComposedItemInfo);
	//int64 MainItemInstanceUID = GET_TUPLE_DATA(FB2ResponseComposeItem::deleted_main_item_id_index, ComposedItemInfo);

	//FPCItemContainer &ItemContainer = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems();
	//int32 PrevPresetID = -1;
	//FB2Item* PrevMainItem = ItemContainer.FindItem(MainItemInstanceUID);
	//if(PrevMainItem)
	//	PrevPresetID = ItemContainer.GetContainsItemPresetID(IntToPCClass(PrevMainItem->AllowedPCClass), PrevMainItem->InstanceUID);

	//ItemOpTargetItemID = BladeIIGameImpl::GetClientDataStore().ComposeItem(ComposedItemInfo); // 합성은 아이템이 아싸리 바뀐다. 모드 종료 전까지는 TargetItemID 가 필요하니 새로 바뀐 걸로 갱신.

	////장착한 셋트가 있었다면 서버에서는 이미 장착된 상태이므로 클라만 업데이트 해준다.
	//if (PrevPresetID >= 0)
	//{
	//	TArray<int64> EquippedItem;
	//	EquippedItem.Add(NewItem.InstanceUID);
	//	EquippedItems(PrevPresetID, EquippedItem, TArray<int64>(), false);
	//}
	//
	//OnItemOpResponseCommon(ELobbyInvenItemOpMode::EIVIOP_Composition);
}

void UB2LobbyInventory::EnhanceItem(const FB2EnhanceItem& EnhancedItemInfo)
{	
	DeltaEnhanceLevelForResultPresent = BladeIIGameImpl::GetClientDataStore().EnhanceItem(EnhancedItemInfo);	

	OnItemOpResponseCommon(ELobbyInvenItemOpMode::EIVIOP_Enhancement);
}

void UB2LobbyInventory::CostumeEnhanceItem(const FB2ResponseEnhanceCostumePtr& msg)
{
	if (msg->current_money < BladeIIGameImpl::GetClientDataStore().GetGoldAmount())
		BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, msg->current_money);


	int32 MaterialID = SvrToCliItemInvenType(msg->enhanced_costume->inventory_type) == EItemInvenType::EIIVT_CostumeArmor ? FItemRefIDHelper::ITEM_REF_ID_ENHANCE_COSTUMEARMOR_ITEM : FItemRefIDHelper::ITEM_REF_ID_ENHANCE_COSTUMEWEAPON_ITEM;
	BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountDecrease(MaterialID, -1, msg->changed_material_items, msg->deleted_material_item_ids);
	BladeIIGameImpl::GetClientDataStore().ChangeCostumeItem(msg->enhanced_costume);

	OnItemOpResponseCommon(ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume);
}

void UB2LobbyInventory::ResponseRefineTotem(const FB2RefineTotemPtr& RefineTotemInfo)
{
	//FB2Totem RefineResultTotem(RefineTotemInfo->refined_totem);
	//BladeIIGameImpl::GetClientDataStore().SmeltingTotem(RefineTotemInfo, CachedTotemRefineCostType);
	//UpdateTotemSmeltingResultClass<bool, FB2Totem>::GetInstance().Signal(RefineTotemInfo->is_success, RefineResultTotem);
}

void UB2LobbyInventory::ResponseAddTotemToInventory(const FB2AddTotemToInventoryPtr& AddTotemToInventoryInfo)
{
	//if (AddTotemToInventoryInfo && AddTotemToInventoryInfo->totems.IsValidIndex(0))
	//{
	//	const int32 TotemInfoArrayMax = AddTotemToInventoryInfo->totems.Num();
	//	for (int32 i = 0; i < TotemInfoArrayMax; ++i)
	//	{
	//		if (AddTotemToInventoryInfo->totems.IsValidIndex(i) && AddTotemToInventoryInfo->totems[i])
	//		{
	//			BladeIIGameImpl::GetClientDataStore().AddTotem(AddTotemToInventoryInfo->totems[i]);
	//		}
	//	}

	//	BladeIIGameImpl::GetRedDotManager().RequestCheckRedDot({ RedDotHint::RED_DOT_EQUIP });
	//	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
	//}
}

void UB2LobbyInventory::OnItemOpResponseCommon(ELobbyInvenItemOpMode InOpMode)
{
	//// 서버쪽에 강화/합성 등 Req 보내고 Response 온 상황.

	//// 인벤토리 UI 페이지에서 잠시 빠지는데 연출 끝나고 돌아올 때를 위해 탭 선택을 백업해 둔다.
	//OnCommitItemOpHeroMgmtPCBackup = TransientLobbyGM ? TransientLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	//UB2UIManager_Lobby* LobbyUIManager = TransientLobbyGM ? TransientLobbyGM->DJLegacy_GetLobbyUIManager() : NULL;
	//OnCommitItemOpInvenPageTabBackup = LobbyUIManager ? LobbyUIManager->GetInventoryTabSelection() : EItemInvenType::EIIVT_End;

	//ResetAllIngredsList();

	//// 모드별로 각자의 메인 창을 닫고 연출을 시작한다. 연출 창부터는 pop-up 이 아닌 독자 페이지로. (따라서 연출에서 돌아왔을 때를 위해 위에서 PC 선택이랑 탭등을 백업해 놓는거)
	//switch (InOpMode)
	//{
	//case ELobbyInvenItemOpMode::EIVIOP_Levelup:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemLevelUpMain);
	//	// ChangeLobbyUIPage 전에 View 부터 먼저 셋업해야 한다. 연출 UI Widget 안에서 카메라에 접근하는 게 있음.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Levelup);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemLevelUpProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Upgrade:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemUpgradeMain);
	//	// ChangeLobbyUIPage 전에 View 부터 먼저 셋업해야 한다. 연출 UI Widget 안에서 카메라에 접근하는 게 있음.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Upgrade);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemUpgradeProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Composition:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemComposeMain);
	//	// ChangeLobbyUIPage 전에 View 부터 먼저 셋업해야 한다. 연출 UI Widget 안에서 카메라에 접근하는 게 있음.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Composition);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemComposeProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Surpass:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemSurpassMain);
	//	// ChangeLobbyUIPage 전에 View 부터 먼저 셋업해야 한다. 연출 UI Widget 안에서 카메라에 접근하는 게 있음.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Surpass);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemSurpassProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceMain);
	//	// ChangeLobbyUIPage 전에 View 부터 먼저 셋업해야 한다. 연출 UI Widget 안에서 카메라에 접근하는 게 있음.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Enhancement);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemEnhanceProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_TotemSmelting:
	//	// ChangeLobbyUIPage 전에 View 부터 먼저 셋업해야 한다. 연출 UI Widget 안에서 카메라에 접근하는 게 있음.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_TotemSmelting);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_TotemSmeltingProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume:
	//	// ChangeLobbyUIPage 전에 View 부터 먼저 셋업해야 한다. 연출 UI Widget 안에서 카메라에 접근하는 게 있음.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemLevelUpProgress);
	//	break;
	//}

	//LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(OnCommitItemOpHeroMgmtPCBackup); // 아이템 외관이 바뀔 수 있으므로 장비 장착 상태도 업데이트를..
	//UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(OnCommitItemOpHeroMgmtPCBackup, false);
}

void UB2LobbyInventory::ItemUpgradeCostDelivered(const FB2GetItemUpgradeCost& UpgradeCost)
{
//	CurrentBaseItemOpGoldCost = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::need_gold_index, UpgradeCost);
//	CurrentUpgradeStoneCost = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::need_upgrade_stone_count_index, UpgradeCost);
//	// 요건 정해놓고 사용하니 체크만..
//	int32 UpgradeStoneID = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::need_upgrade_stone_template_id_index, UpgradeCost);
//	b2network::B2ItemServerInfoPtr ServerNextUpgradeItem = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::next_item_index, UpgradeCost);
//	NextUpgradeItem = ServerNextUpgradeItem;
//
//#if !UE_BUILD_SHIPPING
//	FB2Item UpgradeTargetItem;
//	GetItemOPTargetItemData(UpgradeTargetItem, false);
//	// 제대로 안 오는 모양인데..
//	//checkSlow(UpgradeStoneID == FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(UpgradeTargetItem));
//#endif
//	EnterItemUpgradeModeClass<int64>::GetInstance().Signal(BeforeUpgradeItem.InstanceUID);
//	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI 도 업데이트
}

void UB2LobbyInventory::ItemComposeCostDelivered(const FB2GetItemComposeCost& ComposeCost)
{
	//CurrentBaseItemOpGoldCost = GET_TUPLE_DATA(FB2ResponseItemComposeCost::need_gold_index, ComposeCost);

	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI 도 업데이트
}

void UB2LobbyInventory::ItemSurpassCostDelivered(const FB2GetItemSurpassCost& SurpassCost)
{
	//CurrentBaseItemOpGoldCost = GET_TUPLE_DATA(FB2ResponseItemSurpassCost::need_gold_index, SurpassCost);

	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI 도 업데이트
}

void UB2LobbyInventory::SetCurEnhanceItemData(const int64 TargetItemID)
{
	//FB2Item OpTarget;
	//if (!GetItemOPTargetItemData(OpTarget))
	//	return;

	//CurrentBaseItemOpGoldCost = BladeIIGameImpl::GetClientDataStore().GetEnhanceItem_NextStepMoneyCost(OpTarget.EnhanceLevel);
	//CurrentEnhanceItemSuccessRate = BladeIIGameImpl::GetClientDataStore().GetEnhanceItem_NextStepRate(OpTarget.EnhanceLevel);
	//CurrentEnhanceItemScrollCost = BladeIIGameImpl::GetClientDataStore().GetEnhanceItem_NextStepScrollCost(OpTarget.EnhanceLevel);
	//CurrentEnhanceItemProtectionScrollCost = BladeIIGameImpl::GetClientDataStore().GetEnhanceItem_NextStepProtectionScrollCost(OpTarget.EnhanceLevel);

	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI 도 업데이트
}

void UB2LobbyInventory::BeginAutoSelectGradeItems(const int32 StarGrade)
{
	CachedStarGrade = StarGrade;
	//if (IsHigherGradeThanEnhanceTarget(CachedStarGrade))
	//{
	//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
	//		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_HigherGradeThanEnhanceTarget"))),
	//		0.f,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::YesOrNo,
	//		FMsgPopupOnClick::CreateLambda([this]() {
	//		AutoSelectGradeItems(CachedStarGrade);
	//	})
	//	);
	//}
	//else
	//{
		AutoSelectGradeItems(CachedStarGrade);
	//}
	
}

void UB2LobbyInventory::AutoSelectGradeItems(const int32 StarGrade)
{
	//FB2Item EnhanceTargetItem;
	//if (GetItemOPTargetItemData(EnhanceTargetItem) == false)
	//	return;

	//EPCClass PCClass = IntToPCClass(EnhanceTargetItem.AllowedPCClass);
	//EItemInvenType InvenType = EnhanceTargetItem.InventoryType;

	//TArray<FB2Item> OutFilteredList;
	//OutFilteredList.Empty();

	//GetStoredItemList_ItemOpModeFilter(OutFilteredList, InvenType, PCClass, true, StarGrade);

	//for (int32 i = OutFilteredList.Num() -1 ; i > -1; --i)
	//{
	//	const FB2Item &Each = OutFilteredList[i];
	//	if (Each.bSelectedAsIngred)
	//		continue;
	//	if(Each.StarGrade != StarGrade)
	//		continue;

	//	if(IsEnableAutoSelectLevelupItem(Each) == false)
	//		continue;

	//	if (IsMaxItemLevelUpExpectedByCurrentSelection())
	//		break;

	//	if (SelectForIngred(Each.InstanceUID, ELobbyInvenItemOpMode::EIVIOP_Levelup, false) == false)
	//		continue;
	//}
	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // 강화창은 따로 업데이트	
}

void UB2LobbyInventory::AutoSelectGradeItemsForConditions(const int32 StarGrade)
{
	FB2Item EnhanceTargetItem;
	if (GetItemOPTargetItemData(EnhanceTargetItem) == false)
		return;

	EPCClass PCClass = IntToPCClass(EnhanceTargetItem.AllowedPCClass);
	EItemInvenType InvenType = EnhanceTargetItem.InventoryType;

	TArray<FB2Item> OutFilteredList;
	OutFilteredList.Empty();

	GetStoredItemList_ItemOpModeFilter(OutFilteredList, InvenType, PCClass, true);

	for (int32 i = OutFilteredList.Num() - 1; i > -1; --i)
	{
		const FB2Item &Each = OutFilteredList[i];

		if (Each.StarGrade != StarGrade)
			continue;

		if (IsEnableAutoSelectLevelupItem(Each) == false)
			continue;

		if (IsMaxItemLevelUpExpectedByCurrentSelection())
			break;

		if (SelectForIngred(Each.InstanceUID, ELobbyInvenItemOpMode::EIVIOP_Levelup) == false)
			continue;
	}
}

void UB2LobbyInventory::BeginAutoLevelupItem(const TArray<FB2Item>& AllBulkList, bool bCostRequestOnly)
{
	BII_CHECK(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None);
	if (CurrentItemOpMode != ELobbyInvenItemOpMode::EIVIOP_None){
		return;
	}

	AutoItemLevelupPhase = EAutoItemLevelupPhase::AEPhase_SortingList;
	CachedAutoItemLevelupResultItems.Empty();
	SortedAutoItemLevelupList.Empty();
	SortAutoLevelupItemList(AllBulkList, SortedAutoItemLevelupList);

	if (SortedAutoItemLevelupList.Num() == 0){
		EndAutoItemLevelup(bCostRequestOnly, false);
		return;
	}

	// SortedAutoItemLevelupList 의 조건에 따라 자동레벨업 진행..
	BeginReqAutoItemLevelupCostWithSortedList(SortedAutoItemLevelupList);
}

void UB2LobbyInventory::SortAutoLevelupItemList(const TArray<FB2Item>& AllBulkList, TArray<FAutoItemLevelupSingleSet>& OutSortedList)
{
	// 몇몇 필터를 가지고 마구 모아놓은 리스트에서 어떤 걸 타겟으로 하고 어떤 걸 재료로 할지 등을 정해서 OutSortedList 로 넘긴다.

	// 일단 서로 타겟/재료가 될 수 있는 동일한 종류끼리 묶는다. 여기에 추가로 동일 등급까지 적용
	TArray<FAutoItemLevelupPossibleSet> AllPossibleSet;
	
	for (int32 BLI = 0; BLI < AllBulkList.Num(); ++BLI)
	{
		const FB2Item& ThisBulkOne = AllBulkList[BLI];
		bool bFoundExistingSet = false;

		BII_CHECK(!ThisBulkOne.bIsLocked); // 잠겨 있다면 진작에 여기 리스트로 들어와선 안됨.
		if (ThisBulkOne.bIsLocked){
			continue;
		}

		if (!IsEssenceItem(ThisBulkOne) && !IsItemLevelupAllowed(ThisBulkOne)){ // 최대 강화한도에 도달했다거나 해서 강화 허용 안되는 애들은 UI 에서 필터링 안되어 있을 수 있으니 여기서 거른다.
			continue;
		}

		for (int32 API = 0; API < AllPossibleSet.Num(); ++API)
		{
			FAutoItemLevelupPossibleSet& ThisPossibleSet = AllPossibleSet[API];
			if (ThisPossibleSet.ItemsList.Num() > 0)
			{ // 현재 PossibleSet 에서 첫번째 요소만으로 같은 부류로 속할 수 있는 것인지 검사한다.
				if (
					// 하나를 target 으로 상정해서 재료가 될 수 있다면 서로 같이 다른 타겟의 재료도 될 수 있는 거
					IsPossibleItemLevelupIngred(ThisPossibleSet.ItemsList[0], ThisBulkOne) &&
					// 추가로 동일 등급끼리 묶기.
					(ThisPossibleSet.ItemsList[0].StarGrade == ThisBulkOne.StarGrade) &&
					// 허용 PC Class 는 이걸 사용할 UI 상 어지간해선 여기로 들어오는 애들은 같아야 하는데.. 혹 모르니 검사해서 나쁠 거 없지.
					(ThisPossibleSet.ItemsList[0].AllowedPCClass == ThisBulkOne.AllowedPCClass)
					) 
				{
					if(!IsEssenceItem(ThisBulkOne))
						ThisPossibleSet.ItemsList.Add(ThisBulkOne); 
					else
						ThisPossibleSet.EssenceItemList.Add(ThisBulkOne);
					bFoundExistingSet = true;
					break;
				}
			}
		}

		if (!bFoundExistingSet)
		{ // 기존에서 못 찾았다면 본인의 문파 창설
			FAutoItemLevelupPossibleSet NewSet;
			if (!IsEssenceItem(ThisBulkOne))
				NewSet.ItemsList.Add(ThisBulkOne);
			else
				NewSet.EssenceItemList.Add(ThisBulkOne);
			AllPossibleSet.Add(NewSet);
		}
	}

	// 각 PossibleSet 내에서 레벨 단위로 소팅. 높은 레벨을 우선적으로 강화할 거라.
	struct FAutoEnhanceListPrioritySorter
	{
		bool operator()(const FB2Item& A, const FB2Item& B) const
		{
			return A.Level > B.Level;
		}
	};

	for (int32 API = 0; API < AllPossibleSet.Num(); ++API)
	{
		AllPossibleSet[API].ItemsList.Sort(FAutoEnhanceListPrioritySorter());
	}

	const int MAXINGREDIENTCOUNT = 10;
	
	// 마지막으로 30레벨 및 기타 한도에 걸리는 단위로 잘라서 최종 리스트를 빼낸다.
	for (int32 API = 0; API < AllPossibleSet.Num(); ++API)
	{
		TArray<FB2Item> ThisPossibleSetList = AllPossibleSet[API].ItemsList;
		TArray<FB2Item>& ThisEssenceItemList = AllPossibleSet[API].EssenceItemList;

		if(ThisPossibleSetList.Num() == 0)
			continue;
		else if (ThisPossibleSetList.Num() == 1 && ThisEssenceItemList.Num() == 0)
			continue;

		//List 합
		ThisPossibleSetList.Append(ThisEssenceItemList);

		// 앞에걸 (높은 레벨) 우선적으로 타겟으로 삼고 뒤에걸 (낮은 레벨) 우선적으로 재료로 삼는다.
		int32 IdxF = 0; // from Front
		int32 IdxR = ThisPossibleSetList.Num() - 1; // from Rear
		for (; IdxF < IdxR; ++IdxF)
		{
			if(IsEssenceItem(ThisPossibleSetList[IdxF]))
				break;

			FAutoItemLevelupSingleSet NewFinalSet;
			FB2Item EnhancePreviewCheckTarget = ThisPossibleSetList[IdxF]; // 타겟은 앞에서부터
			NewFinalSet.TargetItemID = ThisPossibleSetList[IdxF].InstanceUID;
			
			TArray<FB2Item> EnhancePreviewCheckIngreds;

			for (; IdxR > IdxF; --IdxR)
			{
				FB2Item ThisPreviewCheckIngred = ThisPossibleSetList[IdxR]; // 재료는 뒤에서부터
				EnhancePreviewCheckIngreds.Add(ThisPreviewCheckIngred);
				NewFinalSet.IngredientsID.Add(ThisPossibleSetList[IdxR].InstanceUID);

				// 재료개수 제한
				if (NewFinalSet.IngredientsID.Num() >= MAXINGREDIENTCOUNT && NewFinalSet.TargetItemID > 0 && NewFinalSet.IngredientsID.Num() > 0)
				{
					OutSortedList.Add(NewFinalSet);
					NewFinalSet.IngredientsID.Empty();
				}

				// 현재 NewFinalSet 에서 최고 기대 레벨에 예측치에 도달하면 IdxR 하나 감소시키고 여기선 그만. 다음 IdxF 로 넘어가도록.
				// 자동 강화 리스트에선 재료 개수 제한을 두지 않는다. ReqForSingleAutoItemLevelupList 에서 서버 쪽으로 Req 날릴 때 인자가 다르게 들어감.
				// 자동 강화에서 재료 개수 제한이 들어가면 최대 강화 레벨을 맞추기가 매우 어려워짐.
				float ExpectedPostExp = 0.0f;
				int32 PreviewLevel = GetLevel_PreviewItemLevelup(ExpectedPostExp, EnhancePreviewCheckTarget, EnhancePreviewCheckIngreds, false);
				if (PreviewLevel >= EnhancePreviewCheckTarget.MaxLevel)
				{
					--IdxR;
					break;
				}
			}

			// 최종 리스트에 포함시키기 전에 마지막 검사라도..
			// 재료 개수가 10개일 경우 예외 처리
			if (NewFinalSet.TargetItemID > 0 && NewFinalSet.IngredientsID.Num() > 0)
			{
				OutSortedList.Add(NewFinalSet);
			}
		}
	}
}

void UB2LobbyInventory::BeginReqAutoItemLevelupCostWithSortedList(const TArray<FAutoItemLevelupSingleSet>& InSortedList)
{
	CopiedAutoItemLevelupCostReqList.Empty();
	CopiedAutoItemLevelupCostReqList.Append(InSortedList);

	if (CopiedAutoItemLevelupCostReqList.Num() > 0) // CopiedAutoItemLevelupCostReqList 를 가지고 하는 거라 이게 없으면 안됨.
	{
		OnCheckAllAutoItemLevelupCost();
	}
}

void UB2LobbyInventory::OnCheckAllAutoItemLevelupCost()
{
	//if (BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= AutoItemLevelupTotalCost)
	//{
	//	BeginReqAutoItemLevelupWithSortedList(SortedAutoItemLevelupList);
	//}
	//else
	//{ // 현 요구사항에 따라 비용이 모자라더라도 실행 버튼 클릭은 할 수 있으므로 여기로 올 수 있음. 단, 안내 메시지를 띄운다.
	//	ShortageMGR->PopupGoToShopForGoldMessage();
	//	// 비용을 취합해 봤더니 모자라서 중단이지만 UI 는 닫지 않도록.
	//	EndAutoItemLevelup(true, false);
	//}
}

void UB2LobbyInventory::BeginReqAutoItemLevelupWithSortedList(const TArray<FAutoItemLevelupSingleSet>& InSortedList)
{
	CopiedAutoItemLevelupMainReqList.Empty();
	CopiedAutoItemLevelupMainReqList.Append(InSortedList);
	if (CopiedAutoItemLevelupMainReqList.Num() > 0) // CopiedAutoItemLevelupMainReqList 를 가지고 하는 거라 이게 없으면 안됨.
	{
		AutoItemLevelupPhase = EAutoItemLevelupPhase::AEPhase_RequestingEnhancement;
		ReqForSingleAutoItemLevelupList(); // 본격 자동 강화 요청 킥오프. 다 끝나면 알아서 EndAutoItemLevelup 가 불릴 것. Cost 요청이 있다면 여기서 시작을 하지 않게 될 것.
	}
}

void UB2LobbyInventory::ReqForSingleAutoItemLevelupList()
{
	BII_CHECK(AutoItemLevelupPhase == EAutoItemLevelupPhase::AEPhase_RequestingEnhancement);
	BII_CHECK(CopiedAutoItemLevelupMainReqList.Num() > 0);

	// 현재 구현으로는 요청에 대해 응답이 온 후에야 다음 요청을 보낼 수 있으므로 CopiedAutoItemLevelupMainReqList 에서 하나씩 요청을 진행한다.

	for (int32 SI = CopiedAutoItemLevelupMainReqList.Num() - 1; SI >= 0; --SI) // 뒷번호부터 해서.
	{
		// 여기서 for 문을 도는 건 최종 검사에서 적절하지 않은 리스트라도 있을 경우 스킵하기 위해서임. 요청 자체는 한번에 하나만 날린다.
		FAutoItemLevelupSingleSet& ThisSet = CopiedAutoItemLevelupMainReqList[SI];
		if (ThisSet.TargetItemID <= 0 || ThisSet.IngredientsID.Num() == 0){
			CopiedAutoItemLevelupMainReqList.Pop();
			continue;
		}

		data_trader::Retailer::GetInstance().RequestLevelupItem(ThisSet.TargetItemID, ThisSet.IngredientsID, true); // 자동강화에서의 강화 요청은 일반 강화와 마찬가지 프로토콜이지만 인자를 다르게 줌.
		break;
	}

	if (CopiedAutoItemLevelupMainReqList.Num() == 0) // 정상적인 경우는 아니지만 대비할 필요는 있지.
	{
		EndAutoItemLevelup(false, false);
	}
	else
	{
		// 이 경우에도 반드시 Response 가 온다는 보장은 없으니, 최후의 타이머라도 설치해 놓는게 좋을 듯?
		if (TransientLobbyGM)
		{
			TransientLobbyGM->GetWorldTimerManager().SetTimer(AutoItemLevelupResponseWaitTH, this, &UB2LobbyInventory::ForceEndAutoItemLevelupTimerCB, 7.0f, false);
		}
	}
}

void UB2LobbyInventory::OnResponseOfSingleAutoItemLevelupList(const FB2LevelupItem& EnhancedItemInfo)
{
	BII_CHECK(AutoItemLevelupPhase == EAutoItemLevelupPhase::AEPhase_RequestingEnhancement);
	BII_CHECK(CopiedAutoItemLevelupMainReqList.Num() > 0);

	//if (TransientLobbyGM)
	//{
	//	TransientLobbyGM->GetWorldTimerManager().ClearTimer(AutoItemLevelupResponseWaitTH);
	//}

	//// GET_TUPLE_DATA 해서 ID 맞는지라도 확인해 볼까..

	//FB2Item JustEnhancedTarget;
	//JustEnhancedTarget = GET_TUPLE_DATA(FB2ResponseLevelupItem::main_item_index, EnhancedItemInfo);
	//FAutoItemLevelupSingleSet RearSet = CopiedAutoItemLevelupMainReqList.Pop();
	//BII_CHECK(JustEnhancedTarget.InstanceUID == RearSet.TargetItemID); // 직전에 ReqForSingleAutoItemLevelupList 로 요청을 보낸 것에 대한 응답이 맞는지 체크
	//
	//CachedAutoItemLevelupResultItems.Add(JustEnhancedTarget.InstanceUID, JustEnhancedTarget); // 결과 UI 에 뿌릴 데이터 캐싱.
	//
	//// 여전히 남아 있는지 여부에 따라 계속 진행 혹은 종료.
	//if (CopiedAutoItemLevelupMainReqList.Num() > 0)
	//{
	//	ReqForSingleAutoItemLevelupList();
	//}
	//else
	//{
	//	EndAutoItemLevelup();
	//}
}

void UB2LobbyInventory::ForceEndAutoItemLevelupTimerCB()
{
	EndAutoItemLevelup(false, false);
}

void UB2LobbyInventory::EndAutoItemLevelup(bool bEndOfCostRequestOnly, bool bEndOnOperationSuccess)
{
	//SortedAutoItemLevelupList.Empty();
	//CopiedAutoItemLevelupCostReqList.Empty();
	//CopiedAutoItemLevelupMainReqList.Empty();
	//AutoItemLevelupPhase = EAutoItemLevelupPhase::AEPhase_None;
	//AutoItemLevelupTotalCost = 0;

	//if (TransientLobbyGM)
	//{
	//	TransientLobbyGM->GetWorldTimerManager().ClearTimer(AutoItemLevelupResponseWaitTH);
	//}

	//// 이후 스텝은 실제 자동강화 요청이 끝난 경우에만..
	//if (!bEndOfCostRequestOnly)
	//{
	//	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//	// 자동 강화 팝업이 열려 있다면 여기서 닫고 결과창을 띄운다.
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelup);
	//	if (bEndOnOperationSuccess)
	//	{
	//		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelupResult, FDJLegacySubpopupAddPanelInfo());
	//	}
	//}

	//CachedAutoItemLevelupResultItems.Empty(); // 요건 최소한 자동강화 결과창 쪽에서 (생성 시) 사용한 후에 비운다.
}

void UB2LobbyInventory::ResponseEquipCostumeItems(const FB2EquipCostumePtr& EquipCostumePtr)
{
	if (EquipCostumePtr == nullptr)
	{
		return;
	}

	const int32 iCostumeItemNotUsedPresetID = 0;
	EquippedItems(iCostumeItemNotUsedPresetID, EquipCostumePtr->equiped_costume_ids, EquipCostumePtr->unequiped_costume_ids);
}

void UB2LobbyInventory::ResponseUnEquipCostumeItems(const FB2UnequipCostumePtr& UnEquipCostumePtr)
{
	if (UnEquipCostumePtr == nullptr)
	{
		return;
	}

	const int32 iCostumeItemNotUsedPresetID = 0;
	UnEquippedItems_New(UnEquipCostumePtr->unequiped_costume_ids, iCostumeItemNotUsedPresetID);
}

void UB2LobbyInventory::ResponseGetChangeItemCharacterTypeCost(const FB2ResponseGetChangeItemCharacterTypeCostPtr& InGetChangeItemCharacterTypeCostPtr)
{
	ChangeItem.Empty();

	ChangeItem.SetNum(InGetChangeItemCharacterTypeCostPtr->change_items.Num());

	for (int32 i = 0; i < ChangeItem.Num(); ++i)
	{
		ChangeItem[i] = InGetChangeItemCharacterTypeCostPtr->change_items[i];
	}

	DarkCrystalCost = InGetChangeItemCharacterTypeCostPtr->dark_crystal_cost;
	this->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_HeroChange, ItemOpTargetItemID);
}

void UB2LobbyInventory::OpenPopupItemLock(const FText& InAskingItemActText, int64 TargetIDUID)
{
	/*UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_AskForUnlockForSomeItemAct")), InAskingItemActText),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this, TargetIDUID]() {
		LobbyReqToggleItemLockClass<int64, bool>::GetInstance().Signal(TargetIDUID, false);
	})
	);*/
}

void UB2LobbyInventory::ResponseLockTotems(const FB2LockTotemsPtr& LockTotemsPtr)
{
	/*if (LockTotemsPtr == nullptr)
	{
		return;
	}
	
	BladeIIGameImpl::GetClientDataStore().LockTotems(LockTotemsPtr);

	for (int i = 0; i < LockTotemsPtr->locked_totems.Num(); ++i)
	{
		FB2Totem TotemInfo(LockTotemsPtr->locked_totems[i]);
		UpdateTotemInfoPopupControlClass<FB2Totem>::GetInstance().Signal(TotemInfo);
	}
	
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();*/
}

void UB2LobbyInventory::ResponseUnlockTotems(const FB2UnlockTotemsPtr& UnlockTotemsPtr)
{
	/*if (UnlockTotemsPtr == nullptr)
	{
		return;
	}

	BladeIIGameImpl::GetClientDataStore().UnlockTotems(UnlockTotemsPtr);

	for (int i = 0; i < UnlockTotemsPtr->unlocked_totems.Num(); ++i)
	{
		FB2Totem TotemInfo(UnlockTotemsPtr->unlocked_totems[i]);
		UpdateTotemInfoPopupControlClass<FB2Totem>::GetInstance().Signal(TotemInfo);
	}

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();*/
}

void UB2LobbyInventory::ResponseSellTotems(const FB2SellTotemsPtr& SellTotemsPtr)
{
	/*if (SellTotemsPtr == nullptr)
	{
		return;
	}

	const int32 GoldBefore = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();

	BladeIIGameImpl::GetClientDataStore().SellTotem(SellTotemsPtr);

	switch (ItemSellLoopType)
	{
	case EItemLoopType::EI_Looping:
	{
		GetSellItemGold += BladeIIGameImpl::GetClientDataStore().GetGoldAmount() - GoldBefore;

		if (TransientLobbyGM && TransientLobbyGM->IsValidObj())
		{
			ResponseTime = TransientLobbyGM->GetWorld()->GetTimeSeconds();
			float BackOffTime = GetBackOffTime(RequestTime, ResponseTime);
			if (BackOffTime < 0)
			{
				SendSellItem();
			}
			else
			{
				TransientLobbyGM->GetWorldTimerManager().SetTimer(LoopRequestSellItem, this, &UB2LobbyInventory::SendSellItem, BackOffTime, false);
			}
		}
		break;
	}
	case EItemLoopType::EI_EndLoop:
		GetSellItemGold += BladeIIGameImpl::GetClientDataStore().GetGoldAmount() - GoldBefore;
		SellTotemsEndLoop();
		break;
	}

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();*/
}

void UB2LobbyInventory::SellTotemsEndLoop()
{
	/*FDJLegacySubpopupAddPanelInfo SubPopupAddPanelInfo;

	UB2UI_TotemInfoPopup* pEquippedTotemInfo = UB2UIManager::GetInstance()->GetUI<UB2UI_TotemInfoPopup>(UIFName::TotemInfoPopupLeft);
	if (pEquippedTotemInfo)
	{
		UPanelWidget* PanelWidget = pEquippedTotemInfo->GetPopupBackGroundPanel();
		if (PanelWidget)
		{
			SubPopupAddPanelInfo = FDJLegacySubpopupAddPanelInfo(pEquippedTotemInfo, PanelWidget);
		}
	}

	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_SellItemPopup, SubPopupAddPanelInfo);
	if (TransientLobbyGM)
	{
		UB2LobbyUI_SellItemPopup* SellItemPopup = Cast<UB2LobbyUI_SellItemPopup>(TransientLobbyGM->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_SellItemPopup));
		if (SellItemPopup)
		{
			SellItemPopup->PopupAsSellTotemResult(GetSellItemGold);
		}

		if (TransientLobbyGM->IsValidObj())
		{
			TransientLobbyGM->GetWorldTimerManager().ClearTimer(LoopRequestSellItem);
		}
	}
	UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);*/
}

void UB2LobbyInventory::ResponseDismantleTotems(const FB2DismantleTotemsPtr& DismantleTotemsPtr)
{
	if (DismantleTotemsPtr == nullptr)
	{
		return;
	}
	/*
	BladeIIGameImpl::GetClientDataStore().DecompositionTotem(DismantleTotemsPtr);

	GetDismantleItems.Add(static_cast<int32>(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE), DismantleTotemsPtr->obtain_dimensional_piece_count);
	GetDismantleItemsKey.AddUnique(static_cast<int32>(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE));

	switch (ItemDecompositionLoopType)
	{
	case EItemLoopType::EI_Looping:
	{
		if (TransientLobbyGM && TransientLobbyGM->IsValidObj())
		{
			ResponseTime = TransientLobbyGM->GetWorld()->GetTimeSeconds();
			float BackOffTime = GetBackOffTime(RequestTime, ResponseTime);
			if (BackOffTime < 0)
			{
				SendDecompositionItem();
			}
			else
			{
				TransientLobbyGM->GetWorldTimerManager().SetTimer(LoopRequestDecompositionItem, this, &UB2LobbyInventory::SendDecompositionItem, BackOffTime, false);
			}
		}
		break;
	}
	case EItemLoopType::EI_EndLoop:
		DismantleItemsEndLoop();
		break;
	}

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();*/
}

void UB2LobbyInventory::ShowTotemRefineOpFx()
{
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_TotemSmelting)
	{
		OnItemOpResponseCommon(ELobbyInvenItemOpMode::EIVIOP_TotemSmelting);
	}
}

void UB2LobbyInventory::GetCachedAutoItemLevelupResultItems(TArray<FB2Item>& OutResults)
{
	for (TPair<int64, FB2Item>& Elem : CachedAutoItemLevelupResultItems)
	{
		OutResults.Add(Elem.Value);
	}
}

void UB2LobbyInventory::GetAutoItemLevelupPreviewInfo(const TArray<FB2Item>& AllBulkList, int32& Cost, int32& LvUpItemCount, int32& IngredItemCount)
{ 
	// UI 프리뷰 용도임. 리스트 소팅하고 비용 계산까지만 하고 손 씻음.

	TArray<FAutoItemLevelupSingleSet> LocalSortedList;
	
	SortAutoLevelupItemList(AllBulkList, LocalSortedList);

	int32 LevelupCost = 0;

	for (const FAutoItemLevelupSingleSet& ThisSet : LocalSortedList)
	{
		// FAutoItemLevelupSingleSet 엔 UID 만 들어있어서 다시 찾아옴.. 바보같군
		FB2Item ThisTargetItem;
		if (!FindFromAll(ThisTargetItem, ThisSet.TargetItemID)){
			continue;
		}

		TArray<FB2Item> AllIngredItems;
		for (int64 ThisIngredID : ThisSet.IngredientsID)
		{
			FB2Item ThisIngredItem;
			if (FindFromAll(ThisIngredItem, ThisIngredID))
			{
				AllIngredItems.Add(ThisIngredItem);
			}
		}
		IngredItemCount += AllIngredItems.Num();
		LevelupCost += GetTotalCost_PreviewItemLevelup(ThisTargetItem, AllIngredItems);
	}
	Cost = LevelupCost;
	LvUpItemCount = LocalSortedList.Num();
}

void UB2LobbyInventory::LockedItems(const FB2LockedItems& LockedItems)
{
	BladeIIGameImpl::GetClientDataStore().LockItems(LockedItems);

	//GetB2ItemArray(FB2ResponseLockItem::locked_items_index, LockedItems);
	//for (auto Item : B2ItemArray)
	//{
	//	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Signal(Item.InstanceUID); // 열려 있을 세부 정보창 업데이트를 위해..
	//}
	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal(); // 잠금 상태에 따라 정렬도 다시 해야 하므로 단일 아이템만 업데이트 하는 게 아니라 전체 업데이트 해 준다.
}

void UB2LobbyInventory::UnlockedItems(const FB2UnlockedItems& UnlockedItems)
{
	BladeIIGameImpl::GetClientDataStore().UnLockItems(UnlockedItems);

	//GetB2ItemArray(FB2ResponseUnlockItem::unlocked_items_index, UnlockedItems);
	//for (auto Item : B2ItemArray)
	//{
	//	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Signal(Item.InstanceUID); // 열려 있을 세부 정보창 업데이트를 위해..
	//}
	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal(); // 잠금 상태에 따라 정렬도 다시 해야 하므로 단일 아이템만 업데이트 하는 게 아니라 전체 업데이트 해 준다.
}

//////////////////////////////////////////////////////////////////////
// Item Sell
void UB2LobbyInventory::SendSellItem()
{
	/*const int MaxCount = 10;
	TArray<int64> SendItemUID;
	int SellItemNum = SellItemUIDs.Num();

	if (SellItemNum > MaxCount)
	{
		ItemSellLoopType = EItemLoopType::EI_Looping;
		for (int32 i = 0; i < MaxCount; i++)
		{
			SendItemUID.Add(SellItemUIDs.Pop());
		}
	}
	else
	{
		ItemSellLoopType = EItemLoopType::EI_EndLoop;
		for (int32 i = 0; i < SellItemNum; i++)
		{
			SendItemUID.Add(SellItemUIDs.Pop());
		}
	}

	if (TransientLobbyGM && TransientLobbyGM->IsValidObj())
	{
		RequestTime = TransientLobbyGM->GetWorld()->GetTimeSeconds();
		TransientLobbyGM->GetWorldTimerManager().ClearTimer(LoopRequestSellItem);
	}

	if (bUseTotem)
		data_trader::Retailer::GetInstance().RequestSellTotems(SendItemUID);
	else
		data_trader::Retailer::GetInstance().RequestSellItem(SendItemUID);*/
}

void UB2LobbyInventory::ResponseSellItems(const FB2SoldItems& SoldItems)
{
	/*const int32 GoldBefore = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();

	BladeIIGameImpl::GetClientDataStore().SellItem(SoldItems);

	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();

	switch (ItemSellLoopType)
	{
	case EItemLoopType::EI_Looping:
	{
		GetSellItemGold += BladeIIGameImpl::GetClientDataStore().GetGoldAmount() - GoldBefore;

		if (TransientLobbyGM && TransientLobbyGM->IsValidObj())
		{
			ResponseTime = TransientLobbyGM->GetWorld()->GetTimeSeconds();
			float BackOffTime = GetBackOffTime(RequestTime, ResponseTime);
			if (BackOffTime < 0)
			{
				SendSellItem();
			}
			else
			{
				TransientLobbyGM->GetWorldTimerManager().SetTimer(LoopRequestSellItem, this, &UB2LobbyInventory::SendSellItem, BackOffTime, false);
			}
		}
		break;
	}
	case EItemLoopType::EI_EndLoop:
		GetSellItemGold += BladeIIGameImpl::GetClientDataStore().GetGoldAmount() - GoldBefore;
		SellItemsEndLoop();
		break;
	}

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();*/
}

void UB2LobbyInventory::SellItemsEndLoop()
{
	/*DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_SellItemPopup, FDJLegacySubpopupAddPanelInfo());
	if (TransientLobbyGM)
	{
		UB2LobbyUI_SellItemPopup* SellItemPopup = Cast<UB2LobbyUI_SellItemPopup>(TransientLobbyGM->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_SellItemPopup));
		if (SellItemPopup)
		{
			SellItemPopup->PopupAsSellResult(GetSellItemGold);
		}

		if (TransientLobbyGM->IsValidObj())
		{
			TransientLobbyGM->GetWorldTimerManager().ClearTimer(LoopRequestSellItem);
		}
	}
	UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);*/
}

void UB2LobbyInventory::SellConsumableItem(const FB2SellConsumableItem& SellConsumableItem)
{
	/*const int32 GoldBefore = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();

	BladeIIGameImpl::GetClientDataStore().SellConsumableItem(SellConsumableItem);

	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();*/
}

//////////////////////////////////////////////////////////////////////
// Item Convert
void UB2LobbyInventory::SendConvertData()
{
	//어떤 아이템을 할껀지 서버로 데이터를 보내겟 끔 해야 함.
	//변환할 아이템 ID를 서버로 보낼 예정. 밑에서 똑같이 id를 받고 필요한 테이블을 조회하도록 해야 함
	data_trader::Retailer::GetInstance().RequestConvertData(templateId);
}
void UB2LobbyInventory::ResponseConvertData(const FB2ConvertData& ConvertData)
{
	//BladeIIGameImpl::GetClientDataStore().ConvertData(ConvertData);
	//if (GET_TUPLE_DATA(FB2ResponseConvertData::templateId_index, ConvertData) == 0)
	//{
	//	UE_LOG(LogBladeII, Error, TEXT("ConvertData is NULL."));
	//	return;
	//}
	//DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ConvertItemPopup, FDJLegacySubpopupAddPanelInfo());
	//check(TransientLobbyGM); // 없으면 바보.
	//ConvertPopupPointer = Cast<UB2LobbyUI_ConvertItemPopup>(TransientLobbyGM->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_ConvertItemPopup));

	//ConvertPopupPointer->PopupMain(templateUID, GET_TUPLE_DATA(FB2ResponseConvertData::exchange_remain_count_gold_index, ConvertData), GET_TUPLE_DATA(FB2ResponseConvertData::exchange_remain_count_gem_index, ConvertData), FConvertItemPopupOnClick::CreateLambda([this]() {}));
}

void UB2LobbyInventory::SendConvertItem()
{
	data_trader::Retailer::GetInstance().RequestConvertItem(convertItemID, convertType);
}

void UB2LobbyInventory::ResponseConvertItem(const FB2ConvertItem& ConvertItem)
{
	////ConvertPopupPointer->ResponseConvertItem(GET_TUPLE_DATA(FB2ResponseConvertItem::exchangeTemplateId_index, ConvertItem));

	//if(GET_TUPLE_DATA(FB2ResponseConvertItem::exchangetype_index, ConvertItem) == b2::protocol::commondata::ExchangePriceType::EC_GOLD)
	//	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, GET_TUPLE_DATA(FB2ResponseConvertItem::currentGold_index, ConvertItem));
	//else if (GET_TUPLE_DATA(FB2ResponseConvertItem::exchangetype_index, ConvertItem) == b2::protocol::commondata::ExchangePriceType::EC_GEM)
	//	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, GET_TUPLE_DATA(FB2ResponseConvertItem::currentCash_index, ConvertItem));
	//

	////BladeIIGameImpl::GetClientDataStore().DecSharedConsumableAmountOfType(
	////FPCItemContainer &ItemContainer = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems();

	//FB2Item convertedItem;
	//convertedItem = GET_TUPLE_DATA(FB2ResponseConvertItem::exchangeItem_index, ConvertItem);

	//ConvertPopupPointer->ResponseConvertItem(convertedItem.ItemRefID);
	//
	//BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountIncrease(convertedItem.ItemRefID, GET_TUPLE_DATA(FB2ResponseConvertItem::exchangeItem_index, ConvertItem));
	//if(ConvertPopupPointer->resultAfterConvertItemQuantity>1)
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().IncSharedConsumableAmountOfType(convertedItem.ItemRefID, GET_TUPLE_DATA(FB2ResponseConvertItem::exchangeItemAmount_index, ConvertItem) - 1);
	//BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().DecSharedConsumableAmountOfType(ConvertPopupPointer->beforeconvertItemID_ref, ConvertPopupPointer->requiredBeforeConvertItemQuantity);

	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_StoredItemDetail);

	//ConvertPopupPointer->CloseMe();

	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();

	//data_trader::Retailer::GetInstance().RequestConvertData(templateId);
}

//////////////////////////////////////////////////////////////////////
// Item Decomposition
void UB2LobbyInventory::SendDecompositionItem()
{
	//const int MaxCount = 10;
	//TArray<int64> SendItemUIDs;
	//int32 DecompositionItemNum = DecompositionItemUIDs.Num();

	//if (DecompositionItemNum > MaxCount)
	//{
	//	ItemDecompositionLoopType = EItemLoopType::EI_Looping;
	//	for (int32 i = 0; i < MaxCount; i++)
	//	{
	//		SendItemUIDs.Add(DecompositionItemUIDs.Pop());
	//	}
	//}
	//else
	//{
	//	ItemDecompositionLoopType = EItemLoopType::EI_EndLoop;
	//	for (int32 i = 0; i < DecompositionItemNum; i++)
	//	{
	//		SendItemUIDs.Add(DecompositionItemUIDs.Pop());
	//	}
	//}

	//if (TransientLobbyGM && TransientLobbyGM->IsValidObj())
	//{
	//	RequestTime = TransientLobbyGM->GetWorld()->GetTimeSeconds();
	//	TransientLobbyGM->GetWorldTimerManager().ClearTimer(LoopRequestDecompositionItem);
	//}

	//if(bUseTotem)
	//	data_trader::Retailer::GetInstance().RequestDismantleTotems(SendItemUIDs);
	//else
	//	data_trader::Retailer::GetInstance().RequestDismantleItems(SendItemUIDs);
}

void UB2LobbyInventory::ResponseDismantleItems(const FB2DismantleItems& DismantleItems)
{
	//const TArray<b2network::B2ItemServerInfoPtr> ExtractedItems = GET_TUPLE_DATA(FB2ResponseDismantleItems::extracted_items_index, DismantleItems);
	//const TArray<int32> ExtractedItemChangeAmoumt = GET_TUPLE_DATA(FB2ResponseDismantleItems::extracted_item_change_amount_index, DismantleItems);
	//for (int i = 0; i < ExtractedItems.Num(); ++i)
	//{
	//	BladeIIGameImpl::GetClientDataStore().DecompositionItem(DismantleItems);

	//	GetDismantleItems.Add(ExtractedItems[i]->template_id, ExtractedItemChangeAmoumt[i]);
	//	GetDismantleItemsKey.AddUnique(ExtractedItems[i]->template_id);
	//	
	//	switch (ItemDecompositionLoopType)
	//	{
	//	case EItemLoopType::EI_Looping:
	//	{
	//		if (TransientLobbyGM && TransientLobbyGM->IsValidObj())
	//		{
	//			ResponseTime = TransientLobbyGM->GetWorld()->GetTimeSeconds();
	//			float BackOffTime = GetBackOffTime(RequestTime, ResponseTime);
	//			if (BackOffTime < 0)
	//			{
	//				SendDecompositionItem();
	//			}
	//			else
	//			{
	//				TransientLobbyGM->GetWorldTimerManager().SetTimer(LoopRequestDecompositionItem, this, &UB2LobbyInventory::SendDecompositionItem, BackOffTime, false);
	//			}
	//		}
	//		break;
	//	}
	//	case EItemLoopType::EI_EndLoop:
	//		DismantleItemsEndLoop();
	//		break;
	//	}
	//}

	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyInventory::DismantleItemsEndLoop()
{
	/*if (TransientLobbyGM)
	{
		TArray<int32> DismantleItems;
		DismantleItems.Empty();
		for (int32 i = 0; i < GetDismantleItemsKey.Num(); ++i)
		{
			TArray<int32> DismantleItemsTemp;
			DismantleItemsTemp.Empty();
			GetDismantleItems.MultiFind(GetDismantleItemsKey[i], DismantleItemsTemp);

			int32 DismantlesAmount = 0;
			for (int32& Dis : DismantleItemsTemp)
			{
				DismantlesAmount += Dis;
			}
			DismantleItems.Add(DismantlesAmount);
		}

		if (GetDismantleItemsKey.Num() > 0)
		{
			UB2UIDecompositionObtainPopup* DecompositionObtainPopup = UB2UIManager::GetInstance()->OpenUI<UB2UIDecompositionObtainPopup>(UIFName::DecompositionObtainPopup);
			if (DecompositionObtainPopup && GetDismantleItemsKey.Num() > 0)
				DecompositionObtainPopup->SetEquipItemInfo(GetDismantleItemsKey, DismantleItems);
		}

		if (TransientLobbyGM->IsValidObj())
		{
			TransientLobbyGM->GetWorldTimerManager().ClearTimer(LoopRequestSellItem);
		}
	}*/
	
	UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);
}

#if !UE_BUILD_SHIPPING
bool UB2LobbyInventory::IsInventoryTotallyEmpty()
{ // 개발 용도 이외에도 필요하게 되면 !UE_BUILD_SHIPPING 제거하고 사용하도록.
	for (int32 IPI = 0; IPI < GetMaxPCClassNum(); ++IPI)
	{
		TMap<int64, FB2Item>* WeaponMap = PCItems[IPI].GetItemMapForInvenType(EItemInvenType::EIIVT_Weapon);
		TMap<int64, FB2Item>* ProtectionMap = PCItems[IPI].GetItemMapForInvenType(EItemInvenType::EIIVT_Protection);
		TMap<int64, FB2Item>* AccessoryMap = PCItems[IPI].GetItemMapForInvenType(EItemInvenType::EIIVT_Accessory);

		if (WeaponMap && WeaponMap->Num() > 0)
		{
			return false;
		}
		if (ProtectionMap && ProtectionMap->Num() > 0)
		{
			return false;
		}
		if (AccessoryMap && AccessoryMap->Num() > 0)
		{
			return false;
		}
	}
	if (GetConsumablesList().Num() > 0)
	{
		return false;
	}

	return true;
}

void UB2LobbyInventory::DevOnlyRequestDummyItems(int32 RemainingNum, int32 ItemRefID, float NextRequestInterval)
{
	//if (TransientLobbyGM && TransientLobbyGM->IsValidObj())
	//{
	//	TransientLobbyGM->GetWorldTimerManager().ClearTimer(RequestDummyItemsTH);
	//}

	//if (RemainingNum > 0)
	//{		
	//	data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), ItemRefID, FItemRefIDHelper::ExtractItemStarGradeFromRefID(ItemRefID)));
	//	
	//	// RemainingNum 을 하나 줄인 채로 다음 요청을 기약한다.
	//	if (TransientLobbyGM && TransientLobbyGM->IsValidObj())
	//	{
	//		TransientLobbyGM->GetWorldTimerManager().SetTimer(RequestDummyItemsTH, 
	//			FTimerDelegate::CreateUObject(this, &UB2LobbyInventory::DevOnlyRequestDummyItems, RemainingNum - 1, ItemRefID, NextRequestInterval), 
	//			NextRequestInterval, false
	//			);
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogBladeII, Log, TEXT("DevOnlyRequestDummyItems with RefID %d just completed."), ItemRefID);
	//}
}

void UB2LobbyInventory::DevOnlyReplaceEquippedOptions(EPCClass InLocalCharClass, EItemEquipPlace InEquipPlace, const TArray<FItemOption>& InOptionsToReplace, bool bMaintainCurrentOptions)
{ // 전투 밸런스 테스트 용. 특정 캐릭터의 특정 장착한 아이템의 옵션 바꿔치기.
	FB2Item FoundEquipItem;
	if (GetEquippedItemAtPlace(FoundEquipItem, InEquipPlace, InLocalCharClass))
	{
		if (!bMaintainCurrentOptions)
		{
			FoundEquipItem.IntrinsicOptions.Empty();
			FoundEquipItem.RandomOptions.Empty();
		}

		TArray<FItemOption> RemainingOptionsToReplace = InOptionsToReplace;

		if (bMaintainCurrentOptions)
		{ // 기존 꺼 유지하는 경우 기존에 있는 거 찾고 업데이트부터 시도.
			for (int32 ROI = 0; ROI < RemainingOptionsToReplace.Num(); ++ROI)
			{
				FItemOption& ThisToReplace = RemainingOptionsToReplace[ROI];
				bool bFoundAndUpdated = false;
				for (FItemOption& ThisExisting : FoundEquipItem.IntrinsicOptions)
				{
					if (ThisExisting.OptionType == ThisToReplace.OptionType)
					{
						ThisExisting.RawOptionAmount = ThisToReplace.RawOptionAmount; // 변경.
						bFoundAndUpdated = true;
						break;
					}
				}
				if (!bFoundAndUpdated) // 못 찾았다면 랜덤옵션 검색 시도
				{
					for (FItemOption& ThisExisting : FoundEquipItem.RandomOptions)
					{
						if (ThisExisting.OptionType == ThisToReplace.OptionType)
						{
							ThisExisting.RawOptionAmount = ThisToReplace.RawOptionAmount; // 변경.
							bFoundAndUpdated = true;
							break;
						}
					}
				}
				if (bFoundAndUpdated)
				{
					RemainingOptionsToReplace.RemoveAt(ROI);
					--ROI;
				}
			}
		}

		// 아직 남은 것들은 기존에 없던 옵션이니 고유옵션, 랜덤옵션 가리지는 않고 순서대로 가능할 때까지 채워넣기만 한다.
		// 옵션별로 가능한 수치는 개발용 기능이라 틀려서 큰일날 건 없으니 여기에 하드코딩함. 클라이언트에서는 옵션 데이터를 받기만 해서 수치 제한을 관리하지는 않는다.
		
		while (RemainingOptionsToReplace.Num() > 0 && FoundEquipItem.IntrinsicOptions.Num() < 4)
		{
			FItemOption ThisToReplace = RemainingOptionsToReplace[0]; // 앞에서부터..
			FoundEquipItem.IntrinsicOptions.Add(ThisToReplace); // 기존꺼랑 중복될 가능성이 여전히 있기는 하지만 그건 InOptionsToReplace 를 알아서 잘 주는 걸로.			
			RemainingOptionsToReplace.RemoveAt(0);
		}
		while (RemainingOptionsToReplace.Num() > 0 && FoundEquipItem.RandomOptions.Num() < 3)
		{ // 마찬가지로.
			FItemOption ThisToReplace = RemainingOptionsToReplace[0];
			FoundEquipItem.RandomOptions.Add(ThisToReplace);
			RemainingOptionsToReplace.RemoveAt(0);
		}

		UpdateSingleItemData(FoundEquipItem.InstanceUID, FoundEquipItem); // 최종적으로 변경된 데이터를 적용.
	}
}

#endif

#if WITH_EDITOR
void UB2LobbyInventory::CheckForGenerateDummyDataSuggestion()
{
#if !PLATFORM_MAC
	if (!bSelectedNoForDummyDataSuggestion && IsInventoryTotallyEmpty())
	{
		if (FB2ErrorMessage::Open(EAppMsgType::YesNo, FText::FromString(
			FString::Printf(TEXT("[안내] 인벤토리가 비어있습니다.\n\"예(Yes)\"를 선택하시면 테스트 용 아이템들을 채워드립니다.\n\n"))
			)) == EAppReturnType::Yes)
		{
			data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddDemoItem"));
		}
		else
		{ // 한번 No 를 선택했으면 현재 레벨에서는 더 물어보지 않게.
			bSelectedNoForDummyDataSuggestion = true;
		}
	}
#endif
}

void UB2LobbyInventory::DevOnlyItemLevelupProgressSceneTest(int32 IngredNum)
{
	// 기본적인 뷰 전환 같은 건 하지만 인벤토리 페이지가 아니면 잘 안먹힐 수 있음. 
	//if (TransientLobbyGM && TransientLobbyGM->IsValidObj() && CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None) // 가능한 ItemOp 모드가 아닌 상태가 좋을 듯..
	//{
	//	bItemOpProgressSceneTestMode = true;

	//	// 강화연출에선 ItemOpTargetItemID 를 세팅해봐야 소용이 없다. 테스트 함수라 어떤 아이템이 있는지 조사해서 세팅할 수도 없는 노릇이고
	//	// 업데이트 전의 캐싱되는 타겟 아이템 데이터를 세팅.
	//	CachedItemOpTargetData = FB2Item();
	//	CachedItemOpTargetData.ItemRefID = 1000100; // 실제로 ItemInfo 에 있는 RefID 를 넣어줄 필요가 있다.
	//	CachedItemOpTargetData.InstanceUID = 32423243; // UID 도 뭔가 값을 넣어주긴 해야 함.
	//	DevOnlyLoadItemClientTestData(CachedItemOpTargetData);
	//	CachedItemOpTargetData.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//	IngredNum = FMath::Clamp(IngredNum, 1, ItemLevelupIngredLimit);

	//	GhostItemOpIngreds.Empty(); // 강화 연출 창에서 사용할 재료 아이템 데이터.
	//	for (int32 GI = 0; GI < IngredNum; ++GI)
	//	{
	//		FB2Item NewIngred;
	//		NewIngred.ItemRefID = 1000100; // 실제로 ItemInfo 에 있는 RefID 를 넣어줄 필요가 있다.
	//		NewIngred.InstanceUID = 4820942398429 + GI;

	//		DevOnlyLoadItemClientTestData(NewIngred);
	//		NewIngred.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//		GhostItemOpIngreds.Add(NewIngred);
	//	}

	//	CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_Levelup;		
	//	// ChangeLobbyUIPage 전에 View 부터 먼저 셋업해야 한다. 연출 UI Widget 안에서 카메라에 접근하는 게 있음.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Levelup);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemLevelUpProgress);
	//}
}

void UB2LobbyInventory::DevOnlyItemEnhanceProgressSceneTest(int32 IngredNum)
{
	// 기본적인 뷰 전환 같은 건 하지만 인벤토리 페이지가 아니면 잘 안먹힐 수 있음. 
	//if (TransientLobbyGM && TransientLobbyGM->IsValidObj() && CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None) // 가능한 ItemOp 모드가 아닌 상태가 좋을 듯..
	//{
	//	bItemOpProgressSceneTestMode = true;

	//	// 강화연출에선 ItemOpTargetItemID 를 세팅해봐야 소용이 없다. 테스트 함수라 어떤 아이템이 있는지 조사해서 세팅할 수도 없는 노릇이고
	//	// 업데이트 전의 캐싱되는 타겟 아이템 데이터를 세팅.
	//	CachedItemOpTargetData = FB2Item();
	//	CachedItemOpTargetData.ItemRefID = 1000100; // 실제로 ItemInfo 에 있는 RefID 를 넣어줄 필요가 있다.
	//	CachedItemOpTargetData.InstanceUID = 32423243; // UID 도 뭔가 값을 넣어주긴 해야 함.

	//	DevOnlyLoadItemClientTestData(CachedItemOpTargetData);
	//	CachedItemOpTargetData.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//	IngredNum = FMath::Clamp(IngredNum, 1, ItemLevelupIngredLimit);

	//	GhostItemOpIngreds.Empty(); // 강화 연출 창에서 사용할 재료 아이템 데이터.
	//	for (int32 GI = 0; GI < IngredNum; ++GI)
	//	{
	//		FB2Item NewIngred;
	//		NewIngred.ItemRefID = 1000100; // 실제로 ItemInfo 에 있는 RefID 를 넣어줄 필요가 있다.
	//		NewIngred.InstanceUID = 4820942398429 + GI;

	//		DevOnlyLoadItemClientTestData(NewIngred);
	//		NewIngred.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//		GhostItemOpIngreds.Add(NewIngred);
	//	}

	//	CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_Enhancement;
	//	// ChangeLobbyUIPage 전에 View 부터 먼저 셋업해야 한다. 연출 UI Widget 안에서 카메라에 접근하는 게 있음.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Enhancement);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemEnhanceProgress);
	//}
}

void UB2LobbyInventory::DevOnlyItemCUSProgressSceneTest(ELobbyInvenItemOpMode InOpMode)
{	
	//if (InOpMode == ELobbyInvenItemOpMode::EIVIOP_None || InOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup){
	//	return; // 강화는 여기서 처리 안 함.
	//}

	//// 기본적인 뷰 전환 같은 건 하지만 인벤토리 페이지가 아니면 잘 안먹힐 수 있음. 
	//if (TransientLobbyGM && TransientLobbyGM->IsValidObj() && CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None) // 가능한 ItemOp 모드가 아닌 상태가 좋을 듯..
	//{
	//	bItemOpProgressSceneTestMode = true;

	//	// 타겟 아이템 데이터.
	//	CachedItemOpTargetData = FB2Item();
	//	// 실제로 ItemInfo 에 있는 RefID 를 넣어줄 필요가 있다. 초월이면 6성 장비 껄로다가
	//	CachedItemOpTargetData.ItemRefID = InOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass ? 1000600 : 1000100; 
	//	CachedItemOpTargetData.InstanceUID = 32423243; // UID 도 뭔가 값을 넣어주긴 해야 함.
	//	DevOnlyLoadItemClientTestData(CachedItemOpTargetData);
	//	CachedItemOpTargetData.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;
	//	CachedItemOpTargetData.Level = 30; // 합성승급 등은 특별히 Level 을 맞춰줘야 함.

	//	GhostItemOpIngreds.Empty();
	//	if (InOpMode == ELobbyInvenItemOpMode::EIVIOP_Composition || InOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass)
	//	{ // 재료 아이템 데이터를 필요로 하는 모드
	//		FB2Item NewIngred;
	//		// 실제로 ItemInfo 에 있는 RefID 를 넣어줄 필요가 있다. 초월이면 6성 장비 껄로다가
	//		NewIngred.ItemRefID = InOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass ? 1000600 : 1000100;
	//		NewIngred.InstanceUID = 4820942398429;
	//		DevOnlyLoadItemClientTestData(NewIngred);
	//		NewIngred.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;
	//		NewIngred.Level = 30; // 마찬가지로 재료도 30레벨 맞춰줌
	//		GhostItemOpIngreds.Add(NewIngred);
	//	}

	//	CurrentItemOpMode = InOpMode;

	//	// ChangeLobbyUIPage 전에 View 부터 먼저 셋업해야 한다. 연출 UI Widget 안에서 카메라에 접근하는 게 있음.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(InOpMode);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(
	//		(InOpMode == ELobbyInvenItemOpMode::EIVIOP_Composition) ? ELobbyUIPages::ELUP_ItemComposeProgress :
	//		((InOpMode == ELobbyInvenItemOpMode::EIVIOP_Upgrade) ? ELobbyUIPages::ELUP_ItemUpgradeProgress :
	//			ELobbyUIPages::ELUP_ItemSurpassProgress)
	//	);
	//}
}

#endif

void UB2LobbyInventory::ChangePreset(const EPCClass InPCClass, const EItemPresetSlotType PresetSlot)
{
	//FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	//FPCItemContainer &ItemContainer = CliDataStore.GetLocalCharacterData().GetUserAllItems();
	//int32 PrePresetID = ItemContainer.GetCurrentItemPresetID(InPCClass);
	//if (PrePresetID != (int32)PresetSlot)
	//{
	//	TArray<EPCClass> PCPartUpdateSignalClass; // 외관상으로 변경되는 아이템을 장착한 PC 클래스들 모두 모음. 각 클래스에다 신호를 보내기 위해
	//	TArray<EPCClass> AnyEquipStateChangedClass; // 외관 장착 파트 변경되는 경우가 아니라도 전투력 반영은 될 수 있으니 클래스 따로 모음
	//					
	//	//  UIDoc 리셋
	//	for (int32 i = (int32)EItemEquipPlace::EIEP_Weapon; i < (int32)EItemEquipPlace::EIEP_End; ++i)
	//	{
	//		UB2UIDocEquip* DocEquip = UB2UIDocHelper::GetDocEquip((int32)InPCClass, i);
	//		if (DocEquip)
	//		{
	//			DocEquip->RemoveEquip();
	//		}
	//	}
	//	ItemContainer.ResetAllItems(InPCClass);
	//	ItemContainer.SetCurrentItemPresetID(InPCClass, (int32)PresetSlot);

	//	TArray<FB2Item> MergedList;
	//	TArray<int64> EquipList;
	//	GetMergedItemList(MergedList, true, InPCClass); // 일단 filter 외의 조건에서 전체 합쳐진 아이템 목록.

	//	for (const auto &Each : MergedList)
	//	{
	//		if (ItemContainer.IsInCurrentItemPreset(InPCClass, Each.InstanceUID))
	//		{
	//			EquipList.Add(Each.InstanceUID);

	//			UB2UIDocEquip* DocEquip = UB2UIDocHelper::GetDocEquip(Each.AllowedPCClass, static_cast<int32>(Each.EquipPlace));
	//			if (DocEquip)
	//			{
	//				DocEquip->DoEquip(Each);
	//			}
	//		}
	//	}
	//	ItemContainer.SwitchPresetItems(EquipList, InPCClass, (int32)PresetSlot);

	//	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();

	//	LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(InPCClass);
	//	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(InPCClass, true);

	//	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(InPCClass);
	//}

	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyInventory::NoticeSetItemEffect(const TMap<int32, int32>& PreSetItems, const TMap<int32, int32>& AfterSetItems, const bool& InIsCostumeItem /*= false*/)
{
	//if (PreSetItems.Num() == 0 && AfterSetItems.Num() == 0)
	//	return;

	//TArray<int32> PreAppliedSetIds;
	//TArray<int32> AfterAppliedSetIds;

	//for (auto Items : PreSetItems)	{ if (Items.Value > 1) PreAppliedSetIds.Add(Items.Key);	}
	//for (auto Items : AfterSetItems) { if (Items.Value > 1) AfterAppliedSetIds.Add(Items.Key); }
	//
	//TArray<int32> AddSetIds(AfterAppliedSetIds);
	//AddSetIds.RemoveAll([PreAppliedSetIds](const int32& SetItemId) { return PreAppliedSetIds.Contains(SetItemId); });
	//TArray<int32> RemoveSetIds(PreAppliedSetIds);
	//RemoveSetIds.RemoveAll([AfterAppliedSetIds](const int32& SetItemId) { return AfterAppliedSetIds.Contains(SetItemId); });

	//TArray<int32> ChangeLevelSetIds;
	//for (auto Items : PreSetItems)
	//{
	//	if (AfterSetItems.Contains(Items.Key))
	//	{
	//		if (Items.Value <= 1 || AfterSetItems[Items.Key] <= 1)
	//			continue;

	//		if (Items.Value != AfterSetItems[Items.Key])
	//			ChangeLevelSetIds.Add(Items.Key);
	//	}
	//}

	//// 안내문 우선순위 => 변경 > 추가/해체
	//// 1. 세트효과가 추가/삭제된게 있는 경우 => 세트효과 변경
	//// 2. 세트효과 증/감된게 있는 경우 => 세트효과 변경
	//// 3. 세트효과가 추가된게 있는 경우 => 세트효과 추가
	//// 4. 세트효과가 해제된게 있는 경우 => 세트효과 해제

	//ESetItemEffectNotify SetEffectType = ESetItemEffectNotify::SIEN_None;
	//if (AddSetIds.Num() != 0 && RemoveSetIds.Num() != 0)
	//	SetEffectType = ESetItemEffectNotify::SIEN_Change;
	//else if (AddSetIds.Num() != 0)
	//	SetEffectType = ESetItemEffectNotify::SIEN_Add;
	//else if (RemoveSetIds.Num() != 0)
	//	SetEffectType = ESetItemEffectNotify::SIEN_Delete;
	//else if (ChangeLevelSetIds.Num() > 0)
	//	SetEffectType = ESetItemEffectNotify::SIEN_Change;

	//if (SetEffectType != ESetItemEffectNotify::SIEN_None)
	//{
	//	if (auto* InventorySetInfoNotifyUI = UB2UIManager::GetInstance()->OpenUI<UB2UIInventorySetInfoNotify>(UIFName::SetInfoNotify))
	//		InventorySetInfoNotifyUI->SetItemEffectType(SetEffectType, InIsCostumeItem);
	//}
}

void UB2LobbyInventory::ResponseUnsealBox(const FB2ResponseUnsealBoxPtr& UnsealBoxPtr)
{
	//if (UnsealBoxPtr->left_amount == 0)
	//{
	//	TArray<int64> tempArray;
	//	tempArray.Empty();
	//	tempArray.Add(UnsealBoxPtr->seal_box_id);
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().RemoveItems(tempArray);
	//}
	//else
	//{
	//	FB2Item* sealBox = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().FindItem(UnsealBoxPtr->seal_box_id);
	//	if (sealBox != nullptr)
	//	{
	//		BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().DecSharedConsumableAmountOfType(sealBox->ItemRefID, 1);
	//	}
	//	else
	//	{
	//		return;
	//	}
	//}

	//for (int32 i = 0; i < UnsealBoxPtr->rewards.Num(); i++)
	//{
	//	if (UnsealBoxPtr->rewards[i]->reward_type == b2network::B2RewardType::ITEM)
	//	{
	//		FB2Item tempItem;
	//		tempItem = UnsealBoxPtr->rewards[i]->item;
	//		tempItem.bNewlyAddedForEquipSuggest = true;
	//		BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().AddItem(tempItem);
	//	}
	//	else if (UnsealBoxPtr->rewards[i]->reward_type == b2network::B2RewardType::TOTEM)
	//	{
	//		BladeIIGameImpl::GetClientDataStore().AddTotem(UnsealBoxPtr->rewards[i]->totem);
	//	}
	//}

	//BladeIIGameImpl::GetClientDataStore().SetGoldAmount(UnsealBoxPtr->gold);
	//BladeIIGameImpl::GetClientDataStore().SetGemAmount(UnsealBoxPtr->gem);
	////BladeIIGameImpl::GetClientDataStore().SetBladeCurPoint(UnsealBoxPtr->blade_point);
	//BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::BladePoint, UnsealBoxPtr->blade_point);
	//BladeIIGameImpl::GetClientDataStore().SetSocialPoint(UnsealBoxPtr->social_point);
	//BladeIIGameImpl::GetClientDataStore().SetDarkCrystal(UnsealBoxPtr->dark_crystal);
	//BladeIIGameImpl::GetClientDataStore().SetStageBoostTicket(UnsealBoxPtr->stage_boost_ticket);
	//BladeIIGameImpl::GetClientDataStore().SetStageAutoClearTicket(UnsealBoxPtr->stage_auto_clear_ticket);
	//BladeIIGameImpl::GetClientDataStore().SetShopTenLotteryTicket(UnsealBoxPtr->shop_ten_lottery_ticket);
	//BladeIIGameImpl::GetClientDataStore().SetFamePoint(UnsealBoxPtr->fame_point);

	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//UpdateLobbyInventoryControlClass<>::GetInstance().Signal();

	////UnsealBoxPtr
	///*
	////3D 연출 있는 (아님 그냥 파티클 시스템을 사용하는 연출 쪽
	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::SealBoxResult);
	//UB2UISealBoxResult* UISealBoxResult = UB2UIManager::GetInstance()->OpenUI<UB2UISealBoxResult>(UIFName::SealBoxResult);
	//if (UISealBoxResult)
	//{
	//	UISealBoxResult->SetRewardDataManually(UnsealBoxPtr, SealBoxClass);
	//	UISealBoxResult->BindDocAuto();
	//}
	//*/
	//
	////제작소랑 동일한 방법으로 연출하는 쪽
	//UB2UISealBoxResultSimple* UISealBoxResultSimple = UB2UIManager::GetInstance()->OpenUI<UB2UISealBoxResultSimple>(UIFName::SealBoxResultSimple);
	//if (UISealBoxResultSimple)
	//{
	//	UISealBoxResultSimple->Init();
	//	UISealBoxResultSimple->SetRewardDataManually(UnsealBoxPtr, SealBoxClass);
	//}	
	
}

void UB2LobbyInventory::ResponseSealBoxSelectChar(int32 PCClass)
{
	SealBoxClass = PCClass;
}

//SummonItem 부분에서 복사해서 가져온 부분. 텍스트나 버튼 조작, 가격 표시 부분에 조작이 가해진 상태.
void UB2LobbyInventory::ConditionalCreateCharSelectPopup()
{
//	//if (!CreatedUIP_CharSelectPopup && X_CP_UIP_CharSelectPopupPanel.IsValid())
//// 	if (!CreatedUIP_CharSelectPopup)
//// 	{
//		//CreatedUIP_CharSelectPopup = DynLoadAndCreateInCanvasPanelFull<UB2UIBackWidgetBase>(UIP_CharSelectPopupClass, this, X_CP_UIP_CharSelectPopupPanel.Get());
//
//		if (CreatedUIP_CharSelectPopup)
//		{
//			UE_LOG(LogTemp, Log, TEXT("fuction in"));
//			CreatedUIP_CharSelectPopup->Init();
//			CreatedUIP_CharSelectPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//
//			// 이 정도면 별도의 Widget part 클래스가 되어야 할 것 같은데 완전 블루프린트로 코딩이 되어 있음.
//			// 파트에서 가져오는게 상당히 많다.. ㅡㅡ
//
//			TB_CharSelectTitle = CreatedUIP_CharSelectPopup->GetSlot<UTextBlock>(FName(TEXT("TB_CharSelectTitle")));
//			TB_CharSelectFullText = CreatedUIP_CharSelectPopup->GetSlot<UTextBlock>(FName(TEXT("TB_CharSelectFullText")));
//
//			FSingleItemInfoData* ItemInfo = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(SealBoxRefID) : nullptr;
//			if (ItemInfo != nullptr)
//				TB_CharSelectTitle->SetText(ItemInfo->GetLocalizedName());
//			TB_CharSelectFullText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SealBoxClassSelect_Guide")));
//
//			BTN_CharSelectPopupClose = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Close")));
//
//			BTN_CharSelectPopupGladiator = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Gladiator")));
//			BTN_CharSelectPopupAssassin = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Assassin")));
//			BTN_CharSelectPopupWizard = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Wizard")));
//			BTN_CharSelectPopupFighter = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Fighter")));
//
//			if (CharacterButtonOnArr.Num() == static_cast<int32>(EPCClass::EPC_End))
//			{
//				CharacterButtonOnArr[static_cast<int32>(EPCClass::EPC_Gladiator)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_GladiatorOn")));
//				CharacterButtonOnArr[static_cast<int32>(EPCClass::EPC_Assassin)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_AssassinOn")));
//				CharacterButtonOnArr[static_cast<int32>(EPCClass::EPC_Wizard)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_WizardOn")));
//				CharacterButtonOnArr[static_cast<int32>(EPCClass::EPC_Fighter)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_FighterOn")));
//			}
//
//			if (CharacterButtonOffArr.Num() == static_cast<int32>(EPCClass::EPC_End))
//			{
//				CharacterButtonOffArr[static_cast<int32>(EPCClass::EPC_Gladiator)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_GladiatorOff")));
//				CharacterButtonOffArr[static_cast<int32>(EPCClass::EPC_Assassin)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_AssassinOff")));
//				CharacterButtonOffArr[static_cast<int32>(EPCClass::EPC_Wizard)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_WizardOff")));
//				CharacterButtonOffArr[static_cast<int32>(EPCClass::EPC_Fighter)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_FighterOff")));
//			}
//
//			SetButtonLineVisible(EPCClass::EPC_End);
//
//			BTN_CharSelectConfirm = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Confirm")));
//
//			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectPopupClose, &UB2LobbyInventory::CloseSelectCharacterPopupWindow);
//			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectPopupGladiator, &UB2LobbyInventory::SelectedGladiator);
//			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectPopupAssassin, &UB2LobbyInventory::SelectedAssassin);
//			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectPopupWizard, &UB2LobbyInventory::SelectedWizard);
//			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectPopupFighter, &UB2LobbyInventory::SelectedFighter);
//			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectConfirm, &UB2LobbyInventory::OnClickSelectConfirm);
//
//			UIP_CharSelectPopupCosts.Empty();
//
//			// 캐릭터 enum 순서와 일치..?
//			TWeakObjectPtr<UB2UIStoreProductCost> UIP_CharSelectPopupCost1 = CreatedUIP_CharSelectPopup->GetSlot<UB2UIStoreProductCost>(FName(TEXT("UIP_Cost1")));
//			if (UIP_CharSelectPopupCost1.IsValid())
//			{
//				UIP_CharSelectPopupCost1->SetVisibility(ESlateVisibility::Collapsed);
//			}
//			TWeakObjectPtr<UB2UIStoreProductCost> UIP_CharSelectPopupCost2 = CreatedUIP_CharSelectPopup->GetSlot<UB2UIStoreProductCost>(FName(TEXT("UIP_Cost2")));
//			if (UIP_CharSelectPopupCost2.IsValid())
//			{
//				UIP_CharSelectPopupCost2->SetVisibility(ESlateVisibility::Collapsed);
//			}
//			TWeakObjectPtr<UB2UIStoreProductCost> UIP_CharSelectPopupCost3 = CreatedUIP_CharSelectPopup->GetSlot<UB2UIStoreProductCost>(FName(TEXT("UIP_Cost3")));
//			if (UIP_CharSelectPopupCost3.IsValid())
//			{
//				UIP_CharSelectPopupCost3->SetVisibility(ESlateVisibility::Collapsed);
//			}
//			TWeakObjectPtr<UB2UIStoreProductCost> UIP_CharSelectPopupCost4 = CreatedUIP_CharSelectPopup->GetSlot<UB2UIStoreProductCost>(FName(TEXT("UIP_Cost4")));
//			if (UIP_CharSelectPopupCost4.IsValid())
//			{
//				UIP_CharSelectPopupCost4->SetVisibility(ESlateVisibility::Collapsed);
//			}
//		}
//	//}
}

void UB2LobbyInventory::CloseSelectCharacterPopupWindow()
{
	//if (CreatedUIP_CharSelectPopup)
	//{
	//	CreatedUIP_CharSelectPopup->ClosePopup();
	//	SetButtonLineVisible(EPCClass::EPC_End);
	//}

	//if (BTN_CharSelectConfirm.IsValid())
	//	BTN_CharSelectConfirm->SetIsEnabled(false);

	//SetForcedBlockHeader(false);
}

void UB2LobbyInventory::OnClickSelectConfirm()
{
	if (BTN_CharSelectConfirm.IsValid())
		BTN_CharSelectConfirm->SetIsEnabled(false);

	CloseSelectCharacterPopupWindow();

	//서버로 봉인함 해제 요청하는 곳
	//한번에 몇개 열라는 내용은 기획에 없어서 한번에 1개씩 열도록 하드코딩 됨
	data_trader::Retailer::GetInstance().RequestUnsealBox(SealBoxUID, SealBoxClass, 1);
}

void UB2LobbyInventory::SelectedGladiator()
{
	//SetCharSelectPopupButtonState(ESlateVisibility::HitTestInvisible);
	SealBoxClass = PCClassToInt(EPCClass::EPC_Gladiator);
	//SummonItem();
	SetButtonLineVisible(EPCClass::EPC_Gladiator);
	if (BTN_CharSelectConfirm.IsValid())
		BTN_CharSelectConfirm->SetIsEnabled(true);
}

void UB2LobbyInventory::SelectedAssassin()
{
	//SetCharSelectPopupButtonState(ESlateVisibility::HitTestInvisible);
	SealBoxClass = PCClassToInt(EPCClass::EPC_Assassin);
	//SummonItem();
	SetButtonLineVisible(EPCClass::EPC_Assassin);
	if (BTN_CharSelectConfirm.IsValid())
		BTN_CharSelectConfirm->SetIsEnabled(true);
}

void UB2LobbyInventory::SelectedWizard()
{
	//SetCharSelectPopupButtonState(ESlateVisibility::HitTestInvisible);
	SealBoxClass = PCClassToInt(EPCClass::EPC_Wizard);
	SetButtonLineVisible(EPCClass::EPC_Wizard);
	//SummonItem();

	if (BTN_CharSelectConfirm.IsValid())
		BTN_CharSelectConfirm->SetIsEnabled(true);
}

void UB2LobbyInventory::SelectedFighter()
{
	//SetCharSelectPopupButtonState(ESlateVisibility::HitTestInvisible);
	SealBoxClass = PCClassToInt(EPCClass::EPC_Fighter);
	SetButtonLineVisible(EPCClass::EPC_Fighter);
	//SummonItem();

	if (BTN_CharSelectConfirm.IsValid())
		BTN_CharSelectConfirm->SetIsEnabled(true);
}

void UB2LobbyInventory::SetButtonLineVisible(EPCClass InPCClass)
{
	if (CharacterButtonOnArr.Num() == static_cast<int32>(EPCClass::EPC_End)
		&& CharacterButtonOffArr.Num() == static_cast<int32>(EPCClass::EPC_End))
	{
		for (int32 i = 0; i < static_cast<int32>(EPCClass::EPC_End); ++i)
		{
			bool bSelectPc = InPCClass == static_cast<EPCClass>(i);

			CharacterButtonOnArr[i]->SetVisibility(bSelectPc ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
			CharacterButtonOffArr[i]->SetVisibility(bSelectPc ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

		}
	}
}

void UB2LobbyInventory::SetForcedBlockHeader(bool IsBlock)
{
	//UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
	//if (HeaderUI != nullptr)
	//	HeaderUI->SetForcedBlockHeader(IsBlock);
}