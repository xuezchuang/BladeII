// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UITalkDialog.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETalkOpeningAnimType : uint8
{
	None,
	Anim2,
	Anim3_1,
	Anim3_2,
	Anim4
};

UENUM(BlueprintType)
enum class ETalkAnimSequenceType : uint8
{
	None,
	Anim1, Anim2, Anim3, Anim4, Anim5,
	Anim6, Anim7, Anim8, Anim9, Anim10,
	Anim11, Anim12, Anim13, Anim14, Anim15
};

UENUM(BlueprintType)
enum class ETalkAnimBehaviorType : uint8
{
	None,
	Appear,
	Disappear,
	AppearAndDisappear
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITalkDialog : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	UB2UITalkDialog(const FObjectInitializer& ObjectInitializer);

	void OnPause(bool Pause);

	void SetTalkDialog(const int32 GroupID, const bool IsClear);
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void OnOpenComplete() override;
	virtual void OnCloseComplete() override;

	UFUNCTION()
	void OnClickedNext();

	UFUNCTION()
	void OnClickedSkip();

	UFUNCTION(BlueprintImplementableEvent)
	void SetModifyDepth_BP(int32 Index, bool IsSpeaker,  bool IsDirect = false);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowOpeningAnim_BP(ETalkOpeningAnimType AnimType);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowTalkAnimSequence_BP(ETalkAnimSequenceType AnimType);

private:
	void SetInitSpeakerPortrait(const struct FTalkLine &TalkLine);
	void SetSpeakerName(const struct FTalkLine &TalkLine);
	void SetSpeakerPortrait(const struct FTalkLine &TalkLine);
	void SetDialogMessage(const struct FTalkLine &TalkLine);
	void SetBackground(const struct FTalkLine &TalkLine);
	void SetPlaySound(const struct FTalkLine &TalkLine);
	void SetCompleteSpeaker(const struct FTalkLine &TalkLine);

	void InitPortraits();
	void StartFirstSequence();
	void OnStartNext(bool bForced = false);
	void SetNextSequence(bool bForced = false);
	bool HasNextSequence();
	void ShowCurrentSequence();
	const struct FTalkLine* GetCurrentTalkLine();

	bool GetIsCompleteTalk();
	void FlushTalkText();

	USoundCue* GetTalkSound(FName CharacterCodeName, FName SoundCodeName);
	void StopCurrentDialogSound();

	void UpdateManualTimeCounter();
	void UpdateTalkTimeCounter(float InDeltaTime);
	void UpdateNextTimeCounter(float InDeltaTime);
	void UpdateTalkText(float InDeltaTime);
	void UpdateBeginShow(float InDeltaTime);

	void SetPause(bool IsPause);

	void ApplyDialogText();

	void SetSelectDefaultBackground(bool ShowDefault);

	void SetCheckVisiblePortrait(int32 Index, bool IsShow);
	int32 GetShowedPortraitCount();
	ETalkOpeningAnimType GetTalkBeginAnim();
	ETalkAnimSequenceType GetTalkLineAnim(const struct FTalkLine &CompleteLine, const struct FTalkLine &NextLine);
	ETalkAnimBehaviorType GetTalkBehaviorType(const struct FTalkLine &CompleteLine, const struct FTalkLine &NextLine);
protected:
	//*~~~~~~~~~ TAsset 函版 抗沥
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<TSoftObjectPtr<UTexture2D>> BackgroundImages;

private:
	TArray<bool> ShowedPortrait;

	UPROPERTY(Transient)
	class UAudioComponent* CurrentPlayingDialogSound;

	const struct FTalkInfoInstance *CachedTalkInfo;

	TArray<TWeakObjectPtr<class UB2UITalkPortrait>> Portraits;

	TWeakObjectPtr<UButton> BTN_Dialogue_Next;
	TWeakObjectPtr<UB2Button> BTN_Area;

	TWeakObjectPtr<UB2Button> BTN_Skip;
	TWeakObjectPtr<UTextBlock> TB_Skip;
	TWeakObjectPtr<UTextBlock>	TB_Dialog;

	TWeakObjectPtr<UImage>	IMG_Background;
	TWeakObjectPtr<UImage>	IMG_DefaultBackground;

	FProgressiveTextBuilder ProgressiveDialogText;

	int32 TalkProgressIndex;
	int32 CachedGroupID;
	int32 LastBackgroundCode;

	FTimerHandle	AutoSkipTH;

	float MaxCharShowInterval;

	float ManualRealDeltaTime;
	float ManualLastRealTime;
	float ManualPauseDiffTime;

	float TalkLineTimerTarget;
	float TalkLineTimerCounter;

	float NextTimerTarget;
	float NextTimerCounter;

	float ShowTimerCounter;

	bool bPause;
	bool bSwithTalkLineTimer;
	bool bSwithNextSequenceTimer;
	bool bBeginShow;
	bool CachedClearMode;
};
