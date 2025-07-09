// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UICommonInvite.generated.h"


UCLASS()
class BLADEII_API UB2UICommonInvite : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()


protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	void OnRequestGameInvite(const FString& InviteeName, EB2GameMode GameModeType);

	void OnReceiveInvitation(const FString& InviterName, EB2GameMode GameModeType);
	void OnReceiveAcceptInvitation(bool bMyInvitation);
	void OnReceiveDecline(const FString& InviteeName, bool LocalIsInvitee = false);

	void ShowInvitationNotify(const FString& InviterName, int32 TimeOutSeconds);
	void ShowWaitingAccept(const FString& InviteeName, int32 TimeOutSeconds);

	void AcceptInvitation();
	void DeclineInvitation();

	void SetPageBlock(bool bBlock);

	// 초대한 쪽에서 상대방 거절 시 - (초대에 응답 없거나 명시적 거절 시)
	void OnInvitationDeclined();

	void CloseInvitationUI();

	float GetAcceptTimeoutSeconds(EB2GameMode GameModeType);
	float GetCharSelectTimeoutSeconds(float AcceptElapsedSeconds);
	float GetInviterWaitingSeconds();

	FText GetSummaryTextByGameMode(EB2GameMode GameModeType, const FString& Inviter);

protected:
	
	void SetVisibleWaitingAccept(bool bShow);
	void SetVisibleInviationNotify(bool bShow);

	void SetLeftSeconds_WaitingAccept(float Seconds);
	void SetLeftSeconds_InvitationNotify(float Seconds);

	void StartTimer_WaitingAccept(float Seconds);
	void StartTimer_NotifyInvitation(float Seconds);

	void StopTimer(FTimerHandle& StopHandle);

	void TimerTick_WaitingAccept(float LeftSeconds);
	void TimerTick_NotifyInvitation(float LeftSeconds);

	UFUNCTION()
	void OnClickBTN_Decline();
	UFUNCTION()
	void OnClickBTN_Accept();

private:

	TWeakObjectPtr<UB2Button> BTN_Block;
	TWeakObjectPtr<UCanvasPanel> P_InviteNotify;
	TWeakObjectPtr<class UB2RichTextBlock> RTB_InvitationSummary;
	TWeakObjectPtr<UB2Button> BTN_Decline;
	TWeakObjectPtr<UTextBlock> TB_Decline;
	TWeakObjectPtr<UB2Button> BTN_Accept;
	TWeakObjectPtr<UTextBlock> TB_Accept;
	TWeakObjectPtr<UTextBlock> TB_InvitationNotifySeconds;
	TWeakObjectPtr<UCanvasPanel> P_WaitingInvitation;
	TWeakObjectPtr<UTextBlock> TB_StaticWaitSeconds;
	TWeakObjectPtr<UTextBlock> TB_DynamicWaitSeconds;
	TWeakObjectPtr<class UB2RichTextBlock> RTB_WaitingNotice;

	FString			Inviter;

	float	WaitingAcceptSeconds;
	float	NotifyInvitationSeconds;

	bool	bInDecision;
	bool	bWaitAccept;

	float	InvitationBeginTime;
	float	ElapsedSeconds;

	UPROPERTY(Transient)
	class AB2MatchGameModeBase* OwnerGameMode;

	EB2GameMode InvitationGameMode;
};
