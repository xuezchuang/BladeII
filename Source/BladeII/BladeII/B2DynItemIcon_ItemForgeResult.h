// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "../UI/B2RichTextBlock.h"
#include "../UI/B2UIEnum.h"
#include "B2DynItemIcon_ItemForgeResult.generated.h"

/**
 *
 */
UCLASS()
class BLADEII_API UB2DynItemIcon_ItemForgeResult : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void UpdateStaticText() override;


	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnShowingResultFail_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnShowingResultSuccess_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnShowingResultHugeSuccess_BP();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool IsSkipping();


	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnShowingResultAnimEnd();

public:
	//UB2DynItemIcon_ItemForgeResult(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;


	void SetOwnerItemForgeUI(class UB2UIItemForge* MainUI_, EItemForgeResultType resultType_, FB2Item item_,bool isSkipping, int32 actualQuantity=1);

protected:
	class UB2UIItemForge* ItemForgeMainUI;
	EItemForgeResultType resultType;

	bool beenSkipped;

private:
	TWeakObjectPtr<class UB2DynItemIcon_ItemForge> UIP_ResultItem;
	TWeakObjectPtr<UB2RichTextBlock> TB_SuccessResult;
};
