// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2WingUIP_PropOptionIcon.h"
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
		// 원래 열려있을 때만 이걸 켜는 거였는데 잠긴 이미지를 이 위에 씌우는 식으로 바뀜. 이건 항상 on.
		IMG_Icon->SetVisibility(/*CachedOptionData.bIsOpen ? */ESlateVisibility::HitTestInvisible /*: ESlateVisibility::Collapsed*/);

		// 잠겨있는 아이콘도 해당 옵션에 맞게 출력되게끔
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
		// 날개 메인 UI 에서는 속성 옵션이 Open 되지 않은 경우라도 클릭 가능하도록 해서 속성옵션 강화창이 열리도록 한다.
		// 기타 다른 경우는 클릭할 곳 없을 듯..
		BTN_Area->SetVisibility(
			(MyUsageType == EWingPropOptionUIPUsage::WPOUU_WingPageMain /*&& CachedOptionData.bIsOpen*/) ? 
			ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UB2WingUIP_PropOptionIcon::OnClickBtnArea()
{
	if (MyUsageType == EWingPropOptionUIPUsage::WPOUU_WingPageMain /*&& CachedOptionData.bIsOpen*/) // 이 조건이 아닐 때 아예 click 이 되지 말아야..
	{// 속성옵션 강화 메뉴 오픈.
		DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_WingPropOptionEnhance, FDJLegacySubpopupAddPanelInfo());
	}
}

bool UB2WingUIP_PropOptionIcon::RedDotCondition_Upgrade()
{
	return UB2Airport::HasAnyNewStuff_WingPropOption(WingOwnerClass, WingOptionPropIndex);
}
