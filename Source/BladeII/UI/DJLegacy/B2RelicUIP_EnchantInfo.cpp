// Fill out your copyright notice in the Description page of Project Settings.
#include "B2RelicUIP_EnchantInfo.h"
//#include "BladeII.h"

#include "Retailer.h"
#include "B2LobbyUI_RelicMain.h"
#include "B2UIManager.h"


#include "BladeIIGameImpl.h"

void UB2RelicUIP_EnchantInfo::CacheAssets()
{
	Super::CacheAssets();
	//
	GET_SLOT(UTextBlock, TB_RelicName);
	GET_SLOT(UTextBlock, TB_RelicName_Close);
	GET_SLOT(UImage, IMG_RelicIcon);
	GET_SLOT(UTextBlock, STB_RelicOption);
	GET_SLOT(UB2Button, BTN_Relic);
	GET_SLOT(UPanelWidget, P_Disable);
	GET_SLOT(UPanelWidget, P_Able);

	GET_SLOT(UProgressBar, PB_Count);
	GET_SLOT(UTextBlock, TB_Count);

	GET_SLOT(UB2LobbyUI_RelicStarItem, GradeWidget);
}

void UB2RelicUIP_EnchantInfo::Init()
{
	Super::Init();
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(this), &UB2RelicUIP_EnchantInfo::RedDotCondition_RelicItem);
}

void UB2RelicUIP_EnchantInfo::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Relic, &UB2RelicUIP_EnchantInfo::OnClicked_RelicButton);
}

void UB2RelicUIP_EnchantInfo::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2RelicUIP_EnchantInfo::InitRelicOptionData(int RelicID, int32 ClientStageID, EPCClass ePC)
{

	if (RelicID == 0) return;

	if (RelicIconsArray.IsValidIndex(RelicID - 1) && IMG_RelicIcon.IsValid())
	{
		IMG_RelicIcon->SetBrushFromMaterial(RelicIconsArray[RelicID - 1]);
	}

	if (TB_RelicName.IsValid() && TB_RelicName_Close.IsValid())
	{
		TB_RelicName->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, "Relic_Type" + FString::FromInt(RelicID)));
		TB_RelicName_Close->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, "Relic_Type" + FString::FromInt(RelicID)));
	}

	if (STB_RelicOption.IsValid())
	{
		int32 nMapID = (int32)ClientStageID / 10;
		int32 nStageID = ClientStageID % 10 == 0 ? 10 : ClientStageID % 10;

		FText OpenName = FText::Format(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, TEXT("Relic_NormalComplete_Obtain")), FText::FromString(FString::Printf(TEXT("%d-%d"), nMapID, nStageID)));
		STB_RelicOption->SetText(OpenName);
	}
	CurrentPCClass = ePC;
	CurrentRelicInfo = FAncientRelic(RelicID, 0, 0);
}

void UB2RelicUIP_EnchantInfo::OnClicked_RelicButton()
{
	ButtonDelegate.ExecuteIfBound(CurrentRelicInfo);
}

void UB2RelicUIP_EnchantInfo::UpdateRelicData(FAncientRelic RelicInfo, EPCClass ePC)
{
	if (STB_RelicOption.IsValid())
		STB_RelicOption->SetVisibility(ESlateVisibility::Hidden);

	if (GradeWidget.IsValid())
	{
		int32 Grade = RelicInfo.nRelicGrade;
		GradeWidget->SetStarGrade(Grade);
	}

	CurrentRelicInfo = RelicInfo;

	if (P_Able.IsValid() && P_Disable.IsValid())
	{
		P_Able->SetVisibility(ESlateVisibility::HitTestInvisible);
		P_Disable->SetVisibility(ESlateVisibility::Collapsed);
		GradeWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (PB_Count.IsValid())
	{
		float fPercent = float(RelicInfo.nRelicLevel) / 10.0f;
		PB_Count->SetPercent(fPercent);
	}
	if (TB_Count.IsValid())
	{
		FString CountText = FString::FromInt(RelicInfo.nRelicLevel) + "/ 10";
		TB_Count->SetText(FText::FromString(CountText));
	}
	
	if(BTN_Relic.IsValid())
		BTN_Relic->SetIsEnabled(true);

	CurrentPCClass = ePC;
}

void UB2RelicUIP_EnchantInfo::ClearRelicData()
{
	if (P_Able.IsValid() && P_Disable.IsValid())
	{
		P_Able->SetVisibility(ESlateVisibility::Collapsed);
		P_Disable->SetVisibility(ESlateVisibility::HitTestInvisible);
		GradeWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (BTN_Relic.IsValid())
		BTN_Relic->SetIsEnabled(false);
}

bool UB2RelicUIP_EnchantInfo::RedDotCondition_RelicItem()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Relic(CurrentPCClass,CurrentRelicInfo.nRelicId);
}