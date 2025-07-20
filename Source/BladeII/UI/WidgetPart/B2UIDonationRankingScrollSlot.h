// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIDonationRankingScrollSlot.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDonationRankingScrollSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetData(struct FRankerData InRankerData);

	UFUNCTION()
		void OnClickUserInfo();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	int64 AccountID;

	TWeakObjectPtr<UTextBlock> TB_Ranking;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_Name;
	TWeakObjectPtr<UB2RichTextBlock> X_RTB_Point;
	TWeakObjectPtr<UImage> IMG_RankBG_1;
	TWeakObjectPtr<UImage> IMG_RankBG_2;
	TWeakObjectPtr<UImage> IMG_RankBG_3;
	TWeakObjectPtr<UB2Button> BTN_UserInfo;
};
