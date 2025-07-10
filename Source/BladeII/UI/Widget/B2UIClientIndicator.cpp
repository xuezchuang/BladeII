// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIClientIndicator.h"



void UB2UIClientIndicator::IsActive(bool InValue)
{

}

void UB2UIClientIndicator::Init()
{
	Super::Init();
	PlayIndicatorAnimation();
}


float GetBackOffTime(float RequestTime, float ResponseTime)
{
	float Result = ResponseTime - RequestTime;

	if (Result < 0.1f)
	{
        // min 100ms
        Result = 0.1f;
	}
    else if (Result > 0.5f)
    {
        // max 500ms
        Result = 0.5f;
    }

	return Result;
}
