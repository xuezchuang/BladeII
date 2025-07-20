
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
	//InTB->SetFont(FontInfoAdjust); // 捞犯霸 秦具 犬角洒 诀单捞飘 登绰 淀.
	//InTB->SetColorAndOpacity(FontColor);

	//if (bShadowed)
	//{ // Shadow 甫 倔付唱 临瘤绰 傲 咯扁辑 烙狼肺
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
	// 咆胶飘 喉废捞 谅惑窜 巨目肺 傈眉 荤捞令狼 牡滚胶 菩澄俊 汲摹登绢 乐促绰 啊沥.
	InCPS->SetPosition(UpperLeftCoord);
	// 技肺肺绰 利寸洒 咯盒滴绊, 啊肺肺绰 拳搁 场鳖瘤 疵妨淋.
	InCPS->SetSize(FVector2D(UB2UnitedWidgetBase::DesignedRefViewSize.X - UpperLeftCoord.X, FontSize * 2.0f));
}

UB2DebugTextWidget::UB2DebugTextWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

#if WITH_BII_ON_SCREEN_DEBUG_TEXT

TSubclassOf<class UB2DebugTextWidget> UB2DebugTextWidget::GetUniqueClassForScreenDisplay()
{
	// 傈眉 胶农赴俊 剁匡 侩档狼 WidgetBP 努贰胶
	// 奖俏俊辑 弥措茄 腹捞 狐瘤档废 被捞 ini 俊辑 佬绢甸捞瘤 臼绰 吧肺.
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
		
	UpdateDrawText(); // 老窜 概 平 秦 焊绊..

	UpdateTimedText(InDeltaTime);
}

void UB2DebugTextWidget::DestroySelf()
{
	DestroyAllTexts();

	UB2UnitedWidgetBase::DestroySelfCommon();
}

void UB2DebugTextWidget::EnqueueDrawText(float InStartX, float InStartY, const FString& InText, const FLinearColor& InFontColor, int32 InFontSize, bool bDrawShadow, const FLinearColor& InShadowColor)
{
	// 咯扁辑 钮雷茄 咆胶飘 draw 沥焊绰 促澜 诀单捞飘 锭 馆康登绊 弊 促澜 诀单捞飘 锭鳖瘤 (茄 橇饭烙) 瘤加. 概 橇饭烙 Draw 龋免窍淀捞 静搁 凳.

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
	NewEntry.DebugTextInfo.UpperLeftCoord = FVector2D(20.0f, 10.0f); // 咯扁辑 困摹绰 郴何利栏肺 绊沥等促. 眠啊肺 甸绢棵 荐废 捞傈 搏 郴妨哎 巴..
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
	while (TimedTextList.Num() > MaxTimedListSize) // 弥措 俺荐 檬苞窍搁 菊俊何磐 力芭..
	{
		if (TimedTextList[0].CreatedTB)
		{
			TimedTextList[0].CreatedTB->RemoveFromParent();
			TimedTextList[0].CreatedTB->MarkPendingKill();
		}
		TimedTextList.RemoveAt(0);
	}

	
	FTimedDebugTextWidgetTextEntry LocalNewEntry = NewTimedEntry;
	LocalNewEntry.DebugTextInfo.UpperLeftCoord.Y = 10.0f; // 促矫 茄锅 盖 困肺 犬角洒
	
	// 咆胶飘 积己
	LocalNewEntry.CreatedTB = NewObject<UTextBlock>(this, NAME_None, RF_Transient);
	if (LocalNewEntry.CreatedTB && CP_Base.IsValid())
	{		
		LocalNewEntry.CreatedTB->SetVisibility(ESlateVisibility::HitTestInvisible); // 滚瓢捞 啊府瘤 臼霸.
		LocalNewEntry.DebugTextInfo.SetTBByThisInfo(LocalNewEntry.CreatedTB);
		UCanvasPanelSlot* NewTBCPS = Cast<UCanvasPanelSlot>(CP_Base->AddChild(LocalNewEntry.CreatedTB));
		if (NewTBCPS)
		{
			LocalNewEntry.DebugTextInfo.SetCPSByThisInfo(NewTBCPS);
		}
	}
		
	// 货肺款 扒 第俊促啊 持绊.. 捞傈 巴甸篮 促 酒贰肺 茄沫究
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
	// 流傈鳖瘤 阶牢 DebugTextQueue 俊 蝶扼 display 甫 窍绊 DebugTextQueue 甫 厚款促.

	// 扁粮俊 积己等 TextBlock 甸阑 弥措茄 犁劝侩.
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
	{ // 巢篮 镜葛绝绰 扒 力芭
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
	{ // 眠啊肺 积己
		for (int32 NTI = RecycleNum; NTI < DebugTextQueue.Num(); ++NTI)
		{
			FDebugTextWidgetTextEntry& ThisTextEntry = DebugTextQueue[NTI];
			
			UTextBlock* NewTB = NewObject<UTextBlock>(this, NAME_None, RF_Transient);
			if(NewTB && CP_Base.IsValid())
			{
				NewTB->SetVisibility(ESlateVisibility::HitTestInvisible); // 滚瓢捞 啊府瘤 臼霸.
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

	DebugTextQueue.Empty(); // 捞锅 搏 葛滴 贸府.
}

void UB2DebugTextWidget::UpdateTimedText(float InDeltaSecond)
{
	for (int32 TLI = TimedTextList.Num() - 1; TLI >= 0; --TLI)
	{
		FTimedDebugTextWidgetTextEntry& ThisEntry = TimedTextList[TLI];

		// 矫埃 促 等 芭 力芭
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