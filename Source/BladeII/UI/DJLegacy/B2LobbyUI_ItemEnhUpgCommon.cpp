// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_ItemEnhUpgCommon.h"
#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2LobbyUI_InventoryMain.h"
#include "B2DynText_Multiple.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2PCClassInfo.h"
#include "B2LobbyUI_ItemAfterOption.h"

#include "BladeIIGameImpl.h"

UB2LobbyUI_ItemEnhUpgCommon::UB2LobbyUI_ItemEnhUpgCommon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;

	bIsForEnhanceMenu = true;
	ItemIconWidgetClass = NULL;
	CreatedTargetItemIcon = NULL;

	IntrinsicOptionTitleClass = NULL;
	CreatedIntrinsicOptionTitle = NULL;
	RandomOptionTitleClass = NULL;
	CreatedRandomOptionTitle = NULL;
	RandomOptionDisplayClass = NULL;
}

void UB2LobbyUI_ItemEnhUpgCommon::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, X_CP_ItemIconSet)

	GET_SLOT(UCanvasPanel, P_StatSet)
	GET_SLOT(UTextBlock, TB_PrimPointType)
	GET_SLOT(UTextBlock, TB_CurrentPrimPoint)
	GET_SLOT(UTextBlock, TB_PostOpPrimPoint)
	GET_SLOT(UVerticalBox, VB_OptionStats)
	GET_SLOT(UVerticalBox, VB_OptionStats_Upgrade)
}

void UB2LobbyUI_ItemEnhUpgCommon::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	UpdateStaticTexts(); // NativeTargetData 없이도 가능한 것.
}

void UB2LobbyUI_ItemEnhUpgCommon::SetItemOpTargetData(FB2Item& InItem, bool bInIsForEnhanceMenu)
{
	NativeTargetData = InItem;
	bIsForEnhanceMenu = bInIsForEnhanceMenu;

	UpdateStaticTexts();
	UpdateTargetItemIcon(); // Target Item 관련한 것들은 처음에 한번 생성하면 됨.
	UpdateOptionStatTexts();
}

void UB2LobbyUI_ItemEnhUpgCommon::DestroySelf()
{
	Super::DestroySelf();

	DestroyTargetItemIcon();
	DestroyOptionStatTexts();

	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemAfterOpiton);
}

void UB2LobbyUI_ItemEnhUpgCommon::UpdateStaticTexts()
{
	
}

void UB2LobbyUI_ItemEnhUpgCommon::UpdateTargetItemIcon()
{
	DestroyTargetItemIcon();
	
	if (ItemIconWidgetClass == NULL || !X_CP_ItemIconSet.IsValid() || NativeTargetData.InstanceUID <= 0 || NativeTargetData.ItemRefID <= 0){
		return;
	}

	// Anchor 설정에 따라 GetSize 는 원하는 값이 안 나올 것이므로 주의.
	UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(X_CP_ItemIconSet->Slot);
	FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);

	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	// ItemIconPanelNRef 위에 최종적인 아이템 아이콘 widget 생성
	CreatedTargetItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(
		ItemIconWidgetClass, this, X_CP_ItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
		FVector2D(0.0f, 0.0f), 0, false
		));

	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget); // 강화/승급 마찬가지
		CreatedTargetItemIcon->UpdateItemData(NativeTargetData);
	}
}

void UB2LobbyUI_ItemEnhUpgCommon::UpdateOptionStatTexts()
{
	DestroyOptionStatTexts();

	//////////
	// 강화 혹은 승급 후 능력치 계산.
	FB2Item PostOpItemData = NativeTargetData;

	if (bIsForEnhanceMenu)
	{
		TArray<FB2Item> AllIngredsData;
		if (CachedInventory)
			CachedInventory->GetAllSelectedItemOpIngreds(AllIngredsData);
		
		GetCalculateItem_PreviewItemLevelup(NativeTargetData, AllIngredsData, PostOpItemData, true);
	}
	else
	{
		if(CachedInventory)
			PostOpItemData = CachedInventory->GetUpgradeNextItem();
		GetCalculateItem_PreviewItemEnhance(PostOpItemData, PostOpItemData);
	}

	if (TB_PrimPointType.IsValid())
		TB_PrimPointType->SetText(GetLOCTextOfPrimPointType(NativeTargetData.PrimaryPointType));

	if (TB_CurrentPrimPoint.IsValid())
		TB_CurrentPrimPoint->SetText(FText::FromString(FString::FromInt(NativeTargetData.GetPrimaryPoint())));

	if (TB_PostOpPrimPoint.IsValid())
		TB_PostOpPrimPoint->SetText(FText::FromString(FString::FromInt(PostOpItemData.GetPrimaryPoint())));

	if (!bIsForEnhanceMenu)
	{
		const int32 IntrinsicOptionNum = PostOpItemData.IntrinsicOptions.Num(); // 둘이 같아야 할 것.
		if (IntrinsicOptionNum > 0) // 하나라도 있어야 타이틀을 띄우도록
		{
			// 고유 옵션은 승급 메뉴인 경우만
			CreatedIntrinsicOptionTitle = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(IntrinsicOptionTitleClass, this, (bIsForEnhanceMenu ? VB_OptionStats : VB_OptionStats_Upgrade).Get(), FMargin()));
			if (CreatedIntrinsicOptionTitle)
				CreatedIntrinsicOptionTitle->SetDynText(0, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyItemDetail_IntrinsicOptionPanelTitle"))));

			for (int32 OI = 0; OI < IntrinsicOptionNum; ++OI)
			{
				FItemOption CurrOption = NativeTargetData.IntrinsicOptions.IsValidIndex(OI) ? NativeTargetData.IntrinsicOptions[OI] : FItemOption();
				FItemOption PostOption = PostOpItemData.IntrinsicOptions[OI];
				
				if (FMath::IsNearlyEqual(CurrOption.RawOptionAmount, 0.0f))
				{
					CreateIntrinsicOptionNew((bIsForEnhanceMenu ? VB_OptionStats : VB_OptionStats_Upgrade).Get(), CreatedOptionStatTexts, OI, true, PostOption.OptionType, CurrOption.RawOptionAmount, PostOption.RawOptionAmount, IntToPCClass(NativeTargetData.AllowedPCClass));
				}
				else
				{
					CreateSingleItemOptionStatText((bIsForEnhanceMenu ? VB_OptionStats : VB_OptionStats_Upgrade).Get(), CreatedOptionStatTexts, OI, true, CurrOption.OptionType, CurrOption.RawOptionAmount, PostOption.RawOptionAmount, IntToPCClass(NativeTargetData.AllowedPCClass));
				}
			}
		}
	}

	// 랜덤 옵션은 둘 다. 
	const int32 MinRandomOptionNum = FMath::Min(NativeTargetData.RandomOptions.Num(), PostOpItemData.RandomOptions.Num()); // 둘이 같아야 할 것.
	if (MinRandomOptionNum > 0)
	{
		CreatedRandomOptionTitle = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(RandomOptionTitleClass, this, (bIsForEnhanceMenu ? VB_OptionStats : VB_OptionStats_Upgrade).Get(), FMargin()));
		if (CreatedRandomOptionTitle)
		{
			CreatedRandomOptionTitle->SetDynText(0, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyItemDetail_RandOptionPanelTitle"))));
			
			if (!bIsForEnhanceMenu)
			{
				CreatedRandomOptionTitle->FClickOptionInfoBtn.BindUObject(this, &UB2LobbyUI_ItemEnhUpgCommon::OnClickedOptionInfo);
				CreatedRandomOptionTitle->SetVisibleOptionButton(true);
			}
		}

		for (int32 OI = 0; OI < MinRandomOptionNum; ++OI)
		{
			FItemOption& CurrOption = NativeTargetData.RandomOptions[OI];
			FItemOption& PostOption = PostOpItemData.RandomOptions[OI];

			if (!bIsForEnhanceMenu)
			{
				CreateRandomOptionPreviewStatText((bIsForEnhanceMenu ? VB_OptionStats : VB_OptionStats_Upgrade).Get(), CreatedOptionStatTexts, OI, false, CurrOption.OptionType, CurrOption.RawOptionAmount, IntToPCClass(NativeTargetData.AllowedPCClass));
			}
			else
			{
				CreateSingleItemOptionStatText((bIsForEnhanceMenu ? VB_OptionStats : VB_OptionStats_Upgrade).Get(), CreatedOptionStatTexts, OI, false, CurrOption.OptionType, CurrOption.RawOptionAmount, PostOption.RawOptionAmount, IntToPCClass(NativeTargetData.AllowedPCClass));
			}			
		}
	}

	if(!bIsForEnhanceMenu)
		CreateRandomOptionNew(PostOpItemData.ItemRefID, NativeTargetData.RandomOptions.Num(), !(MinRandomOptionNum > 0));
}

void UB2LobbyUI_ItemEnhUpgCommon::CreateRandomOptionNew(int32 itemRef, int32 CurrentCount, bool bCreateTitle)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	const FMD_ItemSingleElem* ItemMDElem = CliDataStore.GetItemMasterDetailInfo(itemRef);
	if (ItemMDElem)
	{
		if (CurrentCount < ItemMDElem->RandomOptCount)
		{
			if (bCreateTitle)
			{
				CreatedRandomOptionTitle = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(RandomOptionTitleClass, this, VB_OptionStats_Upgrade.Get(), FMargin()));
				if (CreatedRandomOptionTitle)
				{
					CreatedRandomOptionTitle->SetDynText(0, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyItemDetail_RandOptionPanelTitle"))));
					CreatedRandomOptionTitle->FClickOptionInfoBtn.BindUObject(this, &UB2LobbyUI_ItemEnhUpgCommon::OnClickedOptionInfo);
					CreatedRandomOptionTitle->SetVisibleOptionButton(true);
				}
			}

			CreatedRandomOptionNew = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(RandomOptionNewClass, this, VB_OptionStats_Upgrade.Get(), FMargin()));
			if (CreatedRandomOptionNew)
			{
				CreatedRandomOptionNew->SetDynText(0, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_UpgradeBonus"))));
			}
		}
	}
}

void UB2LobbyUI_ItemEnhUpgCommon::CreateSingleItemOptionStatText(class UVerticalBox* VBToCreateIn, TArray<class UB2DynText_Multiple*>& ArrayToPutCreatedText, int32 RowIndexInVB, bool bIsForIntrinsicOption, EItemOption InOptionType, float RawOptionValue_Current, float RawOptionValue_PostOp, EPCClass InDisplayPCClass)
{
	UB2DynText_Multiple* ThisCreated = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(bIsForIntrinsicOption ? IntrinsicOptionDisplayClass : RandomOptionDisplayClass, this, VBToCreateIn, FMargin()));
	if (ThisCreated)
	{
		ThisCreated->SetDynText(RowIndexInVB,
			GetLOCTextOfItemOption(InOptionType, InDisplayPCClass),
			GetItemOptionValueDisplayText(InOptionType, RawOptionValue_Current, true),
			GetItemOptionValueDisplayText(InOptionType, RawOptionValue_PostOp, true)
			);

		ArrayToPutCreatedText.Add(ThisCreated);
	}
}

void UB2LobbyUI_ItemEnhUpgCommon::CreateRandomOptionPreviewStatText(class UVerticalBox* VBToCreateIn, TArray<class UB2DynText_Multiple*>& ArrayToPutCreatedText, int32 RowIndexInVB, bool bIsForIntrinsicOption, EItemOption InOptionType, float RawOptionValue_Current, EPCClass InDisplayPCClass)
{
	UB2DynText_Multiple* ThisCreated = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(bIsForIntrinsicOption ? IntrinsicOptionDisplayClass : RandomOptionDisplayClass, this, VBToCreateIn, FMargin()));
	if (ThisCreated)
	{
		ThisCreated->SetDynText(RowIndexInVB,
			GetLOCTextOfItemOption(InOptionType, InDisplayPCClass),
			GetItemOptionValueDisplayText(InOptionType, RawOptionValue_Current, true),
			FText::FromString(TEXT("?"))
		);

		ArrayToPutCreatedText.Add(ThisCreated);
	}
}

void UB2LobbyUI_ItemEnhUpgCommon::CreateIntrinsicOptionNew(class UVerticalBox* VBToCreateIn, TArray<class UB2DynText_Multiple*>& ArrayToPutCreatedText, int32 RowIndexInVB, bool bIsForIntrinsicOption, EItemOption InOptionType, float RawOptionValue_Current, float RawOptionValue_PostOp, EPCClass InDisplayPCClass)
{
	UB2DynText_Multiple* ThisCreated = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(bIsForIntrinsicOption ? IntrinsicOptionDisplayClass : RandomOptionDisplayClass, this, VBToCreateIn, FMargin()));
	if (ThisCreated)
	{
		ThisCreated->SetDynText(RowIndexInVB,
			GetLOCTextOfItemOption(InOptionType, InDisplayPCClass),
			GetItemOptionValueDisplayText(InOptionType, RawOptionValue_PostOp, true),
			FText::FromString(FString("(New)")),
			FSlateColor(FLinearColor::Yellow)
		);

		ArrayToPutCreatedText.Add(ThisCreated);
	}
}

void UB2LobbyUI_ItemEnhUpgCommon::OnClickedOptionInfo()
{
	CreateLobbyItemAfterOptionPopup(GetOwningPlayer(), NativeTargetData);
}

void UB2LobbyUI_ItemEnhUpgCommon::CreateLobbyItemAfterOptionPopup(UObject* WorldContextObject, FB2Item& InItemData)
{
	// 보관함 아이템 혹은 장착 아이템
	const ELobbySubPopups LobbyPopupEnum = ELobbySubPopups::ELSPU_ItemAfterOpiton;

	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// 해당 user widget 생성 신호.
	// 생성된 Popup widget 을 UIP 처럼 배치해 넣을 패널 정보를 만들어준다.
	UB2LobbyUI_InventoryMain* InventoryUI = LobbyGM ? Cast<UB2LobbyUI_InventoryMain>(LobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
	UPanelWidget* PopupAddPanel = InventoryUI ? InventoryUI->GetItemDetailPopupParentPanel(false) : nullptr;

	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(LobbyPopupEnum, FDJLegacySubpopupAddPanelInfo(InventoryUI, PopupAddPanel));

	// 생성된 user widget 을 찾아서 ItemData 를 업데이트
	UB2LobbyUI_ItemAfterOption* CreatedDetailWindow = LobbyGM ? Cast<UB2LobbyUI_ItemAfterOption>(LobbyGM->DJLegacy_FindLobbySubPopup(LobbyPopupEnum)) : NULL;

	if (CreatedDetailWindow)
	{
		CreatedDetailWindow->UpdateItemData(InItemData, false);
	}
}

void UB2LobbyUI_ItemEnhUpgCommon::DestroyTargetItemIcon()
{
	if (CreatedTargetItemIcon){
		CreatedTargetItemIcon->DestroySelf();
	}
	CreatedTargetItemIcon = NULL;
}

void UB2LobbyUI_ItemEnhUpgCommon::DestroyOptionStatTexts()
{
	if (CreatedIntrinsicOptionTitle)
	{
		CreatedIntrinsicOptionTitle->DestroySelf();
		CreatedIntrinsicOptionTitle = NULL;
	}
	if (CreatedRandomOptionTitle)
	{
		CreatedRandomOptionTitle->DestroySelf();
		CreatedRandomOptionTitle = NULL;
	}
	if (CreatedRandomOptionNew)
	{
		CreatedRandomOptionNew->DestroySelf();
		CreatedRandomOptionNew = NULL;
	}

	for (UB2DynText_Multiple* CurrElem : CreatedOptionStatTexts)
	{
		if (CurrElem)
			CurrElem->DestroySelf();
	}
	CreatedOptionStatTexts.Empty();
}
