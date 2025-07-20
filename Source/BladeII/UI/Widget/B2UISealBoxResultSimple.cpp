// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UISealBoxResultSimple.h"
#include "B2UIManager.h"
#include "Retailer.h"
#include "Event.h"
#include "B2DynItemIcon_SealBoxPreview.h"
#include "BladeIIGameImpl.h"
#include "B2LobbyGameMode.h"
#include "../../Lobby/B2LobbyInventory.h"

void UB2UISealBoxResultSimple::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_ResultTitle);

	GET_SLOT(UB2ScrollBox, SB_ResultScroll);

	GET_SLOT(UB2Button, BTN_OneMoreTime);
	GET_SLOT(UTextBlock, TB_OneMoreTime);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UTextBlock, TB_Close);
}

void UB2UISealBoxResultSimple::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_OneMoreTime, &UB2UISealBoxResultSimple::OnClickedBTNOneMoreTime);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UISealBoxResultSimple::OnClickedBTNClose);
}

void UB2UISealBoxResultSimple::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_ResultTitle.IsValid())
		TB_ResultTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_PleaseSelectFactoryList")));
	if (TB_Close.IsValid())
		TB_Close->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
	if (TB_OneMoreTime.IsValid())
		TB_OneMoreTime->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SealBox_NextUnseal")));
}

void UB2UISealBoxResultSimple::Init()
{
	Super::Init();

	currentArrayIndex = 0;

	if (SB_ResultScroll.IsValid())
		SB_ResultScroll->ClearChildren();
}

void UB2UISealBoxResultSimple::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UISealBoxResultSimple::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);

	//SubscribeEvent();
}

void UB2UISealBoxResultSimple::OnClose(bool RightNow)
{
	Super::OnClose(RightNow);

	//UnSubscribeEvent();
}

void UB2UISealBoxResultSimple::OnClickedBTNOneMoreTime()
{
	if (UnsealData->left_amount > 0)
	{
		data_trader::Retailer::GetInstance().RequestUnsealBox(UnsealData->seal_box_id,savedClass + 1, 1);	//茄锅俊 割俺 凯扼绰 郴侩篮 扁裙俊 绝绢辑 茄锅俊 1俺究 凯档废 窍靛内爹 凳
		SealBoxSelectCharClass<int32>::GetInstance().Signal(savedClass);
		
		UB2UIManager::GetInstance()->CloseUI(UIFName::SealBoxResultSimple);
	}
}

void UB2UISealBoxResultSimple::OnClickedBTNClose()
{
	if (AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (UB2LobbyInventory* LobbyInven = LobbyGM->GetLobbyInventory())
		{
			LobbyInven->ConditionalCheckForBetterEquipSuggest();
		}
	}
	UB2UIManager::GetInstance()->CloseUI(UIFName::SealBoxResultSimple);
}

void UB2UISealBoxResultSimple::SetRewardDataManually(const FB2ResponseUnsealBoxPtr& UnsealBoxPtr, int32 pcClass)
{
	UnsealData = UnsealBoxPtr;
	savedClass = pcClass;
	currentArrayIndex = 0;

	//if (UnsealData->left_amount > 0)
	//{
	//	if (BTN_OneMoreTime.IsValid())
	//		BTN_OneMoreTime->SetVisibility(ESlateVisibility::Visible);

	//	if (TB_OneMoreTime.IsValid())
	//		TB_OneMoreTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SealBox_NextUnseal")), FText::AsNumber(UnsealData->left_amount)));
	//}
	//else
	//{
	//	if (BTN_OneMoreTime.IsValid())
	//		BTN_OneMoreTime->SetVisibility(ESlateVisibility::Collapsed);
	//}

	//
	//for (int32 i = 0; i < UnsealData->rewards.Num(); i++)
	//{		
	//	if (UnsealData->rewards[i]->reward_type == b2network::B2RewardType::ITEM)
	//	{
	//		FB2Item tempItem;
	//		tempItem = UnsealData->rewards[i]->item;

	//		UB2DynItemIcon_SealBoxPreview* SealBoxIconWidget = CreateWidget<UB2DynItemIcon_SealBoxPreview>(GetOwningPlayer(), resultWidget);
	//		SealBoxIconWidget->CacheAssets();
	//		SealBoxIconWidget->SetItem(tempItem.ItemRefID, false);
	//		SealBoxIconWidget->SetResultPannel(this);
	//		SealBoxIconWidget->UpdateItemData(tempItem);;
	//		SB_ResultScroll->AddChild(SealBoxIconWidget);
	//	}
	//	else
	//	{
	//		UB2DynItemIcon_SealBoxPreview* SealBoxIconWidget = CreateWidget<UB2DynItemIcon_SealBoxPreview>(GetOwningPlayer(), resultWidget);
	//		SealBoxIconWidget->CacheAssets();
	//		SealBoxIconWidget->SetItem(BladeIIGameImpl::GetClientDataStore().GetRewardItemId(UnsealData->rewards[i]->reward_type), UnsealData->rewards[i]->amount);
	//		SealBoxIconWidget->SetResultPannel(this);
	//		SB_ResultScroll->AddChild(SealBoxIconWidget);
	//	}
	//}
	//
	/*
	if (UnsealData->rewards[currentArrayIndex]->reward_type == b2network::B2RewardType::ITEM)
	{
		FB2Item tempItem;
		tempItem = UnsealData->rewards[currentArrayIndex]->item;

		UB2DynItemIcon_SealBoxPreview* SealBoxIconWidget = CreateWidget<UB2DynItemIcon_SealBoxPreview>(GetOwningPlayer(), resultWidget);
		SealBoxIconWidget->CacheAssets();
		SealBoxIconWidget->SetItem(tempItem.ItemRefID, false);
		SealBoxIconWidget->SetResultPannel(this);
		SealBoxIconWidget->UpdateItemData(tempItem);;
		SB_ResultScroll->AddChild(SealBoxIconWidget);
	}
	else
	{
		UB2DynItemIcon_SealBoxPreview* SealBoxIconWidget = CreateWidget<UB2DynItemIcon_SealBoxPreview>(GetOwningPlayer(), resultWidget);
		SealBoxIconWidget->CacheAssets();
		SealBoxIconWidget->SetItem(BladeIIGameImpl::GetClientDataStore().GetRewardItemId(UnsealData->rewards[currentArrayIndex]->reward_type), UnsealData->rewards[currentArrayIndex]->amount);
		SealBoxIconWidget->SetResultPannel(this);
		SB_ResultScroll->AddChild(SealBoxIconWidget);
	}

	currentArrayIndex++;
	*/
}

void UB2UISealBoxResultSimple::ContinueResultShowing()
{
	//// 力累家 贸烦 局聪皋捞记 场车阑 锭 弊 促澜波 柳青窍档废

	//if (currentArrayIndex >= UnsealData->rewards.Num())
	//{
	//	//everything ends here.
	//	return;
	//}
	//else
	//{
	//	if (UnsealData->rewards[currentArrayIndex]->reward_type == b2network::B2RewardType::ITEM)
	//	{
	//		FB2Item tempItem;
	//		tempItem = UnsealData->rewards[currentArrayIndex]->item;

	//		UB2DynItemIcon_SealBoxPreview* SealBoxIconWidget = CreateWidget<UB2DynItemIcon_SealBoxPreview>(GetOwningPlayer(), resultWidget);
	//		SealBoxIconWidget->CacheAssets();
	//		SealBoxIconWidget->SetItem(tempItem.ItemRefID, false);
	//		SealBoxIconWidget->SetResultPannel(this);
	//		SealBoxIconWidget->UpdateItemData(tempItem);;
	//		SB_ResultScroll->AddChild(SealBoxIconWidget);
	//	}
	//	else
	//	{
	//		UB2DynItemIcon_SealBoxPreview* SealBoxIconWidget = CreateWidget<UB2DynItemIcon_SealBoxPreview>(GetOwningPlayer(), resultWidget);
	//		SealBoxIconWidget->CacheAssets();
	//		SealBoxIconWidget->SetItem(BladeIIGameImpl::GetClientDataStore().GetRewardItemId(UnsealData->rewards[currentArrayIndex]->reward_type), UnsealData->rewards[currentArrayIndex]->amount);
	//		SealBoxIconWidget->SetResultPannel(this);
	//		SB_ResultScroll->AddChild(SealBoxIconWidget);
	//	}

	//	currentArrayIndex++;
	//}
}