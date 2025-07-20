// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIFairyLinkButton.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFairyLinkButton : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void UpdateStaticText() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
protected:
	UFUNCTION()
	void OnClickBTN_Fairy();

	UFUNCTION()
	bool RedDotCondition_Fairy();

	virtual void RegisterUIMarkForRedDot() final;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	int32 OpenConditionTutorialID;

private:
	TWeakObjectPtr<UB2Button> BTN_LinkFairy;
	TWeakObjectPtr<UTextBlock> STB_LinkFairy;

	TWeakObjectPtr<UPanelWidget> X_Parent;
};
