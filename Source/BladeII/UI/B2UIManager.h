// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "UMG.h"
#include "Blueprint/UserWidget.h"
#include "B2UIEnum.h"
#include "Engine/StreamableManager.h"
#include "B2UIWidget.h"
#include "B2UIMsgPopupSimple.h"
#include "B2UIMsgPopupItemCostConfirm.h"
#include "Tickable.h"
#include "BladeIIUtil.h"
#include "B2AssetLoader.h"
#include "B2UIEnum.h"
#include "Framework/Application/IInputProcessor.h"
#include <memory>
#include "B2UIManager.generated.h"

#define ShortageMGR UB2UIManager::GetInstance()->GetShortagePopupMGR()

/** It provides the data you need to know to configure the Widget.
UIName, UILayer, UIClass */
USTRUCT(BlueprintType)
struct FB2UIWidgetData
{
	GENERATED_USTRUCT_BODY()

	FB2UIWidgetData() :WidgetFName(NAME_None)
		, Layer(EUILayer::WidgetScenes)
		, bStayInViewport(0)
		, bCacheOnLoad(0)
		, bCacheByHistory(0)
		, bBindDocAutoOnOpen(0)
		, Widget(nullptr)
		, bAllowWorldBackgroundBlur(0)
		, WorldBackgroundBlurScale(1.f)
		, bHideBattleRelatedWidgets(0)
		, IsBattleRelatedWidget(0)
	{}

	/** New independent UI widget identification. Not available for manual setting. It will come from key value of a TMap element */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BladeII")
	FName WidgetFName;

	/*The layer hierarchy representing the zorder.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	EUILayer Layer;

	/* It will stay loaded once being opened. Just hidden on CloseUI call and will be destroyed on level unload. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	uint32 bStayInViewport : 1;

	/** If set, the TAsset data is cached to prevent unloading.
	 * Behaves somewhat similar to bStayInViewport, but just caches TAsset, not maintaining UI open state.
	 * For now, it is dynamically managed flag.  */
	uint32 bCacheOnLoad : 1;

	/**
	 * Another widget data caching flag, but has nothing to do with bCacheOnLoad.
	 * It is being set by UI open history, first-in-first-out manner.
	 */
	uint32 bCacheByHistory : 1;

	/* Whether to automatically bind when the UI is opened. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	uint32 bBindDocAutoOnOpen : 1;

protected:
	/* The TAssetSubclass of widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSoftClassPtr<class UB2UIWidget> WidgetClass;
public:

	FName GetWidgetAssetName();

	FORCEINLINE TSoftClassPtr<class UB2UIWidget> GetWidgetClassAsset() const {
		return WidgetClass;
	}


	/** It can be used for WidgetCalss loading */
	FORCEINLINE FSoftObjectPath GetWidgetClassStringAssetReference() const {
		return WidgetClass.ToSoftObjectPath();
	}
	FORCEINLINE bool IsWidgetClasseLoaded() const {
		return WidgetClass.IsValid();
	}

	/** If this returns true, the loaded widget data can be explicitly unloaded.
	 * It might still not be unloaded though, if it's in the root-set. */
	FORCEINLINE bool CanUnloadByPolicy() const {
		return !bCacheOnLoad && !bCacheByHistory;
	}
	void ResetAllCacheMarkForUnload();

	////////////////////////////////////////////////////////////////////////////////
	// 
	// REMARK!
	// This Widget reference must be managed carefully.
	// FB2UIWidgetData is an element of B2UIManager which is essentially a Blueprint asset.
	// It must be removed on level change. Refer to UB2UIManager::CloseAllUIs and ABladeIIGameMode::OnPreLoadMap
	// 
	/* The ref of widget. */
	class UB2UIWidget* Widget;
	////////////////////////////////////////////////////////////////////////////////

	FName WidgetAssetName;

	/** Allows blurring on background rendered image using DOF. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII WorldBackgroundBlur")
	uint32 bAllowWorldBackgroundBlur : 1;

	/** How much blur the WorldBackgroundBlur will be in general?
	* -> It does not really working for mobile. Looks like due to performance consideration. Just a dummy variable for now.. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII WorldBackgroundBlur")
	float WorldBackgroundBlurScale;

	/** When a widget appeared whether to hide the widgets related to combat. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII WorldBackgroundBlur")
	uint32 bHideBattleRelatedWidgets : 1;

	/** Whether this UI is related to the combat? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII WorldBackgroundBlur")
	uint32 IsBattleRelatedWidget : 1;
};


/** It provides the data you need to know to configure the UIScene. */
USTRUCT(BlueprintType)
struct FB2UISceneData
{
	GENERATED_USTRUCT_BODY()

	FB2UISceneData()
	{
	}

	/** Not available for manual setting. It will come from key value of a TMap element */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BladeII")
	EUIScene SceneName;

	/* The name of the widget composing the scene. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<FName> WidgetFNames;

	/** Whether this UIScene is related to the combat? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	uint32 IsBattleRelatedScene : 1;
};

struct FB2ErrorCode
{
	FB2ErrorCode(int32 InErrorCode, int64 InTimeStame)
		: ErrorCodeNum(InErrorCode)
		, ErrorTimeStamp(InTimeStame)
	{}

	int32 ErrorCodeNum;
	int64 ErrorTimeStamp;
};

class B2BlockClickEvent : public IInputProcessor
{
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override { }
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override {
		return true;
	}
};


DECLARE_DELEGATE(FB2UISceneOnOpen)

/*
* B2 UIManager(Singleton).
*/

//TArray<TPair<EUIScene, FB2UISceneOnOpen>> UISceneLobbyHistory;

UCLASS(BlueprintType, Blueprintable, Transient)
class BLADEII_API UB2UIManager : public UObject, public FTickableGameObject, public IB2AsyncInterface
{
	GENERATED_BODY()

public:
	/* The class of widget's top-level containers. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII UIMain")
	TSubclassOf<UUserWidget> UIMainClass;

	FORCEINLINE class UB2UIMain* GetMainWidget() const {
		return MainWidget;
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII UIMain")
	TSubclassOf<UB2UnitedWidgetBase> RedDotWidgetClass;

protected:
	/* The reference of widget's top-level containers. */
	UPROPERTY()
	class UB2UIMain* MainWidget;

	/** It must not be created for shipping build */
	UPROPERTY(Transient)
	class UB2DebugTextWidget* ScreenDebugTextWidget;

	UPROPERTY(Transient)
	TMap<FName, UClass*>	CachedWidgetClasses;

	/**
	 * The main data set of UIManager.
	 * Registered widget data here can be opened solely or can be opened as a part of UIScene.
	 * The key of this map and WidgetFName member of FB2UIWidgetData supposed to be the same.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII WidgetDatas")
	TMap<FName, FB2UIWidgetData> AllWidgetData;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII SceneData")
	TMap<EUIScene, FB2UISceneData> AllSceneData;

	/* Used as a general-purpose text color : When used it un satisfies certain conditions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Common TextColor")
	FSlateColor TextColor_NonSatisfy;

	/* Used as a general-purpose text color : When used it satisfies certain conditions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Common TextColor")
	FSlateColor TextColor_Satisfy;

	/* Used as a general-purpose text color : When the value is increased using a color (positive). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Common TextColor")
	FSlateColor TextColor_Increase;

	/* Used as a general-purpose text color :  When the value is reduced using the color(negative). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Common TextColor")
	FSlateColor TextColor_Decrease;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII Common TextColor")
	FSlateColor TextColor_Same;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Common TextColor")
	FSlateColor TextColor_Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Common TextColor")
	FSlateColor TextColor_Esay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Common TextColor")
	FSlateColor TextColor_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Common TextColor")
	FSlateColor TextColor_Hard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Common TextColor")
	FSlateColor TextColor_CanBuyItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Common TextColor")
	FSlateColor TextColor_CannotBuyItem;

public:
	/* The interface of the singleton pattern. */
	static class UB2UIManager* GetInstance();
	FORCEINLINE static bool IsInstanceNull() {
		return (Instance == nullptr);
	}
#if WITH_EDITOR
	static class UB2UIManager* EditorGetNewInstance();
#endif
	virtual void PostLoad() override;
#if WITH_EDITOR 
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	void EditorCheckForWidgetNameIntegrity();
	void EditorPIEEnd();
#endif

#if 0
	void SyncPreloadTAssets(const TArray<FName>& InPreloadUIName); // Preload in synchronous manner
#endif
	void ReqAsyncLoadAsset(const FName& InLoadUI);
	void ReqAsyncLoadAssets(const TArray<FName>& InLoadUIs); // Async loading of UIs are not for Pre-load purpose.

	/** Close all UI pages currently open. Need to do this before destroying world, but not during BeginDestroy.
	 * It will close UIs with bStayInViewport option. */
	void CloseAllUIs();

	/** Close all UI pages, which is not a part of current UI scene, i.e. close all opened by direct OpenUI call or MsgPopup stuff.
	 * It will close UIs with bStayInViewport option. */
	void CloseAllStandaloneUIs();

	void Destroy();

	/** This is the one need to be called at in-game level transition. */
	void MasterDestroy();

	/*It is initialized once the first.*/
	void Init(const FLocalPlayerContext& InPlayerContext);

	/** UObject interface  */
	virtual class UWorld* GetWorld() const override;
	/** Sets the player context associated with this UI. */
	void SetPlayerContext(const FLocalPlayerContext& InPlayerContext);


	FB2UIWidgetData* GetWidgetData(FName InUIName);

	/** Slower than other GetWidgetData accepting UIName, iterating all WidgetData. */
	FB2UIWidgetData* GetWidgetDataByWidget(class UB2UIWidget* Widget);
	FB2UISceneData* GetUISceneData(EUIScene UISceneName);


	/** Start WorldBackgroundBlur if bAllowWorldBackgroundBlur is true. It can fail in some circumstances, then it returns false. */
	void StartWorldBackgroundBlur(float WorldBackgroundBlurScale);
	/** Stop WorldBackgroundBlur, automatically called when it is destroyed. */
	void StopWorldBackgroundBlur();

	/** Ability to hide the widget when the widget associated with the battle appeared.. */
	void HideBattleRelatedWidgets(bool bHide);

	/* It calls from ABladeIIPlayerController::SetCinematicMode */
	void SetHUDHidingCinematicMode(bool bInCinematicMode, bool bIsHUDHiding);

	/* This is a helper that the format change beginning with 00:00:00( or 0:0:0 or 00:00 or 0:0 or 00 or 0). */
	static FString FormatSecondsAsTime(int32 Secs, FString Format);
	static FString FormatMilliSecondsAsTime(int32 MilliSecs, FString Format);

	static int64	RemainTimeFunction(int64 InEndTime);

	UTexture2D* LoadTexture2D(const TCHAR* Name);

	void SetMainWidgetServerVersionText(FText ftServerInfoText);
	void ShowMainWidgetVersion();

	static void BlockUIClickEvent(bool bBlock);

	//>> UIScene ==================================================================================
public:
	/*
	 * UIScene = UIGroup. Reducing the overhead of programmers know all the UI.
	 * @param SceneName : The name of the group that assembled UI.
	 * @param TransType : When switching control enables the transition type of a Scene.
	 * @param UISceneOnOpenDelegate : If there is work to be processed with Scene Switch
	 * @param DontCloseSameUIAtReservedUIScene :If you don't want close UI when overlaps in the reserved scene.
	 * @param bPassBattleUIScene :Scene opens the way to find the next time you want to pass the Scene related to combat.
	 */
	void ChangeUIScene(EUIScene SceneName, EUIChangeUISceneTransitionType TransType = EUIChangeUISceneTransitionType::NO_CANCEL, const FB2UISceneOnOpen& UISceneOnOpenDelegate = 0, bool DontCloseSameUIAtReservedUIScene = true, bool bPassBattleUIScene = true);

	/* Save the scene and switching to the previous history is easier when turning. */
	void ChangeUISceneBack(EUIChangeUISceneTransitionType TransType = EUIChangeUISceneTransitionType::NO_CANCEL, const FB2UISceneOnOpen& UISceneOnOpenDelegate = 0, bool DontCloseSameUIAtReservedUIScene = true, bool bPassBattleUIScene = true);

	EUIScene GetPrevUIScene();
	EUIScene GetPrevUIScenePassedBattle();
	bool IsUIScene(EUIScene SceneName) {
		return CurrUIScene == SceneName;
	}
	void RemoveUISceneHistory(EUIScene SceneName);
	void ClearUISceneHistory();
	/** It is "artificial", not by natural scene open. */
	void ArtificialAddUIHistory(EUIScene AddSceneName);

	FORCEINLINE EUIScene GetCurrUIScene() {
		return CurrUIScene;
	}
	FORCEINLINE bool IsOpeningDJLegacyHeroMgmtBySceneHistory() const {
		return bOpeningDJLegacyHeroMgmtBySceneHistory;
	}

	bool HasUISceneHistory(EUIScene SceneName);

	const TArray<TPair<EUIScene, FB2UISceneOnOpen>>& GetUISceneHistory() {
		return UISceneHistory;
	}

private:
	void OpenUIScene(EUIScene SceneName, const FB2UISceneOnOpen& UISceneOnOpenDelegate, bool RightNow = false, bool DontOpenSameUIAtReservedUIScene = true);
	void CloseUIScene(EUIScene SceneName, bool RightNow = false, bool DontCloseSameUIAtReservedUIScene = true);
	void CloseUIScene(bool RightNow = false) {
		CloseUIScene(CurrUIScene, RightNow);
	}
	void CheckClosingCurrUISceneAndChangeToReservedUIScene(); //You should check on the Tick. When you close the UI that corresponds to the current scene
	void OpenNextUIScene();
	void InitNotAddUISceneHistoryList();
	bool NotAddUISceneHistoryCheck(EUIScene SceneName);

	EUIScene CurrUIScene;
	EUIScene ClosingUIScene;
	EUIScene OpeningUIScene;

	bool bDontCloseSameUIAtReservedUIScene;						//If you don't want close UI when overlaps in the reserved scene.
	bool bClosingUIScene;										//Close whether the current scene?
	bool bOpeningUIScene;										//Open whether the current scene?
	EUIChangeUISceneTransitionType ChangeUISceneTransitionType; //There is a storage UISceneTransitionType. When the current scene transitions
	TPair<EUIScene, FB2UISceneOnOpen> ReservedUISceneOpenData;	//Unit for storing a UIScene transition history
	TArray<TPair<EUIScene, FB2UISceneOnOpen>> UISceneHistory;	//It is stored UIScene transition history.
	uint32 bOpeningDJLegacyHeroMgmtBySceneHistory : 1; // For back button compatibility between djlegacy hero mgmt and this UIManager system
	TArray<EUIScene>	NotAddUISceneHistoryList;

	//<< UIScene ==================================================================================

		// Error
	bool CheckErrorCodeContains(int32 InErrorCode) const;
	bool ErrorCodeContains(int32 InErrorCode) const;
	void SetErrorTimeStamp(int32 InErrorCode);
	void CheckErrorTimeStamp(float DeltaTime);

	TArray<int32> NoCheckErrorCode;
	TArray<FB2ErrorCode> ErrorTimeStamp;
	int64 ErrorResetPerSecond;

public:
	/* interface of FTickableGameObject. */
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual bool IsTickableInEditor() const override;

	/* Even where non-UserWidget allows you to create widgets.
	 * (In general, a function for internal use only UserWidget.) */
	template< class T >
	T* CreateWidget(UWorld* World, UObject* CustomOuter, UClass* UserWidgetClass)
	{
		BLADE2_SCOPE_CYCLE_COUNTER(UB2UIWidget_CreateWidget);
		if (World == nullptr)
		{
			return nullptr;
		}

		if (CustomOuter == nullptr)
		{
			return nullptr;
		}

		if (!UserWidgetClass->IsChildOf(UUserWidget::StaticClass()))
		{
			return nullptr;
		}

		if (UserWidgetClass->HasAnyClassFlags(CLASS_Abstract | CLASS_NewerVersionExists | CLASS_Deprecated))
		{
			return nullptr;
		}

		UUserWidget* NewWidget = NewObject<UUserWidget>(CustomOuter, UserWidgetClass);

		if (ULocalPlayer* Player = World->GetFirstLocalPlayerFromController())
		{
			NewWidget->SetPlayerContext(FLocalPlayerContext(Player));
		}

		NewWidget->Initialize();

		return Cast<T>(NewWidget);
	}

	template< typename T >
	T* GetUI(FName UIName)
	{
		BLADE2_SCOPE_CYCLE_COUNTER(UB2UIWidget_GetUI);
		auto WidgetData = GetWidgetData(UIName);
		if (!WidgetData) return nullptr;
		else
		{
			if (!WidgetData->Widget) return nullptr;
			else return Cast<T>(WidgetData->Widget);
		}
	}

	template< typename T >
	T* OpenUI(FName UIName, bool RightNow = false, bool bMustTop = false)
	{
		BLADE2_SCOPE_CYCLE_COUNTER(UB2UIWidget_OpenUI);
		B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UIManager::OpenUI %s"), *UIName.ToString()));

#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
		extern bool gDisableCombatEventUI;
		if (gDisableCombatEventUI && DevIsCombatEventUI(UIName))
		{
			return nullptr;
		}
#endif
		auto* Widget = GetUI<T>(UIName);
		if (Widget == nullptr)
		{
			if (FB2UIWidgetData* WidgetData = GetWidgetData(UIName))
			{
				// Need to call it before GetWidgetClass, to let it be cached at CachedWidgetClasses..
				UpdateUIDataCacheHistoryOnOpen(WidgetData);

				UClass* WidgetClass = GetWidgetClass(WidgetData); // load the widget class.
				auto* NewWidget = CreateWidget<T>(GetWorld(), this, WidgetClass);
				if (NewWidget != nullptr)
				{
					WidgetData->Widget = NewWidget;
					WidgetData->WidgetAssetName = WidgetData->GetWidgetAssetName();

					NewWidget->MyUIFName = WidgetData->WidgetFName;
					NewWidget->Init();
					NewWidget->SetVisibility(ESlateVisibility::Collapsed);

					Widget = NewWidget;
				}

				// CanUnloadByPolicy will always fail if UIDataCacheHistoryNum is positive.
				if (WidgetData->CanUnloadByPolicy())
				{
					if (GIsEditor == false) // We don't usually unload TAsset in the Editor.
						Unload(WidgetData);
				}
			}
		}
		InternalOpenUI(UIName, Widget, RightNow, bMustTop);

		return Widget;
	}

	void OpenUI(FName UIName, bool RightNow = false, bool bMustTop = false)
	{
		OpenUI<UB2UIWidget>(UIName, RightNow);
	}

	/**
	 * bCloseStaticUIs : If true, widgets with bStayInViewport option are closed too.
	 */
	void CloseUI(FName UIName, bool RightNow = false, bool bCloseStaticUIs = false);


	TSubclassOf<class UB2UIWidget> GetWidgetClass(const FB2UIWidgetData* WidgetData);
	UClass* FindCachedWidgetClass(const FName& WidgetFName);

	void Unload(const FB2UIWidgetData* WidgetData);


	//>>Common Msg Popup============================================================================================================
		/* Common Pop-up UI
		 * param UIMsgPopupType : annoying..., but that helplessly for separate processing.
		 * param InTitleText : Title Text
		 * param InContentText : The contents of the text.
		 * param bModal : Make modal-window.
		 * param bAutoCloseOnClick : When you click the button, whether it is automatically closed.
		 * param ButtonGroup : Specifying the type of button set. One button type or Two buttons type. You can specify the text of the button.
		 * param OnBtnPosClick : Button click handler in positive way (e.g. Yes, Confirm). Use as lambda.
		 * param OnBtnNegClick : Button click handler in negative way (e.g. No, Cancel). Use as lambda.
		 */
	template< typename T >
	T* OpenMsgPopup(EUIMsgPopup UIMsgPopupType, const FText& InTitleText, const FText& InContentText, float AutoCloseTime = 0.f
		, const bool bModal = true, const bool bAutoCloseOnClick = true, EUIMsgPopupButtonGroup ButtonGroup = EUIMsgPopupButtonGroup::Yes
		, const FMsgPopupOnClick& OnBtnPosClick = 0, const FMsgPopupOnClick& OnBtnNegClick = 0
		, const bool MustTopView = false, EPopUpPriority PopupPriority = EPopUpPriority::Base)
	{


	////	BLADE2_SCOPE_CYCLE_COUNTER(UB2UIWidget_OpenMsgPopup);
	////	B2_SCOPED_TRACK_LOG(TEXT("UB2UIManager::OpenMsgPopup"));
	////	StopRelicAutoEnchantClass<>::GetInstance().Signal();
	////	//Type convert... ;;
	////	FName UIName;
	////	switch (UIMsgPopupType)
	////	{
	////	default:
	////	case EUIMsgPopup::Simple: UIName = UIFName::MsgPopupSimple; break;
	////	case EUIMsgPopup::Input: UIName = UIFName::MsgPopupInput; break;
	////	case EUIMsgPopup::ItemReward: UIName = UIFName::MsgPopupItemReward; break;
	////	case EUIMsgPopup::Center: UIName = UIFName::MsgPopupCenter; break;
	////	case EUIMsgPopup::ItemCostConfirm: UIName = UIFName::MsgPopupItemCost; break;
	////	case EUIMsgPopup::BuySkillPoint: UIName = UIFName::MsgPopupBuySkillPoint; break;
	////	case EUIMsgPopup::GoTeamMatch: UIName = UIFName::MsgPopupGoTeamMatch; break;
	////	case EUIMsgPopup::NeedClearStage: UIName = UIFName::MsgPopupNeedClearStage; break;
	////	case EUIMsgPopup::UseGold: UIName = UIFName::MsgPopupUseGold; break;
	////	case EUIMsgPopup::UseGem: UIName = UIFName::MsgPopupUseGem; break;
	////	case EUIMsgPopup::SimpleAddComment: UIName = UIFName::MsgPopupSimpleAddComment; break;
	////	case EUIMsgPopup::UserInfo: UIName = UIFName::MsgPopupUserInfo; break;
	////	case EUIMsgPopup::SelectPCClass: UIName = UIFName::MsgPopupSelectPCClass; break;
	////	case EUIMsgPopup::InputNoneFocus: UIName = UIFName::MsgPopupInputNoneFocus; break;
	////	case EUIMsgPopup::SimpleInvite: UIName = UIFName::MsgPopupSimpleInvite; break;
	////	case EUIMsgPopup::RaidPenalty: UIName = UIFName::MsgPopupRaidPenalty; break;
	////	case EUIMsgPopup::InventoryFull: UIName = UIFName::MsgPopupInventoryFull; break;
	////	case EUIMsgPopup::DetailMyInfo: UIName = UIFName::MsgPopupDetailMyInfo; break;
	////	case EUIMsgPopup::BuyHotTime:  UIName = UIFName::MsgPopupBuyHotTime; break;
	////	case EUIMsgPopup::MsgPopupBuyDicePoint:  UIName = UIFName::MsgPopupBuyDicePoint; break;
	////	}

	////	// Just guess better not allow duplicated, close first if found existing one.
	////	if (auto* ExistingOne = GetUI<UB2UIMsgPopupBase>(UIName))
	////	{
	////		if (ExistingOne->GetPriority() > PopupPriority)
	////			return nullptr;

	////		CloseMsgPopup(ExistingOne);
	////	}

	////	auto MsgPopup = OpenUI<UB2UIMsgPopupBase>(UIName, false, MustTopView);
	////	if (MsgPopup)
	////	{
	////		++MsgPopupCnt;
	////		MsgPopupMap.Add(MsgPopupCnt, MsgPopup);
	////		MsgPopup->SetMsgID(MsgPopupCnt);
	////		MsgPopup->MsgPopupType = UIMsgPopupType;
	////		MsgPopup->SetTitle(InTitleText);
	////		MsgPopup->SetContent(InContentText);
	////		MsgPopup->SetModalWindow(bModal);
	////		MsgPopup->SetAutoCloseTime(AutoCloseTime);
	////		MsgPopup->SetPriority(PopupPriority);

	////		//Is this buttoned popup
	////		auto MsgPopupSimple = Cast<UB2UIMsgPopupSimple>(MsgPopup);
	////		if (MsgPopupSimple)
	////		{
	////			MsgPopupSimple->SetAutoCloseOnClick(bAutoCloseOnClick);
	////			MsgPopupSimple->SetButtonGroup(ButtonGroup);
	////			MsgPopupSimple->AddHandler(EUIMsgPopupButton::Positive, OnBtnPosClick);
	////			MsgPopupSimple->AddHandler(EUIMsgPopupButton::Negative, OnBtnNegClick);
	////		}

	////		return Cast<T>(MsgPopup);
	////	}
	////	else
	////	{
			return nullptr;
	////	}
	}

	/* When simple to use without a return type. */
	void OpenMsgPopup(EUIMsgPopup UIMsgPopupType, const FText& InTitleText, const FText& InContentText, float AutoCloseTime = 0.f, const bool bModal = true, 
		const bool bAutoCloseOnClick = true, EUIMsgPopupButtonGroup ButtonGroup = EUIMsgPopupButtonGroup::Yes, const FMsgPopupOnClick& OnBtnPosClick = 0, 
		const FMsgPopupOnClick& OnBtnNegClick = 0, const bool MustTopView = false, EPopUpPriority PopupPriority = EPopUpPriority::Base)
	{
		//OpenMsgPopup<UB2UIMsgPopupSimple>(UIMsgPopupType, InTitleText, InContentText, AutoCloseTime, bModal, bAutoCloseOnClick, ButtonGroup, OnBtnPosClick, OnBtnNegClick, MustTopView, PopupPriority);
	}

	void OpenMsgPopupFromErrorCode(int32 nErrorCode);

	void CloseMsgPopup(int32 MsgPopupId, bool bRightNow = false);
	//void CloseMsgPopup(UB2UIMsgPopupBase* MsgPopup, bool bRightNow = false);

	/** It can be used when you cannot cache last open msg popup id or ref, for any reason like level transition, but like last resort. */
	void CloseAllMsgPopups(bool bRightNow = false);

	bool IsMsgPopupOpened(int32 MsgPopupId);
	bool IsPopupPriority(EPopUpPriority InIsPriority) const;

	void OpenAppUpdatePopup();
	void GoToTitlePopup(int32 nErrorCode);
	void GoToLobbyPopup(int32 nErrorCode);
	void SettlePopup(int32 nErrorCode);

	void ResetRepeatBattle();

	FString GetPlatformAppStoreURL();
	bool IsOpendAppStoreReview();
	bool OpenPopupAppStoreReview();

	void OpenToolTipTextBox(FText& ContextText, UB2UIWidgetBase* TargetUB2UIWidget, UWidget* Target, float ClockDirection = 6.0f, float InLifeTime = 5.0f);
	bool CheckRewardPushType(int32 PushType, int32 RewardId);
	class UB2UIMailRewardPopUp* OpenRewardMailPopUp(const int32 RewardId, bool CheckMailReward = true);
	class UB2UIMailRewardPopUp* OpenRewardMailPopUp(const b2network::B2RewardPtr RewardItem, bool CheckMailReward = true);
	class UB2UIMailRewardPopUp* OpenRewardMailPopUp(const TArray<b2network::B2RewardPtr>& RewardItem, bool CheckMailReward = true);
	//<<Common Msg Popup=================================================================================================================

	UB2UnitedWidgetBase* GetRedDotWidget(UUserWidget* OwnerUserWidget, UWidget* ParentWidget = nullptr);
	FName GetUIFNameFromAssetName(const FName& AssetName);
	FName GetWidgetAssetName(const FName& WidgetInstName);

	//>>Shortage Info Popup============================================================================================================
	//Function
public:
	class B2UIShortagePopupManager* GetShortagePopupMGR();
protected:
private:
	//Variable
public:
protected:
private:
	class B2UIShortagePopupManager* m_ShortagePopupMGR;
	//<<Shortage Info Popup=================================================================================================================
private:
	UB2UIManager(const FObjectInitializer& ObjectInitializer);
	~UB2UIManager();

	/* The Instance of singleton. */
	static class UB2UIManager* Instance;

	/* It is needed to prevent duplicate initialization. */
	bool bInit;

	/* It does not create a widget, without the player information.......  */
	FLocalPlayerContext PlayerContext;

	bool bCinematicMode;
	bool bHUDHidingCinematicMode; // Subset of bCinematicMode, regarding HudHiding only.

	/** If being set positive number, it performs garbage collection at each CloseUIScene call of this interval. */
	int32 ForceGCOnUISceneCloseInterval;
	int32 UISceneCloseWithoutGCCount; // GC if it reaches ForceGCOnUISceneCloseInterval

	/** How many UI widget data to cache by open history?
	 * i.e. Once open, UI widget data will remain in memory until other UIs of this number get opened. */
	int32 UIDataCacheHistoryNum;
	/** UI widget data of name contained in this list will remain in memory. */
	TArray<FName> UIDataCacheHistory;

	/* To be prepared when it is initialized. */
	void PrepareUI();

	void SubscribeEvents();
	void UnsubscribeEvents();

	/* The internal function separated from the function template. */
	void InternalOpenUI(FName InUIName, class UB2UIWidget* Widget, bool bRightNow = false, bool bMustTop = false);

	/**
	 * bCloseStaticUIs : If true, widgets with bStayInViewport option are closed too.
	 */
	void InternalCloseUI(FName InUIName, class UB2UIWidget* Widget, bool bRightNow = false, bool bCloseStaticUIs = false);

	void UpdateUIDataCacheHistoryOnOpen(FB2UIWidgetData* InJustOpenedUIData);

	/* To be able to know the specific pop-up. */
	int32 MsgPopupCnt;
	/* Save As pop-ups that are currently open. */
	//TMap<int32, UB2UIMsgPopupBase*> MsgPopupMap;

	FNumberFormattingOptions FloatNumberOption;

	TArray<std::shared_ptr<class issue>> Issues;

public:
	/** Unload especially for TSoftClassPtr */
	void UnloadAllTAssets();
#if WITH_EDITOR
	/** For safe use of TSoftClassPtr in editor.
	 *  Arguments are totally optional only for editor splash progress. */
	void EditorLoadAll(SetB2ContentLoadingProgressSplashFnPtr InSplashProgFnPtr = nullptr, float InStartProgress = 0.0f, float InEndProgress = 0.0f);
#endif
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif

	/** It is initially implemented for displaying created names for debug purpose, but might be used elsewhere.. */
	void GetAllCreatedUINames(TArray<FName>& OutUINames);
	void GetAllCreatedUIWidgetDatas(TArray<FB2UIWidgetData>& OutUIWidgetDatas);
	bool GetIsOtherWidgetVisible(TArray<FName> IgnoreWidgetName);

	const FNumberFormattingOptions* GetNumberFormattingOption() const {
		return &FloatNumberOption;
	}

	/** Some widget data will be added at Rootset on first-load to reduce loading time.. */
private:
	static TArray<FName> WidgetNamesForRootSet; // If this list gets too big, then change it to TMap.. but better not that big, only specially chosen.
	static TArray<FName> WidgetNamesForLoadOnStartup; // They are also RootSet, but being loaded from the start-up, always in memory.
public:
	static bool IsRootSetWidgetName(FName InUIName);
private:
	void SetupCacheOnLoadList();
	void SetupRootsetWidgetList();
	void SetupLoadOnStartupWidgetList();

public:
#if WITH_BII_ON_SCREEN_DEBUG_TEXT
	class UB2DebugTextWidget* GetScreenDebugTextWidget();

private:
	void CreateScreenDebugWidget();
	void DestroyScreenDebugWidget();
#endif
#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
	bool DevIsCombatEventUI(FName InCheckUI) const;
#endif

public:
	bool GetIndicatorVisible() {
		return IsIndicatorVisible;
	}
private:
	bool IsIndicatorVisible;

protected:

	// IB2AsyncInterface
	virtual void OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& CompleteRequestList);

};
