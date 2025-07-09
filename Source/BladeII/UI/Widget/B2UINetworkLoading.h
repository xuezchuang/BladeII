#pragma once

#include "B2UIWidget.h"
#include "B2UINetWorkLoading.generated.h"

/*
* Login UI
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UINetWorkLoading : public UB2UIWidget
{
	GENERATED_BODY()
private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetVisibilityReserve(bool bShow);
}; 