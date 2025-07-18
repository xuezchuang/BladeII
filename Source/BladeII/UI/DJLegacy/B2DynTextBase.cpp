// Fill out your copyright notice in the Description page of Project Settings.
#include "B2DynTextBase.h"
//#include "BladeII.h"



UB2DynTextBase::UB2DynTextBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ModColor_EvenRow = FLinearColor(1.0f, 1.0f, 1.0f);
	ModColor_OddRow = FLinearColor(0.7f, 0.7f, 0.7f);
}

void UB2DynTextBase::CacheAssets()
{
	GET_SLOT(UImage, IMG_BG)
}

void UB2DynTextBase::ModulateBGImageToRowIndex(int32 RowIndex)
{
	if (IMG_BG.IsValid())
	{ // 줄 구분 쉽게 하기 위한 컬러 넣기
		IMG_BG->SetColorAndOpacity((RowIndex % 2 == 0) ? ModColor_EvenRow : ModColor_OddRow);
	}
}