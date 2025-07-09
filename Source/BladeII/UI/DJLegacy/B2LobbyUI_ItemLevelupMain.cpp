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
	
	// 단순히 아이템 정보만 얻어오려면 Inventory 인스턴스 없이 static 으로 얻어올 수 있으나 레벨업 기능은 인벤토리가 떠 있는 상태에서 동작.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// 이 시점에 Inventory 의 TargetItemID 는 세팅되어 있어야 함. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeTargetItem) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Levelup)
	{
		CloseAndQuitItemOpMode(); // 만에 하나 실패한다면.. 어쩔 수 없지.
		return;
	}

	SetPositionInViewport(FVector2D(0.0f, 0.0f));

	UpdateStaticTexts();
	UpdatePostItemLevelupExpValues();
	UpdateDynamicTexts();
	UpdateItemLevelupCommitBtnState();

	LevelExpProgressAnim.SyncToTargetValues(); // 타겟값들은 UpdatePostItemLevelupExpValues 에서 들어가 있을 것.

	if (EnhUpgCommonNRef)
	{
		EnhUpgCommonNRef->StartFromLobby(InUIManager, InGM);
		EnhUpgCommonNRef->SetItemOpTargetData(NativeTargetItem, true); // Enhancement mode 로.
	}

	LevelExpProgressAnim.OnLevelChanged.AddUObject(this, &UB2LobbyUI_ItemLevelupMain::OnUpdateProgressLevel);
	LevelExpProgressAnim.OnPrecentChanged.AddUObject(this, &UB2LobbyUI_ItemLevelupMain::OnUpdateProgressPercent);
}

void UB2LobbyUI_ItemLevelupMain::UpdateOnLevelupStateChange()
{
	UpdateStaticTexts(); // 레벨업 메뉴 완료 전에 바로 결과 확인 차 StaticText 도 업데이트 하는 건데, 굳이 필요없어야 할 코드이지만 있어서 큰일 날 것도 없음.
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
	// 레벨이랑 Exp 는 애니메이션 때문에 여기 말고 매 틱 업데이트.
	if (TB_EnhanceCostNum.IsValid()) // 레벨업 재료 개수 따라 레벨업 비용이 바뀜.
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
	
	// 기존에 생성된거는 재사용~
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
			CurrElem->UpdateItemData(ThisIngred); // 개별 아이템 정보를 넘겨준다.
		}
		else
		{
			CurrElem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	// 새로 추가된것만 새로 만든..
	for (int32 II = CreatedIngredientItemIcons.Num(); II < AllIngredIDs.Num(); ++II)
	{
		FB2Item ThisIngred;
		if (!UB2LobbyInventory::FindFromAll(ThisIngred, AllIngredIDs[II]))
			continue;

		UB2DynItemIcon_EnhanceIngred* NewItemIcon = CreateItemIcon();
		if (NewItemIcon)
		{
			NewItemIcon->UpdateItemData(ThisIngred); // 개별 아이템 정보를 넘겨준다.
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
		CreatedWidget->SetDynCreatedDummyParent(OuterSB); // CreatedWidget 을 제거할 때 OuterSB 도 같이 제거되도록 등록.
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
	PostItemLevel = GetLevel_PreviewItemLevelup(PostExp, NativeTargetItem, AllIngredsData, true); // UI 에 보여주려면 clamp 도 해야겠다 ㅋ
	PostExp *= 0.01f; // 0 ~ 1 사이로 스케일링.

	LevelExpProgressAnim.ApplyCustomTimerTick(this);
	LevelExpProgressAnim.SetTargetValues(PostItemLevel, PostExp, false);
}

void UB2LobbyUI_ItemLevelupMain::UpdatePostItemLevelupOptionStatTexts()
{
	// 공통 파트로
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
			// 돈도 체크해야 함. -> 체크하지 않는다. 클릭은 가능하도록 해서 사용자 현질을 유도한다.
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
	// 최대 레벨에선 만땅 채워주기
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

	// 공통 파트 제거
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
	{ // 골드 부족의 경우 여길 클릭할 수는 있으나 실제 시작은 안하고 사용자 현질을 유도
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{ // 레벨업 가능하다고 판단된 경우

		// 애니메이션 신호만 보내고 바로 시작하지는 않는다.
		BeginEnhanceCommitAnim();
		if (EnhUpgCommonNRef)
		{
			EnhUpgCommonNRef->BeginEnhanceCommitAnim();
		}

		if (CachedInventory)
		{// 연출되는 동안 여전히 재료 선택 등을 하게 될 수 있어서 막는다.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_EnhanceCommit.IsValid())
		{
			BTN_EnhanceCommit->SetIsEnabled(false); // 얘도 계속 클릭이 될 수 있어서 막음.
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

// Tutorial 400 아이템 레벨업 전용 Protocol을 사용하는 
TUTORIAL void UB2LobbyUI_ItemLevelupMain::Tutorial_FinalItemLevelUpCommit()
{
	// 서버에 튜토리얼 전용 Protocol을 받기 전까지는 일반 Protocol로
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
{// 여기서 바로 레벨업창을 닫는 대신 Inventory 쪽으로 신호.
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
		// 본격 동작. 이후 연출이나 결과 등의 flow 는 서버 응답에 따라 별도 UI 창에서 진행.
		LobbyReqItemLevelupWithCurrentSelectionClass<>::GetInstance().Signal();
	}
}
