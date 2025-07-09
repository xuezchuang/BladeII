// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIControlMvpViewPlayerInfo.h"
#include "BladeIINetPlayer.h"
#include "B2NetGameMode.h"
#include "Retailer.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "B2UIDocHelper.h"
#include "B2UIDocControl.h"



UB2UIControlMvpViewPlayerInfo::UB2UIControlMvpViewPlayerInfo(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	m_nKillCount = 0;
	m_nPlayerNetId = 0;

	m_pTargetActor = NULL;

	AdjustPosition = FVector2D(0.0f, 0.0f);
}

void UB2UIControlMvpViewPlayerInfo::SetTargetActor(class AActor* TargetActor)
{
	m_pTargetActor = TargetActor;
}

void UB2UIControlMvpViewPlayerInfo::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, TB_LvNickName);
	GET_SLOT(UTextBlock, TB_KillCount);
	GET_SLOT(UTextBlock, TB_Contribution);
	GET_SLOT(UCanvasPanel, C_Mvp);
	GET_SLOT(UB2Button, BTN_AskFriend);

	if (C_Mvp.IsValid())
		C_Mvp->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIControlMvpViewPlayerInfo::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_AskFriend, &UB2UIControlMvpViewPlayerInfo::OnClickAskFriend);
}

void UB2UIControlMvpViewPlayerInfo::OnClickAskFriend()
{
	// 친구 신청
	// 요청보내구 버튼 하이드.
	AB2NetGameMode* pGM = Cast<AB2NetGameMode>(UGameplayStatics::GetGameMode(this));
	
	if (pGM && m_nPlayerNetId != 0)
	{
		int64 nAccountId = pGM->GetRemotePlayerInfoData(m_nPlayerNetId).GetAccountId();

		data_trader::Retailer::GetInstance().RequestAskFriend(nAccountId);
	}	

	if (BTN_AskFriend.IsValid())
		BTN_AskFriend->SetVisibility(ESlateVisibility::Hidden);

	ABladeIIGameMode::SetHUDCenterMessage(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("FriendAdd")), 1.0f);
}

void UB2UIControlMvpViewPlayerInfo::SetPlayer(class ABladeIIPlayer* pPlayer)
{
	// 플레이어 정보 셋팅
	EPCClass PcClass = pPlayer->GetCharacterDataStore()->GetMainPlayerClass();
	int32 nLevel = pPlayer->GetCharacterDataStore()->GetCharacterLevel(PcClass);
	FString strName = pPlayer->GetCharacterDataStore()->GetUserNickName();

	ABladeIINetPlayer* pNetPlayer = Cast<ABladeIINetPlayer>(pPlayer);

	if (pNetPlayer)
	{
		m_nPlayerNetId = pNetPlayer->GetNetId();
		m_nKillCount = UB2UIDocHelper::GetDocControl()->GetKillCount(pNetPlayer->GetNetId());
	}

	if (TB_LvNickName.IsValid())
	{
		FString strLvName = TEXT("Lv.") + FString::FromInt(nLevel) + TEXT(" ") + strName;
		TB_LvNickName->SetText(FText::FromString(strLvName));
	}

	if (TB_KillCount.IsValid())
	{
		FString strKillCount = TEXT("KILL : ") + FString::FromInt(m_nKillCount);
		TB_KillCount->SetText(FText::FromString(strKillCount));
	}
}

void UB2UIControlMvpViewPlayerInfo::SetContribution(int32 InContribution)
{
	if (TB_Contribution.IsValid())
	{
		FString ContributionStr = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Raid_Contribution")).ToString() + TEXT(":") + FString::FromInt(InContribution) + TEXT("%");
		TB_Contribution->SetText(FText::FromString(ContributionStr));
	}
}

void UB2UIControlMvpViewPlayerInfo::SetMvp()
{
	if (C_Mvp.IsValid())
		C_Mvp->SetVisibility(ESlateVisibility::Visible);
}

void UB2UIControlMvpViewPlayerInfo::SetHiddenKillCount()
{
	if (TB_KillCount.IsValid())
		TB_KillCount->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIControlMvpViewPlayerInfo::SetVisibleContribution()
{
	if (TB_Contribution.IsValid())
		TB_Contribution->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIControlMvpViewPlayerInfo::SetAskFriendBtnHidden()
{
	if (BTN_AskFriend.IsValid())
		BTN_AskFriend->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIControlMvpViewPlayerInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Super::NativeTick(MyGeometry, InDeltaTime);

	//// 위치 업데이트

	//APlayerController* PC = UGameplayStatics::GetLocalPlayerController(this);

	//FVector2D ProjectedLocation;
	//if (m_pTargetActor && PC && PC->ProjectWorldLocationToScreen(m_pTargetActor->GetActorLocation(), ProjectedLocation))
	//{
	//	ForceLayoutPrepass();

	//	FVector2D vUIPos = ProjectedLocation;

	//	vUIPos.X -= (GetDesiredSize().X * UWidgetLayoutLibrary::GetViewportScale(this)) / 2;
	//	vUIPos.Y -= GetDesiredSize().Y * UWidgetLayoutLibrary::GetViewportScale(this);

	//	// BP에서 입력하는 보정값
	//	vUIPos.X += AdjustPosition.X * UWidgetLayoutLibrary::GetViewportScale(this);
	//	vUIPos.Y += AdjustPosition.Y * UWidgetLayoutLibrary::GetViewportScale(this);

	//	SetPositionInViewport(vUIPos);
	//}
}
