// Fill out your copyright notice in the Description page of Project Settings.
#include "B2AcqItemEquipSuggest.h"
//#include "BladeII.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "Event.h"
#include "B2ItemInfo.h"
#include "B2UIUtil.h"
#include "BladeIIUtil.h"

UB2AcqItemEquipSuggest::UB2AcqItemEquipSuggest(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemIconWidgetClass = NULL;
	CreatedItemIcon = NULL;
}

void UB2AcqItemEquipSuggest::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, CP_ItemIcon);
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_PrimPointType);
	GET_SLOT(UTextBlock, TB_PrimPoint);
	//GET_SLOT(UTextBlock, TB_IncAmount);
	GET_SLOT(UTextBlock, TB_ItemName);
	GET_SLOT(UTextBlock, TB_Equip);

	GET_SLOT(UB2Button, BTN_Equip);
	BIND_CLICK_FUNC_TO_BTN(BTN_Equip, &UB2AcqItemEquipSuggest::OnClickedEquipBtn);
	
	GET_SLOT(UB2Button, BTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2AcqItemEquipSuggest::OnClickedCloseBtn);
	
	UpdateStaticTexts();
}

void UB2AcqItemEquipSuggest::DestroySelf()
{
	Super::DestroySelf();

	DestroyItemIcon();
}

void UB2AcqItemEquipSuggest::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
}

void UB2AcqItemEquipSuggest::UpdateStaticTexts()
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_ItemAcquired"))));
	}
	if (TB_Equip.IsValid())
	{
		TB_Equip->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Equip"))));
	}
}

void UB2AcqItemEquipSuggest::UpdateData(bool bWasEqiupped, const FB2Item& CurrentEquippedItem, const FB2Item& InSuggestedItem, B2AcqItemEquipSuggestCBType InOnCloseCB)
{
	EquipSuggestedItem = InSuggestedItem;

	OnCloseCB = InOnCloseCB;

	DestroyItemIcon();
	if (CP_ItemIcon.IsValid() && ItemIconWidgetClass)
	{
		UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(CP_ItemIcon->Slot);
		FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);

		UB2DynItemIcon_LobbyInven* DynIconCDO = Cast<UB2DynItemIcon_LobbyInven>(ItemIconWidgetClass->GetDefaultObject());

		// ItemIconPanelNRef ���� �������� ������ ������ widget ����
		CreatedItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
			ItemIconWidgetClass, this, CP_ItemIcon.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
			FVector2D(0.0f, 0.0f), 0, false
			));

		if (CreatedItemIcon)
		{			
			CreatedItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_AcqItemEquipSuggest);
			CreatedItemIcon->UpdateItemData(EquipSuggestedItem);
		}
	}

	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(EquipSuggestedItem.ItemRefID) : NULL;

	// �����ۿ� ���� ���ŵǴ� ��ġ��
	if (TB_PrimPointType.IsValid())
	{
		TB_PrimPointType->SetText(GetLOCTextOfPrimPointType(EquipSuggestedItem.PrimaryPointType));
	}
	if (TB_PrimPoint.IsValid())
	{
		const int32 PrimPointInc = !bWasEqiupped ? (int32)EquipSuggestedItem.GetPrimaryPoint() : B2UIUtil::DifferenceItemPrimaryPoint(EquipSuggestedItem, CurrentEquippedItem);
		TB_PrimPoint->SetText(FText::FromString(FString::Printf(TEXT("+ %d"), PrimPointInc)));
		//TB_PrimPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)EquipSuggestedItem.GetPrimaryPoint())));
	}
	/*if (TB_IncAmount.IsValid())
	{
		const int32 PrimPointInc = !bWasEqiupped ? (int32)EquipSuggestedItem.GetPrimaryPoint() : ((int32)EquipSuggestedItem.GetPrimaryPoint() - (int32)CurrentEquippedItem.GetPrimaryPoint());
		TB_IncAmount->SetText(FText::FromString(FString::Printf(TEXT("%d"), PrimPointInc)));
	}*/
	if (TB_ItemName.IsValid())
	{
		TB_ItemName->SetText(ThisItemInfo ? ThisItemInfo->GetLocalizedName() : FText::FromString(TEXT("Unknown Item")));
	}
}

void UB2AcqItemEquipSuggest::OnClickedEquipBtn()
{
	TArray<int64> ReqIDs;
	ReqIDs.Add(EquipSuggestedItem.InstanceUID);
	LobbyReqEquipItemsClass<const TArray<int64>&>::GetInstance().Signal(ReqIDs);

	// �� UI �� �ű� ȹ�� ��� ���� ����Ǵ� ���ε� �и� �� ���� ���� ���� �ǿ��� �� ������ üũ �˸��� ���� ��.
	// �׷��� ������ �ϰ� �ȴٸ� �� ���������� ���� �ű� ȹ�� �˸��� �����ؾ� �Ѵ�.
	// ���� ���� ������ �� ��Ȳ�� �ƴϱ� ������ ���ϰ� üũ�� �� ���̴� ��	
//	BladeIIGameImpl::GetRedDotManager().RemoveNewItem(EquipSuggestedItem);

	CloseMe();
}

void UB2AcqItemEquipSuggest::CloseWidgetDelegate()
{
	OnClickedCloseBtn();
}

void UB2AcqItemEquipSuggest::OnClickedCloseBtn()
{
	CloseMe();
}

void UB2AcqItemEquipSuggest::CloseMe()
{
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AcqItemEquipSuggest);

	if (OnCloseCB)
	{
		OnCloseCB();
	}
}

void UB2AcqItemEquipSuggest::DestroyItemIcon()
{
	if (CreatedItemIcon)
	{
		CreatedItemIcon->DestroySelf();
		CreatedItemIcon = NULL;
	}
}