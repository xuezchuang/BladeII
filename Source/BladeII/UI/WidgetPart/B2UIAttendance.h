// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UIAttendance.generated.h"

UCLASS()
class BLADEII_API UB2UIAttendance : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init	() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;
		
	void ReceiveReward(const FAttendanceRewardInfo& DailyAttendanceRewardInfo);

protected:
	virtual void CacheAssets() override;

private:
	TWeakObjectPtr<UB2RichTextBlock>	TB_Noti;
	TArray<TWeakObjectPtr<class UB2UIAttendanceParts>> UIP_AttendanceParts;
	FTimerHandle TimeOpenPopup;
	
	void SetAttendanceData();
};
