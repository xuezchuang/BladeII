// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIEnterAdventure.generated.h"


/**
 * 
 */
DECLARE_DELEGATE(FClickAdventureSlot)

UCLASS()
class BLADEII_API UB2UIEnterAdventure : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;

	virtual void RegisterUIMarkForRedDot() final;

	virtual void OnOpen(bool RightNow = false);

	void ChangeChaperScene(int32 ChapterNum);

	virtual void UpdateStaticText() override;

	virtual void UpdateServercmdGetAccount() override;

public:
	UFUNCTION()
		void OnClickAct15();
	UFUNCTION()
		void OnClickAct6();
	UFUNCTION()
		void OnClickAct7();

public:
	TWeakObjectPtr<class UB2UIEnterAdventureSlot>		UIP_15Act;
	TWeakObjectPtr<class UB2UIEnterAdventureSlot>		UIP_6Act;
	TWeakObjectPtr<class UB2UIEnterAdventureSlot>		UIP_7Act;
};

UCLASS(Blueprintable, BlueprintType)
class BLADEII_API UB2UIEnterAdventureSlot : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void OnOpen(bool RightNow = false);

	void SetAdventureLock(bool InState, int32 InChapter = 0);
	void SetAdventureType(int32 InState);
	void SetDungeonNameText(FText InText);
	void SetDungeonDescText(FText InText);
	void SetNoEnterText(FText InText);

	UFUNCTION(BlueprintImplementableEvent)
		void OpenAnimation();
public:
	UFUNCTION()
		void OnClickAdventure();

public:

	TWeakObjectPtr<UB2Button>				BTN_Select;
	TWeakObjectPtr<UOverlay>				OV_NoEnter;
	TWeakObjectPtr<UOverlay>				Act_6;
	TWeakObjectPtr<UOverlay>				Act_7;


	TWeakObjectPtr<UImage>					Img_ACT6NoEnter;
	TWeakObjectPtr<UImage>					Img_ACT7NoEnter;


	TWeakObjectPtr<UTextBlock>				TB_DungeonName;
	TWeakObjectPtr<UTextBlock>				TB_DungeonDesc;
	TWeakObjectPtr<UB2RichTextBlock>		TB_NoEnterText;

	FClickAdventureSlot				FOnClickDelegate;
};
