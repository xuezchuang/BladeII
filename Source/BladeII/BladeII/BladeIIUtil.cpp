#include "BladeIIUtil.h"
#include "../Common/CommonStruct.h"

#include "Components/SkeletalMeshComponent.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"
#include "B2StageInfo.h"

#include "B2ChapterInfo.h"
#include "BladeIIGameMode.h"
#include "B2NPCClassInfoBox.h"
#include "B2PCClassInfoBox.h"
#include "B2ItemInfo.h"
#include "B2CommonSoundInfo.h"
#include "B2CombatConstantInfo.h"
#include "BladeIICharacter.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIDocManager.h"
#include "B2StageManager.h"
#include "B2ScenarioDialogInfo.h"
#include "B2LobbyInventory.h"
#include "B2NPCSensitiveInfo.h"
#include "Event.h"
#include "InternalEvents.h"
#include "QuestDataTable.h"
#include "B2UIDocHelper.h"
#include "B2WingInfo.h"
#include "B2SomeInfo.h"
#include "B2TalkInfo.h"
#include "B2GuildMarkInfo.h"
#include "B2GoodsTypeInfo.h"
#include "B2GoodsShortcutinfo.h"
#include "B2LoadingImageInfo.h"
#include "PaperSprite.h"
#include "Retailer.h"
#include "B2UIGoodsShortcutTooltipPopup.h"
#include "B2LogManager.h"
#include "BladeIIBlockToSyncNetwork.h"
#if WITH_BII_ON_SCREEN_DEBUG_TEXT
#include "B2DebugTextWidget.h"
#endif
#include "B2HeroTowerGameMode.h"
#include "Animation/AnimNotifies/AnimNotify_PlayParticleEffect.h"
#include "Animation/AnimNotifies/AnimNotifyState_TimedParticleEffect.h"
//#include "Matinee/MatineeActor.h"
//#include "Matinee/InterpGroup.h"
//#include "Matinee/InterpGroupInst.h"
//#include "Matinee/InterpTrackMove.h"
#include "B2GuildBuffInfo.h"
#include "B2NoAnimSkillInfo.h"
#include "B2InfluenceSkillInfo.h"
#include "B2DamageEffectInfo.h"
#include "B2BuffModeEffectInfo.h"
#include "B2SkillInfo.h"
#include "B2CharacterTagDialogueInfo.h"
#include "B2SeasonEventInfo.h"
#include "NavigationSystem.h"
#include "BladeIIScalabilityControl.h"
#include "B2GameLoadingProgress.h"
#include "TutorialManager.h"
#include "BladeIIPlayerController.h"
#include "Global.h"
#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"
#include "B2GuildNPCInfo.h"
#include "B2ScenarioDialogInfo.h"
#include "B2DisconnectLogSender.h"
#include "ErrorCode.h"
//
//#include "Sound/SoundNode.h"
//#include "Sound/SoundNodeConcatenator.h"
//#include "Sound/SoundNodeWavePlayer.h"
//#include "Sound/SoundNodeLooping.h"
//
#include "B2GameInstance.h"

#include "B2NetworkBridge.h"
#include "B2ServerResultCodeTable.h"
#include "B2CodeTable.h"
#include "FairyManager.h"




FText BladeIIGetLOCText(const FString& TextCategory, const FString& TextKey)
{
	FText OutText;
	if (FText::FindText(TextCategory, TextKey, OutText, nullptr))
		return OutText;
	else
		return FText::FromString(TEXT("Unknown Text"));
}
FText BladeIIGetDuelModLoseMentText()
{
	FRandomStream TextRandom;

	TextRandom.GenerateNewSeed();
	int32 MaxKeyNumber = FCString::Atoi(*(BladeIIGetLOCText(B2LOC_CAT_DUELMODLOSEMENT, TEXT("Total")).ToString()));
	int32 RandomNumber = TextRandom.RandRange(1, MaxKeyNumber);

	return BladeIIGetLOCText(B2LOC_CAT_DUELMODLOSEMENT, FString::Printf(TEXT("Tip%d"), RandomNumber));
}

FString GetSkillLocalizedTextKey(ESkillTextDataType DataType, int32 SkillId, int32 OptionId, int32 OptionLv)
{
	FString GeneratedTextKey(TEXT(""));

	switch (DataType)
	{
	case ESkillTextDataType::SkillName:
		GeneratedTextKey = FString::Printf(TEXT("SkillName_%d"), SkillId);
		break;
	case ESkillTextDataType::SkillDesc:
		GeneratedTextKey = FString::Printf(TEXT("SkillDesc_%d"), SkillId);
		break;
	case ESkillTextDataType::OptionTooltipDesc:
		GeneratedTextKey = FString::Printf(TEXT("SkillOption_%d_%02d_%02d"), SkillId, OptionId, OptionLv);
		break;
	case ESkillTextDataType::OptionPopupDesc:
		GeneratedTextKey = FString::Printf(TEXT("SkillOption_%02d"), OptionId);
		break;
	}

	return GeneratedTextKey;
}

FString GetGuildNPCLocalizedTextKey(EGuildNPCTextDataType DataType, int32 NPCId)
{
	FString GeneratedTextKey(TEXT(""));

	switch (DataType)
	{
	case EGuildNPCTextDataType::NPCName:
		GeneratedTextKey = FString::Printf(TEXT("NPCName_%d"), NPCId);
		break;
	case EGuildNPCTextDataType::NPCNameDesc:
		GeneratedTextKey = FString::Printf(TEXT("NPCNameDesc_%d"), NPCId);
		break;
	case EGuildNPCTextDataType::SpecialSkillName1:
		GeneratedTextKey = FString::Printf(TEXT("SpecialSkillName1_%d"), NPCId);
		break;
	case EGuildNPCTextDataType::SpecialSkillName2:
		GeneratedTextKey = FString::Printf(TEXT("SpecialSkillName2_%d"), NPCId);
		break;
	case EGuildNPCTextDataType::SpecialSkillName3:
		GeneratedTextKey = FString::Printf(TEXT("SpecialSkillName3+%d"), NPCId);
		break;
	case EGuildNPCTextDataType::SpecialSkillNameDesc1:
		GeneratedTextKey = FString::Printf(TEXT("SpecialSkillNameDesc1_%d"), NPCId);
		break;
	case EGuildNPCTextDataType::SpecialSkillNameDesc2:
		GeneratedTextKey = FString::Printf(TEXT("SpecialSkillNameDesc2_%d"), NPCId);
		break;
	case EGuildNPCTextDataType::SpecialSkillNameDesc3:
		GeneratedTextKey = FString::Printf(TEXT("SpecialSkillNameDesc3_%d"), NPCId);
		break;
	case EGuildNPCTextDataType::SpecialSkillOption1:
		GeneratedTextKey = FString::Printf(TEXT("SpecialSkillOption1_%d"), NPCId);
		break;
	case EGuildNPCTextDataType::SpecialSkillOption2:
		GeneratedTextKey = FString::Printf(TEXT("SpecialSkillOption2_%d"), NPCId);
		break;
	case EGuildNPCTextDataType::SpecialSkillOption3:
		GeneratedTextKey = FString::Printf(TEXT("SpecialSkillOption3_%d"), NPCId);
		break;
	}

	return GeneratedTextKey;
}

FString GetGuildNPCSkillLocalizedTextKey(EGuildNPCSkillTextDataType DataType, int32 SkillID)
{
	FString GeneratedTextKey(TEXT(""));

	switch (DataType)
	{
	case EGuildNPCSkillTextDataType::SkillName:
		GeneratedTextKey = FString::Printf(TEXT("SkillName_%d"), SkillID);
		break;
	case EGuildNPCSkillTextDataType::SkillNameDesc:
		GeneratedTextKey = FString::Printf(TEXT("SkillNameDesc_%d"), SkillID);
		break;
	}

	return GeneratedTextKey;
}


FText GetPCClassLocalizedText(EPCClass PCClass)
{
	switch (PCClass)
	{
	case EPCClass::EPC_Gladiator:		return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Gladiator"));
	case EPCClass::EPC_Assassin:		return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Assassin"));
	case EPCClass::EPC_Wizard:			return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Wizard"));
	case EPCClass::EPC_Fighter:			return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, TEXT("PCClass_Fighter"));
	}

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	return FText::FromString(TEXT("Unknown Text"));
}

bool OpenBladeIIDLCFrontCommon(class AGameMode* InCurrentGameMode)
{
	// DLC 柳涝阑 困茄 内绢 扁瓷父 持绢初澜. 
	// 捞巴阑 妮 窍扁 捞傈俊 DeferredRegistChangeLobbyScene 阑 持绢具 柳青档 抗螟捞 狼档措肺 瞪 巴.

	//[@AKI, 180508] 聚捞 犁角青 登绰 巴 力寇窍绊绰 荤侩秦辑绰 救凳!!!
	//单捞磐甫 檬扁拳 窍瘤 臼绊 荤侩窍绊 乐绢辑 救凳!! 荤侩矫 单捞磐 檬扁拳 窍绰 何盒阑 备泅 秦具 且 巴

	FString DLCFrontMapPath;
	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIDLCMap"), DLCFrontMapPath, GGameIni);

	FB2NetworkBridge::GetInstance()->Shutdown();

	if (DLCFrontMapPath.Len() > 0)
	{
		// 鉴荐 甘 肺爹父狼 抗惑 瘤盒 技泼.
		B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Lobby);
		UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*DLCFrontMapPath));
		return true;
	}
	else
	{
		UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*UGameMapsSettings::GetGameDefaultMap()));
		return false;
	}
}

bool OpenBladeIILobbyCommon(class AGameMode* InCurrentGameMode)
{
	// 肺厚 柳涝阑 困茄 内绢 扁瓷父 持绢初澜. 
	// 捞巴阑 妮 窍扁 捞傈俊 DeferredRegistChangeLobbyScene 阑 持绢具 柳青档 抗螟捞 狼档措肺 瞪 巴.

	FString LobbyMapPath;
	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIILobbyMap"), LobbyMapPath, GGameIni);

	if (LobbyMapPath.Len() > 0)
	{
		// 鉴荐 甘 肺爹父狼 抗惑 瘤盒 技泼.
		B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Lobby);
		UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*LobbyMapPath));
		return true;
	}
	else
	{
		UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*UGameMapsSettings::GetGameDefaultMap()));
		return false;
	}
}

bool StartBladeIIGameStage(AGameMode* InCurrentGameMode, int32 InClientStageId, FString OpenURLOption, EB2GameMode ChangeGameMode)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIGameStage"));

	// 促澜 甘 肺爹 柳青档甫 困秦 鉴荐 甘 肺爹父狼 抗惑 瘤盒阑 技泼
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, ChangeGameMode);

	// 林狼: 促弗 镑俊辑绰 被捞 B2GameModule 俊辑 StageInfoTable 阑 流立 啊廉促 镜 鞘夸啊 绝促.
	// 咯扁绰 StageManager 啊 甸绊 乐绰 CurrentStageNumber 客 促弗 StageInfoData 甫 啊廉坷扁 困窃.
	// 咯鸥 老馆利牢 荤侩俊辑绰 啊鞭利 AB2StageManager::GetCurrentStageInfoData 唱 AB2StageGameMode::GetCurrentStageInfoData 甫 荤侩

	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	FSingleStageInfoData* StageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(InClientStageId) : NULL;
	if (StageInfoData)
	{
		// 努扼捞攫飘 抛胶飘 侩栏肺 StageNum 逞败淋.
		BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = InClientStageId;

		// OnPreLoadMap 俊辑 Movie 矫累 矫痢鳖瘤 泪瘪 拳搁阑 丹阑 UI Screen.
		OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, ChangeGameMode);

		UGameplayStatics::OpenLevel(InCurrentGameMode, StageInfoData->MainMap, true, OpenURLOption);
		return true;
	}

	return false;
}

bool StartBladeIIPVPGame(class AGameMode* InCurrentGameMode, FString OpenURLOption)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIPVPGame"));

	// 促澜 甘 肺爹 柳青档甫 困秦 鉴荐 甘 肺爹父狼 抗惑 瘤盒阑 技泼
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::PVP_Tag);

	FString PVPMapName;

	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIPVPMap"), PVPMapName, GGameIni);

	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::PVP_Tag); // 肺爹 胶农赴阑 固府 茄锅 肺爹秦 林绰 霸 救傈窍促绰 啊汲捞 乐绢辑 固府 茄瞒肥 秦 霖促..

	UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*PVPMapName), true, OpenURLOption);
	return true;
}

bool StartBladeIIPVPReplay(class AGameMode* InCurrentGameMode, FModReplayInfo ReplayInfo, FString OpenURLOption)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIPVPReplay"));

	auto* doc = UB2UIDocHelper::GetDocSome();
	if (doc)
	{
		doc->ReplayInfo = ReplayInfo;
	}

	FString ReplayMapName(TEXT("/Game/Maps/ReplayTest"));

	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance()
		.Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::PVPReplay);

	UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*ReplayMapName), true, OpenURLOption);

	return true;
}

bool StartBladeIITeamMatchGame(class AGameMode* InCurrentGameMode, FString OpenURLOption /*= FString(TEXT(""))*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIITeamMatchGame"));

	// 促澜 甘 肺爹 柳青档甫 困秦 鉴荐 甘 肺爹父狼 抗惑 瘤盒阑 技泼
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::PVP_Team);

	FString TeamMatchMapName;

	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIITeamMatchMap"), TeamMatchMapName, GGameIni);

	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::PVP_Team); // 肺爹 胶农赴阑 固府 茄锅 肺爹秦 林绰 霸 救傈窍促绰 啊汲捞 乐绢辑 固府 茄瞒肥 秦 霖促..

	UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*TeamMatchMapName), true, OpenURLOption);
	return true;
}

bool StartBladeIICounterDungeon(class AGameMode* InCurrentGameMode, int32 CounterStageNum)
{
	return StartBladeIIGameStage(InCurrentGameMode, CounterStageNum, FString(TEXT("")), EB2GameMode::CounterDungeon);
}

bool StartBladeIIHerotowerGame(class AGameMode* InCurrentGameMode, int32 HeroTowerNum)
{
	return StartBladeIIGameStage(InCurrentGameMode, HeroTowerNum, FString(TEXT("")), EB2GameMode::HeroTower);
}

bool StartBladeIIRaidGame(class AGameMode* InCurrentGameMode, int32 RaidClientStageID, FString OpenURLOption)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIRaidGame"));
	return StartBladeIIGameStage(InCurrentGameMode, RaidClientStageID, FString(TEXT("")), EB2GameMode::Raid);
}

bool StartBladeIIControlGame(class AGameMode* InCurrentGameMode, FString OpenURLOption /*= FString(TEXT(""))*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIControlGame"));

	// 促澜 甘 肺爹 柳青档甫 困秦 鉴荐 甘 肺爹父狼 抗惑 瘤盒阑 技泼
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Control);

	FString ControlMapName;

	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIControlMap"), ControlMapName, GGameIni);

	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::Control); // 肺爹 胶农赴阑 固府 茄锅 肺爹秦 林绰 霸 救傈窍促绰 啊汲捞 乐绢辑 固府 茄瞒肥 秦 霖促..

	UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*ControlMapName), true, OpenURLOption);
	return true;
}

bool StartBladeIIGuildMatchGame(class AGameMode* InCurrentGameMode, FString OpenURLOption /*= FString(TEXT(""))*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIGuildGame"));

	// 促澜 甘 肺爹 柳青档甫 困秦 鉴荐 甘 肺爹父狼 抗惑 瘤盒阑 技泼
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Guild);

	FString ControlMapName;

	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIGuildMatchMap"), ControlMapName, GGameIni);

	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::Guild); // 肺爹 胶农赴阑 固府 茄锅 肺爹秦 林绰 霸 救傈窍促绰 啊汲捞 乐绢辑 固府 茄瞒肥 秦 霖促..

	UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*ControlMapName), true, OpenURLOption);
	return true;

}

// 捞芭 措脚 ABladeIIGameMode::GoToTutorial 捞芭 静绰 淀..?
bool StartBladeIIGameTutorial(class AGameMode* InCurrentGameMode, FString OpenURLOption)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIGameTutorial"));

	// 促澜 甘 肺爹 柳青档甫 困秦 鉴荐 甘 肺爹父狼 抗惑 瘤盒阑 技泼
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Tutorial);

	FString TutorialMapPath;
	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIITutorialMap"), TutorialMapPath, GGameIni);

	if (TutorialMapPath.Len() > 0)
	{
		UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*TutorialMapPath));
	}
	else
		UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*UGameMapsSettings::GetGameDefaultMap()));

	return true;
}

bool StartBladeIIDimension(class AGameMode* InCurrentGameMode, int32 MapLoadID, FString OpenURLOption)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIDimension"));

	// 促澜 甘 肺爹 柳青档甫 困秦 鉴荐 甘 肺爹父狼 抗惑 瘤盒阑 技泼
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Dimension);

	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	FSingleStageInfoData* StageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(MapLoadID) : NULL;
	if (StageInfoData)
	{
		// 努扼捞攫飘 抛胶飘 侩栏肺 StageNum 逞败淋.
		BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = MapLoadID;

		// OnPreLoadMap 俊辑 Movie 矫累 矫痢鳖瘤 泪瘪 拳搁阑 丹阑 UI Screen.
		OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::Dimension);

		UGameplayStatics::OpenLevel(InCurrentGameMode, StageInfoData->MainMap, true, OpenURLOption);
		return true;
	}

	return false;
}

UB2StageInfo* StaticFindStageInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetStageInfoTable()));
	return B2GameModule ? B2GameModule->GetStageInfoTable() : NULL;
}

UB2ChapterInfo* StaticFindChapterInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetChapterInfoTable()));
	return B2GameModule ? B2GameModule->GetChapterInfoTable() : NULL;
}

UB2ItemInfo* StaticFindItemInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetItemInfoTable()));
	return B2GameModule ? B2GameModule->GetItemInfoTable() : NULL;
}

UB2ScenarioDialogInfo* StaticFindDialogInfoTable()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetDialogInfoTable()));
	return B2GameModule ? B2GameModule->GetDialogInfoTable() : NULL;
}

UB2DialogSoundInfoSet* StaticFindDialogSoundInfoTable()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetDialogSoundInfoTable()));
	return B2GameModule ? B2GameModule->GetDialogSoundInfoTable() : NULL;
}

UB2NPCSensitiveInfo* StaticFindNPCSensitiveInfoTable()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetNPCSensitiveInfoTable()));
	return B2GameModule ? B2GameModule->GetNPCSensitiveInfoTable() : NULL;
}

UB2BrevetInfo* StaticFindBrevetInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetBrevetInfoTable()));
	return B2GameModule ? B2GameModule->GetBrevetInfoTable() : NULL;
}

UB2ErrorCodeInfo* StaticFindErrorCodeInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetErrorCodeInfoTable()));
	return B2GameModule ? B2GameModule->GetErrorCodeInfoTable() : NULL;
}

UB2QuestInfoTable* StaticFindQuestTable()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetQuestInfoTable()));
	return B2GameModule ? B2GameModule->GetQuestInfoTable() : NULL;
}

class UB2QuestNPCSoundInfo* StaticFindQuestNPCSoundTable()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetQuestNPCSoundInfoTable()));
	return B2GameModule ? B2GameModule->GetQuestNPCSoundInfoTable() : NULL;
}

UB2MissionInfo* StaticFindMissionInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetMissionInfoTable()));
	return B2GameModule ? B2GameModule->GetMissionInfoTable() : NULL;
}

UB2RewardInfo* StaticFindRewardInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetRewardInfoTable()));
	return B2GameModule ? B2GameModule->GetRewardInfoTable() : NULL;
}

UB2GoodsTypeInfo* StaticFindGoodsTypeInfoTable()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetGoodsTypeInfoTable()));
	return B2GameModule ? B2GameModule->GetGoodsTypeInfoTable() : NULL;
}

UB2GoodsShortcutInfo* StaticFindGoodsShortcutInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetGoodsShortcutInfo()));
	return B2GameModule ? B2GameModule->GetGoodsShortcutInfo() : NULL;
}

class UB2RaidInfoTable* StaticFindRaidInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetRaidInfoTable()));
	return B2GameModule ? B2GameModule->GetRaidInfoTable() : NULL;
}

class UB2TalkInfo* StaticFindTalkInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetTalkInfoTable()));
	return B2GameModule ? B2GameModule->GetTalkInfoTable() : NULL;
}

class UB2DialogSoundInfoSet* StaticFindTalkSoundInfoTable()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetTalkSoundInfoTable()));
	return B2GameModule ? B2GameModule->GetTalkSoundInfoTable() : NULL;
}

class UB2ChattingFilteringInfo* StaticFindChattingFilteringInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetChattingFilteringInfoTable()));
	return B2GameModule ? B2GameModule->GetChattingFilteringInfoTable() : NULL;
}

class UB2EtherInfo* StaticFindEtherInfoTable()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetEtherInfoTable()));
	return B2GameModule ? B2GameModule->GetEtherInfoTable() : NULL;
}

class UB2SelectiveEtherLottery* StaticFindSelectiveEtherLotteryTable()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetSelectiveEtherLotteryTable()));
	return B2GameModule ? B2GameModule->GetSelectiveEtherLotteryTable() : NULL;
}

class UB2EtherSetOptionInfo* StaticFindEtherSetOptionInfoTable()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetEtherSetOptionInfoTable()));
	return B2GameModule ? B2GameModule->GetEtherSetOptionInfoTable() : NULL;
}

class UB2EtherSetInfoTable* StaticFindEtherSetEffectInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetEtherSetEffectInfo()));
	return B2GameModule ? B2GameModule->GetEtherSetEffectInfo() : NULL;
}

class UB2TotemRefineInfo* StaticFindTotemRefineInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetTotemRefineInfoTable()));
	return B2GameModule ? B2GameModule->GetTotemRefineInfoTable() : NULL;
}

UB2ItemSortInfo * StaticFindItemSortInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetItemSortInfoTable()));
	return B2GameModule ? B2GameModule->GetItemSortInfoTable() : NULL;
}

class UB2CharacterTagDialogueInfo * StaticFindCharacterTagDialogueInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetCharacterTagDialogueInfoTable()));
	return B2GameModule ? B2GameModule->GetCharacterTagDialogueInfoTable() : NULL;
}

class UB2SeasonEventInfo * StaticFindSeasonEventInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetSeasonEventInfoTable()));
	return B2GameModule ? B2GameModule->GetSeasonEventInfoTable() : NULL;
}

class UB2FairyInfo* StaticFindFairyInfo()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(GMinimalDLCFrontMode || (B2GameModule && B2GameModule->GetFairyInfoTable()));
	return B2GameModule ? B2GameModule->GetFairyInfoTable() : NULL;
}

//
//////////////////////////////////////////////////////////////////////////////////////////
// 捞 捞窍 StaticFind 甸篮 蠢副 荐 乐栏骨肺 啊鞭利 BladeIIGameMode 俊 某教等 吧 荤侩窍绊(WorldContextObject甫 逞败淋), 概 tick 荤侩窍瘤 富 巴.

UB2PCClassInfoBox* StaticFindPCClassInfoBox(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI)
		{
			return B2GI->GetPCClassInfoBox();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2PCClassInfoBox>(TEXT("/Script/BladeII.B2PCClassInfo"), TEXT("PCClassInfoBoxAsset"));
}
UB2NPCClassInfoBox* StaticFindMobClassInfoBox(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI) {
			return B2GI->GetMobClassInfoBox();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2NPCClassInfoBox>(TEXT("/Script/BladeII.B2NPCClassInfo"), TEXT("MobClassInfoBoxAsset"));
}

UB2CommonSoundInfo* StaticFindCommonSoundInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI)
		{
			return B2GI->GetCommonSoundInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2CommonSoundInfo>(TEXT("/Script/BladeII"), TEXT("CommonSoundInfoAsset"));
}

UB2DialogCharacterInfoSet* StaticFindDialogCharacterInfoTable(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI) {
			return B2GI->GetDialogCharacterInfoTable();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2DialogCharacterInfoSet>(TEXT("/Script/BladeII.B2DialogCharacterInfoSet"), TEXT("DialogCharacterInfoTable"));
}

UB2WingInfo* StaticFindWingInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI)
		{
			return B2GI->GetWingInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2WingInfo>(TEXT("/Script/BladeII.B2WingInfo"), TEXT("WingInfoAsset"));
}

UB2SomeInfo* StaticFindSomeInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI)
		{
			return B2GI->GetSomeInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2SomeInfo>(TEXT("/Script/BladeII"), TEXT("SomeInfoAsset"));
}

UB2ItemMiscInfo* StaticFindItemMiscInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI) {
			return B2GI->GetItemMiscInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2ItemMiscInfo>(TEXT("/Script/BladeII.B2Item"), TEXT("ItemMiscInfoAsset"));
}

class UB2GuildMarkInfo* StaticFindGuildMarkInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI) {
			return B2GI->GetGuildMarkInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2GuildMarkInfo>(TEXT("/Script/BladeII"), TEXT("GuildMarkInfoAsset"));
}

UB2LoadingImageInfo* StaticFindLoadingImageInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI) {
			return B2GI->GetLoadingImageInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2LoadingImageInfo>(TEXT("/Script/BladeII"), TEXT("LoadingImageInfoAsset"));
}

class UB2GuildSkillInfo* StaticFindGuildSkillInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI) {
			return B2GI->GetGuildSkillInfo();
		}
	}

	return StaticFindBPInfoAssetTempl<UB2GuildSkillInfo>(TEXT("/Script/BladeII"), TEXT("GuildSkillInfoAsset"));
}

class UB2NoAnimSkillInfo* StaticFindNoAnimSkillInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI) {
			return B2GI->GetNoAnimSkillInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2NoAnimSkillInfo>(TEXT("/Script/BladeII"), TEXT("NoAnimSkillInfo"));
}

class UB2InfluenceSkillInfo* StaticFindInfluenceSkillInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI) {
			return B2GI->GetInfluenceSkillInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2InfluenceSkillInfo>(TEXT("/Script/BladeII"), TEXT("InfluenceSkillInfoAsset"));
}

UB2DamageEffectInfo* StaticFindDamageEffectInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI)
		{
			return B2GI->GetDamageEffectInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2DamageEffectInfo>(TEXT("/Script/BladeII"), TEXT("DefaultDamageEffectClassPath"));
}

UB2BuffModeEffectInfo* StaticFindBuffModeEffectInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI) {
			return B2GI->GetBuffModeEffectInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2BuffModeEffectInfo>(TEXT("/Script/BladeII"), TEXT("DefaultBuffModeEffectClassPath"));
}

UB2SkillAnimInfo* StaticFindSkillAnimInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI) {
			return B2GI->GetSkillAnimInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2SkillAnimInfo>(TEXT("/Script/BladeII"), TEXT("SkillAnimInfoAsset"));
}

class UB2GuildNPCInfo* StaticFindGuildNPCInfo(UObject* WorldContextObject)
{
	//if (WorldContextObject)
	//{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
	//	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	//	if (B2GI)
	//	{
	//		return B2GI->GetGuildNPCInfo();
	//	}
	//}
	return StaticFindBPInfoAssetTempl<UB2GuildNPCInfo>(TEXT("/Script/BladeII"), TEXT("GuildNPCInofAsset"));
}

class UTutorialControlBotInfos* StaticFindTutorialControlBotInfos()
{
	return StaticFindBPInfoAssetTempl<UTutorialControlBotInfos>(TEXT("/Script/BladeII.B2TutorialInfo"), TEXT("TutorialControlBotInfos"));
}

UB2CombatConstantInfo * StaticFindCombatConstantInfo(UObject * WorldContextObject)
{
	if (WorldContextObject)
	{ // 啊瓷茄 版快 GameInstance 狼 某教等 饭欺繁胶甫 荤侩.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI)
		{
			return B2GI->GetCombatConstantInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2CombatConstantInfo>(TEXT("/Script/BladeII"), TEXT("CombatConstantInfoAsset"));
}

int32 UpdateAutoBattleType(int32 InAutoBattleType)
{
	int32 AutoBallte = InAutoBattleType;

	if (AutoBallte == AutoBattleType::NONE)
	{
		AutoBallte = AutoBattleType::Attack;
	}
	else
	{
		AutoBallte = AutoBallte << 1;
		if (AutoBallte > AutoBattleType::Skill)
		{
			AutoBallte = AutoBattleType::NONE;
		}
	}

	return AutoBallte;
}

//////////////////////////////////////////////////////////////////////////////////////////

#if !UE_BUILD_SHIPPING
DEFINE_LOG_CATEGORY_STATIC(LogB2ScopedCallTrack, Log, All)

bool FB2ScopedCallTracker::bIsOn = true;
float FB2ScopedCallTracker::PerfMarkLv1Time = 0.1f;
float FB2ScopedCallTracker::PerfMarkLv2Time = 0.5f;
FB2ScopedCallTracker::FB2ScopedCallTracker(FString InScopeName)
{
	ScopeName = InScopeName;
	RecordedStartTime = FPlatformTime::Seconds();
	if (bIsOn)
	{
		UE_LOG(LogB2ScopedCallTrack, Log, TEXT("%s Begin"), *ScopeName);
	}
}

FB2ScopedCallTracker::FB2ScopedCallTracker(const ANSICHAR* InScopeName)
{
	FB2ScopedCallTracker(FString(InScopeName));
}

FB2ScopedCallTracker::~FB2ScopedCallTracker()
{
	extern bool gDrawPerfMark;
	if (bIsOn || gDrawPerfMark)
	{
		double TimeTaken = FPlatformTime::Seconds() - RecordedStartTime;
		// 角青 矫埃捞 老沥 扁霖 捞惑栏肺 坷贰 吧府搁 版绊肺 钎矫.. 隔酒辑 茫酒焊扁 亮霸.. 措眉肺 捞吧 汲摹窍绰 镑篮 Tick 俊辑 阂府绰 扒 酒聪绢辑 捞亥飘 己栏肺档 坷贰 吧赴促 酵篮 沥档牢 芭甫 吧矾辰促.
		const bool bConsiderAsWarningLv1 = (TimeTaken >= PerfMarkLv1Time);
		const bool bConsiderAsWarningLv2 = (TimeTaken >= PerfMarkLv2Time);

		FString LogString = FString::Printf(TEXT("%s End (Exec time : %f ms)"), *ScopeName, TimeTaken * 1000.0);

		FLinearColor PerfMarkDrawTextColor(1.0f, 1.0f, 0.0f, 1.0f);
		if (bConsiderAsWarningLv2) {
			UE_LOG(LogB2ScopedCallTrack, Warning, TEXT("%s [PerfMark_Lv2]"), *LogString);
			PerfMarkDrawTextColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (bConsiderAsWarningLv1) {
			UE_LOG(LogB2ScopedCallTrack, Display, TEXT("%s [PerfMark_Lv1]"), *LogString);
		}
		else {
			UE_LOG(LogB2ScopedCallTrack, Log, TEXT("%s"), *LogString);
		}

		// 漂喊洒 拳搁俊 焊扁 奖霸 钎矫
		if (gDrawPerfMark && (bConsiderAsWarningLv1 || bConsiderAsWarningLv2))
		{
			BII_SCREEN_LOG(LogString, PerfMarkDrawTextColor, 12, 8.0f);
		}
	}
}


// ScopedCallTracker 俊档 矫埃 螟沥 扁瓷捞 甸绢艾绰单 弊犯促绊 ScopedTimeLogger 甫 镜葛绝促绊 且 鞘挎 绝绊 漂喊洒 矫埃 螟沥捞 鞘夸茄 胶讫俊 持绢辑 静搁 凳.
//
DEFINE_LOG_CATEGORY_STATIC(LogB2ScopedTimeLog, Log, All)
bool FB2ScopedTimeLogger::bIsOn = false; // 捞扒 扁夯蔼 false.. 鞘夸且 锭父 荤侩窍妨绰 芭
float FB2ScopedTimeLogger::PerfMarkLv1Time = 0.1f;
float FB2ScopedTimeLogger::PerfMarkLv2Time = 0.5f;
FB2ScopedTimeLogger::FB2ScopedTimeLogger(FString InScopeName)
{
	ScopeName = InScopeName;
	RecordedStartTime = FPlatformTime::Seconds();

	if (bIsOn)
	{
		UE_LOG(LogB2ScopedTimeLog, Log, TEXT("Beginning of %s"), *ScopeName);
	}
}
FB2ScopedTimeLogger::FB2ScopedTimeLogger(const ANSICHAR* InScopeName)
{
	FB2ScopedTimeLogger(FString(InScopeName));
}
FB2ScopedTimeLogger::~FB2ScopedTimeLogger()
{
	if (bIsOn)
	{
		double TimeTaken = FPlatformTime::Seconds() - RecordedStartTime;
		// 角青 矫埃捞 老沥 扁霖 捞惑栏肺 坷贰 吧府搁 版绊肺 钎矫.. 隔酒辑 茫酒焊扁 亮霸.. 措眉肺 捞吧 汲摹窍绰 镑篮 Tick 俊辑 阂府绰 扒 酒聪绢辑 捞亥飘 己栏肺档 坷贰 吧赴促 酵篮 沥档牢 芭甫 吧矾辰促.
		const bool bConsiderAsWarningLv1 = (TimeTaken >= PerfMarkLv1Time);
		const bool bConsiderAsWarningLv2 = (TimeTaken >= PerfMarkLv2Time);

		FString LogString = FString::Printf(TEXT("Execution time of %s : %f ms"), *ScopeName, TimeTaken * 1000.0);

		if (bConsiderAsWarningLv2) {
			UE_LOG(LogB2ScopedTimeLog, Warning, TEXT("%s [PerfMark_Lv2]"), *LogString);
		}
		else if (bConsiderAsWarningLv1) {
			UE_LOG(LogB2ScopedTimeLog, Display, TEXT("%s [PerfMark_Lv1]"), *LogString);
		}
		else {
			UE_LOG(LogB2ScopedCallTrack, Log, TEXT("%s"), *LogString);
		}
	}
}
#endif

void ManualUnloadBeforeNewBeginning(UObject* WorldContextObject)
{
	B2_SCOPED_TRACK_LOG(TEXT("ManualUnloadBeforeNewBeginning"));

#if WITH_EDITOR
	if (GIsEditor)
	{
		return; // 俊叼磐俊急 捞繁 窿阑 秦急 救等促. 葛滴 肺爹等 惑怕 蜡瘤.
	}
#endif

	FlushAsyncLoading(); // 捞吧 窍绰 霸 救傈且鳖?

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// TAsset 利侩等 InfoAsset 甸 吝俊辑 喊档狼 饭欺繁胶 某教苞 窃膊 FStreamableManager 甫 静绰 局甸篮 疙矫利栏肺 攫肺爹茄促. 
	// FStreamableManager 狼 巩力痢捞扁档 茄单, 怖 弊芭 酒聪歹扼档 Preload 狼 鞘夸己栏肺 饭欺繁胶 某教捞 乐促搁 力芭绰 秦 林绢具 窃.
	// 磊肋茄 局甸篮 FStreamableManager 措脚 窜鉴 LoadSynchronous 荤侩栏肺 傈券窍搁 捞芭 救窍霸 瞪 巴烙.
	// 捞芭狼 馆措绰 GameMode 狼 PreloadAnyNecessaryInfo

	UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox(WorldContextObject); // 啊瓷茄 茄 BladeIIGameMode 俊 某教登绢 乐绰 吧 啊廉坷扁 困秦 WorldContextObject 甫 逞败淋.
	if (MobInfoBox)
	{
		MobInfoBox->UnloadExceptCurrentExplicit(B2GM);
	}

	UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox(WorldContextObject);
	if (PCClassInfoBox)
	{ // 汲沥苞 敲阀汽俊 蝶扼 unload 甫 救窍霸 瞪 荐档 乐促.
		PCClassInfoBox->UnloadAllOrExceptSelectedIfAllowed();
	}
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{ // PCClassInfo 甫 犬角洒 Unload 窍妨搁 咯扁 单捞磐甫 拱绊 乐绰 HeroDoc 档 救傈窍霸 release 秦 临 鞘夸啊 乐促.
		UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCI);
		if (DocHero)
		{
			DocHero->ReleaseResources();
		}
	}
	// 茄锭绰 PCClassInfo 狼 老何肺 肺爹登带 skill 局聪皋捞记 单捞磐
	UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo(WorldContextObject);
	if (SkillAnimInfo)
	{
		SkillAnimInfo->UnloadAllOrExceptSelected(B2GM); // PCClassInfo 狼 老何 拜牢 父怒 付蛮啊瘤肺 UnloadAllOrExceptSelected 肺
	}

	UB2UIDocScenarioDialog* DocScenarioDlg = UB2UIDocHelper::GetDocScenarioDialog();
	if (DocScenarioDlg)
	{
		DocScenarioDlg->ReleaseResources(); // 檬惑拳 material 阑 甸绊 乐澜.
	}

	UB2CommonSoundInfo* CommonSoundInfo = StaticFindCommonSoundInfo(WorldContextObject);
	if (CommonSoundInfo)
	{
		CommonSoundInfo->UnloadAll();
	}

	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	if (ItemInfoTable)
	{
		ItemInfoTable->UnloadAllExceptCurrentLocalEquipPartData();
		ItemInfoTable->ResetAsyncLoadReqInfo();
	}

	UB2SomeInfo* SomeInfo = StaticFindSomeInfo(WorldContextObject);
	if (SomeInfo)
	{
		SomeInfo->UnloadAll();
	}

	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo(WorldContextObject);
	if (ItemMiscInfo)
	{
		ItemMiscInfo->UnloadAll();
	}

	UB2DamageEffectInfo* DamageFxInfo = StaticFindDamageEffectInfo(WorldContextObject);
	if (DamageFxInfo)
	{
		DamageFxInfo->ConditionalUnloadAllOrPreserve();
	}

	UB2BuffModeEffectInfo* BuffFxInfo = StaticFindBuffModeEffectInfo(WorldContextObject);
	if (BuffFxInfo)
	{
		BuffFxInfo->ConditionalUnloadAllOrPreserve();
	}

	UB2DialogCharacterInfoSet* DlgCharInfo = StaticFindDialogCharacterInfoTable(WorldContextObject);
	if (DlgCharInfo)
	{
		DlgCharInfo->UnloadAll();
	}

	UB2DialogSoundInfoSet* DlgSoundInfo = StaticFindDialogSoundInfoTable();
	if (DlgSoundInfo)
	{
		DlgSoundInfo->UnloadAll();
	}
	UB2CharacterTagDialogueInfo* CharDlgInfo = StaticFindCharacterTagDialogueInfo();
	if (CharDlgInfo)
	{
		CharDlgInfo->ConditionalUnloadAllOrPreserve();
	}

	// UIManager 率篮 咯扁辑 Unload 救窃. 宝洒 弊芭 Destroy 矫痢窍绊 爵鳃扼..
	// UB2LoadingImageInfo 档 咯扁辑 Unload 救 窃. 弊扒 馆措肺 措帆 捞 矫痢俊 鞘夸茄 吧 肺爹窍绊 促澜 饭骇 肺爹 饶俊 攫肺爹捞 瞪 巴.

	UE_LOG(LogBladeII, Log, TEXT("Unloaded lazy-loaded info assets"));

	// 扁鸥 眠啊瞪 荐 乐澜.
}

void ManualUnloadOnStageClear(UObject* WorldContextObject)
{
	B2_SCOPED_TRACK_LOG(TEXT("ManualUnloadOnStageClear"));

#if WITH_EDITOR
	if (GIsEditor)
	{
		return; // 俊叼磐俊急 捞繁 窿阑 秦急 救等促. 葛滴 肺爹等 惑怕 蜡瘤.
	}
#endif

	FlushAsyncLoading(); // 捞吧 窍绰 霸 救傈且鳖?

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// 胶抛捞瘤 辆丰 楷免 鞍篮 芭 唱坷搁辑 皋葛府 惑铰窍扁 流傈 郴副 芭 郴府扁.
	// 咯扁辑 攫肺爹 窍瘤 臼篮 巴甸档 ManualUnloadBeforeNewBeginning 俊辑 葛滴 郴妨哎 芭

	// 胶抛捞瘤 努府绢 矫痢捞扼 MobInfo 绰 歹 捞惑 鞘夸绝阑 巴栏肺 埃林. (汲飞 鞘夸茄 版快啊 惯积茄促 窍歹扼档 肺爹 喉肺欧捞 粱 积变促 挥)
	UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox(WorldContextObject); // 啊瓷茄 茄 BladeIIGameMode 俊 某教登绢 乐绰 吧 啊廉坷扁 困秦 WorldContextObject 甫 逞败淋.
	if (MobInfoBox)
	{
		MobInfoBox->UnloadExceptCurrentExplicit(B2GM);
	}

	UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox(WorldContextObject);
	if (PCInfoBox)
	{// 捞 矫痢俊辑 怕弊绰 拱沸 绝绢具 窍绊.. 铰府 楷免俊辑 怕弊某腐磐 puppet 阑 spawn 且 荐绰 乐绰单 芭扁辑 角龙利牢 PCClassInfo 单捞磐甫 荤侩窍瘤 臼档废 秦具 茄促.
	 // ABladeIIGameMode::SpawnPlayerCharAsPuppet 俊辑何磐 包访 牢磊甫 逞败林档废 登绢 乐澜.
		PCInfoBox->UnloadAllOrExceptSelectedIfAllowed();
	}
	// 茄锭绰 PCClassInfo 狼 老何肺 肺爹登带 skill 局聪皋捞记 单捞磐
	UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo(WorldContextObject);
	if (SkillAnimInfo)
	{
		SkillAnimInfo->UnloadAllOrExceptSelected(B2GM); // PCClassInfo 狼 老何 拜牢 父怒 付蛮啊瘤肺 UnloadAllOrExceptSelected 肺
	}

	UB2DamageEffectInfo* DamageFxInfo = StaticFindDamageEffectInfo(WorldContextObject);
	if (DamageFxInfo)
	{
		DamageFxInfo->ConditionalUnloadAllOrPreserve();
	}

	UB2BuffModeEffectInfo* BuffFxInfo = StaticFindBuffModeEffectInfo(WorldContextObject);
	if (BuffFxInfo)
	{
		BuffFxInfo->ConditionalUnloadAllOrPreserve();
	}

	UB2CharacterTagDialogueInfo* CharDlgInfo = StaticFindCharacterTagDialogueInfo();
	if (CharDlgInfo)
	{
		CharDlgInfo->ConditionalUnloadAllOrPreserve();
	}

	// 扁鸥 捞哩 喊 杭老 绝绢 焊捞绰 巴甸 攫肺爹.


	CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
}

void CopyObjectProperties(UObject* ObjCopyTo, UObject* ObjCopyFrom, bool bExcludeTransient, bool bExcludeSubComponents, const TArray<FName>& ExcludeProperties)
{
	//B2_SCOPED_TRACK_LOG(TEXT("CopyObjectProperties"));

	////if (
	////	!ObjCopyTo || !ObjCopyFrom ||
	////	// 鞍篮 努贰胶带瘤, 郴瘤绰 ObjCopyFrom 捞 ObjCopyTo 狼 窍困 努贰胶带瘤.
	////	(ObjCopyTo->GetClass() != ObjCopyFrom->GetClass() && !ObjCopyFrom->IsA(ObjCopyTo->GetClass()))
	////	)
	////{
	////	return;
	////}

	//// 曼绊 : FObjectInitializer::InitProperties
	//// 捞扒 扁夯 serialize 饶俊 荤侩登绰 巴捞骨肺 琴缴 颇飘绰 粱 窜鉴拳 登绢 乐澜.

	//UClass* ObjClass = ObjCopyTo->GetClass();
	//BII_CHECK(ObjClass);
	//for (FProperty* P = ObjClass->PropertyLink; P; P = P->PropertyLinkNext)
	//{
	//	const bool bIsTransient = P->HasAnyPropertyFlags(CPF_Transient | CPF_DuplicateTransient | CPF_NonPIEDuplicateTransient);
	//	if (bIsTransient && bExcludeTransient) {
	//		continue;
	//	}

	//	// Actor 甫 墨乔窍绰 版快 窍困 Component 鳖瘤 墨乔窍绊 酵瘤绰 臼阑 巴捞促. 愧父窍搁 蝶肺 贸府
	//	UObjectProperty* CastedObjProp = Cast<UObjectProperty>(P);
	//	if (bExcludeSubComponents && CastedObjProp && CastedObjProp->PropertyClass &&
	//		CastedObjProp->PropertyClass->GetDefaultObject()->IsA(UActorComponent::StaticClass())) {
	//		continue;
	//	}

	//	// 扁鸥 漂喊洒 力寇窍绊 酵篮 巴甸.
	//	if (ExcludeProperties.Find(P->GetFName()) == INDEX_NONE)
	//	{
	//		P->CopyCompleteValue_InContainer(ObjCopyTo, ObjCopyFrom);
	//	}
	//}
}

void DevShowNotReadyYetMessage(FString Message)
{
	ABladeIIGameMode::SetHUDCenterMessage(
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), Message), 1.0f
	);
}

void DevShowError(FString Message)
{
	ABladeIIGameMode::SetHUDCenterMessage(
		BladeIIGetLOCText(FString(B2LOC_CAT_ERROR_CODE), Message), 1.0f
	);
}

void DevShowNotReadyYetPopup()
{
	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DevShowNotReadyYet")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
		);
}

bool isLocalConnectIniFileExist()
{
	//UE4Game啊 Root牢淀 窃.
	//FPaths::GameDir();		//  ../../../BladeII/
	//FPaths::EngineDir();		//  ../../../Engine/
	//FPaths::GameSavedDir();	//  ../../../BladeII/Saved/
	//FPaths::GameUserDir();	//	../../../BladeII/
	//FPaths::RootDir();		//	../../../
	//FPaths::ProfilingDir();	//	../../../BladeII/Saved/Profiling/

	FString fileName(TEXT("3918D5C54C244BC2C1F44FD221EBD84F200162E1.Luc"));

	FString FullPath;
#if PLATFORM_IOS
	FullPath = FPaths::Combine(FPaths::RootDir(), fileName);
#else 
	//FullPath = FPaths::Combine(FPaths::GamePersistentDownloadDir(), fileName);
#endif

	bool bFileExist = FPaths::FileExists(FullPath);
//	if (bFileExist == false)
//	{
//		//家巩磊 八荤
//		fileName = fileName.ToLower();
//#if PLATFORM_IOS
//		FullPath = FPaths::Combine(FPaths::RootDir(), fileName);
//#else 
//		FullPath = FPaths::Combine(FPaths::GamePersistentDownloadDir(), fileName);
//#endif
//		bFileExist = FPaths::FileExists(FullPath);
//	}

#ifdef NONE_KAKAO_SDK_VERSION
	bFileExist = true;
#endif

	UE_LOG(LogBladeII, Log, TEXT("isLocalConnectIniFileExist : FullPath : %s"), *FullPath);

	/*
	//JSon Writer
	FArchive* WriteArc = IFileManager::Get().CreateFileWriter(*FullPath);
	if (WriteArc)
	{
	FString JsonStr;
	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(WriteArc);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(FString(TEXT("B3D42311E981FA6399D5CD01153D5DD03F6FF220")), FString(TEXT("LOCAL")));
	JsonWriter->WriteValue(FString(TEXT("B624E063500C07A876526621E2E7BAB39B3102AE")), true);

	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();

	WriteArc->Flush();
	WriteArc->Close();
	delete WriteArc;
	}
	if (bFileExist)
	{
	//JSon Reader
	FArchive* ReadArc = IFileManager::Get().CreateFileReader(*FullPath);
	if (ReadArc)
	{
	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(ReadArc);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed) && JsonParsed.IsValid())
	{
	FString stServerType = JsonParsed->GetStringField(FString(TEXT("B3D42311E981FA6399D5CD01153D5DD03F6FF220"))); //Sha1(ServerType)
	stServerType = stServerType.ToUpper();
	bFileExist = stServerType.Compare(FString("LOCAL")) == 0 ? true : false;
	bool bCheatEnable = JsonParsed->GetBoolField(FString(TEXT("B624E063500C07A876526621E2E7BAB39B3102AE"))); //Sha1(CheatEnable)
	//GShowConsoleWindowNextTick = bCheatEnable;
	}
	}
	}
	//*/
	return bFileExist;
}

// 酒贰 SetupNamedMIDForFxComp, GetFxNamedMIDCommon, SetupMIDAtlasParamFromUIMIC 甫 荤侩秦辑 
// 老馆利牢 3D material 阑 漂沥 UI material 苞 付蛮啊瘤肺 操固绰 抗矫甫 B2LobbyUI_ItemCUSProg 俊辑 曼绊..

void SetupNamedMIDForFxComp(UParticleSystemComponent* InPSComp)
{
	B2_SCOPED_TRACK_LOG(TEXT("SetupNamedMIDForFxComp"));

	if (!InPSComp || !InPSComp->Template) {
		return;
	}

	// FParticleEmitterInstance::Tick_MaterialOverrides 曼绊
	// NamedMaterialSlots 俊 技泼茄 巴俊 蝶扼 鞍篮 牢郸胶狼 EmitterMaterials 啊 乐栏搁 弊吧 荤侩窍霸 等促. 
	// Fx Component 积己窍搁辑 鉴辑措肺 EmitterMaterials 俊 MID 甫 父甸绢辑 持绢淋
	for (int32 SI = 0; SI < InPSComp->Template->NamedMaterialSlots.Num(); ++SI)
	{
		FNamedEmitterMaterial& CurrSlot = InPSComp->Template->NamedMaterialSlots[SI];
		if (CurrSlot.Material)
		{
			UMaterialInstanceDynamic* CurrSlotMID = UMaterialInstanceDynamic::Create(CurrSlot.Material, InPSComp);
			InPSComp->EmitterMaterials.Add(CurrSlotMID);
		}
	}
}

UMaterialInstanceDynamic* GetFxNamedMID(UParticleSystemComponent* InPSComp, const FName& MatSlotName)
{
	B2_SCOPED_TRACK_LOG(TEXT("GetFxNamedMID"));

	if (InPSComp && InPSComp->Template)
	{
		for (int32 SI = 0; SI < InPSComp->Template->NamedMaterialSlots.Num(); ++SI)
		{
			// FParticleEmitterInstance::Tick_MaterialOverrides 曼绊
			// NamedMaterialSlots 俊 技泼茄 巴俊 蝶扼 鞍篮 牢郸胶狼 EmitterMaterials 啊 乐栏搁 弊吧 荤侩窍霸 等促. 
			FNamedEmitterMaterial& CurrSlot = InPSComp->Template->NamedMaterialSlots[SI];
			if (CurrSlot.Name == MatSlotName && InPSComp->EmitterMaterials.IsValidIndex(SI))
			{
				// Fx 积己矫 鉴辑措肺 EmitterMaterials 俊 MID 积己秦辑 持绢躇绢具 窃. (SetupNamedMIDForFxComp)
				return Cast<UMaterialInstanceDynamic>(InPSComp->EmitterMaterials[SI]);
			}
		}
	}
	return NULL;
}

bool HasFxNamedMID(class UParticleSystemComponent* InPSComp, const FName& MatSlotName)
{
	return GetFxNamedMID(InPSComp, MatSlotName) != nullptr;
}

// SetupMIDAtlasParamFromUIMIC 狼 馆汗 颇飘
void GetAndSetMIScalarParamCommon(UMaterialInstanceDynamic* MIDToSet, UMaterialInstanceConstant* RefUIMIC, const FName& ParamName)
{
	B2_SCOPED_TRACK_LOG(TEXT("GetAndSetMIScalarParamCommon"));

	check(MIDToSet && RefUIMIC);
	float ThisParamValue = 0.0f;
	if (RefUIMIC->GetScalarParameterValue(ParamName, ThisParamValue)) {
		MIDToSet->SetScalarParameterValue(ParamName, ThisParamValue);
	}
}

void SetupMIDAtlasParamFromUIMIC(class UMaterialInstanceDynamic* MIDToSet, class UMaterialInstanceConstant* RefUIMIC)
{
	B2_SCOPED_TRACK_LOG(TEXT("SetupMIDAtlasParamFromUIMIC"));

	if (!MIDToSet || !RefUIMIC) {
		return;
	}
	// 拱沸 阿阿狼 master material 俊绰 鞍篮 捞抚狼 颇扼固磐啊 鞍篮 开且肺 甸绢啊 乐绢具摆促.
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleX);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleY);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleWidth);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleHeight);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasWholeWidth);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasWholeHeight);
	// 付瘤阜栏肺 咆胶媚 颇扼固磐档.
	UTexture* ThisTextureParamValue = NULL;
	if (RefUIMIC->GetTextureParameterValue(MtrlParamName_UIAtlasTexture, ThisTextureParamValue)) {
		MIDToSet->SetTextureParameterValue(MtrlParamName_UIAtlasTexture, ThisTextureParamValue);
	}
}
// 困俊 SetupMIDAtlasParamFromUIMIC 尔 悼老茄 付胶磐 赣萍府倔 啊沥俊 付蛮啊瘤 扁瓷牢单 NoAtlas static switch 肺 荤侩窍绰 芭
void SetupMIDNonAtlasParamFromUIMIC(class UMaterialInstanceDynamic* MIDToSet, class UMaterialInstanceConstant* RefUIMIC)
{
	B2_SCOPED_TRACK_LOG(TEXT("SetupMIDNonAtlasParamFromUIMIC"));

	if (!MIDToSet || !RefUIMIC) {
		return;
	}
	// 咆胶媚 颇扼固磐父 乐栏搁 凳.
	UTexture* ThisTextureParamValue = NULL;
	if (RefUIMIC->GetTextureParameterValue(MtrlParamName_NoAtlasSingleTexture, ThisTextureParamValue)) {
		MIDToSet->SetTextureParameterValue(MtrlParamName_NoAtlasSingleTexture, ThisTextureParamValue);
		MIDToSet->SetTextureParameterValue(MtrlParamName_UIAtlasTexture, nullptr); // 绢瞒乔 钎矫登瘤绰 臼摆瘤父 犬角洒 朝府绰 霸 粱 歹 亮阑 芭 鞍酒辑 
	}
}

void SetMIDScalarParam(class UMaterialInstanceDynamic* MIDToSet, const FName& ParamName, const float& ParamValue)
{
	if (!MIDToSet) {
		return;
	}

	MIDToSet->SetScalarParameterValue(ParamName, ParamValue);
}

void SetStageResultStar(class UMaterialInstanceDynamic* MIDToSet, const uint8 InStar)
{
	if (!MIDToSet)
	{
		return;
	}

	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo();
	if (ItemMiscInfo)
	{
		UMaterialInterface* StarNumberMtrl = ItemMiscInfo->GetStarNumberMtrl(InStar);
		if (StarNumberMtrl)
		{
			UMaterialInstanceConstant* ItemStarIconMIC_Ref = Cast<UMaterialInstanceConstant>(StarNumberMtrl);
			SetupMIDNonAtlasParamFromUIMIC(MIDToSet, ItemStarIconMIC_Ref);
		}
	}
}

int32 B2ServerRoundDown(double a, float NumDigit)
{
	// 辑滚俊辑 荤侩窍绰 RoundDown 傍侥
	return FPlatformMath::FloorToInt(a * FMath::Pow(10.f, NumDigit)) / FMath::Pow(10.f, NumDigit);
}

////////////////////////////////////////////////////
// Other miscellaneous utilities.

void GetTimeFormatFromRawSecond(int32 InRawSecond, int32& OutHour, int32& OutMin, int32& OutSec)
{
	OutHour = InRawSecond / 3600;

	int32 RemainingSecond = InRawSecond - OutHour * 3600; // 1矫埃 固父狼 唱赣瘤 檬

	OutMin = RemainingSecond / 60;
	OutSec = RemainingSecond % 60;
}

FVector GetGroundHitLocation(const FVector& BeginLocation, AActor* TargetActor)
{
	if (TargetActor && TargetActor->GetWorld())
	{
		UWorld* World = TargetActor->GetWorld();
		static const FVector DownVector = FVector(0.f, 0.f, -1.f);

		FHitResult HitResult;
		FCollisionQueryParams Params(TEXT("GroundCheckTrace"), false, TargetActor);
		bool bBlocked = World->LineTraceSingleByChannel(HitResult, BeginLocation, DownVector * 1000.f, ECollisionChannel::ECC_WorldStatic, Params);
		if (bBlocked && HitResult.Distance > 0.f)
		{
			return BeginLocation + DownVector * HitResult.Distance;
		}
	}

	return BeginLocation;
}

void SnapBladeIICharacterToGround(class ABladeIICharacter* InB2Char)
{
	B2_SCOPED_TRACK_LOG(TEXT("SnapBladeIICharacterToGround"));

	// bFixInAir敲贰弊肺 牢秦 公宏刚飘舶栏搁 菩胶
	if (!InB2Char->GetCharacterMovement()->bAutoActivate)
		return;

	// Refered to UEditorEngine::SnapObjectTo
	// It doesn't have to be highly precise. Could be rough snap at the moment of spawn. They will fall to the ground by gravity anyway.

	if (InB2Char)
	{
		FVector ActorBoundOrigin;
		FVector ActorBoundBoxExtent;
		// Sometimes it could be better to use RootComponent's Bounds. Character 俊 汲摹等 snapping 阑 规秦且 父茄 促弗 巴甸狼 康氢阑 临捞扁 困秦 风飘牢 Capsule component 俊辑 bound 蔼阑 啊廉咳.
		ActorBoundOrigin = InB2Char->GetCapsuleComponent()->Bounds.Origin;
		ActorBoundBoxExtent = InB2Char->GetCapsuleComponent()->Bounds.BoxExtent;
		FVector TraceStartLocation = ActorBoundOrigin;
		// Toward the bottom
		FVector TraceDirection = FVector(0.f, 0.f, -1.f);

		// If this LocationOffset is zero, Actor's pivot is at the center.
		FVector SnapLocationOffset = TraceStartLocation - InB2Char->GetActorLocation();

		// Do the actual actor->world check.  We try to collide against the world, straight down from our current position.
		// If we hit anything, we will move the actor to a position that lets it rest on the floor.
		FHitResult HitRes(1.0f);
		FCollisionQueryParams ColQueryParams(FName(TEXT("B2SnapActorToGroundTrace")), false);
		ColQueryParams.AddIgnoredActor(InB2Char);

		if (InB2Char->GetWorld()->SweepSingleByChannel(HitRes, TraceStartLocation, TraceStartLocation + TraceDirection*WORLD_MAX, FQuat::Identity, ECC_WorldStatic,
			FCollisionShape::MakeBox(ActorBoundBoxExtent), ColQueryParams))
		{
			FVector NewSnappedLocation = HitRes.Location - SnapLocationOffset;
			NewSnappedLocation.Z += KINDA_SMALL_NUMBER; // Move the new desired location up by an error tolerance

			InB2Char->SetActorLocation(NewSnappedLocation);
		}
	}
}

void SnapSkeletalMeshComponentToGround(class USkeletalMeshComponent* InMeshComp)
{
	B2_SCOPED_TRACK_LOG(TEXT("SnapSkeletalMeshComponentToGround"));

	AActor* MeshCompOwner = InMeshComp ? InMeshComp->GetOwner() : NULL;
	if (InMeshComp && MeshCompOwner && MeshCompOwner->GetWorld())
	{
		FVector CompBoundOrigin = InMeshComp->Bounds.Origin;
		FVector CompBoundBoxExtent = InMeshComp->Bounds.BoxExtent;
		FVector TraceStartLocation = CompBoundOrigin;
		// Toward the bottom
		FVector TraceDirection = FVector(0.f, 0.f, -1.f);

		// If this LocationOffset is zero, component's pivot is at the center.
		FVector SnapLocationOffset = TraceStartLocation - InMeshComp->GetComponentLocation();

		// Do the actual actor->world check.  We try to collide against the world, straight down from our current position.
		// If we hit anything, we will move the actor to a position that lets it rest on the floor.
		FHitResult HitRes(1.0f);
		FCollisionQueryParams ColQueryParams(FName(TEXT("B2SnapSKCompToGroundTrace")), false);
		ColQueryParams.AddIgnoredActor(InMeshComp->GetOwner());

		if (MeshCompOwner->GetWorld()->SweepSingleByChannel(HitRes, TraceStartLocation, TraceStartLocation + TraceDirection*WORLD_MAX, FQuat::Identity, ECC_WorldStatic,
			FCollisionShape::MakeBox(CompBoundBoxExtent), ColQueryParams))
		{
			FVector NewSnappedLocation = HitRes.Location - SnapLocationOffset;
			NewSnappedLocation.Z += KINDA_SMALL_NUMBER; // Move the new desired location up by an error tolerance

			InMeshComp->SetWorldLocation(NewSnappedLocation);
		}

	}
}

float ZOffsetFromMatchingSKActorToBIICharacter(class ABladeIICharacter* InB2Char)
{
	if (InB2Char)
	{
		return InB2Char->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}
	return 0.0f;
}

void EnsureActivateStaticallyAttachedPSCs(class AActor* InActor)
{
	// 捞扒 漂洒 敲饭捞绢 某腐磐俊 嘿篮 20/30肪 棺 6己 公扁 particle effect 啊 肋 救唱坷帮 窍绰 捞酱俊 措览窍扁 困秦 霖厚登菌澜.
	// AB2StageEventDirector::ActivateMatineePuppetPSCs 客 付蛮啊瘤 钙遏.
	// 窜鉴洒 Hidden 秦力 捞饶 Activate 甫 矫虐绰单 沥犬洒 公均捞 巩力登菌绰瘤绰 舅瘤 给茄促.

	/*if (InActor && InActor->IsValidObj())
	{
		TArray<UActorComponent*> AllPSC = InActor->GetComponentsByClass(UParticleSystemComponent::StaticClass());
		for (UActorComponent* ThisAC : AllPSC)
		{
			UParticleSystemComponent* ThisPSC = Cast<UParticleSystemComponent>(ThisAC);
			if (IsStaticallyVisiblePSC(ThisPSC))
			{
				EnsureActivatePSCInternal(ThisPSC);
			}
		}
	}*/
}

void EnsureActivatePSCInternal(UParticleSystemComponent* InPSC)
{ // 漂沥窍霸 荤侩窍绰 镑捞 乐澜.
	if (InPSC) {
		InPSC->SetVisibility(true);
		InPSC->SetHiddenInGame(false);
		InPSC->SecondsBeforeInactive = BIG_NUMBER; // 捞巴 锭巩俊 巩力啊 等 惑炔捞 捞傈俊 绢恫 康惑 纠俊辑 惯斑等 利篮 乐菌绰单 捞扒 傲 眠啊肺 档框捞 瞪鳖 酵绢辑..
		InPSC->Activate(true);
	}
}

bool IsStaticallyVisiblePSC(class UParticleSystemComponent* InPSC)
{ // 楷免捞唱 扁鸥狼 版快俊 visibility 勤甸傅 矫 贸府 格废俊 持阑 局甸. 捞霸 酒聪搁 anim notify 甫 烹秦 老矫利栏肺 嘿篮 吧肺 埃林.
	//return (InPSC && InPSC->Template
	//	&& InPSC->Template->IsLooping()
	//	&& InPSC->IsStaticVisible
	//	);
	return true;
}

bool IsActiveControlTutorial()
{
	return TutorialManager::GetInstance().IsCurrentTutorial(TutorialID_Occupy);
}

bool IsActiveFakeChannelMessage()
{
	return IsActiveControlTutorial();
}

// GameplayStatics.cpp 俊 乐绰 肺拿 蜡瓶 窃荐甫 啊廉咳. 老馆利牢 版快俊绰 UGameplayStatics::SpawnEmitter 揪府令甫 静档废.
UParticleSystemComponent* CreateParticleSystemHelper(UParticleSystem* EmitterTemplate, UWorld* World, AActor* Actor, bool bAutoDestroy)
{
	UParticleSystemComponent* PSC = NewObject<UParticleSystemComponent>((Actor ? Actor : (UObject*)World));
	PSC->bAutoDestroy = bAutoDestroy;
	// 割割 扁鸥 函荐甸篮 烙狼肺 盲况持绰促. 措眉肺 牢霸烙 dynamic spawn 俊 静扁 利例茄 汲沥牢扒啊..
	PSC->bAllowAnyoneToDestroyMe = true;
	PSC->SecondsBeforeInactive = 0.0f;
	PSC->bAutoActivate = false;
	PSC->SetTemplate(EmitterTemplate);
	PSC->bOverrideLODMethod = false;

	return PSC;
}

UCameraComponent* GetCurrentActiveCamComp(UObject* WorldContextObject)
{ // 泅犁 轰 坊歹傅俊 静捞绊 乐绰 墨皋扼 孽府. 抗甫 甸绢 老馆利牢 敲饭捞绢 傈捧 轰扼搁 敲饭捞绢俊 崔妨 乐绰 墨皋扼.
	//APlayerCameraManager* PCM = UGameplayStatics::GetLocalPlayerCameraManager(WorldContextObject);
	//AActor* CurrentViewTarget = PCM ? PCM->GetViewTarget() : NULL;
	//if (CurrentViewTarget)
	//{
	//	TInlineComponentArray<UCameraComponent*> ViewTargetCameras;
	//	CurrentViewTarget->GetComponents<UCameraComponent>(ViewTargetCameras);
	//	for (UCameraComponent* CurrCam : ViewTargetCameras)
	//	{
	//		if (CurrCam->bIsActive)
	//		{
	//			return CurrCam;
	//		}
	//	}
	//}
	return nullptr;
}

void ApplyNormallyExpectedInGameCamera(ABladeIIGameMode* InB2GM, ABladeIIPlayerController* InLocalPC, float InBlendInTime)
{
	//// 牢霸烙 傈捧 惑炔俊辑 泅犁 利例茄 墨皋扼 况农肺 倒酒哎 荐 乐档废 窍绰 扁瓷.
	//// 老馆利栏肺 捞吧 妮秦具 墨皋扼啊 利侩登绰 扒 酒聪绊 老矫利栏肺 漂荐茄 墨皋扼甫 借促啊 倒酒啊绰 殿狼 惑炔俊辑 镜 荐 乐促.

	//if (InB2GM && InLocalPC)
	//{
	//	check(InLocalPC == Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(InB2GM)));

	//	if (!InB2GM->ApplyActiveCameraIfAny(InBlendInTime)) // 漂沥 备开俊 汲摹茄 ActiveCamera 利侩 矫档.
	//	{
	//		// 迭洒 泅犁 利侩登绰 ActiveCamera 啊 绝促搁 岿靛 扁夯 墨皋扼肺.
	//		// 咯扁 喉坊爹 汲沥篮 蝶肺 啊绰单 BlendFunction 档 鞘夸窍搁 牢磊肺 逞败 林芭唱.. 捞 惑炔俊急 Cubic 捞 粮唱 利例秦 焊咯辑 媚持绢航.
	//		InLocalPC->ApplyWorldDefaultCamera(InBlendInTime, EViewTargetBlendFunction::VTBlend_Cubic);
	//	}
	//}
}

FText ClampTextLength(const FText& InSrcText, int32 InMaxLen, bool bAppendEllipsisIfClamped)
{
	B2_SCOPED_TRACK_LOG(TEXT("ClampTextLength"));

	FString TextString = InSrcText.ToString();
	const int32 SrcLen = TextString.Len();

	if (SrcLen > InMaxLen)
	{
		FString ClampedStr = TextString.LeftChop(SrcLen - InMaxLen);
		if (bAppendEllipsisIfClamped)
		{
			ClampedStr += TEXT("..."); // 富临烙钎 嘿捞绰 可记
		}
		return FText::FromString(ClampedStr);
	}
	else
	{
		return InSrcText;
	}
}

FText GetAutoFractionalFormattedText(float InNumber, int32 InMaxFracDigit)
{
	FNumberFormattingOptions NumberDisplayFormat;
	// 家荐痢 InMaxFracDigit 鳖瘤 倾侩窍登 场磊府 0 篮 蜡瓤箭磊肺 救 摹绊 积帆.
	NumberDisplayFormat.SetMinimumFractionalDigits(0);
	NumberDisplayFormat.SetMaximumFractionalDigits(InMaxFracDigit);

	return FText::AsNumber(InNumber, &NumberDisplayFormat);
}

FText GetAutoFractionalFormattedText_FirstFracCountOnly(float InCountNumber, float InMaxNumber, int32 InMaxFracDigit)
{
	// 蜡瓶捞扼扁焊促绰 漂沥茄 纳捞胶俊 荤侩窍绰 扁瓷牢单 荤侩且 镑捞 咯矾 镑 乐绢辑 咯扁俊 哗 初绰促.
	// 弥措 酿鸥烙捞 家荐痢栏肺 炼沥等 版快甫 钎矫窍扁 困秦 霉 家荐痢阑 墨款飘窍绰 悼救俊父 家荐痢阑 钎矫窍扁困茄 侩档. 拌加 钎矫窍搁 家荐痢捞 唱鸥车促 荤扼脸促 秦辑 沥脚绝促 焊聪

	const bool bIsCountingFirstFraction = (InMaxNumber > 0.0f && FMath::TruncToInt(InMaxNumber) == FMath::TruncToInt(InCountNumber)) ? true : false;
	return GetAutoFractionalFormattedText(InCountNumber, bIsCountingFirstFraction ? InMaxFracDigit : 0);
}

FSlateColor GetPowerColor(const int32 InCurrentPower, const int32 InRecommandPower)
{
	FSlateColor Result = FSlateColor(FLinearColor::Black);

	const float POWERSTANDARD = 0.05;

	const int32 PowerGap = InCurrentPower - InRecommandPower;
	const int32 GapValue = (int32)((float)InRecommandPower * POWERSTANDARD);

	const bool IsPositivenumber = PowerGap > 0;
	const bool IsNegativenumber = PowerGap < 0;
	const bool IsGap = FMath::Abs(PowerGap) >= GapValue;

	const bool IsHard = IsNegativenumber && IsGap;
	const bool IsEasy = IsPositivenumber && IsGap;

	if (IsHard)
	{
		Result = UB2UIManager::GetInstance()->TextColor_Hard;
	}
	else if (IsEasy)
	{
		Result = UB2UIManager::GetInstance()->TextColor_Esay;
	}
	else
	{
		Result = UB2UIManager::GetInstance()->TextColor_Normal;
	}

	return Result;
}

void SetLobbyUIHeaderTitle(const FText& InTitleText)
{
	auto* DocCondition = UB2UIDocHelper::GetDocUICondition();
	if (DocCondition)
	{
		DocCondition->SetHeaderTitle(InTitleText);
	}
}

void TryGatherAllBoundPSFromSKComp(class USkeletalMeshComponent* InSKComp, TMap<FName, UParticleSystem*>& OutAllFound)
{
	//// OutAllFound 甫 府悸窍瘤 臼绊 荤侩..

	//if (!InSKComp || !InSKComp->GetAnimInstance()) {
	//	return;
	//}

	//B2_SCOPED_TRACK_LOG(TEXT("TryGatherAllBoundPSFromSKComp"));
	//B2_SCOPED_TIME_LOG(FString::Printf(TEXT("TryGatherAllBoundPSFromSKComp %s"), InSKComp->GetOwner() ? *(InSKComp->GetOwner()->GetName()) : *InSKComp->GetName()));

	//// 利例洒 檬扁拳啊 等 AnimInstance 俊辑父 冈鳃 巴捞促.
	//UAnimInstance* TheAnimInst = InSKComp->GetAnimInstance();

	//TArray<UAnimationAsset*> AllPlayableAnims;
	//TheAnimInst->GetAllAnimAssets(AllPlayableAnims);

	//for (UAnimationAsset* ThisAnim : AllPlayableAnims)
	//{
	//	// BlendSpace 甫 持绢具 窍绰单., 老窜 夸 窃荐狼 荤侩 格利 惑 角龙利栏肺 狼固乐绰 ParticleSystem 甸篮 窜老 AnimSequence 俊 甸绢啊 乐阑 淀.
	//	UAnimSequenceBase* ThisAnimSeq = Cast<UAnimSequenceBase>(ThisAnim);
	//	TryGatherAllBoundPSFromAnimSeq(ThisAnimSeq, OutAllFound);
	//}
}

void TryGatherAllBoundPSFromAnimSeq(UAnimSequenceBase* InAnimSeq, TMap<FName, UParticleSystem*>& OutAllFound)
{
	if (!InAnimSeq) {
		return;
	}

	for (FAnimNotifyEvent& ThisNotifyEvt : InAnimSeq->Notifies)
	{
		UParticleSystem* FoundPS = CheckNotifyEventForParticleSystemUse(ThisNotifyEvt);
		if (FoundPS)
		{ // 捞固 眠啊等 芭 酒囱瘤 八荤. TArray 肺 AddUnique 窍搁 逞 蠢副鳖毫 PathName 阑 Key 肺 茄 甘栏肺..
			FName ThisPSKey(*FoundPS->GetPathName());
			UParticleSystem** FoundOne = OutAllFound.Find(ThisPSKey);
			if (!FoundOne)
			{
				OutAllFound.Add(ThisPSKey, FoundPS);
			}
		}
	}
}

UParticleSystem* CheckNotifyEventForParticleSystemUse(FAnimNotifyEvent& InNotifyEvent)
{
	// ParticleSystem 阑 荤侩窍绰 辆幅狼 AnimNotify(State) 肺.. 老窜 酒绰 扒 滴啊瘤..
	UAnimNotify_PlayParticleEffect* CastedPPX = Cast<UAnimNotify_PlayParticleEffect>(InNotifyEvent.Notify);
	if (CastedPPX) {
		return CastedPPX->PSTemplate;
	}
	else
	{
		UAnimNotifyState_TimedParticleEffect* CastedTPX = Cast<UAnimNotifyState_TimedParticleEffect>(InNotifyEvent.NotifyStateClass);
		if (CastedTPX) {
			return CastedTPX->PSTemplate;
		}
	}
	return NULL;
}

/** 茄锅 龋免俊 茄 橇饭烙 弊妨咙. 鞘夸茄 扁埃 悼救 概 橇饭烙 龋免 */
void DrawDebugText(float InStartX, float InStartY, const FString& InText, const FLinearColor& InFontColor, int32 InFontSize, bool bDrawShadow, const FLinearColor& InShadowColor)
{
#if WITH_BII_ON_SCREEN_DEBUG_TEXT
	if (UB2UIManager::IsInstanceNull()) { // 咯扁辑 GetInstance 甫 且 锭俊 积己捞 登档废 窍瘤 臼绰促.
		return;
	}

	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	UB2DebugTextWidget* ScreenDebugTextWidget = UIMgr ? UIMgr->GetScreenDebugTextWidget() : NULL;
	if (ScreenDebugTextWidget)
	{
		ScreenDebugTextWidget->EnqueueDrawText(InStartX, InStartY, InText, InFontColor, InFontSize, bDrawShadow, InShadowColor);
	}
#endif
}

#if WITH_BII_ON_SCREEN_DEBUG_TEXT
/** 捞扒 茄锅父 妮窍搁 沥秦柳 矫埃悼救 叼胶敲饭捞 等促. 措脚 困摹绰 沥秦廉 乐澜 */
void DrawTimedDebugText(const FString& InText, const FLinearColor& InFontColor, int32 InFontSize, float InDisplayTime, bool bDrawShadow, const FLinearColor& InShadowColor)
{
	if (UB2UIManager::IsInstanceNull()) { // 咯扁辑 GetInstance 甫 且 锭俊 积己捞 登档废 窍瘤 臼绰促. 漂洒 FB2ScopedCallTracker 俊 荤侩登扁 锭巩俊 UIManager 啊 Unload 等 矫痢俊档 阂府霸 瞪 荐 乐促.
		return;
	}

	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	UB2DebugTextWidget* ScreenDebugTextWidget = UIMgr ? UIMgr->GetScreenDebugTextWidget() : NULL;
	if (ScreenDebugTextWidget)
	{
		ScreenDebugTextWidget->AddTimedDrawText(InText, InFontColor, InFontSize, InDisplayTime, bDrawShadow, InShadowColor);
	}
}
#endif

bool IsUsingMobileRendering(UWorld* InWorld)
{
//#if PLATFORM_ANDROID || PLATFORM_IOS
	return true;
//#else
//	// 咯扁 吧府搁 葛官老 橇府轰. 割割 霸烙惑狼 坊歹傅 橇府轰 殿阑 困茄 芭扼 肋给等促绊 秦辑 弥辆 扁瓷俊 康氢阑 固摹绰 扒 酒聪促.
//	ERHIFeatureLevel::Type RHIFeatureLevel = (InWorld && InWorld->Scene) ? InWorld->Scene->GetFeatureLevel() : ERHIFeatureLevel::Num;
//	return (RHIFeatureLevel == ERHIFeatureLevel::ES2 || RHIFeatureLevel == ERHIFeatureLevel::ES3_1);
//#endif
}

void TurnOffMeshComponentsDyamicShadowForModulated(AActor* InOwnerActor)
{
	//////////////////////////////////////
	// InteractiveActor 客 鞍捞 BladeII 磊眉利牢 窍困俊 movable MeshComponent 甸捞 乐绰 Actor 甫 困茄 dynamic shadow 加己 牧飘费 蜡瓶.
	// Modulated shadow 啊 粱 巩力啊 乐绢辑 迭洒 鞘夸档 绝绊 巩力父 老栏懦 家瘤啊 乐绰 巴甸篮 shadow 甫 靛府快瘤 臼档废 窍扁 困窃.
	// Modulated shadow 甫 措眉窍绰 宝满篮 荐窜捞 唱坷扁 傈鳖瘤..
	// 辟单 硅版捞 static lighting 俊 扁馆窍绊 乐绰 茄 绢瞒乔 modulated shadow 寇俊 利例茄 措救阑 茫扁档 绢妨框 せ
	//////////////////////////////////////

	if (InOwnerActor && ShouldUsePerObjectModulatedShadow(InOwnerActor->GetWorld()))
	{
		TArray<UActorComponent*> AllMeshComps;
		InOwnerActor->GetComponents(AllMeshComps);
		for (UActorComponent* ThisActorComp : AllMeshComps)
		{
			UMeshComponent* ThisMeshComp = Cast<UMeshComponent>(ThisActorComp);
			if (ThisMeshComp && ThisMeshComp->Mobility == EComponentMobility::Movable && ThisMeshComp->bCastDynamicShadow)
			{
				ThisMeshComp->bCastDynamicShadow = false;
				ThisMeshComp->MarkRenderStateDirty();
			}
		}
	}
}
void ForceSendReallocateRenderTargetsSignal()
{
	// 叼官捞胶俊辑 埃趣 啊促 焊捞绰 坊歹傅 滚弊俊 措贸窍绰 瞒盔俊辑..
	// 父电 矫痢俊 捞霸 角力肺 瓤苞啊 乐绰瘤绰 葛抚. 犁泅捞 登绊 抛胶飘秦辑 冈洒搁 利例茄 鸥捞怪俊 荤侩窍霸 登绰 芭..

	IConsoleVariable* CVarSendSignal = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ReallocateRenderTargetsSignal"));
	if (CVarSendSignal)
	{ // 窜鉴洒 窍唱 刘啊矫挪 吧 矫弊澄肺 靖 ぱぱ;
		CVarSendSignal->Set(CVarSendSignal->GetInt() + 1, ECVF_SetByCode);
	}
}

namespace Utilities
{
	FConnectionState& FConnectionState::GetInstance()
	{
		static FConnectionState	inst;
		return inst;
	}

	void FConnectionState::Initialize()
	{
		SessionCloseReason = ESessionCloseReason::ESessionCloseReason_None;
		data_trader::ServiceConnectingEventClass<>::GetInstance().Subscribe(
			[this]()
			{
				this->SetState(CONNECTION_STATE_CONNECTING);
			}
		);
		data_trader::ServiceConnectedEventClass<>::GetInstance().Subscribe(
			[this]()
		{
			this->SetState(CONNECTION_STATE_CONNECTED);

			//UB2DisconnectLogSender* Sender = UB2DisconnectLogSender::GetInstance();
			//if (Sender != nullptr)
			//{
			//	Sender->Init(FString::Printf(TEXT("%lld"), FB2NetworkBridge::GetInstance()->GetAccountID()));
			//	Sender->SendRemainDisconnectedLog();
			//}
		}
		);
		data_trader::ServiceNotAvailableEventClass<int32>::GetInstance().Subscribe(
			[this](int32 error)
		{
			// 技记 辑滚客 立加捞 谗辫
			this->SetState(CONNECTION_STATE_SERVICENOTAVAILABLE);

			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			if (UIMgrInst)
			{
				if (this->GetSessionCloseReason() == ESessionCloseReason::ESessionCloseReason_None)
				{
					data_trader::DisconnectReasonLogEventClass<int32>::GetInstance().Signal(error);
					auto* gameMode = GetBladeIIGameMode(UIMgrInst);

					if (gameMode && gameMode->GetB2GameModeType() == EB2GameMode::PVP_Tag) //PVP Matching老锭父 肺厚肺 焊郴扁 困秦 抗寇 贸府. 窃荐 捞抚捞 救绢匡府瘤父....
						FBladeIIBlockToSyncNetwork::GetInstance().ChannelDisconnect(error);
					else
						FBladeIIBlockToSyncNetwork::GetInstance().ServiceNotAvailable(error);
				}
			}
		}
		);
		data_trader::ChannelDisconnectedEventClass<int32>::GetInstance().Subscribe(
			[this](int32 error)
		{
			// TODO: 盲澄 辑滚客 楷搬 谗辫, PVP/Raid/Assault 殿 包访 霸烙 葛靛 吝窜凳
			data_trader::DisconnectReasonLogEventClass<int32>::GetInstance().Signal(error);
			FBladeIIBlockToSyncNetwork::GetInstance().ChannelDisconnect(error);
		}
		);
		data_trader::DisconnectReasonLogEventClass<int32>::GetInstance().Subscribe(
			[this](int32 InErrorCode)
		{
			//if (InErrorCode == 0)
			//	return;
			//
			//EDisconnectType type;
			//if (ExtractNthDigitR<4>(InErrorCode) == ExtractNthDigitR<4>((int32)b2network::ErrorCode::SessionConnectError))
			//	type = EDisconnectType::Session;
			//else if (ExtractNthDigitR<4>(InErrorCode) == ExtractNthDigitR<4>((int32)b2network::ErrorCode::ChannelConnectError))
			//	type = EDisconnectType::Channel;
			//else
			//	return;
			//
			//UB2DisconnectLogSender* Sender = UB2DisconnectLogSender::GetInstance();
			//if (Sender != nullptr)
			//	Sender->DiscconectedLog(type, InErrorCode, TEXT(""));
		}
		);
		data_trader::RequireUpdateProtocolEventClass<>::GetInstance().Subscribe(
			[this]()
		{
#if !UE_BUILD_SHIPPING && WITH_EDITOR
			if (GIsEditor) return;
#endif // WITH_EDITOR

			//[@AKI, 170619] 咯扁绰 咯矾锅 甸绢 棵荐 勒栏骨肺 茄锅父 甸绢 坷霸 窃
			if (this->GetSessionCloseReason() == ESessionCloseReason::ESessionCloseReason_None)
			{
				this->SetSessionCloseReason(ESessionCloseReason::ESessionCloseReason_UpdateProtocolEvent);

				UB2UIManager* UIMgr = UB2UIManager::GetInstance();
				if (UIMgr)
				{
					UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
						BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
						BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("NeedtoUpdateClient")),
						0.f,
						true,
						true,
						EUIMsgPopupButtonGroup::Confirm,
						FMsgPopupOnClick::CreateLambda([]()
					{
						exit(0);
					}
						)
						);
				}
				else	exit(0);
			}
		}
		);
		data_trader::RequireUpdateAppVersionEventClass<>::GetInstance().Subscribe(
			[this]()
		{
#if !UE_BUILD_SHIPPING && WITH_EDITOR
			if (GIsEditor) return;
#endif // WITH_EDITOR

			DeliveryAppUpdatePopUpClass<>::GetInstance().Signal();
		}
		);
		data_trader::ServerAddressErrorEventClass<>::GetInstance().Subscribe(
			[this]()
		{
#if !UE_BUILD_SHIPPING && WITH_EDITOR
			if (GIsEditor) return;
#endif // WITH_EDITOR
			UB2UIManager* UIMgr = UB2UIManager::GetInstance();
			if (UIMgr)
			{
				UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
					BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
					BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("GameServerAddressError")),
					0.f,
					true,
					true,
					EUIMsgPopupButtonGroup::Confirm,
					FMsgPopupOnClick::CreateLambda([]()
				{
					exit(0);
				}
					)
					);
			}
			else	exit(0);
		}
		);
		data_trader::SessionClosedNotifyClass<int32>::GetInstance().Subscribe(
			[this](int32 _reason)
		{
			this->SetSessionCloseReason(ESessionCloseReason::ESessionCloseReason_ClosedNotify);

			FBladeIIBlockToSyncNetwork::GetInstance().SessionDisconnetFromServer(_reason);
		}
		);

		data_trader::ServerTimeSyncEventClass<int64>::GetInstance().Subscribe(
			[this](const int64 InServerTime)
		{
			UB2GameInstance::SetUTCNow(InServerTime);
		}
		);


	}

	void FConnectionState::Uninitialize()
	{
		data_trader::ServiceConnectingEventClass<>::GetInstance().UnsubscribeAll();
		data_trader::ServiceConnectedEventClass<>::GetInstance().UnsubscribeAll();
		data_trader::ServiceNotAvailableEventClass<int32>::GetInstance().UnsubscribeAll();
		data_trader::ChannelDisconnectedEventClass<int32>::GetInstance().UnsubscribeAll();
		data_trader::RequireUpdateProtocolEventClass<>::GetInstance().UnsubscribeAll();
		data_trader::RequireUpdateAppVersionEventClass<>::GetInstance().UnsubscribeAll();
		data_trader::ServerAddressErrorEventClass<>::GetInstance().UnsubscribeAll();
		data_trader::SessionClosedNotifyClass<int32>::GetInstance().UnsubscribeAll();
		data_trader::ServerTimeSyncEventClass<int64>::GetInstance().UnsubscribeAll();
		data_trader::DisconnectReasonLogEventClass<int32>::GetInstance().UnsubscribeAll();
	}
}

EB2GameMode GetB2GameModeType(UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
		WorldContextObject = UB2UIManager::GetInstance();

	if (WorldContextObject == nullptr)
		return EB2GameMode::Unknown;

	auto* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	return B2GameMode ? B2GameMode->GetB2GameModeType() : EB2GameMode::Unknown;
}

ABladeIIGameMode* GetBladeIIGameMode(UObject* WorldContextObect)
{
	if (WorldContextObect)
		return Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObect));
	else
		return nullptr;
}

bool m_bInvenQuickCheck = false;
void SetInvenQuickSlot(bool bSet)
{
	m_bInvenQuickCheck = bSet;
}

bool GetInvenQuickSlot()
{
	bool bTempCheck;
	bTempCheck = m_bInvenQuickCheck;
	m_bInvenQuickCheck = false;
	return bTempCheck;
}

bool m_bVibration = true;

void SetVibration(bool bSet)
{
	m_bVibration = bSet;
}

bool IsVibration()
{
	return m_bVibration;
}

bool m_bViewVibBtn = true;
void SetViewVibBtn(bool bSet)
{
	m_bViewVibBtn = bSet;
}
bool IsViewVibBtn()
{
	return m_bViewVibBtn;
}

// view Pormotion

bool m_bViewPromotion = false;

void SetViewPromotion(bool bSet)
{
	m_bViewPromotion = bSet;
}

bool IsViewPromotion()
{
	return m_bViewPromotion;
}

bool m_bPurchasesInit = false;
void SetPurchasesInit(bool bSet)
{
	m_bPurchasesInit = bSet;
}

bool IsInitPurchases()
{
	return m_bPurchasesInit;
}

void SetDevModeContent(bool IsDevMode)
{
	if (GConfig)
		GConfig->SetInt(TEXT("DevSetting"), *FString::Printf(TEXT("DevClientMode")), IsDevMode, GB2GeneralSavedStateIni);
}

bool IsDevModeContent()
{
	bool bRetVal = false;
	if (GConfig)
		GConfig->GetBool(TEXT("DevSetting"), TEXT("DevClientMode"), bRetVal, GGameIni);
	return bRetVal;
}

void SetClearNewTutorial(bool IsClear)
{
	if (GConfig)
	{
		GConfig->SetBool(TEXT("NewTutorial"), *FString::Printf(TEXT("ClearTutorial")), IsClear, GB2UserSavedStateIni);
		GConfig->Flush(false, GB2UserSavedStateIni);
	}
}

bool IsClearNewTutorial()
{
	bool bRetVal = false;
	if (GConfig)
		GConfig->GetBool(TEXT("NewTutorial"), TEXT("ClearTutorial"), bRetVal, GB2UserSavedStateIni);
	return bRetVal;
}

void SetTrueOpenPopupAppStoreReview()
{
	if (GConfig)
	{
		GConfig->SetBool(TEXT("PopupAppStoreReview"), *FString::Printf(TEXT("Open")), true, GB2UserSavedStateIni);
		GConfig->Flush(false, GB2UserSavedStateIni);
	}
}

bool IsOpenPopupAppStoreReview()
{
	bool bRetVal = false;
	if (GConfig)
		GConfig->GetBool(TEXT("PopupAppStoreReview"), TEXT("Open"), bRetVal, GB2UserSavedStateIni);
	return bRetVal;
}

void SaveGameData_KakaoInvitedFriends(const TArray<FString> &SaveList)
{
	if (GConfig)
	{
		GConfig->SetArray(TEXT("KakaoInvitedFriends"), *FString::Printf(TEXT("Friend")), SaveList, GB2UserSavedStateIni);
		GConfig->Flush(false, GB2UserSavedStateIni);
	}
}

bool LoadGameData_KakaoInvitedFriends(TArray<FString> &OutList)
{
	bool bRetVal = false;
	if (GConfig)
		bRetVal = GConfig->GetArray(TEXT("KakaoInvitedFriends"), TEXT("Friend"), OutList, GB2UserSavedStateIni) > 0 ? true : false;
	return bRetVal;
}

// [GDC] Feb 2017 temporary. 
// GDC 捞饶浚 捞芭 荤侩茄 何盒 祸免秦郴辑 葛滴 绝矩促. 傲 快府内靛俊辑 GDC 八祸秦辑 唱坷绰 芭 促 眉农.
bool ShouldShowGDCVersionContent()
{
	bool bRetVal = false;
	if (GConfig)
	{
		GConfig->GetBool(TEXT("/Script/BladeII.BladeIIReadyVersion"), TEXT("GDCVersionReady"), bRetVal, GGameIni);
	}
	return bRetVal;
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
/** CBTVersionReady 啊 true 牢 惑怕肺 捻欧 抛胶飘且 锭 牧刨明啊 阜囚辑 寸炔胶矾款 惑炔 措厚.
//* 捞吧 函版窍绰 能贾 目盖靛啊 乐澜. */
bool gbAllowNonCBTVersionContentOverride = false;
#endif
bool (ShouldShowCBTVersionContent)()
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (gbAllowNonCBTVersionContentOverride)
	{
		return false;
	}
#endif

	bool bRetVal = false;

	if (GConfig)
	{
		GConfig->GetBool(TEXT("/Script/BladeII.BladeIICBTVersion"), TEXT("CBTVersionReady"), bRetVal, GGameIni);
	}

	return bRetVal;
}

bool ChangeGuildBattleId(int32 SeasonID, int32 turnID)
{
	int32 bRetVal = 0;
	int32 GuildBattleID = SeasonID * 100000 + turnID;
	if (GConfig)
	{
		GConfig->GetInt(TEXT("GuildInfo"), TEXT("GuildBattleID"), bRetVal, GB2UserSavedStateIni);
		GConfig->SetInt(TEXT("GuildInfo"), TEXT("GuildBattleID"), GuildBattleID, GB2UserSavedStateIni);

		if (bRetVal != GuildBattleID)
			return true;
	}

	return false;
}

void SaveGameSetting_Graphics(int32 InValue)
{
	// 咯扁俊 蝶扼 角龙利栏肺 利侩登绰 柯爱 浚柳 弊贰侨 可记甸篮 B2Scalability 甫 曼绊
	if (GConfig) {
		GConfig->SetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameGraphicLevel")), InValue, GB2GeneralSavedStateIni);
	}
}
void SaveGameSetting_FrameLimit(int32 InValue)
{
	if (GConfig) {
		GConfig->SetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameFrameLimitLevel")), InValue, GB2GeneralSavedStateIni);
	}
}
void SaveGameSetting_Resolution(int32 InValue)
{
	if (GConfig) {
		GConfig->SetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameResolutionLevel")), InValue, GB2GeneralSavedStateIni);
	}
}
void SaveGameSetting_BGMVolume(float InValue)
{
	if (GConfig) {
		GConfig->SetFloat(TEXT("GameSetting"), *FString::Printf(TEXT("GameBGMVolume")), InValue, GB2GeneralSavedStateIni);
	}
}
void SaveGameSetting_EffectVolume(float InValue)
{
	if (GConfig) {
		GConfig->SetFloat(TEXT("GameSetting"), *FString::Printf(TEXT("GameEffectVolume")), InValue, GB2GeneralSavedStateIni);
	}
}
void SaveGameSetting_Vibration(int32 InValue)
{
	if (GConfig) {
		GConfig->SetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameVibrationSet")), InValue, GB2GeneralSavedStateIni);
	}
}
void SaveGameSetting_KeepScreenOn(int32 InValue)
{
	if (GConfig) {
		GConfig->SetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameKeepScreenSet")), InValue, GB2GeneralSavedStateIni);
	}
}
void SaveGameSetting_GamePush(int32 InValue)
{
	if (GConfig) {
		GConfig->SetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GamePush")), InValue, GB2GeneralSavedStateIni);
	}
}
void SaveGameSetting_kakaoDayPush(int32 InValue)
{
	if (GConfig) {
		GConfig->SetInt(TEXT("GameSetting"), *FString::Printf(TEXT("KakaoDayPush")), InValue, GB2GeneralSavedStateIni);
	}
}
void SaveGameSetting_KakaoNightPush(int32 InValue)
{
	if (GConfig) {
		GConfig->SetInt(TEXT("GameSetting"), *FString::Printf(TEXT("KakaoNightPush")), InValue, GB2GeneralSavedStateIni);
	}
}

void SaveGameSetting_SoundLocType(int32 InValue)
{
	if (GConfig)
	{
		GConfig->SetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameSoundLocType")), InValue, GB2GeneralSavedStateIni);
		GConfig->Flush(false, GB2GeneralSavedStateIni);
	}
}

bool LoadGameSetting_Graphics(int32& OutValue)
{
	// 咯扁俊 蝶扼 角龙利栏肺 利侩登绰 柯爱 浚柳 弊贰侨 可记甸篮 B2Scalability 甫 曼绊
	// EB2GraphicsLevel enum 鉴辑 官操绰 客吝俊.. 汲沥 货肺 肺爹窍档废 "GameGraphicLevel_N" 栏肺 官厕. 老沥 矫埃 瘤唱绊 唱辑 促矫 "GameGraphicLevel" 肺 登倒副 巴.
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameGraphicLevel")), OutValue, GB2GeneralSavedStateIni));
}
bool LoadGameSetting_Graphics_OrByDefault(int32& OutValue)
{
	if (!LoadGameSetting_Graphics(OutValue)) {
		// 酒流 荤侩磊 急琶捞 绝菌促搁 DeviceProfile 俊 狼秦 detect 等 浚柳 scalability 矫胶袍 扁夯 汲沥蔼栏肺.
		OutValue = B2GraphicsLevelToInt(B2Scalability::GetGraphicsLevelByExpectedScalability());
		return false; // 咯窍埃 历厘等 荤侩磊 急琶蔼篮 绝菌澜阑 府畔窃.
	}
	return true;
}
bool LoadGameSetting_FrameLimit(int32& OutValue)
{
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameFrameLimitLevel")), OutValue, GB2GeneralSavedStateIni));
}
bool LoadGameSetting_FrameLimit_OrByDefault(int32& OutValue)
{
	if (!LoadGameSetting_FrameLimit(OutValue)) {
		// 酒流 荤侩磊 汲沥 历厘捞 绝菌促搁 DeviceProfile 俊 狼秦 甸绢啊 乐阑 泅犁 蔼..
		OutValue = B2FrameLevelToInt(B2Scalability::GetDefaultRelativeFrameLimitLevel());
		return false; // 咯窍埃 历厘等 荤侩磊 急琶蔼篮 绝菌澜阑 府畔窃.
	}
	return true;
}
bool LoadGameSetting_Resolution(int32& OutValue)
{
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameResolutionLevel")), OutValue, GB2GeneralSavedStateIni));
}
bool LoadGameSetting_Resolution_OrByDefault(int32& OutValue)
{
	if (!LoadGameSetting_Resolution(OutValue)) {
		// 酒流 荤侩磊 汲沥 历厘捞 绝菌促搁 DeviceProfile 俊 狼秦 甸绢啊 乐阑 扁夯 蔼.
		OutValue = B2ResolutionLevelToInt(B2Scalability::GetResolutionLevelOfRQType(GetDefaultGraphicsRQType()));
		return false; // 咯窍埃 历厘等 荤侩磊 急琶蔼篮 绝菌澜阑 府畔窃.
	}
	return true;
}

bool LoadGameSetting_BGMVolume(float& OutValue)
{
	return (GConfig && GConfig->GetFloat(TEXT("GameSetting"), *FString::Printf(TEXT("GameBGMVolume")), OutValue, GB2GeneralSavedStateIni));
}
bool LoadGameSetting_EffectVolume(float& OutValue)
{
	return (GConfig && GConfig->GetFloat(TEXT("GameSetting"), *FString::Printf(TEXT("GameEffectVolume")), OutValue, GB2GeneralSavedStateIni));
}
bool LoadGameSetting_Vibration(int32& OutValue)
{
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameVibrationSet")), OutValue, GB2GeneralSavedStateIni));
}

bool LoadGameSetting_KeepScreenOn(int32& OutValue)
{
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameKeepScreenSet")), OutValue, GB2GeneralSavedStateIni));
}
bool LoadGameSetting_GamePush(int32& OutValue)
{
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GamePush")), OutValue, GB2GeneralSavedStateIni));
}

bool LoadGameSetting_KakaoDayPush(int32& OutValue)
{
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("KakaoDayPush")), OutValue, GB2GeneralSavedStateIni));
}

bool LoadGameSetting_KakaoNightPush(int32& OutValue)
{
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("KakaoNightPush")), OutValue, GB2GeneralSavedStateIni));
}

bool LoadGameSetting_SoundLocType(int32& OutValue)
{
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameSoundLocType")), OutValue, GB2GeneralSavedStateIni));
}

void ReserveResolutionLevelForNextAppStart(EB2ResolutionLevel InReserveLevel)
{
	if (GConfig)
	{
		if (InReserveLevel != EB2ResolutionLevel::End)
		{
			int32 CastedSaveValue = B2ResolutionLevelToInt(InReserveLevel);
			check(CastedSaveValue >= 0); // 澜荐绰 努府绢 等 吧肺 牢侥且 巴.
			GConfig->SetInt(TEXT("GameSetting"), TEXT("ReservedResolutionLevel"), CastedSaveValue, GB2GeneralSavedStateIni);
		}
		else
		{ // 捞扒 努府绢俊 秦寸.
			GConfig->SetInt(TEXT("GameSetting"), TEXT("ReservedResolutionLevel"), -1, GB2GeneralSavedStateIni);
		}
	}
}
EB2ResolutionLevel LoadReservedResolutionLevelOfPrevAppRun()
{
	if (GConfig)
	{
		int32 LoadedValue = B2ResolutionLevelToInt(EB2ResolutionLevel::Mid);
		if (GConfig->GetInt(TEXT("GameSetting"), TEXT("ReservedResolutionLevel"), LoadedValue, GB2GeneralSavedStateIni))
		{
			if (LoadedValue >= 0)
			{
				return IntToB2ResolutionLevel(LoadedValue);
			}
		}
	}
	return EB2ResolutionLevel::End; // 捞扒 reserve 救 登绢 乐促绰 狼固肺.
}

void InitializeAllGameSettingData(UObject* WorldContextObject, bool bExtraContentScaleFallback)
{
	// High-level 霸烙 技泼 单捞磐甸阑 佬绢甸捞芭唱, 绝栏搁 檬扁蔼栏肺 历厘 棺 利侩.
	// 霸烙 胶鸥飘诀, 郴瘤绰 霉 BladeIIGameMode 矫累 矫痢俊
	bool bSavedSome = false;

	int32 LoadedGraphicsLevel = B2GraphicsLevelToInt(EB2GraphicsLevel::GraphicsLevel_HIGH);
	if (!LoadGameSetting_Graphics_OrByDefault(LoadedGraphicsLevel))
	{
		// 捞扒 肺爹 角菩 矫 扁夯蔼阑 咯扁辑 持阑 鞘夸绰 绝促. LoadGameSetting_Graphics_OrByDefault 俊辑 贸府瞪 巴.
		SaveGameSetting_Graphics(LoadedGraphicsLevel);
		bSavedSome = true;
	}
	int32 LoadedFrameLimitLevel = B2FrameLevelToInt(EB2FrameLimitLevel::Mid); // 绢瘤埃窍搁 捞扒 High 甫 扁夯蔼栏肺 救 窍绰 霸 亮阑波促 偿偿
	if (!LoadGameSetting_FrameLimit_OrByDefault(LoadedFrameLimitLevel))
	{
		// 捞扒 肺爹 角菩 矫 扁夯蔼阑 咯扁辑 持阑 鞘夸绰 绝促. LoadGameSetting_FrameLimit_OrByDefault 俊辑 贸府瞪 巴.
		SaveGameSetting_FrameLimit(LoadedFrameLimitLevel);
		bSavedSome = true;
	}
	int32 LoadedResolutionLevel = B2ResolutionLevelToInt(EB2ResolutionLevel::Mid);  // 绢瘤埃窍搁 捞扒 High 甫 扁夯蔼栏肺 救 窍绰 霸 亮阑波促 洒洒
	if (!LoadGameSetting_Resolution_OrByDefault(LoadedResolutionLevel))
	{
		// 捞扒 肺爹 角菩 矫 扁夯蔼阑 咯扁辑 持阑 鞘夸绰 绝促. LoadGameSetting_Resolution_OrByDefault 俊辑 贸府瞪 巴.
		SaveGameSetting_Resolution(LoadedResolutionLevel);
		bSavedSome = true;
	}
	// 捞傈 角青俊辑 繁鸥烙 胶困莫捞 救登绰 秦惑档 函版阑 茄 版快. 喊档 鞘靛俊 历厘登绢 乐绰 吧 利侩.
	EB2ResolutionLevel ReservedResLevel = LoadReservedResolutionLevelOfPrevAppRun();
	if (ReservedResLevel != EB2ResolutionLevel::End && IntToB2ResolutionLevel(LoadedResolutionLevel) != ReservedResLevel)
	{
		UE_LOG(LogBladeII, Log, TEXT("Applying reserved resolution level %d (Was loaded as %d)"), B2ResolutionLevelToInt(ReservedResLevel), LoadedResolutionLevel);
		LoadedResolutionLevel = B2ResolutionLevelToInt(ReservedResLevel);
		SaveGameSetting_Resolution(LoadedResolutionLevel);
		ClearResolutionLevelReserve();
		bSavedSome = true;
	}

	float LoadedBGMVolume = 1.0f;
	if (!LoadGameSetting_BGMVolume(LoadedBGMVolume))
	{
		LoadedBGMVolume = 0.7f; // 扁夯蔼
		SaveGameSetting_BGMVolume(LoadedBGMVolume);
		bSavedSome = true;
	}
	float LoadedFxVolume = 1.0f;
	if (!LoadGameSetting_EffectVolume(LoadedFxVolume))
	{
		LoadedFxVolume = 0.7f; // 扁夯蔼
		SaveGameSetting_EffectVolume(LoadedFxVolume);
		bSavedSome = true;
	}
	int32 LoadedVibrationScale = 1;
	if (!LoadGameSetting_Vibration(LoadedVibrationScale))
	{
		LoadedVibrationScale = 1; // 扁夯蔼
		SaveGameSetting_Vibration(LoadedVibrationScale);
		bSavedSome = true;
	}

	int32 LoadedKeepScreenOn = 1;
	if (!LoadGameSetting_KeepScreenOn(LoadedKeepScreenOn))
	{
		LoadedKeepScreenOn = 1; // 扁夯蔼
		SaveGameSetting_KeepScreenOn(LoadedKeepScreenOn);
		bSavedSome = true;
	}

	int32 LoadGamePush = 1;
	if (!LoadGameSetting_GamePush(LoadGamePush))
	{
		LoadedKeepScreenOn = 1; // 扁夯蔼
		SaveGameSetting_GamePush(LoadGamePush);
		bSavedSome = true;
	}

	//int32 LoadGameSoundLocType = B2SoundLocalizeType::Default_Type();
	//if (!LoadGameSetting_SoundLocType(LoadGameSoundLocType))
	//{
	//	SaveGameSetting_SoundLocType(LoadGameSoundLocType);
	//	bSavedSome = true;
	//}
	//FGenericPlatformMisc::SetSoundLocType(LoadGameSoundLocType);

	// 浚柳 檬扁拳 矫痢俊 阂府绰 镑捞扁档 窍绊 包访等 内靛扁档 窍聪 促弗 葛碘俊辑 荤侩阑 困秦 窃荐 器牢磐 且寸. 
	gLoadGameFxVolumeSettingFnPtr = LoadGameSetting_EffectVolume;
	gLoadGameBGMVolumeSettingFnPtr = LoadGameSetting_BGMVolume;

	if (bSavedSome)
	{
		GConfig->Flush(false, GB2UserSavedStateIni);
		GConfig->Flush(false, GB2GeneralSavedStateIni);
	}

	// WorldContextObject 绰 霸烙葛靛 喊 bias 甫 困茄 芭扼 葛碘 矫累捞唱 DLCFront 俊辑 阂府绰 芭搁 鞘夸啊 绝促.
	B2Scalability::AdjustScalabilityBySelectedLevel(WorldContextObject, LoadedGraphicsLevel, LoadedResolutionLevel, bExtraContentScaleFallback);
	B2Scalability::SetRelativeFrameLimitLevel(IntToB2FrameLimitLevel(LoadedFrameLimitLevel));

	//if (GEngine)
	//{
	//	FAudioDevice* audioDevice = GEngine->GetMainAudioDevice();
	//	if (audioDevice)
	//	{
	//		GlobalSetGameBGMVolume(LoadedBGMVolume);
	//		GlobalSetGameEffectVolume(LoadedFxVolume);
	//	}
	//}

	if (LoadedVibrationScale == 1)
	{
		SetVibration(true);
	}
	else
	{
		SetVibration(false);
	}
}

void SaveGameSetting_AccessTerm(bool InValue)
{
	// 捞芭 唱吝俊 辑滚档 历厘秦具窃 [@AKI, 171220] 侩款丛俊霸 犬牢搬苞 辑滚俊 历厘捞 鞘夸 绝澜
	if (GConfig) {
		GConfig->SetBool(TEXT("AccessTermNotice"), *FString::Printf(TEXT("AccessTerm")), InValue, GB2GeneralSavedStateIni);
	}
}

bool IsAccessTerm()
{
	bool bRetVal = false;
	if (GConfig)
		GConfig->GetBool(TEXT("AccessTermNotice"), TEXT("AccessTerm"), bRetVal, GB2GeneralSavedStateIni);
	return bRetVal;
}

void SaveGameData_DLCDownload(bool bSuccess)
{
	if (GConfig) {
		GConfig->SetBool(TEXT("GameSetting"), *FString::Printf(TEXT("DLCDownLoad")), bSuccess, GB2GeneralSavedStateIni);
	}
}

bool IsDLCDownload()
{
	bool bSuccess = false;
	if (GConfig)
		GConfig->GetBool(TEXT("GameSetting"), TEXT("DLCDownLoad"), bSuccess, GB2GeneralSavedStateIni);
	return bSuccess;
}

void GlobalSetGameVolumeCommon(const FName& SoundClassName, float InVolume)
{ // 霸烙 荤款靛 可记 炼例俊 荤侩. 角力 历厘阑 窍瘤绰 臼绊 弊扒 SaveGameSetting_ 栏肺 蝶肺. 林狼 : Object iterate 窃.
//	for (TObjectIterator<USoundClass>lt; lt; ++lt)
//	{
//		USoundClass* soundClass = *lt;
//		if (soundClass && soundClass->GetFName() == SoundClassName)
//		{
//#if !UE_BUILD_SHIPPING
//			UE_LOG(LogBladeII, Log, TEXT("SoundClass %s Volume [[[[[[[[[[[ %.2f -> %.2f ]]]]]]]]"), *SoundClassName.ToString(), soundClass->Properties.Volume, InVolume);
//#endif
//			soundClass->Properties.Volume = InVolume;
//		}
//	}
}

void SetImageBrushFromPaperSprite(UImage* InImageToSetBrush, UPaperSprite* InSpriteToSet)
{
	//// 4.13 俊辑 WrapBox 客 PaperSprite 甫 荤侩茄 image widget batching 阑 劝侩窍扁 困秦 烙矫肺 持澜.
	//// 恐 烙矫衬搁 UImage 率俊辑 SetBrushFromMaterial 俊 措览窍绰 SetBrushFromPaperSprite 鞍篮 吧 力傍且 芭扼 扁措窍扁 锭巩.
	//// 瘤陛 寸厘篮 UMG 率俊 PaperSprite 内靛甫 持阑 荐啊 绝焙..
	//if (InImageToSetBrush && InSpriteToSet)
	//{
	//	InImageToSetBrush->GetBrush().SetResourceObject(InSpriteToSet);
	//	InImageToSetBrush->SetBrush(InImageToSetBrush->Brush);
	//}
}

void SetBlendColorForPaperSpriteBoundImage(UImage* InImageWithPaperSprite, const FLinearColor InBlendColor)
{
	//if (InImageWithPaperSprite)
	//{
	//	// MaterialInstance 甫 技泼茄 UImage 绰 GetDynamicMaterial 阑 烹秦 MID 甫 啊廉客辑 颇扼固磐甫 技泼且 荐 乐绰单 
	//	// PaperSprite 绰 泅犁肺急 (4.14) 弊繁 霸 救冈塞.
	//	// 窜 拿矾 喉坊爹鳖瘤绰 UImage 俊 乐绰 吧肺 且 荐 乐促. PaperSpriteComponent 档 拿矾 宏坊爹 沥档父 瘤盔窍绰 淀窃.
	//	InImageWithPaperSprite->SetColorAndOpacity(InBlendColor);
	//	InImageWithPaperSprite->GetBrush().TintColor = InBlendColor; // SetColorAndOpacity 肺绰 救冈洒帮 秦辑.. Tint 档 汲沥.
	//	InImageWithPaperSprite->SetBrush(InImageWithPaperSprite->Brush);
	//}
}

/**
* MatineeActor 档 牢磊肺 罐绰 付寸俊 楷免侩栏肺 焊搁 等促. 持绢林绰 DataStore 俊 嘎苗辑 敲饭捞绢 某腐磐狼 寇包阑 茄 SkeletalMeshActor spawn.
* StageEventDirector 率俊辑 窍绰 老苞 厚搅茄 老阑 窍绰单 弊吧 鸥瘤 臼绰 喊档狼 楷免 扁瓷阑 荤侩窍霸 等 镑捞 乐绢辑 捞 窃荐啊 蝶肺 父甸绢咙.
*/
class ASkeletalMeshActor* SpawnPuppetActor(FString TrackName, ALevelSequenceActor * MatineeActor, EPCClass CharClass, const class ICharacterDataStore* DataStore, class UAnimationAsset* ToPlayAnimation)
{
//	if (!DataStore)
//		return NULL;
//
//	if (!MatineeActor)
//		return NULL;
//
//	if (!MatineeActor->bIsPlaying)
//		MatineeActor->InitInterp();
//
//	FTransform SpawnTransform;
//
//	for (auto* InterpGroup : MatineeActor->GroupInst)
//	{
//		if (InterpGroup->Group && InterpGroup->Group->GroupName == FName(*TrackName) && InterpGroup->GroupActor)
//		{
//			SpawnTransform = InterpGroup->GroupActor->GetTransform();
//		}
//	}
//
//	if (!MatineeActor->bIsPlaying)
//		MatineeActor->TermInterp();
//
//	auto* ClassInfoBox = StaticFindPCClassInfoBox();
//	auto* ClassInfo = ClassInfoBox ? ClassInfoBox->GetSingleClassInfo(CharClass) : nullptr;
//
//	if (ClassInfo)
//	{
//		auto* EntryActor = GWorld->SpawnActor<ASkeletalMeshActor>(ASkeletalMeshActor::StaticClass(), SpawnTransform);
//		check(EntryActor);
//
//		float fScale = ClassInfo->MeshScaleOverride;
//		EntryActor->SetActorScale3D(FVector(fScale, fScale, fScale));
//
//		if (!GWorld->HasBegunPlay())
//		{
//			UE_LOG(LogBladeII, Log, TEXT("Forcing call to 'BeginPlay' on newly spawned actor."));
//			EntryActor->DispatchBeginPlay();
//		}
//
//		auto* SkeletalMeshComponent = EntryActor->GetSkeletalMeshComponent();
//		if (SkeletalMeshComponent)
//		{
//			SkeletalMeshComponent->SetSkeletalMesh(ClassInfo->BaseMeshAsset);
//		}
//
//		if (ClassInfo && SkeletalMeshComponent)
//		{
//			USkeletalMesh* PrebuiltMeshOrResult = NULL;
//
//			TArray<FB2Item> EquippedItems;
//			DataStore->GetEquippedItems(CharClass, EquippedItems);
//			DataStore->GetEquippedCostumeItems(CharClass, EquippedItems);
//			FB2Wing WingData;
//			const bool bHasWing = DataStore->GetCharacterWing(CharClass, WingData);
//			ICharacterDataStore::GetRenderItem(DataStore, CharClass, EquippedItems);
//
//			if (UB2PCClassInfo::SetupSKCompForParts(CharClass, EntryActor, SkeletalMeshComponent, PrebuiltMeshOrResult, ClassInfo->BaseMeshAsset, ClassInfo->DefaultPartsInfo,
//				EquippedItems, bHasWing ? &WingData : NULL, NULL,
//				false, // 臂疥, 趣矫 傈捧 矫累 傈俊 荤侩且 啊瓷己阑 积阿茄促搁 搬苞拱 mesh 甫 某教窍瘤档 臼绰 魄俊 盔夯 府家胶扼档 皋葛府俊 巢败 滴搁 亮阑 巴 鞍酒辑..
//					   // 捞芭 侩档啊 牢霸烙 敲饭捞绢 某腐磐啊 酒囱 葛靛狼 EntryActor spawn.. 郴瘤绰 促弗 荤侩捞扼档 楷免侩老波扼 秦寸 窃荐肺 魄喊
//				UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(EntryActor)
//			))
//			{
//				// 咯扁急 merge 等 mesh 甫 某教窍搁 宝洒 ID 部捞绊 秦辑 巩力父 瞪 荐 乐栏骨肺 弊成 逞绢皑. 弊贰档 甘 肺爹 促矫 窍绰 霸 酒聪扼搁 秦 滴搁 亮变 窍促. 傈捧 柳涝窍搁辑 促矫 救 秦档 登聪.
//			}
//
//			ABladeIIPlayer::SetupLODInfoForLobbyRepresentative(EntryActor); // 楷免侩 LOD == 肺厚侩 LOD
//
//			if (SkeletalMeshComponent)
//			{
//				SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
//				SkeletalMeshComponent->PlayAnimation(ToPlayAnimation, true);
//			}
//		}
//
//		return EntryActor;
//	}
//
	return NULL;
}

/** UE 4.15 鳖瘤 UNavigationSystem::GetRandomReachablePointInRadius static 滚傈捞 乐绢辑 弊吧 曹弧搁辑 静绊 乐菌绰单
* 4.16 何磐 deprecated 登搁辑 搬惫 捞犯霸 度鞍捞 备泅秦 初澜. */
FVector GetRandomReachablePointInRadiusWithCurrNavSys(UWorld* InWorld, const FVector& Origin, float Radius, ANavigationData* NavData, TSubclassOf<UNavigationQueryFilter> FilterClass)
{
	FNavLocation RandomPoint;

	//UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(InWorld); // 窜 盔夯 GetRandomReachablePointInRadius 俊辑绰 WorldContextObject 甫 罐疽绰单 咯扁急 弊成 World 肺 罐绰促.
	//if (NavSys)
	//{
	//	ANavigationData* UseNavData = NavData ? NavData : NavSys->GetMainNavData(FNavigationSystem::DontCreate);
	//	if (UseNavData)
	//	{
	//		NavSys->GetRandomReachablePointInRadius(Origin, Radius, RandomPoint, UseNavData, UNavigationQueryFilter::GetQueryFilter(*UseNavData, InWorld, FilterClass));
	//	}
	//}

	return RandomPoint.Location;
}

#if WITH_EDITOR
void EditorPopupTAssetBlowupWarning(const FString& InfoAssetName, const FString& WarningAssetName)
{
#if !PLATFORM_MAC
	FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
		FString::Printf(TEXT("InfoAsset %s 俊 殿废等 TAsset %s 肺爹 角菩. 哪腔磐啊 梆 气惯茄促."), *InfoAssetName, *WarningAssetName)
	));
#endif
}
#endif

#if !UE_BUILD_SHIPPING
void CheckResetAssumeNoWorldLoadingCondition(UObject* WorldContextObject)
{ // GameMode 棺 割割 咀磐甸俊 甸绢埃 ResetAssumeNoWorldLoading 扁瓷狼 角力 荤侩 炼扒 眉农. 酒公单辑唱 荤侩捞 啊瓷且 沥档肺 斑绊茄 扁瓷篮 酒聪骨肺..
#if WITH_EDITOR
	if (GIsEditor) { // 俊叼磐俊辑父 抛胶飘甫 困秦 促弗 霸烙葛靛俊辑狼 荤侩阑 倾啊窃.
		return;
	}
#endif
	//AB2HeroTowerGameMode* CastedHTGM = Cast<AB2HeroTowerGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	//checkf(CastedHTGM, TEXT("Current GameMode does NOT officially support ResetAssumeNoWorldLoading!"));
}
#endif

bool CreateSoundCueConcatenator(class USoundCue* OutSound, class USoundWave* SoundWaveA, class USoundWave* SoundWaveB)
{
	//if (OutSound == nullptr)
	//{
	//	ensure(!TEXT("NodeConcatenator OutSound is NULL!"));
	//	return false;
	//}

	//USoundNodeConcatenator* NodeConcatenator = OutSound->ConstructSoundNode<USoundNodeConcatenator>();
	//NodeConcatenator->CreateStartingConnectors();
	//OutSound->FirstNode = NodeConcatenator;

	//USoundNodeWavePlayer* SoundA = OutSound->ConstructSoundNode<USoundNodeWavePlayer>();
	//SoundA->SetSoundWave(SoundWaveA);

	//USoundNodeWavePlayer* SoundB = OutSound->ConstructSoundNode<USoundNodeWavePlayer>();
	//SoundB->SetSoundWave(SoundWaveB);
	//SoundB->bLooping = true;

	//USoundNodeLooping* LoopingNode = OutSound->ConstructSoundNode<USoundNodeLooping>();
	//LoopingNode->ChildNodes.Add(SoundB);

	//if (NodeConcatenator->ChildNodes.Num() < 2)
	//{
	//	ensure(!TEXT("NodeConcatenator ChildNodes CHECK!"));
	//	return false;
	//}
	//NodeConcatenator->ChildNodes[0] = SoundA;
	//NodeConcatenator->ChildNodes[1] = LoopingNode;
	return true;
}

FDateTime GetServerTimeToDateTime(int64 ServerTime)
{
	return FDateTime::FromUnixTimestamp(ServerTime / 1000);
}

FDateTime ConvertTime(const FDateTime & InDate, int32 InTimezone, int32 OutTimezone)
{
	if (InTimezone == OutTimezone)
	{
		return InDate;
	}

	// Timezone Hour ranges go from -12 to +14 from UTC
	// Convert from whole-hour to the full-format HHMM (-5 -> -0500, 0 -> +0000, etc)
	InTimezone = ConvertShortTimezone(InTimezone);
	OutTimezone = ConvertShortTimezone(OutTimezone);

	// Extract timezone minutes
	const int32 InTimezoneMinutes = (FMath::Abs(InTimezone) % 100);
	const int32 OutTimezoneMinutes = (FMath::Abs(OutTimezone) % 100);

	// Calculate our Minutes difference
	const int32 MinutesDifference = OutTimezoneMinutes - InTimezoneMinutes;

	// Calculate our Hours difference
	const int64 HoursDifference = (OutTimezone / 100) - (InTimezone / 100);

	return FDateTime(InDate + FTimespan(HoursDifference, MinutesDifference, 0));
}

FDateTime ConvertUTCToLocalTime(const FDateTime & InDate)
{
	int32 InTimeZone = UB2GameInstance::GetTimeZone(ETimeZone::UTC);
	//烙矫 老窜 辑匡肺..
	int32 OutTimeZone = UB2GameInstance::GetTimeZone(ETimeZone::Seoul);

	return ConvertTime(InDate, InTimeZone, OutTimeZone);
}

int32 ConvertShortTimezone(int32 ShortTimezone)
{
	// Convert timezones from short-format into long format, -5 -> -0500
	// Timezone Hour ranges go from -12 to +14 from UTC
	if (ShortTimezone >= -12 && ShortTimezone <= 14)
	{
		return ShortTimezone * 100;
	}

	// Not a short-form timezone
	return ShortTimezone;
}

bool m_bPopupopen = false;
void SetPopupOpen(bool bSet)
{
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("1... SetPopupOpen: %d"), int32(bSet));
	m_bPopupopen = bSet;
}

bool IsPopupUIOpen()
{
	return m_bPopupopen;
}

bool CheckContentsModeState(int32 ContentsModeType)
{
	switch (UB2UIDocHelper::GetDocMode()->GetContentsModeState(ContentsModeType))
	{
	case b2network::B2ContentsModeState::NONE:
	case b2network::B2ContentsModeState::RUN:
		// 鞠巴档 救窃. 
		break;
	case b2network::B2ContentsModeState::BLOCK_ENTER:
	case b2network::B2ContentsModeState::MODE_FINISH:
	case b2network::B2ContentsModeState::SETTLE_BEGIN:
	case b2network::B2ContentsModeState::SETTLE_END:
		UB2UIManager::GetInstance()->OpenMsgPopupFromErrorCode(811);
		return true;
		break;
	case b2network::B2ContentsModeState::SYSTEM_CHECK:
		// 痢八吝UI
		UB2UIManager::GetInstance()->OpenMsgPopupFromErrorCode(812);
		return true;
		break;
	default:
		break;
	}

	return false;
}

FText FItemGradeInfo::GetItemGradeColor(uint32 ItemGrade)
{
	static FText GradeColors[MAX_NORMAL_ITEM_STAR_GRADE] = {
		FText::FromString(TEXT("#b8b9b8")),
		FText::FromString(TEXT("#7bc315")),
		FText::FromString(TEXT("#068edc")),
		FText::FromString(TEXT("#c83dd9")),
		FText::FromString(TEXT("#e97223")),
		FText::FromString(TEXT("#f2d802")),
		FText::FromString(TEXT("#cf2c2c")),
		FText::FromString(TEXT("#8250f3")),
		FText::FromString(TEXT("#00BF9F")),
		FText::FromString(TEXT("#efecbf")),
		FText::FromString(TEXT("#77d5ff")),
		FText::FromString(TEXT("#8B008B")),

	};

	return GradeColors[FMath::Clamp<uint32>(ItemGrade - 1, 0, MAX_NORMAL_ITEM_STAR_GRADE - 1)];
}