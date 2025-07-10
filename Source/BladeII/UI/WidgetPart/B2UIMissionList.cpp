// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIMissionList.h"
#include "MissionManager.h"
#include "B2UIPMissionSlot.h"
#include "B2UIManager.h"
#include "B2UIMsgPopupReward.h"
#include "Retailer.h"
#include "B2UILinkManager.h"
#include "B2MissionInfo.h"
#include "B2UIClientIndicator.h"


void UB2UIMissionList::Init()
{
	Super::Init();

	SubscribeEvent();
}

void UB2UIMissionList::DestroySelf(class UB2UIManager* InUIManager)
{
	UnSubscribeEvent();

	Super::DestroySelf(InUIManager);
}

void UB2UIMissionList::UpdateMissionSlotList(EMissionType MissionType)
{
	SelectedMissionType = MissionType;
	TArray<int32> MissionIDList = MissionManager::GetInstance().GetMissionIDList(MissionType);
	TArray<MissionPtr> MissionList;
	for (int32 MissionID : MissionIDList)
	{
		MissionList.Add(MissionManager::GetInstance().GetMission(MissionID));
	}

	TArray<int32> SortedMissionIDList;
	for (auto Mission : MissionList)
	{
		if (Mission && Mission->MissionState == EMissionState::Complete)
		{
			SortedMissionIDList.AddUnique(Mission->MissionID);
		}
	}
	for (auto Mission : MissionList)
	{
		if (Mission && Mission->MissionState == EMissionState::InProgress)
		{
			SortedMissionIDList.AddUnique(Mission->MissionID);
		}
	}
	for (auto Mission : MissionList)
	{
		if (Mission && Mission->MissionState == EMissionState::Rewarded)
		{
			SortedMissionIDList.AddUnique(Mission->MissionID);
		}
	}

	MissionIDList = SortedMissionIDList;

	int32 Index = 0;
	for (int32 MissionID : MissionIDList)
	{
		TWeakObjectPtr<UB2UIPMissionSlot> MissionSlot = GetMissionSlot(Index);
		if (MissionSlot.IsValid() == false)
		{
			MissionSlot = CreateMissionSlot();
			if (SB_Mission.IsValid())
			{
				SB_Mission->AddChild(MissionSlot.Get());
			}
		}

		if (MissionSlot.IsValid())
		{
			MissionSlot->InitMissionSlot(MissionID);
			Index++;
		}
	}

	for (int32 LeftIdx = Index; LeftIdx < MissionSlots.Num(); LeftIdx++)
	{
		if (MissionSlots[LeftIdx].IsValid())
		{
			MissionSlots[LeftIdx]->SetVisibility(ESlateVisibility::Collapsed);
			MissionSlots[LeftIdx]->ClearMissionSlot();
		}
	}
}

void UB2UIMissionList::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2ScrollBox, SB_Mission);
}

void UB2UIMissionList::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UIMissionList);

	DeliveryReceiveMissionRewardTicket = DeliveryReceiveMissionRewardClass<FB2ReceiveMissionReward>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryReceiveMissionReward, const FB2ReceiveMissionReward& MissionReward)
		Capture->OnReceiveMissionReward(MissionReward);
	END_CAPTURE_OBJECT()
		);

	DeliveryReceiveCompleteMissionRewardTicket = DeliveryReceiveCompleteMissionRewardClass<FB2ReceiveCompleteMissionReward>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryReceiveCompleteMissionReward, const FB2ReceiveCompleteMissionReward& CompleteReward)
		Capture->OnReceiveMissionPointReward(CompleteReward);
	END_CAPTURE_OBJECT()
		);

	DeliveryGetContentsOpenStatusTicket = DeliveryGetContentsOpenStatusClass<FB2ResponseGetContentsOpenStatusPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetContentsOpenStatus, const FB2ResponseGetContentsOpenStatusPtr& GetContentsOpenStatusPtr)
		Capture->OnReceiveGetContentsOpenStatus(GetContentsOpenStatusPtr);
	END_CAPTURE_OBJECT()
		);

	GetAllMIssionQuestRewardEventID = GetAllMIssionQuestRewardClass<>::GetInstance().Subscribe([this]() { this->GetAllMissionReward(); });

	DeliveryReceiveMissionRewardErrorClass<const int32>::GetInstance().Subscribe(
		[this](const int32 ErrorCode)
	{
		this->bLoopReceiveSerialRewards = false;
		this->bRequestRewardAllCompleteMission = false;
		UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);
		SetAllRewardButtonDisable(false);
		if (this->CachedReceiveRewards.Num() > 0)
		{
			this->OpenMissionRewardDialog(this->CachedReceiveRewards);
			this->CachedReceiveRewards.Empty();
		}

		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst)
		{
			UIMgrInst->OpenMsgPopupFromErrorCode(ErrorCode);
		}
	});

	ReqGetContetnsOpenStatusClass<const int32>::GetInstance().Subscribe([this](const int32 MissionID)
	{
		this->ReqGetContetnsOpenStatus(MissionID);
	});

	SetAllRewardButtonDisableClass<const bool>::GetInstance().Subscribe([this](const bool bDisable)
	{
		this->SetAllRewardButtonDisable(bDisable);
	});
}

void UB2UIMissionList::UnSubscribeEvent()
{
	DeliveryReceiveMissionRewardClass<FB2ReceiveMissionReward>::GetInstance().Unsubscribe(DeliveryReceiveMissionRewardTicket);
	DeliveryReceiveMissionRewardTicket = 0;

	DeliveryReceiveCompleteMissionRewardClass<FB2ReceiveCompleteMissionReward>::GetInstance().Unsubscribe(DeliveryReceiveCompleteMissionRewardTicket);
	DeliveryReceiveCompleteMissionRewardTicket = 0;

	DeliveryGetContentsOpenStatusClass<FB2ResponseGetContentsOpenStatusPtr>::GetInstance().Unsubscribe(DeliveryGetContentsOpenStatusTicket);
	DeliveryGetContentsOpenStatusTicket = 0;

	GetAllMIssionQuestRewardClass<>::GetInstance().Unsubscribe(GetAllMIssionQuestRewardEventID);
	GetAllMIssionQuestRewardEventID = 0;

	DeliveryReceiveMissionRewardErrorClass<const int32>::GetInstance().UnsubscribeAll();
	ReqGetContetnsOpenStatusClass<const int32>::GetInstance().UnsubscribeAll();
	SetAllRewardButtonDisableClass<const bool>::GetInstance().UnsubscribeAll();
}

TWeakObjectPtr<UB2UIPMissionSlot> UB2UIMissionList::CreateMissionSlot()
{
	UB2UIPMissionSlot* MissionSlot = CreateWidget<UB2UIPMissionSlot>(GetWorld(), MissionSlotClass);
	if (MissionSlot != nullptr)
	{
		MissionSlot->Init();
		MissionSlots.Add(MissionSlot);
	}

	return MissionSlot;
}

TWeakObjectPtr<class UB2UIPMissionSlot> UB2UIMissionList::GetMissionSlot(int32 Index)
{
	if (MissionSlots.IsValidIndex(Index))
	{
		return MissionSlots[Index];
	}

	return nullptr;
}

void UB2UIMissionList::OnReceiveMissionReward(const FB2ReceiveMissionReward& MissionReward)
{
	//b2network::B2MissionInfoPtr MissionInfoPtr = GET_TUPLE_DATA(FB2ResponseReceiveMissionReward::received_mission_info_index, MissionReward);
	//TArray<b2network::B2RewardPtr> ServerRewards = GET_TUPLE_DATA(FB2ResponseReceiveMissionReward::received_rewards_index, MissionReward);

	//UE_LOG(LogBladeII, Log, TEXT("B2UIMission Call - ReceiveMissionReward  MissionID : %d "), MissionInfoPtr->mission_id);

	//bool RewardList = CheckMissionPointReward(SelectedMissionType) || CompleteMissionIDs.Num();

	//if (MissionInfoPtr)
	//{
	//	if (bRequestRewardAllCompleteMission)
	//	{
	//		CachedReceiveRewards.Append(ServerRewards);
	//	}

	//	if (bRequestRewardAllCompleteMission && RewardList == true)
	//	{
	//		if (GetWorld())
	//		{
	//			ResponseTime = GetWorld()->GetTimeSeconds();
	//		}
	//		StartTimerReceiveMissionReward();
	//	}
	//	// 전체 보상받기를 눌렀다면 모든 보상이 완료될 때 까지 UI 갱신하지 않음
	//	else
	//	{
	//		if (bRequestRewardAllCompleteMission && RewardList == false)
	//		{
	//			UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);
	//			bRequestRewardAllCompleteMission = false;

	//			// 일반임무 전부다받기를 진행 중일 경우 목록 갱신을 하여 재보상을 받을게 있는지 다시 확인
	//			if (bLoopReceiveSerialRewards == true)
	//			{
	//				data_trader::Retailer::GetInstance().RequestGetMission(0);
	//			}
	//			else
	//			{
	//				OpenMissionRewardDialog(CachedReceiveRewards);
	//				CachedReceiveRewards.Empty();
	//			}
	//		}
	//		else if (bRequestRewardAllCompleteMission == false)
	//		{
	//			OpenMissionRewardDialog(ServerRewards);
	//		}

	//		ChangeMissionTabClass<const EMissionType, const bool>::GetInstance().Signal(SelectedMissionType, false);
	//	}
	//}
}

void UB2UIMissionList::OpenMissionRewardDialog(const TArray<b2network::B2RewardPtr>& Rewards)
{
	if (!UB2UIManager::GetInstance()->OpenRewardMailPopUp(Rewards))
	{
		if (UB2UIMsgPopupReward* RewardPopup = OpenRewardDialog(Rewards))
		{
			RewardPopup->SetAutoCloseOnClick(true);
			RewardPopup->ShowRewardPopup(true);
		}
	}
}

void UB2UIMissionList::OnReceiveMissionPointReward(const FB2ReceiveCompleteMissionReward& CompleteReward)
{
	//int32 ServerMissionType = GET_TUPLE_DATA(FB2ResponseReceiveCompleteMissionReward::received_mission_type_index, CompleteReward);
	//int32 RewardBoxIndex = GET_TUPLE_DATA(FB2ResponseReceiveCompleteMissionReward::received_reward_index_index, CompleteReward);
	//TArray<b2network::B2RewardPtr> ServerRewards = GET_TUPLE_DATA(FB2ResponseReceiveCompleteMissionReward::received_rewards_index, CompleteReward);

	//UE_LOG(LogBladeII, Log, TEXT("B2UIMission Call - Receive MissionCompletePoint MissionType : %d , RewardIndex : %d"), ServerMissionType, RewardBoxIndex);


	//int32 RewardPointIndex = CheckMissionPointReward(SelectedMissionType);

	//if (bRequestRewardAllCompleteMission)
	//{
	//	CachedReceiveRewards.Append(ServerRewards);
	//}

	//if (bRequestRewardAllCompleteMission && RewardPointIndex > 0)				//모두받기하고 보상이 남아있을경우
	//{
	//	if (GetWorld())
	//	{
	//		ResponseTime = GetWorld()->GetTimeSeconds();
	//	}
	//	StartTimerReceiveMissionReward();
	//}
	//else
	//{
	//	if (bRequestRewardAllCompleteMission && RewardPointIndex <= 0)
	//	{
	//		UB2UIManager::GetInstance()->CloseUI(UIFName::ClientIndicator);
	//		bRequestRewardAllCompleteMission = false;

	//		OpenMissionRewardDialog(CachedReceiveRewards);
	//		CachedReceiveRewards.Empty();
	//	}
	//	else if (bRequestRewardAllCompleteMission == false)
	//	{
	//		EMissionType MissionType = ConvertMissionType(ServerMissionType);
	//		OpenMissionRewardDialog(ServerRewards);
	//	}

	//	ChangeMissionTabClass<const EMissionType, const bool>::GetInstance().Signal(SelectedMissionType, false);
	//}

}

void UB2UIMissionList::OnReceiveGetContentsOpenStatus(const FB2ResponseGetContentsOpenStatusPtr& GetContentsOpenStatusPtr)
{
	TArray<bool> IsContentsOpened;
	IsContentsOpened.Empty();

	for (auto El : GetContentsOpenStatusPtr->contents)
	{
		IsContentsOpened.Add(El->opened);
	}

	if (IsContentsOpened.Num() > 0 && IsContentsOpened[0])
	{
		UB2MissionInfo* MissionTable = StaticFindMissionInfo();
		if (MissionTable)
		{
			int32 Step = 0;
			FMissionInfo* MissionInfo = MissionTable->GetMissionInfo(CheckMissionID);
			if (MissionInfo == nullptr)
			{
				MissionPtr Mission = MissionManager::GetInstance().GetMission(CheckMissionID);
				Step = Mission ? Mission->CurrentStep : 0;
				MissionInfo = MissionTable->GetMissionInfo(CheckMissionID, Step);
				if (Step != 0)
					Step--;
			}
			if (MissionInfo)
			{
				if (IsContentsOpened.IsValidIndex(Step))
				{
					if (IsContentsOpened[Step])
					{
						FB2UILinkManager::LinkUIScene(MissionInfo->Scene, MissionInfo->Optional1, MissionInfo->Optional2);
					}
					else
					{
						OpenContentsFailMsgPopup();
					}
				}
			}
		}
	}
	else
	{
		OpenContentsFailMsgPopup();
	}
}

class UB2UIMsgPopupReward* UB2UIMissionList::OpenRewardDialog(const TArray<b2network::B2RewardPtr>& Rewards)
{
	UB2UIMsgPopupReward* RewardPopup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
	if (RewardPopup != nullptr)
	{
		RewardPopup->InitRewardMsgPopup(Rewards);
		RewardPopup->SetConfirmCallback(FMsgPopupOnClick::CreateLambda([this]() { this->OnClickGetRewardMsgPopup(); }));
	}

	return RewardPopup;
}

void UB2UIMissionList::OnClickGetRewardMsgPopup()
{
	SetAllRewardButtonDisable(false); // 막은 보상받기 버튼을 풀어
	bLoopReceiveSerialRewards = false;
	data_trader::Retailer::GetInstance().RequestGetMission(0);
}

void UB2UIMissionList::SetAllRewardButtonDisable(bool bDisable)
{
	for (auto MissionSlot : MissionSlots)
	{
		if (MissionSlot.IsValid() && MissionSlot->GetMissionState() == EMissionState::Complete)
		{
			MissionSlot->SetRewardButtonVisibility(bDisable);
			if (!bDisable)
			{
				MissionSlot->SetRewardButtonDisable(bDisable);
			}
		}
	}
}

void UB2UIMissionList::SetListScrollOffSet(float OffSet)
{
	if (SB_Mission.IsValid())
	{
		SB_Mission->SetScrollOffset(OffSet);
	}
}

void UB2UIMissionList::ReqGetContetnsOpenStatus(int32 MissionID)
{
	CheckMissionID = MissionID;
	data_trader::Retailer::GetInstance().RequestGetContentsOpenStatus(MissionID);
}

void UB2UIMissionList::OpenContentsFailMsgPopup()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_MISSIONTEXT, TEXT("MissionOpenContentsFail")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
	);
}

void UB2UIMissionList::StartTimerReceiveMissionReward()
{
	float DelayTime = GetBackOffTime(RequestTime, ResponseTime);
	if (DelayTime <= 0)
	{
		EndTimerReceiveMissionReward();
	}
	else
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(LoopReceiveMissionRewardTH, this, &UB2UIMissionList::EndTimerReceiveMissionReward, DelayTime, false);
		}
	}
}

void UB2UIMissionList::EndTimerReceiveMissionReward()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(LoopReceiveMissionRewardTH);
		RequestTime = GetWorld()->GetTimeSeconds();
	}

	if (CompleteMissionIDs.Num())
	{
		data_trader::Retailer::GetInstance().RequestReceiveMissionReward(CompleteMissionIDs[0]);
		CompleteMissionIDs.RemoveAt(0);
	}
	else if(SelectedMissionType != EMissionType::Serial)
	{
		int32 PointRewardIndex = CheckMissionPointReward(SelectedMissionType);															//한번더 검사
		int32 ServerMissionType = ConvertMissionType(SelectedMissionType);
		data_trader::Retailer::GetInstance().RequestReceiveCompleteMissionReward(ServerMissionType, PointRewardIndex);
	}

}

void UB2UIMissionList::GetAllMissionReward()
{
	if(CompleteMissionIDs.Num() > 0)
		CompleteMissionIDs.Empty();

	if (UB2UIManager::GetInstance()->GetUI<UB2UIClientIndicator>(UIFName::ClientIndicator) == nullptr)
	{
		UB2UIManager::GetInstance()->OpenUI(UIFName::ClientIndicator);
	}

	CompleteMissionIDs = MissionManager::GetInstance().GetCompleteMissionIDList(SelectedMissionType);
	StartTimerReceiveMissionReward();
	bRequestRewardAllCompleteMission = true;
	bLoopReceiveSerialRewards = (SelectedMissionType == EMissionType::Serial);
}

int32 UB2UIMissionList::CheckMissionPointReward(EMissionType InMissiontype)
{
	int32 TotalRewardBoxIndex = 3;
	int32 CurrentPoint = MissionManager::GetInstance().GetMissionPoint(InMissiontype);

	for (int32 i = 0; i < TotalRewardBoxIndex; ++i)
	{
		MissionPointRewardPtr RewardPtr = MissionManager::GetInstance().GetMissionPointRewardInfo(SelectedMissionType, i + 1);
		if (RewardPtr)
		{
			if (RewardPtr->bRewarded == true)
				continue;

			if (CurrentPoint >= RewardPtr->RequirePoint)
				return i + 1;
		}
	}

	return 0;
}

void UB2UIMissionList::CheckDoneReceiveSerialRewards()
{
	if (bLoopReceiveSerialRewards)
	{
		if (MissionManager::GetInstance().GetCompleteMissionIDList(SelectedMissionType).Num() < 1)
		{
			bLoopReceiveSerialRewards = false;

			OpenMissionRewardDialog(CachedReceiveRewards);
			CachedReceiveRewards.Empty();

			ChangeMissionTabClass<const EMissionType, const bool>::GetInstance().Signal(SelectedMissionType, false);
		}
		else
		{
			if (GetWorld())
			{
				if (UB2UIManager::GetInstance()->GetUI<UB2UIClientIndicator>(UIFName::ClientIndicator) == nullptr)
				{
					UB2UIManager::GetInstance()->OpenUI(UIFName::ClientIndicator);
				}
				float DelayTime = GetBackOffTime(RequestTime, ResponseTime);
				if (DelayTime <= 0)
				{
					GetAllMissionReward();
				}
				else
				{
					GetWorld()->GetTimerManager().SetTimer(LoopReceiveSerialRewardsTH, this, &UB2UIMissionList::GetAllMissionReward, DelayTime, false);
				}
				
			}
		}
	}
	
}