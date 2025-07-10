#include "B2UIChapterStarRewardBox.h"
#include "B2UIManager.h"

UB2UIChapterStarRewardBox::UB2UIChapterStarRewardBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GoalReachedState = EUIChapterStarRewardState::NotReached;
	MyRewardIndex = INDEX_NONE;
	StarRewardMaxNum = INDEX_NONE;
	StarRewardCurNum = INDEX_NONE;
	bMyPreviousRewarded = false;
	bAlreadyRewarded = false;
	ParentChapterNum = INDEX_NONE;
}

void UB2UIChapterStarRewardBox::SetFillImage(EStageDifficulty InStageDifficulty)
{
	if (FillImages.Contains(InStageDifficulty) == false)
		return;

	if (PB_Star.IsValid() == false)
		return;

	//PB_Star->WidgetStyle.SetFillImage(FillImages[InStageDifficulty]);	
}

void UB2UIChapterStarRewardBox::CacheAssets()
{
	GET_SLOT(UButton, BTN_Area)
	GET_SLOT(UTextBlock, TB_StarCur)
	GET_SLOT(UTextBlock, TB_StarMax)
	GET_SLOT(UTextBlock, TB_StarGoalNum)
	GET_SLOT(UProgressBar, PB_Star)

	GET_SLOT(UB2RichTextBlock, RTB_JoinGoalNum)

	GET_SLOT(UHorizontalBox, H_PBTexts)
	GET_SLOT(UHorizontalBox, HB_Star)
	GET_SLOT(UHorizontalBox, HB_MissionPoint)
	GET_SLOT(UTextBlock, TB_MissionGoalNum)
}

void UB2UIChapterStarRewardBox::UpdateStaticText()
{
}

void UB2UIChapterStarRewardBox::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UIChapterStarRewardBox::OnClickBtnArea)
}

void UB2UIChapterStarRewardBox::SetRewarded(bool InAlreadyRewarded)
{
	bAlreadyRewarded = InAlreadyRewarded;
	
	UpdateRewardState();
}

void UB2UIChapterStarRewardBox::SetMyPreviousRewarded(bool InPreviousRewarded)
{
	bMyPreviousRewarded = InPreviousRewarded;

	UpdateRewardState();
}

void UB2UIChapterStarRewardBox::SetRewardIndex(int32 InIndex)
{
	MyRewardIndex = InIndex;
}

void UB2UIChapterStarRewardBox::SetStarMaxNum(int32 InValue)
{
	StarRewardMaxNum = InValue;

	if (TB_StarMax.IsValid())
		TB_StarMax->SetText(FText::AsNumber(InValue));

	if (TB_StarGoalNum.IsValid())
		TB_StarGoalNum->SetText(FText::AsNumber(InValue));

	ShowStarText(true);
}

void UB2UIChapterStarRewardBox::SetJoinMaxNum(int32 inValue)
{
	StarRewardMaxNum = inValue;

	if (TB_StarMax.IsValid())
		TB_StarMax->SetText(FText::AsNumber(inValue));

	if (RTB_JoinGoalNum.IsValid())
		RTB_JoinGoalNum->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDungeon_WeeklyReward")), FText::FromString(FString::FromInt(inValue))));

	ShowStarText(false);
}

void UB2UIChapterStarRewardBox::SetStarCurNum(int32 InValue)
{
	StarRewardCurNum = InValue;

	if (TB_StarCur.IsValid())
		TB_StarCur->SetText(FText::AsNumber(InValue));

	if (PB_Star.IsValid())
	{
		float Percent = 0.f;
		if (!(StarRewardMaxNum == 0 && StarRewardCurNum == 0))
			Percent = static_cast<float>(StarRewardCurNum) / static_cast<float>(StarRewardMaxNum);

		PB_Star->SetPercent(Percent);
	}

	UpdateRewardState();
}

void UB2UIChapterStarRewardBox::SetForceProgressText(int32 InCurValue, int32 InMaxValue)
{
	if (TB_StarCur.IsValid())
		TB_StarCur->SetText(FText::AsNumber(InCurValue));

	if (TB_StarMax.IsValid())
		TB_StarMax->SetText(FText::AsNumber(InMaxValue));
}

void UB2UIChapterStarRewardBox::SetRemainTime(int64 InCurValue, int64 InMaxValue)
{
	
	FString StrDay, StrHour, StrMin, StrSconds;


	FTimespan RemainTime = FDateTime::FromUnixTimestamp(InMaxValue) - FDateTime::FromUnixTimestamp(InCurValue);

	int32 RemainDay = RemainTime.GetDays();
	if (RemainDay !=0)
	StrDay = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Day")), FText::FromString(FString::FromInt(RemainDay))).ToString();

	int32 RemainHour = RemainTime.GetHours();
	if (RemainHour != 0)
	StrHour = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Hour")), FText::FromString(FString::FromInt(RemainHour))).ToString();

	int32 RemainMin = RemainTime.GetMinutes();
	if (RemainMin != 0)
	StrMin = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), FText::FromString(FString::FromInt(RemainMin))).ToString();

	int32 Sconds = 0;

	FString strTime;

	if (RemainDay == 0)
	{
		Sconds = RemainTime.GetSeconds();
		StrSconds = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sconds")), FText::FromString(FString::FromInt(Sconds))).ToString();
		strTime = FString::Printf(TEXT("%s %s %s"), *StrHour, *StrMin, *StrSconds);
	}
	else
		strTime = FString::Printf(TEXT("%s %s %s"), *StrDay, *StrHour, *StrMin);


	TB_StarCur->SetText(FText::FromString(strTime));
}

void UB2UIChapterStarRewardBox::SetForceProgressBar(int32 InCurValue, int32 InMaxValue)
{
	if (PB_Star.IsValid())
	{
		float Percent = 0.f;
		
		Percent = static_cast<float>(InCurValue) / static_cast<float>(InMaxValue);

		PB_Star->SetPercent(Percent);
	}
}

void UB2UIChapterStarRewardBox::UpdateRewardState()
{
	GoalReachedState = EUIChapterStarRewardState::NotReached;

	if (bAlreadyRewarded)
		GoalReachedState = EUIChapterStarRewardState::Rewarded;
	else
	{
		if (StarRewardMaxNum > StarRewardCurNum) GoalReachedState = EUIChapterStarRewardState::NotReached;
		else if (StarRewardMaxNum <= StarRewardCurNum) GoalReachedState = EUIChapterStarRewardState::Reached;
	}

	if (BTN_Area.IsValid())
	{ // Reward 를 받은 상황에서는 더 이상 볼 일이 없겠다. 혹은 bMyPreviousRewarded 여부에 따라 달라질 수도..
		BTN_Area->SetIsEnabled(GoalReachedState == EUIChapterStarRewardState::Rewarded ? false : true);
	}

	SetGoalReachedState(GoalReachedState);
}

void UB2UIChapterStarRewardBox::SetParentChapterNum(int32 Invalue)
{
	ParentChapterNum = Invalue;
}


void UB2UIChapterStarRewardBox::SetMissionBox(int32 CurrentPoint, int32 RequirePoint, bool bRewarded)
{
	StarRewardCurNum = CurrentPoint;
	StarRewardMaxNum = RequirePoint;

	if (HB_Star.IsValid())
		HB_Star->SetVisibility(ESlateVisibility::Collapsed);

	if (RTB_JoinGoalNum.IsValid())
		RTB_JoinGoalNum->SetVisibility(ESlateVisibility::Collapsed);

	if (HB_MissionPoint.IsValid())
		HB_MissionPoint->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (TB_MissionGoalNum.IsValid())
	{
		TB_MissionGoalNum->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TB_MissionGoalNum->SetText(FText::AsNumber(RequirePoint));
	}

	// Mission은 RewardState와 상관없이 항상 Click가능
	if (BTN_Area.IsValid())
	{
		BTN_Area->SetVisibility(ESlateVisibility::Visible);
		BTN_Area->SetIsEnabled(true);
	}

	if (CurrentPoint >= RequirePoint)
	{
		if (bRewarded)
			GoalReachedState = EUIChapterStarRewardState::Rewarded;

		else
			GoalReachedState = EUIChapterStarRewardState::Reached;
	}
	else
	{
		GoalReachedState = EUIChapterStarRewardState::NotReached;
	}

	SetGoalReachedState(GoalReachedState);
	
}

void UB2UIChapterStarRewardBox::SetVisibleProgressText(bool bShow)
{
	if (H_PBTexts.IsValid())
		H_PBTexts->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIChapterStarRewardBox::OnClickBtnArea()
{
	OnChapterStarRewardBoxClickedDelgate.ExecuteIfBound(this);
	RewardListDelgate.ExecuteIfBound();
}


//====================================================================================
//									Doc Delegate
//====================================================================================
