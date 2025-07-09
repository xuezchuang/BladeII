// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIStoreItemSlot.h"
#include "B2UIPackageStore.h"
#include "B2UIPackageTypePanel.h"
#include "Retailer.h"
#include "B2UIRewardEventSet.h"
#include "B2UIDocStore.h"
#include "B2UIDocHelper.h"
#include "B2ItemInfo.h"
#include "B2UIMsgPopupReward.h"
#include "BladeIIGameImpl.h"
#include "B2UIStoreEventLabel.h"
#include "B2UIStoreItemSlotIcon.h"
#include "B2PlatformAPI.h"
//#include "B2IAPGenericPlatform.h"
#include "B2GameInstance.h"

UB2UIPackageTypePanel::UB2UIPackageTypePanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentStoreCategory = -1;
}

void UB2UIPackageTypePanel::CacheAssets()
{
	Super::CacheAssets();
}

void UB2UIPackageTypePanel::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIPackageTypePanel::Init()
{
	Super::Init();
}

void UB2UIPackageTypePanel::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIPackageTypePanel::InitPanelInfo(int32 nType)
{
	CurrentStoreCategory = nType;
}

void UB2UIPackageTypePanel::UpdatePanelInfo()
{

}

void UB2UIPackageTypePanel::DoMarkRedDotPanel()
{

}

ESTORE_SLOT_WIDGET_TYPE UB2UIPackageTypePanel::GetProductWidget(int32 PackageID)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductWidget(PackageID);

	return ESTORE_SLOT_WIDGET_TYPE::NONE;
}

UMaterialInstance* UB2UIPackageTypePanel::GetProductIcon(int32 PackageID, int32 OptionalItemId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductIcon(PackageID, OptionalItemId);

	return nullptr;
}

ESTORE_SLOT_EFFECT_TYPE UB2UIPackageTypePanel::GetProductIconEffect(int32 ProductId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductIconEffect(ProductId);

	return ESTORE_SLOT_EFFECT_TYPE::NONE;
}

FText UB2UIPackageTypePanel::GetProductName(int32 PackageID, int32 OptionalItemId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductName(PackageID, OptionalItemId);

	return FText::FromString(TEXT("Unknown Product"));
}

void UB2UIPackageTypePanel::GetBonusIconAndName(EStoreItemType BonusType, UMaterialInstance*& OutBonusIcon, FText& OutBonusName, int32 OptionalItemTemplateId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		ProductData->GetBonusIconAndName(BonusType, OutBonusIcon, OutBonusName, OptionalItemTemplateId);
}

static UB2UIStoreProductCost* GetCostToBuyUIP(UB2UIWidgetBase* InCostToBuyOwnerUIP)
{
	if (InCostToBuyOwnerUIP)
	{
		// 네이티브 UIWidgetBase 클래스로 그냥 만들어서 쓰는 게 있음.. ㅡㅡ
		return InCostToBuyOwnerUIP->GetSlot<UB2UIStoreProductCost>(TEXT("UIP_CostToBuy"));
	}
	return nullptr;
}

ESTORE_BUYPOPUP_WIDGET_TYPE UB2UIPackageTypePanel::GetPopupWidgetType(int32 PackageID)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetPopupWidgetType(PackageID);

	return ESTORE_BUYPOPUP_WIDGET_TYPE::END;
}

EPACKAGE_TYPE UB2UIPackageTypePanel::GetPackageType(int32 PackageID)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetPackageType(PackageID);

	return EPACKAGE_TYPE::None;
}

///////////////////////////////////////////////////////////////////
//                    UB2UIPackageTypePanel_Basic
///////////////////////////////////////////////////////////////////
UB2UIPackageTypePanel_Basic::UB2UIPackageTypePanel_Basic(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2UIPackageTypePanel_Basic::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_Banner);
	GET_SLOT(UHorizontalBox, HB_ItemList);
	GET_SLOT(UB2ScrollBox, SB_List);
}

void UB2UIPackageTypePanel_Basic::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIPackageTypePanel_Basic::Init()
{
	Super::Init();
}

void UB2UIPackageTypePanel_Basic::InitPanelInfo(int32 nType)
{
	Super::InitPanelInfo(nType);

	if (SB_List.IsValid())
		SB_List->SetScrollOffset(0.f);
}

void UB2UIPackageTypePanel_Basic::UpdatePanelInfo()
{
	// 여기서 포지션값.. 미리 셋
	if (HB_ItemList.IsValid())
	{
		HB_ItemList->ClearChildren();

		TArray<FStoreProductData> ProductList;
		BladeIIGameImpl::GetClientDataStore().GetPackageList(GetCurrentStoreCategory(), ProductList);

		for (auto& Elem : ProductList)
		{
			auto* CreatedWidget = CreateWidget<UB2UIStoreItemSlot>(GetOwningPlayer(), DefaultItemSlot);
			if (CreatedWidget != NULL)
			{
				CreatedWidget->Init();
				// Limit Sale
				int64 StartTime = 0;
				int64 EndTime = 0;

				if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
				{
					bool IsLimited = DocStore->GetProductSalePeriod(Elem.StoreProductId, StartTime, EndTime);

					if (IsLimited && !UB2GameInstance::IncludeTimeNow(StartTime, EndTime))
						continue;

					CreatedWidget->SetSalePeriodData(IsLimited, EndTime);
				}

				// Product
				int32 PackageID = Elem.Product.ProductId;
				int32 SlotWidgetNum = static_cast<uint32>(GetProductWidget(PackageID));
				int32 SlotIconEffectNum = static_cast<uint32>(GetProductIconEffect(PackageID));
				CreatedWidget->CreateSlotWidget(SlotWidgetNum, GetProductIcon(PackageID, PackageID), SlotIconEffectNum);
				CreatedWidget->SetData(Elem.StoreProductId,
					GetProductIcon(PackageID, PackageID),
					GetProductName(PackageID, PackageID),
					Elem.Product.ProductCount,
					true);

				// Bonus
				UMaterialInstance* BonusIcon = nullptr;
				FText BonusName;
				GetBonusIconAndName(static_cast<EStoreItemType>(Elem.Bonus.ProductType), BonusIcon, BonusName, Elem.Bonus.ProductId);
				CreatedWidget->SetBonusData(Elem.Bonus.ProductCount > 0, BonusIcon, BonusName, Elem.Bonus.ProductCount, Elem.CalculatedBonusCount);

				// Restrict
				CreatedWidget->SetRestrictData(Elem.StoreProductId, Elem.RestrictType, Elem.RestrictCount);

				// Product Mark
				CreatedWidget->SetPackageProductMark(Elem.StoreProductId);

				// Period - 부스트 패키지 전용. 남은기간 표시.
				CreatedWidget->SetPeriodData(PackageID);

				// Cost
				CreatedWidget->SetCost(static_cast<EStoreItemCost>(Elem.CostType), Elem.Cost, Elem.CalculatedPrice);

				auto* AddedSlot = Cast<UHorizontalBoxSlot>(HB_ItemList->AddChild(CreatedWidget));
				check(AddedSlot);
			}
		}
	}
	// 포지션값 복구

	SetBannerInfo();
}

void UB2UIPackageTypePanel_Basic::SetBannerInfo()
{
	if (WS_Banner.IsValid())
		WS_Banner->SetActiveWidgetIndex(GetCurrentStoreCategory() - 1);
}

void UB2UIPackageTypePanel_Basic::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

///////////////////////////////////////////////////////////////////
//                    UB2UIPackageTypePanel_Flatrate
///////////////////////////////////////////////////////////////////
UB2UIPackageTypePanel_Flatrate::UB2UIPackageTypePanel_Flatrate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIPackageTypePanel_Flatrate::Init()
{
	Super::Init();

	DeliveryResponseFlatratePackageBonusTicket = DeliveryResponseRewardFlatRatePackageBonusClass<FB2ResponseRewardFlatRatePackageBonusPtr>::GetInstance().Subscribe(
		[this](FB2ResponseRewardFlatRatePackageBonusPtr Ptr)
	{
		this->OnReciveRewardFlatRatePackageBonus(Ptr);
	});

	if(WS_Info.IsValid())
		WS_Info->SetVisibility(B2P_IsInReview() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);	//IOS 검수일때 숨겨줌
}

void UB2UIPackageTypePanel_Flatrate::DestroySelf(UB2UIManager* InUIManager)
{
	DeliveryResponseRewardFlatRatePackageBonusClass<FB2ResponseRewardFlatRatePackageBonusPtr>::GetInstance().Unsubscribe(DeliveryResponseFlatratePackageBonusTicket);

	Super::DestroySelf(InUIManager);
}

void UB2UIPackageTypePanel_Flatrate::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UHorizontalBox, HB_FlatrateList);

	GET_SLOT(UTextBlock, STB_FlatrateTitle);
	GET_SLOT(UTextBlock, STB_FlatrateDesc);

	GET_SLOT(UHorizontalBox, HB_FlatrateReward);
	GET_SLOT(UCheckBox, CB_Weekly);
	GET_SLOT(UTextBlock, STB_Weekly);
	GET_SLOT(UTextBlock, TB_Weekly);
	GET_SLOT(UCheckBox, CB_Monthly);
	GET_SLOT(UTextBlock, STB_Monthly);
	GET_SLOT(UTextBlock, TB_Monthly);
	GET_SLOT(UB2UIRewardIcon, UIP_FlatrateReward);
	if (UIP_FlatrateReward.IsValid())
	{
		UIP_FlatrateReward->Init();
	}

	GET_SLOT(UButton, BTN_GetReward);
	GET_SLOT(UB2RichTextBlock, RTB_ReceiveBtn);
	GET_SLOT(UTextBlock, STB_Complete);

	GET_SLOT(UB2RichTextBlock, RTB_WarningSummary);
	GET_SLOT(UB2RichTextBlock, RTB_FlatrateSummary);
	GET_SLOT(UButton, BTN_Detail);
	GET_SLOT(UTextBlock, STB_Detail);

	GET_SLOT(UWidgetSwitcher, WS_Info);
	GET_SLOT(UCanvasPanel, CP_NormalInfo);
	GET_SLOT(UCanvasPanel, CP_MoreAdventage);

	GET_SLOT(UButton, BTN_SummaryDetail);
	GET_SLOT(UB2RichTextBlock, RTB_SummaryInfo);
	GET_SLOT(UTextBlock, STB_SummaryDetail);
	GET_SLOT(UTextBlock, STB_Summary);
}

void UB2UIPackageTypePanel_Flatrate::UpdateStaticText()
{
	if (STB_FlatrateTitle.IsValid())
		STB_FlatrateTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Flatrate_Title")));

	if (STB_FlatrateDesc.IsValid())
		STB_FlatrateDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Flatrate_Desc")));

	if (STB_Weekly.IsValid())
		STB_Weekly->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Weekly")));

	if (STB_Monthly.IsValid())
		STB_Monthly->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Monthly")));

	if (STB_Complete.IsValid())
		STB_Complete->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_FlatrateBonus_Complete")));

	if (STB_SummaryDetail.IsValid())
		STB_SummaryDetail->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Flatrate_Detail")));

	if (STB_Detail.IsValid())
		STB_Detail->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Flatrate_Detail")));

	if (RTB_SummaryInfo.IsValid())
		RTB_SummaryInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Terms_Text")));

	if (STB_Summary.IsValid())
		STB_Summary->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_WarningSummury")));

	if (RTB_WarningSummary.IsValid())
		RTB_WarningSummary->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Terms_Text")));

	if (RTB_FlatrateSummary.IsValid())
		RTB_FlatrateSummary->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_WarningSummury")));
}

void UB2UIPackageTypePanel_Flatrate::BindDelegates()
{
	Super::BindDelegates();
	
	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIPackageTypePanel_Flatrate::OnClickBTN_GetReward);
	BIND_CLICK_FUNC_TO_BTN(BTN_Detail, &UB2UIPackageTypePanel_Flatrate::OnClickBTN_Detail);
	BIND_CLICK_FUNC_TO_BTN(BTN_SummaryDetail, &UB2UIPackageTypePanel_Flatrate::OnClickBTN_Detail);

	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_GetReward.Get()), &UB2UIPackageTypePanel_Flatrate::RedDotCondition_FlatRate);
}

void UB2UIPackageTypePanel_Flatrate::InitPanelInfo(int32 nType)
{
	Super::InitPanelInfo(nType);
}

void UB2UIPackageTypePanel_Flatrate::UpdatePanelInfo()
{
	TArray<FStoreProductData> ProductList;
	BladeIIGameImpl::GetClientDataStore().GetPackageList(GetCurrentStoreCategory(), ProductList);

	HB_FlatrateList->ClearChildren();

	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		FPackageFlatrateBonusState BonusState = DocStore->GetReceivedFlatratePackageBonusState();

		for (auto& Elem : ProductList)
		{
			auto* CreatedWidget = CreateWidget<class UB2UIPacakgeFlatrateItem>(GetOwningPlayer(), FlatRateItem);
			if (CreatedWidget != NULL)
			{
				TArray<FPackageProductData> PackageProductData;
				int32 PackageID = Elem.Product.ProductId;
				BladeIIGameImpl::GetClientDataStore().GetPackageProductList(PackageID, PackageProductData);

				UHorizontalBoxSlot* HBSlot = Cast<UHorizontalBoxSlot>(HB_FlatrateList->AddChild(CreatedWidget));
				HBSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				HBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				HBSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

				CreatedWidget->Init();
				
				// Product
				int32 SlotIconEffectNum = static_cast<uint32>(GetProductIconEffect(PackageID));
				CreatedWidget->SetFlatrateSlotWidget(GetProductIcon(PackageID, PackageID), SlotIconEffectNum);
				if (PackageProductData.IsValidIndex(0))
					CreatedWidget->SetFlatrateItemData(Elem, PackageProductData[0], GetPackageType(PackageID));
				CreatedWidget->BuyDelegate.BindUObject(this, &UB2UIPackageTypePanel_Flatrate::BuyFlatRateItem);

				// Period
				CreatedWidget->SetPeriodData(PackageID);

				// Product Mark
				CreatedWidget->SetPackageProductMark(Elem.StoreProductId);
			}
		}

		// 기간패키지가 2개가 아닐때 가위젯을 만들어서 와꾸맞춰줌.
		if (ProductList.Num() < 2)
		{
			for (int32 i = 0; i < (2 - ProductList.Num()); i++)
			{
				auto* CreatedWidget = CreateWidget<class UB2UIPacakgeFlatrateItem>(GetOwningPlayer(), FlatRateItem);
				if (CreatedWidget != NULL)
				{
					UHorizontalBoxSlot* HBSlot = Cast<UHorizontalBoxSlot>(HB_FlatrateList->AddChild(CreatedWidget));
					HBSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
					HBSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					HBSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
					CreatedWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}

		FText TempText;
		if (CB_Weekly.IsValid())
			CB_Weekly->SetCheckedState(BonusState.WeeklyState ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		if (TB_Weekly.IsValid())
		{
			if (BonusState.WeeklyState)
			{
				TempText = BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Complete"));
			}
			else
			{
				TempText = BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_InComplete"));
			}
			TB_Weekly->SetText(TempText);
		}

		if (CB_Monthly.IsValid())
			CB_Monthly->SetCheckedState(BonusState.MonthlyState ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		if (TB_Monthly.IsValid())
		{
			if (BonusState.MonthlyState)
			{
				TempText = BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Complete"));
			}
			else
			{
				TempText = BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_InComplete"));
			}
			TB_Monthly->SetText(TempText);
		}

		if (BonusState.WeeklyState && BonusState.MonthlyState && BonusState.RewardState)
		{
			if (STB_Complete.IsValid())
				STB_Complete->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (HB_FlatrateReward.IsValid())
				HB_FlatrateReward->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			if (STB_Complete.IsValid())
				STB_Complete->SetVisibility(ESlateVisibility::Collapsed);

			if (HB_FlatrateReward.IsValid())
				HB_FlatrateReward->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		SetBonusIcon();
		SetRewardButton();
	}
}

void UB2UIPackageTypePanel_Flatrate::DoMarkRedDotPanel()
{
	DoMarkRedDot();
}

void UB2UIPackageTypePanel_Flatrate::SetBonusIcon()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		FPackageFlatrateBonusState BonusState = DocStore->GetReceivedFlatratePackageBonusState();

		TArray<FB2ShopFlatRatePackageBonusPtr> FlatRatePackageBonusList;
		BladeIIGameImpl::GetClientDataStore().GetFlatratePackageBonusList(BonusState.CurrentIndex, FlatRatePackageBonusList);

		for (auto& Elem : FlatRatePackageBonusList)
		{
			if (UIP_FlatrateReward.IsValid())
			{
				UIP_FlatrateReward->UpdateRewardIcon(Elem->reward_id, Elem->reward_count);
			}
		}
	}
}

void UB2UIPackageTypePanel_Flatrate::SetRewardButton()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		FPackageFlatrateBonusState BonusState = DocStore->GetReceivedFlatratePackageBonusState();

		// 보상 받음
		if (BonusState.RewardState)
		{
			if (BTN_GetReward.IsValid())
				BTN_GetReward->SetIsEnabled(false);
			if (RTB_ReceiveBtn.IsValid())
				RTB_ReceiveBtn->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			// 보상 받을 수 있음
			if (BonusState.WeeklyState && BonusState.MonthlyState)
			{
				if (BTN_GetReward.IsValid())
				{
					BTN_GetReward->SetVisibility(ESlateVisibility::Visible);
					BTN_GetReward->SetIsEnabled(true);
				}
			}
			else
			{
				// 보상 받을 수 없음
				if (BTN_GetReward.IsValid())
					BTN_GetReward->SetIsEnabled(false);
			}

			if (RTB_ReceiveBtn.IsValid())
			{
				RTB_ReceiveBtn->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				RTB_ReceiveBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Flatrate_Complete")));
			}
		}
	}
}

void UB2UIPackageTypePanel_Flatrate::OnReciveRewardFlatRatePackageBonus(FB2ResponseRewardFlatRatePackageBonusPtr Ptr)
{
	auto* RewardMailPopUp = UB2UIManager::GetInstance()->OpenRewardMailPopUp(Ptr->received_reward);
	if (RewardMailPopUp == nullptr)
	{
		if (UB2UIMsgPopupReward* RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty()))
		{
			if (RewardPopup)
			{
				RewardPopup->AddSingleRewardItem(Ptr->received_reward);

				// RewardPopup에서 ClientDataStore::AccountInfo에 보상 강제 적용
				RewardPopup->ShowRewardPopup(true);
				RewardPopup->ShowMailDescription(false);
			}
		}
	}

	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		DocStore->SetReceivedFlatratePackageBonusState(Ptr->next_bonus_state);
	}

	UpdatePanelInfo();
	FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_SHOP_PACKAGE });
}

void UB2UIPackageTypePanel_Flatrate::OnClickBTN_GetReward()
{
	data_trader::Retailer::GetInstance().RequestRewardFlatRatePackageBonus();
}

void UB2UIPackageTypePanel_Flatrate::OnClickBTN_Detail()
{
	B2P_ShowOpenURL(B2P_GetKakaoCustomData(TEXT("withdSubscUrl")));
}

bool UB2UIPackageTypePanel_Flatrate::RedDotCondition_FlatRate()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Package_FlatRate();
}

void UB2UIPackageTypePanel_Flatrate::BuyFlatRateItem(int32 StoreID)
{
	ButtonOneParamDelegate.ExecuteIfBound(StoreID);
}

///////////////////////////////////////////////////////////////////
//                    UB2UIPackageTypePanel_Level
///////////////////////////////////////////////////////////////////
UB2UIPackageTypePanel_Level::UB2UIPackageTypePanel_Level(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SelectedPCClass = EPCClass::EPC_End;
}

void UB2UIPackageTypePanel_Level::CacheAssets()
{
	Super::CacheAssets();

	// All used
	TopTabs.Empty();
	TopTabs.SetNum(static_cast<int32>(EPCClass::EPC_End));
	GET_SLOT_BYNAME(UB2UICommonTab, TopTabs[static_cast<int32>(EPCClass::EPC_Gladiator)], TAB_Top_01);
	GET_SLOT_BYNAME(UB2UICommonTab, TopTabs[static_cast<int32>(EPCClass::EPC_Assassin)], TAB_Top_02);
	GET_SLOT_BYNAME(UB2UICommonTab, TopTabs[static_cast<int32>(EPCClass::EPC_Wizard)], TAB_Top_03);
	GET_SLOT_BYNAME(UB2UICommonTab, TopTabs[static_cast<int32>(EPCClass::EPC_Fighter)], TAB_Top_04);

	for (auto TabItem : TopTabs)
	{
		TabItem->Init();
	}

	GET_SLOT(UB2RichTextBlock, RTB_LevelupSummary);
	GET_SLOT(UButton, BTN_Detail);
	GET_SLOT(UTextBlock, STB_Detail);

	GET_SLOT(UB2ScrollBox, SB_LeftTab);
	GET_SLOT(UVerticalBox, VB_LeftTab);

	GET_SLOT(UWidgetSwitcher, WS_Content);

	// Levelup content
	GET_SLOT(UB2ScrollBox, SB_LevelUpList);
	GET_SLOT(UVerticalBox, VB_LevelUpList);

	GET_SLOT(UWidgetSwitcher, WS_ClassImage);

	GET_SLOT(UB2RichTextBlock, STB_ReceiveDesc);
	GET_SLOT(UB2RichTextBlock, TB_LevelPackageDesc);

	GET_SLOT(UButton, BTN_LvBuy);

	GET_SLOT(UB2UIStoreProductCost, UIP_Cost);
	if (UIP_Cost.IsValid())
		UIP_Cost->Init();

	// Reach Level (Reach)
	GET_SLOT(UTextBlock, STB_Reach_Title);
	GET_SLOT(UB2UIRewardIcon, UIP_Reach_Reward);
	if (UIP_Reach_Reward.IsValid())
		UIP_Reach_Reward->Init();
	GET_SLOT(UTextBlock, STB_Reach_Restrict);
	GET_SLOT(UTextBlock, TB_Reach_RestrictCount);

	GET_SLOT(UButton, BTN_Reach_Buy);
	GET_SLOT(UTextBlock, STB_Reach_RestrictComplete);
	GET_SLOT(UB2UIStoreProductCost, UIP_Reach_Cost);
	if (UIP_Reach_Cost.IsValid())
		UIP_Reach_Cost->Init();

	// Reach Level (Mission)
	GET_SLOT(UTextBlock, STB_Mission_Title);
	GET_SLOT(UB2UIRewardIcon, UIP_Mission_Reward);
	if (UIP_Mission_Reward.IsValid())
		UIP_Mission_Reward->Init();
	GET_SLOT(UTextBlock, TB_Mission_PeriodCount);

	GET_SLOT(UWidgetSwitcher, WS_Mission_RewardState);
	GET_SLOT(UTextBlock, STB_Mission_NeedPackage);
	GET_SLOT(UButton, BTN_Mission_Reward);
	GET_SLOT(UTextBlock, STB_Mission_Reward);
	GET_SLOT(UTextBlock, STB_Mission_CompleteReward);
	GET_SLOT(UTextBlock, STB_Mission_FailReward);

	GET_SLOT(UTextBlock, STB_Reach_LimitLevel);
	GET_SLOT(UTextBlock, TB_Reach_LimitLevel);
	GET_SLOT(UTextBlock, TB_Reach_Warning);

	GET_SLOT(UTextBlock, TB_SalePeriod);

	GET_SLOT(UB2UIStoreEventLabel, UIP_Store_Event_Label);
	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->Init();
}

void UB2UIPackageTypePanel_Level::BindDelegates()
{
	Super::BindDelegates();

	if (TopTabs[static_cast<int32>(EPCClass::EPC_Gladiator)].IsValid())
	{
		TopTabs[static_cast<int32>(EPCClass::EPC_Gladiator)]->TabButtonDelegateBindLambda(
			[this]() { OnClickTopTab(EPCClass::EPC_Gladiator);
		});

		REGISTER_REDDOT_CONDITION(TopTabs[static_cast<int32>(EPCClass::EPC_Gladiator)]->GetMyRedDotWidget(), &UB2UIPackageTypePanel_Level::RedDotCondition_Gladiator);
	}

	if (TopTabs[static_cast<int32>(EPCClass::EPC_Assassin)].IsValid())
	{
		TopTabs[static_cast<int32>(EPCClass::EPC_Assassin)]->TabButtonDelegateBindLambda(
			[this]() { OnClickTopTab(EPCClass::EPC_Assassin);
		});

		REGISTER_REDDOT_CONDITION(TopTabs[static_cast<int32>(EPCClass::EPC_Assassin)]->GetMyRedDotWidget(), &UB2UIPackageTypePanel_Level::RedDotCondition_Assassin);
	}

	if (TopTabs[static_cast<int32>(EPCClass::EPC_Wizard)].IsValid())
	{
		TopTabs[static_cast<int32>(EPCClass::EPC_Wizard)]->TabButtonDelegateBindLambda(
			[this]() { OnClickTopTab(EPCClass::EPC_Wizard);
		});

		REGISTER_REDDOT_CONDITION(TopTabs[static_cast<int32>(EPCClass::EPC_Wizard)]->GetMyRedDotWidget(), &UB2UIPackageTypePanel_Level::RedDotCondition_Wizard);
	}

	if (TopTabs[static_cast<int32>(EPCClass::EPC_Fighter)].IsValid())
	{
		TopTabs[static_cast<int32>(EPCClass::EPC_Fighter)]->TabButtonDelegateBindLambda(
			[this]() { OnClickTopTab(EPCClass::EPC_Fighter);
		});

		REGISTER_REDDOT_CONDITION(TopTabs[static_cast<int32>(EPCClass::EPC_Fighter)]->GetMyRedDotWidget(), &UB2UIPackageTypePanel_Level::RedDotCondition_Fighter);
	}

	BIND_CLICK_FUNC_TO_BTN(BTN_Detail, &UB2UIPackageTypePanel_Level::OnClickBTN_Detail);
	BIND_CLICK_FUNC_TO_BTN(BTN_LvBuy, &UB2UIPackageTypePanel_Level::OnClickBTN_LvupBuy);

	BIND_CLICK_FUNC_TO_BTN(BTN_Reach_Buy, &UB2UIPackageTypePanel_Level::OnClickBTN_ReachBuy);
	BIND_CLICK_FUNC_TO_BTN(BTN_Mission_Reward, &UB2UIPackageTypePanel_Level::OnClickBTN_MissionReward);
}

void UB2UIPackageTypePanel_Level::Init()
{
	Super::Init();

	DeliveryResponseRewardLevelUpPackageTicket = DeliveryResponseRewardLevelUpPackageClass<FB2ResponseRewardLevelUpPackagePtr>::GetInstance().Subscribe(
		[this](FB2ResponseRewardLevelUpPackagePtr ResponsetReward)
	{
		this->OnReciveLevelupReward(ResponsetReward);
	});

	DeliveryResponseRewardReachLevelPackageTicket = DeliveryResponseRewardReachLevelPackageClass<FB2ResponseRewardReachLevelPackagePtr>::GetInstance().Subscribe(
		[this](FB2ResponseRewardReachLevelPackagePtr ResponsetReward)
	{
		this->OnReciveReachLevelReward(ResponsetReward);
	});
}

void UB2UIPackageTypePanel_Level::DestroySelf(UB2UIManager* InUIManager)
{
	DeliveryResponseRewardLevelUpPackageClass<FB2ResponseRewardLevelUpPackagePtr>::GetInstance().Unsubscribe(DeliveryResponseRewardLevelUpPackageTicket);
	DeliveryResponseRewardReachLevelPackageClass<FB2ResponseRewardReachLevelPackagePtr>::GetInstance().Unsubscribe(DeliveryResponseRewardReachLevelPackageTicket);

	Super::DestroySelf(InUIManager);
}

void UB2UIPackageTypePanel_Level::UpdateStaticText()
{
	if (RTB_LevelupSummary.IsValid())
		RTB_LevelupSummary->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Flatrate_Summary")));
	if (STB_ReceiveDesc.IsValid())
		STB_ReceiveDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Receive_Immediately")));
	
	if (STB_Detail.IsValid())
		STB_Detail->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageText2")));

	if (TopTabs[static_cast<int32>(EPCClass::EPC_Gladiator)].IsValid())
		TopTabs[static_cast<int32>(EPCClass::EPC_Gladiator)]->SetTabName(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Levelup_Gladiator")));

	if (TopTabs[static_cast<int32>(EPCClass::EPC_Assassin)].IsValid())
		TopTabs[static_cast<int32>(EPCClass::EPC_Assassin)]->SetTabName(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Levelup_Assassin")));

	if (TopTabs[static_cast<int32>(EPCClass::EPC_Wizard)].IsValid())
		TopTabs[static_cast<int32>(EPCClass::EPC_Wizard)]->SetTabName(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Levelup_Wizard")));

	if (TopTabs[static_cast<int32>(EPCClass::EPC_Fighter)].IsValid())
		TopTabs[static_cast<int32>(EPCClass::EPC_Fighter)]->SetTabName(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Levelup_Fighter")));

	// Reach Level
	if (STB_Reach_Title.IsValid())
		STB_Reach_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Reach_Title")));
	if (STB_Reach_Restrict.IsValid())
		STB_Reach_Restrict->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Number")));
	if (STB_Reach_RestrictComplete.IsValid())
		STB_Reach_RestrictComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Complete")));

	if (STB_Mission_Title.IsValid())
		STB_Mission_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Mission_Title")));

	if (STB_Mission_NeedPackage.IsValid())
		STB_Mission_NeedPackage->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_LevelupBuyDesc")));
	if (STB_Mission_Reward.IsValid())
		STB_Mission_Reward->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Flatrate_Complete")));
	if (STB_Mission_CompleteReward.IsValid())
		STB_Mission_CompleteReward->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Mission_Complete")));
	if (STB_Mission_FailReward.IsValid())
		STB_Mission_FailReward->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Mission_InComplete")));

	if (STB_Reach_LimitLevel.IsValid())
		STB_Reach_LimitLevel->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_LimitLevel")));
}

void UB2UIPackageTypePanel_Level::InitPanelInfo(int32 nType)
{
	Super::InitPanelInfo(nType);

	if (TopTabs.IsValidIndex(0))
		TopTabs[0]->ForcedClickTab();
}

void UB2UIPackageTypePanel_Level::UpdatePanelInfo()
{
	if (VB_LeftTab->GetChildrenCount() > 0)
	{
		for (int Idx = 0; Idx < VB_LeftTab->GetChildrenCount(); Idx++)
		{
			UB2UICommonTab* ChildrenTab = static_cast<UB2UICommonTab*>(VB_LeftTab->GetChildAt(Idx));

			if (ChildrenTab)
			{
				if (ChildrenTab->GetTabButtonIndex() == CurrentStoreID)
				{
					ChildrenTab->ForcedClickTab();
				}
			}
		}
	}
}

void UB2UIPackageTypePanel_Level::DoMarkRedDotPanel()
{
	DoMarkRedDot();

	if (VB_LeftTab.IsValid())
	{
		for (int32 i = 0; i < VB_LeftTab->GetChildrenCount(); i++)
		{
			UB2UIPacakgeLevelupLeftTab* TabWidget = Cast<class UB2UIPacakgeLevelupLeftTab>(VB_LeftTab->GetChildAt(i));
			if (TabWidget)
			{
				TabWidget->DoMarkRedDot();
			}
		}
	}
}

void UB2UIPackageTypePanel_Level::CreateLeftTab()
{
	if (VB_LeftTab.IsValid())
	{
		VB_LeftTab->ClearChildren();

		TArray<FStoreProductData> ProductList;
		ProductList.Empty();
		BladeIIGameImpl::GetClientDataStore().GetPackageList(GetCurrentStoreCategory(), ProductList);

		for (FStoreProductData Elem : ProductList)
		{
			int32 StoreID = Elem.StoreProductId;
			int32 PackageID = Elem.Product.ProductId;

			// Level Up Package
			TArray<FB2ShopLevelUpPackagePtr> LevelupList;
			LevelupList.Empty();

			BladeIIGameImpl::GetClientDataStore().GetPackageLevelUpList(PackageID, LevelupList);
			if (LevelupList.IsValidIndex(0))
			{
				if (SvrToCliPCClassType(LevelupList[0]->character_type) == SelectedPCClass)
				{
					auto* CreatedWidget = CreateWidget<class UB2UIPacakgeLevelupLeftTab>(GetOwningPlayer(), LeftTabItem);
					if (CreatedWidget != NULL)
					{
						CreatedWidget->Init();

						// Limit Sale
						int64 StartTime = 0;
						int64 EndTime = 0;

						if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
						{
							bool IsLimited = DocStore->GetProductSalePeriod(StoreID, StartTime, EndTime);
							bool bInTime = UB2GameInstance::IncludeTimeNow(StartTime, EndTime);

							if (IsLimited && !bInTime)
								continue;

							CreatedWidget->SetLimitedTime(IsLimited);
						}

						CreatedWidget->SetTabSelected(false);
						CreatedWidget->SetIsTabEnable(true);
						CreatedWidget->SetTabName(GetLeftTabName(PackageID));
						CreatedWidget->SetTabButtonIndex(StoreID);
						CreatedWidget->SetTabStoreEventLabel(StoreID);
						CreatedWidget->SetCurrentPackageID(PackageID);
						CreatedWidget->BindReddot();
						CreatedWidget->TabButtonDelegateBindLambda(
							[this, StoreID]() { OnClickTabButtonDelegate(StoreID, ELevelPackageType::LevelPackage_Levelup);
						});

						VB_LeftTab->AddChild(CreatedWidget);
					}
				}
			}

			// Reach Level Package
			TArray<FB2ShopReachLevelPackagePtr> ReachLevelList;
			ReachLevelList.Empty();

			BladeIIGameImpl::GetClientDataStore().GetPackageReachLevelList(PackageID, ReachLevelList);
			if (ReachLevelList.IsValidIndex(0))
			{
				if (SvrToCliPCClassType(ReachLevelList[0]->character_type) == SelectedPCClass)
				{
					auto* CreatedWidget = CreateWidget<class UB2UIPacakgeLevelupLeftTab>(GetOwningPlayer(), LeftTabItem);
					if (CreatedWidget != NULL)
					{
						CreatedWidget->Init();

						CreatedWidget->SetTabSelected(false);
						CreatedWidget->SetIsTabEnable(true);
						CreatedWidget->SetTabName(GetLeftTabName(PackageID));
						CreatedWidget->SetTabButtonIndex(StoreID);
						CreatedWidget->SetTabLimitLevel(true);
						CreatedWidget->SetCurrentPackageID(PackageID, true);
						CreatedWidget->BindReddot();
						CreatedWidget->TabButtonDelegateBindLambda(
							[this, StoreID]() { OnClickTabButtonDelegate(StoreID, ELevelPackageType::LevelPackage_ReachLevel);
						});

						VB_LeftTab->AddChild(CreatedWidget);
					}
				}
			}
		}
	}

	if (SB_LeftTab.IsValid())
		SB_LeftTab->SetScrollOffset(0.f);

	UB2UICommonTab* ChildrenTab = static_cast<UB2UICommonTab*>(VB_LeftTab->GetChildAt(0));
	if (ChildrenTab)
	{
		ChildrenTab->ForcedClickTab();
	}

	DoMarkRedDotPanel();
}

void UB2UIPackageTypePanel_Level::SetSelectedLeftTab(int32 StoreID)
{
	if (VB_LeftTab->GetChildrenCount() > 0)
	{
		for (int Idx = 0; Idx < VB_LeftTab->GetChildrenCount(); Idx++)
		{
			UB2UICommonTab* ChildrenTab = static_cast<UB2UICommonTab*>(VB_LeftTab->GetChildAt(Idx));

			if (ChildrenTab)
			{
				if (ChildrenTab->GetTabButtonIndex() == StoreID)
					ChildrenTab->SetTabSelected(true);
				else
					ChildrenTab->SetTabSelected(false);
			}
		}
	}
}

FText UB2UIPackageTypePanel_Level::GetLeftTabName(int32 PackageID)
{
	FString TextString = TEXT("Store_PackageTab_") + FString::FromInt(PackageID);
	return BladeIIGetLOCText(B2LOC_CAT_STORE, TextString);
}

void UB2UIPackageTypePanel_Level::SetPackageProductMark(int32 ProductId)
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int32 PackageProductMark = DocStore->GetPackageProductMark(ProductId);

		if (UIP_Store_Event_Label.IsValid())
			UIP_Store_Event_Label->SetStoreEventLabel(PackageProductMark);
	}
}

void UB2UIPackageTypePanel_Level::OnClickTabButtonDelegate(int32 ButtonIndex, ELevelPackageType PackageType)
{
	WS_Content->SetActiveWidgetIndex(static_cast<int32>(PackageType));

	CurrentStoreID = ButtonIndex;
	auto* ProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(ButtonIndex);
	CurrentPackageID = ProductData ? ProductData->Product.ProductId : 0;

	if (PackageType == ELevelPackageType::LevelPackage_Levelup)
	{
		UpdateLevelupPanel();
	}
	else if (PackageType == ELevelPackageType::LevelPackage_ReachLevel)
	{
		UpdateReachLevelPanel();
	}

	SetSelectedLeftTab(ButtonIndex);
}

void UB2UIPackageTypePanel_Level::OnClickTopTab(EPCClass selectedClass)
{
	SelectedPCClass = selectedClass;
	ScrollOffset_LeftTab = 0.f;
	ScrollOffset_List = 0.f;

	for (int32 i = 0; i < static_cast<int32>(EPCClass::EPC_End); i++)
	{
		if (TopTabs.IsValidIndex(i))
		{
			if (i == static_cast<int32>(selectedClass))
				TopTabs[i]->SetTabSelected(true);
			else
				TopTabs[i]->SetTabSelected(false);
		}
	}

	if (WS_ClassImage.IsValid())
		WS_ClassImage->SetActiveWidgetIndex(static_cast<int32>(selectedClass));

	CreateLeftTab();
}

void UB2UIPackageTypePanel_Level::OnClickBTN_LvupBuy()
{
	ButtonOneParamDelegate.ExecuteIfBound(CurrentStoreID);
}

void UB2UIPackageTypePanel_Level::OnClickBTN_ReachBuy()
{
	TArray<FB2ShopReachLevelPackagePtr> ReachLevelList;
	ReachLevelList.Empty();

	BladeIIGameImpl::GetClientDataStore().GetPackageReachLevelList(CurrentPackageID, ReachLevelList);

	if (ReachLevelList.IsValidIndex(0))
	{
		if (ReachLevelList[0]->limit_level >= BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(SelectedPCClass))
		{
			ButtonOneParamDelegate.ExecuteIfBound(CurrentStoreID);
		}
		else
		{
			FText TempText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_NotPurchase")),
				GetPCClassLocalizedText(SelectedPCClass),
				ReachLevelList[0]->limit_level);

			UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				TempText,
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm
				);
		}
	}
}

void UB2UIPackageTypePanel_Level::OnClickBTN_MissionReward()
{
	data_trader::Retailer::GetInstance().RequestRewardReachLevelPackage(CurrentPackageID);
}

void UB2UIPackageTypePanel_Level::OnClickBTN_Detail()
{
	B2P_ShowOpenURL(B2P_GetKakaoCustomData(TEXT("withdSubscUrl")));
}

void UB2UIPackageTypePanel_Level::UpdateLevelupPanel()
{
	auto* ProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(CurrentStoreID);

	TArray<FB2ShopLevelUpPackagePtr> LevelupList;
	LevelupList.Empty();

	BladeIIGameImpl::GetClientDataStore().GetPackageLevelUpList(CurrentPackageID, LevelupList);

	int32 TempLevel = 0;
	int32 nTotalGem = 0;

	for (auto Levelupitem : LevelupList)
	{
		nTotalGem += Levelupitem->reward_count;
		TempLevel = Levelupitem->level;
	}

	if (TB_LevelPackageDesc.IsValid())
	{
		FText TabName = FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_LevelupDesc")), GetLeftTabName(CurrentPackageID), FText::AsNumber(nTotalGem));
		TB_LevelPackageDesc->SetText(TabName);
	}

	bool bIsBuy = false;
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		FPackageLevelupMission* CurrentMission = DocStore->GetReceivedLevelupPacakgeMission(CurrentPackageID, TempLevel);
		if (CurrentMission != nullptr)
			bIsBuy = true;
	}

	if (BTN_LvBuy.IsValid())
	{
		BTN_LvBuy->SetIsEnabled(!bIsBuy);
	}

	if (ProductData)
	{
		if (UIP_Cost.IsValid())
		{
			EStoreItemCost eCostType = bIsBuy ? EStoreItemCost::End : static_cast<EStoreItemCost>(ProductData->CostType);

			UIP_Cost->SetIsEnabled(!bIsBuy);

			if (eCostType == EStoreItemCost::Cash)
			{
				//B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(CurrentStoreID);
				//if (ProductInfo)
				//	UIP_Cost->SetCashCost(ProductInfo->Price, ProductInfo->CurrencyCode);
			}
			else
			{
				UIP_Cost->SetCost(eCostType, ProductData->Cost, ProductData->CalculatedPrice);
			}
		}
	}
	
	UpdateLevelupPackageLIst(LevelupList);

	if (SB_LeftTab.IsValid())
		SB_LeftTab->SetScrollOffset(ScrollOffset_LeftTab);
	if (SB_LevelUpList.IsValid())
		SB_LevelUpList->SetScrollOffset(ScrollOffset_List);

	// Limit Sale
	int64 StartTime = 0;
	int64 EndTime = 0;

	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		bool IsLimited = DocStore->GetProductSalePeriod(CurrentStoreID, StartTime, EndTime);

		FDateTime EndDateTime = FDateTime::FromUnixTimestamp(EndTime / 1000);
		if (TB_SalePeriod.IsValid())
		{
			if (IsLimited)
			{
				TB_SalePeriod->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EventAttendance_Period2")),
					FText::FromString(FString::FromInt(EndDateTime.GetYear())), FText::FromString(FString::FromInt(EndDateTime.GetMonth())), FText::FromString(FString::FromInt(EndDateTime.GetDay()))
				));
			}
			else
			{
				TB_SalePeriod->SetText(FText::GetEmpty());
			}
		}

		int32 MarkType = b2network::B2ShopMarkType::MARK_NONE;
		MarkType = DocStore->GetPackageProductMark(CurrentStoreID);

		if (UIP_Store_Event_Label.IsValid())
			UIP_Store_Event_Label->SetStoreEventLabel(MarkType);
	}
}

void UB2UIPackageTypePanel_Level::UpdateLevelupPackageLIst(TArray<FB2ShopLevelUpPackagePtr>& LevelupList)
{
	VB_LevelUpList->ClearChildren();
	for (auto Each : LevelupList)
	{
		auto* CreatedWidget = CreateWidget<class UB2UIPacakgeLevelupListPart>(GetOwningPlayer(), LevelupSlotItem);
		if (CreatedWidget != NULL)
		{
			CreatedWidget->Init();
			CreatedWidget->InitEventLevelupPart(Each);
			VB_LevelUpList->AddChild(CreatedWidget);
		}
	}
}

void UB2UIPackageTypePanel_Level::UpdateReachLevelPanel()
{
	TArray<FB2ShopReachLevelPackagePtr> ReachLevelList;
	ReachLevelList.Empty();

	BladeIIGameImpl::GetClientDataStore().GetPackageReachLevelList(CurrentPackageID, ReachLevelList);

	if (ReachLevelList.IsValidIndex(0))
	{
		if (UIP_Reach_Reward.IsValid())
			UIP_Reach_Reward->UpdateRewardIcon(ReachLevelList[0]->buy_reward_id, ReachLevelList[0]->buy_reward_count);

		if (UIP_Mission_Reward.IsValid())
			UIP_Mission_Reward->UpdateRewardIcon(ReachLevelList[0]->complete_reward_id, ReachLevelList[0]->complete_reward_count);

		auto* ProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(CurrentStoreID);
		if (ProductData)
		{
			if (UIP_Reach_Cost.IsValid())
			{
				if (static_cast<EStoreItemCost>(ProductData->CostType) == EStoreItemCost::Cash)
				{
					//B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(CurrentStoreID);
					//if (ProductInfo)
					//	UIP_Reach_Cost->SetCashCost(ProductInfo->Price, ProductInfo->CurrencyCode);
				}
				else
				{
					UIP_Reach_Cost->SetCost(static_cast<EStoreItemCost>(ProductData->CostType), ProductData->Cost, ProductData->CalculatedPrice);
				}

				SetRestrictData(ProductData->RestrictCount);
			}
		}

		SetReachRewardState();

		FText TempText;
		if (TB_Reach_LimitLevel.IsValid())
		{
			TempText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Reach_LimitLevel")),
				GetPCClassLocalizedText(SelectedPCClass),
				ReachLevelList[0]->limit_level);
			TB_Reach_LimitLevel->SetText(TempText);
		}

		if (TB_Reach_Warning.IsValid())
		{
			TempText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_Reach_Warning")),
				ReachLevelList[0]->expire_day,
				GetPCClassLocalizedText(SelectedPCClass),
				ReachLevelList[0]->reach_level,
				ReachLevelList[0]->complete_reward_count);
			TB_Reach_Warning->SetText(TempText);
		}
	}
}

void UB2UIPackageTypePanel_Level::SetRestrictData(int32 RestrictCount)
{
	if (TB_Reach_RestrictCount.IsValid())
	{
		int32 RemainCount = 0;
		TB_Reach_RestrictCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
		{
			int32 UsedCount = DocStore->GetReceivedRestrictCount(CurrentStoreID);
			RemainCount = RestrictCount - UsedCount;

			TB_Reach_RestrictCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Count")), RemainCount, RestrictCount));
		}

		if (BTN_Reach_Buy.IsValid())
		{
			BTN_Reach_Buy->SetIsEnabled(RemainCount == 0 ? false : true);
			BTN_Reach_Buy->SetVisibility(RemainCount == 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Visible);
		}

		if (UIP_Reach_Cost.IsValid())
			UIP_Reach_Cost->SetVisibility(RemainCount == 0 ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);

		if (STB_Reach_RestrictComplete.IsValid())
			STB_Reach_RestrictComplete->SetVisibility(RemainCount == 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UIPackageTypePanel_Level::SetReachRewardState()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		FPackageReachLevelMission* CurrentMission = DocStore->GetReceivedReachLevelPacakgeMissions(CurrentPackageID);
		EREWARD_STATE RewardState = EREWARD_STATE::NotBuy;

		if (CurrentMission)
		{
			int64 EndTime = CurrentMission->ExpireTime;
			FTimespan RemainTime = FDateTime::FromUnixTimestamp(EndTime / 1000) - FDateTime::UtcNow();

			if (CurrentMission->State == static_cast<int32>(b2network::B2MissionStateType::PROGRESS))
			{
				if (RemainTime > 0)
					RewardState = EREWARD_STATE::Impossible;
				else
					RewardState = EREWARD_STATE::Fail;

			}
			else if (CurrentMission->State == static_cast<int32>(b2network::B2MissionStateType::COMPLETED))
			{
				RewardState = EREWARD_STATE::Possible;
			}
			else if (CurrentMission->State == static_cast<int32>(b2network::B2MissionStateType::REWARDED))
			{
				RewardState = EREWARD_STATE::Rewarded;
			}

			int32 RemainDay = RemainTime.GetDays();
			int32 RemainHour = RemainTime.GetHours();
			int32 RemainMin = RemainTime.GetMinutes();
			int32 RemainSecond = RemainTime.GetSeconds();

			if (TB_Mission_PeriodCount.IsValid())
			{
				if (RemainDay > 0)
				{
					TB_Mission_PeriodCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageMission")), RemainDay));
				}
				else
				{
					if (RemainHour > 0)
					{
						TB_Mission_PeriodCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageMission_Hour")), RemainHour, RemainMin));
					}
					else
					{
						if (RemainMin > 0)
						{
							TB_Mission_PeriodCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageMission_Minute")), RemainMin));
						}
						else
						{
							TB_Mission_PeriodCount->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageMission_Second")));
						}
					}
				}
			}
		}

		if (WS_Mission_RewardState.IsValid())
		{
			switch (RewardState)
			{
			case EREWARD_STATE::Impossible:	// 구매했지만 보상 받을 수 없음
			case EREWARD_STATE::Possible:	// 보상 받을 수 있음
				WS_Mission_RewardState->SetActiveWidgetIndex(1);
				break;

			case EREWARD_STATE::Rewarded:	// 보상 받음
				WS_Mission_RewardState->SetActiveWidgetIndex(2);
				break;

			case EREWARD_STATE::Fail:		// 미션 실패
				WS_Mission_RewardState->SetActiveWidgetIndex(3);
				break;

			default:						// 구매 안함
				WS_Mission_RewardState->SetActiveWidgetIndex(0);
				break;
			}
		}

		if (TB_Mission_PeriodCount.IsValid())
			TB_Mission_PeriodCount->SetVisibility(RewardState == EREWARD_STATE::Impossible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		if (BTN_Mission_Reward.IsValid())
		{
			BTN_Mission_Reward->SetIsEnabled(RewardState == EREWARD_STATE::Possible ? true : false);
			BTN_Mission_Reward->SetVisibility(RewardState == EREWARD_STATE::Possible ? ESlateVisibility::Visible : ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UB2UIPackageTypePanel_Level::OnReciveLevelupReward(FB2ResponseRewardLevelUpPackagePtr RewardPtr)
{
	UB2UIMsgPopupReward* RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
	if (RewardPopup != nullptr)
	{
		RewardPopup->AddSingleRewardItem(RewardPtr->received_reward);

		// RewardPopup에서 ClientDataStore::AccountInfo에 보상 강제 적용
		RewardPopup->ShowRewardPopup(true);
		RewardPopup->ShowMailDescription(false);
	}

	if (SB_LevelUpList.IsValid())
		ScrollOffset_LeftTab = SB_LeftTab->GetScrollOffset();
	if (SB_LevelUpList.IsValid())
		ScrollOffset_List = SB_LevelUpList->GetScrollOffset();

	data_trader::Retailer::GetInstance().RequestGetPackageShop();
}

void UB2UIPackageTypePanel_Level::OnReciveReachLevelReward(FB2ResponseRewardReachLevelPackagePtr RewardPtr)
{
	UB2UIMsgPopupReward* RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
	if (RewardPopup != nullptr)
	{
		RewardPopup->AddSingleRewardItem(RewardPtr->received_reward);

		// RewardPopup에서 ClientDataStore::AccountInfo에 보상 강제 적용
		RewardPopup->ShowRewardPopup(true);
		RewardPopup->ShowMailDescription(false);
	}

	data_trader::Retailer::GetInstance().RequestGetPackageShop();
}


bool UB2UIPackageTypePanel_Level::RedDotCondition_Gladiator()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		return DocStore->GetReddotPackageLevelMissionByPCClass(EPCClass::EPC_Gladiator);
	}

	return false;
}

bool UB2UIPackageTypePanel_Level::RedDotCondition_Assassin()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		return DocStore->GetReddotPackageLevelMissionByPCClass(EPCClass::EPC_Assassin);
	}

	return false;
}

bool UB2UIPackageTypePanel_Level::RedDotCondition_Wizard()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		return DocStore->GetReddotPackageLevelMissionByPCClass(EPCClass::EPC_Wizard);
	}

	return false;
}

bool UB2UIPackageTypePanel_Level::RedDotCondition_Fighter()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		return DocStore->GetReddotPackageLevelMissionByPCClass(EPCClass::EPC_Fighter);
	}

	return false;
}

///////////////////////////////////////////////////////////////////
//                    UB2UIPacakgeLevelupListPart
///////////////////////////////////////////////////////////////////
UB2UIPacakgeLevelupListPart::UB2UIPacakgeLevelupListPart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PackageIndex = 0;
	PCLevel = 0;
	RewardState = EREWARD_STATE::NotBuy;
	PCClass = EPCClass::EPC_End;
}

void UB2UIPacakgeLevelupListPart::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UIRewardIcon, UIP_RewardIcon);
	GET_SLOT(UB2Button, BTN_GetReward);
	GET_SLOT(UTextBlock, TB_GetReward);
	GET_SLOT(UTextBlock, TB_GetRewardPossible);
	GET_SLOT(UTextBlock, TB_GetRewardImPossible);
	GET_SLOT(UTextBlock, TB_CompleteReward);
	GET_SLOT(UTextBlock, TB_NeedPackage);
	GET_SLOT(UOverlay, O_GetRewardPossible);
	GET_SLOT(UOverlay, O_GetRewardImPossible);
	GET_SLOT(UWidgetSwitcher, WS_ButtonState);
}

void UB2UIPacakgeLevelupListPart::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_GetReward, &UB2UIPacakgeLevelupListPart::OnClickBTN_GetReward);
}

void UB2UIPacakgeLevelupListPart::Init()
{
	Super::Init();
}

void UB2UIPacakgeLevelupListPart::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIPacakgeLevelupListPart::UpdateStaticText()
{
	if (TB_NeedPackage.IsValid())
		TB_NeedPackage->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_LevelupBuyDesc")));
	if (TB_CompleteReward.IsValid())
		TB_CompleteReward->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Acquired")));
	if (TB_GetReward.IsValid())
		TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Store_PossibleAcquire")));
}

void UB2UIPacakgeLevelupListPart::OnClickBTN_GetReward()
{
	if (RewardState != EREWARD_STATE::Possible)
		return;

	RewardState = EREWARD_STATE::Rewarded;

	data_trader::Retailer::GetInstance().RequestRewardLevelUpPackage(PackageIndex, CliToSvrPCClassType(PCClass), PCLevel);

	UpdateVisibleState();
}

void UB2UIPacakgeLevelupListPart::InitEventLevelupPart(FB2ShopLevelUpPackagePtr LevelItem)
{
	PackageIndex = LevelItem->package_id;
	PCClass = SvrToCliPCClassType(LevelItem->character_type);
	PCLevel = LevelItem->level;

	if (UIP_RewardIcon.IsValid())
	{
		UIP_RewardIcon->Init();
		UIP_RewardIcon->UpdateRewardIcon(LevelItem->reward_id, LevelItem->reward_count);
	}

	if (TB_GetReward.IsValid())
		TB_GetReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward")));

	if (TB_CompleteReward.IsValid())
		TB_CompleteReward->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GetReward2")));

	if (TB_GetRewardPossible.IsValid())
		TB_GetRewardPossible->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_NotEnoughCharLevel")), FText::AsNumber(LevelItem->level)));

	if (TB_GetRewardImPossible.IsValid())
		TB_GetRewardImPossible->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_NotEnoughCharLevel")), FText::AsNumber(LevelItem->level)));

	UpdateVisibleState();
}

void UB2UIPacakgeLevelupListPart::UpdateVisibleState()
{
	bool bPossible = false;
	RewardState = EREWARD_STATE::NotBuy;

	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		FPackageLevelupMission* CurrentMission = DocStore->GetReceivedLevelupPacakgeMission(PackageIndex, PCLevel);
		if (CurrentMission)
		{
			if (CurrentMission->Package_State == static_cast<int32>(b2network::B2MissionStateType::PROGRESS))
			{
				RewardState = EREWARD_STATE::Impossible;
			}
			else if (CurrentMission->Package_State == static_cast<int32>(b2network::B2MissionStateType::COMPLETED))
			{
				RewardState = EREWARD_STATE::Possible;
			}
			else if (CurrentMission->Package_State == static_cast<int32>(b2network::B2MissionStateType::REWARDED))
			{
				RewardState = EREWARD_STATE::Rewarded;
			}
		}
	}

	switch (RewardState)
	{
	case EREWARD_STATE::Impossible:	// 구매했지만 불가능
	{
		if (BTN_GetReward.IsValid())
			BTN_GetReward->SetIsEnabled(false);

		if (WS_ButtonState.IsValid())
			WS_ButtonState->SetActiveWidgetIndex(1);
	}
	break;
	case EREWARD_STATE::Possible:	//가능
	{
		if (BTN_GetReward.IsValid())
			BTN_GetReward->SetIsEnabled(true);

		if (WS_ButtonState.IsValid())
			WS_ButtonState->SetActiveWidgetIndex(1);

		bPossible = true;
	}
	break;
	case EREWARD_STATE::Rewarded:	//완료
	{
		if (WS_ButtonState.IsValid())
			WS_ButtonState->SetActiveWidgetIndex(2);
	}
	break;
	default: // EREWARD_STATE::NotBuy
	{
		// 상품 구매후 수령 가능.
		if (WS_ButtonState.IsValid())
			WS_ButtonState->SetActiveWidgetIndex(0);
	}
	break;
	}

	if (O_GetRewardPossible.IsValid())
		O_GetRewardPossible->SetVisibility(bPossible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (O_GetRewardImPossible.IsValid())
		O_GetRewardImPossible->SetVisibility(bPossible ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

///////////////////////////////////////////////////////////////////
//                    UB2UIPacakgeLevelupLeftTab
///////////////////////////////////////////////////////////////////
void UB2UIPacakgeLevelupLeftTab::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Limited);
	GET_SLOT(UOverlay, O_Limited);

	if (O_Limited.IsValid())
		O_Limited->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIPacakgeLevelupLeftTab::Init()
{
	Super::Init();
}

void UB2UIPacakgeLevelupLeftTab::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIPacakgeLevelupLeftTab::UpdateStaticText()
{
	if (TB_Limited.IsValid())
		TB_Limited->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Limited")));
}

void UB2UIPacakgeLevelupLeftTab::BindReddot()
{
	REGISTER_REDDOT_CONDITION(this->GetMyRedDotWidget(), &UB2UIPacakgeLevelupLeftTab::RedDotCondition_LeftTab);
}

void UB2UIPacakgeLevelupLeftTab::SetCurrentPackageID(int32 PackageID, bool ReachLevel)
{
	CurrentPackageID = PackageID;
	IsReachLevel = ReachLevel;
}

bool UB2UIPacakgeLevelupLeftTab::RedDotCondition_LeftTab()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		if (!IsReachLevel)
			return DocStore->GetReddotPackageLevelUpMissionByPackageID(CurrentPackageID);
		else
		{
			FPackageReachLevelMission* thisMission = DocStore->GetReceivedReachLevelPacakgeMissions(CurrentPackageID);
			if (thisMission)
				return (thisMission->State == static_cast<int32>(b2network::B2MissionStateType::COMPLETED));
		}
	}

	return false;
}

void UB2UIPacakgeLevelupLeftTab::SetLimitedTime(bool bIsLimited)
{
	if (O_Limited.IsValid())
		O_Limited->SetVisibility(bIsLimited ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

///////////////////////////////////////////////////////////////////
//                    UB2UIPacakgeFlatrateItem
///////////////////////////////////////////////////////////////////
UB2UIPacakgeFlatrateItem::UB2UIPacakgeFlatrateItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIPacakgeFlatrateItem::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_BuyItem, &UB2UIPacakgeFlatrateItem::OnClickBTN_BuyItem);
}

void UB2UIPacakgeFlatrateItem::Init()
{
	Super::Init();
}

void UB2UIPacakgeFlatrateItem::DestroySelf(class UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UIPacakgeFlatrateItem::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_FlatrateItemTitle);
	GET_SLOT(UOverlay, OV_SlotWidget);

	GET_SLOT(UB2UIRewardIcon, UIP_FlatrateItemReward);
	if (UIP_FlatrateItemReward.IsValid())
		UIP_FlatrateItemReward->Init();
	GET_SLOT(UTextBlock, TB_FlatrateItemDay);
	GET_SLOT(UTextBlock, TB_FlatrateItemAll);
	GET_SLOT(UTextBlock, TB_FlatrateItemDate);

	GET_SLOT(UButton, BTN_BuyItem);
	GET_SLOT(UTextBlock, TB_RestrictComplete);
	GET_SLOT(UB2UIStoreProductCost, UIP_Cost);
	if (UIP_Cost.IsValid())
		UIP_Cost->Init();

	GET_SLOT(UB2UIStoreEventLabel, UIP_Store_Event_Label);
	if (UIP_Store_Event_Label.IsValid())
		UIP_Store_Event_Label->Init();
}

void UB2UIPacakgeFlatrateItem::SetFlatrateSlotWidget(class UMaterialInstance* ProductIcon, int32 SlotIconEffectType)
{
	TSubclassOf<UB2UIStoreItemSlotIcon> StoreSlotUIClass = StoreSlotWidget.LoadSynchronous();
	auto* SlotIcon = StoreSlotUIClass ? CreateWidget<UB2UIStoreItemSlotIcon>(GetWorld(), StoreSlotUIClass) : nullptr;

	if (OV_SlotWidget.IsValid())
	{
		SlotIcon->Init();
		SlotIcon->SetIcon(ProductIcon);
		SlotIcon->SetEffect(SlotIconEffectType);

		OV_SlotWidget->AddChild(SlotIcon);
	}
}

void UB2UIPacakgeFlatrateItem::SetFlatrateItemData(FStoreProductData StoreData, FPackageProductData PacakgeData, EPACKAGE_TYPE PackageType)
{
	StoreProductId = StoreData.StoreProductId;
	FString LstringBuffer = FString::Printf(TEXT("Store_PackageName_%d"), StoreData.Product.ProductId);

	if (PackageType == EPACKAGE_TYPE::Weekly_Flatrate)
	{
		if (TB_FlatrateItemTitle.IsValid())
			TB_FlatrateItemTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageName_500")));
	}
	else if (PackageType == EPACKAGE_TYPE::Monthly_Flatrate)
	{
		if (TB_FlatrateItemTitle.IsValid())
		{
			//TB_FlatrateItemTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageName_503")));
			TB_FlatrateItemTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT(stringBuffer)));
		}
	}
	else if (PackageType == EPACKAGE_TYPE::Monthly_Flatrate2)
	{
		if (TB_FlatrateItemTitle.IsValid())
		{
			//TB_FlatrateItemTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageName_504")));
			TB_FlatrateItemTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT(stringBuffer)));
		}
	}
	
	/*
	// 밑의 내용은 위의 내용이 하드코딩 되어있는걸 완전히 풀었을 때의 코드이다. 아직 "매일 혜택 패키지" 외의 항목과 "주간 혜택"에 대해선 검증이 되지 않아서 필요한 부분에만 적용해서 사용하는 방향으로 작성되어 있다.
	if (TB_FlatrateItemTitle.IsValid())
	{
		TB_FlatrateItemTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT(stringBuffer)));
	}
	*/

	int32 ItemID = BladeIIGameImpl::GetClientDataStore().GetItemTemplateIdFromMDProductData(PacakgeData.PackageProductId, PacakgeData.PackageProductType);
	FText RewardName = FText::GetEmpty();
	
	UB2ItemInfo* const iteminfo(StaticFindItemInfo());
	if (iteminfo)
	{
		RewardName = StaticFindItemInfo()->GetItemName(ItemID);
	}

	if (TB_FlatrateItemDay.IsValid())
	{
		int32 Day = StoreData.PayPeriod;
		TB_FlatrateItemDay->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_FlatrateItem_Day")), RewardName, FText::AsNumber(Day)));
	}

	if (TB_FlatrateItemAll.IsValid())
	{
		int32 AllCount = StoreData.PayPeriod * PacakgeData.PackageProductCount;
		TB_FlatrateItemAll->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Package_FlatrateItem_All")), FText::AsNumber(AllCount), RewardName));
	}

	if (UIP_FlatrateItemReward.IsValid())
	{
		UIP_FlatrateItemReward->UpdateRewardIconByItemIndex(ItemID, FText::AsNumber(PacakgeData.PackageProductCount));
	}

	if (static_cast<EStoreItemCost>(StoreData.CostType) == EStoreItemCost::Cash)
	{
		//B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(StoreProductId);
		//if (ProductInfo)
		//	UIP_Cost->SetCashCost(ProductInfo->Price, ProductInfo->CurrencyCode);
	}
	else
	{
		UIP_Cost->SetCost(static_cast<EStoreItemCost>(StoreData.CostType), StoreData.Cost, StoreData.CalculatedPrice);
	}
}

void UB2UIPacakgeFlatrateItem::SetPeriodData(int32 PackageID)
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int64 EndTime = DocStore->GetReceivedPeriodicPackageLeftTime(PackageID);
		FTimespan RemainTime = FDateTime::FromUnixTimestamp(EndTime / 1000) - FDateTime::UtcNow();

		if (BTN_BuyItem.IsValid())
			BTN_BuyItem->SetIsEnabled(RemainTime.GetSeconds() > 0 ? false : true);

		if (UIP_Cost.IsValid())
			UIP_Cost->SetVisibility(RemainTime.GetSeconds() > 0 ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);

		if (TB_RestrictComplete.IsValid())
		{
			TB_RestrictComplete->SetVisibility(RemainTime.GetSeconds() > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
			TB_RestrictComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Restrict_Complete")));
		}

		int32 RemainDay = RemainTime.GetDays();
		int32 RemainHour = RemainTime.GetHours();
		int32 RemainMin = RemainTime.GetMinutes();
		int32 RemainSecond = RemainTime.GetSeconds();

		if (TB_FlatrateItemDate.IsValid())
		{
			TB_FlatrateItemDate->SetVisibility(RemainTime.GetSeconds() > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

			if (RemainDay > 0)
			{
				TB_FlatrateItemDate->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackagePeriod")), RemainDay));
			}
			else
			{
				if (RemainHour > 0)
				{
					TB_FlatrateItemDate->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackagePeriod_Hour")), RemainHour, RemainMin));
				}
				else
				{
					if (RemainMin > 0)
					{
						TB_FlatrateItemDate->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackagePeriod_Minute")), RemainMin));
					}
					else
					{
						TB_FlatrateItemDate->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackagePeriod_Second")));
					}
				}
			}
		}
	}
}

void UB2UIPacakgeFlatrateItem::SetPackageProductMark(int32 ProductId)
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		int32 PackageProductMark = DocStore->GetPackageProductMark(ProductId);

		if (UIP_Store_Event_Label.IsValid())
			UIP_Store_Event_Label->SetStoreEventLabel(PackageProductMark);
	}
}

void UB2UIPacakgeFlatrateItem::OnClickBTN_BuyItem()
{
	BuyDelegate.ExecuteIfBound(StoreProductId);
}
