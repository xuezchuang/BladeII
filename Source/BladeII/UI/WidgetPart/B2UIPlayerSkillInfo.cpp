#include "B2UIPlayerSkillInfo.h"
#include "B2SkillInfo.h"
#include "B2PCClassInfoBox.h"
#include "Event.h"
#include "B2UIFloatingText.h"
#include "B2UIUpgradeStatNotify.h"
#include "B2Inventory.h"
#include "B2UIManager.h"
#include "B2UIUtil.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

UMaterialInstance* FSkillBadgeImage::GetSkillBadgeImage(const TArray<FSkillBadgeImage>& SkillBadgeImages, ESkillType InSkillType)
{
	for (auto& SkillBadgeImage : SkillBadgeImages)
	{
		if (SkillBadgeImage.SkillType == InSkillType)
			return SkillBadgeImage.BadgeImage;
	}

	return nullptr;
}

const FSlateColor& FSkillBadgeImage::GetSkillBadgeFontColor(const TArray<FSkillBadgeImage>& SkillBadgeImages, ESkillType InSkillType)
{
	for (auto& SkillBadgeImage : SkillBadgeImages)
	{
		if (SkillBadgeImage.SkillType == InSkillType)
			return SkillBadgeImage.FontColor;
	}

	return FLearnLevelFontColor::DefaultColor;
}

FSlateColor FLearnLevelFontColor::DefaultColor = FSlateColor();
const FSlateColor& FLearnLevelFontColor::GetFontColor(const TArray<FLearnLevelFontColor>& LearnLevelFontColors, int32 LearnLevel)
{
	for (auto& LearnLevelFontColor : LearnLevelFontColors)
	{
		if (LearnLevelFontColor.LearnLevel >= LearnLevel)
			return LearnLevelFontColor.FontColor;
	}

	return DefaultColor;
}

void UB2UIPlayerSkillInfo::SetButtonStyle(const FButtonStyle& InButtonStyle)
{
	//if (IMG_SkillIcon.IsValid())
	//{
	//	auto* MI = Cast<UMaterialInterface>(InButtonStyle.Normal.GetResourceObject());
	//	if (MI == nullptr)
	//	{
	//		IMG_SkillIcon->SetVisibility(ESlateVisibility::Hidden);
	//	}
	//	else
	//	{
	//		IMG_SkillIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	//		IMG_SkillIcon->SetBrushFromMaterial(MI);
	//		if (bLockLimitLevel)
	//			IMG_SkillIcon->GetBrush().TintColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f));
	//		else
	//			IMG_SkillIcon->GetBrush().TintColor = FSlateColor(FLinearColor::White);
	//	}		
	//}
}

void UB2UIPlayerSkillInfo::SetSkillName(FText InSkillName)
{
	if (TB_SkillName.IsValid())
		TB_SkillName->SetText(InSkillName);
}

void UB2UIPlayerSkillInfo::SetSkillLevel(int32 InSkillLevel, bool bIsRegist)
{
	/*
	if (Ov_SkillAddedOptions.IsValid())
	{
		if (SkillLevel != UNDEFINED_SKILL_LEVEL && InSkillLevel > SkillLevel)
		{
			TArray<FSkillOptionData> DiffOptions;
			BladeIIGameImpl::GetClientDataStore().GetDiffSkillOption(SkillId, SkillLevel, InSkillLevel, DiffOptions);

			Ov_SkillAddedOptions->SetVisibility(ESlateVisibility::HitTestInvisible);
						
			UB2UIFloatingText* CreatedWidget = nullptr;

			for (auto& Widget : SkillAddedOptions)
			{
				if (Widget.IsValid() && Widget->IsFinished())
				{
					CreatedWidget = Widget.Get();
					break;
				}
			}

			if (CreatedWidget == nullptr)
				CreatedWidget = CreateWidget<UB2UIFloatingText>(GetOwningPlayer(), FloatTextClass);
			else
			{
				Ov_SkillAddedOptions->RemoveChild(CreatedWidget);
			}

			if (CreatedWidget)
			{
				Ov_SkillAddedOptions->AddChild(CreatedWidget);
				CreatedWidget->Init();

				FText SkillOptionText;
				for (auto& AddedOption : DiffOptions)
				{
					if (!SkillOptionText.IsEmpty())
						SkillOptionText = FText::Format(FText::FromString(TEXT("{0}\n{1}")), SkillOptionText, FText::Format(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::OptionPopupDesc, SkillId, AddedOption.OptionId)), FText::AsNumber(AddedOption.OptionValue)));
					else
						SkillOptionText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::OptionPopupDesc, SkillId, AddedOption.OptionId)), FText::AsNumber(AddedOption.OptionValue));
				}

				CreatedWidget->SetText(SkillOptionText);
				CreatedWidget->PlayAnim_BP();				
					
				SkillAddedOptions.AddUnique(CreatedWidget);
			}	

			PlaySkillEnhance_BP();
		}
		else
		{
			Ov_SkillAddedOptions->SetVisibility(ESlateVisibility::Hidden);
		}
	}*/

	if (!bIsRegist)
	{
		if (SkillLevel != UNDEFINED_SKILL_LEVEL && InSkillLevel > SkillLevel)
		{
			TArray<FSkillOptionData> DiffOptions;
			BladeIIGameImpl::GetClientDataStore().GetDiffSkillOption(SkillId, SkillLevel, InSkillLevel, DiffOptions);

			if (DiffOptions.Num() > 0)
			{
				UB2UIUpgradeStatNotify* UpgradeStatNotifyUI = UB2UIManager::GetInstance()->OpenUI<UB2UIUpgradeStatNotify>(UIFName::UpgradeStatNotify);
				FText SkillOptionText;
				if (UpgradeStatNotifyUI != nullptr)
				{
					UpgradeStatNotifyUI->ClearList();
					for (auto& AddedOption : DiffOptions)
					{
						SkillOptionText = BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::OptionPopupDesc, SkillId, AddedOption.OptionId));

						UpgradeStatNotifyUI->AddListItem(SkillOptionText, GetSkillOptionValueDisplayText(SvrToCliSkillOption(AddedOption.OptionId), AddedOption.OptionValue), AddedOption.OptionValue > 0);
					}
				}
			}

			PlaySkillEnhance_BP();
		}
	}

	SkillLevel = InSkillLevel;

	if (TB_SkillLevel.IsValid())
	{
		TB_SkillLevel->SetColorAndOpacity(SkillLevel == 0 ? FSlateColor(FLinearColor::White) : SkillLevelColor);
		TB_SkillLevel->SetText(FText::AsNumber(SkillLevel));
	}
}

void UB2UIPlayerSkillInfo::DestroySelf(class UB2UIManager* InUIManager)
{
	if (Ov_SkillAddedOptions.IsValid())
	{
		Ov_SkillAddedOptions->ClearChildren();
	}

	SkillAddedOptions.Empty();

	Super::DestroySelf(InUIManager);
}

void UB2UIPlayerSkillInfo::SetSkillMaxLevel(int32 InSkillMaxLevel)
{
	if (TB_SkillMaxLevel.IsValid())
	{
		TB_SkillMaxLevel->SetText(FText::Format(FText::FromString(TEXT("/{0}")), FText::AsNumber(InSkillMaxLevel)));
	}
}

void UB2UIPlayerSkillInfo::SetSkillType(ESkillType InSkillType)
{
	if (TB_SkillType.IsValid())
	{
		switch (InSkillType)
		{
		case ESkillType::ESkillType_Attack:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Attack")));
			break;
		case ESkillType::ESkillType_Defense:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Defense")));
			break;
		case ESkillType::ESkillType_Survive:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Survive")));
			break;
		case ESkillType::ESkillType_Buff:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Buff")));
			break;
		case ESkillType::ESkillType_Counter:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Counter")));
			break;
		case ESkillType::ESkillType_Tag:
			TB_SkillType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillType_Tag")));
			break;
		}

		TB_SkillType->SetColorAndOpacity(FSkillBadgeImage::GetSkillBadgeFontColor(SkillBadgeImages, InSkillType));
	}

	if (IMG_Skill_Badge.IsValid())
	{
		auto* SkillBadgeImage = FSkillBadgeImage::GetSkillBadgeImage(SkillBadgeImages, InSkillType);
		if (SkillBadgeImage)
			IMG_Skill_Badge->SetBrushFromMaterial(SkillBadgeImage);
	}
}

void UB2UIPlayerSkillInfo::SetSkillId(int32 InSkillId)
{
	SkillId = InSkillId;
	SkillLevelMax = BladeIIGameImpl::GetClientDataStore().GetMaxSkillLevel(SkillId);
	SetSkillName(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillName, SkillId)));
	SetSkillMaxLevel(SkillLevelMax);
	SetSkillLevelupGold(SkillId);
}

void UB2UIPlayerSkillInfo::SetSkillLevelupGold(int32 InSkillId)
{
	int32 Cost = GetLevelUpGold(SkillId);
	if (TB_NeedLevelupGold.IsValid())
	{
		bool EnoughGold = Cost <= BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
		TB_NeedLevelupGold->SetText(FText::AsNumber(Cost));
		TB_NeedLevelupGold->SetColorAndOpacity(EnoughGold ? SkillEnableColor : SkillNotEnoughConditionColor);
	}
}

void UB2UIPlayerSkillInfo::SetSkillLevelupPoint(int32 InSkillId, int32 SkillPointAmount)
{
	int32 Cost = BladeIIGameImpl::GetLocalCharacterData().GetCharacterSkillLevelupPoint(SkillId, SkillLevel);
	if (TB_NeedSkillPoint.IsValid())
	{
		bool EnoughPoint = Cost <= SkillPointAmount;
		TB_NeedSkillPoint->SetText(FText::AsNumber(Cost));
		TB_NeedSkillPoint->SetColorAndOpacity(EnoughPoint ? SkillEnableColor : SkillNotEnoughConditionColor);
	}
}

void UB2UIPlayerSkillInfo::UpdatePassiveSkillLock()
{
	SetRegistred(false);
	UpdateEnableLevelUp(false);

	if (Ov_Lock.IsValid())
		Ov_Lock->SetVisibility(ESlateVisibility::Hidden);

	if (SkillIconBase.IsValid())
		SkillIconBase->SetVisibility(ESlateVisibility::Hidden);

	if (OV_SkillLevel.IsValid())
		OV_SkillLevel->SetVisibility(ESlateVisibility::Hidden);

	if (Skill_Badge.IsValid())
		Skill_Badge->SetVisibility(ESlateVisibility::Hidden);

	if (IMG_Learned.IsValid())
		IMG_Learned->SetVisibility(ESlateVisibility::Hidden);

	if (IMG_OpenSkillBG.IsValid())
		IMG_OpenSkillBG->SetVisibility(ESlateVisibility::Hidden);

	if (IMG_LockImage.IsValid())
		IMG_LockImage->SetVisibility(ESlateVisibility::HitTestInvisible);

	if(Add_SkillFrame.IsValid())
		Add_SkillFrame->SetVisibility(ESlateVisibility::Hidden);

	if (PN_NeedLevelupGold.IsValid())
		PN_NeedLevelupGold->SetVisibility(ESlateVisibility::Hidden);

	if (PN_NeedSkillPoint.IsValid())
		PN_NeedSkillPoint->SetVisibility(ESlateVisibility::Hidden);
	
	if (TB_ConditionUpdate.IsValid())
	{
		TB_ConditionUpdate->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_ComingSoon")));
		TB_ConditionUpdate->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UB2UIPlayerSkillInfo::UpdateLock(int32 CharacterLevel)
{
	int32 LearnLevel = BladeIIGameImpl::GetClientDataStore().GetSkillLearnLevel(SkillId);

	if (Ov_Lock.IsValid())
		Ov_Lock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (SkillIconBase.IsValid())
		SkillIconBase->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (Skill_Badge.IsValid())
		Skill_Badge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (OV_SkillLevel.IsValid())
		OV_SkillLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (Skill_Badge.IsValid())
		Skill_Badge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (OV_SkillLevel.IsValid())
		OV_SkillLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (TB_ConditionUpdate.IsValid())
		TB_ConditionUpdate->SetVisibility(ESlateVisibility::Hidden);

	if (CharacterLevel >= LearnLevel)
	{		
		if (BladeIIGameImpl::GetLocalCharacterData().IsJustAddedSkill(SkillId))
		{
			//연출
			PlayLearnSkill_BP();
			BladeIIGameImpl::GetLocalCharacterData().SetPlayedUnlockSkill(SkillId);
		}

		bLockLimitLevel = false;			

		if (TB_SkillRestrict.IsValid())
			TB_SkillRestrict->SetVisibility(ESlateVisibility::Hidden);

		if (IMG_SkillLevel_N.IsValid())
			IMG_SkillLevel_N->SetVisibility(ESlateVisibility::Hidden);		

		if (HB_SkillLevel.IsValid())
			HB_SkillLevel->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (TB_SkillMaxLevel.IsValid())
			TB_SkillMaxLevel->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (IMG_Learned.IsValid())
			IMG_Learned->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (IMG_OpenSkillBG.IsValid())
			IMG_OpenSkillBG->SetVisibility(ESlateVisibility::HitTestInvisible);	

		if(PN_NeedLevelupGold.IsValid())
			PN_NeedLevelupGold->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (PN_NeedSkillPoint.IsValid())
			PN_NeedSkillPoint->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (TB_SkillName.IsValid())
			TB_SkillName->SetColorAndOpacity(SkillEnableColor);
	}
	else
	{
		bLockLimitLevel = true;

		if (TB_SkillRestrict.IsValid())
		{
			TB_SkillRestrict->SetVisibility(ESlateVisibility::HitTestInvisible);
			TB_SkillRestrict->SetColorAndOpacity(FLearnLevelFontColor::GetFontColor(LearnLevelFontColors, LearnLevel));
			TB_SkillRestrict->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_NotEnoughCharLevel")), FText::AsNumber(LearnLevel)));
		}		

		if (IMG_SkillLevel_N.IsValid())
			IMG_SkillLevel_N->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (HB_SkillLevel.IsValid())
			HB_SkillLevel->SetVisibility(ESlateVisibility::Collapsed);

		if (TB_SkillMaxLevel.IsValid())
			TB_SkillMaxLevel->SetVisibility(ESlateVisibility::Hidden);

		if (IMG_Learned.IsValid())
			IMG_Learned->SetVisibility(ESlateVisibility::Hidden);

		if (IMG_OpenSkillBG.IsValid())
			IMG_OpenSkillBG->SetVisibility(ESlateVisibility::Hidden);

		if (PN_NeedLevelupGold.IsValid())
			PN_NeedLevelupGold->SetVisibility(ESlateVisibility::Hidden);

		if (PN_NeedSkillPoint.IsValid())
			PN_NeedSkillPoint->SetVisibility(ESlateVisibility::Hidden);

		if (TB_SkillName.IsValid())
			TB_SkillName->SetColorAndOpacity(SkillDisableColor);
	}
	
	if (IMG_LockImage.IsValid())
		IMG_LockImage->SetVisibility(bLockLimitLevel ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	auto* ClassInfoBox = StaticFindPCClassInfoBox();
	auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;
	// GetButtonIconStyle 에 로딩이 있어서 const 를 빼야 함..
	UB2SkillInfo* CastedNonConstSkillInfo = const_cast<UB2SkillInfo*>(AllSkillInfo);
	if (CastedNonConstSkillInfo)
		SetButtonStyle(CastedNonConstSkillInfo->GetButtonIconStyle(SkillId));

}

void UB2UIPlayerSkillInfo::UpdateEnableLevelUp(bool bEnoughCondition)
{
	bool IsLevelupPossible = SkillLevel < BladeIIGameImpl::GetClientDataStore().GetMaxSkillLevel(SkillId);
	bool EnoughGold = GetLevelUpGold(SkillId) <= BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
	bool EnoughPoint = B2UIUtil::HasLevelUpSkillPoint(SkillId, SkillLevel);
	bool bSkillLevelMax = SkillLevel == SkillLevelMax;
	bEnableLevelUp = !bLockLimitLevel && bEnoughCondition && IsLevelupPossible && EnoughGold && EnoughPoint;
	IsWatingLevelup = false;

	if (BTN_AddSkillPoint.IsValid())
	{
		BTN_AddSkillPoint->SetIsEnabled(bEnableLevelUp);
		//에디터플레이가 아닐때 버튼이 사라지면 Button IsPressed가 바뀌지 않아서, 비져블 처리하지않고 알파처리
		BTN_AddSkillPoint->SetBackgroundColor(bEnableLevelUp ? FLinearColor(1.0f,1.0f,1.0f,1.0f) : FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	}		
	if (IMG_AddSkillPoint.IsValid())
		IMG_AddSkillPoint->SetVisibility(bEnableLevelUp ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_EnableAddSP.IsValid())
		IMG_EnableAddSP->SetVisibility(bEnableLevelUp ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (FX_EnableAddSP_01.IsValid())
		FX_EnableAddSP_01->SetVisibility(bEnableLevelUp ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (FX_EnableAddSP_02.IsValid())
		FX_EnableAddSP_02->SetVisibility(bEnableLevelUp ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (PN_NeedLevelupGold.IsValid() && bSkillLevelMax)
		PN_NeedLevelupGold->SetVisibility(ESlateVisibility::Hidden);

	if (PN_NeedSkillPoint.IsValid() && bSkillLevelMax)
		PN_NeedSkillPoint->SetVisibility(ESlateVisibility::Hidden);

	if (OV_MaxText.IsValid())
		OV_MaxText->SetVisibility(SkillLevel == SkillLevelMax ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	PlayEnableAddSP(bEnableLevelUp);

	if (Add_SkillFrame.IsValid())
		Add_SkillFrame->SetVisibility(bEnableLevelUp ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

}

void UB2UIPlayerSkillInfo::Init()
{
	Super::Init();

	IsWatingLevelup = false;
	SkillLevel	    = UNDEFINED_SKILL_LEVEL;

	SetSelected(false);
	SetRegistred(false);
	SetSuggested(false);
}

void UB2UIPlayerSkillInfo::CacheAssets()
{
	GET_SLOT(UButton, BTN_SkillIcon);
	GET_SLOT(UButton, BTN_AddSkillPoint);
	GET_SLOT(UImage, IMG_AddSkillPoint);
	GET_SLOT(UImage, IMG_EnableAddSP);
	GET_SLOT(UImage, FX_EnableAddSP_01);
	GET_SLOT(UImage, FX_EnableAddSP_02);

	GET_SLOT(UTextBlock, TB_SkillName);
	GET_SLOT(UHorizontalBox, HB_SkillLevel);
	GET_SLOT(UTextBlock, TB_SkillLevel);
	GET_SLOT(UTextBlock, TB_SkillMaxLevel);
	GET_SLOT(UTextBlock, TB_SkillRestrict);
	GET_SLOT(UImage, IMG_SkillLevel_N);

	GET_SLOT(UTextBlock, TB_SkillType);
	GET_SLOT(UImage, IMG_Skill_Badge);
	
	GET_SLOT(UImage, IMG_Registred);
	GET_SLOT(UImage, IMG_RegistredCheck);
	GET_SLOT(UTextBlock, TB_Registred);

	GET_SLOT(UImage, IMG_Selected);
	GET_SLOT(UImage, IMG_Suggest_Select01);	
	GET_SLOT(UImage, IMG_Suggest_Select02);

	GET_SLOT(UImage, IMG_OpenSkillBG);
	GET_SLOT(UImage, IMG_Learned);
	GET_SLOT(UImage, IMG_NotLearn);
	GET_SLOT(UImage, IMG_SkillIcon);

	GET_SLOT(UImage,  IMG_LockImage);

	GET_SLOT(UOverlay, Ov_SkillAddedOptions);
	
	GET_SLOT(UOverlay, Skill_Badge);
	GET_SLOT(UOverlay, OV_SkillLevel);
	GET_SLOT(UOverlay, Ov_Lock);
	GET_SLOT(UOverlay, SkillIconBase);

	GET_SLOT(UPanelWidget, PN_NeedSkillPoint);
	GET_SLOT(UTextBlock, TB_NeedSkillPoint);

	GET_SLOT(UPanelWidget, PN_NeedLevelupGold);
	GET_SLOT(UTextBlock, TB_NeedLevelupGold);

	GET_SLOT(UTextBlock, TB_ConditionUpdate);
	GET_SLOT(UOverlay, OV_MaxText);
	
	GET_SLOT(UImage, Add_SkillFrame);
}

void UB2UIPlayerSkillInfo::SetSelected(bool bSet)
{
	if (IMG_Selected.IsValid())
		IMG_Selected->SetVisibility(bSet ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);	
}

void UB2UIPlayerSkillInfo::SetSuggested(bool bSet)
{
	if (IMG_Suggest_Select01.IsValid())
		IMG_Suggest_Select01->SetVisibility(bSet ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	if (IMG_Suggest_Select02.IsValid())
		IMG_Suggest_Select02->SetVisibility(bSet ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (!bSuggested && bSet)
		PlaySuggest_BP();

	bSuggested = bSet;
}

void UB2UIPlayerSkillInfo::SetRegistred(bool bSet)
{
	if (IMG_Registred.IsValid())
		IMG_Registred->SetVisibility(bSet ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_RegistredCheck.IsValid())
		IMG_RegistredCheck->SetVisibility(bSet ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	if (TB_Registred.IsValid())
		TB_Registred->SetVisibility(bSet ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);	
}

void UB2UIPlayerSkillInfo::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_AddSkillPoint, &UB2UIPlayerSkillInfo::OnClickBtnAddSkillPoint);
	BIND_CLICK_FUNC_TO_BTN(BTN_SkillIcon, &UB2UIPlayerSkillInfo::OnClickBtnSkillIcon);
}

void UB2UIPlayerSkillInfo::OnClickBtnAddSkillPoint()
{
	if (!IsWatingLevelup)
	{
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2UIPlayerSkillInfo::OnDealyAddSkillPoint, 0.1f, false);
		IsWatingLevelup = true;
	}
}

void UB2UIPlayerSkillInfo::OnClickBtnSkillIcon()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2UIPlayerSkillInfo::OnDelaySkillIcon, 0.1f, false);

}

void UB2UIPlayerSkillInfo::OnDealyAddSkillPoint()
{
	if (bEnableLevelUp == false)
		return;

	if (SkillLevelUpClickHandler.IsBound())
		SkillLevelUpClickHandler.Execute();
}

void UB2UIPlayerSkillInfo::OnDelaySkillIcon()
{
	if (SkillIconClickHandler.IsBound())
		SkillIconClickHandler.Execute();
}

void UB2UIPlayerSkillInfo::SetSkillIconHandler(const FSkillInfoOnClick& ClickHandler)
{
	SkillIconClickHandler = ClickHandler;
}

void UB2UIPlayerSkillInfo::SetSkillLevelUpHandler(const FSkillLevelUpOnClick& ClickHandler)
{
	SkillLevelUpClickHandler = ClickHandler;
}

int32 UB2UIPlayerSkillInfo::GetLevelUpGold(int32 InSkillID)
{
	int32 LevelUpCost = BladeIIGameImpl::GetLocalCharacterData().GetCharacterSkillLevelupGold(InSkillID);
	return LevelUpCost;
}
