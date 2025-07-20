// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRepeatBattleDetail.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattleStage.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIRepeatBattleDetail::Init()
{
	Super::Init();

	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		if (BladeIIGameImpl::GetStageDataStore().IsRepeatBattleLocked(DocBattleStage->GetServerStageId()))
		{
			DocBattleStage->SetRepeatBattleLoopOne(false);
			DocBattleStage->SetRepeatBattleBoostOn(false);
			CB_Sub_Detail_1->SetIsEnabled(false);
			CB_Sub_Detail_3->SetIsEnabled(false);
		}
		CB_Sub_Detail_2->SetIsEnabled(false);

		CB_Sub_Detail_1->SetCheckedState(DocBattleStage->GetRepeatBattleLoopOne()? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		CB_Sub_Detail_2->SetCheckedState(DocBattleStage->GetRepeatBattleLoopAll() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		CB_Sub_Detail_3->SetCheckedState(DocBattleStage->GetRepeatBattleBoostOn() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		
	}
	UpdateStaticText();
}

void UB2UIRepeatBattleDetail::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_Ok);
	GET_SLOT(UCheckBox, CB_Sub_Detail_1);
	GET_SLOT(UCheckBox, CB_Sub_Detail_2);
	GET_SLOT(UCheckBox, CB_Sub_Detail_3);
	GET_SLOT(UTextBlock, TB_RepeatBattleBoostCount);
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_Detail);
	GET_SLOT(UB2RichTextBlock, TB_Sub_Detail_1);
	GET_SLOT(UB2RichTextBlock, TB_Sub_Detail_2);
	GET_SLOT(UB2RichTextBlock, TB_Sub_Detail_3);
	GET_SLOT(UTextBlock, TB_Sub_Title_1);
	GET_SLOT(UTextBlock, TB_Sub_Title_2);
	GET_SLOT(UTextBlock, TB_Sub_Title_3);
}

void UB2UIRepeatBattleDetail::UpdateStaticText()
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleDetail_004"))));
	}
	if (TB_Detail.IsValid())
	{
		TB_Detail->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleDetail_001"))));
	}
	if (TB_Sub_Detail_1.IsValid())
	{
		TB_Sub_Detail_1->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleDetail_009"))));
	}
	if (TB_Sub_Detail_2.IsValid())
	{
		TB_Sub_Detail_2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleDetail_002"))));
	}
	if (TB_Sub_Detail_3.IsValid())
	{
		TB_Sub_Detail_3->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleDetail_003"))));
	}
	if (TB_Sub_Title_1.IsValid())
	{
		TB_Sub_Title_1->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleDetail_005"))));
	}
	if (TB_Sub_Title_2.IsValid())
	{
		TB_Sub_Title_2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleDetail_006"))));
	}
	if (TB_Sub_Title_3.IsValid())
	{
		TB_Sub_Title_3->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleDetail_007"))));
	}
	if (TB_RepeatBattleBoostCount.IsValid())
	{
		auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
		if (DocBattleStage)
		{
			TB_RepeatBattleBoostCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleDetail_008"))),
				DocBattleStage->GetRepeatBattleBoostRemainCount(), BladeIIGameImpl::GetClientDataStore().GetMaxBoostCount()));
		}
	}
}

void UB2UIRepeatBattleDetail::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIRepeatBattleDetail::OnClickBtnClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_Ok, &UB2UIRepeatBattleDetail::OnClickBtnOk);
	CB_Sub_Detail_1->OnCheckStateChanged.AddDynamic(this, &UB2UIRepeatBattleDetail::CheckStateChanged_1);
	CB_Sub_Detail_2->OnCheckStateChanged.AddDynamic(this, &UB2UIRepeatBattleDetail::CheckStateChanged_2);
	CB_Sub_Detail_3->OnCheckStateChanged.AddDynamic(this, &UB2UIRepeatBattleDetail::CheckStateChanged_3);
}

void UB2UIRepeatBattleDetail::DestroySelf(class UB2UIManager* InUIManager)
{
}

void UB2UIRepeatBattleDetail::OnClickBtnOk()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::BattleRepeatDetail);
}

void UB2UIRepeatBattleDetail::OnClickBtnClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::BattleRepeatDetail);
}

void UB2UIRepeatBattleDetail::CheckStateChanged_1(bool bCheck)
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		if (bCheck)
		{
			CB_Sub_Detail_1->SetCheckedState(ECheckBoxState::Checked);
			CB_Sub_Detail_2->SetCheckedState(ECheckBoxState::Unchecked);
			DocBattleStage->SetRepeatBattleLoopOne(true);
			DocBattleStage->SetRepeatBattleLoopAll(false);
		}
		else
		{
			CB_Sub_Detail_1->SetCheckedState(ECheckBoxState::Unchecked);
			DocBattleStage->SetRepeatBattleLoopOne(false);
		}
	}
}
void UB2UIRepeatBattleDetail::CheckStateChanged_2(bool bCheck)
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		if (bCheck)
		{
			CB_Sub_Detail_1->SetCheckedState(ECheckBoxState::Unchecked);
			CB_Sub_Detail_2->SetCheckedState(ECheckBoxState::Checked);
			DocBattleStage->SetRepeatBattleLoopOne(false);
			DocBattleStage->SetRepeatBattleLoopAll(true);
		}
		else
		{
			CB_Sub_Detail_2->SetCheckedState(ECheckBoxState::Unchecked);
			DocBattleStage->SetRepeatBattleLoopAll(false);
		}
	}
}
void UB2UIRepeatBattleDetail::CheckStateChanged_3(bool bCheck)
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		if (bCheck)
		{
			CB_Sub_Detail_3->SetCheckedState(ECheckBoxState::Checked);
			DocBattleStage->SetRepeatBattleBoostOn(true);
		}
		else
		{
			CB_Sub_Detail_3->SetCheckedState(ECheckBoxState::Unchecked);
			DocBattleStage->SetRepeatBattleBoostOn(false);
		}
	}
}