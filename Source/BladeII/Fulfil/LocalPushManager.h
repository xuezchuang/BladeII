// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"

/**
 * 
 */
#define DELAY_TIME 30;

class BLADEII_API LocalPushManager
{
public:
	LocalPushManager();
	~LocalPushManager();

	void	 SetLocalPushSchedule_RestPoint();
	void	 CancelLocalPushSchedule_RestPoint();
	int32	 GetLocalPushTime_RestPoint();
};
