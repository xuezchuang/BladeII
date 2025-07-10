// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIAttendanceRow.h"
#include "B2UIManager.h"



void UB2UIAttendanceRow::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2UIAttendanceParts, UIP_AttendanceParts_1);
	if(UIP_AttendanceParts_1.IsValid()) UIP_AttendanceParts_1->Init();
	GET_SLOT(UB2UIAttendanceParts, UIP_AttendanceParts_2);
	if(UIP_AttendanceParts_2.IsValid()) UIP_AttendanceParts_2->Init();
	GET_SLOT(UB2UIAttendanceParts, UIP_AttendanceParts_3);
	if(UIP_AttendanceParts_3.IsValid()) UIP_AttendanceParts_3->Init();
	GET_SLOT(UB2UIAttendanceParts, UIP_AttendanceParts_4);
	if(UIP_AttendanceParts_4.IsValid()) UIP_AttendanceParts_4->Init();
	GET_SLOT(UB2UIAttendanceParts, UIP_AttendanceParts_5);
	if(UIP_AttendanceParts_5.IsValid()) UIP_AttendanceParts_5->Init();
	GET_SLOT(UB2UIAttendanceParts, UIP_AttendanceParts_6);
	if(UIP_AttendanceParts_6.IsValid()) UIP_AttendanceParts_6->Init();
	GET_SLOT(UB2UIAttendanceParts, UIP_AttendanceParts_7);
	if(UIP_AttendanceParts_7.IsValid()) UIP_AttendanceParts_7->Init();

	m_AttendanceParts.Empty();

	m_AttendanceParts.Add(UIP_AttendanceParts_1.Get());
	m_AttendanceParts.Add(UIP_AttendanceParts_2.Get());
	m_AttendanceParts.Add(UIP_AttendanceParts_3.Get());
	m_AttendanceParts.Add(UIP_AttendanceParts_4.Get());
	m_AttendanceParts.Add(UIP_AttendanceParts_5.Get());
	m_AttendanceParts.Add(UIP_AttendanceParts_6.Get());
	m_AttendanceParts.Add(UIP_AttendanceParts_7.Get());
}

void UB2UIAttendanceRow::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIAttendanceRow::Init()
{
	Super::Init();
}

void UB2UIAttendanceRow::DestroySelf(UB2UIManager* InUIManager)
{

	if(UIP_AttendanceParts_1.IsValid()) UIP_AttendanceParts_1->DestroySelf(InUIManager);
	if(UIP_AttendanceParts_2.IsValid()) UIP_AttendanceParts_2->DestroySelf(InUIManager);
	if(UIP_AttendanceParts_3.IsValid()) UIP_AttendanceParts_3->DestroySelf(InUIManager);
	if(UIP_AttendanceParts_4.IsValid()) UIP_AttendanceParts_4->DestroySelf(InUIManager);
	if(UIP_AttendanceParts_5.IsValid()) UIP_AttendanceParts_5->DestroySelf(InUIManager);
	if(UIP_AttendanceParts_6.IsValid()) UIP_AttendanceParts_6->DestroySelf(InUIManager);
	if(UIP_AttendanceParts_7.IsValid()) UIP_AttendanceParts_7->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}

TArray<UB2UIAttendanceParts*> UB2UIAttendanceRow::getAttendancePart()
{
	return m_AttendanceParts;
}

