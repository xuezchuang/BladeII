// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2NetGameMode.h"
#include "B2RichTextBlock.h"
#include "TutorialManager.h"
#include "B2UIPVPCharacterSelectPage.h"
#include "B2UIDocHelper.h"
#include "B2UIDocLobbyMatch.h"
#include "B2UIDocRaid.h"
#include "B2UICommonInvite.h"


void UB2UICommonInvite::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_Block);
	GET_SLOT(UCanvasPanel, P_InviteNotify);
	GET_SLOT(UB2RichTextBlock, RTB_InvitationSummary);
	GET_SLOT(UB2Button, BTN_Decline);
	GET_SLOT(UTextBlock, TB_Decline);
	GET_SLOT(UB2Button, BTN_Accept);
	GET_SLOT(UTextBlock, TB_Accept);
	GET_SLOT(UTextBlock, TB_InvitationNotifySeconds);
	GET_SLOT(UCanvasPanel, P_WaitingInvitation);
	GET_SLOT(UTextBlock, TB_StaticWaitSeconds);
	GET_SLOT(UTextBlock, TB_DynamicWaitSeconds);
	GET_SLOT(UB2RichTextBlock, RTB_WaitingNotice);
}

void UB2UICommonInvite::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Decline, &UB2UICommonInvite::OnClickBTN_Decline);
	BIND_CLICK_FUNC_TO_BTN(BTN_Accept, &UB2UICommonInvite::OnClickBTN_Accept);
}

void UB2UICommonInvite::CloseWidgetDelegate()
{
	DeclineInvitation();
	CloseInvitationUI();
}

void UB2UICommonInvite::Init()
{
	Super::Init();

	OwnerGameMode = Cast<AB2MatchGameModeBase>(UGameplayStatics::GetGameMode(this));
	InvitationGameMode = EB2GameMode::Unknown;

	bWaitAccept = false;
	bInDecision = false;

	SetVisibleWaitingAccept(false);
	SetVisibleInviationNotify(false);
}

void UB2UICommonInvite::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}


void UB2UICommonInvite::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bInDecision || bWaitAccept)
	{
		ElapsedSeconds += InDeltaTime;
		if (ElapsedSeconds >= 1.f)
		{
			if (bInDecision)
			{
				NotifyInvitationSeconds -= ElapsedSeconds;
				TimerTick_NotifyInvitation(NotifyInvitationSeconds);
			}
			else if (bWaitAccept)
			{
				WaitingAcceptSeconds -= ElapsedSeconds;
				TimerTick_WaitingAccept(WaitingAcceptSeconds);
			}

			ElapsedSeconds = 0.f;
		}
	}
}

// 초대(발신) 자가 RequestGameInvite 호출 후 GameMode에서 콜
void UB2UICommonInvite::OnRequestGameInvite(const FString& InviteeName, EB2GameMode GameModeType)
{
	// 초대자는 수락 + 캐릭터 선택까지 기다림
	const float TimeoutSeconds = GetInviterWaitingSeconds();

	InvitationGameMode = GameModeType;

	ShowWaitingAccept(InviteeName, TimeoutSeconds);
	StartTimer_WaitingAccept(TimeoutSeconds);

	if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		DocLobbyMatch->SetInvitationBeginTime(GWorld->GetTimeSeconds());
}

// 초대(수신) 자가 RequestGameInvite 받았을 때 GameMode에서 콜
void UB2UICommonInvite::OnReceiveInvitation(const FString& InviterName, EB2GameMode GameModeType)
{
	const float TimeoutSeconds = GetAcceptTimeoutSeconds(GameModeType);
	
	SetPageBlock(false);

	InvitationGameMode = GameModeType;

	ShowInvitationNotify(InviterName, TimeoutSeconds);
	StartTimer_NotifyInvitation(TimeoutSeconds);

	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
			DocLobbyMatch->SetInvitationBeginTime(GWorld->GetTimeSeconds());
	}

	else if (GameModeType == EB2GameMode::Raid)
	{

	}
}


void UB2UICommonInvite::OnReceiveAcceptInvitation(bool bMyInvitation)
{
	if (bMyInvitation)
	{
		bWaitAccept = false;

		// "상대방이 초대를 수락 했습니다 " 메시지

		FText AcceptInvitationMsg = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Invitation_InvitationAccepted"));
		ABladeIIGameMode::SetHUDCenterMessage(AcceptInvitationMsg, 1.0f);
	}


	CloseInvitationUI();
}

void UB2UICommonInvite::OnReceiveDecline(const FString& InviteeName, bool LocalIsInvitee/* = false*/)
{
	if(LocalIsInvitee)
		CloseInvitationUI();
	else
		OnInvitationDeclined();

	SetPageBlock(false);
}

void UB2UICommonInvite::ShowInvitationNotify(const FString& InviterName, int32 TimeOutSeconds)
{
	//SetVisibleInviationNotify(true);
	//SetPageBlock(true);

	//// Dynamics..

	//// %s 님이 일대일 친선전(레이드)에 초대하였습니다...
	//const FText& Summary = GetSummaryTextByGameMode(InvitationGameMode, InviterName);
	//if (RTB_InvitationSummary.IsValid())
	//	RTB_InvitationSummary->SetText(Summary);

	//const int32 LeftSeconds = FMath::FloorToInt(TimeOutSeconds);
	//SetLeftSeconds_InvitationNotify(LeftSeconds);
}


void UB2UICommonInvite::ShowWaitingAccept(const FString& InviteeName, int32 TimeOutSeconds)
{
	SetVisibleWaitingAccept(true);
	SetPageBlock(true);

	// Statics..
	if (TB_StaticWaitSeconds.IsValid())
		TB_StaticWaitSeconds->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Invitation_WaitingAccept")));

	// Dynamics..

	// %s 님이 친선전 도전을 받아들이기를 기다리는 중입니다...
	const FText Summary = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Invitation_WaitingAcceptFriendshipGame")), FText::FromString(InviteeName));
	if (RTB_WaitingNotice.IsValid())
		RTB_WaitingNotice->SetText(Summary);

	SetLeftSeconds_WaitingAccept(TimeOutSeconds);
}

void UB2UICommonInvite::AcceptInvitation()
{
	OwnerGameMode->RequestAcceptInvitation(InvitationGameMode);
}

void UB2UICommonInvite::DeclineInvitation()
{
	OwnerGameMode->RequestDeclineInvitation(InvitationGameMode);
	CloseInvitationUI();
}

void UB2UICommonInvite::SetPageBlock(bool bBlock)
{
	if (BTN_Block.IsValid())
		BTN_Block->SetVisibility(bBlock ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}


void UB2UICommonInvite::OnInvitationDeclined()
{
	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		UIManager->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Invitation_DeclineInvitation")),
			0,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
		);
	}

	CloseInvitationUI();
}


void UB2UICommonInvite::CloseInvitationUI()
{
	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
		UIManager->CloseUI(UIFName::InviteGame);
}


float UB2UICommonInvite::GetAcceptTimeoutSeconds(EB2GameMode GameModeType)
{
	if (GameModeType == EB2GameMode::PVP_Tag)
	{
		if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
			return DocLobbyMatch->AutoDeclineSeconds;
	}
	else if (GameModeType == EB2GameMode::Raid)
	{
		if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
		{
			FTimespan RemainTime = FDateTime::UtcNow() - DocRaid->GetRaidInvitationInfo().InviteTime;
			const int32 DefaultTime = 20.f;
			const int32 LeftTime = DefaultTime - RemainTime.GetTotalSeconds();

			return static_cast<float>(LeftTime);
		}
	}

	return 0.f;
}


float UB2UICommonInvite::GetCharSelectTimeoutSeconds(float AcceptElapsedSeconds)
{
	if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		return DocLobbyMatch->InviterTimeout - AcceptElapsedSeconds;

	return 0.f;
}

float UB2UICommonInvite::GetInviterWaitingSeconds()
{
	if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
		return DocLobbyMatch->InviterTimeout;

	return 0.f;
}


FText UB2UICommonInvite::GetSummaryTextByGameMode(EB2GameMode GameModeType, const FString& InviterName)
{
	switch (GameModeType)
	{
	case EB2GameMode::Raid : 
		{
			auto* DocRaid = UB2UIDocHelper::GetDocRaid();
			const auto& RaidInviteInfo = DocRaid->GetRaidInvitationInfo();

			FText RaidName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_Name") + FString::FromInt(RaidInviteInfo.RaidType)));
			FText Difficulty = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("counterDungeon_Step")), FText::AsNumber((int32)RaidInviteInfo.RaidStep));

			return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_NotificationInvitieMessage")), FText::FromString(InviterName), RaidName, Difficulty);
		}
	case EB2GameMode::PVP_Tag : 
		
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Invitation_NotifyFriendshipGame")), FText::FromString(InviterName));
	}
	return FText::GetEmpty();
}

void UB2UICommonInvite::SetVisibleWaitingAccept(bool bShow)
{
	if (P_WaitingInvitation.IsValid())
		P_WaitingInvitation->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}


void UB2UICommonInvite::SetVisibleInviationNotify(bool bShow)
{
	if (P_InviteNotify.IsValid())
		P_InviteNotify->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}


void UB2UICommonInvite::SetLeftSeconds_WaitingAccept(float Seconds)
{
	const int32 DisplaySeconds = FMath::CeilToInt(Seconds);
	if (TB_DynamicWaitSeconds.IsValid())
		TB_DynamicWaitSeconds->SetText(FText::AsNumber(DisplaySeconds));
}

void UB2UICommonInvite::SetLeftSeconds_InvitationNotify(float Seconds)
{
	const int32 DisplaySeconds = FMath::CeilToInt(Seconds);
	if (TB_InvitationNotifySeconds.IsValid())
		TB_InvitationNotifySeconds->SetText(FText::AsNumber(DisplaySeconds));
}


void UB2UICommonInvite::StartTimer_WaitingAccept(float Seconds)
{
	bWaitAccept = true;
	WaitingAcceptSeconds = Seconds;
}

void UB2UICommonInvite::StartTimer_NotifyInvitation(float Seconds)
{
	bInDecision = true;
	NotifyInvitationSeconds = Seconds;
}

void UB2UICommonInvite::TimerTick_WaitingAccept(float LeftSeconds)
{
	if (LeftSeconds <= 0.f)
	{
		CloseInvitationUI();
	}

	SetLeftSeconds_WaitingAccept(LeftSeconds);
}


void UB2UICommonInvite::TimerTick_NotifyInvitation(float LeftSeconds)
{
	if (LeftSeconds <= 0.f)
	{
		DeclineInvitation();
		CloseInvitationUI();
	}

	SetLeftSeconds_InvitationNotify(LeftSeconds);
}

void UB2UICommonInvite::OnClickBTN_Decline()
{
	bInDecision = false;

	DeclineInvitation();
	SetVisibleInviationNotify(false);
}

void UB2UICommonInvite::OnClickBTN_Accept()
{
	bInDecision = false;

	if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_PvPOneAndOne))
	{
		if (OwnerGameMode)
			OwnerGameMode->PrevAcceptGameInvite(Inviter, InvitationGameMode);
	}
	else
	{
		DeclineInvitation();
		TutorialManager::OpenLockContentPopup(TutorialID_PvPOneAndOne, 1);
	}

	CloseInvitationUI();
}


