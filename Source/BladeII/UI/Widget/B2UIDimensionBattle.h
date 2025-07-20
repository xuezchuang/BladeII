// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBattleMain.h"
#include "B2UIDimensionBattle.generated.h"


UCLASS()
class BLADEII_API UB2UIDimensionBattle : public UB2UIBattleMain
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void SetWidgetForDimensionBattle();

	UB2UIInGameBuffIcon_DimensionTower* CreateOrUpdateDimensionBuffIcon(EResurrectBuffType InBuffType);
	UB2UIInGameBuffIcon* CreateBuffIcon(TSubclassOf<UB2UIInGameBuffIcon> InCreateClass, UHorizontalBox* InToCreatePanel);
protected:
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSubclassOf<UB2UIInGameBuffIcon>							BuffIconClass_Dimension;

	TWeakObjectPtr<UTextBlock>									TB_DimensionStep;
	TArray<UB2UIInGameBuffIcon*>								AllMyBuffIcons_Dimension; // All elements created in HB_StageResurrectBuffIconSe
};
