// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIModRewardBoxGradeInfo.h"

#include "B2UIManager.h"
#include "Retailer.h"

void UB2UIModRewardBoxGradeInfo::CacheAssets()
{
	Super::CacheAssets();

	CountTextArr.SetNum(6);
	GET_SLOT_BYNAME(UTextBlock, CountTextArr[0], TB_BronzeWinCount);
	GET_SLOT_BYNAME(UTextBlock, CountTextArr[1], TB_SilverWinCount);
	GET_SLOT_BYNAME(UTextBlock, CountTextArr[2], TB_GoldWinCount);
	GET_SLOT_BYNAME(UTextBlock, CountTextArr[3], TB_PlatinumWinCount);
	GET_SLOT_BYNAME(UTextBlock, CountTextArr[4], TB_DiaWinCount);
	GET_SLOT_BYNAME(UTextBlock, CountTextArr[5], TB_MasterWinCount);

	GET_SLOT(UB2Button, BTN_Bronze);
	GET_SLOT(UB2Button, BTN_Silver);
	GET_SLOT(UB2Button, BTN_Gold);
	GET_SLOT(UB2Button, BTN_Platinum);
	GET_SLOT(UB2Button, BTN_Dia);
	GET_SLOT(UB2Button, BTN_Master);

	GET_SLOT(UB2Button, BTN_Close);
	
}

void UB2UIModRewardBoxGradeInfo::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIModRewardBoxGradeInfo::OnClickClose);


	BIND_CLICK_FUNC_TO_BTN(BTN_Bronze, &UB2UIModRewardBoxGradeInfo::OnClickBronze);
	BIND_CLICK_FUNC_TO_BTN(BTN_Silver, &UB2UIModRewardBoxGradeInfo::OnClickSilver);
	BIND_CLICK_FUNC_TO_BTN(BTN_Gold, &UB2UIModRewardBoxGradeInfo::OnClickGold);
	BIND_CLICK_FUNC_TO_BTN(BTN_Platinum, &UB2UIModRewardBoxGradeInfo::OnClickPlatinum);
	BIND_CLICK_FUNC_TO_BTN(BTN_Dia, &UB2UIModRewardBoxGradeInfo::OnClickDia);
	BIND_CLICK_FUNC_TO_BTN(BTN_Master, &UB2UIModRewardBoxGradeInfo::OnClickMaster);

}


void UB2UIModRewardBoxGradeInfo::SetBoxGrade(int32 InModeType ,int32 Grade, int32 WinCount)
{
	if (b2network::B2RewardBoxGrade::REWARD_BOX_MASTER != CountTextArr.Num())
		return;

	ModType = InModeType;

	CountTextArr[Grade - 1]->SetText(FText::AsNumber(WinCount));
}

void UB2UIModRewardBoxGradeInfo::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIModRewardBoxGradeInfo::OnClickClose()
{
	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
	{
		UIManager->CloseUI(UIFName::ModRewardBoxList);
	}
}
void UB2UIModRewardBoxGradeInfo::OnClickBronze()
{
	data_trader::Retailer::GetInstance().RequestGetDuelRewardBoxGainableList(ModType, b2network::B2RewardBoxGrade::REWARD_BOX_BRONZE);
}
void UB2UIModRewardBoxGradeInfo::OnClickSilver()
{
	data_trader::Retailer::GetInstance().RequestGetDuelRewardBoxGainableList(ModType, b2network::B2RewardBoxGrade::REWARD_BOX_SILVER);
}
void UB2UIModRewardBoxGradeInfo::OnClickGold()
{
	data_trader::Retailer::GetInstance().RequestGetDuelRewardBoxGainableList(ModType, b2network::B2RewardBoxGrade::REWARD_BOX_GOLD);
}
void UB2UIModRewardBoxGradeInfo::OnClickPlatinum()
{
	data_trader::Retailer::GetInstance().RequestGetDuelRewardBoxGainableList(ModType, b2network::B2RewardBoxGrade::REWARD_BOX_PLATINUM);
}
void UB2UIModRewardBoxGradeInfo::OnClickDia()
{
	data_trader::Retailer::GetInstance().RequestGetDuelRewardBoxGainableList(ModType, b2network::B2RewardBoxGrade::REWARD_BOX_DIAMOND);
}
void UB2UIModRewardBoxGradeInfo::OnClickMaster()
{
	data_trader::Retailer::GetInstance().RequestGetDuelRewardBoxGainableList(ModType, b2network::B2RewardBoxGrade::REWARD_BOX_MASTER);
}

void UB2UIModRewardBoxGradeInfo::Init()
{

	Super::Init();
}

void UB2UIModRewardBoxGradeInfo::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


