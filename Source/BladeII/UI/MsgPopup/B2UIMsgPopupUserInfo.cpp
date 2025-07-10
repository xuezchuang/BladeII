#include "B2UIMsgPopupUserInfo.h"


void UB2UIMsgPopupUserInfo::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Character);
	GET_SLOT(UTextBlock, TB_Guild);
	GET_SLOT(UTextBlock, TB_PlayerLevel);
	GET_SLOT(UTextBlock, TB_PlayerName);
	GET_SLOT(UB2RichTextBlock, TB_AddComment);
}

void UB2UIMsgPopupUserInfo::SetUserInfo(UMaterialInstance* pIconMI, const FText& txtName, const FText& txtLevel, const FText& txtGuild)
{
	if(IMG_Character.IsValid())
		IMG_Character->SetBrushFromMaterial(pIconMI);

	if (TB_PlayerName.IsValid())
		TB_PlayerName->SetText(txtName);

	if (TB_PlayerLevel.IsValid())
		TB_PlayerLevel->SetText(txtLevel);

	if (TB_Guild.IsValid())
		TB_Guild->SetText(txtGuild);

}

void UB2UIMsgPopupUserInfo::SetAddComment(const FText& comment)
{
	if (TB_AddComment.IsValid())
		TB_AddComment->SetText(comment);
}
