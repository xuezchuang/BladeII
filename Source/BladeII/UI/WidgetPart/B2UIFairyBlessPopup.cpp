// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIFairyBlessPopup.h"
#include "BladeIIGameImpl.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2AndroidBackManager.h"
#include "B2UIFairyLevelUpStatRow.h"
#include "FairyManager.h"
#include "B2Inventory.h"

void UB2UIFairyBlessPopup::Init()
{
	Super::Init(); 
}

void UB2UIFairyBlessPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIFairyBlessPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Notice);
	GET_SLOT(UB2RichTextBlock, TB_BelssName);
	GET_SLOT(UTextBlock, TB_NextInfo);

	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_BelssInfo);

	GET_SLOT(UImage, IMG_BelssIcon);

	GET_SLOT(UB2ScrollBox, ScrollBox);
}

void UB2UIFairyBlessPopup::UpdateStaticText()
{
	Super::UpdateStaticText();
	if (TB_Notice.IsValid())
		TB_Notice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_Bless")));
	if (TB_NextInfo.IsValid())
		TB_NextInfo->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_NextLevel")));
}

void UB2UIFairyBlessPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIFairyBlessPopup::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_BelssInfo, &UB2UIFairyBlessPopup::OnClickBTN_BlessIcon);
}

void UB2UIFairyBlessPopup::CloseWidgetDelegate()
{
	CloseSelf();
}

void UB2UIFairyBlessPopup::OnClickBTN_Close()
{
	CloseSelf();
}

void UB2UIFairyBlessPopup::OnClickBTN_BlessIcon()
{

}

void UB2UIFairyBlessPopup::SetPreviewStats(TArray<FB2FairyBlessRowInfo> InfoList)
{
	if (ScrollBox.IsValid() == false)
		return;

	ScrollBox->ClearChildren();

	for (FB2FairyBlessRowInfo &Each : InfoList)
	{
		UB2UIFairyLevelUpStatRow* ThisRow = CreateWidget<UB2UIFairyLevelUpStatRow>(GetOwningPlayer(), PartClass_StatRow);
		if (ThisRow) 
		{
			ThisRow->Init();
			ThisRow->SetupManualScrollBoxSender_byReceiver(this, ScrollBox.Get());

			ThisRow->SetContent(Each.ConditionLevel, Each.PreviewStat, Each.PreviewStatValue);

			UScrollBoxSlot* AddedRowSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(ThisRow));
		}
	}
}

void UB2UIFairyBlessPopup::SetActivatedBlessInfo(EFairyType FairyType, int32 OptionId)
{
	const FFairyBelssInfo* BlessInfo = FairyManager::GetInstance().GetFairyBlessInfo(FairyType, OptionId);
	if (BlessInfo)
	{
		if (IMG_BelssIcon.IsValid())
		{
			IMG_BelssIcon->SetBrushFromTexture(BlessInfo->GetFairySkillIconTexture());
		}
	}

	FairyManager &Instance = FairyManager::GetInstance();

	FB2FairyStatusInfo StatusInfo;
	if (Instance.GetFairyInfo(FairyType, StatusInfo))
	{
		FMD_FairyInfo FairyInfo;

		if (Instance.GetFairyOpendBlessInfo(FairyType, OptionId, StatusInfo.Level, FairyInfo))
		{
			EItemOption CurrOption = SvrToCliOptionType(FairyInfo.OptionId);

			FText OptionName;
			OptionName = GetLOCTextOfItemOption(CurrOption, EPCClass::EPC_End);
			FText OptionValue = GetItemOptionValueDisplayText(CurrOption, FairyInfo.OptionValue, true);

			FText StatText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessStat")), OptionName, OptionValue);

			if (TB_BelssName.IsValid())
				TB_BelssName->SetText(StatText);
		}
	}
}

void UB2UIFairyBlessPopup::SetBlessInfo(EFairyType FairyType, int32 OptionId)
{
	TArray<FMD_FairyInfo> OutList;
	FairyManager::GetInstance().GetFairyBlessInfoAll(FairyType, OptionId, OutList);

	TArray<FB2FairyBlessRowInfo> RowList;

	for (FMD_FairyInfo &Each : OutList)
	{
		EItemOption CurrOption = SvrToCliOptionType(Each.OptionId);

		FText OptionName;
		OptionName = GetLOCTextOfItemOption(CurrOption, EPCClass::EPC_End);

		FText OptionValue = GetItemOptionValueDisplayText(CurrOption, Each.OptionValue, true);

		FB2FairyBlessRowInfo RowInfo;
		RowInfo.PreviewStat = OptionName;
		RowInfo.ConditionLevel = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessOpenLevel")), FText::AsNumber(Each.Level));
		RowInfo.PreviewStatValue = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessStat1")), OptionValue);

		RowList.Add(RowInfo);
	}

	SetPreviewStats(RowList);
	SetActivatedBlessInfo(FairyType, OptionId);
}