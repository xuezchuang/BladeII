// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
////#include "BladeII.h"
#include "UMG.h"
#include "Blueprint/UserWidget.h"
#include "B2Button.h"
#include "B2ButtonGoodInfoToolTip.h"
#include "B2ButtonGoodsShortcutToolTip.h"
#include "B2Image.h"
#include "B2ScrollBox.h"
#include "B2RichTextBlock.h"
#include "B2UnitedWidgetBase.generated.h"

#if WITH_EDITOR
#define ISVALID(NAME)					\
		if(false == GIsEditor)			\
			BII_CHECK(NAME.IsValid())

#define ADDCHECKROUTINE(TYPE,NAME)		\
		if(true == GIsEditor)			\
			m_CheckValid.Add(NAME, TYPE##::StaticClass());
#else
#define ISVALID(NAME)
#define ADDCHECKROUTINE(TYPE,NAME)
#endif //WITH_EDITOR
/* This is a macro to import easily the widget. */
#ifdef GET_SLOT
#undef GET_SLOT
#endif
#define GET_SLOT(TYPE,NAME)NAME = GetCachedWidget<TYPE>(FName(#NAME));
#define GET_SLOT_BYNAME(TYPE,SLOT,NAME)SLOT = GetCachedWidget<TYPE>(FName(#NAME));
#define GET_SLOT_FNAME(TYPE,NAME) GetCachedWidget<TYPE>(NAME);

#ifdef GET_SLOT_CHECK
#undef GET_SLOT_CHECK
#else
#define GET_SLOT_CHECK(TYPE,NAME)			\
			GET_SLOT(TYPE,NAME)				\
			ISVALID(NAME)					\
			ADDCHECKROUTINE(TYPE,#NAME)
#endif //GET_SLOT_CHECK

/* This macro easily bind the click function. */
#ifdef BIND_CLICK_FUNC_TO_BTN
#undef BIND_CLICK_FUNC_TO_BTN
#endif // BIND_CLICK_FUNC_TO_BTN(BTN,FUNC)
#define BIND_CLICK_FUNC_TO_BTN(BTN,FUNC)									\
		if (BTN.IsValid())													\
		{																	\
			BTN->OnClicked.RemoveDynamic(this, FUNC);						\
			BTN->OnClicked.AddDynamic(this, FUNC);							\
		};

/* This macro easily bind the press function. */
#ifdef BIND_PRESS_FUNC_TO_BTN
#undef BIND_PRESS_FUNC_TO_BTN
#endif // BIND_PRESS_FUNC_TO_BTN(BTN,FUNC)
#define BIND_PRESS_FUNC_TO_BTN(BTN,FUNC)									\
		if (BTN.IsValid())													\
		{																	\
			BTN->OnPressed.RemoveDynamic(this, FUNC);						\
			BTN->OnPressed.AddDynamic(this, FUNC);							\
		};

/* This macro easily bind the press function. */
#ifdef BIND_RELEASE_FUNC_TO_BTN
#undef BIND_RELEASE_FUNC_TO_BTN
#endif // BIND_RELEASE_FUNC_TO_BTN(BTN,FUNC)
#define BIND_RELEASE_FUNC_TO_BTN(BTN,FUNC)									\
		if (BTN.IsValid())													\
		{																	\
			BTN->OnReleased.RemoveDynamic(this, FUNC);						\
			BTN->OnReleased.AddDynamic(this, FUNC);							\
		};

DECLARE_DELEGATE_RetVal(bool, FRedDotConditionDelegate)
DECLARE_DELEGATE_OneParam(FRedDotNotifyDelegate, bool)

USTRUCT()
struct FAutoRedDotConditionCheck
{
	GENERATED_USTRUCT_BODY()

	class UB2UnitedWidgetBase*	RedDotWidget;
	FRedDotConditionDelegate	ConditionCheckHandler;
	FRedDotNotifyDelegate		OnRedDotNotify;

	FAutoRedDotConditionCheck() : RedDotWidget(nullptr) {}
	FAutoRedDotConditionCheck(class UB2UnitedWidgetBase* InRedDotWidget, FRedDotConditionDelegate& RedDotConditionHandler)
		: RedDotWidget(InRedDotWidget)
	{
		ConditionCheckHandler = RedDotConditionHandler;
	}
	FAutoRedDotConditionCheck(class UB2UnitedWidgetBase* InRedDotWidget, FRedDotConditionDelegate& RedDotConditionHandler,
		FRedDotNotifyDelegate& RedDotNotify)
		:RedDotWidget(InRedDotWidget), ConditionCheckHandler(RedDotConditionHandler), OnRedDotNotify(RedDotNotify)
	{ }
};

#ifndef REGISTER_REDDOT_CONDITION
#undef REGISTER_REDDOT_CONDITION
#endif
#define REGISTER_REDDOT_CONDITION(RedDotWidget, ConditionLogic)											\
	{																									\
		if (RedDotWidget != nullptr) {																	\
			FRedDotConditionDelegate RedDotConditionDelegate;											\
				RedDotConditionDelegate.BindUObject(this, ConditionLogic);								\
				FAutoRedDotConditionCheck ConditionChecker(RedDotWidget, RedDotConditionDelegate);		\
				RedDotConditionCheckList.Add(ConditionChecker);											\
		}																								\
	}

#ifndef REGISTER_REDDOT_CONDITION_LAMBDA
#undef REGISTER_REDDOT_CONDITION_LAMBDA
#endif
#define REGISTER_REDDOT_CONDITION_LAMBDA(RedDotWidget, ConditionLogic)									\
	{																									\
		if (RedDotWidget != nullptr) {																	\
			FRedDotConditionDelegate RedDotConditionDelegate;											\
				RedDotConditionDelegate.BindLambda(ConditionLogic);										\
				FAutoRedDotConditionCheck ConditionChecker(RedDotWidget, RedDotConditionDelegate);		\
				RedDotConditionCheckList.Add(ConditionChecker);											\
		}																								\
	}


#ifndef REGISTER_REDDOT_CONDITION_WITHDELEGATE
#undef REGISTER_REDDOT_CONDITION_WITHDELEGATE
#endif
#define REGISTER_REDDOT_CONDITION_WITHDELEGATE(RedDotWidget, ConditionLogic, NotifyHandler)								\
	{																													\
		if (RedDotWidget != nullptr) {																					\
			FRedDotConditionDelegate RedDotConditionDelegate;															\
			RedDotConditionDelegate.BindUObject(this, ConditionLogic);													\
			FRedDotNotifyDelegate RedDotNotifyDelegate;																	\
			RedDotNotifyDelegate.BindLambda(NotifyHandler);																\
			FAutoRedDotConditionCheck ConditionChecker(RedDotWidget, RedDotConditionDelegate, RedDotNotifyDelegate);	\
			RedDotConditionCheckList.Add(ConditionChecker);																\
		}																												\
}																														\

	
DECLARE_DELEGATE(FTabButtonDelegate)
#ifndef REGISTER_UITAB_BINDING
#undef REGISTER_UITAB_BINDING
#endif
#define REGISTER_UITAB_BINDING(TabWidget, ClickBoundLogic)												\
	{																									\
		if (TabWidget != nullptr) {																		\
			FTabButtonDelegate TabButtonDelegate;														\
				TabButtonDelegate.BindUObject(this, ClickBoundLogic);									\
				OnRegistTabWidgets(TabWidget,TabButtonDelegate);										\
		}																								\
	}

/*
 * The base class of all UserWidgets of BladeII project. This is introduced rather late. 
 * BladeIIUserWidget and B2UIWidgetBase were the base UserWidget for two different UI systems.
 * It is now the base of both of them, to put any common features (probably utilities) or interface whatever possible.
 */
UCLASS()
class BLADEII_API UB2UnitedWidgetBase : public UUserWidget
{
	GENERATED_BODY()


	/**
	 * Intended to set overall visibility of all widgets contained here in certain circumstances (e.g. Cinematic).
	 * Each subclass might have to refer to this value (by IsForceHidden) if SetVisibility is directly used.
	 */
	UPROPERTY(Transient)
	uint32 bForceHidden : 1;

	ESlateVisibility VisibilityBeforeForceHidden;

	/** Filled at Construct timing, being iterated for animated ColorAndOpacity value to MID param mapping */
	//UPROPERTY(Transient)
	//TArray<UB2Image*> ColorToMIDParamMappedImages;

	// Master use flag for button StunForDuration, to prevent multiple click/press within one frame or for some time.
	static bool bUseButtonStunForDuration;
	// Flags being set on stun for duration, to prevent multiple entrance and do restore routine.
	uint32 bButtonsStunnedThisTick : 1;
	uint32 bHasStunnedButtons : 1;
	
	UPROPERTY(Transient)
	TArray<UB2Button*> AllB2Buttons;
	UPROPERTY(Transient)
	TArray<UB2Button*> TickableButtons; // Subset of AllB2Buttons.

	/** It is created as a intermediate size or scale box panel, 
	 * when this user widget is dynamically created in a placed panel widget (by UB2UnitedWidgetBase::DynCreate** series).
	 * This is cached here to ensure destruction when this user widget is get to be destroyed. */
	UPROPERTY(Transient)
	UPanelWidget* DynCreatedDummyParent;

protected:

	UPROPERTY(Transient)
	TArray<UButton*> HighlightRects;

	UPROPERTY(Transient)
	TArray<UCanvasPanelSlot*>	HilightPanels;

	//UPROPERTY(Transient)
	//TArray<class UB2HighlightWidget*>	HighlightWidgets;

	//================================================================================
	// Manual Scroll-Box handling. Those settings are required by two UnitedUserWidgets. Sender and Receiver.
	// where Sender is the one catches user input, and Receiver is the one having actual Scroll-box.


	///// Settings for Sender

	/** It will send its input handling to specified scroll-box */
	uint32 bManualScrollBoxHandling_Sender : 1;
	/** To be set by native, with bManualScrollBoxHandling_Sender. It (receiver) better exist longer than myself (sender). */
	TWeakObjectPtr<UB2UnitedWidgetBase> MyManualScrollBoxHandlingReceiver;
	/** This scroll box supposed to be resides in MyManualScrollBoxHandlingReceiver, not me. */
	//TWeakObjectPtr<UB2ScrollBox> MyManualScrollBox;


	///// Settings for Receiver

	/** It will receive its input handling for its own scroll-box from other sender. */
	uint32 bManualScrollBoxHandling_Receiver : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float ManualScrollBoxSensitivity;

	//================================================================================
	//[@AKI, 170330] 바로가기 툴팁 관련하여 ENum값으로 어떤 행동을 할지 결정하기 위해 만듬.
	//원래는 B2ButtonGoodInfoToolTip에 있었으나 그 ENum값을 가져오는데 너무 Depth가 있어 이 위치로 바꿈
	//추후 더 좋은 곳이 있다면 그곳으로 보낼 것 임 또는 보내주세요.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Popup Type")
	EGoodsInfoPopupType GoodsPopupType;

	// Pre Load Asset
	TMap<FName, UWidget*> CachedWidgets;

#if WITH_EDITOR
	TMap<FString, UClass*> m_CheckValid;
#endif //WITH_EDITOR

private:

	/** Flags being marked when it is used with WidgetPool. */
	uint32 bIsPooledObject : 1;
	uint32 bCurrentlyRentAsPooledObject : 1;

public:
	UB2UnitedWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void PostInitProperties() override;

	virtual bool Initialize() override;

	virtual void UpdateServercmdGetAccount();

	/* Function to search for specific widgets by name. */
	template <typename T>
	T* GetSlot(FName SlotName, UWidget* BaseWidget = nullptr)
	{
		if (!BaseWidget)
			BaseWidget = GetRootWidget();

		auto Found = GetSlotInternal(SlotName, T::StaticClass(), BaseWidget);
		return Cast<T>(Found);
	}

	template<typename T>
	T* GetCachedWidget(FName WidgetName)
	{
		if(UWidgetAnimation::StaticClass() == T::StaticClass())
		{
			return Cast<T>(GetAnimation(WidgetName));
		}
		if (CachedWidgets.Contains(WidgetName))
		{
			return Cast<T>(CachedWidgets[WidgetName]);
		}
		return nullptr;
	}

	/* Function to search for specific Anim by name. */
	TWeakObjectPtr<UWidgetAnimation> GetAnimation(const UObject* OuterObject, const FName& animName);
	UWidgetAnimation*				 GetAnimation(const FName& animName);

	//[@AKI, 170411] 버튼에서 사용하기 위해 만든 메소드
	EGoodsInfoPopupType GetGoodsPopupType() const { return GoodsPopupType; };

	UFUNCTION(BlueprintCallable, Category = "BladeIIGame", DisplayName = "GetTimeTextFromSecond")
	static FText GetTimeTextFromSecond(int32 nSecond);

protected:
	void DestroySelfCommon();

protected:

	TArray<FAutoRedDotConditionCheck>	RedDotConditionCheckList;

	virtual void RegisterUIMarkForRedDot() {}
	virtual void UnRegisterUIMarkForRedDot();

public:	
	virtual void DoMarkRedDot();
	void ForceMarkRedDot(bool bShow);
	
	/** Intended to disable all other buttons when a button is pressed */
	void SetButtonsStunForDuration(UB2Button* InJustPressedButton);
	void RestoreButtonsFromStun();

	/**
	 * Hide this widget unconditionally.
	 * Use RestoreFromForceHidden to end ForceHidden.
	 * It might not working as expected if SetVisibility is called between this call and RestoreFromForceHidden.
	 */
	void ForceHide();

	/**
	 * Restore the visibility state to before ForceHide call.
	 * Any visibility setting by SetVisibility after last ForceHide call will be ignored
	 */
	void RestoreFromForceHidden();

	FORCEINLINE bool IsForceHidden() { return bForceHidden; }

	/**
	 * Gets a widget's screen coordinate in more or less formal way, than HACKGetWidgetScreenCoord
	 */
	static FVector2D GetWidgetScreenPos(UWidget* InWidget, const FVector2D& SpotInWidgetSpace = FVector2D(0.f, 0.0f), bool bForce = false);

	//================================================================================
	// Some HACKs to get slate widget's position stuff, which is not officially supported by UE4.

	/** Reference size used for creating our UMG widget. UIs are stretched when current viewport is bigger than this, or extended. */
	const static FVector2D DesignedRefViewSize;

	const static FVector2D WrongScreenPosition;

	/** For scale the actual size or position of slate widget from designed UMG reference scale to current viewport size.
	* Using this is some sort of hack.. but need this as there's no official way to get slate widget coordinate. 
	* @param bIgnoreNonScaledSide : Ignores either width or height, whatever non scaled side by DPI curve scale rule */
	static FVector2D GetDesignToCurrentViewScale(class APlayerController* InPlayerController, bool bIgnoreNonScaledSide = true);

	/** !!!!! WORKS ONLY FOR SPECIAL CONDITION !!!!!
	* Get the widget's screen space coordinate starting from screen upper-left.
	* It won't return correct result from all kind of widgets, and returns false for failure.
	* Possible for widgets in whole screen-sized CanvasPanelSlot, using non stretched anchor.
	* @param bWidgetCenterCoord : If true, returns widget's center coordinate considering widget's size, or upper left coordinate. 
	* @param bIgnoreNonScaledSide : Ignores either width or height, whatever non scaled side by DPI curve scale rule */
	static bool HACKGetWidgetScreenCoord(UWidget* InWidget, APlayerController* InPC, FVector2D& OutResultCoord, bool bWidgetCenterCoord, bool bIgnoreNonScaledSide = true);

	/** Get world position beneath the InScreenCoord deep much as DepthFromScreen. Better use small DepthFromScreen to get result looks close to InScreenCoord.
	 * @param bDeprojectFromScreenCenter : If false, de-project from InScreenCoord */
	static FVector GetWorldPosBeneathScreen(APlayerController* InPC, FVector2D InScreenCoord, float DepthFromScreen, bool bDeprojectFromScreenCenter = true);

	//================================================================================
	
	/** An utility to dynamically create a user widget as a child of specified canvas panel using specified class.
	 * The most common usage example is creating player's item icons on inventory panel.
	 * @param NonScaledSize : Most likely the original size of InClass. It will affect created widget's alignment.
	 * @param FinalSlotSize : The final size that the created widget will fit. With 1 : 1 ratio, NonScaledSize and FinalSlotSize is the same. If ParentPanel contains only one widget, FinalSlotSize could be the panel's size.
	 * @param bPivotAtCenter : Render transform pivot will be at center if true, at upper left if false. */
	static UB2UnitedWidgetBase* DynCreateInCanvasPanel(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, class UCanvasPanel* ParentPanel,
		FVector2D NonScaledSize, FVector2D FinalSlotSize, FVector2D RelPosInPanel, int ZOrderInPanel = 0, bool bPivotAtCenter = false);
	/** Sub-routine of DynCreateInCanvasPanel.. separated for some reason.. */
	static bool PutWidgetInCanvasPanel(UB2UnitedWidgetBase* OwnerUserWidget, UB2UnitedWidgetBase* TargetWidget, class UCanvasPanel* ParentPanel,
		FVector2D NonScaledSize, FVector2D FinalSlotSize, FVector2D RelPosInPanel, int ZOrderInPanel = 0, bool bPivotAtCenter = false);

	/** More or less simpler version.. Create in full screen size. */
	static UB2UnitedWidgetBase* DynCreateInCanvasPanelFull(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, class UCanvasPanel* ParentPanel);

	/** An utility to dynamically create a user widget as a child of specified vertical box using specified class.
	 * As placement in vertical box is more limited than in canvas panel, we got less parameters here.
	 * -> Later we can do the same stuff as DynCreateInHorizontalBox, setting up size box internally.. */
	static UB2UnitedWidgetBase* DynCreateInVerticalBox(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, class UVerticalBox* ParentVB, FMargin InPlacementMargin = FMargin());

	/** An utility to dynamically create a user widget as a child of specified horizontal box using specified class. 
	 * If you specify InElementSize as non-zero, it will create size box first to contain created widget. */
	static UB2UnitedWidgetBase* DynCreateInHorizontalBox(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, class UHorizontalBox* ParentHB, FVector2D InElementSize = FVector2D(0.0f, 0.0f), FMargin InPlacementMargin = FMargin());

	/** An utility to dynamically create a user widget as a child of specified wrap box using specified class.
	 * where wrap box is suppose to align all elements in order.
	 * If you specify InElementSize as non-zero, it will create size box first to contain created widget. */
	static UB2UnitedWidgetBase* DynCreateInWrapBox(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, class UWrapBox* ParentWB, FVector2D InElementSize = FVector2D(0.0f, 0.0f), FMargin InPlacementMargin = FMargin());

	/** Need to set this when DynCreate** creates dummy panel widget inside to contain dynamically created user widget. */
	void SetDynCreatedDummyParent(UPanelWidget* InWidget); 
	void ConditionalClearDynCreatedDummyParent();

	void ActiveHighlight(const FName& HighlightWidgetName);
	void ClearHighlight();
	void AddActiveHighlight(FVector2D fv2Size);

	//================================================================================
	// Manual Scroll-Box handling. 
public:
	/** To be called on sender, by receiver. */
	void SetupManualScrollBoxSender_byReceiver(UB2UnitedWidgetBase* InReceiver, UB2ScrollBox* HandlingScrollBox);
protected:
	///// For Sender

	virtual void OnSetupManualScrollBoxSender(UB2UnitedWidgetBase* InReceiver, UB2ScrollBox* HandlingScrollBox) {} // Override if post setup handling is necessary.

	double TimeSenderBtnAreaPressStarted;
	uint32 bSenderBtnAreaPressed : 1;
	// Override to send manual input signal to the wanted scroll-box of receiver.
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	// Sender assumed to have an area button which covers its most area. Its handling is now done by these functions below (by overridden).
	virtual void OnClickedSenderBTNArea(){}
	virtual void OnPressedSenderBTNArea(){}
	virtual void OnReleasedSenderBTNArea(){}


	UB2UnitedWidgetBase* GetRedDotWidget(UWidget* ParentWidget = nullptr);
	UB2UnitedWidgetBase* GetRedDotWidget(const FString& ParentWidgetName);
	UB2UnitedWidgetBase* GetRedDotWidgetFromSubWidget(UUserWidget* SubWidget);

	void ForceSetVisibleRedDot(UWidget* ParentWidget, bool bVisible);
	void ForceSetVisibleRedDot(const FString& ParentWidgetName, bool bVisible);

	///// For Receiver. Guess they don't need to be overridden, but make them virtual if necessary.

	//uint32 bIsManualScrollBoxTouching : 1;
	//FReply OnSBMouseButtonDownFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	//FReply OnSBMouseButtonUpFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	//FReply OnSBMouseMoveFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	//void OnSBMouseLeaveFromSender(UB2ScrollBox* InScrollBox, const FPointerEvent& InMouseEvent);
	//FReply OnSBTouchStartedFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	//FReply OnSBTouchMovedFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	//FReply OnSBTouchEndedFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);

	//================================================================================




private:
	/* The internal function separated from the function template. */
	UWidget* GetSlotInternal(FName SlotName, UClass* TargetClass, UWidget* CurrWidget);
	void GetSlotInternal(UWidget* CurrentWidget);

public:
	void ClearUITabList();
	void OnChangeStateTabWidgets(class UB2UICommonTab* TabWidget);
	void OnRegistTabWidgets(class UB2UICommonTab* TabWidget, FTabButtonDelegate TabButtonDelegate);
	TArray<class UB2UICommonTab*> TabWidgetList;

	/** Just calling it does not make it being actually pooled. Proper pooling manager object (derived from FWidgetPoolBase) will use this. */
	void SetAsPooledObject();
	virtual void SetCurrentlyRent(bool bInRent, UObject* InRenter); // Only work when bIsPooledObject is true.
	FORCEINLINE bool IsPooledObject() const { return bIsPooledObject; }
	FORCEINLINE bool IsCurrentlyRent() const { return (bIsPooledObject && bCurrentlyRentAsPooledObject); }
	bool ShouldBeHiddenAsPooledObject() const;
protected:
	/** Pooled object Rent/TurnIn event, most likely for some UI animation state reset.. if necessary. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnPooledObjectRentBP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnPooledObjectTurnInBP();
};

#if UE_BUILD_DEBUG
void DebugCheckOnLoadSynchronousUse(UObject* WorldContextObject);
#endif
/**  
 * A little extension on UB2UnitedWidgetBase::DynCreateInCanvasPanelFull
 */
template<class WidgetClassT>
WidgetClassT* DynLoadAndCreateInCanvasPanelFull(TSoftClassPtr<WidgetClassT> InClass, UB2UnitedWidgetBase* OwnerUserWidget, class UCanvasPanel* ParentPanel)
{
	// Different from UB2UnitedWidgetBase::DynCreateInCanvasPanelFull in that InClass is TSoftClassPtr, which might not be loaded yet.
	if (!InClass.ToString().IsEmpty())
	{
		// Simple LoadSynchronous of TSoftObjectPtr and/or TSoftClassPtr can cause FlushAsyncLoading, so better not use it if async loading can be going on at using moment.
		// In that case, you better use LoadSynchronous interface provided by FStreamableManager.
		// For now,, let it being used at lobby..
#if UE_BUILD_DEBUG
		DebugCheckOnLoadSynchronousUse(OwnerUserWidget);
#endif
		// Do NOT presume that LoadSynchronous of TSoftClassPtr is always fine and suggested.
		// It can be used only when there are no possible use of async loading, or it will cause FlushAsyncLoading.
		TSubclassOf<WidgetClassT> LoadedClass = InClass.LoadSynchronous();
		return Cast<WidgetClassT>(UB2UnitedWidgetBase::DynCreateInCanvasPanelFull(LoadedClass, OwnerUserWidget, ParentPanel));
	}
	return nullptr;
}


DECLARE_MULTICAST_DELEGATE_OneParam(FOnProgLevelChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnProgPercentChanged, float);
DECLARE_MULTICAST_DELEGATE(FOnFinishProgAnimation);

/** An utility struct for progress bar filling-up animation, 
 * where progress is going up by progress percent within a level and reset on level gets increased */
UENUM(BlueprintType)
enum class EProgressAnimType :  uint8
{
	PRAT_ConstSpeed,
	PRAT_SpringDamp // It is especially useful when increasing range is largely varying
};


USTRUCT(BlueprintType)
struct FProgressAnimateUtil
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EProgressAnimType AnimType;

	/** Constant speed in PRAT_ConstSpeed mode, minimum speed in PRAT_SpringDamp mode. 
	 * Like unit per sec. 1.0 means progress bar will be filled up in 1 sec. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSpeed; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpringConst; // Only for PRAT_SpringDamp

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DampConst; // Only for PRAT_SpringDamp

	FOnProgLevelChanged OnLevelChanged;
	FOnProgPercentChanged OnPrecentChanged;
	FOnFinishProgAnimation	OnFinishProgAnimation;

	FTimerHandle CustomTickHandle;

	float TickDelta;

	UB2UnitedWidgetBase* OwnerWidget;

private:
	int32 TargetLevel;
	float TargetPercentScale; // 0.0 ~ 1.0 scale

	int32 CurrentAnimatedLevel; // The result level value
	float CurrentAnimatedPercent; // The result progress percent value, 0.0 ~ 1.0.

	float CurrentSpeed;
	
public:
	FProgressAnimateUtil()
	{
		AnimType = EProgressAnimType::PRAT_SpringDamp;

		BaseSpeed = 2.0f;
		SpringConst = 5.0f;
		DampConst = 2.5f;

		TickDelta = 0.f;
		OwnerWidget = nullptr;

		TargetLevel = 0;
		TargetPercentScale = 0.0f;

		CurrentAnimatedLevel = 0;
		CurrentAnimatedPercent = 0.0f;

		CurrentSpeed = 0.0f;
	}

	void ApplyCustomTimerTick(UB2UnitedWidgetBase* InOwnerWidget, float DeltaTime = 0.05f);
	void ClearCustomTimerTick();

	void Tick();

	/** Tick() above is supposed to be used with ApplyCustomTimerTick. If you don't like that, just use this one for each tick. */
	void ManualTick(float InDeltaSecond);

	void SetTargetValues(int32 InTargetLevel, float InTargetPercentScale /*0.0 ~ 1.0*/, bool bSyncCurrentValues = false);

	void SyncToTargetValues(); // It force finish progress animation to target value.

	float GetUnitDistanceToTargetValue(); // How far the progress should go to reach the target? 0.0 Means reached at target.
	bool IsApplied();
	// In most cases, you get those values by On**Changed callback, but we sometimes need to get them..
	FORCEINLINE int32 GetCurrentAnimatedLevel() const { return CurrentAnimatedLevel; }
	FORCEINLINE float GetCurrentAnimatedPercent() const { return CurrentAnimatedPercent; }
};

/** For showing text progressively, like that of scenario dialog stuff. */
USTRUCT()
struct FProgressiveTextBuilder
{
	GENERATED_USTRUCT_BODY()

private:

	/**
	 * Whole text is to be built at most in this time.
	 */
	float TotalShowingTime;

	/**
	 * Each character will be shown at automatically calculated interval, but cannot over this time even when text is short.
	 */
	float MaxCharShowInterval;

	/** 
	 * To be automatically calculated as Total-Char-Count / Total-Showing-Time, and clamped by MaxCharShowInterval
	 */
	float CharShowInterval;

	/** Manually counted delta time, for use in paused circumstance */
	float ThisDeltaTime;
	/** Just the realtime counted at last frame. */
	float LastRealTime; 
		
	int32 CurrentCharShowIndex; // The last shown character index in whole dialog string.
	float LastCharUpdatedTime; // The time when CurrentCharShowIndex was lastly updated

	/** Contains all text to be displayed as current dialog, in FString format. */
	FString WholeTextString;

	/** Start from WholeTextString[0] ~ to WholeTextString. The result here. */
	FString CurrentTextString; 

	struct IndexedSpanTagText
	{
		IndexedSpanTagText(int32 idx, FString& Text) : Index(idx)
		{
			SpanTagText = TEXT("<") + Text + TEXT(">");
		}
		int32 Index;
		FString SpanTagText;
	};

	TArray<IndexedSpanTagText> IndexedSpanTagTextArray;

public:

	FProgressiveTextBuilder();


	/** Call this as the initial data set. */
	void SetWholeText(const FString& InString, float InTotalTime, float InMaxCharShowInterval, float InThisRealTime);

	void SetTotalShowingTime(float InNewTime);

	/** Has kinda different arguments. True return means CurrentTextString is changed. */
	bool Tick(float InThisRealTime, float InDeltaSec, bool& OutIsLastChar);

private:
	void ApplyStringProgress(int32 InNewCharacterIndex);
public:
	void ApplyToWholeString(); // Just directly goes to whole string.

	bool IsStringFilledUp();

	FString GetCurrentString() const;
};

#if WITH_EDITOR && !PLATFORM_MAC
/**
 * An utility to replace all sub-widgets of ClassTobeRemoved to ClassToReplace in BasePage WidgetBlueprint.
 */
void ReplaceSubWidgetClass(class UWidgetBlueprint* BasePage, TSubclassOf<UPanelWidget> ClassTobeRemoved, TSubclassOf<UPanelWidget> ClassToReplace, bool bSilent = false);
void ForceSetButtonClickSoundIndex(class UWidgetBlueprint* BasePage, int32 InSoundIndex, bool bSilent = false);

/** Implemented for special purpose. Intended to find a resource object having name partly the same to InOldAsset, but partly replaced (a different path) */
template<class TAssetClass>
TAssetClass* LoadAssetWithReplacedName(TAssetClass* InOldAsset, const FString& InOldPath, const FString& InNewPath)
{
	if (!InOldAsset) {
		return NULL;
	}
	FString AssetPath = InOldAsset->GetPathName();
	if (AssetPath.Contains(InOldPath)) {
		FString NewPath = AssetPath.Replace(*InOldPath, *InNewPath);
		return LoadObject<TAssetClass>(NULL, *NewPath);
	}
	return NULL;
}
// Repeated function of ReplaceSubWidgetResources
template<class TAssetClass>
bool SetBrushWithReplacedAsset(FSlateBrush& InBrushToChange, const FString& InOldPath, const FString& InNewPath)
{
	TAssetClass* CurrentAsset = Cast<TAssetClass>(InBrushToChange.GetResourceObject());
	TAssetClass* AssetAtNewPath = LoadAssetWithReplacedName<TAssetClass>(CurrentAsset, InOldPath, InNewPath);
	if (CurrentAsset && AssetAtNewPath)
	{
		InBrushToChange.SetResourceObject(AssetAtNewPath);
		return true;
	}
	return false;
}
// Repeated function of ReplaceSubWidgetResources
template<class TAssetClass>
bool SetFontWithReplacedAsset(FSlateFontInfo& InFontToChange, const FString& InOldPath, const FString& InNewPath)
{
	TAssetClass* CurrentAsset = Cast<TAssetClass>(InFontToChange.FontMaterial);
	TAssetClass* AssetAtNewPath = LoadAssetWithReplacedName<TAssetClass>(CurrentAsset, InOldPath, InNewPath);
	if (CurrentAsset && AssetAtNewPath)
	{
		InFontToChange.FontMaterial = AssetAtNewPath;
		return true;
	}
	return false;
}
// For very special purpose.. replaces resources of some type of widgets. having partly the same and partly different naming.
template<class TAssetClass>
int32 ReplaceSubWidgetResources(class UWidgetBlueprint* BasePage, const FString& InOldPath, const FString& InNewPath)
{
	if (!BasePage || !BasePage->WidgetTree || InOldPath.Len() == 0 || InNewPath.Len() == 0) {
		return 0;
	}

	int32 ChangedCount = 0; // Will count how many widgets are actually changed by this function.

	BasePage->WidgetTree->ForEachWidget([&](UWidget* CurrWidget) {
		
		// Hard to check for all type, but include as many as we can
		UImage* CastedImage = Cast<UImage>(CurrWidget);
		UButton* CastedBtn = Cast<UButton>(CurrWidget);
		UProgressBar* CastedPB = Cast<UProgressBar>(CurrWidget);
		UScrollBar* CastedSB = Cast<UScrollBar>(CurrWidget);

		UTextBlock* CastedTB = Cast<UTextBlock>(CurrWidget);
		UB2RichTextBlock* CastedRTB = Cast<UB2RichTextBlock>(CurrWidget);
		UEditableText* CastedET = Cast<UEditableText>(CurrWidget);

		if (CastedImage)
		{			
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedImage->Brush, InOldPath, InNewPath)){
				++ChangedCount;
			}
		}
		else if (CastedBtn)
		{
			bool bIsChangedOne = false;
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedBtn->WidgetStyle.Normal, InOldPath, InNewPath)){
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedBtn->WidgetStyle.Hovered, InOldPath, InNewPath)){
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedBtn->WidgetStyle.Pressed, InOldPath, InNewPath)){
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedBtn->WidgetStyle.Disabled, InOldPath, InNewPath)){
				bIsChangedOne = true;
			}
			if (bIsChangedOne){
				++ChangedCount;
			}
		}
		else if (CastedPB)
		{
			bool bIsChangedOne = false;
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedPB->WidgetStyle.BackgroundImage, InOldPath, InNewPath)){
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedPB->WidgetStyle.FillImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedPB->WidgetStyle.MarqueeImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (bIsChangedOne) {
				++ChangedCount;
			}
		}
		else if (CastedSB)
		{
			bool bIsChangedOne = false;
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedSB->WidgetStyle.HorizontalBackgroundImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedSB->WidgetStyle.VerticalBackgroundImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedSB->WidgetStyle.VerticalTopSlotImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedSB->WidgetStyle.HorizontalTopSlotImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedSB->WidgetStyle.VerticalBottomSlotImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedSB->WidgetStyle.HorizontalBottomSlotImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedSB->WidgetStyle.NormalThumbImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedSB->WidgetStyle.HoveredThumbImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedSB->WidgetStyle.DraggedThumbImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (bIsChangedOne) {
				++ChangedCount;
			}
		}
		else if (CastedTB)
		{
			if (SetFontWithReplacedAsset<TAssetClass>(CastedTB->Font, InOldPath, InNewPath)) {
				++ChangedCount;
			}
		}
		else if (CastedRTB)
		{
			// Font is protected here..
			//if (SetFontWithReplacedAsset<TAssetClass>(CastedRTB->Font, InOldPath, InNewPath)) {
			//	++ChangedCount;
			//}
		}
		else if (CastedET)
		{
			bool bIsChangedOne = false;
			if (SetFontWithReplacedAsset<TAssetClass>(CastedET->WidgetStyle.Font, InOldPath, InNewPath)) {
				++ChangedCount;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedET->WidgetStyle.BackgroundImageSelected, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedET->WidgetStyle.BackgroundImageComposing, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (SetBrushWithReplacedAsset<TAssetClass>(CastedET->WidgetStyle.CaretImage, InOldPath, InNewPath)) {
				bIsChangedOne = true;
			}
			if (bIsChangedOne) {
				++ChangedCount;
			}
		}
	});

	return ChangedCount;
}
#endif