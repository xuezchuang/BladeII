// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIAutoClearResult.h"
#include "B2UIPAutoClearResultCharacter.h"
#include "BladeIILocText.h"
#include "B2UIManager.h"
#include "BladeIIGameImpl.h"
#include "B2UIPAutoClearResultRow.h"
#include "Retailer.h"
#include "B2UIStageResultLevelUp.h"
#include "Event.h"

void UB2UIAutoClearResult::Init()
{
	RepeatCount = 0;
	Super::Init();

	SubscribeEvents();

	SetRepeat(false);

	if (UIP_MainChar.IsValid())
	{
		UIP_MainChar->Init();
	}

	if (UIP_SubChar.IsValid())
	{
		UIP_SubChar->Init();
	}

	if (UIP_LevelUp.IsValid())
	{
		UIP_LevelUp->Init();
	}
}

void UB2UIAutoClearResult::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	UnsubscribeEvents();

	if (UIP_MainChar.IsValid())
	{
		UIP_MainChar->DestroySelf(InUIManager);
	}

	if (UIP_SubChar.IsValid())
	{
		UIP_SubChar->DestroySelf(InUIManager);
	}

	if (UIP_LevelUp.IsValid())
	{
		UIP_LevelUp->DestroySelf(InUIManager);
	}
}

void UB2UIAutoClearResult::SetInfo(const FB2SweepStageInfo& InSweepStageinfo)
{
	//if (TB_Default.IsValid())
	//{
	//	TB_Default->SetVisibility(ESlateVisibility::Collapsed);
	//}

	//ServerStageId = FServerStageID(GET_TUPLE_DATA(FB2ResponseSweepStage::stage_id_index, InSweepStageinfo));
	//
	//const TArray<b2network::B2CharacterServerInfoPtr> characters = GET_TUPLE_DATA(FB2ResponseSweepStage::characters_index, InSweepStageinfo);
	//bool IsMainCharLevelUp = GET_TUPLE_DATA(FB2ResponseSweepStage::is_level_up_main_character_index, InSweepStageinfo);
	//bool IsSubCharLevelUp = GET_TUPLE_DATA(FB2ResponseSweepStage::is_level_up_tag_character_index, InSweepStageinfo);
	//SetCharacterInfo(characters, IsMainCharLevelUp, IsSubCharLevelUp);

	//TArray<FB2Item> Items;
	//GetResultItemInfo(Items, InSweepStageinfo);

	//if (TB_AutoCount.IsValid())
	//{
	//	int64 AutoClearCount = GET_TUPLE_DATA(FB2ResponseSweepStage::stage_sweep_ticket_index, InSweepStageinfo);;
	//	TB_AutoCount->SetText(FText::Format(FText::FromString(TEXT("x {0}")), FText::AsNumber(AutoClearCount)));

	//	if (AutoClearCount <= 0)
	//	{
	//		TB_AutoCount->SetColorAndOpacity(FLinearColor::Red);
	//	}
	//}

	//// 최종 Total Gold (장비 판매 Gold 미포함)
	//int32 TotalGold = GET_TUPLE_DATA(FB2ResponseSweepStage::total_added_gold_index, InSweepStageinfo);
	//int32 ServerRepeatCount = GET_TUPLE_DATA(FB2ResponseSweepStage::repeat_count_index, InSweepStageinfo);
	//AddSlot(Items, ServerRepeatCount, TotalGold);
	//
	//CheckRepeat(IsMainCharLevelUp, IsSubCharLevelUp);
}

void UB2UIAutoClearResult::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_AutoClear);
	GET_SLOT(UTextBlock, TB_AutoCount);
	GET_SLOT(UTextBlock, TB_OneClear);
	GET_SLOT(UTextBlock, TB_RepeatAutoClear);
	GET_SLOT(UTextBlock, TB_OK);
	GET_SLOT(UTextBlock, TB_Exp);
	GET_SLOT(UTextBlock, TB_Default);
	GET_SLOT(UTextBlock, TB_TripleInfo);
	GET_SLOT(UB2ScrollBox, SB_ResultItem);
	GET_SLOT(UB2Button, BTN_OneClear);
	GET_SLOT(UB2Button, BTN_RepeatAutoClear);
	GET_SLOT(UB2Button, BTN_OK);
	GET_SLOT(UB2UIPAutoClearResultCharacter, UIP_MainChar);
	GET_SLOT(UB2UIPAutoClearResultCharacter, UIP_SubChar);
	GET_SLOT(UImage, IMG_On);
	GET_SLOT(UImage, IMG_Off);
	GET_SLOT(UB2Image, FX_On);
	GET_SLOT(UThrobber, T_On);
	GET_SLOT(UB2UIStageResultLevelUp, UIP_LevelUp);
}

void UB2UIAutoClearResult::UpdateStaticText()
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_AutoClear_Result")));
	}

	if (TB_AutoClear.IsValid())
	{
		TB_AutoClear->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_AutoClear_TicketCount")));
	}

	if (TB_OneClear.IsValid())
	{
		TB_OneClear->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_AutoClear_OneTime")));
	}

	if (TB_RepeatAutoClear.IsValid())
	{
		TB_RepeatAutoClear->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_AutoClear_Ongoing")));
	}

	if (TB_OK.IsValid())
	{
		TB_OK->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
	}

	if (TB_Exp.IsValid())
	{
		TB_Exp->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_EXP")));
	}

	if (TB_Default.IsValid())
	{
		TB_Default->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_AutoClear_DefaultDesc")));
	}

	if (TB_TripleInfo.IsValid())
	{
		TB_TripleInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_AutoClear_Tip")));
	}
}

void UB2UIAutoClearResult::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_OneClear, &UB2UIAutoClearResult::OnClickBTNOneClear);
	BIND_CLICK_FUNC_TO_BTN(BTN_RepeatAutoClear, &UB2UIAutoClearResult::OnClickBTNRepeatAutoClear);
	BIND_CLICK_FUNC_TO_BTN(BTN_OK, &UB2UIAutoClearResult::OnClickBTNOK);
}

void UB2UIAutoClearResult::OnClickBTNOneClear()
{
	ResetInfo();
	RequestSweepStagePacket();
}

void UB2UIAutoClearResult::OnClickBTNRepeatAutoClear()
{
	if (IsRepeat == true) // 반복끝
	{
		OpenRepeatEndPopup();
		SetRepeat(false);
	}
	else // 반복시작
	{
		ResetInfo();
		SetRepeat(true);
	}
}

void UB2UIAutoClearResult::OnClickBTNOK()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::AutoClearResultPopup);
}

void UB2UIAutoClearResult::ResetInfo()
{
	RepeatCount = 0;

	for (TWeakObjectPtr<class UB2UIPAutoClearResultRow> Elem : RowSlots)
	{
		if (Elem.IsValid())
		{
			Elem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (TB_Default.IsValid())
	{
		TB_Default->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2UIAutoClearResult::RequestSweepStagePacket()
{
	EPCClass MainChar = SvrToCliPCClassType(ServerTypeMainChar);
	EPCClass SubChar = SvrToCliPCClassType(ServerTypeSubChar);
	if (MainChar != EPCClass::EPC_End && SubChar != EPCClass::EPC_End && MainChar != SubChar)
	{
		int32 MainPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(MainChar);
		int32 SubPCPresetId = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().GetCurrentItemPresetID(SubChar);

		data_trader::Retailer::GetInstance().RequestSweepStage(
			ServerStageId,
			ServerTypeMainChar,
			ServerTypeSubChar,
			MainPCPresetId,
			SubPCPresetId,
			RepeatCount);
	}
}

void UB2UIAutoClearResult::SubscribeEvents()
{
	CAPTURE_UOBJECT(UB2UIAutoClearResult);

	Issues.Add(DeliverySweepStageErrorClass<FB2SweepStageInfo, int32>::GetInstance().Subscribe2(
		[Capture](const FB2SweepStageInfo& InSweepStageinfo, int32 InErrorCode)
	{
		if (Capture.IsValid())
		{
			Capture->SetRepeat(false);
		}
	}
	));

	//LevelupFinishDelegate 계속 지워서 사용하기 힘듬..
	Issues.Add(OnResetStartingLevelInfoClass<>::GetInstance().Subscribe2(
		[Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->LevelupFinish();
		}
	}
	));

	Issues.Add(ToolTipMoveClass<>::GetInstance().Subscribe2(
		[Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->OnClickBTNOK();
		}
	}
	));
}

void UB2UIAutoClearResult::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UIAutoClearResult::SetRepeat(bool InValue)
{
	IsRepeat = InValue;
	if (InValue)//자동 소탕 시작
	{
		IsRepeat = true;
		RequestSweepStagePacket();

		if (BTN_OK.IsValid())
		{
			BTN_OK->SetIsEnabled(false);
		}

		if (BTN_OneClear.IsValid())
		{
			BTN_OneClear->SetIsEnabled(false);
		}

		if (IMG_Off.IsValid())
		{
			IMG_Off->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IMG_On.IsValid())
		{
			IMG_On->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (FX_On.IsValid())
		{
			FX_On->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (T_On.IsValid())
		{
			T_On->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (TB_RepeatAutoClear.IsValid())
		{
			TB_RepeatAutoClear->SetColorAndOpacity(FLinearColor(0.82f, 0.79f, 0.77f, 1.0f));
			TB_RepeatAutoClear->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_AutoClear_Ongoing2")));
		}
	}
	else//자동 소탕 끝
	{
		if (BTN_OK.IsValid())
		{
			BTN_OK->SetIsEnabled(true);
		}

		if (BTN_OneClear.IsValid())
		{
			BTN_OneClear->SetIsEnabled(true);
		}

		if (IMG_Off.IsValid())
		{
			IMG_Off->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (IMG_On.IsValid())
		{
			IMG_On->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (FX_On.IsValid())
		{
			FX_On->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (T_On.IsValid())
		{
			T_On->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (TB_RepeatAutoClear.IsValid())
		{
			TB_RepeatAutoClear->SetColorAndOpacity(FLinearColor(0.52f, 0.55f, 0.59f, 1.0f));
			TB_RepeatAutoClear->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_AutoClear_Ongoing")));
		}
	}
}

void UB2UIAutoClearResult::OpenRepeatEndPopup()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(
		EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_AutoClear_StopDesc")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
	);
}

void UB2UIAutoClearResult::SetCharacterInfo(const TArray<b2network::B2CharacterServerInfoPtr>& InCharInfo, bool IsMainCharLevelUp, bool IsSubCharLevelUp)
{
	if (InCharInfo.IsValidIndex(0))
	{
		UIP_MainChar->SetInfo(InCharInfo[0], IsMainCharLevelUp);
		ServerTypeMainChar = InCharInfo[0]->character_type;
	}

	if (InCharInfo.IsValidIndex(1))
	{
		UIP_SubChar->SetInfo(InCharInfo[1], IsSubCharLevelUp);
		ServerTypeSubChar = InCharInfo[1]->character_type;
	}
}

void UB2UIAutoClearResult::GetResultItemInfo(OUT TArray<FB2Item>& ResultItems, const FB2SweepStageInfo& InSweepStageinfo)
{
	//// 보상 아이템.. 기본 아이템 목록이랑.. 보상골드(드랍골드 이외의)도 있으면 보상아이템 형태로 추가한다.
	//GetB2ItemArray(FB2ResponseSweepStage::items_index, InSweepStageinfo);

	//// 보스조각 보상 아이템
	//const TArray<b2network::B2DropDataPtr> FixedItemInfos = GET_TUPLE_DATA(FB2ResponseSweepStage::fixed_items_index, InSweepStageinfo);
	//for (auto FixedItemInfo : FixedItemInfos)
	//{
	//	FB2Item BossItem;
	//	BossItem.ItemRefID = FixedItemInfo->template_id;
	//	BossItem.ConsumingAmount = FixedItemInfo->count;
	//	BossItem.ItemClass = EItemClass::EIC_BossPiece;
	//	B2ItemArray.Add(BossItem);
	//}

	//// 에테르 보상 아이템
	//const TArray<b2network::B2AetherServerInfoPtr> EtherInfos = GET_TUPLE_DATA(FB2ResponseSweepStage::aethers_index, InSweepStageinfo);

	//if (EtherInfos.Num() > 0)
	//{
	//	BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().MakeDirty();
	//}

	//for (const b2network::B2AetherServerInfoPtr& Elem : EtherInfos)
	//{
	//	FB2Item ItemEther(Elem);
	//	B2ItemArray.Add(ItemEther);
	//}

	//// 재료 보상 아이템
	//const TArray<b2network::B2DropDataPtr> MaterialItemInfos = GET_TUPLE_DATA(FB2ResponseSweepStage::material_items_index, InSweepStageinfo);
	//for (const b2network::B2DropDataPtr& Elem : MaterialItemInfos)
	//{
	//	FB2Item MaterialItem;
	//	MaterialItem.ItemRefID = Elem->template_id;
	//	MaterialItem.ConsumingAmount = Elem->count;
	//	MaterialItem.InventoryType = EItemInvenType::EIIVT_Consumables;
	//	MaterialItem.ItemClass = EItemClass::EIC_End;
	//	B2ItemArray.Add(MaterialItem);
	//}

	///* Gold */
	//// 스테이지 클리어 후 보상으로 줄수 있는 Reward Gold
	//TArray<int32> StageRewardGolds = GET_TUPLE_DATA(FB2ResponseSweepStage::reward_golds_index, InSweepStageinfo);
	//int32 GoldBuffRate = GET_TUPLE_DATA(FB2ResponseSweepStage::gold_buff_rate_index, InSweepStageinfo);// 핫타임 골드 버프 증가율(퍼센트)
	//int32 GoldItemEquipRate = GET_TUPLE_DATA(FB2ResponseSweepStage::equip_item_gold_rate_index, InSweepStageinfo);
	//int32 GoldItemSetRate = GET_TUPLE_DATA(FB2ResponseSweepStage::item_set_gold_rate_index, InSweepStageinfo);
	//int32 TotalGoldRate = GoldBuffRate + GoldItemEquipRate + GoldItemSetRate;

	//if (StageRewardGolds.Num() > 0)
	//{
	//	for (int32 StageRewardGold : StageRewardGolds)
	//	{
	//		FB2Item RewardGoldItem;
	//		float GoldSum = StageRewardGold *  (1.0f + (TotalGoldRate / 100.0f));
	//		RewardGoldItem.ItemRefID = FItemRefIDHelper::GetGoodsID_Gold();
	//		RewardGoldItem.ItemClass = EItemClass::EIC_Gold;
	//		RewardGoldItem.InventoryType = EItemInvenType::EIIVT_Consumables;
	//		RewardGoldItem.ConsumingAmount = (int32)GoldSum;
	//		RewardGoldItem.SetPrimaryPoint((int32)GoldSum); // 예전에 PrimaryPoint 로 쓴 게 있어서..
	//		B2ItemArray.Add(RewardGoldItem);
	//	}
	//}

	//// 보상 아이템 리스트
	//B2ItemArray.Sort([](const FB2Item& lhs, const FB2Item& rhs)->bool
	//{
	//	if (lhs.StarGrade == rhs.StarGrade)
	//	{
	//		return lhs.ItemClass < rhs.ItemClass;
	//	}
	//	else
	//	{
	//		return lhs.StarGrade > rhs.StarGrade;
	//	}
	//});

	//for (FB2Item Elem : B2ItemArray)
	//{
	//	ResultItems.Add(Elem);
	//}
}

void UB2UIAutoClearResult::AddSlot(TArray<FB2Item>& InItems, int32 InRepeatCount, int32 InTotalGold)
{
	UB2UIPAutoClearResultRow* NewItem;
	if (RowSlots.IsValidIndex(InRepeatCount))
	{
		NewItem = RowSlots[InRepeatCount].Get();
		NewItem->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		NewItem = CreateWidget<UB2UIPAutoClearResultRow>(GetWorld(), AutoClearResultRowClass);
		RowSlots.Add(NewItem);
	}

	if (SB_ResultItem.IsValid())
	{
		SB_ResultItem->AddChild(NewItem);
		SB_ResultItem->SetAnimateScroll(true);
		SB_ResultItem->SetAdjustFromSlotIndex(InRepeatCount);
	}

	NewItem->Init();
	NewItem->SetInfo(InRepeatCount, InTotalGold, InItems);
}

void UB2UIAutoClearResult::CheckRepeat(bool IsMainCharLevelUp, bool IsSubCharLevelUp)
{
	if (IsRepeat == true)
	{
		if (IsMainCharLevelUp || IsSubCharLevelUp)
		{
			OpenLevelUpPopup(RepeatLevelUpDelayTime);
		}
		else
		{
			RepeatCount++;

			if (RepeatMaxCount > RepeatCount)
			{
				UWorld* TheWorld = GetWorld();
				if (TheWorld)
				{
					TheWorld->GetTimerManager().ClearTimer(SweepStagePacketTH);
					TheWorld->GetTimerManager().SetTimer(SweepStagePacketTH, this, &UB2UIAutoClearResult::RequestSweepStagePacket, DelayTime);
				}
			}
			else
			{
				OpenRepeatEndPopup();
				SetRepeat(false);
			}
		}
	}
	else
	{
		if (IsMainCharLevelUp || IsSubCharLevelUp)
		{
			OpenLevelUpPopup(LevelUpDelayTime);
		}
	}
}

void UB2UIAutoClearResult::OpenLevelUpPopup(float InDelayTime)
{
	EPCClass MainChar = SvrToCliPCClassType(ServerTypeMainChar);
	EPCClass SubChar = SvrToCliPCClassType(ServerTypeSubChar);
	if (UIP_LevelUp.IsValid() && MainChar != EPCClass::EPC_End && SubChar != EPCClass::EPC_End)
	{
		UIP_LevelUp->SetAutoAdvanceInterval(InDelayTime);
		TArray<EPCClass> ThisPcClass;
		ThisPcClass.Add(MainChar);
		ThisPcClass.Add(SubChar);
		UIP_LevelUp->Init();
		UIP_LevelUp->StartMe(ThisPcClass);
	}
}

void UB2UIAutoClearResult::LevelupFinish()
{
	if (IsRepeat)
	{
		RepeatCount++;

		if (RepeatMaxCount > RepeatCount)
		{
			RequestSweepStagePacket();
		}
		else
		{
			OpenRepeatEndPopup();
			SetRepeat(false);
		}
	}
}
