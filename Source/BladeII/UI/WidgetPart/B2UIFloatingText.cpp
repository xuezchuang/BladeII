#include "BladeII.h"
#include "B2UIFloatingText.h"

void UB2UIFloatingText::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Text);
}

void UB2UIFloatingText::SetText(FText Text)
{
	if (TB_Text.IsValid())
		TB_Text->SetText(Text);
}

bool UB2UIFloatingText::IsFinished()
{
	if (TB_Text.IsValid())
		return TB_Text->GetVisibility() == ESlateVisibility::Hidden;

	return true;
}