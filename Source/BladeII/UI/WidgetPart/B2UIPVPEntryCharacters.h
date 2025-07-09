#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIUtil.h"

#include "B2UIPVPEntryCharacters.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPVPEntryCharacters : public UB2UIWidgetBase
{
	GENERATED_UCLASS_BODY()
public:	
	void SetEntryClasses(EPCClass MainChar, EPCClass SubChar);

protected:
	virtual void CacheAssets() override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<FB2ClassPortraitImage> PortraitInfo;

private:
	TWeakObjectPtr<UImage> IMG_MainClass;
	TWeakObjectPtr<UImage> IMG_SubClass;
	TWeakObjectPtr<UTextBlock> TB_MainClass;
	TWeakObjectPtr<UTextBlock> TB_SubClass;
};
