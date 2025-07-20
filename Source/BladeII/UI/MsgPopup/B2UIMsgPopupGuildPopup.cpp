#include "B2UIMsgPopupGuildPopup.h"
#include "B2UIManager.h"
#include "B2UIGuildMark.h"



void UB2UIMsgPopupGuildPopup::CacheAssets()
{
	GET_SLOT(UB2UIGuildMark, UIP_LargeGuildMark)
	if(UIP_LargeGuildMark.IsValid())
		UIP_LargeGuildMark->Init();
	GET_SLOT(UB2RichTextBlock, RTB_GuildName)
	GET_SLOT(UB2Button, BTN_Confirm)

	GET_SLOT(UB2RichTextBlock, TB_Content)
	GET_SLOT(UTextBlock, TB_Notice)
	GET_SLOT(UTextBlock, TB_BtnPositive)

}

void UB2UIMsgPopupGuildPopup::UpdateStaticText()
{
	if (TB_Content.IsValid())
		TB_Content->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_CreateMsg2")));

	if (TB_Notice.IsValid())
		TB_Notice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));

	if (TB_BtnPositive.IsValid())
		TB_BtnPositive->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
}

void UB2UIMsgPopupGuildPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIMsgPopupGuildPopup::OnClickConfirm)
}

void UB2UIMsgPopupGuildPopup::CloseWidgetDelegate()
{
	OnClickConfirm();
}

void UB2UIMsgPopupGuildPopup::OnClickConfirm()
{
	ButtonDelegate.ExecuteIfBound();
}

void UB2UIMsgPopupGuildPopup::SetGuildMark(int32 GuildImage, int32 GuildColor, int32 GuildBG, int32 GuildBGColor)
{
	if (UIP_LargeGuildMark.IsValid())
		UIP_LargeGuildMark->SetGuildMark(GuildImage, GuildColor, GuildBG, GuildBGColor);

}

void UB2UIMsgPopupGuildPopup::SetGuildName(const FString& GuildName)
{
	if (RTB_GuildName.IsValid())
		RTB_GuildName->SetText(FText::FromString(GuildName));
}

void UB2UIMsgPopupGuildPopup::AddHandler(const FMsgPopupOnClick& Handler)
{
	ButtonDelegate = Handler;
}

void UB2UIMsgPopupGuildPopup::Init()
{
	Super::Init();

	SetOpenState(EUIOpenState::Opened);
}

