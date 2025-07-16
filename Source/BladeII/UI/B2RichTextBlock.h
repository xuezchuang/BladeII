#pragma once
//#include "BladeII.h"
#include "Components/TextWidgetTypes.h"
#include "Components/RichTextBlockDecorator.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "B2RichTextBlock.generated.h"

class SB2RichTextBlock : public SRichTextBlock
{
public:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
#if WITH_EDITOR
	class UBlueprint* CachedPendingUpdateWidgetBlueprint;
#endif
};


UCLASS(Blueprintable, BlueprintType)
class UB2RichTextBlock : public UTextLayoutWidget
{
	GENERATED_BODY()

public:
	UB2RichTextBlock(const FObjectInitializer& ObjectInitializer);

	// UWidget interface
	virtual void SynchronizeProperties() override;
	// End of UWidget interface

	// UVisual interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	// End of UVisual interface
	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "SetText (RichText)"))
	void SetText(const FText& ToSetText);

	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "GetText (RichText)"))
	const FText& GetText();


	void SetColorAndOpacity(FLinearColor InColor);
#if WITH_EDITOR
	// UWidget interface
	virtual const FText GetPaletteCategory() override;
	virtual void OnCreationFromPalette() override;
	// End UWidget interface
#endif

protected:
	/** The text to display */
	UPROPERTY(EditAnywhere, Category = Content, meta = (MultiLine = "true"))
	FText Text;

	/** A bindable delegate to allow logic to drive the text of the widget */
	UPROPERTY()
	FGetText TextDelegate;

	/** The default font for the text. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateFontInfo Font;

	/** The default color for the text. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FLinearColor Color;

	UPROPERTY(EditAnywhere, Instanced, Category = Decorators)
	TArray<class URichTextBlockDecorator*> Decorators;

protected:
	FTextBlockStyle DefaultStyle;

	/** Native Slate Widget */
	TSharedPtr<SB2RichTextBlock> MyRichTextBlock;

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface	

protected:
	PROPERTY_BINDING_IMPLEMENTATION(FText, Text);
};
