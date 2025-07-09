#include "BladeII.h"
#include "B2RichTextBlock.h"
#include "B2UISkillOptionText.h"


void UB2UISkillOptionText::SetSkillOption(FText InSkillOption)
{
	if (TB_SkillOption.IsValid())
		TB_SkillOption->SetText(InSkillOption);
}

void UB2UISkillOptionText::CacheAssets()
{
	GET_SLOT(UB2RichTextBlock, TB_SkillOption);
}