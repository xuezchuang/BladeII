#include "BladeII.h"
#include "B2UIModWeeklyReward.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2UIRankingRewardRow.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIModWeeklyReward::Init()
{
	Super::Init();
}


void UB2UIModWeeklyReward::CacheAssets()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2UIModWeeklyReward::CacheAssets"));
	GradeOverlayArr.SetNum(9);

	GET_SLOT_BYNAME(UCanvasPanel, GradeOverlayArr[0], X_Grade2);
	GET_SLOT_BYNAME(UCanvasPanel, GradeOverlayArr[1], X_Grade3);
	GET_SLOT_BYNAME(UCanvasPanel, GradeOverlayArr[2], X_Grade4);
	GET_SLOT_BYNAME(UCanvasPanel, GradeOverlayArr[3], X_Grade5);
	GET_SLOT_BYNAME(UCanvasPanel, GradeOverlayArr[4], X_Grade6);
	GET_SLOT_BYNAME(UCanvasPanel, GradeOverlayArr[5], X_Grade7);
	GET_SLOT_BYNAME(UCanvasPanel, GradeOverlayArr[6], X_Grade8);
	GET_SLOT_BYNAME(UCanvasPanel, GradeOverlayArr[7], X_Grade9);
	GET_SLOT_BYNAME(UCanvasPanel, GradeOverlayArr[8], X_Grade10);


	GradeInfo();

	GET_SLOT(UB2Button, BTN_Close)
		


	RankingRewardRow.AddUninitialized(13);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[0], RankingRewardRow0);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[1], RankingRewardRow1);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[2], RankingRewardRow2);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[3], RankingRewardRow3);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[4], RankingRewardRow4);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[5], RankingRewardRow5);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[6], RankingRewardRow6);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[7], RankingRewardRow7);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[8], RankingRewardRow8);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[9], RankingRewardRow9);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[10], RankingRewardRow10);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[11], RankingRewardRow11);
	GET_SLOT_BYNAME(UB2UIRankingRewardRow, RankingRewardRow[12], RankingRewardRow12);

	for (auto& Ele : RankingRewardRow)
	{
		if (Ele.IsValid())
		{
			Ele->Init();
		}
	}
}

void UB2UIModWeeklyReward::UpdateStaticText()
{

}

void UB2UIModWeeklyReward::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIModWeeklyReward::OnClickClose);
}


void UB2UIModWeeklyReward::SetOpenRewardListType(FName UIType)
{
	if(UIType == UIFName::PVPOneOnOne)
	{
		RewardDataSetting(BladeIIGameImpl::GetClientDataStore().GetPvPWeeklyRewardDataList());
	}
	else if (UIType == UIFName::TeamMatchUI)
	{
		RewardDataSetting(BladeIIGameImpl::GetClientDataStore().GetTeamMatchWeeklyRewardDataList());
	}
	else if (UIType == UIFName::ModMatchReward)
	{
		RewardDataSetting(BladeIIGameImpl::GetClientDataStore().GetControlMatchWeeklyRewardDataList());
	}
}

void UB2UIModWeeklyReward::GradeInfo()
{

	B2_SCOPED_TRACK_LOG(TEXT("UB2UIModWeeklyReward::GradeInfo"));
	for (int32 i = 0; i < GradeOverlayArr.Num(); ++i)
	{
		if (GradeOverlayArr[i].IsValid())
		{
			TSubclassOf<UB2UIGradeRankAndStar> GradeInfo = UIP_GradeRankAndStar.LoadSynchronous();
			UB2UIGradeRankAndStar* UIPPart = CreateWidget<UB2UIGradeRankAndStar>(this->GetOwningPlayer(), GradeInfo);

			if (UIPPart)
			{
				UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(GradeOverlayArr[i]->AddChild(UIPPart));
				UIPPart->Init();
				UIPPart->SetGradeRank(i + 2, 3);
				UIPPart->SetRenderScale(FVector2D(0.7f, 0.7f));
				UIPPart->bHideEmptyImage = true;
				UIPPart->SetMaxElementNum(0);
				if (CanvasSlot)
				{
					CanvasSlot->SetOffsets(GradeMargin);
				}
			}
		}
	}

}

void UB2UIModWeeklyReward::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIModWeeklyReward::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::ModMatchRewardList_Week);
}

void UB2UIModWeeklyReward::RewardDataSetting(const TArray<FModWeeklyReward>& WeeklyData)
{

	B2_SCOPED_TRACK_LOG(TEXT("UB2UIModWeeklyReward::RewardDataSetting"));
	BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Mod_WeekRewardRanking1")));

	for (int32 i = 0; i < WeeklyData.Num(); ++i)
	{
		if (RankingRewardRow.IsValidIndex(i))
		{
			RankingRewardRow[i]->SetRowInfo(WeeklyData[i]);
		}
	}

}

