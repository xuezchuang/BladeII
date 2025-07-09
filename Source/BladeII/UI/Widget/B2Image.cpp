// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2Image.h"

#define LOCTEXT_NAMESPACE "UMG"

UB2Image::UB2Image(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bMapColorToMIDParam = false;
	ColorToMIDParamMap_R = NAME_None;
	ColorToMIDParamMap_G = NAME_None;
	ColorToMIDParamMap_B = NAME_None;
	ColorToMIDParamMap_A = NAME_None;
}

void UB2Image::TransferColorToMIDParam()
{
	UMaterialInstanceDynamic* BrushMID = GetDynamicMaterial();
	if (BrushMID)
	{
		//// 이것은 ColorAndOpacity 를 Animation 한다는 가정하에 사용하는 것. Animation 도중에 해당 변수값은 매 프레임 새로 세팅된다.
		//// 그렇게 Animation 을 통해 세팅된 값을 MID 파라미터로 전달하고 ColorAndOpacity 본래 값은 1.0 으로.
		//// ColorToMIDParamMap_* 이 세팅된 채널에 한해서만.
		//if (ColorToMIDParamMap_R != NAME_None)
		//{
		//	BrushMID->SetScalarParameterValue(ColorToMIDParamMap_R, GetColorAndOpacity().R);
		//	ColorAndOpacity.R = 1.0f;
		//}
		//if (ColorToMIDParamMap_G != NAME_None)
		//{
		//	BrushMID->SetScalarParameterValue(ColorToMIDParamMap_G, GetColorAndOpacity().G);
		//	ColorAndOpacity.G = 1.0f;
		//}
		//if (ColorToMIDParamMap_B != NAME_None)
		//{
		//	BrushMID->SetScalarParameterValue(ColorToMIDParamMap_B, GetColorAndOpacity().B);
		//	ColorAndOpacity.B = 1.0f;
		//}
		//if (ColorToMIDParamMap_A != NAME_None)
		//{
		//	BrushMID->SetScalarParameterValue(ColorToMIDParamMap_A, GetColorAndOpacity().A);
		//	ColorAndOpacity.A = 1.0f;
		//}
		//SetColorAndOpacity(ColorAndOpacity());
	}
}

#if WITH_EDITOR
const FText UB2Image::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common"); // Original UImage palette category
}
#endif

#undef LOCTEXT_NAMESPACE