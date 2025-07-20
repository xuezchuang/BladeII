// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_ConvertItemPopup.h"
//#include "BladeII.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"

#include "B2ClientDataStore.h"
#include "CharacterDataStore.h"
#include "B2GameInstance.h"
#include "B2UIManager.h"
#include "B2UIMsgPopupReward.h"
#include "B2ItemInfo.h"
#include "B2UIDocHelper.h"
#include "Event.h"
#include "B2Button.h"
#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"

UB2LobbyUI_ConvertItemPopup::UB2LobbyUI_ConvertItemPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	tableID = 0;
	beforeConvertItemQuantity = 1;
	requiredBeforeConvertItemQuantity = 1;
	resultAfterConvertItemQuantity = 1;
	ElementSize = 0.1;
	//goldPrice = 1;
	//crystalPrice = 1;
	StartTimeTick = false;
	NextRenewalTime = 0;

	CachedDataStore = &BladeIIGameImpl::GetClientDataStore();
}

void UB2LobbyUI_ConvertItemPopup::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_AskForConverting);
	GET_SLOT(UTextBlock, TB_BeforeConvertQuantity);
	GET_SLOT(UTextBlock, TB_AfterConvertQuantity);
	GET_SLOT(UTextBlock, TB_GoldConvertLimit);
	GET_SLOT(UTextBlock, TB_MaxGoldConvertCount);
	GET_SLOT(UTextBlock, TB_CrystalConvertLimit);
	GET_SLOT(UTextBlock, TB_MaxCrystalConvertCount);
	GET_SLOT(UTextBlock, TB_GoldPrice);
	GET_SLOT(UTextBlock, TB_CrystalPrice);

	GET_SLOT(UB2Button, BTN_Cancel);
	GET_SLOT(UB2Button, BTN_PayGold);
	GET_SLOT(UB2Button, BTN_PayCrystal);

	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_BeforeConvertInfo);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_AfterConvertInfo);
	GET_SLOT(UImage, IMG_BeforeConvert);
	GET_SLOT(UImage, IMG_AfterConvert);
	GET_SLOT(UImage, IMG_Arrow);

	GET_SLOT(UImage, IMG_Gold);
	GET_SLOT(UImage, IMG_Crystal);

	GET_SLOT(UVerticalBox, V_Content);

	BIND_CLICK_FUNC_TO_BTN(BTN_PayGold, &UB2LobbyUI_ConvertItemPopup::OnClickPayGoldBtn)
	BIND_CLICK_FUNC_TO_BTN(BTN_PayCrystal, &UB2LobbyUI_ConvertItemPopup::OnClickPayCrystalBtn)
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2LobbyUI_ConvertItemPopup::OnClickCancelBtn)
}

void UB2LobbyUI_ConvertItemPopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Super::NativeTick(MyGeometry, InDeltaTime);

	//UpdateRemainTime();
}

void UB2LobbyUI_ConvertItemPopup::SetConvertTableData(int32 beforeConvertItemID_, int32 goldConvertCount, int32 crystalConvertCount)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	FConvertTableInfo ConvertTable;
	FConvertCostInfo ConvertCost;
	tableID = CliDataStore.GetConvertTable(beforeConvertItemID_, ConvertTable);
	if (tableID > 0)
	{
		bool isSuccess = CliDataStore.GetConvertCost(tableID, BladeIIGameImpl::GetClientDataStore().GetMaxGoldConvertCount() - goldConvertCount + 1, ConvertCost);
		if (isSuccess)
			goldPrice = ConvertCost.goldPrice;

		isSuccess = CliDataStore.GetConvertCost(tableID, BladeIIGameImpl::GetClientDataStore().GetMaxCrystalConvertCount() - crystalConvertCount + 1, ConvertCost);
		if (isSuccess)
			crystalPrice = ConvertCost.crystalPrice;

		requiredBeforeConvertItemQuantity = ConvertTable.beforeConvertItemQuantity;
		afterconvertItemID = ConvertTable.afterConvertItemID;
		resultAfterConvertItemQuantity = ConvertTable.afterConvertItemQuantity;
	}
}

void UB2LobbyUI_ConvertItemPopup::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	UpdateStaticWidgets();
}

void UB2LobbyUI_ConvertItemPopup::UpdateStaticWidgets()
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_MaterialConversion")));
	}
	if (TB_AskForConverting.IsValid())
	{
		TB_AskForConverting->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ConversionQuestion")));
	}
}

void UB2LobbyUI_ConvertItemPopup::UpdateDynamicWidgets()
{
	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	
	//扁裙俊辑 榜靛 背券 (榜靛函券 力茄 箭磊 菊俊 嘿绰 臂蓖)俊 措茄 巩磊凯 捞抚阑 罐酒客具 窃.
	if (TB_GoldConvertLimit.IsValid())
		TB_GoldConvertLimit->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("convertgold_text")));

	//扁裙俊辑 促捞酒 背券 (促捞酒函券 力茄 箭磊 菊俊 嘿绰 臂蓖)俊 措茄 巩磊凯 捞抚阑 罐酒客具 窃.
	if (TB_CrystalConvertLimit.IsValid())
		TB_CrystalConvertLimit->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("convertgem_text")));
	

	if (BTN_Cancel.IsValid())
		BTN_Cancel->SetVisibility(ESlateVisibility::Visible); // 弊成 搬苞 烹瘤绰 cancel 滚瓢捞 鞘夸绝澜.

	//函券 傈 酒捞袍 捞固瘤 棺 沥焊
	if (IMG_BeforeConvert.IsValid())
	{
		FSingleItemInfoData* BeforeConvertItemInfo = ItemInfoTable ? ItemInfoTable->GetInfoData(ItemBeforeConvert->ItemRefID) : NULL;
		if (BeforeConvertItemInfo)
		{
			IMG_BeforeConvert->SetBrushFromMaterial(BeforeConvertItemInfo->GetIconMaterial(ItemInfoTable));
			if (BTN_BeforeConvertInfo.IsValid())
			{
				BTN_BeforeConvertInfo->SetItemInfo(ItemBeforeConvert->ItemRefID);
			}
		}
	}

	//函券 饶 酒捞袍 捞固瘤 棺 沥焊
	if (IMG_AfterConvert.IsValid())
	{
		FSingleItemInfoData* AfterConvertItemInfo = ItemInfoTable ? ItemInfoTable->GetInfoData(afterconvertItemID) : NULL;
		if (AfterConvertItemInfo)
		{
			IMG_AfterConvert->SetBrushFromMaterial(AfterConvertItemInfo->GetIconMaterial(ItemInfoTable));
			if (BTN_AfterConvertInfo.IsValid())
			{
				BTN_AfterConvertInfo->SetItemInfo(afterconvertItemID);
			}
		}
	}
}

void UB2LobbyUI_ConvertItemPopup::UpdateConvertItemWidgets()
{
	//函券 傈 酒捞袍 肮荐
	if (TB_BeforeConvertQuantity.IsValid())
		TB_BeforeConvertQuantity->SetText(FText::AsNumber(requiredBeforeConvertItemQuantity));

	//函券 饶 酒捞袍 肮荐
	if (TB_AfterConvertQuantity.IsValid())
		TB_AfterConvertQuantity->SetText(FText::AsNumber(resultAfterConvertItemQuantity));

	//榜靛 啊拜
	if (TB_GoldPrice.IsValid())
		TB_GoldPrice->SetText(FText::AsNumber(goldPrice));

	//促捞酒 啊拜
	if (TB_CrystalPrice.IsValid())
		TB_CrystalPrice->SetText(FText::AsNumber(crystalPrice));
}

template<class T>
void UB2LobbyUI_ConvertItemPopup::ResponseConvertData(const T& ConvertDataPtr)
{
	/*
	if (auto* DocConvert = UB2UIDocHelper::GetDocConvert())
	{
		DocConvert->SetConvertRemainCount(ConvertDataPtr->exchange_remain_count);

		DocConvert->SetConvertMaxCount(ConvertDataPtr->exchange_max_count);

		//DocConvert->SetConvertRemainTime(ConvertDataPtr->next_exchange_remain_time);	//绢叼辑 鞘夸茄 扒瘤 葛福摆澜

		DocConvert->ResetConvertItemList();
		for (auto ConvertTable : ConvertDataPtr->convert_table)
		{
			DocConvert->SetConvertItemList(ConvertTable);
		}
	}
	*/
}

void UB2LobbyUI_ConvertItemPopup::SetGoldCountInfo(int32 LastCount, int32 MaxCount)
{
	if (TB_MaxGoldConvertCount.IsValid())// (榜靛 函券 冉荐 / MAX)
	{
		if (MaxCount > 0)
			TB_MaxGoldConvertCount->SetText(FText::FromString("(" + FString::FromInt(LastCount) + "/" + FString::FromInt(MaxCount) + ")"));
		else
			TB_MaxGoldConvertCount->SetText(FText::GetEmpty());

		if (MaxCount > 0 && LastCount == 0)
		{
			TB_MaxGoldConvertCount->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
			TB_GoldConvertLimit->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
			BTN_PayGold->SetIsEnabled(false);
		}
		else
		{
			TB_MaxGoldConvertCount->SetColorAndOpacity(FLinearColor::White);
			TB_GoldConvertLimit->SetColorAndOpacity(FLinearColor::White);
			BTN_PayGold->SetIsEnabled(true);
		}
	}
}

void UB2LobbyUI_ConvertItemPopup::SetCrystalCountInfo(int32 LastCount, int32 MaxCount)
{
	if (TB_MaxCrystalConvertCount.IsValid())// (促捞酒 函券 冉荐 / MAX)
	{
		if (MaxCount > 0)
			TB_MaxCrystalConvertCount->SetText(FText::FromString("(" + FString::FromInt(LastCount) + "/" + FString::FromInt(MaxCount) + ")"));
		else
			TB_MaxCrystalConvertCount->SetText(FText::GetEmpty());

		if (MaxCount > 0 && LastCount == 0)
		{
			TB_MaxCrystalConvertCount->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
			TB_CrystalConvertLimit->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
			BTN_PayCrystal->SetIsEnabled(false);
		}
		else
		{
			TB_MaxCrystalConvertCount->SetColorAndOpacity(FLinearColor::White);
			TB_CrystalConvertLimit->SetColorAndOpacity(FLinearColor::White);
			BTN_PayCrystal->SetIsEnabled(true);
		}
	}
}

void UB2LobbyUI_ConvertItemPopup::PopupMain(int32 convertItemID, int32 convertGold, int32 convertCrystal, const FConvertItemPopupOnClick& InCancelBtnHandler)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ConvertItemPopup_PopupMain);
	OnClickCancelBtnHandler = InCancelBtnHandler;

	V_Content->SetVisibility(ESlateVisibility::Visible);
	IMG_Arrow->SetVisibility(ESlateVisibility::Visible);

	BTN_PayGold->SetIsEnabled(true);
	BTN_PayCrystal->SetIsEnabled(true);

	SetConvertInfo(convertItemID, convertGold, convertCrystal);	//咯扁辑 鞘夸茄芭 促 汲沥秦 临 巴烙.
	if (ItemBeforeConvert == NULL)
	{
		CloseMe();
		return;
	}

	SetGoldCountInfo(convertGold, BladeIIGameImpl::GetClientDataStore().GetMaxGoldConvertCount());
	SetCrystalCountInfo(convertCrystal, BladeIIGameImpl::GetClientDataStore().GetMaxCrystalConvertCount());

	UpdateDynamicWidgets();
	UpdateConvertItemWidgets();

	//酒抗 鞘夸茄 父怒 绝栏搁 葛电 滚瓢 厚劝己拳
	if (requiredBeforeConvertItemQuantity <= beforeConvertItemQuantity)
	{
		//BTN_PayCrystal->SetIsEnabled(true);
		//BTN_PayGold->SetIsEnabled(true);
		TB_BeforeConvertQuantity->SetColorAndOpacity(FLinearColor::White);
	}
	else
	{
		TB_BeforeConvertQuantity->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
		BTN_PayCrystal->SetIsEnabled(false);
		BTN_PayGold->SetIsEnabled(false);
	}

	beforeconvertItemID_ref = ItemBeforeConvert->ItemRefID;

	//SetRemainTime();
	V_Content->SetVisibility(ESlateVisibility::Visible);
	IMG_Arrow->SetVisibility(ESlateVisibility::Visible);
}

void UB2LobbyUI_ConvertItemPopup::PopupAsConvertResult(const int32 AcquiredGold)
{
	//搬苞 免仿 饶 PopupMain贸烦 悼老窍霸 秦 拎具 窃.

	UpdateDynamicWidgets();
	UpdateConvertItemWidgets();
}

void UB2LobbyUI_ConvertItemPopup::SetConvertInfo(int32 beforeConvertItem, int32 goldConvertCount, int32 crystalConvertCount)
{
	beforeconvertItemID = beforeConvertItem;
	ItemBeforeConvert = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().FindItem(beforeconvertItemID);

	if (ItemBeforeConvert == nullptr)
	{
		UE_LOG(LogBladeII, Error, TEXT("Unable to find Item on [SetConvertInfo]."));
		return;
	}

	beforeConvertItemQuantity = ItemBeforeConvert->ConsumingAmount;	//泅犁 啊柳 酒捞袍 肮荐

	//抛捞喉俊辑 函券 单捞磐甫 佬绢辑 单捞磐 棺 啊拜俊 嘎霸 荐沥窍档废 秦具 窃
	SetConvertTableData(ItemBeforeConvert->ItemRefID, goldConvertCount, crystalConvertCount);
}

void UB2LobbyUI_ConvertItemPopup::SetGoldCost(EStoreItemCost InCostType, int32 price, int32 convertCount)
{
	auto UserInfo = UB2UIDocHelper::GetDocUser();
	
	if (TB_GoldPrice.IsValid())
	{
		TB_GoldPrice->SetColorAndOpacity(price <= UserInfo->GetGold() ? FLinearColor::White : UB2UIManager::GetInstance()->TextColor_Decrease);	//祸 汲沥. 林绢瘤绰 啊拜苞 家瘤陛阑 厚背窍咯具 茄促.

		auto* HBSlot = Cast<UHorizontalBoxSlot>(TB_GoldPrice->Slot);
		if (HBSlot)
			HBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
	}
}
void UB2LobbyUI_ConvertItemPopup::SetCrystalCost(EStoreItemCost InCostType, int32 price, int32 convertCount)
{
	auto UserInfo = UB2UIDocHelper::GetDocUser();

	if (TB_CrystalPrice.IsValid())
	{
		TB_GoldPrice->SetColorAndOpacity(price <= UserInfo->GetGem() ? FLinearColor::White : UB2UIManager::GetInstance()->TextColor_Decrease);	//祸 汲沥. 林绢瘤绰 啊拜苞 家瘤陛阑 厚背窍咯具 茄促.

		auto* HBSlot = Cast<UHorizontalBoxSlot>(TB_CrystalPrice->Slot);
		if (HBSlot)
			HBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
	}
}


void UB2LobbyUI_ConvertItemPopup::OnClickPayGoldBtn()
{
	LobbyReqConvertItemClass<int32, int32>::GetInstance().Signal(tableID, 2);
}

void UB2LobbyUI_ConvertItemPopup::OnClickPayCrystalBtn()
{
	LobbyReqConvertItemClass<int32, int32>::GetInstance().Signal(tableID,1);
}

void UB2LobbyUI_ConvertItemPopup::CloseWidgetDelegate()
{
	OnClickCancelBtn();
}

void UB2LobbyUI_ConvertItemPopup::OnClickCancelBtn()
{
	OnClickCancelBtnHandler.ExecuteIfBound();

	CloseMe();
}

void UB2LobbyUI_ConvertItemPopup::CloseMe()
{
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ConvertItemPopup);
}

void UB2LobbyUI_ConvertItemPopup::SetRemainTime()
{
	if (auto* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int64 NextTime = DocStore->GetNextFreeRenewalTime();
		NextRenewalTime = FDateTime::FromUnixTimestamp(NextTime / 1000);

		StartTimeTick = true;
	}
}

void UB2LobbyUI_ConvertItemPopup::UpdateRemainTime()
{
	if (!StartTimeTick)
		return;

	FTimespan RemainTime = NextRenewalTime - UB2GameInstance::GetUTCNow();

	if (RemainTime.GetTicks() < 0 && StartTimeTick)
	{
		StartTimeTick = false;

		LobbyReqConvertDataClass<int32>::GetInstance().Signal(beforeconvertItemID);
	}
}

void UB2LobbyUI_ConvertItemPopup::ResponseConvertItem(int32 itemid)
{
	auto* UIManager = UB2UIManager::GetInstance();

	auto RewardPopup = UIManager->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
	RewardPopup->AddSingleRewardItem(itemid, resultAfterConvertItemQuantity);
	RewardPopup->InitRewardMsgPopupText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("converssuccss_title")), FText::FromString(" "));
	RewardPopup->UpdateDescriptionText(FText::FromString(" "));
	RewardPopup->ShowRewardPopup(true);

}