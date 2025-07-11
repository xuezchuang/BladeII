// Fill out your copyright notice in the Description page of Project Settings.


#include "B2Button.h"
//#include "B2UnitedWidgetBase.h"
#include "BladeIIGameMode.h"
#include "Event.h"
#include "B2UnitedWidgetBase.h"
#include "Materials/MaterialInstanceDynamic.h"
//#include "B2CommonSoundInfo.h"
//#include "TutorialManager.h"
//#include "B2GameInstance.h"

#define LOCTEXT_NAMESPACE "UMG"
#define ON_TOUCH_IMPULSE_RING_MAT_ANIM_PARAM_NAME TEXT("RingScale")

//================================================================================

bool FImpluseRingInstInfo::Init(class UB2UnitedWidgetBase* InOwnerUnitedWidget, const FName& InImageWidgetName, float InWholeAnimTime, float InBeginMIDParam, float InEndMIDParam)
{
	if (InOwnerUnitedWidget && InImageWidgetName != NAME_None)
	{
		return Init(InOwnerUnitedWidget, InOwnerUnitedWidget->GetSlot<UImage>(InImageWidgetName), InWholeAnimTime, InBeginMIDParam, InEndMIDParam);
	}
	return false;
}
bool FImpluseRingInstInfo::Init(class UB2UnitedWidgetBase* InOwnerUnitedWidget, UImage* InImageWidgetPtr, float InWholeAnimTime, float InBeginMIDParam, float InEndMIDParam)
{
	if (InOwnerUnitedWidget && InImageWidgetPtr)
	{
		ImpulseRingImage = InImageWidgetPtr;

		if (ImpulseRingImage.IsValid())
		{
			WholeAnimTime = InWholeAnimTime; // �Ķ���� ����.
			MIDParamRange_Begin = InBeginMIDParam;
			MIDParamRange_End = InEndMIDParam;

			ImpulseRingImage->SetVisibility(ESlateVisibility::Hidden); // �ϴ� ���ܾ߰���
			bIsCurrentOn = false;
			ImpulseRingMID = NULL;
			return true;
		}
	}

	return false;
}
void FImpluseRingInstInfo::Clear()
{
	Stop(); // �� �ʿ��Ѱ�?
}

void FImpluseRingInstInfo::Play()
{
	if (ImpulseRingImage.IsValid())
	{
		ImpulseRingImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		ImpulseRingMID = ImpulseRingImage->GetDynamicMaterial();
		if (ImpulseRingMID)
		{ // Material Anim progress ����
			CurrentAnimTime = 0.0f;
			ApplyMIDParam();
			bIsCurrentOn = true;
		}
	}
}
void FImpluseRingInstInfo::Stop()
{
	if (ImpulseRingImage.IsValid())
	{
		ImpulseRingImage->SetVisibility(ESlateVisibility::Hidden);
	}
	ImpulseRingMID = NULL;
	bIsCurrentOn = false;
}

bool FImpluseRingInstInfo::Tick(float DeltaSecond, bool bAutoRepeatOnEnd)
{
	bool bRetVal = false;
	if (bIsCurrentOn)
	{
		// 0 ~ WholeAnimTime ���̷� �Ķ���� �ִϸ��̼� ��Ŵ
		CurrentAnimTime += DeltaSecond;
		if (CurrentAnimTime > WholeAnimTime)
		{
			CurrentAnimTime = 0.0f;
			bRetVal = true; // �ٽ� ó������ �÷��� �ϴ���, stop �ϴ���, �ۿ��� �˾Ƽ� �ϵ���
			if (bAutoRepeatOnEnd) // �׷��� bAutoRepeatOnEnd �� ���� �ۿ��� �� �Ű澵 �ʿ� ���ڴ�.
			{
				Play();
			}
		}
		ApplyMIDParam();
	}
	return bRetVal;
}

void FImpluseRingInstInfo::ApplyMIDParam()
{
	if (ImpulseRingMID)
	{
		// CurrentAnimTime �� WholeAnimTime ������ �����ϴµ� �������� �Ķ���� ������ MIDParamRange_Begin ~ MIDParamRange_End ���̷�.
		const float ParamRange = MIDParamRange_End - MIDParamRange_Begin;
		const float AnimScale = (WholeAnimTime > KINDA_SMALL_NUMBER) ? (ParamRange / WholeAnimTime) : 1.0f;
		const float FinalParameter = MIDParamRange_Begin + (AnimScale * CurrentAnimTime);

		ImpulseRingMID->SetScalarParameterValue(FName(ON_TOUCH_IMPULSE_RING_MAT_ANIM_PARAM_NAME), FinalParameter);
	}
}

void FImpluseRingInstInfo::SetImageCenterPosition(const FVector2D& InPosUpperLeftOrigin)
{ // InPosUpperLeftOrigin �� ȭ�� �»�� ������ UMG ���۷��� �ػ� ���� ��ǥ�� ģ��. UB2UnitedWidgetBase::HACKGetWidgetScreenCoord ����
	//if (ImpulseRingImage.IsValid())
	//{
	//	UCanvasPanelSlot* CPSlot = Cast<UCanvasPanelSlot>(ImpulseRingImage->Slot); // ��ü ���� ũ���� CanvasPanel �� ��ġ�� ��츸 ���� ����.
	//	if (CPSlot)
	//	{
	//		const FAnchors& SlotAnchor = CPSlot->LayoutData.Anchors;
	//		if (!SlotAnchor.IsStretchedVertical() && !SlotAnchor.IsStretchedHorizontal())
	//		{
	//			const FVector2D RefScaledAnchor = SlotAnchor.Minimum * UB2UnitedWidgetBase::DesignedRefViewSize;
	//			FVector2D FinalAdjusted = InPosUpperLeftOrigin;
	//			FinalAdjusted.X += RefScaledAnchor.X;
	//			FinalAdjusted.Y += RefScaledAnchor.Y;

	//			const FVector2D& ImageSize = CPSlot->GetSize();
	//			// ���� ��ǥ�� �̹��� �߽� ��ǥ�� 
	//			FinalAdjusted.X -= (ImageSize.X * 0.5f);
	//			FinalAdjusted.Y -= (ImageSize.Y * 0.5f);

	//			CPSlot->SetPosition(FinalAdjusted);
	//		}
	//	}
	//}
}

//================================================================================

UB2Button::UB2Button(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//bIsEnabledByNormalUsage = bIsEnabled;
	//bIsPressedForImpulseRing = false;

	//CachedOwnerUnitedWidget = NULL;

	//DefaultOnClickSoundIndex = 0;
	//bPlayDefaultOnClickSoundOnPress = false;

	//bReleaseOnFocusLost = true;

	///*
	// Click/TouchMethod ������ MouseDown/PreciseTap �� �� ��ư Click �ñ׳��� Press ������ �߻�.
	// Pressed �� Click ���ε��� ��ǻ� ���������� �ȴ�.
	// �̴� ��ư ��ġ ������ �ÿ��ÿ��ϰ� ����� �Ѵٴ� �䱸���׿� ���� ���ε� (�߰��� ����� ���ƿ� ��ư ��ġ ���� Ȯ�嵵 ������)
	// �Ƹ� ��ñ����� �������� �� �������� Click �� ���ε��� �������� ��ư�� ��� ��ġ ��� ������ ���ߴ� �� ����.
	// ��ư �뵵�� ���� �̰��� ���ۿ��� ����ų �� �ִµ� ���� �� �⺻���� ���� �⺻�� DownAndUp ���� �ǵ��� �� 
	// ������ Click �� ���ε��� ��ư�� �� �ﰢ���� ������ �ʿ��� ��ư���� ���� ������ �����ϰų� Pressed �� ���ε��ϰų�..
	// �̿� ���� ���� �޴� ������ �������� ��ġ���� �޶��� �� �ִ�.
	//*/
	//ClickMethod = EButtonClickMethod::MouseDown;
	//TouchMethod = EButtonTouchMethod::PreciseTap;

	//OnTouchImpulseRingAnimTime = 0.2f;
	//OnTouchImpulseRingParamRange_Begin = 0.5f;
	//OnTouchImpulseRingParamRange_End = 1.5f;
	//bLoopOnTouchImpulseRing = false;

	//StunDuration = 0.0f;
	//LastStunnedTime = 0.0f;
	//bIsStunned = false;
}

void UB2Button::SetIsEnabled(bool bInIsEnabled)
{
	bIsEnabledByNormalUsage = bInIsEnabled; // �ٸ� ��ɿ� ���� Enable/Disable �� ���� ���� ���°��� Ȯ���ϴ� ���� �� ��.
	Super::SetIsEnabled(bInIsEnabled);
}

//////////////////////////////////////////////////
// Ư���� ��� �ܿ� �Ϲ����� �뵵�� ������� �� ��.
void UB2Button::StunForDuration()
{ // bIsEnabledByNormalUsage �� ������ ä ��� SetIsEnabled �� ȣ��.
	bIsStunned = true;
	LastStunnedTime = static_cast<float>(FPlatformTime::Seconds());
	Super::SetIsEnabled(false);
}
bool UB2Button::TryRestoreFromStun()
{
	if (bIsStunned)
	{
		// �ð� üũ..  StunDuration �� 0 ���ϸ� �ٷ� ����
		const float CurrTime = static_cast<float>(FPlatformTime::Seconds());
		if (ShouldStunSingleTickOnly() || (CurrTime - LastStunnedTime >= StunDuration))
		{
			Super::SetIsEnabled(bIsEnabledByNormalUsage);
			bIsStunned = false; // ���� ������ disable �̴��� stun �� Ǯ�� ��.
			return true; // ���� stun �̾��ٰ� Ǯ�� ��츸
		}		
	}
	return false;
}
//////////////////////////////////////////////////

bool UB2Button::IsTickRequired() const
{ // Owning user widget ���� �� �Ҹ��� ����. ������ ���� ������ �����θ� �����ؾ� ��.
	return (OnTouchImpulseRingWidgetName != NAME_None); // Name ������ �Ǿ� ������ OnTouchImpulseRingImage �� ���� ���ɼ��� �����Ƿ� tick �� ����.
}

void UB2Button::ManualTick(float DeltaSecond)
{
	if (OnTouchImpulseRingInst.IsCurrentOn())
	{
		if (OnTouchImpulseRingInst.Tick(DeltaSecond))
		{
			OnOnTouchImpulseRingAnimTimeCycle(); // �� ����Ŭ �� ���� �ٽ� ó������ �÷��� �ϴ���, stop �Ǵ���.
		}
	}
}

void UB2Button::RegisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget)
{
	//CachedOwnerUnitedWidget = InOwnerUnitedWidget;
	//bIsEnabledByNormalUsage = bIsEnabled; // �� ���������� �̰� ���ƾ� �Ѵ�. �̰� �޶����� ���� CachedOwnerUnitedWidget �� ���� �����ϹǷ�

	//OnClicked.AddUniqueDynamic(this, &UB2Button::ExtraOnClick);
	//OnPressed.AddUniqueDynamic(this, &UB2Button::ExtraOnPress);
	//OnReleased.AddUniqueDynamic(this, &UB2Button::ExtraOnRelease);
	//OnHovered.AddUniqueDynamic(this, &UB2Button::ExtraOnHovered);

	//OnTouchImpulseRingInst.Init(InOwnerUnitedWidget, OnTouchImpulseRingWidgetName, OnTouchImpulseRingAnimTime, OnTouchImpulseRingParamRange_Begin, OnTouchImpulseRingParamRange_End);
}

void UB2Button::UnregisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget)
{
	OnClicked.RemoveDynamic(this, &UB2Button::ExtraOnClick);
	OnPressed.RemoveDynamic(this, &UB2Button::ExtraOnPress);
	OnReleased.RemoveDynamic(this, &UB2Button::ExtraOnRelease);
	OnHovered.RemoveDynamic(this, &UB2Button::ExtraOnHovered);
	OnTouchImpulseRingInst.Clear();
}

void UB2Button::ExtraOnClick()
{
	if (!bPlayDefaultOnClickSoundOnPress){
		PlayOnClickDefaultSound();
	}

	TutorialButtonSignal();
}

void UB2Button::ExtraOnPress()
{
	//if (CachedOwnerUnitedWidget) {
	//	// �ƹ��� �������� �ٷ� �ٸ� ��ư�� stun ��.. ������ ���� �� ƽ Ȥ�� ���� �ð� ����. �ڽ��� StunDuration �� 0 ���� ũ�� �ڽŵ� stun
	//	// �ٵ� �̰� ��ü�� �� ƽ�� ���ÿ� �� ����..?
	//	CachedOwnerUnitedWidget->SetButtonsStunForDuration(this);
	//}

	//bIsPressedForImpulseRing = true;
	//
	//if (!OnTouchImpulseRingInst.IsCurrentOn()){
	//	PlayOnTouchImpulseRing(); // ���� ��ġ �� �Ź� ���� �÷��� ���� �ʵ��� ���� �ִ� ���ȿ��� Ÿ�̸ӿ� ���ؼ��� �ٽ� �÷���.
	//}

	//if (bPlayDefaultOnClickSoundOnPress){
	//	PlayOnClickDefaultSound();
	//}
}

void UB2Button::ExtraOnRelease()
{
	bIsPressedForImpulseRing = false;

	// OnTouchImpulseRing �� ���⼭ ���� stop ��Ű���� �ʴ´�.
}

void UB2Button::ExtraOnHovered()
{
	//[@AKi, 170329] Ư���� ����� ���ϳ� Ȥ�� ���� ����� ����.
}

void UB2Button::PlayOnClickDefaultSound()
{
	//if (DefaultOnClickSoundIndex >= 0)
	//{
	//	UWorld* TheWorld = GetWorld();
	//	if (TheWorld)
	//	{
	//		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(TheWorld));
	//		UB2CommonSoundInfo* CSI = B2GI ? B2GI->GetCommonSoundInfo() : NULL;
	//		USoundCue* WantedSoundCue = CSI ? CSI->GetButtonClickDefSound(DefaultOnClickSoundIndex) : NULL;
	//		if (WantedSoundCue)
	//		{
	//			UGameplayStatics::PlaySound2D(TheWorld, WantedSoundCue);
	//		}
	//	}
	//}
}

void UB2Button::PlayOnTouchImpulseRing()
{
	OnTouchImpulseRingInst.Play();
}

void UB2Button::StopOnTouchImpulseRing()
{
	OnTouchImpulseRingInst.Stop();
}

void UB2Button::OnOnTouchImpulseRingAnimTimeCycle()
{
	if (bIsPressedForImpulseRing && bLoopOnTouchImpulseRing){
		PlayOnTouchImpulseRing(); // �ٽ� ó������ ����
	}
	else{
		StopOnTouchImpulseRing(); // �ƴϸ� ���� �׸�
	}
}

void UB2Button::CustomStopOnTouchImpulseRing()
{
	bIsPressedForImpulseRing = false; // �÷��� �ǰ� �ִ� ��Ȳ�̶�� OnOnTouchImpulseRingAnimTimeCycle ���� �Ⱦ��� ��.
}

void UB2Button::TutorialButtonSignal()
{
	/*if (TutorialManager::GetInstance().HasAnyTutorial())
	{
		NotifyUIButtonClickedClass<UButton*>::GetInstance().Signal(this);
	}*/
}

void UB2Button::OnChangedClickMethod()
{	
	//if (MyButton.IsValid())
	//{
	//	MyButton->ChangeClickMethod_Dynamic(ClickMethod);
	//	MyButton->ChangeTouchMethod_Dynamic(TouchMethod);
	//}
}

#if WITH_EDITOR
const FText UB2Button::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common"); // Original UBtton palette category
}
#endif

#undef LOCTEXT_NAMESPACE