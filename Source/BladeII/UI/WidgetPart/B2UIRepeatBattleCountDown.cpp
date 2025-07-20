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

	// Tick 俊辑 流立 墨款飘 茄促. Pause 惑怕俊辑档 荤侩窍扁 困秦.
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
		{ // UI 钎矫 逞滚 诀单捞飘
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
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // NativeTick 阑 荤侩窍妨搁 傈眉甫 Hidden 矫虐搁 救等促.

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
			// 墨款磐 促 济澜.
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
			{ // UI 钎矫 逞滚 诀单捞飘
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

	UGameplayStatics::SetGamePaused(InPC, true); // 墨款飘促款阑 窍绰 惑炔捞 辆辆 乐栏骨肺 老矫沥瘤.

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