// Fill out your copyright notice in the Description page of Project Settings.


#include "B2UI_TotemSmeltingPopup.h"

#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2UI_TotemSmeltingStatus.h"
#include "BladeIIGameImpl.h"
#include "B2AndroidBackManager.h"

const int32 iTOTEM_TAB_SMELTINGBYGOLD = 0;
const int32 iTOTEM_TAB_SMELTINGBYPIECE = 1;

void UB2UI_TotemSmeltingPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	if (UIP_TotemSmeltingStatusBP.IsValid())
	{
		UIP_TotemSmeltingStatusBP->DestroySelf(InUIManager);
	}

	UnsubscribeEvents();
	Super::DestroySelf(InUIManager);
}

void UB2UI_TotemSmeltingPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock,						TB_SmeltingTitle);
	GET_SLOT(UB2Button,							BTN_Close);

	GET_SLOT(UB2RichTextBlock,					RTB_TotemName);
	GET_SLOT(UImage,							IMG_TotemIcon);
	GET_SLOT(UB2RichTextBlock,					RTB_SuccessChance);
	GET_SLOT(UTextBlock,						TB_SmeltingLevel);

	GET_SLOT(UWidgetSwitcher,					WS_SmeltingByGold);
	GET_SLOT(UTextBlock,						TB_UnSelectedSmeltingByGold);
	GET_SLOT(UTextBlock,						TB_SelectedSmeltingByGold);
	GET_SLOT(UB2Button,							BTN_TabSmeltingByGold);

	GET_SLOT(UWidgetSwitcher,					WS_SmeltingByPiece);
	GET_SLOT(UTextBlock,						TB_UnSelectedSmeltingByPiece);
	GET_SLOT(UTextBlock,						TB_SelectedSmeltingByPiece);
	GET_SLOT(UB2Button,							BTN_TabSmeltingByPiece);

	GET_SLOT(UB2UI_TotemSmeltingStatus,			UIP_TotemSmeltingStatusBP);
	if (UIP_TotemSmeltingStatusBP.IsValid())
	{
		UIP_TotemSmeltingStatusBP->Init();
	}
}

void UB2UI_TotemSmeltingPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close,				&UB2UI_TotemSmeltingPopup::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_TabSmeltingByGold,	&UB2UI_TotemSmeltingPopup::OnClickBTN_TabSmeltingByGold);
	BIND_CLICK_FUNC_TO_BTN(BTN_TabSmeltingByPiece,	&UB2UI_TotemSmeltingPopup::OnClickBTN_TabSmeltingByPiece);
}

void UB2UI_TotemSmeltingPopup::UpdateStaticText()
{
	if (TB_SmeltingTitle.IsValid())
	{
		TB_SmeltingTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_smelt")));
	}
	if (TB_UnSelectedSmeltingByGold.IsValid())
	{
		TB_UnSelectedSmeltingByGold->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_smelt_Gold")));
	}
	if (TB_SelectedSmeltingByGold.IsValid())
	{
		TB_SelectedSmeltingByGold->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_smelt_Gold")));
	}
	if (TB_UnSelectedSmeltingByPiece.IsValid())
	{
		TB_UnSelectedSmeltingByPiece->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_smelt_DimensionPiece")));
	}
	if (TB_SelectedSmeltingByPiece.IsValid())
	{
		TB_SelectedSmeltingByPiece->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_smelt_DimensionPiece")));
	}
}

void UB2UI_TotemSmeltingPopup::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UI_TotemSmeltingPopup::InitTotemSmeltingPopup(const FB2Totem& InTotem)
{
	CachedB2TotemInfo = InTotem;

	SubscribeEvents();

	UpdateTotemSmeltingPopup(CachedB2TotemInfo);

	OnClickBTN_TabSmeltingByGold();

	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
}

void UB2UI_TotemSmeltingPopup::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(UpdateTotemSmeltingResultClass<bool, FB2Totem>::GetInstance().Subscribe2(
		[this](const bool IsSuccess, const FB2Totem ResultTotem)
	{
		this->CachedB2TotemInfo = ResultTotem;

		if (IsSuccess)
		{
			this->PlayAnimation_SmeltingSuccess();
		}
		else
		{
			this->PlayAnimation_SmeltingFail();
		}
	}
	));
}

void UB2UI_TotemSmeltingPopup::UnsubscribeEvents()
{
	Issues.Empty();
}

void UB2UI_TotemSmeltingPopup::UpdateTotemSmeltingPopup(const FB2Totem& InTotem)
{
	UpdateTotemIcon(InTotem);
	UpdateTotemName(InTotem);
	UpdateSmeltingLevel(InTotem);
	UpdateSmeltingSuccessChance(InTotem);

	if (UIP_TotemSmeltingStatusBP.IsValid())
	{
		UIP_TotemSmeltingStatusBP->UpdateTotemSmeltingStatus(CachedB2TotemInfo);
	}
}

void UB2UI_TotemSmeltingPopup::UpdateTotemIcon(const FB2Totem& InTotem)
{
	if (IMG_TotemIcon.IsValid() == false)
	{
		return;
	}

	if (InTotem.IsSmeltingComplete())
	{
		if (UB2ItemInfo* pAllItemInfo = StaticFindItemInfo())
		{
			if (FSingleItemInfoData* pItemInfo = pAllItemInfo->GetInfoData(InTotem.RefID))
			{
				if (pAllItemInfo == nullptr || pItemInfo == nullptr)
				{
					return;
				}

				IMG_TotemIcon->SetBrushFromMaterial(pItemInfo->GetIconMaterial(pAllItemInfo));
			}
		}
	}
	else
	{
		if (UB2ItemMiscInfo* pAllItemMiscInfo = StaticFindItemMiscInfo())
		{
			IMG_TotemIcon->SetBrushFromMaterial(pAllItemMiscInfo->GetTotemIconNotSmelted());
		}
	}
}

void UB2UI_TotemSmeltingPopup::UpdateTotemName(const FB2Totem& InTotem)
{
	if (RTB_TotemName.IsValid())
	{
		RTB_TotemName->SetText(BladeIIGameImpl::GetTotemDataStore().GetTotemName(InTotem, false));
	}
}

void UB2UI_TotemSmeltingPopup::UpdateSmeltingLevel(const FB2Totem& InTotem)
{
	if (TB_SmeltingLevel.IsValid())
	{
		FText SmeltingLevelFormat = FText::FromString(TEXT("+{0}"));
		TB_SmeltingLevel->SetText(FText::Format(SmeltingLevelFormat, InTotem.RefineLevel));
	}
}

void UB2UI_TotemSmeltingPopup::UpdateSmeltingSuccessChance(const FB2Totem& InTotem)
{
	switch (CurrentSelectedTab)
	{
	case iTOTEM_TAB_SMELTINGBYGOLD:
	{
		RTB_SuccessChance->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Smelt_probability")), (InTotem.GoldSuccessRate / 100)));
	}break;
	case iTOTEM_TAB_SMELTINGBYPIECE:
	{
		RTB_SuccessChance->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Totem_Smelt_probability")), (InTotem.PieceSuccessRate / 100)));
	}break;
	}
}

void UB2UI_TotemSmeltingPopup::UpdateTabBtnState(const int32& iTabIndex)
{
	if (WS_SmeltingByGold.IsValid() == false || WS_SmeltingByPiece.IsValid() == false)
	{
		return;
	}

	CurrentSelectedTab = iTabIndex;

	int32 iIsSelected = static_cast<int32>(false);

	WS_SmeltingByGold->SetActiveWidgetIndex(iIsSelected);
	WS_SmeltingByPiece->SetActiveWidgetIndex(iIsSelected);

	iIsSelected = static_cast<int32>(true);

	switch (iTabIndex)
	{
	case iTOTEM_TAB_SMELTINGBYGOLD:
		{
			WS_SmeltingByGold->SetActiveWidgetIndex(iIsSelected);
		}break;
	case iTOTEM_TAB_SMELTINGBYPIECE:
		{
			WS_SmeltingByPiece->SetActiveWidgetIndex(iIsSelected);
		}break;
	}
}

void UB2UI_TotemSmeltingPopup::OnFinishAnimationEvent_SmeltingSuccess()
{
	UpdateBySmeltingResult();
	PlaySound_SetSmeltingHoleSuccess();
}

void UB2UI_TotemSmeltingPopup::OnFinishAnimationEvent_SmeltingFail()
{
	UpdateBySmeltingResult();
	PlaySound_SetSmeltingHoleFail();
}

void UB2UI_TotemSmeltingPopup::UpdateBySmeltingResult()
{
	UpdateTotemSmeltingPopup(CachedB2TotemInfo);
	UpdateTotemInfoPopupControlClass<FB2Totem>::GetInstance().Signal(CachedB2TotemInfo);
	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();

	if (CurrentSelectedTab == iTOTEM_TAB_SMELTINGBYGOLD)
	{
		UIP_TotemSmeltingStatusBP->SetTotemSmeltingMaterial(ETotemRefineCostType::TRC_GOLD);
	}
	else if (CurrentSelectedTab == iTOTEM_TAB_SMELTINGBYPIECE)
	{
		UIP_TotemSmeltingStatusBP->SetTotemSmeltingMaterial(ETotemRefineCostType::TRC_DIMENTIONAL_PIECE);
	}

	if (CachedB2TotemInfo.IsSmeltingComplete())
	{
		LobbyReqSmeltingCompleteTotemClass<>::GetInstance().Signal();
	}
}

void UB2UI_TotemSmeltingPopup::OnClickBTN_Close()
{
	if (UIP_TotemSmeltingStatusBP.IsValid() == false)
	{
		return;
	}

	if (UIP_TotemSmeltingStatusBP->GetSmeltingButtonLockState() == true)
	{
		return;
	}

	QuitItemOpModeClass<bool>::GetInstance().Signal(false);

	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	UB2UIManager::GetInstance()->CloseUI(UIFName::TotemSmeltingPopup);
}

void UB2UI_TotemSmeltingPopup::OnClickBTN_TabSmeltingByGold()
{
	if (UIP_TotemSmeltingStatusBP.IsValid() == false)
	{
		return;
	}

	if (UIP_TotemSmeltingStatusBP->GetSmeltingButtonLockState() == true)
	{
		return;
	}

	UpdateTabBtnState(iTOTEM_TAB_SMELTINGBYGOLD);
	UIP_TotemSmeltingStatusBP->SetTotemSmeltingMaterial(ETotemRefineCostType::TRC_GOLD);
}

void UB2UI_TotemSmeltingPopup::OnClickBTN_TabSmeltingByPiece()
{
	if (UIP_TotemSmeltingStatusBP.IsValid() == false)
	{
		return;
	}

	if (UIP_TotemSmeltingStatusBP->GetSmeltingButtonLockState() == true)
	{
		return;
	}

	UpdateTabBtnState(iTOTEM_TAB_SMELTINGBYPIECE);
	UIP_TotemSmeltingStatusBP->SetTotemSmeltingMaterial(ETotemRefineCostType::TRC_DIMENTIONAL_PIECE);
}