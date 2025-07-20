// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyInventory.h"
//#include "BladeII.h"

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
		// 汲飞 InstanceUID 啊 嘎促绊 窍歹扼档 Equipped 登瘤 臼篮 惑怕扼搁 valid 窍瘤 臼阑 巴.
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
		// 汲飞 InstanceUID 啊 嘎促绊 窍歹扼档 Equipped 登瘤 臼篮 惑怕扼搁 valid 窍瘤 臼阑 巴.
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
int32 UB2LobbyInventory::ItemLevelupIngredLimit = 20; // @TODO BII_SERVER 傲 沥秦柳 蔼牢单 辑滚俊辑 坷芭唱..

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

	ResetAllIngredsList(); // 老馆利牢 霸烙 敲肺快扼搁 鞘夸绰 绝阑 扒单 俊叼磐狼 版快殿 犬角洒 厚况林搁 亮阑 巴.
	ResetEnhanceItemDatas();
	GhostItemOpIngreds.Empty();

	UnsubscribeEvents();

	Super::BeginDestroy();
}

void UB2LobbyInventory::StaticInit()
{
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		PCItems.Add(FInvenPCItemAccessor(IntToPCClass(PCI))); // 阿 PC 喊肺 DataStore 俊 立辟窍绰 按眉甫 父甸绢 滴绢具 窃.
	}
}

void UB2LobbyInventory::InitLobbyInventory(class AB2LobbyGameMode* InLobbyGM)
{
	TransientLobbyGM = InLobbyGM;
	check(TransientLobbyGM); // 按眉 积己篮 LobbyGameMode 俊辑父

	SubscribeEvents();
}

void UB2LobbyInventory::SubscribeEvents()
{
	if (bEventSubscribed)
		return;

	CAPTURE_UOBJECT(UB2LobbyInventory);

	// 辑滚 夸没 捞亥飘甸..
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

	// 辑滚俊辑 柯 览翠阑 捞镑捞 酒囱 ClientDataStore 俊辑 流立 贸府且 锭 咯扁肺 焊郴绰 捞亥飘
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
		// Backup阑 秦霖促.
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
	// UI lobby inventory 其捞瘤 坷锹 矫 贸府且 巴甸. Init 锭客绰 促福促.
}

void UB2LobbyInventory::OnLobbyInventoryViewClose()
{
	// UI lobby inventory 其捞瘤 摧阑 矫 贸府且 巴甸. Destroy 锭客绰 促福促.

	QuitItemOpMode(false);
}

void UB2LobbyInventory::OnAllItemsAddedAtStore()
{ 
#if WITH_EDITOR
	// 厚绢 乐阑 版快 抛胶飘俊 祈窍档废 歹固 单捞磐 积己秦林绰 措拳芒阑 凯绢淋.
	//CheckForGenerateDummyDataSuggestion(); 酒 灯促. 辑滚 檬扁拳啊 磊林 登绢辑 篮辟 阂祈. 鞘夸窍搁 舅酒辑 摹飘虐 巩狼秦辑 静带瘤 窍摆瘤..
#endif
	// 捞 版快 单捞磐 勤甸傅篮 ClientDataStore 俊辑 等 惑怕肺 咯扁辑绰 UI 率 诀单捞飘 脚龋父 朝妨霖促.
	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
}

void UB2LobbyInventory::OnItemsAddedAtStore(const FB2AddedItems& AddedItems)
{
	// 捞 版快 单捞磐 勤甸傅篮 ClientDataStore 俊辑 等 惑怕肺 咯扁辑绰 UI 率 诀单捞飘 脚龋父 朝妨霖促.
	// 俺喊 酒捞袍 诀单捞飘 脚龋绰 乐栏唱 眠啊窍绰 巴俊 措秦辑绰 酒丛. 葛滴 诀单捞飘.
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
	// 厘馒, 泪陛, 橇府悸 登绢 乐绰芭 力寇.
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
	// 泪陛, 橇府悸 登绢 乐绰芭 力寇.
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
		// 厘馒 棺 泪陛, 橇府悸 登绢 乐绰芭 力寇.
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
	// 厘馒 棺 泪陛, 橇府悸 登绢 乐绰芭 力寇.
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
	// 厘馒 棺 泪陛, 橇府悸 登绢 乐绰芭 力寇.
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
	// 扁粮俊 咯扁俊 眠啊沁带 bRequestToServer 啊 歹 捞惑 救冈洒绰 包拌肺 DevOnlyRequestDummyItems 甫 措脚 荤侩
	if (bRequestToServer)
	{
		DevOnlyRequestDummyItems(30, 1000100, 0.3f);
		return;
	}

	if (!bRequestToServer) // 努扼捞攫飘 傈侩 扁瓷狼 版快, 牢亥配府 扁瓷 犬牢 瞒盔捞聪 扁粮 巴甸 厚快磊.
	{
		for (int32 IPI = 0; IPI < GetMaxPCClassNum(); ++IPI)
		{
			// 角力 ClientDataStore 俊 乐绰 单捞磐甫 力芭窍绰 巴栏肺辑 角力 扁瓷俊绰 捞繁 内靛甫 持栏搁 救凳.
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
		// 备龙备龙窍瘤父 抛胶飘 扁瓷捞聪 弊成 埃促.
		EPCClass CurrPCClass = ItemAllowedPCClassEnum(IPI);

		// 咯扁辑 ItemRefID 箭磊 盲况持绰 肺流篮 窜鉴洒 备泅 寸矫 歹固 单捞磐俊 嘎冕 巴烙.

		// 酒贰 歹且 酒捞袍 吝俊辑 厘馒且 芭 割俺甫 公累困肺 哗 柯促. 辑滚俊 夸没窍绰 惑炔捞搁 救瞪 巴.
		TArray<int64> IDsToEquip;

		const int WeaponNum = FMath::RandRange(24, 36); // 傈眉 俺荐
		const int WeaponEquipIndex = FMath::RandRange(0, WeaponNum - 1); // 弊 吝 厘馒且 芭 牢郸胶
		for (int32 NI = 0; NI < WeaponNum; ++NI)
		{
			const int32 StarGrade = (NI % 3) + 1;

			FB2Item NewWeapon;
			NewWeapon.InstanceUID = 1230000 + IPI * 1000 + NI; // 荤侩登绰 惑炔俊辑 促弗 酒捞袍篮 绝阑 巴捞骨肺 咯扁辑父捞扼档 般摹瘤 臼霸 利寸洒 持绢淋
			// 磊府荐 喊肺 狼固啊 乐澜. 绢瞒乔 瘤陛篮 歹固 单捞磐瘤父 嘎苗夯促.
			NewWeapon.ItemRefID = 1000000 + 100000 * IPI + 0 + 100 * StarGrade;

			if (bRequestToServer) // 辑滚俊 夸没茄促搁 瘤陛鳖瘤 沥焊肺 面盒窃. 辑滚 抛胶飘 目盖靛 荤侩.
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewWeapon.ItemRefID, StarGrade));
				continue;
			}

			// 捞窍绰 柳楼 啊扼 单捞磐 偿偿
			DevOnlyLoadItemClientTestData(NewWeapon);
			NewWeapon.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;
			NewWeapon.SetPrimaryPoint((float)StarGrade * 100.0f + FMath::FRandRange(0.0f, 100.0f));
			NewWeapon.Level = FMath::RandRange(1, NewWeapon.MaxLevel - 1); // 碍拳 饭骇篮 焊咯瘤绰 吝夸 沥焊 吝 窍唱捞聪 公累困肺 持绢林磊 せ
			NewWeapon.Quality = FMath::RandRange(1, MAX_ITEM_QUALITY); // 前龙档 焊咯瘤绰 沥焊.
			
			// 漂沥 炼扒俊辑父 荤侩 啊瓷茄 钦己, 铰鞭, 檬岿 抛胶飘甫 困秦 漂喊 荐摹 技泼.
			if (FMath::RandRange(1, 10) == 1)
			{
				NewWeapon.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewWeapon.Level = NewWeapon.MaxLevel;
			}
			else if (FMath::RandRange(1, 10) == 1)
			{ // 檬岿 犁丰
				NewWeapon.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewWeapon.Level = 1;
			}
			else if (FMath::RandRange(1, 4) == 1)
			{
				NewWeapon.Level = NewWeapon.MaxLevel;
			}

			// 瓷仿摹 可记 利寸洒 持绢淋 せ
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

		// 规绢备甸
		const int ProtectionNum = FMath::RandRange(36, 48); // 傈眉 俺荐
		const int ProtectionEquipIndex = FMath::RandRange(0, ProtectionNum - 1); // 弊 吝 厘馒且 芭 牢郸胶
		for (int32 NI = 0; NI < ProtectionNum; ++NI)
		{
			const int32 ItemClassID = (NI % 3) + 1; // 规绢备 辆幅
			const int32 StarGrade = (NI % 3) + 1;

			FB2Item NewProtection;
			NewProtection.InstanceUID = 56340000 + IPI * 1000 + NI;
			NewProtection.ItemRefID = 1000000 + 100000 * IPI + 1000 * ItemClassID + 100 * StarGrade;

			if (bRequestToServer) // 辑滚俊 夸没茄促搁 瘤陛鳖瘤 沥焊肺 面盒窃. 辑滚 抛胶飘 目盖靛 荤侩.
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewProtection.ItemRefID, StarGrade));
				continue;
			}

			DevOnlyLoadItemClientTestData(NewProtection);
			NewProtection.PrimaryPointType = EItemPrimaryPointType::EIPP_Defense;
			NewProtection.SetPrimaryPoint((float)StarGrade * 100.0f + FMath::FRandRange(0.0f, 100.0f));
			NewProtection.Level = FMath::RandRange(1, NewProtection.MaxLevel - 1); // 碍拳 饭骇篮 焊咯瘤绰 吝夸 沥焊 吝 窍唱捞聪 公累困肺 持绢林磊 せ
			NewProtection.Quality = FMath::RandRange(1, MAX_ITEM_QUALITY); // 前龙档 焊咯瘤绰 沥焊.
			
			// 漂沥 炼扒俊辑父 荤侩 啊瓷茄 钦己, 铰鞭, 檬岿 抛胶飘甫 困秦 漂喊 荐摹 技泼.
			if (FMath::RandRange(1, 10) == 1)
			{
				NewProtection.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewProtection.Level = NewProtection.MaxLevel;
			}
			else if (FMath::RandRange(1, 10) == 1)
			{ // 檬岿 犁丰
				NewProtection.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewProtection.Level = 1;
			}
			else if (FMath::RandRange(1, 4) == 1)
			{
				NewProtection.Level = NewProtection.MaxLevel;
			}

			// 瓷仿摹 可记 利寸洒 持绢淋 せ
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

		// 厘脚备甸
		const int AccessoryNum = FMath::RandRange(36, 48); // 傈眉 俺荐
		const int AccessoryEquipIndex = FMath::RandRange(0, AccessoryNum - 1); // 弊 吝 厘馒且 芭 牢郸胶
		for (int32 NI = 0; NI < AccessoryNum; ++NI)
		{
			const int32 ItemClassID = (NI % 3) + 5; // 厘脚备 辆幅
			const int32 StarGrade = (NI % 3) + 1;

			FB2Item NewAccessory;
			NewAccessory.InstanceUID = 98230000 + IPI * 1000 + NI;
			NewAccessory.ItemRefID = 1000000 + 100000 * IPI + 1000 * ItemClassID + 100 * StarGrade;

			if (bRequestToServer) // 辑滚俊 夸没茄促搁 瘤陛鳖瘤 沥焊肺 面盒窃. 辑滚 抛胶飘 目盖靛 荤侩.
			{
				data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*FString::Printf(TEXT("AddItem %d %d"), NewAccessory.ItemRefID, StarGrade));
				continue;
			}

			DevOnlyLoadItemClientTestData(NewAccessory);
			NewAccessory.PrimaryPointType = EItemPrimaryPointType::EIPP_Health; // 厘脚备绰 扁夯 瓷仿摹啊 眉仿牢扒啊.
			NewAccessory.SetPrimaryPoint((float)StarGrade * 100.0f + FMath::FRandRange(0.0f, 100.0f));
			NewAccessory.Level = FMath::RandRange(1, NewAccessory.MaxLevel - 1); // 碍拳 饭骇篮 焊咯瘤绰 吝夸 沥焊 吝 窍唱捞聪 公累困肺 持绢林磊 せ
			NewAccessory.Quality = FMath::RandRange(1, MAX_ITEM_QUALITY); // 前龙档 焊咯瘤绰 沥焊.
			
			// 漂沥 炼扒俊辑父 荤侩 啊瓷茄 钦己, 铰鞭, 檬岿 抛胶飘甫 困秦 漂喊 荐摹 技泼.
			if (FMath::RandRange(1, 10) == 1)
			{
				NewAccessory.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewAccessory.Level = NewAccessory.MaxLevel;
			}
			else if (FMath::RandRange(1, 10) == 1)
			{ // 檬岿 犁丰
				NewAccessory.StarGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
				NewAccessory.Level = 1;
			}
			else if (FMath::RandRange(1, 4) == 1)
			{
				NewAccessory.Level = NewAccessory.MaxLevel;
			}

			// 瓷仿摹 可记 利寸洒 持绢淋 せ
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

		// 沥荐 酒捞袍档
		int32 WeaponEssenceNum = 4;
		for (int32 WEI = 0; WEI < WeaponEssenceNum; ++WEI)
		{
			const int32 StarGrade = WEI + 2;

			FB2Item NewWeaponEssence;
			NewWeaponEssence.InstanceUID = 7349579324000 + IPI * 1000 + WEI; // 荤侩登绰 惑炔俊辑 促弗 酒捞袍篮 绝阑 巴捞骨肺 咯扁辑父捞扼档 般摹瘤 臼霸 利寸洒 持绢淋
			// 沥荐 酒捞袍篮 2000000 锅措
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

		// 困俊辑 阿 辆幅喊 公累困肺 绊弗 巴甸阑 厘馒窍绰单 辑滚俊辑 夸没窍绰 惑炔捞搁 捞 矫痢俊 贸府啊 酒付档 救登菌阑抛聪 厘馒鳖瘤绰 救窃 せ
		if (!bRequestToServer)
		{
			// TODO 酒捞袍 橇府悸 (老窜 0锅 橇府悸 荤侩)
			BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().EquipItems(IDsToEquip, true, 0);
		}
	}

	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
}
#endif


void UB2LobbyInventory::ReqForEquipItems(const TArray<int64>& UIDsToEquip)
{
	// TODO 酒捞袍 橇府悸
	auto ePCClass = GetInventorySelectedPCClass();
	int32 currentItemPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(ePCClass);

	data_trader::Retailer::GetInstance().RequestEquipItem(currentItemPresetId, UIDsToEquip);
}

void UB2LobbyInventory::ReqForUnEquipItems(const TArray<int64>& UIDsToUnEquip)
{
	// TODO 酒捞袍 橇府悸
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
	GhostItemOpIngreds.Empty(); // 府胶飘 厚况瘤扁 傈俊 UI 楷免俊 静扁 困秦 犁丰 沥焊父 浇陆 历厘
	GetAllSelectedItemOpIngreds(GhostItemOpIngreds);
	//ResetAllIngredsList(); -> 咯扁辑 犁丰 付农 绝局绰 霸 救傈窍变 茄单 UI 楷免 包访 夸没捞 乐绢辑 窍瘤 臼绰促.

	// Request 甫 焊郴绊 唱辑具 CachedItemOpTargetData 甫 某教秦 初绰促. 捞傈鳖瘤绰 CachedItemOpTargetData 甫 荤侩窍搁 救凳.
	GetItemOPTargetItemData(CachedItemOpTargetData, false); // 咯扁辑 true 甫 林搁 CachedItemOpTargetData 甫 CachedItemOpTargetData 俊 持栏妨绊 窍摆瘤 せ
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
	{// 肺厚俊急 牢亥配府绰 康旷包府 芒 急琶 PC 甫 扁夯栏肺. 拱沸 绢叼鳖瘤唱 扁夯蔼
		return TransientLobbyGM->GetHeroMgmtModePCClass();
	}

	// GetLocalPlayerCharacter 甫 荤侩且 荐 乐促搁 牢霸烙俊辑 BladeIIPlayer 狼 GetCurrentPlayerClass 甫 荤侩秦 杭 荐档.
	// 咯窍埃 捞霸 荤侩登绰 惑炔俊辑绰 PCClass 甫 流立 瘤沥秦 临 荐 乐阑 抛聪 啊鞭利 流立 瘤沥阑..

	return EPCClass::EPC_End;
}

FInvenPCItemAccessor* UB2LobbyInventory::GetPCItemAccessor(EPCClass InPCClass)
{
	int32 InvenPCIdx = GetInvenPCIndexFromPCClass(InPCClass);
	return (PCItems.IsValidIndex(InvenPCIdx)) ? &(PCItems[InvenPCIdx]) : NULL;
}

TMap<int64, FB2Item>* UB2LobbyInventory::GetStoredItemMapPtr(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	TMap<int64, FB2Item>* RetList = NULL;

	if (InItemInvenType == EItemInvenType::EIIVT_Consumables) // 家葛前篮 葛电 PC 埃俊 傍蜡凳.
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
	// UI 殿俊辑 窜鉴 府胶飘肺 荤侩窍扁 困秦 TArray 俊 持绢林绰 扁瓷烙
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
	{// InOutList 阿 element 甸狼 InventoryType 捞 老摹秦具 窃.
		checkSlow(ThisItemCheck.InventoryType == InInvenType);
	}
#endif

	if (InInvenType != EItemInvenType::EIIVT_Consumables) 
	{
		// 牢亥配府俊 柳凯窍扁 困茄 家泼.
		// 碍拳 < 饭骇 < 喊殿鞭 < 泪陛 < (老矫利)货芭 鉴
		// 疙矫利 夸备荤亲篮 酒聪菌瘤父 困 炼扒甸捞 葛滴 鞍篮 版快甫 困秦 配呕 傈捧仿阑 海捞胶肺..
		struct FInvItemCombatPowerSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return A.Power > B.Power;
			}
		};
		InOutList.StableSort(FInvItemCombatPowerSorter()); // 捞绢瘤绰 酒贰 家泼俊辑档 捞傈 家泼阑 焊粮窍扁 困秦 StableSort 肺.

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
		
		// 弊府绊 货肺 掘篮 酒捞袍阑 老矫利栏肺 力老 菊俊 初酒林绢具 茄促绰单..
		struct FInvItemNewSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return (A.IsNew && !B.IsNew) ? true : false;
			}
		};
		InOutList.StableSort(FInvItemNewSorter());
		

		// 奴 鸥涝俊 蝶弗 沥纺档 窍绰单 焊包窃俊 厘馒 酒捞袍 寇俊 沥荐 酒捞袍档 甸绢坷扁 锭巩. 捞扒 货肺 甸绢坷歹扼档 第俊 持绢临鳖唱..
		struct FInvItemTypeSorter
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return (IsEquipmentItem(A) && !IsEquipmentItem(B)) ? true : false;
			}
		};
		InOutList.StableSort(FInvItemTypeSorter());

		//InstanceUID绰 酒捞袍捞 瘤鞭等 鉴辑肺 譬配府倔侩 酒捞袍捞 啊厘 唱吝俊 瘤鞭登扁 锭巩俊 秦寸 炼扒栏肺 沥纺
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
	{ // 家葛前篮 粱 促弗 扁霖栏肺, 傈捧仿捞聪 饭骇捞聪 捞繁 芭 绝栏聪.. 措眉肺 鸥涝俊 蝶扼 沥秦咙.

		// 酒贰俊 捞矾龙 鸥涝 沥纺档 弊犯瘤父 啊厘 扁夯篮 抛捞喉俊 殿废等 RefID 鉴捞促. 
		// 鸥涝喊 沥纺俊辑档 公扁/规绢备/厘脚备 吝 绢叼俊 利侩登绰 犁丰衬俊 蝶扼 沥纺捞 登绢具 窍绰单 葛滴 老老捞 疙矫窍瘤 给窍绊 RefID 肺 茄促. 
		// 拱沸 沥纺 鉴辑俊 蝶扼 RefID 啊 沥秦廉 乐绢具 茄促.
		struct FConsumablesTypeSorter_RefID
		{
			bool operator()(const FB2Item& A, const FB2Item& B) const
			{
				return (A.ItemRefID < B.ItemRefID);
			}
		};
		InOutList.StableSort(FConsumablesTypeSorter_RefID());  // 捞绢瘤绰 酒贰 家泼俊辑档 捞傈 家泼阑 焊粮窍扁 困秦 StableSort 肺.

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

				//2啊瘤 版快 A sort value > 0捞绊 B啊 -1 牢版快
				//笛促 乐绰 版快 A.SortValue > B.SortValue

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

		// IsEnable** 甸俊辑 UID 眉农甫 窍变 且扒单, Upgrade 狼 版快 磊脚阑 焊咯林霸 登绰瘤绰 葛福摆促. 老窜 绢蠢 版快电 磊脚篮 救 唱坷档废.
		bRemoveCurrentCheck = (CurrCheck.InstanceUID == OpTarget.InstanceUID);

		switch (CurrentItemOpMode)
		{
		case ELobbyInvenItemOpMode::EIVIOP_Levelup:
			bRemoveCurrentCheck |= !IsEnableItemLevelupIngred(CurrCheck); // 急琶 啊瓷茄 巴父 焊咯林扁.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Upgrade:
			bRemoveCurrentCheck |= !(IsItemUpgradeAllowed(CurrCheck) || IsItemAllowedCompositeAndSurpass(CurrCheck)); // 铰鞭狼 版快绰 炼陛 漂捞茄单, 铰鞭 犁丰鄂 扒 绝绊, 铰鞭 鸥百捞 瞪 荐 乐绰 局甸父 焊咯霖促.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Composition:
			bRemoveCurrentCheck |= !IsEnableComposeIngred(CurrCheck); // 急琶 啊瓷茄 巴父 焊咯林扁.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Surpass:
			bRemoveCurrentCheck |= !IsEnableSurpassIngred(CurrCheck); // 急琶 啊瓷茄 巴父 焊咯林扁.
			break;
		case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
			bRemoveCurrentCheck |= !IsVisibilityEhanceIngred(CurrCheck); // 急琶 啊瓷茄 巴父 焊咯林扁.
			break;			
		case ELobbyInvenItemOpMode::EIVIOP_EnhancementChange:
			bRemoveCurrentCheck |= !IsEnableEhanceChangeIngred(CurrCheck); // 急琶 啊瓷茄 巴父 焊咯林扁.
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
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	for (int32 ITI = 0; ITI < (int32)EItemInvenType::EIIVT_End; ++ITI)
	{
		TArray<FB2Item> CurrList; 
		GetStoredItemList(CurrList, (EItemInvenType)ITI, FinalPCClass, false); // 咯扁辑 家泼秦 毫具 绢瞒乔 歹且 芭 鞘夸啊 绝瘤.
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
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	TArray<FB2Item> MergedList;
	GetMergedItemList(MergedList, bIncludeEquipped, FinalPCClass); // 老窜 filter 寇狼 炼扒俊辑 傈眉 钦媚柳 酒捞袍 格废.

	//TArray 静搁 Resize 老绢朝锭 Lambda 罐篮 std::function捞 皋葛府 柄咙
	std::list<std::function<bool(FB2Item)>> FIlters;
	// 瞒肥措肺 鞘磐 眠啊
	FIlters.emplace_back(
		[&InvenTypeFilter](FB2Item CurrItem)
	{
		for (EItemInvenType CurrInvenType : InvenTypeFilter) // InvenTypeFilter 俊 乐绰 element 吝 绢蠢 窍唱客 老摹窍搁 器窃.
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
		for (int32 CurrStarGrade : StarGradeFilter) // StarGradeFilter 俊 乐绰 element 吝 绢蠢 窍唱客 老摹窍搁 器窃.
		{
			if (CurrItem.StarGrade == CurrStarGrade)
			{
				return true;
			}
		}
		return false;
	});
	
	// Lock 等 巴阑 吧矾郴绰 巴篮 磊悼魄概唱 磊悼碍拳 鞍篮 扁瓷俊辑 荤侩.
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

	// 檬岿犁丰(葛风) 抗寇贸府
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
	
	OutResult = MergedList; // 搬苞 府胶飘肺 傈崔.
}

void UB2LobbyInventory::GetStoredItemListForEquipPlace(TArray<FB2Item>& OutResult, EItemEquipPlace InEquipPlace, EPCClass InPCClass /*= EPCClass::EPC_End*/)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	TArray<FB2Item> MergedList;
	GetMergedItemList(MergedList, false, FinalPCClass); // InEquipPlace 俊 蝶扼 InvenType 捞 乐变 茄单.. 努扼俊 弊吧 甘俏窍绰 窃荐甫 滴绰 巴档 构窍绊 秦辑 傲 老窜 MergedList 掘绢柯 饶俊..

	if (MergedList.Num() > 0)
	{
		OutResult = MergedList.FilterByPredicate( // InEquipPlace 客 老摹窍绰 吧肺父 榜扼辰促.
			[&InEquipPlace](FB2Item CurrItem)
			{ // 沥荐, 檬岿犁丰(葛风) 抗寇贸府
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
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	int32 RetSlotCount = BladeIIGameImpl::GetClientDataStore().GetCharacterInvenSlotCount(FinalPCClass, InItemInvenType);
	
	return RetSlotCount;
}

/* 某腐磐 何困喊 牢亥配府 */
int32 UB2LobbyInventory::GetStoredItemNumInSharedSlot(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	if (InItemInvenType == EItemInvenType::EIIVT_Consumables)
	{
		return GetConsumablesList().Num(); // PCClass 埃 傍蜡.
	}
	else if (InItemInvenType != EItemInvenType::EIIVT_End)
	{
		int32 InvenPCIdx = GetInvenPCIndexFromPCClass(FinalPCClass);
		if (InvenPCIdx >= 0 && InvenPCIdx < GetMaxPCClassNum())
		{
			FInvenPCItemAccessor& PCList = PCItems[InvenPCIdx];
			TMap<int64, FB2Item>* ItemMap = PCList.GetItemMapForInvenType(InItemInvenType);

			// 厘馒吝牢 厘厚档 器窃.
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

/* 某腐磐 烹钦 牢亥配府 */
int UB2LobbyInventory::GetStoredItemNumInUnitedSlot(EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	if (InItemInvenType == EItemInvenType::EIIVT_Consumables)
	{
		return GetConsumablesList().Num(); // PCClass 埃 傍蜡.
	}
	else if (InItemInvenType != EItemInvenType::EIIVT_End)
	{
		int32 InvenPCIdx = GetInvenPCIndexFromPCClass(FinalPCClass);
		if (InvenPCIdx >= 0 && InvenPCIdx < GetMaxPCClassNum())
		{
			FInvenPCItemAccessor& PCList = PCItems[InvenPCIdx];
			int32 ItemCount = PCList.GetAllItemMapCount();

			// 厘馒吝牢 厘厚档 器窃.
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

		// 厘馒吝牢 厘厚档 器窃.
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
	InNewItem.bNewlyAddedForEquipSuggest = true; // 芭狼 捞霸 格利老 淀..

	return ReAddStoredItem(InNewItem, bForceAdd, InPCClass, true); // 扁瓷惑 咯扁辑绰 OptionalInsertIndex 绰 鞘夸绝摆促.
}

bool UB2LobbyInventory::ReAddStoredItem(FB2Item& InNewItem, bool bForceAdd, EPCClass InPCClass, bool bUpdateUIDocImmediately /*= true*/)
{ // 咯扁俊 InPCClass 牢磊啊 苞楷 鞘夸且鳖. InNewItem.AllowedPCClass 甫 荤侩且荐档
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	if (InNewItem.InventoryType == EItemInvenType::EIIVT_End)
	{
		return false;
	}

	if (!bForceAdd && IsInventoryFull(InNewItem.InventoryType, FinalPCClass))
	{
		return false;
	}

	bool bRetAdded = false;
	// 泅犁 汲沥俊辑狼 府胶飘甫 啊廉客辑 歹窃.
	TMap<int64, FB2Item>* MapToAdd = GetStoredItemMapPtr(InNewItem.InventoryType, FinalPCClass);
	if (MapToAdd)
	{
		InNewItem.bCurrentlyEquipped = false; // 咯扁促 持绰促绰 扒 利绢档 厘馒窍瘤绰 臼绰 巴.
		MapToAdd->Add(InNewItem.InstanceUID, InNewItem);
		bRetAdded = true;
	}

	if (bUpdateUIDocImmediately)
	{ // 包访 UI Doc 单捞磐 诀单捞飘.. 甫 窍瘤 臼绰 版快绰 ReAdd 甫 荤侩茄 促弗 悼累
		LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	}

	return bRetAdded;
}

bool UB2LobbyInventory::InternalIsInventoryFull(bool IsOver, EItemInvenType InItemInvenType, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	if (InItemInvenType == EItemInvenType::EIIVT_End)
	{
		return true;
	}

	// 泅犁 鸥涝俊辑 啊瓷茄 酒捞袍 焊包窃 俺荐客 角力 酒捞袍 俺荐
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
	// Sort 绰 酒付 狼固啊 绝阑 淀 窍瘤父.. 酒丛 家葛前 傈侩栏肺 sort 扁霖阑 技况辑 贸府窍霸 瞪 荐档. -> ConsumingAmount 肺 窍搁 瞪 淀.
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

	// 1000俺寸 窍唱究 父甸绢焊磊.. 酒捞袍喊肺 促弗瘤绰 葛福摆瘤父..
	const int32 AllowedAmountPerSingleItem = 1000;
	int32 GenerateItemNum = ((InAmount + AllowedAmountPerSingleItem-1) / AllowedAmountPerSingleItem);
	TArray<FB2Item> GeneratedData;
	int32 RemainingAmount = InAmount;
	for (int32 GI = 0; GI < GenerateItemNum; ++GI)
	{
		int32 ThisAmount = FMath::Min(RemainingAmount, AllowedAmountPerSingleItem);
		RemainingAmount = FMath::Max(0, RemainingAmount - ThisAmount);
		FB2Item NewConsumableData;
		// UID 般摹搁 救登绰单 绢瞒乔 摹飘扁瓷捞聪 せ
		NewConsumableData.InstanceUID = 438402842 + FMath::RandRange(1, INT_MAX) + FMath::RandRange(1, INT_MAX); 
		NewConsumableData.ItemRefID = InRefID;
		NewConsumableData.InventoryType = EItemInvenType::EIIVT_Consumables;
		NewConsumableData.ConsumingAmount = ThisAmount;

		GeneratedData.Add(NewConsumableData);
	}

	for (FB2Item& ThisToAdd : GeneratedData)
	{ // 沥侥栏肺 且 芭 鞍栏搁 扁粮俊 乐绰 芭俊 荐樊 鞍捞 钦眉秦具 且 荐档.. 弊扒 绢瞒乔 辑滚俊辑 坷绰 芭浚 贸府啊 登唱..
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
	//酒捞袍 New沥焊 false肺 函版
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
	// 厘馒 棺 泪陛, 橇府悸 乐绰芭 力寇.
	UB2LobbyInventory::GetFilteredItemList(AllTargetItems, false, false, false, InvenTypeFilter, StarGradeFilter);

	// 泅犁 饭骇 诀 措惑捞 登绰 酒捞袍父 乐阑 版快..
	if(EnhanceTargetItem.StarGrade == StarGrade && AllTargetItems.Num() == 1)
		return 0;
	return AllTargetItems.Num();
}

bool UB2LobbyInventory::UpdateSingleItemData(int64 ItemInstanceUID, FB2Item& InDataToUpdate)
{
	FB2Item* FoundItemPtr = FindFromAllPtr(ItemInstanceUID);
	if (!FoundItemPtr)
		return false; // 秦寸 酒捞袍 给茫澜.

	// InDataToUpdate 绰 荤角惑 瓷仿摹 殿狼 单捞磐父 荤侩窍扁 困窃栏肺, 酒捞袍狼 扁檬利牢 沥焊绰 老摹秦具 窃.
	if (InDataToUpdate.InstanceUID != FoundItemPtr->InstanceUID || InDataToUpdate.ItemRefID != FoundItemPtr->ItemRefID || InDataToUpdate.ItemClass != FoundItemPtr->ItemClass
		|| InDataToUpdate.AllowedPCClass != FoundItemPtr->AllowedPCClass || InDataToUpdate.InventoryType != FoundItemPtr->InventoryType || InDataToUpdate.EquipPlace != FoundItemPtr->EquipPlace)
	{
		return false;
	}

	bool bWasEquipped = FoundItemPtr->bCurrentlyEquipped; // 厘馒 惑怕档 咯扁辑 函版措惑篮 酒丛. 捞扒 return false 鳖瘤 窍柳 臼绰促.

	*FoundItemPtr = InDataToUpdate;
	FoundItemPtr->bCurrentlyEquipped = bWasEquipped; // 酒付档 付蛮啊瘤咯具摆瘤父 せ

	if (FoundItemPtr->bCurrentlyEquipped)
	{
		// 包访 UI Doc 单捞磐 诀单捞飘. 酒流篮 厘馒等 巴俊 措秦辑父 乐澜.
		UB2UIDocEquip* DocEquip = UB2UIDocHelper::GetDocEquip(FoundItemPtr->AllowedPCClass, (int32)FoundItemPtr->EquipPlace);
		if (DocEquip)
		{
			DocEquip->UpdateData(InDataToUpdate);
		}
	}
	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Signal(ItemInstanceUID); // 函版等 窍唱父 麦绢辑 诀单捞飘. 眠啊唱 力芭矫俊绰 捞吧 荤侩窍瘤 臼绰促.

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
	//TArray<EPCClass> PCPartUpdateSignalClass; // 寇包惑栏肺 函版登绰 酒捞袍阑 厘馒茄 PC 努贰胶甸 葛滴 葛澜. 阿 努贰胶俊促 脚龋甫 焊郴扁 困秦
	//TArray<EPCClass> AnyEquipStateChangedClass; // 寇包 厘馒 颇飘 函版登绰 版快啊 酒聪扼档 傈捧仿 馆康篮 瞪 荐 乐栏聪 努贰胶 蝶肺 葛澜
	//bool bIsCostumeItem = false;

	//TArray<const FB2Item*> B2ItemPtrArray;
	//for (auto EquippedItemUID : EquipedItems)
	//{
	//	const FB2Item* EquippedItem = FindFromAllPtr(EquippedItemUID);
	//	if (EquippedItem != nullptr)
	//	{
	//		B2ItemPtrArray.Emplace(EquippedItem);

	//		if (GetPCPartForItemEquipPlace(EquippedItem->EquipPlace) != EPCPart::EPCPT_End)
	//		{ // 3D 皋浆 诀单捞飘 矫弊澄阑 困茄 巴捞聪 厘馒茄 巴捞 PC 颇飘 吝 窍唱牢 巴俊 茄秦.
	//			if (ItemAllowedPCClassEnum(EquippedItem->AllowedPCClass) == EPCClass::EPC_End) {
	//				// EPC_End 搁 厘馒秦档 钎矫啊 救登绰 酒捞袍捞芭唱 且 刨单 老窜 葛福聪 葛电 努贰胶甫 倒档废 窍磊.
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
	//		// 包访 UI Doc 单捞磐 诀单捞飘.
	//		if (UB2UIDocEquip* DocEquip = UB2UIDocHelper::GetDocEquip(EquippedItem->AllowedPCClass, static_cast<int32>(EquippedItem->EquipPlace)))
	//			DocEquip->DoEquip(*EquippedItem);

	//		if (EquippedItem->IsNew)
	//		{
	//			SetChangeNewItemTofalse(EquippedItemUID);
	//		}
	//	}
	//}

	//// 厘馒 酒捞袍 技何 沥焊 芒捞 凯妨 乐促搁 厘馒 厘厚 轰甫 阜阑 巴捞骨肺 摧绰促.
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);

	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();

	//for (EPCClass& CurrUpdateClass : PCPartUpdateSignalClass)
	//{ // 秦寸 努贰胶狼 3D 皋浆甫 货肺款 厘馒 惑怕 馆康秦辑 诀单捞飘
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

	////皋矫 诀单捞飘 饶, 局聪皋捞记敲饭捞.
	//for (auto EquippedItem : B2ItemPtrArray)
	//{
	//	EItemEquipPlace eItemEquipPlace = EquippedItem->EquipPlace;

	//	// 厘馒 浇吩 UI effect 档
	//	LobbyPlayUIEquipEffectClass<EItemEquipPlace>::GetInstance().Signal(eItemEquipPlace);

	//	//内胶片 何困老 版快 局聪皋捞记 犁积 何困 瘤沥
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
	//TArray<EPCClass> PCPartUpdateSignalClass; // 寇包惑栏肺 函版登绰 酒捞袍阑 厘馒茄 PC 努贰胶甸 葛滴 葛澜. 阿 努贰胶俊促 脚龋甫 焊郴扁 困秦
	//TArray<EPCClass> AnyEquipStateChangedClass; // 寇包 厘馒 颇飘 函版登绰 版快啊 酒聪扼档 傈捧仿 馆康篮 瞪 荐 乐栏聪 努贰胶 蝶肺 葛澜
	//bool bIsCostumeItem = false;

	//for (auto UnequippedItemUID : ArrayUnEquipItemID)
	//{
	//	const FB2Item* UnequippedItem = FindFromAllPtr(UnequippedItemUID);
	//	if (UnequippedItem != nullptr)
	//	{
	//		if (GetPCPartForItemEquipPlace(UnequippedItem->EquipPlace) != EPCPart::EPCPT_End)
	//		{ // 3D 皋浆 诀单捞飘 矫弊澄阑 困茄 巴捞聪 厘馒茄 巴捞 PC 颇飘 吝 窍唱牢 巴俊 茄秦.
	//			if (ItemAllowedPCClassEnum(UnequippedItem->AllowedPCClass) == EPCClass::EPC_End) {
	//				// EPC_End 搁 厘馒秦档 钎矫啊 救登绰 酒捞袍捞芭唱 且 刨单 老窜 葛福聪 葛电 努贰胶甫 倒档废 窍磊.
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

	//// UnEquipItems!! 府春倔 抗沥
	//BladeIIGameImpl::GetClientDataStore().UnEquipItems(ArrayUnEquipItemID, iPresetID);

	//for (EPCClass& CurrUpdateClass : AnyEquipStateChangedClass)
	//	BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetSetItemValue(CurrUpdateClass, AfterSetItemCounts[CurrUpdateClass]);

	//for (auto UnequippedItemUID : ArrayUnEquipItemID)
	//{
	//	if (const FB2Item* UnequippedItem = FindFromAllPtr(UnequippedItemUID))
	//	{
	//		// 包访 UI Doc 单捞磐诀单捞飘.
	//		if (UB2UIDocEquip* DocEquip = UB2UIDocHelper::GetDocEquip(UnequippedItem->AllowedPCClass, static_cast<int32>(UnequippedItem->EquipPlace)))
	//			DocEquip->RemoveEquip();
	//	}
	//}

	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();

	//for (EPCClass& CurrUpdateClass : PCPartUpdateSignalClass)
	//{ // 秦寸 努贰胶狼 3D 皋浆甫 货肺款 厘馒 惑怕 馆康秦辑 诀单捞飘
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
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);
	if (CurrPCItemAccessor)
	{
		return CurrPCItemAccessor->FindEquippedItem(OutFoundItem, InEquipPlace);
	}
	return false;
}

bool UB2LobbyInventory::GetEquippedCostumeAtPlace(FB2Item& OutFoundItem, ECostumeEquipPlace InEquipPlace, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);
	if (CurrPCItemAccessor)
	{
		return CurrPCItemAccessor->FindEquippedCostume(OutFoundItem, InEquipPlace);
	}
	return false;
}

void UB2LobbyInventory::GetAllEquippedItem(TArray<FB2Item>& OutList, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);

	if (CurrPCItemAccessor)
	{
		CurrPCItemAccessor->GetAllEquippedItems(OutList);
	}
}

void UB2LobbyInventory::GetAllEquippedCostumeItem(TArray<FB2Item>& OutList, EPCClass InPCClass /*= EPCClass::EPC_End*/)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	FInvenPCItemAccessor* CurrPCItemAccessor = GetPCItemAccessor(FinalPCClass);

	if (CurrPCItemAccessor)
	{
		CurrPCItemAccessor->GetAllEquippedCostumeItems(OutList);
	}
}

void UB2LobbyInventory::ConditionalCheckForBetterEquipSuggest()
{
	//bool bIgnoreSuggest = false;

	//// 1. 馆汗 傈捧 档吝俊绰 货肺 掘篮 酒捞袍俊 措茄 厘馒鼻厘阑 公矫
	//// 2. Tutorial档吝俊绰 厘馒鼻厘 公矫
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
			{ // 捞锅 CheckForBetterEquipSuggestInner 俊辑 亮篮 厘厚啊 乐绢辑 鼻厘 措拳惑磊甫 凯菌促搁 措拳惑磊 吝汗秦辑 凯瘤 臼霸 府畔. 弊率俊辑 促矫 咯辨 call 且 巴. 
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
	////{ // 父老 扁粮 厘馒等 霸 酒抗 绝促搁 鼻厘阑 救且 荐档 乐绊.. 傲 俩 粮吧肺 鼻厘且 荐档.
	////	return false; // 捞犯霸 窍搁 鼻厘 救 窍绰 吧肺.
	////}

	//TArray<FB2Item> StoredList;
	//GetStoredItemListForEquipPlace(StoredList, InEquipPlace, InPCClass);
	//
	//// bNewlyAddedForEquipSuggest 眉农等 吧肺父 促矫 鞘磐傅
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
	//struct FItemStarGradeSorter // 老瞒利栏肺 PrimaryPoint 肺 厚背且 芭瘤父 趣矫扼档 悼老茄 霸 乐阑 锭 扁夯利牢 StarGrade 扼档 杭 荐 乐霸 扁霖阑 粱 歹 滴磊..
	//{
	//	bool operator()(const FB2Item& A, const FB2Item& B) const
	//	{
	//		return A.StarGrade > B.StarGrade;
	//	}
	//};
	//NewlyAddedList.StableSort(FItemStarGradeSorter());
	//struct FPrimaryPointSorter // PrimaryPoint 俩 粮 芭 鉴辑措肺 厚背窍扁 困秦 家泼.
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
	//	{ // PrimaryPoint 鉴栏肺 家泼窍看栏聪 啊厘 菊俊波 荤侩窍搁 凳. 窜, Type 鳖瘤 厚背甫.. 窍绰 霸 亮摆瘤..?
	//		if (!bHadEquipped || (CurrCadid.PrimaryPointType == EquippedOne.PrimaryPointType 
	//			&& (B2UIUtil::DifferenceItemPrimaryPoint(CurrCadid, EquippedOne) >= 1)))
	//		{
	//			DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_AcqItemEquipSuggest, FDJLegacySubpopupAddPanelInfo());

	//			check(TransientLobbyGM); // 绝栏搁 官焊.
	//			UB2AcqItemEquipSuggest* AcqItemEquipSuggestPopup = Cast<UB2AcqItemEquipSuggest>(TransientLobbyGM->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_AcqItemEquipSuggest));
	//							
	//			if (AcqItemEquipSuggestPopup)
	//			{
	//				AcqItemEquipSuggestPopup->UpdateData(bHadEquipped, EquippedOne, CurrCadid,
	//					// 急琶 捞饶 CheckForBetterEquipSuggest 甫 促矫 妮且 荐 乐档废.
	//					[this](){CheckForBetterEquipSuggest(); }
	//				);
	//			}

	//			bOpenedDialog = true;
	//			break;
	//		}
	//	}

	//	for (FB2Item& CurrNew : NewlyAddedList)
	//	{ // 歹 亮篮 霸 乐菌电 绝菌电 眉农绰 窍看栏聪 捞锅 府胶飘俊辑绰 bNewlyAddedForEquipSuggest 甫 葛滴 reset
	//		CurrNew.bNewlyAddedForEquipSuggest = false;
	//		UpdateSingleItemData(CurrNew.InstanceUID, CurrNew);
	//	}
	//			
	//	return bOpenedDialog; // 捞锅俊 措拳惑磊甫 凯菌促搁 true 甫 府畔秦辑 惑困窜俊辑 吝汗秦辑 凯瘤 臼霸 茄促. 绢瞒乔 眉农绰 促 且 巴烙.
	//}

	return false;
}

void UB2LobbyInventory::IgnoreBetterEquipSuggest()
{
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{// 傲 葛电 酒捞袍 倒搁辑 bNewlyAddedForEquipSuggest 府悸..
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
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	for (int32 EQI = 0; EQI < (int32)EItemEquipPlace::EIEP_End; ++EQI)
	{ // 厘馒登瘤 臼篮 镑俊 措秦辑档 八祸阑.
		FB2Item EquippedOne;
		const bool bHadEquipped = GetEquippedItemAtPlace(EquippedOne, static_cast<EItemEquipPlace>(EQI), FinalPCClass);

		// 悼老 困摹俊辑 歹 唱篮 器牢飘甫 棵副 荐 乐绰 巴阑 茫绰促.
		TArray<FB2Item> StoredListAtPlace;
		GetStoredItemListForEquipPlace(StoredListAtPlace, static_cast<EItemEquipPlace>(EQI), FinalPCClass);
		
		float BestScoreSoFar = -1.0f;
		int64 CurrentBestUID = 0;
		for (FB2Item& CurrStored : StoredListAtPlace)
		{
			if ((!bHadEquipped || (B2UIUtil::DifferenceItemPrimaryPoint(CurrStored, EquippedOne) >= 1)) && // 厘馒等 巴捞 绝促搁 窜鉴洒 俩 诫 吧肺.
				(!bHadEquipped || CurrStored.PrimaryPointType == EquippedOne.PrimaryPointType) &&
				CurrStored.GetPrimaryPoint() > BestScoreSoFar)
			{
				BestScoreSoFar = CurrStored.GetPrimaryPoint();
				CurrentBestUID = CurrStored.InstanceUID;
			}
		}

		if (CurrentBestUID > 0) // 捞锅 困摹俊 厘馒等 巴焊促 (趣篮 厚绢乐绰 沫俊) 歹 亮篮 巴阑 茫澜.
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
//			AllGathered += CurrCheck.GetPrimaryPoint(); // 扁夯 prim point 歹窃. 咯扁辑 可记蔼鳖瘤 墨款飘窍瘤绰 臼绰促. 扁夯 PrimaryPointType 捞 InPrimPointType 苞 促弗 版快 绢瞒乔 酒贰 可记蔼 秒钦捞 鞘夸窍扁 锭巩俊.
//		}
//
//		// 秦寸 prim point 狼 刘啊可记捞 乐促搁 弊巴档 歹茄促.
//		// AllGathered += FMath::Max(GetEffectiveItemOptionValue(AssociatedOption, CurrCheck), 0.0f); // 窜鉴 刘啊 可记. 歹窃. 辟单 澜荐啊 唱坷绰 版快档 乐唱..?
//	}
//	// Wing 档 咯扁辑 眠啊.
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
//	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.
//
//	TArray<FB2Item> AllEquipped;
//	GetAllEquippedItem(AllEquipped, FinalPCClass);
//
//	// Wing 档 酒捞袍苞 蜡荤茄 矫胶袍捞绊 可记 鸥涝捞 鞍促 焊聪 咯扁辑 贸府.
//	bool bFoundWingData = false;
//	FB2Wing AdditionalWingData;
//	bFoundWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(FinalPCClass, AdditionalWingData);
//
//	return GatherPrimPointFromList(InPrimPointType, AllEquipped, (bFoundWingData ? &AdditionalWingData : NULL));
//}

//void UB2LobbyInventory::GetAllEquippedPrimPointsLocalPlayer(TMap<EItemPrimaryPointType, float>& OutAllPrimPoints, EPCClass InPCClass)
//{
//	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.
//
//	for (int32 PTI = 0; PTI < (int32)EItemPrimaryPointType::EIPP_End; ++PTI)
//	{
//		float ThisTypePoint = GetEquippedPrimPointLocalPlayer((EItemPrimaryPointType)PTI, FinalPCClass);
//		if (ThisTypePoint > 0.0f) // 0 捞窍搁 泅犁 厘馒俊急 绝绰 芭
//		{
//			OutAllPrimPoints.Add((EItemPrimaryPointType)PTI, ThisTypePoint);
//		}
//	}
//}

//float UB2LobbyInventory::GetEquippedOptionPointLocalPlayer(EItemOption InOptionToCheck, EPCClass InPCClass)
//{
//	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.
//
//	TArray<FB2Item> AllEquipped;
//	GetAllEquippedItem(AllEquipped, FinalPCClass);
//
//	// Wing 档 酒捞袍苞 蜡荤茄 矫胶袍捞绊 可记 鸥涝捞 鞍促 焊聪 咯扁辑 贸府.
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
//	{ // 绊蜡 可记捞电 扁成 可记捞电 葛滴.
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
//	// Wing 档 酒捞袍苞 蜡荤茄 矫胶袍捞绊 可记 鸥涝捞 鞍促 焊聪 咯扁辑 贸府.
//	if (InWing)
//	{
//		const FWingPropOption* ThisPropOption = InWing->GetPropOptionPtr(InOptionToCheck); 
//		if (ThisPropOption && ThisPropOption->bIsOpen)
//		{
//			AllFoundRawValues.Add(ThisPropOption->RawOptionAmount);
//		}
//	}
//	
//	// AllFoundRawValues 啊 绝歹扼档 GetEffectiveItemOptionValue 肺 逞变促. 绝绰 版快俊 措茄 扁夯蔼阑 可记 鸥涝俊 蝶扼 府畔秦 临 巴.
//	return GetEffectiveItemOptionValue(InOptionToCheck, AllFoundRawValues);
//}

/* 粱 汗棱秦廉辑 老窜 绝局绊.. 趣矫 鞘夸秦瘤搁 Wing 单捞磐档 绊妨窍档废.
void UB2LobbyInventory::GetAllEquippedOptionPointsLocalPlayer(TMap<EItemOption, float>& OutAllOptions, EPCClass InPCClass)
{
	const EPCClass FinalPCClass = (InPCClass == EPCClass::EPC_End) ? GetInventorySelectedPCClass() : InPCClass; // EPC_End 瘤沥 矫 泅犁 急琶等 吧肺.

	TArray<FB2Item> AllEquipped;
	GetAllEquippedItem(AllEquipped, FinalPCClass);

	// 辆幅喊 Option 甸 吝埃 秒钦
	TMap<EItemOption, TArray<float>> IntermOptionMap;

	for (FB2Item& CurrEquip : AllEquipped)
	{ // 绊蜡 可记捞电 扁成 可记捞电 葛滴.
		for (FItemOption& CurrOption : CurrEquip.IntrinsicOptions)
		{
			float NewValue = CurrOption.RawOptionAmount;
			TArray<float>* ExistingValueArray = IntermOptionMap.Find(CurrOption.OptionType);
			if (ExistingValueArray)
			{ // 咯扁急 窜鉴洒 蔼阑 持扁父 窍绊 葛滴 葛篮 饶 弥辆利牢 蔼栏肺 促矫 啊傍且 巴.
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
			{ // 咯扁急 窜鉴洒 蔼阑 持扁父 窍绊 葛滴 葛篮 饶 弥辆利牢 蔼栏肺 促矫 啊傍且 巴.
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
	{ // 葛酒初篮 吧肺 弥辆利牢 啊傍.
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

	//ResetAllIngredsList(); // 犬角洒..
	//ResetEnhanceItemDatas();

	//GhostItemOpIngreds.Empty();
	//CachedItemOpTargetData = FB2Item(); // 辑滚肺 夸没 焊辰 捞饶俊唱 某教 凳.
	//OnCommitItemOpHeroMgmtPCBackup = EPCClass::EPC_End;
	//OnCommitItemOpInvenPageTabBackup = EItemInvenType::EIIVT_End;
	//CurrentItemOpMode = InMode;
	//ItemOpTargetItemID = TargetItemID;
	//bFreezeitemOpModeIngredSelection = false;

	//if (IsBackupRollBackInventoryTab && TransientLobbyGM && TransientLobbyGM->DJLegacy_GetLobbyUIManager())
	//	CustomBackupInvenPageTab = TransientLobbyGM->DJLegacy_GetLobbyUIManager()->GetInventoryTabSelection();

	//// 积己等 Popup widget 阑 UIP 贸烦 硅摹秦 持阑 菩澄 沥焊甫 父甸绢霖促.
	//UB2LobbyUI_InventoryMain* InventoryUI = TransientLobbyGM ? Cast<UB2LobbyUI_InventoryMain>(TransientLobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
	//UPanelWidget* PopupAddPanel = InventoryUI ? InventoryUI->GetItemDetailPopupParentPanel(true) : nullptr;
	//
	//// 扁鸥 唱赣瘤 葛靛狼 犁丰 急琶 惑怕档.

	//// Mode 尔 TargetID 殿 技泼何磐 窍绊 UI 甫 凯绢具 窃.
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

	//	return; //傈眉 扑诀捞扼 第俊 官层临鞘夸绝促
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
	//		//碍拳傈券
	//		PopUpUI->AddHandler(EUIItemChangePopupMode::Enhance, FItemChangePopupMode::CreateLambda([this]()
	//		{	this->EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_EnhancementChange, ItemOpTargetItemID); }));

	//		//可记 函版
	//		PopUpUI->AddHandler(EUIItemChangePopupMode::Option, FItemChangePopupMode::CreateLambda([TargetItemData]()
	//		{	UB2UIItemOptionChange* ItemOptionChangeWidget = UB2UIManager::GetInstance()->OpenUI<UB2UIItemOptionChange>(UIFName::ItemOptionChange);
	//		if (ItemOptionChangeWidget)
	//		{
	//			ItemOptionChangeWidget->SetTargetItem(TargetItemData);
	//		}
	//		}));
	//		
	//		//康旷傈券
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
	//		//殿鞭 函版
	//		PopUpUI->AddHandler(EUIItemChangePopupMode::Quality, FItemChangePopupMode::CreateLambda([TargetItemData]()
	//		{
	//			DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_QualityChange, FDJLegacySubpopupAddPanelInfo());
	//			check(TransientLobbyGM); // 绝栏搁 官焊.
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
	//	// Note :	捞力何磐绰 碍拳 厚侩(Cost)包访秦辑 辑滚俊 夸没窍瘤臼绊, 扁粮俊 罐酒滴菌带 付胶磐单捞磐俊辑 曼炼窍档废 函版 (2017-04)
	//	//			扁粮 巴档 辑滚/努扼 累诀磊啊 朝棱绊 茄锅俊 函版窍搁 亮摆绰单....
	//}


	//if (OpenInventoryType == EItemInvenType::EIIVT_End)
	//{
	//	// 鸥百 酒捞袍苞 鞍篮 鸥涝狼 徘栏肺 碍力 急琶. 
	//	// QuitItemOpMode 俊辑档 厚搅茄 悼累阑 窍绰单 咯扁辑 徘急琶 磊眉甫 泪辟 扒 钱绢林绢具 茄促.
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

	//// 焊包窃 酒捞袍 技何 沥焊 芒捞 凯妨 乐促搁 摧绰促. 焊包窃俊辑 犁丰甫 波郴具 窍扁 锭巩俊.
	//// 傈券葛靛(傈眉 扑诀 芒)老 版快 摧酒林瘤 臼绰促
	//
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_StoredItemDetail);
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_EquippedItemDetail);
	//ChangeInvenItemOpModeClass<ELobbyInvenItemOpMode>::GetInstance().Signal(CurrentItemOpMode);
	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal(); // 牢亥配府 焊包窃 轰 诀单捞飘 矫俊绰 Target 酒捞袍篮 焊捞瘤 臼霸 瞪 巴.
	//
}

void UB2LobbyInventory::QuitItemOpMode(bool bQuitFromOpSuccess)
{
//	// 葛靛甫 辆丰窍扁 傈, 捞傈 Op 葛靛 己傍 捞饶扼搁 楷崔酒辑 Op 葛靛甫 瘤加且 荐 乐栏骨肺 诀弊饭捞靛 等 酒捞袍 单捞磐 殿阑 焊包秦 敌促.
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
//	GhostItemOpIngreds.Empty(); // GhostItemOpIngreds 绰 蝶肺 厚框 贸府
//	CachedItemOpTargetData = FB2Item();
//#if WITH_EDITOR
//	bItemOpProgressSceneTestMode = false;
//#endif
//
//	ResetAllIngredsList();
//	ResetEnhanceItemDatas();
//
//	// 凯妨 乐阑瘤 葛甫 酒捞袍 碍拳 包访 皋春甸阑 葛滴 摧绰促. (官肺 酒贰俊辑 促矫 凯副 荐档 乐瘤父 せ)
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemLevelUpMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemUpgradeMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemComposeMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemSurpassMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceChangeMain);
//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemHeroChangeMain);
//		
//	LobbyInvenHideItemOpModeGuideTextClass<>::GetInstance().Signal(); // 啊捞靛 咆胶飘档 钎矫登绢 乐阑 荐 乐栏骨肺 摧澜.
//
//	if (bQuitFromOpSuccess && // 弊成 犬牢瞒盔俊辑 QuitItemOpMode 甫 妮窍绰 版快档 乐澜. 角力 Op mode 己傍 矫 阂府绰 版快狼 贸府甸.
//		TransientLobbyGM && TransientLobbyGM->IsValidObj() && TransientLobbyGM->IsHeroMgmtMode()) 
//	{
//		// 碍拳 楷免 棺 己傍 其捞瘤 殿捞 popup 捞 酒囱 喊档 其捞瘤肺 登绢 乐栏骨肺 牢亥配府肺 倒酒咳.
//		// 攫力唱 窍歹扼档 迭洒 巩力绰 救登绰单 鞘夸 绝阑锭俊档 拌加 捞霸 角青登搁 焊扁 荤唱框 せ
//		// 康旷包府 葛靛啊 酒聪扼搁 捞固 DJLegacy UI page 绰 摧腮 惑怕肺 皋牢 肺厚芒栏肺 唱客 乐阑 巴捞骨肺 促矫 柳涝窍搁 救凳.
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
//		// LobbyGM 率 楷免侩 轰 技泼档 辆丰. 
//		LobbyQuitItemOpDirectingViewClass<>::GetInstance().Signal();
//		
//		// 牢亥配府 UI甫 货肺 咯绰单 捞傈 徘阑 蜡瘤窍霸 粱 贸府.
//		if (OnCommitItemOpHeroMgmtPCBackup != EPCClass::EPC_End)
//			TransientLobbyGM->SetHeroMgmtModePCSelection(OnCommitItemOpHeroMgmtPCBackup);
//
//		//if (OnCommitItemOpInvenPageTabBackup != EItemInvenType::EIIVT_End)
//		{// EnterItemOpMode 矫 徘急琶 泪辟 芭 钱绢林绰 巴档 器窃. 蝶扼辑 汲飞 OnCommitItemOpInvenPageTabBackup 捞 抗惑苞 促福霸 甸绢啊 乐绢档 捞扒 bool 牢磊甫 false 肺 秦辑 怖 阂矾霖促.
//			LobbyExternalSetInventoryTabClass<EItemInvenType, bool>::GetInstance().Signal(OnCommitItemOpInvenPageTabBackup, false);
//		}
//
//		if (IsReturnTotemInventory)
//		{
//			// 配袍俊辑 倒酒柯 惑怕扼搁 促矫 芒阑 剁况林档废 窍磊.
//			ReturnTotemInventoryClass<int64>::GetInstance().Signal(PrevSuccessTargetData.InstanceUID);
//		}
//		else
//		{
//			if (bGotPrevItemOpTarget)
//			{
//				// 碍拳/饭骇诀捞 拌加 啊瓷窍促搁 芒阑 剁况林档废 窍磊.
//
//				if (JustFinishedOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup && IsItemLevelupAllowed(PrevSuccessTargetData))
//				{
//					EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_Levelup, PrevSuccessTargetData.InstanceUID);
//				}
//				else if (JustFinishedOpMode == ELobbyInvenItemOpMode::EIVIOP_Enhancement && IsItemEnhanceAllowed(PrevSuccessTargetData))
//				{
//					// 碍拳狼 版快 犁丰甸档 固府 持绢霖促. 
//					EnterItemOpMode(ELobbyInvenItemOpMode::EIVIOP_Enhancement, PrevSuccessTargetData.InstanceUID, EItemInvenType::EIIVT_Consumables);
//
//					if (PrevSelectedIngredItems.Num() > 0)
//					{
//						// 999俺啊 逞栏搁 酒捞袍 InstanceUID啊 咯矾俺老荐 乐栏骨肺 RefID扁霖栏肺 促矫 拌魂阑 秦霖促.
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
//				{ // 扁鸥 促弗 版快扼搁 傲 技何 沥焊 芒 促矫 剁快绰 吧肺. 钦己捞唱 铰鞭阑 秦具 且 惑炔捞扼档 笛 吝 窍唱甫 急琶秦具 窍聪 傲 技何芒栏肺 せ
//					UB2DynItemIcon::CreateLobbyItemDetailPopupCommon(TransientLobbyGM, PrevSuccessTargetData, PrevSuccessTargetData.bCurrentlyEquipped);
//				}
//			}
//		}
//	}
//	else
//	{	
//		// 徘急琶 泪变 芭 钱绢林绰 扒 绢恫 惑炔俊辑电
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
	{ // Item OP req 悼累 磊眉绰 茄 惑炔俊辑 诀单捞飘 窍扁 捞傈 蔼 啊廉咳.
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
			if (FindFromAll(OutData, ItemOpTargetItemID)) // 碍拳 棺 铰鞭 殿狼 鸥百篮 厘馒 酒捞袍老 荐档 乐促.
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
	// 饭骇诀俊辑绰 犁丰 饭骇喊肺 拌魂凳.
	if (CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup)
	{
		return GetCurrentItemLevelUpGoldCost();
	}

	return CurrentBaseItemOpGoldCost;
}

bool UB2LobbyInventory::CanAffordCurrentItemOp() const
{ // 陛傈利牢 何盒阑 焊绰 芭. 碍拳 皋春 殿俊辑 捞霸 false 牢 版快 角青 滚瓢 努腐篮 啊瓷窍瘤父 荤侩磊 泅龙 蜡档 扑诀阑 剁匡 巴烙.
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
		// 捞扒 努扼捞攫飘 拌魂烙. 拱沸 弥辆利栏肺绰 辑滚俊辑 碍拳甫 秦辑 郴妨临 巴捞瘤父 捞扒 努扼捞攫飘俊辑 且 鞘夸啊 粱 乐澜.
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
		// 捞扒 努扼捞攫飘 拌魂烙. 拱沸 弥辆利栏肺绰 辑滚俊辑 碍拳甫 秦辑 郴妨临 巴捞瘤父 捞扒 努扼捞攫飘俊辑 且 鞘夸啊 粱 乐澜.
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
	{ // 饭骇诀矫 葛风 抗寇贸府
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
		EnhanceTarget.InventoryType == IngredCandidate.InventoryType /*&& !IngredCandidate.bIsLocked*/); // 泪变 巴档 犁丰啊 登绰 扒 酒囱单, 弥家茄 急琶 矫档绰 登绢具 窃. (皋矫瘤 冠胶啊 哆档废.)
}

bool UB2LobbyInventory::SelectForIngred(int64 ItemInstanceUID, ELobbyInvenItemOpMode ItemOpMode, bool InForceMenuUpdate)
{
	//if (bFreezeitemOpModeIngredSelection || CurrentItemOpMode != ItemOpMode || ItemInstanceUID == ItemOpTargetItemID) // 泅犁 鸥百篮 ingredient 啊 登绢急 救登摆瘤
	//	return false;

	//// 厘馒 厘厚啊 酒囱 焊包窃俊辑父 茫绰促.
	//FB2Item FoundItem;
	//if (!FindStoredItem(FoundItem, ItemInstanceUID))
	//	return false;

	//if (// 葛靛喊 钦寸茄 犁丰牢瘤 眉农
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup && !IsEnableItemLevelupIngred(FoundItem)) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Composition && !IsEnableComposeIngred(FoundItem)) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass && !IsEnableSurpassIngred(FoundItem)) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Enhancement && !IsEnableEhanceIngred(FoundItem) ||
	//	(CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_EnhancementChange && !IsEnableEhanceChangeIngred(FoundItem)))
	//	)
	//{
	//	return false;
	//}

	//// 急琶等巴捞 乐栏搁 Swap且巴牢瘤 眠啊 急琶阑 阜阑巴牢瘤 搬沥
	//switch (ItemOpMode)
	//{
	//	// Swap SelectItem
	//case ELobbyInvenItemOpMode::EIVIOP_Composition:
	//case ELobbyInvenItemOpMode::EIVIOP_Surpass:
	//case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
	//case ELobbyInvenItemOpMode::EIVIOP_EnhancementChange:
	//	{
	//		// 扁粮俊 急琶茄 巴篮 瘤况霖促.
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

	//// 单捞磐 盎脚
	//UpdateSingleItemData(ItemInstanceUID, FoundItem); // 郴何俊辑 UI 率 单捞磐 盎脚 夸没档 且 巴.

	//if (InForceMenuUpdate)
	//{
	//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // 碍拳芒篮 蝶肺 诀单捞飘	
	//}
	return true;
}

void UB2LobbyInventory::UnSelectForIngred(int64 ItemInstanceUID, ELobbyInvenItemOpMode ItemOpMode)
{
	//if (bFreezeitemOpModeIngredSelection || CurrentItemOpMode != ItemOpMode)
	//	return;

	//FB2Item FoundItem;
	//// 厘馒 厘厚啊 酒囱 焊包窃俊辑父 茫绰促.
	//if (FindStoredItem(FoundItem, ItemInstanceUID))
	//{
	//	FoundItem.bSelectedAsIngred = false;

	//	// 府胶飘俊辑 力芭
	//	for (int32 RI = 0; RI < PendingSelectItemOpIngreds.Num(); ++RI)
	//	{
	//		if (PendingSelectItemOpIngreds[RI].InstanceUID == ItemInstanceUID)
	//		{
	//			PendingSelectItemOpIngreds.RemoveAt(RI);
	//			break;
	//		}
	//	}

	//	// 单捞磐 盎脚
	//	UpdateSingleItemData(ItemInstanceUID, FoundItem);  // 郴何俊辑 UI 率 单捞磐 盎脚 夸没档 且 巴.

	//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // 碍拳芒篮 蝶肺 诀单捞飘
	//}
}

void UB2LobbyInventory::UnSelectForIngred(const TArray<int64>& InInstanceUID, ELobbyInvenItemOpMode ItemOpMode)
{
	if (bFreezeitemOpModeIngredSelection || CurrentItemOpMode != ItemOpMode)
		return;

	for (int32 i = 0; i < InInstanceUID.Num(); ++i)
	{
		FB2Item FoundItem;
		// 厘馒 厘厚啊 酒囱 焊包窃俊辑父 茫绰促.
		if (FindStoredItem(FoundItem, InInstanceUID[i]))
		{
			FoundItem.bSelectedAsIngred = false;

			// 府胶飘俊辑 力芭
			for (int32 RI = 0; RI < PendingSelectItemOpIngreds.Num(); ++RI)
			{
				if (PendingSelectItemOpIngreds[RI].InstanceUID == InInstanceUID[i])
				{
					PendingSelectItemOpIngreds.RemoveAt(RI);
					break;
				}
			}

			// 单捞磐 盎脚
			UpdateSingleItemData(InInstanceUID[i], FoundItem);  // 郴何俊辑 UI 率 单捞磐 盎脚 夸没档 且 巴.
		}
	}
	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // 碍拳芒篮 蝶肺 诀单捞飘
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
	//PendingSelectItemOpIngreds.Empty(); // 府胶飘绰 厚快绊.

	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{ // 捞霸 粱 蠢妨柳促 酵栏搁 PendingSelectItemOpIngreds 厚快扁 傈俊 秦寸 ID 父 茫酒辑 府悸窍绰 吧 秦杭 荐档.. 弊霸 歹 蠢府妨唱 ぇぇ
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

	//if (TransientLobbyGM) // 捞吧 八荤窍绰 捞蜡绰 BeginDestroy 矫痢俊 阂府 锭俊 UI 窜 诀单捞飘甫 阜扁 困秦辑烙. 老馆利牢 版肺绰 酒聪瘤父 咯瓢 困氰窃.
	//{ // 包访 UI Doc 单捞磐 诀单捞飘.
	//	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // 饭骇诀芒档 凯妨 乐促搁..
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
	return GetSharedConsumableAmountOfType(FItemRefIDHelper::GetEnhanceProtectionItemRefId()); // 焊龋鼻 鸥涝 眠啊俊 蝶扼 眠饶 促矫..
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
			// 饭骇捞 鞍芭唱 // 檬岿捞芭唱(泅犁6己)
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
		//炔陛葛风 抗寇贸府
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
		
		// 11己 捞惑狼 厘厚绰 11己 捞惑栏肺 父 檬岿窍绊
		// 捞窍绰 捞傈狼 6己 何磐 鞘磐傅 且 荐 乐档废 力茄 眠啊
		uint16 Minimum_Surpass_Star_Grade = (SurpassTargetItem.StarGrade >= FItemGradeInfo::MINIMUM_ADVANCED_SURPASS_STAR_GRADE ?
			FItemGradeInfo::MINIMUM_ADVANCED_SURPASS_STAR_GRADE : FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE);		

		return (SurpassTargetItem.InstanceUID != Ingred.InstanceUID &&
			Ingred.StarGrade >= Minimum_Surpass_Star_Grade &&			// 函荐 眠啊窍咯 背眉
			SurpassTargetItem.ItemClass == Ingred.ItemClass && // Item Slot栏肺 眠沥. 奴 盒幅父捞 酒囱 备眉 努贰胶鳖瘤 老摹窃.
			(FItemRefIDHelper::ExtractItemVariationFromRefID(SurpassTargetItem.ItemRefID) == FItemRefIDHelper::ExtractItemVariationFromRefID(Ingred.ItemRefID)) // 付瘤阜栏肺 悼老 拌凯 variation 捞绢具.
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

		// todo : 眠饶俊 胶农费捞 咯矾俺啊 登搁 鸥涝栏肺 哗档废 窍磊! 
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

		// todo : 眠饶俊 胶农费捞 咯矾俺啊 登搁 鸥涝栏肺 哗档废 窍磊! 
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
	//钦己等 酒捞袍阑 绝局扁 傈俊 绢恫 橇府悸俊 厘馒登绢 乐绰瘤 眉农茄促
	//FB2Item NewItem;
	//NewItem = GET_TUPLE_DATA(FB2ResponseComposeItem::new_composed_item_index, ComposedItemInfo);
	//int64 MainItemInstanceUID = GET_TUPLE_DATA(FB2ResponseComposeItem::deleted_main_item_id_index, ComposedItemInfo);

	//FPCItemContainer &ItemContainer = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems();
	//int32 PrevPresetID = -1;
	//FB2Item* PrevMainItem = ItemContainer.FindItem(MainItemInstanceUID);
	//if(PrevMainItem)
	//	PrevPresetID = ItemContainer.GetContainsItemPresetID(IntToPCClass(PrevMainItem->AllowedPCClass), PrevMainItem->InstanceUID);

	//ItemOpTargetItemID = BladeIIGameImpl::GetClientDataStore().ComposeItem(ComposedItemInfo); // 钦己篮 酒捞袍捞 酒轿府 官诧促. 葛靛 辆丰 傈鳖瘤绰 TargetItemID 啊 鞘夸窍聪 货肺 官诧 吧肺 盎脚.

	////厘馒茄 悸飘啊 乐菌促搁 辑滚俊辑绰 捞固 厘馒等 惑怕捞骨肺 努扼父 诀单捞飘 秦霖促.
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
	//// 辑滚率俊 碍拳/钦己 殿 Req 焊郴绊 Response 柯 惑炔.

	//// 牢亥配府 UI 其捞瘤俊辑 泪矫 狐瘤绰单 楷免 场唱绊 倒酒棵 锭甫 困秦 徘 急琶阑 归诀秦 敌促.
	//OnCommitItemOpHeroMgmtPCBackup = TransientLobbyGM ? TransientLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	//UB2UIManager_Lobby* LobbyUIManager = TransientLobbyGM ? TransientLobbyGM->DJLegacy_GetLobbyUIManager() : NULL;
	//OnCommitItemOpInvenPageTabBackup = LobbyUIManager ? LobbyUIManager->GetInventoryTabSelection() : EItemInvenType::EIIVT_End;

	//ResetAllIngredsList();

	//// 葛靛喊肺 阿磊狼 皋牢 芒阑 摧绊 楷免阑 矫累茄促. 楷免 芒何磐绰 pop-up 捞 酒囱 刀磊 其捞瘤肺. (蝶扼辑 楷免俊辑 倒酒吭阑 锭甫 困秦 困俊辑 PC 急琶捞尔 徘殿阑 归诀秦 初绰芭)
	//switch (InOpMode)
	//{
	//case ELobbyInvenItemOpMode::EIVIOP_Levelup:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemLevelUpMain);
	//	// ChangeLobbyUIPage 傈俊 View 何磐 刚历 悸诀秦具 茄促. 楷免 UI Widget 救俊辑 墨皋扼俊 立辟窍绰 霸 乐澜.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Levelup);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemLevelUpProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Upgrade:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemUpgradeMain);
	//	// ChangeLobbyUIPage 傈俊 View 何磐 刚历 悸诀秦具 茄促. 楷免 UI Widget 救俊辑 墨皋扼俊 立辟窍绰 霸 乐澜.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Upgrade);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemUpgradeProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Composition:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemComposeMain);
	//	// ChangeLobbyUIPage 傈俊 View 何磐 刚历 悸诀秦具 茄促. 楷免 UI Widget 救俊辑 墨皋扼俊 立辟窍绰 霸 乐澜.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Composition);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemComposeProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Surpass:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemSurpassMain);
	//	// ChangeLobbyUIPage 傈俊 View 何磐 刚历 悸诀秦具 茄促. 楷免 UI Widget 救俊辑 墨皋扼俊 立辟窍绰 霸 乐澜.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Surpass);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemSurpassProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemEnhanceMain);
	//	// ChangeLobbyUIPage 傈俊 View 何磐 刚历 悸诀秦具 茄促. 楷免 UI Widget 救俊辑 墨皋扼俊 立辟窍绰 霸 乐澜.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Enhancement);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemEnhanceProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_TotemSmelting:
	//	// ChangeLobbyUIPage 傈俊 View 何磐 刚历 悸诀秦具 茄促. 楷免 UI Widget 救俊辑 墨皋扼俊 立辟窍绰 霸 乐澜.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_TotemSmelting);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_TotemSmeltingProgress);
	//	break;
	//case ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume:
	//	// ChangeLobbyUIPage 傈俊 View 何磐 刚历 悸诀秦具 茄促. 楷免 UI Widget 救俊辑 墨皋扼俊 立辟窍绰 霸 乐澜.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemLevelUpProgress);
	//	break;
	//}

	//LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(OnCommitItemOpHeroMgmtPCBackup); // 酒捞袍 寇包捞 官拆 荐 乐栏骨肺 厘厚 厘馒 惑怕档 诀单捞飘甫..
	//UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(OnCommitItemOpHeroMgmtPCBackup, false);
}

void UB2LobbyInventory::ItemUpgradeCostDelivered(const FB2GetItemUpgradeCost& UpgradeCost)
{
//	CurrentBaseItemOpGoldCost = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::need_gold_index, UpgradeCost);
//	CurrentUpgradeStoneCost = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::need_upgrade_stone_count_index, UpgradeCost);
//	// 夸扒 沥秦初绊 荤侩窍聪 眉农父..
//	int32 UpgradeStoneID = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::need_upgrade_stone_template_id_index, UpgradeCost);
//	b2network::B2ItemServerInfoPtr ServerNextUpgradeItem = GET_TUPLE_DATA(FB2ResponseItemUpgradeCost::next_item_index, UpgradeCost);
//	NextUpgradeItem = ServerNextUpgradeItem;
//
//#if !UE_BUILD_SHIPPING
//	FB2Item UpgradeTargetItem;
//	GetItemOPTargetItemData(UpgradeTargetItem, false);
//	// 力措肺 救 坷绰 葛剧牢单..
//	//checkSlow(UpgradeStoneID == FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(UpgradeTargetItem));
//#endif
//	EnterItemUpgradeModeClass<int64>::GetInstance().Signal(BeforeUpgradeItem.InstanceUID);
//	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
//	LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI 档 诀单捞飘
}

void UB2LobbyInventory::ItemComposeCostDelivered(const FB2GetItemComposeCost& ComposeCost)
{
	//CurrentBaseItemOpGoldCost = GET_TUPLE_DATA(FB2ResponseItemComposeCost::need_gold_index, ComposeCost);

	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI 档 诀单捞飘
}

void UB2LobbyInventory::ItemSurpassCostDelivered(const FB2GetItemSurpassCost& SurpassCost)
{
	//CurrentBaseItemOpGoldCost = GET_TUPLE_DATA(FB2ResponseItemSurpassCost::need_gold_index, SurpassCost);

	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI 档 诀单捞飘
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

	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // UI 档 诀单捞飘
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
	//LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // 碍拳芒篮 蝶肺 诀单捞飘	
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

	// SortedAutoItemLevelupList 狼 炼扒俊 蝶扼 磊悼饭骇诀 柳青..
	BeginReqAutoItemLevelupCostWithSortedList(SortedAutoItemLevelupList);
}

void UB2LobbyInventory::SortAutoLevelupItemList(const TArray<FB2Item>& AllBulkList, TArray<FAutoItemLevelupSingleSet>& OutSortedList)
{
	// 割割 鞘磐甫 啊瘤绊 付备 葛酒初篮 府胶飘俊辑 绢恫 吧 鸥百栏肺 窍绊 绢恫 吧 犁丰肺 且瘤 殿阑 沥秦辑 OutSortedList 肺 逞变促.

	// 老窜 辑肺 鸥百/犁丰啊 瞪 荐 乐绰 悼老茄 辆幅尝府 弓绰促. 咯扁俊 眠啊肺 悼老 殿鞭鳖瘤 利侩
	TArray<FAutoItemLevelupPossibleSet> AllPossibleSet;
	
	for (int32 BLI = 0; BLI < AllBulkList.Num(); ++BLI)
	{
		const FB2Item& ThisBulkOne = AllBulkList[BLI];
		bool bFoundExistingSet = false;

		BII_CHECK(!ThisBulkOne.bIsLocked); // 泪败 乐促搁 柳累俊 咯扁 府胶飘肺 甸绢客急 救凳.
		if (ThisBulkOne.bIsLocked){
			continue;
		}

		if (!IsEssenceItem(ThisBulkOne) && !IsItemLevelupAllowed(ThisBulkOne)){ // 弥措 碍拳茄档俊 档崔沁促芭唱 秦辑 碍拳 倾侩 救登绰 局甸篮 UI 俊辑 鞘磐傅 救登绢 乐阑 荐 乐栏聪 咯扁辑 芭弗促.
			continue;
		}

		for (int32 API = 0; API < AllPossibleSet.Num(); ++API)
		{
			FAutoItemLevelupPossibleSet& ThisPossibleSet = AllPossibleSet[API];
			if (ThisPossibleSet.ItemsList.Num() > 0)
			{ // 泅犁 PossibleSet 俊辑 霉锅掳 夸家父栏肺 鞍篮 何幅肺 加且 荐 乐绰 巴牢瘤 八荤茄促.
				if (
					// 窍唱甫 target 栏肺 惑沥秦辑 犁丰啊 瞪 荐 乐促搁 辑肺 鞍捞 促弗 鸥百狼 犁丰档 瞪 荐 乐绰 芭
					IsPossibleItemLevelupIngred(ThisPossibleSet.ItemsList[0], ThisBulkOne) &&
					// 眠啊肺 悼老 殿鞭尝府 弓扁.
					(ThisPossibleSet.ItemsList[0].StarGrade == ThisBulkOne.StarGrade) &&
					// 倾侩 PC Class 绰 捞吧 荤侩且 UI 惑 绢瘤埃秦急 咯扁肺 甸绢坷绰 局甸篮 鞍酒具 窍绰单.. 趣 葛福聪 八荤秦辑 唱卉 芭 绝瘤.
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
		{ // 扁粮俊辑 给 茫疽促搁 夯牢狼 巩颇 芒汲
			FAutoItemLevelupPossibleSet NewSet;
			if (!IsEssenceItem(ThisBulkOne))
				NewSet.ItemsList.Add(ThisBulkOne);
			else
				NewSet.EssenceItemList.Add(ThisBulkOne);
			AllPossibleSet.Add(NewSet);
		}
	}

	// 阿 PossibleSet 郴俊辑 饭骇 窜困肺 家泼. 臭篮 饭骇阑 快急利栏肺 碍拳且 芭扼.
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
	
	// 付瘤阜栏肺 30饭骇 棺 扁鸥 茄档俊 吧府绰 窜困肺 肋扼辑 弥辆 府胶飘甫 哗辰促.
	for (int32 API = 0; API < AllPossibleSet.Num(); ++API)
	{
		TArray<FB2Item> ThisPossibleSetList = AllPossibleSet[API].ItemsList;
		TArray<FB2Item>& ThisEssenceItemList = AllPossibleSet[API].EssenceItemList;

		if(ThisPossibleSetList.Num() == 0)
			continue;
		else if (ThisPossibleSetList.Num() == 1 && ThisEssenceItemList.Num() == 0)
			continue;

		//List 钦
		ThisPossibleSetList.Append(ThisEssenceItemList);

		// 菊俊吧 (臭篮 饭骇) 快急利栏肺 鸥百栏肺 伙绊 第俊吧 (撤篮 饭骇) 快急利栏肺 犁丰肺 伙绰促.
		int32 IdxF = 0; // from Front
		int32 IdxR = ThisPossibleSetList.Num() - 1; // from Rear
		for (; IdxF < IdxR; ++IdxF)
		{
			if(IsEssenceItem(ThisPossibleSetList[IdxF]))
				break;

			FAutoItemLevelupSingleSet NewFinalSet;
			FB2Item EnhancePreviewCheckTarget = ThisPossibleSetList[IdxF]; // 鸥百篮 菊俊辑何磐
			NewFinalSet.TargetItemID = ThisPossibleSetList[IdxF].InstanceUID;
			
			TArray<FB2Item> EnhancePreviewCheckIngreds;

			for (; IdxR > IdxF; --IdxR)
			{
				FB2Item ThisPreviewCheckIngred = ThisPossibleSetList[IdxR]; // 犁丰绰 第俊辑何磐
				EnhancePreviewCheckIngreds.Add(ThisPreviewCheckIngred);
				NewFinalSet.IngredientsID.Add(ThisPossibleSetList[IdxR].InstanceUID);

				// 犁丰俺荐 力茄
				if (NewFinalSet.IngredientsID.Num() >= MAXINGREDIENTCOUNT && NewFinalSet.TargetItemID > 0 && NewFinalSet.IngredientsID.Num() > 0)
				{
					OutSortedList.Add(NewFinalSet);
					NewFinalSet.IngredientsID.Empty();
				}

				// 泅犁 NewFinalSet 俊辑 弥绊 扁措 饭骇俊 抗螟摹俊 档崔窍搁 IdxR 窍唱 皑家矫虐绊 咯扁急 弊父. 促澜 IdxF 肺 逞绢啊档废.
				// 磊悼 碍拳 府胶飘俊急 犁丰 俺荐 力茄阑 滴瘤 臼绰促. ReqForSingleAutoItemLevelupList 俊辑 辑滚 率栏肺 Req 朝副 锭 牢磊啊 促福霸 甸绢皑.
				// 磊悼 碍拳俊辑 犁丰 俺荐 力茄捞 甸绢啊搁 弥措 碍拳 饭骇阑 嘎眠扁啊 概快 绢妨况咙.
				float ExpectedPostExp = 0.0f;
				int32 PreviewLevel = GetLevel_PreviewItemLevelup(ExpectedPostExp, EnhancePreviewCheckTarget, EnhancePreviewCheckIngreds, false);
				if (PreviewLevel >= EnhancePreviewCheckTarget.MaxLevel)
				{
					--IdxR;
					break;
				}
			}

			// 弥辆 府胶飘俊 器窃矫虐扁 傈俊 付瘤阜 八荤扼档..
			// 犁丰 俺荐啊 10俺老 版快 抗寇 贸府
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

	if (CopiedAutoItemLevelupCostReqList.Num() > 0) // CopiedAutoItemLevelupCostReqList 甫 啊瘤绊 窍绰 芭扼 捞霸 绝栏搁 救凳.
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
	//{ // 泅 夸备荤亲俊 蝶扼 厚侩捞 葛磊扼歹扼档 角青 滚瓢 努腐篮 且 荐 乐栏骨肺 咯扁肺 棵 荐 乐澜. 窜, 救郴 皋矫瘤甫 剁款促.
	//	ShortageMGR->PopupGoToShopForGoldMessage();
	//	// 厚侩阑 秒钦秦 好歹聪 葛磊扼辑 吝窜捞瘤父 UI 绰 摧瘤 臼档废.
	//	EndAutoItemLevelup(true, false);
	//}
}

void UB2LobbyInventory::BeginReqAutoItemLevelupWithSortedList(const TArray<FAutoItemLevelupSingleSet>& InSortedList)
{
	CopiedAutoItemLevelupMainReqList.Empty();
	CopiedAutoItemLevelupMainReqList.Append(InSortedList);
	if (CopiedAutoItemLevelupMainReqList.Num() > 0) // CopiedAutoItemLevelupMainReqList 甫 啊瘤绊 窍绰 芭扼 捞霸 绝栏搁 救凳.
	{
		AutoItemLevelupPhase = EAutoItemLevelupPhase::AEPhase_RequestingEnhancement;
		ReqForSingleAutoItemLevelupList(); // 夯拜 磊悼 碍拳 夸没 疟坷橇. 促 场唱搁 舅酒辑 EndAutoItemLevelup 啊 阂副 巴. Cost 夸没捞 乐促搁 咯扁辑 矫累阑 窍瘤 臼霸 瞪 巴.
	}
}

void UB2LobbyInventory::ReqForSingleAutoItemLevelupList()
{
	BII_CHECK(AutoItemLevelupPhase == EAutoItemLevelupPhase::AEPhase_RequestingEnhancement);
	BII_CHECK(CopiedAutoItemLevelupMainReqList.Num() > 0);

	// 泅犁 备泅栏肺绰 夸没俊 措秦 览翠捞 柯 饶俊具 促澜 夸没阑 焊尘 荐 乐栏骨肺 CopiedAutoItemLevelupMainReqList 俊辑 窍唱究 夸没阑 柳青茄促.

	for (int32 SI = CopiedAutoItemLevelupMainReqList.Num() - 1; SI >= 0; --SI) // 缔锅龋何磐 秦辑.
	{
		// 咯扁辑 for 巩阑 档绰 扒 弥辆 八荤俊辑 利例窍瘤 臼篮 府胶飘扼档 乐阑 版快 胶诺窍扁 困秦辑烙. 夸没 磊眉绰 茄锅俊 窍唱父 朝赴促.
		FAutoItemLevelupSingleSet& ThisSet = CopiedAutoItemLevelupMainReqList[SI];
		if (ThisSet.TargetItemID <= 0 || ThisSet.IngredientsID.Num() == 0){
			CopiedAutoItemLevelupMainReqList.Pop();
			continue;
		}

		data_trader::Retailer::GetInstance().RequestLevelupItem(ThisSet.TargetItemID, ThisSet.IngredientsID, true); // 磊悼碍拳俊辑狼 碍拳 夸没篮 老馆 碍拳客 付蛮啊瘤 橇肺配妮捞瘤父 牢磊甫 促福霸 淋.
		break;
	}

	if (CopiedAutoItemLevelupMainReqList.Num() == 0) // 沥惑利牢 版快绰 酒聪瘤父 措厚且 鞘夸绰 乐瘤.
	{
		EndAutoItemLevelup(false, false);
	}
	else
	{
		// 捞 版快俊档 馆靛矫 Response 啊 柯促绰 焊厘篮 绝栏聪, 弥饶狼 鸥捞赣扼档 汲摹秦 初绰霸 亮阑 淀?
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

	//// GET_TUPLE_DATA 秦辑 ID 嘎绰瘤扼档 犬牢秦 杭鳖..

	//FB2Item JustEnhancedTarget;
	//JustEnhancedTarget = GET_TUPLE_DATA(FB2ResponseLevelupItem::main_item_index, EnhancedItemInfo);
	//FAutoItemLevelupSingleSet RearSet = CopiedAutoItemLevelupMainReqList.Pop();
	//BII_CHECK(JustEnhancedTarget.InstanceUID == RearSet.TargetItemID); // 流傈俊 ReqForSingleAutoItemLevelupList 肺 夸没阑 焊辰 巴俊 措茄 览翠捞 嘎绰瘤 眉农
	//
	//CachedAutoItemLevelupResultItems.Add(JustEnhancedTarget.InstanceUID, JustEnhancedTarget); // 搬苞 UI 俊 谎副 单捞磐 某教.
	//
	//// 咯傈洒 巢酒 乐绰瘤 咯何俊 蝶扼 拌加 柳青 趣篮 辆丰.
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

	//// 捞饶 胶跑篮 角力 磊悼碍拳 夸没捞 场抄 版快俊父..
	//if (!bEndOfCostRequestOnly)
	//{
	//	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	//	// 磊悼 碍拳 扑诀捞 凯妨 乐促搁 咯扁辑 摧绊 搬苞芒阑 剁款促.
	//	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelup);
	//	if (bEndOnOperationSuccess)
	//	{
	//		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelupResult, FDJLegacySubpopupAddPanelInfo());
	//	}
	//}

	//CachedAutoItemLevelupResultItems.Empty(); // 夸扒 弥家茄 磊悼碍拳 搬苞芒 率俊辑 (积己 矫) 荤侩茄 饶俊 厚款促.
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
	// UI 橇府轰 侩档烙. 府胶飘 家泼窍绊 厚侩 拌魂鳖瘤父 窍绊 颊 灸澜.

	TArray<FAutoItemLevelupSingleSet> LocalSortedList;
	
	SortAutoLevelupItemList(AllBulkList, LocalSortedList);

	int32 LevelupCost = 0;

	for (const FAutoItemLevelupSingleSet& ThisSet : LocalSortedList)
	{
		// FAutoItemLevelupSingleSet 浚 UID 父 甸绢乐绢辑 促矫 茫酒咳.. 官焊鞍焙
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
	//	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Signal(Item.InstanceUID); // 凯妨 乐阑 技何 沥焊芒 诀单捞飘甫 困秦..
	//}
	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal(); // 泪陛 惑怕俊 蝶扼 沥纺档 促矫 秦具 窍骨肺 窜老 酒捞袍父 诀单捞飘 窍绰 霸 酒聪扼 傈眉 诀单捞飘 秦 霖促.
}

void UB2LobbyInventory::UnlockedItems(const FB2UnlockedItems& UnlockedItems)
{
	BladeIIGameImpl::GetClientDataStore().UnLockItems(UnlockedItems);

	//GetB2ItemArray(FB2ResponseUnlockItem::unlocked_items_index, UnlockedItems);
	//for (auto Item : B2ItemArray)
	//{
	//	LobbyTryUpdateSingleItemIconClass<int64>::GetInstance().Signal(Item.InstanceUID); // 凯妨 乐阑 技何 沥焊芒 诀单捞飘甫 困秦..
	//}
	//LobbyUpdateAllItemIconsClass<>::GetInstance().Signal(); // 泪陛 惑怕俊 蝶扼 沥纺档 促矫 秦具 窍骨肺 窜老 酒捞袍父 诀单捞飘 窍绰 霸 酒聪扼 傈眉 诀单捞飘 秦 霖促.
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
	//绢恫 酒捞袍阑 且搏瘤 辑滚肺 单捞磐甫 焊郴百 阐 秦具 窃.
	//函券且 酒捞袍 ID甫 辑滚肺 焊尘 抗沥. 关俊辑 度鞍捞 id甫 罐绊 鞘夸茄 抛捞喉阑 炼雀窍档废 秦具 窃
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
	//check(TransientLobbyGM); // 绝栏搁 官焊.
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
{ // 俺惯 侩档 捞寇俊档 鞘夸窍霸 登搁 !UE_BUILD_SHIPPING 力芭窍绊 荤侩窍档废.
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
	//	// RemainingNum 阑 窍唱 临牢 盲肺 促澜 夸没阑 扁距茄促.
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
{ // 傈捧 闺繁胶 抛胶飘 侩. 漂沥 某腐磐狼 漂沥 厘馒茄 酒捞袍狼 可记 官层摹扁.
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
		{ // 扁粮 波 蜡瘤窍绰 版快 扁粮俊 乐绰 芭 茫绊 诀单捞飘何磐 矫档.
			for (int32 ROI = 0; ROI < RemainingOptionsToReplace.Num(); ++ROI)
			{
				FItemOption& ThisToReplace = RemainingOptionsToReplace[ROI];
				bool bFoundAndUpdated = false;
				for (FItemOption& ThisExisting : FoundEquipItem.IntrinsicOptions)
				{
					if (ThisExisting.OptionType == ThisToReplace.OptionType)
					{
						ThisExisting.RawOptionAmount = ThisToReplace.RawOptionAmount; // 函版.
						bFoundAndUpdated = true;
						break;
					}
				}
				if (!bFoundAndUpdated) // 给 茫疽促搁 罚待可记 八祸 矫档
				{
					for (FItemOption& ThisExisting : FoundEquipItem.RandomOptions)
					{
						if (ThisExisting.OptionType == ThisToReplace.OptionType)
						{
							ThisExisting.RawOptionAmount = ThisToReplace.RawOptionAmount; // 函版.
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

		// 酒流 巢篮 巴甸篮 扁粮俊 绝带 可记捞聪 绊蜡可记, 罚待可记 啊府瘤绰 臼绊 鉴辑措肺 啊瓷且 锭鳖瘤 盲况持扁父 茄促.
		// 可记喊肺 啊瓷茄 荐摹绰 俺惯侩 扁瓷捞扼 撇妨辑 奴老朝 扒 绝栏聪 咯扁俊 窍靛内爹窃. 努扼捞攫飘俊辑绰 可记 单捞磐甫 罐扁父 秦辑 荐摹 力茄阑 包府窍瘤绰 臼绰促.
		
		while (RemainingOptionsToReplace.Num() > 0 && FoundEquipItem.IntrinsicOptions.Num() < 4)
		{
			FItemOption ThisToReplace = RemainingOptionsToReplace[0]; // 菊俊辑何磐..
			FoundEquipItem.IntrinsicOptions.Add(ThisToReplace); // 扁粮波尔 吝汗瞪 啊瓷己捞 咯傈洒 乐扁绰 窍瘤父 弊扒 InOptionsToReplace 甫 舅酒辑 肋 林绰 吧肺.			
			RemainingOptionsToReplace.RemoveAt(0);
		}
		while (RemainingOptionsToReplace.Num() > 0 && FoundEquipItem.RandomOptions.Num() < 3)
		{ // 付蛮啊瘤肺.
			FItemOption ThisToReplace = RemainingOptionsToReplace[0];
			FoundEquipItem.RandomOptions.Add(ThisToReplace);
			RemainingOptionsToReplace.RemoveAt(0);
		}

		UpdateSingleItemData(FoundEquipItem.InstanceUID, FoundEquipItem); // 弥辆利栏肺 函版等 单捞磐甫 利侩.
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
			FString::Printf(TEXT("[救郴] 牢亥配府啊 厚绢乐嚼聪促.\n\"抗(Yes)\"甫 急琶窍矫搁 抛胶飘 侩 酒捞袍甸阑 盲况靛赋聪促.\n\n"))
			)) == EAppReturnType::Yes)
		{
			data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddDemoItem"));
		}
		else
		{ // 茄锅 No 甫 急琶沁栏搁 泅犁 饭骇俊辑绰 歹 拱绢焊瘤 臼霸.
			bSelectedNoForDummyDataSuggestion = true;
		}
	}
#endif
}

void UB2LobbyInventory::DevOnlyItemLevelupProgressSceneTest(int32 IngredNum)
{
	// 扁夯利牢 轰 傈券 鞍篮 扒 窍瘤父 牢亥配府 其捞瘤啊 酒聪搁 肋 救冈鳃 荐 乐澜. 
	//if (TransientLobbyGM && TransientLobbyGM->IsValidObj() && CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None) // 啊瓷茄 ItemOp 葛靛啊 酒囱 惑怕啊 亮阑 淀..
	//{
	//	bItemOpProgressSceneTestMode = true;

	//	// 碍拳楷免俊急 ItemOpTargetItemID 甫 技泼秦毫具 家侩捞 绝促. 抛胶飘 窃荐扼 绢恫 酒捞袍捞 乐绰瘤 炼荤秦辑 技泼且 荐档 绝绰 畴俯捞绊
	//	// 诀单捞飘 傈狼 某教登绰 鸥百 酒捞袍 单捞磐甫 技泼.
	//	CachedItemOpTargetData = FB2Item();
	//	CachedItemOpTargetData.ItemRefID = 1000100; // 角力肺 ItemInfo 俊 乐绰 RefID 甫 持绢临 鞘夸啊 乐促.
	//	CachedItemOpTargetData.InstanceUID = 32423243; // UID 档 购啊 蔼阑 持绢林变 秦具 窃.
	//	DevOnlyLoadItemClientTestData(CachedItemOpTargetData);
	//	CachedItemOpTargetData.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//	IngredNum = FMath::Clamp(IngredNum, 1, ItemLevelupIngredLimit);

	//	GhostItemOpIngreds.Empty(); // 碍拳 楷免 芒俊辑 荤侩且 犁丰 酒捞袍 单捞磐.
	//	for (int32 GI = 0; GI < IngredNum; ++GI)
	//	{
	//		FB2Item NewIngred;
	//		NewIngred.ItemRefID = 1000100; // 角力肺 ItemInfo 俊 乐绰 RefID 甫 持绢临 鞘夸啊 乐促.
	//		NewIngred.InstanceUID = 4820942398429 + GI;

	//		DevOnlyLoadItemClientTestData(NewIngred);
	//		NewIngred.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//		GhostItemOpIngreds.Add(NewIngred);
	//	}

	//	CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_Levelup;		
	//	// ChangeLobbyUIPage 傈俊 View 何磐 刚历 悸诀秦具 茄促. 楷免 UI Widget 救俊辑 墨皋扼俊 立辟窍绰 霸 乐澜.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Levelup);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemLevelUpProgress);
	//}
}

void UB2LobbyInventory::DevOnlyItemEnhanceProgressSceneTest(int32 IngredNum)
{
	// 扁夯利牢 轰 傈券 鞍篮 扒 窍瘤父 牢亥配府 其捞瘤啊 酒聪搁 肋 救冈鳃 荐 乐澜. 
	//if (TransientLobbyGM && TransientLobbyGM->IsValidObj() && CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None) // 啊瓷茄 ItemOp 葛靛啊 酒囱 惑怕啊 亮阑 淀..
	//{
	//	bItemOpProgressSceneTestMode = true;

	//	// 碍拳楷免俊急 ItemOpTargetItemID 甫 技泼秦毫具 家侩捞 绝促. 抛胶飘 窃荐扼 绢恫 酒捞袍捞 乐绰瘤 炼荤秦辑 技泼且 荐档 绝绰 畴俯捞绊
	//	// 诀单捞飘 傈狼 某教登绰 鸥百 酒捞袍 单捞磐甫 技泼.
	//	CachedItemOpTargetData = FB2Item();
	//	CachedItemOpTargetData.ItemRefID = 1000100; // 角力肺 ItemInfo 俊 乐绰 RefID 甫 持绢临 鞘夸啊 乐促.
	//	CachedItemOpTargetData.InstanceUID = 32423243; // UID 档 购啊 蔼阑 持绢林变 秦具 窃.

	//	DevOnlyLoadItemClientTestData(CachedItemOpTargetData);
	//	CachedItemOpTargetData.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//	IngredNum = FMath::Clamp(IngredNum, 1, ItemLevelupIngredLimit);

	//	GhostItemOpIngreds.Empty(); // 碍拳 楷免 芒俊辑 荤侩且 犁丰 酒捞袍 单捞磐.
	//	for (int32 GI = 0; GI < IngredNum; ++GI)
	//	{
	//		FB2Item NewIngred;
	//		NewIngred.ItemRefID = 1000100; // 角力肺 ItemInfo 俊 乐绰 RefID 甫 持绢临 鞘夸啊 乐促.
	//		NewIngred.InstanceUID = 4820942398429 + GI;

	//		DevOnlyLoadItemClientTestData(NewIngred);
	//		NewIngred.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;

	//		GhostItemOpIngreds.Add(NewIngred);
	//	}

	//	CurrentItemOpMode = ELobbyInvenItemOpMode::EIVIOP_Enhancement;
	//	// ChangeLobbyUIPage 傈俊 View 何磐 刚历 悸诀秦具 茄促. 楷免 UI Widget 救俊辑 墨皋扼俊 立辟窍绰 霸 乐澜.
	//	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_Enhancement);
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_ItemEnhanceProgress);
	//}
}

void UB2LobbyInventory::DevOnlyItemCUSProgressSceneTest(ELobbyInvenItemOpMode InOpMode)
{	
	//if (InOpMode == ELobbyInvenItemOpMode::EIVIOP_None || InOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup){
	//	return; // 碍拳绰 咯扁辑 贸府 救 窃.
	//}

	//// 扁夯利牢 轰 傈券 鞍篮 扒 窍瘤父 牢亥配府 其捞瘤啊 酒聪搁 肋 救冈鳃 荐 乐澜. 
	//if (TransientLobbyGM && TransientLobbyGM->IsValidObj() && CurrentItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None) // 啊瓷茄 ItemOp 葛靛啊 酒囱 惑怕啊 亮阑 淀..
	//{
	//	bItemOpProgressSceneTestMode = true;

	//	// 鸥百 酒捞袍 单捞磐.
	//	CachedItemOpTargetData = FB2Item();
	//	// 角力肺 ItemInfo 俊 乐绰 RefID 甫 持绢临 鞘夸啊 乐促. 檬岿捞搁 6己 厘厚 铂肺促啊
	//	CachedItemOpTargetData.ItemRefID = InOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass ? 1000600 : 1000100; 
	//	CachedItemOpTargetData.InstanceUID = 32423243; // UID 档 购啊 蔼阑 持绢林变 秦具 窃.
	//	DevOnlyLoadItemClientTestData(CachedItemOpTargetData);
	//	CachedItemOpTargetData.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;
	//	CachedItemOpTargetData.Level = 30; // 钦己铰鞭 殿篮 漂喊洒 Level 阑 嘎苗拎具 窃.

	//	GhostItemOpIngreds.Empty();
	//	if (InOpMode == ELobbyInvenItemOpMode::EIVIOP_Composition || InOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass)
	//	{ // 犁丰 酒捞袍 单捞磐甫 鞘夸肺 窍绰 葛靛
	//		FB2Item NewIngred;
	//		// 角力肺 ItemInfo 俊 乐绰 RefID 甫 持绢临 鞘夸啊 乐促. 檬岿捞搁 6己 厘厚 铂肺促啊
	//		NewIngred.ItemRefID = InOpMode == ELobbyInvenItemOpMode::EIVIOP_Surpass ? 1000600 : 1000100;
	//		NewIngred.InstanceUID = 4820942398429;
	//		DevOnlyLoadItemClientTestData(NewIngred);
	//		NewIngred.PrimaryPointType = EItemPrimaryPointType::EIPP_Attack;
	//		NewIngred.Level = 30; // 付蛮啊瘤肺 犁丰档 30饭骇 嘎苗淋
	//		GhostItemOpIngreds.Add(NewIngred);
	//	}

	//	CurrentItemOpMode = InOpMode;

	//	// ChangeLobbyUIPage 傈俊 View 何磐 刚历 悸诀秦具 茄促. 楷免 UI Widget 救俊辑 墨皋扼俊 立辟窍绰 霸 乐澜.
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
	//	TArray<EPCClass> PCPartUpdateSignalClass; // 寇包惑栏肺 函版登绰 酒捞袍阑 厘馒茄 PC 努贰胶甸 葛滴 葛澜. 阿 努贰胶俊促 脚龋甫 焊郴扁 困秦
	//	TArray<EPCClass> AnyEquipStateChangedClass; // 寇包 厘馒 颇飘 函版登绰 版快啊 酒聪扼档 傈捧仿 馆康篮 瞪 荐 乐栏聪 努贰胶 蝶肺 葛澜
	//					
	//	//  UIDoc 府悸
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
	//	GetMergedItemList(MergedList, true, InPCClass); // 老窜 filter 寇狼 炼扒俊辑 傈眉 钦媚柳 酒捞袍 格废.

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

	//// 救郴巩 快急鉴困 => 函版 > 眠啊/秦眉
	//// 1. 技飘瓤苞啊 眠啊/昏力等霸 乐绰 版快 => 技飘瓤苞 函版
	//// 2. 技飘瓤苞 刘/皑等霸 乐绰 版快 => 技飘瓤苞 函版
	//// 3. 技飘瓤苞啊 眠啊等霸 乐绰 版快 => 技飘瓤苞 眠啊
	//// 4. 技飘瓤苞啊 秦力等霸 乐绰 版快 => 技飘瓤苞 秦力

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
	////3D 楷免 乐绰 (酒丛 弊成 颇萍努 矫胶袍阑 荤侩窍绰 楷免 率
	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::SealBoxResult);
	//UB2UISealBoxResult* UISealBoxResult = UB2UIManager::GetInstance()->OpenUI<UB2UISealBoxResult>(UIFName::SealBoxResult);
	//if (UISealBoxResult)
	//{
	//	UISealBoxResult->SetRewardDataManually(UnsealBoxPtr, SealBoxClass);
	//	UISealBoxResult->BindDocAuto();
	//}
	//*/
	//
	////力累家尔 悼老茄 规过栏肺 楷免窍绰 率
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

//SummonItem 何盒俊辑 汗荤秦辑 啊廉柯 何盒. 咆胶飘唱 滚瓢 炼累, 啊拜 钎矫 何盒俊 炼累捞 啊秦柳 惑怕.
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
//			// 捞 沥档搁 喊档狼 Widget part 努贰胶啊 登绢具 且 巴 鞍篮单 肯傈 喉风橇赴飘肺 内爹捞 登绢 乐澜.
//			// 颇飘俊辑 啊廉坷绰霸 惑寸洒 腹促.. ぱぱ
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
//			// 某腐磐 enum 鉴辑客 老摹..?
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

	//辑滚肺 豪牢窃 秦力 夸没窍绰 镑
	//茄锅俊 割俺 凯扼绰 郴侩篮 扁裙俊 绝绢辑 茄锅俊 1俺究 凯档废 窍靛内爹 凳
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