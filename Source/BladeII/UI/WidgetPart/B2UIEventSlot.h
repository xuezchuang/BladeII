// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIEventSlot.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIEventSlot : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	virtual void UpdateVisibleState() {};

public:
	int32 GetEventIndex() { return iEventIndex; }
	int32 GetEventState() { return iEventState; }
	void SetEventState(int32 iState);

	void OpenMailRewardPopup();

protected:
	int32	iEventID;
	int32	iEventIndex;
	int32	iEventState = 1;	//1==default==progress
};
