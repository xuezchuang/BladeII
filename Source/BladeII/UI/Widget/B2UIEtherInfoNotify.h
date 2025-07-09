// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIEtherInfoNotify.generated.h"

/**
 * 
 */

UCLASS()
class BLADEII_API UB2UIEtherInfoNotify : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	void SetInfoNotify(FText InEtherSetText);
	void SetEndTime(float fEndTime) { EndTime = fEndTime; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Count")
		float EndTime;

protected:
	virtual void CacheAssets() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	float PlayTime;

	TWeakObjectPtr<UTextBlock> X_TB_SetInfo;
};
