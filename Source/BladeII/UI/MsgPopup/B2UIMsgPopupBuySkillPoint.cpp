#include "B2UIMsgPopupBuySkillPoint.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2UISlotItem.h"
#include "B2UIBuySkillPointBar.h"
#include "Event.h"
#include "B2UIShortagePopupManager.h"


#include "BladeIIGameImpl.h"

UB2UIMsgPopupBuySkillPoint::UB2UIMsgPopupBuySkillPoint(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UB2UIMsgPopupBuySkillPoint::Init()
{
	Super::Init();

	CAPTURE_UOBJECT(UB2UIMsgPopupBuySkillPoint);

	ChangeBuySkillPointTicket = ChangeBuySkillPointClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(ChangeBuySkillPoint)
			Capture->UpdateGemCostTotal(); 
			Capture->UpdateSelectPoint();
		END_CAPTURE_OBJECT()
	);
}

void UB2UIMsgPopupBuySkillPoint::DestroySelf(class UB2UIManager* InUIManager)
{
	ChangeBuySkillPointClass<>::GetInstance().Unsubscribe(ChangeBuySkillPointTicket);

	Super::DestroySelf(InUIManager);
}

void UB2UIMsgPopupBuySkillPoint::SetButtonText(const FText& InText)
{
	if (TB_BtnConfirm.IsValid())
		TB_BtnConfirm->SetText(InText);
}

void UB2UIMsgPopupBuySkillPoint::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UButton, BTN_Confirm);
	GET_SLOT(UButton, BTN_Close);
	GET_SLOT(UTextBlock, TB_BtnConfirm);
	GET_SLOT(UTextBlock, TB_GemPerSP);
	GET_SLOT(UB2UISlotItem, UIP_ItemCost);
	GET_SLOT(UB2UIBuySkillPointBar, UIP_BuySkillPointBar);

	GET_SLOT(UImage, IMG_SelectedChar);
	GET_SLOT(UTextBlock, TB_TotalBoughtPointText);
	GET_SLOT(UTextBlock, TB_SelectPoint);
	GET_SLOT(UTextBlock, TB_SelectPointText);
	GET_SLOT(UTextBlock, TB_TotalBoughtPoint);
	GET_SLOT(UTextBlock, TB_SoldOut);

	GET_SLOT(UHorizontalBox, HB_SelectPointBox);

	GET_SLOT(UTextBlock, TB_Title);
}

void UB2UIMsgPopupBuySkillPoint::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_BuySkillPoint_Title")));

	if (TB_TotalBoughtPointText.IsValid())
		TB_TotalBoughtPointText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skillinfo_Buyskillpoint_State")));

	if (TB_SelectPointText.IsValid())
		TB_SelectPointText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skillinfo_Buyskillpoint_Select")));
}

void UB2UIMsgPopupBuySkillPoint::BindDelegates()
{
	//BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIMsgPopupBuySkillPoint::OnClickBtnConfirm);
	//BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIMsgPopupBuySkillPoint::OnClickBtnClose);
}

void UB2UIMsgPopupBuySkillPoint::InitializeBuySkillPoint(EPCClass PCClass)
{
	SelectedCharacter = PCClass;

	int32 CharIndex = PCClassToInt(SelectedCharacter);
	if (IMG_SelectedChar.IsValid() && CharacterPortraits.IsValidIndex(CharIndex))
	{
		IMG_SelectedChar->SetBrushFromMaterial(CharacterPortraits[CharIndex]);
	}

	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	int32 BuyableMaxSkillPoint = ClientDataStore.GetMaxSkillPointBuyable(SelectedCharacter);

	if (TB_TotalBoughtPoint.IsValid())
	{
		TB_TotalBoughtPoint->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		int32 MaxSkillPoint = ClientDataStore.GetMaxSkillPoint();
		TB_TotalBoughtPoint->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillLevel")), FText::AsNumber(MaxSkillPoint - BuyableMaxSkillPoint), FText::AsNumber(MaxSkillPoint)));
	}

	if (BuyableMaxSkillPoint > 0)
	{
		if (UIP_BuySkillPointBar.IsValid())
		{
			UIP_BuySkillPointBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UIP_BuySkillPointBar->Init();
			UIP_BuySkillPointBar->SetMaxNum(BuyableMaxSkillPoint);
			UIP_BuySkillPointBar->SetNum(1);
			UIP_BuySkillPointBar->SetCharacterName(ClientDataStore.GetLocalCharClassName(SelectedCharacter));
		}			

		if (TB_SoldOut.IsValid())
			TB_SoldOut->SetVisibility(ESlateVisibility::Collapsed);

		if (HB_SelectPointBox.IsValid())
			HB_SelectPointBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	}	
	else
	{
		if (HB_SelectPointBox.IsValid())
			HB_SelectPointBox->SetVisibility(ESlateVisibility::Collapsed);

		if (UIP_BuySkillPointBar.IsValid())
			UIP_BuySkillPointBar->SetVisibility(ESlateVisibility::Collapsed);

		if (TB_SoldOut.IsValid())
		{
			TB_SoldOut->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TB_SoldOut->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_BuySkillPoint_SoldOut")), ClientDataStore.GetLocalCharClassName(SelectedCharacter)));
		}			

		if (BTN_Confirm.IsValid())
			BTN_Confirm->SetIsEnabled(false);
	}
}

void UB2UIMsgPopupBuySkillPoint::OnClickBtnConfirm()
{
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
		
	TArray<int32> NumSkillPointToBuy;
	NumSkillPointToBuy.AddZeroed(PCClassToInt(EPCClass::EPC_End));
	NumSkillPointToBuy[PCClassToInt(SelectedCharacter)] = UIP_BuySkillPointBar.IsValid() ? UIP_BuySkillPointBar->GetNum() : 0;
	
	uint32 TotalCost = 0;
	if (UIP_BuySkillPointBar.IsValid())
		TotalCost = UIP_BuySkillPointBar->GetNum() * BladeIIGameImpl::GetClientDataStore().GetGemsBuySkillPoint();
	
	const uint32 userHaveAmount(ClientDataStore.GetGemAmount());
	if(TotalCost >	userHaveAmount)
	{
		ShortageMGR->PopupGoToShopForGemMessage();
	}
	else
	{
		ClientDataStore.RequestBuySkillPoint(NumSkillPointToBuy);

		DestroySelf(UB2UIManager::GetInstance());
	}
}

void UB2UIMsgPopupBuySkillPoint::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2UIMsgPopupBuySkillPoint::OnClickBtnClose()
{
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIMsgPopupBuySkillPoint::SetGemCostUnit(int32 InCost)
{
	if (TB_GemPerSP.IsValid())
		TB_GemPerSP->SetText(FText::AsNumber(InCost));
}

void UB2UIMsgPopupBuySkillPoint::SetGemCostTotal(int32 InCostTotal)
{
	if (UIP_ItemCost.IsValid())
	{		
		UIP_ItemCost->SetCount(InCostTotal);

		if (InCostTotal <= BladeIIGameImpl::GetClientDataStore().GetGemAmount())
			UIP_ItemCost->SetItemCountTextColor(ButtonNameColorBase);
		else
			UIP_ItemCost->SetItemCountTextColor(UB2UIManager::GetInstance()->TextColor_Decrease);
	}
}

void UB2UIMsgPopupBuySkillPoint::UpdateGemCostTotal()
{
	int32 TotalCost = 0;
	if (UIP_BuySkillPointBar.IsValid())
		TotalCost += UIP_BuySkillPointBar->GetNum();

	SetGemCostTotal(TotalCost * BladeIIGameImpl::GetClientDataStore().GetGemsBuySkillPoint());
}

void UB2UIMsgPopupBuySkillPoint::UpdateSelectPoint()
{
	if (TB_SelectPoint.IsValid())
		TB_SelectPoint->SetText(FText::AsNumber(UIP_BuySkillPointBar->GetNum()));
}
