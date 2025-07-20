// Fill out your copyright notice in the Description page of Project Settings.


#include "B2ItemSortInfo.h"
#include "Misc/ConfigCacheIni.h" 

UB2ItemSortInfo::UB2ItemSortInfo(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer), DataTable(nullptr)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// 沥秦柳 窍唱甫 肺爹
		FString ItemSortInfoTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.ItemSortInfo"), TEXT("ItemSortInfoTable"), ItemSortInfoTablePath, GGameIni);

		DataTable = LoadObject<UDataTable>(NULL, *ItemSortInfoTablePath);
		TArray<FSingleItemSortInfoData*> Datas;
		DataTable->GetAllRows(TEXT("ItemSortTable"), Datas);
		for(auto& Element : Datas)
		{
			ItemSortDataMap.Add(Element->ItemRefID, Element->ItemSortValue);
		}
	}
}
void UB2ItemSortInfo::PostLoad()
{
}

int32 UB2ItemSortInfo::GetItemSortValue(int32 ItemRefID)
{
	if (ItemSortDataMap.Contains(ItemRefID))
		return ItemSortDataMap[ItemRefID];

	return INDEX_NONE;
}
