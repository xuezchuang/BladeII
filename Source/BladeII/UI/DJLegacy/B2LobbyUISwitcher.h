// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "Engine/StreamableManager.h"
#include "B2AssetLoader.h"
#include "../../Common/CommonStruct.h"
#include "B2LobbyUISwitcher.generated.h"

/** UserWidgets considered as one of main pages or shown without any other base page. */
UENUM(BlueprintType)
enum class ELobbyUIPages : uint8
{
	//>> Belows share common UI elements of HeroMgmtCommon.
	ELUP_InventoryMain,
	ELUP_Brevet,
	ELUP_Skill,
	ELUP_Wing,
	ELUP_Relic,
	ELUP_Costume,
	ELUP_Totem,
	ELUP_RelicManagement,
	//<<

	ELUP_ItemLevelUpProgress,
	ELUP_ItemUpgradeProgress,
	ELUP_ItemSurpassProgress,
	ELUP_ItemComposeProgress,
	ELUP_ItemEnhanceProgress,

	ELUP_WingEvolutionProgress,
	ELUP_WingEvolutionResult,

	ELUP_TotemSmeltingProgress,
	ELUP_ItemForge,
	ELUP_SealBoxResult,

	ELUP_End
};

#define ELobbyUIPages_ENUM_PACKAGE_NAME TEXT("ELobbyUIPages")
FString GetUIPageEnumString(ELobbyUIPages UIPageEnum);


FORCEINLINE ELobbyUIPages HeroMgmtSubModeToLobbyUIPage(EHeroMgmtSubMode InMode)
{
	switch (InMode)
	{
	case EHeroMgmtSubMode::EHMSM_Inventory: return ELobbyUIPages::ELUP_InventoryMain;
	case EHeroMgmtSubMode::EHMSM_Skill:		return ELobbyUIPages::ELUP_Skill;
	case EHeroMgmtSubMode::EHMSM_Brevet:	return ELobbyUIPages::ELUP_Brevet;
	case EHeroMgmtSubMode::EHMSM_Wing:		return ELobbyUIPages::ELUP_Wing;
	case EHeroMgmtSubMode::EHMSM_Relic:		return ELobbyUIPages::ELUP_Relic;
	case EHeroMgmtSubMode::EHMSM_Costume:	return ELobbyUIPages::ELUP_Costume;
	case EHeroMgmtSubMode::EHMSM_Totem:		return ELobbyUIPages::ELUP_Totem;
	case EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE: return ELobbyUIPages::ELUP_RelicManagement;
	}
	return ELobbyUIPages::ELUP_End;
}

FORCEINLINE EHeroMgmtSubMode LobbyUIPageToHeroMgmtSubMode(ELobbyUIPages InUIPage)
{
	switch (InUIPage)
	{
	case ELobbyUIPages::ELUP_InventoryMain: return EHeroMgmtSubMode::EHMSM_Inventory;
	case ELobbyUIPages::ELUP_Skill:			return EHeroMgmtSubMode::EHMSM_Skill;
	case ELobbyUIPages::ELUP_Brevet:		return EHeroMgmtSubMode::EHMSM_Brevet;
	case ELobbyUIPages::ELUP_Wing:			return EHeroMgmtSubMode::EHMSM_Wing;
	case ELobbyUIPages::ELUP_Relic:			return EHeroMgmtSubMode::EHMSM_Relic;
	case ELobbyUIPages::ELUP_Costume:		return EHeroMgmtSubMode::EHMSM_Costume;
	case ELobbyUIPages::ELUP_Totem:			return EHeroMgmtSubMode::EHMSM_Totem;
	case ELobbyUIPages::ELUP_RelicManagement: return EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE;
	}

	return EHeroMgmtSubMode::EHMSM_End;
}

/** User widgets defined by this are pop-ups reside in one of LobbyUIPages. */
UENUM(BlueprintType)
enum class ELobbySubPopups : uint8
{
	ELSPU_StoredItemDetail,
	ELSPU_EquippedItemDetail,
	
	ELSPU_ExtendInventory,
	ELSPU_SellAllItems,
	ELSPU_DecompositionAllItems,
	ELSPU_AutoEquip,
	ELSPU_AutoItemLevelup,
	ELSPU_AutoItemLevelupResult,
	ELSPU_CharStatDetail,
	ELSPU_CharStatDetailPopup,

	// Item OP (Levelup, upgrade stuff) menus are all pop-ups of inventory menu.
	ELSPU_ItemLevelUpMain,
	ELSPU_ItemUpgradeMain,
	ELSPU_ItemSurpassMain,
	ELSPU_ItemComposeMain,
	ELSPU_ItemEnhanceMain,
	ELSPU_ItemEnhanceChangeMain,
	ELSPU_ItemHeroChangeMain,

	ELSPU_AcqItemEquipSuggest,
	ELSPU_SellItemPopup,

	ELSPU_EnhanceItemSetEffectPopup,

	ELSPU_WingPropOptionEnhance,

	ELSPU_ItemAfterOpiton,

	ELSPU_ConvertItemPopup,

	ELSPU_QualityChange,
	ELSPU_QualityChangeHammerSelect,

	ELSPU_SealBoxResult,

	ELSPU_End
};

/** Provide mapping between LobbyUIPages enum and its relevant user widget class, for configuration. */
USTRUCT(BlueprintType)
struct FLobbyUIPageClassMapping
{
	GENERATED_USTRUCT_BODY()

	FLobbyUIPageClassMapping()
	{
		PageEnum = ELobbyUIPages::ELUP_End;
		bCacheOnLoad = false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	ELobbyUIPages PageEnum;

private:
	/** If true, the loaded PageWidgetClass will be cached to skip loading at next time, until forced to unload all. */
	uint32 bCacheOnLoad : 1;
public:
	FORCEINLINE void SetCacheOnLoad(bool bInSet) { bCacheOnLoad = bInSet; }
	FORCEINLINE bool ShouldCacheOnLoad() const { return bCacheOnLoad; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSoftClassPtr<class UB2LobbyUserWidget> PageWidgetClass;
};

/** Similar to the FLobbyUIPageClassMapping, for SubPopups, instead of the main UI page. */
USTRUCT(BlueprintType)
struct FLobbySubPopupClassMapping
{
	GENERATED_USTRUCT_BODY()

	FLobbySubPopupClassMapping()
	{
		SubPopupEnum = ELobbySubPopups::ELSPU_End;
		ZOrder = 2; // by default, set it to be above the main page.
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	ELobbySubPopups SubPopupEnum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSoftClassPtr<class UB2LobbyUserWidget> SubPopupClass;

	/** Need to set Z order to handle multiple pop-ups within single page. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 ZOrder;
};

/**
 * To set UserWidget BP classes to be used for lobby UI, and provide some of UI flow interfaces.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2LobbyUISwitcher : public UObject, public IB2AsyncInterface
{
	GENERATED_BODY()
	

protected:

	//////////
	// Declares widget class properties here..
	// They will be picked up to create relevant widget in here.
	
	/** Main settings here, classes for Lobby UI pages. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<FLobbyUIPageClassMapping> LobbyPageClassMapping;

	/** Class settings for sub pop-ups, which reside in one of Lobby (main) UI pages. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<FLobbySubPopupClassMapping> LobbySubPopupClassMapping;

	/** DynItemIcon classes are a little specially here. They were initially referenced by other WidgetBP which will be the parent widget of created item icons,
	 * but they are now created from different place.. for widget pool */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSoftClassPtr<class UB2DynItemIcon> InvenStoredItemIconClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSoftClassPtr<class UB2DynItemIcon> InvenEquippedItemIconClass;

	//////////
	/** Some widget data will be added at Rootset on first-load to reduce loading time.. even more than CacheOnLoad */
	static TArray<ELobbyUIPages> RootSetPages;
	static TArray<ELobbyUIPages> LoadOnStartupPages; // They are also RootSet, and even being loaded from the startup.
	
public:

	UB2LobbyUISwitcher(const FObjectInitializer& ObjectInitializer);

	void InitUISwitcher(class AB2LobbyGameMode* InLobbyGM);
private:
	void SetupCacheOnLoadList();
	void SetupRoosetPagesList();
	void SetupLoadOnStartupPagesList(class UB2UIManager_Lobby* InOwnerUIManager);
public:

	static bool IsRootSetPage(ELobbyUIPages InCheckPage);

	FLobbyUIPageClassMapping*	GetUIPageMapping(ELobbyUIPages InPageEnum);
	FLobbySubPopupClassMapping*	GetSubPopupMapping(ELobbySubPopups InSubPopupEnum);

	TSubclassOf<class UB2LobbyUserWidget>		GetWidgetClass(ELobbyUIPages UIPage);
	TSoftClassPtr<class UB2LobbyUserWidget>	GetWidgetClassAsset(ELobbyUIPages UIPage);

	TSubclassOf<class UB2LobbyUserWidget>		GetSubPoupClass(ELobbySubPopups SubPopup);
	TSoftClassPtr<class UB2LobbyUserWidget>	GetSubPopupClassAsset(ELobbySubPopups SubPopup);

	UClass* GetInventoryItemIconClass(bool bIsForEquipped);
	int32	GetSubPopupZOrder(ELobbySubPopups SubPopup);

	void	UnloadTAssets(bool bUnloadeAllCached, UB2UIManager_Lobby* InOwnerUIManager);

	
	/** Unload especially for TSoftClassPtr */
	//void UnloadTAssets(bool bUnloadeAllCached, class UB2UIManager_Lobby* InOwnerUIManager);

#if WITH_EDITOR
	/** For safe use of TSoftClassPtr in editor. */
	void EditorLoadAll();
#endif
};
