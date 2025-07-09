// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Doc/B2UIDocBase.h"
#include "B2UIDocFriend.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDocFriend : public UB2UIDocBase
{
	GENERATED_BODY()
	
public:
	UB2UIDocFriend(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocFriend();
	
	virtual void Init() override;
	virtual void Destroy() override;

	TMap<int64, FB2FriendPtr> GetFriendList();
	FB2FriendPtr	GetFriendInfo(int64 AccountId);
	void			AddFriendInfo(int32 AccountId,FB2FriendPtr FriendInfo);
	TMap<int64, FB2UserPtr> GetFriendSendListInfo();
	FB2UserPtr		GetFriendSendInfo(int64 AccountId);
	void			AddFriendSendInfo(int32 AccountId, FB2UserPtr FriendInfo);
	void			RemoveFriendSendInfo(int32 AccountId);

	TMap<int64, FB2UserPtr> GetAllReceiveAskFriendList();
	FB2UserPtr		GetReceiveAskFriendInfo(int64 AccountId);
	void			AddReceiveAskFriendInfo(int32 AccountId, FB2UserPtr FriendInfo);

	bool IsFriendAskable(int64 AccountId);
	bool IsMyFriend(int64 AccountId);
	
	bool GetAbleToSendFriendPoint(); // it used to reddot.
	bool GetIsNewFriend();
	bool GetIsAskFriend();
	void OnForcedFriendSocialPointInfo(int64 AccountId, int64 LastSendPointTime);
	void OnForcedAddFriendInfo(int64 AccountId);
	void OnForcedDeleteFriendInfo(int64 AccountId);
	void OnForcedDeleteAskFriend(int64 AccountId);
	void OnForcedRemoveNewFriend(int64 AccountId);

	void GetKakaoFriendList(TArray<FB2KakaoFriendInfo> &OutList);
	void GetKakaoFriendEventList(TArray<FB2KakaoFriendEventInfo> &OutList);

	bool IsGetInviteReward(const int32 RewardIndex);
	bool IsGetJoinerReward(const int32 RewardIndex);
	b2network::B2FriendInviteRewardInfoPtr GetFriendInviteRewardInfo();
	
	void GetInviteFriendInfo(const int32 RowIndex, FB2KakaoFriendInfo &FriendInfo);
	void RemoveInviteFriendInfo(const int32 RowIndex);
	FB2KakaoFriendEventInfo GetRecentlyKakaoEvent();
	bool HasKakaoEvent();

	void RefreshKakaoFriendInfo() { SetKakaoFriendInfoData(); }
	void RefreshKakaoEventInfo() { SetKakaoFriendEventData(); }

	void AddInviteKakaoFriend(const int32 TargetIndex);

private:
	void OnDeliveryGetFriendList(const FB2FriendList& FriendList);
	void OnDeliveryGetAllReceiveAskFriend(const FB2AllReceiveAskFriendList& FriendList);
	void OnDeliveryGetAllSendAskFriend(const FBAllSendAskFriendList& FriendList);
	void OnDeliveryPlatformFriendInviteInfo(const FB2FriendInviteRewardInfoPtr& RewardInfo);
	void OnDeliveryInviteKakaoResult(EKakaoInviteResultCode Code);
	void OnDeliveryInviteReward(const FB2ResponseFriendInviteRewardPtr &RewardInfo);

	void SetKakaoFriendInfoData();
	void SetKakaoFriendEventData();

	void SaveLocalInvitedFriends();
	void LoadLocalInvitedFriends();
public:
	DECLARE_UIDOC_FIELD(bool, IsUpdateFriendInviteRewardInfo)
	DECLARE_UIDOC_FIELD(FString, CurSendInviteFriendUUID)

	DECLARE_UIDOC_FIELD(int32, CurSendInviteFriendRowIndex)

private:
	TMap<int64, FB2FriendPtr>	m_ptrFriendList;
	TMap<int64, FB2UserPtr>		m_ptrFriendSendList;
	TMap<int64, FB2UserPtr>		m_ptrReceiveAskFriendList;
	TArray<FB2KakaoFriendInfo>	m_KakaoFriendInfoList;

	TSet<FString> m_KakaoInvitedSet;
	TArray<FString> m_KakaoInvietedInfoList;

	TArray<FB2KakaoFriendEventInfo>	m_KakaoFriendEventInfoList;

	b2network::B2FriendInviteRewardInfoPtr InviteRewardInfo;

protected:
	int32 DeliveryGetFriendListTicket;
	int32 DeliveryGetAllReceiveAskFriendTicket;
	int32 DeliveryGetAllSendAskFriendTicket;
	int32 CacheSendAskFriendAccountIdTicket;
	int32 RemoveSendAskFriendAccountIdTicket;
	int32 DeliveryPlatformFriendInviteInfoTicket;
	int32 DeliveryKakaoInviteFriendTicket;
	int32 DeliveryFriendInviteCountUpTicket;
	int32 DeliveryFriendJoinerCountUpTicket;
	int32 DeliveryFriendInviteRewardTicket;
};
