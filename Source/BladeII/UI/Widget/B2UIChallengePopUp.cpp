// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIChallengePopUp.h"
#include "B2UIManager.h"
#include "../../Lobby/B2LobbySceneManager.h"




void UB2UIChallengePopUp::CacheAssets()
{
	Super::CacheAssets();
		
	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UB2Button, BTN_Close);

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_Confirm);
	GET_SLOT(UTextBlock, TB_Explanation);
	GET_SLOT(UTextBlock, TB_IconReward);
	GET_SLOT(UTextBlock, TB_IconName);
}

void UB2UIChallengePopUp::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIChallengePopUp::OnClickBTN_Confirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIChallengePopUp::OnClickBTN_Close);
}

void UB2UIChallengePopUp::Init()
{
	Super::Init();
}

void UB2UIChallengePopUp::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIChallengePopUp::SetPopupInfo(ELobbyModSceneElem InModEnum, bool bLock)
{
	MyMod = InModEnum;
	bOpenLock = bLock;
	FString fstrTitle;
	FString fstrExplan;
	FString fstrConfirm;
	
	TB_IconReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChapterStarRewardReceiveTitle")));
	
	switch (InModEnum)
	{
	case ELobbyModSceneElem::LMODIDX_PVP:
		fstrTitle = TEXT("ChallengeMod_PVP");
		fstrConfirm = TEXT("ChallengePopUpInfo_Pvp");
		break;
	case ELobbyModSceneElem::LMODIDX_TeamMatch:
		fstrTitle = TEXT("ChallengeMod_TeamMatch");
		fstrConfirm = TEXT("ChallengePopUpInfo_TeamMatch");
		break;
	case ELobbyModSceneElem::LMODIDX_HeroTop:
		fstrTitle = TEXT("ChallengeMod_HeroTop");
		fstrConfirm = TEXT("ChallengePopUpInfo_HeroTop");
		break;
	case ELobbyModSceneElem::LMODIDX_GuildMatch:
		fstrTitle = TEXT("ChallengeMod_GuildMatch");
		fstrConfirm = TEXT("ChallengePopUpInfo_GuildMatch");
		break;
	case ELobbyModSceneElem::LMODIDX_CounterDungeon:
		fstrTitle = TEXT("ChallengeMod_DayDungeon");
		fstrConfirm = TEXT("ChallengePopUpInfo_DayDungeon");
		break;
	case ELobbyModSceneElem::LMODIDX_Raid:
		fstrTitle = TEXT("ChallengeMod_Raid");
		fstrConfirm = TEXT("ChallengePopUpInfo_Raid");
		break;
	case ELobbyModSceneElem::LMODIDX_Control:
		fstrTitle = TEXT("ChallengeMod_Control");
		fstrConfirm = TEXT("ChallengePopUpInfo_Control");
		break;
	default:
		break;
	}
	if ( bLock)
		fstrExplan = TEXT("DevShowNotReadyYet");
	else
		fstrExplan = TEXT("Admission");

	SettingInfo(fstrTitle, fstrExplan, fstrConfirm);
	SetPopUpInfo_Bp(InModEnum);
}

void UB2UIChallengePopUp::SettingInfo(const FString& Title, const FString& Explan, const FString& Confrim)
{
	TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, Title));
	TB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, Explan));
	TB_Explanation->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, Confrim));
}

void UB2UIChallengePopUp::ClosePopUp()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::ChallengePopUp);
	OnClickDelegate.Unbind();
}

void UB2UIChallengePopUp::OnClickBTN_Confirm()
{
	if (!bOpenLock)
	{
		OnClickDelegate.ExecuteIfBound(MyMod);
	}
	ClosePopUp();
}

void UB2UIChallengePopUp::OnClickBTN_Close()
{
	ClosePopUp();
}


