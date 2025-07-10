// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPMissionSlot.h"
#include "B2UIRewardEventSet.h"
#include "B2RewardInfo.h"
#include "B2MissionInfo.h"
#include "B2RewardInfo.h"
#include "BladeIIGameImpl.h"
#include "BladeIILocText.h"
#include "BladeIIUtil.h"
#include "B2UIMission.h"
#include "Retailer.h"


UB2UIPMissionSlot::UB2UIPMissionSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MissionID = 0;

	LinkScene = EUIScene::None;
	LinkOptional1 = 0;
	LinkOptional2 = 0;
}


void UB2UIPMissionSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_MissionIcon);

	GET_SLOT(UImage, IMG_BaseDefault);
	GET_SLOT(UOverlay, O_BaseCompleted);

	GET_SLOT(UImage, IMG_Complete);
	GET_SLOT(UImage, IMG_Dimmed);
	GET_SLOT(UPanelWidget, X_OverlayCompleted);

	GET_SLOT(UTextBlock, TB_MissionTitle);
	GET_SLOT(UB2RichTextBlock, RTB_MissionDesc);

	GET_SLOT(UButton, BTN_Link);
	GET_SLOT(UButton, BTN_GetReward);
	GET_SLOT(UTextBlock, TB_Link);
	GET_SLOT(UTextBlock, TB_GetReward);

	GET_SLOT(UProgressBar, PB_MissionProgress);
	GET_SLOT(UTextBlock, TB_ProgressCount);

	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon1);
	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon2);
	if (UIP_RewardIcon1.IsValid())
	{
		UIP_RewardIcon1->Init();
	}
	if (UIP_RewardIcon2.IsValid()) 
	{
		UIP_RewardIcon2->Init();
	}
}

void UB2UIPMissionSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Link, &UB2UIPMissionSlot::OnClickLinkButton);
	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIPMissionSlot::OnClickGetRewardButton);
}

void UB2UIPMissionSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	if (UIP_RewardIcon1.IsValid()) 
	{
		UIP_RewardIcon1->DestroySelf(InUIManager);
	}

	if (UIP_RewardIcon2.IsValid())
	{
		UIP_RewardIcon2->DestroySelf(InUIManager);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIPMissionSlot::UpdateMissionSlot(MissionPtr Mission)
{
	if (Mission != nullptr)
	{
		UpdateMissionSlotDefault(Mission);
		UpdateMissionSlotDynamic(Mission);
	}
}

// Master Data 만으로도 Setting 가능 한 것들
void UB2UIPMissionSlot::UpdateMissionSlotDefault(MissionPtr Mission)
{
	if (Mission != nullptr)
	{
		FMissionInfo* LocalMissionInfo = GetLocalMissionInfo(Mission->MissionID);
		if (LocalMissionInfo != nullptr)
		{
			if (IMG_MissionIcon.IsValid())
			{
				IMG_MissionIcon->SetBrushFromMaterial(LocalMissionInfo->MissionIcon.LoadSynchronous());
				IMG_MissionIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}

		UpdateStaticText_TitleDesc(Mission);
		UpdateMissionRewardIcon(Mission);
	}
}

// Mission의 상태를 Server에서 받아야만 Update 가능 한 것들
void UB2UIPMissionSlot::UpdateMissionSlotDynamic(MissionPtr Mission)
{
	if (Mission != nullptr)
	{
		UpdateMissionStateDisplay(Mission->MissionState);
		if (TB_ProgressCount.IsValid())
		{
			int32 DispalyCurrCount = FMath::Clamp<int32>(Mission->ProgressCount, 0, Mission->GoalCount);
			FString ProgressString = FString::Printf(TEXT("(%d / %d)"), DispalyCurrCount, Mission->GoalCount);
			TB_ProgressCount->SetText(FText::FromString(ProgressString));
		}

		if (PB_MissionProgress.IsValid() && Mission->GoalCount != 0)
		{
			float ProgressRate = static_cast<float>(Mission->ProgressCount) / static_cast<float>(Mission->GoalCount);
			if (Mission->MissionState == EMissionState::Complete || Mission->MissionState == EMissionState::Rewarded)
			{
				ProgressRate = 1.f;
			}

			PB_MissionProgress->SetPercent(ProgressRate);
		}

		if (Mission->MissionState == EMissionState::InProgress)
			UpdateBtn_Link(Mission);
		else if (Mission->MissionState == EMissionState::Complete)
		{
			if (BTN_GetReward.IsValid())
			{
				BTN_GetReward->SetVisibility(ESlateVisibility::Visible);
			}
		}

		MissionState = Mission->MissionState;
	}
}

void UB2UIPMissionSlot::UpdateMissionStateDisplay(EMissionState InMissionState)
{
	switch (InMissionState)
	{
	case EMissionState::InProgress:
		if (TB_Link.IsValid())
		{
			TB_Link->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Mission_Link")));
		}
		
		if (BTN_GetReward.IsValid())
		{
			BTN_GetReward->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		if (IMG_BaseDefault.IsValid() && SlotBaseMaterials.IsValidIndex(0))
		{
			IMG_BaseDefault->SetBrushFromMaterial(SlotBaseMaterials[0]);
			IMG_BaseDefault->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (O_BaseCompleted.IsValid())
		{
			O_BaseCompleted->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_Dimmed.IsValid())
		{
			IMG_Dimmed->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (X_OverlayCompleted.IsValid())
		{
			X_OverlayCompleted->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_Complete.IsValid())
		{
			IMG_Complete->SetVisibility(ESlateVisibility::Hidden);
		}

		if (UIP_RewardIcon1.IsValid())
		{
			UIP_RewardIcon1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (UIP_RewardIcon2.IsValid())
		{
			UIP_RewardIcon2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		break;
	case EMissionState::Complete:
		if (TB_GetReward.IsValid())
		{
			TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Quest_GetReward")));
		}

		if (BTN_Link.IsValid())
		{
			BTN_Link->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (O_BaseCompleted.IsValid())
		{
			O_BaseCompleted->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_BaseDefault.IsValid() && SlotBaseMaterials.IsValidIndex(1))
		{
			IMG_BaseDefault->SetBrushFromMaterial(SlotBaseMaterials[1]);
			IMG_BaseDefault->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (IMG_Dimmed.IsValid())
		{
			IMG_Dimmed->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (X_OverlayCompleted.IsValid())
		{
			X_OverlayCompleted->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_Complete.IsValid())
		{
			IMG_Complete->SetVisibility(ESlateVisibility::Hidden);
		}

		if (UIP_RewardIcon1.IsValid())
		{
			UIP_RewardIcon1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (UIP_RewardIcon2.IsValid())
		{
			UIP_RewardIcon2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		break;
	case EMissionState::Rewarded:
		if (O_BaseCompleted.IsValid())
		{
			O_BaseCompleted->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (IMG_BaseDefault.IsValid())
		{
			IMG_BaseDefault->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (UIP_RewardIcon1.IsValid())
		{
			UIP_RewardIcon1->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (UIP_RewardIcon2.IsValid())
		{
			UIP_RewardIcon2->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BTN_Link.IsValid())
		{
			BTN_Link->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BTN_GetReward.IsValid())
		{
			BTN_GetReward->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (TB_MissionTitle.IsValid())
		{
			TB_MissionTitle->SetColorAndOpacity(CompleteTextColor);
		}

		if (IMG_Dimmed.IsValid())
		{
			IMG_Dimmed->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (X_OverlayCompleted.IsValid())
		{
			X_OverlayCompleted->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (IMG_Complete.IsValid())
		{
			IMG_Complete->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		break;
	}

}

void UB2UIPMissionSlot::UpdateMissionRewardIcon(MissionPtr Mission)
{
	if (Mission != nullptr)
	{
		if (UIP_RewardIcon1.IsValid())
		{
			UIP_RewardIcon1->UpdateRewardIcon(Mission->RewardID, Mission->RewardCount);
		}

		if (UIP_RewardIcon2.IsValid())
		{
			bool bHasRewardPoint = Mission->CompletePoint > 0 ? true : false;
			if (bHasRewardPoint)
			{
				FSingleRewardInfoData* RewardInfo = StaticFindRewardInfo() ? StaticFindRewardInfo()->GetInfoData(RewardPointItemIndex) : nullptr;
				if (RewardInfo)
				{
					UIP_RewardIcon2->UpdateRewardIcon(RewardPointItemIndex, Mission->CompletePoint);
				}
				else
				{
					UIP_RewardIcon2->SetIconAndText(RewardPointMaterial, FText::AsNumber(Mission->CompletePoint));
				}
			}

			UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(UIP_RewardIcon2->GetParent());
			if (CanvasPanel)
			{
				CanvasPanel->SetVisibility(bHasRewardPoint ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
			}
		}
	}
}

void UB2UIPMissionSlot::UpdateBtn_Link(MissionPtr Mission)
{
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();

	bool bDisplayLinkBtn = false;
	FMissionInfo* LocalMissionInfo = GetLocalMissionInfo(MissionID);
	if (LocalMissionInfo != nullptr && Mission->MissionState == EMissionState::InProgress)
	{
		LinkScene = LocalMissionInfo->Scene;
		LinkOptional1 = LocalMissionInfo->Optional1;
		LinkOptional2 = LocalMissionInfo->Optional2;

		if (LinkScene != EUIScene::None && LinkScene != EUIScene::Max)
		{
			bDisplayLinkBtn = true;
			// Clear 하지 못한 Stage는 바로가기 안보여줌
			/*
			if (LinkScene == EUIScene::BattleStageInfo)
			{
			FServerStageID ServerStageId = FServerStageID(LocalMissionInfo->Optional1);
			int32 ClientStageId = StageDataStore.GetClientStageId(ServerStageId);
			EStageDifficulty CurDifficulty = StageDataStore.GetStageDifficulty(ServerStageId);

			int32 LastClearStgae = StageDataStore.GetLastClearClientId(CurDifficulty);
			if (LastClearStgae < ClientStageId)
			bDisplayLinkBtn = false;
			}
			*/
		}
	}

	if (BTN_Link.IsValid())
	{
		BTN_Link->SetVisibility(bDisplayLinkBtn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UB2UIPMissionSlot::UpdateStaticText_TitleDesc(MissionPtr Mission)
{
	if (Mission)
	{
		FString TitleKey = GetMissionTitleKey(Mission->MissionID);
		FText TitleText;

		int32 Step = Mission->RewardStep + 1;
		if (Mission->MissionState == EMissionState::Rewarded)
		{
			Step = Mission->RewardStep;
		}

		if (FText::FindText(B2LOC_CAT_MISSIONTEXT, TitleKey, TitleText, nullptr) == false)
		{
			// GoalCount가 아닌 condition이 변경된 연속미션이라면 Title(MissionID_Step) 과 같은 포맷의 키값을 갖는다. - Desc동일
			TitleKey = GetMissionTitleKey(Mission->MissionID, Step);
			TitleText = BladeIIGetLOCText(B2LOC_CAT_MISSIONTEXT, TitleKey);
		}

		if (TB_MissionTitle.IsValid())
		{
			TB_MissionTitle->SetText(TitleText);
		}

		FString DescKey = GetMissionDescKey(Mission->MissionID);
		FText DescText;
		if (FText::FindText(B2LOC_CAT_MISSIONTEXT, DescKey, DescText, nullptr) == false)
		{
			DescKey = GetMissionDescKey(Mission->MissionID, Step);
			DescText = BladeIIGetLOCText(B2LOC_CAT_MISSIONTEXT, DescKey);
		}

		if (RTB_MissionDesc.IsValid())
		{
			/*하드코딩 13008 ~ 13010 부분은  Condition1값을 이용해야한다.. 아직은 딱히 방법이 없습니다*/

			int32 StringCount = 0;

			if (Mission->MissionID >= 13008 && MissionID <= 13010)
			{
				StringCount = Mission->Condition1;
			}
			else
			{
				StringCount = Mission->GoalCount;
			}

			DescText = FText::Format(DescText, FText::AsNumber(StringCount));
			RTB_MissionDesc->SetText(DescText);
		}
	}
}

void UB2UIPMissionSlot::ClearMissionSlot()
{
	MissionID = 0;
	MissionType = EMissionType::None;
	MissionState = EMissionState::None;
}

FMissionInfo* UB2UIPMissionSlot::GetLocalMissionInfo(int32 InMissionID)
{
	UB2MissionInfo* MissionTable = StaticFindMissionInfo();
	if (MissionTable)
	{
		FMissionInfo* MissionInfo = MissionTable->GetMissionInfo(InMissionID);
		if (MissionInfo == nullptr)
		{
			MissionPtr Mission = MissionManager::GetInstance().GetMission(InMissionID);
			int32 Step = Mission ? Mission->CurrentStep : 0;

			MissionInfo = MissionTable->GetMissionInfo(InMissionID, Step);
		}

		return MissionInfo;
	}

	return nullptr;
}

void UB2UIPMissionSlot::SetRewardButtonDisable(bool bDisable)
{
	if (BTN_GetReward.IsValid())
	{
		BTN_GetReward->SetIsEnabled(!bDisable);
	}
}

void UB2UIPMissionSlot::SetRewardButtonVisibility(bool InVisibility)
{
	if (BTN_GetReward.IsValid())
	{
		BTN_GetReward->SetVisibility(InVisibility ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
	}
}

void UB2UIPMissionSlot::InitMissionSlot(int32 ID)
{
	MissionID = ID;

	MissionPtr MissionInfo = MissionManager::GetInstance().GetMission(MissionID);
	if (MissionInfo != nullptr)
	{
		UpdateMissionSlot(MissionInfo);
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UIPMissionSlot::OnClickLinkButton()
{
	MissionPtr CurrentMission = MissionManager::GetInstance().GetMission(MissionID);
	if (CurrentMission != nullptr)
	{
		if (CurrentMission->MissionState == EMissionState::InProgress)
		{
			switch (LinkScene)
			{
			case EUIScene::Mission:
				ChangeMissionTabClass<const EMissionType, const bool>::GetInstance().Signal(static_cast<EMissionType>(LinkOptional1), true);
				break;

			default:
				ReqGetContetnsOpenStatusClass<const int32>::GetInstance().Signal(MissionID);
				break;
			}
		}
	}
}

void UB2UIPMissionSlot::OnClickGetRewardButton()
{
	MissionPtr CurrentMission = MissionManager::GetInstance().GetMission(MissionID);
	
	if (CurrentMission != nullptr)
	{
		if (CurrentMission->ProgressCount >= CurrentMission->GoalCount &&
			CurrentMission->MissionState == EMissionState::Complete)
		{
			SetAllRewardButtonDisableClass<const bool>::GetInstance().Signal(true); // Receive 받기전까지 Reward 못누르게 막자

			data_trader::Retailer::GetInstance().RequestReceiveMissionReward(MissionID);
		}
	}
}

