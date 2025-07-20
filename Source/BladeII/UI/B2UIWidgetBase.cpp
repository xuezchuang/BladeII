
#include "B2UIWidgetBase.h"
#include "../BladeII/BladeIIUtil.h"
//#include "MovieScene.h"
//#include "Animation/WidgetAnimation.h"
////#include "B2UIManager.h"
//
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

	// CustomTimeCallback 鸥捞赣 汲摹. 
	SetupWidgetCustomTimer();
	// CustomTimedHide 鸥捞赣 汲摹
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
	//// 咯扁辑 BeginDestroyed 看芭唱 捞固 PendingKill 捞鄂 娟扁绰 DestroySelf 甫 滴锅 龋免沁带瘤, 酒丛 岿靛啊 郴妨啊绰 惑炔捞电瘤.. 
	//if (!HasAnyFlags(EObjectFlags::RF_BeginDestroyed) && !IsPendingKillOrUnreachable())
	//{
	//	APlayerController* OwningPC = GetOwningPlayer();
	//	BII_CHECK(OwningPC);
	//	// CustomTimer 档 葛滴 力芭
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
	// 岿靛啊 pause 登绢 乐绰 惑怕俊辑 荤侩登绰 widget 捞扼搁 鸥捞赣绰 冈洒瘤 臼阑 巴.抗甫 甸绢 Pause 皋春唱 StageClear 皋春 鞍篮 芭.
	// 弊繁 版快绰 绢瞒乔 鸥捞赣啊 累悼窍柳 臼阑 抛聪 NativeTick 阑 静电瘤 秦具.
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

		CustomTimedCallBackInfo[InfoIndex].bIsInvoked = true; // 鸥捞赣 累悼 惑炔捞扼搁 鞘夸 绝瘤父 抗狼惑 技泼窍磊.
	}

	NativeCustomTimeCallback(InfoIndex);
	CustomTimeCallback(InfoIndex);
}

void UB2UIWidgetBase::NativeCustomTimeCallback(int32 InfoIndex)
{

}

void UB2UIWidgetBase::SetupWidgetCustomHideTimer()
{
	// 岿靛啊 pause 登绢 乐绰 惑怕俊辑 荤侩登绰 widget 捞扼搁 鸥捞赣绰 冈洒瘤 臼阑 巴.抗甫 甸绢 Pause 皋春唱 StageClear 皋春 鞍篮 芭.
	// 弊繁 版快绰 绢瞒乔 鸥捞赣啊 累悼窍柳 臼阑 抛聪 NativeTick 阑 静电瘤 秦具.
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

			ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide 且 锭俊档 归诀秦 初瘤父 酒贰 肺流俊 蝶扼 HideStop 捞 刚历 瞪 荐档 乐栏聪 咯扁辑档 茄锅.

			// HideStopTime 捞 HideStartTime 焊促 奴瘤俊 措茄 犬牢篮.. 被捞 窍瘤 臼绰促.

			if (ThisHideInfo.HideStartTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					ThisHideInfo.HideStartTimerHandle, FTimerDelegate::CreateUObject(this, &UB2UIWidgetBase::CustomTimeHideStartCallback, CTI), ThisHideInfo.HideStartTime, false
					);
			}
			else
			{
				CustomTimeHideStartCallback(CTI); // 官肺 妮归 角青
			}

			if (ThisHideInfo.HideStopTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					ThisHideInfo.HideStopTimerHandle, FTimerDelegate::CreateUObject(this, &UB2UIWidgetBase::CustomTimeHideStopCallback, CTI), ThisHideInfo.HideStopTime, false
					);
			}
			else
			{
				CustomTimeHideStopCallback(CTI); // 官肺 妮归 角青. 搬惫 HideStopTime 捞 0 捞窍搁 救 见败咙
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
			ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // 老窜 捞傈 惑怕甫 归诀秦 初绊 见变促.
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
