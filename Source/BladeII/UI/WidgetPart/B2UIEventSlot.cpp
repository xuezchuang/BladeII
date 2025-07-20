// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEventSlot.h"

#include "B2UIManager.h"
#include "B2UIMailRewardPopUp.h"

void UB2UIEventSlot::SetEventState(int32 iState)
{
	iEventState = iState;
}

void UB2UIEventSlot::OpenMailRewardPopup()
{
	UB2UIManager::GetInstance()->OpenUI<UB2UIMailRewardPopUp>(UIFName::RewardMailPopup);
}


