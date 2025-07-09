// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIInventorySetInfoNotify.h"

void UB2UIInventorySetInfoNotify::CacheAssets()
{
	Super::CacheAssets();
	
	GET_SLOT(UTextBlock, TB_CombatLable);
}

void UB2UIInventorySetInfoNotify::BindDelegates()
{
	Super::BindDelegates();
}


void UB2UIInventorySetInfoNotify::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateValueAmount();

	ClosePanel(InDeltaTime);
}


void UB2UIInventorySetInfoNotify::UpdateValueAmount()
{
	if (m_fValueChangeStartTime == -1.0f)
		return;

	float fElapseTime = GetWorld()->GetTimeSeconds() - m_fValueChangeStartTime;

	float fTimeTotalDuration = ValueActionTime;

	if (fElapseTime >= fTimeTotalDuration)
	{
		m_fValueChangeStartTime = -1.0f;
		m_endTime = 0.5f;
		return;
	}
}


void UB2UIInventorySetInfoNotify::ClosePanel(float InDeltaTime)
{
	if (m_fValueChangeStartTime < 0)
	{
		m_endTime -= InDeltaTime;
		if (m_endTime < 0)
			UB2UIManager::GetInstance()->CloseUI(UIFName::SetInfoNotify);
	}
}

void UB2UIInventorySetInfoNotify::Init()
{
	Super::Init();

	m_fValueChangeStartTime = -1;
	m_endTime = 0.0f;
}

void UB2UIInventorySetInfoNotify::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIInventorySetInfoNotify::SetItemEffectType(ESetItemEffectNotify SetItemEffectType, const bool& InIsCostumeItem /*= false*/)
{
	FText SetItemStr;

	switch (SetItemEffectType)
	{
	case ESetItemEffectNotify::SIEN_Add:
		if (InIsCostumeItem)
		{
			SetItemStr = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_AddSetItemEffect"));
		}
		else
		{
			SetItemStr = BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("LobbyInven_AddSetItemEffect")));
		}
		break;
	case ESetItemEffectNotify::SIEN_Delete:
		if (InIsCostumeItem)
		{
			SetItemStr = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_DeleteSetItemEffect"));
		}
		else
		{
			SetItemStr = BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("LobbyInven_DeleteSetItemEffect")));
		}
		break;
	case ESetItemEffectNotify::SIEN_Change:
		if (InIsCostumeItem)
		{
			SetItemStr = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_ChangeSetItemEffect"));
		}
		else
		{
			SetItemStr = BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("LobbyInven_ChangeSetItemEffect")));
		}
		break;
	}

	if (TB_CombatLable.IsValid())
		TB_CombatLable->SetText(SetItemStr);

	m_fValueChangeStartTime = GetWorld()->GetTimeSeconds();
}


