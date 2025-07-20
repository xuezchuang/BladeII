// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIGuildMatchPopup.h"
#include "B2UIManager.h"

#include "B2UIGuildSchedule.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIGuildMatchPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UB2Button, BTN_Cancel);
	GET_SLOT(UB2Button, BTN_Cancel2);
	GET_SLOT(UCanvasPanel,CP_GuildMatchRequest)
	GET_SLOT(UCanvasPanel,CP_ImpossibleMsg)
	GET_SLOT(UCanvasPanel,CP_GuildMatch)
	GET_SLOT(UCanvasPanel, CP_Btns)

	GET_SLOT(UB2RichTextBlock, RTB_GuildMatch)

	GET_SLOT(UB2RichTextBlock , RTB_GuildMemberCount)

	GET_SLOT(UB2RichTextBlock, RTB_ErrMsg)
	GET_SLOT(UB2UIGuildSchedule , UIP_GuildSchedule)
	if(UIP_GuildSchedule.IsValid())
		UIP_GuildSchedule->Init();

	GET_SLOT(UB2RichTextBlock, RTB_GuildStateAccout);

	GET_SLOT(UB2RichTextBlock, RTB_ImpossibleMsg);

	GET_SLOT(UTextBlock, TB_BtnPositive)
}
void UB2UIGuildMatchPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIGuildMatchPopup::OnClickBTN_Confirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2UIGuildMatchPopup::OnClickBTN_Cancel);
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel2, &UB2UIGuildMatchPopup::OnClickBTN_Cancel);
}

void UB2UIGuildMatchPopup::CloseWidgetDelegate()
{
	OnClickBTN_Cancel();
}

void UB2UIGuildMatchPopup::Init()
{
	Super::Init();
	SetGuildSchedule();
}

void UB2UIGuildMatchPopup::DestroySelf(UB2UIManager* InUIManager)
{


	Super::DestroySelf(InUIManager);
}

void UB2UIGuildMatchPopup::SetGuildSchedule()
{
	if (UIP_GuildSchedule.IsValid())
		UIP_GuildSchedule->SetSchedule(BladeIIGameImpl::GetClientDataStore().GetGuildBattleScheduleInfo());
}

void UB2UIGuildMatchPopup::SetVisibilityPopup(bool InValue)
{
	if (InValue)
	{
		SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	} 
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
}

#define GUILDMATCHSTATE(visible, hidden, hidden2)						\
	visible->SetVisibility(ESlateVisibility::Visible);					\
	hidden->SetVisibility(ESlateVisibility::Hidden);					\
	hidden2->SetVisibility(ESlateVisibility::Hidden);					\


void UB2UIGuildMatchPopup::OpenPopup(int32 GuildState, int32 GuildPosition)
{
	CP_Btns->SetVisibility(ESlateVisibility::Visible);
	BTN_Cancel2->SetVisibility(ESlateVisibility::Visible);

	auto& DataStore = BladeIIGameImpl::GetClientDataStore();
	int32 GuildNumber = DataStore.GetMyGuildInfo().GuildMemberInfo.Num();

	if (!RTB_GuildStateAccout.IsValid())
		return;

	if (b2network::B2GuildBattleStateType::NONE == GuildState ||
		b2network::B2GuildBattleStateType::INIT == GuildState ||
		b2network::B2GuildBattleStateType::INITDONE == GuildState)
	{
		GUILDMATCHSTATE(CP_ImpossibleMsg, CP_GuildMatch, CP_GuildMatchRequest);
		RTB_ImpossibleMsg->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleRequestReadyStatePopupMessage")));
		CP_Btns->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	else if (b2network::B2GuildBattleStateType::READY == GuildState)
	{
		if (BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->join_guild_battle)
		{
			GUILDMATCHSTATE(CP_ImpossibleMsg, CP_GuildMatch, CP_GuildMatchRequest);
			RTB_ImpossibleMsg->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleRequestCompleteStatePopupMessage")));
			CP_Btns->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}

		if (b2network::B2GuildMemberRank::NORMAL == GuildPosition)		//何辨付唱 辨付啊 酒匆锭 脚没给窃
		{
			GUILDMATCHSTATE(CP_ImpossibleMsg, CP_GuildMatchRequest, CP_GuildMatch);
			RTB_ImpossibleMsg->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleRequestStatePopupMessageMember")));
			CP_Btns->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			GUILDMATCHSTATE(CP_GuildMatchRequest, CP_ImpossibleMsg, CP_GuildMatch);
			BTN_Cancel2->SetVisibility(ESlateVisibility::Hidden);

			RTB_GuildMemberCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_MatchGuildMemeberInfo")), FText::AsNumber(GuildNumber)));
		}
	}
	else if (b2network::B2GuildBattleStateType::MATCHING == GuildState ||
		b2network::B2GuildBattleStateType::MATCHINGDONE == GuildState)
	{
		GUILDMATCHSTATE(CP_GuildMatch, CP_ImpossibleMsg, CP_GuildMatchRequest);
		CP_Btns->SetVisibility(ESlateVisibility::Collapsed);
		RTB_GuildStateAccout->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleReadyStatePopupMessage")));
	}
	else if (b2network::B2GuildBattleStateType::SETTLEMENT == GuildState ||
		b2network::B2GuildBattleStateType::SETTLEMENTDONE == GuildState)
	{
		GUILDMATCHSTATE(CP_ImpossibleMsg, CP_GuildMatch, CP_GuildMatchRequest);
		CP_Btns->SetVisibility(ESlateVisibility::Collapsed);
		RTB_ImpossibleMsg->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleEndReadyStatePopupMessage")));
	}

}

void UB2UIGuildMatchPopup::SetDelegate(const FGuildMatchPopup& Delegate)
{
	GuildMatchPopupDelegate = Delegate;
}

void UB2UIGuildMatchPopup::OnClickBTN_Confirm()
{
	GuildMatchPopupDelegate.ExecuteIfBound();
	SetVisibilityPopup(false);
}

void UB2UIGuildMatchPopup::OnClickBTN_Cancel()
{
	SetVisibilityPopup(false);
}


