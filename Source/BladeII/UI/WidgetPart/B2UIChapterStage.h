// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIDocBindable.h"
#include "B2UIChapterStage.generated.h"

/* When the stage state change is the purpose of the font handling. */
USTRUCT(BlueprintType)
struct FChapterStageFontInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSlateFontInfo		Font;

	UPROPERTY(EditAnywhere)
		FSlateColor			FontColor;
};

USTRUCT(BlueprintType)
struct FChapterImageInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush LockImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush NextImageFx1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush NextImageFx2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush NextImageFx3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush NextImageSW;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush NextStateBG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush ClearImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush ClearImageFX;
};


/*
* Each data of the stage in the chapter.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIChapterStage : public UB2UIWidgetBase, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	/* font style of EUIChapterStageState::NonCleared, Cleared */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII ChapterStage")
		FChapterStageFontInfo FontNormal;

	/* font style of EUIChapterStageState::NextDest */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII ChapterStage")
		FChapterStageFontInfo FontActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII ChapterStage")
	TMap<EStageDifficulty, FChapterImageInfo> ChapterImages;


public:
	UB2UIChapterStage(const FObjectInitializer& ObjectInitializer);
	void BindDoc(/*const FStage绢录绊& InStageData*/);
	virtual void UnbindDoc() override;
	virtual void Init() override;

	//void SetDocStruct(FStage绢录绊 InStageData)			{ BindedStruct = InStageData; }
	//FStage绢录绊& GetDocStruct()						{ return BindedStruct; }

	void SetChangeImage(EStageDifficulty InStageDifficulty);
	void SetStageDifficulty(EStageDifficulty InStageDifficulty);
	void SetSubChapterNum(int32 InValue);
	int32 GetSubChapterNum() const { return SubChapterNum; }
	void SetChapterNum(int32 InValue);
	void SetStageName(const FText& InText);
	void SetBonusType(EUIChapterStageBonusType InBonusType);
	void SetClearedStar(int32 InValue);
	void SetCinemaIconVisible(bool bVisible);

	/* stage's cleared state. NonCleared, Cleared, NextDestination. */
	void SetStageState(EUIChapterStageState InStateState);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterStage")
		void SetBonusType_BP(EUIChapterStageBonusType InBonusType);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterStage")
		void SetClearedStar_BP(int32 ClearedStarNum);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterStage")
		void SetStageState_BP(EUIChapterStageState InStateState);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterStage")
		void PlaySoundSelecting_BP();
	UFUNCTION(BlueprintCallable, Category = "BladeII ChapterStage")
		bool IsChapterStageAllClear();
	

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//=================================================== Btn
	UFUNCTION()
		void OnClickBtnArea();	

	//==================================================== Doc Delegate

	void UpdateChapterBossFlipbookMID();

	int32 LoadClearChapterNum();

protected:
	TWeakObjectPtr<UImage>							IMG_Lock;
	TWeakObjectPtr<UImage>							IMG_NextStateBG;
	TWeakObjectPtr<UImage>							IMG_NextStateFx1;
	TWeakObjectPtr<UImage>							IMG_NextStateFx2;
	TWeakObjectPtr<UImage>							IMG_NextStateFx3;
	TWeakObjectPtr<UImage>							IMG_NextStateSW;
	TWeakObjectPtr<UImage>							IMG_Clear;
	TWeakObjectPtr<UImage>							IMG_ClearFX;

	TWeakObjectPtr<UTextBlock>						TB_StageNum;
	TWeakObjectPtr<UTextBlock>						TB_ChapterNum;
	TWeakObjectPtr<UTextBlock>						TB_Hyphen;
	TWeakObjectPtr<UTextBlock>						TB_StageName;
	TWeakObjectPtr<UImage>							IMG_CinemaIcon;
	TWeakObjectPtr<class UB2UIRepeatableElements>	UIP_Stars;

	TWeakObjectPtr<UImage>							IMG_CharFlipbook;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	bool bPressed;
	double TimeInputStarted;


private:
	//FStage绢录绊 BindedStruct;

	int32 ChapterNum;
	int32 SubChapterNum;

	int32 ClientStageId;
	int32 LastClearClientStageId;

	FServerStageID ServerStageNum;
	EStageDifficulty StageDifficulty;

	float BossFlipbookMIDTimeSync;
	class UMaterialInstanceDynamic* CurrCharIntroFlipbookMID;

	int32 nCurChapterNum;
	int32 nClearStageNum;
	bool bActClear;
};
