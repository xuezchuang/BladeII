// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UI/B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2GuildResult.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2GuildResult : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void UpdateStaticText() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void OnOpenComplete_BP(int32 ReceiveMedal, bool bWin);

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void OnOpenComplete() override;

	UFUNCTION()
		void OnClickBtnGuild();
	UFUNCTION()
		void OnClickBtnMain();

	TWeakObjectPtr<UButton>				BTN_GUILD;
	TWeakObjectPtr<UButton>				BTN_MAIN;
	TWeakObjectPtr<UImage>				IMG_Lose;
	TWeakObjectPtr<UImage>				IMG_Victory;
	TWeakObjectPtr<UHorizontalBox>		X_RewardBox;
	TWeakObjectPtr<UTextBlock>			TB_Error;
	TWeakObjectPtr<UTextBlock>			TB_BtnMain;
	TWeakObjectPtr<UTextBlock>			TB_BtnGuild;
	
	TArray<TWeakObjectPtr<class UB2UIGuildInfoMedal>>	MedalArr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TSubclassOf<class UB2UIRewardIcon>		GuildRewardSlot;
};