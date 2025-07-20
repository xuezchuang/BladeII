// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemLevelupMain.h"
#include "B2LobbyUI_ItemEnhUpgCommon.h"
#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2DynItemIcon_EnhanceIngred.h"
#include "B2UIShortagePopupManager.h"
#include "TutorialManager.h"

UB2LobbyUI_ItemLevelupMain::UB2LobbyUI_ItemLevelupMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemIconWidgetClass = NULL;
	IngredientIconPlacementMargin = 10.0f;
	IngredientIconNextRowOffset = 20.0f;
	EnhUpgCommonNRef = NULL;
	EnhanceCommitAnimLength = 0.5f;
}

void UB2LobbyUI_ItemLevelupMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_EnhanceTitle)
	GET_SLOT(UTextBlock, TB_EnhanceCommitBtn)
	GET_SLOT(UB2Button, BTN_EnhanceCommit)
	BIND_CLICK_FUNC_TO_BTN(BTN_EnhanceCommit, &UB2LobbyUI_ItemLevelupMain::OnEnhanceCommit)
	GET_SLOT(UB2Button, BTN_Close)
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_ItemLevelupMain::OnCloseButtonPressed)

	GET_SLOT(UTextBlock, TB_EnhanceLevelText)
	GET_SLOT(UTextBlock, TB_EnhanceLevelNum)
	GET_SLOT(UProgressBar, PB_EnhanceExpProgress)
	GET_SLOT(UTextBlock, TB_EnhanceExpPercent)

	GET_SLOT(UTextBlock, TB_EnhanceCostNum)
	GET_SLOT(UTextBlock, TB_EnhanceIngredNote)
	GET_SLOT(UTextBlock, TB_EnhanceIngredCount)
	GET_SLOT(UTextBlock, TB_SelectEnhanceItemText)
		

	GET_SLOT(UB2ScrollBox, SB_EnhanceIngredIcons)
}

void UB2LobbyUI_ItemLevelupMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
	
	// 窜鉴洒 酒捞袍 沥焊父 掘绢坷妨搁 Inventory 牢胶畔胶 绝捞 static 栏肺 掘绢棵 荐 乐栏唱 饭骇诀 扁瓷篮 牢亥配府啊 栋 乐绰 惑怕俊辑 悼累.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// 捞 矫痢俊 Inventory 狼 TargetItemID 绰 技泼登绢 乐绢具 窃. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeTargetItem) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Levelup)
	{
		CloseAndQuitItemOpMode(); // 父俊 窍唱 角菩茄促搁.. 绢驴 荐 绝瘤.
		return;
	}

	SetPositionInViewport(FVector2D(0.0f, 0.0f));

	UpdateStaticTexts();
	UpdatePostItemLevelupExpValues();
	UpdateDynamicTexts();
	UpdateItemLevelupCommitBtnState();

	LevelExpProgressAnim.SyncToTargetValues(); // 鸥百蔼甸篮 UpdatePostItemLevelupExpValues 俊辑 甸绢啊 乐阑 巴.

	if (EnhUpgCommonNRef)
	{
		EnhUpgCommonNRef->StartFromLobby(InUIManager, InGM);
		EnhUpgCommonNRef->SetItemOpTargetData(NativeTargetItem, true); // Enhancement mode 肺.
	}

	LevelExpProgressAnim.OnLevelChanged.AddUObject(this, &UB2LobbyUI_ItemLevelupMain::OnUpdateProgressLevel);
	LevelExpProgressAnim.OnPrecentChanged.AddUObject(this, &UB2LobbyUI_ItemLevelupMain::OnUpdateProgressPercent);
}

void UB2LobbyUI_ItemLevelupMain::UpdateOnLevelupStateChange()
{
	UpdateStaticTexts(); // 饭骇诀 皋春 肯丰 傈俊 官肺 搬苞 犬牢 瞒 StaticText 档 诀单捞飘 窍绰 扒单, 被捞 鞘夸绝绢具 且 内靛捞瘤父 乐绢辑 奴老 朝 巴档 绝澜.
	UpdatePostItemLevelupExpValues();
	UpdateDynamicTexts();
	UpdateLevelupIngredientItemIcons();
	UpdatePostItemLevelupOptionStatTexts();
	UpdateItemLevelupCommitBtnState();
}

void UB2LobbyUI_ItemLevelupMain::UpdateStaticTexts()
{
	if (TB_EnhanceTitle.IsValid())
		TB_EnhanceTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemLevelup")));

	if (TB_EnhanceCommitBtn.IsValid())
		TB_EnhanceCommitBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Levelup")));

	if (TB_EnhanceLevelText.IsValid())
		TB_EnhanceLevelText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemLevelUpLevel")));
	
	if (TB_SelectEnhanceItemText.IsValid())
		TB_SelectEnhanceItemText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SelectEnhanceItem")));
	

	TArray<FB2Item> CurrPCItemList;
	CachedInventory->GetStoredItemList_ItemOpModeFilter(CurrPCItemList, CachedInventory->GetMainInvenViewCurrentTab(), EPCClass::EPC_End, true);
	if (CurrPCItemList.Num() == 0)
	{
		LobbyInvenSetItemOpModeGuideTextClass<const FText&>::GetInstance().Signal(
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_NoLevelupIngredInStoreMessage"))
		);
	}
	else
	{
		LobbyInvenHideItemOpModeGuideTextClass<>::GetInstance().Signal();
	}
}

void UB2LobbyUI_ItemLevelupMain::UpdateDynamicTexts()
{
	// 饭骇捞尔 Exp 绰 局聪皋捞记 锭巩俊 咯扁 富绊 概 平 诀单捞飘.
	if (TB_EnhanceCostNum.IsValid()) // 饭骇诀 犁丰 俺荐 蝶扼 饭骇诀 厚侩捞 官柴.
	{
		FText CostText;
		int32 NeedCost = CachedInventory->GetCurrentItemOpGoldCost();
		if (CachedInventory && NeedCost > 0)
		{
			CostText = FText::FromString(FString::FromInt(NeedCost));
		}
		else
		{
			CostText = FText::FromString(TEXT("-"));
		}
		TB_EnhanceCostNum->SetText(CostText);
	}

	if (TB_EnhanceIngredNote.IsValid())
	{
		TB_EnhanceIngredNote->SetText(FText::Format(
			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_ItemLevelUpIngredNote"))), FText::AsNumber(UB2LobbyInventory::GetItemLevelupIngredLimit())
			));
	}
	if (TB_EnhanceIngredCount.IsValid())
	{
		TB_EnhanceIngredCount->SetText(FText::FromString(
			FString::Printf(TEXT("(%d/%d)"), CachedInventory ? CachedInventory->GetCurrentSelectIngredItemNum() : 0, UB2LobbyInventory::GetItemLevelupIngredLimit())
			));
	}
}

void UB2LobbyUI_ItemLevelupMain::UpdateLevelupIngredientItemIcons()
{
	if (ItemIconWidgetClass == NULL || !SB_EnhanceIngredIcons.IsValid() || !CachedInventory) {
		return;
	}

	TArray<int64> AllIngredIDs;
	CachedInventory->GetAllSelectedItemOpIngreds(AllIngredIDs);

	if (TB_SelectEnhanceItemText.IsValid())
		TB_SelectEnhanceItemText->SetVisibility(AllIngredIDs.Num() > 0 ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	
	// 扁粮俊 积己等芭绰 犁荤侩~
	for(int32 II = 0; II < CreatedIngredientItemIcons.Num(); ++II)
	{
		UB2DynItemIcon_EnhanceIngred* CurrElem = CreatedIngredientItemIcons[II];
		if (CurrElem == nullptr)
			continue;

		if (AllIngredIDs.IsValidIndex(II))
		{
			FB2Item ThisIngred;
			if (!UB2LobbyInventory::FindFromAll(ThisIngred, AllIngredIDs[II]))
				continue;

			ESlateVisibility Visibilty = CurrElem->GetVisibility();
			if(Visibilty == ESlateVisibility::Collapsed)
				CurrElem->SetVisibility(ESlateVisibility::Visible);
			CurrElem->UpdateItemData(ThisIngred); // 俺喊 酒捞袍 沥焊甫 逞败霖促.
		}
		else
		{
			CurrElem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	// 货肺 眠啊等巴父 货肺 父电..
	for (int32 II = CreatedIngredientItemIcons.Num(); II < AllIngredIDs.Num(); ++II)
	{
		FB2Item ThisIngred;
		if (!UB2LobbyInventory::FindFromAll(ThisIngred, AllIngredIDs[II]))
			continue;

		UB2DynItemIcon_EnhanceIngred* NewItemIcon = CreateItemIcon();
		if (NewItemIcon)
		{
			NewItemIcon->UpdateItemData(ThisIngred); // 俺喊 酒捞袍 沥焊甫 逞败霖促.
			//NewItemIcon->SetupManualScrollBoxSender_byReceiver(this, SB_EnhanceIngredIcons.Get());
			CreatedIngredientItemIcons.Add(NewItemIcon);
		}
	}
}

UB2DynItemIcon_EnhanceIngred* UB2LobbyUI_ItemLevelupMain::CreateItemIcon()
{
	if (!SB_EnhanceIngredIcons.IsValid() || ItemIconWidgetClass == nullptr)
		return nullptr;

	UScaleBox* OuterSB = NewObject<UScaleBox>(this, NAME_None, RF_Transient);
	if (!OuterSB) {
		return nullptr;
	}
	UB2UnitedWidgetBase* CreatedWidget = CreateWidget<UB2UnitedWidgetBase>(GetOwningPlayer(), ItemIconWidgetClass);
	if (CreatedWidget == NULL) {
		return nullptr;
	}

	const float DefaultSacle = 0.8f;
	OuterSB->SetStretch(EStretch::UserSpecified);
	OuterSB->SetUserSpecifiedScale(DefaultSacle);

	UScaleBoxSlot* AddedSBSlot = Cast<UScaleBoxSlot>(OuterSB->AddChild(CreatedWidget));
	if (AddedSBSlot)
	{
		CreatedWidget->SetDynCreatedDummyParent(OuterSB); // CreatedWidget 阑 力芭且 锭 OuterSB 档 鞍捞 力芭登档废 殿废.
	}

	UScrollBoxSlot* AddedCPSlot = Cast<UScrollBoxSlot>(SB_EnhanceIngredIcons->AddChild(OuterSB));
	if (AddedCPSlot)
	{
		FMargin Margin = FMargin(5.0f, 0.0f, 0.0f, 0.0f);
		AddedCPSlot->SetPadding(Margin);
	}

	return Cast<UB2DynItemIcon_EnhanceIngred>(CreatedWidget);
}

void UB2LobbyUI_ItemLevelupMain::UpdatePostItemLevelupExpValues()
{
	TArray<FB2Item> AllIngredsData;
	if (CachedInventory){
		CachedInventory->GetAllSelectedItemOpIngreds(AllIngredsData);
	}
	float PostExp = NativeTargetItem.Exp;
	PostItemLevel = GetLevel_PreviewItemLevelup(PostExp, NativeTargetItem, AllIngredsData, true); // UI 俊 焊咯林妨搁 clamp 档 秦具摆促 せ
	PostExp *= 0.01f; // 0 ~ 1 荤捞肺 胶纳老傅.

	LevelExpProgressAnim.ApplyCustomTimerTick(this);
	LevelExpProgressAnim.SetTargetValues(PostItemLevel, PostExp, false);
}

void UB2LobbyUI_ItemLevelupMain::UpdatePostItemLevelupOptionStatTexts()
{
	// 傍烹 颇飘肺
	if (EnhUpgCommonNRef)
	{
		EnhUpgCommonNRef->UpdateOptionStatTexts();
	}
}

void UB2LobbyUI_ItemLevelupMain::UpdateItemLevelupCommitBtnState()
{
	if (BTN_EnhanceCommit.IsValid())
	{
		BTN_EnhanceCommit->SetIsEnabled(
			CachedInventory && CachedInventory->GetCurrentSelectIngredItemNum() > 0
			// 捣档 眉农秦具 窃. -> 眉农窍瘤 臼绰促. 努腐篮 啊瓷窍档废 秦辑 荤侩磊 泅龙阑 蜡档茄促.
			// && CachedInventory->CanAffordCurrentItemOp()
			);
	}
}

void UB2LobbyUI_ItemLevelupMain::OnUpdateProgressLevel(int32 CurrentLevel)
{
	if (TB_EnhanceLevelNum.IsValid())
	{
		TB_EnhanceLevelNum->SetText(FText::FromString(
			FString::Printf(TEXT("%d/%d"), CurrentLevel, NativeTargetItem.MaxLevel)
			));
	}
}

void UB2LobbyUI_ItemLevelupMain::OnUpdateProgressPercent(float CurrentPercent)
{
	// 弥措 饭骇俊急 父顶 盲况林扁
	const bool bAtMaxLevel = (LevelExpProgressAnim.GetCurrentAnimatedLevel() == NativeTargetItem.MaxLevel);
	
	if (PB_EnhanceExpProgress.IsValid())
	{
		PB_EnhanceExpProgress->SetPercent(bAtMaxLevel ? 1.0f : CurrentPercent);
	}
	if (TB_EnhanceExpPercent.IsValid())
	{
		TB_EnhanceExpPercent->SetText(
			bAtMaxLevel ? BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Maximum_ValueMark")) :
			FText::FromString(FString::Printf(TEXT("%.1f %%"), CurrentPercent * 100.0f))
			);
	}
}

void UB2LobbyUI_ItemLevelupMain::DestroySelf()
{
	Super::DestroySelf();

	// 傍烹 颇飘 力芭
	if (EnhUpgCommonNRef)
	{
		EnhUpgCommonNRef->DestroySelf();
		EnhUpgCommonNRef = NULL;
	}

	DestroyItemLevelupIngredientItemIcon();

	LevelExpProgressAnim.ClearCustomTimerTick();

	LevelExpProgressAnim.OnLevelChanged.RemoveAll(this);
	LevelExpProgressAnim.OnPrecentChanged.RemoveAll(this);
}

void UB2LobbyUI_ItemLevelupMain::OnEnhanceCommit()
{
	if (CachedInventory && !CachedInventory->CanAffordCurrentItemOp())
	{ // 榜靛 何练狼 版快 咯辨 努腐且 荐绰 乐栏唱 角力 矫累篮 救窍绊 荤侩磊 泅龙阑 蜡档
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{ // 饭骇诀 啊瓷窍促绊 魄窜等 版快

		// 局聪皋捞记 脚龋父 焊郴绊 官肺 矫累窍瘤绰 臼绰促.
		BeginEnhanceCommitAnim();
		if (EnhUpgCommonNRef)
		{
			EnhUpgCommonNRef->BeginEnhanceCommitAnim();
		}

		if (CachedInventory)
		{// 楷免登绰 悼救 咯傈洒 犁丰 急琶 殿阑 窍霸 瞪 荐 乐绢辑 阜绰促.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_EnhanceCommit.IsValid())
		{
			BTN_EnhanceCommit->SetIsEnabled(false); // 娟档 拌加 努腐捞 瞪 荐 乐绢辑 阜澜.
		}

		APlayerController* OwningPC = GetOwningPlayer();
		if (OwningPC && EnhanceCommitAnimLength > 0.0f)
		{
			OwningPC->GetWorldTimerManager().SetTimer(ItemLevelupCommintTH, this, &UB2LobbyUI_ItemLevelupMain::FinalItemLevelupCommit, EnhanceCommitAnimLength, false);
		}
		else
		{
			FinalItemLevelupCommit();
		}
	}
}

// Tutorial 400 酒捞袍 饭骇诀 傈侩 Protocol阑 荤侩窍绰 
TUTORIAL void UB2LobbyUI_ItemLevelupMain::Tutorial_FinalItemLevelUpCommit()
{
	// 辑滚俊 譬配府倔 傈侩 Protocol阑 罐扁 傈鳖瘤绰 老馆 Protocol肺
	LobbyReqItemLevelupWithCurrentSelectionClass<>::GetInstance().Signal();
}

void UB2LobbyUI_ItemLevelupMain::DestroyItemLevelupIngredientItemIcon()
{
	for (UB2DynItemIcon_EnhanceIngred* CurrElem : CreatedIngredientItemIcons)
	{
		if (CurrElem)
			CurrElem->DestroySelf();
	}
	CreatedIngredientItemIcons.Empty();
}

void UB2LobbyUI_ItemLevelupMain::CloseWidgetDelegate()
{
	OnCloseButtonPressed();
}

void UB2LobbyUI_ItemLevelupMain::OnCloseButtonPressed()
{
	CloseAndQuitItemOpMode();
}

void UB2LobbyUI_ItemLevelupMain::CloseAndQuitItemOpMode()
{// 咯扁辑 官肺 饭骇诀芒阑 摧绰 措脚 Inventory 率栏肺 脚龋.
	QuitItemOpModeClass<bool>::GetInstance().Signal(false);
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_ItemLevelupMain::FinalItemLevelupCommit()
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetWorldTimerManager().ClearTimer(ItemLevelupCommintTH);
	}

	TUTORIAL // Hardcoding
	//if (TutorialManager::GetInstance().IsCurrentTutorial(400, 7))
	if (TutorialManager::GetInstance().HasAnyTutorial())
	{
		TutorialManager::GetInstance().OnItemLevelupClicked();
		Tutorial_FinalItemLevelUpCommit();
	}

	else
	{
		// 夯拜 悼累. 捞饶 楷免捞唱 搬苞 殿狼 flow 绰 辑滚 览翠俊 蝶扼 喊档 UI 芒俊辑 柳青.
		LobbyReqItemLevelupWithCurrentSelectionClass<>::GetInstance().Signal();
	}
}
