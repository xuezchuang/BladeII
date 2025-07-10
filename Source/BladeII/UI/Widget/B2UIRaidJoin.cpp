// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIRaidJoin.h"
#include "B2UIDocHelper.h"
#include "B2UIDocRaid.h"
#include "B2UIManager.h"
#include "B2LobbyGameMode.h"
#include "B2UIGameInviteList.h"
#include "B2UIStartButton.h"
#include "B2UIRaidMain.h"
#include "B2UIRepeatBattleCountDown.h"

#include "BladeIIGameImpl.h"

#define UIRAIDDOC_ADD_SLOTDELEGATE(INDEX)																									\
	if (JoinSlotList.IsValidIndex(INDEX) && JoinSlotList[INDEX].IsValid())																	\
	{																																		\
	RaidDoc->OnMySlotIdxChanged.AddUObject(JoinSlotList[INDEX].Get(), &UB2UIRaidJoinSlot::OnChanagedMySlotIndex);							\
	RaidDoc->OnJoinSlotState##INDEX##Changed.AddUObject(JoinSlotList[INDEX].Get(), &UB2UIRaidJoinSlot::OnChanagedSlotState);				\
	RaidDoc->OnJoinSlotPCClass##INDEX##Changed.AddUObject(JoinSlotList[INDEX].Get(), &UB2UIRaidJoinSlot::OnChanagedPCClass);				\
	RaidDoc->OnJoinSlotCombatPoint##INDEX##Changed.AddUObject(JoinSlotList[INDEX].Get(), &UB2UIRaidJoinSlot::OnChanagedCombatPoint);		\
	RaidDoc->OnJoinSlotCharLevel##INDEX##Changed.AddUObject(JoinSlotList[INDEX].Get(), &UB2UIRaidJoinSlot::OnChanagedCharLevel);			\
	RaidDoc->OnJoinSlotCharName##INDEX##Changed.AddUObject(JoinSlotList[INDEX].Get(), &UB2UIRaidJoinSlot::OnChanagedCharName);				\
	RaidDoc->OnJoinSlotIsMyGuildMember##INDEX##Changed.AddUObject(JoinSlotList[INDEX].Get(), &UB2UIRaidJoinSlot::OnChangedMyGuildMember);			\
	}

#define UIRAIDDOC_REMOVE_SLOTDELEGATE(INDEX) \
	if (JoinSlotList.IsValidIndex(INDEX) && JoinSlotList[INDEX].IsValid())							\
	{																								\
		RaidDoc->OnMySlotIdxChanged.RemoveAll(JoinSlotList[INDEX].Get());							\
		RaidDoc->OnJoinSlotState##INDEX##Changed.RemoveAll(JoinSlotList[INDEX].Get());				\
		RaidDoc->OnJoinSlotPCClass##INDEX##Changed.RemoveAll(JoinSlotList[INDEX].Get());			\
		RaidDoc->OnJoinSlotCombatPoint##INDEX##Changed.RemoveAll(JoinSlotList[INDEX].Get());		\
		RaidDoc->OnJoinSlotCharLevel##INDEX##Changed.RemoveAll(JoinSlotList[INDEX].Get());			\
		RaidDoc->OnJoinSlotCharName##INDEX##Changed.RemoveAll(JoinSlotList[INDEX].Get());			\
		RaidDoc->OnJoinSlotIsMyGuildMember##INDEX##Changed.RemoveAll(JoinSlotList[INDEX].Get());			\
	}

void UB2UIRaidJoin::Init()
{
	Super::Init();

	ExitPopupTemp = nullptr;
	StartSetEnableCloseBtn();
}

void UB2UIRaidJoin::OnOpenComplete()
{
	if (auto* UIRaidMain = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMain>(UIFName::Raid))
	{
		// RaidJoin UI 가 오픈되면 방에 입장했으므로 레이드 페이지의 버튼을 활성화 시켜줌.
		UIRaidMain->ButtonFree();
	}
}

void UB2UIRaidJoin::BindDocAuto()
{
	UnbindDoc();

	auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	BII_CHECK(RaidDoc);
	if (!RaidDoc)
		return;

	SetDoc(RaidDoc);

	InitSyncSlotData(RaidDoc);

	UIRAIDDOC_ADD_SLOTDELEGATE(0);
	UIRAIDDOC_ADD_SLOTDELEGATE(1);
	UIRAIDDOC_ADD_SLOTDELEGATE(2);
	UIRAIDDOC_ADD_SLOTDELEGATE(3);
}

void UB2UIRaidJoin::UnbindDoc()
{
	auto* RaidDoc = Cast<UB2UIDocRaid>(GetDoc());
	if (RaidDoc)
	{
		UIRAIDDOC_REMOVE_SLOTDELEGATE(0);
		UIRAIDDOC_REMOVE_SLOTDELEGATE(1);
		UIRAIDDOC_REMOVE_SLOTDELEGATE(2);
		UIRAIDDOC_REMOVE_SLOTDELEGATE(3);
	}	

	SetDoc(nullptr);
}

void UB2UIRaidJoin::DestroySelf(class UB2UIManager* InUIManager)
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::RaidInviteList);

	GetWorld()->GetTimerManager().ClearTimer(EnableCloseBtnHandle);

	if (UIP_BattleCountDown.IsValid())
		UIP_BattleCountDown->DestroySelf(InUIManager);

	if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
		DocRaid->ClearRaidInvitationInfo();

	Super::DestroySelf(InUIManager);
}

void UB2UIRaidJoin::CacheAssets()
{
	Super::CacheAssets();
	
	GET_SLOT(UButton, BTN_Close);
	GET_SLOT(UImage, IMG_InviteBG);
	GET_SLOT(class UB2UIStartButton, UIP_QuickMatching);
	if (UIP_QuickMatching.IsValid())
	{
		UIP_QuickMatching->Init();
		UIP_QuickMatching->StartButtonDelegateBindLambda([this]() {	OnClickQuickMatching(); });
		UIP_QuickMatching->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_QuickMatching")));
	}
	GET_SLOT(class UB2UIStartButton, UIP_QuickStart);
	if (UIP_QuickStart.IsValid())
	{
		UIP_QuickStart->Init();	
		UIP_QuickStart->StartButtonDelegateBindLambda([this]() { OnClickBTN_QuickStart(); });
		UIP_QuickStart->SetStartButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_QuickBattleStart")));
	}

	
	GET_SLOT(UTextBlock, STB_Wait);
	GET_SLOT(UTextBlock, STB_Wait2);
	GET_SLOT(UTextBlock, TB_RaidStep);
	GET_SLOT(UTextBlock, TB_RaidName);

	GET_SLOT(UPanelWidget, P_GuildInfo);
	GET_SLOT(UTextBlock, STB_GuildParty);
	GET_SLOT(UTextBlock, STB_GuildParty2);
	GET_SLOT(UTextBlock, STB_GuildParty3);
	GET_SLOT(UTextBlock, STB_GuildParty4);
	GET_SLOT(UTextBlock, STB_QuickStart);
	GET_SLOT(UB2RichTextBlock, RTB_GuildPartyDesc);

	JoinSlotList.SetNum(4);
	GET_SLOT_BYNAME(UB2UIRaidJoinSlot, JoinSlotList[0], JoinSlot0);
	GET_SLOT_BYNAME(UB2UIRaidJoinSlot, JoinSlotList[1], JoinSlot1);
	GET_SLOT_BYNAME(UB2UIRaidJoinSlot, JoinSlotList[2], JoinSlot2);
	GET_SLOT_BYNAME(UB2UIRaidJoinSlot, JoinSlotList[3], JoinSlot3);
	GET_SLOT(UB2UIRepeatBattleCountDown, UIP_BattleCountDown);

	// Slot Init
	if (UIP_BattleCountDown.IsValid())
	{
		UIP_BattleCountDown->Init();
		UIP_BattleCountDown->SetNotiText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_StartGameComingsoon"))));
	}

	for (int32 i = 0; i < JoinSlotList.Num(); ++i)
	{
		if (JoinSlotList[i].IsValid())
		{
			JoinSlotList[i]->Init();
			JoinSlotList[i]->SetSlotNum(i);
			
			JoinSlotList[i]->FUpdateSlot.BindUObject(this, &UB2UIRaidJoin::UpdateSlotState);
			JoinSlotList[i]->FClickBTNSelect.BindUObject(this, &UB2UIRaidJoin::OnClickSlot);
		}
	}
}

void UB2UIRaidJoin::UpdateStaticText()
{
	if (STB_Wait.IsValid())
		STB_Wait->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_SearchOtherTeam"))));
	if (STB_Wait2.IsValid())
		STB_Wait2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_SearchWait"))));
	if(STB_GuildParty.IsValid())
		STB_GuildParty->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_GuildPartyReward"))));
	if (STB_GuildParty2.IsValid())
		STB_GuildParty2->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_GuildMembersReward")), 2, 1));
	if (STB_GuildParty3.IsValid())
		STB_GuildParty3->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_GuildMembersReward")), 3, 2));
	if (STB_GuildParty4.IsValid())
		STB_GuildParty4->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_GuildMembersReward")), 4, 3));
	if (RTB_GuildPartyDesc.IsValid())
		RTB_GuildPartyDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_GuildRewardGuide"))));
}

void UB2UIRaidJoin::BindDelegates()
{	
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIRaidJoin::OnClickBTN_Close);
}

void UB2UIRaidJoin::InitRoomType(ERaidJoinType RoomType)
{
	RaidJoinType = RoomType;

	// Set SlotState
	if (auto* RaidDoc = Cast<UB2UIDocRaid>(GetDoc()))
	{
		for (int32 i = 0; i < JoinSlotList.Num(); ++i)
		{
			auto JoinSlot = JoinSlotList[i];
			if (JoinSlot.IsValid())
			{
				if (RaidDoc->GetJoinSlotStateByIndex(i) == ERaidJoinSlotState::Max)
					RaidDoc->SetJoinSlotStateByIndex(i, RaidDoc->GetDefaultSlotState(RoomType));

				if (RoomType == ERaidJoinType::MakeRoom)
					JoinSlot->SetIsVisibleBTNCancel(true);

				JoinSlot->SetIsMySlot(i == RaidDoc->GetMySlotIdx());
			}
		}

		if (TB_RaidStep.IsValid())
			TB_RaidStep->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("counterDungeon_Step")), FText::AsNumber(RaidDoc->GetLastRaidStep())));
		if (TB_RaidName.IsValid())
			TB_RaidName->SetText(BladeIIGameImpl::GetClientDataStore().GetRaidName(RaidDoc->GetLastRaidType()));
	}
	
	InitRoomType_BP((int32)RoomType);
}

void UB2UIRaidJoin::ChangeQuickStartRoom()
{
	// 방만들기 모드에서 -> 퀵서치 모드로 변경 
	RaidJoinType = ERaidJoinType::QuickStart;
	InitRoomType_BP((int32)RaidJoinType);

	if (auto* RaidDoc = Cast<UB2UIDocRaid>(GetDoc()))
		RaidDoc->SetOpenedRaidJoinType(ERaidJoinType::QuickStart);

	for (auto JoinSlot : JoinSlotList)
	{
		if (JoinSlot->IsBattleReady() == false)
			JoinSlot->SetSlotState(ERaidJoinSlotState::SearchingQuickJoin);
	}
}

void UB2UIRaidJoin::UpdateSlotState()
{
	UpdateSlotState_BP();
	SetMyGuildMember();
}

void UB2UIRaidJoin::SetVisibilityInviteBG(bool IsEnable)
{
	if (IMG_InviteBG.IsValid())
		IMG_InviteBG->SetVisibility(IsEnable ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIRaidJoin::OnClickSlot(int32 SlotIdx)
{	
	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		if (auto* InviteListUI = UIManager->OpenUI<UB2UIGameInviteList>(UIFName::RaidInviteList))
		{
			InviteListUI->SetInviteUIByGameMode(EB2GameMode::Raid);

			SetVisibilityInviteBG(true);
			InviteListUI->FCloseEvent.BindUObject(this, &UB2UIRaidJoin::SetVisibilityInviteBG);
		}
	}
}

void UB2UIRaidJoin::StartCountDownAnimation(float InCountDownTime)
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::RaidInviteList);
	if (ExitPopupTemp)
		ExitPopupTemp->CloseSelf();

	if (UIP_BattleCountDown.IsValid())
	{
		StartCountDownAnimation_BP();

		UIP_BattleCountDown->StartCountdown(
			FRepeatBattleCountDownCB::CreateLambda([this](){
			if (auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this)))
				LobbyGameMode->StartFinishLobbyMatch();
		}), InCountDownTime);

		// 카운트 시작하면 나가기 버튼 사라지도록.
		if (BTN_Close.IsValid())
			BTN_Close->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIRaidJoin::SetEnableBtnClose(bool IsEnable)
{
	// 레이드 나가기 버튼 비활성화
	if (BTN_Close.IsValid())
		BTN_Close->SetIsEnabled(IsEnable);
}

void UB2UIRaidJoin::SetHost(bool IsHost)
{
	SetHost_BP(IsHost);

	if (UIP_QuickMatching.IsValid())
		UIP_QuickMatching->SetButtonEnabled(IsHost);

	for (auto ThisSlot : JoinSlotList)
		ThisSlot->SetHost_BP(IsHost);

	if (UIP_QuickStart.IsValid())
		UIP_QuickStart->SetButtonEnabled(IsHost);
}

bool UB2UIRaidJoin::IsAllReadyBattle()
{
	for (auto ThisSlot : JoinSlotList)
	{
		if (ThisSlot->IsBattleReady() == false)
			return false;
	}

	return true;
}

void UB2UIRaidJoin::InitSyncSlotData(UB2UIDocBase* InUIRaidDoc)
{
	// RaidDoc : DocData(JoinSlot) => Widget Data
	if (auto* RaidDoc = Cast<UB2UIDocRaid>(InUIRaidDoc))
	{
		InitRoomType(RaidDoc->GetOpenedRaidJoinType());

		for (int32 i = 0; i < JoinSlotList.Num(); ++i)
		{
			if (JoinSlotList[i].IsValid())
			{
				JoinSlotList[i]->SetSlotState(RaidDoc->GetJoinSlotStateByIndex(i));
				JoinSlotList[i]->SetSlotEntryData(RaidDoc->GetJoinSlotCharNameByIndex(i),
					RaidDoc->GetJoinSlotCharLevelByIndex(i),
					RaidDoc->GetJoinSlotPCClassByIndex(i),
					RaidDoc->GetJoinSlotCombatPointByIndex(i),
					RaidDoc->GetJoinSlotIsMyGuildMemberByIndex(i));
			}
		}
	}

	SetMyGuildMember();
}

void UB2UIRaidJoin::LeaveMatchAndCloseUI()
{
	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));		

	if (LobbyGameMode == nullptr || DocRaid == nullptr)
		return;

	LobbyGameMode->LeaveRoomIfJoined();

	DocRaid->ClearPartyRetryInfo();
	DocRaid->SetIsRequestPartyRematch(false);
	DocRaid->SetIsForceStartGame(false);

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIRaidJoin::StartSetEnableCloseBtn()
{
	SetEnableBtnClose(false);

	GetWorld()->GetTimerManager().SetTimer(EnableCloseBtnHandle, this,
		&UB2UIRaidJoin::EnableBtnClose, 1.0f, false);
}

void UB2UIRaidJoin::EnableBtnClose()
{
	SetEnableBtnClose(true);
}

void UB2UIRaidJoin::OnClickQuickMatching()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_SearchOtherUser")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this](){
			ChangePublicRoomType_BP();

			// 룸을 공개방으로 열어준다. (빠른 찾기로 변경)
			if (auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this)))
				LobbyGameMode->ChangeRoomPublicAccess();
		}),
		FMsgPopupOnClick::CreateLambda([this]() {
			if (UIP_QuickMatching.IsValid())
				UIP_QuickMatching->OpenAnimation();
		})
	);
}

void UB2UIRaidJoin::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UIRaidJoin::OnClickBTN_Close()
{
	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode == nullptr)
		return;

	ExitPopupTemp = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_BreakRoom")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]() {
		LeaveMatchAndCloseUI();
	}
	));
}

void UB2UIRaidJoin::OnClickBTN_QuickStart()
{
	if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
		DocRaid->SetForceStartRaid(true);

	if (auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		LobbyGameMode->CheckFullPeerRoom();
	}
}

void UB2UIRaidJoin::SetMyGuildMember()
{
	int32 MyGuildNum = 0;

	if (auto* RaidDoc = Cast<UB2UIDocRaid>(GetDoc()))
	{
		for (int32 i = 0; i < JoinSlotList.Num(); ++i)
		{
			if (RaidDoc->GetJoinSlotIsMyGuildMemberByIndex(i))
				MyGuildNum++;
		}
	}

	SetMyGuildMember_BP(MyGuildNum);
}

#undef UIRAIDDOC_ADD_SLOTDELEGATE
#undef UIRAIDDOC_REMOVE_SLOTDELEGATE