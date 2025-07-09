// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2TalkInfo.h"

FTalkLine::FTalkLine(struct FTalkInfoData* InfoData)
{
	check(InfoData);
	if (InfoData != NULL)
	{
		GroupID = InfoData->GroupID;
		AppearType = InfoData->AppearType;
		SpeakerLeave = InfoData->SpeakerLeave;
		ReverseImage = InfoData->ReverseImage;
		ModifyDepth = InfoData->ModifyDepth;
		ShowingTime = InfoData->ShowingTime;
		DelayToNextLine = InfoData->DelayToNextLine;
		SpeakerCodeName = InfoData->SpeakerCodeName;
		TalkLineCodeName = InfoData->TalkLineCodeName;
		TalkSoundCodeName = InfoData->TalkSoundCodeName;
		PortraitPosition = InfoData->PortraitPosition;
		BackgroundCode = InfoData->BackgroundCode;
	}
}

FTalkInfoInstance::FTalkInfoInstance(struct FTalkInfoData* InfoData)
{
	check(InfoData);
	if (InfoData != NULL)
	{
		GroupID = InfoData->GroupID;
		TalkSequence.Add(FTalkLine(InfoData));
	}
}

void UB2TalkInfo::SetDataTable(UDataTable* InDataTable)
{
	if (InDataTable == NULL)
		return;

	TalkInfoTable = InDataTable;

	TalkInfoMap.Empty(TalkInfoTable->GetRowMap().Num());

	for (TMap<FName, uint8*>::TConstIterator It(TalkInfoTable->GetRowMap()); It; ++It)
	{
		auto* TalkInfoData = reinterpret_cast<FTalkInfoData*>(It.Value());
		if (TalkInfoData)
		{
			auto* InstancePtr = TalkInfoMap.Find(TalkInfoData->GroupID);
			if (InstancePtr)
			{
				InstancePtr->TalkSequence.Add(FTalkLine(TalkInfoData));
			}
			else
			{
				TalkInfoMap.Add(TalkInfoData->GroupID, FTalkInfoInstance(TalkInfoData));
			}
		}
	}

}

const FTalkInfoInstance* UB2TalkInfo::GetTalkInfo(const int32 GroupID)
{
	return TalkInfoMap.Find(GroupID);
}


