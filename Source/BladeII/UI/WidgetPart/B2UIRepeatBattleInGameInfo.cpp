#include "B2UIRepeatBattleInGameInfo.h"
#include "B2UIRepeatBattleCountDown.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2AndroidBackManager.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

UB2UIRepeatBattleInGameInfo::UB2UIRepeatBattleInGameInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIRepeatBattleInGameInfo::Init()
{
	Super::Init();

	IsBackPopup = false;

	if (m_DocBattleStage.IsValid() == false)
	{
		auto* DocBattle = UB2UIDocHelper::GetDocBattleStage();
		BII_CHECK(DocBattle);
		m_DocBattleStage = TWeakObjectPtr<UB2UIDocBattleStage>(DocBattle);
	}

	BindDocAuto(); // 실제 UIDocBindable 인터페이스 대로 동작하는 건 아니므로 이건 직접 불러줌. 걍 모양새만 내는 거

	if (GetB2GameModeType(this) == EB2GameMode::Scenario)
	{
		UpdateDynamicWidgets();		// 처음 한번은 필요하겠지.
	}
}

void UB2UIRepeatBattleInGameInfo::DestroySelf(UB2UIManager* InUIManager)
{
	if (IsBackPopup)
	{
		IsBackPopup = false;
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
	Super::DestroySelf(InUIManager);
}

void UB2UIRepeatBattleInGameInfo::BindDocAuto()
{
	if (m_DocBattleStage.IsValid())
	{
		m_DocBattleStage->OnRepeatBattleCurrentOnChanged.AddUObject(this, &UB2UIRepeatBattleInGameInfo::OnChangedRepeatBattleCurrentOn);
	}
}

void UB2UIRepeatBattleInGameInfo::UnbindDoc()
{
	if (m_DocBattleStage.IsValid())
	{
		m_DocBattleStage->OnRepeatBattleCurrentOnChanged.RemoveAll(this);
	}
}

void UB2UIRepeatBattleInGameInfo::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_RepeatBattleOnMessage);
	GET_SLOT(UTextBlock, TB_RepeatBattleCount);
	GET_SLOT(UTextBlock, TB_RepeatBattleStopBtn);
	GET_SLOT(UB2Button, BTN_RepeatBattleStop);

	GET_SLOT(UPanelWidget, CV_RepeatBattleInfo);
	GET_SLOT(UPanelWidget, CV_3TimesInfo);
	GET_SLOT(UTextBlock, TB_RepeatBattleCount_Boost);
	GET_SLOT(UTextBlock, TB_RepeatBattleOnMessage_Boost);
	GET_SLOT(UImage, IMG_BoostFull);
}

void UB2UIRepeatBattleInGameInfo::UpdateStaticText()
{
	if (TB_RepeatBattleOnMessage.IsValid()){
		TB_RepeatBattleOnMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_RepeatBattleIsOn")));
	}
	if (TB_RepeatBattleOnMessage_Boost.IsValid()) {
		TB_RepeatBattleOnMessage_Boost->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("B2UIRepeatBattleInGameInfo_001")));
	}
	if (TB_RepeatBattleStopBtn.IsValid()){
		TB_RepeatBattleStopBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stop")));
	}
	if (TB_RepeatBattleStopBtn.IsValid()) {
		TB_RepeatBattleStopBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stop")));
	}
}

void UB2UIRepeatBattleInGameInfo::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_RepeatBattleStop, &UB2UIRepeatBattleInGameInfo::OnClickRepeatBattleStopBtn)
}

void UB2UIRepeatBattleInGameInfo::UpdateDynamicWidgets()
{
	if (GetB2GameModeType(this) == EB2GameMode::Scenario && m_DocBattleStage.IsValid())
	{
		// 숨기는 거 자체 처리
		if (m_DocBattleStage->GetRepeatBattleCurrentOn())
		{
			this->SetVisibility(ESlateVisibility::Visible);
			BTN_RepeatBattleStop->SetVisibility(ESlateVisibility::Visible);
		}
		else if (m_DocBattleStage->GetRepeatBattleBoostOn())
		{
			this->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			this->SetVisibility(ESlateVisibility::Collapsed);
		}

		auto UIManager = UB2UIManager::GetInstance();
		if (CV_3TimesInfo.IsValid())
		{
			if (UIManager && (EUIScene::StageClear == UIManager->GetCurrUIScene() || EUIScene::StageResult == UIManager->GetCurrUIScene()))
			{
				if(m_DocBattleStage->GetRepeatBattleBoostOn() && m_DocBattleStage->GetRepeatBattleCurrentOn())		//반복전투 3배모험이 다켜져 있을경우는 결과화면에서도 다보여달라고 함...
					CV_3TimesInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
				else
					CV_3TimesInfo->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				CV_3TimesInfo->SetVisibility(m_DocBattleStage->GetRepeatBattleBoostOn() ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
			}
		}
		
		if (CV_RepeatBattleInfo.IsValid()) 
		{
			if (m_DocBattleStage->GetRepeatBattleCurrentOn() && IsBackPopup == false)
			{
				B2AndroidBackManager::GetInstance()->AddBackWidget(this);
				IsBackPopup = true;
			}
			CV_RepeatBattleInfo->SetVisibility(m_DocBattleStage->GetRepeatBattleCurrentOn() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}

		if (TB_RepeatBattleOnMessage.IsValid())
		{
			if(m_DocBattleStage->GetRepeatBattleLoopAll())
			{
				TB_RepeatBattleOnMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("B2UIRepeatBattleCountDown_002")));
			}
			else if (m_DocBattleStage->GetRepeatBattleLoopOne())
			{
				TB_RepeatBattleOnMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("B2UIRepeatBattleCountDown_001")));
			}
		}
		if (TB_RepeatBattleCount.IsValid())
		{
			TB_RepeatBattleCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_RepeatCurrentCount")),
				m_DocBattleStage->GetRepeatBattleCurrentCount() + 1));

			TB_RepeatBattleCount->SetVisibility(m_DocBattleStage->GetRepeatBattleLoopAll() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		}

		if (TB_RepeatBattleCount_Boost.IsValid())
		{	
			int32 BoostCount = FMath::Min(m_DocBattleStage->GetRepeatBattleBoostMaxCount(), m_DocBattleStage->GetRepeatBattleBoostRemainCount());
			TB_RepeatBattleCount_Boost->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("CounterDG_RewardNum"))), BoostCount));
		}

		if (IMG_BoostFull.IsValid())
		{
			bool IsMaxCount = m_DocBattleStage->GetRepeatBattleBoostRemainCount() > m_DocBattleStage->GetRepeatBattleBoostMaxCount();
			IMG_BoostFull->SetVisibility(IsMaxCount ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIRepeatBattleInGameInfo::OnClickRepeatBattleStopBtn()
{
	PopUpRepeatBattleStopMsg(GetOwningPlayer());
}

void UB2UIRepeatBattleInGameInfo::OnChangedRepeatBattleCurrentOn(class UB2UIDocBase* Sender, bool bCurrentOn, bool bPrevOn)
{
	UpdateDynamicWidgets();
}