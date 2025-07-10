#include "B2UIGuildMemberListInfo.h"
#include "B2UIGuildMemberSlot.h"
#include "B2UIManager.h"
#include "B2UIGuildMemberPart.h"
#include "Retailer.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2UIDocHelper.h"

UB2UIGuildMemberListInfo::~UB2UIGuildMemberListInfo()
{
	DeliveryReceiveAppointSubGuildMasterClass<FB2ReciveAppointSubGuildMasterInfo>::GetInstance().Unsubscribe(AppointSubGuildMasterTicket);
	DeliveryReceiveDismissSubGuildMasterClass<FB2ReciveDismissSubGuildMasterInfo>::GetInstance().Unsubscribe(DismissSubGuildMasterTicket);
	DeliveryReceiveKickGuildMemberClass<>::GetInstance().Unsubscribe(KickGuildMemberTicket);

	Issues.Empty();
}

void UB2UIGuildMemberListInfo::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	SetOpenState(EUIOpenState::Opened);

	SetGuildMemberList();

	if (UIP_Profile.IsValid())
	{
		UIP_Profile->Init();
		UIP_Profile->SetSlotText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyCharacterSimpleInfo_Detail")));
		UIP_Profile->SetGuildChangeType(EGuildChangeType::PROFILE, 0);
	}

	if (UIP_SecondMaster.IsValid())
	{
		UIP_SecondMaster->Init();
		UIP_SecondMaster->SetSlotText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_SecondMasterEntruct")));
	}

	if (UIP_MemberOut.IsValid())
	{
		UIP_MemberOut->Init();
		UIP_MemberOut->SetSlotText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Deportation")));
	}

	if (UIP_Master.IsValid())
	{
		UIP_Master->Init();
	}

	SetEventSubscribe();

}

void UB2UIGuildMemberListInfo::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (UIP_Profile.IsValid())
	{
		UIP_Profile->DestroySelf(InUIManager);
	}

	if (UIP_SecondMaster.IsValid())
	{
		UIP_SecondMaster->DestroySelf(InUIManager);
	}

	if (UIP_MemberOut.IsValid())
	{
		UIP_MemberOut->DestroySelf(InUIManager);
	}

	if (UIP_Master.IsValid())
	{
		UIP_Master->DestroySelf(InUIManager);
	}
}

void UB2UIGuildMemberListInfo::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2RichTextBlock, RTB_GuildMemberCount);
	GET_SLOT(UVerticalBox, VB_List);

	GET_SLOT(UB2UIGuildInfoMemberPart1, UIP_Profile);
	GET_SLOT(UB2UIGuildInfoMemberPart1, UIP_SecondMaster);
	GET_SLOT(UB2UIGuildInfoMemberPart1, UIP_MemberOut);
	GET_SLOT(UB2UIGuildInfoMemberPart1, UIP_Master);

	GET_SLOT(UB2ScrollBox, SB_ScrollBox);
	GET_SLOT(UCanvasPanel, CP_Popup);

	GET_SLOT(UOverlay, OV_PopupBG);
}

void UB2UIGuildMemberListInfo::BindDelegates()
{
	if (BTN_Close.IsValid())
		BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGuildMemberListInfo::OnClickClose)
}

void UB2UIGuildMemberListInfo::SetEventSubscribe()
{
	/*AppointSubGuildMasterTicket = DeliveryReceiveAppointSubGuildMasterClass<FB2ReciveAppointSubGuildMasterInfo>::GetInstance().Subscribe([this](const FB2ReciveDismissSubGuildMasterInfo& SubMasterInfo)
	{
		auto GuildMember = GET_TUPLE_DATA(FB2ResponseeAppointSubGuildMaster::changed_member_index, SubMasterInfo);

		BladeIIGameImpl::GetClientDataStore().SetMyGuildSubMaster(true, GuildMember->character_summary->account_id);
		SetGuildMemberList();


	});
	DismissSubGuildMasterTicket = DeliveryReceiveDismissSubGuildMasterClass<FB2ReciveDismissSubGuildMasterInfo>::GetInstance().Subscribe([this](const FB2ReciveDismissSubGuildMasterInfo& SubMasterInfo)
	{
		auto GuildMember = GET_TUPLE_DATA(FB2ResponseeDismissSubGuildMaster::changed_member_index, SubMasterInfo);

		BladeIIGameImpl::GetClientDataStore().SetMyGuildSubMaster(false, GuildMember->character_summary->account_id);
		SetGuildMemberList();
	});

	KickGuildMemberTicket = DeliveryReceiveKickGuildMemberClass<>::GetInstance().Subscribe([this](){

		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")), FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Notification_GuildBanish")), this->UIP_MemberOut->GetMemberName()),
			0, true, true, EUIMsgPopupButtonGroup::Confirm);

		BladeIIGameImpl::GetClientDataStore().SetMyGuildMemberOut(MemberID);
		SetGuildMemberList();
	});

	Issues.Add(DeliverytObtainGuildMasterClass<FB2ResponseObtainGuildMasterPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseObtainGuildMasterPtr& InData)
	{
		SetGuildInfo(InData->guild, InData->changed_members);
	}
	));

	Issues.Add(DeliverytHandOverGuildMasterClass<FB2ResponseHandOverGuildMasterPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseHandOverGuildMasterPtr& InData)
	{
		SetGuildInfo(InData->guild, InData->changed_members);
	}
	));*/
}

void UB2UIGuildMemberListInfo::SetGuildMemberList()
{
	VB_List->ClearChildren();

	const FGuildInfo& GuildInfo = BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo();

	RTB_GuildMemberCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildNumber")),
		FText::AsNumber(GuildInfo.GuildInfo->cur_member_count), FText::AsNumber(GuildInfo.GuildInfo->max_member_count)));

	int32 Rank = 1;
	int32 SlotCount = 0;
	for (auto Ele : GuildInfo.GuildMemberInfo)
	{
		
		auto MemberSlot = CreateWidget<UB2UIGuildMemberSlot>(GetOwningPlayer(), UIP_GuildMemberInfoSlot);
		check(MemberSlot)
		UVerticalBoxSlot* AddedSlot = Cast<UVerticalBoxSlot>(VB_List->AddChild(MemberSlot));

		MemberSlot->Init();
		MemberSlot->SetSlotIndex(SlotCount);
		//MemberSlot->SetupManualScrollBoxSender_byReceiver(this, SB_ScrollBox.Get());
		FGuildMemberChangeList Delegate;
		Delegate.BindUObject(this, &UB2UIGuildMemberListInfo::SetScondPopup);
		MemberSlot->SetChangeDelegate(Delegate);
		if (Ele->contribution != 0)
		{
			MemberSlot->SetGuildMember(Ele, Rank);
			++Rank;
		}
		else
			MemberSlot->SetGuildMember(Ele, 0);

		++SlotCount;
	}
	CP_Popup->SetVisibility(ESlateVisibility::Hidden);
}

bool UB2UIGuildMemberListInfo::IsLongTermUnconnect(int64 InMemberID)
{
	bool Result = false;

	FTimespan LastLogoutTime = BladeIIGameImpl::GetClientDataStore().GetGuildMemberLastLogoutTime(InMemberID);
	int32 PeriodDay = BladeIIGameImpl::GetClientDataStore().GetGuildMasterLogoutLimitPeriodDay();
	FTimespan GuildMasterLogoutLimitPeriodDay = FTimespan(PeriodDay, 0, 0, 0);
	
	if (LastLogoutTime > GuildMasterLogoutLimitPeriodDay)
	{
		Result = true;
	}

	return Result;
}

void UB2UIGuildMemberListInfo::SetGuildInfo(FB2ResponseGuildInfoPtr InGuildInfo, TArray<b2network::B2GuildMemberPtr> InGuildMemberInfo)
{
	UB2UIDocGuild* GuildDoc = UB2UIDocHelper::GetDocGuild();
	if (GuildDoc)
	{
		GuildDoc->SetJoinGuildBattle(InGuildInfo->join_guild_battle);
		GuildDoc->SetGuildLv(InGuildInfo->level);
		GuildDoc->SetGuildExp(InGuildInfo->exp);
		GuildDoc->SetObtainMedal(InGuildInfo->turn_obtain_medal);
	}

	BladeIIGameImpl::GetClientDataStore().SetMyGuildInfo(InGuildInfo);
	BladeIIGameImpl::GetClientDataStore().UpdateMyGuildGuildMemberInfo(InGuildMemberInfo);
	SetGuildMemberList();
}

void UB2UIGuildMemberListInfo::SetScondPopup(int32 GuildId, int32 Rank, int32 Index, int32 YPos)
{
	//const FVector2D ScreenSize = GEngine->GameViewport->Viewport->GetSizeXY();
	//UCanvasPanelSlot* PopupSlot = Cast<UCanvasPanelSlot>(OV_PopupBG->Slot);
	//UOverlaySlot* CPSlot = Cast<UOverlaySlot>(SB_ScrollBox->Slot);

	//float PopupYPos = 0.0f;

	//check(CPSlot);
	//check(PopupSlot);

	//	//UScrollBox
	//
	//MemberID = GuildId;

	//UIP_SecondMaster->SetVisibility(ESlateVisibility::Visible);
	//UIP_MemberOut->SetVisibility(ESlateVisibility::Visible);
	//UIP_Master->SetVisibility(ESlateVisibility::Collapsed);

	//FGuildUesrClick UserDelegate;
	//int32 MyRank = BladeIIGameImpl::GetClientDataStore().GetUserGuildInfo().GuildRank;
	//UIP_Profile->SetGuildChangeType(EGuildChangeType::PROFILE, MemberID);

	//if (Rank == b2network::B2GuildMemberRank::MASTER || MyRank == b2network::B2GuildMemberRank::NORMAL)
	//{
	//	UIP_SecondMaster->SetVisibility(ESlateVisibility::Collapsed);
	//	UIP_MemberOut->SetVisibility(ESlateVisibility::Collapsed);

	//	if (Rank == b2network::B2GuildMemberRank::MASTER && MyRank == b2network::B2GuildMemberRank::SUB_MASTER)
	//	{
	//		int64 MasterID = BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->master_account_id;
	//		if (IsLongTermUnconnect(MasterID) == true)
	//		{
	//			UIP_Master->SetVisibility(ESlateVisibility::Visible);
	//			UIP_Master->SetSlotText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildMasterTakeOver")));
	//			UIP_Master->SetGuildChangeType(EGuildChangeType::OBTAIN, MemberID);
	//		}
	//	}
	//}
	//else
	//{
	//	if (MyRank == b2network::B2GuildMemberRank::MASTER)
	//	{
	//		if (IsLongTermUnconnect(MemberID) == false)
	//		{
	//			PopupYPos = 70.0f;
	//			UIP_Master->SetVisibility(ESlateVisibility::Visible);
	//			UIP_Master->SetSlotText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildMasterAuthorize")));
	//			UIP_Master->SetGuildChangeType(EGuildChangeType::HANDOVER, MemberID);
	//		}

	//		if (Rank == b2network::B2GuildMemberRank::SUB_MASTER)
	//		{
	//			UIP_SecondMaster->SetSlotText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_SecondMasterRemove")));
	//			UIP_SecondMaster->SetGuildChangeType(EGuildChangeType::DISMISS, MemberID);
	//		}
	//		else if (Rank == b2network::B2GuildMemberRank::NORMAL)
	//		{
	//			UIP_SecondMaster->SetSlotText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_SecondMasterEntruct")));
	//			UIP_SecondMaster->SetGuildChangeType(EGuildChangeType::APPOINT, MemberID);
	//		}
	//	}
	//	else if (MyRank == b2network::B2GuildMemberRank::SUB_MASTER)
	//	{
	//		UIP_SecondMaster->SetVisibility(ESlateVisibility::Collapsed);

	//		if (Rank == b2network::B2GuildMemberRank::SUB_MASTER)
	//		{
	//			UIP_MemberOut->SetVisibility(ESlateVisibility::Collapsed);
	//		}
	//	}

	//	UIP_MemberOut->SetGuildChangeType(EGuildChangeType::KICK, MemberID);
	//}
	//
	//float ypos = CPSlot->Padding.Top - SB_ScrollBox->GetScrollOffset() + (Index * YPos) + ((ScreenSize.Y / 2) + PopupSlot->GetOffsets().Top) - PopupYPos;

	//FMargin PopupMargin = Cast<UCanvasPanelSlot>(CP_Popup->Slot)->GetOffsets();

	//PopupMargin.Top = ypos;
	//Cast<UCanvasPanelSlot>(CP_Popup->Slot)->SetOffsets(PopupMargin);

	//CP_Popup->SetVisibility(ESlateVisibility::Visible);
}

void UB2UIGuildMemberListInfo::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::MyGuildMemeberInfo);

}

void UB2UIGuildMemberListInfo::SendMemberProfile()
{
	///프로필 아직없음
}

void UB2UIGuildMemberListInfo::SendSecondMasterDismissal()
{
	data_trader::Retailer::GetInstance().RequestAppointSubGuildMaster(MemberID);
}

void UB2UIGuildMemberListInfo::SendSecondMasterEntrust()
{
	data_trader::Retailer::GetInstance().RequestDismissSubGuildMaster(MemberID);
}

void UB2UIGuildMemberListInfo::SendMemberOut()
{
	data_trader::Retailer::GetInstance().RequestKickGuildMember(MemberID);
}

FReply UB2UIGuildMemberListInfo::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	CP_Popup->SetVisibility(ESlateVisibility::Hidden);

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}
