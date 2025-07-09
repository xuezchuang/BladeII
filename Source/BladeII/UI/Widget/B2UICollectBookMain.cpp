// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2ClientDataStore.h"
#include "Event.h"
#include "Retailer.h"
#include "B2Inventory.h"
#include "B2ItemInfo.h"
#include "B2UIDocHelper.h"
#include "B2UIMsgPopupReward.h"
#include "B2UICollectBookIcon.h"
#include "B2UICollectBookWing.h"
#include "B2UICollectBookMain.h"
#include "B2UICommonTab.h"


#include "BladeIIGameImpl.h"

// todo delete widget IMG_UnSelectedEquipType
#define GET_COLLECTION_TAB(TabStructure, CollectionTab)

#define PAGE_MAX_ROWLINE 3

int32 GetTotalItemCount(EPCClass TargetClass, ECollectionType CollectionType)
{
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	
	int32 ItemCount = 0;
	TArray<EItemClass> ItemClassList = GetItemClassListByCollectionType(CollectionType);
	for (EItemClass ItemClass : ItemClassList)
		ItemCount += DataStore.GetTotalItemCount(TargetClass, ItemClass);
	
	return ItemCount;
}

int32 GetCollectionItemCount(EPCClass TargetClass, ECollectionType CollectionType)
{
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();

	int32 ItemCount = 0;
	TArray<EItemClass> ItemClassList = GetItemClassListByCollectionType(CollectionType);
	for (EItemClass ItemClass : ItemClassList)
		ItemCount += DataStore.GetCollectionItemCount(TargetClass, ItemClass);
	
	return ItemCount;
}

int32 GetEnhancedCollectionItemCount(EPCClass TargetClass, ECollectionType CollectionType)
{
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();

	int32 ItemCount = 0;
	TArray<EItemClass> ItemClassList = GetItemClassListByCollectionType(CollectionType);
	for (EItemClass ItemClass : ItemClassList)
		ItemCount += DataStore.GetEnhancedCollectionItemCount(TargetClass, ItemClass);
	
	return ItemCount;
}

ECollectionType GetCollectionTypeByItemClass(EItemClass ItemClass)
{
	switch (ItemClass)
	{
	case EItemClass::EIC_SetItem: return ECollectionType::SetItem;
	case EItemClass::EIC_Weapon: return ECollectionType::Weapon;
	case EItemClass::EIC_Helmet :
	case EItemClass::EIC_BodyArmor:
	case EItemClass::EIC_Glove:
	case EItemClass::EIC_Shoes: return ECollectionType::Armor;
	case EItemClass::EIC_Ring :
	case EItemClass::EIC_Necklace:
	case EItemClass::EIC_Belt: return ECollectionType::Accessory;
	case EItemClass::EIC_Wing: return ECollectionType::Wing;
	}

	return ECollectionType::None;
}

FText FCollectionTab::GetTabDisplayString()
{
	FText OrgText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TabLocalizeKey);
	FString ProgressText = FString::Printf(TEXT("(%d / %d)"), CollectCount, MaxCollectCount);

	FText DisplayText = OrgText;
	if (CollectionType != ECollectionType::Wing)
	{
		DisplayText = FText::FromString(OrgText.ToString() + ProgressText);
	}

	return DisplayText;
}

void FCollectionTab::SetAlarmIcon(bool bVisible)
{
	if (IMG_NewIcon.IsValid())
	{
		IMG_NewIcon->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
	if (IMG_NewIconActive.IsValid())
	{
		IMG_NewIconActive->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

void FCollectionTab::SetActive(bool bActive)
{
	bActiveCurrentTab = bActive;
	if (WS_Click.IsValid())
	{
		WS_Click->SetActiveWidgetIndex(bActive ? 1 : 0);
	}
}

void FCollectionTab::InitTabDynamicTemplate(EPCClass CharacterClass)
{
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	
	CollectCount = GetCollectionItemCount(CharacterClass, CollectionType);
	MaxCollectCount = GetTotalItemCount(CharacterClass, CollectionType);

	bool bSetAlarmIcon = GetEnhancedCollectionItemCount(CharacterClass, CollectionType) > 0 ? true : false;
	SetAlarmIcon(bSetAlarmIcon);

	InitTabDefaultTemplate();
}

void FCollectionTab::InitTabDefaultTemplate()
{
	const FText& DisplayText = GetTabDisplayString();

	if (TB_TabName.IsValid())
		TB_TabName->SetText(DisplayText);

	if (TB_TabNameActive.IsValid())
		TB_TabNameActive->SetText(DisplayText);
}


void FSelectedIconInfo::UpdateIconInfo(EItemClass InItemClass, UB2UICollectBookIcon* NewIcon)
{
	if (IconWidget) // 이전 Data는 삭제
	{
		SelectedID = 0;
		ItemClass = EItemClass::EIC_End;
		IconWidget->UpdateSelectState(false);
		IconWidget = nullptr;
	}

	if (NewIcon)
	{
		IconWidget = NewIcon;
		SelectedID = IconWidget->GetSelectedID();
		ItemClass = InItemClass;

		UpdateSelect(true);
	}
}

void FSelectedIconInfo::UpdateSelect(bool bSelect)
{
	if (IconWidget && SelectedID != 0)
		IconWidget->UpdateSelectState(bSelect);
}

void UB2UICollectBookMain::CacheAssets()
{
	Super::CacheAssets();

	/* 최종 UI 확실한 결정이 나지 않았으므로 일단 주석처리 및 CollectionTabList 강제 Visible처리 */
	GET_SLOT(UVerticalBox, VB_CollectionTab);
	if (VB_CollectionTab.IsValid())
		ShowCollectionTabButtonList(true);
	//GET_SLOT(UButton, BTN_CollectionTab);

	GET_SLOT(UTextBlock, TB_CollectionTab);

	GET_SLOT(UCanvasPanel, CP_ItemPage);
	GET_SLOT(UB2ScrollBox, SB_ItemPage);
	if (SB_ItemPage.IsValid())
	{
		SB_ItemPage->SetAnimateScroll(false);
	}

	CollectionTabList.Add(FCollectionTab(ECollectionType::SetItem, TEXT("CollectBook_TabSetItem")));
	CollectionTabList.Add(FCollectionTab(ECollectionType::Weapon, TEXT("CollectBook_TabWeapon")));
	CollectionTabList.Add(FCollectionTab(ECollectionType::Armor, TEXT("CollectBook_TabArmor")));
	CollectionTabList.Add(FCollectionTab(ECollectionType::Accessory, TEXT("CollectBook_TabAccessory")));
	CollectionTabList.Add(FCollectionTab(ECollectionType::Wing, TEXT("CollectBook_TabWing")));

	GET_SLOT_BYNAME(UButton, CollectionTabList[0].BTN_Tab, BTN_SetTab);
	GET_SLOT_BYNAME(UTextBlock, CollectionTabList[0].TB_TabName, TB_SetTab);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CollectionTabList[0].IMG_NewIcon, IMG_NewSetTab);
	GET_SLOT_BYNAME(UWidgetSwitcher, CollectionTabList[0].WS_Click, BTN_Set_Switcher);
	GET_SLOT_BYNAME(UTextBlock, CollectionTabList[0].TB_TabNameActive, TB_SetTab_A);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CollectionTabList[0].IMG_NewIconActive, IMG_NewSetTab_A);

	GET_SLOT_BYNAME(UButton, CollectionTabList[1].BTN_Tab, BTN_WeaponTab);
	GET_SLOT_BYNAME(UTextBlock, CollectionTabList[1].TB_TabName, TB_WeaponTab);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CollectionTabList[1].IMG_NewIcon, IMG_NewWeaponTab);
	GET_SLOT_BYNAME(UWidgetSwitcher, CollectionTabList[1].WS_Click, BTN_Weapon_Switcher);
	GET_SLOT_BYNAME(UTextBlock, CollectionTabList[1].TB_TabNameActive, TB_WeaponTab_A);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CollectionTabList[1].IMG_NewIconActive, IMG_NewWeaponTab_A);

	GET_SLOT_BYNAME(UButton, CollectionTabList[2].BTN_Tab, BTN_ArmorTab);
	GET_SLOT_BYNAME(UTextBlock, CollectionTabList[2].TB_TabName, TB_ArmorTab);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CollectionTabList[2].IMG_NewIcon, IMG_NewArmorTab);
	GET_SLOT_BYNAME(UWidgetSwitcher, CollectionTabList[2].WS_Click, BTN_Armor_Switcher);
	GET_SLOT_BYNAME(UTextBlock, CollectionTabList[2].TB_TabNameActive, TB_ArmorTab_A);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CollectionTabList[2].IMG_NewIconActive, IMG_NewArmorTab_A);

	GET_SLOT_BYNAME(UButton, CollectionTabList[3].BTN_Tab, BTN_AccTab);
	GET_SLOT_BYNAME(UTextBlock, CollectionTabList[3].TB_TabName, TB_AccTab);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CollectionTabList[3].IMG_NewIcon, IMG_NewAccTab);
	GET_SLOT_BYNAME(UWidgetSwitcher, CollectionTabList[3].WS_Click, BTN_Acc_Switcher);
	GET_SLOT_BYNAME(UTextBlock, CollectionTabList[3].TB_TabNameActive, TB_AccTab_A);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CollectionTabList[3].IMG_NewIconActive, IMG_NewAccTab_A);

	GET_SLOT_BYNAME(UButton, CollectionTabList[4].BTN_Tab, BTN_WingTab);
	GET_SLOT_BYNAME(UTextBlock, CollectionTabList[4].TB_TabName, TB_WingTab);
	GET_SLOT_BYNAME(UWidgetSwitcher, CollectionTabList[4].WS_Click, BTN_WingTab_Switcher);
	GET_SLOT_BYNAME(UTextBlock, CollectionTabList[4].TB_TabNameActive, TB_WingTab_A);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CollectionTabList[4].IMG_NewIconActive, IMG_NewWingTab_A);


	for (auto& CollectionTab : CollectionTabList)
		CollectionTab.InitTabDefaultTemplate();

	GET_SLOT(UOverlay, O_ItemName);
	GET_SLOT(UTextBlock, TB_ItemName);
	GET_SLOT(UB2Button, BTN_UnEquip);
	GET_SLOT(UTextBlock, TB_UnEquip);
	GET_SLOT(UVerticalBox, VB_IconRows);
	
	GET_SLOT(UCheckBox, CB_ShowHead);
	
	EquipTypeTabs.AddDefaulted(4);
	GET_SLOT_BYNAME(UB2UICommonTab, EquipTypeTabs[0].UI_CommonTab, UI_EquipCommon_Tab1);
	GET_SLOT_BYNAME(UB2UICommonTab, EquipTypeTabs[1].UI_CommonTab, UI_EquipCommon_Tab2);
	GET_SLOT_BYNAME(UB2UICommonTab, EquipTypeTabs[2].UI_CommonTab, UI_EquipCommon_Tab3);
	GET_SLOT_BYNAME(UB2UICommonTab, EquipTypeTabs[3].UI_CommonTab, UI_EquipCommon_Tab4);
	for (auto& EquipTab : EquipTypeTabs)
		EquipTab.Init();
	
	GET_SLOT(UB2RichTextBlock, RTB_RewardDesc);

	GET_SLOT(UB2Button, BTN_ShowWing);
	GET_SLOT(UTextBlock, TB_ShowWing);
	GET_SLOT(UImage, IMG_ShowWing_BGOn);
	GET_SLOT(UImage, IMG_ShowWing_BGOff);
	GET_SLOT(UImage, IMG_ShowWing_IconOn);
	GET_SLOT(UImage, IMG_ShowWing_IconOff);

	IMG_Stars.AddDefaulted(FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE);
	GET_SLOT_BYNAME(UImage, IMG_Stars[0], IMG_Star1);
	GET_SLOT_BYNAME(UImage, IMG_Stars[1], IMG_Star2);
	GET_SLOT_BYNAME(UImage, IMG_Stars[2], IMG_Star3);
	GET_SLOT_BYNAME(UImage, IMG_Stars[3], IMG_Star4);
	GET_SLOT_BYNAME(UImage, IMG_Stars[4], IMG_Star5);
	GET_SLOT_BYNAME(UImage, IMG_Stars[5], IMG_Star6);

	GET_SLOT(UCanvasPanel, X_CP_WingPagePanel);
	GET_SLOT(UCanvasPanel, X_CP_ItemDetailViewPanel);
	
	CharacterChangeButtons.AddDefaulted(GetMaxPCClassNum());
	GET_SLOT_BYNAME(UButton,	CharacterChangeButtons[0].BTN_Area, BTN_Gladiator);
	GET_SLOT_BYNAME(UImage,		CharacterChangeButtons[0].IMG_CharacterSelected, IMG_GladiatorSelected);
	GET_SLOT_BYNAME(UImage,		CharacterChangeButtons[0].IMG_CharacterUnSelected, IMG_GladiatorUnSelected);
	GET_SLOT_BYNAME(UTextBlock, CharacterChangeButtons[0].TB_ClassName, TB_GladiatorClass);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CharacterChangeButtons[0].IMG_NewIcon, IMG_GladiatorNew);
	GET_SLOT_BYNAME(UButton,	CharacterChangeButtons[1].BTN_Area, BTN_Assassin);
	GET_SLOT_BYNAME(UImage,		CharacterChangeButtons[1].IMG_CharacterSelected, IMG_AssassinSelected);
	GET_SLOT_BYNAME(UImage,		CharacterChangeButtons[1].IMG_CharacterUnSelected, IMG_AssassinUnSelected);
	GET_SLOT_BYNAME(UTextBlock, CharacterChangeButtons[1].TB_ClassName, TB_AssassinClass);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CharacterChangeButtons[1].IMG_NewIcon, IMG_AssassinNew);
	GET_SLOT_BYNAME(UButton,	CharacterChangeButtons[2].BTN_Area, BTN_Wizard);
	GET_SLOT_BYNAME(UImage,		CharacterChangeButtons[2].IMG_CharacterSelected, IMG_WizardSelected);
	GET_SLOT_BYNAME(UImage,		CharacterChangeButtons[2].IMG_CharacterUnSelected, IMG_WizardUnSelected);
	GET_SLOT_BYNAME(UTextBlock, CharacterChangeButtons[2].TB_ClassName, TB_WizardClass);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CharacterChangeButtons[2].IMG_NewIcon, IMG_WizardNew);
	GET_SLOT_BYNAME(UButton,	CharacterChangeButtons[3].BTN_Area, BTN_Fighter);
	GET_SLOT_BYNAME(UImage,		CharacterChangeButtons[3].IMG_CharacterSelected, IMG_FighterSelected);
	GET_SLOT_BYNAME(UImage,		CharacterChangeButtons[3].IMG_CharacterUnSelected, IMG_FighterUnSelected);
	GET_SLOT_BYNAME(UTextBlock, CharacterChangeButtons[3].TB_ClassName, TB_FighterClass);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CharacterChangeButtons[3].IMG_NewIcon, IMG_FighterNew);
	
	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	InitCharacterButtons();
}


void UB2UICollectBookMain::BindDelegates()
{
	Super::BindDelegates();

	//BIND_CLICK_FUNC_TO_BTN(BTN_CollectionTab, &UB2UICollectBookMain::OnClickBTN_CollectionTab);
	BIND_CLICK_FUNC_TO_BTN(CollectionTabList[0].BTN_Tab, &UB2UICollectBookMain::OnClickBTN_SetTab);
	BIND_CLICK_FUNC_TO_BTN(CollectionTabList[1].BTN_Tab, &UB2UICollectBookMain::OnClickBTN_WeaponTab);
	BIND_CLICK_FUNC_TO_BTN(CollectionTabList[2].BTN_Tab, &UB2UICollectBookMain::OnClickBTN_ArmorTab);
	BIND_CLICK_FUNC_TO_BTN(CollectionTabList[3].BTN_Tab, &UB2UICollectBookMain::OnClickBTN_AccTab);
	BIND_CLICK_FUNC_TO_BTN(CollectionTabList[4].BTN_Tab, &UB2UICollectBookMain::OnClickBTN_WingTab);

	BIND_CLICK_FUNC_TO_BTN(BTN_UnEquip, &UB2UICollectBookMain::OnClickBTN_UnEquip);
	BIND_CLICK_FUNC_TO_BTN(BTN_ShowWing, &UB2UICollectBookMain::OnClickBTN_ShowWing);

	BIND_CLICK_FUNC_TO_BTN(CharacterChangeButtons[0].BTN_Area, &UB2UICollectBookMain::OnClickBTN_Gladiator);
	BIND_CLICK_FUNC_TO_BTN(CharacterChangeButtons[1].BTN_Area, &UB2UICollectBookMain::OnClickBTN_Assassin);
	BIND_CLICK_FUNC_TO_BTN(CharacterChangeButtons[2].BTN_Area, &UB2UICollectBookMain::OnClickBTN_Wizard);
	BIND_CLICK_FUNC_TO_BTN(CharacterChangeButtons[3].BTN_Area, &UB2UICollectBookMain::OnClickBTN_Fighter);

	if (CB_ShowHead.IsValid())
		CB_ShowHead->OnCheckStateChanged.AddDynamic(this, &UB2UICollectBookMain::OnShowHeadButtonPressed);
}

void UB2UICollectBookMain::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_UnEquip.IsValid()) 
		TB_UnEquip->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_UnEquip")));

	SetHeaderTitle(CharacterClass);
}

void UB2UICollectBookMain::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	SetHeaderTitle(CharacterClass);
}

void UB2UICollectBookMain::NativeConstruct()
{
	Super::NativeConstruct();
	OnClickCollectionTab(ECollectionType::SetItem, true);
}

void UB2UICollectBookMain::Init()
{
	Super::Init();
	SubscribeEvent();

	UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome)
		OnCharacterChange(DocSome->GetCollectBookSelectClass());

	SelectedIconInfos.AddDefaulted(static_cast<int32>(EItemClass::EIC_SetItem) + 1);
	CurrentEquipItems.AddDefaulted(static_cast<int32>(EItemClass::EIC_SetItem) + 1);
	SetEquipItems.AddDefaulted(static_cast<int32>(EItemClass::EIC_SetItem) + 1);

	UpdateShowWingButton(true);
}

void UB2UICollectBookMain::DestroySelf(UB2UIManager* InUIManager)
{
	DestroyItemDetailViewUIP();
	DestroyWingPageUIP();

	for(UB2UICollectBookIconRow* IconRowWidget : ItemIconRows)
		if (IconRowWidget != nullptr)
			IconRowWidget->DestroySelf(InUIManager);

	FreeupSomeMemory();

	BladeIIGameImpl::GetClientDataStore().RequestSyncToServerShowHead(); //머리 Visible 관련 서버 동기화
	UnSubscribeEvent();
	Super::DestroySelf(InUIManager);
}


UB2UICollectBookMain::UB2UICollectBookMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrevCollectionType = ECollectionType::None;
	CurrentCollectionType = ECollectionType::SetItem;
	CurrentItemClass = EItemClass::EIC_SetItem;

	CharacterClass = EPCClass::EPC_End;
	
	bShowDetailView = false;
	bShowWing = true;
	EquippedWingGrade = 0;

	bDirtyRequestGetCollection = false;

	DeliveryRewardCollectionItemTicket = 0;
	DeliveryRewardCollectionSetItemTicket = 0;

	CreatedUIP_ItemDetailView = nullptr;
	CreatedUIP_WingPage = nullptr;
	CachedScrollOffset = 0.f;
}

void UB2UICollectBookMain::OnClickedItemIcon(class UB2UICollectBookIcon* IconWidget)
{
	if (IconWidget)
	{
		bool bRequestReward = (IconWidget->GetCollectionState() == EItemCollectionState::Enhanced);
		if (bRequestReward)
		{
			// 보상일때는 요청만
			RequestCollectionReward(CurrentCollectionType, IconWidget->GetSelectedID());
			return;
		}

		HideDetailView();

		EItemClass ItemClass = IconWidget->GetSelectedItemClass();
		FSelectedIconInfo CurrentSelectedIcon;
		if (GetSelectIconInfo(ItemClass, CurrentSelectedIcon))
		{
			int32 NewSelectedID = IconWidget->GetSelectedID();
			
			bool bEquip = (NewSelectedID && NewSelectedID != CurrentSelectedIcon.SelectedID);
			if (bEquip)
				SetSelectIconInfo(ItemClass, IconWidget);
			
			else
				SetSelectIconInfo(ItemClass, nullptr);	// 이전에 선택했던 아이콘이라면

			OnUpdateLobbyCharacterView(ItemClass, NewSelectedID, bEquip);
		}
	}
}

void UB2UICollectBookMain::OnClickedDetailViewIcon(class UB2UICollectBookIcon* IconWidget)
{
	if (IconWidget)
	{
		const int32 NewSelectedID = IconWidget->GetSelectedID();
			
		if (bShowDetailView == false)
		{
			if (CurrentCollectionType == ECollectionType::SetItem)
				ShowSetDetailView(NewSelectedID);
			else
				ShowItemDetailView(NewSelectedID);
		}
		
		else
			HideDetailView();
	}
}

void UB2UICollectBookMain::OnClickedWingGradeButton(int32 WingGrade)
{
	EquippedWingGrade = WingGrade;
	OnUpdateLobbyCharacterViewWing(EquippedWingGrade, bShowWing);
}


void UB2UICollectBookMain::SetCharacterClass(EPCClass InCharacterClass)
{
	CharacterClass = InCharacterClass;

	UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome)
		DocSome->SetCollectBookSelectClass(InCharacterClass);
}

const TArray<FB2Item>& UB2UICollectBookMain::GetCollectBookCurrentEquipItems()
{
	if (CurrentCollectionType == ECollectionType::SetItem || 
		(CurrentCollectionType == ECollectionType::Wing && PrevCollectionType == ECollectionType::SetItem) )
		return SetEquipItems;

	return CurrentEquipItems;
}

void UB2UICollectBookMain::LinkUICollectBook(EPCClass InCharacterClass, EItemClass InItemClass)
{
	OnCharacterChange(InCharacterClass);
	ECollectionType LinkCollectionType = GetCollectionTypeByItemClass(InItemClass);

	OnClickCollectionTab(LinkCollectionType, true);

	if (LinkCollectionType == ECollectionType::Wing)
	{
		FB2Wing WingData;
		
		if (BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InCharacterClass, WingData))
		{
			ConditionalCreateWingPageUIP();
			if (CreatedUIP_WingPage)
			{
				CreatedUIP_WingPage->OnClickedWingGrade(WingData.EvolutionGrade);
			}
		}
	}
}

void UB2UICollectBookMain::UpdateSingleIconRow(class UB2UICollectBookIconRow* IconRowWidget, const TArray<int32>& ItemIDs, EItemClass InItemClass)
{
	if (IconRowWidget)
	{
		int32 LocalizeNameItemID = ItemIDs.IsValidIndex(0) ? ItemIDs[0] : 0;
		IconRowWidget->SetRowTitleText(GetIconRowText(InItemClass, LocalizeNameItemID));

		bool bExistSelectedIcon = false;
		FSelectedIconInfo SelectedIcon;
		if (GetSelectIconInfo(InItemClass, SelectedIcon))
		{
			bExistSelectedIcon = SelectedIcon.IsSelectedIcon();
		}
		
		for (int32 Grade = 1; Grade <= FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; Grade++)
			IconRowWidget->ClearCollectBookIcon(Grade);	// 일단 Clear 후 다시 셋팅

		for (int32 ID : ItemIDs)
		{
			int32 Grade = 0;
			if (InItemClass == EItemClass::EIC_SetItem)
				Grade = GetSetGradeFromUniqueKey(ID);
			else
				Grade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(ID);

			IconRowWidget->SetCollectBookIconInfo(InItemClass, Grade, ID);

			if (bExistSelectedIcon && SelectedIcon.SelectedID == ID)
				SelectedIcon.UpdateSelect(true);
		}
	}
}


FText UB2UICollectBookMain::GetIconRowText(EItemClass ItemClass, int32 ID)
{
	if (ItemClass == EItemClass::EIC_SetItem)
	{
		static FText SetNameFormat(FText::FromString(TEXT("{0} {1}")));

		const int32 GroupID = GetSetGroupIDFromUniqueKey(ID);
		return GetLOCTextOfSetItemName(GroupID);
	}
	else
	{
		FSingleItemInfoData* ItemInfo = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(ID) : nullptr;
		if (ItemInfo)
		{
			return ItemInfo->GetLocalizedName();
		}
	}

	return FText::FromString(TEXT("Unknown"));
}

void UB2UICollectBookMain::SetHeaderTitle(EPCClass InCharacterClass)
{
	FText CharacterText = GetPCClassLocalizedText(InCharacterClass);
	SetLobbyUIHeaderTitle(FText::Format(FText::FromString(TEXT("{0} {1}")), CharacterText, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook"))));
}

void UB2UICollectBookMain::ShowCollectionTabButtonList(bool bVisible)
{
	if (VB_CollectionTab.IsValid())
		VB_CollectionTab->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UB2UICollectBookMain::InitIconRowWidget(UB2UICollectBookIconRow* IconRowWidget)
{
	if (IconRowWidget)
	{
		IconRowWidget->Init();
		IconRowWidget->SetOwnerCollectBookMainUI(this);
	}
}

UB2UICollectBookIconRow* UB2UICollectBookMain::GetOrCreateIconRowWidget(int32 RowIndex)
{
	if (ItemIconRows.IsValidIndex(RowIndex) && ItemIconRows[RowIndex] != nullptr)
		return ItemIconRows[RowIndex];

	UB2UICollectBookIconRow* IconRowWidget = CreateWidget<UB2UICollectBookIconRow>(GetOwningPlayer(), IconRowClassBP);
	if (IconRowWidget != nullptr)
	{
		InitIconRowWidget(IconRowWidget);
		ItemIconRows.Add(IconRowWidget);

		if (SB_ItemPage.IsValid())
			SB_ItemPage->AddChild(IconRowWidget);
	}

	return IconRowWidget;
}

bool UB2UICollectBookMain::GetCollectionTab(ECollectionType CollectionType, FCollectionTab& OutTab)
{
	for (const auto& Tab : CollectionTabList)
	{
		if (Tab.CollectionType == CollectionType)
		{
			OutTab = Tab;
			return true;
		}
	}

	return false;
}


FCollectionTab& UB2UICollectBookMain::GetCollectionTab(ECollectionType CollectionType)
{
	check(CollectionType != ECollectionType::None && CollectionType != ECollectionType::Max);

	const int32 TabIndex = static_cast<int32>(CollectionType) - 1;
	return CollectionTabList[TabIndex];
}

void UB2UICollectBookMain::InitCollectionTab(EPCClass InCharacterClass)
{
	for (auto& Tab : CollectionTabList)
	{
		Tab.InitTabDynamicTemplate(InCharacterClass);
	}
}

void UB2UICollectBookMain::OnClickCollectionTab(ECollectionType NewCollectionType, bool bForce/*= false*/)
{
	CachedScrollOffset = 0.f;
	if (bForce == false && CurrentCollectionType == NewCollectionType)
		return;

	PrevCollectionType = CurrentCollectionType;
	CurrentCollectionType = NewCollectionType;

	OnChangedCollectionTab(PrevCollectionType, CurrentCollectionType);
	UpdateEquipItemByChangeTab(PrevCollectionType, CurrentCollectionType);

	bool bVisibleWingPage = CurrentCollectionType == ECollectionType::Wing ? true : false;
	UpdateWingPage(bVisibleWingPage);

	UpdateCharacterButton(CharacterClass);
	UpdateCollectionTab(CurrentCollectionType);

	if (RTB_RewardDesc.IsValid())
	{
		FText RewardDescText;
		if (NewCollectionType == ECollectionType::SetItem)
			RewardDescText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_SetRewardDescription"));
		else
			RewardDescText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_RewardDescription"));

		RTB_RewardDesc->SetText(RewardDescText);
	}
}

void UB2UICollectBookMain::OnChangedCollectionTab(ECollectionType PrevCollection, ECollectionType NewCollection)
{
	FCollectionTab& NewTab = GetCollectionTab(NewCollection);
	NewTab.SetActive(true);

	if (NewCollection != PrevCollection)
	{
		FCollectionTab& PrevTab = GetCollectionTab(PrevCollection);
		PrevTab.SetActive(false);
	}
}

void UB2UICollectBookMain::UpdateCollectionTab(ECollectionType CollectionType, EItemClass FocusItemClass)
{
	FCollectionTab& NewTab = GetCollectionTab(CollectionType);
	// 1. CollectionTab New & CollectionCount 갱신
	NewTab.InitTabDynamicTemplate(CharacterClass);

	// 2. CollectionTab 아래 계층 EquipTypeTab 갱신
	UpdateAllEquipTypeTabs(CollectionType, FocusItemClass);
}

void UB2UICollectBookMain::UpdateEquipItemByChangeTab(ECollectionType PrevCollection, ECollectionType NewCollection)
{
	// 탭 이동간 장비 착용 조건은 하드코딩성격이 짙어질 가능성이 있으므로 따로 관리
	if (PrevCollection != NewCollection)
	{
		// SetTab -> WingTab 진입시에만 Set장비 유지, 무기/방어구/장신구 Tab이동시 세트장비 해제
		if (PrevCollection == ECollectionType::SetItem && NewCollection != ECollectionType::Wing)
		{
			OnUpdateLobbyCharacterView(EItemClass::EIC_SetItem, CurrentEquipSetItemKey, false);
		}
	}
}

void UB2UICollectBookMain::GetSortedSetItemUniqueKeys(TArray<int32>& OutUniqueKeys, EPCClass TargetClass)
{
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	DataStore.GetSetItemUniqueKeys(CharacterClass, OutUniqueKeys);

	OutUniqueKeys.Sort([](int32 SetKey1, int32 SetKey2)->bool{
		if (SetKey1 < SetKey2)
			return true;
		return false;
	});
}

TArray<int32> UB2UICollectBookMain::GetIconRowItemIDs(EItemClass InItemClass, int32 GroupID)
{
	TArray<int32> SortedItemIDs;

	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	DataStore.GetClassItemTemplateIDs(SortedItemIDs, CharacterClass, InItemClass, GroupID);

	return SortedItemIDs;
}

bool AllowedSetItemGroupID(const ECollectionType CollectionType, const EItemClass ItemClass, const int32 GroupID)
{
	if (CollectionType != ECollectionType::Accessory)
	{
		switch (GroupID)
		{
		case b2network::B2GroupPrefix::GROWTH:
			return false;
		case b2network::B2GroupPrefix::DUEL:
			return false;
		//case b2network::B2GroupPrefix::ETERNAL:
		//	return false;
		case b2network::B2GroupPrefix::NON_EQUIP:
			return false;

		default:
			return true;
		}
	}

	return true;
}

void UB2UICollectBookMain::UpdateCollectionPage(ECollectionType CollectionType, EItemClass InItemClass)
{
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	TArray<int32> GroupIDs = DataStore.GetItemGroupIDs();
	TArray<int32> CostumeGroupIDs = DataStore.GetCostumeItemGroupIDs();
	
	//Costume 제외
	GroupIDs.RemoveAll([&CostumeGroupIDs](int32 ID) 
	{
		return CostumeGroupIDs.Contains(ID);
	});

	// 임시 코드
	GroupIDs.Remove(5);
	GroupIDs.Remove(6);

	GroupIDs.Add(5);
	GroupIDs.Add(6);
	

	int32 BlockedRowCount = 0;
	const int32 IconRowCount = GroupIDs.Num();	//  GroupID와 IconRow는 1:1매칭된다.
	for (int32 RowIdx = 0; RowIdx < IconRowCount; RowIdx++)
	{
		const int32 GroupID = GroupIDs[RowIdx];

		if (AllowedSetItemGroupID(CollectionType, InItemClass, GroupID) == false)
		{
			++BlockedRowCount;
			continue;
		}

		UB2UICollectBookIconRow* IconRowWidget = GetOrCreateIconRowWidget(RowIdx);
		
		if (IconRowWidget && GroupID != 0)
		{
			UpdateSingleIconRow(IconRowWidget, GetIconRowItemIDs(InItemClass, GroupID), InItemClass);
			IconRowWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	// 이전에 생성되었으나 남은 IconRowWidget은 접어버리자
	for (int32 InvisibleIdx = IconRowCount - BlockedRowCount; InvisibleIdx < ItemIconRows.Num(); InvisibleIdx++)
	{
		if (ItemIconRows[InvisibleIdx] != nullptr)
			ItemIconRows[InvisibleIdx]->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 이전에 선택했던 SetItem이 있었다면 복원시켜줌
	FSelectedIconInfo SelectedIcon;
	if (InItemClass == EItemClass::EIC_SetItem && GetSelectIconInfo(InItemClass, SelectedIcon))
	{
		CurrentEquipSetItemKey = SelectedIcon.SelectedID;
		OnUpdateLobbyCharacterView(EItemClass::EIC_SetItem, CurrentEquipSetItemKey, true);
	}

	if (SB_ItemPage.IsValid())
		SB_ItemPage->SetScrollOffset(CachedScrollOffset);
}

void UB2UICollectBookMain::UpdateWingPage(bool bVisible)
{
	if (bVisible) { // 첨부터 Hide 가 올 수 있는데 그 경우 아직 생성하지 않았다 해도 굳이 생성할 필요 없지.
		ConditionalCreateWingPageUIP();
	}
	if (CreatedUIP_WingPage)
	{
		CreatedUIP_WingPage->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		if (bVisible)
		{
			CreatedUIP_WingPage->SetCharacterClass(CharacterClass);
			CreatedUIP_WingPage->OnClickedWingPage();
			UpdateShowWingButton(bShowWing);
		}
	}

	if (RTB_RewardDesc.IsValid()) 
		RTB_RewardDesc->SetVisibility(bVisible ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	
	if (CP_ItemPage.IsValid())
		CP_ItemPage->SetVisibility(bVisible ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	
	if (BTN_UnEquip.IsValid())
		BTN_UnEquip->SetVisibility(bVisible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	

	bShowWing = bVisible;
	UpdateShowWingButton(bShowWing);
	OnUpdateLobbyCharacterViewWing(EquippedWingGrade, bVisible);
	HideDetailView();
}

void UB2UICollectBookMain::ClearLobbyCharacterView()
{ 
	if (CurrentCollectionType == ECollectionType::SetItem)
	{
		for (auto& SetItems : SetEquipItems)
		{
			if (SetItems.ItemClass != EItemClass::EIC_Wing)
			{
				SetItems.ItemRefID = 0;
				SetItems.StarGrade = 0;
				SetItems.EquipPlace = EItemEquipPlace::EIEP_End;
				SetItems.ItemClass = EItemClass::EIC_End;
			}	
		}
	}

	else
	{
		for (auto& EquipItem : CurrentEquipItems)
		{
			if (EquipItem.ItemClass != EItemClass::EIC_Wing)
			{
				EquipItem.ItemRefID = 0;
				EquipItem.StarGrade = 0;
				EquipItem.EquipPlace = EItemEquipPlace::EIEP_End;
				EquipItem.ItemClass = EItemClass::EIC_End;
			}
		}
	}

	LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Signal(CharacterClass, EItemEquipPlace::EIEP_Weapon);
}

void UB2UICollectBookMain::ClearAllEquipItems()
{
	for (auto& SetItems : SetEquipItems)
	{
		SetItems.ItemRefID = 0;
		SetItems.StarGrade = 0;
		SetItems.EquipPlace = EItemEquipPlace::EIEP_End;
		SetItems.ItemClass = EItemClass::EIC_End;
	}

	for (auto& EquipItem : CurrentEquipItems)
	{
		EquipItem.ItemRefID = 0;
		EquipItem.StarGrade = 0;
		EquipItem.EquipPlace = EItemEquipPlace::EIEP_End;
		EquipItem.ItemClass = EItemClass::EIC_End;
	}

	EquippedWingGrade = 0;
	CurrentEquipSetItemKey = 0;

	LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Signal(CharacterClass, EItemEquipPlace::EIEP_Weapon);
}

void UB2UICollectBookMain::OnUpdateLobbyCharacterView(EItemClass ItemClass, int32 ItemTemplateID, bool bEquip)
{
	switch (ItemClass)
	{
	case EItemClass::EIC_Belt:
	case EItemClass::EIC_Necklace:
	case EItemClass::EIC_Ring:
		// 현재 accessory들은 Mesh표현하지 않음
		CachedSurpassLevel = 0;
		SetCurrentEquipItem(ItemClass, ItemTemplateID, bEquip);
		return;
	}

	if (ItemClass == EItemClass::EIC_SetItem)
	{
		if (bEquip)
		{
			CurrentEquipSetItemKey = ItemTemplateID;
			FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
			FMD_SetInfo* SetItemInfo = DataStore.GetSetItemInfo(CharacterClass, CurrentEquipSetItemKey);
			if (SetItemInfo)
			{
				const TArray<int32>& SetCompositionItems = SetItemInfo->SetCompositionItems;
				for (int32 SetItemID : SetCompositionItems)
				{
					EItemClass SetCompItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(SetItemID);
					SetCurrentEquipSetItem(SetCompItemClass, SetItemID, true);
				}
			}
		}
		else
		{
			for (int32 Idx = 0; Idx < SetEquipItems.Num(); Idx++)
			{
				const int32 NormalItemTemplateID = SetEquipItems[Idx].ItemRefID;
				EItemClass NormalItemClass = static_cast<EItemClass>(Idx);
				SetCurrentEquipSetItem(NormalItemClass, NormalItemTemplateID, false);
			}
		}
	}
	else
	{
		SetCurrentEquipItem(ItemClass, ItemTemplateID, bEquip);
	}
	
	LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Signal(CharacterClass, GetItemEquipPlaceFromClass(ItemClass));
}

void UB2UICollectBookMain::OnUpdateLobbyCharacterViewWing(int32 WingGrade, bool bEquip)
{
	int32 WingEquipIndex = static_cast<int32>(EItemClass::EIC_Wing);
	if (CurrentEquipItems.IsValidIndex(WingEquipIndex))
	{
		CurrentEquipItems[WingEquipIndex].ItemClass = (bEquip && WingGrade > 0) ? EItemClass::EIC_Wing : EItemClass::EIC_End;
		CurrentEquipItems[WingEquipIndex].StarGrade = bEquip ? WingGrade : 0;
	}

	if (SetEquipItems.IsValidIndex(WingEquipIndex))
	{
		SetEquipItems[WingEquipIndex].ItemClass = (bEquip && WingGrade > 0) ? EItemClass::EIC_Wing : EItemClass::EIC_End;
		SetEquipItems[WingEquipIndex].StarGrade = bEquip ? WingGrade : 0;
	}

	// ItemEquipPlace는 아무거나 넣어도 상관없음
	LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Signal(CharacterClass, EItemEquipPlace::EIEP_Weapon);
}

void UB2UICollectBookMain::SetCurrentEquipSetItem(EItemClass ItemClass, int32 ItemTemplateID, bool bEquip)
{
	EItemEquipPlace EquipPlace = GetItemEquipPlaceFromClass(ItemClass);
	int32 EquipIndex = static_cast<int32>(EquipPlace);

	if (SetEquipItems.IsValidIndex(EquipIndex))
	{
		if (bEquip)
		{
			SetEquipItems[EquipIndex].ItemRefID = ItemTemplateID;
			SetEquipItems[EquipIndex].StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(ItemTemplateID);
			SetEquipItems[EquipIndex].ItemClass = ItemClass;
			SetEquipItems[EquipIndex].EquipPlace = EquipPlace;
			SetEquipItems[EquipIndex].Level = 1;
		}

		else
		{
			SetEquipItems[EquipIndex].ItemRefID = 0;
			SetEquipItems[EquipIndex].EquipPlace = EItemEquipPlace::EIEP_End;
		}
	}
}

void UB2UICollectBookMain::SetCurrentEquipItem(EItemClass ItemClass, int32 ItemTemplateID, bool bEquip)
{
	EItemEquipPlace EquipPlace = GetItemEquipPlaceFromClass(ItemClass);
	int32 EquipIndex = static_cast<int32>(EquipPlace);

	if (CurrentEquipItems.IsValidIndex(EquipIndex))
	{
		if (bEquip)
		{
			CurrentEquipItems[EquipIndex].ItemRefID = ItemTemplateID;
			CurrentEquipItems[EquipIndex].StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(ItemTemplateID);
			CurrentEquipItems[EquipIndex].ItemClass = ItemClass;
			CurrentEquipItems[EquipIndex].EquipPlace = EquipPlace;
			CurrentEquipItems[EquipIndex].Level = 1;
			CurrentEquipItems[EquipIndex].EnhanceLevel = 0;
			
			
			FItemSurpassLevel InSurpassInfo = BladeIIGameImpl::GetClientDataStore().GetSurpassLevelData(CurrentEquipItems[EquipIndex].StarGrade);
			if(CurrentEquipItems[EquipIndex].StarGrade>5) CachedSurpassLevel = InSurpassInfo.MaxSurpassLevel;
			else CachedSurpassLevel = 0;
			CurrentEquipItems[EquipIndex].SurpassCount = CachedSurpassLevel;

			CachedItem = CurrentEquipItems[EquipIndex];
		}

		else
		{
			CurrentEquipItems[EquipIndex].ItemRefID = 0;
			CurrentEquipItems[EquipIndex].EquipPlace = EItemEquipPlace::EIEP_End;
		}
	}
}

int32 UB2UICollectBookMain::GetCurrentEquipItemTemplateID(EItemClass ItemClass)
{
	EItemEquipPlace EquipPlace = GetItemEquipPlaceFromClass(ItemClass);
	int32 EquipIndex = static_cast<int32>(EquipPlace);

	if (CurrentEquipItems.IsValidIndex(EquipIndex))
	{
		return CurrentEquipItems[EquipIndex].ItemRefID;
	}

	return 0;
}

void UB2UICollectBookMain::ResetAllClickedIcons()
{
	for (auto& IconInfo : SelectedIconInfos)
	{
		if (IconInfo.IconWidget)
		{
			OnUpdateLobbyCharacterView(static_cast<EItemClass>(IconInfo.ItemClass), IconInfo.SelectedID, false);
			IconInfo.IconInfoClear();
		}
	}
}


void UB2UICollectBookMain::UpdateShowWingButton(bool bShowState)
{
	if (bShowState)
	{
		if (TB_ShowWing.IsValid())
			TB_ShowWing->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_ShowWing")));

		if (IMG_ShowWing_BGOn.IsValid()) IMG_ShowWing_BGOn->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (IMG_ShowWing_BGOff.IsValid()) IMG_ShowWing_BGOff->SetVisibility(ESlateVisibility::Hidden);

		if (IMG_ShowWing_IconOn.IsValid()) IMG_ShowWing_IconOn->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (IMG_ShowWing_IconOff.IsValid()) IMG_ShowWing_IconOff->SetVisibility(ESlateVisibility::Hidden);
	}

	else
	{
		if (TB_ShowWing.IsValid())
			TB_ShowWing->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_HideWing")));

		if (IMG_ShowWing_BGOn.IsValid()) IMG_ShowWing_BGOn->SetVisibility(ESlateVisibility::Hidden);
		if (IMG_ShowWing_BGOff.IsValid()) IMG_ShowWing_BGOff->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (IMG_ShowWing_IconOn.IsValid()) IMG_ShowWing_IconOn->SetVisibility(ESlateVisibility::Hidden);
		if (IMG_ShowWing_IconOff.IsValid()) IMG_ShowWing_IconOff->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

bool UB2UICollectBookMain::GetSelectIconInfo(EItemClass ItemClass, FSelectedIconInfo& OutIconInfo)
{
	int32 Index = static_cast<int32>(ItemClass);
	if (SelectedIconInfos.IsValidIndex(Index))
	{
		OutIconInfo = SelectedIconInfos[Index];
		return true;
	}
	return false;
}

void UB2UICollectBookMain::SetSelectIconInfo(EItemClass ItemClass, UB2UICollectBookIcon* IconInfo)
{
	int32 IconIndex = static_cast<int32>(ItemClass);
	if (SelectedIconInfos.IsValidIndex(IconIndex))
	{
		SelectedIconInfos[IconIndex].UpdateIconInfo(ItemClass, IconInfo);
	}
}

void UB2UICollectBookMain::ClearSelectIconInfo(EItemClass ItemClass)
{
	SetSelectIconInfo(ItemClass, nullptr);
}

void UB2UICollectBookMain::ClearAllSelectIconInfo()
{
	int32 BeginIdx = static_cast<int32>(EItemClass::EIC_Weapon);
	int32 EndIdx = static_cast<int32>(EItemClass::EIC_Wing);

	for (int32 Idx = BeginIdx; Idx <= EndIdx; Idx++)
	{
		SetSelectIconInfo(static_cast<EItemClass>(Idx), nullptr);
	}
}

void UB2UICollectBookMain::ClearIconSelectedImage(EItemClass ItemClass)
{
	FSelectedIconInfo SelectedIconInfo;
	if (GetSelectIconInfo(ItemClass, SelectedIconInfo) && SelectedIconInfo.IconWidget)
	{
		SelectedIconInfo.IconWidget->UpdateSelectState(false);
	}
}

EItemClass UB2UICollectBookMain::GetItemClassByIndex(ECollectionType CollectionType, int32 Index)
{
	switch (CollectionType)
	{
	case ECollectionType::SetItem: return EItemClass::EIC_SetItem;
	case ECollectionType::Weapon: return EItemClass::EIC_Weapon;
	case ECollectionType::Wing: return EItemClass::EIC_Wing;
	case ECollectionType::Armor:
		{
			const int32 ArmorBegin = static_cast<int32>(EItemClass::EIC_Helmet);
			const int32 ArmorEnd = static_cast<int32>(EItemClass::EIC_Shoes);

			if (ArmorEnd - ArmorBegin < Index)
				break;

			return static_cast<EItemClass>(ArmorBegin + Index);
		}
	case ECollectionType::Accessory:
		{
			const int32 AccBegin = static_cast<int32>(EItemClass::EIC_Belt);
			const int32 AccEnd = static_cast<int32>(EItemClass::EIC_Ring);

			if (AccEnd - AccBegin < Index)
				break;

			return static_cast<EItemClass>(AccBegin + Index);
		}
	}

	return EItemClass::EIC_End;
}

int32 UB2UICollectBookMain::GetItemClassTypeCount(ECollectionType CollectionType)
{
	return GetItemClassListByCollectionType(CollectionType).Num();
}

int32 UB2UICollectBookMain::GetEquipTypeTabIndex(EItemClass InItemClass)
{
	for (int32 TabIndex=0; TabIndex < EquipTypeTabs.Num(); TabIndex++)
	{
		if (EquipTypeTabs[TabIndex].ItemClass == InItemClass)
			return TabIndex;
	}

	return INDEX_NONE;
}

void UB2UICollectBookMain::UpdateAllEquipTypeTabs(ECollectionType CollectionType, EItemClass FocusItemClass)
{
	ClearUITabList();
	for (auto& TabInfo : EquipTypeTabs)
	{
		TabInfo.SetVisible(false);	// 일단 다 안보이게
	}
	
	int32 EquipTypeCount = GetItemClassTypeCount(CollectionType);
	for (int32 EquipTypeIdx = 0; EquipTypeIdx < EquipTypeCount; EquipTypeIdx++)
	{
		EquipTypeTabs[EquipTypeIdx].InitEquipTypeTab(CharacterClass, GetItemClassByIndex(CollectionType, EquipTypeIdx));
		EquipTypeTabs[EquipTypeIdx].SetVisible(true);

		if (EquipTypeTabs[EquipTypeIdx].UI_CommonTab.IsValid())
		{
			FTabButtonDelegate TabButtonDelegate;
			TabButtonDelegate.BindLambda([this, EquipTypeIdx](){ 
				OnClickedEquipTypeTab(CurrentCollectionType, EquipTypeTabs[EquipTypeIdx].ItemClass);
			});
			OnRegistTabWidgets(EquipTypeTabs[EquipTypeIdx].UI_CommonTab.Get(), TabButtonDelegate);
		}
	}

	int32 FocusIndex = FocusItemClass != EItemClass::EIC_End ? GetEquipTypeTabIndex(FocusItemClass)  : -1;
	FocusIndex = EquipTypeTabs.IsValidIndex(FocusIndex) ? FocusIndex : 0;

	if (EquipTypeTabs.IsValidIndex(FocusIndex) && EquipTypeTabs[FocusIndex].UI_CommonTab.IsValid())
	{
		EquipTypeTabs[FocusIndex].UI_CommonTab->ForcedClickTab();
	}
}

void UB2UICollectBookMain::OnClickedEquipTypeTab(ECollectionType CollectionType, EItemClass InItemClass)
{
	const int32 ClickedTabIndex = GetEquipTypeTabIndex(InItemClass);
	if (EquipTypeTabs.IsValidIndex(ClickedTabIndex))
	{
		UpdateEquipTypeTab(CollectionType, InItemClass);
		CurrentItemClass = InItemClass;
	}
}

void UB2UICollectBookMain::UpdateEquipTypeTab(ECollectionType CollectionType, EItemClass InItemClass)
{
	ClearIconSelectedImage(CurrentItemClass);

	if (CollectionType == ECollectionType::Wing)
		return;

	const int32 UpdateTabIndex = GetEquipTypeTabIndex(InItemClass);
	if (EquipTypeTabs.IsValidIndex(UpdateTabIndex))
	{
		// 1. EquipTypeTab 아래계층 Page 갱신
		UpdateCollectionPage(CollectionType, InItemClass);
	}
}


void UB2UICollectBookMain::SetItemNameText(EItemClass ItemClass, int32 SelectedID)
{
	if (TB_ItemName.IsValid())
	{
		FText DisplayText;
		if (ItemClass == EItemClass::EIC_SetItem)
			DisplayText = FText::FromString(FString::Printf(TEXT("Set %d"), SelectedID));

		else
		{
			FSingleItemInfoData* ItemInfo = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(SelectedID) : nullptr;
			if (ItemInfo)
				DisplayText = ItemInfo->GetLocalizedName();
		}
		
		TB_ItemName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TB_ItemName->SetText(DisplayText);
	}
}

void UB2UICollectBookMain::SetStarGrade(int32 StarGrade)
{
	for (int32 StarIdx = 0; StarIdx < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; StarIdx++)
	{
		ESlateVisibility ThisVisibility = (StarIdx + 1 <= StarGrade) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
		if (IMG_Stars.IsValidIndex(StarIdx) && IMG_Stars[StarIdx].IsValid())
		{
			IMG_Stars[StarIdx]->SetVisibility(ThisVisibility);
		}
	}
}

void UB2UICollectBookMain::ShowItemDetailView(int32 ItemTemplateID)
{
	ConditionalCreateItemDetailViewUIP();

	if (CreatedUIP_ItemDetailView && ItemTemplateID)
	{
		CreatedUIP_ItemDetailView->OpenPopup();
		CreatedUIP_ItemDetailView->SetDetailViewSingleItem(ItemTemplateID, CharacterClass,CachedSurpassLevel, CachedItem);
		CreatedUIP_ItemDetailView->SetCollectBookIcon(CurrentItemClass, ItemTemplateID);
		FSelectedIconInfo IconInfo;
		if (GetSelectIconInfo(CurrentItemClass, IconInfo) && IconInfo.IconWidget)
		{
			CreatedUIP_ItemDetailView->SetItemIconDefaultTemplate(IconInfo.IconWidget);
		}

		bShowDetailView = true;
	}
}

void UB2UICollectBookMain::ShowSetDetailView(int32 SetUniqueKey)
{
	ConditionalCreateItemDetailViewUIP();

	if (CreatedUIP_ItemDetailView && SetUniqueKey)
	{
		CreatedUIP_ItemDetailView->OpenPopup();
		CreatedUIP_ItemDetailView->SetDetailViewSetAbility(SetUniqueKey, CharacterClass);

		FSelectedIconInfo IconInfo;
		if (GetSelectIconInfo(CurrentItemClass, IconInfo) && IconInfo.IconWidget)
		{
			CreatedUIP_ItemDetailView->SetItemIconDefaultTemplate(IconInfo.IconWidget);
		}

		bShowDetailView = true;
	}
}

void UB2UICollectBookMain::HideDetailView()
{
	if (CreatedUIP_ItemDetailView)
	{
		CreatedUIP_ItemDetailView->SetVisibility(ESlateVisibility::Hidden);
		bShowDetailView = false;
	}
}

void UB2UICollectBookMain::InitCharacterButtons()
{
	for (int32 Index = 0; Index < CharacterChangeButtons.Num(); Index++)
	{
		auto& CharBtn = CharacterChangeButtons[Index];
		EPCClass ClassType = IntToPCClass(Index);
		
		if (CharBtn.TB_ClassName.IsValid())
			CharBtn.TB_ClassName->SetText(GetPCClassLocalizedText(ClassType));

		UpdateCharacterButton(ClassType);
	}
}

void UB2UICollectBookMain::UpdateCharacterButton(EPCClass InCharacterClass)
{
	const int32 CharacterIdx = PCClassToInt(InCharacterClass);
	if (CharacterChangeButtons.IsValidIndex(CharacterIdx))
	{
		FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
		CharacterChangeButtons[CharacterIdx].SetAlarmIcon(DataStore.HasEnhancedCollectionItem(InCharacterClass));
	}
}

void UB2UICollectBookMain::OnCharacterChange(EPCClass ChangeCharacterClass)
{
	if (CharacterClass != ChangeCharacterClass)
	{
		UpdateCharacterChange(ChangeCharacterClass);
		ClearAllSelectIconInfo();

		ClearAllEquipItems();

		FreeupSomeMemory();

		InitCollectionTab(ChangeCharacterClass);
		OnClickCollectionTab(CurrentCollectionType, true);
		SetHeaderTitle(ChangeCharacterClass);
	}
}

void UB2UICollectBookMain::UpdateCharacterChange(EPCClass NewCharacterClass)
{
	int32 CurrIdx = PCClassToInt(CharacterClass);
	if (CharacterChangeButtons.IsValidIndex(CurrIdx))
		CharacterChangeButtons[CurrIdx].SetSelect(false);

	int32 NewIdx = PCClassToInt(NewCharacterClass);
	if (CharacterChangeButtons.IsValidIndex(NewIdx))
		CharacterChangeButtons[NewIdx].SetSelect(true);

	OnUpdateCharacterChangeBP(CharacterClass, NewCharacterClass);

	ClearLobbyCharacterView();
	SetCharacterClass(NewCharacterClass);
	if (CB_ShowHead.IsValid())
		CB_ShowHead->SetIsChecked(BladeIIGameImpl::GetLocalCharacterData().IsShowHeadItem(NewCharacterClass));
	LobbyCharCollectBookSelectClass<EPCClass>::GetInstance().Signal(NewCharacterClass);
}

void UB2UICollectBookMain::RequestCollectionReward(int32 ItemTemplateID)
{
	data_trader::Retailer::GetInstance().RequestRewardCollectionItem(ItemTemplateID);
}

void UB2UICollectBookMain::RequestCollectionReward(ECollectionType CollectionType, int32 RequestID)
{
	if (CollectionType == ECollectionType::SetItem)
	{
		const int32 GroupID = GetSetGroupIDFromUniqueKey(RequestID);
		const int32 Grade = GetSetGradeFromUniqueKey(RequestID);
		EPCClass RequestClass = GetSetCharacterClassFromUniqueKey(RequestID);
		const int32 SvrRequestClass = CliToSvrPCClassType(RequestClass);

		data_trader::Retailer::GetInstance().RequestRewardCollectionSetItem(SvrRequestClass, Grade, GroupID);
	}

	else
	{
		RequestCollectionReward(RequestID);
	}
}

void UB2UICollectBookMain::RequestSetCollectionReward(int32 GroupID, int32 CharacterType, int32 Grade)
{
	data_trader::Retailer::GetInstance().RequestRewardCollectionSetItem(CharacterType, Grade, GroupID);
}

bool UB2UICollectBookMain::IsEquippedWing()
{
	return EquippedWingGrade > 0 ? true : false;
}

void UB2UICollectBookMain::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UICollectBookMain);

	DeliveryRewardCollectionItemTicket = DeliveryRewardCollectionItemClass<FB2RewardCollectionItemPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryRewardCollectionItem, FB2RewardCollectionItemPtr CollectionReward)
		Capture->OnReceiveRewardCollectionItem(CollectionReward);
	END_CAPTURE_OBJECT()
		);

	DeliveryRewardCollectionSetItemTicket = DeliveryRewardCollectionSetItemClass<FB2RewardCollectionSetItemPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryRewardCollectionSetItem, FB2RewardCollectionSetItemPtr CollectionReward)
		Capture->OnReceiveRewardCollectionSetItem(CollectionReward);
	END_CAPTURE_OBJECT()
		);

	LinkUICollectBookTicket = LinkUICollectBookClass<EPCClass, EItemClass>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LinkUICollectBook, EPCClass LinkCharacterClass, EItemClass LinkItemClass)
		Capture->LinkUICollectBook(LinkCharacterClass, LinkItemClass);
	END_CAPTURE_OBJECT()
		);
}

void UB2UICollectBookMain::UnSubscribeEvent()
{
	DeliveryRewardCollectionItemClass<FB2RewardCollectionItemPtr>::GetInstance().Unsubscribe(DeliveryRewardCollectionItemTicket);
	DeliveryRewardCollectionSetItemClass<FB2RewardCollectionSetItemPtr>::GetInstance().Unsubscribe(DeliveryRewardCollectionSetItemTicket);
	LinkUICollectBookClass<EPCClass, EItemClass>::GetInstance().Unsubscribe(LinkUICollectBookTicket);
}

/*
	보상은 도감 Page에서만 Request / Response 가능
*/
void UB2UICollectBookMain::OnReceiveRewardCollectionItem(FB2RewardCollectionItemPtr RewardInfo)
{
	if (RewardInfo != nullptr)
	{
		const int32 ItemTemplateID = RewardInfo->item_template_id;
		b2network::B2RewardPtr RewardPtr = RewardInfo->received_reward;

		EPCClass ReceiveCharacterClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(ItemTemplateID);

		FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
		DataStore.OnReceiveCollectionReward(ReceiveCharacterClass, ItemTemplateID, RewardPtr);

		TArray<b2network::B2RewardPtr> RewardArr;
		RewardArr.Add(RewardPtr);

		if (auto* RewardMailPopUp = UB2UIManager::GetInstance()->OpenRewardMailPopUp(RewardArr))
		{
			/*RewardMailPopUp->SetConfirmCallback(FMsgPopupOnClick::CreateLambda([this]() { this->OnClickMsgReward(); }));*/
		}
		else
		{
			if (UB2UIMsgPopupReward* RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty()))
			{
				RewardPopup->AddRewardItems(RewardArr);
				/*RewardPopup->SetConfirmCallback(FMsgPopupOnClick::CreateLambda([this]() { this->OnClickMsgReward(); }));*/
				RewardPopup->ShowRewardPopup();
			}
		}
		
		if (SB_ItemPage.IsValid())
			CachedScrollOffset = SB_ItemPage->GetScrollOffset();

		EItemClass RewardItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(ItemTemplateID);
		UpdateCharacterButton(ReceiveCharacterClass);
		UpdateCollectionTab(GetCollectionTypeByItemClass(RewardItemClass), RewardItemClass);
	}
}

void UB2UICollectBookMain::OnReceiveRewardCollectionSetItem(FB2RewardCollectionSetItemPtr RewardInfo)
{
	if (RewardInfo != nullptr)
	{
		const int32 GroupID = RewardInfo->gropup_id;
		const int32 Grade = RewardInfo->grade;
		const int32 CharacterType = RewardInfo->character_type;

		const EPCClass ReceiveCharacterClass = SvrToCliPCClassType(CharacterType);
		const int32 SetUniqueKey = GetSetItemUniqueKey(GroupID, ReceiveCharacterClass, Grade);
		
		b2network::B2RewardPtr RewardPtr = RewardInfo->received_reward;
		FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
		DataStore.OnReceiveSetCollectionReward(ReceiveCharacterClass, SetUniqueKey, RewardPtr);

		TArray<b2network::B2RewardPtr> RewardArr;
		RewardArr.Add(RewardPtr);

		if (auto* RewardMailPopUp = UB2UIManager::GetInstance()->OpenRewardMailPopUp(RewardArr))
		{
			/*RewardMailPopUp->SetConfirmCallback(FMsgPopupOnClick::CreateLambda([this]() { this->OnClickMsgReward(); }));*/
		}
		else
		{
			if (UB2UIMsgPopupReward* RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty()))
			{
				RewardPopup->AddRewardItems(RewardArr);
				/*RewardPopup->SetConfirmCallback(FMsgPopupOnClick::CreateLambda([this]() { this->OnClickMsgReward(); }));*/
				RewardPopup->ShowRewardPopup();
			}
		}

		if (SB_ItemPage.IsValid())
			CachedScrollOffset = SB_ItemPage->GetScrollOffset();

		UpdateCharacterButton(ReceiveCharacterClass);
		UpdateCollectionTab(ECollectionType::SetItem, EItemClass::EIC_SetItem);
	}
}

void UB2UICollectBookMain::ConditionalCreateItemDetailViewUIP()
{
	if (!CreatedUIP_ItemDetailView && X_CP_ItemDetailViewPanel.IsValid())
	{
		CreatedUIP_ItemDetailView = DynLoadAndCreateInCanvasPanelFull<UB2UICollectBookItemDetail>(UIP_ItemDetailViewClass, this, X_CP_ItemDetailViewPanel.Get());
		if (CreatedUIP_ItemDetailView)
		{
			CreatedUIP_ItemDetailView->Init();
			FOnClickDetailClose DetailCloseDelegate;
			DetailCloseDelegate.BindUObject(this, &UB2UICollectBookMain::HideDetailView);
			CreatedUIP_ItemDetailView->SetDetailViewCloseDelegate(DetailCloseDelegate);
		}
	}
}
void UB2UICollectBookMain::ConditionalCreateWingPageUIP()
{
	if (!CreatedUIP_WingPage && X_CP_WingPagePanel.IsValid())
	{
		CreatedUIP_WingPage = DynLoadAndCreateInCanvasPanelFull<UB2UICollectBookWing>(UIP_WingPageClass, this, X_CP_WingPagePanel.Get());
		if (CreatedUIP_WingPage)
		{
			CreatedUIP_WingPage->Init();
			FOnClickedCollectionWingGrade WingGradeDelegate;
			WingGradeDelegate.BindUObject(this, &UB2UICollectBookMain::OnClickedWingGradeButton);
			CreatedUIP_WingPage->SetClickWingGradeDelegate(WingGradeDelegate);
		}
	}
}
void UB2UICollectBookMain::DestroyItemDetailViewUIP()
{
	if (CreatedUIP_ItemDetailView)
	{
		CreatedUIP_ItemDetailView->DestroySelf(UB2UIManager::GetInstance());
		CreatedUIP_ItemDetailView = nullptr;
	}
}
void UB2UICollectBookMain::DestroyWingPageUIP()
{
	if (CreatedUIP_WingPage)
	{
		CreatedUIP_WingPage->DestroySelf(UB2UIManager::GetInstance());
		CreatedUIP_WingPage = nullptr;
	}
}

void UB2UICollectBookMain::OnClickBTN_CollectionTab()
{
	if (VB_CollectionTab.IsValid())
	{
		bool bVisible = VB_CollectionTab->GetVisibility() == ESlateVisibility::Hidden ? true : false;
		ShowCollectionTabButtonList(bVisible);
	}
}

void UB2UICollectBookMain::OnClickBTN_SetTab()
{
	OnClickCollectionTab(ECollectionType::SetItem);
}

void UB2UICollectBookMain::OnClickBTN_WeaponTab()
{
	OnClickCollectionTab(ECollectionType::Weapon);
}

void UB2UICollectBookMain::OnClickBTN_ArmorTab()
{
	OnClickCollectionTab(ECollectionType::Armor);
}

void UB2UICollectBookMain::OnClickBTN_AccTab()
{
	OnClickCollectionTab(ECollectionType::Accessory);	
}

void UB2UICollectBookMain::OnClickBTN_WingTab()
{
	OnClickCollectionTab(ECollectionType::Wing);
}

void UB2UICollectBookMain::OnClickBTN_UnEquip()
{
	ClearAllSelectIconInfo();
	ClearLobbyCharacterView();
}

void UB2UICollectBookMain::OnShowHeadButtonPressed(bool IsCheck)
{
	bool CurrentShowHead = BladeIIGameImpl::GetLocalCharacterData().IsShowHeadItem(CharacterClass);
	if (CurrentShowHead != IsCheck)
	{
		BladeIIGameImpl::GetLocalCharacterData().SetCacheShowHeadItem(CharacterClass, IsCheck);
		LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Signal(CharacterClass, EItemEquipPlace::EIEP_Weapon);
	}
}

void UB2UICollectBookMain::OnClickBTN_ShowWing()
{
	bShowWing ^= true;

	UpdateShowWingButton(bShowWing);

	if (bShowWing && EquippedWingGrade == 0)
		OnClickedWingGradeButton(1);

	OnUpdateLobbyCharacterViewWing(EquippedWingGrade, bShowWing);
}

void UB2UICollectBookMain::OnClickBTN_Gladiator()
{
	OnCharacterChange(EPCClass::EPC_Gladiator);
}

void UB2UICollectBookMain::OnClickBTN_Assassin()
{
	OnCharacterChange(EPCClass::EPC_Assassin);
}

void UB2UICollectBookMain::OnClickBTN_Wizard()
{
	OnCharacterChange(EPCClass::EPC_Wizard);
}

void UB2UICollectBookMain::OnClickBTN_Fighter()
{
	OnCharacterChange(EPCClass::EPC_Fighter);
}

void UB2UICollectBookMain::FreeupSomeMemory()
{
	B2_SCOPED_TRACK_LOG_L2(TEXT("UB2UICollectBookMain::FreeupSomeMemory"));

	//// 도감 뷰에서 이것저것 구경하고 갈아끼면서 순식간에 메모리가 증가할 수 있어서 좀 무리해서라도 필요없는 것들을 자주 해제시킬 필요가 있다.

	//UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	//if (ItemInfoTable)
	//{ // 현재 로컬 캐릭터 장착 분 제외하고 다 unload mark 하는 거
	//	ItemInfoTable->UnloadAllExceptCurrentLocalEquipPartData();
	//}

	//UWorld* TheWorld = GetWorld();
	//if (TheWorld)
	//{
	//	TheWorld->ForceGarbageCollection();
	//}
}

void FEquipTypeTab::Init()
{
	if (UI_CommonTab.IsValid())
		UI_CommonTab->Init();
}

void FEquipTypeTab::InitEquipTypeTab(EPCClass InCharacterClass, EItemClass InItemClass)
{
	ItemClass = InItemClass;
	CharacterClass = InCharacterClass;

	FText TabName;
	switch (InItemClass)
	{
	case EItemClass::EIC_Weapon: TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_WeaponTab"))); break;
	case EItemClass::EIC_Helmet: TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_HelmetTab"))); break;
	case EItemClass::EIC_BodyArmor: TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_BodyArmorTab"))); break;
	case EItemClass::EIC_Glove: TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_GloveTab"))); break;
	case EItemClass::EIC_Shoes: TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_ShoesTab"))); break;
	case EItemClass::EIC_Belt: TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_BeltTab"))); break;
	case EItemClass::EIC_Necklace: TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_NecklaceTab"))); break;
	case EItemClass::EIC_Ring: TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_RingTab"))); break;
	case EItemClass::EIC_SetItem: TabName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_SetTab"))); break;
	}

	if (UI_CommonTab.IsValid())
	{
		UI_CommonTab->SetTabName(TabName);
		
		if (!UI_RedDotWidget.IsValid())
			UI_RedDotWidget = UI_CommonTab->GetMyRedDotWidget();
	}

	DoMarkRedDot();
}

void FEquipTypeTab::SetVisible(bool bVisible)
{
	bVisibleTab = bVisible;

	if (UI_CommonTab.IsValid())
	{
		UI_CommonTab->SetVisible(bVisibleTab);
	}
	if (UI_RedDotWidget.IsValid())
		UI_RedDotWidget->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	DoMarkRedDot();
}

void FEquipTypeTab::DoMarkRedDot()
{
	if (!bVisibleTab || !UI_RedDotWidget.IsValid())
		return;

	if (CharacterClass == EPCClass::EPC_End || ItemClass == EItemClass::EIC_End)
		return;

	//이건 RedDot으로
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	bool bVisibleNew = DataStore.GetEnhancedCollectionItemCount(CharacterClass, ItemClass) > 0 ? true : false;

	UI_RedDotWidget->SetVisibility(bVisibleNew ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void FCharacterButton::SetSelect(bool bSelect)
{
	if (IMG_CharacterSelected.IsValid())
		IMG_CharacterSelected->SetVisibility(bSelect ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	
	if (IMG_CharacterUnSelected.IsValid())
		IMG_CharacterUnSelected->SetVisibility(bSelect ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
}

void FCharacterButton::SetAlarmIcon(bool bVisible)
{
	if (IMG_NewIcon.IsValid())
		IMG_NewIcon->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void FIconRow::SetVisible(bool bVisible)
{
	if (CP_IconRow.IsValid())
		CP_IconRow->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (UIP_IconRow.IsValid())
		UIP_IconRow->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void FIconRow::SetText(const FText& RowText)
{
	if (TB_IconRowText.IsValid())
		TB_IconRowText->SetText(RowText);
}
