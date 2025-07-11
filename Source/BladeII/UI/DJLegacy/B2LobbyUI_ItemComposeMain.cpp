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
	
	// �ܼ��� ������ ������ �������� Inventory �ν��Ͻ� ���� static ���� ���� �� ������ �ռ� ���� ����� �κ��丮�� �� �ִ� ���¿��� ����.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// �� ������ Inventory �� TargetItemID �� ���õǾ� �־�� ��. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeComposeTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Composition)
	{
		CloseAndQuitItemOpMode(); // ���� �ϳ� �����Ѵٸ�.. ��¿ �� ����.
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

	if (TB_ComposeCostNum.IsValid()) // �ռ� ����� �����ϴ��� �� �������� ���� �� �� �������� ������ �ް� �Ǹ� ����� �����ϰ� �� ��.
	{
		TB_ComposeCostNum->SetText(FText::FromString(FString::FromInt(CachedInventory ? CachedInventory->GetCurrentItemOpGoldCost() : 0)));
	}

	// ���� �ռ� ���� ��� ������ ��� �����Կ� �� �̻� ���ٸ� �޽����� �ѷ��ش�.
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

	// �̰� �ϳ� �����ϴ� �Ŵϱ� ���⼭ �� 1 �÷��� �ؼ�
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

	// Anchor ������ ���� GetSize �� ���ϴ� ���� �� ���� ���̹Ƿ� ����.
	UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(CP_TargetItemIconSet->Slot);
	FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);

	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	// ItemIconPanelNRef ���� �������� ������ ������ widget ����
	CreatedTargetItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
		ItemIconWidgetClass, this, CP_TargetItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
		FVector2D(0.0f, 0.0f), 0, false
		));

	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget); // �ռ��� ����������?
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

		// MainPanelNativeRef ���� �������� ������ ������ widget ����
		CreatedIngredItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
			ItemIconWidgetClass, this, CP_IngredItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
			FVector2D(0.0f, 0.0f), false
			));
		if (CreatedIngredItemIcon)
		{
			CreatedIngredItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ComposeOrSurpassIngredient);
			CreatedIngredItemIcon->UpdateItemData(ThisIngred); // ���� ������ ������ �Ѱ��ش�.
		}
	}
}

void UB2LobbyUI_ItemComposeMain::UpdateComposeCommitBtnState()
{
	if (BTN_ComposeCommit.IsValid())
	{
		BTN_ComposeCommit->SetIsEnabled(
			IsAnyIngredSelected()
			// ���� üũ�ؾ� ��. -> üũ���� �ʴ´�. Ŭ���� �����ϵ��� �ؼ� ����� ������ �����Ѵ�.
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
	{ // ��� ������ ��� ���� Ŭ���� ���� ������ ���� ������ ���ϰ� ����� ������ ����
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{
		// �ִϸ��̼� ��ȣ����.
		BeginComposeCommitAnim();

		if (CachedInventory)
		{// ����Ǵ� ���� ������ ��� ���� ���� �ϰ� �� �� �־ ���´�.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_ComposeCommit.IsValid())
		{
			BTN_ComposeCommit->SetIsEnabled(false); // �굵 ��� Ŭ���� �� �� �־ ����.
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
		// ���� ���⼱ IngredIDs �� �ϳ�����
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

	// ���� ����. ���� �����̳� ��� ���� flow �� ���� ���信 ���� ���� UI â���� ����.
	LobbyReqComposeItemWithCurrentSelectionClass<>::GetInstance().Signal();
}