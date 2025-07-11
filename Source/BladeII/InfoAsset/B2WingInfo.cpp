// Fill out your copyright notice in the Description page of Project Settings.


#include "B2WingInfo.h"
//#include "B2Airport.h"

#include "FB2ErrorMessage.h"


////////////////////////////////////////////////////////////////////////////////

USkeletalMesh* FSingleWingInfoData::GetWingMesh()
{
	return WingMesh.LoadSynchronous();
}

UMaterialInterface* FSingleWingInfoData::GetMtrlOverride()
{
	return MtrlOverride.LoadSynchronous();
}

#if WITH_EDITOR
void FSingleWingInfoData::EditorLoadAllTAsset()
{
	//if (!GIsEditor) {
	//	return;
	//}

	//// TAsset �� �����Ϳ��� �ε� �� �� ���� �� redirector ó���� �߸��� �� �־.. DataTable �ƴϸ� ������ �� ������ ������ ���ҽ����� �ε����ش�.
	//// ���۷��� ĳ�̵� ���� ���� �ִ��� �𸣰ڴµ� �����Ϳ��� ���� �־��� �� ����. 

	//WingMesh.LoadSynchronous();
	//MtrlOverride.LoadSynchronous();

	//for (FCHSKCompAttachParticleSystemInfo& ThisAttachEffect : AttachEffectInfoArray)
	//{
	//	ThisAttachEffect.GetParticleSystemAsset();
	//}
}
#endif

////////////////////////////////////////////////////////////////////////////////

UMaterialInterface* FWingPropOptionIconSetup::GetIconMtrl()
{
	return IconMtrl.LoadSynchronous();
}

#if WITH_EDITOR
void FWingPropOptionIconSetup::EditorLoadAllTAsset()
{
	if (!GIsEditor) {
		return;
	}

	// TAsset �� �����Ϳ��� �ε� �� �� ���� �� redirector ó���� �߸��� �� �־.. DataTable �ƴϸ� ������ �� ������ ������ ���ҽ����� �ε����ش�.
	// ���۷��� ĳ�̵� ���� ���� �ִ��� �𸣰ڴµ� �����Ϳ��� ���� �־��� �� ����. 

	IconMtrl.LoadSynchronous();
}
#endif

////////////////////////////////////////////////////////////////////////////////

UB2WingInfo::UB2WingInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2WingInfo::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITOR
	CheckInfoDataIntegrity();
#endif
}

TArray<FSingleWingInfoData>* UB2WingInfo::GetMainInfoArrayPtrForChar(EPCClass InCharClass)
{
	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
	if (InCharClass != EPCClass::EPC_End)
	{
		return (
			(InCharClass == EPCClass::EPC_Gladiator) ? &MainInfoArray_Gladiator : 
				(InCharClass == EPCClass::EPC_Assassin ? &MainInfoArray_Assassin : 
					(InCharClass == EPCClass::EPC_Wizard ? &MainInfoArray_Wizard : &MainInfoArray_Fighter))			
			);
	}
	return NULL;
}

FSingleWingInfoData* UB2WingInfo::GetInfoData(const FSingleWingInfoID& InID)
{
//	// ���⼭�� MAX_WING_EVOLUTION_GRADE_UNOFFICIAL ���� ����� �Ѵ�. ���� �ý��� �����δ� ��� �ȵǴ��� ���ҽ� ���������� ���� �� Ȯ���� �����ؾ�.
//	if (InID.PCClassEnum != EPCClass::EPC_End && InID.EvolutionGrade >= MIN_WING_EVOLUTION_GRADE && InID.EvolutionGrade <= MAX_WING_EVOLUTION_GRADE_UNOFFICIAL)
//	{
//		TArray<FSingleWingInfoData>* MainInfoDataOfClass = GetMainInfoArrayPtrForChar(InID.PCClassEnum);
//		if (MainInfoDataOfClass && MainInfoDataOfClass->Num() > 0)
//		{ 
//			int32 ArrayIndexOfGrade = InID.EvolutionGrade - 1; // �뷫 �̷��� grade �� index ���缭 ������ �ǵ�.
//			if (MainInfoDataOfClass->IsValidIndex(ArrayIndexOfGrade))
//			{
//				FSingleWingInfoData& WantedDataSet = (*MainInfoDataOfClass)[ArrayIndexOfGrade];
//				checkSlow(WantedDataSet.MyID == InID); // ó������ iteration �ϸ鼭 �´� ID ���� �� �����ϵ��� �� ���� �ִ�.
//				return &WantedDataSet;
//			}
//		}
//	}
//
//#if WITH_EDITOR && !PLATFORM_MAC
//	FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//			FString::Printf(TEXT("WingInfo ���� ID PC %d Evol %d �� �׸��� ã�� ����. ��ǻ�Ͱ� �� �����Ѵ�."), (int32)InID.PCClassEnum, InID.EvolutionGrade)
//			));
//#endif
	return NULL;
}

FSingleWingInfoData* UB2WingInfo::GetInfoData(const FB2Wing& InWing)
{
	return GetInfoData(FSingleWingInfoID(InWing));
}

FWingPropOptionIconSetup* UB2WingInfo::GetPropOptionIconSetup(EItemOption InOption)
{
	if (InOption != EItemOption::EIO_End)
	{
		for (int32 II = 0; II < PropOptionIconSetup.Num(); ++II)
		{
			FWingPropOptionIconSetup* ThisSetup = &PropOptionIconSetup[II];
			if (ThisSetup->PropOption == InOption)
			{
				return ThisSetup;
			}
		}
	}
	return NULL;
}

UMaterialInterface* UB2WingInfo::GetWingMainIconMtrlForGrade(int32 InEvolveGrade)
{
	TSoftObjectPtr<UMaterialInterface>* FoundAssetPtr = PerGradeWingMainIconMtrl.Find(InEvolveGrade);
	if (FoundAssetPtr)
	{
		return (*FoundAssetPtr).LoadSynchronous();
	}
	return NULL;
}

#if WITH_EDITOR
void UB2WingInfo::EditorLoadAll()
{
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		TArray<FSingleWingInfoData>* TheArrayToLoad = GetMainInfoArrayPtrForChar(IntToPCClass(PCI));
		if (TheArrayToLoad)
		{
			for (FSingleWingInfoData& ThisInfo : *TheArrayToLoad)
			{
				ThisInfo.EditorLoadAllTAsset();
			}
		}
	}

	for (FWingPropOptionIconSetup& ThisInfo : PropOptionIconSetup)
	{
		ThisInfo.EditorLoadAllTAsset();
	}

	for (TMap<int32, TSoftObjectPtr<UMaterialInterface>>::TIterator IconMatIt(PerGradeWingMainIconMtrl); IconMatIt; ++IconMatIt)
	{
		TSoftObjectPtr<UMaterialInterface>& ThisAssetPtr = IconMatIt.Value();
		ThisAssetPtr.LoadSynchronous();
	}
}

void UB2WingInfo::CheckInfoDataIntegrity()
{
//	int32 SameIDCount = 0;
//
//#if !PLATFORM_MAC
//	FString WarnMsgDupIDSetting = TEXT("WingInfo ���� �ߺ��� ID ���� �߰�\n\n");
//	FString RemovedNonAllowedSetupMsg = TEXT("���� ������ �ʴ� �¾��� ����\n\n");
//#endif
//
//	int32 NonAllowedSetupCount = 0;
//	
//	// ĳ���� �� MainInfoArray �� �ߺ��� ���� üũ ��� �� ID �� �ڵ����� �����ִ� �ɷ�.
//
//	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//	{
//		EPCClass ThisPCClassEnum = IntToPCClass(PCI);
//		TArray<FSingleWingInfoData>* TheArrayToCheck = GetMainInfoArrayPtrForChar(IntToPCClass(PCI));
//		if (TheArrayToCheck)
//		{
//			for (int32 WGI = 0; WGI < TheArrayToCheck->Num(); ++WGI)
//			{
//				FSingleWingInfoData& ThisMainInfoData = (*TheArrayToCheck)[WGI];
//				ThisMainInfoData.MyID.PCClassEnum = ThisPCClassEnum;
//				ThisMainInfoData.MyID.EvolutionGrade = WGI + 1;
//
//				if (ThisMainInfoData.MyID.EvolutionGrade < 7 && ThisMainInfoData.AttachEffectInfoArray.Num() > 0)
//				{
//					// 7��� �̸������� ��� ����. �Ǽ� ���� ����.
//					ThisMainInfoData.AttachEffectInfoArray.Empty();
//
//					++NonAllowedSetupCount;
//#if !PLATFORM_MAC
//					RemovedNonAllowedSetupMsg += FString::Printf(TEXT("%d. %d ��޿����� AttachEffectInfo ����\n"), NonAllowedSetupCount, ThisMainInfoData.MyID.EvolutionGrade);
//#endif
//				}
//			}
//		}
//	}
//
//	for (int32 IA = 0; IA < PropOptionIconSetup.Num(); ++IA)
//	{
//		const FWingPropOptionIconSetup& CheckDataA = PropOptionIconSetup[IA];
//		for (int32 IB = IA + 1; IB < PropOptionIconSetup.Num(); ++IB)
//		{
//			const FWingPropOptionIconSetup& CheckDataB = PropOptionIconSetup[IB];
//
//			if (CheckDataA.PropOption != EItemOption::EIO_End && CheckDataA.PropOption == CheckDataB.PropOption)
//			{
//				++SameIDCount;
//#if !PLATFORM_MAC
//				WarnMsgDupIDSetting += FString::Printf(TEXT("%d. At PropOptionIconSetup index %d and %d\n"), SameIDCount, IA, IB);
//#endif
//			}
//		}
//	}
//
//	if (SameIDCount > 0)
//	{
//#if !PLATFORM_MAC
//		WarnMsgDupIDSetting += TEXT("\n��ǻ�Ͱ� �� �����Ѵ�.");
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMsgDupIDSetting));
//#endif
//	}
//	if (NonAllowedSetupCount > 0)
//	{
//#if !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(RemovedNonAllowedSetupMsg));
//#endif
//	}
}

void UB2WingInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{ 
	//Super::PostEditChangeProperty(PropertyChangedEvent);

	//FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	//FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	//const FName Name_MainInfoArray_Gladiator = GET_MEMBER_NAME_CHECKED(UB2WingInfo, MainInfoArray_Gladiator);
	//const FName Name_MainInfoArray_Assassin = GET_MEMBER_NAME_CHECKED(UB2WingInfo, MainInfoArray_Assassin);
	//const FName Name_MainInfoArray_Wizard = GET_MEMBER_NAME_CHECKED(UB2WingInfo, MainInfoArray_Wizard);
	//const FName Name_MainInfoArray_Fighter = GET_MEMBER_NAME_CHECKED(UB2WingInfo, MainInfoArray_Fighter);
	//const FName Name_MyID = GET_MEMBER_NAME_CHECKED(FSingleWingInfoData, MyID);
	//const FName Name_EvolutionGrade = GET_MEMBER_NAME_CHECKED(FSingleWingInfoID, EvolutionGrade);
	//const FName Name_ParticleSystemAsset = GET_MEMBER_NAME_CHECKED(FCHSKCompAttachParticleSystemInfo, ParticleSystemAsset);
	//const FName Name_AttachEffectInfoArray = GET_MEMBER_NAME_CHECKED(FSingleWingInfoData, AttachEffectInfoArray);

	//const FName Name_PropOptionIconSetup = GET_MEMBER_NAME_CHECKED(UB2WingInfo, PropOptionIconSetup);
	//const FName Name_PropOption = GET_MEMBER_NAME_CHECKED(FWingPropOptionIconSetup, PropOption);

	//if (PropertyName == Name_MainInfoArray_Gladiator || PropertyName == Name_MainInfoArray_Assassin || PropertyName == Name_MainInfoArray_Wizard || PropertyName == Name_MainInfoArray_Fighter ||
	//	PropertyName == Name_MyID || PropertyName == Name_EvolutionGrade || PropertyName == Name_ParticleSystemAsset || PropertyName == Name_AttachEffectInfoArray ||
	//	PropertyName == Name_PropOptionIconSetup || PropertyName == Name_PropOption)
	//{
	//	CheckInfoDataIntegrity();
	//}
}
#endif
