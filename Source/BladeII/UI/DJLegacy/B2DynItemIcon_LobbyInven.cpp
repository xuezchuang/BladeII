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

	// 어떤 건 블루프린트서 네이티브 레퍼런스 세팅을 하는데 걍 기존 방식하고 새로운 방식이 혼재하는 것. 단지 그땐 이런 방식을 몰랐던 것일 뿐 기타 다른 이유는 없다.
	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UB2UIWidgetBase, UIP_RedDot);
	GET_SLOT(UPanelWidget, P_EquipInfo);
	GET_SLOT(UImage, IMG_PresetBG);
	GET_SLOT(UTextBlock, TB_Preset);
	GET_SLOT(UImage, IMG_EquippedCheck);

	GET_SLOT(UPanelWidget, P_DevUI); // 이건 혹시 몰라서 확실히 숨기기 위해 배포판에서도 캐싱
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
		// 아래처럼 바인드를 걸지 않는다. 우리의 목적은 NativeOn** 을 받는 것. 거기서 On**SenderBTNArea 를 통해 올 것임.
		//BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2DynItemIcon_LobbyInven::OnClickedBTNArea);
		//BIND_PRESS_FUNC_TO_BTN(BTN_Area, &UB2DynItemIcon_LobbyInven::OnPressedBTNArea);
		//BIND_RELEASE_FUNC_TO_BTN(BTN_Area, &UB2DynItemIcon_LobbyInven::OnReleasedBTNArea);
		BTN_Area->SetVisibility(ESlateVisibility::HitTestInvisible); 
	}
	this->SetVisibility(ESlateVisibility::Visible); // BTN_Area 는 통과하고 나는 입력을 받도록. NativeOn** 씨리즈가 들어오도록 함.
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

	// 배치해둔 widget 들의 native reference 들이 valid 한 대로 업데이트.
	// 설령 NativeItemData 자체가 바뀌지 않더라도 현재 장착 장비가 바뀔 때 등 여러 상황에서 업데이트가 필요할 것. 매번 새로 생성한다면 어차피 필요 없겠지만
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

	//본아이템은 New상태를 바꿔줍니다
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
		// 종류에 따라 Simple 을 사용하는 게 있고 아닌 게 있음.
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
		BTN_Area->SetIsEnabled(IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven); // 일단은 메인 보관함 뷰에서만 사용.

	// 아이템 데이터가 이미 들어가 있었다면 표시도 갱신해야 할 듯. 일반적인 경우는 UpdateItemData 를 하기 전에 SetIconUsageType 을 먼저 하는 것.
	if (NativeItemData.ItemRefID > 0)
		UpdateItemData(NativeItemData);

	if (InUsageType == ELobbyInvenItemIconUsage::EEIIT_AutoEnhanceResult ||
		InUsageType == ELobbyInvenItemIconUsage::EEIIT_MailAllReceiveResult)
	{ // 자동 강화 결과창에서는 아이콘 자신은 입력 받을 필요 없이 자신이 위치한 백그라운드 스크롤박스가 스크롤만 되면 됨.
		// 빠른 뽑기에서도 동일하게 작동 해야함 하지만 나눔
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
	{	// 강화 재료 아이콘은 많아서 작으니까 레벨 표시를 하지 않는다. 정수 아이템은 레벨이 없는 걸로 치고..		
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::FromString(FString::FromInt(NativeItemData.Level)));
	}

	return FText();
}

FText UB2DynItemIcon_LobbyInven::GetItemAmountText() const
{
	if (IconUsageType != ELobbyInvenItemIconUsage::EEIIT_ItemLevelupMenuIngredient && !IsEssenceItem(NativeItemData) && 
		NativeItemData.InventoryType == EItemInvenType::EIIVT_Consumables && !IsAnvilItem(NativeItemData))
	{ // 강화 재료 아이콘은 많아서 작으니까 레벨 표시를 하지 않는다. 정수 아이템은 레벨이 없는 걸로 치고..		
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

	// 아마도 메인 보관함 창에서만 보이게 될 듯.
	if (IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven)
	{
		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;

		if (LobbyInven)
		{
			FB2Item EquippedAtThisPos;
			if (LobbyInven->GetEquippedItemAtPlace(EquippedAtThisPos, NativeItemData.EquipPlace)) // 같은 위치에 장착된 걸 찾아서 비교.
			{	// True 리턴이어야 제대로 찾은 것. 장신구면 PrimaryPointType 이 다를 수 있음.
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
{ // 보관함 용
	return (NativeItemData.bIsLocked && IsEquipmentItem(NativeItemData) && (IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven)) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
}

ESlateVisibility UB2DynItemIcon_LobbyInven::GetItemLockedImageVisibility_Detail() const
{ // 세부창 용
	return (NativeItemData.bIsLocked && IsEquipmentItem(NativeItemData) && (IconUsageType == ELobbyInvenItemIconUsage::EEIIT_ItemDetailWindow || IconUsageType == ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget)) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
}

void UB2DynItemIcon_LobbyInven::OnClickedSenderBTNArea()
{ // ManualScrollBox 기능의 일부로 불리는 인터페이스
	Super::OnClickedSenderBTNArea();

	if (IsBtnAreaUsed())
	{
		OnClickedBTNArea(); // 실제 클릭 핸들링
	}
}
void UB2DynItemIcon_LobbyInven::OnPressedSenderBTNArea()
{// ManualScrollBox 기능의 일부로 불리는 인터페이스
	Super::OnPressedSenderBTNArea();

	if (IsBtnAreaUsed())
	{
		OnPressedBTNArea(); // 실제 클릭 핸들링
	}
}
void UB2DynItemIcon_LobbyInven::OnReleasedSenderBTNArea()
{// ManualScrollBox 기능의 일부로 불리는 인터페이스
	Super::OnReleasedSenderBTNArea();

	if (IsBtnAreaUsed())
	{
		OnReleasedBTNArea(); // 실제 클릭 핸들링
	}
}

bool UB2DynItemIcon_LobbyInven::OnClickBTNForLevelup(UB2LobbyInventory* LobbyInven)
{
	bool bHandledClickSoundPlay = false;

	// 선택 상태 변경해 주고 아이콘 업데이트도 바로 한다.
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
		{ // 선택이 안되고 메시지 뿌림
			ABladeIIGameMode::SetHUDCenterMessage(
				FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AlreadySelectedMaxEnhanceIngred"))), FText::AsNumber(LobbyInven->GetItemLevelupIngredLimit())),
				3.0f // 이런 자질구레한 것까지 설정하게 할 건 아닌 거 같고 임의로 적당히 시간을 준다.
			);
		}
		else if (LobbyInven->IsMaxItemLevelUpExpectedByCurrentSelection())
		{ // 선택이 안되고 메시지 뿌림
			ABladeIIGameMode::SetHUDCenterMessage(
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AlreadyExpectedMaxEnhanceLevel"))),
				3.0f // 이런 자질구레한 것까지 설정하게 할 건 아닌 거 같고 임의로 적당히 시간을 준다.
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
			// 바로 선택하지 않고 경고 메시지를 띄움.
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
		{ // SelectForItemLevelupIngredClass 이벤트 처리하는 곳에서도 현재 타겟과 같은 아이템인지, 동종인지 등의 처리를 함. 이걸 보냈다고 해서 바로 선택되는 건 아님.
			SelectForItemLevelupIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
			PlayLevelupIngredSelectedSound();
			bHandledClickSoundPlay = true;
			//모든 버튼류는 눌릴때 한번 튜토리얼 시그널을 체크해야되서 팝업창이 떳을때 강제 호출																			
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

	// 선택 상태 변경해 주고 아이콘 업데이트도 바로 한다.
	if (NativeItemData.bSelectedAsIngred)
	{
		UnSelectForItemHammerClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	}
	else
	{
		SelectForItemHammerClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
		PlayLevelupIngredSelectedSound();
		bHandledClickSoundPlay = true;
		//모든 버튼류는 눌릴때 한번 튜토리얼 시그널을 체크해야되서 팝업창이 떳을때 강제 호출																			
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
			// 이 시점에 Inventory 의 TargetItemID 는 세팅되어 있어야 함. (UB2LobbyInventory::EnterItemOpMode)
			if (LobbyInven->GetItemOPTargetItemData(TargetItem) && LobbyInven->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_EnhancementChange)
			{
				// 강화 레벨이 같을때
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
	if (IconUsageType != ELobbyInvenItemIconUsage::EEIIT_LobbyInven) // 이미 보관함 창이 아니면 첨부터 눌리지 말아야 낄낄
	{
		return;
	}

	bool bHandledClickSoundPlay = false; // 끝까지 false 이면 기본 사운드 플레이

	// 현재 NativeItemData 를 사용해서 세부 정보창 팝업... 만이 아니라 강화 모드일 때는 선택을 해야.
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
	// 승급에서는 재료 아이템이란 게 없는데 그냥 기본으로 detail 창 띄우고 거기서 그 아이템으로 승급창을 다시 띄울 수 있다.
	else
	{
		CreateLobbyItemDetailPopupCommon(GetOwningPlayer(), NativeItemData, false); // Inventory (stored) 용으로 detail 창 생성

		//모든 버튼류는 눌릴때 한번 튜토리얼 시그널을 체크해야되서 팝업창이 떳을때 강제 호출		
		if(BTN_Area.IsValid())
		{ BTN_Area->TutorialButtonSignal(); }		
	}

	if (!bHandledClickSoundPlay)
	{
		PlayCommonIconClickSound();
	}

	OnClickedBTNArea_BP(); // 블루프린트 핸들링

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
{ // 강화 합성 초월 재료로 사용하기 위해 선택 클릭 시 잠금 해제하겠는가 하는 팝업.
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
	OnPressedBTNArea_BP(); // 블루프린트 핸들링
}

void UB2DynItemIcon_LobbyInven::OnReleasedBTNArea()
{
	OnReleasedBTNArea_BP(); // 블루프린트 핸들링
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
	// 아이템강화 보호석 체크
	if (NativeItemData.ItemClass == EItemClass::EIC_ProtectionScroll)
	{
		// 현재 NativeItemData 를 사용해서 세부 정보창 팝업... 만이 아니라 강화 모드일 때는 선택을 해야.
		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
		if (LobbyInven && LobbyInven->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_Enhancement)
			return ESlateVisibility::HitTestInvisible;
	}

	return ESlateVisibility::Hidden;
}

void UB2DynItemIcon_LobbyInven::SetShowOverlayInfoOnly(bool IsVisibleBGImg /*= false*/, bool IsVisibleGrade /*= false*/)
{
	// 이거 대체로 합성 연출에서 3D 로 표시한 메쉬 위에 UI 로만 뿌릴 수 있는 것들만 남기려는 거임.
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