// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_ItemEnhance.h"
#include "B2LobbyUI_ItemEnhUpgCommon.h"
#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2UIShortagePopupManager.h"
#include "TutorialManager.h"


#include "BladeIIGameImpl.h"

UB2LobbyUI_ItemEnhance::UB2LobbyUI_ItemEnhance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemIconWidgetClass = NULL;
	EnhanceCommitAnimLength = 0.5f;
}

void UB2LobbyUI_ItemEnhance::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_EnhanceTitle)
	GET_SLOT(UTextBlock, TB_EnhanceCommitBtn)
	GET_SLOT(UB2Button, BTN_EnhanceCommit)
	BIND_CLICK_FUNC_TO_BTN(BTN_EnhanceCommit, &UB2LobbyUI_ItemEnhance::OnEnhanceCommit)
	GET_SLOT(UB2Button, BTN_Close)
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_ItemEnhance::OnCloseButtonPressed)
	GET_SLOT(UB2Button, BTN_EnhanceIngredItem)
	BIND_CLICK_FUNC_TO_BTN(BTN_EnhanceIngredItem, &UB2LobbyUI_ItemEnhance::OnClickEnhanceIngredItem)

	GET_SLOT(UTextBlock, TB_EnhanceCostNum)
	GET_SLOT(UCanvasPanel, CP_EnhanceTargetItem)
	GET_SLOT(UCanvasPanel, CP_EnhanceIngredItem)
	GET_SLOT(UCanvasPanel, CP_SelectItemExplain)
	GET_SLOT(UCanvasPanel, CP_EnhanceExplain)
	GET_SLOT(UTextBlock, TB_NeedIngredItem)
	GET_SLOT(UTextBlock, TB_NeedIngredItemCount)
	GET_SLOT(UB2RichTextBlock, RTB_IngredItemCount)
	GET_SLOT(UTextBlock, TB_SelectItemExplainTitle)
	GET_SLOT(UB2RichTextBlock, RTB_SelectItemExplain)
	GET_SLOT(UTextBlock, TB_ProtectionItem)
	GET_SLOT(UB2RichTextBlock, RTB_ProtectionItemCount)
	GET_SLOT(UTextBlock, TB_ProtectionItemNotice)


	GET_SLOT(UTextBlock, TB_StatValueOld)
	GET_SLOT(UTextBlock, TB_StatValueNew)
	GET_SLOT(UTextBlock, TB_StatIncreaseValue)
	GET_SLOT(UTextBlock, TB_StatKey)
	GET_SLOT(UB2RichTextBlock, RTB_ProbabilityOfSuccess)
	GET_SLOT(UB2RichTextBlock, RTB_EnhanceFailNotice)
	GET_SLOT(UCheckBox, CB_UseProtectionItem)
}

void UB2LobbyUI_ItemEnhance::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	// 단순히 아이템 정보만 얻어오려면 Inventory 인스턴스 없이 static 으로 얻어올 수 있으나 강화 기능은 인벤토리가 떠 있는 상태에서 동작.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// 이 시점에 Inventory 의 TargetItemID 는 세팅되어 있어야 함. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeTargetItem) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Enhancement)
	{
		CloseAndQuitItemOpMode(); // 만에 하나 실패한다면.. 어쩔 수 없지.
		return;
	}

	GetCalculateItem_PreviewItemEnhance(NativeTargetItem, NativeTargetEnhanceLevelUpItem, 1);
	SetPositionInViewport(FVector2D(0.0f, 0.0f));
	UpdateStaticTexts();
	UpdateEnhanceTargetItemIcon();
	UpdateDynamicTexts();
	UpdateItemEnhanceCommitBtnState();
	UpdateIsProtectionItemCheckBoxState();
}

void UB2LobbyUI_ItemEnhance::UpdateOnEnhanceStateChange()
{	
	UpdateDynamicTexts();	
	UpdateEnhanceIngredientItemIcon();
	UpdateEnhanceExplain();
	UpdateItemEnhanceCommitBtnState();
}

void UB2LobbyUI_ItemEnhance::UpdateStaticTexts()
{
	if (TB_EnhanceTitle.IsValid())
		TB_EnhanceTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemEnhancement")));

	if (TB_EnhanceCommitBtn.IsValid())
		TB_EnhanceCommitBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ItemEnhancement")));

	if(RTB_SelectItemExplain.IsValid())
		RTB_SelectItemExplain->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_UpgradescrollDesc2")));

	if(TB_SelectItemExplainTitle.IsValid())
		TB_SelectItemExplainTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_UpgradescrollDesc1")));

	if(TB_NeedIngredItem.IsValid())
		TB_NeedIngredItem->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Upgradescroll")));

	if (TB_ProtectionItemNotice.IsValid())
		TB_ProtectionItemNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_UpgradescrollDesc4")));  //수호의 결정 사용시 하락이 방지됩니다

	if (TB_ProtectionItem.IsValid())
		TB_ProtectionItem->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_UpgradescrollDesc5")));  //수호의 결정 사용시 하락이 방지됩니다

}

void UB2LobbyUI_ItemEnhance::UpdateDynamicTexts()
{
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();

	TArray<FB2Item> AllIngreds;
	CachedInventory->GetAllSelectedItemOpIngreds(AllIngreds);


	TArray<FB2Item> CurrPCItemList;
	CachedInventory->GetStoredItemList_ItemOpModeFilter(CurrPCItemList, CachedInventory->GetMainInvenViewCurrentTab(), EPCClass::EPC_End, true);
	if (CurrPCItemList.Num() == 0)
	{
		LobbyInvenSetItemOpModeGuideTextClass<const FText&>::GetInstance().Signal(
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenTextUpgradeScrollDesc3"))
		);
	}
	else
	{
		LobbyInvenHideItemOpModeGuideTextClass<>::GetInstance().Signal();
	}
	
	// 레벨이랑 Exp 는 애니메이션 때문에 여기 말고 매 틱 업데이트.
	if (TB_EnhanceCostNum.IsValid()) // 강화 재료 개수 따라 강화 비용이 바뀜.
		TB_EnhanceCostNum->SetText(FText::FromString(FString::FromInt( CachedInventory ? CachedInventory->GetCurrentItemOpGoldCost() : 0 )));

	if (TB_NeedIngredItemCount.IsValid())
	{
		TB_NeedIngredItemCount->SetText(
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_ItemCount"))), 
				FText::AsNumber(CachedInventory ? CachedInventory->GetCurEnhanceItemScrollCost() : 0))
		);
	}

	if (RTB_IngredItemCount.IsValid())
	{
		const int32 PossessionCount = AllIngreds.Num() > 0 ? UB2LobbyInventory::GetSharedConsumableAmountOfType(AllIngreds[0].ItemRefID) : 0;
		const int32 NeedCount = CachedInventory ? CachedInventory->GetCurEnhanceItemScrollCost() : 0;

		FText FormatText = GetIngredColorFormat(PossessionCount, NeedCount, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_InvitemCount"))));
 		RTB_IngredItemCount->SetText(FormatText);
		RTB_IngredItemCount->SetVisibility(AllIngreds.Num() > 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}

	if (TB_StatKey.IsValid())
		TB_StatKey->SetText(GetLOCTextOfPrimPointType(NativeTargetItem.PrimaryPointType));

	if (TB_StatValueOld.IsValid())
		TB_StatValueOld->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)NativeTargetItem.GetPrimaryPoint())));
	
	if (TB_StatValueNew.IsValid())
		TB_StatValueNew->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)NativeTargetEnhanceLevelUpItem.GetPrimaryPoint())));

	if (TB_StatIncreaseValue.IsValid())
	{
		int32 PrimaryPointDelta = (int32)NativeTargetEnhanceLevelUpItem.GetPrimaryPoint() - (int32)NativeTargetItem.GetPrimaryPoint();
		TB_StatIncreaseValue->SetText(FText::FromString(FString::Printf(TEXT("( %s%d )"), TEXT("+"), FMath::Abs(PrimaryPointDelta))));
	}

	if (RTB_ProtectionItemCount.IsValid())
	{
		const int32 PossessionCount = CachedInventory ? CachedInventory->GetEnhanceProtectionItemCount() : 0;
		const int32 NeedCount = CachedInventory ? CachedInventory->GetCurEnhanceItemProtectionScrollCost() : 0;

		FText FormatText = GetIngredColorFormat(PossessionCount, NeedCount, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_InvitemCount"))));
		RTB_ProtectionItemCount->SetText(FText::FromString(FString::Printf(TEXT("(%s)"), *FString(FormatText.ToString()))));
	}

	if (RTB_ProbabilityOfSuccess.IsValid())
	{
		RTB_ProbabilityOfSuccess->SetText(GetProbabilityOfSuccessText());
	}		
}

void UB2LobbyUI_ItemEnhance::UpdateEnhanceIngredientItemIcon()
{
	if (CreatedIngredientItemIcon)
		CreatedIngredientItemIcon->DestroySelf();

	if (ItemIconWidgetClass == NULL || !CP_EnhanceIngredItem.IsValid() || !CachedInventory)
		return;

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(CP_EnhanceTargetItem->Slot);
	FVector2D AllowedIconSize = PanelSlot ? PanelSlot->GetSize() : FVector2D(0.0f, 0.0f);
	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	TArray<int64> AllIngredIDs;
	CachedInventory->GetAllSelectedItemOpIngreds(AllIngredIDs);

	if (AllIngredIDs.Num() == 0)
		return;

	FB2Item ThisIngred;
	if (!UB2LobbyInventory::FindFromAll(ThisIngred, AllIngredIDs[0]))
		return;

	CreatedIngredientItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
		ItemIconWidgetClass, this, CP_EnhanceIngredItem.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
		FVector2D(0.0f, 0.0f), 0, false));

	if (CreatedIngredientItemIcon)
	{
		CreatedIngredientItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemEnhanceMenuIngredient);
		CreatedIngredientItemIcon->UpdateItemData(ThisIngred); // 개별 아이템 정보를 넘겨준다.
	}


	TUTORIAL if (TutorialManager::GetInstance().IsCurrentTutorial(TutorialID_ItemEnhance, 6))
	{
		TutorialManager::GetInstance().OnEnhanceIngredRegistered(ThisIngred);
	}
}

void UB2LobbyUI_ItemEnhance::UpdateEnhanceTargetItemIcon()
{
	if (CreatedTargetItemIcon)
		CreatedTargetItemIcon->DestroySelf();

	if (ItemIconWidgetClass == NULL || !CP_EnhanceTargetItem.IsValid())
		return;

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(CP_EnhanceTargetItem->Slot);
	FVector2D AllowedIconSize = PanelSlot ? PanelSlot->GetSize() : FVector2D(0.0f, 0.0f);
	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	CreatedTargetItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
		ItemIconWidgetClass, this, CP_EnhanceTargetItem.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
		FVector2D(0.0f, 0.0f), 0, false));

	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeProgTarget);
		CreatedTargetItemIcon->UpdateItemData(NativeTargetItem); // 개별 아이템 정보를 넘겨준다.
	}
}

void UB2LobbyUI_ItemEnhance::UpdateEnhanceExplain()
{
	bool IsSelectedEnhanceItem = (CachedInventory && CachedInventory->GetCurrentSelectIngredItemNum() > 0);
	if (CP_SelectItemExplain.IsValid())
		CP_SelectItemExplain->SetVisibility(IsSelectedEnhanceItem ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);

	if (CP_EnhanceExplain.IsValid())
		CP_EnhanceExplain->SetVisibility(IsSelectedEnhanceItem ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (IsSelectedEnhanceItem && RTB_EnhanceFailNotice.IsValid())
	{
		RTB_EnhanceFailNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, 
			IsItemSafeEnhanceLevel(NativeTargetItem) ? TEXT("LobbyInvenText_UpgradescrollDesc7") : TEXT("LobbyInvenText_UpgradescrollDesc6")));
	}
}

void UB2LobbyUI_ItemEnhance::UpdateItemEnhanceCommitBtnState()
{
	if (CB_UseProtectionItem.IsValid())
		CB_UseProtectionItem->SetIsEnabled(CachedInventory && CachedInventory->IsSelectableEnhanceProtectionItem());

	if (BTN_EnhanceCommit.IsValid())
		BTN_EnhanceCommit->SetIsEnabled(CachedInventory && CachedInventory->GetCurrentSelectIngredItemNum() > 0);
}

void UB2LobbyUI_ItemEnhance::UpdateIsProtectionItemCheckBoxState()
{
	if (CB_UseProtectionItem.IsValid())
		CB_UseProtectionItem->SetIsChecked(CachedInventory && CachedInventory->IsSelectableEnhanceProtectionItem() 
			&& CachedInventory->IsSelectedEnhanceProtectionItem() && !IsItemSafeEnhanceLevel(NativeTargetItem));
}

void UB2LobbyUI_ItemEnhance::OnChangeStateIsProtectionItemCheckBox(bool IsCheck)
{
	if (IsCheck && IsItemSafeEnhanceLevel(NativeTargetItem))
	{
		UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SafeEnhanceLevelPopup")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm);

		SelectForEnhanceProtectionItemClass<int32>::GetInstance().Signal(0);

		if (CB_UseProtectionItem.IsValid())
			CB_UseProtectionItem->SetIsChecked(false);
		return;
	}

	int32 ProtectionRefId = IsCheck ? FItemRefIDHelper::GetEnhanceProtectionItemRefId() : 0; // 일단은 강화 보호 아이템 타입 하나.. =ㅇ=
	SelectForEnhanceProtectionItemClass<int32>::GetInstance().Signal(ProtectionRefId);
}

void UB2LobbyUI_ItemEnhance::DestroySelf()
{
	Super::DestroySelf();

	if (CreatedTargetItemIcon)
		CreatedTargetItemIcon->DestroySelf();

	if (CreatedIngredientItemIcon)
		CreatedIngredientItemIcon->DestroySelf();
}

void UB2LobbyUI_ItemEnhance::OnEnhanceCommit()
{
	TArray<FB2Item> AllIngreds;
	CachedInventory->GetAllSelectedItemOpIngreds(AllIngreds);

	int32 IngredItemCount = AllIngreds.Num() > 0 ? UB2LobbyInventory::GetSharedConsumableAmountOfType(AllIngreds[0].ItemRefID) : 0;
	int32 ScrollCost = CachedInventory ? CachedInventory->GetCurEnhanceItemScrollCost() : 0;
	
	if (IngredItemCount == 0 || IngredItemCount < ScrollCost)
	{
		ShortageMGR->PopupGoToControlGameModeMessage(AllIngreds[0].ItemRefID);
	}
	else if (CachedInventory && !CachedInventory->CanAffordCurrentItemOp())
	{ // 골드 부족의 경우 여길 클릭할 수는 있으나 실제 시작은 안하고 사용자 현질을 유도
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{ 
		// 강화 가능하다고 판단된 경우
		// 애니메이션 신호만 보내고 바로 시작하지는 않는다.
		BeginEnhanceCommitAnim();

		// 연출되는 동안 여전히 재료 선택 등을 하게 될 수 있어서 막는다.
		if (CachedInventory)		
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);

		if (BTN_EnhanceCommit.IsValid())
			BTN_EnhanceCommit->SetIsEnabled(false); // 얘도 계속 클릭이 될 수 있어서 막음.

		APlayerController* OwningPC = GetOwningPlayer();
		if (OwningPC && EnhanceCommitAnimLength > 0.0f)
		{
			OwningPC->GetWorldTimerManager().SetTimer(ItemEnhanceCommintTH, this, &UB2LobbyUI_ItemEnhance::FinalItemEnhanceCommit, EnhanceCommitAnimLength, false);
		}
		else
		{
			FinalItemEnhanceCommit();
		}
	}
}

void UB2LobbyUI_ItemEnhance::CloseWidgetDelegate()
{
	OnCloseButtonPressed();
}

void UB2LobbyUI_ItemEnhance::OnCloseButtonPressed()
{
	CloseAndQuitItemOpMode();
}

void UB2LobbyUI_ItemEnhance::OnClickEnhanceIngredItem()
{
	if (!CachedInventory)
		return;

	TArray<int64> AllIngredIDs;
	CachedInventory->GetAllSelectedItemOpIngreds(AllIngredIDs);
	
	if (AllIngredIDs.Num() == 0)
		return;
	
	FB2Item ThisIngred;
	if (!UB2LobbyInventory::FindFromAll(ThisIngred, AllIngredIDs[0]))
		return;

	UnSelectForEnhanceIngredClass<int64>::GetInstance().Signal(ThisIngred.InstanceUID);
}

void UB2LobbyUI_ItemEnhance::CloseAndQuitItemOpMode()
{// 여기서 바로 강화창을 닫는 대신 Inventory 쪽으로 신호.
	QuitItemOpModeClass<bool>::GetInstance().Signal(false);
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();
}

void UB2LobbyUI_ItemEnhance::FinalItemEnhanceCommit()
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
		OwningPC->GetWorldTimerManager().ClearTimer(ItemEnhanceCommintTH);

	// 본격 동작. 이후 연출이나 결과 등의 flow 는 서버 응답에 따라 별도 UI 창에서 진행.
	LobbyReqEnhanceItemClass<>::GetInstance().Signal();
}

FText UB2LobbyUI_ItemEnhance::GetProbabilityOfSuccessText()
{
	const int32 Percent = CachedInventory ? (CachedInventory->GetCurEnhanceItemSuccessRate() / 100.0f) : 0;
	FString ColorText = FColor::White.ToHex();

	for (const auto &Each : EnhanceConditions)
	{
		if (Each.CompareValue <= Percent)
		{
			ColorText = Each.ViewColor.ToHex();
			break;
		}
	}

	return FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL),
				FString(TEXT("LobbyInvenText_ProbabilityOfSuccess"))), FText::FromString(ColorText), FText::AsNumber(Percent));
}

FText UB2LobbyUI_ItemEnhance::GetIngredColorFormat(const int32 PossessionCount, const int32 NeedCount, const FText &AppendText)
{
	const bool bCanEnhance = NeedCount <= PossessionCount;

	FString ColorText = bCanEnhance ? EnhanceEnable.ToHex() : EnhanceDisable.ToHex();

	return FText::Format(AppendText, FText::FromString(ColorText), FText::AsNumber(PossessionCount), FText::AsNumber(NeedCount));
}