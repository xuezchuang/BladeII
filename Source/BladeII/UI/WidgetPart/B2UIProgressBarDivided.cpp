#include "BladeII.h"
#include "B2UIProgressBarDivided.h"
#include "B2UIManager.h"

UB2UIProgressBarDivided::UB2UIProgressBarDivided(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TotalDivideNum = 1;
	CurDividedNum = 0;
	PrevCurDivideNum = 0;
	IsOddCurDividedNum = false;
	PercentPerEachDivide = 0.f;
}

void UB2UIProgressBarDivided::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UProgressBar, PB_Extra)
	GET_SLOT(UTextBlock, TB_CurDivideNum)
}

void UB2UIProgressBarDivided::SetPercent(float InValue)
{
	//현재 나타내야 되는 x숫자
	CurDividedNum = FPlatformMath::CeilToInt(InValue / PercentPerEachDivide);
	SetCurDividedNum(CurDividedNum);

	//현재 진행중인 Progress가 홀수냐
	IsOddCurDividedNum = ( CurDividedNum % 2 != 0 );

	//막판에는 밑에 깔린 바를 안보이게 해줘야됨
	HideExtraProgressBar(CurDividedNum <= 1);

	//위와 아래의 프로그래스바의 스타일을 재설정
	UpdateProgressBarStyle();

	//현재 프로그레스바의 퍼센트
	float CurDividedPercent = CurDividedNum > 0 ? 1.f - (static_cast<float>(CurDividedNum)-(InValue / PercentPerEachDivide)) : 0.f;

	Super::SetPercent(CurDividedPercent);
	SetPercentDivided_BP(InValue, CurDividedPercent, PrevCurDividedPercent, CurDividedNum, PrevCurDivideNum);

	//이전 값 저장
	PrevCurDivideNum = CurDividedNum;
	PrevCurDividedPercent = CurDividedPercent;
}

void UB2UIProgressBarDivided::UpdateProgressBarStyle()
{
	//두가지 이미지가 교차되므로 홀짝에 따라 바꿔준다.
	if (IsOddCurDividedNum)
	{
		SetProgressStyle(PB, StyleOdd);
		SetProgressStyle(PB_Extra, StyleEven);
	}
	else
	{
		SetProgressStyle(PB_Extra, StyleOdd);
		SetProgressStyle(PB, StyleEven);
	}
}

void UB2UIProgressBarDivided::SetPercentByValue(float CurValue, float MaxValue)
{
	Super::SetPercentByValue(CurValue, MaxValue);
}

void UB2UIProgressBarDivided::SetDivideNum(int32 InValue)
{
	TotalDivideNum = InValue;

	PercentPerEachDivide = 1.f / static_cast<float>(InValue);
}

void UB2UIProgressBarDivided::SetCurDividedNum(int32 InValue)
{
	if (TB_CurDivideNum.IsValid())
		TB_CurDivideNum->SetText(FText::AsNumber(InValue));
}

void UB2UIProgressBarDivided::SetProgressStyle(TWeakObjectPtr<UProgressBar> TargetPB, const FProgressBarStyle& InStyle)
{
	//if (TargetPB.IsValid())
		//TargetPB->WidgetStyle = InStyle;
}

void UB2UIProgressBarDivided::HideExtraProgressBar(bool bHide)
{
	if (PB_Extra.IsValid())
		bHide ? PB_Extra->SetVisibility(ESlateVisibility::Hidden) : PB_Extra->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UB2UIProgressBarDivided::SynchronizeProperties()
{
	if (!PB_Extra.IsValid())
		CacheAssets();

	SetDivideNum(TotalDivideNum);

	Super::SynchronizeProperties();
}

////////////////////////////////////////////////////////////////////////////////
// UB2UIProgressBarDividedDual - 그냥 별도의 수치를 표시하는 추가 ProgressBar 설치한 것임.
////////////////////////////////////////////////////////////////////////////////

UB2UIProgressBarDividedDual::UB2UIProgressBarDividedDual(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowSecondPB = false;
}

void UB2UIProgressBarDividedDual::SetShowSecond(bool bShow)
{
	bShowSecondPB = bShow;
	if (P_SecondPBSet.IsValid())
	{
		P_SecondPBSet->SetVisibility(bShowSecondPB ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UIProgressBarDividedDual::SetProgressOfSecond(float CurValue, float MaxValue)
{
	if (PB_Second.IsValid())
	{
		PB_Second->SetPercent(CurValue / MaxValue);
	}
}

void UB2UIProgressBarDividedDual::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UPanelWidget, P_SecondPBSet);
	GET_SLOT(UProgressBar, PB_Second);
}

void UB2UIProgressBarDividedDual::Init()
{
	Super::Init();
	
	SetShowSecond(bShowSecondPB); // 초기 설정된 값으로.
}