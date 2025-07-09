// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIBrevetAddStatus.h"

void UB2UIBrevetAddStatus::InitBrevetAddStatus(UB2LobbyUI_BrevetMain* pOwnerUI, const FBrevetNodeInfo* BrevetNodeInfo, UMaterialInterface* pGaugeBackMI, UMaterialInterface* pGaugeMI, UMaterialInterface* pIconMI, EPCClass OwnerClass)
{
	if (!pOwnerUI || !BrevetNodeInfo || !pGaugeMI || !pGaugeBackMI || !pIconMI)
		return;

	m_pOwnerUI = pOwnerUI;
	CurrentOwnerClass = OwnerClass;

	if (TB_StatusType.IsValid())
	{
		switch (static_cast<EBrevetStatusType>(BrevetNodeInfo->StatusType))
		{
		case EBrevetStatusType::EBST_Atk:
			TB_StatusType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Attack")));
			break;
		case EBrevetStatusType::EBST_Def:
			TB_StatusType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Defense")));
			break;
		case EBrevetStatusType::EBST_Hp:
			TB_StatusType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Health")));
			break;
		}
	}

	if (TB_StatusValue.IsValid())
		TB_StatusValue->SetText(FText::FromString(TEXT("+") + FString::FromInt(BrevetNodeInfo->StatusValue)));

	if (IMG_GaugeBg.IsValid() && IMG_Gauge.IsValid() && IMG_Icon.IsValid())
	{
		IMG_GaugeBg->SetBrushFromMaterial(pGaugeBackMI);
		IMG_Gauge->SetBrushFromMaterial(pGaugeMI);
		IMG_Icon->SetBrushFromMaterial(pIconMI);
	}

	// 게이지 설정..
	if (IMG_Gauge.IsValid())
	{
		UMaterialInstanceDynamic* DynamicMI = IMG_Gauge->GetDynamicMaterial();

		if (DynamicMI)
			DynamicMI->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, 1.0f);
	}

	PlayAnimationEvent_BrevetAddStatusInitAnimation();
}

void UB2UIBrevetAddStatus::CacheAssets()
{	
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_GaugeBg);
	GET_SLOT(UImage, IMG_Gauge);
	GET_SLOT(UImage, IMG_Icon);

	GET_SLOT(UTextBlock, TB_StatusType);
	GET_SLOT(UTextBlock, TB_StatusValue);
}

void UB2UIBrevetAddStatus::OnFinishAnimationEvent_BrevetAddStatusInitAnimation()
{
	m_pOwnerUI->OnFinishAnimationEvent_BrevetAddStatusInitAnimation();
	ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Signal(CurrentOwnerClass);
	DestroySelf(UB2UIManager::GetInstance());
}
