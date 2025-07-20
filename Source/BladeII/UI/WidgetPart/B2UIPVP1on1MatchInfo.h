// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIPVPEntryCharacters.h"
#include "B2UIPVP1on1MatchInfo.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPVP1on1MatchInfo : public UB2UIWidget
{
	GENERATED_BODY()

	UB2UIPVP1on1MatchInfo(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;
	virtual void UpdateStaticText();

	void SetLeft(bool bSet);
	void SetAttackPower(int32 InAttackPower);
	void SetDefense(int32 InDefense);
	void SetGradeAndStar(int32 Grade, int32 Star);
	void SetUserName(const FString& UserName);
	void SetWinningStreak(int32 InWins);
	void SetCombatPower(int32 InValue);
	void SetMyBadgeImage(bool _isLocalPlayer);

	void SetVisibilityStarGrade(bool bVisible);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//[@AKI, 170413] M16_老措老&评措傈俺急栏肺 救俊 郴侩父 昏力.
	//眠饶俊 鞘夸且 啊瓷己档 乐绢焊咯 窃荐绰 巢败 初澜
	void SetChracterLevel(int32 InLevel) {};
	void SetGuildNameAndMark(const FString& GuildName, int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor) {};
	////////////////////////////////////////////////////////////////////////////////////////////////////

protected:
	virtual void CacheAssets() override;

public:
	UPROPERTY(EditAnywhere, Category = "BladeII")
	TArray<FString> BattleMentTextKeys;
	
private:
	TWeakObjectPtr<class UB2UIGradeRankAndStar> UIP_GradeStar;

	TWeakObjectPtr<UTextBlock> TB_AttackPowerText;
	TWeakObjectPtr<UTextBlock> TB_AttackPowerValue;
	TWeakObjectPtr<UPanelWidget> P_AttackPowerInfo;

	TWeakObjectPtr<UTextBlock> TB_DefenseText;
	TWeakObjectPtr<UTextBlock> TB_DefenseValue;
	TWeakObjectPtr<UPanelWidget> P_DefenseInfo;

	TWeakObjectPtr<UTextBlock> TB_UserLevel;
	TWeakObjectPtr<UTextBlock> TB_UserName;
	TWeakObjectPtr<UTextBlock> TB_GuildName;
	TWeakObjectPtr<UTextBlock> TB_WinningStreak;
	TWeakObjectPtr<UTextBlock> TB_CombatPower;
	TWeakObjectPtr<class UB2UIGuildMark> UIP_GuildMark;

	TWeakObjectPtr<UOverlay>	Talk;
	TWeakObjectPtr<UTextBlock>	TB_BattleMent;
	TWeakObjectPtr<UB2Image>	IMG_MyBadge;

private:
	bool bLeft;
	EPCClass MainClass;
	EPCClass SubClass;
};
