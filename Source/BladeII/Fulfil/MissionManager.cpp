#include "BladeII.h"
#include "MissionManager.h"
#include "B2MissionInfo.h"
#include "Event.h"
#include "InternalEvents.h"
#include "BladeIILocText.h"
#include "B2UIManager.h"

EMissionType ConvertMissionType(int32 ServerMissionType)
{
	return static_cast<EMissionType>(ServerMissionType - 1);
}

int32 ConvertMissionType(EMissionType ClientMissionType)
{
	return static_cast<int32>(ClientMissionType) + 1;
}


void FMission::GenerateMission(b2network::B2MissionInfoPtr ReceiveMission)
{
	// ���� Server�� ������ �����ؾ� �� �ֽ� ������
	if (ReceiveMission)
	{
		MissionState = static_cast<EMissionState>(ReceiveMission->mission_state_type);

		ProgressCount = ReceiveMission->progress_count;
		if (MissionType == EMissionType::Serial)
		{
			CompleteStep = ReceiveMission->completed_step;
			CurrentStep = CompleteStep + 1;
			RewardStep = ReceiveMission->rewarded_step;

			int32 GenerateSerialStep = FMath::Clamp<int32>(RewardStep + 1, 1, CurrentStep);
			b2network::B2mdSerialMissionInfoPtr SerialMissionMasterInfo = MissionManager::GetInstance().GetSerialMissionMasterInfo(MissionID, GenerateSerialStep);
			GenerateInfoFromMasterData(SerialMissionMasterInfo);
			if (SerialMissionMasterInfo)
			{
				if (CompleteStep > RewardStep)	// ���� ������ �����ִٸ�
				{
					MissionState = EMissionState::Complete;
					ProgressCount = SerialMissionMasterInfo->goal_count;
				}
			}

			// Serial Mission ��� �Ϸ� �� �������� MissionState�� Rewarded�� �ٲٱ� ����ٸ� Ŭ�� ��ü�Ǵ��� �ʿ�
			else if (MissionManager::GetInstance().IsSerialMissionComplete(MissionID))
			{
				SerialMissionMasterInfo = MissionManager::GetInstance().GetSerialMissionMasterInfo(MissionID, RewardStep);
				MissionState = EMissionState::Rewarded;
				if (SerialMissionMasterInfo)
				{
					ProgressCount = SerialMissionMasterInfo->goal_count;
					GoalCount = SerialMissionMasterInfo->goal_count; // ��� �ӹ� �Ϸ��϶� goalcount�� progresscount�� ������ ������ �ʿ䰳���� ����.
				}
			}
		}
	}
}

void FMission::GenerateInfoFromMasterData(b2network::B2mdMissionInfoPtr MissionMasterData)
{
	// MasterData�� ���ε� �Ǿ�� �� Default ������
	if (MissionMasterData)
	{
		MissionID = MissionMasterData->mission_id;
		MissionType = ConvertMissionType(MissionMasterData->mission_type);
		MissionState = EMissionState::InProgress;	// �⺻�� InProgress����

		GoalCount = MissionMasterData->goal_count;

		CompletePoint = MissionMasterData->complete_point;

		Condition1 = MissionMasterData->condition1;		//1�� �ʿ������� Ȥ�ø𸣴� �ִ°� �ٹ���
		Condition2 = MissionMasterData->condition2;
		Condition3 = MissionMasterData->condition3;
		Condition4 = MissionMasterData->condition4;

		RewardID = MissionMasterData->reward_id;
		RewardCount = MissionMasterData->reward_count;
	}
}

void FMission::GenerateInfoFromMasterData(b2network::B2mdSerialMissionInfoPtr SerialMissionMasterData)
{
	if (SerialMissionMasterData)
	{
		MissionID = SerialMissionMasterData->mission_id;
		MissionType = EMissionType::Serial;

		MissionState = EMissionState::InProgress;	// �⺻�� InProgress����

		GoalCount = SerialMissionMasterData->goal_count;

		Condition1 = SerialMissionMasterData->condition1;		//1�� �ʿ������� Ȥ�ø𸣴� �ִ°� �ٹ���
		Condition2 = SerialMissionMasterData->condition2;
		Condition3 = SerialMissionMasterData->condition3;
		Condition4 = SerialMissionMasterData->condition4;

		RewardID = SerialMissionMasterData->reward_id;
		RewardCount = SerialMissionMasterData->reward_count;
	}
}

MissionManager& MissionManager::GetInstance()
{
	static MissionManager Instance;
	return Instance;
}

void MissionManager::Init()
{
	SubscribeEvents();
}

void MissionManager::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryMasterDataClass<FB2MasterDatas>::GetInstance().Subscribe2(
		[this](const FB2MasterDatas& data)
	{
		this->ReceiveMissionMasterData(data);
	}
	));

	Issues.Add(DeliveryGetMissionClass<FB2GetMission>::GetInstance().Subscribe2(
		[this](const FB2GetMission& data)
	{
		this->UpdateMission(data);

		//������ ���� �� UI ��ȯ
		if (this->IsChangeScene)
		{
			this->IsChangeScene = false;
			LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::Mission);
		}
	}
	));

	Issues.Add(DeliveryReceiveMissionRewardClass<FB2ReceiveMissionReward>::GetInstance().Subscribe2(
		[this](const FB2ReceiveMissionReward& data)
	{
		this->ReceiveMissionReward(data);
	}
	));
	
	Issues.Add(DeliveryReceiveCompleteMissionRewardClass<FB2ReceiveCompleteMissionReward>::GetInstance().Subscribe2(
		[this](const FB2ReceiveCompleteMissionReward& data)
	{
		this->ReceiveMissionCompletePointReward(data);
	}
	));

	Issues.Add(data_trader::MissionAlarmNotifyClass<int32, int32>::GetInstance().Subscribe2(
		[this](int32 MissionID, int32 CompleteStep)
	{
		this->ReceveiMissionCompleteNotify(MissionID, CompleteStep);
	}
	));
}

void MissionManager::UnsubscribeEvents()
{
	Issues.Empty();
}

void MissionManager::ReceiveMissionMasterData(const FB2MasterDatas& MasterData)
{
	//PeriodMissions.Empty();

	//// ������ �����͸� ������ ���ÿ� ProgressMissionList�� �˸��� Mission���� ���� �ϰ�
	//// UpdateMission�� ������ �ش� Mission�� ã�ư� Update�� �� �ִ� �������
	//const TArray<b2network::B2mdMissionInfoPtr>& ServerMissions = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_mission_infos_index, MasterData);
	//for (const b2network::B2mdMissionInfoPtr& Mission : ServerMissions)
	//{
	//	PeriodMissions.Add(Mission);
	//	GenerateEmptyMission(Mission);
	//}

	//const TArray<b2network::B2mdSerialMissionInfoPtr>& ServerSerialMissions = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_serial_mission_infos_index, MasterData);
	//for (const b2network::B2mdSerialMissionInfoPtr& SerialMission : ServerSerialMissions)
	//{
	//	SerialMissions.AddUnique(SerialMission->mission_id, SerialMission);
	//}

	//// Serial Mission�� �� Mission�� ù ��° Step Mission �����θ� ä��� UpdateMission���� �����Ȳ�� �ݿ��Ѵ�.
	//TArray<int32> SerialMissionIDList;
	//SerialMissions.GetKeys(SerialMissionIDList);
	//for (auto MissionID : SerialMissionIDList)
	//{
	//	auto SerialMissionMasterInfo = GetSerialMissionMasterInfo(MissionID, 1); // ù��° Step���� �ʱ� ����
	//	if (SerialMissionMasterInfo != nullptr)
	//	{
	//		GenerateEmptyMission(SerialMissionMasterInfo);
	//	}
	//}

	//MissionPointRewardInfos.Empty();
	//MissionPointRewardList.Empty();

	//const TArray<b2network::B2mdMissionCompletePointInfoPtr>& ServerCompletePoints = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_mission_complete_point_infos_index, MasterData);
	//for (const b2network::B2mdMissionCompletePointInfoPtr& CompletePointInfo : ServerCompletePoints)
	//{
	//	MissionPointRewardInfos.Add(CompletePointInfo);

	//	MissionPointRewardPtr RewardPtr(new FMissionPointRewardInfo());
	//	RewardPtr->MissionType = ConvertMissionType(CompletePointInfo->mission_type);
	//	RewardPtr->RequirePoint = CompletePointInfo->require_point;
	//	RewardPtr->RewardIndex = CompletePointInfo->index;
	//	RewardPtr->RewardInfos = CompletePointInfo->rewards;

	//	MissionPointRewardList.Add(RewardPtr);
	//}
}

void MissionManager::UpdateMission(const FB2GetMission& MissionData)
{
	//// ���� ��Ȳ Upadate
	//TArray<b2network::B2MissionInfoPtr>	MissionInfos = GET_TUPLE_DATA(FB2ResponseGetMission::mission_infos_index, MissionData);
	//for (auto ServerMission : MissionInfos)
	//{
	//	if (ServerMission != nullptr)
	//	{
	//		GenerateMission(ServerMission);
	//	}
	//}

	//CurrentCompletePoints = GET_TUPLE_DATA(FB2ResponseGetMission::mission_complete_point_infos_index, MissionData);
	//for (auto ServerPointReward : CurrentCompletePoints)
	//{
	//	if (ServerPointReward != nullptr)
	//	{
	//		TArray<int32> RewardedIndexList = ServerPointReward->reward_indexs;

	//		EMissionType ClientMissionType = ConvertMissionType(ServerPointReward->mission_type);
	//		int32 MissionPoint = GetMissionPoint(ClientMissionType);
	//		for (auto RewardPtr : MissionPointRewardList)
	//		{
	//			if (RewardPtr && ClientMissionType == RewardPtr->MissionType)
	//			{
	//				if (MissionPoint >= RewardPtr->RequirePoint)
	//					RewardPtr->bReached = true;

	//				int32 FindedIndex;
	//				if (RewardedIndexList.Find(RewardPtr->RewardIndex, FindedIndex))
	//				{
	//					RewardPtr->bRewarded = true;
	//				}
	//			}
	//		}
	//	}
	//}
}

// ���� �Ϸ� ����
void MissionManager::ReceiveMissionReward(const FB2ReceiveMissionReward& MissionReward)
{
	//b2network::B2MissionInfoPtr MissionInfoPtr = GET_TUPLE_DATA(FB2ResponseReceiveMissionReward::received_mission_info_index, MissionReward);
	////	TArray<b2network::B2RewardPtr> ServerRewards = GET_TUPLE_DATA(FB2ResponseReceiveMissionReward::received_rewards_index, MissionReward);

	//UE_LOG(LogBladeII, Log, TEXT("MissionManager Call - ReceiveMissionReward  MissionID : %d "), MissionInfoPtr->mission_id);

	//MissionPtr Mission = GetMission(MissionInfoPtr->mission_id);
	//if (Mission)
	//{
	//	Mission->GenerateMission(MissionInfoPtr);
	//}

	//OnChangedMissionReward.Broadcast(MissionReward);
}

// ���� ����Ʈ �Ϸ� ����
void MissionManager::ReceiveMissionCompletePointReward(const FB2ReceiveCompleteMissionReward& CompleteReward)
{
	//int32 ServerMissionType = GET_TUPLE_DATA(FB2ResponseReceiveCompleteMissionReward::received_mission_type_index, CompleteReward);
	//int32 RewardIndex = GET_TUPLE_DATA(FB2ResponseReceiveCompleteMissionReward::received_reward_index_index, CompleteReward);
	////TArray<b2network::B2RewardPtr> ServerRewards = GET_TUPLE_DATA(FB2ResponseReceiveCompleteMissionReward::received_rewards_index, CompleteReward);

	//UE_LOG(LogBladeII, Log, TEXT("MissionManager Call - Receive MissionCompletePoint MissionType : %d , RewardIndex : %d"), ServerMissionType, RewardIndex);

	//for (auto PointReward : MissionPointRewardList)
	//{
	//	if (PointReward)
	//	{
	//		EMissionType ClientMissionType = ConvertMissionType(ServerMissionType);
	//		if (PointReward->MissionType == ClientMissionType && PointReward->RewardIndex == RewardIndex)
	//		{
	//			PointReward->bRewarded = true;
	//		}
	//	}
	//}
}

void MissionManager::ReceveiMissionCompleteNotify(int32 MissionID, int32 CompleteStep)
{
	MissionPtr CompleteMision = GetMission(MissionID);
	if (CompleteMision != nullptr)
	{
		CachedCompleteMissions.Add(CompleteMision);
		CompleteMision->CurrentStep = CompleteStep; // + 1�� ����� �ϴ����� Ȯ�� ��

		LobbyMissionAlarmClass<>::GetInstance().Signal();
	}
}

TArray<int32> MissionManager::GetMissionIDList(EMissionType MissionType)
{
	TArray<int32> MissionIDList;

	if (MissionType == EMissionType::Serial)
	{
		SerialMissions.GetKeys(MissionIDList);
	}

	else
	{
		int32 ServerMissionType = ConvertMissionType(MissionType);
		for (auto Mission : PeriodMissions)
		{
			if (Mission != nullptr && Mission->mission_type == ServerMissionType)
			{
				MissionIDList.Add(Mission->mission_id);
			}
		}
	}

	return MissionIDList;
}


TArray<int32> MissionManager::GetCompletePeriodMissionIDList(EMissionType MissionType)
{
	TArray<int32> Result;													// ���� �ְ� ����
	int32 ServerMissionType = ConvertMissionType(MissionType);

	for (b2network::B2mdMissionInfoPtr Mission : PeriodMissions)
	{
		if (Mission != nullptr && Mission->mission_type == ServerMissionType)
		{
			MissionPtr MissionPtrInfo = GetMission(Mission->mission_id);
			if (MissionPtrInfo->MissionState == EMissionState::Complete)
			{
				Result.Add(Mission->mission_id);
			}
		}
	}

	return Result;
}

TArray<int32> MissionManager::GetCompleteSerialMissionIDList(EMissionType MissionType)
{
	TArray<int32> Result;
	int32 ServerMissionType = ConvertMissionType(MissionType);

	TArray<int32> MissionIDList = GetMissionIDList(MissionType);
	for (auto MissionID : MissionIDList)
	{
		MissionPtr Mission = GetMission(MissionID);
		if (Mission && Mission->MissionState == EMissionState::Complete)
		{
			Result.Add(MissionID);
		}
	}
	return Result;
}

TArray<int32> MissionManager::GetCompleteMissionIDList(EMissionType MissionType)
{
	switch (MissionType)
	{
	case EMissionType::Serial:
		return GetCompleteSerialMissionIDList(MissionType);
	default:
		return GetCompletePeriodMissionIDList(MissionType);
	}

	return TArray<int32>();
}

MissionPtr MissionManager::GetMission(int32 MissionID)
{
	for (auto MissionInfo : ProgressMissionList)
	{
		if (MissionInfo && MissionInfo->MissionID == MissionID)
		{
			return MissionInfo;
		}
	}

	return nullptr;
}

MissionPointRewardPtr MissionManager::GetMissionPointRewardInfo(EMissionType MissionType, int32 ServerDefineRewardIndex)
{
	int32 ServerMissionType = ConvertMissionType(MissionType);
	for (auto PointRewardPtr : MissionPointRewardList)
	{
		if (PointRewardPtr && PointRewardPtr->MissionType == MissionType && PointRewardPtr->RewardIndex == ServerDefineRewardIndex)
		{
			return PointRewardPtr;
		}
	}

	return nullptr;
}

int32 MissionManager::GetMissionPoint(EMissionType MissionType)
{
	int32 ServerMissionType = ConvertMissionType(MissionType);
	for (auto CurrentPointInfo : CurrentCompletePoints)
	{
		if (CurrentPointInfo->mission_type == ServerMissionType)
		{
			return CurrentPointInfo->completePoint;
		}
	}

	return 0;
}

FText MissionManager::GetMissionTitle(int32 MissionID, int32 Step /*= 0*/)
{
	return BladeIIGetLOCText(B2LOC_CAT_MISSIONTEXT, GetMissionTitleKey(MissionID, Step));
}

void MissionManager::FlushCachedCompleteMission(EMissionType FlushMissionType)
{
	if (FlushMissionType == EMissionType::Max)
	{
		CachedCompleteMissions.Empty();
		return;
	}

	for (auto CachedMission : CachedCompleteMissions)
	{
		if (CachedMission && CachedMission->MissionType == FlushMissionType)
		{
			CachedCompleteMissions.Remove(CachedMission);
		}
	}
}

bool MissionManager::HasCachedCompleteMission(EMissionType MissionType)
{
	if (MissionType == EMissionType::Max)
		return CachedCompleteMissions.Num() > 0 ? true : false;

	for (auto CachedMission : CachedCompleteMissions)
	{
		if (CachedMission && CachedMission->MissionType == MissionType)
			return true;
	}

	return false;
}

b2network::B2mdMissionInfoPtr MissionManager::GetMissionMasterInfo(int32 MissionID)
{
	for (auto PeriodMission : PeriodMissions)
	{
		if (PeriodMission && PeriodMission->mission_id == MissionID)
			return PeriodMission;
	}

	return nullptr;
}

b2network::B2mdSerialMissionInfoPtr MissionManager::GetSerialMissionMasterInfo(int32 MissionID, int32 MissionStep)
{
	TArray<b2network::B2mdSerialMissionInfoPtr> FindedSerialMissions;
	SerialMissions.MultiFind(MissionID, FindedSerialMissions);
	for (auto SerialPtr : FindedSerialMissions)
	{
		if (SerialPtr && SerialPtr->mission_step == MissionStep)
		{
			return SerialPtr;
		}
	}

	return nullptr;
}

bool MissionManager::IsSerialMissionComplete(int32 MissionID)
{
	MissionPtr Mission = GetMission(MissionID);
	if (Mission && Mission->MissionType == EMissionType::Serial)
	{
		if (Mission->CompleteStep == Mission->RewardStep)
		{
			TArray<b2network::B2mdSerialMissionInfoPtr> FindedSerialMissions;
			SerialMissions.MultiFind(MissionID, FindedSerialMissions);
			if (Mission->CurrentStep >= FindedSerialMissions.Num()) // Mission Step �ܰ谡 SerialMission Step���� ������
				return true;
		}
	}

	return false;
}

bool MissionManager::IsReadyforReaward(EMissionType MissionType)
{
	TArray<int32> MissionIDList = GetMissionIDList(MissionType);
	for (auto MissionID : MissionIDList)
	{
		MissionPtr Mission = GetMission(MissionID);
		if (Mission && Mission->MissionState == EMissionState::Complete)
		{
			return true;
		}
	}

	for (auto RewardPtr : MissionPointRewardList)
	{
		if (RewardPtr && RewardPtr->MissionType == MissionType)
		{
			if (RewardPtr->bReached == true && RewardPtr->bRewarded == false)	// ���������� ������� �������
			{
				return true;
			}
		}
	}

	return false;
}

bool MissionManager::IsReadyforMIssionReaward(EMissionType MissionType)
{
	bool Result = false;
	TArray<int32> MissionIDList = GetMissionIDList(MissionType);
	for (auto MissionID : MissionIDList)
	{
		MissionPtr Mission = GetMission(MissionID);
		if (Mission && Mission->MissionState == EMissionState::Complete)
		{
			Result = true;
		}
	}

	int32 TotalRewardBoxIndex = 3;
	int32 CurrentPoint = MissionManager::GetInstance().GetMissionPoint(MissionType);

	for (int32 i = 0; i < TotalRewardBoxIndex; ++i)
	{
		MissionPointRewardPtr RewardPtr = MissionManager::GetInstance().GetMissionPointRewardInfo(MissionType, i + 1);
		if (RewardPtr)
		{
			if (RewardPtr->bRewarded == true)
				continue;

			if (CurrentPoint >= RewardPtr->RequirePoint)
			{
				Result = true;
				break;
			}
		}
	}
	
	return Result;
}

void MissionManager::MarkIsChangeScene()
{
	IsChangeScene = true;
}

void MissionManager::GenerateMission(b2network::B2MissionInfoPtr ServerMission)
{
	MissionPtr TargetMission = GetMission(ServerMission->mission_id);
	if (TargetMission == nullptr)
	{
		TargetMission = MissionPtr(new FMission());
		ProgressMissionList.Add(TargetMission);
	}

	if (TargetMission->MissionType == EMissionType::Serial)
	{
		b2network::B2mdSerialMissionInfoPtr SerialMissionMasterInfo = GetSerialMissionMasterInfo(TargetMission->MissionID, TargetMission->CurrentStep);
		TargetMission->GenerateInfoFromMasterData(SerialMissionMasterInfo);
	}
	else
	{
		b2network::B2mdMissionInfoPtr MissionMasterInfo = GetMissionMasterInfo(TargetMission->MissionID);
		TargetMission->GenerateInfoFromMasterData(MissionMasterInfo);
	}

	TargetMission->GenerateMission(ServerMission);
}

void MissionManager::GenerateEmptyMission(b2network::B2mdMissionInfoPtr MissionMasterData)
{
	if (GetMission(MissionMasterData->mission_id))
		return;

	MissionPtr EmptyMission(new FMission());
	EmptyMission->GenerateInfoFromMasterData(MissionMasterData);

	ProgressMissionList.Add(EmptyMission);
}

void MissionManager::GenerateEmptyMission(b2network::B2mdSerialMissionInfoPtr SerialMissionMasterData)
{
	if (GetMission(SerialMissionMasterData->mission_id))
		return;

	MissionPtr EmptyMission(new FMission());
	EmptyMission->GenerateInfoFromMasterData(SerialMissionMasterData);

	ProgressMissionList.Add(EmptyMission);
}
