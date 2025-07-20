#include "B2UISelectiveLotteryPopUp.h"
#include "B2UISlotItem.h"
#include "B2UICollectBookItemDetail.h"
#include "B2UICollectBookIcon.h"
#include "B2UISelectiveLotteryCostumeDT.h"
#include "B2ItemInfo.h"
#include "B2AndroidBackManager.h"
#include "CommonStruct.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UISelectiveLotteryItemRow::CacheAssets()
{
	GET_SLOT(UB2UISlotItem, UIP_ItemElem01);
	if (UIP_ItemElem01.IsValid())
	{
		UIP_ItemElem01->Init();
		UIP_ItemElem01->SetPreciseClickAndTab(true);
		UIP_ItemElem01->SetVisibility(ESlateVisibility::Hidden);
	}
		
	GET_SLOT(UB2UISlotItem, UIP_ItemElem02);
	if (UIP_ItemElem02.IsValid())
	{
		UIP_ItemElem02->Init();
		UIP_ItemElem02->SetPreciseClickAndTab(true);
		UIP_ItemElem02->SetVisibility(ESlateVisibility::Hidden);
	}

	GET_SLOT(UB2UISlotItem, UIP_ItemElem03);
	if (UIP_ItemElem03.IsValid())
	{
		UIP_ItemElem03->Init();
		UIP_ItemElem03->SetPreciseClickAndTab(true);
		UIP_ItemElem03->SetVisibility(ESlateVisibility::Hidden);
	}

	GET_SLOT(UB2UISlotItem, UIP_ItemElem04);
	if (UIP_ItemElem04.IsValid())
	{
		UIP_ItemElem04->Init();
		UIP_ItemElem04->SetPreciseClickAndTab(true);
		UIP_ItemElem04->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UISelectiveLotteryItemRow::BindDelegates()
{
	if (UIP_ItemElem01.IsValid())
		UIP_ItemElem01->OnSlotItemClickDelgate.BindDynamic(this, &UB2UISelectiveLotteryItemRow::SelectItem);
	if (UIP_ItemElem02.IsValid())
		UIP_ItemElem02->OnSlotItemClickDelgate.BindDynamic(this, &UB2UISelectiveLotteryItemRow::SelectItem);
	if (UIP_ItemElem03.IsValid())
		UIP_ItemElem03->OnSlotItemClickDelgate.BindDynamic(this, &UB2UISelectiveLotteryItemRow::SelectItem);
	if (UIP_ItemElem04.IsValid())
		UIP_ItemElem04->OnSlotItemClickDelgate.BindDynamic(this, &UB2UISelectiveLotteryItemRow::SelectItem);
}

void UB2UISelectiveLotteryItemRow::SelectItem(UB2UISlotItem* ClickedItem)
{
	if (LotteryPopup && ClickedItem)
		LotteryPopup->SetSelectedItemRefId(ClickedItem->GetItemRefId());
}

UB2UISlotItem* UB2UISelectiveLotteryItemRow::SetItemRefId(int32 index, int32 RefId)
{
	TWeakObjectPtr<UB2UISlotItem> ItemElem = 
		index == 0 ? UIP_ItemElem01 :
		index == 1 ? UIP_ItemElem02 :
		index == 2 ? UIP_ItemElem03 :
		index == 3 ? UIP_ItemElem04 : nullptr;

	if (ItemElem.IsValid())
	{
		ItemElem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemElem->BindDoc(FB2Item(BladeIIGameImpl::GetClientDataStore().GetItemMasterDetailInfo(RefId)));

		return ItemElem.Get();
	}

	return nullptr;
}

void UB2UISelectiveLotteryItemRow::SetEnableReadingGlasses(bool Enable)
{
	if (UIP_ItemElem01.IsValid())
	{
		UIP_ItemElem01->SetEnableReadingGlasses(Enable);
	}
	if (UIP_ItemElem02.IsValid())
	{
		UIP_ItemElem02->SetEnableReadingGlasses(Enable);
	}
	if (UIP_ItemElem03.IsValid())
	{
		UIP_ItemElem03->SetEnableReadingGlasses(Enable);
	}
	if (UIP_ItemElem04.IsValid())
	{
		UIP_ItemElem04->SetEnableReadingGlasses(Enable);
	}
}

#include "B2UICollectBookIcon.h"
void UB2UISelectiveLotteryItemDetail::SetItemIconDefaultTemplate(int32 ItemGrade, EItemCollectionState ItemCollectionState)
{
	if (UIP_CollectBookIcon.IsValid())
	{
		UIP_CollectBookIcon->InitStarGrade(ItemGrade);
		UIP_CollectBookIcon->OnInitCollectBookIcon(ItemGrade);

		UIP_CollectBookIcon->SetIconCollectionState(ItemCollectionState);
	}
}

void UB2UISelectiveLotteryPopup::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_SelectFullText);
	GET_SLOT(UVerticalBox, VB_ItemList);
	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UB2Button, BTN_ItemDetailBackGround);

	if (BTN_Select.IsValid())
	{
		BTN_Select->SetIsEnabled(false);
	}
	GET_SLOT(UTextBlock, TB_Select);
	GET_SLOT(UB2Button, BTN_Close);

	GET_SLOT(UB2UISlotItem, UIP_SelectedItem);
	if (UIP_SelectedItem.IsValid())
	{
		UIP_SelectedItem->Init();
		UIP_SelectedItem->SetVisibility(ESlateVisibility::Hidden);
	}

	GET_SLOT(UB2UISelectiveLotteryItemDetail, UIP_SelectedItemDetail);
	if (UIP_SelectedItemDetail.IsValid())
	{
		UIP_SelectedItemDetail->Init();
	}

	GET_SLOT(UB2UICollectBookItemDetail, UIP_ItemDetailView);
	if (UIP_ItemDetailView.IsValid())
	{
		UIP_ItemDetailView->Init();
		FOnClickDetailClose DetailCloseDelegate;
		DetailCloseDelegate.BindUObject(this, &UB2UISelectiveLotteryPopup::HideDetailView);
		UIP_ItemDetailView->SetDetailViewCloseDelegate(DetailCloseDelegate);
	}
}

void UB2UISelectiveLotteryPopup::HideDetailView()
{
	if (UIP_ItemDetailView.IsValid())
	{
		UIP_ItemDetailView->SetVisibility(ESlateVisibility::Hidden);
	}

	if (BTN_ItemDetailBackGround.IsValid())
	{
		BTN_ItemDetailBackGround->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UISelectiveLotteryPopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UISelectiveLotteryPopup::CloseMe);
	if (UIP_SelectedItem.IsValid())
	{
		UIP_SelectedItem->OnSlotItemClickDelgate.BindDynamic(this, &UB2UISelectiveLotteryPopup::ShowSelectedItemDetail);		
	}
	if (UIP_SelectedItemDetail.IsValid())
	{
		UIP_SelectedItemDetail->SetDetailViewCloseDelegate(FOnClickDetailClose::CreateLambda([this]() {this->CloseSelectedItemDetail(); }));
	}
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UISelectiveLotteryPopup::OnClickSelectedConfirm);
}

void UB2UISelectiveLotteryPopup::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_SelectFullText.IsValid())
	{
		TB_SelectFullText->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("Lottery_SelectItemText")));
	}
	if (TB_Select.IsValid())
	{
		TB_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_MAILCONTENT, TEXT("Lottery_SelectItemConfirm")));
	}
}

void UB2UISelectiveLotteryPopup::CloseWidgetDelegate()
{
	CloseMe();
}

void UB2UISelectiveLotteryPopup::CloseMe()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	SetVisibility(ESlateVisibility::Collapsed);

	if (UIP_SelectedItem.IsValid())
	{
		UIP_SelectedItem->SetVisibility(ESlateVisibility::Hidden);
		UIP_SelectedItem->UnbindDoc();
	}

	if (BTN_Select.IsValid())
	{
		BTN_Select->SetIsEnabled(false);
	}
}


void UB2UISelectiveLotteryPopup::SetTitleText(const FText& Title)
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(Title);
	}
}

#include "B2UIMail.h"
void UB2UISelectiveLotteryPopup::RequestItemSelected()
{
	if (UIP_SelectedItem.IsValid())
	{
		int32 RequestSelectedItemRefId = UIP_SelectedItem->GetItemRefId();
		if (RequestSelectedItemRefId > 0 && MailBox)
		{
			MailBox->SelectedItemSelectiveLottery(RequestSelectedItemRefId);
		}
	}
}

void UB2UISelectiveLotteryPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	if (UIP_SelectedItem.IsValid())
	{
		UIP_SelectedItem->DestroySelf(InUIManager);
	}
	if (UIP_SelectedItemDetail.IsValid())
	{
		UIP_SelectedItemDetail->DestroySelf(InUIManager);
	}
	if (UIP_ItemDetailView.IsValid())
	{
		UIP_ItemDetailView->DestroySelf(InUIManager);
	}

	for (auto& ItemSlot : ItemUILists)
	{
		if (ItemSlot.IsValid())
		{
			ItemSlot->DestroySelf(InUIManager);
		}
	}

	Super::DestroySelf(InUIManager);
}

void UB2UISelectiveLotteryPopup::AddBackWidget()
{
	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
}

#define ITEM_ROW_MAX_ITEM_COUNT 4
void UB2UISelectiveLotteryPopup::SetItemRefIds(const TArray<int32>& ItemRefIds)
{
	if (VB_ItemList.IsValid())
	{
		VB_ItemList->ClearChildren();
		ItemUILists.Empty();

		UB2UISelectiveLotteryItemRow* ItemRowUI = nullptr;
		for (int32 ItemIndex = 0; ItemIndex < ItemRefIds.Num(); ++ItemIndex)
		{
			if (ItemIndex % ITEM_ROW_MAX_ITEM_COUNT == 0)
			{
				ItemRowUI = CreateWidget<UB2UISelectiveLotteryItemRow>(GetWorld(), ItemListRowUIClass);
				if (ItemRowUI)
				{
					EPCClass ItemClassId = static_cast<EPCClass>(ExtractNthDigitR<6>(ItemRefIds[ItemIndex]));

					VB_ItemList->AddChildToVerticalBox(ItemRowUI);
					ItemRowUI->SetSelectiveLotteryPopup(this);
					ItemRowUI->Init();
					ItemRowUI->SetEnableReadingGlasses(false);
					ItemRowUI->SetItemCharacterClass(ItemClassId);
				}
			}

			if (ItemRowUI)
			{
				ItemUILists.Add(ItemRowUI->SetItemRefId(ItemIndex % ITEM_ROW_MAX_ITEM_COUNT, ItemRefIds[ItemIndex]));
			}
		}		
	}
}

void UB2UISelectiveLotteryPopup::SetItemRefIds_AutoSort(const TArray<int32>& ItemRefIds)
{
	if (VB_ItemList.IsValid())
	{
		VB_ItemList->ClearChildren();
		ItemUILists.Empty();

		UB2UISelectiveLotteryItemRow* ItemRowUI = nullptr;

		EPCClass PrevClass = EPCClass::EPC_End;
		int32 ColumnIndex = 0;
		for (int32 ItemIndex = 0; ItemIndex < ItemRefIds.Num(); ++ItemIndex)
		{
			EPCClass TargetClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(ItemRefIds[ItemIndex]);

			if (PrevClass != TargetClass)
			{
				ColumnIndex = 0;
				PrevClass = TargetClass;
				ItemRowUI = CreateWidget<UB2UISelectiveLotteryItemRow>(GetWorld(), ItemListRowUIClass);
				if (ItemRowUI)
				{
					VB_ItemList->AddChildToVerticalBox(ItemRowUI);
					ItemRowUI->SetSelectiveLotteryPopup(this);
					ItemRowUI->Init();
					ItemRowUI->SetEnableReadingGlasses(false);
					ItemRowUI->SetItemCharacterClass(TargetClass);
				}
			}

			if (ItemRowUI)
			{
				ItemUILists.Add(ItemRowUI->SetItemRefId(ColumnIndex, ItemRefIds[ItemIndex]));
				ColumnIndex += 1;
			}
		}
	}
}

void UB2UISelectiveLotteryPopup::SetSelectedItemRefId(int32 ToSet)
{
	SelectedId = ToSet;

	if (ToSet > 0)
	{
		if (UIP_SelectedItem.IsValid())
		{
			UIP_SelectedItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UIP_SelectedItem->BindDoc(FB2Item(BladeIIGameImpl::GetClientDataStore().GetItemMasterDetailInfo(ToSet)));

			UB2UIAllSelectiveLotteryPopup* AllSelectiveLotteryPopup = dynamic_cast<UB2UIAllSelectiveLotteryPopup*>(this);
			if (AllSelectiveLotteryPopup)
			{
				if(AllSelectiveLotteryPopup->GetLotterySurpassLevel() > 0)
					UIP_SelectedItem->SetStarGrade(FB2Item(BladeIIGameImpl::GetClientDataStore().GetItemMasterDetailInfo(ToSet)).StarGrade, true);
			}
		}

		if (BTN_Select.IsValid())
		{
			BTN_Select->SetIsEnabled(true);
		}
	}
	else
	{
		if (UIP_SelectedItem.IsValid())
		{
			UIP_SelectedItem->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BTN_Select.IsValid())
		{
			BTN_Select->SetIsEnabled(false);
		}
	}

	for (auto& ItemUI : ItemUILists)
	{
		if (ItemUI.IsValid())
		{
			ItemUI->SetSelected(ItemUI->GetItemRefId() == ToSet);			
		}
	}
}

#include "B2UICollectBookMain.h"
void UB2UISelectiveLotteryPopup::ShowSelectedItemDetail(class UB2UISlotItem* SelectedItem)
{
	if (UIP_ItemDetailView.IsValid() && SelectedItem)
	{
		UIP_ItemDetailView->OpenPopup();
		UB2UIAllSelectiveLotteryPopup* AllSelectiveLotteryPopup = dynamic_cast<UB2UIAllSelectiveLotteryPopup*>(this);
		if (AllSelectiveLotteryPopup)
		{
			UIP_ItemDetailView->SetDetailViewSingleItem(SelectedItem->GetItemRefId(),
				FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(SelectedItem->GetItemRefId()),
				AllSelectiveLotteryPopup->GetLotterySurpassLevel());
		}
		else
		{
			UIP_ItemDetailView->SetDetailViewSingleItem(SelectedItem->GetItemRefId(), FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(SelectedItem->GetItemRefId()));
		}
		
		UIP_ItemDetailView->SetCollectBookIcon(SelectedItem->GetItemClass(), SelectedItem->GetItemRefId());
		UIP_ItemDetailView->SetSelectiveLottery();
		if(auto* BGMtrl = StaticFindItemMiscInfo()->GetItemIconBGMtrl(SelectedItem->GetItemStarGrade(), true, SelectedItem->IsConsumables()))
		{
			UIP_ItemDetailView->SetItemIconDefaultTemplate(SelectedItem->GetItemStarGrade(), BGMtrl, EItemCollectionState::Collected);

			if (AllSelectiveLotteryPopup)
			{
				UIP_ItemDetailView->SetItemIconStarGrade(SelectedItem->GetItemStarGrade(), AllSelectiveLotteryPopup->GetLotterySurpassLevel() > 0 ? true : false);
			}
		}
			
		UIP_ItemDetailView->SetEnableBackGround(false);

		if (BTN_ItemDetailBackGround.IsValid())
		{
			BTN_ItemDetailBackGround->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UB2UISelectiveLotteryPopup::CloseSelectedItemDetail()
{
	if (UIP_SelectedItemDetail.IsValid())
	{
		UIP_SelectedItemDetail->SetVisibility(ESlateVisibility::Collapsed);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_NUM_SELECTIBLE_ITEM 4

#include "B2ItemInfo.h"
void UB2UISelectiveLotteryItemSlot::SetItemData(int32 InItemRefId, int32 Num)
{
	if (Num == 0)
		return;
	ItemRefId = InItemRefId;

	SetItemIcon(ItemRefId);

	if (TB_ItemAmount.IsValid())
	{
		TB_ItemAmount->SetText(FText::AsNumber(Num));
	}
}

void UB2UISelectiveLotteryItemSlot::SetItemData(int32 InItemRefId)
{
	ItemRefId = InItemRefId;

	SetItemIcon(ItemRefId);

	if (TB_ItemAmount.IsValid())
	{
		TB_ItemAmount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UISelectiveLotteryItemSlot::SetSelected(bool bToSet)
{
	bSelected = bToSet;

	if (OV_Selected.IsValid())
		OV_Selected->SetVisibility(bSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UISelectiveLotteryItemSlot::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UImage, IMG_Icon);
	GET_SLOT(UTextBlock, TB_ItemName);
	GET_SLOT(UTextBlock, TB_ItemAmount);
	GET_SLOT(UOverlay, OV_Selected);
}

void UB2UISelectiveLotteryItemSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UISelectiveLotteryItemSlot::OnClickBTNArea);
}

void UB2UISelectiveLotteryItemSlot::SetSelectDelegate(const FOnSelectItemSlot& ToSetDelegate)
{
	SelectDelegate = ToSetDelegate;
}

void UB2UISelectiveLotteryItemSlot::OnClickBTNArea()
{
	SelectDelegate.ExecuteIfBound(ItemRefId);
}

void UB2UISelectiveLotteryItemSlot::SetItemIcon(int32 InItemRefId)
{
	UMaterialInstance* ItemIconMat = nullptr;
	FText ItemName;
	UB2ItemInfo::GetItemIconAndName(InItemRefId, ItemIconMat, ItemName);

	if (IMG_Icon.IsValid())
	{
		IMG_Icon->SetBrushFromMaterial(ItemIconMat);
	}

	if (TB_ItemName.IsValid())
	{
		TB_ItemName->SetText(ItemName);
	}
}

void UB2UISelectiveLotteryConsumItemPopup::SetItemData(const TArray<int32>& ItemRefIds, const TArray<int32>& ItemNums)
{
	check(ItemRefIds.Num() <= UIP_ItemSlots.Num());
	check(ItemRefIds.Num() == ItemNums.Num());

	for (int32 Index = 0; Index < UIP_ItemSlots.Num(); ++Index)
	{
		if (UIP_ItemSlots[Index].IsValid())
		{
			UIP_ItemSlots[Index]->SetVisibility(ItemRefIds.IsValidIndex(Index) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
			UIP_ItemSlots[Index]->SetItemData(ItemRefIds.IsValidIndex(Index) ? ItemRefIds[Index] : 0, ItemNums.IsValidIndex(Index) ? ItemNums[Index] : 0);
		}		
	}

	bool bCheck = true;			// 窍靛 内爹 绊鞭, 老馆 铰鞭籍 抗寇贸府

	for (int32 Index = 0; Index < ItemRefIds.Num(); Index++)
	{
		if (ItemRefIds[Index] == FItemRefIDHelper::GetRareWeaponUpgradeStoneID() ||		// 绊鞭 公扁 铰鞭籍
			ItemRefIds[Index] == FItemRefIDHelper::GetRareArmorUpgradeStoneID() ||		// 绊鞭 规绢备 铰鞭籍
			ItemRefIds[Index] == FItemRefIDHelper::GetRareAccessoryUpgradeStoneID())		// 绊鞭 厘脚备 铰鞭籍
		{
			continue;
		}
		else
		{
			bCheck = false;
			break;
		}
	}

	if (bCheck)
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Lottery_SelectItemRareUpgradeStoneTitleText")));
	}
	else
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Lottery_SelectItemUpgradeStoneTitleText")));
	}

}

void UB2UISelectiveLotteryConsumItemPopup::SetSelectedItemRefId(int32 ToSet)
{
	bool bSelected = false;

	for (auto& ItemSlot : UIP_ItemSlots)
	{
		if (ItemSlot.IsValid())
		{
			bool bSelectedItem = ItemSlot->GetItemRefId() == ToSet;
			bSelected |= bSelectedItem;
			ItemSlot->SetSelected(bSelectedItem);
		}
	}

	if (BTN_SelectConfirm.IsValid())
		BTN_SelectConfirm->SetIsEnabled(bSelected);
}

void UB2UISelectiveLotteryConsumItemPopup::SetTitleText(const FText& Title)
{

}

void UB2UISelectiveLotteryConsumItemPopup::CacheAssets()
{
	UIP_ItemSlots.Empty(MAX_NUM_SELECTIBLE_ITEM);
	UIP_ItemSlots.AddDefaulted(MAX_NUM_SELECTIBLE_ITEM);

	for (int32 Index = 0; Index < MAX_NUM_SELECTIBLE_ITEM; ++Index)
	{
		FString VarName = FString::Printf(TEXT("UIP_ItemSlot%02d"), Index + 1);
		
		UIP_ItemSlots[Index] = GetSlot<UB2UISelectiveLotteryItemSlot>(FName(*VarName));
		if(UIP_ItemSlots[Index].IsValid())
			UIP_ItemSlots[Index]->Init();
	}
	
	GET_SLOT(UTextBlock, TB_Title);
	
	GET_SLOT(UVerticalBox, VB_ItemList);
	GET_SLOT(UB2Button, BTN_SelectConfirm);
	if (BTN_SelectConfirm.IsValid())
		BTN_SelectConfirm->SetIsEnabled(false);

	GET_SLOT(UB2Button, BTN_Close);
}

void UB2UISelectiveLotteryConsumItemPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UISelectiveLotteryConsumItemPopup::CloseMe);
	BIND_CLICK_FUNC_TO_BTN(BTN_SelectConfirm, &UB2UISelectiveLotteryConsumItemPopup::RequestItemSelected);

	for (auto& ItemSlot : UIP_ItemSlots)
	{
		if (ItemSlot.IsValid())
		{
			ItemSlot->SetSelectDelegate(FOnSelectItemSlot::CreateLambda([this](int32 SelectedItemRefId) {this->SetSelectedItemRefId(SelectedItemRefId); }));
		}
	}
}

void UB2UISelectiveLotteryConsumItemPopup::CloseMe()
{
	for (auto& ItemSlot : UIP_ItemSlots)
	{
		if (ItemSlot.IsValid())
		{
			ItemSlot->SetSelected(false);
			ItemSlot->SetVisibility(ESlateVisibility::Collapsed);
		}			
	}

	if (BTN_SelectConfirm.IsValid())
	{
		BTN_SelectConfirm->SetIsEnabled(false);
	}

	SetVisibility(ESlateVisibility::Collapsed);	
}

void UB2UISelectiveLotteryConsumItemPopup::RequestItemSelected()
{
	if (MailBox)
	{
		for (auto& ItemSlot : UIP_ItemSlots)
		{
			if (ItemSlot.IsValid() && ItemSlot->IsSelected())
			{
				MailBox->SelectedItemSelectiveLottery(ItemSlot->GetItemRefId());
				break;
			}
		}		
	}
}

void UB2UIAllSelectiveLotteryPopupBase::OnClickedBTNGladiator()
{
	if (CurrentCharacterTab != EPCClass::EPC_Gladiator)
	{
		CurrentCharacterTab = EPCClass::EPC_Gladiator;
		SetButtonClick(EPCClass::EPC_Gladiator);
	}
}

void UB2UIAllSelectiveLotteryPopupBase::OnClickedBTNAssassin()
{
	if (CurrentCharacterTab != EPCClass::EPC_Assassin)
	{
		CurrentCharacterTab = EPCClass::EPC_Assassin;
		SetButtonClick(EPCClass::EPC_Assassin);
	}
}

void UB2UIAllSelectiveLotteryPopupBase::OnClickedBTNWizard()
{
	if (CurrentCharacterTab != EPCClass::EPC_Wizard)
	{
		CurrentCharacterTab = EPCClass::EPC_Wizard;
		SetButtonClick(EPCClass::EPC_Wizard);
	}
}

void UB2UIAllSelectiveLotteryPopupBase::OnClickedBTNFighter()
{
	if (CurrentCharacterTab != EPCClass::EPC_Fighter)
	{
		CurrentCharacterTab = EPCClass::EPC_Fighter;
		SetButtonClick(EPCClass::EPC_Fighter);
	}
}

void UB2UIAllSelectiveLotteryPopupBase::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_SelectGladiator)
	GET_SLOT(UB2Button, BTN_SelectAssassin)
	GET_SLOT(UB2Button, BTN_SelectWizard)
	GET_SLOT(UB2Button, BTN_SelectFighter)
}

void UB2UIAllSelectiveLotteryPopupBase::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_SelectGladiator, &UB2UIAllSelectiveLotteryPopupBase::OnClickedBTNGladiator);
	BIND_CLICK_FUNC_TO_BTN(BTN_SelectAssassin, &UB2UIAllSelectiveLotteryPopupBase::OnClickedBTNAssassin);
	BIND_CLICK_FUNC_TO_BTN(BTN_SelectWizard, &UB2UIAllSelectiveLotteryPopupBase::OnClickedBTNWizard);
	BIND_CLICK_FUNC_TO_BTN(BTN_SelectFighter, &UB2UIAllSelectiveLotteryPopupBase::OnClickedBTNFighter);
}

void UB2UIAllSelectiveLotteryPopupBase::UpdateStaticText()
{
	Super::UpdateStaticText();
}

void UB2UIAllSelectiveLotteryPopupBase::SetButtonClick(EPCClass InEPCClass)
{
	EventChangedHeroMgmtPCClass(InEPCClass);
	SetItemSlotVisible(InEPCClass);
	SetSelectedItemRefId(0);
}

void UB2UIAllSelectiveLotteryPopup::InitLotterySetting()
{
	SetButtonClick(EPCClass::EPC_Gladiator);
	CurrentCharacterTab = EPCClass::EPC_Gladiator;
	LotterySurpassLevel = 0;
}

void UB2UIAllSelectiveLotteryPopup::UpdateItemSurpassLevel()
{
	for (auto& ItemUI : ItemUILists)
	{
		if (ItemUI.IsValid())
		{
			if (GetLotterySurpassLevel() > 0)
				ItemUI->SetStarGrade(ItemUI->GetItemStarGrade(), true);
		}
	}
}

void UB2UIAllSelectiveLotteryPopup::SetItemSlotVisible(EPCClass InEPCClass)
{
	int32 ChildCount = VB_ItemList->GetChildrenCount();

	for (int32 i = 0; i < ChildCount; ++i)
	{
		auto* ItemRow = Cast<UB2UISelectiveLotteryItemRow>(VB_ItemList->GetChildAt(i));

		if (ItemRow && ItemRow->GetItemCharacterClass() == InEPCClass)
		{
			ItemRow->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemRow->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}


void UB2UIAllSelectiveLotteryCostumePopup::DestroySelf(class UB2UIManager* InUIManager)
{
	if (UIP_CostumeDetailView.IsValid())
	{
		UIP_CostumeDetailView->DestroySelf(InUIManager);
	}
	if (UIP_SelectedCostume.IsValid())
	{
		UIP_SelectedCostume->DestroySelf(InUIManager);
	}

	for (auto& CostumeSlot : CostumeUILists)
	{
		if (CostumeSlot.IsValid())
		{
			CostumeSlot->DestroySelf(InUIManager);
		}
	}

	Super::DestroySelf(InUIManager);
}


void UB2UIAllSelectiveLotteryCostumePopup::InitLotterySetting()
{
	SetButtonClick(EPCClass::EPC_Gladiator);
	CurrentCharacterTab = EPCClass::EPC_Gladiator;
}

void UB2UIAllSelectiveLotteryCostumePopup::SetSelectedItemRefId(int32 ToSet)
{
	SelectedId = ToSet;

	if (ToSet > 0)
	{
		if (UIP_SelectedCostume.IsValid())
		{
			UIP_SelectedCostume->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UIP_SelectedCostume->BindDoc(FB2Item(BladeIIGameImpl::GetClientDataStore().GetItemMasterDetailInfo(ToSet)));
		}

		if (BTN_Select.IsValid())
		{
			BTN_Select->SetIsEnabled(true);
		}
	}
	else
	{
		if (UIP_SelectedCostume.IsValid())
		{
			UIP_SelectedCostume->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (BTN_Select.IsValid())
		{
			BTN_Select->SetIsEnabled(false);
		}
	}

	for (auto& CostumeUI : CostumeUILists)
	{
		if (CostumeUI.IsValid())
		{
			CostumeUI->SetSelected(CostumeUI->GetItemRefID() == ToSet);
		}
	}
}

void UB2UIAllSelectiveLotteryCostumePopup::SetItemRefIds_AutoCostumeSort(const TArray<int32>& ItemRefIds)
{
	if (VB_ItemList.IsValid())
	{
		VB_ItemList->ClearChildren();
		CostumeUILists.Empty();

		UB2UISelectiveLotteryCostumeRow* ItemRowUI = nullptr;

		EPCClass PrevClass = EPCClass::EPC_End;
		int32 ColumnIndex = 0;
		for (int32 ItemIndex = 0; ItemIndex < ItemRefIds.Num(); ++ItemIndex)
		{
			EPCClass TargetClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(ItemRefIds[ItemIndex]);

			if (PrevClass != TargetClass)
			{
				ColumnIndex = 0;
				PrevClass = TargetClass;
				ItemRowUI = CreateWidget<UB2UISelectiveLotteryCostumeRow>(GetWorld(), CostumeListRowUIClass);
				if (ItemRowUI)
				{
					VB_ItemList->AddChildToVerticalBox(ItemRowUI);
					ItemRowUI->SetLotteryCostumePopup(this);
					ItemRowUI->Init();
					ItemRowUI->SetItemCharacterClass(TargetClass);
				}
			}

			if (ItemRowUI)
			{
				CostumeUILists.Add(ItemRowUI->SetLotteryCostumeSlotInfo(ColumnIndex, ItemRefIds[ItemIndex]));
				ColumnIndex += 1;
			}
		}
	}
}

void UB2UIAllSelectiveLotteryCostumePopup::ShowSelectedItemDetail(class UB2UISelectiveLotteryCostumeSlot* SelectedItem)
{
	if (UIP_CostumeDetailView.IsValid() && SelectedItem)
	{
		FB2Item* selectedItemInfo = SelectedItem->GetBindedItem();

		UIP_CostumeDetailView->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(UIP_CostumeDetailView.Get());
		UIP_CostumeDetailView->UpdateCostumeData(*selectedItemInfo);

		if (BTN_ItemDetailBackGround.IsValid())
		{
			BTN_ItemDetailBackGround->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UB2UIAllSelectiveLotteryCostumePopup::HideCostumeDetailView()
{
	if (UIP_CostumeDetailView.IsValid())
	{
		UIP_CostumeDetailView->SetVisibility(ESlateVisibility::Hidden);
	}

	if (BTN_ItemDetailBackGround.IsValid())
	{
		BTN_ItemDetailBackGround->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIAllSelectiveLotteryCostumePopup::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2UISelectiveLotteryCostumeSlot, UIP_SelectedCostume);
	if (UIP_SelectedCostume.IsValid())
	{
		UIP_SelectedCostume->Init();
		UIP_SelectedCostume->SetVisibility(ESlateVisibility::Hidden);
	}

	GET_SLOT(UB2UISelectiveLotteryCostumeDT, UIP_CostumeDetailView);
	if (UIP_CostumeDetailView.IsValid())
	{
		UIP_CostumeDetailView->Init();
		FOnClickCostumeDetailClose DetailCloseDelegate;
		DetailCloseDelegate.BindUObject(this, &UB2UIAllSelectiveLotteryCostumePopup::HideCostumeDetailView);
		UIP_CostumeDetailView->SetDetailViewCloseDelegate(DetailCloseDelegate);
	}
}

void UB2UIAllSelectiveLotteryCostumePopup::BindDelegates()
{
	Super::BindDelegates();
	if (UIP_SelectedCostume.IsValid())
	{
		UIP_SelectedCostume->OnSlotCostumeClickDelgate.BindDynamic(this, &UB2UIAllSelectiveLotteryCostumePopup::ShowSelectedItemDetail);
	}
}

void UB2UIAllSelectiveLotteryCostumePopup::RequestItemSelected()
{
	if (UIP_SelectedCostume.IsValid())
	{
		int32 RequestSelectedItemRefId = UIP_SelectedCostume->GetItemRefID();
		if (RequestSelectedItemRefId > 0 && MailBox)
		{
			MailBox->SelectedItemSelectiveLottery(RequestSelectedItemRefId);
		}
	}
}

void UB2UIAllSelectiveLotteryCostumePopup::SetItemSlotVisible(EPCClass InEPCClass)
{
	int32 ChildCount = VB_ItemList->GetChildrenCount();

	for (int32 i = 0; i < ChildCount; ++i)
	{
		auto* ItemRow = Cast<UB2UISelectiveLotteryCostumeRow>(VB_ItemList->GetChildAt(i));

		if (ItemRow && ItemRow->GetItemCharacterClass() == InEPCClass)
		{
			ItemRow->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemRow->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}