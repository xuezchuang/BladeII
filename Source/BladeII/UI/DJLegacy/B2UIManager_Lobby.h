// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "InfoAsset/B2PCClassInfo.h"
#include "B2LobbyUISwitcher.h"
#include "Quest.h"
#include "B2UIEnum.h"
#include "CommonStruct.h" 
#include "B2WidgetPool.h"
#include "B2AssetLoader.h"
#include "B2UIManager_Lobby.generated.h"

enum class ELobbyOnHeadDisplayType
{
	// Some differences of UB2FloatingLobbyOnHeadDisplay according to this.

	LOHDT_LobbyMain,
	LOHDT_CharObserve
};

/** Utils provided for mapping between some dummy EUIscene and DJLegacy hero management lobby UI */
FORCEINLINE EUIScene DJLegacyHeroMgmtPageToUIScene(EHeroMgmtSubMode InDJLegacyHeroMgmtMode)
{
	switch (InDJLegacyHeroMgmtMode)
	{
	case EHeroMgmtSubMode::EHMSM_Inventory: return EUIScene::Inventory;
	case EHeroMgmtSubMode::EHMSM_Skill: return EUIScene::Skill;
	case EHeroMgmtSubMode::EHMSM_Brevet: return EUIScene::Brevet;
	case EHeroMgmtSubMode::EHMSM_Wing: return EUIScene::Wing;
	case EHeroMgmtSubMode::EHMSM_Relic: return EUIScene::Relic;
	case EHeroMgmtSubMode::EHMSM_Costume: return EUIScene::Costume;
	case EHeroMgmtSubMode::EHMSM_Totem: return EUIScene::Totem;
	case EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE: return EUIScene::RelicManagement;
	}
	// Don't forget for new entry
	checkSlow(static_cast<int32>(EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE) + 1 == static_cast<int32>(EHeroMgmtSubMode::EHMSM_End));
	return EUIScene::Max;
}
FORCEINLINE EHeroMgmtSubMode UISceneToDJLegacyHeroMgmt(EUIScene InUIScene)
{
	switch (InUIScene)
	{
	case EUIScene::Inventory: return EHeroMgmtSubMode::EHMSM_Inventory;
	case EUIScene::Skill: return EHeroMgmtSubMode::EHMSM_Skill;
	case EUIScene::Brevet: return EHeroMgmtSubMode::EHMSM_Brevet;
	case EUIScene::Wing: return EHeroMgmtSubMode::EHMSM_Wing;
	case EUIScene::Relic: return EHeroMgmtSubMode::EHMSM_Relic;
	case EUIScene::Costume: return EHeroMgmtSubMode::EHMSM_Costume;
	case EUIScene::Totem: return EHeroMgmtSubMode::EHMSM_Totem;
	case EUIScene::RelicManagement: return EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE;
	}
	// Don't forget for new entry
	checkSlow(static_cast<int32>(EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE) + 1 == static_cast<int32>(EHeroMgmtSubMode::EHMSM_End));
	return EHeroMgmtSubMode::EHMSM_End;
}
void OpenDJLegacyHeroMgmtPageForUIScene(EUIScene InUIScene);

/**
 * Optional information data struct, to put DJLegacy sub popup, opened by DJLegacy_OpenLobbySubPopup, 
 * in a panel widget, instead of a viewport.
 */
USTRUCT()
struct FDJLegacySubpopupAddPanelInfo
{
	GENERATED_USTRUCT_BODY()

	FDJLegacySubpopupAddPanelInfo(class UB2UnitedWidgetBase* InOwner, class UPanelWidget* InParent);

	FDJLegacySubpopupAddPanelInfo()
	{
		OwnerWidget = nullptr;
		ParentPanel = nullptr;
	}
protected:
	UPROPERTY(Transient)
	class UB2UnitedWidgetBase* OwnerWidget;
	UPROPERTY(Transient)
	class UPanelWidget* ParentPanel;
public:
	bool AddWidgetToThisInfo(class UB2LobbyUserWidget* InCreatedWidget);
	FORCEINLINE bool HasValidInfo() const { return (OwnerWidget && ParentPanel); }
};

DECLARE_DELEGATE(FDJLegacyUIManager_OnAsyncLoadCompleteExec)

/**
 * Manager and event messenger for lobby DJLegacy UI..
 */
UCLASS()
class BLADEII_API UB2UIManager_Lobby : public UObject, public IB2AsyncInterface
{
	GENERATED_BODY()

	/** Must be consistent while this UIManager is alive after InitUIManager */
	UPROPERTY(Transient)
	class AB2LobbyGameMode* OwnerLobbyGM;
	
	UPROPERTY(Transient)
	class APlayerController* CachedPlayerController; // Better not be BladeIIPlayerController.

	/** All actual widget classes and their created objects are here. */
	UPROPERTY()
	class UB2LobbyUISwitcher* LobbyUIAssetInfo;
	
	/** Current active UI page created by LobbyUISwitcher. Will be keep changing along the UI page transition. */
	UPROPERTY(Transient)
	class UB2LobbyUserWidget*	CurrentActiveUIPage;

	ELobbyUIPages				CurrentPageEnum;

	/** Cached LobbyUIPages that have bCacheOnLoad checked. */
	UPROPERTY(Transient)
	TMap<ELobbyUIPages, TSubclassOf<class UB2LobbyUserWidget>> CachedLobbyUserWidgetClasses;

	/** Specially referenced here. They are also loaded from LobbyUISwitcher reference. */
	UPROPERTY(Transient)
	TSubclassOf<class UB2DynItemIcon> CachedInvenStoredItemIconClass;
	UPROPERTY(Transient)
	TSubclassOf<class UB2DynItemIcon> CachedInvenEquippedItemIconClass;
	/** Widget pools containing dynamically created item icons in inventory view */
	UPROPERTY(Transient)
	FInventoryDynItemIconPool StoredItemIconWidgetPool;
	UPROPERTY(Transient)
	FInventoryDynItemIconPool EquippedItemIconWidgetPool;

	/** All sub pop-ups created during current main page (CurrentActiveUIPage) by LobbyUISwitcher.
	 * They are destroyed whenever CurrentActiveUIPage is changed by BeginLobbyUIPage */
	TMap<ELobbySubPopups, UB2LobbyUserWidget*> CreatedSubPopups;
	
	friend class UB2LobbyUISwitcher; // To provide access to LobbyUserWidgets placed here.

	/** Character level information stuff attached on character head. */
	UPROPERTY()
	TSubclassOf<class UB2FloatingLobbyOnHeadDisplay> OnHeadDisplayWidgetClass;
	UPROPERTY(Transient)
	TArray<class UB2FloatingLobbyOnHeadDisplay*> CreatedOnHeadDisplays;

public:

	UB2UIManager_Lobby(const FObjectInitializer& ObjectInitializer);

	virtual bool	InitUIManager(class AB2LobbyGameMode* InLobbyGM);

	virtual void	BeginDestroy() override;
	void			OnPreLoadMap();

	void			UpdateManual(float DeltaSeconds); // Try do some stupid stuff now..

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

	uint32 bEventsSubscribed : 1; // To un-subscribe only in subscribed GameMode object.

	uint32 DJLegacy_ChangeLobbyUIPageTicket;
	uint32 DJLegacy_OpenLobbySubPopupTicket;
	uint32 DJLegacy_CloseLobbySubPopupTicket;
	uint32 LobbyPCSelectionChangedTicket;
	uint32 LobbyUpdateAllItemIconsTicket;
	uint32 LobbyTryUpdateSingleItemIconTicket;
	uint32 LobbyPopupCreateAccountTicket;
	uint32 LobbyTryUpdateItemOPMainMenuTicket;
	uint32 LobbyInvenSetItemOpModeGuideTextTicket;
	uint32 LobbyInvenHideItemOpModeGuideTextTicket;
	uint32 LobbyExternalSetInventoryTabTicket;
	uint32 CreateLobbyCharOnHeadDisplayTicket;
	uint32 DestroyAllLobbyCharOnHeadDisplayTicket;
	uint32 UpdateLobbyCharOnHeadDisplayTicket;
	uint32 LobbyPlayUIEquipEffectTicket;
	uint32 LobbyUpdateWingMainUITicket;
	uint32 LobbyNoticePopUpTicket;
	uint32 DeliveryGetQuestUITicket;
	uint32 DeliveryAcceptQuestTicket;
	uint32 DeliveryQuestRewardTicket;
	uint32 DeliveryGetAccountRelicTicket;
	uint32 DeliveryEnhanceRelicTicket;
	uint32 DeliveryPromotionRelicTicket;
	uint32 OnQuestCompleteNotifyTicket;

	TArray<b2network::B2RewardPtr> ToViewReward;

	// Quest, Attendnace
	enum class NoticeState : uint32
	{
		NONE,
		ENABLE,
		DISABLE,
	};

	NoticeState QuestNoticeState;
	NoticeState AttendanceNoticeState;

	TArray<b2network::B2QuestInfoPtr> QuestNoticeList;
	const TArray<b2network::B2QuestInfoPtr>& GetQuestNoticeList() const { return QuestNoticeList; }
	
	bool IsOpenDailyQeustDialog;

	static const FString AsyncOpenUIStr;

private:

	void ClearLobbyUIPage();


public:
	/** A simple wrapper to LobbyUISwitcher's main functionality. */
	void DJLegacy_ChangeLobbyUIPage(ELobbyUIPages InPageEnum);
	void DJLegacy_OpenLobbySubPopup(ELobbySubPopups InPopupEnum, 
		// You may optionally give parent panel information, to treat the created popup widget as a UIP of owner user widget.
		FDJLegacySubpopupAddPanelInfo InOptionalAddPanelInfo = FDJLegacySubpopupAddPanelInfo()
	);
	void DJLegacy_CloseLobbySubPopup(ELobbySubPopups InPopupEnum);
	class UB2LobbyUserWidget* DJLegacy_FindLobbySubPopup(ELobbySubPopups InPopupEnum);
	
	void ReqAsyncLoadUIPageAsset(ELobbyUIPages InLoadUIPage, bool bBlockingOpenUI = false);
	bool TryAsyncLoadOpenUI(ELobbyUIPages InLoadUIPage, bool bBlockingOpenUI);

	void CloseLobbySubPopup(ELobbySubPopups InPopupEnum);
	void OpenItemDetailPopup(int32 InUID);

protected:

	// 일정시간 이상 진행시 Flush & SynchronousLoad 하는 부분이 필요

	void		 ReqAsyncLoadUIPageAssets(const TArray<ELobbyUIPages>& InLoadUIPages, bool bBlocking);

	virtual void OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FB2AsyncRequestInfo>& CompleteRequestList);
	void		 OnLoadedLobbyWidget(ELobbyUIPages UIPage, TSubclassOf<UB2LobbyUserWidget>&  LoadedClass);

	bool		 IsOpenUIRequest(const FString& RequestName);
	FString		 GetAsyncRequestName(ELobbyUIPages UIPage, bool bOpenUI);

	void		 RemoveCachedWidgetClasse(ELobbyUIPages UIPageEnum);

	void		 RemoveCachedWidgetClasses();
	void		 RemoveCachedSubWidgetClasses();

public:
	UClass* GetInventoryItemIconClass(bool bIsForEquipped);
private:
	void InitializeDynItemIconWidgetPool();
	void DestroyDynItemIconWidgetPool();
	FInventoryDynItemIconPool& GetMatchingDynItemIconWidgetPool(bool bIsForEquipped);
public:
	/** Interfaces for using DynItemIcon widget pool, reducing inventory view performance. */
	class UB2DynItemIcon* RentInvenDynItemIconWidget(bool bIsForEqupped, class APlayerController* InPC, class UB2UnitedWidgetBase* InRentOwner, class UPanelWidget* InIconPlacePanel);
	void TurnInDynItemIconWidget(bool bIsForEqupped, class UB2DynItemIcon* InUsedOne);

	/** Mostly originate from LobbyUI_CharSel widget.. rather than from the game code. */
	void NotifyPCSelectionChanged(EPCClass InSelectedPCClass);

	/** It can be called in various circumstances, when an item's certain state is changed, remove or added, etc..
	 * Request for this call in any case requiring inventory item icon update. */
	void TryUpdateAllVisibleItemIcons();
	/** For optimization, it only update one item icon, displaying item of ItemInstanceUID.
	 * It will do nothing if there's no such currently visible icon of ItemInstanceUID */
	void TryUpdateSingleVisibleItemIcon(int64 ItemInstanceUID);

	void TryPopupCreateAccountMenu();
	void TryUpdateItemOPMainMenu();

	/** They will work only when Lobby inventory page is open */
	void SetInventoryItemOpModeGuideText(const FText& InText); // Set and display
	void HideInventoryItemOpModeGuideText();

	EItemInvenType GetInventoryTabSelection(); // Get valid data only when inventory page is open.
	void ExternalSetInventoryTab(EItemInvenType InNewTab, bool bDisableOtherTabSelection = false);
	
	class UB2FloatingLobbyOnHeadDisplay* CreateLobbyCharOnHeadDisplay(EPCClass InCharacerClass, class ASkeletalMeshActor* InLobbyChar, ELobbyOnHeadDisplayType InMasterType);
	void DestroyAllLobbyCharOnHeadDisplay();
	void UpdateLobbyCharOnHeadDisplay();

	void PlayUIEquipEffect(EItemEquipPlace InEquippedPlace);

	void UpdateWingMainUI();
	void BeginShowWingEvolutionSuccessMessage();
	void BeginShowWingEvolutionFailMessage();
	void PlayWingEnhancedAnim();
	void UpdateOnWingAutoEnhanceComplete();
	void UpdateOnWingAutoEvolveComplete();
	void PopUpLobbyNotice(int32 NoticeIndex); // todo modify

	FORCEINLINE class UB2LobbyUISwitcher* GetLobbyUIAssetInfo() { return LobbyUIAssetInfo; }

	void ResponseGetQuest(const FB2GetQuest& Quest);
	void ResponseAcceptQuest(const FB2AcceptQuest& Quest);
	void ResponseGetQuestReward(const Quest& CompleteQuest, const TArray<b2network::B2RewardPtr>& Reward);
	void ResponseGetAccountRelic(const FB2ResponseGetAccountRelicPtr& msgPtr);
	void ResponseEnhanceRelic(const FB2ResponseEnhanceRelicPtr& msgPtr);
	void ResponsePromotionRelic(const FB2ResponsePromotionRelicPtr& msgPtr);
	void ResponseQuestComplete(int32 QuestSlot);
	void OnClickMsgReward(int32 QuestSlot);


	FName GetDJLegacyUIName(UB2UnitedWidgetBase* WidgetBase);
	class UB2UnitedWidgetBase* GetCurrActiveUI();
};
