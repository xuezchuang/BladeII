// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIFairyLevelUpResultPopup.generated.h"

DECLARE_DELEGATE(FOnFairyLevelUpResultFinished)

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFairyLevelUpResultPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	UB2UIFairyLevelUpResultPopup(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	UFUNCTION(BlueprintCallable)
	void ShowResult(EFairyType FairyType, bool bIsLevelUp);

	UFUNCTION(BlueprintCallable)
	void OnFinishedLevelUpAnimation();

	void SetFinishedHandler(const FOnFairyLevelUpResultFinished& InFinishedHandler);
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayLevelUpResult(bool bGetNewOption);

private:
	UFUNCTION()
	void OnClickBTN_Close();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	float AutoCloseDalayTime;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TMap<EFairyType, TSoftObjectPtr<class UMaterialInterface>>		NPCImage;

private:
	FTimerHandle DealyFinishedTH;

	FOnFairyLevelUpResultFinished OnFinishedHandler;

	TWeakObjectPtr<UB2Button> BTN_WholeArea;

	TWeakObjectPtr<UImage> IMG_NPC;
	TWeakObjectPtr<UImage> IMG_SkillIcon;
	TWeakObjectPtr<UTextBlock> TB_SkillName;
	TWeakObjectPtr<UTextBlock> TB_Unlock; 
	TWeakObjectPtr<UTextBlock> TB_LevelUpNotice;
	TWeakObjectPtr<UB2RichTextBlock> RTB_SkillDesc;

	TWeakObjectPtr<UTextBlock> TB_CharLevel;
	TWeakObjectPtr<UTextBlock> TB_CharLevel_Prev;

	TWeakObjectPtr<UPanelWidget> X_BlessUnlock;

	TWeakObjectPtr<UPanelWidget> X_BlessLevelup;

};
