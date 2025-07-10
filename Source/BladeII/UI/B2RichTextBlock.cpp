
#include "B2RichTextBlock.h"
#include "UMGStyle.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// SB2RichTextBlock

#if WITH_EDITOR
#include "Kismet2/BlueprintEditorUtils.h"
#include "WidgetBlueprint.h"
#endif

int32 SB2RichTextBlock::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
#if WITH_EDITOR
	if (CachedPendingUpdateWidgetBlueprint)
	{
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(CachedPendingUpdateWidgetBlueprint);
	}
#endif
	return SRichTextBlock::OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}


/////////////////////////////////////////////////////
// UB2RichTextBlock

UB2RichTextBlock::UB2RichTextBlock(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//if (!IsRunningDedicatedServer())
	//{
	//	Font = GetDefaultSlateFontInfo();
	//}
	//Color = FLinearColor::White;

	//Visibility = ESlateVisibility::HitTestInvisible; // 괜히 아래 가리지 않게 기본값 HitTestInvisible 로.
}

void UB2RichTextBlock::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyRichTextBlock.Reset();
}

TSharedRef<SWidget> UB2RichTextBlock::RebuildWidget()
{
//	//+ OnHyperlinkClicked = FSlateHyperlinkRun::FOnClick::CreateStatic(&RichTextHelper::OnBrowserLinkClicked, AsShared());
//	//+ FHyperlinkDecorator::Create(TEXT("browser"), OnHyperlinkClicked))
//	//+MakeShareable(new FDefaultRichTextDecorator(Font, Color));
//
//	DefaultStyle.SetFont(Font);
//	DefaultStyle.SetColorAndOpacity(Color);
//
//	TArray< TSharedRef< class ITextDecorator > > CreatedDecorators;
//
//	for (URichTextBlockDecorator* Decorator : Decorators)
//	{
//		if (Decorator)
//		{
//			CreatedDecorators.Add(Decorator->CreateDecorator(Font, Color));
//		}
//	}
//
//	MyRichTextBlock =
//		SNew(SB2RichTextBlock)
//		.TextStyle(&DefaultStyle)
//		.Decorators(CreatedDecorators);
//#if WITH_EDITOR
//	MyRichTextBlock->CachedPendingUpdateWidgetBlueprint = nullptr;
//#endif
	return MyRichTextBlock.ToSharedRef();
}

void UB2RichTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();
//
//	TAttribute<FText> TextBinding = PROPERTY_BINDING(FText, Text);
//#if WITH_EDITOR
//	if ((Font != DefaultStyle.Font) || (DefaultStyle.ColorAndOpacity.GetSpecifiedColor() != Color))
//	{
//		MyRichTextBlock->CachedPendingUpdateWidgetBlueprint = Cast<UBlueprint>(WidgetGeneratedBy.Get());
//	}
//#endif
//
//	DefaultStyle.SetFont(Font);
//	DefaultStyle.SetColorAndOpacity(Color);
//	MyRichTextBlock->SetText(TextBinding);
//	
//	Super::SynchronizeTextLayoutProperties(*MyRichTextBlock);		
}

void UB2RichTextBlock::SetText(const FText& ToSetText)
{

	Text = ToSetText;

	////Woongbok
	//if (bIgnoreLineBreak)
	//{
	//	RemoveLineBreakFromText(Text);
	//}

	//if (MyRichTextBlock.IsValid())
	//{
	//	MyRichTextBlock->SetText(Text);
	//}
}

void UB2RichTextBlock::SetColorAndOpacity(FLinearColor InColor)
{
	Color = InColor;

	DefaultStyle.SetColorAndOpacity(Color);

	if(MyRichTextBlock.IsValid())
		MyRichTextBlock->SetTextStyle(DefaultStyle);

	//SynchronizeProperties();
}

const FText& UB2RichTextBlock::GetText()
{
	check(MyRichTextBlock.IsValid())

	return MyRichTextBlock->GetText();
}

#if WITH_EDITOR

//const FSlateBrush* UB2RichTextBlock::GetEditorIcon()
//{
	// re4ever UE4 MERGE 4.14. GetEditorIcon is deprecated. 
	// DEPRECATED(4.12, "GetEditorIcon is deprecated. Please define widget icons in your style set in the form ClassIcon.MyWidget, and register your style through FClassIconFinder::(Un)RegisterIconSource")
	// FUMGStyle::Create 에 뭔가 등록은 해 놓았는데 정확히 이전 이미지로 나오지는 않고 있음. 기냥 이대로 써도 별 문제는 없을 듯 하지만
	//return FUMGStyle::Get().GetBrush("Widget.RichTextBlock");
//}

const FText UB2RichTextBlock::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common");
}

void UB2RichTextBlock::OnCreationFromPalette()
{
	Decorators.Add(NewObject<URichTextBlockDecorator>(this, NAME_None, RF_Transactional));
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE