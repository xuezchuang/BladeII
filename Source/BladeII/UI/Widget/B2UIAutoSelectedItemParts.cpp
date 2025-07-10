// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIAutoSelectedItemParts.h"
#include "B2UIManager.h"

#include "B2UILevelupAutoSelect.h"
#include "B2UIEtherInfoNotify.h"


EEnable_Touch UB2UIAutoSelectedItemParts::IsEnableClick()
{
	if (IsSelected == false)
	{
		auto* AutoLevelUp = UB2UIManager::GetInstance()->GetUI<UB2UILevelupAutoSelect>(UIFName::LevelupAutoSelect);

		if (AutoLevelUp)
		{
			if (AutoLevelUp->GetPendingItemCount() >= UB2UILevelupAutoSelect::MAX_ITEM_COUNT)
			{
				return EEnable_Touch::DISABLE_OVERCOUNT;
			}

			if (AutoLevelUp->IsMaxItemLevelUpExpectedByCurrentSelection())
			{
				return EEnable_Touch::DISABLE_MAXLEVEL;
			}
		}

		if (ItemGrade >= HighRankGradeWarning)
		{
			return EEnable_Touch::DISABLE_HIGHRANK;
		}
	}
	return EEnable_Touch::ENABLE;
}

void UB2UIAutoSelectedItemParts::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_StarCount);
	GET_SLOT(UTextBlock, TB_StarCount_UnSelect);
	GET_SLOT(UTextBlock, TB_ItemCount);
	GET_SLOT(UTextBlock, TB_ItemCount_UnSelect);
	GET_SLOT(UTextBlock, TB_MaxItemCount);

	GET_SLOT(UB2Button, BTN_Selected);
	GET_SLOT(UImage, IMG_Selected);
	GET_SLOT(UImage, IMG_SelectedCheck);
}

void UB2UIAutoSelectedItemParts::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Selected, &UB2UIAutoSelectedItemParts::OnClickBTN_Selected)
}

void UB2UIAutoSelectedItemParts::Init()
{
	Super::Init();

	ItemGrade = 0;
	MaxItemCount = 0;
	CurrentItemCount = 0;
	IsSelected = false;
}

void UB2UIAutoSelectedItemParts::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIAutoSelectedItemParts::SetData(int32 StartCount, int32 ItemCount, int32 InMaxItemCount)
{
	ItemGrade = StartCount;
	MaxItemCount = InMaxItemCount;
	CurrentItemCount = ItemCount;

	if (ItemCount <= 0 || MaxItemCount <= 0)
	{
		IsSelected = false;
	}

	if (TB_StarCount.IsValid())
	{
		TB_StarCount->SetText(FText::AsNumber(StartCount));
	}

	if (TB_StarCount_UnSelect.IsValid())
	{
		TB_StarCount_UnSelect->SetText(FText::AsNumber(StartCount));
	}

	if (TB_ItemCount.IsValid())
	{
		TB_ItemCount->SetText(FText::AsNumber(ItemCount));
	}

	if (TB_ItemCount_UnSelect.IsValid())
	{
		TB_ItemCount_UnSelect->SetText(FText::AsNumber(ItemCount));
	}

	if (TB_MaxItemCount.IsValid())
	{
		TB_MaxItemCount->SetText(FText::AsNumber(MaxItemCount));
	}

	SetUIVisibility(IsSelected);
}


void UB2UIAutoSelectedItemParts::SetUIVisibility(bool InSelected)
{
	if (TB_StarCount_UnSelect.IsValid())
	{
		TB_StarCount_UnSelect->SetVisibility(InSelected ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	if (TB_ItemCount_UnSelect.IsValid())
	{
		TB_ItemCount_UnSelect->SetVisibility(InSelected ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	if (TB_StarCount.IsValid())
	{
		TB_StarCount->SetVisibility(InSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (TB_ItemCount.IsValid())
	{
		TB_ItemCount->SetVisibility(InSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (IMG_Selected.IsValid())
	{
		IMG_Selected->SetVisibility(InSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (IMG_SelectedCheck.IsValid())
	{
		IMG_SelectedCheck->SetVisibility(InSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}


void UB2UIAutoSelectedItemParts::SelectPart()
{
	IsSelected = !IsSelected;

	SetUIVisibility(IsSelected);

	auto* AutoLevelUp = UB2UIManager::GetInstance()->GetUI<UB2UILevelupAutoSelect>(UIFName::LevelupAutoSelect);

	if (AutoLevelUp)
	{
		if (IsSelected)
		{
			AutoLevelUp->SetSelectItems(ItemGrade);
		}
		else
		{
			AutoLevelUp->SetUnSelectItems(ItemGrade);
		}
	}
}

void UB2UIAutoSelectedItemParts::OnClickBTN_Selected()
{
	switch (IsEnableClick())
	{
	case EEnable_Touch::DISABLE_OVERCOUNT:
	{
		FText sPopupMessage;
		sPopupMessage = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AlreadySelectedMaxEnhanceIngred")), FText::AsNumber(UB2UILevelupAutoSelect::MAX_ITEM_COUNT));
		UB2UIEtherInfoNotify* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIEtherInfoNotify>(UIFName::EtherInfoNotify);
		if (Popup)
		{
			Popup->SetInfoNotify(sPopupMessage);
		}
	}
	return;
	case EEnable_Touch::DISABLE_MAXLEVEL:
	{
		FText sPopupMessage;
		sPopupMessage = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AlreadyExpectedMaxEnhanceLevel")), FText::AsNumber(UB2UILevelupAutoSelect::MAX_ITEM_LEVEL));
		UB2UIEtherInfoNotify* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIEtherInfoNotify>(UIFName::EtherInfoNotify);
		if (Popup)
		{
			Popup->SetInfoNotify(sPopupMessage);
		}
	}
	return;
	case EEnable_Touch::DISABLE_HIGHRANK:
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInven_WarningEquipFilter"))),
				ItemGrade, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LevelupAutoSelectTitle"))),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([this]() {
			SelectPart();
		}));
	}
	return;
	default:
		break;
	}

	SelectPart();
}