#include "BladeII.h"
#include "B2UIChatMessage.h"
#include "B2UIManager.h"
#include "B2Chatting.h"
#include "B2UIGuildMark.h"
#include "B2GameInstance.h"

UB2UIChatMessage::UB2UIChatMessage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedMessageType = EB2ChatType::End;
}

void UB2UIChatMessage::Init()
{
	Super::Init();

	this->SetVisibility(ESlateVisibility::Visible);
}

void UB2UIChatMessage::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_UserName);
	GET_SLOT(UTextBlock, TB_Message);
	GET_SLOT(UTextBlock, TB_ElapsedTime);
	GET_SLOT(UB2Button, BTN_UserInfoPopup);
	if (BTN_UserInfoPopup.IsValid())
	{
		BTN_UserInfoPopup->SetVisibility(ESlateVisibility::HitTestInvisible); // ä�� �г� ��ũ�ѹڽ� �ڵ鸵�� ���� ��ư�� HitTestInvisible ��
	}

	GET_SLOT(UTextBlock, TB_GuildName);
	GET_SLOT(UB2UIGuildMark, GuildMark);
	GET_SLOT(UPanelWidget, X_GuildInfo);

	GET_SLOT(UTextBlock, TB_UserCharacterMaxLevel);
}

void UB2UIChatMessage::UpdateStaticText()
{

}

void UB2UIChatMessage::BindDelegates()
{
	// ä�� �г� ��ũ�� �ڵ鸵�� ���� ��ư Ŭ�� ó���� ����..
	//BIND_CLICK_FUNC_TO_BTN(BTN_UserInfoPopup, &UB2UIChatMessage::OnClickUserInfoPopup);
}

void UB2UIChatMessage::OnClickedSenderBTNArea()
{ // ManualScrollBox ����� �Ϻη� �Ҹ��� �������̽�
	Super::OnClickedSenderBTNArea();

	OnClickUserInfoPopup();// ���� Ŭ�� �ڵ鸵
}
void UB2UIChatMessage::OnPressedSenderBTNArea()
{// ManualScrollBox ����� �Ϻη� �Ҹ��� �������̽�
	Super::OnPressedSenderBTNArea();
}
void UB2UIChatMessage::OnReleasedSenderBTNArea()
{// ManualScrollBox ����� �Ϻη� �Ҹ��� �������̽�
	Super::OnReleasedSenderBTNArea();
}

void UB2UIChatMessage::OnClickUserInfoPopup()
{	
	OnChatUserNameClicked.ExecuteIfBound(CachedUserAccountID, CachedGuildID, CachedUserNickName);
}

void UB2UIChatMessage::SetChatMessageInfo(const FB2ChatMessage& InMessageInfo)
{
	Super::SetChatMessageInfo(InMessageInfo);

	CachedUserAccountID = InMessageInfo.UserAcccountID;
	CachedMessageType = InMessageInfo.MessageType;
	CachedUserNickName = InMessageInfo.SendUserNick.ToString(); 
	CachedNoticeType = InMessageInfo.NoticeType;
	CachedMessageTime = InMessageInfo.MessageTime;
	CachedGuildID = InMessageInfo.HasGuildInfo() ? InMessageInfo.GuildInfo->GuildId : 0;
	int32 CharacterMaxLevel = InMessageInfo.CharacterMaxLevel;
	bool HasGuildInfo = InMessageInfo.HasGuildInfo();
	if (HasGuildInfo == true)
	{
		if (X_GuildInfo.IsValid())
			X_GuildInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		const FB2ChatGuildInfoPtr &GuildInfo = InMessageInfo.GuildInfo;
		if(GuildMark.IsValid())
			GuildMark->SetGuildMark(GuildInfo->PatternIndex, GuildInfo->PetternColor, GuildInfo->BGImage, GuildInfo->BGColor);
		
		if (TB_GuildName.IsValid())
			TB_GuildName->SetText(GuildInfo->Name);
	}
	else
	{
		if (X_GuildInfo.IsValid())
			X_GuildInfo->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (TB_UserName.IsValid())
	{
		// ChatType �� ���� ������ �̸��� �ְų�.. ������ SendUserNick �� �װ� ���ų�..?
		if (CachedMessageType == EB2ChatType::Notice || CachedMessageType == EB2ChatType::GuildNotice)
		{
			TB_UserName->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")));
		}
		else
		{
			TB_UserName->SetText(InMessageInfo.SendUserNick);
		}
	}
	if (TB_Message.IsValid())
	{
		TB_Message->SetText(InMessageInfo.MessageText);
	}

	if (TB_UserCharacterMaxLevel.IsValid())
	{
		TB_UserCharacterMaxLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("chatting_UserLevel")), FText::AsNumber(CharacterMaxLevel)));
	}

	UpdateMessageTime();
}

void UB2UIChatMessage::UpdateMessageTime()
{
	if (TB_ElapsedTime.IsValid())
	{
		if (CachedMessageType != EB2ChatType::GuildNotice && CachedMessageType != EB2ChatType::Guild)
		{
			int32 ElapsedSec = static_cast<int32>(UB2GameInstance::GetUTCNow().ToUnixTimestamp() - CachedMessageTime);
			FTimespan Span = FTimespan::FromSeconds(ElapsedSec);
			TB_ElapsedTime->SetText(GetElapsedTime(Span));
		}
		else
		{	// ��� �޼����� ��� �ð� ����� �ٸ��� �Ѵ�.
			FTimespan RemainTime = UB2GameInstance::GetUTCNow() - FDateTime::FromUnixTimestamp(CachedMessageTime / 1000);
			TB_ElapsedTime->SetText(GetElapsedTime(RemainTime));
		}
	}
}