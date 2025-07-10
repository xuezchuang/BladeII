// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIFormationPopup.h"
#include "B2UIDocHelper.h"

#include "B2ItemInfo.h"
#include "B2UISlotItem.h"
#include "Retailer.h"
#include "BladeIILocText.h"
#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "B2UIUpgradeStatNotify.h"
#include "B2UIManager.h"

#define SET_ABILITY_SLOT(Index); \
GET_SLOT_BYNAME(UHorizontalBox, AbilitySlots[Index].HB_Box, X_HB_Slot##Index); \
GET_SLOT_BYNAME(UB2RichTextBlock, AbilitySlots[Index].RTB_CurAbil, X_RTB_CurrentAbility##Index); \
GET_SLOT_BYNAME(UTextBlock, AbilitySlots[Index].TB_NextAbil, X_TB_NextAbility##Index);	\
GET_SLOT_BYNAME(UImage, AbilitySlots[Index].IMG_Arrow, IMG_Arrow##Index);

void UB2UIFormationPopup::CacheAssets()
{
	GET_SLOT(UButton, BTN_Cancle);
	GET_SLOT(UB2Button, BTN_Confirm);
	GET_SLOT(UTextBlock, TB_EnhanceCost);
	GET_SLOT(UTextBlock, TB_Lv);
	GET_SLOT(UTextBlock, TB_Formation);
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_Descrition);
	GET_SLOT(UTextBlock, TB_Confirm);
	GET_SLOT(UTextBlock, TB_Cancle);
	GET_SLOT(UTextBlock, TB_MaxLevel);
	GET_SLOT(UImage, IMG_Formation);
	GET_SLOT(UB2UISlotItem, UIP_EnhanceCost);
	AbilitySlots.SetNum(MAX_SLOT);
	SET_ABILITY_SLOT(0);
	SET_ABILITY_SLOT(1);
	SET_ABILITY_SLOT(2);

	Anim_Enhance = GetAnimation(FName("Anim_FX_Enhance"));
	Anim_Background = GetAnimation(FName("Anim_FX_Background"));
}

void UB2UIFormationPopup::BindDelegates()
{
	//if(BTN_Cancle.IsValid())
	//{
	//	BIND_CLICK_FUNC_TO_BTN(BTN_Cancle, &UB2UIFormationPopup::CloseSelf);
	//}

	//if (BTN_Confirm.IsValid())
	//{
	//	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIFormationPopup::OnClickedConfirm);
	//}

	//EnhanceFormationEventID = DeliveryEnhanceTeamFormationClass<FB2EnhanceTeamFormation>::GetInstance().Subscribe([this](const FB2EnhanceTeamFormation& EnhanceResult)
	//{
	//	this->NextFormationInfo = GET_TUPLE_DATA(FB2ResponseEnhanceTeamFormation::formation_index, EnhanceResult);
	//	this->PlaySuccessAnim();
	//});
}

void UB2UIFormationPopup::UpdateStaticText()
{
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TeamFormationBuff_Upgrade")));
	}

	if (TB_Descrition.IsValid())
	{
		TB_Descrition->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Teamformation_UpgradeQuestion"))); //진형을 강화하시겠습니까?
	}

	if (TB_Confirm.IsValid())
	{
		TB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Teamformation_Upgrade")));
	}

	if (TB_Cancle.IsValid())
	{
		TB_Cancle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Teamformation_Cancel"))); //취소
	}
}

void UB2UIFormationPopup::SetAbility(ETMFormation InFormation, int32 InLevel)
{
	if (TB_Lv.IsValid())
	{
		TB_Lv->SetText(FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Stat_Level_Abbrev"))), FText::AsNumber(InLevel)));
	}

	if (UIP_EnhanceCost.IsValid())
	{
		if (auto* StageInfoTable = StaticFindItemInfo())
		{
			if (auto* ItemData = StageInfoTable->GetInfoData(FItemRefIDHelper::GetGoodsID_Gold()))
			{
				int32 NeedMoney = BladeIIGameImpl::GetClientDataStore().GetUpgradeFormationCost(InFormation, InLevel);
				UIP_EnhanceCost->BindItemInfoData(ItemData, 0);
				UIP_EnhanceCost->SetCount(NeedMoney);

				bool bNotEnoughMoney = NeedMoney > BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
				UIP_EnhanceCost->SetItemCountTextColor(bNotEnoughMoney ? UB2UIManager::GetInstance()->TextColor_NonSatisfy : FLinearColor::White);
			}
		}
	}

	IncreaseDatas.Empty();

	for (int i = 0; i < MAX_SLOT; ++i)
	{
		if (AbilitySlots.IsValidIndex(i))
		{
			IncreaseData data = AbilitySlots[i].SetInfo(FormationType, InLevel, i + 1);
			if (data.AttackIncrease != 0 || data.DefenceIncrease != 0)
			{
				IncreaseDatas.Add(data);
			}

			if (InLevel == BladeIIGameImpl::GetClientDataStore().GetMaxLevelTeamFormation(FormationType))
			{
				AbilitySlots[i].MaxLevelSetting();
			}
		}
	}
}

void UB2UIFormationPopup::OnClickedConfirm()
{
	data_trader::Retailer::GetInstance().RequestEnhanceTeamBattleFormation(FormationType);
	BTN_Confirm->SetIsEnabled(false);
}

void UB2UIFormationPopup::SetData(ETMFormation InFormation, int32 InLevel)
{
	FormationType = InFormation;
	int MaterialIdx = static_cast<int32>(FormationType);

	FText FormationName;
	switch (FormationType)
	{
	case ETMFormation::ETM_Normal:
		FormationName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch_FormationNormal"));
		break;
	case ETMFormation::ETM_Attack:
		FormationName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch_FormationAtt"));
		break;
	case ETMFormation::ETM_Defense:
		FormationName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch_FormationDef"));
		break;
	}

	if (IMG_Formation.IsValid() && FormationIconArr.IsValidIndex(MaterialIdx))
	{
		IMG_Formation->SetBrushFromMaterial(FormationIconArr[MaterialIdx]);
	}

	if (TB_Formation.IsValid())
	{
		TB_Formation->SetText(FormationName);
	}

	SetAbility(FormationType, InLevel);

	if (InLevel == BladeIIGameImpl::GetClientDataStore().GetMaxLevelTeamFormation(FormationType))
	{
		if(TB_MaxLevel.IsValid())
			TB_MaxLevel->SetVisibility(ESlateVisibility::HitTestInvisible);
		if(BTN_Confirm.IsValid())
			BTN_Confirm->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIFormationPopup::EndSuccessAnim_BP()
{
	BTN_Confirm->SetIsEnabled(true);
}

void UB2UIFormationPopup::PlaySuccessAnim()
{
	PlayAnimation(Anim_Enhance);
	PlayAnimation(Anim_Background);

	if (UB2UIUpgradeStatNotify* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIUpgradeStatNotify>(UIFName::UpgradeStatNotify))
	{
		pUI->ClearList();
		for (const IncreaseData& Elem : IncreaseDatas)
		{
			if (Elem.AttackIncrease != 0 && Elem.DefenceIncrease == 0)
			{
				FText ValueName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIO_Offense_IncDamage"));
				pUI->AddListItem(ValueName, FText::AsNumber(Elem.AttackIncrease), true);
			}
			else if (Elem.AttackIncrease == 0 && Elem.DefenceIncrease != 0)
			{
				FText ValueName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIO_Defense_DecDamage"));
				pUI->AddListItem(ValueName, FText::AsNumber(Elem.DefenceIncrease), true);
			}
			else if (Elem.AttackIncrease != 0 && Elem.DefenceIncrease != 0)
			{
				FText ValueName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TeamMatch_EnhanceNotice"));
				pUI->AddListItem(ValueName, FText::Format(FText::FromString(TEXT("{0}%\n{1}%")), FText::AsNumber(Elem.AttackIncrease), FText::AsNumber(Elem.DefenceIncrease)), true);
			}
		}
	}

	if (NextFormationInfo != nullptr)
	{
		SetAbility(SvrToCliFormationType(NextFormationInfo->formation_type), NextFormationInfo->formation_level);
		if (NextFormationInfo->formation_level >= BladeIIGameImpl::GetClientDataStore().GetMaxLevelTeamFormation(FormationType))
		{
			if (TB_MaxLevel.IsValid())
				TB_MaxLevel->SetVisibility(ESlateVisibility::HitTestInvisible);
			if (BTN_Confirm.IsValid())
				BTN_Confirm->SetVisibility(ESlateVisibility::Collapsed);
		}

		NextFormationInfo = nullptr;
	}
}

void UB2UIFormationPopup::Init()
{
	Super::Init();
	NextFormationInfo = nullptr;
}

void UB2UIFormationPopup::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	if (UIP_EnhanceCost.IsValid())
	{
		UIP_EnhanceCost->DestroySelf(InUIManager);
	}

	DeliveryEnhanceTeamFormationClass<FB2EnhanceTeamFormation>::GetInstance().Unsubscribe(EnhanceFormationEventID);
}

void UB2UIFormationPopup::CloseWidgetDelegate()
{
	CloseSelf();
}

void FAbilitySlot::MaxLevelSetting()
{
	if (TB_NextAbil.IsValid())
	{
		TB_NextAbil->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IMG_Arrow.IsValid())
	{
		IMG_Arrow->SetVisibility(ESlateVisibility::Collapsed);
	}
}

IncreaseData FAbilitySlot::SetInfo(ETMFormation InFormation, int32 InLevel, int32 InIndex)
{
	IncreaseData Result;
	auto& DataStore = BladeIIGameImpl::GetClientDataStore();
	float CurAB = DataStore.GetAttackDamageBonusByTeamFormation(InFormation, InLevel, InIndex);
	float CurDB = DataStore.GetReduceDamageBonusByTeamFormation(InFormation, InLevel, InIndex);
	
	int32 NextLevel = InLevel + 1;
	float NextAB = DataStore.GetAttackDamageBonusByTeamFormation(InFormation, NextLevel, InIndex);
	float NextDB = DataStore.GetReduceDamageBonusByTeamFormation(InFormation, NextLevel, InIndex);

	if (CurAB == 0 && CurDB == 0)
	{
		if (HB_Box.IsValid())
		{
			HB_Box->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		if (HB_Box.IsValid())
		{
			HB_Box->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (RTB_CurAbil.IsValid() && TB_NextAbil.IsValid())
		{
			if (CurAB == 0 && NextAB == 0)
			{
				RTB_CurAbil->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TMBuff_Defensive")), FText::AsNumber(CurDB)));
				TB_NextAbil->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_ResultContributionPercent")), FText::AsNumber(NextDB)));
				Result.DefenceIncrease = NextDB - CurDB;
				Result.AttackIncrease = 0;
			}
			else if (CurDB == 0 && NextDB == 0)
			{
				RTB_CurAbil->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TMBuff_Offensive")), FText::AsNumber(CurAB)));
				TB_NextAbil->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_ResultContributionPercent")), FText::AsNumber(NextAB)));
				Result.DefenceIncrease = 0;
				Result.AttackIncrease = NextAB - CurAB;
			}
			else
			{
				RTB_CurAbil->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TMBuff_BothPopup")), FText::AsNumber(CurAB), FText::AsNumber(CurDB)));
				TB_NextAbil->SetText(FText::Format(FText::FromString(TEXT("{0}%\n{1}%")), FText::AsNumber(NextAB), FText::AsNumber(NextDB)));
				Result.DefenceIncrease = NextDB - CurDB;
				Result.AttackIncrease = NextAB - CurAB;
			}
		}
	}
	return Result;
}
