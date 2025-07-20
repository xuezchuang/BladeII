#include "B2PreLoadingScreen.h"
#include "BladeII.h"

#include "B2LoadingImageInfo.h"
#include "B2GameInstance.h"
#include "BladeIILocText.h"
#include "BladeIIUtil.h"

// 汲沥甸.. 弊府 吝夸茄 汲沥鳖瘤绰 酒聪聪 老窜 咯扁 窍靛内爹茄 蔼栏肺
float FPreRenderProgressTextState::AnimatePeriod = 0.5f;

void FPreRenderProgressTextState::Tick(float DeltaSecond)
{
	TimeSinceLastAnimUpdate += DeltaSecond; // UI 率辑 逞绢坷绰 胆鸥鸥烙捞搁 World Time Scale 鞍篮 芭 救 冈菌摆瘤..

	if (TimeSinceLastAnimUpdate >= AnimatePeriod)
	{
		++CurrentAnimateState;
		if (CurrentAnimateState > PRE_RENDER_PROGRESS_TEXT_ANIM_MAX_STATE_NUM)
		{
			CurrentAnimateState = 0;
		}
		TimeSinceLastAnimUpdate = 0.0f;
	}
}

void FPreRenderProgressTextState::ResetAnimState()
{
	CurrentAnimateState = 0;
	TimeSinceLastAnimUpdate = 0.0f;
}

FText FPreRenderProgressTextState::GetCurrentResultText()
{
	FString AnimatedTextString;
	// CurrentAnimateState 父怒 痢 嘛绢淋.
	for(int32 AI = 0; AI < CurrentAnimateState; ++AI)
	{
		AnimatedTextString += TEXT(".");
	}
	
	if (bDisplayProgressText) // 柳青档 欺季飘甫 咆胶飘肺 场俊 嘿咯辑 钎矫. 捞芭 富绊 UB2PreLoadingScreen 俊 备厚等 ProgressBar 肺 钎矫且 荐档 乐促.
	{
		FString ProgressValueTextString = FString::Printf(TEXT("%d %%"), (int32)FMath::Clamp(CurrentProgressScale * 100.0f, 0.0f, 100.0f));
	
		return FText::Format(
			FText::FromString(TEXT("{0}{1} {2}")),
			BasicInfoText,
			FText::FromString(AnimatedTextString),
			FText::FromString(ProgressValueTextString)			
		);
	}
	else
	{
		if (bAnimatedTextInSeparatedWidget)
		{ // 捞 版快 AnimatedTextString 阑 咯扁辑 流立 荤侩窍瘤 臼绊 CurrentAnimateState 俊 蝶扼 喊档狼 widget 阑 on/off.
			return BasicInfoText;
		}
		else
		{ // 檬扁 备泅牢单 捞霸 吝居沥纺 饭捞酒眶栏肺 荤侩窍绰 单 巩力啊 乐绢辑 肋 荤侩 救窍霸 凳.
			return FText::Format(
					FText::FromString(TEXT("{0}{1}")),
					BasicInfoText,
					FText::FromString(AnimatedTextString)
				);
		}
	}
}

UB2PreLoadingScreen::UB2PreLoadingScreen(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PreLoadingType = EPreLoadingScreenType::PLST_StageBegin;

	FirstPreRenderImageIndex = 0;
	FirstPreRenderImageRotateInterval = 5.0f;
	LastFPRImageRotateTickTime = 0.0;
	TimeSinceFPRImageRotate = 0.0;

	bShouldDisplayPreRenderText = false;
	bDisplayPreRenderProgressBar = false;
}

void UB2PreLoadingScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First)
	{
		UpdateForFirstPreRenderImage(InDeltaTime);
	}

	if (bShouldDisplayPreRenderText)
	{
		PreRenderProgressTextState.Tick(InDeltaTime); // 柳青档 箭磊客绰 公包茄 磊眉 咆胶飘 局聪皋捞记
		ApplyCurrentPreRenderProgressText();
	}

	// ProgressBar 盲快绰 巴档 唱抚狼 局聪皋捞记捞 乐绢辑 捞巴档 概 平.
	PreRenderPBAnimatedValue.ManualTick(FMath::Min(InDeltaTime, 0.06f)); // DeltaTime 阑 利寸洒 肋扼辑.. 秦 焊绊..
	ApplyCurrentPreRenderProgressBar();
}

void UB2PreLoadingScreen::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_BottomText);
	GET_SLOT(UTextBlock, TB_PreRenderInfo);
	for (int32 TAI = 1; TAI < PRE_RENDER_PROGRESS_TEXT_ANIM_MAX_STATE_NUM; ++TAI) // PRE_RENDER_PROGRESS_TEXT_ANIM_MAX_STATE_NUM - 1 俺
	{ // 硅凯俊 箭磊 鉴辑措肺 持绢具 狼档措肺 局聪皋捞记捞 瞪 巴.
		UTextBlock* FoundAnimPiece = GetSlot<UTextBlock>(FName(*FString::Printf(TEXT("TB_PreRenderInfo_AnimPiece_%d"), TAI)));
		if (FoundAnimPiece)
		{
			FoundAnimPiece->SetVisibility(ESlateVisibility::Hidden);
			TB_PreRenderInfo_AnimPieces.Add(FoundAnimPiece);
		}
	}

	GET_SLOT(UProgressBar, PB_PreRenderProgress);
	GET_SLOT(UTextBlock, TB_MiddleText1);
	GET_SLOT(UTextBlock, TB_MiddleText2);
	GET_SLOT(UImage, Image_BG);
	GET_SLOT(UPanelWidget, P_LoadingWheel);
}

void UB2PreLoadingScreen::DestroySelf()
{
	Super::DestroySelf();

}

void UB2PreLoadingScreen::SetPreLoadingType(EPreLoadingScreenType InType, UTexture2D* InSpecifiedImage)
{
	PreLoadingType = InType;

	// 鸥涝喊肺 促福霸 技泼秦具 且 巴捞 乐促搁..

	if (IsOneOfPreRenderScreenType(PreLoadingType))
	{		
		if (Image_BG.IsValid()) 
		{
			Image_BG->SetVisibility(ESlateVisibility::HitTestInvisible);
			
			// 汲摹 饶 霉 Pre-render 饭骇牢瘤, 老馆 甘甸狼 pre-render 牢瘤俊 蝶扼 促抚
			UTexture2D* FinalPreRenderImage = 
				InSpecifiedImage ? InSpecifiedImage : // 荤角惑 老馆 甘甸狼 pre-render 绰 寇何俊辑 啊茄 捞固瘤甫 荤侩窍霸 瞪 巴. 扁夯 肺爹 拳搁 楷厘 瞒盔俊辑
				(
					(PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First) ? 
					GetCurrentFirsPreRenderImage() : DefaultPreRenderImagePtr.LoadSynchronous()
				);

			Image_BG->SetBrushFromTexture(FinalPreRenderImage);
			
			if (PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First)
			{
				LastFPRImageRotateTickTime = FPlatformTime::Seconds();
				TimeSinceFPRImageRotate = 0.0; // 肺抛捞记 檬扁拳
			}
		}
		// 粱 辨绢廉辑 救郴巩备档 浇陆.. 
		if (TB_PreRenderInfo.IsValid())
		{
			TB_PreRenderInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
			// 扁夯 悸诀 函版秦辑 荤侩窍绊 酵栏搁 蝶肺 妮.
			SetupPreRenderProgress((PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First), 
				false, (PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First));
		}

		if (TB_MiddleText1.IsValid()) {
			TB_MiddleText1->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TB_MiddleText2.IsValid()) {
			TB_MiddleText2->SetVisibility(ESlateVisibility::Hidden);
		}
		if (P_LoadingWheel.IsValid()) {
			P_LoadingWheel->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	//if (PreLoadingType == EPreLoadingScreenType::PLST_StageBegin) 唱赣瘤 Type 俊 蝶弗 备盒篮 酒流.
	{
		if (TB_BottomText.IsValid())
		{
			TB_BottomText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PreLoadingText_GameLoading_Type01")));
		}

		if (Image_BG.IsValid())
		{
			// 寇何俊辑 霖 霸 绝栏搁 捞率狼 扁夯 捞固瘤 肺爹.
			UTexture2D* FinalImage = InSpecifiedImage ? InSpecifiedImage : DefaultLoadingImagePtr.LoadSynchronous();
			Image_BG->SetBrushFromTexture(FinalImage);
		}
	}
}

void UB2PreLoadingScreen::SetLoadingText_Middle1(const FText& InText)
{
	if (TB_MiddleText1.IsValid())
		TB_MiddleText1->SetText(InText);
}

void UB2PreLoadingScreen::SetLoadingText_Middle2(const FText& InText)
{
	if (TB_MiddleText2.IsValid())
		TB_MiddleText2->SetText(InText);
}

void UB2PreLoadingScreen::SetLoadingText_Bottom(const FText& InText)
{
	if (TB_BottomText.IsValid())
		TB_BottomText->SetText(InText);
}

void UB2PreLoadingScreen::SetupPreRenderProgress(bool bIsForFistPreRenderGM, bool bDisplayProgressText, bool bDisplayProgressBar)
{
	bShouldDisplayPreRenderText = true;

	if (bIsForFistPreRenderGM){
		PreRenderProgressTextState.BasicInfoText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PreLoadingText_PreRender_FirstTime"));
	}
	else {
		PreRenderProgressTextState.BasicInfoText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PreLoadingText_PreRender"));
	}

	// 柳青档绰 咆胶飘唱 橇肺弊饭胶官 笛 葛滴 趣篮 急琶利栏肺..
	PreRenderProgressTextState.bDisplayProgressText = bDisplayProgressText;
	bDisplayPreRenderProgressBar = bDisplayProgressBar;
	if (bDisplayPreRenderProgressBar && PB_PreRenderProgress.IsValid()) 
	{		
		// 侩荐枚 汲沥篮 利寸茄 吧 茫酒辑..
		PreRenderPBAnimatedValue.AnimType = EProgressAnimType::PRAT_SpringDamp;
		PreRenderPBAnimatedValue.BaseSpeed = 0.4f;
		PreRenderPBAnimatedValue.SpringConst = 100.0f;
		PreRenderPBAnimatedValue.DampConst = 10.0f;
		PreRenderPBAnimatedValue.SetTargetValues(0, 0.0f);

		PB_PreRenderProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// Progress 箭磊甫 钎矫窍绊磊 窍绰 版快 捞饶俊 柳青档 沥焊甫 瘤加利栏肺 盎脚秦 林绢具 茄促.
	PreRenderProgressTextState.CurrentProgressScale = 0.0f;
	PreRenderProgressTextState.ResetAnimState();

	ApplyCurrentPreRenderProgressText();
	ApplyCurrentPreRenderProgressBar();
}

void UB2PreLoadingScreen::UpdatePreRenderProgressScale(float InNewProgressScale, bool bSyncProgressImmediately)
{
	PreRenderProgressTextState.CurrentProgressScale = InNewProgressScale;
	PreRenderPBAnimatedValue.SetTargetValues(0, InNewProgressScale, bSyncProgressImmediately);
	ApplyCurrentPreRenderProgressText();
	ApplyCurrentPreRenderProgressBar();
}

void UB2PreLoadingScreen::ApplyCurrentPreRenderProgressText()
{
	if (TB_PreRenderInfo.IsValid())
	{
		TB_PreRenderInfo->SetText(PreRenderProgressTextState.GetCurrentResultText());
	}

	for (int32 TAI = 0; TAI < TB_PreRenderInfo_AnimPieces.Num(); ++TAI)
	{
		UTextBlock* ThisAnimPiece = TB_PreRenderInfo_AnimPieces[TAI].Get();
		if (ThisAnimPiece)
		{
			ThisAnimPiece->SetVisibility(
				(PreRenderProgressTextState.bAnimatedTextInSeparatedWidget && TAI < PreRenderProgressTextState.GetCurrentAnimateState()) 
				// Collapse 甫 矫虐搁 扁夯 咆胶飘狼 沥纺 惑怕啊 瑞绢龙 荐 乐栏聪 hidden 栏肺.
				? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden
			);
		}
	}
}

void UB2PreLoadingScreen::ApplyCurrentPreRenderProgressBar()
{
	if (bDisplayPreRenderProgressBar && PB_PreRenderProgress.IsValid())
	{ // ProgressAnimateUtil 俊辑 混娄 局聪皋捞飘 等 柳青档 蔼栏肺 progress bar 俊 技泼
		PB_PreRenderProgress->SetPercent(FMath::Clamp(PreRenderPBAnimatedValue.GetCurrentAnimatedPercent(), 0.0f, 1.0f));
	}
}

void UB2PreLoadingScreen::UpdateForFirstPreRenderImage(float InDeltaTime)
{
	check(PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First);
		
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	check(B2GM && B2GM->GetB2GameModeType() == EB2GameMode::PreRender); // 荤角惑 荤侩贸绰 沥秦廉 乐瘤氛
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	UB2LoadingImageInfo* LoadingImgInfo = B2GI ? B2GI->GetLoadingImageInfo() : StaticFindLoadingImageInfo();
	
	// 咯扁肺 逞绢坷绰 DeltaTime 捞 World time delation 篮 冈瘤 臼歹扼档 酒付 老沥 蔼栏肺 努伐橇绰 瞪 搏单..
	// 霉 PreRender 绰 吝埃吝埃 奴 窜困 肺爹栏肺 牢秦 角力 儒弗 矫埃苞 DeltaTime 苞狼 瞒捞啊 努 巴捞促. 蝶扼辑 FPlatformTime 阑 荤侩.

	TimeSinceFPRImageRotate += (FPlatformTime::Seconds() - LastFPRImageRotateTickTime);
	LastFPRImageRotateTickTime = FPlatformTime::Seconds();

	if (LoadingImgInfo && (float)TimeSinceFPRImageRotate >= FirstPreRenderImageRotateInterval)
	{
		const int32 TotalImageNum = LoadingImgInfo->GetLoadingImageNum(EB2GameMode::PreRender);

		// 捞固瘤 肺抛捞飘.
		++FirstPreRenderImageIndex;
		if (FirstPreRenderImageIndex >= TotalImageNum)
		{
			FirstPreRenderImageIndex = 0;
		}
		
		UTexture2D* ImageThisTime = GetCurrentFirsPreRenderImage();
		if (ImageThisTime && Image_BG.IsValid())
		{
			Image_BG->SetBrushFromTexture(ImageThisTime);
		}

		TimeSinceFPRImageRotate = 0.0;
	}
}

UTexture2D* UB2PreLoadingScreen::GetCurrentFirsPreRenderImage()
{
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	check(B2GM && B2GM->GetB2GameModeType() == EB2GameMode::PreRender); // 荤角惑 荤侩贸绰 沥秦廉 乐瘤氛
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	UB2LoadingImageInfo* LoadingImgInfo = B2GI ? B2GI->GetLoadingImageInfo() : StaticFindLoadingImageInfo();

	if (LoadingImgInfo)
	{
		return LoadingImgInfo->GetLoadingImageOfIndex(EB2GameMode::PreRender, FirstPreRenderImageIndex);
	}
	return DefaultPreRenderImagePtr.LoadSynchronous(); // 巩力 乐澜 捞芭扼档.
}