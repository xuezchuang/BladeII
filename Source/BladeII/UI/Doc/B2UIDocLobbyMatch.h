// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "BladeIIPlayer.h"
#include "B2UIDocLobbyMatch.generated.h"

USTRUCT()
struct FLobbyMatchRemotePlayerInfo
{
	GENERATED_USTRUCT_BODY()

		FLobbyMatchRemotePlayerInfo()
	{
		RemoteNetId = INDEX_NONE;
		RemoteAccountId = 0;
		RemoteAccountInfo = nullptr;
		ReservedRemotePCClass = EPCClass::EPC_End;
		RemotePlayerStartIndex = 0;
	}

	int32 RemoteNetId;
	int64 RemoteAccountId;
	FB2FindAccountInfoPtr RemoteAccountInfo;
	EPCClass ReservedRemotePCClass;
	uint8 RemotePlayerStartIndex;
};

USTRUCT()
struct FLobbyMatchRemotePlayerMatchGradeInfo
{
	GENERATED_USTRUCT_BODY()

		FLobbyMatchRemotePlayerMatchGradeInfo()
	{
		BattleGrade = 10;
		BattleStarCount = 0;
	}

	int32 BattleGrade;
	int32 BattleStarCount;
};


/* This is the place where you can put "SOME" fucking data that you have no idea of where to put shits in. */
UCLASS()
class BLADEII_API UB2UIDocLobbyMatch : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocLobbyMatch(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocLobbyMatch();

	virtual void Init() override;
	virtual void Destroy() override;

	void					ClearRemotePlayerInfoData();
	void					SetRemotePlayerInfoData(uint32 NetId, int64 AccountId, FB2FindAccountInfoPtr AccountInfoPtr, EPCClass ReservedPCClass, uint8 PlayerStartIndex = 0);
	uint32 GetRemoteNetIdByIndex(uint32 nIndex);
	FB2FindAccountInfoPtr	GetRemoteAccountInfo(uint32 NetId);
	FB2FindAccountInfoPtr	GetRemoteAccountInfoByIndex(uint32 nIndex);
	EPCClass GetReservedRemotePCClass(uint32 NetId);
	EPCClass GetReservedRemotePCClassByIndex(uint32 nIndex);
	uint8 GetRemotePlayerStartIndex(uint32 NetId);
	uint8 GetRemotePlayerStartIndexByIndex(uint32 nIndex);
	int32 GetRemotePlayerInfoDataCount();
	const TMap<uint32, FString>& GetMatchToken() const { return LobbyMatchPlayerToken; }

	void SetTokenData(uint32 NetId, const FString& MatchToken);
	void SetGradeInfoData(uint32 NetId, int32 BattleGrade, int32 BattleStarCount);
	int32 GetBattleGrade(uint32 NetId);
	int32 GetBattleStarCount(uint32 NetId);

	int64	GetRemoteAccountId(uint32 NetId);
	int64	GetRemoteAccountIdByIndex(uint32 nIndex);

	void  SetInvitationInfo(int32 InvitationType, const FString& Inviter, const FString& Invitee, const FString& InvitationRef, b2network::B2RoomLocationPtr RoomInfoPtr, bool bMyInvitation);
	void  ClearInvitationInfo();
	const FB2InvitationInfo& GetInvitationInfo() { return InvitationInfo; }
	bool  HasValidInvitation() { return InvitationInfo.IsValid(); }

	EInvitationState GetInvitationState() { return InvitationInfo.State; }
	void SetInvitationState(EInvitationState InvitationState) { InvitationInfo.State = InvitationState; }

	float GetInviterTimeout() const { return InviterTimeout; }
	float GetAcceptTimeout() const { return AutoDeclineSeconds; }

	DECLARE_UIDOC_FIELD(uint32 ,	PeerType)
	DECLARE_UIDOC_FIELD(FString,	RoomId)
	DECLARE_UIDOC_FIELD(int32,		HostId)
	DECLARE_UIDOC_FIELD(FString,	RoomToken)
	DECLARE_UIDOC_FIELD(int32,		MyNetId)
	DECLARE_UIDOC_FIELD(uint8,		MyStartIndex)

	DECLARE_UIDOC_FIELD(float,		InvitationBeginTime)

public:
	/** Cached from Lobby for some network game mode that matching process is done at Lobby, and is used for skill anim pre-loading.
	 * It is nothing to do with UI, but place it here as here is a good place and some other info asset pre-loading already uses data here. */
	TArray<FCombinedPCSkillAnimID> MatchedRemotePlayerSkillAnims;


	FB2InvitationInfo	InvitationInfo;
	float				InviterTimeout;
	float				AutoDeclineSeconds;

private:
	TArray<FLobbyMatchRemotePlayerInfo> LobbyMatchRemotePlayerInfoArray;
	TMap<uint32, FLobbyMatchRemotePlayerMatchGradeInfo> LobbyMatchRemotePlayerMatchGradeInfoArray;
	TMap<uint32, FString> LobbyMatchPlayerToken;
};