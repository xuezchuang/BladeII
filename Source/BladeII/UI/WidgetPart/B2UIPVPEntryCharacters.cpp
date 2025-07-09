#include "BladeII.h"
#include "B2UIPVPEntryCharacters.h"


#include "BladeIIUtil.h"

UB2UIPVPEntryCharacters::UB2UIPVPEntryCharacters(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIPVPEntryCharacters::CacheAssets()
{
	GET_SLOT(UImage, IMG_MainClass);
	GET_SLOT(UImage, IMG_SubClass);
	GET_SLOT(UTextBlock, TB_MainClass);
	GET_SLOT(UTextBlock, TB_SubClass);
}

void UB2UIPVPEntryCharacters::SetEntryClasses(EPCClass MainChar, EPCClass SubChar)
{
	/*if (IMG_MainClass.IsValid())
	{
		IMG_MainClass->SetVisibility(ESlateVisibility::Collapsed);
		for (auto& Info : PortraitInfo)
		{
			if (Info.CharClass == MainChar)
			{
				IMG_MainClass->SetBrushFromMaterial(Info.Portrait);
				IMG_MainClass->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}

	if (IMG_SubClass.IsValid())
	{
		IMG_SubClass->SetVisibility(ESlateVisibility::Collapsed);
		for (auto& Info : PortraitInfo)
		{
			if (Info.CharClass == SubChar)
			{
				IMG_SubClass->SetBrushFromMaterial(Info.Portrait);
				IMG_SubClass->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}

	if (TB_MainClass.IsValid())
		TB_MainClass->SetText(GetPCClassLocalizedText(MainChar));

	if (TB_SubClass.IsValid())
		TB_SubClass->SetText(GetPCClassLocalizedText(SubChar));*/

}