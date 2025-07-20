// 弥檬 俺惯磊啊 盔贰 i, j, k肺 for巩阑 倒绊 乐促绰吧 颇厩窍绰单, 内靛 府轰肺 秦寸 巩备甫 荤侩窍瘤 富扼绰 瘤利阑 罐酒辑 俺惯磊 炼瞒 捞秦 给窍绰 函荐疙栏肺 官柴
// 父老 盔贰 i, j, k肺 毫具 茄促搁 欺器胶狼 89041 滚怜捞 付瘤阜 i,j,k看澜.


#include "B2UIItemForge.h"
#include "B2UIManager.h"
#include "UI/Widget/B2UICommonTab.h"
#include "B2LobbyGameMode.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUser.h"
#include "B2UISlotItem.h"
#include "B2UICollectBookItemDetail.h"
#include "B2UICollectBookIcon.h"
#include "B2LobbyUI_ItemDetail.h"
#include "B2UIGoodsShortcutTooltipPopup.h"
#include "B2DynItemIcon_ItemForge.h"
#include "B2UIItemForgeTabButton.h"
#include "B2UIItemForgeListButton.h"
#include "B2UIItemForgeGroupTitle.h"
#include "B2UIHeaderResource.h"
#include "B2UIHeader.h"
#include "Retailer.h"
#include "B2ItemInfo.h"
#include "BladeIIGameImpl.h"
#include "B2ServerResultCodeTable.h"
#include "B2Inventory.h"
#include "Misc/DateTime.h"
#include "B2GameInstance.h"
#include "B2LoadingImageInfo.h"
#include "Event.h"

void UB2UIItemForge::CacheAssets()
{
	Super::CacheAssets();

	//扁裙辑啊 唱坷扁档 傈俊 措面 捞犯霸 叼磊牢 登摆瘤 秦辑 沥狼等 函荐疙甸捞 阜惑 弥辆 叼磊牢俊辑 硅摹客 备己捞 官差搁辑 荤侩 救窍绰 函荐甸捞 甸绢乐澜.

	GET_SLOT(UB2Button, BTN_MasterModal);

	GET_SLOT(UB2UICommonTab, TAB_Forge);
	GET_SLOT(UB2UICommonTab, TAB_Upgrade);

	GET_SLOT(UB2Button, BTN_SelectGladiator);
	GET_SLOT(UB2Button, BTN_SelectAssassin);
	GET_SLOT(UB2Button, BTN_SelectWizard);
	GET_SLOT(UB2Button, BTN_SelectFighter);

	GET_SLOT(UHorizontalBox, HB_TabList);

	GET_SLOT(UOverlay, O_PleaseSelectForgeStuff);
	GET_SLOT(UTextBlock, TB_PleaseSelectForgeStuff);

	GET_SLOT(UTextBlock, TB_ListIsEmpty);

	GET_SLOT(UTextBlock, TB_AvailableItemList);
	GET_SLOT(UB2ScrollBox, SB_AvailableItemList);

	GET_SLOT(UB2UISlotItem, UIP_SelectedItem);

	GET_SLOT(UCanvasPanel, CP_Right);

	GET_SLOT(UB2ScrollBox, SB_IngredientList);

	GET_SLOT(UB2Button, BTN_ResultItemDetail);
	GET_SLOT(UB2DynItemIcon_ItemForge, UIP_ResultItemIcon);

	GET_SLOT(UTextBlock, TB_ForgeRate);
	GET_SLOT(UTextBlock, TB_SuccessRate);

	GET_SLOT(UTextBlock, TB_Warning_Forge);
	GET_SLOT(UTextBlock, TB_Warning_Upgrade);

	GET_SLOT(UTextBlock, TB_ForgeLimit);
	GET_SLOT(UTextBlock, TB_ForgeLimitNumber);

	GET_SLOT(UHorizontalBox, HB_ShowingLimitNumber);

	GET_SLOT(UHorizontalBox, HB_AdjustQuantity);
	GET_SLOT(UTextBlock, TB_ForgeQuantity);
	GET_SLOT(UTextBlock, TB_MaximumQuantity);
	GET_SLOT(UB2Button, BTN_DecQuantity);
	GET_SLOT(USlider, SD_ControlQuantity);
	GET_SLOT(UTextBlock, TB_CurrentQuantity);
	GET_SLOT(UB2Button, BTN_IncQuantity);
	GET_SLOT(UB2Button, BTN_MaxQuantity);
	GET_SLOT(UTextBlock, TB_MaxQuantity);

	GET_SLOT(UCanvasPanel, CP_PeriodLimit);
	GET_SLOT(UB2RichTextBlock, TB_LimitStartTime);
	GET_SLOT(UB2RichTextBlock, TB_LimitEndTime);

	GET_SLOT(UB2Button, BTN_Execute);
	GET_SLOT(UTextBlock, TB_Execute);

	GET_SLOT(UB2Button, BTN_Filter);
	GET_SLOT(UTextBlock, TB_Filter);

	GET_SLOT(UTextBlock, TB_PopupTitle);
	GET_SLOT(UB2UIFilterItemForge, UIP_ForgeFilter);

	GET_SLOT(UCanvasPanel, CP_Filter);
	GET_SLOT(UB2Button, F_Btn_Close);
	GET_SLOT(UB2Button, F_Modal);

	GET_SLOT(UB2Button, F_BTN_Apply);
	GET_SLOT(UTextBlock, F_TB_Apply);

	GET_SLOT(UB2Button, F_BTN_Reset);
	GET_SLOT(UTextBlock, F_TB_Reset);

	GET_SLOT(UB2Button, BTN_ItemDetailBackGround);
	GET_SLOT(UB2UICollectBookItemDetail, UIP_ItemDetail);
	if (UIP_ItemDetail.IsValid())
	{
		UIP_ItemDetail->Init();
		FOnClickDetailClose DetailCloseDelegate;
		DetailCloseDelegate.BindUObject(this, &UB2UIItemForge::SetvisibilityOfDetail, false);
		UIP_ItemDetail->SetDetailViewCloseDelegate(DetailCloseDelegate);
	}

	GET_SLOT(UTextBlock, TB_Price);

	GET_SLOT(UHorizontalBox, HB_ProgressBar);

	GET_SLOT(UHorizontalBox, HB_Resource);
	GET_SLOT(UB2UIHeaderResource, UIP_HeaderResource_DarkCrystal);
	GET_SLOT(UB2UIHeaderResource, UIP_HeaderResource_Gold);
	GET_SLOT(UB2UIHeaderResource, UIP_HeaderResource_Gem);

	GET_SLOT(UTextBlock, TB_Forging);

	GET_SLOT(UCanvasPanel, CP_Result);
	GET_SLOT(UB2ScrollBox, SB_ResultScroll);

	GET_SLOT(UTextBlock, X_TB_ResultTitle);

	GET_SLOT(UTextBlock, X_TB_Result);
	GET_SLOT(UB2RichTextBlock, X_TB_Success);
	GET_SLOT(UB2RichTextBlock, X_TB_HugeSuccess);
	GET_SLOT(UB2RichTextBlock, X_TB_Fail);

	GET_SLOT(UB2Button, X_BTN_Skip);
	GET_SLOT(UTextBlock, X_TB_Skip);
	GET_SLOT(UB2Button, X_BTN_Confirm);
	GET_SLOT(UTextBlock, X_TB_Confirm);
}

void UB2UIItemForge::BindDelegates()
{
	Super::BindDelegates();

	

	if (TAB_Forge.IsValid())
		REGISTER_UITAB_BINDING(TAB_Forge.Get(), &UB2UIItemForge::OnClickedBTNForgeMenu);
	if (TAB_Upgrade.IsValid())
		REGISTER_UITAB_BINDING(TAB_Upgrade.Get(), &UB2UIItemForge::OnClickedBTNUpgradeMenu);

	BIND_CLICK_FUNC_TO_BTN(BTN_SelectGladiator, &UB2UIItemForge::OnClickedBTNGladiator);
	BIND_CLICK_FUNC_TO_BTN(BTN_SelectAssassin, &UB2UIItemForge::OnClickedBTNAssassin);
	BIND_CLICK_FUNC_TO_BTN(BTN_SelectWizard, &UB2UIItemForge::OnClickedBTNWizard);
	BIND_CLICK_FUNC_TO_BTN(BTN_SelectFighter, &UB2UIItemForge::OnClickedBTNFighter);

	BIND_CLICK_FUNC_TO_BTN(BTN_ResultItemDetail, &UB2UIItemForge::OnClickedBTNResultItemDetail);

	BIND_CLICK_FUNC_TO_BTN(BTN_IncQuantity, &UB2UIItemForge::OnClickedBTNIncQuantity);
	BIND_CLICK_FUNC_TO_BTN(BTN_DecQuantity, &UB2UIItemForge::OnClickedBTNDecQuantity);
	BIND_CLICK_FUNC_TO_BTN(BTN_MaxQuantity, &UB2UIItemForge::OnClickedBTNMaxQuantity);

	BIND_CLICK_FUNC_TO_BTN(BTN_Execute, &UB2UIItemForge::OnClickedBTNExecute);

	BIND_CLICK_FUNC_TO_BTN(BTN_Filter, &UB2UIItemForge::OnClickedBTNFilter);
	BIND_CLICK_FUNC_TO_BTN(F_Btn_Close, &UB2UIItemForge::OnClickedBTNFilterClose);
	BIND_CLICK_FUNC_TO_BTN(F_BTN_Apply, &UB2UIItemForge::OnClickedBTNFilterApply);
	BIND_CLICK_FUNC_TO_BTN(F_BTN_Reset, &UB2UIItemForge::OnClickedBTNFilterReset);
	if (UIP_ForgeFilter.IsValid())
		UIP_ForgeFilter->OnCheckStateChanged.AddDynamic(this, &UB2UIItemForge::OnAnyForgeFilterCBChanged);

	BIND_CLICK_FUNC_TO_BTN(X_BTN_Skip, &UB2UIItemForge::OnClickedBTNSkipResult);
	BIND_CLICK_FUNC_TO_BTN(X_BTN_Confirm, &UB2UIItemForge::OnClickedBTNResultConfirm);

	if (SD_ControlQuantity.IsValid())
	{
		SD_ControlQuantity->OnValueChanged.AddDynamic(this, &UB2UIItemForge::OnChangeLevelSlider);
	}
}

void UB2UIItemForge::Init()
{
	Super::Init();
}
void UB2UIItemForge::ForgeInit()
{
	
	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Title")));

	TAB_Forge->SetTabName(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_ForgeTab")));
	TAB_Upgrade->SetTabName(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_UpgradeTab")));

	ClientDataStore = &BladeIIGameImpl::GetClientDataStore();
	selectedForgeType = EItemForgeType::Forge;
	selectedForgeTab = 1;

	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		selectedClass = IntToPCClass(DocBattle->GetCurPCClass());
		backupClass = selectedClass;
	}
	else
	{
		selectedClass = EPCClass::EPC_Gladiator;
	}
	EventChangedHeroMgmtPCClass(selectedClass);

	LobbyCharObvForcedChangeClass<EPCClass>::GetInstance().Signal(EPCClass::EPC_Gladiator);
	LobbyCharObvForcedChangeClass<EPCClass>::GetInstance().Signal(backupClass);

	quantity = 1;
	maxQuantity = 1;

	ResetForgeSetting();

	UIP_HeaderResource_Gold->Init();
	UIP_HeaderResource_Gem->Init();
	UIP_HeaderResource_DarkCrystal->Init();
	UIP_HeaderResource_Gold->SetVisibilityOfPlus(false);
	UIP_HeaderResource_Gem->SetVisibilityOfPlus(false);
	UIP_HeaderResource_DarkCrystal->SetVisibilityOfPlus(false);
	UIP_HeaderResource_Gold->SetAmount(0);
	UIP_HeaderResource_Gem->SetAmount(0);
	UIP_HeaderResource_DarkCrystal->SetAmount(0);
	UIP_HeaderResource_Gold->SetVisibility(ESlateVisibility::Visible);
	UIP_HeaderResource_Gem->SetVisibility(ESlateVisibility::Visible);
	UIP_HeaderResource_DarkCrystal->SetVisibility(ESlateVisibility::Visible);

	if (CP_Result.IsValid())
		CP_Result->SetVisibility(ESlateVisibility::Collapsed);

	if (SD_ControlQuantity.IsValid())
		SD_ControlQuantity->SetValue(1);

	resultSkip = false;

	if (UIP_ForgeFilter.IsValid())
	{
		UIP_ForgeFilter->Init();
		UIP_ForgeFilter->SetMinGradeFilter();
		UIP_ForgeFilter->LoadFilterInfo(TEXT("ItemForgeFilter"));
	}

	SetTabList();
	SetBasicData();

	TAB_Forge->SetTabSelected(true);

	BTN_Execute->SetIsEnabled(false);

	aniEnd = false;
	dataRecieved = false;
}

static void OpenLobbyLoginLoadingScreen()
{
	// AccountInfo 罐酒辑 捞巴历巴 肺爹窍绰 霸 唱抚 矫埃捞 吧府绰单 DLC 俊辑 肺厚 肺爹且 锭狼 肺爹 拳搁阑 捞绢辑 钎矫.
	UB2LoadingImageInfo* LoadingImageInfo = StaticFindLoadingImageInfo();
	// 角力 霸烙俊辑狼 AccountInfo 罐绰 扒 肺厚 皋牢拳搁 霉 柳涝(肺弊牢)俊 秦寸.
	const TCHAR* LobbyLoginLoadingImageName = LoadingImageInfo ? *(LoadingImageInfo->GetLobbyLoginLoadingImageName()) : nullptr;
	SetupLoadingScreenImageClass<bool, bool, bool, const TCHAR*, EB2LoadingMessageType>::GetInstance().Signal(
		false, false, true,
		LobbyLoginLoadingImageName, // 父老 null 捞扼搁 救俊辑 促弗 侥栏肺 肺爹 捞固瘤甫 波郴静霸 瞪 巴.
		EB2LoadingMessageType::ELMT_LobbyLoginLoading
	);
}
static void CloseLobbyLoginLoadingScreen()
{
	ConditionalWaitForLoadingScreenClass<>::GetInstance().Signal();
}

void UB2UIItemForge::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);
	//力累家甫 唱艾促啊 甸绢艾阑锭 历厘茄 单捞磐甸捞 罐酒瘤瘤 臼绰 巩力客 Init阑 咯矾锅 档绰 巩力甫 秦搬窍扁 困秦 喊档肺 哗 狄
	if (forgeListRequest == false)
	{
		data_trader::Retailer::GetInstance().RequestItemForgeList();
		forgeListRequest = true;
	}
	SubscribeEvent();
}

void UB2UIItemForge::OnClose(bool RightNow)
{
	Super::OnClose(RightNow);

	UnSubscribeEvent();
}

void UB2UIItemForge::SubscribeEvent()
{
	if (bSubscribed == false)
	{
		UnSubscribeEvent();

		Issues.Add(DeliveryTryItemForgeClass<FB2ResponseTryItemForgePtr>::GetInstance().Subscribe2(
			[this](FB2ResponseTryItemForgePtr TryItemForgePtr)
		{
			this->ResponseTryItemForge(TryItemForgePtr);
		}
		));
	}
	bSubscribed = true;
}

void UB2UIItemForge::UnSubscribeEvent()
{
	Issues.Empty();
	bSubscribed = false;
}

void UB2UIItemForge::ResponseItemForgeList(FB2ResponseGetItemForgeListPtr ItemForgeListPtr)
{
	if (CP_Result.IsValid())
		CP_Result->SetVisibility(ESlateVisibility::Collapsed);

	tabArrayForForge.Empty();
	tabArrayForUpgrade.Empty();

	LimitStatus.Empty();
	PeriodLimitStatus.Empty();
	if (HB_TabList.IsValid())
		if (HB_TabList->HasAnyChildren())
			HB_TabList->ClearChildren();

	ResetForgeSetting();

	if (TB_Warning_Forge.IsValid())
		TB_Warning_Forge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	for (int32 itemForgeListPtrIndex = 0; itemForgeListPtrIndex < ItemForgeListPtr->limit_status.Num(); itemForgeListPtrIndex++)
	{
		FItemForgeLimitInfo NewInfo;

		NewInfo.limitID = ItemForgeListPtr->limit_status[itemForgeListPtrIndex]->limit_id;
		NewInfo.limitQuantity = ItemForgeListPtr->limit_status[itemForgeListPtrIndex]->left_count;
		switch (ItemForgeListPtr->limit_status[itemForgeListPtrIndex]->limit_type)
		{
		case 1: NewInfo.limitType = EItemForgeLimitType::NumberOfTry; break;
		case 2: NewInfo.limitType = EItemForgeLimitType::NumberOfSuccess; break;
		}

		switch (ItemForgeListPtr->limit_status[itemForgeListPtrIndex]->limit_time_type)
		{
		case 5: NewInfo.limitTimeType = EItemForgePeriodLimitType::None; break;
		case 1: NewInfo.limitTimeType = EItemForgePeriodLimitType::Once; break;
		case 2: NewInfo.limitTimeType = EItemForgePeriodLimitType::Day; break;
		case 3: NewInfo.limitTimeType = EItemForgePeriodLimitType::Week; break;
		case 4: NewInfo.limitTimeType = EItemForgePeriodLimitType::Month; break;
		}

		LimitStatus.Add(NewInfo);
	}

	for (int32 itemForgeListPtrIndex = 0; itemForgeListPtrIndex < ItemForgeListPtr->factory_periodic_status.Num(); itemForgeListPtrIndex++)
	{
		FItemForgePeriodicStatus NewInfo;

		

		NewInfo.forgeID = ItemForgeListPtr->factory_periodic_status[itemForgeListPtrIndex]->factory_id;
		NewInfo.previewTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(ItemForgeListPtr->factory_periodic_status[itemForgeListPtrIndex]->preview_time));
		NewInfo.startTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(ItemForgeListPtr->factory_periodic_status[itemForgeListPtrIndex]->start_time));
		NewInfo.endTime = UB2GameInstance::GetUtcToLocalTime(GetServerTimeToDateTime(ItemForgeListPtr->factory_periodic_status[itemForgeListPtrIndex]->end_time));

		PeriodLimitStatus.Add(NewInfo);
	}

	selectedForgeType = EItemForgeType::Forge;
	forgeAble = false;
// 	forgeTable.Empty();
// 	ClientDataStore->GetItemForgeTable(selectedForgeType, selectedClass, forgeTable, PeriodLimitStatus);

	ResetForgeSetting();
	SetTabList();
	SetBasicData();

	OnClickedBTNUpgradeMenu();
	OnClickedBTNForgeMenu();

	UIP_ForgeFilter->GetSelectedFilters(filterInvenType, filterStarGrade);
	if (filterInvenType.Num() > 0 || filterStarGrade.Num() > 0)
		ResetForgeSettingForFilter();
	else
		ResetForgeSetting();

	SetTabList();
	SetBasicData();

	TAB_Forge->SetTabSelected(true);

	BTN_Execute->SetIsEnabled(false);
}

void UB2UIItemForge::ResponseTryItemForge(FB2ResponseTryItemForgePtr TryItemForgePtr)
{
	if (mainIngredientTrack)
		BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().DecSharedConsumableAmountOfType(selectedMainItem.ItemRefID, mainIngredientTrack->GetRequiredQuantity());
	if (subIngredientTrack1)
		BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().DecSharedConsumableAmountOfType(selected1stMaterial.ItemRefID, subIngredientTrack1->GetRequiredQuantity());
	if (subIngredientTrack2)
		BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().DecSharedConsumableAmountOfType(selected2ndMaterial.ItemRefID, subIngredientTrack2->GetRequiredQuantity());
	if (subIngredientTrack3)
		BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().DecSharedConsumableAmountOfType(selected3rdMaterial.ItemRefID, subIngredientTrack3->GetRequiredQuantity());
	if (subIngredientTrack4)
		BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().DecSharedConsumableAmountOfType(selected4thMaterial.ItemRefID, subIngredientTrack4->GetRequiredQuantity());


	resultItems.Empty();
	for (int32 tryItemForgePtrIndex = 0; tryItemForgePtrIndex < TryItemForgePtr->make_result.Num(); tryItemForgePtrIndex++)
	{
		FItemForgeResult newData;

		switch (TryItemForgePtr->make_result[tryItemForgePtrIndex]->factory_result_type)
		{
		case 1: newData.resultType = EItemForgeResultType::Fail; break;
		case 2: newData.resultType = EItemForgeResultType::Success; break;
		case 3: newData.resultType = EItemForgeResultType::HugeSuccess; break;
		}

		newData.forgedItem = TryItemForgePtr->make_result[tryItemForgePtrIndex]->make_item;
		newData.bonusItem = TryItemForgePtr->make_result[tryItemForgePtrIndex]->bonus_item;

		for (int32 forRefundIndex = 0; forRefundIndex < TryItemForgePtr->make_result[tryItemForgePtrIndex]->refund_item.Num(); forRefundIndex++)
		{
			FB2Item tempRefundedItem;
			tempRefundedItem = TryItemForgePtr->make_result[tryItemForgePtrIndex]->refund_item[forRefundIndex];
			newData.refundedItem.Add(tempRefundedItem);
		}

		resultItems.Add(newData);

		if (TryItemForgePtr->make_result[tryItemForgePtrIndex]->make_item->id > 0)
		{
			if (BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().FindItem(TryItemForgePtr->make_result[tryItemForgePtrIndex]->make_item->id) != nullptr)
			{
				FB2Item tempItem;
				tempItem = TryItemForgePtr->make_result[tryItemForgePtrIndex]->make_item;
				BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().IncSharedConsumableAmountOfType(tempItem.ItemRefID, tempItem.ConsumingAmount);
			}
			else
			{
				FB2Item tempItem;
				tempItem = TryItemForgePtr->make_result[tryItemForgePtrIndex]->make_item;
				BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().AddItem(tempItem);
			}
		}

		if (TryItemForgePtr->make_result[tryItemForgePtrIndex]->bonus_item->id > 0)
		{
			if (BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().FindItem(TryItemForgePtr->make_result[tryItemForgePtrIndex]->bonus_item->id) != nullptr)
			{
				FB2Item tempItem;
				tempItem = TryItemForgePtr->make_result[tryItemForgePtrIndex]->bonus_item;
				BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().IncSharedConsumableAmountOfType(tempItem.ItemRefID, tempItem.ConsumingAmount);
			}
			else
			{
				FB2Item tempItem;
				tempItem = TryItemForgePtr->make_result[tryItemForgePtrIndex]->bonus_item;
				BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().AddItem(tempItem);
			}
		}

		if (TryItemForgePtr->make_result[tryItemForgePtrIndex]->refund_item.Num() > 0)
		{
			for (int32 forRefundIndex = 0; forRefundIndex < TryItemForgePtr->make_result[tryItemForgePtrIndex]->refund_item.Num(); forRefundIndex++)
			{
				if (TryItemForgePtr->make_result[tryItemForgePtrIndex]->refund_item[forRefundIndex]->id > 0)
				{
					if (BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().FindItem(TryItemForgePtr->make_result[tryItemForgePtrIndex]->refund_item[refundIndex]->id) != nullptr)
					{
						FB2Item tempItem;
						tempItem = TryItemForgePtr->make_result[tryItemForgePtrIndex]->refund_item[forRefundIndex];
						BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().IncSharedConsumableAmountOfType(tempItem.ItemRefID, tempItem.ConsumingAmount);
					}
					else
					{
						FB2Item tempItem;
						tempItem = TryItemForgePtr->make_result[tryItemForgePtrIndex]->refund_item[forRefundIndex];
						BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().GetUserAllItems().AddItem(tempItem);
					}
				}
			}
		}
	}

	if (!selectedMainItem.IsEmpty()) leftQualityMain = TryItemForgePtr->left_main_item_count;
	if (!selected1stMaterial.IsEmpty())leftQualitySub1 = TryItemForgePtr->left_material_item_count1;
	if (!selected2ndMaterial.IsEmpty())leftQualitySub2 = TryItemForgePtr->left_material_item_count2;
	if (!selected3rdMaterial.IsEmpty())leftQualitySub3 = TryItemForgePtr->left_material_item_count3;
	if (!selected4thMaterial.IsEmpty())leftQualitySub4 = TryItemForgePtr->left_material_item_count4;

	LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();

	if (TryItemForgePtr->current_gold > 0)BladeIIGameImpl::GetClientDataStore().SetGoldAmount(TryItemForgePtr->current_gold);
	if (TryItemForgePtr->current_gem > 0)BladeIIGameImpl::GetClientDataStore().SetGemAmount(TryItemForgePtr->current_gem);
	if (TryItemForgePtr->current_dark_crystal > 0)BladeIIGameImpl::GetClientDataStore().SetDarkCrystal(TryItemForgePtr->current_dark_crystal);

	dataRecieved = true;

	if (aniEnd == true)
	{
		if (dataRecieved == true)
		{
			ReadytoShowResult();
		}
	}
}

void UB2UIItemForge::SetBasicData()
{
	SetAvailableItemList();

	if (selectedForgeID < 0)
	{
		if (O_PleaseSelectForgeStuff.IsValid())
		{
			O_PleaseSelectForgeStuff->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (O_PleaseSelectForgeStuff.IsValid())
		{
			O_PleaseSelectForgeStuff->SetVisibility(ESlateVisibility::Hidden);
		}

		SetIngredientList();

		SetElementSize(maxQuantity);
		if (TB_MaximumQuantity.IsValid())
			TB_MaximumQuantity->SetText(FText::FromString(FString::FromInt(isUnlimited == true ? AVAILABLE_FORGE_QUANTITY : maxQuantity)));
	}
}

void UB2UIItemForge::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_PleaseSelectForgeStuff.IsValid())
		TB_PleaseSelectForgeStuff->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_PleaseSelectFactoryList")));

	if (TB_Warning_Forge.IsValid())
		TB_Warning_Forge->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Alert_Forge")));
	if (TB_Warning_Upgrade.IsValid())
		TB_Warning_Upgrade->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Alert_Upgrade")));

	if (TB_ListIsEmpty.IsValid())
		TB_ListIsEmpty->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_ListIsEmpty")));

	if (TB_AvailableItemList.IsValid())
		TB_AvailableItemList->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_AvailableFactory")));
	if (TB_Price.IsValid())
		TB_Price->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_ResourcePrice")));
	if (TB_ForgeLimit.IsValid())
		TB_ForgeLimit->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Limitation")));

	if(TB_PopupTitle.IsValid())
		TB_PopupTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Filter_Title")));
	if (UIP_ForgeFilter.IsValid())
		UIP_ForgeFilter->SetFilterTitle(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_Filter_Title"))));

	if (TB_Filter.IsValid())
		TB_Filter->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Filter_Name")));
	if (F_TB_Apply.IsValid())
		F_TB_Apply->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Filter_Button")));
	if (F_TB_Reset.IsValid())
		F_TB_Reset->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Reset")));

	if (X_TB_ResultTitle.IsValid())
		X_TB_ResultTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_ResultTitle")));
	if (X_TB_Result.IsValid())
		X_TB_Result->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_ResultCounter")));
	if (X_TB_Confirm.IsValid())
		X_TB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_ResultConfirm")));
	if (X_TB_Skip.IsValid())
		X_TB_Skip->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemFactory_Skip")));
}

void UB2UIItemForge::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIItemForge::OnClickedBTNForgeMenu()
{
	if (selectedForgeType != EItemForgeType::Forge)
	{
		if (TB_Warning_Forge.IsValid())
			TB_Warning_Forge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (TB_Warning_Upgrade.IsValid())
			TB_Warning_Upgrade->SetVisibility(ESlateVisibility::Collapsed);

		if (HB_ShowingLimitNumber.IsValid())
		{
			HB_ShowingLimitNumber->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		selectedForgeType = EItemForgeType::Forge;
		ResetForgeSetting();
		SetTabList();
		SetBasicData();
	}
}

void UB2UIItemForge::OnClickedBTNUpgradeMenu()
{
	if (selectedForgeType != EItemForgeType::Upgrade)
	{
		if (TB_Warning_Forge.IsValid())
			TB_Warning_Forge->SetVisibility(ESlateVisibility::Collapsed);
		if (TB_Warning_Upgrade.IsValid())
			TB_Warning_Upgrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		selectedForgeType = EItemForgeType::Upgrade;
		ResetForgeSetting();
		SetTabList();
		SetBasicData();

		if (HB_ShowingLimitNumber.IsValid())
		{
			HB_ShowingLimitNumber->SetVisibility(ESlateVisibility::Collapsed);
		}

		HideQuantityAdjust(true);
	}
}

void UB2UIItemForge::OnClickedBTNGladiator()
{
	if (selectedClass != EPCClass::EPC_Gladiator)
	{
		selectedClass = EPCClass::EPC_Gladiator;
		SetButtonClick(EPCClass::EPC_Gladiator);
	}
}

void UB2UIItemForge::OnClickedBTNAssassin()
{
	if (selectedClass != EPCClass::EPC_Assassin)
	{
		selectedClass = EPCClass::EPC_Assassin;
		SetButtonClick(EPCClass::EPC_Assassin);
	}
}

void UB2UIItemForge::OnClickedBTNWizard()
{
	if (selectedClass != EPCClass::EPC_Wizard)
	{
		selectedClass = EPCClass::EPC_Wizard;
		SetButtonClick(EPCClass::EPC_Wizard);
	}
}

void UB2UIItemForge::OnClickedBTNFighter()
{
	if (selectedClass != EPCClass::EPC_Fighter)
	{
		selectedClass = EPCClass::EPC_Fighter;
		SetButtonClick(EPCClass::EPC_Fighter);
	}
}

void UB2UIItemForge::OnClickedBTNIncQuantity()
{
	if (quantity < maxQuantity)
		quantity++;
	if (SD_ControlQuantity.IsValid())
		SD_ControlQuantity->SetValue(ConvertItemCountToSliderValue(quantity));

	if (TB_CurrentQuantity.IsValid())
	{
		TB_CurrentQuantity->SetText(FText::FromString(FString::FromInt(quantity)));
	}

	AdjustQuantity();
}

void UB2UIItemForge::OnClickedBTNDecQuantity()
{
	if (quantity > 1)
		quantity--;

	if (SD_ControlQuantity.IsValid())
		SD_ControlQuantity->SetValue(ConvertItemCountToSliderValue(quantity));

	if (TB_CurrentQuantity.IsValid())
	{
		TB_CurrentQuantity->SetText(FText::FromString(FString::FromInt(quantity)));
	}

	AdjustQuantity();
}

void UB2UIItemForge::OnClickedBTNMaxQuantity()
{
	quantity = maxQuantity;

	if (SD_ControlQuantity.IsValid())
		SD_ControlQuantity->SetValue(ConvertItemCountToSliderValue(quantity));

	if (TB_CurrentQuantity.IsValid())
	{
		TB_CurrentQuantity->SetText(FText::FromString(FString::FromInt(quantity)));
	}

	AdjustQuantity();
}

void UB2UIItemForge::OnClickedBTNResultItemDetail()
{
	int64 resultID = 0;
	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (selectedForgeID == forgeTable[forgeTableIndex].forgeID)
		{
			resultID = forgeTable[forgeTableIndex].resultItemID;
			break;
		}
	}

	if (FItemRefIDHelper::ExtractItemStarGradeFromRefID(resultID) > 0)
	{
		if (UIP_ItemDetail.IsValid())
		{
			EItemClass itemClass = FItemRefIDHelper::ExtractItemClassFromRefID(resultID);
			FItemSurpassLevel InSurpassInfo = BladeIIGameImpl::GetClientDataStore().GetSurpassLevelData(FItemRefIDHelper::ExtractItemStarGradeFromRefID(resultID));
			UIP_ItemDetail->SetDetailViewSingleItem(resultID, FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(resultID), InSurpassInfo.MaxSurpassLevel);


			UIP_ItemDetail->SetCollectBookIcon(itemClass, resultID);
			if (auto* BGMtrl = StaticFindItemMiscInfo()->GetItemIconBGMtrl(FItemRefIDHelper::ExtractItemStarGradeFromRefID(resultID), true, false))	//力傍瞪 酒捞袍篮 檬岿捞 救登绢乐促绊 啊沥 (扁裙苞 抗扁登瘤 臼澜)
			{
				UIP_ItemDetail->SetItemIconDefaultTemplate(FItemRefIDHelper::ExtractItemStarGradeFromRefID(resultID), BGMtrl, EItemCollectionState::Collected);
				UIP_ItemDetail->SetItemIconStarGrade(FItemRefIDHelper::ExtractItemStarGradeFromRefID(resultID), false);

			}

			SetvisibilityOfDetail(true);
		}
	}
	else
	{
		UB2UIManager* const UIMgrInst = UB2UIManager::GetInstance();
		if (!UIMgrInst)	return;

		auto* pUI = UIMgrInst->OpenUI<UB2UIGoodsShortcutTooltipPopup>(UIFName::GoodsShortcutTooltipPopup);
		if (pUI) pUI->SetItemInfo(resultID, EGoodsButtonType::EGoodsButtonType_Info);
	}
}

float UB2UIItemForge::ConvertItemCountToSliderValue(int32 InItemCount)
{
	return (InItemCount - 1) * GetDivideElement();
}

int32 UB2UIItemForge::ConvertSliderValueToItemCount(float InSliderValue)
{
	int32 nValue = InSliderValue == 1 ? maxQuantity : (int32)(InSliderValue * maxQuantity);

	return FMath::Clamp(nValue, 1, maxQuantity);
}

void UB2UIItemForge::SetElementSize(int32 InMaxCount)
{
	ElementSize = (1.f / (float)FMath::Max(1, InMaxCount - 1));

	if (SD_ControlQuantity.IsValid())
		SD_ControlQuantity->SetStepSize(ElementSize);
}

void UB2UIItemForge::OnChangeItemCountSliderBar(float fValue)
{
	// 0俺扼绰巴篮 绝扁锭巩俊 0捞窍老 版快 1俺肺 焊沥秦霖促.
	if (fValue <= 0.f)
	{
		if (SD_ControlQuantity.IsValid())
			SD_ControlQuantity->SetValue(GetDivideElement());

		return;
	}
}

float UB2UIItemForge::GetDivideElement()
{
	return ElementSize;
}

void UB2UIItemForge::SetItemCountAndUpdateWidget(int32 InItemCount)
{
	if (InItemCount >= 1)
		quantity = InItemCount;
	else
		quantity = 1;

	if (TB_CurrentQuantity.IsValid())
	{
		TB_CurrentQuantity->SetText(FText::FromString(FString::FromInt(quantity)));
	}

	AdjustQuantity();
	CheckIsEverythingOkay();
}

int32 UB2UIItemForge::GetPreQuantityItemCount()
{
	return FMath::Max(quantity - 1, 1);
}

int32 UB2UIItemForge::GetNextQuantityItemCount()
{
	return FMath::Min(quantity + 1, maxQuantity);
}

int32 UB2UIItemForge::GetForgeItemCount()
{
	return quantity;
}

int32 UB2UIItemForge::GetForgeItemMaxCount()
{
	return maxQuantity;
}

void UB2UIItemForge::OnClickedBTNExecute()
{
	resultItems.Empty();

	for (int32 periodIndex = 0; periodIndex < PeriodLimitStatus.Num(); periodIndex++)
	{
		if (PeriodLimitStatus[periodIndex].forgeID == selectedForgeID)
		{
			endTime = PeriodLimitStatus[periodIndex].endTime;
			FDateTime DateNow = UB2GameInstance::GetLocalNow();

			if (DateNow.GetTicks() > endTime.GetTicks())
			{
				OutofTimeForLimited();
				ResetForgeSetting();
				SetTabList();
				SetBasicData();
				return;
			}
		}
	}

	data_trader::Retailer::GetInstance().RequestItemForge(
		(int64)selectedForgeID,
		(int64)quantity,
		selectedMainItem.InstanceUID,
		selected1stMaterial.InstanceUID,
		selected2ndMaterial.InstanceUID,
		selected3rdMaterial.InstanceUID,
		selected4thMaterial.InstanceUID,
		selectedMainItem.ItemRefID,
		selected1stMaterial.ItemRefID,
		selected2ndMaterial.ItemRefID,
		selected3rdMaterial.ItemRefID,
		selected4thMaterial.ItemRefID
	);

	if (HB_ProgressBar.IsValid())
		HB_ProgressBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (HB_Resource.IsValid())
		HB_Resource->SetVisibility(ESlateVisibility::Hidden);
	if (TB_Price.IsValid())
		TB_Price->SetVisibility(ESlateVisibility::Hidden);

	if (BTN_MasterModal.IsValid())
		BTN_MasterModal->SetVisibility(ESlateVisibility::Visible);

	//力累 滚瓢阑 穿福绰 鉴埃何磐 搬苞芒阑 摧阑 锭 鳖瘤 搬苞芒阑 力寇茄 葛电 滚瓢 涝仿阑 阜疽促.
	//角力肺绰 辑滚俊辑 单捞磐甫 罐绰 溜矫 阜阑 鞘夸啊 绝瘤父 阂鞘夸茄 炼累栏肺 牢秦 盔摹臼绰 坷幅啊 唱绰吧 阜扁 困秦 弊锭 鳖瘤 阜绰促.
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();
	if (pUIManager)
	{
		UB2UIHeader* pUI = NULL;
		pUI = pUIManager->GetUI<UB2UIHeader>(UIFName::Header);
		if (pUI) pUI->UpdateUIVisibleVerticalMode(true);
	}

	OnExecute_BP();
}

void UB2UIItemForge::OnClickedBTNFilter()
{
	if (CP_Filter.IsValid())
		CP_Filter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (UIP_ForgeFilter.IsValid())
		UIP_ForgeFilter->InitData();
}

void UB2UIItemForge::OnClickedBTNFilterClose()
{
	UIP_ForgeFilter->LoadFilterInfo(TEXT("ItemForgeFilter"));

	if (CP_Filter.IsValid())
		CP_Filter->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIItemForge::OnClickedBTNFilterApply()
{
	UIP_ForgeFilter->GetSelectedFilters(filterInvenType, filterStarGrade);
	UIP_ForgeFilter->SaveFilterInfo(TEXT("ItemForgeFilter"));

	ResetForgeSettingForFilter();
	SetTabList();
	SetBasicData();

	OnClickedBTNFilterClose();
}

void UB2UIItemForge::OnClickedBTNFilterReset()
{
	if (UIP_ForgeFilter.IsValid())
		UIP_ForgeFilter->ResetFilter();
}

void UB2UIItemForge::OnAnyForgeFilterCBChanged(bool InCheck)
{
	
}

void UB2UIItemForge::OnClickedBTNSkipResult()
{
	resultSkip = true;
}

void UB2UIItemForge::OnClickedBTNResultConfirm()
{
	resultIndex = 0;
	refundIndex = 0;

	if (CP_Result.IsValid())
		CP_Result->SetVisibility(ESlateVisibility::Collapsed);

	data_trader::Retailer::GetInstance().RequestGetAccountItem(CliToSvrPCClassType(selectedClass), 0);

	if (!selectedMainItem.IsEmpty()) if (leftQualityMain == 0) BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().RemoveItem(selectedMainItem.InstanceUID);
	if (!selected1stMaterial.IsEmpty()) if (leftQualitySub1 == 0) BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().RemoveItem(selected1stMaterial.InstanceUID);
	if (!selected2ndMaterial.IsEmpty()) if (leftQualitySub2 == 0) BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().RemoveItem(selected2ndMaterial.InstanceUID);
	if (!selected3rdMaterial.IsEmpty()) if (leftQualitySub3 == 0) BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().RemoveItem(selected3rdMaterial.InstanceUID);
	if (!selected4thMaterial.IsEmpty()) if (leftQualitySub4 == 0) BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().RemoveItem(selected4thMaterial.InstanceUID);

	if (UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser())
	{
		DocUser->UpdateUserData();
	}

	LobbyAllItemsAddedClass<>::GetInstance().Signal();
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();

	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_Gladiator);
	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_Assassin);
	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_Wizard);
	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_Fighter);
	BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(EPCClass::EPC_End);


	for (int32 limitStatusIndex = 0; limitStatusIndex < LimitStatus.Num(); limitStatusIndex++)
	{
		if (selectedLimitID == LimitStatus[limitStatusIndex].limitID)
		{
			switch (LimitStatus[limitStatusIndex].limitType)
			{
			case EItemForgeLimitType::NumberOfTry:
				LimitStatus[limitStatusIndex].limitQuantity -= quantity;
				break;
			case EItemForgeLimitType::NumberOfSuccess:
				LimitStatus[limitStatusIndex].limitQuantity -= totalSuccess + totalHugeSuccess;
				break;
			}
		}
	}

	UB2UIManager* pUIManager = UB2UIManager::GetInstance();
	if (pUIManager)
	{
		UB2UIHeader* pUI = NULL;
		pUI = pUIManager->GetUI<UB2UIHeader>(UIFName::Header);
		if (pUI) pUI->UpdateUIVisibleVerticalMode(false);
	}
	resultSkip = false;
	aniEnd = false;
	dataRecieved = false;

	EItemForgeType tempSelectedForgeType = selectedForgeType;
	EPCClass tempSelectedClass = selectedClass;
	int32 tempSelectedTabID = selectedForgeTab;
	int32 tempSelectedForgeID=selectedForgeID;
	int32 tempSelectedLimitID = selectedLimitID;

	ResetForgeSetting();
	selectedForgeType = tempSelectedForgeType;
	selectedClass = tempSelectedClass;
	selectedForgeTab = tempSelectedTabID;
	SetTabList();
	selectedForgeID = tempSelectedForgeID;
	selectedLimitID = tempSelectedLimitID;

	OnManualForgeListSet();


	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (forgeTable[forgeTableIndex].forgeID == selectedForgeID)
		{
			for (int32 periodIndex = 0; periodIndex < PeriodLimitStatus.Num(); periodIndex++)
			{
				if (PeriodLimitStatus[periodIndex].forgeID == selectedForgeID)
				{
					startTime = PeriodLimitStatus[periodIndex].startTime;
					endTime = PeriodLimitStatus[periodIndex].endTime;
					FDateTime DateNow = UB2GameInstance::GetLocalNow();

					if (DateNow.GetTicks() <= endTime.GetTicks())
					{
						if (CP_PeriodLimit.IsValid())
							CP_PeriodLimit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

						if (TB_LimitStartTime.IsValid())
							TB_LimitStartTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, TEXT("Factory_LimitDate_1")), FText::FromString(FString::Printf(TEXT("%d"), startTime.GetYear())), FText::AsNumber(startTime.GetMonth()), FText::AsNumber(startTime.GetDay()), FText::AsNumber(startTime.GetHour()), FText::AsNumber(startTime.GetMinute())));
						if (TB_LimitEndTime.IsValid())
							TB_LimitEndTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, TEXT("Factory_LimitDate_2")), FText::FromString(FString::Printf(TEXT("%d"), endTime.GetYear())), FText::AsNumber(endTime.GetMonth()), FText::AsNumber(endTime.GetDay()), FText::AsNumber(endTime.GetHour()), FText::AsNumber(endTime.GetMinute())));
					}
				}
			}
		}
	}
}

void UB2UIItemForge::SetButtonClick(EPCClass InEPCClass)
{
	EventChangedHeroMgmtPCClass(InEPCClass);
	ResetForgeSetting();
	SetTabList();
	SetBasicData();
}

void UB2UIItemForge::OnChangeLevelSlider(float InValue)
{
	quantity = FMath::Clamp<int32>((int32)(InValue * maxQuantity), 1, maxQuantity);
}

void UB2UIItemForge::SetvisibilityOfDetail(bool showing)
{
	if (showing == true)
	{
		if (UIP_ItemDetail.IsValid() && BTN_ItemDetailBackGround.IsValid())
		{
			BTN_ItemDetailBackGround->SetVisibility(ESlateVisibility::Visible);
			UIP_ItemDetail->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		if (UIP_ItemDetail.IsValid() && BTN_ItemDetailBackGround.IsValid())
		{
			BTN_ItemDetailBackGround->SetVisibility(ESlateVisibility::Collapsed);
			UIP_ItemDetail->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIItemForge::OnClickedTabButton(class UB2UIItemForgeTabButton* ButtonWidget)
{
	if (ButtonWidget)
	{
		//徘 傈券
		if (selectedForgeTab != ButtonWidget->GetTabID())
		{
			ResetForgeSetting();
			selectedForgeTab = ButtonWidget->GetTabID();
			SetTabList();
			SetBasicData();
		}
	}
}

void UB2UIItemForge::OnClickedForgeList(class UB2UIItemForgeListButton* ButtonWidget)
{
	if (ButtonWidget)
	{
		if (selectedForgeID == ButtonWidget->GetListID()) return;
		int32 tempselectedID = selectedForgeID;
		int32 tempselectedTab = selectedForgeTab;
		ResetForgeSetting();
		selectedForgeID = tempselectedID;
		selectedForgeTab = tempselectedTab;


		if (CP_Right.IsValid())
		{
			CP_Right->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		if (O_PleaseSelectForgeStuff.IsValid())
			O_PleaseSelectForgeStuff->SetVisibility(ESlateVisibility::Collapsed);

		// 力累 且 酒捞袍 急琶
		if (selectedForgeID != ButtonWidget->GetListID())
		{
			selectedForgeID = ButtonWidget->GetListID();

			if (SB_IngredientList.IsValid())
				if (SB_IngredientList->HasAnyChildren())
					SB_IngredientList->ClearChildren();

			//insert material and detail data function here

			int32 rateOfWin = 0;
			int32 resultRefID = 0;
			resultQuantity = 0;
			bonusQuantity = 0;

			for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
			{
				if (forgeTable[forgeTableIndex].forgeID == selectedForgeID)
				{
					for (int32 periodIndex = 0; periodIndex < PeriodLimitStatus.Num(); periodIndex++)
					{
						if (PeriodLimitStatus[periodIndex].forgeID == selectedForgeID)
						{
							startTime = PeriodLimitStatus[periodIndex].startTime;
							endTime = PeriodLimitStatus[periodIndex].endTime;
							FDateTime DateNow = UB2GameInstance::GetLocalNow();

							if (DateNow.GetTicks()> endTime.GetTicks())
							{
								OutofTimeForLimited();
								ResetForgeSetting();
								SetTabList();
								SetBasicData();
								return;
							}
							else
							{
								if (CP_PeriodLimit.IsValid())
									CP_PeriodLimit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

								if(TB_LimitStartTime.IsValid())
									TB_LimitStartTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, TEXT("Factory_LimitDate_1")), FText::FromString(FString::Printf(TEXT("%d"), startTime.GetYear())), FText::AsNumber(startTime.GetMonth()), FText::AsNumber(startTime.GetDay()), FText::AsNumber(startTime.GetHour()), FText::AsNumber(startTime.GetMinute())));
								if (TB_LimitEndTime.IsValid())
									TB_LimitEndTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, TEXT("Factory_LimitDate_2")), FText::FromString(FString::Printf(TEXT("%d"), endTime.GetYear())), FText::AsNumber(endTime.GetMonth()), FText::AsNumber(endTime.GetDay()), FText::AsNumber(endTime.GetHour()), FText::AsNumber(endTime.GetMinute())));
							}
						}
					}

					selectedLimitID = forgeTable[forgeTableIndex].limitID;
					rateOfWin = (int32)((forgeTable[forgeTableIndex].successRate + forgeTable[forgeTableIndex].hugeSuccessRate) / 100);
					resultRefID = forgeTable[forgeTableIndex].resultItemID;
					resultQuantity = forgeTable[forgeTableIndex].resultItemQuantity;
					bonusQuantity = forgeTable[forgeTableIndex].hugeSuccessAdditionalItemQuantity;
					if (FItemRefIDHelper::IsEquipmentItemByRefID(forgeTable[forgeTableIndex].resultItemID))
					{
						maxQuantity = 1;
						quantity = 1;
						HideQuantityAdjust(true);
					}
				}
			}

			SetAvailableItemList();
			SetIngredientList();

			if (SD_ControlQuantity.IsValid())
				SD_ControlQuantity->SetValue(ConvertItemCountToSliderValue(quantity));

			SetRequiredGoods();
			SetLimitation();

			if (maxQuantity < 1) maxQuantity = 1;
			SetElementSize(maxQuantity);
			if (TB_MaximumQuantity.IsValid())
				TB_MaximumQuantity->SetText(FText::FromString(FString::FromInt(isUnlimited == true ? AVAILABLE_FORGE_QUANTITY : maxQuantity)));

			if (TB_MaximumQuantity.IsValid())
			{
				TB_MaximumQuantity->SetText(FText::FromString(FString::FromInt(maxQuantity)));
			}
			if (TB_CurrentQuantity.IsValid())
			{
				TB_CurrentQuantity->SetText(FText::FromString(FString::FromInt(quantity)));
			}

			if (TB_SuccessRate.IsValid())
			{
				TB_SuccessRate->SetText(FText::FromString(FString::FromInt(rateOfWin) + "%"));
			}

			UMaterialInstance* IconMtrl = nullptr;
			FText ItemName;
			StaticFindItemInfo()->GetItemIconAndName(resultRefID, IconMtrl, ItemName);

			UIP_ResultItemIcon->Initialize();
			UIP_ResultItemIcon->NativeConstruct();
			UIP_ResultItemIcon->InitItemForgeSlot(EItemForgeSlot::MainMaterialItem, EItemForgeItemIconUsage::EIFI_TargetItem, resultRefID);

			UIP_ResultItemIcon->UpdateTargetItemData(resultRefID, resultQuantity * quantity);

		}
	}
}

void UB2UIItemForge::OnManualForgeListSet()
{
	if (selectedForgeID < 0) return;

	int32 tempselectedID = selectedForgeID;
	int32 tempselectedTab = selectedForgeTab;
	ResetForgeSetting();
	selectedForgeID = tempselectedID;
	selectedForgeTab = tempselectedTab;


	if (CP_Right.IsValid())
	{
		CP_Right->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (O_PleaseSelectForgeStuff.IsValid())
		O_PleaseSelectForgeStuff->SetVisibility(ESlateVisibility::Collapsed);

	if (SB_IngredientList.IsValid())
		if (SB_IngredientList->HasAnyChildren())
			SB_IngredientList->ClearChildren();

	//insert material and detail data function here

	int32 rateOfWin = 0;
	int32 resultRefID = 0;
	resultQuantity = 0;
	bonusQuantity = 0;

	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (forgeTable[forgeTableIndex].forgeID == selectedForgeID)
		{
			selectedLimitID = forgeTable[forgeTableIndex].limitID;
			rateOfWin = (int32)((forgeTable[forgeTableIndex].successRate + forgeTable[forgeTableIndex].hugeSuccessRate) / 100);
			resultRefID = forgeTable[forgeTableIndex].resultItemID;
			resultQuantity = forgeTable[forgeTableIndex].resultItemQuantity;
			bonusQuantity = forgeTable[forgeTableIndex].hugeSuccessAdditionalItemQuantity;
			if (FItemRefIDHelper::IsEquipmentItemByRefID(forgeTable[forgeTableIndex].resultItemID))
			{
				maxQuantity = 1;
				quantity = 1;
				HideQuantityAdjust(true);
			}
		}
	}

	SetAvailableItemList();
	SetIngredientList();

	if (SD_ControlQuantity.IsValid())
		SD_ControlQuantity->SetValue(ConvertItemCountToSliderValue(quantity));

	SetRequiredGoods();
	SetLimitation();

	if (maxQuantity < 1) maxQuantity = 1;
	SetElementSize(maxQuantity);
	if (TB_MaximumQuantity.IsValid())
		TB_MaximumQuantity->SetText(FText::FromString(FString::FromInt(isUnlimited == true ? AVAILABLE_FORGE_QUANTITY : maxQuantity)));

	if (TB_MaximumQuantity.IsValid())
	{
		TB_MaximumQuantity->SetText(FText::FromString(FString::FromInt(maxQuantity)));
	}
	if (TB_CurrentQuantity.IsValid())
	{
		TB_CurrentQuantity->SetText(FText::FromString(FString::FromInt(quantity)));
	}

	if (TB_SuccessRate.IsValid())
	{
		TB_SuccessRate->SetText(FText::FromString(FString::FromInt(rateOfWin) + "%"));
	}

	UMaterialInstance* IconMtrl = nullptr;
	FText ItemName;
	StaticFindItemInfo()->GetItemIconAndName(resultRefID, IconMtrl, ItemName);

	UIP_ResultItemIcon->Initialize();
	UIP_ResultItemIcon->NativeConstruct();
	UIP_ResultItemIcon->InitItemForgeSlot(EItemForgeSlot::MainMaterialItem, EItemForgeItemIconUsage::EIFI_TargetItem, resultRefID);

	UIP_ResultItemIcon->UpdateTargetItemData(resultRefID, resultQuantity * quantity);

}

void UB2UIItemForge::OnSetMaterialID(EItemForgeSlot materialSlot, FB2Item Selecteditem, bool isReady, int32 singleRequiredQuantity, bool thisContainsGear, int32 weHaveThatMany)
{
	maxQuantity = 1;

	switch (materialSlot)
	{
	case EItemForgeSlot::MainMaterialItem: selectedMainItem = Selecteditem; mainReady = isReady;  mainQuantity = singleRequiredQuantity; owningMainQuantity = weHaveThatMany; break;
	case EItemForgeSlot::Sub1stMaterialItem: selected1stMaterial = Selecteditem; sub1Ready = isReady; sub1Quantity = singleRequiredQuantity; owningSub1Quantity = weHaveThatMany; break;
	case EItemForgeSlot::Sub2ndMaterialItem: selected2ndMaterial = Selecteditem; sub2Ready = isReady; sub2Quantity = singleRequiredQuantity; owningSub2Quantity = weHaveThatMany; break;
	case EItemForgeSlot::Sub3rdMaterialItem: selected3rdMaterial = Selecteditem; sub3Ready = isReady; sub3Quantity = singleRequiredQuantity; owningSub3Quantity = weHaveThatMany; break;
	case EItemForgeSlot::Sub4thMaterialItem: selected4thMaterial = Selecteditem; sub4Ready = isReady; sub4Quantity = singleRequiredQuantity; owningSub4Quantity = weHaveThatMany; break;
	}
	if (thisContainsGear == true)
	{
		HideQuantityAdjust(true);
	}

	TArray<int32> findMin;
	if (mainReady) findMin.Add(owningMainQuantity / mainQuantity);
	if (sub1Ready) findMin.Add(owningSub1Quantity / sub1Quantity);
	if (sub2Ready) findMin.Add(owningSub2Quantity / sub2Quantity);
	if (sub3Ready) findMin.Add(owningSub3Quantity / sub3Quantity);
	if (sub4Ready) findMin.Add(owningSub4Quantity / sub4Quantity);


	//咯扁辑 TMap阑 荤侩 救茄 捞蜡绰 绝促. 窜鉴洒 俺惯 寸矫俊 TMap阑 绢痘霸 荤侩秦具 窍绰瘤甫 隔耳绊, 沥纺捞 鞘夸沁扁 锭巩俊 烙矫肺 累己登菌促.
	for (int32 repeatIndex = 0; repeatIndex < findMin.Num(); repeatIndex++)
	{
		for (int32 sortingIndex = 1; sortingIndex < findMin.Num(); sortingIndex++)
		{
			if (findMin[sortingIndex - 1] > findMin[sortingIndex])
			{
				findMin.Swap(sortingIndex - 1, sortingIndex);
			}
		}
	}

	if (findMin.Num() > 0)
	{
		maxQuantity = findMin[0];
		lowestQuantity = findMin[0];
	}

	AdjustQuantity(true);

	SetLimitation();

	if (maxQuantity > AVAILABLE_FORGE_QUANTITY)
		maxQuantity = AVAILABLE_FORGE_QUANTITY;

	if (maxQuantity < 1) maxQuantity = 1;
	SetElementSize(maxQuantity);
	if (TB_MaximumQuantity.IsValid())
		TB_MaximumQuantity->SetText(FText::FromString(FString::FromInt(maxQuantity)));

	if (quantity > maxQuantity)
		quantity = maxQuantity;

	if (SD_ControlQuantity.IsValid())
		SD_ControlQuantity->SetValue(ConvertItemCountToSliderValue(quantity));

	if (TB_CurrentQuantity.IsValid())
	{
		TB_CurrentQuantity->SetText(FText::FromString(FString::FromInt(quantity)));
	}

	CheckIsEverythingOkay();
}

void UB2UIItemForge::UpdatePanelWidget()
{

}

void UB2UIItemForge::SetTabList()
{
	if (HB_TabList.IsValid())
		if (HB_TabList->HasAnyChildren())
			HB_TabList->ClearChildren();
	
	TArray<FItemForgeInfo> tempArray;
	int32 lowestTab = INT_MAX;
	int32 tempIndex = 0;
	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (forgeTable[forgeTableIndex].forgeTab < lowestTab)
		{
			lowestTab = forgeTable[forgeTableIndex].forgeTab;
			tempIndex = forgeTableIndex;
		}
	}

	if (forgeTable.Num() != 0)
	{
		tempArray.Add(forgeTable[tempIndex]);

		if (selectedForgeTab < 0)
			selectedForgeTab = forgeTable[tempIndex].forgeTab;

		for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
		{
			lowestTab = INT_MAX;
			if (forgeTable[forgeTableIndex].forgeTab < lowestTab)
			{
				bool isThere = false;
				for (int32 tempArrayIndex = 0; tempArrayIndex < tempArray.Num(); tempArrayIndex++)
				{
					if (forgeTable[forgeTableIndex].forgeTab == tempArray[tempArrayIndex].forgeTab)
						isThere = true;
				}
				if (isThere == false)
				{
					tempArray.Add(forgeTable[forgeTableIndex]);
				}
			}
		}
	}

	if (selectedForgeType == EItemForgeType::Forge)
	{
		if (tabArrayForForge.Num() == 0)
		{
			for (int32 tempArrayIndex = 0; tempArrayIndex < tempArray.Num(); tempArrayIndex++)
			{
				tabArrayForForge.Add(tempArray[tempArrayIndex]);
			}
		}

		for (int32 tempArrayIndex = 0; tempArrayIndex < tabArrayForForge.Num(); tempArrayIndex++)
		{
			UB2UIItemForgeTabButton* ButtonWidget = CreateWidget<UB2UIItemForgeTabButton>(GetOwningPlayer(), TabButtonBP);
			ButtonWidget->Init();
			ButtonWidget->SetOwnerItemForgeUI(this, tabArrayForForge[tempArrayIndex].forgeTab);
			if (tabArrayForForge[tempArrayIndex].forgeTab == selectedForgeTab)
				ButtonWidget->YouHaveBeenChosen(true);
			HB_TabList->AddChild(ButtonWidget);
		}
	}

	else
	{
		if (tabArrayForUpgrade.Num() == 0)
		{
			for (int32 tempArrayIndex = 0; tempArrayIndex < tempArray.Num(); tempArrayIndex++)
			{
				tabArrayForUpgrade.Add(tempArray[tempArrayIndex]);
			}
		}

		for (int32 tempArrayIndex = 0; tempArrayIndex < tabArrayForUpgrade.Num(); tempArrayIndex++)
		{
			UB2UIItemForgeTabButton* ButtonWidget = CreateWidget<UB2UIItemForgeTabButton>(GetOwningPlayer(), TabButtonBP);
			ButtonWidget->Init();
			ButtonWidget->SetOwnerItemForgeUI(this, tabArrayForUpgrade[tempArrayIndex].forgeTab);
			if (tabArrayForUpgrade[tempArrayIndex].forgeTab == selectedForgeTab)
				ButtonWidget->YouHaveBeenChosen(true);
			HB_TabList->AddChild(ButtonWidget);
		}
	}
}

void UB2UIItemForge::SetAvailableItemList()
{
	if (SB_AvailableItemList.IsValid())
		if (SB_AvailableItemList->HasAnyChildren())
			SB_AvailableItemList->ClearChildren();

	TArray<FItemForgeInfo> tempArray;
	int32 lowestGroup = INT_MAX;
	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (forgeTable[forgeTableIndex].groupID < lowestGroup && forgeTable[forgeTableIndex].forgeTab == selectedForgeTab)
			lowestGroup = forgeTable[forgeTableIndex].groupID;
	}

	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (lowestGroup == forgeTable[forgeTableIndex].groupID && forgeTable[forgeTableIndex].forgeTab == selectedForgeTab)
		{
			if (forgeTable[forgeTableIndex].PCClassType == selectedClass || forgeTable[forgeTableIndex].PCClassType == EPCClass::EPC_End)
				tempArray.Add(forgeTable[forgeTableIndex]);
		}
	}

	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		lowestGroup = INT_MAX;
		if (forgeTable[forgeTableIndex].groupID < lowestGroup && forgeTable[forgeTableIndex].forgeTab == selectedForgeTab)
		{
			bool isThere = false;
			for (int32 tempArrayIndex = 0; tempArrayIndex < tempArray.Num(); tempArrayIndex++)
			{
				if (forgeTable[forgeTableIndex].forgeID == tempArray[tempArrayIndex].forgeID)
					isThere = true;
			}
			if (isThere == false)
			{
				if (forgeTable[forgeTableIndex].PCClassType == selectedClass || forgeTable[forgeTableIndex].PCClassType == EPCClass::EPC_End)
					tempArray.Add(forgeTable[forgeTableIndex]);
			}
		}
	}

	//弊缝 沥纺, 酒捞袍 沥纺捞 悼矫俊 利侩登搁辑 搬苞甫 拳搁俊 谎妨拎具 沁扁 锭巩俊 腹捞 歹反霸 内爹登菌促.
	//悼矫俊 俺惯 寸矫俊 TMap俊 措秦 舅瘤 给沁绊, 变鞭窍霸 累诀窍蠢扼 力老 刚历 且荐 乐绰吧 利侩窍看促.
	if (tempArray.Num() > 1)
	{
		for (int32 repeatIndex = 0; repeatIndex < tempArray.Num(); repeatIndex++)
		{
			for (int32 sortingArrayIndex = 1; sortingArrayIndex < tempArray.Num(); sortingArrayIndex++)
			{
				if (tempArray[sortingArrayIndex - 1].groupSort > tempArray[sortingArrayIndex].groupSort)
				{
					tempArray.Swap(sortingArrayIndex - 1, sortingArrayIndex);
				}
			}
		}

		int32 currentGroup = 0;

		for (int32 tempArrayIndex = 0; tempArrayIndex < tempArray.Num(); tempArrayIndex++)
		{
			currentGroup = tempArray[tempArrayIndex].groupID;

			for (int32 repeatIndex = 0; repeatIndex < tempArray.Num(); repeatIndex++)
			{
				for (int32 arraySortingIndex = 1; arraySortingIndex < tempArray.Num(); arraySortingIndex++)
				{
					if (tempArray[arraySortingIndex - 1].itemSort > tempArray[arraySortingIndex].itemSort &&
						tempArray[arraySortingIndex].groupID == currentGroup &&
						tempArray[arraySortingIndex - 1].groupID == currentGroup)
					{
						tempArray.Swap(arraySortingIndex - 1, arraySortingIndex);
					}
				}
			}
		}
	}

	int32 currentGroup = 0;
	TArray<int32> listedGroup;
	for (int32 tempArrayIndex = 0; tempArrayIndex < tempArray.Num(); tempArrayIndex++)
	{
		bool isThere = false;
		if (listedGroup.Num() != 0)
		{
			for (int32 listedGroupIndex = 0; listedGroupIndex < listedGroup.Num(); listedGroupIndex++)
			{
				if (listedGroup[listedGroupIndex] == tempArray[tempArrayIndex].groupID)
					isThere = true;
			}
		}
		if (isThere) continue;

		currentGroup = tempArray[tempArrayIndex].groupID;

		UB2UIItemForgeGroupTitle* TitleWidget = CreateWidget<UB2UIItemForgeGroupTitle>(GetOwningPlayer(), GroupTitleBP);
		TitleWidget->Init();
		TitleWidget->SetOwnerItemForgeUI(this, currentGroup);
		SB_AvailableItemList->AddChild(TitleWidget);

		for (int32 tempArraySecondIndex = 0; tempArraySecondIndex < tempArray.Num(); tempArraySecondIndex++)
		{
			if (tempArray[tempArraySecondIndex].groupID == currentGroup)
			{
				UB2UIItemForgeListButton* ButtonWidget = CreateWidget<UB2UIItemForgeListButton>(GetOwningPlayer(), ListButtonBP);
				ButtonWidget->Init();
				ButtonWidget->SetOwnerItemForgeUI(this, tempArray[tempArraySecondIndex].forgeID);
				if (tempArray[tempArraySecondIndex].forgeID == selectedForgeID)
					ButtonWidget->YouHaveBeenChosen(true);
				SB_AvailableItemList->AddChild(ButtonWidget);
			}
		}

		listedGroup.Add(tempArray[tempArrayIndex].groupID);
	}

	quantity = 1;


	if (SB_AvailableItemList->HasAnyChildren() == false)
	{
		TB_ListIsEmpty->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		TB_ListIsEmpty->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIItemForge::SetIngredientList()
{
	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (forgeTable[forgeTableIndex].forgeID == selectedForgeID && forgeTable[forgeTableIndex].forgeTab == selectedForgeTab)
		{
			//insert material item list as blueprint here
			if (selectedForgeType == EItemForgeType::Upgrade)
			{
				UB2UIItemForgeGroupTitle* TitleWidget = CreateWidget<UB2UIItemForgeGroupTitle>(GetOwningPlayer(), IngredientTitleBP);
				TitleWidget->Init();
				TitleWidget->SetOwnerItemForgeUI(this, INGREDIENTTITLE_MAINMATERIAL, true,true,false);
				SB_IngredientList->AddChild(TitleWidget);
			}
			else if (selectedForgeType == EItemForgeType::Forge)
			{
				UB2UIItemForgeGroupTitle* TitleWidget = CreateWidget<UB2UIItemForgeGroupTitle>(GetOwningPlayer(), IngredientTitleBP);
				TitleWidget->Init();
				TitleWidget->SetOwnerItemForgeUI(this, INGREDIENTTITLE_JUSTMATERIAL, true,true,true);
				SB_IngredientList->AddChild(TitleWidget);
			}


			if (forgeTable[forgeTableIndex].mainMaterialItemID != 0)
			{
				needMain = true;
				UB2UIItemForgeIngredientList* IngredientWidget = CreateWidget<UB2UIItemForgeIngredientList>(GetOwningPlayer(), IngredientBP);
				IngredientWidget->Init();
				IngredientWidget->SetOwnerItemForgeUI(this, EItemForgeSlot::MainMaterialItem, forgeTable[forgeTableIndex].mainMaterialItemID, selectedForgeID, selectedClass, quantity, forgeTable[forgeTableIndex].mainMaterialItemQuantity, forgeTable[forgeTableIndex].mainMaterialItemEnhance, forgeTable[forgeTableIndex].mainMaterialItemLevel);
				SB_IngredientList->AddChild(IngredientWidget);
				mainIngredientTrack = IngredientWidget;
			}

			if (selectedForgeType == EItemForgeType::Upgrade)
			{
				UB2UIItemForgeGroupTitle* TitleWidget = CreateWidget<UB2UIItemForgeGroupTitle>(GetOwningPlayer(), IngredientTitleBP);
				TitleWidget->Init();
				TitleWidget->SetOwnerItemForgeUI(this, INGREDIENTTITLE_JUSTMATERIAL, true);
				SB_IngredientList->AddChild(TitleWidget);
			}

			if (forgeTable[forgeTableIndex].material1stItemID != 0)
			{
				needSub1 = true;
				UB2UIItemForgeIngredientList* IngredientWidget = CreateWidget<UB2UIItemForgeIngredientList>(GetOwningPlayer(), IngredientBP);
				IngredientWidget->Init();
				IngredientWidget->SetOwnerItemForgeUI(this, EItemForgeSlot::Sub1stMaterialItem, forgeTable[forgeTableIndex].material1stItemID, selectedForgeID, selectedClass, quantity, forgeTable[forgeTableIndex].material1stItemQuantity);
				SB_IngredientList->AddChild(IngredientWidget);
				subIngredientTrack1 = IngredientWidget;
			}
			if (forgeTable[forgeTableIndex].material2ndItemID != 0)
			{
				needSub2 = true;
				UB2UIItemForgeIngredientList* IngredientWidget = CreateWidget<UB2UIItemForgeIngredientList>(GetOwningPlayer(), IngredientBP);
				IngredientWidget->Init();
				IngredientWidget->SetOwnerItemForgeUI(this, EItemForgeSlot::Sub2ndMaterialItem, forgeTable[forgeTableIndex].material2ndItemID, selectedForgeID, selectedClass, quantity, forgeTable[forgeTableIndex].material2ndItemQuantity);
				SB_IngredientList->AddChild(IngredientWidget);
				subIngredientTrack2 = IngredientWidget;
			}
			if (forgeTable[forgeTableIndex].material3rdItemID != 0)
			{
				needSub3 = true;
				UB2UIItemForgeIngredientList* IngredientWidget = CreateWidget<UB2UIItemForgeIngredientList>(GetOwningPlayer(), IngredientBP);
				IngredientWidget->Init();
				IngredientWidget->SetOwnerItemForgeUI(this, EItemForgeSlot::Sub3rdMaterialItem, forgeTable[forgeTableIndex].material3rdItemID, selectedForgeID, selectedClass, quantity, forgeTable[forgeTableIndex].material3rdItemQuantity);
				SB_IngredientList->AddChild(IngredientWidget);
				subIngredientTrack3 = IngredientWidget;
			}
			if (forgeTable[forgeTableIndex].material4thItemID != 0)
			{
				needSub4 = true;
				UB2UIItemForgeIngredientList* IngredientWidget = CreateWidget<UB2UIItemForgeIngredientList>(GetOwningPlayer(), IngredientBP);
				IngredientWidget->Init();
				IngredientWidget->SetOwnerItemForgeUI(this, EItemForgeSlot::Sub4thMaterialItem, forgeTable[forgeTableIndex].material4thItemID, selectedForgeID, selectedClass, quantity, forgeTable[forgeTableIndex].material4thItemQuantity);
				SB_IngredientList->AddChild(IngredientWidget);
				subIngredientTrack4 = IngredientWidget;
			}
		}
	}
}

void UB2UIItemForge::SetRequiredGoods()
{
	UIP_HeaderResource_Gold->SetAmount(0);
	UIP_HeaderResource_Gem->SetAmount(0);
	UIP_HeaderResource_DarkCrystal->SetAmount(0);

	UIP_HeaderResource_Gold->SetVisibility(ESlateVisibility::Collapsed);
	UIP_HeaderResource_Gem->SetVisibility(ESlateVisibility::Collapsed);
	UIP_HeaderResource_DarkCrystal->SetVisibility(ESlateVisibility::Collapsed);

	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (forgeTable[forgeTableIndex].forgeID == selectedForgeID)
		{
			UIP_HeaderResource_Gold->SetAmount(forgeTable[forgeTableIndex].goldPrice * quantity);
			UIP_HeaderResource_Gem->SetAmount(forgeTable[forgeTableIndex].gemPrice * quantity);
			UIP_HeaderResource_DarkCrystal->SetAmount(forgeTable[forgeTableIndex].darkCrystalPrice *quantity);

			if (forgeTable[forgeTableIndex].goldPrice > 0)
			{
				UIP_HeaderResource_Gold->SetVisibility(ESlateVisibility::Visible);
				UIP_HeaderResource_Gold->SetTextColorByAmount(BladeIIGameImpl::GetClientDataStore().GetGoldAmount(), forgeTable[forgeTableIndex].goldPrice * quantity);
			}
			if (forgeTable[forgeTableIndex].gemPrice > 0)
			{
				UIP_HeaderResource_Gem->SetVisibility(ESlateVisibility::Visible);
				UIP_HeaderResource_Gem->SetTextColorByAmount(BladeIIGameImpl::GetClientDataStore().GetGemAmount(), forgeTable[forgeTableIndex].gemPrice * quantity);
			}
			if (forgeTable[forgeTableIndex].darkCrystalPrice > 0)
			{
				UIP_HeaderResource_DarkCrystal->SetVisibility(ESlateVisibility::Visible);
				UIP_HeaderResource_DarkCrystal->SetTextColorByAmount(BladeIIGameImpl::GetClientDataStore().GetDarkCrystal(), forgeTable[forgeTableIndex].darkCrystalPrice * quantity);
			}
			break;
		}
	}
}

void UB2UIItemForge::SetLimitation()
{
	selectedLimitID = 0;
	int32 limitation = 0;
	for (int32 i = 0; i < forgeTable.Num(); i++)
	{
		if (forgeTable[i].forgeID == selectedForgeID)
		{
			selectedLimitID = forgeTable[i].limitID;
			break;
		}
	}

	FItemForgeLimitInfo MasterLimitTable;
	ClientDataStore->GetItemForgeLimitTable(selectedLimitID, MasterLimitTable);

	limitation = MasterLimitTable.limitQuantity;


	if (maxQuantity > MasterLimitTable.limitQuantity)
	{
		maxQuantity = MasterLimitTable.limitQuantity;
	}


	if (quantity <= maxQuantity)
	{
		forgeAble = true;
	}

	int32 limitType = 0;
	if (TB_ForgeLimit.IsValid())
	{

		switch (MasterLimitTable.limitType)
		{
		case EItemForgeLimitType::NumberOfTry: limitType = 1; break;
		case EItemForgeLimitType::NumberOfSuccess: limitType = 2; break;
		}
	}

	int32 tempLimitCount = 0;
	if (TB_ForgeLimitNumber.IsValid())
	{

		for (int32 i = 0; i < LimitStatus.Num(); i++)
		{
			if (LimitStatus[i].limitID == selectedLimitID)
			{
				tempLimitCount = LimitStatus[i].limitQuantity;
			}
		}
	}

	FText name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("LimitType_Unlimitted")));
	bool itsUnlimitted = false;
	switch (MasterLimitTable.limitTimeType)
	{
	case EItemForgePeriodLimitType::None:	//公力茄
		name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("LimitType_Unlimitted")));
		TB_ForgeLimitNumber->SetText(BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("LimitType_UnlimittedCount"))));
		itsUnlimitted = true;
		break;

	case EItemForgePeriodLimitType::Day:
		if (MasterLimitTable.limitType == EItemForgeLimitType::NumberOfSuccess)
			name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("LimitType_Success_Day")));
		else if (MasterLimitTable.limitType == EItemForgeLimitType::NumberOfTry)
			name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("LimitType_Try_Day")));
		break;

	case EItemForgePeriodLimitType::Week:
		if (MasterLimitTable.limitType == EItemForgeLimitType::NumberOfSuccess)
			name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("LimitType_Success_Week")));
		else if (MasterLimitTable.limitType == EItemForgeLimitType::NumberOfTry)
			name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("LimitType_Try_Week")));
		break;

	case EItemForgePeriodLimitType::Month:
		if (MasterLimitTable.limitType == EItemForgeLimitType::NumberOfSuccess)
			name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("LimitType_Success_Month")));
		else if (MasterLimitTable.limitType == EItemForgeLimitType::NumberOfTry)
			name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("LimitType_Try_Month")));
		break;

	case EItemForgePeriodLimitType::Once:
		if (MasterLimitTable.limitType == EItemForgeLimitType::NumberOfSuccess)
			name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("LimitType_Success_Once")));
		else if (MasterLimitTable.limitType == EItemForgeLimitType::NumberOfTry)
			name = BladeIIGetLOCText(B2LOC_CAT_ITEMFORGELSIT, FString::Printf(TEXT("LimitType_Try_Once")));
		break;
	}

	TB_ForgeLimit->SetText(name);

	if (itsUnlimitted == false)
	{
		currentLimit = tempLimitCount;
		TB_ForgeLimitNumber->SetText(FText::FromString(FString::FromInt(tempLimitCount) + " / " + FString::FromInt(MasterLimitTable.limitQuantity)));
		if (tempLimitCount > 0 || MasterLimitTable.limitTimeType == EItemForgePeriodLimitType::None)
		{
			TB_ForgeLimitNumber->SetColorAndOpacity(FLinearColor::White);
		}
		else
		{
			TB_ForgeLimitNumber->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
		}

		if (maxQuantity > currentLimit)
			maxQuantity = currentLimit;
	}
	else
	{
		TB_ForgeLimitNumber->SetColorAndOpacity(FLinearColor::White);
		maxQuantity = lowestQuantity;
	}
	if (maxQuantity > AVAILABLE_FORGE_QUANTITY)
		maxQuantity = AVAILABLE_FORGE_QUANTITY;

	isUnlimited = itsUnlimitted;

	SetElementSize(maxQuantity);
	if (TB_MaximumQuantity.IsValid())
		TB_MaximumQuantity->SetText(FText::FromString(FString::FromInt(maxQuantity)));

	CheckIsEverythingOkay();
}

void UB2UIItemForge::CheckIsEverythingOkay()
{
	int32 goldPrice = 0;
	int32 gemPrice = 0;
	int32 darkCrystalPrice = 0;

	bool showPreviewOnly = false;

	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (selectedForgeID == forgeTable[forgeTableIndex].forgeID)
		{
			goldPrice = forgeTable[forgeTableIndex].goldPrice;
			gemPrice = forgeTable[forgeTableIndex].gemPrice;
			darkCrystalPrice = forgeTable[forgeTableIndex].darkCrystalPrice;

			showPreviewOnly = forgeTable[forgeTableIndex].previewOnly;
		}
	}

	if (needMain == mainReady
		&& needSub1 == sub1Ready
		&& needSub2 == sub2Ready
		&& needSub3 == sub3Ready
		&& needSub4 == sub4Ready
		&& BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= goldPrice*quantity
		&&BladeIIGameImpl::GetClientDataStore().GetGemAmount() >= gemPrice*quantity
		&&BladeIIGameImpl::GetClientDataStore().GetDarkCrystal() >= darkCrystalPrice*quantity
		)
	{
		if (isUnlimited == false)
		{
			if (currentLimit > 0)
			{
				forgeAble = true;
			}
			else
			{
				forgeAble = false;
			}
		}
		else
		{
			if (showPreviewOnly == true)
			{
				forgeAble = false;
			}
			else
			{
				//everything is ready
				forgeAble = true;
			}
		}
	}
	else
	{
		forgeAble = false;
	}

	if (BTN_Execute.IsValid())
		BTN_Execute->SetIsEnabled(forgeAble);
}

void UB2UIItemForge::ResetForgeSetting()
{
	if (filterInvenType.Num() > 0 || filterStarGrade.Num() > 0)
	{
		ResetForgeSettingForFilter();
		return;
	}
	forgeAble = false;
	forgeTable.Empty();
	ClientDataStore->GetItemForgeTable(selectedForgeType, selectedClass, forgeTable, PeriodLimitStatus);

	selectedForgeID = -1;

	if (HB_ProgressBar.IsValid())
		HB_ProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	if (HB_Resource.IsValid())
		HB_Resource->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (TB_Price.IsValid())
		TB_Price->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (SB_AvailableItemList.IsValid())
		if (SB_AvailableItemList->HasAnyChildren())
			SB_AvailableItemList->ClearChildren();

	if (SB_IngredientList.IsValid())
		if (SB_IngredientList->HasAnyChildren())
			SB_IngredientList->ClearChildren();

	int32 lowestTab = INT_MAX;
	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (forgeTable[forgeTableIndex].forgeTab < lowestTab)
		{
			lowestTab = forgeTable[forgeTableIndex].forgeTab;
		}
	}
	selectedForgeTab = lowestTab;

	selectedMainItem.Empty();
	selected1stMaterial.Empty();
	selected2ndMaterial.Empty();
	selected3rdMaterial.Empty();
	selected4thMaterial.Empty();

	HideQuantityAdjust(false);

	needMain = false;
	needSub1 = false;
	needSub2 = false;
	needSub3 = false;
	needSub4 = false;

	mainReady = false;
	sub1Ready = false;
	sub2Ready = false;
	sub3Ready = false;
	sub4Ready = false;

	mainIngredientTrack = NULL;
	subIngredientTrack1 = NULL;
	subIngredientTrack2 = NULL;
	subIngredientTrack3 = NULL;
	subIngredientTrack4 = NULL;

	mainQuantity = 1;
	sub1Quantity = 1;
	sub2Quantity = 1;
	sub3Quantity = 1;
	sub4Quantity = 1;

	owningMainQuantity = 0;
	owningSub1Quantity = 0;
	owningSub2Quantity = 0;
	owningSub3Quantity = 0;
	owningSub4Quantity = 0;
	lowestQuantity = 1;

	leftQualityMain = -1;
	leftQualitySub1 = -1;
	leftQualitySub2 = -1;
	leftQualitySub3 = -1;
	leftQualitySub4 = -1;

	currentLimit = 0;

	if (CP_Right.IsValid())
	{
		CP_Right->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (O_PleaseSelectForgeStuff.IsValid())
		O_PleaseSelectForgeStuff->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	if(CP_PeriodLimit.IsValid())
		CP_PeriodLimit->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIItemForge::ResetForgeSettingForFilter()
{
	forgeAble = false;
	forgeTable.Empty();
	ClientDataStore->GetItemForgeTable(selectedForgeType, selectedClass, forgeTable, PeriodLimitStatus,filterInvenType,filterStarGrade);

	selectedForgeID = -1;

	if (HB_ProgressBar.IsValid())
		HB_ProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	if (HB_Resource.IsValid())
		HB_Resource->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (TB_Price.IsValid())
		TB_Price->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (SB_AvailableItemList.IsValid())
		if (SB_AvailableItemList->HasAnyChildren())
			SB_AvailableItemList->ClearChildren();

	if (SB_IngredientList.IsValid())
		if (SB_IngredientList->HasAnyChildren())
			SB_IngredientList->ClearChildren();

	int32 lowestTab = INT_MAX;
	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (forgeTable[forgeTableIndex].forgeTab < lowestTab)
		{
			lowestTab = forgeTable[forgeTableIndex].forgeTab;
		}
	}
	selectedForgeTab = lowestTab;

	selectedMainItem.Empty();
	selected1stMaterial.Empty();
	selected2ndMaterial.Empty();
	selected3rdMaterial.Empty();
	selected4thMaterial.Empty();

	HideQuantityAdjust(false);

	needMain = false;
	needSub1 = false;
	needSub2 = false;
	needSub3 = false;
	needSub4 = false;

	mainReady = false;
	sub1Ready = false;
	sub2Ready = false;
	sub3Ready = false;
	sub4Ready = false;

	mainIngredientTrack = NULL;
	subIngredientTrack1 = NULL;
	subIngredientTrack2 = NULL;
	subIngredientTrack3 = NULL;
	subIngredientTrack4 = NULL;

	mainQuantity = 1;
	sub1Quantity = 1;
	sub2Quantity = 1;
	sub3Quantity = 1;
	sub4Quantity = 1;

	owningMainQuantity = 0;
	owningSub1Quantity = 0;
	owningSub2Quantity = 0;
	owningSub3Quantity = 0;
	owningSub4Quantity = 0;
	lowestQuantity = 1;

	leftQualityMain = -1;
	leftQualitySub1 = -1;
	leftQualitySub2 = -1;
	leftQualitySub3 = -1;
	leftQualitySub4 = -1;

	currentLimit = 0;

	if (CP_Right.IsValid())
	{
		CP_Right->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (O_PleaseSelectForgeStuff.IsValid())
		O_PleaseSelectForgeStuff->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (CP_PeriodLimit.IsValid())
		CP_PeriodLimit->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIItemForge::HideQuantityAdjust(bool hideIt)
{
	if (HB_AdjustQuantity.IsValid())
	{
		if (hideIt)
			HB_AdjustQuantity->SetVisibility(ESlateVisibility::Collapsed);
		else
			HB_AdjustQuantity->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2UIItemForge::AdjustQuantity(bool uiOnly)
{
	if (mainIngredientTrack) mainIngredientTrack->RecalculateQuantity(quantity, uiOnly);
	if (subIngredientTrack1) subIngredientTrack1->RecalculateQuantity(quantity, uiOnly);
	if (subIngredientTrack2) subIngredientTrack2->RecalculateQuantity(quantity, uiOnly);
	if (subIngredientTrack3) subIngredientTrack3->RecalculateQuantity(quantity, uiOnly);
	if (subIngredientTrack4) subIngredientTrack4->RecalculateQuantity(quantity, uiOnly);

	for (int32 forgeTableIndex = 0; forgeTableIndex < forgeTable.Num(); forgeTableIndex++)
	{
		if (selectedForgeID == forgeTable[forgeTableIndex].forgeID)
		{
			UIP_ResultItemIcon->UpdateTargetItemData(forgeTable[forgeTableIndex].resultItemID, forgeTable[forgeTableIndex].resultItemQuantity * quantity);
		}
	}

	SetRequiredGoods();
}

void UB2UIItemForge::OnExecuteAnimEnd()
{
	aniEnd = true;

	if (aniEnd == true)
	{
		if (dataRecieved == true)
		{
			ReadytoShowResult();
		}
		else
		{
			OpenLobbyLoginLoadingScreen();
		}
	}
	else
	{
		OpenLobbyLoginLoadingScreen();
	}
	
}

void UB2UIItemForge::ReadytoShowResult()
{
	CloseLobbyLoginLoadingScreen();

	if (BTN_MasterModal.IsValid())
		BTN_MasterModal->SetVisibility(ESlateVisibility::Collapsed);
	if (X_BTN_Skip.IsValid())
		X_BTN_Skip->SetVisibility(ESlateVisibility::Visible);


	resultIndex = 0;	//搬苞 硅凯阑 促弗 努贰胶俊辑档 楷搬矫难辑 荤侩窍扁 困秦 瘤沥茄 函荐. 瘤陛 咯扁 窃荐绰 局聪皋捞记捞 场唱绊 茄锅父 备悼窍扁 嫐巩俊 0栏肺 檬扁拳甫 茄促. 弊 捞饶何磐绰 喉风橇赴飘甫 烹秦 促弗 窃荐俊辑 龋免凳
	refundIndex = 0;	//搬苞 硅凯 救俊 券阂 硅凯捞 乐绢辑 秦寸 馆汗巩 眠利阑 困茄 函荐. 困客 悼老窍霸 累悼茄促.

	X_BTN_Confirm->SetVisibility(ESlateVisibility::Collapsed);

	if (SB_ResultScroll.IsValid())
		SB_ResultScroll->ClearChildren();

	if (CP_Result.IsValid())
		CP_Result->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	totalFail = 0;
	totalSuccess = 0;
	totalHugeSuccess = 0;

	switch (resultItems[resultIndex].resultType)
	{
	case EItemForgeResultType::Fail: totalFail++; break;
	case EItemForgeResultType::Success: totalSuccess++; break;
	case EItemForgeResultType::HugeSuccess: totalHugeSuccess++; break;
	}

	UB2DynItemIcon_ItemForgeResult* resultIconWidget = CreateWidget<UB2DynItemIcon_ItemForgeResult>(GetOwningPlayer(), ForgeResultBP);
	resultIconWidget->SetOwnerItemForgeUI(this, resultItems[resultIndex].resultType, resultItems[resultIndex].forgedItem, resultSkip, resultItems[resultIndex].forgedItem.ConsumingAmount > 0 ? resultItems[resultIndex].forgedItem.ConsumingAmount : 1);
	SB_ResultScroll->AddChild(resultIconWidget);
	SB_ResultScroll->ScrollWidgetIntoView(resultIconWidget);

	if (X_TB_Fail.IsValid())
		X_TB_Fail->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_ResultFail"))), FText::AsNumber(totalFail)));
	if (X_TB_Success.IsValid())
		X_TB_Success->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_ResultSuccess"))), FText::AsNumber(totalSuccess)));
	if (X_TB_HugeSuccess.IsValid())
		X_TB_HugeSuccess->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_ResultHugeSuccess"))), FText::AsNumber(totalHugeSuccess)));
}

void UB2UIItemForge::ContinueShowingResult(EItemForgeResultType previousResult)
{
	if (resultIndex >= resultItems.Num() - 1)
	{
		if (previousResult == EItemForgeResultType::Fail)
		{
			if (resultItems[resultIndex].refundedItem.Num() == 0)
			{
				if (X_BTN_Confirm.IsValid())
					X_BTN_Confirm->SetVisibility(ESlateVisibility::Visible);
				if (X_BTN_Skip.IsValid())
					X_BTN_Skip->SetVisibility(ESlateVisibility::Collapsed);
				return;
			}
		}
		else if (previousResult == EItemForgeResultType::FailRefund)
		{
			if (resultItems[resultIndex].refundedItem.Num() <= refundIndex)
			{
				if (X_BTN_Confirm.IsValid())
					X_BTN_Confirm->SetVisibility(ESlateVisibility::Visible);
				if (X_BTN_Skip.IsValid())
					X_BTN_Skip->SetVisibility(ESlateVisibility::Collapsed);
				return;
			}
		}
		else if (previousResult == EItemForgeResultType::HugeSuccess)
		{

		}
		else if (previousResult == EItemForgeResultType::Success || previousResult == EItemForgeResultType::HugeSuccessReward)
		{
			if (X_BTN_Confirm.IsValid())
				X_BTN_Confirm->SetVisibility(ESlateVisibility::Visible);
			if (X_BTN_Skip.IsValid())
				X_BTN_Skip->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
	}

	if (resultSkip == false)
	{

		resultIndex++;
		EItemForgeResultType currentType = EItemForgeResultType::FailRefund;

		UB2DynItemIcon_ItemForgeResult* resultIconWidget = CreateWidget<UB2DynItemIcon_ItemForgeResult>(GetOwningPlayer(), ForgeResultBP);

		if (previousResult == EItemForgeResultType::Success || previousResult == EItemForgeResultType::HugeSuccessReward)
		{
			resultIconWidget->SetOwnerItemForgeUI(this, resultItems[resultIndex].resultType, resultItems[resultIndex].forgedItem, resultSkip, resultItems[resultIndex].forgedItem.ConsumingAmount > 0 ? resultItems[resultIndex].forgedItem.ConsumingAmount : 1);
			currentType = resultItems[resultIndex].resultType;
		}
		else if (previousResult == EItemForgeResultType::HugeSuccess)
		{
			resultIndex--;
			resultIconWidget->SetOwnerItemForgeUI(this, EItemForgeResultType::HugeSuccessReward, resultItems[resultIndex].bonusItem, resultSkip, resultItems[resultIndex].bonusItem.ConsumingAmount > 0 ? resultItems[resultIndex].bonusItem.ConsumingAmount : 1);
			currentType = EItemForgeResultType::HugeSuccessReward;
		}
		else if (previousResult == EItemForgeResultType::Fail)
		{
			if (resultItems[resultIndex - 1].refundedItem.Num() > 0)
			{
				resultIndex--;
				resultIconWidget->SetOwnerItemForgeUI(this, EItemForgeResultType::FailRefund, resultItems[resultIndex].refundedItem[refundIndex], resultSkip, resultItems[resultIndex].refundedItem[refundIndex].ConsumingAmount > 0 ? resultItems[resultIndex].refundedItem[refundIndex].ConsumingAmount : 1);
				currentType = EItemForgeResultType::FailRefund;
				refundIndex++;
			}
			else
			{
				resultIconWidget->SetOwnerItemForgeUI(this, resultItems[resultIndex].resultType, resultItems[resultIndex].forgedItem, resultSkip, resultItems[resultIndex].forgedItem.ConsumingAmount > 0 ? resultItems[resultIndex].forgedItem.ConsumingAmount : 1);
				currentType = resultItems[resultIndex].resultType;
			}
		}
		else if (previousResult == EItemForgeResultType::FailRefund)
		{
			if (resultItems[resultIndex - 1].refundedItem.Num() == refundIndex)
			{
				resultIconWidget->SetOwnerItemForgeUI(this, resultItems[resultIndex].resultType, resultItems[resultIndex].forgedItem, resultSkip, resultItems[resultIndex].forgedItem.ConsumingAmount > 0 ? resultItems[resultIndex].forgedItem.ConsumingAmount : 1);
				currentType = resultItems[resultIndex].resultType;
				refundIndex = 0;
			}
			else
			{
				resultIndex--;
				resultIconWidget->SetOwnerItemForgeUI(this, EItemForgeResultType::FailRefund, resultItems[resultIndex].refundedItem[refundIndex], resultSkip, resultItems[resultIndex].refundedItem[refundIndex].ConsumingAmount > 0 ? resultItems[resultIndex].refundedItem[refundIndex].ConsumingAmount : 1);
				currentType = EItemForgeResultType::FailRefund;
				refundIndex++;
			}
		}
		SB_ResultScroll->AddChild(resultIconWidget);
		SB_ResultScroll->ScrollWidgetIntoView(resultIconWidget);

		switch (currentType)
		{
		case EItemForgeResultType::Fail: totalFail++; break;
		case EItemForgeResultType::Success: totalSuccess++; break;
		case EItemForgeResultType::HugeSuccess: totalHugeSuccess++; break;
		}

		if (X_TB_Fail.IsValid())
			X_TB_Fail->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_ResultFail"))), FText::AsNumber(totalFail)));
		if (X_TB_Success.IsValid())
			X_TB_Success->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_ResultSuccess"))), FText::AsNumber(totalSuccess)));
		if (X_TB_HugeSuccess.IsValid())
			X_TB_HugeSuccess->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_ResultHugeSuccess"))), FText::AsNumber(totalHugeSuccess)));
	}
	else
	{
		SB_ResultScroll->ClearChildren();
		totalFail = 0;
		totalSuccess = 0;
		totalHugeSuccess = 0;
		for (int32 i = 0; i < resultItems.Num(); i++)
		{
			if (resultItems[i].resultType == EItemForgeResultType::Success)
			{
				UB2DynItemIcon_ItemForgeResult* resultIconWidget = CreateWidget<UB2DynItemIcon_ItemForgeResult>(GetOwningPlayer(), ForgeResultBP);
				resultIconWidget->SetOwnerItemForgeUI(this, resultItems[i].resultType, resultItems[i].forgedItem, resultSkip, resultItems[i].forgedItem.ConsumingAmount > 0 ? resultItems[i].forgedItem.ConsumingAmount : 1);
				SB_ResultScroll->AddChild(resultIconWidget);
				SB_ResultScroll->ScrollWidgetIntoView(resultIconWidget);

				totalSuccess++;
			}
			else if (resultItems[i].resultType == EItemForgeResultType::HugeSuccess)
			{
				UB2DynItemIcon_ItemForgeResult* resultIconWidget = CreateWidget<UB2DynItemIcon_ItemForgeResult>(GetOwningPlayer(), ForgeResultBP);
				resultIconWidget->SetOwnerItemForgeUI(this, resultItems[i].resultType, resultItems[i].forgedItem, resultSkip, resultItems[i].forgedItem.ConsumingAmount > 0 ? resultItems[i].forgedItem.ConsumingAmount : 1);
				SB_ResultScroll->AddChild(resultIconWidget);
				SB_ResultScroll->ScrollWidgetIntoView(resultIconWidget);

				totalHugeSuccess++;
				
				UB2DynItemIcon_ItemForgeResult* resultBonusIconWidget = CreateWidget<UB2DynItemIcon_ItemForgeResult>(GetOwningPlayer(), ForgeResultBP);
				resultBonusIconWidget->SetOwnerItemForgeUI(this, EItemForgeResultType::HugeSuccessReward, resultItems[i].bonusItem, resultSkip, resultItems[i].bonusItem.ConsumingAmount > 0 ? resultItems[i].bonusItem.ConsumingAmount : 1);
				SB_ResultScroll->AddChild(resultBonusIconWidget);
				SB_ResultScroll->ScrollWidgetIntoView(resultBonusIconWidget);
			}
			else if (resultItems[i].resultType == EItemForgeResultType::Fail)
			{
				UB2DynItemIcon_ItemForgeResult* resultIconWidget = CreateWidget<UB2DynItemIcon_ItemForgeResult>(GetOwningPlayer(), ForgeResultBP);
				resultIconWidget->SetOwnerItemForgeUI(this, resultItems[i].resultType, resultItems[i].forgedItem, resultSkip, resultItems[i].forgedItem.ConsumingAmount > 0 ? resultItems[i].forgedItem.ConsumingAmount : 1);
				SB_ResultScroll->AddChild(resultIconWidget);
				SB_ResultScroll->ScrollWidgetIntoView(resultIconWidget);

				totalFail++;

				for (int32 j = 0; j < resultItems[i].refundedItem.Num(); j++)
				{
					UB2DynItemIcon_ItemForgeResult* resultRefundIconWidget = CreateWidget<UB2DynItemIcon_ItemForgeResult>(GetOwningPlayer(), ForgeResultBP);
					resultRefundIconWidget->SetOwnerItemForgeUI(this, EItemForgeResultType::FailRefund, resultItems[i].refundedItem[j], resultSkip, resultItems[i].refundedItem[j].ConsumingAmount > 0 ? resultItems[i].refundedItem[j].ConsumingAmount : 1);
					SB_ResultScroll->AddChild(resultRefundIconWidget);
					SB_ResultScroll->ScrollWidgetIntoView(resultRefundIconWidget);
				}
			}
		}

		if (X_TB_Fail.IsValid())
			X_TB_Fail->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_ResultFail"))), FText::AsNumber(totalFail)));
		if (X_TB_Success.IsValid())
			X_TB_Success->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_ResultSuccess"))), FText::AsNumber(totalSuccess)));
		if (X_TB_HugeSuccess.IsValid())
			X_TB_HugeSuccess->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_ResultHugeSuccess"))), FText::AsNumber(totalHugeSuccess)));

		if (X_BTN_Confirm.IsValid())
			X_BTN_Confirm->SetVisibility(ESlateVisibility::Visible);
		if (X_BTN_Skip.IsValid())
			X_BTN_Skip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIItemForge::ResultEnd()
{
	
}

void UB2UIItemForge::OutofTimeForLimited()
{
	selectedForgeID = -1;

	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("ItemFactory_OutofTimeForLimitedItem")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
		);
}