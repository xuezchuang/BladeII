// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIAttendanceParts.h"
#include "B2UIAttendanceRow.generated.h"


UCLASS()
class BLADEII_API UB2UIAttendanceRow : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	TArray<UB2UIAttendanceParts*> getAttendancePart();

protected:

private:
	TWeakObjectPtr<UB2UIAttendanceParts> UIP_AttendanceParts_1;
	TWeakObjectPtr<UB2UIAttendanceParts> UIP_AttendanceParts_2;
	TWeakObjectPtr<UB2UIAttendanceParts> UIP_AttendanceParts_3;
	TWeakObjectPtr<UB2UIAttendanceParts> UIP_AttendanceParts_4;
	TWeakObjectPtr<UB2UIAttendanceParts> UIP_AttendanceParts_5;
	TWeakObjectPtr<UB2UIAttendanceParts> UIP_AttendanceParts_6;
	TWeakObjectPtr<UB2UIAttendanceParts> UIP_AttendanceParts_7;

	TArray<UB2UIAttendanceParts*> m_AttendanceParts;
};
