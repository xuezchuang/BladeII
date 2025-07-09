// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIMsgPopupBase.h"
#include "B2UIGuildUserMsg.generated.h"

DECLARE_DELEGATE(FGuildUserPopup)
UCLASS()
class BLADEII_API UB2UIGuildUserMsg : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetGuildUserMsg(b2network::B2GuildMemberPtr GuildMemberInfo, FText ContentText, FGuildUserPopup Delegate);

protected:
	UFUNCTION()
		void OnClickBTN_Confirm();
	UFUNCTION()
		void OnClickBTN_Cancle();

private:
	TWeakObjectPtr<UB2RichTextBlock>	TB_Content;
	TWeakObjectPtr<UImage> BottomBGImage;
	TWeakObjectPtr<UImage> TitleBGImage;
	TWeakObjectPtr<UTextBlock> TextBlock_2332;
	TWeakObjectPtr<UImage> IMG_Content;
	TWeakObjectPtr<UB2Button> BTN_Confirm;
	TWeakObjectPtr<UB2Button> BTN_Cancle;
	TWeakObjectPtr<UTextBlock> TB_BtnPositive;
	TWeakObjectPtr<class UB2UIGuildMemberSlot> UIP_GuildMemberSlot3;

	FGuildUserPopup PopupDelegate;
};
