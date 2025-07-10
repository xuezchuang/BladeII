
#include "B2ModPlayerInfoData.h"
#include "B2PCClassInfo.h"
//#include "B2SkillInfo.h"
//#include "B2Airport.h"
//#include "B2LobbyInventory.h"
//#include "B2CombatStatEvaluator.h"
//#include "FakeConfigure.h"
//#include "RelicManager.h"
//#include "Algo/Copy.h"
#include "FairyManager.h"

#include "BladeIIGameImpl.h"

FB2ModPlayerInfo::FB2ModPlayerInfo()
	: MainPlayerClass(EPCClass::EPC_Assassin)
	, SubPlayerClass(EPCClass::EPC_Gladiator)
{}

FB2ModPlayerInfo::~FB2ModPlayerInfo()
{}

FB2ModPlayerInfo::FB2ModPlayerInfo(b2network::B2FindAccountInfoPtr InPlayerInfo)
{
	SetModPlayerInfo(InPlayerInfo);
}

//Use only reference variable.
FB2ModPlayerInfo& FB2ModPlayerInfo::operator=(b2network::B2FindAccountInfoPtr InPlayerInfo)
{
	SetModPlayerInfo(InPlayerInfo);
	return *this;
}

void FB2ModPlayerInfo::SetModPlayerInfo(const FB2FindAccountInfoPtr& InPlayerInfo)
{
	check(InPlayerInfo);

	ServerAccountInfo = InPlayerInfo;

	NickName = InPlayerInfo->nickname;

	Characters.Empty(InPlayerInfo->characters.Num());
	for (auto Character : InPlayerInfo->characters)
	{
		int32 Index = Characters.AddDefaulted();
		check(Index != INDEX_NONE);
		Characters[Index].CharInfo = Character;
		Characters[Index].CharClass = SvrToCliPCClassType(Character->character_type);
	}
		
	ConstructEquipItems();
	ConstructEquipCostumeItems();
	ConstructEquipTotems();

	for (int32 CI = 0; CI < Characters.Num(); ++CI)
	{ 
		FB2ModCharInfo& ThisModCharInfo = Characters[CI];
		b2network::B2CharacterServerInfoPtr& ThisServerCharInfo = InPlayerInfo->characters[CI];

		// InPlayerInfo->wing_options 중에서 이번 캐릭터 것만 빼 옴. 아직 한번도 GetWing 을 하지 않은 캐릭터라면 옵션 데이터가 안 올 수 있다.
		TArray<b2network::B2WingOptionInfoPtr> WingOptionsForThisChar;
		for (int32 AllInfoIndex = 0; AllInfoIndex < InPlayerInfo->wing_options.Num(); ++AllInfoIndex)
		{
			const b2network::B2WingOptionInfoPtr& ThisOptionInfo = InPlayerInfo->wing_options[AllInfoIndex];
			if (ThisOptionInfo && ThisOptionInfo->character_type == ThisServerCharInfo->character_type)
			{
				WingOptionsForThisChar.Add(ThisOptionInfo);
			}
		}
		
		ThisModCharInfo.WingContainer.WingData.UpdateByPartialServerInfo(ThisServerCharInfo, WingOptionsForThisChar);
		ThisModCharInfo.WingContainer.SetHasWingByWingData(); // bHasWing 을 따로 서버에서 받아야 할 거는 같은데 일단 없으니..
	}

	MainPlayerClass = Characters.IsValidIndex(0) ? Characters[0].CharClass : EPCClass::EPC_End;
	SubPlayerClass = Characters.IsValidIndex(1) ? Characters[1].CharClass : EPCClass::EPC_End;
	
	for (auto RelicItem : InPlayerInfo->relics) // 현재 오픈된 유물의 정보.
	{
		for (auto& Character : Characters)
		{
			if (Character.CharInfo->character_type == RelicItem->character_type)
			{
				Character.RelicInfos.Add(FAncientRelic(RelicItem->relic_id, RelicItem->enhance_level, RelicItem->grade));
			}
		}
	}

	for (b2network::B2AetherServerInfoPtr EtherItem : InPlayerInfo->mount_aethers) // 에테르 정보
	{
		for (FB2ModCharInfo& Character : Characters)
		{
			if (Character.CharInfo->character_type == EtherItem->character_type)
			{
				FB2Ether Ether(EtherItem);
				Character.EquippedEthers.Add(EtherItem->id, Ether);
			}
		}
	}

	for (auto UnityItem : InPlayerInfo->unity_skills) // 현재 강화된 결속스킬 정보
	{
		for (auto& Character : Characters)
		{
			if (Character.CharInfo->character_type == UnityItem->main_character_type)
			{
				FUnitySkillMission NewMission = FUnitySkillMission(SvrToCliPCClassType(UnityItem->main_character_type), 
																	SvrToCliPCClassType(UnityItem->unity_character_type), 
																	UnityItem->step,
																	0);
				Character.UnityInfos.Add(NewMission);
			}
		}
	}
	//RelicInfos = InPlayerInfo->relics;

	//페어리
	FairyStatus.Empty();

	FairyStatus = FairyManager::GetInstance().ModUpdateFairyStatus(InPlayerInfo->fairies);


	//길드 스킬 정보
	GuildSkillInfos.Empty();
	GuildSkillInfos.Append(InPlayerInfo->guild_skills);
}

void FB2ModPlayerInfo::AddModCharInfo(const b2network::B2CharacterServerInfoPtr& InCharInfo, const TArray<FB2Item>& InEquippedItems)
{	
	int32 Index = Characters.AddDefaulted();
	check(Index != INDEX_NONE);

	Characters[Index].CharInfo = InCharInfo;
	Characters[Index].CharClass = SvrToCliPCClassType(InCharInfo->character_type);
	Characters[Index].EquippedItems.Append(InEquippedItems);
}

const FB2FindAccountInfoPtr FB2ModPlayerInfo::GetAccountInfoPtr()
{
	return ServerAccountInfo;
}

int32 FB2ModPlayerInfo::GetIndex(EPCClass CharClass) const
{
	if (CharClass == EPCClass::EPC_End)
		return 0;

	int32 Index = 0;
	for (auto& Character : Characters)
	{
		if (Character.CharClass == CharClass)
			break;

		Index++;
	}

	check(Characters.IsValidIndex(Index));
	return Index;
}

int32 FB2ModPlayerInfo::GetUserLevel() const
{
	//기획확립시까지 임시
	//제일 레벨이 높은 캐릭터 레벨 반환
	int32 UserLevel = 0;
	for (auto Character : Characters)
	{
		if (UserLevel < Character.CharInfo->level)
			UserLevel = Character.CharInfo->level;
	}

	return UserLevel;
}

int32 FB2ModPlayerInfo::GetCharacterLevel(EPCClass CharClass) const
{	
	return Characters[GetIndex(CharClass)].CharInfo->level;
}

void FB2ModPlayerInfo::GetEquippedItems(EPCClass CharClass, TArray<FB2Item>& EquippedItems) const
{
	EquippedItems = Characters[GetIndex(CharClass)].EquippedItems;
}

void FB2ModPlayerInfo::GetEquippedItems(EPCClass CharClass, TMap<EItemEquipPlace, FB2Item>& EquippedItems) const
{
	EquippedItems.Empty();
	for (auto& Item : Characters[GetIndex(CharClass)].EquippedItems)
	{
		EquippedItems.Add(Item.EquipPlace, Item);
	}
}

void FB2ModPlayerInfo::GetEquippedCostumeItems(EPCClass PCClass, TArray<FB2Item>& EquippedCostumeItems) const
{
	EquippedCostumeItems.Append(Characters[GetIndex(PCClass)].EquippedCostumeItems);
}

void FB2ModPlayerInfo::GetEquippedEthers(EPCClass PCClass, TMap<int64, FB2Ether>& EquippedEthers) const
{

}

void FB2ModPlayerInfo::GetEquippedTotems(EPCClass PCClass, TMap<int64, FB2Totem>& EquippedTotems) const
{

}

void FB2ModPlayerInfo::GetCharacterRegistredSkills(EPCClass InCharacterClass, TArray<int32>& RegistredSkills, bool bIncludeUnitySkil) const
{
	RegistredSkills.Empty();

	const int32 SkillPresetID = GetClientPresetID(EInventoryPresetMode::EIPM_Skill, InCharacterClass);
	auto CharInfo = Characters[GetIndex(InCharacterClass)].CharInfo;
	// TODO skill quick slot

	if (CharInfo && CharInfo->skill_quick_slots.IsValidIndex(SkillPresetID))
	{
		auto SkillInfo = CharInfo->skill_quick_slots[SkillPresetID];
		if (SkillInfo)
		{
			RegistredSkills.Add(SkillInfo->using_skill_id_1);
			RegistredSkills.Add(SkillInfo->using_skill_id_2);
			RegistredSkills.Add(SkillInfo->using_skill_id_3);
		}
	}	

	if (bIncludeUnitySkil)
		RegistredSkills.Add(GetUnitySkillId(InCharacterClass));
}

int32 FB2ModPlayerInfo::GetUnitySkillId(EPCClass InCharacterClass) const
{
	int32 SkillID = SKILL_INVALID_ID;

	b2network::B2CharacterServerInfoPtr CharInfo = Characters[GetIndex(InCharacterClass)].CharInfo;
	if (CharInfo)
	{
//#if !UE_BUILD_SHIPPING
//		if (NetFakeConfigure::GetInstance().GetForcedUnitySkillOpen())
//		{
//			if (CharInfo->aether_offense_set_option_id == b2network::B2AetherSetOptionType::SKILL_GAGE)
//			{
//				SkillID = (PCClassToInt(InCharacterClass) + 1) * 100 + 2;
//			}
//			else if (CharInfo->unity_skill_awaken)
//			{
//				SkillID = (PCClassToInt(InCharacterClass) + 1) * 100 + 3;
//			}
//			else
//			{
//				SkillID = (PCClassToInt(InCharacterClass) + 1) * 100 + 1;
//			}
//			return SkillID;
//		}
//#endif
		if (CharInfo->unity_skill)
		{
			if (CharInfo->aether_offense_set_option_id == b2network::B2AetherSetOptionType::SKILL_GAGE)
			{
				SkillID = (PCClassToInt(InCharacterClass) + 1) * 100 + 2;
			} 
			else if (CharInfo->unity_skill_awaken)
			{
				SkillID = (PCClassToInt(InCharacterClass) + 1) * 100 + 3;
			}
			else
			{
				SkillID = (PCClassToInt(InCharacterClass) + 1) * 100 + 1;
			}
		}
	}

	return SkillID;
}

void FB2ModPlayerInfo::GetUnitySkillMissionArray(FUnitySkillMissionArray& UnitySkillMissionArray, EPCClass InCharacterClass)
{
	if (Characters.IsValidIndex(GetIndex(InCharacterClass)))
	{
		UnitySkillMissionArray = Characters[GetIndex(InCharacterClass)].UnityInfos;
	}
}

TArray<b2network::B2GuildSkillPtr>& FB2ModPlayerInfo::GetGuildSkillInfos()
{
	return GuildSkillInfos;
}

const TArray<int32>& FB2ModPlayerInfo::GetPreviewCostumeItems() const
{
	static TArray<int32>tmp;
	return tmp;
}

bool FB2ModPlayerInfo::IsSkillRegistred(EPCClass InCharacterClass, int32 SkillId) const
{
	auto CharInfo = Characters[GetIndex(InCharacterClass)].CharInfo;
	// TODO skill quick slot

	EItemPresetSlotType SlotType = BladeIIGameImpl::GetLocalCharacterData().GetPCClassSkillSlotID(InCharacterClass);
	int32 SkillSlotID = static_cast<int32>(SlotType);

	if (CharInfo && CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID))
	{
		auto SkillInfo = CharInfo->skill_quick_slots[SkillSlotID];
		if (SkillInfo)
		{
			return SkillInfo->using_skill_id_1 == SkillId || SkillInfo->using_skill_id_2 == SkillId || SkillInfo->using_skill_id_3 == SkillId;
		}
	}
	
	return false;
}

void FB2ModPlayerInfo::GetCharacterUnregistredSkills(EPCClass InCharacterClass, TArray<int32>& UnregistredSkills) const
{
	UnregistredSkills.Empty();

	auto CharInfo = Characters[GetIndex(InCharacterClass)].CharInfo;
	// TODO skill quick slot

	EItemPresetSlotType SlotType = BladeIIGameImpl::GetLocalCharacterData().GetPCClassSkillSlotID(InCharacterClass);
	int32 SkillSlotID = static_cast<int32>(SlotType);

	if (CharInfo && CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID))
	{
		auto SkillInfo = CharInfo->skill_quick_slots[SkillSlotID];
		//if (SkillInfo)
		//{
		//	for (auto skill : CharInfo->skills)
		//	{
		//		auto* SkillMsterInfo = BladeIIGameImpl::GetClientDataStore().GetSkillMasterData(skill->skill_id);
		//		if (SkillMsterInfo && SkillMsterInfo->bActiveSkill && CharInfo->level >= SkillMsterInfo->LearnLevel)
		//		{
		//			if (skill->skill_id != SkillInfo->using_skill_id_1 && skill->skill_id != SkillInfo->using_skill_id_2 && skill->skill_id != SkillInfo->using_skill_id_3)
		//				UnregistredSkills.Add(skill->skill_id);
		//		}
		//	}
		//}
	}	
}

void FB2ModPlayerInfo::GetCharacterSkills(EPCClass InCharacterClass, TArray<FB2SkillInfo>& Skills) const
{
	Skills.Empty();

	auto CharInfo = Characters[GetIndex(InCharacterClass)].CharInfo;
	// TODO skill quick slot
	EItemPresetSlotType SlotType = BladeIIGameImpl::GetLocalCharacterData().GetPCClassSkillSlotID(InCharacterClass);
	int32 SkillSlotID = static_cast<int32>(SlotType);
	if (CharInfo && CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID))
	{
		auto SkillInfo = CharInfo->skill_quick_slots[SkillSlotID];
		if (SkillInfo)
		{
			for (auto Skill : CharInfo->skills)
				Skills.Add(*Skill);
		}
	}	
}
void FB2ModPlayerInfo::GetCharacterSkills(EPCClass InCharacterClass, TArray<int32>& OutSkillIDs) const
{
	OutSkillIDs.Empty();

	auto CharInfo = Characters[GetIndex(InCharacterClass)].CharInfo;
	// TODO skill quick slot
	EItemPresetSlotType SlotType = BladeIIGameImpl::GetLocalCharacterData().GetPCClassSkillSlotID(InCharacterClass);
	int32 SkillSlotID = static_cast<int32>(SlotType);

	if (CharInfo && CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID))
	{
		auto SkillInfo = CharInfo->skill_quick_slots[SkillSlotID];
		if (SkillInfo)
		{
			for (auto Skill : CharInfo->skills)
				OutSkillIDs.Add(Skill->skill_id); // ID 만
		}
	}	
}

void FB2ModPlayerInfo::GetCharacterPassiveSkills(EPCClass InCharacterClass, TArray<int32>& SkillIds) const
{
	SkillIds.Empty();

	if (InCharacterClass == EPCClass::EPC_End)
		return;

	auto CharInfo = Characters[GetIndex(InCharacterClass)].CharInfo;
	// TODO skill quick slot
	EItemPresetSlotType SlotType = BladeIIGameImpl::GetLocalCharacterData().GetPCClassSkillSlotID(InCharacterClass);
	int32 SkillSlotID = static_cast<int32>(SlotType);

	if (CharInfo && CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID))
	{
		//auto SkillInfo = CharInfo->skill_quick_slots[SkillSlotID];
		//if (SkillInfo)
		//{
		//	for (auto Skill : CharInfo->skills)
		//	{
		//		auto* SkillMD = BladeIIGameImpl::GetClientDataStore().GetSkillMasterData(Skill->skill_id);
		//		if (SkillMD && !SkillMD->bActiveSkill)
		//		{
		//			SkillIds.Add(Skill->skill_id);
		//		}
		//	}
		//}
	}	
}

int32 FB2ModPlayerInfo::GetCharacterSkillLevel(int32 SkillId) const
{
	//for (auto CharInfo : Characters)
	//{
	//	if (CharInfo.CharInfo == nullptr)
	//		continue;

	//	// TODO skill quick slot
	//	EPCClass InCharacterClass = SvrToCliPCClassType(CharInfo.CharInfo->character_type);

	//	EItemPresetSlotType SlotType = BladeIIGameImpl::GetLocalCharacterData().GetPCClassSkillSlotID(InCharacterClass);
	//	int32 SkillSlotID = static_cast<int32>(SlotType);

	//	if (CharInfo.CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID) == false)
	//		continue;

	//	auto SkillInfo = CharInfo.CharInfo->skill_quick_slots[SkillSlotID];
	//	if (SkillInfo)
	//	{
	//		for (auto Skill : CharInfo.CharInfo->skills)
	//		{
	//			if (Skill->skill_id == SkillId)
	//			{
	//				if (CharInfo.CharInfo->level < BladeIIGameImpl::GetClientDataStore().GetSkillLearnLevel(SkillId))
	//					return -1;

	//				return Skill->skill_level;
	//			}
	//		}
	//	}
	//}

	return -1;
}

void FB2ModPlayerInfo::GetCharacterSkillOptionsOfID(int32 InSkillId, TArray<FSkillOptionData>& OutFoundOptionData) const
{
	// 특정 스킬 ID 와 현재 해당 스킬 레벨에서의 스킬옵션들
	//const FSkillMasterData* SkillMDOfID = BladeIIGameImpl::GetClientDataStore().GetSkillMasterData(InSkillId);
	//if (SkillMDOfID)
	//{
	//	const int32 SKillLvOfID = GetCharacterSkillLevel(InSkillId);
	//	SkillMDOfID->LevelOptionData.MultiFind(SKillLvOfID, OutFoundOptionData);
	//}
}

int32 FB2ModPlayerInfo::GetCharacterExp(EPCClass InClass)
{
	return Characters[GetIndex(InClass)].CharInfo->exp;
}

int32 FB2ModPlayerInfo::GetCharacterExpMax(EPCClass InClass)
{
	return Characters[GetIndex(InClass)].CharInfo->exp_max;
}

float FB2ModPlayerInfo::GetCharacterExpPercent(EPCClass InClass)
{
	float ResultExpPercent = 0;

	if (b2network::B2CharacterServerInfoPtr pCharInfo = Characters[GetIndex(InClass)].CharInfo)
	{
		if (pCharInfo->exp_max > 0)
		{
			ResultExpPercent = (pCharInfo->exp * 100.0f) / pCharInfo->exp_max;
		}
	}

	return ResultExpPercent;
}

void FB2ModPlayerInfo::SetAccountId(int64 id)
{
	AccountId = id;
}

int64 FB2ModPlayerInfo::GetAccountId()
{
	return AccountId;
}

int32 FB2ModPlayerInfo::GetEtherOffenseSetID(EPCClass InPCClass)
{
	if (InPCClass == EPCClass::EPC_End)
		return 0;

	b2network::B2CharacterServerInfoPtr CharInfo = Characters[GetIndex(InPCClass)].CharInfo;

	return CharInfo->aether_offense_set_option_id;
}

int32 FB2ModPlayerInfo::GetEtherDefenceSetID(EPCClass InPCClass)
{
	if (InPCClass == EPCClass::EPC_End)
		return 0;

	b2network::B2CharacterServerInfoPtr CharInfo = Characters[GetIndex(InPCClass)].CharInfo;

	return CharInfo->aether_defense_set_option_id;
}

EPCClass FB2ModPlayerInfo::GetMainPlayerClass() const
{
	return MainPlayerClass;
}

EPCClass FB2ModPlayerInfo::GetSubPlayerClass() const
{
	return SubPlayerClass;
}

void FB2ModPlayerInfo::SetMainPlayerClass(EPCClass InMainClass)
{
	MainPlayerClass = InMainClass;
}

void FB2ModPlayerInfo::SetSubPlayerClass(EPCClass InSubClass)
{
	SubPlayerClass = InSubClass;
}

void FB2ModPlayerInfo::ConstructEquipItems()
{
	for (auto& Character : Characters)
		SetCharacterEquipItems(Character);
}

void FB2ModPlayerInfo::SetCharacterEquipItems(FB2ModCharInfo& CharacterInfo)
{
	const EPCClass CharType = SvrToCliPCClassType(CharacterInfo.CharInfo->character_type);
	TArray<int32> EquipItems;
	GetEquipItemsByPreset(CharType, EquipItems);

	CharacterInfo.EquippedItems.Empty();

	if (EquipItems.Num() > 0)
	{
		for (int32 ItemID : EquipItems)
		{
			for (auto& Item : ServerAccountInfo->equip_items)
			{
				if (ItemID == Item->id)
				{
					FB2Item EquippedItem;
					EquippedItem = Item;

					CharacterInfo.EquippedItems.Add(EquippedItem);
				}
			}
		}
	}
	else
	{
		// 프리셋 정보가 없다면 아이템 인덱스 0 으로 채워줌.
		for(int32 i = 0; i < static_cast<int32>(EItemEquipPlace::EIEP_End) ; i++)
		{
			FB2Item EquippedItem;
			EquippedItem.AllowedPCClass = PCClassToInt(CharType);
			EquippedItem.EquipPlace = static_cast<EItemEquipPlace>(i);
			EquippedItem.ItemRefID = 0;

			CharacterInfo.EquippedItems.Add(EquippedItem);
		}
	}
}

void FB2ModPlayerInfo::ConstructEquipCostumeItems()
{
	for (auto& Character : Characters)
		SetCharacterEquipCostumeItems(Character);
}

void FB2ModPlayerInfo::SetCharacterEquipCostumeItems(FB2ModCharInfo & CharacterInfo)
{
	const EPCClass CharType = SvrToCliPCClassType(CharacterInfo.CharInfo->character_type);
	TArray<int32> EquipItems;
	GetEquipItemsByPreset(CharType, EquipItems);

	CharacterInfo.EquippedCostumeItems.Empty();
	for (auto& CostumeItem : ServerAccountInfo->equip_costumes)
	{
		if (CostumeItem->character_type == CharacterInfo.CharInfo->character_type)
		{
			if(CostumeItem == nullptr)
				continue;

			FB2Item EquippedItem;
			EquippedItem = *CostumeItem;

			CharacterInfo.EquippedCostumeItems.Add(EquippedItem);
		}
	}
}

void FB2ModPlayerInfo::ConstructEquipTotems()
{
	for (auto& Character : Characters)
		SetCharacterEquipTotems(Character);
}

void FB2ModPlayerInfo::SetCharacterEquipTotems(FB2ModCharInfo & CharacterInfo)
{
	const EPCClass CharType = SvrToCliPCClassType(CharacterInfo.CharInfo->character_type);

	CharacterInfo.EquippedTotems.Empty();
	for (b2network::B2TotemServerInfoPtr& Totem : ServerAccountInfo->equip_totems)
	{
		if (Totem->character_type == CharacterInfo.CharInfo->character_type)
		{
			if (Totem == nullptr)
				continue;

			CharacterInfo.EquippedTotems.Add(Totem->id, FB2Totem(Totem));
		}
	}
}

void FB2ModPlayerInfo::GetEquipItemsByPreset(const EPCClass TargetClass, TArray<int32>& EquipItemIDs)
{
	const int32 ClientPresetID = GetClientPresetID(EInventoryPresetMode::EIPM_Equipment, TargetClass);
	const int32 SvrPCClass = CliToSvrPCClassType(TargetClass);

	for (auto& PresetItem : ServerAccountInfo->item_presets)
	{
		if (PresetItem->character_type == SvrPCClass && PresetItem->preset_id == ClientPresetID)
			EquipItemIDs.AddUnique(PresetItem->item_id);
	}
}

int32 FB2ModPlayerInfo::GetClientPresetID(EInventoryPresetMode PresetMode, EPCClass TargetClass) const
{
	const int32 SvrPCClass = CliToSvrPCClassType(TargetClass);
	for (auto& PresetInfo : ServerAccountInfo->character_presets)
	{
		if (PresetInfo && PresetInfo->character_type == SvrPCClass)
		{
			switch (PresetMode)
			{
			case EInventoryPresetMode::EIPM_Equipment	: return PresetInfo->item_preset_num;
			case EInventoryPresetMode::EIPM_Skill		: return PresetInfo->skill_preset_num;
			}
		}
	}

	return 0;
}

bool FB2ModPlayerInfo::DoesCharacterHaveWing(EPCClass InCharacterClass) const
{
	if (InCharacterClass == EPCClass::EPC_End){
		return false;
	}
	int32 CharInfoIndex = GetIndex(InCharacterClass);
	return Characters.IsValidIndex(CharInfoIndex) ? Characters[CharInfoIndex].WingContainer.bHasWing : false;
}
bool FB2ModPlayerInfo::GetCharacterWing(EPCClass InCharacterClass, FB2Wing& OutData) const
{
	if (InCharacterClass == EPCClass::EPC_End){
		return false;
	}
	int32 CharInfoIndex = GetIndex(InCharacterClass);
	if (Characters.IsValidIndex(CharInfoIndex) && Characters[CharInfoIndex].WingContainer.bHasWing)
	{
		OutData = Characters[CharInfoIndex].WingContainer.WingData;
		return true;
	}
	return false;
}

bool FB2ModPlayerInfo::IsShowHeadItem(EPCClass InCharacterClass) const
{
	if (InCharacterClass == EPCClass::EPC_End) {
		return true;
	}

	int32 CharInfoIndex = GetIndex(InCharacterClass);
	if (Characters.IsValidIndex(CharInfoIndex) && Characters[CharInfoIndex].CharInfo)
		return Characters[CharInfoIndex].CharInfo->helmet_visible;

	return true;
}

bool FB2ModPlayerInfo::IsShowCostume(EPCClass InCharacterClass, ECostumeEquipPlace InCostumeEquipPlace) const
{
	if (InCharacterClass == EPCClass::EPC_End) 
		return true;

	int32 CharInfoIndex = GetIndex(InCharacterClass);

	if (Characters.IsValidIndex(CharInfoIndex) && Characters[CharInfoIndex].CharInfo)
	{
		switch (InCostumeEquipPlace)
		{
		case ECostumeEquipPlace::ECEP_Weapon:
			return Characters[CharInfoIndex].CharInfo->costume_weapon_visible;
		case ECostumeEquipPlace::ECEP_Helmet:
			return Characters[CharInfoIndex].CharInfo->costume_hat_visible;
		case ECostumeEquipPlace::ECEP_Body:
			return Characters[CharInfoIndex].CharInfo->costume_armor_visible;
		}
	}

	return true;
}

bool FB2ModPlayerInfo::GetOptionStatusRawValues(EPCClass InPCClass, EItemOption OptionType, CombatStatInfoRawDatas& OutRawValues, TArray<FB2Item>* InItemArray /*= NULL*/)
{
	//const int32 PCI = GetIndex(InPCClass);
	//if (Characters.IsValidIndex(PCI))
	//{
	//	const FB2ModCharInfo& ModCharInfo = Characters[PCI];
	//	// 장착 장비를 따로 지정할 수 있다.
	//	const TArray<FB2Item>& FinalEquipmentInfo = InItemArray ? *InItemArray : ModCharInfo.EquippedItems;

	//	return CombatStatEval::GetOptionStatusRawValues(InPCClass,
	//		OptionType,
	//		FinalEquipmentInfo,
	//		ModCharInfo.WingContainer,
	//		GetAppliedCharBrevetRank(InPCClass),
	//		GetAppliedCharBrevetNodeIndex(InPCClass),
	//		ModCharInfo.RelicInfos,
	//		ModCharInfo.EquippedEthers,
	//		OutRawValues,
	//		FairyStatus,
	//		ModCharInfo.EquippedTotems
	//	);
	//}
	return false;
}

//float FLocalCharacterData::GetOptionStatusValue
//수정시 위에것두 수정
float FB2ModPlayerInfo::GetOptionStatusValue(EPCClass InPCClass, EItemOption OptionType, TArray<FB2Item>* InItemArray/* = NULL*/)
{
	//const int32 PCI = GetIndex(InPCClass);
	//if (Characters.IsValidIndex(PCI))
	//{
	//	const FB2ModCharInfo& ModCharInfo = Characters[PCI];
	//	TArray<FB2Item>	CharEquipmentInfo;
	//	if (InItemArray == nullptr)
	//	{
	//		GetEquippedItems(InPCClass, CharEquipmentInfo);
	//		GetEquippedCostumeItems(InPCClass, CharEquipmentInfo);
	//	}

	//	// 장착 장비를 따로 지정할 수 있다.
	//	const TArray<FB2Item>& FinalEquipmentInfo = InItemArray ? *InItemArray : CharEquipmentInfo;

	//	return CombatStatEval::GetOptionStatusValue(InPCClass,
	//		OptionType,
	//		FinalEquipmentInfo,
	//		ModCharInfo.WingContainer,
	//		GetAppliedCharBrevetRank(InPCClass),
	//		GetAppliedCharBrevetNodeIndex(InPCClass),
	//		ModCharInfo.RelicInfos,
	//		ModCharInfo.EquippedEthers,
	//		FairyStatus,
	//		ModCharInfo.EquippedTotems
	//	);
	//}
	return 0.0f;
}

//float FLocalCharacterData::GetOptionStatusValueByMod
//수정시 위에것두 수정
float FB2ModPlayerInfo::GetOptionStatusValueByMod(EPCClass InPCClass, EB2GameMode ModeType, EItemOption OptionType, TArray<FB2Item>* InItemArray/* = NULL*/)
{
	const int32 PCI = GetIndex(InPCClass);
	//if (Characters.IsValidIndex(PCI))
	//{
	//	const FB2ModCharInfo& ModCharInfo = Characters[PCI];
	//	TArray<FB2Item>	CharEquipmentInfo;
	//	if (InItemArray == nullptr)
	//	{
	//		GetEquippedItems(InPCClass, CharEquipmentInfo);
	//		GetEquippedCostumeItems(InPCClass, CharEquipmentInfo);
	//	}

	//	// 장착 장비를 따로 지정할 수 있다.
	//	const TArray<FB2Item>& FinalEquipmentInfo = InItemArray ? *InItemArray : CharEquipmentInfo;

	//	return CombatStatEval::GetOptionStatusValueByMod(InPCClass,
	//		ModeType,
	//		OptionType,
	//		FinalEquipmentInfo,
	//		ModCharInfo.WingContainer,
	//		GetAppliedCharBrevetRank(InPCClass),
	//		GetAppliedCharBrevetNodeIndex(InPCClass),
	//		ModCharInfo.RelicInfos,
	//		ModCharInfo.EquippedTotems
	//	);
	//}
	return 0.0f;
}

float FB2ModPlayerInfo::GetCombatStatusValue(EPCClass InPCClass, TArray<FB2Item>* InItemArray/* = NULL*/) const
{
	TArray<FB2Item> EquippedItems;
	TArray<FB2Item> EquippedCostumes;
	GetEquippedItems(InPCClass, EquippedItems);
	GetEquippedCostumeItems(InPCClass, EquippedCostumes);
	
	// 장착 장비를 따로 지정할 수 있다. Local 아닌 ModPlayerInfo 에서 그럴 일이 있을지는 모르겠지만
	const TArray<FB2Item>& FinalEquipmentItemInfo = InItemArray ? *InItemArray : EquippedItems;
	const TArray<FB2Item>& FinalEquipmentCostumeInfo = EquippedCostumes;

	const int8 PCI = GetIndex(InPCClass);
	float fSkillCombatPower = 0;

	//if (Characters.IsValidIndex(PCI))
	//{
	//	const FB2ModCharInfo& ModCharInfo = Characters[PCI];

	//	int32 SkillSlotID = GetClientPresetID(EInventoryPresetMode::EIPM_Skill, InPCClass);

	//	if (ModCharInfo.CharInfo && ModCharInfo.CharInfo->skill_quick_slots.IsValidIndex(SkillSlotID))
	//	{
	//		if (ModCharInfo.CharInfo->skill_quick_slots[SkillSlotID] && ModCharInfo.CharInfo->skill_quick_slots[SkillSlotID]->skill_total_power)
	//		{
	//			fSkillCombatPower = ModCharInfo.CharInfo->skill_quick_slots[SkillSlotID]->skill_total_power;
	//		}
	//	}

	//	return fSkillCombatPower + CombatStatEval::GetCombatStatusValue(
	//		InPCClass,
	//		FinalEquipmentItemInfo,
	//		FinalEquipmentCostumeInfo,
	//		ModCharInfo.WingContainer,
	//		GetAppliedCharBrevetRank(InPCClass),
	//		GetAppliedCharBrevetNodeIndex(InPCClass),
	//		ModCharInfo.RelicInfos,
	//		ModCharInfo.UnityInfos,
	//		ModCharInfo.EquippedEthers,
	//		FairyStatus,
	//		ModCharInfo.EquippedTotems
	//	);
	//}
	
	return 0.0f;
}

int32 FB2ModPlayerInfo::GetCharBrevetRank(EPCClass InCharacterClass) const
{	
	int32 CharInfoIndex = GetIndex(InCharacterClass);
	if (Characters.IsValidIndex(CharInfoIndex))
	{
		return Characters[CharInfoIndex].CharInfo->rank;
	}

	return 0;
}

FPCWingContainer* FB2ModPlayerInfo::GetCharacterWingDataPtr(EPCClass InPCClass)
{
	const int8 PCI = GetIndex(InPCClass);

	//return nullptr;
	return Characters.IsValidIndex(PCI) ? &(Characters[PCI].WingContainer) : nullptr;
}

void FB2ModPlayerInfo::GetSetItemValue(EPCClass InPCClass, TMap<int32, int32>& OutSetItemDatas)
{
	const int32 PCI = GetIndex(InPCClass);
	if (Characters.IsValidIndex(PCI) == false)
		return;

	//CombatStatEval::GetSetItemValue(Characters[PCI].EquippedItems, OutSetItemDatas);
}

int32 FB2ModPlayerInfo::GetCharBrevetNodeIndex(EPCClass InClass) const
{
	const int8 PCI = GetIndex(InClass);
	if (Characters.IsValidIndex(PCI))
		return Characters[PCI].CharInfo->rank_node_index;

	return 0;
}

int32 FB2ModPlayerInfo::GetAppliedCharBrevetRank(EPCClass InCharacterClass) const
{
	int32 CurRank = GetCharBrevetRank(InCharacterClass);
	int32 CurNodeIndex = GetCharBrevetNodeIndex(InCharacterClass);

	// 첫 랭크(랭크 1)가 아니고 랭크의 첫노드일 경우 이전 랭크를 반환
	if (CurRank != 0 && CurRank != 1 && CurNodeIndex == 1)
		return CurRank - 1;

	// 일반경우는 현재 랭크가 적용된 랭크
	return CurRank;
}

int32 FB2ModPlayerInfo::GetAppliedCharBrevetNodeIndex(EPCClass InClass) const
{
	int32 CurRank = GetCharBrevetRank(InClass);
	int32 CurNodeIndex = GetCharBrevetNodeIndex(InClass);

	//if (CurRank == 0)
	//	return 0;

	//// 첫 랭크가 아닌 경우 첫번쨰 노드는 이전 랭크의 최대 인덱스가 적용값
	//if (CurRank != 1 && CurNodeIndex == 1)
	//	return BladeIIGameImpl::GetClientDataStore().GetBrevetNodeCount(CurRank);
	//
	//const int8 PCI = GetIndex(InClass);
	//bool MaxRank = false;
	//if (Characters.IsValidIndex(PCI))
	//{
	//	MaxRank = Characters[PCI].CharInfo->rank_node_state;
	//}

	//// 마지막 랭킹에 마지막 노드인덱스면
	//if (BladeIIGameImpl::GetClientDataStore().GetBrevetMaxRank() == CurRank
	//	&& BladeIIGameImpl::GetClientDataStore().GetBrevetNodeCount(CurRank) == CurNodeIndex
	//	&& MaxRank)
	//	return CurNodeIndex;

	// 기존적으로는 현재 노드에서 하나 작은게 적용된 노드
	return FMath::Max(CurNodeIndex - 1, 0);
}

int32 FB2ModPlayerInfo::GetEnhanceSetEffectLevel(EPCClass InPCClass)
{
	//if (Characters.IsValidIndex(GetIndex(InPCClass)) == false)
		return 0;

	//auto& EquipItems = Characters[GetIndex(InPCClass)].EquippedItems;
	//return CombatStatEval::GetEnhanceSetEffectLevel(EquipItems);
}

double FB2ModPlayerInfo::GetEnhanceSetEffectFactor(EPCClass InPCClass)
{
	//if (Characters.IsValidIndex(GetIndex(InPCClass)) == false)
		return 0.0f;

	//auto& EquipItems = Characters[GetIndex(InPCClass)].EquippedItems;
	//return CombatStatEval::GetEnhanceSetEffectFactor(EquipItems);
}
