#pragma once
#include "B2UIWidgetBase.h"
#include "B2UIFloatingText.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UB2UIFloatingText : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnim_BP();

	void SetText(FText);

	bool IsFinished();

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UTextBlock>	TB_Text;
};