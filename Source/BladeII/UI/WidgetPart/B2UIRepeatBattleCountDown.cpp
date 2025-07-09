#include "BladeII.h"
#include "B2UIRepeatBattleCountDown.h"
#include "B2UIManager.h"
#include "B2CommonSoundInfo.h"
#include "BladeIIGameMode.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2AndroidBackManager.h"

UB2UIRepeatBattleCountDown::UB2UIRepeatBattleCountDown(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsPauseAble = false;
	bIsCounting = false;
	bIsFinishCount = false;
	CountDownNumber = 0;
	IsBackPopup = false;
}

void UB2UIRepeatBattleCountDown::Init()
{
	Super::Init();
	StopCountdown();
}

void UB2UIRepeatBattleCountDown::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(Btn_Pause, &UB2UIRepeatBattleCountDown::OnClickBTN_Pause);
}

void UB2UIRepeatBattleCountDown::DestroySelf(class UB2UIManager* InUIManager)
{
	if (IsBackPopup)
	{
		IsBackPopup = false;
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIRepeatBattleCountDown::CacheAssets()
{
	GET_SLOT(UB2Button, Btn_Pause);
	GET_SLOT(UTextBlock, TB_RepeatBattleOnState);
	GET_SLOT(UTextBlock, TB_RepeatBattleCountDownNumber);
	GET_SLOT(UPanelWidget, CP_RepeatBattleInfoSet);
	GET_SLOT(UImage, IMG_BG);
}

void UB2UIRepeatBattleCountDown::UpdateStaticText()
{
	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		if (TB_RepeatBattleOnState.IsValid())
		{
			if (DocBS->GetRepeatBattleLoopAll())
			{
				TB_RepeatBattleOnState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("B2UIRepeatBattleCountDown_002")));
			}
			else if (DocBS->GetRepeatBattleLoopOne())
			{
				TB_RepeatBattleOnState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("B2UIRepeatBattleCountDown_001")));
			}
		}
	}
}

void UB2UIRepeatBattleCountDown::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Tick 에서 직접 카운트 한다. Pause 상태에서도 사용하기 위해.
	CountdownTick(InDeltaTime);
}

void UB2UIRepeatBattleCountDown::SetNotiText(FText NotiText)
{
	if (TB_RepeatBattleOnState.IsValid())
	{
		TB_RepeatBattleOnState->SetText(NotiText);
	}
}

void UB2UIRepeatBattleCountDown::StartCountdown(FRepeatBattleCountDownCB InAllCountedCB, float WholeCountTime)
{
	OnAllCountedCB = InAllCountedCB;

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{		
		CountDownNumber = WholeCountTime;

		if (TB_RepeatBattleCountDownNumber.IsValid())
		{ // UI 표시 넘버 업데이트
			TB_RepeatBattleCountDownNumber->SetText(FText::AsNumber(CountDownNumber));
		}

		OnStartCountdownBP();

		bIsCounting = true;

		if (IsBackPopup == false)
		{
			IsBackPopup = true;
			B2AndroidBackManager::GetInstance()->AddBackWidget(this);
		}
	}
}

void UB2UIRepeatBattleCountDown::BattleCountDownSetVisibility(ESlateVisibility InVisibility)
{
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // NativeTick 을 사용하려면 전체를 Hidden 시키면 안된다.

	if (CP_RepeatBattleInfoSet.IsValid())
		CP_RepeatBattleInfoSet->SetVisibility(InVisibility);
	if (IMG_BG.IsValid())
		IMG_BG->SetVisibility(InVisibility);

	if (Btn_Pause.IsValid())
		Btn_Pause->SetVisibility(ESlateVisibility::HitTestInvisible);

	bool IsVisible = InVisibility == ESlateVisibility::Collapsed || InVisibility == ESlateVisibility::Hidden;
	if (IsBackPopup == true && IsVisible)
	{
		IsBackPopup = false;
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
}

void UB2UIRepeatBattleCountDown::OnClickBTN_Pause()
{
	if (bIsPauseAble == false)
		return;
	
	PauseCountdown();

	auto BattleDoc = UB2UIDocHelper::GetDocBattleStage();

	if (!BattleDoc)
		return;

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, BattleDoc->GetRepeatBattleLoopAll() ? TEXT("BattleStageInfo_ContBattleQuit") :
			TEXT("BattleStageInfo_AskForRepeatBattleQuit")), 
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]() {
		CancelOrStopRepeatBattleClass<>::GetInstance().Signal();
		StopCountdown();
	}),
		FMsgPopupOnClick::CreateLambda([this]() {
		ResumeCountdown();
	})
		);
}

void UB2UIRepeatBattleCountDown::CountdownTick(float InDeltaTime)
{
	if (bIsCounting == false)
		return;

	if (bIsFinishCount)
		return;
	
	UWorld* TheWorld = GetWorld();

	if (TheWorld)
	{
		int32 NextTime = CountDownNumber - InDeltaTime;

		if (NextTime == static_cast<int32>(CountDownNumber))
		{
			CountDownNumber -= InDeltaTime;
			return;
		}

		CountDownNumber -= InDeltaTime;
		if (CountDownNumber <= 0)
		{
			// 카운터 다 셌음.
			if (auto* B2GameMode = Cast<ABladeIIGameMode>(TheWorld->GetAuthGameMode()))
				B2GameMode->PlayAMBByCommonSoundID(ECommonSoundID::ECSID_AMB_BattleCountDown_Last);

			bIsCounting = false;
			bIsFinishCount = true;
			OnAllCountedCB.ExecuteIfBound();
		}
		else
		{
			if (auto* B2GameMode = Cast<ABladeIIGameMode>(TheWorld->GetAuthGameMode()))
				B2GameMode->PlayAMBByCommonSoundID(ECommonSoundID::ECSID_AMB_BattleCountDown);

			if (TB_RepeatBattleCountDownNumber.IsValid())
			{ // UI 표시 넘버 업데이트
				TB_RepeatBattleCountDownNumber->SetText(FText::AsNumber(FMath::CeilToFloat(CountDownNumber)));
			}
		}
	}
}

void UB2UIRepeatBattleCountDown::StopCountdown()
{
	CountDownNumber = 0;
	bIsCounting = false;
	bIsFinishCount = false;
	SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIRepeatBattleCountDown::PauseCountdown()
{
	if (bIsFinishCount)
		return;

	bIsCounting = false;
}

void UB2UIRepeatBattleCountDown::ResumeCountdown()
{
	if (bIsFinishCount)
		return;

	bIsCounting = true;
}

void UB2UIRepeatBattleCountDown::SetPauseAble(bool IsEnable)
{
	bIsPauseAble = IsEnable;
}

void PopUpRepeatBattleStopMsg(class APlayerController* InPC)
{
	if (!InPC) {
		return;
	}

	UGameplayStatics::SetGamePaused(InPC, true); // 카운트다운을 하는 상황이 종종 있으므로 일시정지.

	auto BattleDoc = UB2UIDocHelper::GetDocBattleStage();

	if (!BattleDoc)
		return;

	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, BattleDoc->GetRepeatBattleLoopAll() ? TEXT("BattleStageInfo_ContBattleQuit") :
				TEXT("BattleStageInfo_AskForRepeatBattleQuit")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([InPC]() {
			CancelOrStopRepeatBattleClass<>::GetInstance().Signal();
			UGameplayStatics::SetGamePaused(InPC, false);
		}),
			FMsgPopupOnClick::CreateLambda([InPC]() {
			UGameplayStatics::SetGamePaused(InPC, false);
		})
			);
	}
}