// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIFilterCheckBox.h"
#include "B2LobbyUI_AutoSell.h"
#include "Global.h"


void UB2LobbyUI_AutoSell::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_AutoSellTitle);
	GET_SLOT(UB2Button, BTN_Cancel);

	GET_SLOT(UTextBlock, TB_ItemCategory);
	GET_SLOT(UTextBlock, TB_ItemGrade);
	for (int32 i = 0; i < MAX_AUTOSELL_ITEM_CATEGORY; i++)
	{
		Filter_Category.Add(i);
		Filter_Category[i] = GetCachedWidget<UB2UIFilterCheckBox>(FName(*FString::Printf(TEXT("UIP_Category_%d"), i)));
		if (Filter_Category[i].IsValid())
		{
			Filter_Category[i]->Init();
			Filter_Category[i]->SetLabel(SetCategoryLabel(i + 1));
			Filter_Category[i]->OnCheckStateChanged.AddDynamic(this, &UB2LobbyUI_AutoSell::OnClickCategoryCheckBox);
			CategoryArray.Add(Filter_Category[i]);
		}
	}

	Filter_EtherCategory = GetCachedWidget<UB2UIFilterCheckBox>(FName(*FString::Printf(TEXT("UIP_EtherCategory"))));
	if (Filter_EtherCategory.IsValid())
	{
		Filter_EtherCategory->Init();
		Filter_EtherCategory->SetLabel(SetCategoryLabel(4));
		Filter_EtherCategory->OnCheckStateChanged.AddDynamic(this, &UB2LobbyUI_AutoSell::OnClickCategoryCheckBox);
	}

	for (int32 i = 0; i < MAX_AUTOSELL_ITEM_GRADE; i++)
	{
		Filter_Grade.Add(i);
		Filter_Grade[i] = GetCachedWidget<UB2UIFilterCheckBox>(FName(*FString::Printf(TEXT("UIP_Grade_%d"), i)));
		if (Filter_Grade[i].IsValid())
		{
			Filter_Grade[i]->Init();
			Filter_Grade[i]->SetLabel(SetGradeLabel(i + 1));
			Filter_Grade[i]->CheckButtonDelegateBindLambda(
				[this, i]() { OnClickGradeCheckBox(i);
			});
			GradeArray.Add(Filter_Grade[i]);
		}
	}
	for (int32 i = 0; i < MAX_AUTOSELL_ITEM_GRADE; i++)
	{
		Filter_EtherGrade.Add(i);
		Filter_EtherGrade[i] = GetCachedWidget<UB2UIFilterCheckBox>(FName(*FString::Printf(TEXT("UIP_EtherGrade_%d"), i)));
		if (Filter_EtherGrade[i].IsValid())
		{
			Filter_EtherGrade[i]->Init();
			Filter_EtherGrade[i]->SetLabel(SetGradeLabel(i + 1));
			Filter_EtherGrade[i]->CheckButtonDelegateBindLambda(
				[this, i]() { OnClickEtherGradeCheckBox(i);
			});
			EtherGradeArray.Add(Filter_EtherGrade[i]);
		}
	}

	GET_SLOT(UTextBlock, TB_AutoSellDesc);

	GET_SLOT(UB2Button, BTN_AutoSellConfirm);
	GET_SLOT(UTextBlock, TB_AutoSellConfirm);

	GET_SLOT(UB2Button, BTN_Release);
	GET_SLOT(UTextBlock, TB_Release);
}

void UB2LobbyUI_AutoSell::UpdateStaticText()
{
	if (TB_AutoSellTitle.IsValid())
		TB_AutoSellTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoSell_Title")));
	if (TB_AutoSellDesc.IsValid())
		TB_AutoSellDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoSell_Desc")));
	if (TB_AutoSellConfirm.IsValid())
		TB_AutoSellConfirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoSell_Confirm")));
	if (TB_Release.IsValid())
		TB_Release->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoSell_Release")));
}

void UB2LobbyUI_AutoSell::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2LobbyUI_AutoSell::OnClickBTN_Cancel);
	BIND_CLICK_FUNC_TO_BTN(BTN_AutoSellConfirm, &UB2LobbyUI_AutoSell::OnClickBTN_AutoSellConfirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Release, &UB2LobbyUI_AutoSell::OnClickBTN_Release);
}

void UB2LobbyUI_AutoSell::CloseWidgetDelegate()
{
	CloseSelf();
}

void UB2LobbyUI_AutoSell::Init()
{
	Super::Init();

	LoadFilterInfo();

	UpdateButonState();
}

void UB2LobbyUI_AutoSell::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

FText UB2LobbyUI_AutoSell::SetCategoryLabel(int32 index)
{
	switch (static_cast<EAutoSellCategoryType>(index))
	{
	case EAutoSellCategoryType::WEAPON:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Weapon"));
	case EAutoSellCategoryType::ARMOR:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Protection"));
	case EAutoSellCategoryType::ACCESSORY:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Accessory"));
	case EAutoSellCategoryType::ETHER:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Ether"));
	}

	return FText();
}

FText UB2LobbyUI_AutoSell::SetGradeLabel(int32 index)
{
	return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoSell_Item")), index);
}

void UB2LobbyUI_AutoSell::SaveFilterInfo()
{
	if (GConfig)
	{
		for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Category)
		{
			if (Elem.Value.IsValid())
			{
				int32 Category = Elem.Key;
				GConfig->SetBool(TEXT("AutoSell"), *FString::Printf(TEXT("Category%d"), Category), Elem.Value->IsChecked(), GB2UserSavedStateIni);
			}
		}
		GConfig->SetInt(TEXT("AutoSell"), *FString::Printf(TEXT("Grade")), GetGradeFilterInfo(), GB2UserSavedStateIni);

		GConfig->SetBool(TEXT("AutoSell"), *FString::Printf(TEXT("EtherCategory")), Filter_EtherCategory->IsChecked(), GB2UserSavedStateIni);
		GConfig->SetInt(TEXT("AutoSell"), *FString::Printf(TEXT("EtherGrade")), GetEtherGradeFilterInfo(), GB2UserSavedStateIni);

		GConfig->Flush(false, GB2UserSavedStateIni);
	}
}

void UB2LobbyUI_AutoSell::LoadFilterInfo()
{
	if (GConfig)
	{
		for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Category)
		{
			if (Elem.Value.IsValid())
			{
				bool CheckedValue = false;
				int32 Category = Elem.Key;
				GConfig->GetBool(TEXT("AutoSell"), *FString::Printf(TEXT("Category%d"), Category), CheckedValue, GB2UserSavedStateIni);
				Elem.Value->SetIsChecked(CheckedValue);
			}
		}

		for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Grade)
		{
			if (Elem.Value.IsValid())
			{
				int32 GradeValue = 0;
				GConfig->GetInt(TEXT("AutoSell"), *FString::Printf(TEXT("Grade")), GradeValue, GB2UserSavedStateIni);
				Elem.Value->SetIsChecked(Elem.Key == (GradeValue - 1) ? true : false, false);
			}
		}

		bool EtherCheckedValue = false;
		GConfig->GetBool(TEXT("AutoSell"), *FString::Printf(TEXT("EtherCategory")), EtherCheckedValue, GB2UserSavedStateIni);
		if (Filter_EtherCategory.IsValid())
		{
			Filter_EtherCategory->SetIsChecked(EtherCheckedValue);
		}

		for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_EtherGrade)
		{
			if (Elem.Value.IsValid())
			{
				int32 EtherGradeValue = 0;
				GConfig->GetInt(TEXT("AutoSell"), *FString::Printf(TEXT("EtherGrade")), EtherGradeValue, GB2UserSavedStateIni);
				Elem.Value->SetIsChecked(Elem.Key == (EtherGradeValue - 1) ? true : false, false);
			}
		}
	}
}

void UB2LobbyUI_AutoSell::ResetFilterInfos()
{
	for (int i = 0; i < CategoryArray.Num(); ++i)
	{
		if (CategoryArray[i].IsValid())
			CategoryArray[i]->SetIsChecked(false);
	}
	for (int i = 0; i < GradeArray.Num(); ++i)
	{
		if (GradeArray[i].IsValid())
			GradeArray[i]->SetIsChecked(false, false);
	}
	
	if (Filter_EtherCategory.IsValid())
	{
		Filter_EtherCategory->SetIsChecked(false);
	}
	for (int i = 0; i < EtherGradeArray.Num(); ++i)
	{
		if (EtherGradeArray[i].IsValid())
			EtherGradeArray[i]->SetIsChecked(false, false);
	}
}

int32 UB2LobbyUI_AutoSell::GetGradeFilterInfo()
{
	for (int i = 0; i < Filter_Grade.Num(); i++)
	{
		if (Filter_Grade[i].IsValid() && Filter_Grade[i]->IsChecked())
		{
			return i + 1;
		}
	}

	return 0;
}

int32 UB2LobbyUI_AutoSell::GetEtherGradeFilterInfo()
{
	for (int i = 0; i < Filter_EtherGrade.Num(); i++)
	{
		if (Filter_EtherGrade[i].IsValid() && Filter_EtherGrade[i]->IsChecked())
		{
			return i + 1;
		}
	}

	return 0;
}

void UB2LobbyUI_AutoSell::UpdateButonState()
{
	bool bCategoryEnable = false;
	bool bGradeEnable = false;
	bool bEtherCategoryEnable = false;
	bool bEtherGradeEnable = false;

	for (int i = 0; i < CategoryArray.Num(); ++i)
	{
		if (CategoryArray[i].IsValid() && CategoryArray[i]->IsChecked())
		{
			bCategoryEnable = true;
			break;
		}
	}
	for (int i = 0; i < GradeArray.Num(); ++i)
	{
		if (GradeArray[i].IsValid() && GradeArray[i]->IsChecked())
		{
			bGradeEnable = true;
			break;
		}
	}

	if (Filter_EtherCategory.IsValid() && Filter_EtherCategory->IsChecked())
	{
		bEtherCategoryEnable = true;
	}
	for (int i = 0; i < EtherGradeArray.Num(); ++i)
	{
		if (EtherGradeArray[i].IsValid() && EtherGradeArray[i]->IsChecked())
		{
			bEtherGradeEnable = true;
			break;
		}
	}

	if (BTN_AutoSellConfirm.IsValid())
		BTN_AutoSellConfirm->SetIsEnabled(bCategoryEnable == bGradeEnable && bEtherCategoryEnable == bEtherGradeEnable);

	if (BTN_Release.IsValid())
		BTN_Release->SetIsEnabled(true);
}

void UB2LobbyUI_AutoSell::OnClickCategoryCheckBox(bool bCheck)
{
	UpdateButonState();
}

void UB2LobbyUI_AutoSell::OnClickGradeCheckBox(int32 ButtonIndex)
{
	for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_Grade)
	{
		if (Elem.Value.IsValid())
		{
			Elem.Value->SetIsChecked(Elem.Key == ButtonIndex ? Elem.Value->IsChecked() : false, false);
		}
	}

	UpdateButonState();
}

void UB2LobbyUI_AutoSell::OnClickEtherGradeCheckBox(int32 ButtonIndex)
{
	for (TPair<int32, TWeakObjectPtr<UB2UIFilterCheckBox>>& Elem : Filter_EtherGrade)
	{
		if (Elem.Value.IsValid())
		{
			Elem.Value->SetIsChecked(Elem.Key == ButtonIndex ? Elem.Value->IsChecked() : false, false);
		}
	}

	UpdateButonState();
}

void UB2LobbyUI_AutoSell::OnClickBTN_Cancel()
{
	CloseSelf();
}

void UB2LobbyUI_AutoSell::OnClickBTN_AutoSellConfirm()
{
	SaveFilterInfo();

	RefreshAutoSellStateClass<>::GetInstance().Signal();

	CloseSelf();
}

void UB2LobbyUI_AutoSell::OnClickBTN_Release()
{
	ResetFilterInfos();
	SaveFilterInfo();

	RefreshAutoSellStateClass<>::GetInstance().Signal();

	CloseSelf();
}
