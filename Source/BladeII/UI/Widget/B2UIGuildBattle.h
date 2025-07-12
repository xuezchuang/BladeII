// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIGuildBattle.generated.h"


UCLASS()
class BLADEII_API UB2UIGuildBattle : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	UB2UIGuildBattle(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	void SetTitleSetting();
	void SetBattleTime(int64 BattleTime);
	void RefreshGuildTeam(ETeamIndex InTeamIndex, uint8 InTeamNum);

	virtual void OnOpen(bool RightNow = false) final;

protected:
	UFUNCTION()
	void OnClickBTN_ChangeCam();

	UFUNCTION()
	void OnClickPauseButton();

	UFUNCTION()
	void OnChangeGuildTeam(ETeamIndex InTeamIndex, uint8 InTeamNum);

	UFUNCTION()
	void OnChangePlayerGuildTeamDisplay(UB2UIDocBase* Sender, uint8 NewValue, uint8 PreValue);
	UFUNCTION()
	void OnChangeEnemyGuildTeamDisplay(UB2UIDocBase* Sender, uint8 NewValue, uint8 PreValue);
	
	UFUNCTION()
	void OnChangeBattleTime(UB2UIDocBase* Sender, int32 NewValue, int32 PreValue);

private:
	TWeakObjectPtr<UB2Button> BTN_ChangeCam;
	TWeakObjectPtr<UCanvasPanel> CP_PVPFightMsgSet;
	TWeakObjectPtr<UImage> IMG_Fight_01;
	TWeakObjectPtr<UImage> IMG_Fight_02;


	TWeakObjectPtr<class UB2UIGuildMark> UIP_MyGuildMark;
	TWeakObjectPtr<class UB2UIGuildMark> UIP_OtherGuildMark;
	TWeakObjectPtr<UB2RichTextBlock> TB_MyGuildName;
	TWeakObjectPtr<UB2RichTextBlock> TB_MyNickName;
	TWeakObjectPtr<UB2RichTextBlock> TB_OtherGuildName;
	TWeakObjectPtr<UB2RichTextBlock> TB_OtherNickName;
	TWeakObjectPtr<UTextBlock>		 TB_BattleTime;

	TWeakObjectPtr<UB2Button>		 BTN_Pause;

	TArray<TWeakObjectPtr<class UB2UIGuildNPCSkill>>		UIP_GuildNPCSkills;

	TArray<TWeakObjectPtr<class UB2UIGuildMatchBattleSlot>>	LeftSlots;
	TArray<TWeakObjectPtr<class UB2UIGuildMatchBattleSlot>>	RightSlots;

	int8 LeftSlotCurrentTeamNum;
	int8 RightSlotCurrentTeamNum;

	TArray<TWeakObjectPtr<class UB2UIGuildTeam>> LeftGuildTeams;
	TArray<TWeakObjectPtr<class UB2UIGuildTeam>> RightGuildTeams;
};
