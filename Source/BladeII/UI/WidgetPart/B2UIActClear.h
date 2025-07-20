// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
//#include "BladeII.h"
#include "B2UIWidget.h"
#include "B2UIActClear.generated.h"


UCLASS()
class BLADEII_API UB2UIActClear : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void PlayClearAnim();

	UFUNCTION(BlueprintCallable, Category = "BladeII Chapter")
	void OnPlayClearAnimFinish();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Chapter")
		void PlayClearChapter_BP();

	void SetTextData(int32 nChapterNum);

	void HasAnyTutorialVisibleCheck();
	void HasAnyTutorialActClearAnimStartCheck();

protected:

private:
	TWeakObjectPtr<UImage> IMG_C_BG;
	TWeakObjectPtr<UImage> FX_Back_02;
	TWeakObjectPtr<UImage> FX_Back_01;
	TWeakObjectPtr<UImage> IMG_C_BG02;
	TWeakObjectPtr<UImage> IMG_C_Deco;
	TWeakObjectPtr<UImage> FX_Back_03;
	TWeakObjectPtr<UImage> IMG_Deco03;
	TWeakObjectPtr<UImage> IMG_Deco04;
	TWeakObjectPtr<UB2Button> BTN_Back;
	TWeakObjectPtr<UCanvasPanel> ArenaClear;

	TWeakObjectPtr<UTextBlock> TXT_Clear;
	TWeakObjectPtr<UTextBlock> TXT_ClearDoc;
};
