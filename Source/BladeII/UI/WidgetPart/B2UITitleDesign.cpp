#include "bladeII.h"
#include "B2UITitleDesign.h"
#include "B2UITitleDLC.h"
#include "B2UITitleScreen.h"
#include "B2UIEnum.h"
#include "Event.h"
#include "B2UIManager.h"
#include "B2AuthInfo.h"
#include "B2DLCFrontGameMode.h"
#include "MediaSource.h"
#include "MediaPlayer.h"
#include "Global.h"
#include "BladeIIUtil.h"
#if !UE_BUILD_SHIPPING
#include "B2UIWidgetBase.h"
#endif

/************************************************************************/
/* 
 [ B2UITitleDesign ]

 UITitleScreen ���� ��������� ����� UITitleDLC �� �������� ������ ä �Ű����� 
 �ߺ��� �κ��� ��Ʈ�� ��Ƴ���.
 ������ ���� (Ÿ��Ʋ ������) �κ��̸� DLC �� �α��� ����� ���⼱ ����
 DLCFront ���� ���� �ǹǷ� UIWidgetBase �� ��ӹ��� ����.

*/
/************************************************************************/

UMediaSource* FB2UITitleMovieSetup::GetMovieSource(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded)
{
	// �� �� ���� ���ҽ��� TAsset ���� ����µ� ���⼱ ���� �ǹ̵� ���� ��½ Ÿ�̹��� �̻������� �� ���Ƽ� �׳����� �ٲ�
	//if (bLoadIfUnloaded)
	//{
	//	if (!MovieSource.IsValid())
	//	{ // �̰� ����ϴ� ������ ������ async �ε��� ���ư� �������� ���� �ɷ�.
	//		MovieSource.LoadSynchronous();
	//	}
	//	// ���� LoadSynchronous �ؼ� ������ ���� ���ư� �� ���� ���ص� �Ǵµ� ��� ������ �÷��̾� ����ʿ��� ���۷��� ������ �� �ȵǴ��� �ٷ� GC �Ǵ� ��찡 �־ ĳ���� ��.
	//	LoadedCacher.LoadedMovieSource = MovieSource.Get();
	//}

	return MovieSource;
}
UMediaPlayer* FB2UITitleMovieSetup::GetMoviePlayer(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded)
{
	// �� �� ���� ���ҽ��� TAsset ���� ����µ� ���⼱ ���� �ǹ̵� ���� ��½ Ÿ�̹��� �̻������� �� ���Ƽ� �׳����� �ٲ�
	//if (bLoadIfUnloaded)
	//{
	//	if (!PlayerSetup.IsValid())
	//	{
	//		PlayerSetup.LoadSynchronous();
	//	}
	//	LoadedCacher.LoadedMoviePlayer = PlayerSetup.Get();
	//}
	return PlayerSetup;
}

USoundCue* FB2UITitleMovieSetup::GetMoiveSoundCue(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded)
{
	if (bLoadIfUnloaded && (!LoadedCacher.LoadedMovieSoundCue || !MoiveSoundCue.IsValid()))
	{
		MoiveSoundCue.LoadSynchronous();
		LoadedCacher.LoadedMovieSoundCue = MoiveSoundCue.Get();
	}
	return LoadedCacher.LoadedMovieSoundCue;
}
UTexture2D* FB2UITitleMovieSetup::GetFallbackSnapshotImage(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded)
{
	if (bLoadIfUnloaded && (!LoadedCacher.LoadedFallbackSnapshotImage || !FallbackSnapshotImage.IsValid()))
	{
		FallbackSnapshotImage.LoadSynchronous();
		LoadedCacher.LoadedFallbackSnapshotImage = FallbackSnapshotImage.Get();
	}
	return LoadedCacher.LoadedFallbackSnapshotImage;
}

UB2UITitleDesign::UB2UITitleDesign(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OwnerDLCTitle = nullptr;
	OwnerLobbyTitle = nullptr;

	ReaptedStartMovieCountdown = 10;

#if !UE_BUILD_SHIPPING // Ȥ���� ������ �𸣰� �ٲٴ� �� ����
	check(!IsA(UB2UIWidgetBase::StaticClass()));
#endif
}

void UB2UITitleDesign::Init(UB2UnitedWidgetBase* InOwnerWidget)
{
	OwnerDLCTitle = Cast<UB2UITitleDLC>(InOwnerWidget);
	OwnerLobbyTitle = Cast<UB2UITitleScreen>(InOwnerWidget);

	check(OwnerDLCTitle || OwnerLobbyTitle);
	
	CacheAssets();
}

void UB2UITitleDesign::DestroySelf()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(ReaptedStartMovieTH);
	}

	TurnOffStartMovies();
	DestroySelfCommon();
}

#define _UITITLEDESIGN_STARTBACKMOVIEIMAGE_GET_CODEBLOCK(ClassEnum, ClassName) \
	TWeakObjectPtr<UImage> StartBackMovie_##ClassName; \
	GET_SLOT(UImage, StartBackMovie_##ClassName); \
	/* �̸��� �ϵ��ڵ����� ���µ� assert �� �ɾ��. */ \
	checkSlow(StartBackMovie_##ClassName.IsValid()); \
	StartMovieImagePerPC.Add(ClassEnum, StartBackMovie_##ClassName);

void UB2UITitleDesign::CacheAssets()
{	
	StartMovieImagePerPC.Empty();

	_UITITLEDESIGN_STARTBACKMOVIEIMAGE_GET_CODEBLOCK(EPCClass::EPC_Gladiator, Gladiator)
	_UITITLEDESIGN_STARTBACKMOVIEIMAGE_GET_CODEBLOCK(EPCClass::EPC_Assassin, Assassin)
	_UITITLEDESIGN_STARTBACKMOVIEIMAGE_GET_CODEBLOCK(EPCClass::EPC_Wizard, Wizard)
	_UITITLEDESIGN_STARTBACKMOVIEIMAGE_GET_CODEBLOCK(EPCClass::EPC_Fighter, Fighter)

	GET_SLOT(UImage, StartBackMovie_ApkOnly);
	checkSlow(StartBackMovie_ApkOnly.IsValid());

	SetHiddenAllBackImages();

	GET_SLOT(UPanelWidget, PN_BI);
	GET_SLOT(UPanelWidget, P_OtherCIs)
	GET_SLOT(UPanelWidget, P_StartTextSet);
	GET_SLOT(UTextBlock, TB_Start);
	if (TB_Start.IsValid())
		TB_Start->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Title_TouchToStart")));

	GET_SLOT(UImage, IMG_Loading);
	if (IMG_Loading.IsValid())
	{
		IMG_Loading->SetVisibility(ESlateVisibility::Collapsed);
	}
	GET_SLOT(UWidgetAnimation, ANIM_Loading);

	GET_SLOT(UImage, IMG_FallbackMovieSnapshot);
}

void UB2UITitleDesign::SetConnectingUI()
{
	if (TB_Start.IsValid())
		TB_Start->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Title_Connecting")));

	if (IMG_Loading.IsValid())
	{
		IMG_Loading->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (ANIM_Loading.IsValid())
	{
		PlayAnimation(ANIM_Loading.Get(), 0.0f, 0);
	}
}

bool UB2UITitleDesign::IsLimitedResourceAvailable() const
{
	// DLC Download �� �� �� ��Ȳ���� apk �� ���� �����Ǵ� Ư�� �������� ����ؾ� ��.
	return /*IsFinalDLCCookRun() &&*/ !IsDLCDownload();
}

FB2UITitleMovieSetup* UB2UITitleDesign::GetMovieSetup(bool bApkOnly, EPCClass InPCClass)
{
	if (bApkOnly) {
		return &MovieSetup_ApkOnly;
	}

	for (FB2UITitleMovieSetup& CheckSetup : MovieSetup_Normal)
	{
		if (CheckSetup.RepresentingClass == InPCClass)
		{
			return &CheckSetup;
		}
	}
	return nullptr;
}
FB2UITitleMovieSetup* UB2UITitleDesign::GetMovieSetupSafe(bool bApkOnly, EPCClass InPCClass)
{
	FB2UITitleMovieSetup* FoundSetup = GetMovieSetup(bApkOnly, InPCClass);

	// ���� ���� ���¿� DLC �ٿ�ε� ���� ���°� ������ ���.. MovieSource �� MoviePlayer �� NULL �� �� �ְ�, ������ ������ ������ ���� ���� �ִ�.
	// ����ó���� �� �Ϸ��� ���� Get �Լ��ε� ����ٴ��ϰ� �ϳ� ���±�..  �Ѥ�
	UMediaSource* FoundMediaSourceOfSetup = FoundSetup ? FoundSetup->GetMovieSource(CurrLoadedMovieResourceCache) : nullptr;
	UMediaPlayer* FoundMediaPlayerOfSetup = FoundSetup ? FoundSetup->GetMoviePlayer(CurrLoadedMovieResourceCache) : nullptr;
	const USoundCue* FoundSoundCueOfSetup = FoundSetup ? FoundSetup->GetMoiveSoundCue(CurrLoadedMovieResourceCache) : nullptr;

	bool bMediaSourceValid = false;
	if (FoundMediaSourceOfSetup && FoundMediaPlayerOfSetup 
		//&& FoundSoundCueOfSetup �̰ͱ��� �˻��� �ʿ� ���� ���ϴ� ��ŵ. �̰� Ư���� ����� Ư�� �� ���ҽ� �¾� ���ٳ��Ⱑ ��� �׷� ��� ������ �ٷ� �巯������ �ʰ� �ȴ�.
		)
	{
#if PLATFORM_IOS
		if (FoundMediaPlayerOfSetup->CanPlaySource(FoundMediaSourceOfSetup))
#else 
		// DLC �������� FoundMediaSourceOfSetup->Validate() �� �����ϰ� �Ǿ� �־ �װɷ� üũ �� ��..
		if (FoundMediaPlayerOfSetup->OpenSource(FoundMediaSourceOfSetup))
#endif
		{
			bMediaSourceValid = true;
#if !PLATFORM_IOS
			FoundMediaPlayerOfSetup->Close(); // Valid üũ�� �ϱ� ���� Open �� ���̹Ƿ� �ٷ� ����.
#endif
		}
	}

	if (!bMediaSourceValid)
	{
#if !UE_BUILD_SHIPPING
		UE_LOG(LogBladeII, Log, TEXT("TitleDesign Fallback to Apk Movie.. from %d, SetupFound %d, MovieSource %d, SourceValid %d"),
			(int32)InPCClass,
			FoundSetup ? 1 : 0,
			FoundMediaSourceOfSetup ? 1 : 0,
			(int32)bMediaSourceValid
		);
#endif
		// �� ��� Apk �� �ִ� �� ������.
		FoundSetup = GetMovieSetup(true, InPCClass); // Apk �� �ִ� �� �� �� Ŭ������ �Ŷ� InPCClass ������ �ǹ̰� ������ Ȥ���� �� ���� ���� ������ ����.
	}
	return FoundSetup;
}

TWeakObjectPtr<UImage> UB2UITitleDesign::GetMovieBackImageOfSetup(FB2UITitleMovieSetup* InSetup)
{
	//// InSetup �� �ش��ϴ� ���̽� Image Widget �� ����
	//if (InSetup)
	//{
	//	UMediaSource* MediaSourceOfSetup = InSetup->GetMovieSource(CurrLoadedMovieResourceCache);
	//	if (MediaSourceOfSetup)
	//	{
	//		if (!MediaSourceOfSetup->IsUsingDLCPath()) // IsUsingDLCPath �� �ƴϸ� Apk ���̶� ���.
	//		{
	//			return StartBackMovie_ApkOnly;
	//		}
	//		else
	//		{
	//			TWeakObjectPtr<UImage>* FoundImage = StartMovieImagePerPC.Find(InSetup->RepresentingClass);
	//			return FoundImage ? *FoundImage : nullptr;
	//		}
	//	}
	//}
	// � ���� ������ �̰ɷ�..
	return StartBackMovie_ApkOnly;
}

void UB2UITitleDesign::SetHiddenAllBackImages()
{
	for (TMap<EPCClass, TWeakObjectPtr<UImage>>::TIterator ImgIt(StartMovieImagePerPC); ImgIt; ++ImgIt)
	{
		TWeakObjectPtr<UImage>& ThisMovieImage = ImgIt.Value();
		if (ThisMovieImage.IsValid())
		{
			ThisMovieImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (StartBackMovie_ApkOnly.IsValid())
	{
		StartBackMovie_ApkOnly->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UITitleDesign::StartMovieRandomPick()
{
	// Random Pick �� ���� ���������� �� ó�� �����ϴ� DLCFrontGM ���� ����ϰ�
	// ���߹��������� �κ� Ÿ��Ʋ���� ���.	

	FB2UITitleMovieSetup* RandomPickedSetup = nullptr;
	{
		EPCClass RandomPickedClass = IntToPCClass(FMath::RandRange(0, GetMaxPCClassNum() - 1));
		RandomPickedSetup = GetMovieSetupSafe(IsLimitedResourceAvailable(), RandomPickedClass);
		// RandomPickedClass �� RandomPickedSetup->RepresentingClass �� �ٸ� �� �ִ�.
	}

	check(RandomPickedSetup);
//	if (RandomPickedSetup)
//	{
//		StartMovie(RandomPickedSetup->RepresentingClass);
//		// ���� �ٸ� ȭ�鿡���� �̾ ����ϱ� ���� ���� ������ ������ ������ �д�.
//		if (OwnerDLCTitle)
//		{ 
//			OwnerDLCTitle->SetSelectedMediaSourceInfo(RandomPickedSetup->GetMovieSource(CurrLoadedMovieResourceCache), RandomPickedSetup->RepresentingClass);
//		}
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		else if(!IsFinalDLCCookRun())
//		{ // �� ���� �׳� ���߹��� �����̰ų� NonDLC ��ŷ�� ���. �׽�Ʈ ���̶� ȥ�� ������ ���� �ּ����� ������ �־���.
//			AB2DLCFrontGameMode::DevSetFrontTitleMovieClassOnly(RandomPickedSetup->RepresentingClass);
//		}
//#endif
//	}
}

void UB2UITitleDesign::StartMovie(EPCClass InWantedClass)
{
	FB2UITitleMovieSetup* WantedSetup = GetMovieSetupSafe(IsLimitedResourceAvailable(), InWantedClass);
	if (WantedSetup)
	{
		ReaptedStartMovieBySetupCommon(*WantedSetup);
		StartMovieBP(); // �������� widget �ִϸ��̼� �÷��̰� �ǰ���..
		StartMovieSound(WantedSetup->GetMoiveSoundCue(CurrLoadedMovieResourceCache));
	}
}

void UB2UITitleDesign::StartMovieImmediate(EPCClass InWantedClass)
{
	// �ٸ� ��ҵ��� UI Widget Anim �� ��ŵ�ϰ� ù Ÿ��Ʋ ȭ�鿡�� ������ �� �״�� �����ϴ� ��������..
	
	FB2UITitleMovieSetup* WantedSetup = GetMovieSetupSafe(IsLimitedResourceAvailable(), InWantedClass);
	if (WantedSetup)
	{
		ReaptedStartMovieBySetupCommon(*WantedSetup, 1); // 0 �ʺ��ʹ� � ȭ���̶� 1 �ʺ��� �÷���.
		StartMovieImmediateBP(); // Blueprint ���� �ƹ��͵� ���ϵ���, ���� �ϸ� ÷���� ������ �� ������..
		StartMovieSound(WantedSetup->GetMoiveSoundCue(CurrLoadedMovieResourceCache));
	}

	// ���� ���� �� ����
	if (P_StartTextSet.IsValid())
	{ 
		P_StartTextSet->SetVisibility(ESlateVisibility::Hidden);
	}
}

// ���� �� ���� Ƚ�� ��õ��� �Ϸ��� �̰� ���� ����ϴ� ��� ReaptedStartMovieBySetupCommon �� ���.
bool UB2UITitleDesign::StartMovieBySetupCommon(FB2UITitleMovieSetup& InMovieSetup, int32 InDesiredStartPosSec)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UITitleDesign::StartMovieBySetupCommon PCClass %d"), PCClassToInt(InMovieSetup.RepresentingClass)));

	bool bRetVal = false;

	UMediaSource* TheMovieSource = InMovieSetup.GetMovieSource(CurrLoadedMovieResourceCache);
	UMediaPlayer* TheMoviePlayer = InMovieSetup.GetMoviePlayer(CurrLoadedMovieResourceCache);
	TWeakObjectPtr<UImage> MovieBackImage = GetMovieBackImageOfSetup(&InMovieSetup);
	
	SetHiddenAllBackImages(); // �� �� �� ���ܳ���..
	if (MovieBackImage.IsValid())
	{ // HitTestInvisible �� ���� �� ��. ���� �ϳ��� Visible �� ���� �־�� TitleDLC ���� Touch ��ȣ�� ���� ���� ����.
		MovieBackImage->SetVisibility(ESlateVisibility::Visible);
	}
	if (TheMovieSource && TheMoviePlayer)
	{
		// �÷��� ����.
		if (TheMoviePlayer->OpenSource(TheMovieSource)) // ���⼭ �Ͻ������� �����ϴ� ��찡 �ִٴ� ī���� ����� ���� �ٰ� ����.
		{
			bRetVal = true;
			TheMoviePlayer->Play();
		}
		
		if (InDesiredStartPosSec > 0)
		{ // Ư�� ��ġ���� �����ֱ� ���� ��.. 
			TheMoviePlayer->Seek(FTimespan(0, 0, InDesiredStartPosSec));
		}

		// �� Movie ���� ������ BI ��ġ�� ����.
		if (PN_BI.IsValid())
		{
			PN_BI->SetRenderTranslation(InMovieSetup.BITranslation);
		}
	}

	if(!bRetVal)
	{ // �����ϴ� ��� �ڿ� �ִ� Fallback �̹����� ��� ������.
	  // ���� ���ο� ������� ���̽��� �׻� ���̵��� �ϰ� ������ Movie �÷��̿����� ������ Image widget �� ����� �ؼ� Ư���� ���� ���� ���̴� �׷��� �� �� ����.
		ShowTitleMovieFallbackImage(InMovieSetup);
	}

	return bRetVal;
}

void UB2UITitleDesign::ReaptedStartMovieBySetupCommon(FB2UITitleMovieSetup InMovieSetup, int32 InDesiredStartPosSec)
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(ReaptedStartMovieTH);
	}

	const bool bStartMovieSucceeded = StartMovieBySetupCommon(InMovieSetup, InDesiredStartPosSec);

	// StartMovieBySetupCommon ���� �� ���� Ƚ������ ��õ��� �Ѵ�.
	if (!bStartMovieSucceeded && ReaptedStartMovieCountdown > 0 && TheWorld)
	{ // �ð��� ���⼭ ������ ��. ���� �ణ �Ŀ� ��õ�.
		TheWorld->GetTimerManager().SetTimer(ReaptedStartMovieTH, FTimerDelegate::CreateUObject(this, &UB2UITitleDesign::ReaptedStartMovieBySetupCommon, InMovieSetup, InDesiredStartPosSec), 0.1f, false);
		--ReaptedStartMovieCountdown;
	}
}

void UB2UITitleDesign::ShowTitleMovieFallbackImage(FB2UITitleMovieSetup& InMovieSetup)
{ // � �����ε� Ÿ��Ʋ �������� ������ ���ϴ� ��Ȳ���� �������� ĸ���� �̹����� ���̵��� �ϱ� ����.
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UITitleDesign::ShowTitleMovieFallbackImage PCClass %d"), PCClassToInt(InMovieSetup.RepresentingClass)));

	if (IMG_FallbackMovieSnapshot.IsValid())
	{
		SetHiddenAllBackImages();
		// HitTestInvisible �� ���� �� ��. ���� �ϳ��� Visible �� ���� �־�� TitleDLC ���� Touch ��ȣ�� ���� ���� ����.
		IMG_FallbackMovieSnapshot->SetVisibility(ESlateVisibility::Visible);
		UTexture2D* LoadedImage = InMovieSetup.GetFallbackSnapshotImage(CurrLoadedMovieResourceCache);
		if (LoadedImage)
		{
			IMG_FallbackMovieSnapshot->SetBrushFromTexture(LoadedImage);
		}
	}
}

void UB2UITitleDesign::StartMovieSound(USoundCue* pSoundCue)
{
	StopCurrentMovieSound();

	if (pSoundCue)
		CurrentMovieSound = UGameplayStatics::SpawnSound2D(this, pSoundCue);
}

void UB2UITitleDesign::StopCurrentMovieSound()
{
	if (CurrentMovieSound)
	{
		CurrentMovieSound->Stop();
		CurrentMovieSound->ConditionalBeginDestroy();
		CurrentMovieSound = nullptr;
	}
}

void UB2UITitleDesign::SetShowOtherCIs(bool bShow)
{
	if (P_OtherCIs.IsValid())
	{
		P_OtherCIs->SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UITitleDesign::PlayTouchAnim()
{
	PlayTouchAnimBP();
}

void UB2UITitleDesign::TurnOffStartMovies()
{
#if !PLATFORM_IOS
	// �̰� Ȯ���� �� ���� �� �κ� ������ ���Ŀ��� ������ ��׶��忡 Ÿ��Ʋ �������� ���ư��ٴ� �������ϸ� ����� �־���.
	
	// �ε��� �Ǿ� �ִ� �͸� Close ��.
	UMediaPlayer* LoadedPlayerIfAny = MovieSetup_ApkOnly.GetMoviePlayer(CurrLoadedMovieResourceCache, false);
	if(LoadedPlayerIfAny)
	{
		LoadedPlayerIfAny->Close();
	}

	for (FB2UITitleMovieSetup& CheckSetup : MovieSetup_Normal)
	{
		LoadedPlayerIfAny = CheckSetup.GetMoviePlayer(CurrLoadedMovieResourceCache, false);
		if (LoadedPlayerIfAny)
		{
			LoadedPlayerIfAny->Close();
		}
	}
#endif
	CurrLoadedMovieResourceCache.LoadedMoviePlayer = nullptr;
	CurrLoadedMovieResourceCache.LoadedMovieSource = nullptr;

	StopCurrentMovieSound();
}
