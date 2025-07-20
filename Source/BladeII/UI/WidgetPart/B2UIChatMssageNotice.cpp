// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIChatMssageNotice.h"
#include "B2UIManager.h"
#include "B2Chatting.h"

void UB2UIChatMssageNotice::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_UserName);	
	GET_SLOT(UTextBlock, TB_ElapsedTime);
	GET_SLOT(UB2RichTextBlock, TB_Message);
}

void UB2UIChatMssageNotice::UpdateStaticText()
{

}

void UB2UIChatMssageNotice::BindDelegates()
{

}

void UB2UIChatMssageNotice::SetChatMessageInfo(const struct FB2ChatMessage& InMessageInfo)
{
	Super::SetChatMessageInfo(InMessageInfo);
	if (TB_UserName.IsValid())
	{
		// ChatType 俊 蝶扼辑 沥秦柳 捞抚阑 持芭唱.. 郴瘤绰 SendUserNick 俊 弊霸 甸绢啊芭唱..?
		if (InMessageInfo.MessageType == EB2ChatType::Notice || InMessageInfo.MessageType == EB2ChatType::GuildNotice)
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

	if (TB_ElapsedTime.IsValid())
	{
		const int32 ElapsedSec = InMessageInfo.GetMessageElapsedTime();
		FTimespan Span = FTimespan::FromSeconds(ElapsedSec);
		TB_ElapsedTime->SetText(GetElapsedTime(Span));
	}
}