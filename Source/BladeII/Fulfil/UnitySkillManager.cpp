#include "UnitySkillManager.h"

#include "Event.h"
#include "InternalEvents.h"
#include "B2LobbyInventory.h"
#include "B2UIDocHelper.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UnitySkillManager::Init()
{
	SubscribeEvents();
}

void UnitySkillManager::SubscribeEvents()
{
	UnsubscribeEvents();

	Issues.Add(DeliveryMasterDataClass<FB2MasterDatas>::GetInstance().Subscribe2(
		[this](const FB2MasterDatas& data)
	{
		this->ReceiveUnitySkillMissionMasterData(data);
	}
	));

	Issues.Add(DeliveryGetUnitySkillMissionClass<FB2ResponseGetUnitySkillMissionPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseGetUnitySkillMissionPtr& data)
	{
		if (data->missions.Num() > 0)
			BladeIIGameImpl::GetLocalCharacterData().SetUnitySkillMissionInfo(SvrToCliPCClassType(data->missions[0]->main_character_type), data);
	}
	));
}

void UnitySkillManager::UnsubscribeEvents()
{
	Issues.Empty();
}

void UnitySkillManager::ReceiveUnitySkillMissionMasterData(const FB2MasterDatas& MasterData)
{
	//// MissionData
	//const TArray<b2network::B2mdUnitySkillMissionPtr>& UnitySkills = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_unity_skill_missions_index, MasterData);

	//MD_UnitySkillMission.Empty();

	//int32 KeyIndex = 0;

	//for (auto& UnitySkill : UnitySkills)
	//{
	//	MD_UnitySkillMission.Add(KeyIndex, UnitySkill);
	//	KeyIndex++;
	//}

	//// OptionData
	//const TArray<FB2UnitySkillOptionPtr>& SkillOption = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_unity_skill_options_index, MasterData);

	//MD_UnitySkillOption.Empty();

	//for (auto& UnitySkill : SkillOption)
	//{
	//	FUnitySkillOption NewElem;
	//	NewElem.MainClass = SvrToCliPCClassType(UnitySkill->main_character_type);
	//	NewElem.SubClass = SvrToCliPCClassType(UnitySkill->unity_character_type);
	//	NewElem.Step = UnitySkill->step;
	//	NewElem.OptionId = UnitySkill->option_id;
	//	NewElem.OptionValue = UnitySkill->option_value;
	//	NewElem.Power = UnitySkill->power;

	//	MD_UnitySkillOption.Add(NewElem.GetMyKey(), NewElem);
	//}

	//// UnityAwakeData

	//const TArray<FB2UnitySkillAwakenMissionPtr>& UnityAwakens = GET_TUPLE_DATA(FB2ResponseMasterDatas::md_unity_skill_awaken_missions_index, MasterData);

	//MD_UnityAwakenMission.Empty();

	//for (auto& Skillitem : UnityAwakens)
	//{
	//	int32 KeyId = (Skillitem->character_type * 10) + Skillitem->mission_id;
	//	MD_UnityAwakenMission.Add(KeyId, Skillitem);
	//}
}

const TArray<b2network::B2mdUnitySkillMissionPtr> UnitySkillManager::GetUnitySkillMissionInfo(EPCClass MainChar, EPCClass UnityChar)
{
	TArray<b2network::B2mdUnitySkillMissionPtr> SelectedUnityMission;

	for (auto Mission : MD_UnitySkillMission)
	{
		if (Mission.Value->main_character_type == CliToSvrPCClassType(MainChar) && Mission.Value->unity_character_type == CliToSvrPCClassType(UnityChar))
			SelectedUnityMission.Add(Mission.Value);
	}

	return SelectedUnityMission;
}

bool UnitySkillManager::CheckStateUnitySkillMission(EPCClass PCClass, EPCClass SubPCClass)
{
	const TArray<b2network::B2mdUnitySkillMissionPtr> MissionInfos = GetUnitySkillMissionInfo(PCClass, SubPCClass);
	FUnitySkillMissionArray* CurrentMissions = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillMission(PCClass);
	int32 nStep = 0;
	for (auto MissionItem : *CurrentMissions)
	{
		if (MissionItem.SubClass == SubPCClass)
		{
			if (MissionItem.nState != 1) return false; // 肯丰 惑怕俊辑绰 公炼扒 舅覆 付农 波霖促.

			nStep = MissionItem.nStep;
			break;
		}
	}

	//固记捞 绝栏搁 舅覆付农 磊眉甫 剁匡 鞘夸啊 绝澜
	if (MissionInfos.Num() == 0)
		return false;

	for (auto Info : MissionInfos)
	{
		if (nStep != Info->step)
			continue;

		switch (Info->event_type)
		{
		case b2network::B2UnitySkillEventType::EQUIP_ITEM:
			if (Info->condition2 != 0 && Info->condition3 != 0)
			{
				if (!CheckGradeEquipPresetItem(SubPCClass, static_cast<EItemEquipType>(Info->condition3), Info->condition2))
					return false;
			}
			break;

		case b2network::B2UnitySkillEventType::EQUIP_ITEM_All:
			if (!CheckGradeEquipPresetItem(SubPCClass, EItemEquipType::EIET_All, Info->condition2))
				return false;
			break;

		case b2network::B2UnitySkillEventType::REACH_LEVEL:
			if (!CheckStateCurrentLevel(SubPCClass, Info->goal_count))
				return false;
			break;
		}
	}

	return true;
}

bool UnitySkillManager::CheckStateUnitySkillMissionByIndex(EPCClass pcClass, EPCClass subPCClass, int32 step, int32 index)
{
	const TArray<b2network::B2mdUnitySkillMissionPtr> MissionInfos = GetUnitySkillMissionInfo(pcClass, subPCClass);

	for (auto Info : MissionInfos)
	{
		if (step != Info->step)
			continue;

		if (index != Info->index)
			continue;

		switch (Info->event_type)
		{
		case b2network::B2UnitySkillEventType::EQUIP_ITEM:
			if (Info->condition2 != 0 && Info->condition3 != 0)
			{
				return CheckGradeEquipPresetItem(subPCClass, static_cast<EItemEquipType>(Info->condition3), Info->condition2);
			}
			break;

		case b2network::B2UnitySkillEventType::EQUIP_ITEM_All:
			return CheckGradeEquipPresetItem(subPCClass, EItemEquipType::EIET_All, Info->condition2);
			break;

		case b2network::B2UnitySkillEventType::REACH_LEVEL:
			return CheckStateCurrentLevel(subPCClass, Info->goal_count); // 饭骇篮 Goal_count 荤侩
		}
	}

	return false;
}

bool UnitySkillManager::CheckStateCurrentLevel(EPCClass PCClass, int32 Level)
{
	//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
	//auto* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(PCClass));

	//if (DocHero)
	//{
	//	return (DocHero->GetCurrentLevel() >= Level);
	//}

	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	const int32 CurrentLevel = CharacterDataStore.GetCharacterLevel(PCClass);
	return CurrentLevel >= Level;
}

bool UnitySkillManager::CheckGradeEquipPresetItem(EPCClass PCClass, EItemEquipType EquipParts, int32 Grade)
{
	TArray<bool> IsCorrecteds;
	int32 StartNum = 0;
	int32 EndNum = 0;
	FindEquipTypeNumber(StartNum, EndNum, EquipParts);

	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	FPCItemContainer &ItemContainer = CliDataStore.GetLocalCharacterData().GetUserAllItems();
	const FPerCharacterItemContainer& ThisCharContainer = ItemContainer.CharacterItemContainers[(int32)PCClass];

	for (int32 i = StartNum; i < EndNum + 1; i++)
	{
		IsCorrecteds.Add(false);
	}

	for (auto preset : ThisCharContainer.AllItemPresets)
	{
		int32 nCount = 0;
		for (int32 i = StartNum; i < EndNum + 1; i++)
		{
			int64 EquipedItemUID = preset.Value.EquippedItemID[i];
			FB2Item* Item = ItemContainer.FindItem(EquipedItemUID);
			if (Item)
			{
				if (Item->StarGrade >= Grade)
					IsCorrecteds[nCount] = true;
			}
			nCount++;
		}
	}

	int32 nCorrectNum = 0;
	for (auto isCorrect : IsCorrecteds)
	{
		if (isCorrect) nCorrectNum++;
	}

	return (IsCorrecteds.Num() == 0 ? false : (nCorrectNum == IsCorrecteds.Num()));
}

void UnitySkillManager::FindEquipTypeNumber(int32 &StartNum, int32 &EndNum, EItemEquipType EquipParts)
{
	if (EquipParts == EItemEquipType::EIET_Weapon)
	{
		StartNum = (int32)EItemEquipPlace::EIEP_Weapon;
		EndNum = (int32)EItemEquipPlace::EIEP_Weapon;
	}
	else if (EquipParts == EItemEquipType::EIET_Protections)
	{
		StartNum = (int32)EItemEquipPlace::EIEP_Helmet;
		EndNum = (int32)EItemEquipPlace::EIEP_Shoes;
	}
	else if (EquipParts == EItemEquipType::EIET_Accessories)
	{
		StartNum = (int32)EItemEquipPlace::EIEP_Belt;
		EndNum = (int32)EItemEquipPlace::EIEP_Ring;
	}
	else /// all
	{
		StartNum = (int32)EItemEquipPlace::EIEP_Weapon;
		EndNum = (int32)EItemEquipPlace::EIEP_Ring;
	}
}

FText UnitySkillManager::GetTextSkillMission(EPCClass mainPCClass, EPCClass subPCClass, int32 step, int32 index)
{
	const TArray<b2network::B2mdUnitySkillMissionPtr> MissionInfos = GetUnitySkillMissionInfo(mainPCClass, subPCClass);

	for (auto Info : MissionInfos)
	{
		if (step != Info->step)
			continue;

		if (index != Info->index)
			continue;

		switch (Info->event_type)
		{
		case b2network::B2UnitySkillEventType::EQUIP_ITEM:
		{
			if (Info->condition2 != 0 && Info->condition3 != 0)
			{
				if (static_cast<EItemEquipType>(Info->condition3) == EItemEquipType::EIET_Weapon)
				{
					return FText::Format(BladeIIGetLOCText(B2LOC_CAT_UNITYSKILLMISSION, TEXT("Mission_Equip_Weapon")), FText::FromString(FString::FromInt(Info->condition2)));
				}
				else if (static_cast<EItemEquipType>(Info->condition3) == EItemEquipType::EIET_Protections)
				{
					return FText::Format(BladeIIGetLOCText(B2LOC_CAT_UNITYSKILLMISSION, TEXT("Mission_Equip_Protections")), FText::FromString(FString::FromInt(Info->condition2)));
				}
				else if (static_cast<EItemEquipType>(Info->condition3) == EItemEquipType::EIET_Accessories)
				{
					return FText::Format(BladeIIGetLOCText(B2LOC_CAT_UNITYSKILLMISSION, TEXT("Mission_Equip_Accessories")), FText::FromString(FString::FromInt(Info->condition2)));
				}
			}
		}
		break;

		case b2network::B2UnitySkillEventType::EQUIP_ITEM_All:
			return FText::Format(BladeIIGetLOCText(B2LOC_CAT_UNITYSKILLMISSION, TEXT("Mission_AllEquip")), FText::FromString(FString::FromInt(Info->condition2)));
			break;

		case b2network::B2UnitySkillEventType::REACH_LEVEL:
			return FText::Format(BladeIIGetLOCText(B2LOC_CAT_UNITYSKILLMISSION, TEXT("Mission_ReachLevel")), FText::FromString(FString::FromInt(Info->goal_count)));
			break;

		default:
			break;
		}
	}

	return FText::GetEmpty();
}

const TArray<FUnitySkillOption> UnitySkillManager::GetUnitySkillOptionInfoList(EPCClass MainChar)
{
	FUnitySkillMissionArray* CurrentMissionArray = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillMission(MainChar);
	FUnitySkillOption NewOption = FUnitySkillOption();
	TArray<FUnitySkillOption> OutList;
	OutList.Empty();

	for (auto MissionItem : *CurrentMissionArray)
	{
		if (MissionItem.MainClass == MainChar)
		{
			NewOption.MainClass = MainChar;
			NewOption.SubClass = MissionItem.SubClass;

			if (MissionItem.nState != b2network::B2MissionStateType::PROGRESS)
			{
				NewOption.Step = MissionItem.nStep;

				if (MD_UnitySkillOption.Contains(NewOption.GetMyKey()))
					OutList.Add(*MD_UnitySkillOption.Find(NewOption.GetMyKey()));
			}
			else
			{
				NewOption.Step = MissionItem.nStep - 1;

				if (MD_UnitySkillOption.Contains(NewOption.GetMyKey()))
					OutList.Add(*MD_UnitySkillOption.Find(NewOption.GetMyKey()));
			}
		}
	}

	OutList.Sort([](const FUnitySkillOption& A, const FUnitySkillOption& B) {
		return static_cast<int32>(A.SubClass) < static_cast<int32>(B.SubClass);
	});

	return OutList;
}

const FUnitySkillOption UnitySkillManager::GetUnitySkillOptionInfo(EPCClass MainChar, EPCClass SubChar, bool bIsNextStep)
{
	FUnitySkillMissionArray* CurrentMissionArray = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillMission(MainChar);
	FUnitySkillOption NewOption = FUnitySkillOption();

	for (auto MissionItem : *CurrentMissionArray)
	{
		if (MissionItem.MainClass == MainChar && MissionItem.SubClass == SubChar)
		{
			NewOption.MainClass = MainChar;
			NewOption.SubClass = SubChar;

			if (bIsNextStep) // 促澜 胶跑 沥焊啊 鞘夸且锭.
			{
				NewOption.Step = MissionItem.nStep;
			}
			else // 泅犁 胶跑狼 胶懦 可记 沥焊啊 鞘夸且锭.
			{
				if (MissionItem.nState != b2network::B2MissionStateType::PROGRESS) // 窜拌 傈何 肯丰惑怕
				{
					NewOption.Step = MissionItem.nStep;
				}
				else
				{
					NewOption.Step = MissionItem.nStep - 1;
				}
			}

			if (MD_UnitySkillOption.Contains(NewOption.GetMyKey()))
				NewOption = *MD_UnitySkillOption.Find(NewOption.GetMyKey());
			break;
		}
	}

	return NewOption;
}

double UnitySkillManager::GetUnitySkillPower(const FUnitySkillMission& UnityInfo)
{
	double SkillPower = 0;
	int32 UnityStep = 0;

	if (UnityInfo.nState != b2network::B2MissionStateType::PROGRESS) // 窜拌 傈何 肯丰惑怕
	{
		UnityStep = UnityInfo.nStep;
	}
	else
	{
		UnityStep = UnityInfo.nStep - 1;
	}

	int32 SkillKey = FUnitySkillOption::GetKey(UnityInfo.MainClass, UnityInfo.SubClass, UnityStep);

	if (MD_UnitySkillOption.Contains(SkillKey))
	{
		FUnitySkillOption UnitySkillOption = *MD_UnitySkillOption.Find(SkillKey);
		SkillPower = UnitySkillOption.Power;
	}

	return SkillPower;
}

const TArray<FB2UnitySkillAwakenMissionPtr> UnitySkillManager::GetUnitySkillAwakenMissions(EPCClass MainChar)
{
	TArray<FB2UnitySkillAwakenMissionPtr> SelectedUnityMission;

	for (auto Mission : MD_UnityAwakenMission)
	{
		if (Mission.Value->character_type == CliToSvrPCClassType(MainChar))
			SelectedUnityMission.Add(Mission.Value);
	}

	return SelectedUnityMission;
}


const FB2UnitySkillAwakenMissionPtr UnitySkillManager::GetUnitySkillAwakenMission(EPCClass MainChar, int32 Step)
{
	TArray<FB2UnitySkillAwakenMissionPtr> SelectedUnityMission;

	int32 KeyID = CliToSvrPCClassType(MainChar) * 10 + Step;

	if (MD_UnityAwakenMission.Contains(KeyID))
		return *MD_UnityAwakenMission.Find(KeyID);

	return nullptr;

}