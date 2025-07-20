// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIKakaoFriendInviteRow.generated.h"

struct FB2KakaoFriendInfo;
class UB2UIFriendKakaoInvite;
/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIKakaoFriendInviteRow : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	UB2UIKakaoFriendInviteRow(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	void SetUserInfo(const FB2KakaoFriendInfo &UserInfo);
	void SetUserProfileImage(class UTexture2DDynamic *Tex);

	void SetOwnerUI(UB2UIFriendKakaoInvite *InOwner);

	int32 GetRowIndex() { return RowIndex; }
	void SetRowIndex(const int32 Index) { RowIndex = Index; }

	void SetKakaoInviteBtn(bool bEnabled);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnInvite();
private:
	TWeakObjectPtr<UImage> IMG_Character;
	TWeakObjectPtr<UB2Button> BTN_WholeArea;
	TWeakObjectPtr<UB2Button> BTN_KakaoInvite;
	TWeakObjectPtr<UTextBlock> TB_KakaoInvite;
	TWeakObjectPtr<UTextBlock> TB_PlayerName;

	UB2UIFriendKakaoInvite *OwnerUI;

	int32 RowIndex;
};
