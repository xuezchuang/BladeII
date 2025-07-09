// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UITimerNotice.h"

void UB2UITimerNotice::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_ContentText);
	GET_SLOT(UTextBlock, TB_RemainTime);
}

void UB2UITimerNotice::Init()
{
	Super::Init();

	m_fRemainTime = -1;
}

void UB2UITimerNotice::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UITimerNotice::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_fRemainTime > 0)
	{
		RefreshTimerUI();

		m_fRemainTime -= InDeltaTime;

		if (m_fRemainTime <= 0)
		{
			OnFinishHandler.ExecuteIfBound();
		}
	}
}

void UB2UITimerNotice::SetContentText(FText InText)
{
	TB_ContentText->SetText(InText);
}

void UB2UITimerNotice::SetNoticeTimer(float InTimer, const FTimerNoticeOnFinish& handler)
{
	OnFinishHandler = handler;

	m_fRemainTime = InTimer;

	RefreshTimerUI();
}

void UB2UITimerNotice::RefreshTimerUI()
{
	if (TB_RemainTime.IsValid())
		TB_RemainTime->SetText(FText::AsNumber(int32(m_fRemainTime)));
}
