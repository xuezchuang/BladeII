// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#include "B2GameLoadingScreen.h"
#include "Engine/Engine.h"
//#include "GenericApplication.h"
//#include "GenericApplicationMessageHandler.h"
#include "SlateExtras.h"
#if !UE_BUILD_SHIPPING
#include "GameMapsSettings.h"
#endif
#if WITH_EDITOR
#include "Misc/MessageDialog.h"
#endif
#include "Engine/UserInterfaceSettings.h"

// ¿©±â¼­ »ç¿ëÇÏ´Â ¸®¼Ò½º ¸µÅ©µéÀÌ ¾ÆÁÖ ¿¾ÀûºÎÅÍ ÇÏµåÄÚµù µÇ¾î ÀÖ¾ú´Âµ¥ ¾î¶»°Ô ÇÒ±î ÇÏ´Ù°¡ 
// ±¸Çö ¸ðµâµµ ´Ù¸¥ Âü¿¡ SomeInfo °°Àº °Íµµ ¸ø ¾²°í ÇØ¼­ ¼ÕÀÌ ¸¹ÀÌ °¡Áö ¾Ê´Â ÇÑ¿¡¼­´Â ±×³É ¿©±â¿¡ ¸ðÀ¸´Â Á¤µµ·Î.. 
//ÕâÀïÊ¹ÓÃµÄ×ÊÔ´Á´½Ó´ÓºÜ¾ÃÒÔÇ°¾Í±»Ó²±àÂëÁË£¬ÎÒÒ»Ö±ÔÚË¼¿¼¸ÃÔõÃ´×ö
//ÊµÏÖÄ£¿éÒ²²»ÄÜË³±ãÊ¹ÓÃSomeInfo£¬ËùÒÔÖ»Òª²»ÐèÒªÌ«¶àÈËÊÖ¾Í·ÅÔÚÕâÀï¡­
#define B2GAME_LS_MODULE_RES_DEFAULT_LOADING_IMG TEXT("/Game/UI/Loading/B2_MainTitle_BG_01.B2_MainTitle_BG_01")
#define B2GAME_LS_MODULE_RES_PROG_CIRCLE_IMG TEXT("/Game/UI/Loading/B2_Loading_BG_03.B2_Loading_BG_03")
#define B2GAME_LS_MODULE_RES_SKIP_BTN_IMG TEXT("/Game/UI/Loading/B2_BT_Skip_01.B2_BT_Skip_01")

#define B2GAME_LS_MODULE_RES_PROG_GAUGE_BG TEXT("/Game/UI_Resource/RE_Materials/Re_Common/MAT_B2_Common_Gauge_01")
#define B2GAME_LS_MODULE_RES_PROG_GAUGE_FILL TEXT("/Game/UI_Resource/RE_Materials/Re_Common/MAT_B2_Common_Gauge_02")

#if WITH_EDITOR	// ¸®¼Ò½º °æ·Î ÇÏµåÄÚµù¿¡ ´ëÇÑ ÃÖ¼ÒÇÑÀÇ ¾ÈÀüÀåÄ¡
void EditorCheckForHardCodedResources()
{
	UObject* LoadedCheck1 = LoadObject<UObject>(NULL, B2GAME_LS_MODULE_RES_DEFAULT_LOADING_IMG);
	UObject* LoadedCheck2 = LoadObject<UObject>(NULL, B2GAME_LS_MODULE_RES_PROG_CIRCLE_IMG);
	UObject* LoadedCheck3 = LoadObject<UObject>(NULL, B2GAME_LS_MODULE_RES_SKIP_BTN_IMG);
	UObject* LoadedCheck4 = LoadObject<UObject>(NULL, B2GAME_LS_MODULE_RES_PROG_GAUGE_BG);
	UObject* LoadedCheck5 = LoadObject<UObject>(NULL, B2GAME_LS_MODULE_RES_PROG_GAUGE_FILL);

	if (!LoadedCheck1 || !LoadedCheck2 || !LoadedCheck3 || !LoadedCheck4 || !LoadedCheck5)
	{
#if !PLATFORM_MAC
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("B2GameLoadingScreen ¿¡¼­ ÇÊ¿ä·Î ÇÏ´Â ¸®¼Ò½º ÀÏºÎ¸¦ ·Îµù ¸øÇÔ. ÄÄÇ»ÅÍ Æø¹ß.")));
#endif
	}
}
#endif

// From BladeIILocText.h, dependency problem...
#define B2LOC_CAT_GENERAL TEXT("General")
// BladeII ÂÊ¿¡ ÀÖ´Â BladeIIGetLOCText ¸¦ ¸ø °¡Á®¿Í¼­..
FText B2LSModuleGetLOCText(const FString& TextCategory, const FString& TextKey)
{
	FText OutText;
	if (FText::FindText(TextCategory, TextKey, OutText, nullptr))
		return OutText;
	else
		return FText::FromString(TEXT("Unknown Text"));
}

static float GetLoadingScreenContentScale()
{
	// ÀÌÂÊ UI ¼Â¾÷¿¡´Â ¿£Áø DPI ½ºÄÉÀÏÀÌ Àû¿ëµÇÁö ¾ÊÀ¸¹Ç·Î ÇÊ¿äÇÑ °÷¿¡´Â Á÷Á¢ °¡ÇØÁÖ¾î¾ß.
#if PLATFORM_ANDROID
	// ¾Èµå·ÎÀÌµå¿¡¼­´Â MobileContentScaleFactor °¡ ÇØ»óµµ¿¡ ÇØ´ç.
	static IConsoleVariable* CVarMCSF = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor"));
	check(CVarMCSF);
	return CVarMCSF ? CVarMCSF->GetFloat() : 1.0f;
#else
	return 1.0f; // IOS ´Â Á» ´Ù¸£°í.. PC ´Â ½ß.
#endif
}

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

struct FB2GameLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	static FSlateFontInfo* FontInfo;

	FB2GameLoadingScreenBrush(const FName InRenderResourceName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InRenderResourceName, InImageSize )
	{
		SetResourceObject(LoadObject<UObject>(NULL, *InRenderResourceName.ToString()));
	}

	/** 	ÇÑ ¶§ °Þ¾ú´ø GC Å©·¡½¬ ¹®Á¦(B2CLT-1495) ¶§¹®¿¡ ÇØ°á ½ÃµµÀÇ ÇÏ³ª·Î RootSet Ã³¸®¸¦ ÇÏ´ø °Å.
	 * ÀÛÀº ÀÌ¹ÌÁö µþ¶û ÇÏ³ª ¸Þ¸ð¸® »óÁÖÇÏ´Â °Å Âë °¨¼öÇÏ°íÀÚ ÇÏ´Â °ÍÀÌ°í ÀÏ¹ÝÀûÀÎ ÀÎÅÍÆäÀÌ½º·Î ¿ÀÇØÇÏ¸é °ï¶õ. */
	/*	ÒòÎªÔø¾­¾­Àú¹ýµÄGC×²³µÎÊÌâ(B2CLT - 1495)£¬×÷Îª½â¾ö·½·¨Ö®Ò»½øÐÐRootSet´¦Àí¡£
		Ö»ÊÇÏë½ÓÊÜÒ»¸öÐ¡Í¼ÏñµÄÄÚ´æ³£×¤£¬Èç¹ûÎó½âÎªÒ»°ãµÄ½çÃæ¾Í»áºÜÀ§ÄÑ¡£
	*/
	void SetResourceObjectAsRootset(bool bRootset)
	{
		if (GetResourceObject())
		{
			if (bRootset)
			{
				GetResourceObject()->AddToRoot();
			}
			else
			{
				GetResourceObject()->RemoveFromRoot();
			}
		}
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		//if( GetResourceObject() )
		//{
		//	Collector.AddReferencedObject(GetResourceObject());
		//}
	}
	
	// ini Æ¯Á¤ À§Ä¡¿¡ ¼³Á¤µÈ ÆùÆ® °æ·Î °¡Á®¿Í¼­ ·Îµù. uasset ÀÌ ¾Æ´Ô.
	static void LoadFontInfo()
	{
		FString FontResourcePath;
		if (GConfig)
		{
			GConfig->GetString(TEXT("LoadingScreenSlate"), TEXT("SubtitleFontPath"), FontResourcePath, GEngineIni);
		}
		FontInfo = new FSlateFontInfo(FontResourcePath, (int32)(15.0f * GetLoadingScreenContentScale()));
	}
	static void ConditionalLoadFontInfo()
	{
		if (FontInfo == nullptr)
		{
			LoadFontInfo();
		}
	}
	// Ã³À½ LoadFontInfo ¸¦ ÇÒ ¶§ GetLoadingScreenContentScale ÀÌ ÀÇµµÇÑ °ªÀ» ¸®ÅÏÇÏÁö ¾ÊÀ» ¼ö ÀÖ¾î¼­ Áß°£ Áß°£ »çÀÌÁî ¸®ÇÁ·¹½¬°¡ ÇÊ¿ä.
	// ¾Æ´Ô ±×³É ´Ù¸¥ »çÀÌÁî·Î ¾²°í ½Í´Ù°Å³ª ÇÒ ¶§¿¡µµ..
	static void SetFontSize(int32 NewSize)
	{
		if (FontInfo)
		{
			FontInfo->Size = NewSize;
		}
	}
};

// ·Îµù È­¸é¿¡¼­ »±»± µ¹¾Æ°¡´Â °Å Àç·á ¹× ±â´É ¸ðÀ½.
struct FB2LoadingScreenIndicatorSet
{
private:
	TSharedPtr<FB2GameLoadingScreenBrush> PieceBrush;
	TSharedPtr<SImage> MainImage;
	float ElapsedTime;
public:

	FB2LoadingScreenIndicatorSet()
	{
		ElapsedTime = 0.0f;
	}

	void Init()
	{
		static const FName PieceImageName(B2GAME_LS_MODULE_RES_PROG_CIRCLE_IMG);

		PieceBrush = MakeShareable(new FB2GameLoadingScreenBrush(PieceImageName, FVector2D(64.0f, 64.0f) * GetLoadingScreenContentScale()));

		//////////////////////////////////////////////////
		// ÇÑ ¶§ °Þ¾ú´ø GC Å©·¡½¬ ¹®Á¦(B2CLT-1495) ¶§¹®¿¡ ÇØ°á ½ÃµµÀÇ ÇÏ³ª·Î RootSet ¿¡ ³Ö¾ú´ø °Çµ¥.. ÀÌ°Ô µµ¿òÀÌ µÈ °Å °°Áö´Â ¾ÊÁö¸¸
		// PieceBrush ¿¡ ¾²ÀÌ´Â ÀÌ¹ÌÁö´Â ÀÛÀº °Å¶ó ¸Þ¸ð¸®¿¡ »óÁÖÇÑ´Ù°í ÇØ¼­ ¹®Á¦°¡ µÇÁö´Â ¾ÊÀ» °ÍÀÓ.
		if (PieceBrush.IsValid()) {
			PieceBrush->SetResourceObjectAsRootset(true);
		}
		//////////////////////////////////////////////////

		MainImage = SNew(SImage).Image(PieceBrush.Get());
		MainImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));

		ElapsedTime = 0.0f;
	}

	void TickAnim(const double InCurrentTime, const float InDeltaTime)
	{
		ElapsedTime += InDeltaTime;
		if (MainImage.IsValid())
		{
			MainImage->SetRenderTransform(FSlateRenderTransform(FQuat2D(-1.f * PI * ElapsedTime)));
		}
	}


	void SetMainImageVisible(bool bVisible)
	{
		if (MainImage.IsValid())
		{
			MainImage->SetVisibility(bVisible ? EVisibility::HitTestInvisible : EVisibility::Collapsed);
		}
	}

	TSharedRef<SImage> GetMainImageSharedRef() const { return MainImage.ToSharedRef(); }
};


FSlateFontInfo* FB2GameLoadingScreenBrush::FontInfo = nullptr;

// StartupLoadingScreen.. ÇÏÁö¸¸ °á±¹ ÃÖÁ¾ ¹èÆ÷ÆÇ DLC ¿¡¼± ½ÃÀÛ °æ·Î°¡ ´Ù¸£´Ï ÀÌ°É »ç¿ëÇÏÁö ¾Ê°Ô µÉ µí. Áï, »ç½Ç»ó °³¹ß¹öÀü¸¸ÀÇ ±â´É?
class SB2StartupLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SB2StartupLoadingScreen) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		static const FName LoadingScreenName(B2GAME_LS_MODULE_RES_DEFAULT_LOADING_IMG);
		static const FName PieceImageName(B2GAME_LS_MODULE_RES_PROG_CIRCLE_IMG);

		//since we are not using game styles here, just load one image
		LoadingScreenBrush = MakeShareable( new FB2GameLoadingScreenBrush( LoadingScreenName, FVector2D(1280.0f,720.0f) ) );

		IndicatorSet.Init();

		ChildSlot
			[
				SNew(SOverlay)
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SImage)
					.Image(LoadingScreenBrush.Get())
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(FMargin(0.f, 0.f, 0.f, 0.f))
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						IndicatorSet.GetMainImageSharedRef()
					]
				]
			];
	}

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
	{
		IndicatorSet.TickAnim(InCurrentTime, InDeltaTime);
	}

private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		return EVisibility::Visible;
	}

	/** loading screen image brush */
	TSharedPtr<FB2GameLoadingScreenBrush> LoadingScreenBrush;

	FB2LoadingScreenIndicatorSet IndicatorSet;
};

/** ·Îµù È­¸é¿¡¼­ ÁøÇàµµ ¹Ù Ç¥½ÃÇÒ °Íµé ¸ð¾Æ³õÀº °Å. ¼³Á¤À» ´Ù¸£°Ô ÇØ¼­ ¾²·Á´Â °Ô ¾Æ´Ô. ±×³É ¹Ýº¹ »ç¿ëµÉÁö ¸ô¶ó¼­ */
struct FB2LoadingScreenProgressBarDisplaySet
{
private:
	TSharedPtr<SProgressBar> ThePB;
	/** SProgressBar ¿¡¼­ »ç¿ëÇÒ SlateBrush */
	TSharedPtr<FB2GameLoadingScreenBrush> BGImageBrush;
	TSharedPtr<FB2GameLoadingScreenBrush> FillImageBrush;
	/** °è»êµÈ ·Îµù ÁøÇàµµ¸¦ ¿©±â¿¡ ¼¼ÆÃÇÏ¸é, ¿©±â¼± ¸¶Áö¸·À¸·Î Progress Bar ¿¡ »Ñ¸± animate µÈ ·Îµù ÁøÇàµµ °ªÀ» °è»êÇØ ÁØ´Ù. */
	FLoadingProgressBarAnim AnimatedProgressValue;

	float BasicTargetProgress;
	/** ´ÙÀ½ ÁøÇàµµ ¾÷µ¥ÀÌÆ®±îÁö ÅÒÀÌ ±æ°í °ªÀÌ ¾ó¸¶°¡ µÉÁö ¾Ë°í ÀÖ´Â °æ¿ì°¡ ÀÖ´Âµ¥, 
	 * ÀÌ·² ¶§ ÀÌ°É ´ÙÀ½ ¿¹»óµÇ´Â ÁøÇàµµ °ªÀ¸·Î ¼¼ÆÃÇØ¼­ Áß°£Áß°£ ÀÚÃ¼ÀûÀ¸·Î ÁøÇàµµ¸¦ °¡¶ó·Î Á¶±Ý¾¿ ¿Ã¸®µµ·Ï ÇÔ. */
	float NextPredictedTargetProgress;
	/** BasicTarget °ª°ú NextPredicted °ªÀÌ ´Ù¸¦ ¶§ ±× »çÀÌ¿¡ ÀÖ°Ô µÊ. */
	float FinalTargetProgress;
	/** ¼³Á¤°ª. NextPredictedTarget °ªÀÌ CurrentTarget º¸´Ù Å¬ ¶§ ¾ó¸¶³ª ÀÌ°É Àß°Ô Àß¶ó¼­ °¡¶ó ¾÷µ¥ÀÌÆ® ÇÒ Áö. */
	const int32 BaseFakeStepNumToNextPredictedTarget = 4;
	/** ¼³Á¤°ª. NextPredictedTarget °ªÀÌ CurrentTarget º¸´Ù Å¬ ¶§ °¡¶ó ¾÷µ¥ÀÌÆ® ½ºÅÜÀ» ¹âÀ» ÁÖ±â. */
	const float BaseFakeStepTimeInterval = 1.0f;
	/** BaseFakeStepNumToNextPredictedTarget ÀÌ¶û BaseFakeStepTimeInterval ¿¡¼­ ·£´ýÀ» Á» °¡¹Ì */
	int32 AdjustedFakeStepNumToNextPredictedTarget;
	float AdjustedFakeStepTimeInterval;
	float TimeSinceLastFakeStep;

	void RandSetAdjustedFakeStepNumToNextPredictedTarget()
	{
		AdjustedFakeStepNumToNextPredictedTarget = FMath::RandRange(BaseFakeStepNumToNextPredictedTarget - 1, BaseFakeStepNumToNextPredictedTarget + 1);
	}
	void RandSetAdjustedFakeStepTimeInterval() 
	{
		AdjustedFakeStepTimeInterval = FMath::FRandRange(BaseFakeStepTimeInterval * 0.7f, BaseFakeStepTimeInterval * 1.3f);
	}
public:

	FB2LoadingScreenProgressBarDisplaySet()
		: BasicTargetProgress(0.0f), NextPredictedTargetProgress(0.0f), FinalTargetProgress(0.0f)
		, AdjustedFakeStepNumToNextPredictedTarget(5), AdjustedFakeStepTimeInterval(1.0f), TimeSinceLastFakeStep(0.0f)
	{}
	~FB2LoadingScreenProgressBarDisplaySet()
	{}

	void Init()
	{
		// ´Ù¾çÇÏ°Ô ¾²·Á´Â °Ô ¾Æ´Ï°í À¯´ÏÅ©ÇÑ ¸®¼Ò½º ¾²´Â °Å. B2PreLoadingScreen ±â¹ÝÀ¸·Î µðÀÚÀÎÇÑ UI_Loading WidgetBP ¿¡ ¸ÂÃçÁÖ´Â °Í
		BGImageBrush = MakeShareable(new FB2GameLoadingScreenBrush(B2GAME_LS_MODULE_RES_PROG_GAUGE_BG, FVector2D(10.0f, 6.0f) * GetLoadingScreenContentScale()));
		FillImageBrush = MakeShareable(new FB2GameLoadingScreenBrush(B2GAME_LS_MODULE_RES_PROG_GAUGE_FILL, FVector2D(10.0f, 6.0f) * GetLoadingScreenContentScale()));

		check(BGImageBrush.IsValid() && FillImageBrush.IsValid());

		if (BGImageBrush.IsValid() && FillImageBrush.IsValid())
		{
			// ³ª¸ÓÁö ¼³Á¤µéµµ ¸ÂÃçÁà¾ß ÇÏ´Â °Ô ÀÖÀ½. UI_Loading ÂÊ ¹Ù²î¸é ¿©±âµµ.. 
			BGImageBrush->DrawAs = ESlateBrushDrawType::Box;
			BGImageBrush->TintColor = FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
			BGImageBrush->Margin = FMargin(0.5f, 0.5f, 0.5f, 0.5f);
			FillImageBrush->DrawAs = ESlateBrushDrawType::Box;
			FillImageBrush->TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			FillImageBrush->Margin = FMargin(0.5f, 0.0f, 0.5f, 0.0f);

			// ±×¸®°í ¾êµéµµ.. ¿¹Àü¿¡ ·ÎµùÈ­¸é Å©·¡½¬ ¹®Á¦°¡ ÇØ°áµÈ »óÈ²ÀÏ ¼ö ÀÖ¾î¼­ Áö±ÝÀº È®½ÇÈ÷ ¸ð¸£°Ú´Âµ¥ ÀÌ Á¤µµ´Â ·çÆ®¼Â ÇØ¼­ ¾ÈµÉ °Íµµ ¾øÁö.
			BGImageBrush->SetResourceObjectAsRootset(true);
			FillImageBrush->SetResourceObjectAsRootset(true);

			ThePB = SNew(SProgressBar).BackgroundImage(BGImageBrush.Get()).FillImage(FillImageBrush.Get());
			check(ThePB.IsValid());
			if (ThePB.IsValid())
			{
				// ¼¼·Î »çÀÌÁî°¡ ¹º°¡ ±âº»°ªÀÌ µé¾î°¡´ÂÁö Overlay ½½·Ô Á¶Àý¸¸À¸·Î´Â µðÀÚÀÎ µÈ °Å¶û ¸ÂÃßÁö ¸øÇØ¼­ RenderTransform À» ÁÖ´Â °Å.
				ThePB->SetRenderTransformPivot(FVector2D(0.0, 1.0f));
				ThePB->SetRenderTransform(FSlateRenderTransform(FScale2D(1.0f, 0.6f)));
				ThePB->SetPercent(0.0f);
			}
		}
	}
	void TickLoadingProgress(const float InDeltaTime, const float InTargetProgressScale, const float InNextPredictedTargetProgress = 0.0f)
	{
		// ¹º°¡ º¯°æÀÌ ÀÏ¾î³ª´Â Å¸ÀÌ¹ÖÀ» Æ½¿¡¼­ °¨ÁöÇÏ±â À§ÇÑ »¹Áþ.
		bool bTimeToBeginNewFakeStep = false;
		if (BasicTargetProgress < InTargetProgressScale && InTargetProgressScale < InNextPredictedTargetProgress)
		{
			bTimeToBeginNewFakeStep = true;
			TimeSinceLastFakeStep = 0.0f;
			RandSetAdjustedFakeStepNumToNextPredictedTarget();
			RandSetAdjustedFakeStepTimeInterval();
		}

		BasicTargetProgress = InTargetProgressScale;
		// ÀÏºÎ ¿¹Ãø °¡´ÉÇÑ »óÈ²¿¡¼­ BasicTargetProgress º¸´Ù Å©°Ô µé¾î¿Ã °Í.
		NextPredictedTargetProgress = FMath::Max(InNextPredictedTargetProgress, BasicTargetProgress);

		FinalTargetProgress = FMath::Max(FinalTargetProgress, BasicTargetProgress); // È¤½Ã³ª ÃÊ±âÈ­°¡ ¾È µÇ¾úÀ» ¶§ÀÇ ¿¹¿ÜÃ³¸®¸¦ ¸ÕÀú ÇØ µÎ´Â °Å.

		if(NextPredictedTargetProgress > BasicTargetProgress && !bTimeToBeginNewFakeStep && AdjustedFakeStepNumToNextPredictedTarget > 0)
		{
			// NextPredictedTargetProgress °¡ ´Ù¸¥ °æ¿ì ÀÌ¿¡ µû¸¥ ÈÑÀÌÅ© ¾÷µ¥ÀÌÆ®

			TimeSinceLastFakeStep += InDeltaTime;
			if (TimeSinceLastFakeStep >= AdjustedFakeStepTimeInterval)
			{
				const float SingleFakeStepInterval = (NextPredictedTargetProgress - BasicTargetProgress) / (float)AdjustedFakeStepNumToNextPredictedTarget;
				//int32 CurrentEstimatedFakeStep = FMath::RoundToInt((FinalTargetProgress - BasicTargetProgress) / SingleFakeStepInterval);
				// °¡¶ó ÁøÇàµµ ¾÷µ¥ÀÌÆ® ·£´ý °¡¹Ì.
				FinalTargetProgress += FMath::FRandRange(SingleFakeStepInterval * 0.7f, SingleFakeStepInterval * 1.3f);
				// ´ÙÀ½ ½ºÅÜÀ» À§ÇÑ ÃÊ±âÈ­
				TimeSinceLastFakeStep = 0.0f;
				RandSetAdjustedFakeStepTimeInterval();
			}
			// ÃÖÁ¾ º¸Á¤. Fake ½ºÅÜÀ» ³Ê¹« ¸¹ÀÌ ¹â°Ô µÇ´õ¶óµµ NextPredictedTargetProgress ¸¦ ³Ñ¾î¼± ¾ÈµÊ.
			FinalTargetProgress = FMath::Clamp(FinalTargetProgress, BasicTargetProgress, NextPredictedTargetProgress); 
		}
		else 
		{
			FinalTargetProgress = BasicTargetProgress;
		}

		// AnimatedProgressValue ·Î ¸Ô¿©ÁÖ´Â °ªµµ fake ¾÷µ¥ÀÌÆ®¸¦ Á» ÇÏÁö¸¸ AnimatedProgressValue ÀÚÃ¼ÀûÀ¸·Îµµ ÁøÇàµµ Áõ°¡ ¾Ö´Ï¸ÞÀÌ¼ÇÀÌ ÀÖ´Ù. 
		AnimatedProgressValue.SetTargetValue(FinalTargetProgress);
		AnimatedProgressValue.Tick(InDeltaTime);

		if (ThePB.IsValid())
		{
			ThePB->SetPercent(AnimatedProgressValue.GetCurrentAnimatedPercentScale());
		}
	}

	void SetProgressBarVisible(bool bVisible)
	{
		if (ThePB.IsValid())
		{
			ThePB->SetVisibility(bVisible ? EVisibility::HitTestInvisible : EVisibility::Collapsed);
		}
	}

	TSharedRef<SProgressBar> GetPBSharedRef() const { return ThePB.ToSharedRef(); }
};

bool ShouldUseShorterScreenLoadingImageLayout(const FVector2D& CurrViewportSize)
{ 
	// iPAD ·ùÀÇ ÇØ»óµµ¿¡¼­ ·Îµù ½ºÅ©¸° ·¹ÀÌ¾Æ¿ô¿¡ ´ëÇÑ ¿¹¿ÜÃ³¸®¸¦ ÇÏ±â À§ÇØ. ÀÏ¹Ý UI ¿¡¼­´Â ±×·± ÇØ»óµµµµ UUserInterfaceSettings::GetDPIScaleBasedOnSize ¿¡ °¡ÇÑ ¼öÁ¤À» ÅëÇØ Ã³¸®µÊ.

#if !UE_BUILD_SHIPPING
	const UUserInterfaceSettings* UISetting = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass());
	// UIScaleRule ÀÌ È¤¿©³ª ¹Ù²ð °¡´É¼ºÀ» ´Ù ´ëºñÇÏ·Á´Ï ³Ñ ¹÷Â÷°í ±×·¡µµ È¤¿©³ª ¹Ù²î¸é ³õÄ¡Áö ¾Ê°Ô..
	// »ç½Ç ÀÌ °¡Á¤ÀÌ ¹Ù²î¸é ShouldUseShorterScreenLoadingImageLayout ÀÌ¸§µµ ¹Ý´ë·Î ¹Ù²î¾î¾ß ÇÒ µí. ShouldUseLongerScreenLoadingImageLayout À¸·Î.
	ensure(UISetting && (UISetting->UIScaleRule == EUIScalingRule::ShortestSide || UISetting->UIScaleRule == EUIScalingRule::Horizontal));
#endif

	const float CurrentViewAspectRatio = (CurrViewportSize.Y > KINDA_SMALL_NUMBER) ? (CurrViewportSize.X / CurrViewportSize.Y) : 1.0f;

	// ÀÌ°Ç Àû´çÈ÷ »êÁ¤ÇÏ´Â °Çµ¥.. GEngine->SlateUIMinAspectRatio ¸¦ ¹Ù·Î ¾²Áö´Â ¾Ê´Â´Ù. ±× ºÎ±Ù¿¡¼­´Â °ªÀÌ ¼³·É ¾à°£ ÀÛ´Ù°í ÇØµµ ±âº» aspect ratio ¸¦ À§ÇÑ layout À» »ç¿ëÇØµµ ¹®Á¦°¡ µÇÁö ¾ÊÀ» °ÍÀÌ¹Ç·Î.
	//ÕâÊÇÊÊµ±¼ÆËãµÄ¡­²»Ö±½ÓÊ¹ÓÃGEngine-SlateUIMinAspectRatio¡£ÔÚÕâ¸½½ü£¬¼´Ê¹ÖµÉÔÎ¢Ð¡Ò»Ð©£¬Ê¹ÓÃ»ù±¾aspect ratioµÄlayoutÒ²²»»áÓÐÎÊÌâ¡£
	const float MinDesiredAspectRatio = 1.0;// (GEngine->SlateUIMinAspectRatio * 0.9f);
	
	return (MinDesiredAspectRatio > 0.0f && CurrentViewAspectRatio < MinDesiredAspectRatio);
}

// BladeII ¸ðµâ ÂÊ¿¡¼­ ÀûÀýÇÑ ÇÔ¼ö¸¦ ÇÒ´çÇØ ÁÖ¾î¾ß ÇÔ.
LoadGameSettingFloatFnType gLoadGameFxVolumeSettingFnPtr = nullptr;
LoadGameSettingFloatFnType gLoadGameBGMVolumeSettingFnPtr = nullptr;

//==================================================================================
// SB2LoadingScreenImage. ÀÏ¹ÝÀûÀÎ ¸Ê ·Îµù¿¡¼­ »ç¿ëÇÏ´Â ·Îµù ½ºÅ©¸° ±¸Çö.
//==================================================================================
class SB2LoadingScreenImage : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SB2LoadingScreenImage) {}
	SLATE_ARGUMENT(const TCHAR*, ImageName)
	SLATE_ARGUMENT(EB2LoadingMessageType, LoadingMessageType)
	SLATE_ARGUMENT(bool, bDisplayProgress)
	SLATE_ARGUMENT(bool, bDisplayIndicator)
	SLATE_ARGUMENT(bool, bStretchImage)
	SLATE_ARGUMENT(float, OptimalAspectRatio)
	SLATE_ARGUMENT(FVector2D, CurrentViewSize)
	SLATE_END_ARGS()

	static FVector2D GetAdjustedViewSizeByAspectRatio(const FVector2D& InRawViewSize, float InOptimalAspectRatio)
	{
		check(InOptimalAspectRatio > KINDA_SMALL_NUMBER);

		const bool bUseShorterScreenLoadingLayout = ShouldUseShorterScreenLoadingImageLayout(InRawViewSize);

		FVector2D RetVal = InRawViewSize;
		// ¾î´À °æ¿ìµç DPI Rule ÀÌ ÂªÀº º¯ ±âÁØ..
		if (bUseShorterScreenLoadingLayout)
		{
			RetVal.Y = RetVal.X / InOptimalAspectRatio;
		}
		else
		{
			RetVal.X = InOptimalAspectRatio * RetVal.Y;
		}
		return RetVal;
	}
	
	void Construct(const FArguments& InArgs)
	{
		const FName LoadingScreenName((InArgs._ImageName == nullptr) ? B2GAME_LS_MODULE_RES_DEFAULT_LOADING_IMG : InArgs._ImageName);
		static const FName PieceImageName(B2GAME_LS_MODULE_RES_PROG_CIRCLE_IMG);
		
		LoadingMessageType = InArgs._LoadingMessageType;
		bDisplayProgress = InArgs._bDisplayProgress;
		bDisplayIndicator = InArgs._bDisplayIndicator;

		// Æ¯Á¤ Å©±â(ºñÀ²)¸¦ ÃÖÀûÀ¸·Î ÇØ¼­ Áß¾ÓÁ¤·Ä, ¾Æ´Ï¸é Ã¤¿ì±â.
		const bool bNonFilledCenterAlignment = !InArgs._bStretchImage;
		const FVector2D CurrentViewSize = InArgs._CurrentViewSize;
		
		const FVector2D DesiredImageSize = bNonFilledCenterAlignment ? // Overlay ·Î ´Ü¼ø Fill À» ÇÏ¸é »çÀÌÁî¸¦ ¸ô¶óµµ µÇ´Âµ¥ Canvas ÆÐ³Î·Î ÇÏ·Á´Ù º¸´Ï ¾î¶² °æ¿ìµç ¾Ë¾Æ¾ß.. ¹º°¡ ´Ù¸¥ ÁÁÀº ¹æ¹ýÀÌ ¾ø³ª?
			GetAdjustedViewSizeByAspectRatio(CurrentViewSize, InArgs._OptimalAspectRatio) : CurrentViewSize;

		// iPAD °°Àº ±âº» ºñÀ²º¸´Ù ¹¶ÅöÇÑ È­¸é ºñÀ²ÀÎ °æ¿ì¸¦ »ý°¢ÇØ¼­. ÀÌ ¶§´Â ¾ç¿·À» Ã¤¿ì°í ¾Æ·¡À§¸¦ ºñ¿ì°ÚÁö.
		const bool bUseShorterScreenLoadingLayout = ShouldUseShorterScreenLoadingImageLayout(CurrentViewSize);

		//since we are not using game styles here, just load one image
		LoadingScreenBrush = MakeShareable(new FB2GameLoadingScreenBrush(LoadingScreenName, DesiredImageSize));
		
		IndicatorSet.Init();
		IndicatorSet.SetMainImageVisible(bDisplayIndicator);

		// Progress Bar ÃÊ±âÈ­. º¸ÀÌÁö ¾Ê°Ô ÇÏ·Á°í ÇØµµ ÀÏ´Ü ÇØ ³õ¾Æ¾ß.. ¤Ð¤Ð
		LoadingPBSet.Init();
		LoadingPBSet.SetProgressBarVisible(bDisplayProgress);
		if (bDisplayProgress){
			// ½ÇÁ¦ progress º¯¼öµµ ÃÊ±âÈ­ ÇØ¾ß ÇÑ´Ù. ÀÌÁ¦ ¸Ê ·Îµù ¿Ï·áµÉ ¶§±îÁö ÀÌ°÷Àú°÷¿¡¼­ progress ¸¦ Áõ°¡..
			//Êµ¼ÊµÄprogress±äÁ¿Ò²ÐèÒª³õÊ¼»¯¡£ÏÖÔÚÔÚµØÍ¼ÔØÈëÍê³ÉÖ®Ç°£¬ÔÚ¸÷´¦Ôö¼Óprogress¡­
			//GlobalResetMapLoadingProgress();
		}

		InitLoadingMessage();
		
#if PLATFORM_IOS
		// ¼ÎÀÌÇÁ Á¸
		FDisplayMetrics Metrics;
		FSlateApplication::Get().GetDisplayMetrics(Metrics);
#endif

		ChildSlot
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					/* Áß¾ÓÁ¤·Ä½Ã ¸ÞÀÎ ·Îµù ÀÌ¹ÌÁö¸¸ ¾à°£ Æ¯¼öÇÏ°Ô ÇÔ. Æ¯Á¤ È­¸é ºñÀ²¿¡¼­ ²Ë Ã¤¿ì°í ±×°Ô ¾Æ´Ï¸é Àß¸®°Å³ª ºñ´Â ½ÄÀ¸·Î */
					SNew(SCanvas)
					+ SCanvas::Slot()
					.HAlign((bNonFilledCenterAlignment && !bUseShorterScreenLoadingLayout) ? HAlign_Center : HAlign_Fill)
					.VAlign(bUseShorterScreenLoadingLayout ? VAlign_Center : VAlign_Fill)
					/* Áß¾ÓÁ¤·ÄÀ» ¾²·Á¸é ÀÏ¹ÝÀûÀÎ UWidget ¹èÄ¡¿Í´Â ´Þ¸® ÇöÀç ÇØ»óµµ¸¦ °¡Áö°í Á÷Á¢ ¼öÁ¤À» Á» ÇØ ÁÖ¾î¾ß.. */
					.Position(bNonFilledCenterAlignment ? (bUseShorterScreenLoadingLayout ? FVector2D(0.0f, CurrentViewSize.Y * 0.5f) : FVector2D(CurrentViewSize.X * 0.5f,0.0f)) : FVector2D(0.0f,0.0f))
					.Size(DesiredImageSize)
					[
						SNew(SImage)
						.Image(LoadingScreenBrush.Get())
					]
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Padding(FMargin(0.f, 0.f, 0.f, 0.f))
					[
						SNew(SOverlay)
						+SOverlay::Slot()
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							IndicatorSet.GetMainImageSharedRef()
						]
					]
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Bottom)
					.Padding(FMargin(0.f, 0.f, 0.f, 0.f))
					[
						LoadingPBSet.GetPBSharedRef()
					]
					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Bottom)
#if PLATFORM_IOS
					// ¼ÎÀÌÇÁ Á¸ ¿µ¿ª¸¸Å­ + 10 ¶ç¿ò
					.Padding(FMargin(0.0f, 0.0f, 0.0f, Metrics.ActionSafePaddingSize.Y + 10.0f))
 #else 
 					.Padding(FMargin(0.0f,0.0f,0.0f, (LoadingMessageType == EB2LoadingMessageType::ELMT_LobbyLoginLoading) ? 40.0f : 10.0f))
 #endif
					[
						LoadingMessageTB.ToSharedRef()
					]
				]
			];
	}

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
	{
		if (bDisplayIndicator)
		{
			IndicatorSet.TickAnim(InCurrentTime, InDeltaTime);
		}

		if (bDisplayProgress)
		{
			//LoadingPBSet.TickLoadingProgress(InDeltaTime, GlobalGetMapLoadingProgress(), GlobalGetNextPredictedMapLoadingProgress());
		}
	}

private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		return EVisibility::Visible;
	}

	void InitLoadingMessage()
	{
		if (LoadingMessageType != EB2LoadingMessageType::ELMT_None && LoadingMessageType != EB2LoadingMessageType::ELMT_End)
		{
			LMTBStyle = MakeShareable(new FTextBlockStyle());
			if (LMTBStyle.IsValid())
			{
				LMTBStyle->SetColorAndOpacity(FSlateColor(FLinearColor(0.955f, 0.955f, 0.955f, 1.0f)));
				LMTBStyle->SetFont(FB2GameLoadingScreenBrush::FontInfo ? *FB2GameLoadingScreenBrush::FontInfo : FSlateFontInfo());
				LMTBStyle->SetFontSize((int32)(15.0f * GetLoadingScreenContentScale()));  // ¿©±â ÆùÆ® »çÀÌÁî´Â º°µµ·Î ¼¼ÆÃµÇ´Â±º.
				LMTBStyle->SetShadowOffset(FVector2D(1.f, 1.f));
			}
			LoadingMessageTB = SNew(STextBlock).TextStyle(LMTBStyle.Get());

			if (LoadingMessageTB.IsValid())
			{
				// LoadingMessageType ÀÌ Ãß°¡µÇ¸é ±×¿¡ µû¶ó..
				switch (LoadingMessageType)
				{
				case EB2LoadingMessageType::ELMT_GameLoading:
					LoadingMessageTB->SetText(B2LSModuleGetLOCText(B2LOC_CAT_GENERAL, TEXT("PreLoadingText_GameLoading_Type03")));
					break;
				case EB2LoadingMessageType::ELMT_LobbyLoginLoading:
					LoadingMessageTB->SetText(B2LSModuleGetLOCText(B2LOC_CAT_GENERAL, TEXT("PreLoadingText_GameLoading_Type02")));
					break;
				}				
			}
		}
	}

	/** loading screen image brush */
	TSharedPtr<FB2GameLoadingScreenBrush> LoadingScreenBrush;
	
	FB2LoadingScreenProgressBarDisplaySet LoadingPBSet;
	FB2LoadingScreenIndicatorSet IndicatorSet;

	EB2LoadingMessageType LoadingMessageType;
	TSharedPtr<STextBlock> LoadingMessageTB;
	TSharedPtr<FTextBlockStyle> LMTBStyle;

	bool bDisplayProgress;
	bool bDisplayIndicator;
};

//==================================================================================
// SB2MovieLoadingScreen. µ¿¿µ»óÀÌ µ¹¾Æ°¡´Â ·Îµù ½ºÅ©¸° ±¸Çö. 
//==================================================================================
class SB2MovieLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SB2MovieLoadingScreen) {}
	SLATE_ARGUMENT(bool, bEnableSkipButton)
	SLATE_ARGUMENT(bool, bDisplayProgress)
	SLATE_ARGUMENT(bool, bDisplayIndicator)	
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		bEnableSkipButton = InArgs._bEnableSkipButton;
		bDisplayProgress = InArgs._bDisplayProgress;
		bDisplayIndicator = InArgs._bDisplayIndicator;

		static const FName SkipButtonName(B2GAME_LS_MODULE_RES_SKIP_BTN_IMG);

		//since we are not using game styles here, just load one image
		SkipButtonBrush = MakeShareable(new FB2GameLoadingScreenBrush(SkipButtonName, FVector2D(128.0, 64.0f) * GetLoadingScreenContentScale()));

		bDisableSubtitle = true;
		CurrentSubtitleIdx = 0;
		m_fSkipButtonDelayTime = 3.f;

		if (SkipButtonBrush.IsValid())
		{
			ButtonStyle.SetNormal(*SkipButtonBrush.Get());
			ButtonStyle.SetPressed(*SkipButtonBrush.Get());
			ButtonStyle.SetHovered(*SkipButtonBrush.Get());
			ButtonStyle.SetDisabled(*SkipButtonBrush.Get());
		}

		IndicatorSet.Init();
		IndicatorSet.SetMainImageVisible(bDisplayIndicator);

		// Progress Bar ÃÊ±âÈ­. º¸ÀÌÁö ¾Ê°Ô ÇÏ·Á°í ÇØµµ ÀÏ´Ü ÇØ ³õ¾Æ¾ß.. ¤Ð¤Ð
		LoadingPBSet.Init();
		LoadingPBSet.SetProgressBarVisible(bDisplayProgress);
		if (bDisplayProgress) {
			// ½ÇÁ¦ progress º¯¼öµµ ÃÊ±âÈ­ ÇØ¾ß ÇÑ´Ù. ÀÌÁ¦ ¸Ê ·Îµù ¿Ï·áµÉ ¶§±îÁö ÀÌ°÷Àú°÷¿¡¼­ progress ¸¦ Áõ°¡..
			//GlobalResetMapLoadingProgress();
		}

		ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Center)
			.Padding(FMargin(0.f, 0.f, 0.f, 40.f))
			[
				SNew(STextBlock)
				.Font(FB2GameLoadingScreenBrush::FontInfo ? *FB2GameLoadingScreenBrush::FontInfo : FSlateFontInfo())
				.Text(this, &SB2MovieLoadingScreen::GetSubtitleText)
				.ColorAndOpacity(this, &SB2MovieLoadingScreen::GetSubtitleColor)
				.ShadowOffset(FVector2D(1.f, 1.f))
				.Visibility(this, &SB2MovieLoadingScreen::GetMessageIndicatorVisibility)
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(FMargin(0.f, 0.f, 0.f, 0.f))
			[
				IndicatorSet.GetMainImageSharedRef()
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			.Padding(FMargin(0.f, 0.f, 0.f, 0.f))
			[
				LoadingPBSet.GetPBSharedRef()
			]
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Right)
			.Padding(FMargin(0.f, 10.f, 0.f, 10.f))
			[
				SNew(SButton)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.ClickMethod(EButtonClickMethod::MouseDown)
				.ButtonStyle(&ButtonStyle)
				.OnClicked(this, &SB2MovieLoadingScreen::OnSkipPressed)
				.Visibility(this, &SB2MovieLoadingScreen::GetSkipButtonVisibility)
			]
		];
	}

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
	{
		float CurrentTime = 0.0;// GetMoviePlayer()->GetMovieCurrentTime();
		while (Subtitles.Num() > CurrentSubtitleIdx && Subtitles[CurrentSubtitleIdx].EndTime < CurrentTime)
		{
			CurrentSubtitleIdx++;
		}

		if (Subtitles.Num() > CurrentSubtitleIdx)
		{
			if (Subtitles[CurrentSubtitleIdx].StartTime < CurrentTime)
			{
				bDisableSubtitle = false;
			}
			else
			{
				bDisableSubtitle = true;
			}
		}
		else
		{
			bDisableSubtitle = true;
		}

		if (bDisplayProgress)
		{
			//LoadingPBSet.TickLoadingProgress(InDeltaTime, GlobalGetMapLoadingProgress(), GlobalGetNextPredictedMapLoadingProgress());
		}
		if (bDisplayIndicator)
		{
			IndicatorSet.TickAnim(InCurrentTime, InDeltaTime);
		}
	}

private:
	EVisibility GetMessageIndicatorVisibility() const
	{
		return bDisableSubtitle ? EVisibility::Hidden : EVisibility::Visible;
	}

	EVisibility GetSkipButtonVisibility() const
	{
		//if (bEnableSkipButton)
		//{
		//	if (GetMoviePlayer()->GetMovieCurrentTime() < m_fSkipButtonDelayTime)
		//	{
		//		return EVisibility::Hidden;
		//	}

		//	return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Visible : EVisibility::Hidden;
		//}
		//else
		{
			return EVisibility::Collapsed;
		}
	}

	FText GetSubtitleText() const
	{
		if (bDisableSubtitle)
		{
			return FText::GetEmpty();
		}
		else
		{
			check(Subtitles.Num() > CurrentSubtitleIdx);
			return Subtitles[CurrentSubtitleIdx].Text;
		}
	}

	FSlateColor GetSubtitleColor() const
	{
		if (bDisableSubtitle)
		{
			return FSlateColor::UseForeground();
		}
		else
		{
			check(Subtitles.Num() > CurrentSubtitleIdx);
			return Subtitles[CurrentSubtitleIdx].Color;
		}
	}

	FReply OnSkipPressed()
	{
		GetMoviePlayer()->StopMovie();
		return FReply::Handled();
	}

	/** loading screen image brush */
	//TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
	int32 CurrentSubtitleIdx;
	bool bDisableSubtitle;
	float	m_fSkipButtonDelayTime;

public:
	TArray<B2MovieSubtitle> Subtitles;
private:
	bool bEnableSkipButton;
	bool bDisplayProgress;
	bool bDisplayIndicator;
	TSharedPtr<FSlateDynamicImageBrush> SkipButtonBrush;
	FButtonStyle ButtonStyle;

	FB2LoadingScreenProgressBarDisplaySet LoadingPBSet;
	FB2LoadingScreenIndicatorSet IndicatorSet;
};


//==================================================================================
// ¸ðµâ ±¸Çö. ¿ÜºÎ¿¡¼­ Á÷Á¢ÀûÀ¸·Î »ç¿ëÇÒ ÀÎÅÍÆäÀÌ½º
//==================================================================================
class FB2GameLoadingScreenModule : public IB2GameLoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
#if WITH_EDITOR
		if (GIsEditor) 
		{
			//if (GMinimalDLCFrontMode) {
			//	return; // ¾µ¸ð¾ø´Â ¸®¼Ò½º ÄíÅ· Á¦°Å
			//}
			//else
			 {
				EditorCheckForHardCodedResources(); // ¸®¼Ò½º ÀÖ´ÂÁö Ã¼Å©³ª
			}
		}
#endif

		FB2GameLoadingScreenBrush::ConditionalLoadFontInfo(); // ÀÌ ½ÃÁ¡¿¡´Â »óÈ²¿¡ µû¶ó ·ÎµùÀÌ Àß ¾ÈµÉ ¼ö ÀÖÀ½. ±×³É Á¦°ÅÇØ ¹ö¸±±î.

		bool bDoSetupFirstTimeLoadingScreen = true;
#if PLATFORM_ANDROID || PLATFORM_IOS 
		//if (IsFinalDLCCookRun())
		//{
		//	bDoSetupFirstTimeLoadingScreen = false; // ÃÖÁ¾ DLC ·±¿¡¼­´Â ¿©±â¼­ Ã¹ ·Îµù È­¸éÀ» ¶ç¿ï °Ô ¾Æ´Ï¶ó¼­..
		//}
#endif
		
		if (bDoSetupFirstTimeLoadingScreen) // ¾Æ¸¶µµ ÃÖÁ¾ ¹èÆ÷ ¹öÀüÀº ÀÌÂÊÀ¸·Î ¿ÀÁö ¾ÊÀ» µí?
		{
			if (IsMoviePlayerEnabled())
			{
				FLoadingScreenAttributes LoadingScreen;
				LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
				LoadingScreen.WidgetLoadingScreen = SNew(SB2StartupLoadingScreen); //FLoadingScreenAttributes::NewTestLoadingScreenWidget();
				GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
			}
		}
	}

	virtual void ShutdownModule()
	{
		if (FB2GameLoadingScreenBrush::FontInfo)
		{
			delete FB2GameLoadingScreenBrush::FontInfo;
			FB2GameLoadingScreenBrush::FontInfo = nullptr;
		}
	}

	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartGameLoadingScreen_Image(const TCHAR* LoadingImageName, bool bInDisplayProgress, bool bInDisplayIndicator = false,
		const FVector2D InViewSize = FVector2D(1280.0f,720.0f),
		bool bFullyStretchImage = false,
		float InOptimalAspectRatio = 2.055556f,
		EB2LoadingMessageType InMessageType = EB2LoadingMessageType::ELMT_GameLoading
	) override
	{
		//if (GMinimalDLCFrontMode && GIsEditor) { // DLC Front ¸ðµå ¸®¼Ò½º·Îµù ÃÖ´ëÇÑ Á¦°Å. ¿©±ä ÀÏ´Ü ÄíÅ·¿¡¼­¸¸ Á¦¿Ü½ÃÅ°°í.. ½ÇÁ¦ DLCFrontMode ½ÇÇà¿¡´Â ÇÊ¿äÇÒ ¼öµµ ÀÖ´Ù. ÇÊ¿ä¿¡ µû¶ó ¼ÕÁú.
		//	return;
		//}

		FB2GameLoadingScreenBrush::ConditionalLoadFontInfo(); // StartupModule ½Ã ·Îµù ¾ÈµÉ ¼ö ÀÖ´Â °Å ´ëºñ..

		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.bMoviesAreSkippable = false;
		LoadingScreen.bWaitForManualStop = false;

		LoadingScreen.WidgetLoadingScreen = SNew(SB2LoadingScreenImage).ImageName(LoadingImageName).LoadingMessageType(InMessageType).
			bDisplayProgress(bInDisplayProgress).bDisplayIndicator(bInDisplayIndicator).
			OptimalAspectRatio(InOptimalAspectRatio).bStretchImage(bFullyStretchImage).CurrentViewSize(InViewSize);
		
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);

		GetMoviePlayer()->PlayMovie(); // ¿£Áø ±âº» ·çÆ¾¿¡ ÀÇÇØ ºÒ¸®±ä ÇÔ.
	}

	//virtual TArray<B2MovieSubtitle>* StartGameLoadingScreen_Movie(const TCHAR* MovieName, 
	//	ELoadingScreenMovieAlignMode InAlignmentMode = ELoadingScreenMovieAlignMode::FitToMin,
	//	bool bInEnableMovieSkipButton = true,
	//	bool bInDisplayProgressBar = false,
	//	bool bInDisplayIndicator = false,
	//	EMoviePlaybackType InMoviePlayBackType = EMoviePlaybackType::MT_Normal,
	//	bool bAutoCloseOnLoadingComplete = false) override
	//{
	//	if (GMinimalDLCFrontMode && GIsEditor) { // DLC Front ¸ðµå ¸®¼Ò½º·Îµù ÃÖ´ëÇÑ Á¦°Å. ¿©±ä ÀÏ´Ü ÄíÅ·¿¡¼­¸¸ Á¦¿Ü½ÃÅ°°í.. ½ÇÁ¦ DLCFrontMode ½ÇÇà¿¡´Â ÇÊ¿äÇÒ ¼öµµ ÀÖ´Ù. ÇÊ¿ä¿¡ µû¶ó ¼ÕÁú.
	//		return nullptr;
	//	}

	//	UE_LOG(LogB2GameLoadingScreen, Log, TEXT("StartGameLoadingScreen_Movie with MovieName : %s"), MovieName ? MovieName : TEXT("None"));

	//	FB2GameLoadingScreenBrush::ConditionalLoadFontInfo(); // StartupModule ½Ã ·Îµù ¾ÈµÉ ¼ö ÀÖ´Â °Å ´ëºñ..
	//	FB2GameLoadingScreenBrush::SetFontSize((int32)15.0f * GetLoadingScreenContentScale()); // Ã³À½ ·Îµù ½Ã ÆùÆ® »çÀÌÁî°¡ ÀÇµµ´ë·Î ¼¼ÆÃ ¾ÈµÉ ¼ö ÀÖÀ½.

	//	FLoadingScreenAttributes LoadingScreen; 
	//	LoadingScreen.bAutoCompleteWhenLoadingCompletes = bAutoCloseOnLoadingComplete; // ¾Æ¸¶µµ Looping ÀÌ¸é ½Ã³ª¸®¿À µ¿¿µ»óÀÌ ¾Æ´Ñ »ç½Ç»ó ·Îµù ÀÌ¹ÌÁöÀÌ´Ï ÀÌ°É true ·Î ¿øÇÏ°ÚÁö¸¸ ¿¹¿Ü »óÈ²µµ ÀÖÀ» °Í °°´Ù.
	//	LoadingScreen.bMoviesAreSkippable = false;
	//	LoadingScreen.bWaitForManualStop = false;
	//	LoadingScreen.PlaybackType = InMoviePlayBackType;
	//	LoadingScreen.AlignMode = InAlignmentMode;

	//	float LoadedFxVolume = 1.0f, LoadedBGMVolume = 1.0f;
	//	// ÀúÀåµÈ °ÔÀÓ ¼³Á¤¿¡ µû¶ó µ¿¿µ»ó »ç¿îµå¸¦ ÄÑ°Å³ª ²ô°Å³ª. ÇÃ·§Æû º°·Î ¸ðµÎ volume ÄÁÆ®·Ñ±îÁö´Â °®ÃçÁ® ÀÖÁö ¾Ê¾Æ¼­ On/Off ¸¸ ÇÔ.
	//	if (gLoadGameFxVolumeSettingFnPtr)
	//	{ 
	//		gLoadGameFxVolumeSettingFnPtr(LoadedFxVolume);
	//	}
	//	if (gLoadGameBGMVolumeSettingFnPtr)
	//	{
	//		gLoadGameBGMVolumeSettingFnPtr(LoadedBGMVolume);
	//	}
	//	// ¹è°æÀ½, È¿°úÀ½ ±¸ºÐµµ ¾øÀ½. ¿©±ä ±×³É ÇÑ ¸öÃ¼ »ç¿îµåÀÓ. µÑ Áß ÇÏ³ª¸¸ 0 ¼³Á¤ÀÌ¸é ²ô´Â °É·Î.
	//	LoadingScreen.bIsSoundOn = (LoadedFxVolume > KINDA_SMALL_NUMBER && LoadedBGMVolume > KINDA_SMALL_NUMBER);

	//	TArray<B2MovieSubtitle>* RetArray = NULL;
	//	if (MovieName != NULL)
	//	{
	//		FString MovieNameString(MovieName);
	//		LoadingScreen.MoviePaths.Add(MovieNameString);
	//		// ÇöÀç 2°³ ÀÌ»ó Áö¿ø ¾ÈµÊ. Windows ´Â µÉ ²«µ¥ AndroidMovieStreamer ¶û AppleMovieStreamer ÂÊ¿¡ º°µµ·Î ±¸ÇöÇØ ÁÖ¾î¾ß ÇÔ.
	//		checkSlow(LoadingScreen.MoviePaths.Num() == 1);
	//		
	//		// µ¿¿µ»ó ³×ÀÌ¹Ö ÇÊ¼ö ¸ÂÃç¾ß.
	//		checkSlow(
	//			MovieNameString.StartsWith(FPaths::GetMovieFilePrefix_DLC())
	//			|| MovieNameString.StartsWith(FPaths::GetMovieFilePrefix_APK())
	//		);

	//		// ¸ð¹ÙÀÏ ¹èÆ÷ÆÇ¿¡¼­´Â DLC °æ·Î¸¦ »ç¿ëÇÏµµ·Ï.. ÇÏÁö¸¸ ÀÏ¹Ý (´Ü¼ø ¹èÆ÷) ÄíÅ·¿¡¼­ ÀÛµ¿ÇÒ ¿©Áö°¡ ÀÖ¾î¾ß ÇÔ.
	//		LoadingScreen.bMovieInDLCPath = (
	//			(PLATFORM_ANDROID || PLATFORM_IOS) &&
	//			IsFinalDLCCookRun() &&
	//			MovieNameString.StartsWith(FPaths::GetMovieFilePrefix_DLC())
	//		);

	//		LoadingScreen.WidgetLoadingScreen = SNew(SB2MovieLoadingScreen).bEnableSkipButton(bInEnableMovieSkipButton).bDisplayProgress(bInDisplayProgressBar).bDisplayIndicator(bInDisplayIndicator);

	//		SB2MovieLoadingScreen* MovieScreen = static_cast<SB2MovieLoadingScreen*>(LoadingScreen.WidgetLoadingScreen.Get());
	//		if (MovieScreen)
	//		{
	//			RetArray = &MovieScreen->Subtitles;
	//		}
	//	}

	//	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);

	//	if (MovieName != NULL)
	//	{
	//		GetMoviePlayer()->PlayMovie();
	//	}

	//	//GetMoviePlayer()¿¡¼­ FNullGameMoviePlayer°¡ ³ª¿Â °æ¿ì. À§¿¡¼­ ±¸ÇÑ RetArray°¡ Dangling pointer°¡ µÊ.
	//	return IsMoviePlayerEnabled() ? RetArray : nullptr; // ¿©±â¼­ subtitle ¼¼ÆÃÇÒ ·¹ÆÛ·±½º¸¦ ¸®ÅÏ°ªÀ¸·Î Àü´ÞÇØ¼­ ¹Û¿¡¼­ ¼¼ÆÃÇÏ´Â ±¸Á¶
	//}
};

/////////////////////////////////////////////////////////////////////////////////
// FLoadingProgressBarAnim

FLoadingProgressBarAnim::FLoadingProgressBarAnim()
{
	///////////// ÀûÀýÇÑ ¼³Á¤À¸·Î tweak ..
	BaseSpeed = 0.6f;
	SpringConst = 4000.0f;
	DampConst = 50.0f;

	TargetPercentScale = 0.0f;
	CurrentAnimatedPercentScale = 0.0f;
	CurrentSpeed = 0.0f;
}

void FLoadingProgressBarAnim::Tick(float InDeltaSeconds)
{
	const float DistToTarget = GetUnitDistanceToTargetValue();
	if (DistToTarget > 0.0f)
	{
		//if (AnimType == EProgressAnimType::PRAT_SpringDamp)
		{
			// ¿ë¼öÃ¶ ¸ðµ¨. ·¹º§ÀÌ ÇÑ¹ø¿¡ ¿©·¯ ´Ü°è¸¦ ¶Ù¾î¾ß ÇÏ´Â °æ¿ì¿¡ µî¼Óµµ·Î ÇÏ¸é ´Ù¾çÇÑ ±¸°£ Â÷ÀÌ¸¦ Ä¿¹öÇÒ ¼ö ÀÖ´Â Àû´çÇÑ ¼Óµµ¸¦ Ã£±â°¡ ¾î·Á¿ï ¶§°¡ ÀÖ¾î¼­ ¿ë¼öÃ¶ ¸ðµ¨À» ¾²¸é ÁÁÀ½.
			CurrentSpeed += DistToTarget * SpringConst * InDeltaSeconds;
			CurrentSpeed -= CurrentSpeed * DampConst * InDeltaSeconds;
			// ÀÌ °æ¿ì¿¡µµ ÃÖ¼ÒÇÑÀÇ ¼Óµµ´Â ³ª¿À°Ô ÇÔ.
			CurrentSpeed = FMath::Max(CurrentSpeed, BaseSpeed);
		}
		// ¾Æ·¡Ã³·³ ÇÏ¸é ´Ü¼øÈ÷ ¼±Çü Áõ°¡
		//else
		//{
		//	CurrentSpeed = BaseSpeed;
		//}

		CurrentAnimatedPercentScale += CurrentSpeed * InDeltaSeconds;
	}

	// ´Ù½Ã±Ý ¸ñÇ¥°ª¿¡ µµ´ÞÇß´ÂÁö Ã¼Å©ÇÏ°í Å¬·¥ÇÁ
	if (GetUnitDistanceToTargetValue() <= 0.0f)
	{ // ¸ñÇ¥°ª¿¡ µµ´ÞÇÑ »óÅÂ
		SyncToTargetValue();
	}
}

void FLoadingProgressBarAnim::SetTargetValue(float InTargetPercentScale, bool bSyncCurrentValues)
{
	TargetPercentScale = InTargetPercentScale;

	if (bSyncCurrentValues) // ÀÌ°Ç Áõ°¡ ¾Ö´Ï¸ÞÀÌ¼Ç °°Àº °Å ¾øÀÌ ±×³É ¹Ù·Î ¸ÂÃß°Ú´Ù´Â °Å 
	{
		SyncToTargetValue();
	}
}

void FLoadingProgressBarAnim::SyncToTargetValue()
{
	CurrentAnimatedPercentScale = TargetPercentScale;
	CurrentSpeed = 0.0f;
}

float FLoadingProgressBarAnim::GetUnitDistanceToTargetValue()
{
	return FMath::Max(TargetPercentScale - CurrentAnimatedPercentScale, 0.0f);
}

/////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_GAME_MODULE(FB2GameLoadingScreenModule, B2GameLoadingScreen);
