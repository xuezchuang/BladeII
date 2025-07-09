// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIDocRaid.h"
#include "B2UIRaidJoinSlot.h"
#include "B2UIBackWidget.h"
#include "B2UIMsgPopupSimple.h"
#include "B2UIRaidJoin.generated.h"

/*
* 
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRaidJoin : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnOpenComplete() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	void OnClickSlot(int32 SlotIdx);

	// event
public:
	UFUNCTION()
	void OnClickBTN_Close();

	UFUNCTION()
	void OnClickBTN_QuickStart();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void InitRoomType_BP(int32 roomType);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void StartCountDownAnimation_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void StopCountDownAnimation_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void UpdateSlotState_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void SetHost_BP(bool IsHost);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void ChangePublicRoomType_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Raid")
	void SetMyGuildMember_BP(int32 Count);
public:
	void InitRoomType(ERaidJoinType RoomType);
	void ChangeQuickStartRoom();
	void UpdateSlotState();
	void SetVisibilityInviteBG(bool IsEnable);
	void StartCountDownAnimation(float InCountDownTime);
	void SetEnableBtnClose(bool IsEnableBTNClose);
	void SetHost(bool IsHost);

	void SetMyGuildMember();
private:
	bool IsAllReadyBattle();
	void InitSyncSlotData(UB2UIDocBase* UIRaidDoc);
	void LeaveMatchAndCloseUI();
	void StartSetEnableCloseBtn();
	void EnableBtnClose();
	void OnClickQuickMatching();

protected:
	TWeakObjectPtr<UButton>							BTN_Close;
	TWeakObjectPtr<UImage>							IMG_InviteBG;
	TArray<TWeakObjectPtr<class UB2UIRaidJoinSlot>>	JoinSlotList;
	TWeakObjectPtr<class UB2UIRepeatBattleCountDown> UIP_BattleCountDown;
	TWeakObjectPtr<class UB2UIStartButton>			UIP_QuickMatching;
	TWeakObjectPtr<class UB2UIStartButton>			UIP_QuickStart;

	TWeakObjectPtr<UTextBlock>						STB_Wait;
	TWeakObjectPtr<UTextBlock>						STB_Wait2;
	TWeakObjectPtr<UTextBlock>						TB_RaidStep;
	TWeakObjectPtr<UTextBlock>						TB_RaidName;
	TWeakObjectPtr<UTextBlock>						STB_QuickStart;

	TWeakObjectPtr<UPanelWidget>					P_GuildInfo;
	TWeakObjectPtr<UTextBlock>						STB_GuildParty;
	TWeakObjectPtr<UTextBlock>						STB_GuildParty2;
	TWeakObjectPtr<UTextBlock>						STB_GuildParty3;
	TWeakObjectPtr<UTextBlock>						STB_GuildParty4;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_GuildPartyDesc;

private:

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Raid")
	int32	StartCountDownNum;

private:
	ERaidJoinType	RaidJoinType;
	FTimerHandle	EnableCloseBtnHandle;
	UB2UIMsgPopupSimple* ExitPopupTemp;
};
