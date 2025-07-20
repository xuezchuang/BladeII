// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemComposeMain.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "Event.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIStarGradePart.h"

UB2LobbyUI_ItemComposeMain::UB2LobbyUI_ItemComposeMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;

	ItemIconWidgetClass = NULL;
	CreatedTargetItemIcon = NULL;
	CreatedIngredItemIcon = NULL;

	CompseCommitAnimLength = 0.5f;
}

void UB2LobbyUI_ItemComposeMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, CP_TargetItemIconSet)
	GET_SLOT(UCanvasPanel, CP_IngredItemIconSet)
	GET_SLOT(UTextBlock, TB_ComposeCommitBtn)
	GET_SLOT(UTextBlock, TB_ComposeCostNum)
	GET_SLOT(UB2Button, BTN_ComposeCommit)
	BIND_CLICK_FUNC_TO_BTN(BTN_ComposeCommit, &UB2LobbyUI_ItemComposeMain::OnComposeCommit)
	GET_SLOT(UB2Button, BTN_Close)
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_ItemComposeMain::OnCloseButtonPressed)
	GET_SLOT(UTextBlock, TB_ComposeTitle)
	GET_SLOT(UTextBlock, TB_ComposeDescription)
	GET_SLOT(UTextBlock, TB_ComposeDescription2)
	GET_SLOT(UTextBlock, TB_ComposeDescription3)
	GET_SLOT(UTextBlock, TB_ExpectedResult)
	GET_SLOT(UTextBlock, TB_NoIngredMessage)
	GET_SLOT(UB2Button, BTN_IngredUnselect)

	BIND_CLICK_FUNC_TO_BTN(BTN_IngredUnselect, &UB2LobbyUI_ItemComposeMain::OnClickIngredUnselectBtn)
	
	GET_SLOT(UB2UIStarGradePart, StarGradePart);

	if (StarGradePart.IsValid())
	{
		StarGradePart->Init();
	}
}

void UB2LobbyUI_ItemComposeMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
	
	// 窜鉴洒 酒捞袍 沥焊父 掘绢坷妨搁 Inventory 牢胶畔胶 绝捞 static 栏肺 掘绢棵 荐 乐栏唱 钦己 殿狼 扁瓷篮 牢亥配府啊 栋 乐绰 惑怕俊辑 悼累.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// 捞 矫痢俊 Inventory 狼 TargetItemID 绰 技泼登绢 乐绢具 窃. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeComposeTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Composition)
	{
		CloseAndQuitItemOpMode(); // 父俊 窍唱 角菩茄促搁.. 绢驴 荐 绝瘤.
		return;
	}

	SetPositionInViewport(FVector2D(0.0f, 0.0f));

	UpdateStaticTexts();
	UpdateDynamicWidgets();
	UpdateTargetItemIcon();
	UpdateComposeCommitBtnState();
}

void UB2LobbyUI_ItemComposeMain::DestroySelf()
{
	Super::DestroySelf();
	
	DestroyTargetItemIcon();
	DestroyIngredItemIcon();
}

void UB2LobbyUI_ItemComposeMain::UpdateOnComposeStateChange()
{
	UpdateDynamicWidgets();
	UpdateIngredItemIcon();
	UpdateComposeCommitBtnState();
}

void UB2LobbyUI_ItemComposeMain::UpdateStaticTexts()
{
	if (TB_ComposeCommitBtn.IsValid())
	{
		TB_ComposeCommitBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemCompose")));
	}
	if (TB_ComposeTitle.IsValid())
	{
		TB_ComposeTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemCompose")));
	}
	if (TB_ComposeDescription.IsValid())
	{
		TB_ComposeDescription->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ComposeNote")));
	}
	if (TB_ComposeDescription2.IsValid())
	{
		TB_ComposeDescription2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ComposeAddNote")));
	}

	if (TB_ComposeDescription3.IsValid())
	{
		TB_ComposeDescription3->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ComposeNote3")));
	}

	if (TB_NoIngredMessage.IsValid())
	{
		TB_NoIngredMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ComposeNoIngredMessage")));
	}
}

void UB2LobbyUI_ItemComposeMain::UpdateDynamicWidgets()
{
	check(CachedInventory);

	if (TB_ComposeCostNum.IsValid()) // 钦己 厚侩捞 老沥窍歹扼档 捞 其捞瘤甫 凯绊 抄 饶 辑滚俊辑 览翠阑 罐霸 登搁 力措肺 技泼窍霸 瞪 巴.
	{
		TB_ComposeCostNum->SetText(FText::FromString(FString::FromInt(CachedInventory ? CachedInventory->GetCurrentItemOpGoldCost() : 0)));
	}

	// 泅犁 钦己 犁丰肺 荤侩 啊瓷茄 厘厚啊 焊包窃俊 歹 捞惑 绝促搁 皋矫瘤甫 谎妨霖促.
	TArray<FB2Item> CurrPCItemList;
	CachedInventory->GetStoredItemList_ItemOpModeFilter(CurrPCItemList, CachedInventory->GetMainInvenViewCurrentTab(), EPCClass::EPC_End, true);
	if (CurrPCItemList.Num() == 0)
	{
		LobbyInvenSetItemOpModeGuideTextClass<const FText&>::GetInstance().Signal(
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_NoComposeIngredInStoreMessage"))
			);
	}
	else
	{
		LobbyInvenHideItemOpModeGuideTextClass<>::GetInstance().Signal();
	}

	if (TB_NoIngredMessage.IsValid())
	{
		TB_NoIngredMessage->SetVisibility(IsAnyIngredSelected() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	// 捞扒 窍唱 刘啊窍绰 芭聪鳖 咯扁辑 傲 1 敲矾胶 秦辑
	const int32 TargetGrade = NativeComposeTargetData.StarGrade + 1;

	if (TB_ExpectedResult.IsValid())
	{
		TB_ExpectedResult->SetVisibility(IsAnyIngredSelected() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		if (IsAnyIngredSelected())
		{ 
			TB_ExpectedResult->SetText(
				FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ComposeExpectedResult")), FText::AsNumber(TargetGrade))
			);
		}
	}

	if (BTN_IngredUnselect.IsValid())
	{
		BTN_IngredUnselect->SetVisibility(IsAnyIngredSelected() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UB2LobbyUI_ItemComposeMain::UpdateTargetItemIcon()
{
	DestroyTargetItemIcon();

	if (ItemIconWidgetClass == NULL || !CP_TargetItemIconSet.IsValid() || NativeComposeTargetData.InstanceUID <= 0 || NativeComposeTargetData.ItemRefID <= 0){
		return;
	}

	// Anchor 汲沥俊 蝶扼 GetSize 绰 盔窍绰 蔼捞 救 唱棵 巴捞骨肺 林狼.
	UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(CP_TargetItemIconSet->Slot);
	FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);

	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	// ItemIconPanelNRef 困俊 弥辆利牢 酒捞袍 酒捞能 widget 积己
	CreatedTargetItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
		ItemIconWidgetClass, this, CP_TargetItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
		FVector2D(0.0f, 0.0f), 0, false
		));

	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget); // 钦己档 付蛮啊瘤肺?
		CreatedTargetItemIcon->UpdateItemData(NativeComposeTargetData);
	}

	if (StarGradePart.IsValid())
	{
		StarGradePart->UpdateStar(NativeComposeTargetData.StarGrade + 1, false);
	}
}

void UB2LobbyUI_ItemComposeMain::UpdateIngredItemIcon()
{
	DestroyIngredItemIcon();

	TArray<int64> IngredIDs;
	CachedInventory->GetAllSelectedItemOpIngreds(IngredIDs);

	if (IngredIDs.Num() > 0)
	{
		FB2Item ThisIngred;
		if (!UB2LobbyInventory::FindFromAll(ThisIngred, IngredIDs[0]))
			return;

		UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(CP_IngredItemIconSet->Slot);

		FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);
		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

		// MainPanelNativeRef 困俊 弥辆利牢 酒捞袍 酒捞能 widget 积己
		CreatedIngredItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
			ItemIconWidgetClass, this, CP_IngredItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
			FVector2D(0.0f, 0.0f), false
			));
		if (CreatedIngredItemIcon)
		{
			CreatedIngredItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ComposeOrSurpassIngredient);
			CreatedIngredItemIcon->UpdateItemData(ThisIngred); // 俺喊 酒捞袍 沥焊甫 逞败霖促.
		}
	}
}

void UB2LobbyUI_ItemComposeMain::UpdateComposeCommitBtnState()
{
	if (BTN_ComposeCommit.IsValid())
	{
		BTN_ComposeCommit->SetIsEnabled(
			IsAnyIngredSelected()
			// 捣档 眉农秦具 窃. -> 眉农窍瘤 臼绰促. 努腐篮 啊瓷窍档废 秦辑 荤侩磊 泅龙阑 蜡档茄促.
			//&& CachedInventory->CanAffordCurrentItemOp()
			);
	}
}

void UB2LobbyUI_ItemComposeMain::DestroyTargetItemIcon()
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->DestroySelf();
		CreatedTargetItemIcon = NULL;
	}
}

void UB2LobbyUI_ItemComposeMain::DestroyIngredItemIcon()
{
	if (CreatedIngredItemIcon)
	{
		CreatedIngredItemIcon->DestroySelf();
		CreatedIngredItemIcon = NULL;
	}
}

bool UB2LobbyUI_ItemComposeMain::IsAnyIngredSelected()
{
	return (CachedInventory && CachedInventory->GetCurrentSelectIngredItemNum() > 0);
}

void UB2LobbyUI_ItemComposeMain::OnComposeCommit()
{
	if (CachedInventory && !CachedInventory->CanAffordCurrentItemOp())
	{ // 榜靛 何练狼 版快 咯辨 努腐且 荐绰 乐栏唱 角力 矫累篮 救窍绊 荤侩磊 泅龙阑 蜡档
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{
		// 局聪皋捞记 脚龋何磐.
		BeginComposeCommitAnim();

		if (CachedInventory)
		{// 楷免登绰 悼救 咯傈洒 犁丰 急琶 殿阑 窍霸 瞪 荐 乐绢辑 阜绰促.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_ComposeCommit.IsValid())
		{
			BTN_ComposeCommit->SetIsEnabled(false); // 娟档 拌加 努腐捞 瞪 荐 乐绢辑 阜澜.
		}

		APlayerController* OwningPC = GetOwningPlayer();
		if (OwningPC && CompseCommitAnimLength > 0.0f)
		{
			OwningPC->GetWorldTimerManager().SetTimer(ComposeCommintTH, this, &UB2LobbyUI_ItemComposeMain::FinalComposeCommit, CompseCommitAnimLength, false);
		}
		else
		{
			FinalComposeCommit();
		}
	}
}

void UB2LobbyUI_ItemComposeMain::CloseWidgetDelegate()
{
	OnCloseButtonPressed();
}

void UB2LobbyUI_ItemComposeMain::OnCloseButtonPressed()
{
	CloseAndQuitItemOpMode();
}

void UB2LobbyUI_ItemComposeMain::OnClickIngredUnselectBtn()
{
	if (CachedInventory && IsAnyIngredSelected())
	{
		TArray<int64> IngredIDs;
		CachedInventory->GetAllSelectedItemOpIngreds(IngredIDs);
		// 拱沸 咯扁急 IngredIDs 绰 窍唱咯具
		if (IngredIDs.Num() > 0)
		{
			UnSelectForComposeIngredClass<int64>::GetInstance().Signal(IngredIDs[0]);
		}
	}
}

void UB2LobbyUI_ItemComposeMain::CloseAndQuitItemOpMode()
{
	QuitItemOpModeClass<bool>::GetInstance().Signal(false);
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_ItemComposeMain::FinalComposeCommit()
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetWorldTimerManager().ClearTimer(ComposeCommintTH);
	}

	// 夯拜 悼累. 捞饶 楷免捞唱 搬苞 殿狼 flow 绰 辑滚 览翠俊 蝶扼 喊档 UI 芒俊辑 柳青.
	LobbyReqComposeItemWithCurrentSelectionClass<>::GetInstance().Signal();
}