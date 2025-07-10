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

	// 이 시점에서 창을 닫으면 곤란할 거 같다. 자동 강화는 서버와 패킷을 여러차례 교환하므로 최종 응답이 온 후에 이걸 닫고 결과창으로 전환하도록. 

	if (Btn_AutoReinforceCommit.IsValid())
	{
		Btn_AutoReinforceCommit->SetIsEnabled(false); // 혹시 모르니 실행 버튼은 비활성화..
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
	// 리스트가 바뀌었을 때 현재 리스트에 대한 자동 강화 예상 비용을 요청해야 하는데 여기서 바로 블로킹으로는 안되니 
	// 버튼 등의 UI 를 disable 시킨 채 요청만 날리고 응답은 따로 처리.
	// 내지는 금액 계산을 클라이언트에서 처리하게 될 수도 있음.
	TArray<FB2Item> AllTargetItems;
	GetItemListByFilter(AllTargetItems);

	// 비용 계산해서 관련 UI 들 세팅.
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

	// 클라이언트 계산을 하기 전에는 매 필터 변경마다 서버로 요청을 보내서 비용을 알아냈는데 이제는 그럴 필요는 없지만 개발버전에서는 일치하는지 체크 정도만 하기 위해 그대로 요청을 함.
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
	// OnAnyItemFilterCBChanged 에서 보내는 요청에 따라 개발 버전이 아니면 이 응답이 오지 않을 것.

	// OnAnyItemFilterCBChanged 에서 Cost 리퀘스트 날리면서 Freeze 했던 거 해제.
	FreezeSelectionInterface(false);

#if WITH_EDITOR
	// 클라 계산과 일치하는지 체크만 하도록..
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

	// 장착 및 잠겨 있는거 제외. 프리셋은 포함.
	UB2LobbyInventory::GetFilteredItemList(OutFilteredList, false, false, true, InvenTypeFilter, StarGradeFilter);
}

void UB2LobbyUI_AutoReinforce::GetSelectedFilters(TArray<EItemInvenType>& OutInvenTypeFilter, TArray<int32>& OutStarGradeFilter)
{
	OutInvenTypeFilter.Add(SelectedAutoReinfTab);

	for (int32 CGI = 0; CGI < AutoReinfALLITEMS_STARGRADE_NUM; ++CGI){
		if (CB_ItemGrade[CGI] && CB_ItemGrade[CGI]->IsChecked()){
			OutStarGradeFilter.Add(CGI + 1); // StarGrade 는 1부터 시작.
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

	// 처음에 현재 인벤토리 탭으로 선택해 준다.
	UB2LobbyInventory* LobbyInvenObj = CachedLobbyGM ? CachedLobbyGM->GetLobbyInventory() : NULL;
	if (LobbyInvenObj && SelectedAutoReinfTab == EItemInvenType::EIIVT_End)
	{
		SetSelectedAutoReinfTab(LobbyInvenObj->GetMainInvenViewCurrentTab());
		OnAnyItemFilterCBChanged();
	}

	UpdateStaticTexts();
}

void UB2LobbyUI_AutoReinforce::CloseMe()
{ // 이건 단순히 팝업창만을 닫으므로 어떤 식으로든 자동 강화가 진행중인 상태에서는 이런 식으로 닫으면 안 됨. 그런 경우가 필요하면 UB2LobbyInventory::EndAutoItemEnhance 를 사용해야.
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
			// 비용에 따라 disable 시키려면 아래처럼. 현 요구사항은 비용이 부족한 상태에서 시도 시 메시지 박스 띄움. UB2LobbyInventory::OnAllAutoItemLevelupCostDelivered 로..
			//(AutoReinforceCost > 0 && BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= AutoReinforceCost && !bSelectionFreezed)
			);
	}
}