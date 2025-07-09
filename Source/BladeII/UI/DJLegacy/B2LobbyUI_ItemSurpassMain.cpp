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

	// 단순히 아이템 정보만 얻어오려면 Inventory 인스턴스 없이 static 으로 얻어올 수 있으나 합성 등의 기능은 인벤토리가 떠 있는 상태에서 동작.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	// 이 시점에 Inventory 의 TargetItemID 는 세팅되어 있어야 함. (UB2LobbyInventory::EnterItemOpMode)
	if (!CachedInventory->GetItemOPTargetItemData(NativeSurpassTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Surpass)
	{
		CloseAndQuitItemOpMode(); // 만에 하나 실패한다면.. 어쩔 수 없지.
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
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassNote")), FText::AsNumber(1) // 1 이상 올라갈 일이 있나? ㅋ : 있네!
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

	if (TB_SurpassCostNum.IsValid()) // 초월 비용이 일정하더라도 이 페이지를 열고 난 후 서버에서 응답을 받게 되면 제대로 세팅하게 될 것.
	{
		TB_SurpassCostNum->SetText(FText::FromString(FString::FromInt(CachedInventory ? CachedInventory->GetCurrentItemOpGoldCost() : 0)));
	}

	// 현재 초월 재료로 사용 가능한 장비가 보관함에 더 이상 없다면 메시지를 뿌려준다.
	TArray<FB2Item> CurrPCItemList;
	CachedInventory->GetStoredItemList_ItemOpModeFilter(CurrPCItemList, CachedInventory->GetMainInvenViewCurrentTab(), EPCClass::EPC_End, true);
	if (CurrPCItemList.Num() == 0)
	{
		const wchar_t* GuideText;	// 출력돼야 되는 문구 키값을 저장할 변수.
		if (NativeSurpassTargetData.StarGrade >= FItemGradeInfo::MINIMUM_ADVANCED_SURPASS_STAR_GRADE)	// 11성 이상일 경우
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

	// 재료 선택 상태에 따른 visibility..
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

		// Surpass 정보

		FItemSurpassLevel InSurpassInfo = BladeIIGameImpl::GetClientDataStore().GetSurpassLevelData(SumSurpassData.StarGrade);
		FItemSurpassLevel InCurrentItemSurpassInfo = BladeIIGameImpl::GetClientDataStore().GetSurpassLevelData(NativeSurpassTargetData.StarGrade);
		
		// 대체로 P_Selected 아래에 배치된 것들.

		// 초월 이후 정보는 이 시점엔 없을 것임. 어차피 최대 강화레벨 하나 증가하는 거.
		const int32 LimitLevel = FB2Item::ItemNormalMaxLevel + InCurrentItemSurpassInfo.MaxSurpassLevel; // 아이템별 최대 초월레벨
		const int32 CurrLevel = NativeSurpassTargetData.Level;
		const int32 CurrMaxLevel = NativeSurpassTargetData.MaxLevel;
		const int32 NextSurpassStep = NativeSurpassTargetData.SurpassCount + InSurpassInfo.IncreaseLevel; // 다음 초월 레벨

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
			TB_CurrMaxLevelLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassMaxLevelLabel"))); // 걍 고정 텍스트.
		}
		if (TB_CurrMaxLevel.IsValid())
		{
			TB_CurrMaxLevel->SetText(FText::AsNumber(CurrMaxLevel));
		}
		if (TB_PostSurpassMaxLevelLabel.IsValid())
		{
			TB_PostSurpassMaxLevelLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_SurpassMaxLevelLabel"))); // 걍 고정 텍스트.
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

		// MainPanelNativeRef 위에 최종적인 아이템 아이콘 widget 생성
		CreatedIngredItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
			ItemIconWidgetClass, this, CP_IngredItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
			FVector2D(0.0f, 0.0f), 0, false
			));
		if (CreatedIngredItemIcon)
		{
			CreatedIngredItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ComposeOrSurpassIngredient);
			CreatedIngredItemIcon->UpdateItemData(SumSurpassData); // 개별 아이템 정보를 넘겨준다.
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
			// 돈도 체크해야 함. -> 체크하지 않는다. 클릭은 가능하도록 해서 사용자 현질을 유도한다.
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
	{ // 골드 부족의 경우 여길 클릭할 수는 있으나 실제 시작은 안하고 사용자 현질을 유도
		ShortageMGR->PopupGoToShopForGoldMessage();
	}
	else
	{
		// 애니메이션 신호부터.
		BeginSurpassCommitAnim();

		if (CachedInventory)
		{// 연출되는 동안 여전히 재료 선택 등을 하게 될 수 있어서 막는다.
			CachedInventory->SetFreezeitemOpModeIngredSelection(true);
		}
		if (BTN_SurpassCommit.IsValid())
		{
			BTN_SurpassCommit->SetIsEnabled(false); // 얘도 계속 클릭이 될 수 있어서 막음.
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
		// 물론 여기선 IngredIDs 는 하나여야
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

	// 본격 동작. 이후 연출이나 결과 등의 flow 는 서버 응답에 따라 별도 UI 창에서 진행.
	LobbyReqSurpassItemWithCurrentSelectionClass<>::GetInstance().Signal();
}