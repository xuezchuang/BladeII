
#include "B2UIWidgetBase.h"
#include "MovieScene.h"
#include "Animation/WidgetAnimation.h"
#include "B2UIManager.h"

UB2UIWidgetBase::UB2UIWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UB2UIWidgetBase::~UB2UIWidgetBase()
{
}

void UB2UIWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	// CustomTimeCallback 타이머 설치. 
	SetupWidgetCustomTimer();
	// CustomTimedHide 타이머 설치
	SetupWidgetCustomHideTimer();

}

void UB2UIWidgetBase::Init()
{
	CacheAssets();
	UpdateStaticText();
	UpdateStaticText_BP();
	BindDelegates();

	RegisterUIMarkForRedDot();

	DoMarkRedDot();
}

void UB2UIWidgetBase::DestroySelf(UB2UIManager* InUIManager)
{
	//// 여기서 BeginDestroyed 였거나 이미 PendingKill 이란 얘기는 DestroySelf 를 두번 호출했던지, 아님 월드가 내려가는 상황이든지.. 
	//if (!HasAnyFlags(EObjectFlags::RF_BeginDestroyed) && !IsPendingKillOrUnreachable())
	//{
	//	APlayerController* OwningPC = GetOwningPlayer();
	//	BII_CHECK(OwningPC);
	//	// CustomTimer 도 모두 제거
	//	if (OwningPC)
	//	{
	//		for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
	//		{
	//			OwningPC->GetWorldTimerManager().ClearTimer(CustomTimedCallBackInfo[CTI].TimerHandle);
	//		}
	//		for (int32 CTI = 0; CTI < CustomTimedHideInfo.Num(); ++CTI)
	//		{
	//			OwningPC->GetWorldTimerManager().ClearTimer(CustomTimedHideInfo[CTI].HideStartTimerHandle);
	//			OwningPC->GetWorldTimerManager().ClearTimer(CustomTimedHideInfo[CTI].HideStopTimerHandle);
	//		}
	//	}
	//}

	//UB2UnitedWidgetBase::DestroySelfCommon();
}

#if WITH_EDITOR
void UB2UIWidgetBase::PIEEndDestroy(class UB2UIManager* InUIManager)
{

}
#endif

void UB2UIWidgetBase::SetupWidgetCustomTimer()
{
	// 월드가 pause 되어 있는 상태에서 사용되는 widget 이라면 타이머는 먹히지 않을 것.예를 들어 Pause 메뉴나 StageClear 메뉴 같은 거.
	// 그런 경우는 어차피 타이머가 작동하진 않을 테니 NativeTick 을 쓰든지 해야.
	if (GetOwningPlayer())
	{
		for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
		{
			if (CustomTimedCallBackInfo[CTI].InvokeTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					CustomTimedCallBackInfo[CTI].TimerHandle, FTimerDelegate::CreateUObject(this, &UB2UIWidgetBase::OriginalCustomTimeCallback, CTI), CustomTimedCallBackInfo[CTI].InvokeTime, false
					);
			}
		}
	}
}

void UB2UIWidgetBase::OriginalCustomTimeCallback(int32 InfoIndex)
{
	if (GetOwningPlayer() && InfoIndex >= 0 && InfoIndex < CustomTimedCallBackInfo.Num())
	{
		GetOwningPlayer()->GetWorldTimerManager().ClearTimer(CustomTimedCallBackInfo[InfoIndex].TimerHandle);

		CustomTimedCallBackInfo[InfoIndex].bIsInvoked = true; // 타이머 작동 상황이라면 필요 없지만 예의상 세팅하자.
	}

	NativeCustomTimeCallback(InfoIndex);
	CustomTimeCallback(InfoIndex);
}

void UB2UIWidgetBase::NativeCustomTimeCallback(int32 InfoIndex)
{

}

void UB2UIWidgetBase::SetupWidgetCustomHideTimer()
{
	// 월드가 pause 되어 있는 상태에서 사용되는 widget 이라면 타이머는 먹히지 않을 것.예를 들어 Pause 메뉴나 StageClear 메뉴 같은 거.
	// 그런 경우는 어차피 타이머가 작동하진 않을 테니 NativeTick 을 쓰든지 해야.
	BII_CHECK(GetOwningPlayer());
	if (GetOwningPlayer())
	{
		for (int32 CTI = 0; CTI < CustomTimedHideInfo.Num(); ++CTI)
		{
			FB2UIWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[CTI];

			UWidget* WidgetToHide = GetWidgetFromName(ThisHideInfo.WidgetNameToHide);

			if (WidgetToHide == NULL)
			{
				continue;
			}

			ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide 할 때에도 백업해 놓지만 아래 로직에 따라 HideStop 이 먼저 될 수도 있으니 여기서도 한번.

			// HideStopTime 이 HideStartTime 보다 큰지에 대한 확인은.. 굳이 하지 않는다.

			if (ThisHideInfo.HideStartTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					ThisHideInfo.HideStartTimerHandle, FTimerDelegate::CreateUObject(this, &UB2UIWidgetBase::CustomTimeHideStartCallback, CTI), ThisHideInfo.HideStartTime, false
					);
			}
			else
			{
				CustomTimeHideStartCallback(CTI); // 바로 콜백 실행
			}

			if (ThisHideInfo.HideStopTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					ThisHideInfo.HideStopTimerHandle, FTimerDelegate::CreateUObject(this, &UB2UIWidgetBase::CustomTimeHideStopCallback, CTI), ThisHideInfo.HideStopTime, false
					);
			}
			else
			{
				CustomTimeHideStopCallback(CTI); // 바로 콜백 실행. 결국 HideStopTime 이 0 이하면 안 숨겨짐
			}
		}
	}
}

void UB2UIWidgetBase::CustomTimeHideStartCallback(int32 InfoIndex)
{
	if (GetOwningPlayer() && InfoIndex >= 0 && InfoIndex < CustomTimedHideInfo.Num())
	{
		FB2UIWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[InfoIndex];

		GetOwningPlayer()->GetWorldTimerManager().ClearTimer(ThisHideInfo.HideStartTimerHandle);

		UWidget* WidgetToHide = GetWidgetFromName(ThisHideInfo.WidgetNameToHide);

		if (WidgetToHide)
		{
			ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // 일단 이전 상태를 백업해 놓고 숨긴다.
			WidgetToHide->SetVisibility(ESlateVisibility::Hidden);
			ThisHideInfo.bIsHidden = true;
			ThisHideInfo.bHiddenDoneOnce = true;
		}
	}
}

void UB2UIWidgetBase::CustomTimeHideStopCallback(int32 InfoIndex)
{
	if (GetOwningPlayer() && InfoIndex >= 0 && InfoIndex < CustomTimedHideInfo.Num())
	{
		FB2UIWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[InfoIndex];

		GetOwningPlayer()->GetWorldTimerManager().ClearTimer(ThisHideInfo.HideStopTimerHandle);

		UWidget* WidgetToHide = GetWidgetFromName(ThisHideInfo.WidgetNameToHide);

		if (WidgetToHide)
		{
			WidgetToHide->SetVisibility(ThisHideInfo.OriginalVisibility);
			ThisHideInfo.bIsHidden = false;
		}
	}
}
