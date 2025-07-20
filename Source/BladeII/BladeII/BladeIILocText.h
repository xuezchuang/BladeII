#pragma once

#include "EngineMinimal.h"

// !!!!!
// Note: Add matching entry to EB2LOC_CAT (and EB2LOC_CAT_ToString) for new Localized text category, unless you don't have to use the category in Blueprint
#define B2LOC_CAT_GENERAL TEXT("General")
#define B2LOC_CAT_WORLDNAME TEXT("WorldName")
#define B2LOC_CAT_CHARNAME TEXT("CharacterNameS2") /** It was originally "CharacterName", and been changed a little due to some stupid mistery, related to UE4 4.17 */
#define B2LOC_CAT_ITEMNAME TEXT("ItemName")
#define B2LOC_CAT_DIALOGTEXT TEXT("DialogLines")
#define B2LOC_CAT_SKILLDATA TEXT("SkillData")
#define B2LOC_CAT_MAILCONTENT TEXT("MailContent")
#define B2LOC_CAT_BATTLE_MENT TEXT("PVPMent")
#define B2LOC_CAT_QUESTTEXT	TEXT("Quest")
#define B2LOC_CAT_ERROR_CODE TEXT("ErrorCode")
#define B2LOC_CAT_MISSIONTEXT TEXT("Mission")
#define B2LOC_CAT_WINGTEXT TEXT("Wing")
#define B2LOC_CAT_STORE TEXT("Store")
#define B2LOC_CAT_ITEMNAMESUB	TEXT("Itemdescription")
#define B2LOC_CAT_ITEMNAMESUBSUB	TEXT("ItemdescriptionSub")
#define B2LOC_CAT_STAGENAME TEXT("StageName")
#define B2LOC_CAT_STAGESTORY TEXT("StageStory")
#define B2LOC_CAT_TALKTEXT TEXT("TalkLines")
#define B2LOC_CAT_TOOLTIPTEXT TEXT("ToolTipDescription")
#define B2LOC_CAT_CONTROLCHAT TEXT("ControlChat")
#define B2LOC_CAT_ANCIENTRELIC TEXT("AncientRelic")
#define B2LOC_CAT_TUTORIAL TEXT("Tutorial")
#define B2LOC_CAT_ETHER TEXT("Ether")
#define B2LOC_CAT_UNITYSKILLMISSION TEXT("UnitySkillMission")
#define B2LOC_CAT_ITEMPREFIX TEXT("ItemPrefix")
#define B2LOC_CAT_LOCALPUSH TEXT("LocalPush")
#define B2LOC_CAT_DUELMODLOSEMENT TEXT("DuelModeLoseMent")
#define B2LOC_CAT_EVENT TEXT("Event")
#define B2LOC_CAT_CONFIGURATION TEXT("Configuration")
#define B2LOC_CAT_BATTLEMESSAGE TEXT("BattleMessage")
#define B2LOC_CAT_USERREPORT TEXT("UserReport")
#define B2LOC_CAT_GUILDNPC TEXT("GuildNPC")
#define B2LOC_CAT_GUILDNPCSKILL TEXT("GuildNPCSkill")
#define B2LOC_CAT_ITEMFORGELSIT TEXT("ItemForgeList")

// Note: Add matching entry to EB2LOC_CAT (and EB2LOC_CAT_ToString) for new Localized text category, unless you don't have to use the category in Blueprint.
// !!!!!

/** Global util to use LocalizedTextTable. Use B2LOC_CAT_** defines for Category argument. */
FText BladeIIGetLOCText(const FString& Category, const FString& TextKey);
FText BladeIIGetDuelModLoseMentText();

enum class ESkillTextDataType : uint8
{
	SkillName,
	SkillDesc,
	OptionTooltipDesc,
	OptionPopupDesc
};

enum class EGuildNPCTextDataType : uint8
{
	NPCName,
	NPCNameDesc,
	SpecialSkillName1,
	SpecialSkillName2,
	SpecialSkillName3,
	SpecialSkillNameDesc1,
	SpecialSkillNameDesc2,
	SpecialSkillNameDesc3,
	SpecialSkillOption1,
	SpecialSkillOption2,
	SpecialSkillOption3,
};

enum class EGuildNPCSkillTextDataType : uint8
{
	SkillName,
	SkillNameDesc,
};

FString GetSkillLocalizedTextKey(ESkillTextDataType DataType, int32 SkillId, int32 OptionId = 0, int32 OptionLv = 0);
FString GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType DataType, int32 NPCId);
FString GetGuildNPCSkillLocalizedTextKey(EGuildNPCSkillTextDataType DataType, int32 SkillID);
