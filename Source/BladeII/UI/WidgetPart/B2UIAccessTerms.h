// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
//#include "BladeII.h"
#include "B2UIDLCFrontBase.h"
#include "B2UIAccessTerms.generated.h"

enum class EdateType : uint8
{
	EdateType_Year = 0,
	EdateType_Month,
	EdateType_Day,
	
	max
};

UCLASS()
class BLADEII_API UB2UIAccessTerms : public UB2UIDLCFrontBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "DLCFront")
		void GotoTitleScreen();
	UFUNCTION(BlueprintCallable, Category = "DLCFront")
		FString SetDateString(int32 nYear, int32 nMonth, int32 nDay);

protected:

private:
//	TWeakObjectPtr<UB2Image> B2Image_0;
};
