// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UILevelupAutoSelect.h"
#include "B2UIManager.h"
#include "Event.h"

#include "B2LobbyInventory.h"
#include "B2UIAutoSelectedItemParts.h"
#include "B2LobbyUI_ItemLevelupMain.h"
#include "B2UIManager_Lobby.h"
#include "B2LobbyGameMode.h"

UB2UILevelupAutoSelect::UB2UILevelupAutoSelect(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UB2UILevelupAutoSelect::NativeConstruct()
{
	Super::NativeConstruct();
}

void UB2UILevelupAutoSelect::Init()
{
	Super::Init();

	EnableLevelUpItem = false;
	EnableEnhanceItem = false;
	EnableEssensceItem = false;

	SetVisibleLevelupButton(EnableLevelUpItem);
	SetVisibleEnhanceButton(EnableEnhanceItem);
	SetVisibleEssensceButton(EnableEssensceItem);
		
	for (int32 i = 0; i < MAX_AUTO_GRADE; ++i)
	{
		if (SeletedParts.IsValidIndex(i))
		{
			SeletedParts[i]->Init();
		}
	}

	UpdateTargetItemInfo();
}

void UB2UILevelupAutoSelect::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Close);

	GET_SLOT(UB2Button, BTN_AutoLevelUp);
	GET_SLOT(UB2Button, BTN_Levelup);
	GET_SLOT(UB2Button, BTN_Enhance);
	GET_SLOT(UB2Button, BTN_EssenceItem);
	GET_SLOT(UB2Button, BTN_Select);

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_Guide);

	GET_SLOT(UTextBlock, TB_Levelup_Unselect);
	GET_SLOT(UTextBlock, TB_Enhance_Unselect);
	GET_SLOT(UTextBlock, TB_Essence_Unselect);
	GET_SLOT(UTextBlock, TB_Levelup_Select);
	GET_SLOT(UTextBlock, TB_Enhance_Select);
	GET_SLOT(UTextBlock, TB_Essence_Select);

	GET_SLOT(UTextBlock, TB_Levelup_Select);
	GET_SLOT(UTextBlock, TB_CurrentItemLevel);
	GET_SLOT(UTextBlock, TB_CurrentItemMaxLevel);
	GET_SLOT(UTextBlock, TB_AutoLevelUp);
	GET_SLOT(UTextBlock, TB_AutoLevelUpCost);

	GET_SLOT(UImage, IMG_Levelup_Unselect);
	GET_SLOT(UImage, IMG_Enhance_Unselect);
	GET_SLOT(UImage, IMG_Essence_Unselect);
	GET_SLOT(UImage, IMG_Levelup_Select);
	GET_SLOT(UImage, IMG_Enhance_Select);
	GET_SLOT(UImage, IMG_Essence_Select);

	GET_SLOT(UImage, IMG_Levelup_Check);
	GET_SLOT(UImage, IMG_Enhance_Check);
	GET_SLOT(UImage, IMG_Essence_Check);

	GET_SLOT(UPanelWidget, P_ItemLevel);

	SeletedParts.SetNum(MAX_AUTO_GRADE);

	for (int32 i = 0; i < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; ++i)
	{
		SeletedParts[i] = GetCachedWidget<UB2UIAutoSelectedItemParts>(FName(*FString::Printf(TEXT("UIP_AutoSelectedItemPart%d"), i)));
	}
}

void UB2UILevelupAutoSelect::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UILevelupAutoSelect::OnClicked_Close);

	BIND_CLICK_FUNC_TO_BTN(BTN_Levelup, &UB2UILevelupAutoSelect::OnClicked_Levelup);
	BIND_CLICK_FUNC_TO_BTN(BTN_Enhance, &UB2UILevelupAutoSelect::OnClicked_Enhance);
	BIND_CLICK_FUNC_TO_BTN(BTN_EssenceItem, &UB2UILevelupAutoSelect::OnClicked_Essence);
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UILevelupAutoSelect::OnClicked_Select);
	BIND_CLICK_FUNC_TO_BTN(BTN_AutoLevelUp, &UB2UILevelupAutoSelect::OnClicked_AutoLevelup);
}

void UB2UILevelupAutoSelect::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LevelupAutoSelectTitle")));
	if (TB_Guide.IsValid())
		TB_Guide->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LevelupAutoSelectMessage")));
	if (TB_AutoLevelUp.IsValid())
		TB_AutoLevelUp->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_LevelUp")));
	if (TB_SelectComplete.IsValid())
		TB_SelectComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CharacterSelect_SelectComplete")));
	if (TB_Levelup_Select.IsValid())
		TB_Levelup_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Filter_LevelUp")));
	if (TB_Levelup_Unselect.IsValid())
		TB_Levelup_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Filter_LevelUp")));
	
	if (TB_Enhance_Select.IsValid())
		TB_Enhance_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Filter_Enhancement")));
	if (TB_Enhance_Unselect.IsValid())
		TB_Enhance_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Filter_Enhancement")));

	if (TB_Essence_Select.IsValid())
		TB_Essence_Select->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Filter_Essence")));
	if (TB_Essence_Unselect.IsValid())
		TB_Essence_Unselect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Filter_Essence")));
}

void UB2UILevelupAutoSelect::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UB2UILevelupAutoSelect::CloseWidgetDelegate()
{
	OnClicked_Close();
}

void UB2UILevelupAutoSelect::SetSelectState(bool IsEnabled1, bool IsEnabled2, bool IsEnabled3)
{
	OnSelectEnabled(0, IsEnabled1);
	OnSelectEnabled(1, IsEnabled2);
	OnSelectEnabled(2, IsEnabled3);
}

bool UB2UILevelupAutoSelect::IsEnableAutoSelect(const FB2Item& IngredCandidate)
{
	if (!IsEnableLeveupItem())
	{
		if(IngredCandidate.Level > 1)
			return false;
	}
	if (!IsEnableEnhanceItem())
	{
		if(IngredCandidate.EnhanceLevel > 0)
			return false;
	}
	if (!IsEnableEssenceItem())
	{
		if(IngredCandidate.ItemClass == EItemClass::EIC_WeaponEssence || IngredCandidate.ItemClass == EItemClass::EIC_ProtectionEssence || IngredCandidate.ItemClass == EItemClass::EIC_AccessoryEssence)
			return false;
	}
	return true;
}

bool UB2UILevelupAutoSelect::IsMaxItemLevelUpExpectedByCurrentSelection()
{
	if (LobbyInventory)
	{
		return LobbyInventory->IsMaxItemLevelUpExpectedByCurrentSelection();
	}
	return false;
}

int32 UB2UILevelupAutoSelect::GetPendingItemCount()
{
	TArray<FB2Item> OutGradeItemList;

	if (LobbyInventory)
	{
		LobbyInventory->GetStoredPendingItemList(OutGradeItemList);
	}

	return OutGradeItemList.Num();
}

void UB2UILevelupAutoSelect::UpdateTargetItemInfo()
{
	auto* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	auto* DJLegacyUIMgr = LobbyGM ? LobbyGM->DJLegacy_GetLobbyUIManager() : nullptr;
	
	// 饭骇 拌魂
	if (DJLegacyUIMgr)
	{
		auto* LevelupPopup = Cast<UB2LobbyUI_ItemLevelupMain>(DJLegacyUIMgr->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_ItemLevelUpMain));
		if (LevelupPopup)
		{
			if (TB_CurrentItemLevel.IsValid())
			{
				TB_CurrentItemLevel->SetText(FText::AsNumber(LevelupPopup->GetPostItemLevel()));
			}

			if (TB_CurrentItemMaxLevel.IsValid())
			{
				TB_CurrentItemMaxLevel->SetText(FText::AsNumber(LevelupPopup->GetMaxItemLevel()));
			}
		}
	}

	// 厚侩 拌魂
	FText CostText;
	if (LobbyInventory && LobbyInventory->GetCurrentItemOpGoldCost() > 0)
	{
		CostText = FText::FromString(FString::FromInt(LobbyInventory->GetCurrentItemOpGoldCost()));
	}
	else
	{
		CostText = FText::FromString(TEXT("-"));
	}

	if (TB_AutoLevelUpCost.IsValid())
	{
		TB_AutoLevelUpCost->SetText(CostText);
	}
}

void UB2UILevelupAutoSelect::UpdateSelectedItem()
{
	for (auto& AutoSelectPart : SeletedParts)
	{
		if (AutoSelectPart->IsAutoSelected())
		{
			if (PendingItems.Contains(AutoSelectPart->GetItemGrade()))
			{
				TArray<int64> InGrades;
				TArray<FB2Item> PendingItemList = PendingItems[AutoSelectPart->GetItemGrade()];

				for (auto& InItem : PendingItemList)
				{
					InGrades.Add(InItem.InstanceUID);
				}

				if (LobbyInventory)
				{
					LobbyInventory->UnSelectForIngred(InGrades, ELobbyInvenItemOpMode::EIVIOP_Levelup);
				}
			}
		}
	}

	for (auto& AutoSelectPart : SeletedParts)
	{
		if (AutoSelectPart->IsAutoSelected())
		{
			SelectForLevelUpGradeClass<int32>::GetInstance().Signal(AutoSelectPart->GetItemGrade());
		}
	}
	UpdateItemMapForPending();
	UpdatePartsInfo();
	UpdateTargetItemInfo();
}

void UB2UILevelupAutoSelect::UpdateItemMapForGrade()
{
	TArray<FB2Item> OutGradeItemList;
	FB2Item OpTarget;

	if (LobbyInventory)
	{
		LobbyInventory->GetStoredItemList(OutGradeItemList, LobbyInventory->GetMainInvenViewCurrentTab());
		LobbyInventory->GetItemOPTargetItemData(OpTarget);
	}
	GradeItems.Empty();

	for (auto& OutItem : OutGradeItemList)
	{
		if (!GradeItems.Contains(OutItem.StarGrade))
		{
			GradeItems.Add(OutItem.StarGrade, TArray<FB2Item>());
		}
		if(OpTarget.InstanceUID == OutItem.InstanceUID)
			continue;

		GradeItems[OutItem.StarGrade].Add(OutItem);
	}
}

void UB2UILevelupAutoSelect::UpdateItemMapForPending()
{
	TArray<FB2Item> OutPendingItemList;

	if (LobbyInventory)
	{
		LobbyInventory->GetStoredPendingItemList(OutPendingItemList);
	}
	PendingItems.Empty();

	for (auto& OutItem : OutPendingItemList)
	{
		if (!PendingItems.Contains(OutItem.StarGrade))
		{
			PendingItems.Add(OutItem.StarGrade, TArray<FB2Item>());
		}
		PendingItems[OutItem.StarGrade].Add(OutItem);
	}

	if (BTN_AutoLevelUp.IsValid())
	{
		BTN_AutoLevelUp->SetIsEnabled(PendingItems.Num() > 0 ? true : false);
	}
}

void UB2UILevelupAutoSelect::UpdatePartsInfo()
{
	for (int32 i = 0; i < MAX_AUTO_GRADE; ++i)
	{
		int32 GradeItemCount = 0;
		int32 PendingItemCount = 0;

		if (GradeItems.Contains(i + 1))
		{
			GradeItemCount = GradeItems[i + 1].Num();
		}

		if (PendingItems.Contains(i + 1))
		{
			PendingItemCount = PendingItems[i + 1].Num();
		}

		SeletedParts[i]->SetData(i + 1, PendingItemCount, GradeItemCount);
	}
}

void UB2UILevelupAutoSelect::UpdateItemLevel()
{
	if (LobbyInventory)
	{
		FB2Item TargetItem;

		if (LobbyInventory->GetItemOPTargetItemData(TargetItem))
		{
			if (TB_CurrentItemLevel.IsValid())
			{
				TB_CurrentItemLevel->SetText(FText::AsNumber(TargetItem.Level));
			}

			if (TB_CurrentItemMaxLevel.IsValid())
			{
				TB_CurrentItemMaxLevel->SetText(FText::AsNumber(TargetItem.MaxLevel));
			}
		}
		else
		{
			if (P_ItemLevel.IsValid())
			{
				P_ItemLevel->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UB2UILevelupAutoSelect::SetSelectedItemPart(UB2LobbyInventory* InLobbyInventory)
{
	LobbyInventory = InLobbyInventory;
	UpdateItemMapForGrade();
	UpdateItemMapForPending();
	UpdatePartsInfo();
	UpdateTargetItemInfo();
}

void UB2UILevelupAutoSelect::SetSelectItems(int32 InGrade)
{
	SelectForLevelUpGradeClass<int32>::GetInstance().Signal(InGrade);
	UpdateItemMapForPending();
	UpdatePartsInfo();
	UpdateTargetItemInfo();
}

void UB2UILevelupAutoSelect::SetUnSelectItems(int32 InGrade)
{
	if (PendingItems.Contains(InGrade))
	{
		TArray<int64> InGrades;

		TArray<FB2Item> PendingItemList = PendingItems[InGrade];
		
		for (auto& InItem : PendingItemList)
		{
			InGrades.Add(InItem.InstanceUID);
		}

		if (LobbyInventory)
		{
			LobbyInventory->UnSelectForIngred(InGrades, ELobbyInvenItemOpMode::EIVIOP_Levelup);
		}
	}
	UpdateItemMapForPending();
	UpdatePartsInfo();
	UpdateTargetItemInfo();
}


void UB2UILevelupAutoSelect::SetVisibleLevelupButton(bool Enable)
{
	if(IMG_Levelup_Unselect.IsValid())
	{
		IMG_Levelup_Unselect->SetVisibility(Enable ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
	if (IMG_Levelup_Select.IsValid())
	{
		IMG_Levelup_Select->SetVisibility(Enable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (IMG_Levelup_Check.IsValid())
	{
		IMG_Levelup_Check->SetVisibility(Enable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (TB_Levelup_Unselect.IsValid())
	{
		TB_Levelup_Unselect->SetVisibility(Enable ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
	if (TB_Levelup_Select.IsValid())
	{
		TB_Levelup_Select->SetVisibility(Enable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UILevelupAutoSelect::SetVisibleEnhanceButton(bool Enable)
{
	if (IMG_Enhance_Unselect.IsValid())
	{
		IMG_Enhance_Unselect->SetVisibility(Enable ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
	if (IMG_Enhance_Select.IsValid())
	{
		IMG_Enhance_Select->SetVisibility(Enable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (IMG_Enhance_Check.IsValid())
	{
		IMG_Enhance_Check->SetVisibility(Enable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (TB_Enhance_Unselect.IsValid())
	{
		TB_Enhance_Unselect->SetVisibility(Enable ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
	if (TB_Enhance_Select.IsValid())
	{
		TB_Enhance_Select->SetVisibility(Enable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UILevelupAutoSelect::SetVisibleEssensceButton(bool Enable)
{
	if (IMG_Essence_Unselect.IsValid())
	{
		IMG_Essence_Unselect->SetVisibility(Enable ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
	if (IMG_Essence_Select.IsValid())
	{
		IMG_Essence_Select->SetVisibility(Enable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (IMG_Essence_Check.IsValid())
	{
		IMG_Essence_Check->SetVisibility(Enable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (TB_Essence_Unselect.IsValid())
	{
		TB_Essence_Unselect->SetVisibility(Enable ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
	if (TB_Essence_Select.IsValid())
	{
		TB_Essence_Select->SetVisibility(Enable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UILevelupAutoSelect::OnClicked_Close()
{
	for (auto& AutoSelectPart : SeletedParts)
	{
		if (AutoSelectPart->IsAutoSelected())
		{
			const int32 ItemGrade = AutoSelectPart->GetItemGrade();

			if (PendingItems.Contains(ItemGrade))
			{
				TArray<int64> InGrades;

				TArray<FB2Item> PendingItemList = PendingItems[ItemGrade];

				for (auto& InItem : PendingItemList)
				{
					InGrades.Add(InItem.InstanceUID);
				}

				if (LobbyInventory)
				{
					LobbyInventory->UnSelectForIngred(InGrades, ELobbyInvenItemOpMode::EIVIOP_Levelup);
				}
			}
		}
	}
	UB2UIManager::GetInstance()->CloseUI(UIFName::LevelupAutoSelect);
}

void UB2UILevelupAutoSelect::OnClicked_AutoLevelup()
{
	auto* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	auto* DJLegacyUIMgr = LobbyGM ? LobbyGM->DJLegacy_GetLobbyUIManager() : nullptr;

	if (DJLegacyUIMgr)
	{
		auto* LevelupPopup = Cast<UB2LobbyUI_ItemLevelupMain>(DJLegacyUIMgr->DJLegacy_FindLobbySubPopup(ELobbySubPopups::ELSPU_ItemLevelUpMain));

		if (LevelupPopup)
		{
			LevelupPopup->OnEnhanceCommit();
		}
	}

	OnClicked_Close();
}

void UB2UILevelupAutoSelect::OnClicked_Select()
{
	UE_LOG(LogBladeII, Log, TEXT("OnClicked_Select"));
	UB2UIManager::GetInstance()->CloseUI(UIFName::LevelupAutoSelect);
}

void UB2UILevelupAutoSelect::OnClicked_Levelup()
{
	UE_LOG(LogBladeII, Log, TEXT("OnClicked_Levelup"));
	EnableLevelUpItem = !EnableLevelUpItem;
	SetVisibleLevelupButton(EnableLevelUpItem);
	UpdateSelectedItem();
}

void UB2UILevelupAutoSelect::OnClicked_Enhance()
{
	UE_LOG(LogBladeII, Log, TEXT("OnClicked_Enhance"));
	EnableEnhanceItem = !EnableEnhanceItem;
	SetVisibleEnhanceButton(EnableEnhanceItem);
	UpdateSelectedItem();
}

void UB2UILevelupAutoSelect::OnClicked_Essence()
{
	UE_LOG(LogBladeII, Log, TEXT("OnClicked_Essence"));
	EnableEssensceItem = !EnableEssensceItem;
	SetVisibleEssensceButton(EnableEssensceItem);
	UpdateSelectedItem();
}
