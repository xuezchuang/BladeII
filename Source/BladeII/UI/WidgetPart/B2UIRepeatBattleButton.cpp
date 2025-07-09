// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIRepeatBattleButton.h"
#include "B2UIRepeatBattleDetail.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattleStage.h"
#include "Event.h"
#include "B2ClientDataStore.h"
#include "Retailer.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIRepeatBattleButton::Init()
{
	Super::Init();

	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		DocBattleStage->OnRepeatBattleLoopOneChanged.AddUObject(this, &UB2UIRepeatBattleButton::RepeatBattleLoopOneChanged);
		DocBattleStage->OnRepeatBattleLoopAllChanged.AddUObject(this, &UB2UIRepeatBattleButton::RepeatBattleLoopAllChanged);
		DocBattleStage->OnRepeatBattleBoostOnChanged.AddUObject(this, &UB2UIRepeatBattleButton::RepeatBattleBoostOnChanged);
		
		if (IMG_RB_Lock_Icon.IsValid())
		{ // 처음 lock 이 풀렸을 때 연출이 들어갈 수 있음.. 만일 연출 시 이 이미지를 사용한다면 사용하는 쪽에서 켜도록..
			IMG_RB_Lock_Icon->SetVisibility(/*BladeIIGameImpl::GetStageDataStore().IsRepeatBattleLocked(DocBattleStage->GetServerStageId()) ? ESlateVisibility::HitTestInvisible : */ESlateVisibility::Collapsed);
		}
		BTN_RepeatSetting->SetIsEnabled(true/*!BladeIIGameImpl::GetStageDataStore().IsRepeatBattleLocked(DocBattleStage->GetServerStageId())*/);
		RepeatBattleChanged();
	}
	UpdateStaticText();
}

void UB2UIRepeatBattleButton::RepeatBattleChanged()
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		if (DocBattleStage->GetRepeatBattleLoopOne() || DocBattleStage->GetRepeatBattleLoopAll() || DocBattleStage->GetRepeatBattleBoostOn() )
		{
			CV_REPEAT_OPTION->SetVisibility(ESlateVisibility::Visible);
			BTN_RepeatSetting->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			CV_REPEAT_OPTION->SetVisibility(ESlateVisibility::Hidden);
			BTN_RepeatSetting->SetVisibility(ESlateVisibility::Visible);
		}

		if (CV_LoopOne.IsValid())
		{
			CV_LoopOne->SetVisibility(DocBattleStage->GetRepeatBattleLoopOne() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
		if (CV_LoopAll.IsValid())
		{
			CV_LoopAll->SetVisibility(DocBattleStage->GetRepeatBattleLoopAll() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
		if (CV_3Times.IsValid())
		{
			CV_3Times->SetVisibility(DocBattleStage->GetRepeatBattleBoostOn() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIRepeatBattleButton::RepeatBattleLoopOneChanged(class UB2UIDocBase* Sender, bool newBool, bool oldBool) {
	if (CV_LoopOne.IsValid())
	{
		if (newBool)
			CV_LoopOne->SetVisibility(ESlateVisibility::Visible);
		else
			CV_LoopOne->SetVisibility(ESlateVisibility::Collapsed);
	}
	RepeatBattleChanged();
}

void UB2UIRepeatBattleButton::RepeatBattleLoopAllChanged(class UB2UIDocBase* Sender, bool newBool, bool oldBool) {
	if (CV_LoopAll.IsValid())
	{
		if (newBool)
			CV_LoopAll->SetVisibility(ESlateVisibility::Visible);
		else
			CV_LoopAll->SetVisibility(ESlateVisibility::Collapsed);
	}
	RepeatBattleChanged();
}

void UB2UIRepeatBattleButton::RepeatBattleBoostOnChanged(class UB2UIDocBase* Sender, bool newFactor, bool oldFactor) {
	if (CV_3Times.IsValid())
	{
		if (newFactor)
			CV_3Times->SetVisibility(ESlateVisibility::Visible);
		else
			CV_3Times->SetVisibility(ESlateVisibility::Collapsed);
	}
	RepeatBattleChanged();
}

void UB2UIRepeatBattleButton::CacheAssets()
{
	GET_SLOT(UImage, IMG_RB_Lock_Icon);
	GET_SLOT(UB2Button, BTN_RepeatStart);
	GET_SLOT(UB2Button, BTN_RepeatSetting);
	GET_SLOT(UCanvasPanel, CV_REPEAT_OPTION);
	GET_SLOT(UOverlay, CV_LoopOne);
	GET_SLOT(UOverlay, CV_LoopAll);
	GET_SLOT(UOverlay, CV_3Times);
	GET_SLOT(UTextBlock, TB_3Times);
	GET_SLOT(UTextBlock, TB_RepeatStart);
	GET_SLOT(UTextBlock, TB_RepeatSetting);
	GET_SLOT(UTextBlock, TB_3Times_Title);
	GET_SLOT(UTextBlock, TB_LoopOne);
	GET_SLOT(UTextBlock, TB_LoopAll);
}

void UB2UIRepeatBattleButton::UpdateStaticText()
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		if (TB_3Times.IsValid())
		{
			TB_3Times->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleButton_003"))),
				DocBattleStage->GetRepeatBattleBoostRemainCount(), BladeIIGameImpl::GetClientDataStore().GetMaxBoostCount()));
		}
	}
	if (TB_RepeatStart.IsValid())
	{
		TB_RepeatStart->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleButton_004"))));
	}
	if (TB_RepeatSetting.IsValid())
	{
		TB_RepeatSetting->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleButton_006"))));
	}
	if (TB_3Times_Title.IsValid())
	{
		TB_3Times_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleButton_002"))));
	}
	if (TB_LoopOne.IsValid())
	{
		TB_LoopOne->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleButton_001"))));
	}
	if (TB_LoopAll.IsValid())
	{
		TB_LoopAll->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("B2UIRepeatBattleButton_005"))));
	}
}

void UB2UIRepeatBattleButton::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_RepeatStart, &UB2UIRepeatBattleButton::OnClickBtnRepeatStart);
	BIND_CLICK_FUNC_TO_BTN(BTN_RepeatSetting, &UB2UIRepeatBattleButton::OnClickBtnRepeatSetting);
}

void UB2UIRepeatBattleButton::DestroySelf(class UB2UIManager* InUIManager)
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::BattleRepeatDetail);
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		DocBattleStage->OnRepeatBattleLoopOneChanged.RemoveAll(this);
		DocBattleStage->OnRepeatBattleLoopAllChanged.RemoveAll(this);
		DocBattleStage->OnRepeatBattleBoostOnChanged.RemoveAll(this);
	}
}

void UB2UIRepeatBattleButton::OnClickBtnRepeatStart()
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	//if (DocBattleStage && !BladeIIGameImpl::GetStageDataStore().IsRepeatBattleLocked(DocBattleStage->GetServerStageId()))
	{
		UB2UIManager::GetInstance()->OpenUI<UB2UIRepeatBattleDetail>(UIFName::BattleRepeatDetail);
	}
	/*
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	if (DocBattleStage)
	{
		int32 ClientStageId = DocBattleStage->GetClientStageId();
		EStageDifficulty StageDifficulty = DocBattleStage->GetStageDifficulty();
		StartRepeatBattleStageClass<int32, EStageDifficulty>::GetInstance().Signal(ClientStageId, StageDifficulty);
	}
	*/
}

void UB2UIRepeatBattleButton::OnClickBtnRepeatSetting()
{
	auto DocBattleStage = UB2UIDocHelper::GetDocBattleStage();
	//if (DocBattleStage && !BladeIIGameImpl::GetStageDataStore().IsRepeatBattleLocked(DocBattleStage->GetServerStageId()))
	{
		UB2UIManager::GetInstance()->OpenUI<UB2UIRepeatBattleDetail>(UIFName::BattleRepeatDetail);
	}
}