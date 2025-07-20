// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/WidgetPart/B2UIChatMessageBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIChatMssageGuildSupport.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIChatMssageGuildSupport : public UB2UIChatMessageBase
{
	GENERATED_BODY()
	
public:
	virtual void SetChatMessageInfo(const struct FB2ChatMessage& InMessageInfo) override;
	virtual void UpdateMessageTime() override;
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnClickBtnSupport();
private:
	TWeakObjectPtr<UTextBlock> TB_UserName;
	TWeakObjectPtr<UB2RichTextBlock> TB_Message;

	TWeakObjectPtr<UTextBlock> TB_TitleAmount;
	TWeakObjectPtr<UTextBlock> TB_CurAmount;

	TWeakObjectPtr<UTextBlock> TB_Support;
	TWeakObjectPtr<UB2Button> BTN_Support;

	TWeakObjectPtr<UTextBlock> TB_ReceivedSupport;
	TWeakObjectPtr<UTextBlock> TB_SupportGuage;
	TWeakObjectPtr<UProgressBar> PB_SupportGuage;

	TWeakObjectPtr<class UB2DynItemIcon_GuildSupport> Icon;

	int64 CachedMessageTime;
	int64 OwnerAccountID;
	int32 CachedTempleteID;
};
