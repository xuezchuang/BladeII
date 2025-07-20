// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square


#include "BladeIIUserWidget.h"
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

	// CustomTimeCallback 鸥捞赣 汲摹. 
	SetupWidgetCustomTimer();
	// CustomTimedHide 鸥捞赣 汲摹
	SetupWidgetCustomHideTimer();

	CacheAssets(); // 鞘夸俊 蝶扼 BP 俊 硅摹茄 widget 甸阑 匙捞萍宏 饭欺繁胶俊 某教茄促.

	RegisterUIMarkForRedDot();
	DoMarkRedDot();
}

bool UBladeIIUserWidget::StartWorldBackgroundBlur()
{
	if (bAllowWorldBackgroundBlur)
	{
		// 酒付档 捞吧 贸府且 ABladeIIGameMode::BeginGlobalSceneBlur 俊辑绰 己傍咯何俊 蝶扼 府畔蔼阑 林绰单 Event 肺绰 府畔蔼阑 罐酒棵 荐 绝绊.. 傲 捞率档 敲贰弊 技泼
		BeginGlobalSceneBlurClass<float>::GetInstance().Signal(WorldBackgroundBlurScale);
		bWorldBackgroundBlurOn = true;

		if (bHideInGameHUDForWorldBackgroundBlur)
		{
			// 捞霸 惑炔俊 蝶扼 肯寒洒 见败林瘤绰 臼绰单 gbHACKInGameUIHiddenByDevCommand 甫 曼绊. 
			// 酒付档 WorldBackgroundBlur 甫 静绰 惑炔俊急 弊巴鳖瘤 皑救且 鞘夸啊 绝瘤 臼阑鳖 窃.
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
	// 岿靛啊 pause 登绢 乐绰 惑怕俊辑 荤侩登绰 widget 捞扼搁 鸥捞赣绰 冈洒瘤 臼阑 巴.抗甫 甸绢 Pause 皋春唱 StageClear 皋春 鞍篮 芭.
	// 弊繁 版快绰 绢瞒乔 鸥捞赣啊 累悼窍柳 臼阑 抛聪 NativeTick 阑 静电瘤 秦具.
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

		CustomTimedCallBackInfo[InfoIndex].bIsInvoked = true; // 鸥捞赣 累悼 惑炔捞扼搁 鞘夸 绝瘤父 抗狼惑 技泼窍磊.
	}

	NativeCustomTimeCallback(InfoIndex);
	CustomTimeCallback(InfoIndex);
}

void UBladeIIUserWidget::NativeCustomTimeCallback(int32 InfoIndex)
{
	
}

void UBladeIIUserWidget::SetupWidgetCustomHideTimer()
{
	// 岿靛啊 pause 登绢 乐绰 惑怕俊辑 荤侩登绰 widget 捞扼搁 鸥捞赣绰 冈洒瘤 臼阑 巴.抗甫 甸绢 Pause 皋春唱 StageClear 皋春 鞍篮 芭.
	// 弊繁 版快绰 绢瞒乔 鸥捞赣啊 累悼窍柳 臼阑 抛聪 NativeTick 阑 静电瘤 秦具.
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

			ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // Hide 且 锭俊档 归诀秦 初瘤父 酒贰 肺流俊 蝶扼 HideStop 捞 刚历 瞪 荐档 乐栏聪 咯扁辑档 茄锅.

			// HideStopTime 捞 HideStartTime 焊促 奴瘤俊 措茄 犬牢篮.. 被捞 窍瘤 臼绰促.

			if (ThisHideInfo.HideStartTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					ThisHideInfo.HideStartTimerHandle, FTimerDelegate::CreateUObject(this, &UBladeIIUserWidget::CustomTimeHideStartCallback, CTI), ThisHideInfo.HideStartTime, false
					);
			}
			else
			{
				CustomTimeHideStartCallback(CTI); // 官肺 妮归 角青
			}

			if (ThisHideInfo.HideStopTime > 0.0f)
			{
				GetOwningPlayer()->GetWorldTimerManager().SetTimer(
					ThisHideInfo.HideStopTimerHandle, FTimerDelegate::CreateUObject(this, &UBladeIIUserWidget::CustomTimeHideStopCallback, CTI), ThisHideInfo.HideStopTime, false
					);
			}
			else
			{
				CustomTimeHideStopCallback(CTI); // 官肺 妮归 角青. 搬惫 HideStopTime 捞 0 捞窍搁 救 见败咙
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
			ThisHideInfo.OriginalVisibility = WidgetToHide->GetVisibility(); // 老窜 捞傈 惑怕甫 归诀秦 初绊 见变促.
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
	//StopWorldBackgroundBlur(); // 父老 难廉 乐促搁 吝窜.

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
