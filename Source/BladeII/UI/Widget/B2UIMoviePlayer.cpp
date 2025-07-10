// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIMoviePlayer.h"
#include "BladeIIGameMode.h"
//#include "MediaSoundWave.h"
#include "MediaPlayer.h"


UB2UIMoviePlayer::UB2UIMoviePlayer(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	m_MediaPlayer = nullptr;
	m_MovieSoundComp = nullptr;
	m_SubtitleArray.Empty();
}

void UB2UIMoviePlayer::SetMediaPlayer(class UMediaPlayer* InMediaPlayer)
{
	// 재생 완료시 제거는 BP에 있음.
	m_MediaPlayer = InMediaPlayer;
}

void UB2UIMoviePlayer::SetMovie(class UMediaSource* InMediaSource, const TArray<struct FMovieSubtitle>& InSubtitleArray)
{
	// 게임정지
	UGameplayStatics::SetGamePaused(this, true);
	
	// BGM 스탑
	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
		pGM->SetPauseBGM(true);
	
	// 사운드 컴포넌트
	CreateMovieSoundComponent();

	if (m_MovieSoundComp)
		m_MovieSoundComp->Play();

	check(m_MediaPlayer);

	// 오픈. 바로재생하게 설정되있다.
	m_MediaPlayer->OpenSource(InMediaSource);

	// 자막(저장만 해보구 아직 처리안함)
	m_SubtitleArray.Empty();
	m_SubtitleArray = InSubtitleArray;
}

void UB2UIMoviePlayer::NativeDestruct()
{
	Super::NativeDestruct();

	// 게임정지 해제
	UGameplayStatics::SetGamePaused(this, false);

	// BGM 재개
	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
		pGM->SetPauseBGM(false);

	// 사운드 컴포넌트 제거
	if (m_MovieSoundComp)
	{
		m_MovieSoundComp->Stop();
		m_MovieSoundComp->DestroyComponent();
		m_MovieSoundComp = nullptr;
	}
}

void UB2UIMoviePlayer::CreateMovieSoundComponent()
{
	//// 사운드 컴포넌트 셋팅
	//if (m_MediaPlayer)
	//{
	//	UMediaSoundWave* pMSW = m_MediaPlayer->GetSoundWave();

	//	if (pMSW)
	//		m_MovieSoundComp = UGameplayStatics::SpawnSound2D(this, pMSW);
	//}
}
