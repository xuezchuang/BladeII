
#include "B2RedDotManager.h"
//#include "B2UIManager.h"
#include "Event.h"

#include "B2PCClassInfo.h"
//#include "B2UIDocHelper.h"
#include "B2Airport.h"
//#include "RelicManager.h"
//#include "B2UIChatting.h"
#include "B2ClientDataStore.h"
//#include "B2UIAttendanceMain.h"
#include "B2PCClassInfoBox.h"
//#include "B2SkillInfo.h"
//#include "TutorialManager.h"
#include "Retailer.h"
//#include "Global.h"
//#include "B2LobbyInventory.h"
//#include "B2GameInstance.h"
//#include "B2UIUtil.h"
#include "EventSubsystem.h"
//#include "B2BrevetInfo.h"

#include "BladeIIGameImpl.h"

const TCHAR* FLocalNewActRedDotData::SECTION_NAME = TEXT("RedDot");

FLocalNewActRedDotData::FLocalNewActRedDotData(EStageDifficulty InDifficulty, int32 InActID, bool bNew)
{
	this->Difficulty = InDifficulty;
	this->ActID = InActID;
	bNewAct = false;

	CreateKey();
	if (!bNew && GConfig)
	{
		//if (!GConfig->GetBool(SECTION_NAME, *SaveLoadKey, bNewAct, GB2UserSavedStateIni))
		//	bNewAct = false;
	}
	SetNewAct(bNew);
}

FLocalNewActRedDotData::FLocalNewActRedDotData(const TCHAR * Key, const TCHAR * Value)
{
	ParseKey(Key);
	ParseValue(Value);

	CreateKey();
}

void FLocalNewActRedDotData::CreateKey()
{
	FString DifficultyString = GetDifficultyToString(Difficulty);
	SaveLoadKey = DifficultyString + TEXT("-") + FString::FromInt(ActID);
	UniqueKey = GetUniqueKey(Difficulty, ActID);
}

void FLocalNewActRedDotData::ParseKey(const TCHAR * Key)
{
	FString WrapKey(Key);
	FString DiffStr, ActStr;
	WrapKey.Split(TEXT("-"), &DiffStr, &ActStr);
	Difficulty = GetStringToDifficulty(DiffStr);
	ActID = FCString::Atoi(*ActStr);
}

void FLocalNewActRedDotData::ParseValue(const TCHAR * Value)
{
	bNewAct = FCString::ToBool(Value);
}

EStageDifficulty FLocalNewActRedDotData::GetStringToDifficulty(FString& DifficultyStr)
{
	if (DifficultyStr.Equals(FString(TEXT("ESD_Normal")), ESearchCase::IgnoreCase))
		return EStageDifficulty::ESD_Normal;
	else if (DifficultyStr.Equals(FString(TEXT("ESD_Hard")), ESearchCase::IgnoreCase))
		return EStageDifficulty::ESD_Hard;
	else if (DifficultyStr.Equals(FString(TEXT("ESD_VeryHard")), ESearchCase::IgnoreCase))
		return EStageDifficulty::ESD_VeryHard;
	else if (DifficultyStr.Equals(FString(TEXT("ESD_Hell")), ESearchCase::IgnoreCase))
		return EStageDifficulty::ESD_Hell;
	else
		return EStageDifficulty::ESD_None;
}

FString FLocalNewActRedDotData::GetDifficultyToString(EStageDifficulty InDifficulty)
{
	switch (InDifficulty)
	{
	case EStageDifficulty::ESD_Normal:
		return FString(TEXT("ESD_Normal"));
	case EStageDifficulty::ESD_Hard:
		return FString(TEXT("ESD_Hard"));
	case EStageDifficulty::ESD_VeryHard:
		return FString(TEXT("ESD_VeryHard"));
	case EStageDifficulty::ESD_Hell:
		return FString(TEXT("ESD_Hell"));
	default:
		return FString(TEXT("ESD_None"));
	}
}

void FLocalNewActRedDotData::SetNewAct(bool bNew)
{
	if (bNewAct != bNew)
	{
		bNewAct = bNew;
		//if (GConfig)
		//{
		//	if (bNewAct)
		//		GConfig->SetBool(SECTION_NAME, *SaveLoadKey, bNewAct, GB2UserSavedStateIni);
		//	else
		//		GConfig->RemoveKey(SECTION_NAME, *SaveLoadKey, GB2UserSavedStateIni);

		//	GConfig->Flush(false, GB2UserSavedStateIni);
		//}
	}
}

int32 FLocalNewActRedDotData::GetUniqueKey()
{
	return UniqueKey;
}

bool FLocalNewActRedDotData::IsNewAct()
{
	return bNewAct;
}

int32 FLocalNewActRedDotData::GetUniqueKey(EStageDifficulty InDifficulty, int32 InActID)
{
	return static_cast<int32>(InDifficulty) * 1000 + InActID;
}


FB2RedDotManager::FB2RedDotManager()
{
	
}

void FB2RedDotManager::SubscribeEvents()
{
	UnsubscribeEvents();

	//Issues.Add(DeliveryCheckRedDotClass<FB2MessageInfoResponseCheckRedDotPtr>::GetInstance().Subscribe2(
	//	[this](FB2MessageInfoResponseCheckRedDotPtr LobbyRedDotInfo)
	//{
	//	this->ResponseLobbyCheckRedDot(LobbyRedDotInfo);
	//}
	//));
	//Issues.Add(UpdateMarkRedDotClass<const FName&>::GetInstance().Subscribe2(
	//	[this](const FName& UIName)
	//{
	//	this->UpdateMarkRedDot(UIName);
	//}
	//));
	//Issues.Add(DeliveryCheckModeOpenClass<FB2ResponseCheckModeOpenPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseCheckModeOpenPtr& CheckModeOpen)
	//{
	//	this->UpdateCheckModeOpen();
	//}
	//));
	//Issues.Add(ChangeLastClearStageClass<EStageDifficulty, int32, int32>::GetInstance().Subscribe2(
	//	[this](EStageDifficulty Difficulty, int32 PrevStageID, int32 CurrentStageID)
	//{
	//	this->SetChangeStageClear(Difficulty, PrevStageID, CurrentStageID);
	//}
	//));
}

void FB2RedDotManager::UnsubscribeEvents()
{
	Issues.Empty();
}

void FB2RedDotManager::LoadLocalData()
{
	CreateNewActMap();
}

void FB2RedDotManager::CreateNewActMap()
{
	if (GConfig)
	{
		FKeyValueSink sink;
		sink.BindLambda([this](const TCHAR* key, const TCHAR* value) {
			FLocalNewActRedDotData NewActRedDotData(key, value);
			this->CheckNewActs.Add(NewActRedDotData.GetUniqueKey(), NewActRedDotData);
		});
		//GConfig->ForEachEntry(sink, FLocalNewActRedDotData::SECTION_NAME, GB2UserSavedStateIni);
	}
}

bool FB2RedDotManager::HasLobbyRedDot_ConnectReward()
{
	//int32 Index = 0;
	//const auto& RewardInfo = BladeIIGameImpl::GetClientDataStore().GetPlayTimeRewardData();
	//const auto& PlayTimeStatus = BladeIIGameImpl::GetClientDataStore().GetPlayTimeStatus();

	//for (const auto& Data : RewardInfo)
	//{
	//	if (Index < 6 && Index + 1 > PlayTimeStatus.daily_play_time_reward_index)
	//	{
	//		if (Data.play_time_inmin * 60.0f < PlayTimeStatus.daily_play_time_in_sec)
	//		{
	//			return true;
	//		}
	//	}
	//	++Index;
	//}
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_NewUser()
{
	//FTimespan RemainTime = FDateTime::FromUnixTimestamp(BladeIIGameImpl::GetClientDataStore().GetEventNewUserStatus().NextAttendanceTime / 1000) - UB2GameInstance::GetUTCNow();
	//bool EnableReceive = BladeIIGameImpl::GetClientDataStore().GetEventNewUserStatus().TotalAttendanceDays > BladeIIGameImpl::GetClientDataStore().GetEventNewUserStatus().AttendanceDay;
	//return RemainTime.GetTicks() < 0 && EnableReceive;
	return false;
}

bool FB2RedDotManager::HadLobbyRedDot_Attendance()
{
	return HasLobbyRedDot_EventAttendance() || HasLobbyRedDot_ConnectReward() || HasLobbyRedDot_NewUser();
}

bool FB2RedDotManager::HasLobbyRedDot_EventAttendance()
{
	//TArray<FEventAttendanceReward> AttendanceRewards;
	//
	//const auto& RewardStatus = BladeIIGameImpl::GetClientDataStore().GetEventAttendanceStatus();
	//const auto& RemainTime = FDateTime::FromUnixTimestamp(RewardStatus.next_attendance_time / 1000) - UB2GameInstance::GetUTCNow();
	//
	//bool EntirePeriod = BladeIIGameImpl::GetClientDataStore().GetEventAttendanceRewardData(AttendanceRewards, RewardStatus.event_id);
	//bool ReceptiblePeriod = AttendanceRewards.Num() > RewardStatus.attendance_day;
	//auto TimePeriod = RemainTime.GetTicks() < 0.0f;

	//return EntirePeriod && ReceptiblePeriod && TimePeriod;
	return true;
}

void FB2RedDotManager::ResponseLobbyCheckRedDot(FB2MessageInfoResponseCheckRedDotPtr LobbyRedDotInfo)
{
	UpdateRedDotState(LobbyRedDotInfo);
	check(CurrentRedDotState);
	
	//UB2UnitedWidgetBase* LobbyMainUI = UB2UIManager::GetInstance()->GetUI<UB2UnitedWidgetBase>(UIFName::LobbyMain);
	//if (LobbyMainUI)
	//	LobbyMainUI->DoMarkRedDot();

	//UB2UnitedWidgetBase* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UnitedWidgetBase>(UIFName::Header);
	//if (HeaderUI)
	//	HeaderUI->DoMarkRedDot();
}


void FB2RedDotManager::UpdateMarkRedDot(const FName& UIName)
{
	/*if (UB2UnitedWidgetBase* RedDotUI = UB2UIManager::GetInstance()->GetUI<UB2UnitedWidgetBase>(UIName))
	{
		RedDotUI->DoMarkRedDot();
	}*/
}

bool FB2RedDotManager::IsLevelupableSkill(int32 SkillID)
{
	//const EPCClass SkillClass = BladeIIGameImpl::GetClientDataStore().GetSkillClass(SkillID);
	//if (SkillClass != EPCClass::EPC_End)
	//{
	//	const int32 SkillLearnLevel = BladeIIGameImpl::GetClientDataStore().GetSkillLearnLevel(SkillID);
	//	const int32 CharacterLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(SkillClass);

	//	if (CharacterLevel >= SkillLearnLevel)
	//	{
	//		const int32 SkillLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterSkillLevel(SkillID);
	//		const int32 MaxSkillLevel = BladeIIGameImpl::GetClientDataStore().GetMaxSkillLevel(SkillID);

	//		if (SkillLevel >= MaxSkillLevel)
	//			return false;

	//		const bool HasLevelUpGold = HasLevelUpSkillGold(SkillID);
	//		const bool HasSkillLevelUpPoint = HasLevelUpSkillPoint(SkillID, SkillLevel);
	//		return (CharacterLevel >= SkillLevel) && HasLevelUpGold && HasSkillLevelUpPoint;
	//	}
	//}

	return false;
}

bool FB2RedDotManager::HasLevelUpSkillGold(int32 SkillId)
{
	//int32 LevelUpCost = BladeIIGameImpl::GetLocalCharacterData().GetCharacterSkillLevelupGold(SkillId);
	//int32 GoldAmount = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
	//return LevelUpCost <= GoldAmount;
	return false;
}

bool FB2RedDotManager::HasLevelUpSkillPoint(int32 SkillID, int32 SkillLevel)
{
	//return B2UIUtil::HasLevelUpSkillPoint(SkillID, SkillLevel);
	return false;
}

void FB2RedDotManager::UpdateRedDotState(FB2MessageInfoResponseCheckRedDotPtr NewRedDotInfo)
{
	if (CurrentRedDotState.get() == nullptr)
	{
		CurrentRedDotState = NewRedDotInfo;
		return;
	}

	if (NewRedDotInfo.get() == nullptr)
		return;

	if (NewRedDotInfo->is_valid_equip)
	{
		CurrentRedDotState->has_new_equipment = NewRedDotInfo->has_new_equipment;
		CurrentRedDotState->has_new_aether = NewRedDotInfo->has_new_aether;
		CurrentRedDotState->has_new_costume = NewRedDotInfo->has_new_costume;
		CurrentRedDotState->has_new_totem = NewRedDotInfo->has_new_totem;
	}
	if (NewRedDotInfo->is_valid_collection)
	{
		CurrentRedDotState->has_new_collection = NewRedDotInfo->has_new_collection;
	}
	if (NewRedDotInfo->is_valid_mission)
	{
		CurrentRedDotState->has_mission_to_reward = NewRedDotInfo->has_mission_to_reward;
		CurrentRedDotState->has_mission_mileage_reward = NewRedDotInfo->has_mission_mileage_reward;
		CurrentRedDotState->has_serial_mission_to_reward = NewRedDotInfo->has_serial_mission_to_reward;
	}
	if (NewRedDotInfo->is_valid_friend)
	{
		CurrentRedDotState->has_new_friend = NewRedDotInfo->has_new_friend;
		CurrentRedDotState->has_friend_request = NewRedDotInfo->has_friend_request;
	}
	if (NewRedDotInfo->is_valid_adventure)
	{
		CurrentRedDotState->has_adventure_mileage_reward = NewRedDotInfo->has_adventure_mileage_reward;
	}
	if (NewRedDotInfo->is_valid_dungeon)
	{
		CurrentRedDotState->counter_dungeon_weekly_clear_count = NewRedDotInfo->counter_dungeon_weekly_clear_count;
		CurrentRedDotState->counter_dungeon_weekly_reward_received = NewRedDotInfo->counter_dungeon_weekly_reward_received;
		CurrentRedDotState->tickets_for_counter_dungeon = NewRedDotInfo->tickets_for_counter_dungeon;
		CurrentRedDotState->tickets_for_hero_tower = NewRedDotInfo->tickets_for_hero_tower;
		CurrentRedDotState->used_tickets_for_hero_tower = NewRedDotInfo->used_tickets_for_hero_tower;
		CurrentRedDotState->dimension_daily_clear_count = NewRedDotInfo->dimension_daily_clear_count;

	}
	if (NewRedDotInfo->is_valid_pvp)
	{
		CurrentRedDotState->has_one_vs_one_reward = NewRedDotInfo->has_one_vs_one_reward;
		CurrentRedDotState->has_team_battle_reward = NewRedDotInfo->has_team_battle_reward;
		CurrentRedDotState->has_team_assault_reward = NewRedDotInfo->has_team_assault_reward;
		CurrentRedDotState->team_assault_mvp_point = NewRedDotInfo->team_assault_mvp_point;
	}
	if (NewRedDotInfo->is_valid_shop)
	{
		CurrentRedDotState->remaining_free_item_time = NewRedDotInfo->remaining_free_item_time;
		CurrentRedDotState->has_shop_item_mileage_reward = NewRedDotInfo->has_shop_item_mileage_reward;
		CurrentRedDotState->has_shop_cash_mileage_reward = NewRedDotInfo->has_shop_cash_mileage_reward;
	}
	if (NewRedDotInfo->is_valid_mail)
	{
		CurrentRedDotState->unread_mail_count = NewRedDotInfo->unread_mail_count;
		CurrentRedDotState->has_present_mail = NewRedDotInfo->has_present_mail;
		CurrentRedDotState->has_blade_point_mail = NewRedDotInfo->has_blade_point_mail;
		CurrentRedDotState->has_social_point_mail = NewRedDotInfo->has_social_point_mail;
		CurrentRedDotState->has_lottery_mail = NewRedDotInfo->has_lottery_mail;
		//UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::Header);
		//UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::Mail);
	}
	if (NewRedDotInfo->is_valid_event)
	{
		CurrentRedDotState->has_event_level_up_reward = NewRedDotInfo->has_event_level_up_reward;
		CurrentRedDotState->has_event_point_reward = NewRedDotInfo->has_event_point_reward;
		CurrentRedDotState->has_event_season_mission_reward = NewRedDotInfo->has_event_season_mission_reward;
		CurrentRedDotState->has_event_stage_clear_reward = NewRedDotInfo->has_event_stage_clear_reward;
		CurrentRedDotState->has_event_point_shop_reward = NewRedDotInfo->has_event_point_shop_reward;
		CurrentRedDotState->has_event_gem_purchase_reward = NewRedDotInfo->has_event_gem_purchase_reward;
		CurrentRedDotState->has_event_ten_lottery_reward = NewRedDotInfo->has_event_ten_lottery_reward;
		CurrentRedDotState->has_event_spend_gem_reward = NewRedDotInfo->has_event_spend_gem_reward;
		CurrentRedDotState->has_event_ten_lottery_savings_reward = NewRedDotInfo->has_event_ten_lottery_savings_reward;
		CurrentRedDotState->has_event_dice_reward = NewRedDotInfo->has_event_dice_reward;

		//UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::EventMain);
		//UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::SeasonEvent);
	}
	if (NewRedDotInfo->is_valid_etc)
	{
		CurrentRedDotState->has_item_random_option_change = NewRedDotInfo->has_item_random_option_change;
	}
	if (NewRedDotInfo->is_valid_shop_package)
	{
		CurrentRedDotState->has_shop_package_flat_rate_reward = NewRedDotInfo->has_shop_package_flat_rate_reward;
		CurrentRedDotState->has_shop_package_level_up_reward = NewRedDotInfo->has_shop_package_level_up_reward;
		CurrentRedDotState->has_shop_package_reach_level_reward = NewRedDotInfo->has_shop_package_reach_level_reward;
	}

	if (NewRedDotInfo->is_valid_magic_shop)
	{
		CurrentRedDotState->has_magic_shop_free_renewal = NewRedDotInfo->has_magic_shop_free_renewal;
	}

	if (NewRedDotInfo->is_valid_hall_of_fame)
	{
		CurrentRedDotState->has_hall_of_fame_praise_count = NewRedDotInfo->has_hall_of_fame_praise_count;
	}

	if (NewRedDotInfo->is_valid_guild)
		CurrentRedDotState->has_guild_battle_reward_box = NewRedDotInfo->has_guild_battle_reward_box;
	if (NewRedDotInfo->is_valid_not_apply_bless_fairy)
		CurrentRedDotState->has_not_apply_bless_fairy = NewRedDotInfo->has_not_apply_bless_fairy;

	if (NewRedDotInfo->is_valid_donation)
	{
		CurrentRedDotState->has_donation_point_reward = NewRedDotInfo->has_donation_point_reward;
		CurrentRedDotState->has_usable_free_donation_count = NewRedDotInfo->has_usable_free_donation_count;
	}
}

void FB2RedDotManager::UpdateCheckModeOpen()
{
	//UpdateMarkRedDotClass<const FName&>::GetInstance().Signal(UIFName::LobbyMain);
}

void FB2RedDotManager::SetChangeStageClear(EStageDifficulty Difficulty, int32 CurrentStageID, int32 NextStageID)
{
	//int32 NextActorNum = BladeIIGameImpl::GetStageDataStore().GetActIdByClientStageId(NextStageID);
	//int32 NextSubChapterNum = BladeIIGameImpl::GetStageDataStore().GetSubChapterNumByClientStageId(NextStageID);
	//int32 LastSubChapterNum = BladeIIGameImpl::GetStageDataStore().GetLastSubChapterNum(NextActorNum);
	//if (NextSubChapterNum == LastSubChapterNum)
	//{
	//	NextActorNum += 1;
	//	int32 CheckKey = FLocalNewActRedDotData::GetUniqueKey(Difficulty, NextActorNum);
	//	if (CheckNewActs.Contains(CheckKey))
	//	{
	//		CheckNewActs[CheckKey].SetNewAct(true);
	//	}
	//	else
	//	{
	//		FLocalNewActRedDotData NewActRedDotData(Difficulty, NextActorNum, true);
	//		CheckNewActs.Add(CheckKey, NewActRedDotData);
	//	}
	//}
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt()
{
	return HasLobbyRedDot_HeroMgmt_Skill() ||
		HasLobbyRedDot_HeroMgmt_Brevet() ||
		HasLobbyRedDot_HeroMgmt_Wing() ||
		HasLobbyRedDot_HeroMgmt_Relic() ||
		HasUnappliedItemRandomOption() ||
		HasLobbyRedDot_HeroMgmt_Ether();
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Inventory()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_Store) == false)
		return false;

	//return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_new_equipment : false;
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Inventory(EPCClass PCClass)
{
	for (int32 i = int32(EItemInvenType::EIIVT_Weapon); i <= int32(EItemInvenType::EIIVT_Consumables); ++i)
	{
		if (HasLobbyRedDot_HeroMgmt_Inventory(PCClass, static_cast<EItemInvenType>(i)))
			return true;
	}
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Inventory(EPCClass PCClass, EItemInvenType TapType)
{
	TArray<FB2Item> CurrPCItemList;
	//UB2LobbyInventory::GetStoredItemList(CurrPCItemList, TapType, PCClass, false);

	return CurrPCItemList.ContainsByPredicate([](FB2Item& Item) { return Item.IsNew; });
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Skill()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_PlayerSkill) == false)
	//	return false;

	for (int32 PCIndex = 0; PCIndex < GetMaxPCClassNum(); PCIndex++)
	{
		if (HasLobbyRedDot_HeroMgmt_Skill(IntToPCClass(PCIndex)))
			return true;
	}

	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Skill(EPCClass PCClass)
{
	return (HasLobbyRedDot_HeroMgmt_PointSkill(PCClass) 
			|| HasLobbyReddot_HeroMgmt_UnitySkill(PCClass)
			);
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_PointSkill(EPCClass PCClass)
{
	return HasLobbyRedDot_HeroMgmt_PointSkill(PCClass, true) || HasLobbyRedDot_HeroMgmt_PointSkill(PCClass, false);
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_PointSkill(EPCClass PCClass, bool bActiveSkill)
{
	//const int32 LeftSkillPoint = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLeftSkillPoint(PCClass);

	//if (LeftSkillPoint <= 0)
	//	return false;

	//if (PCClass != EPCClass::EPC_End)
	//{
	//	//if (!cacheB2SkillInfo.IsValid())
	//	//{
	//	//	auto* ClassInfoBox = StaticFindPCClassInfoBox();
	//	//	cacheB2SkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;
	//	//}

	//	if (cacheB2SkillInfo.IsValid())
	//	{
	//		TArray<FB2SkillInfo> CharacterSkills;
	//		BladeIIGameImpl::GetLocalCharacterData().GetCharacterSkills(PCClass, CharacterSkills);

	//		for (auto& SkillInfo : CharacterSkills)
	//		{
	//			//const FSingleSkillInfo* SingleSkillInfo = cacheB2SkillInfo->GetSingleInfoOfID(SkillInfo.skill_id);
	//			//if (SingleSkillInfo != nullptr)
	//			//{
	//			//	bool bTargetSkill = (bActiveSkill && SingleSkillInfo->PassiveType == EPassiveType::EPassiveType_None) ||
	//			//		(!bActiveSkill && SingleSkillInfo->PassiveType != EPassiveType::EPassiveType_None);

	//			//	if (bTargetSkill && IsLevelupableSkill(SkillInfo.skill_id))
	//			//		return true;
	//			//}
	//		}
	//	}
	//}

	return false;
}

bool FB2RedDotManager::HasLobbyReddot_HeroMgmt_UnitySkill(EPCClass PCClass)
{
	for (int32 SubPCIndex = 0; SubPCIndex < GetMaxPCClassNum(); SubPCIndex++)
	{
		if (HasLobbyReddot_HeroMgmt_UnitySkill(PCClass, IntToPCClass(SubPCIndex)))
			return true;
	}
	
	if (HasLobbyReddot_HeroMgmt_UnitySkillAwakenMission(PCClass) || HasLobbyReddot_HeroMgmt_UnitySkillAwakenAble(PCClass))
		return true;

	return false;
}

bool FB2RedDotManager::HasLobbyReddot_HeroMgmt_UnitySkill(EPCClass PCClass, EPCClass SubPCClass)
{
	//return  GLOBALUNITYSKILLMANAGER.CheckStateUnitySkillMission(PCClass,SubPCClass);
	return false;
}

bool FB2RedDotManager::HasLobbyReddot_HeroMgmt_UnitySkillAwakenAble(EPCClass PCClass)
{
	// 각성 가능
	//FLocalCharacterData& CharData = BladeIIGameImpl::GetLocalCharacterData();
	//if (CharData.GetIsUnityAwaken(PCClass))
	//	return false;

	//FUnitySkillState* UnityState = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillState(PCClass);
	//if (UnityState && UnityState->AllAwakenComplete)
	//	return true;

	return false;
}

bool FB2RedDotManager::HasLobbyReddot_HeroMgmt_UnitySkillAwakenMission(EPCClass PCClass)
{
	// 각성 미션 가능
	bool Result = false;
	//FLocalCharacterData& CharData = BladeIIGameImpl::GetLocalCharacterData();
	//if (CharData.GetIsUnityAwaken(PCClass))
	//	return false;

	//FUnitySkillState* UnityState = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillState(PCClass);
	//if (UnityState && !UnityState->AllMissionComplete)
	//	return false;

	//FUnitySkillAwakenMissionArray* Missions = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillAwakenMission(PCClass);
	//for (auto MissionItem : *Missions)
	//{
	//	FB2UnitySkillAwakenMissionPtr MissionInfo = GLOBALUNITYSKILLMANAGER.GetUnitySkillAwakenMission(PCClass, MissionItem.nMissionID);
	//	int32 HaveCount = 0;
	//	int32 NeedCount = 0;

	//	if (MissionItem.bCompleted)
	//		continue;

	//	// 필요재화가 골드임
	//	if (MissionInfo->req_gold > 0)
	//	{
	//		HaveCount = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
	//		NeedCount = MissionInfo->req_gold;
	//	}
	//	else
	//	{
	//		HaveCount = UB2LobbyInventory::GetSharedConsumableAmountOfType(MissionInfo->req_item_id_1);
	//		NeedCount = MissionInfo->req_item_count_1;
	//	}

	//	if (HaveCount >= NeedCount)
	//	{
	//		Result = true;
	//		break;
	//	}
	//}


	return Result;
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Brevet()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_PlayerPromote) == false)
	//	return false;

	for (int32 PCIndex = 0; PCIndex < GetMaxPCClassNum(); PCIndex++)
	{
		if (HasLobbyRedDot_HeroMgmt_Brevet(IntToPCClass(PCIndex)))
			return true;
	}

	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Brevet(EPCClass PCClass)
{
	//FLocalCharacterData& CharData = BladeIIGameImpl::GetLocalCharacterData();
	//FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	//
	//const int32 BrevetRank = CharData.GetCharBrevetRank(PCClass);
	//const int32 BrevetNodeIndex = CharData.GetCharBrevetNodeIndex(PCClass);

	//if (const FBrevetNodeInfo* BrevetInfo = ClientDataStore.GetBrevetNodeInfo(BrevetRank, BrevetNodeIndex))
	//{
	//	//진급 최고레벨 도달
	//	int32 BrevetNodeMaxCount = BladeIIGameImpl::GetClientDataStore().GetBrevetNodeCount(BrevetRank);
	//	if (StaticFindBrevetInfo()->GetMaxBrevetRank() <= BrevetRank && BrevetNodeMaxCount <= BrevetNodeIndex)
	//		return false;
	//	// 일단 돈 없으면 나가리
	//	const int32 CurrentGold = ClientDataStore.GetGoldAmount();
	//	if (CurrentGold >= BrevetInfo->NeedActiveGold)
	//	{
	//		// Level Check && ProtmotionPoint Check
	//		const int32 PCLevel = CharData.GetCharacterLevel(PCClass);
	//		int32 PromotionPoint;

	//		bool checkNormal = (BrevetRank <= ClientDataStore.GetMaximumNormalBrevetGade());

	//		if (checkNormal)	PromotionPoint = ClientDataStore.GetRankPromotionPoint();
	//		else				PromotionPoint = ClientDataStore.GetAdvancedRankPromotionPoint();
	//		
	//		if (PCLevel >= BrevetInfo->OpenLevel && PromotionPoint >= BrevetInfo->NeedRankPromotionPoint)
	//		{
	//			return true;
	//		}
	//	}
	//}

	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Wing()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_Wing) == false)
	//	return false;

	//for (int32 PCIndex = 0; PCIndex < GetMaxPCClassNum(); PCIndex++)
	//{
	//	if (HasLobbyRedDot_HeroMgmt_Wing(IntToPCClass(PCIndex)))
	//		return true;
	//}

	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Wing(EPCClass PCClass)
{
	return UB2Airport::HasAnyNewStuff_Char(PCClass);
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Relic()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_HeroTower) == false)
	//	return false;

	//for (int32 PCIndex = 0; PCIndex < GetMaxPCClassNum(); PCIndex++)
	//{
	//	if (HasLobbyRedDot_HeroMgmt_Relic(IntToPCClass(PCIndex)))
	//		return true;
	//}
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Relic(EPCClass PCClass)
{
	//bool bIsRedot = GLOBALRELICMANAGER.GetIsReddotRelicByClass(static_cast<int32>(PCClass));
	//return bIsRedot;
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Relic(EPCClass PCClass, int32 RelicID)
{
	//bool bIsRedot = GLOBALRELICMANAGER.GetIsReddotByRelicId(PCClass, RelicID);
	//return bIsRedot;
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Ether()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_Ether) == false)
		return false;

	//return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_new_aether : false;
}

bool FB2RedDotManager::HasLobbyRedDot_HeroMgmt_Totem()
{
	// Totem HeroMgmt RedDot is Always False.
	return false;
}

void FB2RedDotManager::MarkAct(EStageDifficulty StageDifficulty, int32 ActNum)
{
	int32 UniqueKey = FLocalNewActRedDotData::GetUniqueKey(StageDifficulty, ActNum);
	if (CheckNewActs.Contains(UniqueKey))
		CheckNewActs[UniqueKey].SetNewAct(false);
}

bool FB2RedDotManager::HasLobbyRedDot_NewAct(EStageDifficulty StageDifficulty, int32 ActNum)
{
	int32 UniqueKey = FLocalNewActRedDotData::GetUniqueKey(StageDifficulty, ActNum);
	if (CheckNewActs.Contains(UniqueKey))
		return CheckNewActs[UniqueKey].IsNewAct();
	
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_AllNewAct()
{
	for (auto& Element : CheckNewActs)
	{
		if (Element.Value.IsNewAct())
			return true;
	}
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Adventure()
{
	bool HasReward = CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_adventure_mileage_reward : false;
	return HasReward || HasLobbyRedDot_AllNewAct();
}

bool FB2RedDotManager::HasLobbyRedDot_Dungeon()
{
	//int32 CheckMinTutorialID = FMath::Min3<int32>(TutorialID_HeroTower, TutorialID_Raid, TutorialID_CounterDungeon);

	//if (TutorialLockRedDotVisibleCheck(CheckMinTutorialID) == false)
	//	return false;

	return HasLobbyRedDot_Dungeon_Counter() ||
		HasLobbyRedDot_Dungeon_Hero() ||
		HasLobbyRedDot_Dungeon_Raid() ||
		HasLobbyRedDot_Dungeon_Dimension();
}

bool FB2RedDotManager::HasLobbyRedDot_Dungeon_Counter()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_CounterDungeon) == false)
	//	return false;

	// 계정 생성시 -1로 (RequestGetCounterDungeon) 요청 전까지 들어오며 라이브 서비스 시 바로 반격던전이 오픈되지 않을 것 같으니 이는 Tutorial이랑 연계 가능성을 생각해야함
	const int32 TicketCount = CurrentRedDotState.get() != nullptr ? CurrentRedDotState->tickets_for_counter_dungeon : 0;
	bool TicketResult = TicketCount != INDEX_NONE ? TicketCount > 0 : true;
	
	bool RewardResult = false;
	if (CurrentRedDotState.get() != nullptr)
	{
		RewardResult = IsAvailableCounterDungeonReward(CurrentRedDotState->counter_dungeon_weekly_clear_count, 
														CurrentRedDotState->counter_dungeon_weekly_reward_received);
	}

	return TicketResult || RewardResult;
}

bool FB2RedDotManager::HasLobbyRedDot_Dungeon_Hero()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_HeroTower) == false)
	//	return false;

	const int32 CurrentTicket = CurrentRedDotState.get() != nullptr ? CurrentRedDotState->tickets_for_hero_tower: 0;

	return CurrentTicket > 0;
}

bool FB2RedDotManager::HasLobbyRedDot_Dungeon_Raid()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_Raid) == false)
	//	return false;

	//auto DocSome = UB2UIDocHelper::GetDocSome();

	//if (DocSome)
	//	return DocSome->GetIsOpenRaid();

	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Dungeon_Dimension()
{
	//UB2UIDocDimension * pDocDimension = UB2UIDocHelper::GetDocDimension();
	//if (!pDocDimension)
	//	return false;
	//if (!CurrentRedDotState.get())
	//	return false;

	//const int32 CurrentTicket = pDocDimension->GetDailyTryCount() - CurrentRedDotState->dimension_daily_clear_count;
	//
	//return CurrentTicket > 0;
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Battle()
{
	//int32 CheckMinTutorialID = FMath::Min3<int32>(TutorialID_PvPOneAndOne, TutorialID_PvPTeam, TutorialID_Occupy);

	//if (TutorialLockRedDotVisibleCheck(CheckMinTutorialID) == false)
	//	return false;

	return HasLobbyRedDot_Battle_PvP() ||
		HasLobbyRedDot_Battle_Team() ||
		HasLobbyRedDot_Battle_Control();
}

bool FB2RedDotManager::HasLobbyRedDot_Battle_PvP()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_PvPOneAndOne) == false)
	//	return false;

	bool IsAbleToPvp = false;
	bool HasReward = false;

	//auto DocPvp = UB2UIDocHelper::GetDocPVP1on1Rival();
	//if (DocPvp)
	//	IsAbleToPvp = (DocPvp->GetMatchPoint() > 0);

	//if (CurrentRedDotState.get() != nullptr)
	//	HasReward = CurrentRedDotState->has_one_vs_one_reward;

	return (HasReward || IsAbleToPvp);
}

bool FB2RedDotManager::HasLobbyRedDot_Battle_Team()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_PvPTeam) == false)
	//	return false;

	bool IsAbleToTeam = false;
	bool HasReward = false;

	//auto DocTeam = UB2UIDocHelper::GetDocTeamMatch();
	//if (DocTeam)
	//	IsAbleToTeam = (DocTeam->GetTeamMatchPoint() > 0);

	//if (CurrentRedDotState.get() != nullptr)
	//	HasReward =  CurrentRedDotState->has_team_battle_reward;

	return (HasReward || IsAbleToTeam);
}

bool FB2RedDotManager::HasLobbyRedDot_Battle_Control()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_Occupy) == false)
	//	return false;
	
	bool HasAssaultReward = false;
	bool HasMvpReward = false;
	bool OpenAssault = false;

	//if (CurrentRedDotState.get() != nullptr)
	//{
	//	HasAssaultReward = CurrentRedDotState->has_team_assault_reward;

	//	int32 MVPPoint = CurrentRedDotState->team_assault_mvp_point;
	//	int32 MaxPoint = BladeIIGameImpl::GetClientDataStore().GetAssaultMVPMaxCount();
	//	HasMvpReward = MVPPoint >= MaxPoint;
	//}

	//auto DocSome = UB2UIDocHelper::GetDocSome();
	//if (DocSome)
	//	OpenAssault = DocSome->GetIsOpenAssault();

	return OpenAssault || HasAssaultReward || HasMvpReward;
}

bool FB2RedDotManager::HasLobbyRedDot_CollectBook()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_new_collection : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Mission()
{
	if (CurrentRedDotState.get() != nullptr)
	{
		bool bHasMissionReward = CurrentRedDotState->has_mission_to_reward || CurrentRedDotState->has_serial_mission_to_reward;
		bool bHasMissionMilageReward = CurrentRedDotState->has_mission_mileage_reward;

		return (bHasMissionReward || bHasMissionMilageReward) ? true : false;
	}

	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Guild()
{
	if (CurrentRedDotState.get() != nullptr)
	{
		if (CurrentRedDotState->is_valid_guild && CurrentRedDotState->has_guild_battle_reward_box)
			return true;
	}

	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Shop()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_Store) == false)
	//	return false;

	return HasLobbyRedDot_Mileage() || HasLobbyRedDot_FreeItem();
}

bool FB2RedDotManager::HasLobbyRedDot_Mileage_Cash()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_shop_cash_mileage_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Mileage_Item()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_shop_item_mileage_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Mileage()
{
	return HasLobbyRedDot_Mileage_Cash() || HasLobbyRedDot_Mileage_Item();
}

bool FB2RedDotManager::HasLobbyRedDot_FreeItem()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_Store) == false)
	//	return false;

	int64 ServerFreeItemTime = CurrentRedDotState.get() != nullptr ? CurrentRedDotState->remaining_free_item_time : 1;
	bool bHasNewShopFreeTicket = ServerFreeItemTime <= 0 ? true : false;

	return bHasNewShopFreeTicket;
}

bool FB2RedDotManager::HasLobbyRedDot_Friend()
{
	return (HasLobbyRedDot_Friend_New() || HasLobbyRedDot_Friend_Request() || HasLobbyRedDot_Friend_FriendPoint());
}

bool FB2RedDotManager::HasLobbyRedDot_Friend_New()
{
	if (CurrentRedDotState.get() != nullptr)
		return CurrentRedDotState->has_new_friend;

	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Friend_Request()
{
	//if (CurrentRedDotState.get() != nullptr)
	//{
	//	auto DocFriend = UB2UIDocHelper::GetDocFriend();
	//	if (DocFriend && BladeIIGameImpl::GetClientDataStore().GetMaxFriendCount() <= DocFriend->GetFriendList().Num())
	//		return false;

	//	return CurrentRedDotState->has_friend_request;
	//}
	//	

	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Friend_FriendPoint()
{
	//auto DocFriend = UB2UIDocHelper::GetDocFriend();
	//if (DocFriend)
	//	return DocFriend->GetAbleToSendFriendPoint();

	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Mail()
{
	// 해당 mail count로 추후 Header에 Unread Mail Count 표시 해줘야 함
	if (CurrentRedDotState.get() != nullptr)
	{
		return CurrentRedDotState->unread_mail_count > 0 ? true : false;
	}
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Mail(MailTabIndex MailType)
{
	if (CurrentRedDotState.get() != nullptr)
	{
		switch (MailType)
		{
		case MailTabIndex::EMailTab_Gift: return CurrentRedDotState->has_present_mail;
		case MailTabIndex::EMailTab_BladePoint: return CurrentRedDotState->has_blade_point_mail;
		case MailTabIndex::EMailTab_Friendship: return CurrentRedDotState->has_social_point_mail;
		case MailTabIndex::EMailTab_Ticket: return CurrentRedDotState->has_lottery_mail;
		}
	}

	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Chat()
{
	//bool bHideChatting = UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting) == nullptr;
	//bool bIsNewMessage = false;
	//UB2UIDocGuild* DocGuild = UB2UIDocHelper::GetDocGuild();
	//if (DocGuild)
	//{
	//	bIsNewMessage = DocGuild->GetGuildChatBadge() > 0;
	//}
	//return bIsNewMessage && bHideChatting;;
	return true;
}

bool FB2RedDotManager::HasLobbyRedDot_Setting()
{
	return false;
}

bool FB2RedDotManager::HasUnappliedItemRandomOption()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_item_random_option_change : false;
}

bool FB2RedDotManager::HasUnappliedItemRandomQuality()
{
	//return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_item_random_quality_change : false;
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Event()
{
	return (HasLobbyRedDot_Event_Levelup() ||
			HasLobbyRedDot_Event_Point() ||
			HasLobbyRedDot_Event_StageClear() ||
			HasLobbyRedDot_Event_PointConsume() ||
			HasLobbyRedDot_Event_GemPurchase() ||
			HasLobbyRedDot_Event_TenLottery() ||
			HasLobbyRedDot_Event_GemSpend() ||
			HasLobbyRedDot_Event_TenLotterySaving() ||
			HasLobbyRedDot_Event_Dice()
			);
}

bool FB2RedDotManager::HasLobbyRedDot_Event_Levelup()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_event_level_up_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Event_Point()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_event_point_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_SeasonEvent()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_event_season_mission_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Event_StageClear()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_event_stage_clear_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Event_PointConsume()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_event_point_shop_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Event_GemPurchase()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_event_gem_purchase_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Event_TenLottery() 
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_event_ten_lottery_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Event_GemSpend()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_event_spend_gem_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Event_TenLotterySaving()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_event_ten_lottery_savings_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Event_Dice()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_event_dice_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_RestPoint()
{
	//return BladeIIGameImpl::GetClientDataStore().GetRestPoint() > 0;
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_Package_FlatRate()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_shop_package_flat_rate_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Package_Levelup()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_shop_package_level_up_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Package_ReachLevel()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_shop_package_reach_level_reward : false;
}

bool FB2RedDotManager::HasLobbyRedDot_MagicStore()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_magic_shop_free_renewal : false;
}

bool FB2RedDotManager::HasLobbyRedDot_HallOfFame()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->is_valid_hall_of_fame && CurrentRedDotState->has_hall_of_fame_praise_count : false;
}

bool FB2RedDotManager::HasNotApplyBlessFairy()
{
	//if (TutorialLockRedDotVisibleCheck(TutorialID_Fairy) == false)
	//	return false;

	//return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_not_apply_bless_fairy : false;
	return true;
}

bool FB2RedDotManager::HasLobbyRedDot_Donation()
{
	bool Result = false;
	if (CurrentRedDotState.get() != nullptr)
	{
		if (CurrentRedDotState->has_donation_point_reward || CurrentRedDotState->has_usable_free_donation_count)
		{
			Result = true;
		}
	}
	return Result;
}

bool FB2RedDotManager::HasLobbyRedDot_Totem()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_new_totem : false;
}

bool FB2RedDotManager::HasLobbyRedDot_Costume()
{
	return CurrentRedDotState.get() != nullptr ? CurrentRedDotState->has_new_costume : false;
}

bool FB2RedDotManager::HasLobbyRedDot_CostumeInventory(EPCClass PCClass)
{
	static TArray<EItemInvenType> CostumeInvenTypeArray{ EItemInvenType::EIIVT_CostumeWeapon , EItemInvenType::EIIVT_CostumeArmor};

	for (EItemInvenType CurrentInvenType : CostumeInvenTypeArray)
	{
		if (HasLobbyRedDot_HeroMgmt_Inventory(PCClass, CurrentInvenType))
			return true;
	}
	return false;
}

bool FB2RedDotManager::HasLobbyRedDot_CostumeInventory(EPCClass PCClass, EItemInvenType TapType)
{
	TArray<FB2Item> CurrPCItemList;
	//UB2LobbyInventory::GetStoredItemList(CurrPCItemList, TapType, PCClass, false);

	return CurrPCItemList.ContainsByPredicate([](FB2Item& Item) { return Item.IsNew; });
}

void FB2RedDotManager::OnOpenedAllMails(MailTabIndex OpenedMailIdx, int32 OpenedMailCount)
{
	if (CurrentRedDotState.get() != nullptr)
	{
		switch (OpenedMailIdx)
		{
		case MailTabIndex::EMailTab_Gift:		CurrentRedDotState->has_present_mail = false;		break;
		case MailTabIndex::EMailTab_BladePoint: CurrentRedDotState->has_blade_point_mail = false;	break;
		case MailTabIndex::EMailTab_Friendship: CurrentRedDotState->has_social_point_mail = false;	break;
		case MailTabIndex::EMailTab_Ticket:		CurrentRedDotState->has_lottery_mail = false;		break;
		}
		CurrentRedDotState->unread_mail_count -= OpenedMailCount;
	}
}

void FB2RedDotManager::OnOpenedSingleMail(MailTabIndex OpenedMailIdx)
{
	if (CurrentRedDotState.get() != nullptr)
	{
		if (OpenedMailIdx == MailTabIndex::EMailTab_Gift && CurrentRedDotState->has_present_mail)
			CurrentRedDotState->unread_mail_count--;
		else if (OpenedMailIdx == MailTabIndex::EMailTab_BladePoint && CurrentRedDotState->has_blade_point_mail)
			CurrentRedDotState->unread_mail_count--;
		else if (OpenedMailIdx == MailTabIndex::EMailTab_Friendship && CurrentRedDotState->has_social_point_mail)
			CurrentRedDotState->unread_mail_count--;
		else if (OpenedMailIdx == MailTabIndex::EMailTab_Ticket && CurrentRedDotState->has_lottery_mail)
			CurrentRedDotState->unread_mail_count--;
	}
}

const int32 FB2RedDotManager::GetUsedHeroTowerTicket()
{
	int32 UsedHeroTowerTicket = 0;

	if (CurrentRedDotState.get() != nullptr)
	{
		UsedHeroTowerTicket = CurrentRedDotState->used_tickets_for_hero_tower;
	}

	return UsedHeroTowerTicket;
}

bool FB2RedDotManager::TutorialLockRedDotVisibleCheck(int32 iLockTutorialID)
{
	//if (TutorialManager::GetInstance().LockWidgetRedDotVisibleCheck(iLockTutorialID))
	//	return true;
	//else
		return false;
}

bool FB2RedDotManager::IsAvailableCounterDungeonReward(int32 ClearCount, int32 RewardReceived)
{
	//const TArray<FCounterDungeunWeekReward>& ClientRewardData = BladeIIGameImpl::GetClientDataStore().GetCounterDeungeonRewardData();
	int32 AvailReceiveRewardIndex = INDEX_NONE;
	//for (int32 index = 0; index < ClientRewardData.Num(); index++)
	//{
	//	if (ClearCount >= ClientRewardData[index].RequireCount && index > AvailReceiveRewardIndex)
	//		AvailReceiveRewardIndex = index;
	//}

	return AvailReceiveRewardIndex >= RewardReceived;
}

void FB2RedDotManager::SetArrayRedDotHint(TArray<int32>& SetArray, std::initializer_list<RedDotHint> HintArguments) throw()
{
	try
	{
		SetArray.Empty();

		for (const auto& Hint : HintArguments)
		{
			SetArray.Add(static_cast<int32>(Hint)); 
		}
	}
	catch (...)
	{}
}

void FB2RedDotManager::RequestCheckRedDot(std::initializer_list<RedDotHint> HintArguments)
{
	TArray<int32> Hints;
	SetArrayRedDotHint(Hints, HintArguments);
	data_trader::Retailer::GetInstance().RequestCheckRedDot(Hints);
}
