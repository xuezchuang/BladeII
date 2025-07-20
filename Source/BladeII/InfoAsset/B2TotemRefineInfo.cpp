#include "B2TotemRefineInfo.h"
#include "Misc/ConfigCacheIni.h"
UB2TotemRefineInfo::UB2TotemRefineInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), DataTable(nullptr)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 沥秦柳 窍唱甫 肺爹
		FString TotemRefineInfoTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2TotemRefineInfo"), TEXT("TotemRefineInfoTable"), TotemRefineInfoTablePath, GGameIni);

		DataTable = LoadObject<UDataTable>(NULL, *TotemRefineInfoTablePath);
		TArray<FTotemRefineInfoData*> Datas;
		DataTable->GetAllRows(TEXT("TotemRefineInfoTable"), Datas);
		for (auto& Element : Datas)
		{
			TotemRefineDataMap.Add(Element->RefineSuccessPrefixLevel, Element->RefineLevel);
		}
	}
}
void UB2TotemRefineInfo::PostLoad()
{
}

int32 UB2TotemRefineInfo::GetTotemRefineValue(ETotemRefinePrefix PrefixLevel)
{
	int32 TotemRefinePrefixLevel = CliToSvrTotemRefinePrefix(PrefixLevel);

	if (TotemRefineDataMap.Contains(TotemRefinePrefixLevel))
		return TotemRefineDataMap[TotemRefinePrefixLevel];

	return INDEX_NONE;
}