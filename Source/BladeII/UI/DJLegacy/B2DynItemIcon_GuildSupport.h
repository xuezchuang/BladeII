// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/*#include "B2DynItemIcon.h"*/
#include "B2UIWidgetBase.h"
#include "B2DynItemIcon_GuildSupport.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2DynItemIcon_GuildSupport : public UB2UIWidgetBase
{
	GENERATED_BODY()
protected:
	TWeakObjectPtr<UImage> IMG_GradeBG;
	TWeakObjectPtr<UImage> MainIconImage;
	
	TArray<TWeakObjectPtr<UImage>> StarGradeImages;

	TWeakObjectPtr<UImage> IMG_ExtraSlotEffect;

	TWeakObjectPtr<UImage> EnhanceIngredSelectionImage_01;
	TWeakObjectPtr<UImage> EnhanceIngredSelectionImage_02;

	TWeakObjectPtr<UTextBlock> TB_ItemLevel;
	TWeakObjectPtr<UTextBlock> TB_EnhanceLevel;
	TWeakObjectPtr<UImage>	IMG_SelectFx;

public:
	UB2DynItemIcon_GuildSupport(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	void UpdateItemData(const FB2Item& InItem);

	void SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables);
	void SetBackgroundStarGrade(int32 InItemGrade, UMaterialInterface* StarMtrl);
	void SetSelectIcon(bool bIsSelected);
};
