// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEnhanceEther.h"
#include "B2UIEtherOptionSlot.h"
#include "B2EtherManager.h"
#include "B2UIEtherScrollSlot.h"
#include "B2Inventory.h"
#include "B2ItemInfo.h"
#include "B2UIManager.h"
#include "B2LobbyInventory.h"
#include "Retailer.h"
#include "B2UIDocHelper.h"
#include "B2ButtonGoodInfoToolTip.h"
#include "B2UIShortagePopupManager.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

//void UB2UIEnhanceEther::SetInfo(const FB2Ether* EnhanceEther, EPCClass InSelectPCClass)
//{
//	if (EnhanceEther == nullptr)
//	{
//		return;
//	}
//
//	IsTouch = true;
//	PCClass = InSelectPCClass;
//	EnhaceCost = -1;
//	EtherID = EnhanceEther->ID;
//
//	for (int i = 0; i < UIP_BaseEtherOptions.Num(); ++i)
//	{
//		if (UIP_BaseEtherOptions[i].IsValid())
//		{
//			UIP_BaseEtherOptions[i]->SetVisibility(ESlateVisibility::Hidden);
//		}
//		if (X_RTB_UPSubOptions[i].IsValid())
//		{
//			X_RTB_UPSubOptions[i]->SetVisibility(ESlateVisibility::Hidden);
//		}
//		if (X_IMG_UPBGs[i].IsValid())
//		{
//			X_IMG_UPBGs[i]->SetVisibility(ESlateVisibility::Hidden);
//		}
//	}
//
//	int32 SubOptionMax = EnhanceEther->SubOption.Num();
//
//	if (X_UIP_BaseMainOption.IsValid())
//	{
//		X_UIP_BaseMainOption->SetInfo(EnhanceEther->MainOption.OptionType, EnhanceEther->MainOption.RawOptionAmount, PCClass);
//	}
//
//	for (int32 i = 0; i < SubOptionMax; ++i)
//	{
//		if (UIP_BaseEtherOptions[i].IsValid())
//		{
//			UIP_BaseEtherOptions[i]->SetVisibility(ESlateVisibility::Visible);
//			UIP_BaseEtherOptions[i]->SetInfo(EnhanceEther->SubOption[i].OptionType, EnhanceEther->SubOption[i].RawOptionAmount, PCClass);
//		}
//	}
//
//	if (SubOptionState != 0 && UIP_BaseEtherOptions.IsValidIndex(SubOptionMax - 1))
//	{
//		int32 AnimIndex = GetChangeSubOptionIndex(EnhanceEther, SubOptionState);
//
//		if(UIP_BaseEtherOptions.IsValidIndex(AnimIndex))
//			UIP_BaseEtherOptions[AnimIndex]->PlaySpotLightAnimation();
//	}
//
//	if (X_TB_BaseTier.IsValid())
//	{
//		X_TB_BaseTier->SetText(B2EtherManager::GetInstance().GetEtherTierName(EnhanceEther->Tier));
//	}
//		
//	if (X_UIP_EtherScrollSlot.IsValid())
//	{
//		X_UIP_EtherScrollSlot->SetInfo(EnhanceEther);
//	}
//
//	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
//	if (ItemMiscInfo)
//	{
//		if (X_IMG_BaseTierBG.IsValid())
//		{
//			X_IMG_BaseTierBG->SetBrushFromMaterial(ItemMiscInfo->GetEtherTierNameBGMtrl(EnhanceEther->Tier));
//		}
//	}
//
//	int32 UPTier = EnhanceEther->Tier;
//	int32 UpLevel = EnhanceEther->EnhanceLevel + 1;
//	SubOptionState = B2EtherManager::GetInstance().GetEnhanceSubOptionState(UpLevel, EnhanceEther->Tier);
//	FText OptionName = GetLOCTextOfItemOption(EnhanceEther->MainOption.OptionType, PCClass);
//
//	if (UpLevel <= MAXLEVEL)
//	{
//		if (X_RTB_UpMainOptionName.IsValid())
//		{
//			X_RTB_UpMainOptionName->SetText(OptionName);
//		}
//
//		float UpMainValue = EnhanceEther->MainOption.RawOptionAmount + B2EtherManager::GetInstance().GetEnhanceIncMainValue(EnhanceEther->MainOptionID, EnhanceEther->Star);
//		if (X_RTB_UpMainOptionValue.IsValid())
//		{
//			X_RTB_UpMainOptionValue->SetText(GetItemOptionValueDisplayText(EnhanceEther->MainOption.OptionType, UpMainValue, true));
//		}
//
//		if (SubOptionState == 1) // 추가
//		{
//			if (X_O_UpOptionDesc.IsValid())
//			{
//				X_O_UpOptionDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
//			}
//
//			UPTier += 1;
//			if (X_RTB_UPSubOptions[EnhanceEther->SubOption.Num()].IsValid())
//			{
//				X_RTB_UPSubOptions[EnhanceEther->SubOption.Num()]->SetVisibility(ESlateVisibility::Visible);
//			}
//			if (X_IMG_UPBGs[EnhanceEther->SubOption.Num()].IsValid())
//			{
//				X_IMG_UPBGs[EnhanceEther->SubOption.Num()]->SetVisibility(ESlateVisibility::Visible);
//			}
//			if (X_TB_UpOptionDesc.IsValid())
//			{
//				X_TB_UpOptionDesc->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Option_Add"))));
//			}
//		}
//		else if (SubOptionState == 2) // 강화
//		{
//			if (X_O_UpOptionDesc.IsValid())
//			{
//				X_O_UpOptionDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
//			}
//
//			if (X_TB_UpOptionDesc.IsValid())
//			{
//				X_TB_UpOptionDesc->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Option_Enhancement"))));
//			}
//
//			for (int i = 0; i < EnhanceEther->SubOption.Num(); ++i)
//			{
//				if (X_RTB_UPSubOptions[i].IsValid())
//				{
//					X_RTB_UPSubOptions[i]->SetVisibility(ESlateVisibility::Visible);
//				}
//			}
//		}
//		else
//		{
//			if (X_O_UpOptionDesc.IsValid())
//			{
//				X_O_UpOptionDesc->SetVisibility(ESlateVisibility::Hidden);
//			}
//		}
//
//		if (X_TB_UpTier.IsValid())
//		{
//			X_TB_UpTier->SetText(B2EtherManager::GetInstance().GetEtherTierName(UPTier));
//		}
//
//		if (ItemMiscInfo)
//		{
//			if (X_IMG_UpTierBG.IsValid())
//			{
//				X_IMG_UpTierBG->SetBrushFromMaterial(ItemMiscInfo->GetEtherTierNameBGMtrl(UPTier));
//			}
//		}
//
//		if (X_BTN_Confirm.IsValid())
//		{
//			X_BTN_Confirm->SetVisibility(ESlateVisibility::Collapsed);
//		}
//
//		if (X_HB_Buttons.IsValid())
//		{
//			X_HB_Buttons->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//		}
//
//		if (X_HB_Cost.IsValid())
//		{
//			X_HB_Cost->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//		}
//
//		if (X_PW_MaxLevel.IsValid())
//		{
//			X_PW_MaxLevel->SetVisibility(ESlateVisibility::Collapsed);
//		}
//
//		if (X_PW_Tier.IsValid())
//		{
//			X_PW_Tier->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//		}
//		if (X_IMG_Line.IsValid())
//		{
//			X_IMG_Line->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//		}
//
//		if (X_RTB_EnhanceRate.IsValid())
//		{
//			X_RTB_EnhanceRate->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//			X_RTB_EnhanceRate->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_ETHER, TEXT("Ether_Enhancement_Percentage")), B2EtherManager::GetInstance().GetEnhanceRate(UpLevel, EnhanceEther->Star) * 100));
//		}
//	}
//	else
//	{
//		IsTouch = false;
//		if (X_RTB_UpMainOptionName.IsValid())
//		{
//			X_RTB_UpMainOptionName->SetVisibility(ESlateVisibility::Hidden);
//		}
//		if (X_RTB_UpMainOptionValue.IsValid())
//		{
//			X_RTB_UpMainOptionValue->SetVisibility(ESlateVisibility::Hidden);
//		}
//		if (X_O_UpOptionDesc.IsValid())
//		{
//			X_O_UpOptionDesc->SetVisibility(ESlateVisibility::Hidden);
//		}
//
//		if (X_TB_Cost.IsValid())
//		{
//			X_TB_Cost->SetVisibility(ESlateVisibility::Hidden);
//		}
//			
//		if (X_TB_LeveUPText.IsValid())
//		{
//			X_TB_LeveUPText->SetVisibility(ESlateVisibility::Hidden);
//		}
//
//		if (X_BTN_Confirm.IsValid())
//		{
//			X_BTN_Confirm->SetVisibility(ESlateVisibility::Visible);
//		}
//
//		if (X_HB_Buttons.IsValid())
//		{
//			X_HB_Buttons->SetVisibility(ESlateVisibility::Collapsed);
//		}
//
//		if (X_HB_Cost.IsValid())
//		{
//			X_HB_Cost->SetVisibility(ESlateVisibility::Collapsed);
//		}
//
//		if (X_PW_MaxLevel.IsValid())
//		{
//			X_PW_MaxLevel->SetVisibility(ESlateVisibility::HitTestInvisible);
//		}
//		if (X_PW_Tier.IsValid())
//		{
//			X_PW_Tier->SetVisibility(ESlateVisibility::Collapsed);
//		}
//		if (X_IMG_Line.IsValid())
//		{
//			X_IMG_Line->SetVisibility(ESlateVisibility::Collapsed);
//		}
//
//		if (X_RTB_EnhanceRate.IsValid())
//		{
//			X_RTB_EnhanceRate->SetVisibility(ESlateVisibility::Hidden);
//		}
//	}
//		
//	EnhaceCost = B2EtherManager::GetInstance().GetEnhanceCost(UpLevel, EnhanceEther->Star);
//	BeforeSubOptionValue.Empty();
//	BeforeSubOptionValue.SetNum(SubOptionMax);
//
//	for (int32 i = 0; i < SubOptionMax; ++i)
//	{
//		if (UIP_BaseEtherOptions.IsValidIndex(i) && UIP_BaseEtherOptions[i].IsValid()
//			&& EnhanceEther->SubOption.IsValidIndex(i) && BeforeSubOptionValue.IsValidIndex(i))
//		{
//			BeforeSubOptionValue[i] = EnhanceEther->SubOption[i].RawOptionAmount;
//		}
//	}
//
//	SetEtherCost();
//}

void UB2UIEnhanceEther::Init()
{
	IsAutoEnhance = false;
	EnhanceEtherInfo = nullptr;
	IsAnimantion = false;
	IsEnhanceSuccess = false;
	Super::Init();

	for (int i = 0; i < UIP_BaseEtherOptions.Num(); ++i)
	{
		if (UIP_BaseEtherOptions[i].IsValid())
		{
			UIP_BaseEtherOptions[i]->Init();
		}
	}

	if (X_UIP_BaseMainOption.IsValid())
	{
		X_UIP_BaseMainOption->Init();
	}

	if (X_UIP_EtherScrollSlot.IsValid())
	{
		X_UIP_EtherScrollSlot->Init();
	}
}

void UB2UIEnhanceEther::EndFailNotice_BP()
{
	IsAnimantion = false;
	SetEtherCost();
	if (IsAutoEnhance == true)
	{
		OnClickEnhance();
	}
}

void UB2UIEnhanceEther::CacheAssets()
{
	GET_SLOT(UB2Button, X_BTN_Close);
	GET_SLOT(UTextBlock, X_TB_TItle);
	GET_SLOT(UTextBlock, X_TB_BaseTier);
	GET_SLOT(UTextBlock, X_TB_UpTier);
	GET_SLOT(UTextBlock, X_TB_CostText);
	GET_SLOT(UTextBlock, X_TB_AutoEnhanceBTN);
	GET_SLOT(UTextBlock, X_TB_UpOptionDesc);
	GET_SLOT(UTextBlock, X_TB_Cost);
	GET_SLOT(UTextBlock, X_TB_EnhanceBTN);
	GET_SLOT(UOverlay, X_O_UpOptionDesc);
	GET_SLOT(UTextBlock, X_TB_LeveUPText);
	GET_SLOT(UImage, X_IMG_BaseTierBG);
	GET_SLOT(UImage, X_IMG_UpTierBG);
	GET_SLOT(UB2RichTextBlock, X_RTB_UpMainOptionName);
	GET_SLOT(UB2RichTextBlock, X_RTB_UpMainOptionValue);
	GET_SLOT(UB2UIEtherScrollSlot, X_UIP_EtherScrollSlot);
	GET_SLOT(UB2Button, X_BTN_Enhance);
	GET_SLOT(UB2Button, X_BTN_AutoEnhance);
	GET_SLOT(UB2Button, X_BTN_Confirm);
	GET_SLOT(UHorizontalBox, X_HB_Buttons);
	GET_SLOT(UHorizontalBox, X_HB_Cost);
	GET_SLOT(UTextBlock, X_TB_MaxDesc);
	GET_SLOT(UPanelWidget, X_PW_MaxLevel);
	GET_SLOT(UPanelWidget, X_PW_Tier);
	GET_SLOT(UImage, X_IMG_Line);
	GET_SLOT(UTextBlock, X_TB_Confirm);
	GET_SLOT(UB2RichTextBlock, X_RTB_EnhanceRate);
	GET_SLOT(UWidgetAnimation, ANIM_Enhance_Success);
	GET_SLOT(UWidgetAnimation, ANIM_Bonus_01);
	GET_SLOT(UWidgetAnimation, ANIM_Bonus_02);
	GET_SLOT(UWidgetAnimation, Anim_Collect_light);
	GET_SLOT(UB2UIEtherOptionSlot, X_UIP_BaseMainOption);

	UIP_BaseEtherOptions.SetNum(MAXSUBOPTION);
	GET_SLOT_BYNAME(UB2UIEtherOptionSlot, UIP_BaseEtherOptions[0], X_UIP_BaseSubOption1);
	GET_SLOT_BYNAME(UB2UIEtherOptionSlot, UIP_BaseEtherOptions[1], X_UIP_BaseSubOption2);
	GET_SLOT_BYNAME(UB2UIEtherOptionSlot, UIP_BaseEtherOptions[2], X_UIP_BaseSubOption3);

	X_RTB_UPSubOptions.SetNum(MAXSUBOPTION);
	GET_SLOT_BYNAME(UB2RichTextBlock, X_RTB_UPSubOptions[0], X_RTB_UPSubOption1);
	GET_SLOT_BYNAME(UB2RichTextBlock, X_RTB_UPSubOptions[1], X_RTB_UPSubOption2);
	GET_SLOT_BYNAME(UB2RichTextBlock, X_RTB_UPSubOptions[2], X_RTB_UPSubOption3);

	X_IMG_UPBGs.SetNum(MAXSUBOPTION);
	GET_SLOT_BYNAME(UImage, X_IMG_UPBGs[0], X_IMG_UPBG1);
	GET_SLOT_BYNAME(UImage, X_IMG_UPBGs[1], X_IMG_UPBG2);
	GET_SLOT_BYNAME(UImage, X_IMG_UPBGs[2], X_IMG_UPBG3);
}

void UB2UIEnhanceEther::UpdateStaticText()
{
	if (X_TB_AutoEnhanceBTN.IsValid())
	{
		X_TB_AutoEnhanceBTN->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AutoEnhancement"))));
	}

	if (X_TB_CostText.IsValid())
	{
		X_TB_CostText->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("RelicUIP_EnhancementBP_002"))));
	}

	if (X_TB_EnhanceBTN.IsValid())
	{
		X_TB_EnhanceBTN->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Enhance"))));
	}

	if (X_TB_MaxDesc.IsValid())
	{
		X_TB_MaxDesc->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Enhancement_Max"))));
	}

	if (X_TB_Confirm.IsValid())
	{
		X_TB_Confirm->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("DefeatMenu_Confirm"))));
	}

	if (X_TB_LeveUPText.IsValid())
	{
		X_TB_LeveUPText->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Enhancement_Success"))));
	}

	if (X_TB_TItle.IsValid())
	{
		X_TB_TItle->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Enhancement_Title")))); //에테르 강화
	}
}

void UB2UIEnhanceEther::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Close, &UB2UIEnhanceEther::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Confirm, &UB2UIEnhanceEther::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Enhance, &UB2UIEnhanceEther::OnClickEnhance);
	BIND_CLICK_FUNC_TO_BTN(X_BTN_AutoEnhance, &UB2UIEnhanceEther::OnClickAutoEnhance);

	//if (Anim_Collect_light.IsValid())
	//{
	//	Anim_Collect_light->OnAnimationFinished.AddDynamic(this, &UB2UIEnhanceEther::CollectLightAnimationFinished);
	//}

	//if (ANIM_Enhance_Success.IsValid())
	//{
	//	ANIM_Enhance_Success->OnAnimationFinished.AddDynamic(this, &UB2UIEnhanceEther::EnhanceSuccessAnimationFinished);
	//}

	//EnhanceEtherEventID = DeliveryEnhanceEtherClass<FB2EnhanceEtherPtr>::GetInstance().Subscribe([this](const FB2EnhanceEtherPtr EtherPtr)
	//{
	//	ResponseEnhanceEther(EtherPtr);
	//});

	//DeliveryEnhanceEther2630Class<FB2EnhanceEtherPtr>::GetInstance().Subscribe([this](const FB2EnhanceEtherPtr EtherPtr)
	//{
	//	UB2UIManager::GetInstance()->GetShortagePopupMGR()->PopupGotoEtherPieceMessage();

	//	if (IsAutoEnhance == true)
	//	{
	//		SetIsAutoEnhance(false);
	//	}
	//});
}

void UB2UIEnhanceEther::DestroySelf(class UB2UIManager* InUIManager)
{
	//Super::DestroySelf(InUIManager);

	//DeliveryEnhanceEtherClass<FB2EnhanceEtherPtr>::GetInstance().Unsubscribe(EnhanceEtherEventID);
	//DeliveryEnhanceEther2630Class<FB2EnhanceEtherPtr>::GetInstance().UnsubscribeAll();

	//for (int IDX = 0; IDX < UIP_BaseEtherOptions.Num(); ++IDX)
	//{
	//	if (UIP_BaseEtherOptions[IDX].IsValid())
	//	{
	//		UIP_BaseEtherOptions[IDX]->DestroySelf(InUIManager);
	//	}
	//}

	//if (X_UIP_BaseMainOption.IsValid())
	//{
	//	X_UIP_BaseMainOption->DestroySelf(InUIManager);
	//}
	// 
	//if (X_UIP_EtherScrollSlot.IsValid())
	//{
	//	X_UIP_EtherScrollSlot->DestroySelf(InUIManager);
	//}

	//if (Anim_Collect_light.IsValid())
	//{
	//	//Anim_Collect_light->OnAnimationFinished.RemoveDynamic(this, &UB2UIEnhanceEther::CollectLightAnimationFinished);
	//}
}

void UB2UIEnhanceEther::SetEtherCost()
{
	if (X_TB_Cost.IsValid())
	{
		int32 ConsumableAmount = UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ETHER_PIECE);

		if (EnhaceCost > ConsumableAmount)
		{
			X_TB_Cost->SetColorAndOpacity(FLinearColor::Red);
		}

		X_TB_Cost->SetText(FText::AsNumber(EnhaceCost));
	}
}

void UB2UIEnhanceEther::CloseWidgetDelegate()
{
	if (IsTouch == true && IsAnimantion == true && IsAutoEnhance == true)
	{
		SetIsAutoEnhance(false);

		UB2UIManager::GetInstance()->OpenMsgPopup(
			EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_NoticeTab")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoEnhanceCancel")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
		);
	}
	else
	{
		OnClickClose();
	}
}

void UB2UIEnhanceEther::OnClickClose()
{
	if (IsAnimantion == false)
	{
		UB2UIManager::GetInstance()->CloseUI(UIFName::EnhanceEther);
	}
}

void UB2UIEnhanceEther::OnClickEnhance()
{
	if (IsTouch == true && IsAnimantion == false)
	{
		data_trader::Retailer::GetInstance().RequestEnhanceAether(EtherID);
	}
}

void UB2UIEnhanceEther::OnClickAutoEnhance()
{
	if (IsTouch == true && IsAutoEnhance == false && IsAnimantion == false)
	{
		SetIsAutoEnhance(true);

		OnClickEnhance();
	}
	else if(IsTouch == true && IsAutoEnhance == true)
	{
		SetIsAutoEnhance(false);
		
		UB2UIManager::GetInstance()->OpenMsgPopup(
			EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_NoticeTab")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoEnhanceCancel")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
		);
	}
}

void UB2UIEnhanceEther::CollectLightAnimationFinished()
{
	if (IsEnhanceSuccess == true) // 강화 성공
	{
		if (ANIM_Bonus_01.IsValid())
		{
			PlayAnimation(ANIM_Bonus_01.Get());
		}

		if (ANIM_Bonus_02.IsValid())
		{
			PlayAnimation(ANIM_Bonus_02.Get());
		}

		if (ANIM_Enhance_Success.IsValid())
		{
			PlayAnimation(ANIM_Enhance_Success.Get());
		}
	}
	else
	{
		PlayFailAnim_BP();
	}
}

void UB2UIEnhanceEther::EnhanceSuccessAnimationFinished()
{
	//if (X_O_UpOptionDesc.IsValid())
	//{
	//	X_O_UpOptionDesc->SetVisibility(ESlateVisibility::Hidden);
	//}

	//IsAnimantion = false;
	//SetEtherCost();
	//UpdateInfo();

	//if (IsAutoEnhance == true)
	//{
	//	OnClickEnhance();
	//}

	//UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(PCClass, true);
}

void UB2UIEnhanceEther::UpdateInfo()
{
	//if (EnhanceEtherInfo != nullptr)
	//{
	//	const FB2Ether EnhanceEther(EnhanceEtherInfo);
	//	SetInfo(&EnhanceEther, PCClass);
	//}
	//EnhanceEtherInfo = nullptr;
}

void UB2UIEnhanceEther::SetIsAutoEnhance(bool Value)
{
	IsAutoEnhance = Value;
	if (IsAutoEnhance == true)
	{
		if (X_TB_AutoEnhanceBTN.IsValid())
		{
			X_TB_AutoEnhanceBTN->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AutoEnhancementCancel"))));
		}
	}
	else
	{
		if (X_TB_AutoEnhanceBTN.IsValid())
		{
			X_TB_AutoEnhanceBTN->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_AutoEnhancement"))));
		}
	}
}

void UB2UIEnhanceEther::ResponseEnhanceEther(const FB2EnhanceEtherPtr EnhanceEtherPtr)
{
	BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountDecrease(FItemRefIDHelper::ITEM_REF_ID_ETHER_PIECE, -1,
		EnhanceEtherPtr->changed_aether_piece_items,
		EnhanceEtherPtr->deleted_aether_piece_ids
	);

	if (auto* DocUser = UB2UIDocHelper::GetDocUser())
	{
		DocUser->SetEtherPiece(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_ETHER_PIECE));
	}

	if (EnhanceEtherPtr != nullptr)
	{
		IsEnhanceSuccess = EnhanceEtherPtr->is_success;
		//결과 에테르 갱신
		EnhanceEtherInfo = EnhanceEtherPtr->aether;
	}
	
	if (Anim_Collect_light.IsValid())
	{
		IsAnimantion = true;
		PlayAnimation(Anim_Collect_light.Get());
	}
}

//int UB2UIEnhanceEther::GetChangeSubOptionIndex(const FB2Ether* EnhanceEther, int32 InSubOptionState) const
//{
//	//if (EnhanceEther)
//	//{
//	//	if (InSubOptionState == 1) // 추가
//	//	{
//	//		return EnhanceEther->SubOption.Num() - 1;
//	//	}
//	//	else if (InSubOptionState == 2) // 강화
//	//	{
//	//		for (int i = 0; i < EnhanceEther->SubOption.Num(); ++i)
//	//		{
//	//			if (BeforeSubOptionValue.IsValidIndex(i) && EnhanceEther->SubOption.IsValidIndex(i))
//	//			{
//	//				if (!FMath::IsNearlyEqual(BeforeSubOptionValue[i], EnhanceEther->SubOption[i].RawOptionAmount))
//	//					return i;
//	//			}
//	//		}
//	//	}
//	//}
//	return -1;
//}