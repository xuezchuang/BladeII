// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIRaidRoulettePiece.h"


void UB2UIRaidRoulettePiece::Init()
{
	Super::Init();
}

void UB2UIRaidRoulettePiece::CacheAssets()
{
	GET_SLOT(UImage, IMG_Piece);
	GET_SLOT(UTextBlock, RTB_Percent);
	GET_SLOT(UCanvasPanel, CP_Percent);
	GET_SLOT(UScaleBox, SB_Percent);
}

void UB2UIRaidRoulettePiece::BindDelegates()
{

}

void UB2UIRaidRoulettePiece::SetPercent(const FLinearColor &Color, float Percent)
{
	if (Percent < 0.001f)
	{
		auto DynamicMat = IMG_Piece->GetDynamicMaterial();

		if (DynamicMat != nullptr)
			DynamicMat->SetScalarParameterValue(FName(TEXT("Percentage")), 0.0f);
		if (RTB_Percent.IsValid())
			RTB_Percent->SetText(FText::FromString(TEXT("")));
		return;
	}

	if (IMG_Piece.IsValid())
	{
		const float Margin = 0.01f;
		auto DynamicMat = IMG_Piece->GetDynamicMaterial();
		if (DynamicMat != nullptr)
		{
			DynamicMat->SetScalarParameterValue(FName(TEXT("Percentage")), Percent > 0.9f ? 1.0f : Percent + Margin);
			if (IMG_Piece.IsValid())
			{
				IMG_Piece->SetColorAndOpacity(Color);
			}
		}
	}

	if (RTB_Percent.IsValid())
	{
		int32 Num = FMath::RoundToInt(Percent * 100.0f);
		RTB_Percent->SetText(FText::Format(FText::FromString(TEXT("{0}%")), FText::AsNumber(Num)));
	}

	if (SB_Percent.IsValid())
	{
		FVector2D SBSize = Cast<UCanvasPanelSlot>(SB_Percent->Slot)->GetSize();

		if (Percent > 0.2f)
			SBSize.Y = 34.f;
		else if (Percent > 0.15f)
			SBSize.Y = 28.f;
		else if (Percent > 0.1f)
			SBSize.Y = 24.f;
		else 
			SBSize.Y = 16.f;
		Cast<UCanvasPanelSlot>(SB_Percent->Slot)->SetSize(FVector2D(SBSize.X, SBSize.Y));
	}
}

void UB2UIRaidRoulettePiece::SetAngle(float Angle, float Length)
{
	//SetRenderAngle(Angle);
	//if (CP_Percent.IsValid())
	//	CP_Percent->SetRenderAngle(Length * 0.5f);
}