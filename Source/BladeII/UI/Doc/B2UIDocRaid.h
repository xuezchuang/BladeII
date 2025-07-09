#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "B2UIDocRaid.generated.h"

/* ERaidJointSlotState */
enum class ERaidJoinSlotState : uint8
{
	Invitable,				// 초대가능
	OpenInviteUI,			// 초대 팝업 오픈
	SearchingQuickJoin,		// 빠른검색중
	JoinCharacter,			// 파티합류
	Max,					//
};

enum class ERaidJoinType : uint8
{
	MakeRoom = 0,
	QuickStart,
};

UCLASS()
class BLADEII_API UB2UIDocRaid : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocRaid(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocRaid();

	virtual void Init() override;
	virtual void Destroy() override;

	bool	IsEmpty();
	void	ClearRaidJoinData();		// RaidJoin 관련 데이터를 초기화 한다. (슬롯데이터, 초대받은 RoomId)
	void	ClearRaidBattleData();
	
	void	SetJoinSlotData(int32 SlotIndex, int32 InJoinSlotNetId, FText InCharName, int32 InCharLevel, EPCClass InPCClass, int32 InCombatPower, int32 InGuildIndex);
	void	ResetJoinSlotData(int32 SlotIndex);
	bool	IsAllEmptyJoinSlot();
	int32	GetInvitedUserNum();
	void	SetOpenedInviteUIAndChangeJoinSlotState(bool IsOpen);
	bool	IsJoinedUser(const FText& CharName);
	ERaidJoinSlotState GetDefaultSlotState(ERaidJoinType JoinType);
	int32	GetBattleSlotIndex(uint32 NetID);
	int32	GetJoinSlotIndex(uint32 NetID);
	//EStageDifficulty GetLastBattleStageDifficulty();

	bool GetRaidRewardInfo(int32 Ranking, EPCClass PcClass, int32& OutItemId, int32& OutItemCount, bool& UsedRewardID);
	bool GetRaidBonusRewardInfo(int32 BonusUserRank, EPCClass UserPCClass, int32& OutItemId, int32& OutItemCount, bool& UsedRewardID);

	void SetGetRaidPtr(FB2ResponseGetRaidPtr ResponsePtr);
	FB2ResponseGetRaidPtr GetGetRaidPtr();
	bool IsPanelty();
	FTimespan GetPaneltyTime();
	
	void SetRaidInvitationInfo(const FString& Inviter, const FString& RoomId, int32 MapId, int32 Step, const FDateTime& DateTime);
	const FRaidInviteInfo& GetRaidInvitationInfo() { return RaidInvitationInfo; }
	bool HasRaidInvitationInfo() const { return RaidInvitationInfo.InvitedRoomId.Len() > 0; }

	void ClearRaidInvitationInfo();

	void SetPeriodInfos(TArray<b2network::B2MatchOpenPeriodInfoPtr>& PeriodInfo);
	bool GetRaidOpenByType(int32 nType);

	TArray<b2network::B2MatchOpenPeriodInfoPtr>& GetPeriodInfos() { return RaidOpenPeriodInfo; }

	void SetPartyRetryInfo(b2network::B2EndpointPtr EndPoint, const FString& _NextRoomId);
	void ClearPartyRetryInfo();

	bool HasValidPartyRetryInfo();
	
	b2network::B2EndpointPtr GetPartyRetryChannelPoint() { return ChannelEndpoint; }
	const FString& GetNextRoomId() { return NextRoomId; }

	// ==================== Raid Main 
	//DECLARE_UIDOC_FIELD(uint8,					TryCountPerDay);
	DECLARE_UIDOC_FIELD(int32,					LastRaidStep)
	DECLARE_UIDOC_FIELD(int32,					LastRaidType)
	DECLARE_UIDOC_FIELD(ERaidJoinType,			OpenedRaidJoinType)
	DECLARE_UIDOC_FIELD(bool,					RaidOpen)

	// ==================== RaidJoin UI (0~3 : 슬롯 순서대로)
	DECLARE_UIDOC_FIELD(int32,	MySlotIdx)
	DECLARE_UIDOC_FIELD_FourField(ERaidJoinSlotState, JoinSlotState, ERaidJoinSlotState::Max)
	DECLARE_UIDOC_FIELD_FourField(int32,	JoinSlotNetId, -1)
	DECLARE_UIDOC_FIELD_FourField(FText,	JoinSlotCharName, FText::FromString(TEXT("")))
	DECLARE_UIDOC_FIELD_FourField(int32,	JoinSlotCharLevel, 0)
	DECLARE_UIDOC_FIELD_FourField(EPCClass, JoinSlotPCClass, EPCClass::EPC_End)

	DECLARE_UIDOC_FIELD_FourField(int32,	JoinSlotCombatPoint, 0)
	DECLARE_UIDOC_FIELD_FourField(bool,		JoinSlotIsMyGuildMember, false)

	// =================== Invited User Data
	DECLARE_UIDOC_FIELD(bool,		IsOpenedInviteUI)
	DECLARE_UIDOC_FIELD(bool,		IsReceiveInvite)

	// ==================== InGame UI (0번 인덱스 : 내 캐릭터 데이터 (주의 : DocBattle에서 중복으로 관리하는 데이터는 무시)
	DECLARE_UIDOC_FIELD(int32,	RaidBattleTime)
	DECLARE_UIDOC_FIELD(int32, RaidEndBattleTime)
	DECLARE_UIDOC_FIELD(int32,	FailCountDownTime)
	DECLARE_UIDOC_FIELD(bool, VisibleButtonForDefeat)
	DECLARE_UIDOC_FIELD(bool, AlreadyExitUser)

	DECLARE_UIDOC_FIELD_FourField(int64, AccountId, -1)
	DECLARE_UIDOC_FIELD_FourField(int32, NetId, -1)
	DECLARE_UIDOC_FIELD_FourField(FText, CharName, FText::FromString(TEXT("")))
	DECLARE_UIDOC_FIELD_FourField(int32, CharLevel, 0)
	DECLARE_UIDOC_FIELD_FourField(EPCClass, PCClass, EPCClass::EPC_End)
	DECLARE_UIDOC_FIELD_FourField(int32, PCLevel, 0)
	DECLARE_UIDOC_FIELD_FourField(float, PCHealth, 0.f)
	DECLARE_UIDOC_FIELD_FourField(float, PCMaxHealth, 0.f)
	DECLARE_UIDOC_FIELD_FourField(float, PCShield, 0.f)
	DECLARE_UIDOC_FIELD_FourField(bool,  PCInCriticalCondition, false)
	DECLARE_UIDOC_FIELD_FourField(int32, RaidContribution, 0)
	DECLARE_UIDOC_FIELD_FourField(bool,  PCExitMainGame, false)
	DECLARE_UIDOC_FIELD_FourField(bool,  IsMyGuildMember, false)

	DECLARE_UIDOC_FIELD(bool, IsRequestPartyRematch);
	DECLARE_UIDOC_FIELD(bool, ForceStartRaid);	// for UI
	DECLARE_UIDOC_FIELD(bool, IsForceStartGame); // for InGame
	DECLARE_UIDOC_FIELD(ERaidJoinType, RaidJoinType);

	// ==================== Result UI
	DECLARE_UIDOC_FIELD(FB2ResponseRaidResultPtr, RaidResultReward)
	DECLARE_UIDOC_FIELD(int32, RaidBonusPlayerNetID);
	DECLARE_UIDOC_FIELD(ERaidGotoWhere, ReturnToPageType);
	// ===== Wrong Match, Disconnected Channel
	DECLARE_UIDOC_FIELD(int32, RaidErrorCode);
private:

	FB2ResponseGetRaidPtr m_ptrGetRaidPtr;
	TArray<b2network::B2MatchOpenPeriodInfoPtr> RaidOpenPeriodInfo;

	TArray<bool> RaidOpenByRaidType;

	FRaidInviteInfo				RaidInvitationInfo;

	b2network::B2EndpointPtr	ChannelEndpoint;
	FString						NextRoomId;
};
