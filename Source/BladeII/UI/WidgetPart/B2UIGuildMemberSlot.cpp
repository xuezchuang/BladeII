#include "BladeII.h"
#include "B2UIGuildMemberSlot.h"
#include "B2UIGuildMemberPart.h"
#include "B2UIGuildInfoPart.h"
#include "Retailer.h"
#include "BladeIIUtil.h"

UB2UIGuildMemberSlot::UB2UIGuildMemberSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIGuildMemberSlot::Init()
{
	Super::Init();

	this->SetVisibility(ESlateVisibility::Visible);
}

void UB2UIGuildMemberSlot::CacheAssets()
{
	GET_SLOT(UB2UIGuildInfoMemberPart, UIP_GuildMemberPart);
	UIP_GuildMemberPart->Init();
	GET_SLOT(UB2UIGuildInfoPart3, UIP_GuildInfoPart3);
	UIP_GuildInfoPart3->Init();
	GET_SLOT(UB2RichTextBlock, RTB_ContactState);
	GET_SLOT(UB2Button, BTN_Click);
	GET_SLOT(UB2Button, BTN_CharacterDetail);

	GET_SLOT(UWidgetSwitcher, WS_RankNumber);
	GET_SLOT(UB2RichTextBlock, RTB_Rank);

	GET_SLOT(UTextBlock, TB_SupportTitle);
	GET_SLOT(UTextBlock, TB_SupportCount);
}

void UB2UIGuildMemberSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Click, &UB2UIGuildMemberSlot::OnClickSelect);
	BIND_CLICK_FUNC_TO_BTN(BTN_CharacterDetail, &UB2UIGuildMemberSlot::OnClickCharacterDetail);
}

void UB2UIGuildMemberSlot::UpdateStaticText()
{
	if (TB_SupportTitle.IsValid())
		TB_SupportTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EssenceRequestNumber")));
}

void UB2UIGuildMemberSlot::OnClickedSenderBTNArea()
{
	Super::OnClickedSenderBTNArea();
}

void UB2UIGuildMemberSlot::OnPressedSenderBTNArea()
{
	Super::OnPressedSenderBTNArea();
}

void UB2UIGuildMemberSlot::OnReleasedSenderBTNArea()
{
	Super::OnReleasedSenderBTNArea();
}

void UB2UIGuildMemberSlot::SetRank(int32 Rank)
{
	if (WS_RankNumber.IsValid() && RTB_Rank.IsValid())
	{
		switch (Rank)
		{
		case 1:
		case 2:
		case 3:
			WS_RankNumber->SetActiveWidgetIndex(Rank - 1);
			break;
		
		default:
			WS_RankNumber->SetActiveWidgetIndex(3);
			if (Rank == 0)
			{
				RTB_Rank->SetText(FText::FromString(FString::Printf(TEXT("-"))));
			}
			else
			{
				RTB_Rank->SetText(FText::AsNumber(Rank));
			}
			break;
		}
	}
}

void UB2UIGuildMemberSlot::SetGuildMember(b2network::B2GuildMemberPtr GuildMemberInfo, int32 Rank)
{
	SetRank(Rank);
	GuildID = GuildMemberInfo->guild_id;
	MemberID = GuildMemberInfo->character_summary->account_id;
	SlotRank = GuildMemberInfo->rank;

	UIP_GuildInfoPart3->SetGuildScore(GuildMemberInfo->contribution);
	UIP_GuildMemberPart->SetCombat(GuildMemberInfo->character_summary->power);
	UIP_GuildMemberPart->SetNickName(GuildMemberInfo->character_summary->account_name);
	UIP_GuildMemberPart->SetCharacterType(IntToPCClass(GuildMemberInfo->character_summary->character_type));
	UIP_GuildMemberPart->SetCharacterLv(GuildMemberInfo->character_summary->character_level);
	UIP_GuildMemberPart->SetRank(GuildMemberInfo->rank);

	if (RTB_ContactState.IsValid())
		SetLogOutTime(GuildMemberInfo->last_logout_time);

	if (TB_SupportCount.IsValid())
		TB_SupportCount->SetText(FText::AsNumber(GuildMemberInfo->total_support_count));
}


void UB2UIGuildMemberSlot::SetLogOutTime(int64 LogOutTime)
{
	FString StrDay, StrHour, StrMin;

	FTimespan RemainTime = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(LogOutTime / 1000);

	int32 RemainDay = RemainTime.GetDays();
	int32 RemainHour = RemainTime.GetHours();
	int32 RemainMin = RemainTime.GetMinutes();

	if (!RemainDay && !RemainHour && !RemainMin)
		RemainMin = 1;

	if (RemainDay != 0)
		StrDay = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Day")), FText::FromString(FString::FromInt(RemainDay))).ToString();

	if (RemainHour != 0)
		StrHour = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Hour")), FText::FromString(FString::FromInt(RemainHour))).ToString();

	if (RemainMin != 0)
		StrMin = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), FText::FromString(FString::FromInt(RemainMin))).ToString();

	FString strTime;


	if (RemainDay == 0)
	{
		if (RemainHour == 0)
			strTime = FString::Printf(TEXT("%s"), *StrMin);
		else
			strTime = FString::Printf(TEXT("%s %s"), *StrHour, *StrMin);
	}
	else
	{
		if (RemainHour == 0)
			strTime = FString::Printf(TEXT("%s"), *StrDay);
		else
			strTime = FString::Printf(TEXT("%s %s"), *StrDay, *StrHour);
	}

	RTB_ContactState->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_ContactTime")), FText::FromString(strTime)));
}

void UB2UIGuildMemberSlot::OnClickSelect()
{
	FVector2D Size = GetDesiredSize();
	MemeberChangeListDelegate.ExecuteIfBound(MemberID, SlotRank, SlotIndex, Size.Y);

}

void UB2UIGuildMemberSlot::OnClickCharacterDetail()
{
	data_trader::Retailer::GetInstance().RequestFindAccount(MemberID);
}

void UB2UIGuildMemberSlot::SetChangeDelegate(FGuildMemberChangeList Delegate)
{
	MemeberChangeListDelegate = Delegate;
}
