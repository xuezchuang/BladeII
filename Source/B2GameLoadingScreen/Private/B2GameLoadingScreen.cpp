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

// ���⼭ ����ϴ� ���ҽ� ��ũ���� ���� �������� �ϵ��ڵ� �Ǿ� �־��µ� ��� �ұ� �ϴٰ� 
// ���� ��⵵ �ٸ� ���� SomeInfo ���� �͵� �� ���� �ؼ� ���� ���� ���� �ʴ� �ѿ����� �׳� ���⿡ ������ ������.. 
//����ʹ�õ���Դ���ӴӺܾ���ǰ�ͱ�Ӳ�����ˣ���һֱ��˼������ô��
//ʵ��ģ��Ҳ����˳��ʹ��SomeInfo������ֻҪ����Ҫ̫�����־ͷ������
#define B2GAME_LS_MODULE_RES_DEFAULT_LOADING_IMG TEXT("/Game/UI/Loading/B2_MainTitle_BG_01.B2_MainTitle_BG_01")
#define B2GAME_LS_MODULE_RES_PROG_CIRCLE_IMG TEXT("/Game/UI/Loading/B2_Loading_BG_03.B2_Loading_BG_03")
#define B2GAME_LS_MODULE_RES_SKIP_BTN_IMG TEXT("/Game/UI/Loading/B2_BT_Skip_01.B2_BT_Skip_01")

#define B2GAME_LS_MODULE_RES_PROG_GAUGE_BG TEXT("/Game/UI_Resource/RE_Materials/Re_Common/MAT_B2_Common_Gauge_01")
#define B2GAME_LS_MODULE_RES_PROG_GAUGE_FILL TEXT("/Game/UI_Resource/RE_Materials/Re_Common/MAT_B2_Common_Gauge_02")

#if WITH_EDITOR	// ���ҽ� ��� �ϵ��ڵ��� ���� �ּ����� ������ġ
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
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("B2GameLoadingScreen ���� �ʿ�� �ϴ� ���ҽ� �Ϻθ� �ε� ����. ��ǻ�� ����.")));
#endif
	}
}
#endif

// From BladeIILocText.h, dependency problem...
#define B2LOC_CAT_GENERAL TEXT("General")
// BladeII �ʿ� �ִ� BladeIIGetLOCText �� �� �����ͼ�..
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
	// ���� UI �¾����� ���� DPI �������� ������� �����Ƿ� �ʿ��� ������ ���� �����־��.
#if PLATFORM_ANDROID
	// �ȵ���̵忡���� MobileContentScaleFactor �� �ػ󵵿� �ش�.
	static IConsoleVariable* CVarMCSF = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor"));
	check(CVarMCSF);
	return CVarMCSF ? CVarMCSF->GetFloat() : 1.0f;
#else
	return 1.0f; // IOS �� �� �ٸ���.. PC �� ��.
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

	/** 	�� �� �޾��� GC ũ���� ����(B2CLT-1495) ������ �ذ� �õ��� �ϳ��� RootSet ó���� �ϴ� ��.
	 * ���� �̹��� ���� �ϳ� �޸� �����ϴ� �� �� �����ϰ��� �ϴ� ���̰� �Ϲ����� �������̽��� �����ϸ� ���. */
	/*	��Ϊ������������GCײ������(B2CLT - 1495)����Ϊ�������֮һ����RootSet����
		ֻ�������һ��Сͼ����ڴ泣פ��������Ϊһ��Ľ���ͻ�����ѡ�
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
	
	// ini Ư�� ��ġ�� ������ ��Ʈ ��� �����ͼ� �ε�. uasset �� �ƴ�.
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
	// ó�� LoadFontInfo �� �� �� GetLoadingScreenContentScale �� �ǵ��� ���� �������� ���� �� �־ �߰� �߰� ������ ���������� �ʿ�.
	// �ƴ� �׳� �ٸ� ������� ���� �ʹٰų� �� ������..
	static void SetFontSize(int32 NewSize)
	{
		if (FontInfo)
		{
			FontInfo->Size = NewSize;
		}
	}
};

// �ε� ȭ�鿡�� ���� ���ư��� �� ��� �� ��� ����.
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
		// �� �� �޾��� GC ũ���� ����(B2CLT-1495) ������ �ذ� �õ��� �ϳ��� RootSet �� �־��� �ǵ�.. �̰� ������ �� �� ������ ������
		// PieceBrush �� ���̴� �̹����� ���� �Ŷ� �޸𸮿� �����Ѵٰ� �ؼ� ������ ������ ���� ����.
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

// StartupLoadingScreen.. ������ �ᱹ ���� ������ DLC ���� ���� ��ΰ� �ٸ��� �̰� ������� �ʰ� �� ��. ��, ��ǻ� ���߹������� ���?
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

/** �ε� ȭ�鿡�� ���൵ �� ǥ���� �͵� ��Ƴ��� ��. ������ �ٸ��� �ؼ� ������ �� �ƴ�. �׳� �ݺ� ������ ���� */
struct FB2LoadingScreenProgressBarDisplaySet
{
private:
	TSharedPtr<SProgressBar> ThePB;
	/** SProgressBar ���� ����� SlateBrush */
	TSharedPtr<FB2GameLoadingScreenBrush> BGImageBrush;
	TSharedPtr<FB2GameLoadingScreenBrush> FillImageBrush;
	/** ���� �ε� ���൵�� ���⿡ �����ϸ�, ���⼱ ���������� Progress Bar �� �Ѹ� animate �� �ε� ���൵ ���� ����� �ش�. */
	FLoadingProgressBarAnim AnimatedProgressValue;

	float BasicTargetProgress;
	/** ���� ���൵ ������Ʈ���� ���� ��� ���� �󸶰� ���� �˰� �ִ� ��찡 �ִµ�, 
	 * �̷� �� �̰� ���� ����Ǵ� ���൵ ������ �����ؼ� �߰��߰� ��ü������ ���൵�� ����� ���ݾ� �ø����� ��. */
	float NextPredictedTargetProgress;
	/** BasicTarget ���� NextPredicted ���� �ٸ� �� �� ���̿� �ְ� ��. */
	float FinalTargetProgress;
	/** ������. NextPredictedTarget ���� CurrentTarget ���� Ŭ �� �󸶳� �̰� �߰� �߶� ���� ������Ʈ �� ��. */
	const int32 BaseFakeStepNumToNextPredictedTarget = 4;
	/** ������. NextPredictedTarget ���� CurrentTarget ���� Ŭ �� ���� ������Ʈ ������ ���� �ֱ�. */
	const float BaseFakeStepTimeInterval = 1.0f;
	/** BaseFakeStepNumToNextPredictedTarget �̶� BaseFakeStepTimeInterval ���� ������ �� ���� */
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
		// �پ��ϰ� ������ �� �ƴϰ� ����ũ�� ���ҽ� ���� ��. B2PreLoadingScreen ������� �������� UI_Loading WidgetBP �� �����ִ� ��
		BGImageBrush = MakeShareable(new FB2GameLoadingScreenBrush(B2GAME_LS_MODULE_RES_PROG_GAUGE_BG, FVector2D(10.0f, 6.0f) * GetLoadingScreenContentScale()));
		FillImageBrush = MakeShareable(new FB2GameLoadingScreenBrush(B2GAME_LS_MODULE_RES_PROG_GAUGE_FILL, FVector2D(10.0f, 6.0f) * GetLoadingScreenContentScale()));

		check(BGImageBrush.IsValid() && FillImageBrush.IsValid());

		if (BGImageBrush.IsValid() && FillImageBrush.IsValid())
		{
			// ������ �����鵵 ������� �ϴ� �� ����. UI_Loading �� �ٲ�� ���⵵.. 
			BGImageBrush->DrawAs = ESlateBrushDrawType::Box;
			BGImageBrush->TintColor = FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
			BGImageBrush->Margin = FMargin(0.5f, 0.5f, 0.5f, 0.5f);
			FillImageBrush->DrawAs = ESlateBrushDrawType::Box;
			FillImageBrush->TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			FillImageBrush->Margin = FMargin(0.5f, 0.0f, 0.5f, 0.0f);

			// �׸��� ��鵵.. ������ �ε�ȭ�� ũ���� ������ �ذ�� ��Ȳ�� �� �־ ������ Ȯ���� �𸣰ڴµ� �� ������ ��Ʈ�� �ؼ� �ȵ� �͵� ����.
			BGImageBrush->SetResourceObjectAsRootset(true);
			FillImageBrush->SetResourceObjectAsRootset(true);

			ThePB = SNew(SProgressBar).BackgroundImage(BGImageBrush.Get()).FillImage(FillImageBrush.Get());
			check(ThePB.IsValid());
			if (ThePB.IsValid())
			{
				// ���� ����� ���� �⺻���� ������ Overlay ���� ���������δ� ������ �� �Ŷ� ������ ���ؼ� RenderTransform �� �ִ� ��.
				ThePB->SetRenderTransformPivot(FVector2D(0.0, 1.0f));
				ThePB->SetRenderTransform(FSlateRenderTransform(FScale2D(1.0f, 0.6f)));
				ThePB->SetPercent(0.0f);
			}
		}
	}
	void TickLoadingProgress(const float InDeltaTime, const float InTargetProgressScale, const float InNextPredictedTargetProgress = 0.0f)
	{
		// ���� ������ �Ͼ�� Ÿ�̹��� ƽ���� �����ϱ� ���� ����.
		bool bTimeToBeginNewFakeStep = false;
		if (BasicTargetProgress < InTargetProgressScale && InTargetProgressScale < InNextPredictedTargetProgress)
		{
			bTimeToBeginNewFakeStep = true;
			TimeSinceLastFakeStep = 0.0f;
			RandSetAdjustedFakeStepNumToNextPredictedTarget();
			RandSetAdjustedFakeStepTimeInterval();
		}

		BasicTargetProgress = InTargetProgressScale;
		// �Ϻ� ���� ������ ��Ȳ���� BasicTargetProgress ���� ũ�� ���� ��.
		NextPredictedTargetProgress = FMath::Max(InNextPredictedTargetProgress, BasicTargetProgress);

		FinalTargetProgress = FMath::Max(FinalTargetProgress, BasicTargetProgress); // Ȥ�ó� �ʱ�ȭ�� �� �Ǿ��� ���� ����ó���� ���� �� �δ� ��.

		if(NextPredictedTargetProgress > BasicTargetProgress && !bTimeToBeginNewFakeStep && AdjustedFakeStepNumToNextPredictedTarget > 0)
		{
			// NextPredictedTargetProgress �� �ٸ� ��� �̿� ���� ����ũ ������Ʈ

			TimeSinceLastFakeStep += InDeltaTime;
			if (TimeSinceLastFakeStep >= AdjustedFakeStepTimeInterval)
			{
				const float SingleFakeStepInterval = (NextPredictedTargetProgress - BasicTargetProgress) / (float)AdjustedFakeStepNumToNextPredictedTarget;
				//int32 CurrentEstimatedFakeStep = FMath::RoundToInt((FinalTargetProgress - BasicTargetProgress) / SingleFakeStepInterval);
				// ���� ���൵ ������Ʈ ���� ����.
				FinalTargetProgress += FMath::FRandRange(SingleFakeStepInterval * 0.7f, SingleFakeStepInterval * 1.3f);
				// ���� ������ ���� �ʱ�ȭ
				TimeSinceLastFakeStep = 0.0f;
				RandSetAdjustedFakeStepTimeInterval();
			}
			// ���� ����. Fake ������ �ʹ� ���� ��� �Ǵ��� NextPredictedTargetProgress �� �Ѿ �ȵ�.
			FinalTargetProgress = FMath::Clamp(FinalTargetProgress, BasicTargetProgress, NextPredictedTargetProgress); 
		}
		else 
		{
			FinalTargetProgress = BasicTargetProgress;
		}

		// AnimatedProgressValue �� �Կ��ִ� ���� fake ������Ʈ�� �� ������ AnimatedProgressValue ��ü�����ε� ���൵ ���� �ִϸ��̼��� �ִ�. 
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
	// iPAD ���� �ػ󵵿��� �ε� ��ũ�� ���̾ƿ��� ���� ����ó���� �ϱ� ����. �Ϲ� UI ������ �׷� �ػ󵵵� UUserInterfaceSettings::GetDPIScaleBasedOnSize �� ���� ������ ���� ó����.

#if !UE_BUILD_SHIPPING
	const UUserInterfaceSettings* UISetting = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass());
	// UIScaleRule �� Ȥ���� �ٲ� ���ɼ��� �� ����Ϸ��� �� ������ �׷��� Ȥ���� �ٲ�� ��ġ�� �ʰ�..
	// ��� �� ������ �ٲ�� ShouldUseShorterScreenLoadingImageLayout �̸��� �ݴ�� �ٲ��� �� ��. ShouldUseLongerScreenLoadingImageLayout ����.
	ensure(UISetting && (UISetting->UIScaleRule == EUIScalingRule::ShortestSide || UISetting->UIScaleRule == EUIScalingRule::Horizontal));
#endif

	const float CurrentViewAspectRatio = (CurrViewportSize.Y > KINDA_SMALL_NUMBER) ? (CurrViewportSize.X / CurrViewportSize.Y) : 1.0f;

	// �̰� ������ �����ϴ� �ǵ�.. GEngine->SlateUIMinAspectRatio �� �ٷ� ������ �ʴ´�. �� �αٿ����� ���� ���� �ణ �۴ٰ� �ص� �⺻ aspect ratio �� ���� layout �� ����ص� ������ ���� ���� ���̹Ƿ�.
	//�����ʵ�����ġ���ֱ��ʹ��GEngine-SlateUIMinAspectRatio�����⸽������ʹֵ��΢СһЩ��ʹ�û���aspect ratio��layoutҲ���������⡣
	const float MinDesiredAspectRatio = 1.0;// (GEngine->SlateUIMinAspectRatio * 0.9f);
	
	return (MinDesiredAspectRatio > 0.0f && CurrentViewAspectRatio < MinDesiredAspectRatio);
}

// BladeII ��� �ʿ��� ������ �Լ��� �Ҵ��� �־�� ��.
LoadGameSettingFloatFnType gLoadGameFxVolumeSettingFnPtr = nullptr;
LoadGameSettingFloatFnType gLoadGameBGMVolumeSettingFnPtr = nullptr;

//==================================================================================
// SB2LoadingScreenImage. �Ϲ����� �� �ε����� ����ϴ� �ε� ��ũ�� ����.
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
		// ��� ���� DPI Rule �� ª�� �� ����..
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

		// Ư�� ũ��(����)�� �������� �ؼ� �߾�����, �ƴϸ� ä���.
		const bool bNonFilledCenterAlignment = !InArgs._bStretchImage;
		const FVector2D CurrentViewSize = InArgs._CurrentViewSize;
		
		const FVector2D DesiredImageSize = bNonFilledCenterAlignment ? // Overlay �� �ܼ� Fill �� �ϸ� ����� ���� �Ǵµ� Canvas �гη� �Ϸ��� ���� � ���� �˾ƾ�.. ���� �ٸ� ���� ����� ����?
			GetAdjustedViewSizeByAspectRatio(CurrentViewSize, InArgs._OptimalAspectRatio) : CurrentViewSize;

		// iPAD ���� �⺻ �������� ������ ȭ�� ������ ��츦 �����ؼ�. �� ���� �翷�� ä��� �Ʒ����� ������.
		const bool bUseShorterScreenLoadingLayout = ShouldUseShorterScreenLoadingImageLayout(CurrentViewSize);

		//since we are not using game styles here, just load one image
		LoadingScreenBrush = MakeShareable(new FB2GameLoadingScreenBrush(LoadingScreenName, DesiredImageSize));
		
		IndicatorSet.Init();
		IndicatorSet.SetMainImageVisible(bDisplayIndicator);

		// Progress Bar �ʱ�ȭ. ������ �ʰ� �Ϸ��� �ص� �ϴ� �� ���ƾ�.. �Ф�
		LoadingPBSet.Init();
		LoadingPBSet.SetProgressBarVisible(bDisplayProgress);
		if (bDisplayProgress){
			// ���� progress ������ �ʱ�ȭ �ؾ� �Ѵ�. ���� �� �ε� �Ϸ�� ������ �̰��������� progress �� ����..
			//ʵ�ʵ�progress����Ҳ��Ҫ��ʼ���������ڵ�ͼ�������֮ǰ���ڸ�������progress��
			//GlobalResetMapLoadingProgress();
		}

		InitLoadingMessage();
		
#if PLATFORM_IOS
		// ������ ��
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
					/* �߾����Ľ� ���� �ε� �̹����� �ణ Ư���ϰ� ��. Ư�� ȭ�� �������� �� ä��� �װ� �ƴϸ� �߸��ų� ��� ������ */
					SNew(SCanvas)
					+ SCanvas::Slot()
					.HAlign((bNonFilledCenterAlignment && !bUseShorterScreenLoadingLayout) ? HAlign_Center : HAlign_Fill)
					.VAlign(bUseShorterScreenLoadingLayout ? VAlign_Center : VAlign_Fill)
					/* �߾������� ������ �Ϲ����� UWidget ��ġ�ʹ� �޸� ���� �ػ󵵸� ������ ���� ������ �� �� �־��.. */
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
					// ������ �� ������ŭ + 10 ���
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
				LMTBStyle->SetFontSize((int32)(15.0f * GetLoadingScreenContentScale()));  // ���� ��Ʈ ������� ������ ���õǴ±�.
				LMTBStyle->SetShadowOffset(FVector2D(1.f, 1.f));
			}
			LoadingMessageTB = SNew(STextBlock).TextStyle(LMTBStyle.Get());

			if (LoadingMessageTB.IsValid())
			{
				// LoadingMessageType �� �߰��Ǹ� �׿� ����..
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
// SB2MovieLoadingScreen. �������� ���ư��� �ε� ��ũ�� ����. 
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

		// Progress Bar �ʱ�ȭ. ������ �ʰ� �Ϸ��� �ص� �ϴ� �� ���ƾ�.. �Ф�
		LoadingPBSet.Init();
		LoadingPBSet.SetProgressBarVisible(bDisplayProgress);
		if (bDisplayProgress) {
			// ���� progress ������ �ʱ�ȭ �ؾ� �Ѵ�. ���� �� �ε� �Ϸ�� ������ �̰��������� progress �� ����..
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
// ��� ����. �ܺο��� ���������� ����� �������̽�
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
			//	return; // ������� ���ҽ� ��ŷ ����
			//}
			//else
			 {
				EditorCheckForHardCodedResources(); // ���ҽ� �ִ��� üũ��
			}
		}
#endif

		FB2GameLoadingScreenBrush::ConditionalLoadFontInfo(); // �� �������� ��Ȳ�� ���� �ε��� �� �ȵ� �� ����. �׳� ������ ������.

		bool bDoSetupFirstTimeLoadingScreen = true;
#if PLATFORM_ANDROID || PLATFORM_IOS 
		//if (IsFinalDLCCookRun())
		//{
		//	bDoSetupFirstTimeLoadingScreen = false; // ���� DLC �������� ���⼭ ù �ε� ȭ���� ��� �� �ƴ϶�..
		//}
#endif
		
		if (bDoSetupFirstTimeLoadingScreen) // �Ƹ��� ���� ���� ������ �������� ���� ���� ��?
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
		//if (GMinimalDLCFrontMode && GIsEditor) { // DLC Front ��� ���ҽ��ε� �ִ��� ����. ���� �ϴ� ��ŷ������ ���ܽ�Ű��.. ���� DLCFrontMode ���࿡�� �ʿ��� ���� �ִ�. �ʿ信 ���� ����.
		//	return;
		//}

		FB2GameLoadingScreenBrush::ConditionalLoadFontInfo(); // StartupModule �� �ε� �ȵ� �� �ִ� �� ���..

		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.bMoviesAreSkippable = false;
		LoadingScreen.bWaitForManualStop = false;

		LoadingScreen.WidgetLoadingScreen = SNew(SB2LoadingScreenImage).ImageName(LoadingImageName).LoadingMessageType(InMessageType).
			bDisplayProgress(bInDisplayProgress).bDisplayIndicator(bInDisplayIndicator).
			OptimalAspectRatio(InOptimalAspectRatio).bStretchImage(bFullyStretchImage).CurrentViewSize(InViewSize);
		
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);

		GetMoviePlayer()->PlayMovie(); // ���� �⺻ ��ƾ�� ���� �Ҹ��� ��.
	}

	//virtual TArray<B2MovieSubtitle>* StartGameLoadingScreen_Movie(const TCHAR* MovieName, 
	//	ELoadingScreenMovieAlignMode InAlignmentMode = ELoadingScreenMovieAlignMode::FitToMin,
	//	bool bInEnableMovieSkipButton = true,
	//	bool bInDisplayProgressBar = false,
	//	bool bInDisplayIndicator = false,
	//	EMoviePlaybackType InMoviePlayBackType = EMoviePlaybackType::MT_Normal,
	//	bool bAutoCloseOnLoadingComplete = false) override
	//{
	//	if (GMinimalDLCFrontMode && GIsEditor) { // DLC Front ��� ���ҽ��ε� �ִ��� ����. ���� �ϴ� ��ŷ������ ���ܽ�Ű��.. ���� DLCFrontMode ���࿡�� �ʿ��� ���� �ִ�. �ʿ信 ���� ����.
	//		return nullptr;
	//	}

	//	UE_LOG(LogB2GameLoadingScreen, Log, TEXT("StartGameLoadingScreen_Movie with MovieName : %s"), MovieName ? MovieName : TEXT("None"));

	//	FB2GameLoadingScreenBrush::ConditionalLoadFontInfo(); // StartupModule �� �ε� �ȵ� �� �ִ� �� ���..
	//	FB2GameLoadingScreenBrush::SetFontSize((int32)15.0f * GetLoadingScreenContentScale()); // ó�� �ε� �� ��Ʈ ����� �ǵ���� ���� �ȵ� �� ����.

	//	FLoadingScreenAttributes LoadingScreen; 
	//	LoadingScreen.bAutoCompleteWhenLoadingCompletes = bAutoCloseOnLoadingComplete; // �Ƹ��� Looping �̸� �ó����� �������� �ƴ� ��ǻ� �ε� �̹����̴� �̰� true �� ���ϰ����� ���� ��Ȳ�� ���� �� ����.
	//	LoadingScreen.bMoviesAreSkippable = false;
	//	LoadingScreen.bWaitForManualStop = false;
	//	LoadingScreen.PlaybackType = InMoviePlayBackType;
	//	LoadingScreen.AlignMode = InAlignmentMode;

	//	float LoadedFxVolume = 1.0f, LoadedBGMVolume = 1.0f;
	//	// ����� ���� ������ ���� ������ ���带 �Ѱų� ���ų�. �÷��� ���� ��� volume ��Ʈ�ѱ����� ������ ���� �ʾƼ� On/Off �� ��.
	//	if (gLoadGameFxVolumeSettingFnPtr)
	//	{ 
	//		gLoadGameFxVolumeSettingFnPtr(LoadedFxVolume);
	//	}
	//	if (gLoadGameBGMVolumeSettingFnPtr)
	//	{
	//		gLoadGameBGMVolumeSettingFnPtr(LoadedBGMVolume);
	//	}
	//	// �����, ȿ���� ���е� ����. ���� �׳� �� ��ü ������. �� �� �ϳ��� 0 �����̸� ���� �ɷ�.
	//	LoadingScreen.bIsSoundOn = (LoadedFxVolume > KINDA_SMALL_NUMBER && LoadedBGMVolume > KINDA_SMALL_NUMBER);

	//	TArray<B2MovieSubtitle>* RetArray = NULL;
	//	if (MovieName != NULL)
	//	{
	//		FString MovieNameString(MovieName);
	//		LoadingScreen.MoviePaths.Add(MovieNameString);
	//		// ���� 2�� �̻� ���� �ȵ�. Windows �� �� ���� AndroidMovieStreamer �� AppleMovieStreamer �ʿ� ������ ������ �־�� ��.
	//		checkSlow(LoadingScreen.MoviePaths.Num() == 1);
	//		
	//		// ������ ���̹� �ʼ� �����.
	//		checkSlow(
	//			MovieNameString.StartsWith(FPaths::GetMovieFilePrefix_DLC())
	//			|| MovieNameString.StartsWith(FPaths::GetMovieFilePrefix_APK())
	//		);

	//		// ����� �����ǿ����� DLC ��θ� ����ϵ���.. ������ �Ϲ� (�ܼ� ����) ��ŷ���� �۵��� ������ �־�� ��.
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

	//	//GetMoviePlayer()���� FNullGameMoviePlayer�� ���� ���. ������ ���� RetArray�� Dangling pointer�� ��.
	//	return IsMoviePlayerEnabled() ? RetArray : nullptr; // ���⼭ subtitle ������ ���۷����� ���ϰ����� �����ؼ� �ۿ��� �����ϴ� ����
	//}
};

/////////////////////////////////////////////////////////////////////////////////
// FLoadingProgressBarAnim

FLoadingProgressBarAnim::FLoadingProgressBarAnim()
{
	///////////// ������ �������� tweak ..
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
			// ���ö ��. ������ �ѹ��� ���� �ܰ踦 �پ�� �ϴ� ��쿡 ��ӵ��� �ϸ� �پ��� ���� ���̸� Ŀ���� �� �ִ� ������ �ӵ��� ã�Ⱑ ����� ���� �־ ���ö ���� ���� ����.
			CurrentSpeed += DistToTarget * SpringConst * InDeltaSeconds;
			CurrentSpeed -= CurrentSpeed * DampConst * InDeltaSeconds;
			// �� ��쿡�� �ּ����� �ӵ��� ������ ��.
			CurrentSpeed = FMath::Max(CurrentSpeed, BaseSpeed);
		}
		// �Ʒ�ó�� �ϸ� �ܼ��� ���� ����
		//else
		//{
		//	CurrentSpeed = BaseSpeed;
		//}

		CurrentAnimatedPercentScale += CurrentSpeed * InDeltaSeconds;
	}

	// �ٽñ� ��ǥ���� �����ߴ��� üũ�ϰ� Ŭ����
	if (GetUnitDistanceToTargetValue() <= 0.0f)
	{ // ��ǥ���� ������ ����
		SyncToTargetValue();
	}
}

void FLoadingProgressBarAnim::SetTargetValue(float InTargetPercentScale, bool bSyncCurrentValues)
{
	TargetPercentScale = InTargetPercentScale;

	if (bSyncCurrentValues) // �̰� ���� �ִϸ��̼� ���� �� ���� �׳� �ٷ� ���߰ڴٴ� �� 
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
