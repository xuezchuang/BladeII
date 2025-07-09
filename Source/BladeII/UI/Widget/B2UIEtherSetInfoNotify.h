// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIEtherSetInfoNotify.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEtherSetInfoNotify : public UB2UIWidget
{
	GENERATED_BODY()
public:
	void SetSetText(FText Text);
	virtual void Init() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Count")
		float EndTime;
	
protected:
	virtual void CacheAssets() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	float											PlayTime;
	TWeakObjectPtr<UTextBlock>						X_TB_SetInfo;

};
