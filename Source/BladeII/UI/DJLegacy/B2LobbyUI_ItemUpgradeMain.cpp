// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_ItemUpgradeMain.h"
#include "B2LobbyUI_ItemEnhUpgCommon.h"
#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2UIShortagePopupManager.h"
#include "BladeIIGameImpl.h"

UB2LobbyUI_ItemUpgradeMain::UB2LobbyUI_ItemUpgradeMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EnhUpgCommonNRef = NULL;

	UpgradeCommitAnimLength = 0.5f;
}

void UB2LobbyUI_ItemUpgradeMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_UpgradeTitle)
	GET_SLOT(UTextBlock, TB_UpgradeCommitBtn)
	GET_SLOT(UB2Button, BTN_UpgradeCommit)
	BIND_CLICK_FUNC_TO_BTN(BTN_UpgradeCommit, &UB2LobbyUI_ItemUpgradeMain::OnUpgradeCommit)
	GET_SLOT(UB2Button, BTN_Close)
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_ItemUpgradeMain::OnCloseButtonPressed)

	GET_SLOT(UTextBlock, TB_UpgradeCostNum)
	GET_SLOT(UImage, IMG_UpgradeStone)
	GET_SLOT(UTextBlock, TB_CurrentUpgradeStone)
	GET_SLOT(UTextBlock, TB_RequiredUpgradeStone)
	GET_SLOT(UTextBlock, TB_UpgradeNote)
	GET_SLOT(UTextBlock, TB_UpgradeNote2)
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_UpgradeGoods);
}

void UB2LobbyUI_ItemUpgradeMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	// 窜鉴洒 酒捞袍 沥焊父 掘绢坷妨搁 Inventory 牢胶畔胶 绝捞 static 栏肺 掘绢棵 荐 乐栏唱 碍拳 扁瓷篮 牢亥配府啊 栋 乐绰 惑怕俊辑 悼累.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// 捞 矫痢俊 Inventory 狼 TargetItemID 绰 技泼登绢 乐绢具 窃. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeUpgradeTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Upgrade)
	{
		CloseAndQuitItemOpMode(); // 父俊 窍唱 角菩茄促搁.. 绢驴 荐 绝瘤.
		return;
	}

	SetPositionInViewport(FVector2D(0.0f, 0.0f));

	UpdateStaticTexts();
	UpdateDynamicTexts();
	UpdateUpgradeCommitBtnState();

	if (EnhUpgCommonNRef)
	{
		EnhUpgCommonNRef->StartFromLobby(InUIManager, InGM);
		EnhUpgCommonNRef->SetItemOpTargetData(NativeUpgradeTargetData, false);
	}
}

void UB2LobbyUI_ItemUpgradeMain::UpdateOnUpgradeStateChange()
{
	UpdateDynamicTexts();
	UpdatePostUpgradeOptionStatTexts();
	UpdateUpgradeCommitBtnState();
}

void UB2LobbyUI_ItemUpgradeMain::UpdateStaticTexts()
{
	if (TB_UpgradeTitle.IsValid())
	{
		TB_UpgradeTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemUpgrade")));
	}
	if (TB_UpgradeCommitBtn.IsValid())
	{
		TB_UpgradeCommitBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemUpgrade")));
	}
}

void UB2LobbyUI_ItemUpgradeMain::UpdateDynamicTexts()
{
	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();

	if (TB_UpgradeCostNum.IsValid()) // 铰鞭 厚侩篮 酒付档 老沥窍瘤父 捞 其捞瘤甫 凯绊 抄 饶 辑滚俊辑 览翠阑 罐霸 登搁 力措肺 技泼窍霸 瞪 巴.
	{
		int32 UpgradeGoldCost = CachedInventory ? CachedInventory->GetCurrentItemOpGoldCost() : 0;
		int32 CurrentGold = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();

		TB_UpgradeCostNum->SetText(FText::FromString(FString::FromInt(UpgradeGoldCost)));
		TB_UpgradeCostNum->SetColorAndOpacity(
			(CurrentGold >= UpgradeGoldCost) ?
			// 累篮 咆胶飘扼 窍靛内爹 窍看绰单 父老 咯扁档 漂喊茄 祸惑栏肺 持绊 酵促绊 窍搁 property 肺 哗档废.
			FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)) : FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))
		);
	}
	const int32 CurrentUpdgradeStoneID = FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(NativeUpgradeTargetData);
	const int32 CurrentUpgradeStoneAmount = CachedInventory ? CachedInventory->HowManyUpgradeStoneForCurrentTarget() : 0; 
	const int32 RequiredUpgradeStone = CachedInventory ? CachedInventory->GetCurrentUpgradeStoneCost() : 0;
	if (TB_CurrentUpgradeStone.IsValid())
	{
		TB_CurrentUpgradeStone->SetText(FText::FromString(FString::FromInt(CurrentUpgradeStoneAmount)));

		TB_CurrentUpgradeStone->SetColorAndOpacity(
			(CurrentUpgradeStoneAmount >= RequiredUpgradeStone) ?
			// 累篮 咆胶飘扼 窍靛内爹 窍看绰单 父老 咯扁档 漂喊茄 祸惑栏肺 持绊 酵促绊 窍搁 property 肺 哗档废.
			FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)) : FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))
			);
	}
	if (TB_RequiredUpgradeStone.IsValid())
	{
		TB_RequiredUpgradeStone->SetText(FText::FromString(FString::FromInt(RequiredUpgradeStone)));
	}
	if (IMG_UpgradeStone.IsValid())
	{
		FSingleItemInfoData* WantedUpgradeStoneItemInfo = ItemInfoTable ? ItemInfoTable->GetInfoData(CurrentUpdgradeStoneID) : NULL;
		if (WantedUpgradeStoneItemInfo)
		{
			IMG_UpgradeStone->SetBrushFromMaterial(WantedUpgradeStoneItemInfo->GetIconMaterial(ItemInfoTable));
			if(BTN_UpgradeGoods.IsValid())
			{
				BTN_UpgradeGoods->SetItemInfo(CurrentUpdgradeStoneID);
			}
		}
	}

	if (TB_UpgradeNote.IsValid())
	{
		TB_UpgradeNote->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_UpgradeNote")));
	}

	if (TB_UpgradeNote2.IsValid())
	{
		TB_UpgradeNote2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_UpgradeAddNote")));
	}

	// 泅犁 铰鞭 啊瓷茄 厘厚啊 焊包窃俊 歹 捞惑 绝促搁 皋矫瘤甫 谎妨霖促.
	TArray<FB2Item> CurrPCItemList;
	check(CachedInventory);
	CachedInventory->GetStoredItemList_ItemOpModeFilter(CurrPCItemList, CachedInventory->GetMainInvenViewCurrentTab(), EPCClass::EPC_End, true);
	if (CurrPCItemList.Num() == 0)
	{
		LobbyInvenSetItemOpModeGuideTextClass<const FText&>::GetInstance().Signal(
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_NoUpgradeItemInStoreMessage"))
			);
	}
	else
	{
		LobbyInvenHideItemOpModeGuideTextClass<>::GetInstance().Signal();
	}
}

void UB2LobbyUI_ItemUpgradeMain::UpdatePostUpgradeOptionStatTexts()
{
	// 傍烹 颇飘肺
	if (EnhUpgCommonNRef)
	{
		EnhUpgCommonNRef->UpdateOptionStatTexts();
	}
}

void UB2LobbyUI_ItemUpgradeMain::UpdateUpgradeCommitBtnState()
{
	int32 UpgradeGoldCost = CachedInventory ? CachedInventory->GetCurrentItemOpGoldCost() : 0;
	int32 CurrentGold = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
	bool IsGold = CurrentGold >= UpgradeGoldCost ? true : false;

	const int32 CurrentUpgradeStoneAmount = CachedInventory ? CachedInventory->HowManyUpgradeStoneForCurrentTarget() : 0;
	const int32 RequiredUpgradeStone = CachedInventory ? CachedInventory->GetCurrentUpgradeStoneCost() : 0;
	bool IsUpgradeStone = CurrentUpgradeStoneAmount >= RequiredUpgradeStone ? true : false;

	if (BTN_UpgradeCommit.IsValid())
	{
		if (IsGold == true && IsUpgradeStone == true)
		{
			BTN_UpgradeCommit->SetIsEnabled(true);
		}
		else
		{
			BTN_UpgradeCommit->SetIsEnabled(false);
		}
	}
}

void UB2LobbyUI_ItemUpgradeMain::DestroySelf()
{
	Super::DestroySelf();

	// 傍烹 颇飘 力芭
	if (EnhUpgCommonNRef)
	{
		EnhUpgCommonNRef->DestroySelf();
		EnhUpgCommonNRef = NULL;
	}
}

void UB2LobbyUI_ItemUpgradeMain::OnUpgradeCommit()
{
	if (CachedInventory && !CachedInventory->HasEnoughUpgradeStoneForCurrentTarget())
	{ // 铰鞭籍 何练狼 版快 咯辨 努腐且 荐绰 乐栏唱 角力 矫累篮 救窍绊 馆拜带傈栏肺狼 涝厘阑 蜡档.
		const int32 CurrentUpdgradeStoneID(FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(NativeUpgradeTargetData));
		ShortageMGR->PopupGoToCounterDungeonForUpgradeStone(CurrentUpdgradeStoneID);
	}
	else if (CachedInventory && !CachedInventory->CanAffordCurrentItemOp())
	{ // 榜靛 何练狼 版快 咯辨 努腐且 荐绰 乐栏唱 角力 矫累篮 救窍绊 荤侩磊 泅龙阑 蜡档
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{
		// 局聪皋捞记 脚龋何磐.
		BeginUpgradeCommitAnim();
		if (EnhUpgCommonNRef)
		{
			EnhUpgCommonNRef->BeginUpgradeCommitAnim();
		}

		if (CachedInventory)
		{// 楷免登绰 悼救 咯傈洒 犁丰 急琶 殿阑 窍霸 瞪 荐 乐绢辑 阜绰促.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_UpgradeCommit.IsValid())
		{
			BTN_UpgradeCommit->SetIsEnabled(false); // 娟档 拌加 努腐捞 瞪 荐 乐绢辑 阜澜.
		}
		
		APlayerController* OwningPC = GetOwningPlayer();
		if (OwningPC && UpgradeCommitAnimLength > 0.0f)
		{
			OwningPC->GetWorldTimerManager().SetTimer(UpgradeCommintTH, this, &UB2LobbyUI_ItemUpgradeMain::FinalUpgradeCommit, UpgradeCommitAnimLength, false);
		}
		else
		{
			FinalUpgradeCommit();
		}
	}
}

void UB2LobbyUI_ItemUpgradeMain::CloseWidgetDelegate()
{
	OnCloseButtonPressed();
}

void UB2LobbyUI_ItemUpgradeMain::OnCloseButtonPressed()
{
	CloseAndQuitItemOpMode();
}

void UB2LobbyUI_ItemUpgradeMain::CloseAndQuitItemOpMode()
{// 咯扁辑 官肺 碍拳芒阑 摧绰 措脚 Inventory 率栏肺 脚龋.
	QuitItemOpModeClass<bool>::GetInstance().Signal(false);
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_ItemUpgradeMain::FinalUpgradeCommit()
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetWorldTimerManager().ClearTimer(UpgradeCommintTH);
	}

	// 夯拜 悼累. 捞饶 楷免捞唱 搬苞 殿狼 flow 绰 辑滚 览翠俊 蝶扼 喊档 UI 芒俊辑 柳青.
	LobbyReqUpgradeItemClass<>::GetInstance().Signal();
}