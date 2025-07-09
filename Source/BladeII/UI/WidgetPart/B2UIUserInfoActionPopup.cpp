#include "BladeII.h"
#include "B2UIUserInfoActionPopup.h"
#include "BladeIIGameMode.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "Retailer.h"

#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "B2Chatting.h"

UB2UIUserInfoActionPopup::UB2UIUserInfoActionPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIUserInfoActionPopup::Init()
{
	Super::Init();
	SubscribeEvent();

	ButtonDelay = 0.0f;
}
void UB2UIUserInfoActionPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	UnsubscribeEvent();
}

void UB2UIUserInfoActionPopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ButtonDelay += InDeltaTime;
}

void UB2UIUserInfoActionPopup::SetUserInfo(int64 InAccountID, const int64 GuildID, const FString& InUserName)
{
	CachedGuildID = GuildID;
	TheUserName = InUserName;
	CachedAccountID = InAccountID;

	IsBlockingUser = BladeIIGameImpl::GetChatStore().IsBlockUser(CachedAccountID);

	if (TB_UserName.IsValid())
	{
		TB_UserName->SetText(FText::FromString(TheUserName));
	}

	if (BTN_VisitGuild.IsValid()) 
	{
		BTN_VisitGuild->SetVisibility(CachedGuildID > 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (BTN_Friendship.IsValid())
	{
		auto FriendDoc = UB2UIDocHelper::GetDocFriend();
		if (FriendDoc)
		{
			bool IsFriendAble = FriendDoc->IsFriendAskable(CachedAccountID);
			BTN_Friendship->SetIsEnabled(IsFriendAble);
		}
	}

	SetUserBlockState(IsBlockingUser);
}

void UB2UIUserInfoActionPopup::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Visit);
	GET_SLOT(UB2Button, BTN_Friendship);
	GET_SLOT(UB2Button, BTN_VisitGuild);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_UserBlock);

	GET_SLOT(UTextBlock, TB_UserName);
	GET_SLOT(UTextBlock, TB_Visit);
	GET_SLOT(UTextBlock, TB_Friendship);
	GET_SLOT(UTextBlock, TB_VisitGuild);
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_UserBlock);
}

void UB2UIUserInfoActionPopup::UpdateStaticText()
{
	if (TB_Visit.IsValid())
	{
		TB_Visit->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_UserInfo_Visit")));
	}
	if (TB_Friendship.IsValid())
	{
		TB_Friendship->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_UserInfo_ReqFriend")));
	}
	if (TB_VisitGuild.IsValid())
	{
		TB_VisitGuild->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_VisitGuild")));
	}
	if (TB_Title.IsValid())
	{
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));
	}

	if (TB_UserBlock.IsValid())
	{
		TB_UserBlock->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), IsBlockingUser ? TEXT("ChattingUnBlock") : TEXT("ChattingBlock")));
	}
}

void UB2UIUserInfoActionPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Visit, &UB2UIUserInfoActionPopup::OnClickBtnVisit);
	BIND_CLICK_FUNC_TO_BTN(BTN_Friendship, &UB2UIUserInfoActionPopup::OnClickBtnFriendship);
	BIND_CLICK_FUNC_TO_BTN(BTN_VisitGuild, &UB2UIUserInfoActionPopup::OnClickBtnVisitGuild);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIUserInfoActionPopup::OnClickBtnClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_UserBlock, &UB2UIUserInfoActionPopup::OnClickBtnUserBlock);
}

void UB2UIUserInfoActionPopup::CloseWidgetDelegate()
{
	OnClickBtnClose();
}

void UB2UIUserInfoActionPopup::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UIUserInfoActionPopup);

	DeliveryAskFriendTicket = DeliveryAskFriendClass<>::GetInstance().Subscribe(
	USE_CAPTURE_OBJECT_AND_TICKET(DeliveryAskFriend)
		Capture->OnDeliveryAskFriend();
	END_CAPTURE_OBJECT());

	HandleServerError5503Ticket = HandleServerError5503Class<>::GetInstance().Subscribe([this]() { DevShowError("5503"); });
}

void UB2UIUserInfoActionPopup::UnsubscribeEvent()
{
	DeliveryAskFriendClass<>::GetInstance().Unsubscribe(DeliveryAskFriendTicket);
	HandleServerError5503Class<>::GetInstance().Unsubscribe(HandleServerError5503Ticket);
}

void UB2UIUserInfoActionPopup::OnClickBtnVisit()
{
	FindAccountRequestEventClass<int64, EPCClass, EPCClass, EPCClass>::GetInstance().Signal(CachedAccountID, EPCClass::EPC_End, EPCClass::EPC_End, EPCClass::EPC_End);
	CloseMe();
}

void UB2UIUserInfoActionPopup::OnClickBtnFriendship()
{
	auto SomeDoc = UB2UIDocHelper::GetDocSome();
	auto FriendDoc = UB2UIDocHelper::GetDocFriend();
	if (!SomeDoc && !FriendDoc)
		return;

 	if (FriendDoc->GetFriendSendInfo(CachedAccountID))
 	{
 		DevShowError(TEXT("5503"));			//이미친구신청을 했을때
 		return;
 	}
 	else if (FriendDoc->GetFriendSendListInfo().Num() >= BladeIIGameImpl::GetClientDataStore().GetMaxSendAskFriend())
 	{
 		DevShowError(TEXT("5504"));			//보낸친구가 오버 할경우
 		return;
 	}

	data_trader::Retailer::GetInstance().RequestAskFriend(CachedAccountID);
	//CloseMe();
}

void UB2UIUserInfoActionPopup::OnClickBtnVisitGuild()
{
	if (CachedGuildID > 0)
	{
		CloseMe();
		UB2UIManager::GetInstance()->CloseUI(UIFName::Chatting);
		data_trader::Retailer::GetInstance().RequestGuildDetailInfo(CachedGuildID);
	}
}

void UB2UIUserInfoActionPopup::OnClickBtnClose()
{
	CloseMe();
}

void UB2UIUserInfoActionPopup::OnClickBtnUserBlock()
{
	if (ButtonDelay > 0.5f)
	{
		if (IsBlockingUser)
		{
			BladeIIGameImpl::GetChatStore().RequestUnBlockUser(CachedAccountID);
		}
		else
		{
			BladeIIGameImpl::GetChatStore().RequestBlockUser(CachedAccountID);
		}

		ButtonDelay = 0.0f;
	}
}

void UB2UIUserInfoActionPopup::CloseMe()
{ // 다른 것의 파트임. 숨기기만
	this->SetVisibility(ESlateVisibility::Collapsed);
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
}

void UB2UIUserInfoActionPopup::OnDeliveryAskFriend()
{
	auto SomeDoc = UB2UIDocHelper::GetDocSome();
	auto FriendDoc = UB2UIDocHelper::GetDocFriend();
	if (!SomeDoc && !FriendDoc)
		return;

	int64 AccountId = SomeDoc->GetFindUserAccountID();
	FB2UserPtr FriendInfo(new b2network::B2User);

	FriendDoc->AddFriendSendInfo(AccountId, FriendInfo);

	ABladeIIGameMode::SetHUDCenterMessage(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("FriendAdd")), 1.0f);
}

void UB2UIUserInfoActionPopup::SetUserBlockState(bool Isblock)
{
	IsBlockingUser = Isblock;

	if (TB_UserBlock.IsValid())
	{
		TB_UserBlock->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), IsBlockingUser ? TEXT("ChattingUnBlock") : TEXT("ChattingBlock")));
	}
}
