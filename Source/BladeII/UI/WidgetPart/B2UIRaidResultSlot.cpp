// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRaidResultSlot.h"
#include "B2UIDocHelper.h"
#include "B2UIDocRaid.h"
#include "B2ItemInfo.h"

#include "B2UIRewardEventSet.h"


#include "BladeIIUtil.h"
#include "B2UIUserReport.h"
#include "B2UIManager.h"

void UB2UIRaidResultSlot::Init()
{
	Super::Init();

	CAPTURE_UOBJECT(UB2UIRaidResultSlot);

	UserReportTicket = DeliveryReportUserClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UserReport)
		Capture->UserReportComplete();
		END_CAPTURE_OBJECT()
		);

}

void UB2UIRaidResultSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Character);
	GET_SLOT(UImage, IMG_MyBG);
	GET_SLOT(UImage, IMG_BG);
	GET_SLOT(UImage, IMG_Lose);

	GET_SLOT(UTextBlock, RTB_RankerName);
	GET_SLOT(UTextBlock, RTB_RankerLevel);
	GET_SLOT(UTextBlock, RTB_RankerPercent);
	GET_SLOT(UTextBlock, TB_ContributionGuide);

	GET_SLOT(UTextBlock, STB_GuildMan);
	GET_SLOT(UPanelWidget, P_GuildMan);

	GET_SLOT(UTextBlock, RTB_RewardNoticeMultiline);
	GET_SLOT(UB2RichTextBlock, RTB_RewardNoticeSingleline);

	GET_SLOT(UScaleBox, p_RewardExp);
	GET_SLOT(UScaleBox, p_RewardItem);
	GET_SLOT(UScaleBox, p_RewardStep);
	GET_SLOT(UB2UIRewardIcon, RewardItem0);
	GET_SLOT(UB2UIRewardIcon, RewardExp);
	GET_SLOT(UB2UIRewardIcon, RewardStep);
	GET_SLOT(UWidgetSwitcher, WS_Switcher);

	GET_SLOT(UB2Button, BTN_UserReport);


	if (RewardItem0.IsValid())
		RewardItem0->Init();
	if (RewardExp.IsValid())
		RewardExp->Init();
	if (RewardExp.IsValid())
		RewardExp->Init();
	if (RewardStep.IsValid())
		RewardStep->Init();

	Numbers.SetNum(4);
	GET_SLOT_BYNAME(UImage, Numbers[0], IMG_RankBG_1);
	GET_SLOT_BYNAME(UImage, Numbers[1], IMG_RankBG_2);
	GET_SLOT_BYNAME(UImage, Numbers[2], IMG_RankBG_3);
	GET_SLOT_BYNAME(UImage, Numbers[3], IMG_RankBG_4);
}

void UB2UIRaidResultSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_UserReport, &UB2UIRaidResultSlot::OnClickUserRepot);
}

void UB2UIRaidResultSlot::InitRankSlot(int32 Rank)
{
	RankIndex = Rank;
}

void UB2UIRaidResultSlot::UpdateStaticText()
{
	if (TB_ContributionGuide.IsValid())
		TB_ContributionGuide->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_Contribution")));
	if (STB_GuildMan.IsValid())
		STB_GuildMan->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_guildmembers"))));
}

void UB2UIRaidResultSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	DeliveryReportUserClass<>::GetInstance().Unsubscribe(UserReportTicket);
}

void UB2UIRaidResultSlot::OnClickUserRepot()
{
	UB2UIUserReport* UserReportPopup = UB2UIManager::GetInstance()->OpenUI<UB2UIUserReport>(UIFName::UserReportPopup);

	if (UserReportPopup)
		UserReportPopup->UpdatePopup(b2network::B2ReportModeType::RAID, AccountID, RTB_RankerName->GetText().BuildSourceString());


}

void UB2UIRaidResultSlot::UpdateSlot(const FRaidResultSlotInfo &SlotInfo)
{
	if (SlotInfo.Level < 1) 
	{
		this->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	SetRankNumber(SlotInfo.Number, SlotInfo.SlotState);
	SetRankerName(SlotInfo.Name);
	SetRankerLevel(SlotInfo.Level);
	SetRankerPercent(SlotInfo.ContributionPercent);

	bIsMy = SlotInfo.IsMy;

	AccountID = SlotInfo.AccountID;

	if (BTN_UserReport.IsValid())
		BTN_UserReport->SetVisibility(bIsMy ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	if (IMG_MyBG.IsValid())
	{
		ESlateVisibility VisibleState = SlotInfo.IsMy ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		IMG_MyBG->SetVisibility(VisibleState);
	}
	
	if(IMG_BG.IsValid())
	{
		ESlateVisibility VisibleState = SlotInfo.IsMy ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
		IMG_BG->SetVisibility(VisibleState);
	}

	int32 PcIndex = PCClassToInt(SlotInfo.PCIndex);
	if (CharterTexture.IsValidIndex(PcIndex) && IMG_Character.IsValid())
		IMG_Character->SetBrushFromMaterial(CharterTexture[PcIndex]);

	SetResultExpReward(SlotInfo.IsMy ? FText::AsNumber(SlotInfo.GetExp) : FText::GetEmpty());

	if (P_GuildMan.IsValid())
		P_GuildMan->SetVisibility(SlotInfo.IsMyGuildMan ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	switch (SlotInfo.SlotState)
	{
	case ERaidResultSlotState::Win:
	{
		if (SlotInfo.ContributionPercent > 1)
		{
			FText RewardCountText = SlotInfo.IsMy ? FText::AsNumber(SlotInfo.RewardCount) : FText::GetEmpty();
			SetResultReward(SlotInfo.RewardID, RewardCountText, SlotInfo.RewardGrade, SlotInfo.IsMainUsedRewardID);

			FText StepRewardCountText = SlotInfo.IsMy ? FText::AsNumber(SlotInfo.StepRewardCount) : FText::GetEmpty();
			bool IsMyAndGuildParty = (SlotInfo.IsMy && SlotInfo.IsGuildParty);
			SetResultStepReward(SlotInfo.StepRewardID, StepRewardCountText, SlotInfo.StepRewardGrade, IsMyAndGuildParty);
		}
		else
			SetResultNotEnoughContribution();
	}
	break;
	case ERaidResultSlotState::Lose:
	{
		SetResultFail();
	}
	break;
	case ERaidResultSlotState::Giveup:
	{
		SetResultGiveup();
	}
	break;
	default:
		break;
	}
}

void UB2UIRaidResultSlot::SetRankNumber(int32 Number, ERaidResultSlotState State)
{
	if (State == ERaidResultSlotState::Lose || State == ERaidResultSlotState::Giveup)
	{
		if (IMG_Lose.IsValid())
		{
			IMG_Lose->SetVisibility(ESlateVisibility::Visible);
		}

		for (auto Each : Numbers)
		{
			if (Each.IsValid())
			{
				Each->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	else
	{
		if (IMG_Lose.IsValid())
		{
			IMG_Lose->SetVisibility(ESlateVisibility::Hidden);
		}

		int32 i = 0;
		for (auto Each : Numbers)
		{
			if (Each.IsValid())
			{
				bool IsShow = i == (Number - 1);
				ESlateVisibility VisibleState = IsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
				Each->SetVisibility(VisibleState);
			}

			++i;
		}
	}
	
}

void UB2UIRaidResultSlot::SetRankerName(FText Name)
{
	if (RTB_RankerName.IsValid())
	{
		RTB_RankerName->SetText(Name);
	}
}

void UB2UIRaidResultSlot::SetRankerLevel(int32 Level)
{
	if (RTB_RankerLevel.IsValid())
	{
		RTB_RankerLevel->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Hero_Level"))), FText::AsNumber(Level)));
	}
}

void UB2UIRaidResultSlot::SetRankerPercent(float Percent)
{
	if (RTB_RankerPercent.IsValid())
	{
		int32 ConvertPercent = (int32)Percent;
		RTB_RankerPercent->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Raid_ResultContributionPercent"))), FText::AsNumber(ConvertPercent)));
	}
}

void UB2UIRaidResultSlot::SetRankerCharacter(UMaterialInstance* pIconMI)
{
	if (IMG_Character.IsValid())
		IMG_Character->SetBrushFromMaterial(pIconMI);
}

void UB2UIRaidResultSlot::SetResultReward(const int32 Index, const FText& Text, const int32 nGrade, const bool bUsedRewardID)
{
	if (WS_Switcher.IsValid())
		WS_Switcher->SetActiveWidgetIndex(static_cast<int32>(ERewardVisibleState::RewardItem));

	if (RewardItem0.IsValid() && p_RewardItem.IsValid() && p_RewardExp.IsValid())
	{
		p_RewardItem->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		if(bUsedRewardID)
			RewardItem0->UpdateRewardIcon(Index, Text);
		else
			RewardItem0->UpdateRewardIconByItemIndex(Index, Text);

		RewardItem0->SetGrade(nGrade);
	}
}

void UB2UIRaidResultSlot::SetResultExpReward(const FText& Text)
{
	if (WS_Switcher.IsValid())
		WS_Switcher->SetActiveWidgetIndex(static_cast<int32>(ERewardVisibleState::RewardItem));

	if (RewardExp.IsValid() && p_RewardExp.IsValid())
	{
		if (!Text.IsEmpty())
		{
			p_RewardExp->SetVisibility(ESlateVisibility::HitTestInvisible);
			RewardExp->UpdateRewardIconByItemIndex(FItemRefIDHelper::GetGoodsID_EXP(), Text);
		}
	}
}

void UB2UIRaidResultSlot::SetResultStepReward(const int32 RewardID, const FText& Text, const int32 nGrade, const bool bMyGuildMan)
{
	if (RewardStep.IsValid() && RewardStep.IsValid())
	{
		p_RewardStep->SetVisibility(ESlateVisibility::HitTestInvisible);
		RewardStep->UpdateRewardIcon(RewardID, Text);
		RewardStep->SetGrade(nGrade);
		RewardStep->SetCountColor(bMyGuildMan ? UB2UIManager::GetInstance()->TextColor_Gold : FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UB2UIRaidResultSlot::SetResultGiveup()
{
	if (WS_Switcher.IsValid())
		WS_Switcher->SetActiveWidgetIndex(static_cast<int32>(ERewardVisibleState::MultiLine));

	if (RTB_RewardNoticeMultiline.IsValid())
		RTB_RewardNoticeMultiline->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_ResultGiveupDesc")));
}

void UB2UIRaidResultSlot::SetResultFail()
{
	if (WS_Switcher.IsValid())
		WS_Switcher->SetActiveWidgetIndex(static_cast<int32>(ERewardVisibleState::SingleLine));

	if (RTB_RewardNoticeSingleline.IsValid())
		RTB_RewardNoticeSingleline->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_ResultNoReward")));

}

void UB2UIRaidResultSlot::SetResultNotEnoughContribution()
{
	if (WS_Switcher.IsValid())
		WS_Switcher->SetActiveWidgetIndex(static_cast<int32>(ERewardVisibleState::MultiLine));

	if (RTB_RewardNoticeMultiline.IsValid())
		RTB_RewardNoticeMultiline->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_ResultNotEnoughContribution")));
}

void UB2UIRaidResultSlot::UserReportComplete()
{
	auto DocSome = UB2UIDocHelper::GetDocSome();

	if (DocSome && DocSome->GetSendReportUserAccountID() == AccountID)
	{

		if (BTN_UserReport.IsValid())
			BTN_UserReport->SetIsEnabled(false);

		UB2UIManager::GetInstance()->CloseUI(UIFName::UserReportPopup);
		DevShowNotReadyYetMessage(TEXT("General_ReportComplete"));
	}
}

void UB2UIRaidResultSlot::ShowBonusItem()
{
	ShowBonusAnim();
}

void UB2UIRaidResultSlot::ShowBonusAnim()
{
	PlayBonusItemAnim();
}