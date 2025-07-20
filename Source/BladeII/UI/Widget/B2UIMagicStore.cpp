// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIMagicStore.h"
#include "B2UIManager.h"
#include "Retailer.h"
#include "BladeIIGameImpl.h"
#include "B2GameInstance.h"
#include "B2UIDocHelper.h"
#include "B2UIMagicStoreItemSlot.h"
#include "B2UIMsgPopupMagicStoreRefresh.h"
#include "B2UIMsgPopupMagicStoreBuyConfirm.h"



void UB2UIMagicStore::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, X_CP_MagicStoreRefresh_Panel);
	GET_SLOT(UCanvasPanel, X_CP_MagicStoreBuyConfirm_Panel);

	GET_SLOT(UTextBlock, TB_RemainTitle);
	GET_SLOT(UTextBlock, TB_RemainTime);
	GET_SLOT(UB2Button, BTN_Refresh);
	GET_SLOT(UTextBlock, TB_Refresh);
	GET_SLOT(UTextBlock, TB_Banner);

	// List
	MagicStoreItemSlotList.SetNum(10);
	GET_SLOT_BYNAME(UB2UIMagicStoreItemSlot, MagicStoreItemSlotList[0], UIP_MagicStore_ItemSlot_0);
	GET_SLOT_BYNAME(UB2UIMagicStoreItemSlot, MagicStoreItemSlotList[1], UIP_MagicStore_ItemSlot_1);
	GET_SLOT_BYNAME(UB2UIMagicStoreItemSlot, MagicStoreItemSlotList[2], UIP_MagicStore_ItemSlot_2);
	GET_SLOT_BYNAME(UB2UIMagicStoreItemSlot, MagicStoreItemSlotList[3], UIP_MagicStore_ItemSlot_3);
	GET_SLOT_BYNAME(UB2UIMagicStoreItemSlot, MagicStoreItemSlotList[4], UIP_MagicStore_ItemSlot_4);
	GET_SLOT_BYNAME(UB2UIMagicStoreItemSlot, MagicStoreItemSlotList[5], UIP_MagicStore_ItemSlot_5);
	GET_SLOT_BYNAME(UB2UIMagicStoreItemSlot, MagicStoreItemSlotList[6], UIP_MagicStore_ItemSlot_6);
	GET_SLOT_BYNAME(UB2UIMagicStoreItemSlot, MagicStoreItemSlotList[7], UIP_MagicStore_ItemSlot_7);
	GET_SLOT_BYNAME(UB2UIMagicStoreItemSlot, MagicStoreItemSlotList[8], UIP_MagicStore_ItemSlot_8);
	GET_SLOT_BYNAME(UB2UIMagicStoreItemSlot, MagicStoreItemSlotList[9], UIP_MagicStore_ItemSlot_9);
	for (const auto &Elem : MagicStoreItemSlotList)
	{
		if (Elem.IsValid())
		{
			Elem->Init();
		}
	}
}

void UB2UIMagicStore::UpdateStaticText()
{
	if (TB_RemainTitle.IsValid())
		TB_RemainTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MagicStore_RemainTitle")));
	if (TB_Refresh.IsValid())
		TB_Refresh->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MagicStore_Refresh")));
	if (TB_Banner.IsValid())
		TB_Banner->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MagicStore_Banner")));
}

void UB2UIMagicStore::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Refresh, &UB2UIMagicStore::OnClickBTN_Refresh);
}

UB2UIMagicStore::UB2UIMagicStore(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, NextRenewalTime(0)
	, StartTimeTick(false)
	, WaitingRequest(false)
	, BuyItemProductID(0)

{
	Created_UIP_MagicStoreRefresh = nullptr;
	Created_UIP_MagicStoreBuyConfirm = nullptr;
}

void UB2UIMagicStore::Init()
{
	Super::Init();

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_MagicStore")));

	SubscribeEvents();
}

void UB2UIMagicStore::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateRemainTime();
}

void UB2UIMagicStore::DestroySelf(UB2UIManager* InUIManager)
{
	NextRenewalTime = 0;
	StartTimeTick = false;
	WaitingRequest = false;
	BuyItemProductID = 0;

	UnsubscribeEvents();

	DestroyMagicStoreRefreshPopup();
	DestroyMagicStoreBuyConfirmPopup();

	for (const auto &Elem : MagicStoreItemSlotList)
	{
		if (Elem.IsValid())
		{
			Elem->DestroySelf(InUIManager);
		}
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIMagicStore::SubscribeEvents()
{
	UnsubscribeEvents();

	//Issues.Add(DeliveryRenewalMagicShopClass<FB2ResponseRenewalMagicShopPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseRenewalMagicShopPtr MagicShopPtr)
	//{
	//	this->ResponseMagicShopData(MagicShopPtr);
	//	this->ResponseReplaceCurrentGem(MagicShopPtr);
	//	this->SetItemList();
	//}
	//));

	//Issues.Add(DeliveryPurchaseMagicShopClass<FB2ResponsePurchaseMagicShopPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponsePurchaseMagicShopPtr MagicShopPtr)
	//{
	//	this->ResponseMagicShopData(MagicShopPtr);
	//	this->ResponseBuyMagicShopProduct(MagicShopPtr);
	//	this->SetItemList(false);
	//}
	//));

	//Issues.Add(DeliveryReceiveRenewalMagicShopErrorClass<FB2ResponseRenewalMagicShopPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseRenewalMagicShopPtr MagicShopPtr)
	//{
	//	this->ResponseMagicShopData(MagicShopPtr);
	//	this->ResponseRenewalMagicShopErrorPopup();
	//}
	//));

	//Issues.Add(DeliveryReceivePurchaseMagicShopErrorClass<FB2ResponsePurchaseMagicShopPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponsePurchaseMagicShopPtr MagicShopPtr)
	//{
	//	this->ResponseMagicShopData(MagicShopPtr);
	//	this->ResponsePurchaseMagicShopErrorPopup();
	//}
	//));
}

void UB2UIMagicStore::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UIMagicStore::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);

	SetItemList(false);
	SetRemainTime();
}

void UB2UIMagicStore::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);
}

void UB2UIMagicStore::SetItemList(bool bAnimPlay)
{
	WaitingRequest = false;

	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		TArray<FMagicShopItem> ItemList = DocStore->GetMagicShopItemList();

		for (int i = 0; i < ItemList.Num(); i++)
		{
			int32 ProductID = ItemList[i].ProductID;
			MagicStoreItemSlotList[i]->SetVisibilityItem(bAnimPlay ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
			MagicStoreItemSlotList[i]->SetData(ProductID, ItemList[i].IsPurchased);
			MagicStoreItemSlotList[i]->BuyButtonDelegateBindLambda(
				[this, ProductID]() { OnClickBuyButtonDelegate(ProductID);
			});

			if (bAnimPlay)
				MagicStoreItemSlotList[i]->StartAnimation(i);
		}
	}
}

void UB2UIMagicStore::SetRemainTime()
{
	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int64 NextTime = DocStore->GetNextFreeRenewalTime();
		NextRenewalTime = FDateTime::FromUnixTimestamp(NextTime / 1000);

		StartTimeTick = true;
	}
}

void UB2UIMagicStore::UpdateRemainTime()
{
	if (!StartTimeTick)
		return;

	FTimespan RemainTime = NextRenewalTime - UB2GameInstance::GetUTCNow();
	int32 RemainHour = RemainTime.GetHours();
	int32 RemainMin = RemainTime.GetMinutes();
	int32 RemainSecond = RemainTime.GetSeconds();

	if (RemainTime.GetTicks() < 0 && StartTimeTick)
	{
		StartTimeTick = false;

		bool IsOpendPopup = false;
		if (Created_UIP_MagicStoreRefresh)
		{
			IsOpendPopup = Created_UIP_MagicStoreRefresh->IsOpened();
		}
		if (Created_UIP_MagicStoreBuyConfirm && !IsOpendPopup)
		{
			IsOpendPopup = Created_UIP_MagicStoreBuyConfirm->IsOpened();
		}
		if (IsOpendPopup)
		{
			WaitingRequest = true;
		}
		else
		{
			RequestGetMagicShop();
		}
	}

	if (TB_RemainTime.IsValid())
	{
		if (RemainHour > 0 || RemainMin > 0)
		{
			TB_RemainTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MagicStore_RefreshTime")),
				FText::AsNumber(RemainHour), FText::AsNumber(RemainMin)));
		}
		else
		{
			TB_RemainTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MagicStore_RefreshTimeSecond")),
				FText::AsNumber(RemainSecond)));
		}
	}
}

void UB2UIMagicStore::RequestGetMagicShop()
{
	data_trader::Retailer::GetInstance().RequestGetMagicShop();
}

template<class T>
void UB2UIMagicStore::ResponseMagicShopData(const T& MagicShopPtr)
{
	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		// 寸老 盎脚 冉荐
		DocStore->SetRenewalCount(MagicShopPtr->renewal_count);

		// 促澜 磊悼 盎脚 矫埃
		DocStore->SetNextFreeRenewalTime(MagicShopPtr->next_free_renewal_time);

		// 公丰 盎脚 咯何
		DocStore->SetIsFreeRenewal(MagicShopPtr->is_free_renewal);

		// 付过 惑痢 惑前 府胶飘
		DocStore->ResetMagicShopList();
		for (auto MagicShopItem : MagicShopPtr->magic_shops)
		{
			DocStore->SetMagicShopList(MagicShopItem);
		}
	}
}

void UB2UIMagicStore::ResponseReplaceCurrentGem(const FB2ResponseRenewalMagicShopPtr& MagicShopPtr)
{
	// 泅犁 焊蜡茄 焊籍 荐樊
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, MagicShopPtr->current_gem);
}

void UB2UIMagicStore::ResponseBuyMagicShopProduct(const FB2ResponsePurchaseMagicShopPtr& MagicShopPtr)
{
	// 荤侩茄 犁拳
	BladeIIGameImpl::GetClientDataStore().ResponseBuyMagicShopProduct(MagicShopPtr);
}

void UB2UIMagicStore::ResponseRenewalMagicShopErrorPopup()
{
	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MagicStore_RefreshInit")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm,
		FMsgPopupOnClick::CreateLambda([this]() {
		SetItemList(true);
		SetRemainTime();
	})
		);
}

void UB2UIMagicStore::ResponsePurchaseMagicShopErrorPopup()
{
	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MagicStore_EndTime")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm,
		FMsgPopupOnClick::CreateLambda([this]() {
		SetItemList(true);
		SetRemainTime();
	})
		);
}

/* Refresh Popup */
void UB2UIMagicStore::ConditionalCreateMagicStoreRefreshPopup()
{
	//if (!Created_UIP_MagicStoreRefresh && X_CP_MagicStoreRefresh_Panel.IsValid())
	//{
	//	Created_UIP_MagicStoreRefresh = DynLoadAndCreateInCanvasPanelFull<UB2UIMsgPopupMagicStoreRefresh>(UIP_MagicStoreRefresh_Class, this, X_CP_MagicStoreRefresh_Panel.Get());
	//	if (Created_UIP_MagicStoreRefresh)
	//	{
	//		Created_UIP_MagicStoreRefresh->Init();

	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_MagicStoreRefresh->GetSlot<UB2Button>(TEXT("BTN_Close"))), &UB2UIMagicStore::CloseMagicStoreRefreshPopup);
	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_MagicStoreRefresh->GetSlot<UB2Button>(TEXT("BTN_Refresh"))), &UB2UIMagicStore::BuyMagicStoreRefreshPopup);
	//	}
	//}
}

void UB2UIMagicStore::OpenMagicStoreRefreshPopup()
{
	ConditionalCreateMagicStoreRefreshPopup();

	if (Created_UIP_MagicStoreRefresh)
	{
		Created_UIP_MagicStoreRefresh->OpenPopup();
	}
}

void UB2UIMagicStore::CloseMagicStoreRefreshPopup()
{
	if (Created_UIP_MagicStoreRefresh)
	{
		Created_UIP_MagicStoreRefresh->ClosePopup();
	}

	if (WaitingRequest)
	{
		RequestGetMagicShop();
	}
}

void UB2UIMagicStore::BuyMagicStoreRefreshPopup()
{
	if (Created_UIP_MagicStoreRefresh)
	{
		Created_UIP_MagicStoreRefresh->ClosePopup();

		if (auto* DocStore = UB2UIDocHelper::GetDocStore())
		{
			int32 RenewalCount = DocStore->GetRenewalCount();
			int32 RenewalMaxCount = BladeIIGameImpl::GetClientDataStore().GetMagicShopRenewalCostDataCount();
			if (RenewalCount < RenewalMaxCount)
			{
				data_trader::Retailer::GetInstance().RequestRenewalMagicShop();
			}
		}
	}
}

void UB2UIMagicStore::DestroyMagicStoreRefreshPopup()
{
	if (Created_UIP_MagicStoreRefresh)
	{
		Created_UIP_MagicStoreRefresh->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_MagicStoreRefresh = nullptr;
	}
}

/* Buy Confirm Popup */
void UB2UIMagicStore::ConditionalCreateMagicStoreBuyConfirmPopup()
{
	//if (!Created_UIP_MagicStoreBuyConfirm && X_CP_MagicStoreBuyConfirm_Panel.IsValid())
	//{
	//	Created_UIP_MagicStoreBuyConfirm = DynLoadAndCreateInCanvasPanelFull<UB2UIMsgPopupMagicStoreBuyConfirm>(UIP_MagicStoreBuyConfirm_Class, this, X_CP_MagicStoreBuyConfirm_Panel.Get());
	//	if (Created_UIP_MagicStoreBuyConfirm)
	//	{
	//		Created_UIP_MagicStoreBuyConfirm->Init();

	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_MagicStoreBuyConfirm->GetSlot<UB2Button>(TEXT("BTN_Close"))), &UB2UIMagicStore::CloseMagicStoreBuyConfirmPopup);
	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_MagicStoreBuyConfirm->GetSlot<UB2Button>(TEXT("BTN_Buy"))), &UB2UIMagicStore::BuyMagicStoreBuyConfirmPopup);
	//	}
	//}
}

void UB2UIMagicStore::OpenMagicStoreBuyConfirmPopup()
{
	ConditionalCreateMagicStoreBuyConfirmPopup();

	if (Created_UIP_MagicStoreBuyConfirm)
	{
		Created_UIP_MagicStoreBuyConfirm->OpenPopup();
	}
}

void UB2UIMagicStore::SetMagicStoreBuyConfirmPopup(int32 ProductID)
{
	if (Created_UIP_MagicStoreBuyConfirm)
	{
		Created_UIP_MagicStoreBuyConfirm->SetBuyConfirmPopupData(ProductID);
	}
}

void UB2UIMagicStore::CloseMagicStoreBuyConfirmPopup()
{
	if (Created_UIP_MagicStoreBuyConfirm)
	{
		Created_UIP_MagicStoreBuyConfirm->ClosePopup();
	}

	if (WaitingRequest)
	{
		RequestGetMagicShop();
	}
}

void UB2UIMagicStore::BuyMagicStoreBuyConfirmPopup()
{
	if (Created_UIP_MagicStoreBuyConfirm)
	{
		Created_UIP_MagicStoreBuyConfirm->ClosePopup();

		data_trader::Retailer::GetInstance().RequestPurchaseMagicShop(BuyItemProductID);
	}
}

void UB2UIMagicStore::DestroyMagicStoreBuyConfirmPopup()
{
	if (Created_UIP_MagicStoreBuyConfirm)
	{
		Created_UIP_MagicStoreBuyConfirm->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_MagicStoreBuyConfirm = nullptr;
	}
}

void UB2UIMagicStore::OnClickBTN_Refresh()
{
	OpenMagicStoreRefreshPopup();
}

void UB2UIMagicStore::OnClickBuyButtonDelegate(int32 ProductID)
{
	BuyItemProductID = ProductID;

	OpenMagicStoreBuyConfirmPopup();
	SetMagicStoreBuyConfirmPopup(BuyItemProductID);
}
