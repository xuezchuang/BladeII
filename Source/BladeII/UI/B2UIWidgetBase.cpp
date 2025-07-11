
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

	// CustomTimeCallback Ÿ�̸� ��ġ. 
	SetupWidgetCustomTimer();
	// CustomTimedHide Ÿ�̸� ��ġ
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

#if WITH_EDITOR
void UB2UIWidgetBase::PIEEndDestroy(class UB2UIManager* InUIManager)
{

}
#endif

void UB2UIWidgetBase::SetupWidgetCustomTimer()
{
	// ���尡 pause �Ǿ� �ִ� ���¿��� ���Ǵ� widget �̶�� Ÿ�̸Ӵ� ������ ���� ��.���� ��� Pause �޴��� StageClear �޴� ���� ��.
	// �׷� ���� ������ Ÿ�̸Ӱ� �۵����� ���� �״� NativeTick �� ������ �ؾ�.
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

		CustomTimedCallBackInfo[InfoIndex].bIsInvoked = true; // Ÿ�̸� �۵� ��Ȳ�̶�� �ʿ� ������ ���ǻ� ��������.
	}

	NativeCustomTimeCallback(InfoIndex);
	CustomTimeCallback(InfoIndex);
}

void UB2UIWidgetBase::NativeCustomTimeCallback(int32 InfoIndex)
{

}

void UB2UIWidgetBase::SetupWidgetCustomHideTimer()
{
	// ���尡 pause �Ǿ� �ִ� ���¿��� ���Ǵ� widget �̶�� Ÿ�̸Ӵ� ������ ���� ��.���� ��� Pause �޴��� StageClear �޴� ���� ��.
	// �׷� ���� ������ Ÿ�̸Ӱ� �۵����� ���� �״� NativeTick �� ������ �ؾ�.
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

			ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide �� ������ ����� ������ �Ʒ� ������ ���� HideStop �� ���� �� ���� ������ ���⼭�� �ѹ�.

			// HideStopTime �� HideStartTime ���� ū���� ���� Ȯ����.. ���� ���� �ʴ´�.

			if (ThisHideInfo.HideStartTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					ThisHideInfo.HideStartTimerHandle, FTimerDelegate::CreateUObject(this, &UB2UIWidgetBase::CustomTimeHideStartCallback, CTI), ThisHideInfo.HideStartTime, false
					);
			}
			else
			{
				CustomTimeHideStartCallback(CTI); // �ٷ� �ݹ� ����
			}

			if (ThisHideInfo.HideStopTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					ThisHideInfo.HideStopTimerHandle, FTimerDelegate::CreateUObject(this, &UB2UIWidgetBase::CustomTimeHideStopCallback, CTI), ThisHideInfo.HideStopTime, false
					);
			}
			else
			{
				CustomTimeHideStopCallback(CTI); // �ٷ� �ݹ� ����. �ᱹ HideStopTime �� 0 ���ϸ� �� ������
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
			ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // �ϴ� ���� ���¸� ����� ���� �����.
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
