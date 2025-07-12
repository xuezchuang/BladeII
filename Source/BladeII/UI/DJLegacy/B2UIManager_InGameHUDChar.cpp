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
// 캐릭터 단위로 필요한 floating widget 류 관리 (e.g. HP 바)
// 아마도 계속 사용될 듯한 DJLegacy UI 일부
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
//	// FloatingHP 바는 여전히 이쪽 소관
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
//	// 게임모드별 추가UI
//	AdditionalUIForGameMode();
//
//	// 처음부터 Cinematic mode 인 경우가 있는데 UpdateManual 에서 캐치하지 못하므로 한번 체크
//	if (bCachedHUDHidingCinematicMode)
//	{
//		HideAll();
//	}
//#if !UE_BUILD_SHIPPING
//	extern bool gbHACKInGameUIHiddenByDevCommand;
//	if (gbHACKInGameUIHiddenByDevCommand) // 개발용 커맨드로 숨긴 경우에 대해
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
//	// 시네마틱에서 UI 끄기.
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
//#if !UE_BUILD_SHIPPING // InGameUI 치트를 썼다면 다시 숨긴다 ㅋ
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
		{ // 반납
			MasterUIManager->TurnInFloatingWidget(MasterUIManager->GetDamageNumWidgetPoolTypeByOwnerChar(OwnerCharacter), DamageNumWidget);
			OwnerGameMode->GetWorldTimerManager().ClearTimer(DamageNumWidgetSetArray[DWI].WidgetDestroyTimerHandle);
		}
	}
	DamageNumWidgetSetArray.Empty();

	for (int32 AWI = 0; AWI < AbnormalEffectWidgetSetArray.Num(); ++AWI)
	{
		UB2FloatingWidget* ThisWidget = AbnormalEffectWidgetSetArray[AWI].FloatingWidget;
		if (ThisWidget && MasterUIManager)
		{ // 반납
			MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_AbnormalEffect, ThisWidget);
			OwnerGameMode->GetWorldTimerManager().ClearTimer(AbnormalEffectWidgetSetArray[AWI].WidgetDestroyTimerHandle);
		}
	}
	AbnormalEffectWidgetSetArray.Empty();

	// 추가되는 것들 넣어줌
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

	// 추가되는 것들 넣어줌
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

	// 추가되는 것들 넣어줌
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
	//	// 점령전 게임모드에 설정된걸로 만듬
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
	// 일단 별볼일 없고 플레이어 쪽에..
}

void UB2UIManager_InGameHUDChar::NotifyTakeDamage(float Damage, bool bInCritical)
{

	// Changed Damage Number to spawn as effect, in TakeDamage directly

	// TakeDamage 이벤트마다 DamageNumWidget 생성
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
		// 이제 생성이 아니라 Rent 이다. 생성이 될 수도 있긴 하지.

		auto PoolType = MasterUIManager->GetDamageNumWidgetPoolTypeByOwnerChar(OwnerCharacter);
		auto* FloatingWidget = MasterUIManager->RentFloatingWidget(PoolType, CachedPlayerController, OwnerCharacter);

		if (auto* DamageWidget = Cast<UB2FloatingDamageNum>(FloatingWidget))
		{
			// 원래 DamageNumWidget 하나만 생성해서 매 TakeDamage 이벤트마다 새로 파라미터 세팅하던 이전 코드 잔재인데 놔둬서 안될 것도 없어 보여서 그대로 씀.
			// -> 놔뒀길래 다행.. 결국 FloatWidget Pooling 을 하면서 이전 방식이 필요해짐
			DamageWidget->NotifyTakeDamage(Damage, bInCritical);

			if (bCurrentlyHideAll) // 도중에 생성한다면 숨기는 걸 까먹어선 안되겠다.
				DamageWidget->ForceHide();

			else
				DamageWidget->RestoreFromForceHidden();

			FDynamicFloatingWidgetSet NewWidgetSet;
			NewWidgetSet.FloatingWidget = DamageWidget;

			// VisibleDuration 으로 작동하는 해제 타이머 설치
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
				// 이름이 옛날 흔적이라 Destroy 인데 실제로 이제는 Destroy 가 아니라 pool 에 반납한다.

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
		// 이제 생성이 아니라 Rent 이다. 생성이 될 수도 있긴 하지.
		UB2FloatingAbnormalEffect* NewAbnormalEffectWidget = Cast<UB2FloatingAbnormalEffect>(MasterUIManager->RentFloatingWidget(
			EFloatingWidgetPoolClassType::EFWPCT_AbnormalEffect, CachedPlayerController, OwnerCharacter
		));

		if (NewAbnormalEffectWidget)
		{
			NewAbnormalEffectWidget->NotifyAbnormalEffect(InEffectSetupInfo);

			if (bCurrentlyHideAll) // 도중에 생성한다면 숨기는 걸 까먹어선 안되겠다.
			{
				NewAbnormalEffectWidget->ForceHide();
			}
			else
			{
				NewAbnormalEffectWidget->RestoreFromForceHidden();
			}

			FDynamicFloatingWidgetSet NewWidgetSet;
			NewWidgetSet.FloatingWidget = NewAbnormalEffectWidget;

			// VisibleDuration 으로 작동하는 해제 타이머 설치
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
				// 이름이 옛날 흔적이라 Destroy 인데 실제로 이제는 Destroy 가 아니라 pool 에 반납한다.
				MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_AbnormalEffect, WidgetToDestroy);
				OwnerGameMode->GetWorldTimerManager().ClearTimer(AbnormalEffectWidgetSetArray[AWI].WidgetDestroyTimerHandle);
				AbnormalEffectWidgetSetArray.RemoveAt(AWI);
				break;
			}
		}
	}
}
