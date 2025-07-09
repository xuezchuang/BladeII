// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIEquipLinkBtn.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEquipLinkBtn : public UB2UIWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void RegisterUIMarkForRedDot() final;

private:
	UFUNCTION()
	void OnClickBTN_Equip();

	UFUNCTION()
	bool RedDotCondition_Equip();

public:
	UFUNCTION(BlueprintCallable, Category = "B2UIEquipLinkBtn")
	void InitEquipLinkBtn_BP();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	int32 OpenConditionTutorialID;

private:
	TWeakObjectPtr<UB2Button>			BTN_LinkEquip;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_LinkEquipBtn;

	TWeakObjectPtr<UPanelWidget>		X_Parent;
};
