#include "BladeII.h"
#include "B2UIMsgPopupSimpleAddComment.h"


void UB2UIMsgPopupSimpleAddComment::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, TB_AddComment);
}

void UB2UIMsgPopupSimpleAddComment::SetAddComment(const FText& comment)
{
	if (TB_AddComment.IsValid())
	{
		TB_AddComment->SetText(comment);
	}
}
