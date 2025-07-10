// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
//#include "BladeII.h"
#include "B2MessageInfoCommonStruct.h"
#include "EventSubsystem.h"
#include "B2ClientDataStore.h"

struct FUnitySkillOption
{
	FUnitySkillOption(EPCClass eMainClass, EPCClass eSubClass, int32 nStep, int32 nOptionId, double nOptionValue, double nPower)
		: MainClass(eMainClass)
		, SubClass(eSubClass)
		, Step(nStep)
		, OptionId(nOptionId)
		, OptionValue(nOptionValue)
		, Power(nPower)
	{
	}
	explicit FUnitySkillOption()
		: MainClass(EPCClass::EPC_End)
		, SubClass(EPCClass::EPC_End)
		, Step(0)
		, OptionId(0)
		, OptionValue(0.0)
		, Power(0.0)
	{
	}

	EPCClass MainClass;
	EPCClass SubClass;
	int64 Step;
	int32 OptionId;
	double OptionValue;
	double Power;

	FORCEINLINE static int32 GetKey(EPCClass eMainClass, EPCClass eSubClass, int32 nStep) 
	{ 
		return ((static_cast<int32>(eMainClass) * 100) + (static_cast<int32>(eSubClass) * 10) + nStep); 
	}

	FORCEINLINE int32 GetMyKey() { return GetKey(MainClass, SubClass, Step); }
};

enum class EItemEquipType : uint8
{
	// Each has equipping slot in inventory.
	EIET_All = 1,
	EIET_Weapon = 2, // b2network::B2InventoryType::Weapon  = 2;
	EIET_Protections = 3,// b2network::B2InventoryType::Protection  = 3;
	EIET_Accessories = 4// b2network::B2InventoryType::Accessory  = 4;
};

class UnitySkillManager
{
public:
	explicit UnitySkillManager()
	{
		MD_UnitySkillMission.Empty();
	}

	void Init();

	void SubscribeEvents();
	void UnsubscribeEvents();

	void ReceiveUnitySkillMissionMasterData(const FB2MasterDatas& MasterData);

	bool CheckStateUnitySkillMission(EPCClass PCClass, EPCClass SubPCClass);
	bool CheckStateUnitySkillMissionByIndex(EPCClass PCClass, EPCClass SubPCClass, int32 Step, int32 index);

	bool CheckStateCurrentLevel(EPCClass PCClass, int32 Level);
	bool CheckGradeEquipPresetItem(EPCClass PCClass, EItemEquipType EquipParts, int32 Grade);
	void FindEquipTypeNumber(int32 &StartNum, int32 &EndNum, EItemEquipType EquipParts);
	FText GetTextSkillMission(EPCClass mainPCClass, EPCClass subPCClass, int32 step, int32 index);

	TArray<b2network::B2mdUnitySkillMissionPtr> const GetUnitySkillMissionInfo(EPCClass MainChar, EPCClass UnityChar);
	TArray<FUnitySkillOption> const GetUnitySkillOptionInfoList(EPCClass MainChar);
	FUnitySkillOption const GetUnitySkillOptionInfo(EPCClass MainChar, EPCClass SubChar, bool bIsNextStep = false);
	double GetUnitySkillPower(const FUnitySkillMission& UnityInfo);

	TArray<FB2UnitySkillAwakenMissionPtr> const GetUnitySkillAwakenMissions(EPCClass MainChar);
	FB2UnitySkillAwakenMissionPtr const GetUnitySkillAwakenMission(EPCClass MainChar, int32 Step);
private:

	TMap<int32, b2network::B2mdUnitySkillMissionPtr>		MD_UnitySkillMission;
	TMap<int32, FUnitySkillOption>							MD_UnitySkillOption;
	TMap<int32, FB2UnitySkillAwakenMissionPtr>				MD_UnityAwakenMission;

	TArray<std::shared_ptr<issue>> Issues;
};
