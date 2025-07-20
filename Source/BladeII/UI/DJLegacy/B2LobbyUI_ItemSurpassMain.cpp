// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemSurpassMain.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "Event.h"
#include "B2UIShortagePopupManager.h"
#include "BladeIIGameImpl.h"

UB2LobbyUI_ItemSurpassMain::UB2LobbyUI_ItemSurpassMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;

	ItemIconWidgetClass = NULL;
	CreatedTargetItemIcon = NULL;
	CreatedIngredItemIcon = NULL;

	SurpassCommitAnimLength = 0.5f;
}

void UB2LobbyUI_ItemSurpassMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, CP_TargetItemIconSet)
	GET_SLOT(UCanvasPanel, CP_IngredItemIconSet)
	GET_SLOT(UTextBlock, TB_SurpassCommitBtn)
	GET_SLOT(UTextBlock, TB_SurpassTitle)
	GET_SLOT(UTextBlock, TB_SurpassCostNum)
	GET_SLOT(UB2Button, BTN_SurpassCommit)
	BIND_CLICK_FUNC_TO_BTN(BTN_SurpassCommit, &UB2LobbyUI_ItemSurpassMain::OnSurpassCommit)
	GET_SLOT(UB2Button, BTN_Close)
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_ItemSurpassMain::OnCloseButtonPressed)
	GET_SLOT(UTextBlock, TB_NoIngredMessage)
	GET_SLOT(UB2Button, BTN_IngredUnselect)
	BIND_CLICK_FUNC_TO_BTN(BTN_IngredUnselect, &UB2LobbyUI_ItemSurpassMain::OnClickIngredUnselectBtn)

	GET_SLOT(UPanelWidget, P_Selected)

	GET_SLOT(UTextBlock, TB_CurrentLabel)
	GET_SLOT(UTextBlock, TB_PostSurpassLabel)
	GET_SLOT(UTextBlock, TB_CurrMaxLevelLabel)
	GET_SLOT(UTextBlock, TB_CurrMaxLevel)
	GET_SLOT(UTextBlock, TB_PostSurpassMaxLevelLabel)
	GET_SLOT(UTextBlock, TB_PostSurpassMaxLevel)
	GET_SLOT(UTextBlock, TB_SurpassDescription)
	GET_SLOT(UTextBlock, TB_SurpassUnselectDescription)
}

void UB2LobbyUI_ItemSurpassMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	// 窜鉴洒 酒捞袍 沥焊父 掘绢坷妨搁 Inventory 牢胶畔胶 绝捞 static 栏肺 掘绢棵 荐 乐栏唱 钦己 殿狼 扁瓷篮 牢亥配府啊 栋 乐绰 惑怕俊辑 悼累.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// 捞 矫痢俊 Inventory 狼 TargetItemID 绰 技泼登绢 乐绢具 窃. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeSurpassTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Surpass)
	{
		CloseAndQuitItemOpMode(); // 父俊 窍唱 角菩茄促搁.. 绢驴 荐 绝瘤.
		return;
	}

	SetPositionInViewport(FVector2D(0.0f, 0.0f));

	UpdateStaticTexts();
	UpdateDynamicWidgets();
	UpdateTargetItemIcon();
	UpdateSurpassCommitBtnState();
}

void UB2LobbyUI_ItemSurpassMain::DestroySelf()
{
	Super::DestroySelf();

	DestroyTargetItemIcon();
	DestroyIngredItemIcon();
}

void UB2LobbyUI_ItemSurpassMain::UpdateOnSurpassStateChange()
{
	UpdateSurpassInfo();
	UpdateDynamicWidgets();
	UpdateIngredItemIcon();
	UpdateSurpassCommitBtnState();
}

void UB2LobbyUI_ItemSurpassMain::UpdateSurpassInfo()
{
	SumSurpassData.Empty();
	CurrIngredIDs.Empty();

	CachedInventory->GetAllSelectedItemOpIngreds(CurrIngredIDs);

	if (CurrIngredIDs.Num() > 0)
	{
		UB2LobbyInventory::FindFromAll(SumSurpassData, CurrIngredIDs[0]);
	}
}

void UB2LobbyUI_ItemSurpassMain::UpdateStaticTexts()
{
	if (TB_SurpassCommitBtn.IsValid())
	{
		TB_SurpassCommitBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemSurpass")));
	}
	if (TB_SurpassTitle.IsValid())
	{
		TB_SurpassTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemSurpass")));
	}
	if (TB_NoIngredMessage.IsValid())
	{
		TB_NoIngredMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassNoIngredMessage")));
	}
	if (TB_SurpassDescription.IsValid())
	{
		TB_SurpassDescription->SetText(FText::Format(
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassNote")), FText::AsNumber(1) // 1 捞惑 棵扼哎 老捞 乐唱? せ : 乐匙!
			));
	}
	if (TB_SurpassUnselectDescription.IsValid())
	{
		TB_SurpassUnselectDescription->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassDefaultNote")));
	}
	if (TB_CurrentLabel.IsValid())
	{
		TB_CurrentLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_CurrentLabel")));
	}
}

void UB2LobbyUI_ItemSurpassMain::UpdateDynamicWidgets()
{
	check(CachedInventory);

	if (TB_SurpassCostNum.IsValid()) // 檬岿 厚侩捞 老沥窍歹扼档 捞 其捞瘤甫 凯绊 抄 饶 辑滚俊辑 览翠阑 罐霸 登搁 力措肺 技泼窍霸 瞪 巴.
	{
		TB_SurpassCostNum->SetText(FText::FromString(FString::FromInt(CachedInventory ? CachedInventory->GetCurrentItemOpGoldCost() : 0)));
	}

	// 泅犁 檬岿 犁丰肺 荤侩 啊瓷茄 厘厚啊 焊包窃俊 歹 捞惑 绝促搁 皋矫瘤甫 谎妨霖促.
	TArray<FB2Item> CurrPCItemList;
	CachedInventory->GetStoredItemList_ItemOpModeFilter(CurrPCItemList, CachedInventory->GetMainInvenViewCurrentTab(), EPCClass::EPC_End, true);
	if (CurrPCItemList.Num() == 0)
	{
		const wchar_t* GuideText;	// 免仿蹬具 登绰 巩备 虐蔼阑 历厘且 函荐.
		if (NativeSurpassTargetData.StarGrade >= FItemGradeInfo::MINIMUM_ADVANCED_SURPASS_STAR_GRADE)	// 11己 捞惑老 版快
		{
			GuideText = TEXT("LobbyInvenText_No11StarSurpassIngredInStoreMessage");
		}
		else
		{
			GuideText = TEXT("LobbyInvenText_NoSurpassIngredInStoreMessage");
		}
		LobbyInvenSetItemOpModeGuideTextClass<const FText&>::GetInstance().Signal(
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, GuideText)
		);
	}
	else
	{
		LobbyInvenHideItemOpModeGuideTextClass<>::GetInstance().Signal();
	}

	// 犁丰 急琶 惑怕俊 蝶弗 visibility..
	if (TB_NoIngredMessage.IsValid())
	{
		TB_NoIngredMessage->SetVisibility(IsAnyIngredSelected() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
	if (BTN_IngredUnselect.IsValid())
	{
		BTN_IngredUnselect->SetVisibility(IsAnyIngredSelected() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (P_Selected.IsValid())
	{
		P_Selected->SetVisibility(IsAnyIngredSelected() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (TB_SurpassUnselectDescription.IsValid())
	{
		TB_SurpassUnselectDescription->SetVisibility(IsAnyIngredSelected() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
	if (TB_SurpassDescription.IsValid())
	{
		TB_SurpassDescription->SetVisibility(IsAnyIngredSelected() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	
	if (IsAnyIngredSelected())
	{

		// Surpass 沥焊

		FItemSurpassLevel InSurpassInfo = BladeIIGameImpl::GetClientDataStore().GetSurpassLevelData(SumSurpassData.StarGrade);
		FItemSurpassLevel InCurrentItemSurpassInfo = BladeIIGameImpl::GetClientDataStore().GetSurpassLevelData(NativeSurpassTargetData.StarGrade);
		
		// 措眉肺 P_Selected 酒贰俊 硅摹等 巴甸.

		// 檬岿 捞饶 沥焊绰 捞 矫痢浚 绝阑 巴烙. 绢瞒乔 弥措 碍拳饭骇 窍唱 刘啊窍绰 芭.
		const int32 LimitLevel = FB2Item::ItemNormalMaxLevel + InCurrentItemSurpassInfo.MaxSurpassLevel; // 酒捞袍喊 弥措 檬岿饭骇
		const int32 CurrLevel = NativeSurpassTargetData.Level;
		const int32 CurrMaxLevel = NativeSurpassTargetData.MaxLevel;
		const int32 NextSurpassStep = NativeSurpassTargetData.SurpassCount + InSurpassInfo.IncreaseLevel; // 促澜 檬岿 饭骇

		int32 PostSurpassMaxEnhanceLevel = NativeSurpassTargetData.MaxLevel + InSurpassInfo.IncreaseLevel;

		if (LimitLevel < PostSurpassMaxEnhanceLevel)
		{
			PostSurpassMaxEnhanceLevel = LimitLevel;
		}

		if (TB_PostSurpassLabel.IsValid())
		{
			TB_PostSurpassLabel->SetText(
				FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_PostSurpassLabel")), FText::AsNumber(PostSurpassMaxEnhanceLevel - FB2Item::ItemNormalMaxLevel))
			);
		}

		if (TB_CurrMaxLevelLabel.IsValid())
		{
			TB_CurrMaxLevelLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassMaxLevelLabel"))); // 傲 绊沥 咆胶飘.
		}
		if (TB_CurrMaxLevel.IsValid())
		{
			TB_CurrMaxLevel->SetText(FText::AsNumber(CurrMaxLevel));
		}
		if (TB_PostSurpassMaxLevelLabel.IsValid())
		{
			TB_PostSurpassMaxLevelLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassMaxLevelLabel"))); // 傲 绊沥 咆胶飘.
		}

		if (TB_PostSurpassMaxLevel.IsValid())
		{
			TB_PostSurpassMaxLevel->SetText(FText::AsNumber(PostSurpassMaxEnhanceLevel));
		}

		if (TB_SurpassDescription.IsValid())
		{
			int32 IncreaseLevel = PostSurpassMaxEnhanceLevel - CurrMaxLevel;
			TB_SurpassDescription->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassNote")), FText::AsNumber(IncreaseLevel >= 0 ? IncreaseLevel : InSurpassInfo.IncreaseLevel)));
		}
	}
}

void UB2LobbyUI_ItemSurpassMain::UpdateTargetItemIcon()
{
	DestroyTargetItemIcon();

	if (ItemIconWidgetClass == NULL || !CP_TargetItemIconSet.IsValid() || NativeSurpassTargetData.InstanceUID <= 0 || NativeSurpassTargetData.ItemRefID <= 0){
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
		CreatedTargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget);
		CreatedTargetItemIcon->UpdateItemData(NativeSurpassTargetData);
	}
}

void UB2LobbyUI_ItemSurpassMain::UpdateIngredItemIcon()
{
	DestroyIngredItemIcon();

	if (CurrIngredIDs.Num() > 0 && SumSurpassData.StarGrade != 0)
	{
		UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(CP_IngredItemIconSet->Slot);

		FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);
		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

		// MainPanelNativeRef 困俊 弥辆利牢 酒捞袍 酒捞能 widget 积己
		CreatedIngredItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
			ItemIconWidgetClass, this, CP_IngredItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
			FVector2D(0.0f, 0.0f), 0, false
			));
		if (CreatedIngredItemIcon)
		{
			CreatedIngredItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ComposeOrSurpassIngredient);
			CreatedIngredItemIcon->UpdateItemData(SumSurpassData); // 俺喊 酒捞袍 沥焊甫 逞败霖促.
		}
	}

	CurrIngredIDs.Empty();
}

void UB2LobbyUI_ItemSurpassMain::UpdateSurpassCommitBtnState()
{
	if (BTN_SurpassCommit.IsValid())
	{
		BTN_SurpassCommit->SetIsEnabled(
			IsAnyIngredSelected()
			// 捣档 眉农秦具 窃. -> 眉农窍瘤 臼绰促. 努腐篮 啊瓷窍档废 秦辑 荤侩磊 泅龙阑 蜡档茄促.
			//&& CachedInventory->CanAffordCurrentItemOp()
			);
	}
}

void UB2LobbyUI_ItemSurpassMain::DestroyTargetItemIcon()
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->DestroySelf();
		CreatedTargetItemIcon = NULL;
	}
}

void UB2LobbyUI_ItemSurpassMain::DestroyIngredItemIcon()
{
	if (CreatedIngredItemIcon)
	{
		CreatedIngredItemIcon->DestroySelf();
		CreatedIngredItemIcon = NULL;
	}
}

bool UB2LobbyUI_ItemSurpassMain::IsAnyIngredSelected()
{
	return (CachedInventory && CachedInventory->GetCurrentSelectIngredItemNum() > 0);
}

void UB2LobbyUI_ItemSurpassMain::OnSurpassCommit()
{
	if (CachedInventory && !CachedInventory->CanAffordCurrentItemOp())
	{ // 榜靛 何练狼 版快 咯辨 努腐且 荐绰 乐栏唱 角力 矫累篮 救窍绊 荤侩磊 泅龙阑 蜡档
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{
		// 局聪皋捞记 脚龋何磐.
		BeginSurpassCommitAnim();

		if (CachedInventory)
		{// 楷免登绰 悼救 咯傈洒 犁丰 急琶 殿阑 窍霸 瞪 荐 乐绢辑 阜绰促.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_SurpassCommit.IsValid())
		{
			BTN_SurpassCommit->SetIsEnabled(false); // 娟档 拌加 努腐捞 瞪 荐 乐绢辑 阜澜.
		}

		APlayerController* OwningPC = GetOwningPlayer();
		if (OwningPC && SurpassCommitAnimLength > 0.0f)
		{
			OwningPC->GetWorldTimerManager().SetTimer(SurpassCommintTH, this, &UB2LobbyUI_ItemSurpassMain::FinalSurpassCommit, SurpassCommitAnimLength, false);
		}
		else
		{
			FinalSurpassCommit();
		}
	}
}

void UB2LobbyUI_ItemSurpassMain::CloseWidgetDelegate()
{
	OnCloseButtonPressed();
}

void UB2LobbyUI_ItemSurpassMain::OnCloseButtonPressed()
{
	CloseAndQuitItemOpMode();
}

void UB2LobbyUI_ItemSurpassMain::OnClickIngredUnselectBtn()
{
	if (CachedInventory && IsAnyIngredSelected())
	{
		TArray<int64> IngredIDs;
		CachedInventory->GetAllSelectedItemOpIngreds(IngredIDs);
		// 拱沸 咯扁急 IngredIDs 绰 窍唱咯具
		if (IngredIDs.Num() > 0)
		{
			UnSelectForSurpassIngredClass<int64>::GetInstance().Signal(IngredIDs[0]);
		}
	}
}

void UB2LobbyUI_ItemSurpassMain::CloseAndQuitItemOpMode()
{
	QuitItemOpModeClass<bool>::GetInstance().Signal(false);
}

void UB2LobbyUI_ItemSurpassMain::FinalSurpassCommit()
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetWorldTimerManager().ClearTimer(SurpassCommintTH);
	}

	// 夯拜 悼累. 捞饶 楷免捞唱 搬苞 殿狼 flow 绰 辑滚 览翠俊 蝶扼 喊档 UI 芒俊辑 柳青.
	LobbyReqSurpassItemWithCurrentSelectionClass<>::GetInstance().Signal();
}