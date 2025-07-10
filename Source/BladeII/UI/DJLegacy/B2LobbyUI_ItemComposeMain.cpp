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
	
	// 단순히 아이템 정보만 얻어오려면 Inventory 인스턴스 없이 static 으로 얻어올 수 있으나 합성 등의 기능은 인벤토리가 떠 있는 상태에서 동작.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// 이 시점에 Inventory 의 TargetItemID 는 세팅되어 있어야 함. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeComposeTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Composition)
	{
		CloseAndQuitItemOpMode(); // 만에 하나 실패한다면.. 어쩔 수 없지.
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

	if (TB_ComposeCostNum.IsValid()) // 합성 비용이 일정하더라도 이 페이지를 열고 난 후 서버에서 응답을 받게 되면 제대로 세팅하게 될 것.
	{
		TB_ComposeCostNum->SetText(FText::FromString(FString::FromInt(CachedInventory ? CachedInventory->GetCurrentItemOpGoldCost() : 0)));
	}

	// 현재 합성 재료로 사용 가능한 장비가 보관함에 더 이상 없다면 메시지를 뿌려준다.
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

	// 이건 하나 증가하는 거니까 여기서 걍 1 플러스 해서
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

	// Anchor 설정에 따라 GetSize 는 원하는 값이 안 나올 것이므로 주의.
	UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(CP_TargetItemIconSet->Slot);
	FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);

	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	// ItemIconPanelNRef 위에 최종적인 아이템 아이콘 widget 생성
	CreatedTargetItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
		ItemIconWidgetClass, this, CP_TargetItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
		FVector2D(0.0f, 0.0f), 0, false
		));

	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget); // 합성도 마찬가지로?
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

		// MainPanelNativeRef 위에 최종적인 아이템 아이콘 widget 생성
		CreatedIngredItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
			ItemIconWidgetClass, this, CP_IngredItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
			FVector2D(0.0f, 0.0f), false
			));
		if (CreatedIngredItemIcon)
		{
			CreatedIngredItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ComposeOrSurpassIngredient);
			CreatedIngredItemIcon->UpdateItemData(ThisIngred); // 개별 아이템 정보를 넘겨준다.
		}
	}
}

void UB2LobbyUI_ItemComposeMain::UpdateComposeCommitBtnState()
{
	if (BTN_ComposeCommit.IsValid())
	{
		BTN_ComposeCommit->SetIsEnabled(
			IsAnyIngredSelected()
			// 돈도 체크해야 함. -> 체크하지 않는다. 클릭은 가능하도록 해서 사용자 현질을 유도한다.
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
	{ // 골드 부족의 경우 여길 클릭할 수는 있으나 실제 시작은 안하고 사용자 현질을 유도
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{
		// 애니메이션 신호부터.
		BeginComposeCommitAnim();

		if (CachedInventory)
		{// 연출되는 동안 여전히 재료 선택 등을 하게 될 수 있어서 막는다.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_ComposeCommit.IsValid())
		{
			BTN_ComposeCommit->SetIsEnabled(false); // 얘도 계속 클릭이 될 수 있어서 막음.
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
		// 물론 여기선 IngredIDs 는 하나여야
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

	// 본격 동작. 이후 연출이나 결과 등의 flow 는 서버 응답에 따라 별도 UI 창에서 진행.
	LobbyReqComposeItemWithCurrentSelectionClass<>::GetInstance().Signal();
}