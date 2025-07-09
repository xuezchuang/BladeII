// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2Inventory.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "BladeIIGameMode.h"
#include "Event.h"
#include "B2LobbyUI_InventoryMain.h"
#include "B2ItemInfo.h"

#include "B2UIStarGradePart.h"
#include "BladeIIGameImpl.h"

#if !UE_BUILD_SHIPPING
extern bool gIsDevUI;
#endif //UE_BUILD_SHIPPING

UB2DynItemIcon_LobbyInven::UB2DynItemIcon_LobbyInven(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IconUsageType = ELobbyInvenItemIconUsage::EEIIT_LobbyInven;

	ItemLevelTextBlockNRef = NULL;
	ItemAmountTextBlockNRef = NULL;
	ItemEnhanceLevelTextBlockNRef = NULL;
	OV_ItemQualitySet_NRef = NULL;
	ItemQualityTextBlockNRef = NULL;
	ItemQualityBGImageNRef = NULL;
	EnhanceIngredSelectionImageNRef = NULL;
	EnhanceIngredCheckImageNRef = NULL;
	StatIncSignImageNRef = NULL;
	OV_StatIncAmountSet_NRef = NULL;
	TB_StatIncAmountNRef = NULL;
	ItemLockedImageNRef_Store = NULL;
	ItemLockedImageNRef_Detail = NULL;
	

	CachedCharacterStore = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
}

void UB2DynItemIcon_LobbyInven::CacheAssets()
{
	Super::CacheAssets();

	// � �� �������Ʈ�� ����Ƽ�� ���۷��� ������ �ϴµ� �� ���� ����ϰ� ���ο� ����� ȥ���ϴ� ��. ���� �׶� �̷� ����� ������ ���� �� ��Ÿ �ٸ� ������ ����.
	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UB2UIWidgetBase, UIP_RedDot);
	GET_SLOT(UPanelWidget, P_EquipInfo);
	GET_SLOT(UImage, IMG_PresetBG);
	GET_SLOT(UTextBlock, TB_Preset);
	GET_SLOT(UImage, IMG_EquippedCheck);

	GET_SLOT(UPanelWidget, P_DevUI); // �̰� Ȥ�� ���� Ȯ���� ����� ���� �����ǿ����� ĳ��
	GET_SLOT(UPanelWidget, P_StarGradeDisplayPanel);
#if !UE_BUILD_SHIPPING
	GET_SLOT(UTextBlock, TB_ItemRefId_Dev);
	GET_SLOT(UTextBlock, TB_ItemUId_Dev);
	GET_SLOT(UB2Button, BTN_ItemRefId_Dev);
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemRefId_Dev, &UB2DynItemIcon_LobbyInven::OnPressedBTNItemRefid);
	GET_SLOT(UB2Button, BTN_ItemUId_Dev);
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemUId_Dev, &UB2DynItemIcon_LobbyInven::OnPressedBTNItemUid);
#endif
	
	if (BTN_Area.IsValid())
	{
		// �Ʒ�ó�� ���ε带 ���� �ʴ´�. �츮�� ������ NativeOn** �� �޴� ��. �ű⼭ On**SenderBTNArea �� ���� �� ����.
		//BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2DynItemIcon_LobbyInven::OnClickedBTNArea);
		//BIND_PRESS_FUNC_TO_BTN(BTN_Area, &UB2DynItemIcon_LobbyInven::OnPressedBTNArea);
		//BIND_RELEASE_FUNC_TO_BTN(BTN_Area, &UB2DynItemIcon_LobbyInven::OnReleasedBTNArea);
		BTN_Area->SetVisibility(ESlateVisibility::HitTestInvisible); 
	}
	this->SetVisibility(ESlateVisibility::Visible); // BTN_Area �� ����ϰ� ���� �Է��� �޵���. NativeOn** ����� �������� ��.
	GET_SLOT(UB2UIStarGradePart, StarGradePart);
	if (StarGradePart.IsValid())
	{
		StarGradePart->Init();
	}
}

void UB2DynItemIcon_LobbyInven::UpdateItemData(const FB2Item& InItem)
{
	Super::UpdateItemData(InItem);

	IsContainsPreset = IsContainsCurrentPreset(InItem);

	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();

	const bool bIsEquipmentItem = IsEquipmentItem(InItem);
	const bool bIsEssenceItem = IsEssenceItem(InItem);
	const bool bIsCostumeItem = IsCostumeItem(InItem);
	const bool bIsAnvilItem = IsAnvilItem(InItem);

	// ��ġ�ص� widget ���� native reference ���� valid �� ��� ������Ʈ.
	// ���� NativeItemData ��ü�� �ٲ��� �ʴ��� ���� ���� ��� �ٲ� �� �� ���� ��Ȳ���� ������Ʈ�� �ʿ��� ��. �Ź� ���� �����Ѵٸ� ������ �ʿ� ��������
	if (ItemLevelTextBlockNRef)
		ItemLevelTextBlockNRef->SetText(GetItemLevelText());

	if (ItemAmountTextBlockNRef)
		ItemAmountTextBlockNRef->SetText(GetItemAmountText());

#if !UE_BUILD_SHIPPING
	if (P_DevUI.IsValid())
		P_DevUI->SetVisibility(ShouldShowDevUI() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (ShouldShowDevUI())
	{
		if (TB_ItemRefId_Dev.IsValid())
			TB_ItemRefId_Dev->SetText(GetItemRefId());

		if (TB_ItemUId_Dev.IsValid())
			TB_ItemUId_Dev->SetText(GetItemUId());
	}
#else
	if (P_DevUI.IsValid()) {
		P_DevUI->SetVisibility(ESlateVisibility::Collapsed);
	}
#endif

	if (ItemEnhanceLevelTextBlockNRef)
		ItemEnhanceLevelTextBlockNRef->SetText(GetItemEnhanceLevelText());
	
	if (OV_ItemQualitySet_NRef) {
		OV_ItemQualitySet_NRef->SetVisibility(bIsEquipmentItem ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	
	if (ItemQualityBGImageNRef)
	{
		if (bIsEquipmentItem)
		{
			ItemQualityBGImageNRef->SetVisibility(ESlateVisibility::HitTestInvisible);
			UMaterialInterface* QualityBGMtrl = ItemMiscInfo ? ItemMiscInfo->GetItemQualityBGMtrl(InItem.Quality) : NULL;
			if (QualityBGMtrl)
				ItemQualityBGImageNRef->SetBrushFromMaterial(QualityBGMtrl);
		}
		else
		{
			ItemQualityBGImageNRef->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (ItemQualityTextBlockNRef)
	{
		ItemQualityTextBlockNRef->SetVisibility(bIsEquipmentItem ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		ItemQualityTextBlockNRef->SetText(GetLOCTextOfItemQuality(InItem.Quality));
	}
	
	if (EnhanceIngredSelectionImageNRef)	
		EnhanceIngredSelectionImageNRef->SetVisibility(GetEnhanceIngredSelectionImageVisibility());
	
	if (EnhanceIngredCheckImageNRef)	
		EnhanceIngredCheckImageNRef->SetVisibility(GetEnhanceIngredSelectionImageVisibility());
	
	SetupStatIncInfo();

	if (ItemLockedImageNRef_Store)	
		ItemLockedImageNRef_Store->SetVisibility(GetItemLockedImageVisibility_Store());
	
	if (ItemLockedImageNRef_Detail)	
		ItemLockedImageNRef_Detail->SetVisibility(GetItemLockedImageVisibility_Detail());

	if (IMG_DisabledSlot)
		IMG_DisabledSlot->SetVisibility(GetDisabledSlotImageVisibility());

	if (bIsEquipmentItem &&
		(IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven || IconUsageType == ELobbyInvenItemIconUsage::EEIIT_ItemDetailWindow || IconUsageType == ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget)
		)
	{
		if (P_EquipInfo.IsValid()) {
			P_EquipInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		if (IMG_PresetBG.IsValid()) {
			IMG_PresetBG->SetVisibility(IsContainsPreset ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
		if (TB_Preset.IsValid())
		{
			TB_Preset->SetText(GetEquipPresetText());
			TB_Preset->SetVisibility(IsContainsPreset ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
		if (IMG_EquippedCheck.IsValid()) {
			IMG_EquippedCheck->SetVisibility(IsContainsPreset ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
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

	//���������� New���¸� �ٲ��ݴϴ�
	FB2Item InventoryItem;
	if (UB2LobbyInventory::FindStoredItem(InventoryItem, InItem.InstanceUID))
		InventoryItem.SetNewItemState();
}

bool UB2DynItemIcon_LobbyInven::IsContainsCurrentPreset(const FB2Item& InItem)
{
	if (CachedCharacterStore == nullptr)
		return false;

	FPCItemContainer &ItemContainer = CachedCharacterStore->GetUserAllItems();
	return ItemContainer.IsInItemPresets(IntToPCClass(InItem.AllowedPCClass), InItem.InstanceUID);
}

void UB2DynItemIcon_LobbyInven::SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (BackgroundImageNRef && ItemMiscInfo)
	{
		// ������ ���� Simple �� ����ϴ� �� �ְ� �ƴ� �� ����.
		UMaterialInterface* BGMtrl = ItemMiscInfo->GetItemIconBGMtrl(InItemGrade, 
			(IconUsageType == ELobbyInvenItemIconUsage::EEIIT_ComposeOrSurpassIngredient) ? true : false,
			bForConsumables
		);
		if (BGMtrl)
		{	
			BackgroundImageNRef->SetBrushFromMaterial(BGMtrl);
		}
	}

	if (P_StarGradeDisplayPanel.IsValid())
		P_StarGradeDisplayPanel->SetVisibility(bForConsumables ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void UB2DynItemIcon_LobbyInven::SetIconUsageType(ELobbyInvenItemIconUsage InUsageType)
{ 
	IconUsageType = InUsageType;

	if (BTN_Area.IsValid())
		BTN_Area->SetIsEnabled(IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven); // �ϴ��� ���� ������ �信���� ���.

	// ������ �����Ͱ� �̹� �� �־��ٸ� ǥ�õ� �����ؾ� �� ��. �Ϲ����� ���� UpdateItemData �� �ϱ� ���� SetIconUsageType �� ���� �ϴ� ��.
	if (NativeItemData.ItemRefID > 0)
		UpdateItemData(NativeItemData);

	if (InUsageType == ELobbyInvenItemIconUsage::EEIIT_AutoEnhanceResult ||
		InUsageType == ELobbyInvenItemIconUsage::EEIIT_MailAllReceiveResult)
	{ // �ڵ� ��ȭ ���â������ ������ �ڽ��� �Է� ���� �ʿ� ���� �ڽ��� ��ġ�� ��׶��� ��ũ�ѹڽ��� ��ũ�Ѹ� �Ǹ� ��.
		// ���� �̱⿡���� �����ϰ� �۵� �ؾ��� ������ ����
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

FText UB2DynItemIcon_LobbyInven::GetEquipPresetText() const
{
	return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Equiped_Preset"));
}

FText UB2DynItemIcon_LobbyInven::GetItemLevelText() const
{
	if (IconUsageType != ELobbyInvenItemIconUsage::EEIIT_ItemLevelupMenuIngredient && !IsEssenceItem(NativeItemData) && 
		NativeItemData.InventoryType != EItemInvenType::EIIVT_Consumables && !IsCostumeItem(NativeItemData) && !IsAnvilItem(NativeItemData))
	{	// ��ȭ ��� �������� ���Ƽ� �����ϱ� ���� ǥ�ø� ���� �ʴ´�. ���� �������� ������ ���� �ɷ� ġ��..		
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::FromString(FString::FromInt(NativeItemData.Level)));
	}

	return FText();
}

FText UB2DynItemIcon_LobbyInven::GetItemAmountText() const
{
	if (IconUsageType != ELobbyInvenItemIconUsage::EEIIT_ItemLevelupMenuIngredient && !IsEssenceItem(NativeItemData) && 
		NativeItemData.InventoryType == EItemInvenType::EIIVT_Consumables && !IsAnvilItem(NativeItemData))
	{ // ��ȭ ��� �������� ���Ƽ� �����ϱ� ���� ǥ�ø� ���� �ʴ´�. ���� �������� ������ ���� �ɷ� ġ��..		
		if (NativeItemData.ConsumingAmount >= 10000)
			return FText::FromString(TEXT("9999+"));

		return FText::AsNumber(NativeItemData.ConsumingAmount);
	}

	return FText();
}

FText UB2DynItemIcon_LobbyInven::GetItemEnhanceLevelText() const
{
	if (IconUsageType != ELobbyInvenItemIconUsage::EEIIT_ItemLevelupMenuIngredient && IsEquipmentItem(NativeItemData) && (NativeItemData.EnhanceLevel != 0))
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel")), FText::FromString(FString::FromInt(NativeItemData.EnhanceLevel)));
	}

	return FText();
}

FText UB2DynItemIcon_LobbyInven::GetItemRefId() const
{
#if !UE_BUILD_SHIPPING
	return FText::FromString(FString::Printf(TEXT("RefId: %d"), NativeItemData.ItemRefID));
#endif 

	return FText();
}

FText UB2DynItemIcon_LobbyInven::GetItemUId() const
{
#if !UE_BUILD_SHIPPING
	return FText::FromString(FString::Printf(TEXT("UId:%I64d"), NativeItemData.InstanceUID));
#endif 

	return FText();
}

void UB2DynItemIcon_LobbyInven::SetSelectIcon(bool IsSelected)
{
	ESlateVisibility Visible = IsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;

	if (EnhanceIngredSelectionImageNRef)
		EnhanceIngredSelectionImageNRef->SetVisibility(Visible);

	if (EnhanceIngredCheckImageNRef)
		EnhanceIngredCheckImageNRef->SetVisibility(Visible);
}

void UB2DynItemIcon_LobbyInven::SetupStatIncInfo()
{
	ESlateVisibility StatIncCommonVisibility = ESlateVisibility::Hidden;
	int32 StatIncAmount = 0;

	// �Ƹ��� ���� ������ â������ ���̰� �� ��.
	if (IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven)
	{
		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;

		if (LobbyInven)
		{
			FB2Item EquippedAtThisPos;
			if (LobbyInven->GetEquippedItemAtPlace(EquippedAtThisPos, NativeItemData.EquipPlace)) // ���� ��ġ�� ������ �� ã�Ƽ� ��.
			{	// True �����̾�� ����� ã�� ��. ��ű��� PrimaryPointType �� �ٸ� �� ����.
				if (NativeItemData.PrimaryPointType == EquippedAtThisPos.PrimaryPointType ) 
				{
					StatIncAmount = (int32)(NativeItemData.GetPrimaryPoint() - EquippedAtThisPos.GetPrimaryPoint());
					if (StatIncAmount > 0)
					{
						StatIncCommonVisibility = ESlateVisibility::HitTestInvisible;
					}
				}
			}
		}
	}

	if (StatIncSignImageNRef)
	{
		StatIncSignImageNRef->SetVisibility(StatIncCommonVisibility);
	}
	if (OV_StatIncAmountSet_NRef)
	{
		OV_StatIncAmountSet_NRef->SetVisibility(StatIncCommonVisibility);
	}
	if (TB_StatIncAmountNRef)
	{
		TB_StatIncAmountNRef->SetText(FText::AsNumber(StatIncAmount));
	}
}

ESlateVisibility UB2DynItemIcon_LobbyInven::GetItemLockedImageVisibility_Store() const
{ // ������ ��
	return (NativeItemData.bIsLocked && IsEquipmentItem(NativeItemData) && (IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven)) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
}

ESlateVisibility UB2DynItemIcon_LobbyInven::GetItemLockedImageVisibility_Detail() const
{ // ����â ��
	return (NativeItemData.bIsLocked && IsEquipmentItem(NativeItemData) && (IconUsageType == ELobbyInvenItemIconUsage::EEIIT_ItemDetailWindow || IconUsageType == ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget)) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
}

void UB2DynItemIcon_LobbyInven::OnClickedSenderBTNArea()
{ // ManualScrollBox ����� �Ϻη� �Ҹ��� �������̽�
	Super::OnClickedSenderBTNArea();

	if (IsBtnAreaUsed())
	{
		OnClickedBTNArea(); // ���� Ŭ�� �ڵ鸵
	}
}
void UB2DynItemIcon_LobbyInven::OnPressedSenderBTNArea()
{// ManualScrollBox ����� �Ϻη� �Ҹ��� �������̽�
	Super::OnPressedSenderBTNArea();

	if (IsBtnAreaUsed())
	{
		OnPressedBTNArea(); // ���� Ŭ�� �ڵ鸵
	}
}
void UB2DynItemIcon_LobbyInven::OnReleasedSenderBTNArea()
{// ManualScrollBox ����� �Ϻη� �Ҹ��� �������̽�
	Super::OnReleasedSenderBTNArea();

	if (IsBtnAreaUsed())
	{
		OnReleasedBTNArea(); // ���� Ŭ�� �ڵ鸵
	}
}

bool UB2DynItemIcon_LobbyInven::OnClickBTNForLevelup(UB2LobbyInventory* LobbyInven)
{
	bool bHandledClickSoundPlay = false;

	// ���� ���� ������ �ְ� ������ ������Ʈ�� �ٷ� �Ѵ�.
	if (NativeItemData.bSelectedAsIngred)
	{
		UnSelectForItemLevelupIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	}
	else
	{
		if (NativeItemData.bIsLocked)
		{
			AskForUnlockItemPopupCommon(ELobbyInvenItemOpMode::EIVIOP_Levelup);
		}
		else if (LobbyInven->IsIngredsFull())
		{ // ������ �ȵǰ� �޽��� �Ѹ�
			ABladeIIGameMode::SetHUDCenterMessage(
				FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AlreadySelectedMaxEnhanceIngred"))), FText::AsNumber(LobbyInven->GetItemLevelupIngredLimit())),
				3.0f // �̷� ���������� �ͱ��� �����ϰ� �� �� �ƴ� �� ���� ���Ƿ� ������ �ð��� �ش�.
			);
		}
		else if (LobbyInven->IsMaxItemLevelUpExpectedByCurrentSelection())
		{ // ������ �ȵǰ� �޽��� �Ѹ�
			ABladeIIGameMode::SetHUDCenterMessage(
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AlreadyExpectedMaxEnhanceLevel"))),
				3.0f // �̷� ���������� �ͱ��� �����ϰ� �� �� �ƴ� �� ���� ���Ƿ� ������ �ð��� �ش�.
			);
		}
		else if (NativeItemData.EnhanceLevel > 0)
		{
			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_AskLevelUpAlreadyEnhanceItem"))),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::YesOrNo,
				FMsgPopupOnClick::CreateLambda([this]() {
				SelectForItemLevelupIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
				PlayLevelupIngredSelectedSound();
			})
			);
			bHandledClickSoundPlay = true;
		}
		else if (IsContainsPreset)
		{
			AskForUnEquipItemPopupCommon();

			bHandledClickSoundPlay = true;
		}
		else if (LobbyInven->IsHigherGradeThanEnhanceTarget(NativeItemData))
		{
			// �ٷ� �������� �ʰ� ��� �޽����� ���.
			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_HigherGradeThanEnhanceTarget"))),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::YesOrNo,
				FMsgPopupOnClick::CreateLambda([this]() {
				SelectForItemLevelupIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
				PlayLevelupIngredSelectedSound();
			})
			);
			bHandledClickSoundPlay = true;
		}
		else
		{ // SelectForItemLevelupIngredClass �̺�Ʈ ó���ϴ� �������� ���� Ÿ�ٰ� ���� ����������, �������� ���� ó���� ��. �̰� ���´ٰ� �ؼ� �ٷ� ���õǴ� �� �ƴ�.
			SelectForItemLevelupIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
			PlayLevelupIngredSelectedSound();
			bHandledClickSoundPlay = true;
			//��� ��ư���� ������ �ѹ� Ʃ�丮�� �ñ׳��� üũ�ؾߵǼ� �˾�â�� ������ ���� ȣ��																			
			if (BTN_Area.IsValid())
			{
				BTN_Area->TutorialButtonSignal();
			}
		}
	}
	
	return bHandledClickSoundPlay;
}

bool UB2DynItemIcon_LobbyInven::OnClickBTNForHammer(UB2LobbyInventory* LobbyInven)
{
	bool bHandledClickSoundPlay = false;

	// ���� ���� ������ �ְ� ������ ������Ʈ�� �ٷ� �Ѵ�.
	if (NativeItemData.bSelectedAsIngred)
	{
		UnSelectForItemHammerClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	}
	else
	{
		SelectForItemHammerClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
		PlayLevelupIngredSelectedSound();
		bHandledClickSoundPlay = true;
		//��� ��ư���� ������ �ѹ� Ʃ�丮�� �ñ׳��� üũ�ؾߵǼ� �˾�â�� ������ ���� ȣ��																			
		if (BTN_Area.IsValid())
		{
			BTN_Area->TutorialButtonSignal();
		}

	}

	return bHandledClickSoundPlay;
}

bool UB2DynItemIcon_LobbyInven::OnClickBTNForComposition(UB2LobbyInventory* LobbyInven)
{
	bool bHandledClickSoundPlay = false;

	if (NativeItemData.bSelectedAsIngred)
	{
		UnSelectForComposeIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	}
	else
	{
		if (NativeItemData.bIsLocked)
		{
			AskForUnlockItemPopupCommon(ELobbyInvenItemOpMode::EIVIOP_Composition);
		}
		else if (NativeItemData.EnhanceLevel > 0)
		{
			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_AskCompositionAlreadyEnhanceItem"))),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::YesOrNo,
				FMsgPopupOnClick::CreateLambda([this]() {
				SelectForComposeIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
				PlayComposeIngredSelectedSound();
			})
			);
			bHandledClickSoundPlay = true;
		}
		else if (IsContainsPreset)
		{
			AskForUnEquipItemPopupCommon();

			bHandledClickSoundPlay = true;
		}
		else
		{
			SelectForComposeIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
			PlayComposeIngredSelectedSound();
			bHandledClickSoundPlay = true;
		}
	}

	return bHandledClickSoundPlay;
}

bool UB2DynItemIcon_LobbyInven::OnClickBTNForSurpass(UB2LobbyInventory* LobbyInven)
{
	bool bHandledClickSoundPlay = false;

	if (NativeItemData.bSelectedAsIngred)
	{
		UnSelectForSurpassIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	}
	else
	{
		if (NativeItemData.bIsLocked)
		{
			AskForUnlockItemPopupCommon(ELobbyInvenItemOpMode::EIVIOP_Surpass);
		}
		else if (NativeItemData.EnhanceLevel > 0)
		{
			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_AskLevelUpAlreadySurpassItem"))),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::YesOrNo,
				FMsgPopupOnClick::CreateLambda([this]() {
				SelectForSurpassIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
				PlaySurpassIngredSelectedSound();
			})
			);
			bHandledClickSoundPlay = true;
		}
		else if (IsContainsPreset)
		{
			AskForUnEquipItemPopupCommon();

			bHandledClickSoundPlay = true;
		}
		else
		{
			SelectForSurpassIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
			PlaySurpassIngredSelectedSound();
			bHandledClickSoundPlay = true;
		}
	}

	return bHandledClickSoundPlay;
}

bool UB2DynItemIcon_LobbyInven::OnClickBTNForEnhance(UB2LobbyInventory* LobbyInven)
{
	bool bHandledClickSoundPlay = false;

	if (NativeItemData.bSelectedAsIngred)
	{
		UnSelectForEnhanceIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	}
	else
	{
		if (NativeItemData.bIsLocked)
		{
			AskForUnlockItemPopupCommon(ELobbyInvenItemOpMode::EIVIOP_Enhancement);
		}
		else if (IsContainsPreset)
		{
			AskForUnEquipItemPopupCommon();

			bHandledClickSoundPlay = true;
		}
		else
		{
			SelectForEnhanceIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
			PlayEnhanceIngredSelectedSound();
			bHandledClickSoundPlay = true;
		}
	}
	return bHandledClickSoundPlay;
}

bool UB2DynItemIcon_LobbyInven::OnClickBTNForEnhanceChange(UB2LobbyInventory* LobbyInven)
{
	bool bHandledClickSoundPlay = false;

	if (NativeItemData.bSelectedAsIngred)
	{
		UnSelectForEnhanceChangeIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	}
	else
	{
		if (NativeItemData.bIsLocked)
		{
			AskForUnlockItemPopupCommon(ELobbyInvenItemOpMode::EIVIOP_EnhancementChange);
		}
		//else if (NativeItemData.EnhanceLevel > 0)
		//{
		//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		//		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
		//		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_AskLevelUpAlreadySurpassItem"))),
		//		0.f,
		//		true,
		//		true,
		//		EUIMsgPopupButtonGroup::YesOrNo,
		//		FMsgPopupOnClick::CreateLambda([this]() {
		//		SelectForSurpassIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
		//		PlaySurpassIngredSelectedSound();
		//	})
		//	);
		//	bHandledClickSoundPlay = true;
		//}
		else if (IsContainsPreset)
		{
			AskForUnEquipItemPopupCommon();

			bHandledClickSoundPlay = true;
		}
		else
		{
			FB2Item TargetItem;
			// �� ������ Inventory �� TargetItemID �� ���õǾ� �־�� ��. (UB2LobbyInventory::EnterItemOpMode)
			if (LobbyInven->GetItemOPTargetItemData(TargetItem) && LobbyInven->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_EnhancementChange)
			{
				// ��ȭ ������ ������
				if (TargetItem.EnhanceLevel == NativeItemData.EnhanceLevel)
				{
					UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
						BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
						BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EnhancementExchange_Guide_4")),
						0.f,
						true,
						true,
						EUIMsgPopupButtonGroup::Confirm
					);
				}
				else
				{
					SelectForEnhanceChangeIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
					PlaySurpassIngredSelectedSound();
					bHandledClickSoundPlay = true;
				}
			}
		}
	}

	return bHandledClickSoundPlay;
}

void UB2DynItemIcon_LobbyInven::OnClickedBTNArea()
{
	if (IconUsageType != ELobbyInvenItemIconUsage::EEIIT_LobbyInven) // �̹� ������ â�� �ƴϸ� ÷���� ������ ���ƾ� ����
	{
		return;
	}

	bool bHandledClickSoundPlay = false; // ������ false �̸� �⺻ ���� �÷���

	// ���� NativeItemData �� ����ؼ� ���� ����â �˾�... ���� �ƴ϶� ��ȭ ����� ���� ������ �ؾ�.
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;

	if (LobbyInven && LobbyInven->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_Levelup)
	{
		bHandledClickSoundPlay = OnClickBTNForLevelup(LobbyInven);
	}
	else if (LobbyInven && LobbyInven->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_Composition)
	{
		bHandledClickSoundPlay = OnClickBTNForComposition(LobbyInven);
	}
	else if (LobbyInven && LobbyInven->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_Surpass)
	{
		bHandledClickSoundPlay = OnClickBTNForSurpass(LobbyInven);
	}
	else if (LobbyInven && LobbyInven->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_Enhancement)
	{
		bHandledClickSoundPlay = OnClickBTNForEnhance(LobbyInven);
	}
	else if (LobbyInven && LobbyInven->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_EnhancementChange)
	{
		bHandledClickSoundPlay = OnClickBTNForEnhanceChange(LobbyInven);
	}
	// �±޿����� ��� �������̶� �� ���µ� �׳� �⺻���� detail â ���� �ű⼭ �� ���������� �±�â�� �ٽ� ��� �� �ִ�.
	else
	{
		CreateLobbyItemDetailPopupCommon(GetOwningPlayer(), NativeItemData, false); // Inventory (stored) ������ detail â ����

		//��� ��ư���� ������ �ѹ� Ʃ�丮�� �ñ׳��� üũ�ؾߵǼ� �˾�â�� ������ ���� ȣ��		
		if(BTN_Area.IsValid())
		{ BTN_Area->TutorialButtonSignal(); }		
	}

	if (!bHandledClickSoundPlay)
	{
		PlayCommonIconClickSound();
	}

	OnClickedBTNArea_BP(); // �������Ʈ �ڵ鸵

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2DynItemIcon_LobbyInven::AskForUnEquipItemPopupCommon()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_ContainsItemOtherPreset"))),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]() {
			if (CachedCharacterStore == nullptr)
				return;

			TArray<int64> ReqIDs;
			ReqIDs.Add(NativeItemData.InstanceUID);

			FPCItemContainer &ItemContainer = CachedCharacterStore->GetUserAllItems();

			int32 PresetID = ItemContainer.GetContainsItemPresetID(IntToPCClass(NativeItemData.AllowedPCClass), NativeItemData.InstanceUID);

			LobbyReqUnEquipItemsOtherPresetClass<int32, const TArray<int64>&>::GetInstance().Signal(PresetID, ReqIDs);
			PlayLevelupIngredSelectedSound();
		})
	);
}

void UB2DynItemIcon_LobbyInven::AskForUnlockItemPopupCommon(ELobbyInvenItemOpMode InModeForUnlock)
{ // ��ȭ �ռ� �ʿ� ���� ����ϱ� ���� ���� Ŭ�� �� ��� �����ϰڴ°� �ϴ� �˾�.
	FString PopupStringKey;
	switch (InModeForUnlock)
	{
	case ELobbyInvenItemOpMode::EIVIOP_Levelup:		PopupStringKey = TEXT("LobbyInvenText_ItemLevelup"); break;
	case ELobbyInvenItemOpMode::EIVIOP_Composition: PopupStringKey = TEXT("LobbyInvenText_Composition_Short"); break;
	case ELobbyInvenItemOpMode::EIVIOP_Surpass:		PopupStringKey = TEXT("LobbyInvenText_Surpass_Short"); break;
	case ELobbyInvenItemOpMode::EIVIOP_Enhancement:	PopupStringKey = TEXT("LobbyInvenText_ItemEnhancement"); break;
	default : PopupStringKey = TEXT("LobbyInvenText_ItemLevelup"); break;
	}

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_AskForUnlockForItemOpIngred")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, PopupStringKey)),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this](){
			LobbyReqToggleItemLockClass<int64, bool>::GetInstance().Signal(NativeItemData.InstanceUID, false);
		})
		);
}

void UB2DynItemIcon_LobbyInven::OnPressedBTNArea()
{
	OnPressedBTNArea_BP(); // �������Ʈ �ڵ鸵
}

void UB2DynItemIcon_LobbyInven::OnReleasedBTNArea()
{
	OnReleasedBTNArea_BP(); // �������Ʈ �ڵ鸵
}

void UB2DynItemIcon_LobbyInven::OnPressedBTNItemUid()
{
#if !UE_BUILD_SHIPPING
	//FPlatformMisc::ClipboardCopy(*FString::Printf(TEXT("%I64d"), NativeItemData.InstanceUID));
	//UE_LOG(LogBladeII, Log, TEXT("ClipboardCopy Success! ItemUID : %I64d"), NativeItemData.InstanceUID);
#endif
}

void UB2DynItemIcon_LobbyInven::OnPressedBTNItemRefid()
{
#if !UE_BUILD_SHIPPING
	//FPlatformMisc::ClipboardCopy(*FString::Printf(TEXT("%d"), NativeItemData.ItemRefID));
	//UE_LOG(LogBladeII, Log, TEXT("ClipboardCopy Success! ItemRefID : %d"), NativeItemData.ItemRefID);
#endif
}

ESlateVisibility UB2DynItemIcon_LobbyInven::GetEnhanceIngredSelectionImageVisibility() const
{
	return (NativeItemData.bSelectedAsIngred && IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
}

bool UB2DynItemIcon_LobbyInven::ShouldShowDevUI() const
{
#if !UE_BUILD_SHIPPING
	return gIsDevUI ? true : false;
#endif
	return false;
}

ESlateVisibility UB2DynItemIcon_LobbyInven::GetDisabledSlotImageVisibility() const
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

void UB2DynItemIcon_LobbyInven::SetShowOverlayInfoOnly(bool IsVisibleBGImg /*= false*/, bool IsVisibleGrade /*= false*/)
{
	// �̰� ��ü�� �ռ� ���⿡�� 3D �� ǥ���� �޽� ���� UI �θ� �Ѹ� �� �ִ� �͵鸸 ������� ����.
	if (BackgroundImageNRef)
	{
		BackgroundImageNRef->SetVisibility(IsVisibleBGImg ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
	if (MainIconImageNRef)
	{
		MainIconImageNRef->SetVisibility(IsVisibleBGImg ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	if (StarGradePart.IsValid())
	{
		if (IsCostumeItem(NativeItemData))
			StarGradePart->SetVisibility(ESlateVisibility::Collapsed);
		else
			StarGradePart->UpdateStar(NativeItemData);
	}
}

UMaterialInstanceConstant* UB2DynItemIcon_LobbyInven::GetBGPanelMIC()
{
	return BackgroundImageNRef ? Cast<UMaterialInstanceConstant>(BackgroundImageNRef->GetBrush().GetResourceObject()) : NULL;
}

UMaterialInstanceConstant* UB2DynItemIcon_LobbyInven::GetMainIconMIC()
{
	return MainIconImageNRef ? Cast<UMaterialInstanceConstant>(MainIconImageNRef->GetBrush().GetResourceObject()) : NULL;
}

bool UB2DynItemIcon_LobbyInven::IsBtnAreaUsed() const
{
	return (IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven);
}