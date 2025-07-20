// Fill out your copyright notice in the Description page of Project Settings.
#include "B2WingUIP_PropOptionIcon.h"
//#include "BladeII.h"

#include "B2Airport.h"
#include "B2WingInfo.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "Event.h"

UB2WingUIP_PropOptionIcon::UB2WingUIP_PropOptionIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyUsageType = EWingPropOptionUIPUsage::WPOUU_PropOptionEnhance;
	WingOwnerClass = EPCClass::EPC_End;
	WingOptionPropIndex = INDEX_NONE;
}

void UB2WingUIP_PropOptionIcon::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UImage, IMG_Icon);
	GET_SLOT(UImage, IMG_Icon_Locked);
	GET_SLOT(UImage, IMG_Locked);
	GET_SLOT(UTextBlock, TB_PropOptionLevel);

	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2WingUIP_PropOptionIcon::OnClickBtnArea)
}

void UB2WingUIP_PropOptionIcon::SetUsageType(EWingPropOptionUIPUsage InUsageType)
{
	MyUsageType = InUsageType;

	UpdateWidgets();
}

void UB2WingUIP_PropOptionIcon::SetMyOptionInfo(const FWingPropOption& InOptionData)
{
	CachedOptionData = InOptionData;

	UpdateWidgets();
}

void UB2WingUIP_PropOptionIcon::SetWingPropExtraInfo(EPCClass OwnerClass, int32 PropIndex)
{
	WingOwnerClass = OwnerClass;
	WingOptionPropIndex = PropIndex;

	DoMarkRedDot();
}

void UB2WingUIP_PropOptionIcon::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(), &UB2WingUIP_PropOptionIcon::RedDotCondition_Upgrade);
}

void UB2WingUIP_PropOptionIcon::UpdateWidgets()
{
	if (IMG_Icon.IsValid())
	{
		// 盔贰 凯妨乐阑 锭父 捞吧 难绰 芭看绰单 泪变 捞固瘤甫 捞 困俊 竞快绰 侥栏肺 官柴. 捞扒 亲惑 on.
		IMG_Icon->SetVisibility(/*CachedOptionData.bIsOpen ? */ESlateVisibility::HitTestInvisible /*: ESlateVisibility::Collapsed*/);

		// 泪败乐绰 酒捞能档 秦寸 可记俊 嘎霸 免仿登霸阐
		//if (CachedOptionData.bIsOpen)
		{
			UB2WingInfo* WingInfoTable = StaticFindWingInfo();
			FWingPropOptionIconSetup* ThisOptionIconSetup = WingInfoTable ? WingInfoTable->GetPropOptionIconSetup(CachedOptionData.MyOptionType) : NULL;
			if (ThisOptionIconSetup)
			{
				IMG_Icon->SetBrushFromMaterial(ThisOptionIconSetup->GetIconMtrl());
			}
		}
	}
	if (IMG_Locked.IsValid())
	{
		IMG_Locked->SetVisibility(CachedOptionData.bIsOpen ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}
	if (IMG_Icon_Locked.IsValid())
	{
		IMG_Icon_Locked->SetVisibility(CachedOptionData.bIsOpen ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}

	if (TB_PropOptionLevel.IsValid())
	{
		bool bShowLevelText = (MyUsageType == EWingPropOptionUIPUsage::WPOUU_WingPageMain || MyUsageType == EWingPropOptionUIPUsage::WPOUU_CollectBookPreview);
		if (bShowLevelText && CachedOptionData.bIsOpen)
		{
			TB_PropOptionLevel->SetText(
				FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(CachedOptionData.OptionLevel))
				);

			TB_PropOptionLevel->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			TB_PropOptionLevel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (BTN_Area.IsValid())
	{
		// 朝俺 皋牢 UI 俊辑绰 加己 可记捞 Open 登瘤 臼篮 版快扼档 努腐 啊瓷窍档废 秦辑 加己可记 碍拳芒捞 凯府档废 茄促.
		// 扁鸥 促弗 版快绰 努腐且 镑 绝阑 淀..
		BTN_Area->SetVisibility(
			(MyUsageType == EWingPropOptionUIPUsage::WPOUU_WingPageMain /*&& CachedOptionData.bIsOpen*/) ? 
			ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UB2WingUIP_PropOptionIcon::OnClickBtnArea()
{
	if (MyUsageType == EWingPropOptionUIPUsage::WPOUU_WingPageMain /*&& CachedOptionData.bIsOpen*/) // 捞 炼扒捞 酒匆 锭 酒抗 click 捞 登瘤 富酒具..
	{// 加己可记 碍拳 皋春 坷锹.
		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_WingPropOptionEnhance, FDJLegacySubpopupAddPanelInfo());
	}
}

bool UB2WingUIP_PropOptionIcon::RedDotCondition_Upgrade()
{
	return UB2Airport::HasAnyNewStuff_WingPropOption(WingOwnerClass, WingOptionPropIndex);
}
