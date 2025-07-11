// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UITotemRaffle.h"
//#include "B2UIManager.h"
//#include "B2LobbyGameMode.h"
//
//#include "B2AndroidBackManager.h"
//#include "B2Inventory.h"
//#include "B2LobbyInventory.h"
//#include "B2DynText_Multiple.h"
//#include "B2CombatStatEvaluator.h"
//#include "BladeIIGameImpl.h"
//#include "B2UI_TotemSmeltingBonus.h"
//
//// UB2UITotemRaffle ///////////////////////////////////////////////////////////////
//
//void UB2UITotemRaffle::DestroySelf(UB2UIManager* InUIManager)
//{
//	Super::DestroySelf(InUIManager);
//
//	if (UID_TotemRaffleOp.IsValid())
//	{
//		UID_TotemRaffleOp->DestroySelf();
//	}
//}
//
//void UB2UITotemRaffle::Init()
//{
//	Super::Init();
//
//
//	//static void GetAllTotemList(TArray<FB2Totem>& OutResult);
//}
//
//void UB2UITotemRaffle::StartFromRaffle(const FB2Totem& InTotem)
//{
//	if (UID_TotemRaffleOp.IsValid())
//	{
//		UID_TotemRaffleOp->StartFromRaffle(InTotem);
//	}
//
//	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
//}
//
//void UB2UITotemRaffle::OnClickButton()
//{
//	CloseMe();
//}
//
//void UB2UITotemRaffle::OnClickTotem()
//{
//	// 메인 캐릭터 확인
//	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Totem);
//	//EnterCostumePageClass<ECostumePage>::GetInstance().Signal(ECostumePage::);
//	CloseMe();
//}
//
//void UB2UITotemRaffle::CacheAssets()
//{
//	Super::CacheAssets();
//
//	GET_SLOT(UB2UITotemRaffleOp, UID_TotemRaffleOp);
//	if (UID_TotemRaffleOp.IsValid())
//	{
//		UID_TotemRaffleOp->CacheAssets();
//	}
//}
//
//void UB2UITotemRaffle::BindDelegates()
//{
//	Super::BindDelegates();
//
//	if (UID_TotemRaffleOp.IsValid())
//		BIND_CLICK_FUNC_TO_BTN(UID_TotemRaffleOp->BTN_Receive, &UB2UITotemRaffle::OnClickButton);
//
//	if (UID_TotemRaffleOp.IsValid())
//		BIND_CLICK_FUNC_TO_BTN(UID_TotemRaffleOp->BTN_Confirm, &UB2UITotemRaffle::OnClickButton);
//
//	if (UID_TotemRaffleOp.IsValid())
//		BIND_CLICK_FUNC_TO_BTN(UID_TotemRaffleOp->BTN_Totem, &UB2UITotemRaffle::OnClickTotem);
//}
//
//void UB2UITotemRaffle::CloseWidgetDelegate()
//{
//	OnClickButton();
//}
//
//void UB2UITotemRaffle::CloseMe()
//{
//	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
//	// TotemRaffle 추가 필요
//	UB2UIManager::GetInstance()->CloseUI(UIFName::TotemRaffle);
//}
//
//// UB2UITotemRaffleOp ///////////////////////////////////////////////////////////////
//
//UB2UITotemRaffleOp::UB2UITotemRaffleOp(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	ItemStatIncColor = FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f));
//	ItemStatSameColor = FSlateColor(FLinearColor(10.0f, 10.0f, 10.0f));
//	ItemStatDecColor = FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f));
//
//}
//
//void UB2UITotemRaffleOp::DestroySelf()
//{
//	if (RaffleIcon.IsValid())
//	{
//		RaffleIcon->DestroySelf(UB2UIManager::GetInstance());
//	}
//
//	DestroyItemOptions();
//
//	Super::DestroySelf();
//}
//
//void UB2UITotemRaffleOp::CacheAssets()
//{
//	Super::CacheAssets();
//
//	GET_SLOT(UCanvasPanel, X_CP_ItemIconPanel);
//
//	GET_SLOT(UB2RichTextBlock, TB_Receive);
//	GET_SLOT(UTextBlock, TB_ResultType);
//	GET_SLOT(UB2RichTextBlock, TB_Totem);
//
//	GET_SLOT(UTextBlock, TB_Combat);
//	GET_SLOT(UTextBlock, TB_CombatPower);
//	GET_SLOT(UTextBlock, TB_CombatPowerDelta);
//	GET_SLOT(UTextBlock, TB_PrimaryPointType);
//	GET_SLOT(UTextBlock, TB_PrimaryPoint);
//	GET_SLOT(UTextBlock, TB_PrimaryPointDelta);
//	GET_SLOT(UTextBlock, TB_SmeltingSuccess);
//	GET_SLOT(UTextBlock, TB_SmeltingLevel);
//	GET_SLOT(UTextBlock, TB_ConfirmBtn);
//	
//	GET_SLOT(UVerticalBox, VB_OptionList);
//
//	GET_SLOT(UButton, BTN_Confirm);
//	GET_SLOT(UButton, BTN_Receive);
//	GET_SLOT(UButton, BTN_Totem);
//
//	GET_SLOT(UVerticalBox, VB_SmeltingBonus);
//	//GET_SLOT(UVerticalBox, VB_SpecialInfoDisplayPanel);
//
//	GET_SLOT(UB2LobbyUI_TotemInventorySlot, RaffleIcon);
//	if (RaffleIcon.IsValid())
//	{
//		RaffleIcon->Init();
//	}
//
//	Anim_ItemRaffle = GetAnimation(this, "Anim_ItemRaffle");
//
//	UpdateStaticText();
//}
//
//void UB2UITotemRaffleOp::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
//{
//	Super::StartFromLobby(InUIManager, InGM);
//
//}
//
//void UB2UITotemRaffleOp::StartFromRaffle(const FB2Totem& InTotem)
//{
//	if (Anim_ItemRaffle.IsValid())
//		PlayAnimation(Anim_ItemRaffle.Get());
//
//	NativeItemData_After = InTotem;
//	//UB2LobbyInventory::GetEquippedCostumeAtPlace(NativeItemData_Before, ItemInfo.CostumeEquipPlace, EPCClass(ItemInfo.AllowedPCClass));
//
//	//RaffleIcon->BindDoc(InTotem);
//	UpdateItemData(NativeItemData_Before, NativeItemData_After);
//}
//
//void UB2UITotemRaffleOp::UpdateItemData(const FB2Totem& InBeforTotem, const FB2Totem& InAfterTotem)
//{
//	UpdateSubWidgets(InBeforTotem, InAfterTotem); // 본격 배치한 widget 들 업데이트
//	UpdateItemOptions(InAfterTotem);
//	UpdateTotemSmeltingBonus(InAfterTotem);
//	UpdataItemSetEffect();
//}
//
//void UB2UITotemRaffleOp::UpdateSubWidgets(const FB2Totem& InBeforTotem, const FB2Totem& InAfterTotem)
//{
//	if (RaffleIcon.IsValid())
//		RaffleIcon->UpdateTotemSlot_FromFB2Totem(InAfterTotem);
//
//	if (TB_CombatPower.IsValid())
//		TB_CombatPower->SetText(FText::AsNumber(InAfterTotem.Power));
//
//	if (TB_PrimaryPoint.IsValid())
//		TB_PrimaryPoint->SetText(FText::AsNumber(InAfterTotem.MainOptionDetail.RawOptionAmount));
//
//	EItemPrimaryPointType PrimaryPointType = CombatStatEval::ConvertItemOptionToItemPrimaryPointType(InAfterTotem.MainOptionDetail.OptionType);
//
//	if (TB_PrimaryPointType.IsValid())
//		TB_PrimaryPointType->SetText(GetLOCTextOfPrimPointType(PrimaryPointType));
//
//	if (TB_SmeltingLevel.IsValid())
//	{
//		FText SmeltingLevelFormat = FText::FromString(TEXT("+{0}"));
//		TB_SmeltingLevel->SetText(FText::Format(SmeltingLevelFormat, InAfterTotem.RefineLevel));
//	}
//
//}
//
//void UB2UITotemRaffleOp::UpdateTotemSmeltingBonus(const FB2Totem& InTotem)
//{
//	if (VB_SmeltingBonus.IsValid() == false)
//	{
//		return;
//	}
//
//	VB_SmeltingBonus->ClearChildren();
//
//	UB2UI_TotemSmeltingBonus* pSmeltingBonus = Cast<UB2UI_TotemSmeltingBonus>(DynCreateInVerticalBox(UITotemSmeltingBonusClass, this, VB_SmeltingBonus.Get(), FMargin()));
//	if (pSmeltingBonus)
//	{
//		pSmeltingBonus->InitTotemSmeltingBonus(InTotem);
//	}
//
//}
//
//FText UB2UITotemRaffleOp::GetSubOptionTypeText(ETotemSubOptionType InSubOptionType)
//{
//	if (InSubOptionType == ETotemSubOptionType::OPTION_PLUS)
//	{
//		return FText::FromString(TEXT("+"));
//	}
//	else if (InSubOptionType == ETotemSubOptionType::OPTION_MINUS)
//	{
//		return  FText::FromString(TEXT("-"));
//	}
//	else
//	{
//		return  FText::FromString(TEXT(""));
//	}
//
//}
//
//void UB2UITotemRaffleOp::UpdateStaticText()
//{
//	if (TB_Receive.IsValid())
//		TB_Receive->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
//
//	if (TB_ResultType.IsValid())
//		TB_ResultType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MailLottery_QuickResult")));
//
//	if (TB_Totem.IsValid())
//		TB_Totem->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Totem")));
//
//	if (TB_Combat.IsValid())
//		TB_Combat->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));
//
//	if (TB_ConfirmBtn.IsValid())
//		TB_ConfirmBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
//}
//
//void UB2UITotemRaffleOp::UpdateItemOptions(const FB2Totem& InTotem)
//{
//	DestroyItemOptions();
//
//
// 	if (!VB_OptionList.IsValid())
//		return;
//
//
//	VB_OptionList->ClearChildren();
//
//
//	const int32 iArrayOptionMax = InTotem.SubOptionDetails.Num();
//
//	if (iArrayOptionMax <= 0)
//	{
//		return;
//	}
//
//	const TArray<FB2TotemSubOptionDetail>& ArrayItemOption = InTotem.SubOptionDetails;
//
//	// 제련 완료 시와 제련 미완료 시를 구분해야한다.
//	for (int32 i = 0; i < iArrayOptionMax; ++i)
//	{
//		FText OptionText = (IsItemOptionPerPCClass(ArrayItemOption[i].SubOption.OptionType) ?
//			BladeIIGameImpl::GetTotemDataStore().GetLOCTextOfSkillOption(ArrayItemOption[i].SubOption.OptionType) :
//			GetLOCTextOfItemOption(ArrayItemOption[i].SubOption.OptionType, EPCClass::EPC_Fighter));
//
//		if (InTotem.IsSmeltingComplete())
//		{
//			FText OptionValueFormat = FText::FromString(TEXT("{0}{1}"));
//			FText OptionValueText = FText::Format(OptionValueFormat,
//				SubOptionType.IsValidIndex(i) ? GetSubOptionTypeText(SubOptionType[i]) : FText::FromString(TEXT("")),
//				GetItemOptionValueDisplayText(InTotem.SubOptionDetails[i].SubOption.OptionType, InTotem.SubOptionDetails[i].SubOption.RawOptionAmount, true));
//
//			UB2DynText_Multiple* pTotemDetailOption = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(UITotemOptionClass, this, VB_OptionList.Get(), FMargin()));
//			if (pTotemDetailOption)
//			{
//				pTotemDetailOption->SetIndexIcon(i);
//				pTotemDetailOption->SetDynText(i, OptionText, FText::FromString(TEXT("")), OptionValueText);
//			}
//		}
//		else
//		{
//			FText OptionRangeFormat = FText::FromString(TEXT("[{0} ~ {1}{2}]"));
//			float MinOptionValue, MaxOptionValue;
//			BladeIIGameImpl::GetTotemDataStore().GetTotemSubOptionMinMax(InTotem.Grade, InTotem.SubOptionDetails[i].SubOption.OptionType, MinOptionValue, MaxOptionValue);
//			FText OptionRangeText = FText::Format(OptionRangeFormat,
//				MinOptionValue,
//				SubOptionType.IsValidIndex(i) ? GetSubOptionTypeText(SubOptionType[i]) : FText::FromString(TEXT("{0}")),
//				GetItemOptionValueDisplayText(InTotem.SubOptionDetails[i].SubOption.OptionType, MaxOptionValue, true));
//
//			FB2TotemInfo TotemInfo;
//			int32 OptionChance = 0;
//			if (BladeIIGameImpl::GetTotemDataStore().GetTotemInfo(InTotem.RefID, TotemInfo))
//			{
//				OptionChance = TotemInfo.RefineLimitCounts[i];
//			}
//			FText OptionChanceFormat = FText::FromString(TEXT("({0})"));
//			FText OptionChanceText = FText::Format(OptionChanceFormat, FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Smelt_ChanceCount")), OptionChance));
//
//			UB2DynText_Multiple* pTotemDetailOption = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(UITotemOptionClass, this, VB_OptionList.Get(), FMargin()));
//			if (pTotemDetailOption)
//			{
//				pTotemDetailOption->SetIndexIcon(i);
//				pTotemDetailOption->SetDynText(i, OptionText, OptionRangeText, OptionChanceText);
//			}
//		}
//
//		/*FText OptionRangeFormat = FText::FromString(TEXT("[{0} ~ {1}]"));
//		float MinOptionValue, MaxOptionValue;
//		BladeIIGameImpl::GetTotemDataStore().GetTotemSubOptionMinMax(InTotem.Grade, InTotem.SubOptionDetails[i].SubOption.OptionType, MinOptionValue, MaxOptionValue);
//		FText OptionRangeText = FText::Format(OptionRangeFormat,
//			GetItemOptionValueDisplayText(ArrayItemOption[i].SubOption.OptionType, MinOptionValue, true),
//			GetItemOptionValueDisplayText(ArrayItemOption[i].SubOption.OptionType, MaxOptionValue, true));
//
//		FB2TotemInfo TotemInfo;
//		int32 OptionChance = 0;
//		if (BladeIIGameImpl::GetTotemDataStore().GetTotemInfo(InTotem.RefID, TotemInfo))
//		{
//			OptionChance = TotemInfo.RefineLimitCounts[i];
//		}
//		FText OptionChanceFormat = FText::FromString(TEXT("({0})"));
//		FText OptionChanceText = FText::Format(OptionChanceFormat, FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Smelt_ChanceCount")), OptionChance));
//
//		UB2DynText_Multiple* pTotemDetailOption = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(UITotemOptionClass, this, VB_OptionList.Get(), FMargin()));
//		if (pTotemDetailOption)
//		{
//			if (i == 2)
//			{
//				pTotemDetailOption->SetIndexIcon(i);
//				pTotemDetailOption->SetDynText(i, OptionText, FText::FromString(TEXT("")), FText::FromString(TEXT("+20%")));
//			}
//			else
//			{
//				pTotemDetailOption->SetIndexIcon(i);
//				pTotemDetailOption->SetDynText(i, OptionText, OptionRangeText, OptionChanceText);
//			}
//		}*/
//	}
//
//}
//
//void UB2UITotemRaffleOp::DestroyItemOptions()
//{
//// 	if (CreatedIntrinsicOptionTitle)
//// 	{
//// 		CreatedIntrinsicOptionTitle->DestroySelf();
//// 		CreatedIntrinsicOptionTitle = NULL;
//// 	}
//// 	for (UB2DynText_ItemDetailOption* CurrElem : CreatedIntrinsicOptionDisplay)
//// 	{
//// 		if (CurrElem)
//// 			CurrElem->DestroySelf();
//// 	}
//// 	CreatedIntrinsicOptionDisplay.Empty();
//}
//
//void UB2UITotemRaffleOp::UpdataItemSetEffect()
//{
//// 	if (VB_SpecialInfoDisplayPanel.IsValid() == false)
//// 		return;
//// 
//// 	if (IsItemSetEffectAvailable(NativeItemData_After) == false)
//// 		return;
//// 
//// 	UB2UICostumeItemDetailSetEffect* pSetItemInfo = Cast<UB2UICostumeItemDetailSetEffect>(DynCreateInVerticalBox(UICostumeItemDetailSetEffectClass, this, VB_SpecialInfoDisplayPanel.Get(), FMargin()));
//// 	if (pSetItemInfo)
//// 	{
//// 		pSetItemInfo->InitCostumeItemDetailSetEffect(NativeItemData_After);
//// 	}
//}