// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIMailReceiveState.h"
#include "B2UIManager.h"
#include "B2ServerResultCodeTable.h"
#include "B2UIEtherFullPopup.h"
#include "B2UIUtil.h"
#include "Retailer.h"
#include "B2UIMailContent.h"
#include "B2MailTimer.h"
#include "B2UIMail.h"
#include "B2UILotteryAllReceive.h"
#include "B2UILotteryAllReceiveResult.h"
#include "BladeIIGameImpl.h"
#include "B2UIMail.h"


void B2UIMailReceiveState::OpenMail(int64 MailId, const FB2ResponseOpenMailPtr& MailInfo)
{
	UB2UIMail* MailUI = UB2UIManager::GetInstance()->GetUI<UB2UIMail>(UIFName::Mail);
	if (MailUI)
	{
		MailUI->OpenMailUI(MailId, MailInfo);
	}
}

void B2UIMailReceiveState::OpenMailError(const int32 ErrorCode, const FB2ResponseOpenMailPtr MailInfo)
{
	int32 InCharacterType = MailInfo->exceed_inven_character_type;
	auto PCClass = static_cast<EPCClass>(InCharacterType - 1);
	if (ErrorCode == B2ResultCode::FAILURE_NOT_ENOUGH_AETHER_INVENTORY_LEFT_SLOT)
	{
		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst)
		{
			int32 CurrentCount = MailInfo->current_aether_count;
			int32 MaxCount = MailInfo->aether_inventory_max_count;
			UB2UIEtherFullPopup* Popup = UIMgrInst->OpenUI<UB2UIEtherFullPopup>(UIFName::EtherFullPopup);
			Popup->SetEtherFullPopupInfo(CurrentCount, MaxCount);
		}
	}
	else if (ErrorCode == B2ResultCode::FAILURE_NOT_ENOUGH_INVENTORY_LEFT_SLOT && PCClass != EPCClass::EPC_End && InCharacterType >= 0)
	{
		B2UIUtil::OpenInventoryFullPopup(PCClass);
	}
	else
	{
		UB2UIManager::GetInstance()->OpenMsgPopupFromErrorCode(ErrorCode);
	}
}

void B2UIMailReceiveState::OpenAllMail(const FB2OpenAllMail& OpenedAllMail)
{

}

void B2UIMailReceiveState::OpenAllMailError(const int32 ErrorCode)
{
	UB2UIManager::GetInstance()->OpenMsgPopupFromErrorCode(ErrorCode);
}

void B2UIMailReceiveState::ReceiveAllBTN(const TArray<TWeakObjectPtr<class UB2UIMailContent>>& InMailContents, MailTabIndex InCurrentTabIndex)
{
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void B2UIMailReceiveBasicState::OpenAllMail(const FB2OpenAllMail& OpenedAllMail)
{
	//UB2UIMail* MailUI = UB2UIManager::GetInstance()->GetUI<UB2UIMail>(UIFName::Mail);
	//if (MailUI)
	//{
	//	FB2ResponseOpenAllMail AllMailReceiveData;
	//	AllMailReceiveData.mail_ids = GET_TUPLE_DATA(FB2ResponseOpenAllMail::mail_ids_index, OpenedAllMail);
	//	AllMailReceiveData.gold = GET_TUPLE_DATA(FB2ResponseOpenAllMail::gold_index, OpenedAllMail);
	//	AllMailReceiveData.gem = GET_TUPLE_DATA(FB2ResponseOpenAllMail::gem_index, OpenedAllMail);
	//	AllMailReceiveData.blade_point = GET_TUPLE_DATA(FB2ResponseOpenAllMail::blade_point_index, OpenedAllMail);
	//	AllMailReceiveData.social_point = GET_TUPLE_DATA(FB2ResponseOpenAllMail::social_point_index, OpenedAllMail);
	//	AllMailReceiveData.dark_crystal = GET_TUPLE_DATA(FB2ResponseOpenAllMail::dark_crystal_index, OpenedAllMail);
	//	AllMailReceiveData.stage_boost_ticket = GET_TUPLE_DATA(FB2ResponseOpenAllMail::stage_boost_ticket_index, OpenedAllMail);
	//	AllMailReceiveData.hot_time_gold_buff_ticket = GET_TUPLE_DATA(FB2ResponseOpenAllMail::hot_time_gold_buff_ticket_index, OpenedAllMail);
	//	AllMailReceiveData.hot_time_exp_buff_ticket = GET_TUPLE_DATA(FB2ResponseOpenAllMail::hot_time_exp_buff_ticket_index, OpenedAllMail);
	//	AllMailReceiveData.shop_ten_lottery_ticket = GET_TUPLE_DATA(FB2ResponseOpenAllMail::shop_ten_lottery_ticket_index, OpenedAllMail);
	//	AllMailReceiveData.item_infos = GET_TUPLE_DATA(FB2ResponseOpenAllMail::item_infos_index, OpenedAllMail);
	//	AllMailReceiveData.fame_point = GET_TUPLE_DATA(FB2ResponseOpenAllMail::fame_point_index, OpenedAllMail);
	//	AllMailReceiveData.stage_auto_clear_ticket = GET_TUPLE_DATA(FB2ResponseOpenAllMail::stage_auto_clear_ticket_index, OpenedAllMail);

	//	MailUI->OpenAllMailUI(AllMailReceiveData);
	//}
}

void B2UIMailReceiveBasicState::ReceiveAllBTN(const TArray<TWeakObjectPtr<UB2UIMailContent>>& InMailContents, MailTabIndex InCurrentTabIndex)
{
	int64 StartMailID = -1;
	int64 EndMailID = -1;
	TArray<int64> MailIDs;
	MailIDs.Empty();

	for (TWeakObjectPtr<UB2UIMailContent> MailInfoElem : InMailContents)
	{
		int MailID = MailInfoElem->GetMailId();
		MailIDs.Add(MailID);
	}

	MailIDs.Sort([](const int64& A, const int64& B)
	{
		bool Result = false;
		if (A < B)
		{
			Result = true;
		}
		return Result;
	});

	if (MailIDs.Num() > 0)
	{
		StartMailID = MailIDs[0];
		EndMailID = MailIDs[MailIDs.Num() - 1];
	}

	if (StartMailID > 0 && EndMailID > 0)
	{
		data_trader::Retailer::GetInstance().RequestOpenAllMail(static_cast<int32>(InCurrentTabIndex), StartMailID, EndMailID);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

B2UIMailReceiveAllState::B2UIMailReceiveAllState()
{
	IsAllReceive = false;
	SelectedEPCClass = EPCClass::EPC_End;
	SelectedItemRefId = 0;
}

void B2UIMailReceiveAllState::OpenMail(int64 MailId, const FB2ResponseOpenMailPtr& MailInfo)
{
	if (IsAllReceive == true)
	{
		if (HaveItemMailID.IsValidIndex(0) && HaveItemMailID[0] == MailId)
		{
			AllMailReceiveData.item_infos.Add(MailInfo->mail_item_info);
			AllMailReceiveData.mail_ids.Add(MailId);
			HaveItemMailID.RemoveAt(0);
		}

		if (HaveItemMailID.Num() > 0)
		{
			MailTimer.SetResponseTime();
			MailTimer.StartTimerRequestOpenMail(HaveItemMailID[0], SelectedEPCClass, SelectedItemRefId);
		}
		else if (AllMailReceiveData.mail_ids.Num() > 0)
		{
			IsAllReceive = false;
			OpenAllMailReceiveData();
		}
	}
	else
	{
		B2UIMailReceiveState::OpenMail(MailId, MailInfo);
	}
}

void B2UIMailReceiveAllState::OpenMailError(const int32 ErrorCode, const FB2ResponseOpenMailPtr MailInfo)
{
	if (IsAllReceive == true)
	{
		IsAllReceive = false;
		UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);

		if (this->AllMailReceiveData.gold > 0 ||
			this->AllMailReceiveData.gem > 0 ||
			this->AllMailReceiveData.blade_point > 0 ||
			this->AllMailReceiveData.social_point > 0 ||
			this->AllMailReceiveData.dark_crystal > 0 ||
			this->AllMailReceiveData.stage_boost_ticket > 0 ||
			this->AllMailReceiveData.stage_auto_clear_ticket > 0 ||
			this->AllMailReceiveData.hot_time_gold_buff_ticket > 0 ||
			this->AllMailReceiveData.hot_time_exp_buff_ticket > 0 ||
			this->AllMailReceiveData.shop_ten_lottery_ticket > 0 ||
			this->AllMailReceiveData.item_infos.Num() > 0)
		{
			OpenAllMailReceiveData();
		}
	}

	B2UIMailReceiveState::OpenMailError(ErrorCode, MailInfo);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void B2UIMailReceiveGiftState::OpenAllMail(const FB2OpenAllMail& OpenedAllMail)
{
	//AllMailReceiveData.mail_ids = GET_TUPLE_DATA(FB2ResponseOpenAllMail::mail_ids_index, OpenedAllMail);
	//AllMailReceiveData.gold = GET_TUPLE_DATA(FB2ResponseOpenAllMail::gold_index, OpenedAllMail);
	//AllMailReceiveData.gem = GET_TUPLE_DATA(FB2ResponseOpenAllMail::gem_index, OpenedAllMail);
	//AllMailReceiveData.blade_point = GET_TUPLE_DATA(FB2ResponseOpenAllMail::blade_point_index, OpenedAllMail);
	//AllMailReceiveData.social_point = GET_TUPLE_DATA(FB2ResponseOpenAllMail::social_point_index, OpenedAllMail);
	//AllMailReceiveData.dark_crystal = GET_TUPLE_DATA(FB2ResponseOpenAllMail::dark_crystal_index, OpenedAllMail);
	//AllMailReceiveData.stage_boost_ticket = GET_TUPLE_DATA(FB2ResponseOpenAllMail::stage_boost_ticket_index, OpenedAllMail);
	//AllMailReceiveData.stage_auto_clear_ticket = GET_TUPLE_DATA(FB2ResponseOpenAllMail::stage_auto_clear_ticket_index, OpenedAllMail);
	//AllMailReceiveData.hot_time_gold_buff_ticket = GET_TUPLE_DATA(FB2ResponseOpenAllMail::hot_time_gold_buff_ticket_index, OpenedAllMail);
	//AllMailReceiveData.hot_time_exp_buff_ticket = GET_TUPLE_DATA(FB2ResponseOpenAllMail::hot_time_exp_buff_ticket_index, OpenedAllMail);
	//AllMailReceiveData.shop_ten_lottery_ticket = GET_TUPLE_DATA(FB2ResponseOpenAllMail::shop_ten_lottery_ticket_index, OpenedAllMail);
	//AllMailReceiveData.item_infos = GET_TUPLE_DATA(FB2ResponseOpenAllMail::item_infos_index, OpenedAllMail);
	//AllMailReceiveData.fame_point = GET_TUPLE_DATA(FB2ResponseOpenAllMail::fame_point_index, OpenedAllMail);

	//if (HaveItemMailID.Num() > 0)
	//{
	//	MailTimer.StartTimerRequestOpenMail(HaveItemMailID[0], SelectedEPCClass, SelectedItemRefId);
	//}
	//else
	//{
	//	IsAllReceive = false;
	//	OpenAllMailReceiveData();
	//}
}

void B2UIMailReceiveGiftState::OpenAllMailError(const int32 ErrorCode)
{
	IsAllReceive = false;

	UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);
	B2UIMailReceiveState::OpenAllMailError(ErrorCode);
}

void B2UIMailReceiveGiftState::ReceiveAllBTN(const TArray<TWeakObjectPtr<UB2UIMailContent>>& InMailContents, MailTabIndex InCurrentTabIndex)
{
	UB2UIManager::GetInstance()->OpenUI(UIFName::ClientIndicator);
	IsAllReceive = true;
	HaveItemMailID.Empty();

	int64 StartMailID = -1;
	int64 EndMailID = -1;
	TArray<int64> MailIDs;

	for (TWeakObjectPtr<UB2UIMailContent> MailInfoElem : InMailContents)
	{
		int MailID = MailInfoElem->GetMailId();
		MailIDs.Add(MailID);

		MailGiftType GiftType = MailInfoElem->GetGiftType();
		if (GiftType == MailGiftType::Item)
		{
			HaveItemMailID.Add(MailID);
		}
	}

	MailIDs.Sort([](const int64& A, const int64& B)
	{
		bool Result = false;
		if (A < B)
		{
			Result = true;
		}
		return Result;
	});

	if (MailIDs.Num() > 0 && MailIDs.Num() > HaveItemMailID.Num())
	{
		StartMailID = MailIDs[0];
		EndMailID = MailIDs[MailIDs.Num() - 1];
	}

	if (StartMailID > 0 && EndMailID > 0)
	{
		data_trader::Retailer::GetInstance().RequestOpenAllMail(static_cast<int32>(InCurrentTabIndex), StartMailID, EndMailID);
	}
	else if (HaveItemMailID.Num() > 0)
	{
		MailTimer.StartTimerRequestOpenMail(HaveItemMailID[0], SelectedEPCClass, SelectedItemRefId);
	}
	else
	{
		UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);
	}
}

void B2UIMailReceiveGiftState::OpenAllMailReceiveData()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);
	UB2UIMail* MailUI = UB2UIManager::GetInstance()->GetUI<UB2UIMail>(UIFName::Mail);
	if (MailUI)
	{
		MailUI->OpenAllMailUI(AllMailReceiveData);
	}

	AllMailReceiveData.mail_ids.Empty();
	AllMailReceiveData.gold = 0;
	AllMailReceiveData.gem = 0;
	AllMailReceiveData.blade_point = 0;
	AllMailReceiveData.social_point = 0;
	AllMailReceiveData.dark_crystal = 0;
	AllMailReceiveData.fame_point = 0;
	AllMailReceiveData.stage_boost_ticket = 0;
	AllMailReceiveData.stage_auto_clear_ticket = 0;
	AllMailReceiveData.hot_time_gold_buff_ticket = 0;
	AllMailReceiveData.hot_time_exp_buff_ticket = 0;
	AllMailReceiveData.shop_ten_lottery_ticket = 0;
	AllMailReceiveData.item_infos.Empty();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

B2UIMailReceiveTicketState::B2UIMailReceiveTicketState() : B2UIMailReceiveAllState()
{
	SetLotteryAllFilteringInfoClass<TArray<int64>&, EPCClass, int32>::GetInstance().Subscribe(
		[this](TArray<int64>& InSelectMailIDs, EPCClass InSelectEPCClass, int32 InSelectedItemRefId)
	{
		HaveItemMailID.Empty();
		SelectedEPCClass = InSelectEPCClass;
		SelectedItemRefId = InSelectedItemRefId;

		for (int64 Elem : InSelectMailIDs)
		{
			HaveItemMailID.Add(Elem);
		}

		if (HaveItemMailID.Num() > 0)
		{
			UB2UIManager::GetInstance()->OpenUI(UIFName::ClientIndicator);
			IsAllReceive = true;
			MailTimer.StartTimerRequestOpenMail(HaveItemMailID[0], SelectedEPCClass, SelectedItemRefId);
		}
		else
		{
			UB2UIManager::GetInstance()->OpenMsgPopup(
				EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MailLottery_QuickErrorSorting")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm
			);
		}
	});
}

B2UIMailReceiveTicketState::~B2UIMailReceiveTicketState()
{
	SetLotteryAllFilteringInfoClass<TArray<int64>&, EPCClass, int32>::GetInstance().UnsubscribeAll();
}

void B2UIMailReceiveTicketState::ReceiveAllBTN(const TArray<TWeakObjectPtr<class UB2UIMailContent>>& InMailContents, MailTabIndex InCurrentTabIndex)
{
	UB2UILotteryAllReceive* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UILotteryAllReceive>(UIFName::LotteryAllReceive);
	Popup->SetLotteryMailInfo(InMailContents);
}

void B2UIMailReceiveTicketState::OpenAllMailReceiveData()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);

	TArray<FB2Item> ResultItems;
	for (b2network::B2MailItemInfoPtr& ItemInfo : AllMailReceiveData.item_infos)
	{
		if (ItemInfo->item.get())
		{
			FB2Item AddItem;
			AddItem = ItemInfo->item;

			if (AddItem.InventoryType != EItemInvenType::EIIVT_Consumables)
			{
				BladeIIGameImpl::GetClientDataStore().AddNewAcquiredItem(AddItem);
			}
			else
			{
				FB2Item* ThisItem = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().FindItem(AddItem.InstanceUID);
				if (ThisItem && ThisItem->ConsumingAmount < AddItem.ConsumingAmount)
				{
					BladeIIGameImpl::GetClientDataStore().AddNewAcquiredItem(AddItem);
				}

				AddItem.ConsumingAmount = ItemInfo->item_amount; // UI표시를 위해 변경
			}

			ResultItems.Add(AddItem);
		}
	}

	UB2UILotteryAllReceiveResult* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UILotteryAllReceiveResult>(UIFName::LotteryAllReceiveResult);
	if (Popup)
	{
		Popup->UpdateResultItemIcons(ResultItems);
	}

	UB2UIMail* MailUI = UB2UIManager::GetInstance()->GetUI<UB2UIMail>(UIFName::Mail);
	if (MailUI)
	{
		if (MailUI->IsRare)
			MailUI->rare_upgradestone_mail_totalcount = MailUI->changed_rare_upgradestone;
		else
			MailUI->upgradestone_mail_totalcount = MailUI->changed_upgradestone;
		
		MailUI->UpdateMailList(AllMailReceiveData.mail_ids);
		MailUI->UpdateMailList();
	}

	AllMailReceiveData.mail_ids.Empty();
	AllMailReceiveData.gold = 0;
	AllMailReceiveData.gem = 0;
	AllMailReceiveData.blade_point = 0;
	AllMailReceiveData.social_point = 0;
	AllMailReceiveData.dark_crystal = 0;
	AllMailReceiveData.fame_point = 0;
	AllMailReceiveData.stage_boost_ticket = 0;
	AllMailReceiveData.stage_auto_clear_ticket = 0;
	AllMailReceiveData.hot_time_gold_buff_ticket = 0;
	AllMailReceiveData.hot_time_exp_buff_ticket = 0;
	AllMailReceiveData.shop_ten_lottery_ticket = 0;
	AllMailReceiveData.item_infos.Empty();
}

