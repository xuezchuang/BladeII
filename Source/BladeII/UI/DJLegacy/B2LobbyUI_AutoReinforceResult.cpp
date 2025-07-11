// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_AutoReinforceResult.h"
//#include "BladeII.h"

#include "B2DynItemIconRow_LobbyInven.h"
#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"

UB2LobbyUI_AutoReinforceResult::UB2LobbyUI_AutoReinforceResult(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UB2LobbyUI_AutoReinforceResult::CacheAssets()
{
	GET_SLOT(UScrollBox, SB_ItemDisplayPanel);
	GET_SLOT(UTextBlock, TB_AutoEnhanceResultTitle);
	GET_SLOT(UTextBlock, TB_AutoEnhanceCompleteMsg);
	GET_SLOT(UTextBlock, TB_ConfirmBtn);
}

void UB2LobbyUI_AutoReinforceResult::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	UB2LobbyInventory* LobbyInven = InGM ? InGM->GetLobbyInventory() : NULL;
	if (LobbyInven)
	{
		TArray<FB2Item> ResultItems;
		LobbyInven->GetCachedAutoItemLevelupResultItems(ResultItems); // ��� �� �� ������ ���� �������� ��� ������ ����. (UB2LobbyInventory::EndAutoItemEnhance)
		UpdateResultItemIcons(ResultItems);
	}

	UpdateStaticTexts();
}

void UB2LobbyUI_AutoReinforceResult::DestroySelf()
{
	Super::DestroySelf();
	DestroyResultItemIcons();
}

void UB2LobbyUI_AutoReinforceResult::UpdateStaticTexts()
{
	if (TB_AutoEnhanceResultTitle.IsValid())
	{
		TB_AutoEnhanceResultTitle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("AutoEnhanceResultTitle"))));
	}
	if (TB_AutoEnhanceCompleteMsg.IsValid())
	{
		TB_AutoEnhanceCompleteMsg->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("AutoEnhanceCompleted"))));
	}
	if (TB_ConfirmBtn.IsValid())
	{
		TB_ConfirmBtn->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Confirm"))));
	}
}

void UB2LobbyUI_AutoReinforceResult::UpdateResultItemIcons(const TArray<FB2Item>& InResultItems)
{
	DestroyResultItemIcons();

	if (ItemIconRowWidgetClass == NULL || !SB_ItemDisplayPanel.IsValid()){
		return;
	}

	// B2LobbyUI_InventoryMain ���� �ϴ� �Ŷ� ��������
	const int32 InventoryRowMaxCount = DynItemIconRowMaxCount::DefaultInven;
	const int32 RequiredRowNum = (FMath::Max(0, InResultItems.Num() - 1) / InventoryRowMaxCount) + 1;
	for (int RI = 0; RI < RequiredRowNum; ++RI)
	{
		UB2DynItemIconRow_LobbyInven* NewItemRow = CreateWidget<UB2DynItemIconRow_LobbyInven>(GetOwningPlayer(), ItemIconRowWidgetClass);
		if (NewItemRow == NULL){
			continue;
		}

		UScrollBoxSlot* AddedRowSlot = Cast<UScrollBoxSlot>(SB_ItemDisplayPanel->AddChild(NewItemRow));
		// �ʿ信 ���� AddedRowSlot �� ����..

		// NewItemRow �� ������ CanvasPanel �����̰� ���⿡ ���� ������ �����ܵ��� �ĳִ´�.
		NewItemRow->UpdateItemIcons(InResultItems, RI * InventoryRowMaxCount, NULL); // 3��° ���ڴ� ���� ��ũ�� ó���� �ؾ� �ϴ� ���� �����Կ����� �ʿ�.
		// AutoEnhanceResult IconUsage ������ ������ ��ü�� �Է��� �н��ϰ� ��ũ�Ѹ� �ǵ��� ���õ� ��.
		NewItemRow->OverrideItemIconUsage(ELobbyInvenItemIconUsage::EEIIT_AutoEnhanceResult);

		CreatedItemRows.Add(NewItemRow);
	}
}

void UB2LobbyUI_AutoReinforceResult::DestroyResultItemIcons()
{
	for (UB2DynItemIconRow_LobbyInven* ThisRow : CreatedItemRows)
	{
		if (ThisRow){
			ThisRow->DestroySelf();
		}
	}
	CreatedItemRows.Empty();
}

void UB2LobbyUI_AutoReinforceResult::OnCloseButtonClicked()
{
	CloseMe();
}

void UB2LobbyUI_AutoReinforceResult::OnConfirmBtnClicked()
{
	CloseMe();
}

void UB2LobbyUI_AutoReinforceResult::CloseMe()
{ 
	// �� â�� �������� �� �̹� �ڵ���ȭ ��� ��ü�� ���� ������. â�� �ݾ��ش�.
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelupResult);
}
