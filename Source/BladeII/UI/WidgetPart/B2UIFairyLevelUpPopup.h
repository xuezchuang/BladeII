// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include <memory>
#include "../Common/EventSubsystem.h"
#include "B2UIFairyLevelUpPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFairyLevelUpPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	void SetFairyInfo(EFairyType Fairy);


	UFUNCTION(BlueprintCallable)
	void OnFinishedLevelUpAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayLevelUpAnimation();

	UFUNCTION()
	void OnClickBTN_Close();

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	//UFUNCTION(BlueprintImplementableEvent)
	//virtual void OnOpen_BP()override;
	
	void OnLevelUpFairy(EFairyType Fairy, bool IsLevelUp);
	void OnErrorLevelUpFairy(int32 Error);

	bool IsMaxLevel();
	bool IsEnoughMaterial();

	bool CheckAutoLevelUp();
	void StopAutoLevelUp();

	void UpdateLevelUpButton();
	void UpdateLevelStepIcons();
	void UpdateHeaderState(bool NewState);

	void ShowLevelMaxNotice();
	void ShowLevelUpAutoCancelNotice();

	void ShowSimplePopup(FText MessageText);

	void SubscribeEvent();
	void UnSubscribeEvent();
	bool bSubscribed;
	TArray<std::shared_ptr<issue>> Issues;

private:
	UFUNCTION()
	void OnClickBTN_AutoLevelUp();

	UFUNCTION()
	void OnClickBTN_LevelUp();

private:
	bool bLoopReceiveAutoLevelUp;
	bool bAnimationLevelUp;

	FTimerHandle LoopReceiveAutoLevelUpTH;

	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_AutoLevelUp;
	TWeakObjectPtr<UB2Button> BTN_LevelUp;

	TWeakObjectPtr<UImage> IMG_MaterialIcon;
	TWeakObjectPtr<UTextBlock> TB_MaterialInfo;

	TWeakObjectPtr<UB2RichTextBlock> RTB_Condition;

	TWeakObjectPtr<UTextBlock> TB_PrevewLevel;
	TWeakObjectPtr<UB2RichTextBlock> RTB_PreviewInfo;

	TWeakObjectPtr<UTextBlock> TB_Notice;
	TWeakObjectPtr<UTextBlock> TB_SubTitle;

	TWeakObjectPtr<UTextBlock> TB_AutoLevelUp;
	TWeakObjectPtr<UTextBlock> TB_LevelUp;
	TWeakObjectPtr<UTextBlock> TB_LevelUpCostNum;

	TArray<TWeakObjectPtr<class UB2UIFairyStepIcon>> FairyStepIconList;

	EFairyType CachedFairy;

	int32 CachedEnhancePoint;
	bool CachedIsLevelUp;

	float RequestTime;
	float ResponseTime;
};
