// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherFilteringSlot.h"
#include "B2UIEther.h"
#include "B2EtherManager.h"
#include "B2EtherInfo.h"

#include "B2GameDefine.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIEtherFilteringSlot::SetIcon(int32 InType)
{
	SlotType = InType;
	if (StaticFindEtherInfoTable() != NULL && SlotType != b2network::B2AetherType::ALL)
	{
		if (X_TB_SetName.IsValid())
		{
			X_TB_SetName->SetText(FText::FromString(StaticFindEtherInfoTable()->GetEtherTypeName(SlotType)));
		}

		if (X_IMG_Type.IsValid())
		{
			X_IMG_Type->SetBrushFromMaterial(StaticFindEtherInfoTable()->GetEtherIcon(SlotType));
		}

		b2network::B2mdAetherSetOptionInfoPtr SetOptionPtr = B2EtherManager::GetInstance().GetEtherSetOptionInfoPtrByType(InType, 1); //tier绰 1肺 劝侩
		if (SetOptionPtr != nullptr && X_iMG_DefenseBG.IsValid() && X_IMG_OffenseBG.IsValid())
		{
			if (SetOptionPtr->min_apply_count == EtherSetCount::DEFENCE) //规绢 技飘
			{
				X_iMG_DefenseBG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				X_IMG_OffenseBG->SetVisibility(ESlateVisibility::Collapsed);
			}
			else if (SetOptionPtr->min_apply_count == EtherSetCount::OFFENCE) // 傍拜 技飘
			{
				X_iMG_DefenseBG->SetVisibility(ESlateVisibility::Collapsed);
				X_IMG_OffenseBG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}
	else
	{
		if (X_iMG_DefenseBG.IsValid() && X_IMG_OffenseBG.IsValid())
		{
			X_iMG_DefenseBG->SetVisibility(ESlateVisibility::Collapsed);
			X_IMG_OffenseBG->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIEtherFilteringSlot::SetCount(int32 InType)
{
	const int32 MAXCOUNT = 99; // 肮荐啊 99俺啊 逞栏搁 钎矫甫 促福霸 秦具茄促
	SlotType = InType;
	if (X_TB_Count.IsValid())
	{
		int32 Count = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetScrollEther(SlotType).Num();
		if (Count <= MAXCOUNT)
		{
			X_TB_Count->SetText(FText::AsNumber(Count));
		}
		else if (Count > MAXCOUNT)
		{
			X_TB_Count->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_Inventory_MaxCount"))));
		}
	}
}

void UB2UIEtherFilteringSlot::SetReceiver(class UB2UIEther* InReceiver)
{
	OnSlotTouch.BindUObject(InReceiver, &UB2UIEther::SetEtherScroll);
}

void UB2UIEtherFilteringSlot::Init()
{
	Super::Init();
	this->SetVisibility(ESlateVisibility::Visible);
}

void UB2UIEtherFilteringSlot::CacheAssets()
{
	GET_SLOT(UTextBlock, X_TB_Count);
	GET_SLOT(UTextBlock, X_TB_SetName);
	GET_SLOT(UImage, X_IMG_Type);
	GET_SLOT(UImage, X_IMG_OffenseBG);
	GET_SLOT(UImage, X_iMG_DefenseBG);
}

void UB2UIEtherFilteringSlot::OnClickedSenderBTNArea()
{
	OnSlotTouch.ExecuteIfBound(SlotType);
}
