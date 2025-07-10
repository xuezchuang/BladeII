// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
//#include "BladeII.h"
#include "B2UIWidget.h"
#include "B2UIActOpen.generated.h"


UCLASS()
class BLADEII_API UB2UIActOpen : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void PlayOpenAnim();

	UFUNCTION(BlueprintCallable, Category = "BladeII Chapter")
	void OnPlayOpenAnimFinish();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Chapter")
		void PlayOpenChapter_BP();

	void SetTextData(int32 nChapterNum);
protected:

private:
	TWeakObjectPtr<UImage> IMG_O_BG;
	TWeakObjectPtr<UImage> FX_O_Back_02;
	TWeakObjectPtr<UImage> FX_O_Back_01;
	TWeakObjectPtr<UImage> IMG_O_BG02;
	TWeakObjectPtr<UImage> IMG_O_Deco;
	TWeakObjectPtr<UImage> IMG_O_Deco03;
	TWeakObjectPtr<UImage> IMG_O_Deco04;
	TWeakObjectPtr<UImage> FX_O_Line01;
	TWeakObjectPtr<UImage> FX_O_Line02;

	TWeakObjectPtr<UB2Button> BTN_Back;
	TWeakObjectPtr<UCanvasPanel> ArenaOpen;

	TWeakObjectPtr<UTextBlock> TXT_Open;
};
