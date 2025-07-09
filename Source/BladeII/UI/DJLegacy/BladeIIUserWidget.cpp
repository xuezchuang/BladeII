// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#include "BladeII.h"
#include "BladeIIUserWidget.h"
#include "B2UIWidgetBase.h"
#include "B2UIManager_InGameHUDChar.h"
#include "BladeIIGameMode.h"
#include "Event.h"
#include "Particles/ParticleSystemComponent.h"

UParticleSystemComponent* FSingleUMGParticleSetup::CreateUnderScreen(APlayerController* InPC, FVector2D ScreenPos)
{
	if (InPC)
	{
		FTransform FXCreateTransform(
			UB2UIWidgetBase::GetWorldPosBeneathScreen(InPC, ScreenPos, CreateDepth)
			);
		UParticleSystemComponent* CreatedFx = UGameplayStatics::SpawnEmitterAtLocation(InPC->GetWorld(), FxTemplate, FXCreateTransform);
		if (CreatedFx)
		{
			CreatedFx->SetWorldScale3D(FxScale);
			CreatedFx->SecondsBeforeInactive = 0.f;
			return CreatedFx;
		}
	}
	return NULL;
}

UBladeIIUserWidget::UBladeIIUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowWorldBackgroundBlur = false;
	bWorldBackgroundBlurOn = false;
	WorldBackgroundBlurScale = 1.0f;
	bHideInGameHUDForWorldBackgroundBlur = true;
}

void UBladeIIUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartWorldBackgroundBlur();

	// CustomTimeCallback 타이머 설치. 
	SetupWidgetCustomTimer();
	// CustomTimedHide 타이머 설치
	SetupWidgetCustomHideTimer();

	CacheAssets(); // 필요에 따라 BP 에 배치한 widget 들을 네이티브 레퍼런스에 캐싱한다.

	RegisterUIMarkForRedDot();
	DoMarkRedDot();
}

bool UBladeIIUserWidget::StartWorldBackgroundBlur()
{
	if (bAllowWorldBackgroundBlur)
	{
		// 아마도 이걸 처리할 ABladeIIGameMode::BeginGlobalSceneBlur 에서는 성공여부에 따라 리턴값을 주는데 Event 로는 리턴값을 받아올 수 없고.. 걍 이쪽도 플래그 세팅
		BeginGlobalSceneBlurClass<float>::GetInstance().Signal(WorldBackgroundBlurScale);
		bWorldBackgroundBlurOn = true;

		if (bHideInGameHUDForWorldBackgroundBlur)
		{
			// 이게 상황에 따라 완벽히 숨겨주지는 않는데 gbHACKInGameUIHiddenByDevCommand 를 참고. 
			// 아마도 WorldBackgroundBlur 를 쓰는 상황에선 그것까지 감안할 필요가 없지 않을까 함.
			for (TObjectIterator<UB2UIManager_InGameHUDChar> WMIT; WMIT; ++WMIT)
			{
				(*WMIT)->HideAll();
			}
		}
		return true;
	}

	return false;
}

void UBladeIIUserWidget::StopWorldBackgroundBlur()
{
	if (bWorldBackgroundBlurOn)
	{
		EndGlobalSceneBlurClass<>::GetInstance().Signal();

		if (bHideInGameHUDForWorldBackgroundBlur)
		{
			for (TObjectIterator<UB2UIManager_InGameHUDChar> WMIT; WMIT; ++WMIT)
			{
				(*WMIT)->RestoreVisibilityAll();
			}
		}

		bWorldBackgroundBlurOn = false;
	}
}

void UBladeIIUserWidget::SetupWidgetCustomTimer()
{
	// 월드가 pause 되어 있는 상태에서 사용되는 widget 이라면 타이머는 먹히지 않을 것.예를 들어 Pause 메뉴나 StageClear 메뉴 같은 거.
	// 그런 경우는 어차피 타이머가 작동하진 않을 테니 NativeTick 을 쓰든지 해야.
	BII_CHECK(GetOwningPlayer());
	if (GetOwningPlayer())
	{
		for (int32 CTI = 0; CTI < CustomTimedCallBackInfo.Num(); ++CTI)
		{
			if (CustomTimedCallBackInfo[CTI].InvokeTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					CustomTimedCallBackInfo[CTI].TimerHandle, FTimerDelegate::CreateUObject(this, &UBladeIIUserWidget::OriginalCustomTimeCallback, CTI), CustomTimedCallBackInfo[CTI].InvokeTime, false
					);
			}
		}
	}
}

void UBladeIIUserWidget::OriginalCustomTimeCallback(int32 InfoIndex)
{
	BII_CHECK(GetOwningPlayer());
	if (GetOwningPlayer() && InfoIndex >= 0 && InfoIndex < CustomTimedCallBackInfo.Num())
	{
		GetOwningPlayer()->GetWorldTimerManager().ClearTimer(CustomTimedCallBackInfo[InfoIndex].TimerHandle);

		CustomTimedCallBackInfo[InfoIndex].bIsInvoked = true; // 타이머 작동 상황이라면 필요 없지만 예의상 세팅하자.
	}

	NativeCustomTimeCallback(InfoIndex);
	CustomTimeCallback(InfoIndex);
}

void UBladeIIUserWidget::NativeCustomTimeCallback(int32 InfoIndex)
{
	
}

void UBladeIIUserWidget::SetupWidgetCustomHideTimer()
{
	// 월드가 pause 되어 있는 상태에서 사용되는 widget 이라면 타이머는 먹히지 않을 것.예를 들어 Pause 메뉴나 StageClear 메뉴 같은 거.
	// 그런 경우는 어차피 타이머가 작동하진 않을 테니 NativeTick 을 쓰든지 해야.
	BII_CHECK(GetOwningPlayer());
	if (GetOwningPlayer())
	{
		for (int32 CTI = 0; CTI < CustomTimedHideInfo.Num(); ++CTI)
		{
			FWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[CTI];

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
					ThisHideInfo.HideStartTimerHandle, FTimerDelegate::CreateUObject(this, &UBladeIIUserWidget::CustomTimeHideStartCallback, CTI), ThisHideInfo.HideStartTime, false
					);
			}
			else
			{
				CustomTimeHideStartCallback(CTI); // 바로 콜백 실행
			}

			if (ThisHideInfo.HideStopTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					ThisHideInfo.HideStopTimerHandle, FTimerDelegate::CreateUObject(this, &UBladeIIUserWidget::CustomTimeHideStopCallback, CTI), ThisHideInfo.HideStopTime, false
					);
			}
			else
			{
				CustomTimeHideStopCallback(CTI); // 바로 콜백 실행. 결국 HideStopTime 이 0 이하면 안 숨겨짐
			}
		}
	}
}

void UBladeIIUserWidget::CustomTimeHideStartCallback(int32 InfoIndex)
{
	BII_CHECK(GetOwningPlayer());
	if (GetOwningPlayer() && InfoIndex >= 0 && InfoIndex < CustomTimedHideInfo.Num())
	{
		FWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[InfoIndex];

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

void UBladeIIUserWidget::CustomTimeHideStopCallback(int32 InfoIndex)
{
	BII_CHECK(GetOwningPlayer());
	if (GetOwningPlayer() && InfoIndex >= 0 && InfoIndex < CustomTimedHideInfo.Num())
	{
		FWidgetCustomTimedHideInfo& ThisHideInfo = CustomTimedHideInfo[InfoIndex];

		GetOwningPlayer()->GetWorldTimerManager().ClearTimer(ThisHideInfo.HideStopTimerHandle);

		UWidget* WidgetToHide = GetWidgetFromName(ThisHideInfo.WidgetNameToHide);

		if (WidgetToHide)
		{
			WidgetToHide->SetVisibility(ThisHideInfo.OriginalVisibility);
			ThisHideInfo.bIsHidden = false;
		}
	}
}

void UBladeIIUserWidget::DestroySelf()
{
	//StopWorldBackgroundBlur(); // 만일 켜져 있다면 중단.

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
