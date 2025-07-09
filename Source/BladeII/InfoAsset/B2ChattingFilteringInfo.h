// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "B2ChattingFilteringInfo.generated.h"


USTRUCT()
struct FChattingFilteringInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Slang;
};

class FilteringNode;
using FFilteringNodePtr = std::shared_ptr<FilteringNode>;

class FilteringNode
{
private:
	TMap<TCHAR, FFilteringNodePtr> ChildNode;
	bool IsEnd;
public:
	FFilteringNodePtr AddChild(const TCHAR& Key);
	FFilteringNodePtr GetChild(const TCHAR& Key);
	bool HasChild(const TCHAR& Key);

	bool GetIsEnd() { return IsEnd; }
	void SetIsEnd(bool Value) { IsEnd = Value; }
};

UCLASS()
class BLADEII_API UB2ChattingFilteringInfo : public UObject
{
	GENERATED_BODY()
public:
	void SetDataTable(UDataTable* InDataTable);
	FText Filtering(const FString& Message);
private:
	FFilteringNodePtr RootFilteringNode;
	int32 GetCount(const FString& Message, int32 StartIndex);
};


