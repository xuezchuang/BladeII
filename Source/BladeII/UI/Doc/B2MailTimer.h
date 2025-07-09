// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class UB2MailTimer
{
	
public:
	void StartTimerRequestOpenMail(int64 InMailID, EPCClass InEPCClass, int32 InItemRefId);
	void EndTimerRequestOpenMail();
	void SetResponseTime();
	
private:
	FTimerHandle MailTimerTH;
	float RequestTime;
	float ResponseTime;
	int64 MailID;
	EPCClass SelectEPCClass;
	int32 SelectedItemRefId;
};
