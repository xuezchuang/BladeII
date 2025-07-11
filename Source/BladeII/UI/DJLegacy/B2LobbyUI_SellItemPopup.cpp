// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_SellItemPopup.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"

#include "Event.h"
#include "BladeIIGameImpl.h"

UB2LobbyUI_SellItemPopup::UB2LobbyUI_SellItemPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsForSellAllResult = false;
	SellAllResultGold = 0;
	MaxItemCount = 1;
	SellItemCount = 1;
	ElementSize = 0.1;
}

void UB2LobbyUI_SellItemPopup::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_AskForSelling);
	GET_SLOT(UTextBlock, TB_TotalGold);
	GET_SLOT(UTextBlock, TB_Confirm);
	GET_SLOT(UTextBlock, TB_ItemSellMax);
	GET_SLOT(UTextBlock, TB_ItemSell);
	GET_SLOT(UTextBlock, TB_CancelBtn);
	GET_SLOT(UTextBlock, TB_ItemCount);
	GET_SLOT(UTextBlock, TB_MaxItemCount);

	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UB2Button, BTN_Cancel);
	GET_SLOT(UB2Button, BTN_MaxItemCount);
	GET_SLOT(UB2Button, BTN_PlusItemCount);
	GET_SLOT(UB2Button, BTN_MinusItemCount);
	GET_SLOT(USlider, SD_ItemCount);		

	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2LobbyUI_SellItemPopup::OnClickConfirmBtn)
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2LobbyUI_SellItemPopup::OnClickCancelBtn)
	BIND_CLICK_FUNC_TO_BTN(BTN_MaxItemCount, &UB2LobbyUI_SellItemPopup::OnClickMaxItemCountBtn)
}

void UB2LobbyUI_SellItemPopup::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
	
	UpdateStaticWidgets(); 
}

//// Sell Item ////
void UB2LobbyUI_SellItemPopup::PopupMain(const FB2Item& InItemToSell, const FSellItemPopupOnClick& InConfirmBtnHandler, const FSellItemPopupOnClick& InCancelBtnHandler)
{
	bIsForSellAllResult = false;

	ItemToSell = InItemToSell;
	OnClickConfirmBtnHandler = InConfirmBtnHandler;
	OnClickCancelBtnHandler = InCancelBtnHandler;

	// ���̵�� �ʱ�ȭ
	if (SD_ItemCount.IsValid())
		SD_ItemCount->SetValue(0);

	int32 MaxAmount = ItemToSell.ConsumingAmount;
	bool bUseCheckMoney = (InItemToSell.InventoryType == EItemInvenType::EIIVT_Consumables && InItemToSell.ConsumingAmount > 1);
	
	SetItemCount(bUseCheckMoney ? 0 : 1);

	if (bUseCheckMoney)
	{
		// ��� �ȱ� �������� ������ 21���� ������.. max ������ �ٲ���
		int64 RemainMoney = MaxGoldAmount - static_cast<int64>(BladeIIGameImpl::GetClientDataStore().GetGoldAmount());
		int64 PreviewMoney = static_cast<int64>(ItemToSell.SellingPrice) * static_cast<int64>(ItemToSell.ConsumingAmount);
		bool bIsMaxMoney = RemainMoney < PreviewMoney;

		if (bIsMaxMoney)
		{
			MaxAmount = static_cast<int32>(RemainMoney / ItemToSell.SellingPrice);
		}
	}

	SetMaxItemCount(MaxAmount, ItemToSell.ConsumingAmount);
	SetElementSize(MaxAmount);

	UpdateDynamicWidgets();
	UpdateSellItemWidgets();
}

void UB2LobbyUI_SellItemPopup::PopupAsSellResult(const int32 AcquiredGold)
{
	bIsForSellAllResult = true;
	SellAllResultGold = AcquiredGold;

	UpdateDynamicWidgets();
	UpdateSellItemWidgets();
}

//// Sell Ether ////
void UB2LobbyUI_SellItemPopup::PopupMain(const int32 EtherPrice, const FSellItemPopupOnClick& InConfirmBtnHandler, const FSellItemPopupOnClick& InCancelBtnHandler)
{
	bIsForSellAllResult = false;

	SellAllResultGold = EtherPrice;
	OnClickConfirmBtnHandler = InConfirmBtnHandler;
	OnClickCancelBtnHandler = InCancelBtnHandler;

	// ���̵�� �ʱ�ȭ
	if (SD_ItemCount.IsValid())
		SD_ItemCount->SetValue(0);
	SetItemCount(1);
	SetMaxItemCount(1);
	SetElementSize(1);

	UpdateStaticWidgets();
	UpdateDynamicWidgets();
	UpdateSellEtherWidgets();
}

void UB2LobbyUI_SellItemPopup::PopupAsSellEtherResult(const int32 AcquiredGold, const FSellItemPopupOnClick& InConfirmBtnHandler)
{
	bIsForSellAllResult = true;
	SellAllResultGold = AcquiredGold;

	OnClickConfirmBtnHandler = InConfirmBtnHandler;

	UpdateDynamicWidgets();
	UpdateSellItemWidgets();
}

//// Sell Totem ////
void UB2LobbyUI_SellItemPopup::PopupMain(const FB2Totem& InTotemToSell, const FSellItemPopupOnClick& InConfirmBtnHandler, const FSellItemPopupOnClick& InCancelBtnHandler)
{
	bIsForSellAllResult = false;

	FB2TotemInfo SellTotemInfo;
	BladeIIGameImpl::GetTotemDataStore().GetTotemInfo(InTotemToSell.RefID, SellTotemInfo);

	SellAllResultGold = SellTotemInfo.SellPrice;
	OnClickConfirmBtnHandler = InConfirmBtnHandler;
	OnClickCancelBtnHandler = InCancelBtnHandler;

	// ���̵�� �ʱ�ȭ
	if (SD_ItemCount.IsValid())
		SD_ItemCount->SetValue(0);
	SetItemCount(1);
	SetMaxItemCount(1);
	SetElementSize(1);

	UpdateStaticWidgets();
	UpdateDynamicWidgets();
	UpdateSellEtherWidgets();
}

void UB2LobbyUI_SellItemPopup::PopupAsSellTotemResult(const int32 AcquiredGold)
{
	bIsForSellAllResult = true;
	SellAllResultGold = AcquiredGold;

	UpdateDynamicWidgets();
	UpdateSellItemWidgets();
}

void UB2LobbyUI_SellItemPopup::OnChangeItemCountSliderBar(float fValue)
{
	// �⺻������ �ǸŽ� 0����°��� ���⶧���� 0������ ��� 1���� �������ش�.
	if (fValue <= 0.f)
	{
		if (SD_ItemCount.IsValid())
			SD_ItemCount->SetValue(GetDivideElement());

		return;
	}
}

float UB2LobbyUI_SellItemPopup::ConvertItemCountToSliderValue(int32 InItemCount)
{
	return (InItemCount - 1) * GetDivideElement();
}


int32 UB2LobbyUI_SellItemPopup::ConvertSliderValueToItemCount(float InSliderValue)
{
	int32 nValue = InSliderValue == 1 ? MaxItemCount : (InSliderValue * (1 / GetDivideElement()));

	return FMath::Min(nValue,MaxItemCount);
}

void UB2LobbyUI_SellItemPopup::SetItemCountAndUpdateWidget(int32 InItemCount)
{
	SetItemCount(InItemCount);

	UpdateSellItemWidgets();
}

int32 UB2LobbyUI_SellItemPopup::GetSellItemCount()
{
	return SellItemCount;
}

int32 UB2LobbyUI_SellItemPopup::GetNextSellItemCount()
{
	return FMath::Min(SellItemCount + 1, MaxItemCount);	
}

int32 UB2LobbyUI_SellItemPopup::GetPreSellItemCount()
{
	return FMath::Max(SellItemCount - 1, 0);
}

void UB2LobbyUI_SellItemPopup::SetItemCount(int32 InItemCount)
{
	SellItemCount = InItemCount;

	bool bEnableConfirm = SellItemCount > 0;
	if (BTN_Confirm.IsValid())
		BTN_Confirm->SetIsEnabled(bEnableConfirm);
}

void UB2LobbyUI_SellItemPopup::UpdateStaticWidgets()
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")));
	}
	if (TB_Confirm.IsValid())
	{
		TB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
	}
	if (TB_CancelBtn.IsValid())
	{
		TB_CancelBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Cancel")));
	}

	if (TB_ItemSellMax.IsValid())
		TB_ItemSellMax->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Maximum")));		//�ִ�

	if(TB_ItemSell.IsValid())
		TB_ItemSell->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SellMax")));		//�Ǹ� ���� ����
}

void UB2LobbyUI_SellItemPopup::UpdateDynamicWidgets()
{
	// ������ AskForSelling ���̾����� ��� �ȱ� ����� �߰���.
	if (TB_AskForSelling.IsValid())
		TB_AskForSelling->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, bIsForSellAllResult ? TEXT("LobbyInvenText_SellAllItemResultMessage") : TEXT("LobbyInvenText_AskForSellingItem")));
	
	if (BTN_Cancel.IsValid())
		BTN_Cancel->SetVisibility(bIsForSellAllResult ? ESlateVisibility::Collapsed : ESlateVisibility::Visible); // �׳� ��� ������ cancel ��ư�� �ʿ����.
}

void UB2LobbyUI_SellItemPopup::UpdateSellItemWidgets()
{
	if (TB_ItemCount.IsValid())
		TB_ItemCount->SetText(FText::AsNumber(bIsForSellAllResult ? MaxItemCount : SellItemCount));

	if (TB_TotalGold.IsValid())
		TB_TotalGold->SetText(FText::FromString(FString::FormatAsNumber(bIsForSellAllResult ? SellAllResultGold : (ItemToSell.SellingPrice * SellItemCount))));

	// ������ �Ұ����� ��Ȳ������ ���ð��� ��ư�� ���� ó��
	bool IsSelectableButton = (bIsForSellAllResult == false && MaxItemCount != 1);  // �⺻������ ������ ������ 1�̰ų� �ƽ�ġ�� �����ִ� ���� ����

	if (IsSelectableButton == false)
		HiddenSelectWidgets_BP();
}

void UB2LobbyUI_SellItemPopup::UpdateSellEtherWidgets()
{
	if (TB_ItemCount.IsValid())
		TB_ItemCount->SetText(FText::AsNumber(bIsForSellAllResult ? MaxItemCount : SellItemCount));

	if (TB_TotalGold.IsValid())
		TB_TotalGold->SetText(FText::FromString(FString::FormatAsNumber(SellAllResultGold)));

	// ������ �Ұ����� ��Ȳ������ ���ð��� ��ư�� ���� ó��
	bool IsSelectableButton = (bIsForSellAllResult == false && MaxItemCount != 1);

	if (IsSelectableButton == false)
		HiddenSelectWidgets_BP();
}

void UB2LobbyUI_SellItemPopup::SetMaxItemCount(int32 InMaxCount, int32 TotalItemCount)
{
	MaxItemCount = InMaxCount;

	if (TB_MaxItemCount.IsValid())
		TB_MaxItemCount->SetText(FText::AsNumber(TotalItemCount));
}

void UB2LobbyUI_SellItemPopup::SetMaxItemCount(int32 InMaxCount)
{
	MaxItemCount = InMaxCount;

	if (TB_MaxItemCount.IsValid())
		TB_MaxItemCount->SetText(FText::AsNumber(InMaxCount));
}

void UB2LobbyUI_SellItemPopup::SetElementSize(int32 InMaxCount)
{
	ElementSize = (1.f / (float)FMath::Max(1, InMaxCount - 1));

	if (SD_ItemCount.IsValid())
		SD_ItemCount->SetStepSize(ElementSize);
}

int32 UB2LobbyUI_SellItemPopup::GetMaxItemCount()
{
	return MaxItemCount;
}

float UB2LobbyUI_SellItemPopup::GetDivideElement()
{
	return ElementSize;
}

void UB2LobbyUI_SellItemPopup::OnClickConfirmBtn()
{
	OnClickConfirmBtnHandler.ExecuteIfBound(); // ���� �Ǹŵ����� ���⼭ �ϵ��� �ܺο��� ����. �� �˾��� invoke �� �θ� widget �� ���� ���־� �ؼ� ������ ���⼭ �� ó���ϱⰡ ���ŷο�.
	
	CloseMe();
}

void UB2LobbyUI_SellItemPopup::CloseWidgetDelegate()
{
	OnClickCancelBtn();
}

void UB2LobbyUI_SellItemPopup::OnClickCancelBtn()
{
	OnClickCancelBtnHandler.ExecuteIfBound();

	CloseMe();
}

void UB2LobbyUI_SellItemPopup::OnClickMaxItemCountBtn()
{
	SetItemCount(MaxItemCount);

	if (SD_ItemCount.IsValid())
		SD_ItemCount->SetValue(ConvertItemCountToSliderValue(MaxItemCount));
	
	UpdateDynamicWidgets();
	UpdateSellItemWidgets();
}

void UB2LobbyUI_SellItemPopup::CloseMe()
{
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_SellItemPopup);
}