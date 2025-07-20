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
	// 犁积 肯丰矫 力芭绰 BP俊 乐澜.
	m_MediaPlayer = InMediaPlayer;
}

void UB2UIMoviePlayer::SetMovie(class UMediaSource* InMediaSource, const TArray<struct FMovieSubtitle>& InSubtitleArray)
{
	// 霸烙沥瘤
	UGameplayStatics::SetGamePaused(this, true);
	
	// BGM 胶啪
	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
		pGM->SetPauseBGM(true);
	
	// 荤款靛 哪器惩飘
	CreateMovieSoundComponent();

	if (m_MovieSoundComp)
		m_MovieSoundComp->Play();

	check(m_MediaPlayer);

	// 坷锹. 官肺犁积窍霸 汲沥登乐促.
	m_MediaPlayer->OpenSource(InMediaSource);

	// 磊阜(历厘父 秦焊备 酒流 贸府救窃)
	m_SubtitleArray.Empty();
	m_SubtitleArray = InSubtitleArray;
}

void UB2UIMoviePlayer::NativeDestruct()
{
	Super::NativeDestruct();

	// 霸烙沥瘤 秦力
	UGameplayStatics::SetGamePaused(this, false);

	// BGM 犁俺
	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
		pGM->SetPauseBGM(false);

	// 荤款靛 哪器惩飘 力芭
	if (m_MovieSoundComp)
	{
		m_MovieSoundComp->Stop();
		m_MovieSoundComp->DestroyComponent();
		m_MovieSoundComp = nullptr;
	}
}

void UB2UIMoviePlayer::CreateMovieSoundComponent()
{
	//// 荤款靛 哪器惩飘 悸泼
	//if (m_MediaPlayer)
	//{
	//	UMediaSoundWave* pMSW = m_MediaPlayer->GetSoundWave();

	//	if (pMSW)
	//		m_MovieSoundComp = UGameplayStatics::SpawnSound2D(this, pMSW);
	//}
}
