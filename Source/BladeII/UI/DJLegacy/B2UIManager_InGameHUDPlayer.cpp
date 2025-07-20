// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIManager_InGameHUDPlayer.h"
//#include "BladeII.h"

#include "BladeIINetPlayer.h"
#include "BladeIIPlayerController.h"
#include "B2FloatingGoldAcq.h"
#include "B2FloatingDamageNum.h"
#include "BladeIIGameMode.h"
#include "B2StageGameMode.h"
#include "B2StageManager.h"
#include "B2FloatingHPBar.h"
#include "B2ControlGameMode.h"
#include "B2PVPGameMode.h"
#include "B2RaidGameMode.h"
#include "B2UIManager_InGameCombat.h"
#include "BladeIIUtil.h"

UB2UIManager_InGameHUDPlayer::UB2UIManager_InGameHUDPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GConfig->GetFloat(TEXT("/Script/BladeII.B2HiddenMobPointer"), TEXT("RadiusFromPlayer"), HMP_RadiusFromPlayer, GGameIni);
	CurrentHMPTarget = NULL;
	HiddenMobPointerOnlyVisiblityFlag = true;
}

bool UB2UIManager_InGameHUDPlayer::InitUIManager(class ABladeIICharacter* InOwner, AB2UIManager_InGameCombat* InMasterManager)
{
	if(Super::InitUIManager(InOwner, InMasterManager) == false)
	{
		return false;
	}

	OwnerPlayer = Cast<ABladeIIPlayer>(OwnerCharacter);
	if (OwnerPlayer == NULL)
	{
		return false;
	}
	
	// 捞扒 颇萍努矫胶袍 扁馆牢单, 咯窍埃 Widget 苞 鞍捞 秒鞭. 捞傈浚 UserWidget 栏肺 备泅登绢 乐菌澜.
	if (OwnerPlayer->HiddenMobPointerPSTemplate != NULL)
	{
		HiddenMobPointerPSInst = UGameplayStatics::SpawnEmitterAttached(OwnerPlayer->HiddenMobPointerPSTemplate, OwnerPlayer->GetRootComponent());
	}

	// Static 窍霸 眠啊肺 积己且 霸 乐栏哥 咯扁辑.

	// 贸澜何磐 Cinematic mode 牢 版快啊 乐绰单 UpdateManual 俊辑 某摹窍瘤 给窍骨肺 茄锅 眉农
	if (bCachedHUDHidingCinematicMode)
	{
		HideAll();
	}
#if !UE_BUILD_SHIPPING
	extern bool gbHACKInGameUIHiddenByDevCommand;
	if (gbHACKInGameUIHiddenByDevCommand) // 俺惯侩 目盖靛肺 见变 版快俊 措秦
	{
		HideAll();
	}
#endif

	return true;
}

void UB2UIManager_InGameHUDPlayer::UpdateManual(float DeltaSeconds)
{
	Super::UpdateManual(DeltaSeconds);
	
	for (int32 GAI = 0; GAI < GoldAcqWidgetSetArray.Num(); ++GAI)
	{
		UB2FloatingWidget* GoldAcqWidget = GoldAcqWidgetSetArray[GAI].FloatingWidget;
		if (GoldAcqWidget)
		{
			GoldAcqWidget->UpdatePosition();
		}
	}

	if (HiddenMobPointerPSInst)
	{
		auto CurrentGameMode = GetB2GameModeType(UGameplayStatics::GetGameMode(OwnerPlayer));
		if (CurrentGameMode == EB2GameMode::Control)
			UpdateConquestAreaPointerForControl();
		else if(CurrentGameMode == EB2GameMode::PVP_Tag)
			UpdateConquestAreaPointerForPvP();
		else
			UpdateHiddenMobPointer();
	}

}

void UB2UIManager_InGameHUDPlayer::DestroyPlayerWidgets()
{
	// DestroyUserWidgets 篮 酒公巴档 救窍绊 捞扒 促弗 炼扒俊辑 阂府霸 瞪 巴.
	if (FloatingHPBarWidget)
	{
		FloatingHPBarWidget->DestroySelf();
		FloatingHPBarWidget = NULL;
	}

	for (int32 GAI = 0; GAI < GoldAcqWidgetSetArray.Num(); ++GAI)
	{
		UB2FloatingWidget* GoldAcqWidget = GoldAcqWidgetSetArray[GAI].FloatingWidget;
		if (GoldAcqWidget && MasterUIManager)
		{	// 馆吵
			MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_GoldAcq, GoldAcqWidget);
		}
	}
	GoldAcqWidgetSetArray.Empty();

	for (int32 DWI = 0; DWI < DamageNumWidgetSetArray.Num(); ++DWI)
	{
		UB2FloatingWidget* DamageNumWidget = DamageNumWidgetSetArray[DWI].FloatingWidget;
		if (DamageNumWidget && MasterUIManager)
		{	// 馆吵
			MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_DamageNum_PC, DamageNumWidget);
			OwnerCharacter->GetWorldTimerManager().ClearTimer(DamageNumWidgetSetArray[DWI].WidgetDestroyTimerHandle);
		}
	}
	DamageNumWidgetSetArray.Empty();

	if (HiddenMobPointerPSInst)
	{
		HiddenMobPointerPSInst->ConditionalBeginDestroy();
		HiddenMobPointerPSInst = NULL;
	}
}

void UB2UIManager_InGameHUDPlayer::HideAll()
{
	Super::HideAll();

	for (int32 GAI = 0; GAI < GoldAcqWidgetSetArray.Num(); ++GAI)
	{
		UB2FloatingWidget* GoldAcqWidget = GoldAcqWidgetSetArray[GAI].FloatingWidget;
		if (GoldAcqWidget)
		{
			GoldAcqWidget->ForceHide();
		}
	}
	if (HiddenMobPointerPSInst)
	{
		HiddenMobPointerPSInst->SetVisibility(false);
	}

	CachedPlayerController->SetVirtualJoystickVisibility(false); // VirtualJoystick 篮 促弗 widget 苞绰 混娄 促抚.
}

void UB2UIManager_InGameHUDPlayer::RestoreVisibilityAll()
{
	Super::RestoreVisibilityAll();

	for (int32 GAI = 0; GAI < GoldAcqWidgetSetArray.Num(); ++GAI)
	{
		UB2FloatingWidget* GoldAcqWidget = GoldAcqWidgetSetArray[GAI].FloatingWidget;
		if (GoldAcqWidget)
		{
			GoldAcqWidget->RestoreFromForceHidden();
		}
	}
	if (HiddenMobPointerPSInst)
	{
		HiddenMobPointerPSInst->SetVisibility(true); // 老窜 难 霖促. 绢瞒乔 bCurrentlyHideAll 捞 酒聪搁 概平 八荤窍搁辑 visibility 技泼窍霸 瞪 巴.
	}

	CachedPlayerController->SetVirtualJoystickVisibility(!HiddenVirtualJoystick);
}

void UB2UIManager_InGameHUDPlayer::HideMobPointer(bool bInHide)
{
	HiddenMobPointerOnlyVisiblityFlag = !bInHide;

	if (HiddenMobPointerPSInst)
	{
		HiddenMobPointerPSInst->SetVisibility(HiddenMobPointerOnlyVisiblityFlag); // 绢瞒乔 bCurrentlyHideAll 捞 酒聪搁 概平 八荤窍搁辑 visibility 技泼窍霸 瞪 巴.
	}
}

void UB2UIManager_InGameHUDPlayer::NotifyBeginPlay()
{
	Super::NotifyBeginPlay();

}
void UB2UIManager_InGameHUDPlayer::NotifyStageBegin()
{

}

void UB2UIManager_InGameHUDPlayer::NotifyTargetDamaged(int32 SuccessiveComboNum, float DamageAmount)
{

}

void UB2UIManager_InGameHUDPlayer::NotifyAutoStateChange(bool bInAutoNow)
{

}
void UB2UIManager_InGameHUDPlayer::NotifyPlayerTagged(class ABladeIIPlayer* OldPlayer, class ABladeIIPlayer* NewPlayer)
{
}

void UB2UIManager_InGameHUDPlayer::NotifyPlayerSuckedGold(int32 AcquiredAmount)
{
	// Create a new one for floating gold acquire notification text
	CreateSingleGoldAcqWidget(AcquiredAmount);

}

UB2FloatingGoldAcq* UB2UIManager_InGameHUDPlayer::CreateSingleGoldAcqWidget(int32 AcquiredAmount)
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
		UB2FloatingGoldAcq* NewGoldAcqWidget = Cast<UB2FloatingGoldAcq>(MasterUIManager->RentFloatingWidget(
			EFloatingWidgetPoolClassType::EFWPCT_GoldAcq, CachedPlayerController, OwnerCharacter
		));
		if (NewGoldAcqWidget)
		{			
			// 盔贰 GoldAcqWidget 窍唱父 积己秦辑 概 SuckedGold 捞亥飘付促 货肺 颇扼固磐 技泼窍带 捞傈 内靛 儡犁牢单 出抵辑 救瞪 巴档 绝绢 焊咯辑 弊措肺 靖.
			// -> 出底辨贰 促青.. 搬惫 FloatWidget Pooling 阑 窍搁辑 捞傈 规侥捞 鞘夸秦咙
			NewGoldAcqWidget->NotifyPlayerSuckedGold(AcquiredAmount);

			if (bCurrentlyHideAll) // 档吝俊 积己茄促搁 见扁绰 吧 鳖冈绢急 救登摆促.
			{
				NewGoldAcqWidget->ForceHide();
			}

			FDynamicFloatingWidgetSet NewWidgetSet;
			NewWidgetSet.FloatingWidget = NewGoldAcqWidget;

			// VisibleDuration 栏肺 累悼窍绰 秦力 鸥捞赣 汲摹
			OwnerPlayer->GetWorldTimerManager().SetTimer(NewWidgetSet.WidgetDestroyTimerHandle,
				FTimerDelegate::CreateUObject(this, &UB2UIManager_InGameHUDPlayer::DestroySingleGoldAcqWidgetTimerCB, NewGoldAcqWidget),
				NewGoldAcqWidget->GetVisibleDuration(), false);

			GoldAcqWidgetSetArray.Add(NewWidgetSet);

			return NewGoldAcqWidget;
		}
	}

	return NULL;
}

void UB2UIManager_InGameHUDPlayer::DestroySingleGoldAcqWidgetTimerCB(class UB2FloatingGoldAcq* WidgetToDestroy)
{
	if (WidgetToDestroy && MasterUIManager)
	{
		for (int32 GAI = 0; GAI < GoldAcqWidgetSetArray.Num(); ++GAI)
		{
			UB2FloatingWidget* CurrDW = GoldAcqWidgetSetArray[GAI].FloatingWidget;
			if (CurrDW == WidgetToDestroy)
			{
				// 捞抚捞 烤朝 如利捞扼 Destroy 牢单 角力肺 捞力绰 Destroy 啊 酒聪扼 pool 俊 馆吵茄促.
				MasterUIManager->TurnInFloatingWidget(EFloatingWidgetPoolClassType::EFWPCT_GoldAcq, WidgetToDestroy);
				OwnerCharacter->GetWorldTimerManager().ClearTimer(GoldAcqWidgetSetArray[GAI].WidgetDestroyTimerHandle);
				GoldAcqWidgetSetArray.RemoveAt(GAI);
				break;
			}
		}
	}
}

void UB2UIManager_InGameHUDPlayer::SetCenterMessage(FText& InMessageText, float DisplayTime, int32 AnimIndex /*= -1*/)
{

}

void UB2UIManager_InGameHUDPlayer::HideCenterMessage()
{

}

void UB2UIManager_InGameHUDPlayer::HideCenterMessageTimerCB()
{
	if (OwnerPlayer)
	{
		OwnerPlayer->GetWorldTimerManager().ClearTimer(CenterMessageHideTimerHandle);
	}
	HideCenterMessage();
}

void UB2UIManager_InGameHUDPlayer::HideVirtualJoystick(bool bInHide)
{
	HiddenVirtualJoystick = bInHide;
	CachedPlayerController->SetVirtualJoystickVisibility(!HiddenVirtualJoystick);
}

void UB2UIManager_InGameHUDPlayer::UpdateHiddenMobPointer()
{
	if (bCurrentlyHideAll || !HiddenMobPointerOnlyVisiblityFlag)
	{
		HiddenMobPointerPSInst->SetVisibility(false);
		return;
	}

	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(OwnerPlayer->Controller);

	if (B2PC)
	{
		int32 ScreenSizeX = 0;
		int32 ScreenSizeY = 0;
		B2PC->GetViewportSize(ScreenSizeX, ScreenSizeY);

#if 0 // 历 钢府 乐绰 target 栏肺 牢秦 利例洒 盎脚登瘤 臼绰 盲肺 乐绰 版快啊 乐绢辑 傲 概 橇饭烙 货肺 茫绊 措脚 苞窍霸 FActorIterator 甫 倒瘤绰 臼档废..
		bool bFindNewHMPTarget = false;
		// CurrentHMPTarget 阑 蜡瘤且瘤 八荤. 概锅 CurrentHMPTarget 阑 盎脚窍扁绰 厚轿促..
		//if (CurrentHMPTarget == NULL || CurrentHMPTarget->IsPendingKill() || CurrentHMPTarget->IsAlive() == false)
		{
			bFindNewHMPTarget = true;
		}
		else
		{
			FVector2D CurrentTargetScreenLocation(0.0f, 0.0f);
			if (B2PC->ProjectWorldLocationToScreen(CurrentHMPTarget->GetActorLocation(), CurrentTargetScreenLocation))
			{
				if (CurrentTargetScreenLocation.X >= 0.0f && CurrentTargetScreenLocation.X <= (float)ScreenSizeX &&
					CurrentTargetScreenLocation.Y >= 0.0f && CurrentTargetScreenLocation.Y <= (float)ScreenSizeY)
				{
					bFindNewHMPTarget = true; // 拳搁 救栏肺 甸绢吭栏聪 货肺 茫绰促.
				}
			}
			else
			{
				bFindNewHMPTarget = true;
			}
		}

		if (bFindNewHMPTarget)
#endif
		{
			CurrentHMPTarget = NULL; // 老窜 NULL. 给茫栏搁 救 谎府绰 芭.

			float ClosestOutScreenDistSquared2D = MAX_FLT; // 拳搁 观俊 乐绰 各 吝 啊厘 啊鳖款 巴狼 芭府 力蚌.

														   // FActorIterator 甫 倒扁绰 厚轿聪 SpawnPool 俊辑 spawn 等 局甸 府胶飘父 眠妨辑 八荤.
			TArray<class ABladeIICharacter*> AllWaveMobList;
			ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(OwnerPlayer));
			if (B2GM)
			{
				B2GM->GetAllWorldMobList(AllWaveMobList);
			}
			for (int32 MI = 0; MI < AllWaveMobList.Num(); ++MI)
			{
				ABladeIICharacter* B2Char = AllWaveMobList[MI];
				if (B2Char)
				{
					FVector2D CandidScreenLocation(0.0f, 0.0f);
					if (B2PC->ProjectWorldLocationToScreen(B2Char->GetActorLocation(), CandidScreenLocation))
					{
						if (CandidScreenLocation.X < 0.0f || CandidScreenLocation.X >(float)ScreenSizeX || CandidScreenLocation.Y < 0.0f || CandidScreenLocation.Y >(float)ScreenSizeY)
						{
							// 拳搁 观俊 乐澜. XY 乞搁俊辑 啊厘 啊鳖款 吧肺 急琶.

							float CurrCandidDistSquared2D = (B2Char->GetActorLocation() - OwnerPlayer->GetActorLocation()).SizeSquared2D();

							if (CurrCandidDistSquared2D < ClosestOutScreenDistSquared2D)
							{
								CurrentHMPTarget = B2Char;
								ClosestOutScreenDistSquared2D = CurrCandidDistSquared2D;
							}
						}
						else
						{
							CurrentHMPTarget = NULL; // 窜 窍唱扼档 矫具俊 甸绢柯 巴捞 乐促搁 谎府瘤 臼绰促.
							break;
						}
					}
				}
			}
		}
		VisiblePointer();
	}
}

void UB2UIManager_InGameHUDPlayer::UpdateConquestAreaPointerForPvP()
{
	if (auto* pGM = Cast<AB2PVPGameMode>(UGameplayStatics::GetGameMode(OwnerPlayer)))
	{
		auto* B2Char = pGM->GetOpponent();
		auto* B2PC = pGM->GetLocalController();

		CurrentHMPTarget = NULL;

		if (B2Char && B2PC)
		{
			FVector2D CandidScreenLocation(0.0f, 0.0f);

			if (B2PC->ProjectWorldLocationToScreen(B2Char->GetActorLocation(), CandidScreenLocation))
			{
				int32 ScreenSizeX = 0;
				int32 ScreenSizeY = 0;

				B2PC->GetViewportSize(ScreenSizeX, ScreenSizeY);

				if (CandidScreenLocation.X < 0.0f || CandidScreenLocation.X >(float)ScreenSizeX || CandidScreenLocation.Y < 0.0f || CandidScreenLocation.Y >(float)ScreenSizeY)
				{
					CurrentHMPTarget = B2Char;
				}
			}
		}

		VisiblePointer();
	}
}

void UB2UIManager_InGameHUDPlayer::VisiblePointer()
{
	if (CurrentHMPTarget)
	{
		FVector PlayerToTargetVector = (CurrentHMPTarget->GetActorLocation() - OwnerPlayer->GetActorLocation());
		PlayerToTargetVector.Z = 0.0f; // Z 蔼篮 蝶肺 拌魂
		FVector PlayerToHMPVector = PlayerToTargetVector.GetSafeNormal() * HMP_RadiusFromPlayer;

		// Z 蔼篮 敲饭捞绢 某腐磐 惯官蹿 何辟栏肺. 距埃 棵赴促.
		FVector HMPLocation = OwnerPlayer->GetActorLocation() + PlayerToHMPVector +
			FVector(0.0f, 0.0f, -1.0f * OwnerPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 1.0f);

		HiddenMobPointerPSInst->SetWorldLocation(HMPLocation);
		HiddenMobPointerPSInst->SetWorldRotation(PlayerToTargetVector.Rotation());
		HiddenMobPointerPSInst->SetVisibility(true);
	}
	else
	{
		HiddenMobPointerPSInst->SetVisibility(false);
	}
}

void UB2UIManager_InGameHUDPlayer::UpdateConquestAreaPointerForControl()
{
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(OwnerPlayer));

	if (pGM)
	{
		FVector PlayerToTargetVector = (pGM->GetControlObjectLocation() - OwnerPlayer->GetActorLocation());
		
		PlayerToTargetVector.Z = 0.0f; // Z 蔼篮 蝶肺 拌魂

		// 老沥芭府 捞窍搁 救钎矫
		if (PlayerToTargetVector.Size() <= pGM->GetControlObjectViewLimitLength())
		{
			HiddenMobPointerPSInst->SetVisibility(false);
			return;
		}

		FVector PlayerToHMPVector = PlayerToTargetVector.GetSafeNormal() * HMP_RadiusFromPlayer;

		// Z 蔼篮 敲饭捞绢 某腐磐 惯官蹿 何辟栏肺. 距埃 棵赴促.
		FVector HMPLocation = OwnerPlayer->GetActorLocation() + PlayerToHMPVector +
			FVector(0.0f, 0.0f, -1.0f * OwnerPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 1.0f);

		HiddenMobPointerPSInst->SetWorldLocation(HMPLocation);
		HiddenMobPointerPSInst->SetWorldRotation(PlayerToTargetVector.Rotation());
		HiddenMobPointerPSInst->SetVisibility(true);
	}
}
