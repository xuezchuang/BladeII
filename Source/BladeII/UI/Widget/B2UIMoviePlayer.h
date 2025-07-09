// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIMoviePlayer.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIMoviePlayer : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	UB2UIMoviePlayer(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetMediaPlayer(class UMediaPlayer* InMediaPlayer);

	void SetMovie(class UMediaSource* InMediaSource, const TArray<struct FMovieSubtitle>& InSubtitleArray);
	
protected:
	virtual void NativeDestruct() override;

private:
	void CreateMovieSoundComponent();

protected:
	class UMediaPlayer* m_MediaPlayer;
	class UAudioComponent* m_MovieSoundComp;

	TArray<struct FMovieSubtitle> m_SubtitleArray;
};
