#pragma once

#include "B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIStoreProductCost.generated.h"

USTRUCT(BlueprintType)
struct FB2StoreCostIconData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "BladeII")
	EStoreItemCost Type;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	UMaterialInstance* Icon;
};

UCLASS(Blueprintable, BlueprintType)
class UB2UIStoreProductCost : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category="BladeII")
	void SetCost(EStoreItemCost InCostType, int32 InCost, int32 InCalculatedCost);

	void SetCashCost(float InCost, FString InCurrencyCode);
	void SetSummonDesc(int32 ButtonType);

	void SetSealCosnt(ESealCostType InCostType, int32 InCost);

protected:
	virtual void CacheAssets() override;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	TArray<FB2StoreCostIconData> IconDataInfos;

private:

	TWeakObjectPtr<UTextBlock>			TB_Desc;
	TWeakObjectPtr<UImage>				IMG_Icon;
	TWeakObjectPtr<UTextBlock>			TB_Cost;

	TWeakObjectPtr<UOverlay>			OV_PreCost;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_PreCost;
	TWeakObjectPtr<UImage>				IMG_Arrow;

};