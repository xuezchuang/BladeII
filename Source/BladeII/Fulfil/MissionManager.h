#pragma once


#include "B2MessageInfoCommonStruct.h"
#include "EventSubsystem.h"

enum class EMissionType : uint8
{
	Daily = 0,
	Weekly,
	Monthly,
	Serial,
	GoogleMission,
	Event,
	Max,

	None
};

EMissionType ConvertMissionType(int32 ServerMissionType);
int32 ConvertMissionType(EMissionType ClientMissionType);


enum class EMissionState : uint8
{
	None,
	InProgress,
	Complete,
	Rewarded
};

struct FMission
{
	int32				MissionID;
	EMissionType		MissionType;
	EMissionState		MissionState;

	int32				ProgressCount;
	int32				GoalCount;

	int32				Condition1;
	int32				Condition2;
	int32				Condition3;
	int32				Condition4;

	int32				CurrentStep;	// Only for SerialMission
	int32				CompleteStep;	// Only for SerialMission
	int32				RewardStep;		// Only for SerialMission

	int32				CompletePoint;	// Only for PeriodMission

	int32				RewardID;
	int32				RewardCount;

	FMission()
		: MissionID(0), MissionType(EMissionType::None), MissionState(EMissionState::None), ProgressCount(0), GoalCount(0), CurrentStep(1), CompleteStep(0), RewardStep(0), CompletePoint(0), RewardID(0), RewardCount(0)
	{}

	void GenerateMission(b2network::B2MissionInfoPtr ReceiveMission);
	void GenerateInfoFromMasterData(b2network::B2mdMissionInfoPtr MissionMasterData);
	void GenerateInfoFromMasterData(b2network::B2mdSerialMissionInfoPtr SerialMissionMasterData);
};
using MissionPtr = std::shared_ptr<FMission>;

struct FMissionPointRewardInfo
{
	EMissionType					MissionType;
	int32							RewardIndex;
	int32							RequirePoint;
	bool							bReached;
	bool							bRewarded;

	TArray<b2network::B2RewardPtr>	RewardInfos;

	FMissionPointRewardInfo() : MissionType(EMissionType::None), RewardIndex(0), RequirePoint(0), bReached(false), bRewarded(false) {}
};
using MissionPointRewardPtr = std::shared_ptr<FMissionPointRewardInfo>;


class MissionManager
{
public:

	static MissionManager& GetInstance();

	void Init();

	void SubscribeEvents();
	void UnsubscribeEvents();
	
	TArray<int32>			GetMissionIDList(EMissionType MissionType);
	TArray<int32>			GetCompleteMissionIDList(EMissionType MissionType);
	MissionPtr				GetMission(int32 MissionID);
	MissionPointRewardPtr	GetMissionPointRewardInfo(EMissionType MissionType, int32 ServerDefineRewardIndex);
	int32					GetMissionPoint(EMissionType MissionType);
	FText					GetMissionTitle(int32 MissionID, int32 Step = 0);

	const TArray<MissionPtr>& GetCachedCompleteMission() { return CachedCompleteMissions; }
	void FlushCachedCompleteMission(EMissionType FlushMissionType);
	bool HasCachedCompleteMission(EMissionType MissionType);

	b2network::B2mdMissionInfoPtr		GetMissionMasterInfo(int32 MissionID);
	b2network::B2mdSerialMissionInfoPtr	GetSerialMissionMasterInfo(int32 MissionID, int32 MissionStep);

	bool	IsSerialMissionComplete(int32 MissionID);
	bool	IsReadyforReaward(EMissionType MissionType);
	bool	IsReadyforMIssionReaward(EMissionType MissionType);

	void	MarkIsChangeScene();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedMissionReward, const FB2ReceiveMissionReward&);
	FOnChangedMissionReward		OnChangedMissionReward;

private:

	MissionManager() {}
	MissionManager(MissionManager const&) = delete;
	MissionManager operator = (MissionManager const&) = delete;

	void	GenerateMission(b2network::B2MissionInfoPtr ServerMission);
	void	GenerateEmptyMission(b2network::B2mdMissionInfoPtr MissionMasterData);
	void	GenerateEmptyMission(b2network::B2mdSerialMissionInfoPtr SerialMissionMasterData);

	void ReceiveMissionMasterData(const FB2MasterDatas& MasterData);
	void UpdateMission(const FB2GetMission& MissionData);
	void ReceiveMissionReward(const FB2ReceiveMissionReward& MissionReward);
	void ReceiveMissionCompletePointReward(const FB2ReceiveCompleteMissionReward& CompleteInfo);

	void ReceveiMissionCompleteNotify(int32 MissionID, int32 CompleteStep);

	TArray<int32> GetCompletePeriodMissionIDList(EMissionType MissionType);
	TArray<int32> GetCompleteSerialMissionIDList(EMissionType MissionType);
private:
	
	TArray<b2network::B2mdMissionInfoPtr>					PeriodMissions;
	TMultiMap<int32, b2network::B2mdSerialMissionInfoPtr>	SerialMissions;
	TArray<b2network::B2mdMissionCompletePointInfoPtr>		MissionPointRewardInfos;
	TArray<b2network::B2MissionCompletePointInfoPtr>		CurrentCompletePoints;

	TArray<MissionPointRewardPtr>							MissionPointRewardList;
	TArray<MissionPtr>										ProgressMissionList;

	TArray<MissionPtr>										CachedCompleteMissions;

	bool													IsChangeScene = false;

	TArray<std::shared_ptr<issue>>							Issues;
};
