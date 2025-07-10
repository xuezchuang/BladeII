// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "B2UIWidget.h"
#include "B2UITeamMatchInfo.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITeamMatchInfo : public UB2UIWidget
{
	GENERATED_BODY()

	UB2UITeamMatchInfo(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;
	void SetLeft(bool bSet);
	void SetChracterLevel(int32 InLevel);
	void SetGradeAndStar(int32 Grade, int32 Star);
	void SetUserName(const FString& UserName);
	void SetGuildName(const FString& GuildName);
	void SetWinningStreak(int32 InWins);
	void SetMyBadgeImage(bool _isLocalPlayer);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_GradeStar;

	TWeakObjectPtr<UTextBlock> TB_UserLevel;
	TWeakObjectPtr<UTextBlock> TB_UserName;
	TWeakObjectPtr<UTextBlock> TB_GuildName;
	TWeakObjectPtr<UTextBlock> TB_WinningStreak;

	TWeakObjectPtr<UB2Image> IMG_MyBadge;

private:
	bool bLeft;
	EPCClass MainClass;
	EPCClass SubClass;
};
