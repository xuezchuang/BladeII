// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UISealEquipmentMain.h"
#include "B2UIStoreProductCost.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "BladeIIGameImpl.h"
#include "B2DynText_Multiple.h"
#include "BladeIILocText.h"
#include "B2UIRewardEventSet.h"
#include "B2UIManager.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "TutorialManager.h"

void UB2UISealEquipmentSlot::Init()
{
	Super::Init();
	SetButtonStyle();
}

void UB2UISealEquipmentSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_SealSlot);
	GET_SLOT(UTextBlock, TB_SealOff);
	GET_SLOT(UTextBlock, TB_SealOption);

	GET_SLOT(UWidgetSwitcher, WS_SlotState);
	GET_SLOT(UOverlay, O_Choice);

	SealOpenAni = GetAnimation(this, "SealOpen");
	SealAni = GetAnimation(this, "Seal");
}

void UB2UISealEquipmentSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UISealEquipmentSlot::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_SealOff.IsValid())
		TB_SealOff->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_SealSlot")));
}


void UB2UISealEquipmentSlot::SealSlotSetting(const FSealOption& InOption, EPCClass InPCClass)
{
	if (InOption.IsOpened)
	{
		if (InOption.OptionInfo.OptionType == EItemOption::EIO_End)
			SetSwitcherState(ESealSlotSwitcher::Off);
		else
		{
			SetSwitcherState(ESealSlotSwitcher::On);
			SetSealOptionText(InOption.OptionInfo.OptionType, InOption.OptionInfo.RawOptionAmount, InPCClass);
		}
	}
	else
		SetSwitcherState(ESealSlotSwitcher::Lock);
}

void UB2UISealEquipmentSlot::SetSwitcherState(ESealSlotSwitcher InType)
{
	if (WS_SlotState.IsValid())
		WS_SlotState->SetActiveWidgetIndex(static_cast<int32>(InType));
}

void UB2UISealEquipmentSlot::SetSealOptionText(EItemOption InOption, float Value, EPCClass InPCClass)
{
	FText OptionName = GetLOCTextOfItemOption(InOption, InPCClass);
	FText OptionValue = GetItemOptionValueDisplayText(InOption, Value , true);

	if (TB_SealOption.IsValid())
		TB_SealOption->SetText(FText::Format(FText::FromString(TEXT("{0}  {1}")), OptionName, OptionValue));
}

void UB2UISealEquipmentSlot::SetButtonStyle()
{
	//FButtonStyle SlotButtonStyle;

	//if (BTN_SealSlot.IsValid())
	//{
	//	SlotButtonStyle = BTN_SealSlot->WidgetStyle;
	//	SlotButtonStyle.Normal.TintColor = TintColor;
	//	SlotButtonStyle.Hovered.TintColor = TintColor;
	//	SlotButtonStyle.Pressed.TintColor = TintColor;
	//	SlotButtonStyle.Disabled.TintColor = TintColor;
	//	BTN_SealSlot->SetStyle(SlotButtonStyle);
	//}
}

void UB2UISealEquipmentSlot::PlaySealOpenAnimation()
{
	PlayAnimation(SealOpenAni.Get());
}

void UB2UISealEquipmentSlot::PlaySealAnimation()
{
	PlayAnimation(SealAni.Get());

}

float UB2UISealEquipmentSlot::GetSealOpenEndAniTime()
{
	if (SealOpenAni.IsValid())
		return SealOpenAni->GetEndTime();

	return 0;
}

float UB2UISealEquipmentSlot::GetSealEndAniTime()
{
	if (SealAni.IsValid())
		return SealAni->GetEndTime();

	return 0;
}

void UB2UISealEquipmentMain::Init()
{
	Super::Init();
	
}

void UB2UISealEquipmentMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_NoAction);

	//////////////////
	GET_SLOT(UVerticalBox, VB_OptionList);
	GET_SLOT(UHorizontalBox, HB_SealMaterial);

	GET_SLOT(UB2Button, BTN_Seal);
	GET_SLOT(UB2Button, BTN_UnLock);


	GET_SLOT(UB2UIStoreProductCost, UIP_SealCost);
	if (UIP_SealCost.IsValid())
		UIP_SealCost->Init();
	GET_SLOT(UB2UIStoreProductCost, UIP_UnLockCost);
	if (UIP_UnLockCost.IsValid())
		UIP_UnLockCost->Init();

	GET_SLOT(UCanvasPanel, CP_Seal);
	GET_SLOT(UCanvasPanel, CP_Lock);



	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UB2RichTextBlock, RTB_OptionTitle);
	GET_SLOT(UB2RichTextBlock, RTB_OptionDesc);

	GET_SLOT(UTextBlock, TB_SealButton);

	GET_SLOT(UTextBlock, TB_LockDesc);
	GET_SLOT(UTextBlock, TB_UnLock);

	GET_SLOT(UScrollBox, SB_SealMaterial)

	GET_SLOT(UB2DynItemIcon_LobbyInven, TargetItemIcon);

	ArraySealSlot.Empty();
	ArraySealSlot.SetNum(4);
	GET_SLOT_BYNAME(UB2UISealEquipmentSlot, ArraySealSlot[0], UIP_SealEquipmentSlot);
	GET_SLOT_BYNAME(UB2UISealEquipmentSlot, ArraySealSlot[1], UIP_SealEquipmentSlot_1);
	GET_SLOT_BYNAME(UB2UISealEquipmentSlot, ArraySealSlot[2], UIP_SealEquipmentSlot_2);
	GET_SLOT_BYNAME(UB2UISealEquipmentSlot, ArraySealSlot[3], UIP_SealEquipmentSlot_3);

	for (TWeakObjectPtr<UB2UISealEquipmentSlot> SlotInfo : ArraySealSlot)
		SlotInfo->Init();
}

void UB2UISealEquipmentMain::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UISealEquipmentMain::CloseWidgetDelegate()
{
	if (CP_Seal.IsValid() && CP_Seal->GetVisibility() != ESlateVisibility::Collapsed)
	{
		if (BTN_Seal.IsValid() && BTN_Seal->GetIsEnabled() == false)
			return;
	}

	if (CP_Lock.IsValid() && CP_Lock->GetVisibility() != ESlateVisibility::Collapsed)
	{
		if (BTN_UnLock.IsValid() && BTN_UnLock->GetIsEnabled() == false)
			return;
	}

	UB2UIManager::GetInstance()->CloseUI(UIFName::SealMain);
}


void UB2UISealEquipmentMain::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UISealEquipmentMain::OnClickClose);

	BIND_CLICK_FUNC_TO_BTN(BTN_UnLock, &UB2UISealEquipmentMain::OnClickUnLock);
	BIND_CLICK_FUNC_TO_BTN(BTN_Seal, &UB2UISealEquipmentMain::OnClickSeal);

	BIND_CLICK_FUNC_TO_BTN(ArraySealSlot[0]->BTN_SealSlot, &UB2UISealEquipmentMain::OnClickSlot1);
	BIND_CLICK_FUNC_TO_BTN(ArraySealSlot[1]->BTN_SealSlot, &UB2UISealEquipmentMain::OnClickSlot2);
	BIND_CLICK_FUNC_TO_BTN(ArraySealSlot[2]->BTN_SealSlot, &UB2UISealEquipmentMain::OnClickSlot3);
	BIND_CLICK_FUNC_TO_BTN(ArraySealSlot[3]->BTN_SealSlot, &UB2UISealEquipmentMain::OnClickSlot4);
}

void UB2UISealEquipmentMain::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);
	SubscribeEvents();
}

void UB2UISealEquipmentMain::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);
	UnSubscribeEvents();
}

void UB2UISealEquipmentMain::SubscribeEvents()
{
	
	DeliveryOpenSealSlotItemTicket = DeliveryOpenSealSlotItemClass<FB2ResponseOpenSealSlotItemPtr>::GetInstance().Subscribe(
		[this](const FB2ResponseOpenSealSlotItemPtr& Info)
	{
		FB2Item SealItem;
		SealItem = Info->slot_opened_item;
		this->ResponseSealOpenUIRefresh(SealItem);
		this->ResponseCostRefresh(Info->current_gold, Info->current_gem);
		LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal();
	}
	);

	DeliveryOpenSealSlotItemErrorTicket = DeliveryOpenSealSlotItemErrorClass<>::GetInstance().Subscribe(
		[this]()
	{
		this->ActionFunction(true);
	}
	);

	DeliveryCarveSealItemTicket = DeliveryCarveSealItemClass<FB2ResponseCarveSealItemPtr>::GetInstance().Subscribe(
		[this](const FB2ResponseCarveSealItemPtr& Info)
	{
		FB2Item SealItem;
		SealItem = Info->sealed_item;
		this->ResponseSealUIRefresh(SealItem);
		this->ResponseCostRefresh(Info->current_gold, Info->current_gem);
		this->ResponsematerialRefresh(Info->updated_material_items, Info->deleted_material_item_ids);
		LobbyTryUpdateItemOPMainMenuClass<>::GetInstance().Signal();
	}
	);

	DeliveryCarveSealItemErrorTicket = DeliveryCarveSealItemErrorClass<>::GetInstance().Subscribe(
		[this]()
	{
		this->ActionFunction(true);
	}
	);

}

void UB2UISealEquipmentMain::UnSubscribeEvents()
{
	DeliveryOpenSealSlotItemClass<FB2ResponseOpenSealSlotItemPtr>::GetInstance().Unsubscribe(DeliveryOpenSealSlotItemTicket);
	DeliveryOpenSealSlotItemErrorClass<>::GetInstance().Unsubscribe(DeliveryOpenSealSlotItemErrorTicket);
	DeliveryCarveSealItemClass<FB2ResponseCarveSealItemPtr>::GetInstance().Unsubscribe(DeliveryCarveSealItemTicket);
	DeliveryCarveSealItemErrorClass<>::GetInstance().Unsubscribe(DeliveryCarveSealItemErrorTicket);
}

void UB2UISealEquipmentMain::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Seal")));
}

void UB2UISealEquipmentMain::OpenSealItemMain(const FB2Item& TargetItem)
{
	UpdateTargetItem(TargetItem);
	UpdateSlot();
	OnClickSlot1();
}

void UB2UISealEquipmentMain::UpdateTargetItem(const FB2Item& TargetItem)
{
	CacheItem = TargetItem;
	TargetItemIcon->UpdateItemData(CacheItem);
	TargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget);
}

void UB2UISealEquipmentMain::UpdateSlot()
{
	for (FSealOption SealOptionInfo : CacheItem.SealOptions)
	{
		for (TWeakObjectPtr<UB2UISealEquipmentSlot> SlotInfo : ArraySealSlot)
		{
			if (SlotInfo->OptionSlotNumber == SealOptionInfo.SealSlotNumber)
			{
				SlotInfo->SealSlotSetting(SealOptionInfo, static_cast<EPCClass>(CacheItem.AllowedPCClass));
				break;
			}
		}
	}
}


void UB2UISealEquipmentMain::UpdateSealOptionList(int32 InSlotNumber)
{
	TArray<b2network::B2mdSelectItemSealOptionPtr> SealOptionData = BladeIIGameImpl::GetTotemDataStore().GetSealOptionData(InSlotNumber);
	VB_OptionList->ClearChildren();

	int32 ListIndex = 0;
	for (b2network::B2mdSelectItemSealOptionPtr SealOptionInfo : SealOptionData)
	{
		EItemOption SealOptionType = SvrToCliOptionType(SealOptionInfo->option_id);
		FText OptionName = GetLOCTextOfItemOption(SealOptionType, static_cast<EPCClass>(CacheItem.AllowedPCClass));
		FText OptionValueMin = GetItemOptionValueDisplayText(SealOptionType, SealOptionInfo->min, true);
		FText OptionValueMax = GetItemOptionValueDisplayText(SealOptionType, SealOptionInfo->max, true);

		UB2DynText_Multiple* pSealOption = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(UISealOptionClass, this, VB_OptionList.Get(), FMargin()));
		if (pSealOption)
		{
			pSealOption->SetDynText(ListIndex, OptionName, FText::Format(FText::FromString(TEXT("({0} ~ {1})")), OptionValueMin, OptionValueMax));
			++ListIndex;
		}
	}

}

void UB2UISealEquipmentMain::UpdateCost(int32 InSlotNumber)
{
	bool IsBeforeSlotLock = true;
	b2network::B2mdItemSealCostPtr	SealCostData = BladeIIGameImpl::GetTotemDataStore().GetSealCostData(InSlotNumber);

	for (FSealOption SealOptionInfo : CacheItem.SealOptions)
	{
		if (InSlotNumber - 1 == SealOptionInfo.SealSlotNumber && SealOptionInfo.IsOpened == false)		//전슬롯이 닫혀있는지 보자
		{
			IsBeforeSlotLock = false;
		}

		if (InSlotNumber == SealOptionInfo.SealSlotNumber)
		{
			if (SealOptionInfo.IsOpened == false)
			{
				if (CP_Lock.IsValid())
					CP_Lock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				if (CP_Seal.IsValid())
					CP_Seal->SetVisibility(ESlateVisibility::Collapsed);

				if (IsBeforeSlotLock == false)			//그이전슬롯도 닫혀있을경우 버튼 비활성화 && 텍스트 변경
				{
					TB_LockDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SealPopup_SlotUnlock_Precondition")));
					BTN_UnLock->SetIsEnabled(false);
				}
				else
				{
					TB_LockDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SealPopup_SlotUnlock"))); //추후나오면 변경
					BTN_UnLock->SetIsEnabled(true);
				}

				if (UIP_UnLockCost.IsValid())
					UIP_UnLockCost->SetSealCosnt(static_cast<ESealCostType>(SealCostData->open_price_type), SealCostData->open_price);
			}
			else                     //	각인 개방되어있을때
			{
				if (CP_Lock.IsValid())
					CP_Lock->SetVisibility(ESlateVisibility::Collapsed);
				if (CP_Seal.IsValid())
					CP_Seal->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				SB_SealMaterial->ClearChildren();

				if(UIP_SealCost.IsValid())
					UIP_SealCost->SetSealCosnt(static_cast<ESealCostType>(SealCostData->seal_price_type), SealCostData->seal_price);

				RewardIconCreate(SealCostData->template_id1, SealCostData->amount1);
				RewardIconCreate(SealCostData->template_id2, SealCostData->amount2);
				RewardIconCreate(SealCostData->template_id3, SealCostData->amount3);
				RewardIconCreate(SealCostData->template_id4, SealCostData->amount4);

			}
		}
	}
}

void UB2UISealEquipmentMain::ResponseSealOpenUIRefresh(const FB2Item& TargetItem)
{
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this->GetWorld()));

	CacheItem = TargetItem;

	float AniTime = 0;

	for (TWeakObjectPtr<UB2UISealEquipmentSlot> SlotInfo : ArraySealSlot)
	{
		if (SlotInfo->OptionSlotNumber == SelectSlotNumber)
		{
			SlotInfo->PlaySealOpenAnimation();
		}
	}

	if (ArraySealSlot.Num() > 0)
	{
		AniTime = ArraySealSlot[0]->GetSealOpenEndAniTime();
	}


	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2UISealEquipmentMain::OpenSealRefresh, AniTime, false);
}

void UB2UISealEquipmentMain::ResponseSealUIRefresh(const FB2Item& TargetItem)
{
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this->GetWorld()));

	CacheItem = TargetItem;

	float AniTime = 0;

	for (TWeakObjectPtr<UB2UISealEquipmentSlot> SlotInfo : ArraySealSlot)
	{
		if (SlotInfo->OptionSlotNumber == SelectSlotNumber)
		{
			SlotInfo->PlaySealAnimation();
		}
	}

	if (TargetItemIcon.IsValid())
		TargetItemIcon->PlayEnhanceResultAnim();

	if (ArraySealSlot.Num() > 0)
	{
		AniTime = ArraySealSlot[0]->GetSealEndAniTime();
	}

	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2UISealEquipmentMain::UIRefresh, AniTime, false);


}

void UB2UISealEquipmentMain::ResponseCostRefresh(int32 InGold, int32 Gem)
{
	BladeIIGameImpl::GetClientDataStore().SetGemAmount(Gem);
	BladeIIGameImpl::GetClientDataStore().SetGoldAmount(InGold);

	UB2UIDocUser* UserDoc = UB2UIDocHelper::GetDocUser();

	if (UserDoc)
	{
		UserDoc->SetGold(InGold);
		UserDoc->SetGem(Gem);
	}
}

void UB2UISealEquipmentMain::ResponsematerialRefresh(const TArray<b2network::B2ItemServerInfoPtr>& InUpdatedMaterialItems, const TArray<int64>& InDeletedConsumableIDs)
{
	if (InUpdatedMaterialItems.Num() > 0 || InDeletedConsumableIDs.Num() > 0)
	{
		int32 TemplateID = InUpdatedMaterialItems.Num() == 0 ? 0 : InUpdatedMaterialItems[0]->template_id;
		BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountDecrease(TemplateID, -1, InUpdatedMaterialItems, InDeletedConsumableIDs);
	}
}

bool UB2UISealEquipmentMain::CheckSealPopup()
{
	for (FSealOption SealInfo : CacheItem.SealOptions)
	{
		if (SealInfo.SealSlotNumber == SelectSlotNumber && SealInfo.OptionInfo.OptionType != EItemOption::EIO_End)
		{

			FText OptionName = GetLOCTextOfItemOption(SealInfo.OptionInfo.OptionType, static_cast<EPCClass>(CacheItem.AllowedPCClass));
			FText OptionValue = GetItemOptionValueDisplayText(SealInfo.OptionInfo.OptionType, SealInfo.OptionInfo.RawOptionAmount, true);

			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			if (UIMgrInst)
			{
				UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
					BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
					FText::Format(
						BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SealPopup_OptionDeleteMessage")), OptionName, OptionValue)
					, // 문구에는 추가 정보가 필요해질 수도..?
					0.f,
					true,
					true,
					EUIMsgPopupButtonGroup::ConfirmOrCancel,
					FMsgPopupOnClick::CreateLambda([this]() {
					data_trader::Retailer::GetInstance().RequestCarveSealItem(CacheItem.InstanceUID, SelectSlotNumber);
					ActionFunction(false);
				})
					);
				return true;
			}
		}
	}

	return false;
}

void UB2UISealEquipmentMain::UIRefresh()
{
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this->GetWorld()));

	UpdateTargetItem(CacheItem);
	UpdateSlot();
	OnClickUpdate(SelectSlotNumber);


	if (LobbyGM && LobbyGM->GetLobbyInventory())
	{
		LobbyGM->GetLobbyInventory()->UpdateSingleItemData(CacheItem.InstanceUID, CacheItem);
	}

	ActionFunction(true);

	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(static_cast<EPCClass>(CacheItem.AllowedPCClass), true);
}

void UB2UISealEquipmentMain::OpenSealRefresh()
{
	DevShowNotReadyYetMessage(FString(TEXT("SealPopup_SlotOpenMessage")));
	UIRefresh();
}

void UB2UISealEquipmentMain::RewardIconCreate(int32 InTemplateId, int32 InAmount)
{
	if (InTemplateId)
	{
		UB2UIRewardIcon* SealNeedItem = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), UIRewardIconClass);
		SB_SealMaterial->AddChild(SealNeedItem);

		int32 CurrentAmount = UB2LobbyInventory::GetSharedConsumableAmountOfType(InTemplateId);

		SealNeedItem->Init();
		SealNeedItem->UpdateRewardIconByItemIndex(InTemplateId, FText::AsNumber(InAmount));
		SealNeedItem->SetNeedCount(CurrentAmount, InAmount);
	}
}

void UB2UISealEquipmentMain::OnClickUpdate(int32 InSlotNumber)
{
	SelectSlotNumber = InSlotNumber;
	UpdateSealOptionList(InSlotNumber);
	UpdateCost(InSlotNumber);

	for (TWeakObjectPtr<UB2UISealEquipmentSlot> SlotInfo : ArraySealSlot)
	{
		SlotInfo->O_Choice->SetVisibility(SlotInfo->OptionSlotNumber == InSlotNumber ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UISealEquipmentMain::ActionFunction(bool InAction)
{
	if (BTN_Seal.IsValid())
		BTN_Seal->SetIsEnabled(InAction);

	if (BTN_UnLock.IsValid())
		BTN_UnLock->SetIsEnabled(InAction);

	if (BTN_NoAction.IsValid())
		BTN_NoAction->SetVisibility(InAction ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UB2UISealEquipmentMain::CheckTutorialNextStep()
{
	if (TutorialManager::GetInstance().IsCurrentTutorial(TutorialID_Seal))
	{
		TutorialManager::GetInstance().GotoNextStep();
	}
}

void UB2UISealEquipmentMain::OnClickSlot1()
{
	OnClickUpdate(1);
	//CheckTutorialNextStep();
}

void UB2UISealEquipmentMain::OnClickSlot2()
{
	OnClickUpdate(2);
	//CheckTutorialNextStep();
}

void UB2UISealEquipmentMain::OnClickSlot3()
{
	OnClickUpdate(3);
	CheckTutorialNextStep();
}

void UB2UISealEquipmentMain::OnClickSlot4()
{
	OnClickUpdate(4);
	CheckTutorialNextStep();
}

void UB2UISealEquipmentMain::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::SealMain);
}

void UB2UISealEquipmentMain::OnClickUnLock()
{
	data_trader::Retailer::GetInstance().RequestOpenSealSlotItem(CacheItem.InstanceUID, SelectSlotNumber);
	ActionFunction(false);

}

void UB2UISealEquipmentMain::OnClickSeal()
{
	if (!CheckSealPopup())
	{
		data_trader::Retailer::GetInstance().RequestCarveSealItem(CacheItem.InstanceUID, SelectSlotNumber);
		ActionFunction(false);
	}
}
