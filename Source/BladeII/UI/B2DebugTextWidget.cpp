
#include "B2DebugTextWidget.h"
#include "BladeIIUtil.h"

void FDebugTextWidgetTextEntry::SetTBByThisInfo(UTextBlock* InTB)
{
	//if (!InTB){
	//	return;
	//}

	//InTB->SetText(FText::FromString(TheText));

	//FSlateFontInfo& FontInfoAdjust = InTB->Font;
	//FontInfoAdjust.Size = FontSize;
	//InTB->SetFont(FontInfoAdjust); // �̷��� �ؾ� Ȯ���� ������Ʈ �Ǵ� ��.
	//InTB->SetColorAndOpacity(FontColor);

	//if (bShadowed)
	//{ // Shadow �� �󸶳� ������ �� ���⼭ ���Ƿ�
	//	float ShadowOffsetScale = (float)FontSize * 0.12f;
	//	InTB->SetShadowOffset( FVector2D(ShadowOffsetScale, ShadowOffsetScale) );
	//	InTB->SetShadowColorAndOpacity(ShadowColor);
	//}
}

void FDebugTextWidgetTextEntry::SetCPSByThisInfo(UCanvasPanelSlot* InCPS)
{
	if (!InCPS) {
		return;
	}
	// �ؽ�Ʈ ����� �»�� ��Ŀ�� ��ü �������� ĵ���� �гο� ��ġ�Ǿ� �ִٴ� ����.
	InCPS->SetPosition(UpperLeftCoord);
	// ���ηδ� ������ ���еΰ�, ���ηδ� ȭ�� ������ �÷���.
	InCPS->SetSize(FVector2D(UB2UnitedWidgetBase::DesignedRefViewSize.X - UpperLeftCoord.X, FontSize * 2.0f));
}

UB2DebugTextWidget::UB2DebugTextWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

#if WITH_BII_ON_SCREEN_DEBUG_TEXT

TSubclassOf<class UB2DebugTextWidget> UB2DebugTextWidget::GetUniqueClassForScreenDisplay()
{
	// ��ü ��ũ���� ��� �뵵�� WidgetBP Ŭ����
	// ���ο��� �ִ��� ���� �������� ���� ini ���� �о������ �ʴ� �ɷ�.
	FString WidgetClassPath = TEXT("/Game/UI/DevOnlyWidget/ScreenDebugTextWidget");

	ConstructorHelpers::StripObjectClass(WidgetClassPath, true);
	TSubclassOf<UB2DebugTextWidget> FoundWidgetClass = ConstructorHelpersInternal::FindOrLoadClass(WidgetClassPath, UB2DebugTextWidget::StaticClass());
	return FoundWidgetClass;
}

int32 UB2DebugTextWidget::GetViewportZOrder()
{
	return BII_WIDGET_ZORDER_DEBUG_TEXT_WIDGET; 
}

void UB2DebugTextWidget::NativeConstruct()
{
	Super::NativeConstruct();
		
	CacheAssets();
}

void UB2DebugTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
		
	UpdateDrawText(); // �ϴ� �� ƽ �� ����..

	UpdateTimedText(InDeltaTime);
}

void UB2DebugTextWidget::DestroySelf()
{
	DestroyAllTexts();

	UB2UnitedWidgetBase::DestroySelfCommon();
}

void UB2DebugTextWidget::EnqueueDrawText(float InStartX, float InStartY, const FString& InText, const FLinearColor& InFontColor, int32 InFontSize, bool bDrawShadow, const FLinearColor& InShadowColor)
{
	// ���⼭ ť���� �ؽ�Ʈ draw ������ ���� ������Ʈ �� �ݿ��ǰ� �� ���� ������Ʈ ������ (�� ������) ����. �� ������ Draw ȣ���ϵ��� ���� ��.

	FDebugTextWidgetTextEntry NewEntry;
	NewEntry.UpperLeftCoord = FVector2D(InStartX, InStartY);
	NewEntry.TheText = InText;
	NewEntry.FontColor = InFontColor;
	NewEntry.FontSize = InFontSize;
	NewEntry.bShadowed = bDrawShadow;
	NewEntry.ShadowColor = InShadowColor;

	AddNewTextEntry(NewEntry);
}

void UB2DebugTextWidget::AddTimedDrawText(const FString& InText, const FLinearColor& InFontColor, int32 InFontSize, float InDisplayDuration, bool bDrawShadow, const FLinearColor& InShadowColor)
{
	FTimedDebugTextWidgetTextEntry NewEntry;
	NewEntry.DebugTextInfo.UpperLeftCoord = FVector2D(20.0f, 10.0f); // ���⼭ ��ġ�� ���������� �����ȴ�. �߰��� ���� ���� ���� �� ������ ��..
	NewEntry.DebugTextInfo.TheText = InText;
	NewEntry.DebugTextInfo.FontColor = InFontColor;
	NewEntry.DebugTextInfo.FontSize = InFontSize;
	NewEntry.DebugTextInfo.bShadowed = bDrawShadow;
	NewEntry.DebugTextInfo.ShadowColor = InShadowColor;

	NewEntry.CurrentDisplayedTime = 0.0;
	NewEntry.MaxDisplayTime = InDisplayDuration;

	AddTimedTextEntry(NewEntry);
}

void UB2DebugTextWidget::CacheAssets()
{
	GET_SLOT(UCanvasPanel, CP_Base);
}

void UB2DebugTextWidget::AddNewTextEntry(const FDebugTextWidgetTextEntry& NewTextEntry)
{
	DebugTextQueue.Add(NewTextEntry);
}

void UB2DebugTextWidget::AddTimedTextEntry(const FTimedDebugTextWidgetTextEntry& NewTimedEntry)
{
	while (TimedTextList.Num() > MaxTimedListSize) // �ִ� ���� �ʰ��ϸ� �տ����� ����..
	{
		if (TimedTextList[0].CreatedTB)
		{
			TimedTextList[0].CreatedTB->RemoveFromParent();
			TimedTextList[0].CreatedTB->MarkPendingKill();
		}
		TimedTextList.RemoveAt(0);
	}

	
	FTimedDebugTextWidgetTextEntry LocalNewEntry = NewTimedEntry;
	LocalNewEntry.DebugTextInfo.UpperLeftCoord.Y = 10.0f; // �ٽ� �ѹ� �� ���� Ȯ����
	
	// �ؽ�Ʈ ����
	LocalNewEntry.CreatedTB = NewObject<UTextBlock>(this, NAME_None, RF_Transient);
	if (LocalNewEntry.CreatedTB && CP_Base.IsValid())
	{		
		LocalNewEntry.CreatedTB->SetVisibility(ESlateVisibility::HitTestInvisible); // ��ư�� ������ �ʰ�.
		LocalNewEntry.DebugTextInfo.SetTBByThisInfo(LocalNewEntry.CreatedTB);
		UCanvasPanelSlot* NewTBCPS = Cast<UCanvasPanelSlot>(CP_Base->AddChild(LocalNewEntry.CreatedTB));
		if (NewTBCPS)
		{
			LocalNewEntry.DebugTextInfo.SetCPSByThisInfo(NewTBCPS);
		}
	}
		
	// ���ο� �� �ڿ��ٰ� �ְ�.. ���� �͵��� �� �Ʒ��� ��ĭ��
	TimedTextList.Add(LocalNewEntry);	
	for (int32 TLI = TimedTextList.Num()-1; TLI >= 0; --TLI)
	{
		FTimedDebugTextWidgetTextEntry& ThisEntry = TimedTextList[TLI];
		ThisEntry.DebugTextInfo.UpperLeftCoord.Y += TimedTextLineHeight;

		if (ThisEntry.CreatedTB)
		{
			UCanvasPanelSlot* TBCPS = Cast<UCanvasPanelSlot>(ThisEntry.CreatedTB->Slot);
			if (TBCPS)
			{
				ThisEntry.DebugTextInfo.SetCPSByThisInfo(TBCPS);
			}
		}
	}
}

void UB2DebugTextWidget::UpdateDrawText()
{
	// �������� ���� DebugTextQueue �� ���� display �� �ϰ� DebugTextQueue �� ����.

	// ������ ������ TextBlock ���� �ִ��� ��Ȱ��.
	const int32 RecycleNum = FMath::Min(DebugTextQueue.Num(), AllCreatedTBs.Num());
	for (int32 TI = 0; TI < RecycleNum; ++TI)
	{
		FDebugTextWidgetTextEntry& ThisTextEntry = DebugTextQueue[TI];
		UTextBlock* ThisTB = AllCreatedTBs[TI];
		if (ThisTB)
		{
			ThisTextEntry.SetTBByThisInfo(ThisTB);
			UCanvasPanelSlot* CPS = Cast<UCanvasPanelSlot>(ThisTB->Slot);
			if(CPS)
			{
				ThisTextEntry.SetCPSByThisInfo(CPS);
			}
		}
	}

	if (AllCreatedTBs.Num() > RecycleNum)
	{ // ���� ������� �� ����
		int32 RemoveStartIndex = RecycleNum;
		for (int32 TBRI = RemoveStartIndex; TBRI < AllCreatedTBs.Num(); ++TBRI)
		{
			UTextBlock* TBToRemove = AllCreatedTBs[TBRI];
			if (TBToRemove)
			{
				TBToRemove->RemoveFromParent();
				TBToRemove->MarkPendingKill();
			}
			AllCreatedTBs.RemoveAt(TBRI);
			--TBRI;
		}
		checkSlow(AllCreatedTBs.Num() == RecycleNum);
	}
	else if(DebugTextQueue.Num() > RecycleNum)
	{ // �߰��� ����
		for (int32 NTI = RecycleNum; NTI < DebugTextQueue.Num(); ++NTI)
		{
			FDebugTextWidgetTextEntry& ThisTextEntry = DebugTextQueue[NTI];
			
			UTextBlock* NewTB = NewObject<UTextBlock>(this, NAME_None, RF_Transient);
			if(NewTB && CP_Base.IsValid())
			{
				NewTB->SetVisibility(ESlateVisibility::HitTestInvisible); // ��ư�� ������ �ʰ�.
				ThisTextEntry.SetTBByThisInfo(NewTB);
				UCanvasPanelSlot* NewTBCPS = Cast<UCanvasPanelSlot>(CP_Base->AddChild(NewTB));
				if (NewTBCPS)
				{
					ThisTextEntry.SetCPSByThisInfo(NewTBCPS);
				}
				
				AllCreatedTBs.Add(NewTB);
			}
		}
	}

	DebugTextQueue.Empty(); // �̹� �� ��� ó��.
}

void UB2DebugTextWidget::UpdateTimedText(float InDeltaSecond)
{
	for (int32 TLI = TimedTextList.Num() - 1; TLI >= 0; --TLI)
	{
		FTimedDebugTextWidgetTextEntry& ThisEntry = TimedTextList[TLI];

		// �ð� �� �� �� ����
		ThisEntry.CurrentDisplayedTime += InDeltaSecond;
		if (ThisEntry.CurrentDisplayedTime >= ThisEntry.MaxDisplayTime) {
			if (ThisEntry.CreatedTB) {
				ThisEntry.CreatedTB->RemoveFromParent();
				ThisEntry.CreatedTB->MarkPendingKill();
			}
			TimedTextList.RemoveAt(TLI);
			continue;
		}
	}
}

void UB2DebugTextWidget::DestroyAllTexts()
{
	for (UTextBlock* ThisTB : AllCreatedTBs)
	{
		if (ThisTB)
		{
			ThisTB->RemoveFromParent();
			ThisTB->MarkPendingKill();
		}
	}
	AllCreatedTBs.Empty();
	DebugTextQueue.Empty();

	for (FTimedDebugTextWidgetTextEntry& TimedEntry : TimedTextList)
	{
		if (TimedEntry.CreatedTB)
		{
			TimedEntry.CreatedTB->RemoveFromParent();
			TimedEntry.CreatedTB->MarkPendingKill();
		}
	}
	TimedTextList.Empty();
}
#endif