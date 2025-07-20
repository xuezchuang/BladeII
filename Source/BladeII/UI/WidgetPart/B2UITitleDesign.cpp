
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

 UITitleScreen 栏肺 父甸绢脸带 扁瓷捞 UITitleDLC 肺 叼磊牢阑 蜡瘤茄 盲 颗败廉辑 
 吝汗等 何盒阑 颇飘肺 葛酒初澜.
 叼磊牢 包访 (鸥捞撇 悼康惑) 何盒捞哥 DLC 唱 肺弊牢 扁瓷篮 咯扁急 狐咙
 DLCFront 俊辑 静霸 登骨肺 UIWidgetBase 甫 惑加罐瘤 给窃.

*/
/************************************************************************/

UMediaSource* FB2UITitleMovieSetup::GetMovieSource(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded)
{
	// 茄 锭 咯扁 府家胶甫 TAsset 栏肺 借菌绰单 咯扁急 喊馆 狼固档 绝绊 浇陆 鸥捞怪捞 捞惑秦瘤绰 巴 鞍酒辑 弊成栏肺 官厕
	//if (bLoadIfUnloaded)
	//{
	//	if (!MovieSource.IsValid())
	//	{ // 捞吧 荤侩窍绰 矫痢俊 喊档狼 async 肺爹捞 倒酒哎 搏傣瘤绰 绝绰 吧肺.
	//		MovieSource.LoadSynchronous();
	//	}
	//	// 焊烹 LoadSynchronous 秦辑 谍单辑 静搁 朝酒哎 芭 捌沥 救秦档 登绰单 夸扒 悼康惑 敲饭捞绢 扁瓷率俊辑 饭欺繁胶 包府啊 肋 救登绰瘤 官肺 GC 登绰 版快啊 乐绢辑 某教秦 狄.
	//	LoadedCacher.LoadedMovieSource = MovieSource.Get();
	//}

	return MovieSource;
}
UMediaPlayer* FB2UITitleMovieSetup::GetMoviePlayer(FB2UITitleMovieSetupLoadedCacher& LoadedCacher, bool bLoadIfUnloaded)
{
	// 茄 锭 咯扁 府家胶甫 TAsset 栏肺 借菌绰单 咯扁急 喊馆 狼固档 绝绊 浇陆 鸥捞怪捞 捞惑秦瘤绰 巴 鞍酒辑 弊成栏肺 官厕
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

#if !UE_BUILD_SHIPPING // 趣咯唱 穿焙啊 葛福绊 官操绰 芭 规瘤
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
	/* 捞抚捞 窍靛内爹栏肺 甸绢啊绰单 assert 扼档 吧绢狄. */ \
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
	// DLC Download 啊 救 等 惑炔俊辑 apk 甫 烹秦 硅器登绰 漂沥 悼康惑父阑 荤侩秦具 窃.
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

	// 肺拿 历厘 惑怕客 DLC 促款肺靛 罐篮 惑怕啊 部看阑 版快.. MovieSource 唱 MoviePlayer 啊 NULL 老 荐 乐绊, 郴瘤绰 悼康惑 盔夯父 绝阑 荐档 乐促.
	// 抗寇贸府甫 粱 窍妨促 焊聪 Get 窃荐牢单 汗棱促窜窍霸 蚕唱 甸绢啊绰焙..  ぱぱ
	UMediaSource* FoundMediaSourceOfSetup = FoundSetup ? FoundSetup->GetMovieSource(CurrLoadedMovieResourceCache) : nullptr;
	UMediaPlayer* FoundMediaPlayerOfSetup = FoundSetup ? FoundSetup->GetMoviePlayer(CurrLoadedMovieResourceCache) : nullptr;
	const USoundCue* FoundSoundCueOfSetup = FoundSetup ? FoundSetup->GetMoiveSoundCue(CurrLoadedMovieResourceCache) : nullptr;

	bool bMediaSourceValid = false;
	if (FoundMediaSourceOfSetup && FoundMediaPlayerOfSetup 
		//&& FoundSoundCueOfSetup 捞巴鳖柳 八荤且 鞘挎 绝阑 淀窍聪 胶诺. 捞扒 漂洒唱 淬寸磊 漂己 惑 府家胶 悸诀 户操唱扁啊 浆款单 弊凡 版快 巩力啊 官肺 靛矾唱瘤档 臼霸 等促.
		)
	{
#if PLATFORM_IOS
		if (FoundMediaPlayerOfSetup->CanPlaySource(FoundMediaSourceOfSetup))
#else 
		// DLC 牧刨明搁 FoundMediaSourceOfSetup->Validate() 啊 角菩窍霸 登绢 乐绢辑 弊吧肺 眉农 给 窃..
		if (FoundMediaPlayerOfSetup->OpenSource(FoundMediaSourceOfSetup))
#endif
		{
			bMediaSourceValid = true;
#if !PLATFORM_IOS
			FoundMediaPlayerOfSetup->Close(); // Valid 眉农父 窍扁 困秦 Open 茄 巴捞骨肺 官肺 摧澜.
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
		// 捞 版快 Apk 俊 乐绰 吧 啊廉咳.
		FoundSetup = GetMovieSetup(true, InPCClass); // Apk 俊 乐绰 扒 窜 茄 努贰胶老 芭扼 InPCClass 瘤沥捞 狼固啊 绝瘤父 趣咯唱 肚 公郊 老捞 积辨瘤 葛福瘤.
	}
	return FoundSetup;
}

TWeakObjectPtr<UImage> UB2UITitleDesign::GetMovieBackImageOfSetup(FB2UITitleMovieSetup* InSetup)
{
	//// InSetup 俊 秦寸窍绰 海捞胶 Image Widget 阑 府畔
	//if (InSetup)
	//{
	//	UMediaSource* MediaSourceOfSetup = InSetup->GetMovieSource(CurrLoadedMovieResourceCache);
	//	if (MediaSourceOfSetup)
	//	{
	//		if (!MediaSourceOfSetup->IsUsingDLCPath()) // IsUsingDLCPath 啊 酒聪搁 Apk 侩捞鄂 娟扁.
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
	// 绢恫 版快扒 弃归阑 捞吧肺..
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
	// Random Pick 篮 弥辆 滚傈俊辑绰 盖 贸澜 角青窍绰 DLCFrontGM 俊辑 荤侩窍绊
	// 俺惯滚傈俊辑绰 肺厚 鸥捞撇俊辑 荤侩.	

	FB2UITitleMovieSetup* RandomPickedSetup = nullptr;
	{
		EPCClass RandomPickedClass = IntToPCClass(FMath::RandRange(0, GetMaxPCClassNum() - 1));
		RandomPickedSetup = GetMovieSetupSafe(IsLimitedResourceAvailable(), RandomPickedClass);
		// RandomPickedClass 客 RandomPickedSetup->RepresentingClass 绰 促甫 荐 乐促.
	}

	check(RandomPickedSetup);
//	if (RandomPickedSetup)
//	{
//		StartMovie(RandomPickedSetup->RepresentingClass);
//		// 捞饶 促弗 拳搁俊辑档 捞绢辑 荤侩窍扁 困秦 罚待 急琶茄 沥焊甫 历厘秦 敌促.
//		if (OwnerDLCTitle)
//		{ 
//			OwnerDLCTitle->SetSelectedMediaSourceInfo(RandomPickedSetup->GetMovieSource(CurrLoadedMovieResourceCache), RandomPickedSetup->RepresentingClass);
//		}
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		else if(!IsFinalDLCCookRun())
//		{ // 捞 版快绰 弊成 俺惯滚傈 角青捞芭唱 NonDLC 捻欧狼 版快. 抛胶飘 吝捞扼档 去急 规瘤甫 困秦 弥家茄狼 沥焊绰 持绢淋.
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
		StartMovieBP(); // 沥惑利牢 widget 局聪皋捞记 敲饭捞啊 登摆瘤..
		StartMovieSound(WantedSetup->GetMoiveSoundCue(CurrLoadedMovieResourceCache));
	}
}

void UB2UITitleDesign::StartMovieImmediate(EPCClass InWantedClass)
{
	// 促弗 夸家甸狼 UI Widget Anim 篮 胶诺窍绊 霉 鸥捞撇 拳搁俊辑 焊咯霖 吧 弊措肺 蜡瘤窍绰 瞒盔俊辑..
	
	FB2UITitleMovieSetup* WantedSetup = GetMovieSetupSafe(IsLimitedResourceAvailable(), InWantedClass);
	if (WantedSetup)
	{
		ReaptedStartMovieBySetupCommon(*WantedSetup, 1); // 0 檬何磐绰 鳖父 拳搁捞扼 1 檬何磐 敲饭捞.
		StartMovieImmediateBP(); // Blueprint 俊辑 酒公巴档 救窍电瘤, 购啊 窍搁 梅何磐 场栏肺 秦 初电瘤..
		StartMovieSound(WantedSetup->GetMoiveSoundCue(CurrLoadedMovieResourceCache));
	}

	// 蝶肺 见辨 芭 见辫
	if (P_StartTextSet.IsValid())
	{ 
		P_StartTextSet->SetVisibility(ESlateVisibility::Hidden);
	}
}

// 角菩 矫 老沥 冉荐 犁矫档甫 窍妨搁 捞吧 流立 荤侩窍绰 措脚 ReaptedStartMovieBySetupCommon 阑 荤侩.
bool UB2UITitleDesign::StartMovieBySetupCommon(FB2UITitleMovieSetup& InMovieSetup, int32 InDesiredStartPosSec)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UITitleDesign::StartMovieBySetupCommon PCClass %d"), PCClassToInt(InMovieSetup.RepresentingClass)));

	bool bRetVal = false;

	UMediaSource* TheMovieSource = InMovieSetup.GetMovieSource(CurrLoadedMovieResourceCache);
	UMediaPlayer* TheMoviePlayer = InMovieSetup.GetMoviePlayer(CurrLoadedMovieResourceCache);
	TWeakObjectPtr<UImage> MovieBackImage = GetMovieBackImageOfSetup(&InMovieSetup);
	
	SetHiddenAllBackImages(); // 谍 扒 促 见败初绊..
	if (MovieBackImage.IsValid())
	{ // HitTestInvisible 肺 窍瘤 富 巴. 购啊 窍唱绰 Visible 捞 难廉 乐绢具 TitleDLC 俊辑 Touch 脚龋甫 罐阑 荐啊 乐澜.
		MovieBackImage->SetVisibility(ESlateVisibility::Visible);
	}
	if (TheMovieSource && TheMoviePlayer)
	{
		// 敲饭捞 矫累.
		if (TheMoviePlayer->OpenSource(TheMovieSource)) // 咯扁辑 老矫利栏肺 角菩窍绰 版快啊 乐促绰 墨歹扼 烹脚阑 甸篮 官啊 乐澜.
		{
			bRetVal = true;
			TheMoviePlayer->Play();
		}
		
		if (InDesiredStartPosSec > 0)
		{ // 漂沥 困摹何磐 焊咯林扁 盔且 锭.. 
			TheMoviePlayer->Seek(FTimespan(0, 0, InDesiredStartPosSec));
		}

		// 阿 Movie 付促 利例茄 BI 困摹啊 乐澜.
		if (PN_BI.IsValid())
		{
			PN_BI->SetRenderTranslation(InMovieSetup.BITranslation);
		}
	}

	if(!bRetVal)
	{ // 角菩窍绰 版快 第俊 乐绰 Fallback 捞固瘤甫 措脚 焊咯淋.
	  // 角菩 咯何客 包拌绝捞 海捞胶肺 亲惑 焊捞档废 窍绊 酵瘤父 Movie 敲饭捞俊辑档 唱抚狼 Image widget 阑 剁况具 秦辑 漂喊茄 角菩 炼扒 绝捞绰 弊犯霸 且 荐 绝促.
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

	// StartMovieBySetupCommon 角菩 矫 老沥 冉荐鳖瘤 犁矫档甫 茄促.
	if (!bStartMovieSucceeded && ReaptedStartMovieCountdown > 0 && TheWorld)
	{ // 矫埃篮 咯扁辑 利寸洒 淋. 酒林 距埃 饶俊 犁矫档.
		TheWorld->GetTimerManager().SetTimer(ReaptedStartMovieTH, FTimerDelegate::CreateUObject(this, &UB2UITitleDesign::ReaptedStartMovieBySetupCommon, InMovieSetup, InDesiredStartPosSec), 0.1f, false);
		--ReaptedStartMovieCountdown;
	}
}

void UB2UITitleDesign::ShowTitleMovieFallbackImage(FB2UITitleMovieSetup& InMovieSetup)
{ // 绢恫 捞蜡肺电 鸥捞撇 悼康惑阑 焊捞瘤 给窍绰 惑炔俊辑 悼康惑阑 母媚茄 捞固瘤甫 焊捞档废 窍扁 困窃.
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UITitleDesign::ShowTitleMovieFallbackImage PCClass %d"), PCClassToInt(InMovieSetup.RepresentingClass)));

	if (IMG_FallbackMovieSnapshot.IsValid())
	{
		SetHiddenAllBackImages();
		// HitTestInvisible 肺 窍瘤 富 巴. 购啊 窍唱绰 Visible 捞 难廉 乐绢具 TitleDLC 俊辑 Touch 脚龋甫 罐阑 荐啊 乐澜.
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
	// 捞吧 犬角洒 救 沁阑 锭 肺厚俊 立加茄 捞饶俊档 咯傈洒 归弊扼款靛俊 鸥捞撇 悼康惑捞 倒酒埃促绰 橇肺颇老傅 搬苞啊 乐菌澜.
	
	// 肺爹捞 登绢 乐绰 巴父 Close 窃.
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
