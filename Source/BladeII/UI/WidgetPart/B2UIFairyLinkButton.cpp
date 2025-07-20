// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIFairyLinkButton.h"
#include "BladeIIGameImpl.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2UILinkManager.h"
#include "TutorialManager.h"

void UB2UIFairyLinkButton::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_LinkFairy);
	GET_SLOT(UTextBlock, STB_LinkFairy);

	GET_SLOT(UPanelWidget, X_Parent);

	bool ConditionFairy = TutorialManager::GetInstance().IsLockTutorial(OpenConditionTutorialID, 0) == false;

	if (BTN_LinkFairy.IsValid())
		BTN_LinkFairy->SetIsEnabled(ConditionFairy);
}

void UB2UIFairyLinkButton::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_LinkFairy, &UB2UIFairyLinkButton::OnClickBTN_Fairy);
}

void UB2UIFairyLinkButton::Init()
{
	Super::Init();

}

void UB2UIFairyLinkButton::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIFairyLinkButton::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (STB_LinkFairy.IsValid())
		STB_LinkFairy->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_Fairy")));
}

void UB2UIFairyLinkButton::OnClickBTN_Fairy()
{
	FB2UILinkManager::LinkUIScene(EUIScene::FairyMain);
}

bool UB2UIFairyLinkButton::RedDotCondition_Fairy()
{
	return BladeIIGameImpl::GetRedDotManager().HasNotApplyBlessFairy();
}

void UB2UIFairyLinkButton::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(X_Parent.Get()), &UB2UIFairyLinkButton::RedDotCondition_Fairy);
}