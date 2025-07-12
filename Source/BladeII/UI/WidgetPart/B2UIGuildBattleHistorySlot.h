// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIGuildBattleHistorySlot.generated.h"


UCLASS()
class BLADEII_API UB2UIGuildBattleHistorySlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

public:
	void SetMyGuild(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor, FString GuildName, int32 Medal, int32 MaxMedal, int32 Result);
	void SetOtherGuild(int32 Pattern, int32 PatternColor, int32 BG, int32 BGColor, FString GuildName, int32 Medal, int32 MaxMedal);
	void SetTime(int64 UnixTime);
	void SetBattleLooking(int32 MyMaxMedal, int32 OtherMaxMedal);
	//void SetBattleResult(int32 Win)

private:
	TWeakObjectPtr<class UB2UIGuildMark> UIP_MyMark;
	TWeakObjectPtr<UB2RichTextBlock> TB_MyGuildName;
	TWeakObjectPtr<UProgressBar> PB_MyMedal;
	TWeakObjectPtr<UTextBlock> TB_MyMedal;
	TWeakObjectPtr<class UB2UIGuildMark> UIP_OtherMark;
	TWeakObjectPtr<UB2RichTextBlock> TB_OtherGuildName;
	TWeakObjectPtr<UProgressBar> PB_OtherMedal;
	TWeakObjectPtr<UTextBlock> TB_OtherMedal;
	TWeakObjectPtr<UB2RichTextBlock> TB_Time;
	TWeakObjectPtr<UB2Image> IMG_Win;
	TWeakObjectPtr<UB2Image> IMG_Lose;
	TWeakObjectPtr<UB2Image> IMG_Draw;

	TWeakObjectPtr<UB2RichTextBlock>		RTB_BattleResult;

	TWeakObjectPtr<UTextBlock>	TB_MyMedalPercent;
	TWeakObjectPtr<UTextBlock>	TB_OtherMedalPercent;

	TWeakObjectPtr<UB2RichTextBlock>	RTB_BattleLooking;
};
UCLASS()
class BLADEII_API UB2UIGuildBattleHistoryRecord : public UB2UIWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;

	void SetRecord(int32 Win, int32 Lose, int32 Draw, int32 Rank, int32 TotalRank, int32 Score);
private:
	TWeakObjectPtr<UB2RichTextBlock>		TB_Win;
	TWeakObjectPtr<UB2RichTextBlock>		TB_Lose;
	TWeakObjectPtr<UB2RichTextBlock>		TB_Draw;
	TWeakObjectPtr<UB2RichTextBlock>		TB_Rank;
	TWeakObjectPtr<UB2RichTextBlock>		TB_Score;

};