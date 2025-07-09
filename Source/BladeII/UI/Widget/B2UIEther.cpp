// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEther.h"
#include "B2UIEtherDetailPopup.h"
#include "B2EtherManager.h"
#include "B2UIManager.h"
#include "B2LobbyUI_SellItemPopup.h"
#include "B2UIEnhanceEther.h"
#include "B2UIEtherFiltering.h"
#include "B2UIEtherScroll.h"
#include "B2UIEtherCharacterSlot.h"
#include "B2UIEtherOption.h"
#include "Retailer.h"
#include "B2EtherInfo.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2AndroidBackManager.h"
#include "B2CodeTable.h"
#include "B2UIPEtherSetToolTip.h"
#include "B2UIDocHelper.h"
#include "EventSubsystem.h"


void UB2UIEther::GetScrollDetailPopup(const int64 EtherID)
{
	if (X_UIP_ScrollDetailPopup.IsValid())
	{
		X_UIP_ScrollDetailPopup->SetPopupInfo(EtherID, CurrentTabType, DetailPopupType::Scroll);
	}
}

void UB2UIEther::GetEquipDetailPopup(const int64 EtherID)
{
	if (X_UIP_EquipDetailPopup.IsValid())
	{
		X_UIP_EquipDetailPopup->SetPopupInfo(EtherID, CurrentTabType, DetailPopupType::Character);
	}
}

void UB2UIEther::RefreshEtherUI(bool IsEqiup)
{
	BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().ResetStoredEthers();
	RefreshCharacterEtherUI(IsEqiup);
	RefreshScrollEtherUI();
	if (X_UIP_EtherFiltering.IsValid())
	{
		X_UIP_EtherFiltering->SetSlotInfo(false);
	}
}

void UB2UIEther::SetEtherScroll(const int32 SetName)
{
	if (CurrentSetName != SetName)
	{
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().AllEtherNewFlagOff();
		SetCurrentSetName(SetName);
	}
	X_UIP_EtherFiltering->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIEther::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_ETHER, TEXT("Ether_Title")));
}

void UB2UIEther::Init()
{
	CachedScrollOffset = 0.0f;

	Super::Init();

	SubscribeEvents();

	if (BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetDirty())
	{
		data_trader::Retailer::GetInstance().RequestGetAccountAether(b2network::B2AetherType::ALL);
	}
	else
	{
		InitEtherUIInfo();
	}
}

void UB2UIEther::CacheAssets()
{
	GET_SLOT(UB2Button, X_BTN_AutoTakeOff);
	
	GET_SLOT(UB2Button, X_BTN_AllSell);
	GET_SLOT(UTextBlock, X_TB_AllSell);
	if (X_TB_AllSell.IsValid())
	{
		X_TB_AllSell->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_SellAll"))));
	}

	GET_SLOT(UB2Button, X_BTN_AutoDecomposition);
	GET_SLOT(UTextBlock, X_TB_AutoDecomposition);
	if (X_TB_AutoDecomposition.IsValid())
	{
		X_TB_AutoDecomposition->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Decomposition_Auto"))));
	}
	
	GET_SLOT(UTextBlock, X_TB_AutoTakeOff);
	if (X_TB_AutoTakeOff.IsValid())
	{
		X_TB_AutoTakeOff->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_UnmountAll"))));
	}
	
	GET_SLOT(UB2UIEtherDetailPopup, X_UIP_ScrollDetailPopup);
	if (X_UIP_ScrollDetailPopup.IsValid())
	{
		X_UIP_ScrollDetailPopup->Init();
		X_UIP_ScrollDetailPopup->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	GET_SLOT(UB2UIEtherDetailPopup, X_UIP_EquipDetailPopup);
	if (X_UIP_EquipDetailPopup.IsValid())
	{
		X_UIP_EquipDetailPopup->Init();
		X_UIP_EquipDetailPopup->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	CharacterTabs.Add(EPCClass::EPC_Gladiator);
	GET_SLOT_BYNAME(UButton, CharacterTabs[EPCClass::EPC_Gladiator].BTN_Tab, X_BTN_SelectGladiator);
	GET_SLOT_BYNAME(UImage, CharacterTabs[EPCClass::EPC_Gladiator].IMG_OnImage, X_IMG_On_Gladiator);
	GET_SLOT_BYNAME(UImage, CharacterTabs[EPCClass::EPC_Gladiator].IMG_OffImage, X_IMG_Off_Gladiator);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CharacterTabs[EPCClass::EPC_Gladiator].NewIcon, X_IMG_MarkForNew_Gladiator);

	CharacterTabs.Add(EPCClass::EPC_Assassin);
	GET_SLOT_BYNAME(UButton, CharacterTabs[EPCClass::EPC_Assassin].BTN_Tab, X_BTN_SelectAssassin);
	GET_SLOT_BYNAME(UImage, CharacterTabs[EPCClass::EPC_Assassin].IMG_OnImage, X_IMG_On_Assassin);
	GET_SLOT_BYNAME(UImage, CharacterTabs[EPCClass::EPC_Assassin].IMG_OffImage, X_IMG_Off_Assassin);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CharacterTabs[EPCClass::EPC_Assassin].NewIcon, X_IMG_MarkForNew_Assassin);

	CharacterTabs.Add(EPCClass::EPC_Wizard);
	GET_SLOT_BYNAME(UButton, CharacterTabs[EPCClass::EPC_Wizard].BTN_Tab, X_BTN_SelectWizard);
	GET_SLOT_BYNAME(UImage, CharacterTabs[EPCClass::EPC_Wizard].IMG_OnImage, X_IMG_On_Wizard);
	GET_SLOT_BYNAME(UImage, CharacterTabs[EPCClass::EPC_Wizard].IMG_OffImage, X_IMG_Off_Wizard);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CharacterTabs[EPCClass::EPC_Wizard].NewIcon, X_IMG_MarkForNew_Wizard);

	CharacterTabs.Add(EPCClass::EPC_Fighter);
	GET_SLOT_BYNAME(UButton, CharacterTabs[EPCClass::EPC_Fighter].BTN_Tab, X_BTN_SelectFighter);
	GET_SLOT_BYNAME(UImage, CharacterTabs[EPCClass::EPC_Fighter].IMG_OnImage, X_IMG_On_Fighter);
	GET_SLOT_BYNAME(UImage, CharacterTabs[EPCClass::EPC_Fighter].IMG_OffImage, X_IMG_Off_Fighter);
	GET_SLOT_BYNAME(UB2UIWidgetBase, CharacterTabs[EPCClass::EPC_Fighter].NewIcon, X_IMG_MarkForNew_Fighter);

	GET_SLOT(UB2Button, X_BTN_EtherFiltering);
	GET_SLOT(UTextBlock, X_TB_EtherFiltering);

	GET_SLOT(UB2LobbyUI_SellItemPopup, X_UIP_EtherSellPopup);
	if (X_UIP_EtherSellPopup.IsValid())
	{
		X_UIP_EtherSellPopup->SetVisibility(ESlateVisibility::Collapsed);
	}

	GET_SLOT(UB2UIEtherFiltering, X_UIP_EtherFiltering);
	if (X_UIP_EtherFiltering.IsValid())
	{
		X_UIP_EtherFiltering->Init();
		X_UIP_EtherFiltering->SetReceiver(this);
		X_UIP_EtherFiltering->SetSlotInfo(true);
	}

	GET_SLOT(UB2UIEtherScroll, X_UIP_EtherScroll);
	if (X_UIP_EtherScroll.IsValid())
	{
		X_UIP_EtherScroll->Init();
	}

	GET_SLOT(UB2UIEtherCharacterSlot, X_UIP_EtherCharacterSlot);
	if (X_UIP_EtherCharacterSlot.IsValid())
	{
		X_UIP_EtherCharacterSlot->Init();
	}

	GET_SLOT(UTextBlock, X_TB_CurrentCount);
	GET_SLOT(UTextBlock, X_TB_MaxCount);

	GET_SLOT(UB2UIEtherOption, X_UIP_EtherOption);
	if (X_UIP_EtherOption.IsValid())
	{
		X_UIP_EtherOption->Init();
	}

	GET_SLOT(UTextBlock, X_TB_EmptyText);
	if (X_TB_EmptyText.IsValid())
	{
		X_TB_EmptyText->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Ether_InvenDefaultDesc"))));
	}
	GET_SLOT(UB2Button, X_BTN_Extend);
	GET_SLOT(UImage, X_IMG_Plus);

	GET_SLOT(UB2UIPEtherSetToolTip, UIP_EtherSetToolTip);
	if (UIP_EtherSetToolTip.IsValid())
	{
		UIP_EtherSetToolTip->Init();
		UIP_EtherSetToolTip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIEther::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	UnsubscribeEvents();

	X_UIP_EtherScroll->DestroySelf(InUIManager);
	X_UIP_EtherCharacterSlot->DestroySelf(InUIManager);
	X_UIP_EtherOption->DestroySelf(InUIManager);

	if (X_UIP_EtherSellPopup.IsValid())
	{
		if (X_UIP_EtherSellPopup->GetVisibility() == ESlateVisibility::Visible)
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(X_UIP_EtherSellPopup.Get());
		}
		X_UIP_EtherSellPopup->DestroySelf();
	}

	if (X_UIP_EquipDetailPopup.IsValid())
	{
		if (X_UIP_EquipDetailPopup->GetVisibility() == ESlateVisibility::Visible)
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(X_UIP_EquipDetailPopup.Get());
		}
		X_UIP_EquipDetailPopup->DestroySelf(InUIManager);
	}

	if (X_UIP_EtherFiltering.IsValid())
	{
		if (X_UIP_EtherFiltering->GetVisibility() == ESlateVisibility::Visible)
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(X_UIP_EtherFiltering.Get());
		}
		X_UIP_EtherFiltering->DestroySelf(InUIManager);
	}

	if (X_UIP_ScrollDetailPopup.IsValid())
	{
		if (X_UIP_ScrollDetailPopup->GetVisibility() == ESlateVisibility::Visible)
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(X_UIP_ScrollDetailPopup.Get());
		}
		X_UIP_ScrollDetailPopup->DestroySelf(InUIManager);
	}

	if (UIP_EtherSetToolTip.IsValid())
	{
		UIP_EtherSetToolTip->DestroySelf(InUIManager);
	}


	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	if (UIMgr)
	{
		UIMgr->CloseAllStandaloneUIs();
	}
}

void UB2UIEther::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(CharacterTabs[EPCClass::EPC_Gladiator].BTN_Tab, &UB2UIEther::OnClickGladiator);
	BIND_CLICK_FUNC_TO_BTN(CharacterTabs[EPCClass::EPC_Assassin].BTN_Tab, &UB2UIEther::OnClickAssassin);
	BIND_CLICK_FUNC_TO_BTN(CharacterTabs[EPCClass::EPC_Wizard].BTN_Tab, &UB2UIEther::OnClickWizard);
	BIND_CLICK_FUNC_TO_BTN(CharacterTabs[EPCClass::EPC_Fighter].BTN_Tab, &UB2UIEther::OnClickFighter);

	if (X_BTN_Extend.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_Extend, &UB2UIEther::OnClickExtend);
	}

	if (X_BTN_AutoTakeOff.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_AutoTakeOff, &UB2UIEther::OnClickAutoTakeOff);
	}

	if (X_BTN_AllSell.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_AllSell, &UB2UIEther::OnClickAutoAllSell);
	}

	if (X_BTN_AutoDecomposition.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_AutoDecomposition, &UB2UIEther::OnClickAutoDecomposition);
	}

	if (X_BTN_EtherFiltering.IsValid())
	{
		BIND_CLICK_FUNC_TO_BTN(X_BTN_EtherFiltering, &UB2UIEther::OnClickEtherFiltering);
	}
}

bool UB2UIEther::OnBackButtonProc()
{
	if (IsSendModifyAetherNewFlag == true)
	{
		data_trader::Retailer::GetInstance().RequestModifyAetherNewFlag(0);
	}
	
	return IsSendModifyAetherNewFlag;
}

void UB2UIEther::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryMountEtherClass<FB2MountEtherPtr>::GetInstance().Subscribe2(
		[this](const FB2MountEtherPtr EtherPtr)
	{
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().EquipEther(EtherPtr, this->CurrentTabType);

		UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(this->CurrentTabType, true);

		this->RefreshEtherUI(true);
		if (this->X_UIP_ScrollDetailPopup.IsValid())
		{
			this->X_UIP_ScrollDetailPopup->OnClickClose();
		}
	}
	));

	Issues.Add(DeliveryUnmountEtherClass<FB2UnmountEtherPtr>::GetInstance().Subscribe2(
		[this](const FB2UnmountEtherPtr EtherPtr)
	{
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().TakeOffEther(EtherPtr, this->CurrentTabType);

		this->RefreshEtherUI(false);
		this->PlayUnmountEtherSound_BP();

		UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(this->CurrentTabType, true);
		if (this->X_UIP_EquipDetailPopup.IsValid())
		{
			this->X_UIP_EquipDetailPopup->OnClickClose();
		}
	}
	));

	Issues.Add(DeliverySellEtherClass<FB2SellEtherPtr>::GetInstance().Subscribe2(
		[this](const FB2SellEtherPtr EtherPtr)
	{
		this->ResponseSellEther(EtherPtr);
		this->RefreshScrollEtherUI();
	}
	));

	Issues.Add(DeliveryDecompositionEtherClass<FB2DecompositionEtherPtr>::GetInstance().Subscribe2(
		[this](const FB2DecompositionEtherPtr EtherPtr)
	{
		this->ResponseDecompositionEther(EtherPtr);
	}
	));

	Issues.Add(DeliveryEnhanceEtherClass<FB2EnhanceEtherPtr>::GetInstance().Subscribe2(
		[this](const FB2EnhanceEtherPtr EtherPtr)
	{
		this->EnhanceEther(EtherPtr);
	}
	));

	Issues.Add(DeliveryUnlockEtherClass<FB2UnlockEtherPtr>::GetInstance().Subscribe2(
		[this](const FB2UnlockEtherPtr EtherPtr)
	{
		if (EtherPtr->unlocked_aethers.Num() > 0 && this->X_UIP_EtherScroll.IsValid())
		{
			for (const b2network::B2AetherServerInfoPtr& Elme : EtherPtr->unlocked_aethers)
			{
				this->X_UIP_EtherScroll->UpdateLock(Elme->id, Elme->locked);
			}
		}
	}
	));

	Issues.Add(DeliveryLockEtherClass<FB2LockEtherPtr>::GetInstance().Subscribe2(
		[this](const FB2LockEtherPtr EtherPtr)
	{
		if (EtherPtr->locked_aethers.Num() > 0 && this->X_UIP_EtherScroll.IsValid())
		{
			for (const b2network::B2AetherServerInfoPtr& Elme : EtherPtr->locked_aethers)
			{
				this->X_UIP_EtherScroll->UpdateLock(Elme->id, Elme->locked);
			}
		}
	}
	));

	Issues.Add(DeliveryExtendEtherInventoryClass<FB2ExtendEtherInventoryPtr>::GetInstance().Subscribe2(
		[this](const FB2ExtendEtherInventoryPtr EtherPtr)
	{
		this->SetInvenCount(EtherPtr->current_slot_count);
	}
	));

	Issues.Add(RefreshScrollUIClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->RefreshScrollEtherUI();
	}
	));

	Issues.Add(OpenEtherSetToolTipClass<int32, EPCClass>::GetInstance().Subscribe2(
		[this](int32 InSetID, EPCClass InEPCClass)
	{
		if (this->UIP_EtherSetToolTip.IsValid())
		{
			this->UIP_EtherSetToolTip->SetInfo(InSetID, InEPCClass);
		}
	}
	));
	
	Issues.Add(DeliveryGetAccountEtherClass<FB2GetAccountEtherPtr>::GetInstance().Subscribe2(
		[this](const FB2GetAccountEtherPtr EtherPtr)
	{
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().SetAccountEther(EtherPtr);
		this->InitEtherUIInfo();
	}
	));

	Issues.Add(DeliveryModifyEtherNewFlagClass<FB2ModifyEtherNewFlagPtr>::GetInstance().Subscribe2(
		[this](const FB2ModifyEtherNewFlagPtr EtherPtr)
	{
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().AllEtherNewFlagOff();
		this->IsSendModifyAetherNewFlag = false;
		if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
		{
			UIManager->ChangeUISceneBack();
		}
	}
	));

	Issues.Add(SellEtherItemClass<int64, int32>::GetInstance().Subscribe2(
		[this](const int64 EtherID, const int32 EtherPrice)
	{
		this->SellEtherItem(EtherID, EtherPrice);
	}
	));

	Issues.Add(ResultAllSellEtherItemClass<int32>::GetInstance().Subscribe2(
		[this](const int32 EtherPrice)
	{
		this->ResultAllSellEtherItem(EtherPrice);
	}
	));
}

void UB2UIEther::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UIEther::SetCharacterInfo(EPCClass SelectTabType)
{
	CurrentTabType = SelectTabType;
	for (TPair<EPCClass, FCharacterTab>& Tab : CharacterTabs)
	{
		if (Tab.Key == SelectTabType)
		{
			Tab.Value.SetTabVisible(true);
		}
		else
		{
			Tab.Value.SetTabVisible(false);
		}
	}
	
	RefreshCharacterEtherUI(false);

	if (X_UIP_ScrollDetailPopup.IsValid() && X_UIP_ScrollDetailPopup->IsOpen())
	{
		X_UIP_ScrollDetailPopup->SetPCClass(SelectTabType);
	}

	if (X_UIP_EquipDetailPopup.IsValid() && X_UIP_EquipDetailPopup->IsOpen())
	{
		X_UIP_EquipDetailPopup->SetPCClass(SelectTabType);
	}
}

void UB2UIEther::OnClickGladiator()
{
	if (CurrentTabType != EPCClass::EPC_Gladiator)
	{
		SetCharacterInfo(EPCClass::EPC_Gladiator);
	}
}

void UB2UIEther::OnClickAssassin()
{
	if (CurrentTabType != EPCClass::EPC_Assassin)
	{
		SetCharacterInfo(EPCClass::EPC_Assassin);
	}
}

void UB2UIEther::OnClickWizard()
{
	if (CurrentTabType != EPCClass::EPC_Wizard)
	{
		SetCharacterInfo(EPCClass::EPC_Wizard);
	}
}

void UB2UIEther::OnClickFighter()
{
	if (CurrentTabType != EPCClass::EPC_Fighter)
	{
		SetCharacterInfo(EPCClass::EPC_Fighter);
	}
}

void UB2UIEther::OnClickAutoTakeOff()
{
	TArray<int64> EtherIDs;

	TMap<int64, FB2Ether>& Ethers = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetEquippedEthers(CurrentTabType);
	for (TPair<int64, FB2Ether>& Elem : Ethers)
	{
		EtherIDs.Add(Elem.Value.ID);
	}

	if (EtherIDs.Num() > 0)
	{
		//colosseum comment : characterType 파라미터 추가
		data_trader::Retailer::GetInstance().RequestUnmountAethers(EtherIDs, CliToSvrPCClassType(CurrentTabType));
	}
	else
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(
			EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_CONFIGURATION, TEXT("Configuration_NoticeTab")),
			BladeIIGetLOCText(B2LOC_CAT_ETHER, TEXT("Ether_UnmountAll_Impossible")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
		);
	}

	EtherIDs.Empty();
}

void UB2UIEther::OnClickAutoAllSell()
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::EtherAutoSell);
}

void UB2UIEther::OnClickAutoDecomposition()
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::EtherAutoDecomposition);
}

void UB2UIEther::OnClickEtherFiltering()
{
	if (X_UIP_EtherFiltering.IsValid() && X_UIP_EtherFiltering->GetVisibility() != ESlateVisibility::Visible)
	{
		X_UIP_EtherFiltering->SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(X_UIP_EtherFiltering.Get());
	}
}

void UB2UIEther::OnClickExtend()
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::EtherExtendInventory);
}

void UB2UIEther::RefreshCharacterEtherUI(bool IsEquip)
{
	if (X_UIP_EtherCharacterSlot.IsValid())
	{
		X_UIP_EtherCharacterSlot->SetInfo(this, CurrentTabType, IsEquip);
	}

	if (X_UIP_EtherOption.IsValid())
	{
		X_UIP_EtherOption->SetInfo(CurrentTabType);
	}
}

void UB2UIEther::RefreshScrollEtherUI()
{
	if (X_UIP_EtherScroll.IsValid())
	{
		CachedScrollOffset = X_UIP_EtherScroll->GetEtherScrollOffset();
		X_UIP_EtherScroll->SetEtherScroll(this, CurrentSetName, CachedScrollOffset);
	}
}

void UB2UIEther::SetCurrentSetName(int32 SetName)
{
	CurrentSetName = SetName;
	if (X_TB_EtherFiltering.IsValid())
	{
		if (CurrentSetName != b2network::B2AetherType::ALL && StaticFindEtherInfoTable() != NULL)
		{
			X_TB_EtherFiltering->SetText(FText::FromString(StaticFindEtherInfoTable()->GetEtherTypeName(CurrentSetName)));
		}
		else
		{
			X_TB_EtherFiltering->SetText(FText::FromName(TEXT("ALL")));
		}
	}
	RefreshScrollEtherUI();
}

void UB2UIEther::ResponseSellEther(const FB2SellEtherPtr EtherPtr)
{
	for (int64 Elem : EtherPtr->sell_aether_ids)
	{
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().DeleteEther(Elem);
	}

	BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().ResetStoredEthers();
	if (X_UIP_EtherFiltering.IsValid())
	{
		X_UIP_EtherFiltering->SetSlotInfo(false);
	}

	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, EtherPtr->current_gold);
	SetInvenCount(B2EtherManager::GetInstance().GetEtherInventoryCount());
}

void UB2UIEther::ResponseDecompositionEther(const FB2DecompositionEtherPtr EtherPtr)
{
	for (int64 Elem : EtherPtr->dismantle_aether_Ids)
	{
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().DeleteEther(Elem);
	}
	
	BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().ResetStoredEthers();
	if (X_UIP_EtherFiltering.IsValid())
	{
		X_UIP_EtherFiltering->SetSlotInfo(false);
	}

	BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountIncrease(EtherPtr->current_aether_piece->template_id, EtherPtr->current_aether_piece);
	SetInvenCount(B2EtherManager::GetInstance().GetEtherInventoryCount());
}

void UB2UIEther::EnhanceEther(const FB2EnhanceEtherPtr EtherPtr)
{
	if (EtherPtr->is_success)
	{
		//////////////////////////////////////////////////////////////////
		//액션 지표 로그 (캐릭터 에테르 강화)
		B2ActionLogSender Sender;
		Sender.setCategory(FString("CHARACTER"))
			  .setAction(FString("AEHTER_ENHANCE"))
			  .LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
		//////////////////////////////////////////////////////////////////

		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().DeleteEther(EtherPtr->aether->id);
		BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().AddEther(EtherPtr->aether);
		this->RefreshEtherUI(false);
	}
}

void UB2UIEther::SetInvenCount(int32 InCount)
{
	int32 MaxCount = BladeIIGameImpl::GetClientDataStore().GetEtherInvenMaxCount();
	int32 EtherCount = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetEtherTotalCount();

	if (EtherCount > 0)
	{
		X_TB_EmptyText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		X_TB_EmptyText->SetVisibility(ESlateVisibility::Visible);
	}

	if (X_TB_CurrentCount.IsValid() && X_TB_MaxCount.IsValid())
	{
		
		X_TB_CurrentCount->SetText(FText::AsNumber(EtherCount));
		X_TB_MaxCount->SetText(FText::AsNumber(InCount));

		if (EtherCount > InCount)
		{
			X_TB_CurrentCount->SetColorAndOpacity(FLinearColor::Red);
		}
		else
		{
			X_TB_CurrentCount->SetColorAndOpacity(FLinearColor::White);
		}
	}

	if (InCount >= MaxCount)
	{
		if (X_BTN_Extend.IsValid())
		{
			X_BTN_Extend->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (X_IMG_Plus.IsValid())
		{
			X_IMG_Plus->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIEther::InitEtherUIInfo()
{
	BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().ResetStoredEthers();
	IsSendModifyAetherNewFlag = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().IsEtherNewFlag();
	SetCurrentSetName(b2network::B2AetherType::ALL);

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		SetCharacterInfo(IntToPCClass(DocBattle->GetCurPCClass()));
	}
	else
	{
		SetCharacterInfo(EPCClass::EPC_Gladiator);
	}

	SetInvenCount(B2EtherManager::GetInstance().GetEtherInventoryCount());
}

void UB2UIEther::SellEtherItem(const int64 EtherID, const int32 EtherPrice)
{	
	if (X_UIP_EtherSellPopup.IsValid())
	{
		X_UIP_EtherSellPopup->SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(X_UIP_EtherSellPopup.Get());

		X_UIP_EtherSellPopup->PopupMain(EtherPrice,
			FSellItemPopupOnClick::CreateLambda([this, EtherID]() {
			TArray<int64> EtherIDs;
			EtherIDs.Add(EtherID);
			data_trader::Retailer::GetInstance().RequestSellAethers(EtherIDs);
			EtherIDs.Empty();
			X_UIP_EtherSellPopup->SetVisibility(ESlateVisibility::Collapsed);
			X_UIP_ScrollDetailPopup->OnClickClose();
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(X_UIP_EtherSellPopup.Get());
		}),
			FSellItemPopupOnClick::CreateLambda([this]() {
			X_UIP_EtherSellPopup->SetVisibility(ESlateVisibility::Collapsed);
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(X_UIP_EtherSellPopup.Get());
		}));
	}
}

void UB2UIEther::ResultAllSellEtherItem(const int32 EtherPrice)
{
	if (X_UIP_EtherSellPopup.IsValid())
	{
		X_UIP_EtherSellPopup->SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(X_UIP_EtherSellPopup.Get());

		X_UIP_EtherSellPopup->PopupAsSellEtherResult(EtherPrice,
			FSellItemPopupOnClick::CreateLambda([this]() {
			X_UIP_EtherSellPopup->SetVisibility(ESlateVisibility::Collapsed);
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(X_UIP_EtherSellPopup.Get());
		}));
	}
}

