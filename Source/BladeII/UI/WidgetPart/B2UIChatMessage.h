#pragma once

#include "B2UIChatMessageBase.h"
#include "../../Common/B2Chatting.h"
#include "B2UIChatMessage.generated.h"

/*
 * Single chatting message UI, dynamically created in UIChatting frame
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIChatMessage : public UB2UIChatMessageBase
{
	GENERATED_BODY()

protected:

	TWeakObjectPtr<UTextBlock> TB_UserName;
	TWeakObjectPtr<UTextBlock> TB_Message;
	TWeakObjectPtr<UTextBlock> TB_ElapsedTime;

	TWeakObjectPtr<UB2Button> BTN_UserInfoPopup;

	TWeakObjectPtr<UTextBlock> TB_GuildName;
	TWeakObjectPtr<class UB2UIGuildMark> GuildMark;
	TWeakObjectPtr<UPanelWidget> X_GuildInfo;


	EB2ChatType CachedMessageType;
	EB2ChatNoticeType CachedNoticeType;

	FString CachedUserNickName;	

	int64 CachedUserAccountID;
	int64 CachedMessageTime;
	int64 CachedGuildID;

	TWeakObjectPtr<UTextBlock> TB_UserCharacterMaxLevel;

public:
	UB2UIChatMessage(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	virtual void UpdateMessageTime() override;
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	// Button input handling interfaces for ManualScrollBox feature.
	virtual void OnClickedSenderBTNArea() override;
	virtual void OnPressedSenderBTNArea() override;
	virtual void OnReleasedSenderBTNArea() override;

	UFUNCTION()
	void OnClickUserInfoPopup();

public:
	virtual void SetChatMessageInfo(const struct FB2ChatMessage& InMessageInfo) override;
};