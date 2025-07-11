// Fill out your copyright notice in the Description page of Project Settings.


#include "B2UIWaitRevive.h"
#include "B2NetGameMode.h"
#include "B2UIManager.h"
#include "B2ControlGameMode.h"


void UB2UIWaitRevive::Init()
{
	//Super::Init();

	//ResetTimer();

	//// ��Ʋ����UI����
	//UUserWidget* pUI = UB2UIManager::GetInstance()->GetUI<UUserWidget>(UIFName::BattleMain);
	//if (pUI)
	//{
	//	BattleMainUiOrgVisibility = pUI->GetVisibility();
	//	pUI->SetVisibility(ESlateVisibility::Hidden);
	//}
	//
	//// �̵��гμ���
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

	//		// ��Ʋ����UI����
	//		UUserWidget* pUI = UB2UIManager::GetInstance()->GetUI<UUserWidget>(UIFName::BattleMain);
	//		if (pUI)
	//		{
	//			pUI->SetVisibility(BattleMainUiOrgVisibility);
	//		}

	//		// �̵��гκ���
	//		APlayerController* LocalController = UGameplayStatics::GetLocalPlayerController(this);
	//		if (LocalController)
	//			LocalController->SetVirtualJoystickVisibility(true);

	//		// Ŭ���̾�Ʈ ���� ��Ȱ
	//		if (AB2NetGameMode* pGM = Cast<AB2NetGameMode>(UGameplayStatics::GetGameMode(this)))
	//			pGM->ForceResurrectLocalPlayer();

	//		//�������� ��Ƴ��ٰ� �˸�
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

