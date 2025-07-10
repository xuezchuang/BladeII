// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIFilterItemForge.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "Event.h"
#include "B2UIFilterCheckBox.h"

#include "Global.h"


UB2UIFilterItemForge::UB2UIFilterItemForge(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MinGradeFilter(1)
{

}

void UB2UIFilterItemForge::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_ItemCategory);
	GET_SLOT(UTextBlock, TB_ItemGrade);

	AllFilterCheckBox.Empty();

	GET_SLOT(UB2UIFilterCheckBox, UIP_All);
	AllFilterCheckBox.Emplace(UIP_All);
	GET_SLOT(UB2UIFilterCheckBox, UIP_Weapon);
	AllFilterCheckBox.Emplace(UIP_Weapon);
	GET_SLOT(UB2UIFilterCheckBox, UIP_Protection);
	AllFilterCheckBox.Emplace(UIP_Protection);
	GET_SLOT(UB2UIFilterCheckBox, UIP_Accessory);
	AllFilterCheckBox.Emplace(UIP_Accessory);
	GET_SLOT(UB2UIFilterCheckBox, UIP_Item);
	AllFilterCheckBox.Emplace(UIP_Item);

	GradeFilters.Empty();
	GradeFilters.SetNum(FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE + ApplyAllAtStarGrade);

	const int32 GradeFilterCount = VisibleMaxStarGrade - MinGradeFilter + 1+ ApplyAllAtStarGrade; // ex) 1부터8까지면 MinGradeFilter = 1, VisibleMaxStarGrade = 8 ( 8 - 1 + 1 +1) 8개+ 전채

	for (int32 i = 0; i < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE + ApplyAllAtStarGrade; i++)
	{
		bool IsVisible = false;
		auto TempGradeFilter = GetCachedWidget<UB2UIFilterCheckBox>(FName(*FString::Printf(TEXT("UIP_GradeFilterCheckBox_%d"), i)));

		// 최대 - 최소해서 원하는 갯수만 캐싱
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

void UB2UIFilterItemForge::UpdateStaticText()
{
	if (TB_ItemCategory.IsValid())
		TB_ItemCategory->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Category"))));
	if (TB_ItemGrade.IsValid())
		TB_ItemGrade->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Grade"))));

	if (UIP_All.IsValid())
		UIP_All->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_Filter_Total"))));
	if (UIP_Weapon.IsValid())
		UIP_Weapon->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Weapon"))));
	if (UIP_Protection.IsValid())
		UIP_Protection->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Protection"))));
	if (UIP_Accessory.IsValid())
		UIP_Accessory->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Accessory"))));
	if (UIP_Item.IsValid())
		UIP_Item->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_TotalItem"))));

	SetMinGradeFilter();
	ResetFilter();
}

void UB2UIFilterItemForge::BindDelegates()
{
	for (int32 i = 0; i < AllFilterCheckBox.Num(); ++i)
	{
		if (AllFilterCheckBox[i].IsValid())
			AllFilterCheckBox[i]->OnCheckStateChanged.AddDynamic(this, &UB2UIFilterItemForge::FilterCBChanged);
	}
}

void UB2UIFilterItemForge::SetMinGradeFilter()
{
	for (int32 i = 0; i < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE + ApplyAllAtStarGrade; i++)
	{
		if (!GradeFilters.IsValidIndex(i))
			continue;

		if (i == 0)
		{
			if (GradeFilters[i].IsValid())
			{
				GradeFilters[i]->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ItemFactory_Filter_Total"))));
			}
		}
		else
		{
			if (GradeFilters[i].IsValid())
			{
				GradeFilters[i]->SetLabel(FText::FromString(FString::FormatAsNumber(i)));
			}
		}
	}
}

void UB2UIFilterItemForge::GetSelectedFilters(TArray<EItemInvenType>& OutInvenTypeFilter, TArray<int32>& OutStarGradeFilter, bool checkAllBox)
{
	OutInvenTypeFilter.Empty();
	OutStarGradeFilter.Empty();

	if (checkAllBox == false)
	{
		if (UIP_All.IsValid() && UIP_All->IsChecked())
		{
			OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Weapon);
			OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Protection);
			OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Accessory);
			OutInvenTypeFilter.Add(EItemInvenType::EIIVT_End);
		}
		else
		{
			if (UIP_Weapon.IsValid() && UIP_Weapon->IsChecked())
				OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Weapon);

			if (UIP_Protection.IsValid() && UIP_Protection->IsChecked())
				OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Protection);

			if (UIP_Accessory.IsValid() && UIP_Accessory->IsChecked())
				OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Accessory);

			//무슨 버그인지 몰라도 아이템을 Consumables가 아닌 End로 인식해서 End로 정함.
			if (UIP_Item.IsValid() && UIP_Item->IsChecked())
				OutInvenTypeFilter.Add(EItemInvenType::EIIVT_End);
		}

		if (GradeFilters[0].IsValid() && GradeFilters[0]->IsChecked())
		{
			for (int32 Ccount = 1 * ApplyAllAtStarGrade; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE + ApplyAllAtStarGrade; ++Ccount)
			{
				OutStarGradeFilter.Add(Ccount);
			}
		}
		else
		{
			for (int32 Ccount = 1 * ApplyAllAtStarGrade; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE + ApplyAllAtStarGrade; ++Ccount)
			{
				if (GradeFilters[Ccount].IsValid() && GradeFilters[Ccount]->IsChecked())
				{
					OutStarGradeFilter.Add(Ccount);
				}
			}
		}
	}
	else
	{
		if (UIP_All.IsValid() && UIP_All->IsChecked())
			OutInvenTypeFilter.Add(EItemInvenType::EIIVT_United);

		if (UIP_Weapon.IsValid() && UIP_Weapon->IsChecked())
			OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Weapon);

		if (UIP_Protection.IsValid() && UIP_Protection->IsChecked())
			OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Protection);

		if (UIP_Accessory.IsValid() && UIP_Accessory->IsChecked())
			OutInvenTypeFilter.Add(EItemInvenType::EIIVT_Accessory);

		//무슨 버그인지 몰라도 아이템을 Consumables가 아닌 End로 인식해서 End로 정함.
		if (UIP_Item.IsValid() && UIP_Item->IsChecked())
			OutInvenTypeFilter.Add(EItemInvenType::EIIVT_End);

		for (int32 Ccount = 0; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE + ApplyAllAtStarGrade; ++Ccount)
		{
			if (GradeFilters[Ccount].IsValid() && GradeFilters[Ccount]->IsChecked())
			{
				OutStarGradeFilter.Add(Ccount);
			}
		}
	}
}

void UB2UIFilterItemForge::SaveFilterInfo(const TCHAR* InSectionName)
{
	if (UIP_All.IsValid())
	{
		GConfig->SetBool(InSectionName, *FString::Printf(TEXT("All")), UIP_All->IsChecked(), GB2UserSavedStateIni);
	}

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

	if (UIP_Item.IsValid())
	{
		GConfig->SetBool(InSectionName, *FString::Printf(TEXT("Item")), UIP_Item->IsChecked(), GB2UserSavedStateIni);
	}

	for (int32 Ccount = 0; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE+ ApplyAllAtStarGrade; ++Ccount)
	{
		if (GradeFilters[Ccount].IsValid())
		{
			GConfig->SetBool(InSectionName, *FString::FromInt(Ccount), GradeFilters[Ccount]->IsChecked(), GB2UserSavedStateIni);
		}
	}

	GConfig->Flush(false, GB2UserSavedStateIni);
}

void UB2UIFilterItemForge::LoadFilterInfo(const TCHAR* InSectionName)
{
	bool CheckedValue = true;

	if (UIP_All.IsValid())
	{
		CheckedValue = true;
		GConfig->GetBool(InSectionName, *FString::Printf(TEXT("All")), CheckedValue, GB2UserSavedStateIni);
		UIP_All->SetIsChecked(CheckedValue);
	}

	if (UIP_All.IsValid())
	{
		if (UIP_All->IsChecked())
		{
			UIP_Weapon->SetIsChecked(false);
			UIP_Protection->SetIsChecked(false);
			UIP_Accessory->SetIsChecked(false);
			UIP_Item->SetIsChecked(false);
		}
		else
		{
			if (UIP_Weapon.IsValid())
			{
				CheckedValue = true;
				GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Weapon")), CheckedValue, GB2UserSavedStateIni);
				UIP_Weapon->SetIsChecked(CheckedValue);
			}

			if (UIP_Protection.IsValid())
			{
				CheckedValue = true;
				GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Protection")), CheckedValue, GB2UserSavedStateIni);
				UIP_Protection->SetIsChecked(CheckedValue);
			}

			if (UIP_Accessory.IsValid())
			{
				CheckedValue = true;
				GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Accessory")), CheckedValue, GB2UserSavedStateIni);
				UIP_Accessory->SetIsChecked(CheckedValue);
			}

			if (UIP_Item.IsValid())
			{
				CheckedValue = true;
				GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Item")), CheckedValue, GB2UserSavedStateIni);
				UIP_Item->SetIsChecked(CheckedValue);
			}
		}
	}
	else
	{
		if (UIP_Weapon.IsValid())
		{
			CheckedValue = true;
			GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Weapon")), CheckedValue, GB2UserSavedStateIni);
			UIP_Weapon->SetIsChecked(CheckedValue);
		}

		if (UIP_Protection.IsValid())
		{
			CheckedValue = true;
			GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Protection")), CheckedValue, GB2UserSavedStateIni);
			UIP_Protection->SetIsChecked(CheckedValue);
		}

		if (UIP_Accessory.IsValid())
		{
			CheckedValue = true;
			GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Accessory")), CheckedValue, GB2UserSavedStateIni);
			UIP_Accessory->SetIsChecked(CheckedValue);
		}

		if (UIP_Item.IsValid())
		{
			CheckedValue = true;
			GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Item")), CheckedValue, GB2UserSavedStateIni);
			UIP_Item->SetIsChecked(CheckedValue);
		}
	}

	
	if (ApplyAllAtStarGrade > 0)
	{
		if (GradeFilters[0].IsValid())
		{
			CheckedValue = true;
			GConfig->GetBool(InSectionName, *FString::FromInt(0), CheckedValue, GB2UserSavedStateIni);
			GradeFilters[0]->SetIsChecked(CheckedValue);

			if (GradeFilters[0]->IsChecked())
			{
				for (int32 Ccount = ApplyAllAtStarGrade; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE + ApplyAllAtStarGrade; ++Ccount)
				{
					if (GradeFilters[Ccount].IsValid())
					{
						GradeFilters[Ccount]->SetIsChecked(false);
					}
				}
			}
			else
			{
				for (int32 Ccount = 0; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE + ApplyAllAtStarGrade; ++Ccount)
				{
					if (GradeFilters[Ccount].IsValid())
					{
						CheckedValue = true;
						GConfig->GetBool(InSectionName, *FString::FromInt(Ccount), CheckedValue, GB2UserSavedStateIni);
						GradeFilters[Ccount]->SetIsChecked(CheckedValue);
					}
				}
			}
		}
	}
	else
	{
		for (int32 Ccount = 0; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE + ApplyAllAtStarGrade; ++Ccount)
		{
			if (GradeFilters[Ccount].IsValid())
			{
				CheckedValue = true;
				GConfig->GetBool(InSectionName, *FString::FromInt(Ccount), CheckedValue, GB2UserSavedStateIni);
				GradeFilters[Ccount]->SetIsChecked(CheckedValue);
			}
		}
	}
}

void UB2UIFilterItemForge::ResetFilter()
{
	for (int32 i = 0; i < AllFilterCheckBox.Num(); ++i)
	{
		if (ApplyAllAtStarGrade > 0)
		{
			if (AllFilterCheckBox[i].IsValid())
				AllFilterCheckBox[i]->SetIsChecked(false);
		}
		else
		{
			if (AllFilterCheckBox[i].IsValid())
				AllFilterCheckBox[i]->SetIsChecked(true);
		}
	}
	if (ApplyAllAtStarGrade > 0)
	{
		UIP_All->SetIsChecked(true);
		GradeFilters[0]->SetIsChecked(true);
	}
}

void UB2UIFilterItemForge::SetFilterTitle(FText TitleText)
{
	FilterTitleText = TitleText;
}

void UB2UIFilterItemForge::FilterCBChanged(bool bIsChecked)
{
	OnCheckStateChanged.Broadcast(bIsChecked);
}

void UB2UIFilterItemForge::InitData()
{
	GetSelectedFilters(PastInvenTypeFilter, PastStarGradeFilter,true);
}

//////////////////////////////////////////////////////////////////////////
//	체크박스의 상태가 바뀌었을때 호출하는 이벤트는 블루프린트에 하드코딩 되어있음.
//////////////////////////////////////////////////////////////////////////
void UB2UIFilterItemForge::AdjustCheckboxes()
{
	if (ApplyAllAtStarGrade == 0)
		return;

	TArray<EItemInvenType> InvenTypeFilter;
	TArray<int32> StarGradeFilter;

	GetSelectedFilters(InvenTypeFilter, StarGradeFilter,true);

	//무기, 방어구, 장신구, 아이템 모두 눌렸을 경우
	if (InvenTypeFilter.Num() == 4)
	{
		UIP_All->SetIsChecked(true);
		UIP_Weapon->SetIsChecked(false);
		UIP_Protection->SetIsChecked(false);
		UIP_Accessory->SetIsChecked(false);
		UIP_Item->SetIsChecked(false);
	}
	else
	{
		if (UIP_All->IsChecked())
		{
			if (PastInvenTypeFilter.Num() > 0)
			{
				if (PastInvenTypeFilter[0] != EItemInvenType::EIIVT_United)
				{
					UIP_All->SetIsChecked(true);
					UIP_Weapon->SetIsChecked(false);
					UIP_Protection->SetIsChecked(false);
					UIP_Accessory->SetIsChecked(false);
					UIP_Item->SetIsChecked(false);
				}
				else
				{
					if (InvenTypeFilter.Num() > 1)
					{
						UIP_All->SetIsChecked(false);
					}
				}
			}
		}
	}

	//모든 별이 눌렸을 경우
	if (StarGradeFilter.Num() == VisibleMaxStarGrade)
	{
		for (int32 Ccount = 1; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE+ApplyAllAtStarGrade; ++Ccount)
		{
			if (GradeFilters[Ccount].IsValid())
			{
				GradeFilters[Ccount]->SetIsChecked(false);
			}
		}
		GradeFilters[0]->SetIsChecked(true);
	}
	else
	{
		if (GradeFilters[0]->IsChecked())
		{
			if (PastStarGradeFilter.Num() > 0)
			{
				if (PastStarGradeFilter[0] != 0)
				{
					for (int32 Ccount = 1; Ccount < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE + ApplyAllAtStarGrade; ++Ccount)
					{
						if (GradeFilters[Ccount].IsValid())
						{
							GradeFilters[Ccount]->SetIsChecked(false);
						}
					}
					GradeFilters[0]->SetIsChecked(true);
				}
				else
				{
					if (StarGradeFilter.Num() > 1)
					{
						GradeFilters[0]->SetIsChecked(false);
					}
				}
			}
		}
	}

	InitData();
}