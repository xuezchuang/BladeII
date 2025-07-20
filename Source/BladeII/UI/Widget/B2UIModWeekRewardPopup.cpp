#include "B2UIModWeekRewardPopup.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "BladeIIGameImpl.h"


void UB2UIModWeekRewardPopup::Init()
{
	Super::Init();

	//OpenSetting();
}

void UB2UIModWeekRewardPopup::CacheAssets()
{
	GET_SLOT(UB2UIGradeRankAndStar, UIP_LargeGrade)
	if (UIP_LargeGrade.IsValid())
		UIP_LargeGrade->Init();
	GET_SLOT(UTextBlock, TB_MyRank)
		GET_SLOT(UTextBlock, TB_RankPercent)
		GET_SLOT(UTextBlock, TB_RewardGem)
		GET_SLOT(UButton, BTN_Close)

		GET_SLOT(UTextBlock, TB_WeekRewardTitle)
		GET_SLOT(UTextBlock, TB_MyRanking)
		GET_SLOT(UTextBlock, TB_MyRankingUnit)
		GET_SLOT(UTextBlock, TB_RewardExplanation1)
		GET_SLOT(UTextBlock, TB_RewardExplanation2)
		GET_SLOT(UTextBlock, TB_SubTitle)
		GET_SLOT(UTextBlock, TB_BestGrade)
		GET_SLOT(UTextBlock, TB_WinNumber)
		GET_SLOT(UTextBlock, TB_MaximumWin)

	RewardItemImage.SetNum(MAX_REWARD_ITEM);
	GET_SLOT_BYNAME(UImage, RewardItemImage[0], IMG_RewardIcon01);
	GET_SLOT_BYNAME(UImage, RewardItemImage[1], IMG_RewardIcon02);
	GET_SLOT_BYNAME(UImage, RewardItemImage[2], IMG_RewardIcon03);

	RewardItemCount.SetNum(MAX_REWARD_ITEM);
	GET_SLOT_BYNAME(UTextBlock, RewardItemCount[0], TB_RewardCount01);
	GET_SLOT_BYNAME(UTextBlock, RewardItemCount[1], TB_RewardCount02);
	GET_SLOT_BYNAME(UTextBlock, RewardItemCount[2], TB_RewardCount03);

	RewardItemPanel.SetNum(MAX_REWARD_ITEM);
	GET_SLOT_BYNAME(UPanelWidget, RewardItemPanel[0], P_RewardPanel01);
	GET_SLOT_BYNAME(UPanelWidget, RewardItemPanel[1], P_RewardPanel02);
	GET_SLOT_BYNAME(UPanelWidget, RewardItemPanel[2], P_RewardPanel03);
}

void UB2UIModWeekRewardPopup::UpdateStaticText()
{
	TB_WeekRewardTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardTitle"))));
	TB_MyRanking->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardMyRanking"))));
	TB_MyRankingUnit->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("PVP1on1_RankUnit"))));
	TB_RewardExplanation1->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardExplanation1"))));
	TB_RewardExplanation2->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardExplanation2"))));
	TB_SubTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekReward_HallOfFame"))));
	TB_BestGrade->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardBestGrade"))));
	TB_WinNumber->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardWinNumber"))));
	TB_MaximumWin->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardMaximumWin"))));
}

void UB2UIModWeekRewardPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIModWeekRewardPopup::OnClickClose);
}

void UB2UIModWeekRewardPopup::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::ModMatchReward);
}

void UB2UIModWeekRewardPopup::OpenReward(FName UIType)
{
	if (UIType == UIFName::PVPOneOnOne)
	{
		if (auto* PvPDoc = UB2UIDocHelper::GetDocPVP1on1Rival())
			OpenSetting(PvPDoc->GetTagReward());
	}
	else if (UIType == UIFName::TeamMatchUI)
	{
		if (auto* TeamDoc = UB2UIDocHelper::GetDocTeamMatch())
			OpenSetting(TeamDoc->GetTeamReward());
	}
	else if (UIType == UIFName::ControlMain)
	{
		if (auto* ControlDoc = UB2UIDocHelper::GetDocControl())
		{
			OpenSetting(ControlDoc->GetControlRankingReward());
			ControlDoc->SetControlRankingReward(nullptr);
		}
	}
}

void UB2UIModWeekRewardPopup::OpenSetting(b2network::B2RankingRewardPtr Reward)
{
	if (Reward.get() == nullptr)
		return;

	UIP_LargeGrade->SetGradeRank(Reward->grade, Reward->star_count);
	TB_MyRank->SetText(FText::AsNumber(Reward->ranking));

	float RankPercent = static_cast<float>(Reward->ranking) / static_cast<float>(Reward->ranker_count) * 100;

	TB_RankPercent->SetText(FText::FromString(FString::Printf(TEXT("(%.1f%%)"), RankPercent)));
	//TB_RewardGem->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Reward->reward_gem)));

	const TArray<b2network::B2RewardPtr>& RankingReward = Reward->rewards;

	for (int32 i = 0; i < MAX_REWARD_ITEM; ++i)
	{
		if (RankingReward.IsValidIndex(i))
		{
			if (RewardItemPanel.IsValidIndex(i))
			{
				RewardItemPanel[i]->SetVisibility(ESlateVisibility::Visible);
			}

			if (RewardItemCount.IsValidIndex(i))
			{
				RewardItemCount[i]->SetText(FText::FromString(FString::Printf(TEXT("x%d"), RankingReward[i]->raw_reward->count)));
			}

			if (RewardItemImage.IsValidIndex(i))
			{
				RewardItemImage[i]->SetVisibility(ESlateVisibility::Visible);
				RewardItemImage[i]->SetBrushFromMaterial(BladeIIGameImpl::GetClientDataStore().GetRewardIconMaterial(RankingReward[i]->raw_reward->id));
			}

		}
	}



	//BladeIIGameImpl::GetClientDataStore().AddGem(Reward->reward_gem);
	ChangeUserAccountInfoClass<>::GetInstance().Signal();

}

