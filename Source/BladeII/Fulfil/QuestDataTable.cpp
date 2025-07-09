#include "BladeII.h"
#include "QuestManager.h"
#include "QuestDataTable.h"


#include "FB2ErrorMessage.h"
#include "BladeIIUtil.h"

UB2QuestInfoTable::UB2QuestInfoTable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		FString MainDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2QuestInfo"), TEXT("MainQuestTable"), MainDataTablePath, GGameIni);
		
		MainQuestTable = LoadObject<UDataTable>(NULL, *MainDataTablePath);

		GConfig->GetString(TEXT("/Script/BladeII.B2QuestInfo"), TEXT("DailyQuestTable"), MainDataTablePath, GGameIni);

		DailyQuestTable = LoadObject<UDataTable>(NULL, *MainDataTablePath);

#if WITH_EDITOR
		CheckInfoDataIntegrity(MainQuestTable);
		CheckInfoDataIntegrity(DailyQuestTable);
#endif
	}
}

void UB2QuestInfoTable::SetCommonTable(UDataTable* DataTable)
{
	if (DataTable != nullptr)
		MainQuestTable = DataTable;
}

FQuestCommonInfo* UB2QuestInfoTable::GetQuestMainInfo(int32 QuestID)
{
	if (MainQuestTable != nullptr)
	{
		auto QuestInfo = MainQuestTable->FindRow<FQuestCommonInfo>(FName(*FString::FromInt(QuestID)), TEXT(""));
		if (QuestInfo != nullptr)
			return QuestInfo;
	}

	return nullptr;
}

FQuestCommonInfo* UB2QuestInfoTable::GetQuestDailyInfo(int32 QuestID)
{
	if (DailyQuestTable != nullptr)
	{
		auto QuestInfo = DailyQuestTable->FindRow<FQuestCommonInfo>(FName(*FString::FromInt(QuestID)), TEXT(""));
		if (QuestInfo != nullptr)
			return QuestInfo;
	}

	return nullptr;
}

FQuestCommonInfo* UB2QuestInfoTable::GetQuestInfo(QuestPtr TargetQuest)
{
	if (TargetQuest != nullptr)
	{
		if (TargetQuest->GetType() == MAIN_QUEST)
			return GetQuestMainInfo(TargetQuest->GetQuestId());
		else if (TargetQuest->GetType() == DAILY_QUEST)
			return GetQuestDailyInfo(TargetQuest->GetQuestId());
	}

	return nullptr;
}



#if WITH_EDITOR
void UB2QuestInfoTable::CheckInfoDataIntegrity(UDataTable* DataTable)
{
	if (DataTable)
	{
		bool bHasRowKeyLessThan0 = false;
		bool bHasNonSequentialRowKey = false;

		// DataTable 의 모든 row 를 iterate 하는 방법인듯.
		TArray<FName> RowKeys = DataTable->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			int32 RowKeyNumber = FCString::Atoi(*RowKeys[RI].ToString());

			if (RowKeyNumber <= 0)
			{
				bHasRowKeyLessThan0 = true;
			}
			if (RI + 1 != RowKeyNumber) // bHasRowKeyLessThan0 이면 결국 여기도..
			{
				bHasNonSequentialRowKey = true;
			}
		}
		if (bHasRowKeyLessThan0 /*|| bHasNonSequentialRowKey*/) // 순서대로일 필요는 없을 것 같다. 일단 이건 경고에서 제거.
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[경고] DataTable 데이터의 줄번호에 치명적인 문제가 있음. 컴퓨터가 곧 폭발한다."))
				));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("DataTable 데이터 없음. 컴퓨터가 곧 폭발한다."))
			));
#endif
	}
}
#endif


FText GetQuestInfoText(int32 QuestID, int32 QuestType)
{
	if (QuestType == MAIN_QUEST)
	{
		auto MasterData = QuestManager::GetInstance().GetMainQuestMasterInfo(QuestID);
		if (MasterData != nullptr)
		{
			return BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(QuestID));
		}
	}
	else if (QuestType == DAILY_QUEST)
	{
		auto MasterData = QuestManager::GetInstance().GetDailyQuestMasterInfo(QuestID);
		if (MasterData != nullptr)
		{
			return BladeIIGetLOCText(B2LOC_CAT_QUESTTEXT, FString::FromInt(QuestID));
		}
	}

	return FText::FromString(TEXT("Unknown Text"));
}


FText GetProgressText(int32 QuestID, int32 QuestType, int32 CurrentCount)
{
	const int32 GoalCount = QuestManager::GetInstance().GetQuestGoalCount(QuestID, QuestType);
	return FText::FromString(FString::Printf(TEXT("(%d / %d)"), CurrentCount, GoalCount));
}

FText GetProgressText(QuestPtr QuestObject)
{
	if (QuestObject != nullptr)
	{
		const int32 GoalCount = QuestManager::GetInstance().GetQuestGoalCount(QuestObject->GetQuestId(), QuestObject->GetType());
		int32 TargetCount = QuestObject->GetProgressCount() > GoalCount ? GoalCount : QuestObject->GetProgressCount();
		return FText::FromString(FString::Printf(TEXT("(%d / %d)"), TargetCount, GoalCount));
	}

	return FText::FromString(TEXT("(0 / 0)"));
}

FText GetProgressCompletedText(int32 QuestID, int32 QuestType)
{
	const int32 GoalCount = QuestManager::GetInstance().GetQuestGoalCount(QuestID, QuestType);
	return FText::FromString(FString::Printf(TEXT("(%d / %d)"), GoalCount, GoalCount));
}

FText GetCompressedText(const FText& OrgText, int32 LimitTextCount)
{
	static FString TagEnd(TEXT("</>"));

	FString OrgString = OrgText.ToString();

	FString StringTagBegin;
	const bool bHasColorTag = OrgString.Contains(TagEnd);
	if (bHasColorTag)
	{
		int32 LastTagBegin = OrgString.Find(TagEnd, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

		const int32 ColorTagEndIdx = 22; // <span color="#20ff20"> length - 정해진 형식이므로 Code에 박는다
		StringTagBegin = OrgString.Left(ColorTagEndIdx);
		OrgString = OrgString.Mid(ColorTagEndIdx, LastTagBegin - ColorTagEndIdx); // Color Tag를 제외한 순수 문자열
		// </> 마지막 Tag는 길이로 짤라서 버림
	}

	const int32 StringLength = OrgString.Len();
	if (StringLength > LimitTextCount)
	{
		OrgString.RemoveAt(LimitTextCount, StringLength - LimitTextCount, false);
		OrgString += TEXT("...");

		if (bHasColorTag)
		{
			OrgString = StringTagBegin + OrgString + TagEnd;
		}
		
		return FText::FromString(OrgString);
	}

	return OrgText;
}

int32 ParseTextFromSpecialChar(const FText& OriginalText, const FString& ParseString, TArray<FText>& OutParsedResult)
{
	FString OrgString = OriginalText.ToString();

	TArray<FString> ParsedStringList;
	OrgString.ParseIntoArray(ParsedStringList, *ParseString);
	for (const auto& ParsedString : ParsedStringList)
	{
		OutParsedResult.Add(FText::FromString(ParsedString));
	}

	return OutParsedResult.Num();
}

//======================================================================
// UB2QuestNPCSoundInfo.
//======================================================================
USoundCue* FQuestNPCSoundInfoData::GetSoundCue()
{
	return AssetPtr.LoadSynchronous();
}

//======================================================================
// UB2QuestNPCSoundInfo.
//======================================================================
UB2QuestNPCSoundInfo::UB2QuestNPCSoundInfo(const FObjectInitializer& ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 정해진 하나를 로딩
		FString QuestNPCSoundInfoTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2QuestInfo"), TEXT("QuestNPCSoundInfoTable"), QuestNPCSoundInfoTablePath, GGameIni);

		SetDataTable(LoadObject<UDataTable>(NULL, *QuestNPCSoundInfoTablePath));
#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

#if WITH_EDITOR
void UB2QuestNPCSoundInfo::CheckInfoDataIntegrity()
{
	if (NPCSoundInfoTable)
	{
		bool bHasRowKeyLessThan0 = false;
		bool bHasNonSequentialRowKey = false;

		// DataTable 의 모든 row 를 iterate 하는 방법인듯.
		TArray<FName> RowKeys = NPCSoundInfoTable->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			int32 RowKeyNumber = FCString::Atoi(*RowKeys[RI].ToString());

			if (RowKeyNumber < 0)
			{
				bHasRowKeyLessThan0 = true;
			}
			if (RI + 1 != RowKeyNumber) // bHasRowKeyLessThan0 이면 결국 여기도..
			{
				bHasNonSequentialRowKey = true;
			}
		}

		if (bHasRowKeyLessThan0 /*|| bHasNonSequentialRowKey*/) // 순서대로일 필요는 없을 것 같다. 일단 이건 경고에서 제거.
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[경고] UB2QuestNPCSoundInfo 데이터의 줄번호에 치명적인 문제가 있음. 컴퓨터가 곧 폭발한다."))
			));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("UB2QuestNPCSoundInfo 데이터 없음. 컴퓨터가 곧 폭발한다."))
		));
#endif
	}
}
#endif

void UB2QuestNPCSoundInfo::SetDataTable(UDataTable* InDataTable)
{
	if (InDataTable == NULL)
		return;

	NPCSoundInfoTable = InDataTable;

	QuestNPCSoundInfoMap.Empty();

	for (TMap<FName, uint8*>::TConstIterator It(NPCSoundInfoTable->GetRowMap()); It; ++It)
	{
		FName TextureName = It.Key();
		auto* SoundInfoData = reinterpret_cast<FQuestNPCSoundInfoData*>(It.Value());
		if (SoundInfoData)
		{
			auto* InstancePtr = QuestNPCSoundInfoMap.Find(SoundInfoData->NPCCode);
			if (InstancePtr)
			{
				InstancePtr->SoundCodes.Add(TextureName);
			}
			else
			{
				QuestNPCSoundInfoMap.Add(SoundInfoData->NPCCode, FQuestNPCSoundRow());
				QuestNPCSoundInfoMap[SoundInfoData->NPCCode].SoundCodes.Add(TextureName);
			}
		}
	}

// TEST
// 	GetNPCSound(FName(TEXT("Ramiel_001")), 100002);
// 	GetNPCSound(FName(TEXT("")), 100002);
}

USoundCue* UB2QuestNPCSoundInfo::GetNPCSound(const FName InRawSoundCode, const int32 NpcCode)
{
	if (!NPCSoundInfoTable) {
		return nullptr;
	}

	FName FindRowName = NAME_None;
	FQuestNPCSoundInfoData* FoundInfo = NULL;
	if (InRawSoundCode == NAME_None)
	{
		auto* InstancePtr = QuestNPCSoundInfoMap.Find(NpcCode);
		if (InstancePtr)
		{
			FindRowName = InstancePtr->SoundCodes[FMath::RandRange(0, InstancePtr->SoundCodes.Num() - 1)];
		}
	}
	else
	{
		FindRowName = InRawSoundCode;
	}

	FoundInfo = NPCSoundInfoTable->FindRow<FQuestNPCSoundInfoData>(FindRowName, TEXT(""));

	if (FoundInfo)
	{
		USoundCue* RetObj = FoundInfo->GetSoundCue();
#if WITH_EDITOR && !PLATFORM_MAC
		if (!RetObj)
		{
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("UB2QuestNPCSoundInfo SoundCode %s 에 대한 Sound Asset 로딩 실패. 컴퓨터가 곧 폭발한다."), *InRawSoundCode.ToString())
			));
		}
#endif
		return RetObj;
	}

	return nullptr;
}