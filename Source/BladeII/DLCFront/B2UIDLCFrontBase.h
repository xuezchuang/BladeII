#pragma once
//#include "BladeII.h"
#include "B2UnitedWidgetBase.h"
#include "B2UIDLCFrontBase.generated.h"

/**
 * Base user widget class for DLC front mode.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIDLCFrontBase : public UB2UnitedWidgetBase
{
	GENERATED_BODY()


public:

	UB2UIDLCFrontBase(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init(); // Supposed to be called from its managing class when it is created.

protected:
	// Intended to be similar to B2UIWidgetBase
	virtual void CacheAssets() {}
	virtual void UpdateStaticText() {}
	virtual void BindDelegates() {}
	
public:
	virtual void DestroySelf();

};