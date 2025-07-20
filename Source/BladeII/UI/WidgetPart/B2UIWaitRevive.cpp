// Fill out your copyright notice in the Description page of Project Settings.


#include "B2UIWaitRevive.h"
#include "B2NetGameMode.h"
#include "B2UIManager.h"
#include "B2ControlGameMode.h"


void UB2UIWaitRevive::Init()
{
	//Super::Init();

	//ResetTimer();

	//// 硅撇皋牢UI见辫
	//UUserWidget* pUI = UB2UIManager::GetInstance()->GetUI<UUserWidget>(UIFName::BattleMain);
	//if (pUI)
	//{
	//	BattleMainUiOrgVisibility = pUI->GetVisibility();
	//	pUI->SetVisibility(ESlateVisibility::Hidden);
	//}
	//
	//// 捞悼菩澄见辫
	//APlayerController* LocalController = UGameplayStatics::GetLocalPlayerController(this);
	//if (LocalController)
	//	LocalController->SetVirtualJoystickVisibility(false);
}

void UB2UIWaitRevive::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//if (m_fCheckStartTime != -1 && m_fWaitTime != -1)
	//{
	//	float fElapseTime = GetWorld()->GetTimeSeconds() - m_fCheckStartTime;
	//	
	//	float fRemainTime = m_fWaitTime - fElapseTime;

	//	if (fRemainTime < 0)
	//	{
	//		ResetTimer();

	//		// 硅撇皋牢UI焊烙
	//		UUserWidget* pUI = UB2UIManager::GetInstance()->GetUI<UUserWidget>(UIFName::BattleMain);
	//		if (pUI)
	//		{
	//			pUI->SetVisibility(BattleMainUiOrgVisibility);
	//		}

	//		// 捞悼菩澄焊烙
	//		APlayerController* LocalController = UGameplayStatics::GetLocalPlayerController(this);
	//		if (LocalController)
	//			LocalController->SetVirtualJoystickVisibility(true);

	//		// 努扼捞攫飘 碍力 何劝
	//		if (AB2NetGameMode* pGM = Cast<AB2NetGameMode>(UGameplayStatics::GetGameMode(this)))
	//			pGM->ForceResurrectLocalPlayer();

	//		//痢飞傈矫 混酒车促绊 舅覆
	//		if (AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this)))
	//		{
	//			pGM->SetWaitRevive(false);
	//		}

	//		UB2UIManager::GetInstance()->CloseUI(UIFName::WaitRevive);

	//		return;
	//	}

	//	if (TB_RemainTime.IsValid())
	//	{
	//		TB_RemainTime->SetText(FText::AsNumber(int32(fRemainTime)));
	//	}
	//}
}

void UB2UIWaitRevive::InitTimer(float fWaitTime)
{
	m_fWaitTime = fWaitTime;
	m_fCheckStartTime = GetWorld()->GetTimeSeconds();
}

void UB2UIWaitRevive::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_RemainTime);
}

void UB2UIWaitRevive::ResetTimer()
{
	m_fWaitTime = -1;
	m_fCheckStartTime = -1;
}

