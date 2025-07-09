// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UILotteryAllReceiveResult.h"
#include "B2DynItemIconRow_LobbyInven.h"
#include "BladeIIUtil.h"
#include "B2UIManager.h"



void UB2UILotteryAllReceiveResult::DestroySelf(class UB2UIManager* InUIManager)
{
	for (TWeakObjectPtr<class UB2DynItemIconRow_LobbyInven> Elem : CreatedItemRows)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf();
		}
	}

	Super::DestroySelf(InUIManager);
}

void UB2UILotteryAllReceiveResult::UpdateResultItemIcons(TArray<FB2Item>& InResultItems)
{
	if (ItemIconRowWidgetClass == NULL || !SB_ItemDisplayPanel.IsValid()) 
	{
		return;
	}

	ResultItems = InResultItems;

	if (ResultItems.Num() > 0)
	{
		ResultItems.Sort([](const FB2Item& Data1, const FB2Item& Data2)
		{
			if (Data1.StarGrade > Data2.StarGrade)
			{
				return true;
			}
			return false;
		});

		// B2LobbyUI_InventoryMain ���� �ϴ� �Ŷ� ��������
		const int32 InventoryRowMaxCount = DynItemIconRowMaxCount::AutoLevelupResult;
		const int32 RequiredRowNum = (FMath::Max(0, ResultItems.Num() - 1) / InventoryRowMaxCount) + 1;
		for (int RI = 0; RI < RequiredRowNum; ++RI)
		{
			UB2DynItemIconRow_LobbyInven* NewItemRow = CreateWidget<UB2DynItemIconRow_LobbyInven>(GetOwningPlayer(), ItemIconRowWidgetClass);
			if (NewItemRow == NULL)
			{
				continue;
			}

			UScrollBoxSlot* AddedRowSlot = Cast<UScrollBoxSlot>(SB_ItemDisplayPanel->AddChild(NewItemRow));
			// �ʿ信 ���� AddedRowSlot �� ����..

			NewItemRow->SetInventorySlotMaxCount(DynItemIconRowMaxCount::AutoLevelupResult);
			// NewItemRow �� ������ CanvasPanel �����̰� ���⿡ ���� ������ �����ܵ��� �ĳִ´�.
			NewItemRow->UpdateItemIcons(ResultItems, RI * InventoryRowMaxCount, NULL); // 3��° ���ڴ� ���� ��ũ�� ó���� �ؾ� �ϴ� ���� �����Կ����� �ʿ�.
																						 // AutoEnhanceResult IconUsage ������ ������ ��ü�� �Է��� �н��ϰ� ��ũ�Ѹ� �ǵ��� ���õ� ��.
			NewItemRow->OverrideItemIconUsage(ELobbyInvenItemIconUsage::EEIIT_MailAllReceiveResult);
			CreatedItemRows.Add(NewItemRow);
		}
	}
	else
	{
		CloseWidgetDelegate();
	}
}

void UB2UILotteryAllReceiveResult::CacheAssets()
{
	GET_SLOT(UScrollBox, SB_ItemDisplayPanel);
	GET_SLOT(UTextBlock, TB_AutoEnhanceResultTitle);
	GET_SLOT(UTextBlock, TB_ConfirmBtn);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_Confirm);
}

void UB2UILotteryAllReceiveResult::UpdateStaticText()
{
	if (TB_AutoEnhanceResultTitle.IsValid())
	{
		TB_AutoEnhanceResultTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MailLottery_QuickResult"))));
	}

	if (TB_ConfirmBtn.IsValid())
	{
		TB_ConfirmBtn->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Confirm"))));
	}
}

void UB2UILotteryAllReceiveResult::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UILotteryAllReceiveResult::OnCloseButtonClicked);
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UILotteryAllReceiveResult::OnConfirmBtnClicked);
}

void UB2UILotteryAllReceiveResult::CloseWidgetDelegate()
{
	OnCloseButtonClicked();
}

void UB2UILotteryAllReceiveResult::OnCloseButtonClicked()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::LotteryAllReceiveResult);
}

void UB2UILotteryAllReceiveResult::OnConfirmBtnClicked()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::LotteryAllReceiveResult);
}
