// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIControlFindMatch.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIControlFindMatch : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetAllyPlayerCount(int32 nPlayerCount);

	void SetFindingEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_Anim_FindingEnd();

protected:
	virtual void CacheAssets() override;
	
public:
	UFUNCTION()
		void OnCancleBtnClick();

protected:
	TWeakObjectPtr<UTextBlock> TB_NameRank;
	TWeakObjectPtr<UTextBlock> TB_ATK_Value;
	TWeakObjectPtr<UTextBlock> TB_DEF_Value;
	TWeakObjectPtr<UTextBlock> TB_MatchingNotice;

	TWeakObjectPtr<UButton> BTN_Cancle;
	
	
};
