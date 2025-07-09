// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_WingEvolveResult.h"
#include "B2LobbyGameMode.h"
#include "B2Airport.h"
#include "B2WingInfo.h"
#include "B2Inventory.h"
#include "Event.h"
#include "B2AndroidBackManager.h"

#include "BladeIIGameImpl.h"

UB2LobbyUI_WingEvolveResult::UB2LobbyUI_WingEvolveResult(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedAirport = NULL;
	SetClose(false);
}

void UB2LobbyUI_WingEvolveResult::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2WingUIP_MainSymbol, UIP_MainSymbol);

	GET_SLOT(UTextBlock, TB_AttackIncLabel);
	GET_SLOT(UTextBlock, TB_AttackIncAmount);
	GET_SLOT(UImage, IMG_AttackIncSign);
	GET_SLOT(UTextBlock, TB_DefenseIncLabel);
	GET_SLOT(UTextBlock, TB_DefenseIncAmount);
	GET_SLOT(UImage, IMG_DefenseIncSign);
	GET_SLOT(UTextBlock, TB_HealthIncLabel);
	GET_SLOT(UTextBlock, TB_HealthIncAmount);
	GET_SLOT(UImage, IMG_HealthIncSign);

	GET_SLOT(UImage, IMG_NewPropOptionIcon);
	GET_SLOT(UTextBlock, TB_PropOptionLabel);
	GET_SLOT(UTextBlock, TB_NewPropOptionInfo);
	GET_SLOT(UTextBlock, TB_NewPropOptionInfo2);

	GET_SLOT(UTextBlock, TB_Suc);

	GET_SLOT(UB2Button, BTN_Confirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2LobbyUI_WingEvolveResult::OnClickedBtnConfirm);

	GET_SLOT(UTextBlock, TB_Confirm);

	GET_SLOT(UOverlay, O_OpsionIcon);

	GET_SLOT(UOverlay, O_Complete);
	GET_SLOT(UTextBlock, TB_Complete);
	
}

void UB2LobbyUI_WingEvolveResult::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
	
	B2AndroidBackManager::GetInstance()->AddBackWidget(this);

	if (UIP_MainSymbol.IsValid())
	{
		UIP_MainSymbol->StartFromLobby(InUIManager, InGM);
		UIP_MainSymbol->SetPropIconUsageType(EWingPropOptionUIPUsage::WPOUU_WingEvolveResult);
	}

	CachedAirport = CachedLobbyGM ? CachedLobbyGM->GetLobbyAirport() : NULL;
	BII_CHECK(CachedAirport);

	const EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;

	// 진화가 완료된 시점이어야 한다. 진화 이전 데이터는 Airport 에 캐싱..
	FB2Wing PreEvolveData;
	const bool bPreEvolveDataValid = CachedAirport ? CachedAirport->GetPreEvolveData(PreEvolveData) : false;
	FB2Wing EvolvedWingData;
	const bool bEvolvedWingDataValid = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, EvolvedWingData);
	if (bPreEvolveDataValid && bEvolvedWingDataValid)
	{
		UpdateByWingEvolutionData(PreEvolveData, EvolvedWingData);
	}

	UpdateStaticWidgets();
}

void UB2LobbyUI_WingEvolveResult::DestroySelf()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	if (UIP_MainSymbol.IsValid())
	{
		UIP_MainSymbol->DestroySelf();
	}

	Super::DestroySelf();
}

void UB2LobbyUI_WingEvolveResult::UpdateStaticWidgets()
{
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

	if (TB_PropOptionLabel.IsValid())
	{
		TB_PropOptionLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_PropOptionLabel")));
	}

	if (TB_Suc.IsValid())
	{
		TB_Suc->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_EvolutionSuccess")));
	}
	
	if (TB_Confirm.IsValid())
	{
		TB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
	}
}

void UB2LobbyUI_WingEvolveResult::UpdateByWingEvolutionData(const FB2Wing& PrevWing, const FB2Wing& EvolvedWing)
{
	// 여기는 WingMain 화면처럼 현재 수치를 뿌리는데.. 증가 표시를 좀 한다.
	if (TB_AttackIncAmount.IsValid())
	{
		TB_AttackIncAmount->SetText(
			FText::FromString(FString::Printf(TEXT("%d"), (int32)EvolvedWing.GetAttackPoint())) // 추후 요구사항에 따라 GetAutoFractionalFormattedText 를 참고..
			);
	}
	if (TB_DefenseIncAmount.IsValid())
	{
		TB_DefenseIncAmount->SetText(
			FText::FromString(FString::Printf(TEXT("%d"), (int32)EvolvedWing.GetDefensePoint()))
			);
	}
	if (TB_HealthIncAmount.IsValid())
	{
		TB_HealthIncAmount->SetText(
			FText::FromString(FString::Printf(TEXT("%d"), (int32)EvolvedWing.GetHealthPoint()))
			);
	}

	// 수치가 이전보다 증가했는지.. 아마도 증가했겠지만 예의상 체크는 해야지
	const bool bAttackIncreased = (EvolvedWing.GetAttackPoint() - PrevWing.GetAttackPoint() > 0.0f);
	const bool bDefenseIncreased = (EvolvedWing.GetDefensePoint() - PrevWing.GetDefensePoint() > 0.0f);
	const bool bHealthIncreased = (EvolvedWing.GetHealthPoint() - PrevWing.GetHealthPoint() > 0.0f);

	if (IMG_AttackIncSign.IsValid())
	{
		IMG_AttackIncSign->SetVisibility(bAttackIncreased ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (IMG_DefenseIncSign.IsValid())
	{
		IMG_DefenseIncSign->SetVisibility(bDefenseIncreased ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (IMG_HealthIncSign.IsValid())
	{
		IMG_HealthIncSign->SetVisibility(bHealthIncreased ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	// 이번엔 열린 속성 옵션을 찾아냄
	int32 JustOpenedOptionIndex = -1;
	for (int32 POI = 0; POI < MAX_WING_PROP_OPTION_NUM; ++POI)
	{
		// 같은 인덱스에 같은 타입이 있긴 할 거임
		const FWingPropOption* CurrOptionOfIdx = EvolvedWing.GetPropOptionPtr(POI);
		if (CurrOptionOfIdx && CurrOptionOfIdx->bIsOpen)
		{ // 이번엔 열려 있는 타입의 옵션이 이전엔 열려 있지 않았는지.
			const FWingPropOption* PrevOptionOfSameType = PrevWing.GetPropOptionPtr(CurrOptionOfIdx->MyOptionType);
			if (PrevOptionOfSameType && !PrevOptionOfSameType->bIsOpen)
			{
				JustOpenedOptionIndex = POI;
				break;
			}
		}
	}
	
	const FWingPropOption* JustOpenedOption = (JustOpenedOptionIndex >= 0) ? EvolvedWing.GetPropOptionPtr(JustOpenedOptionIndex) : NULL;
	// 아마도 없지는 않겠지만 만에 하나 없다면 새로운 옵션 정보는 숨긴다.
	if (TB_PropOptionLabel.IsValid())
	{
		TB_PropOptionLabel->SetVisibility(JustOpenedOption ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (IMG_NewPropOptionIcon.IsValid())
	{
		IMG_NewPropOptionIcon->SetVisibility(JustOpenedOption ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (TB_NewPropOptionInfo.IsValid())
	{
		TB_NewPropOptionInfo->SetVisibility(JustOpenedOption ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	if (TB_NewPropOptionInfo2.IsValid())
	{
		TB_NewPropOptionInfo2->SetVisibility(JustOpenedOption ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (O_OpsionIcon.IsValid())
	{
		O_OpsionIcon->SetVisibility(JustOpenedOption ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (O_Complete.IsValid())
		O_Complete->SetVisibility(EvolvedWing.EvolutionGrade >= 8 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (TB_Complete.IsValid())
		TB_Complete->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Wing_EvolveCompletion")), FText::AsNumber(EvolvedWing.EvolutionGrade)));
	
	
	if (JustOpenedOption)
	{
		UB2WingInfo* WingInfoTable = StaticFindWingInfo();
		FWingPropOptionIconSetup* ThisOptionIconSetup = WingInfoTable ? WingInfoTable->GetPropOptionIconSetup(JustOpenedOption->MyOptionType) : NULL;

		if (IMG_NewPropOptionIcon.IsValid() && ThisOptionIconSetup)
		{
			IMG_NewPropOptionIcon->SetBrushFromMaterial(ThisOptionIconSetup->GetIconMtrl());
		}

		FText NewOpenWingPropOptionText = GetLOCTextOfWingPropOption(JustOpenedOption->MyOptionType, IntToPCClass(EvolvedWing.OwnerPCClass));
		if (TB_NewPropOptionInfo.IsValid())
		{
			TB_NewPropOptionInfo->SetText(
				FText::Format(
					BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_NewOpenPropOptionInfo")),
					NewOpenWingPropOptionText
				));
		}
		if (TB_NewPropOptionInfo2.IsValid())
		{
			TB_NewPropOptionInfo2->SetText(
				FText::Format(
					BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_NewOpenPropOptionInfo_Detail")),
					NewOpenWingPropOptionText,
					GetItemOptionValueDisplayText(JustOpenedOption->MyOptionType, JustOpenedOption->RawOptionAmount, true)
				));
		}
		
	}
}

void UB2LobbyUI_WingEvolveResult::CloseWidgetDelegate()
{
	OnClickedBtnConfirm();
}

void UB2LobbyUI_WingEvolveResult::OnClickedBtnConfirm()
{
	// 이 이벤트 받는 곳에서 이걸 닫고 메인 Wing 페이지로 전환할 것임.
	EndWingEvolutionSceneClass<>::GetInstance().Signal();
}

void UB2LobbyUI_WingEvolveResult::ShowBattleScoreNotify()
{
	ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Signal(CachedLobbyGM->GetHeroMgmtModePCClass());
	SetClose(true);
}