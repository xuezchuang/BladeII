// Fill out your copyright notice in the Description page of Project Settings.


#include "B2DynItemIcon_ItemForge.h"
//#include "BladeII.h"
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
#include "CharacterDataStore.h"
#include "CommonStruct.h"




UB2DynItemIcon_ItemForge::UB2DynItemIcon_ItemForge
(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	CachedCharacterStore = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	bIsNativeItemLocked = false;
}


void UB2DynItemIcon_ItemForge::CacheAssets()
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

	if (ItemIconUseType == EItemForgeItemIconUsage::EIFI_InIvenItem)
	{
		BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2DynItemIcon_ItemForge::OnClickBTNArea);
	}
	else
	{
		
	}


	
	//BTN_Area->OnClicked.AddDynamic(this, &UB2DynItemIcon_ItemForge::OnClickBTNArea);
	GET_SLOT(UImage, BackgroundImage);
	GET_SLOT(UImage, IMG_CanNotSelect);
	
	if (ItemIconUseType == EItemForgeItemIconUsage::EIFI_TargetItem)
	{
		//ItemLevelText->SetVisibility(ESlateVisibility::Hidden);
		//ItemEnhanceLevelText->SetVisibility(ESlateVisibility::Hidden);
		ItemQualityBGImage->SetVisibility(ESlateVisibility::Hidden);
		//ItemQualityTextBlock->SetVisibility(ESlateVisibility::Hidden);
		OV_ItemQualitySet->SetVisibility(ESlateVisibility::Hidden);
		TB_Preset->SetVisibility(ESlateVisibility::Hidden);
		IMG_EquippedCheck->SetVisibility(ESlateVisibility::Hidden);
		P_EquipInfo->SetVisibility(ESlateVisibility::Hidden);

		if (NativeItemData.InventoryType != EItemInvenType::EIIVT_Consumables)
		{
			ItemAmountText->SetVisibility(ESlateVisibility::Hidden);
		}

		ItemLockedStateImage_Store->SetVisibility(ESlateVisibility::Hidden);
		ItemLockedStateImage_Detail->SetVisibility(ESlateVisibility::Hidden);


		
	}


	GET_SLOT(UCanvasPanel, CP_Main);
	GET_SLOT(UCanvasPanel, CP_BackGroundOnly);
}

void UB2DynItemIcon_ItemForge::UpdateItemData(const FB2Item& InItem)
{
	Super::UpdateItemData(InItem);

	//UE_LOG(LogTemp, Log, TEXT("UpdateItemdata"));

	IsContainsPreset = IsContainsCurrentPreset(InItem);
	UB2ItemInfo* AllItemInfo = StaticFindItemInfo();
	FSingleItemInfoData* ThisItemInfo = AllItemInfo ? AllItemInfo->GetInfoData(NativeItemData.ItemRefID) : NULL;
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();

	SetBackgroundImageFromGrade((IsEquipmentItem(InItem) || IsEssenceItem(InItem)) ? InItem.StarGrade : 1, (InItem.InventoryType == EItemInvenType::EIIVT_Consumables));

	if (MainIconImage!=nullptr)
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

	if (DisabledSlotImage.IsValid())
		DisabledSlotImage->SetVisibility(GetDisabledSlotImageVisibility());

	if (bIsEquipmentItem)
	{
 		if (P_EquipInfo.IsValid()) {
 			P_EquipInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		//if (IMG_PresetBG.IsValid()) {
		//	IMG_PresetBG->SetVisibility(IsContainsPreset ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		//}
		if (TB_Preset.IsValid())
		{
			TB_Preset->SetText(GetEquipPresetText());
			TB_Preset->SetVisibility(IsContainsPreset ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
		if (IMG_EquippedCheck.IsValid()) {
			IMG_EquippedCheck->SetVisibility(IsContainsPreset ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
			//IMG_CanNotSelect->SetVisibility(IsContainsPreset ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
			//BTN_Area->SetVisibility(IsContainsPreset ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
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

		if (InItem.bIsLocked)
		{
			ItemLockedStateImage_Store->SetVisibility(ESlateVisibility::Visible);
			ItemLockedStateImage_Detail->SetVisibility(ESlateVisibility::Visible);
			bIsNativeItemLocked = InItem.bIsLocked;
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
		UIP_RedDot->SetVisibility(InItem.IsNew ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (ItemIconUseType == EItemForgeItemIconUsage::EIFI_TargetItem)
	{
 		ItemLevelText->SetVisibility(ESlateVisibility::Hidden);
 		ItemEnhanceLevelText->SetVisibility(ESlateVisibility::Hidden);
		ItemQualityBGImage->SetVisibility(ESlateVisibility::Hidden);
		ItemQualityTextBlock->SetVisibility(ESlateVisibility::Hidden);
		ItemLockedStateImage_Store->SetVisibility(ESlateVisibility::Hidden);
		ItemLockedStateImage_Detail->SetVisibility(ESlateVisibility::Hidden);

		if (requiredEnhanceLevel > 0)
		{
			ItemEnhanceLevelText->SetVisibility(ESlateVisibility::HitTestInvisible);
			ItemEnhanceLevelText->SetText(SetItemEnhanceLevelText(requiredEnhanceLevel));
		}
		if (requiredItemLevel > 0)
		{
			ItemLevelText->SetVisibility(ESlateVisibility::HitTestInvisible);
			ItemLevelText->SetText(SetItemLevelText(requiredItemLevel));
		}
	}

	//���������� New���¸� �ٲ��ݴϴ�
	FB2Item InventoryItem;
	if (UB2LobbyInventory::FindStoredItem(InventoryItem, InItem.InstanceUID))
		InventoryItem.SetNewItemState();



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
	}
}

void UB2DynItemIcon_ItemForge::UpdateTargetItemData(int32 RefId,int32 Count)
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
		StarGradePart->UpdateStar(StarGrade,requiredItemLevel>dummyItem.ItemNormalMaxLevel ? true:false);	//������ ��ű��� ��쵵 �׳� 30 �Ѿ�� �ʿ� ���� ���� ��.
	}

	if (Count > 1)
	{
		ItemAmountText->SetText(FText::AsNumber(Count));
		ItemAmountText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ItemAmountText->SetVisibility(ESlateVisibility::Hidden);
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

	if (requiredEnhanceLevel > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("enhancedLevel if in %d"), requiredEnhanceLevel);
		ItemEnhanceLevelText->SetVisibility(ESlateVisibility::HitTestInvisible);
		ItemEnhanceLevelText->SetText(SetItemEnhanceLevelText(requiredEnhanceLevel));
	}
	else
	{
		ItemEnhanceLevelText->SetVisibility(ESlateVisibility::Hidden);
	}
	if (requiredItemLevel > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Level if in %d"), requiredItemLevel);
		ItemLevelText->SetVisibility(ESlateVisibility::HitTestInvisible);
		ItemLevelText->SetText(SetItemLevelText(requiredItemLevel));
	}
	else
	{
		ItemLevelText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2DynItemIcon_ItemForge::SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables)
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

	//if (P_StarGradeDisplayPanel.IsValid())
	//	P_StarGradeDisplayPanel->SetVisibility(bForConsumables ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

}

bool UB2DynItemIcon_ItemForge::IsContainsCurrentPreset(const FB2Item& InItem)
{
	if (CachedCharacterStore == nullptr)
		return false;

	FPCItemContainer &ItemContainer = CachedCharacterStore->GetUserAllItems();
	return ItemContainer.IsInItemPresets(IntToPCClass(InItem.AllowedPCClass), InItem.InstanceUID);
}

ESlateVisibility UB2DynItemIcon_ItemForge::GetDisabledSlotImageVisibility() const
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

void UB2DynItemIcon_ItemForge::OnClickedSenderBTNArea()
{
	Super::OnClickedSenderBTNArea();
}

void UB2DynItemIcon_ItemForge::OnPressedSenderBTNArea()
{
	Super::OnPressedSenderBTNArea();
}

void UB2DynItemIcon_ItemForge::OnReleasedSenderBTNArea()
{
	Super::OnReleasedSenderBTNArea();
}

void UB2DynItemIcon_ItemForge::InitItemForgeSlot(EItemForgeSlot EIFS, EItemForgeItemIconUsage UseType, int32 FactoryId, int32 enhanceLevel, int32 itemLevel)
{
	SetMaterialSlotType(EIFS);
	SetUseType(UseType);
	FactoryID = FactoryId;

	requiredEnhanceLevel = enhanceLevel;
	requiredItemLevel = itemLevel;
}

void UB2DynItemIcon_ItemForge::OnClickBTNArea()
{
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;

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


	
	
}

void UB2DynItemIcon_ItemForge::AskForUnlockItemPopupCommon( )
{ // �������� ����ְų� �ٸ� �����¿� �����Ǿ� ���� ��� ����� �˾�
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
		if(LobbyInven->IsContainsAllPreset(NativeItemData))
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

	}), FMsgPopupOnClick::CreateLambda([](){}),true
	);
}

FText UB2DynItemIcon_ItemForge::GetEquipPresetText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Equiped_Preset"));
}


FText UB2DynItemIcon_ItemForge::GetItemLevelText() const
{
	if (!IsEssenceItem(NativeItemData) &&
		NativeItemData.InventoryType != EItemInvenType::EIIVT_Consumables && 
		!IsCostumeItem(NativeItemData) && !IsAnvilItem(NativeItemData))
	{	// ��ȭ ��� �������� ���Ƽ� �����ϱ� ���� ǥ�ø� ���� �ʴ´�. ���� �������� ������ ���� �ɷ� ġ��..		
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::FromString(FString::FromInt(NativeItemData.Level)));
	}

	return FText();
}

FText UB2DynItemIcon_ItemForge::GetItemAmountText() const
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

FText UB2DynItemIcon_ItemForge::GetItemEnhanceLevelText() const
{
	if (IsEquipmentItem(NativeItemData) && (NativeItemData.EnhanceLevel != 0))
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel")), FText::FromString(FString::FromInt(NativeItemData.EnhanceLevel)));
	}

	return FText();
}

void UB2DynItemIcon_ItemForge::ShowJustEmptySlot()
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

FText UB2DynItemIcon_ItemForge::SetItemEnhanceLevelText(int32 enhanceLevel) const
{
	if (enhanceLevel != 0)
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel")), FText::FromString(FString::FromInt(enhanceLevel)));
	}

	return FText();
}

FText UB2DynItemIcon_ItemForge::SetItemLevelText(int32 itemLevel) const
{
	if (!IsEssenceItem(NativeItemData) &&
		NativeItemData.InventoryType != EItemInvenType::EIIVT_Consumables &&
		!IsCostumeItem(NativeItemData) && !IsAnvilItem(NativeItemData))
	{	// ��ȭ ��� �������� ���Ƽ� �����ϱ� ���� ǥ�ø� ���� �ʴ´�. ���� �������� ������ ���� �ɷ� ġ��..		
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::FromString(FString::FromInt(itemLevel)));
	}

	return FText();
}