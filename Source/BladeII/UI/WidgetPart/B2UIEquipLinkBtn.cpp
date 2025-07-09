// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIEquipLinkBtn.h"

#include "BladeIIGameImpl.h"
#include "TutorialManager.h"
#include "BladeIIUtil.h"

void UB2UIEquipLinkBtn::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_LinkEquip);
	GET_SLOT(UB2RichTextBlock, RTB_LinkEquipBtn);
	
	GET_SLOT(UPanelWidget, X_Parent);
}

void UB2UIEquipLinkBtn::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_LinkEquip, &UB2UIEquipLinkBtn::OnClickBTN_Equip);
}

void UB2UIEquipLinkBtn::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (RTB_LinkEquipBtn.IsValid())
	{
		RTB_LinkEquipBtn->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_EquipMgmtButton")));
	}
}

void UB2UIEquipLinkBtn::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(X_Parent.Get()), &UB2UIEquipLinkBtn::RedDotCondition_Equip);
}

void UB2UIEquipLinkBtn::OnClickBTN_Equip()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
}

bool UB2UIEquipLinkBtn::RedDotCondition_Equip()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_Inventory();
}

void UB2UIEquipLinkBtn::InitEquipLinkBtn_BP()
{
	this->Init();

	bool IsEnable = TutorialManager::GetInstance().IsLockTutorial(OpenConditionTutorialID, 0) == false;

	if (BTN_LinkEquip.IsValid())
	{
		BTN_LinkEquip->SetIsEnabled(IsEnable);
	}
}
