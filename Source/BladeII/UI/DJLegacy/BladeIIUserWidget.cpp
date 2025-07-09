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

	// CustomTimeCallback Ÿ�̸� ��ġ. 
	SetupWidgetCustomTimer();
	// CustomTimedHide Ÿ�̸� ��ġ
	SetupWidgetCustomHideTimer();

	CacheAssets(); // �ʿ信 ���� BP �� ��ġ�� widget ���� ����Ƽ�� ���۷����� ĳ���Ѵ�.

	RegisterUIMarkForRedDot();
	DoMarkRedDot();
}

bool UBladeIIUserWidget::StartWorldBackgroundBlur()
{
	if (bAllowWorldBackgroundBlur)
	{
		// �Ƹ��� �̰� ó���� ABladeIIGameMode::BeginGlobalSceneBlur ������ �������ο� ���� ���ϰ��� �ִµ� Event �δ� ���ϰ��� �޾ƿ� �� ����.. �� ���ʵ� �÷��� ����
		BeginGlobalSceneBlurClass<float>::GetInstance().Signal(WorldBackgroundBlurScale);
		bWorldBackgroundBlurOn = true;

		if (bHideInGameHUDForWorldBackgroundBlur)
		{
			// �̰� ��Ȳ�� ���� �Ϻ��� ���������� �ʴµ� gbHACKInGameUIHiddenByDevCommand �� ����. 
			// �Ƹ��� WorldBackgroundBlur �� ���� ��Ȳ���� �װͱ��� ������ �ʿ䰡 ���� ������ ��.
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
	// ���尡 pause �Ǿ� �ִ� ���¿��� ���Ǵ� widget �̶�� Ÿ�̸Ӵ� ������ ���� ��.���� ��� Pause �޴��� StageClear �޴� ���� ��.
	// �׷� ���� ������ Ÿ�̸Ӱ� �۵����� ���� �״� NativeTick �� ������ �ؾ�.
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

		CustomTimedCallBackInfo[InfoIndex].bIsInvoked = true; // Ÿ�̸� �۵� ��Ȳ�̶�� �ʿ� ������ ���ǻ� ��������.
	}

	NativeCustomTimeCallback(InfoIndex);
	CustomTimeCallback(InfoIndex);
}

void UBladeIIUserWidget::NativeCustomTimeCallback(int32 InfoIndex)
{
	
}

void UBladeIIUserWidget::SetupWidgetCustomHideTimer()
{
	// ���尡 pause �Ǿ� �ִ� ���¿��� ���Ǵ� widget �̶�� Ÿ�̸Ӵ� ������ ���� ��.���� ��� Pause �޴��� StageClear �޴� ���� ��.
	// �׷� ���� ������ Ÿ�̸Ӱ� �۵����� ���� �״� NativeTick �� ������ �ؾ�.
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

			ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide �� ������ ����� ������ �Ʒ� ������ ���� HideStop �� ���� �� ���� ������ ���⼭�� �ѹ�.

			// HideStopTime �� HideStartTime ���� ū���� ���� Ȯ����.. ���� ���� �ʴ´�.

			if (ThisHideInfo.HideStartTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					ThisHideInfo.HideStartTimerHandle, FTimerDelegate::CreateUObject(this, &UBladeIIUserWidget::CustomTimeHideStartCallback, CTI), ThisHideInfo.HideStartTime, false
					);
			}
			else
			{
				CustomTimeHideStartCallback(CTI); // �ٷ� �ݹ� ����
			}

			if (ThisHideInfo.HideStopTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					ThisHideInfo.HideStopTimerHandle, FTimerDelegate::CreateUObject(this, &UBladeIIUserWidget::CustomTimeHideStopCallback, CTI), ThisHideInfo.HideStopTime, false
					);
			}
			else
			{
				CustomTimeHideStopCallback(CTI); // �ٷ� �ݹ� ����. �ᱹ HideStopTime �� 0 ���ϸ� �� ������
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
			ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // �ϴ� ���� ���¸� ����� ���� �����.
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
	//StopWorldBackgroundBlur(); // ���� ���� �ִٸ� �ߴ�.

	//// ���⼭ BeginDestroyed ���ų� �̹� PendingKill �̶� ���� DestroySelf �� �ι� ȣ���ߴ���, �ƴ� ���尡 �������� ��Ȳ�̵���.. 
	//if (!HasAnyFlags(EObjectFlags::RF_BeginDestroyed) && !IsPendingKillOrUnreachable())
	//{
	//	APlayerController* OwningPC = GetOwningPlayer();
	//	BII_CHECK(OwningPC);
	//	// CustomTimer �� ��� ����
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
