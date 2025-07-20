#pragma once

#include "B2UIWidget.h"
#include "B2UINatWorkLoading.generated.h"

/*
* Login UI
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UINatWorkLoading : public UB2UIWidget
{
	GENERATED_BODY()
private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
}; 