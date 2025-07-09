// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UICostumeOpenBtn.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UICostumeOpenBtn : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void RegisterUIMarkForRedDot() final;
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

private:
	void SubscribeEvents();
	void UnSubscribeEvents();
	void UpdateVisibility(const bool& bIsOpen);
	
private:
	UFUNCTION()
	void OnClickBTN_CostumeOpen();

	UFUNCTION()
	bool HasRedDot_CostumeOpenBtn();

public:
	UFUNCTION(BlueprintCallable, Category = "B2UICostumeOpenBtn")
	void InitCostumeOpenBtn_BP();

private:
	TWeakObjectPtr<UB2Button>			BTN_CostumeOpen;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_CostumeOpen;

private:
	TArray<issue_ptr>					Issues;
};
