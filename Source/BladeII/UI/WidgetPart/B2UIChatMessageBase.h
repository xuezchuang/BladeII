// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIChatMessageBase.generated.h"

/**
 * 
 */
namespace FChatMessageType
{
	enum Type
	{
		None = 0,
		PublicNotice,
		LocalNotice,
		MyChat,
		OtherChat,
		GuildSupport,
	};
}

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnChatUserNameClickedDelegate, const int64, AccountID, const int64, GuildID, const FString&, UserNickName);

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIChatMessageBase : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void SetChatMessageInfo(const struct FB2ChatMessage& InMessageInfo);

	virtual void UpdateMessageTime();

	int32 GetSlotType() { return CachedSlotType; }
	void SetSlotType(const int32 InSlotType) { CachedSlotType = InSlotType; }

	int64 GetChatID() { return CachedChatID; }
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	FText GetElapsedTime(const int32 ElapsedSec);
	FText GetElapsedTime(const FTimespan RemainTime);
public:
	FOnChatUserNameClickedDelegate OnChatUserNameClicked;
protected:
	int32 CachedSlotType;
	int64 CachedChatID;
};
