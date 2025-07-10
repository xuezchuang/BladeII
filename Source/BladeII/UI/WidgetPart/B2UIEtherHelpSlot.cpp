// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherHelpSlot.h"
#include "B2EtherSetOptionInfo.h"
#include "B2EtherInfo.h"
#include "B2EtherManager.h"
#include "B2EtherSetOptionInfo.h"
#include "BladeIIUtil.h"
#include "B2GameDefine.h"


void UB2UIEtherHelpSlot::Init()
{
	Super::Init();
}

void UB2UIEtherHelpSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIEtherHelpSlot::SetInfo(int32 InType)
{
	if (StaticFindEtherInfoTable != NULL)
	{
		if (X_IMG_SetImage.IsValid())
		{
			X_IMG_SetImage->SetBrushFromMaterial(StaticFindEtherInfoTable()->GetEtherIcon(InType));
		}

		if (X_TB_SetName.IsValid())
		{
			X_TB_SetName->SetText(FText::FromString(StaticFindEtherInfoTable()->GetEtherTypeName(InType)));
		}
	}

	b2network::B2mdAetherSetOptionInfoPtr SetOptionPtr = B2EtherManager::GetInstance().GetEtherSetOptionInfoPtrByType(InType, 1); //tier는 1로 활용
	if (SetOptionPtr != nullptr)
	{
		if (StaticFindEtherSetOptionInfoTable() != NULL)
		{
			if (X_RTB_SetOptionName.IsValid())
			{
				X_RTB_SetOptionName->SetText(StaticFindEtherSetOptionInfoTable()->GetEtherSetOptionName(SetOptionPtr->set_option_id));
				
			}

			if (X_TB_SetOptionDesc.IsValid())
			{
				X_TB_SetOptionDesc->SetText(StaticFindEtherSetOptionInfoTable()->GetEtherSetOptionDesc(SetOptionPtr->set_option_id));
			}
		}

		if (SetOptionPtr->min_apply_count == EtherSetCount::DEFENCE) //방어 세트
		{
			X_RTB_SetOption->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetName_2_Skin"))));
		}
		else if (SetOptionPtr->min_apply_count == EtherSetCount::OFFENCE) // 공격 세트
		{
			X_RTB_SetOption->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetName_1_Skin"))));
		}
	}
}

void UB2UIEtherHelpSlot::CacheAssets()
{
	GET_SLOT(UB2Image, X_IMG_SetImage);
	GET_SLOT(UTextBlock, X_TB_SetName);
	GET_SLOT(UB2RichTextBlock, X_RTB_SetOptionName);
	GET_SLOT(UTextBlock, X_TB_SetOptionDesc);
	GET_SLOT(UB2RichTextBlock, X_RTB_SetOption);
}
