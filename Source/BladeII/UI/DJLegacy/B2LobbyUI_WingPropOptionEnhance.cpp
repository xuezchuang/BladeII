// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_WingPropOptionEnhance.h"
//#include "BladeII.h"

#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2ItemInfo.h"
#include "B2LobbyInventory.h"


#include "BladeIIGameImpl.h"

UB2LobbyUI_WingPropOptionEnhance::UB2LobbyUI_WingPropOptionEnhance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedAirport = NULL;

	PropOptionDetailSlotPrefix = TEXT("PropOptionSlot");
}

void UB2LobbyUI_WingPropOptionEnhance::CacheAssets()
{
	Super::CacheAssets();

	PropOptionDetailSlots.Empty();
	for (int32 POPI = 1; POPI <= MAX_WING_PROP_OPTION_NUM; ++POPI)
	{// 순서대로 찾아서 넣는다.
		const FString& ThisPartName = FString::Printf(TEXT("%s_%d"), *PropOptionDetailSlotPrefix, POPI);
		UB2WingUIP_PropOptionDetail* ThisFoundElem = GetSlot<UB2WingUIP_PropOptionDetail>(FName(*ThisPartName));
		if (ThisFoundElem)
		{
			PropOptionDetailSlots.Add(ThisFoundElem);
		}
	}
		
	GET_SLOT(UB2Button, BTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_WingPropOptionEnhance::OnClickCloseBtn);

	GET_SLOT(UTextBlock, TB_PropOptionEnhanceTitle);
	GET_SLOT(UTextBlock, TB_PropOptionEnhanceNote);
	GET_SLOT(UTextBlock, TB_AngelFeatherLabel);
	GET_SLOT(UTextBlock, TB_AngelFeatherCount);
	GET_SLOT(UImage, IMG_AngelFeather);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_Goods);

	if (BTN_Goods.IsValid())
	{
		BTN_Goods->SetItemInfo(FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID());
	}
}

void UB2LobbyUI_WingPropOptionEnhance::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
		
	CachedAirport = CachedLobbyGM->GetLobbyAirport();
	check(CachedAirport);

	UpdateStaticWidgets();
	UpdateByCurrentWingData();
}

void UB2LobbyUI_WingPropOptionEnhance::DestroySelf()
{
	Super::DestroySelf();

}

void UB2LobbyUI_WingPropOptionEnhance::UpdateStaticWidgets()
{
	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	FSingleItemInfoData* AngelFeatherData = ItemInfoTable ? ItemInfoTable->GetInfoData(FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID()) : NULL;

	if (TB_PropOptionEnhanceTitle.IsValid())
	{
		TB_PropOptionEnhanceTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_PropOptionEnhance")));
	}
	if (TB_PropOptionEnhanceNote.IsValid())
	{
		TB_PropOptionEnhanceNote->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_PropOptionEnhance_Note")));
	}
	if (TB_AngelFeatherLabel.IsValid() && AngelFeatherData)
	{
		TB_AngelFeatherLabel->SetText(AngelFeatherData->GetLocalizedName());
	}
	if (IMG_AngelFeather.IsValid() && AngelFeatherData)
	{
		IMG_AngelFeather->SetBrushFromMaterial(AngelFeatherData->GetIconMaterial(ItemInfoTable));
	}
}

void UB2LobbyUI_WingPropOptionEnhance::UpdateByCurrentWingData()
{
	EPCClass HeroMgmtPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing WingData;
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(HeroMgmtPCClass, WingData);

	if (bGotValidWingData)
	{
		if (TB_AngelFeatherCount.IsValid())
		{
			TB_AngelFeatherCount->SetText(
				FText::AsNumber(UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID()))
				);
		}

		for (int32 POI = 0; POI < PropOptionDetailSlots.Num(); ++POI)
		{
			const FWingPropOption* PropOptionThisIndex = WingData.GetPropOptionPtr(POI);
			UB2WingUIP_PropOptionDetail* ThisDetailSlot = PropOptionDetailSlots[POI];
			if (PropOptionThisIndex && ThisDetailSlot)
			{
				ThisDetailSlot->SetMyOptionInfo(POI, WingData, *PropOptionThisIndex);
			}
		}
	}
}

void UB2LobbyUI_WingPropOptionEnhance::CloseWidgetDelegate()
{
	OnClickCloseBtn();
}

void UB2LobbyUI_WingPropOptionEnhance::OnClickCloseBtn()
{
	CloseMe();
}

void UB2LobbyUI_WingPropOptionEnhance::CloseMe()
{
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_WingPropOptionEnhance);
}
