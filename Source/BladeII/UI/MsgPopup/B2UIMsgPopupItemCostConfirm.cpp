#include "B2UIMsgPopupItemCostConfirm.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2UISlotItem.h"
#include "BladeIIGameImpl.h"
#include "CommonStruct.h"

UB2UIMsgPopupItemCostConfirm::UB2UIMsgPopupItemCostConfirm(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ButtonState = false;
}

void UB2UIMsgPopupItemCostConfirm::AddHandler(const FMsgPopupOnClick& Handler)
{
	ClickHandler = Handler;
}

void UB2UIMsgPopupItemCostConfirm::SetButtonText(const FText& InText)
{
	if (TB_BtnConfirm.IsValid())
		TB_BtnConfirm->SetText(InText);
}

void UB2UIMsgPopupItemCostConfirm::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UButton, BTN_Confirm);
	GET_SLOT(UButton, BTN_Close);
	GET_SLOT(UTextBlock, TB_BtnConfirm);
	GET_SLOT(UB2UISlotItem, UIP_ItemCost);
}

void UB2UIMsgPopupItemCostConfirm::UpdateStaticText()
{
	Super::UpdateStaticText();
}

void UB2UIMsgPopupItemCostConfirm::BindDelegates()
{
	//BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIMsgPopupItemCostConfirm::OnClickBtnConfirm);
	//BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIMsgPopupItemCostConfirm::OnClickBtnClose);
}

void UB2UIMsgPopupItemCostConfirm::OnClickBtnConfirm()
{
	if (ClickHandler.IsBound())
		ClickHandler.Execute();

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIMsgPopupItemCostConfirm::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

bool UB2UIMsgPopupItemCostConfirm::IsPayAbleItemCount(const FIntPoint& ItemCost)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();

	if(ItemCost.X == FItemRefIDHelper::GetGoodsID_Gem())
		return CliDataStore.GetGemAmount() >= ItemCost.Y;
	else if (ItemCost.X == FItemRefIDHelper::GetGoodsID_Gold())
		return CliDataStore.GetGoldAmount() >= ItemCost.Y;
	else
		return false;
}

void UB2UIMsgPopupItemCostConfirm::OnClickBtnClose()
{
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIMsgPopupItemCostConfirm::SetItemCost(FIntPoint ItemCost)
{
	if (UIP_ItemCost.IsValid())
	{
		UB2ItemInfo* StageInfoTable = StaticFindItemInfo();
		if (StageInfoTable)
		{
			FSingleItemInfoData* ItemData = StageInfoTable->GetInfoData(ItemCost.X);
			if (ItemData)
			{
				UIP_ItemCost->BindItemInfoData(ItemData, 0);
				UIP_ItemCost->SetCount(ItemCost.Y);
				if (!IsPayAbleItemCount(ItemCost))
					UIP_ItemCost->SetItemCountTextColor(UB2UIManager::GetInstance()->TextColor_Decrease);
			}
		}
	}
}