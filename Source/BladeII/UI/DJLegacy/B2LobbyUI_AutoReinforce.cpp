// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_AutoReinforce.h"
//#include "BladeII.h"

#include "Event.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"

UB2LobbyUI_AutoReinforce::UB2LobbyUI_AutoReinforce(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SelectedAutoReinfTab = EItemInvenType::EIIVT_End;

	TB_AutoReinfTitle = NULL;
	TB_AutoRef = NULL;
	TB_GoldToAcquire = NULL;
	TB_Weapon = NULL;
	TB_Protection = NULL;
	TB_Accessory = NULL;
	TB_ItemGrade = NULL;
	Btn_Close = NULL;

	CachedLastPreviewAutoEnhanceCost = 0;
}

void UB2LobbyUI_AutoReinforce::OnCloseButtonClicked()
{
	CloseMe();
}

void UB2LobbyUI_AutoReinforce::OnClickTapSelect(EItemInvenType InTabSelection)
{
	SetSelectedAutoReinfTab(InTabSelection);
}

void UB2LobbyUI_AutoReinforce::SetSelectedAutoReinfTab(EItemInvenType InSelection)
{
	(SelectedAutoReinfTab == InSelection) ? SelectedAutoReinfTab = EItemInvenType::EIIVT_End : SelectedAutoReinfTab = InSelection;
}

void UB2LobbyUI_AutoReinforce::OnClickAutoReinforceCommit()
{
	TArray<FB2Item> AllTargetItems;
	GetItemListByFilter(AllTargetItems);

	LobbyReqAutoItemLevelupClass<const TArray<FB2Item>&, bool>::GetInstance().Signal(AllTargetItems, false);

	// �� �������� â�� ������ ����� �� ����. �ڵ� ��ȭ�� ������ ��Ŷ�� �������� ��ȯ�ϹǷ� ���� ������ �� �Ŀ� �̰� �ݰ� ���â���� ��ȯ�ϵ���. 

	if (Btn_AutoReinforceCommit.IsValid())
	{
		Btn_AutoReinforceCommit->SetIsEnabled(false); // Ȥ�� �𸣴� ���� ��ư�� ��Ȱ��ȭ..
	}
}

ESlateVisibility UB2LobbyUI_AutoReinforce::GetWeaponVisibility() const
{
	
	return GetAutoReinfTabSelectedImageVisibilityCommon(EItemInvenType::EIIVT_Weapon);
}

ESlateVisibility UB2LobbyUI_AutoReinforce::GetProtectionVisibility() const
{

	return GetAutoReinfTabSelectedImageVisibilityCommon(EItemInvenType::EIIVT_Protection);
}

ESlateVisibility UB2LobbyUI_AutoReinforce::GetAccessoryVisibility() const
{

	return GetAutoReinfTabSelectedImageVisibilityCommon(EItemInvenType::EIIVT_Accessory);
}

void UB2LobbyUI_AutoReinforce::OnAnyItemFilterCBChanged()
{
	// ����Ʈ�� �ٲ���� �� ���� ����Ʈ�� ���� �ڵ� ��ȭ ���� ����� ��û�ؾ� �ϴµ� ���⼭ �ٷ� ���ŷ���δ� �ȵǴ� 
	// ��ư ���� UI �� disable ��Ų ä ��û�� ������ ������ ���� ó��.
	// ������ �ݾ� ����� Ŭ���̾�Ʈ���� ó���ϰ� �� ���� ����.
	TArray<FB2Item> AllTargetItems;
	GetItemListByFilter(AllTargetItems);

	// ��� ����ؼ� ���� UI �� ����.
	CachedLastPreviewAutoEnhanceCost = 0;
	int32 LvUpItemCount = 0;
	int32 IngredItemCount = 0;
	
	UB2LobbyInventory* LobbyInvenObj = CachedLobbyGM ? CachedLobbyGM->GetLobbyInventory() : NULL;
	if (LobbyInvenObj)
		LobbyInvenObj->GetAutoItemLevelupPreviewInfo(AllTargetItems, CachedLastPreviewAutoEnhanceCost, LvUpItemCount, IngredItemCount);
	
	if (TB_GoldToAcquire.IsValid())
	{
		TB_GoldToAcquire->SetText(FText::AsNumber(CachedLastPreviewAutoEnhanceCost));
	}

	UpdateAutoReinforceCommitBtnEnable(CachedLastPreviewAutoEnhanceCost, false);

	// Ŭ���̾�Ʈ ����� �ϱ� ������ �� ���� ���渶�� ������ ��û�� ������ ����� �˾Ƴ´µ� ������ �׷� �ʿ�� ������ ���߹��������� ��ġ�ϴ��� üũ ������ �ϱ� ���� �״�� ��û�� ��.
#if WITH_EDITOR	
	if (AllTargetItems.Num() > 0)
	{
		FreezeSelectionInterface(true);
		LobbyReqAutoItemLevelupClass<const TArray<FB2Item>&, bool>::GetInstance().Signal(AllTargetItems, true);
	}
#endif
}

void UB2LobbyUI_AutoReinforce::UpdateOnAutoItemLevelupCostDelivered(int32 DeliveredCost)
{
	// OnAnyItemFilterCBChanged ���� ������ ��û�� ���� ���� ������ �ƴϸ� �� ������ ���� ���� ��.

	// OnAnyItemFilterCBChanged ���� Cost ������Ʈ �����鼭 Freeze �ߴ� �� ����.
	FreezeSelectionInterface(false);

#if WITH_EDITOR
	// Ŭ�� ���� ��ġ�ϴ��� üũ�� �ϵ���..
	checkSlow(CachedLastPreviewAutoEnhanceCost == DeliveredCost);
#endif
}

ESlateVisibility UB2LobbyUI_AutoReinforce::GetAutoReinfTabSelectedImageVisibilityCommon(EItemInvenType InTabSelection) const
{
	return (SelectedAutoReinfTab == InTabSelection) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
}

void UB2LobbyUI_AutoReinforce::UpdateStaticTexts()
{
	if (TB_AutoEnhanceNote_1.IsValid())
	{
		TB_AutoEnhanceNote_1->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoItemLevelup_BasicInfo")));
	}
	if (TB_AutoEnhanceNote_2.IsValid())
	{
		TB_AutoEnhanceNote_2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoItemLevelup_EnhanceCondition")));
	}
}

void UB2LobbyUI_AutoReinforce::GetItemListByFilter(TArray<FB2Item>& OutFilteredList)
{
	TArray<EItemInvenType> InvenTypeFilter;
	TArray<int32> StarGradeFilter;
	GetSelectedFilters(InvenTypeFilter, StarGradeFilter);

	// ���� �� ��� �ִ°� ����. �������� ����.
	UB2LobbyInventory::GetFilteredItemList(OutFilteredList, false, false, true, InvenTypeFilter, StarGradeFilter);
}

void UB2LobbyUI_AutoReinforce::GetSelectedFilters(TArray<EItemInvenType>& OutInvenTypeFilter, TArray<int32>& OutStarGradeFilter)
{
	OutInvenTypeFilter.Add(SelectedAutoReinfTab);

	for (int32 CGI = 0; CGI < AutoReinfALLITEMS_STARGRADE_NUM; ++CGI){
		if (CB_ItemGrade[CGI] && CB_ItemGrade[CGI]->IsChecked()){
			OutStarGradeFilter.Add(CGI + 1); // StarGrade �� 1���� ����.
		}
	}
}

void UB2LobbyUI_AutoReinforce::NativeConstruct()
{
	Super::NativeConstruct();

	GET_SLOT(UTextBlock, TB_AutoReinfTitle);
	GET_SLOT(UTextBlock, TB_AutoRef);
	GET_SLOT(UTextBlock, TB_GoldToAcquire);
	GET_SLOT(UTextBlock, TB_Weapon);
	GET_SLOT(UTextBlock, TB_Protection);
	GET_SLOT(UTextBlock, TB_Accessory);
	GET_SLOT(UTextBlock, TB_ItemGrade);
	GET_SLOT(UB2Button, BTN_Weapon);
	GET_SLOT(UB2Button, BTN_WeaponSelect);
	GET_SLOT(UB2Button, BTN_Protection);
	GET_SLOT(UB2Button, BTN_ProtectionSelect);
	GET_SLOT(UB2Button, BTN_Accessory);
	GET_SLOT(UB2Button, BTN_AccessorySelect);
	GET_SLOT(UCheckBox, CB_ItemGrade_1);
	GET_SLOT(UCheckBox, CB_ItemGrade_2);
	GET_SLOT(UCheckBox, CB_ItemGrade_3);
	GET_SLOT(UCheckBox, CB_ItemGrade_4);
	GET_SLOT(UCheckBox, CB_ItemGrade_5);
	GET_SLOT(UCheckBox, CB_ItemGrade_6);
	GET_SLOT(UB2Button, Btn_Close);
	GET_SLOT(UB2Button, Btn_AutoReinforceCommit);

	GET_SLOT(UTextBlock, TB_AutoEnhanceNote_1);
	GET_SLOT(UTextBlock, TB_AutoEnhanceNote_2);
	
	if (TB_AutoReinfTitle.IsValid())
		TB_AutoReinfTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AutoItemLevelup")));

	if (TB_AutoRef.IsValid())
		TB_AutoRef->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AutoItemLevelup")));

	if (TB_Weapon.IsValid())
		TB_Weapon->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_WeaponTab")));

	if (TB_Protection.IsValid())
		TB_Protection->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_ProtectionTab")));

	if (TB_Accessory.IsValid())
		TB_Accessory->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AccessoryTab")));

	if (TB_ItemGrade.IsValid())
		TB_ItemGrade->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Grade")));

	if (TB_GoldToAcquire.IsValid())
		TB_GoldToAcquire->SetText(FText::AsNumber(0));


	CB_ItemGrade[0] = CB_ItemGrade_1.Get();
	CB_ItemGrade[1] = CB_ItemGrade_2.Get();
	CB_ItemGrade[2] = CB_ItemGrade_3.Get();
	CB_ItemGrade[3] = CB_ItemGrade_4.Get();
	CB_ItemGrade[4] = CB_ItemGrade_5.Get();
	CB_ItemGrade[5] = CB_ItemGrade_6.Get();
}

void UB2LobbyUI_AutoReinforce::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	// ó���� ���� �κ��丮 ������ ������ �ش�.
	UB2LobbyInventory* LobbyInvenObj = CachedLobbyGM ? CachedLobbyGM->GetLobbyInventory() : NULL;
	if (LobbyInvenObj && SelectedAutoReinfTab == EItemInvenType::EIIVT_End)
	{
		SetSelectedAutoReinfTab(LobbyInvenObj->GetMainInvenViewCurrentTab());
		OnAnyItemFilterCBChanged();
	}

	UpdateStaticTexts();
}

void UB2LobbyUI_AutoReinforce::CloseMe()
{ // �̰� �ܼ��� �˾�â���� �����Ƿ� � �����ε� �ڵ� ��ȭ�� �������� ���¿����� �̷� ������ ������ �� ��. �׷� ��찡 �ʿ��ϸ� UB2LobbyInventory::EndAutoItemEnhance �� ����ؾ�.
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoItemLevelup);
}

void UB2LobbyUI_AutoReinforce::FreezeSelectionInterface(bool bFreeze)
{
	UpdateAutoReinforceCommitBtnEnable(CachedLastPreviewAutoEnhanceCost, bFreeze);

	if (BTN_Weapon.IsValid())
	{
		BTN_Weapon->SetIsEnabled(!bFreeze);
	}
	if (BTN_WeaponSelect.IsValid())
	{
		BTN_WeaponSelect->SetIsEnabled(!bFreeze);
	}
	if (BTN_Protection.IsValid())
	{
		BTN_Protection->SetIsEnabled(!bFreeze);
	}
	if (BTN_ProtectionSelect.IsValid())
	{
		BTN_ProtectionSelect->SetIsEnabled(!bFreeze);
	}
	if (BTN_Accessory.IsValid())
	{
		BTN_Accessory->SetIsEnabled(!bFreeze);
	}
	if (BTN_AccessorySelect.IsValid())
	{
		BTN_AccessorySelect->SetIsEnabled(!bFreeze);
	}
	if (CB_ItemGrade_1.IsValid())
	{
		CB_ItemGrade_1->SetIsEnabled(!bFreeze);
	}
	if (CB_ItemGrade_2.IsValid())
	{
		CB_ItemGrade_2->SetIsEnabled(!bFreeze);
	}
	if (CB_ItemGrade_3.IsValid())
	{
		CB_ItemGrade_3->SetIsEnabled(!bFreeze);
	}
	if (CB_ItemGrade_4.IsValid())
	{
		CB_ItemGrade_4->SetIsEnabled(!bFreeze);
	}
	if (CB_ItemGrade_5.IsValid())
	{
		CB_ItemGrade_5->SetIsEnabled(!bFreeze);
	}
	if (CB_ItemGrade_6.IsValid())
	{
		CB_ItemGrade_6->SetIsEnabled(!bFreeze);
	}
}

void UB2LobbyUI_AutoReinforce::UpdateAutoReinforceCommitBtnEnable(int32 AutoReinforceCost, bool bSelectionFreezed)
{
	if (Btn_AutoReinforceCommit.IsValid())
	{
		Btn_AutoReinforceCommit->SetIsEnabled(
			!bSelectionFreezed
			// ��뿡 ���� disable ��Ű���� �Ʒ�ó��. �� �䱸������ ����� ������ ���¿��� �õ� �� �޽��� �ڽ� ���. UB2LobbyInventory::OnAllAutoItemLevelupCostDelivered ��..
			//(AutoReinforceCost > 0 && BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= AutoReinforceCost && !bSelectionFreezed)
			);
	}
}