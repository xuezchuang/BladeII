// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIBattleScoreNotify.h"
#include "B2UIPowerValueChange.h"


void UB2UIBattleScoreNotify::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_CombatLable);
	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UB2UIPowerValueChange, UIP_PowerValueChange);
	GET_SLOT(UImage, IMG_CharacterPortrait);

	if (UIP_PowerValueChange.IsValid())
		UIP_PowerValueChange->Init();
}

void UB2UIBattleScoreNotify::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIBattleScoreNotify::Init()
{
	Super::Init();

	m_nCurValueAmount = 0;
	m_nGoalValueAmount = -1;
	m_fValueChangeStartTime = -1.0f;
	m_endTime = 0.0f;
	ValueActionTime = 1.0f;
}

void UB2UIBattleScoreNotify::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIBattleScoreNotify::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UpdateValueAmount();
	
	//끝나는 시간 맞춰서 위젯 종료
	//if(m_endTime > 0) 
		ClosePanel(InDeltaTime);
}

void UB2UIBattleScoreNotify::UpdateStaticText()
{
	if (TB_CombatLable.IsValid())
	{
		TB_CombatLable->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));
	}
}

void UB2UIBattleScoreNotify::SetAmount(int32 NewValueAmount, int32 beforeValueAmount, EPCClass ClassType)
{
	m_nCurValueAmount = beforeValueAmount;
	m_nOrgValueAmount = m_nCurValueAmount;
	m_nGoalValueAmount = NewValueAmount;
	m_fValueChangeStartTime = GetWorld()->GetTimeSeconds();
	SetMarginScore(m_nGoalValueAmount - m_nCurValueAmount);

	if (IMG_CharacterPortrait.IsValid())
	{
		IMG_CharacterPortrait->SetVisibility(ClassType != EPCClass::EPC_End ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		IMG_CharacterPortrait->SetBrushFromMaterial(FB2ClassPortraitImage::GetCharImage(PortraitInfo, ClassType));
	}
}

void UB2UIBattleScoreNotify::SetMarginScore(int32 nMarginScore)
{

	if (UIP_PowerValueChange.IsValid())
	{
		UIP_PowerValueChange->SetValueAndPlay(nMarginScore);
	}
}

void UB2UIBattleScoreNotify::SetBattleScore(int32 NewValueAmount)
{
	m_nCurValueAmount = NewValueAmount;

	if (TB_CombatPower.IsValid())
	{
		TB_CombatPower->SetText(FText::AsNumber(NewValueAmount)); // (목표-현재)값 출력
	}
}

void UB2UIBattleScoreNotify::UpdateValueAmount()
{
	if (m_fValueChangeStartTime == -1.0f)
		return;

	float fElapseTime = GetWorld()->GetTimeSeconds() - m_fValueChangeStartTime;

	float fTimeTotalDuration = ValueActionTime;

	if (fElapseTime >= fTimeTotalDuration)
	{
		SetBattleScore(m_nGoalValueAmount);
		m_fValueChangeStartTime = -1.0f;
		m_endTime = 0.5f;
		return;
	}

	// UI 애니메이션 전투력 텍스트
	int32 nAmount = int32(m_nOrgValueAmount + ((m_nGoalValueAmount - m_nOrgValueAmount) * (fElapseTime / fTimeTotalDuration)));
	SetBattleScore(nAmount);

}

void UB2UIBattleScoreNotify::ClosePanel(float InDeltaTime)
{
	if (m_fValueChangeStartTime < 0)
	{
		m_endTime -= InDeltaTime;
		if (m_endTime < 0)
		{
			UB2UIManager::GetInstance()->CloseUI(UIFName::UpgradeStatNotify);
			UB2UIManager::GetInstance()->CloseUI(UIFName::BattleScoreNotify);
		}
	}
}