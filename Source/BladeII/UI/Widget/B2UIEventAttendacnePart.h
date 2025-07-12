// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "../B2RichTextBlock.h"
#include "../../../B2Network/Public/B2MessageInfoCommonStruct.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "B2UIEventAttendacnePart.generated.h"

enum class EEventRewardState : uint32
{
	REWARD_WAIT,
	REWARD_ENABLE,
	REWARD_COMPLETE,
};


UCLASS()
class BLADEII_API UB2UIEventAttendacnePart : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void OnClickAttendanceEvent();
	void OnClickNewUserEvent();

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	const int32 GetRewardDay() { return CachedRewardInfo.day; }

	void SetSpecialDay(bool InSpecial) { IsSpecialDay = InSpecial; }
	void SetNewUserInfo(const FAttendanceDayData& InDayInfo);
	void SetCachedEventInfo(const b2network::B2mdEventAttendance& rEventInfo);
	void SetIsDoubleReward(const bool& IsDouble) { IsDoubleReward = IsDouble; }
	void SetEventPartsType(EAttendanceTab InType) { PartsType = InType; }

	void InitRewardSlotInfo(const FEventAttendanceReward& RewardInfo);
	void UpdateRewardState(EEventRewardState eRewardState, b2network::B2RewardPtr Reward = nullptr);
	void UpdateGetRewardBtnText(const bool& IsPossibleDoubleReward);

public:
	UFUNCTION(BlueprintCallable, Category = Animations)
	void FinishRecieveAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void OnStartRewardAnimation_BP();

protected:
	UFUNCTION()
	void OnClickReceive();

	void OpenPopupBuyPackageOrRequestReward();
	void EnterPackageShop();

private:
	TWeakObjectPtr<UB2Image>		 IMG_ITEM;
	TWeakObjectPtr<UImage>			 IMG_Day;
	TWeakObjectPtr<UImage>			 IMG_BackGround;
	TWeakObjectPtr<UImage>			 IMG_BackGroundSpecial;
	TWeakObjectPtr<UImage>			 IMG_ButtonEffect;
	TWeakObjectPtr<UB2Button>		 BTN_Receive;
	TWeakObjectPtr<UTextBlock>		 TB_RewardCount;
	TWeakObjectPtr<UTextBlock>		 TB_Day;
	TWeakObjectPtr<UB2RichTextBlock> TB_ItemName;
	TWeakObjectPtr<UB2RichTextBlock> TB_Receive;

	TWeakObjectPtr<UWidgetAnimation> Anim_Recieve;
	TWeakObjectPtr<UPanelWidget>	 P_Complete;
	TWeakObjectPtr<UPanelWidget>     P_ReceiveButton;
	TWeakObjectPtr<UPanelWidget>	 P_BackGroundSpecial;
	b2network::B2RewardPtr			 RewardEventItem;

	TWeakObjectPtr<UTextBlock>		TB_GetReward;

	b2network::B2mdEventAttendance	CachedEventInfo;
	FEventAttendanceReward			CachedRewardInfo;
	EAttendanceTab					PartsType;
	bool							IsSpecialDay;
	bool							IsDoubleReward;
};
