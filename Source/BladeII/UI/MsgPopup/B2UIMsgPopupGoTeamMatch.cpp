// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIManager.h"
#include "Retailer.h"
#include "B2UIMsgPopupGoTeamMatch.h"


#include "BladeIIGameImpl.h"

void UB2UIMsgPopupGoTeamMatch::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_GoTeamMatch);
	GET_SLOT(UB2Button, BTN_Close);

	BIND_CLICK_FUNC_TO_BTN(BTN_GoTeamMatch, &UB2UIMsgPopupGoTeamMatch::OnGoTeamMatchBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIMsgPopupGoTeamMatch::OnCloseBtnClick);
}

void UB2UIMsgPopupGoTeamMatch::OnGoTeamMatchBtnClick()
{
	if (BladeIIGameImpl::GetClientDataStore().ExistAccountInfo())
	{
		data_trader::Retailer::GetInstance().RequestGetTeamBattleStatus();
	}
	
	// 요청했으면 닫자
	DestroySelf(UB2UIManager::GetInstance());	
}

void UB2UIMsgPopupGoTeamMatch::OnCloseBtnClick()
{
	DestroySelf(UB2UIManager::GetInstance());
}
