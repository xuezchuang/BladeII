#include "B2UISummonItem.h"
#include "Event.h"
#include "Retailer.h"

#include "B2UIDocHelper.h"
#include "TutorialManager.h"
#include "B2LobbyInventory.h"
#include "B2PCClassInfoBox.h"
#include "B2UIManager.h"
#include "B2UIBackWidgetBase.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2UIRewardEventSet.h"
#include "B2UIHeader.h"
#include "B2AndroidBackManager.h"
#include "B2UIUtil.h"
#include "B2GameInstance.h"
#include "B2PlatformAPI.h"

#include "BladeIIGameImpl.h"
UB2UISummonItem::UB2UISummonItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CreatedUIP_CharSelectPopup = nullptr;
	CreatedUIP_MileageRewardPopup = nullptr;
	CreatedUIP_MileageGuidePopup = nullptr;
	MRP_RewardIconTemplate = nullptr;

	CostType = ESummonItemCost::Gem;
	InCost = 0;
	CurrentTabMenu = ESummonItemTabMenu::Costume;
	SummonCount = 0;
	SelectedCharacterClass = EPCClass::EPC_End;
	bUsedTenLotteryTicket = false;
}

ESummonItemDifficulty UB2UISummonItem::GetCurrentStageDifficulty()
{
	EStageDifficulty StageDifficulty = BladeIIGameImpl::GetStageDataStore().GetMaxOpenedStageDifficulty();
	return static_cast<ESummonItemDifficulty>((static_cast<int32>(StageDifficulty) - static_cast<int32>(EStageDifficulty::ESD_Normal)));
}

void UB2UISummonItem::SubscribeEvent()
{
	UnsubscribeEvent();

	Issues.Add(SelectCharacterToSummonItemClass<int32, bool>::GetInstance().Subscribe2(
		[this](int32 SlotHashKey, bool UseTenTicket)
	{
		this->OpenSelectCharacterPopupWindow(SlotHashKey, UseTenTicket);
	}
	));

	Issues.Add(DeliveryRewardShopMileageClass<FB2ResponseRewardShopMileagePtr>::GetInstance().Subscribe2(
		[this](FB2ResponseRewardShopMileagePtr ResponseRewardShopMileage)
	{
		this->OnResponseRewardShopMileage(ResponseRewardShopMileage);
	}
	));
}

void UB2UISummonItem::UnsubscribeEvent()
{
	Issues.Empty();
}

void UB2UISummonItem::Init()
{
	SubscribeEvent();

	Super::Init();

	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		if (MileageChangedHandler.IsValid())
		{
			DocStore->OnMileageChanged.Remove(MileageChangedHandler);
		}
		MileageChangedHandler.Reset();
		MileageChangedHandler = DocStore->OnMileageChanged.AddUObject(this, &UB2UISummonItem::OnMileageChanged);

		SetTab();

		DocStore->SetSummonItemTabToOpen(ESummonItemTabMenu::GemBox);
	}

	if (UIP_StoreMileage.IsValid())
	{
		UIP_StoreMileage->Init();
		UIP_StoreMileage->SetMileageData(EStoreMileageType::SummonItem);
	}

	if (OV_DoumiGirl.IsValid())
		OV_DoumiGirl->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UB2UISummonItem::DestroySelf(class UB2UIManager* InUIManager)
{
	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		if (MileageChangedHandler.IsValid())
		{
			DocStore->OnMileageChanged.Remove(MileageChangedHandler);
		}
		MileageChangedHandler.Reset();
	}

	UnsubscribeEvent();

	CloseSelectCharacterPopupWindow();
	CloseMileageGuidePopup();

	DestroyCharSelectPopup();
	DestroyMileageRewardPopup();
	DestroyMileageGuidePopup();

	Super::DestroySelf(InUIManager);
}

void UB2UISummonItem::CacheAssets()
{
	SummonItemSlotItems.AddDefaulted(static_cast<int32>(ESummonItemSlotIndex::Max));

	GET_SLOT_BYNAME(UB2UISummonItemSlotItem, SummonItemSlotItems[ESummonItemSlotIndex::SummonItem_Gem], UIP_SummonSlotItem_Gem);
	GET_SLOT_BYNAME(UB2UISummonItemSlotItem, SummonItemSlotItems[ESummonItemSlotIndex::SummonItem_FriendShip], UIP_SummonSlotItem_FriendShip);

	for (int i = 0; i < SummonItemSlotItems.Num(); ++i)
	{
		if (SummonItemSlotItems[i].IsValid())
		{
			SummonItemSlotItems[i]->Init();
		}
	}

	GET_SLOT(UB2UIStoreMileage, UIP_StoreMileage);
	if (UIP_StoreMileage.IsValid())
	{
		UIP_StoreMileage->Init();
		UIP_StoreMileage->SetMileageData(EStoreMileageType::SummonItem);
	}

	GET_SLOT(UB2Button, BTN_Chance);
	GET_SLOT(UTextBlock, TB_Chance);

	GET_SLOT(UOverlay, OV_DoumiGirl);
	GET_SLOT(UTextBlock, TB_DoumiGirl);
	
	// 동적 생성하는 팝업들은 CanvasPanel 만 얻어두고 실제 생성은 이후 on-demand 로
	GET_SLOT(UCanvasPanel, X_CP_UIP_CharSelectPopupPanel);
	GET_SLOT(UCanvasPanel, X_CP_UIP_MileageRewardPopupPanel);
	GET_SLOT(UCanvasPanel, X_CP_UIP_MileageGuidePopupPanel);

	GET_SLOT(UB2Button, BTN_MileageGuide);

	CharacterButtonOnArr.SetNum(static_cast<int32>(EPCClass::EPC_End));
	CharacterButtonOffArr.SetNum(static_cast<int32>(EPCClass::EPC_End));

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
}

void UB2UISummonItem::UpdateStaticText()
{
	if (TB_Chance.IsValid())
		TB_Chance->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem_Chance")));

	if (TB_DoumiGirl.IsValid())
		TB_DoumiGirl->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_EquipStore_AddItemDesc")));
}

void UB2UISummonItem::BindDelegates()
{
	if (UIP_StoreMileage.IsValid())
	{
		auto* BClassRewardBoxBTN = UIP_StoreMileage->GetBoxButtonPtr(EUIStoreMileageGrade::BClass);
		if (BClassRewardBoxBTN)
		{
			BClassRewardBoxBTN->OnClicked.RemoveAll(this);
			BClassRewardBoxBTN->OnClicked.AddDynamic(this, &UB2UISummonItem::SetOpenRewardPopupBClass);
		}

		auto* AClassRewardBoxBTN = UIP_StoreMileage->GetBoxButtonPtr(EUIStoreMileageGrade::AClass);
		if (AClassRewardBoxBTN)
		{
			AClassRewardBoxBTN->OnClicked.RemoveAll(this);
			AClassRewardBoxBTN->OnClicked.AddDynamic(this, &UB2UISummonItem::SetOpenRewardPopupAClass);
		}

		auto* SClassRewardBoxBTN = UIP_StoreMileage->GetBoxButtonPtr(EUIStoreMileageGrade::SClass);
		if (SClassRewardBoxBTN)
		{
			SClassRewardBoxBTN->OnClicked.RemoveAll(this);
			SClassRewardBoxBTN->OnClicked.AddDynamic(this, &UB2UISummonItem::SetOpenRewardPopupSClass);
		}
	}

	BIND_CLICK_FUNC_TO_BTN(BTN_MileageGuide, &UB2UISummonItem::OpenMileageGuidePopup);
	BIND_CLICK_FUNC_TO_BTN(BTN_Chance, &UB2UISummonItem::OnClickChance);
}

void UB2UISummonItem::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem")));
}

void UB2UISummonItem::SetTab()
{
	for (int32 SlotIdx = 0; SlotIdx < ESummonItemSlotIndex::Max; SlotIdx++)		// SlotIdx = UIP Type
	{
		if (SummonItemSlotItems[SlotIdx].IsValid())
		{
			SummonItemSlotItems[SlotIdx]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SummonItemSlotItems[SlotIdx]->SetSummonItemSlot(static_cast<ESummonItemCost>(SlotIdx));

			// Grade
			auto& GradeRangeData = BladeIIGameImpl::GetClientDataStore().GetStoreSummonItemGradeRangeData();
			auto* GradeRange = GradeRangeData.Find(FSummonItemGradeRange::GetHashKey(SlotIdx));
			if (GradeRange)
			{
				SummonItemSlotItems[SlotIdx]->SetGradeRange(GradeRange->MinGrade, GradeRange->MaxGrade);
			}

			for (int32 BtnIdx = 0; BtnIdx < ESummonButtonType::Max; BtnIdx++)	// BtnIdx = Cost Type
			{
				int32 HashKey = FSummonItemSlotData::GetHashKey(SlotIdx, BtnIdx % 2 == 0, false);
				auto* Data = BladeIIGameImpl::GetClientDataStore().GetSummonItemData(HashKey);
				if (Data)
				{
					// Button
					ESummonButtonType ButtonType = Data->IsMultiple() ? ESummonButtonType::SummonButtonType_Ten : ESummonButtonType::SummonButtonType_One;
					
					// 10회 뽑기권 사용가능한지 Check
					bool bIsTenLotteryTicket = (Data->GetCostType() == int32(ESummonItemCost::Gem) && Data->IsMultiple() && BladeIIGameImpl::GetClientDataStore().GetShopTenLotteryTicket() > 0);
							
					// Lottery Sale
					const FSummonItemLotterySale* LotterySale = BladeIIGameImpl::GetClientDataStore().GetSummonItemLotterySaleData(HashKey);
					bool bIsInclude = false;
					if (LotterySale)
					{
						bIsInclude = UB2GameInstance::IncludeTimeNow(LotterySale->StartTime, LotterySale->EndTime);
					}

					SummonItemSlotItems[SlotIdx]->SetSummonItem(
						ButtonType, 
						HashKey, 
						Data->GetNextFreeTimeLeftSeconds());

					SummonItemSlotItems[SlotIdx]->SetSummonButtonItem(
						ButtonType, 
						static_cast<ESummonItemCost>(SlotIdx), Data->GetCost(), 
						bIsInclude ? Data->GetCalculatedCost() : 0, 
						Data->GetBuyCount(),
						Data->GetLimitdBuyCount(), 
						bIsTenLotteryTicket);
					
					// Sale Mark
					bool bIsTenCostGem = Data->IsMultiple() && Data->GetCostType() == int32(ESummonItemCost::Gem) ? true : false;
					if (bIsTenCostGem && !bIsTenLotteryTicket)
					{
						SummonItemSlotItems[SlotIdx]->SetLotterySale(ButtonType);
					}
					
					/*
					const FSummonItemLotterySale* LotterySale = BladeIIGameImpl::GetClientDataStore().GetSummonItemLotterySaleData(HashKey);
					if (LotterySale)
					{
						// Lottery Sale
						if (LotterySale->DiscountRate > 0)
							SummonItemSlotItems[SlotIdx]->SetLotterySale(ButtonType, LotterySale->DiscountRate);
					}
					*/
				}
			}

			SummonItemSlotItems[SlotIdx]->StartFreeLeftTime(true);
		}
	}

	DoMarkRedDot();
}

void UB2UISummonItem::OpenSelectCharacterPopupWindow(int32 SlotHashKey, bool UseTenTicket)
{
	bUsedTenLotteryTicket = UseTenTicket;
	auto StoreDoc = UB2UIDocHelper::GetDocStore();

	if (StoreDoc)
		StoreDoc->SetSelectBoxHashKey(SlotHashKey);

	SelectedSlotHashKey = SlotHashKey;

	ConditionalCreateCharSelectPopup();
	if (CreatedUIP_CharSelectPopup)
		CreatedUIP_CharSelectPopup->OpenPopup();

	auto* Data = BladeIIGameImpl::GetClientDataStore().GetSummonItemData(SelectedSlotHashKey);
	BII_CHECK(Data);

	if (Data)
	{
		SummonCount = Data->IsMultiple() ? 10 : 1;
		CostType = static_cast<ESummonItemCost>(Data->GetCostType());
		InCost = Data->GetCost();
		InCalculatedCost = Data->GetCalculatedCost();

		FText TitleText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, static_cast<ESummonItemCost>(Data->GetCostType()) == ESummonItemCost::Gem ?
			TEXT("Store_SummonItem_SummonGem") : TEXT("Store_SummonItem_SummonFriendShip")), FText::AsNumber(SummonCount));

		UpdateSelectCharacterPopupWindowTitle(TitleText);
	}

	UpdateSelectCharacterPopupWindow();
	SetForcedBlockHeader(true);
}

void UB2UISummonItem::CloseSelectCharacterPopupWindow()
{
	if (CreatedUIP_CharSelectPopup)
	{
		CreatedUIP_CharSelectPopup->ClosePopup();
		SetButtonLineVisible(EPCClass::EPC_End);
	}

	if (BTN_CharSelectConfirm.IsValid())
		BTN_CharSelectConfirm->SetIsEnabled(false);

	SetForcedBlockHeader(false);
}

void UB2UISummonItem::OnClickSelectConfirm()
{
	if (BTN_CharSelectConfirm.IsValid())
		BTN_CharSelectConfirm->SetIsEnabled(false);

	SummonItem();
}

void UB2UISummonItem::SelectedGladiator()
{
	//SetCharSelectPopupButtonState(ESlateVisibility::HitTestInvisible);
	SelectedCharacterClass = EPCClass::EPC_Gladiator;
	//SummonItem();
	SetButtonLineVisible(EPCClass::EPC_Gladiator);
	if (BTN_CharSelectConfirm.IsValid())
		BTN_CharSelectConfirm->SetIsEnabled(true);
}

void UB2UISummonItem::SelectedAssassin()
{
	//SetCharSelectPopupButtonState(ESlateVisibility::HitTestInvisible);
	SelectedCharacterClass = EPCClass::EPC_Assassin;
	//SummonItem();
	SetButtonLineVisible(EPCClass::EPC_Assassin);
	if (BTN_CharSelectConfirm.IsValid())
		BTN_CharSelectConfirm->SetIsEnabled(true);
}

void UB2UISummonItem::SelectedWizard()
{
	//SetCharSelectPopupButtonState(ESlateVisibility::HitTestInvisible);
	SelectedCharacterClass = EPCClass::EPC_Wizard;
	SetButtonLineVisible(EPCClass::EPC_Wizard);
	//SummonItem();

	if (BTN_CharSelectConfirm.IsValid())
		BTN_CharSelectConfirm->SetIsEnabled(true);
}

void UB2UISummonItem::SelectedFighter()
{
	//SetCharSelectPopupButtonState(ESlateVisibility::HitTestInvisible);
	SelectedCharacterClass = EPCClass::EPC_Fighter;
	SetButtonLineVisible(EPCClass::EPC_Fighter);
	//SummonItem();

	if (BTN_CharSelectConfirm.IsValid())
		BTN_CharSelectConfirm->SetIsEnabled(true);
}

void UB2UISummonItem::UpdateSelectCharacterPopupWindow()
{
	// 이게 생성된 상태에서 불려야 하는 거
	checkSlow(CreatedUIP_CharSelectPopup);

	// 아마도 캐릭터당 하나씩일 거 같은데.. 지금 이건 모두 동일한 가격이라는 가정인건가
	const EStoreItemCost StoreItemCostType = bUsedTenLotteryTicket ? EStoreItemCost::TenLotteryTicket : SummonItemCostToStoreItemCost(CostType);
	for (int32 PCI = 0; PCI < UIP_CharSelectPopupCosts.Num(); ++PCI)
	{
		TWeakObjectPtr<UB2UIStoreProductCost>& ThisPopupCost = UIP_CharSelectPopupCosts[PCI];
		if (ThisPopupCost.IsValid())
		{
			ThisPopupCost->SetCost(StoreItemCostType, InCost, bUsedTenLotteryTicket ? 0 : InCalculatedCost);
		}
	}
}

void UB2UISummonItem::UpdateSelectCharacterPopupWindowTitle(const FText& TitleText)
{	
	// 이게 생성된 상태에서 불려야 하는 거
	checkSlow(CreatedUIP_CharSelectPopup && TB_CharSelectTitle.IsValid());
	if (TB_CharSelectTitle.IsValid())
	{
		TB_CharSelectTitle->SetText(TitleText);
	}
}

void UB2UISummonItem::SetCharSelectPopupButtonState(ESlateVisibility VisibleType)
{
	if (BTN_CharSelectPopupGladiator.IsValid())
	{
		BTN_CharSelectPopupGladiator->SetVisibility(VisibleType);
	}

	if (BTN_CharSelectPopupAssassin.IsValid())
	{
		BTN_CharSelectPopupAssassin->SetVisibility(VisibleType);
	}

	if (BTN_CharSelectPopupWizard.IsValid())
	{
		BTN_CharSelectPopupWizard->SetVisibility(VisibleType);
	}

	if (BTN_CharSelectPopupFighter.IsValid())
	{
		BTN_CharSelectPopupFighter->SetVisibility(VisibleType);
	}

	FTimerHandle CharSelectPopupHandle;
	if (VisibleType == ESlateVisibility::HitTestInvisible)
		GetWorld()->GetTimerManager().SetTimer(CharSelectPopupHandle, this, &UB2UISummonItem::SetCharSelectPopupButtonVisible, 1.f, false);
}

void UB2UISummonItem::SetCharSelectPopupButtonVisible()
{
	SetCharSelectPopupButtonState(ESlateVisibility::Visible);
}

void UB2UISummonItem::SetButtonLineVisible(EPCClass InPCClass)
{
	if (CharacterButtonOnArr.Num() == static_cast<int32>(EPCClass::EPC_End)
		&& CharacterButtonOffArr.Num() == static_cast<int32>(EPCClass::EPC_End))
	{
		for (int32 i = 0; i < static_cast<int32>(EPCClass::EPC_End); ++i)
		{
			bool bSelectPc = InPCClass == static_cast<EPCClass>(i);

			CharacterButtonOnArr[i]->SetVisibility(bSelectPc ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
			CharacterButtonOffArr[i]->SetVisibility(bSelectPc ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

		}
	}
}

void UB2UISummonItem::OpenConfirmMsgPopupWindow()
{
	CloseSelectCharacterPopupWindow();

	/*
	if (CP_MsgPopupConfirmSummon.IsValid())
	CP_MsgPopupConfirmSummon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	*/

	auto* Data = BladeIIGameImpl::GetClientDataStore().GetSummonItemData(SelectedSlotHashKey);
	BII_CHECK(Data);

	if (Data)
	{
		SummonCount = Data->IsMultiple() ? 10 : 1;
		CostType = static_cast<ESummonItemCost>(Data->GetCostType());
		InCost = Data->GetCost();
	}

	SetForcedBlockHeader(true);
}

void UB2UISummonItem::CloseConfirmMsgPopupWindow()
{
	/*
	if (CP_MsgPopupConfirmSummon.IsValid())
	CP_MsgPopupConfirmSummon->SetVisibility(ESlateVisibility::Collapsed);
	*/
	SetForcedBlockHeader(false);
}

void UB2UISummonItem::SummonItem()
{
	CloseSelectCharacterPopupWindow();

	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		DocStore->SetSummonItemTabToOpen(CurrentTabMenu);
		DocStore->SetCurrentSummonItemSelectedPCClass(SelectedCharacterClass);

		int32 NewSlotHashKey = SelectedSlotHashKey | 0x01;
		DocStore->SetCurrentSummonItemSlotHashKey(NewSlotHashKey);
	}

	if (auto* Data = BladeIIGameImpl::GetClientDataStore().GetSummonItemData(SelectedSlotHashKey))
	{
		TUTORIAL
			if (TutorialManager::GetInstance().IsCurrentTutorial(TutorialID_Store))
			{
				TutorialManager::GetInstance().OnClickSummonItem();
				data_trader::Retailer::GetInstance().RequestDrawTutorialItem(CliToSvrPCClassType(SelectedCharacterClass));
			}
			else
			{
				int32 CurrentCostType = bUsedTenLotteryTicket ? 
					b2network::B2ShopDrawPriceType::SHOP_TEN_LOTTERY_TICKET : static_cast<int32>(Data->GetCostType()) + b2network::B2ShopDrawPriceType::GEM;

				bool bIsFree = IsFreeSummonableSlot(SelectedSlotHashKey);
				UB2LobbyInventory::IsInventoryFull(EItemInvenType::EIIVT_United, SelectedCharacterClass) ? InventoryFullSimplePopup(SelectedCharacterClass) :
					data_trader::Retailer::GetInstance().RequestDrawShopLottery(SelectedCharacterClass, CurrentCostType, Data->IsMultiple(), false, "", bIsFree);
			}
	}
}

bool UB2UISummonItem::IsFreeSummonableSlot(int32 SlotHashKey)
{
	if (auto* SummonData = BladeIIGameImpl::GetClientDataStore().GetSummonItemData(SlotHashKey))
	{
		if (SummonData->IsMultiple() && SummonData->GetCostType() == int32(ESummonItemCost::Gem))
		{
			// 보석 10개짜리는 Free가 없음
			return false;
		}
		else
		{
			int64 FreeSummonLeftTime = SummonData->GetNextFreeTimeLeftSeconds();
			if (FreeSummonLeftTime == 0)
			{
				return true;
			}
		}
	}

	return false;
}

void UB2UISummonItem::InventoryFullSimplePopup(EPCClass PCClass)
{
	B2UIUtil::OpenInventoryFullPopup(PCClass);
}

void UB2UISummonItem::OnResponseRewardShopMileage(FB2ResponseRewardShopMileagePtr ResponseRewardShopMileage)
{
	if (CreatedUIP_MileageRewardPopup && UIP_StoreMileage.IsValid())
		CreatedUIP_MileageRewardPopup->SetRewardPopup(EStoreMileageType::SummonItem);

	DoMarkRedDot();
	FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_SHOP });
}

void UB2UISummonItem::OnMileageChanged()
{
	DoMarkRedDot();
	FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_SHOP });
}

void UB2UISummonItem::SetOpenRewardPopupBClass()
{
	OpenRewardPopup(EUIStoreMileageGrade::BClass);
}

void UB2UISummonItem::SetOpenRewardPopupAClass()
{
	OpenRewardPopup(EUIStoreMileageGrade::AClass);
}

void UB2UISummonItem::SetOpenRewardPopupSClass()
{
	OpenRewardPopup(EUIStoreMileageGrade::SClass);
}

void UB2UISummonItem::OpenMileageGuidePopup()
{
	ConditionalCreateMileageGuidePopup();
	if (CreatedUIP_MileageGuidePopup)
	{
		CreatedUIP_MileageGuidePopup->OpenPopup();
	}
	SetForcedBlockHeader(true);
}

void UB2UISummonItem::CloseMileageGuidePopup()
{
	if (CreatedUIP_MileageGuidePopup)
	{
		CreatedUIP_MileageGuidePopup->ClosePopup();
	}
	SetForcedBlockHeader(false);
}

void UB2UISummonItem::OnClickChance()
{
	// 가챠 확률 공개
	B2P_ShowKakaoGachaOddsView();
	//B2P_ShowOpenURL(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("ChanceURL")).ToString());
}

#include "B2ItemInfo.h"
void UB2UISummonItem::OpenRewardPopup(EUIStoreMileageGrade BoxGrade)
{
	ConditionalCreateMileageRewardPopup();
	if (CreatedUIP_MileageRewardPopup)
	{
		CreatedUIP_MileageRewardPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (UIP_StoreMileage.IsValid())
			CreatedUIP_MileageRewardPopup->SetRewardPopup(EStoreMileageType::SummonItem);

		B2AndroidBackManager::GetInstance()->AddBackWidget(CreatedUIP_MileageRewardPopup);
	}

	SetForcedBlockHeader(true);
}

void UB2UISummonItem::ConditionalCreateCharSelectPopup()
{
	if (!CreatedUIP_CharSelectPopup && X_CP_UIP_CharSelectPopupPanel.IsValid())
	{
		CreatedUIP_CharSelectPopup = DynLoadAndCreateInCanvasPanelFull<UB2UIBackWidgetBase>(UIP_CharSelectPopupClass, this, X_CP_UIP_CharSelectPopupPanel.Get());

		if (CreatedUIP_CharSelectPopup)
		{
			CreatedUIP_CharSelectPopup->Init();
			CreatedUIP_CharSelectPopup->SetVisibility(ESlateVisibility::Collapsed);

			// 이 정도면 별도의 Widget part 클래스가 되어야 할 것 같은데 완전 블루프린트로 코딩이 되어 있음.
			// 파트에서 가져오는게 상당히 많다.. ㅡㅡ

			TB_CharSelectTitle = CreatedUIP_CharSelectPopup->GetSlot<UTextBlock>(FName(TEXT("TB_CharSelectTitle")));

			BTN_CharSelectPopupClose = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Close")));

			BTN_CharSelectPopupGladiator = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Gladiator")));
			BTN_CharSelectPopupAssassin = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Assassin")));
			BTN_CharSelectPopupWizard = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Wizard")));
			BTN_CharSelectPopupFighter = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Fighter")));

			if (CharacterButtonOnArr.Num() == static_cast<int32>(EPCClass::EPC_End))
			{
				CharacterButtonOnArr[static_cast<int32>(EPCClass::EPC_Gladiator)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_GladiatorOn")));
				CharacterButtonOnArr[static_cast<int32>(EPCClass::EPC_Assassin)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_AssassinOn")));
				CharacterButtonOnArr[static_cast<int32>(EPCClass::EPC_Wizard)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_WizardOn")));
				CharacterButtonOnArr[static_cast<int32>(EPCClass::EPC_Fighter)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_FighterOn")));
			}

			if (CharacterButtonOffArr.Num() == static_cast<int32>(EPCClass::EPC_End))
			{
				CharacterButtonOffArr[static_cast<int32>(EPCClass::EPC_Gladiator)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_GladiatorOff")));
				CharacterButtonOffArr[static_cast<int32>(EPCClass::EPC_Assassin)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_AssassinOff")));
				CharacterButtonOffArr[static_cast<int32>(EPCClass::EPC_Wizard)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_WizardOff")));
				CharacterButtonOffArr[static_cast<int32>(EPCClass::EPC_Fighter)] = CreatedUIP_CharSelectPopup->GetSlot<UOverlay>(FName(TEXT("O_FighterOff")));
			}

			SetButtonLineVisible(EPCClass::EPC_End);

			BTN_CharSelectConfirm = CreatedUIP_CharSelectPopup->GetSlot<UB2Button>(FName(TEXT("BTN_Confirm")));

			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectPopupClose, &UB2UISummonItem::CloseSelectCharacterPopupWindow);
			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectPopupGladiator, &UB2UISummonItem::SelectedGladiator);
			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectPopupAssassin, &UB2UISummonItem::SelectedAssassin);
			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectPopupWizard, &UB2UISummonItem::SelectedWizard);
			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectPopupFighter, &UB2UISummonItem::SelectedFighter);
			BIND_CLICK_FUNC_TO_BTN(BTN_CharSelectConfirm, &UB2UISummonItem::OnClickSelectConfirm);

			UIP_CharSelectPopupCosts.Empty();

			// 캐릭터 enum 순서와 일치..?
			TWeakObjectPtr<UB2UIStoreProductCost> UIP_CharSelectPopupCost1 = CreatedUIP_CharSelectPopup->GetSlot<UB2UIStoreProductCost>(FName(TEXT("UIP_Cost1")));
			if (UIP_CharSelectPopupCost1.IsValid())
			{
				UIP_CharSelectPopupCost1->Init();
				UIP_CharSelectPopupCosts.Add(UIP_CharSelectPopupCost1);
			}
			TWeakObjectPtr<UB2UIStoreProductCost> UIP_CharSelectPopupCost2 = CreatedUIP_CharSelectPopup->GetSlot<UB2UIStoreProductCost>(FName(TEXT("UIP_Cost2")));
			if (UIP_CharSelectPopupCost2.IsValid())
			{
				UIP_CharSelectPopupCost2->Init();
				UIP_CharSelectPopupCosts.Add(UIP_CharSelectPopupCost2);
			}
			TWeakObjectPtr<UB2UIStoreProductCost> UIP_CharSelectPopupCost3 = CreatedUIP_CharSelectPopup->GetSlot<UB2UIStoreProductCost>(FName(TEXT("UIP_Cost3")));
			if (UIP_CharSelectPopupCost3.IsValid())
			{
				UIP_CharSelectPopupCost3->Init();
				UIP_CharSelectPopupCosts.Add(UIP_CharSelectPopupCost3);
			}
			TWeakObjectPtr<UB2UIStoreProductCost> UIP_CharSelectPopupCost4 = CreatedUIP_CharSelectPopup->GetSlot<UB2UIStoreProductCost>(FName(TEXT("UIP_Cost4")));
			if (UIP_CharSelectPopupCost4.IsValid())
			{
				UIP_CharSelectPopupCost4->Init();
				UIP_CharSelectPopupCosts.Add(UIP_CharSelectPopupCost4);
			}
		}
	}
}

void UB2UISummonItem::ConditionalCreateMileageRewardPopup()
{
	if (!CreatedUIP_MileageRewardPopup && X_CP_UIP_MileageRewardPopupPanel.IsValid())
	{
		CreatedUIP_MileageRewardPopup = DynLoadAndCreateInCanvasPanelFull<UB2UIStoreMileageRewardPopup>(UIP_MileageRewardPopupClass, this, X_CP_UIP_MileageRewardPopupPanel.Get());

		if (CreatedUIP_MileageRewardPopup)
		{
			if (MRP_RewardIconTemplate)
			{ // 여기 별도 세팅이 필요한 건데 이것까진 동적 로딩이 아니고..
				CreatedUIP_MileageRewardPopup->SetRewardTemplateClass(MRP_RewardIconTemplate);
			}

			CreatedUIP_MileageRewardPopup->Init();

			// 나머지는 OpenRewardPopup 시 조건에 따라 그때그때 세팅됨
		}
	}
}

void UB2UISummonItem::ConditionalCreateMileageGuidePopup()
{
	if (!CreatedUIP_MileageGuidePopup && X_CP_UIP_MileageGuidePopupPanel.IsValid())
	{
		CreatedUIP_MileageGuidePopup = DynLoadAndCreateInCanvasPanelFull<UB2UIBackWidgetBase>(UIP_MileageGuidePopupClass, this, X_CP_UIP_MileageGuidePopupPanel.Get());
		if (CreatedUIP_MileageGuidePopup)
		{
			CreatedUIP_MileageGuidePopup->Init();
			BTN_MileageGuidePopupClose = CreatedUIP_MileageGuidePopup->GetSlot<UB2Button>(FName(TEXT("BTN_Confirm")));
			BIND_CLICK_FUNC_TO_BTN(BTN_MileageGuidePopupClose, &UB2UISummonItem::CloseMileageGuidePopup);
			CreatedUIP_MileageGuidePopup->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UISummonItem::DestroyCharSelectPopup()
{
	if (CreatedUIP_CharSelectPopup)
	{
		CreatedUIP_CharSelectPopup->DestroySelf(UB2UIManager::GetInstance());
		CreatedUIP_CharSelectPopup = nullptr;
	}
}

void UB2UISummonItem::DestroyMileageRewardPopup()
{
	if (CreatedUIP_MileageRewardPopup)
	{
		if (CreatedUIP_MileageRewardPopup->GetVisibility() == ESlateVisibility::Visible)
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(CreatedUIP_MileageRewardPopup);
		}
		CreatedUIP_MileageRewardPopup->DestroySelf(UB2UIManager::GetInstance());
		CreatedUIP_MileageRewardPopup = nullptr;
	}
}

void UB2UISummonItem::DestroyMileageGuidePopup()
{
	if (CreatedUIP_MileageGuidePopup)
	{
		CreatedUIP_MileageGuidePopup->ClosePopup();
		CreatedUIP_MileageGuidePopup->DestroySelf(UB2UIManager::GetInstance());
		CreatedUIP_MileageGuidePopup = nullptr;
	}
}

void UB2UISummonItem::SetForcedBlockHeader(bool IsBlock)
{
	UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
	if (HeaderUI != nullptr)
		HeaderUI->SetForcedBlockHeader(IsBlock);
}
