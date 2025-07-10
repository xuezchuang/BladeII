// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIGameInviteList.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocRaid.h"
#include "Retailer.h"
#include "B2LobbyGameMode.h"
#include "B2UIGameInviteSlot.h"

#include "B2UICommonTab.h"
#include "B2UIDocPVP1on1Rival.h"
#include "BladeIIGameImpl.h"

void UB2UIGameInviteList::Init()
{
	SubscribeEvent();
	Super::Init();
	InvitingSlotDatas.Empty();
	SendPointTime = 0;
	GetWorld()->GetTimerManager().ClearTimer(RefeshPointTimer);

	bool bHasGuild = BladeIIGameImpl::GetClientDataStore().GetUserGuildID() != 0;
	if (TAB_Friend.IsValid())
		TAB_Friend->ForcedClickTab();
	if (TAB_Guild.IsValid())
		TAB_Guild->SetIsEnabled(bHasGuild);


	auto* DocPVP = UB2UIDocHelper::GetDocPVP1on1Rival();
	bool bShowSearch = DocPVP->GetIsFriendshipGame();
	SetVisibleSearchTab(bShowSearch);

	GameModeType = EB2GameMode::Unknown;
}

void UB2UIGameInviteList::BindDocAuto()
{
	UnbindDoc();

	auto* RaidDoc = UB2UIDocHelper::GetDocRaid();
	BII_CHECK(RaidDoc);
	if (!RaidDoc)
		return;

	RaidDoc->SetOpenedInviteUIAndChangeJoinSlotState(true);

	SetDoc(RaidDoc);

}

void UB2UIGameInviteList::UnbindDoc()
{
	auto* RaidDoc = Cast<UB2UIDocRaid>(GetDoc());
	if (RaidDoc)
	{
	}

	SetDoc(nullptr);
}

void UB2UIGameInviteList::DestroySelf(class UB2UIManager* InUIManager)
{
	FCloseEvent.ExecuteIfBound(false);

	UnSubscribeEvent();

	// 스크롤박스 비워주자
	for (int32 i = 0; i < InviteSlots.Num(); ++i)
	{
		if (InviteSlots[i].IsValid())
			InviteSlots[i]->DestroySelf(NULL);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIGameInviteList::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_CloseArea);
	GET_SLOT(UB2Button, BTN_Refresh);
	GET_SLOT(UTextBlock, TB_RefreshTime);
	
	GET_SLOT(UTextBlock, STB_Title);

	GET_SLOT(UB2ScrollBox, SB_SlotList);

	GET_SLOT(UB2Button, BTN_Block);

	GET_SLOT(UOverlay, O_UserSearch);
	GET_SLOT(UB2Button, BTN_UserSearch);
	GET_SLOT(UEditableTextBox, TIB_SearchName);

	GET_SLOT(UB2UICommonTab, TAB_Friend);
	GET_SLOT(UB2UICommonTab, TAB_Guild);
	GET_SLOT(UB2UICommonTab, TAB_Search);

	InviteSlots.AddUninitialized(6);
	GET_SLOT_BYNAME(UB2UIGameInviteSlot, InviteSlots[0], UIP_Friend1);
	GET_SLOT_BYNAME(UB2UIGameInviteSlot, InviteSlots[1], UIP_Friend2);
	GET_SLOT_BYNAME(UB2UIGameInviteSlot, InviteSlots[2], UIP_Friend3);
	GET_SLOT_BYNAME(UB2UIGameInviteSlot, InviteSlots[3], UIP_Friend4);
	GET_SLOT_BYNAME(UB2UIGameInviteSlot, InviteSlots[4], UIP_Friend5);
	GET_SLOT_BYNAME(UB2UIGameInviteSlot, InviteSlots[5], UIP_Friend6);

	for (int32 i = 0; i < InviteSlots.Num(); ++i)
	{
		auto InviteSlot = InviteSlots[i];
		if (InviteSlot.IsValid())
		{
			InviteSlot->Init();
			InviteSlot->FClickBTNInvite.BindUObject(this, &UB2UIGameInviteList::StartInviteUser);
			InviteSlot->FStopInviteTimer.BindUObject(this, &UB2UIGameInviteList::StopInviteTimer);
		}
	}

	SetDisibleSlotList();
}

void UB2UIGameInviteList::UpdateStaticText()
{
	if (STB_Title.IsValid())
		STB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("General_InviteList"))));
}

void UB2UIGameInviteList::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGameInviteList::OnClick_BTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_CloseArea, &UB2UIGameInviteList::OnClick_BTN_CloseArea);
	BIND_CLICK_FUNC_TO_BTN(BTN_Refresh, &UB2UIGameInviteList::OnClick_BTN_Refresh);

	BIND_CLICK_FUNC_TO_BTN(BTN_UserSearch, &UB2UIGameInviteList::OnClick_BTN_UserSearch);

	REGISTER_UITAB_BINDING(TAB_Friend.Get(), &UB2UIGameInviteList::OnClickBtnFriend);
	REGISTER_UITAB_BINDING(TAB_Guild.Get(), &UB2UIGameInviteList::OnClickBtnGuild);
	REGISTER_UITAB_BINDING(TAB_Search.Get(), &UB2UIGameInviteList::OnClickBtnUserSearchTab);
}

void UB2UIGameInviteList::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UIGameInviteList);

	DeliveryGetFriendListTicket = DeliveryGetFriendListClass<FB2FriendList>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetFriendList, const FB2FriendList& FriendList)
		Capture->OnDeliveryGetFriendList(FriendList);
		END_CAPTURE_OBJECT()
		);

	DeliveryGetGuildMemberTicket = DeliveryGetGuildMemberClass<FB2ResponseGuildMemberPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetGuildMember, const FB2ResponseGuildMemberPtr& GuildMemberList)
		Capture->OnDeliveryGetGuildMemberList(GuildMemberList);
	END_CAPTURE_OBJECT()
		);


	DeliveryFindUserTicket = DeliveryFindUserClass<FB2FindUser>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryFindUser, const FB2FindUser& FindUser)
		Capture->OnDeliveryFindUser(FindUser);
	END_CAPTURE_OBJECT()
		);

}

void UB2UIGameInviteList::UnSubscribeEvent()
{
	DeliveryGetFriendListClass<FB2FriendList>::GetInstance().Unsubscribe(DeliveryGetFriendListTicket);
	DeliveryGetGuildMemberClass<FB2ResponseGuildMemberPtr>::GetInstance().Unsubscribe(DeliveryGetGuildMemberTicket);
	DeliveryFindUserClass<FB2FindUser>::GetInstance().Unsubscribe(DeliveryFindUserTicket);
}

void UB2UIGameInviteList::StartRefreshTimer(EGameInviteTab InviteTap)
{
	RequestUserList(SelectedTabType);

	SendPointTime = FDateTime::UtcNow().ToUnixTimestamp();

	ProcRefreshTime();
}

void UB2UIGameInviteList::ResetSlotList(bool IsDeleteSlot /*= true*/, int32 RemainSlotCountByDelete /*= 10*/)
{
	SetDisibleSlotList();

	if (IsDeleteSlot)
		DeleteSlotList(RemainSlotCountByDelete);

	if (SB_SlotList.IsValid())
	{
		SB_SlotList->SetAnimateScroll(false);
		SB_SlotList->SetScrollOffset(0.0f);
	}
}

void UB2UIGameInviteList::DeleteSlotList(int32 InRemainCount /*= 0*/)
{
	int32 RemainCount = InRemainCount;
	for (auto& InviteSlot : InviteSlots)
	{
		if (RemainCount != 0)
		{
			--RemainCount;
			continue;
		}

		if (InviteSlot.IsValid())
			InviteSlot->DestroySelf(NULL);
	}
	
	InviteSlots.SetNumUnsafeInternal(FMath::Clamp(InRemainCount, 0, InviteSlots.Num()));
}

void UB2UIGameInviteList::SetDisibleSlotList(int32 RemainCount /*= 0*/)
{
	for (auto InviteSlot : InviteSlots)
	{
		if (InviteSlot.IsValid())
			InviteSlot->SetVisibility((RemainCount > 0) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		if (RemainCount > 0)
			--RemainCount;
	}
}

void UB2UIGameInviteList::SetDisibleSlot(const FText& CharName)
{
	for (auto InviteSlot : InviteSlots)
	{
		if (InviteSlot.IsValid())
		{
			const FText NickName = InviteSlot->GetNickName();
			if (NickName.CompareTo(CharName) == 0)
			{
				InviteSlot->SetVisibility(ESlateVisibility::Hidden);
				break;
			}
		}
	}
}

void UB2UIGameInviteList::RemoveSlot(const FText& CharName)
{
	if (SB_SlotList.IsValid() == false)
		return;

	for (auto InviteSlot : InviteSlots)
	{
		if (InviteSlot.IsValid())
		{
			const FText NickName = InviteSlot->GetNickName();
			if (NickName.CompareTo(CharName) == 0)
			{
				SB_SlotList->RemoveChild(InviteSlot.Get());
				break;
			}
		}
	}
}

void UB2UIGameInviteList::CloseInviteListUI()
{
	SetRaidJoinSlotStateByClose();

	UB2UIManager::GetInstance()->CloseUI(UIFName::RaidInviteList);
}

void UB2UIGameInviteList::SetUIState(EGameInviteTab InviteState)
{
	SelectedTabType = InviteState;

	ResetSlotList(false);
	RequestUserList(InviteState);

	const bool bShowSearchWidget = InviteState == EGameInviteTab::Search ? true : false;
	UpdateVisibleSearchWidget(bShowSearchWidget);
}

void UB2UIGameInviteList::CreateInviteSlots(TArray<FB2FriendPtr>& FriendList)
{
	int32 InsertSlotIndex = 0;
	for (int32 i = 0; i < FriendList.Num(); ++i)
	{
		auto CharSummaryPtr = FriendList[i]->user->character_summary;
		InsertSlotIndex = CreateInviteSlotCommon(InsertSlotIndex, CharSummaryPtr, FriendList[i]->raid_penalty_remove_time);
	}

	// 사용하지 않는 Slot은 안보이게 처리
	SetDisibleSlotList(InsertSlotIndex);
}

void UB2UIGameInviteList::CreateInviteSlots(TArray<FB2GuildMemberPtr> CharList)
{
	int32 InsertSlotIndex = 0;
	for (int32 i = 0; i < CharList.Num(); ++i)
	{
		auto CharSummaryPtr = CharList[i]->character_summary;
		InsertSlotIndex = CreateInviteSlotCommon(InsertSlotIndex, CharSummaryPtr, CharList[i]->raid_penalty_remove_time);
	}

	// 사용하지 않는 Slot은 안보이게 처리
	SetDisibleSlotList(InsertSlotIndex);
}

TWeakObjectPtr<class UB2UIGameInviteSlot> UB2UIGameInviteList::CreateOrGetSlot(int32 SlotIndex)
{
	if (InviteSlots.IsValidIndex(SlotIndex))
		return InviteSlots[SlotIndex];

	if (UB2UIGameInviteSlot* CreatedWidget = CreateWidget<UB2UIGameInviteSlot>(GetOwningPlayer(), GameInviteSlotClass))
	{
		BII_CHECK(CreatedWidget);

		CreatedWidget->Init();
		CreatedWidget->FClickBTNInvite.BindUObject(this, &UB2UIGameInviteList::StartInviteUser);
		SB_SlotList->AddChild(CreatedWidget);

		TWeakObjectPtr<class UB2UIGameInviteSlot> CastedWidget(CreatedWidget);
		InviteSlots.Add(CastedWidget);

		return CastedWidget;
	}

	return nullptr;
}

bool UB2UIGameInviteList::CanUserJoin(const FString& AccountName)
{
	if (auto* RaidDoc = Cast<UB2UIDocRaid>(GetDoc()))
	{
		// 이미 파티 참석이 완료된 유저는 패스
		if (RaidDoc->IsJoinedUser(FText::FromString(AccountName)))
			return false;
	}
	
	return true;
}

void UB2UIGameInviteList::SetVisibleSearchTab(bool bVisible)
{
	ESlateVisibility TabVisibility = bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	if (TAB_Search.IsValid())
		TAB_Search->SetVisibility(TabVisibility);
}

void UB2UIGameInviteList::UpdateVisibleSearchWidget(bool bVisible)
{
	ESlateVisibility WidgetVisibility = bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	if (O_UserSearch.IsValid())
		O_UserSearch->SetVisibility(WidgetVisibility);
}

void UB2UIGameInviteList::RequestFindUser(const FString& NickName)
{
	if (NickName.Len() < 2 || NickName.Len() > 8)
	{
		UB2UIManager* pUIManager = UB2UIManager::GetInstance();
		pUIManager->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_NickNameExplanation")),
			0,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
		);

		return;
	}

	data_trader::Retailer::GetInstance().RequestFindUser(NickName);
}

void UB2UIGameInviteList::SetRaidJoinSlotStateByClose()
{
	if (auto* RaidDoc = Cast<UB2UIDocRaid>(GetDoc()))
		RaidDoc->SetOpenedInviteUIAndChangeJoinSlotState(false);
}

void UB2UIGameInviteList::RequestUserList(EGameInviteTab InviteTap)
{
	switch (InviteTap)
	{
	case EGameInviteTab::Friend:
		data_trader::Retailer::GetInstance().RequestGetFriendList();
		break;
	case EGameInviteTab::Guild:
		data_trader::Retailer::GetInstance().RequestGetGuildMember();
		break;
	}
}



int32 UB2UIGameInviteList::CreateInviteSlotCommon(int32 SlotIndex, FB2CharacterSummaryPtr CharSummaryPtr, int64 InPenaltyTime)
{
	if (CharSummaryPtr == nullptr)
		return SlotIndex;

	const FString& AccountName = CharSummaryPtr->account_name;
	const FString& MyNickName = BladeIIGameImpl::GetLocalCharacterData().GetUserNickName();

	// 이미 파티 참석이 완료된 유저는 패스
	if (CanUserJoin(AccountName) == false || AccountName == MyNickName)
		return SlotIndex;

	// 기존에 사용된 위젯이 있다면 재활용, 없으면 생성
	auto InviteSlotWidget = CreateOrGetSlot(SlotIndex);

	BII_CHECK(InviteSlotWidget.IsValid());
	if (InviteSlotWidget.IsValid())
	{
		bool IsAlreadyInviteUser = false;
		uint8 InviteTime_Second = 0;
		FName PCName = FName(*AccountName);

		if (InvitingSlotDatas.Contains(PCName))
		{
			int64 InvitedStartTime = InvitingSlotDatas[PCName];
			FTimespan timeElapse = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(InvitedStartTime);
			FTimespan timeRemain = FTimespan::FromSeconds(20.5f) - timeElapse;
			IsAlreadyInviteUser = (timeRemain > 1);
			InviteTime_Second = timeRemain.GetSeconds();
		}

		FDateTime PaneltyTime = FDateTime::FromUnixTimestamp(InPenaltyTime / 1000);
		FDateTime TimeNow = FDateTime::UtcNow();
		FTimespan RemainTime = PaneltyTime - TimeNow;
		bool bIsPenalty = (RemainTime > 0);

		InviteSlotWidget->SetSlotEntryData(CharSummaryPtr, IsAlreadyInviteUser ? InviteTime_Second : -1, bIsPenalty);
		InviteSlotWidget->SetSlotIndex(SlotIndex);

		SlotIndex++;
	}

	return SlotIndex;
}

void UB2UIGameInviteList::ProcRefreshTime()
{
	FTimespan timeElapse = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(SendPointTime);
	FTimespan timeRemain = FTimespan::FromSeconds(5.5f) - timeElapse;
	bool IsRefreshRemainTime = (timeRemain > 1);

	// Widget-Visibility는 BluePrint에서 처리
	SetRefreshTimer_BP(IsRefreshRemainTime);

	// 새로고침 시간이 남아있다면 재귀호출
	if (IsRefreshRemainTime)
	{
		if (TB_RefreshTime.IsValid())
			TB_RefreshTime->SetText(FText::FromString(FString::Printf(TEXT("%d"), timeRemain.GetSeconds())));

		GetWorld()->GetTimerManager().SetTimer(RefeshPointTimer, this, &UB2UIGameInviteList::ProcRefreshTime, 1.0f, false);
	}
}

void UB2UIGameInviteList::StartInviteUser(const FText& SelectCharName, int32 CheckSlotIndex)
{
	if (GameModeType == EB2GameMode::Raid)
		StartInvite_Raid(SelectCharName.ToString(), CheckSlotIndex);

	else if(GameModeType == EB2GameMode::PVP_Tag)
		StartInvite_FriendlyPVP(SelectCharName.ToString(), CheckSlotIndex);

}

void UB2UIGameInviteList::StopInviteTimer(const FText& CharName)
{	
	if (CharName.IsEmpty())
		return;

	InvitingSlotDatas.Remove(FName(*(CharName.ToString())));
}

void UB2UIGameInviteList::SetJoinEvent(const FText& CharName)
{
	InvitingSlotDatas.Remove(FName(*(CharName.ToString())));
	StartRefreshTimer(SelectedTabType);
}

void UB2UIGameInviteList::SetInviteUIByGameMode(EB2GameMode ModeType)
{
	GameModeType = ModeType;

	const bool IsFriendshipPVP = GameModeType == EB2GameMode::PVP_Tag;
	SetVisibleSearchTab(IsFriendshipPVP);

	if (IsFriendshipPVP && BTN_Block.IsValid())
		BTN_Block->SetVisibility(ESlateVisibility::Visible);
}

bool UB2UIGameInviteList::StartInvite_Raid(const FString& NickName, int32 SlotIndex)
{
	auto* RaidDoc = Cast<UB2UIDocRaid>(GetDoc());
	if (RaidDoc && NickName.IsEmpty() == false)
	{
		const int32 NowInvitingUserCount = InvitingSlotDatas.Num();
		const int32 InvitedUserCount = RaidDoc->GetInvitedUserNum();

		if (NowInvitingUserCount + InvitedUserCount >= UB2UIGameInviteList::MAX_INVITE_SLOT)
			return false;

		InvitingSlotDatas.Emplace(FName(*NickName), FDateTime::UtcNow().ToUnixTimestamp());
		if (InviteSlots.IsValidIndex(SlotIndex))
		{
			InviteSlots[SlotIndex]->SetSlotState(EGameInviteSlotIndex::Inviting);

			if (auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this)))
			{
				data_trader::Retailer::GetInstance().RequestRoomInvitation(LobbyGameMode->GetRoomId(), NickName, SlotIndex, RaidDoc->GetLastRaidType(), RaidDoc->GetLastRaidStep());
				return true;
			}
		}
	}

	return false;
}

bool UB2UIGameInviteList::StartInvite_FriendlyPVP(const FString& NickName, int32 SlotIndex)
{
	if (InviteSlots.IsValidIndex(SlotIndex))
	{
		InviteSlots[SlotIndex]->SetSlotState(EGameInviteSlotIndex::Inviting);
		if (auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			LobbyGameMode->RequestGameInvite(NickName, EB2GameMode::PVP_Tag);

			return true;
		}
	}

	return false;
}

void UB2UIGameInviteList::OnDeliveryGetFriendList(const FB2FriendList& FriendList)
{
	//TArray<FB2FriendPtr> arFriendList = GET_TUPLE_DATA(FB2ResponseGetFriendList::friends_index, FriendList);
	//int32 FriendDeleteRemainCount = GET_TUPLE_DATA(FB2ResponseGetFriendList::left_delete_friend_count_index, FriendList);

	////CreateFriendInviteSlots(arFriendList);
	//CreateInviteSlots(arFriendList);
}

void UB2UIGameInviteList::OnDeliveryGetGuildMemberList(const FB2ResponseGuildMemberPtr& GuildMemberList)
{
	CreateInviteSlots(GuildMemberList->guild_members);
}

void UB2UIGameInviteList::OnDeliveryFindUser(const FB2FindUser& FindUser)
{
	//TArray<FB2UserPtr> FriendList;
	//FB2UserPtr UserInfo = GET_TUPLE_DATA(FB2ResponseFindUser::user_index, FindUser);

	//if (UserInfo != nullptr)
	//{
	//	const int32 NextSlotIndex = CreateInviteSlotCommon(0, UserInfo->character_summary, 0);
	//	SetDisibleSlotList(NextSlotIndex);
	//}
}

void UB2UIGameInviteList::CloseWidgetDelegate()
{
	OnClick_BTN_Close();
}

void UB2UIGameInviteList::OnClick_BTN_Close()
{
	CloseInviteListUI();
}

void UB2UIGameInviteList::OnClick_BTN_CloseArea()
{
	CloseInviteListUI();
}

void UB2UIGameInviteList::OnClick_BTN_UserSearch()
{
	if (TIB_SearchName.IsValid())
	{
		const FString NickName = TIB_SearchName->GetText().ToString();
		RequestFindUser(NickName);
	}
}

void UB2UIGameInviteList::OnClick_BTN_Refresh()
{
	StartRefreshTimer(SelectedTabType);
}

void UB2UIGameInviteList::OnClickBtnFriend()
{
	SetUIState(EGameInviteTab::Friend);
}

void UB2UIGameInviteList::OnClickBtnGuild()
{
	SetUIState(EGameInviteTab::Guild);
}

void UB2UIGameInviteList::OnClickBtnUserSearchTab()
{
	SetUIState(EGameInviteTab::Search);
}
