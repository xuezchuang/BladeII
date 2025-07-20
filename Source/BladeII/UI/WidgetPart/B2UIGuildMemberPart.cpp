#include "B2UIGuildMemberPart.h"
#include "Retailer.h"
#include "B2UIManager.h"
#include "B2UIGuildUserMsg.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIGuildInfoMemberPart::CacheAssets()
{
	GET_SLOT(UB2Image, IMG_Character)
		GET_SLOT(UB2Image, IMG_Rank)
		GET_SLOT(UB2RichTextBlock, RTB_NickName)
		GET_SLOT(UB2RichTextBlock, RTB_Lv)
		GET_SLOT(UTextBlock, TB_CombatPower)
}

void UB2UIGuildInfoMemberPart::BindDelegates()
{

}

void UB2UIGuildInfoMemberPart::SetNickName(const FString& NickName)
{
	if (RTB_NickName.IsValid())
		RTB_NickName->SetText(FText::FromString(NickName));
}

void UB2UIGuildInfoMemberPart::SetCombat(int32 Combat)
{
	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(Combat));
}

void UB2UIGuildInfoMemberPart::SetCharacterType(EPCClass PCClass)
{
	SetHeroImage_BP(static_cast<int32>(PCClass));

}

void UB2UIGuildInfoMemberPart::SetCharacterLv(int32 Lv)
{
	if (RTB_Lv.IsValid())
		RTB_Lv->SetText(FText::Format((BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev"))), FText::AsNumber(Lv)));
}

void UB2UIGuildInfoMemberPart::SetRank(int32 Rank)
{
	if (!IMG_Rank.IsValid())
		return;


	/* 0. 辨付 1 何辨付 炼盔甸篮 见败咯*/

	if (Rank == b2network::B2GuildMemberRank::MASTER)
	{
		IMG_Rank->SetBrushFromMaterial(RankImage[0]);
	}
	else if (Rank == b2network::B2GuildMemberRank::SUB_MASTER)
	{
		IMG_Rank->SetBrushFromMaterial(RankImage[1]);
	}
	else
		IMG_Rank->SetVisibility(ESlateVisibility::Hidden);

}

void UB2UIGuildInfoMemberPart1::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Select)
		GET_SLOT(UB2RichTextBlock, RTB_Text)
}

void UB2UIGuildInfoMemberPart1::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIGuildInfoMemberPart1::OnClickSelect)
}

void UB2UIGuildInfoMemberPart1::SetSlotText(FText Text)
{
	if (RTB_Text.IsValid())
		RTB_Text->SetText(Text);
}

void UB2UIGuildInfoMemberPart1::SetDelegate(FGuildUesrClick Memberdelegate)
{
	GuildUserDelegate = Memberdelegate;
}

void UB2UIGuildInfoMemberPart1::SetGuildChangeType(EGuildChangeType Type, int64 ID)
{
	ChangeType = Type;
	MemberID = ID;
}

FText UB2UIGuildInfoMemberPart1::GetMemberName()
{
	FText MemberName;
	const FGuildInfo& GuildInfo = BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo();

	for (auto Ele : GuildInfo.GuildMemberInfo)
	{
		if (Ele->character_summary->account_id == MemberID)
		{
			MemberName = FText::FromString(Ele->character_summary->account_name);
			break;
		}
	}

	return MemberName;
}

void UB2UIGuildInfoMemberPart1::OnClickSelect()
{
	if (ChangeType == EGuildChangeType::PROFILE)
	{
		data_trader::Retailer::GetInstance().RequestFindAccount(MemberID);			//扑诀绝捞 官肺 剁况霖促
	}
	else
	{
		int32 MatserDay = BladeIIGameImpl::GetClientDataStore().GetGuildMasterChangePeriodDay();
		FTimespan MasterTime = FTimespan(MatserDay, 0, 0, 0);
		FTimespan ElectedTime = BladeIIGameImpl::GetClientDataStore().GetMasterElectedTime();
		if (ChangeType == EGuildChangeType::HANDOVER && ElectedTime < MasterTime)
		{
			FTimespan RemaindTime = MasterTime - ElectedTime;
			FText PopupText = FText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_DelegateGuildMasterTime")), FText::AsNumber(RemaindTime.GetHours()), FText::AsNumber(RemaindTime.GetMinutes())));

			UB2UIManager::GetInstance()->OpenMsgPopup(
				EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")),
				PopupText,
				0,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm);
		}
		else
		{
			const FGuildInfo& GuildInfo = BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo();

			b2network::B2GuildMemberPtr MemberInfo;
			for (auto Ele : GuildInfo.GuildMemberInfo)
			{
				if (Ele->character_summary->account_id == MemberID)
				{
					MemberInfo = Ele;
					break;
				}
			}

			FMsgPopupOnClick	Delegate;
			FText ContentText;
			switch (ChangeType)
			{
			case EGuildChangeType::APPOINT:
				ContentText = FText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_SecondMasterEntructMsg")), FText::FromString(MemberInfo->character_summary->account_name)));
				Delegate.BindLambda([this]() { data_trader::Retailer::GetInstance().RequestAppointSubGuildMaster(MemberID); });
				break;
			case EGuildChangeType::DISMISS:
				ContentText = FText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_SecondMasterRemoveMsg")), FText::FromString(MemberInfo->character_summary->account_name)));
				Delegate.BindLambda([this]() { data_trader::Retailer::GetInstance().RequestDismissSubGuildMaster(MemberID); });
				break;
			case EGuildChangeType::KICK:
				ContentText = FText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_SecondMasterDeportationMsg")), FText::FromString(MemberInfo->character_summary->account_name)));
				Delegate.BindLambda([this]() { data_trader::Retailer::GetInstance().RequestKickGuildMember(MemberID); });
				break;
			case EGuildChangeType::OBTAIN:
				ContentText = FText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildMasterTakeOverInfo")));
				Delegate.BindLambda([this]() { data_trader::Retailer::GetInstance().RequesttObtainGuildMaster(); });
				break;
			case EGuildChangeType::HANDOVER:
				ContentText = FText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_DelegateGuildMaster")), FText::FromString(MemberInfo->character_summary->account_name)));
				Delegate.BindLambda([this]() {data_trader::Retailer::GetInstance().RequestHandOverGuildMaster(MemberID); });
				break;
			}

			UB2UIManager::GetInstance()->OpenMsgPopup(
				EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")), 
				ContentText,
				0, 
				true, 
				true, 
				EUIMsgPopupButtonGroup::YesOrNo, 
				Delegate);
		}
	}
}

