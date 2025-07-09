// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIManager.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUI_ItemDetailSetEffect.h"
#include "B2UICostumeItemDetailSetEffect.h"
#include "B2DynText_ItemDetailOption.h"
#include "B2DynText_Multiple.h"
#include "B2UICostumeRaffle.h"
#include "B2AndroidBackManager.h"

// UB2UICostumeRaffle ///////////////////////////////////////////////////////////////

void UB2UICostumeRaffle::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (ResultPage.IsValid())
	{
		ResultPage->DestroySelf();
	}
}

void UB2UICostumeRaffle::StartFromRaffle(const FB2Item& ItemInfo)
{
	ChadedCharacterClass = EPCClass(ItemInfo.AllowedPCClass);

	if (ResultPage.IsValid())
	{
		ResultPage->StartFromRaffle(ItemInfo);
	}

	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
}

void UB2UICostumeRaffle::OnClickButton()
{
	CloseMe();
}

void UB2UICostumeRaffle::OnClickCostume()
{
	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Signal(ChadedCharacterClass);
	EnterCostumePageClass<ECostumePage>::GetInstance().Signal(ECostumePage::Inven);
	CloseMe();
}

void UB2UICostumeRaffle::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UICostumeRaffleOp, ResultPage);
	if (ResultPage.IsValid())
	{
		ResultPage->CacheAssets();
	}
}

void UB2UICostumeRaffle::BindDelegates()
{
	Super::BindDelegates();

	if (ResultPage.IsValid())
		BIND_CLICK_FUNC_TO_BTN(ResultPage->BTN_Receive, &UB2UICostumeRaffle::OnClickButton);

	if (ResultPage.IsValid())
		BIND_CLICK_FUNC_TO_BTN(ResultPage->BTN_Confirm, &UB2UICostumeRaffle::OnClickButton);

	if (ResultPage.IsValid())
		BIND_CLICK_FUNC_TO_BTN(ResultPage->BTN_Costume, &UB2UICostumeRaffle::OnClickCostume);
}

void UB2UICostumeRaffle::CloseWidgetDelegate()
{
	OnClickButton();
}

void UB2UICostumeRaffle::CloseMe()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	UB2UIManager::GetInstance()->CloseUI(UIFName::CostumeRaffle);
}

// UB2UICostumeRaffleOp ///////////////////////////////////////////////////////////////

UB2UICostumeRaffleOp::UB2UICostumeRaffleOp(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemStatIncColor = FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f));
	ItemStatSameColor = FSlateColor(FLinearColor(10.0f, 10.0f, 10.0f));
	ItemStatDecColor = FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f));

	CreatedIntrinsicOptionTitle = NULL;
	IntrinsicOptionDisplayClass = NULL;
}

void UB2UICostumeRaffleOp::DestroySelf()
{
	if (RaffleIcon.IsValid())
	{
		RaffleIcon->DestroySelf(UB2UIManager::GetInstance());
	}

	DestroyItemOptions();

	Super::DestroySelf();
}

void UB2UICostumeRaffleOp::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, X_CP_ItemIconPanel);

	GET_SLOT(UB2RichTextBlock, TB_Receive);
	GET_SLOT(UTextBlock, TB_ResultType);
	GET_SLOT(UB2RichTextBlock, TB_Costume);

	GET_SLOT(UTextBlock, TB_Combat);
	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UTextBlock, TB_CombatPowerDelta);
	GET_SLOT(UTextBlock, TB_PrimaryPointType);
	GET_SLOT(UTextBlock, TB_PrimaryPoint);
	GET_SLOT(UTextBlock, TB_PrimaryPointDelta);
	GET_SLOT(UTextBlock, TB_ConfirmBtn);

	GET_SLOT(UButton, BTN_Confirm);
	GET_SLOT(UButton, BTN_Receive);
	GET_SLOT(UButton, BTN_Costume);
	
	GET_SLOT(UVerticalBox, VB_OptionsDisplayPanel);
	GET_SLOT(UVerticalBox, VB_SpecialInfoDisplayPanel);

	GET_SLOT(UB2UISelectiveLotteryCostumeSlot, RaffleIcon);
	if (RaffleIcon.IsValid())
	{
		RaffleIcon->Init();
		RaffleIcon->SetEnableReadingGlasses(false);
	}

	Anim_ItemRaffle = GetAnimation(this, "Anim_ItemRaffle");

	UpdateStaticText();
}

void UB2UICostumeRaffleOp::StartFromRaffle(const FB2Item& ItemInfo)
{
	if (Anim_ItemRaffle.IsValid())
		PlayAnimation(Anim_ItemRaffle.Get());

	NativeItemData_After = ItemInfo;
	UB2LobbyInventory::GetEquippedCostumeAtPlace(NativeItemData_Before, ItemInfo.CostumeEquipPlace, EPCClass(ItemInfo.AllowedPCClass));

	RaffleIcon->BindDoc(ItemInfo);
	UpdateItemData(NativeItemData_Before, NativeItemData_After);
}

void UB2UICostumeRaffleOp::UpdateItemData(const FB2Item& InBeforeItem, const FB2Item& InAfterItem)
{
	UpdateSubWidgets(InBeforeItem, InAfterItem); // 본격 배치한 widget 들 업데이트
	UpdateItemOptions();
	UpdataItemSetEffect();
}

void UB2UICostumeRaffleOp::UpdateSubWidgets(const FB2Item& InBeforeItem, const FB2Item& InAfterItem)
{
	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)InAfterItem.Power)));

	if (TB_PrimaryPointType.IsValid())
		TB_PrimaryPointType->SetText(GetLOCTextOfPrimPointType(InAfterItem.PrimaryPointType));

	if (TB_PrimaryPoint.IsValid())
		TB_PrimaryPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)InAfterItem.GetPrimaryPoint())));

	int32 CombatDelta = InAfterItem.Power - InBeforeItem.Power;
	int32 PrimaryPointDelta = (int32)InAfterItem.GetPrimaryPoint() - (int32)InBeforeItem.GetPrimaryPoint();
	int32 EnhanceLevelDelta = InAfterItem.EnhanceLevel - InBeforeItem.EnhanceLevel;

	FString CombatDeltaString = (CombatDelta == 0) ? TEXT("( - )") : FString::Printf(TEXT("( %s%d )"), (CombatDelta > 0) ? TEXT("+") : TEXT("-"), FMath::Abs(CombatDelta));
	FString PrimaryDeltaString = (PrimaryPointDelta == 0) ? TEXT("( - )") : FString::Printf(TEXT("( %s%d )"), (PrimaryPointDelta > 0) ? TEXT("+") : TEXT("-"), FMath::Abs(PrimaryPointDelta));

	if (TB_CombatPowerDelta.IsValid())
	{
		TB_CombatPowerDelta->SetText(FText::FromString(CombatDeltaString));
		TB_CombatPowerDelta->SetColorAndOpacity((CombatDelta > 0) ? ItemStatIncColor : (CombatDelta == 0 ? ItemStatSameColor : ItemStatDecColor));
	}

	if (TB_PrimaryPointDelta.IsValid())
	{
		TB_PrimaryPointDelta->SetText(FText::FromString(PrimaryDeltaString));
		TB_PrimaryPointDelta->SetColorAndOpacity((PrimaryPointDelta > 0) ? ItemStatIncColor : (PrimaryPointDelta == 0 ? ItemStatSameColor : ItemStatDecColor));
	}
}

void UB2UICostumeRaffleOp::UpdateStaticText()
{
	if (TB_Receive.IsValid())
		TB_Receive->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));

	if (TB_ResultType.IsValid())
		TB_ResultType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MailLottery_QuickResult")));

	if (TB_Costume.IsValid())
		TB_Costume->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_Costume")));

	if (TB_Combat.IsValid())
		TB_Combat->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));

	if (TB_ConfirmBtn.IsValid())
		TB_ConfirmBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
}

void UB2UICostumeRaffleOp::UpdateItemOptions()
{
	DestroyItemOptions();

	if (!VB_OptionsDisplayPanel.IsValid())
		return;

	if (NativeItemData_After.IntrinsicOptions.Num() > 0)
	{
		// Padding 은 일단 주지 않는다.
		CreatedIntrinsicOptionTitle = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(IntrinsicOptionTitleClass, this, VB_OptionsDisplayPanel.Get(), FMargin()));
		if (CreatedIntrinsicOptionTitle)
		{
			CreatedIntrinsicOptionTitle->SetDynText(0, // 걍 첫줄만 있음.
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyItemDetail_IntrinsicOptionPanelTitle")))
			);
		}

		for (int32 AI = 0; AI < NativeItemData_After.IntrinsicOptions.Num(); ++AI)
		{
			FItemOption& CurrOption = NativeItemData_After.IntrinsicOptions[AI];
			FItemOption SameOptionOfBefore; // 강화 이전의 동일 옵션 수치
			bool bFoundSameOptionOfBefore = false; // 물론 true 여야겠지? 아마도?

												   // bFoundSameOptionOfBefore 가 true 이면 SameOptionOfEquipped 가 valid 한 것.
			bFoundSameOptionOfBefore = GetItemIntrinsicOptionOfType(NativeItemData_Before, SameOptionOfBefore, CurrOption.OptionType);

			UB2DynText_ItemDetailOption* CurrCreated = Cast<UB2DynText_ItemDetailOption>(DynCreateInVerticalBox(IntrinsicOptionDisplayClass, this, VB_OptionsDisplayPanel.Get(), FMargin()));
			if (CurrCreated)
			{ // SameOptionOfBefore 는 찾은 경우 아니면 NULL 로. AllowedPCClass 는 일부 캐릭터 별로 다르게 들어가는 옵션 표시를 위함인데, 공용이라면 그런 옵션이 없어야겠지..?
				CurrCreated->UpdateOptionTextSet(false, AI, ItemAllowedPCClassEnum(NativeItemData_After.AllowedPCClass), &CurrOption, bFoundSameOptionOfBefore ? &SameOptionOfBefore : NULL, ItemStatIncColor, ItemStatSameColor, ItemStatDecColor, true);
				CreatedIntrinsicOptionDisplay.Add(CurrCreated);
			}
		}
	}
}

void UB2UICostumeRaffleOp::DestroyItemOptions()
{
	if (CreatedIntrinsicOptionTitle)
	{
		CreatedIntrinsicOptionTitle->DestroySelf();
		CreatedIntrinsicOptionTitle = NULL;
	}
	for (UB2DynText_ItemDetailOption* CurrElem : CreatedIntrinsicOptionDisplay)
	{
		if (CurrElem)
			CurrElem->DestroySelf();
	}
	CreatedIntrinsicOptionDisplay.Empty();
}

void UB2UICostumeRaffleOp::UpdataItemSetEffect()
{
	if (VB_SpecialInfoDisplayPanel.IsValid() == false)
		return;

	if (IsItemSetEffectAvailable(NativeItemData_After) == false)
		return;

	UB2UICostumeItemDetailSetEffect* pSetItemInfo = Cast<UB2UICostumeItemDetailSetEffect>(DynCreateInVerticalBox(UICostumeItemDetailSetEffectClass, this, VB_SpecialInfoDisplayPanel.Get(), FMargin()));
	if (pSetItemInfo)
	{
		pSetItemInfo->InitCostumeItemDetailSetEffect(NativeItemData_After);
	}
}