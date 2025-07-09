// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIChapterStarRewardBox.h"
#include "B2UIRecommandPower.h"
#include "B2UIStartButton.h"
#include "B2UIRewardEventSet.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIPVPEntryCharacters.h"
#include "B2ItemInfo.h"
#include "B2CombatStatEvaluator.h"
#include "B2UIDimensionMain.h"
#include "B2LobbyGameMode.h"
#include "B2UIDimensionWeekRewardInfoPopup.h"
#include "B2UIDimensionWeekRewardPopup.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIDimensionMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, STB_MaxStep);
	GET_SLOT(UTextBlock, STB_ClearTime);
	GET_SLOT(UTextBlock, STB_RewardTitle);
	GET_SLOT(UTextBlock, STB_StepTitle);
	GET_SLOT(UTextBlock, STB_MyRank);
	GET_SLOT(UTextBlock, STB_RankDesc);
	GET_SLOT(UTextBlock, STB_DailyCount);
	GET_SLOT(UTextBlock, STB_RankTitle);

	GET_SLOT(UTextBlock, TB_ClearTime);
	GET_SLOT(UTextBlock, TB_MaxStep);
	GET_SLOT(UTextBlock, TB_DailyCount);
	GET_SLOT(UTextBlock, TB_Diffulty);

	GET_SLOT(UHorizontalBox, List_Reward);
	GET_SLOT(UVerticalBox, List_Ranker);

	GET_SLOT(UB2Button, BTN_Next);
	GET_SLOT(UB2Button, BTN_NextEnd);
	GET_SLOT(UB2Button, BTN_Prev);
	GET_SLOT(UB2Button, BTN_PrevEnd);

	GET_SLOT(UB2UIRecommandPower, UIP_RecommendPower);
	if(UIP_RecommendPower.IsValid()) 
		UIP_RecommendPower->Init();

	GET_SLOT(UB2UIStartButton, UIP_StartBT);
	if (UIP_StartBT.IsValid())
	{
		UIP_StartBT->Init();
		UIP_StartBT->StartButtonDelegateBindLambda([this]() {this->OnClickBtnStart(); });
		UIP_StartBT->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatchBattleReady")));
	}

	GET_SLOT(UB2UIDimensionRankerInfo, UIP_MyRank);
	if (UIP_MyRank.IsValid())
		UIP_MyRank->Init();

	GET_SLOT(UB2UIChapterStarRewardBox, UIP_WeekRewardBox);
	if (UIP_WeekRewardBox.IsValid())
		UIP_WeekRewardBox->Init();
}

void UB2UIDimensionMain::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Next, &UB2UIDimensionMain::OnClickNext);
	BIND_CLICK_FUNC_TO_BTN(BTN_NextEnd, &UB2UIDimensionMain::OnClickNextEnd);
	BIND_CLICK_FUNC_TO_BTN(BTN_Prev, &UB2UIDimensionMain::OnClickPrev);
	BIND_CLICK_FUNC_TO_BTN(BTN_PrevEnd, &UB2UIDimensionMain::OnClickPrevEnd);

	if (UIP_WeekRewardBox.IsValid())
		UIP_WeekRewardBox->RewardListDelgate.BindDynamic(this, &UB2UIDimensionMain::OnClickBtnWeeklyReward);
}

void UB2UIDimensionMain::Init()
{
	Super::Init();

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_Dimension")));

	auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
	if (UIDocDimension)
		CurrentDiff = UIDocDimension->GetLastClearDiff();

	UpdateDiffPanel();
	UpdateRanking();
	UpdateMyRankDetail();
	UpdateDailyCount();

	// 정산보상
	ShowWeekReward();
}

void UB2UIDimensionMain::DestroySelf(UB2UIManager* InUIManager)
{
	if(UIP_MyRank.IsValid()) 
		UIP_MyRank->DestroySelf(InUIManager);
	if(UIP_WeekRewardBox.IsValid()) 
		UIP_WeekRewardBox->DestroySelf(InUIManager);
	if(UIP_RecommendPower.IsValid()) 
		UIP_RecommendPower->DestroySelf(InUIManager);
	if(UIP_StartBT.IsValid())
		UIP_StartBT->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}

void UB2UIDimensionMain::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (STB_MaxStep.IsValid())
		STB_MaxStep->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DimensionResult_PlayerTopDifficulty")));

	if (STB_ClearTime.IsValid())
		STB_ClearTime->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DimensionResult_ClearTime")));

	if (STB_RewardTitle.IsValid())
		STB_RewardTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_RewardInfo")));

	if (STB_StepTitle.IsValid())
		STB_StepTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Dimension_DifficultyChoice")));

	if (STB_MyRank.IsValid())
		STB_MyRank->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Dimension_MyRank")));

	if (STB_RankDesc.IsValid())
		STB_RankDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Dimension_RewardToolTip")));

	if (STB_DailyCount.IsValid())
		STB_DailyCount->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_DailyEnter")));

	if (STB_RankTitle.IsValid())
		STB_RankTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Dimension_Ranking")));
}

void UB2UIDimensionMain::SubscribeEvent()
{
	UnsubscribeEvent();
}

void UB2UIDimensionMain::UnsubscribeEvent()
{
	Issues.Empty();
}

void UB2UIDimensionMain::OnClickNext()
{
	auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
	if (!UIDocDimension)
		return;

	int32 MaxOpenDiff = UIDocDimension->GetMaxOpenDiff();
	int32 TempDiff = CurrentDiff + 1;

	CurrentDiff = FMath::Min(TempDiff, MaxOpenDiff);

	UpdateDiffPanel();
}

void UB2UIDimensionMain::OnClickNextEnd()
{
	auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
	if (!UIDocDimension)
		return;

	int32 MaxOpenDiff = UIDocDimension->GetMaxOpenDiff();
	int32 TempDiff = CurrentDiff + 10;

	CurrentDiff = FMath::Min(TempDiff, MaxOpenDiff);

	UpdateDiffPanel();
}

void UB2UIDimensionMain::OnClickPrev()
{
	auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
	if (!UIDocDimension)
		return;

	int32 MinOpenDiff = UIDocDimension->GetMinOpenDiff();
	int32 TempDiff = CurrentDiff - 1;

	CurrentDiff = FMath::Max(TempDiff, MinOpenDiff);

	UpdateDiffPanel();
}

void UB2UIDimensionMain::OnClickPrevEnd()
{
	auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
	if (!UIDocDimension)
		return;

	int32 MinOpenDiff = UIDocDimension->GetMinOpenDiff();
	int32 TempDiff = CurrentDiff - 10;

	CurrentDiff = FMath::Max(TempDiff, MinOpenDiff);

	UpdateDiffPanel();
}

void UB2UIDimensionMain::OnClickBtnStart()
{
	// 점검 정산 체크
	if (CheckContentsModeState(b2network::B2ContentsMode::DIMENSION))
		return;

	auto DocDimension = UB2UIDocHelper::GetDocDimension();
	if (DocDimension)
	{
		// 티켓부족
		if (DocDimension->GetDimensionTicket() < 1)
		{
			UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Dimension_StartError")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm
				);

			return;
		}

		// 인벤토리 풀
		if (BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().IsFullTotemInventory())
		{
			UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Dimension_StartErrorTotemFull")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::YesOrNo,
				FMsgPopupOnClick::CreateLambda([this]() {

				if (auto* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this)))
				{
					ELobbyUIPages MatchingDjLegacyUIPage = HeroMgmtSubModeToLobbyUIPage(EHeroMgmtSubMode::EHMSM_Totem);
					bool bRequetAsyncLoading = BladeIIGameImpl::bAllowUIAsyncLoading && BladeIIGameImpl::bAllowUIOnDemandAsyncLoading;

					LobbyGM->DJLegacy_AsyncLoadUIPage(MatchingDjLegacyUIPage, bRequetAsyncLoading);
				}

			}));

			return;
		}

		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_DimensionSelectPage);
	}
}

void UB2UIDimensionMain::UpdateRanking()
{
	if (List_Ranker.IsValid())
	{
		List_Ranker->ClearChildren();

		auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
		if (!UIDocDimension) 
			return;

		const TArray<FModRankerInfo>* ModRankerList = UIDocDimension->GetRankerList();
		if (!ModRankerList)
			return;

		for (auto Ranker : *ModRankerList)
		{
			auto* CreatedWidget = CreateWidget<UB2UIDimensionRankerInfo>(GetOwningPlayer(), RankingSlot);
			
			if (CreatedWidget != NULL)
			{
				auto* AddedSlot = Cast<UVerticalBoxSlot>(List_Ranker->AddChild(CreatedWidget));
				CreatedWidget->Init();
				CreatedWidget->SetRankerInfo(Ranker);
			}
		}
	}

	UpdateMyRanking();
}

void UB2UIDimensionMain::UpdateMyRanking()
{
	if (UIP_MyRank.IsValid())
	{
		auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
		if (UIDocDimension)
		{
			UIP_MyRank->SetRankerInfo(UIDocDimension->GetMyRank());
		}
	}
}

void UB2UIDimensionMain::UpdateMyRankDetail()
{
	auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
	if (!UIDocDimension)
		return;

	if (TB_MaxStep.IsValid())
		TB_MaxStep->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DimensionResult_NDifficulty")), FText::AsNumber(UIDocDimension->GetWeeklyBestRecordDiff())));

	if (TB_ClearTime.IsValid())
		TB_ClearTime->SetText(FText::FromString(UB2UIManager::FormatMilliSecondsAsTime(UIDocDimension->GetWeeklyBestRecordTime(), TEXT("hh:mm:ss.ms"))));
}

void UB2UIDimensionMain::UpdateDiffReward()
{
	if (List_Reward.IsValid())
	{
		List_Reward->ClearChildren();
		FB2DimensionInfo* CurrentInfo = BladeIIGameImpl::GetTotemDataStore().GetDimensionInfo(CurrentDiff);
		if (CurrentInfo)
		{
			// 한개는 고정 드랍 균열 조각
			auto* CreatedWidget = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), RewardSlot);
			if (CreatedWidget != NULL)
			{
				CreatedWidget->Init();
				CreatedWidget->UpdateRewardIconByItemIndex(CurrentInfo->Fixed_drop_item_id1, FText::AsNumber(CurrentInfo->Fixed_drop_count1));
				List_Reward->AddChild(CreatedWidget);
			}
			// 다른한개는 토템
			auto* CreatedWidget2 = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), RewardSlot);
			if (CreatedWidget2 != NULL)
			{
				CreatedWidget2->Init();
				
				CreatedWidget2->UpdateRewardIconByItemIndex(FItemRefIDHelper::ITEM_REF_ID_PREVIEW_TOTEM, FText::GetEmpty());
				List_Reward->AddChild(CreatedWidget2);
			}
		}
	}
}

void UB2UIDimensionMain::UpdateCombatPower()
{
	EPCClass MainClass = EPCClass::EPC_Gladiator;
	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		MainClass = IntToPCClass(DocBattle->GetCurPCClass());
	}

	FB2DimensionInfo* CurrentInfo = BladeIIGameImpl::GetTotemDataStore().GetDimensionInfo(CurrentDiff);
	if (UIP_RecommendPower.IsValid() && CurrentInfo)
	{
		UIP_RecommendPower->SetInfo(CombatStatEval::GetPCAttack(MainClass), CombatStatEval::GetPCDefense(MainClass), CurrentInfo->Recommend_attack, CurrentInfo->Recommend_defense);
	}
}

void UB2UIDimensionMain::UpdateDiffPanel()
{
	if (TB_Diffulty.IsValid())
		TB_Diffulty->SetText(FText::AsNumber(CurrentDiff));

	UpdateDiffReward();
	UpdateCombatPower();

	auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
	if (UIDocDimension)
		UIDocDimension->SetDifficulty(CurrentDiff);
}

void UB2UIDimensionMain::UpdateDailyCount()
{
	auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
	if (!UIDocDimension)
		return;

	int32 MaxCount = UIDocDimension->GetDailyTryCount();
	int32 PossibleCount = UIDocDimension->GetDimensionTicket();

	if (TB_DailyCount.IsValid())
	{
		TB_DailyCount->SetText(FText::FromString(FString::FromInt(PossibleCount) + "/" + FString::FromInt(MaxCount)));
		TB_DailyCount->SetColorAndOpacity(PossibleCount == 0 ? UB2UIManager::GetInstance()->TextColor_Decrease : UB2UIManager::GetInstance()->TextColor_Increase);
	}
}

void UB2UIDimensionMain::OnClickBtnWeeklyReward()
{
	UB2UIManager::GetInstance()->OpenUI<UB2UIDimensionWeekRewardInfoPopup>(UIFName::DimensionWeeklyRewardInfoPopup);
}

void UB2UIDimensionMain::ShowWeekReward()
{
	auto* UIDocDimension = UB2UIDocHelper::GetDocDimension();
	if (!UIDocDimension)
		return;

	if (UIDocDimension->GetWeeklyReward())
	{
		auto* WeeklyReward = UB2UIManager::GetInstance()->OpenUI<UB2UIDimensionWeekRewardPopup>(UIFName::DimensionWeeklyRewardPopup);

		if (WeeklyReward != nullptr)
		{
			WeeklyReward->SetReward(UIDocDimension->GetWeeklyReward());
			UIDocDimension->ClearWeeklyReward();
		}
	}
}

void UB2UIDimensionMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	auto* DiemsnionDoc = UB2UIDocHelper::GetDocDimension();

	if (!DiemsnionDoc)
		return;

	UIP_WeekRewardBox->SetRemainTime(FDateTime::UtcNow().ToUnixTimestamp(), (DiemsnionDoc->GetSettlement_Time() / 1000));

	FTimespan RemainTime = FDateTime::FromUnixTimestamp(DiemsnionDoc->GetSettlement_Time() / 1000) - FDateTime::FromUnixTimestamp(FDateTime::UtcNow().ToUnixTimestamp());
	int32 nRemainTimeSec = RemainTime.GetTotalSeconds();
	int32 nTotalTimeSec = DiemsnionDoc->GetSettlement_PeriodHour() * 3600;

	UIP_WeekRewardBox->SetForceProgressBar(nRemainTimeSec, nTotalTimeSec);
}

/////////////////////////////////////////////////////////////////
// UB2UIDimensionRankerInfo
//////////////////////////////////////////////////////////////////

void UB2UIDimensionRankerInfo::Init()
{
	Super::Init();
}

void UB2UIDimensionRankerInfo::SetRankerInfo(const FModRankerInfo& RankerInfo)
{
	AccountInfo = RankerInfo.AccountID;
	
	MainClass = RankerInfo.Entry[0];
	SubClass = EPCClass::EPC_End;
	UserName = RankerInfo.UserName;

	if (IMG_RankNumber_1.IsValid())
		IMG_RankNumber_1->SetVisibility(RankerInfo.Rank == 1 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_RankNumber_2.IsValid())
		IMG_RankNumber_2->SetVisibility(RankerInfo.Rank == 2 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_RankNumber_3.IsValid())
		IMG_RankNumber_3->SetVisibility(RankerInfo.Rank == 3 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_RankBG_1.IsValid())
		IMG_RankBG_1->SetVisibility(RankerInfo.Rank == 1 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_RankBG_2.IsValid())
		IMG_RankBG_2->SetVisibility(RankerInfo.Rank == 2 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_RankBG_3.IsValid())
		IMG_RankBG_3->SetVisibility(RankerInfo.Rank == 3 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (TB_Ranking.IsValid())
	{
		TB_Ranking->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("PVP1on1_RankerRank")), FText::AsNumber(RankerInfo.Rank)));
		
		if(RankerInfo.Rank > 3)
			TB_Ranking->SetVisibility(ESlateVisibility::HitTestInvisible);
		else if (RankerInfo.Rank == 0)
		{
			TB_Ranking->SetText(FText::FromString(TEXT("-")));
			TB_Ranking->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
			TB_Ranking->SetVisibility(ESlateVisibility::Hidden);
	}

	if (TB_PlayerName.IsValid())
		TB_PlayerName->SetText(FText::FromString(RankerInfo.UserName));

	if (TB_CombatPower.IsValid())
	{
		if (RankerInfo.Rank == 0)
			TB_CombatPower->SetText(FText::FromString(TEXT("-")));
		else
			TB_CombatPower->SetText(FText::AsNumber(RankerInfo.TotalCombatPower));
	}

	if (TB_ClearTime.IsValid())
		TB_ClearTime->SetText(FText::FromString(UB2UIManager::FormatMilliSecondsAsTime(RankerInfo.ClearTimeSec, TEXT("hh:mm:ss.ms"))));

	if (TB_ClearTower.IsValid())
	{
		if(RankerInfo.BestFloor > 0)
			TB_ClearTower->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DimensionResult_NDifficulty")), FText::AsNumber(RankerInfo.BestFloor)));
		else
			TB_ClearTower->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DimensionResult_NDifficulty")), FText::FromString(TEXT("-"))));
	}

	if (UIP_GradeIcon.IsValid())
		UIP_GradeIcon->SetGradeRank(RankerInfo.Grade, RankerInfo.GradeStar);

	if (UIP_EntryCharacters.IsValid())
		UIP_EntryCharacters->SetEntryClasses(MainClass, SubClass);
}

void UB2UIDimensionRankerInfo::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Ranking);
	GET_SLOT(UB2UIGradeRankAndStar, UIP_GradeIcon);

	if (UIP_GradeIcon.IsValid())
		UIP_GradeIcon->Init();

	GET_SLOT(UB2UIPVPEntryCharacters, UIP_EntryCharacters);
	if (UIP_EntryCharacters.IsValid())
		UIP_EntryCharacters->Init();

	GET_SLOT(UButton, BTN_ShowPlayerInfo);

	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UTextBlock, TB_PlayerName);
	GET_SLOT(UTextBlock, TB_ClearTime);
	GET_SLOT(UTextBlock, TB_ClearTower);

	GET_SLOT(UImage, IMG_RankNumber_1);
	GET_SLOT(UImage, IMG_RankNumber_2);
	GET_SLOT(UImage, IMG_RankNumber_3);
	GET_SLOT(UImage, IMG_RankBG_1);
	GET_SLOT(UImage, IMG_RankBG_2);
	GET_SLOT(UImage, IMG_RankBG_3);

	GET_SLOT(UB2Button, BTN_Select);
}

void UB2UIDimensionRankerInfo::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIDimensionRankerInfo::OnClickSelect);
}

void UB2UIDimensionRankerInfo::OnClickSelect()
{
	if (UserName.Equals(BladeIIGameImpl::GetLocalCharacterData().GetUserNickName()))
		return;

	FindAccountRequestEventClass<int64, EPCClass, EPCClass, EPCClass>::GetInstance().Signal(AccountInfo, MainClass, SubClass, EPCClass::EPC_End);
}

void UB2UIDimensionRankerInfo::UpdateStaticText()
{
	Super::UpdateStaticText();
}
