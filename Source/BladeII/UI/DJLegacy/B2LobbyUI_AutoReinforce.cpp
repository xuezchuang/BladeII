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

	// 捞 矫痢俊辑 芒阑 摧栏搁 帮鄂且 芭 鞍促. 磊悼 碍拳绰 辑滚客 菩哦阑 咯矾瞒肥 背券窍骨肺 弥辆 览翠捞 柯 饶俊 捞吧 摧绊 搬苞芒栏肺 傈券窍档废. 

	if (Btn_AutoReinforceCommit.IsValid())
	{
		Btn_AutoReinforceCommit->SetIsEnabled(false); // 趣矫 葛福聪 角青 滚瓢篮 厚劝己拳..
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
	// 府胶飘啊 官差菌阑 锭 泅犁 府胶飘俊 措茄 磊悼 碍拳 抗惑 厚侩阑 夸没秦具 窍绰单 咯扁辑 官肺 喉肺欧栏肺绰 救登聪 
	// 滚瓢 殿狼 UI 甫 disable 矫挪 盲 夸没父 朝府绊 览翠篮 蝶肺 贸府.
	// 郴瘤绰 陛咀 拌魂阑 努扼捞攫飘俊辑 贸府窍霸 瞪 荐档 乐澜.
	TArray<FB2Item> AllTargetItems;
	GetItemListByFilter(AllTargetItems);

	// 厚侩 拌魂秦辑 包访 UI 甸 技泼.
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

	// 努扼捞攫飘 拌魂阑 窍扁 傈俊绰 概 鞘磐 函版付促 辑滚肺 夸没阑 焊郴辑 厚侩阑 舅酒陈绰单 捞力绰 弊凡 鞘夸绰 绝瘤父 俺惯滚傈俊辑绰 老摹窍绰瘤 眉农 沥档父 窍扁 困秦 弊措肺 夸没阑 窃.
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
	// OnAnyItemFilterCBChanged 俊辑 焊郴绰 夸没俊 蝶扼 俺惯 滚傈捞 酒聪搁 捞 览翠捞 坷瘤 臼阑 巴.

	// OnAnyItemFilterCBChanged 俊辑 Cost 府涅胶飘 朝府搁辑 Freeze 沁带 芭 秦力.
	FreezeSelectionInterface(false);

#if WITH_EDITOR
	// 努扼 拌魂苞 老摹窍绰瘤 眉农父 窍档废..
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

	// 厘馒 棺 泪败 乐绰芭 力寇. 橇府悸篮 器窃.
	UB2LobbyInventory::GetFilteredItemList(OutFilteredList, false, false, true, InvenTypeFilter, StarGradeFilter);
}

void UB2LobbyUI_AutoReinforce::GetSelectedFilters(TArray<EItemInvenType>& OutInvenTypeFilter, TArray<int32>& OutStarGradeFilter)
{
	OutInvenTypeFilter.Add(SelectedAutoReinfTab);

	for (int32 CGI = 0; CGI < AutoReinfALLITEMS_STARGRADE_NUM; ++CGI){
		if (CB_ItemGrade[CGI] && CB_ItemGrade[CGI]->IsChecked()){
			OutStarGradeFilter.Add(CGI + 1); // StarGrade 绰 1何磐 矫累.
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

	// 贸澜俊 泅犁 牢亥配府 徘栏肺 急琶秦 霖促.
	UB2LobbyInventory* LobbyInvenObj = CachedLobbyGM ? CachedLobbyGM->GetLobbyInventory() : NULL;
	if (LobbyInvenObj && SelectedAutoReinfTab == EItemInvenType::EIIVT_End)
	{
		SetSelectedAutoReinfTab(LobbyInvenObj->GetMainInvenViewCurrentTab());
		OnAnyItemFilterCBChanged();
	}

	UpdateStaticTexts();
}

void UB2LobbyUI_AutoReinforce::CloseMe()
{ // 捞扒 窜鉴洒 扑诀芒父阑 摧栏骨肺 绢恫 侥栏肺电 磊悼 碍拳啊 柳青吝牢 惑怕俊辑绰 捞繁 侥栏肺 摧栏搁 救 凳. 弊繁 版快啊 鞘夸窍搁 UB2LobbyInventory::EndAutoItemEnhance 甫 荤侩秦具.
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
			// 厚侩俊 蝶扼 disable 矫虐妨搁 酒贰贸烦. 泅 夸备荤亲篮 厚侩捞 何练茄 惑怕俊辑 矫档 矫 皋矫瘤 冠胶 剁框. UB2LobbyInventory::OnAllAutoItemLevelupCostDelivered 肺..
			//(AutoReinforceCost > 0 && BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= AutoReinforceCost && !bSelectionFreezed)
			);
	}
}