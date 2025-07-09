// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIItemForgeGroupTitle.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIItemForgeGroupTitle : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void UpdateStaticText() override;

	UFUNCTION()
		void OnClickedBTNInfo();

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetOwnerItemForgeUI(class UB2UIItemForge* OwnerUI, int32 groupid, bool isIngredient = false, bool thisIsMainIngred = false, bool isItForge=false);

protected:
	class UB2UIItemForge* ItemForgeMainUI;

private:
	TWeakObjectPtr<UB2Button> BTN_Info;

	TWeakObjectPtr<UTextBlock> TB_GroupTitle;
	TWeakObjectPtr<UTextBlock> TB_IngredientTitle;

	TWeakObjectPtr<UOverlay> O_Group;
	TWeakObjectPtr<UOverlay> O_Ingredient;

private:
	int32 GroupID;
	bool isForgeSection;
};
