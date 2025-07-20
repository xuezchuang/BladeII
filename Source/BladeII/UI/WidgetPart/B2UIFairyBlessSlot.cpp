// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIFairyBlessSlot.h"
#include "FairyManager.h"
#include "B2UIManager.h"
#include "B2Inventory.h"
#include "TutorialManager.h"

void UB2UIFairyBlessSlot::Init()
{
	Super::Init();
	bEnabledClick = false;
	CurrentSlotStatus = EFairyBlessSlotStatus::Disabled;
	bIsBless = false;
}

void UB2UIFairyBlessSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIFairyBlessSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UPanelWidget, P_Disable);
	GET_SLOT(UPanelWidget, P_Able);
	GET_SLOT(UPanelWidget, P_Selected);
	GET_SLOT(UImage, IMG_LockBG);
	GET_SLOT(UImage, IMG_BelssIcon);

	GET_SLOT(UOverlay, OV_Detail);

	GET_SLOT(UB2Button, BTN_BelssSelect);

	GET_SLOT(UTextBlock, TB_Bless);
	GET_SLOT(UB2RichTextBlock, RTB_Count);
	GET_SLOT(UB2RichTextBlock, RTB_Desc);
	GET_SLOT(UTextBlock, TB_FairyOpen);

	GET_SLOT(UPanelWidget, OV_Lock);
	GET_SLOT(UPanelWidget, OV_Check); 

	GET_SLOT(UPanelWidget, OV_FXBlessing);
	GET_SLOT(UPanelWidget, OV_FXBlessON);

	if (TB_Bless.IsValid())
		TB_Bless->SetText(FText());
	if (RTB_Count.IsValid())
		RTB_Count->SetText(FText());
	if (RTB_Desc.IsValid())
		RTB_Desc->SetText(FText());
	if (TB_FairyOpen.IsValid())
		TB_FairyOpen->SetText(FText());
	if (OV_Lock.IsValid())
		OV_Lock->SetVisibility(ESlateVisibility::Hidden);
	if (OV_Check.IsValid())
		OV_Check->SetVisibility(ESlateVisibility::Hidden);
	if (IMG_BelssIcon.IsValid())
		IMG_BelssIcon->SetVisibility(ESlateVisibility::Hidden);
	if (OV_FXBlessing.IsValid())
		OV_FXBlessing->SetVisibility(ESlateVisibility::Hidden);
	if (OV_FXBlessON.IsValid())
		OV_FXBlessON->SetVisibility(ESlateVisibility::Hidden);
	if(OV_Detail.IsValid())
		OV_Detail->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIFairyBlessSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_BelssSelect, &UB2UIFairyBlessSlot::OnClickBTN_BlessSlot);
}

void UB2UIFairyBlessSlot::UpdateStaticText()
{
	if (TB_Bless.IsValid())
		TB_Bless->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_Blessing")));
}

void UB2UIFairyBlessSlot::SetEnabledStatus()
{
	if (P_Able.IsValid())
		P_Able->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (P_Disable.IsValid())
		P_Disable->SetVisibility(ESlateVisibility::Hidden);
	if (P_Selected.IsValid())
		P_Selected->SetVisibility(bIsBless ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	if (OV_Detail.IsValid())
		OV_Detail->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIFairyBlessSlot::SetDisabledStatus()
{
	if (P_Able.IsValid())
		P_Able->SetVisibility(ESlateVisibility::Hidden);
	if (P_Disable.IsValid())
		P_Disable->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (P_Selected.IsValid())
		P_Selected->SetVisibility(bIsBless ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	if (OV_Detail.IsValid())
		OV_Detail->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIFairyBlessSlot::SetSelectedStatus()
{
	if (P_Able.IsValid())
		P_Able->SetVisibility(ESlateVisibility::Hidden);
	if (P_Disable.IsValid())
		P_Disable->SetVisibility(ESlateVisibility::Hidden);
	if (P_Selected.IsValid())
		P_Selected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (OV_Detail.IsValid())
		OV_Detail->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIFairyBlessSlot::SetSlotStaus(EFairyBlessSlotStatus::Type Status)
{
	CurrentSlotStatus = Status;
	switch (Status)
	{
	case EFairyBlessSlotStatus::Type::Enabled:
		SetEnabledStatus();
		break;
	case EFairyBlessSlotStatus::Type::Disabled:
		SetDisabledStatus();
		break;
	case EFairyBlessSlotStatus::Type::Selected:
		SetSelectedStatus();
		break;
	default:
		break;
	}
}

void UB2UIFairyBlessSlot::SetBlessSlotInfo(int32 RowIndex, const FB2FairyBlessSlotInfo &Info)
{
	bEnabledClick = false;
	CachedRowIndex = RowIndex;

	bEnabledClick = Info.bOpendBless;

	SetSlotStaus(Info.bOpendBless ? EFairyBlessSlotStatus::Type::Enabled : EFairyBlessSlotStatus::Disabled);

	ESlateVisibility BlessVisibility = Info.bEquiped ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;
	ESlateVisibility FairyOpenVisibility = Info.bOpendBless == false ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;
	ESlateVisibility LockVisibility = Info.bOpendBless ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	ESlateVisibility CheckVisibility = Info.bEquiped ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
	ESlateVisibility BlessingEffectVisibility = Info.bEquiped ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	
	bIsBless = Info.bEquiped;

	if (TB_Bless.IsValid())
		TB_Bless->SetVisibility(BlessVisibility);

	if (TB_FairyOpen.IsValid())
	{
		TB_FairyOpen->SetVisibility(FairyOpenVisibility);
		TB_FairyOpen->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessOpenLevel")), FText::AsNumber(Info.OpenLevel)));
	}
	if (OV_Lock.IsValid())
		OV_Lock->SetVisibility(LockVisibility);
	if (OV_Check.IsValid())
		OV_Check->SetVisibility(CheckVisibility);
	if (OV_FXBlessing.IsValid())
		OV_FXBlessing->SetVisibility(BlessingEffectVisibility);
	if (P_Selected.IsValid())
		P_Selected->SetVisibility(BlessingEffectVisibility);

	const FFairyBelssInfo* BlessInfo = FairyManager::GetInstance().GetFairyBlessInfo(Info.FairyType, Info.OptionId);
	if (BlessInfo)
	{
		if (IMG_BelssIcon.IsValid())
		{
			IMG_BelssIcon->SetVisibility(ESlateVisibility::Visible);
			IMG_BelssIcon->SetBrushFromTexture(BlessInfo->GetFairySkillIconTexture());
		}

		if (IMG_LockBG.IsValid())
		{
			IMG_LockBG->SetBrushFromTexture(BlessInfo->GetFairySkillIconTexture());
		}
	}

	if (RTB_Desc.IsValid())
	{
		FText OptionName = FairyManager::GetInstance().GetBlessOptionName(Info.OptionId);
		RTB_Desc->SetText(OptionName);
	}

	if (RTB_Count.IsValid())
	{
		if (Info.bOpendBless)
		{
			EItemOption CurrOption = SvrToCliOptionType(Info.OptionId);

			FText OptionValue = Info.bOpendBless ? GetItemOptionValueDisplayText(CurrOption, Info.OptionValue, true) : FText();
			RTB_Count->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessStat1")), OptionValue));
		}
		else
		{
			RTB_Count->SetText(FText::GetEmpty());
		}
	}
}

void UB2UIFairyBlessSlot::SetFairyBlessSlotHandler(const FFairyBlessSlotOnClick& InBtnHandler)
{
	OnBlessSlotClickHandler = InBtnHandler;
}

void UB2UIFairyBlessSlot::SetFairyBlessIconHandler(const FFairyBlessIconOnClick& InBtnhandler)
{
	OnBlessIconClickHandler = InBtnhandler;
}

void UB2UIFairyBlessSlot::OnClickBTN_BlessSlot()
{
	if (bEnabledClick == false)
		return;

	if (CurrentSlotStatus == EFairyBlessSlotStatus::Selected)
	{
		OnBlessIconClickHandler.ExecuteIfBound(CachedRowIndex); 
	}
	else
	{
		OnBlessSlotClickHandler.ExecuteIfBound(CachedRowIndex);
	}
}

void UB2UIFairyBlessSlot::StartBlessAnimation()
{
	PlayBlessAnimation();
}

void UB2UIFairyBlessSlot::OnFinishedBlessAnimation()
{

}