// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIFilterInventory.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "Event.h"
#include "B2UIFilterCheckBox.h"

#include "Global.h"

UB2UIFilterInventory::UB2UIFilterInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MinGradeFilter(1)
{

}

void UB2UIFilterInventory::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_ItemCategory);
	GET_SLOT(UTextBlock, TB_ItemGrade);
	GET_SLOT(UTextBlock, TB_Etc);

	AllFilterCheckBox.Empty();

	GET_SLOT(UB2UIFilterCheckBox, UIP_Weapon);
	AllFilterCheckBox.Emplace(UIP_Weapon);
	GET_SLOT(UB2UIFilterCheckBox, UIP_Protection);
	AllFilterCheckBox.Emplace(UIP_Protection);
	GET_SLOT(UB2UIFilterCheckBox, UIP_Accessory);
	AllFilterCheckBox.Emplace(UIP_Accessory);

	GET_SLOT(UB2UIFilterCheckBox, UIP_LVUp);
	AllFilterCheckBox.Emplace(UIP_LVUp);
	GET_SLOT(UB2UIFilterCheckBox, UIP_Enhance);
	AllFilterCheckBox.Emplace(UIP_Enhance);
	GET_SLOT(UB2UIFilterCheckBox, UIP_Essence);
	AllFilterCheckBox.Emplace(UIP_Essence);

	GradeFilters.Empty();
	GradeFilters.SetNum(FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE);

	const int32 GradeFilterCount = VisibleMaxStarGrade - MinGradeFilter + 1; // ex) 1何磐8鳖瘤搁 MinGradeFilter = 1, VisibleMaxStarGrade = 8 ( 8 - 1 + 1) 8俺

	for (int32 i = 0; i < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; i++)
	{
		bool IsVisible = false;
		auto TempGradeFilter = GetCachedWidget<UB2UIFilterCheckBox>(FName(*FString::Printf(TEXT("UIP_GradeFilterCheckBox_%d"), i + 1)));

		// 弥措 - 弥家秦辑 盔窍绰 肮荐父 某教
		if (i < GradeFilterCount)
		{
			GradeFilters[i] = TempGradeFilter;
			AllFilterCheckBox.Emplace(GradeFilters[i]);
			IsVisible = true;
		}

		if (TempGradeFilter != nullptr)
		{
			TempGradeFilter->SetVisibility(IsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}

	for (int32 i = 0; i < AllFilterCheckBox.Num(); i++)
	{
		if (AllFilterCheckBox[i].IsValid())
			AllFilterCheckBox[i]->Init();
	}
}

void UB2UIFilterInventory::UpdateStaticText()
{
	if (TB_ItemCategory.IsValid())
		TB_ItemCategory->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Category"))));
	if (TB_ItemGrade.IsValid())
		TB_ItemGrade->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Grade"))));
	if (TB_Etc.IsValid())
		TB_Etc->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Etc"))));

	if (UIP_Weapon.IsValid())
		UIP_Weapon->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Weapon"))));
	if (UIP_Protection.IsValid())
		UIP_Protection->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Protection"))));
	if (UIP_Accessory.IsValid())
		UIP_Accessory->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Accessory"))));

	if (UIP_LVUp.IsValid())
		UIP_LVUp->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Filter_LevelUp"))));
	if (UIP_Enhance.IsValid())
		UIP_Enhance->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Filter_Enhancement"))));
	if (UIP_Essence.IsValid())
		UIP_Essence->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Filter_Essence"))));

	SetMinGradeFilter();
}

void UB2UIFilterInventory::BindDelegates()
{
	for (int32 i = 0; i < AllFilterCheckBox.Num(); ++i)
	{
		if (AllFilterCheckBox[i].IsValid())
			AllFilterCheckBox[i]->OnCheckStateChanged.AddDynamic(this, &UB2UIFilterInventory::FilterCBChanged);
	}
}

void UB2UIFilterInventory::BindWarningPopup()
{
	for (int32 i = WarningPopupStartGrade - MinGradeFilter; i < GradeFilters.Num(); i++)
	{
		if (GradeFilters[i].IsValid())
		{
			int32 RealGrade = i + MinGradeFilter;

			OnPreSelectFilter OnPreSelect;
			OnPreSelect.BindLambda(
				[RealGrade, this](UB2UIFilterCheckBox* FilterCheckBox) {
				if (FilterCheckBox && FilterCheckBox->IsChecked())
					return;

				UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
					BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
					FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_WarningEquipFilter"))),
						RealGrade, this->FiltetTitleText),
					0.f,
					true,
					true,
					EUIMsgPopupButtonGroup::YesOrNo,
					FMsgPopupOnClick::CreateLambda([FilterCheckBox]() {
					if (FilterCheckBox)
						FilterCheckBox->SetIsChecked(true);
				}));
			});

			GradeFilters[i]->SetPreFilterCheck(OnPreSelect);
		}
	}
}

void UB2UIFilterInventory::SetMinGradeFilter()
{
	for (int32 i = 0; i < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; i++)
	{
		if (!GradeFilters.IsValidIndex(i))
			continue;

		if (GradeFilters[i].IsValid())
			GradeFilters[i]->SetLabel(FText::FromString(FString::FormatAsNumber(i + MinGradeFilter)));
	}
}

bool UB2UIFilterInventory::GetAllTargetItems(TArray<FB2Item>& OutList, EPCClass PCClass)
{
	TArray<EItemInvenType> InvenTypeFilter;
	TArray<int32> StarGradeFilter;
	bool IsLVUp, IsEnhance, IsEssence, IsAnvil;
	GetSelectedFilters(InvenTypeFilter, StarGradeFilter, IsLVUp, IsEnhance, IsEssence, IsAnvil);

	// 厘馒 棺 泪陛, 橇府悸俊 厘馒登绢 乐绰芭 力寇.
	UB2LobbyInventory::GetFilteredItemList(OutList, false, false, false, InvenTypeFilter, StarGradeFilter, PCClass, IsLVUp, IsEnhance, IsEssence, IsAnvil);

	return OutList.Num() > 0;
}

void UB2UIFilterInventory::SaveFilterInfo(const TCHAR* InSectionName)
{
	if (UIP_Weapon.IsValid())
	{
		GConfig->SetBool(InSectionName, *FString::Printf(TEXT("Weapon")), UIP_Weapon->IsChecked(), GB2UserSavedStateIni);
	}

	if (UIP_Protection.IsValid())
	{
		GConfig->SetBool(InSectionName, *FString::Printf(TEXT("Protection")), UIP_Protection->IsChecked(), GB2UserSavedStateIni);
	}

	if (UIP_Accessory.IsValid())
	{
		GConfig->SetBool(InSectionName, *FString::Printf(TEXT("Accessory")), UIP_Accessory->IsChecked(), GB2UserSavedStateIni);
	}

	for (int32 Ccount = 0; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; ++Ccount)
	{
		if (GradeFilters[Ccount].IsValid())
		{
			GConfig->SetBool(InSectionName, *FString::FromInt(Ccount + MinGradeFilter), GradeFilters[Ccount]->IsChecked(), GB2UserSavedStateIni);
		}
	}

	if (UIP_LVUp.IsValid())
	{
		GConfig->SetBool(InSectionName, *FString::Printf(TEXT("LVUp")), UIP_LVUp->IsChecked(), GB2UserSavedStateIni);
	}

	if (UIP_Enhance.IsValid())
	{
		GConfig->SetBool(InSectionName, *FString::Printf(TEXT("Enhance")), UIP_Enhance->IsChecked(), GB2UserSavedStateIni);
	}

	if (UIP_Essence.IsValid())
	{
		GConfig->SetBool(InSectionName, *FString::Printf(TEXT("Essence")), UIP_Essence->IsChecked(), GB2UserSavedStateIni);
	}

	GConfig->Flush(false, GB2UserSavedStateIni);
}

void UB2UIFilterInventory::LoadFilterInfo(const TCHAR* InSectionName)
{
	bool CheckedValue = false;
	if (UIP_Weapon.IsValid())
	{
		GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Weapon")), CheckedValue, GB2UserSavedStateIni);
		UIP_Weapon->SetIsChecked(CheckedValue);
	}

	if (UIP_Protection.IsValid())
	{
		CheckedValue = false;
		GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Protection")), CheckedValue, GB2UserSavedStateIni);
		UIP_Protection->SetIsChecked(CheckedValue);
	}

	if (UIP_Accessory.IsValid())
	{
		CheckedValue = false;
		GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Accessory")), CheckedValue, GB2UserSavedStateIni);
		UIP_Accessory->SetIsChecked(CheckedValue);
	}

	for (int32 Ccount = 0; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; ++Ccount)
	{
		if (GradeFilters[Ccount].IsValid())
		{
			CheckedValue = false;
			GConfig->GetBool(InSectionName, *FString::FromInt(Ccount + MinGradeFilter), CheckedValue, GB2UserSavedStateIni);
			GradeFilters[Ccount]->SetIsChecked(CheckedValue);
		}
	}

	if (UIP_LVUp.IsValid())
	{
		CheckedValue = false;
		GConfig->GetBool(InSectionName, *FString::Printf(TEXT("LVUp")), CheckedValue, GB2UserSavedStateIni);
		UIP_LVUp->SetIsChecked(CheckedValue);
	}

	if (UIP_Enhance.IsValid())
	{
		CheckedValue = false;
		GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Enhance")), CheckedValue, GB2UserSavedStateIni);
		UIP_Enhance->SetIsChecked(CheckedValue);
	}

	if (UIP_Essence.IsValid())
	{
		CheckedValue = false;
		GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Essence")), CheckedValue, GB2UserSavedStateIni);
		UIP_Essence->SetIsChecked(CheckedValue);
	}
}

void UB2UIFilterInventory::ResetFilter()
{
	for (int32 i = 0; i < AllFilterCheckBox.Num(); ++i)
	{
		if (AllFilterCheckBox[i].IsValid())
			AllFilterCheckBox[i]->SetIsChecked(false);
	}
}

void UB2UIFilterInventory::SetFilterTitle(FText TitleText)
{
	FiltetTitleText = TitleText;
}

void UB2UIFilterInventory::SetVisibleEssenceCheckBox(ESlateVisibility InVisibility)
{
	if (UIP_Essence.IsValid())
		UIP_Essence->SetVisibility(InVisibility);
}

void UB2UIFilterInventory::GetSelectedFilters(TArray<EItemInvenType>& OutInvenTypeFilter, TArray<int32>& OutStarGradeFilter, bool & IsLVUp, bool & IsEnhance, bool & IsEssence, bool & IsAnvil)
{
	if (UIP_Weapon.IsValid() && UIP_Weapon->IsChecked())
		OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Weapon);
	
	if (UIP_Protection.IsValid() && UIP_Protection->IsChecked())
		OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Protection);
	
	if (UIP_Accessory.IsValid() && UIP_Accessory->IsChecked())
		OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Accessory);

	for (int32 Ccount = 0; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; ++Ccount)
	{
		if (GradeFilters[Ccount].IsValid() && GradeFilters[Ccount]->IsChecked())
			OutStarGradeFilter.Add(Ccount + MinGradeFilter); // StarGrade 绰 MinGrade 何磐 矫累.
	}

	IsLVUp = UIP_LVUp.IsValid() ? UIP_LVUp->IsChecked() : false;
	IsEnhance = UIP_Enhance.IsValid() ? UIP_Enhance->IsChecked() : false;
	IsEssence = UIP_Essence.IsValid() ? UIP_Essence->IsChecked() : false;
	IsAnvil = false;
}

void UB2UIFilterInventory::FilterCBChanged(bool bIsChecked)
{
	OnCheckStateChanged.Broadcast(bIsChecked);
}
