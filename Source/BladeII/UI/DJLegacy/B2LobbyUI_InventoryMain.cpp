// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_InventoryMain.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "B2UIManager_Lobby.h"
#include "B2Inventory.h"
#include "B2LobbyInventory.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2DynItemIconRow_LobbyInven.h"
#include "B2DynItemIcon_LobbyEquip.h"
#include "B2LobbyUI_HeroMgmtCommon.h"
#include "Event.h"
#include "B2UIWidgetBase.h"
#include "B2ItemInfo.h"
#include "B2UILevelupAutoSelect.h"
#include "Retailer.h"
#include "B2UICommonTab.h"
#include "TutorialManager.h"
#include "B2HighlightWidget.h"
#include "B2UIUtil.h"

#include "BladeIIGameImpl.h"

FString UB2LobbyUI_InventoryMain::GetEquipSlotSuffix(int32 InEquipPlaceIdx)
{
	return GetEquipSlotSuffix(static_cast<EItemEquipPlace>(InEquipPlaceIdx));
}

FString UB2LobbyUI_InventoryMain::GetEquipSlotSuffix(EItemEquipPlace InEquipPlace)
{
	switch (InEquipPlace)
	{
	case EItemEquipPlace::EIEP_Weapon: return EquipSlotSuffix_Weapon;
	case EItemEquipPlace::EIEP_Helmet: return EquipSlotSuffix_Helmet;
	case EItemEquipPlace::EIEP_BodyArmor: return EquipSlotSuffix_BodyArmor;
	case EItemEquipPlace::EIEP_Glove: return EquipSlotSuffix_Glove;
	case EItemEquipPlace::EIEP_Shoes: return EquipSlotSuffix_Shoes;
	case EItemEquipPlace::EIEP_Belt: return EquipSlotSuffix_Belt;
	case EItemEquipPlace::EIEP_Necklace: return EquipSlotSuffix_Necklace;
	case EItemEquipPlace::EIEP_Ring: return EquipSlotSuffix_Ring;
	}
	return TEXT("");
}

UImage* UB2LobbyUI_InventoryMain::GetEquipEffectImg(EItemEquipPlace InEquipPlace)
{
	int32 EquipPlaceIdx = static_cast<int32>(InEquipPlace);
	return EquipEffectImgs.IsValidIndex(EquipPlaceIdx) ? EquipEffectImgs[EquipPlaceIdx] : NULL;
}

UImage* UB2LobbyUI_InventoryMain::GetEquipEffectImg2(EItemEquipPlace InEquipPlace)
{
	int32 EquipPlaceIdx = static_cast<int32>(InEquipPlace);
	return EquipEffectImgs2.IsValidIndex(EquipPlaceIdx) ? EquipEffectImgs2[EquipPlaceIdx] : NULL;
}

UImage* UB2LobbyUI_InventoryMain::GetNoEquipIconImageForEquipPlace(EItemEquipPlace InEquipPlace)
{
	int32 EquipPlaceIdx = static_cast<int32>(InEquipPlace);
	return NoEquipIcons.IsValidIndex(EquipPlaceIdx) ? NoEquipIcons[EquipPlaceIdx] : NULL;
}

UB2LobbyUI_InventoryMain::UB2LobbyUI_InventoryMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;
	HeroMgmtBase = NULL; // 喉风橇赴飘甫 烹秦 技泼.
	ItemIconRowWidgetClass = NULL; // Property 技泼 登绢 乐绢具 窃.
	ItemDisplayPanelNativeRef = NULL; // 捞扒 喉风橇赴飘俊辑 技泼.
	CurSelectedItemInvenTab = EItemInvenType::EIIVT_Weapon;
	CachedSelectedItemInvenTab = EItemInvenType::EIIVT_Weapon;

	for (int32 II = 0; II < MaxEquipPlace; ++II)
	{
		EquipItemIconPanels[II] = NULL;
		CreatedEquipItemIcons[II] = NULL;
	}

	StoredItemNumTextColor_Normal = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));
	StoredItemNumTextColor_Over = FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f));


	EquipSlotSuffix_Weapon = TEXT("_Weapon");
	EquipSlotSuffix_Helmet = TEXT("_Helmet");
	EquipSlotSuffix_BodyArmor = TEXT("_BodyArmor");
	EquipSlotSuffix_Glove = TEXT("_Glove");
	EquipSlotSuffix_Shoes = TEXT("_Shoes");
	EquipSlotSuffix_Belt = TEXT("_Belt");
	EquipSlotSuffix_Necklace = TEXT("_Necklace");
	EquipSlotSuffix_Ring = TEXT("_Ring");

	NoEquipIconImgPrefix = TEXT("IMG_NoEquipIcon");
	EquipEffectImgPrefix = TEXT("IMG_EquipEffect");
	EquipButtonPrefix = TEXT("BTN_Equip");

	EquipEffectPlayTime = 1.0f;

	CachedCharacterStore = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
}

void UB2LobbyUI_InventoryMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateEquipEffectParamAnim();
}

void UB2LobbyUI_InventoryMain::SubscribeEvents()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_SubscribeEvents);

	UnsubscribeEvents();

	Issues.Add(UpdateLobbyInventoryControlClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->UpdateInvenControlButtonState();
		//		this->UpdateInvenTabWidgetVisibilityState();
		//		this->UpdateStoredItemIconsForCurrentSelection();	 // 焊包窃 酒捞袍父 府橇饭浆.
		this->RedDotCondition_TapState();
	}
	));

	Issues.Add(LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->UpdateAutoSelectButton();
	}
	));

	Issues.Add(DeliveryModifyItemNewFlagClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->DeliveryModifyItemNewFlag();
	}
	));

	Issues.Add(LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe2(
		[this](EPCClass PCClass)
	{
		this->OnHeroMgmtPCChanged(PCClass);
	}
	));

	Issues.Add(ChangeInvenItemOpModeClass<ELobbyInvenItemOpMode>::GetInstance().Subscribe2(
		[this](ELobbyInvenItemOpMode InvenItemOpMode)
	{
		this->OnChangeInvenItemOpMode(InvenItemOpMode);
	}
	));

	Issues.Add(LobbySlotChangeEffectClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->AllPlayEquipEffect();
	}
	));

	Issues.Add(EnableInventoryTouchClass<bool>::GetInstance().Subscribe2(
		[this](bool InIsEnable)
	{
		if (IMG_BackGround.IsValid())
		{
			IMG_BackGround->SetVisibility(InIsEnable ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}
	));

	Issues.Add(CostumeEquipSlotVisibleClass<bool>::GetInstance().Subscribe2(
	[this](bool IsVisible)
	{
		this->UpdateItemEquipSlotVisible(IsVisible);
	}
	));

	Issues.Add(ForcedInvenTabOpenClass<EItemInvenType>::GetInstance().Subscribe2(
		[this](EItemInvenType InvenType)
	{
		this->NativeSetInvenTabSelection(InvenType);
	}
	));

	if (bEventSubscribed)
		return;

	for (int EquipSlot = 0; EquipSlot < MaxEquipPlace; ++EquipSlot)
	{
		if (EquipSlotButtons.IsValidIndex(EquipSlot) && EquipSlotButtons[EquipSlot] != nullptr)
		{
			EItemInvenType ItemInvenType = B2UIUtil::GetItemEquipPlaceToItemInventoryType(static_cast<EItemEquipPlace>(EquipSlot));

			switch (ItemInvenType)
			{
			case EItemInvenType::EIIVT_Weapon:
				EquipSlotButtons[EquipSlot]->OnClicked.AddDynamic(this, &UB2LobbyUI_InventoryMain::OnClickTabWeapon);
				break;
			case EItemInvenType::EIIVT_Protection:
				EquipSlotButtons[EquipSlot]->OnClicked.AddDynamic(this, &UB2LobbyUI_InventoryMain::OnClickTabProtection);
				break;
			case EItemInvenType::EIIVT_Accessory:
				EquipSlotButtons[EquipSlot]->OnClicked.AddDynamic(this, &UB2LobbyUI_InventoryMain::OnClickTabAccessory);
				break;
			}
		}
	}

	bEventSubscribed = true;
}

void UB2LobbyUI_InventoryMain::UnsubscribeEvents()
{
	Issues.Empty();

	if (!bEventSubscribed)
		return;

	for (int EquipSlot = 0; EquipSlot < MaxEquipPlace; ++EquipSlot)
	{
		if (EquipSlotButtons.IsValidIndex(EquipSlot) && EquipSlotButtons[EquipSlot] != nullptr)
		{
			EquipSlotButtons[EquipSlot]->OnClicked.RemoveAll(this);
		}
	}

	bEventSubscribed = false;
}

void UB2LobbyUI_InventoryMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_StartFromLobby);
	Super::StartFromLobby(InUIManager, InGM);

	bFirstHeroChange = true;

	SubscribeEvents();

	CurSelectedPCClass = CachedLobbyGM->GetHeroMgmtModePCClass();

	// 霉 轰俊辑 急琶等 PC 酒捞袍 眉农. 傍侩 家葛前狼 版快绰 咯扁 富绊 AccountInfo 啊廉棵 锭 令澜 夸没.. 窍妨绊 窍绰单 巩力啊 乐绢辑 蝶肺 咯扁辑档 夸没秦具 且 荐 乐澜.
	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(CurSelectedPCClass);

	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	CachedInventory->SetMainInvenViewCurrentTab(CurSelectedItemInvenTab);
	UpdateInvenTabWidgetVisibilityState();
	// 贸澜俊 茄锅 窍绊.. 泅犁 某腐磐唱 厘厚 徘 函版, 漂沥 酒捞袍 厘馒/秦力 殿狼 惑炔俊辑 促矫 阂妨具 窃.
	// UpdateAllItemIconsForCurrentSelection();	//饭靛村捞 部烙 快急 巩力痢捞 绝绢辑 何福瘤 臼嚼聪促 -woong

	if (HeroMgmtBase)
	{
		HeroMgmtBase->StartFromLobby(InUIManager, InGM); // 捞扒 LobbyUISwitcher 俊辑 葛福聪 流立 秦 林绢具..
		HeroMgmtBase->SetPresetMode(EInventoryPresetMode::EIPM_Equipment);
		HeroMgmtBase->ShowEnhanceItemSetButton(true);

		ShowHeroMgmtControlMenu(true);
	}

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_EquipMgmtButton")));

	DoMarkRedDot();

	UpdateInvenControlButtonState();
}

void UB2LobbyUI_InventoryMain::ShowHeroMgmtControlMenu(bool bShow)
{
	if (HeroMgmtBase)
	{
		HeroMgmtBase->ShowCharacterButtons(bShow);
		HeroMgmtBase->ShowEnhanceItemSetButton(bShow);
		HeroMgmtBase->ShowWingButton(bShow);
		HeroMgmtBase->ShowHeadButton(bShow);
		HeroMgmtBase->ShowDetailedInfoExtraButtons(bShow);

		if (bShow)
			HeroMgmtBase->SetPresetMode(EInventoryPresetMode::EIPM_Equipment);
		else
			HeroMgmtBase->SetHidePresetControls();
	}
}

void UB2LobbyUI_InventoryMain::OnChangeInvenItemOpMode(ELobbyInvenItemOpMode InvenItemOpMode)
{
	ShowHeroMgmtControlMenu(InvenItemOpMode == ELobbyInvenItemOpMode::EIVIOP_None);
}

void UB2LobbyUI_InventoryMain::CacheAssets()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_CacheAssets);
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_ItemOpModeGuide);
	HideItemOpModeGuideText();

	// 唱吝俊 甸绢柯 局甸父 捞繁 规侥捞 利侩凳..
	GET_SLOT(UPanelWidget, CP_ContainerSizeTextSet);
	GET_SLOT(UB2Button, BTN_ContainerExpansion);
	GET_SLOT(UB2Button, BTN_SellAll);
	GET_SLOT(UB2Button, BTN_AutoEnhancement);
	GET_SLOT(UB2Button, BTN_AutoEquip);
	GET_SLOT(UB2Button, BTN_AutoSelect);
	GET_SLOT(UB2Button, BTN_AutoDecomposition);

	GET_SLOT(UImage, IMG_BackGround);

	GET_SLOT(UTextBlock, TB_AutoSelect);
	if (TB_AutoSelect.IsValid())
	{
		const FText& AutoSelectBtnText = GetAutoSelectButtonText();
		TB_AutoSelect->SetText(AutoSelectBtnText);
	}

	// 阿 厘馒 浇吩 困摹俊 蝶弗 widget 甸阑 匙捞怪 嘎苗 啊廉咳.
	NoEquipIcons.Empty();
	NoEquipIcons.AddZeroed(MaxEquipPlace);
	EquipEffectImgs.Empty();
	EquipEffectImgs.AddZeroed(MaxEquipPlace);
	EquipEffectImgs2.Empty();
	EquipEffectImgs2.AddZeroed(MaxEquipPlace);
	EquipSlotButtons.Empty();
	EquipSlotButtons.AddZeroed(MaxEquipPlace);

	for (int32 II = 0; II < MaxEquipPlace; ++II)
	{
		FString NoEquipIconImgNameStr = NoEquipIconImgPrefix + GetEquipSlotSuffix(II);
		NoEquipIcons[II] = GetSlot<UImage>(FName(*NoEquipIconImgNameStr));

		FString EquipEffectImgNameStr = EquipEffectImgPrefix + GetEquipSlotSuffix(II);
		EquipEffectImgs[II] = GetSlot<UImage>(FName(*EquipEffectImgNameStr));
		if (EquipEffectImgs[II])
		{
			EquipEffectImgs[II]->SetVisibility(ESlateVisibility::Collapsed);
		}

		FString EquipEffectImgNameStr2 = EquipEffectImgPrefix + GetEquipSlotSuffix(II) + TEXT("2"); // 2鳖瘤绰 窍靛内爹栏肺..
		EquipEffectImgs2[II] = GetSlot<UImage>(FName(*EquipEffectImgNameStr2));
		if (EquipEffectImgs2[II])
		{
			EquipEffectImgs2[II]->SetVisibility(ESlateVisibility::Collapsed);
		}
		FString EquipSlotButtonNameStr = EquipButtonPrefix + GetEquipSlotSuffix(II);
		EquipSlotButtons[II] = GetSlot<UButton>(FName(*EquipSlotButtonNameStr));
	}

	GET_SLOT(UCanvasPanel, X_LeftPopupPanel);
	GET_SLOT(UCanvasPanel, X_RightPopupPanel);

	GET_SLOT(UImage, IMG_EquipEffectParamAnimDummy);
	GET_SLOT(UImage, IMG_EquipEffectParamAnimDummy2);

	GET_SLOT(UB2UIWidgetBase, IMG_NewItemCheck_Weapon);
	GET_SLOT(UB2UIWidgetBase, IMG_NewItemCheck_Protection);
	GET_SLOT(UB2UIWidgetBase, IMG_NewItemCheck_Accessory);
	GET_SLOT(UB2UIWidgetBase, IMG_NewItemCheck_Consumables);

	NewitemCheckArr.SetNum(int32(EItemInvenType::EIIVT_End));
	GET_SLOT_BYNAME(UB2UIWidgetBase, NewitemCheckArr[0], IMG_NewItemCheck_Weapon);
	GET_SLOT_BYNAME(UB2UIWidgetBase, NewitemCheckArr[1], IMG_NewItemCheck_Protection);
	GET_SLOT_BYNAME(UB2UIWidgetBase, NewitemCheckArr[2], IMG_NewItemCheck_Accessory);
	GET_SLOT_BYNAME(UB2UIWidgetBase, NewitemCheckArr[3], IMG_NewItemCheck_Consumables);

	GET_SLOT(UTextBlock, TXT_Helmet);
	GET_SLOT(UTextBlock, TXT_BodyArmor);
	GET_SLOT(UTextBlock, TXT_Glove);
	GET_SLOT(UTextBlock, TXT_Shoes);
	GET_SLOT(UTextBlock, TXT_Weapon);
	GET_SLOT(UTextBlock, TXT_Belt);
	GET_SLOT(UTextBlock, TXT_Necklace);
	GET_SLOT(UTextBlock, TXT_Ring);

	GET_SLOT(UTextBlock, TB_AllSell);
	GET_SLOT(UTextBlock, TB_AutoEquip);
	GET_SLOT(UTextBlock, TB_AutoDecomposition);

	GET_SLOT(UB2UICommonTab, TAB_Weapon);
	GET_SLOT(UB2UICommonTab, TAB_Protection);
	GET_SLOT(UB2UICommonTab, TAB_Accessory);
	GET_SLOT(UB2UICommonTab, TAB_Consumables);

	UpdateStaticText();
	BindDelegates();
}

void UB2LobbyUI_InventoryMain::UpdateStaticText()
{
	if (TXT_Helmet.IsValid())
		TXT_Helmet->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_HelmetTab"))));
	if (TXT_BodyArmor.IsValid())
		TXT_BodyArmor->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_BodyArmorTab"))));
	if (TXT_Glove.IsValid())
		TXT_Glove->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_GloveTab"))));
	if (TXT_Shoes.IsValid())
		TXT_Shoes->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_ShoesTab"))));
	if (TXT_Weapon.IsValid())
		TXT_Weapon->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_WeaponTab"))));
	if (TXT_Belt.IsValid())
		TXT_Belt->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_BeltTab"))));
	if (TXT_Necklace.IsValid())
		TXT_Necklace->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_NecklaceTab"))));
	if (TXT_Ring.IsValid())
		TXT_Ring->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_RingTab"))));

	if (TB_AllSell.IsValid())
		TB_AllSell->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SellAll")));
	if (TB_AutoEquip.IsValid())
		TB_AutoEquip->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AutoEquip")));
	if (TB_AutoDecomposition.IsValid())
		TB_AutoDecomposition->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AutoDecomposition")));
}

void UB2LobbyUI_InventoryMain::BindDelegates()
{
	if (TAB_Weapon.IsValid())
		REGISTER_UITAB_BINDING(TAB_Weapon.Get(), &UB2LobbyUI_InventoryMain::OnClickTabWeapon);
	if (TAB_Protection.IsValid())
		REGISTER_UITAB_BINDING(TAB_Protection.Get(), &UB2LobbyUI_InventoryMain::OnClickTabProtection);
	if (TAB_Accessory.IsValid())
		REGISTER_UITAB_BINDING(TAB_Accessory.Get(), &UB2LobbyUI_InventoryMain::OnClickTabAccessory);
	if (TAB_Consumables.IsValid())
		REGISTER_UITAB_BINDING(TAB_Consumables.Get(), &UB2LobbyUI_InventoryMain::OnClickTabConsumables);
}

void UB2LobbyUI_InventoryMain::SetEquipItemIconPanelNativeRef(EItemEquipPlace InEquipPlace, class UCanvasPanel* InRef)
{
	if (InEquipPlace < EItemEquipPlace::EIEP_End)
	{
		EquipItemIconPanels[(int32)InEquipPlace] = InRef;
	}
}

UCanvasPanel* UB2LobbyUI_InventoryMain::GetEquipItemIconPanelNativeRef(EItemEquipPlace InEquipPlace)
{
	if (InEquipPlace < EItemEquipPlace::EIEP_End)
	{
		return EquipItemIconPanels[(int32)InEquipPlace];
	}
	return NULL;
}

void UB2LobbyUI_InventoryMain::SetEquipItemIcon(EItemEquipPlace InEquipPlace, class UB2DynItemIcon_LobbyEquip* InCreatedIcon)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_SetEquipItemIcon);
	if (InEquipPlace < EItemEquipPlace::EIEP_End)
	{
		CreatedEquipItemIcons[(int32)InEquipPlace] = InCreatedIcon;
	}
}

UB2DynItemIcon_LobbyEquip* UB2LobbyUI_InventoryMain::GetEquipItemIcon(EItemEquipPlace InEquipPlace)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_GetEquipItemIcon);
	if (InEquipPlace < EItemEquipPlace::EIEP_End)
	{
		return CreatedEquipItemIcons[(int32)InEquipPlace];
	}
	return NULL;
}

UB2DynItemIcon_LobbyEquip* UB2LobbyUI_InventoryMain::GetEquipItemIconOfID(int64 ItemInstanceID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_GetEquipItemIconOfID);
	for (int32 EII = 0; EII < static_cast<int32>(EItemEquipPlace::EIEP_End); ++EII)
	{
		UB2DynItemIcon_LobbyEquip* ThisOne = CreatedEquipItemIcons[EII];
		if (ThisOne && ThisOne->GetItemInstanceUID() == ItemInstanceID)
		{
			return ThisOne;
		}
	}

	return NULL;
}

void UB2LobbyUI_InventoryMain::UpdateAllItemIconsForCurrentSelection()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_UpdateAllItemIconsForCurrentSelection);
	UpdateStoredItemIconsForCurrentSelection();
	UpdateEquipItemIconsForCurrentSelection();
}

void UB2LobbyUI_InventoryMain::UpdateStoredItemIconsForCurrentSelection()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_UpdateStoredItemIconsForCurrentSelection);
	//DestroyAllItemRows(); // 扁粮俊 父甸菌带 芭 力芭何磐
	//if (ItemIconRowWidgetClass == NULL || ItemDisplayPanelNativeRef == NULL) {
	//	return;
	//}

	//TArray<FB2Item> CurrPCItemList;
	//// 搬惫 static 牢 GetStoredItemList 绰 荤侩窍瘤 给窍摆焙.
	////check(CachedInventory); 眉农吧瘤 臼绰促. 康旷包府 UI 官差搁辑 固贸 霖厚登瘤 臼篮 惑炔俊辑 咯扁 甸绢坷霸 瞪 荐 乐澜.
	//if (CachedInventory)
	//{
	//	CachedInventory->GetStoredItemList_ItemOpModeFilter(CurrPCItemList, CurSelectedItemInvenTab, EPCClass::EPC_End, true);
	//}

	//CachedLevelupItemIngredsCount = CurrPCItemList.Num();
	//if (CurrPCItemList.Num() == 0)
	//{
	//	return;
	//}

	//const int32 InventoryRowMaxCount = DynItemIconRowMaxCount::DefaultInven;
	//const int32 RequiredRowNum = (FMath::Max(0, CurrPCItemList.Num() - 1) / InventoryRowMaxCount) + 1;
	//for (int RI = 0; RI < RequiredRowNum; ++RI)
	//{
	//	UB2DynItemIconRow_LobbyInven* NewItemRow = CreateWidget<UB2DynItemIconRow_LobbyInven>(GetOwningPlayer(), ItemIconRowWidgetClass);
	//	if (NewItemRow == NULL)
	//	{
	//		continue;
	//	}

	//	UScrollBoxSlot* AddedRowSlot = Cast<UScrollBoxSlot>(ItemDisplayPanelNativeRef->AddChild(NewItemRow));

	//	// 夸 PanelSlot 捞 乐绢具 困摹 殿狼 技泼捞 登绰单.. ScrollBoxSlot 捞搁 鞘夸茄 霸 迭洒 绝阑淀..?
	//	if (AddedRowSlot)
	//	{
	//	}

	//	// NewItemRow 绰 咯傈洒 CanvasPanel 荐霖捞绊 咯扁俊 角力 酒捞袍 酒捞能甸阑 媚持绰促.
	//	NewItemRow->UpdateItemIcons(CurrPCItemList, RI * InventoryRowMaxCount, this);

	//	// 捞率 ScrollBox 勤甸傅阑 困茄 Sender/Receiver 殿废
	//	TArray<UB2DynItemIcon_LobbyInven*> AllCreatedICons;
	//	NewItemRow->GetAllCreatedIcons(AllCreatedICons);
	//	for (UB2DynItemIcon_LobbyInven* ThisIcon : AllCreatedICons)
	//	{
	//		if (ThisIcon)
	//		{
	//			ThisIcon->SetupManualScrollBoxSender_byReceiver(this, ItemDisplayPanelNativeRef);
	//		}
	//	}
	//	CreatedItemRows.Add(NewItemRow);
	//}

	//TUTORIAL // Tutorial Hardcoding
	//TutorialHighlightCheck();
}

void UB2LobbyUI_InventoryMain::UpdateEquipItemIconsForCurrentSelection()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_UpdateEquipItemIconsForCurrentSelection);
	DestroyAllEquipItemIcons();

	UB2UIManager_Lobby* DJLegacyUIMgr = CachedLobbyGM ? CachedLobbyGM->DJLegacy_GetLobbyUIManager() : nullptr;
	// 咯扁辑狼 酒捞袍 酒捞能 widget class 绰 谍单辑档 曼炼且 老捞 乐绢辑 漂喊洒 B2UIManager_Lobby 芭媚辑 啊廉柯促
	UClass* EquipItemIconWidgetClass = DJLegacyUIMgr ? DJLegacyUIMgr->GetInventoryItemIconClass(true) : nullptr;

	if (EquipItemIconWidgetClass == NULL)
	{
		return;
	}

	TArray<FB2Item> CurrPCEquipItemList;
	// 肺厚 牢亥配府俊辑 泅犁 急琶等 PC 俊 措秦 啊廉坷扁 困秦 EPC_End 甫 荤侩沁绰单 static 窃荐甫 荤侩窍瘤父 
	// 荤角惑 LobbyInventory 啊 积己登绢 乐绰 惑怕俊辑唱 泅犁 急琶等 PC 啊 力措肺 牢侥捞 瞪 巴.
	UB2LobbyInventory::GetAllEquippedItem(CurrPCEquipItemList, EPCClass::EPC_End);

	for (UImage* ThisNoEquipIcon : NoEquipIcons)
	{ // 厘馒茄 镑俊 措秦 见扁绰 扒 酒贰俊辑..
		if (ThisNoEquipIcon) {
			ThisNoEquipIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	APlayerController* OwningPC = GetOwningPlayer();

	for (int32 EI = 0; EI < CurrPCEquipItemList.Num(); ++EI)
	{
		FB2Item& CurrEquip = CurrPCEquipItemList[EI];

		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(EquipItemIconWidgetClass->GetDefaultObject());

		// 汲摹茄 canvas panel 困俊 EquipItemIcon 阑 积己.

		// MainPanelNativeRef 困俊 弥辆利牢 酒捞袍 酒捞能 widget 阑 积己窍绰单.. WidgetPool 俊辑 烙措瞒 拌距
		if (DJLegacyUIMgr)
		{
			UB2DynItemIcon_LobbyEquip* NewIcon =
				Cast<UB2DynItemIcon_LobbyEquip>(DJLegacyUIMgr->RentInvenDynItemIconWidget(true, OwningPC, this, nullptr)); // ParentPanel 阑 咯扁辑 逞败林瘤 富绊.. 老窜 烙措瞒 拌距 饶俊

			if (NewIcon)
			{
				// 利例茄 困摹俊 冠酒持扁
				PutWidgetInCanvasPanel(this, NewIcon, GetEquipItemIconPanelNativeRef(CurrEquip.EquipPlace), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f),
					GetAllowedSingleEquipItemIconSize(CurrEquip.EquipPlace), FVector2D(0.0f, 0.0f), 0, false);
				NewIcon->SetVisibility(ESlateVisibility::Visible);
				NewIcon->SetEquipItemIconType(ELobbyEquipItemIconType::EEIIT_LobbyInven);
				NewIcon->UpdateItemData(CurrEquip); // 俺喊 酒捞袍 沥焊.
				SetEquipItemIcon(CurrEquip.EquipPlace, NewIcon);

				UImage* NoEquipIconRef = GetNoEquipIconImageForEquipPlace(CurrEquip.EquipPlace);
				if (NoEquipIconRef) {
					NoEquipIconRef->SetVisibility(ESlateVisibility::Hidden); // 厘馒茄 镑篮 NoEquip 惑怕俊辑 唱坷绰 扁夯 酒捞能阑 见变促.
				}
			}
		}
	}

	for (int EquipPlaceNum = 0; EquipPlaceNum < MaxEquipPlace; ++EquipPlaceNum)
	{
		EItemEquipPlace EquipPlace = static_cast<EItemEquipPlace>(EquipPlaceNum);
		UB2DynItemIcon_LobbyEquip* EquipIcon = GetEquipItemIcon(EquipPlace);
		if (EquipIcon)
		{
			if (EquipSlotButtons[EquipPlaceNum])
				EquipSlotButtons[EquipPlaceNum]->SetVisibility(ESlateVisibility::Collapsed);
			EquipIcon->OnClickDetailInfo.BindLambda([this, EquipPlace]() {
				NativeSetInvenTabSelection(B2UIUtil::GetItemEquipPlaceToItemInventoryType(EquipPlace));
			});
		}
		else
		{
			if (EquipSlotButtons[EquipPlaceNum])
				EquipSlotButtons[EquipPlaceNum]->SetVisibility(ESlateVisibility::Visible);
		}
	}

	//UB2LobbyInventory::SetChangeNewItemTofalse(AllItemsArray[II].InstanceUID);//惶酒霖酒捞袍篮 false肺 官层林磊
}

void UB2LobbyUI_InventoryMain::TryUpdateSingleItemIconOfID(int64 ItemInstanceUID)
{
	FB2Item FoundItemData; // ItemInstanceUID 俊 秦寸窍绰 货肺款 单捞磐 沥档绰 咯扁 窃荐 牢磊肺 焊郴临 荐档 乐阑 巴牢单, 扁粮俊 父电巴苞 付蛮啊瘤肺 父甸绢初澜. 眠饶 UI Doc 俊 持霸 登搁 肚 促弗 规侥栏肺.
	if (UB2LobbyInventory::FindFromAll(FoundItemData, ItemInstanceUID))
	{
		UB2DynItemIcon_LobbyInven* FoundIcon = FindSingleItemIcon(ItemInstanceUID);
		if (FoundIcon)
		{
			FoundIcon->UpdateItemData(FoundItemData);
		}
		else
		{ // 厘馒茄 镑档 第廉焊磊.
			UB2DynItemIcon_LobbyEquip* EquippedIcon = GetEquipItemIconOfID(ItemInstanceUID);
			if (EquippedIcon)
			{
				EquippedIcon->UpdateItemData(FoundItemData);
			}
		}
	}
}

UB2DynItemIcon_LobbyInven* UB2LobbyUI_InventoryMain::FindSingleItemIcon(int64 ItemInstanceUID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_FindSingleItemIcon);
	for (UB2DynItemIconRow_LobbyInven* CurrRow : CreatedItemRows)
	{
		UB2DynItemIcon_LobbyInven* FoundIcon = CurrRow ? CurrRow->FindSingleItemIcon(ItemInstanceUID) : NULL;
		if (FoundIcon)
		{
			return FoundIcon;
		}
	}
	return NULL; // 拱沸 绝阑 荐 乐促.
}

UB2DynItemIcon_LobbyInven* UB2LobbyUI_InventoryMain::FindSingleItemIconFromIndex(int32 iIndexX, int32 iIndexY)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_FindSingleItemIconFromIndex);

	if (CreatedItemRows.IsValidIndex(iIndexX))
	{
		return CreatedItemRows[iIndexX]->FindSingleItemIconFromIndex(iIndexY);
	}

	return NULL;
}

void UB2LobbyUI_InventoryMain::DestroyAllItemRows()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_DestroyAllItemRows);
	for (int32 II = 0; II < CreatedItemRows.Num(); ++II)
	{
		if (CreatedItemRows[II])
		{
			CreatedItemRows[II]->DestroySelf();
		}
	}
	CreatedItemRows.Empty();
}

void UB2LobbyUI_InventoryMain::DestroyAllEquipItemIcons()
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_DestroyAllEquipItemIcons);

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UB2UIManager_Lobby* DJLegacyUIMgr = LobbyGM ? LobbyGM->DJLegacy_GetLobbyUIManager() : nullptr;

	for (int32 II = 0; II < (int32)EItemEquipPlace::EIEP_End; ++II)
	{
		UB2DynItemIcon_LobbyEquip* ThisOne = CreatedEquipItemIcons[II];
		if (ThisOne)
		{
			ThisOne->OnClickDetailInfo.Unbind();
			if (DJLegacyUIMgr)
			{
				DJLegacyUIMgr->TurnInDynItemIconWidget(true, ThisOne);
			}
			else
			{
				ThisOne->DestroySelf();
			}
			CreatedEquipItemIcons[II] = NULL;
		}
	}
}

FVector2D UB2LobbyUI_InventoryMain::GetAllowedSingleEquipItemIconSize(EItemEquipPlace InEquipPlace)
{
	UCanvasPanel* PlacePanel = GetEquipItemIconPanelNativeRef(InEquipPlace);
	UCanvasPanelSlot* PanelSlot = PlacePanel ? Cast<UCanvasPanelSlot>(PlacePanel->Slot) : NULL;
	if (PanelSlot)
	{
		return PanelSlot->GetSize(); // 傈眉 荤捞令 弊措肺
	}
	return FVector2D(0.0f, 0.0f);
}

void UB2LobbyUI_InventoryMain::DestroySelf()
{
	Super::DestroySelf();

	//摧阑锭档 New好促绊焊郴霖促 罐阑 镑捞 绝栏聪 咯扁辑 流立 官层霖促
	RequestNewItem();

	UnsubscribeEvents();

	BladeIIGameImpl::GetClientDataStore().SaveLocalData();

	if (HeroMgmtBase)
	{
		HeroMgmtBase->DestroySelf();
	}

	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	if (ItemInfoTable)
	{ // 牢亥配府 轰俊辑 单捞磐甸捞 腹捞 阶看阑 荐 乐栏聪 厘馒俊 鞘夸茄 芭 力寇窍绊 老窜 攫肺靛
		ItemInfoTable->UnloadAllExceptCurrentLocalEquipPartData();
	}

	//	BladeIIGameImpl::GetRedDotManager().RemoveAllNewItemInInvenTab(PrevSelectedPCClass, CurSelectedItemInvenTab);
	//	BladeIIGameImpl::GetRedDotManager().SaveMarkForNewData();

	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(EquipEffectPlayTH);

	UB2LobbyInventory::SetChangeNewItemTofalseTap(CurSelectedPCClass, CurSelectedItemInvenTab); //瘤陛 焊绊乐绰酒捞袍 new沥焊 false肺 背眉

	DestroyAllEquipItemIcons();
	DestroyAllItemRows();
}

void UB2LobbyUI_InventoryMain::SetItemOpModeGuideText(const FText& InText)
{
	if (TB_ItemOpModeGuide.IsValid())
	{
		TB_ItemOpModeGuide->SetVisibility(ESlateVisibility::HitTestInvisible);
		TB_ItemOpModeGuide->SetText(InText);
	}
}

void UB2LobbyUI_InventoryMain::HideItemOpModeGuideText()
{
	if (TB_ItemOpModeGuide.IsValid())
	{
		TB_ItemOpModeGuide->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2LobbyUI_InventoryMain::OnHeroMgmtPCChanged(EPCClass InSelectedPCClass)
{
	//BladeIIGameImpl::GetRedDotManager().RemoveAllNewItemInInvenTab(PrevSelectedPCClass, CurSelectedItemInvenTab);
	if (bFirstHeroChange == false)
	{
		RequestNewItem();			//徘喘矾林搁 焊郴林绊
	}
	else
	{
		bFirstHeroChange = false;
	}

	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(InSelectedPCClass);
	UpdateAllItemIconsForCurrentSelection();

	if (HeroMgmtBase && CachedCharacterStore)
	{
		FPCItemContainer &ItemContainer = CachedCharacterStore->GetUserAllItems();
		int32 PresetID = ItemContainer.GetCurrentItemPresetID(InSelectedPCClass);

		HeroMgmtBase->SetPresetMode(EInventoryPresetMode::EIPM_Equipment);
		HeroMgmtBase->SwitchPresetState((EItemPresetSlotType)PresetID);
	}

	CurSelectedPCClass = InSelectedPCClass;

	RedDotCondition_TapState();
}

void UB2LobbyUI_InventoryMain::RegisterUIMarkForRedDot()
{
	//	BladeIIGameImpl::GetRedDotManager().AddUI(EUICategoryMarkForNew::Item, this);
}

void UB2LobbyUI_InventoryMain::UnRegisterUIMarkForRedDot()
{
	//	BladeIIGameImpl::GetRedDotManager().RemoveUI(EUICategoryMarkForNew::Item, this);
}

void UB2LobbyUI_InventoryMain::DoMarkRedDot()
{
	if (CachedLobbyGM)
	{
		EPCClass CurrPCClass = CachedLobbyGM->GetHeroMgmtModePCClass();
		/*
		if (IMG_NewItemCheck_Weapon.IsValid())
		{
		IMG_NewItemCheck_Weapon->SetVisibility(
		BladeIIGameImpl::GetRedDotManager().ShouldMarkForNewItem(CurrPCClass, EItemInvenType::EIIVT_Weapon) ?
		ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden
		);
		}
		if (IMG_NewItemCheck_Protection.IsValid())
		{
		IMG_NewItemCheck_Protection->SetVisibility(
		BladeIIGameImpl::GetRedDotManager().ShouldMarkForNewItem(CurrPCClass, EItemInvenType::EIIVT_Protection) ?
		ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden
		);
		}
		if (IMG_NewItemCheck_Accessory.IsValid())
		{
		IMG_NewItemCheck_Accessory->SetVisibility(
		BladeIIGameImpl::GetRedDotManager().ShouldMarkForNewItem(CurrPCClass, EItemInvenType::EIIVT_Accessory) ?
		ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden
		);
		}
		*/
	}

	if (HeroMgmtBase)
	{
		HeroMgmtBase->DoMarkRedDot();

	}
}

void UB2LobbyUI_InventoryMain::UpdateInvenControlButtonState()
{
	// 脚痹 UI 俊辑绰 捞繁 版快 UI Doc 俊辑 函版 捞亥飘 罐酒辑 贸府且 荐 乐绰单 咯扁急 流立 函版 捞亥飘甫 朝妨林绢具 窃. 
	// 概 Tick 俊辑 捞繁 贸府甫 粱 茄促 秦档 攫府倔 扁夯 官牢爹 扁瓷捞尔 喊馆 促甫 芭 绝澜.

	BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_UpdateInvenControlButtonState);
	if (BTN_ContainerExpansion.IsValid())
	{
		const bool bPossibleToInvokeContainerExpansionMenu = IsPossibleToInvokeContainerExpansionMenu();
		BTN_ContainerExpansion->SetIsEnabled(bPossibleToInvokeContainerExpansionMenu);
		// 搬惫 visibility 档 波林扁肺..
		BTN_ContainerExpansion->SetVisibility(bPossibleToInvokeContainerExpansionMenu ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	const bool bPossibleToInvokeSubMenu = IsPossibleToInvokeSubMenu();

	if (BTN_SellAll.IsValid())
		BTN_SellAll->SetVisibility(bPossibleToInvokeSubMenu ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (BTN_AutoEnhancement.IsValid())
		BTN_AutoEnhancement->SetVisibility(bPossibleToInvokeSubMenu ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (BTN_AutoEquip.IsValid())
		BTN_AutoEquip->SetVisibility(bPossibleToInvokeSubMenu ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (BTN_AutoSelect.IsValid())
	{
		BTN_AutoSelect->SetVisibility(IsInventoryModeLevelup() && !bPossibleToInvokeSubMenu && IsPossibleToAutoSelctSubMenu() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		UpdateAutoSelectButton();
	}

	if (BTN_AutoDecomposition.IsValid())
		BTN_AutoDecomposition->SetVisibility(bPossibleToInvokeSubMenu ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (HeroMgmtBase)
	{
		HeroMgmtBase->SetInventoryFullText();
	}

	if (CP_ContainerSizeTextSet.IsValid())
	{	// 家葛前 徘俊辑绰 焊包俺荐尔 焊包窃农扁 钎矫档 窍瘤 臼扁肺 窃.
		CP_ContainerSizeTextSet->SetVisibility(CurSelectedItemInvenTab != EItemInvenType::EIIVT_Consumables ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UB2LobbyUI_InventoryMain::UpdateAutoSelectButton()
{
	if (BTN_AutoSelect.IsValid())
	{
		if (BTN_AutoSelect->GetVisibility() != ESlateVisibility::Collapsed)
			BTN_AutoSelect->SetIsEnabled(CachedInventory ? CachedInventory->GetCurrentSelectIngredItemNum() < UB2LobbyInventory::GetItemLevelupIngredLimit() : false);
	}
}

bool UB2LobbyUI_InventoryMain::IsPossibleToInvokeSellAll()
{
	bool AllEquiped = CachedInventory ? CachedInventory->IsIngredsAllItemEquiped(CurSelectedPCClass, CurSelectedItemInvenTab) : false;
	bool AllLock = CachedInventory ? CachedInventory->IsIngredsAllItemLock(CurSelectedPCClass, CurSelectedItemInvenTab) : false;
	bool PossibleLevelup = CachedInventory ? CachedInventory->HasPossibleLevelupItem(CurSelectedPCClass, CurSelectedItemInvenTab) : false;

	bool Result = false;
	if (PossibleLevelup && AllLock == false && AllEquiped == false)
		Result = true;

	return Result;
}

bool UB2LobbyUI_InventoryMain::IsPossibleToInvokeAutoEnhancement()
{
	bool AllEquiped = CachedInventory ? CachedInventory->IsIngredsAllItemEquiped(CurSelectedPCClass, CurSelectedItemInvenTab) : false;
	bool AllLock = CachedInventory ? CachedInventory->IsIngredsAllItemLock(CurSelectedPCClass, CurSelectedItemInvenTab) : false;
	bool HasSameItem = CachedInventory ? CachedInventory->HasIngredsSameGrade(CurSelectedPCClass, CurSelectedItemInvenTab) : false;
	bool AllMaxLevels = CachedInventory ? CachedInventory->IsIngredsAllMaxLevel(CurSelectedPCClass, CurSelectedItemInvenTab) : false;

	bool Result = false;
	if (HasSameItem == true && AllMaxLevels == false && AllLock == false && AllEquiped == false)
		Result = true;

	return Result;
}

FText UB2LobbyUI_InventoryMain::GetAutoSelectButtonText() const
{
	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AutoSelect")));
}

FText UB2LobbyUI_InventoryMain::GetContainerSizeText() const
{
	return FText::FromString(FString::Printf(TEXT("%d"), CachedInventory->GetItemContainerSize(CurSelectedItemInvenTab)));
}

FText UB2LobbyUI_InventoryMain::GetStoredItemNumText() const
{
	return FText::FromString(FString::Printf(TEXT("%d"), CachedInventory->GetStoredItemNumInUnitedSlot(CurSelectedItemInvenTab)));
}

FSlateColor UB2LobbyUI_InventoryMain::GetStoredItemNumTextColor() const
{
	return CachedInventory->IsInventoryOverLimit(CurSelectedItemInvenTab) ? StoredItemNumTextColor_Over : StoredItemNumTextColor_Normal;
}

FText UB2LobbyUI_InventoryMain::GetSellAllButtonText() const
{
	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_SellAll")));
}

FText UB2LobbyUI_InventoryMain::GetAutoLevelupButtonText() const
{
	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AutoItemLevelup")));
}

FText UB2LobbyUI_InventoryMain::GetAutoEquipmentButtonText() const
{
	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AutoEquip")));
}

void UB2LobbyUI_InventoryMain::OnItemInvenTabButtonPressed(EItemInvenType InSelectedInvenTab)
{
	if (InSelectedInvenTab == CachedSelectedItemInvenTab)
		return;

	if (InSelectedInvenTab != EItemInvenType::EIIVT_End)
	{
		RequestNewItem();			//徘喘矾林搁 焊郴林绊

		CurSelectedItemInvenTab = InSelectedInvenTab;
		CachedSelectedItemInvenTab = InSelectedInvenTab;

		check(CachedInventory);
		CachedInventory->SetMainInvenViewCurrentTab(CurSelectedItemInvenTab); // 促弗 UI 俊辑 粱 镜老捞 乐绢辑

		LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal(); // 钦己 铰鞭 殿 葛靛矫 咆胶飘 诀单捞飘 秦 林绢具 且 惑炔捞 粱 乐澜.
		UpdateInvenTabWidgetVisibilityState();
		UpdateStoredItemIconsForCurrentSelection();

		if (HeroMgmtBase)
		{
			HeroMgmtBase->SetPresetMode(EInventoryPresetMode::EIPM_Equipment);
		}

		if (CurSelectedItemInvenTab == EItemInvenType::EIIVT_Consumables)
		{
			// 盔贰 家葛前篮 贸澜俊 啊廉客具 窍绰单.. 吝埃俊 粱 巩力啊 积败辑 咯扁档 犬牢 瞒..
			if (HeroMgmtBase)
			{
				HeroMgmtBase->SetHidePresetControls();
			}

			BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_End); //EPC_End 肺 林搁 傍侩 酒捞袍阑 夸没且 巴.
		}

		UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
	}
}

void UB2LobbyUI_InventoryMain::OnContainerExpansionButtonPressed()
{
	if (IsPossibleToInvokeContainerExpansionMenu())
	{	// 焊包窃 犬厘 皋春 pop-up
		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_ExtendInventory, FDJLegacySubpopupAddPanelInfo());
	}
}

void UB2LobbyUI_InventoryMain::OnSellAllButtonPressed()
{
	if (IsPossibleToInvokeSubMenu())
	{
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(EquipEffectPlayTH);
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(EquipEffectPlayTH, this, &UB2LobbyUI_InventoryMain::OnDelayCallbackSellAll, 0.1f, false);
	}
}

void UB2LobbyUI_InventoryMain::OnAutoSelectButtonPressed()
{
	if (CachedInventory == nullptr)
		return;

	const int32 StarGrade1 = 1;
	const int32 StarGrade2 = 2;
	const int32 StarGrade3 = 3;
	bool isEnable1 = CachedInventory->GetAvailableAutoSelectItemCount(StarGrade1) > 0 ? true : false;
	bool isEnable2 = CachedInventory->GetAvailableAutoSelectItemCount(StarGrade2) > 0 ? true : false;
	bool isEnable3 = CachedInventory->GetAvailableAutoSelectItemCount(StarGrade3) > 0 ? true : false;

	UB2UIManager::GetInstance()->OpenUI(UIFName::LevelupAutoSelect);
	UB2UIManager::GetInstance()->GetUI<UB2UILevelupAutoSelect>(UIFName::LevelupAutoSelect)->SetSelectState(isEnable1, isEnable2, isEnable3);

	UB2UIManager::GetInstance()->OpenUI(UIFName::LevelupAutoSelect);
	auto* UIAutoLevelUp = UB2UIManager::GetInstance()->GetUI<UB2UILevelupAutoSelect>(UIFName::LevelupAutoSelect);
	UIAutoLevelUp->SetSelectedItemPart(CachedInventory);
}

void UB2LobbyUI_InventoryMain::OnAutoLevelupButtonPressed()
{
	if (IsPossibleToInvokeSubMenu())
	{
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(EquipEffectPlayTH);
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(EquipEffectPlayTH, this, &UB2LobbyUI_InventoryMain::OnDelayCallbackAutoLevelUp, 0.1f, false);
	}
}

void UB2LobbyUI_InventoryMain::OnAutoEquipButtonPressed()
{
	if (IsPossibleToInvokeSubMenu())
	{
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(EquipEffectPlayTH);
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(EquipEffectPlayTH, this, &UB2LobbyUI_InventoryMain::OnDelayCallbackAutoEquip, 0.1f, false);
	}
}

void UB2LobbyUI_InventoryMain::OnAutoDecompositionButtonPressed()
{
	if (IsPossibleToInvokeSubMenu())
	{
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(EquipEffectPlayTH);
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(EquipEffectPlayTH, this, &UB2LobbyUI_InventoryMain::OnDelayCallbackAutoDecomposition, 0.1f, false);
	}
}

void UB2LobbyUI_InventoryMain::OnDelayCallbackSellAll()
{
	if (IsPossibleToInvokeSubMenu())
	{// 葛滴 迫扁 皋春 pop-up
		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_SellAllItems, FDJLegacySubpopupAddPanelInfo());
	}
}

void UB2LobbyUI_InventoryMain::OnDelayCallbackAutoEquip()
{
	if (IsPossibleToInvokeSubMenu())
	{// 磊悼 厘馒 皋春 pop-up
		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoEquip, FDJLegacySubpopupAddPanelInfo());
	}
}

void UB2LobbyUI_InventoryMain::OnDelayCallbackAutoLevelUp()
{
	if (IsPossibleToInvokeSubMenu())
	{// 磊悼 饭骇诀 皋春 pop-up
		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelup, FDJLegacySubpopupAddPanelInfo());
	}
}

void UB2LobbyUI_InventoryMain::OnDelayCallbackAutoDecomposition()
{
	if (IsPossibleToInvokeSubMenu())
	{// 磊悼 盒秦 pop-up
		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_DecompositionAllItems, FDJLegacySubpopupAddPanelInfo());
	}
}

void UB2LobbyUI_InventoryMain::TutorialHighlightCheck()
{
	if (CreatedItemRows.Num() <= 0)
		return;

	TutorialManager& TutorialMgr = TutorialManager::GetInstance();

	if (!TutorialMgr.IsTutorialAvailable())
		return;

	if (TutorialMgr.HasAnyTutorial())
	{
		if (UB2UnitedWidgetBase* pBase = FindSingleItemIconFromIndex(0, 0))
			TutorialMgr.SetCachedUIItemIcon(pBase);
	}
}

void UB2LobbyUI_InventoryMain::DeliveryModifyItemNewFlag()
{
	UB2LobbyInventory::SetChangeNewItemTofalseTap(BeforeCurSelectedPCClass, BeforeCurSelectedItemInvenTab);

	if (BeforeCurSelectedItemInvenTab == EItemInvenType::EIIVT_Consumables || CurSelectedItemInvenTab == EItemInvenType::EIIVT_Consumables)
		UpdateStoredItemIconsForCurrentSelection();

	RedDotCondition_TapState();
}

void UB2LobbyUI_InventoryMain::UpdateInvenTabWidgetVisibilityState()
{ // 泅犁 徘 滚瓢 急琶 惑炔俊 蝶弗 滚瓢捞尔 捞固瘤 visible 惑怕 诀单捞飘.

	TAB_Accessory->SetTabSelected(false);
	TAB_Consumables->SetTabSelected(false);
	TAB_Protection->SetTabSelected(false);
	TAB_Weapon->SetTabSelected(false);

	TAB_Accessory->SetIsTabEnable(true);
	TAB_Consumables->SetIsTabEnable(true);
	TAB_Protection->SetIsTabEnable(true);
	TAB_Weapon->SetIsTabEnable(true);

	switch (CurSelectedItemInvenTab)
	{
	case EItemInvenType::EIIVT_Accessory:
		if (TAB_Accessory.IsValid())
		{
			TAB_Accessory->SetTabSelected(true);
			TAB_Accessory->SetIsTabEnable(false);
		}
		break;
	case EItemInvenType::EIIVT_Consumables:
		if (TAB_Consumables.IsValid())
		{
			TAB_Consumables->SetTabSelected(true);
			TAB_Consumables->SetIsTabEnable(false);
		}
		break;
	case EItemInvenType::EIIVT_Protection:
		if (TAB_Protection.IsValid())
		{
			TAB_Protection->SetTabSelected(true);
			TAB_Protection->SetIsTabEnable(false);
		}
		break;
	case EItemInvenType::EIIVT_Weapon:
		if (TAB_Weapon.IsValid())
		{
			TAB_Weapon->SetTabSelected(true);
			TAB_Weapon->SetIsTabEnable(false);
		}
		break;
	default:
		break;
	}
}

void UB2LobbyUI_InventoryMain::UpdateItemEquipSlotVisible(const bool& bIsVisibleCostumeSlot)
{
	const bool bIsItemEquipSlotVisible = bIsVisibleCostumeSlot ? false : true;

	PlayChangeAnimEquipSlotOrCostumeSlot_BP(bIsItemEquipSlotVisible);
}

void UB2LobbyUI_InventoryMain::OnClickTabWeapon()
{
	OnItemInvenTabButtonPressed(EItemInvenType::EIIVT_Weapon);
}

void UB2LobbyUI_InventoryMain::OnClickTabProtection()
{
	OnItemInvenTabButtonPressed(EItemInvenType::EIIVT_Protection);
}

void UB2LobbyUI_InventoryMain::OnClickTabAccessory()
{
	OnItemInvenTabButtonPressed(EItemInvenType::EIIVT_Accessory);
}

void UB2LobbyUI_InventoryMain::OnClickTabConsumables()
{
	OnItemInvenTabButtonPressed(EItemInvenType::EIIVT_Consumables);
}

void UB2LobbyUI_InventoryMain::NativeSetInvenTabSelection(EItemInvenType NewTabSelection)
{
	OnItemInvenTabButtonPressed(NewTabSelection);
}

bool UB2LobbyUI_InventoryMain::IsPossibleToInvokeSubMenu()
{
	return (CachedInventory && CachedInventory->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_None && CurSelectedItemInvenTab != EItemInvenType::EIIVT_Consumables);
}

bool UB2LobbyUI_InventoryMain::IsPossibleToAutoSelctSubMenu()
{
	bool MaxLevel = (CachedInventory && CachedInventory->IsMaxItemLevelUpExpectedByCurrentSelection());
	bool HasItem = CachedLevelupItemIngredsCount > 0;

	return HasItem && !MaxLevel && CurSelectedItemInvenTab != EItemInvenType::EIIVT_Consumables;
}

bool UB2LobbyUI_InventoryMain::IsPossibleToInvokeContainerExpansionMenu()
{
	const int32 CurrentContainerSize = UB2LobbyInventory::GetItemContainerSize(CurSelectedItemInvenTab);
	int32 CashPerSingleExtend = 30;
	int32 SlotCountPerSingleExtend = 4;
	int32 MaxAllowedSlotCount = 200;
	BladeIIGameImpl::GetClientDataStore().GetInventoryExtendReferenceData(CashPerSingleExtend, SlotCountPerSingleExtend, MaxAllowedSlotCount);

	return (IsPossibleToInvokeSubMenu() && (CurrentContainerSize < MaxAllowedSlotCount));
}

bool UB2LobbyUI_InventoryMain::IsInventoryModeLevelup()
{
	return (CachedInventory && CachedInventory->GetItemOpMode() == ELobbyInvenItemOpMode::EIVIOP_Levelup && CurSelectedItemInvenTab != EItemInvenType::EIIVT_Consumables);
}

void UB2LobbyUI_InventoryMain::AllPlayEquipEffect()
{
	for (int32 i = 0; i < static_cast<int32>(EItemEquipPlace::EIEP_End); ++i)
	{
		PlayEquipEffect(static_cast<EItemEquipPlace>(i));
	}
}

void UB2LobbyUI_InventoryMain::PlayEquipEffect(EItemEquipPlace InEquippedPlace)
{
	// InEquippedPlace 俊 蝶扼.. 弥辆利栏肺绰 硅凯俊辑 波郴档废. 瘤陛篮 抛胶飘肺 Helmet 俊 硅摹茄 巴父.

	UImage* EquipEffectImage = GetEquipEffectImg(InEquippedPlace);
	if (EquipEffectImage)
	{
		EquipEffectImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		UMaterialInstanceDynamic* ThisMID = EquipEffectImage->GetDynamicMaterial();
		if (ThisMID)
		{
			CurrentActiveEquipEffectMID.AddUnique(ThisMID);
		}
	}
	UImage* EquipEffectImage2 = GetEquipEffectImg2(InEquippedPlace);
	if (EquipEffectImage2)
	{
		EquipEffectImage2->SetVisibility(ESlateVisibility::HitTestInvisible);
		UMaterialInstanceDynamic* ThisMID = EquipEffectImage2->GetDynamicMaterial();
		if (ThisMID)
		{
			CurrentActiveEquipEffectMID2.AddUnique(ThisMID);
		}
	}

	BeginEquipEffectParamAnimBP(); // 歹固 image widget 狼 animation 阑 敲饭捞窍霸 瞪 巴.

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(EquipEffectPlayTH);
		TheWorld->GetTimerManager().SetTimer(EquipEffectPlayTH, this, &UB2LobbyUI_InventoryMain::StopEquipEffect, EquipEffectPlayTime, false);
	}
}

void UB2LobbyUI_InventoryMain::UpdateEquipEffectParamAnim()
{
	if (IMG_EquipEffectParamAnimDummy.IsValid())
	{
		// 歹固肺 彬酒初篮 image 俊辑 局聪皋捞飘 登绰 拿矾蔼阑 啊廉客辑 技泼. 葛电 MID 俊 度鞍捞 技泼茄促. 弊巴鳖瘤 促 蝶肺 汲摹窍变 粱 公府老 淀.
		for (auto* ThisMID : CurrentActiveEquipEffectMID)
		{
			BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_UpdateEquipEffectParamAnim_CurrentActiveEquipEffectMID);
			if (!ThisMID)
				continue;

			//FLinearColor ColorParam(IMG_EquipEffectParamAnimDummy->ColorAndOpacity.R, IMG_EquipEffectParamAnimDummy->ColorAndOpacity.G, IMG_EquipEffectParamAnimDummy->ColorAndOpacity.B, 1.0f);
			//ThisMID->SetVectorParameterValue(FName(TEXT("UMG_Color")), ColorParam);
			//ThisMID->SetScalarParameterValue(FName(TEXT("Opacity")), IMG_EquipEffectParamAnimDummy->ColorAndOpacity.A);
		}
	}

	if (IMG_EquipEffectParamAnimDummy2.IsValid())
	{
		for (auto* ThisMID : CurrentActiveEquipEffectMID2)
		{
			BLADE2_SCOPE_CYCLE_COUNTER(UB2LobbyUI_InventoryMain_UpdateEquipEffectParamAnim_CurrentActiveEquipEffectMID2);
			if (!ThisMID)
				continue;

			FLinearColor ColorParam(IMG_EquipEffectParamAnimDummy2->GetColorAndOpacity().R, IMG_EquipEffectParamAnimDummy2->GetColorAndOpacity().G, IMG_EquipEffectParamAnimDummy2->GetColorAndOpacity().B, 1.0f);
			ThisMID->SetVectorParameterValue(FName(TEXT("UMG_Color")), ColorParam);
			ThisMID->SetScalarParameterValue(FName(TEXT("Opacity")), IMG_EquipEffectParamAnimDummy2->GetColorAndOpacity().A);
		}
	}
}

void UB2LobbyUI_InventoryMain::StopEquipEffect()
{
	// 葛电 厘馒 浇吩俊 措秦 茄波锅俊 吝窜矫糯. 泅 矫胶袍狼 茄拌牢单 被捞 蝶肺 倒霸 且 鞘夸档 绝阑 淀.
	for (auto* ThisEquipEffect : EquipEffectImgs)
	{
		if (!ThisEquipEffect)
			continue;

		ThisEquipEffect->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (auto* ThisEquipEffect2 : EquipEffectImgs2)
	{
		if (!ThisEquipEffect2)
			continue;

		ThisEquipEffect2->SetVisibility(ESlateVisibility::Collapsed);
	}

	CurrentActiveEquipEffectMID.Empty(); // 茄波锅俊 厚快绰 吧肺. 局聪皋捞记档 茄波锅俊..
	CurrentActiveEquipEffectMID2.Empty();

	auto* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(EquipEffectPlayTH);
	}
}

void UB2LobbyUI_InventoryMain::RedDotCondition_TapState()
{
	for (int32 i = 0; i <= int32(EItemInvenType::EIIVT_Consumables); ++i)		//家葛前篮 救哆聪 3俺父 倒妨凛聪促
	{
		bool IsNew = BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Inventory(CurSelectedPCClass, static_cast<EItemInvenType>(i));

		NewitemCheckArr[i]->SetVisibility(IsNew ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	}
	DoMarkRedDot();
}

void UB2LobbyUI_InventoryMain::RequestNewItem()
{
	if (BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Inventory(CurSelectedPCClass, CurSelectedItemInvenTab))
	{
		int32 ClassType = CurSelectedItemInvenTab != EItemInvenType::EIIVT_Consumables ? CliToSvrPCClassType(CurSelectedPCClass) : b2network::B2CharacterType::Common;
		data_trader::Retailer::GetInstance().RequestModifyItemNewFlag(ClassType, CliToSvrItemInvenType(CurSelectedItemInvenTab));
		BeforeCurSelectedPCClass = CurSelectedPCClass;
		BeforeCurSelectedItemInvenTab = CurSelectedItemInvenTab;
	}
}

void UB2LobbyUI_InventoryMain::SetEnableAllTabButtons(bool bEnable)
{
	if (TAB_Weapon.IsValid())
	{
		TAB_Weapon->SetIsTabEnable(bEnable);
	}
	if (TAB_Protection.IsValid())
	{
		TAB_Protection->SetIsTabEnable(bEnable);
	}
	if (TAB_Accessory.IsValid())
	{
		TAB_Accessory->SetIsTabEnable(bEnable);
	}
	if (TAB_Consumables.IsValid())
	{
		TAB_Consumables->SetIsTabEnable(bEnable);
	}
}

void UB2LobbyUI_InventoryMain::SetEnableOnlyOneTabType(EItemInvenType InEnableTabType)
{
	if (TAB_Weapon.IsValid())
	{
		TAB_Weapon->SetIsTabEnable(InEnableTabType == EItemInvenType::EIIVT_Weapon ? true : false);
	}
	if (TAB_Protection.IsValid())
	{
		TAB_Protection->SetIsTabEnable(InEnableTabType == EItemInvenType::EIIVT_Protection ? true : false);
	}
	if (TAB_Accessory.IsValid())
	{
		TAB_Accessory->SetIsTabEnable(InEnableTabType == EItemInvenType::EIIVT_Accessory ? true : false);
	}
	if (TAB_Consumables.IsValid())
	{
		TAB_Consumables->SetIsTabEnable(InEnableTabType == EItemInvenType::EIIVT_Consumables ? true : false);
	}
}

UCanvasPanel* UB2LobbyUI_InventoryMain::GetItemDetailPopupParentPanel(bool bOfLeftSide) const
{
	return bOfLeftSide ? X_LeftPopupPanel.Get() :
		// 快螟扑诀篮 HeroMgmt 傍烹颇飘狼 庆歹颇飘俊辑 唱坷绰 葛靛 舅覆阑 啊府瘤 臼档废 HeroMgmt 率 Panel 阑 荤侩茄促.
		// 咯扁 widget 甸 硅摹 矫 鉴辑 棺 z-order 林狼秦具 窃.
		(HeroMgmtBase ? HeroMgmtBase->GetInfoPopupParentPanel(bOfLeftSide) : X_RightPopupPanel.Get());
}
