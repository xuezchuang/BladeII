// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIMailRewardPopUp.h"
#include "B2PCClassInfoBox.h"
#include "B2SkillInfo.h"
#include "B2UIUnitySkillRewardPopup.h"


#include "BladeIIGameImpl.h"

void UB2UIUnitySkillRewardPopup::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, STB_UnityRewardSuccess);
	GET_SLOT(UTextBlock, TB_RewardMessage);
	GET_SLOT(UB2Image, IMG_SkillIcon);
	GET_SLOT(UTextBlock, TB_SkillName);
	GET_SLOT(UB2Button, BTN_Confirm);

	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIUnitySkillRewardPopup::OnClickBTN_Modal);

	Anim_MailPopUp = GetAnimation(this, "Anim_MailPopUp");
}

void UB2UIUnitySkillRewardPopup::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIUnitySkillRewardPopup::Init()
{
	Super::Init();
	CacheAssets();
}

void UB2UIUnitySkillRewardPopup::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIUnitySkillRewardPopup::CloseWidgetDelegate()
{
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIUnitySkillRewardPopup::SetUnitySkillImage(int32 nUnitySkillIndex)
{
	auto* ClassInfoBox = StaticFindPCClassInfoBox();
	auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;
	// LoadSynchronous 를 하려니 const 를 없애야..
	FSingleSkillInfo* SkillClientData = AllSkillInfo ? const_cast<FSingleSkillInfo*>(AllSkillInfo->GetSingleInfoOfID(nUnitySkillIndex)) : nullptr;

	if (IMG_SkillIcon.IsValid() && SkillClientData)
	{
		IMG_SkillIcon->SetBrushFromMaterial(Cast<UMaterialInstance>(SkillClientData->ButtonIcon_Normal.LoadSynchronous()));
	}

	if (TB_SkillName.IsValid())
	{
		TB_SkillName->SetText(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillName, nUnitySkillIndex)));
	}

	if (TB_RewardMessage.IsValid())
	{		
		TB_RewardMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_Awaken_Desc")));
	}
}

void UB2UIUnitySkillRewardPopup::FinishAnimation()
{
	//UB2UIManager::GetInstance()->OpenUI<UB2UIMailRewardPopUp>(UIFName::RewardMailPopup);
	//DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIUnitySkillRewardPopup::OnClickBTN_Modal()
{
	DestroySelf(UB2UIManager::GetInstance());
}



