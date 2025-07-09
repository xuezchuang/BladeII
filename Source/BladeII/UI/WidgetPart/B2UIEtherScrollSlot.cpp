// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEtherScrollSlot.h"
#include "B2UIEther.h"
#include "B2EtherManager.h"
#include "B2ItemInfo.h"
#include "B2EtherInfo.h"
#include "BladeIIUtil.h"
#include "TutorialManager.h"
#include "B2UIStarGradePart.h"


void UB2UIEtherScrollSlot::Init()
{
	Super::Init();
	this->SetVisibility(ESlateVisibility::Visible);
	
	if (UIP_Star.IsValid())
	{
		UIP_Star->Init();
	}
}

void UB2UIEtherScrollSlot::SetRowSlotInfo(class UB2UIEther* Class, FB2Ether* SlotEther)
{
	OnEtherSlotTouch.BindUObject(Class, &UB2UIEther::GetScrollDetailPopup);
	SetInfo(SlotEther);
}

void UB2UIEtherScrollSlot::CacheAssets()
{
	GET_SLOT(UB2RichTextBlock, EtherID);

	GET_SLOT(UImage, X_IMG_SetImage);
	GET_SLOT(UImage, X_IMG_Lock);
	GET_SLOT(UImage, X_IMG_PosImage);
	GET_SLOT(UImage, X_IMG_BG);
	GET_SLOT(UB2UIWidgetBase, UIP_RedDot);
	GET_SLOT(UTextBlock, X_TB_EtherName);
	GET_SLOT(UTextBlock, X_TB_EnhaceLevel);
	
	//실제로 눌리는게 아닌 시그널 호출용 튜토리얼 버튼
	GET_SLOT(UB2Button, BTN_Tutorial);
	GET_SLOT(UB2UIStarGradePart, UIP_Star);
	GET_SLOT(UPanelWidget, X_PW_EquipUI);
	UpdateEquipUI(false);
}

void UB2UIEtherScrollSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	OnEtherSlotTouch.Unbind();
	if (UIP_Star.IsValid())
	{
		UIP_Star->DestroySelf(InUIManager);
	}
}

void UB2UIEtherScrollSlot::UpdateLock(bool Value)
{
	if (X_IMG_Lock.IsValid())
	{
		if (Value)
		{
			X_IMG_Lock->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			X_IMG_Lock->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIEtherScrollSlot::UpdateEquipUI(bool Value)
{
	if (X_PW_EquipUI.IsValid())
	{
		if (Value)
		{
			X_PW_EquipUI->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			X_PW_EquipUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIEtherScrollSlot::UpdateRedDot(bool Value)
{
	if (UIP_RedDot.IsValid())
	{
		UIP_RedDot->SetVisibility(ESlateVisibility::Collapsed);
		if (Value)
		{
			UIP_RedDot->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			UIP_RedDot->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIEtherScrollSlot::SetInfo(const FB2Ether* SlotEther)
{
	if (SlotEther)
	{
		ID = SlotEther->ID;
		
		SetPosImage(true, SlotEther->Part);
		SetEtherImageInfo(SlotEther->Tier, SlotEther->Type);
		SetEtherStar(SlotEther->Star);
		SetEtherName(SlotEther->Tier, SlotEther->Type);
		SetEtherEnhaceLevel(SlotEther->EnhanceLevel);
		UpdateLock(SlotEther->IsLock);
		UpdateRedDot(SlotEther->IsNew);

		//ethertest
		EtherID->SetText(FText::FromString(FString::Printf(TEXT("ID %d"), SlotEther->ID)));
	}
}

void UB2UIEtherScrollSlot::SetInfo(int32 InType, int32 InTier, int32 InStar)
{
	ID = -1;

	SetPosImage(false, -1);
	SetEtherImageInfo(InTier, InType);
	SetEtherStar(InStar);
	SetEtherName(InTier, InType);
	SetEtherEnhaceLevel(-1);
	UpdateLock(false);
	UpdateRedDot(false);
}

void UB2UIEtherScrollSlot::OnClickedSenderBTNArea()
{
	OnEtherSlotTouch.ExecuteIfBound(ID);

	PlayClickSoundEther_BP();

	//TUTORIAL HardCording
	if (TutorialManager::GetInstance().HasAnyTutorial())
	{
		if (BTN_Tutorial.IsValid())
		{ BTN_Tutorial->TutorialButtonSignal(); }
	}
}

void UB2UIEtherScrollSlot::SetEtherImageInfo(int32 InTier, int32 InType)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (ItemMiscInfo)
	{
		if (X_IMG_BG.IsValid())
		{
			X_IMG_BG->SetBrushFromMaterial(ItemMiscInfo->GetEtherTierBGMtrl(InTier));
		}
	}

	if (X_IMG_SetImage.IsValid() && StaticFindEtherInfoTable() != NULL)
	{
		X_IMG_SetImage->SetBrushFromMaterial(StaticFindEtherInfoTable()->GetEtherIcon(InType));
		FSlateColor Color = ItemMiscInfo->GetEtherTierTextColor(InTier);
		X_IMG_SetImage->SetColorAndOpacity(Color.GetSpecifiedColor());
	}
}

void UB2UIEtherScrollSlot::SetEtherStar(int32 InStar)
{
	if (UIP_Star.IsValid())
	{
		UIP_Star->UpdateStar(InStar, false);
	}
}

void UB2UIEtherScrollSlot::SetEtherName(int32 InTier, int32 InType)
{
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (X_TB_EtherName.IsValid())
	{
		X_TB_EtherName->SetColorAndOpacity(ItemMiscInfo->GetEtherTierTextColor(InTier));
		X_TB_EtherName->SetText(B2EtherManager::GetInstance().GetEtherName(InType));
	}
}

void UB2UIEtherScrollSlot::SetEtherEnhaceLevel(int32 InEnhanceLevel)
{
	if (X_TB_EnhaceLevel.IsValid())
	{
		if (InEnhanceLevel > 0)
		{
			X_TB_EnhaceLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			X_TB_EnhaceLevel->SetText(FText::FromString(FString::Printf(TEXT("+%d"), InEnhanceLevel)));
		}
		else
		{
			X_TB_EnhaceLevel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIEtherScrollSlot::SetPosImage(bool Value, int32 Part)
{
	if (X_IMG_PosImage.IsValid())
	{
		if (Value)
		{
			UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
			if (ItemMiscInfo)
			{
				X_IMG_PosImage->SetBrushFromMaterial(ItemMiscInfo->GetEtherPosBGMtrl(Part));
			}
			X_IMG_PosImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			X_IMG_PosImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

