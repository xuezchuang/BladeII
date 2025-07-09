// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIFairyLevelUpResultPopup.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "FairyManager.h"
#include "B2Inventory.h"

UB2UIFairyLevelUpResultPopup::UB2UIFairyLevelUpResultPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AutoCloseDalayTime = 3.0f;
}

void UB2UIFairyLevelUpResultPopup::Init()
{
	Super::Init();
}

void UB2UIFairyLevelUpResultPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIFairyLevelUpResultPopup::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);
}

void UB2UIFairyLevelUpResultPopup::OnClose(bool RightNow)
{
	Super::OnClose(RightNow);
}

void UB2UIFairyLevelUpResultPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_WholeArea);
	GET_SLOT(UPanelWidget, X_BlessUnlock);
	GET_SLOT(UPanelWidget, X_BlessLevelup);

	if (X_BlessUnlock.IsValid())
		X_BlessUnlock->SetVisibility(ESlateVisibility::Hidden);
	if (X_BlessLevelup.IsValid())
		X_BlessLevelup->SetVisibility(ESlateVisibility::Hidden);

	GET_SLOT(UTextBlock, TB_CharLevel); 
	GET_SLOT(UTextBlock, TB_CharLevel_Prev); 

	GET_SLOT(UImage, IMG_NPC);
	GET_SLOT(UImage, IMG_SkillIcon);
	GET_SLOT(UTextBlock, TB_SkillName);
	GET_SLOT(UTextBlock, TB_Unlock);
	GET_SLOT(UTextBlock, TB_LevelUpNotice);
	GET_SLOT(UB2RichTextBlock, RTB_SkillDesc); 
}

void UB2UIFairyLevelUpResultPopup::UpdateStaticText()
{
	if (TB_Unlock.IsValid())
		TB_Unlock->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessUnlockNotice")));
	if (TB_LevelUpNotice.IsValid())
		TB_LevelUpNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessEnhanceNotice")));
}

void UB2UIFairyLevelUpResultPopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_WholeArea, &UB2UIFairyLevelUpResultPopup::OnClickBTN_Close);
}

void UB2UIFairyLevelUpResultPopup::CloseWidgetDelegate()
{
	OnFinishedHandler.ExecuteIfBound();
	CloseSelf();
}

void UB2UIFairyLevelUpResultPopup::ShowResult(EFairyType FairyType, bool bIsLevelUp)
{
	bool bGetNewOption = false;
	FairyManager &Instance = FairyManager::GetInstance();
	FB2FairyStatusInfo StatusInfo;

	FText NewFairyInfoLevel;
	FText PrevFairyInfoLevel;

	FText SkillNameText;
	FText SkillDescText;

	if (Instance.GetFairyInfo(FairyType, StatusInfo))
	{
		FMD_FairyInfo PrevFairyInfo;
		FMD_FairyInfo NewFairyInfo;
		Instance.GetFairyTargetBlessInfo(StatusInfo.FairyType, StatusInfo.Level - 1, PrevFairyInfo);
		if (Instance.GetFairyTargetBlessInfo(StatusInfo.FairyType, StatusInfo.Level, NewFairyInfo))
		{
			NewFairyInfoLevel = FText::AsNumber(NewFairyInfo.Level);
			PrevFairyInfoLevel = FText::AsNumber(PrevFairyInfo.Level);

			bGetNewOption = PrevFairyInfo.OptionId != NewFairyInfo.OptionId;

			if (TB_SkillName.IsValid())
			{
				FText OptionName = FairyManager::GetInstance().GetBlessOptionName(NewFairyInfo.OptionId);
	
				EItemOption CurrOption = SvrToCliOptionType(NewFairyInfo.OptionId);
				FText OptionValue = GetItemOptionValueDisplayText(CurrOption, NewFairyInfo.OptionValue, true);

				SkillNameText = FText::Format(FText::FromString(TEXT("{0} + {1}")), OptionName, OptionValue);
				SkillDescText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Fairy_BlessStat")), OptionName, OptionValue);
			}

			const FFairyBelssInfo* BlessInfo = FairyManager::GetInstance().GetFairyBlessInfo(NewFairyInfo.FairyType, NewFairyInfo.OptionId);
			if (BlessInfo)
			{
				if (IMG_SkillIcon.IsValid())
				{
					IMG_SkillIcon->SetBrushFromTexture(BlessInfo->GetFairySkillIconTexture());
				}
			}
		}
	}

	if (TB_CharLevel.IsValid())
		TB_CharLevel->SetText(NewFairyInfoLevel);
	if (TB_CharLevel_Prev.IsValid())
		TB_CharLevel_Prev->SetText(PrevFairyInfoLevel);
	if (TB_SkillName.IsValid())
		TB_SkillName->SetText(SkillNameText);
	if (RTB_SkillDesc.IsValid())
		RTB_SkillDesc->SetText(SkillDescText);

	if (NPCImage.Contains(FairyType))
	{
		TSoftObjectPtr<UMaterialInterface> MIPtr = NPCImage.FindRef(FairyType);
		UMaterialInterface* Mat = MIPtr.LoadSynchronous();

		if (IMG_NPC.IsValid())
			IMG_NPC->SetBrushFromMaterial(Mat);
	}

	if (X_BlessUnlock.IsValid())
		X_BlessUnlock->SetVisibility(bGetNewOption ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (X_BlessLevelup.IsValid())
		X_BlessLevelup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlayLevelUpResult(bGetNewOption);
}

void UB2UIFairyLevelUpResultPopup::OnFinishedLevelUpAnimation()
{
	GetWorld()->GetTimerManager().SetTimer(DealyFinishedTH, this, &UB2UIFairyLevelUpResultPopup::OnClickBTN_Close, AutoCloseDalayTime, false);
}

void UB2UIFairyLevelUpResultPopup::OnClickBTN_Close()
{
	OnFinishedHandler.ExecuteIfBound();
	CloseSelf();
}

void UB2UIFairyLevelUpResultPopup::SetFinishedHandler(const FOnFairyLevelUpResultFinished& InFinishedHandler)
{
	OnFinishedHandler = InFinishedHandler;
}