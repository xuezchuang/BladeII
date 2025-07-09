// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIWealthPopupBtn.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIWealthPopupBtn : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	UFUNCTION()
	void OnClickBTN_WealthPopupOpen();

public:
	UFUNCTION(BlueprintCallable, Category = "B2UIWealthPopupBtn")
	void InitWealthPopupBtn_BP();

	bool IsVisibleWealthPopupBtn();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "B2UIWealthPopupBtn")
	TSubclassOf<class UB2UIWealthPopup> UIWealthPopupClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WealthPopupZOrder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<EUIScene>	ArrayIsVisibleUIScene;
	
private:
	TWeakObjectPtr<UB2Button>	BTN_WealthPopupOpen;
};
