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
	//泅犁 唱鸥郴具 登绰 x箭磊
	CurDividedNum = FPlatformMath::CeilToInt(InValue / PercentPerEachDivide);
	SetCurDividedNum(CurDividedNum);

	//泅犁 柳青吝牢 Progress啊 圈荐衬
	IsOddCurDividedNum = ( CurDividedNum % 2 != 0 );

	//阜魄俊绰 关俊 彬赴 官甫 救焊捞霸 秦拎具凳
	HideExtraProgressBar(CurDividedNum <= 1);

	//困客 酒贰狼 橇肺弊贰胶官狼 胶鸥老阑 犁汲沥
	UpdateProgressBarStyle();

	//泅犁 橇肺弊饭胶官狼 欺季飘
	float CurDividedPercent = CurDividedNum > 0 ? 1.f - (static_cast<float>(CurDividedNum)-(InValue / PercentPerEachDivide)) : 0.f;

	Super::SetPercent(CurDividedPercent);
	SetPercentDivided_BP(InValue, CurDividedPercent, PrevCurDividedPercent, CurDividedNum, PrevCurDivideNum);

	//捞傈 蔼 历厘
	PrevCurDivideNum = CurDividedNum;
	PrevCurDividedPercent = CurDividedPercent;
}

void UB2UIProgressBarDivided::UpdateProgressBarStyle()
{
	//滴啊瘤 捞固瘤啊 背瞒登骨肺 圈娄俊 蝶扼 官层霖促.
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
// UB2UIProgressBarDividedDual - 弊成 喊档狼 荐摹甫 钎矫窍绰 眠啊 ProgressBar 汲摹茄 巴烙.
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
	
	SetShowSecond(bShowSecondPB); // 檬扁 汲沥等 蔼栏肺.
}