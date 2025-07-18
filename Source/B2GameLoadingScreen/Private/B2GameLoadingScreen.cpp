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

// 여기서 사용하는 리소스 링크들이 아주 옛적부터 하드코딩 되어 있었는데 어떻게 할까 하다가 
// 구현 모듈도 다른 참에 SomeInfo 같은 것도 못 쓰고 해서 손이 많이 가지 않는 한에서는 그냥 여기에 모으는 정도로.. 
//侶쟁賈痰돨栗都졍쌈닒붤씹鹿품앎굳袒긍쯤죄，乖寧殮瞳鋼옘맡等척隣
//茄君친욥冷꼇콘糠긱賈痰SomeInfo，杰鹿怜狼꼇矜狼格뜩훙癎앎렴瞳侶쟁∼
#define B2GAME_LS_MODULE_RES_DEFAULT_LOADING_IMG TEXT("/Game/UI/Loading/B2_MainTitle_BG_01.B2_MainTitle_BG_01")
#define B2GAME_LS_MODULE_RES_PROG_CIRCLE_IMG TEXT("/Game/UI/Loading/B2_Loading_BG_03.B2_Loading_BG_03")
#define B2GAME_LS_MODULE_RES_SKIP_BTN_IMG TEXT("/Game/UI/Loading/B2_BT_Skip_01.B2_BT_Skip_01")

#define B2GAME_LS_MODULE_RES_PROG_GAUGE_BG TEXT("/Game/UI_Resource/RE_Materials/Re_Common/MAT_B2_Common_Gauge_01")
#define B2GAME_LS_MODULE_RES_PROG_GAUGE_FILL TEXT("/Game/UI_Resource/RE_Materials/Re_Common/MAT_B2_Common_Gauge_02")

#if WITH_EDITOR	// 리소스 경로 하드코딩에 대한 최소한의 안전장치
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
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("B2GameLoadingScreen 에서 필요로 하는 리소스 일부를 로딩 못함. 컴퓨터 폭발.")));
#endif
	}
}
#endif

// From BladeIILocText.h, dependency problem...
#define B2LOC_CAT_GENERAL TEXT("General")
// BladeII 쪽에 있는 BladeIIGetLOCText 를 못 가져와서..
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
	// 이쪽 UI 셋업에는 엔진 DPI 스케일이 적용되지 않으므로 필요한 곳에는 직접 가해주어야.
#if PLATFORM_ANDROID
	// 안드로이드에서는 MobileContentScaleFactor 가 해상도에 해당.
	static IConsoleVariable* CVarMCSF = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor"));
	check(CVarMCSF);
	return CVarMCSF ? CVarMCSF->GetFloat() : 1.0f;
#else
	return 1.0f; // IOS 는 좀 다르고.. PC 는 쌩.
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

	/** 	한 때 겪었던 GC 크래쉬 문제(B2CLT-1495) 때문에 해결 시도의 하나로 RootSet 처리를 하던 거.
	 * 작은 이미지 딸랑 하나 메모리 상주하는 거 쯤 감수하고자 하는 것이고 일반적인 인터페이스로 오해하면 곤란. */
	/*	凜槨鄧쒔쒔저법돨GC旒났狂痙(B2CLT - 1495)，鱗槨썩엄렘랬裂寧쏵契RootSet뇹잿。
		怜角拳쌈肝寧몸鬼暠獗돨코닸끽壘，흔벎轎썩槨寧겹돨썹충앎삔붤위켜。
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
	
	// ini 특정 위치에 설정된 폰트 경로 가져와서 로딩. uasset 이 아님.
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
	// 처음 LoadFontInfo 를 할 때 GetLoadingScreenContentScale 이 의도한 값을 리턴하지 않을 수 있어서 중간 중간 사이즈 리프레쉬가 필요.
	// 아님 그냥 다른 사이즈로 쓰고 싶다거나 할 때에도..
	static void SetFontSize(int32 NewSize)
	{
		if (FontInfo)
		{
			FontInfo->Size = NewSize;
		}
	}
};

// 로딩 화면에서 뺑뺑 돌아가는 거 재료 및 기능 모음.
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
		// 한 때 겪었던 GC 크래쉬 문제(B2CLT-1495) 때문에 해결 시도의 하나로 RootSet 에 넣었던 건데.. 이게 도움이 된 거 같지는 않지만
		// PieceBrush 에 쓰이는 이미지는 작은 거라 메모리에 상주한다고 해서 문제가 되지는 않을 것임.
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

// StartupLoadingScreen.. 하지만 결국 최종 배포판 DLC 에선 시작 경로가 다르니 이걸 사용하지 않게 될 듯. 즉, 사실상 개발버전만의 기능?
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

/** 로딩 화면에서 진행도 바 표시할 것들 모아놓은 거. 설정을 다르게 해서 쓰려는 게 아님. 그냥 반복 사용될지 몰라서 */
struct FB2LoadingScreenProgressBarDisplaySet
{
private:
	TSharedPtr<SProgressBar> ThePB;
	/** SProgressBar 에서 사용할 SlateBrush */
	TSharedPtr<FB2GameLoadingScreenBrush> BGImageBrush;
	TSharedPtr<FB2GameLoadingScreenBrush> FillImageBrush;
	/** 계산된 로딩 진행도를 여기에 세팅하면, 여기선 마지막으로 Progress Bar 에 뿌릴 animate 된 로딩 진행도 값을 계산해 준다. */
	FLoadingProgressBarAnim AnimatedProgressValue;

	float BasicTargetProgress;
	/** 다음 진행도 업데이트까지 텀이 길고 값이 얼마가 될지 알고 있는 경우가 있는데, 
	 * 이럴 때 이걸 다음 예상되는 진행도 값으로 세팅해서 중간중간 자체적으로 진행도를 가라로 조금씩 올리도록 함. */
	float NextPredictedTargetProgress;
	/** BasicTarget 값과 NextPredicted 값이 다를 때 그 사이에 있게 됨. */
	float FinalTargetProgress;
	/** 설정값. NextPredictedTarget 값이 CurrentTarget 보다 클 때 얼마나 이걸 잘게 잘라서 가라 업데이트 할 지. */
	const int32 BaseFakeStepNumToNextPredictedTarget = 4;
	/** 설정값. NextPredictedTarget 값이 CurrentTarget 보다 클 때 가라 업데이트 스텝을 밟을 주기. */
	const float BaseFakeStepTimeInterval = 1.0f;
	/** BaseFakeStepNumToNextPredictedTarget 이랑 BaseFakeStepTimeInterval 에서 랜덤을 좀 가미 */
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
		// 다양하게 쓰려는 게 아니고 유니크한 리소스 쓰는 거. B2PreLoadingScreen 기반으로 디자인한 UI_Loading WidgetBP 에 맞춰주는 것
		BGImageBrush = MakeShareable(new FB2GameLoadingScreenBrush(B2GAME_LS_MODULE_RES_PROG_GAUGE_BG, FVector2D(10.0f, 6.0f) * GetLoadingScreenContentScale()));
		FillImageBrush = MakeShareable(new FB2GameLoadingScreenBrush(B2GAME_LS_MODULE_RES_PROG_GAUGE_FILL, FVector2D(10.0f, 6.0f) * GetLoadingScreenContentScale()));

		check(BGImageBrush.IsValid() && FillImageBrush.IsValid());

		if (BGImageBrush.IsValid() && FillImageBrush.IsValid())
		{
			// 나머지 설정들도 맞춰줘야 하는 게 있음. UI_Loading 쪽 바뀌면 여기도.. 
			BGImageBrush->DrawAs = ESlateBrushDrawType::Box;
			BGImageBrush->TintColor = FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
			BGImageBrush->Margin = FMargin(0.5f, 0.5f, 0.5f, 0.5f);
			FillImageBrush->DrawAs = ESlateBrushDrawType::Box;
			FillImageBrush->TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			FillImageBrush->Margin = FMargin(0.5f, 0.0f, 0.5f, 0.0f);

			// 그리고 얘들도.. 예전에 로딩화면 크래쉬 문제가 해결된 상황일 수 있어서 지금은 확실히 모르겠는데 이 정도는 루트셋 해서 안될 것도 없지.
			BGImageBrush->SetResourceObjectAsRootset(true);
			FillImageBrush->SetResourceObjectAsRootset(true);

			ThePB = SNew(SProgressBar).BackgroundImage(BGImageBrush.Get()).FillImage(FillImageBrush.Get());
			check(ThePB.IsValid());
			if (ThePB.IsValid())
			{
				// 세로 사이즈가 뭔가 기본값이 들어가는지 Overlay 슬롯 조절만으로는 디자인 된 거랑 맞추지 못해서 RenderTransform 을 주는 거.
				ThePB->SetRenderTransformPivot(FVector2D(0.0, 1.0f));
				ThePB->SetRenderTransform(FSlateRenderTransform(FScale2D(1.0f, 0.6f)));
				ThePB->SetPercent(0.0f);
			}
		}
	}
	void TickLoadingProgress(const float InDeltaTime, const float InTargetProgressScale, const float InNextPredictedTargetProgress = 0.0f)
	{
		// 뭔가 변경이 일어나는 타이밍을 틱에서 감지하기 위한 뻘짓.
		bool bTimeToBeginNewFakeStep = false;
		if (BasicTargetProgress < InTargetProgressScale && InTargetProgressScale < InNextPredictedTargetProgress)
		{
			bTimeToBeginNewFakeStep = true;
			TimeSinceLastFakeStep = 0.0f;
			RandSetAdjustedFakeStepNumToNextPredictedTarget();
			RandSetAdjustedFakeStepTimeInterval();
		}

		BasicTargetProgress = InTargetProgressScale;
		// 일부 예측 가능한 상황에서 BasicTargetProgress 보다 크게 들어올 것.
		NextPredictedTargetProgress = FMath::Max(InNextPredictedTargetProgress, BasicTargetProgress);

		FinalTargetProgress = FMath::Max(FinalTargetProgress, BasicTargetProgress); // 혹시나 초기화가 안 되었을 때의 예외처리를 먼저 해 두는 거.

		if(NextPredictedTargetProgress > BasicTargetProgress && !bTimeToBeginNewFakeStep && AdjustedFakeStepNumToNextPredictedTarget > 0)
		{
			// NextPredictedTargetProgress 가 다른 경우 이에 따른 훼이크 업데이트

			TimeSinceLastFakeStep += InDeltaTime;
			if (TimeSinceLastFakeStep >= AdjustedFakeStepTimeInterval)
			{
				const float SingleFakeStepInterval = (NextPredictedTargetProgress - BasicTargetProgress) / (float)AdjustedFakeStepNumToNextPredictedTarget;
				//int32 CurrentEstimatedFakeStep = FMath::RoundToInt((FinalTargetProgress - BasicTargetProgress) / SingleFakeStepInterval);
				// 가라 진행도 업데이트 랜덤 가미.
				FinalTargetProgress += FMath::FRandRange(SingleFakeStepInterval * 0.7f, SingleFakeStepInterval * 1.3f);
				// 다음 스텝을 위한 초기화
				TimeSinceLastFakeStep = 0.0f;
				RandSetAdjustedFakeStepTimeInterval();
			}
			// 최종 보정. Fake 스텝을 너무 많이 밟게 되더라도 NextPredictedTargetProgress 를 넘어선 안됨.
			FinalTargetProgress = FMath::Clamp(FinalTargetProgress, BasicTargetProgress, NextPredictedTargetProgress); 
		}
		else 
		{
			FinalTargetProgress = BasicTargetProgress;
		}

		// AnimatedProgressValue 로 먹여주는 값도 fake 업데이트를 좀 하지만 AnimatedProgressValue 자체적으로도 진행도 증가 애니메이션이 있다. 
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
	// iPAD 류의 해상도에서 로딩 스크린 레이아웃에 대한 예외처리를 하기 위해. 일반 UI 에서는 그런 해상도도 UUserInterfaceSettings::GetDPIScaleBasedOnSize 에 가한 수정을 통해 처리됨.

#if !UE_BUILD_SHIPPING
	const UUserInterfaceSettings* UISetting = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass());
	// UIScaleRule 이 혹여나 바뀔 가능성을 다 대비하려니 넘 벅차고 그래도 혹여나 바뀌면 놓치지 않게..
	// 사실 이 가정이 바뀌면 ShouldUseShorterScreenLoadingImageLayout 이름도 반대로 바뀌어야 할 듯. ShouldUseLongerScreenLoadingImageLayout 으로.
	ensure(UISetting && (UISetting->UIScaleRule == EUIScalingRule::ShortestSide || UISetting->UIScaleRule == EUIScalingRule::Horizontal));
#endif

	const float CurrentViewAspectRatio = (CurrViewportSize.Y > KINDA_SMALL_NUMBER) ? (CurrViewportSize.X / CurrViewportSize.Y) : 1.0f;

	// 이건 적당히 산정하는 건데.. GEngine->SlateUIMinAspectRatio 를 바로 쓰지는 않는다. 그 부근에서는 값이 설령 약간 작다고 해도 기본 aspect ratio 를 위한 layout 을 사용해도 문제가 되지 않을 것이므로.
	//侶角刊뎠셕炬돨∼꼇殮쌈賈痰GEngine-SlateUIMinAspectRatio。瞳侶맒쐤，섦賈令�仝☜⌒뿅ð�賈痰샘굶aspect ratio돨layout冷꼇삔唐狂痙。
	const float MinDesiredAspectRatio = 1.0;// (GEngine->SlateUIMinAspectRatio * 0.9f);
	
	return (MinDesiredAspectRatio > 0.0f && CurrentViewAspectRatio < MinDesiredAspectRatio);
}

// BladeII 모듈 쪽에서 적절한 함수를 할당해 주어야 함.
LoadGameSettingFloatFnType gLoadGameFxVolumeSettingFnPtr = nullptr;
LoadGameSettingFloatFnType gLoadGameBGMVolumeSettingFnPtr = nullptr;

//==================================================================================
// SB2LoadingScreenImage. 일반적인 맵 로딩에서 사용하는 로딩 스크린 구현.
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
		// 어느 경우든 DPI Rule 이 짧은 변 기준..
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

		// 특정 크기(비율)를 최적으로 해서 중앙정렬, 아니면 채우기.
		const bool bNonFilledCenterAlignment = !InArgs._bStretchImage;
		const FVector2D CurrentViewSize = InArgs._CurrentViewSize;
		
		const FVector2D DesiredImageSize = bNonFilledCenterAlignment ? // Overlay 로 단순 Fill 을 하면 사이즈를 몰라도 되는데 Canvas 패널로 하려다 보니 어떤 경우든 알아야.. 뭔가 다른 좋은 방법이 없나?
			GetAdjustedViewSizeByAspectRatio(CurrentViewSize, InArgs._OptimalAspectRatio) : CurrentViewSize;

		// iPAD 같은 기본 비율보다 뭉툭한 화면 비율인 경우를 생각해서. 이 때는 양옆을 채우고 아래위를 비우겠지.
		const bool bUseShorterScreenLoadingLayout = ShouldUseShorterScreenLoadingImageLayout(CurrentViewSize);

		//since we are not using game styles here, just load one image
		LoadingScreenBrush = MakeShareable(new FB2GameLoadingScreenBrush(LoadingScreenName, DesiredImageSize));
		
		IndicatorSet.Init();
		IndicatorSet.SetMainImageVisible(bDisplayIndicator);

		// Progress Bar 초기화. 보이지 않게 하려고 해도 일단 해 놓아야.. ㅠㅠ
		LoadingPBSet.Init();
		LoadingPBSet.SetProgressBarVisible(bDisplayProgress);
		if (bDisplayProgress){
			// 실제 progress 변수도 초기화 해야 한다. 이제 맵 로딩 완료될 때까지 이곳저곳에서 progress 를 증가..
			//茄셥돨progress긴좆冷矜狼놓迦뺏。君瞳瞳뒈暠潼흙供냥裂품，瞳몹뇹藤속progress∼
			//GlobalResetMapLoadingProgress();
		}

		InitLoadingMessage();
		
#if PLATFORM_IOS
		// 셰이프 존
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
					/* 중앙정렬시 메인 로딩 이미지만 약간 특수하게 함. 특정 화면 비율에서 꽉 채우고 그게 아니면 잘리거나 비는 식으로 */
					SNew(SCanvas)
					+ SCanvas::Slot()
					.HAlign((bNonFilledCenterAlignment && !bUseShorterScreenLoadingLayout) ? HAlign_Center : HAlign_Fill)
					.VAlign(bUseShorterScreenLoadingLayout ? VAlign_Center : VAlign_Fill)
					/* 중앙정렬을 쓰려면 일반적인 UWidget 배치와는 달리 현재 해상도를 가지고 직접 수정을 좀 해 주어야.. */
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
					// 셰이프 존 영역만큼 + 10 띄움
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
				LMTBStyle->SetFontSize((int32)(15.0f * GetLoadingScreenContentScale()));  // 여기 폰트 사이즈는 별도로 세팅되는군.
				LMTBStyle->SetShadowOffset(FVector2D(1.f, 1.f));
			}
			LoadingMessageTB = SNew(STextBlock).TextStyle(LMTBStyle.Get());

			if (LoadingMessageTB.IsValid())
			{
				// LoadingMessageType 이 추가되면 그에 따라..
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
// SB2MovieLoadingScreen. 동영상이 돌아가는 로딩 스크린 구현. 
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

		// Progress Bar 초기화. 보이지 않게 하려고 해도 일단 해 놓아야.. ㅠㅠ
		LoadingPBSet.Init();
		LoadingPBSet.SetProgressBarVisible(bDisplayProgress);
		if (bDisplayProgress) {
			// 실제 progress 변수도 초기화 해야 한다. 이제 맵 로딩 완료될 때까지 이곳저곳에서 progress 를 증가..
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
// 모듈 구현. 외부에서 직접적으로 사용할 인터페이스
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
			//	return; // 쓸모없는 리소스 쿠킹 제거
			//}
			//else
			 {
				EditorCheckForHardCodedResources(); // 리소스 있는지 체크나
			}
		}
#endif

		FB2GameLoadingScreenBrush::ConditionalLoadFontInfo(); // 이 시점에는 상황에 따라 로딩이 잘 안될 수 있음. 그냥 제거해 버릴까.

		bool bDoSetupFirstTimeLoadingScreen = true;
#if PLATFORM_ANDROID || PLATFORM_IOS 
		//if (IsFinalDLCCookRun())
		//{
		//	bDoSetupFirstTimeLoadingScreen = false; // 최종 DLC 런에서는 여기서 첫 로딩 화면을 띄울 게 아니라서..
		//}
#endif
		
		if (bDoSetupFirstTimeLoadingScreen) // 아마도 최종 배포 버전은 이쪽으로 오지 않을 듯?
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
		//if (GMinimalDLCFrontMode && GIsEditor) { // DLC Front 모드 리소스로딩 최대한 제거. 여긴 일단 쿠킹에서만 제외시키고.. 실제 DLCFrontMode 실행에는 필요할 수도 있다. 필요에 따라 손질.
		//	return;
		//}

		FB2GameLoadingScreenBrush::ConditionalLoadFontInfo(); // StartupModule 시 로딩 안될 수 있는 거 대비..

		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.bMoviesAreSkippable = false;
		LoadingScreen.bWaitForManualStop = false;

		LoadingScreen.WidgetLoadingScreen = SNew(SB2LoadingScreenImage).ImageName(LoadingImageName).LoadingMessageType(InMessageType).
			bDisplayProgress(bInDisplayProgress).bDisplayIndicator(bInDisplayIndicator).
			OptimalAspectRatio(InOptimalAspectRatio).bStretchImage(bFullyStretchImage).CurrentViewSize(InViewSize);
		
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);

		GetMoviePlayer()->PlayMovie(); // 엔진 기본 루틴에 의해 불리긴 함.
	}

	//virtual TArray<B2MovieSubtitle>* StartGameLoadingScreen_Movie(const TCHAR* MovieName, 
	//	ELoadingScreenMovieAlignMode InAlignmentMode = ELoadingScreenMovieAlignMode::FitToMin,
	//	bool bInEnableMovieSkipButton = true,
	//	bool bInDisplayProgressBar = false,
	//	bool bInDisplayIndicator = false,
	//	EMoviePlaybackType InMoviePlayBackType = EMoviePlaybackType::MT_Normal,
	//	bool bAutoCloseOnLoadingComplete = false) override
	//{
	//	if (GMinimalDLCFrontMode && GIsEditor) { // DLC Front 모드 리소스로딩 최대한 제거. 여긴 일단 쿠킹에서만 제외시키고.. 실제 DLCFrontMode 실행에는 필요할 수도 있다. 필요에 따라 손질.
	//		return nullptr;
	//	}

	//	UE_LOG(LogB2GameLoadingScreen, Log, TEXT("StartGameLoadingScreen_Movie with MovieName : %s"), MovieName ? MovieName : TEXT("None"));

	//	FB2GameLoadingScreenBrush::ConditionalLoadFontInfo(); // StartupModule 시 로딩 안될 수 있는 거 대비..
	//	FB2GameLoadingScreenBrush::SetFontSize((int32)15.0f * GetLoadingScreenContentScale()); // 처음 로딩 시 폰트 사이즈가 의도대로 세팅 안될 수 있음.

	//	FLoadingScreenAttributes LoadingScreen; 
	//	LoadingScreen.bAutoCompleteWhenLoadingCompletes = bAutoCloseOnLoadingComplete; // 아마도 Looping 이면 시나리오 동영상이 아닌 사실상 로딩 이미지이니 이걸 true 로 원하겠지만 예외 상황도 있을 것 같다.
	//	LoadingScreen.bMoviesAreSkippable = false;
	//	LoadingScreen.bWaitForManualStop = false;
	//	LoadingScreen.PlaybackType = InMoviePlayBackType;
	//	LoadingScreen.AlignMode = InAlignmentMode;

	//	float LoadedFxVolume = 1.0f, LoadedBGMVolume = 1.0f;
	//	// 저장된 게임 설정에 따라 동영상 사운드를 켜거나 끄거나. 플랫폼 별로 모두 volume 컨트롤까지는 갖춰져 있지 않아서 On/Off 만 함.
	//	if (gLoadGameFxVolumeSettingFnPtr)
	//	{ 
	//		gLoadGameFxVolumeSettingFnPtr(LoadedFxVolume);
	//	}
	//	if (gLoadGameBGMVolumeSettingFnPtr)
	//	{
	//		gLoadGameBGMVolumeSettingFnPtr(LoadedBGMVolume);
	//	}
	//	// 배경음, 효과음 구분도 없음. 여긴 그냥 한 몸체 사운드임. 둘 중 하나만 0 설정이면 끄는 걸로.
	//	LoadingScreen.bIsSoundOn = (LoadedFxVolume > KINDA_SMALL_NUMBER && LoadedBGMVolume > KINDA_SMALL_NUMBER);

	//	TArray<B2MovieSubtitle>* RetArray = NULL;
	//	if (MovieName != NULL)
	//	{
	//		FString MovieNameString(MovieName);
	//		LoadingScreen.MoviePaths.Add(MovieNameString);
	//		// 현재 2개 이상 지원 안됨. Windows 는 될 껀데 AndroidMovieStreamer 랑 AppleMovieStreamer 쪽에 별도로 구현해 주어야 함.
	//		checkSlow(LoadingScreen.MoviePaths.Num() == 1);
	//		
	//		// 동영상 네이밍 필수 맞춰야.
	//		checkSlow(
	//			MovieNameString.StartsWith(FPaths::GetMovieFilePrefix_DLC())
	//			|| MovieNameString.StartsWith(FPaths::GetMovieFilePrefix_APK())
	//		);

	//		// 모바일 배포판에서는 DLC 경로를 사용하도록.. 하지만 일반 (단순 배포) 쿠킹에서 작동할 여지가 있어야 함.
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

	//	//GetMoviePlayer()에서 FNullGameMoviePlayer가 나온 경우. 위에서 구한 RetArray가 Dangling pointer가 됨.
	//	return IsMoviePlayerEnabled() ? RetArray : nullptr; // 여기서 subtitle 세팅할 레퍼런스를 리턴값으로 전달해서 밖에서 세팅하는 구조
	//}
};

/////////////////////////////////////////////////////////////////////////////////
// FLoadingProgressBarAnim

FLoadingProgressBarAnim::FLoadingProgressBarAnim()
{
	///////////// 적절한 설정으로 tweak ..
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
			// 용수철 모델. 레벨이 한번에 여러 단계를 뛰어야 하는 경우에 등속도로 하면 다양한 구간 차이를 커버할 수 있는 적당한 속도를 찾기가 어려울 때가 있어서 용수철 모델을 쓰면 좋음.
			CurrentSpeed += DistToTarget * SpringConst * InDeltaSeconds;
			CurrentSpeed -= CurrentSpeed * DampConst * InDeltaSeconds;
			// 이 경우에도 최소한의 속도는 나오게 함.
			CurrentSpeed = FMath::Max(CurrentSpeed, BaseSpeed);
		}
		// 아래처럼 하면 단순히 선형 증가
		//else
		//{
		//	CurrentSpeed = BaseSpeed;
		//}

		CurrentAnimatedPercentScale += CurrentSpeed * InDeltaSeconds;
	}

	// 다시금 목표값에 도달했는지 체크하고 클램프
	if (GetUnitDistanceToTargetValue() <= 0.0f)
	{ // 목표값에 도달한 상태
		SyncToTargetValue();
	}
}

void FLoadingProgressBarAnim::SetTargetValue(float InTargetPercentScale, bool bSyncCurrentValues)
{
	TargetPercentScale = InTargetPercentScale;

	if (bSyncCurrentValues) // 이건 증가 애니메이션 같은 거 없이 그냥 바로 맞추겠다는 거 
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
