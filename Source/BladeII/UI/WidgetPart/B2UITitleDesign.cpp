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

 UITitleScreen 으로 만들어졌던 기능이 UITitleDLC 로 디자인을 유지한 채 옮겨져서 
 중복된 부분을 파트로 모아놓음.
 디자인 관련 (타이틀 동영상) 부분이며 DLC 나 로그인 기능은 여기선 빠짐
 DLCFront 에서 쓰게 되므로 UIWidgetBase 를 상속받지 못함.

*/
/************************************************************************/

UMediaSource* FB2UITitleMovieSetup::GetMovieSource(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded)
{
	// 한 때 여기 리소스를 TAsset 으로 썼었는데 여기선 별반 의미도 없고 슬쩍 타이밍이 이상해지는 것 같아서 그냥으로 바꿈
	//if (bLoadIfUnloaded)
	//{
	//	if (!MovieSource.IsValid())
	//	{ // 이걸 사용하는 시점에 별도의 async 로딩이 돌아갈 껀덕지는 없는 걸로.
	//		MovieSource.LoadSynchronous();
	//	}
	//	// 보통 LoadSynchronous 해서 딴데서 쓰면 날아갈 거 걱정 안해도 되는데 요건 동영상 플레이어 기능쪽에서 레퍼런스 관리가 잘 안되는지 바로 GC 되는 경우가 있어서 캐싱해 둠.
	//	LoadedCacher.LoadedMovieSource = MovieSource.Get();
	//}

	return MovieSource;
}
UMediaPlayer* FB2UITitleMovieSetup::GetMoviePlayer(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded)
{
	// 한 때 여기 리소스를 TAsset 으로 썼었는데 여기선 별반 의미도 없고 슬쩍 타이밍이 이상해지는 것 같아서 그냥으로 바꿈
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

#if !UE_BUILD_SHIPPING // 혹여나 누군가 모르고 바꾸는 거 방지
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
	/* 이름이 하드코딩으로 들어가는데 assert 라도 걸어둠. */ \
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
	// DLC Download 가 안 된 상황에서 apk 를 통해 배포되는 특정 동영상만을 사용해야 함.
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

	// 로컬 저장 상태와 DLC 다운로드 받은 상태가 꼬였을 경우.. MovieSource 나 MoviePlayer 가 NULL 일 수 있고, 내지는 동영상 원본만 없을 수도 있다.
	// 예외처리를 좀 하려다 보니 Get 함수인데 복잡다단하게 꽤나 들어가는군..  ㅡㅡ
	UMediaSource* FoundMediaSourceOfSetup = FoundSetup ? FoundSetup->GetMovieSource(CurrLoadedMovieResourceCache) : nullptr;
	UMediaPlayer* FoundMediaPlayerOfSetup = FoundSetup ? FoundSetup->GetMoviePlayer(CurrLoadedMovieResourceCache) : nullptr;
	const USoundCue* FoundSoundCueOfSetup = FoundSetup ? FoundSetup->GetMoiveSoundCue(CurrLoadedMovieResourceCache) : nullptr;

	bool bMediaSourceValid = false;
	if (FoundMediaSourceOfSetup && FoundMediaPlayerOfSetup 
		//&& FoundSoundCueOfSetup 이것까진 검사할 필욘 없을 듯하니 스킵. 이건 특히나 담당자 특성 상 리소스 셋업 빵꾸나기가 쉬운데 그럴 경우 문제가 바로 드러나지도 않게 된다.
		)
	{
#if PLATFORM_IOS
		if (FoundMediaPlayerOfSetup->CanPlaySource(FoundMediaSourceOfSetup))
#else 
		// DLC 컨텐츠면 FoundMediaSourceOfSetup->Validate() 가 실패하게 되어 있어서 그걸로 체크 못 함..
		if (FoundMediaPlayerOfSetup->OpenSource(FoundMediaSourceOfSetup))
#endif
		{
			bMediaSourceValid = true;
#if !PLATFORM_IOS
			FoundMediaPlayerOfSetup->Close(); // Valid 체크만 하기 위해 Open 한 것이므로 바로 닫음.
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
		// 이 경우 Apk 에 있는 걸 가져옴.
		FoundSetup = GetMovieSetup(true, InPCClass); // Apk 에 있는 건 단 한 클래스일 거라 InPCClass 지정이 의미가 없지만 혹여나 또 무슨 일이 생길지 모르지.
	}
	return FoundSetup;
}

TWeakObjectPtr<UImage> UB2UITitleDesign::GetMovieBackImageOfSetup(FB2UITitleMovieSetup* InSetup)
{
	//// InSetup 에 해당하는 베이스 Image Widget 을 리턴
	//if (InSetup)
	//{
	//	UMediaSource* MediaSourceOfSetup = InSetup->GetMovieSource(CurrLoadedMovieResourceCache);
	//	if (MediaSourceOfSetup)
	//	{
	//		if (!MediaSourceOfSetup->IsUsingDLCPath()) // IsUsingDLCPath 가 아니면 Apk 용이란 얘기.
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
	// 어떤 경우건 폴백을 이걸로..
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
	// Random Pick 은 최종 버전에서는 맨 처음 실행하는 DLCFrontGM 에서 사용하고
	// 개발버전에서는 로비 타이틀에서 사용.	

	FB2UITitleMovieSetup* RandomPickedSetup = nullptr;
	{
		EPCClass RandomPickedClass = IntToPCClass(FMath::RandRange(0, GetMaxPCClassNum() - 1));
		RandomPickedSetup = GetMovieSetupSafe(IsLimitedResourceAvailable(), RandomPickedClass);
		// RandomPickedClass 와 RandomPickedSetup->RepresentingClass 는 다를 수 있다.
	}

	check(RandomPickedSetup);
//	if (RandomPickedSetup)
//	{
//		StartMovie(RandomPickedSetup->RepresentingClass);
//		// 이후 다른 화면에서도 이어서 사용하기 위해 랜덤 선택한 정보를 저장해 둔다.
//		if (OwnerDLCTitle)
//		{ 
//			OwnerDLCTitle->SetSelectedMediaSourceInfo(RandomPickedSetup->GetMovieSource(CurrLoadedMovieResourceCache), RandomPickedSetup->RepresentingClass);
//		}
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		else if(!IsFinalDLCCookRun())
//		{ // 이 경우는 그냥 개발버전 실행이거나 NonDLC 쿠킹의 경우. 테스트 중이라도 혼선 방지를 위해 최소한의 정보는 넣어줌.
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
		StartMovieBP(); // 정상적인 widget 애니메이션 플레이가 되겠지..
		StartMovieSound(WantedSetup->GetMoiveSoundCue(CurrLoadedMovieResourceCache));
	}
}

void UB2UITitleDesign::StartMovieImmediate(EPCClass InWantedClass)
{
	// 다른 요소들의 UI Widget Anim 은 스킵하고 첫 타이틀 화면에서 보여준 걸 그대로 유지하는 차원에서..
	
	FB2UITitleMovieSetup* WantedSetup = GetMovieSetupSafe(IsLimitedResourceAvailable(), InWantedClass);
	if (WantedSetup)
	{
		ReaptedStartMovieBySetupCommon(*WantedSetup, 1); // 0 초부터는 까만 화면이라 1 초부터 플레이.
		StartMovieImmediateBP(); // Blueprint 에서 아무것도 안하든지, 뭔가 하면 첨부터 끝으로 해 놓든지..
		StartMovieSound(WantedSetup->GetMoiveSoundCue(CurrLoadedMovieResourceCache));
	}

	// 따로 숨길 거 숨김
	if (P_StartTextSet.IsValid())
	{ 
		P_StartTextSet->SetVisibility(ESlateVisibility::Hidden);
	}
}

// 실패 시 일정 횟수 재시도를 하려면 이걸 직접 사용하는 대신 ReaptedStartMovieBySetupCommon 을 사용.
bool UB2UITitleDesign::StartMovieBySetupCommon(FB2UITitleMovieSetup& InMovieSetup, int32 InDesiredStartPosSec)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UITitleDesign::StartMovieBySetupCommon PCClass %d"), PCClassToInt(InMovieSetup.RepresentingClass)));

	bool bRetVal = false;

	UMediaSource* TheMovieSource = InMovieSetup.GetMovieSource(CurrLoadedMovieResourceCache);
	UMediaPlayer* TheMoviePlayer = InMovieSetup.GetMoviePlayer(CurrLoadedMovieResourceCache);
	TWeakObjectPtr<UImage> MovieBackImage = GetMovieBackImageOfSetup(&InMovieSetup);
	
	SetHiddenAllBackImages(); // 딴 건 다 숨겨놓고..
	if (MovieBackImage.IsValid())
	{ // HitTestInvisible 로 하지 말 것. 뭔가 하나는 Visible 이 켜져 있어야 TitleDLC 에서 Touch 신호를 받을 수가 있음.
		MovieBackImage->SetVisibility(ESlateVisibility::Visible);
	}
	if (TheMovieSource && TheMoviePlayer)
	{
		// 플레이 시작.
		if (TheMoviePlayer->OpenSource(TheMovieSource)) // 여기서 일시적으로 실패하는 경우가 있다는 카더라 통신을 들은 바가 있음.
		{
			bRetVal = true;
			TheMoviePlayer->Play();
		}
		
		if (InDesiredStartPosSec > 0)
		{ // 특정 위치부터 보여주기 원할 때.. 
			TheMoviePlayer->Seek(FTimespan(0, 0, InDesiredStartPosSec));
		}

		// 각 Movie 마다 적절한 BI 위치가 있음.
		if (PN_BI.IsValid())
		{
			PN_BI->SetRenderTranslation(InMovieSetup.BITranslation);
		}
	}

	if(!bRetVal)
	{ // 실패하는 경우 뒤에 있는 Fallback 이미지를 대신 보여줌.
	  // 실패 여부와 관계없이 베이스로 항상 보이도록 하고 싶지만 Movie 플레이에서도 나름의 Image widget 을 띄워야 해서 특별한 실패 조건 없이는 그렇게 할 수 없다.
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

	// StartMovieBySetupCommon 실패 시 일정 횟수까지 재시도를 한다.
	if (!bStartMovieSucceeded && ReaptedStartMovieCountdown > 0 && TheWorld)
	{ // 시간은 여기서 적당히 줌. 아주 약간 후에 재시도.
		TheWorld->GetTimerManager().SetTimer(ReaptedStartMovieTH, FTimerDelegate::CreateUObject(this, &UB2UITitleDesign::ReaptedStartMovieBySetupCommon, InMovieSetup, InDesiredStartPosSec), 0.1f, false);
		--ReaptedStartMovieCountdown;
	}
}

void UB2UITitleDesign::ShowTitleMovieFallbackImage(FB2UITitleMovieSetup& InMovieSetup)
{ // 어떤 이유로든 타이틀 동영상을 보이지 못하는 상황에서 동영상을 캡쳐한 이미지를 보이도록 하기 위함.
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UITitleDesign::ShowTitleMovieFallbackImage PCClass %d"), PCClassToInt(InMovieSetup.RepresentingClass)));

	if (IMG_FallbackMovieSnapshot.IsValid())
	{
		SetHiddenAllBackImages();
		// HitTestInvisible 로 하지 말 것. 뭔가 하나는 Visible 이 켜져 있어야 TitleDLC 에서 Touch 신호를 받을 수가 있음.
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
	// 이걸 확실히 안 했을 때 로비에 접속한 이후에도 여전히 백그라운드에 타이틀 동영상이 돌아간다는 프로파일링 결과가 있었음.
	
	// 로딩이 되어 있는 것만 Close 함.
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
