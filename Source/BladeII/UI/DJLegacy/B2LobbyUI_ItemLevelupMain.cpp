// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
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
	
	// �ܼ��� ������ ������ �������� Inventory �ν��Ͻ� ���� static ���� ���� �� ������ ������ ����� �κ��丮�� �� �ִ� ���¿��� ����.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// �� ������ Inventory �� TargetItemID �� ���õǾ� �־�� ��. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeTargetItem) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Levelup)
	{
		CloseAndQuitItemOpMode(); // ���� �ϳ� �����Ѵٸ�.. ��¿ �� ����.
		return;
	}

	SetPositionInViewport(FVector2D(0.0f, 0.0f));

	UpdateStaticTexts();
	UpdatePostItemLevelupExpValues();
	UpdateDynamicTexts();
	UpdateItemLevelupCommitBtnState();

	LevelExpProgressAnim.SyncToTargetValues(); // Ÿ�ٰ����� UpdatePostItemLevelupExpValues ���� �� ���� ��.

	if (EnhUpgCommonNRef)
	{
		EnhUpgCommonNRef->StartFromLobby(InUIManager, InGM);
		EnhUpgCommonNRef->SetItemOpTargetData(NativeTargetItem, true); // Enhancement mode ��.
	}

	LevelExpProgressAnim.OnLevelChanged.AddUObject(this, &UB2LobbyUI_ItemLevelupMain::OnUpdateProgressLevel);
	LevelExpProgressAnim.OnPrecentChanged.AddUObject(this, &UB2LobbyUI_ItemLevelupMain::OnUpdateProgressPercent);
}

void UB2LobbyUI_ItemLevelupMain::UpdateOnLevelupStateChange()
{
	UpdateStaticTexts(); // ������ �޴� �Ϸ� ���� �ٷ� ��� Ȯ�� �� StaticText �� ������Ʈ �ϴ� �ǵ�, ���� �ʿ����� �� �ڵ������� �־ ū�� �� �͵� ����.
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
	// �����̶� Exp �� �ִϸ��̼� ������ ���� ���� �� ƽ ������Ʈ.
	if (TB_EnhanceCostNum.IsValid()) // ������ ��� ���� ���� ������ ����� �ٲ�.
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
	
	// ������ �����ȰŴ� ����~
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
			CurrElem->UpdateItemData(ThisIngred); // ���� ������ ������ �Ѱ��ش�.
		}
		else
		{
			CurrElem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	// ���� �߰��Ȱ͸� ���� ����..
	for (int32 II = CreatedIngredientItemIcons.Num(); II < AllIngredIDs.Num(); ++II)
	{
		FB2Item ThisIngred;
		if (!UB2LobbyInventory::FindFromAll(ThisIngred, AllIngredIDs[II]))
			continue;

		UB2DynItemIcon_EnhanceIngred* NewItemIcon = CreateItemIcon();
		if (NewItemIcon)
		{
			NewItemIcon->UpdateItemData(ThisIngred); // ���� ������ ������ �Ѱ��ش�.
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
		CreatedWidget->SetDynCreatedDummyParent(OuterSB); // CreatedWidget �� ������ �� OuterSB �� ���� ���ŵǵ��� ���.
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
	PostItemLevel = GetLevel_PreviewItemLevelup(PostExp, NativeTargetItem, AllIngredsData, true); // UI �� �����ַ��� clamp �� �ؾ߰ڴ� ��
	PostExp *= 0.01f; // 0 ~ 1 ���̷� �����ϸ�.

	LevelExpProgressAnim.ApplyCustomTimerTick(this);
	LevelExpProgressAnim.SetTargetValues(PostItemLevel, PostExp, false);
}

void UB2LobbyUI_ItemLevelupMain::UpdatePostItemLevelupOptionStatTexts()
{
	// ���� ��Ʈ��
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
			// ���� üũ�ؾ� ��. -> üũ���� �ʴ´�. Ŭ���� �����ϵ��� �ؼ� ����� ������ �����Ѵ�.
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
	// �ִ� �������� ���� ä���ֱ�
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

	// ���� ��Ʈ ����
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
	{ // ��� ������ ��� ���� Ŭ���� ���� ������ ���� ������ ���ϰ� ����� ������ ����
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{ // ������ �����ϴٰ� �Ǵܵ� ���

		// �ִϸ��̼� ��ȣ�� ������ �ٷ� ���������� �ʴ´�.
		BeginEnhanceCommitAnim();
		if (EnhUpgCommonNRef)
		{
			EnhUpgCommonNRef->BeginEnhanceCommitAnim();
		}

		if (CachedInventory)
		{// ����Ǵ� ���� ������ ��� ���� ���� �ϰ� �� �� �־ ���´�.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_EnhanceCommit.IsValid())
		{
			BTN_EnhanceCommit->SetIsEnabled(false); // �굵 ��� Ŭ���� �� �� �־ ����.
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

// Tutorial 400 ������ ������ ���� Protocol�� ����ϴ� 
TUTORIAL void UB2LobbyUI_ItemLevelupMain::Tutorial_FinalItemLevelUpCommit()
{
	// ������ Ʃ�丮�� ���� Protocol�� �ޱ� �������� �Ϲ� Protocol��
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
{// ���⼭ �ٷ� ������â�� �ݴ� ��� Inventory ������ ��ȣ.
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
		// ���� ����. ���� �����̳� ��� ���� flow �� ���� ���信 ���� ���� UI â���� ����.
		LobbyReqItemLevelupWithCurrentSelectionClass<>::GetInstance().Signal();
	}
}
