// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIModRewardBox.h"
#include "Retailer.h"
#include "B2UIModRewardBoxDetailInfo.h"
#include "B2UIDocHelper.h"



void UB2UIModRewardBox::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock,	TB_BoxName);
	GET_SLOT(UProgressBar,	PB_Progress);
	GET_SLOT(UTextBlock,	TB_Progress);
	GET_SLOT(UB2Image,		IMG_Box);
	GET_SLOT(UB2Button,		BTN_Reward);
	GET_SLOT(UB2Button,		BTN_Questionmark);
	GET_SLOT(UPanelWidget,	CP_Progress);
	GET_SLOT(UOverlay,		OV_MVPKeyText);
	GET_SLOT(UB2Image,		IMG_CountMark);
	GET_SLOT(UB2Button,		BTN_Detail);
	GET_SLOT(UPanelWidget,	CP_RewardBtnPanel);
	GET_SLOT(UTextBlock,	TB_ModExplanation);
	GET_SLOT(UTextBlock,	TB_CreateBtn);

	GET_SLOT(UPanelWidget, HB_Defeat);
}

void UB2UIModRewardBox::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Reward, &UB2UIModRewardBox::OnClickBTN_Reward);
	BIND_CLICK_FUNC_TO_BTN(BTN_Questionmark, &UB2UIModRewardBox::OnClickBTN_Questionmark);
	BIND_CLICK_FUNC_TO_BTN(BTN_Detail, &UB2UIModRewardBox::OnClickBTN_Detail);
}

void UB2UIModRewardBox::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION_LAMBDA(GetRedDotWidget(), [this](){ return this->bIsReward; });
}

void UB2UIModRewardBox::UpdateStaticText()
{
	if(TB_CreateBtn.IsValid())
		TB_CreateBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));
}

void UB2UIModRewardBox::Init()
{
	Super::Init();

	bIsReward = false;
}

void UB2UIModRewardBox::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIModRewardBox::OnClickBTN_Reward()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		if (BTN_Reward.IsValid())
			BTN_Reward->SetIsEnabled(false);

		TheWorld->GetTimerManager().ClearTimer(ButtonTimerhandle);
		data_trader::Retailer::GetInstance().RequestOpenDuelRewardBox(ModType);
		TheWorld->GetTimerManager().SetTimer(ButtonTimerhandle, this, &UB2UIModRewardBox::SetButtonEnable, 1.0f, false);
	}
}


void UB2UIModRewardBox::OnClickBTN_Questionmark()
{
	data_trader::Retailer::GetInstance().RequestGetDuelRewardBoxInfo(ModType);
}


void UB2UIModRewardBox::OnClickBTN_Detail()
{
	data_trader::Retailer::GetInstance().RequestGetDuelRewardBoxGainableList(ModType, m_nGrade);
}

void UB2UIModRewardBox::SetModRewardBox(bool IsRewardState, int32 ServerModType, int32 BoxGrade, int32 WinCount, int32 MaxCount, int32 DefeatCount /*= 0*/)
{
	bIsReward = IsRewardState;
	ModType = ServerModType;
	m_nGrade = BoxGrade;


	bool RewardBoxResult = DdfeatMaxCount == DefeatCount && IsRewardState == false;

	if(BTN_Detail.IsValid())
		BTN_Detail->SetVisibility(m_nGrade == b2network::B2RewardBoxGrade::REWARD_BOX_NONE ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (CP_Progress.IsValid())
	{
		if (bIsReward || RewardBoxResult)
		{
			CP_Progress->SetVisibility(ESlateVisibility::Hidden);
		}
		else 
		{
			CP_Progress->SetVisibility(ESlateVisibility::Visible);
		}
		//CP_Progress->SetVisibility(RewardBoxResult&&IsRewardState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (BTN_Reward.IsValid())
		BTN_Reward->SetVisibility(bIsReward ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (CP_RewardBtnPanel.IsValid())
		CP_RewardBtnPanel->SetVisibility(bIsReward ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	OpenRewardFx(DdfeatMaxCount == DefeatCount ? true : false);

	if (TB_ModExplanation.IsValid())
		TB_ModExplanation->SetVisibility(RewardBoxResult ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	SetModRewardBoxType(ServerModType, DefeatCount == INDEX_NONE ? true : false);
	SetModRewardBoxProgress(WinCount, MaxCount);
	SetModRewardBoxGrade(BoxGrade, DefeatCount == INDEX_NONE ? true : false , DefeatCount, MaxCount);

	DefeatCountEvent(DefeatCount);
	DoMarkRedDot();
}

void UB2UIModRewardBox::SetModRewardBoxType(int32 servertype, bool IsMvp)
{
	bool bAssault = servertype == b2network::B2DuelMode::DUEL_MODE_ASSAULT;

	if(HB_Defeat.IsValid())
		HB_Defeat->SetVisibility(IsMvp ? ESlateVisibility::Hidden : ESlateVisibility::Visible);		//Lose표시부분

	if(OV_MVPKeyText.IsValid())
		OV_MVPKeyText->SetVisibility(IsMvp ? ESlateVisibility::Visible : ESlateVisibility::Hidden);	//MVP일때 텍스트

}

void UB2UIModRewardBox::SetModRewardBoxProgress(int32 WinCount, int32 MaxCount)
{
	if (PB_Progress.IsValid())
		PB_Progress->SetPercent(float(WinCount) / float(MaxCount));

	if (TB_Progress.IsValid())
		TB_Progress->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL,TEXT("duelRewardBox_Progress")),FText::AsNumber(WinCount), FText::AsNumber(MaxCount)));

}

void UB2UIModRewardBox::SetModRewardBoxGrade(int32 nGrade , bool IsMvp , int32 DefaultWinCount, int32 WinMax)
{
	
	IsMvp ? SetMVPImageGradeEvent() :SetBoxImageGradeEvent(nGrade);

	FString GradeBoxString;

	if (!TB_BoxName.IsValid())
		return;

	if (IsMvp)
	{
		GradeBoxString = TEXT("RewardBox_MVP");
	}
	else
	{
		if (nGrade == b2network::B2RewardBoxGrade::REWARD_BOX_NONE)
		{
			if (TB_BoxName.IsValid())
				TB_BoxName->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("duelRewardBox_Default")), FText::AsNumber(WinMax)));

			return;
		}
		switch (nGrade)
		{
		case b2network::B2RewardBoxGrade::REWARD_BOX_BRONZE:
			GradeBoxString = TEXT("duelRewardBox_Bronze");
			break;
		case b2network::B2RewardBoxGrade::REWARD_BOX_SILVER:
			GradeBoxString = TEXT("duelRewardBox_Silver");
			break;
		case b2network::B2RewardBoxGrade::REWARD_BOX_GOLD:
			GradeBoxString = TEXT("duelRewardBox_Gold");
			break;
		case b2network::B2RewardBoxGrade::REWARD_BOX_PLATINUM:
			GradeBoxString = TEXT("duelRewardBox_Platinum");
			break;
		case b2network::B2RewardBoxGrade::REWARD_BOX_DIAMOND:
			GradeBoxString = TEXT("duelRewardBox_Diamond");
			break;
		case b2network::B2RewardBoxGrade::REWARD_BOX_MASTER:
			GradeBoxString = TEXT("duelRewardBox_Master");
			break;
		}
		if (TB_BoxName.IsValid())
			TB_BoxName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, GradeBoxString));
	}

}


void UB2UIModRewardBox::SetButtonEnable()
{
	if (BTN_Reward.IsValid())
		BTN_Reward->SetIsEnabled(true);
}

void UB2UIModRewardBox::SetModExplanation(FText InExplanation)
{
	if(TB_ModExplanation.IsValid())
		TB_ModExplanation->SetText(InExplanation);
}

void UB2UIMvpRewardBox::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UButton, BTN_RewardBox);
	GET_SLOT(UPanelWidget, CP_Reward);
	GET_SLOT(UPanelWidget, CP_Progress);
	GET_SLOT(UProgressBar, PB_Progress);
	GET_SLOT(UTextBlock, TB_ProgressText);
	GET_SLOT(UTextBlock, TB_MVPKeyText);
	GET_SLOT(UB2Button, BTN_Detail);
}

void UB2UIMvpRewardBox::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Detail, &UB2UIMvpRewardBox::OnClickBTN_Detail);
	BIND_CLICK_FUNC_TO_BTN(BTN_RewardBox, &UB2UIMvpRewardBox::OnClickBTN_RewardBox);
}

void UB2UIMvpRewardBox::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION_LAMBDA(GetRedDotWidget(), [this]() { return this->bIsReward; });
}

void UB2UIMvpRewardBox::Init()
{
	Super::Init();
	bIsReward = false;
}

void UB2UIMvpRewardBox::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIMvpRewardBox::SetMvpBox(int32 CurKey, int32 MaxKey)
{
	bIsReward = CurKey >= MaxKey ? true : false;

	if (CP_Reward.IsValid())
		CP_Reward->SetVisibility(bIsReward ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (CP_Progress.IsValid())
		CP_Progress->SetVisibility(bIsReward ? ESlateVisibility::Hidden : ESlateVisibility::Visible);


	if (TB_MVPKeyText.IsValid())
	{
		TB_MVPKeyText->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("duelRewardBox_Key_Notice")), FText::AsNumber(MaxKey)));
	}

	if (PB_Progress.IsValid())
		PB_Progress->SetPercent(float(CurKey) / float(MaxKey));

	if (TB_ProgressText.IsValid())
		TB_ProgressText->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("duelRewardBox_Progress")), FText::AsNumber(CurKey), FText::AsNumber(MaxKey)));

	OpenRewardFx(bIsReward);
	DoMarkRedDot();
}

void UB2UIMvpRewardBox::OnClickBTN_Detail()
{
	auto* UIManager = UB2UIManager::GetInstance();
	auto* ControlDoc = UB2UIDocHelper::GetDocControl();

	if (UIManager)
	{
		auto BoxGradeUI = UIManager->OpenUI<UB2UIModRewardBoxDetailInfo>(UIFName::ModRewardBoxDetail);

		BoxGradeUI->SetPanelClear();;
		if (BoxGradeUI)
		{
			//BoxGradeUI->SetPanelClear();
			BoxGradeUI->MVPEvent();

			auto itemList = ControlDoc->GetAssaultMvpRewardInfo();

			for (int32 i = 0; i < itemList.Num(); ++i)
			{
				BoxGradeUI->SetRewardIcon(itemList[i]->reward_id, itemList[i]->reward_min_count, itemList[i]->reward_max_count, i);
			}
		}
	}
}

void UB2UIMvpRewardBox::OnClickBTN_RewardBox()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		if (BTN_RewardBox.IsValid())
			BTN_RewardBox->SetIsEnabled(false);

		TheWorld->GetTimerManager().ClearTimer(ButtonTimerhandle);
		data_trader::Retailer::GetInstance().RequestReceiveAssaultMvpReward();
		TheWorld->GetTimerManager().SetTimer(ButtonTimerhandle, this, &UB2UIMvpRewardBox::SetButtonEnable, 1.0f, false);
	}
}


void UB2UIMvpRewardBox::SetButtonEnable()
{
	if (BTN_RewardBox.IsValid())
		BTN_RewardBox->SetIsEnabled(true);
}
