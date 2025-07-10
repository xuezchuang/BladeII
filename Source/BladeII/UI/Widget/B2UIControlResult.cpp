// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIControlResult.h"
#include "B2ControlGameMode.h"
#include "B2UIDocHelper.h"
#include "B2UIRewardEventSet.h"
#include "B2UIControlRecordItem.h"
#include "B2UIGradeRankAndStar.h"
#include "B2UIModRewardBox.h"
#include "B2UIStageResultLevelUp.h"
#include "Retailer.h"
#include "B2UIRankChanged.h"

#include "BladeIIGameImpl.h"
#include "BladeIINetPlayer.h"
#include "B2UIManager.h"
#include "B2UIControlUserReportPopup.h"

void UB2UIControlResult::Init()
{
	Super::Init();

	SubscribeEvent();
}

void UB2UIControlResult::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	auto ControlDoc = UB2UIDocHelper::GetDocControl();

	TMap<int64, int64> UseRepotUser;

	if (ControlDoc)
		ControlDoc->SetUserReportUser(UseRepotUser);

	UnsubscribeEvent();
}

void UB2UIControlResult::NativeConstruct()
{
	Super::NativeConstruct();

	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

	if (TB_LoseMent.IsValid())
		TB_LoseMent->SetVisibility(ESlateVisibility::Hidden);
	
	bool IsLose = false;

	if (pGM)
	{
		ETeamType WinTeam = pGM->ResultWinTeam;
		ETeamType MyTeam = ETeamType(pGM->GetMyTeamNum());

		if (WinTeam == ETeamType::ETT_End)
		{
			PlayAnimationEvent_AnimDraw();
		}
		else if (WinTeam == MyTeam)
		{
			PlayAnimationEvent_AnimWin();

			if (STB_GetMailReward.IsValid())
			{
				STB_GetMailReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_VictoryReward")));
			}
		}
		else
		{
	
			PlayAnimationEvent_AnimLose();

			if (TB_LoseMent.IsValid())
			{
				TB_LoseMent->SetText(BladeIIGetDuelModLoseMentText());
				TB_LoseMent->SetVisibility(ESlateVisibility::Visible);
			}

			if (STB_GetMailReward.IsValid())
			{
				STB_GetMailReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_LoseReward")));
			}

			if (P_LoseText.IsValid())
			{
				P_LoseText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}

			IsLose = true;
		}
	}

	// 등급 마크
	if (UIP_Grade.IsValid())
	{
		int32 nGrade = 10;
		int32 nStar = 0;
		int32 beforeGrade = 10;
		int32 beforenStar = 0;
		nGrade = UB2UIDocHelper::GetDocControl()->GetBattleGrade();
		nStar = UB2UIDocHelper::GetDocControl()->GetBattleStarCount();
		beforeGrade = UB2UIDocHelper::GetDocControl()->GetBeforeGradeInfo()->battle_grade;
		beforenStar = UB2UIDocHelper::GetDocControl()->GetBeforeGradeInfo()->battle_star_count;

		//등급 방어
		if (TB_PVPRankPrevent.IsValid())
		{
			if (nGrade == beforeGrade && nStar == beforenStar)
			{
				TB_PVPRankPrevent->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				TB_PVPRankPrevent->SetVisibility(ESlateVisibility::Collapsed);
			}
		}

		UIP_Grade->SetGradeRank(nGrade, nStar);

		//연승보너스
		if (TB_WinningBonus.IsValid())
		{
			TB_WinningBonus->SetVisibility(UB2UIDocHelper::GetDocControl()->GetGradeInfo()->continue_win >=
				BladeIIGameImpl::GetClientDataStore().GetWinCountForBonus() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

			// 1등급은 연승보너스 표기 안한다.
			if (beforeGrade == 1)
			{
				TB_WinningBonus->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

	// 적 영웅 처치
	// n 킬
	if (UIP_Control_Record_EnemyKill.IsValid()) 
	{
		UIP_Control_Record_EnemyKill->SetTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlRecordKillEnemy")));

		if (pGM)
		{
			int32 nKillCount = UB2UIDocHelper::GetDocControl()->GetKillCount(pGM->GetMyNetId());			
			UIP_Control_Record_EnemyKill->SetAmount(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlRecordKillEnemyCount")), FText::AsNumber(nKillCount)));
		}
	}

	// 피해량 기여도
	// 10,000
	if (UIP_ControlRecord_TotalDamage.IsValid())
	{
		UIP_ControlRecord_TotalDamage->SetTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlRecordDamage")));

		int32 nTotalDamageAmount = UB2UIDocHelper::GetDocControl()->GetMyTotalDamage();
		UIP_ControlRecord_TotalDamage->SetAmount(FText::AsNumber(nTotalDamageAmount));
	}

	// 임무 기여 시간
	// n시간 n분 General_Min
	if (UIP_ControlRecord_ContributeTime.IsValid())
	{
		UIP_ControlRecord_ContributeTime->SetTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlRecordContributeTime")));

		float fContributeTime = UB2UIDocHelper::GetDocControl()->GetMyContributeTime();

		FString strContributeTime = TEXT("");

		if (fContributeTime / 60 > 0)
			strContributeTime += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), FText::AsNumber(int32(fContributeTime / 60))).ToString();

		strContributeTime += FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sconds")), FText::AsNumber(int32(fContributeTime) % 60)).ToString();

		UIP_ControlRecord_ContributeTime->SetAmount(FText::FromString(strContributeTime));
	}

	// 경기 내 죽음
	// n 회
	if (UIP_ControlRecord_Dead.IsValid())
	{
		UIP_ControlRecord_Dead->SetTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlRecordDead")));

		if (pGM)
		{
			int32 nDeadCount = UB2UIDocHelper::GetDocControl()->GetDeadCount(pGM->GetMyNetId());
			UIP_ControlRecord_Dead->SetAmount(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlRecordDeadCount")), FText::AsNumber(nDeadCount)));
		}
	}

	// 보상쪽
	FB2ResponseAssaultResultPtr ptrResult = UB2UIDocHelper::GetDocControl()->GetResultPtr();

	check(ControlRewardClass);

	if (List_Reward.IsValid() && ptrResult)
	{
		List_Reward->ClearChildren();

		if (ptrResult->victory_rewards.Num() > 0)
		{
			for (auto RewardItem : ptrResult->victory_rewards)
			{
				auto* CreatedWidget = CreateWidget<UB2UIControlResultRewardIcon>(GetWorld(), ControlRewardClass);
				if (CreatedWidget)
				{
					CreatedWidget->Init();
					CreatedWidget->SetRewardItem(RewardItem);
					List_Reward->AddChild(CreatedWidget);
				}
			}
		}

		if (ptrResult->increased_mvp_point > 0)
		{
			auto* CreatedWidget = CreateWidget<UB2UIControlResultRewardIcon>(GetWorld(), ControlRewardClass);
			if (CreatedWidget)
			{
				CreatedWidget->Init();
				CreatedWidget->SetRewardMvp(ptrResult->increased_mvp_point);
				List_Reward->AddChild(CreatedWidget);
			}
		}		

		if (IsLose)
		{
			List_Reward->AddChild(P_LoseText.Get());
		}

		// 박스보상(브실골박스)
		UB2UIDocControl*	ControlDoc = UB2UIDocHelper::GetDocControl();
		if (!ControlDoc) return;
		ControlDoc->SetRewardBoxStatus(ptrResult->reward_box_status);

		b2network::B2DuelRewardBoxStatusPtr RewardBosxStatus = ptrResult->reward_box_status;

		if (UIP_ModRewardBox.IsValid())
		{
			UIP_ModRewardBox->SetModRewardBox(false, b2network::B2DuelMode::DUEL_MODE_ASSAULT, RewardBosxStatus->box_grade
				, RewardBosxStatus->win_count, RewardBosxStatus->upgrade_win_count, RewardBosxStatus->defeat_count);
			UIP_ModRewardBox->SetModExplanation(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("duelRewardBox_Notice3")));
		}
	}

	CheckRankChanged();
}

void UB2UIControlResult::UpdateStaticText()
{
	if (TB_WinningBonus.IsValid())
		TB_WinningBonus->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("WinningStreakBonus")));

	if(TB_PVPRankPrevent.IsValid())
		TB_PVPRankPrevent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPRankPrevent")));
}

void UB2UIControlResult::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UIGradeRankAndStar, UIP_Grade);
	if (UIP_Grade.IsValid())
		UIP_Grade->Init();

	GET_SLOT(UB2UIControlRecordItem, UIP_Control_Record_EnemyKill);
	if (UIP_Control_Record_EnemyKill.IsValid())
		UIP_Control_Record_EnemyKill->Init();

	GET_SLOT(UB2UIControlRecordItem, UIP_ControlRecord_TotalDamage);
	if (UIP_ControlRecord_TotalDamage.IsValid())
		UIP_ControlRecord_TotalDamage->Init();

	GET_SLOT(UB2UIControlRecordItem, UIP_ControlRecord_ContributeTime);
	if (UIP_ControlRecord_ContributeTime.IsValid())
		UIP_ControlRecord_ContributeTime->Init();

	GET_SLOT(UB2UIControlRecordItem, UIP_ControlRecord_Dead);
	if (UIP_ControlRecord_Dead.IsValid())
		UIP_ControlRecord_Dead->Init();

	GET_SLOT(UB2Button, BTN_Exit);

	GET_SLOT(UPanelWidget, p_MailText);

	GET_SLOT(UHorizontalBox, List_Reward);

	GET_SLOT(UTextBlock, STB_GetMailReward);
	GET_SLOT(UTextBlock, TB_LoseMent);

	GET_SLOT(UB2UIStageResultLevelUp, UIP_LevelUp);
	if (UIP_LevelUp.IsValid())
		UIP_LevelUp->Init();

	GET_SLOT(UB2UIModRewardBox, UIP_ModRewardBox);
	if (UIP_ModRewardBox.IsValid())
		UIP_ModRewardBox->Init();

	GET_SLOT(UTextBlock, TB_WinningBonus);
	GET_SLOT(UTextBlock, TB_PVPRankPrevent);

	GET_SLOT(UB2UIRankChanged, UIP_ControlRankingChangedPopup);
	if (UIP_ControlRankingChangedPopup.IsValid())
	{
		UIP_ControlRankingChangedPopup->Init();
		UIP_ControlRankingChangedPopup->ButtonDelegateBindLambda([this]() { CloseRankingChangePopup(); });
	}

	GET_SLOT(UB2Button, BTN_UserReport)
	GET_SLOT(UPanelWidget, P_LoseText);
}

void UB2UIControlResult::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Exit, &UB2UIControlResult::OnClickExit);
	BIND_CLICK_FUNC_TO_BTN(BTN_UserReport, &UB2UIControlResult::OnClickUserReport);
}

void UB2UIControlResult::CloseWidgetDelegate()
{
	OnClickExit();
}

void UB2UIControlResult::SubscribeEvent()
{
	DeliveryGetAssaultBattleStatusTicket = DeliveryGetAssaultBattleStatusClass<FB2ResponseGetAssaultBattleStatusPtr>::GetInstance().Subscribe([this](const FB2ResponseGetAssaultBattleStatusPtr& msgPtr)
	{
		this->SetAssaultBattleStatus(msgPtr);
	});
}

void UB2UIControlResult::UnsubscribeEvent()
{
	DeliveryGetAssaultBattleStatusClass<FB2ResponseGetAssaultBattleStatusPtr>::GetInstance().Unsubscribe(DeliveryGetAssaultBattleStatusTicket);
}

void UB2UIControlResult::OnClickExit()
{
	data_trader::Retailer::GetInstance().RequestAssaultBattleStatus();	

}

void UB2UIControlResult::OnClickUserReport()
{
	UB2UIManager::GetInstance()->OpenUI<UB2UIControlUserReportPopup>(UIFName::ControlUserReportPopup);
}

void UB2UIControlResult::CheckRankChanged()
{
	UB2UIDocSome* pDocSome = UB2UIDocHelper::GetDocSome();
	UB2UIDocControl* pDocControl = UB2UIDocHelper::GetDocControl();

	// 받아논 정보로 독에 셋팅..
	if (pDocSome && pDocControl)
	{
		FB2ResponseGetAssaultBattleStatusPtr GetAssaultPtr = pDocControl->GetGetAssaultPtr();
		FB2ResponseAssaultResultPtr ResultPtr = pDocControl->GetResultPtr();

		if (GetAssaultPtr && ResultPtr)
		{
			// 내꺼
			if (ResultPtr->ranking)
			{
				int32 PrevRanking = GetAssaultPtr->ranking->ranking;
				int32 CurRanking = ResultPtr->ranking->ranking;
				int32 CurRankingTotalCount = ResultPtr->ranking->total_ranker_count;

				pDocSome->SetRankingChanged((PrevRanking != 0 ? PrevRanking : CurRankingTotalCount) - CurRanking);

				FModRankerInfo MyRankerInfo;
				MyRankerInfo.Grade = ResultPtr->match_status->battle_grade;
				MyRankerInfo.GradeStar = ResultPtr->match_status->battle_star_count;
				MyRankerInfo.Rank = CurRanking;
				MyRankerInfo.UserName = BladeIIGameImpl::GetClientDataStore().GetUserNickName();

				auto* DocBattle = UB2UIDocHelper::GetDocBattle();
				EPCClass SelectedPcClass = DocBattle ? IntToPCClass(DocBattle->GetCurPCClass()) : EPCClass::EPC_Gladiator;
				MyRankerInfo.Entry.Add(SelectedPcClass);

				UB2UIDocHero* pDocHero = UB2UIDocHelper::GetDocHero((int32)SelectedPcClass);
				if (pDocHero)
				{
					MyRankerInfo.TotalCombatPower = pDocHero->GetCombatPower();
					//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
					//MyRankerInfo.EntryLevels.Add(pDocHero->GetCurrentLevel());
					FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
					const int32 SelectedCharLevel = CharacterDataStore.GetCharacterLevel(SelectedPcClass);
					MyRankerInfo.EntryLevels.Add(SelectedCharLevel);
				}

				//pDocSome->SetMyRankerInfo(MyRankerInfo);
			}

			// 상대꺼
			if (ResultPtr->ranking && ResultPtr->prev_ranker)
			{
				FModRankerInfo OtherRankerInfo;
				OtherRankerInfo.Grade = ResultPtr->prev_ranker->battle_grade;
				OtherRankerInfo.GradeStar = ResultPtr->prev_ranker->battle_star_count;
				OtherRankerInfo.Rank = ResultPtr->ranking->ranking + 1;
				OtherRankerInfo.UserName = ResultPtr->prev_ranker->nickname;

				for (auto& EntryChar : ResultPtr->prev_ranker->entries)
				{
					OtherRankerInfo.Entry.Add(SvrToCliPCClassType(EntryChar.character_type));
					OtherRankerInfo.EntryLevels.Add(EntryChar.level);
				}

				OtherRankerInfo.TotalCombatPower = ResultPtr->prev_ranker->total_power;

				//pDocSome->SetOtherRankerInfo(OtherRankerInfo);
			}
		}
	}
	// 받아논 정보로 독에 셋팅..끝

	if (pDocSome && pDocSome->GetRankingChanged() > 0)
	{
		ShowRankingChanged(pDocSome->GetRankingChanged());
		pDocSome->SetRankingChanged(0);
	}
	else
	{
		CheckAndOpenLevelUpPart();
	}
}

void UB2UIControlResult::ShowRankingChanged(int32 DiffChangedRanking)
{
	UB2UIDocSome* pDocSome = UB2UIDocHelper::GetDocSome();

	if (pDocSome && UIP_ControlRankingChangedPopup.IsValid())
	{
		UIP_ControlRankingChangedPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UIP_ControlRankingChangedPopup->SetUsingGameMode(EB2GameMode::Control);
		//UIP_ControlRankingChangedPopup->SetMyRankingData(pDocSome->GetMyRankerInfo());
		//UIP_ControlRankingChangedPopup->SetOtherRankingData(pDocSome->GetOtherRankerInfo());
		UIP_ControlRankingChangedPopup->SetChangedRanking(DiffChangedRanking);
	}
}

void UB2UIControlResult::CloseRankingChangePopup()
{
	CheckAndOpenLevelUpPart();
}

void UB2UIControlResult::CheckAndOpenLevelUpPart()
{
	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	TArray<EPCClass> ThisPCClass;

	ThisPCClass.Add(CharacterDataStore.GetMainPlayerClass());

	for (auto pcclass : ThisPCClass)
	{
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
		//UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(pcclass));
		//bool bCharLevelUp = (FMath::Max(DocHero->GetBasicUpgradeLevel(), DocHero->GetExtraUpgradeLevel()) > DocHero->GetCurrentLevel());
		const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(pcclass);
		const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(pcclass);
		if (ClearAfterLevel > ClearBeforeLevel)
		{
			if (UIP_LevelUp.IsValid())
			{
				UIP_LevelUp->StartMe(ThisPCClass);
				//UIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this]() { this->OnLevelUpFXFinish(); });
				return;
			}
		}
	}

	//OnLevelUpFXFinish();
}

void UB2UIControlResult::SetAssaultBattleStatus(const FB2ResponseGetAssaultBattleStatusPtr& MsgPtr)
{
	UB2UIDocControl* ControlDoc = UB2UIDocHelper::GetDocControl();
	if (!ControlDoc)
		return;

	ControlDoc->SetGetAssaultPtr(MsgPtr);

	// 정보 받았으면 나감
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
	{
		if (!pGM->GetRoomId().IsEmpty())
			pGM->LeaveRoomIfJoined();
		else
			pGM->ReturnToControlMainMenu();
	}
}

void UB2UIControlResultRewardIcon::Init()
{
	Super::Init();
}

void UB2UIControlResultRewardIcon::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UWidgetSwitcher, SW_RewardType);
	//GET_SLOT(UB2UIModRewardBox, UIP_RewardIconBox);
	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon);
	GET_SLOT(UTextBlock, TB_RewardTitle);

	if (UIP_RewardIcon.IsValid())
		UIP_RewardIcon->Init();
	//if (UIP_RewardIconBox.IsValid())
	//	UIP_RewardIconBox->Init();
}

void UB2UIControlResultRewardIcon::SetRewardBox()
{
	if (SW_RewardType.IsValid())
	{
		SW_RewardType->SetActiveWidgetIndex(1);
	}

	FB2ResponseAssaultResultPtr ptrResult = UB2UIDocHelper::GetDocControl()->GetResultPtr();
	if (ptrResult)
	{
		auto RewardBoxInfo = ptrResult->reward_box_status;
		/*if (UIP_RewardIconBox.IsValid() && RewardBoxInfo)
		{
			UIP_RewardIconBox->SetModRewardBox(false, b2network::B2DuelMode::DUEL_MODE_ASSAULT, RewardBoxInfo->box_grade, RewardBoxInfo->win_count, RewardBoxInfo->upgrade_win_count, RewardBoxInfo->defeat_count);
			UIP_RewardIconBox->SetBoxImageGradeEvent(RewardBoxInfo->box_grade);
			SetRewardBoxGrade_IMP(RewardBoxInfo->box_grade);
		}*/
	}
}


void UB2UIControlResultRewardIcon::SetRewardItem(b2network::B2RewardPtr Rewardptr)
{
	if (SW_RewardType.IsValid())
	{
		SW_RewardType->SetActiveWidgetIndex(0);
	}

	if (UIP_RewardIcon.IsValid())
	{
		UIP_RewardIcon->UpdateRewardIcon(Rewardptr);
	}

	if (TB_RewardTitle.IsValid())
	{
		if (Rewardptr->light_characters.Num() > 0)
			TB_RewardTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_ExpReward")));
		else
			TB_RewardTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVPResult_VictoryReward")));
	}
}

void UB2UIControlResultRewardIcon::SetRewardMvp(int32 RewardCount)
{
	if (UIP_RewardIcon.IsValid())
	{
		UIP_RewardIcon->UpdateRewardIconByItemIndex(FItemRefIDHelper::GetGoodsID_ControlMVPPoint(), FText::AsNumber(RewardCount));
	}

	if (TB_RewardTitle.IsValid())
	{
		TB_RewardTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_MVP")));
	}
}