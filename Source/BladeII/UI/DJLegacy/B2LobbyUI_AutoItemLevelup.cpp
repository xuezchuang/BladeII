// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_AutoItemLevelup.h"
#include "Event.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "B2UIDocHelper.h"

#include "BladeIIGameImpl.h"

void UB2LobbyUI_AutoItemLevelup::CloseWidgetDelegate()
{
	OnCloseButtonClicked();
}

void UB2LobbyUI_AutoItemLevelup::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_AutoLevelupTitle);
	GET_SLOT(UTextBlock, TB_AutoLevelUp);
	GET_SLOT(UTextBlock, TB_GoldToAcquire);
	GET_SLOT(UTextBlock, TB_LevelUpItemCount);
	GET_SLOT(UTextBlock, TB_LevelUpItemNumber);
	GET_SLOT(UTextBlock, TB_IngredItemNumber);
	GET_SLOT(UTextBlock, TB_IngredItemCount);
	GET_SLOT(UTextBlock, TB_LVUP);
	GET_SLOT(UTextBlock, TB_Reset);
	GET_SLOT(UTextBlock, TB_SelectIngred);

	GET_SLOT(UB2Button, Btn_Close);
	GET_SLOT(UB2Button, Btn_AutoLevelupCommit);
	GET_SLOT(UB2Button, Btn_Reset);

	GET_SLOT(UTextBlock, TB_AutoLevelupNote_1);
	GET_SLOT(UTextBlock, TB_AutoLevelupNote_2);

	GET_SLOT(UB2Image, IMG_ClassPortrait);

	GET_SLOT(UB2UIFilterInventory, UIP_InventoryFilter);
}

void UB2LobbyUI_AutoItemLevelup::BindDelegates()
{
	if (UIP_InventoryFilter.IsValid())
		UIP_InventoryFilter->OnCheckStateChanged.AddDynamic(this, &UB2LobbyUI_AutoItemLevelup::OnAnyItemFilterCBChanged);

	BIND_CLICK_FUNC_TO_BTN(Btn_Close, &UB2LobbyUI_AutoItemLevelup::OnCloseButtonClicked)
	BIND_CLICK_FUNC_TO_BTN(Btn_AutoLevelupCommit, &UB2LobbyUI_AutoItemLevelup::OnClickAutoLevelUpCommit)
	BIND_CLICK_FUNC_TO_BTN(Btn_Reset, &UB2LobbyUI_AutoItemLevelup::OnClickReset)
}

void UB2LobbyUI_AutoItemLevelup::OnCloseButtonClicked()
{
	CloseMe();
}

void UB2LobbyUI_AutoItemLevelup::OnClickReset()
{
	if (UIP_InventoryFilter.IsValid())
		UIP_InventoryFilter->ResetFilter();
}

void UB2LobbyUI_AutoItemLevelup::OnClickAutoLevelUpCommit()
{
	TArray<FB2Item> AllTargetItems;
	GetItemListByFilter(AllTargetItems);

	LobbyReqAutoItemLevelupClass<const TArray<FB2Item>&, bool>::GetInstance().Signal(AllTargetItems, false);

	if (UIP_InventoryFilter.IsValid())
	{
		UIP_InventoryFilter->SaveFilterInfo(TEXT("AutoItemLevelup"));
	}

	// 이 시점에서 창을 닫으면 곤란할 거 같다. 자동 강화는 서버와 패킷을 여러차례 교환하므로 최종 응답이 온 후에 이걸 닫고 결과창으로 전환하도록. 

	if (Btn_AutoLevelupCommit.IsValid())
	{
		Btn_AutoLevelupCommit->SetIsEnabled(false); // 혹시 모르니 실행 버튼은 비활성화..
	}
}

void UB2LobbyUI_AutoItemLevelup::OnAnyItemFilterCBChanged(bool bCheck)
{
	UpdateWidgets();
}

void UB2LobbyUI_AutoItemLevelup::UpdateStaticWidgets()
{
	if (IMG_ClassPortrait.IsValid())
		IMG_ClassPortrait->SetBrushFromMaterial(FB2ClassPortraitImage::GetCharImage(ClassPortraitImage, UB2LobbyInventory::GetInventorySelectedPCClass()));

	if (TB_AutoLevelupTitle.IsValid())
		TB_AutoLevelupTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AutoItemLevelup")));

	if (UIP_InventoryFilter.IsValid())
		UIP_InventoryFilter->SetFilterTitle(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AutoItemLevelup"))));

	if (TB_AutoLevelUp.IsValid())
		TB_AutoLevelUp->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AutoItemLevelup")));
	
	if(TB_LVUP.IsValid())
		TB_LVUP->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Levelup")));

	if (TB_GoldToAcquire.IsValid())
		TB_GoldToAcquire->SetText(FText::AsNumber(0));

	if (TB_AutoLevelupNote_1.IsValid())
		TB_AutoLevelupNote_1->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoItemLevelup_BasicInfo")));

	if (TB_AutoLevelupNote_2.IsValid())
		TB_AutoLevelupNote_2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoItemLevelup_LevelupCondition")));

	if(TB_Reset.IsValid())
		TB_Reset->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Reset"))));

	if (TB_SelectIngred.IsValid())
		TB_SelectIngred->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Select"))));

	if (TB_LevelUpItemNumber.IsValid())
		TB_LevelUpItemNumber->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_CountUnit"))));

	if (TB_IngredItemNumber.IsValid())
		TB_IngredItemNumber->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_CountUnit"))));

	if (TB_LevelUpItemCount.IsValid())
		TB_LevelUpItemCount->SetText(FText::AsNumber(0));

	if (TB_IngredItemCount.IsValid())
		TB_IngredItemCount->SetText(FText::AsNumber(0));
}

void UB2LobbyUI_AutoItemLevelup::GetItemListByFilter(TArray<FB2Item>& OutFilteredList)
{
	if (!UIP_InventoryFilter.IsValid())
		return;

	UIP_InventoryFilter->GetAllTargetItems(OutFilteredList);

}

void UB2LobbyUI_AutoItemLevelup::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	if (UIP_InventoryFilter.IsValid())
	{
		UIP_InventoryFilter->Init();
		UIP_InventoryFilter->BindWarningPopup();
		UIP_InventoryFilter->LoadFilterInfo(TEXT("AutoItemLevelup"));
	}

	UpdateStaticWidgets();

	BindDelegates();
	UpdateWidgets();
}

void UB2LobbyUI_AutoItemLevelup::CloseMe()
{ // 이건 단순히 팝업창만을 닫으므로 어떤 식으로든 자동 강화가 진행중인 상태에서는 이런 식으로 닫으면 안 됨. 그런 경우가 필요하면 UB2LobbyInventory::EndAutoItemLevelup 를 사용해야.
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelup);
}

void UB2LobbyUI_AutoItemLevelup::UpdateWidgets()
{
	// 리스트가 바뀌었을 때 현재 리스트에 대한 자동 강화 예상 비용을 요청해야 하는데 여기서 바로 블로킹으로는 안되니 
	// 버튼 등의 UI 를 disable 시킨 채 요청만 날리고 응답은 따로 처리.
	// 내지는 금액 계산을 클라이언트에서 처리하게 될 수도 있음.
	TArray<FB2Item> AllTargetItems;
	GetItemListByFilter(AllTargetItems);

	// 비용 계산해서 관련 UI 들 세팅.
	UB2LobbyInventory* LobbyInvenObj = CachedLobbyGM ? CachedLobbyGM->GetLobbyInventory() : NULL;
	int32 PreviewAutoLevelupCost = 0;
	int32 PreviewAutoLevelupItemCount = 0;
	int32 PreviewIngredItemCount = 0;
	
	if (LobbyInvenObj)
		LobbyInvenObj->GetAutoItemLevelupPreviewInfo(AllTargetItems, PreviewAutoLevelupCost, PreviewAutoLevelupItemCount, PreviewIngredItemCount);

	if (LobbyInvenObj)
		LobbyInvenObj->SetAutoItemLevelupTotalCost(PreviewAutoLevelupCost);

	if (TB_GoldToAcquire.IsValid())
		TB_GoldToAcquire->SetText(FText::AsNumber(PreviewAutoLevelupCost));

	if (Btn_AutoLevelupCommit.IsValid())
		Btn_AutoLevelupCommit->SetIsEnabled(PreviewAutoLevelupCost > 0 && BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= PreviewAutoLevelupCost);

	if (TB_LevelUpItemCount.IsValid())
		TB_LevelUpItemCount->SetText(FText::AsNumber(PreviewAutoLevelupItemCount));

	if (TB_IngredItemCount.IsValid())
		TB_IngredItemCount->SetText(FText::AsNumber(PreviewIngredItemCount));
}