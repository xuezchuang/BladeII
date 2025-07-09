// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UITutorial.generated.h"


UCLASS()
class BLADEII_API UB2UITutorial : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void OnClose(bool RightNow) override;

	void TutorialUICloseTimer(const float UICloseSeconds);

	void ProcessStepHighlight(const struct FTutorialStep* StepInfo);
	void ProcessStepHighlight_Material(const struct FTutorialStep* StepInfo);
	void ProcessStepHighlight_Arrow(const struct FTutorialStep* StepInfo, const FVector2D& TopLeft, const FVector2D& DrawSize);

	void SetVisibileHighlightRect(bool bVisible);

	void ClearStepHighlight(const struct FTutorialStep* StepInfo);
	
	void SetTutorialDialog(const struct FTutorialStep* StepInfo, const FVector2D& DrawPosition);
	void SetHighlightMaterial(class UImage* ImageWidget, UMaterialInterface* BrushMtrl);

	UMaterialInterface* GetArrowMaterial(int32 ArrowIndex);

	FName GetCurrentDJLegacyUIName();

	UFUNCTION(BlueprintCallable, Category = "BladeII B2UITutorial")
	static bool GlobalTutorialProcessingCheck_BP();

	UFUNCTION(BlueprintCallable, Category = "BladeII B2UITutorial")
	static void GlobalOpenAnimCheckTutorialVisible_BP(bool bOpenAnimPlaying);

	// Debug
	void SetDebugInfo(const struct FTutorialStep* StepInfo);

	void HideAllTutorialUI();

	void HiddenTutorialUI();

// Tutorial Begin UI Panel
	void OpenTutorialBegin(int32 TutorialID);

	void OpenTutorialBeginUI(const struct FTutorialInfo* TutorialInfo);
	void CloseTutorialBeginUI();

// Skip Panel
	void OpenTutorialSkipNoticeUI(const int32 TutorialID);
	void CloseTutorialSkipNoticeUI();

// Contents Open Type
	void OpenDialogContentsOpenUI();
	void CloseDialogContentsOpenUI();

// Reward Type
	void OpenTutorialRewardUI(const int32 TutorialID, const TArray<b2network::B2RewardPtr>& ServerRewards);
	void CloseTutorialRewardUI();

	bool IsPlayingRewardAnimation();

	void SetVisibleSkipButton(bool bVisible) { bVisibleSkipButton = bVisible; }

public:
	
	virtual UCanvasPanelSlot* AddHighlightPart(UCanvasPanel* RootCanvasPanel, int32 QuadrantIdx);
	virtual void OnAddedHighlightRectButton(UButton* AddedHighlightRectButton);

	virtual void TickHighlight(const FVector2D& fv2Position, const FVector2D& fv2Size);
	virtual void TickHighlightDialog(const FVector2D& fv2Position);

	virtual void ActiveHighlightRect(const FVector2D& TopLeft, const FVector2D& DrawSize);

	int32	GetUIWidgetTuTorialID();
	int32	GetUIWidgetTuTorialStep();

	bool GetHighlightRedrawCheck() { return bHighlightRedrawCheck; }
	bool GetHighlightDialogRedrawCheck() { return bHighlightDialogRedrawCheck; }
	void SetHighlightRedrawCheck(bool bCheck) { bHighlightRedrawCheck = bCheck; }
	void SetHighlightDialogRedrawCheck(bool bCheck) { bHighlightDialogRedrawCheck = bCheck; }

	void ActiveTransparencyBlock(bool bCheck);
	
	UFUNCTION()
	void OnClickBTN_Skip();
	UFUNCTION()
	void OnClickBTN_Highlight();

	UFUNCTION()
	void OnClickBTN_SkipConfirm();
	UFUNCTION()
	void OnClickBTN_SkipCancel();

	UFUNCTION()
	void OnClickBTN_GotoLobby();

	UFUNCTION()
	void OnClickBTN_RewardConfirm();

	UFUNCTION()
	void OnClickBTN_BlockContentOpen();

	UFUNCTION()
	void OnClickBTN_MainHighlight();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnOpenTutorialBegin();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnShowContentOpenUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnVisibleHighlightArrow(bool bVisible);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnOpenTutorialSkipReward();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnOpenTutorialReward();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnVisibleAndSetLOCTextSkipBtn();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnVisibleDialogArrow();

	//Tutorial Main Panel
	TWeakObjectPtr<UCanvasPanel>	CP_MainTutorial;
	TWeakObjectPtr<UCanvasPanel>	CP_TransparencyBlock;

// Tutorial Begin UI Panel
	TWeakObjectPtr<UPanelWidget>	P_TutorialBegin;
	TWeakObjectPtr<UB2Image>		IMG_TutorialMain;
	TWeakObjectPtr<UTextBlock>		TB_TutorialName;
	TWeakObjectPtr<UTextBlock>		TB_TutorialDesc;


// Tutorial Step Dialog Panel
	TWeakObjectPtr<UCanvasPanel>	CP_Dialog;
	TWeakObjectPtr<UImage>			IMG_Speaker;
	TWeakObjectPtr<UButton>			BTN_Skip;
	TWeakObjectPtr<UB2RichTextBlock> RTB_Dialog;

	// ------------- Debug -------------------
	TWeakObjectPtr<UTextBlock>		TB_Debug1;
	TWeakObjectPtr<UTextBlock>		TB_Debug2;
	// ------------- Debug -------------------

// Reward UI Panel
	TWeakObjectPtr<UPanelWidget>	P_TutorialSkip;
	TWeakObjectPtr<UHorizontalBox>	HB_SkipRewardItems;
	TArray<TWeakObjectPtr<class UB2UIRewardIcon>>	SkipRewardIcons;

	TWeakObjectPtr<UTextBlock>		TB_TutorialSkipWarning;
	TWeakObjectPtr<UTextBlock>		TB_SkipPopupTitle;

	TWeakObjectPtr<UButton>			BTN_SkipConfirm;
	TWeakObjectPtr<UTextBlock>		TB_SkipConfirm;

	TWeakObjectPtr<UButton>			BTN_SkipCancel;
	TWeakObjectPtr<UTextBlock>		TB_SkipCancel;

// ContentOpen Panel
	TWeakObjectPtr<UPanelWidget>	P_ContentOpen;
	TWeakObjectPtr<UTextBlock>		TB_ContentOpenDesc;
	TWeakObjectPtr<UButton>			BTN_GoToLobby;
	TWeakObjectPtr<UTextBlock>		TB_GotoLobby;
	TWeakObjectPtr<UButton>			BlockContentOpen;

// Tutorial Reward UI Panel

	TWeakObjectPtr<UPanelWidget>	P_TutorialReward;
	TWeakObjectPtr<UHorizontalBox>	HB_TutorialRewardItems;
	TArray<TWeakObjectPtr<class UB2UIRewardIcon>>	TutorialRewardIcons;

	TWeakObjectPtr<UTextBlock>		TB_TutorialRewardTitle;
	TWeakObjectPtr<UTextBlock>		TB_RewardDestination;

	TWeakObjectPtr<UButton>			BTN_RewardConfirm;

	TWeakObjectPtr<UImage>			IMG_Arrow;
	TWeakObjectPtr<UImage>			IMG_DialogArrow;

	TWeakObjectPtr<UCanvasPanel>	CP_HilightMainBtn;
	TWeakObjectPtr<UButton>			BTN_HilightMain;

// Private Variables
	UPROPERTY(Transient)
	FTimerHandle			CloseTimer;

	UPROPERTY(Transient)
	class UImage*			HighlightImage;

	bool	bHighlightRedrawCheck;
	bool	bHighlightDialogRedrawCheck;

	bool bVisibleSkipButton;

// Blueprint Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> ArrowMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMargin HighlightMargin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> ArrayHighlightMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 iHighlightZOder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor HighlightRectColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TutorialOpenCloseTime;

	const struct FTutorialStep* CurrentTutorialStep;
};
