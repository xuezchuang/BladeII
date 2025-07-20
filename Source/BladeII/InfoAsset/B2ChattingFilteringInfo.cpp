// Fill out your copyright notice in the Description page of Project Settings.


#include "B2ChattingFilteringInfo.h"
#include "BladeII.h"
#include "memory"

void UB2ChattingFilteringInfo::SetDataTable(UDataTable* InDataTable)
{
	if (InDataTable == NULL)
		return;

	/*ChattingFilteringData.Empty(ChattingFilteringInfoTable->RowMap.Num());
	for (TMap<FName, uint8*>::TConstIterator It(ChattingFilteringInfoTable->RowMap); It; ++It)
	{
		auto* ChattingFilteringInfoData = reinterpret_cast<FChattingFilteringInfoData*>(It.Value());
		if (ChattingFilteringInfoData)
		{
			ChattingFilteringData.Add(ChattingFilteringInfoData->Filtering);
		}
	}*/
	RootFilteringNode = std::make_shared<FilteringNode>();
	for (TMap<FName, uint8*>::TConstIterator It(InDataTable->GetRowMap()); It; ++It)
	{
		auto* Data = reinterpret_cast<FChattingFilteringInfoData*>(It.Value());
		if (Data && Data->Slang.IsEmpty() == false)
		{
			FString Text = Data->Slang.ToUpper();

			FFilteringNodePtr CurrentNode = RootFilteringNode;
			for (const TCHAR Ch : Text)
			{
				CurrentNode = CurrentNode->AddChild(Ch);
			}
			CurrentNode->SetIsEnd(true);
		}
	}
}

FText UB2ChattingFilteringInfo::Filtering(const FString& Message)
{
	FString Result = Message;

	int CheckStartIDX = 0;

	for (CheckStartIDX = 0; CheckStartIDX < Result.Len();)
	{
		int Count = GetCount(Result.ToUpper(), CheckStartIDX);
		if (Count > 0)
		{
			for (int j = CheckStartIDX; j < Count; j++)
			{
				Result[j] = '*';
			}
			CheckStartIDX = Count;
		}
		else
		{
			CheckStartIDX++;
		}
	}
	/*for (FString& Filtering : ChattingFilteringData)
	{
		if (Result.Contains(Filtering)) 
		{
			Result = Result.Replace(*Filtering, TEXT("*"));
		}
	}*/
	return FText::FromString(Result);
}

int32 UB2ChattingFilteringInfo::GetCount(const FString& Message, int32 StartIndex)
{
	FFilteringNodePtr CurrentNode = RootFilteringNode;

	int32 MatchCount = 0;
	for (int i = StartIndex; i < Message.Len(); i++)
	{
		if (CurrentNode->HasChild(Message[i]) == false)
		{
			break;
		}
		else
		{
			CurrentNode = CurrentNode->GetChild(Message[i]);
		}

		if (CurrentNode->GetIsEnd())
		{
			MatchCount = i + 1;
		}
	}
	return MatchCount;
}

FFilteringNodePtr FilteringNode::AddChild(const TCHAR& Value)
{
	FFilteringNodePtr resultNode(nullptr);
	if (ChildNode.Contains(Value))
	{
		resultNode = ChildNode[Value];
	}
	else
	{
		resultNode = ChildNode.Add(Value, std::make_shared <FilteringNode>());
	}
	return resultNode;
}

bool FilteringNode::HasChild(const TCHAR& Key)
{
	return ChildNode.Contains(Key);
}

FFilteringNodePtr FilteringNode::GetChild(const TCHAR& Key)
{
	return ChildNode[Key];
}

