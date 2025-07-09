// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "B2Airport.h"
#include "B2WingUIP_EvolveGradeStar.generated.h"

/**
 * A simple UIP class displaying star image according to wing's evolve grade. Might have to display some effect though..
 */

USTRUCT(BlueprintType)
struct FOverStarGrade
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterialInterface*	FilledStarImageMtrl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterialInterface*	EmptyStarImageMtrl;
};

UCLASS()
class BLADEII_API UB2WingUIP_EvolveGradeStar : public UBladeIIUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* FilledStarImageMtrl;
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* EmptyStarImageMtrl;

	UPROPERTY(EditAnywhere)
		TArray<FOverStarGrade>		StarGrade;
	
	UPROPERTY()
	TArray<UImage*> AllStarImages;

	UPROPERTY(EditAnywhere)
	FString StarImagePrefix;

	TArray<TWeakObjectPtr<UPanelWidget>> AllStarScale;

	TWeakObjectPtr<UImage>			IMG_GradeNum;
	TWeakObjectPtr<UPanelWidget>	SB_GradeStarNum;
	TWeakObjectPtr<UPanelWidget>	HB_StarGradeBase;
public:

	UB2WingUIP_EvolveGradeStar(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	void SetCurrentGrade(int32 InGrade, bool bInShowNextGradeExpectation = false);
};

