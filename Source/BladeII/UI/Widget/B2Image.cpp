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
		//// �̰��� ColorAndOpacity �� Animation �Ѵٴ� �����Ͽ� ����ϴ� ��. Animation ���߿� �ش� �������� �� ������ ���� ���õȴ�.
		//// �׷��� Animation �� ���� ���õ� ���� MID �Ķ���ͷ� �����ϰ� ColorAndOpacity ���� ���� 1.0 ����.
		//// ColorToMIDParamMap_* �� ���õ� ä�ο� ���ؼ���.
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