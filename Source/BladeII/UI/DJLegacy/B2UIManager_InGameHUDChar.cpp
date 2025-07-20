// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIManager_InGameHUDChar.h"
//#include "BladeII.h"

#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
#include "BladeIIPlayerController.h"
#include "B2FloatingHPBar.h"
#include "B2FloatingDamageNum.h"
#include "B2FloatingAbnormalEffect.h"
#include "B2ControlGameMode.h"
#include "B2RaidGameMode.h"
#include "B2FloatingResurrectionBar.h"
#include "B2UIManager_InGameCombat.h"
#include "BladeIIUtil.h"

//////////////////////////////////////////////////
//
// 某腐磐 窜困肺 鞘夸茄 floating widget 幅 包府 (e.g. HP 官)
// 酒付档 拌加 荤侩瞪 淀茄 DJLegacy UI 老何
//
//////////////////////////////////////////////////

UB2UIManager_InGameHUDChar::UB2UIManager_InGameHUDChar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCurrentlyHideAll = false;
	FloatingHPBarWidget = NULL;	
	OwnerCharacter = NULL;
	MasterUIManager = NULL;
	OwnerGameMode = NULL;
	CachedPlayerController = NULL;
}

bool UB2UIManager_InGameHUDChar::InitUIManager(class ABladeIICharacter* InOwner, AB2UIManager_InGameCombat* InMasterManager)
{
//	OwnerCharacter = InOwner;
//	MasterUIManager = InMasterManager;
//	OwnerGameMode = Cast<AGameMode>(UGameplayStatics::GetGameMode(OwnerCharacter));
//
//	check(OwnerCharacter && OwnerCharacter->IsValidObj() && MasterUIManager && MasterUIManager->IsValidObj() && OwnerGameMode && OwnerGameMode->IsValidObj());
//	
//	// If the widget class is specified, create relevant UI widget and add it the the local player's viewport.
//	CachedPlayerController = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(OwnerCharacter));
//	if (CachedPlayerController == NULL)
//	{
//		return false;
//	}
//	bCachedHUDHidingCinematicMode = CachedPlayerController->IsHUDHidingCinematicMode();
//
//	// FloatingHP 官绰 咯傈洒 捞率 家包
//	if (OwnerCharacter->FloatingHPBarWidgetClass != NULL)
//	{
//		FloatingHPBarWidget = CreateWidget<UB2FloatingHPBar>(CachedPlayerController, OwnerCharacter->FloatingHPBarWidgetClass);
//		if (FloatingHPBarWidget)
//		{
//			FloatingHPBarWidget->AddToViewport(BII_WIDGET_ZORDER_FLOATINGHPBAR);
//			FloatingHPBarWidget->SetOwnerCharacter(OwnerCharacter);
//		}
//	}
//
//	// 霸烙葛靛喊 眠啊UI
//	AdditionalUIForGameMode();
//
//	// 贸澜何磐 Cinematic mode 牢 版快啊 乐绰单 UpdateManual 俊辑 某摹窍瘤 给窍骨肺 茄锅 眉农
//	if (bCachedHUDHidingCinematicMode)
//	{
//		HideAll();
//	}
//#if !UE_BUILD_SHIPPING
//	extern bool gbHACKInGameUIHiddenByDevCommand;
//	if (gbHACKInGameUIHiddenByDevCommand) // 俺惯侩 目盖靛肺 见变 版快俊 措秦
//	{
//		HideAll();
//	}
//#endif

	return true;
}

void UB2UIManager_InGameHUDChar::UpdateManual(float DeltaSeconds)
{
//	// For FloatingHPBarWidget and HiddenMobPointerWidget, I would like to handle their update in their NativeTick,
//	// but have no idea why it's not working.. possibly because their position is dynamic?
//	if (FloatingHPBarWidget)
//		FloatingHPBarWidget->UpdatePosition();
//
//	for (int32 DWI = 0; DWI < DamageNumWidgetSetArray.Num(); ++DWI)
//	{
//		UB2FloatingWidget* DamageNumWidget = DamageNumWidgetSetArray[DWI].FloatingWidget;
//		if (DamageNumWidget)
//		{
//			DamageNumWidget->UpdatePosition();
//		}
//	}
//	for (int32 AWI = 0; AWI < AbnormalEffectWidgetSetArray.Num(); ++AWI)
//	{
//		UB2FloatingWidget* ThisWidget = AbnormalEffectWidgetSetArray[AWI].FloatingWidget;
//		if (ThisWidget)
//		{
//			ThisWidget->UpdatePosition();
//		}
//	}
//	// 矫匙付平俊辑 UI 掺扁.
//	if (bCachedHUDHidingCinematicMode != CachedPlayerController->IsHUDHidingCinematicMode())
//	{
//		bCachedHUDHidingCinematicMode = CachedPlayerController->IsHUDHidingCinematicMode();
//
//		if (bCachedHUDHidingCinematicMode)
//		{
//			HideAll();
//		}
//		else
//		{
//			RestoreVisibilityAll();
//#if !UE_BUILD_SHIPPING // InGameUI 摹飘甫 借促搁 促矫 见变促 せ
//			extern bool gbHACKInGameUIHiddenByDevCommand;
//			if (gbHACKInGameUIHiddenByDevCommand)
//			{
//				HideAll();
//			}
//#endif
//		}
//	}
}

void UB2UIManager_InGameHUDChar::NotifyCharacterDeath(class ABladeIICharacter* InNotifyOwner)
{
	if (FloatingHPBarWidget)
	{
		FloatingHPBarWidget->NotifyCharacterDeath(InNotifyOwner);
	}
}

void UB2UIManager_InGameHUDChar::DestroyUserWidgets()
{
	if (FloatingHPBarWidget)
	{
		FloatingHPBarWidget->DestroySelf();
		FloatingHPBarWidget = NULL;
	}

	for (int32 DWI = 0; DWI < DamageNumWidgetSetArray.Num(); ++DWI)
	{
		UB2FloatingWidget* DamageNumWidget = DamageNumWidgetSetArray[DWI].FloatingWidget;
		if (DamageNumWidget && MasterUIManager)
		{ // 馆吵
			MasterUIManager->TurnInFloatingWidget(MasterUIManager->GetDamageNumWidgetPoolTypeByOwnerChar(OwnerCharacter), DamageNumWidget);
			OwnerGameMode->GetWorldTimerManager().ClearTimer(DamageNumWidgetSetArray[DWI].WidgetDestroyTimerHandle);
		}
	}
	DamageNumWidgetSetArray.Empty();

	for (int32 AWI = 0; AWI < AbnormalEffectWidgetSetArray.Num(); ++AWI)
	{
		UB2FloatingWidget* ThisWidget = AbnormalEffectWidgetSetArray[AWI].FloatingWidget;
		if (ThisWidget && MasterUIManager)
		{ // 馆吵
			MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_AbnormalEffect, ThisWidget);
			OwnerGameMode->GetWorldTimerManager().ClearTimer(AbnormalEffectWidgetSetArray[AWI].WidgetDestroyTimerHandle);
		}
	}
	AbnormalEffectWidgetSetArray.Empty();

	// 眠啊登绰 巴甸 持绢淋
}

void UB2UIManager_InGameHUDChar::HideFloatingHPBar(bool bInHide)
{
	if (FloatingHPBarWidget)
	{
		if (bInHide){
			FloatingHPBarWidget->ForceHide();
		}
		else if (!bCurrentlyHideAll){
			FloatingHPBarWidget->RestoreFromForceHidden();
		}
	}
}

void UB2UIManager_InGameHUDChar::HideAll()
{
	bCurrentlyHideAll = true;

	if (FloatingHPBarWidget)
		FloatingHPBarWidget->ForceHide();
	
	for (int32 DWI = 0; DWI < DamageNumWidgetSetArray.Num(); ++DWI)
	{
		UB2FloatingWidget* DamageNumWidget = DamageNumWidgetSetArray[DWI].FloatingWidget;
		if (DamageNumWidget)
		{
			DamageNumWidget->ForceHide();
		}
	}
	for (int32 AWI = 0; AWI < AbnormalEffectWidgetSetArray.Num(); ++AWI)
	{
		UB2FloatingWidget* ThisWidget = AbnormalEffectWidgetSetArray[AWI].FloatingWidget;
		if (ThisWidget)
		{
			ThisWidget->ForceHide();
		}
	}

	// 眠啊登绰 巴甸 持绢淋
}

void UB2UIManager_InGameHUDChar::RestoreVisibilityAll()
{
	bCurrentlyHideAll = false;

	if (FloatingHPBarWidget)
		FloatingHPBarWidget->RestoreFromForceHidden();
		
	for (int32 DWI = 0; DWI < DamageNumWidgetSetArray.Num(); ++DWI)
	{
		UB2FloatingWidget* DamageNumWidget = DamageNumWidgetSetArray[DWI].FloatingWidget;
		if (DamageNumWidget)
		{
			DamageNumWidget->RestoreFromForceHidden();
		}
	}
	for (int32 AWI = 0; AWI < AbnormalEffectWidgetSetArray.Num(); ++AWI)
	{
		UB2FloatingWidget* ThisWidget = AbnormalEffectWidgetSetArray[AWI].FloatingWidget;
		if (ThisWidget)
		{
			ThisWidget->RestoreFromForceHidden();
		}
	}

	// 眠啊登绰 巴甸 持绢淋
}

bool UB2UIManager_InGameHUDChar::IsOwnerPlayer()
{
	return (Cast<ABladeIIPlayer>(OwnerCharacter) != NULL);
}

void UB2UIManager_InGameHUDChar::AdditionalUIForGameMode()
{
	//if (!OwnerGameMode || !CachedPlayerController)
	//	return;

	//AB2ControlGameMode* pControlGamemode = Cast<AB2ControlGameMode>(OwnerGameMode);
	//if (pControlGamemode && !FloatingHPBarWidget)
	//{
	//	// 痢飞傈 霸烙葛靛俊 汲沥等吧肺 父惦
	//	FloatingHPBarWidget = CreateWidget<UB2FloatingHPBar>(CachedPlayerController, pControlGamemode->FloatingHPBarWidgetClass);
	//	if (FloatingHPBarWidget)
	//	{
	//		FloatingHPBarWidget->AddToViewport(BII_WIDGET_ZORDER_FLOATINGHPBAR);
	//		FloatingHPBarWidget->SetOwnerCharacter(OwnerCharacter);
	//	}
	//}

	//AB2RaidGameMode* pRaidGamemode = Cast<AB2RaidGameMode>(OwnerGameMode);
	//if (pRaidGamemode && !FloatingHPBarWidget)
	//{
	//	FloatingHPBarWidget = CreateWidget<UB2FloatingResurrectionBar>(CachedPlayerController, pRaidGamemode->FloatingResurrectionBarWidgetClass);
	//	if (FloatingHPBarWidget)
	//	{
	//		FloatingHPBarWidget->AddToViewport(BII_WIDGET_ZORDER_FLOATINGHPBAR);
	//		FloatingHPBarWidget->SetOwnerCharacter(OwnerCharacter);
	//	}
	//}
}

void UB2UIManager_InGameHUDChar::NotifyBeginPlay()
{
	// 老窜 喊杭老 绝绊 敲饭捞绢 率俊..
}

void UB2UIManager_InGameHUDChar::NotifyTakeDamage(float Damage, bool bInCritical)
{

	// Changed Damage Number to spawn as effect, in TakeDamage directly

	// TakeDamage 捞亥飘付促 DamageNumWidget 积己
	CreateSingleDamageNumWidget(Damage, bInCritical);
}

void UB2UIManager_InGameHUDChar::NotifyAbnormalEffect(const FLoadedFloatingAbnormalEffectSetupInfo& InEffectSetupInfo)
{
	CreateAbnormalEffectWidget(InEffectSetupInfo);
}

UB2FloatingDamageNum* UB2UIManager_InGameHUDChar::CreateSingleDamageNumWidget(float Damage, bool bInCritical)
{
#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
	extern bool gDisableInGameFloatingWidgets;
	if (gDisableInGameFloatingWidgets)
		return nullptr;
#endif

	if (MasterUIManager && OwnerCharacter && OwnerGameMode)
	{
		// 捞力 积己捞 酒聪扼 Rent 捞促. 积己捞 瞪 荐档 乐变 窍瘤.

		auto PoolType = MasterUIManager->GetDamageNumWidgetPoolTypeByOwnerChar(OwnerCharacter);
		auto* FloatingWidget = MasterUIManager->RentFloatingWidget(PoolType, CachedPlayerController, OwnerCharacter);

		if (auto* DamageWidget = Cast<UB2FloatingDamageNum>(FloatingWidget))
		{
			// 盔贰 DamageNumWidget 窍唱父 积己秦辑 概 TakeDamage 捞亥飘付促 货肺 颇扼固磐 技泼窍带 捞傈 内靛 儡犁牢单 出抵辑 救瞪 巴档 绝绢 焊咯辑 弊措肺 靖.
			// -> 出底辨贰 促青.. 搬惫 FloatWidget Pooling 阑 窍搁辑 捞傈 规侥捞 鞘夸秦咙
			DamageWidget->NotifyTakeDamage(Damage, bInCritical);

			if (bCurrentlyHideAll) // 档吝俊 积己茄促搁 见扁绰 吧 鳖冈绢急 救登摆促.
				DamageWidget->ForceHide();

			else
				DamageWidget->RestoreFromForceHidden();

			FDynamicFloatingWidgetSet NewWidgetSet;
			NewWidgetSet.FloatingWidget = DamageWidget;

			// VisibleDuration 栏肺 累悼窍绰 秦力 鸥捞赣 汲摹
			OwnerGameMode->GetWorldTimerManager().SetTimer(NewWidgetSet.WidgetDestroyTimerHandle,
				FTimerDelegate::CreateUObject(this, &UB2UIManager_InGameHUDChar::DestroySingleDamageNumWidgetTimerCB, DamageWidget),
				DamageWidget->GetVisibleDuration(), false);

			DamageNumWidgetSetArray.Add(NewWidgetSet);
			return DamageWidget;
		}
	}

	return NULL;
}

void UB2UIManager_InGameHUDChar::DestroySingleDamageNumWidgetTimerCB(class UB2FloatingDamageNum* WidgetToDestroy)
{
	if (WidgetToDestroy && MasterUIManager)
	{
		for (int32 DWI = 0; DWI < DamageNumWidgetSetArray.Num(); ++DWI)
		{
			UB2FloatingWidget* CurrDW = DamageNumWidgetSetArray[DWI].FloatingWidget;
			if (CurrDW == WidgetToDestroy)
			{
				// 捞抚捞 烤朝 如利捞扼 Destroy 牢单 角力肺 捞力绰 Destroy 啊 酒聪扼 pool 俊 馆吵茄促.

				if (WidgetToDestroy->IsForceHidden() && bCurrentlyHideAll == false)
					WidgetToDestroy->RestoreFromForceHidden();

				MasterUIManager->TurnInFloatingWidget(MasterUIManager->GetDamageNumWidgetPoolTypeByOwnerChar(OwnerCharacter), WidgetToDestroy);
				OwnerGameMode->GetWorldTimerManager().ClearTimer(DamageNumWidgetSetArray[DWI].WidgetDestroyTimerHandle);
				DamageNumWidgetSetArray.RemoveAt(DWI);
				break;
			}
		}
	}
}

UB2FloatingAbnormalEffect* UB2UIManager_InGameHUDChar::CreateAbnormalEffectWidget(const FLoadedFloatingAbnormalEffectSetupInfo& InEffectSetupInfo)
{
#if !UE_BUILD_SHIPPING // Detailed feature On/Off for performance test
	extern bool gDisableInGameFloatingWidgets;
	if (gDisableInGameFloatingWidgets) {
		return nullptr;
	}
#endif

	if (MasterUIManager)
	{
		// 捞力 积己捞 酒聪扼 Rent 捞促. 积己捞 瞪 荐档 乐变 窍瘤.
		UB2FloatingAbnormalEffect* NewAbnormalEffectWidget = Cast<UB2FloatingAbnormalEffect>(MasterUIManager->RentFloatingWidget(
			EFloatingWidgetPoolClassType::EFWPCT_AbnormalEffect, CachedPlayerController, OwnerCharacter
		));

		if (NewAbnormalEffectWidget)
		{
			NewAbnormalEffectWidget->NotifyAbnormalEffect(InEffectSetupInfo);

			if (bCurrentlyHideAll) // 档吝俊 积己茄促搁 见扁绰 吧 鳖冈绢急 救登摆促.
			{
				NewAbnormalEffectWidget->ForceHide();
			}
			else
			{
				NewAbnormalEffectWidget->RestoreFromForceHidden();
			}

			FDynamicFloatingWidgetSet NewWidgetSet;
			NewWidgetSet.FloatingWidget = NewAbnormalEffectWidget;

			// VisibleDuration 栏肺 累悼窍绰 秦力 鸥捞赣 汲摹
			OwnerGameMode->GetWorldTimerManager().SetTimer(NewWidgetSet.WidgetDestroyTimerHandle,
				FTimerDelegate::CreateUObject(this, &UB2UIManager_InGameHUDChar::DestroySingleAbnormalEffectWidgetTimerCB, NewAbnormalEffectWidget),
				NewAbnormalEffectWidget->GetVisibleDuration(), false);

			AbnormalEffectWidgetSetArray.Add(NewWidgetSet);

			return NewAbnormalEffectWidget;
		}
	}

	return NULL;
}

void UB2UIManager_InGameHUDChar::DestroySingleAbnormalEffectWidgetTimerCB(class UB2FloatingAbnormalEffect* WidgetToDestroy)
{
	if (WidgetToDestroy && MasterUIManager)
	{
		for (int32 AWI = 0; AWI < AbnormalEffectWidgetSetArray.Num(); ++AWI)
		{
			UB2FloatingWidget* CurrAbnormalEffect = AbnormalEffectWidgetSetArray[AWI].FloatingWidget;
			if (CurrAbnormalEffect == WidgetToDestroy)
			{
				// 捞抚捞 烤朝 如利捞扼 Destroy 牢单 角力肺 捞力绰 Destroy 啊 酒聪扼 pool 俊 馆吵茄促.
				MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_AbnormalEffect, WidgetToDestroy);
				OwnerGameMode->GetWorldTimerManager().ClearTimer(AbnormalEffectWidgetSetArray[AWI].WidgetDestroyTimerHandle);
				AbnormalEffectWidgetSetArray.RemoveAt(AWI);
				break;
			}
		}
	}
}
