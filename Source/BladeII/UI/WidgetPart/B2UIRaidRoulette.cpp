// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRaidRoulette.h"
#include "B2UIRaidRoulettePiece.h"

void UB2UIRaidRoulette::Init()
{
	Super::Init();
}

void UB2UIRaidRoulette::CacheAssets()
{
	GET_SLOT(UCanvasPanel, CP_Root);
	GET_SLOT(UImage, IMG_Arrow);
	
	const int32 MaxPiece = 4;
	PieceArray.SetNum(MaxPiece);
	GET_SLOT_BYNAME(UB2UIRaidRoulettePiece, PieceArray[0], Piece0);
	GET_SLOT_BYNAME(UB2UIRaidRoulettePiece, PieceArray[1], Piece1);
	GET_SLOT_BYNAME(UB2UIRaidRoulettePiece, PieceArray[2], Piece2);
	GET_SLOT_BYNAME(UB2UIRaidRoulettePiece, PieceArray[3], Piece3);

	for (int32 i = 0; i < PieceArray.Num(); ++i)
	{
		PieceArray[i]->Init();
	}


}

void UB2UIRaidRoulette::BindDelegates()
{
	
}

void UB2UIRaidRoulette::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SpinRoulette(InDeltaTime);
}

void UB2UIRaidRoulette::SpinRoulette(float InDeltaTime)
{
	static float Testa = 0;
	if (RouletteState == ERouletteState::None)
		return;

	RotateSpeed = MaxRotateSpeed;

	ProgressTime += InDeltaTime;
	RotatedAngle += //RotateSpeed * InDeltaTime;
	RotateSpeed * Curve->GetFloatValue(ProgressTime) * InDeltaTime;
	//CP_Root->SetRenderAngle(RotatedAngle);

	switch (RouletteState)
	{
	case UB2UIRaidRoulette::ERouletteState::Rotating:
	{
		const float PlayTime = 2.7f;
		if (ProgressTime >= PlayTime)
		{
			RouletteState = ERouletteState::RremainingTarget;
		}
	}
	break;
	case UB2UIRaidRoulette::ERouletteState::RremainingTarget:
	{
		int32 ParseRotatedAngle = static_cast<int32>(RotatedAngle) % 360;
		if (ParseRotatedAngle >= TargetAngle)
		{
			RouletteState = ERouletteState::None;
			//CP_Root->SetRenderAngle(TargetAngle);
			UE_LOG(LogBladeII, Log, TEXT("Spin End : %f"), ProgressTime);
			
			RouletteSpinDelegate.ExecuteIfBound();
		}
	}
	break;
	}
}

void UB2UIRaidRoulette::StartRoulette()
{
	RouletteState = ERouletteState::Rotating;

	PlayRouletteSound_BP();
}

void UB2UIRaidRoulette::SetRoulette(const TArray<FRaidRouletteInfo> &InfoArray, int32 TargetIndex)
{
	if (TargetIndex < 0)
		TargetIndex = 0;
	
	CurrentTargetIndex = TargetIndex;

	float AccumulatedAngle = 0.0f;
	
	RoulettePercents.Empty();
	int32 MaxPiece = 4;
	for (int32 i = 0; i < MaxPiece; ++i)
	{
		if (i > InfoArray.Num() - 1)
		{
			PieceArray[i]->SetPercent(InfoArray[0].PieceColor, 0.0f);
			PieceArray[i]->SetAngle(0.0f, 0.0f);

			RoulettePercents.Add(0.0f);
		}
		else
		{
			float Percent = InfoArray[i].Contribution > 0 ? InfoArray[i].Contribution / 100.0f : 0.0f;
			float Angle = Degree360 * Percent;

			PieceArray[i]->SetPercent(InfoArray[i].PieceColor, Percent);
			PieceArray[i]->SetAngle(AccumulatedAngle, Angle);
			AccumulatedAngle += Angle;

			RoulettePercents.Add(Percent);
		}
	}
	
	RotatedAngle = 0.0f;
	//SetRenderAngle(RotatedAngle);
	ProgressTime = 0.0f;

	TargetAngle = GetAngleRange(TargetIndex);
}

void  UB2UIRaidRoulette::SkipRoulette()
{
	RouletteState = ERouletteState::RremainingTarget;
}

float UB2UIRaidRoulette::GetAccumulatedAngle(const int32 Index)
{
	float AccumulatedAngle = 0.0f;
	for (int32 i = 0; i <= Index; ++i)
	{
		AccumulatedAngle += Degree360 * RoulettePercents[i];
	}

	return AccumulatedAngle;
}

float UB2UIRaidRoulette::GetAngleRange(const int32 Index)
{
	float ResultAngle = 0.0f;
	float AccumulatedAngle = GetAccumulatedAngle(Index);

	float Margin = 0.2f;
	if (AccumulatedAngle < 45.f)
		Margin = 0.4f;

	float LeftSide = 0.f;
	float RightSide = 0.f;
	if (Index == 0)
	{
		float Angle = (RoulettePercents[Index] * Degree360);

		LeftSide = 0;
		RightSide = Angle;
	}
	else
	{
		LeftSide = AccumulatedAngle - (RoulettePercents[Index] * Degree360);
		RightSide = AccumulatedAngle;
	}

	float Dist = RightSide - LeftSide;
	if (Dist > 0.f)
	{
		LeftSide += Dist * Margin;
		RightSide -= Dist * Margin;
	}
	ResultAngle = FMath::RandRange(LeftSide, RightSide);

	//UE_LOG(LogBladeII, Log, TEXT("@@ :Index %d == %f <> %f == AccumulatedAngle %f - %f = Margin %f"), Index, LeftSide, RightSide, AccumulatedAngle, (AccumulatedAngle - (RoulettePercents[Index] * Degree360)), Margin);

	float CurAngle = (float)((int32)RotatedAngle % (int32)Degree360);
	ResultAngle = RotatedAngle + (Degree360 - CurAngle) - (ResultAngle);
	return ResultAngle;
}
