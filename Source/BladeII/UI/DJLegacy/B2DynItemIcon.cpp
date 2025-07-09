// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2DynItemIcon.h"
#include "B2Inventory.h"
#include "B2ItemInfo.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "B2LobbyUI_ItemDetail.h"
#include "Event.h"
#include "B2LobbyUI_InventoryMain.h"
#include "B2UIStarGradePart.h"

//
// Note : ������ ������ ��ü�� ��ư���� ����ϱ� ���� �ٴڿ� ��ư�� ��Ƴ����� �� ������,
//		Ư���� ������ ���� �̻� Visibility �� HitTestInvisible �� ���.
//

UB2DynItemIcon::UB2DynItemIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NonScaledSize = FVector2D(100.0f, 100.0f); // ���� ���� ������ ��� ������ �ʿ䰡 ����.

	BackgroundImageNRef = NULL;
	MainIconImageNRef = NULL;
	ItemNameTextBlockNRef = NULL;
}

void UB2DynItemIcon::NativeConstruct()
{
	Super::NativeConstruct();

}

void UB2DynItemIcon::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_ExtraSlotEffect);
	GET_SLOT(UB2UIStarGradePart, StarGradePart);

	if (StarGradePart.IsValid())
	{
		StarGradePart->Init();
	}
}

void UB2DynItemIcon::UpdateItemData(const FB2Item& InItem)
{
	NativeItemData = InItem;

	// Ŭ���̾�Ʈ ������ ���ҽ� ���̺�
	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(NativeItemData.ItemRefID) : NULL;
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();

	// UMG �ʿ� ��ġ�� widget �� ������Ʈ

	// ���� �������̳� ������ ������ ���� ����..
	SetBackgroundImageFromGrade((IsEquipmentItem(InItem) || IsEssenceItem(InItem)) ? InItem.StarGrade : 1, (InItem.InventoryType == EItemInvenType::EIIVT_Consumables));

	if (MainIconImageNRef)
	{
		MainIconImageNRef->SetBrushFromMaterial(ThisItemInfo ? ThisItemInfo->GetIconMaterial(AllItemInfo) : NULL);
	}
	if (ItemNameTextBlockNRef)
	{ // ������ �̸����� ǰ�� ���� ǥ���� ��. GetLocalizedName �� �Ű����� �ȳѰ���.
		ItemNameTextBlockNRef->SetText(ThisItemInfo ? ThisItemInfo->GetLocalizedName() : FText::FromString(TEXT("Unknown Item")));
		if (ItemMiscInfo)
		{
			ItemNameTextBlockNRef->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(InItem.StarGrade));
		}
	}

	if (StarGradePart.IsValid())
	{
		StarGradePart->UpdateStar(NativeItemData);
	}

	if (IMG_ExtraSlotEffect.IsValid())
	{
		IMG_ExtraSlotEffect->SetVisibility((ThisItemInfo && ThisItemInfo->bHasExtraSlotEffect) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	// Widget Blueprint �� ���� ������ ���/ǰ�� �� �̹��� ��
	UpdateItemData_BP(NativeItemData.StarGrade, NativeItemData.Quality, (NativeItemData.SurpassCount > 0));
}

void UB2DynItemIcon::SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (BackgroundImageNRef)
	{
		// �⺻������ Simple �� ����ϴµ� ���� Ŭ���� �ʼ� ������ ���� �ٲ㼭 ���.
		UMaterialInterface* BGMtrl = ItemMiscInfo ? ItemMiscInfo->GetItemIconBGMtrl(InItemGrade, true, bForConsumables) : NULL;
		if (BGMtrl)
		{
			BackgroundImageNRef->SetBrushFromMaterial(BGMtrl);
		}
	}
}

int32 UB2DynItemIcon::GetItemStarGrade() const
{
	return NativeItemData.StarGrade;
}

UB2LobbyUI_ItemDetail* UB2DynItemIcon::CreateLobbyItemDetailPopupCommon(UObject* WorldContextObject, FB2Item& InItemData, bool bIsForEquippedItem)
{
	// ������ ������ Ȥ�� ���� ������
	const ELobbySubPopups LobbyPopupEnum = bIsForEquippedItem ? ELobbySubPopups::ELSPU_EquippedItemDetail : ELobbySubPopups::ELSPU_StoredItemDetail;

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// �ش� user widget ���� ��ȣ.
	// ������ Popup widget �� UIP ó�� ��ġ�� ���� �г� ������ ������ش�.
	UB2LobbyUI_InventoryMain* InventoryUI = LobbyGM ? Cast<UB2LobbyUI_InventoryMain>(LobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
	UPanelWidget* PopupAddPanel = InventoryUI ? InventoryUI->GetItemDetailPopupParentPanel(bIsForEquippedItem ? true : false) : nullptr;
	
	//DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(LobbyPopupEnum, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));

	// ������ user widget �� ã�Ƽ� ItemData �� ������Ʈ
	UB2LobbyUI_ItemDetail* CreatedDetailWindow = LobbyGM ? Cast<UB2LobbyUI_ItemDetail>(LobbyGM->DJLegacy_FindLobbySubPopup(LobbyPopupEnum)) : NULL;

	if (CreatedDetailWindow)
	{
		CreatedDetailWindow->SetIsForEquippedItem(bIsForEquippedItem);
		CreatedDetailWindow->UpdateItemData(InItemData);
	}

	return CreatedDetailWindow;
}