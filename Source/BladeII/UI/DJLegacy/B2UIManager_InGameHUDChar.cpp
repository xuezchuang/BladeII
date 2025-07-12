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
// ĳ���� ������ �ʿ��� floating widget �� ���� (e.g. HP ��)
// �Ƹ��� ��� ���� ���� DJLegacy UI �Ϻ�
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
//	// FloatingHP �ٴ� ������ ���� �Ұ�
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
//	// ���Ӹ�庰 �߰�UI
//	AdditionalUIForGameMode();
//
//	// ó������ Cinematic mode �� ��찡 �ִµ� UpdateManual ���� ĳġ���� ���ϹǷ� �ѹ� üũ
//	if (bCachedHUDHidingCinematicMode)
//	{
//		HideAll();
//	}
//#if !UE_BUILD_SHIPPING
//	extern bool gbHACKInGameUIHiddenByDevCommand;
//	if (gbHACKInGameUIHiddenByDevCommand) // ���߿� Ŀ�ǵ�� ���� ��쿡 ����
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
//	// �ó׸�ƽ���� UI ����.
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
//#if !UE_BUILD_SHIPPING // InGameUI ġƮ�� ��ٸ� �ٽ� ����� ��
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
		{ // �ݳ�
			MasterUIManager->TurnInFloatingWidget(MasterUIManager->GetDamageNumWidgetPoolTypeByOwnerChar(OwnerCharacter), DamageNumWidget);
			OwnerGameMode->GetWorldTimerManager().ClearTimer(DamageNumWidgetSetArray[DWI].WidgetDestroyTimerHandle);
		}
	}
	DamageNumWidgetSetArray.Empty();

	for (int32 AWI = 0; AWI < AbnormalEffectWidgetSetArray.Num(); ++AWI)
	{
		UB2FloatingWidget* ThisWidget = AbnormalEffectWidgetSetArray[AWI].FloatingWidget;
		if (ThisWidget && MasterUIManager)
		{ // �ݳ�
			MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_AbnormalEffect, ThisWidget);
			OwnerGameMode->GetWorldTimerManager().ClearTimer(AbnormalEffectWidgetSetArray[AWI].WidgetDestroyTimerHandle);
		}
	}
	AbnormalEffectWidgetSetArray.Empty();

	// �߰��Ǵ� �͵� �־���
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

	// �߰��Ǵ� �͵� �־���
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

	// �߰��Ǵ� �͵� �־���
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
	//	// ������ ���Ӹ�忡 �����Ȱɷ� ����
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
	// �ϴ� ������ ���� �÷��̾� �ʿ�..
}

void UB2UIManager_InGameHUDChar::NotifyTakeDamage(float Damage, bool bInCritical)
{

	// Changed Damage Number to spawn as effect, in TakeDamage directly

	// TakeDamage �̺�Ʈ���� DamageNumWidget ����
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
		// ���� ������ �ƴ϶� Rent �̴�. ������ �� ���� �ֱ� ����.

		auto PoolType = MasterUIManager->GetDamageNumWidgetPoolTypeByOwnerChar(OwnerCharacter);
		auto* FloatingWidget = MasterUIManager->RentFloatingWidget(PoolType, CachedPlayerController, OwnerCharacter);

		if (auto* DamageWidget = Cast<UB2FloatingDamageNum>(FloatingWidget))
		{
			// ���� DamageNumWidget �ϳ��� �����ؼ� �� TakeDamage �̺�Ʈ���� ���� �Ķ���� �����ϴ� ���� �ڵ� �����ε� ���ּ� �ȵ� �͵� ���� ������ �״�� ��.
			// -> ���ױ淡 ����.. �ᱹ FloatWidget Pooling �� �ϸ鼭 ���� ����� �ʿ�����
			DamageWidget->NotifyTakeDamage(Damage, bInCritical);

			if (bCurrentlyHideAll) // ���߿� �����Ѵٸ� ����� �� ��Ծ �ȵǰڴ�.
				DamageWidget->ForceHide();

			else
				DamageWidget->RestoreFromForceHidden();

			FDynamicFloatingWidgetSet NewWidgetSet;
			NewWidgetSet.FloatingWidget = DamageWidget;

			// VisibleDuration ���� �۵��ϴ� ���� Ÿ�̸� ��ġ
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
				// �̸��� ���� �����̶� Destroy �ε� ������ ������ Destroy �� �ƴ϶� pool �� �ݳ��Ѵ�.

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
		// ���� ������ �ƴ϶� Rent �̴�. ������ �� ���� �ֱ� ����.
		UB2FloatingAbnormalEffect* NewAbnormalEffectWidget = Cast<UB2FloatingAbnormalEffect>(MasterUIManager->RentFloatingWidget(
			EFloatingWidgetPoolClassType::EFWPCT_AbnormalEffect, CachedPlayerController, OwnerCharacter
		));

		if (NewAbnormalEffectWidget)
		{
			NewAbnormalEffectWidget->NotifyAbnormalEffect(InEffectSetupInfo);

			if (bCurrentlyHideAll) // ���߿� �����Ѵٸ� ����� �� ��Ծ �ȵǰڴ�.
			{
				NewAbnormalEffectWidget->ForceHide();
			}
			else
			{
				NewAbnormalEffectWidget->RestoreFromForceHidden();
			}

			FDynamicFloatingWidgetSet NewWidgetSet;
			NewWidgetSet.FloatingWidget = NewAbnormalEffectWidget;

			// VisibleDuration ���� �۵��ϴ� ���� Ÿ�̸� ��ġ
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
				// �̸��� ���� �����̶� Destroy �ε� ������ ������ Destroy �� �ƴ϶� pool �� �ݳ��Ѵ�.
				MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_AbnormalEffect, WidgetToDestroy);
				OwnerGameMode->GetWorldTimerManager().ClearTimer(AbnormalEffectWidgetSetArray[AWI].WidgetDestroyTimerHandle);
				AbnormalEffectWidgetSetArray.RemoveAt(AWI);
				break;
			}
		}
	}
}
