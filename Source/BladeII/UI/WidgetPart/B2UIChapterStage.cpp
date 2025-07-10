#include "B2UIChapterStage.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
#include "B2UIRepeatableElements.h"
#include "B2StageInfo.h"


#include "Global.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

UB2UIChapterStage::UB2UIChapterStage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BossFlipbookMIDTimeSync = 0.0f;
	CurrCharIntroFlipbookMID = nullptr;

	nCurChapterNum = 0;
	nClearStageNum = 0;
	bActClear = false;
}

void UB2UIChapterStage::Init()
{
	Super::Init();

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
}

void UB2UIChapterStage::SetChangeImage(EStageDifficulty InStageDifficulty)
{
	if (ChapterImages.Contains(InStageDifficulty) == false)
		return;

	FChapterImageInfo ChapterImageInfo = ChapterImages[InStageDifficulty];

	if (IMG_Lock.IsValid())
		IMG_Lock->SetBrush(ChapterImageInfo.LockImage);
	if (IMG_NextStateBG.IsValid())
		IMG_NextStateBG->SetBrush(ChapterImageInfo.NextStateBG);
	if (IMG_NextStateFx1.IsValid())
		IMG_NextStateFx1->SetBrush(ChapterImageInfo.NextImageFx1);
	if (IMG_NextStateFx2.IsValid())
		IMG_NextStateFx2->SetBrush(ChapterImageInfo.NextImageFx2);
	if (IMG_NextStateFx3.IsValid())
		IMG_NextStateFx3->SetBrush(ChapterImageInfo.NextImageFx3);
	if (IMG_NextStateSW.IsValid())
		IMG_NextStateSW->SetBrush(ChapterImageInfo.NextImageSW);
	if (IMG_Clear.IsValid())
		IMG_Clear->SetBrush(ChapterImageInfo.ClearImage);
	if (IMG_ClearFX.IsValid())
		IMG_ClearFX->SetBrush(ChapterImageInfo.ClearImageFX);
}

void UB2UIChapterStage::CacheAssets()
{
	GET_SLOT(UImage, IMG_Lock)
	GET_SLOT(UImage, IMG_NextStateBG)
	GET_SLOT(UImage, IMG_NextStateFx1)
	GET_SLOT(UImage, IMG_NextStateFx2)
	GET_SLOT(UImage, IMG_NextStateFx3)
	GET_SLOT(UImage, IMG_NextStateSW)
	GET_SLOT(UImage, IMG_Clear)
	GET_SLOT(UImage, IMG_ClearFX)

	GET_SLOT(UTextBlock, TB_StageNum)
	GET_SLOT(UTextBlock, TB_ChapterNum)
	GET_SLOT(UTextBlock, TB_StageName)
	GET_SLOT(UTextBlock, TB_Hyphen)
	GET_SLOT(UImage, IMG_CinemaIcon)
	GET_SLOT(UImage, IMG_CharFlipbook)
	
	GET_SLOT(UB2UIRepeatableElements, UIP_Stars)
	if (UIP_Stars.IsValid())
		UIP_Stars->Init();
}

void UB2UIChapterStage::UpdateStaticText()
{
}

void UB2UIChapterStage::BindDelegates()
{
	
}

void UB2UIChapterStage::BindDoc(/*const FStage어쩌고& InStageData*/)
{
	UnbindDoc();

	auto DocChapter = UB2UIDocHelper::GetDocChapter();
	if (!DocChapter) return;
	
	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	if (StageInfoTable == nullptr)
		return;

	ClientStageId = StageInfoTable->GetClientStageIdOfChapterNums(ChapterNum, SubChapterNum);
	FSingleStageInfoData* ThisStageInfoData = StageInfoTable->GetInfoData(ClientStageId);
	SetCinemaIconVisible(ThisStageInfoData ? ThisStageInfoData->HasScenarioMovie() : false);

	// 난이도에 따른 StageID 재계산
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	ServerStageNum = StageDataStore.GetServerStageID(ClientStageId, StageDifficulty);
	LastClearClientStageId = StageDataStore.GetLastClearClientId(StageDifficulty);

	int32 MaxChapterNum = StageInfoTable ? StageInfoTable->GetStartAdventureModeChapterNum() : 0;
	
	if (MaxChapterNum >= ChapterNum)
	{
		if (ClientStageId > 0 && ClientStageId <= LastClearClientStageId)
		{
			SetStageState(EUIChapterStageState::Cleared);
		}
		else
		{
			if (ClientStageId == LastClearClientStageId + 1)
				SetStageState(EUIChapterStageState::NextDest);
			else
				SetStageState(EUIChapterStageState::NonCleared);
		}
	}
	else
	{

		int32 BeforeChapterNum = ChapterNum - 1;										//이전 챕터 지옥까지 깻는지 개산
		FServerStageID  LastClearClientStageIdHell = StageDataStore.GetLastClearServerStageId(EStageDifficulty::ESD_Hell);					//마지막 깬 serverStageID
		FServerStageID	ServerStageLastNum = StageDataStore.GetServerStageID(BeforeChapterNum * 10, EStageDifficulty::ESD_Hell);			//이전챕터 Hell마지막 스테이지 아이디

		if (ServerStageLastNum.Id <= LastClearClientStageIdHell.Id)
		{
			if(ClientStageId <= LastClearClientStageId)
				SetStageState(EUIChapterStageState::Cleared);
			else
			{
				if (ClientStageId == LastClearClientStageId + 1)
					SetStageState(EUIChapterStageState::NextDest);
				else
				{
					SetStageState(EUIChapterStageState::NonCleared);
					LastClearClientStageId = 0;
				}
			}

		}
		else
		{
			SetStageState(EUIChapterStageState::NonCleared);
			LastClearClientStageId = 0;
		}

		
	}
	
	SetClearedStar(BladeIIGameImpl::GetStageDataStore().GetStageClearGradeInfo(ServerStageNum));
	SetBonusType(BladeIIGameImpl::GetStageDataStore().GetStageBonusType(ClientStageId));
}

void UB2UIChapterStage::UnbindDoc()
{
	
}

void UB2UIChapterStage::SetStageDifficulty(EStageDifficulty InStageDifficulty)
{
	StageDifficulty = InStageDifficulty;

	SetChangeImage(InStageDifficulty);
}

void UB2UIChapterStage::SetSubChapterNum(int32 InValue)
{
	SubChapterNum = InValue;

	if (TB_StageNum.IsValid())
		TB_StageNum->SetText(FText::AsNumber(InValue));
}

void UB2UIChapterStage::SetChapterNum(int32 InValue)
{
	ChapterNum = InValue;

	if (TB_ChapterNum.IsValid())
		TB_ChapterNum->SetText(FText::AsNumber(InValue));
}

void UB2UIChapterStage::SetStageName(const FText& InText)
{
	if (TB_StageName.IsValid())
		TB_StageName->SetText(InText);
}

void UB2UIChapterStage::SetBonusType(EUIChapterStageBonusType InBonusType)
{
	SetBonusType_BP(InBonusType);
}

void UB2UIChapterStage::SetClearedStar(int32 InValue)
{
	if (UIP_Stars.IsValid())
		UIP_Stars->SetCurElementNum(InValue);

	SetClearedStar_BP(InValue);
}

void UB2UIChapterStage::SetCinemaIconVisible(bool bVisible)
{
	if (IMG_CinemaIcon.IsValid())
	{
		IMG_CinemaIcon->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UB2UIChapterStage::SetStageState(EUIChapterStageState InStateState)
{
	bool bStarVisible = false;
	FSlateFontInfo		Font;
	FSlateColor			FontColor;
	switch(InStateState)
	{
		default:
		case EUIChapterStageState::NonCleared:
			Font = FontNormal.Font;
			FontColor = FontNormal.FontColor;
			break;
		case EUIChapterStageState::Cleared:
			bStarVisible = true;
			Font = FontNormal.Font;
			FontColor = FontNormal.FontColor;
			break;
		case EUIChapterStageState::NextDest:
			Font = FontActive.Font;
			FontColor = FontActive.FontColor;
			break;
	}

	//클리어 안하면 별 보이면 안됨
	if (UIP_Stars.IsValid())
		bStarVisible ? UIP_Stars->SetVisibility(ESlateVisibility::HitTestInvisible) : UIP_Stars->SetVisibility(ESlateVisibility::Hidden);

	//폰트변경
	if (TB_StageNum.IsValid())
	{
		TB_StageNum->SetFont(Font);
		TB_StageNum->SetColorAndOpacity(FontColor);
	}
	if (TB_ChapterNum.IsValid())
	{
		TB_ChapterNum->SetFont(Font);
		TB_ChapterNum->SetColorAndOpacity(FontColor);
	}
	if (TB_Hyphen.IsValid())
	{
		TB_Hyphen->SetFont(Font);
		TB_Hyphen->SetColorAndOpacity(FontColor);
	}
	
	SetStageState_BP(InStateState);
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIChapterStage::OnClickBtnArea()
{
	//안 깬건 입장 못하게
	if (LastClearClientStageId + 1 >= ClientStageId)
	{
		BladeIIGameImpl::GetStageDataStore().RequestGetStageInfo(ServerStageNum, true);
		PlaySoundSelecting_BP();
	}
}

//====================================================================================
//									Doc Delegate
//====================================================================================


//====================================================================================
//									Input Override
//====================================================================================
#include "B2LobbyGameMode.h"

FReply UB2UIChapterStage::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bPressed = true;
		TimeInputStarted = FApp::GetCurrentTime();		
	}		

	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode)
		LobbyGameMode->SetPressedInputInUI(true);

	return FReply::Handled();
}

FReply UB2UIChapterStage::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{	
	return FReply::Unhandled();
}

FReply UB2UIChapterStage::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bPressed && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && (FApp::GetCurrentTime() - TimeInputStarted) <= GetTimeToDecideInputClickOrTab())
	{
		OnClickBtnArea();
	}		

	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode)
		LobbyGameMode->SetPressedInputInUI(false);

	bPressed = false;

	return FReply::Handled();
}

void UB2UIChapterStage::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bPressed = false;
}

FReply UB2UIChapterStage::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	UE_LOG(LogBladeII, Log, TEXT("TouchStart bPressed = %s"), bPressed ? TEXT("true") : TEXT("false"));

	TimeInputStarted = FApp::GetCurrentTime();
	bPressed = true;
	
	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode)
	{
		return LobbyGameMode->TouchStartFromUI(InGeometry, InGestureEvent);
	}

	return FReply::Unhandled();
}

FReply UB2UIChapterStage::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	UE_LOG(LogBladeII, Log, TEXT("TouchMove bPressed = %s"), bPressed ? TEXT("true") : TEXT("false"));
	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode)
	{
		return LobbyGameMode->TouchMoveFromUI(InGeometry, InGestureEvent);
	}

	return FReply::Unhandled();
}

FReply UB2UIChapterStage::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	double ElapsedTime = FApp::GetCurrentTime() - TimeInputStarted;

	UE_LOG(LogBladeII, Log, TEXT("TouchEnd bPressed = %s"), bPressed ? TEXT("true") : TEXT("false"));
	UE_LOG(LogBladeII, Log, TEXT("ElapsedTime = %0.2f"), static_cast<float>(ElapsedTime));

	if (bPressed && ElapsedTime <= GetTimeToDecideInputClickOrTab())
	{
		UE_LOG(LogBladeII, Log, TEXT("Call OnClickBtnArea"));
		OnClickBtnArea();
	}		

	bPressed = false;

	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode)
	{
		return LobbyGameMode->TouchEndFromUI(InGeometry, InGestureEvent);
	}	

	return FReply::Unhandled();
}

void UB2UIChapterStage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateChapterBossFlipbookMID();
}

void UB2UIChapterStage::UpdateChapterBossFlipbookMID()
{
	if (IMG_CharFlipbook.IsValid())
		CurrCharIntroFlipbookMID = IMG_CharFlipbook->GetDynamicMaterial();

	if (CurrCharIntroFlipbookMID)
	{
		// 해당 material 이 ExternalTime 을 사용하도록 세팅되어 있어야 작동할 것. 그게 아니면 material 내부적으로 시간이 자동 업데이트 될 것이다.
		CurrCharIntroFlipbookMID->SetScalarParameterValue(FName(TEXT("ExternalTime")), GetWorld()->GetRealTimeSeconds() - BossFlipbookMIDTimeSync);
	}
}

bool UB2UIChapterStage::IsChapterStageAllClear()
{
	UnbindDoc();

	auto DocChapter = UB2UIDocHelper::GetDocChapter();
	SetDoc(DocChapter);
	if (DocChapter)
		nCurChapterNum = DocChapter->GetCurChapterNum();

	if (BladeIIGameImpl::GetStageDataStore().IsFirstActClear(nCurChapterNum))
	{
		return true;
	}

	return false;

	//nClearStageNum = BladeIIGameImpl::GetStageDataStore().GetLastClearClientId(StageDifficulty);

	//if ((nCurChapterNum * 10) == nClearStageNum && nCurChapterNum > LoadClearChapterNum())
	//	bActClear = true;

	//return bActClear;
}

int32 UB2UIChapterStage::LoadClearChapterNum()
{
	int32 RetValue = 0;
	GConfig->GetInt(TEXT("ChapterState"), *FString::Printf(TEXT("ClearChapterNum")), RetValue, GB2UserSavedStateIni);
	return RetValue;
}
