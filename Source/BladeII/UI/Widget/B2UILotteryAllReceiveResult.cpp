// Fill out your copyright notice in the Description page of Project Settings.

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

		// B2LobbyUI_InventoryMain 俊辑 窍绰 芭尔 付蛮啊瘤
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
			// 鞘夸俊 蝶扼 AddedRowSlot 甫 炼例..

			NewItemRow->SetInventorySlotMaxCount(DynItemIconRowMaxCount::AutoLevelupResult);
			// NewItemRow 绰 咯傈洒 CanvasPanel 荐霖捞绊 咯扁俊 角力 酒捞袍 酒捞能甸阑 媚持绰促.
			NewItemRow->UpdateItemIcons(ResultItems, RI * InventoryRowMaxCount, NULL); // 3锅掳 牢磊绰 汗钦 胶农费 贸府甫 秦具 窍绰 皋牢 焊包窃俊辑父 鞘夸.
																						 // AutoEnhanceResult IconUsage 俊辑绰 酒捞能 磊眉绰 涝仿阑 菩胶窍绊 胶农费父 登档废 技泼瞪 巴.
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
