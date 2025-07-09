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

		// B2LobbyUI_InventoryMain 에서 하는 거랑 마찬가지
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
			// 필요에 따라 AddedRowSlot 를 조절..

			NewItemRow->SetInventorySlotMaxCount(DynItemIconRowMaxCount::AutoLevelupResult);
			// NewItemRow 는 여전히 CanvasPanel 수준이고 여기에 실제 아이템 아이콘들을 쳐넣는다.
			NewItemRow->UpdateItemIcons(ResultItems, RI * InventoryRowMaxCount, NULL); // 3번째 인자는 복합 스크롤 처리를 해야 하는 메인 보관함에서만 필요.
																						 // AutoEnhanceResult IconUsage 에서는 아이콘 자체는 입력을 패스하고 스크롤만 되도록 세팅될 것.
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
