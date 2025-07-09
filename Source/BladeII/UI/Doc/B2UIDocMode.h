// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "CommonStruct.h"
#include "B2UIDocMode.generated.h"

/* This purpose is to hold miscellaneous UI-related data. */
UCLASS()
class BLADEII_API UB2UIDocMode : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocMode(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocMode();

public:
	// B2MessageInfoCommonStruct.h
	// Key : b2network::B2ContentsMode
	// Value : b2network::B2ContentsModeState
	TMap<int32, int32> ContentsModeInfos;

	void ReceiveGetContentsModeState(TArray<b2network::B2ContentsModeInfoPtr>& ModeInfos);
	void ReceiveSingleGetContentsModeState(int32 ContentsMode, int32 ContentsModeState);

	// ContentsMode - b2network::B2ContentsMode
	int32 GetContentsModeState(int32 ContentsMode);

private:
	uint32 DeliveryResponseGetContentsModeStateTicket;
};
