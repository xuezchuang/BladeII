// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2MailTimer.h"
#include "B2UIClientIndicator.h"
#include "Retailer.h"
#include "B2UIManager.h"



void UB2MailTimer::StartTimerRequestOpenMail(int64 InMailID, EPCClass InEPCClass, int32 InItemRefId)
{
	MailID = InMailID;
	SelectEPCClass = InEPCClass;
	SelectedItemRefId = InItemRefId;

	float DelayTime = GetBackOffTime(RequestTime, ResponseTime);

	UWorld* MailWorld = UB2UIManager::GetInstance()->GetWorld();

	if (MailWorld && DelayTime > 0)
	{
		FTimerDelegate Func;
		Func.BindLambda([this]()
		{
			this->EndTimerRequestOpenMail();
		});
		MailWorld->GetTimerManager().SetTimer(MailTimerTH, Func, DelayTime, false);
	}
	else
	{
		EndTimerRequestOpenMail();
	}
}

void UB2MailTimer::EndTimerRequestOpenMail()
{
	UWorld* MailWorld = UB2UIManager::GetInstance()->GetWorld();
	if (MailWorld)
	{
		MailWorld->GetTimerManager().ClearTimer(MailTimerTH);
		RequestTime = MailWorld->GetTimeSeconds();
	}

	if (SelectEPCClass == EPCClass::EPC_End)
	{
		data_trader::Retailer::GetInstance().RequestOpenMail(MailID, 0, SelectedItemRefId, 0);
	}
	else
	{
		data_trader::Retailer::GetInstance().RequestOpenMail(MailID, CliToSvrPCClassType(SelectEPCClass), SelectedItemRefId, 0);
	}
}

void UB2MailTimer::SetResponseTime()
{
	UWorld* MailWorld = UB2UIManager::GetInstance()->GetWorld();
	if (MailWorld)
	{
		ResponseTime = MailWorld->GetTimeSeconds();
	}
}
