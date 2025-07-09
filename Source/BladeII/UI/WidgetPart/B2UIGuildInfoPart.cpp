#include "BladeII.h"
#include "B2UIGuildInfoPart.h"
#include "B2UIGuildMark.h"
#include "B2ItemInfo.h"
#include "B2UIGuildMemberPart.h"
#include "Retailer.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIGuildInfoPart1::CacheAssets()
{
	GET_SLOT(UB2UIGuildMark, UIP_GuildMark)
	if (UIP_GuildMark.IsValid())
		UIP_GuildMark->Init();
	GET_SLOT(UB2RichTextBlock, RTB_GuildName)
	GET_SLOT(UB2RichTextBlock, RTB_GuildMasterName)
	GET_SLOT(UTextBlock, TB_GuildLv)
}

void UB2UIGuildInfoPart1::BindDelegates()
{

}

void UB2UIGuildInfoPart1::SetGuildMark(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor)
{
	if (UIP_GuildMark.IsValid())
		UIP_GuildMark->SetGuildMark(Pattern, PatternColor, BG, BGColor);
}

void UB2UIGuildInfoPart1::SetGuildName(FText GuildName)
{
	if (RTB_GuildName.IsValid())
		RTB_GuildName->SetText(GuildName);
}

void UB2UIGuildInfoPart1::SetGuildMasterName(FText GuildMasterName)
{
	if (RTB_GuildMasterName.IsValid())
		RTB_GuildMasterName->SetText(GuildMasterName);
}

void UB2UIGuildInfoPart1::SetGuildLv(int32 InLv)
{
	if (TB_GuildLv.IsValid())
		TB_GuildLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(InLv)));
}

void UB2UIGuildInfoPart2::CacheAssets()
{
	GET_SLOT(UB2RichTextBlock, RTB_GuildMember)
	GET_SLOT(UTextBlock, TB_CombatPower)

	GET_SLOT(UTextBlock, TB_Combat)
	GET_SLOT(UB2RichTextBlock, RTB_GuildMember_text)
}

void UB2UIGuildInfoPart2::BindDelegates()
{

}

void UB2UIGuildInfoPart2::UpdateStaticText()
{
	if(TB_Combat.IsValid())
		TB_Combat->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));
	if (RTB_GuildMember_text.IsValid())
		RTB_GuildMember_text->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildNumber2")));

}

void UB2UIGuildInfoPart2::SetCombat(int32 Combat)
{
	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(Combat ? FText::AsNumber(Combat) : FText::FromString(TEXT("-")));
}

void UB2UIGuildInfoPart2::SetGuildNumber(int32 MemberCount, int32 MaxMemberCount)
{
	if (RTB_GuildMember.IsValid())
		RTB_GuildMember->SetText(FText::AsNumber(MemberCount));
}

void UB2UIGuildInfoPart3::CacheAssets()
{
	GET_SLOT(UB2RichTextBlock, RTB_GuildScore);
}

void UB2UIGuildInfoPart3::BindDelegates()
{

}

void UB2UIGuildInfoPart3::SetGuildScore(int32 Score)
{
	if (RTB_GuildScore.IsValid())
		RTB_GuildScore->SetText(Score ? FText::AsNumber(Score) : FText::FromString(TEXT("-")));
}

void UB2UIGuildInfoPart4::CacheAssets()
{
	GET_SLOT(UB2UIGuildMark, UIP_GuildMark);
	if (UIP_GuildMark.IsValid())
		UIP_GuildMark->Init();
	GET_SLOT(UB2RichTextBlock,		TB_GuildName)
	GET_SLOT(UB2RichTextBlock,		TB_Record)
	GET_SLOT(UB2RichTextBlock,		TB_Point)
}

void UB2UIGuildInfoPart4::BindDelegates()
{

}

void UB2UIGuildInfoPart4::SetGuildMark(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor)
{
	if (UIP_GuildMark.IsValid())
		UIP_GuildMark->SetGuildMark(Pattern, PatternColor, BG, BGColor);
}

void UB2UIGuildInfoPart4::SetGuildInfo(FText GuildName, int32 AttackCount, int32 AttackMaxCount, int32 Point, int32 MaxPoint)
{
	if (TB_GuildName.IsValid())
		TB_GuildName->SetText(GuildName);
	if (TB_Record.IsValid())
		TB_Record->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_AttackCount")), FText::AsNumber(AttackCount), FText::AsNumber(AttackMaxCount)));
	if (TB_Point.IsValid())
		TB_Point->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildInfoMedalCount")), FText::AsNumber(Point), FText::AsNumber(MaxPoint)));
	
}

void UB2UIGuildInfoPart5::CacheAssets()
{
	GET_SLOT(UB2UIGuildMark, UIP_GuildMark);
	if (UIP_GuildMark.IsValid())
		UIP_GuildMark->Init();
	GET_SLOT(UB2RichTextBlock, TB_GuildName)
	GET_SLOT(UB2RichTextBlock, TB_Point)

	GET_SLOT(UTextBlock, TB_GuildExp)
}

void UB2UIGuildInfoPart5::BindDelegates()
{

}

void UB2UIGuildInfoPart5::SetGuildMark(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor)
{
	if (UIP_GuildMark.IsValid())
		UIP_GuildMark->SetGuildMark(Pattern, PatternColor, BG, BGColor);
}

void UB2UIGuildInfoPart5::SetGuildInfo(FText GuildName, int32 Point, int32 GuildExp)
{
	if (TB_GuildName.IsValid())
		TB_GuildName->SetText(GuildName);

	if (TB_Point.IsValid())
		TB_Point->SetText(FText::AsNumber(Point));

	if (TB_GuildExp.IsValid())
	{
		TB_GuildExp->SetVisibility(GuildExp ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		TB_GuildExp->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Result_Exp")), FText::AsNumber(GuildExp)));
	}
}

void UB2UIGuildInfoMedal::CacheAssets()
{
	GET_SLOT(UB2Image, IMG_OnMedal)
	GET_SLOT(UB2Image, IMG_OffMedal)
}

void UB2UIGuildInfoMedal::BindDelegates()
{

}

void UB2UIGuildInfoMedalPart::CacheAssets()
{

	GET_SLOT(UB2UIGuildInfoMedal, UIP_GuildMedal1);

	GuildMadalArr.SetNum(4);

	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, GuildMadalArr[0], UIP_GuildMedal1)
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, GuildMadalArr[1], UIP_GuildMedal2)
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, GuildMadalArr[2], UIP_GuildMedal3)
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, GuildMadalArr[3], UIP_GuildMedal4)


	GET_SLOT(UB2RichTextBlock, TB_GuildName)
	GET_SLOT(UB2RichTextBlock, TB_GuildNumber)

	GET_SLOT(UPanelWidget, O_NoAttack)
	GET_SLOT(UPanelWidget, O_AttackInfo)

	GET_SLOT(UTextBlock, TB_AtteckNumber)
	GET_SLOT(UTextBlock, TB_AttackComplet)
}

void UB2UIGuildInfoMedalPart::BindDelegates()
{

}

void UB2UIGuildInfoMedalPart::SetMedalCount(int32 MedalCount)
{
	if(UIP_GuildMedal1.IsValid())
		MedalCount ? UIP_GuildMedal1->OnMedal() : UIP_GuildMedal1->OffMedal();

	for (int32 i = 0; i < GuildMadalArr.Num(); ++i)
	{
		if (i < MedalCount)
			GuildMadalArr[i]->OnMedal();
		else
			GuildMadalArr[i]->OffMedal();
	}


	RemainMedalCount = MedalCount;
}


void UB2UIGuildInfoMedalPart::SetGuildName(bool InMyGuild, int32 Number, FString GuildName)
{

	TB_GuildNumber->SetText(FText::AsNumber(Number));
	TB_GuildName->SetText(FText::FromString(GuildName));

}

void UB2UIGuildInfoMedalPart::SetIsAttack(bool IsAttack)
{
	if(O_NoAttack.IsValid())
		O_NoAttack->SetVisibility(IsAttack ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (O_AttackInfo.IsValid())
		O_AttackInfo->SetVisibility(IsAttack ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if(TB_AttackComplet.IsValid())
		TB_AttackComplet->SetVisibility(IsAttack ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}


void UB2UIGuildInfoMedalPart::SettextAttackNumber(FText InText)
{
	if (TB_AtteckNumber.IsValid())
		TB_AtteckNumber->SetText(InText);
}

void UB2UIGuildInfoMedalPart::UpdateStaticText()
{
	if (TB_AttackComplet.IsValid())
		TB_AttackComplet->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_Complete")));
}

void UB2UIGuildInfoMedalCastlePart::CacheAssets()
{

	GET_SLOT(UB2UIGuildInfoMedal, UIP_GuildMedal1);

	GET_SLOT(UB2RichTextBlock, TB_MedalCount)

	GET_SLOT(UB2Image, IMG_MyGuild)
	GET_SLOT(UB2Image, IMG_OtherGuild)

	GET_SLOT(UB2RichTextBlock, TB_GuildName)
	GET_SLOT(UB2RichTextBlock, TB_GuildNumber)

}

void UB2UIGuildInfoMedalCastlePart::BindDelegates()
{

}

void UB2UIGuildInfoMedalCastlePart::SetMedalCount(int32 MedalCount)
{
	if (UIP_GuildMedal1.IsValid())
		MedalCount ? UIP_GuildMedal1->OnMedal() : UIP_GuildMedal1->OffMedal();

	if (TB_MedalCount.IsValid())
	{
		TB_MedalCount->SetVisibility(MedalCount ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		TB_MedalCount->SetText(FText::FromString(FString::Printf(TEXT("x%d"), MedalCount)));
	}

}


void UB2UIGuildInfoMedalCastlePart::SetGuildName(bool bMy, int32 Number, FString GuildName)
{
	bMy ? IMG_MyGuild->SetVisibility(ESlateVisibility::Visible) : IMG_MyGuild->SetVisibility(ESlateVisibility::Hidden);

	TB_GuildNumber->SetText(FText::AsNumber(Number));
	TB_GuildName->SetText(FText::FromString(GuildName));

}


void UB2UIGuildInfoMedalCastlePart::PlayMedalShot()
{
	UIP_GuildMedal1->MedalShot();
}

void UB2UIGuildMapSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Image, IMG_Select);
	GET_SLOT(UB2Image, IMG_Disable);
	GET_SLOT(UB2Image, IMG_Me);
	GET_SLOT(UB2Button, BTN_Click);
}

void UB2UIGuildMapSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Click, &UB2UIGuildMapSlot::OnClickSelect)
}

void UB2UIGuildMapSlot::OnSelectMap()
{
	IMG_Select->SetVisibility(ESlateVisibility::Visible);
	IMG_Disable->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIGuildMapSlot::OffSelectMap()
{
	IMG_Select->SetVisibility(ESlateVisibility::Hidden);
	IMG_Disable->SetVisibility(ESlateVisibility::Visible);
}

void UB2UIGuildMapSlot::SetMyCastlePos(bool MyPos)
{
	IMG_Me->SetVisibility(MyPos ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIGuildMapSlot::SetMapIndex(int32 Index)
{
	SlotIndex = Index;
}

void UB2UIGuildMapSlot::SetDelegate(FGuildMapquickSlot MapSlotdelegate)
{
	QuickSlotDelegate = MapSlotdelegate;
}

void UB2UIGuildMapSlot::OnClickSelect()
{
	QuickSlotDelegate.ExecuteIfBound(SlotIndex);
}

void UB2UIGuildBattleSituationSlot::CacheAssets()
{
	GET_SLOT(UB2RichTextBlock, TB_MyName)
	GET_SLOT(UB2RichTextBlock, TB_OtherName)
	GET_SLOT(UB2Image, IMG_MyAttack)
	GET_SLOT(UB2Image, IMG_OtherAttack)

	GET_SLOT(UB2RichTextBlock, TB_Time)

	GuildMedalArr.SetNum(4);

	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, GuildMedalArr[0], UIP_GuildMedal1)
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, GuildMedalArr[1], UIP_GuildMedal2)
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, GuildMedalArr[2], UIP_GuildMedal3)
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, GuildMedalArr[3], UIP_GuildMedal4)

	GET_SLOT(UB2UIGuildMark, UIP_MyGuildMark)
		if(UIP_MyGuildMark.IsValid())
			UIP_MyGuildMark->Init();
	GET_SLOT(UB2UIGuildMark, UIP_OtherGuildMark)
		if (UIP_OtherGuildMark.IsValid())
			UIP_OtherGuildMark->Init();

	GET_SLOT(UTextBlock, TB_MyPowerIndex)
	GET_SLOT(UTextBlock, TB_OtherPowerIndex)

	for (auto& El : GuildMedalArr)
	{
		El->Init();
	}
}

void UB2UIGuildBattleSituationSlot::BindDelegates()
{

}

void UB2UIGuildBattleSituationSlot::SetGuildMark(bool MyGuild, int32 Mark, int32 MarkColor, int32 BG, int32 BGIndex)
{
	if (MyGuild)
	{
		if (UIP_MyGuildMark.IsValid())
			UIP_MyGuildMark->SetGuildMark(Mark, MarkColor, BG, BGIndex);
	}
	else
	{
		if (UIP_OtherGuildMark.IsValid())
			UIP_OtherGuildMark->SetGuildMark(Mark, MarkColor, BG, BGIndex);
	}

}

void UB2UIGuildBattleSituationSlot::SetSituationSlot(int32 MyNumber, FString MyName, int32 OtherNumber, FString OtherName, int32 MedalCount, bool MyAttack, int64 Time)
{
	SetMyGuildName(MyNumber, MyName);
	SetOtherGuildName(OtherNumber, OtherName);
	SetMedal(MedalCount);
	SetAttack(MyAttack);

	SetTime(Time);
}

void UB2UIGuildBattleSituationSlot::SetMyGuildName(int32 Number, FString Name)
{
	if (TB_MyPowerIndex.IsValid())
		TB_MyPowerIndex->SetText(FText::AsNumber(Number));

	if (TB_MyName.IsValid())
		TB_MyName->SetText(FText::FromString(FString::Printf(TEXT("%s"), *Name)));
}

void UB2UIGuildBattleSituationSlot::SetOtherGuildName(int32 Number, FString Name)
{
	if (TB_OtherPowerIndex.IsValid())
		TB_OtherPowerIndex->SetText(FText::AsNumber(Number));

	if (TB_OtherName.IsValid())
		TB_OtherName->SetText(FText::FromString(FString::Printf(TEXT("%s"),  *Name)));
}

void UB2UIGuildBattleSituationSlot::SetMedal(int32 Count)
{
	int32 i = 0;

	for (auto& El : GuildMedalArr)
	{
		if (i + 1 <= Count)
			El->OnMedal();
		else
			El->OffMedal();

		++i;
	}
}

void UB2UIGuildBattleSituationSlot::SetAttack(bool MyAttack)
{
	if (MyAttack)
	{
		IMG_MyAttack->SetVisibility(ESlateVisibility::Visible);
		IMG_OtherAttack->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		IMG_MyAttack->SetVisibility(ESlateVisibility::Hidden);
		IMG_OtherAttack->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2UIGuildBattleSituationSlot::SetTime(int64 Time)
{

	FString StrDay, StrHour, StrMin;

	FTimespan RemainTime = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(Time / 1000);

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

	TB_Time->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_ContactTime")), FText::FromString(strTime)));
}

void UB2UIGuildSeasonRewardSlot::CacheAssets()
{
	GET_SLOT(UB2RichTextBlock, TB_Rank)
	GET_SLOT(UB2RichTextBlock, TB_ItemNumber)
	GET_SLOT(UB2Image, IMG_Select)

	GET_SLOT(UB2Image, IMG_Item)
}

void UB2UIGuildSeasonRewardSlot::BindDelegates()
{
	
}

void UB2UIGuildSeasonRewardSlot::SetRewardSlot(FText Rank, int32 RewardId, int32 Count)
{
	SetRank(Rank);
	SetRewardInfo(RewardId, Count);
}

void UB2UIGuildSeasonRewardSlot::SetRank(FText Rank)
{
	TB_Rank->SetText(Rank);
}

void UB2UIGuildSeasonRewardSlot::SetRewardInfo(int32 RewardId, int32 Count)
{
	if (IMG_Item.IsValid())
		IMG_Item->SetBrushFromMaterial(BladeIIGameImpl::GetClientDataStore().GetRewardIconMaterial(RewardId));

	if(TB_ItemNumber.IsValid())
		TB_ItemNumber->SetText(FText::FromString(FString::Printf(TEXT("x %d"), Count)));
}

void UB2UIGuildSeasonRewardSlot::SetIncludeRank(bool bRank)
{
	if (IMG_Select.IsValid())
		IMG_Select->SetVisibility(bRank ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}


void UB2UIGuildInviteSlot::CacheAssets()
{
	GET_SLOT(UB2UIGuildInfoMemberPart, UIP_GuildMemberPart)
	if (UIP_GuildMemberPart.IsValid())
		UIP_GuildMemberPart->Init();
	GET_SLOT(UB2Button, BTN_Invite)
	GET_SLOT(UB2RichTextBlock, TB_Invite)
	GET_SLOT(UB2Button, BTN_CharacterDetail)
}

void UB2UIGuildInviteSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Invite, &UB2UIGuildInviteSlot::OnClickSurch);
	BIND_CLICK_FUNC_TO_BTN(BTN_CharacterDetail, &UB2UIGuildInviteSlot::OnClickCharacterDetail);
}

void UB2UIGuildInviteSlot::SetGuildInfo(const FString& NickName, int32 Combat, EPCClass PCClass, int32 Lv, int32 Rank)
{
	UIP_GuildMemberPart->SetNickName(NickName);
	UIP_GuildMemberPart->SetCombat(Combat);
	UIP_GuildMemberPart->SetCharacterType(PCClass);
	UIP_GuildMemberPart->SetCharacterLv(Lv);
	UIP_GuildMemberPart->SetRank(Rank);
}

void UB2UIGuildInviteSlot::SetGuildState(int32 Stats, int64 CharAccountId)
{
	if (b2network::B2GuildInviteState::INVITABLE == Stats)
	{
		BTN_Invite->SetIsEnabled(true);
		TB_Invite->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Invitable")));
	}
	else
	{
		BTN_Invite->SetIsEnabled(false);
		if (b2network::B2GuildInviteState::JOINED_GUILD == Stats)
			TB_Invite->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_JoindGuild")));
		else if (b2network::B2GuildInviteState::INVITED == Stats)
			TB_Invite->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Invited")));
		else if (b2network::B2GuildInviteState::NON_INVITING == Stats)
			TB_Invite->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_NonInvitable")));

	}

	AccountId = CharAccountId;
}

void UB2UIGuildInviteSlot::OnClickSurch()
{
	DeliveryInviteGuildTicket = DeliveryInviteGuildClass<>::GetInstance().Subscribe([this](){

		this->TB_Invite->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Invited")));
		this->BTN_Invite->SetIsEnabled(false);
		DeliveryInviteGuildClass<>::GetInstance().Unsubscribe(DeliveryInviteGuildTicket);

	});


	data_trader::Retailer::GetInstance().RequestInviteGuild(AccountId);
}

void UB2UIGuildInviteSlot::OnClickCharacterDetail()
{
	data_trader::Retailer::GetInstance().RequestFindAccount(AccountId);
}

