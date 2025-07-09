#include "BladeII.h"
#include "B2UnitedWidgetBase.h"
#include "MovieScene.h"
//#include "Animation/WidgetAnimation.h"
#include "B2UIManager.h"
#include "B2UICommonTab.h"
#include "Engine/UserInterfaceSettings.h"
#include "B2HighlightWidget.h"
#if WITH_EDITOR
#include "Kismet2/BlueprintEditorUtils.h"
#include "Editor/UMGEditor/Public/WidgetBlueprint.h"
#endif
#include "BladeIIGameMode.h"

#include "FB2ErrorMessage.h"


const FVector2D UB2UnitedWidgetBase::WrongScreenPosition = FVector2D(FLT_MAX, FLT_MAX);

/** 
 * UI Widget 제작 시 사용되는 레퍼런스 크기. 공식적으로 지원되지 않는 widget 스크린 좌표 계산에 필요. 
 * UI DPI 스케일에서 이 중 한변에 대한 값은 1.0 으로 쓰긴 하지만 두 값이 모두 박혀있는 건 아니다.
 * DPI 커브 특성에 따라 둘 중 하나는 무시해야..
 */
const FVector2D UB2UnitedWidgetBase::DesignedRefViewSize = FVector2D(1280.0f, 720.0f); 
bool UB2UnitedWidgetBase::bUseButtonStunForDuration = true;

UB2UnitedWidgetBase::UB2UnitedWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, GoodsPopupType(EGoodsInfoPopupType::EGoodsPopup_None)
{
	bForceHidden = false;
	VisibilityBeforeForceHidden = ESlateVisibility::Visible;
	DynCreatedDummyParent = NULL;
	bButtonsStunnedThisTick = false;
	bHasStunnedButtons = false;
	bManualScrollBoxHandling_Sender = false;
	bManualScrollBoxHandling_Receiver = false;
	ManualScrollBoxSensitivity = 1.0f;
	TimeSenderBtnAreaPressStarted = 0.0;
	bSenderBtnAreaPressed = false;
//	bIsManualScrollBoxTouching = false;
	
	bIsPooledObject = false;
	bCurrentlyRentAsPooledObject = false;

	if (GConfig)
	{ // 여러 번 값을 가져오게 될 것.. 그냥 감안하고 여기에 둠.
		GConfig->GetBool(TEXT("/Script/BladeII"), TEXT("UseButtonStunForDuration"), bUseButtonStunForDuration, GGameIni);
	}
}

void UB2UnitedWidgetBase::NativeConstruct()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_NativeConstruct);
	Super::NativeConstruct();
//		
//	if (WidgetTree)
//	{
//		ColorToMIDParamMappedImages.Empty();
//		AllB2Buttons.Empty();
//		TickableButtons.Empty();
//
//		WidgetTree->ForEachWidget([&](UWidget* Widget) {
//			if (UB2Image* CastedB2Image = Cast<UB2Image>(Widget))
//			{ // ColorToMIDParamMapping 리스트 취합
//				if (CastedB2Image->IsUsingColorToMIDParamMapping())
//				{
//					ColorToMIDParamMappedImages.AddUnique(CastedB2Image);
//				}
//			}
//			else if (UB2Button* CastedB2Button = Cast<UB2Button>(Widget))
//			{
//				CastedB2Button->RegisterExtraStuff(this);
//
//				if (CastedB2Button->IsTickRequired())
//				{
//					TickableButtons.Add(CastedB2Button);
//				}
//				AllB2Buttons.Add(CastedB2Button);
//			}
//		});
//	}
//
////	RegisterUIMarkForRedDot();
}

void UB2UnitedWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_NativeTick);
	Super::NativeTick(MyGeometry, InDeltaTime);
	//	
	//if (IsPlayingAnimation()) // PlayingAnimation 도중에만 TransferColorToMIDParam 를 사용.
	//{
	//	for (UB2Image* CurrImage : ColorToMIDParamMappedImages)
	//	{
	//		if (CurrImage && CurrImage->IsUsingColorToMIDParamMapping())
	//		{
	//			CurrImage->TransferColorToMIDParam();
	//		}
	//	}
	//}

	//for (UB2Button* CurrBtn : TickableButtons)
	//{
	//	if (CurrBtn)
	//	{
	//		CurrBtn->ManualTick(InDeltaTime);
	//	}
	//}

	//if (bHasStunnedButtons) // 하나라도 남아 있으면
	//{
	//	RestoreButtonsFromStun();
	//}
}

void UB2UnitedWidgetBase::PostInitProperties()
{
	Super::PostInitProperties();
}

bool UB2UnitedWidgetBase::Initialize()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_Initialize);
	Super::Initialize();

	GetSlotInternal(GetRootWidget());

	return true;
}

void UB2UnitedWidgetBase::UpdateServercmdGetAccount()
{

}

UWidgetAnimation* UB2UnitedWidgetBase::GetAnimation(const FName& animName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_GetAnimation_FName);
	//UObjectProperty* AnimProperty = FindField<UObjectProperty>(this->GetClass(), animName);

	//if (AnimProperty == nullptr)
	//{
		return nullptr;
	//}

	//auto* ContainedAnim = AnimProperty->GetPropertyValue_InContainer(this);
	//auto* Anim = Cast<UWidgetAnimation>(ContainedAnim);

	//return Anim;
}

TWeakObjectPtr<UWidgetAnimation> UB2UnitedWidgetBase::GetAnimation(const UObject* OuterObject, const FName& animName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_GetAnimation);
	//UObjectProperty* AnimProperty = FindField<UObjectProperty>(OuterObject->GetClass(), animName);

	//if (AnimProperty == nullptr)
	//	return TWeakObjectPtr<UWidgetAnimation>(nullptr);

	//auto* ContainedAnim = AnimProperty->GetPropertyValue_InContainer(OuterObject);
	//auto* Anim = Cast<UWidgetAnimation>(ContainedAnim);
	//
	//return TWeakObjectPtr<UWidgetAnimation>(Anim);
	return NULL;
}

FText UB2UnitedWidgetBase::GetTimeTextFromSecond(int32 nSecond)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_GetTimeTextFromSecond);
	//분이랑 초까지 구현
	FString strTime = TEXT("");

	// 분
	if (nSecond >= 60)
	{
		strTime += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Time_NM")), FText::AsNumber(nSecond / 60)).ToString();
		strTime += TEXT(" ");
	}

	// 초
	strTime += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Time_NS")), FText::AsNumber(nSecond % 60)).ToString();

	return FText::FromString(strTime);
}

UWidget* UB2UnitedWidgetBase::GetSlotInternal(FName SlotName, UClass* TargetClass, UWidget* CurrWidget)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_GetSlotInternal_FName);
	if (!CurrWidget) return nullptr;

	// 먼저 현재 Widget자신과 비교
	if (CurrWidget->GetFName() == SlotName && CurrWidget->IsA(TargetClass))
		return CurrWidget;

	// CurrWidget의 내부 Element들과 비교
	if (auto NamedSlotHost = Cast<INamedSlotInterface>(CurrWidget))
	{
		TArray<FName> SlotNames;
		NamedSlotHost->GetSlotNames(SlotNames);

		for (FName s : SlotNames)
		{
			if (UWidget* ThisSlot = NamedSlotHost->GetContentForSlot(s))
			{
				if (ThisSlot->GetFName() == SlotName && ThisSlot->IsA(TargetClass))
					return ThisSlot;
			}
		}
	}
	if (auto PanelParent = Cast<UPanelWidget>(CurrWidget))
	{
		for (int32 i = 0; i < PanelParent->GetChildrenCount(); ++i)
		{
			if (UWidget* Child = PanelParent->GetChildAt(i))
			{
				if (Child->GetFName() == SlotName && Child->IsA(TargetClass))
					return Child;
			}
		}
	}

	// CurrWidget의 자식으로 재귀 호출
	if (auto NamedSlotHost = Cast<INamedSlotInterface>(CurrWidget))
	{
		TArray<FName> SlotNames;
		NamedSlotHost->GetSlotNames(SlotNames);

		for (FName s : SlotNames)
		{
			if (UWidget* ThisSlot = NamedSlotHost->GetContentForSlot(s))
			{
				auto Out = GetSlotInternal(SlotName, TargetClass, ThisSlot);
				if (Out)
					return Out;
			}
		}
	}
	if (auto PanelParent = Cast<UPanelWidget>(CurrWidget))
	{
		for (int32 i = 0; i < PanelParent->GetChildrenCount(); ++i)
		{
			if (UWidget* Child = PanelParent->GetChildAt(i))
			{
				auto Out = GetSlotInternal(SlotName, TargetClass, Child);
				if (Out)
					return Out;
			}
		}
	}
	if (auto UIWidget = Cast<UUserWidget>(CurrWidget))
	{
		return nullptr;
		return GetSlotInternal(SlotName, TargetClass, UIWidget->GetRootWidget());
	}

	return nullptr;
}

void UB2UnitedWidgetBase::GetSlotInternal(UWidget* CurrentWidget)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_GetSlotInternal);
	if (CurrentWidget == nullptr)
		return;

	if (auto NamedSlotHost = Cast<INamedSlotInterface>(CurrentWidget))
	{
		TArray<FName> SlotNames;
		NamedSlotHost->GetSlotNames(SlotNames);

		for (FName s : SlotNames)
		{
			if (UWidget* ThisSlot = NamedSlotHost->GetContentForSlot(s))
			{
				CachedWidgets.Add(ThisSlot->GetFName(), ThisSlot);
			}
		}
	}

	if (auto PanelParent = Cast<UPanelWidget>(CurrentWidget))
	{
		for (int32 i = 0; i < PanelParent->GetChildrenCount(); ++i)
		{
			if (UWidget* Child = PanelParent->GetChildAt(i))
			{
				CachedWidgets.Add(Child->GetFName(), Child);
			}
		}
	}

	// CurrWidget의 자식으로 재귀 호출
	if (auto NamedSlotHost = Cast<INamedSlotInterface>(CurrentWidget))
	{
		TArray<FName> SlotNames;
		NamedSlotHost->GetSlotNames(SlotNames);

		for (FName s : SlotNames)
		{
			if (UWidget* ThisSlot = NamedSlotHost->GetContentForSlot(s))
			{
				GetSlotInternal(ThisSlot);
			}
		}
	}

	if (auto PanelParent = Cast<UPanelWidget>(CurrentWidget))
	{
		for (int32 i = 0; i < PanelParent->GetChildrenCount(); ++i)
		{
			if (UWidget* Child = PanelParent->GetChildAt(i))
			{
				GetSlotInternal(Child);
			}
		}
	}

	if (auto UIWidget = Cast<UUserWidget>(CurrentWidget))
	{
		CachedWidgets.Add(CurrentWidget->GetFName(), CurrentWidget);
	}

	// if root
	if (CurrentWidget->GetClass() == GetRootWidget()->GetClass() &&
		CurrentWidget->GetFName() == GetRootWidget()->GetFName())
	{
		CachedWidgets.Add(CurrentWidget->GetFName(), CurrentWidget);
	}
}

#if 0 // 이 기능 있으면 나쁠 거 없는데.. 혹시 아는 사람 있으면 구현 시도를..
UWidgetAnimation* UB2UnitedWidgetBase::GetAnimation(FString AnimName, UWidget* BaseWidget)
{
	// 현재 이 기능은 제대로 동작하지 않으며 사용하지 말 것.
	// 아래 WidgetAnim 의 Outer 는 WidgetBlueprint 이지 UserWidget 이 아니다.
	// 게다가 WidgetBlueprint 가 Editor 용 오브젝트란 건 덤.
	// 물론 이렇게 ObjectIterator 를 도는 것도 별로 좋아보이지 않음.

	// check 를 걸고 싶지만 빌드 안정성을 위해 지금 당장 릴리즈 크래쉬는 내지 않는다.
	//checkSlow(0);

	//for (TObjectIterator<UWidgetAnimation> It; It; ++It)
	//{
	//	UWidgetAnimation* WidgetAnim = *It;

	//	if (WidgetAnim->MovieScene->GetName() == AnimName && WidgetAnim->GetOuter() == BaseWidget)
	//	{
	//		return WidgetAnim;
	//	}
	//}
	return nullptr;
}
#endif

void UB2UnitedWidgetBase::DestroySelfCommon()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_DestroySelfCommon);
	//UnRegisterUIMarkForRedDot();
	//ClearUITabList();

	//ColorToMIDParamMappedImages.Empty();
	//AllB2Buttons.Empty();
	//TickableButtons.Empty();

	//if (WidgetTree)
	//{
	//	WidgetTree->ForEachWidget([&](UWidget* Widget) {
	//		if (UB2Button* CastedB2Button = Cast<UB2Button>(Widget))
	//		{
	//			CastedB2Button->UnregisterExtraStuff(this);
	//		}
	//	});
	//}

	//ConditionalClearDynCreatedDummyParent();

	//RemoveFromParent();
	//RemoveFromViewport();
	//// A little special case as a slate widget.
	//MarkPendingKill();

	//UnRegisterUIMarkForRedDot();
}

void UB2UnitedWidgetBase::UnRegisterUIMarkForRedDot()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_UnRegisterUIMarkForRedDot);
	for (auto& RedDotCondition : RedDotConditionCheckList)
	{
		RedDotCondition.ConditionCheckHandler.Unbind();
	}
}

void UB2UnitedWidgetBase::DoMarkRedDot()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_DoMarkRedDot);
	for (auto& RedDotCondition : RedDotConditionCheckList)
	{
		if (RedDotCondition.RedDotWidget)
		{
			bool bShowRedDot = false;
			checkSlow(RedDotCondition.ConditionCheckHandler.IsBound()); // 이전 구현에 바로 RedDotCondition.ConditionCheckHandler.Execute 콜을 했던 걸 볼 때 기본 가정은 Bound 되어야 하는 것인듯..
			bShowRedDot = RedDotCondition.ConditionCheckHandler.IsBound() ? RedDotCondition.ConditionCheckHandler.Execute() : false;
			RedDotCondition.RedDotWidget->SetVisibility(bShowRedDot ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
			RedDotCondition.OnRedDotNotify.ExecuteIfBound(bShowRedDot);
		}
	}
}

void UB2UnitedWidgetBase::ForceMarkRedDot(bool bShow)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_ForceMarkRedDot);
	for (auto& RedDotCondition : RedDotConditionCheckList)
	{
		if (RedDotCondition.RedDotWidget)
		{
			RedDotCondition.RedDotWidget->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
			RedDotCondition.OnRedDotNotify.ExecuteIfBound(bShow);
		}
	}
}

void UB2UnitedWidgetBase::SetButtonsStunForDuration(UB2Button* InJustPressedButton)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_RestoreButtonsFromSingleTickDisable);
	if (bButtonsStunnedThisTick || !bUseButtonStunForDuration) {
		// Slate 에서 처리하는 것에 따라 한 틱에 이게 여러번 들어올 수 있을지도. 걍 젤 먼저 들어온 것에 대해서만 반응..
		return; // 근데 실제로 그런 상황이라면 이 코드가 무용지물인 거 아닌가.. 여럿 들어올 수 있다는 거 자체가..
	}

	for (UB2Button* ThisBtn : AllB2Buttons)
	{
		if (ThisBtn 
			// 이미 Stun 상태인 것을 중복 호출하면 시간 카운트가 리셋되어서 더 긴 시간 stun 이 되는데.. 중복 호출하는 게 맞나 아님 스킵하는 게 나은가..
			&& !ThisBtn->IsStunned()
			// 이걸 호출한 당사자가 단일 tick 만 stun 걸리는 버튼이라면 자신은 stun 하지 않고, 일정 시간 stun 이라면 자신도 stun.
			// 단일 tick stun 은 적어도 자주 눌리고 빠른 반응이 필요할 수 있는 버튼으로 간주하고, 일정 시간 stun 은 그 자체로서 자신도 stun 해야 맞는 것.
			&& (!InJustPressedButton || !InJustPressedButton->ShouldStunSingleTickOnly() || (ThisBtn != InJustPressedButton))
			) 
		{ 
			// SetIsEnabled 대신 따로 마련한 걸 사용. SetIsEnabled 은 기타 다른 목적으로 사용될 수 있기 때문.
			ThisBtn->StunForDuration();
		}
	}
	bButtonsStunnedThisTick = true; // 이건 단지 한 틱 유지 후 리셋될 것.
	bHasStunnedButtons = true; // 이건 모두 해제될 때까지 true
}

void UB2UnitedWidgetBase::RestoreButtonsFromStun()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_RestoreButtonsFromSingleTickDisable);

	bButtonsStunnedThisTick = false; // 이건 바로 리셋
	int32 StunnedButtonCount = 0;
	for (UB2Button* ThisBtn : AllB2Buttons)
	{
		if (ThisBtn) 
		{
			ThisBtn->TryRestoreFromStun();
			if (ThisBtn->IsStunned()) {
				++StunnedButtonCount;
			}
		}
	}

	if (StunnedButtonCount == 0)
	{
		bHasStunnedButtons = false; // 이후 다시 SetButtonsStunForDuration 호출시까지 이 루틴을 쉬게 된다.
	}
}

void UB2UnitedWidgetBase::ForceHide()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_ForceHide);
	if (bForceHidden == false)
	{
		bForceHidden = true;
		UWidget* RootWidget = GetRootWidget();
		if (RootWidget)
		{
			VisibilityBeforeForceHidden = RootWidget->GetVisibility();	//루트위젯의 Visibility를 조절하지 않고, 최종컨테이너의 Visibility를 조절하면 Open/Close 단의 숨김처리와 엉킴
			RootWidget->SetVisibility(ESlateVisibility::Hidden);
		}

		// ImpulseRing 을 쓰는 애들은 TickableButtons 에 포함되므로 Hide 시 Stop 해 줌. 아마 hide 이후에 Release 신호 못받는 듯.
		for (UB2Button* CurrBtn : TickableButtons)
		{
			if (CurrBtn)
			{
				CurrBtn->CustomStopOnTouchImpulseRing();
			}
		}
	}
}

void UB2UnitedWidgetBase::RestoreFromForceHidden()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_RestoreFromForceHidden);
	if (bForceHidden == true)
	{
		bForceHidden = false;
		// 마지막 ForceHide 이후에 SetVisibility 를 했다면 무시됨.
		UWidget* RootWidget = GetRootWidget();
		if (RootWidget && 
			!ShouldBeHiddenAsPooledObject() // PooledObject 의 visibility 처리는 또한 필요에 따라 직접 껀바이껀으로도..
			)
		{
			RootWidget->SetVisibility(VisibilityBeforeForceHidden);
		}
	}
}

FVector2D UB2UnitedWidgetBase::GetDesignToCurrentViewScale(class APlayerController* InPlayerController, bool bIgnoreNonScaledSide)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_GetDesignToCurrentViewScale);

	const UUserInterfaceSettings* UISettings = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass());
	// LongestSide 나 Horizontal 이면 Y 방향 해상도 변화에 따른 stretch 는 없을 것.
	const bool bIgnoreY = (UISettings && (UISettings->UIScaleRule == EUIScalingRule::LongestSide || UISettings->UIScaleRule == EUIScalingRule::Horizontal));

	FVector2D RetScale(1.0f, 1.0f);
	if (InPlayerController && UISettings)
	{
		int32 ViewSizeX, ViewSizeY;
		InPlayerController->GetViewportSize(ViewSizeX, ViewSizeY);
		// UIScaleRule 에 따라 실제로 stretch 되지 않는 방향으로는 scale 을 무시
		if (bIgnoreY)
		{
			RetScale = FVector2D((float)ViewSizeX / DesignedRefViewSize.X, bIgnoreNonScaledSide ? 1.0f : ((float)ViewSizeY / DesignedRefViewSize.Y));
		}
		else
		{
			RetScale = FVector2D(bIgnoreNonScaledSide ? 1.0f : ((float)ViewSizeX / DesignedRefViewSize.X), (float)ViewSizeY / DesignedRefViewSize.Y);
		}
	}
	return RetScale;
}

bool UB2UnitedWidgetBase::HACKGetWidgetScreenCoord(UWidget* InWidget, APlayerController* InPC, FVector2D& OutResultCoord, bool bWidgetCenterCoord, bool bIgnoreNonScaledSide)
{
#if 1
	if (InWidget == nullptr)
		return false;

	OutResultCoord = GetWidgetScreenPos(InWidget, bWidgetCenterCoord ? FVector2D(0.5f, 0.5f) : FVector2D(0.f, 0.f));

	return OutResultCoord != WrongScreenPosition;
#else
	if (!InWidget || !InPC){
		return false;
	}

	//================================================================================
	//		사용 전 필독 !
	//
	// 공식적으로 slate widget 의 스크린 공간 좌표를 알아내는 기능은 지원되지 않는다.
	// 여기서는 몇가지 가정을 기반으로 좌표를 계산해 내는데, 
	// 스크린 전체 크기의 CanvasPanel 상에 한쪽으로 strech 되는 앵커를 사용하지 않도록 배치한 경우 가능.
	// 어떤 조건은 코드 상으로 알아내고 return false 할 수 없으므로 가정과 어긋난 widget 을 인자로 넣어주었을 때 잘못된 결과를 리턴할 수 있음.
	// 또한 처음엔 잘 되다가도 UMG 상에서 배치 구성을 바꾸면 의도한 값을 리턴하지 않을 수 있으므로 주의해야 한다.
	// 추가로 특정 크기로 (DesignedRefViewSize) UMG 작업이 되었다는 가정이 있는데 이건 전역적인 가정이라 쉽게 바뀌진 않을 것.
	//================================================================================

	UCanvasPanelSlot* CPSlot = Cast<UCanvasPanelSlot>(InWidget->Slot);
	if (!CPSlot){
		return false;
	}

	const FAnchors& SlotAnchor = CPSlot->LayoutData.Anchors;
	if (SlotAnchor.IsStretchedVertical() || SlotAnchor.IsStretchedHorizontal()){
		return false;
	}

	// Anchor 의 Min 값과 Max 값이 다른 걸 걸러내므로 이하 둘 중 아무거나 하나 사용.
	// 여기에 기본 앵커 값은 0.0 ~ 1.0 스케일된 값을 사용하는데, 계산을 위해 레퍼런스 뷰 사이즈로 스케일한다.
	const FVector2D RefScaledAnchor = SlotAnchor.Minimum * DesignedRefViewSize;

	// CanvasPanelSlot 이 들고 있는 좌표 값을 앵커에 따라 보정. Stretch 된 앵커의 경우도 값이 필요한 상황이 된다면 추가 계산 필요.
	FVector2D NonScaledCoord = CPSlot->GetPosition();
	NonScaledCoord.X += RefScaledAnchor.X;
	NonScaledCoord.Y += RefScaledAnchor.Y;

	if (bWidgetCenterCoord)
	{
		NonScaledCoord += (CPSlot->GetSize() * 0.5f); // 기본적으로는 좌상단 기준 좌표임.
	}

	OutResultCoord = NonScaledCoord * GetDesignToCurrentViewScale(InPC, bIgnoreNonScaledSide); // 최종적으로 현재 뷰포트 사이즈로 스케일.

	return true;
#endif
}

FVector UB2UnitedWidgetBase::GetWorldPosBeneathScreen(APlayerController* InPC, FVector2D InScreenCoord, float DepthFromScreen, bool bDeprojectFromScreenCenter)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_GetWorldPosBeneathScreen);
	if (InPC)
	{
		int32 ViewSizeX, ViewSizeY;
		InPC->GetViewportSize(ViewSizeX, ViewSizeY);
		FVector ScreenCenterWorldPos;
		FVector ScreenCenterWorldDir(0.0f, 1.0f, 0.0f); // 이번 Deproject 에선 이것만 필요
		const bool bScreenCenterProjRes = bDeprojectFromScreenCenter ?
			InPC->DeprojectScreenPositionToWorld((float)ViewSizeX * 0.5f, (float)ViewSizeY * 0.5f, ScreenCenterWorldPos, ScreenCenterWorldDir) :
			false;

		FVector ResWorldPos; // 여기선 Pos 가 필요.
		FVector ResWorldDir; // bDeprojectFromScreenCenter 가 false 일 때 내지는 폴백용. 근데 이게 더 잘먹히는 거 같은데..
		if (InPC->DeprojectScreenPositionToWorld(InScreenCoord.X, InScreenCoord.Y, ResWorldPos, ResWorldDir))
		{ // DepthFromScreen 만큼 화면 안쪽으로. 물론 이게 먹히려면 depth 는 너무 커선 안됨.
			return ResWorldPos + ((bScreenCenterProjRes ? ScreenCenterWorldDir : ResWorldDir) * DepthFromScreen);
		}
	}
	return FVector(0.0f, 0.0f, 0.0f);
}

UB2UnitedWidgetBase* UB2UnitedWidgetBase::DynCreateInCanvasPanel(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, class UCanvasPanel* ParentPanel,
	FVector2D NonScaledSize, FVector2D FinalSlotSize, FVector2D RelPosInPanel, int ZOrderInPanel /*= 0*/, bool bPivotAtCenter /*= false*/)
{
	//[FIXME][@AKI, 171009] 이게 장비함 로딩하는 시간을 많이 소요 하고 있음
	// -> [FIXING] B2CLT-1893 .. WidgetPool 을 사용해서 동적 생성 자체를 덜 하도록..

	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_DynCreateInCanvasPanel);
	if (InClass == NULL || OwnerUserWidget == NULL || ParentPanel == NULL){
		return NULL;
	}

	UB2UnitedWidgetBase* CreatedWidget = CreateWidget<UB2UnitedWidgetBase>(OwnerUserWidget->GetOwningPlayer(), InClass);
	if (CreatedWidget == NULL){
		return NULL;
	}
	
	// 여기서 CreateWidget 안 하고 밖에서 한다음 CanvasPanel 에만 추가하는 루틴이 필요해서 따로 빼 놓음.
	if (PutWidgetInCanvasPanel(OwnerUserWidget, CreatedWidget, ParentPanel, NonScaledSize, FinalSlotSize, RelPosInPanel, ZOrderInPanel, bPivotAtCenter))
	{
		return CreatedWidget;
	}
	return nullptr;
}

bool UB2UnitedWidgetBase::PutWidgetInCanvasPanel(UB2UnitedWidgetBase* OwnerUserWidget, UB2UnitedWidgetBase* TargetWidget, class UCanvasPanel* ParentPanel,
	FVector2D NonScaledSize, FVector2D FinalSlotSize, FVector2D RelPosInPanel, int ZOrderInPanel /*= 0*/, bool bPivotAtCenter /*= false*/)
{
	//
	// 실질적으로는 DynCreateInCanvasPanel 의 일부인데 WidgetPool 구현하면서
	// CreateWidget 하는 거랑 여기서 CanvasPanel 에 넣는 거랑 따로 쓸 일이 있어서 구분함.
	//

	if (TargetWidget == NULL || OwnerUserWidget == NULL || ParentPanel == NULL) {
		return false;
	}

	// ParentPanel 크기에 따른 스케일 조절을 위해 ScaleBox 안에 생성한다.
	UScaleBox* OuterSB = NewObject<UScaleBox>(OwnerUserWidget, NAME_None, RF_Transient);
	if (!OuterSB) {
		return false;
	}

	UScaleBoxSlot* AddedSBSlot = Cast<UScaleBoxSlot>(OuterSB->AddChild(TargetWidget));
	if (AddedSBSlot)
	{ // 크기는 ScaleBox 조절로 하고 ScaleBox 안에는 꽉 채우는 걸로.

		if (NonScaledSize.X > NonScaledSize.Y) // ScaleBox 사용하면 NonScaledSize 가 필요없어질 줄 알았더니 Alignment 적당히 맞추려면 좀 필요하군 ㅋ
		{
			AddedSBSlot->SetHorizontalAlignment(HAlign_Fill);
			AddedSBSlot->SetVerticalAlignment(VAlign_Center);
		}
		else if (NonScaledSize.X < NonScaledSize.Y)
		{
			AddedSBSlot->SetHorizontalAlignment(HAlign_Center);
			AddedSBSlot->SetVerticalAlignment(VAlign_Fill);
		}
		else
		{
			AddedSBSlot->SetHorizontalAlignment(HAlign_Fill);
			AddedSBSlot->SetVerticalAlignment(VAlign_Fill);
		}

		TargetWidget->SetDynCreatedDummyParent(OuterSB); // CreatedWidget 을 제거할 때 OuterSB 도 같이 제거되도록 등록.
	}

	UCanvasPanelSlot* AddedCPSlot = Cast<UCanvasPanelSlot>(ParentPanel->AddChild(OuterSB));
	if (AddedCPSlot)
	{
		// CanvasPanelSlot 이 있어야 위치, 사이즈 조정 등이 됨.

		OuterSB->SetRenderTransformPivot(bPivotAtCenter ? FVector2D(0.5f, 0.5f) : FVector2D(0.0f, 0.0f));
		AddedCPSlot->SetSize(FinalSlotSize);
		AddedCPSlot->SetPosition(RelPosInPanel);
		AddedCPSlot->SetZOrder(ZOrderInPanel);
	}

	return true;
}

UB2UnitedWidgetBase* UB2UnitedWidgetBase::DynCreateInCanvasPanelFull(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, UCanvasPanel* ParentPanel)
{
	// 전체 화면 크기로 UIP 생성 시 사용. WidgetBP 에 그냥 배치해서 기본 로딩 시간을 먹는 대신 on-demand 로 dynamic 하게 생성해서 사용하는 등의 경우.. 
	// 이렇게 쓰는 대신 별도 ID (UIEnum) 로 등록해서 사용하고 싶은 경우도 있을 건데 그건 상황에 따라..
	if (InClass && OwnerUserWidget && ParentPanel)
	{
		UB2UnitedWidgetBase* NewCreated = CreateWidget<UB2UnitedWidgetBase>(OwnerUserWidget->GetOwningPlayer(), InClass);
		if (NewCreated)
		{
			UCanvasPanelSlot* AddedSlot = Cast<UCanvasPanelSlot>(ParentPanel->AddChild(NewCreated));
			if (AddedSlot)
			{
				// 상하좌우 꽉 채움.
				AddedSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
				AddedSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
			}
			return NewCreated;
		}
	}
	return nullptr;
}

UB2UnitedWidgetBase* UB2UnitedWidgetBase::DynCreateInVerticalBox(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, class UVerticalBox* ParentVB, FMargin InPlacementMargin)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_DynCreateInVerticalBox);
	if (InClass == NULL || OwnerUserWidget == NULL || ParentVB == NULL)
	{
		return NULL;
	}

	UB2UnitedWidgetBase* CreatedWidget = CreateWidget<UB2UnitedWidgetBase>(OwnerUserWidget->GetOwningPlayer(), InClass);
	if (CreatedWidget == NULL)
	{
		return NULL;
	}

	UVerticalBoxSlot* AddedSlot = Cast<UVerticalBoxSlot>(ParentVB->AddChild(CreatedWidget));
	if (AddedSlot)
	{
		AddedSlot->SetPadding(InPlacementMargin); // Vertical box 가 걍 위에서부터 순서대로 채우는 거라.. 몇가지 옵션이 더 있긴 한데 사실상 padding 말고는 딱히 없을듯?
	}

	return CreatedWidget;
}

UB2UnitedWidgetBase* UB2UnitedWidgetBase::DynCreateInHorizontalBox(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, class UHorizontalBox* ParentHB, FVector2D InElementSize, FMargin InPlacementMargin)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_DynCreateInHorizontalBox);
	if (InClass == NULL || OwnerUserWidget == NULL || ParentHB == NULL)
	{
		return NULL;
	}

	USizeBox* OptionalOuterSizeBox = NULL;
	if (InElementSize.X > 0.0f && InElementSize.Y > 0.0f)
	{
		// 이 경우 지정된 사이즈를 맞추기 위해 SizeBox 를 생성.
		OptionalOuterSizeBox = NewObject<USizeBox>(OwnerUserWidget, NAME_None, RF_Transient);
		if (OptionalOuterSizeBox)
		{
			OptionalOuterSizeBox->SetWidthOverride(InElementSize.X);
			OptionalOuterSizeBox->SetHeightOverride(InElementSize.Y);
		}
	}

	UB2UnitedWidgetBase* CreatedWidget = CreateWidget<UB2UnitedWidgetBase>(OwnerUserWidget->GetOwningPlayer(), InClass);
	if (CreatedWidget == NULL)
	{
		return NULL;
	}

	UHorizontalBoxSlot* AddedHBSlot = Cast<UHorizontalBoxSlot>(ParentHB->AddChild(CreatedWidget));

	if (OptionalOuterSizeBox)
	{
		// SizeBox 를 생성한 경우 한단계 더..
		USizeBoxSlot* AddedSizeBoxSlot = Cast<USizeBoxSlot>(OptionalOuterSizeBox->AddChild(CreatedWidget));
		AddedHBSlot = Cast<UHorizontalBoxSlot>(ParentHB->AddChild(OptionalOuterSizeBox));

		CreatedWidget->SetDynCreatedDummyParent(OptionalOuterSizeBox); // CreatedWidget 을 제거할 때 OptionalOuterSizeBox 도 같이 제거되도록 등록.
	}
	else
	{
		AddedHBSlot = Cast<UHorizontalBoxSlot>(ParentHB->AddChild(CreatedWidget));
	}
	
	if (AddedHBSlot)
	{
		AddedHBSlot->SetPadding(InPlacementMargin);
	}

	return CreatedWidget;
}

UB2UnitedWidgetBase* UB2UnitedWidgetBase::DynCreateInWrapBox(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, class UWrapBox* ParentWB, FVector2D InElementSize, FMargin InPlacementMargin)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_DynCreateInWrapBox);
	if (InClass == NULL || OwnerUserWidget == NULL || ParentWB == NULL)
	{
		return NULL;
	}

	USizeBox* OptionalOuterSizeBox = NULL;
	if (InElementSize.X > 0.0f && InElementSize.Y > 0.0f)
	{
		// 이 경우 지정된 사이즈를 맞추기 위해 SizeBox 를 생성.
		OptionalOuterSizeBox = NewObject<USizeBox>(OwnerUserWidget, NAME_None, RF_Transient);
		if (OptionalOuterSizeBox)
		{
			OptionalOuterSizeBox->SetWidthOverride(InElementSize.X);
			OptionalOuterSizeBox->SetHeightOverride(InElementSize.Y);
		}
	}

	UB2UnitedWidgetBase* CreatedWidget = CreateWidget<UB2UnitedWidgetBase>(OwnerUserWidget->GetOwningPlayer(), InClass);
	if (CreatedWidget == NULL){
		return NULL;
	}

	UWrapBoxSlot* AddedWrapBoxSlot = NULL;
	if (OptionalOuterSizeBox)
	{
		// SizeBox 를 생성한 경우 한단계 더..
		USizeBoxSlot* AddedSizeBoxSlot = Cast<USizeBoxSlot>(OptionalOuterSizeBox->AddChild(CreatedWidget));
		AddedWrapBoxSlot = Cast<UWrapBoxSlot>(ParentWB->AddChild(OptionalOuterSizeBox));

		CreatedWidget->SetDynCreatedDummyParent(OptionalOuterSizeBox); // CreatedWidget 을 제거할 때 OptionalOuterSizeBox 도 같이 제거되도록 등록.
	}
	else
	{
		AddedWrapBoxSlot = Cast<UWrapBoxSlot>(ParentWB->AddChild(CreatedWidget));
	}

	if (AddedWrapBoxSlot)
	{
		AddedWrapBoxSlot->SetPadding(InPlacementMargin);
	}
	return CreatedWidget;
}

void UB2UnitedWidgetBase::SetDynCreatedDummyParent(UPanelWidget* InWidget)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_SetDynCreatedDummyParent);
	ConditionalClearDynCreatedDummyParent();
	DynCreatedDummyParent = InWidget; // 대체로 ScaleBox 나 SizeBox 류..
}
void UB2UnitedWidgetBase::ConditionalClearDynCreatedDummyParent()
{
	//if (DynCreatedDummyParent && GetParent() == DynCreatedDummyParent)
	//{ // DynCreatedDummyParent 가 있다면 이것도 같이 없애주어야 하는데.. 이게 만약 child 가 여러개가 있다면 곤란.. 그런 식으로 생성해서 사용하지는 말자.
	//	checkSlow(DynCreatedDummyParent->GetChildrenCount() == 1);
	//	this->RemoveFromParent();
	//	DynCreatedDummyParent->RemoveFromParent();
	//	DynCreatedDummyParent->MarkPendingKill();
	//	DynCreatedDummyParent = NULL;
	//}
}

void UB2UnitedWidgetBase::ActiveHighlight(const FName& HighlightWidgetName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_ActiveHighlight);
	//if (UB2HighlightWidget* HighlightWidget = GetSlot<UB2HighlightWidget>(HighlightWidgetName))
	//{
	//	HighlightWidgets.AddUnique(HighlightWidget);
	//	HighlightWidget->SetActivate(true);
	//}
}

void UB2UnitedWidgetBase::ClearHighlight()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_ClearHighlight);

	//if (HighlightWidgets.Num() <= 0)
	//	return;

	//for (UB2HighlightWidget* HighlightWidget : HighlightWidgets)
	//{
	//	if (HighlightWidget != nullptr)
	//		HighlightWidget->SetActivate(false);
	//}

	//HighlightWidgets.Empty();
}

void UB2UnitedWidgetBase::AddActiveHighlight(FVector2D fv2Size)
{
	//if (UB2HighlightWidget* pHighlightWidget = NewObject<UB2HighlightWidget>(this, NAME_None, RF_Transient))
	//{
	//	if (UCanvasPanel* RootCanvas = Cast<UCanvasPanel>(GetRootWidget()))
	//	{
	//		UCanvasPanelSlot* PanelSlot = RootCanvas->AddChildToCanvas(pHighlightWidget);
	//		if (PanelSlot)
	//		{
	//			PanelSlot->SetSize(fv2Size);
	//			PanelSlot->SetAnchors(FAnchors());
	//		}
	//	}

	//	HighlightWidgets.AddUnique(pHighlightWidget);
	//	pHighlightWidget->SetActivate(true);
	//}
}

//================================================================================
// Manual Scroll-Box handling. 

void UB2UnitedWidgetBase::SetupManualScrollBoxSender_byReceiver(UB2UnitedWidgetBase* InReceiver, UB2ScrollBox* HandlingScrollBox)
{ // Receiver 쪽에서 불러줌. Sender 는 보통 Receiver 안에 있는 하위 파트일 것이므로.
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_SetupManualScrollBoxSender_byReceiver);
	//if (InReceiver && HandlingScrollBox)
	//{
	//	// Sender 쪽 Visibility 세팅은 필요에 따라 적절히..
	//	this->bManualScrollBoxHandling_Sender = true;
	//	InReceiver->bManualScrollBoxHandling_Receiver = true;

	//	MyManualScrollBoxHandlingReceiver = InReceiver;
	//	MyManualScrollBox = HandlingScrollBox;

	//	OnSetupManualScrollBoxSender(InReceiver, HandlingScrollBox);
	//}
	//else
	//{
	//	this->bManualScrollBoxHandling_Sender = false;
	//}
}

//////////

FReply UB2UnitedWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_NativeOnMouseButtonDown);
	FReply SuperRet = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//if (bManualScrollBoxHandling_Sender)
	//{
	//	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) // 마우스 버튼 핸들링은 개발버전 용
	//	{
	//		OnPressedSenderBTNArea(); // Area 버튼이 있다면 여기서 입력 핸들링을

	//		bSenderBtnAreaPressed = true;
	//		TimeSenderBtnAreaPressStarted = FApp::GetCurrentTime();
	//	}

	//	if (MyManualScrollBoxHandlingReceiver.IsValid() && MyManualScrollBox.IsValid())
	//	{
	//		return MyManualScrollBoxHandlingReceiver->OnSBMouseButtonDownFromSender(MyManualScrollBox.Get(), InGeometry, InMouseEvent);
	//	}
	//}

	return SuperRet;
}
FReply UB2UnitedWidgetBase::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_NativeOnMouseButtonUp);
	FReply SuperRet = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	//if (bManualScrollBoxHandling_Sender)
	//{
	//	OnReleasedSenderBTNArea(); // Area 버튼이 있다면 여기서 입력 핸들링을
	//	if (bSenderBtnAreaPressed && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && (FApp::GetCurrentTime() - TimeSenderBtnAreaPressStarted) <= GetTimeToDecideInputClickOrTab())
	//	{
	//		OnClickedSenderBTNArea(); // Area 버튼이 있다면 여기서 입력 핸들링을
	//	}

	//	bSenderBtnAreaPressed = false;

	//	if (MyManualScrollBoxHandlingReceiver.IsValid() && MyManualScrollBox.IsValid())
	//	{
	//		return MyManualScrollBoxHandlingReceiver->OnSBMouseButtonUpFromSender(MyManualScrollBox.Get(), InGeometry, InMouseEvent);
	//	}
	//}

	return SuperRet;
}
FReply UB2UnitedWidgetBase::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_NativeOnMouseMove);
	FReply SuperRet = Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	//if (bManualScrollBoxHandling_Sender)
	//{
	//	if (MyManualScrollBoxHandlingReceiver.IsValid() && MyManualScrollBox.IsValid())
	//	{
	//		return MyManualScrollBoxHandlingReceiver->OnSBMouseMoveFromSender(MyManualScrollBox.Get(), InGeometry, InMouseEvent);
	//	}
	//}

	return SuperRet;
}
void UB2UnitedWidgetBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_NativeOnMouseLeave);
	Super::NativeOnMouseLeave(InMouseEvent);

	//if (bManualScrollBoxHandling_Sender)
	//{
	//	OnReleasedSenderBTNArea(); // Area 버튼이 있다면 여기서 입력 핸들링을

	//	// 마우스 호출 순서 보장이 안되어 임시로 막아둠..
	//	//bSenderBtnAreaPressed = false;

	//	if (MyManualScrollBoxHandlingReceiver.IsValid() && MyManualScrollBox.IsValid())
	//	{
	//		MyManualScrollBoxHandlingReceiver->OnSBMouseLeaveFromSender(MyManualScrollBox.Get(), InMouseEvent);
	//	}
	//}
}
FReply UB2UnitedWidgetBase::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_NativeOnTouchStarted);
	FReply SuperRet = Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	//if (bManualScrollBoxHandling_Sender)
	//{
	//	OnPressedSenderBTNArea(); // Area 버튼이 있다면 여기서 입력 핸들링을
	//	bSenderBtnAreaPressed = true;
	//	TimeSenderBtnAreaPressStarted = FApp::GetCurrentTime();

	//	if (MyManualScrollBoxHandlingReceiver.IsValid() && MyManualScrollBox.IsValid())
	//	{
	//		return MyManualScrollBoxHandlingReceiver->OnSBTouchStartedFromSender(MyManualScrollBox.Get(), InGeometry, InGestureEvent);
	//	}
	//}

	return SuperRet;
}
FReply UB2UnitedWidgetBase::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_NativeOnTouchMoved);
	FReply SuperRet = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
	
	//if (bManualScrollBoxHandling_Sender)
	//{
	//	if (MyManualScrollBoxHandlingReceiver.IsValid() && MyManualScrollBox.IsValid())
	//	{
	//		return MyManualScrollBoxHandlingReceiver->OnSBTouchMovedFromSender(MyManualScrollBox.Get(), InGeometry, InGestureEvent);
	//	}
	//}

	return SuperRet;
}
FReply UB2UnitedWidgetBase::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_NativeOnTouchEnded);
	FReply SuperRet = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	//if (bManualScrollBoxHandling_Sender)
	//{
	//	OnReleasedSenderBTNArea(); // Area 버튼이 있다면 여기서 입력 핸들링을
	//	if (bSenderBtnAreaPressed && (FApp::GetCurrentTime() - TimeSenderBtnAreaPressStarted) <= GetTimeToDecideInputClickOrTab())
	//	{
	//		OnClickedSenderBTNArea(); // Area 버튼이 있다면 여기서 입력 핸들링을
	//	}

	//	bSenderBtnAreaPressed = false;

	//	if (MyManualScrollBoxHandlingReceiver.IsValid() && MyManualScrollBox.IsValid())
	//	{
	//		return MyManualScrollBoxHandlingReceiver->OnSBTouchEndedFromSender(MyManualScrollBox.Get(), InGeometry, InGestureEvent);
	//	}
	//}

	return SuperRet;
}

UB2UnitedWidgetBase* UB2UnitedWidgetBase::GetRedDotWidget(UWidget* ParentWidget /*= nullptr*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_GetRedDotWidget);
	//return UB2UIManager::GetInstance()->GetRedDotWidget(this, ParentWidget);
	return NULL;
}

UB2UnitedWidgetBase* UB2UnitedWidgetBase::GetRedDotWidget(const FString& ParentWidgetName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_GetRedDotWidget_FString);
	//UWidget* ParentWidget = GetSlot<UWidget>(FName(*ParentWidgetName));
	//return UB2UIManager::GetInstance()->GetRedDotWidget(this, ParentWidget);
	return NULL;
}

UB2UnitedWidgetBase* UB2UnitedWidgetBase::GetRedDotWidgetFromSubWidget(UUserWidget* SubWidget)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_GetRedDotWidgetFromSubWidget);
	//return UB2UIManager::GetInstance()->GetRedDotWidget(SubWidget, nullptr);
	return NULL;
}

void UB2UnitedWidgetBase::ForceSetVisibleRedDot(UWidget* ParentWidget, bool bVisible)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_ForceSetVisibleRedDot);
	if (UB2UnitedWidgetBase* RedDotWidget = GetRedDotWidget(ParentWidget))
	{
		RedDotWidget->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UnitedWidgetBase::ForceSetVisibleRedDot(const FString& ParentWidgetName, bool bVisible)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_ForceSetVisibleRedDot_FString);
	if (UB2UnitedWidgetBase* RedDotWidget = GetRedDotWidget(ParentWidgetName))
	{
		RedDotWidget->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

//////////
//
//FReply UB2UnitedWidgetBase::OnSBMouseButtonDownFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_OnSBMouseButtonDownFromSender);
//	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
//	
//	if (InScrollBox && bManualScrollBoxHandling_Receiver)
//	{
//		return InScrollBox->OnSBMouseButtonDownFromSender(InGeometry, InMouseEvent);
//	}
//
//	return FReply::Handled();
//}
//
//FReply UB2UnitedWidgetBase::OnSBMouseButtonUpFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_OnSBMouseButtonUpFromSender);
//	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
//	
//	if (InScrollBox && bManualScrollBoxHandling_Receiver)
//	{
//		return InScrollBox->OnSBMouseButtonUpFromSender(InGeometry, InMouseEvent);
//	}
//
//	return FReply::Handled();
//}
//
//FReply UB2UnitedWidgetBase::OnSBMouseMoveFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_OnSBMouseMoveFromSender);
//	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
//	
//	if (InScrollBox && bManualScrollBoxHandling_Receiver)
//	{
//		return InScrollBox->OnSBMouseMoveFromSender(InGeometry, InMouseEvent);
//	}
//	return FReply::Unhandled();
//}
//
//void UB2UnitedWidgetBase::OnSBMouseLeaveFromSender(UB2ScrollBox* InScrollBox, const FPointerEvent& InMouseEvent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_OnSBMouseLeaveFromSender);
//	Super::NativeOnMouseLeave(InMouseEvent);
//
//	// 이건 굳이 전달 안해두 될듯.. 아이템 영역을 나간거지 스크롤영역을 나간게 아님
//	//if (InScrollBox && bManualScrollBoxHandling_Receiver)
//	//{
//	//	InScrollBox->OnSBMouseLeaveFromSender(InMouseEvent);
//	//}
//}
//
//FReply UB2UnitedWidgetBase::OnSBTouchStartedFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_OnSBTouchStartedFromSender);
//	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
//	
//	if (InScrollBox && bManualScrollBoxHandling_Receiver)
//	{
//		return InScrollBox->OnSBTouchStartedFromSender(InGeometry, InGestureEvent);
//	}
//
//	return FReply::Handled();
//}
//
//FReply UB2UnitedWidgetBase::OnSBTouchMovedFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_OnSBTouchMovedFromSender);
//	Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
//
//	if (InScrollBox && bManualScrollBoxHandling_Receiver)
//	{
//		return InScrollBox->OnSBTouchMovedFromSender(InGeometry, InGestureEvent);
//	}
//
//	return FReply::Unhandled();
//}
//
//FReply UB2UnitedWidgetBase::OnSBTouchEndedFromSender(UB2ScrollBox* InScrollBox, const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
//{
//	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_OnSBTouchEndedFromSender);
//	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
//	
//	if (InScrollBox && bManualScrollBoxHandling_Receiver)
//	{
//		return InScrollBox->OnSBTouchEndedFromSender(InGeometry, InGestureEvent);
//	}
//
//	return FReply::Handled();
//}

//================================================================================

void FProgressAnimateUtil::ApplyCustomTimerTick(UB2UnitedWidgetBase* InOwnerWidget, float DeltaTime)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FProgressAnimateUtil_ApplyCustomTimerTick);
	if (InOwnerWidget != nullptr && InOwnerWidget->GetOwningPlayer() != nullptr && DeltaTime > 0.f)
	{
		OwnerWidget = InOwnerWidget;
		if (OwnerWidget->GetOwningPlayer()->GetWorldTimerManager().IsTimerActive(CustomTickHandle) == false)
		{
			TickDelta = DeltaTime;
			OwnerWidget->GetOwningPlayer()->GetWorldTimerManager().SetTimer(CustomTickHandle, FTimerDelegate::CreateRaw(this, &FProgressAnimateUtil::Tick), TickDelta, true);
		}
	}
}

void FProgressAnimateUtil::ClearCustomTimerTick()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_ClearCustomTimerTick);
	if (OwnerWidget != nullptr && OwnerWidget->GetOwningPlayer() != nullptr)
	{
		OwnerWidget->GetOwningPlayer()->GetWorldTimerManager().ClearTimer(CustomTickHandle);
		OwnerWidget = nullptr;
	}
}

void FProgressAnimateUtil::Tick()
{
	const float DistToTarget = GetUnitDistanceToTargetValue();
	if (DistToTarget > 0.0f)
	{
		if (AnimType == EProgressAnimType::PRAT_SpringDamp)
		{
			// 용수철 모델. 레벨이 한번에 여러 단계를 뛰어야 하는 경우에 등속도로 하면 다양한 구간 차이를 커버할 수 있는 적당한 속도를 찾기가 어려울 때가 있어서 용수철 모델을 쓰면 좋음.
			CurrentSpeed += DistToTarget * SpringConst * TickDelta;
			CurrentSpeed -= CurrentSpeed * DampConst * TickDelta;
			// 이 경우에도 최소한의 속도는 나오게 함.
			CurrentSpeed = FMath::Max(CurrentSpeed, BaseSpeed);
		}
		else
		{
			CurrentSpeed = BaseSpeed;
		}

		CurrentAnimatedPercent += CurrentSpeed * TickDelta;
		if (CurrentAnimatedPercent >= 1.0f) // 레벨 올라가는 상황.
		{
			float AnimatedPctOver = CurrentAnimatedPercent - 1.0f;

			int32 AddedLevel = (int32)AnimatedPctOver + 1;
			CurrentAnimatedLevel += AddedLevel;
			CurrentAnimatedPercent = FMath::Frac(AnimatedPctOver);

			OnLevelChanged.Broadcast(CurrentAnimatedLevel);
		}
		OnPrecentChanged.Broadcast(CurrentAnimatedPercent);
	}

	// 다시금 목표값에 도달했는지 체크하고 클램프
	if (GetUnitDistanceToTargetValue() <= 0.0f)
	{ // 목표값에 도달한 상태
		SyncToTargetValues();
		ClearCustomTimerTick();

		OnFinishProgAnimation.Broadcast();
	}
}

void FProgressAnimateUtil::ManualTick(float InDeltaSecond)
{
	// ApplyCustomTimerTick 은 필요할 때만 Tick 이 불리게 하고자 하면 사용하고.. 꼭 그럴 필요 없으면 이렇게
	TickDelta = InDeltaSecond;
	Tick();
}

void FProgressAnimateUtil::SetTargetValues(int32 InTargetLevel, float InTargetPercentScale, bool bSyncCurrentValues)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FProgressAnimateUtil_SetTargetValues);
	TargetLevel = InTargetLevel;
	TargetPercentScale = InTargetPercentScale;

	if (bSyncCurrentValues)
	{
		SyncToTargetValues();
	}
}

void FProgressAnimateUtil::SyncToTargetValues()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FProgressAnimateUtil_SyncToTargetValues);
	CurrentAnimatedLevel = TargetLevel;
	CurrentAnimatedPercent = TargetPercentScale;
	CurrentSpeed = 0.0f;

	OnLevelChanged.Broadcast(CurrentAnimatedLevel);
	OnPrecentChanged.Broadcast(CurrentAnimatedPercent);
}

float FProgressAnimateUtil::GetUnitDistanceToTargetValue()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FProgressAnimateUtil_GetUnitDistanceToTargetValue);
	const int32 LevelDifference = TargetLevel - CurrentAnimatedLevel;
	const float PctDifference = TargetPercentScale - CurrentAnimatedPercent;

	if (LevelDifference < 0 || (LevelDifference == 0 && PctDifference <= 0.0f))
	{// 목표값에 도달한 상태
		return 0.0f;
	}

	return (float)LevelDifference + PctDifference;
}

bool FProgressAnimateUtil::IsApplied()
{
	return OwnerWidget != nullptr;
}

//================================================================================

FProgressiveTextBuilder::FProgressiveTextBuilder()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FProgressAnimateUtil_FProgressiveTextBuilder);
	TotalShowingTime = 1.0f;
	MaxCharShowInterval = 0.15f;
	CharShowInterval = 0.0f;
	ThisDeltaTime = 0.0f;
	LastRealTime = 0.0f;
	CurrentCharShowIndex = 0;
	LastCharUpdatedTime = 0.0f;
}

void FProgressiveTextBuilder::SetWholeText(const FString& InString, float InTotalTime, float InMaxCharShowInterval, float InThisRealTime)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FProgressAnimateUtil_SetWholeText);
	LastRealTime = InThisRealTime;
	IndexedSpanTagTextArray.Empty();
	WholeTextString.Empty();

	FString ToParseOriginText = InString;
	do{
		FString Left;
		FString Right;
		if (ToParseOriginText.Split(TEXT("<"), &Left, &Right))
		{
			WholeTextString += Left;

			if (Right.Split(TEXT(">"), &Left, &Right))
			{
				IndexedSpanTagTextArray.Add(IndexedSpanTagText(WholeTextString.Len(), Left));
				ToParseOriginText = Right;
			}
			else
			{
				//꺽쇠짝이 안맞아..
				BII_CHECK(false);
				break;
			}			
		}
		else
		{
			WholeTextString += ToParseOriginText;
			break;
		}

	} while (ToParseOriginText.Len() != 0);


	CurrentTextString.Empty(); // 이건 비우고 첨부터 시작

	TotalShowingTime = InTotalTime;
	MaxCharShowInterval = InMaxCharShowInterval;
	
	// 실제 계산에서 사용될 ShowInterval
	CharShowInterval = (TotalShowingTime > 0.0f) ?
		FMath::Min(TotalShowingTime / (float)FMath::Max(WholeTextString.Len(), 1), MaxCharShowInterval) : MaxCharShowInterval;

	// 처음 세팅시에는 0번째 글자부터 보여주는 걸로 시작.
	ApplyStringProgress(0);
}

void FProgressiveTextBuilder::SetTotalShowingTime(float InNewTime)
{
	TotalShowingTime = InNewTime;
	// 추가 처리 안해줘도 됨?
}

bool FProgressiveTextBuilder::Tick(float InThisRealTime, float InDeltaSec, bool& OutIsLastChar)
{
	ThisDeltaTime = InDeltaSec;
	LastRealTime = InThisRealTime;
	OutIsLastChar = false;
	
	const float TimeFromLastUpdate = FMath::Max(LastRealTime - LastCharUpdatedTime, 0.0f);
	const int32 ThisTimeShowCharacterNum = FMath::RoundToInt(TimeFromLastUpdate / CharShowInterval);

	if (ThisTimeShowCharacterNum > 0 && CurrentCharShowIndex < WholeTextString.Len() - 1)
	{
		ApplyStringProgress(CurrentCharShowIndex + ThisTimeShowCharacterNum); // 안에서 LastCharUpdatedTime 갱신.
		OutIsLastChar = IsStringFilledUp();
		return true;
	}

	return false;
}

void FProgressiveTextBuilder::ApplyStringProgress(int32 InNewCharacterIndex)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FProgressAnimateUtil_ApplyStringProgress);
	InNewCharacterIndex = FMath::Clamp(InNewCharacterIndex, 0, WholeTextString.Len() - 1);

	if (InNewCharacterIndex == 0 || (InNewCharacterIndex <= CurrentCharShowIndex) || (InNewCharacterIndex - CurrentCharShowIndex > 2))
	{ // 새로 시작하던지, Append 할 글자가 좀 많던지, 아님 뭔가 예상과 다른 경우. 걍 다시 세팅.
		CurrentTextString = WholeTextString.LeftChop(WholeTextString.Len() - (InNewCharacterIndex + 1));
	}
	else
	{ // 이전 꺼에 Append. 이게 매번 Chop 하는 것보다 빠를 거라는 기대로..
		for (int32 ThisIndex = CurrentCharShowIndex + 1; ThisIndex <= InNewCharacterIndex; ++ThisIndex)
		{
			CurrentTextString += WholeTextString[ThisIndex];
		}
	}

	// 다음 번 Apply 를 위해 기록해 둔다.
	CurrentCharShowIndex = InNewCharacterIndex;
	LastCharUpdatedTime = LastRealTime;
}

void FProgressiveTextBuilder::ApplyToWholeString()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FProgressAnimateUtil_ApplyToWholeString);
	ApplyStringProgress(WholeTextString.Len() - 1);
}

bool FProgressiveTextBuilder::IsStringFilledUp()
{
	return (CurrentCharShowIndex >= WholeTextString.Len() - 1);
}


FString FProgressiveTextBuilder::GetCurrentString() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FProgressAnimateUtil_GetCurrentString);
	FString rtnString = CurrentTextString;
	for (int32 idx = IndexedSpanTagTextArray.Num() - 1; idx > -1; --idx)
	{		
		if (CurrentTextString.Len() <= IndexedSpanTagTextArray[idx].Index)
		{
			if (CurrentTextString.Len() < rtnString.Len())
				rtnString.InsertAt(CurrentTextString.Len(), IndexedSpanTagTextArray[idx].SpanTagText);
			else
				rtnString.Append(IndexedSpanTagTextArray[idx].SpanTagText);
		}			
		else
			rtnString.InsertAt(IndexedSpanTagTextArray[idx].Index, IndexedSpanTagTextArray[idx].SpanTagText);
	}

	return rtnString; 
}

#if WITH_EDITOR && !PLATFORM_MAC
void ReplaceSubWidgetClass(UWidgetBlueprint* BasePage, TSubclassOf<UPanelWidget> ClassTobeRemoved, TSubclassOf<UPanelWidget> ClassToReplace, bool bSilent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ReplaceSubWidgetClass);
	//if (!BasePage || !ClassTobeRemoved || !ClassToReplace || !BasePage->WidgetTree){
	//	return;
	//}

	//if (!bSilent && FB2ErrorMessage::Open(EAppMsgType::OkCancel, FText::FromString(
	//	FString::Printf(TEXT("해당 기능 사용 도중에 컴퓨터가 폭발할 수 있음. 미처 저장하지 못한 작업물이 있다면 지금 취소한 후 다시 시도하시오."))
	//	)) == EAppReturnType::Cancel)
	//{
	//	return;
	//}

	//int32 AllReplacedNum = 0;
	//FString ResultMessage;

	//TArray<FString> AllChangedWidgetNames;

	//// FWidgetBlueprintEditorUtils::ReplaceWidgets 참고

	//BasePage->WidgetTree->ForEachWidget([&](UWidget* CurrWidget) {

	//	UPanelWidget* CastedExistingPW = Cast<UPanelWidget>(CurrWidget);
	//	if (CastedExistingPW && CastedExistingPW->IsA(ClassTobeRemoved) && !CastedExistingPW->IsA(ClassToReplace))
	//	{
	//		UPanelWidget* NewReplacement = BasePage->WidgetTree->ConstructWidget<UPanelWidget>(ClassToReplace, NAME_None);
	//		NewReplacement->OnCreationFromPalette();

	//		if (CastedExistingPW && NewReplacement)
	//		{
	//			CastedExistingPW->Modify();

	//			if (UPanelWidget* CurrentParent = CastedExistingPW->GetParent())
	//			{
	//				CurrentParent->Modify();
	//				CurrentParent->ReplaceChild(CastedExistingPW, NewReplacement);
	//			}
	//			else
	//			{
	//				check(0); // 루트를 갈아치우려는 경우?
	//			}

	//			while (CastedExistingPW->GetChildrenCount() > 0) // 자식들도 새 엄마에게
	//			{
	//				UWidget* Widget = CastedExistingPW->GetChildAt(0);
	//				Widget->Modify();

	//				UPanelSlot* OldSlot = Widget->Slot;
	//				UPanelSlot* NewAddedSlot = NewReplacement->AddChild(Widget);

	//				// 바꾸려는 버튼 아래에 오버레이를 까는 경우가 있어서 속성값 좀 전달. 여타 다른 경우도 많겠지만.
	//				UButtonSlot* NewAddedButtonSlot = Cast<UButtonSlot>(NewAddedSlot);
	//				UButtonSlot* OldButtonSlot = Cast<UButtonSlot>(OldSlot);
	//				if (NewAddedButtonSlot && OldButtonSlot)
	//				{
	//					NewAddedButtonSlot->SetHorizontalAlignment(OldButtonSlot->HorizontalAlignment);
	//					NewAddedButtonSlot->SetVerticalAlignment(OldButtonSlot->VerticalAlignment);
	//					NewAddedButtonSlot->SetPadding(OldButtonSlot->Padding);
	//				}
	//			}

	//			// FWidgetBlueprintEditorUtils::ReplaceWidgets 쪽에도 해 놓은 변경들. 몇가지 편의 사항

	//			FString ExistingPanelName = CastedExistingPW->GetName();
	//			UObject* ExistingPanelOuter = CastedExistingPW->GetOuter();

	//			// Rename the removed widget to the transient package so that it doesn't conflict with future widgets sharing the same name.
	//			CastedExistingPW->Rename(nullptr, nullptr);
	//			NewReplacement->Rename(*ExistingPanelName, ExistingPanelOuter);

	//			AllChangedWidgetNames.Add(ExistingPanelName);

	//			// 속성들 되는대로 맞춰주기. 기본 크기, 앵커 등은 Slot 설정이라 할 필요 없음.
	//			NewReplacement->SetVisibility(CastedExistingPW->GetVisibility());
	//			NewReplacement->SetIsEnabled(CastedExistingPW->GetIsEnabled());
	//			NewReplacement->SetRenderTransform(CastedExistingPW->RenderTransform);
	//			NewReplacement->SetRenderTransformPivot(CastedExistingPW->RenderTransformPivot);

	//			// B2Button 교체 special. For faster UBtton to UB2Button replacement
	//			UButton* CastedExistingButton = Cast<UButton>(CastedExistingPW);
	//			UButton* CastedNewButton = Cast<UButton>(NewReplacement);
	//			if (CastedExistingButton && CastedNewButton)
	//			{
	//				CastedNewButton->SetStyle(CastedExistingButton->WidgetStyle);
	//				CastedNewButton->SetColorAndOpacity(CastedExistingButton->ColorAndOpacity);
	//				CastedNewButton->SetBackgroundColor(CastedExistingButton->BackgroundColor);
	//			}

	//			++AllReplacedNum;
	//		}
	//	}
	//});

	//if (AllReplacedNum > 0)
	//{
	//	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BasePage);

	//	ResultMessage += FString::Printf(TEXT("%s 에 배치된 %d 개의 %s 들을 %s (으)로 변경\n\n"), *BasePage->GetName(), AllReplacedNum, *ClassTobeRemoved->GetName(), *ClassToReplace->GetName());

	//	for (int32 NI = 0; NI < AllChangedWidgetNames.Num(); ++NI)
	//	{
	//		ResultMessage += AllChangedWidgetNames[NI];
	//		if (NI != AllChangedWidgetNames.Num() - 1){
	//			ResultMessage += TEXT(", ");
	//		}
	//		else{
	//			ResultMessage += TEXT("\n\n");
	//		}
	//	}

	//	ResultMessage += TEXT("모든 설정들이 복사되는 것은 아니므로 바뀐 widget 들의 세부 설정들을 직접 확인해 보시기 바랍니다.");
	//}
	//else
	//{
	//	ResultMessage = FString::Printf(TEXT("%s 에서 바뀐 widget 이 없습니다."), *BasePage->GetName());
	//}
	//if (bSilent)
	//{
	//	UE_LOG(LogBladeII, Log, TEXT("%s"), *ResultMessage);
	//}
	//else
	//{
	//	FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(ResultMessage));
	//}
}

void ForceSetButtonClickSoundIndex(class UWidgetBlueprint* BasePage, int32 InSoundIndex, bool bSilent)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ForceSetButtonClickSoundIndex);
	if (!BasePage || !BasePage->WidgetTree){
		return;
	}

	int32 AllSetNum = 0;
	FString ResultMessage;
	TArray<FString> AllChangedButtonNames;

	BasePage->WidgetTree->ForEachWidget([&](UWidget* CurrWidget) {
		UB2Button* CastedB2Button = Cast<UB2Button>(CurrWidget);
		if (CastedB2Button)
		{
			CastedB2Button->EditorOnlySetClickSoundIndex(InSoundIndex);
			AllChangedButtonNames.Add(CastedB2Button->GetName());
			++AllSetNum;
		}
	});

	if (AllSetNum > 0)
	{
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BasePage);

		//ResultMessage += FString::Printf(TEXT("%s 에 배치된 %d 개의 B2Button 들의 DefaultOnClickSoundIndex 값을 %d (으)로 변경\n\n"), *BasePage->GetName(), AllSetNum, InSoundIndex);

		for (int32 NI = 0; NI < AllChangedButtonNames.Num(); ++NI)
		{
			ResultMessage += AllChangedButtonNames[NI];
			if (NI != AllChangedButtonNames.Num() - 1){
				ResultMessage += TEXT(", ");
			}
		}
	}
	else
	{
		ResultMessage = FString::Printf(TEXT("%s 에서 바뀐 widget 이 없습니다."), *BasePage->GetName());
	}
	if (bSilent)
	{
		UE_LOG(LogBladeII, Log, TEXT("%s"), *ResultMessage);
	}
	else
	{
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(ResultMessage));
	}
}
#endif

//SWidget의 위치 정보는 FGeomety에 담겨져있음.
//Widget의 Left-Top의 Screen 위치를 가지고 오며,
//아래의 함수는 UWidget의 FGeometry를 찾기 위한 재귀함수로
//개선의 여지가 있다면 마음껏 개선하기 바람.
FVector2D RecursiveFindWidgetPositionInArrangeChildren(const TSharedRef<SWidget>& Widget, const FGeometry& AllottedGeometry, UWidget* WidgetToFind, const FVector2D& SpotInWidgetSpace)
{
	BLADE2_SCOPE_CYCLE_COUNTER(RecursiveFindWidgetPositionInArrangeChildren);
	FArrangedChildren ArrangedChildren(EVisibility::All);
	Widget->ArrangeChildren(AllottedGeometry, ArrangedChildren);

	if (ArrangedChildren.Num() == 0)
		return UB2UnitedWidgetBase::WrongScreenPosition;

	for (auto Child : ArrangedChildren.GetInternalArray())
	{
		if (Child.Widget == WidgetToFind->GetCachedWidget())
		{
			return Child.Geometry.LocalToAbsolute(SpotInWidgetSpace * Child.Geometry.GetLocalSize());
		}
		
		FVector2D ChildPos = RecursiveFindWidgetPositionInArrangeChildren(Child.Widget, Child.Geometry, WidgetToFind, SpotInWidgetSpace);
		if (ChildPos != UB2UnitedWidgetBase::WrongScreenPosition)
			return ChildPos;
	}

	return UB2UnitedWidgetBase::WrongScreenPosition;
}

FVector2D UB2UnitedWidgetBase::GetWidgetScreenPos(UWidget* InWidget, const FVector2D& SpotInWidgetSpace, bool bForce)
{
	BLADE2_SCOPE_CYCLE_COUNTER(GetWidgetScreenPos);
	if (!InWidget || !InWidget->GetCachedWidget().IsValid())
		return WrongScreenPosition;

	TSharedPtr<SWindow> SlateWindow = FSlateApplication::Get().FindWidgetWindow(InWidget->GetCachedWidget().ToSharedRef());
	if (!SlateWindow.IsValid())
	{
		return WrongScreenPosition;		
	}
		
	if (bForce)
	{
		//Paint가 불린적이 없다면 제대로 나오지 않음.
		//경우에 따라 아래의 함수를 호출해도 제대로 나오지 않는 경우가 있는데, RenderThread가 동작하지 않는 순간인듯? 
		FSlateApplication::Get().ForceRedrawWindow(SlateWindow.ToSharedRef());
	}	
	
	FGeometry WindowGeom = SlateWindow->GetWindowGeometryInWindow();
	
	FVector2D ResultPosition = RecursiveFindWidgetPositionInArrangeChildren(SlateWindow.ToSharedRef(), WindowGeom, InWidget, SpotInWidgetSpace);
	
	if (ResultPosition != WrongScreenPosition)
	{
		FMargin WindowBorder = SlateWindow->GetWindowBorderSize();

		ResultPosition.X -= WindowBorder.Left;
		ResultPosition.Y -= WindowBorder.Top + (SlateWindow->GetTitleBarSize().IsSet() ? SlateWindow->GetTitleBarSize().Get() : 0);
	}

	return ResultPosition;
}

void UB2UnitedWidgetBase::OnChangeStateTabWidgets(UB2UICommonTab* TabWidget)
{
	BLADE2_SCOPE_CYCLE_COUNTER(OnChangeStateTabWidgets);
	//for (int32 i = 0; i < TabWidgetList.Num(); i++)
	//{
	//	TabWidgetList[i]->SetTabSelected(TabWidget == TabWidgetList[i]);
	//}
}

void UB2UnitedWidgetBase::OnRegistTabWidgets(UB2UICommonTab* TabWidget, FTabButtonDelegate TabButtonDelegate)
{
	BLADE2_SCOPE_CYCLE_COUNTER(OnRegistTabWidgets);
	//TabWidgetList.Add(TabWidget);

	//TabWidget->Init();
	//TabWidget->TabButtonDelegateBindLambda([this, TabWidget, TabButtonDelegate]() { OnChangeStateTabWidgets(TabWidget); TabButtonDelegate.ExecuteIfBound(); });
}

void UB2UnitedWidgetBase::ClearUITabList()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ClearUITabList);
	TabWidgetList.Empty();
}

bool UB2UnitedWidgetBase::ShouldBeHiddenAsPooledObject() const
{ 
	// 실제 hidden 동작은 케이스별로도 해야 할 듯.. 여기서 일반적인 루틴으로 처리하는 건 한계가 있을 듯 하다.
	return (bIsPooledObject && !bCurrentlyRentAsPooledObject); 
}

/** PooledObject 로 생성하고서..
 * 적절한 pool managing 오브젝트(FWidgetPoolBase extension)가 콜해야 함. 그냥 이거 콜한다고 pooling 되는 거 아님. */
void UB2UnitedWidgetBase::SetAsPooledObject()
{
	bIsPooledObject = true;

	SetCurrentlyRent(false, nullptr);
}
void UB2UnitedWidgetBase::SetCurrentlyRent(bool bInRent, UObject* InRenter)
{
	if (bIsPooledObject)
	{
		bCurrentlyRentAsPooledObject = bInRent;
		if (bCurrentlyRentAsPooledObject)
		{
			OnPooledObjectRentBP();
		}
		else
		{
			// 현재 Rent 된 상태에서는 각 하위 widget class 에서 상황에 따라 알맞게 Visibility 제어를 하도록 하고 반납한 경우만 숨겨둠.
			// Visibility 제어는 개별 widget class 별로 좀 신경을 써주어야 할 것. 여기에는 최소한의 인터페이스만 갖춤.
			SetVisibility(ESlateVisibility::Hidden);
			OnPooledObjectTurnInBP();
		}
	}
}

#if UE_BUILD_DEBUG
void DebugCheckOnLoadSynchronousUse(UObject* WorldContextObject)
{
	// 로비가 아닌 인게임 상황서 사용하면 곤란할 수 있는 기능에 대한 assert 목적.
	ABladeIIGameMode* CurrGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	checkSlow(CurrGM && CurrGM->GetB2GameModeType() == EB2GameMode::Lobby);
}
#endif