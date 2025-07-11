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
			WholeAnimTime = InWholeAnimTime; // 파라미터 전달.
			MIDParamRange_Begin = InBeginMIDParam;
			MIDParamRange_End = InEndMIDParam;

			ImpulseRingImage->SetVisibility(ESlateVisibility::Hidden); // 일단 숨겨야겠지
			bIsCurrentOn = false;
			ImpulseRingMID = NULL;
			return true;
		}
	}

	return false;
}
void FImpluseRingInstInfo::Clear()
{
	Stop(); // 더 필요한가?
}

void FImpluseRingInstInfo::Play()
{
	if (ImpulseRingImage.IsValid())
	{
		ImpulseRingImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		ImpulseRingMID = ImpulseRingImage->GetDynamicMaterial();
		if (ImpulseRingMID)
		{ // Material Anim progress 리셋
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
		// 0 ~ WholeAnimTime 사이로 파라미터 애니메이션 시킴
		CurrentAnimTime += DeltaSecond;
		if (CurrentAnimTime > WholeAnimTime)
		{
			CurrentAnimTime = 0.0f;
			bRetVal = true; // 다시 처음부터 플레이 하던지, stop 하던지, 밖에서 알아서 하도록
			if (bAutoRepeatOnEnd) // 그러나 bAutoRepeatOnEnd 인 경우는 밖에서 더 신경쓸 필요 없겠다.
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
		// CurrentAnimTime 이 WholeAnimTime 까지만 증가하는데 최종적인 파라미터 적용은 MIDParamRange_Begin ~ MIDParamRange_End 사이로.
		const float ParamRange = MIDParamRange_End - MIDParamRange_Begin;
		const float AnimScale = (WholeAnimTime > KINDA_SMALL_NUMBER) ? (ParamRange / WholeAnimTime) : 1.0f;
		const float FinalParameter = MIDParamRange_Begin + (AnimScale * CurrentAnimTime);

		ImpulseRingMID->SetScalarParameterValue(FName(ON_TOUCH_IMPULSE_RING_MAT_ANIM_PARAM_NAME), FinalParameter);
	}
}

void FImpluseRingInstInfo::SetImageCenterPosition(const FVector2D& InPosUpperLeftOrigin)
{ // InPosUpperLeftOrigin 은 화면 좌상단 기준의 UMG 레퍼런스 해상도 기준 좌표로 친다. UB2UnitedWidgetBase::HACKGetWidgetScreenCoord 참고
	//if (ImpulseRingImage.IsValid())
	//{
	//	UCanvasPanelSlot* CPSlot = Cast<UCanvasPanelSlot>(ImpulseRingImage->Slot); // 전체 영역 크기의 CanvasPanel 에 배치한 경우만 먹힐 것임.
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
	//			// 들어온 좌표를 이미지 중심 좌표로 
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
	// Click/TouchMethod 설정이 MouseDown/PreciseTap 일 때 버튼 Click 시그널은 Press 시점에 발생.
	// Pressed 와 Click 바인딩이 사실상 마찬가지가 된다.
	// 이는 버튼 터치 반응이 시원시원하고 빨라야 한다는 요구사항에 의한 것인데 (추가로 비슷한 무렵에 버튼 터치 영역 확장도 구현됨)
	// 아마 당시까지의 구현에서 별 생각없이 Click 에 바인딩한 어지간한 버튼들 모두 터치 즉시 반응을 원했던 것 같다.
	// 버튼 용도에 따라 이것이 부작용을 일으킬 수 있는데 만일 이 기본값을 엔진 기본인 DownAndUp 으로 되돌릴 시 
	// 기존에 Click 에 바인딩한 버튼들 중 즉각적인 반응이 필요한 버튼들은 직접 설정을 변경하거나 Pressed 에 바인딩하거나..
	// 이에 따라 게임 메뉴 조작의 전반적인 터치감이 달라질 수 있다.
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
	bIsEnabledByNormalUsage = bInIsEnabled; // 다른 기능에 의해 Enable/Disable 할 때에 원래 상태값을 확인하는 데에 쓸 것.
	Super::SetIsEnabled(bInIsEnabled);
}

//////////////////////////////////////////////////
// 특정한 사용 외에 일반적인 용도로 사용하지 말 것.
void UB2Button::StunForDuration()
{ // bIsEnabledByNormalUsage 은 유지한 채 상단 SetIsEnabled 를 호출.
	bIsStunned = true;
	LastStunnedTime = static_cast<float>(FPlatformTime::Seconds());
	Super::SetIsEnabled(false);
}
bool UB2Button::TryRestoreFromStun()
{
	if (bIsStunned)
	{
		// 시간 체크..  StunDuration 이 0 이하면 바로 복구
		const float CurrTime = static_cast<float>(FPlatformTime::Seconds());
		if (ShouldStunSingleTickOnly() || (CurrTime - LastStunnedTime >= StunDuration))
		{
			Super::SetIsEnabled(bIsEnabledByNormalUsage);
			bIsStunned = false; // 설령 여전히 disable 이더라도 stun 은 풀린 것.
			return true; // 실제 stun 이었다가 풀린 경우만
		}		
	}
	return false;
}
//////////////////////////////////////////////////

bool UB2Button::IsTickRequired() const
{ // Owning user widget 생성 시 불리는 것임. 사전에 설정 가능한 값으로만 판정해야 함.
	return (OnTouchImpulseRingWidgetName != NAME_None); // Name 설정이 되어 있으면 OnTouchImpulseRingImage 를 얻어올 가능성이 있으므로 tick 을 돈다.
}

void UB2Button::ManualTick(float DeltaSecond)
{
	if (OnTouchImpulseRingInst.IsCurrentOn())
	{
		if (OnTouchImpulseRingInst.Tick(DeltaSecond))
		{
			OnOnTouchImpulseRingAnimTimeCycle(); // 한 사이클 다 돌면 다시 처음부터 플레이 하던지, stop 되던지.
		}
	}
}

void UB2Button::RegisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget)
{
	//CachedOwnerUnitedWidget = InOwnerUnitedWidget;
	//bIsEnabledByNormalUsage = bIsEnabled; // 이 시점에서는 이게 같아야 한다. 이게 달라지는 경우는 CachedOwnerUnitedWidget 을 통해 동작하므로

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
	//	// 아무나 눌렸으면 바로 다른 버튼들 stun 을.. 설정에 따라 한 틱 혹은 일정 시간 유지. 자신의 StunDuration 이 0 보다 크면 자신도 stun
	//	// 근데 이것 자체가 한 틱에 동시에 들어갈 수도..?
	//	CachedOwnerUnitedWidget->SetButtonsStunForDuration(this);
	//}

	//bIsPressedForImpulseRing = true;
	//
	//if (!OnTouchImpulseRingInst.IsCurrentOn()){
	//	PlayOnTouchImpulseRing(); // 빠른 터치 시 매번 새로 플레이 되지 않도록 켜져 있는 동안에는 타이머에 의해서만 다시 플레이.
	//}

	//if (bPlayDefaultOnClickSoundOnPress){
	//	PlayOnClickDefaultSound();
	//}
}

void UB2Button::ExtraOnRelease()
{
	bIsPressedForImpulseRing = false;

	// OnTouchImpulseRing 을 여기서 딱히 stop 시키지는 않는다.
}

void UB2Button::ExtraOnHovered()
{
	//[@AKi, 170329] 특별히 사용은 안하나 혹시 몰라서 만들어 놓음.
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
		PlayOnTouchImpulseRing(); // 다시 처음부터 시작
	}
	else{
		StopOnTouchImpulseRing(); // 아니면 이제 그만
	}
}

void UB2Button::CustomStopOnTouchImpulseRing()
{
	bIsPressedForImpulseRing = false; // 플레이 되고 있는 상황이라면 OnOnTouchImpulseRingAnimTimeCycle 에서 픽업할 것.
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