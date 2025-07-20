// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "MissionManager.h"
#include "B2UIMission.generated.h"

/**
 * 
 */

enum class ETabType : int8
{
	None = -1,
	Daily,
	Weekly,
	Monthly,
	Serial,
	MainQuest,
	SubQuest,
	Max
};

struct FMissionTab
{
	FMissionTab() 
		: Type(ETabType::Max), BTN_Tab(nullptr), NewIcon(nullptr), bAllowedGetAllReward(false), IsCompleteProgress(false), TabTextKeyString(TEXT("")) {}

	ETabType					Type;
	TWeakObjectPtr<UButton>		BTN_Tab;
	TWeakObjectPtr<UImage>		IMG_Tab[2];
	TWeakObjectPtr<UTextBlock>	TB_Tab[2];
	TWeakObjectPtr<class UB2UIWidgetBase>		NewIcon;

	bool						bAllowedGetAllReward;
	bool						IsCompleteProgress;
	FString						TabTextKeyString;

	void SetMissionTabType(ETabType InTabType)
	{
		switch (InTabType)
		{
		case ETabType::Daily:
			TabTextKeyString = TEXT("Mission_Daily"); 
			IsCompleteProgress = true; 
			break;
		case ETabType::Weekly:
			TabTextKeyString = TEXT("Mission_Weekly"); 
			IsCompleteProgress = true; 
			break;
		case ETabType::Monthly:
			TabTextKeyString = TEXT("Mission_Monthly"); 
			IsCompleteProgress = true; 
			break;
		case ETabType::Serial:
			TabTextKeyString = TEXT("Mission_Normal");
			bAllowedGetAllReward = true;
			break;
		case ETabType::MainQuest:
			TabTextKeyString = TEXT("Quest_Main");
			break;
		case ETabType::SubQuest:
			TabTextKeyString = TEXT("Quest_Sub");
			break;
		}

		Type = InTabType;
	}

	void SetTabVisible(bool bVisible)
	{
		if (bVisible)
		{
			SetTabSelected(false); // 老窜 急琶 救等惑怕肺 Visible
		}
		else
		{
			if (BTN_Tab.IsValid())	BTN_Tab->SetVisibility(ESlateVisibility::Collapsed);
			if (TB_Tab[0].IsValid() && TB_Tab[1].IsValid())
			{
				TB_Tab[0]->SetVisibility(ESlateVisibility::Collapsed);
				TB_Tab[1]->SetVisibility(ESlateVisibility::Collapsed);
			}
			if (IMG_Tab[0].IsValid() && IMG_Tab[1].IsValid())
			{
				IMG_Tab[0]->SetVisibility(ESlateVisibility::Collapsed);
				IMG_Tab[1]->SetVisibility(ESlateVisibility::Collapsed);
			}

		}
	}

	void SetTabNewIcon(bool bNew)
	{
		if (NewIcon.IsValid())
		{
			NewIcon->SetVisibility(bNew ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}

	void SetTabSelected(bool bSelected);
};

UCLASS()
class BLADEII_API UB2UIMission : public UB2UIWidget
{
	GENERATED_BODY()

public:
	UB2UIMission(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UFUNCTION()
		void OnClickDailyTab();
	UFUNCTION()
		void OnClickWeeklyTab();
	UFUNCTION()
		void OnClickMonthlyTab();
	UFUNCTION()
		void OnClickSerialTab();
	UFUNCTION()
		void OnClickMainQuestTab();
	UFUNCTION()
		void OnClickSubQuestTab();

	void ChangeTapShortcut(EMissionType Type);
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void OnHistoryOpen(bool IsHistory) override;
	
private:
	void OnChangedMissionReward(const FB2ReceiveMissionReward& ReceiveMissionReward);
	void RefreshTabs();
	void ChangeTab(ETabType InTabType, bool bScrollReset);
	void SubscribeEvent();
	void UnSubscribeEvent();
	void OnReceiveMissionData(const FB2GetMission& MissionData);
	void OnReceiveMissionPointReward(const FB2ReceiveCompleteMissionReward& CompleteReward);
	EMissionType ConvertMissionType(ETabType InTabType);
	ETabType ConvertTabType(EMissionType InMissionType);

	TMap<ETabType, FMissionTab>		Tabs;
	TWeakObjectPtr<UOverlay>		O_EventDeco;
	TWeakObjectPtr<class UB2UIPMIssionCompleteProgress>	UIP_MissionCompleteProgress;
	TWeakObjectPtr<class UB2UIMissionList>	UIP_MissionList;
	TWeakObjectPtr<class UB2UIPMainQuest> UIP_MainQuest;
	TWeakObjectPtr<class UB2UIPSubQuest> UIP_SubQuest;
	uint8			DeliveryGetMissionTicket;
	uint8			DeliveryReceiveCompleteMissionRewardTicket;
	ETabType		SelectedTabType;
};