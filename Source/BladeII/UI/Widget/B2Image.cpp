// Fill out your copyright notice in the Description page of Project Settings.


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
		//// 捞巴篮 ColorAndOpacity 甫 Animation 茄促绰 啊沥窍俊 荤侩窍绰 巴. Animation 档吝俊 秦寸 函荐蔼篮 概 橇饭烙 货肺 技泼等促.
		//// 弊犯霸 Animation 阑 烹秦 技泼等 蔼阑 MID 颇扼固磐肺 傈崔窍绊 ColorAndOpacity 夯贰 蔼篮 1.0 栏肺.
		//// ColorToMIDParamMap_* 捞 技泼等 盲澄俊 茄秦辑父.
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