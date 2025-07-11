// Fill out your copyright notice in the Description page of Project Settings.
#include "B2ErrorCodeInfo.h"
//#include "BladeII.h"



#include "FB2ErrorMessage.h"

UB2ErrorCodeInfo::UB2ErrorCodeInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		// ������ �ϳ��� �ε�
		FString ErrorCodeInfoDataTablePath;
		GConfig->GetString(TEXT("/Script/BladeII.B2ErrorCodeInfo"), TEXT("ErrorCodeInfoTable"), ErrorCodeInfoDataTablePath, GGameIni);

		TheData = LoadObject<UDataTable>(NULL, *ErrorCodeInfoDataTablePath);

#if WITH_EDITOR
		CheckInfoDataIntegrity();
#endif
	}
}

FSingleErrorCodeInfoData* UB2ErrorCodeInfo::GetInfoData(int32 nErrorCode)
{
	if (TheData)
	{
		FSingleErrorCodeInfoData* FoundDataRow = TheData->FindRow<FSingleErrorCodeInfoData>(FName(*FString::FromInt(nErrorCode)), TEXT(""));

		return FoundDataRow;
	}
	return NULL;
}

#if WITH_EDITOR
void UB2ErrorCodeInfo::CheckInfoDataIntegrity()
{
	if (TheData)
	{
		bool bHasRowKeyLessThan0 = false;
		bool bHasNonSequentialRowKey = false;

		// DataTable �� ��� row �� iterate �ϴ� ����ε�.
		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			int32 RowKeyNumber = FCString::Atoi(*RowKeys[RI].ToString());

			if (RowKeyNumber < 0)
			{
				bHasRowKeyLessThan0 = true;
			}
			if (RI + 1 != RowKeyNumber) // bHasRowKeyLessThan0 �̸� �ᱹ ���⵵..
			{
				bHasNonSequentialRowKey = true;
			}
		}

		if (bHasRowKeyLessThan0 /*|| bHasNonSequentialRowKey*/) // ��������� �ʿ�� ���� �� ����. �ϴ� �̰� ����� ����.
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[���] ErrorCodeInfo �������� �ٹ�ȣ�� ġ������ ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
				));
#endif
		}
	}
	else
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("ErrorCodeInfo ������ ����. ��ǻ�Ͱ� �� �����Ѵ�."))
			));
#endif
	}
}
#endif