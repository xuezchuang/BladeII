// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UICharacterSelectPage.h"
#include "B2UIDimensionSelectPage.generated.h"


UCLASS()
class BLADEII_API UB2UIDimensionSelectPage : public UB2UICharacterSelectPage
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void BattleStart() override;
	virtual void SetChangeCombatPower(int32 InPower, int32 InAttack, int32 InDefense) override;
	virtual void UpdateStaticText() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;

protected:
private:
	TWeakObjectPtr<UTextBlock> STB_Hero;
	TWeakObjectPtr<UTextBlock> STB_Recommend;
	TWeakObjectPtr<UTextBlock> TB_HeroPower;
	TWeakObjectPtr<UTextBlock> TB_RecommendPower;

	TWeakObjectPtr<class UB2UIRecommandPower> UIP_RecommendPower;
};
