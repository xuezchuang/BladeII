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
// Note : 아이템 아이콘 자체를 버튼으로 사용하기 위해 바닥에 버튼을 깔아놓았을 수 있으니,
//		특별한 이유가 없는 이상 Visibility 는 HitTestInvisible 을 사용.
//

UB2DynItemIcon::UB2DynItemIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NonScaledSize = FVector2D(100.0f, 100.0f); // 실제 만든 사이즈 대로 설정될 필요가 있음.

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

	// 클라이언트 아이템 리소스 테이블
	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(NativeItemData.ItemRefID) : NULL;
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();

	// UMG 쪽에 배치한 widget 들 업데이트

	// 장착 아이템이나 정수만 별성에 따른 차이..
	SetBackgroundImageFromGrade((IsEquipmentItem(InItem) || IsEssenceItem(InItem)) ? InItem.StarGrade : 1, (InItem.InventoryType == EItemInvenType::EIIVT_Consumables));

	if (MainIconImageNRef)
	{
		MainIconImageNRef->SetBrushFromMaterial(ThisItemInfo ? ThisItemInfo->GetIconMaterial(AllItemInfo) : NULL);
	}
	if (ItemNameTextBlockNRef)
	{ // 아이템 이름에서 품질 따로 표시할 거. GetLocalizedName 에 매개변수 안넘겨줌.
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

	// Widget Blueprint 서 간편 세팅할 등급/품질 별 이미지 등
	UpdateItemData_BP(NativeItemData.StarGrade, NativeItemData.Quality, (NativeItemData.SurpassCount > 0));
}

void UB2DynItemIcon::SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (BackgroundImageNRef)
	{
		// 기본적으로 Simple 을 사용하는데 하위 클래스 쪽서 설정에 따라 바꿔서 사용.
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
	// 보관함 아이템 혹은 장착 아이템
	const ELobbySubPopups LobbyPopupEnum = bIsForEquippedItem ? ELobbySubPopups::ELSPU_EquippedItemDetail : ELobbySubPopups::ELSPU_StoredItemDetail;

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// 해당 user widget 생성 신호.
	// 생성된 Popup widget 을 UIP 처럼 배치해 넣을 패널 정보를 만들어준다.
	UB2LobbyUI_InventoryMain* InventoryUI = LobbyGM ? Cast<UB2LobbyUI_InventoryMain>(LobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
	UPanelWidget* PopupAddPanel = InventoryUI ? InventoryUI->GetItemDetailPopupParentPanel(bIsForEquippedItem ? true : false) : nullptr;
	
	//DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(LobbyPopupEnum, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));

	// 생성된 user widget 을 찾아서 ItemData 를 업데이트
	UB2LobbyUI_ItemDetail* CreatedDetailWindow = LobbyGM ? Cast<UB2LobbyUI_ItemDetail>(LobbyGM->DJLegacy_FindLobbySubPopup(LobbyPopupEnum)) : NULL;

	if (CreatedDetailWindow)
	{
		CreatedDetailWindow->SetIsForEquippedItem(bIsForEquippedItem);
		CreatedDetailWindow->UpdateItemData(InItemData);
	}

	return CreatedDetailWindow;
}