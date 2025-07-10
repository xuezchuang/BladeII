#include "B2UIDocLobbyMatch.h"
#include "Event.h"


UB2UIDocLobbyMatch::UB2UIDocLobbyMatch(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	InviterTimeout = 30.f;
	AutoDeclineSeconds = 15.f;
}

UB2UIDocLobbyMatch::~UB2UIDocLobbyMatch()
{
	
}

void UB2UIDocLobbyMatch::Init()
{
	Super::Init();

	ClearRemotePlayerInfoData();
}

void UB2UIDocLobbyMatch::Destroy()
{
	Super::Destroy();
}

void UB2UIDocLobbyMatch::ClearRemotePlayerInfoData()
{
	LobbyMatchRemotePlayerInfoArray.Empty();

	LobbyMatchRemotePlayerMatchGradeInfoArray.Empty();

	LobbyMatchPlayerToken.Empty();
}

void UB2UIDocLobbyMatch::SetRemotePlayerInfoData(uint32 NetId, int64 AccountId, FB2FindAccountInfoPtr AccountInfoPtr, EPCClass ReservedPCClass, uint8 PlayerStartIndex)
{
	FLobbyMatchRemotePlayerInfo RemotePlayerInfo;

	RemotePlayerInfo.RemoteNetId = NetId;
	RemotePlayerInfo.RemoteAccountId = AccountId;
	RemotePlayerInfo.RemoteAccountInfo = AccountInfoPtr;
	RemotePlayerInfo.ReservedRemotePCClass = ReservedPCClass;
	RemotePlayerInfo.RemotePlayerStartIndex = PlayerStartIndex;

	LobbyMatchRemotePlayerInfoArray.Add(RemotePlayerInfo);
}

uint32 UB2UIDocLobbyMatch::GetRemoteNetIdByIndex(uint32 nIndex)
{
	if (nIndex >= uint32(LobbyMatchRemotePlayerInfoArray.Num()))
		return INDEX_NONE;

	return LobbyMatchRemotePlayerInfoArray[nIndex].RemoteNetId;
}

FB2FindAccountInfoPtr UB2UIDocLobbyMatch::GetRemoteAccountInfo(uint32 NetId)
{
	if (NetId == INDEX_NONE)
		return nullptr;

	for (FLobbyMatchRemotePlayerInfo& playerInfo : LobbyMatchRemotePlayerInfoArray)
	{
		if (playerInfo.RemoteNetId == NetId)
			return playerInfo.RemoteAccountInfo;
	}

	return nullptr;
}

FB2FindAccountInfoPtr UB2UIDocLobbyMatch::GetRemoteAccountInfoByIndex(uint32 nIndex)
{
	if(nIndex >= uint32(LobbyMatchRemotePlayerInfoArray.Num()))
		return nullptr;

	return LobbyMatchRemotePlayerInfoArray[nIndex].RemoteAccountInfo;	
}

EPCClass UB2UIDocLobbyMatch::GetReservedRemotePCClass(uint32 NetId)
{
	if (NetId == INDEX_NONE)
		return EPCClass::EPC_End;

	for (FLobbyMatchRemotePlayerInfo& playerInfo : LobbyMatchRemotePlayerInfoArray)
	{
		if (playerInfo.RemoteNetId == NetId)
			return playerInfo.ReservedRemotePCClass;
	}

	return EPCClass::EPC_End;
}

EPCClass UB2UIDocLobbyMatch::GetReservedRemotePCClassByIndex(uint32 nIndex)
{
	if (nIndex >= uint32(LobbyMatchRemotePlayerInfoArray.Num()))
		return EPCClass::EPC_End;

	return LobbyMatchRemotePlayerInfoArray[nIndex].ReservedRemotePCClass;
}

uint8 UB2UIDocLobbyMatch::GetRemotePlayerStartIndex(uint32 NetId)
{
	if (NetId == INDEX_NONE)
		return 0;

	for (FLobbyMatchRemotePlayerInfo& playerInfo : LobbyMatchRemotePlayerInfoArray)
	{
		if (playerInfo.RemoteNetId == NetId)
			return playerInfo.RemotePlayerStartIndex;
	}

	return 0;
}

uint8 UB2UIDocLobbyMatch::GetRemotePlayerStartIndexByIndex(uint32 nIndex)
{
	if (nIndex >= uint32(LobbyMatchRemotePlayerInfoArray.Num()))
		return 0;

	return LobbyMatchRemotePlayerInfoArray[nIndex].RemotePlayerStartIndex;
}

int32 UB2UIDocLobbyMatch::GetRemotePlayerInfoDataCount()
{
	return LobbyMatchRemotePlayerInfoArray.Num();
}

void UB2UIDocLobbyMatch::SetTokenData(uint32 NetId, const FString& MatchToken)
{
	if (!LobbyMatchPlayerToken.Contains(NetId))
		LobbyMatchPlayerToken.Add(NetId, MatchToken);
}

void UB2UIDocLobbyMatch::SetGradeInfoData(uint32 NetId, int32 BattleGrade, int32 BattleStarCount)
{
	if (!LobbyMatchRemotePlayerMatchGradeInfoArray.Contains(NetId))
		LobbyMatchRemotePlayerMatchGradeInfoArray.Emplace(NetId, FLobbyMatchRemotePlayerMatchGradeInfo());

	LobbyMatchRemotePlayerMatchGradeInfoArray[NetId].BattleGrade = BattleGrade;
	LobbyMatchRemotePlayerMatchGradeInfoArray[NetId].BattleStarCount = BattleStarCount;
}

int32 UB2UIDocLobbyMatch::GetBattleGrade(uint32 NetId)
{
	if (LobbyMatchRemotePlayerMatchGradeInfoArray.Contains(NetId))
		return LobbyMatchRemotePlayerMatchGradeInfoArray[NetId].BattleGrade;

	return 10;
}

int32 UB2UIDocLobbyMatch::GetBattleStarCount(uint32 NetId)
{
	if (LobbyMatchRemotePlayerMatchGradeInfoArray.Contains(NetId))
		return LobbyMatchRemotePlayerMatchGradeInfoArray[NetId].BattleStarCount;

	return 0;
}

int64 UB2UIDocLobbyMatch::GetRemoteAccountId(uint32 NetId)
{
	if (NetId == INDEX_NONE)
		return 0;

	for (FLobbyMatchRemotePlayerInfo& playerInfo : LobbyMatchRemotePlayerInfoArray)
	{
		if (playerInfo.RemoteNetId == NetId)
			return playerInfo.RemoteAccountId;
	}

	return 0;
}

int64 UB2UIDocLobbyMatch::GetRemoteAccountIdByIndex(uint32 nIndex)
{
	if (nIndex >= uint32(LobbyMatchRemotePlayerInfoArray.Num()))
		return 0;

	return LobbyMatchRemotePlayerInfoArray[nIndex].RemoteAccountId;
}

void UB2UIDocLobbyMatch::SetInvitationInfo(int32 InvitationType, const FString& Inviter, const FString& Invitee, const FString& InvitationRef, b2network::B2RoomLocationPtr RoomInfoPtr, bool bMyInvitation)
{
	InvitationInfo.InvitationType = InvitationType;
	InvitationInfo.Inviter = Inviter;
	InvitationInfo.Invitee = Invitee;
	InvitationInfo.InvitationInfo = InvitationRef;
	InvitationInfo.RoomInfoPtr = RoomInfoPtr;
	InvitationInfo.bMyInvitation = bMyInvitation;
}

void UB2UIDocLobbyMatch::ClearInvitationInfo()
{
	InvitationInfo.Clear();
	InvitationBeginTime = 0.f;
}
