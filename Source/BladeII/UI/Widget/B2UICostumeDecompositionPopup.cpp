// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UICostumeDecompositionPopup.h"
#include "B2ItemInfo.h"
#include "B2Inventory.h"
#include "Retailer.h"

#include "BladeIIUtil.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"

void UB2UICostumeDecompositionPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UHorizontalBox, List_MainMaterials);
	GET_SLOT(UHorizontalBox, List_ReturnMaterial);
	GET_SLOT(UPanelWidget, P_MoreItems);

	GET_SLOT(UB2Button, BTN_Modal);
	GET_SLOT(UB2Button, BTN_NO);
	GET_SLOT(UB2Button, BTN_Yes);
	GET_SLOT(UB2Button, BTN_Confirm);

	GET_SLOT(UWidgetSwitcher, WS_Buttons);

	GET_SLOT(UB2RichTextBlock, RTB_ObtainDesc);
	GET_SLOT(UTextBlock, STB_Title);

	GET_SLOT(UTextBlock, STB_No);
	GET_SLOT(UTextBlock, STB_Confirm);
	GET_SLOT(UTextBlock, STB_Yes);
	GET_SLOT(UTextBlock, STB_ReturnMaterial);
}

void UB2UICostumeDecompositionPopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Modal, &UB2UICostumeDecompositionPopup::OnClickBTN_Modal);
	BIND_CLICK_FUNC_TO_BTN(BTN_NO, &UB2UICostumeDecompositionPopup::OnClickBTN_NO);
	BIND_CLICK_FUNC_TO_BTN(BTN_Yes, &UB2UICostumeDecompositionPopup::OnClickBTN_Yes);
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UICostumeDecompositionPopup::OnClickBTN_Confirm);
}

void UB2UICostumeDecompositionPopup::Init()
{
	Super::Init();
	SubscribeEvent();
}

void UB2UICostumeDecompositionPopup::DestroySelf(UB2UIManager* InUIManager)
{
	UnsubscribeEvent();

	Super::DestroySelf(InUIManager);
}

void UB2UICostumeDecompositionPopup::CloseWidgetDelegate()
{
	OnClickBTN_NO();
}

void UB2UICostumeDecompositionPopup::UpdateStaticText()
{
	if (STB_No.IsValid())
		STB_No->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_No")));

	if (STB_Confirm.IsValid())
		STB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));

	if (STB_Yes.IsValid())
		STB_Yes->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Yes")));

	if (STB_ReturnMaterial.IsValid())
		STB_ReturnMaterial->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_Decomposition_Return")));
}

void UB2UICostumeDecompositionPopup::SubscribeEvent()
{
	UnsubscribeEvent();

	Issues.Add(DeliveryDismantleCostumesClass<FB2ResponseDismantleCostumesPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseDismantleCostumesPtr& msg)
	{
		this->UpdateResultPopupInfo(msg);
	}
	));
	Issues.Add(DeliveryDismantleCostumesFailClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->OnClickBTN_NO();
	}
	));
}

void UB2UICostumeDecompositionPopup::UnsubscribeEvent()
{
	Issues.Empty();
}

void UB2UICostumeDecompositionPopup::OnClickBTN_Modal()
{

}

void UB2UICostumeDecompositionPopup::OnClickBTN_NO()
{
	this->DestroySelf(UB2UIManager::GetInstance());
}

void UB2UICostumeDecompositionPopup::OnClickBTN_Yes()
{
	TArray<int64> ItemIds;
	ItemIds.Add(CurrentCostumeIndex); // 일단은 하나씩만 보내도록 기획쪽으로 정해져있음...

	data_trader::Retailer::GetInstance().RequestDismantleCostumes(ItemIds);
}

void UB2UICostumeDecompositionPopup::OnClickBTN_Confirm()
{
	this->DestroySelf(UB2UIManager::GetInstance());
}

void UB2UICostumeDecompositionPopup::UpdatePopupInfo(const FB2Item& InItem)
{
	CurrentCostumeIndex = InItem.InstanceUID;

	bool bIsWeapon = (InItem.InventoryType == EItemInvenType::EIIVT_CostumeWeapon);
	bool bIsMoreItem = (InItem.EnhanceLevel > 0);

	if (WS_Buttons.IsValid())
		WS_Buttons->SetActiveWidgetIndex(0);

	if (RTB_ObtainDesc.IsValid())
		RTB_ObtainDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_Decomposition_Note")));
	
	if (STB_Title.IsValid())
		STB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Costume_Decomposition_Title")));

	if (P_MoreItems.IsValid())
		P_MoreItems->SetVisibility(bIsMoreItem ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (List_ReturnMaterial.IsValid())
		List_ReturnMaterial->ClearChildren();

	if (List_MainMaterials.IsValid())
		List_MainMaterials->ClearChildren();

	FItemDismantleInfo CostInfo;
	BladeIIGameImpl::GetClientDataStore().GetItemDismantleInfoData(InItem.InventoryType, InItem.StarGrade, CostInfo);
	if (CostInfo.Matrial.Num() > 0)
	{
		if (List_MainMaterials.IsValid())
		{
			for(int32 i = 0; i< CostInfo.Matrial.Num(); i++)
			{
				UB2UICostumeDecompositionItem* NewItemRow = CreateWidget<UB2UICostumeDecompositionItem>(GetOwningPlayer(), UICostumeItemClass);
				if (NewItemRow)
				{
					NewItemRow->Init();
					NewItemRow->SetInfo(CostInfo.Matrial[i], CostInfo.MatrialAmount[i]);
					List_MainMaterials->AddChild(NewItemRow);
				}
			}
		}
	}

	const FCostumeEnhanceCostInfo* MoreCostInfo = BladeIIGameImpl::GetClientDataStore().GetCostumeEnhanceCost(InItem.StarGrade, InItem.EnhanceLevel);
	if (MoreCostInfo)
	{
		if (List_ReturnMaterial.IsValid() && bIsMoreItem)
		{
			UB2UICostumeDecompositionItem* NewItemRow = CreateWidget<UB2UICostumeDecompositionItem>(GetOwningPlayer(), UICostumeItemClass);
			if (NewItemRow)
			{
				NewItemRow->Init();

				if (bIsWeapon)
				{
					NewItemRow->SetInfo(MoreCostInfo->WeaponMaterialId, MoreCostInfo->WeaponRestoreAmount);
				}
				else
				{
					NewItemRow->SetInfo(MoreCostInfo->ArmorMaterialId, MoreCostInfo->ArmorRestoreAmount);
				}

				List_ReturnMaterial->AddChild(NewItemRow);
			}
		}
	}
}

void UB2UICostumeDecompositionPopup::UpdateResultPopupInfo(const FB2ResponseDismantleCostumesPtr& msg) 
{
	// 어차피 아이템 리스트는 똑같으니까 위에서 처리한 그대로 보여주고, 텍스트만 정리.

	if (WS_Buttons.IsValid())
		WS_Buttons->SetActiveWidgetIndex(1);

	if (RTB_ObtainDesc.IsValid())
		RTB_ObtainDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_DecompostionResultDesc")));

	if (STB_Title.IsValid())
		STB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_DecompostionResult")));

	// 아이템 add 처리
	BladeIIGameImpl::GetClientDataStore().DecompositionCostumeItem(msg);
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();

	OnPlayAnimation_IMP();

	if (List_ReturnMaterial.IsValid() && List_MainMaterials.IsValid())
	{
		for (int32 i = 0; i < List_MainMaterials->GetChildrenCount(); i++)
		{
			UB2UICostumeDecompositionItem* NewItemRow = Cast<UB2UICostumeDecompositionItem>(List_MainMaterials->GetChildAt(i));
			if(NewItemRow)
				NewItemRow->OnPlayAnimation_IMP();
		}

		for (int32 i = 0; i < List_ReturnMaterial->GetChildrenCount(); i++)
		{
			UB2UICostumeDecompositionItem* NewItemRow = Cast<UB2UICostumeDecompositionItem>(List_ReturnMaterial->GetChildAt(i));
			if (NewItemRow)
				NewItemRow->OnPlayAnimation_IMP();
		}
	}
}

//////////////////////////////////////////////////////
// UB2UICostumeDecompositionItem
/////////////////////////////////////////////////////

void UB2UICostumeDecompositionItem::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Item);
	GET_SLOT(UTextBlock, TB_Count);
	GET_SLOT(UTextBlock, TB_NeedCount);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_StuffTip);
}

void UB2UICostumeDecompositionItem::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UICostumeDecompositionItem::Init()
{
	Super::Init();
}

void UB2UICostumeDecompositionItem::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UICostumeDecompositionItem::SetInfo(int32 ItemIndex, int32 Amount)
{
	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(ItemIndex) : NULL;

	if (IMG_Item.IsValid())
		IMG_Item->SetBrushFromMaterial(ThisItemInfo ? ThisItemInfo->GetIconMaterial(AllItemInfo) : NULL);

	if (TB_Count.IsValid())
		TB_Count->SetText(FText::AsNumber(Amount));

	if (TB_NeedCount.IsValid())
		TB_NeedCount->SetText(FText::GetEmpty());

	if (BTN_StuffTip.IsValid())
		BTN_StuffTip->SetItemInfo(ItemIndex);
}

void UB2UICostumeDecompositionItem::SetInfo(int32 ItemIndex, int32 CurrentCount, int32 NeedCount)
{
	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(ItemIndex) : NULL;

	if (IMG_Item.IsValid())
		IMG_Item->SetBrushFromMaterial(ThisItemInfo ? ThisItemInfo->GetIconMaterial(AllItemInfo) : NULL);

	if (TB_Count.IsValid())
	{
		bool bNotEnough = (CurrentCount < NeedCount);
		FText Count = (CurrentCount >= 10000) ? FText::FromString(TEXT("9999+")) : FText::FromString(FString::FromInt(CurrentCount));

		TB_Count->SetColorAndOpacity(bNotEnough ? UB2UIManager::GetInstance()->TextColor_NonSatisfy : FLinearColor::White);
		TB_Count->SetText(Count);
	}

	if (TB_NeedCount.IsValid())
		TB_NeedCount->SetText(FText::FromString(TEXT("/ ")+FString::FromInt(NeedCount)));

	if (BTN_StuffTip.IsValid())
		BTN_StuffTip->SetItemInfo(ItemIndex);
}