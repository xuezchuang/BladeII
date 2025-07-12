#include "B2UIMail.h"
#include "B2UIHeader.h"
#include "B2UIManager.h"
#include "Event.h"
#include "EventSubsystem.h"
#include "B2UIMsgPopupReward.h"
#include "B2UIGeneralLotteryPopup.h"
#include "B2UISelectiveLotteryPopUp.h"
#include "B2UISelectiveLotteryConsumeItemResultPopup.h"
#include "B2UIMailSelectiveLotteryResult.h"
#include "B2RedDotManager.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "BladeIIGameImpl.h"
#include "B2UIMailReceiveState.h"
#include "B2UIUpgradeStoneAllReceive.h"



int32 ConvertMailIndexToInt32(MailTabIndex Index)
{
	switch (Index)
	{
	case MailTabIndex::EMailTab_Gift:			return 0;
	case MailTabIndex::EMailTab_BladePoint:		return 1;
	case MailTabIndex::EMailTab_Friendship:		return 2;
	case MailTabIndex::EMailTab_Ticket:			return 3;
	case MailTabIndex::EMailTab_Max:			return 4;
	}

	return INDEX_NONE;
}

void UB2UIMail::SubscribeEvent()
{
	UnsubscribeEvent();

	Issues.Add(DeliveryOpenMailClass<int64, FB2ResponseOpenMailPtr>::GetInstance().Subscribe2(
		[this](int64 MailId, const FB2ResponseOpenMailPtr& MailInfo)
	{
		if (auto* UIManager = UB2UIManager::GetInstance())
		{
			auto* MailUI = UIManager->GetUI<UB2UIMail>(UIFName::Mail);
			if (MailUI)
				MailUI->OpenMail(MailId, MailInfo);
		}

		this->SetTouch(false);
	}
	));

	Issues.Add(DeliveryOpenAllMailClass<FB2OpenAllMail>::GetInstance().Subscribe2(
		[this](const FB2OpenAllMail& OpenedAllMail)
	{
		if (auto* UIManager = UB2UIManager::GetInstance())
		{
			auto* MailUI = UIManager->GetUI<UB2UIMail>(UIFName::Mail);
			if (MailUI)
				MailUI->OpenAllMail(OpenedAllMail);
		}
	}
	));

	Issues.Add(DeliveryOpenMailErrorClass<int32, FB2ResponseOpenMailPtr>::GetInstance().Subscribe2(
		[this](const int32 ErrorCode, const FB2ResponseOpenMailPtr MailInfo)
	{
		this->SetTouch(false);
		MailReceiveState->OpenMailError(ErrorCode, MailInfo);
	}
	));

	Issues.Add(DeliveryOpenAllMailErrorClass<int32>::GetInstance().Subscribe2(
		[this](const int32 ErrorCode)
	{
		MailReceiveState->OpenAllMailError(ErrorCode);
	}
	));

	Issues.Add(RequestCurrentTabMailListClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->RequestCurrentTabMailList();
	}
	));
}

void UB2UIMail::UnsubscribeEvent()
{
	Issues.Empty();
}

void UB2UIMail::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvent();

	DestroyGeneralLotteryPopup();
	DestroySelectiveLotteryPopup();
	DestroyAllSelectiveLotteryPopup();
	DestroyAllSelectiveLotteryCostumePopup();
	DestroySelectiveLotteryConsumeItemPopup();
	DestroySelectiveLotteryConsumeItemResultPopup();
	DestroySelectiveLotteryResultPopup();

	DeleteMailReceiveState();

	Super::DestroySelf(InUIManager);
}

void UB2UIMail::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("GiftTab")), &UB2UIMail::RedDotCondition_Gift);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("BladePointTab")), &UB2UIMail::RedDotCondition_BladePoint);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("FriendshipTab")), &UB2UIMail::RedDotCondition_FriendShip);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("TicketTab")), &UB2UIMail::RedDotCondition_Ticket);
}

void UB2UIMail::DoMarkRedDot()
{
	Super::DoMarkRedDot();
}

UB2UIMail::UB2UIMail(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CreatedGeneralLotteryPopup = nullptr;
	CreatedSelectiveLotteryPopup = nullptr;
	CreatedSelectiveLotteryConsumeItemPopup = nullptr;
	CreatedSelectiveLotteryConsumeItemResultPopup = nullptr;
	CreatedSelectiveLotteryResultPopup = nullptr;
	CreatedAllSelectiveLotteryResultPopup = nullptr;
	CreatedAllSelectiveLotteryCostumePopup = nullptr;
}

void UB2UIMail::Init()
{
	SubscribeEvent();

	Super::Init();
	IsTouch = false;
	CurrentTabIndex = MailTabIndex::EMailTab_Max;
	auto* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome)
		SetTabIndex(DocSome->GetOpenedMailTab());
	else
		SetTabIndex(MailTabIndex::EMailTab_Gift);

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyMain_Mail"));
	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::Mail);
}

void UB2UIMail::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	BladeIIGameImpl::GetRedDotManager().RequestCheckRedDot({ RedDotHint::RED_DOT_MAIL });

	// UIHeader 와 같이 Scene 으로 구성하는 경우 Init 시점에 하면 Scene 에 포함된 header 가 생성이 안된 상황일 수 있음.
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyMain_Mail"));
}

void UB2UIMail::CacheAssets()
{
	MailTabs.Empty(ConvertMailIndexToInt32(MailTabIndex::EMailTab_Max));
	MailTabs.AddDefaulted(ConvertMailIndexToInt32(MailTabIndex::EMailTab_Max));

	TAB_MENU_SLOT(Gift, MailTabs[ConvertMailIndexToInt32(MailTabIndex::EMailTab_Gift)]);
	TAB_MENU_SLOT(BladePoint, MailTabs[ConvertMailIndexToInt32(MailTabIndex::EMailTab_BladePoint)]);
	TAB_MENU_SLOT(Friendship, MailTabs[ConvertMailIndexToInt32(MailTabIndex::EMailTab_Friendship)]);
	TAB_MENU_SLOT(Ticket, MailTabs[ConvertMailIndexToInt32(MailTabIndex::EMailTab_Ticket)]);

	GET_SLOT(UVerticalBox, VB_MailList);
	GET_SLOT(UB2Button, BTN_ReceiveAll);
	GET_SLOT(UB2Button, BTN_UpgradeStoneReceiveAll);
	GET_SLOT(UB2Button, BTN_RareUpgradeStoneReceiveAll);
	GET_SLOT(UTextBlock, TB_ReceiveAll);
	GET_SLOT(UTextBlock, TB_UpgradeStone);
	GET_SLOT(UTextBlock, TB_CurrentMail);
	GET_SLOT(UTextBlock, TB_EmptySlot);

	GET_SLOT(UCanvasPanel, X_CP_UIPCreateBase);
}

void UB2UIMail::UpdateStaticText()
{
	if (TB_UpgradeStone.IsValid())
	{
		TB_UpgradeStone->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetRewardAll_GradeStone")));
	}

	if (TB_EmptySlot.IsValid())
		TB_EmptySlot->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("MailDefaultDesc")));
}

void UB2UIMail::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(MailTabs[ConvertMailIndexToInt32(MailTabIndex::EMailTab_Gift)].Button, &UB2UIMail::OnClicked_GiftTabBTN);
	BIND_CLICK_FUNC_TO_BTN(MailTabs[ConvertMailIndexToInt32(MailTabIndex::EMailTab_BladePoint)].Button, &UB2UIMail::OnClicked_BladePointTabBTN);
	BIND_CLICK_FUNC_TO_BTN(MailTabs[ConvertMailIndexToInt32(MailTabIndex::EMailTab_Friendship)].Button, &UB2UIMail::OnClicked_FriendshipTabBTN);
	BIND_CLICK_FUNC_TO_BTN(MailTabs[ConvertMailIndexToInt32(MailTabIndex::EMailTab_Ticket)].Button, &UB2UIMail::OnClicked_TicketTabBTN);

	BIND_CLICK_FUNC_TO_BTN(BTN_ReceiveAll, &UB2UIMail::OnClicked_ReceiveAllBTN);
	BIND_CLICK_FUNC_TO_BTN(BTN_UpgradeStoneReceiveAll, &UB2UIMail::OnClicked_UpgradeStoneReceiveAllBTN);
	BIND_CLICK_FUNC_TO_BTN(BTN_RareUpgradeStoneReceiveAll, &UB2UIMail::OnClicked_RareUpgradeStoneReceiveAllBTN);
}

void UB2UIMail::SetTabIndex(MailTabIndex Index)
{
	CurrentTabIndex = Index;
	SetMailReceiveState(CurrentTabIndex);

	for (int32 i = 0; i < ConvertMailIndexToInt32(MailTabIndex::EMailTab_Max); ++i)
	{
		if (MailTabs[i].SelectedImage.IsValid())
			MailTabs[i].SelectedImage->SetVisibility(i == ConvertMailIndexToInt32(Index) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		if (MailTabs[i].SelectedText.IsValid())
			MailTabs[i].SelectedText->SetVisibility(i == ConvertMailIndexToInt32(Index) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		if (MailTabs[i].UnselectedImage.IsValid())
			MailTabs[i].UnselectedImage->SetVisibility(i != ConvertMailIndexToInt32(Index) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		if (MailTabs[i].UnselectedText.IsValid())
			MailTabs[i].UnselectedText->SetVisibility(i != ConvertMailIndexToInt32(Index) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

		MailTabs[i].Mails.Empty();
	}

	if (Index == MailTabIndex::EMailTab_Ticket)
	{
		if (BTN_UpgradeStoneReceiveAll.IsValid())
		{
			BTN_UpgradeStoneReceiveAll->SetVisibility(ESlateVisibility::Visible);
		}

		if (BTN_RareUpgradeStoneReceiveAll.IsValid())
		{
			BTN_RareUpgradeStoneReceiveAll->SetVisibility(ESlateVisibility::Visible);
		}

		if (TB_ReceiveAll.IsValid())
		{
			TB_ReceiveAll->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetRewardAll_EquipEssence")));
		}
	}
	else
	{
		if (TB_ReceiveAll.IsValid())
		{
			TB_ReceiveAll->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Gemeral_GetAllReward")));
		}

		if (BTN_UpgradeStoneReceiveAll.IsValid())
		{
			BTN_UpgradeStoneReceiveAll->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BTN_RareUpgradeStoneReceiveAll.IsValid())
		{
			BTN_RareUpgradeStoneReceiveAll->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (VB_MailList.IsValid())
	{
		VB_MailList->ClearChildren();
	}
}

void UB2UIMail::OpenAllMailUI(FB2ResponseOpenAllMail InAllMailReceiveData)
{
	OpenAllMailUI
	(
		InAllMailReceiveData.gold,
		InAllMailReceiveData.gem,
		InAllMailReceiveData.blade_point,
		InAllMailReceiveData.social_point,
		InAllMailReceiveData.dark_crystal,
		InAllMailReceiveData.fame_point,
		InAllMailReceiveData.stage_boost_ticket,
		InAllMailReceiveData.stage_auto_clear_ticket,
		InAllMailReceiveData.hot_time_gold_buff_ticket,
		InAllMailReceiveData.hot_time_exp_buff_ticket,
		InAllMailReceiveData.shop_ten_lottery_ticket,
		InAllMailReceiveData.item_infos,
		InAllMailReceiveData.mail_ids
	);
}

void UB2UIMail::OpenAllMailUI
(
	const int32 ReceivedGold,
	const int32 ReceivedGem,
	const int32 ReceivedBladePoint,
	const int32 ReceivedSocialPoint,
	const int32 ReceivedDarkCrystalPoint,
	const int32 ReceivedFamePoint,
	const int32 ReceivedStageBoostTicket,
	const int32 ReceivedStageAutoClearTicket,
	const int32 ReceivedHotTimeGoldBuffTicket,
	const int32 ReceivedHotTimeExpBuffTicket,
	const int32 ReceivedShopTenLotteryTicket,
	const TArray<FB2MailLotteryItemInfoPtr>& ItemArray,
	const TArray<int64>& MailIdArray
)
{
	if (!VB_MailList.IsValid())
	{
		return;
	}

	auto RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
	if (RewardPopup != nullptr)
	{
		if (ReceivedBladePoint > 0)
			RewardPopup->AddSingleRewardItem(ERewardType::BladePoint, ReceivedBladePoint);
		if (ReceivedGem > 0)
			RewardPopup->AddSingleRewardItem(ERewardType::Gem, ReceivedGem);
		if (ReceivedGold > 0)
			RewardPopup->AddSingleRewardItem(ERewardType::Gold, ReceivedGold);
		if (ReceivedSocialPoint > 0)
			RewardPopup->AddSingleRewardItem(ERewardType::Social_Point, ReceivedSocialPoint);
		if (ReceivedDarkCrystalPoint > 0)
			RewardPopup->AddSingleRewardItem(ERewardType::Dark_Crystal, ReceivedDarkCrystalPoint);
		if (ReceivedFamePoint > 0)
			RewardPopup->AddSingleRewardItem(ERewardType::Fame_Point, ReceivedFamePoint);

		if (ReceivedStageBoostTicket > 0)
		{
			int32 ReceiveItemRefId = FItemRefIDHelper::GetGoodsID_StageBoostTicket();
			RewardPopup->AddSingleRewardItemByTemplateId(ReceiveItemRefId, ReceivedStageBoostTicket);
		}
		if (ReceivedStageAutoClearTicket > 0)
		{
			int32 ReceiveItemRefId = FItemRefIDHelper::GetGoodsID_StageAutoClearTicket();
			RewardPopup->AddSingleRewardItemByTemplateId(ReceiveItemRefId, ReceivedStageAutoClearTicket);
		}
		if (ReceivedHotTimeGoldBuffTicket > 0)
		{
			int32 ReceiveItemRefId = FItemRefIDHelper::GetGoodsID_GoldBuffTicket();
			RewardPopup->AddSingleRewardItemByTemplateId(ReceiveItemRefId, ReceivedHotTimeGoldBuffTicket);
		}
		if (ReceivedHotTimeExpBuffTicket > 0)
		{
			int32 ReceiveItemRefId = FItemRefIDHelper::GetGoodsID_ExpBuffTicket();
			RewardPopup->AddSingleRewardItemByTemplateId(ReceiveItemRefId, ReceivedHotTimeExpBuffTicket);
		}
		if (ReceivedShopTenLotteryTicket > 0)
		{
			int32 ReceiveItemRefId = FItemRefIDHelper::GetGoodsID_ShopTenLotteryTicket();
			RewardPopup->AddSingleRewardItemByTemplateId(ReceiveItemRefId, ReceivedShopTenLotteryTicket);
		}

		bool bOpenNickNameChange = false;

		for (auto ItemInfo : ItemArray)
		{
			BII_CHECK(ItemInfo->item.get());
			if (ItemInfo->item_amount > 0 && ItemInfo->item.get())
			{
				FB2Item AddItem;
				AddItem = ItemInfo->item;
				ClientDataStoreAddNewAcquiredItem(AddItem);
				RewardPopup->AddSingleRewardItem(ItemInfo->item_template_id, ItemInfo->item_amount);	

				if (FItemRefIDHelper::GetGoodsID_NicknameChangeTicket() == AddItem.ItemRefID)
					bOpenNickNameChange = true;
			}
		}
		// RewardPopup에서 ClientDataStore::AccountInfo에 보상 강제 적용

		if(bOpenNickNameChange)
			RewardPopup->SetOpenNickNameChangePopup();

		RewardPopup->ShowRewardPopup(true);
		RewardPopup->ShowMailDescription(false);
	}

	UpdateMailList(MailIdArray);

	if (MailIdArray.Num() == 0 && CurrentTabIndex == MailTabIndex::EMailTab_BladePoint)
	{
		//영혼검 카테고리에서는 메일ID가 넘어오지 않음 -> 변경될 여지 있음.
		VB_MailList->ClearChildren();
		MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.Empty();
	}

	UpdateMailList();
}

void UB2UIMail::SetTouch(bool InTouch)
{
	IsTouch = InTouch;

	if (BTN_ReceiveAll.IsValid())
	{
		BTN_ReceiveAll->SetIsEnabled(!InTouch && MailReceiveAllBtnState());
	}

	if (BTN_UpgradeStoneReceiveAll.IsValid())
	{
		BTN_UpgradeStoneReceiveAll->SetIsEnabled(!InTouch && CheckUpgradeStoneReceiveAllBTN());
	} // 확인 필요

	if (BTN_RareUpgradeStoneReceiveAll.IsValid())
	{
		BTN_RareUpgradeStoneReceiveAll->SetIsEnabled(!InTouch && CheckRareUpgradeStoneReceiveAllBTN());
	}
}

bool UB2UIMail::GetTouch()
{
	return IsTouch;
}

void UB2UIMail::OpenMailUI(int64 InMailId, const FB2ResponseOpenMailPtr& InMailInfo)
{
	if (!VB_MailList.IsValid())
	{
		return;
	}

	//열려있는 모든 선택팝업들을 닫는다.
	if (CreatedGeneralLotteryPopup) 
	{
		CreatedGeneralLotteryPopup->Close(); // 숨기기만 하고 생성된 거 파괴하는 건 아님.
	}
	if (CreatedSelectiveLotteryPopup) 
	{
		CreatedSelectiveLotteryPopup->Close();
	}
	if (CreatedSelectiveLotteryConsumeItemPopup) 
	{
		CreatedSelectiveLotteryConsumeItemPopup->Close();
	}
	if (CreatedAllSelectiveLotteryResultPopup)
	{
		CreatedAllSelectiveLotteryResultPopup->Close();
	}
	if (CreatedAllSelectiveLotteryCostumePopup)
	{
		CreatedAllSelectiveLotteryCostumePopup->Close();
	}
	// 나머지 둘은..?

	const int32 TabIndex = ConvertMailIndexToInt32(CurrentTabIndex);
	if (MailTabs.IsValidIndex(TabIndex))
	{
		TArray<TWeakObjectPtr<UB2UIMailContent>>& MailList = MailTabs[TabIndex].Mails;
		for (int32 MailIndex = 0; MailIndex < MailList.Num(); ++MailIndex)
		{
			if (MailList[MailIndex]->GetMailId() == InMailId)
			{
				MailList[MailIndex]->OpenedMail(InMailInfo);
				VB_MailList->RemoveChild(MailList[MailIndex].Get());
				MailList.RemoveAt(MailIndex);

				BladeIIGameImpl::GetRedDotManager().OnOpenedSingleMail(CurrentTabIndex);
				if (MailList.Num() == 0)
					BladeIIGameImpl::GetRedDotManager().OnOpenedAllMails(CurrentTabIndex);

				UpdateMailList();

				return;
			}
		}
	}
}

void UB2UIMail::OnClicked_GiftTabBTN()
{
	if (MailTabIndex::EMailTab_Gift == CurrentTabIndex)
		return;

	data_trader::Retailer::GetInstance().RequestGetMailList(static_cast<int32>(MailTabIndex::EMailTab_Gift));
	SetTabIndex(MailTabIndex::EMailTab_Gift);
}

void UB2UIMail::OnClicked_BladePointTabBTN()
{
	if (MailTabIndex::EMailTab_BladePoint == CurrentTabIndex)
		return;

	data_trader::Retailer::GetInstance().RequestGetMailList(static_cast<int32>(MailTabIndex::EMailTab_BladePoint));
	SetTabIndex(MailTabIndex::EMailTab_BladePoint);
}

void UB2UIMail::OnClicked_FriendshipTabBTN()
{
	if (MailTabIndex::EMailTab_Friendship == CurrentTabIndex)
		return;

	data_trader::Retailer::GetInstance().RequestGetMailList(static_cast<int32>(MailTabIndex::EMailTab_Friendship));
	SetTabIndex(MailTabIndex::EMailTab_Friendship);
}

void UB2UIMail::OnClicked_TicketTabBTN()
{
	if (MailTabIndex::EMailTab_Ticket == CurrentTabIndex)
		return;

	data_trader::Retailer::GetInstance().RequestGetMailList(static_cast<int32>(MailTabIndex::EMailTab_Ticket));
	SetTabIndex(MailTabIndex::EMailTab_Ticket);
}

void UB2UIMail::OnClicked_ReceiveAllBTN()
{
	MailReceiveState->ReceiveAllBTN(MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails, CurrentTabIndex);
}

void UB2UIMail::OnClicked_UpgradeStoneReceiveAllBTN()
{
	UB2UIUpgradeStoneAllReceive* StonePopup = UB2UIManager::GetInstance()->OpenUI<UB2UIUpgradeStoneAllReceive>(UIFName::UpgradeStoneAllReceive);
	StonePopup->SetUpgradeStoneMailInfo(MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails);
}

void UB2UIMail::OnClicked_RareUpgradeStoneReceiveAllBTN()
{
	UB2UIUpgradeStoneAllReceive* StonePopup = UB2UIManager::GetInstance()->OpenUI<UB2UIUpgradeStoneAllReceive>(UIFName::UpgradeStoneAllReceive);
	StonePopup->SetRareUpgradeStoneMailInfo(MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails);
}

bool UB2UIMail::RedDotCondition_Gift()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Mail(MailTabIndex::EMailTab_Gift);
}

bool UB2UIMail::RedDotCondition_BladePoint()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Mail(MailTabIndex::EMailTab_BladePoint);
}

bool UB2UIMail::RedDotCondition_FriendShip()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Mail(MailTabIndex::EMailTab_Friendship);
}

bool UB2UIMail::RedDotCondition_Ticket()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Mail(MailTabIndex::EMailTab_Ticket);
}

void UB2UIMail::SetMailList(const FB2MailList& MailList)
{
	//if (VB_MailList.IsValid())
	//{
	//	VB_MailList->ClearChildren();
	//}

	//MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.Empty();

	//this->upgradestone_mail_totalcount = 0;
	//this->rare_upgradestone_mail_totalcount = 0;
	//
	//auto MailInfo = GET_TUPLE_DATA(FB2ResponseGetMailList::mail_infos_index, MailList);
	//for (auto MailInfoElem : MailInfo)
	//{
	//	check(static_cast<MailTabIndex>(MailInfoElem->mail_category) == CurrentTabIndex);

	//	auto* CreatedWidget = CreateWidget<UB2UIMailContent>(GetOwningPlayer(), MailContentClass);
	//	if (CreatedWidget != NULL)
	//	{
	//		auto* AddedSlot = Cast<UVerticalBoxSlot>(VB_MailList->AddChild(CreatedWidget));
	//		check(AddedSlot);
	//		CreatedWidget->Init();
	//		MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.Add(CreatedWidget);

	//		CreatedWidget->SetMailBox(this);
	//		CreatedWidget->SetMailContent(MailInfoElem);
	//	}
	//}

	//if (TB_EmptySlot.IsValid())
	//	TB_EmptySlot->SetVisibility(VB_MailList->GetChildrenCount() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);


	//UpdateMailList();
}

void UB2UIMail::UpdateMailList()
{
	if (TB_CurrentMail.IsValid())
	{
		TB_CurrentMail->SetText(FText::AsNumber(MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.Num()));
	}

	if (BTN_ReceiveAll.IsValid())
	{
		BTN_ReceiveAll->SetVisibility(ESlateVisibility::Visible);
		BTN_ReceiveAll->SetIsEnabled(MailReceiveAllBtnState());
	}

	if (MailTabIndex::EMailTab_Ticket == CurrentTabIndex)
	{
		if (BTN_UpgradeStoneReceiveAll.IsValid())
		{
			BTN_UpgradeStoneReceiveAll->SetIsEnabled(CheckUpgradeStoneReceiveAllBTN());
		}
		
		if (BTN_RareUpgradeStoneReceiveAll.IsValid())
		{
			BTN_RareUpgradeStoneReceiveAll->SetIsEnabled(CheckRareUpgradeStoneReceiveAllBTN());
		}
	}// 확인 필요

	DoMarkRedDot();
	if (UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header))
		HeaderUI->DoMarkRedDot();
}

void UB2UIMail::UpdateMailList(const TArray<int64>& InMailIdArray)
{
	// 팝업UI <-
	for (int32 Idx = MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.Num() - 1; Idx > -1; --Idx)
	{
		int32 FoundedIndex = INDEX_NONE;
		if (InMailIdArray.Find(MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails[Idx]->GetMailId(), FoundedIndex))
		{
			VB_MailList->RemoveChild(MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails[Idx].Get());
			MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.RemoveAt(Idx);
		}
	}

	// data syncing
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	if (InMailIdArray.Num() > 0)
	{
		auto& CharItemContainers = ClientDataStore.GetLocalCharacterData().GetUserAllItems().CharacterItemContainers;
		for (auto& CharItemContainer : CharItemContainers)
		{
			CharItemContainer.MakeDirty();
		}
	}

	BladeIIGameImpl::GetRedDotManager().OnOpenedAllMails(CurrentTabIndex, InMailIdArray.Num());
}

void UB2UIMail::OpenMail(int64 MailId, const FB2ResponseOpenMailPtr& MailInfo)
{
	MailReceiveState->OpenMail(MailId, MailInfo);
}

void UB2UIMail::OpenAllMail(const FB2OpenAllMail& OpenedAllMail)
{
	MailReceiveState->OpenAllMail(OpenedAllMail);
}

void UB2UIMail::OpenPopupReceiveOneMail(MailGiftType GiftType, int32 Amount)
{
	ERewardType RewardType = MailGiftToRewardType(GiftType);

	auto RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
	if (RewardPopup != nullptr)
	{
		RewardPopup->AddSingleRewardItem(RewardType, Amount);

		// RewardPopup에서 ClientDataStore::AccountInfo에 보상 강제 적용
		
		// 닉네임 변경권도 타입이 생긴다면 if 문 걸어서..
		/*RewardPopup->SetOpenNickNameChangePopup();*/


		RewardPopup->ShowRewardPopup(true);
		RewardPopup->ShowMailDescription(false);
	}
}

void UB2UIMail::OpenPopupReceiveOneMail(int32 ItemTemplateId, int32 Amount)
{
	auto RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
	if (RewardPopup == nullptr)
	{
		return;
	}

	RewardPopup->AddSingleRewardItemByTemplateId(ItemTemplateId, Amount);

	if (FItemRefIDHelper::GetGoodsID_NicknameChangeTicket() == ItemTemplateId)
		RewardPopup->SetOpenNickNameChangePopup();

	// RewardPopup에서 ClientDataStore::AccountInfo에 보상 강제 적용
	RewardPopup->ShowRewardPopup(true);
	RewardPopup->ShowMailDescription(false);
}

//
// 아래.. 하위 UIP 들의 dynamic on-demand 로딩 및 생성 코드들 이어짐.
//

void UB2UIMail::OpenGeneralLottery(int64 MailId, const FText& TitleText)
{
	ConditionalCreateGeneralLotteryPopup(); // 동적 로딩 및 생성
	BII_CHECK(CreatedGeneralLotteryPopup);

	if (CreatedGeneralLotteryPopup)
	{
		CreatedGeneralLotteryPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CreatedGeneralLotteryPopup->SetTitleText(TitleText);
	}

	MailIdToOpen = MailId;
}

void UB2UIMail::OpenAllSelectiveLottery(int64 MailId, const TArray<int32>& SelectableItemRefIds, const FText& TitleText)
{
	ConditionalCreateAllSelectiveLotteryPopup(); // 동적 로딩 및 생성
	BII_CHECK(CreatedAllSelectiveLotteryResultPopup);

	if (CreatedAllSelectiveLotteryResultPopup)
	{
		CreatedAllSelectiveLotteryResultPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CreatedAllSelectiveLotteryResultPopup->SetItemRefIds(SelectableItemRefIds);
		CreatedAllSelectiveLotteryResultPopup->AddBackWidget();

		CreatedAllSelectiveLotteryResultPopup->SetTitleText(TitleText);
	}
	if (CreatedAllSelectiveLotteryResultPopup)
	{
		CreatedAllSelectiveLotteryResultPopup->InitLotterySetting();
	}

	MailIdToOpen = MailId;
}

void UB2UIMail::OpenPrefixSelectiveLottery(int64 MailId, const int32 SurpassLevel, const TArray<int32>& SelectableItemRefIds, const FText& TitleText)
{
	ConditionalCreateAllSelectiveLotteryPopup(); // 동적 로딩 및 생성
	BII_CHECK(CreatedAllSelectiveLotteryResultPopup);

	if (CreatedAllSelectiveLotteryResultPopup)
	{
		CreatedAllSelectiveLotteryResultPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CreatedAllSelectiveLotteryResultPopup->SetItemRefIds_AutoSort(SelectableItemRefIds);
		CreatedAllSelectiveLotteryResultPopup->AddBackWidget();

		CreatedAllSelectiveLotteryResultPopup->SetTitleText(TitleText);
	}
	if (CreatedAllSelectiveLotteryResultPopup)
	{
		CreatedAllSelectiveLotteryResultPopup->InitLotterySetting();
		CreatedAllSelectiveLotteryResultPopup->SetLotterySurpassLevel(SurpassLevel);
		CreatedAllSelectiveLotteryResultPopup->UpdateItemSurpassLevel();
	}

	MailIdToOpen = MailId;
}

void UB2UIMail::OpenPrefixSelectiveLotteryCostume(int64 MailId, const int32 SurpassLevel, const TArray<int32>& SelectableItemRefIds, const FText& TitleText)
{
	ConditionalCreateAllSelectiveLotteryCostumePopup(); // 동적 로딩 및 생성
	BII_CHECK(CreatedAllSelectiveLotteryCostumePopup);

	if (CreatedAllSelectiveLotteryCostumePopup)
	{
		CreatedAllSelectiveLotteryCostumePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CreatedAllSelectiveLotteryCostumePopup->SetItemRefIds_AutoCostumeSort(SelectableItemRefIds);
		CreatedAllSelectiveLotteryCostumePopup->AddBackWidget();

		CreatedAllSelectiveLotteryCostumePopup->SetTitleText(TitleText);
	}
	if (CreatedAllSelectiveLotteryCostumePopup)
	{
		CreatedAllSelectiveLotteryCostumePopup->InitLotterySetting();
	}

	MailIdToOpen = MailId;
}

void UB2UIMail::OpenSelectiveLottery(int64 MailId, const TArray<int32>& SelectableItemRefIds, const FText& TitleText)
{
	ConditionalCreateSelectiveLotteryPopup(); // 동적 로딩 및 생성
	BII_CHECK(CreatedSelectiveLotteryPopup);

	if (CreatedSelectiveLotteryPopup)
	{
		CreatedSelectiveLotteryPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CreatedSelectiveLotteryPopup->SetItemRefIds(SelectableItemRefIds);
		CreatedSelectiveLotteryPopup->AddBackWidget();

		CreatedSelectiveLotteryPopup->SetTitleText(TitleText);
	}

	MailIdToOpen = MailId;
}

void UB2UIMail::OpenSelectiveLotteryConsumeItem(int64 MailId, const TArray<int32>& SelectableItemRefIds, const TArray<int32>& SelectableItemNums)
{
	ConditionalCreateSelectiveLotteryConsumeItemPopup(); // 동적 로딩 및 생성
	BII_CHECK(CreatedSelectiveLotteryConsumeItemPopup);

	if (CreatedSelectiveLotteryConsumeItemPopup)
	{
		CreatedSelectiveLotteryConsumeItemPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CreatedSelectiveLotteryConsumeItemPopup->SetItemData(SelectableItemRefIds, SelectableItemNums);
	}

	MailIdToOpen = MailId;
}

void UB2UIMail::OpenPopupReceiveSelectiveLotteryConsumeItemResult(int32 ItemRefId, int32 Num)
{
	ConditionalCreateSelectiveLotteryConsumeItemResultPopup(); // 동적 로딩 및 생성
	BII_CHECK(CreatedSelectiveLotteryConsumeItemResultPopup);

	if (CreatedSelectiveLotteryConsumeItemResultPopup)
	{
		CreatedSelectiveLotteryConsumeItemResultPopup->OpenPopup(ItemRefId, Num);
	}
}

void UB2UIMail::OpenPopupReceiveSelectiveLotteryResult(const FB2Item& ResultItem)
{
	ConditionalCreateSelectiveLotteryResultPopup(); // 동적 로딩 및 생성
	BII_CHECK(CreatedSelectiveLotteryResultPopup);

	if (CreatedSelectiveLotteryResultPopup)
	{
		CreatedSelectiveLotteryResultPopup->OpenPopup(ResultItem);
	}
}

template<class WidgetClassT>
static WidgetClassT* CreateDynamicPopupMailUIPCommon(TSoftClassPtr<WidgetClassT>& InCreateWidgetClass, UB2UIMail* InOwnerUserWidget, UCanvasPanel* CreateParentPanel)
{
	//if (CreateParentPanel)
	//{
	//	WidgetClassT* NewCreated = DynLoadAndCreateInCanvasPanelFull<WidgetClassT>(InCreateWidgetClass, InOwnerUserWidget, CreateParentPanel);
	//	if (NewCreated)
	//	{
	//		NewCreated->Init();
	//		return NewCreated;
	//	}
	//}
	return nullptr;
}
void UB2UIMail::ConditionalCreateGeneralLotteryPopup()
{
	if (!CreatedGeneralLotteryPopup && X_CP_UIPCreateBase.IsValid())
	{
		CreatedGeneralLotteryPopup = CreateDynamicPopupMailUIPCommon<UB2UIGeneralLotteryPopup>(UIP_GeneralLotteryPopupClass, this, X_CP_UIPCreateBase.Get());
		if (CreatedGeneralLotteryPopup)
		{
			CreatedGeneralLotteryPopup->SetMailBox(this);
		}
	}
}

void UB2UIMail::ConditionalCreateAllSelectiveLotteryPopup()
{
	if (!CreatedAllSelectiveLotteryResultPopup && X_CP_UIPCreateBase.IsValid())
	{
		CreatedAllSelectiveLotteryResultPopup = CreateDynamicPopupMailUIPCommon<UB2UIAllSelectiveLotteryPopup>(UIP_AllSelectiveLotteryPopupClass, this, X_CP_UIPCreateBase.Get());
		if (CreatedAllSelectiveLotteryResultPopup)
		{
			CreatedAllSelectiveLotteryResultPopup->SetMailBox(this);
		}
	}
}

void UB2UIMail::ConditionalCreateAllSelectiveLotteryCostumePopup()
{
	if (!CreatedAllSelectiveLotteryCostumePopup && X_CP_UIPCreateBase.IsValid())
	{
		CreatedAllSelectiveLotteryCostumePopup = CreateDynamicPopupMailUIPCommon<UB2UIAllSelectiveLotteryCostumePopup>(UIP_AllSelectiveLotteryCostumePopupClass, this, X_CP_UIPCreateBase.Get());
		if (CreatedAllSelectiveLotteryCostumePopup)
		{
			CreatedAllSelectiveLotteryCostumePopup->SetMailBox(this);
		}
	}
}

void UB2UIMail::ConditionalCreateSelectiveLotteryPopup()
{
	if (!CreatedSelectiveLotteryPopup && X_CP_UIPCreateBase.IsValid())
	{
		CreatedSelectiveLotteryPopup = CreateDynamicPopupMailUIPCommon<UB2UISelectiveLotteryPopup>(UIP_SelectiveLotteryPopupClass, this, X_CP_UIPCreateBase.Get());
		if (CreatedSelectiveLotteryPopup)
		{
			CreatedSelectiveLotteryPopup->SetMailBox(this);
		}
	}
}
void UB2UIMail::ConditionalCreateSelectiveLotteryConsumeItemPopup()
{
	if (!CreatedSelectiveLotteryConsumeItemPopup && X_CP_UIPCreateBase.IsValid())
	{
		CreatedSelectiveLotteryConsumeItemPopup = CreateDynamicPopupMailUIPCommon<UB2UISelectiveLotteryConsumItemPopup>(UIP_SelectiveLotteryConsumeItemPopupClass, this, X_CP_UIPCreateBase.Get());
		if (CreatedSelectiveLotteryConsumeItemPopup)
		{
			CreatedSelectiveLotteryConsumeItemPopup->SetMailBox(this);
		}
	}
}
void UB2UIMail::ConditionalCreateSelectiveLotteryConsumeItemResultPopup()
{
	if (!CreatedSelectiveLotteryConsumeItemResultPopup && X_CP_UIPCreateBase.IsValid())
	{
		CreatedSelectiveLotteryConsumeItemResultPopup = CreateDynamicPopupMailUIPCommon<UB2UISelectiveLotteryConsumeItemResultPopup>(UIP_SelectiveLotteryConsumeItemResultClass, this, X_CP_UIPCreateBase.Get());
	}
}
void UB2UIMail::ConditionalCreateSelectiveLotteryResultPopup()
{
	if (!CreatedSelectiveLotteryResultPopup && X_CP_UIPCreateBase.IsValid())
	{
		CreatedSelectiveLotteryResultPopup = CreateDynamicPopupMailUIPCommon<UB2UIMailSelectiveLotteryResult>(UIP_SelectiveLotteryResultPopupClass, this, X_CP_UIPCreateBase.Get());
	}
}
void UB2UIMail::DestroyGeneralLotteryPopup()
{
	if (CreatedGeneralLotteryPopup)
	{
		CreatedGeneralLotteryPopup->DestroySelf(UB2UIManager::GetInstance());
	}
}
void UB2UIMail::DestroySelectiveLotteryPopup()
{
	if (CreatedSelectiveLotteryPopup)
	{
		CreatedSelectiveLotteryPopup->DestroySelf(UB2UIManager::GetInstance());
	}
}

void UB2UIMail::DestroyAllSelectiveLotteryPopup()
{
	if (CreatedAllSelectiveLotteryResultPopup)
	{
		CreatedAllSelectiveLotteryResultPopup->DestroySelf(UB2UIManager::GetInstance());
	}
}

void UB2UIMail::DestroyAllSelectiveLotteryCostumePopup()
{
	if (CreatedAllSelectiveLotteryCostumePopup)
	{
		CreatedAllSelectiveLotteryCostumePopup->DestroySelf(UB2UIManager::GetInstance());
	}
}

void UB2UIMail::DestroySelectiveLotteryConsumeItemPopup()
{
	if (CreatedSelectiveLotteryConsumeItemPopup)
	{
		CreatedSelectiveLotteryConsumeItemPopup->DestroySelf(UB2UIManager::GetInstance());
	}
}
void UB2UIMail::DestroySelectiveLotteryConsumeItemResultPopup()
{
	if (CreatedSelectiveLotteryConsumeItemResultPopup)
	{
		CreatedSelectiveLotteryConsumeItemResultPopup->DestroySelf(UB2UIManager::GetInstance());
	}
}
void UB2UIMail::DestroySelectiveLotteryResultPopup()
{
	if (CreatedSelectiveLotteryResultPopup)
	{
		CreatedSelectiveLotteryResultPopup->DestroySelf(UB2UIManager::GetInstance());
	}
}

void UB2UIMail::SelectedCharacterGeneralLottery(EPCClass SelectedCharacter)
{
	data_trader::Retailer::GetInstance().RequestOpenMail(MailIdToOpen, CliToSvrPCClassType(SelectedCharacter), 0, 0);
}

void UB2UIMail::SelectedItemSelectiveLottery(int32 SelectedItemRefId)
{
	data_trader::Retailer::GetInstance().RequestOpenMail(MailIdToOpen, 0, SelectedItemRefId, 0);
}

void UB2UIMail::ClientDataStoreAddNewAcquiredItem(FB2Item AddItem)
{
	// B2UIMail에서 ClientDataStore::AccountInfo에 보상 강제 적용
	TArray<FB2Item> AddItemList;
	AddItemList.Add(AddItem);
	BladeIIGameImpl::GetClientDataStore().AddNewAcquiredItems(AddItemList);
}

bool UB2UIMail::HasAnyMail()
{
	return HasAnyMail(MailTabIndex::EMailTab_Gift) ||
		HasAnyMail(MailTabIndex::EMailTab_BladePoint) ||
		HasAnyMail(MailTabIndex::EMailTab_Friendship) ||
		HasAnyMail(MailTabIndex::EMailTab_Ticket);
}

bool UB2UIMail::HasAnyMail(MailTabIndex TabIndex)
{
	const int32 Index = ConvertMailIndexToInt32(TabIndex);
	if (MailTabs.IsValidIndex(Index))
	{
		return MailTabs[Index].Mails.Num() > 0 ? true : false;
	}

	return false;
}

void UB2UIMail::RequestCurrentTabMailList()
{
	if (CurrentTabIndex == MailTabIndex::EMailTab_Max)
		return;

	data_trader::Retailer::GetInstance().RequestGetMailList(static_cast<int32>(CurrentTabIndex));
}

void UB2UIMail::SetMailReceiveState(MailTabIndex InIndex)
{
	DeleteMailReceiveState();

	switch (InIndex)
	{
	case MailTabIndex::EMailTab_Gift:
		MailReceiveState = new B2UIMailReceiveGiftState();
		break;
	case MailTabIndex::EMailTab_BladePoint:
	case MailTabIndex::EMailTab_Friendship:
		MailReceiveState = new B2UIMailReceiveBasicState();
		break;
	case MailTabIndex::EMailTab_Ticket:
		MailReceiveState = new B2UIMailReceiveTicketState();
		break;
	default:
		MailReceiveState = new B2UIMailReceiveBasicState();
		break;
	}
}

void UB2UIMail::DeleteMailReceiveState()
{
	if (MailReceiveState)
	{
		delete MailReceiveState;
		MailReceiveState = nullptr;
	}
}

bool UB2UIMail::MailReceiveAllBtnState()
{
	if (MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.Num() > 0)
	{
		if (CurrentTabIndex != MailTabIndex::EMailTab_Ticket)
			return true;

		for (int32 Idx = MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.Num() - 1; Idx > -1; --Idx)
		{
			if (MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails[Idx]->GetGiftType() == MailGiftType::LotteryTicket ||
				MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails[Idx]->GetGiftType() == MailGiftType::Fixed_Grade_Lottey)
			{
				return true;
			}
		}
	}

	return false;
}

bool UB2UIMail::CheckUpgradeStoneReceiveAllBTN()
{
	if (MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.Num() > 0)
	{
		for (int32 Idx = MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.Num() - 1; Idx > -1; --Idx)
		{
			if (MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails[Idx]->GetGiftType() == MailGiftType::Selective_Item_Lottery &&
				MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails[Idx]->GetItemRefId() != 8			// 하드 코딩 고급 승급석 뽑기권 예외
				)
			{
				return true;
			}
		}
	}

	return false;
}
bool UB2UIMail::CheckRareUpgradeStoneReceiveAllBTN()
{
	if (MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.Num() > 0)
	{
		for (int32 Idx = MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails.Num() - 1; Idx > -1; --Idx)
		{
			if (MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails[Idx]->GetGiftType() == MailGiftType::Selective_Item_Lottery &&
				MailTabs[ConvertMailIndexToInt32(CurrentTabIndex)].Mails[Idx]->GetItemRefId() == 8)			// 하드 코딩 고급 승급석 뽑기권 예외
			{
				return true;
			}
		}
	}

	return false;
}

