#include "BladeII.h"
#include "CharacterDataStore.h"
#include "B2PCClassInfo.h"
#include "B2PCClassInfoBox.h"
#include "B2SkillInfo.h"

#include "B2LobbyInventory.h"
#include "Event.h"
#include "B2CombatStatEvaluator.h"
//#include "FakeConfigure.h"
//#include "Algo/Copy.h"
//#include "Algo/Count.h"
#include "B2MessageInfoCommonStruct.h"
#include "FairyManager.h"

#include "Global.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2CodeTable.h"
#include "B2UIDocHelper.h"

void FB2PlayerSharedStats::CheckNeedRegistSkill()
{
	// TODO skill quick slot
	CheckNeedRegistSkill(GetSkillSlotID());
}

void FB2PlayerSharedStats::UpdateBeforeExpLevelInfo()
{
	BeforeExpLevelInfo.Level = BasicInfo->level;
	BeforeExpLevelInfo.Exp = BasicInfo->exp;
	BeforeExpLevelInfo.Exp_Max = BasicInfo->exp_max;
}

void FB2PlayerSharedStats::CheckNeedRegistSkill(EItemPresetSlotType InSkillSlotID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FB2PlayerSharedStats_CheckNeedRegistSkill);
	const int32 SlotID = static_cast<int32>(InSkillSlotID);
	if (BasicInfo && BasicInfo->skill_quick_slots.IsValidIndex(SlotID) && BasicInfo->skill_quick_slots[SlotID])
	{
		TArray<int32> UnregistSkillIds;

		//BladeIIGameImpl::GetLocalCharacterData().GetUnregistredTargetSlotSkills(SvrToCliPCClassType(BasicInfo->character_type), InSkillSlotID, UnregistSkillIds);

		int32 index = 0;
		if (BasicInfo->skill_quick_slots[SlotID]->using_skill_id_1 == SKILL_INVALID_ID && UnregistSkillIds.IsValidIndex(index))
			BasicInfo->skill_quick_slots[SlotID]->using_skill_id_1 = UnregistSkillIds[index++];
		if (BasicInfo->skill_quick_slots[SlotID]->using_skill_id_2 == SKILL_INVALID_ID && UnregistSkillIds.IsValidIndex(index))
			BasicInfo->skill_quick_slots[SlotID]->using_skill_id_2 = UnregistSkillIds[index++];
		if (BasicInfo->skill_quick_slots[SlotID]->using_skill_id_3 == SKILL_INVALID_ID && UnregistSkillIds.IsValidIndex(index))
			BasicInfo->skill_quick_slots[SlotID]->using_skill_id_3 = UnregistSkillIds[index++];

		if (index != 0)
		{
			//BladeIIGameImpl::GetClientDataStore().RequestSetUsingSkill(
			//	SvrToCliPCClassType(BasicInfo->character_type),
			//	SlotID,
			//	BasicInfo->skill_quick_slots[SlotID]->using_skill_id_1,
			//	BasicInfo->skill_quick_slots[SlotID]->using_skill_id_2,
			//	BasicInfo->skill_quick_slots[SlotID]->using_skill_id_3
			//);
		}
	}
}

FText FPCClassOrNickName::GetClassOrNickName() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FB2PlayerSharedStats_GetClassOrNickName);
	return NickName.IsEmpty() ? ClassName : NickName;
}

FLocalCharacterData::FLocalCharacterData()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_FLocalCharacterData);
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RelicInfos.Add(IntToPCClass(PCI));
		UnitySkillMissionInfos.Add(IntToPCClass(PCI));
		UnitySkillAwakenMissionInfos.Add(IntToPCClass(PCI));
		UnitySkillState.Add(IntToPCClass(PCI));
	}

	MainPlayerClass = EPCClass::EPC_End;
	SubPlayerClass = EPCClass::EPC_End; 
}

int32 FLocalCharacterData::GetCharacterLevel(EPCClass InClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterLevel);
	const int32 PCI = PCClassToInt(InClass);
	if (PlayerSharedStats.IsValidIndex(PCI) == false)
		return 0;

	return PlayerSharedStats[PCI].BasicInfo ? PlayerSharedStats[PCI].BasicInfo->level : 1;
}

FString	FLocalCharacterData::GetUserNickName() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetUserNickName);
	//return BladeIIGameImpl::GetClientDataStore().GetUserNickName();
	return FString("");
}

int32 FLocalCharacterData::GetUserLevel() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetUserLevel);
	//기획확립시까지 임시
	//제일 레벨이 높은 캐릭터 레벨 반환
	int32 UserLevel = 0;
	for (auto Character : PlayerSharedStats)
	{
		if (Character.BasicInfo && UserLevel < Character.BasicInfo->level)
			UserLevel = Character.BasicInfo->level;
	}

	return UserLevel;
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
bool gCheatUseDummySkills = false;
#endif
void FLocalCharacterData::GetCharacterRegistredSkills(EPCClass InCharacterClass, TArray<int32>& RegistredSkills, bool bIncludeUnitySkill) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterRegistredSkills);
	RegistredSkills.Empty();
	
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	bool bDevModeDummySkills = false || gCheatUseDummySkills;
#endif

	if (PlayerSharedStats.IsValidIndex(PCClassToInt(InCharacterClass)))
	{
		auto CharInfo = PlayerSharedStats[PCClassToInt(InCharacterClass)].BasicInfo;
		// TODO skill quick slot

		EItemPresetSlotType SlotType = GetPCClassSkillSlotID(InCharacterClass);
		const int32 SkillSlotID = static_cast<int32>(SlotType);

		if (CharInfo && CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID))
		{
			auto SkillInfo = CharInfo->skill_quick_slots[SkillSlotID];
			if (SkillInfo)
			{
				RegistredSkills.Add(SkillInfo->using_skill_id_1);

				// 중복된 ID 가 들어와서 좀 체크.. 데이터가 잘못된 거긴 한데.
				RegistredSkills.Add((SkillInfo->using_skill_id_1 != SkillInfo->using_skill_id_2) ? SkillInfo->using_skill_id_2 : SKILL_INVALID_ID);

				RegistredSkills.Add(
					(SkillInfo->using_skill_id_1 != SkillInfo->using_skill_id_3 && SkillInfo->using_skill_id_2 != SkillInfo->using_skill_id_3) ?
					SkillInfo->using_skill_id_3 : SKILL_INVALID_ID
				);
			}
			else
			{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
				bDevModeDummySkills = true;
#endif
			}
		}
		else
		{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
			bDevModeDummySkills = true;
#endif
		}
	}

	if (bIncludeUnitySkill)
		RegistredSkills.Add(GetUnitySkillId(InCharacterClass));

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (bDevModeDummySkills)
	{ // 암것도 없는 standalone 상황 혹은 테스트 용으로 적당히 공격용 스킬 끼워넣기.
		// 테스트 용으로는 목적이 좀 있어서 Unity 스킬은 없애 보고..
		if (gCheatUseDummySkills)
		{
			RegistredSkills.Empty();
		}
		RegistredSkills.Add((PCClassToInt(InCharacterClass) + 1) * 10000 + 0);
		RegistredSkills.Add((PCClassToInt(InCharacterClass) + 1) * 10000 + 2);
		RegistredSkills.Add((PCClassToInt(InCharacterClass) + 1) * 10000 + 4);
	}
#endif //BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
}

const FB2PlayerSharedStats* FLocalCharacterData::GetPlayerStat(EPCClass InPCClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetPlayerStat);
	if (InPCClass == EPCClass::EPC_End)
		return nullptr;

	int32 PCIndex = PCClassToInt(InPCClass);
	if (PlayerSharedStats.IsValidIndex(PCIndex) == false)
		return nullptr;

	return &(PlayerSharedStats[PCIndex]);
}

FB2OriginCharacterInfoPtr	FLocalCharacterData::GetPlayerStatBasicInfo(EPCClass InPCClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetPlayerStatBasicInfo);
	auto* PlayerStat = GetPlayerStat(InPCClass);
	if (PlayerStat == nullptr)
		return nullptr;

	return PlayerStat->BasicInfo;
}

const FPlayerExpLevelInfo* FLocalCharacterData::GetBeforePlayerExpLevelInfo(EPCClass InPCClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetBeforePlayerExpLevelInfo);
	auto* PlayerStat = GetPlayerStat(InPCClass);
	if (PlayerStat == nullptr)
		return nullptr;

	return &(PlayerStat->BeforeExpLevelInfo);
}

b2network::B2SkillQuickSlotPtr	FLocalCharacterData::GetPlayerStatBasicSkillInfo(EPCClass InPCClass, int32 SkillPresetIndex /* = 0*/) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetPlayerStatBasicSkillInfo);
	auto PlayerStatBasicInfo = GetPlayerStatBasicInfo(InPCClass);
	if (PlayerStatBasicInfo == nullptr)
		return nullptr;

	if (PlayerStatBasicInfo->skill_quick_slots.IsValidIndex(SkillPresetIndex) == false)
		return nullptr;

	return PlayerStatBasicInfo->skill_quick_slots[SkillPresetIndex];
}


int32 FLocalCharacterData::GetUnitySkillId(EPCClass InCharacterClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetUnitySkillId);
	int32 SkillID = SKILL_INVALID_ID;

	FB2OriginCharacterInfoPtr PlayerStatBasicInfo = GetPlayerStatBasicInfo(InCharacterClass);
	if (PlayerStatBasicInfo == nullptr)
	{
		return SkillID;
	}

	if (PlayerStatBasicInfo->aether_offense_set_option_id == b2network::B2AetherSetOptionType::SKILL_GAGE)
	{
		SkillID = (PCClassToInt(InCharacterClass) + 1) * 100 + 2;
	}
	else if (PlayerStatBasicInfo->unity_skill_awaken)
	{
		SkillID = (PCClassToInt(InCharacterClass) + 1) * 100 + 3;
	}
	else
	{
		SkillID = (PCClassToInt(InCharacterClass) + 1) * 100 + 1;
	}

#if !UE_BUILD_SHIPPING
	//if (NetFakeConfigure::GetInstance().GetForcedUnitySkillOpen())
	//{
	//	return SkillID;
	//}
#endif
	
	return SkillID;
}

void FLocalCharacterData::GetUnitySkillMissionArray(FUnitySkillMissionArray& UnitySkillMissionArray, EPCClass InCharacterClass)
{
	UnitySkillMissionArray = UnitySkillMissionInfos.FindRef(InCharacterClass);
}

TArray<b2network::B2GuildSkillPtr>& FLocalCharacterData::GetGuildSkillInfos()
{
	GuildSkillInfos.Empty();
	//BladeIIGameImpl::GetClientDataStore().GetGuildCurrentSkillInfoArr().GenerateValueArray(GuildSkillInfos);

	return GuildSkillInfos;
}

const TArray<int32>& FLocalCharacterData::GetPreviewCostumeItems() const
{
	//return BladeIIGameImpl::GetCostumeDataStore().GetPreviewCostumes();
	static TArray<int32> aTemp;
	return aTemp;
}


void FLocalCharacterData::SetEtherSetID(EPCClass InCharacterClass, int32 OffenseID, int32 DefenseID)
{
	if (InCharacterClass == EPCClass::EPC_End)
		return;

	if (PlayerSharedStats.IsValidIndex(PCClassToInt(InCharacterClass)) == false)
		return;

	PlayerSharedStats[PCClassToInt(InCharacterClass)].BasicInfo->aether_defense_set_option_id = DefenseID;
	PlayerSharedStats[PCClassToInt(InCharacterClass)].BasicInfo->aether_offense_set_option_id = OffenseID;
}

void FLocalCharacterData::SetEtherDefenseSetID(EPCClass InCharacterClass, int32 DefenseID)
{
	if (InCharacterClass == EPCClass::EPC_End)
		return;

	if (PlayerSharedStats.IsValidIndex(PCClassToInt(InCharacterClass)) == false)
		return;

	PlayerSharedStats[PCClassToInt(InCharacterClass)].BasicInfo->aether_defense_set_option_id = DefenseID;
}

bool FLocalCharacterData::IsSkillRegistred(EPCClass InCharacterClass, int32 SkillId) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_IsSkillRegistred);
	EItemPresetSlotType SlotType = GetPCClassSkillSlotID(InCharacterClass);
	int32 SlotID = static_cast<int32>(SlotType);
	auto SkillInfo = GetPlayerStatBasicSkillInfo(InCharacterClass, SlotID);
	if (SkillInfo == nullptr)
		return false;
	
	return SkillInfo->using_skill_id_1 == SkillId || 
		SkillInfo->using_skill_id_2 == SkillId || 
		SkillInfo->using_skill_id_3 == SkillId || 
		GetUnitySkillId(InCharacterClass) == SkillId;
	
}

void FLocalCharacterData::GetCharacterUnregistredSkills(EPCClass InCharacterClass, TArray<int32>& UnregistredSkills) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterUnregistredSkills);
	UnregistredSkills.Empty();

	if (auto CharInfo = GetPlayerStatBasicInfo(InCharacterClass))
	{
		EItemPresetSlotType SlotType = GetPCClassSkillSlotID(InCharacterClass);
		const int32 SkillSlotID = static_cast<int32>(SlotType);

		if (CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID))
		{
			auto SkillInfo = CharInfo->skill_quick_slots[SkillSlotID];

			//for (auto skill : CharInfo->skills)
			//{
			//	auto* SkillMsterInfo = BladeIIGameImpl::GetClientDataStore().GetSkillMasterData(skill->skill_id);
			//	if (SkillMsterInfo && SkillMsterInfo->bActiveSkill && CharInfo->level >= SkillMsterInfo->LearnLevel)
			//	{
			//		if (skill->skill_id != SkillInfo->using_skill_id_1 && skill->skill_id != SkillInfo->using_skill_id_2 && skill->skill_id != SkillInfo->using_skill_id_3)
			//			UnregistredSkills.Add(skill->skill_id);
			//	}
			//}
		}		
	}
}

void FLocalCharacterData::GetCharacterSkillToLearnAtLevel(TArray<int32>& OutSkillIDs, EPCClass InCharClass, int32 InCharLevel) const
{ // MasterData 상에서 InCharLevel 에서 배우도록 되어 있는 스킬 ID 목록 리턴.
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterSkillToLearnAtLevel);
	if (auto CharInfo = GetPlayerStatBasicInfo(InCharClass))
	{
		//for (auto skill : CharInfo->skills)
		//{
		//	auto* SkillMsterInfo = BladeIIGameImpl::GetClientDataStore().GetSkillMasterData(skill->skill_id);
		//	if (SkillMsterInfo && SkillMsterInfo->bActiveSkill && InCharLevel == SkillMsterInfo->LearnLevel)
		//	{
		//		OutSkillIDs.Add(skill->skill_id);
		//	}
		//}
	}
}

void FLocalCharacterData::GetCharacterSkills(EPCClass InCharacterClass, TArray<FB2SkillInfo>& Skills) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterSkills);
	Skills.Empty();

	// 현 구현 상 사용 가능한 모든 스킬임. ActiveSkill 의 경우 장착 여부를 따져야 한다면 추가해야.
	if (auto CharInfo = GetPlayerStatBasicInfo(InCharacterClass))
	{
		for (auto Skill : CharInfo->skills)
			Skills.Add(*Skill);
	}
}

void FLocalCharacterData::GetCharacterSkills(EPCClass InCharacterClass, TArray<int32>& OutSkillIDs) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterSkills_Out);
	OutSkillIDs.Empty();
	
	// 현 구현 상 사용 가능한 모든 스킬임. ActiveSkill 의 경우 장착 여부를 따져야 한다면 추가해야.
	if (auto CharInfo = GetPlayerStatBasicInfo(InCharacterClass))
	{
		for (auto Skill : CharInfo->skills)
			OutSkillIDs.Add(Skill->skill_id); // ID 만
	}
}

void FLocalCharacterData::GetCharacterPassiveSkills(EPCClass InCharacterClass, TArray<int32>& SkillIds) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterPassiveSkills);
	SkillIds.Empty();

	if (auto CharInfo = GetPlayerStatBasicInfo(InCharacterClass))
	{
		//for (auto Skill : CharInfo->skills)
		//{
		//	auto* SkillMD = BladeIIGameImpl::GetClientDataStore().GetSkillMasterData(Skill->skill_id);
		//	if (SkillMD && !SkillMD->bActiveSkill)
		//	{
		//		SkillIds.Add(Skill->skill_id);
		//	}
		//}
	}
}

int32 FLocalCharacterData::GetCharacterSkillLevel(int32 SkillId) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterSkillLevel);
	for (auto CharInfo : PlayerSharedStats)
	{
		//if (CharInfo.BasicInfo)
		//{
		//	for (auto Skill : CharInfo.BasicInfo->skills)
		//	{
		//		if (Skill->skill_id == SkillId)
		//		{
		//			if (CharInfo.BasicInfo->level < BladeIIGameImpl::GetClientDataStore().GetSkillLearnLevel(SkillId))
		//				return UNDEFINED_SKILL_LEVEL;

		//			return Skill->skill_level;
		//		}
		//	}
		//}
	}

	return UNDEFINED_SKILL_LEVEL;
}

void FLocalCharacterData::GetCharacterSkillOptionsOfID(int32 InSkillId, TArray<FSkillOptionData>& OutFoundOptionData) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterSkillOptionsOfID);
	// 특정 스킬 ID 와 현재 해당 스킬 레벨에서의 스킬옵션들
	//const FSkillMasterData* SkillMDOfID = BladeIIGameImpl::GetClientDataStore().GetSkillMasterData(InSkillId);
	//if (SkillMDOfID)
	//{
	//	const int32 SKillLvOfID = GetCharacterSkillLevel(InSkillId);
	//	SkillMDOfID->LevelOptionData.MultiFind(SKillLvOfID, OutFoundOptionData);
	//}
}

void FLocalCharacterData::GetEquippedItems(EPCClass PCClass, TArray<FB2Item>& EquippedItems) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetEquippedItems);
	EquippedItems.Empty();
	for (auto& Item : UserAllItems.CharacterItemContainers[PCClassToInt(PCClass)].AllEuippedInfo)
	{
		if (Item.bIsEquipped)
			EquippedItems.Add(Item.EquippedItem);
	}
}

void FLocalCharacterData::GetEquippedItems(EPCClass PCClass, TMap<EItemEquipPlace, FB2Item>& EquippedItems) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetEquippedItems_TMap);
	EquippedItems.Empty();
	for (auto& Item : UserAllItems.CharacterItemContainers[PCClassToInt(PCClass)].AllEuippedInfo)
	{
		if (Item.bIsEquipped)
			EquippedItems.Add(Item.EquippedItem.EquipPlace, Item.EquippedItem);
	}
}

void FLocalCharacterData::GetEquippedCostumeItems(EPCClass PCClass, TArray<FB2Item>& EquippedCostumeItems) const
{
	for (auto& Elem : UserAllItems.CharacterItemContainers[PCClassToInt(PCClass)].AllEuippedCostumeInfo)
	{
		const FEquippedItemInfo& EquippedCostumeItemInfo = Elem.Value;
		if (EquippedCostumeItemInfo.bIsEquipped)
			EquippedCostumeItems.Add(EquippedCostumeItemInfo.EquippedItem);
	}
}

void FLocalCharacterData::GetEquippedEthers(EPCClass PCClass, TMap<int64, FB2Ether>& EquippedEthers) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetEquippedEthers);
	EquippedEthers.Empty();
	if (!AllEthers.EquippedEthers.Contains(PCClass)) return;

	for (TPair<int64, FB2Ether> Ether : AllEthers.EquippedEthers[PCClass])
	{
		EquippedEthers.Add(Ether.Key, Ether.Value);
	}
}

void FLocalCharacterData::GetEquippedTotems(EPCClass PCClass, TMap<int64, FB2Totem>& OutEquippedTotems) const
{
	OutEquippedTotems.Empty();
	if (!EquippedTotems.Contains(PCClass))
	{
		return;
	}

	for (TPair<int64, FB2Totem> Elem : EquippedTotems[PCClass])
	{
		OutEquippedTotems.Add(Elem.Key, Elem.Value);
	}
}

bool FLocalCharacterData::IsJustAddedSkill(int32 SkillId) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_IsJustAddedSkill);
	if (PlayedUnlockFX.Contains(SkillId))
		return !PlayedUnlockFX.FindRef(SkillId);

	return true;
}

void FLocalCharacterData::SetPlayedUnlockSkill(int32 SkillId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetPlayedUnlockSkill);
	PlayedUnlockFX.Add(SkillId, true);
}

int32 FLocalCharacterData::GetCharacterMaxLevel() const
{
	//return BladeIIGameImpl::GetClientDataStore().GetMaxCharacterLevel();
	return 0;
}

int32 FLocalCharacterData::GetCharacterExp(EPCClass InClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterExp);
	if (auto BasicInfo = GetPlayerStatBasicInfo(InClass))
		return BasicInfo->exp;

	return 0;
}

int32 FLocalCharacterData::GetCharacterExpMax(EPCClass InClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterExpMax);
	if (auto BasicInfo = GetPlayerStatBasicInfo(InClass))
		return BasicInfo->exp_max;

	return 0;
}

float FLocalCharacterData::GetCharacterExpPercent(EPCClass InClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterExpPercent);

	float ResultExpPercent = 0;

	if (FB2OriginCharacterInfoPtr WantedInfo = GetPlayerStatBasicInfo(InClass))
	{
		if (WantedInfo->exp_max > 0)
		{
			ResultExpPercent = (WantedInfo->exp * 100.0f) / WantedInfo->exp_max;
		}
	}

	return ResultExpPercent;
}

int32 FLocalCharacterData::GetBeforeCharacterLevel(EPCClass InClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetBeforeCharacterLevel);

	if (const FPlayerExpLevelInfo* pPlayerExpLevelInfo = GetBeforePlayerExpLevelInfo(InClass))
	{
		return pPlayerExpLevelInfo->Level;
	}

	return 0;
}

float FLocalCharacterData::GetBeforeCharacterExpPercent(EPCClass InClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetBeforeCharacterExpPercent);

	float ResultExpPercent = 0;

	if (const FPlayerExpLevelInfo* pPlayerExpLevelInfo = GetBeforePlayerExpLevelInfo(InClass))
	{
		if (pPlayerExpLevelInfo->Exp_Max > 0)
		{
			ResultExpPercent = (pPlayerExpLevelInfo->Exp * 100.0f) / pPlayerExpLevelInfo->Exp_Max;
		}
	}

	return ResultExpPercent;
}

void FLocalCharacterData::UpdateBeforeExpLevel(EPCClass InClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_UpdateBeforeExpLevel);

	int32 ClassIndex = PCClassToInt(InClass);
	if (PlayerSharedStats.IsValidIndex(ClassIndex))
	{
		PlayerSharedStats[ClassIndex].UpdateBeforeExpLevelInfo();
	}
}

int32 FLocalCharacterData::GetCharacterLeftSkillPoint(EPCClass InCharacterClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterLeftSkillPoint);
	
	FB2OriginCharacterInfoPtr PlayerStatBasicInfo = GetPlayerStatBasicInfo(InCharacterClass);
	if (PlayerStatBasicInfo)
		return PlayerStatBasicInfo->remain_skill_point;

	return 0; //[@AKI, 180426] B2BUG-1310에 덧글에 의해 Default는 0으로 변경
}

void FLocalCharacterData::SetCharacterLeftSkillPoint(EPCClass InCharacterClass, int32 LeftPoint)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetCharacterLeftSkillPoint);

	FB2OriginCharacterInfoPtr PlayerStatBasicInfo = GetPlayerStatBasicInfo(InCharacterClass);
	if (PlayerStatBasicInfo)
		PlayerStatBasicInfo->remain_skill_point = LeftPoint;
}

int32 FLocalCharacterData::GetCharacterUsedSkillPoint(EPCClass InCharacterClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterUsedSkillPoint);
	int32 UsedSkillPoint = 0;

	if (auto CharInfo = GetPlayerStatBasicInfo(InCharacterClass))
	{
		for (auto Skill : CharInfo->skills)
			UsedSkillPoint += Skill->skill_level;
	}

	return UsedSkillPoint;
}

int32 FLocalCharacterData::GetCharacterSkillLevelupGold(int32 SkillId)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterSkillLevelupGold);
	//if (SkillId == SKILL_INVALID_ID)
	//	return 0;

	//int32 SkillLevel = GetCharacterSkillLevel(SkillId);
	//if (SkillLevel == UNDEFINED_SKILL_LEVEL)
		return 0;

	//auto* SkillMD = BladeIIGameImpl::GetClientDataStore().GetSkillMasterData(SkillId);
	//check(SkillMD)  // 없으면 안됨..

	//// 스킬 레벨업 비용 공식 = (현재레벨 +1) * MD의 레벨업 인자값
	//int32 LevelUpCost = (SkillLevel + 1) * SkillMD->LevelUpCostFactor;

	//return LevelUpCost;
}

int32 FLocalCharacterData::GetCharacterSkillLevelupPoint(int32 SkillId, int32 SkillLevel)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterSkillLevelupPoint);
	if (SkillId == SKILL_INVALID_ID)
		return 0;

	//auto* SkillMD = BladeIIGameImpl::GetClientDataStore().GetSkillMasterData(SkillId);
	//check(SkillMD)  // 없으면 안됨..
	//
	//if (SkillMD->LevelUpPointData.Contains(SkillLevel))
	//	return SkillMD->LevelUpPointData[SkillLevel];

	return 0;
}

void FLocalCharacterData::GetAllTotems(TMap<int64, FB2Totem>& OutAllTotems)
{
	OutAllTotems.Empty();

	for (TPair<int64, FB2Totem>& Elem : AllTotems)
	{
		OutAllTotems.Add(Elem.Key, Elem.Value);
	}
}

int32 FLocalCharacterData::GetAllTotemsCount()
{
	return AllTotems.Num();
}

bool FLocalCharacterData::GetEquuipedTotemByEquipSlot(EPCClass InClass, ETotemEquipSlot InTotemEquipSlot, FB2Totem& OutTotem)
{
	if (!EquippedTotems.Contains(InClass))
	{
		return false;
	}

	for (TPair<int64, FB2Totem> Elem : EquippedTotems[InClass])
	{
		const FB2Totem& EquippedTotemInfo = Elem.Value;
		if (EquippedTotemInfo.EquipSlot == InTotemEquipSlot)
		{
			OutTotem = EquippedTotemInfo;
			return true;
		}
	}

	return false;
}

bool FLocalCharacterData::IsFullTotemInventory()
{
	return (MAX_TOTEM_INVENTORY_SLOT <= AllTotems.Num());
}

bool FLocalCharacterData::IsOverTotemInventory()
{
	return (MAX_TOTEM_INVENTORY_SLOT < AllTotems.Num());
}

void FLocalCharacterData::SetAllTotemsNewFlagOff()
{
	for (TPair<int64, FB2Totem>& Elem : AllTotems)
	{
		Elem.Value.IsNew = false;
	}
}

int32 FLocalCharacterData::GetCharBrevetRank(EPCClass InCharacterClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharBrevetRank);
	if (auto CharInfo = GetPlayerStatBasicInfo(InCharacterClass))
		return CharInfo->rank;

	return 0;

}

EPCClass FLocalCharacterData::GetMainPlayerClass() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetMainPlayerClass);
	return MainPlayerClass;
}

EPCClass FLocalCharacterData::GetSubPlayerClass() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetSubPlayerClass);
	return SubPlayerClass;
}

void FLocalCharacterData::SetMainPlayerClass(EPCClass InMainClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetMainPlayerClass);
	MainPlayerClass = InMainClass;
}

void FLocalCharacterData::SetSubPlayerClass(EPCClass InSubClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetSubPlayerClass);
	SubPlayerClass = InSubClass;
}

void FLocalCharacterData::GetSetItemValue(EPCClass InPCClass, TMap<int32, int32>& OutSetItemDatas)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetSetItemValue);
	const int8 PCI = PCClassToInt(InPCClass);
	if (UserAllItems.CharacterItemContainers.IsValidIndex(PCI) && AllWingData.IsValidIndex(PCI))
	{
		TArray<FB2Item> GatheredLocalEquippedItems;
		for (auto& Item : UserAllItems.CharacterItemContainers[PCI].AllEuippedInfo)
		{
			if (Item.bIsEquipped)
				GatheredLocalEquippedItems.Add(Item.EquippedItem);
		}
		for (auto& Elem : UserAllItems.CharacterItemContainers[PCI].AllEuippedCostumeInfo)
		{
			FEquippedItemInfo& Item = Elem.Value;
			if (Item.bIsEquipped)
				GatheredLocalEquippedItems.Add(Item.EquippedItem);
		}

		CombatStatEval::GetSetItemValue_Version2(GatheredLocalEquippedItems, OutSetItemDatas);
	}	
}

//float FB2ModPlayerInfo::GetOptionStatusValue
//수정시 위에것두 수정
float FLocalCharacterData::GetOptionStatusValue(EPCClass InPCClass, EItemOption OptionType, TArray<FB2Item>* InItemArray/* = NULL*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetOptionStatusValue);
	const int8 PCI = PCClassToInt(InPCClass);
	
	//if (UserAllItems.CharacterItemContainers.IsValidIndex(PCI) && AllWingData.IsValidIndex(PCI))
	//{
	//	TArray<FB2Item> GatheredLocalEquippedItems;
	//	GetEquippedItems(InPCClass, GatheredLocalEquippedItems);
	//	GetEquippedCostumeItems(InPCClass, GatheredLocalEquippedItems);

	//	if (AllEthers.EquippedEthers.Contains(InPCClass) == false)
	//	{
	//		AllEthers.EquippedEthers.Add(InPCClass, TMap<int64, FB2Ether>());
	//	}

	//	// 장착 장비를 따로 지정할 수 있다. 다른 장비 입혀놓고 프리뷰를 하는 등의 용도로
	//	const TArray<FB2Item>& FinalEquipmentInfo = InItemArray ? *InItemArray : GatheredLocalEquippedItems;

	//	TArray<FB2FairyStatusInfo> FairyStatus;
	//	FairyManager::GetInstance().GetFairyInfos(FairyStatus);
	//	
	//	TMap<int64, FB2Totem> GatheredLocalEquippedTotems;
	//	GetEquippedTotems(InPCClass, GatheredLocalEquippedTotems);

	//	return CombatStatEval::GetOptionStatusValue(InPCClass,
	//		OptionType,
	//		FinalEquipmentInfo,
	//		AllWingData[PCI],
	//		GetAppliedCharBrevetRank(InPCClass),
	//		GetAppliedCharBrevetNodeIndex(InPCClass),
	//		RelicInfos.FindRef(InPCClass),
	//		AllEthers.EquippedEthers[InPCClass],
	//		FairyStatus,
	//		GatheredLocalEquippedTotems
	//	);
	//}
	return 0.0f;
}


//float FB2ModPlayerInfo::GetOptionStatusValueByMod
//수정시 위에것두 수정
float FLocalCharacterData::GetOptionStatusValueByMod(EPCClass InPCClass, EB2GameMode ModeType, EItemOption OptionType, TArray<FB2Item>* InItemArray /* = NULL */)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetOptionStatusValueByMod);
	const int8 PCI = PCClassToInt(InPCClass);

	//if (UserAllItems.CharacterItemContainers.IsValidIndex(PCI) && AllWingData.IsValidIndex(PCI))
	//{
	//	TArray<FB2Item> GatheredLocalEquippedItems;
	//	GetEquippedItems(InPCClass, GatheredLocalEquippedItems);
	//	GetEquippedCostumeItems(InPCClass, GatheredLocalEquippedItems);

	//	// 장착 장비를 따로 지정할 수 있다. 다른 장비 입혀놓고 프리뷰를 하는 등의 용도로
	//	const TArray<FB2Item>& FinalEquipmentInfo = InItemArray ? *InItemArray : GatheredLocalEquippedItems;

	//	TMap<int64, FB2Totem> GatheredLocalEquippedTotems;
	//	GetEquippedTotems(InPCClass, GatheredLocalEquippedTotems);

	//	return CombatStatEval::GetOptionStatusValueByMod(InPCClass,
	//		ModeType,
	//		OptionType,
	//		FinalEquipmentInfo,
	//		AllWingData[PCI],
	//		GetAppliedCharBrevetRank(InPCClass),
	//		GetAppliedCharBrevetNodeIndex(InPCClass),
	//		RelicInfos.FindRef(InPCClass),
	//		GatheredLocalEquippedTotems
	//	);
	//}
	return 0.0f;
}

bool FLocalCharacterData::GetOptionStatusRawValues(EPCClass InPCClass, EItemOption OptionType, CombatStatInfoRawDatas& OutRawValues, TArray<FB2Item>* InItemArray /*= NULL*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetOptionStatusRawValues);
	const int8 PCI = PCClassToInt(InPCClass);

	//if (UserAllItems.CharacterItemContainers.IsValidIndex(PCI) && AllWingData.IsValidIndex(PCI))
	//{
	//	TArray<FB2Item> GatheredLocalEquippedItems;
	//	GetEquippedItems(InPCClass, GatheredLocalEquippedItems);
	//	GetEquippedCostumeItems(InPCClass, GatheredLocalEquippedItems);

	//	// 장착 장비를 따로 지정할 수 있다. 다른 장비 입혀놓고 프리뷰를 하는 등의 용도로
	//	const TArray<FB2Item>& FinalEquipmentInfo = InItemArray ? *InItemArray : GatheredLocalEquippedItems;

	//	TMap<int64, FB2Ether> EquippedEthers;
	//	GetEquippedEthers(InPCClass, EquippedEthers);

	//	TArray<FB2FairyStatusInfo> FairyStatus;
	//	FairyManager::GetInstance().GetFairyInfos(FairyStatus);

	//	TMap<int64, FB2Totem> GatheredLocalEquippedTotems;
	//	GetEquippedTotems(InPCClass, GatheredLocalEquippedTotems);

	//	return CombatStatEval::GetOptionStatusRawValues(InPCClass,
	//		OptionType,
	//		FinalEquipmentInfo,
	//		AllWingData[PCI],
	//		GetAppliedCharBrevetRank(InPCClass),
	//		GetAppliedCharBrevetNodeIndex(InPCClass),
	//		RelicInfos.FindRef(InPCClass),
	//		EquippedEthers,
	//		OutRawValues,
	//		FairyStatus,
	//		GatheredLocalEquippedTotems
	//	);
	//}
	return false;
}

float FLocalCharacterData::GetCombatStatusValue(EPCClass InPCClass, TArray<FB2Item>* InItemArray/* = NULL*/) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCombatStatusValue);
	
	const int8 PCI = PCClassToInt(InPCClass);
	float fSkillCombatPower = 0;

	TArray<FB2Item> GatheredLocalEquippedItems;
	TArray<FB2Item> GatheredLocalEquippedCostumes;
	GetEquippedItems(InPCClass, GatheredLocalEquippedItems);
	GetEquippedCostumeItems(InPCClass, GatheredLocalEquippedCostumes);

	// 장착 장비를 따로 지정할 수 있다. 다른 장비 입혀놓고 프리뷰를 하는 등의 용도로
	const TArray<FB2Item>& FinalEquipmentItemInfo = InItemArray ? *InItemArray : GatheredLocalEquippedItems;
	const TArray<FB2Item>& FinalEquipmentCostumeInfo = GatheredLocalEquippedCostumes;

	//TMap<int64, FB2Ether> EquippedEthers;
	//GetEquippedEthers(InPCClass, EquippedEthers);

	//TArray<FB2FairyStatusInfo> FairyStatus;
	//FairyManager::GetInstance().GetFairyInfos(FairyStatus);

	//TMap<int64, FB2Totem> GatheredLocalEquippedTotems;
	//GetEquippedTotems(InPCClass, GatheredLocalEquippedTotems);

	/*if (PlayerSharedStats.IsValidIndex(PCI) && AllWingData.IsValidIndex(PCI))
	{
		EItemPresetSlotType SlotType = GetPCClassSkillSlotID(InPCClass);
		int32 SkillSlotID = static_cast<int32>(SlotType);
		if (auto SkillInfo = GetPlayerStatBasicSkillInfo(InPCClass, SkillSlotID))
			fSkillCombatPower = SkillInfo->skill_total_power;

		return fSkillCombatPower + CombatStatEval::GetCombatStatusValue(InPCClass,
			FinalEquipmentItemInfo,
			FinalEquipmentCostumeInfo,
			AllWingData[PCI],
			GetAppliedCharBrevetRank(InPCClass),
			GetAppliedCharBrevetNodeIndex(InPCClass),
			RelicInfos.FindRef(InPCClass),
			UnitySkillMissionInfos.FindRef(InPCClass),
			EquippedEthers,
			FairyStatus,
			GatheredLocalEquippedTotems);
	}*/

	return 0.0f;
}

//////////////////////////////////////////////////////////////////////////
float FLocalCharacterData::GetRequestCombatStatusValue(EPCClass InPCClass, TArray<b2network::B2CharacterPowerSpecificPtr>& OutCharacterPowerPtr)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetRequestCombatStatusValue);

	const int8 PCI = PCClassToInt(InPCClass);
	float fSkillCombatPower = 0;

	TArray<FB2Item> GatheredLocalEquippedItems;
	TArray<FB2Item> GatheredLocalEquippedCostumes;
	GetEquippedItems(InPCClass, GatheredLocalEquippedItems);
	GetEquippedCostumeItems(InPCClass, GatheredLocalEquippedCostumes);

	const TArray<FB2Item>& FinalEquipmentItemInfo = GatheredLocalEquippedItems;
	const TArray<FB2Item>& FinalEquipmentCostumeInfo = GatheredLocalEquippedCostumes;

	//TMap<int64, FB2Ether> EquippedEthers;
	//GetEquippedEthers(InPCClass, EquippedEthers);

	//TArray<FB2FairyStatusInfo> FairyStatus;
	//FairyManager::GetInstance().GetFairyInfos(FairyStatus);

	//TMap<int64, FB2Totem> GatheredLocalEquippedTotems;
	//GetEquippedTotems(InPCClass, GatheredLocalEquippedTotems);

	//if (PlayerSharedStats.IsValidIndex(PCI) && AllWingData.IsValidIndex(PCI))
	//{
	//	EItemPresetSlotType SlotType = GetPCClassSkillSlotID(InPCClass);
	//	int32 SkillSlotID = static_cast<int32>(SlotType);
	//	if (auto SkillInfo = GetPlayerStatBasicSkillInfo(InPCClass, SkillSlotID))
	//		fSkillCombatPower = SkillInfo->skill_total_power;

	//	/* SKILL_POWER */
	//	auto SkillPowerSpecific = std::make_shared<b2network::B2CharacterPowerSpecific>();
	//	SkillPowerSpecific->category = static_cast<int32>(ECharacterPowerSpecific::SKILL_POWER);
	//	SkillPowerSpecific->power = fSkillCombatPower;
	//	OutCharacterPowerPtr.Add(SkillPowerSpecific);
	//	BII_SCREEN_LOG(FString::Printf(TEXT("CombatPower (Skill) : %0.2f"), fSkillCombatPower), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);

	//	return fSkillCombatPower + CombatStatEval::GetRequestCombatStatusValue(InPCClass,
	//		FinalEquipmentItemInfo,
	//		FinalEquipmentCostumeInfo,
	//		AllWingData[PCI],
	//		GetAppliedCharBrevetRank(InPCClass),
	//		GetAppliedCharBrevetNodeIndex(InPCClass),
	//		RelicInfos.FindRef(InPCClass),
	//		UnitySkillMissionInfos.FindRef(InPCClass),
	//		EquippedEthers,
	//		FairyStatus,
	//		GatheredLocalEquippedTotems,
	//		OutCharacterPowerPtr);
	//}

	return 0.0f;
}
//////////////////////////////////////////////////////////////////////////

void FLocalCharacterData::SetCharClassNames(UB2PCClassInfoBox* InClassInfoBox)
{ // 한번만 해주면 되는데 InitializeAccountInfo 시점에서는 PCClassInfoBox 를 가져오지 못할 수 있어서 따로 외부에서..
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetCharClassNames);
	if (InClassInfoBox)
	{
		//for (int8 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
		//{
		//	UB2PCClassInfo* ThisClassInfo = InClassInfoBox->GetSingleClassInfo(IntToPCClass(PCI));
		//	if (ThisClassInfo && CharNames.IsValidIndex(PCI))
		//	{
		//		CharNames[PCI].ClassName = ThisClassInfo->GetLocalizedName();
		//	}
		//}
	}
}

void FLocalCharacterData::SetCharNickName(EPCClass InClass, const FText& InNewNickName)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetCharNickName);
	const int8 PCI = PCClassToInt(InClass);
	if (CharNames.IsValidIndex(PCI))
	{
		CharNames[PCI].NickName = InNewNickName;
	}
}

void FLocalCharacterData::SetCharBrevetInfo(EPCClass InClass, int32 RankIndex, int32 BrevetNodeIndex, bool BrevetNodeState)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetCharBrevetInfo);
	if (auto CharInfo = GetPlayerStatBasicInfo(InClass))
	{
		CharInfo->rank = RankIndex;
		CharInfo->rank_node_index = BrevetNodeIndex;
		CharInfo->rank_node_state = BrevetNodeState;
	}

	////////////////////////////////////////////////////////////////////
	////액션 지표 로그 (캐릭터 진급)
	//FString RankName(TEXT("PROMOTION_NONE"));
	//FString RankNameKey = BladeIIGameImpl::GetClientDataStore().GetBrevetRankNameKey(RankIndex);
	//RankName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), RankNameKey).ToString().ToUpper();

	//B2ActionLogSender Sender;
	//Sender.setCategory(FString(TEXT("CHARACTER")))
	//	.setAction(FString(TEXT("PROMOTION")))
	//	.setLabel(B2ActionLogType<EPCClass>::GetName(InClass))
	//	.setActionAttr1(RankName)
	//	.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	////////////////////////////////////////////////////////////////////
}

int32 FLocalCharacterData::GetCharBrevetNodeIndex(EPCClass InClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharBrevetNodeIndex);
	if (auto CharInfo = GetPlayerStatBasicInfo(InClass))
		return CharInfo->rank_node_index;

	return 0;
}

//FB2ModPlayerInfo::GetAppliedCharBrevetRank 수정
int32 FLocalCharacterData::GetAppliedCharBrevetRank(EPCClass InCharacterClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetAppliedCharBrevetRank);
	int32 CurRank = GetCharBrevetRank(InCharacterClass);
	int32 CurNodeIndex = GetCharBrevetNodeIndex(InCharacterClass);

	// 첫 랭크(랭크 1)가 아니고 랭크의 첫노드일 경우 이전 랭크를 반환
	if (CurRank != 0 && CurRank != 1 && CurNodeIndex == 1)
		return CurRank-1;
	
	// 일반경우는 현재 랭크가 적용된 랭크
	return CurRank;
}

//FB2ModPlayerInfo::GetAppliedCharBrevetNodeIndex 수정
int32 FLocalCharacterData::GetAppliedCharBrevetNodeIndex(EPCClass InClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetAppliedCharBrevetNodeIndex);
	int32 CurRank = GetCharBrevetRank(InClass);
	int32 CurNodeIndex = GetCharBrevetNodeIndex(InClass);

	//if (CurRank == 0)
	//	return 0;

	//// 첫 랭크가 아닌 경우 첫번쨰 노드는 이전 랭크의 최대 인덱스가 적용값
	//if (CurRank != 1 && CurNodeIndex == 1)
	//	return BladeIIGameImpl::GetClientDataStore().GetBrevetNodeCount(CurRank);

	//// 마지막 랭킹에 마지막 노드인덱스면
	//if (BladeIIGameImpl::GetClientDataStore().GetBrevetMaxRank() == CurRank 
	//	&& BladeIIGameImpl::GetClientDataStore().GetBrevetNodeCount(CurRank) == CurNodeIndex
	//	&& GetCharBrevetNodeState(InClass))
	//	return CurNodeIndex;

	// 기존적으로는 현재 노드에서 하나 작은게 적용된 노드
	return CurNodeIndex - 1;
}


bool FLocalCharacterData::GetCharBrevetNodeState(EPCClass InClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharBrevetNodeState);
	if (auto CharInfo = GetPlayerStatBasicInfo(InClass))
		return CharInfo->rank_node_state;	// rank_node_state (-1 : 아무 수행도 안함, 0 :  1

	return false;
}

//이건 지금 유물 구조로는 어려우니....쓰지 말자
/*
void FLocalCharacterData::SetRelicInfos(TMap<EPCClass, FAncientRelicArray> InRelicInfos)
{
	RelicInfos = InRelicInfos;
}
*/

void FLocalCharacterData::SetRelicInfo(EPCClass nClass, FB2ResponseGetAccountRelicPtr msgPtr)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetRelicInfo);
	if (!RelicInfos.Contains(nClass)) return;

	RelicInfos[nClass].Empty();

	for (auto RelicItem : msgPtr->relics) // 현재 오픈된 유물의 정보.
	{
		RelicInfos[nClass].Add(FAncientRelic(RelicItem->relic_id, RelicItem->enhance_level, RelicItem->grade));
	}
}

void FLocalCharacterData::SetRelicInfo(B2RelicPtr RelicInfo)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetRelicInfo);

	EPCClass PCClass = SvrToCliPCClassType(RelicInfo->character_type);
	if (!RelicInfos.Contains(PCClass)) return;
	
	for (auto Relicitem : RelicInfos[PCClass])
	{
		if(Relicitem.nRelicId == RelicInfo->relic_id) return;
	}

	RelicInfos[PCClass].Add(FAncientRelic(RelicInfo->relic_id, RelicInfo->enhance_level, RelicInfo->grade));
}

FAncientRelicArray* FLocalCharacterData::GetRelicInfo(EPCClass InClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetRelicInfo);
	return RelicInfos.Find(InClass);
}

void FLocalCharacterData::SetUnitySkillMissionInfo(EPCClass nClass, FB2ResponseGetUnitySkillMissionPtr msgPtr)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetUnitySkillMissionInfo);
	if (!UnitySkillMissionInfos.Contains(nClass)) return;

	UnitySkillMissionInfos[nClass].Empty();
	UnitySkillAwakenMissionInfos[nClass].Empty();

	for (auto MissionItem : msgPtr->missions) 
	{
		FB2OriginCharacterInfoPtr BasicInfo = GetPlayerStatBasicInfo(SvrToCliPCClassType(MissionItem->main_character_type));

		EPCClass MainChar = SvrToCliPCClassType(MissionItem->main_character_type);
		EPCClass UnityChar = SvrToCliPCClassType(MissionItem->unity_character_type);
		UnitySkillMissionInfos[nClass].Add(FUnitySkillMission(MainChar, 
															UnityChar, 
															MissionItem->step, 
															MissionItem->state));
	}

	for (auto MissionItem : msgPtr->awaken_missions)
	{
		UnitySkillAwakenMissionInfos[nClass].Add(FUnitySkillAwakenMission(SvrToCliPCClassType(MissionItem->character_type), MissionItem->mission_id, MissionItem->try_count, MissionItem->completed));
	}

	UnitySkillState[nClass].AllMissionComplete = msgPtr->all_mission_completed;
	UnitySkillState[nClass].AllAwakenComplete = msgPtr->all_awaken_mission_completed;

	FB2OriginCharacterInfoPtr BasicInfo = GetPlayerStatBasicInfo(nClass);
	UnitySkillState[nClass].HaveAwaken = BasicInfo->unity_skill_awaken;

}

void FLocalCharacterData::SetUnitySkillMissionInfo(TArray<B2UnitySkillMissionPtr> missions)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetUnitySkillMissionInfo);


	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		if (!UnitySkillMissionInfos.Contains(IntToPCClass(PCI)))
			return;
		
		UnitySkillMissionInfos[IntToPCClass(PCI)].Empty();
	}

	for (auto MissionItem : missions)
	{
		EPCClass MainChar = SvrToCliPCClassType(MissionItem->main_character_type);
		EPCClass UnityChar = SvrToCliPCClassType(MissionItem->unity_character_type);

		if (!UnitySkillMissionInfos.Contains(MainChar))
			continue;
		
		UnitySkillMissionInfos[MainChar].Add(FUnitySkillMission(MainChar,
				UnityChar,
				MissionItem->step,
				MissionItem->state));
	}

	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		bool AllCompelete = true;

		EPCClass CurrentPCClass = IntToPCClass(PCI);
		if (UnitySkillMissionInfos.Contains(CurrentPCClass))
		{
			for (auto MissionItem : UnitySkillMissionInfos[CurrentPCClass])
			{
				if (!(MissionItem.nStep == 8 && MissionItem.nState == 3) && MissionItem.SubClass != CurrentPCClass)
				{
					AllCompelete = false;
					break;
				}
			}
		}

		if (UnitySkillState.Contains(CurrentPCClass))
		{
			UnitySkillState[CurrentPCClass].AllMissionComplete = AllCompelete;
		}
	}
}

void FLocalCharacterData::UpdateUnitySkillMissionInfo(const FB2ResponseUnitySkillRewardPtr RewardPtr)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_UpdateUnitySkillMissionInfo);

	auto NextMissionPtr = RewardPtr->next_mission;
	EPCClass MainClass = SvrToCliPCClassType(NextMissionPtr->main_character_type);
	EPCClass SubClass = SvrToCliPCClassType(NextMissionPtr->unity_character_type);

	if(UnitySkillMissionInfos.Contains(MainClass))
	{
		bool AllMissionCompelete = true;
		for (auto &MissionItem :UnitySkillMissionInfos[MainClass])
		{
			if (MissionItem.SubClass == SubClass)
			{
				MissionItem.nStep = NextMissionPtr->step;
				MissionItem.nState = NextMissionPtr->state;
			}

			if (!(MissionItem.nStep == 8 && MissionItem.nState == 3) && MissionItem.SubClass != MainClass)
				AllMissionCompelete = false;
		}

		UnitySkillState[MainClass].AllMissionComplete = AllMissionCompelete;
	}
}

FUnitySkillMissionArray* FLocalCharacterData::GetUnitySkillMission(EPCClass InClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetUnitySkillMission);
	return UnitySkillMissionInfos.Find(InClass);
}

FUnitySkillAwakenMissionArray* FLocalCharacterData::GetUnitySkillAwakenMission(EPCClass InClasss)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetUnitySkillAwakenMission);
	return UnitySkillAwakenMissionInfos.Find(InClasss);
}

FUnitySkillState* FLocalCharacterData::GetUnitySkillState(EPCClass InClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetUnitySkillState);
	return UnitySkillState.Find(InClass);
}

void FLocalCharacterData::SetUnitySkillAwakenMission(EPCClass InClass, int32 nMissionID, bool bComplete, int32 ntry_count)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetUnitySkillAwakenMission);

	if (UnitySkillAwakenMissionInfos.Contains(InClass))
	{
		bool AllCompelete = true;
		for (auto &MissionItem : UnitySkillAwakenMissionInfos[InClass])
		{
			if (MissionItem.nMissionID == nMissionID)
			{
				MissionItem.bCompleted = bComplete;
				MissionItem.ntry_count = ntry_count;
			}

			if (!MissionItem.bCompleted)
			{
				AllCompelete = false;
			}
		}
		UnitySkillState[InClass].AllAwakenComplete = AllCompelete;
	}
}

void FLocalCharacterData::SetUnitySkillAwakenMission(TArray<b2network::B2UnitySkillAwakenMissionPtr> missions)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetUnitySkillAwakenMission);
	
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		UnitySkillAwakenMissionInfos[IntToPCClass(PCI)].Empty();
	}


	for (auto MissionItem : missions)
	{
		EPCClass CurrentPCClass = SvrToCliPCClassType(MissionItem->character_type);

		if(UnitySkillAwakenMissionInfos.Contains(CurrentPCClass))
			UnitySkillAwakenMissionInfos[CurrentPCClass].Add(FUnitySkillAwakenMission(CurrentPCClass, MissionItem->mission_id, MissionItem->try_count, MissionItem->completed));
	}

	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		bool AllCompelete = true;
		EPCClass CurrentPCClass = IntToPCClass(PCI);
		if (UnitySkillAwakenMissionInfos.Contains(CurrentPCClass))
		{
			for (auto& MissionItem : UnitySkillAwakenMissionInfos[CurrentPCClass])
			{
				if (!MissionItem.bCompleted)
				{
					AllCompelete = false;
					break;
				}
			}
		}

		FB2OriginCharacterInfoPtr BasicInfo = GetPlayerStatBasicInfo(CurrentPCClass);
		if (UnitySkillState.Contains(CurrentPCClass))
		{
			UnitySkillState[CurrentPCClass].HaveAwaken = BasicInfo->unity_skill_awaken;
			UnitySkillState[CurrentPCClass].AllAwakenComplete = AllCompelete;
		}
	}
}

void FLocalCharacterData::SetUnitySkillAwakeSuccess(EPCClass InClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(SetUnitySkillAwakeSuccess);

	if(UnitySkillState.Contains(InClass))
		UnitySkillState.Find(InClass)->HaveAwaken = true;

	FB2OriginCharacterInfoPtr BasicInfo = GetPlayerStatBasicInfo(InClass);
	if (BasicInfo)
	{
		BasicInfo->unity_skill_awaken = true;
	}
}

int32 FLocalCharacterData::GetEnhanceSetEffectLevel(EPCClass InPCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetEnhanceSetEffectLevel);
	if (UserAllItems.CharacterItemContainers.IsValidIndex(PCClassToInt(InPCClass)) == false)
		return 0;

	auto& EquipedInfos = UserAllItems.CharacterItemContainers[PCClassToInt(InPCClass)].AllEuippedInfo;

	TArray<FB2Item> AllEquipItems;
	for (FEquippedItemInfo& EquipInfo : EquipedInfos)
	{
		if (EquipInfo.bIsEquipped)
			AllEquipItems.Add(EquipInfo.EquippedItem);
	}

	//return CombatStatEval::GetEnhanceSetEffectLevel(AllEquipItems);
	return 0;
}

double FLocalCharacterData::GetEnhanceSetEffectFactor(EPCClass InPCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetEnhanceSetEffectFactor);
	//if (UserAllItems.CharacterItemContainers.IsValidIndex(PCClassToInt(InPCClass)) == false)
		return 0.0f;

	//auto& EquipedInfos = UserAllItems.CharacterItemContainers[PCClassToInt(InPCClass)].AllEuippedInfo;

	//TArray<FB2Item> AllEquipItems;
	//for (FEquippedItemInfo& EquipInfo : EquipedInfos)
	//{
	//	if (EquipInfo.bIsEquipped)
	//		AllEquipItems.Add(EquipInfo.EquippedItem);
	//}

	//return CombatStatEval::GetEnhanceSetEffectFactor(AllEquipItems);
}

int64 FLocalCharacterData::GetAccountId()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetAccountId);
	//return BladeIIGameImpl::GetClientDataStore().GetAccountId();
	return 0.0;
}

int32 FLocalCharacterData::GetEtherOffenseSetID(EPCClass InPCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetEtherOffenseSetID);
	FB2OriginCharacterInfoPtr BasicInfo = GetPlayerStatBasicInfo(InPCClass);
	int32 Result = 0;
	if (BasicInfo)
	{
		Result = BasicInfo->aether_offense_set_option_id;
	}
	return Result;
}

int32 FLocalCharacterData::GetEtherDefenceSetID(EPCClass InPCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetEtherDefenceSetID);
	FB2OriginCharacterInfoPtr BasicInfo = GetPlayerStatBasicInfo(InPCClass);
	int32 Result = 0;
	if (BasicInfo)
	{
		Result = BasicInfo->aether_defense_set_option_id;
	}
	return Result;
}

bool FLocalCharacterData::GetIsUnityAwaken(EPCClass InPCClass)
{
	FB2OriginCharacterInfoPtr BasicInfo = GetPlayerStatBasicInfo(InPCClass);
	bool Result = false;
	if (BasicInfo)
	{
		Result = BasicInfo->unity_skill_awaken;
	}

	return Result;
}

FPCWingContainer* FLocalCharacterData::GetCharacterWingDataPtr(EPCClass InPCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterWingDataPtr);
	const int8 PCI = PCClassToInt(InPCClass);
	return AllWingData.IsValidIndex(PCI) ? &AllWingData[PCI] : NULL;
}
bool FLocalCharacterData::DoesCharacterHaveWing(EPCClass InPCClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_DoesCharacterHaveWing);
	const int8 PCI = PCClassToInt(InPCClass);
	return AllWingData.IsValidIndex(PCI) ? AllWingData[PCI].bHasWing : false;
}
bool FLocalCharacterData::GetCharacterWing(EPCClass InPCClass, FB2Wing& OutData) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetCharacterWing);
	const int8 PCI = PCClassToInt(InPCClass);

	if (AllWingData.IsValidIndex(PCI) && AllWingData[PCI].bHasWing)
	{
		OutData = AllWingData[PCI].WingData;
		return true;
	}
	return false;
}

bool FLocalCharacterData::IsShowHeadItem(EPCClass InCharacterClass) const
{
	int32 ClassIndex = PCClassToInt(InCharacterClass);
	if (CacheShowPlayerHeads.IsValidIndex(ClassIndex))
		return CacheShowPlayerHeads[ClassIndex];
	
	return true;
}

bool FLocalCharacterData::IsShowCostume(EPCClass InCharacterClass, ECostumeEquipPlace InCostumeEquipPlace) const
{
	//return BladeIIGameImpl::GetCostumeDataStore().GetCostumeVisible(InCharacterClass, InCostumeEquipPlace);
	return true;
}

void FLocalCharacterData::SetCacheShowHeadItem(EPCClass InCharacterClass, bool ShowHead)
{
	int32 ClassIndex = PCClassToInt(InCharacterClass);
	if (CacheShowPlayerHeads.IsValidIndex(ClassIndex))
		CacheShowPlayerHeads[ClassIndex] = ShowHead;
}

void FLocalCharacterData::SetShowHeadItem(EPCClass InCharacterClass, bool ShowHead)
{
	int32 ClassIndex = PCClassToInt(InCharacterClass);
	if (PlayerSharedStats.IsValidIndex(ClassIndex) && PlayerSharedStats[ClassIndex].BasicInfo)
		PlayerSharedStats[ClassIndex].BasicInfo->helmet_visible = ShowHead;
}

void FLocalCharacterData::InitShowHeadItemCache()
{
	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		if (CacheShowPlayerHeads.IsValidIndex(i) && PlayerSharedStats.IsValidIndex(i) && PlayerSharedStats[i].BasicInfo)
			CacheShowPlayerHeads[i] = PlayerSharedStats[i].BasicInfo->helmet_visible;
	}
}

void FLocalCharacterData::InitShowCostumeCache()
{
	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		if (PlayerSharedStats.IsValidIndex(i) && PlayerSharedStats[i].BasicInfo)
		{
			//EPCClass PCClass = IntToPCClass(i);
			//BladeIIGameImpl::GetCostumeDataStore().SetCostumeVisible(PCClass, ECostumeEquipPlace::ECEP_Weapon , PlayerSharedStats[i].BasicInfo->costume_weapon_visible);
			//BladeIIGameImpl::GetCostumeDataStore().SetCostumeVisible(PCClass, ECostumeEquipPlace::ECEP_Helmet , PlayerSharedStats[i].BasicInfo->costume_hat_visible);
			//BladeIIGameImpl::GetCostumeDataStore().SetCostumeVisible(PCClass, ECostumeEquipPlace::ECEP_Body , PlayerSharedStats[i].BasicInfo->costume_armor_visible);
		}
	}
}

void FLocalCharacterData::GetSyncShowHeadItemClass(TArray<EPCClass>& SyncClassList)
{
	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	{
		if (CacheShowPlayerHeads.IsValidIndex(i) && PlayerSharedStats.IsValidIndex(i) && PlayerSharedStats[i].BasicInfo)
		{
			if (CacheShowPlayerHeads[i] != PlayerSharedStats[i].BasicInfo->helmet_visible)
				SyncClassList.Add(IntToPCClass(i));
		}
	}
}

void FLocalCharacterData::GetSyncCostumeVisibleParts(TArray<TPair<EPCClass, ECostumeEquipPlace>>& SyncList)
{
	SyncList.Empty();

	//for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	//{
	//	if (PlayerSharedStats.IsValidIndex(i) && PlayerSharedStats[i].BasicInfo)
	//	{
	//		EPCClass CurrentPCClass = IntToPCClass(i);
	//		for (int32 j = 0; j < FCostumeDataStore::GetCostumeEquipArray().CostumeEquipArray.Num(); j++)
	//		{
	//			ECostumeEquipPlace CurrentPlace = FCostumeDataStore::GetCostumeEquipArray().CostumeEquipArray[j];
	//			bool bLocalVsible = BladeIIGameImpl::GetCostumeDataStore().GetCostumeVisible(CurrentPCClass, CurrentPlace);
	//			if (GetServerCostumeVisible(CurrentPCClass, CurrentPlace) != bLocalVsible)
	//				SyncList.Add(TPair<EPCClass, ECostumeEquipPlace>(CurrentPCClass, CurrentPlace));
	//		}
	//	}
	//}
}

bool FLocalCharacterData::GetServerCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquipPlace)
{
	int32 Index = PCClassToInt(InPCClass);

	if (PlayerSharedStats.IsValidIndex(Index) && PlayerSharedStats[Index].BasicInfo)
	{
		switch (InCostumeEquipPlace)
		{
		case ECostumeEquipPlace::ECEP_Weapon:
			return PlayerSharedStats[Index].BasicInfo->costume_weapon_visible;
		case ECostumeEquipPlace::ECEP_Helmet:
			return PlayerSharedStats[Index].BasicInfo->costume_hat_visible;
		case ECostumeEquipPlace::ECEP_Body:
			return PlayerSharedStats[Index].BasicInfo->costume_armor_visible;
		}
	}
	return true;
}

void FLocalCharacterData::SetServerCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquipPlace, bool bVisible)
{
	int32 Index = PCClassToInt(InPCClass);

	if (PlayerSharedStats.IsValidIndex(Index) && PlayerSharedStats[Index].BasicInfo)
	{
		switch (InCostumeEquipPlace)
		{
		case ECostumeEquipPlace::ECEP_Weapon:
			PlayerSharedStats[Index].BasicInfo->costume_weapon_visible = bVisible;
			break;
		case ECostumeEquipPlace::ECEP_Helmet:
			PlayerSharedStats[Index].BasicInfo->costume_hat_visible = bVisible;
			break;
		case ECostumeEquipPlace::ECEP_Body:
			PlayerSharedStats[Index].BasicInfo->costume_armor_visible = bVisible;
			break;
		}
	}
}

void FLocalCharacterData::SetUpdatePCClassSkillSlotID(EPCClass InPCClass, EItemPresetSlotType InSlotID)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SetUpdatePCClassSkillSlotID);
	const int8 PCI = PCClassToInt(InPCClass);

	if (PlayerSharedStats.IsValidIndex(PCI))
	{
		PlayerSharedStats[PCI].SetSkillSlotID(InSlotID);
		PlayerSharedStats[PCI].CheckNeedRegistSkill();
	}
}

EItemPresetSlotType FLocalCharacterData::GetPCClassSkillSlotID(EPCClass InPCClass) const
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_GetPCClassSkillSlotID);
	const int8 PCI = PCClassToInt(InPCClass);

	if (PlayerSharedStats.IsValidIndex(PCI))
	{
		return PlayerSharedStats[PCI].GetSkillSlotID();
	}

	return EItemPresetSlotType::EIPS_First; // default slot id
}

EItemPresetSlotType FLocalCharacterData::GetPCClassItemPresetSlotID(EPCClass InPCClass)
{
	const int32 PresetID = 0;// UserAllItems.GetCurrentItemPresetID(InPCClass);
	return static_cast<EItemPresetSlotType>(PresetID);
}

void FLocalCharacterData::SaveLocalData()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_SaveLocalData);
	//if (GConfig)
	//{
	//	GConfig->SetInt(TEXT("DataStore"), TEXT("MainPlayerClass"), PCClassToInt(MainPlayerClass), GB2UserSavedStateIni);
	//	GConfig->SetInt(TEXT("DataStore"), TEXT("SubPlayerClass"), PCClassToInt(SubPlayerClass), GB2UserSavedStateIni);


	//	FString SavePropertyText(TEXT("Characters"));

	//	SavePropertyText = SavePropertyText + TEXT(".SkillPlayedUnlockFX.");

	//	for (TMap<int32, bool>::TConstIterator It(PlayedUnlockFX); It; ++It)
	//	{
	//		FString ToSavePropertyText = SavePropertyText + FString::FromInt(It.Key());

	//		GConfig->SetBool(TEXT("CharStore"), *ToSavePropertyText, It.Value(), GB2UserSavedStateIni);
	//	}

	//	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	//	{
	//		// Item
	//		FString ItemPresetKey = FString::Printf(TEXT("%s_%d"), TEXT("ItemPresetID"), i);
	//		int32 ItemPresetID = UserAllItems.GetCurrentItemPresetID((EPCClass)i);
	//		GConfig->SetInt(TEXT("CharStore"), *ItemPresetKey, ItemPresetID, GB2UserSavedStateIni);

	//		// Skill
	//		FString SkillPresetKey = FString::Printf(TEXT("%s_%d"), TEXT("SkillSlotID"), i);
	//		int32 SkillPresetID = static_cast<int32>(PlayerSharedStats[i].GetSkillSlotID());
	//		GConfig->SetInt(TEXT("CharStore"), *SkillPresetKey, SkillPresetID, GB2UserSavedStateIni);
	//	}

	//	GConfig->Flush(false, GB2UserSavedStateIni);
	//}
}

void FLocalCharacterData::LoadLocalData()
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_LoadLocalData);
	//if (GConfig)
	//{
	//	int32 SavedMainPlayerClass = 2;
	//	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	//	if (GConfig->GetInt(TEXT("DataStore"), TEXT("MainPlayerClass"), SavedMainPlayerClass, GB2UserSavedStateIni))
	//	{
	//		MainPlayerClass = IntToPCClass(FMath::Clamp(SavedMainPlayerClass, 0, GetMaxPCClassNum() - 1));

	//		if(DocBattle)
	//			DocBattle->SetCurPCClass(SavedMainPlayerClass);
	//	}
	//	int32 SavedSubPlayerClass = 3;
	//	if (GConfig->GetInt(TEXT("DataStore"), TEXT("SubPlayerClass"), SavedSubPlayerClass, GB2UserSavedStateIni))
	//	{
	//		SubPlayerClass = IntToPCClass(FMath::Clamp(SavedSubPlayerClass, 0, GetMaxPCClassNum() - 1));

	//		if (DocBattle)
	//			DocBattle->SetTagPCClass(SavedSubPlayerClass);
	//	}

	//	int32 SavedPresetID = 0;

	//	int32 MaxItemPresetCount = BladeIIGameImpl::GetClientDataStore().GetItemPresetMaxCount();
	//	int32 MaxSkillPresetCount = BladeIIGameImpl::GetClientDataStore().GetItemPresetMaxCount();
	//	for (int32 i = 0; i < GetMaxPCClassNum(); ++i)
	//	{
	//		FString ItemPresetKey = FString::Printf(TEXT("%s_%d"), TEXT("ItemPresetID"), i);
	//		if (GConfig->GetInt(TEXT("CharStore"), *ItemPresetKey, SavedPresetID, GB2UserSavedStateIni))
	//		{
	//			int32 PresetID = FMath::Clamp(SavedPresetID, 0, MaxItemPresetCount - 1);
	//			UserAllItems.SetCurrentItemPresetID(IntToPCClass(i), PresetID);
	//		}

	//		FString SkillPresetKey = FString::Printf(TEXT("%s_%d"), TEXT("SkillSlotID"), i);
	//		if (GConfig->GetInt(TEXT("CharStore"), *SkillPresetKey, SavedPresetID, GB2UserSavedStateIni))
	//		{
	//			int32 SlotID = FMath::Clamp(SavedPresetID, 0, MaxSkillPresetCount - 1);
	//			EItemPresetSlotType SkillSlot = static_cast<EItemPresetSlotType>(SlotID);
	//			PlayerSharedStats[i].SetSkillSlotID(SkillSlot);
	//		}
	//	}

	//	if (MainPlayerClass == SubPlayerClass)
	//		SubPlayerClass = IntToPCClass(((int32)SubPlayerClass + 1) % (GetMaxPCClassNum()));
	//}
}

void FLocalCharacterData::LoadLocalData_ResourceBound()
{// 모듈 Startup 단계에서는 적절치 않은 InfoAsset 내지는 MasterData 가 필요한 config 데이터가 있어서 따로 떼어냄.
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_LoadLocalData_ResourceBound);
	//if (GConfig)
	//{
	//	auto* PCClassInfoBox = StaticFindPCClassInfoBox();
	//	auto* AllSkillInfo = PCClassInfoBox ? PCClassInfoBox->GetAllSkillInfo() : nullptr;

	//	if (AllSkillInfo)
	//	{
	//		FString LoadPropertyText(TEXT("Characters"));
	//		LoadPropertyText = LoadPropertyText + TEXT(".SkillPlayedUnlockFX.");

	//		auto& SkillInfoMap = AllSkillInfo->GetSkillInfoMap();

	//		for (TMap<int32, FSingleSkillInfo*>::TConstIterator It(SkillInfoMap); It; ++It)
	//		{
	//			// 키 값에 스킬 ID 가 들어가는데 이거 리스트를 현재는 InfoAsset 에서 얻어올 수 있음. 그게 아니어도 MasterData 든지..
	//			FString ToLoadPropertyText = LoadPropertyText + FString::FromInt(It.Key());

	//			bool bSavedProperty = false;
	//			if (GConfig->GetBool(TEXT("CharStore"), *ToLoadPropertyText, bSavedProperty, GB2UserSavedStateIni))
	//			{
	//				PlayedUnlockFX.Add(It.Key(), bSavedProperty);
	//			}
	//		}
	//	}
	//}
}

FPCWingContainer* FTutorialCharacterData::GetCharacterWingDataPtr(EPCClass InPCClass)
{
	BLADE2_SCOPE_CYCLE_COUNTER(FLocalCharacterData_LoadLocalData_GetCharacterWingDataPtr);
	return nullptr;
}

const TArray<int32>& FTutorialCharacterData::GetPreviewCostumeItems() const
{
	static TArray<int32> tmp;
	return tmp;
}


void ICharacterDataStore::GetRenderItem(const ICharacterDataStore* CharacterDataStore, EPCClass InCharacterClass, TArray<FB2Item>& EquippedItems, bool IgnoreFilter)
{
	if (CharacterDataStore == nullptr)
		return;

	if (IgnoreFilter == true)
		return;

	if (!CharacterDataStore->IsShowHeadItem(InCharacterClass))
	{
		auto compare = [](FB2Item& Item) { return Item.EquipPlace == EItemEquipPlace::EIEP_Helmet; };
		if (EquippedItems.ContainsByPredicate(compare))
		{
			EquippedItems.RemoveAll(compare);
		}
	}
	
	const TArray<int32>& PreviewCostumes = CharacterDataStore->GetPreviewCostumeItems();
	//Costume Preview
	for (int32 i = 0; i < PreviewCostumes.Num(); ++i)
	{
		FB2Item CostumeItem;
		CostumeItem.ItemRefID = PreviewCostumes[i];
		CostumeItem.UpdateItemMasterDataOnMD();
		
		EquippedItems.RemoveAll([&CostumeItem](const FB2Item& Item) {
			return CostumeItem.CostumeEquipPlace != ECostumeEquipPlace::ECEP_None &&
				   Item.CostumeEquipPlace == CostumeItem.CostumeEquipPlace;
		});
		EquippedItems.Add(CostumeItem);
	}

	TArray<ECostumeEquipPlace> RemoveCostumePlace;

	for (int32 Count = EquippedItems.Num() - 1 ; Count >= 0 ; --Count)
	{
		if (EquippedItems[Count].CostumeEquipPlace != ECostumeEquipPlace::ECEP_None)
		{
			if (CharacterDataStore->IsShowCostume(InCharacterClass, EquippedItems[Count].CostumeEquipPlace) == false)
				EquippedItems.RemoveAt(Count);
		}
	}
}
