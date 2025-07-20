
#include "B2SkillParameter.h"

FB2SkillParameter::FB2SkillParameter()
{
	ParameterKey = NAME_None;
	Value = 0.0f;
	IsSetParam = false;
}

void FB2SkillParameter::InitializeParameter(UObject* Owner, const FB2SkillParameterContainer& InParamContainer)
{
	if (Owner == nullptr)
		return;

	for (TFieldIterator<FProperty> PropIt(Owner->GetClass()); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		if (FStructProperty *p = Cast<FStructProperty>(Property))
		{
			FB2SkillParameter *SkillParam = p->ContainerPtrToValuePtr<FB2SkillParameter>(Owner);
			if (SkillParam)
			{
				SkillParam->SetParameter(InParamContainer);
			}
		}
	}
}

bool FB2SkillParameter::SetParameter(const FB2SkillParameterContainer& InParamContainer)
{
	float OutValue = 0.0f;
	if (!InParamContainer.GetSkillParamValue(ParameterKey, OutValue))
		return false;

	Value = OutValue;
	IsSetParam = true;
	return true;
}

FB2SkillParameterValue::FB2SkillParameterValue()
{
	IsServerValue = false;
}

void FB2SkillParameterContainer::InitContainer(float ServerValue)
{
	for (auto& Param : SkillParams)
	{
		if(Param.Value.IsServerValue)
			Param.Value.RealValue = ServerValue;
	}
}

bool FB2SkillParameterContainer::GetSkillParamValue(FName ParameterName, float & OutValue) const
{
	if (SkillParams.Contains(ParameterName))
	{
		OutValue = SkillParams[ParameterName].RealValue;
		return true;
	}
	return false;
}
