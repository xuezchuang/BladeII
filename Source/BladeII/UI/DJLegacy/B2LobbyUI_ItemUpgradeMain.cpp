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

	// 단순히 아이템 정보만 얻어오려면 Inventory 인스턴스 없이 static 으로 얻어올 수 있으나 강화 기능은 인벤토리가 떠 있는 상태에서 동작.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// 이 시점에 Inventory 의 TargetItemID 는 세팅되어 있어야 함. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeUpgradeTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Upgrade)
	{
		CloseAndQuitItemOpMode(); // 만에 하나 실패한다면.. 어쩔 수 없지.
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

	if (TB_UpgradeCostNum.IsValid()) // 승급 비용은 아마도 일정하지만 이 페이지를 열고 난 후 서버에서 응답을 받게 되면 제대로 세팅하게 될 것.
	{
		int32 UpgradeGoldCost = CachedInventory ? CachedInventory->GetCurrentItemOpGoldCost() : 0;
		int32 CurrentGold = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();

		TB_UpgradeCostNum->SetText(FText::FromString(FString::FromInt(UpgradeGoldCost)));
		TB_UpgradeCostNum->SetColorAndOpacity(
			(CurrentGold >= UpgradeGoldCost) ?
			// 작은 텍스트라 하드코딩 하였는데 만일 여기도 특별한 색상으로 넣고 싶다고 하면 property 로 빼도록.
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
			// 작은 텍스트라 하드코딩 하였는데 만일 여기도 특별한 색상으로 넣고 싶다고 하면 property 로 빼도록.
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

	// 현재 승급 가능한 장비가 보관함에 더 이상 없다면 메시지를 뿌려준다.
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
	// 공통 파트로
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

	// 공통 파트 제거
	if (EnhUpgCommonNRef)
	{
		EnhUpgCommonNRef->DestroySelf();
		EnhUpgCommonNRef = NULL;
	}
}

void UB2LobbyUI_ItemUpgradeMain::OnUpgradeCommit()
{
	if (CachedInventory && !CachedInventory->HasEnoughUpgradeStoneForCurrentTarget())
	{ // 승급석 부족의 경우 여길 클릭할 수는 있으나 실제 시작은 안하고 반격던전으로의 입장을 유도.
		const int32 CurrentUpdgradeStoneID(FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(NativeUpgradeTargetData));
		ShortageMGR->PopupGoToCounterDungeonForUpgradeStone(CurrentUpdgradeStoneID);
	}
	else if (CachedInventory && !CachedInventory->CanAffordCurrentItemOp())
	{ // 골드 부족의 경우 여길 클릭할 수는 있으나 실제 시작은 안하고 사용자 현질을 유도
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{
		// 애니메이션 신호부터.
		BeginUpgradeCommitAnim();
		if (EnhUpgCommonNRef)
		{
			EnhUpgCommonNRef->BeginUpgradeCommitAnim();
		}

		if (CachedInventory)
		{// 연출되는 동안 여전히 재료 선택 등을 하게 될 수 있어서 막는다.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_UpgradeCommit.IsValid())
		{
			BTN_UpgradeCommit->SetIsEnabled(false); // 얘도 계속 클릭이 될 수 있어서 막음.
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
{// 여기서 바로 강화창을 닫는 대신 Inventory 쪽으로 신호.
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

	// 본격 동작. 이후 연출이나 결과 등의 flow 는 서버 응답에 따라 별도 UI 창에서 진행.
	LobbyReqUpgradeItemClass<>::GetInstance().Signal();
}