// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_BrevetNodeIcon.h"
//#include "BladeII.h"
#include "B2UIManager.h"



UB2LobbyUI_BrevetNodeIcon::UB2LobbyUI_BrevetNodeIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_eBrevetNodeState = EBrevetNodeState::EBNS_None;
	m_pBrevetNodeInfo = NULL;
	m_nBrevetNodeIndex = 0;
	m_fGaugeProgress = 0.0f;
}

void UB2LobbyUI_BrevetNodeIcon::InitBrevetNodeIcon(UB2LobbyUI_BrevetMain* pOwnerUI, const FBrevetNodeInfo* BrevetNodeInfo, EBrevetNodeState BrevetNodeState)
{
	m_eBrevetNodeState = BrevetNodeState;
	m_pBrevetNodeInfo = BrevetNodeInfo;
	m_pOwnerUI = pOwnerUI;

	// 텍스트 설정
	if (m_pBrevetNodeInfo)
	{
		// 앞노드 활성화 필요 안보이게일단
		if (TB_BeforeNodeNum.IsValid())
			TB_BeforeNodeNum->SetVisibility(ESlateVisibility::Hidden);

		if (TB_ActiveOpen.IsValid())
			TB_ActiveOpen->SetVisibility(ESlateVisibility::Hidden);

		// 능력치 획득
		if (TB_GetStatusType.IsValid())
		{
			switch (static_cast<EBrevetStatusType>(m_pBrevetNodeInfo->StatusType))
			{
			case EBrevetStatusType::EBST_Atk:
				TB_GetStatusType->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_FormatReceive")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Attack"))));
				break;
			case EBrevetStatusType::EBST_Def:
				TB_GetStatusType->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_FormatReceive")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Defense"))));
				break;
			case EBrevetStatusType::EBST_Hp:
				TB_GetStatusType->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_FormatReceive")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Health"))));
				break;
			}
		}

		// 획득 값
		if (TB_GetStatusValue.IsValid())
			TB_GetStatusValue->SetText(FText::FromString(TEXT("+") + FString::FromInt(m_pBrevetNodeInfo->StatusValue)));

		// 능력치
		if (TB_StatusType.IsValid())
		{
			switch (static_cast<EBrevetStatusType>(m_pBrevetNodeInfo->StatusType))
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

		// 능력치 값
		if (TB_StatusValue.IsValid())
			TB_StatusValue->SetText(FText::FromString(TEXT("+") + FString::FromInt(m_pBrevetNodeInfo->StatusValue)));

		// 확률 값 
		SetSuccessRateText(m_pBrevetNodeInfo->Rate);		
	}

	switch (m_eBrevetNodeState)
	{
	case EBrevetNodeState::EBNS_Locked_Current:
		PlayAnimationEvent_St_Current_Lock();
		break;
	case EBrevetNodeState::EBNS_Locked:
		PlayAnimationEvent_St_Next_Lock();
		break;
	case EBrevetNodeState::EBNS_UnLocked:
		PlayAnimationEvent_St_Current_Open();
		break;
	case EBrevetNodeState::EBNS_Complete:
		PlayAnimationEvent_St_Current_Close();
		break;
	}

	// 오픈 애니메이션 재생 필요한지 체크
	if (m_eBrevetNodeState == EBrevetNodeState::EBNS_UnLocked && m_pOwnerUI)
	{
		if (m_pOwnerUI->CheckNeedNodeOpenAnimation())
			PlayAnimationEvent_Anim_Current_Open();
	}

	//StaticText
	UpdateStaticText();
}

void UB2LobbyUI_BrevetNodeIcon::SetIconImage(UMaterialInterface* pMI)
{
	if (pMI && IMG_StatusIcon.IsValid() && IMG_StatusIcon.IsValid())
		IMG_StatusIcon->SetBrushFromMaterial(pMI);
}

void UB2LobbyUI_BrevetNodeIcon::SetIconLightImage(UMaterialInterface* pMI)
{
	if (pMI && IMG_StatusIcon_Activate.IsValid() && IMG_StatusIcon.IsValid())
		IMG_StatusIcon_Activate->SetBrushFromMaterial(pMI);
}

void UB2LobbyUI_BrevetNodeIcon::SetProgressImage(UMaterialInterface* pMI)
{
	if (pMI && IMG_Progress.IsValid())
		IMG_Progress->SetBrushFromMaterial(pMI);
}

void UB2LobbyUI_BrevetNodeIcon::SetProgressBackImage(UMaterialInterface* pMI)
{
	if (pMI && IMG_ProgressBack.IsValid())
		IMG_ProgressBack->SetBrushFromMaterial(pMI);
}

void UB2LobbyUI_BrevetNodeIcon::SetProgressGauge(float fProgress)
{
	m_fGaugeProgress = fProgress;

	if (IMG_Progress.IsValid())
	{
		UMaterialInstanceDynamic* DynamicMI = IMG_Progress->GetDynamicMaterial();

		if (DynamicMI)
			DynamicMI->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, fProgress);
	}
}

float UB2LobbyUI_BrevetNodeIcon::GetProgressGauge()
{
	return m_fGaugeProgress;
}

void UB2LobbyUI_BrevetNodeIcon::UpdateStaticText()
{
	if (TB_Completed.IsValid())
		TB_Completed->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_UpgradeComplete")));

	if(TB_Success.IsValid())
		TB_Success->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_Rate")));
}

void UB2LobbyUI_BrevetNodeIcon::SetBreverNodeNum(int32 NewNum)
{
	m_nBrevetNodeIndex = NewNum;
}

int32 UB2LobbyUI_BrevetNodeIcon::GetBrevetNodeNum()
{
	return m_nBrevetNodeIndex;
}

void UB2LobbyUI_BrevetNodeIcon::SetSuccessRateText(int32 nSuccessRate, bool bIsComplete)
{
	if (TB_RateText.IsValid() == false)
		return;

	TB_RateText->SetText(bIsComplete ? (BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BrevetActivable"))) : 
										FText::FromString(FString::FromInt((int32)(nSuccessRate/100)) + TEXT("%")));
}

void UB2LobbyUI_BrevetNodeIcon::SetBrevetNodeState(EBrevetNodeState BrevetNodeState)
{
	m_eBrevetNodeState = BrevetNodeState;
}

EBrevetNodeState UB2LobbyUI_BrevetNodeIcon::GetBrevetNodeState()
{
	return m_eBrevetNodeState;
}

void UB2LobbyUI_BrevetNodeIcon::SetNotOpenText(int32 nNodeNum)
{
	// 클리어 스테이지
	if (TB_BeforeNodeNum.IsValid())
	{
		TB_BeforeNodeNum->SetVisibility(ESlateVisibility::Visible);
		TB_BeforeNodeNum->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("GradeNProgress")), FText::AsNumber(nNodeNum)));
	}

	if (TB_ActiveOpen.IsValid())
		TB_ActiveOpen->SetVisibility(ESlateVisibility::Visible);
}

void UB2LobbyUI_BrevetNodeIcon::OnSelectBrevetNode()
{
	if (m_nBrevetNodeIndex == m_pOwnerUI->GetCurrentBrevetNodeIndex())
		m_pOwnerUI->OnSelectedCurrentBrevetNode();
}

void UB2LobbyUI_BrevetNodeIcon::CacheAssets()
{
	Super::CacheAssets();

	this->SetVisibility(ESlateVisibility::Visible);

	GET_SLOT(UImage, IMG_StatusIcon);
	GET_SLOT(UImage, IMG_StatusIcon_Activate);
	GET_SLOT(UImage, IMG_Progress);
	GET_SLOT(UImage, IMG_ProgressBack);
	GET_SLOT(UB2Button, BTN_Main);
	GET_SLOT(UTextBlock, TB_BeforeNodeNum);
	GET_SLOT(UTextBlock, TB_ActiveOpen);
	GET_SLOT(UTextBlock, TB_GetStatusType);
	GET_SLOT(UTextBlock, TB_GetStatusValue);
	GET_SLOT(UTextBlock, TB_StatusType);
	GET_SLOT(UTextBlock, TB_StatusValue);
	GET_SLOT(UTextBlock, TB_RateText);

	GET_SLOT(UTextBlock, TB_Completed);
	GET_SLOT(UTextBlock, TB_Success);

	BIND_CLICK_FUNC_TO_BTN(BTN_Main, &UB2LobbyUI_BrevetNodeIcon::OnSelectBrevetNode);	
}

void UB2LobbyUI_BrevetNodeIcon::OnFinishAnimationEvent_Anim_Current_Close()
{
	if (m_pOwnerUI)
		m_pOwnerUI->OnFinishAnimationEvent_Anim_Current_Close();
}

void UB2LobbyUI_BrevetNodeIcon::OnFinishAnimationEvent_Anim_UnLock_BP()
{
	if (m_pOwnerUI)
		m_pOwnerUI->OnFinishAnimationEvent_Anim_UnLock_BP();
}

void UB2LobbyUI_BrevetNodeIcon::OnFinishAnimationEvent_Anim_FailNotice_BP()
{
	if (m_pOwnerUI)
		m_pOwnerUI->PlayAnimationEvent_FailPopup_BP();
}
