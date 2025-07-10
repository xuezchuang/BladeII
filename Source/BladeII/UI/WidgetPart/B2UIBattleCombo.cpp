#include "B2UIBattleCombo.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
#include "Styling/SlateStyle.h"
#include "B2UIDocPVP1on1Rival.h"
#include "B2UIBattleMain.h"

UB2UIBattleCombo::UB2UIBattleCombo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ComboPraiseTextSwitchInterval = 10;
	ComboPraiseTextMinLevel = 10;
	ComboPraiseTextMaxLevel = 60;
	ComboPraiseTextShowDuration = 3.0f;
	bComboPraiseTextVisibility = false;
	bIsCounterGameMode = false;
}


void UB2UIBattleCombo::CacheAssets()
{
	GET_SLOT(UImage,			IMG_ComboNum)
	GET_SLOT(UWidgetSwitcher,	WS_ComboPraiseTextImage)
	GET_SLOT(UTextBlock,		TB_AddCriticalRateByCombo)

	if (IMG_ComboNum.IsValid())
		ComboNumDisplayMID = IMG_ComboNum->GetDynamicMaterial();

	bIsCounterGameMode = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::CounterDGBattleMain) != nullptr;
}

void UB2UIBattleCombo::UpdateStaticText()
{
}

void UB2UIBattleCombo::BindDelegates()
{
	//BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UIBattleCombo::OnClickBtnArea)
}

void UB2UIBattleCombo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//UpdateComboNumMaterial();
}

void UB2UIBattleCombo::BindDoc(UB2UIDocBattle* BattleDoc)
{
	UnbindDoc();
	SetDoc(BattleDoc);
	if (!BattleDoc) return;

	SetCombo(BattleDoc->GetComboNum());


	BattleDoc->OnComboNumChanged.AddUObject(this, &UB2UIBattleCombo::OnChangedComboNum);
	BattleDoc->OnComboExpireProgressChanged.AddUObject(this, &UB2UIBattleCombo::OnChangedComboExpireProgress);
	BattleDoc->OnCriticalRateByComboNumChanged.AddUObject(this, &UB2UIBattleCombo::OnChangedCriticalRateByComboNum); // OnChangedComboNum 이랑 사실상 마찬가지인데 그거랑 같이 쓰려면 UI Doc 업데이트 순서를 신경써줘야 해서 따로 감

	BattleDoc->OnIncCounterDamageComboChanged.AddUObject(this, &UB2UIBattleCombo::OnChangedIncCounterDamageCombo);
}

void UB2UIBattleCombo::BindDoc(UB2UIDocPVP1on1Rival* BattleDoc)
{
	UnbindDoc();
	SetDoc(BattleDoc);
	if (!BattleDoc) return;

	SetCombo(BattleDoc->GetComboNum());
	
	BattleDoc->OnComboNumChanged.AddUObject(this, &UB2UIBattleCombo::OnChangedComboNum);
	BattleDoc->OnComboExpireProgressChanged.AddUObject(this, &UB2UIBattleCombo::OnChangedComboExpireProgress);
	BattleDoc->OnCriticalRateByComboNumChanged.AddUObject(this, &UB2UIBattleCombo::OnChangedCriticalRateByComboNum); // OnChangedComboNum 이랑 사실상 마찬가지인데 그거랑 같이 쓰려면 UI Doc 업데이트 순서를 신경써줘야 해서 따로 감
}


void UB2UIBattleCombo::UnbindDoc()
{
	auto DocBattle = Cast<UB2UIDocBattle>(GetDoc());
	if (DocBattle)
	{
		DocBattle->OnComboNumChanged.RemoveAll(this);
		DocBattle->OnComboExpireProgressChanged.RemoveAll(this);
	}
	else
	{
		auto DocBattleRival = Cast<UB2UIDocPVP1on1Rival>(GetDoc());
		if (DocBattleRival)
		{
			DocBattleRival->OnComboNumChanged.RemoveAll(this);
			DocBattleRival->OnComboExpireProgressChanged.RemoveAll(this);
		}
	}
	
	SetDoc(nullptr);
}

void UB2UIBattleCombo::UpdateComboNumMaterial(float ExpireProgress)
{
	if (ComboNumDisplayMID && GetWorld())
	{
		ComboNumDisplayMID->SetScalarParameterValue(FName(TEXT("ComboNum")), FMath::Min(CurComboNum, COMBONUM_DISPLAY_LIMIT));
		ComboNumDisplayMID->SetScalarParameterValue(FName(TEXT("ComboExpireProgress")), ExpireProgress);
	}
}

void UB2UIBattleCombo::SetCombo(int32 SuccessiveComboNum)
{
	const int32 ComboNumSubtractMinLevel = SuccessiveComboNum - ComboPraiseTextMinLevel;
	if (ComboNumSubtractMinLevel >= 0 && SuccessiveComboNum <= ComboPraiseTextMaxLevel)
	{
		// 현 구간에서 어떤 문구를 보여줄지에 대한 인덱스
		const int32 PraiseTextIndex = ComboNumSubtractMinLevel / ComboPraiseTextSwitchInterval;
		// 문구가 바뀔 타이밍인지 여부.
		const bool bTimeToSwitch = ((ComboNumSubtractMinLevel % ComboPraiseTextSwitchInterval) == 0);

		if (WS_ComboPraiseTextImage.IsValid())
		{
			WS_ComboPraiseTextImage->SetActiveWidgetIndex(PraiseTextIndex);
		}

		if (bTimeToSwitch)
		{
			// 이 시점부터 일정 시간동안 보이도록.
			APlayerController* OwningPC = GetOwningPlayer();
			if (OwningPC)
			{
				bComboPraiseTextVisibility = true;
				// 이전 구간 시작 시 설치한 타이머가 아직 발동하지 않은 상태에서 다시 들어올 수 있으니 이전 건 일단 끈다.
				OwningPC->GetWorldTimerManager().ClearTimer(ComboPraiseTextShowTimerHandle);
				OwningPC->GetWorldTimerManager().SetTimer(ComboPraiseTextShowTimerHandle, this, &UB2UIBattleCombo::ComboPraiseTextShowTimerCB, ComboPraiseTextShowDuration, false);

				ComboPraiseTextSwitched(SuccessiveComboNum, PraiseTextIndex);
			}
		}
	}

	// WidgetBP 쪽에도.
	SetCombo_BP(SuccessiveComboNum, bIsCounterGameMode);
}

void UB2UIBattleCombo::ComboPraiseTextShowTimerCB()
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetWorldTimerManager().ClearTimer(ComboPraiseTextShowTimerHandle);
	}
	bComboPraiseTextVisibility = false;
}

//====================================================================================
//									Click Handler
//====================================================================================


//====================================================================================
//									Doc Delegate
//====================================================================================
void UB2UIBattleCombo::OnChangedComboNum(class UB2UIDocBase* Sender, int32 ComboNum, int32 PrevComboNum)
{
	CurComboNum = ComboNum;
	SetCombo(ComboNum);

	ComboNum > 0 ? SetVisibility(ESlateVisibility::HitTestInvisible) : SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIBattleCombo::OnChangedComboExpireProgress(class UB2UIDocBase* Sender, float ComboExpireProgress, float PrevComboExpireProgress)
{
	UpdateComboNumMaterial(ComboExpireProgress);
}

void UB2UIBattleCombo::OnChangedCriticalRateByComboNum(class UB2UIDocBase* Sender, float NewAdditionalRate, float PrevAdditionalRate)
{
	if (TB_AddCriticalRateByCombo.IsValid())
	{
		int32 CastedRate = (int32)(NewAdditionalRate * 100.0f); // Int % 로 환산한 값
		if (CastedRate > 0)
		{
			TB_AddCriticalRateByCombo->SetText(FText::Format(
				// InGameHUD_CriticalRateIncByCombo 에 해당하는 문자열에 {0} 포함.
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("InGameHUD_CriticalRateIncByCombo"))), FText::AsNumber(CastedRate)
				));

			TB_AddCriticalRateByCombo->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			TB_AddCriticalRateByCombo->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIBattleCombo::OnChangedIncCounterDamageCombo(class UB2UIDocBase* Sender, float NewIncCounterDamageCombo, float PrevIncCounterDamageCombo)
{
	//CriticalRateByComboNum과 같은 Text widget을 사용한다.
	if (TB_AddCriticalRateByCombo.IsValid())
	{
		int32 CastedRate = (int32)(NewIncCounterDamageCombo * 100.0f); // Int % 로 환산한 값
		if (CastedRate > 0)
		{
			TB_AddCriticalRateByCombo->SetText(FText::Format(
				// InGameHUD_CriticalRateIncByCombo 에 해당하는 문자열에 {0} 포함.
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("InGameHUD_CounterDamageIncByCombo"))), FText::AsNumber(CastedRate)
				));

			TB_AddCriticalRateByCombo->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			TB_AddCriticalRateByCombo->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}