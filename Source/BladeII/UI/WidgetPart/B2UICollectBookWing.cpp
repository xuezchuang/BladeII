// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2WingUIP_PropOptionIcon.h"
#include "B2ClientDataStore.h"
#include "B2Inventory.h"
#include "B2Airport.h"
#include "B2WingUIP_EnhLevelProgElem.h"
#include "B2WingUIP_EvolveGradeStar.h"
#include "B2UICollectBookWing.h"
#include "B2WingInfo.h"
#include "B2AndroidBackManager.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

// todo delete IMG_WingGradeUnSelected
#define GET_WINGGRADE_TAB_SLOT(TabStructure, text)												\
	GET_SLOT_BYNAME(UOverlay, TabStructure.O_GradeOverlay, O_TabOverlay##text);					\
	GET_SLOT_BYNAME(UButton, TabStructure.BTN_Area, BTN_WingGradeArea##text);					\
	GET_SLOT_BYNAME(UTextBlock, TabStructure.TB_WingGrade, TB_WingGrade##text);					\


void UB2UICollectBookWingOption::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2WingUIP_PropOptionIcon, UIP_WingOptIcon);

	GET_SLOT(UTextBlock, TB_OptionName);
	GET_SLOT(UTextBlock, TB_OptionValue);

	GET_SLOT(UCanvasPanel, CP_LevelControl);
	GET_SLOT(UButton, BTN_LevelDown);
	GET_SLOT(UButton, BTN_LevelUp);
	GET_SLOT(UButton, BTN_Max);
	GET_SLOT(USlider, S_OptionLevelSlider);

	GET_SLOT(UTextBlock, TB_CurrentLevel);
	GET_SLOT(UTextBlock, TB_MaxLevelDesc);
	GET_SLOT(UTextBlock, TB_MaxLevel);
	GET_SLOT(UTextBlock, TB_EnhanceLevel);


	GET_SLOT(UB2RichTextBlock, TB_OptionLock);
}


void UB2UICollectBookWingOption::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_LevelDown, &UB2UICollectBookWingOption::OnClickBTN_LevelDown);
	BIND_CLICK_FUNC_TO_BTN(BTN_LevelUp, &UB2UICollectBookWingOption::OnClickBTN_LevelUp);
	BIND_CLICK_FUNC_TO_BTN(BTN_Max, &UB2UICollectBookWingOption::OnClickBTN_LevelMax);

	if (S_OptionLevelSlider.IsValid())
		S_OptionLevelSlider->OnValueChanged.AddDynamic(this, &UB2UICollectBookWingOption::SetSlider);

}

void UB2UICollectBookWingOption::UpdateStaticText()
{
	if (TB_EnhanceLevel.IsValid())
		TB_EnhanceLevel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_WingOptionLevel")));

}

void UB2UICollectBookWingOption::Init()
{
	Super::Init();
	bInitPropOption = false;
}

void UB2UICollectBookWingOption::DestroySelf(class UB2UIManager* InUIManager)
{
	if (UIP_WingOptIcon.IsValid())
		UIP_WingOptIcon->DestroySelf();

	Super::DestroySelf(InUIManager);
}


void UB2UICollectBookWingOption::SetWingOption(const FWingPropOption* OptionPtr, EPCClass InCharacterClass)
{
	if (OptionPtr && UIP_WingOptIcon.IsValid())
	{
		if (bInitPropOption == false) // 처음에만 복사
		{
			PropOption = *OptionPtr;
			bInitPropOption = true;
			CharacterClass = InCharacterClass;
		}

		else // 이후 필요한 정보만 셋팅
		{
			PropOption.bIsOpen = OptionPtr->bIsOpen;
			PropOption.MaxOptionLevel = OptionPtr->MaxOptionLevel;
			PropOption.OptionLevel = FMath::Clamp<int32>(OptionPtr->OptionLevel, 1, OptionPtr->MaxOptionLevel);
		}

		UpdateWingOption(PropOption);
	}
}

void UB2UICollectBookWingOption::SetSlider(float InValue)
{
	PropOption.OptionLevel = FMath::RoundHalfToZero(InValue * PropOption.MaxOptionLevel);

	PropOption.OptionLevel = FMath::Clamp(PropOption.OptionLevel, 1, PropOption.MaxOptionLevel);

	UpdateWingOption(PropOption);
	
}


void UB2UICollectBookWingOption::UpdateWingOption(const FWingPropOption& WingOption)
{
	UIP_WingOptIcon->SetUsageType(EWingPropOptionUIPUsage::WPOUU_CollectBookPreview);
	UIP_WingOptIcon->SetMyOptionInfo(PropOption);


	if (TB_OptionName.IsValid())
	{
		TB_OptionName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TB_OptionName->SetText(GetLOCTextOfWingPropOption(PropOption.MyOptionType, CharacterClass));
	}

	if (TB_OptionValue.IsValid())
		TB_OptionValue->SetVisibility(WingOption.bIsOpen ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (WingOption.bIsOpen)
	{
		if (CP_LevelControl.IsValid())
			CP_LevelControl->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (TB_OptionLock.IsValid())
			TB_OptionLock->SetVisibility(ESlateVisibility::Hidden);

		FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
		const FWingMD_PropOptionSingleElem* PropOptionMD = ClientDataStore.GetWingMD_PropOption(CharacterClass, PropIndex, WingOption.OptionLevel);
		if (PropOptionMD)
			PropOption.RawOptionAmount = PropOptionMD->OptionValue;

		if (TB_OptionValue.IsValid())
			TB_OptionValue->SetText(GetItemOptionValueDisplayText(PropOption.MyOptionType, PropOption.RawOptionAmount, true));

		if (TB_CurrentLevel.IsValid())
			TB_CurrentLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SkillLevel"))
				, FText::AsNumber(PropOption.OptionLevel), FText::AsNumber(PropOption.MaxOptionLevel)));

		if (TB_MaxLevelDesc.IsValid())
			TB_MaxLevelDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_MaxEnhanceLevel")));

		if (TB_MaxLevel.IsValid())
			TB_MaxLevel->SetText(FText::AsNumber(PropOption.MaxOptionLevel));

		if (S_OptionLevelSlider.IsValid())
			S_OptionLevelSlider->SetValue(static_cast<float>(PropOption.OptionLevel) / static_cast<float>(PropOption.MaxOptionLevel));

		if (BTN_Max.IsValid())
			BTN_Max->SetIsEnabled(PropOption.OptionLevel == PropOption.MaxOptionLevel ? false : true);
	}

	else
	{
		if (CP_LevelControl.IsValid())
			CP_LevelControl->SetVisibility(ESlateVisibility::Hidden);

		if (TB_OptionLock.IsValid())
		{
			TB_OptionLock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			FText OptionLockText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_WingOptionLock"));
			TB_OptionLock->SetText(FText::Format(OptionLockText, FText::AsNumber(WingOption.RequiredWingEvolveGrade)));
		}
	}
}

void UB2UICollectBookWingOption::OnClickBTN_LevelDown()
{
	PropOption.OptionLevel = FMath::Clamp<int32>(PropOption.OptionLevel -1, 1, PropOption.MaxOptionLevel);
	UpdateWingOption(PropOption);
}


void UB2UICollectBookWingOption::OnClickBTN_LevelUp()
{
	PropOption.OptionLevel = FMath::Clamp<int32>(PropOption.OptionLevel + 1, 1, PropOption.MaxOptionLevel);
	UpdateWingOption(PropOption);
}

void UB2UICollectBookWingOption::OnClickBTN_LevelMax()
{
	PropOption.OptionLevel = PropOption.MaxOptionLevel;
	UpdateWingOption(PropOption);
}


void UB2UICollectBookWingOptionPanel::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_WingOptionTitle);
	GET_SLOT(UTextBlock, TB_WingOptionDesc);
	GET_SLOT(UButton, BTN_Close);

	WingOptionSlots.AddDefaulted(MAX_WING_PROP_OPTION_NUM);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, WingOptionSlots[0], UIP_WingOptionSlot1);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, WingOptionSlots[1], UIP_WingOptionSlot2);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, WingOptionSlots[2], UIP_WingOptionSlot3);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, WingOptionSlots[3], UIP_WingOptionSlot4);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, WingOptionSlots[4], UIP_WingOptionSlot5);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, WingOptionSlots[5], UIP_WingOptionSlot6);

	int32 PropIndex = 0;
	for (auto& WingOptionSlot : WingOptionSlots)
	{
		if (WingOptionSlot.IsValid())
		{
			WingOptionSlot->Init();
			WingOptionSlot->InitPropIndex(PropIndex++);
		}
	}
}

void UB2UICollectBookWingOptionPanel::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UICollectBookWingOptionPanel::OnClickBTN_Close);
}

void UB2UICollectBookWingOptionPanel::UpdateStaticText()
{
	if (TB_WingOptionTitle.IsValid())
		TB_WingOptionTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_WingPanelTitle")));

	if (TB_WingOptionDesc.IsValid())
		TB_WingOptionDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectBook_WingPanelDesc")));
}

void UB2UICollectBookWingOptionPanel::Init()
{
	Super::Init();
}

void UB2UICollectBookWingOptionPanel::DestroySelf(class UB2UIManager* InUIManager)
{
	for (auto& WingOptionSlot : WingOptionSlots)
	{
		if (WingOptionSlot.IsValid())
			WingOptionSlot->DestroySelf(InUIManager);
	}

	if (this->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
	
	Super::DestroySelf(InUIManager);
}

void UB2UICollectBookWingOptionPanel::SetPropInfo(EPCClass CharacterClass, int32 PropIndex, const FWingPropOption& PropOption)
{
	if (WingOptionSlots.IsValidIndex(PropIndex) && WingOptionSlots[PropIndex].IsValid())
	{
		WingOptionSlots[PropIndex]->SetWingOption(&PropOption, CharacterClass);
	}
}

void UB2UICollectBookWingOptionPanel::OpenPopup()
{
	if (GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	}
}

void UB2UICollectBookWingOptionPanel::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UICollectBookWingOptionPanel::OnClickBTN_Close()
{
	if (OwnerWingUI != nullptr)
	{
		for (auto& OptionSlot : WingOptionSlots)
		{
			if (OptionSlot.IsValid())
			{
				OwnerWingUI->OverrideWingPropOption(OptionSlot->GetPropIndex(), OptionSlot->GetCurrentPropOption());
			}
		}
	}

	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	SetVisibility(ESlateVisibility::Hidden);
}


void UB2UICollectBookWing::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UHorizontalBox, HB_WingGradeTabs);

	GET_SLOT(UTextBlock, TB_UpperWingName);
	GET_SLOT(UTextBlock, TB_AttackIncLabel);
	GET_SLOT(UTextBlock, TB_AttackIncAmount);
	GET_SLOT(UTextBlock, TB_DefenseIncLabel);
	GET_SLOT(UTextBlock, TB_DefenseIncAmount);
	GET_SLOT(UTextBlock, TB_HealthIncLabel);
	GET_SLOT(UTextBlock, TB_HealthIncAmount);

	GET_SLOT(UImage, IMG_BigFatIcon);
	GET_SLOT(UB2WingUIP_EvolveGradeStar, UIP_WingGradeStar);

	GET_SLOT(UOverlay, O_WingLevel);
	GET_SLOT(UButton, BTN_WingLevelUp);
	GET_SLOT(UButton, BTN_WingLevelDown);

	GET_SLOT(UTextBlock, TB_WingOption);

	GET_WINGGRADE_TAB_SLOT(BeforeTab, Before);
	GET_WINGGRADE_TAB_SLOT(SelectTab, Select);
	GET_WINGGRADE_TAB_SLOT(AfterTab, After);

	UIP_WingLevelIcons.AddDefaulted(MAX_WING_ENHANCE_LEVEL);
	GET_SLOT_BYNAME(UB2WingUIP_EnhLevelProgElem, UIP_WingLevelIcons[0], LevelProgElem_1);
	GET_SLOT_BYNAME(UB2WingUIP_EnhLevelProgElem, UIP_WingLevelIcons[1], LevelProgElem_2);
	GET_SLOT_BYNAME(UB2WingUIP_EnhLevelProgElem, UIP_WingLevelIcons[2], LevelProgElem_3);
	GET_SLOT_BYNAME(UB2WingUIP_EnhLevelProgElem, UIP_WingLevelIcons[3], LevelProgElem_4);
	GET_SLOT_BYNAME(UB2WingUIP_EnhLevelProgElem, UIP_WingLevelIcons[4], LevelProgElem_5);
	GET_SLOT_BYNAME(UB2WingUIP_EnhLevelProgElem, UIP_WingLevelIcons[5], LevelProgElem_6);
	GET_SLOT_BYNAME(UB2WingUIP_EnhLevelProgElem, UIP_WingLevelIcons[6], LevelProgElem_7);
	GET_SLOT_BYNAME(UB2WingUIP_EnhLevelProgElem, UIP_WingLevelIcons[7], LevelProgElem_8);
	GET_SLOT_BYNAME(UB2WingUIP_EnhLevelProgElem, UIP_WingLevelIcons[8], LevelProgElem_9);
	GET_SLOT_BYNAME(UB2WingUIP_EnhLevelProgElem, UIP_WingLevelIcons[9], LevelProgElem_10);
	
	UIP_WingOptionIcons.AddDefaulted(MAX_WING_PROP_OPTION_NUM);
	GET_SLOT_BYNAME(UB2WingUIP_PropOptionIcon, UIP_WingOptionIcons[0], UIP_WingOptIcon1);
	GET_SLOT_BYNAME(UB2WingUIP_PropOptionIcon, UIP_WingOptionIcons[1], UIP_WingOptIcon2);
	GET_SLOT_BYNAME(UB2WingUIP_PropOptionIcon, UIP_WingOptionIcons[2], UIP_WingOptIcon3);
	GET_SLOT_BYNAME(UB2WingUIP_PropOptionIcon, UIP_WingOptionIcons[3], UIP_WingOptIcon4);
	GET_SLOT_BYNAME(UB2WingUIP_PropOptionIcon, UIP_WingOptionIcons[4], UIP_WingOptIcon5);
	GET_SLOT_BYNAME(UB2WingUIP_PropOptionIcon, UIP_WingOptionIcons[5], UIP_WingOptIcon6);

	BTN_WingOptionIcons.AddDefaulted(MAX_WING_PROP_OPTION_NUM);
	GET_SLOT_BYNAME(UButton, BTN_WingOptionIcons[0], BTN_WingOptionIcon1);
	GET_SLOT_BYNAME(UButton, BTN_WingOptionIcons[1], BTN_WingOptionIcon2);
	GET_SLOT_BYNAME(UButton, BTN_WingOptionIcons[2], BTN_WingOptionIcon3);
	GET_SLOT_BYNAME(UButton, BTN_WingOptionIcons[3], BTN_WingOptionIcon4);
	GET_SLOT_BYNAME(UButton, BTN_WingOptionIcons[4], BTN_WingOptionIcon5);
	GET_SLOT_BYNAME(UButton, BTN_WingOptionIcons[5], BTN_WingOptionIcon6);
	
	UIP_WingOptions.AddDefaulted(MAX_WING_PROP_OPTION_NUM);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, UIP_WingOptions[0], UIP_WingOption1);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, UIP_WingOptions[1], UIP_WingOption2);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, UIP_WingOptions[2], UIP_WingOption3);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, UIP_WingOptions[3], UIP_WingOption4);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, UIP_WingOptions[4], UIP_WingOption5);
	GET_SLOT_BYNAME(UB2UICollectBookWingOption, UIP_WingOptions[5], UIP_WingOption6);

	int32 PropIndex = 0;
	for (auto& WingOption : UIP_WingOptions)
	{
		if (WingOption.IsValid())
		{
			WingOption->Init();
			WingOption->InitPropIndex(PropIndex++);
		}
	}

	GET_SLOT(UButton, BTN_WingOptionPanel);
}

void UB2UICollectBookWing::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BeforeTab.BTN_Area, &UB2UICollectBookWing::OnClickBTN_BeforeWingGrade);
	BIND_CLICK_FUNC_TO_BTN(SelectTab.BTN_Area, &UB2UICollectBookWing::OnClickBTN_WingGrade);
	BIND_CLICK_FUNC_TO_BTN(AfterTab.BTN_Area, &UB2UICollectBookWing::OnClickBTN_AfterWingGrade);

	BIND_CLICK_FUNC_TO_BTN(BTN_WingLevelUp, &UB2UICollectBookWing::OnClickBTN_WingLevelUp);
	BIND_CLICK_FUNC_TO_BTN(BTN_WingLevelDown, &UB2UICollectBookWing::OnClickBTN_WingLevelDown);

	BIND_CLICK_FUNC_TO_BTN(BTN_WingOptionPanel, &UB2UICollectBookWing::OnClickBTN_WingOptionPanel);
	BIND_CLICK_FUNC_TO_BTN(BTN_WingOptionIcons[0], &UB2UICollectBookWing::OnClickBTN_WingOptionPanel);
	BIND_CLICK_FUNC_TO_BTN(BTN_WingOptionIcons[1], &UB2UICollectBookWing::OnClickBTN_WingOptionPanel);
	BIND_CLICK_FUNC_TO_BTN(BTN_WingOptionIcons[2], &UB2UICollectBookWing::OnClickBTN_WingOptionPanel);
	BIND_CLICK_FUNC_TO_BTN(BTN_WingOptionIcons[3], &UB2UICollectBookWing::OnClickBTN_WingOptionPanel);
	BIND_CLICK_FUNC_TO_BTN(BTN_WingOptionIcons[4], &UB2UICollectBookWing::OnClickBTN_WingOptionPanel);
	BIND_CLICK_FUNC_TO_BTN(BTN_WingOptionIcons[5], &UB2UICollectBookWing::OnClickBTN_WingOptionPanel);
}

void UB2UICollectBookWing::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_AttackIncLabel.IsValid())
	{
		TB_AttackIncLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Option_AttackInc")));
	}
	if (TB_DefenseIncLabel.IsValid())
	{
		TB_DefenseIncLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Option_DefenseInc")));
	}
	if (TB_HealthIncLabel.IsValid())
	{
		TB_HealthIncLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Option_HealthInc")));
	}

	
	if (TB_WingOption.IsValid())
		TB_WingOption->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_ECSA_WingAdditionalOption")));

}

UB2UICollectBookWing::UB2UICollectBookWing(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CurrentWingGrade = MIN_WING_EVOLUTION_GRADE; // 초기 PageView 셋팅을 위해 
	CurrentEnhanceLevel = MIN_WING_ENHANCE_LEVEL;
}

void UB2UICollectBookWing::Init()
{
	Super::Init();
}

void UB2UICollectBookWing::DestroySelf(UB2UIManager* InUIManager)
{
	WingGradeClickDelegate.Unbind();
	WingOptionClickDelegate.Unbind();

	for (auto& WingIcon : UIP_WingOptionIcons)
		if (WingIcon.IsValid()) WingIcon->DestroySelf();

	for (auto& WingOption : UIP_WingOptions)
		if (WingOption.IsValid()) WingOption->DestroySelf(InUIManager);

	if (WingOptionPanelWidget)
		WingOptionPanelWidget->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}


void UB2UICollectBookWing::OnClickedWingPage()
{
	for (int32 Idx = 0; Idx < MAX_WING_ENHANCE_LEVEL; Idx++)
		if (UIP_WingLevelIcons[Idx].IsValid())
			UIP_WingLevelIcons[Idx]->SetMyEnhanceLevel(Idx + 1);
	
	UpdateWingPage(MIN_WING_EVOLUTION_GRADE, true);	// 등급 1 강화 1 상태로 초기상태 설정
}

void UB2UICollectBookWing::SetClickWingGradeDelegate(FOnClickedCollectionWingGrade& Delegate)
{
	WingGradeClickDelegate = Delegate;
}

void UB2UICollectBookWing::SetClickWingOptionDelegate(FOnClickedWingOptionPanel& Delegate)
{
	WingOptionClickDelegate = Delegate;
}

void UB2UICollectBookWing::OverrideWingPropOption(int32 PropIndex, const FWingPropOption& PropOption)
{
	if (UIP_WingOptions.IsValidIndex(PropIndex))
	{
		auto& WingOptionWidget = UIP_WingOptions[PropIndex];
		if (WingOptionWidget.IsValid())
			WingOptionWidget->SetWingOption(&PropOption, CharacterClass);
	}
}

void UB2UICollectBookWing::UpdateWingPage(int32 WingGrade, bool bForce)
{
	UB2WingInfo* WingInfoTable = StaticFindWingInfo();

	if (CurrentWingGrade != WingGrade || bForce)
	{
		if (IMG_BigFatIcon.IsValid())
		{
			IMG_BigFatIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
			UMaterialInterface* GradeIconMtrl = WingInfoTable ? WingInfoTable->GetWingMainIconMtrlForGrade(WingGrade) : NULL;
			if (GradeIconMtrl)
			{
				IMG_BigFatIcon->SetBrushFromMaterial(GradeIconMtrl);
			}
		}

		if (UIP_WingGradeStar.IsValid())
			UIP_WingGradeStar->SetCurrentGrade(WingGrade);

		if (TB_UpperWingName.IsValid())
			TB_UpperWingName->SetText(GetLOCTextOfWingName(CharacterClass, WingGrade));

		if (BeforeTab.TB_WingGrade.IsValid() && BeforeTab.O_GradeOverlay.IsValid())
		{
			BeforeTab.O_GradeOverlay->SetVisibility(WingGrade == MIN_WING_EVOLUTION_GRADE ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
			BeforeTab.TB_WingGrade->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_gradeTab")), FText::AsNumber(WingGrade - 1)));
		}

		if (SelectTab.TB_WingGrade.IsValid() && SelectTab.O_GradeOverlay.IsValid())
		{
			SelectTab.TB_WingGrade->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_gradeTab")), FText::AsNumber(WingGrade)));
		}

		if (AfterTab.TB_WingGrade.IsValid() && AfterTab.O_GradeOverlay.IsValid())
		{
			AfterTab.O_GradeOverlay->SetVisibility(WingGrade == MAX_WING_EVOLUTION_GRADE ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
			AfterTab.TB_WingGrade->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_gradeTab")), FText::AsNumber(WingGrade  + 1)));
		}


		// Grade가 바뀌면 Level은 초기화 - 기획 요청
		CurrentEnhanceLevel = 0; 
		UpdateWingLevelVisible(CurrentEnhanceLevel);
		UpdateWingAbility(WingGrade, CurrentEnhanceLevel);

		WingGradeClickDelegate.ExecuteIfBound(CurrentWingGrade);
	}
}


void UB2UICollectBookWing::UpdateWingLevelVisible(int32 WingLevel)
{
	for (auto& LevelIcon : UIP_WingLevelIcons)
	{
		if (LevelIcon.IsValid())
		{
			LevelIcon->UpdateByWingEnhanceLevel(WingLevel);
		}
	}
}

void UB2UICollectBookWing::UpdateWingAbility(int32 WingGrade, int32 WingLevel)
{
	WingInfo.EvolutionGrade = WingGrade;
	WingInfo.EnhanceLevel = WingLevel;

	if (WingInfo.UpdateStats())	// 현재 등급과 강화Level로 날개 데이터 갱신
	{
		UpdatePrimaryAbility(static_cast<int32>(WingInfo.GetAttackPoint()),
			static_cast<int32>(WingInfo.GetDefensePoint()),
			static_cast<int32>(WingInfo.GetHealthPoint()));


		for (int32 PropIdx = 0; PropIdx < MAX_WING_PROP_OPTION_NUM; PropIdx++)
		{
			const FWingPropOption* PropOption = WingInfo.GetPropOptionPtr(PropIdx);

			// 1. UI 상단의 날개에 박혀있는 OptionIcon 설정
			if (UIP_WingOptionIcons.IsValidIndex(PropIdx) && UIP_WingOptionIcons[PropIdx].IsValid())
			{
				TWeakObjectPtr<UB2WingUIP_PropOptionIcon>& OptionIconWidget = UIP_WingOptionIcons[PropIdx];
				if (PropOption && OptionIconWidget.IsValid())
				{
					OptionIconWidget->SetUsageType(EWingPropOptionUIPUsage::WPOUU_CollectBookOnlyIcon);
					OptionIconWidget->SetMyOptionInfo(*PropOption);
				}
			}

			// 2. UI 하단의 Option별 상세 수치 설정
			if (UIP_WingOptions.IsValidIndex(PropIdx) && UIP_WingOptions[PropIdx].IsValid())
			{
				UIP_WingOptions[PropIdx]->SetWingOption(PropOption, CharacterClass);
			}
		}

		CurrentEnhanceLevel = WingLevel;
		CurrentWingGrade = WingGrade;
	}
}

void UB2UICollectBookWing::UpdatePrimaryAbility(int32 Attack, int32 Defence, int32 Health)
{
	const FText AbilityFormat(FText::FromString(TEXT("+{0}")));

	if (TB_AttackIncAmount.IsValid()) TB_AttackIncAmount->SetText(FText::Format(AbilityFormat, FText::AsNumber(Attack)));
	if (TB_DefenseIncAmount.IsValid()) TB_DefenseIncAmount->SetText(FText::Format(AbilityFormat, FText::AsNumber(Defence)));
	if (TB_HealthIncAmount.IsValid()) TB_HealthIncAmount->SetText(FText::Format(AbilityFormat, FText::AsNumber(Health)));
}

void UB2UICollectBookWing::OnClickBTN_WingLevelUp()
{
	int32 NewLevel = FMath::Clamp<int32>(CurrentEnhanceLevel + 1, MIN_WING_ENHANCE_LEVEL, MAX_WING_ENHANCE_LEVEL);
	UpdateWingLevelVisible(NewLevel);
	UpdateWingAbility(CurrentWingGrade, NewLevel);
}

void UB2UICollectBookWing::OnClickBTN_WingLevelDown()
{
	int32 NewLevel = FMath::Clamp<int32>(CurrentEnhanceLevel - 1, MIN_WING_ENHANCE_LEVEL, MAX_WING_ENHANCE_LEVEL);
	UpdateWingLevelVisible(NewLevel);
	UpdateWingAbility(CurrentWingGrade, NewLevel);
}

void UB2UICollectBookWing::OnClickBTN_WingOptionPanel()
{
	UB2UIManager* UIManager = UB2UIManager::GetInstance();
	if (UIManager && WingOptionPanelWidget == nullptr)
	{
		WingOptionPanelWidget = UIManager->OpenUI<UB2UICollectBookWingOptionPanel>(UIFName::CollectBookWingOption);
	}

	if (WingOptionPanelWidget)
	{
		for (int32 PropIdx = 0; PropIdx < MAX_WING_PROP_OPTION_NUM; PropIdx++)
		{
			if (UIP_WingOptions.IsValidIndex(PropIdx) && UIP_WingOptions[PropIdx].IsValid())
			{
				WingOptionPanelWidget->SetPropInfo(CharacterClass, PropIdx, UIP_WingOptions[PropIdx]->GetCurrentPropOption());
			}
		}
		
		WingOptionPanelWidget->OpenPopup();
		WingOptionPanelWidget->SetOwnerWingUI(this);
	}
}
