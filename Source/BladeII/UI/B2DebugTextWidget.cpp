
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
	//InTB->SetFont(FontInfoAdjust); // 이렇게 해야 확실히 업데이트 되는 듯.
	//InTB->SetColorAndOpacity(FontColor);

	//if (bShadowed)
	//{ // Shadow 를 얼마나 줄지는 걍 여기서 임의로
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
	// 텍스트 블록이 좌상단 앵커로 전체 사이즈의 캔버스 패널에 설치되어 있다는 가정.
	InCPS->SetPosition(UpperLeftCoord);
	// 세로로는 적당히 여분두고, 가로로는 화면 끝까지 늘려줌.
	InCPS->SetSize(FVector2D(UB2UnitedWidgetBase::DesignedRefViewSize.X - UpperLeftCoord.X, FontSize * 2.0f));
}

UB2DebugTextWidget::UB2DebugTextWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

#if WITH_BII_ON_SCREEN_DEBUG_TEXT

TSubclassOf<class UB2DebugTextWidget> UB2DebugTextWidget::GetUniqueClassForScreenDisplay()
{
	// 전체 스크린에 띄울 용도의 WidgetBP 클래스
	// 쉽핑에서 최대한 많이 빠지도록 굳이 ini 에서 읽어들이지 않는 걸로.
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
		
	UpdateDrawText(); // 일단 매 틱 해 보고..

	UpdateTimedText(InDeltaTime);
}

void UB2DebugTextWidget::DestroySelf()
{
	DestroyAllTexts();

	UB2UnitedWidgetBase::DestroySelfCommon();
}

void UB2DebugTextWidget::EnqueueDrawText(float InStartX, float InStartY, const FString& InText, const FLinearColor& InFontColor, int32 InFontSize, bool bDrawShadow, const FLinearColor& InShadowColor)
{
	// 여기서 큐잉한 텍스트 draw 정보는 다음 업데이트 때 반영되고 그 다음 업데이트 때까지 (한 프레임) 지속. 매 프레임 Draw 호출하듯이 쓰면 됨.

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
	NewEntry.DebugTextInfo.UpperLeftCoord = FVector2D(20.0f, 10.0f); // 여기서 위치는 내부적으로 고정된다. 추가로 들어올 수록 이전 껀 내려갈 것..
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
	while (TimedTextList.Num() > MaxTimedListSize) // 최대 개수 초과하면 앞에부터 제거..
	{
		if (TimedTextList[0].CreatedTB)
		{
			TimedTextList[0].CreatedTB->RemoveFromParent();
			TimedTextList[0].CreatedTB->MarkPendingKill();
		}
		TimedTextList.RemoveAt(0);
	}

	
	FTimedDebugTextWidgetTextEntry LocalNewEntry = NewTimedEntry;
	LocalNewEntry.DebugTextInfo.UpperLeftCoord.Y = 10.0f; // 다시 한번 맨 위로 확실히
	
	// 텍스트 생성
	LocalNewEntry.CreatedTB = NewObject<UTextBlock>(this, NAME_None, RF_Transient);
	if (LocalNewEntry.CreatedTB && CP_Base.IsValid())
	{		
		LocalNewEntry.CreatedTB->SetVisibility(ESlateVisibility::HitTestInvisible); // 버튼이 가리지 않게.
		LocalNewEntry.DebugTextInfo.SetTBByThisInfo(LocalNewEntry.CreatedTB);
		UCanvasPanelSlot* NewTBCPS = Cast<UCanvasPanelSlot>(CP_Base->AddChild(LocalNewEntry.CreatedTB));
		if (NewTBCPS)
		{
			LocalNewEntry.DebugTextInfo.SetCPSByThisInfo(NewTBCPS);
		}
	}
		
	// 새로운 건 뒤에다가 넣고.. 이전 것들은 다 아래로 한칸씩
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
	// 직전까지 쌓인 DebugTextQueue 에 따라 display 를 하고 DebugTextQueue 를 비운다.

	// 기존에 생성된 TextBlock 들을 최대한 재활용.
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
	{ // 남은 쓸모없는 건 제거
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
	{ // 추가로 생성
		for (int32 NTI = RecycleNum; NTI < DebugTextQueue.Num(); ++NTI)
		{
			FDebugTextWidgetTextEntry& ThisTextEntry = DebugTextQueue[NTI];
			
			UTextBlock* NewTB = NewObject<UTextBlock>(this, NAME_None, RF_Transient);
			if(NewTB && CP_Base.IsValid())
			{
				NewTB->SetVisibility(ESlateVisibility::HitTestInvisible); // 버튼이 가리지 않게.
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

	DebugTextQueue.Empty(); // 이번 껀 모두 처리.
}

void UB2DebugTextWidget::UpdateTimedText(float InDeltaSecond)
{
	for (int32 TLI = TimedTextList.Num() - 1; TLI >= 0; --TLI)
	{
		FTimedDebugTextWidgetTextEntry& ThisEntry = TimedTextList[TLI];

		// 시간 다 된 거 제거
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