// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIChatMessageBase.h"
#include "B2UIChatMssageNotice.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIChatMssageNotice : public UB2UIChatMessageBase
{
	GENERATED_BODY()
	
public:
	virtual void SetChatMessageInfo(const struct FB2ChatMessage& InMessageInfo) override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

private:
	TWeakObjectPtr<UTextBlock> TB_UserName;
	TWeakObjectPtr<UTextBlock> TB_ElapsedTime;
	TWeakObjectPtr<UB2RichTextBlock> TB_Message;
};
