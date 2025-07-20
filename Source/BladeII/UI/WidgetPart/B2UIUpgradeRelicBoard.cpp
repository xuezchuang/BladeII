// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIUpgradeRelicBoard.h"
#include "B2UIManager.h"

#include "RelicManager.h"
#include "B2LobbyUI_RelicManagement.h"
//UB2UIUpgradeRelicBoardBar
void UB2UIUpgradeRelicBoardBar::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UProgressBar, ProgressBar);
	GET_SLOT(UProgressBar, FX_ProgressBar_01);
	GET_SLOT(UProgressBar, FX_ProgressBar_02);
}

void UB2UIUpgradeRelicBoardBar::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIUpgradeRelicBoardBar::Init()
{
	Super::Init();
}

void UB2UIUpgradeRelicBoardBar::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIUpgradeRelicBoardBar::SetProgressValue(float nPercent)
{
	if (ProgressBar.IsValid())
		ProgressBar->SetPercent(nPercent);
	if (FX_ProgressBar_01.IsValid())
		FX_ProgressBar_01->SetPercent(nPercent);
	if (FX_ProgressBar_02.IsValid())
		FX_ProgressBar_02->SetPercent(nPercent);
}

//UB2UIUpgradeRelicBoardSlot
void UB2UIUpgradeRelicBoardSlot::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_Locked);
	GET_SLOT(UPanelWidget, P_Locked);
	GET_SLOT(UTextBlock, TB_UnLocked);
	GET_SLOT(UPanelWidget, P_UnLocked);
	GET_SLOT(UTextBlock, TB_Selected);
	GET_SLOT(UPanelWidget, P_Selected);

	GET_SLOT(UB2Image, IMG_Locked);
	GET_SLOT(UB2Image, IMG_UnLocked);
	GET_SLOT(UB2Image, IMG_Selected);
}

void UB2UIUpgradeRelicBoardSlot::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIUpgradeRelicBoardSlot::Init()
{
	Super::Init();
}

void UB2UIUpgradeRelicBoardSlot::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIUpgradeRelicBoardSlot::SetSlotText(int32 nSlotIndex)
{
	if(TB_Locked.IsValid())
		TB_Locked->SetText(FText::AsNumber(nSlotIndex));
	if (TB_UnLocked.IsValid())
		TB_UnLocked->SetText(FText::AsNumber(nSlotIndex));
	if (TB_Selected.IsValid())
		TB_Selected->SetText(FText::AsNumber(nSlotIndex));

	m_nSlotIndex = nSlotIndex -1;

}

void UB2UIUpgradeRelicBoardSlot::SetSlotVisibleState(int32 nType)
{
	if (P_Locked.IsValid() && P_UnLocked.IsValid() && P_Selected.IsValid())
	{
		if (nType == 0) // select
		{
			P_Selected->SetVisibility(ESlateVisibility::Visible);
			P_Locked->SetVisibility(ESlateVisibility::Hidden);
			P_UnLocked->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (nType == 1) // Lock//
		{
			P_Selected->SetVisibility(ESlateVisibility::Hidden);
			P_Locked->SetVisibility(ESlateVisibility::Visible);
			P_UnLocked->SetVisibility(ESlateVisibility::Hidden);
		}
		else // UnLock
		{
			P_Selected->SetVisibility(ESlateVisibility::Hidden);
			P_Locked->SetVisibility(ESlateVisibility::Hidden);
			P_UnLocked->SetVisibility(ESlateVisibility::Visible);
		}
	}

}

// UpgradeRelicBoard
void UB2UIUpgradeRelicBoard::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2UIUpgradeRelicBoardBar, ProgressBar_0);
	GET_SLOT(UB2UIUpgradeRelicBoardBar, ProgressBar_1);
	GET_SLOT(UB2UIUpgradeRelicBoardBar, ProgressBar_2);
	GET_SLOT(UB2UIUpgradeRelicBoardBar, ProgressBar_3);
	GET_SLOT(UB2UIUpgradeRelicBoardBar, ProgressBar_4);
	GET_SLOT(UB2UIUpgradeRelicBoardBar, ProgressBar_5);
	GET_SLOT(UB2UIUpgradeRelicBoardBar, ProgressBar_6);
	GET_SLOT(UB2UIUpgradeRelicBoardBar, ProgressBar_7);
	GET_SLOT(UB2UIUpgradeRelicBoardBar, ProgressBar_8);
	GET_SLOT(UB2UIUpgradeRelicBoardSlot, item_0);
	GET_SLOT(UB2UIUpgradeRelicBoardSlot, item_1);
	GET_SLOT(UB2UIUpgradeRelicBoardSlot, item_2);
	GET_SLOT(UB2UIUpgradeRelicBoardSlot, item_3);
	GET_SLOT(UB2UIUpgradeRelicBoardSlot, item_4);
	GET_SLOT(UB2UIUpgradeRelicBoardSlot, item_5);
	GET_SLOT(UB2UIUpgradeRelicBoardSlot, item_6);
	GET_SLOT(UB2UIUpgradeRelicBoardSlot, item_7);
	GET_SLOT(UB2UIUpgradeRelicBoardSlot, item_8);
	GET_SLOT(UB2UIUpgradeRelicBoardSlot, item_9);
	
	if(item_0.IsValid())
		item_0->Init();
	if (item_1.IsValid())
		item_1->Init();
	if (item_2.IsValid())
		item_2->Init();
	if (item_3.IsValid())
		item_3->Init();
	if (item_4.IsValid())
		item_4->Init();
	if (item_5.IsValid())
		item_5->Init();
	if (item_6.IsValid())
		item_6->Init();
	if (item_7.IsValid())
		item_7->Init();
	if (item_8.IsValid())
		item_8->Init();
	if (item_9.IsValid())
		item_9->Init();
	
	if (ProgressBar_0.IsValid())
		ProgressBar_0->Init();
	if (ProgressBar_1.IsValid())
		ProgressBar_1->Init();
	if (ProgressBar_2.IsValid())
		ProgressBar_2->Init();
	if (ProgressBar_3.IsValid())
		ProgressBar_3->Init();
	if (ProgressBar_4.IsValid())
		ProgressBar_4->Init();
	if (ProgressBar_5.IsValid())
		ProgressBar_5->Init();
	if (ProgressBar_6.IsValid())
		ProgressBar_6->Init();
	if (ProgressBar_7.IsValid())
		ProgressBar_7->Init();
	if (ProgressBar_8.IsValid())
		ProgressBar_8->Init();
}

void UB2UIUpgradeRelicBoard::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIUpgradeRelicBoard::Init()
{
	Super::Init();

	if (item_0.IsValid())
		ItemArray.Add(item_0);
	if (item_1.IsValid())
		ItemArray.Add(item_1);
	if (item_2.IsValid())
		ItemArray.Add(item_2);
	if (item_3.IsValid())
		ItemArray.Add(item_3);
	if (item_4.IsValid())
		ItemArray.Add(item_4);
	if (item_5.IsValid())
		ItemArray.Add(item_5);
	if (item_6.IsValid())
		ItemArray.Add(item_6);
	if (item_7.IsValid())
		ItemArray.Add(item_7);
	if (item_8.IsValid())
		ItemArray.Add(item_8);
	if (item_9.IsValid())
		ItemArray.Add(item_9);
	
	if (ProgressBar_0.IsValid())
		BarArray.Add(ProgressBar_0);
	if (ProgressBar_1.IsValid())
		BarArray.Add(ProgressBar_1);
	if (ProgressBar_2.IsValid())
		BarArray.Add(ProgressBar_2);
	if (ProgressBar_3.IsValid())
		BarArray.Add(ProgressBar_3);
	if (ProgressBar_4.IsValid())
		BarArray.Add(ProgressBar_4);
	if (ProgressBar_5.IsValid())
		BarArray.Add(ProgressBar_5);
	if (ProgressBar_6.IsValid())
		BarArray.Add(ProgressBar_6);
	if (ProgressBar_7.IsValid())
		BarArray.Add(ProgressBar_7);
	if (ProgressBar_8.IsValid())
		BarArray.Add(ProgressBar_8);
	
}

void UB2UIUpgradeRelicBoard::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2UIUpgradeRelicBoard::SetBoardInfo(int32 nLevel,int32 nRelicID, int32 nRelicGrade)
{	
	TempRelicID = nRelicID;
	TempRelicGrade = nRelicGrade;
	TempRelicLevel = nLevel;
	
	int32 nLv = nLevel;
	if (nLevel == 10) nLv++;

	for (int32 i = 0; i < ItemArray.Num(); i++)
	{
		ItemArray[i]->SetSlotText(i + 1);

		if (i == nLv - 1) // target
			ItemArray[i]->SetSlotVisibleState(0);
		else if (i > nLv - 1) // lock
			ItemArray[i]->SetSlotVisibleState(1);
		else // unlock
			ItemArray[i]->SetSlotVisibleState(2);

	}

	for (int32 i = 0; i < BarArray.Num(); i++)
	{
		if (i == nLv - 2)
			BarArray[i]->SetProgressValue(1);
		else if (i > nLv - 2)
			BarArray[i]->SetProgressValue(0);
		else
			BarArray[i]->SetProgressValue(1);
	}
	
}

void UB2UIUpgradeRelicBoard::OnPlayAnimation_Levelup(int32 nPlayIndex)
{
	AnimationPlayIndex = 0;
	AnimationIsLevelUp = true;

	if (!AnimationIsLevelUp) // 铰鞭
	{
		for (int32 i = 0; i < ItemArray.Num(); i++)
		{
			ItemArray[i]->OnPlayAnimation2_BP(this);
		}
	}
	else // 饭骇诀
	{
		AnimationPlayIndex = nPlayIndex;

		if (AnimationPlayIndex == 1)// 1肪诀 局聪绰 坷流 浇吩父..
		{
			if (ItemArray.IsValidIndex(AnimationPlayIndex))
				ItemArray[AnimationPlayIndex - 1]->OnPlayAnimation_BP(this);
		}
		else // 1 捞惑 何磐绰 官+浇吩
		{
			if (BarArray.IsValidIndex(AnimationPlayIndex - 2))
				BarArray[AnimationPlayIndex - 2]->OnPlayAnimation_BP(this, false);
		}
	}
}

void UB2UIUpgradeRelicBoard::OnPlayAnimation_Upgrade(bool bIsFail)
{
	AnimationIsLevelUp = false;
	bIsUpgradeFail = bIsFail;

	for (int32 i = 0; i < ItemArray.Num(); i++)
	{
		ItemArray[i]->OnPlayAnimation2_BP(this);
	}
}

void UB2UIUpgradeRelicBoard::OnPlayFailAnimation_Levelup(int32 nPlayIndex)
{

	AnimationPlayIndex = nPlayIndex;

	if(AnimationPlayIndex != 1) // 1 捞惑 何磐绰 官+浇吩
	{
		if (BarArray.IsValidIndex(AnimationPlayIndex - 2))
			BarArray[AnimationPlayIndex - 2]->OnPlayAnimation_BP(this, true);
	}
	
}

void UB2UIUpgradeRelicBoard::OnEndSlotAnimation_BP(int32 nSlotIndex)
{
	if (!AnimationIsLevelUp)// 铰鞭老锭
	{
		if (nSlotIndex == 0) // 茄锅父 龋免
		{
			if (m_parentClass)
			{
				if (bIsUpgradeFail)
					m_parentClass->OnPlayUpgradeFail_IMP();
				else
					m_parentClass->OnPlaySucessAnimation(false);
			}
		}
	}
	else
	{
		if(m_parentClass)
			m_parentClass->OnPlaySucessAnimation(true);

		SetBoardInfo(AnimationPlayIndex, TempRelicID, TempRelicGrade);
	}
}

void UB2UIUpgradeRelicBoard::OnEndBarAnimation_BP(bool bIsFail)
{
	if (bIsFail)
	{
		if (m_parentClass)
			m_parentClass->ShowFailPopup(true);
	}
	else
	{
		if (ItemArray.IsValidIndex(AnimationPlayIndex - 1))
			ItemArray[AnimationPlayIndex - 1]->OnPlayAnimation_BP(this);
	}
}

void UB2UIUpgradeRelicBoard::SetParentClass(UB2LobbyUI_RelicManagement* parentClass)
{
	if (parentClass)
	{
		m_parentClass = parentClass;
	}
}