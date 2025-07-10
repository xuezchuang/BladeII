// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "B2SkillParameter.generated.h"

USTRUCT()
struct FB2SkillParameterValue
{
	GENERATED_USTRUCT_BODY()

public:

	FB2SkillParameterValue();
	~FB2SkillParameterValue() {}

	UPROPERTY(EditAnywhere)
		float RealValue;
	UPROPERTY(EditAnywhere)
		bool IsServerValue;
};

USTRUCT()
struct FB2SkillParameterContainer
{
	GENERATED_USTRUCT_BODY()

public:
	void InitContainer(float ServerValue);

	bool GetSkillParamValue(FName ParameterName, float & OutValue) const;

private:
	UPROPERTY(EditAnywhere)
		TMap<FName, FB2SkillParameterValue> SkillParams;
};

USTRUCT()
struct FB2SkillParameter
{
	GENERATED_USTRUCT_BODY()

public:
	FB2SkillParameter();
	~FB2SkillParameter() {}

	static void InitializeParameter(UObject* Owner, const FB2SkillParameterContainer& InParamContainer);

	FORCEINLINE float GetValue() { return Value; }
	FORCEINLINE bool IsSet() { return IsSetParam; };
	bool SetParameter(const FB2SkillParameterContainer& InParamContainer);

protected:
	UPROPERTY(EditAnywhere)
	FName ParameterKey;

	UPROPERTY(EditAnywhere)
	float Value;

	bool IsSetParam;
};

