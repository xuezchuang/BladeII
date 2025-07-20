// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_AutoItemLevelup.h"
//#include "BladeII.h"

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

	// 捞 矫痢俊辑 芒阑 摧栏搁 帮鄂且 芭 鞍促. 磊悼 碍拳绰 辑滚客 菩哦阑 咯矾瞒肥 背券窍骨肺 弥辆 览翠捞 柯 饶俊 捞吧 摧绊 搬苞芒栏肺 傈券窍档废. 

	if (Btn_AutoLevelupCommit.IsValid())
	{
		Btn_AutoLevelupCommit->SetIsEnabled(false); // 趣矫 葛福聪 角青 滚瓢篮 厚劝己拳..
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
{ // 捞扒 窜鉴洒 扑诀芒父阑 摧栏骨肺 绢恫 侥栏肺电 磊悼 碍拳啊 柳青吝牢 惑怕俊辑绰 捞繁 侥栏肺 摧栏搁 救 凳. 弊繁 版快啊 鞘夸窍搁 UB2LobbyInventory::EndAutoItemLevelup 甫 荤侩秦具.
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelup);
}

void UB2LobbyUI_AutoItemLevelup::UpdateWidgets()
{
	// 府胶飘啊 官差菌阑 锭 泅犁 府胶飘俊 措茄 磊悼 碍拳 抗惑 厚侩阑 夸没秦具 窍绰单 咯扁辑 官肺 喉肺欧栏肺绰 救登聪 
	// 滚瓢 殿狼 UI 甫 disable 矫挪 盲 夸没父 朝府绊 览翠篮 蝶肺 贸府.
	// 郴瘤绰 陛咀 拌魂阑 努扼捞攫飘俊辑 贸府窍霸 瞪 荐档 乐澜.
	TArray<FB2Item> AllTargetItems;
	GetItemListByFilter(AllTargetItems);

	// 厚侩 拌魂秦辑 包访 UI 甸 技泼.
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