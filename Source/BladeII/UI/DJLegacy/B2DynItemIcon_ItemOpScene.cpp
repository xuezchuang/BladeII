// Fill out your copyright notice in the Description page of Project Settings.
#include "B2DynItemIcon_ItemOpScene.h"
//#include "BladeII.h"


UB2DynItemIcon_ItemOpScene::UB2DynItemIcon_ItemOpScene(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemIconCommonPart_NRef = NULL;
	TotemIconCommonPart_NRef = NULL;
	IMG_TargetItemBlink_NRef = NULL;
	IMG_TargetItemFinalBlow_NRef = NULL;
	IMG_IngredItemFinalBlow_NRef = NULL;
	EnhanceTargetBlinkingMID = NULL;
	EnhanceTargetBlinkParamVaryingRate = 2.0f;
	EnhanceTargetBlinkMIDParam = 0.0f;
	TotalEnhanceIngredNum = 1;

	EnhanceTargetProgressiveBaseValue = 0.5f;
	EnhanceTargetProgressiveParam = 0.5f;
	EnhanceTargetProgressiveParamRate = 0.1f;
}

void UB2DynItemIcon_ItemOpScene::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateEnhanceTargetBlinkingMIDParam(InDeltaTime);
}

void UB2DynItemIcon_ItemOpScene::UpdateEnhanceTargetBlinkingMIDParam(float InDeltaTime)
{
	if (!EnhanceTargetBlinkingMID){
		return;
	}

	EnhanceTargetBlinkMIDParam = FMath::Max(EnhanceTargetBlinkMIDParam - EnhanceTargetBlinkParamVaryingRate * InDeltaTime, 0.0f);
	EnhanceTargetBlinkingMID->SetScalarParameterValue(FName(TEXT("FxEnhance_Alpha")), EnhanceTargetBlinkMIDParam);
}

void UB2DynItemIcon_ItemOpScene::ResetEnhanceTargetBlinkingMIDParam()
{
	EnhanceTargetBlinkMIDParam = 1.0f; // 0 捞 酒聪扼 1.0 栏肺 剁况辑 传俊 剁霸 窍绰 芭.
}

void UB2DynItemIcon_ItemOpScene::UpdateItemData(const FB2Item& InItem)
{
	Super::UpdateItemData(InItem); // Super 焊促绰 CommonPart 俊辑 贸府窍绰 霸 腹阑 巴烙.

	if (ItemIconCommonPart_NRef)
	{
		ItemIconCommonPart_NRef->UpdateItemData(InItem);
	}
}

void UB2DynItemIcon_ItemOpScene::UpdateTotemData(const FB2Totem& InTotem)
{
	if (TotemIconCommonPart_NRef)
	{
		TotemIconCommonPart_NRef->Init();
		TotemIconCommonPart_NRef->UpdateTotemSlot_FromFB2Totem(InTotem);
	}
}

void UB2DynItemIcon_ItemOpScene::SetIconUsageType(ELobbyInvenItemIconUsage InUsageType)
{
	Super::SetIconUsageType(InUsageType);

	if (ItemIconCommonPart_NRef)
	{
		ItemIconCommonPart_NRef->SetIconUsageType(InUsageType);
	}
}

void UB2DynItemIcon_ItemOpScene::OnIngredItemBeginSucking(int32 IngredIndex)
{

	OnIngredItemBeginSucking_BP(IngredIndex);
}

void UB2DynItemIcon_ItemOpScene::OnTargetItemBeginSucking(int32 IngredIndex)
{
	if (IngredIndex == 0)
	{
		// 盖 贸澜 矫累矫 EnhanceTargetProgressiveParam 阑 粱 技泼秦 淋.
		// 拱沸 TotalEnhanceIngredNum 捞 技泼登绢 乐绢具 窃.
		EnhanceTargetProgressiveParam = EnhanceTargetProgressiveBaseValue;
		// 付瘤阜 犁丰俊急 1 捞 登档废.
		EnhanceTargetProgressiveParamRate = (1.0f - EnhanceTargetProgressiveBaseValue) / FMath::Max(TotalEnhanceIngredNum, 1);
	}

	OnTargetItemBeginSucking_BP(IngredIndex);
}

void UB2DynItemIcon_ItemOpScene::OnTargetItemSuckedIngred(int32 IngredIndex)
{
	EnhanceTargetProgressiveParam = FMath::Min(1.0f, EnhanceTargetProgressiveParam + EnhanceTargetProgressiveParamRate);
	if (EnhanceTargetBlinkingMID)
	{
		EnhanceTargetBlinkingMID->SetScalarParameterValue(FName(TEXT("IntensityIncrease")), EnhanceTargetProgressiveParam);
	}

	OnTargetItemSuckedIngred_BP(IngredIndex);
}

void UB2DynItemIcon_ItemOpScene::OnTargetItemLastSucking()
{

	OnTargetItemLastSucking_BP();
}

void UB2DynItemIcon_ItemOpScene::SetShowOverlayInfoOnly(bool IsVisibleBGImg /*= false*/, bool IsVisibleGrade /*= false*/)
{
	if (ItemIconCommonPart_NRef)
	{
		ItemIconCommonPart_NRef->SetShowOverlayInfoOnly(IsVisibleBGImg, IsVisibleGrade);
	}
}

UMaterialInstanceConstant* UB2DynItemIcon_ItemOpScene::GetBGPanelMIC()
{
	return ItemIconCommonPart_NRef ? ItemIconCommonPart_NRef->GetBGPanelMIC() : NULL;
}

UMaterialInstanceConstant* UB2DynItemIcon_ItemOpScene::GetMainIconMIC()
{
	return ItemIconCommonPart_NRef ? ItemIconCommonPart_NRef->GetMainIconMIC() : NULL;
}

UMaterialInstanceConstant* UB2DynItemIcon_ItemOpScene::GetBGPanelMIC_Totem()
{
	return TotemIconCommonPart_NRef ? TotemIconCommonPart_NRef->GetBGPanelMIC() : NULL;
}

UMaterialInstanceConstant* UB2DynItemIcon_ItemOpScene::GetMainIconMIC_Totem()
{
	return TotemIconCommonPart_NRef ? TotemIconCommonPart_NRef->GetMainIconMIC() : NULL;
}