// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIControlMvpView.h"
#include "B2UIControlMvpViewPlayerInfo.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "B2ControlGameMode.h"
#include "Retailer.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2UIMain.h"


UB2UIControlMvpView::UB2UIControlMvpView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_arPlayerInfoUI.Empty();

	m_ArMvpPlayers.Empty();

	m_bSendResult = false;
}

void UB2UIControlMvpView::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	for (UB2UIControlMvpViewPlayerInfo* pUI : m_arPlayerInfoUI)
	{
		pUI->DestroySelf(InUIManager);
	}

	m_arPlayerInfoUI.Empty();
}

void UB2UIControlMvpView::AddPlayer(AActor* TargetActor, class ABladeIIPlayer* pPlayer, bool bMvp)
{
	UB2UIControlMvpViewPlayerInfo* pUI = CreateWidget<UB2UIControlMvpViewPlayerInfo>(GetWorld(), MvpViewItemClass);

	if (pUI)
	{
		pUI->Init();

		pUI->AddToViewport(BII_WIDGET_ZORDER_NEW_UI_MAIN);
				
		pUI->SetTargetActor(TargetActor);
		pUI->SetPlayer(pPlayer);
				
		m_arPlayerInfoUI.Add(pUI);

		if (bMvp)
		{
			pUI->SetMvp();

			m_ArMvpPlayers.Add(pPlayer);
		}

		int64 PlayerAccountId = pPlayer->GetAccountId();
				
		if (pPlayer->IsLocalPlayer() || UB2UIDocHelper::GetDocFriend()->IsMyFriend(PlayerAccountId))
			pUI->SetAskFriendBtnHidden();

		pUI->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIControlMvpView::SetVisibilityPlayerUI(ESlateVisibility NewVisibility)
{
	for (UB2UIControlMvpViewPlayerInfo* pUI : m_arPlayerInfoUI)
	{
		pUI->SetVisibility(NewVisibility);
	}
}

void UB2UIControlMvpView::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Next);
}

void UB2UIControlMvpView::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Next, &UB2UIControlMvpView::OnClickBTN_Next);
}

void UB2UIControlMvpView::CloseWidgetDelegate()
{
	OnClickBTN_Next();
}

void UB2UIControlMvpView::OnClickBTN_Next()
{
	//if (m_bSendResult)
	//{
	//	return;
	//}

	//m_bSendResult = true;
	//
	////이벤트바인드
	//DeliveryAssaultResultTicket = DeliveryAssaultResultClass<FB2ResponseAssaultResultPtr>::GetInstance().Subscribe([this](FB2ResponseAssaultResultPtr MsgPtr) {

	//	DeliveryAssaultResultClass<FB2ResponseAssaultResultPtr>::GetInstance().Unsubscribe(DeliveryAssaultResultTicket);
	//	UB2UIDocHelper::GetDocControl()->SetResultPtr(MsgPtr);

	//	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	//	if (pGM)
	//	{
	//		pGM->SetResultScene();
	//	}
	//});

	//AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

	//if (pGM)
	//{
	//	ETeamType WinTeam = pGM->ResultWinTeam;
	//	ETeamType MyTeam = ETeamType(pGM->GetMyTeamNum());

	//	int32 MatchResult = WinTeam == ETeamType::ETT_End ? b2network::B2MatchResultType::BMRT_DRAW :
	//		WinTeam == MyTeam ? b2network::B2MatchResultType::BMRT_WIN : b2::protocol::commondata::MatchResult::LOSE;
	//			
	//	bool bMvp = m_ArMvpPlayers.Contains(pGM->GetLocalController()->GetPawn());

	//	auto battleInfo = std::make_shared<b2network::B2AssaultBattleInfo>();
	//	uint32 MyNetId = pGM->GetMyNetId();
	//	battleInfo->kill_count = UB2UIDocHelper::GetDocControl()->GetKillCount(MyNetId);
	//	battleInfo->death_count = UB2UIDocHelper::GetDocControl()->GetDeadCount(MyNetId);
	//	battleInfo->damage = pGM->GetActualDamage();
	//	data_trader::Retailer::GetInstance().RequestAssaultResult(pGM->GetToken(), MatchResult, bMvp, false, battleInfo);
	//	//pGM->SetResultScene();
	//}	
}
