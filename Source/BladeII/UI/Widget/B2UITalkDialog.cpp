// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UITalkDialog.h"
#include "B2UITalkPortrait.h"
#include "B2TalkInfo.h"
#include "B2UIManager.h"
#include "B2ScenarioDialogInfo.h"
#include "B2PCClassInfo.h"
#include "Event.h"
#include "Engine/Texture.h"
#include "BladeIIGameMode.h"
#include "B2GameInstance.h"

UB2UITalkDialog::UB2UITalkDialog(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UB2UITalkDialog::NativeConstruct()
{
	Super::NativeConstruct();
}

void UB2UITalkDialog::Init()
{
	Super::Init();

	CachedGroupID = -1;
	LastBackgroundCode = -1;

	MaxCharShowInterval = 0.15f;

	ManualRealDeltaTime = 0.0f;
	ManualLastRealTime = 0.0f;
	ManualPauseDiffTime = 0.0f;

	bBeginShow = false;
	bPause = false;
	bSwithTalkLineTimer = false;
	bSwithNextSequenceTimer = false;
}

void UB2UITalkDialog::OnOpenComplete()
{
	Super::OnOpenComplete();

	// 오픈 후 바로 일시정지
	SetPause(true);
}

void UB2UITalkDialog::OnCloseComplete()
{
	StageEventTalkEndClass<int32, bool>::GetInstance().Signal(CachedGroupID, CachedClearMode);

	SetPause(false);

	Super::OnCloseComplete();
}

void UB2UITalkDialog::CacheAssets()
{
	GET_SLOT(UButton, BTN_Dialogue_Next);

	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UB2Button, BTN_Skip);
	GET_SLOT(UTextBlock, TB_Skip);
	
	GET_SLOT(UTextBlock, TB_Dialog);

	GET_SLOT(UImage, IMG_Background);
	GET_SLOT(UImage, IMG_DefaultBackground);

	const int32 MaxPortrait = 4;
	Portraits.SetNum(MaxPortrait);
	GET_SLOT_BYNAME(UB2UITalkPortrait, Portraits[0], Portrait_1);
	GET_SLOT_BYNAME(UB2UITalkPortrait, Portraits[1], Portrait_2);
	GET_SLOT_BYNAME(UB2UITalkPortrait, Portraits[2], Portrait_3);
	GET_SLOT_BYNAME(UB2UITalkPortrait, Portraits[3], Portrait_4);

	for (const auto &Each : Portraits)
	{
		Each->Init();
		Each->SetVisible(false);
	}
}

void UB2UITalkDialog::UpdateStaticText()
{
	ProgressiveDialogText.SetWholeText(TEXT(""), 0.0f, MaxCharShowInterval, ManualLastRealTime);
	ApplyDialogText();
}

void UB2UITalkDialog::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UITalkDialog::OnClickedNext);
	BIND_CLICK_FUNC_TO_BTN(BTN_Dialogue_Next, &UB2UITalkDialog::OnClickedNext);
	BIND_CLICK_FUNC_TO_BTN(BTN_Skip, &UB2UITalkDialog::OnClickedSkip);
}

void UB2UITalkDialog::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateManualTimeCounter();

	if (bBeginShow == false)
	{
		ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
		// Pre-render 가 돌아가고 있는 와중에는 BeginShow 를 지연시켜야겠다. 사운드가 도중에 나오면 곤란하니
		// AB2StageEventDirector::TemporaryDisableTracksForPreRender 와 마찬가지 차원
		if (!(B2GM && B2GM->IsInPreRenderPhase()))
		{
			UpdateBeginShow(InDeltaTime);
		}
	}

	UpdateTalkTimeCounter(InDeltaTime);
	UpdateNextTimeCounter(InDeltaTime);
	UpdateTalkText(InDeltaTime);
}

void UB2UITalkDialog::OnClickedNext()
{
	OnStartNext(true);
}

void UB2UITalkDialog::OnStartNext(bool bForced)
{
	if (GetIsCompleteTalk())
	{
		if (HasNextSequence() == false && bForced)
		{
			OnClickedSkip();
			return;
		}

		SetNextSequence(bForced);
	}
	else
	{
		FlushTalkText();
	}
}


void UB2UITalkDialog::OnClickedSkip()
{
	StopCurrentDialogSound();

	UB2UIManager::GetInstance()->CloseUI(UIFName::TalkDialog);
}

void UB2UITalkDialog::OnPause(bool Pause)
{
	bPause = Pause;
}

void UB2UITalkDialog::UpdateBeginShow(float InDeltaTime)
{
	// 대화 로딩시에 딜레이를 준다..
	ShowTimerCounter += InDeltaTime;

	if (ShowTimerCounter > 0.1f)
	{
		bBeginShow = true;

		UB2TalkInfo* TalkInfoTable = StaticFindTalkInfo();

		if (TalkInfoTable)
		{
			CachedTalkInfo = TalkInfoTable->GetTalkInfo(CachedGroupID);
			BII_CHECK(CachedTalkInfo);
			if (CachedTalkInfo != nullptr)
			{
				StartFirstSequence();
			}
			else
			{
				// 데이터가 없으면 그냥 종료 처리함...
				OnClickedSkip();
			}
		}
	}
}

void UB2UITalkDialog::UpdateManualTimeCounter()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		float ThisRealTime = TheWorld->GetRealTimeSeconds();
		ManualRealDeltaTime = ThisRealTime - ManualLastRealTime - ManualPauseDiffTime;
		
		ManualLastRealTime = ThisRealTime - ManualPauseDiffTime;
	}
	else
	{
		ManualRealDeltaTime = 0.0f;
		ManualLastRealTime = 0.0f;
		ManualPauseDiffTime = 0.0f;
	}
}

void UB2UITalkDialog::UpdateTalkTimeCounter(float InDeltaTime)
{
	if (bSwithTalkLineTimer)
	{
		TalkLineTimerCounter += InDeltaTime;
		if (TalkLineTimerTarget <= TalkLineTimerCounter)
		{
			// 대화 시간 완료 후 다음 대화로 넘어간다.
			NextTimerCounter = 0.0f;
			bSwithTalkLineTimer = false;

			OnStartNext(false);
		}
	}
}

void UB2UITalkDialog::UpdateNextTimeCounter(float InDeltaTime)
{
	if (bSwithNextSequenceTimer)
	{
		NextTimerCounter += InDeltaTime;
		if (NextTimerTarget <= NextTimerCounter)
		{
			// 다음 대화까지 딜레이 시간 처리 후 실행
			NextTimerCounter = 0.0f;
			bSwithNextSequenceTimer = false;

			const FTalkLine* CompleteLine = GetCurrentTalkLine();
			if(GetCurrentTalkLine() != nullptr)
				SetCompleteSpeaker(*CompleteLine);

			if (HasNextSequence() == false)
			{
				OnClickedSkip();
				return;
			}

			++TalkProgressIndex;

			ShowCurrentSequence();

			const FTalkLine* NextLine = GetCurrentTalkLine();
			ShowTalkAnimSequence_BP(GetTalkLineAnim(*CompleteLine, *NextLine));

		}
	}
}

void UB2UITalkDialog::UpdateTalkText(float InDeltaTime)
{
	bool IsLastChar = false;
	if (ProgressiveDialogText.Tick(ManualLastRealTime, InDeltaTime, IsLastChar))
		ApplyDialogText();
}


void UB2UITalkDialog::SetTalkDialog(const int32 GroupID, const bool IsClear)
{
 	CachedGroupID = GroupID;
	CachedClearMode = IsClear;
}

void UB2UITalkDialog::InitPortraits()
{
	for (auto &Each : Portraits)
	{
		Each->SetVisible(false);
	}

	ShowedPortrait.SetNum(Portraits.Num());

	TSet<FName> ContainCheck;
	TSet<EPortraitPosition> PositionCheck;
	for (const auto &Each : CachedTalkInfo->TalkSequence)
	{
		if (!ContainCheck.Find(Each.SpeakerCodeName))
		{
			if(PositionCheck.Find(Each.PortraitPosition))
				continue;
			PositionCheck.Add(Each.PortraitPosition);

			if(Each.AppearType != EPortraitAppearType::Default)
				continue;
			ContainCheck.Add(Each.SpeakerCodeName);

			int32 Position = static_cast<int32>(Each.PortraitPosition);
			SetCheckVisiblePortrait(Position, true);

			SetInitSpeakerPortrait(Each);
			SetSpeakerName(Each);
		}
	}
}

void UB2UITalkDialog::StartFirstSequence()
{
	TalkProgressIndex = 0;
	UWorld* TheWorld = GetWorld();
	ManualLastRealTime = TheWorld ? TheWorld->GetRealTimeSeconds() : 0.0f;

	InitPortraits();
	ShowCurrentSequence();
	ETalkOpeningAnimType BeginAnim = GetTalkBeginAnim();
	ShowOpeningAnim_BP(BeginAnim);
}

void UB2UITalkDialog::SetNextSequence(bool bForced)
{
	const FTalkLine *CompleteLine = GetCurrentTalkLine();
	if (CompleteLine == nullptr)
		return;

	if (!bForced)
	{
		if (CompleteLine->DelayToNextLine >= 0.0f) // 0 은 일단 안쓰는게..
		{// 다음 대화시까지 딜레이 시간이 있다면..
			NextTimerTarget = CompleteLine->DelayToNextLine;
			NextTimerCounter = 0.0f;
			bSwithNextSequenceTimer = true;

			return;
		}
	}

	SetCompleteSpeaker(*CompleteLine);

	if (HasNextSequence() == false)
		return;

	++TalkProgressIndex;

	ShowCurrentSequence();
	
	const FTalkLine* NextLine = GetCurrentTalkLine();
	ShowTalkAnimSequence_BP(GetTalkLineAnim(*CompleteLine, *NextLine));
}

bool UB2UITalkDialog::HasNextSequence()
{
	if (CachedTalkInfo == nullptr)
		return false;

	if (CachedTalkInfo->TalkSequence.Num() < 1)
		return false;

	const int32 NextIndex = TalkProgressIndex + 1;
	if (CachedTalkInfo->TalkSequence.Num() - 1 < NextIndex)
		return false;

	return true;
}

const FTalkLine* UB2UITalkDialog::GetCurrentTalkLine()
{
	if (CachedTalkInfo == nullptr)
		return nullptr;

	if (CachedTalkInfo->TalkSequence.Num() < 1)
		return nullptr;

	if (CachedTalkInfo->TalkSequence.Num() - 1 < TalkProgressIndex)
		return nullptr;

	return &CachedTalkInfo->TalkSequence[TalkProgressIndex];
}

bool UB2UITalkDialog::GetIsCompleteTalk()
{
	return ProgressiveDialogText.IsStringFilledUp();
}

void UB2UITalkDialog::ShowCurrentSequence()
{
	const FTalkLine *TalkLine = GetCurrentTalkLine();
	if (TalkLine == nullptr)
		return;

	TalkLineTimerCounter = 0.0f;
	TalkLineTimerTarget = TalkLine->ShowingTime;
	bSwithTalkLineTimer = true;

	SetSpeakerName(*TalkLine);
	SetSpeakerPortrait(*TalkLine);
	SetDialogMessage(*TalkLine);
	SetBackground(*TalkLine);
	SetPlaySound(*TalkLine);
}

void UB2UITalkDialog::SetSpeakerName(const struct FTalkLine &TalkLine)
{
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));

	auto* DialogCharacterInfoTable = B2GI ? B2GI->GetDialogCharacterInfoTable() : NULL;
	auto* CharacterInfo = DialogCharacterInfoTable ? DialogCharacterInfoTable->GetDialogCharacterInfo(TalkLine.SpeakerCodeName, this) : NULL;

	if (CharacterInfo)
	{
		int32 Position = static_cast<int32>(TalkLine.PortraitPosition);

		BII_CHECK(Portraits.IsValidIndex(Position))

		Portraits[Position]->SetSpeakerName(BladeIIGetLOCText(B2LOC_CAT_CHARNAME, CharacterInfo->CharacterNameKey.ToString()));
	}
}

void UB2UITalkDialog::SetInitSpeakerPortrait(const struct FTalkLine &TalkLine)
{
	SetSpeakerPortrait(TalkLine);

	int32 Position = static_cast<int32>(TalkLine.PortraitPosition);
	if(Portraits.IsValidIndex(Position))
		Portraits[Position]->SetInactivate();
}

void UB2UITalkDialog::SetSpeakerPortrait(const struct FTalkLine &TalkLine)
{
	int32 Position = static_cast<int32>(TalkLine.PortraitPosition);

	if (TalkLine.AppearType != EPortraitAppearType::OnlySound)
	{
		if (Portraits.IsValidIndex(Position))
		{
			Portraits[Position]->SetVisible(true);

			if (TalkLine.ModifyDepth)
				SetModifyDepth_BP(Position, true, false);

			SetCheckVisiblePortrait(Position, true);

			Portraits[Position]->SetReversePortrait(TalkLine.ReverseImage);

			UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
			UB2DialogCharacterInfoSet* DialogCharacterInfoTable = B2GI ? B2GI->GetDialogCharacterInfoTable() : NULL;
			if (DialogCharacterInfoTable)
			{
				Portraits[Position]->SetPortraitImage(DialogCharacterInfoTable->GetWholeBodyPortrait(TalkLine.SpeakerCodeName, this));
				Portraits[Position]->SetActivate();
			}
		}
	}
	else
	{
		if (Portraits.IsValidIndex(Position))
		{
			Portraits[Position]->SetVisible(false);
			Portraits[Position]->SetVisibleName();
			Portraits[Position]->SetActivate();

			SetModifyDepth_BP(Position, true, false);
		}
	}
}

void UB2UITalkDialog::SetDialogMessage(const struct FTalkLine &TalkLine)
{
	FText Message = BladeIIGetLOCText(B2LOC_CAT_TALKTEXT, TalkLine.TalkLineCodeName.ToString());
	ProgressiveDialogText.SetWholeText(Message.ToString(), TalkLine.ShowingTime, MaxCharShowInterval, ManualLastRealTime);
	ApplyDialogText();
}

void UB2UITalkDialog::SetBackground(const struct FTalkLine &TalkLine)
{
	if (TalkLine.BackgroundCode < 0)
	{
		SetSelectDefaultBackground(true);
	}
	else
	{
		if (BackgroundImages.IsValidIndex(TalkLine.BackgroundCode) == false)
		{
			SetSelectDefaultBackground(true);
		}
		else
		{
			SetSelectDefaultBackground(false);
			// 대화마다 기존과 배경 변경이  있을 경우에만 바꿈.
			if (IMG_Background.IsValid() && LastBackgroundCode != TalkLine.BackgroundCode)
			{
				LastBackgroundCode = TalkLine.BackgroundCode;
				UTexture2D *BackTex = BackgroundImages[TalkLine.BackgroundCode].LoadSynchronous();
				if (BackTex)
					IMG_Background->SetBrushFromTexture(BackTex);
			}
		}
	}
}

void UB2UITalkDialog::SetPlaySound(const struct FTalkLine &TalkLine)
{
	StopCurrentDialogSound(); // 중단부터

	USoundCue* DialogSound = GetTalkSound(TalkLine.SpeakerCodeName, TalkLine.TalkSoundCodeName);
	if (DialogSound)
	{
		CurrentPlayingDialogSound = UGameplayStatics::SpawnSound2D(this, DialogSound);
	}
}

void UB2UITalkDialog::SetSelectDefaultBackground(bool ShowDefault)
{
	if (ShowDefault)
	{
		if (IMG_Background.IsValid() && IMG_Background->GetVisibility() != ESlateVisibility::Hidden)
			IMG_Background->SetVisibility(ESlateVisibility::Hidden);

		if (IMG_DefaultBackground.IsValid() && IMG_DefaultBackground->GetVisibility() != ESlateVisibility::HitTestInvisible)
			IMG_DefaultBackground->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		if (IMG_DefaultBackground.IsValid() && IMG_DefaultBackground->GetVisibility() != ESlateVisibility::Hidden)
			IMG_DefaultBackground->SetVisibility(ESlateVisibility::Hidden);

		if (IMG_Background.IsValid() && IMG_Background->GetVisibility() != ESlateVisibility::HitTestInvisible)
			IMG_Background->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UB2UITalkDialog::SetCompleteSpeaker(const struct FTalkLine &TalkLine)
{
	int32 Position = static_cast<int32>(TalkLine.PortraitPosition);
	if (TalkLine.SpeakerLeave)
	{
		SetCheckVisiblePortrait(Position, false);
	}
	
	if (Portraits.IsValidIndex(Position))
		Portraits[Position]->SetInactivate();
}

USoundCue* UB2UITalkDialog::GetTalkSound(FName CharacterCodeName, FName SoundCodeName)
{
	UB2DialogSoundInfoSet* DialogSoundInfoTable = StaticFindTalkSoundInfoTable();
	return DialogSoundInfoTable ? DialogSoundInfoTable->GetInfoSound(CharacterCodeName, SoundCodeName, this) : NULL;
}

void UB2UITalkDialog::StopCurrentDialogSound()
{
	if (CurrentPlayingDialogSound)
	{
		//if (!CurrentPlayingDialogSound->IsPendingKill())
		{
			CurrentPlayingDialogSound->Stop();
			CurrentPlayingDialogSound->ConditionalBeginDestroy();
		}
	}
	CurrentPlayingDialogSound = NULL;
}

void UB2UITalkDialog::SetPause(bool IsPause)
{
	if (UGameplayStatics::IsGamePaused(this) == !IsPause)
		UGameplayStatics::SetGamePaused(this, IsPause);

	OnPause(IsPause);
}

void UB2UITalkDialog::FlushTalkText()
{
	ProgressiveDialogText.ApplyToWholeString();
	ApplyDialogText();
}

void UB2UITalkDialog::ApplyDialogText()
{
	if (TB_Dialog.IsValid())
	{
		TB_Dialog->SetText(FText::FromString(ProgressiveDialogText.GetCurrentString()));
	}
}

void UB2UITalkDialog::SetCheckVisiblePortrait(int32 Index, bool IsShow)
{
	if (ShowedPortrait.IsValidIndex(Index))
		ShowedPortrait[Index] = IsShow;
}

int32 UB2UITalkDialog::GetShowedPortraitCount()
{
	int32 AppearCount = 0;
	for (int32 i = 0; i < ShowedPortrait.Num(); ++i)
	{
		if (ShowedPortrait[i])
			++AppearCount;
	}
	return AppearCount;
}

ETalkOpeningAnimType UB2UITalkDialog::GetTalkBeginAnim()
{
	int32 AppearCount = GetShowedPortraitCount();
	const int32 CastPosition = static_cast<int32>(EPortraitPosition::Position_4);
	bool IsAnimRight = ShowedPortrait.IsValidIndex(CastPosition) ? ShowedPortrait[CastPosition] : false;

	const int32 Anim2 = 2;
	const int32 Anim3 = 3;
	const int32 Anim4 = 4;
	
	ETalkOpeningAnimType ResultAnim = ETalkOpeningAnimType::None;
	switch (AppearCount)
	{
	case Anim2:
		ResultAnim = ETalkOpeningAnimType::Anim2;
		break;
	case Anim3:
		if(IsAnimRight)
			ResultAnim = ETalkOpeningAnimType::Anim3_2;
		else
			ResultAnim = ETalkOpeningAnimType::Anim3_1;
		break;
	case Anim4:
		ResultAnim = ETalkOpeningAnimType::Anim4;
		break;
	default:
		ResultAnim = ETalkOpeningAnimType::None;
		break;
	}

	return ResultAnim;
}

ETalkAnimSequenceType UB2UITalkDialog::GetTalkLineAnim(const struct FTalkLine &CompleteLine, const struct FTalkLine &NextLine)
{
	const int32 Appeared2 = 2;	// 2인 화자일 경우
	const int32 Appeared3 = 3;	// 3인 화자일 경우
	const int32 Appeared4 = 4;	// 4인 화자일 경우

	int32 AppearCount = GetShowedPortraitCount();
	ETalkAnimBehaviorType BehaviorType = GetTalkBehaviorType(CompleteLine, NextLine);
	switch (BehaviorType)
	{
	case ETalkAnimBehaviorType::Appear:
		{	// 화자가 추가될때
			if (AppearCount == Appeared3)
			{
				if (NextLine.PortraitPosition == EPortraitPosition::Position_3)
					return ETalkAnimSequenceType::Anim1;
				else
					return ETalkAnimSequenceType::Anim2;
			}
			else if (AppearCount == Appeared4)
			{
				if (NextLine.PortraitPosition == EPortraitPosition::Position_3)
					return ETalkAnimSequenceType::Anim4;
				else
					return ETalkAnimSequenceType::Anim5;
			}
		}
		break;
	case ETalkAnimBehaviorType::Disappear:
		{	// 화자가 빠질때
			if (AppearCount == Appeared2)
			{
				if (CompleteLine.PortraitPosition == EPortraitPosition::Position_1)
					return ETalkAnimSequenceType::Anim14;
				else if (CompleteLine.PortraitPosition == EPortraitPosition::Position_2)
					return ETalkAnimSequenceType::Anim15;
				else if (CompleteLine.PortraitPosition == EPortraitPosition::Position_3)
					return ETalkAnimSequenceType::Anim6;
				else
					return ETalkAnimSequenceType::Anim7;
			}
			else if (AppearCount == Appeared3)
			{
				if (CompleteLine.PortraitPosition == EPortraitPosition::Position_3)
					return ETalkAnimSequenceType::Anim8;
				else if (CompleteLine.PortraitPosition == EPortraitPosition::Position_4)
					return ETalkAnimSequenceType::Anim9;
			}
		}
		break;
	case ETalkAnimBehaviorType::AppearAndDisappear:
		{	// 화자가 추가되고 빠질때
			if (CompleteLine.PortraitPosition == EPortraitPosition::Position_3
				&& NextLine.PortraitPosition == EPortraitPosition::Position_4)
			{
				return ETalkAnimSequenceType::Anim11;
			}
			else if (CompleteLine.PortraitPosition == EPortraitPosition::Position_4
				&& NextLine.PortraitPosition == EPortraitPosition::Position_3)
			{
				return ETalkAnimSequenceType::Anim12;
			}
		}
		break;	
	case ETalkAnimBehaviorType::None:
	default:
			break;
	}

	return ETalkAnimSequenceType::None;
}

ETalkAnimBehaviorType UB2UITalkDialog::GetTalkBehaviorType(const struct FTalkLine &CompleteLine, const struct FTalkLine &NextLine)
{
	if (CompleteLine.SpeakerLeave == false
		&& NextLine.AppearType == EPortraitAppearType::OnTime)
	{	// 화자가 추가될때
		return ETalkAnimBehaviorType::Appear;
	}
	else if (CompleteLine.SpeakerLeave == true
		&& NextLine.AppearType == EPortraitAppearType::Default)
	{	// 화자가 빠질때
		return ETalkAnimBehaviorType::Disappear;
	}
	else  if (CompleteLine.SpeakerLeave == true
		&& NextLine.AppearType == EPortraitAppearType::OnTime)
	{	// 화자가 추가되고 빠질때
		return ETalkAnimBehaviorType::AppearAndDisappear;
	}
	
	return ETalkAnimBehaviorType::None;
}