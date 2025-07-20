// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIHallOfFame.h"
#include "B2UIManager.h"

#include "B2UICommonTab.h"
#include "B2LobbyGameMode.h"
#include "B2UIHallOfFamePlayerInfo.h"
#include "B2UIHallOfFameToolTip.h"
#include "B2UIDocHelper.h"
#include "B2UIHallOfFameSkipButton.h"

UB2UIHallOfFame::UB2UIHallOfFame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerInfoUI.Empty();
	IsTest = false;
}

void UB2UIHallOfFame::CacheAssets()
{
	Super::CacheAssets();

	if (MenuTab.Num() == 0)
	{
		MenuTab.SetNum(3);

		GET_SLOT_BYNAME(UB2UICommonTab, MenuTab[0], Tab_PvP);
		GET_SLOT_BYNAME(UB2UICommonTab, MenuTab[1], Tab_TeamMatch);
		GET_SLOT_BYNAME(UB2UICommonTab, MenuTab[2], Tab_Control);
	}

	if (P_CelebrateRank.Num() == 0)
	{
		P_CelebrateRank.SetNum(3);

		GET_SLOT_BYNAME(UPanelWidget, P_CelebrateRank[0], OV_Ranking1);
		GET_SLOT_BYNAME(UPanelWidget, P_CelebrateRank[1], OV_Ranking2);
		GET_SLOT_BYNAME(UPanelWidget, P_CelebrateRank[2], OV_Ranking3);
	}

	GET_SLOT(UTextBlock, TB_LikeNUM);
	GET_SLOT(UTextBlock, TB_LikeText);
	GET_SLOT(UTextBlock, TB_Celebrate);
	GET_SLOT(UTextBlock, TB_RewardInfo);
	GET_SLOT(UTextBlock, TB_Reward);
	GET_SLOT(UTextBlock, TB_ToMail);

	GET_SLOT(UB2RichTextBlock, TB_Ranking);

	GET_SLOT(UB2Button, BTN_ClosePraiseToolTip);
	GET_SLOT(UB2Button, BTN_OpenPraiseToolTip);
	GET_SLOT(UB2Button, BTN_CelebrateRanker);

	GET_SLOT(UPanelWidget, P_RankerPanel);
	GET_SLOT(UPanelWidget, P_MenuTab);
	GET_SLOT(UPanelWidget, OV_LikeInfo);
	GET_SLOT(UPanelWidget, OV_SeasonMsg);

	GET_SLOT(UB2UIHallOfFameSkipButton, UIP_HallOfFameSkipButton)
}

void UB2UIHallOfFame::BindDelegates()
{
	Super::BindDelegates();

	if (MenuTab.IsValidIndex(0) && MenuTab[0].IsValid())
	{
		REGISTER_UITAB_BINDING(MenuTab[0].Get(), &UB2UIHallOfFame::OnClick_PvP);
	}

	if (MenuTab.IsValidIndex(1) && MenuTab[1].IsValid())
	{
		REGISTER_UITAB_BINDING(MenuTab[1].Get(), &UB2UIHallOfFame::OnClick_TeamMatch);
	}

	if (MenuTab.IsValidIndex(2) && MenuTab[2].IsValid())
	{
		REGISTER_UITAB_BINDING(MenuTab[2].Get(), &UB2UIHallOfFame::OnClick_Controll);
	}

	BIND_CLICK_FUNC_TO_BTN(BTN_OpenPraiseToolTip, &UB2UIHallOfFame::OpenPraiseToolTip);
	BIND_CLICK_FUNC_TO_BTN(BTN_ClosePraiseToolTip, &UB2UIHallOfFame::ClosePraiseToolTip);
	BIND_CLICK_FUNC_TO_BTN(BTN_CelebrateRanker, &UB2UIHallOfFame::CloseCelebrate);
}

void UB2UIHallOfFame::Init()
{
	Super::Init();

	if (UIP_HallOfFameSkipButton.IsValid())
	{
		UIP_HallOfFameSkipButton->Init();
	}

	if (MenuTab[0].IsValid())
	{
		MenuTab[0]->Init();
		MenuTab[0]->SetVisible(true);
		MenuTab[0]->SetTabName(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_PVP")));
	}

	if (MenuTab[1].IsValid())
	{
		MenuTab[1]->Init();
		MenuTab[1]->SetTabName(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch")));
	}

	if (MenuTab[2].IsValid())
	{
		MenuTab[2]->Init();
		MenuTab[2]->SetTabName(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_Control")));
	}
	
	if (auto* DocSome = UB2UIDocHelper::GetDocSome())
	{
		DocSome->SetOnClickUserToHallOf(false);
	}

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Mod_WeekReward_HallOfFame")));
	DoMarkRedDot();
	SetMenuTabSelected();
}


void UB2UIHallOfFame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (UB2UIHallOfFamePlayerInfo* pUI : PlayerInfoUI)
	{
		if (pUI && pUI->GetVisibility() != ESlateVisibility::Collapsed)
		{
			pUI->UpdatePosition(true);
		}
	}

}

void UB2UIHallOfFame::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (UIP_HallOfFameSkipButton.IsValid())
	{
		UIP_HallOfFameSkipButton->DestroySelf(InUIManager);
	}

	ClearPlayerInfo();
}

void UB2UIHallOfFame::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(MenuTab[0].Get()), &UB2UIHallOfFame::RedDotCondition_PvP)
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(MenuTab[1].Get()), &UB2UIHallOfFame::RedDotCondition_Team)
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(MenuTab[2].Get()), &UB2UIHallOfFame::RedDotCondition_Control)
}

void UB2UIHallOfFame::OnClick_PvP()
{
	if (auto* DocSome = UB2UIDocHelper::GetDocSome())
	{
		DocSome->SetHallOfFameMode(EHallOfFameMode::PVP);
		SceneForceUpdate();
	}
}

void UB2UIHallOfFame::OnClick_TeamMatch()
{
	if (auto* DocSome = UB2UIDocHelper::GetDocSome())
	{
		DocSome->SetHallOfFameMode(EHallOfFameMode::TEAM_MATCH);
		SceneForceUpdate();
	}
}

void UB2UIHallOfFame::OnClick_Controll()
{
	if (auto* DocSome = UB2UIDocHelper::GetDocSome())
	{
		DocSome->SetHallOfFameMode(EHallOfFameMode::CONTROL);
		SceneForceUpdate();
	}
}


void UB2UIHallOfFame::SetMenuTabSelected()
{
	if (auto* DocSome = UB2UIDocHelper::GetDocSome())
	{
		EHallOfFameMode CurrentFameMode = DocSome->GetHallOfFameMode();
		int32 FameModeIndex = static_cast<int32>(CurrentFameMode) - 2;
		int32 MenuTabCount = MenuTab.Num();

		for (int32 i = 0; i < MenuTabCount; ++i)
		{
			bool IsSelect = i == FameModeIndex;
			MenuTab[i]->SetTabSelected(IsSelect);
		}
	}
}

void UB2UIHallOfFame::SetSettleState()
{
	ClearPlayerInfo();

	//UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//	BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//	BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HallOfFrame_BlockEnter")),
	//	0.f,
	//	true,
	//	true,
	//	EUIMsgPopupButtonGroup::GotoLobby,
	//	FMsgPopupOnClick::CreateLambda([]() {GoToVillageClass<>::GetInstance().Signal(); }),
	//	0,
	//	true,
	//	EPopUpPriority::Server_Message_GoToLobby
	//);
}

void UB2UIHallOfFame::SetCelebratePopup(EHallOfFameMode InMode, int32 InRanking, TArray<b2network::B2RewardPtr>& InRewardItem)
{
	if (P_RankerPanel.IsValid())
	{
		P_RankerPanel->SetVisibility(ESlateVisibility::Visible);
	}

	// 绵窍钦聪促.
	if (TB_Celebrate.IsValid())
	{
		TB_Celebrate->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HallOfFame_Celebrate")));
	}

	// 老措老 措傈 瘤抄 矫琉 {0}困 崔己俊 己傍窍继嚼聪促!
	if (TB_Ranking.IsValid())
	{
		TB_Ranking->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HallOfFame_Ranking")), GetModeString(GetHallOfFameMode()), FText::AsNumber(InRanking)));
	}

	// 焊惑 : 
	if (TB_RewardInfo.IsValid())
	{
		TB_RewardInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HallOfFame_RewardInfo")));
	}

	// {0} 疙抗 林拳
	if (TB_Reward.IsValid())
	{
		if (InRewardItem.IsValidIndex(0) && InRewardItem[0]->raw_reward)
		{
			TB_Reward->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HallOfFame_Reward")), FText::AsNumber(InRewardItem[0]->raw_reward->count)));
		}
	}

	// 焊惑篮 快祈窃俊辑 荐飞 啊瓷钦聪促.
	if (TB_ToMail.IsValid())
	{
		TB_ToMail->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HallOfFame_ToMail")));
	}

	for (int32 i = 0; i < 3; ++i)
	{
		if (P_CelebrateRank.IsValidIndex(InRanking - 1))
		{
			P_CelebrateRank[i]->SetVisibility(i == InRanking - 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}

	PlayBestPopupAnimation();
}

void UB2UIHallOfFame::SetVisiblePraise(bool InPraiseButton, int32 InRanker)
{
	for (int32 i = 0; i < 3; ++i)
	{
		if (PlayerInfoUI.IsValidIndex(i))
		{
			PlayerInfoUI[i]->SetVisiblePraise(InPraiseButton, InRanker - 1 == i);
		}
	}
}

void UB2UIHallOfFame::SetPlayerInfo(const TArray<FRankerInfo>& Rankers)
{
	for (int32 i = 0; i < 3; ++i)
	{
		if (Rankers.IsValidIndex(i) && PlayerInfoUI.IsValidIndex(i))
		{
			PlayerInfoUI[i]->SetUserInfo(Rankers[i]);
		}
	}
	DoMarkRedDot();
}


void UB2UIHallOfFame::OpenPraiseToolTip()
{
	//if (P_PraiseToolTip.IsValid())
	//{
	//	P_PraiseToolTip->SetVisibility(ESlateVisibility::HitTestInvisible);
	//}

	//if (BTN_ClosePraiseToolTip.IsValid())
	//{
	//	BTN_ClosePraiseToolTip->SetVisibility(ESlateVisibility::Visible);
	//}

	if (auto* pUI = CreateWidget<UB2UIHallOfFameToolTip>(GetWorld(), ToolTipInfo))
	{

		FVector2D ScreenSize(1.f, 1.f);
		UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
		if (ViewportClient != nullptr)
			ViewportClient->GetViewportSize(ScreenSize);

		pUI->Init();
		pUI->AddToViewport(12);
		pUI->SetPositionInViewport(FVector2D(0, 0));
		pUI->SetVisibility(ESlateVisibility::Visible);
		ToolTipWidget = pUI;
	}
}

void UB2UIHallOfFame::ClosePraiseToolTip()
{
	if (BTN_ClosePraiseToolTip.IsValid())
	{
		BTN_ClosePraiseToolTip->SetVisibility(ESlateVisibility::Collapsed);
	}

	ToolTipWidget = nullptr;
}


void UB2UIHallOfFame::OpenCelebrate()
{
	if (P_RankerPanel.IsValid())
	{
		P_RankerPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}


void UB2UIHallOfFame::CloseCelebrate()
{
	if (P_RankerPanel.IsValid())
	{
		P_RankerPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UB2UIHallOfFame::RedDotCondition_PvP()
{
	return UB2UIDocHelper::GetDocSome() && UB2UIDocHelper::GetDocSome()->GetPraiseInfo(EHallOfFameMode::PVP);
}


bool UB2UIHallOfFame::RedDotCondition_Team()
{
	return UB2UIDocHelper::GetDocSome() && UB2UIDocHelper::GetDocSome()->GetPraiseInfo(EHallOfFameMode::TEAM_MATCH);
}


bool UB2UIHallOfFame::RedDotCondition_Control()
{
	return UB2UIDocHelper::GetDocSome() && UB2UIDocHelper::GetDocSome()->GetPraiseInfo(EHallOfFameMode::CONTROL);
}


EHallOfFameMode UB2UIHallOfFame::GetHallOfFameMode() const
{
	return UB2UIDocHelper::GetDocSome() ? UB2UIDocHelper::GetDocSome()->GetHallOfFameMode() : EHallOfFameMode::PVP;
}

FText UB2UIHallOfFame::GetModeString(EHallOfFameMode InFameMode)
{
	switch (InFameMode)
	{
	case EHallOfFameMode::PVP:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_TitleName"));
	case EHallOfFameMode::TEAM_MATCH:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch"));
	case EHallOfFameMode::CONTROL:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_Control"));
	default:
		break;
	}
	return FText();
}

void UB2UIHallOfFame::AddPlayer(const FHallOfFameCharacterInfo& PlayerInfo, bool IsVisible)
{
	auto* pUI = CreateWidget<UB2UIHallOfFamePlayerInfo>(GetWorld(), PlayerInfoClass);

	if (pUI)
	{
		pUI->Init();
		pUI->AddToViewport(9);
		pUI->SetPositionInViewport(FVector2D(0.0f, 0.0f));
		//pUI->SetTargetActor(PlayerInfo.HallOfFameCharacter);
		pUI->SetVisible(IsVisible);
		pUI->SetAdjustPosition();

		PlayerInfoUI.Add(pUI);
	}
}


void UB2UIHallOfFame::SetHallOfFamePraiseInfo(const b2network::B2HallOfFamePraisePtr& PraiseInfo, int32 InRanking)
{
	if (PraiseInfo)
	{
		if (auto* DocSome = UB2UIDocHelper::GetDocSome())
		{
			DocSome->SetPraiseInfo(PraiseInfo);
		}
		SetHallOfFamePraiseInfo(PraiseInfo->max_count, PraiseInfo->praise_count, InRanking);
		DoMarkRedDot();
	}
}

void UB2UIHallOfFame::SetHallOfFamePraiseInfo(int32 InMaxPraise, int32 InCurrentPraise, int64 InRanking)
{
	if (TB_LikeNUM.IsValid())
	{
		TB_LikeNUM->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillLevel")), FText::AsNumber(InMaxPraise - InCurrentPraise), FText::AsNumber(InMaxPraise)));
	}

	SetVisiblePraise(InMaxPraise > InCurrentPraise, InRanking);
	DoMarkRedDot();
}


void UB2UIHallOfFame::SetVisiblePlayerInfo(bool InVisible)
{
	for (auto* pUI : PlayerInfoUI)
	{
		if (pUI)
		{
			pUI->SetVisible(InVisible);
		}
	}
}

void UB2UIHallOfFame::SetVisibleForMatinee(bool InVisible)
{
	if (UIP_HallOfFameSkipButton.IsValid())
	{
		UIP_HallOfFameSkipButton->SetActive(InVisible);
	}

	if (P_MenuTab.IsValid())
	{
		P_MenuTab->SetVisibility(InVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}

	if (OV_LikeInfo.IsValid())
	{
		OV_LikeInfo->SetVisibility(InVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}

	if (OV_SeasonMsg.IsValid())
	{
		OV_SeasonMsg->SetVisibility(InVisible ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	SetVisiblePlayerInfo(!InVisible);
}

void UB2UIHallOfFame::SceneForceUpdate()
{
	if (auto* B2GM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		B2GM->SceneForceUpdate();
	}
}

void UB2UIHallOfFame::ClearPlayerInfo()
{
	for (auto* pUI : PlayerInfoUI)
	{
		if (pUI)
		{
			pUI->DestroySelf(UB2UIManager::GetInstance());
			pUI = nullptr;
		}
	}

	PlayerInfoUI.Empty();
}
