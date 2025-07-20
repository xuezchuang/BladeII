// Fill out your copyright notice in the Description page of Project Settings.
#include "B2DynItemIcon_LobbyInven.h"
//#include "BladeII.h"
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
	

	//CachedCharacterStore = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
}

void UB2DynItemIcon_LobbyInven::CacheAssets()
{
	Super::CacheAssets();

	// 绢恫 扒 喉风橇赴飘辑 匙捞萍宏 饭欺繁胶 技泼阑 窍绰单 傲 扁粮 规侥窍绊 货肺款 规侥捞 去犁窍绰 巴. 窜瘤 弊订 捞繁 规侥阑 隔耳带 巴老 挥 扁鸥 促弗 捞蜡绰 绝促.
	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UB2UIWidgetBase, UIP_RedDot);
	GET_SLOT(UPanelWidget, P_EquipInfo);
	GET_SLOT(UImage, IMG_PresetBG);
	GET_SLOT(UTextBlock, TB_Preset);
	GET_SLOT(UImage, IMG_EquippedCheck);

	GET_SLOT(UPanelWidget, P_DevUI); // 捞扒 趣矫 隔扼辑 犬角洒 见扁扁 困秦 硅器魄俊辑档 某教
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
		// 酒贰贸烦 官牢靛甫 吧瘤 臼绰促. 快府狼 格利篮 NativeOn** 阑 罐绰 巴. 芭扁辑 On**SenderBTNArea 甫 烹秦 棵 巴烙.
		//BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2DynItemIcon_LobbyInven::OnClickedBTNArea);
		//BIND_PRESS_FUNC_TO_BTN(BTN_Area, &UB2DynItemIcon_LobbyInven::OnPressedBTNArea);
		//BIND_RELEASE_FUNC_TO_BTN(BTN_Area, &UB2DynItemIcon_LobbyInven::OnReleasedBTNArea);
		BTN_Area->SetVisibility(ESlateVisibility::HitTestInvisible); 
	}
	this->SetVisibility(ESlateVisibility::Visible); // BTN_Area 绰 烹苞窍绊 唱绰 涝仿阑 罐档废. NativeOn** 揪府令啊 甸绢坷档废 窃.
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

	// 硅摹秦敌 widget 甸狼 native reference 甸捞 valid 茄 措肺 诀单捞飘.
	// 汲飞 NativeItemData 磊眉啊 官差瘤 臼歹扼档 泅犁 厘馒 厘厚啊 官拆 锭 殿 咯矾 惑炔俊辑 诀单捞飘啊 鞘夸且 巴. 概锅 货肺 积己茄促搁 绢瞒乔 鞘夸 绝摆瘤父
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

	//夯酒捞袍篮 New惑怕甫 官层凛聪促
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
		// 辆幅俊 蝶扼 Simple 阑 荤侩窍绰 霸 乐绊 酒囱 霸 乐澜.
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
		BTN_Area->SetIsEnabled(IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven); // 老窜篮 皋牢 焊包窃 轰俊辑父 荤侩.

	// 酒捞袍 单捞磐啊 捞固 甸绢啊 乐菌促搁 钎矫档 盎脚秦具 且 淀. 老馆利牢 版快绰 UpdateItemData 甫 窍扁 傈俊 SetIconUsageType 阑 刚历 窍绰 巴.
	if (NativeItemData.ItemRefID > 0)
		UpdateItemData(NativeItemData);

	if (InUsageType == ELobbyInvenItemIconUsage::EEIIT_AutoEnhanceResult ||
		InUsageType == ELobbyInvenItemIconUsage::EEIIT_MailAllReceiveResult)
	{ // 磊悼 碍拳 搬苞芒俊辑绰 酒捞能 磊脚篮 涝仿 罐阑 鞘夸 绝捞 磊脚捞 困摹茄 归弊扼款靛 胶农费冠胶啊 胶农费父 登搁 凳.
		// 狐弗 惶扁俊辑档 悼老窍霸 累悼 秦具窃 窍瘤父 唱串
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
	{	// 碍拳 犁丰 酒捞能篮 腹酒辑 累栏聪鳖 饭骇 钎矫甫 窍瘤 臼绰促. 沥荐 酒捞袍篮 饭骇捞 绝绰 吧肺 摹绊..		
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::FromString(FString::FromInt(NativeItemData.Level)));
	}

	return FText();
}

FText UB2DynItemIcon_LobbyInven::GetItemAmountText() const
{
	if (IconUsageType != ELobbyInvenItemIconUsage::EEIIT_ItemLevelupMenuIngredient && !IsEssenceItem(NativeItemData) && 
		NativeItemData.InventoryType == EItemInvenType::EIIVT_Consumables && !IsAnvilItem(NativeItemData))
	{ // 碍拳 犁丰 酒捞能篮 腹酒辑 累栏聪鳖 饭骇 钎矫甫 窍瘤 臼绰促. 沥荐 酒捞袍篮 饭骇捞 绝绰 吧肺 摹绊..		
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

	// 酒付档 皋牢 焊包窃 芒俊辑父 焊捞霸 瞪 淀.
	if (IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven)
	{
		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;

		if (LobbyInven)
		{
			FB2Item EquippedAtThisPos;
			if (LobbyInven->GetEquippedItemAtPlace(EquippedAtThisPos, NativeItemData.EquipPlace)) // 鞍篮 困摹俊 厘馒等 吧 茫酒辑 厚背.
			{	// True 府畔捞绢具 力措肺 茫篮 巴. 厘脚备搁 PrimaryPointType 捞 促甫 荐 乐澜.
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
{ // 焊包窃 侩
	return (NativeItemData.bIsLocked && IsEquipmentItem(NativeItemData) && (IconUsageType == ELobbyInvenItemIconUsage::EEIIT_LobbyInven)) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
}

ESlateVisibility UB2DynItemIcon_LobbyInven::GetItemLockedImageVisibility_Detail() const
{ // 技何芒 侩
	return (NativeItemData.bIsLocked && IsEquipmentItem(NativeItemData) && (IconUsageType == ELobbyInvenItemIconUsage::EEIIT_ItemDetailWindow || IconUsageType == ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget)) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
}

void UB2DynItemIcon_LobbyInven::OnClickedSenderBTNArea()
{ // ManualScrollBox 扁瓷狼 老何肺 阂府绰 牢磐其捞胶
	Super::OnClickedSenderBTNArea();

	if (IsBtnAreaUsed())
	{
		OnClickedBTNArea(); // 角力 努腐 勤甸傅
	}
}
void UB2DynItemIcon_LobbyInven::OnPressedSenderBTNArea()
{// ManualScrollBox 扁瓷狼 老何肺 阂府绰 牢磐其捞胶
	Super::OnPressedSenderBTNArea();

	if (IsBtnAreaUsed())
	{
		OnPressedBTNArea(); // 角力 努腐 勤甸傅
	}
}
void UB2DynItemIcon_LobbyInven::OnReleasedSenderBTNArea()
{// ManualScrollBox 扁瓷狼 老何肺 阂府绰 牢磐其捞胶
	Super::OnReleasedSenderBTNArea();

	if (IsBtnAreaUsed())
	{
		OnReleasedBTNArea(); // 角力 努腐 勤甸傅
	}
}

bool UB2DynItemIcon_LobbyInven::OnClickBTNForLevelup(UB2LobbyInventory* LobbyInven)
{
	bool bHandledClickSoundPlay = false;

	//// 急琶 惑怕 函版秦 林绊 酒捞能 诀单捞飘档 官肺 茄促.
	//if (NativeItemData.bSelectedAsIngred)
	//{
	//	UnSelectForItemLevelupIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	//}
	//else
	//{
	//	if (NativeItemData.bIsLocked)
	//	{
	//		AskForUnlockItemPopupCommon(ELobbyInvenItemOpMode::EIVIOP_Levelup);
	//	}
	//	else if (LobbyInven->IsIngredsFull())
	//	{ // 急琶捞 救登绊 皋矫瘤 谎覆
	//		ABladeIIGameMode::SetHUDCenterMessage(
	//			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AlreadySelectedMaxEnhanceIngred"))), FText::AsNumber(LobbyInven->GetItemLevelupIngredLimit())),
	//			3.0f // 捞繁 磊龙备饭茄 巴鳖瘤 汲沥窍霸 且 扒 酒囱 芭 鞍绊 烙狼肺 利寸洒 矫埃阑 霖促.
	//		);
	//	}
	//	else if (LobbyInven->IsMaxItemLevelUpExpectedByCurrentSelection())
	//	{ // 急琶捞 救登绊 皋矫瘤 谎覆
	//		ABladeIIGameMode::SetHUDCenterMessage(
	//			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AlreadyExpectedMaxEnhanceLevel"))),
	//			3.0f // 捞繁 磊龙备饭茄 巴鳖瘤 汲沥窍霸 且 扒 酒囱 芭 鞍绊 烙狼肺 利寸洒 矫埃阑 霖促.
	//		);
	//	}
	//	else if (NativeItemData.EnhanceLevel > 0)
	//	{
	//		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
	//			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_AskLevelUpAlreadyEnhanceItem"))),
	//			0.f,
	//			true,
	//			true,
	//			EUIMsgPopupButtonGroup::YesOrNo,
	//			FMsgPopupOnClick::CreateLambda([this]() {
	//			SelectForItemLevelupIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	//			PlayLevelupIngredSelectedSound();
	//		})
	//		);
	//		bHandledClickSoundPlay = true;
	//	}
	//	else if (IsContainsPreset)
	//	{
	//		AskForUnEquipItemPopupCommon();

	//		bHandledClickSoundPlay = true;
	//	}
	//	else if (LobbyInven->IsHigherGradeThanEnhanceTarget(NativeItemData))
	//	{
	//		//// 官肺 急琶窍瘤 臼绊 版绊 皋矫瘤甫 剁框.
	//		//UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		//	BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
	//		//	BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_HigherGradeThanEnhanceTarget"))),
	//		//	0.f,
	//		//	true,
	//		//	true,
	//		//	EUIMsgPopupButtonGroup::YesOrNo,
	//		//	FMsgPopupOnClick::CreateLambda([this]() {
	//		//	SelectForItemLevelupIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	//		//	PlayLevelupIngredSelectedSound();
	//		//})
	//		//);
	//		//bHandledClickSoundPlay = true;
	//	}
	//	else
	//	{ // SelectForItemLevelupIngredClass 捞亥飘 贸府窍绰 镑俊辑档 泅犁 鸥百苞 鞍篮 酒捞袍牢瘤, 悼辆牢瘤 殿狼 贸府甫 窃. 捞吧 焊陈促绊 秦辑 官肺 急琶登绰 扒 酒丛.
	//		//SelectForItemLevelupIngredClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	//		//PlayLevelupIngredSelectedSound();
	//		//bHandledClickSoundPlay = true;
	//		////葛电 滚瓢幅绰 喘副锭 茄锅 譬配府倔 矫弊澄阑 眉农秦具登辑 扑诀芒捞 陡阑锭 碍力 龋免																			
	//		//if (BTN_Area.IsValid())
	//		//{
	//		//	BTN_Area->TutorialButtonSignal();
	//		//}
	//	}
	//}
	
	return bHandledClickSoundPlay;
}

bool UB2DynItemIcon_LobbyInven::OnClickBTNForHammer(UB2LobbyInventory* LobbyInven)
{
	bool bHandledClickSoundPlay = false;

	//// 急琶 惑怕 函版秦 林绊 酒捞能 诀单捞飘档 官肺 茄促.
	//if (NativeItemData.bSelectedAsIngred)
	//{
	//	UnSelectForItemHammerClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	//}
	//else
	//{
	//	SelectForItemHammerClass<int64>::GetInstance().Signal(NativeItemData.InstanceUID);
	//	PlayLevelupIngredSelectedSound();
	//	bHandledClickSoundPlay = true;
	//	//葛电 滚瓢幅绰 喘副锭 茄锅 譬配府倔 矫弊澄阑 眉农秦具登辑 扑诀芒捞 陡阑锭 碍力 龋免																			
	//	if (BTN_Area.IsValid())
	//	{
	//		BTN_Area->TutorialButtonSignal();
	//	}

	//}

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
			// 捞 矫痢俊 Inventory 狼 TargetItemID 绰 技泼登绢 乐绢具 窃. (UB2LobbyInventory::EnterItemOpMode)
			if (LobbyInven->GetItemOPTargetItemData(TargetItem) && LobbyInven->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_EnhancementChange)
			{
				// 碍拳 饭骇捞 鞍阑锭
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
	if (IconUsageType != ELobbyInvenItemIconUsage::EEIIT_LobbyInven) // 捞固 焊包窃 芒捞 酒聪搁 梅何磐 喘府瘤 富酒具 偿偿
	{
		return;
	}

	bool bHandledClickSoundPlay = false; // 场鳖瘤 false 捞搁 扁夯 荤款靛 敲饭捞

	// 泅犁 NativeItemData 甫 荤侩秦辑 技何 沥焊芒 扑诀... 父捞 酒聪扼 碍拳 葛靛老 锭绰 急琶阑 秦具.
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
	// 铰鞭俊辑绰 犁丰 酒捞袍捞鄂 霸 绝绰单 弊成 扁夯栏肺 detail 芒 剁快绊 芭扁辑 弊 酒捞袍栏肺 铰鞭芒阑 促矫 剁匡 荐 乐促.
	else
	{
		CreateLobbyItemDetailPopupCommon(GetOwningPlayer(), NativeItemData, false); // Inventory (stored) 侩栏肺 detail 芒 积己

		//葛电 滚瓢幅绰 喘副锭 茄锅 譬配府倔 矫弊澄阑 眉农秦具登辑 扑诀芒捞 陡阑锭 碍力 龋免		
		if(BTN_Area.IsValid())
		{ BTN_Area->TutorialButtonSignal(); }		
	}

	if (!bHandledClickSoundPlay)
	{
		PlayCommonIconClickSound();
	}

	OnClickedBTNArea_BP(); // 喉风橇赴飘 勤甸傅

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
{ // 碍拳 钦己 檬岿 犁丰肺 荤侩窍扁 困秦 急琶 努腐 矫 泪陛 秦力窍摆绰啊 窍绰 扑诀.
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
	OnPressedBTNArea_BP(); // 喉风橇赴飘 勤甸傅
}

void UB2DynItemIcon_LobbyInven::OnReleasedBTNArea()
{
	OnReleasedBTNArea_BP(); // 喉风橇赴飘 勤甸傅
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
	// 酒捞袍碍拳 焊龋籍 眉农
	if (NativeItemData.ItemClass == EItemClass::EIC_ProtectionScroll)
	{
		// 泅犁 NativeItemData 甫 荤侩秦辑 技何 沥焊芒 扑诀... 父捞 酒聪扼 碍拳 葛靛老 锭绰 急琶阑 秦具.
		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
		if (LobbyInven && LobbyInven->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_Enhancement)
			return ESlateVisibility::HitTestInvisible;
	}

	return ESlateVisibility::Hidden;
}

void UB2DynItemIcon_LobbyInven::SetShowOverlayInfoOnly(bool IsVisibleBGImg /*= false*/, bool IsVisibleGrade /*= false*/)
{
	// 捞芭 措眉肺 钦己 楷免俊辑 3D 肺 钎矫茄 皋浆 困俊 UI 肺父 谎副 荐 乐绰 巴甸父 巢扁妨绰 芭烙.
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