#include "B2UIChatting.h"
#include "B2UIManager.h"
#include "B2UIChatMessage.h"
#include "B2UIMsgPopupInputNoneFocus.h"
#include "B2UIMsgPopupSimple.h"
#include "B2UIGuildSupportItemPopup.h"
#include "MissionManager.h"
#include "B2UIDocHelper.h"
#include "B2UIMain.h"
#include "Retailer.h"
#include "B2AndroidBackManager.h"
#include "OnlineWholesaler.h"
#include "B2UIBlockList.h"
#include "B2UIMsgPopupCenter.h"
#include "B2GameInstance.h"
#include "BladeIIGameImpl.h"
#include "B2UIChatMssageGuildSupport.h"

#define INPUT_GUARD_TIMER	3

UB2UIChatting::UB2UIChatting(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, SYS_CHEAT_PREFIX(TEXT("//LUC"))
	, SYS_SERVERCMD_PREFIX(TEXT("SERVERCMD"))
{
	SelectedCategory = EB2UIChatCategory::UICHTY_End;

	MessagePartClass_OtherUser = NULL;
	MessagePartClass_Myself = NULL;
	MessagePartClass_PublicNoti = NULL;
	bDirectScrolling = false;
	bClampedEditMsgPending = false;
	bBlockScrollBox = false;

	LastScrolledOffect = 0.0f;
}

void UB2UIChatting::Init()
{
	Super::Init();

	//SetChatCategoryTab(EB2UIChatCategory::UICHTY_All); // 여기서 하지 않는다. ScrollBox 가 완전히 초기화 된 상태가 아님..

	BladeIIGameImpl::GetChatStore().RequestGetChannelNum();

	UpdateControlInfo(); // RequestGetChannelNum 에 의해 응답이 오면 다시 세팅되어야.

	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UB2UIMain* UIMain = UIMgrInst->GetMainWidget();
		// 채팅 창 없을 때에 표시하는 한줄 채팅이 열려 있다면 바로 닫는다. 
		if(UIMain)	UIMain->ForceCloseChatSinglePopup();
	}

	bool JoinedGuild = BladeIIGameImpl::GetClientDataStore().GetUserGuildID() > 0;
	if (JoinedGuild)
	{
		//아이템 인벤토리에서 받았는데 정수 정보 때문에 호출..
		for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
		{
			BladeIIGameImpl::GetClientDataStore().SyncingInventoryData(IntToPCClass(i));
		}

		// 최초 채팅 내용 업데이트.
		BladeIIGameImpl::GetChatStore().RequestGetGuildChat();
	}

	SubscribeEvent();
}

void UB2UIChatting::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Chatting_Title)

	GET_SLOT(UB2ScrollBox, SB_MessagePanel);
	GET_SLOT(UEditableText, EDIT_ChatMsg);
	GET_SLOT(UB2Button, BTN_EnterChatMsg);
	GET_SLOT(UTextBlock, TB_EnterChatMsg);
	GET_SLOT(UB2Button, BTN_Close);

	GET_SLOT(UTextBlock, TB_EssentialNotiLabel);
	GET_SLOT(UTextBlock, TB_EssentialNotiContent);

	GET_SLOT(UB2Button, BTN_Control);
	GET_SLOT(UTextBlock, TB_Control);

	GET_SLOT(UB2Button, BTN_BlockList);
	GET_SLOT(UTextBlock, TB_BlockList);

	GET_SLOT(UCanvasPanel, CP_ChatTabSet_All);
	GET_SLOT(UB2Button, BTN_ChatTab_All);
	GET_SLOT(UImage, IMG_Selected_ChatTab_All);
	GET_SLOT(UTextBlock, TB_ChatTab_All);

	GET_SLOT(UCanvasPanel, CP_ChatTabSet_Guild);
	GET_SLOT(UB2Button, BTN_ChatTab_Guild);
	GET_SLOT(UImage, IMG_Selected_ChatTab_Guild);
	GET_SLOT(UTextBlock, TB_ChatTab_Guild);

	GET_SLOT(UCanvasPanel, CP_ChatTabSet_Noti);
	GET_SLOT(UB2Button, BTN_ChatTab_Noti);
	GET_SLOT(UImage, IMG_Selected_ChatTab_Noti);
	GET_SLOT(UTextBlock, TB_ChatTab_Noti);

	GET_SLOT(UPanelWidget, X_ChatInput);

	GET_SLOT(UPanelWidget, X_GuildBadge);
	GET_SLOT(UTextBlock, TB_GuildNotiCount);

	GET_SLOT(UB2Button, BTN_ScrollDown);
	GET_SLOT(UB2Button, BTN_ScrollUp);
	GET_SLOT(UTextBlock, TB_ScrollUp);
	GET_SLOT(UTextBlock, TB_ScrollDown);

	GET_SLOT(UB2UIUserInfoActionPopup, UIP_UserInfoActionPopupSet);
	if (UIP_UserInfoActionPopupSet.IsValid())
	{
		UIP_UserInfoActionPopupSet->Init();
		UIP_UserInfoActionPopupSet->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UIChatting::UpdateStaticText()
{
	if (TB_Chatting_Title.IsValid())
	{
		TB_Chatting_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Title")));
	}
	if (TB_EnterChatMsg.IsValid())
	{
		TB_EnterChatMsg->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_InputMessage")));
	}
	if (EDIT_ChatMsg.IsValid())
	{
		EDIT_ChatMsg->SetHintText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_InputHint_NoBullshit")));
	}
	if (TB_EssentialNotiLabel.IsValid())
	{
		TB_EssentialNotiLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")));
	}
	if (TB_EssentialNotiContent.IsValid())
	{
		TB_EssentialNotiContent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_EssentialNoti")));
	}
	if (TB_ChatTab_All.IsValid())
	{
		TB_ChatTab_All->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Tab_All")));
	}
	if (TB_ChatTab_Guild.IsValid())
	{
		TB_ChatTab_Guild->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Tab_GuildChat")));
	}
	if (TB_ChatTab_Noti.IsValid())
	{
		TB_ChatTab_Noti->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_Tab_Notification")));
	}
	if (TB_BlockList.IsValid())
	{
		TB_BlockList->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BlockUserList")));
	}
}

void UB2UIChatting::UpdateNoticeContent()
{
	if (TB_EssentialNotiContent.IsValid())
	{	
		const bool bTabGuild = (SelectedCategory == EB2UIChatCategory::UICHTY_Guild);
		FText DefaultNoticeMsg = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_EssentialNoti"));

		if (BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo)
		{
			const FString GuildNoticeMsg = BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->notice;
			if (bTabGuild && !GuildNoticeMsg.IsEmpty())
			{
				DefaultNoticeMsg = FText::FromString(GuildNoticeMsg);
			}
		}

		TB_EssentialNotiContent->SetText(DefaultNoticeMsg);
	}
}

void UB2UIChatting::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_EnterChatMsg, &UB2UIChatting::OnClickBtnEnterChatMsg);
	BIND_CLICK_FUNC_TO_BTN(BTN_Control, &UB2UIChatting::OnClickBtnControl);
	BIND_CLICK_FUNC_TO_BTN(BTN_ChatTab_All, &UB2UIChatting::OnClickBtnChatTabAll);
	BIND_CLICK_FUNC_TO_BTN(BTN_ChatTab_Guild, &UB2UIChatting::OnClickBtnChatTabGuild);
	BIND_CLICK_FUNC_TO_BTN(BTN_ChatTab_Noti, &UB2UIChatting::OnClickBtnChatTabNoti);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIChatting::OnClickBtnClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_ScrollDown, &UB2UIChatting::OnClickBtnScrollDown);
	BIND_CLICK_FUNC_TO_BTN(BTN_ScrollUp, &UB2UIChatting::OnClickBtnScrollUp);
	BIND_CLICK_FUNC_TO_BTN(BTN_BlockList, &UB2UIChatting::OnClickBlockList);

	if (EDIT_ChatMsg.IsValid())
	{
		EDIT_ChatMsg->OnTextChanged.RemoveDynamic(this, &UB2UIChatting::OnChatMsgEditChanged);
		EDIT_ChatMsg->OnTextChanged.AddDynamic(this, &UB2UIChatting::OnChatMsgEditChanged);
	}

	if (SB_MessagePanel.IsValid())
	{
		SB_MessagePanel->SetOnUserScrolledEnd(FSimpleDelegate::CreateUObject(this, &UB2UIChatting::OnEndScroll));
		SB_MessagePanel->SetOnUserScrolled(FOnUserScrolled::CreateUObject(this, &UB2UIChatting::OnUpdateScroll));
	}
}

void UB2UIChatting::BindDocAuto()
{
	UnbindDoc();

	UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome)
	{
		DocSome->OnCurrentChatChannelChanged.AddUObject(this, &UB2UIChatting::OnChangedCurrentChannel);
	}
	UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
	if (DocGuild)
	{
		DocGuild->OnGuildChatBadgeChanged.AddUObject(this, &UB2UIChatting::UpdateGuildBadge);
	}
}
void UB2UIChatting::UnbindDoc()
{
	UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome)
	{
		DocSome->OnCurrentChatChannelChanged.RemoveAll(this);
	}
	UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
	if (DocGuild)
	{
		DocGuild->OnGuildChatBadgeChanged.RemoveAll(this);
	}
}

void UB2UIChatting::OnOpen(bool RightNow)
{
	//Super::OnOpen(RightNow);

	//if (X_GuildBadge.IsValid())
	//	X_GuildBadge->SetVisibility(ESlateVisibility::Hidden);

	//SetChatCategoryTab(EB2UIChatCategory::UICHTY_All); // 이전에 채팅 창을 열었을 때의 탭 선택 상태 캐싱이 필요할 수 있음..
	//UpdateGuildBadge();

	//UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToMessageUpdate);
	//UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(
	//	TimeToMessageUpdate
	//	, FTimerDelegate::CreateUObject(this, &UB2UIChatting::OnUpdateMessageTime)
	//	, 5.0f
	//	, true);

	//StartVisibleChatPopupClass<EB2GameMode>::GetInstance().Signal(GetB2GameModeType(this));
}

void UB2UIChatting::OnClose(bool RightNow /*= false*/)
{
	//Super::OnClose(RightNow);
	//UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToMessageUpdate);
	//StopVisibleChatPopupClass<>::GetInstance().Signal();
}

void UB2UIChatting::SubscribeEvent()
{
	//DeliveryGetGuildInfoTicket = DeliveryReceiveGuildDetailInfoClass<FB2ReceiveGuildInfo>::GetInstance().Subscribe([this](const FB2ReceiveGuildInfo& GuildInfoData)
	//{
	//	ReceiveGuildDetailInfo(GuildInfoData);
	//});

	//UpdateChatUITicket = UpdateChatUIClass<bool>::GetInstance().Subscribe(
	//	[this](const bool IsScrollEnd)
	//{
	//	UpdateChatMessage(IsScrollEnd);
	//});

	//if (BladeIIGameImpl::GetClientDataStore().GetUserGuildID() > 0)
	//{
	//	data_trader::Retailer::GetInstance().RequestGuildDetailInfo(/*BladeIIGameImpl::GetClientDataStore().GetUserGuildID()*/0);
	//}
}

void UB2UIChatting::UnsubscribeEvent()
{
	//UpdateChatUIClass<bool>::GetInstance().Unsubscribe(UpdateChatUITicket);
	//DeliveryReceiveGuildDetailInfoClass<FB2ReceiveGuildInfo>::GetInstance().Unsubscribe(DeliveryGetGuildInfoTicket);
}

void UB2UIChatting::ReceiveGuildDetailInfo(const FB2ReceiveGuildInfo& GuildInfoData)
{
	UpdateNoticeContent();
}

void UB2UIChatting::UpdateControlInfo()
{
	if (TB_Control.IsValid())
	{
		const bool bTabAll = (SelectedCategory == EB2UIChatCategory::UICHTY_All);
		const bool bTabGuild = (SelectedCategory == EB2UIChatCategory::UICHTY_Guild);
		if (bTabAll)
		{
			TB_Control->SetText(
				FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_ChannelInfo")), FText::AsNumber(BladeIIGameImpl::GetChatStore().GetCurrentChannel()))
			);
			if (BTN_Control.IsValid())
				BTN_Control->SetIsEnabled(true);
		}
		else if (bTabGuild)
		{
			bool IsOverTime = false;
			UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
			if (DocGuild)
			{
				IsOverTime = DocGuild->IsOverAskSupportTime();
			}
			if (IsOverTime == false)
			{
				FDateTime NextAskTime = FDateTime::FromUnixTimestamp(DocGuild->GetNextAskSupportTime() / 1000);
				int64 RemainTime = (NextAskTime - UB2GameInstance::GetUTCNow()).GetTotalSeconds();
				TB_Control->SetText(FText::FromString(UB2UIManager::FormatSecondsAsTime(RemainTime, TEXT("hh:mm"))));
			}
			else
			{
				TB_Control->SetText(
					BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EssenceRequestBtn"))
				);
			}

			if (BTN_Control.IsValid())
				BTN_Control->SetIsEnabled(IsOverTime);
		}
	}
}

void UB2UIChatting::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 길이 제한 넘어가서 잘린 텍스트 여기서 세팅.
	if (bClampedEditMsgPending && EDIT_ChatMsg.IsValid())
	{
		EDIT_ChatMsg->SetText(PendingClampedEditMsg);
		bClampedEditMsgPending = false;
	}

	//OnUpdateScroll(InDeltaTime);
}

void UB2UIChatting::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	UnsubscribeEvent();

	DestroyChatMessages();

	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToUpdateScroll);
}

void UB2UIChatting::UpdateGuildBadge()
{
	UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
	if (DocGuild)
	{
		UpdateGuildBadge(DocGuild, DocGuild->GetGuildChatBadge(), 0);
	}
}

void UB2UIChatting::UpdateGuildBadge(class UB2UIDocBase* Sender, int32 Cur, int32 Prev)
{
	if (X_GuildBadge.IsValid())
	{
		X_GuildBadge->SetVisibility(Cur > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}

	if (X_GuildBadge.IsValid())
	{
		TB_GuildNotiCount->SetText(FText::AsNumber(Cur));
	}
}

void UB2UIChatting::UpdateChatMessageAll()
{
	if (!SB_MessagePanel.IsValid()) {
		return;
	}

	const bool bTabGuild = (SelectedCategory == EB2UIChatCategory::UICHTY_Guild);
	TArray<FB2ChatMessage> AllMessages;
	BladeIIGameImpl::GetChatStore().GetChatMessages(GetAllDisplayTypes(SelectedCategory), AllMessages, !bTabGuild);
	// GetChatMessages 에서 얻은 건 새 메시지가 앞으로 정렬이 되어 있음. 스크롤 박스에는 오래된 메시지부터 넣어야 함. 뒤에서부터.
	for (int32 MI = AllMessages.Num() - 1; MI >= 0; --MI)
	{
		const FB2ChatMessage& ThisMessage = AllMessages[MI];

		UB2UIChatMessageBase* ThisChatMsg = GetUIMessageSlot(ThisMessage);

		if (ThisMessage.bIsMyMessage == false)
			ThisChatMsg->OnChatUserNameClicked.BindDynamic(this, &UB2UIChatting::OnChatMsgUserNameClicked);

		ThisChatMsg->SetChatMessageInfo(ThisMessage);

		UScrollBoxSlot* AddedRowSlot = Cast<UScrollBoxSlot>(SB_MessagePanel->AddChild(ThisChatMsg));
		if (AddedRowSlot)
		{
			// 혹시 Margin 설정 같은 거 필요하면 AddedRowSlot 을 가지고..

			if (ThisMessage.MessageType == EB2ChatType::Notice || ThisMessage.MessageType == EB2ChatType::GuildNotice)
			{
				AddedRowSlot->SetHorizontalAlignment(HAlign_Fill);
			}
			else if (ThisMessage.bIsMyMessage)
			{
				AddedRowSlot->SetHorizontalAlignment(HAlign_Right);
			}
			else
			{
				AddedRowSlot->SetHorizontalAlignment(HAlign_Left);
			}
		}

		CreatedMessages.Add(ThisChatMsg);

		if (ThisMessage.NoticeType == EB2ChatNoticeType::GuildSupport)
			CachedSupportMessages.Add(ThisChatMsg);
	}
}

void UB2UIChatting::UpdateChatMessage(bool bAutoScrollDown)
{
	if (SB_MessagePanel.IsValid() == false)
		return;

	if (bBlockScrollBox == true && SB_MessagePanel->IsScrolling() == true)
		return;

	bool bUpdateChat = false;
	FB2ChatMessage ThisMessage;
	for ( ; BladeIIGameImpl::GetChatStore().TryGetNewChatMessage(ThisMessage); )
	{
		if (ThisMessage.NoticeType == EB2ChatNoticeType::GuildSupport)
		{
			int64 TargetID = ThisMessage.ChatID;
			for (int32 Index = CachedSupportMessages.Num() - 1; Index > -1; --Index)
			{
				UB2UIChatMssageGuildSupport *EachRow = Cast<UB2UIChatMssageGuildSupport>(CachedSupportMessages[Index]);
				if (EachRow != nullptr && EachRow->GetChatID() == TargetID)
				{
					bUpdateChat = true;
					EachRow->SetChatMessageInfo(ThisMessage);
					break;
				}
			}

			if(bUpdateChat)
				continue;
		}
		else if (ThisMessage.NoticeType == EB2ChatNoticeType::GuildSupportComplete)
		{
			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			int64 TargetID = ThisMessage.ChatID;
			for (int32 Index = CachedSupportMessages.Num() - 1; Index > -1; --Index)
			{
				UB2UIChatMssageGuildSupport *EachRow = Cast<UB2UIChatMssageGuildSupport>(CachedSupportMessages[Index]);
				if (EachRow != nullptr && EachRow->GetChatID() == TargetID)
				{
					EachRow->RemoveFromParent();
					EachRow->DestroySelf(UIMgrInst);
					CachedSupportMessages.RemoveAt(Index);
				}
			}
			continue;
		}

		bUpdateChat = true;
		UB2UIChatMessageBase* ThisChatMsg = GetUIMessageSlot(ThisMessage);

		if (ThisMessage.bIsMyMessage == false)
			ThisChatMsg->OnChatUserNameClicked.BindDynamic(this, &UB2UIChatting::OnChatMsgUserNameClicked);

		ThisChatMsg->SetChatMessageInfo(ThisMessage);

		UScrollBoxSlot* AddedRowSlot = Cast<UScrollBoxSlot>(SB_MessagePanel->AddChild(ThisChatMsg));
		if (AddedRowSlot)
		{
			// 혹시 Margin 설정 같은 거 필요하면 AddedRowSlot 을 가지고..

			if (ThisMessage.MessageType == EB2ChatType::Notice || ThisMessage.MessageType == EB2ChatType::GuildNotice)
			{
				AddedRowSlot->SetHorizontalAlignment(HAlign_Fill);
			}
			else if (ThisMessage.bIsMyMessage)
			{
				AddedRowSlot->SetHorizontalAlignment(HAlign_Right);
			}
			else
			{
				AddedRowSlot->SetHorizontalAlignment(HAlign_Left);
			}
		}

		CreatedMessages.Add(ThisChatMsg);

		if (ThisMessage.NoticeType == EB2ChatNoticeType::GuildSupport)
			CachedSupportMessages.Add(ThisChatMsg);
	}

	if(bUpdateChat)
		UpdateChatScrollEnd(bAutoScrollDown);
}

void UB2UIChatting::UpdateChatScrollEnd(bool bForce)
{
	bool IsValid = SB_MessagePanel.IsValid();
	bool IsScrolling = IsValid ? SB_MessagePanel->IsScrolling() : false;
	bool bFocused  = SB_MessagePanel->HasAnyUserFocus();
	if (bForce && (bBlockScrollBox == false))
		SB_MessagePanel->ScrollToEnd(); // 맨 아래에 최신 메시지가 있다.
}

void UB2UIChatting::SetUserBlockState(bool IsBlock, const FString& UserNickName)
{
	if (UIP_UserInfoActionPopupSet.IsValid())
	{
		UIP_UserInfoActionPopupSet->SetUserBlockState(IsBlock);
	}

	if (!UserNickName.IsEmpty())
	{
		FText TextServerVersion = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BlockNotifyFormat"))
			, FText::FromString(UserNickName)
			, BladeIIGetLOCText(B2LOC_CAT_GENERAL, IsBlock ? TEXT("BlockUser") : TEXT("UnBlockUser")));
		
		UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupCenter>(EUIMsgPopup::Center,
			FText::FromString(TEXT("")),
			TextServerVersion,
			1.0f,
			false
			);
	}
}

void UB2UIChatting::DestroyChatMessages()
{
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	for (UB2UIChatMessageBase* ThisMessageUI : CreatedMessages)
	{
		if (ThisMessageUI) {
			ThisMessageUI->DestroySelf(UIMgrInst);
		}
	}

	for (int32 i = FChatMessageType::None; i < FChatMessageType::GuildSupport + 1; ++i)
	{
		if(CachedSlotMap.Find(i))
			CachedSlotMap[i].Slots.Empty();
	}

	if (SB_MessagePanel.IsValid())
		SB_MessagePanel->ClearChildren();

	CreatedMessages.Empty();
	CachedSupportMessages.Empty();


	LastScrolledOffect = 0.0f;
	ScrolledSupportIndex = 0;
}

void UB2UIChatting::HideChatMessages()
{
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	for (UB2UIChatMessageBase* ThisMessageUI : CreatedMessages)
	{
		if (ThisMessageUI) {
			ThisMessageUI->SetVisibility(ESlateVisibility::Collapsed);
			ThisMessageUI->OnChatUserNameClicked.Unbind();
			ThisMessageUI->RemoveFromParent();
			CachedSlotMap[ThisMessageUI->GetSlotType()].Slots.Add(ThisMessageUI);
		}
	}

	if (SB_MessagePanel.IsValid())
		SB_MessagePanel->ClearChildren();

	CreatedMessages.Empty();

	CachedSupportMessages.Empty();
	LastScrolledOffect = 0.0f;
	ScrolledSupportIndex = 0;
}

void UB2UIChatting::SetChatCategoryTab(EB2UIChatCategory InUIChatCategory)
{
	if (SelectedCategory == InUIChatCategory)
		return;
	SelectedCategory = InUIChatCategory;

	// 길드 구현 전까지 길드 채팅 탭은 숨긴다. 사용자가 길드 가입되어 있는 경우만 사용.
	if (CP_ChatTabSet_Guild.IsValid())
	{
		bool JoinedGuild = BladeIIGameImpl::GetClientDataStore().GetUserGuildID() > 0;
		CP_ChatTabSet_Guild->SetVisibility(JoinedGuild ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	
	const bool bTabAll = (InUIChatCategory == EB2UIChatCategory::UICHTY_All);
	const bool bTabGuild = (InUIChatCategory == EB2UIChatCategory::UICHTY_Guild);
	const bool bTabNoti = (InUIChatCategory == EB2UIChatCategory::UICHTY_Noti);

	if (BTN_ChatTab_All.IsValid())
	{
	//	BTN_ChatTab_All->SetIsEnabled(!bTabAll);
	}
	if (IMG_Selected_ChatTab_All.IsValid())
	{
		IMG_Selected_ChatTab_All->SetVisibility(bTabAll ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
	
	if (BTN_ChatTab_Guild.IsValid())
	{
	//	BTN_ChatTab_Guild->SetIsEnabled(!bTabGuild);
	}
	if (IMG_Selected_ChatTab_Guild.IsValid())
	{
		IMG_Selected_ChatTab_Guild->SetVisibility(bTabGuild ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}

	if (BTN_ChatTab_Noti.IsValid())
	{
	//	BTN_ChatTab_Noti->SetIsEnabled(!bTabNoti);
	}
	if (IMG_Selected_ChatTab_Noti.IsValid())
	{
		IMG_Selected_ChatTab_Noti->SetVisibility(bTabNoti ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}

	if (bTabGuild)
	{
		UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
		if (DocGuild)
		{
			DocGuild->SetGuildChatBadge(0);
		}
		UpdateGuildBadge();
	}

	if (BTN_ScrollDown.IsValid())
		BTN_ScrollDown->SetVisibility(bTabGuild ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (BTN_ScrollUp.IsValid())
		BTN_ScrollUp->SetVisibility(bTabGuild ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	// 알림탭에서는 입력창 없앰
	const bool bChatInputPossible = (SelectedCategory == EB2UIChatCategory::UICHTY_All || SelectedCategory == EB2UIChatCategory::UICHTY_Guild);
	if (X_ChatInput.IsValid())
	{
		X_ChatInput->SetVisibility(bChatInputPossible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}

	UpdateScrollButtonText(false, false);

	BladeIIGameImpl::GetChatStore().ClearNewChatMessages();

	DestroyChatMessages();
	UpdateControlInfo();
	UpdateChatMessageAll();
	UpdateChatScrollEnd(true);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(TimeToUpdateScroll, this, &UB2UIChatting::UpdateSideButtons, 0.1f, false);
	UpdateNoticeContent();
}

TArray<EB2ChatType> UB2UIChatting::GetAllDisplayTypes(EB2UIChatCategory InUIChatCategory)
{
	TArray<EB2ChatType> RetArray;

	if (InUIChatCategory == EB2UIChatCategory::UICHTY_All)
	{
		RetArray.Add(EB2ChatType::Channel);
	}
	else if (InUIChatCategory == EB2UIChatCategory::UICHTY_Guild)
	{
		RetArray.Add(EB2ChatType::Guild);
		RetArray.Add(EB2ChatType::GuildNotice);
	}
	else if (InUIChatCategory == EB2UIChatCategory::UICHTY_Noti)
	{
		RetArray.Add(EB2ChatType::Notice);
	}

	return RetArray;
}

EB2ChatType UB2UIChatting::GetMessageInputType(EB2UIChatCategory InUIChatCategory)
{
	switch (InUIChatCategory)
	{ // 로컬 사용자가 보낼 수 있는 것에 대해서만.
	case EB2UIChatCategory::UICHTY_All: return EB2ChatType::Channel;
	case EB2UIChatCategory::UICHTY_Guild: return EB2ChatType::Guild;
	}
	return EB2ChatType::End;
}

class UB2UIChatMessageBase* UB2UIChatting::GetUIMessageSlot(const FB2ChatMessage& InMessageInfo)
{
	//int32 SlotType = GetUIMessageSlotType(InMessageInfo);
	//	
	//if (CreatedMessages.Num() > 100)
	//{
	//	const int32 DistCount = CreatedMessages.Num() - 100;
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	for (int32 Index = DistCount; Index > -1; --Index)
	//	{
	//		if (CreatedMessages[Index] != nullptr)
	//		{
	//			if (CreatedMessages[Index]->GetSlotType() == SlotType)
	//			{
	//				CreatedMessages[Index]->RemoveFromParent();
	//				CreatedMessages[Index]->DestroySelf(UIMgrInst);
	//			}
	//		}

	//		CreatedMessages.RemoveAt(Index);
	//	}
	//}
	//
	//TSubclassOf<UB2UIChatMessageBase> ThisCreateClass = GetProperUIMessageClass(SlotType);
	//UB2UIChatMessageBase* ThisChatMsg = CreateWidget<UB2UIChatMessageBase>(GetOwningPlayer(), ThisCreateClass);
	//if (ThisChatMsg) {
	//	ThisChatMsg->Init();
	//	ThisChatMsg->SetSlotType(SlotType);

	//	// SB_MessagePanel 의 스크롤 핸들링을 위한 등록. ThisChatMsg 의 버튼이 입력을 가로채지 않고 넘겨주어서 직접 핸들링 하도록.
	//	ThisChatMsg->SetupManualScrollBoxSender_byReceiver(this, SB_MessagePanel.Get());

	//	return ThisChatMsg;
	//}

	return nullptr;
}

int32 UB2UIChatting::GetUIMessageSlotType(const FB2ChatMessage& InMessageInfo)
{
	bool IsNoticeType = InMessageInfo.MessageType == EB2ChatType::Notice || InMessageInfo.MessageType == EB2ChatType::GuildNotice;
	bool IsGuildSupport = InMessageInfo.NoticeType == EB2ChatNoticeType::GuildSupport;
	bool IsCustomNoti = (InMessageInfo.NoticeType == EB2ChatNoticeType::ChangeChannel)
		|| (InMessageInfo.NoticeType == EB2ChatNoticeType::GuildJoin);

	if (IsNoticeType)
	{
		if (IsGuildSupport)
		{
			return FChatMessageType::GuildSupport;
		}
		else
		{
			return FChatMessageType::PublicNotice;
		}
	}
	else
	{
		if (IsCustomNoti)
		{
			return FChatMessageType::LocalNotice;
		}
		else if (InMessageInfo.bIsMyMessage)
		{
			return FChatMessageType::MyChat;
		}
		return FChatMessageType::OtherChat;
	}

	return FChatMessageType::None;	// default
}

TSubclassOf<UB2UIChatMessageBase> UB2UIChatting::GetProperUIMessageClass(const int32 InSlotType)
{
	switch (InSlotType)
	{
	case FChatMessageType::PublicNotice:
		return MessagePartClass_PublicNoti;
	case FChatMessageType::LocalNotice:
		return MessagePartClass_LocalNoti;
	case FChatMessageType::MyChat:
		return MessagePartClass_Myself;
	case FChatMessageType::OtherChat:
		return MessagePartClass_OtherUser;
	case FChatMessageType::GuildSupport:
		return MessagePartClass_GuildSupport;
	default:
		break;
	}

	return MessagePartClass_PublicNoti; // defalut
}

// 채팅창을 활용한 콘솔커맨드.. 에뮬레이터 환경에서 사용을 위해.
bool UB2UIChatting::CheckAndExecuteConsoleCommandFromChatInput(const FString& ChatEditMsg)
{
#if B2_BUILD_LEVEL != B2_BUILD_LEVEL_LIVE
	//4Finger랑 동일한 작동을 하는 코드
	/*
	static const FString SYS_MSG_PREFIX(TEXT("//sys"));

	FString CmdOnly = ChatEditMsg;
	if (InPC && CmdOnly.RemoveFromStart(SYS_MSG_PREFIX)) // 추가로 변수 하나를 더 두던지..
	{
		CmdOnly.RemoveFromStart(TEXT(" ")); // 보통 prefix 뒤에 공백을 두게 될 테니 있으면 제거. 없다고 해도 그대로 인식하도록 하고.

		InPC->ConsoleCommand(CmdOnly);
		return true;
	}
	/*/
	//[@AKI, 180328][TO-DO] Android Build에서 Casting때문에 다시 확인 해봐야 할것
	const FString CheatMsg(ChatEditMsg.ToUpper());
	const TCHAR* Cmd = *CheatMsg;
	if (FParse::Command(&Cmd, SYS_CHEAT_PREFIX))
	{
		if (FParse::Command(&Cmd, SYS_SERVERCMD_PREFIX) )
		{
			data_trader::ExecHandler::GetInstance().ExecuteShellCommand(Cmd);
			return true;
		}
	}
	//*/
#endif
	return false;
}

void UB2UIChatting::OnClickBtnEnterChatMsg()
{
	if (EDIT_ChatMsg.IsValid())
	{
		FText EdittedMsg = EDIT_ChatMsg->GetText();
		EdittedMsg = ClampTextLength(EdittedMsg, FB2ChatStore::MessageLengthLimit); // 길이는 물론 에디트 도중에 잘려야 함. 최종 검사일 뿐.

		if (!CheckAndExecuteConsoleCommandFromChatInput(EdittedMsg.ToString()) &&
			EdittedMsg.ToString().Len() > 0 && 
			!BladeIIGameImpl::GetChatStore().IsPossibleSpamMessage(EdittedMsg)) // 도배 체크
		{
			// @TODO Chat 비속어 필터
			
			BladeIIGameImpl::GetChatStore().RequestChatCommand(GetMessageInputType(SelectedCategory), EdittedMsg);
		}

		EDIT_ChatMsg->SetText(FText::FromString(TEXT("")));
	}

	if (BTN_EnterChatMsg.IsValid())
		BTN_EnterChatMsg->SetIsEnabled(false);

	TimerChatInputCount = INPUT_GUARD_TIMER;
	if (TB_EnterChatMsg.IsValid())
	{
		TB_EnterChatMsg->SetText(FText::AsNumber(TimerChatInputCount));
	}

	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToInputGuard);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(
		TimeToInputGuard
		, FTimerDelegate::CreateUObject(this, &UB2UIChatting::OnUpdateChatInput)
		, 1.0f
		, true);
}

void UB2UIChatting::OnClickBtnChatTabAll()
{
	SetChatCategoryTab(EB2UIChatCategory::UICHTY_All);
}

void UB2UIChatting::OnClickBtnChatTabGuild()
{
	SetChatCategoryTab(EB2UIChatCategory::UICHTY_Guild);
}

void UB2UIChatting::OnClickBtnChatTabNoti()
{
	SetChatCategoryTab(EB2UIChatCategory::UICHTY_Noti);
}

void UB2UIChatting::OnChatMsgUserNameClicked(const int64 AccountID, const int64 GuildID, const FString& InUserName)
{
	if (UIP_UserInfoActionPopupSet.IsValid())
	{
		UIP_UserInfoActionPopupSet->SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(UIP_UserInfoActionPopupSet.Get());

		const bool bTabGuild = (SelectedCategory == EB2UIChatCategory::UICHTY_Guild);
		int64 RealGuildID = bTabGuild ? 0 : GuildID;
		UIP_UserInfoActionPopupSet->SetUserInfo(AccountID, RealGuildID, InUserName);
	}
}

void UB2UIChatting::OnChatMsgEditChanged(const FText& InText)
{
	if (InText.ToString().Len() > FB2ChatStore::MessageLengthLimit)
	{
		// 길이 제한 넘어가면 자르는데 자른 걸 바로 세팅하면 한글에서 크래쉬가 나고 이렇게 자른 걸 저장해 두었다가 Tick 에서 세팅
		// 그리고 여기선 일단 비워둬야 함.

		PendingClampedEditMsg = ClampTextLength(InText, FB2ChatStore::MessageLengthLimit);
		bClampedEditMsgPending = true;
		
		if (EDIT_ChatMsg.IsValid())
		{
			EDIT_ChatMsg->SetText(FText::FromString(TEXT("")));
		}
	}
}

void UB2UIChatting::OnClickBtnControl()
{
	const bool bTabAll = (SelectedCategory == EB2UIChatCategory::UICHTY_All);
	const bool bTabGuild = (SelectedCategory == EB2UIChatCategory::UICHTY_Guild);
	if (bTabAll)
		OpenChangeChannelPopup();
	else if (bTabGuild)
	{
		bool IsOverTime = true;
		UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
		if (DocGuild)
		{
			IsOverTime = DocGuild->IsOverAskSupportTime();
		}
 		//if(IsOverTime)
			OpenRequestGuildItemPopup();
	}
}

void UB2UIChatting::OpenRequestGuildItemPopup()
{
	UB2UIManager* UIMgrInst2 = UB2UIManager::GetInstance();
	if (UIMgrInst2)
	{
		auto *Popup = UIMgrInst2->OpenUI<UB2UIGuildSupportItemPopup>(UIFName::GuildSupportItemPopup);
	}
}

void UB2UIChatting::OpenChangeChannelPopup()
{
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UB2UIMsgPopupInputNoneFocus* InputPopup = UIMgrInst->OpenMsgPopup<UB2UIMsgPopupInputNoneFocus>(EUIMsgPopup::InputNoneFocus,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MoveChannelTitle")),
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MoveChannelInputHint")),
				FText::AsNumber(BladeIIGameImpl::GetChatStore().GetMinAllowedChannel()),
				FText::AsNumber(BladeIIGameImpl::GetChatStore().GetMaxAllowedChannel())),
			0.0f, true, true, EUIMsgPopupButtonGroup::ConfirmOrCancel);

		if (InputPopup)
		{
			InputPopup->AddHandler(EUIMsgPopupButton::Positive, FMsgPopupOnClick::CreateLambda([this, InputPopup](){
				// 채널 변경을 요청..
				int32 EdittedChannelNum = FCString::Atoi(*(InputPopup->GetInputText().ToString()));
				if (BladeIIGameImpl::GetChatStore().IsInAllowedChannelRange(EdittedChannelNum))
				{
					BladeIIGameImpl::GetChatStore().RequestChangeChannel(EdittedChannelNum);
				}
				else
				{ // 입력 범위가 잘못되었다면 다시 이 메시지 박스를 여는 안내 메시지. 현 구조상 닫지 않게는 못할 듯.
					UB2UIManager* UIMgrInst2 = UB2UIManager::GetInstance();
					if (UIMgrInst2)
					{
						UIMgrInst2->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
							BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
							BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MoveChannel_NotValidRange")),
							0.f, true, true,
							EUIMsgPopupButtonGroup::Confirm,
							FMsgPopupOnClick::CreateLambda([this](){
								OpenChangeChannelPopup();
							})
						);
					}					
				}
			}));
		}
	}
}

void UB2UIChatting::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2UIChatting::OnClickBtnClose()
{
	OnCloseCallback();

	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UIMgrInst->CloseUI(UIFName::Chatting);
	}
}

void UB2UIChatting::OnCloseCallback()
{
	/*if (UB2UIManager* UIMgrInst = UB2UIManager::GetInstance())
	{
		const EUIScene CurScene = UIMgrInst->GetCurrUIScene();

		const bool IsLobbyScene = CurScene == EUIScene::LobbyMain;
		const bool IsMissionScene = CurScene == EUIScene::Mission;
		const bool HasCompleteMission = MissionManager::GetInstance().HasCachedCompleteMission(EMissionType::Max);
		if (IsLobbyScene || IsMissionScene)
		{
			data_trader::Retailer::GetInstance().RequestGetMission(0);
		}
		if ((IsLobbyScene || IsMissionScene) && HasCompleteMission)
		{
			ShowMissionDialogClass<>::GetInstance().Signal();
		}
	}*/
}

void UB2UIChatting::OnChangedCurrentChannel(class UB2UIDocBase* Sender, const int32 CurrentChannelNum, const int32 PrevChannelNum)
{
	UpdateControlInfo(); // 어차피 ChatStore 에 들어가 있는 값을 쓸거.
}

bool UB2UIChatting::GetIsAvailableForInput()
{
	//if (BTN_EnterChatMsg.IsValid())
	//	return BTN_EnterChatMsg->bIsEnabled;
	//
	return false;
}

void UB2UIChatting::OnUpdateChatInput()
{
	--TimerChatInputCount;
	if (TB_EnterChatMsg.IsValid())
		TB_EnterChatMsg->SetText(FText::AsNumber(TimerChatInputCount));

	if (TimerChatInputCount < 1)
	{
		if (TB_EnterChatMsg.IsValid())
			TB_EnterChatMsg->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_InputMessage")));

		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToInputGuard);

		if (BTN_EnterChatMsg.IsValid())
			BTN_EnterChatMsg->SetIsEnabled(true);
	}
}

void UB2UIChatting::SetInvisibleChatRows()
{
	if (SB_MessagePanel.IsValid() == false)
		return;

	int32 MaxCount = SB_MessagePanel->GetChildrenCount();
	if (FMath::Max(0, MaxCount) < 1)
		return;

	float SizeCount = SB_MessagePanel->GetChildAt(0)->GetDesiredSize().Y;
	float Offset = SB_MessagePanel->GetScrollOffset();

	bool bScrollDown = SB_MessagePanel->GetScrollVelocity() > 0.0f;

	// 스크롤박스 상단
	int32 ScrolledIndex = FMath::RoundToInt(Offset / SizeCount);
	for (int32 i = ScrolledIndex - 1; i > -1; --i)
	{
		if (CreatedMessages.IsValidIndex(i) == false || CreatedMessages[i] == nullptr)
			continue;

		if (CreatedMessages[i]->GetVisibility() == ESlateVisibility::Hidden)
			break;

		if (CreatedMessages[i]->GetVisibility() != ESlateVisibility::Hidden)
			CreatedMessages[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	// 스크롤박스 클리핑 영역안 (7개는 보여줌)
	int32 VisibleMax = FMath::Min(MaxCount, ScrolledIndex + 7);
	for (int32 i = ScrolledIndex; i < VisibleMax; ++i)
	{
		if (CreatedMessages.IsValidIndex(i) == false || CreatedMessages[i] == nullptr)
			continue;
		if (CreatedMessages[i]->GetVisibility() == ESlateVisibility::Hidden)
			CreatedMessages[i]->SetVisibility(ESlateVisibility::Visible);
	}

	// 스크롤박스 하단
	int MaxBottomCount = FMath::Min(VisibleMax + 5, MaxCount);
	for (int32 i = VisibleMax; i < MaxBottomCount; ++i)
	{
		if (CreatedMessages.IsValidIndex(i) == false || CreatedMessages[i] == nullptr)
			continue;
		if (CreatedMessages[i]->GetVisibility() == ESlateVisibility::Hidden)
			break;
		if (CreatedMessages[i]->GetVisibility() != ESlateVisibility::Hidden)
			CreatedMessages[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIChatting::OnUpdateScroll(float ScrolledDelta)
{
	if (bDirectScrolling == false)
	{
		UpdateSideButtons();
	}
	bDirectScrolling = false;
	bBlockScrollBox = true;

	//SetInvisibleChatRows();
}

void UB2UIChatting::OnEndScroll()
{
	bBlockScrollBox = false;
	UpdateChatMessage(true);
}

int32 UB2UIChatting::GetTargetScrollIndex(const int32 TargetIndex)
{
	if (SB_MessagePanel.IsValid() == false)
		return -1;

	if (TargetIndex < 0)
		return -1;

	for (int32 i = 0; i < CachedSupportMessages.Num(); ++i)
	{
		if (auto *Each = CachedSupportMessages[i])
		{
			int32 index = SB_MessagePanel->GetChildIndex(Each);
			
			if (TargetIndex == index)
				return i;
		}
	}
	
	return -1;
}

int32 UB2UIChatting::GetTargetScrollCount(const int32 From, const bool bIsMin)
{
	if (SB_MessagePanel.IsValid() == false)
		return 0;
	
	int32 Result = 0;
	const int32 VisibleOffset = 5;
	for (int32 i = 0; i < CachedSupportMessages.Num(); ++i)
	{
		if (auto *Each = CachedSupportMessages[i])
		{
			int32 index = SB_MessagePanel->GetChildIndex(Each);
			if (bIsMin)
			{
				if (From > index)
					++Result;
			}
			else
			{
				// 화면에 노출되는 사이즈만큼은 계산 안됨..
				if (From + VisibleOffset < index)
					++Result;
			}
		}
	}

	return Result;
}

int32 UB2UIChatting::GetTargetScrollNumber(const int32 From, const bool bIsMin)
{
	if (SB_MessagePanel.IsValid() == false)
		return -1;

	int32 IintNum = bIsMin ? -1 : 100000;
	int32 ResultIndex = IintNum;
	for (int32 i = 0; i < CachedSupportMessages.Num(); ++i)
	{
		if (auto *Each = CachedSupportMessages[i])
		{
			int32 index = SB_MessagePanel->GetChildIndex(Each);
			if (bIsMin)
			{
				if (From > index)
					ResultIndex = FMath::Max(ResultIndex, index);
			}
			else
			{
				if (From < index)
					ResultIndex = FMath::Min(ResultIndex, index);
			}
		}
	}

	return IintNum == ResultIndex ? -1 : ResultIndex;
}

int32 UB2UIChatting::GetLastSupportSlotIndex()
{
	return CachedSupportMessages.Num() > 0 ? CachedSupportMessages.Num() - 1 : -1;
}

int32 UB2UIChatting::GetFitstSupportSlotIndex()
{
	return CachedSupportMessages.Num() > 0 ? 0 : -1;
}

int32 UB2UIChatting::GetScrollNearTopIndex()
{
	int32 CurScrolledIndex = 0;
	if (SB_MessagePanel.IsValid())
	{
		float HalfSize = 0.0f;
		float Offset = SB_MessagePanel->GetScrollOffset() + HalfSize;

		float SizeCount = 0;
		
		for (CurScrolledIndex = 0; CurScrolledIndex < SB_MessagePanel->GetChildrenCount(); ++CurScrolledIndex)
		{
			SizeCount += SB_MessagePanel->GetChildAt(CurScrolledIndex)->GetDesiredSize().Y;
			if (SizeCount > Offset)
				break;
		}
	}

	return CurScrolledIndex;
}

int32 UB2UIChatting::GetScrollNearBottomIndex()
{
	int32 CurScrolledIndex = 0;
	if (SB_MessagePanel.IsValid())
	{
		float HalfSize = 400.0f;//강제로 박아둠..
		float Offset = SB_MessagePanel->GetScrollOffset() + HalfSize;

		float SizeCount = 0;

		for (CurScrolledIndex = 0; CurScrolledIndex < SB_MessagePanel->GetChildrenCount(); ++CurScrolledIndex)
		{
			SizeCount += SB_MessagePanel->GetChildAt(CurScrolledIndex)->GetDesiredSize().Y;
			if (SizeCount > Offset)
				break;
		}
	}

	return CurScrolledIndex;
}

void UB2UIChatting::OnUpdateMessageTime()
{
	const bool bTabGuild = (SelectedCategory == EB2UIChatCategory::UICHTY_Guild);
	bool IsChangeGuild = BladeIIGameImpl::GetChatStore().UpdateGuildChatMessage();

	if (bTabGuild && IsChangeGuild)
	{
		DestroyChatMessages();
		UpdateChatMessageAll();
	}
	else
	{
		for (UB2UIChatMessageBase* ThisMessageUI : CreatedMessages)
		{
			if (ThisMessageUI) 
				ThisMessageUI->UpdateMessageTime();
		}
	}

	UpdateControlInfo(); 
}

void UB2UIChatting::OnClickBtnScrollUp()
{
	bDirectScrolling = true;

	int32 CurScrolledIndex = GetScrollNearTopIndex();
	int32 NearUp = GetTargetScrollNumber(CurScrolledIndex, true);
	int32 TargetIndex = GetTargetScrollIndex(NearUp);

	CheckScrollButtons(GetTargetScrollIndex(NearUp));
}

void UB2UIChatting::OnClickBlockList()
{
	if (auto *Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIBlockList>(UIFName::BlockUserList))
	{

	}
}

void UB2UIChatting::OnClickBtnScrollDown()
{
	bDirectScrolling = true;
	int32 CurScrolledBottomIndex = GetScrollNearBottomIndex();
	int32 NearBottom = GetTargetScrollNumber(CurScrolledBottomIndex, false);
	int32 TargetIndex = GetTargetScrollIndex(NearBottom);

	CheckScrollButtons(GetTargetScrollIndex(NearBottom));
}

void UB2UIChatting::CheckScrollButtons(const int32 TargetIndex)
{
	ScrolledSupportIndex = TargetIndex;
	if (CachedSupportMessages.IsValidIndex(ScrolledSupportIndex))
	{
		if (SB_MessagePanel.IsValid())
			SB_MessagePanel->ScrollWidgetIntoView(CachedSupportMessages[ScrolledSupportIndex]);

		int32 Slotindex = SB_MessagePanel.IsValid() ? SB_MessagePanel->GetChildIndex(CachedSupportMessages[ScrolledSupportIndex]) : 0;

		UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(TimeToUpdateScroll, this, &UB2UIChatting::UpdateSideButtons, 0.05f, false);
	}
	else
	{
		UpdateScrollButtonText(false, false);
	}
}

void UB2UIChatting::UpdateSideButtons()
{
	const bool bTabGuild = (SelectedCategory == EB2UIChatCategory::UICHTY_Guild);
	if (bTabGuild == false)
	{
		UpdateScrollButtonState(false, false);
		return;
	}

	if (CachedSupportMessages.Num() < 1)
	{
		UpdateScrollButtonState(false, false);
		return;
	}

	if (CachedSupportMessages.Num() > 0 && SB_MessagePanel.IsValid())
	{
		int32 CurScrolledIndex = GetScrollNearTopIndex();
		int32 CurScrolledBottomIndex = GetScrollNearBottomIndex();

		int32 NearUp = GetTargetScrollNumber(CurScrolledIndex, true);
		int32 NearBottom = GetTargetScrollNumber(CurScrolledBottomIndex, false);
		int32 DistUp = FMath::Abs(CurScrolledIndex - NearUp);
		int32 DistDown = FMath::Abs(CurScrolledBottomIndex - NearBottom);
		int32 ClosetNum = DistUp <= DistDown ? NearUp : NearBottom;
		ScrolledSupportIndex = GetTargetScrollIndex(ClosetNum);

		int32 HasUpCount = GetTargetScrollCount(CurScrolledIndex, true);
		int32 HasDownCount = GetTargetScrollCount(CurScrolledIndex, false);
		UpdateScrollButtonText(HasUpCount, HasDownCount);
	}
}

void UB2UIChatting::UpdateScrollButtonState(const bool bUp, const bool bDown)
{
	if (BTN_ScrollUp.IsValid())
		BTN_ScrollUp->SetVisibility(bUp ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (BTN_ScrollDown.IsValid())
		BTN_ScrollDown->SetVisibility(bDown ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIChatting::UpdateScrollButtonText(const int32 UpIndex, const int32 BottomIndex)
{
	if (TB_ScrollUp.IsValid() && UpIndex > 0)
		TB_ScrollUp->SetText(FText::AsNumber(UpIndex));

	if (TB_ScrollDown.IsValid() && BottomIndex > 0)
		TB_ScrollDown->SetText(FText::AsNumber(BottomIndex));

	UpdateScrollButtonState(UpIndex > 0, BottomIndex > 0);
}