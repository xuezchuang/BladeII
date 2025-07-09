// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "B2UISelectiveLotteryCostumeDT.generated.h"

DECLARE_DELEGATE(FOnClickCostumeDetailClose);

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UISelectiveLotteryCostumeDT : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
  	virtual void CloseWidgetDelegate() override;

public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

 	class UB2UICollectBookDetailPart* CreateDetailPart(const FText& DetailPartTitle);

	void UpdateCostumeData(FB2Item& InItemInfo);

	void UpdateCostumeIcon();
	void UpdateCostumeSubWidget();

	void SetDetailViewCloseDelegate(const FOnClickCostumeDetailClose& Delegate);

private:
 	void DestroyDetailParts();

protected:
	FB2Item ChachedItem;

	TArray<class UB2UICollectBookDetailPart*> DetailPartWidgets;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	TSubclassOf<class UB2UICollectBookDetailPart> DetailPartClass;

	FOnClickCostumeDetailClose DetailCloseDelegate;

protected:
	UFUNCTION()
		void OnClickBTN_Close();

protected:
	TWeakObjectPtr<UVerticalBox> VB_DetailParts;

	TWeakObjectPtr<class UB2UICollectBookIcon> UIP_CollectBookIcon;

	TWeakObjectPtr<UTextBlock> TB_PrimaryOptionName;
	TWeakObjectPtr<UTextBlock> TB_PrimaryOptionValue;
	TWeakObjectPtr<UTextBlock> TB_CombatName;
	TWeakObjectPtr<UTextBlock> TB_CombatValue;

	TWeakObjectPtr<UTextBlock>		TB_ItemName;
	TWeakObjectPtr<UB2Button>		BTN_Close;
};
