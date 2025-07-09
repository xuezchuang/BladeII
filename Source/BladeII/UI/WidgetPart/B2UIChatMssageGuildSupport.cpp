// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIChatMssageGuildSupport.h"
#include "B2DynItemIcon_GuildSupport.h"
#include "B2UIManager.h"
#include "B2Chatting.h"
#include "Retailer.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIChatMssageGuildSupport::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_UserName);
	GET_SLOT(UB2RichTextBlock, TB_Message);

	GET_SLOT(UTextBlock, TB_TitleAmount);
	GET_SLOT(UTextBlock, TB_CurAmount);

	GET_SLOT(UTextBlock, TB_Support);
	GET_SLOT(UB2Button, BTN_Support);

	GET_SLOT(UTextBlock, TB_ReceivedSupport);
	GET_SLOT(UTextBlock, TB_SupportGuage);
	GET_SLOT(UProgressBar, PB_SupportGuage);

	GET_SLOT(UB2DynItemIcon_GuildSupport, Icon);
	if (Icon.IsValid())
		Icon->Init();
}

void UB2UIChatMssageGuildSupport::UpdateStaticText()
{
	if (TB_TitleAmount.IsValid())
		TB_TitleAmount->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EssenceRequestMessage_PresentPossession")));
	if (TB_ReceivedSupport.IsValid())
		TB_ReceivedSupport->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EssenceRequestMessage_AcceptEssence")));
	if (TB_Support.IsValid())
		TB_Support->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EssenceRequestMessage_SupportBtn")));
}

void UB2UIChatMssageGuildSupport::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Support, &UB2UIChatMssageGuildSupport::OnClickBtnSupport);
}

void UB2UIChatMssageGuildSupport::SetChatMessageInfo(const struct FB2ChatMessage& InMessageInfo)
{
	Super::SetChatMessageInfo(InMessageInfo);

	//if (InMessageInfo.HasGuildInfo() == false)
	//	return;

	//OwnerAccountID = InMessageInfo.UserAcccountID;
	//CachedChatID = InMessageInfo.ChatID;
	//CachedTempleteID = InMessageInfo.GuildInfo->SupportTempleteID;
	//FB2Item CurItem;
	//BladeIIGameImpl::GetGuildDataStore().GetGuildSupportItem(CachedTempleteID, CurItem);

	//if (TB_UserName.IsValid())
	//{
	//	TB_UserName->SetText(InMessageInfo.SendUserNick);
	//}
	//if (TB_Message.IsValid())
	//{
	//	EItemInvenType InvenType = CurItem.InventoryType;

	//	FText GradeName = FText::AsNumber(CurItem.StarGrade);
	//	FText ClassName = BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(IntToPCClass(CurItem.AllowedPCClass));
	//	FText CategoryName = BladeIIGetLOCText(B2LOC_CAT_GENERAL,
	//		(InvenType == EItemInvenType::EIIVT_Weapon) ? FString(TEXT("LobbyInvenText_Weapon")) : (
	//		(InvenType == EItemInvenType::EIIVT_Protection) ? FString(TEXT("LobbyInvenText_Protection")) : FString(TEXT("LobbyInvenText_Accessory"))
	//			));

	//	TB_Message->SetText(
	//		FText::Format(
	//			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("EssenceRequestMessage_RequestMessage")),
	//			ClassName,
	//			GradeName,
	//			CategoryName
	//		)
	//	);
	//}

	//if (Icon.IsValid() && CachedTempleteID > 0)
	//{
	//	Icon->UpdateItemData(CurItem);
	//}

	//if (PB_SupportGuage.IsValid())
	//{
	//	float Percent = InMessageInfo.GuildInfo->SupportCount / (float)InMessageInfo.GuildInfo->SupportMaxCount;
	//	PB_SupportGuage->SetPercent(Percent);
	//}
	//if (TB_SupportGuage.IsValid())
	//{
	//	FText CountText = FText::FromString(FString::Printf(TEXT("%d/%d"), InMessageInfo.GuildInfo->SupportCount, InMessageInfo.GuildInfo->SupportMaxCount));
	//	TB_SupportGuage->SetText(CountText);
	//}

	//UpdateMessageTime();

	//if (BTN_Support.IsValid())
	//{
	//	BTN_Support->SetVisibility(InMessageInfo.bIsMyMessage ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	//}

	//if(TB_TitleAmount.IsValid())
	//	TB_TitleAmount->SetVisibility(InMessageInfo.bIsMyMessage ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
	//if (TB_CurAmount.IsValid())
	//	TB_CurAmount->SetVisibility(InMessageInfo.bIsMyMessage ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
}

void UB2UIChatMssageGuildSupport::OnClickBtnSupport()
{ 
	int32 DefaultCount = 1;
	data_trader::Retailer::GetInstance().RequestGuildSupport(OwnerAccountID, CachedChatID, DefaultCount);
}

void UB2UIChatMssageGuildSupport::UpdateMessageTime()
{
	int32 HasCount = BladeIIGameImpl::GetLocalCharacterData().GetUserAllItems().FindItemOfTypeCount(CachedTempleteID);
	bool IsEnableSupport = HasCount > 0;
	if (TB_CurAmount.IsValid())
	{
		TB_CurAmount->SetText(FText::AsNumber(HasCount));
	}

	if (BTN_Support.IsValid())
	{
		BTN_Support->SetIsEnabled(IsEnableSupport);
	}
}