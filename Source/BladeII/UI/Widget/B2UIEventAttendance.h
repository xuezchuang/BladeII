// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIEventAttendance.generated.h"


UCLASS()
class BLADEII_API UB2UIEventAttendance : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	bool IsPeriod() const;
	bool IsReceptiblePeriod() const; // Payback period
	bool IsEntirePeriod() const; // Period set by game Designer
	bool IsNewReward() const;
	void ReceiveEventAttendance(int32 EventId, int32 ReciveRewardDay, b2network::B2RewardPtr Reward);
	
	void SetEventSpecialDay();
	void SetEventAttendanceParts(int32 iPartsIndex, int32 iRewardIndex, int32 iAttendanceDay, 
		TArray<FEventAttendanceReward>& rAttendanceRewards);
	void SetPakageSlotVisibleState(bool IsVisible);

	void InitEventInfo();
	void UpdateEventAttendanceParts();
	void UpdateEventAttendancePackageState();
	bool IsPossibleDoubleReward(const int32 BougthPackageCount);

	void ResponseGetPeriodPackageState(FB2ResponseGetPeriodPackageStatePtr ReciveGetPeriodPackageState);

	UFUNCTION()
	void OnClickBtnGotoUrl();

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

private:
	TWeakObjectPtr<UTextBlock>	TB_GotoURL;
	TWeakObjectPtr<UButton>		BTN_GotoURL;
	TWeakObjectPtr<UHorizontalBox>		HB_PakageSlot;

	TArray<TWeakObjectPtr<class UB2UIEventAttendacnePart>>		EventAttendanceParts;
	TArray<TWeakObjectPtr<class UB2UIEventAttendancePakgeSlot>> EventAttendancePakageSlots;

	b2network::B2mdEventAttendance	CachedEventInfo;

	bool							IsDoubleReward;
	TArray<issue_ptr>	Issues;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpecialDay)
		TArray<int32> SpecialDay;
};
