// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
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

	// �ܼ��� ������ ������ �������� Inventory �ν��Ͻ� ���� static ���� ���� �� ������ ��ȭ ����� �κ��丮�� �� �ִ� ���¿��� ����.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// �� ������ Inventory �� TargetItemID �� ���õǾ� �־�� ��. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeUpgradeTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Upgrade)
	{
		CloseAndQuitItemOpMode(); // ���� �ϳ� �����Ѵٸ�.. ��¿ �� ����.
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

	if (TB_UpgradeCostNum.IsValid()) // �±� ����� �Ƹ��� ���������� �� �������� ���� �� �� �������� ������ �ް� �Ǹ� ����� �����ϰ� �� ��.
	{
		int32 UpgradeGoldCost = CachedInventory ? CachedInventory->GetCurrentItemOpGoldCost() : 0;
		int32 CurrentGold = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();

		TB_UpgradeCostNum->SetText(FText::FromString(FString::FromInt(UpgradeGoldCost)));
		TB_UpgradeCostNum->SetColorAndOpacity(
			(CurrentGold >= UpgradeGoldCost) ?
			// ���� �ؽ�Ʈ�� �ϵ��ڵ� �Ͽ��µ� ���� ���⵵ Ư���� �������� �ְ� �ʹٰ� �ϸ� property �� ������.
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
			// ���� �ؽ�Ʈ�� �ϵ��ڵ� �Ͽ��µ� ���� ���⵵ Ư���� �������� �ְ� �ʹٰ� �ϸ� property �� ������.
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

	// ���� �±� ������ ��� �����Կ� �� �̻� ���ٸ� �޽����� �ѷ��ش�.
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
	// ���� ��Ʈ��
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

	// ���� ��Ʈ ����
	if (EnhUpgCommonNRef)
	{
		EnhUpgCommonNRef->DestroySelf();
		EnhUpgCommonNRef = NULL;
	}
}

void UB2LobbyUI_ItemUpgradeMain::OnUpgradeCommit()
{
	if (CachedInventory && !CachedInventory->HasEnoughUpgradeStoneForCurrentTarget())
	{ // �±޼� ������ ��� ���� Ŭ���� ���� ������ ���� ������ ���ϰ� �ݰݴ��������� ������ ����.
		const int32 CurrentUpdgradeStoneID(FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(NativeUpgradeTargetData));
		ShortageMGR->PopupGoToCounterDungeonForUpgradeStone(CurrentUpdgradeStoneID);
	}
	else if (CachedInventory && !CachedInventory->CanAffordCurrentItemOp())
	{ // ��� ������ ��� ���� Ŭ���� ���� ������ ���� ������ ���ϰ� ����� ������ ����
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{
		// �ִϸ��̼� ��ȣ����.
		BeginUpgradeCommitAnim();
		if (EnhUpgCommonNRef)
		{
			EnhUpgCommonNRef->BeginUpgradeCommitAnim();
		}

		if (CachedInventory)
		{// ����Ǵ� ���� ������ ��� ���� ���� �ϰ� �� �� �־ ���´�.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_UpgradeCommit.IsValid())
		{
			BTN_UpgradeCommit->SetIsEnabled(false); // �굵 ��� Ŭ���� �� �� �־ ����.
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
{// ���⼭ �ٷ� ��ȭâ�� �ݴ� ��� Inventory ������ ��ȣ.
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

	// ���� ����. ���� �����̳� ��� ���� flow �� ���� ���信 ���� ���� UI â���� ����.
	LobbyReqUpgradeItemClass<>::GetInstance().Signal();
}