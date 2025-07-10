#include "B2PreLoadingScreen.h"
//#include "BladeII.h"

#include "B2LoadingImageInfo.h"
#include "B2GameInstance.h"

// 설정들.. 그리 중요한 설정까지는 아니니 일단 여기 하드코딩한 값으로
float FPreRenderProgressTextState::AnimatePeriod = 0.5f;

void FPreRenderProgressTextState::Tick(float DeltaSecond)
{
	TimeSinceLastAnimUpdate += DeltaSecond; // UI 쪽서 넘어오는 델타타임이면 World Time Scale 같은 거 안 먹었겠지..

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
	// CurrentAnimateState 만큼 점 찍어줌.
	for(int32 AI = 0; AI < CurrentAnimateState; ++AI)
	{
		AnimatedTextString += TEXT(".");
	}
	
	if (bDisplayProgressText) // 진행도 퍼센트를 텍스트로 끝에 붙여서 표시. 이거 말고 UB2PreLoadingScreen 에 구비된 ProgressBar 로 표시할 수도 있다.
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
		{ // 이 경우 AnimatedTextString 을 여기서 직접 사용하지 않고 CurrentAnimateState 에 따라 별도의 widget 을 on/off.
			return BasicInfoText;
		}
		else
		{ // 초기 구현인데 이게 중앙정렬 레이아웃으로 사용하는 데 문제가 있어서 잘 사용 안하게 됨.
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
		PreRenderProgressTextState.Tick(InDeltaTime); // 진행도 숫자와는 무관한 자체 텍스트 애니메이션
		ApplyCurrentPreRenderProgressText();
	}

	// ProgressBar 채우는 것도 나름의 애니메이션이 있어서 이것도 매 틱.
	PreRenderPBAnimatedValue.ManualTick(FMath::Min(InDeltaTime, 0.06f)); // DeltaTime 을 적당히 잘라서.. 해 보고..
	ApplyCurrentPreRenderProgressBar();
}

void UB2PreLoadingScreen::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_BottomText);
	GET_SLOT(UTextBlock, TB_PreRenderInfo);
	for (int32 TAI = 1; TAI < PRE_RENDER_PROGRESS_TEXT_ANIM_MAX_STATE_NUM; ++TAI) // PRE_RENDER_PROGRESS_TEXT_ANIM_MAX_STATE_NUM - 1 개
	{ // 배열에 숫자 순서대로 넣어야 의도대로 애니메이션이 될 것.
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

	// 타입별로 다르게 세팅해야 할 것이 있다면..

	if (IsOneOfPreRenderScreenType(PreLoadingType))
	{		
		if (Image_BG.IsValid()) 
		{
			Image_BG->SetVisibility(ESlateVisibility::HitTestInvisible);
			
			// 설치 후 첫 Pre-render 레벨인지, 일반 맵들의 pre-render 인지에 따라 다름
			UTexture2D* FinalPreRenderImage = 
				InSpecifiedImage ? InSpecifiedImage : // 사실상 일반 맵들의 pre-render 는 외부에서 가한 이미지를 사용하게 될 것. 기본 로딩 화면 연장 차원에서
				(
					(PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First) ? 
					GetCurrentFirsPreRenderImage() : DefaultPreRenderImagePtr.LoadSynchronous()
				);

			Image_BG->SetBrushFromTexture(FinalPreRenderImage);
			
			if (PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First)
			{
				LastFPRImageRotateTickTime = FPlatformTime::Seconds();
				TimeSinceFPRImageRotate = 0.0; // 로테이션 초기화
			}
		}
		// 좀 길어져서 안내문구도 슬쩍.. 
		if (TB_PreRenderInfo.IsValid())
		{
			TB_PreRenderInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
			// 기본 셋업 변경해서 사용하고 싶으면 따로 콜.
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
	//if (PreLoadingType == EPreLoadingScreenType::PLST_StageBegin) 나머지 Type 에 따른 구분은 아직.
	{
		if (TB_BottomText.IsValid())
		{
			TB_BottomText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PreLoadingText_GameLoading_Type01")));
		}

		if (Image_BG.IsValid())
		{
			// 외부에서 준 게 없으면 이쪽의 기본 이미지 로딩.
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

	// 진행도는 텍스트나 프로그레스바 둘 모두 혹은 선택적으로..
	PreRenderProgressTextState.bDisplayProgressText = bDisplayProgressText;
	bDisplayPreRenderProgressBar = bDisplayProgressBar;
	if (bDisplayPreRenderProgressBar && PB_PreRenderProgress.IsValid()) 
	{		
		// 용수철 설정은 적당한 걸 찾아서..
		PreRenderPBAnimatedValue.AnimType = EProgressAnimType::PRAT_SpringDamp;
		PreRenderPBAnimatedValue.BaseSpeed = 0.4f;
		PreRenderPBAnimatedValue.SpringConst = 100.0f;
		PreRenderPBAnimatedValue.DampConst = 10.0f;
		PreRenderPBAnimatedValue.SetTargetValues(0, 0.0f);

		PB_PreRenderProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// Progress 숫자를 표시하고자 하는 경우 이후에 진행도 정보를 지속적으로 갱신해 주어야 한다.
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
				// Collapse 를 시키면 기본 텍스트의 정렬 상태가 흩어질 수 있으니 hidden 으로.
				? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden
			);
		}
	}
}

void UB2PreLoadingScreen::ApplyCurrentPreRenderProgressBar()
{
	if (bDisplayPreRenderProgressBar && PB_PreRenderProgress.IsValid())
	{ // ProgressAnimateUtil 에서 살짝 애니메이트 된 진행도 값으로 progress bar 에 세팅
		PB_PreRenderProgress->SetPercent(FMath::Clamp(PreRenderPBAnimatedValue.GetCurrentAnimatedPercent(), 0.0f, 1.0f));
	}
}

void UB2PreLoadingScreen::UpdateForFirstPreRenderImage(float InDeltaTime)
{
	check(PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First);
		
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	check(B2GM && B2GM->GetB2GameModeType() == EB2GameMode::PreRender); // 사실상 사용처는 정해져 있지롱
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	UB2LoadingImageInfo* LoadingImgInfo = B2GI ? B2GI->GetLoadingImageInfo() : StaticFindLoadingImageInfo();
	
	// 여기로 넘어오는 DeltaTime 이 World time delation 은 먹지 않더라도 아마 일정 값으로 클램프는 될 껀데..
	// 첫 PreRender 는 중간중간 큰 단위 로딩으로 인해 실제 흐른 시간과 DeltaTime 과의 차이가 클 것이다. 따라서 FPlatformTime 을 사용.

	TimeSinceFPRImageRotate += (FPlatformTime::Seconds() - LastFPRImageRotateTickTime);
	LastFPRImageRotateTickTime = FPlatformTime::Seconds();

	if (LoadingImgInfo && (float)TimeSinceFPRImageRotate >= FirstPreRenderImageRotateInterval)
	{
		const int32 TotalImageNum = LoadingImgInfo->GetLoadingImageNum(EB2GameMode::PreRender);

		// 이미지 로테이트.
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
	check(B2GM && B2GM->GetB2GameModeType() == EB2GameMode::PreRender); // 사실상 사용처는 정해져 있지롱
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	UB2LoadingImageInfo* LoadingImgInfo = B2GI ? B2GI->GetLoadingImageInfo() : StaticFindLoadingImageInfo();

	if (LoadingImgInfo)
	{
		return LoadingImgInfo->GetLoadingImageOfIndex(EB2GameMode::PreRender, FirstPreRenderImageIndex);
	}
	return DefaultPreRenderImagePtr.LoadSynchronous(); // 문제 있음 이거라도.
}