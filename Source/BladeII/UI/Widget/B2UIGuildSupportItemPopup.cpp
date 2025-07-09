// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIGuildSupportCharacterSlot.h"
#include "B2UIGuildSupportItemPopup.h"
#include "B2DynItemIcon_GuildSupport.h"
#include "B2UIGuildSupportRowSlot.h"
#include "B2Inventory.h"
#include "B2GuildDataStore.h"
#include "B2LobbyInventory.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIGuildSupportItemPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);
	
	GET_SLOT(UB2ScrollBox, SB_Items);
	GET_SLOT(UB2Button, BTN_Close);

	GET_SLOT(UB2Button, BTN_RequestItem);
	SetRequestState(false);

	GET_SLOT(UTextBlock, TB_RequestItem);

	CracterTabs.SetNum(PCClassToInt(EPCClass::EPC_End));
	GET_SLOT_BYNAME(UB2UIGuildSupportCharacterSlot, CracterTabs[0], CharacterSlot_0);
	GET_SLOT_BYNAME(UB2UIGuildSupportCharacterSlot, CracterTabs[1], CharacterSlot_1);
	GET_SLOT_BYNAME(UB2UIGuildSupportCharacterSlot, CracterTabs[2], CharacterSlot_2);
	GET_SLOT_BYNAME(UB2UIGuildSupportCharacterSlot, CracterTabs[3], CharacterSlot_3);

	EPCClass MainClass = EPCClass::EPC_End;
	EPCClass SubClass = EPCClass::EPC_End;
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		MainClass = IntToPCClass(DocBattle->GetCurPCClass());
		SubClass = IntToPCClass(DocBattle->GetTagPCClass());
	}
	for (int32 i = 0; i < CracterTabs.Num(); ++i)
	{
		CracterTabs[i]->Init();

		EPCClass PCClass = IntToPCClass(i);
		if (PCClass == MainClass)
			CracterTabs[i]->SetMainHero(true);
		else if (PCClass == SubClass)
			CracterTabs[i]->SetMainHero(false);
		else
			CracterTabs[i]->SetDefaultHero();

		CracterTabs[i]->SetHeroImage(PCClass);
	}
}

void UB2UIGuildSupportItemPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGuildSupportItemPopup::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_RequestItem, &UB2UIGuildSupportItemPopup::OnClickRequestItem);
	
	for (int32 i = 0; i < CracterTabs.Num(); ++i)
	{
		if (CracterTabs[i].IsValid())
		{
			CracterTabs[i]->OnClickedDelgate.BindDynamic(this, &UB2UIGuildSupportItemPopup::OnClickedCharacterTab);
		}
	}
}

void UB2UIGuildSupportItemPopup::UpdateStaticText()
{
	Super::UpdateStaticText();

 	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EssenceRequestPopup_InfoMessage")));
 	if (TB_RequestItem.IsValid())
		TB_RequestItem->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EssenceRequestPopup_RequestBtn")));
}

void UB2UIGuildSupportItemPopup::Init()
{
	Super::Init();

	CachedTempleteID = 0;
	CurrentPCClass = EPCClass::EPC_End;
	CachedGuildSupportIcon = nullptr;
}

void UB2UIGuildSupportItemPopup::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UIGuildSupportItemPopup::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	CAPTURE_UOBJECT(UB2UIGuildSupportItemPopup);

	DeliveryAskGuildSupportTicket = DeliveryAskGuildSupportClass<FB2ResponseAskGuildSupportPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryAskGuildSupport, const FB2ResponseAskGuildSupportPtr &Rewards)
		Capture->OnClickClose();
		END_CAPTURE_OBJECT()
	);

	UpdateCharacterTab(EPCClass::EPC_Gladiator);
}


void UB2UIGuildSupportItemPopup::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);

	for (int32 i = 0; i < CreatedItemRows.Num(); ++i)
	{
		UB2UIGuildSupportRowSlot* NewItemRow = CreatedItemRows[i];
		NewItemRow->OnDelegate.Unbind();
	}

	DeliveryAskGuildSupportClass<FB2ResponseAskGuildSupportPtr>::GetInstance().Unsubscribe(DeliveryAskGuildSupportTicket);
}

void UB2UIGuildSupportItemPopup::UpdateCharacterTab(EPCClass CharType)
{
	const int32 TargetIndex = PCClassToInt(CharType);
	for (int32 i = 0; i < CracterTabs.Num(); ++i)
	{
		if (CracterTabs[i].IsValid())
		{
			CracterTabs[i]->SetSwitchState(i == TargetIndex);
		}
	}

	CurrentPCClass = CharType;
	UpdateStoredGuildItemIcons();
}

void UB2UIGuildSupportItemPopup::DestroyAllItemRows()
{
	for (int32 II = 0; II < CreatedItemRows.Num(); ++II)
	{
		if (CreatedItemRows[II])
		{
			CreatedItemRows[II]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIGuildSupportItemPopup::UpdateStoredGuildItemIcons()
{
	DestroyAllItemRows(); // 기존에 만들었던 거 안보이게
	if (ItemIconRowWidgetClass == NULL || SB_Items.IsValid() == false) {
		return;
	}

	TArray<FB2Item> CurrPCItemList;
	BladeIIGameImpl::GetGuildDataStore().GetGuildSupportItems(CurrentPCClass, CurrPCItemList);
	const int32 TotalCount = FMath::Max(0, CurrPCItemList.Num() - 1);
	const int32 RequiredRowNum = FMath::RoundToInt(TotalCount /  (float)B2GuildRequestItem::SlotColumnMax);

	// 추가로 더 필요한 슬롯 생성.
	for (int32 i = CreatedItemRows.Num(); i < RequiredRowNum; ++i)
	{
		UB2UIGuildSupportRowSlot* NewItemRow = CreateWidget<UB2UIGuildSupportRowSlot>(GetOwningPlayer(), ItemIconRowWidgetClass);
		if (NewItemRow == NULL)
			continue;

		UScrollBoxSlot* AddedRowSlot = Cast<UScrollBoxSlot>(SB_Items->AddChild(NewItemRow));
		if (AddedRowSlot)
		{
		}

		NewItemRow->Init();
		NewItemRow->OnDelegate.BindDynamic(this, &UB2UIGuildSupportItemPopup::OnCallbackClickIcon);
		CreatedItemRows.Add(NewItemRow);
	}

	FText ClassName = BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(CurrentPCClass);
	TArray<FText> RequestFormats =
	{
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EssenceRequestPopup_WeaponEssence")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EssenceRequestPopup_ArmorEssence")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EssenceRequestPopup_AccessoryEssence"))
	};
	// 생성된 슬롯 초기화
	for (int32 RI = 0; RI < RequiredRowNum; ++RI)
	{
		UB2UIGuildSupportRowSlot* NewItemRow = CreatedItemRows[RI];
		if (NewItemRow == NULL)
			continue;

		if (NewItemRow->GetVisibility() == ESlateVisibility::Collapsed)
			NewItemRow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (RequestFormats.IsValidIndex(RI))
			NewItemRow->SetCategoryName(FText::Format(RequestFormats[RI], ClassName));
		else
			NewItemRow->SetCategoryName(FText::GetEmpty());

		NewItemRow->UpdateItemIcons(CurrPCItemList, RI *  B2GuildRequestItem::SlotColumnMax);
	}
}

void UB2UIGuildSupportItemPopup::OnClickedCharacterTab(EPCClass ClickedHero)
{
	if (CurrentPCClass == ClickedHero)
		return;

	UpdateCharacterTab(ClickedHero);

	CachedGuildSupportIcon = nullptr;
	CachedTempleteID = 0;
	SetRequestState(false);
}

void UB2UIGuildSupportItemPopup::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::GuildSupportItemPopup);
}

void UB2UIGuildSupportItemPopup::OnClickRequestItem()
{
	if (CachedTempleteID < 1)
		return;
	data_trader::Retailer::GetInstance().RequestAskGuildSupport(CachedTempleteID);
}

void UB2UIGuildSupportItemPopup::OnCallbackClickIcon(class UB2DynItemIcon_GuildSupport* ItemIcon, int32 TempleteItemID)
{
	if (ItemIcon == nullptr)
		return; 

	if (CachedGuildSupportIcon)
	{
		CachedGuildSupportIcon->SetSelectIcon(false);
	}
	CachedGuildSupportIcon = ItemIcon;
	CachedTempleteID = TempleteItemID;
	if (CachedGuildSupportIcon)
	{
		CachedGuildSupportIcon->SetSelectIcon(true);

		SetRequestState(true);
	}
}

void UB2UIGuildSupportItemPopup::SetRequestState(bool IsEnabled)
{
	if (BTN_RequestItem.IsValid())
		BTN_RequestItem->SetIsEnabled(IsEnabled);
}