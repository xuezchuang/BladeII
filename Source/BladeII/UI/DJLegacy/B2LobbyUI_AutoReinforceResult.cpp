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
		LobbyInven->GetCachedAutoItemLevelupResultItems(ResultItems); // 요건 딱 이 페이지 시작 시점에만 사용 가능할 것임. (UB2LobbyInventory::EndAutoItemEnhance)
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

	// B2LobbyUI_InventoryMain 에서 하는 거랑 마찬가지
	const int32 InventoryRowMaxCount = DynItemIconRowMaxCount::DefaultInven;
	const int32 RequiredRowNum = (FMath::Max(0, InResultItems.Num() - 1) / InventoryRowMaxCount) + 1;
	for (int RI = 0; RI < RequiredRowNum; ++RI)
	{
		UB2DynItemIconRow_LobbyInven* NewItemRow = CreateWidget<UB2DynItemIconRow_LobbyInven>(GetOwningPlayer(), ItemIconRowWidgetClass);
		if (NewItemRow == NULL){
			continue;
		}

		UScrollBoxSlot* AddedRowSlot = Cast<UScrollBoxSlot>(SB_ItemDisplayPanel->AddChild(NewItemRow));
		// 필요에 따라 AddedRowSlot 를 조절..

		// NewItemRow 는 여전히 CanvasPanel 수준이고 여기에 실제 아이템 아이콘들을 쳐넣는다.
		NewItemRow->UpdateItemIcons(InResultItems, RI * InventoryRowMaxCount, NULL); // 3번째 인자는 복합 스크롤 처리를 해야 하는 메인 보관함에서만 필요.
		// AutoEnhanceResult IconUsage 에서는 아이콘 자체는 입력을 패스하고 스크롤만 되도록 세팅될 것.
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
	// 이 창이 열려있을 땐 이미 자동강화 모드 자체는 끝난 상태임. 창만 닫아준다.
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelupResult);
}
