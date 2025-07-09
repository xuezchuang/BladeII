// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2Inventory.h"
#include "B2MessageInfo.h"
#include "B2ClientDataStore.h"
//#include "B2LobbyUI_ConvertItemPopup.h"
//#include "B2UIStoreProductCost.h"
//#include "B2UIBackWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "B2Button.h"
#include "B2LobbyInventory.generated.h"

/** Provide access to ClientDataStore's item data of a specific PC class for lobby inventory operation, not containing the real stuff here. */
USTRUCT()
struct FInvenPCItemAccessor
{
	GENERATED_USTRUCT_BODY()

	FInvenPCItemAccessor()
	{
		MyPCClass = EPCClass::EPC_End;
	}

	FInvenPCItemAccessor(EPCClass InPCClass)
	{
		MyPCClass = InPCClass;
	}
private:
	EPCClass MyPCClass; // Needed for indexing PCItems array of ClientDataStore.

private:
	FPerCharacterItemContainer* GetMyItemStore(); // Access to the real big shit.
	FEquippedItemInfo* GetMyEquippedStore();
	const TMap<int32, FEquippedItemInfo>* GetMyEquippedCostumeStore();
public:
	int32 GetAllItemMapCount();
	/** Returns one of the lists of ClientDataStore or NULL. Note that you cannot get shared item list (consumables) by this. */
	TMap<int64, FB2Item>* GetItemMapForInvenType(EItemInvenType InItemInvenType);

	// To access AllEquippedInfo..
	bool FindEquippedItem(FB2Item& OutFound, EItemEquipPlace InEquipPlace); // OutFound is valid only when it returns true.
	bool FindEquippedItemByID(FB2Item& OutFound, int64 InstanceUID); // OutFound is valid only when it returns true.
	FB2Item* FindEquippedItemPtrByID(int64 InstanceUID); // Returns native reference.
	bool IsEquipped(EItemEquipPlace InEquipPlace); // Is anything equipped at specified place.
	void GetAllEquippedItems(TArray<FB2Item>& OutList); // Get currently equipped items from AllEuippedInfo
	void GetAllEquippedItems(TArray<FB2Item>& OutList, EItemInvenType InItemInvenType);

public:
	bool FindEquippedCostume(FB2Item& OutFound, ECostumeEquipPlace InEquipPlace); // OutFound is valid only when it returns true.
	void GetAllEquippedCostumeItems(TArray<FB2Item>& OutList);
};

UENUM()
enum class EAutoItemLevelupPhase : uint8
{
	AEPhase_None, // Auto enhancement is not currently running
	AEPhase_SortingList,
	AEPhase_RequestingEnhancement
};

UENUM(BlueprintType)
enum class EItemLevelUpJackpotType : uint8
{
	ER_Normal,
	ER_Jackpot,
	ER_GreateJackpot,
	ER_Failed
};

enum class EItemLoopType : uint8
{
	EI_Normal,
	EI_Looping,
	EI_EndLoop
};

/**
 * Management of client item data get by communication to the server
 * It was initially "Lobby" inventory, but now provide some static interfaces to be used in-game.
 * Still provide special handling in Lobby.
 */
UCLASS()
class BLADEII_API UB2LobbyInventory : public UB2Inventory
{
	GENERATED_BODY()

	/** Better be careful when need to handle it. This is static due to allow access from some static functions here.
	 * It must be strictly valid only in LobbyGameMode. */
	static class AB2LobbyGameMode* TransientLobbyGM;

	/** Provide interfaces to access real stuff of each relevant player class. */
	static TArray<FInvenPCItemAccessor> PCItems;
	/** Consumables are shared among all characters. Key is item's InstanceUID */
	static TMap<int64, FB2Item>& GetConsumablesList(); // Like another shared Accessor, from ClientDataStore..


	//////////////////////////////////////////////////
	// For Item Op mode (levelup / enhance / compose / upgrade / surpass)

	/** Item modification (enhancement, upgrade, .. etc) operation mode. */
	ELobbyInvenItemOpMode CurrentItemOpMode;
	int64 ItemOpTargetItemID; // The target item of modification operation. Enhancement, upgrade or whatever case.
	/** Being cached right before the main ItemOp operation. Do NOT refer to this before you request the item operation itself.
	 * This is used for comparing the result and for progress scene, where the old non-updated data is required. */
	FB2Item CachedItemOpTargetData;
	FB2Item BeforeUpgradeItem;
	FB2Item NextUpgradeItem;

	//Current ChangeItemCharacterCost Info

	int32 DarkCrystalCost;
	TArray<FB2Item>		ChangeItem;

	/** To be set at item op mode main UI commit button, i.e. The user decided to do some (levelup) on item with current selection.
	 * Just the final request to server is deferred due to some directed scene. It is set between that interval to prevent any other change. */
	uint32 bFreezeitemOpModeIngredSelection : 1;

	EPCClass OnCommitItemOpHeroMgmtPCBackup; // To restore it back when going back to normal inventory.
	EItemInvenType OnCommitItemOpInvenPageTabBackup;  // To restore it back when going back to normal inventory.

	TArray<FB2Item> PendingSelectItemOpIngreds;
	static int32 ItemLevelupIngredLimit; // constant
	int32 CurrentBaseItemOpGoldCost; // To be set by server response on entering enhancement mode. Not the final cost depend on the Op mode.

	/** This is not for actual item Op operation, for UI scene after the actual item Op operation is requested or completed internally.
	 * As they are "Ghost", do not treat them like part of usable inventory item, and do NOT use them for other than UI visualization. */
	TArray<FB2Item> GhostItemOpIngreds;

	/** Required upgrade stone cost for current selection. Valid only in upgrade op mode. */
	int32 CurrentUpgradeStoneCost;

	//////////////////////////////////////////////////
	EAutoItemLevelupPhase AutoItemLevelupPhase; // Must be different from normal ItemOpMode..

	struct FAutoItemLevelupSingleSet
	{
		FAutoItemLevelupSingleSet()
		{}
		int64 TargetItemID;
		TArray<int64> IngredientsID;
	};

	TArray<FAutoItemLevelupSingleSet> SortedAutoItemLevelupList; // The original sorted list, which will be maintained until the end.
	TArray<FAutoItemLevelupSingleSet> CopiedAutoItemLevelupCostReqList; // Copied and pop for each cost response
	TArray<FAutoItemLevelupSingleSet> CopiedAutoItemLevelupMainReqList; // Copied and pop for each main response

	FTimerHandle AutoItemLevelupResponseWaitTH;
	int32 AutoItemLevelupTotalCost;

	/** Saved during auto enhancement main work, to display result UI. */
	TMap<int64, FB2Item> CachedAutoItemLevelupResultItems;
	EItemLevelUpJackpotType CachedItemLevelupJackpotType;
	EItemInvenType CustomBackupInvenPageTab;  // To restore it back when going back to normal inventory.
	int32 PendingEnhanceProtectionItemRefID;
	int32 CurrentEnhanceItemSuccessRate;
	int32 CurrentEnhanceItemScrollCost;
	int32 CurrentEnhanceItemProtectionScrollCost;
	int32 DeltaEnhanceLevelForResultPresent;	// 강화레벨을 결과 연출에 보여주기 위한 용도

	/** Comes from inventory main UI. It's not supposed to be significant here, just for some reference of other UI. */
	EItemInvenType MainInvenViewCurrentTab;

	uint32 LobbyReqEquipItemsTicket;
	uint32 LobbyReqUnEquipItemsTicket;
	uint32 LobbyReqUnEquipItemsOtherPresetTicket;
	uint32 LobbyReqToggleItemLockTicket;
	uint32 LobbyReqSellItemTicket;
	uint32 LobbyReqSellItemsTicket;
	uint32 LobbyReqSellConsumableItemTicket;
	uint32 LobbyReqDecompositionItemsTicket;
	uint32 LobbyReqConvertDataTicket;
	uint32 LobbyReqConvertItemTicket;
	uint32 LobbyReqItemLevelupWithCurrentSelectionTicket;
	uint32 LobbyReqEnhanceItemTicket;
	uint32 LobbyReqUpgradeItemTicket;
	uint32 LobbyReqComposeItemWithCurrentSelectionTicket;
	uint32 LobbyReqSurpassItemWithCurrentSelectionTicket;
	uint32 LobbyReqAutoItemLevelupTicket;
	uint32 LobbyReqSmeltingTotemTicket;
	uint32 LobbyAllItemsAddedTicket;
	uint32 LobbyItemsAddedTicket;
	uint32 EnterItemLevelupModeTicket;
	uint32 EnterItemUpgradeModeTicket;
	uint32 EnterItemCompositionModeTicket;
	uint32 EnterItemSurpassModeTicket;
	uint32 EnterItemEnhanceModeTicket;
	uint32 EnterItemCostumeEnhanceModeTicket;
	uint32 EnterTotemSmeltingModeTicket;
	uint32 LobbyInventoryViewJustOpenTicket;
	uint32 LobbyInventoryViewJustCloseTicket;
	uint32 QuitItemOpModeTicket;
	uint32 SelectForItemLevelupIngredTicket;
	uint32 UnSelectForItemLevelupIngredTicket;
	uint32 SelectForComposeIngredTicket;
	uint32 UnSelectForComposeIngredTicket;
	uint32 SelectForSurpassIngredTicket;
	uint32 UnSelectForSurpassIngredTicket;
	uint32 SelectForEnhanceIngredTicket;
	uint32 UnSelectForEnhanceIngredTicket;
	uint32 SelectForEnhanceProtectionItemTicket;
	uint32 SelectForEnhanceChangeIngredTicket;
	uint32 UnSelectForEnhanceChangeIngredTicket;
	uint32 SelectForItemHammerTicket;
	uint32 UnSelectForItemHammerTicket;
	uint32 DeliveryEquipItemTicket;
	uint32 DeliveryUnequipItemTicket;
	uint32 DeliveryLockItemTicket;
	uint32 DeliveryUnlockItemTicket;
	uint32 DeliverySellItemTicket;
	uint32 DeliverySellItemErrorTicket;
	uint32 DeliverySellConsumableItemTicket;
	uint32 DeliveryConvertDataTicket;
	uint32 DeliveryConvertItemTicket;
	uint32 DeliveryDismantleItemsTicket;
	uint32 DeliveryDismantleItemsErrorTicket;
	uint32 DeliveryItemLevelupTicket;
	uint32 DeliveryEnhanceItemTicket;
	uint32 DeliverySurpassItemTicket;
	uint32 DeliveryUpgradeItemTicket;
	uint32 DeliveryComposeItemTicket;
	uint32 DeliveryGetItemUpgradeCostTicket;
	uint32 DeliveryGetItemComposeCostTicket;
	uint32 DeliveryGetItemSurpassCostTicket;

	uint32 SelectForLevelUpGradeTicket;
	uint32 UpdateItemPresetTicket;

	uint32 bEventSubscribed : 1;
	int32 CachedStarGrade;

	int32 CachedTotemRefineCostType;

private:
	TArray<std::shared_ptr<issue>>	Issues;

public:

	UB2LobbyInventory(const FObjectInitializer& ObjectInitializer);

	virtual void BeginDestroy() override;

	static void StaticInit(); // From game module starting.

	void InitLobbyInventory(class AB2LobbyGameMode* InLobbyGM); // For LobbyGameMode only..?
private:
	void SubscribeEvents();
	void UnsubscribeEvents();

	/** Not like Init, it is called when Lobby inventory UI page is open. */
	void OnLobbyInventoryViewOpenUp();
	/** Not like destroy, it is called when Lobby inventory UI page is closed. */
	void OnLobbyInventoryViewClose();

	/** Those "add item" operations are handled by ClientDataStore, instead of here, then be called by signals sent by ClientDataStore */
	void OnAllItemsAddedAtStore();
	void OnItemsAddedAtStore(const FB2AddedItems& AddedItems);

public:
#if !UE_BUILD_SHIPPING
	/** If bRequestToServer is true, it actually request to (test) server add operation (still dummy data).
	 * Otherwise, it is client-only, and being a fallback of actual inventory filling-up operation. */
	void DevOnlyGenerateDummyItemData(bool bRequestToServer);
#endif
private:
	//////////////////////////////////////////////////
	// Inventory's request to server functionalities. 
	// They have their relevant client side interfaces to be called by server response callbacks.

	void ReqForEquipItems(const TArray<int64>& UIDsToEquip);
	void ReqForUnEquipItems(const TArray<int64>& UIDsToUnEquip);
	void ReqForUnEquipItems(const int32 PresetID, const TArray<int64>& UIDsToUnEquip);
	void ReqForToggleItemLock(int64 ItemInstanceUID, bool bLock);
	void ReqForSellItem(int64 ItemInstanceUID, bool bIsTotem);
	void ReqForSellItems(const TArray<int64>& UIDsToSell, bool bIsTotem);
	void ReqForSellComsumableItem(int64 ItemInstanceUID, int32 ItemConsumCount);
	void ReqForConvertData(int32 templateId_);
	void ReqForConvertItem(int32 ItemID, int32 ConvertTpye_);
	void ReqForDecompositionItems(const TArray<int64>& UIDsToSell, bool bIsTotem);
	void ReqForItemLevelupWithCurrentSelection();
	void ReqForEnhanceItem();
	void ReqForUpgradeItem();
	void ReqForComposeItemWithCurrentSelection();
	void ReqForSurpassItemWithCurrentSelection();

	void ReqForSmeltingTotem(int64 TotemInstanceID, int32 ServerOptionID, int32 ServerRefineCostType, bool LastRefine);
	void ReqForToggleTotemLock(int64 TotemInstanceUID, bool bLock);

	void OnReqForItemOpCommon();
public:
	/** In this case of request function, it is provided as somewhat like utility for request using client side variable type. */
	static void ReqForExtendInventory(EPCClass InPCClass, EItemInvenType InvenType, int32 ExtendCount);

	//////////////////////////////////////////////////

	//////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Some of interfaces here are may be obsolete, due to direct item data handling of ClientDataStore
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	/** Selected PC class for inventory, considering some context. Could be changed.. Don't trust it that much.. */
	static EPCClass GetInventorySelectedPCClass();

private:
	/** Returns one among PCItems elements. Don't forget that consumables cannot get from this list. */
	static FInvenPCItemAccessor* GetPCItemAccessor(EPCClass InPCClass);

	/** Directly access internal item container of certain tab.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	static TMap<int64, FB2Item>* GetStoredItemMapPtr(EItemInvenType InItemInvenType, EPCClass InPCClass = EPCClass::EPC_End);
public:
	void SetBeforeUpgradeItem(const FB2Item& InItem) {
		BeforeUpgradeItem = InItem;
	}
	FB2Item GetUpgradeNextItem() const {
		return NextUpgradeItem;
	}

	int32 GetAvailableAutoSelectItemCount(int32 StarGrade);

	/** Get an item list for InItemInvenType and InPCClass (or shared list depend on InItemInvenType)
	 * It is mainly intended to be used for displaying items in certain tab.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible.
	 * Optionally sort the array by certain rule. */
	static void GetStoredItemList(TArray<FB2Item>& OutResult, EItemInvenType InItemInvenType, EPCClass InPCClass = EPCClass::EPC_End, bool bSort = true, int32 Grade = -1);

	/**
	 * Item sorting for inventory stored list view is implemented here.
	 * Items in InOutList are expected to have the same InventoryType with InInvenType
	 */
	static void SortItemListForStoredItemView(EItemInvenType InInvenType, TArray<FB2Item>& InOutList);

	/** Compared to GetStoredItemList, It filter out some items which does not fit for display for CurrentItemOpMode from the OutResult. */
	void GetStoredItemList_ItemOpModeFilter(TArray<FB2Item>& OutResult, EItemInvenType InItemInvenType, EPCClass InPCClass = EPCClass::EPC_End, bool bSort = true, int32 Grade = -1);

	void GetStoredPendingItemList(TArray<FB2Item>& OutResult);

	/** Get merged list of all inventory types for InPCClass, optionally include equipped items too.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	static void GetMergedItemList(TArray<FB2Item>& OutResult, bool bIncludeEquipped, EPCClass InPCClass = EPCClass::EPC_End);

	/** Get item list satisfies some condition set.
	 * **Filters are like you can get all items satisfying any one of elements in each Filter. If any one of filter lists is empty, OutResult will be none.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	static void GetFilteredItemList(TArray<FB2Item>& OutResult, bool bIncludeEquipped, bool bIncludeLocked, bool bIncludeEquippedPreset, TArray<EItemInvenType>& InvenTypeFilter, TArray<int32>& StarGradeFilter, EPCClass InPCClass = EPCClass::EPC_End,
		bool bIncludeLevelUp = true, bool bIncludeEnhanced = true, bool bIncludeEssence = true, bool bIncludeAnvil = true);

	/** Get item list which can be equipped at InEquipPlace, probably to compare to currently equipped item at InEquipPlace.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	static void GetStoredItemListForEquipPlace(TArray<FB2Item>& OutResult, EItemEquipPlace InEquipPlace, EPCClass InPCClass = EPCClass::EPC_End);

	/** Get allowed item container slot number for items of InItemInvenType and PCClass.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	static int32 GetItemContainerSize(EItemInvenType InItemInvenType, EPCClass InPCClass = EPCClass::EPC_End);

	/** The return value is to be compared to the ItemContainerSize or ConsumablesContainerSize (which can get from GetItemContainerSize)
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	static int32 GetStoredItemNumInSharedSlot(EItemInvenType InItemInvenType, EPCClass InPCClass = EPCClass::EPC_End);
	static int32 GetStoredItemNumInUnitedSlot(EItemInvenType InItemInvenType, EPCClass InPCClass = EPCClass::EPC_End);
	int32 GetStoredCostomeItemNumInUnitedSlot(EPCClass InPCClass);

	// Totem Inventory Logic
	static void GetAllTotemList(TArray<FB2Totem>& OutResult);
	static void GetEquippedTotemList(TArray<FB2Totem>& OutResult, EPCClass InPCClass);
	static void GetUnEquippedTotemList(TArray<FB2Totem>& OutResult);

private:
	/** Use this for adding any newly acquired items (by buying or combat reward, etc..) to stored list.
	 * It must be used just once for any item, not afterward.
	 * Refer to ReAddStoredItem for parameters. */
	bool NewAddStoredItem(FB2Item& InNewItem, bool bForceAdd = false, EPCClass InPCClass = EPCClass::EPC_End);

	/** Add an item to an inventory slot of a PCClass. Added inventory type will be decided by InNewItem.
	 * It is intended to be used for re-adding already existing item (acquired before) by server sync, or any other various reasons.
	 * It returns false if unable to add, mainly due to ItemContainerSize limitation
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible.
	 * @param bForceAdd : If true, NewItem will be added even if inventory is full. It is actually necessary in many cases. We need to limit it by server or UI stuff..
	 * @param bUpdateUIDocImmediately : Set it to false only in special usage case. */
	bool ReAddStoredItem(FB2Item& InNewItem, bool bForceAdd = false, EPCClass InPCClass = EPCClass::EPC_End, bool bUpdateUIDocImmediately = true);

	/** IsOver : True = Over, False = More */
	static bool InternalIsInventoryFull(bool IsOver, EItemInvenType InItemInvenType, EPCClass InPCClass = EPCClass::EPC_End);
public:
	/**
	 * Get consumable item of designated InRefID defined for specific type, supposed to be in shared slot.
	 * Items of this type typically have some count to totally consume one item of UID.
	 */
	static void GetSharedConsumableOfType(TArray<FB2Item>& OutResult, int32 InRefID);
	static int32 GetSharedConsumableAmountOfType(int32 InRefID);
#if !UE_BUILD_SHIPPING
	void DevOnlyGenerateDummyConsumable(int32 InRefID, int32 InAmount);
#endif

	/** Is specified type of inventory is full for PCClass
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	static bool IsInventoryFull(EItemInvenType InItemInvenType, EPCClass InPCClass = EPCClass::EPC_End);
	/** It returns false only when current stored items are more than container size, almost similar to IsInventoryFull */
	static bool IsInventoryOverLimit(EItemInvenType InItemInvenType, EPCClass InPCClass = EPCClass::EPC_End);
	/** Get the list of local player character class who need to clean up their inventory to enter stage. */
	static void GetPCClassesNeedToCleanInventory(TArray<EPCClass>& OutClassList);

	static bool IsContainsAllPreset(const FB2Item& InItem);

	static bool IsInOtherItemPresets(const FB2Item& InItem);

	static void SetChangeNewItemTofalseTap(EPCClass PCClass, EItemInvenType InvenType);
	static void SetChangeNewItemTofalse(int64 ItemInstanceUID);
	/** Item search function by InstanceUID from store list.
	 * OutFoundItem contains valid data only when it returns true. */
	static bool FindStoredItem(FB2Item& OutFoundItem, int64 ItemInstanceUID);
	/** Search item from all player character's equipped list.
	 * OutFoundItem contains valid data only when it returns true.  */
	static bool FindEquippedItem(FB2Item& OutFoundItem, int64 ItemInstanceUID);
	/** FindStoredItem + FindEquippedItem.
	 * OutFoundItem contains valid data only when it returns true.
	 * You can check if OutFoundItem is in stored list or equipped list by FB2Item's bCurrentlyEquipped field. */
	static bool FindFromAll(FB2Item& OutFoundItem, int64 ItemInstanceUID);

private:
	//////////////////////////////////////////////////
	// For internal use of above Find** functions, provide reference returning function.
	// Better not use them directly if you just need item data.
	static FB2Item* FindStoredItemPtr(int64 ItemInstanceUID);
	static FB2Item* FindEquippedItemPtr(int64 ItemInstanceUID);
	static FB2Item* FindFromAllPtr(int64 ItemInstanceUID);
	//////////////////////////////////////////////////

public:
	static bool UpdateSingleItemData(int64 ItemInstanceUID, FB2Item& InDataToUpdate);
	//////////////////////////////////////////////////////////////////////
	// Equipping interface

private:
	/** Callback handler to the server's equip item response.
	 * Find an item of specified instance UID from relevant PC's item container, then equip it to proper equip place slot.
	 * Existing equipped item will get took off from equip slot and go back to container, if exists. */
	void EquippedItems(const FB2EquippedItems&);

	void EquippedItems(const int32& PresetID, const TArray<int64>& EquipedItems, const TArray<int64>& UnequipedItems, bool UpdateBattleScore = true);

	/** Callback handler to the server's un-equip item response.
	 * If an item having specified instance UID is currently equipped, take it off from player and store it back to container.
	 * It will fail and return false if container is full (but could be fail due to other reasons too).
	 * If failed due to container size (check IsInventoryFull), the only way you can un-equip an item is equipping other one. */
	void UnEquippedItems(const FB2UnequippedItems&);
	void UnEquippedItems_New(const TArray<int64>& ArrayUnEquipItemID, const int32 iPresetID = -1);

	void ChangePreset(const EPCClass InPCClass, const EItemPresetSlotType PresetSlot);

	void NoticeSetItemEffect(const TMap<int32, int32>& PreSetItems, const TMap<int32, int32>& AfterSetItems, const bool& InIsCostumeItem = false);
public:

	/** Get currently equipped item at certain equipping place. OutFoundItem will be valid only when it returns true.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	static bool GetEquippedItemAtPlace(FB2Item& OutFoundItem, EItemEquipPlace InEquipPlace, EPCClass InPCClass = EPCClass::EPC_End);
	static bool GetEquippedCostumeAtPlace(FB2Item& OutFoundItem, ECostumeEquipPlace InEquipPlace, EPCClass InPCClass = EPCClass::EPC_End);

	/** Get the list of currently equipped items for InPCClass. Not like AllEuippedInfo, only actively equipped one.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	static void GetAllEquippedItem(TArray<FB2Item>& OutList, EPCClass InPCClass = EPCClass::EPC_End);
	static void GetAllEquippedCostumeItem(TArray<FB2Item>& OutList, EPCClass InPCClass = EPCClass::EPC_End);

	void ConditionalCheckForBetterEquipSuggest();
private:
	/** Check stored list to see if we got any newly added item which is better than currently equipped, then pop-up a dialog to suggest equipping new one.
	 * It will be recursively called until go through all possible list, for all PC's. */
	void CheckForBetterEquipSuggest();
	/** Sub functionality of CheckForBetterEquipSuggest, for each PC class and EquipPlace */
	bool CheckForBetterEquipSuggestInner(EItemEquipPlace InEquipPlace, EPCClass InPCClass);
	void IgnoreBetterEquipSuggest();

	int32 GetCurrentItemLevelUpGoldCost() const;
public:
	/** Almost like CheckForBetterEquipSuggest, but it just gather IDs of items that suggested for new equipping instead of invokes suggesting pop-up.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	static void GetEquipSuggestion(TArray<int64>& OutSuggestedIDs, EPCClass InPCClass = EPCClass::EPC_End);

	/** Gather designated type of prim point from InItemList, considering options associated with that prim point too. */
	static float GatherPrimPointFromList(EItemPrimaryPointType InPrimPointType, const TArray<FB2Item>& InItemList, const FB2Wing* InWing);

	/** Get accumulated point to be applied to the player character of InPCClass by current equip list.
	 * It will return 0.0 if no such primary point or increasing option found from current equipment.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	 //static float GetEquippedPrimPointLocalPlayer(EItemPrimaryPointType InPrimPointType, EPCClass InPCClass = EPCClass::EPC_End);
	static float GetEquippedPrimPoint(const TArray<FB2Item>& AllEquipped, const FB2Wing* InWing, EItemPrimaryPointType InPrimPointType);
	/** Get all non-zero prim points of InPCClass's equip list.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	static void GetAllEquippedPrimPointsLocalPlayer(TMap<EItemPrimaryPointType, float>& OutAllPrimPoints, EPCClass InPCClass = EPCClass::EPC_End);
	static void GetAllEquippedPrimPoints(const TArray<FB2Item>& AllEquipped, const FB2Wing* InWing, TMap<EItemPrimaryPointType, float>& OutAllPrimPoints);
	/** Get item option to be applied by current equipping state of InPCClass.
	 * If no such option is found, it returns 0.0 or 1.0 depend on the Option type of how to apply the option.
	 * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	 //static float GetEquippedOptionPointLocalPlayer(EItemOption InOptionToCheck, EPCClass InPCClass = EPCClass::EPC_End);
	 //static float GetEquippedOptionPoint(const TArray<FB2Item>& AllEquipped, const FB2Wing* InWing, EItemOption InOptionToCheck);
	 /** Get accumulated options for any option type from InPCClass's current equip list.
	  * The returned OutAllOptions values are supposed to be final effective value to be applied.
	  * It uses current selected player class if InPCClass is EPC_End, but it is suggested to specify it whenever possible. */
	  //static void GetAllEquippedOptionPointsLocalPlayer(TMap<EItemOption, float>& OutAllOptions, EPCClass InPCClass = EPCClass::EPC_End);
	  //static void GetAllEquippedOptionPoints(const TArray<FB2Item>& AllEquipped, const FB2Wing* InWing, TMap<EItemOption, float>& OutAllOptions);

	  //////////////////////////////////////////////////////////////////////

	  /** Enter one of item modification operation mode, with designating target item's ID */
	void EnterItemOpMode(ELobbyInvenItemOpMode InMode, int64 TargetItemID, EItemInvenType OpenInventoryType = EItemInvenType::EIIVT_End, bool IsBackupRollBackInventoryTab = false);
	/**
	 * Quit item op mode if any, clean up any possible item op mode stuff.
	 * @param bQuitFromOpSucess : True if quitting from item op mode scene and complete view. It will do some additional UI page transition and view handling.
	 */
	void QuitItemOpMode(bool bQuitFromOpSuccess);
	FORCEINLINE ELobbyInvenItemOpMode GetItemOpMode() {
		return CurrentItemOpMode;
	}

	FORCEINLINE void SetFreezeitemOpModeIngredSelection(bool bInFreeze) {
		bFreezeitemOpModeIngredSelection = bInFreeze;
	}

	/** It will return true only when it found valid target item data for EnhancementMode
	 * @param bGetCachedNonUpdatedData : Get the cached data before update, for progress scene and result scene where the actual target data is updated. */
	bool GetItemOPTargetItemData(FB2Item& OutData, bool bGetCachedNonUpdatedData = false) const;
	/** It will return true when inventory is currently one of item Op mode, and InCheckItem's UID is ItemOpTargetItemID */
	bool IsItemOpTargetItem(const FB2Item& InCheckItem);

	FORCEINLINE void SetAutoItemLevelupTotalCost(int32 Cost) {
		AutoItemLevelupTotalCost = Cost;
	}

	//////////////////////////////////////////////////////////////////////
	// Item enhancement interfaces

	FORCEINLINE static int32 GetItemLevelupIngredLimit() {
		return ItemLevelupIngredLimit;
	}
	FORCEINLINE int32 GetCurrentSelectIngredItemNum() {
		return PendingSelectItemOpIngreds.Num();
	}
	int32 GetCurrentItemOpGoldCost() const;
	bool CanAffordCurrentItemOp() const; // Returns false if we ain't got enough gold

	/** See if current selection of enhance ingredients can achieve max enhance level, so no more ingredient selection is needed. */
	bool IsMaxItemLevelUpExpectedByCurrentSelection();

	int32 GetOpTargetItemLevel();
	/** See if InCheckItem's grade is higher than current enhancement target, to suggest not to Enhance using it. */
	bool IsHigherGradeThanEnhanceTarget(FB2Item& InCheckItem);
	bool IsHigherGradeThanEnhanceTarget(const int32 InCheckStarGrade);

	bool IsEnableAutoSelectLevelupItem(const FB2Item& IngredCandidate);
	bool IsEnableItemLevelupIngred(const FB2Item& IngredCandidate); // See if IngredCandid suit for current target's enhance ingredient, considering target item's type and such.
	static bool IsPossibleItemLevelupIngred(const FB2Item& EnhanceTarget, const FB2Item& IngredCandidate); // Is IngredCandidate possible ingredient for EnhanceTarget.
	//////////////////////////////////////////////////////////////////////
	// Common Item Op interfaces

	/** Check if maximum amount of enhancement ingredients are selected. */
	bool IsIngredsFull();

	/** Select an item having ItemInstanceUID to be one of enhancement ingredient.
	 * It will search the item list only current PC's list. */
	bool SelectForIngred(int64 ItemInstanceUID, ELobbyInvenItemOpMode ItemOpMode, bool InForceMenuUpdate = true);
	void UnSelectForIngred(int64 ItemInstanceUID, ELobbyInvenItemOpMode ItemOpMode);
	void UnSelectForIngred(const TArray<int64>& InInstanceUID, ELobbyInvenItemOpMode ItemOpMode);
	void AutoItemUnselect(int64 InGrade);
	void GetAllSelectedItemOpIngreds(TArray<int64>& OutIngredsID); // Get all currently selected enhance ingredient IDs 
	void GetAllSelectedItemOpIngreds(TArray<FB2Item>& OutIngredsData);
	void GetAvailableHammers(FB2Item& TargetItem, TArray<int64>& OutputList);
	void ResetAllIngredsList(); // Not for GhostItemOpIngreds.
	void ResetEnhanceItemDatas();

	/** Get "ghost" item op mode ingredients data (most likely for UI scene), which are cached before they are gone for actual operation. */
	void GetAllGhostIngreds(TArray<FB2Item>& OutGhostData);
	void GetAllGhostIngreds(TArray<int64>& OutGhostData);

	//////////////////////////////////////////////////////////////////////
	// Item upgrade interfaces

	FORCEINLINE int32 GetCurrentUpgradeStoneCost() const {
		return CurrentUpgradeStoneCost;
	}
	int32 HowManyUpgradeStoneForCurrentTarget() const;
	bool HasEnoughUpgradeStoneForCurrentTarget() const;

	// Item Compose interfaces
	bool IsEnableComposeIngred(const FB2Item& Ingred);
	// Item Surpass interfaces
	bool IsEnableSurpassIngred(const FB2Item& Ingred);
	// Item Enhance Interface
	bool IsVisibilityEhanceIngred(const FB2Item& Ingred);
	bool IsEnableEhanceIngred(const FB2Item& Ingred);
	bool IsEnableEhanceChangeIngred(const FB2Item& Ingred);

	void CheckUsableEnhanceProtectionItem();
	bool IsSelectableEnhanceProtectionItem();
	bool IsSelectedEnhanceProtectionItem();
	void SetSelectEnhanceProtectionItemRefId(int32 ProtectionItemRefID);
	bool GetSelectEnhanceProtectionItem(FB2Item& OutData);
	int32 GetEnhanceProtectionItemCount();

	int32 HaveEnoughHammer(const FB2Item& TargetItem) const;
	int32 HowMannyHammer(int64 HammerID) const;

	FORCEINLINE int32 GetCurEnhanceItemSuccessRate() const {
		return CurrentEnhanceItemSuccessRate;
	}
	FORCEINLINE int32 GetCurEnhanceItemScrollCost() const {
		return CurrentEnhanceItemScrollCost;
	}
	FORCEINLINE int32 GetCurEnhanceItemProtectionScrollCost() const {
		return CurrentEnhanceItemProtectionScrollCost;
	}
	FORCEINLINE bool IsSuccessEnhanceItemForResultPresent() const {
		return (DeltaEnhanceLevelForResultPresent > 0);
	}
	FORCEINLINE int32 GetDeltaEnhanceLevelForResultPresent() const {
		return DeltaEnhanceLevelForResultPresent;
	}

	bool IsIngredsAllItemEquiped(EPCClass PCClass, EItemInvenType InvenType);
	bool IsIngredsAllItemLock(EPCClass PCClass, EItemInvenType InvenType);
	bool HasIngredsSameGrade(EPCClass PCClass, EItemInvenType InvenType);
	bool HasPossibleLevelupItem(EPCClass PCClass, EItemInvenType InvenType);
	bool IsIngredsAllMaxLevel(EPCClass PCClass, EItemInvenType InvenType);

	// Server response handlers
private:
	void ResponseRefineTotem(const FB2RefineTotemPtr& RefineTotemInfo);
	void ResponseAddTotemToInventory(const FB2AddTotemToInventoryPtr& AddTotemToInventoryInfo);
	void LevelUpItem(const FB2LevelupItem& EnhancedItemInfo);
	void SurpassedItem(const FB2SurpassItem& SurpassedItemInfo);
	void UpgradedItem(const FB2UpgradeItem& UpgradedItemInfo);
	void ComposedItem(const FB2ComposeItem& ComposedItemInfo);
	void EnhanceItem(const FB2EnhanceItem& EnhancedItemInfo);
	void CostumeEnhanceItem(const FB2ResponseEnhanceCostumePtr& msg);
	void OnItemOpResponseCommon(ELobbyInvenItemOpMode InOpMode);

	void ItemUpgradeCostDelivered(const FB2GetItemUpgradeCost& UpgradeCost);
	void ItemComposeCostDelivered(const FB2GetItemComposeCost& ComposeCost);
	void ItemSurpassCostDelivered(const FB2GetItemSurpassCost& SurpassCost);
	void SetCurEnhanceItemData(const int64 TargetItemID);	// 서버로부터 받지 않는다.

	//////////////////////////////////////////////////////////////////////
	// Auto Enhancement
	void BeginAutoSelectGradeItems(const int32 StarGrade);
	void AutoSelectGradeItems(const int32 StarGrade);
	void AutoSelectGradeItemsForConditions(const int32 StarGrade);

	/** Main entry point of auto enhancement, AllBulkList are just gathered and non-sorted list, which one will be selected as target or ingredients are not decided yet. */
	void BeginAutoLevelupItem(const TArray<FB2Item>& AllBulkList, bool bCostRequestOnly);
	/** Probably be the heart of auto enhancement. It decide which one will be the target and which one will be the ingredient from AllBulkList */
	void SortAutoLevelupItemList(const TArray<FB2Item>& AllBulkList, TArray<FAutoItemLevelupSingleSet>& OutSortedList);

	// Intermediate form of AutoEnhance list sorting. Items in this list supposed to be the same inventory type, which can be the enhance target and/or its ingredient.
	struct FAutoItemLevelupPossibleSet
	{
		FAutoItemLevelupPossibleSet(){}
		TArray<FB2Item> ItemsList;
		TArray<FB2Item> EssenceItemList;
	};

	// Method set for auto enhancement cost request.
	void BeginReqAutoItemLevelupCostWithSortedList(const TArray<FAutoItemLevelupSingleSet>& InSortedList);
	void OnCheckAllAutoItemLevelupCost();

	// Method set for auto enhancement main request
	void BeginReqAutoItemLevelupWithSortedList(const TArray<FAutoItemLevelupSingleSet>& InSortedList); // Kicking off the final enhancement request with sorted list.
	void ReqForSingleAutoItemLevelupList();
	void OnResponseOfSingleAutoItemLevelupList(const FB2LevelupItem& EnhancedItemInfo);
	void ForceEndAutoItemLevelupTimerCB();
	void EndAutoItemLevelup(bool bEndOfCostRequestOnly = false, bool bEndOnOperationSuccess = true);

	//costume
	void ResponseEquipCostumeItems(const FB2EquipCostumePtr& EquipCostumePtr);
	void ResponseUnEquipCostumeItems(const FB2UnequipCostumePtr& UnEquipCostumePtr);

	void ResponseGetChangeItemCharacterTypeCost(const FB2ResponseGetChangeItemCharacterTypeCostPtr& InGetChangeItemCharacterTypeCostPtr);

	void OpenPopupItemLock(const FText& InAskingItemActText, int64 TargetIDUID);

	//Totem
	void ResponseEquipTotems(const FB2EquipTotemsPtr& EquipTotemsPtr);
	void ResponseUnequipTotems(const FB2UnequipTotemsPtr& UnequipTotemsPtr);
	void ResponseLockTotems(const FB2LockTotemsPtr& LockTotemsPtr);
	void ResponseUnlockTotems(const FB2UnlockTotemsPtr& UnlockTotemsPtr);
	void ResponseSellTotems(const FB2SellTotemsPtr& SellTotemsPtr);
	void ResponseDismantleTotems(const FB2DismantleTotemsPtr& DismantleTotemsPtr);

	void ShowTotemRefineOpFx();
	void SellTotemsEndLoop();

public:
	void GetCachedAutoItemLevelupResultItems(TArray<FB2Item>& OutResults);
	/** Sort the BulkList for actual AutoEnhancement then get the expected cost by client preview. */
	void GetAutoItemLevelupPreviewInfo(const TArray<FB2Item>& AllBulkList, int32& Cost, int32& LvUpItemCount, int32& IngredItemCount);

	FORCEINLINE EItemLevelUpJackpotType GetCachedItemLevelupJackpotType() {
		return CachedItemLevelupJackpotType;
	}
	FORCEINLINE bool IsItemLevelupJackpot()
	{
		if (GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Levelup)
			return false;

		return (CachedItemLevelupJackpotType == EItemLevelUpJackpotType::ER_Jackpot) || (CachedItemLevelupJackpotType == EItemLevelUpJackpotType::ER_GreateJackpot);
	}

private:

	//////////////////////////////////////////////////////////////////////

	/** Callback handler to the server's lock/unlock item response.
	 * Lock or unlock (whatever to opposite state) an item */
	void LockedItems(const FB2LockedItems& LockedItems);
	void UnlockedItems(const FB2UnlockedItems& UnlockedItems);

	//////////////////////////////////////////////////////////////////////
	// Item Sell

	/** Callback handler to the server's sell item response.
	 * Sells an item of InstanceUID, one of item removing operation. */
	void SendSellItem();
	void ResponseSellItems(const FB2SoldItems& SoldItems);
	void SellItemsEndLoop();

	void SellConsumableItem(const FB2SellConsumableItem& ConsumableItem);

	TArray<int64> SellItemUIDs;
	EItemLoopType ItemSellLoopType;
	FTimerHandle LoopRequestSellItem;

	int32 GetSellItemGold;

	/* */
	float RequestTime;
	float ResponseTime;

	//////////////////////////////////////////////////////////////////////
	// Item Convert
	void SendConvertData();
	void ResponseConvertData(const FB2ConvertData& ConvertData);

	int32 templateId;
	int32 templateUID;

	int32 convertItemID;
	int32 convertType;

	//UB2LobbyUI_ConvertItemPopup* ConvertPopupPointer;

	void SendConvertItem();
	void ResponseConvertItem(const FB2ConvertItem& ConvertItem);

	//////////////////////////////////////////////////////////////////////
	// Item Decomposition
	void SendDecompositionItem();
	void ResponseDismantleItems(const FB2DismantleItems& DismantleItems);
	void DismantleItemsEndLoop();

	//////////////////////////////////////////////////////////////////////
	// Unseal box
	void ResponseUnsealBox(const FB2ResponseUnsealBoxPtr& UnsealBoxPtr);
	void ResponseSealBoxSelectChar(int32 PCClass);

	TArray<int64> DecompositionItemUIDs;
	EItemLoopType ItemDecompositionLoopType;
	FTimerHandle LoopRequestDecompositionItem;

	TMultiMap<int32, int32> GetDismantleItems;
	TArray<int32> GetDismantleItemsKey;

	bool bUseTotem;
public:

	FORCEINLINE void SetMainInvenViewCurrentTab(EItemInvenType InCurrentTab) {
		MainInvenViewCurrentTab = InCurrentTab;
	}
	FORCEINLINE EItemInvenType GetMainInvenViewCurrentTab() {
		return MainInvenViewCurrentTab;
	}

#if !UE_BUILD_SHIPPING
	static bool IsInventoryTotallyEmpty(); // Not sure if we need to know this other than development purpose.

	/** Continuously request to add specified item for test.
	 * It request item of RefID to add, then set timer to add with one decreased RemainingNum until RemainingNum gets to 0. */
	void DevOnlyRequestDummyItems(int32 RemainingNum, int32 ItemRefID = 1000100, float NextRequestInterval = 0.5f);
	FTimerHandle RequestDummyItemsTH;

	/** Append to current or totally replace all existing options of currently equipped item of InEquipPlace to InOptionsToReplace
	 * to the local player character of InLocalCharClass.
	 * Append of replace depend on bMaintainCurrentOptions.
	 * It does nothing if designated local player character has no item at InEquipPlace.
	 * Any option elements of InOptionsToReplace which exceed the limit of all possible option number will be ignored. */
	static void DevOnlyReplaceEquippedOptions(EPCClass InLocalCharClass, EItemEquipPlace InEquipPlace, const TArray<FItemOption>& InOptionsToReplace, bool bMaintainCurrentOptions = false);

#endif
#if WITH_EDITOR
	uint32 bSelectedNoForDummyDataSuggestion : 1;
	void CheckForGenerateDummyDataSuggestion();

private:
	uint32 bItemOpProgressSceneTestMode : 1;
	/** Generate dummy item data and start item enhancement progress scene. This is test feature for designer's setting. */
public:
	void DevOnlyItemLevelupProgressSceneTest(int32 IngredNum);
	void DevOnlyItemEnhanceProgressSceneTest(int32 IngredNum);
	void DevOnlyItemCUSProgressSceneTest(ELobbyInvenItemOpMode InOpMode); // For compose/upgrade/surpass
#endif

protected:
	//봉인함 클래스 선택 부분
	TWeakObjectPtr<UTextBlock> TB_CharSelectTitle;
	TWeakObjectPtr<UTextBlock> TB_CharSelectFullText;

	TWeakObjectPtr<UB2Button> BTN_CharSelectPopupClose;
	TWeakObjectPtr<UB2Button> BTN_CharSelectPopupGladiator;
	TWeakObjectPtr<UB2Button> BTN_CharSelectPopupAssassin;
	TWeakObjectPtr<UB2Button> BTN_CharSelectPopupWizard;
	TWeakObjectPtr<UB2Button> BTN_CharSelectPopupFighter;
	TWeakObjectPtr<UB2Button> BTN_CharSelectConfirm;

	TWeakObjectPtr<UCanvasPanel> X_CP_UIP_CharSelectPopupPanel;

	TArray<TWeakObjectPtr<UOverlay>>	CharacterButtonOnArr;
	TArray<TWeakObjectPtr<UOverlay>>	CharacterButtonOffArr;

	// Per character..
	//TArray<TWeakObjectPtr<UB2UIStoreProductCost>> UIP_CharSelectPopupCosts;



	int32 SealBoxClass;
	int32 SealBoxRefID;
	int64 SealBoxUID;

	void ConditionalCreateCharSelectPopup();
	void SetButtonLineVisible(EPCClass InPCClass);
	void SetForcedBlockHeader(bool IsBlock);

	UFUNCTION()
	void SelectedGladiator();
	UFUNCTION()
	void SelectedAssassin();
	UFUNCTION()
	void SelectedWizard();
	UFUNCTION()
	void SelectedFighter();
	UFUNCTION()
	void CloseSelectCharacterPopupWindow();
	UFUNCTION()
	void OnClickSelectConfirm();

public:
	//UPROPERTY(EditAnywhere, Category="fdsjkl")
	//	TSoftClassPtr<class UB2UIBackWidgetBase> UIP_CharSelectPopupClass;

	//class UB2UIBackWidgetBase* CreatedUIP_CharSelectPopup;
};
