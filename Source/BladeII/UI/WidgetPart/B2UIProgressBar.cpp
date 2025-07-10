#include "B2UIProgressBar.h"
#include "B2UIManager.h"


UB2UIProgressBar::UB2UIProgressBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Percent = 0.f;
	PrevPercent = 0.f;
	IsTextFormatPercent = false;
	bShowText = false;
}

void UB2UIProgressBar::CacheAssets()
{
	GET_SLOT(UProgressBar, PB)
	GET_SLOT(UTextBlock, TB)
}

void UB2UIProgressBar::SetPercent(float InValue)
{
	if (PB.IsValid())
		PB->SetPercent(InValue);

	if (IsTextFormatPercent)
		SetText(FText::FromString(FString::Printf(TEXT("%.1f%%"), InValue * 100.0f)));

	SetPercent_BP(InValue, PrevPercent);

	PrevPercent = InValue;
}

void UB2UIProgressBar::SetPercentByValue(float CurValue, float MaxValue)
{
	SetPercent(CurValue / MaxValue);

	if (!IsTextFormatPercent)
		SetText(FText::FromString(FString::FromInt(CurValue) + TEXT(" / ") + FString::FromInt(MaxValue)));

	SetPercentByValue_BP(CurValue, MaxValue);
}

float UB2UIProgressBar::GetPercent() const
{
	//if (PB.IsValid())
		//return PB->Percent;
	return 0.f;
}

float UB2UIProgressBar::GetPercentPrev() const
{
	return PrevPercent;
}

void UB2UIProgressBar::SetText(const FText& InText)
{
	if (TB.IsValid())
		TB->SetText(InText);
}

void UB2UIProgressBar::ShowText(bool bShow)
{
	bShowText = bShow;

	if (TB.IsValid())
		bShow ? TB->SetVisibility(ESlateVisibility::HitTestInvisible) : TB->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIProgressBar::SynchronizeProperties()
{
	if (!PB.IsValid())
		CacheAssets();

	ShowText(bShowText);
	SetPercent(Percent);

	Super::SynchronizeProperties();
}