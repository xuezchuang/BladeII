// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2Button.h"
#include "B2UIWidgetBase.h"
#include "B2UIStarGradePart.h"
#include "B2ItemInfo.h"
#include "B2Inventory.h"
#include "B2LobbyInventory.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2LobbyGameMode.h"
#include "BladeIIGameMode.h"
#include "Event.h"
#include "B2DynItemIcon_SealBoxPreview.h"
#include "B2UISealBoxResultSimple.h"
#include "CharacterDataStore.h"
#include "CommonStruct.h"


UB2DynItemIcon_SealBoxPreview::UB2DynItemIcon_SealBoxPreview
(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	CachedCharacterStore = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	bIsNativeItemLocked = false;
}

void UB2DynItemIcon_SealBoxPreview::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UImage, MainIconImage);
	GET_SLOT(UTextBlock, ItemAmountText);
	GET_SLOT(UTextBlock, ItemLevelText);
	GET_SLOT(UTextBlock, ItemEnhanceLevelText);
	GET_SLOT(UImage, ItemLockedStateImage_Store);
	GET_SLOT(UImage, ItemLockedStateImage_Detail);
	GET_SLOT(UImage, ItemQualityBGImage);
	GET_SLOT(UTextBlock, ItemQualityTextBlock);
	GET_SLOT(UTextBlock, TB_Preset);
	GET_SLOT(UImage, IMG_EquippedCheck);
	GET_SLOT(UImage, DisabledSlotImage);
	GET_SLOT(UB2UIWidgetBase, UIP_RedDot);
	GET_SLOT(UTextBlock, ItemNameText);
	GET_SLOT(UOverlay, OV_LockedImages);
	GET_SLOT(UPanelWidget, P_EquipInfo);

	GET_SLOT(UOverlay, OV_ItemQualitySet);

	GET_SLOT(UOverlay, OV_Selected);

	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2DynItemIcon_SealBoxPreview::OnClickBTNArea);

	GET_SLOT(UImage, BackgroundImage);
	GET_SLOT(UImage, IMG_CanNotSelect);

	GET_SLOT(UCanvasPanel, CP_Main);
	GET_SLOT(UCanvasPanel, CP_BackGroundOnly);

	if (UIP_RedDot.IsValid())
	{
		UIP_RedDot->SetVisibility(ESlateVisibility::Hidden);
	}
	if (OV_LockedImages.IsValid())
	{
		OV_LockedImages->SetVisibility(ESlateVisibility::Hidden);
	}
	if (P_EquipInfo.IsValid()) {
		P_EquipInfo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2DynItemIcon_SealBoxPreview::UpdateItemData(const FB2Item& InItem)
{
	Super::UpdateItemData(InItem);

	//UE_LOG(LogTemp, Log, TEXT("UpdateItemdata"));

	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(NativeItemData.ItemRefID) : NULL;
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();

	SetBackgroundImageFromGrade((IsEquipmentItem(InItem) || IsEssenceItem(InItem)) ? InItem.StarGrade : 1, (InItem.InventoryType == EItemInvenType::EIIVT_Consumables));

	if (MainIconImage != nullptr)
	{
		MainIconImage->SetBrushFromMaterial(ThisItemInfo ? ThisItemInfo->GetIconMaterial(AllItemInfo) : NULL);
	}

	if (ItemNameText != nullptr)
	{ // ������ �̸����� ǰ�� ���� ǥ���� ��. GetLocalizedName �� �Ű����� �ȳѰ���.
		ItemNameText->SetText(ThisItemInfo ? ThisItemInfo->GetLocalizedName() : FText::FromString(TEXT("Unknown Item")));
		if (ItemMiscInfo)
		{
			ItemNameText->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(InItem.StarGrade));
		}
	}

	if (IMG_ExtraSlotEffect.IsValid())
	{
		IMG_ExtraSlotEffect->SetVisibility((ThisItemInfo && ThisItemInfo->bHasExtraSlotEffect) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	const bool bIsEquipmentItem = IsEquipmentItem(InItem);
	const bool bIsEssenceItem = IsEssenceItem(InItem);
	const bool bIsCostumeItem = IsCostumeItem(InItem);
	const bool bIsAnvilItem = IsAnvilItem(InItem);

	// ���� �����¿� ���� ���� Ȯ���Ͽ� ���� V üũ �� ������ �� ���� �˾����� �̵��ϰ� �ؾ� ��.

	//if (DisabledSlotImage.IsValid())
	//	DisabledSlotImage->SetVisibility(GetDisabledSlotImageVisibility());

	if (bIsEquipmentItem)
	{
		if (P_EquipInfo.IsValid()) {
			P_EquipInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (ItemQualityTextBlock.IsValid())
		{
			ItemQualityTextBlock->SetText(GetLOCTextOfItemQuality(InItem.Quality));
		}
		if (ItemQualityBGImage.IsValid())
		{

			ItemQualityBGImage->SetVisibility(ESlateVisibility::Visible);
			UMaterialInterface* QualityBGMtrl = ItemMiscInfo ? ItemMiscInfo->GetItemQualityBGMtrl(InItem.Quality) : NULL;
			if (QualityBGMtrl)
				ItemQualityBGImage->SetBrushFromMaterial(QualityBGMtrl);
		}

		if (OV_ItemQualitySet.IsValid())
		{
			OV_ItemQualitySet->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (P_EquipInfo.IsValid()) {
			P_EquipInfo->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (StarGradePart.IsValid())
	{
		if (bIsCostumeItem)
			StarGradePart->SetVisibility(ESlateVisibility::Collapsed);
		else
			StarGradePart->UpdateStar(InItem);
	}

	if (UIP_RedDot.IsValid())
	{
		UIP_RedDot->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ItemLevelText.IsValid())
	{
		/*	FString t = (GetItemLevelText().ToString());
		UE_LOG(LogTemp, Log, TEXT("%s"), *t);*/
		ItemLevelText->SetText(GetItemLevelText());
		ItemLevelText->SetVisibility(ESlateVisibility::Visible);
	}

	if (ItemEnhanceLevelText.IsValid())
	{

		/*FString t = (GetItemEnhanceLevelText().ToString());

		UE_LOG(LogTemp, Log, TEXT("%s"), *t);*/
		ItemEnhanceLevelText->SetText(GetItemEnhanceLevelText());
		ItemEnhanceLevelText->SetVisibility(ESlateVisibility::Visible);
	}

	if (ItemAmountText.IsValid())
	{
		if (!bIsEquipmentItem)
		{
			/*FString t = (GetItemAmountText().ToString());
			UE_LOG(LogTemp, Log, TEXT("%s"), *t);*/
			ItemAmountText->SetText(GetItemAmountText());
			ItemAmountText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemAmountText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

//�Ҹ�ǰ ������ ����
void UB2DynItemIcon_SealBoxPreview::SetItem(int32 RefId, int32 minQuantity, int32 maxQuantity)
{
	// �̸�, ������, ����, ����Ʈ �Ǵ��߰� �ʿ�
	EItemClass TempItemClass = FItemRefIDHelper::ExtractItemClassFromRefIDForItemForge(RefId);
	NativeItemData.ItemRefID = RefId;
	NativeItemData.InventoryType = GetItemInvenTypeFromClass(TempItemClass);
	UMaterialInterface* MI = StaticFindItemInfo()->GetItemIcon(RefId);
	if (MI)
	{
		MainIconImage->SetBrushFromMaterial(MI);
	}


	FSingleItemInfoData* ItemInfo = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(RefId) : nullptr;
	if (ItemInfo != nullptr)
	{
		ItemNameText->SetText(ItemInfo->GetLocalizedName());
	}
	int32 StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(RefId);
	//stargrade ����

	if (StarGradePart.IsValid())
	{
		FB2Item dummyItem;
		//StarGradePart->UpdateStar(StarGrade, requiredItemLevel > dummyItem.ItemNormalMaxLevel ? true : false);	//������ ��ű��� ��쵵 �׳� 30 �Ѿ�� �ʿ� ���� ���� ��.
	}

	if (maxQuantity<0)
	{
		if (minQuantity < 0)
		{
			ItemAmountText->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ItemAmountText->SetText(FText::AsNumber(minQuantity));
			ItemAmountText->SetVisibility(ESlateVisibility::Visible);
		}
		
	}
	else
	{
		ItemAmountText->SetText(GetItemAmountText(minQuantity,maxQuantity));
		ItemAmountText->SetVisibility(ESlateVisibility::Visible);
	}

	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(RefId) : NULL;

	if (!ThisItemInfo->bHasExtraSlotEffect)
	{
		IMG_ExtraSlotEffect->SetVisibility(ESlateVisibility::Hidden);
	}

	EItemInvenType EIIT = GetItemInvenTypeFromClass(TempItemClass);

	SetBackgroundImageFromGrade(
		(FItemRefIDHelper::IsEquipmentItemByRefID(RefId) || FItemRefIDHelper::IsEssenceItemByRefID(RefId)) ? StarGrade : 1,
		(EIIT == EItemInvenType::EIIVT_Consumables));
	
	if (ItemEnhanceLevelText.IsValid())
	{
		ItemEnhanceLevelText->SetVisibility(ESlateVisibility::Hidden);
	}
	if (ItemLevelText.IsValid())
	{
		ItemLevelText->SetVisibility(ESlateVisibility::Hidden);
	}
	if (OV_ItemQualitySet.IsValid())
	{
		OV_ItemQualitySet->SetVisibility(ESlateVisibility::Hidden);
	}
	if (StarGradePart.IsValid())
	{
		StarGradePart->SetVisibility(ESlateVisibility::Collapsed);
	}
}

//��� ������ ��
void UB2DynItemIcon_SealBoxPreview::SetItem(int32 groupID, EPCClass equipClass, int32 equipCategory, int32 equipPlace, int32 starGrade, int32 quality, int32 enhancedLevel, int32 surpassLevel)
{
	// �̸�, ������, ����, ����Ʈ �Ǵ��߰� �ʿ�
	int32 RefId = GenerateRefID(groupID, equipClass, equipPlace, equipCategory, starGrade);
	ItemAmountText->SetVisibility(ESlateVisibility::Hidden);

	EItemClass TempItemClass = FItemRefIDHelper::ExtractItemClassFromRefIDForItemForge(RefId);
	NativeItemData.ItemRefID = RefId;
	NativeItemData.InventoryType = GetItemInvenTypeFromClass(TempItemClass);
	UMaterialInterface* MI = StaticFindItemInfo()->GetItemIcon(RefId);
	if (MI)
	{
		if (MainIconImage != nullptr)
			MainIconImage->SetBrushFromMaterial(MI);
	}

	FSingleItemInfoData* ItemInfo = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(RefId) : nullptr;
	if (ItemInfo != nullptr)
	{
		ItemNameText->SetText(ItemInfo->GetLocalizedName());
	}
	int32 StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(RefId);
	//stargrade ����

	if (StarGradePart.IsValid())
	{
		FB2Item dummyItem;
		StarGradePart->UpdateStar(StarGrade, surpassLevel > 0 ? true : false);	//�ʿ� ī��Ʈ�� �ö󰡸� �ʿ����� ���
	}

	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(RefId) : NULL;

	if (!ThisItemInfo->bHasExtraSlotEffect)
	{
		IMG_ExtraSlotEffect->SetVisibility(ESlateVisibility::Hidden);
	}

	EItemInvenType EIIT = GetItemInvenTypeFromClass(TempItemClass);

	SetBackgroundImageFromGrade(
		(FItemRefIDHelper::IsEquipmentItemByRefID(RefId) || FItemRefIDHelper::IsEssenceItemByRefID(RefId)) ? StarGrade : 1,
		(EIIT == EItemInvenType::EIIVT_Consumables));

	if (enhancedLevel > 0)
	{
		ItemEnhanceLevelText->SetVisibility(ESlateVisibility::HitTestInvisible);
		ItemEnhanceLevelText->SetText(SetItemEnhanceLevelText(enhancedLevel));
	}
	else
	{
		ItemEnhanceLevelText->SetVisibility(ESlateVisibility::Hidden);
	}
	
	ItemLevelText->SetVisibility(ESlateVisibility::HitTestInvisible);
	ItemLevelText->SetText(SetItemLevelText(1));

	if (quality > 0 && quality <= MAX_ITEM_QUALITY)
	{
		if (OV_ItemQualitySet.IsValid())
		{
			OV_ItemQualitySet->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (ItemQualityTextBlock.IsValid())
		{
			ItemQualityTextBlock->SetText(GetLOCTextOfItemQuality(quality));
		}
		if (ItemQualityBGImage.IsValid())
		{
			UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();

			ItemQualityBGImage->SetVisibility(ESlateVisibility::Visible);
			UMaterialInterface* QualityBGMtrl = ItemMiscInfo ? ItemMiscInfo->GetItemQualityBGMtrl(quality) : NULL;
			if (QualityBGMtrl)
				ItemQualityBGImage->SetBrushFromMaterial(QualityBGMtrl);
		}
	}
	else
	{
		if (OV_ItemQualitySet.IsValid())
		{
			OV_ItemQualitySet->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2DynItemIcon_SealBoxPreview::SetItem(int32 RefId, bool showQuantity)
{
	// �̸�, ������, ����, ����Ʈ �Ǵ��߰� �ʿ�
	EItemClass TempItemClass = FItemRefIDHelper::ExtractItemClassFromRefIDForItemForge(RefId);
	NativeItemData.ItemRefID = RefId;
	NativeItemData.InventoryType = GetItemInvenTypeFromClass(TempItemClass);
	UMaterialInterface* MI = StaticFindItemInfo()->GetItemIcon(RefId);
	if (MI)
	{
		MainIconImage->SetBrushFromMaterial(MI);
	}


	FSingleItemInfoData* ItemInfo = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(RefId) : nullptr;
	if (ItemInfo != nullptr)
	{
		ItemNameText->SetText(ItemInfo->GetLocalizedName());
	}
	int32 StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(RefId);
	//stargrade ����

// 	if (StarGradePart.IsValid())
// 	{
// 		FB2Item dummyItem;
// 		StarGradePart->UpdateStar(StarGrade, surpassLevel > 0 ? true : false);	//�ʿ� ī��Ʈ�� �ö󰡸� �ʿ����� ���
// 	}

	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(RefId) : NULL;

	if (!ThisItemInfo->bHasExtraSlotEffect)
	{
		IMG_ExtraSlotEffect->SetVisibility(ESlateVisibility::Hidden);
	}

	EItemInvenType EIIT = GetItemInvenTypeFromClass(TempItemClass);

	SetBackgroundImageFromGrade(
		(FItemRefIDHelper::IsEquipmentItemByRefID(RefId) || FItemRefIDHelper::IsEssenceItemByRefID(RefId)) ? StarGrade : 1,
		(EIIT == EItemInvenType::EIIVT_Consumables));
}

void UB2DynItemIcon_SealBoxPreview::SetResultPannel(class UB2UISealBoxResultSimple* mainResult)
{
	resultPannel = mainResult;
}

int32 UB2DynItemIcon_SealBoxPreview::GenerateRefID(int32 groupID, EPCClass equipClass, int32 equipPlace, int32 equipCategory, int32 starGrade)
{
	int32 firstDigit, secondDigit, thirdDigit, fourthDigit, fifthDigit, sixthDigit, seventhDigit, finalResult;

	//firstDigit
	switch (equipCategory)
	{
	case b2network::B2EquipCategory::WEAPON:
	case b2network::B2EquipCategory::ARMOR:
	case b2network::B2EquipCategory::ACCESSORY:
		firstDigit = FItemRefIDHelper::GetEquipmentDigit();
		break;

	case b2network::B2EquipCategory::WEAPON_ESSENCE:
	case b2network::B2EquipCategory::ARMOR_ESSENCE:
	case b2network::B2EquipCategory::ACCESSORY_ESSENCE:
		firstDigit = FItemRefIDHelper::GetEssenceDigit();
		break;

	case b2network::B2EquipCategory::WEAPON_SURPASS_ANVIL:
	case b2network::B2EquipCategory::ARMOR_SURPASS_ANVIL:
	case b2network::B2EquipCategory::ACCESSORY_SURPASS_ANVIL:
		firstDigit = FItemRefIDHelper::GetSurpassAnvilDigit();
		break;

	case b2network::B2EquipCategory::COSTUME_WEAPON:
	case b2network::B2EquipCategory::COSTUME_HAT:
	case b2network::B2EquipCategory::COSTUME_ARMOR:
		firstDigit = FItemRefIDHelper::GetCostumeDigit();
		break;

	default:
		return -1;
		break;
	}

	secondDigit = PCClassToInt(equipClass);

	thirdDigit = (int32)(starGrade / 10);

	if (firstDigit == FItemRefIDHelper::GetCostumeDigit())
	{
		fourthDigit = equipPlace % 200;

		int32 tempGroupID;
		switch (groupID)	//�ڽ�Ƭ�� �κ��� CodeTable�� RefID ���� ��Ģ�� ���� �ʾƼ� ������ �ϵ��ڵ� �ؾ� ��.
		{
		case (int32)b2network::B2GroupPrefix::COLD:	//Ȥ����
			tempGroupID = 1;
			break;
		case (int32)b2network::B2GroupPrefix::CORRUPT:	//Ÿ����
			tempGroupID = 2;
			break;
		case (int32)b2network::B2GroupPrefix::BEACH:	//�غ���
			tempGroupID = 3;
			break;
		case (int32)b2network::B2GroupPrefix::NEW_YEAR:	//������
			tempGroupID = 0;
			break;
		case (int32)b2network::B2GroupPrefix::HALLOWEEN:	//������
			tempGroupID = 4;
			break;
		default:
			tempGroupID = 0;
			break;
		}

		sixthDigit = (int32)(tempGroupID / 10);
		seventhDigit = tempGroupID % 10;
	}
	else
	{
		fourthDigit = equipPlace - 1;	//CodeTable���� ���� ��ġ�� 0���� �����ϴ°� �ƴ� 1�� �����ؼ� ���⼭ 1�� ���� �Ѵ�.

		sixthDigit = (int32)((groupID - 1) / 10);
		seventhDigit = (groupID - 1) % 10;
	}

	fifthDigit = starGrade % 10;

	finalResult = (firstDigit * 1000000) + (secondDigit * 100000) + (thirdDigit * 10000) + (fourthDigit * 1000) + (fifthDigit * 100) + (sixthDigit * 10) + seventhDigit;
	return finalResult;
}

void UB2DynItemIcon_SealBoxPreview::SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();

	if (BackgroundImage.IsValid() && ItemMiscInfo)
	{
		// ������ ���� Simple �� ����ϴ� �� �ְ� �ƴ� �� ����.
		UMaterialInterface* BGMtrl = ItemMiscInfo->GetItemIconBGMtrl(InItemGrade,
			false,
			bForConsumables
		);
		if (BGMtrl)
		{
			BackgroundImage->SetBrushFromMaterial(BGMtrl);
		}
	}
}

ESlateVisibility UB2DynItemIcon_SealBoxPreview::GetDisabledSlotImageVisibility() const
{
	// �����۰�ȭ ��ȣ�� üũ
	if (NativeItemData.ItemClass == EItemClass::EIC_ProtectionScroll)
	{
		// ���� NativeItemData �� ����ؼ� ���� ����â �˾�... ���� �ƴ϶� ��ȭ ����� ���� ������ �ؾ�.
		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
		if (LobbyInven && LobbyInven->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_Enhancement)
			return ESlateVisibility::HitTestInvisible;
	}

	return ESlateVisibility::Hidden;
}

void UB2DynItemIcon_SealBoxPreview::OnClickedSenderBTNArea()
{
	Super::OnClickedSenderBTNArea();
}

void UB2DynItemIcon_SealBoxPreview::OnPressedSenderBTNArea()
{
	Super::OnPressedSenderBTNArea();
}

void UB2DynItemIcon_SealBoxPreview::OnReleasedSenderBTNArea()
{
	Super::OnReleasedSenderBTNArea();
}

/*
void UB2DynItemIcon_SealBoxPreview::InitItemForgeSlot(EItemForgeSlot EIFS, EItemForgeItemIconUsage UseType, int32 FactoryId, int32 enhanceLevel, int32 itemLevel)
{
	SetMaterialSlotType(EIFS);
	SetUseType(UseType);
	FactoryID = FactoryId;

	requiredEnhanceLevel = enhanceLevel;
	requiredItemLevel = itemLevel;
}
*/
void UB2DynItemIcon_SealBoxPreview::OnClickBTNArea()
{
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
	/*
	if (NativeItemData.bIsLocked || LobbyInven->IsContainsAllPreset(NativeItemData))
	{
		AskForUnlockItemPopupCommon();
	}
	else
	{
		DeliverySelectItemForgeClass<FB2Item&>::GetInstance().Signal(NativeItemData);
		// ���� ��ư Ȱ��ȭ ��Ȱ��ȭ �ʿ�
		if (OV_Selected->GetVisibility() == ESlateVisibility::Visible)
		{
			OV_Selected->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			OV_Selected->SetVisibility(ESlateVisibility::Visible);
		}
	}
	*/
}

void UB2DynItemIcon_SealBoxPreview::AskForUnlockItemPopupCommon()
{
	/*
	FString PopupStringKey = TEXT("Chatting_PublicNoti_GetItemFromFactory_Forge");	//�ܼ��� ���� �̸��� ����Ѵٴ� �������� ȹ�� �˸��� �ִ°� ���� ����ϰ� �ִ�.

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_IngredientUnmountUnlockGuide")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, PopupStringKey)),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]() {

		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
		FPCItemContainer &ItemContainer = CachedCharacterStore->GetUserAllItems();

		if (NativeItemData.bIsLocked)
		{
			LobbyReqToggleItemLockClass<int64, bool>::GetInstance().Signal(NativeItemData.InstanceUID, false);
			this->NativeItemData.bIsLocked = false;
			this->OV_LockedImages->SetVisibility(ESlateVisibility::Hidden);
		}
		if (LobbyInven->IsContainsAllPreset(NativeItemData))
		{
			BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_Gladiator);
			BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_Assassin);
			BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_Wizard);
			BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_Fighter);
			BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_End);

			TArray<int64> ReqIDs;
			ReqIDs.Add(this->NativeItemData.InstanceUID);

			int32 PresetID = ItemContainer.GetContainsItemPresetID(IntToPCClass(NativeItemData.AllowedPCClass), NativeItemData.InstanceUID);
			LobbyReqUnEquipItemsOtherPresetClass<int32, const TArray<int64>&>::GetInstance().Signal(PresetID, ReqIDs);

			ItemContainer.UnEquipItemsPresets(ReqIDs);

			this->NativeItemData.bCurrentlyEquipped = false;
			this->P_EquipInfo->SetVisibility(ESlateVisibility::Hidden);
		}

	}), FMsgPopupOnClick::CreateLambda([]() {}), true
		);
		*/

	//�׳� �� �������� ������ �ҷ����� �뵵.
	//���� ���ߵ��� ����
}

void UB2DynItemIcon_SealBoxPreview::FinishedResultAnimationThingy()
{
	resultPannel->ContinueResultShowing();
}

FText UB2DynItemIcon_SealBoxPreview::GetItemLevelText() const
{
	if (!IsEssenceItem(NativeItemData) &&
		NativeItemData.InventoryType != EItemInvenType::EIIVT_Consumables &&
		!IsCostumeItem(NativeItemData) && !IsAnvilItem(NativeItemData))
	{	// ��ȭ ��� �������� ���Ƽ� �����ϱ� ���� ǥ�ø� ���� �ʴ´�. ���� �������� ������ ���� �ɷ� ġ��..		
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::FromString(FString::FromInt(NativeItemData.Level)));
	}

	return FText();
}

FText UB2DynItemIcon_SealBoxPreview::GetItemAmountText()
{
	if (!IsEssenceItem(NativeItemData) &&
		NativeItemData.InventoryType == EItemInvenType::EIIVT_Consumables && !IsAnvilItem(NativeItemData))
	{ // ��ȭ ��� �������� ���Ƽ� �����ϱ� ���� ǥ�ø� ���� �ʴ´�. ���� �������� ������ ���� �ɷ� ġ��..		
		if (NativeItemData.ConsumingAmount >= 10000)
			return FText::FromString(TEXT("9999+"));

		return FText::AsNumber(NativeItemData.ConsumingAmount);
	}

	return FText();
}

FText UB2DynItemIcon_SealBoxPreview::GetItemAmountText(int32 minValue, int32 maxValue)
{
	if (!IsEssenceItem(NativeItemData) &&
		NativeItemData.InventoryType == EItemInvenType::EIIVT_Consumables && !IsAnvilItem(NativeItemData))
	{
		return FText::FromString(FString::FromInt(minValue) + " ~ " + FString::FromInt(maxValue));
	}

	return FText();
}

FText UB2DynItemIcon_SealBoxPreview::GetItemEnhanceLevelText() const
{
	if (IsEquipmentItem(NativeItemData) && (NativeItemData.EnhanceLevel != 0))
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel")), FText::FromString(FString::FromInt(NativeItemData.EnhanceLevel)));
	}

	return FText();
}

void UB2DynItemIcon_SealBoxPreview::ShowJustEmptySlot()
{
	if (CP_Main.IsValid())
	{
		CP_Main->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (CP_BackGroundOnly.IsValid())
	{
		CP_BackGroundOnly->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

FText UB2DynItemIcon_SealBoxPreview::SetItemEnhanceLevelText(int32 enhanceLevel) const
{
	if (enhanceLevel != 0)
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel")), FText::FromString(FString::FromInt(enhanceLevel)));
	}

	return FText();
}

FText UB2DynItemIcon_SealBoxPreview::SetItemLevelText(int32 itemLevel) const
{
	if (!IsEssenceItem(NativeItemData) &&
		NativeItemData.InventoryType != EItemInvenType::EIIVT_Consumables &&
		!IsCostumeItem(NativeItemData) && !IsAnvilItem(NativeItemData))
	{	// ��ȭ ��� �������� ���Ƽ� �����ϱ� ���� ǥ�ø� ���� �ʴ´�. ���� �������� ������ ���� �ɷ� ġ��..		
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::FromString(FString::FromInt(itemLevel)));
	}

	return FText();
}