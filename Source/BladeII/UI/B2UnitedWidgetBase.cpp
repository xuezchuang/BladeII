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
 * UI Widget ���� �� ���Ǵ� ���۷��� ũ��. ���������� �������� �ʴ� widget ��ũ�� ��ǥ ��꿡 �ʿ�. 
 * UI DPI �����Ͽ��� �� �� �Ѻ��� ���� ���� 1.0 ���� ���� ������ �� ���� ��� �����ִ� �� �ƴϴ�.
 * DPI Ŀ�� Ư���� ���� �� �� �ϳ��� �����ؾ�..
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
	{ // ���� �� ���� �������� �� ��.. �׳� �����ϰ� ���⿡ ��.
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
//			{ // ColorToMIDParamMapping ����Ʈ ����
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
	//if (IsPlayingAnimation()) // PlayingAnimation ���߿��� TransferColorToMIDParam �� ���.
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

	//if (bHasStunnedButtons) // �ϳ��� ���� ������
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
	//���̶� �ʱ��� ����
	FString strTime = TEXT("");

	// ��
	if (nSecond >= 60)
	{
		strTime += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Time_NM")), FText::AsNumber(nSecond / 60)).ToString();
		strTime += TEXT(" ");
	}

	// ��
	strTime += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Time_NS")), FText::AsNumber(nSecond % 60)).ToString();

	return FText::FromString(strTime);
}

UWidget* UB2UnitedWidgetBase::GetSlotInternal(FName SlotName, UClass* TargetClass, UWidget* CurrWidget)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_GetSlotInternal_FName);
	if (!CurrWidget) return nullptr;

	// ���� ���� Widget�ڽŰ� ��
	if (CurrWidget->GetFName() == SlotName && CurrWidget->IsA(TargetClass))
		return CurrWidget;

	// CurrWidget�� ���� Element��� ��
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

	// CurrWidget�� �ڽ����� ��� ȣ��
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

	// CurrWidget�� �ڽ����� ��� ȣ��
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

#if 0 // �� ��� ������ ���� �� ���µ�.. Ȥ�� �ƴ� ��� ������ ���� �õ���..
UWidgetAnimation* UB2UnitedWidgetBase::GetAnimation(FString AnimName, UWidget* BaseWidget)
{
	// ���� �� ����� ����� �������� ������ ������� �� ��.
	// �Ʒ� WidgetAnim �� Outer �� WidgetBlueprint ���� UserWidget �� �ƴϴ�.
	// �Դٰ� WidgetBlueprint �� Editor �� ������Ʈ�� �� ��.
	// ���� �̷��� ObjectIterator �� ���� �͵� ���� ���ƺ����� ����.

	// check �� �ɰ� ������ ���� �������� ���� ���� ���� ������ ũ������ ���� �ʴ´�.
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
			checkSlow(RedDotCondition.ConditionCheckHandler.IsBound()); // ���� ������ �ٷ� RedDotCondition.ConditionCheckHandler.Execute ���� �ߴ� �� �� �� �⺻ ������ Bound �Ǿ�� �ϴ� ���ε�..
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
		// Slate ���� ó���ϴ� �Ϳ� ���� �� ƽ�� �̰� ������ ���� �� ��������. �� �� ���� ���� �Ϳ� ���ؼ��� ����..
		return; // �ٵ� ������ �׷� ��Ȳ�̶�� �� �ڵ尡 ���������� �� �ƴѰ�.. ���� ���� �� �ִٴ� �� ��ü��..
	}

	for (UB2Button* ThisBtn : AllB2Buttons)
	{
		if (ThisBtn 
			// �̹� Stun ������ ���� �ߺ� ȣ���ϸ� �ð� ī��Ʈ�� ���µǾ �� �� �ð� stun �� �Ǵµ�.. �ߺ� ȣ���ϴ� �� �³� �ƴ� ��ŵ�ϴ� �� ������..
			&& !ThisBtn->IsStunned()
			// �̰� ȣ���� ����ڰ� ���� tick �� stun �ɸ��� ��ư�̶�� �ڽ��� stun ���� �ʰ�, ���� �ð� stun �̶�� �ڽŵ� stun.
			// ���� tick stun �� ��� ���� ������ ���� ������ �ʿ��� �� �ִ� ��ư���� �����ϰ�, ���� �ð� stun �� �� ��ü�μ� �ڽŵ� stun �ؾ� �´� ��.
			&& (!InJustPressedButton || !InJustPressedButton->ShouldStunSingleTickOnly() || (ThisBtn != InJustPressedButton))
			) 
		{ 
			// SetIsEnabled ��� ���� ������ �� ���. SetIsEnabled �� ��Ÿ �ٸ� �������� ���� �� �ֱ� ����.
			ThisBtn->StunForDuration();
		}
	}
	bButtonsStunnedThisTick = true; // �̰� ���� �� ƽ ���� �� ���µ� ��.
	bHasStunnedButtons = true; // �̰� ��� ������ ������ true
}

void UB2UnitedWidgetBase::RestoreButtonsFromStun()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_RestoreButtonsFromSingleTickDisable);

	bButtonsStunnedThisTick = false; // �̰� �ٷ� ����
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
		bHasStunnedButtons = false; // ���� �ٽ� SetButtonsStunForDuration ȣ��ñ��� �� ��ƾ�� ���� �ȴ�.
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
			VisibilityBeforeForceHidden = RootWidget->GetVisibility();	//��Ʈ������ Visibility�� �������� �ʰ�, ���������̳��� Visibility�� �����ϸ� Open/Close ���� ����ó���� ��Ŵ
			RootWidget->SetVisibility(ESlateVisibility::Hidden);
		}

		// ImpulseRing �� ���� �ֵ��� TickableButtons �� ���ԵǹǷ� Hide �� Stop �� ��. �Ƹ� hide ���Ŀ� Release ��ȣ ���޴� ��.
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
		// ������ ForceHide ���Ŀ� SetVisibility �� �ߴٸ� ���õ�.
		UWidget* RootWidget = GetRootWidget();
		if (RootWidget && 
			!ShouldBeHiddenAsPooledObject() // PooledObject �� visibility ó���� ���� �ʿ信 ���� ���� �����̲����ε�..
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
	// LongestSide �� Horizontal �̸� Y ���� �ػ� ��ȭ�� ���� stretch �� ���� ��.
	const bool bIgnoreY = (UISettings && (UISettings->UIScaleRule == EUIScalingRule::LongestSide || UISettings->UIScaleRule == EUIScalingRule::Horizontal));

	FVector2D RetScale(1.0f, 1.0f);
	if (InPlayerController && UISettings)
	{
		int32 ViewSizeX, ViewSizeY;
		InPlayerController->GetViewportSize(ViewSizeX, ViewSizeY);
		// UIScaleRule �� ���� ������ stretch ���� �ʴ� �������δ� scale �� ����
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
	//		��� �� �ʵ� !
	//
	// ���������� slate widget �� ��ũ�� ���� ��ǥ�� �˾Ƴ��� ����� �������� �ʴ´�.
	// ���⼭�� ��� ������ ������� ��ǥ�� ����� ���µ�, 
	// ��ũ�� ��ü ũ���� CanvasPanel �� �������� strech �Ǵ� ��Ŀ�� ������� �ʵ��� ��ġ�� ��� ����.
	// � ������ �ڵ� ������ �˾Ƴ��� return false �� �� �����Ƿ� ������ ��߳� widget �� ���ڷ� �־��־��� �� �߸��� ����� ������ �� ����.
	// ���� ó���� �� �Ǵٰ��� UMG �󿡼� ��ġ ������ �ٲٸ� �ǵ��� ���� �������� ���� �� �����Ƿ� �����ؾ� �Ѵ�.
	// �߰��� Ư�� ũ��� (DesignedRefViewSize) UMG �۾��� �Ǿ��ٴ� ������ �ִµ� �̰� �������� �����̶� ���� �ٲ��� ���� ��.
	//================================================================================

	UCanvasPanelSlot* CPSlot = Cast<UCanvasPanelSlot>(InWidget->Slot);
	if (!CPSlot){
		return false;
	}

	const FAnchors& SlotAnchor = CPSlot->LayoutData.Anchors;
	if (SlotAnchor.IsStretchedVertical() || SlotAnchor.IsStretchedHorizontal()){
		return false;
	}

	// Anchor �� Min ���� Max ���� �ٸ� �� �ɷ����Ƿ� ���� �� �� �ƹ��ų� �ϳ� ���.
	// ���⿡ �⺻ ��Ŀ ���� 0.0 ~ 1.0 �����ϵ� ���� ����ϴµ�, ����� ���� ���۷��� �� ������� �������Ѵ�.
	const FVector2D RefScaledAnchor = SlotAnchor.Minimum * DesignedRefViewSize;

	// CanvasPanelSlot �� ��� �ִ� ��ǥ ���� ��Ŀ�� ���� ����. Stretch �� ��Ŀ�� ��쵵 ���� �ʿ��� ��Ȳ�� �ȴٸ� �߰� ��� �ʿ�.
	FVector2D NonScaledCoord = CPSlot->GetPosition();
	NonScaledCoord.X += RefScaledAnchor.X;
	NonScaledCoord.Y += RefScaledAnchor.Y;

	if (bWidgetCenterCoord)
	{
		NonScaledCoord += (CPSlot->GetSize() * 0.5f); // �⺻�����δ� �»�� ���� ��ǥ��.
	}

	OutResultCoord = NonScaledCoord * GetDesignToCurrentViewScale(InPC, bIgnoreNonScaledSide); // ���������� ���� ����Ʈ ������� ������.

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
		FVector ScreenCenterWorldDir(0.0f, 1.0f, 0.0f); // �̹� Deproject ���� �̰͸� �ʿ�
		const bool bScreenCenterProjRes = bDeprojectFromScreenCenter ?
			InPC->DeprojectScreenPositionToWorld((float)ViewSizeX * 0.5f, (float)ViewSizeY * 0.5f, ScreenCenterWorldPos, ScreenCenterWorldDir) :
			false;

		FVector ResWorldPos; // ���⼱ Pos �� �ʿ�.
		FVector ResWorldDir; // bDeprojectFromScreenCenter �� false �� �� ������ �����. �ٵ� �̰� �� �߸����� �� ������..
		if (InPC->DeprojectScreenPositionToWorld(InScreenCoord.X, InScreenCoord.Y, ResWorldPos, ResWorldDir))
		{ // DepthFromScreen ��ŭ ȭ�� ��������. ���� �̰� �������� depth �� �ʹ� Ŀ�� �ȵ�.
			return ResWorldPos + ((bScreenCenterProjRes ? ScreenCenterWorldDir : ResWorldDir) * DepthFromScreen);
		}
	}
	return FVector(0.0f, 0.0f, 0.0f);
}

UB2UnitedWidgetBase* UB2UnitedWidgetBase::DynCreateInCanvasPanel(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, class UCanvasPanel* ParentPanel,
	FVector2D NonScaledSize, FVector2D FinalSlotSize, FVector2D RelPosInPanel, int ZOrderInPanel /*= 0*/, bool bPivotAtCenter /*= false*/)
{
	//[FIXME][@AKI, 171009] �̰� ����� �ε��ϴ� �ð��� ���� �ҿ� �ϰ� ����
	// -> [FIXING] B2CLT-1893 .. WidgetPool �� ����ؼ� ���� ���� ��ü�� �� �ϵ���..

	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_DynCreateInCanvasPanel);
	if (InClass == NULL || OwnerUserWidget == NULL || ParentPanel == NULL){
		return NULL;
	}

	UB2UnitedWidgetBase* CreatedWidget = CreateWidget<UB2UnitedWidgetBase>(OwnerUserWidget->GetOwningPlayer(), InClass);
	if (CreatedWidget == NULL){
		return NULL;
	}
	
	// ���⼭ CreateWidget �� �ϰ� �ۿ��� �Ѵ��� CanvasPanel ���� �߰��ϴ� ��ƾ�� �ʿ��ؼ� ���� �� ����.
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
	// ���������δ� DynCreateInCanvasPanel �� �Ϻ��ε� WidgetPool �����ϸ鼭
	// CreateWidget �ϴ� �Ŷ� ���⼭ CanvasPanel �� �ִ� �Ŷ� ���� �� ���� �־ ������.
	//

	if (TargetWidget == NULL || OwnerUserWidget == NULL || ParentPanel == NULL) {
		return false;
	}

	// ParentPanel ũ�⿡ ���� ������ ������ ���� ScaleBox �ȿ� �����Ѵ�.
	UScaleBox* OuterSB = NewObject<UScaleBox>(OwnerUserWidget, NAME_None, RF_Transient);
	if (!OuterSB) {
		return false;
	}

	UScaleBoxSlot* AddedSBSlot = Cast<UScaleBoxSlot>(OuterSB->AddChild(TargetWidget));
	if (AddedSBSlot)
	{ // ũ��� ScaleBox ������ �ϰ� ScaleBox �ȿ��� �� ä��� �ɷ�.

		if (NonScaledSize.X > NonScaledSize.Y) // ScaleBox ����ϸ� NonScaledSize �� �ʿ������ �� �˾Ҵ��� Alignment ������ ���߷��� �� �ʿ��ϱ� ��
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

		TargetWidget->SetDynCreatedDummyParent(OuterSB); // CreatedWidget �� ������ �� OuterSB �� ���� ���ŵǵ��� ���.
	}

	UCanvasPanelSlot* AddedCPSlot = Cast<UCanvasPanelSlot>(ParentPanel->AddChild(OuterSB));
	if (AddedCPSlot)
	{
		// CanvasPanelSlot �� �־�� ��ġ, ������ ���� ���� ��.

		OuterSB->SetRenderTransformPivot(bPivotAtCenter ? FVector2D(0.5f, 0.5f) : FVector2D(0.0f, 0.0f));
		AddedCPSlot->SetSize(FinalSlotSize);
		AddedCPSlot->SetPosition(RelPosInPanel);
		AddedCPSlot->SetZOrder(ZOrderInPanel);
	}

	return true;
}

UB2UnitedWidgetBase* UB2UnitedWidgetBase::DynCreateInCanvasPanelFull(TSubclassOf<UB2UnitedWidgetBase> InClass, UB2UnitedWidgetBase* OwnerUserWidget, UCanvasPanel* ParentPanel)
{
	// ��ü ȭ�� ũ��� UIP ���� �� ���. WidgetBP �� �׳� ��ġ�ؼ� �⺻ �ε� �ð��� �Դ� ��� on-demand �� dynamic �ϰ� �����ؼ� ����ϴ� ���� ���.. 
	// �̷��� ���� ��� ���� ID (UIEnum) �� ����ؼ� ����ϰ� ���� ��쵵 ���� �ǵ� �װ� ��Ȳ�� ����..
	if (InClass && OwnerUserWidget && ParentPanel)
	{
		UB2UnitedWidgetBase* NewCreated = CreateWidget<UB2UnitedWidgetBase>(OwnerUserWidget->GetOwningPlayer(), InClass);
		if (NewCreated)
		{
			UCanvasPanelSlot* AddedSlot = Cast<UCanvasPanelSlot>(ParentPanel->AddChild(NewCreated));
			if (AddedSlot)
			{
				// �����¿� �� ä��.
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
		AddedSlot->SetPadding(InPlacementMargin); // Vertical box �� �� ���������� ������� ä��� �Ŷ�.. ��� �ɼ��� �� �ֱ� �ѵ� ��ǻ� padding ����� ���� ������?
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
		// �� ��� ������ ����� ���߱� ���� SizeBox �� ����.
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
		// SizeBox �� ������ ��� �Ѵܰ� ��..
		USizeBoxSlot* AddedSizeBoxSlot = Cast<USizeBoxSlot>(OptionalOuterSizeBox->AddChild(CreatedWidget));
		AddedHBSlot = Cast<UHorizontalBoxSlot>(ParentHB->AddChild(OptionalOuterSizeBox));

		CreatedWidget->SetDynCreatedDummyParent(OptionalOuterSizeBox); // CreatedWidget �� ������ �� OptionalOuterSizeBox �� ���� ���ŵǵ��� ���.
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
		// �� ��� ������ ����� ���߱� ���� SizeBox �� ����.
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
		// SizeBox �� ������ ��� �Ѵܰ� ��..
		USizeBoxSlot* AddedSizeBoxSlot = Cast<USizeBoxSlot>(OptionalOuterSizeBox->AddChild(CreatedWidget));
		AddedWrapBoxSlot = Cast<UWrapBoxSlot>(ParentWB->AddChild(OptionalOuterSizeBox));

		CreatedWidget->SetDynCreatedDummyParent(OptionalOuterSizeBox); // CreatedWidget �� ������ �� OptionalOuterSizeBox �� ���� ���ŵǵ��� ���.
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
	DynCreatedDummyParent = InWidget; // ��ü�� ScaleBox �� SizeBox ��..
}
void UB2UnitedWidgetBase::ConditionalClearDynCreatedDummyParent()
{
	//if (DynCreatedDummyParent && GetParent() == DynCreatedDummyParent)
	//{ // DynCreatedDummyParent �� �ִٸ� �̰͵� ���� �����־�� �ϴµ�.. �̰� ���� child �� �������� �ִٸ� ���.. �׷� ������ �����ؼ� ��������� ����.
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
{ // Receiver �ʿ��� �ҷ���. Sender �� ���� Receiver �ȿ� �ִ� ���� ��Ʈ�� ���̹Ƿ�.
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UnitedWidgetBase_SetupManualScrollBoxSender_byReceiver);
	//if (InReceiver && HandlingScrollBox)
	//{
	//	// Sender �� Visibility ������ �ʿ信 ���� ������..
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
	//	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) // ���콺 ��ư �ڵ鸵�� ���߹��� ��
	//	{
	//		OnPressedSenderBTNArea(); // Area ��ư�� �ִٸ� ���⼭ �Է� �ڵ鸵��

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
	//	OnReleasedSenderBTNArea(); // Area ��ư�� �ִٸ� ���⼭ �Է� �ڵ鸵��
	//	if (bSenderBtnAreaPressed && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && (FApp::GetCurrentTime() - TimeSenderBtnAreaPressStarted) <= GetTimeToDecideInputClickOrTab())
	//	{
	//		OnClickedSenderBTNArea(); // Area ��ư�� �ִٸ� ���⼭ �Է� �ڵ鸵��
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
	//	OnReleasedSenderBTNArea(); // Area ��ư�� �ִٸ� ���⼭ �Է� �ڵ鸵��

	//	// ���콺 ȣ�� ���� ������ �ȵǾ� �ӽ÷� ���Ƶ�..
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
	//	OnPressedSenderBTNArea(); // Area ��ư�� �ִٸ� ���⼭ �Է� �ڵ鸵��
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
	//	OnReleasedSenderBTNArea(); // Area ��ư�� �ִٸ� ���⼭ �Է� �ڵ鸵��
	//	if (bSenderBtnAreaPressed && (FApp::GetCurrentTime() - TimeSenderBtnAreaPressStarted) <= GetTimeToDecideInputClickOrTab())
	//	{
	//		OnClickedSenderBTNArea(); // Area ��ư�� �ִٸ� ���⼭ �Է� �ڵ鸵��
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
//	// �̰� ���� ���� ���ص� �ɵ�.. ������ ������ �������� ��ũ�ѿ����� ������ �ƴ�
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
			// ���ö ��. ������ �ѹ��� ���� �ܰ踦 �پ�� �ϴ� ��쿡 ��ӵ��� �ϸ� �پ��� ���� ���̸� Ŀ���� �� �ִ� ������ �ӵ��� ã�Ⱑ ����� ���� �־ ���ö ���� ���� ����.
			CurrentSpeed += DistToTarget * SpringConst * TickDelta;
			CurrentSpeed -= CurrentSpeed * DampConst * TickDelta;
			// �� ��쿡�� �ּ����� �ӵ��� ������ ��.
			CurrentSpeed = FMath::Max(CurrentSpeed, BaseSpeed);
		}
		else
		{
			CurrentSpeed = BaseSpeed;
		}

		CurrentAnimatedPercent += CurrentSpeed * TickDelta;
		if (CurrentAnimatedPercent >= 1.0f) // ���� �ö󰡴� ��Ȳ.
		{
			float AnimatedPctOver = CurrentAnimatedPercent - 1.0f;

			int32 AddedLevel = (int32)AnimatedPctOver + 1;
			CurrentAnimatedLevel += AddedLevel;
			CurrentAnimatedPercent = FMath::Frac(AnimatedPctOver);

			OnLevelChanged.Broadcast(CurrentAnimatedLevel);
		}
		OnPrecentChanged.Broadcast(CurrentAnimatedPercent);
	}

	// �ٽñ� ��ǥ���� �����ߴ��� üũ�ϰ� Ŭ����
	if (GetUnitDistanceToTargetValue() <= 0.0f)
	{ // ��ǥ���� ������ ����
		SyncToTargetValues();
		ClearCustomTimerTick();

		OnFinishProgAnimation.Broadcast();
	}
}

void FProgressAnimateUtil::ManualTick(float InDeltaSecond)
{
	// ApplyCustomTimerTick �� �ʿ��� ���� Tick �� �Ҹ��� �ϰ��� �ϸ� ����ϰ�.. �� �׷� �ʿ� ������ �̷���
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
	{// ��ǥ���� ������ ����
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
				//����¦�� �ȸ¾�..
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


	CurrentTextString.Empty(); // �̰� ���� ÷���� ����

	TotalShowingTime = InTotalTime;
	MaxCharShowInterval = InMaxCharShowInterval;
	
	// ���� ��꿡�� ���� ShowInterval
	CharShowInterval = (TotalShowingTime > 0.0f) ?
		FMath::Min(TotalShowingTime / (float)FMath::Max(WholeTextString.Len(), 1), MaxCharShowInterval) : MaxCharShowInterval;

	// ó�� ���ýÿ��� 0��° ���ں��� �����ִ� �ɷ� ����.
	ApplyStringProgress(0);
}

void FProgressiveTextBuilder::SetTotalShowingTime(float InNewTime)
{
	TotalShowingTime = InNewTime;
	// �߰� ó�� �����൵ ��?
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
		ApplyStringProgress(CurrentCharShowIndex + ThisTimeShowCharacterNum); // �ȿ��� LastCharUpdatedTime ����.
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
	{ // ���� �����ϴ���, Append �� ���ڰ� �� ������, �ƴ� ���� ����� �ٸ� ���. �� �ٽ� ����.
		CurrentTextString = WholeTextString.LeftChop(WholeTextString.Len() - (InNewCharacterIndex + 1));
	}
	else
	{ // ���� ���� Append. �̰� �Ź� Chop �ϴ� �ͺ��� ���� �Ŷ�� ����..
		for (int32 ThisIndex = CurrentCharShowIndex + 1; ThisIndex <= InNewCharacterIndex; ++ThisIndex)
		{
			CurrentTextString += WholeTextString[ThisIndex];
		}
	}

	// ���� �� Apply �� ���� ����� �д�.
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
	//	FString::Printf(TEXT("�ش� ��� ��� ���߿� ��ǻ�Ͱ� ������ �� ����. ��ó �������� ���� �۾����� �ִٸ� ���� ����� �� �ٽ� �õ��Ͻÿ�."))
	//	)) == EAppReturnType::Cancel)
	//{
	//	return;
	//}

	//int32 AllReplacedNum = 0;
	//FString ResultMessage;

	//TArray<FString> AllChangedWidgetNames;

	//// FWidgetBlueprintEditorUtils::ReplaceWidgets ����

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
	//				check(0); // ��Ʈ�� ����ġ����� ���?
	//			}

	//			while (CastedExistingPW->GetChildrenCount() > 0) // �ڽĵ鵵 �� ��������
	//			{
	//				UWidget* Widget = CastedExistingPW->GetChildAt(0);
	//				Widget->Modify();

	//				UPanelSlot* OldSlot = Widget->Slot;
	//				UPanelSlot* NewAddedSlot = NewReplacement->AddChild(Widget);

	//				// �ٲٷ��� ��ư �Ʒ��� �������̸� ��� ��찡 �־ �Ӽ��� �� ����. ��Ÿ �ٸ� ��쵵 ��������.
	//				UButtonSlot* NewAddedButtonSlot = Cast<UButtonSlot>(NewAddedSlot);
	//				UButtonSlot* OldButtonSlot = Cast<UButtonSlot>(OldSlot);
	//				if (NewAddedButtonSlot && OldButtonSlot)
	//				{
	//					NewAddedButtonSlot->SetHorizontalAlignment(OldButtonSlot->HorizontalAlignment);
	//					NewAddedButtonSlot->SetVerticalAlignment(OldButtonSlot->VerticalAlignment);
	//					NewAddedButtonSlot->SetPadding(OldButtonSlot->Padding);
	//				}
	//			}

	//			// FWidgetBlueprintEditorUtils::ReplaceWidgets �ʿ��� �� ���� �����. ��� ���� ����

	//			FString ExistingPanelName = CastedExistingPW->GetName();
	//			UObject* ExistingPanelOuter = CastedExistingPW->GetOuter();

	//			// Rename the removed widget to the transient package so that it doesn't conflict with future widgets sharing the same name.
	//			CastedExistingPW->Rename(nullptr, nullptr);
	//			NewReplacement->Rename(*ExistingPanelName, ExistingPanelOuter);

	//			AllChangedWidgetNames.Add(ExistingPanelName);

	//			// �Ӽ��� �Ǵ´�� �����ֱ�. �⺻ ũ��, ��Ŀ ���� Slot �����̶� �� �ʿ� ����.
	//			NewReplacement->SetVisibility(CastedExistingPW->GetVisibility());
	//			NewReplacement->SetIsEnabled(CastedExistingPW->GetIsEnabled());
	//			NewReplacement->SetRenderTransform(CastedExistingPW->RenderTransform);
	//			NewReplacement->SetRenderTransformPivot(CastedExistingPW->RenderTransformPivot);

	//			// B2Button ��ü special. For faster UBtton to UB2Button replacement
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

	//	ResultMessage += FString::Printf(TEXT("%s �� ��ġ�� %d ���� %s ���� %s (��)�� ����\n\n"), *BasePage->GetName(), AllReplacedNum, *ClassTobeRemoved->GetName(), *ClassToReplace->GetName());

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

	//	ResultMessage += TEXT("��� �������� ����Ǵ� ���� �ƴϹǷ� �ٲ� widget ���� ���� �������� ���� Ȯ���� ���ñ� �ٶ��ϴ�.");
	//}
	//else
	//{
	//	ResultMessage = FString::Printf(TEXT("%s ���� �ٲ� widget �� �����ϴ�."), *BasePage->GetName());
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

		//ResultMessage += FString::Printf(TEXT("%s �� ��ġ�� %d ���� B2Button ���� DefaultOnClickSoundIndex ���� %d (��)�� ����\n\n"), *BasePage->GetName(), AllSetNum, InSoundIndex);

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
		ResultMessage = FString::Printf(TEXT("%s ���� �ٲ� widget �� �����ϴ�."), *BasePage->GetName());
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

//SWidget�� ��ġ ������ FGeomety�� ���������.
//Widget�� Left-Top�� Screen ��ġ�� ������ ����,
//�Ʒ��� �Լ��� UWidget�� FGeometry�� ã�� ���� ����Լ���
//������ ������ �ִٸ� ������ �����ϱ� �ٶ�.
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
		//Paint�� �Ҹ����� ���ٸ� ����� ������ ����.
		//��쿡 ���� �Ʒ��� �Լ��� ȣ���ص� ����� ������ �ʴ� ��찡 �ִµ�, RenderThread�� �������� �ʴ� �����ε�? 
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
	// ���� hidden ������ ���̽����ε� �ؾ� �� ��.. ���⼭ �Ϲ����� ��ƾ���� ó���ϴ� �� �Ѱ谡 ���� �� �ϴ�.
	return (bIsPooledObject && !bCurrentlyRentAsPooledObject); 
}

/** PooledObject �� �����ϰ�..
 * ������ pool managing ������Ʈ(FWidgetPoolBase extension)�� ���ؾ� ��. �׳� �̰� ���Ѵٰ� pooling �Ǵ� �� �ƴ�. */
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
			// ���� Rent �� ���¿����� �� ���� widget class ���� ��Ȳ�� ���� �˸°� Visibility ��� �ϵ��� �ϰ� �ݳ��� ��츸 ���ܵ�.
			// Visibility ����� ���� widget class ���� �� �Ű��� ���־�� �� ��. ���⿡�� �ּ����� �������̽��� ����.
			SetVisibility(ESlateVisibility::Hidden);
			OnPooledObjectTurnInBP();
		}
	}
}

#if UE_BUILD_DEBUG
void DebugCheckOnLoadSynchronousUse(UObject* WorldContextObject)
{
	// �κ� �ƴ� �ΰ��� ��Ȳ�� ����ϸ� ����� �� �ִ� ��ɿ� ���� assert ����.
	ABladeIIGameMode* CurrGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	checkSlow(CurrGM && CurrGM->GetB2GameModeType() == EB2GameMode::Lobby);
}
#endif