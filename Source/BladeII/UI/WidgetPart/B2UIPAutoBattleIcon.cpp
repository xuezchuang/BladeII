// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPAutoBattleIcon.h"
#include "B2UIDocHelper.h"
#include "BladeIIUtil.h"
#include "B2UIRepeatBattleCountDown.h"
#include "B2StageGameMode.h"
#include "../../Common/Event.h"


void UB2UIPAutoBattleIcon::Init()
{
	Super::Init();
	
	UB2UIDocBattle* DocBS = UB2UIDocHelper::GetDocBattle();
	if (DocBS)
	{
		IconAutoBattleHandler = DocBS->OnInAutoPlayChanged.AddUObject(this, &UB2UIPAutoBattleIcon::OnChangedIconAutoBattleType);
	}
}

void UB2UIPAutoBattleIcon::DestroySelf(class UB2UIManager* InUIManager)
{
	UB2UIDocBattle* DocBS = UB2UIDocHelper::GetDocBattle();
	if (DocBS && IconAutoBattleHandler.IsValid())
	{
		DocBS->OnInAutoPlayChanged.Remove(IconAutoBattleHandler);
		IconAutoBattleHandler.Reset();
	}
	Super::DestroySelf(InUIManager);
}

void UB2UIPAutoBattleIcon::OnChangedIconAutoBattleType(class UB2UIDocBase* Sender, int32 bCurrentOn, int32 bPrevOn)
{
	SetIconAutoBattleType(bCurrentOn);
}

void UB2UIPAutoBattleIcon::SetIconAutoBattleType(int32 InIconAutoBattleType)
{
	if (IconAutoBattleType != InIconAutoBattleType)
	{
		IconAutoBattleType = InIconAutoBattleType;

		if (BTN_AutoBattle.IsValid())
		{
			switch (IconAutoBattleType)
			{
			case AutoBattleType::NONE:
				BTN_AutoBattle->SetStyle(AutoOff);
				break;
			case AutoBattleType::Attack:
				BTN_AutoBattle->SetStyle(AttackAutoOn);
				break;
			case AutoBattleType::Skill:
				BTN_AutoBattle->SetStyle(SkillAutoOn);
				break;
			default:
				BTN_AutoBattle->SetStyle(AutoOff);
				break;
			}
		}
	}
}

void UB2UIPAutoBattleIcon::SetIsStageMode(bool Value)
{
	IsStageMode = Value;
	if (IsStageMode == true)
	{
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
		bool bRepeatBattleOn = DocBS ? DocBS->GetRepeatBattleCurrentOn() : false;

		if (bRepeatBattleOn)
		{
			SetPlayerAutoTypeClass<int32>::GetInstance().Signal(AutoBattleType::Skill);
		}
	}
}

void UB2UIPAutoBattleIcon::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_AutoBattle);
}

void UB2UIPAutoBattleIcon::BindDelegates()
{
	BIND_PRESS_FUNC_TO_BTN(BTN_AutoBattle, &UB2UIPAutoBattleIcon::OnClickAutoBattle)
}

void UB2UIPAutoBattleIcon::OnClickAutoBattle()
{
	if (IsStageMode == true)
	{
		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();

		bool bRepeatBattleOn = DocBS ? DocBS->GetRepeatBattleCurrentOn() : false;

		if (bRepeatBattleOn)
		{
			PopUpRepeatBattleStopMsg(GetOwningPlayer()); // 馆汗 傈捧 惑怕俊辑绰 吝窜 滚瓢狼 开且捞 凳.
			return;
		}
	}

	TogglePlayerAutoStateClass<>::GetInstance().Signal();
}
