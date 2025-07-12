// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UICostumeEnhancePopup.h"
#include "B2UIManager.h"
#include "B2UICostumeItemSlot.h"
#include "B2UICostumeItemOption.h"
#include "B2UICostumeDecompositionPopup.h"
#include "B2UIShortagePopupManager.h"
#include "B2AndroidBackManager.h"
#include "B2LobbyInventory.h"


#include "Retailer.h"
#include "BladeIIGameImpl.h"

void UB2UICostumeEnhancePopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, CP_PopupMain);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_EnhanceCommit);
	
	GET_SLOT(UTextBlock, STB_EnhanceTitle);
	GET_SLOT(UTextBlock, STB_Enhance);
	GET_SLOT(UTextBlock, STB_Power);
	GET_SLOT(UTextBlock, STB_EnhanceIngredCount);
	GET_SLOT(UTextBlock, STB_EnhanceCommitBtn);

	GET_SLOT(UTextBlock, TB_CurEnhance);
	GET_SLOT(UTextBlock, TB_CurPower);
	GET_SLOT(UTextBlock, TB_AfterEnhance);
	GET_SLOT(UTextBlock, TB_AfterPower);
	GET_SLOT(UTextBlock, TB_AfterPowerPlus);
	GET_SLOT(UTextBlock, TB_EnhanceCostNum);

	GET_SLOT(UPanelWidget, List_NeedEnhance);

	GET_SLOT(UImage, IMG_EnhanceCostGoldIcon);

	GET_SLOT(UB2UICostumeItemSlot, UIP_CostumeItemSlot);
	if(UIP_CostumeItemSlot.IsValid()) 
		UIP_CostumeItemSlot->Init();
}

void UB2UICostumeEnhancePopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UICostumeEnhancePopup::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_EnhanceCommit, &UB2UICostumeEnhancePopup::OnClickBTN_EnhanceCommit);
}

void UB2UICostumeEnhancePopup::Init()
{
	SubscribeEvent();
	Super::Init();
}

void UB2UICostumeEnhancePopup::DestroySelf(UB2UIManager* InUIManager)
{
	UnsubscribeEvent();

	if(UIP_CostumeItemSlot.IsValid()) 
		UIP_CostumeItemSlot->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}

void UB2UICostumeEnhancePopup::CloseWidgetDelegate()
{
	if (bRequest)
		return;

	OnClickBTN_Close();
}

void UB2UICostumeEnhancePopup::UpdateStaticText()
{
	if (STB_EnhanceTitle.IsValid())
		STB_EnhanceTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_Enhance_Title")));
	if (STB_Enhance.IsValid())
		STB_Enhance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Enhance")));
	if (STB_EnhanceIngredCount.IsValid())
		STB_EnhanceIngredCount->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_Enhance_Note")));
	if (STB_EnhanceCommitBtn.IsValid())
		STB_EnhanceCommitBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_Enhance_Title")));
}


void UB2UICostumeEnhancePopup::SubscribeEvent()
{
	UnsubscribeEvent();

	//Issues.Add(DeliveryEnhanceCostumesFailClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->OnFailEnhance();
	//}
	//));
}

void UB2UICostumeEnhancePopup::UnsubscribeEvent()
{
	Issues.Empty();
}

void UB2UICostumeEnhancePopup::OnClickBTN_Close()
{
	//if (GetRootWidget())
	//	GetRootWidget()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	//ForcedClickCostumeMaterialClass<bool>::GetInstance().Signal(false);

	//this->DestroySelf(UB2UIManager::GetInstance());
}

void UB2UICostumeEnhancePopup::OnClickBTN_EnhanceCommit()
{
	if (bRequest)
		return;

	if (bNotEnoughMoney)
	{
		ShortageMGR->PopupGoToShopForGoldMessage();
		return;
	}

	bRequest = true;

	OnPlayAnimation_IMP();

	if (GetRootWidget())
		GetRootWidget()->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UB2UICostumeEnhancePopup::UpdatePopupInfo(const FB2Item& InItem)
{
	bRequest = false;
	CurrentItemID = InItem.InstanceUID;
	CurrentItemInvenType = InItem.InventoryType;
	
	FB2Item CurrentItem = InItem;
	FB2Item NextItem = InItem;
	
	bool bIsArmor = CurrentItem.InventoryType == EItemInvenType::EIIVT_CostumeArmor;

	GetCalculateCostumeItem_PreviewItemEnhance(CurrentItem, NextItem, 1);
	NextItem.EnhanceLevel = FMath::Min(10, InItem.EnhanceLevel + 1);

	const FCostumeEnhanceCostInfo* CostInfo = BladeIIGameImpl::GetClientDataStore().GetCostumeEnhanceCost(NextItem.StarGrade, NextItem.EnhanceLevel);
	if (!CostInfo) 
		return;

	if (UIP_CostumeItemSlot.IsValid())
	{
		UIP_CostumeItemSlot->UpdateCostumeItemSlot_FromFB2Item(CurrentItem);
	}

	if (List_NeedEnhance.IsValid())
	{
		List_NeedEnhance->ClearChildren();

		UB2UICostumeDecompositionItem* NewItemRow = CreateWidget<UB2UICostumeDecompositionItem>(GetOwningPlayer(), MaterialSlot);
		if (NewItemRow)
		{
			NewItemRow->Init();
			if (bIsArmor)
			{
				NewItemRow->SetInfo(CostInfo->ArmorMaterialId, UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ENHANCE_COSTUMEARMOR_ITEM), CostInfo->ArmorAmount);
			}
			else
			{
				NewItemRow->SetInfo(CostInfo->WeaponMaterialId, UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ENHANCE_COSTUMEWEAPON_ITEM), CostInfo->WeaponAmount);
			}

			List_NeedEnhance->AddChild(NewItemRow);
		}
	}

	if (STB_Power.IsValid())
		STB_Power->SetText(GetLOCTextOfPrimPointType(CurrentItem.PrimaryPointType));

	if (TB_CurEnhance.IsValid())
		TB_CurEnhance->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillLevel")), FText::AsNumber(CurrentItem.EnhanceLevel), FText::AsNumber(MAX_ENHANCE_COSTUME_LEVEL)));

	if (TB_CurPower.IsValid())
	{
		TB_CurPower->SetText(FText::AsNumber(CurrentItem.GetPrimaryPoint()));
	}

	if (TB_AfterEnhance.IsValid())
		TB_AfterEnhance->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillLevel")), FText::AsNumber(NextItem.EnhanceLevel), FText::AsNumber(MAX_ENHANCE_COSTUME_LEVEL)));

	if (TB_AfterPower.IsValid())
	{
		TB_AfterPower->SetText(FText::AsNumber(NextItem.GetPrimaryPoint()));
	}

	if (TB_AfterPowerPlus.IsValid())
	{
		int32 PowerAmount = NextItem.GetPrimaryPoint() - CurrentItem.GetPrimaryPoint();
		TB_AfterPowerPlus->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_IncreaseStat")), FText::AsNumber(PowerAmount)));
	}

	if (TB_EnhanceCostNum.IsValid())
	{
		bNotEnoughMoney = (BladeIIGameImpl::GetClientDataStore().GetGoldAmount() < CostInfo->MoneyCost);
		TB_EnhanceCostNum->SetColorAndOpacity(bNotEnoughMoney ? UB2UIManager::GetInstance()->TextColor_NonSatisfy : FLinearColor::White);
		TB_EnhanceCostNum->SetText(FText::AsNumber(CostInfo->MoneyCost));
	}

	if (BTN_EnhanceCommit.IsValid())
	{
		bool bAbleEnhance = false;
		if (CurrentItem.InventoryType == EItemInvenType::EIIVT_CostumeArmor)
		{
			bAbleEnhance = (UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ENHANCE_COSTUMEARMOR_ITEM) >= CostInfo->ArmorAmount);
		}
		else
		{
			bAbleEnhance = (UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ENHANCE_COSTUMEWEAPON_ITEM) >= CostInfo->WeaponAmount);
		}

		BTN_EnhanceCommit->SetIsEnabled(bAbleEnhance);
	}
}

void UB2UICostumeEnhancePopup::OnFailEnhance()
{
	OnClickBTN_Close();
}

void UB2UICostumeEnhancePopup::OnEndAnimation_BP()
{
	//EnterItemCostumeEnhanceModeClass<int64>::GetInstance().Signal(CurrentItemID);
	//data_trader::Retailer::GetInstance().RequestEnhanceCostume(CurrentItemID);
	//// response 는 void UB2LobbyInventory::CostumeEnhanceItem(const FB2ResponseEnhanceCostumePtr& msg) 요기임
	//OnClickBTN_Close();
}

void UB2UICostumeEnhancePopup::InitCostumeItemInfoPopup(const FVector2D& v2PopupPosition, const float& fPopupSizeX)
{
	if (CP_PopupMain.IsValid())
	{
		CP_PopupMain->SetRenderTranslation(v2PopupPosition);

		if (UCanvasPanelSlot* pCanvasPanelSlot = Cast<UCanvasPanelSlot>(CP_PopupMain->Slot))
		{
			pCanvasPanelSlot->SetOffsets(FMargin(0, fTopBezel_BP, fPopupSizeX, 0));
		}
	}
}
