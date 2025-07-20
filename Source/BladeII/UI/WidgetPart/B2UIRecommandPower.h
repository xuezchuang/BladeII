// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIRecommandPower.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIRecommandPower : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TArray<UMaterialInterface*>		Icons;

	void SetInfo(const int32 InCurrentAttack, const int32 InCurrentDefense, const int32 InRecommendAttack, const int32 InRecommendDefense);

protected:
	virtual void CacheAssets() override;

private:
	const int32 ESAY = 0;
	const int32 NORMAL = 1;
	const int32 HARD = 2;
	const int32 MAXSLOT = 3;
	
	TArray<TWeakObjectPtr<UB2Image>> Slots;
	TWeakObjectPtr<UCanvasPanel> X_CP_One;
	TWeakObjectPtr<UCanvasPanel> X_CP_Two;
	TWeakObjectPtr<UB2RichTextBlock> X_TB_OneText;
	TWeakObjectPtr<UB2RichTextBlock> X_TB_TwoText1;
	TWeakObjectPtr<UB2RichTextBlock> X_TB_TwoText2;
};
