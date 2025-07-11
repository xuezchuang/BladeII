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
	// ��� �Ϸ�� ���Ŵ� BP�� ����.
	m_MediaPlayer = InMediaPlayer;
}

void UB2UIMoviePlayer::SetMovie(class UMediaSource* InMediaSource, const TArray<struct FMovieSubtitle>& InSubtitleArray)
{
	// ��������
	UGameplayStatics::SetGamePaused(this, true);
	
	// BGM ��ž
	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
		pGM->SetPauseBGM(true);
	
	// ���� ������Ʈ
	CreateMovieSoundComponent();

	if (m_MovieSoundComp)
		m_MovieSoundComp->Play();

	check(m_MediaPlayer);

	// ����. �ٷ�����ϰ� �������ִ�.
	m_MediaPlayer->OpenSource(InMediaSource);

	// �ڸ�(���常 �غ��� ���� ó������)
	m_SubtitleArray.Empty();
	m_SubtitleArray = InSubtitleArray;
}

void UB2UIMoviePlayer::NativeDestruct()
{
	Super::NativeDestruct();

	// �������� ����
	UGameplayStatics::SetGamePaused(this, false);

	// BGM �簳
	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
		pGM->SetPauseBGM(false);

	// ���� ������Ʈ ����
	if (m_MovieSoundComp)
	{
		m_MovieSoundComp->Stop();
		m_MovieSoundComp->DestroyComponent();
		m_MovieSoundComp = nullptr;
	}
}

void UB2UIMoviePlayer::CreateMovieSoundComponent()
{
	//// ���� ������Ʈ ����
	//if (m_MediaPlayer)
	//{
	//	UMediaSoundWave* pMSW = m_MediaPlayer->GetSoundWave();

	//	if (pMSW)
	//		m_MovieSoundComp = UGameplayStatics::SpawnSound2D(this, pMSW);
	//}
}
