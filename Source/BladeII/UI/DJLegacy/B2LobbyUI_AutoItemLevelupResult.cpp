// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_AutoItemLevelupResult.h"
//#include "BladeII.h"

#include "B2DynItemIconRow_LobbyInven.h"
#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"

UB2LobbyUI_AutoItemLevelupResult::UB2LobbyUI_AutoItemLevelupResult(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UB2LobbyUI_AutoItemLevelupResult::CacheAssets()
{
	GET_SLOT(UScrollBox, SB_ItemDisplayPanel);
	GET_SLOT(UTextBlock, TB_AutoEnhanceCompleteMsg);
	GET_SLOT(UTextBlock, TB_AutoEnhanceResultTitle);
	GET_SLOT(UTextBlock, TB_ConfirmBtn);
}

void UB2LobbyUI_AutoItemLevelupResult::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	UB2LobbyInventory* LobbyInven = InGM ? InGM->GetLobbyInventory() : NULL;
	if (LobbyInven)
	{
		TArray<FB2Item> ResultItems;
		LobbyInven->GetCachedAutoItemLevelupResultItems(ResultItems); // 夸扒 迭 捞 其捞瘤 矫累 矫痢俊父 荤侩 啊瓷且 巴烙. (UB2LobbyInventory::EndAutoItemEnhance)
		UpdateResultItemIcons(ResultItems);
	}

	UpdateStaticTexts();
}

void UB2LobbyUI_AutoItemLevelupResult::DestroySelf()
{
	Super::DestroySelf();
	DestroyResultItemIcons();
}

void UB2LobbyUI_AutoItemLevelupResult::UpdateStaticTexts()
{
	if (TB_AutoEnhanceResultTitle.IsValid())
	{
		TB_AutoEnhanceResultTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("AutoEnhanceResultTitle"))));
	}
	if (TB_ConfirmBtn.IsValid())
	{
		TB_ConfirmBtn->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Confirm"))));
	}
	if (TB_AutoEnhanceCompleteMsg.IsValid())
	{
		TB_AutoEnhanceCompleteMsg->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AutoItemLevelup_Complete"))));
	}
}

void UB2LobbyUI_AutoItemLevelupResult::UpdateResultItemIcons(const TArray<FB2Item>& InResultItems)
{
	DestroyResultItemIcons();

	if (ItemIconRowWidgetClass == NULL || !SB_ItemDisplayPanel.IsValid()){
		return;
	}

	// B2LobbyUI_InventoryMain 俊辑 窍绰 芭尔 付蛮啊瘤
	const int32 InventoryRowMaxCount = DynItemIconRowMaxCount::AutoLevelupResult;
	const int32 RequiredRowNum = (FMath::Max(0, InResultItems.Num() - 1) / InventoryRowMaxCount) + 1;
	for (int RI = 0; RI < RequiredRowNum; ++RI)
	{
		UB2DynItemIconRow_LobbyInven* NewItemRow = CreateWidget<UB2DynItemIconRow_LobbyInven>(GetOwningPlayer(), ItemIconRowWidgetClass);
		if (NewItemRow == NULL){
			continue;
		}

		UScrollBoxSlot* AddedRowSlot = Cast<UScrollBoxSlot>(SB_ItemDisplayPanel->AddChild(NewItemRow));
		// 鞘夸俊 蝶扼 AddedRowSlot 甫 炼例..

		NewItemRow->SetInventorySlotMaxCount(DynItemIconRowMaxCount::AutoLevelupResult);
		// NewItemRow 绰 咯傈洒 CanvasPanel 荐霖捞绊 咯扁俊 角力 酒捞袍 酒捞能甸阑 媚持绰促.
		NewItemRow->UpdateItemIcons(InResultItems, RI * InventoryRowMaxCount, NULL); // 3锅掳 牢磊绰 汗钦 胶农费 贸府甫 秦具 窍绰 皋牢 焊包窃俊辑父 鞘夸.
		// AutoEnhanceResult IconUsage 俊辑绰 酒捞能 磊眉绰 涝仿阑 菩胶窍绊 胶农费父 登档废 技泼瞪 巴.
		NewItemRow->OverrideItemIconUsage(ELobbyInvenItemIconUsage::EEIIT_AutoEnhanceResult);
		NewItemRow->PlayEnhanceResultAnims();
		CreatedItemRows.Add(NewItemRow);
	}
}

void UB2LobbyUI_AutoItemLevelupResult::DestroyResultItemIcons()
{
	for (UB2DynItemIconRow_LobbyInven* ThisRow : CreatedItemRows)
	{
		if (ThisRow){
			ThisRow->DestroySelf();
		}
	}
	CreatedItemRows.Empty();
}

void UB2LobbyUI_AutoItemLevelupResult::OnCloseButtonClicked()
{
	CloseMe();
}

void UB2LobbyUI_AutoItemLevelupResult::OnConfirmBtnClicked()
{
	CloseMe();
}

void UB2LobbyUI_AutoItemLevelupResult::CloseMe()
{ 
	// 捞 芒捞 凯妨乐阑 订 捞固 磊悼碍拳 葛靛 磊眉绰 场抄 惑怕烙. 芒父 摧酒霖促.
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelupResult);
}
