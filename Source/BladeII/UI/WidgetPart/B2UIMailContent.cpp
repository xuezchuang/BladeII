#include "B2UIMailContent.h"
#include "B2UIItemRaffle.h"
#include "B2UICostumeRaffle.h"
#include "B2LobbyUI_ItemDetailPostOp.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2GameInstance.h"
#include "B2UIEtherLottery.h"
#include "B2EtherManager.h"
#include "B2UITotemRaffle.h"
#include "B2UIMail.h"

void UB2UIMailContent::Init()
{
	Super::Init();
}

void UB2UIMailContent::CacheAssets()
{
	GET_SLOT(UImage, IMG_Portrait);
	GET_SLOT(UTextBlock, TB_NickName);
	GET_SLOT(UTextBlock, TB_Name);
	GET_SLOT(UB2RichTextBlock, RTB_MailContent);

	GET_SLOT(UImage, IMG_GiftPortrait);
	GET_SLOT(UTextBlock, TB_NumGift);
	GET_SLOT(UImage, IMG_Clock_01);
	GET_SLOT(UImage, IMG_Clock_02);
	GET_SLOT(UTextBlock, TB_LeftTime);
	GET_SLOT(UTextBlock, TB_Receive);

	GET_SLOT(UButton, BTN_Receive);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_Goods);

	SenderPortraits.Empty();

	if (NPCPortraitDataTable)
	{
		NPCPortraitDataTable->GetAllRows<FB2MailSenderPortraitTemplate>(TEXT("FB2MailSenderPortraitTemplate"), SenderPortraits);
	}
}

void UB2UIMailContent::UpdateStaticText()
{
	if (TB_Receive.IsValid())
		TB_Receive->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));
}

void UB2UIMailContent::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Receive, &UB2UIMailContent::OnClickedReceiveBTN);
}

void UB2UIMailContent::SetTime(int64 TimeSeconds)
{
	CurrentLeftTimeType = GetLeftTimeType(TimeSeconds);

	if (TB_LeftTime.IsValid())
	{
		switch (CurrentLeftTimeType)
		{
		case UB2UIMailContent::ELeftTimeType::Expiration:
			TB_LeftTime->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailReceive_Expiration")));
			TB_LeftTime->SetColorAndOpacity(LeftTimeTextColor_ShortTime);
			break;

		case UB2UIMailContent::ELeftTimeType::Sec:
			TB_LeftTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_TimeFormat_Sec")), FText::AsNumber(TimeSeconds)));
			TB_LeftTime->SetColorAndOpacity(LeftTimeTextColor_ShortTime);
			break;

		case UB2UIMailContent::ELeftTimeType::Min_Sec:
			TB_LeftTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_TimeFormat_MinSec")), FText::AsNumber(TimeSeconds / 60), FText::AsNumber(TimeSeconds % 60)));
			TB_LeftTime->SetColorAndOpacity(LeftTimeTextColor_MidTime);
			break;

		case UB2UIMailContent::ELeftTimeType::Hour_Min_Sec:
			TB_LeftTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_TimeFormat_HourMinSec")), FText::AsNumber(TimeSeconds / 3600), FText::AsNumber((TimeSeconds % 3600) / 60), FText::AsNumber(TimeSeconds % 60)));
			TB_LeftTime->SetColorAndOpacity(LeftTimeTextColor_LongTime);
			break;

		case UB2UIMailContent::ELeftTimeType::Day_Hour_Min:
			TB_LeftTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_TimeFormat_DayHourMin")), FText::AsNumber(TimeSeconds / 86400), FText::AsNumber((TimeSeconds % 86400) / 3600), FText::AsNumber((TimeSeconds % 3600) / 60)));
			TB_LeftTime->SetColorAndOpacity(LeftTimeTextColor_LongTime);
			break;

		case UB2UIMailContent::ELeftTimeType::Unlimited:
			TB_LeftTime->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_TimeFormat_Unlimited")));
			TB_LeftTime->SetColorAndOpacity(LeftTimeTextColor_LongTime);
			break;

		default:
			break;
		}
	}

	if (IMG_Clock_01.IsValid())
	{
		IMG_Clock_01->SetVisibility(static_cast<int32>(CurrentLeftTimeType) < static_cast<int32>(ELeftTimeType::Hour_Min_Sec) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (IMG_Clock_02.IsValid())
	{
		IMG_Clock_02->SetVisibility(static_cast<int32>(CurrentLeftTimeType) < static_cast<int32>(ELeftTimeType::Hour_Min_Sec) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UIMailContent::SetSender(MailSenderType SenderType, const FString& FriendName)
{
	//Sender가 더 추가되면 데이터테이블로 빼는 것이 좋겠다.
	if (IMG_Portrait.IsValid())
	{
		for (auto* Portrait : SenderPortraits)
		{
			if (Portrait && Portrait->NPC_Type == SenderType)
			{
				IMG_Portrait->SetBrushFromMaterial(Portrait->Portrait);
				break;
			}
		}
	}

	if (TB_Name.IsValid())
	{
		switch (SenderType)
		{
		case MailSenderType::Arin:
			TB_Name->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailSender_NPC_Arin")));
			break;
		case MailSenderType::Hunt:
			TB_Name->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailSender_NPC_Hunt")));
			break;
		case MailSenderType::Gaius:
			TB_Name->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailSender_NPC_Gaius")));
			break;
		case MailSenderType::Bella:
			TB_Name->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailSender_NPC_Bella")));
			break;
		case MailSenderType::Luchi:
			TB_Name->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailSender_NPC_Luchi")));
			break;
		case MailSenderType::Ramiel:
			TB_Name->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailSender_NPC_Remiel")));
			break;
		case MailSenderType::Admin:
			TB_Name->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailSender_NPC_Admin")));
			break;
		case MailSenderType::Gratia:
			TB_Name->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailSender_NPC_Gratia")));
			break;
		case MailSenderType::Brokk:
			TB_Name->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailSender_NPC_Brokk")));
			break;
		default:
			TB_Name->SetText(FText::FromString(TEXT("")));
		}
	}

	//기존 결투장 관리자 라미엘 -> 라미엘 로 변경 닉네임 사용안함
	TB_NickName->SetText(FText::FromString(TEXT("")));
}

void UB2UIMailContent::SetGift(int32 ItemTemplateId, int32 Num)
{
	SetPortraitIcon(ItemTemplateId);
	SetGiftNumText(Num);
}

void UB2UIMailContent::SetLotteryTicketIcon(int32 ItemTemplateId)
{
	SetPortraitIcon(ItemTemplateId);

	if (TB_NumGift.IsValid())
	{
		TB_NumGift->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIMailContent::SetPortraitIcon(int32 _ItemTemplateId)
{
	auto* ItemInfoTable = StaticFindItemInfo();
	if (ItemInfoTable == nullptr)
		return;

	if (IMG_GiftPortrait.IsValid())
	{
		IMG_GiftPortrait->SetBrushFromMaterial(ItemInfoTable->GetItemIcon(_ItemTemplateId));
	}

	if (BTN_Goods.IsValid())
	{
		BTN_Goods->SetItemInfo(_ItemTemplateId);
	}
}

void UB2UIMailContent::SetGiftNumText(int32 _Num)
{
	if (TB_NumGift.IsValid())
	{
		TB_NumGift->SetText(FText::AsNumber(_Num));
	}
}

void UB2UIMailContent::SetMailContent(FB2MailContentPtr MailContent)
{
	if (MailContent.get() == nullptr)
		return;

	MailId = MailContent->id;
	CachedSenderName = MailContent->sender_name;

	//Time
	ExpireTime = FDateTime::FromUnixTimestamp(MailContent->expire_time / 1000);
	SetTime((ExpireTime - UB2GameInstance::GetUTCNow()).GetTotalSeconds());

	//Sender
	SetSender(static_cast<MailSenderType>(MailContent->sender_type), MailContent->sender_name);

	//Gift
	ToReceiveGift = static_cast<MailGiftType>(MailContent->mail_attachType);
	ToReceiveGiftNum = 1;
	ToReceiveItemRefId = 0;
	ToSelectItemRefIds.Empty();
	ToSelectItemNums.Empty();

	switch (ToReceiveGift)
	{
	case MailGiftType::Gold:
		ToReceiveItemRefId = FItemRefIDHelper::GetGoodsID_Gold();
		ToReceiveGiftNum = MailContent->gold;
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;

	case MailGiftType::Gem:
		ToReceiveItemRefId = FItemRefIDHelper::GetGoodsID_Gem();
		ToReceiveGiftNum = MailContent->gem;
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;

	case MailGiftType::Item:
	{
		if (MailContent->mail_item_info.get())
		{
			ToReceiveGiftNum = MailContent->mail_item_info->item_amount;
			ToReceiveItemRefId = MailContent->mail_item_info->item_template_id;
		}
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;
	}

	case MailGiftType::BladePoint:
		ToReceiveItemRefId = FItemRefIDHelper::GetGoodsID_BladePoint();
		ToReceiveGiftNum = MailContent->blade_point;
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;

	case MailGiftType::SocialPoint:
		ToReceiveItemRefId = FItemRefIDHelper::GetGoodsID_FriendshipPoint();
		ToReceiveGiftNum = MailContent->social_point;
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;

	case MailGiftType::LotteryTicket:
	{
		ToReceiveItemRefId = MailContent->general_lottery_id;
		int32 ItemNameTemplateId = BladeIIGameImpl::GetClientDataStore().GetGeneralLotteryItemTemplateId(ToReceiveItemRefId);
		SetLotteryTicketIcon(ItemNameTemplateId);
		break;
	}

	case MailGiftType::Selective_Lottery:
	{
		ToReceiveItemRefId = MailContent->selective_lottery_id;
		int32 ItemNameTemplateId = BladeIIGameImpl::GetClientDataStore().GetSelectiveLotteryItemTemplateId(ToReceiveItemRefId);
		for (auto& SelectiveItem : MailContent->selective_item_infos)
		{
			ToSelectItemRefIds.Add(SelectiveItem->item_template_id);
		}
		SetLotteryTicketIcon(ItemNameTemplateId);
		break;
	}

	case MailGiftType::Selective_Item_Lottery:
	{
		ToReceiveItemRefId = MailContent->selective_item_lottery_id;
		int32 ItemNameTemplateId = FItemRefIDHelper::GetSelectiveItemLotteryRefID(ToReceiveItemRefId);

		int32 select_id = 0;
		

		for (auto& SelectiveItem : MailContent->selective_item_infos)
		{
			

			if (1 == SelectiveItem->item_amount)
			{
				UE_LOG(LogBladeII, Log, TEXT("1 == SelectiveItem->item_amount"));
			}
			else if( 5 == SelectiveItem->item_amount)
			{
				UE_LOG(LogBladeII, Log, TEXT("5 == SelectiveItem->item_amount"));
			}

			UE_LOG(LogBladeII, Log, TEXT("id : %d , amount : %d"), SelectiveItem->item_template_id, SelectiveItem->item_amount);

			if (SelectiveItem->item_template_id == 8000000 || SelectiveItem->item_template_id == 8000100)
				select_id = SelectiveItem->item_template_id;
			

			ToSelectItemRefIds.Add(SelectiveItem->item_template_id);
			ToSelectItemNums.Add(SelectiveItem->item_amount);
		}

		if (MailBox)
		{
			switch (select_id)
			{
			case 8000000: MailBox->upgradestone_mail_totalcount += 1; break;
			case 8000100:  MailBox->rare_upgradestone_mail_totalcount += 1; break;
			default: UE_LOG(LogBladeII, Log, TEXT("error mail type"));
			}

			UE_LOG(LogBladeII, Log, TEXT("upgrade mail %d rare upgrade mail %d "), MailBox->upgradestone_mail_totalcount, MailBox->rare_upgradestone_mail_totalcount);
		}
		else
			UE_LOG(LogBladeII, Log, TEXT("mailbox null"));
		
		

		SetLotteryTicketIcon(ItemNameTemplateId);
		break;
	}

	case MailGiftType::Hero_Piece:
		if (MailContent->mail_item_info.get())
		{
			ToReceiveItemRefId = MailContent->mail_item_info->item_template_id;
			ToReceiveGiftNum = MailContent->hero_piece;
		}
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;

	case MailGiftType::Rank_Promotion_Point:
		if (MailContent->mail_item_info.get())
		{
			ToReceiveItemRefId = MailContent->mail_item_info->item_template_id;
			ToReceiveGiftNum = MailContent->rank_promotion_point;
		}
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;

	case MailGiftType::Dark_Crystal:
	{
		ToReceiveItemRefId = FItemRefIDHelper::GetGoodsID_DarkCrystal();
		ToReceiveGiftNum = MailContent->dark_crystal;
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;
	}

	case MailGiftType::Fixed_Grade_Lottey:
	{
		ToReceiveItemRefId = MailContent->fixed_grade_lottery_id;
		int32 iItemNameTemplateId = BladeIIGameImpl::GetClientDataStore().GetFixedGradeLotteryItemTemplateId(ToReceiveItemRefId);
		SetLotteryTicketIcon(iItemNameTemplateId);
		break;
	}

	case MailGiftType::Stage_Boost_Ticket:
	{
		ToReceiveItemRefId = FItemRefIDHelper::GetGoodsID_StageBoostTicket();
		ToReceiveGiftNum = MailContent->stage_boost_ticket;
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;
	}

	case MailGiftType::Hot_Time_Gold_Buff_Ticket:
	{
		ToReceiveItemRefId = FItemRefIDHelper::GetGoodsID_GoldBuffTicket();
		ToReceiveGiftNum = MailContent->hot_time_gold_buff_ticket;
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;
	}

	case MailGiftType::Hot_Time_Exp_Buff_Ticket:
	{
		ToReceiveItemRefId = FItemRefIDHelper::GetGoodsID_ExpBuffTicket();
		ToReceiveGiftNum = MailContent->hot_time_exp_buff_ticket;
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;
	}

	case MailGiftType::Shop_Ten_Lottery_Ticket:
	{
		ToReceiveItemRefId = FItemRefIDHelper::GetGoodsID_ShopTenLotteryTicket();
		ToReceiveGiftNum = MailContent->shop_ten_lottery_ticket;
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;
	}
	case MailGiftType::Fame_Point:
	{
		ToReceiveItemRefId = FItemRefIDHelper::GetGoodsID_FamePoint();
		ToReceiveGiftNum = MailContent->fame_point;
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;
	}
	case MailGiftType::Selective_Ether_Lottery:
	{
		ToReceiveItemRefId = MailContent->selective_aether_lottery_id;
		int32 ItemNameTemplateId = FItemRefIDHelper::GetSelectiveEtherLotteryRefID(ToReceiveItemRefId);
		SetLotteryTicketIcon(ItemNameTemplateId);
		break;
	}
	case MailGiftType::Prefix_Selective_Lottery:
	{
		ToReceiveItemRefId = MailContent->prefix_selective_lottery_id;
		int32 ItemNameTemplateId = FItemRefIDHelper::GetPrefixSelectiveLotteryRefID(ToReceiveItemRefId);

		
		for (auto& SelectiveItem : MailContent->selective_item_infos)
		{
			ToSelectItemRefIds.Add(SelectiveItem->item_template_id);
			ToSelectItemNums.Add(SelectiveItem->item_amount);
		}

		ToSelectItemRefIds.Sort([](const int32& A, const int32& B) {
			EPCClass TargetA = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(A);
			EPCClass TargetB = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(B);
			if (TargetA < TargetB)
				return true;
			else {
				return A < B;
			}
		});

		SetLotteryTicketIcon(ItemNameTemplateId);
		break;
	}
	case MailGiftType::Stage_Auto_Clear_Ticket:
	{
		ToReceiveItemRefId = FItemRefIDHelper::GetGoodsID_StageAutoClearTicket();
		ToReceiveGiftNum = MailContent->stage_auto_clear_ticket;
		SetGift(ToReceiveItemRefId, ToReceiveGiftNum);
		break;
	}
	case MailGiftType::Prefix_Totem_Lottery:
	{
		ToReceiveItemRefId = FItemRefIDHelper::GetPrefixTotemLotteryRefID(MailContent->totem_prefix_lottery_id);

		SetLotteryTicketIcon(ToReceiveItemRefId);
		break;
	}

	}

	if (CheckAdminMail(static_cast<MailSenderType>(MailContent->sender_type), MailContent->message))
	{
		SetContentAdminText(FText::FromString(MailContent->message));
	}
	else
	{
		if (MailContent->package_id > 0)
		{
			SetContentText(
				GetText_PackageName(MailContent->package_id),
				GetText_WhatGift(ToReceiveGift, ToReceiveGiftNum, ToReceiveItemRefId));
		}
		else
		{
			SetContentText(
				GetText_WhatCause(static_cast<MailWhatCause>(MailContent->source_type)),
				GetText_WhatGift(ToReceiveGift, ToReceiveGiftNum, ToReceiveItemRefId));
		}
	}
}

void UB2UIMailContent::SetContentText(const FText& Cause, const FText& Gift)
{
	if (RTB_MailContent.IsValid())
	{
		RTB_MailContent->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_Content")), Cause, Gift));
	}
}

bool UB2UIMailContent::CheckAdminMail(MailSenderType SenderType, const FString& Message)
{
	bool bAdmin = true;// SenderType == MailSenderType::Admin;
	bool HasMessage = Message.Len() > 0;
	return bAdmin && HasMessage;
}

void UB2UIMailContent::SetContentAdminText(const FText& Message)
{
	if (RTB_MailContent.IsValid())
	{
		RTB_MailContent->SetText(Message);
	}
}

UB2UIMailContent::ELeftTimeType UB2UIMailContent::GetLeftTimeType(int64 TimeSeconds)
{
	if (TimeSeconds <= 0)
	{
		// 만료
		return ELeftTimeType::Expiration;
	}
	else if (TimeSeconds < 60)
	{
		// ss초
		return ELeftTimeType::Sec;
	}

	else if (TimeSeconds < 3600)
	{
		// mm분 ss초
		return ELeftTimeType::Min_Sec;
	}

	else if (TimeSeconds < 86400)
	{
		// hh시간 mm분 ss초
		return ELeftTimeType::Hour_Min_Sec;
	}

	else if (TimeSeconds < 157680000)	// 50년
	{
		// dd일 hh시간 mm분
		return ELeftTimeType::Day_Hour_Min;
	}
	else
	{
		// 무제한
		return ELeftTimeType::Unlimited;
	}
}

void UB2UIMailContent::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	SetTime((ExpireTime - UB2GameInstance::GetUTCNow()).GetTotalSeconds());

	Super::NativeTick(MyGeometry, InDeltaTime);
}

FText UB2UIMailContent::GetText_WhatCause(MailWhatCause Cause)
{
	switch (Cause)
	{
	case MailWhatCause::Attendance:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_AttendReward"));
	case MailWhatCause::PlayTime:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_ConnectReward"));
	case MailWhatCause::EventAttendance:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_EventAttendReward"));
	case MailWhatCause::DailyMission:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_DailyMissionReward"));
	case MailWhatCause::WeeklyMission:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_WeeklyMissionReward"));
	case MailWhatCause::MonthlyMission:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_MonthlyMissionReward"));
	case MailWhatCause::SerialMission:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_NormalMissionReward"));
	case MailWhatCause::MissionCompletePointReward:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_MissionAchiveReward"));
	case MailWhatCause::DailyQuest:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_DailyQuestReward"));
	case MailWhatCause::MainQuest:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_MainQuestReward"));
	case MailWhatCause::SubQuest:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_SubQuestReward"));
	case MailWhatCause::PvpWin:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_TagMatchVictoryReward"));
	case MailWhatCause::PvpRewardBox:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_TagMatchDuelBoxReward"));
	case MailWhatCause::PvpRanking:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_TagMatchRankReward"));
	case MailWhatCause::CounterDungeonWin:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_CounterVictoryReward"));
	case MailWhatCause::CounterDungeonWeekly:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_CounterJoinReward"));
	case MailWhatCause::TeamMatchWin:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_TeamMatchVictoryReward"));
	case MailWhatCause::TeamMatchRewardBox:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_TeamMatchDuelBoxReward"));
	case MailWhatCause::TeamMatchRanking:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_TeamMatchRankReward"));
	case MailWhatCause::HeroTower:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_HeroTowerVictoryReward"));
	case MailWhatCause::SourceRaid:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_RaidVictoryReward"));
	case MailWhatCause::Tutorial:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_TutorialReward"));
	case MailWhatCause::AssaultWin:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_ControlMatchVictoryReward"));
	case MailWhatCause::AssaultRewardBox:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_ControlMatchDuelBoxReward"));
	case MailWhatCause::AssaultRanking:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_ControlMatchRankReward"));
	case MailWhatCause::AssaultMvp:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_ControlMatchMvpReward"));
	case MailWhatCause::GuildBattle:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_GuildJoinReward"));
	case MailWhatCause::GuildRewardBox:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_GuildBoxReward"));
	case MailWhatCause::GuildSupport:
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_GuildSupplyReward")), BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_Sender")), FText::FromString(CachedSenderName));
	case MailWhatCause::Friend:
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_Sender")), FText::FromString(CachedSenderName));
	case MailWhatCause::Collect:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_CollectBookReward"));
	case MailWhatCause::ActClear:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_ActClearReward"));
	case MailWhatCause::BuyShop:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_ShopPurchaseReward"));
	case MailWhatCause::ShopDrawMileage:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_ShopMileageReward1"));
	case MailWhatCause::ShopBuyMileage:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_ShopMileageReward2"));
	case MailWhatCause::Push:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_InspectReward"));
	case MailWhatCause::MainTenance:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_Maintenance"));
	case MailWhatCause::Event:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_EventReward"));
	case MailWhatCause::Admin:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_AdminReward"));
	case MailWhatCause::UnitySkill:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_UniteSkillReward"));
	case MailWhatCause::HeroTowerSweep:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_HeroTowerVictoryReward"));
	case MailWhatCause::Rest:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_RestReward"));
	case MailWhatCause::Coupon:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_Coupon"));
	case MailWhatCause::Promotion:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_Promotion"));
	case MailWhatCause::KakaoFriendInvite:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_KaKaoInviteReward"));
	case MailWhatCause::ShopFlatratePackageBonus:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_ShopFlatratePackageBonus"));
	case MailWhatCause::BuyMagicShop:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_MagicShopPurchase"));
	case MailWhatCause::FamePraiseReward:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_FamePraiseReward"));
	case MailWhatCause::FamePraiseRankingReward:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_FamePraiseRankingReward"));
	case MailWhatCause::FameRankerReward:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_FameRankerReward"));
	case MailWhatCause::BeginnerAttendanceReward:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_NewAttendReward"));
	case MailWhatCause::MakeDonationReward:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_DonationReward"));
	case MailWhatCause::DonationPointReward:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_DonationReward"));
	case MailWhatCause::DonationPointRankingReward:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_DonationReward"));
	case MailWhatCause::DimensionRankingReward:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_Dimension"));
	case MailWhatCause::FairyGift:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_FairyGift"));
	case MailWhatCause::NewAccountReward:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_NewAccountReward"));
	case MailWhatCause::GuildOut:
		return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatCause_GuildOut"));
	}

	return FText::FromString(FString::Printf(TEXT("")));
}

FText UB2UIMailContent::GetText_PackageName(int32 PackageId)
{
	FString TextString = TEXT("MailContent_PackageName_") + FString::FromInt(PackageId);
	return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TextString);
}

FText UB2UIMailContent::GetText_WhatGift(MailGiftType GiftType, int32 Num, int32 ItemTemplateId)
{
	switch (GiftType)
	{
	case MailGiftType::Gold:						// 1
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_Gold")), FText::AsNumber(Num));

	case MailGiftType::Gem:							// 2
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_Gem")), FText::AsNumber(Num));

	case MailGiftType::BladePoint:					// 4
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_BladePoint")), FText::AsNumber(Num));

	case MailGiftType::SocialPoint:					// 5
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_SocialPoint")), FText::AsNumber(Num));

	case MailGiftType::Selective_Item_Lottery:		// 8
	{
		if (ItemTemplateId == 8)					// 하드 코딩, 이후 테이블을 하나 생성하여 처리가 필요 함.
		{
			return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_RareUpgradeStoneTicket"));
		}
		else
		{
			return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_UpgradeStoneTicket"));
		}
	}

	case MailGiftType::Dark_Crystal:				// 11
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_DarkCrystal")), FText::AsNumber(Num));

	case MailGiftType::Item:						// 3
	case MailGiftType::Hero_Piece:					// 9
	case MailGiftType::Rank_Promotion_Point:		// 10
	case MailGiftType::Stage_Boost_Ticket:			// 13
	case MailGiftType::Hot_Time_Gold_Buff_Ticket:	// 14
	case MailGiftType::Hot_Time_Exp_Buff_Ticket:	// 15
	case MailGiftType::Shop_Ten_Lottery_Ticket:		// 16
	{
		auto* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable != nullptr)
			return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_Item")), ItemInfoTable->GetItemName(ItemTemplateId, 0), FText::AsNumber(Num));
	}

	case MailGiftType::LotteryTicket:				// 6	
	{
		int32 iItemNameTemplateId = BladeIIGameImpl::GetClientDataStore().GetGeneralLotteryItemTemplateId(ItemTemplateId);

		auto* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable != nullptr)
			return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_Item")), ItemInfoTable->GetItemName(iItemNameTemplateId, 0), FText::AsNumber(Num));
	}

	case MailGiftType::Selective_Lottery:			// 7
	{
		int32 iItemNameTemplateId = BladeIIGameImpl::GetClientDataStore().GetSelectiveLotteryItemTemplateId(ItemTemplateId);

		auto* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable != nullptr)
			return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_Item")), ItemInfoTable->GetItemName(iItemNameTemplateId, 0), FText::AsNumber(Num));
	}

	case MailGiftType::Fixed_Grade_Lottey:			// 12
	{
		int32 iItemNameTemplateId = BladeIIGameImpl::GetClientDataStore().GetFixedGradeLotteryItemTemplateId(ItemTemplateId);

		auto* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable != nullptr)
			return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_Item")), ItemInfoTable->GetItemName(iItemNameTemplateId, 0), FText::AsNumber(Num));
	}
	case MailGiftType::Fame_Point:
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HallOfFame_Reward")), FText::AsNumber(Num));
	}
	case MailGiftType::Selective_Ether_Lottery:
	{
		int32 EtherStar = 0;
		int32 EtherTier = 0;
		B2EtherManager::GetInstance().GetEtherLotteryInfo(ItemTemplateId, EtherStar, EtherTier);
		FText EtherName = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_MAILCONTENT), FString(TEXT("Lottery_SelectItemEtherType"))), EtherStar, EtherTier);
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_Item")), EtherName, FText::AsNumber(Num));
	}
	case MailGiftType::Prefix_Selective_Lottery:		// 19
	{
		int32 iItemNameTemplateId = BladeIIGameImpl::GetClientDataStore().GetPrefixSelectiveLotteryItemTemplateId(ItemTemplateId);

		auto* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable != nullptr)
			return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_Item")), ItemInfoTable->GetItemName(iItemNameTemplateId, 0), FText::AsNumber(Num));
	}
	case MailGiftType::Stage_Auto_Clear_Ticket:		// 20
	{
		auto* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable != nullptr)
			return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_Item")), ItemInfoTable->GetItemName(ItemTemplateId, 0), FText::AsNumber(Num));
	}
	case MailGiftType::Prefix_Totem_Lottery:
	{
		auto* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable != nullptr)
			return FText::Format(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailContent_WhatReward_Item")), ItemInfoTable->GetItemName(ItemTemplateId, 0), FText::AsNumber(Num));
	}
	}

	return BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT(""));
}

#include "Retailer.h"
#include "B2UIMail.h"
void UB2UIMailContent::OnClickedReceiveBTN()
{
	switch (ToReceiveGift)
	{
	case MailGiftType::Gold:						// 1
	case MailGiftType::Gem:							// 2
	case MailGiftType::Item:						// 3
	case MailGiftType::BladePoint:					// 4
	case MailGiftType::SocialPoint:					// 5
	case MailGiftType::Hero_Piece:					// 9
	case MailGiftType::Rank_Promotion_Point:		// 10
	case MailGiftType::Dark_Crystal:				// 11
	case MailGiftType::Stage_Boost_Ticket:			// 13
	case MailGiftType::Hot_Time_Gold_Buff_Ticket:	// 14
	case MailGiftType::Hot_Time_Exp_Buff_Ticket:	// 15
	case MailGiftType::Shop_Ten_Lottery_Ticket:		// 16
	case MailGiftType::Fame_Point:					// 17
	case MailGiftType::Stage_Auto_Clear_Ticket:		// 20
	case MailGiftType::Prefix_Totem_Lottery:		// 21
		if (MailBox)
		{
			if (MailBox->GetTouch() == false)
			{
				MailBox->SetTouch(true);
				return data_trader::Retailer::GetInstance().RequestOpenMail(MailId, 0, 0, 0);
			}
		}

	case MailGiftType::LotteryTicket:				// 6
	{
		EPCClass LotteryCharClass = BladeIIGameImpl::GetClientDataStore().GetGeneralLotteryCharClass(ToReceiveItemRefId);
		if (LotteryCharClass != EPCClass::EPC_End)
			return data_trader::Retailer::GetInstance().RequestOpenMail(MailId, CliToSvrPCClassType(LotteryCharClass), 0, 0);

		if (MailBox)
		{
			FText TitleText;
			int32 ItemNameTemplateId = BladeIIGameImpl::GetClientDataStore().GetGeneralLotteryItemTemplateId(ToReceiveItemRefId);
			auto* ItemInfoTable = StaticFindItemInfo();
			if (ItemInfoTable)
				TitleText = ItemInfoTable->GetItemName(ItemNameTemplateId, 0);

			MailBox->OpenGeneralLottery(MailId, TitleText);
		}
		return;
	}

	case MailGiftType::Selective_Lottery:			// 7
	{
		if (MailBox)
		{
			FText Grade;
			FText EquipCategory;
			FText TitleText;

			int ParamCount = 0;

			FB2LotteryTicketInfo* Info = BladeIIGameImpl::GetClientDataStore().GetSelelctiveLotteryInfo(ToReceiveItemRefId);
			if (Info)
			{
				int32 ItemNameTemplateId = BladeIIGameImpl::GetClientDataStore().GetSelectiveLotteryItemTemplateId(ToReceiveItemRefId);
				auto* ItemInfoTable = StaticFindItemInfo();
				if (ItemInfoTable)
					TitleText = ItemInfoTable->GetItemName(ItemNameTemplateId, 0);

				if (Info->CharClass == EPCClass::EPC_End)
				{
					MailBox->OpenAllSelectiveLottery(MailId, ToSelectItemRefIds, TitleText);
				}
				else
				{
					MailBox->OpenSelectiveLottery(MailId, ToSelectItemRefIds, TitleText);
				}
			}
		}
		return;
	}

	case MailGiftType::Selective_Item_Lottery:		// 8
	{
		if (MailBox)
		{
			MailBox->OpenSelectiveLotteryConsumeItem(MailId, ToSelectItemRefIds, ToSelectItemNums);
		}
		return;
	}

	case MailGiftType::Fixed_Grade_Lottey:			// 12
	{
		EPCClass LotteryCharClass = BladeIIGameImpl::GetClientDataStore().GetGeneralLotteryCharClass(ToReceiveItemRefId);
		if (MailBox)
		{
			FText TitleText;
			int32 ItemNameTemplateId = BladeIIGameImpl::GetClientDataStore().GetFixedGradeLotteryItemTemplateId(ToReceiveItemRefId);
			auto* ItemInfoTable = StaticFindItemInfo();
			if (ItemInfoTable)
				TitleText = ItemInfoTable->GetItemName(ItemNameTemplateId, 0);

			MailBox->OpenGeneralLottery(MailId, TitleText);
		}
		return;
	}

	case MailGiftType::Selective_Ether_Lottery:
	{
		UB2UIEtherLottery* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIEtherLottery>(UIFName::EtherLottery);
		Popup->SetEtherLotteryInfo(MailId, ToReceiveItemRefId);
		break;
	}
	case MailGiftType::Prefix_Selective_Lottery:
	{
		if (MailBox)
		{
			FB2LotteryTicketInfo* Info = BladeIIGameImpl::GetClientDataStore().GetPrefixSelelctiveLotteryInfo(ToReceiveItemRefId);
			if (Info)
			{
				FText TitleText;
				int32 ItemNameTemplateId = FItemRefIDHelper::GetPrefixSelectiveLotteryRefID(ToReceiveItemRefId);
				auto* ItemInfoTable = StaticFindItemInfo();
				if (ItemInfoTable)
					TitleText = ItemInfoTable->GetItemName(ItemNameTemplateId, 0);

				FItemSurpassLevel SurpassInfo = BladeIIGameImpl::GetClientDataStore().GetSurpassLevelData(Info->LotteryGrade);
				int32 InSurpassLevel = FMath::Clamp<int32>(Info->SurpassLevel, 0, SurpassInfo.MaxSurpassLevel);

				if (IsCostumeByEquipCategory(Info->EquipCategory))
				{
					MailBox->OpenPrefixSelectiveLotteryCostume(MailId, InSurpassLevel, ToSelectItemRefIds, TitleText);
				}
				else
				{
					MailBox->OpenPrefixSelectiveLottery(MailId, InSurpassLevel, ToSelectItemRefIds, TitleText);
				}
			}
		}
		return;
	}
	}
}

void UB2UIMailContent::GetEquipTypeText(int32 InEquipType, FText& InText, int32& InParam)
{
	switch (InEquipType)
	{
	case b2network::B2EquipCategory::WEAPON:
		InText = BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("LotteryEquip_Weapon"));
		InParam++;
		break;

	case b2network::B2EquipCategory::ARMOR:
		InText = BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("LotteryEquip_Armor"));
		InParam++;
		break;

	case b2network::B2EquipCategory::ACCESSORY:
		InText = BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("LotteryEquip_Acc"));
		InParam++;
		break;

	case b2network::B2EquipCategory::WEAPON_ESSENCE:
		InText = BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("LotteryEssence_Weapon"));
		InParam++;
		break;

	case b2network::B2EquipCategory::ARMOR_ESSENCE:
		InText = BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("LotteryEssence_Armor"));
		InParam++;
		break;

	case b2network::B2EquipCategory::ACCESSORY_ESSENCE:
		InText = BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("LotteryEssence_Acc"));
		InParam++;
		break;
	case b2network::B2EquipCategory::WEAPON_SURPASS_ANVIL:
		InText = BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("LotteryWeapon_SurpassAnvil"));
		InParam++;
		break;

	case b2network::B2EquipCategory::ARMOR_SURPASS_ANVIL:
		InText = BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("LotteryArmor_SurpassAnvil"));
		InParam++;
		break;

	case b2network::B2EquipCategory::ACCESSORY_SURPASS_ANVIL:
		InText = BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("LotteryAccessory_SurpassAnvil"));
		InParam++;
		break;

	default:
		InText = FText::FromString(TEXT(""));
	}
}

bool UB2UIMailContent::IsCostumeByEquipCategory(int32 InEquipCategory)
{
	if (InEquipCategory == b2network::B2EquipCategory::COSTUME_HAT ||
		InEquipCategory == b2network::B2EquipCategory::COSTUME_ARMOR ||
		InEquipCategory == b2network::B2EquipCategory::COSTUME_WEAPON)
	{
		return true;
	}
	else
	{
		return false;
	}
}


#include "B2UIDocHelper.h"
#include "B2LobbySceneManager.h"
void UB2UIMailContent::OpenedMail(const FB2ResponseOpenMailPtr& MailInfo)
{
	FB2MailLotteryItemInfoPtr& MailItemInfo = MailInfo->mail_item_info;

	

	//Todo
	// ~을 받았습니다. 팝업UI
	BII_CHECK(MailBox);
	if (MailBox)
	{
		switch (ToReceiveGift)
		{
		case MailGiftType::Gold:						// 1
		case MailGiftType::Gem:							// 2
		case MailGiftType::BladePoint:					// 4
		case MailGiftType::SocialPoint:					// 5
		case MailGiftType::Dark_Crystal:				// 11
		case MailGiftType::Fame_Point:				// 11
			return MailBox->OpenPopupReceiveOneMail(ToReceiveGift, ToReceiveGiftNum);
		
		case MailGiftType::Stage_Boost_Ticket:			// 13
		case MailGiftType::Hot_Time_Gold_Buff_Ticket:	// 14
		case MailGiftType::Hot_Time_Exp_Buff_Ticket:	// 15
		case MailGiftType::Shop_Ten_Lottery_Ticket:		// 16
		case MailGiftType::Stage_Auto_Clear_Ticket:		// 20
			return MailBox->OpenPopupReceiveOneMail(ToReceiveItemRefId, ToReceiveGiftNum);

		case MailGiftType::Item:						// 3
		case MailGiftType::Hero_Piece:					// 8
		case MailGiftType::Rank_Promotion_Point:		// 10
		{
			BII_CHECK(MailItemInfo->item.get());
			if (MailItemInfo->item_amount > 0 && MailItemInfo->item.get())
			{
				FB2Item AddItem;
				AddItem = MailItemInfo->item;
				MailBox->ClientDataStoreAddNewAcquiredItem(AddItem);
				MailBox->OpenPopupReceiveOneMail(ToReceiveItemRefId, ToReceiveGiftNum);
			}
			return;
		}

		case MailGiftType::LotteryTicket:				// 6	
		{
			BII_CHECK(MailItemInfo->item.get());
			if (MailItemInfo->item_amount > 0 && MailItemInfo->item.get())
			{
				FB2Item SummonedItem;
				SummonedItem = MailItemInfo->item;

				ESummonItemDifficulty SummonItemDifficulty = ESummonItemDifficulty::Normal;
				FB2LotteryTicketInfo* Info = BladeIIGameImpl::GetClientDataStore().GetGeneralLotteryInfo(ToReceiveItemRefId);
				if (Info)
				{
					switch (Info->LotteryGrade)
					{
					case b2network::B2LotteryGrade::NORMAL:
					case b2network::B2LotteryGrade::SPECIAL:
						SummonItemDifficulty = ESummonItemDifficulty::Normal;
						break;
					case b2network::B2LotteryGrade::PREMIUM:
						SummonItemDifficulty = ESummonItemDifficulty::Hard;
						break;
					case b2network::B2LotteryGrade::SIGNATURE:
						SummonItemDifficulty = ESummonItemDifficulty::VeryHard;
					}
				}

				if (auto* UIItemRaffle = UB2UIManager::GetInstance()->OpenUI<UB2UIItemRaffle>(UIFName::ItemRaffle, true))
				{
					UIItemRaffle->StartFromRaffle(SummonedItem);
				}
			}
			return;
		}

		case MailGiftType::Selective_Lottery:			// 7
		{
			BII_CHECK(MailItemInfo->item.get());
			if (MailItemInfo->item_amount > 0 && MailItemInfo->item.get() && MailBox)
			{
				FB2Item SelectedItem;
				SelectedItem = MailItemInfo->item;

				if (auto* UIItemRaffle = UB2UIManager::GetInstance()->OpenUI<UB2UIItemRaffle>(UIFName::ItemRaffle, true))
				{
					UIItemRaffle->StartFromRaffle(SelectedItem);
				}
			}
			return;
		}
		
		case MailGiftType::Selective_Item_Lottery:		// 8
		{
			BII_CHECK(MailItemInfo->item_amount > 0);
			if (MailItemInfo->item_amount > 0 && MailBox)
			{
				FB2Item AddItem;
				AddItem = MailItemInfo->item;
				MailBox->ClientDataStoreAddNewAcquiredItem(AddItem);
				MailBox->OpenPopupReceiveSelectiveLotteryConsumeItemResult(MailItemInfo->item_template_id, MailItemInfo->item_amount);
			}
			return;
		}

		case MailGiftType::Fixed_Grade_Lottey:		// 12
		{
			if (MailItemInfo->item_amount > 0 && MailItemInfo->item.get())
			{
				FB2Item SummonedItem;
				SummonedItem = MailItemInfo->item;

				ESummonItemDifficulty SummonItemDifficulty = ESummonItemDifficulty::Normal;
				FB2LotteryTicketInfo* Info = BladeIIGameImpl::GetClientDataStore().GetFixedLotteryInfo(ToReceiveItemRefId);

				if (auto* UIItemRaffle = UB2UIManager::GetInstance()->OpenUI<UB2UIItemRaffle>(UIFName::ItemRaffle, true))
				{
					UIItemRaffle->StartFromRaffle(SummonedItem);
				}
			}
			return;
		}
		case MailGiftType::Selective_Ether_Lottery:
		{
		}
		case MailGiftType::Prefix_Selective_Lottery:		// 19
		{
			BII_CHECK(MailItemInfo->item.get());
			BII_CHECK(MailItemInfo->costume.get());

			if (MailItemInfo->item_amount <= 0 && MailBox)
				return;

			FB2Item SelectedItem;

			if (MailItemInfo->item.get())
			{
				if (MailItemInfo->item->amount > 0)
				{
					SelectedItem = MailItemInfo->item;

					if (auto* UIItemRaffle = UB2UIManager::GetInstance()->OpenUI<UB2UIItemRaffle>(UIFName::ItemRaffle, true))
					{
						UIItemRaffle->StartFromRaffle(SelectedItem);
					}
					return;
				}
			}

			if (MailItemInfo->costume.get())
			{
				if (MailItemInfo->costume->amount > 0)
				{
					SelectedItem = *MailItemInfo->costume;

					if (auto* UICostumeRaffle = UB2UIManager::GetInstance()->OpenUI<UB2UICostumeRaffle>(UIFName::CostumeRaffle, true))
					{
						UICostumeRaffle->StartFromRaffle(SelectedItem);
					}
					return;
				}
			}
			return;
		}
		case MailGiftType::Prefix_Totem_Lottery:
			if (MailItemInfo->totem.get())
			{
				UB2UITotemRaffle* UIItemRaffle = UB2UIManager::GetInstance()->OpenUI<UB2UITotemRaffle>(UIFName::TotemRaffle, true);

				FB2Totem SelectedItem = MailInfo->totem;

				if (UIItemRaffle)
				{
					UIItemRaffle->StartFromRaffle(SelectedItem);
				}

			}
			return;
		}
	}
}

TArray<int32> UB2UIMailContent::GetSelectItemRefIDs()
{
	return ToSelectItemRefIds;
}
