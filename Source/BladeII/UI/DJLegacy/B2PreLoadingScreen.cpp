#include "B2PreLoadingScreen.h"
//#include "BladeII.h"

#include "B2LoadingImageInfo.h"
#include "B2GameInstance.h"

// ������.. �׸� �߿��� ���������� �ƴϴ� �ϴ� ���� �ϵ��ڵ��� ������
float FPreRenderProgressTextState::AnimatePeriod = 0.5f;

void FPreRenderProgressTextState::Tick(float DeltaSecond)
{
	TimeSinceLastAnimUpdate += DeltaSecond; // UI �ʼ� �Ѿ���� ��ŸŸ���̸� World Time Scale ���� �� �� �Ծ�����..

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
	// CurrentAnimateState ��ŭ �� �����.
	for(int32 AI = 0; AI < CurrentAnimateState; ++AI)
	{
		AnimatedTextString += TEXT(".");
	}
	
	if (bDisplayProgressText) // ���൵ �ۼ�Ʈ�� �ؽ�Ʈ�� ���� �ٿ��� ǥ��. �̰� ���� UB2PreLoadingScreen �� ����� ProgressBar �� ǥ���� ���� �ִ�.
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
		{ // �� ��� AnimatedTextString �� ���⼭ ���� ������� �ʰ� CurrentAnimateState �� ���� ������ widget �� on/off.
			return BasicInfoText;
		}
		else
		{ // �ʱ� �����ε� �̰� �߾����� ���̾ƿ����� ����ϴ� �� ������ �־ �� ��� ���ϰ� ��.
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
		PreRenderProgressTextState.Tick(InDeltaTime); // ���൵ ���ڿʹ� ������ ��ü �ؽ�Ʈ �ִϸ��̼�
		ApplyCurrentPreRenderProgressText();
	}

	// ProgressBar ä��� �͵� ������ �ִϸ��̼��� �־ �̰͵� �� ƽ.
	PreRenderPBAnimatedValue.ManualTick(FMath::Min(InDeltaTime, 0.06f)); // DeltaTime �� ������ �߶�.. �� ����..
	ApplyCurrentPreRenderProgressBar();
}

void UB2PreLoadingScreen::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_BottomText);
	GET_SLOT(UTextBlock, TB_PreRenderInfo);
	for (int32 TAI = 1; TAI < PRE_RENDER_PROGRESS_TEXT_ANIM_MAX_STATE_NUM; ++TAI) // PRE_RENDER_PROGRESS_TEXT_ANIM_MAX_STATE_NUM - 1 ��
	{ // �迭�� ���� ������� �־�� �ǵ���� �ִϸ��̼��� �� ��.
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

	// Ÿ�Ժ��� �ٸ��� �����ؾ� �� ���� �ִٸ�..

	if (IsOneOfPreRenderScreenType(PreLoadingType))
	{		
		if (Image_BG.IsValid()) 
		{
			Image_BG->SetVisibility(ESlateVisibility::HitTestInvisible);
			
			// ��ġ �� ù Pre-render ��������, �Ϲ� �ʵ��� pre-render ������ ���� �ٸ�
			UTexture2D* FinalPreRenderImage = 
				InSpecifiedImage ? InSpecifiedImage : // ��ǻ� �Ϲ� �ʵ��� pre-render �� �ܺο��� ���� �̹����� ����ϰ� �� ��. �⺻ �ε� ȭ�� ���� ��������
				(
					(PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First) ? 
					GetCurrentFirsPreRenderImage() : DefaultPreRenderImagePtr.LoadSynchronous()
				);

			Image_BG->SetBrushFromTexture(FinalPreRenderImage);
			
			if (PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First)
			{
				LastFPRImageRotateTickTime = FPlatformTime::Seconds();
				TimeSinceFPRImageRotate = 0.0; // �����̼� �ʱ�ȭ
			}
		}
		// �� ������� �ȳ������� ��½.. 
		if (TB_PreRenderInfo.IsValid())
		{
			TB_PreRenderInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
			// �⺻ �¾� �����ؼ� ����ϰ� ������ ���� ��.
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
	//if (PreLoadingType == EPreLoadingScreenType::PLST_StageBegin) ������ Type �� ���� ������ ����.
	{
		if (TB_BottomText.IsValid())
		{
			TB_BottomText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PreLoadingText_GameLoading_Type01")));
		}

		if (Image_BG.IsValid())
		{
			// �ܺο��� �� �� ������ ������ �⺻ �̹��� �ε�.
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

	// ���൵�� �ؽ�Ʈ�� ���α׷����� �� ��� Ȥ�� ����������..
	PreRenderProgressTextState.bDisplayProgressText = bDisplayProgressText;
	bDisplayPreRenderProgressBar = bDisplayProgressBar;
	if (bDisplayPreRenderProgressBar && PB_PreRenderProgress.IsValid()) 
	{		
		// ���ö ������ ������ �� ã�Ƽ�..
		PreRenderPBAnimatedValue.AnimType = EProgressAnimType::PRAT_SpringDamp;
		PreRenderPBAnimatedValue.BaseSpeed = 0.4f;
		PreRenderPBAnimatedValue.SpringConst = 100.0f;
		PreRenderPBAnimatedValue.DampConst = 10.0f;
		PreRenderPBAnimatedValue.SetTargetValues(0, 0.0f);

		PB_PreRenderProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// Progress ���ڸ� ǥ���ϰ��� �ϴ� ��� ���Ŀ� ���൵ ������ ���������� ������ �־�� �Ѵ�.
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
				// Collapse �� ��Ű�� �⺻ �ؽ�Ʈ�� ���� ���°� ����� �� ������ hidden ����.
				? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden
			);
		}
	}
}

void UB2PreLoadingScreen::ApplyCurrentPreRenderProgressBar()
{
	if (bDisplayPreRenderProgressBar && PB_PreRenderProgress.IsValid())
	{ // ProgressAnimateUtil ���� ��¦ �ִϸ���Ʈ �� ���൵ ������ progress bar �� ����
		PB_PreRenderProgress->SetPercent(FMath::Clamp(PreRenderPBAnimatedValue.GetCurrentAnimatedPercent(), 0.0f, 1.0f));
	}
}

void UB2PreLoadingScreen::UpdateForFirstPreRenderImage(float InDeltaTime)
{
	check(PreLoadingType == EPreLoadingScreenType::PLST_PreRender_First);
		
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	check(B2GM && B2GM->GetB2GameModeType() == EB2GameMode::PreRender); // ��ǻ� ���ó�� ������ ������
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	UB2LoadingImageInfo* LoadingImgInfo = B2GI ? B2GI->GetLoadingImageInfo() : StaticFindLoadingImageInfo();
	
	// ����� �Ѿ���� DeltaTime �� World time delation �� ���� �ʴ��� �Ƹ� ���� ������ Ŭ������ �� ����..
	// ù PreRender �� �߰��߰� ū ���� �ε����� ���� ���� �帥 �ð��� DeltaTime ���� ���̰� Ŭ ���̴�. ���� FPlatformTime �� ���.

	TimeSinceFPRImageRotate += (FPlatformTime::Seconds() - LastFPRImageRotateTickTime);
	LastFPRImageRotateTickTime = FPlatformTime::Seconds();

	if (LoadingImgInfo && (float)TimeSinceFPRImageRotate >= FirstPreRenderImageRotateInterval)
	{
		const int32 TotalImageNum = LoadingImgInfo->GetLoadingImageNum(EB2GameMode::PreRender);

		// �̹��� ������Ʈ.
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
	check(B2GM && B2GM->GetB2GameModeType() == EB2GameMode::PreRender); // ��ǻ� ���ó�� ������ ������
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	UB2LoadingImageInfo* LoadingImgInfo = B2GI ? B2GI->GetLoadingImageInfo() : StaticFindLoadingImageInfo();

	if (LoadingImgInfo)
	{
		return LoadingImgInfo->GetLoadingImageOfIndex(EB2GameMode::PreRender, FirstPreRenderImageIndex);
	}
	return DefaultPreRenderImagePtr.LoadSynchronous(); // ���� ���� �̰Ŷ�.
}