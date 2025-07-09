// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIWidget.generated.h"

/*
* The class of Open and close as possible.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIWidget : public UB2UIWidgetBase
{
	GENERATED_BODY()

	friend class UB2UIManager;

public:
	UB2UIWidget(const FObjectInitializer& ObjectInitializer);
	~UB2UIWidget();
	virtual void Init() override;
	virtual void NativeConstruct() override;
	/** Before you use DestroySelf directly, consider CloseUI or UIManager first. 
	 * InUIManager is for least safety, but it could be null in some special circumstance. */
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	/** Events occurring in the open. 
    * @param RightNow : When you open immediately without Anim. */
	virtual void OnOpen(bool RightNow = false);

	/** Events occurring in the close. 
	* @param RightNow : When you close immediately without Anim. */
	virtual void OnClose(bool RightNow = false);
	
	/** Called when a UIScene containing this UIWidget is open, not just this UIWidget, but all UIWidgets in the UIScene. */
	virtual void OnSceneOpen(EUIScene InOpenedScene);

	virtual void OnHistoryOpen(bool IsHistory);

	/* To really pause the game one tick after pause button click. */
	virtual void HandlePendingPause();


	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FName		GetMyUIFName()	const { return MyUIFName; }
	EUIOpenState GetOpenState()	const { return OpenState; }


public:
	/** BP event that passed in the open. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void OnOpen_BP();

	/** BP event that passed in the close. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void OnClose_BP();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void CloseSelf();

protected:
	/* Events that occur when finished completely open. */
	virtual void OnOpenComplete();
	/* Events that occur when finished completely close. */
	virtual void OnCloseComplete();

private:
	
	/** From UIManager WidgetData setting. 
	 * My registered identifier to the UIManager as one of WidgetData which can be opened solely. */
	FName MyUIFName;

	EUIOpenState OpenState;
protected:
	/** For some case that sub-class need to modify OpenState setting timing. */
	FORCEINLINE void SetOpenState(EUIOpenState InState) { OpenState = InState; }
	
	void SetPendingPause();
private:

	/** Transient flag for if using WorldBackgroundBlur now. */
	uint32 bWorldBackgroundBlurOn : 1;

	/** True for one tick after player pressed pause button and the pause menu is invoked but game is not paused yet. */
	uint32 bPendingPause : 1;
};
