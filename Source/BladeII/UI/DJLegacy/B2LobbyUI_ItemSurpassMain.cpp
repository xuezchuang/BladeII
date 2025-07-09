// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
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

	// �ܼ��� ������ ������ �������� Inventory �ν��Ͻ� ���� static ���� ���� �� ������ �ռ� ���� ����� �κ��丮�� �� �ִ� ���¿��� ����.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// �� ������ Inventory �� TargetItemID �� ���õǾ� �־�� ��. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeSurpassTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Surpass)
	{
		CloseAndQuitItemOpMode(); // ���� �ϳ� �����Ѵٸ�.. ��¿ �� ����.
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
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassNote")), FText::AsNumber(1) // 1 �̻� �ö� ���� �ֳ�? �� : �ֳ�!
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

	if (TB_SurpassCostNum.IsValid()) // �ʿ� ����� �����ϴ��� �� �������� ���� �� �� �������� ������ �ް� �Ǹ� ����� �����ϰ� �� ��.
	{
		TB_SurpassCostNum->SetText(FText::FromString(FString::FromInt(CachedInventory ? CachedInventory->GetCurrentItemOpGoldCost() : 0)));
	}

	// ���� �ʿ� ���� ��� ������ ��� �����Կ� �� �̻� ���ٸ� �޽����� �ѷ��ش�.
	TArray<FB2Item> CurrPCItemList;
	CachedInventory->GetStoredItemList_ItemOpModeFilter(CurrPCItemList, CachedInventory->GetMainInvenViewCurrentTab(), EPCClass::EPC_End, true);
	if (CurrPCItemList.Num() == 0)
	{
		const wchar_t* GuideText;	// ��µž� �Ǵ� ���� Ű���� ������ ����.
		if (NativeSurpassTargetData.StarGrade >= FItemGradeInfo::MINIMUM_ADVANCED_SURPASS_STAR_GRADE)	// 11�� �̻��� ���
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

	// ��� ���� ���¿� ���� visibility..
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

		// Surpass ����

		FItemSurpassLevel InSurpassInfo = BladeIIGameImpl::GetClientDataStore().GetSurpassLevelData(SumSurpassData.StarGrade);
		FItemSurpassLevel InCurrentItemSurpassInfo = BladeIIGameImpl::GetClientDataStore().GetSurpassLevelData(NativeSurpassTargetData.StarGrade);
		
		// ��ü�� P_Selected �Ʒ��� ��ġ�� �͵�.

		// �ʿ� ���� ������ �� ������ ���� ����. ������ �ִ� ��ȭ���� �ϳ� �����ϴ� ��.
		const int32 LimitLevel = FB2Item::ItemNormalMaxLevel + InCurrentItemSurpassInfo.MaxSurpassLevel; // �����ۺ� �ִ� �ʿ�����
		const int32 CurrLevel = NativeSurpassTargetData.Level;
		const int32 CurrMaxLevel = NativeSurpassTargetData.MaxLevel;
		const int32 NextSurpassStep = NativeSurpassTargetData.SurpassCount + InSurpassInfo.IncreaseLevel; // ���� �ʿ� ����

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
			TB_CurrMaxLevelLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassMaxLevelLabel"))); // �� ���� �ؽ�Ʈ.
		}
		if (TB_CurrMaxLevel.IsValid())
		{
			TB_CurrMaxLevel->SetText(FText::AsNumber(CurrMaxLevel));
		}
		if (TB_PostSurpassMaxLevelLabel.IsValid())
		{
			TB_PostSurpassMaxLevelLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassMaxLevelLabel"))); // �� ���� �ؽ�Ʈ.
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

		// MainPanelNativeRef ���� �������� ������ ������ widget ����
		CreatedIngredItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
			ItemIconWidgetClass, this, CP_IngredItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
			FVector2D(0.0f, 0.0f), 0, false
			));
		if (CreatedIngredItemIcon)
		{
			CreatedIngredItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ComposeOrSurpassIngredient);
			CreatedIngredItemIcon->UpdateItemData(SumSurpassData); // ���� ������ ������ �Ѱ��ش�.
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
			// ���� üũ�ؾ� ��. -> üũ���� �ʴ´�. Ŭ���� �����ϵ��� �ؼ� ����� ������ �����Ѵ�.
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
	{ // ��� ������ ��� ���� Ŭ���� ���� ������ ���� ������ ���ϰ� ����� ������ ����
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{
		// �ִϸ��̼� ��ȣ����.
		BeginSurpassCommitAnim();

		if (CachedInventory)
		{// ����Ǵ� ���� ������ ��� ���� ���� �ϰ� �� �� �־ ���´�.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_SurpassCommit.IsValid())
		{
			BTN_SurpassCommit->SetIsEnabled(false); // �굵 ��� Ŭ���� �� �� �־ ����.
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
		// ���� ���⼱ IngredIDs �� �ϳ�����
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

	// ���� ����. ���� �����̳� ��� ���� flow �� ���� ���信 ���� ���� UI â���� ����.
	LobbyReqSurpassItemWithCurrentSelectionClass<>::GetInstance().Signal();
}