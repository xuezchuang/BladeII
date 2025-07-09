// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIGuildSeasonReward.generated.h"


UCLASS()
class BLADEII_API UB2UIGuildSeasonReward : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void OnOpen(bool RightNow = false);
protected:
	UFUNCTION()
	void OnClickBTN_Confirm();

public:
	void SetGuildInfo();
	void SetGuildRewardInfo(int32 Cost);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TSubclassOf<class UB2UIGuildSeasonRewardSlot> RewardSlot;

private:
	TWeakObjectPtr<class UB2UIGuildMark> UIP_GuildMark;
	TWeakObjectPtr<UB2RichTextBlock> TB_GuildName;
	TWeakObjectPtr<UB2RichTextBlock> TB_GuildScore;
	TWeakObjectPtr<UB2RichTextBlock> TB_Rank;
	TWeakObjectPtr<UB2Button> BTN_Confirm;

	TWeakObjectPtr<UVerticalBox>		VB_LeftList;
	TWeakObjectPtr<UVerticalBox>		VB_RightList;

};
