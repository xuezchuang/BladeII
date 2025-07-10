#include "B2UIBattleSkillUnity.h"
#include "BladeIIUtil.h"


UB2UIBattleSkillUnity::UB2UIBattleSkillUnity(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GageChangeAllowAmountBySecound = 5.0f;

	m_fMaxPointStack = 0;
	m_fCurPointStack = 0.0f;

	m_fPointStackRateForUI = 0.0f;
}

void UB2UIBattleSkillUnity::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateSkillPointStackUI(InDeltaTime);
}

void UB2UIBattleSkillUnity::SetSkillPointStack(float MaxPoint, float CurPoint)
{
	m_fMaxPointStack = MaxPoint;
	m_fCurPointStack = CurPoint;
}

void UB2UIBattleSkillUnity::UpdateSkillPointStackUI(float InDeltaTime)
{
	UMaterialInstanceDynamic* pPointStackMI = nullptr;

	if (m_fMaxPointStack == 0.0f)
	{
		if (IMG_PointStack.IsValid())
		{
			pPointStackMI = IMG_PointStack->GetDynamicMaterial();
			if (pPointStackMI)
			{
				pPointStackMI->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, 0.0f);
			}
		}

		return;
	}

	// 실제 포인트에따른 게이지량
	float fGageRate = m_fCurPointStack / m_fMaxPointStack;
	
	fGageRate = FMath::Clamp(fGageRate, 0.0f, 1.0f);

	// 이번틱 가능변화량
	float fMaxChaangeAmount = (InDeltaTime * GageChangeAllowAmountBySecound) / m_fMaxPointStack;

	// UI용 fGageRate를 쫒아간다.
	m_fPointStackRateForUI = FMath::Clamp(fGageRate, m_fPointStackRateForUI - fMaxChaangeAmount, m_fPointStackRateForUI + fMaxChaangeAmount);

	// 뺄때는 한번에 빼줘본다.
	if (fGageRate < m_fPointStackRateForUI)
		m_fPointStackRateForUI = fGageRate;
	
	if (IMG_PointStack.IsValid())
	{
		IMG_PointStack->SetVisibility(m_fPointStackRateForUI > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		
		pPointStackMI = IMG_PointStack->GetDynamicMaterial();
		if (pPointStackMI)
		{
			pPointStackMI->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, m_fPointStackRateForUI);
		}
	}
	
	bool bFullGage = m_fPointStackRateForUI >= 1.0f;

	SetEnabledBtn(bFullGage && !IsInCooltime());

	if (bFullGage && !IsInCooltime())
	{
		if (!bFullStackSkillPoint)
		{
			bFullStackSkillPoint = true;
			PlayFullStackSkillPoint();
		}
	}
	else
	{
		bFullStackSkillPoint = false;
		StopFullStackSkillPoint();
	}
}
