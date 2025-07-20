// Fill out your copyright notice in the Description page of Project Settings.
#include "B2DynItemIcon.h"
//#include "BladeII.h"
#include "B2Inventory.h"
#include "B2ItemInfo.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "B2LobbyUI_ItemDetail.h"
#include "Event.h"
#include "B2LobbyUI_InventoryMain.h"
#include "B2UIStarGradePart.h"

//
// Note : 酒捞袍 酒捞能 磊眉甫 滚瓢栏肺 荤侩窍扁 困秦 官蹿俊 滚瓢阑 彬酒初疽阑 荐 乐栏聪,
//		漂喊茄 捞蜡啊 绝绰 捞惑 Visibility 绰 HitTestInvisible 阑 荤侩.
//

UB2DynItemIcon::UB2DynItemIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NonScaledSize = FVector2D(100.0f, 100.0f); // 角力 父电 荤捞令 措肺 汲沥瞪 鞘夸啊 乐澜.

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

	// 努扼捞攫飘 酒捞袍 府家胶 抛捞喉
	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(NativeItemData.ItemRefID) : NULL;
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();

	// UMG 率俊 硅摹茄 widget 甸 诀单捞飘

	// 厘馒 酒捞袍捞唱 沥荐父 喊己俊 蝶弗 瞒捞..
	SetBackgroundImageFromGrade((IsEquipmentItem(InItem) || IsEssenceItem(InItem)) ? InItem.StarGrade : 1, (InItem.InventoryType == EItemInvenType::EIIVT_Consumables));

	if (MainIconImageNRef)
	{
		MainIconImageNRef->SetBrushFromMaterial(ThisItemInfo ? ThisItemInfo->GetIconMaterial(AllItemInfo) : NULL);
	}
	if (ItemNameTextBlockNRef)
	{ // 酒捞袍 捞抚俊辑 前龙 蝶肺 钎矫且 芭. GetLocalizedName 俊 概俺函荐 救逞败淋.
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

	// Widget Blueprint 辑 埃祈 技泼且 殿鞭/前龙 喊 捞固瘤 殿
	UpdateItemData_BP(NativeItemData.StarGrade, NativeItemData.Quality, (NativeItemData.SurpassCount > 0));
}

void UB2DynItemIcon::SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (BackgroundImageNRef)
	{
		// 扁夯利栏肺 Simple 阑 荤侩窍绰单 窍困 努贰胶 率辑 汲沥俊 蝶扼 官层辑 荤侩.
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
	// 焊包窃 酒捞袍 趣篮 厘馒 酒捞袍
	const ELobbySubPopups LobbyPopupEnum = bIsForEquippedItem ? ELobbySubPopups::ELSPU_EquippedItemDetail : ELobbySubPopups::ELSPU_StoredItemDetail;

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// 秦寸 user widget 积己 脚龋.
	// 积己等 Popup widget 阑 UIP 贸烦 硅摹秦 持阑 菩澄 沥焊甫 父甸绢霖促.
	UB2LobbyUI_InventoryMain* InventoryUI = LobbyGM ? Cast<UB2LobbyUI_InventoryMain>(LobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
	UPanelWidget* PopupAddPanel = InventoryUI ? InventoryUI->GetItemDetailPopupParentPanel(bIsForEquippedItem ? true : false) : nullptr;
	
	//DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(LobbyPopupEnum, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));

	// 积己等 user widget 阑 茫酒辑 ItemData 甫 诀单捞飘
	UB2LobbyUI_ItemDetail* CreatedDetailWindow = LobbyGM ? Cast<UB2LobbyUI_ItemDetail>(LobbyGM->DJLegacy_FindLobbySubPopup(LobbyPopupEnum)) : NULL;

	if (CreatedDetailWindow)
	{
		CreatedDetailWindow->SetIsForEquippedItem(bIsForEquippedItem);
		CreatedDetailWindow->UpdateItemData(InItemData);
	}

	return CreatedDetailWindow;
}