// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIGuildSchedule.h"
#include "B2UIManager.h"

#include "B2UIDocHelper.h"
void UB2UIGuildScheduleSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_GuildState)
	GET_SLOT(UTextBlock, TB_Time)
	GET_SLOT(UTextBlock, TB_Day)

	GET_SLOT(UTextBlock, TB_SelectGuildState)
	GET_SLOT(UTextBlock, TB_SelectTime)
	GET_SLOT(UTextBlock, TB_SelectDay)

	GET_SLOT(UPanelWidget, OV_D1)
	GET_SLOT(UPanelWidget, OV_Ing)
}

void UB2UIGuildScheduleSlot::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIGuildScheduleSlot::Init()
{
	Super::Init();
}

void UB2UIGuildScheduleSlot::DestroySelf(UB2UIManager* InUIManager)
{


	Super::DestroySelf(InUIManager);
}


void UB2UIGuildScheduleSlot::SetSchedule(b2network::B2mdGuildBattleScheduleInfoPtr InSchedule)
{
	FDateTime ScheduleTime = FDateTime::Now();

	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	int32 TestDay = ScheduleTime.GetDay();
	int32 TestHour = ScheduleTime.GetHour();
	int32 TestMinute = ScheduleTime.GetMinute();
	//int32 TestDay = ScheduleTime.GetDay();

	bool bScheduleState = false;

	int32 SvrStartTime = (InSchedule->start_hour * 100) + InSchedule->start_min;
	int32 SvrEndTime = (InSchedule->end_hour * 100) +InSchedule->end_min;

	int32 ClientTime = (ScheduleTime.GetHour() * 100) + ScheduleTime.GetMinute();

	if (GuildDoc->GetGuildbattleStateNew().current_state == InSchedule->state)
	{
		if (ScheduleTime.GetDayOfWeek() == SvrToEngineDay(InSchedule->start_day) ||
			ScheduleTime.GetDayOfWeek() == SvrToEngineDay(InSchedule->end_day))
		{
			if (ClientTime >= SvrStartTime
				&& ClientTime <= SvrEndTime)
				bScheduleState = true;
		}
	}

	OV_D1->SetVisibility(bScheduleState ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	OV_Ing->SetVisibility(bScheduleState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	/*if(ScheduleTime.GetDay() == InSchedule->start_day
		&& ScheduleTime.GetHour() >= InSchedule->start_hour
		&& ScheduleTime.GetHour() <= InSchedule->end_hour
		&& ScheduleTime.GetMinute() >= InSchedule->start_min
		&& ScheduleTime.GetMinute() <= InSchedule->end_min)
	{
		OV_D1->SetVisibility(ESlateVisibility::Hidden);
		OV_Ing->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		OV_D1->SetVisibility(ESlateVisibility::Visible);
		OV_Ing->SetVisibility(ESlateVisibility::Hidden);
	}*/


	SetDay(InSchedule->start_day);
	SetGuildBattleStete(InSchedule->state);
	SetTime(InSchedule->start_hour, InSchedule->start_min, InSchedule->end_hour, InSchedule->end_min);
}


void UB2UIGuildScheduleSlot::SetDay(int32 Inday)
{

	FString DayString;

	switch (Inday)
	{
	case b2network::B2DayOfWeek::SUNDAY:
		DayString = TEXT("CounterAttack_Sunday");
		break;
	case b2network::B2DayOfWeek::MONDAY:
		DayString = TEXT("CounterAttack_Monday");
		break;
	case b2network::B2DayOfWeek::TUESDAY:
		DayString = TEXT("CounterAttack_Tuesday");
		break;
	case b2network::B2DayOfWeek::WEDNESDAY:
		DayString = TEXT("CounterAttack_Wedneday");
		break;
	case b2network::B2DayOfWeek::THURSDAY:
		DayString = TEXT("CounterAttack_Thursday");
		break;
	case b2network::B2DayOfWeek::FRIDAY:
		DayString = TEXT("CounterAttack_Friday");
		break;
	case b2network::B2DayOfWeek::SATURDAY:
		DayString = TEXT("CounterAttack_Saturday");
		break;
	}

	if (TB_Day.IsValid())
		TB_Day->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, DayString));

	if(TB_SelectDay.IsValid())
		TB_SelectDay->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, DayString));
}


void UB2UIGuildScheduleSlot::SetGuildBattleStete(int32 InState)
{
	FString BattleStateStateString;

	if (b2network::B2GuildBattleStateType::READY == InState)
	{
		BattleStateStateString = TEXT("Guild_Schedule_GuildReadyRequest");	//woongbok
	}
	else if (b2network::B2GuildBattleStateType::BATTLE == InState)
	{
		BattleStateStateString = TEXT("Guild_Schedule_GuildProgress");//woongbok
	}

	if(TB_GuildState.IsValid())
		TB_GuildState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, BattleStateStateString));

	if (TB_SelectGuildState.IsValid())
		TB_SelectGuildState->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, BattleStateStateString));
}


void UB2UIGuildScheduleSlot::SetTime(int32 InStartHour, int32 InStartMin, int32 InEndHour, int32 InEndMin)
{
	
	FString TimeString =FString::Printf(TEXT("%02d:%02d~%02d:%02d"), InStartHour, InStartMin, InEndHour, InEndMin);

	if (TB_Time.IsValid())
		TB_Time->SetText(FText::FromString(TimeString));

	if (TB_SelectTime.IsValid())
		TB_SelectTime->SetText(FText::FromString(TimeString));

}

void UB2UIGuildSchedule::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UHorizontalBox, X_SlotBox);
}

void UB2UIGuildSchedule::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIGuildSchedule::Init()
{
	Super::Init();
}

void UB2UIGuildSchedule::DestroySelf(UB2UIManager* InUIManager)
{


	Super::DestroySelf(InUIManager);
}

void UB2UIGuildSchedule::SetSchedule(const TArray<b2network::B2mdGuildBattleScheduleInfoPtr>& InSchedule)
{
	if (!X_SlotBox.IsValid())
		return;

	X_SlotBox->ClearChildren();
	for (auto ScheduleSlot : InSchedule)
	{
		if (ScheduleSlot->state != b2network::B2GuildBattleStateType::READY && ScheduleSlot->state != b2network::B2GuildBattleStateType::BATTLE)
			continue;				//길드 준비상태가 아니거나 길드전상태가 아닌거는 걸러준다

		UB2UIGuildScheduleSlot* GuildScheduleSlotWidget = CreateWidget<UB2UIGuildScheduleSlot>(GetWorld(), GuildScheduleSlot);

		
		UHorizontalBoxSlot* SchedulBoxSlot = Cast<UHorizontalBoxSlot>(X_SlotBox->AddChild(GuildScheduleSlotWidget));

		if (!SchedulBoxSlot)
			return;

		GuildScheduleSlotWidget->Init();
		GuildScheduleSlotWidget->SetSchedule(ScheduleSlot);

		SchedulBoxSlot->SetSize(ESlateSizeRule::Type::Fill);
		SchedulBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		SchedulBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

	}
}


