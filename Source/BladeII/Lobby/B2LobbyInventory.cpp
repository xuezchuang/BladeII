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
		// ���� InstanceUID �� �´ٰ� �ϴ��� Equipped ���� ���� ���¶�� valid ���� ���� ��.
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
		// ���� InstanceUID �� �´ٰ� �ϴ��� Equipped ���� ���� ���¶�� valid ���� ���� ��.
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
int32 UB2LobbyInventory::ItemLevelupIngredLimit = 20; // @TODO BII_SERVER �� ������ ���ε� �������� ���ų�..

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

	ResetAllIngredsList(); // �Ϲ����� ���� �÷ο��� �ʿ�� ���� �ǵ� �������� ���� Ȯ���� ����ָ� ���� ��.
	ResetEnhanceItemDatas();
	GhostItemOpIngreds.Empty();

	UnsubscribeEvents();

	Super::BeginDestroy();
}

void UB2LobbyInventory::StaticInit()
{
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		PCItems.Add(FInvenPCItemAccessor(IntToPCClass(PCI))); // �� PC ���� DataStore �� �����ϴ� ��ü�� ����� �ξ�� ��.
	}
}

void UB2LobbyInventory::InitLobbyInventory(class AB2LobbyGameMode* InLobbyGM)
{
	TransientLobbyGM = InLobbyGM;
	check(TransientLobbyGM); // ��ü ������ LobbyGameMode ������

	SubscribeEvents();
}

void UB2LobbyInventory::SubscribeEvents()
{
	if (bEventSubscribed)
		return;

	CAPTURE_UOBJECT(UB2LobbyInventory);

	// ���� ��û �̺�Ʈ��..
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

	// �������� �� ������ �̰��� �ƴ� ClientDataStore ���� ���� ó���� �� ����� ������ �̺�Ʈ
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
		// Backup�� ���ش�.
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
	// UI lobby inventory ������ ���� �� ó���� �͵�. Init ���ʹ� �ٸ���.
}

void UB2LobbyInventory::OnLobbyInventoryViewClose()
{
	// UI lobby inventory ������ ���� �� ó���� �͵�. Destroy ���ʹ� �ٸ���.

	QuitItemOpMode(false);
}

void UB2LobbyInventory::OnAllItemsAddedAtStore()
{ 
#if WITH_EDITOR
	// ��� ���� ��� �׽�Ʈ�� ���ϵ��� ���� ������ �������ִ� ��ȭâ�� ������.
	//CheckForGenerateDummyDataSuggestion(); �� �ƴ�. ���� �ʱ�ȭ�� ���� �Ǿ ���� ����. �ʿ��ϸ� �˾Ƽ� ġƮŰ �����ؼ� ������ �ϰ���..
#endif
	// �� ��� ������ �ڵ鸵�� ClientDataStore ���� �� ���·� ���⼭�� UI �� ������Ʈ ��ȣ�� �����ش�.
	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
}

void UB2LobbyInventory::OnItemsAddedAtStore(const FB2AddedItems& AddedItems)
{
	// �� ��� ������ �ڵ鸵�� ClientDataStore ���� �� ���·� ���⼭�� UI �� ������Ʈ ��ȣ�� �����ش�.
	// ���� ������ ������Ʈ ��ȣ�� ������ �߰��ϴ� �Ϳ� ���ؼ��� �ƴ�. ��� ������Ʈ.
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
	// ����, ���, ������ �Ǿ� �ִ°� ����.
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
	// ���, ������ �Ǿ� �ִ°� ����.
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
		// ���� �� ���, ������ �Ǿ� �ִ°� ����.
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
	// ���� �� ���, ������ �Ǿ� �ִ°� ����.
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
	// ���� �� ���, ������ �Ǿ� �ִ°� ����.
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
	// ������ ���⿡ �߰��ߴ� bRequestToServer �� �� �̻� �ȸ����� ����� DevOnlyRequestDummyItems �� ��� ���
	if (bRequestToServer)
	{
		DevOnlyRequestDummyItems(30, 1000100, 0.3f);
		return;
	}

	if (!bRequestToServer) // Ŭ���̾�Ʈ ���� ����� ���, �κ��丮 ��� Ȯ�� �����̴� ���� �͵� �����.
	{
		for (int32 IPI = 0; IPI < GetMaxPCClassNum(); ++IPI)
		{
			// ���� ClientDataStore �� �ִ� �����͸� �����ϴ� �����μ� ���� ��ɿ��� �̷� �ڵ带 ������ �ȵ�.
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
		// �������������� �׽�Ʈ ����̴� �׳� ����.
		EPCClass CurrPCClass = ItemAllowedPCClassEnum(IPI);

		// ���⼭ ItemRefID ���� ä���ִ� ������ �ܼ��� ���� ��� ���� �����Ϳ� ���� ����.

		// �Ʒ� ���� ������ �߿��� ������ �� ��� �������� �� �´�. ������ ��û�ϴ� ��Ȳ�̸� �ȵ� ��.
		TArray<int64> IDsToEquip;

		const int WeaponNum = FMath::RandRange(24, 36); // ��ü ����
		const int WeaponEquipIndex = FMath::RandRange(0, WeaponNum - 1); // �� �� ������ �� �ε���
		for (int32 NI = 0; NI < WeaponNum; ++NI)
		{
			const int32 StarGrade = (NI % 3) + 1;

			FB2Item NewWeapon;
			NewWeapon.InstanceUID = 1230000 + IPI * 1000 + NI; // ���Ǵ� ��Ȳ���� �ٸ� �������� ���� ���̹Ƿ� ���⼭���̶� ��ġ�� �ʰ� ������ �־���
			// �ڸ��� ���� �ǹ̰� ����. ������ ������ ���� ���������� ���纻��.
			NewWeapon.ItemRefID = 1000000 + 100000 * IPI + 0 + 100 * StarGrade;

			if (bRequestToServer) // ������ ��û�Ѵٸ� ���ݱ��� ������ �����. ���� �׽�Ʈ Ŀ�ǵ� ���.
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewWeapon.ItemRefID, StarGrade));
				continue;
			}

			// ���ϴ� ��¥ ���� ������ ����
			DevOnlyLoadItemClientTestData(NewWeapon);
			NewWeapon.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;
			NewWeapon.SetPrimaryPoint((float)StarGrade * 100.0f + FMath::FRandRange(0.0f, 100.0f));
			NewWeapon.Level = FMath::RandRange(1, NewWeapon.MaxLevel - 1); // ��ȭ ������ �������� �߿� ���� �� �ϳ��̴� �������� �־����� ��
			NewWeapon.Quality = FMath::RandRange(1, MAX_ITEM_QUALITY); // ǰ���� �������� ����.
			
			// Ư�� ���ǿ����� ��� ������ �ռ�, �±�, �ʿ� �׽�Ʈ�� ���� Ư�� ��ġ ����.
			if (FMath::RandRange(1, 10) == 1)
			{
				NewWeapon.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewWeapon.Level = NewWeapon.MaxLevel;
			}
			else if (FMath::RandRange(1, 10) == 1)
			{ // �ʿ� ���
				NewWeapon.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewWeapon.Level = 1;
			}
			else if (FMath::RandRange(1, 4) == 1)
			{
				NewWeapon.Level = NewWeapon.MaxLevel;
			}

			// �ɷ�ġ �ɼ� ������ �־��� ��
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

		// ����
		const int ProtectionNum = FMath::RandRange(36, 48); // ��ü ����
		const int ProtectionEquipIndex = FMath::RandRange(0, ProtectionNum - 1); // �� �� ������ �� �ε���
		for (int32 NI = 0; NI < ProtectionNum; ++NI)
		{
			const int32 ItemClassID = (NI % 3) + 1; // �� ����
			const int32 StarGrade = (NI % 3) + 1;

			FB2Item NewProtection;
			NewProtection.InstanceUID = 56340000 + IPI * 1000 + NI;
			NewProtection.ItemRefID = 1000000 + 100000 * IPI + 1000 * ItemClassID + 100 * StarGrade;

			if (bRequestToServer) // ������ ��û�Ѵٸ� ���ݱ��� ������ �����. ���� �׽�Ʈ Ŀ�ǵ� ���.
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewProtection.ItemRefID, StarGrade));
				continue;
			}

			DevOnlyLoadItemClientTestData(NewProtection);
			NewProtection.PrimaryPointType = EItemPrimaryPointType::EIPP_Defense;
			NewProtection.SetPrimaryPoint((float)StarGrade * 100.0f + FMath::FRandRange(0.0f, 100.0f));
			NewProtection.Level = FMath::RandRange(1, NewProtection.MaxLevel - 1); // ��ȭ ������ �������� �߿� ���� �� �ϳ��̴� �������� �־����� ��
			NewProtection.Quality = FMath::RandRange(1, MAX_ITEM_QUALITY); // ǰ���� �������� ����.
			
			// Ư�� ���ǿ����� ��� ������ �ռ�, �±�, �ʿ� �׽�Ʈ�� ���� Ư�� ��ġ ����.
			if (FMath::RandRange(1, 10) == 1)
			{
				NewProtection.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewProtection.Level = NewProtection.MaxLevel;
			}
			else if (FMath::RandRange(1, 10) == 1)
			{ // �ʿ� ���
				NewProtection.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewProtection.Level = 1;
			}
			else if (FMath::RandRange(1, 4) == 1)
			{
				NewProtection.Level = NewProtection.MaxLevel;
			}

			// �ɷ�ġ �ɼ� ������ �־��� ��
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

		// ��ű���
		const int AccessoryNum = FMath::RandRange(36, 48); // ��ü ����
		const int AccessoryEquipIndex = FMath::RandRange(0, AccessoryNum - 1); // �� �� ������ �� �ε���
		for (int32 NI = 0; NI < AccessoryNum; ++NI)
		{
			const int32 ItemClassID = (NI % 3) + 5; // ��ű� ����
			const int32 StarGrade = (NI % 3) + 1;

			FB2Item NewAccessory;
			NewAccessory.InstanceUID = 98230000 + IPI * 1000 + NI;
			NewAccessory.ItemRefID = 1000000 + 100000 * IPI + 1000 * ItemClassID + 100 * StarGrade;

			if (bRequestToServer) // ������ ��û�Ѵٸ� ���ݱ��� ������ �����. ���� �׽�Ʈ Ŀ�ǵ� ���.
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewAccessory.ItemRefID, StarGrade));
				continue;
			}

			DevOnlyLoadItemClientTestData(NewAccessory);
			NewAccessory.PrimaryPointType = EItemPrimaryPointType::EIPP_Health; // ��ű��� �⺻ �ɷ�ġ�� ü���ΰǰ�.
			NewAccessory.SetPrimaryPoint((float)StarGrade * 100.0f + FMath::FRandRange(0.0f, 100.0f));
			NewAccessory.Level = FMath::RandRange(1, NewAccessory.MaxLevel - 1); // ��ȭ ������ �������� �߿� ���� �� �ϳ��̴� �������� �־����� ��
			NewAccessory.Quality = FMath::RandRange(1, MAX_ITEM_QUALITY); // ǰ���� �������� ����.
			
			// Ư�� ���ǿ����� ��� ������ �ռ�, �±�, �ʿ� �׽�Ʈ�� ���� Ư�� ��ġ ����.
			if (FMath::RandRange(1, 10) == 1)
			{
				NewAccessory.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewAccessory.Level = NewAccessory.MaxLevel;
			}
			else if (FMath::RandRange(1, 10) == 1)
			{ // �ʿ� ���
				NewAccessory.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewAccessory.Level = 1;
			}
			else if (FMath::RandRange(1, 4) == 1)
			{
				NewAccessory.Level = NewAccessory.MaxLevel;
			}

			// �ɷ�ġ �ɼ� ������ �־��� ��
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

		// ���� �����۵�
		int32 WeaponEssenceNum = 4;
		for (int32 WEI = 0; WEI < WeaponEssenceNum; ++WEI)
		{
			const int32 StarGrade = WEI + 2;

			FB2Item NewWeaponEssence;
			NewWeaponEssence.InstanceUID = 7349579324000 + IPI * 1000 + WEI; // ���Ǵ� ��Ȳ���� �ٸ� �������� ���� ���̹Ƿ� ���⼭���̶� ��ġ�� �ʰ� ������ �־���
			// ���� �������� 2000000 ����
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

		// ������ �� ������ �������� �� �͵��� �����ϴµ� �������� ��û�ϴ� ��Ȳ�̸� �� ������ ó���� �Ƹ��� �ȵǾ����״� ���������� ���� ��
		if (!bRequestToServer)
		{
			// TODO ������ ������ (�ϴ� 0�� ������ ���)
			BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().EquipItems(IDsToEquip, true, 0);
		}
	}

	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
}
#endif


void UB2LobbyInventory::ReqForEquipItems(const TArray<int64>& UIDsToEquip)
{
	// TODO ������ ������
	auto ePCClass = GetInventorySelectedPCClass();
	int32 currentItemPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(ePCClass);

	data_trader::Retailer::GetInstance().RequestEquipItem(currentItemPresetId, UIDsToEquip);
}

void UB2LobbyInventory::ReqForUnEquipItems(const TArray<int64>& UIDsToUnEquip)
{
	// TODO ������ ������
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
	GhostItemOpIngreds.Empty(); // ����Ʈ ������� ���� UI ���⿡ ���� ���� ��� ������ ��½ ����
	GetAllSelectedItemOpIngreds(GhostItemOpIngreds);
	//ResetAllIngredsList(); -> ���⼭ ��� ��ũ ���ִ� �� �����ϱ� �ѵ� UI ���� ���� ��û�� �־ ���� �ʴ´�.

	// Request �� ������ ������ CachedItemOpTargetData �� ĳ���� ���´�. ���������� CachedItemOpTargetData �� ����ϸ� �ȵ�.
	GetItemOPTargetItemData(CachedItemOpTargetData, false); // ���⼭ true �� �ָ� CachedItemOpTargetData �� CachedItemOpTargetData �� �������� �ϰ��� ��
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
	{// �κ񿡼� �κ��丮�� �������� â ���� PC �� �⺻����. ���� �������� �⺻��
		return TransientLobbyGM->GetHeroMgmtModePCClass();
	}

	// GetLocalPlayerCharacter �� ����� �� �ִٸ� �ΰ��ӿ��� BladeIIPlayer �� GetCurrentPlayerClass �� ����� �� ����.
	// ���ϰ� �̰� ���Ǵ� ��Ȳ������ PCClass �� ���� ������ �� �� ���� �״� ������ ���� ������..

	return EPCClass::EPC_End;
}

FInvenPCItemAccessor* UB2LobbyInventory::GetPCItemAccessor(EPCClass InPCClass)
{
	int32 InvenPCIdx = GetInvenPCIndexFromPCClass(InPCClass);
	return (PCItems.IsValidIndex(InvenPCIdx)) ? &(PCItems[InvenPCIdx]) : NULL;
}

TMap<int64, FB2Item>* UB2LobbyInventory::GetStoredItemMapPtr(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	TMap<int64, FB2Item>* RetList = NULL;

	if (InItemInvenType == EItemInvenType::EIIVT_Consumables) // �Ҹ�ǰ�� ��� PC ���� ������.
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
	// UI ��� �ܼ� ����Ʈ�� ����ϱ� ���� TArray �� �־��ִ� �����
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
	{// InOutList �� element ���� InventoryType �� ��ġ�ؾ� ��.
		checkSlow(ThisItemCheck.InventoryType == InInvenType);
	}
#endif

	if (InInvenType != EItemInvenType::EIIVT_Consumables) 
	{
		// �κ��丮�� �����ϱ� ���� ����.
		// ��ȭ < ���� < ����� < ��� < (�Ͻ���)���� ��
		// ����� �䱸������ �ƴϾ����� �� ���ǵ��� ��� ���� ��츦 ���� ��Ż �������� ���̽���..
		struct FInvItemCombatPowerSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return A.Power > B.Power;
			}
		};
		InOutList.StableSort(FInvItemCombatPowerSorter()); // �̾����� �Ʒ� ���ÿ����� ���� ������ �����ϱ� ���� StableSort ��.

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
		
		// �׸��� ���� ���� �������� �Ͻ������� ���� �տ� �����־�� �Ѵٴµ�..
		struct FInvItemNewSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return (A.IsNew && !B.IsNew) ? true : false;
			}
		};
		InOutList.StableSort(FInvItemNewSorter());
		

		// ū Ÿ�Կ� ���� ���ĵ� �ϴµ� �����Կ� ���� ������ �ܿ� ���� �����۵� ������ ����. �̰� ���� �������� �ڿ� �־��ٱ..
		struct FInvItemTypeSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return (IsEquipmentItem(A) && !IsEquipmentItem(B)) ? true : false;
			}
		};
		InOutList.StableSort(FInvItemTypeSorter());

		//InstanceUID�� �������� ���޵� ������ Ʃ�丮��� �������� ���� ���߿� ���޵Ǳ� ������ �ش� �������� ����
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
	{ // �Ҹ�ǰ�� �� �ٸ� ��������, �������̴� �����̴� �̷� �� ������.. ��ü�� Ÿ�Կ� ���� ������.

		// �Ʒ��� �̷��� Ÿ�� ���ĵ� �׷����� ���� �⺻�� ���̺� ��ϵ� RefID ���̴�. 
		// Ÿ�Ժ� ���Ŀ����� ����/��/��ű� �� ��� ����Ǵ� ���Ŀ� ���� ������ �Ǿ�� �ϴµ� ��� ������ ������� ���ϰ� RefID �� �Ѵ�. 
		// ���� ���� ������ ���� RefID �� ������ �־�� �Ѵ�.
		struct FConsumablesTypeSorter_RefID
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return (A.ItemRefID < B.ItemRefID);
			}
		};
		InOutList.StableSort(FConsumablesTypeSorter_RefID());  // �̾����� �Ʒ� ���ÿ����� ���� ������ �����ϱ� ���� StableSort ��.

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

				//2���� ��� A sort value > 0�̰� B�� -1 �ΰ��
				//�Ѵ� �ִ� ��� A.SortValue > B.SortValue

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

		// IsEnable** �鿡�� UID üũ�� �ϱ� �Ұǵ�, Upgrade �� ��� �ڽ��� �����ְ� �Ǵ����� �𸣰ڴ�. �ϴ� ��� ���� �ڽ��� �� ��������.
		bRemoveCurrentCheck = (CurrCheck.InstanceUID == OpTarget.InstanceUID);

		switch (CurrentItemOpMode)
		{
		case ELobbyInvenItemOpMode::EIVIOP_Levelup:
			bRemoveCurrentCheck |= !IsEnableItemLevelupIngred(CurrCheck); // ���� ������ �͸� �����ֱ�.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Upgrade:
			bRemoveCurrentCheck |= !(IsItemUpgradeAllowed(CurrCheck) || IsItemAllowedCompositeAndSurpass(CurrCheck)); // �±��� ���� ���� Ư���ѵ�, �±� ���� �� ����, �±� Ÿ���� �� �� �ִ� �ֵ鸸 �����ش�.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Composition:
			bRemoveCurrentCheck |= !IsEnableComposeIngred(CurrCheck); // ���� ������ �͸� �����ֱ�.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Surpass:
			bRemoveCurrentCheck |= !IsEnableSurpassIngred(CurrCheck); // ���� ������ �͸� �����ֱ�.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
			bRemoveCurrentCheck |= !IsVisibilityEhanceIngred(CurrCheck); // ���� ������ �͸� �����ֱ�.
			break;			
		case ELobbyInvenItemOpMode::EIVIOP_EnhancementChange:
			bRemoveCurrentCheck |= !IsEnableEhanceChangeIngred(CurrCheck); // ���� ������ �͸� �����ֱ�.
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
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	for (int32 ITI = 0; ITI < (int32)EItemInvenType::EIIVT_End; ++ITI)
	{
		TArray<FB2Item> CurrList; 
		GetStoredItemList(CurrList, (EItemInvenType)ITI, FinalPCClass, false); // ���⼭ ������ ���� ������ ���� �� �ʿ䰡 ����.
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
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	TArray<FB2Item> MergedList;
	GetMergedItemList(MergedList, bIncludeEquipped, FinalPCClass); // �ϴ� filter ���� ���ǿ��� ��ü ������ ������ ���.

	//TArray ���� Resize �Ͼ�� Lambda ���� std::function�� �޸� ����
	std::list<std::function<bool(FB2Item)>> FIlters;
	// ���ʴ�� ���� �߰�
	FIlters.emplace_back(
		[&InvenTypeFilter](FB2Item CurrItem)
	{
		for (EItemInvenType CurrInvenType : InvenTypeFilter) // InvenTypeFilter �� �ִ� element �� ��� �ϳ��� ��ġ�ϸ� ����.
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
		for (int32 CurrStarGrade : StarGradeFilter) // StarGradeFilter �� �ִ� element �� ��� �ϳ��� ��ġ�ϸ� ����.
		{
			if (CurrItem.StarGrade == CurrStarGrade)
			{
				return true;
			}
		}
		return false;
	});
	
	// Lock �� ���� �ɷ����� ���� �ڵ��Ǹų� �ڵ���ȭ ���� ��ɿ��� ���.
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

	// �ʿ����(���) ����ó��
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
	
	OutResult = MergedList; // ��� ����Ʈ�� ����.
}

void UB2LobbyInventory::GetStoredItemListForEquipPlace(TArray<FB2Item>& OutResult, EItemEquipPlace InEquipPlace, EPCClass InPCClass /*= EPCClass::EPC_End*/)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	TArray<FB2Item> MergedList;
	GetMergedItemList(MergedList, false, FinalPCClass); // InEquipPlace �� ���� InvenType �� �ֱ� �ѵ�.. Ŭ�� �װ� �����ϴ� �Լ��� �δ� �͵� ���ϰ� �ؼ� �� �ϴ� MergedList ���� �Ŀ�..

	if (MergedList.Num() > 0)
	{
		OutResult = MergedList.FilterByPredicate( // InEquipPlace �� ��ġ�ϴ� �ɷθ� ��󳽴�.
			[&InEquipPlace](FB2Item CurrItem)
			{ // ����, �ʿ����(���) ����ó��
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
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	int32 RetSlotCount = BladeIIGameImpl::GetClientDataStore().GetCharacterInvenSlotCount(FinalPCClass, InItemInvenType);
	
	return RetSlotCount;
}

/* ĳ���� ������ �κ��丮 */
int32 UB2LobbyInventory::GetStoredItemNumInSharedSlot(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	if (InItemInvenType == EItemInvenType::EIIVT_Consumables)
	{
		return GetConsumablesList().Num(); // PCClass �� ����.
	}
	else if (InItemInvenType != EItemInvenType::EIIVT_End)
	{
		int32 InvenPCIdx = GetInvenPCIndexFromPCClass(FinalPCClass);
		if (InvenPCIdx >= 0 && InvenPCIdx < GetMaxPCClassNum())
		{
			FInvenPCItemAccessor& PCList = PCItems[InvenPCIdx];
			TMap<int64, FB2Item>* ItemMap = PCList.GetItemMapForInvenType(InItemInvenType);

			// �������� ��� ����.
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

/* ĳ���� ���� �κ��丮 */
int UB2LobbyInventory::GetStoredItemNumInUnitedSlot(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	if (InItemInvenType == EItemInvenType::EIIVT_Consumables)
	{
		return GetConsumablesList().Num(); // PCClass �� ����.
	}
	else if (InItemInvenType != EItemInvenType::EIIVT_End)
	{
		int32 InvenPCIdx = GetInvenPCIndexFromPCClass(FinalPCClass);
		if (InvenPCIdx >= 0 && InvenPCIdx < GetMaxPCClassNum())
		{
			FInvenPCItemAccessor& PCList = PCItems[InvenPCIdx];
			int32 ItemCount = PCList.GetAllItemMapCount();

			// �������� ��� ����.
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

		// �������� ��� ����.
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
	InNewItem.bNewlyAddedForEquipSuggest = true; // ���� �̰� ������ ��..

	return ReAddStoredItem(InNewItem, bForceAdd, InPCClass, true); // ��ɻ� ���⼭�� OptionalInsertIndex �� �ʿ���ڴ�.
}

bool UB2LobbyInventory::ReAddStoredItem(FB2Item& InNewItem, bool bForceAdd, EPCClass InPCClass, bool bUpdateUIDocImmediately /*= true*/)
{ // ���⿡ InPCClass ���ڰ� ���� �ʿ��ұ�. InNewItem.AllowedPCClass �� ����Ҽ���
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	if (InNewItem.InventoryType == EItemInvenType::EIIVT_End)
	{
		return false;
	}

	if (!bForceAdd && IsInventoryFull(InNewItem.InventoryType, FinalPCClass))
	{
		return false;
	}

	bool bRetAdded = false;
	// ���� ���������� ����Ʈ�� �����ͼ� ����.
	TMap<int64, FB2Item>* MapToAdd = GetStoredItemMapPtr(InNewItem.InventoryType, FinalPCClass);
	if (MapToAdd)
	{
		InNewItem.bCurrentlyEquipped = false; // ����� �ִ´ٴ� �� ��� ���������� �ʴ� ��.
		MapToAdd->Add(InNewItem.InstanceUID, InNewItem);
		bRetAdded = true;
	}

	if (bUpdateUIDocImmediately)
	{ // ���� UI Doc ������ ������Ʈ.. �� ���� �ʴ� ���� ReAdd �� ����� �ٸ� ����
		LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	}

	return bRetAdded;
}

bool UB2LobbyInventory::InternalIsInventoryFull(bool IsOver, EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	if (InItemInvenType == EItemInvenType::EIIVT_End)
	{
		return true;
	}

	// ���� Ÿ�Կ��� ������ ������ ������ ������ ���� ������ ����
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
	// Sort �� �Ƹ� �ǹ̰� ���� �� ������.. �ƴ� �Ҹ�ǰ �������� sort ������ ������ ó���ϰ� �� ����. -> ConsumingAmount �� �ϸ� �� ��.
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

	// 1000���� �ϳ��� ������.. �����ۺ��� �ٸ����� �𸣰�����..
	const int32 AllowedAmountPerSingleItem = 1000;
	int32 GenerateItemNum = ((InAmount + AllowedAmountPerSingleItem-1) / AllowedAmountPerSingleItem);
	TArray<FB2Item> GeneratedData;
	int32 RemainingAmount = InAmount;
	for (int32 GI = 0; GI < GenerateItemNum; ++GI)
	{
		int32 ThisAmount = FMath::Min(RemainingAmount, AllowedAmountPerSingleItem);
		RemainingAmount = FMath::Max(0, RemainingAmount - ThisAmount);
		FB2Item NewConsumableData;
		// UID ��ġ�� �ȵǴµ� ������ ġƮ����̴� ��
		NewConsumableData.InstanceUID = 438402842 + FMath::RandRange(1, INT_MAX) + FMath::RandRange(1, INT_MAX); 
		NewConsumableData.ItemRefID = InRefID;
		NewConsumableData.InventoryType = EItemInvenType::EIIVT_Consumables;
		NewConsumableData.ConsumingAmount = ThisAmount;

		GeneratedData.Add(NewConsumableData);
	}

	for (FB2Item& ThisToAdd : GeneratedData)
	{ // �������� �� �� ������ ������ �ִ� �ſ� ���� ���� ��ü�ؾ� �� ����.. �װ� ������ �������� ���� �ſ� ó���� �ǳ�..
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
	//������ New���� false�� ����
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
	// ���� �� ���, ������ �ִ°� ����.
	UB2LobbyInventory::GetFilteredItemList(AllTargetItems, false, false, false, InvenTypeFilter, StarGradeFilter);

	// ���� ���� �� ����� �Ǵ� �����۸� ���� ���..
	if(EnhanceTargetItem.StarGrade == StarGrade && AllTargetItems.Num() == 1)
		return 0;
	return AllTargetItems.Num();
}

bool UB2LobbyInventory::UpdateSingleItemData(int64 ItemInstanceUID, FB2Item& InDataToUpdate)
{
	FB2Item* FoundItemPtr = FindFromAllPtr(ItemInstanceUID);
	if (!FoundItemPtr)
		return false; // �ش� ������ ��ã��.

	// InDataToUpdate �� ��ǻ� �ɷ�ġ ���� �����͸� ����ϱ� ��������, �������� �������� ������ ��ġ�ؾ� ��.
	if (InDataToUpdate.InstanceUID != FoundItemPtr->InstanceUID || InDataToUpdate.ItemRefID != FoundItemPtr->ItemRefID || InDataToUpdate.ItemClass != FoundItemPtr->ItemClass
		|| InDataToUpdate.AllowedPCClass != FoundItemPtr->AllowedPCClass || InDataToUpdate.InventoryType != FoundItemPtr->InventoryType || InDataToUpdate.EquipPlace != FoundItemPtr->EquipPlace)
	{
		return false;
	}

	bool bWasEquipped = FoundItemPtr->bCurrentlyEquipped; // ���� ���µ� ���⼭ �������� �ƴ�. �̰� return false ���� ���� �ʴ´�.

	*FoundItemPtr = InDataToUpdate;
	FoundItemPtr->bCurrentlyEquipped = bWasEquipped; // �Ƹ��� �����������߰����� ��

	if (FoundItemPtr->bCurrentlyEquipped)
	{
		// ���� UI Doc ������ ������Ʈ. ������ ������ �Ϳ� ���ؼ��� ����.
		UB2UIDocEquip* DocEquip = UB2UIDocHelper::GetDocEquip(FoundItemPtr->AllowedPCClass, (int32)FoundItemPtr->EquipPlace);
		if (DocEquip)
		{
			DocEquip->UpdateData(InDataToUpdate);
		}
	}
	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Signal(ItemInstanceUID); // ����� �ϳ��� �� ������Ʈ. �߰��� ���Žÿ��� �̰� ������� �ʴ´�.

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
	//TArray<EPCClass> PCPartUpdateSignalClass; // �ܰ������� ����Ǵ� �������� ������ PC Ŭ������ ��� ����. �� Ŭ�������� ��ȣ�� ������ ����
	//TArray<EPCClass> AnyEquipStateChangedClass; // �ܰ� ���� ��Ʈ ����Ǵ� ��찡 �ƴ϶� ������ �ݿ��� �� �� ������ Ŭ���� ���� ����
	//bool bIsCostumeItem = false;

	//TArray<const FB2Item*> B2ItemPtrArray;
	//for (auto EquippedItemUID : EquipedItems)
	//{
	//	const FB2Item* EquippedItem = FindFromAllPtr(EquippedItemUID);
	//	if (EquippedItem != nullptr)
	//	{
	//		B2ItemPtrArray.Emplace(EquippedItem);

	//		if (GetPCPartForItemEquipPlace(EquippedItem->EquipPlace) != EPCPart::EPCPT_End)
	//		{ // 3D �޽� ������Ʈ �ñ׳��� ���� ���̴� ������ ���� PC ��Ʈ �� �ϳ��� �Ϳ� ����.
	//			if (ItemAllowedPCClassEnum(EquippedItem->AllowedPCClass) == EPCClass::EPC_End) {
	//				// EPC_End �� �����ص� ǥ�ð� �ȵǴ� �������̰ų� �� �ٵ� �ϴ� �𸣴� ��� Ŭ������ ������ ����.
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
	//		// ���� UI Doc ������ ������Ʈ.
	//		if (UB2UIDocEquip* DocEquip = UB2UIDocHelper::GetDocEquip(EquippedItem->AllowedPCClass, static_cast<int32>(EquippedItem->EquipPlace)))
	//			DocEquip->DoEquip(*EquippedItem);

	//		if (EquippedItem->IsNew)
	//		{
	//			SetChangeNewItemTofalse(EquippedItemUID);
	//		}
	//	}
	//}

	//// ���� ������ ���� ���� â�� ���� �ִٸ� ���� ��� �並 ���� ���̹Ƿ� �ݴ´�.
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);

	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();

	//for (EPCClass& CurrUpdateClass : PCPartUpdateSignalClass)
	//{ // �ش� Ŭ������ 3D �޽��� ���ο� ���� ���� �ݿ��ؼ� ������Ʈ
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

	////�޽� ������Ʈ ��, �ִϸ��̼��÷���.
	//for (auto EquippedItem : B2ItemPtrArray)
	//{
	//	EItemEquipPlace eItemEquipPlace = EquippedItem->EquipPlace;

	//	// ���� ���� UI effect ��
	//	LobbyPlayUIEquipEffectClass<EItemEquipPlace>::GetInstance().Signal(eItemEquipPlace);

	//	//�ڽ�Ƭ ������ ��� �ִϸ��̼� ��� ���� ����
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
	//TArray<EPCClass> PCPartUpdateSignalClass; // �ܰ������� ����Ǵ� �������� ������ PC Ŭ������ ��� ����. �� Ŭ�������� ��ȣ�� ������ ����
	//TArray<EPCClass> AnyEquipStateChangedClass; // �ܰ� ���� ��Ʈ ����Ǵ� ��찡 �ƴ϶� ������ �ݿ��� �� �� ������ Ŭ���� ���� ����
	//bool bIsCostumeItem = false;

	//for (auto UnequippedItemUID : ArrayUnEquipItemID)
	//{
	//	const FB2Item* UnequippedItem = FindFromAllPtr(UnequippedItemUID);
	//	if (UnequippedItem != nullptr)
	//	{
	//		if (GetPCPartForItemEquipPlace(UnequippedItem->EquipPlace) != EPCPart::EPCPT_End)
	//		{ // 3D �޽� ������Ʈ �ñ׳��� ���� ���̴� ������ ���� PC ��Ʈ �� �ϳ��� �Ϳ� ����.
	//			if (ItemAllowedPCClassEnum(UnequippedItem->AllowedPCClass) == EPCClass::EPC_End) {
	//				// EPC_End �� �����ص� ǥ�ð� �ȵǴ� �������̰ų� �� �ٵ� �ϴ� �𸣴� ��� Ŭ������ ������ ����.
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

	//// UnEquipItems!! ������ ����
	//BladeIIGameImpl::GetClientDataStore().UnEquipItems(ArrayUnEquipItemID, iPresetID);

	//for (EPCClass& CurrUpdateClass : AnyEquipStateChangedClass)
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetSetItemValue(CurrUpdateClass, AfterSetItemCounts[CurrUpdateClass]);

	//for (auto UnequippedItemUID : ArrayUnEquipItemID)
	//{
	//	if (const FB2Item* UnequippedItem = FindFromAllPtr(UnequippedItemUID))
	//	{
	//		// ���� UI Doc �����;�����Ʈ.
	//		if (UB2UIDocEquip* DocEquip = UB2UIDocHelper::GetDocEquip(UnequippedItem->AllowedPCClass, static_cast<int32>(UnequippedItem->EquipPlace)))
	//			DocEquip->RemoveEquip();
	//	}
	//}

	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();

	//for (EPCClass& CurrUpdateClass : PCPartUpdateSignalClass)
	//{ // �ش� Ŭ������ 3D �޽��� ���ο� ���� ���� �ݿ��ؼ� ������Ʈ
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
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);
	if (CurrPCItemAccessor)
	{
		return CurrPCItemAccessor->FindEquippedItem(OutFoundItem, InEquipPlace);
	}
	return false;
}

bool UB2LobbyInventory::GetEquippedCostumeAtPlace(FB2Item& OutFoundItem, ECostumeEquipPlace InEquipPlace, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);
	if (CurrPCItemAccessor)
	{
		return CurrPCItemAccessor->FindEquippedCostume(OutFoundItem, InEquipPlace);
	}
	return false;
}

void UB2LobbyInventory::GetAllEquippedItem(TArray<FB2Item>& OutList, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);

	if (CurrPCItemAccessor)
	{
		CurrPCItemAccessor->GetAllEquippedItems(OutList);
	}
}

void UB2LobbyInventory::GetAllEquippedCostumeItem(TArray<FB2Item>& OutList, EPCClass InPCClass /*= EPCClass::EPC_End*/)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);

	if (CurrPCItemAccessor)
	{
		CurrPCItemAccessor->GetAllEquippedCostumeItems(OutList);
	}
}

void UB2LobbyInventory::ConditionalCheckForBetterEquipSuggest()
{
	//bool bIgnoreSuggest = false;

	//// 1. �ݺ� ���� ���߿��� ���� ���� �����ۿ� ���� ���������� ����
	//// 2. Tutorial���߿��� �������� ����
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
			{ // �̹� CheckForBetterEquipSuggestInner ���� ���� ��� �־ ���� ��ȭ���ڸ� �����ٸ� ��ȭ���� �ߺ��ؼ� ���� �ʰ� ����. ���ʿ��� �ٽ� ���� call �� ��. 
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
	////{ // ���� ���� ������ �� �ƿ� ���ٸ� ������ ���� ���� �ְ�.. �� �� ���ɷ� ������ ����.
	////	return false; // �̷��� �ϸ� ���� �� �ϴ� �ɷ�.
	////}

	//TArray<FB2Item> StoredList;
	//GetStoredItemListForEquipPlace(StoredList, InEquipPlace, InPCClass);
	//
	//// bNewlyAddedForEquipSuggest üũ�� �ɷθ� �ٽ� ���͸�
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
	//struct FItemStarGradeSorter // ���������� PrimaryPoint �� ���� ������ Ȥ�ö� ������ �� ���� �� �⺻���� StarGrade �� �� �� �ְ� ������ �� �� ����..
	//{
	//	bool operator()(const FB2Item& A, const FB2Item& B) const
	//	{
	//		return A.StarGrade > B.StarGrade;
	//	}
	//};
	//NewlyAddedList.StableSort(FItemStarGradeSorter());
	//struct FPrimaryPointSorter // PrimaryPoint �� �� �� ������� ���ϱ� ���� ����.
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
	//	{ // PrimaryPoint ������ �����Ͽ����� ���� �տ��� ����ϸ� ��. ��, Type ���� �񱳸�.. �ϴ� �� ������..?
	//		if (!bHadEquipped || (CurrCadid.PrimaryPointType == EquippedOne.PrimaryPointType 
	//			&& (B2UIUtil::DifferenceItemPrimaryPoint(CurrCadid, EquippedOne) >= 1)))
	//		{
	//			DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_AcqItemEquipSuggest, FDJLegacySubpopupAddPanelInfo());

	//			check(TransientLobbyGM); // ������ �ٺ�.
	//			UB2AcqItemEquipSuggest* AcqItemEquipSuggestPopup = Cast<UB2AcqItemEquipSuggest>(TransientLobbyGM->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_AcqItemEquipSuggest));
	//							
	//			if (AcqItemEquipSuggestPopup)
	//			{
	//				AcqItemEquipSuggestPopup->UpdateData(bHadEquipped, EquippedOne, CurrCadid,
	//					// ���� ���� CheckForBetterEquipSuggest �� �ٽ� ���� �� �ֵ���.
	//					[this](){CheckForBetterEquipSuggest(); }
	//				);
	//			}

	//			bOpenedDialog = true;
	//			break;
	//		}
	//	}

	//	for (FB2Item& CurrNew : NewlyAddedList)
	//	{ // �� ���� �� �־��� ������ üũ�� �Ͽ����� �̹� ����Ʈ������ bNewlyAddedForEquipSuggest �� ��� reset
	//		CurrNew.bNewlyAddedForEquipSuggest = false;
	//		UpdateSingleItemData(CurrNew.InstanceUID, CurrNew);
	//	}
	//			
	//	return bOpenedDialog; // �̹��� ��ȭ���ڸ� �����ٸ� true �� �����ؼ� �����ܿ��� �ߺ��ؼ� ���� �ʰ� �Ѵ�. ������ üũ�� �� �� ����.
	//}

	return false;
}

void UB2LobbyInventory::IgnoreBetterEquipSuggest()
{
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{// �� ��� ������ ���鼭 bNewlyAddedForEquipSuggest ����..
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
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	for (int32 EQI = 0; EQI < (int32)EItemEquipPlace::EIEP_End; ++EQI)
	{ // �������� ���� ���� ���ؼ��� �˻���.
		FB2Item EquippedOne;
		const bool bHadEquipped = GetEquippedItemAtPlace(EquippedOne, static_cast<EItemEquipPlace>(EQI), FinalPCClass);

		// ���� ��ġ���� �� ���� ����Ʈ�� �ø� �� �ִ� ���� ã�´�.
		TArray<FB2Item> StoredListAtPlace;
		GetStoredItemListForEquipPlace(StoredListAtPlace, static_cast<EItemEquipPlace>(EQI), FinalPCClass);
		
		float BestScoreSoFar = -1.0f;
		int64 CurrentBestUID = 0;
		for (FB2Item& CurrStored : StoredListAtPlace)
		{
			if ((!bHadEquipped || (B2UIUtil::DifferenceItemPrimaryPoint(CurrStored, EquippedOne) >= 1)) && // ������ ���� ���ٸ� �ܼ��� �� �� �ɷ�.
				(!bHadEquipped || CurrStored.PrimaryPointType == EquippedOne.PrimaryPointType) &&
				CurrStored.GetPrimaryPoint() > BestScoreSoFar)
			{
				BestScoreSoFar = CurrStored.GetPrimaryPoint();
				CurrentBestUID = CurrStored.InstanceUID;
			}
		}

		if (CurrentBestUID > 0) // �̹� ��ġ�� ������ �ͺ��� (Ȥ�� ����ִ� ĭ��) �� ���� ���� ã��.
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
//			AllGathered += CurrCheck.GetPrimaryPoint(); // �⺻ prim point ����. ���⼭ �ɼǰ����� ī��Ʈ������ �ʴ´�. �⺻ PrimaryPointType �� InPrimPointType �� �ٸ� ��� ������ �Ʒ� �ɼǰ� ������ �ʿ��ϱ� ������.
//		}
//
//		// �ش� prim point �� �����ɼ��� �ִٸ� �װ͵� ���Ѵ�.
//		// AllGathered += FMath::Max(GetEffectiveItemOptionValue(AssociatedOption, CurrCheck), 0.0f); // �ܼ� ���� �ɼ�. ����. �ٵ� ������ ������ ��쵵 �ֳ�..?
//	}
//	// Wing �� ���⼭ �߰�.
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
//	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.
//
//	TArray<FB2Item> AllEquipped;
//	GetAllEquippedItem(AllEquipped, FinalPCClass);
//
//	// Wing �� �����۰� ������ �ý����̰� �ɼ� Ÿ���� ���� ���� ���⼭ ó��.
//	bool bFoundWingData = false;
//	FB2Wing AdditionalWingData;
//	bFoundWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(FinalPCClass, AdditionalWingData);
//
//	return GatherPrimPointFromList(InPrimPointType, AllEquipped, (bFoundWingData ? &AdditionalWingData : NULL));
//}

//void UB2LobbyInventory::GetAllEquippedPrimPointsLocalPlayer(TMap<EItemPrimaryPointType, float>& OutAllPrimPoints, EPCClass InPCClass)
//{
//	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.
//
//	for (int32 PTI = 0; PTI < (int32)EItemPrimaryPointType::EIPP_End; ++PTI)
//	{
//		float ThisTypePoint = GetEquippedPrimPointLocalPlayer((EItemPrimaryPointType)PTI, FinalPCClass);
//		if (ThisTypePoint > 0.0f) // 0 ���ϸ� ���� �������� ���� ��
//		{
//			OutAllPrimPoints.Add((EItemPrimaryPointType)PTI, ThisTypePoint);
//		}
//	}
//}

//float UB2LobbyInventory::GetEquippedOptionPointLocalPlayer(EItemOption InOptionToCheck, EPCClass InPCClass)
//{
//	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.
//
//	TArray<FB2Item> AllEquipped;
//	GetAllEquippedItem(AllEquipped, FinalPCClass);
//
//	// Wing �� �����۰� ������ �ý����̰� �ɼ� Ÿ���� ���� ���� ���⼭ ó��.
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
//	{ // ���� �ɼ��̵� ��� �ɼ��̵� ���.
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
//	// Wing �� �����۰� ������ �ý����̰� �ɼ� Ÿ���� ���� ���� ���⼭ ó��.
//	if (InWing)
//	{
//		const FWingPropOption* ThisPropOption = InWing->GetPropOptionPtr(InOptionToCheck); 
//		if (ThisPropOption && ThisPropOption->bIsOpen)
//		{
//			AllFoundRawValues.Add(ThisPropOption->RawOptionAmount);
//		}
//	}
//	
//	// AllFoundRawValues �� ������ GetEffectiveItemOptionValue �� �ѱ��. ���� ��쿡 ���� �⺻���� �ɼ� Ÿ�Կ� ���� ������ �� ��.
//	return GetEffectiveItemOptionValue(InOptionToCheck, AllFoundRawValues);
//}

/* �� ���������� �ϴ� ���ְ�.. Ȥ�� �ʿ������� Wing �����͵� ����ϵ���.
void UB2LobbyInventory::GetAllEquippedOptionPointsLocalPlayer(TMap<EItemOption, float>& OutAllOptions, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End ���� �� ���� ���õ� �ɷ�.

	TArray<FB2Item> AllEquipped;
	GetAllEquippedItem(AllEquipped, FinalPCClass);

	// ������ Option �� �߰� ����
	TMap<EItemOption, TArray<float>> IntermOptionMap;

	for (FB2Item& CurrEquip : AllEquipped)
	{ // ���� �ɼ��̵� ��� �ɼ��̵� ���.
		for (FItemOption& CurrOption : CurrEquip.IntrinsicOptions)
		{
			float NewValue = CurrOption.RawOptionAmount;
			TArray<float>* ExistingValueArray = IntermOptionMap.Find(CurrOption.OptionType);
			if (ExistingValueArray)
			{ // ���⼱ �ܼ��� ���� �ֱ⸸ �ϰ� ��� ���� �� �������� ������ �ٽ� ������ ��.
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
			{ // ���⼱ �ܼ��� ���� �ֱ⸸ �ϰ� ��� ���� �� �������� ������ �ٽ� ������ ��.
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
	{ // ��Ƴ��� �ɷ� �������� ����.
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

	//ResetAllIngredsList(); // Ȯ����..
	//ResetEnhanceItemDatas();

	//GhostItemOpIngreds.Empty();
	//CachedItemOpTargetData = FB2Item(); // ������ ��û ���� ���Ŀ��� ĳ�� ��.
	//OnCommitItemOpHeroMgmtPCBackup = EPCClass::EPC_End;
	//OnCommitItemOpInvenPageTabBackup = EItemInvenType::EIIVT_End;
	//CurrentItemOpMode = InMode;
	//ItemOpTargetItemID = TargetItemID;
	//bFreezeitemOpModeIngredSelection = false;

	//if (IsBackupRollBackInventoryTab && TransientLobbyGM && TransientLobbyGM->DJLegacy_GetLobbyUIManager())
	//	CustomBackupInvenPageTab = TransientLobbyGM->DJLegacy_GetLobbyUIManager()->GetInventoryTabSelection();

	//// ������ Popup widget �� UIP ó�� ��ġ�� ���� �г� ������ ������ش�.
	//UB2LobbyUI_InventoryMain* InventoryUI = TransientLobbyGM ? Cast<UB2LobbyUI_InventoryMain>(TransientLobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
	//UPanelWidget* PopupAddPanel = InventoryUI ? InventoryUI->GetItemDetailPopupParentPanel(true) : nullptr;
	//
	//// ��Ÿ ������ ����� ��� ���� ���µ�.

	//// Mode �� TargetID �� ���ú��� �ϰ� UI �� ����� ��.
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

	//	return; //��ü �˾��̶� �ڿ� �ٲ����ʿ����
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
	//		//��ȭ��ȯ
	//		PopUpUI->AddHandler(EUIItemChangePopupMode::Enhance, FItemChangePopupMode::CreateLambda([this]()
	//		{	this->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_EnhancementChange, ItemOpTargetItemID); }));

	//		//�ɼ� ����
	//		PopUpUI->AddHandler(EUIItemChangePopupMode::Option, FItemChangePopupMode::CreateLambda([TargetItemData]()
	//		{	UB2UIItemOptionChange* ItemOptionChangeWidget = UB2UIManager::GetInstance()->OpenUI<UB2UIItemOptionChange>(UIFName::ItemOptionChange);
	//		if (ItemOptionChangeWidget)
	//		{
	//			ItemOptionChangeWidget->SetTargetItem(TargetItemData);
	//		}
	//		}));
	//		
	//		//������ȯ
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
	//		//��� ����
	//		PopUpUI->AddHandler(EUIItemChangePopupMode::Quality, FItemChangePopupMode::CreateLambda([TargetItemData]()
	//		{
	//			DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_QualityChange, FDJLegacySubpopupAddPanelInfo());
	//			check(TransientLobbyGM); // ������ �ٺ�.
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
	//	// Note :	�������ʹ� ��ȭ ���(Cost)�����ؼ� ������ ��û�����ʰ�, ������ �޾Ƶξ��� �����͵����Ϳ��� �����ϵ��� ���� (2017-04)
	//	//			���� �͵� ����/Ŭ�� �۾��ڰ� ����� �ѹ��� �����ϸ� ���ڴµ�....
	//}


	//if (OpenInventoryType == EItemInvenType::EIIVT_End)
	//{
	//	// Ÿ�� �����۰� ���� Ÿ���� ������ ���� ����. 
	//	// QuitItemOpMode ������ ����� ������ �ϴµ� ���⼭ �Ǽ��� ��ü�� ��� �� Ǯ���־�� �Ѵ�.
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

	//// ������ ������ ���� ���� â�� ���� �ִٸ� �ݴ´�. �����Կ��� ��Ḧ ������ �ϱ� ������.
	//// ��ȯ���(��ü �˾� â)�� ��� �ݾ����� �ʴ´�
	//
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_StoredItemDetail);
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);
	//ChangeInvenItemOpModeClass<ELobbyInvenItemOpMode>::GetInstance().Signal(CurrentItemOpMode);
	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal(); // �κ��丮 ������ �� ������Ʈ �ÿ��� Target �������� ������ �ʰ� �� ��.
	//
}

void UB2LobbyInventory::QuitItemOpMode(bool bQuitFromOpSuccess)
{
//	// ��带 �����ϱ� ��, ���� Op ��� ���� ���Ķ�� ���޾Ƽ� Op ��带 ������ �� �����Ƿ� ���׷��̵� �� ������ ������ ���� ������ �д�.
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
//	GhostItemOpIngreds.Empty(); // GhostItemOpIngreds �� ���� ��� ó��
//	CachedItemOpTargetData = FB2Item();
//#if WITH_EDITOR
//	bItemOpProgressSceneTestMode = false;
//#endif
//
//	ResetAllIngredsList();
//	ResetEnhanceItemDatas();
//
//	// ���� ������ �� ������ ��ȭ ���� �޴����� ��� �ݴ´�. (�ٷ� �Ʒ����� �ٽ� ���� ���� ������ ��)
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemLevelUpMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemUpgradeMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemComposeMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemSurpassMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceChangeMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemHeroChangeMain);
//		
//	LobbyInvenHideItemOpModeGuideTextClass<>::GetInstance().Signal(); // ���̵� �ؽ�Ʈ�� ǥ�õǾ� ���� �� �����Ƿ� ����.
//
//	if (bQuitFromOpSuccess && // �׳� Ȯ���������� QuitItemOpMode �� ���ϴ� ��쵵 ����. ���� Op mode ���� �� �Ҹ��� ����� ó����.
//		TransientLobbyGM && TransientLobbyGM->IsValidObj() && TransientLobbyGM->IsHeroMgmtMode()) 
//	{
//		// ��ȭ ���� �� ���� ������ ���� popup �� �ƴ� ���� �������� �Ǿ� �����Ƿ� �κ��丮�� ���ƿ�.
//		// ������ �ϴ��� ���� ������ �ȵǴµ� �ʿ� ���������� ��� �̰� ����Ǹ� ���� �糪�� ��
//		// �������� ��尡 �ƴ϶�� �̹� DJLegacy UI page �� ���� ���·� ���� �κ�â���� ���� ���� ���̹Ƿ� �ٽ� �����ϸ� �ȵ�.
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
//		// LobbyGM �� ����� �� ���õ� ����. 
//		LobbyQuitItemOpDirectingViewClass<>::GetInstance().Signal();
//		
//		// �κ��丮 UI�� ���� ���µ� ���� ���� �����ϰ� �� ó��.
//		if (OnCommitItemOpHeroMgmtPCBackup != EPCClass::EPC_End)
//			TransientLobbyGM->SetHeroMgmtModePCSelection(OnCommitItemOpHeroMgmtPCBackup);
//
//		//if (OnCommitItemOpInvenPageTabBackup != EItemInvenType::EIIVT_End)
//		{// EnterItemOpMode �� �Ǽ��� ��� �� Ǯ���ִ� �͵� ����. ���� ���� OnCommitItemOpInvenPageTabBackup �� ����� �ٸ��� �� �־ �̰� bool ���ڸ� false �� �ؼ� �� �ҷ��ش�.
//			LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(OnCommitItemOpInvenPageTabBackup, false);
//		}
//
//		if (IsReturnTotemInventory)
//		{
//			// ���ۿ��� ���ƿ� ���¶�� �ٽ� â�� ����ֵ��� ����.
//			ReturnTotemInventoryClass<int64>::GetInstance().Signal(PrevSuccessTargetData.InstanceUID);
//		}
//		else
//		{
//			if (bGotPrevItemOpTarget)
//			{
//				// ��ȭ/�������� ��� �����ϴٸ� â�� ����ֵ��� ����.
//
//				if (JustFinishedOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup && IsItemLevelupAllowed(PrevSuccessTargetData))
//				{
//					EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_Levelup, PrevSuccessTargetData.InstanceUID);
//				}
//				else if (JustFinishedOpMode == ELobbyInvenItemOpMode::EIVIOP_Enhancement && IsItemEnhanceAllowed(PrevSuccessTargetData))
//				{
//					// ��ȭ�� ��� ���鵵 �̸� �־��ش�. 
//					EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_Enhancement, PrevSuccessTargetData.InstanceUID, EItemInvenType::EIIVT_Consumables);
//
//					if (PrevSelectedIngredItems.Num() > 0)
//					{
//						// 999���� ������ ������ InstanceUID�� �������ϼ� �����Ƿ� RefID�������� �ٽ� ����� ���ش�.
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
//				{ // ��Ÿ �ٸ� ����� �� ���� ���� â �ٽ� ���� �ɷ�. �ռ��̳� �±��� �ؾ� �� ��Ȳ�̶� �� �� �ϳ��� �����ؾ� �ϴ� �� ����â���� ��
//					UB2DynItemIcon::CreateLobbyItemDetailPopupCommon(TransientLobbyGM, PrevSuccessTargetData, PrevSuccessTargetData.bCurrentlyEquipped);
//				}
//			}
//		}
//	}
//	else
//	{	
//		// �Ǽ��� ��� �� Ǯ���ִ� �� � ��Ȳ������
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
	{ // Item OP req ���� ��ü�� �� ��Ȳ���� ������Ʈ �ϱ� ���� �� ������.
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
			if (FindFromAll(OutData, ItemOpTargetItemID)) // ��ȭ �� �±� ���� Ÿ���� ���� �������� ���� �ִ�.
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
	// ������������ ��� �������� ����.
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup)
	{
		return GetCurrentItemLevelUpGoldCost();
	}

	return CurrentBaseItemOpGoldCost;
}

bool UB2LobbyInventory::CanAffordCurrentItemOp() const
{ // �������� �κ��� ���� ��. ��ȭ �޴� ��� �̰� false �� ��� ���� ��ư Ŭ���� ���������� ����� ���� ���� �˾��� ��� ����.
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
		// �̰� Ŭ���̾�Ʈ �����. ���� ���������δ� �������� ��ȭ�� �ؼ� ������ �������� �̰� Ŭ���̾�Ʈ���� �� �ʿ䰡 �� ����.
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
		// �̰� Ŭ���̾�Ʈ �����. ���� ���������δ� �������� ��ȭ�� �ؼ� ������ �������� �̰� Ŭ���̾�Ʈ���� �� �ʿ䰡 �� ����.
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
	{ // �������� ��� ����ó��
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
		EnhanceTarget.InventoryType == IngredCandidate.InventoryType /*&& !IngredCandidate.bIsLocked*/); // ��� �͵� ��ᰡ �Ǵ� �� �ƴѵ�, �ּ��� ���� �õ��� �Ǿ�� ��. (�޽��� �ڽ��� �ߵ���.)
}

bool UB2LobbyInventory::SelectForIngred(int64 ItemInstanceUID, ELobbyInvenItemOpMode ItemOpMode, bool InForceMenuUpdate)
{
	//if (bFreezeitemOpModeIngredSelection || CurrentItemOpMode != ItemOpMode || ItemInstanceUID == ItemOpTargetItemID) // ���� Ÿ���� ingredient �� �Ǿ �ȵǰ���
	//	return false;

	//// ���� ��� �ƴ� �����Կ����� ã�´�.
	//FB2Item FoundItem;
	//if (!FindStoredItem(FoundItem, ItemInstanceUID))
	//	return false;

	//if (// ��庰 �մ��� ������� üũ
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup && !IsEnableItemLevelupIngred(FoundItem)) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Composition && !IsEnableComposeIngred(FoundItem)) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass && !IsEnableSurpassIngred(FoundItem)) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Enhancement && !IsEnableEhanceIngred(FoundItem) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_EnhancementChange && !IsEnableEhanceChangeIngred(FoundItem)))
	//	)
	//{
	//	return false;
	//}

	//// ���õȰ��� ������ Swap�Ұ����� �߰� ������ ���������� ����
	//switch (ItemOpMode)
	//{
	//	// Swap SelectItem
	//case ELobbyInvenItemOpMode::EIVIOP_Composition:
	//case ELobbyInvenItemOpMode::EIVIOP_Surpass:
	//case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
	//case ELobbyInvenItemOpMode::EIVIOP_EnhancementChange:
	//	{
	//		// ������ ������ ���� �����ش�.
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

	//// ������ ����
	//UpdateSingleItemData(ItemInstanceUID, FoundItem); // ���ο��� UI �� ������ ���� ��û�� �� ��.

	//if (InForceMenuUpdate)
	//{
	//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // ��ȭâ�� ���� ������Ʈ	
	//}
	return true;
}

void UB2LobbyInventory::UnSelectForIngred(int64 ItemInstanceUID, ELobbyInvenItemOpMode ItemOpMode)
{
	//if (bFreezeitemOpModeIngredSelection || CurrentItemOpMode != ItemOpMode)
	//	return;

	//FB2Item FoundItem;
	//// ���� ��� �ƴ� �����Կ����� ã�´�.
	//if (FindStoredItem(FoundItem, ItemInstanceUID))
	//{
	//	FoundItem.bSelectedAsIngred = false;

	//	// ����Ʈ���� ����
	//	for (int32 RI = 0; RI < PendingSelectItemOpIngreds.Num(); ++RI)
	//	{
	//		if (PendingSelectItemOpIngreds[RI].InstanceUID == ItemInstanceUID)
	//		{
	//			PendingSelectItemOpIngreds.RemoveAt(RI);
	//			break;
	//		}
	//	}

	//	// ������ ����
	//	UpdateSingleItemData(ItemInstanceUID, FoundItem);  // ���ο��� UI �� ������ ���� ��û�� �� ��.

	//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // ��ȭâ�� ���� ������Ʈ
	//}
}

void UB2LobbyInventory::UnSelectForIngred(const TArray<int64>& InInstanceUID, ELobbyInvenItemOpMode ItemOpMode)
{
	if (bFreezeitemOpModeIngredSelection || CurrentItemOpMode != ItemOpMode)
		return;

	for (int32 i = 0; i < InInstanceUID.Num(); ++i)
	{
		FB2Item FoundItem;
		// ���� ��� �ƴ� �����Կ����� ã�´�.
		if (FindStoredItem(FoundItem, InInstanceUID[i]))
		{
			FoundItem.bSelectedAsIngred = false;

			// ����Ʈ���� ����
			for (int32 RI = 0; RI < PendingSelectItemOpIngreds.Num(); ++RI)
			{
				if (PendingSelectItemOpIngreds[RI].InstanceUID == InInstanceUID[i])
				{
					PendingSelectItemOpIngreds.RemoveAt(RI);
					break;
				}
			}

			// ������ ����
			UpdateSingleItemData(InInstanceUID[i], FoundItem);  // ���ο��� UI �� ������ ���� ��û�� �� ��.
		}
	}
	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // ��ȭâ�� ���� ������Ʈ
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
	//PendingSelectItemOpIngreds.Empty(); // ����Ʈ�� ����.

	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{ // �̰� �� �������� ������ PendingSelectItemOpIngreds ���� ���� �ش� ID �� ã�Ƽ� �����ϴ� �� �غ� ����.. �װ� �� �������� ����
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

	//if (TransientLobbyGM) // �̰� �˻��ϴ� ������ BeginDestroy ������ �Ҹ� ���� UI �� ������Ʈ�� ���� ���ؼ���. �Ϲ����� ��δ� �ƴ����� ��ư ������.
	//{ // ���� UI Doc ������ ������Ʈ.
	//	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // ������â�� ���� �ִٸ�..
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
	return GetSharedConsumableAmountOfType(FItemRefIDHelper::GetEnhanceProtectionItemRefId()); // ��ȣ�� Ÿ�� �߰��� ���� ���� �ٽ�..
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
			// ������ ���ų� // �ʿ��̰ų�(����6��)
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
		//Ȳ�ݸ�� ����ó��
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
		
		// 11�� �̻��� ���� 11�� �̻����� �� �ʿ��ϰ�
		// ���ϴ� ������ 6�� ���� ���͸� �� �� �ֵ��� ���� �߰�
		uint16 Minimum_Surpass_Star_Grade = (SurpassTargetItem.StarGrade >= FItemGradeInfo::MINIMUM_ADVANCED_SURPASS_STAR_GRADE ?
			FItemGradeInfo::MINIMUM_ADVANCED_SURPASS_STAR_GRADE : FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE);		

		return (SurpassTargetItem.InstanceUID != Ingred.InstanceUID &&
			Ingred.StarGrade >= Minimum_Surpass_Star_Grade &&			// ���� �߰��Ͽ� ��ü
			SurpassTargetItem.ItemClass == Ingred.ItemClass && // Item Slot���� ����. ū �з����� �ƴ� ��ü Ŭ�������� ��ġ��.
			(FItemRefIDHelper::ExtractItemVariationFromRefID(SurpassTargetItem.ItemRefID) == FItemRefIDHelper::ExtractItemVariationFromRefID(Ingred.ItemRefID)) // ���������� ���� �迭 variation �̾��.
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

		// todo : ���Ŀ� ��ũ���� �������� �Ǹ� Ÿ������ ������ ����! 
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

		// todo : ���Ŀ� ��ũ���� �������� �Ǹ� Ÿ������ ������ ����! 
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
	//�ռ��� �������� ���ֱ� ���� � �����¿� �����Ǿ� �ִ��� üũ�Ѵ�
	//FB2Item NewItem;
	//NewItem = GET_TUPLE_DATA(FB2ResponseComposeItem::new_composed_item_index, ComposedItemInfo);
	//int64 MainItemInstanceUID = GET_TUPLE_DATA(FB2ResponseComposeItem::deleted_main_item_id_index, ComposedItemInfo);

	//FPCItemContainer &ItemContainer = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems();
	//int32 PrevPresetID = -1;
	//FB2Item* PrevMainItem = ItemContainer.FindItem(MainItemInstanceUID);
	//if(PrevMainItem)
	//	PrevPresetID = ItemContainer.GetContainsItemPresetID(IntToPCClass(PrevMainItem->AllowedPCClass), PrevMainItem->InstanceUID);

	//ItemOpTargetItemID = BladeIIGameImpl::GetClientDataStore().ComposeItem(ComposedItemInfo); // �ռ��� �������� �ƽθ� �ٲ��. ��� ���� �������� TargetItemID �� �ʿ��ϴ� ���� �ٲ� �ɷ� ����.

	////������ ��Ʈ�� �־��ٸ� ���������� �̹� ������ �����̹Ƿ� Ŭ�� ������Ʈ ���ش�.
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
	//// �����ʿ� ��ȭ/�ռ� �� Req ������ Response �� ��Ȳ.

	//// �κ��丮 UI ���������� ��� �����µ� ���� ������ ���ƿ� ���� ���� �� ������ ����� �д�.
	//OnCommitItemOpHeroMgmtPCBackup = TransientLobbyGM ? TransientLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	//UB2UIManager_Lobby* LobbyUIManager = TransientLobbyGM ? TransientLobbyGM->DJLegacy_GetLobbyUIManager() : NULL;
	//OnCommitItemOpInvenPageTabBackup = LobbyUIManager ? LobbyUIManager->GetInventoryTabSelection() : EItemInvenType::EIIVT_End;

	//ResetAllIngredsList();

	//// ��庰�� ������ ���� â�� �ݰ� ������ �����Ѵ�. ���� â���ʹ� pop-up �� �ƴ� ���� ��������. (���� ���⿡�� ���ƿ��� ���� ���� ������ PC �����̶� �ǵ��� ����� ���°�)
	//switch (InOpMode)
	//{
	//case ELobbyInvenItemOpMode::EIVIOP_Levelup:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemLevelUpMain);
	//	// ChangeLobbyUIPage ���� View ���� ���� �¾��ؾ� �Ѵ�. ���� UI Widget �ȿ��� ī�޶� �����ϴ� �� ����.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Levelup);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemLevelUpProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Upgrade:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemUpgradeMain);
	//	// ChangeLobbyUIPage ���� View ���� ���� �¾��ؾ� �Ѵ�. ���� UI Widget �ȿ��� ī�޶� �����ϴ� �� ����.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Upgrade);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemUpgradeProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Composition:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemComposeMain);
	//	// ChangeLobbyUIPage ���� View ���� ���� �¾��ؾ� �Ѵ�. ���� UI Widget �ȿ��� ī�޶� �����ϴ� �� ����.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Composition);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemComposeProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Surpass:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemSurpassMain);
	//	// ChangeLobbyUIPage ���� View ���� ���� �¾��ؾ� �Ѵ�. ���� UI Widget �ȿ��� ī�޶� �����ϴ� �� ����.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Surpass);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemSurpassProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceMain);
	//	// ChangeLobbyUIPage ���� View ���� ���� �¾��ؾ� �Ѵ�. ���� UI Widget �ȿ��� ī�޶� �����ϴ� �� ����.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Enhancement);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemEnhanceProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_TotemSmelting:
	//	// ChangeLobbyUIPage ���� View ���� ���� �¾��ؾ� �Ѵ�. ���� UI Widget �ȿ��� ī�޶� �����ϴ� �� ����.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_TotemSmelting);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_TotemSmeltingProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume:
	//	// ChangeLobbyUIPage ���� View ���� ���� �¾��ؾ� �Ѵ�. ���� UI Widget �ȿ��� ī�޶� �����ϴ� �� ����.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemLevelUpProgress);
	//	break;
	//}

	//LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(OnCommitItemOpHeroMgmtPCBackup); // ������ �ܰ��� �ٲ� �� �����Ƿ� ��� ���� ���µ� ������Ʈ��..
	//UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(OnCommitItemOpHeroMgmtPCBackup, false);
}

void UB2LobbyInventory::ItemUpgradeCostDelivered(const FB2GetItemUpgradeCost& UpgradeCost)
{
//	CurrentBaseItemOpGoldCost = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::need_gold_index, UpgradeCost);
//	CurrentUpgradeStoneCost = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::need_upgrade_stone_count_index, UpgradeCost);
//	// ��� ���س��� ����ϴ� üũ��..
//	int32 UpgradeStoneID = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::need_upgrade_stone_template_id_index, UpgradeCost);
//	b2network::B2ItemServerInfoPtr ServerNextUpgradeItem = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::next_item_index, UpgradeCost);
//	NextUpgradeItem = ServerNextUpgradeItem;
//
//#if !UE_BUILD_SHIPPING
//	FB2Item UpgradeTargetItem;
//	GetItemOPTargetItemData(UpgradeTargetItem, false);
//	// ����� �� ���� ����ε�..
//	//checkSlow(UpgradeStoneID == FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(UpgradeTargetItem));
//#endif
//	EnterItemUpgradeModeClass<int64>::GetInstance().Signal(BeforeUpgradeItem.InstanceUID);
//	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI �� ������Ʈ
}

void UB2LobbyInventory::ItemComposeCostDelivered(const FB2GetItemComposeCost& ComposeCost)
{
	//CurrentBaseItemOpGoldCost = GET_TUPLE_DATA(FB2ResponseItemComposeCost::need_gold_index, ComposeCost);

	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI �� ������Ʈ
}

void UB2LobbyInventory::ItemSurpassCostDelivered(const FB2GetItemSurpassCost& SurpassCost)
{
	//CurrentBaseItemOpGoldCost = GET_TUPLE_DATA(FB2ResponseItemSurpassCost::need_gold_index, SurpassCost);

	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI �� ������Ʈ
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

	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI �� ������Ʈ
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
	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // ��ȭâ�� ���� ������Ʈ	
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

	// SortedAutoItemLevelupList �� ���ǿ� ���� �ڵ������� ����..
	BeginReqAutoItemLevelupCostWithSortedList(SortedAutoItemLevelupList);
}

void UB2LobbyInventory::SortAutoLevelupItemList(const TArray<FB2Item>& AllBulkList, TArray<FAutoItemLevelupSingleSet>& OutSortedList)
{
	// ��� ���͸� ������ ���� ��Ƴ��� ����Ʈ���� � �� Ÿ������ �ϰ� � �� ���� ���� ���� ���ؼ� OutSortedList �� �ѱ��.

	// �ϴ� ���� Ÿ��/��ᰡ �� �� �ִ� ������ �������� ���´�. ���⿡ �߰��� ���� ��ޱ��� ����
	TArray<FAutoItemLevelupPossibleSet> AllPossibleSet;
	
	for (int32 BLI = 0; BLI < AllBulkList.Num(); ++BLI)
	{
		const FB2Item& ThisBulkOne = AllBulkList[BLI];
		bool bFoundExistingSet = false;

		BII_CHECK(!ThisBulkOne.bIsLocked); // ��� �ִٸ� ���ۿ� ���� ����Ʈ�� ���ͼ� �ȵ�.
		if (ThisBulkOne.bIsLocked){
			continue;
		}

		if (!IsEssenceItem(ThisBulkOne) && !IsItemLevelupAllowed(ThisBulkOne)){ // �ִ� ��ȭ�ѵ��� �����ߴٰų� �ؼ� ��ȭ ��� �ȵǴ� �ֵ��� UI ���� ���͸� �ȵǾ� ���� �� ������ ���⼭ �Ÿ���.
			continue;
		}

		for (int32 API = 0; API < AllPossibleSet.Num(); ++API)
		{
			FAutoItemLevelupPossibleSet& ThisPossibleSet = AllPossibleSet[API];
			if (ThisPossibleSet.ItemsList.Num() > 0)
			{ // ���� PossibleSet ���� ù��° ��Ҹ����� ���� �η��� ���� �� �ִ� ������ �˻��Ѵ�.
				if (
					// �ϳ��� target ���� �����ؼ� ��ᰡ �� �� �ִٸ� ���� ���� �ٸ� Ÿ���� ��ᵵ �� �� �ִ� ��
					IsPossibleItemLevelupIngred(ThisPossibleSet.ItemsList[0], ThisBulkOne) &&
					// �߰��� ���� ��޳��� ����.
					(ThisPossibleSet.ItemsList[0].StarGrade == ThisBulkOne.StarGrade) &&
					// ��� PC Class �� �̰� ����� UI �� �������ؼ� ����� ������ �ֵ��� ���ƾ� �ϴµ�.. Ȥ �𸣴� �˻��ؼ� ���� �� ����.
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
		{ // �������� �� ã�Ҵٸ� ������ ���� â��
			FAutoItemLevelupPossibleSet NewSet;
			if (!IsEssenceItem(ThisBulkOne))
				NewSet.ItemsList.Add(ThisBulkOne);
			else
				NewSet.EssenceItemList.Add(ThisBulkOne);
			AllPossibleSet.Add(NewSet);
		}
	}

	// �� PossibleSet ������ ���� ������ ����. ���� ������ �켱������ ��ȭ�� �Ŷ�.
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
	
	// ���������� 30���� �� ��Ÿ �ѵ��� �ɸ��� ������ �߶� ���� ����Ʈ�� ������.
	for (int32 API = 0; API < AllPossibleSet.Num(); ++API)
	{
		TArray<FB2Item> ThisPossibleSetList = AllPossibleSet[API].ItemsList;
		TArray<FB2Item>& ThisEssenceItemList = AllPossibleSet[API].EssenceItemList;

		if(ThisPossibleSetList.Num() == 0)
			continue;
		else if (ThisPossibleSetList.Num() == 1 && ThisEssenceItemList.Num() == 0)
			continue;

		//List ��
		ThisPossibleSetList.Append(ThisEssenceItemList);

		// �տ��� (���� ����) �켱������ Ÿ������ ��� �ڿ��� (���� ����) �켱������ ���� ��´�.
		int32 IdxF = 0; // from Front
		int32 IdxR = ThisPossibleSetList.Num() - 1; // from Rear
		for (; IdxF < IdxR; ++IdxF)
		{
			if(IsEssenceItem(ThisPossibleSetList[IdxF]))
				break;

			FAutoItemLevelupSingleSet NewFinalSet;
			FB2Item EnhancePreviewCheckTarget = ThisPossibleSetList[IdxF]; // Ÿ���� �տ�������
			NewFinalSet.TargetItemID = ThisPossibleSetList[IdxF].InstanceUID;
			
			TArray<FB2Item> EnhancePreviewCheckIngreds;

			for (; IdxR > IdxF; --IdxR)
			{
				FB2Item ThisPreviewCheckIngred = ThisPossibleSetList[IdxR]; // ���� �ڿ�������
				EnhancePreviewCheckIngreds.Add(ThisPreviewCheckIngred);
				NewFinalSet.IngredientsID.Add(ThisPossibleSetList[IdxR].InstanceUID);

				// ��ᰳ�� ����
				if (NewFinalSet.IngredientsID.Num() >= MAXINGREDIENTCOUNT && NewFinalSet.TargetItemID > 0 && NewFinalSet.IngredientsID.Num() > 0)
				{
					OutSortedList.Add(NewFinalSet);
					NewFinalSet.IngredientsID.Empty();
				}

				// ���� NewFinalSet ���� �ְ� ��� ������ ����ġ�� �����ϸ� IdxR �ϳ� ���ҽ�Ű�� ���⼱ �׸�. ���� IdxF �� �Ѿ����.
				// �ڵ� ��ȭ ����Ʈ���� ��� ���� ������ ���� �ʴ´�. ReqForSingleAutoItemLevelupList ���� ���� ������ Req ���� �� ���ڰ� �ٸ��� ��.
				// �ڵ� ��ȭ���� ��� ���� ������ ���� �ִ� ��ȭ ������ ���߱Ⱑ �ſ� �������.
				float ExpectedPostExp = 0.0f;
				int32 PreviewLevel = GetLevel_PreviewItemLevelup(ExpectedPostExp, EnhancePreviewCheckTarget, EnhancePreviewCheckIngreds, false);
				if (PreviewLevel >= EnhancePreviewCheckTarget.MaxLevel)
				{
					--IdxR;
					break;
				}
			}

			// ���� ����Ʈ�� ���Խ�Ű�� ���� ������ �˻��..
			// ��� ������ 10���� ��� ���� ó��
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

	if (CopiedAutoItemLevelupCostReqList.Num() > 0) // CopiedAutoItemLevelupCostReqList �� ������ �ϴ� �Ŷ� �̰� ������ �ȵ�.
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
	//{ // �� �䱸���׿� ���� ����� ���ڶ���� ���� ��ư Ŭ���� �� �� �����Ƿ� ����� �� �� ����. ��, �ȳ� �޽����� ����.
	//	ShortageMGR->PopupGoToShopForGoldMessage();
	//	// ����� ������ �ô��� ���ڶ� �ߴ������� UI �� ���� �ʵ���.
	//	EndAutoItemLevelup(true, false);
	//}
}

void UB2LobbyInventory::BeginReqAutoItemLevelupWithSortedList(const TArray<FAutoItemLevelupSingleSet>& InSortedList)
{
	CopiedAutoItemLevelupMainReqList.Empty();
	CopiedAutoItemLevelupMainReqList.Append(InSortedList);
	if (CopiedAutoItemLevelupMainReqList.Num() > 0) // CopiedAutoItemLevelupMainReqList �� ������ �ϴ� �Ŷ� �̰� ������ �ȵ�.
	{
		AutoItemLevelupPhase = EAutoItemLevelupPhase::AEPhase_RequestingEnhancement;
		ReqForSingleAutoItemLevelupList(); // ���� �ڵ� ��ȭ ��û ű����. �� ������ �˾Ƽ� EndAutoItemLevelup �� �Ҹ� ��. Cost ��û�� �ִٸ� ���⼭ ������ ���� �ʰ� �� ��.
	}
}

void UB2LobbyInventory::ReqForSingleAutoItemLevelupList()
{
	BII_CHECK(AutoItemLevelupPhase == EAutoItemLevelupPhase::AEPhase_RequestingEnhancement);
	BII_CHECK(CopiedAutoItemLevelupMainReqList.Num() > 0);

	// ���� �������δ� ��û�� ���� ������ �� �Ŀ��� ���� ��û�� ���� �� �����Ƿ� CopiedAutoItemLevelupMainReqList ���� �ϳ��� ��û�� �����Ѵ�.

	for (int32 SI = CopiedAutoItemLevelupMainReqList.Num() - 1; SI >= 0; --SI) // �޹�ȣ���� �ؼ�.
	{
		// ���⼭ for ���� ���� �� ���� �˻翡�� �������� ���� ����Ʈ�� ���� ��� ��ŵ�ϱ� ���ؼ���. ��û ��ü�� �ѹ��� �ϳ��� ������.
		FAutoItemLevelupSingleSet& ThisSet = CopiedAutoItemLevelupMainReqList[SI];
		if (ThisSet.TargetItemID <= 0 || ThisSet.IngredientsID.Num() == 0){
			CopiedAutoItemLevelupMainReqList.Pop();
			continue;
		}

		data_trader::Retailer::GetInstance().RequestLevelupItem(ThisSet.TargetItemID, ThisSet.IngredientsID, true); // �ڵ���ȭ������ ��ȭ ��û�� �Ϲ� ��ȭ�� �������� �������������� ���ڸ� �ٸ��� ��.
		break;
	}

	if (CopiedAutoItemLevelupMainReqList.Num() == 0) // �������� ���� �ƴ����� ����� �ʿ�� ����.
	{
		EndAutoItemLevelup(false, false);
	}
	else
	{
		// �� ��쿡�� �ݵ�� Response �� �´ٴ� ������ ������, ������ Ÿ�̸Ӷ� ��ġ�� ���°� ���� ��?
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

	//// GET_TUPLE_DATA �ؼ� ID �´����� Ȯ���� ����..

	//FB2Item JustEnhancedTarget;
	//JustEnhancedTarget = GET_TUPLE_DATA(FB2ResponseLevelupItem::main_item_index, EnhancedItemInfo);
	//FAutoItemLevelupSingleSet RearSet = CopiedAutoItemLevelupMainReqList.Pop();
	//BII_CHECK(JustEnhancedTarget.InstanceUID == RearSet.TargetItemID); // ������ ReqForSingleAutoItemLevelupList �� ��û�� ���� �Ϳ� ���� ������ �´��� üũ
	//
	//CachedAutoItemLevelupResultItems.Add(JustEnhancedTarget.InstanceUID, JustEnhancedTarget); // ��� UI �� �Ѹ� ������ ĳ��.
	//
	//// ������ ���� �ִ��� ���ο� ���� ��� ���� Ȥ�� ����.
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

	//// ���� ������ ���� �ڵ���ȭ ��û�� ���� ��쿡��..
	//if (!bEndOfCostRequestOnly)
	//{
	//	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//	// �ڵ� ��ȭ �˾��� ���� �ִٸ� ���⼭ �ݰ� ���â�� ����.
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelup);
	//	if (bEndOnOperationSuccess)
	//	{
	//		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelupResult, FDJLegacySubpopupAddPanelInfo());
	//	}
	//}

	//CachedAutoItemLevelupResultItems.Empty(); // ��� �ּ��� �ڵ���ȭ ���â �ʿ��� (���� ��) ����� �Ŀ� ����.
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
	// UI ������ �뵵��. ����Ʈ �����ϰ� ��� �������� �ϰ� �� ����.

	TArray<FAutoItemLevelupSingleSet> LocalSortedList;
	
	SortAutoLevelupItemList(AllBulkList, LocalSortedList);

	int32 LevelupCost = 0;

	for (const FAutoItemLevelupSingleSet& ThisSet : LocalSortedList)
	{
		// FAutoItemLevelupSingleSet �� UID �� ����־ �ٽ� ã�ƿ�.. �ٺ�����
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
	//	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Signal(Item.InstanceUID); // ���� ���� ���� ����â ������Ʈ�� ����..
	//}
	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal(); // ��� ���¿� ���� ���ĵ� �ٽ� �ؾ� �ϹǷ� ���� �����۸� ������Ʈ �ϴ� �� �ƴ϶� ��ü ������Ʈ �� �ش�.
}

void UB2LobbyInventory::UnlockedItems(const FB2UnlockedItems& UnlockedItems)
{
	BladeIIGameImpl::GetClientDataStore().UnLockItems(UnlockedItems);

	//GetB2ItemArray(FB2ResponseUnlockItem::unlocked_items_index, UnlockedItems);
	//for (auto Item : B2ItemArray)
	//{
	//	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Signal(Item.InstanceUID); // ���� ���� ���� ����â ������Ʈ�� ����..
	//}
	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal(); // ��� ���¿� ���� ���ĵ� �ٽ� �ؾ� �ϹǷ� ���� �����۸� ������Ʈ �ϴ� �� �ƴ϶� ��ü ������Ʈ �� �ش�.
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
	//� �������� �Ҳ��� ������ �����͸� ������ �� �ؾ� ��.
	//��ȯ�� ������ ID�� ������ ���� ����. �ؿ��� �Ȱ��� id�� �ް� �ʿ��� ���̺��� ��ȸ�ϵ��� �ؾ� ��
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
	//check(TransientLobbyGM); // ������ �ٺ�.
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
{ // ���� �뵵 �̿ܿ��� �ʿ��ϰ� �Ǹ� !UE_BUILD_SHIPPING �����ϰ� ����ϵ���.
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
	//	// RemainingNum �� �ϳ� ���� ä�� ���� ��û�� ����Ѵ�.
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
{ // ���� �뷱�� �׽�Ʈ ��. Ư�� ĳ������ Ư�� ������ �������� �ɼ� �ٲ�ġ��.
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
		{ // ���� �� �����ϴ� ��� ������ �ִ� �� ã�� ������Ʈ���� �õ�.
			for (int32 ROI = 0; ROI < RemainingOptionsToReplace.Num(); ++ROI)
			{
				FItemOption& ThisToReplace = RemainingOptionsToReplace[ROI];
				bool bFoundAndUpdated = false;
				for (FItemOption& ThisExisting : FoundEquipItem.IntrinsicOptions)
				{
					if (ThisExisting.OptionType == ThisToReplace.OptionType)
					{
						ThisExisting.RawOptionAmount = ThisToReplace.RawOptionAmount; // ����.
						bFoundAndUpdated = true;
						break;
					}
				}
				if (!bFoundAndUpdated) // �� ã�Ҵٸ� �����ɼ� �˻� �õ�
				{
					for (FItemOption& ThisExisting : FoundEquipItem.RandomOptions)
					{
						if (ThisExisting.OptionType == ThisToReplace.OptionType)
						{
							ThisExisting.RawOptionAmount = ThisToReplace.RawOptionAmount; // ����.
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

		// ���� ���� �͵��� ������ ���� �ɼ��̴� �����ɼ�, �����ɼ� �������� �ʰ� ������� ������ ������ ä���ֱ⸸ �Ѵ�.
		// �ɼǺ��� ������ ��ġ�� ���߿� ����̶� Ʋ���� ū�ϳ� �� ������ ���⿡ �ϵ��ڵ���. Ŭ���̾�Ʈ������ �ɼ� �����͸� �ޱ⸸ �ؼ� ��ġ ������ ���������� �ʴ´�.
		
		while (RemainingOptionsToReplace.Num() > 0 && FoundEquipItem.IntrinsicOptions.Num() < 4)
		{
			FItemOption ThisToReplace = RemainingOptionsToReplace[0]; // �տ�������..
			FoundEquipItem.IntrinsicOptions.Add(ThisToReplace); // �������� �ߺ��� ���ɼ��� ������ �ֱ�� ������ �װ� InOptionsToReplace �� �˾Ƽ� �� �ִ� �ɷ�.			
			RemainingOptionsToReplace.RemoveAt(0);
		}
		while (RemainingOptionsToReplace.Num() > 0 && FoundEquipItem.RandomOptions.Num() < 3)
		{ // ����������.
			FItemOption ThisToReplace = RemainingOptionsToReplace[0];
			FoundEquipItem.RandomOptions.Add(ThisToReplace);
			RemainingOptionsToReplace.RemoveAt(0);
		}

		UpdateSingleItemData(FoundEquipItem.InstanceUID, FoundEquipItem); // ���������� ����� �����͸� ����.
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
			FString::Printf(TEXT("[�ȳ�] �κ��丮�� ����ֽ��ϴ�.\n\"��(Yes)\"�� �����Ͻø� �׽�Ʈ �� �����۵��� ä���帳�ϴ�.\n\n"))
			)) == EAppReturnType::Yes)
		{
			data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddDemoItem"));
		}
		else
		{ // �ѹ� No �� ���������� ���� ���������� �� ����� �ʰ�.
			bSelectedNoForDummyDataSuggestion = true;
		}
	}
#endif
}

void UB2LobbyInventory::DevOnlyItemLevelupProgressSceneTest(int32 IngredNum)
{
	// �⺻���� �� ��ȯ ���� �� ������ �κ��丮 �������� �ƴϸ� �� �ȸ��� �� ����. 
	//if (TransientLobbyGM && TransientLobbyGM->IsValidObj() && CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None) // ������ ItemOp ��尡 �ƴ� ���°� ���� ��..
	//{
	//	bItemOpProgressSceneTestMode = true;

	//	// ��ȭ���⿡�� ItemOpTargetItemID �� �����غ��� �ҿ��� ����. �׽�Ʈ �Լ��� � �������� �ִ��� �����ؼ� ������ ���� ���� �븩�̰�
	//	// ������Ʈ ���� ĳ�̵Ǵ� Ÿ�� ������ �����͸� ����.
	//	CachedItemOpTargetData = FB2Item();
	//	CachedItemOpTargetData.ItemRefID = 1000100; // ������ ItemInfo �� �ִ� RefID �� �־��� �ʿ䰡 �ִ�.
	//	CachedItemOpTargetData.InstanceUID = 32423243; // UID �� ���� ���� �־��ֱ� �ؾ� ��.
	//	DevOnlyLoadItemClientTestData(CachedItemOpTargetData);
	//	CachedItemOpTargetData.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//	IngredNum = FMath::Clamp(IngredNum, 1, ItemLevelupIngredLimit);

	//	GhostItemOpIngreds.Empty(); // ��ȭ ���� â���� ����� ��� ������ ������.
	//	for (int32 GI = 0; GI < IngredNum; ++GI)
	//	{
	//		FB2Item NewIngred;
	//		NewIngred.ItemRefID = 1000100; // ������ ItemInfo �� �ִ� RefID �� �־��� �ʿ䰡 �ִ�.
	//		NewIngred.InstanceUID = 4820942398429 + GI;

	//		DevOnlyLoadItemClientTestData(NewIngred);
	//		NewIngred.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//		GhostItemOpIngreds.Add(NewIngred);
	//	}

	//	CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_Levelup;		
	//	// ChangeLobbyUIPage ���� View ���� ���� �¾��ؾ� �Ѵ�. ���� UI Widget �ȿ��� ī�޶� �����ϴ� �� ����.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Levelup);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemLevelUpProgress);
	//}
}

void UB2LobbyInventory::DevOnlyItemEnhanceProgressSceneTest(int32 IngredNum)
{
	// �⺻���� �� ��ȯ ���� �� ������ �κ��丮 �������� �ƴϸ� �� �ȸ��� �� ����. 
	//if (TransientLobbyGM && TransientLobbyGM->IsValidObj() && CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None) // ������ ItemOp ��尡 �ƴ� ���°� ���� ��..
	//{
	//	bItemOpProgressSceneTestMode = true;

	//	// ��ȭ���⿡�� ItemOpTargetItemID �� �����غ��� �ҿ��� ����. �׽�Ʈ �Լ��� � �������� �ִ��� �����ؼ� ������ ���� ���� �븩�̰�
	//	// ������Ʈ ���� ĳ�̵Ǵ� Ÿ�� ������ �����͸� ����.
	//	CachedItemOpTargetData = FB2Item();
	//	CachedItemOpTargetData.ItemRefID = 1000100; // ������ ItemInfo �� �ִ� RefID �� �־��� �ʿ䰡 �ִ�.
	//	CachedItemOpTargetData.InstanceUID = 32423243; // UID �� ���� ���� �־��ֱ� �ؾ� ��.

	//	DevOnlyLoadItemClientTestData(CachedItemOpTargetData);
	//	CachedItemOpTargetData.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//	IngredNum = FMath::Clamp(IngredNum, 1, ItemLevelupIngredLimit);

	//	GhostItemOpIngreds.Empty(); // ��ȭ ���� â���� ����� ��� ������ ������.
	//	for (int32 GI = 0; GI < IngredNum; ++GI)
	//	{
	//		FB2Item NewIngred;
	//		NewIngred.ItemRefID = 1000100; // ������ ItemInfo �� �ִ� RefID �� �־��� �ʿ䰡 �ִ�.
	//		NewIngred.InstanceUID = 4820942398429 + GI;

	//		DevOnlyLoadItemClientTestData(NewIngred);
	//		NewIngred.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//		GhostItemOpIngreds.Add(NewIngred);
	//	}

	//	CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_Enhancement;
	//	// ChangeLobbyUIPage ���� View ���� ���� �¾��ؾ� �Ѵ�. ���� UI Widget �ȿ��� ī�޶� �����ϴ� �� ����.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Enhancement);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemEnhanceProgress);
	//}
}

void UB2LobbyInventory::DevOnlyItemCUSProgressSceneTest(ELobbyInvenItemOpMode InOpMode)
{	
	//if (InOpMode == ELobbyInvenItemOpMode::EIVIOP_None || InOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup){
	//	return; // ��ȭ�� ���⼭ ó�� �� ��.
	//}

	//// �⺻���� �� ��ȯ ���� �� ������ �κ��丮 �������� �ƴϸ� �� �ȸ��� �� ����. 
	//if (TransientLobbyGM && TransientLobbyGM->IsValidObj() && CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None) // ������ ItemOp ��尡 �ƴ� ���°� ���� ��..
	//{
	//	bItemOpProgressSceneTestMode = true;

	//	// Ÿ�� ������ ������.
	//	CachedItemOpTargetData = FB2Item();
	//	// ������ ItemInfo �� �ִ� RefID �� �־��� �ʿ䰡 �ִ�. �ʿ��̸� 6�� ��� ���δٰ�
	//	CachedItemOpTargetData.ItemRefID = InOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass ? 1000600 : 1000100; 
	//	CachedItemOpTargetData.InstanceUID = 32423243; // UID �� ���� ���� �־��ֱ� �ؾ� ��.
	//	DevOnlyLoadItemClientTestData(CachedItemOpTargetData);
	//	CachedItemOpTargetData.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;
	//	CachedItemOpTargetData.Level = 30; // �ռ��±� ���� Ư���� Level �� ������� ��.

	//	GhostItemOpIngreds.Empty();
	//	if (InOpMode == ELobbyInvenItemOpMode::EIVIOP_Composition || InOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass)
	//	{ // ��� ������ �����͸� �ʿ�� �ϴ� ���
	//		FB2Item NewIngred;
	//		// ������ ItemInfo �� �ִ� RefID �� �־��� �ʿ䰡 �ִ�. �ʿ��̸� 6�� ��� ���δٰ�
	//		NewIngred.ItemRefID = InOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass ? 1000600 : 1000100;
	//		NewIngred.InstanceUID = 4820942398429;
	//		DevOnlyLoadItemClientTestData(NewIngred);
	//		NewIngred.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;
	//		NewIngred.Level = 30; // ���������� ��ᵵ 30���� ������
	//		GhostItemOpIngreds.Add(NewIngred);
	//	}

	//	CurrentItemOpMode = InOpMode;

	//	// ChangeLobbyUIPage ���� View ���� ���� �¾��ؾ� �Ѵ�. ���� UI Widget �ȿ��� ī�޶� �����ϴ� �� ����.
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
	//	TArray<EPCClass> PCPartUpdateSignalClass; // �ܰ������� ����Ǵ� �������� ������ PC Ŭ������ ��� ����. �� Ŭ�������� ��ȣ�� ������ ����
	//	TArray<EPCClass> AnyEquipStateChangedClass; // �ܰ� ���� ��Ʈ ����Ǵ� ��찡 �ƴ϶� ������ �ݿ��� �� �� ������ Ŭ���� ���� ����
	//					
	//	//  UIDoc ����
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
	//	GetMergedItemList(MergedList, true, InPCClass); // �ϴ� filter ���� ���ǿ��� ��ü ������ ������ ���.

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

	//// �ȳ��� �켱���� => ���� > �߰�/��ü
	//// 1. ��Ʈȿ���� �߰�/�����Ȱ� �ִ� ��� => ��Ʈȿ�� ����
	//// 2. ��Ʈȿ�� ��/���Ȱ� �ִ� ��� => ��Ʈȿ�� ����
	//// 3. ��Ʈȿ���� �߰��Ȱ� �ִ� ��� => ��Ʈȿ�� �߰�
	//// 4. ��Ʈȿ���� �����Ȱ� �ִ� ��� => ��Ʈȿ�� ����

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
	////3D ���� �ִ� (�ƴ� �׳� ��ƼŬ �ý����� ����ϴ� ���� ��
	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::SealBoxResult);
	//UB2UISealBoxResult* UISealBoxResult = UB2UIManager::GetInstance()->OpenUI<UB2UISealBoxResult>(UIFName::SealBoxResult);
	//if (UISealBoxResult)
	//{
	//	UISealBoxResult->SetRewardDataManually(UnsealBoxPtr, SealBoxClass);
	//	UISealBoxResult->BindDocAuto();
	//}
	//*/
	//
	////���ۼҶ� ������ ������� �����ϴ� ��
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

//SummonItem �κп��� �����ؼ� ������ �κ�. �ؽ�Ʈ�� ��ư ����, ���� ǥ�� �κп� ������ ������ ����.
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
//			// �� ������ ������ Widget part Ŭ������ �Ǿ�� �� �� ������ ���� �������Ʈ�� �ڵ��� �Ǿ� ����.
//			// ��Ʈ���� �������°� ����� ����.. �Ѥ�
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
//			// ĳ���� enum ������ ��ġ..?
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

	//������ ������ ���� ��û�ϴ� ��
	//�ѹ��� � ����� ������ ��ȹ�� ��� �ѹ��� 1���� ������ �ϵ��ڵ� ��
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