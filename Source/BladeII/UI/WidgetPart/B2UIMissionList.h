// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UIMissionList.generated.h"


enum class EMissionType : uint8;
/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIMissionList : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void UpdateMissionSlotList(EMissionType MissionType);
	void SetAllRewardButtonDisable(bool bDisable);
	void SetListScrollOffSet(float OffSet);

	bool IsLoopReceiveSerialRewards() { return bLoopReceiveSerialRewards; }
	void CheckDoneReceiveSerialRewards();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UB2UIPMissionSlot>	MissionSlotClass;

private:
	virtual void CacheAssets() override;

	void SubscribeEvent();
	void UnSubscribeEvent();
	TWeakObjectPtr<UB2UIPMissionSlot> CreateMissionSlot();
	TWeakObjectPtr<class UB2UIPMissionSlot> GetMissionSlot(int32 Index);
	void OnReceiveMissionReward(const FB2ReceiveMissionReward& MissionReward);
	void OpenMissionRewardDialog(const TArray<b2network::B2RewardPtr>& Rewards);
	void OnReceiveMissionPointReward(const FB2ReceiveCompleteMissionReward& CompleteReward);
	void OnReceiveGetContentsOpenStatus(const FB2ResponseGetContentsOpenStatusPtr& GetContentsOpenStatusPtr);
	class UB2UIMsgPopupReward* OpenRewardDialog(const TArray<b2network::B2RewardPtr>& Rewards);
	void OnClickGetRewardMsgPopup();
	void ReqGetContetnsOpenStatus(int32 MissionID);
	void OpenContentsFailMsgPopup();
	void StartTimerReceiveMissionReward();
	void EndTimerReceiveMissionReward();
	void GetAllMissionReward();
	int32 CheckMissionPointReward(EMissionType InMissiontype);

	TWeakObjectPtr<UB2ScrollBox>	SB_Mission;
	TArray<TWeakObjectPtr<class UB2UIPMissionSlot>> MissionSlots;
	TArray<b2network::B2RewardPtr> CachedReceiveRewards;
	TArray<int32>	CompleteMissionIDs;
	FTimerHandle	LoopReceiveMissionRewardTH;
	FTimerHandle	LoopReceiveSerialRewardsTH;
	EMissionType	SelectedMissionType;
	float RequestTime;
	float ResponseTime;
	bool			bRequestRewardAllCompleteMission = false;
	bool			bLoopReceiveSerialRewards = false;
	int32			CheckMissionID = 0;
	uint8			DeliveryReceiveMissionRewardTicket = 0;
	uint8			DeliveryReceiveCompleteMissionRewardTicket = 0;
	uint8			DeliveryGetContentsOpenStatusTicket = 0;
	uint8			GetAllMIssionQuestRewardEventID = 0;
};
