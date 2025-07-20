#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIFriendRow.generated.h"

/*
* Lobby Main UI
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIFriendRow : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	void CheckSocialPointSendTime();

public:
	virtual void Init() override;
	void SetOwnerUI(class UB2UIFriend* pOwner);
	void SetFriendContent(FB2FriendPtr ptrFriend);
	void SetFriendContent(FB2UserPtr ptrUser);
	void SetFriendContent(FB2CharacterSummaryPtr ptrCharacterSummary);
	int64 GetFriendAccountId();
	void CheckButtonState();
	bool IsSocialPointSendable();
	void SetSendPointTime(int64 nUtcTimeSec);

	void UpdateLastLogoutTime();

	UFUNCTION()
		void OnWholeAreaBtnClick();

	UFUNCTION()
		void OnAskBtnClick();

	UFUNCTION()
		void OnSendPointBtnClick();

	UFUNCTION()
		void OnDeleteBtnClick();

	UFUNCTION()
		void OnCancelBtnClick();

	UFUNCTION()
		void OnRejectBtnClick();

	UFUNCTION()
		void OnAcceptBtnClick();

	UFUNCTION()
		void OnConfirmDelete();

private:
	void SetLastLogOutTime(int64 LogOutTime);

protected:
	TWeakObjectPtr<UB2Image> IMG_Character;

	TWeakObjectPtr<UTextBlock> TB_Guild;
	TWeakObjectPtr<UTextBlock> TB_PlayerLevel;
	TWeakObjectPtr<UTextBlock> TB_PlayerName;
	TWeakObjectPtr<UTextBlock> TB_SendPointTimeBtn;
	TWeakObjectPtr<UTextBlock> TB_LoginTime;

	TWeakObjectPtr<UB2Button> BTN_WholeArea;
	TWeakObjectPtr<UB2Button> BTN_Ask;
	TWeakObjectPtr<UB2Button> BTN_SendPoint;
	TWeakObjectPtr<UB2Button> BTN_SendPointTime;
	TWeakObjectPtr<UB2Button> BTN_Delete;
	TWeakObjectPtr<UB2Button> BTN_Cancel;
	TWeakObjectPtr<UB2Button> BTN_Reject;
	TWeakObjectPtr<UB2Button> BTN_Accept;
	TWeakObjectPtr<UB2Button> BTN_KakaoInvite;
	
	TWeakObjectPtr<UTextBlock> TB_Request;
	TWeakObjectPtr<UTextBlock> TB_SendSocialPoint;
	TWeakObjectPtr<UTextBlock> TB_Cancel;
	TWeakObjectPtr<UTextBlock> TB_ConnectTime;
	TWeakObjectPtr<UTextBlock> TB_Accept;
	TWeakObjectPtr<UTextBlock> TB_Reject;
	TWeakObjectPtr<UTextBlock> TB_Revocation;
	

	TWeakObjectPtr<UB2UIWidgetBase> UIP_Reddot_NewFriend;
private:
	int64 m_nAccountId;
	UB2UIFriend* m_pOwnerUI;
	int64 m_nSendPointTime;
	int64 m_nLastLogoutTime;
	FTimerHandle SocialPointTimer;
};