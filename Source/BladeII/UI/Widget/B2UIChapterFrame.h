// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIDocChapter.h"
#include "CommonStruct.h"
#include "B2UIChapterStarRewardPreview.h"
#include "B2UIActClear.h"
#include "B2UIActOpen.h"
#include "B2UIChapterFrame.generated.h"

/*
* Chapter Frame UI. Show each Chapter, and common controls are included.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIChapterFrame : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	UB2UIChapterFrame(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
public:
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	void ChangeChapterCycle(EUISearchType SearchDir);
	void ChangeChapter(int32 ChapterIndex, EStageDifficulty Difficulty);
	bool CheckIfTwoChapterUIsShareClass(int32 CheckChapterNum1, int32 CheckChapterNum2) const;

	class UB2UIChapter* CreateChapter(int32 TargetChaperNum);
	void ClearCurChapter();
	class UB2UIChapter* GetCurChapter();

	void SetChapterTitle(const FText& InText);
	void SetEnabledNextBtn(const bool bEnabled);
	void SetEnabledPrevBtn(const bool bEnabled);
	void SetVisibilityPrevNextButton(bool InVisibility);
	void SetStarRewards(const TArray<struct FUIChapterStarReward>& StarRewards);

	void SetVisibilityBtnAdventure(EStageDifficulty InStageDifficulty, const bool bEnabled);
	void SetMaxOpendAdventureDifficulty(EStageDifficulty InMaxStageDifficulty);

	bool HasTutorialChangeChapter1Check(EStageDifficulty eStageDifficulty);

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	virtual void RegisterUIMarkForRedDot() override;

	TSubclassOf<class UB2UIChapter> GetChapterWidgetClass(int32 ChapterIndex);
	void UpdateSearchBtnEnabled(int32 ChapterOpenedNum);
	void OnSelectBtnStageDifficulty(EStageDifficulty InDifficulty);

	// For ActOpen or ActClear..
	UB2UIWidget* CreateDynamicActFxUIP(UClass* InCreateWidgetClass, UCanvasPanel* CreateParentPanel);
#if !UE_BUILD_SHIPPING
public:
#endif
	void PlayActOpen();
	void PlayActClear();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterFrame")
	void PlayNextChapterTransitionSound(int32 ChapterBefore, int32 ChapterAfter);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterFrame")
	void PlayPrevChapterTransitionSound(int32 ChapterBefore, int32 ChapterAfter);
private:
	bool IsStarRewardPreviewOpen();

	//===================================================Click
	UFUNCTION()
		void OnClickBtnAdventureLastReached();
	UFUNCTION()
		void OnClickBtnNext();
	UFUNCTION()
		void OnClickBtnPrev();
	UFUNCTION()
		void OnClickBtnChapterStarRewardBox(class UB2UIChapterStarRewardBox* ClickedReward);
	UFUNCTION()
		void OnClickBtnStarRewardPreviewAllClose();

	//====================================================Doc Delegate
	UFUNCTION()
		void OnChangedCurChapterNum(class UB2UIDocBase* Sender, int32 CurrChapterNum, int32 PrevCurrChapterNum);
	UFUNCTION()
		void OnChangedCurChapterName(class UB2UIDocBase* Sender, FText CurChapterName, FText PrevCurChapterName);
	UFUNCTION()
		void OnChangedStarRewards(class UB2UIDocBase* Sender, TArray<struct FUIChapterStarReward> StarRewards, TArray<struct FUIChapterStarReward> PrevStarRewards);
	UFUNCTION()
		void OnChangedMaxOpenedStageDifficulty(class UB2UIDocBase* Sender, EStageDifficulty CurrentDifficulty, EStageDifficulty PrevDifficulty);
	UFUNCTION()
		void OnChangedTutorialFixSwipe(class UB2UIDocBase* Sender, bool CurFixSwipe, bool PrevFixSwipe);


	UFUNCTION()
		void OnClickBTN_Normal();
	UFUNCTION()
		void OnClickBTN_Hard();
	UFUNCTION()
		void OnClickBTN_VeryHard();
	UFUNCTION()
		void OnClickBTN_Hell();

	void ResponseChangedCurChapterNum(const FB2GetActInfo&);

	bool RedDotCondition_ChapterInternal(EStageDifficulty Difficulty);

	UFUNCTION()
		bool RedDotCondition_PrevChapter();
	UFUNCTION()
		bool RedDotCondition_NextChapter();

public:

	UPROPERTY(EditAnywhere, Category = "BladeII ChapterFrame")
		FString PrefixChapterStarReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII ChapterFrame")
		float TimeAutoScrolling;

protected:
	/** Chapter widget classes containing stage parts.. The key is chapter number. */
	UPROPERTY(EditAnywhere, Category = "BladeII ChapterFrame")
	TMap<int32, TSoftClassPtr<class UB2UIChapter>> ChapterWidgetClasses;

	/** Put chapter UI reference to make it being loaded on this ChapterFrame class load.
	 * Intended to take effect with one of UIManager CacheOnLoad / Rootset / LoadOnStartup settings.
	 * It is just keep it being loaded, and not for any actual code usage. */
	UPROPERTY(EditAnywhere, Category = "BladeII ChapterFrame")
	TArray<TSubclassOf<class UB2UIChapter>> KeepLoadedChapterWidgetClasses;

protected:
	TWeakObjectPtr<UButton>						BTN_AdventureLastReached;	//Start from where it was finally reached.
	TWeakObjectPtr<UButton>						BTN_Next;			//to next chapter
	TWeakObjectPtr<UButton>						BTN_Prev;			//to prev chapter
	TWeakObjectPtr<UPanelWidget>				CON_Chapter;		//Chapter container panel
	//TWeakObjectPtr<UScrollBox>					SB_Chapter;			//Chapter container scrollbox

	TWeakObjectPtr<UB2Button>					BTN_Normal;
	TWeakObjectPtr<UB2Button>					BTN_Hard;
	TWeakObjectPtr<UB2Button>					BTN_VeryHard;
	TWeakObjectPtr<UB2Button>					BTN_Hell;

	TWeakObjectPtr<UImage>						IMG_Normal;
	TWeakObjectPtr<UImage>						IMG_SelectedNormal;
	TWeakObjectPtr<UImage>						IMG_Hard;
	TWeakObjectPtr<UImage>						IMG_SelectedHard;
	TWeakObjectPtr<UImage>						IMG_VeryHard;
	TWeakObjectPtr<UImage>						IMG_SelectedVeryHard;
	TWeakObjectPtr<UImage>						IMG_Hell;
	TWeakObjectPtr<UImage>						IMG_SelectedHell;

	TWeakObjectPtr<UPanelWidget>				O_Normal;
	TWeakObjectPtr<UPanelWidget>				O_Hard;
	TWeakObjectPtr<UPanelWidget>				O_VeryHard;
	TWeakObjectPtr<UPanelWidget>				O_Hell;

	TMap<EStageDifficulty, TWeakObjectPtr<UB2Button>>			BTN_Adventures;
	TMap<EStageDifficulty, TWeakObjectPtr<UPanelWidget>>		P_Adventures;

	//Reward Info
	TArray<class UB2UIChapterStarRewardBox*>	StarRewardBoxes;
	class UB2UIChapterStarRewardBox* GetStarRewardBoxOfRewardIndex(int32 InRewardIndex);
	TWeakObjectPtr<class UB2UIChapterStarRewardPreview> UIP_StarRewardPreviewP; // Not like for each StarRewardBoxes. Only single one will take all information.
	
	TWeakObjectPtr<UB2Button>					BTN_StarRewardPreviewAllClose; // It is like covering all screen area.

	TWeakObjectPtr<UTextBlock>					TX_LastDG;
	TWeakObjectPtr<UTextBlock>					STB_Easy;
	TWeakObjectPtr<UTextBlock>					STB_Normal;
	TWeakObjectPtr<UTextBlock>					STB_Hard;
	TWeakObjectPtr<UTextBlock>					STB_Hell;

	// ActOpen / Clear Widgets are used only at certain circumstances. No reason to let it being loaded always.
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UB2UIActOpen> UIPActOpenClass;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UB2UIActClear> UIPActClearClass;
	// Will be valid only when needed
	UPROPERTY(Transient)
	UB2UIActOpen* CreatedActOpenUI;
	// Will be valid only when needed
	UPROPERTY(Transient)
	UB2UIActClear* CreatedActClearUI;
	TWeakObjectPtr<UCanvasPanel> X_CP_ActOpenClearUICreatePanel;
	
private:
	int32 CurChapterNum;
	EStageDifficulty CurDifficulty;

	int32 m_nClearStageNum;

	//Stage num is greater than the 10.  When you press BTN_Next, scroll process rather than a chapter change.
	bool IsOvered10Stages;

	void SetChapterDynamicSetupReady();

	/** Created UIChapter widget holding chapter parts that the user actually interact. */
	UPROPERTY(Transient)
	UB2UIChapter* m_TempChapter;

	FTimerHandle ChapterTimer;

protected:
	/** It might not be used but still necessary to keep it being loaded with ChapterFrame widget class. */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UB2UIChapterStage> ChapterStagePartClass_Holder;

};
