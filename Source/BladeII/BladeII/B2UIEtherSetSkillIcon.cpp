//// Fill out your copyright notice in the Description page of Project Settings.
//
//
#include "B2UIEtherSetSkillIcon.h"
//#include "BladeIIUtil.h"
//#include "B2UIDocHelper.h"
//#include "B2EtherInfo.h"
//
//
//
//
//void UB2UIEtherSetSkillIcon::Init()
//{
//	Super::Init();
//
//	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
//	{
//		if (IMG_Skill.IsValid() && StaticFindEtherInfoTable())
//		{
//			int32 SetType = EtherSetType == EEtherSetType::Offense ? DocBattle->GetOffenseSetID() : DocBattle->GetDefenseSetID();
//			IMG_Skill->SetBrushFromMaterial(StaticFindEtherInfoTable()->GetEtherIcon(SetType));
//		}
//	}
//
//	if (IMG_CoolTime.IsValid())
//		TagCooltimeDisplayMID = IMG_CoolTime->GetDynamicMaterial();
//
//	if (TagCooltimeDisplayMID)
//		TagCooltimeDisplayMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, 1.0f);
//}
//
//void UB2UIEtherSetSkillIcon::DestroySelf(class UB2UIManager* InUIManager)
//{
//	Super::DestroySelf(InUIManager);
//}
//
//void UB2UIEtherSetSkillIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//	Super::NativeTick(MyGeometry, InDeltaTime);
//}
//
//void UB2UIEtherSetSkillIcon::CacheAssets()
//{
//	Super::CacheAssets();
//
//	GET_SLOT(UImage, IMG_CoolTime);
//	GET_SLOT(UImage, IMG_Skill);
//}
//
//void UB2UIEtherSetSkillIcon::BindDelegates()
//{
//	Super::BindDelegates();
//}
//
//void UB2UIEtherSetSkillIcon::SubscribeEvents()
//{
//
//}
//void UB2UIEtherSetSkillIcon::UnsubscribeEvents()
//{
//
//}
//
//void UB2UIEtherSetSkillIcon::SetCoolTime(float MaxTime, float CurrentTime, bool InEnable)
//{
//	if (IMG_Skill.IsValid() && IMG_Skill->GetIsEnabled() != InEnable)
//	{
//		if (InEnable && TagCooltimeDisplayMID) // End
//		{
//			TagCooltimeDisplayMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, 1.0f);
//		}
//
//		IMG_Skill->SetIsEnabled(InEnable);
//	}
//}
//
//void UB2UIEtherSetSkillIcon::UpdateCoolTime(float MaxTime, float CurrentTime, bool InEnable)
//{
//	SetCoolTime(MaxTime, CurrentTime, InEnable);
//
//	if (InEnable)
//		return;
//
//	if (TagCooltimeDisplayMID)
//		TagCooltimeDisplayMID->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, (MaxTime - CurrentTime) / (MaxTime));
//}