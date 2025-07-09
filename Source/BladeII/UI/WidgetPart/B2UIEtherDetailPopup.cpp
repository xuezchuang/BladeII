// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEtherDetailPopup.h"
#include "B2EtherManager.h"
#include "B2UIManager.h"
#include "B2UIEther.h"
#include "B2UIEtherScrollSlot.h"
#include "B2UIEtherOptionSlot.h"
#include "B2Inventory.h"
#include "Retailer.h"
#include "B2UIEnhanceEther.h"
#include "B2ItemInfo.h"
#include "B2UIDecompositionPopup.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2AndroidBackManager.h"
#include "B2EtherManager.h"
#include "B2GameDefine.h"


void UB2UIEtherDetailPopup::SetPopupInfo(int64 ID, EPCClass SelectPCClass, DetailPopupType InType)
{
	if (IsOpen() == false)
	{
		EtherID = ID;
		SetPCClass(SelectPCClass);
		PopupType = InType;
		this->SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);

		const FB2Ether* Ether = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetEther(EtherID);

		CompareIsLock = IsLock = Ether->IsLock;
		SetLockImage(IsLock);

		switch (PopupType)
		{
		case DetailPopupType::Character:
			if (X_TB_EquipOrTakeOffBtn.IsValid())
			{
				X_TB_EquipOrTakeOffBtn->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("CollectBook_UnEquip"))));
			}

			if (X_BTN_EquipORTakeOff.IsValid())
			{
				BIND_CLICK_FUNC_TO_BTN(X_BTN_EquipORTakeOff, &UB2UIEtherDetailPopup::OnClickTakeOff)
			}

			if (X_O_Sell.IsValid())
			{
				X_O_Sell->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (X_O_Decomposition.IsValid())
			{
				X_O_Decomposition->SetVisibility(ESlateVisibility::Collapsed);
			}
			
			if (X_UIP_EtherSlot.IsValid())
			{
				X_UIP_EtherSlot->UpdateEquipUI(true);
			}

			break;
		case DetailPopupType::Scroll:
			if (X_TB_EquipOrTakeOffBtn.IsValid())
			{
				X_TB_EquipOrTakeOffBtn->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SkillInfo_UsingSkill"))));
			}

			if (X_BTN_EquipORTakeOff.IsValid())
			{
				BIND_CLICK_FUNC_TO_BTN(X_BTN_EquipORTakeOff, &UB2UIEtherDetailPopup::OnClickEquip)
			}

			if (X_UIP_EtherSlot.IsValid())
			{
				X_UIP_EtherSlot->UpdateEquipUI(false);
			}

			break;
		}

		for (int i = 0; i < UIP_EtherOptions.Num(); ++i)
		{
			if (UIP_EtherOptions[i].IsValid())
			{
				UIP_EtherOptions[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		if (Ether)
		{
			EtherPart = Ether->Part;

			if (X_UIP_EtherSlot.IsValid())
			{
				X_UIP_EtherSlot->SetInfo(Ether);
			}

			if (X_RTB_MainOptionName.IsValid())
			{
				X_RTB_MainOptionName->SetText(GetLOCTextOfItemOption(Ether->MainOption.OptionType, SelectPCClass));
			}

			if (X_RTB_MainOptionValue.IsValid())
			{
				X_RTB_MainOptionValue->SetText(GetItemOptionValueDisplayText(Ether->MainOption.OptionType, Ether->MainOption.RawOptionAmount, true));
			}

			if (X_TB_EtherTierName.IsValid())
			{
				X_TB_EtherTierName->SetText(B2EtherManager::GetInstance().GetEtherTierName(Ether->Tier));
			}

			for (int i = 0; i < Ether->SubOption.Num(); ++i)
			{
				if (UIP_EtherOptions[i].IsValid())
				{
					UIP_EtherOptions[i]->SetVisibility(ESlateVisibility::Visible);
					UIP_EtherOptions[i]->SetInfo(Ether->SubOption[i].OptionType, Ether->SubOption[i].RawOptionAmount, SelectPCClass);
				}
			}
			UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
			if (ItemMiscInfo)
			{
				if (X_ING_TierBG.IsValid())
				{
					X_ING_TierBG->SetBrushFromMaterial(ItemMiscInfo->GetEtherTierNameBGMtrl(Ether->Tier));
				}
			}

			if (Ether->SetType == EtherSetType::DEFENCE)
			{
				if (X_TB_SetName.IsValid())
				{
					X_TB_SetName->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetName_2"))));
				}
			}
			else if (Ether->SetType == EtherSetType::OFFENCE) // 공격 세트
			{
				if (X_TB_SetName.IsValid())
				{
					X_TB_SetName->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetName_1"))));
				}
			}

			b2network::B2mdAetherSetOptionInfoPtr SetOptionPtr = B2EtherManager::GetInstance().GetEtherSetOptionInfoPtrByType(Ether->Type, Ether->Tier);
			if (SetOptionPtr != nullptr)
			{
				if (X_TB_SetInfo.IsValid())
				{
					X_TB_SetInfo->SetText(B2EtherManager::GetInstance().GetEtherSetOptionText(SetOptionPtr->set_option_id));
				}
			}

			if (Ether->EnhanceLevel >= MAXLEVEL)
			{
				if (X_O_Enhance.IsValid())
				{
					X_O_Enhance->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			else
			{
				if (X_O_Enhance.IsValid())
				{
					X_O_Enhance->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}

			b2network::B2mdAetherSetOptionInfoPtr OptionInfoPtr = B2EtherManager::GetInstance().GetEtherSetOptionInfoPtrByType(Ether->Type, Ether->Tier);

			UIP_EtherSetOptionInfos[0].SetText("Ether_SetOption_Detail1", "General_Stat_Percent", OptionInfoPtr->trigger_rate * 0.01);
			UIP_EtherSetOptionInfos[1].SetText("Ether_SetOption_Detail2", "General_Sconds", OptionInfoPtr->cooldown_second);
		}
	}
}

void UB2UIEtherDetailPopup::SetPCClass(EPCClass SelectPCClass)
{
	PCClass = SelectPCClass;
}

bool UB2UIEtherDetailPopup::IsOpen()
{
	bool bOpen = false;
	if (this->GetVisibility() == ESlateVisibility::Visible)
	{
		bOpen = true;
	}
	return bOpen;
}

void UB2UIEtherDetailPopup::CacheAssets()
{
	GET_SLOT(UB2Button, X_BTN_Close);
	
	GET_SLOT(UOverlay, X_O_Sell);
	GET_SLOT(UB2Button, X_BTN_Sell);
	GET_SLOT(UTextBlock, X_TB_Sell);
	if (X_TB_Sell.IsValid())
	{
		X_TB_Sell->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Sell"))));
	}

	GET_SLOT(UOverlay, X_O_Decomposition);
	GET_SLOT(UB2Button, X_BTN_Decomposition);
	GET_SLOT(UTextBlock, X_TB_Decomposition);
	if (X_TB_Decomposition.IsValid())
	{
		X_TB_Decomposition->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Decomposition"))));
	}

	GET_SLOT(UOverlay, X_O_Enhance);
	GET_SLOT(UB2Button, X_BTN_Enhance);
	GET_SLOT(UTextBlock, X_TB_Enhance);
	if (X_TB_Enhance.IsValid())
	{
		X_TB_Enhance->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Enhancement"))));
	}

	GET_SLOT(UB2Button, X_BTN_EquipORTakeOff);
	GET_SLOT(UTextBlock, X_TB_EquipOrTakeOffBtn);

	GET_SLOT(UB2UIEtherScrollSlot, X_UIP_EtherSlot);
	if (X_UIP_EtherSlot.IsValid())
	{
		X_UIP_EtherSlot->Init();
	}
	
	UIP_EtherOptions.SetNum(3);
	GET_SLOT_BYNAME(UB2UIEtherOptionSlot, UIP_EtherOptions[0], X_UIP_SubOption1);
	GET_SLOT_BYNAME(UB2UIEtherOptionSlot, UIP_EtherOptions[1], X_UIP_SubOption2);
	GET_SLOT_BYNAME(UB2UIEtherOptionSlot, UIP_EtherOptions[2], X_UIP_SubOption3);

	for (int i = 0; i < UIP_EtherOptions.Num(); ++i)
	{
		if (UIP_EtherOptions[i].IsValid())
		{
			UIP_EtherOptions[i]->Init();
		}
	}

	UIP_EtherSetOptionInfos.SetNum(2);
	GET_SLOT_BYNAME(UTextBlock, UIP_EtherSetOptionInfos[0].TB_Name, X_TB_SetOptionName_1);
	GET_SLOT_BYNAME(UTextBlock, UIP_EtherSetOptionInfos[0].TB_Value, X_TB_SetOptionValue_1);
	GET_SLOT_BYNAME(UPanelWidget, UIP_EtherSetOptionInfos[0].PW_Slot, X_PW_Slot1);

	GET_SLOT_BYNAME(UTextBlock, UIP_EtherSetOptionInfos[1].TB_Name, X_TB_SetOptionName_2);
	GET_SLOT_BYNAME(UTextBlock, UIP_EtherSetOptionInfos[1].TB_Value, X_TB_SetOptionValue_2);
	GET_SLOT_BYNAME(UPanelWidget, UIP_EtherSetOptionInfos[1].PW_Slot, X_PW_Slot2);

	GET_SLOT(UB2RichTextBlock, X_RTB_MainOptionName);
	GET_SLOT(UB2RichTextBlock, X_RTB_MainOptionValue);
	
	GET_SLOT(UImage, X_ING_TierBG);
	GET_SLOT(UTextBlock, X_TB_EtherTierName);
	GET_SLOT(UImage, X_IMG_Lock);
	GET_SLOT(UImage, X_IMG_Unlock);
	GET_SLOT(UB2Button, X_BTN_Lock);
	GET_SLOT(UTextBlock, X_TB_SetName);
	GET_SLOT(UTextBlock, X_TB_SetInfo);
}

void UB2UIEtherDetailPopup::BindDelegates()
{
	if (X_BTN_Close.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Close, &UB2UIEtherDetailPopup::OnClickClose)
	}

	if (X_BTN_Sell.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Sell, &UB2UIEtherDetailPopup::OnClickSell)
	}

	if (X_BTN_Decomposition.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Decomposition, &UB2UIEtherDetailPopup::OnClickDecomposition)
	}

	if (X_BTN_Enhance.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Enhance, &UB2UIEtherDetailPopup::OnClickEnhance)
	}

	if (X_BTN_Lock.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Lock, &UB2UIEtherDetailPopup::OnClickLock)
	}
}

void UB2UIEtherDetailPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	X_UIP_EtherSlot->DestroySelf(InUIManager);

	for (int i = 0; i < UIP_EtherOptions.Num(); ++i)
	{
		UIP_EtherOptions[i]->DestroySelf(InUIManager);
	}
	UIP_EtherOptions.Empty();

	UIP_EtherSetOptionInfos.Empty();
}

void UB2UIEtherDetailPopup::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIEtherDetailPopup::OnClickClose()
{
	if (CompareIsLock != IsLock && PopupType == DetailPopupType::Scroll)
	{
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().SortStoredEthers();
		RefreshScrollUIClass<>::GetInstance().Signal();
	}

	this->SetVisibility(ESlateVisibility::Collapsed);
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	if (DecompositionEtherEventID != 0)
	{
		DeliveryDecompositionEtherClass<FB2DecompositionEtherPtr>::GetInstance().Unsubscribe(DecompositionEtherEventID);
	}
}

void UB2UIEtherDetailPopup::Init()
{
	Super::Init();

	DecompositionEtherEventID = 0;
	LockEtherEventID = 0;
	UnlockEtherEventID = 0;

	PopupType = DetailPopupType::Max;
}

void UB2UIEtherDetailPopup::OnClickSell()
{
	if (IsLock == false)
	{
		const FB2Ether* Ether = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetEther(EtherID);
		if (Ether)
		{
			SellEtherItemClass<int64, int32>::GetInstance().Signal(EtherID, B2EtherManager::GetInstance().GetEtherSellPrice(Ether->Star, Ether->EnhanceLevel));
		}
	}
	else
	{
		AskForUnlockItemPopupCommon(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_UnlockAskingItemAct_Sell")));
	}
}

void UB2UIEtherDetailPopup::OnClickDecomposition()
{
	if (IsLock == false)
	{
		const FB2Ether* Ether = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetEther(EtherID);
		UB2UIDecompositionPopup* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIDecompositionPopup>(UIFName::DecompositionPopup);
		if (Popup && Ether)
		{
			Popup->SetEtherInfo(B2EtherManager::GetInstance().GetEtherPiece(Ether->Star, Ether->EnhanceLevel),
				FOnClickPositive::CreateLambda([this]() {
				this->Decomposition();
			}));
		}
	}
	else
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(
			EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_NoticeTab")),
			BladeIIGetLOCText(B2LOC_CAT_ETHER, TEXT("Ether_Decomposition_Lock")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([this]() {
			
			this->UnlockEtherEventID = DeliveryUnlockEtherClass<FB2UnlockEtherPtr>::GetInstance().Subscribe([this](const FB2UnlockEtherPtr EtherPtr)
			{
				UnLockEther(EtherPtr);
			});

			TArray<int64> EtherIDs;
			EtherIDs.Add(EtherID);
			data_trader::Retailer::GetInstance().RequestUnlockAethers(EtherIDs);
			EtherIDs.Empty();
		})
		);
	}
}

void UB2UIEtherDetailPopup::OnClickEnhance()
{
	UB2UIEnhanceEther* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIEnhanceEther>(UIFName::EnhanceEther);
	Popup->SetInfo(BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetEther(EtherID), PCClass);
	OnClickClose();
}

void UB2UIEtherDetailPopup::OnClickEquip()
{
	TArray<int64> EtherIDs;
	EtherIDs.Add(EtherID);
	data_trader::Retailer::GetInstance().RequestMountAethers(EtherIDs, CliToSvrPCClassType(PCClass));
	EtherIDs.Empty();
}

void UB2UIEtherDetailPopup::OnClickTakeOff()
{
	TArray<int64> EtherIDs;
	EtherIDs.Add(EtherID);
	data_trader::Retailer::GetInstance().RequestUnmountAethers(EtherIDs, CliToSvrPCClassType(PCClass));
	EtherIDs.Empty();
}

void UB2UIEtherDetailPopup::OnClickLock()
{
	TArray<int64> EtherIDs;
	EtherIDs.Add(EtherID);
	if (IsLock == true)
	{
		UnlockEtherEventID = DeliveryUnlockEtherClass<FB2UnlockEtherPtr>::GetInstance().Subscribe([this](const FB2UnlockEtherPtr EtherPtr)
		{
			UnLockEther(EtherPtr);
		});

		data_trader::Retailer::GetInstance().RequestUnlockAethers(EtherIDs);
	}
	else
	{
		LockEtherEventID = DeliveryLockEtherClass<FB2LockEtherPtr>::GetInstance().Subscribe([this](const FB2LockEtherPtr EtherPtr)
		{
			if (EtherPtr->locked_aethers.Num() > 0)
			{
				BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().SetLock(EtherPtr->locked_aethers[0]->id, EtherPtr->locked_aethers[0]->locked);
			}
			if (X_UIP_EtherSlot.IsValid())
			{
				X_UIP_EtherSlot->UpdateLock(true);
			}
			this->SetLockImage(true);
		});

		data_trader::Retailer::GetInstance().RequestLockAethers(EtherIDs);
	}
	EtherIDs.Empty();
}

void UB2UIEtherDetailPopup::SetLockImage(bool InIsLock)
{
	IsLock = InIsLock;
	if (IsLock == true)
	{
		X_IMG_Lock->SetVisibility(ESlateVisibility::HitTestInvisible);
		X_IMG_Unlock->SetVisibility(ESlateVisibility::Collapsed);

		if (LockEtherEventID != 0)
		{
			DeliveryLockEtherClass<FB2LockEtherPtr>::GetInstance().Unsubscribe(LockEtherEventID);
			LockEtherEventID = 0;
		}
	}
	else
	{
		X_IMG_Lock->SetVisibility(ESlateVisibility::Collapsed);
		X_IMG_Unlock->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (UnlockEtherEventID != 0)
		{
			DeliveryUnlockEtherClass<FB2UnlockEtherPtr>::GetInstance().Unsubscribe(UnlockEtherEventID);
			UnlockEtherEventID = 0;
		}
	}
}

void UB2UIEtherDetailPopup::Decomposition()
{
	this->DecompositionEtherEventID = DeliveryDecompositionEtherClass<FB2DecompositionEtherPtr>::GetInstance().Subscribe([this](const FB2DecompositionEtherPtr EtherPtr)
	{
		this->OnClickClose();
	}
	);
	TArray<int64> EtherIDs;
	EtherIDs.Add(EtherID);
	data_trader::Retailer::GetInstance().RequestDismantleAethers(EtherIDs);
	EtherIDs.Empty();
}

void UB2UIEtherDetailPopup::UnLockEther(const FB2UnlockEtherPtr EtherPtr)
{
	if (EtherPtr->unlocked_aethers.Num() > 0)
	{
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().SetLock(EtherPtr->unlocked_aethers[0]->id, EtherPtr->unlocked_aethers[0]->locked);
	}
	if (X_UIP_EtherSlot.IsValid())
	{
		X_UIP_EtherSlot->UpdateLock(false);
	}
	this->SetLockImage(false);
}

void UB2UIEtherDetailPopup::AskForUnlockItemPopupCommon(const FText& InAskingItemActText)
{
	UB2UIManager::GetInstance()->OpenMsgPopup(
		EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInven_AskForUnlockForSomeItemAct")), InAskingItemActText),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]() {

		this->UnlockEtherEventID = DeliveryUnlockEtherClass<FB2UnlockEtherPtr>::GetInstance().Subscribe([this](const FB2UnlockEtherPtr EtherPtr)
		{
			UnLockEther(EtherPtr);
		});

		TArray<int64> EtherIDs;
		EtherIDs.Add(EtherID);
		data_trader::Retailer::GetInstance().RequestUnlockAethers(EtherIDs);
		EtherIDs.Empty();
	})
	);
}


void FSetInfo::SetText(FString InNameKey, FString InOptionKey, float InValue)
{
	if (InNameKey != TEXT(""))
	{
		if (TB_Name.IsValid() && TB_Name.IsValid())
		{
			TB_Name->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), InNameKey));

			if (InValue > 0)
			{
				TB_Value->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, InOptionKey), FText::AsNumber(InValue)));
			}
			else
			{
				TB_Value->SetText(FText::FromString(TEXT("-")));
			}
		}
	}
	else
	{
		if (PW_Slot.IsValid())
		{
			PW_Slot->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
