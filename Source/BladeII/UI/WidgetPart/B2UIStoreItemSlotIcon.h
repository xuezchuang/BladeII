#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIStoreItemSlotIcon.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStoreItemSlotIcon : public UB2UIWidget
{
	GENERATED_BODY()

public:

	virtual void Init() override;

protected:

	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:

	void SetIcon(UMaterialInterface* InMaterial);
	void SetEffect(int32 EffecNum);

private:

	TWeakObjectPtr<UImage>		IMG_Icon;
	
	TWeakObjectPtr<UOverlay>	OV_Effect_1;
	TWeakObjectPtr<UOverlay>	OV_Effect_2;
	TWeakObjectPtr<UOverlay>	OV_Effect_3;
};