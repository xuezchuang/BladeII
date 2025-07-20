// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIEnterStore.generated.h"


UCLASS()
class BLADEII_API UB2UIEnterStore : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;

	virtual void RegisterUIMarkForRedDot() override;
	virtual void UpdateStaticText()	override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	UFUNCTION()
	void OnClickBTN_Equip();
	UFUNCTION()
	void OnClickBTN_Goods();

	UFUNCTION()
		bool RedDotCondition_EquipStore();

	UFUNCTION()
		bool RedDotCondition_GoodsStore();

private:
	TWeakObjectPtr<UB2Button> BTN_Equip;
	TWeakObjectPtr<UB2Button> BTN_Goods;

	TWeakObjectPtr<UTextBlock>	TB_GoodsStore;
	TWeakObjectPtr<UTextBlock>	TB_SummonItem;
};
