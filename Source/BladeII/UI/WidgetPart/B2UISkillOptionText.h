#pragma once

#include "B2UIWidgetBase.h"
#include "B2UISkillOptionText.generated.h"

/*
* ProgressBar the use of the original version is more easily made.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISkillOptionText : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	void SetSkillOption(FText InSkillOption);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<class UB2RichTextBlock> TB_SkillOption;
};