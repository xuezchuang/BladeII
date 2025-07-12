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
	// DLC ������ ���� �ھ� ��ɸ� �־����. 
	// �̰��� �� �ϱ� ������ DeferredRegistChangeLobbyScene �� �־�� ���൵ ������ �ǵ���� �� ��.

	//[@AKI, 180508] ���� ����� �Ǵ� �� �����ϰ�� ����ؼ��� �ȵ�!!!
	//�����͸� �ʱ�ȭ ���� �ʰ� ����ϰ� �־ �ȵ�!! ���� ������ �ʱ�ȭ �ϴ� �κ��� ���� �ؾ� �� ��

	FString DLCFrontMapPath;
	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIDLCMap"), DLCFrontMapPath, GGameIni);

	FB2NetworkBridge::GetInstance()->Shutdown();

	if (DLCFrontMapPath.Len() > 0)
	{
		// ���� �� �ε����� ���� ���� ����.
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
	// �κ� ������ ���� �ھ� ��ɸ� �־����. 
	// �̰��� �� �ϱ� ������ DeferredRegistChangeLobbyScene �� �־�� ���൵ ������ �ǵ���� �� ��.

	FString LobbyMapPath;
	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIILobbyMap"), LobbyMapPath, GGameIni);

	if (LobbyMapPath.Len() > 0)
	{
		// ���� �� �ε����� ���� ���� ����.
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
	//B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIGameStage"));

	//// ���� �� �ε� ���൵�� ���� ���� �� �ε����� ���� ������ ����
	//B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, ChangeGameMode);

	//// ����: �ٸ� �������� ���� B2GameModule ���� StageInfoTable �� ���� ������ �� �ʿ䰡 ����.
	//// ����� StageManager �� ��� �ִ� CurrentStageNumber �� �ٸ� StageInfoData �� �������� ����.
	//// ��Ÿ �Ϲ����� ��뿡���� ������ AB2StageManager::GetCurrentStageInfoData �� AB2StageGameMode::GetCurrentStageInfoData �� ���

	//UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	//FSingleStageInfoData* StageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(InClientStageId) : NULL;
	//if (StageInfoData)
	//{
	//	// Ŭ���̾�Ʈ �׽�Ʈ ������ StageNum �Ѱ���.
	//	BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = InClientStageId;

	//	// OnPreLoadMap ���� Movie ���� �������� ��� ȭ���� ���� UI Screen.
	//	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, ChangeGameMode);

	//	UGameplayStatics::OpenLevel(InCurrentGameMode, StageInfoData->MainMap, true, OpenURLOption);
	//	return true;
	//}

	return false;
}

bool StartBladeIIPVPGame(class AGameMode* InCurrentGameMode, FString OpenURLOption)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIPVPGame"));

	// ���� �� �ε� ���൵�� ���� ���� �� �ε����� ���� ������ ����
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::PVP_Tag);

	FString PVPMapName;

	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIPVPMap"), PVPMapName, GGameIni);

	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::PVP_Tag); // �ε� ��ũ���� �̸� �ѹ� �ε��� �ִ� �� �����ϴٴ� ������ �־ �̸� ������ �� �ش�..

	UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*PVPMapName), true, OpenURLOption);
	return true;
}

bool StartBladeIIPVPReplay(class AGameMode* InCurrentGameMode, FModReplayInfo ReplayInfo, FString OpenURLOption)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIPVPReplay"));

	//auto* doc = UB2UIDocHelper::GetDocSome();
	//if (doc)
	//{
	//	doc->ReplayInfo = ReplayInfo;
	//}

	//FString ReplayMapName(TEXT("/Game/Maps/ReplayTest"));

	//OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance()
	//	.Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::PVPReplay);

	//UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*ReplayMapName), true, OpenURLOption);

	return true;
}

bool StartBladeIITeamMatchGame(class AGameMode* InCurrentGameMode, FString OpenURLOption /*= FString(TEXT(""))*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIITeamMatchGame"));

	// ���� �� �ε� ���൵�� ���� ���� �� �ε����� ���� ������ ����
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::PVP_Team);

	FString TeamMatchMapName;

	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIITeamMatchMap"), TeamMatchMapName, GGameIni);

	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::PVP_Team); // �ε� ��ũ���� �̸� �ѹ� �ε��� �ִ� �� �����ϴٴ� ������ �־ �̸� ������ �� �ش�..

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

	//// ���� �� �ε� ���൵�� ���� ���� �� �ε����� ���� ������ ����
	//B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Control);

	//FString ControlMapName;

	//GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIControlMap"), ControlMapName, GGameIni);

	//OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::Control); // �ε� ��ũ���� �̸� �ѹ� �ε��� �ִ� �� �����ϴٴ� ������ �־ �̸� ������ �� �ش�..

	//UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*ControlMapName), true, OpenURLOption);
	return true;
}

bool StartBladeIIGuildMatchGame(class AGameMode* InCurrentGameMode, FString OpenURLOption /*= FString(TEXT(""))*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIGuildGame"));

	// ���� �� �ε� ���൵�� ���� ���� �� �ε����� ���� ������ ����
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Guild);

	FString ControlMapName;

	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIGuildMatchMap"), ControlMapName, GGameIni);

	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::Guild); // �ε� ��ũ���� �̸� �ѹ� �ε��� �ִ� �� �����ϴٴ� ������ �־ �̸� ������ �� �ش�..

	UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*ControlMapName), true, OpenURLOption);
	return true;

}

// �̰� ��� ABladeIIGameMode::GoToTutorial �̰� ���� ��..?
bool StartBladeIIGameTutorial(class AGameMode* InCurrentGameMode, FString OpenURLOption)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIGameTutorial"));

	// ���� �� �ε� ���൵�� ���� ���� �� �ε����� ���� ������ ����
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

	// ���� �� �ε� ���൵�� ���� ���� �� �ε����� ���� ������ ����
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Dimension);

	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	FSingleStageInfoData* StageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(MapLoadID) : NULL;
	if (StageInfoData)
	{
		// Ŭ���̾�Ʈ �׽�Ʈ ������ StageNum �Ѱ���.
		BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = MapLoadID;

		// OnPreLoadMap ���� Movie ���� �������� ��� ȭ���� ���� UI Screen.
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
// �� ���� StaticFind ���� ���� �� �����Ƿ� ������ BladeIIGameMode �� ĳ�̵� �� ����ϰ�(WorldContextObject�� �Ѱ���), �� tick ������� �� ��.

UB2PCClassInfoBox* StaticFindPCClassInfoBox(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	//if (WorldContextObject)
	//{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
	//	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	//	if (B2GI) {
	//		return B2GI->GetMobClassInfoBox();
	//	}
	//}
	//return StaticFindBPInfoAssetTempl<UB2NPCClassInfoBox>(TEXT("/Script/BladeII.B2NPCClassInfo"), TEXT("MobClassInfoBoxAsset"));
	return NULL;
}

UB2CommonSoundInfo* StaticFindCommonSoundInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
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
	//{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
	//	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	//	if (B2GI) {
	//		return B2GI->GetGuildNPCInfo();
	//	}
	//}
	return StaticFindBPInfoAssetTempl<UB2GuildNPCInfo>(TEXT("/Script/BladeII"), TEXT("GuildNPCInofAsset"));
}

//class UTutorialControlBotInfos* StaticFindTutorialControlBotInfos()
//{
//	return StaticFindBPInfoAssetTempl<UTutorialControlBotInfos>(TEXT("/Script/BladeII.B2TutorialInfo"), TEXT("TutorialControlBotInfos"));
//}

UB2CombatConstantInfo * StaticFindCombatConstantInfo(UObject * WorldContextObject)
{
	if (WorldContextObject)
	{ // ������ ��� GameInstance �� ĳ�̵� ���۷����� ���.
		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (B2GI)
		{
			return B2GI->GetCombatConstantInfo();
		}
	}
	return StaticFindBPInfoAssetTempl<UB2CombatConstantInfo>(TEXT("/Script/BladeII"), TEXT("CombatConstantInfoAsset"));
}

//int32 UpdateAutoBattleType(int32 InAutoBattleType)
//{
//	int32 AutoBallte = InAutoBattleType;
//
//	if (AutoBallte == AutoBattleType::NONE)
//	{
//		AutoBallte = AutoBattleType::Attack;
//	}
//	else
//	{
//		AutoBallte = AutoBallte << 1;
//		if (AutoBallte > AutoBattleType::Skill)
//		{
//			AutoBallte = AutoBattleType::NONE;
//		}
//	}
//
//	return AutoBallte;
//}
//
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
	//extern bool gDrawPerfMark;
	//if (bIsOn || gDrawPerfMark)
	//{
	//	double TimeTaken = FPlatformTime::Seconds() - RecordedStartTime;
	//	// ���� �ð��� ���� ���� �̻����� ���� �ɸ��� ���� ǥ��.. ���Ƽ� ã�ƺ��� ����.. ��ü�� �̰� ��ġ�ϴ� ���� Tick ���� �Ҹ��� �� �ƴϾ �̺�Ʈ �����ε� ���� �ɸ��� ���� ������ �Ÿ� �ɷ�����.
	//	const bool bConsiderAsWarningLv1 = (TimeTaken >= PerfMarkLv1Time);
	//	const bool bConsiderAsWarningLv2 = (TimeTaken >= PerfMarkLv2Time);

	//	FString LogString = FString::Printf(TEXT("%s End (Exec time : %f ms)"), *ScopeName, TimeTaken * 1000.0);

	//	FLinearColor PerfMarkDrawTextColor(1.0f, 1.0f, 0.0f, 1.0f);
	//	if (bConsiderAsWarningLv2) {
	//		UE_LOG(LogB2ScopedCallTrack, Warning, TEXT("%s [PerfMark_Lv2]"), *LogString);
	//		PerfMarkDrawTextColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
	//	}
	//	else if (bConsiderAsWarningLv1) {
	//		UE_LOG(LogB2ScopedCallTrack, Display, TEXT("%s [PerfMark_Lv1]"), *LogString);
	//	}
	//	else {
	//		UE_LOG(LogB2ScopedCallTrack, Log, TEXT("%s"), *LogString);
	//	}

	//	// Ư���� ȭ�鿡 ���� ���� ǥ��
	//	if (gDrawPerfMark && (bConsiderAsWarningLv1 || bConsiderAsWarningLv2))
	//	{
	//		BII_SCREEN_LOG(LogString, PerfMarkDrawTextColor, 12, 8.0f);
	//	}
	//}
}


// ScopedCallTracker ���� �ð� ���� ����� ���µ� �׷��ٰ� ScopedTimeLogger �� ������ٰ� �� �ʿ� ���� Ư���� �ð� ������ �ʿ��� ������ �־ ���� ��.
//
DEFINE_LOG_CATEGORY_STATIC(LogB2ScopedTimeLog, Log, All)
bool FB2ScopedTimeLogger::bIsOn = false; // �̰� �⺻�� false.. �ʿ��� ���� ����Ϸ��� ��
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
		// ���� �ð��� ���� ���� �̻����� ���� �ɸ��� ���� ǥ��.. ���Ƽ� ã�ƺ��� ����.. ��ü�� �̰� ��ġ�ϴ� ���� Tick ���� �Ҹ��� �� �ƴϾ �̺�Ʈ �����ε� ���� �ɸ��� ���� ������ �Ÿ� �ɷ�����.
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
		return; // �����Ϳ��� �̷� ���� �ؼ� �ȵȴ�. ��� �ε��� ���� ����.
	}
#endif

	FlushAsyncLoading(); // �̰� �ϴ� �� �����ұ�?

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// TAsset ����� InfoAsset �� �߿��� ������ ���۷��� ĳ�̰� �Բ� FStreamableManager �� ���� �ֵ��� ��������� ��ε��Ѵ�. 
	// FStreamableManager �� �������̱⵵ �ѵ�, �� �װ� �ƴϴ��� Preload �� �ʿ伺���� ���۷��� ĳ���� �ִٸ� ���Ŵ� �� �־�� ��.
	// ������ �ֵ��� FStreamableManager ��� �ܼ� LoadSynchronous ������� ��ȯ�ϸ� �̰� ���ϰ� �� ����.
	// �̰��� �ݴ�� GameMode �� PreloadAnyNecessaryInfo

	UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox(WorldContextObject); // ������ �� BladeIIGameMode �� ĳ�̵Ǿ� �ִ� �� �������� ���� WorldContextObject �� �Ѱ���.
	if (MobInfoBox)
	{
		MobInfoBox->UnloadExceptCurrentExplicit(B2GM);
	}

	UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox(WorldContextObject);
	if (PCClassInfoBox)
	{ // ������ �÷����� ���� unload �� ���ϰ� �� ���� �ִ�.
		PCClassInfoBox->UnloadAllOrExceptSelectedIfAllowed();
	}
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{ // PCClassInfo �� Ȯ���� Unload �Ϸ��� ���� �����͸� ���� �ִ� HeroDoc �� �����ϰ� release �� �� �ʿ䰡 �ִ�.
		UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCI);
		if (DocHero)
		{
			DocHero->ReleaseResources();
		}
	}
	// �Ѷ��� PCClassInfo �� �Ϻη� �ε��Ǵ� skill �ִϸ��̼� ������
	UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo(WorldContextObject);
	if (SkillAnimInfo)
	{
		SkillAnimInfo->UnloadAllOrExceptSelected(B2GM); // PCClassInfo �� �Ϻ� ���� ��ŭ ���������� UnloadAllOrExceptSelected ��
	}

	UB2UIDocScenarioDialog* DocScenarioDlg = UB2UIDocHelper::GetDocScenarioDialog();
	if (DocScenarioDlg)
	{
		DocScenarioDlg->ReleaseResources(); // �ʻ�ȭ material �� ��� ����.
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

	// UIManager ���� ���⼭ Unload ����. ���� �װ� Destroy �����ϰ� ������..
	// UB2LoadingImageInfo �� ���⼭ Unload �� ��. �װ� �ݴ�� �뷫 �� ������ �ʿ��� �� �ε��ϰ� ���� ���� �ε� �Ŀ� ��ε��� �� ��.

	UE_LOG(LogBladeII, Log, TEXT("Unloaded lazy-loaded info assets"));

	// ��Ÿ �߰��� �� ����.
}

void ManualUnloadOnStageClear(UObject* WorldContextObject)
{
	B2_SCOPED_TRACK_LOG(TEXT("ManualUnloadOnStageClear"));

#if WITH_EDITOR
	if (GIsEditor)
	{
		return; // �����Ϳ��� �̷� ���� �ؼ� �ȵȴ�. ��� �ε��� ���� ����.
	}
#endif

	FlushAsyncLoading(); // �̰� �ϴ� �� �����ұ�?

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// �������� ���� ���� ���� �� �����鼭 �޸� ����ϱ� ���� ���� �� ������.
	// ���⼭ ��ε� ���� ���� �͵鵵 ManualUnloadBeforeNewBeginning ���� ��� ������ ��

	// �������� Ŭ���� �����̶� MobInfo �� �� �̻� �ʿ���� ������ ����. (���� �ʿ��� ��찡 �߻��Ѵ� �ϴ��� �ε� ���ŷ�� �� ����� ��)
	UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox(WorldContextObject); // ������ �� BladeIIGameMode �� ĳ�̵Ǿ� �ִ� �� �������� ���� WorldContextObject �� �Ѱ���.
	if (MobInfoBox)
	{
		MobInfoBox->UnloadExceptCurrentExplicit(B2GM);
	}

	UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox(WorldContextObject);
	if (PCInfoBox)
	{// �� �������� �±״� ���� ����� �ϰ�.. �¸� ���⿡�� �±�ĳ���� puppet �� spawn �� ���� �ִµ� �ű⼭ �������� PCClassInfo �����͸� ������� �ʵ��� �ؾ� �Ѵ�.
	 // ABladeIIGameMode::SpawnPlayerCharAsPuppet �������� ���� ���ڸ� �Ѱ��ֵ��� �Ǿ� ����.
		PCInfoBox->UnloadAllOrExceptSelectedIfAllowed();
	}
	// �Ѷ��� PCClassInfo �� �Ϻη� �ε��Ǵ� skill �ִϸ��̼� ������
	UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo(WorldContextObject);
	if (SkillAnimInfo)
	{
		SkillAnimInfo->UnloadAllOrExceptSelected(B2GM); // PCClassInfo �� �Ϻ� ���� ��ŭ ���������� UnloadAllOrExceptSelected ��
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

	// ��Ÿ ���� �� ���� ���� ���̴� �͵� ��ε�.


	// GC �� �ѹ� ���ϰ� ������. �̰� �ؾ� ������ �޸𸮰� Ȯ���� ��.
	//UWorld* TheWorld = B2GM ? B2GM->GetWorld() : NULL;
	//if (TheWorld)
	//{
	//	TheWorld->ForceGarbageCollection();
	//}
}

void CopyObjectProperties(UObject* ObjCopyTo, UObject* ObjCopyFrom, bool bExcludeTransient, bool bExcludeSubComponents, const TArray<FName>& ExcludeProperties)
{
	B2_SCOPED_TRACK_LOG(TEXT("CopyObjectProperties"));

	//if (
	//	!ObjCopyTo || !ObjCopyFrom ||
	//	// ���� Ŭ��������, ������ ObjCopyFrom �� ObjCopyTo �� ���� Ŭ��������.
	//	(ObjCopyTo->GetClass() != ObjCopyFrom->GetClass() && !ObjCopyFrom->IsA(ObjCopyTo->GetClass()))
	//	)
	//{
	//	return;
	//}

	// ���� : FObjectInitializer::InitProperties
	// �̰� �⺻ serialize �Ŀ� ���Ǵ� ���̹Ƿ� �ٽ� ��Ʈ�� �� �ܼ�ȭ �Ǿ� ����.

	//UClass* ObjClass = ObjCopyTo->GetClass();
	//BII_CHECK(ObjClass);
	//for (FProperty* P = ObjClass->PropertyLink; P; P = P->PropertyLinkNext)
	//{
	//	const bool bIsTransient = P->HasAnyPropertyFlags(CPF_Transient | CPF_DuplicateTransient | CPF_NonPIEDuplicateTransient);
	//	if (bIsTransient && bExcludeTransient) {
	//		continue;
	//	}

	//	// Actor �� ī���ϴ� ��� ���� Component ���� ī���ϰ� ������ ���� ���̴�. �����ϸ� ���� ó��
	//	UObjectProperty* CastedObjProp = Cast<UObjectProperty>(P);
	//	if (bExcludeSubComponents && CastedObjProp && CastedObjProp->PropertyClass &&
	//		CastedObjProp->PropertyClass->GetDefaultObject()->IsA(UActorComponent::StaticClass())) {
	//		continue;
	//	}

	//	// ��Ÿ Ư���� �����ϰ� ���� �͵�.
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
//	//UE4Game�� Root�ε� ��.
//	//FPaths::GameDir();		//  ../../../BladeII/
//	//FPaths::EngineDir();		//  ../../../Engine/
//	//FPaths::GameSavedDir();	//  ../../../BladeII/Saved/
//	//FPaths::GameUserDir();	//	../../../BladeII/
//	//FPaths::RootDir();		//	../../../
//	//FPaths::ProfilingDir();	//	../../../BladeII/Saved/Profiling/
//
//	FString fileName(TEXT("3918D5C54C244BC2C1F44FD221EBD84F200162E1.Luc"));
//
//	FString FullPath;
//#if PLATFORM_IOS
//	FullPath = FPaths::Combine(FPaths::RootDir(), fileName);
//#else 
//	FullPath = FPaths::Combine(FPaths::GamePersistentDownloadDir(), fileName);
//#endif
//
//	bool bFileExist = FPaths::FileExists(FullPath);
////	if (bFileExist == false)
////	{
////		//�ҹ��� �˻�
////		fileName = fileName.ToLower();
////#if PLATFORM_IOS
////		FullPath = FPaths::Combine(FPaths::RootDir(), fileName);
////#else 
////		FullPath = FPaths::Combine(FPaths::GamePersistentDownloadDir(), fileName);
////#endif
////		bFileExist = FPaths::FileExists(FullPath);
////	}
//
//#ifdef NONE_KAKAO_SDK_VERSION
//	bFileExist = true;
//#endif
//
//	UE_LOG(LogBladeII, Log, TEXT("isLocalConnectIniFileExist : FullPath : %s"), *FullPath);
//
//	/*
//	//JSon Writer
//	FArchive* WriteArc = IFileManager::Get().CreateFileWriter(*FullPath);
//	if (WriteArc)
//	{
//	FString JsonStr;
//	TSharedPtr<FJsonObject> JsonParsed;
//	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(WriteArc);
//	JsonWriter->WriteObjectStart();
//	JsonWriter->WriteValue(FString(TEXT("B3D42311E981FA6399D5CD01153D5DD03F6FF220")), FString(TEXT("LOCAL")));
//	JsonWriter->WriteValue(FString(TEXT("B624E063500C07A876526621E2E7BAB39B3102AE")), true);
//
//	JsonWriter->WriteObjectEnd();
//	JsonWriter->Close();
//
//	WriteArc->Flush();
//	WriteArc->Close();
//	delete WriteArc;
//	}
//	if (bFileExist)
//	{
//	//JSon Reader
//	FArchive* ReadArc = IFileManager::Get().CreateFileReader(*FullPath);
//	if (ReadArc)
//	{
//	TSharedPtr<FJsonObject> JsonParsed;
//	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(ReadArc);
//	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed) && JsonParsed.IsValid())
//	{
//	FString stServerType = JsonParsed->GetStringField(FString(TEXT("B3D42311E981FA6399D5CD01153D5DD03F6FF220"))); //Sha1(ServerType)
//	stServerType = stServerType.ToUpper();
//	bFileExist = stServerType.Compare(FString("LOCAL")) == 0 ? true : false;
//	bool bCheatEnable = JsonParsed->GetBoolField(FString(TEXT("B624E063500C07A876526621E2E7BAB39B3102AE"))); //Sha1(CheatEnable)
//	//GShowConsoleWindowNextTick = bCheatEnable;
//	}
//	}
//	}
//	//*/
//	return bFileExist;
	return true;
}

// �Ʒ� SetupNamedMIDForFxComp, GetFxNamedMIDCommon, SetupMIDAtlasParamFromUIMIC �� ����ؼ� 
// �Ϲ����� 3D material �� Ư�� UI material �� ���������� �ٹ̴� ���ø� B2LobbyUI_ItemCUSProg ���� ����..

void SetupNamedMIDForFxComp(UParticleSystemComponent* InPSComp)
{
	B2_SCOPED_TRACK_LOG(TEXT("SetupNamedMIDForFxComp"));

	if (!InPSComp || !InPSComp->Template) {
		return;
	}

	// FParticleEmitterInstance::Tick_MaterialOverrides ����
	// NamedMaterialSlots �� ������ �Ϳ� ���� ���� �ε����� EmitterMaterials �� ������ �װ� ����ϰ� �ȴ�. 
	// Fx Component �����ϸ鼭 ������� EmitterMaterials �� MID �� ���� �־���
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
			// FParticleEmitterInstance::Tick_MaterialOverrides ����
			// NamedMaterialSlots �� ������ �Ϳ� ���� ���� �ε����� EmitterMaterials �� ������ �װ� ����ϰ� �ȴ�. 
			FNamedEmitterMaterial& CurrSlot = InPSComp->Template->NamedMaterialSlots[SI];
			if (CurrSlot.Name == MatSlotName && InPSComp->EmitterMaterials.IsValidIndex(SI))
			{
				// Fx ������ ������� EmitterMaterials �� MID �����ؼ� �־����� ��. (SetupNamedMIDForFxComp)
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

// SetupMIDAtlasParamFromUIMIC �� �ݺ� ��Ʈ
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
	// ���� ������ master material ���� ���� �̸��� �Ķ���Ͱ� ���� ���ҷ� �� �־�߰ڴ�.
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleX);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleY);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleWidth);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleHeight);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasWholeWidth);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasWholeHeight);
	// ���������� �ؽ��� �Ķ���͵�.
	UTexture* ThisTextureParamValue = NULL;
	if (RefUIMIC->GetTextureParameterValue(MtrlParamName_UIAtlasTexture, ThisTextureParamValue)) {
		MIDToSet->SetTextureParameterValue(MtrlParamName_UIAtlasTexture, ThisTextureParamValue);
	}
}
// ���� SetupMIDAtlasParamFromUIMIC �� ������ ������ ��Ƽ���� ������ �������� ����ε� NoAtlas static switch �� ����ϴ� ��
void SetupMIDNonAtlasParamFromUIMIC(class UMaterialInstanceDynamic* MIDToSet, class UMaterialInstanceConstant* RefUIMIC)
{
	B2_SCOPED_TRACK_LOG(TEXT("SetupMIDNonAtlasParamFromUIMIC"));

	if (!MIDToSet || !RefUIMIC) {
		return;
	}
	// �ؽ��� �Ķ���͸� ������ ��.
	UTexture* ThisTextureParamValue = NULL;
	if (RefUIMIC->GetTextureParameterValue(MtrlParamName_NoAtlasSingleTexture, ThisTextureParamValue)) {
		MIDToSet->SetTextureParameterValue(MtrlParamName_NoAtlasSingleTexture, ThisTextureParamValue);
		MIDToSet->SetTextureParameterValue(MtrlParamName_UIAtlasTexture, nullptr); // ������ ǥ�õ����� �ʰ����� Ȯ���� ������ �� �� �� ���� �� ���Ƽ� 
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
	// �������� ����ϴ� RoundDown ����
	return FPlatformMath::FloorToInt(a * FMath::Pow(10.f, NumDigit)) / FMath::Pow(10.f, NumDigit);
}

////////////////////////////////////////////////////
// Other miscellaneous utilities.

void GetTimeFormatFromRawSecond(int32 InRawSecond, int32& OutHour, int32& OutMin, int32& OutSec)
{
	OutHour = InRawSecond / 3600;

	int32 RemainingSecond = InRawSecond - OutHour * 3600; // 1�ð� �̸��� ������ ��

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

	// bFixInAir�÷��׷� ���� �����Ʈ������ �н�
	if (!InB2Char->GetCharacterMovement()->bAutoActivate)
		return;

	// Refered to UEditorEngine::SnapObjectTo
	// It doesn't have to be highly precise. Could be rough snap at the moment of spawn. They will fall to the ground by gravity anyway.

	if (InB2Char)
	{
		FVector ActorBoundOrigin;
		FVector ActorBoundBoxExtent;
		// Sometimes it could be better to use RootComponent's Bounds. Character �� ��ġ�� snapping �� ������ ���� �ٸ� �͵��� ������ ���̱� ���� ��Ʈ�� Capsule component ���� bound ���� ������.
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
	// �̰� Ư�� �÷��̾� ĳ���Ϳ� ���� 20/30�� �� 6�� ���� particle effect �� �� �ȳ����� �ϴ� �̽��� �����ϱ� ���� �غ�Ǿ���.
	// AB2StageEventDirector::ActivateMatineePuppetPSCs �� �������� �ƶ�.
	// �ܼ��� Hidden ���� ���� Activate �� ��Ű�µ� ��Ȯ�� ������ �����Ǿ������� ���� ���Ѵ�.

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
{ // Ư���ϰ� ����ϴ� ���� ����.
	if (InPSC) {
		InPSC->SetVisibility(true);
		InPSC->SetHiddenInGame(false);
		InPSC->SecondsBeforeInactive = BIG_NUMBER; // �̰� ������ ������ �� ��Ȳ�� ������ � ���� ������ �߰ߵ� ���� �־��µ� �̰� �� �߰��� ������ �ɱ� �;..
		InPSC->Activate(true);
	}
}

bool IsStaticallyVisiblePSC(class UParticleSystemComponent* InPSC)
{ // �����̳� ��Ÿ�� ��쿡 visibility �ڵ鸵 �� ó�� ��Ͽ� ���� �ֵ�. �̰� �ƴϸ� anim notify �� ���� �Ͻ������� ���� �ɷ� ����.
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

// GameplayStatics.cpp �� �ִ� ���� ��ƿ �Լ��� ������. �Ϲ����� ��쿡�� UGameplayStatics::SpawnEmitter ����� ������.
UParticleSystemComponent* CreateParticleSystemHelper(UParticleSystem* EmitterTemplate, UWorld* World, AActor* Actor, bool bAutoDestroy)
{
	UParticleSystemComponent* PSC = NewObject<UParticleSystemComponent>((Actor ? Actor : (UObject*)World));
	PSC->bAutoDestroy = bAutoDestroy;
	// ��� ��Ÿ �������� ���Ƿ� ä���ִ´�. ��ü�� �ΰ��� dynamic spawn �� ���� ������ �����ΰǰ�..
	PSC->bAllowAnyoneToDestroyMe = true;
	PSC->SecondsBeforeInactive = 0.0f;
	PSC->bAutoActivate = false;
	PSC->SetTemplate(EmitterTemplate);
	PSC->bOverrideLODMethod = false;

	return PSC;
}

UCameraComponent* GetCurrentActiveCamComp(UObject* WorldContextObject)
{ // ���� �� �������� ���̰� �ִ� ī�޶� ����. ���� ��� �Ϲ����� �÷��̾� ���� ���� �÷��̾ �޷� �ִ� ī�޶�.
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
	//// �ΰ��� ���� ��Ȳ���� ���� ������ ī�޶� ��ũ�� ���ư� �� �ֵ��� �ϴ� ���.
	//// �Ϲ������� �̰� ���ؾ� ī�޶� ����Ǵ� �� �ƴϰ� �Ͻ������� Ư���� ī�޶� ��ٰ� ���ư��� ���� ��Ȳ���� �� �� �ִ�.

	//if (InB2GM && InLocalPC)
	//{
	//	check(InLocalPC == Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(InB2GM)));

	//	if (!InB2GM->ApplyActiveCameraIfAny(InBlendInTime)) // Ư�� ������ ��ġ�� ActiveCamera ���� �õ�.
	//	{
	//		// ���� ���� ����Ǵ� ActiveCamera �� ���ٸ� ���� �⺻ ī�޶��.
	//		// ���� ���� ������ ���� ���µ� BlendFunction �� �ʿ��ϸ� ���ڷ� �Ѱ� �ְų�.. �� ��Ȳ���� Cubic �� ���� ������ ������ �ĳ־.
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
			ClampedStr += TEXT("..."); // ������ǥ ���̴� �ɼ�
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
	// �Ҽ��� InMaxFracDigit ���� ����ϵ� ���ڸ� 0 �� ��ȿ���ڷ� �� ġ�� ����.
	NumberDisplayFormat.SetMinimumFractionalDigits(0);
	NumberDisplayFormat.SetMaximumFractionalDigits(InMaxFracDigit);

	return FText::AsNumber(InNumber, &NumberDisplayFormat);
}

FText GetAutoFractionalFormattedText_FirstFracCountOnly(float InCountNumber, float InMaxNumber, int32 InMaxFracDigit)
{
	// ��ƿ�̶�⺸�ٴ� Ư���� ���̽��� ����ϴ� ����ε� ����� ���� ���� �� �־ ���⿡ �� ���´�.
	// �ִ� ��Ÿ���� �Ҽ������� ������ ��츦 ǥ���ϱ� ���� ù �Ҽ����� ī��Ʈ�ϴ� ���ȿ��� �Ҽ����� ǥ���ϱ����� �뵵. ��� ǥ���ϸ� �Ҽ����� ��Ÿ���� ������� �ؼ� ���ž��� ����

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
	// OutAllFound �� �������� �ʰ� ���..

	//if (!InSKComp || !InSKComp->GetAnimInstance()) {
	//	return;
	//}

	//B2_SCOPED_TRACK_LOG(TEXT("TryGatherAllBoundPSFromSKComp"));
	//B2_SCOPED_TIME_LOG(FString::Printf(TEXT("TryGatherAllBoundPSFromSKComp %s"), InSKComp->GetOwner() ? *(InSKComp->GetOwner()->GetName()) : *InSKComp->GetName()));

	//// ������ �ʱ�ȭ�� �� AnimInstance ������ ���� ���̴�.
	//UAnimInstance* TheAnimInst = InSKComp->GetAnimInstance();

	//TArray<UAnimationAsset*> AllPlayableAnims;
	//TheAnimInst->GetAllAnimAssets(AllPlayableAnims);

	//for (UAnimationAsset* ThisAnim : AllPlayableAnims)
	//{
	//	// BlendSpace �� �־�� �ϴµ�., �ϴ� �� �Լ��� ��� ���� �� ���������� �ǹ��ִ� ParticleSystem ���� ���� AnimSequence �� �� ���� ��.
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
		{ // �̹� �߰��� �� �ƴ��� �˻�. TArray �� AddUnique �ϸ� �� ������� PathName �� Key �� �� ������..
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
	// ParticleSystem �� ����ϴ� ������ AnimNotify(State) ��.. �ϴ� �ƴ� �� �ΰ���..
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

/** �ѹ� ȣ�⿡ �� ������ �׷���. �ʿ��� �Ⱓ ���� �� ������ ȣ�� */
void DrawDebugText(float InStartX, float InStartY, const FString& InText, const FLinearColor& InFontColor, int32 InFontSize, bool bDrawShadow, const FLinearColor& InShadowColor)
{
#if WITH_BII_ON_SCREEN_DEBUG_TEXT
	if (UB2UIManager::IsInstanceNull()) { // ���⼭ GetInstance �� �� ���� ������ �ǵ��� ���� �ʴ´�.
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
/** �̰� �ѹ��� ���ϸ� ������ �ð����� ���÷��� �ȴ�. ��� ��ġ�� ������ ���� */
void DrawTimedDebugText(const FString& InText, const FLinearColor& InFontColor, int32 InFontSize, float InDisplayTime, bool bDrawShadow, const FLinearColor& InShadowColor)
{
	//if (UB2UIManager::IsInstanceNull()) { // ���⼭ GetInstance �� �� ���� ������ �ǵ��� ���� �ʴ´�. Ư�� FB2ScopedCallTracker �� ���Ǳ� ������ UIManager �� Unload �� �������� �Ҹ��� �� �� �ִ�.
	//	return;
	//}

	//UB2UIManager* UIMgr = UB2UIManager::GetInstance();
	//UB2DebugTextWidget* ScreenDebugTextWidget = UIMgr ? UIMgr->GetScreenDebugTextWidget() : NULL;
	//if (ScreenDebugTextWidget)
	//{
	//	ScreenDebugTextWidget->AddTimedDrawText(InText, InFontColor, InFontSize, InDisplayTime, bDrawShadow, InShadowColor);
	//}
}
#endif

bool IsUsingMobileRendering(UWorld* InWorld)
{
//#if PLATFORM_ANDROID || PLATFORM_IOS
	return true;
//#else
//	// ���� �ɸ��� ����� ������. ��� ���ӻ��� ������ ������ ���� ���� �Ŷ� �߸��ȴٰ� �ؼ� ���� ��ɿ� ������ ��ġ�� �� �ƴϴ�.
//	ERHIFeatureLevel::Type RHIFeatureLevel = (InWorld && InWorld->Scene) ? InWorld->Scene->GetFeatureLevel() : ERHIFeatureLevel::Num;
//	return (RHIFeatureLevel == ERHIFeatureLevel::ES2 || RHIFeatureLevel == ERHIFeatureLevel::ES3_1);
//#endif
}

void TurnOffMeshComponentsDyamicShadowForModulated(AActor* InOwnerActor)
{
	////////////////////////////////////////
	//// InteractiveActor �� ���� BladeII ��ü���� ������ movable MeshComponent ���� �ִ� Actor �� ���� dynamic shadow �Ӽ� ��Ʈ�� ��ƿ.
	//// Modulated shadow �� �� ������ �־ ���� �ʿ䵵 ���� ������ ����ų ������ �ִ� �͵��� shadow �� �帮���� �ʵ��� �ϱ� ����.
	//// Modulated shadow �� ��ü�ϴ� ������ ������ ������ ������..
	//// �ٵ� ����� static lighting �� ����ϰ� �ִ� �� ������ modulated shadow �ܿ� ������ ����� ã�⵵ ����� ��
	////////////////////////////////////////

	//if (InOwnerActor && ShouldUsePerObjectModulatedShadow(InOwnerActor->GetWorld()))
	//{
	//	TArray<UActorComponent*> AllMeshComps = InOwnerActor->GetComponentsByClass(UMeshComponent::StaticClass());
	//	for (UActorComponent* ThisActorComp : AllMeshComps)
	//	{
	//		UMeshComponent* ThisMeshComp = Cast<UMeshComponent>(ThisActorComp);
	//		if (ThisMeshComp && ThisMeshComp->Mobility == EComponentMobility::Movable && ThisMeshComp->bCastDynamicShadow)
	//		{
	//			ThisMeshComp->bCastDynamicShadow = false;
	//			ThisMeshComp->MarkRenderStateDirty();
	//		}
	//	}
	//}
}
void ForceSendReallocateRenderTargetsSignal()
{
	// ����̽����� ��Ȥ ���� ���̴� ������ ���׿� ��ó�ϴ� ��������..
	// ���� ������ �̰� ������ ȿ���� �ִ����� ��. ������ �ǰ� �׽�Ʈ�ؼ� ������ ������ Ÿ�ֿ̹� ����ϰ� �Ǵ� ��..

	IConsoleVariable* CVarSendSignal = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ReallocateRenderTargetsSignal"));
	if (CVarSendSignal)
	{ // �ܼ��� �ϳ� ������Ų �� �ñ׳η� �� �Ѥ�;
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
			//// ���� ������ ������ ����
			//this->SetState(CONNECTION_STATE_SERVICENOTAVAILABLE);

			//UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			//if (UIMgrInst)
			//{
			//	if (this->GetSessionCloseReason() == ESessionCloseReason::ESessionCloseReason_None)
			//	{
			//		data_trader::DisconnectReasonLogEventClass<int32>::GetInstance().Signal(error);
			//		auto* gameMode = GetBladeIIGameMode(UIMgrInst);

			//		if (gameMode && gameMode->GetB2GameModeType() == EB2GameMode::PVP_Tag) //PVP Matching�϶��� �κ�� ������ ���� ���� ó��. �Լ� �̸��� �Ⱦ�︮����....
			//			FBladeIIBlockToSyncNetwork::GetInstance().ChannelDisconnect(error);
			//		else
			//			FBladeIIBlockToSyncNetwork::GetInstance().ServiceNotAvailable(error);
			//	}
			//}
		}
		);
		data_trader::ChannelDisconnectedEventClass<int32>::GetInstance().Subscribe(
			[this](int32 error)
		{
			// TODO: ä�� ������ ���� ����, PVP/Raid/Assault �� ���� ���� ��� �ߴܵ�
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

			//[@AKI, 170619] ����� ������ ��� �ü� �����Ƿ� �ѹ��� ��� ���� ��
			/*if (this->GetSessionCloseReason() == ESessionCloseReason::ESessionCloseReason_None)
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
			}*/
		}
		);
//		data_trader::RequireUpdateAppVersionEventClass<>::GetInstance().Subscribe(
//			[this]()
//		{
//#if !UE_BUILD_SHIPPING && WITH_EDITOR
//			if (GIsEditor) return;
//#endif // WITH_EDITOR
//
//			DeliveryAppUpdatePopUpClass<>::GetInstance().Signal();
//		}
//		);
//		data_trader::ServerAddressErrorEventClass<>::GetInstance().Subscribe(
//			[this]()
//		{
//#if !UE_BUILD_SHIPPING && WITH_EDITOR
//			if (GIsEditor) return;
//#endif // WITH_EDITOR
//			UB2UIManager* UIMgr = UB2UIManager::GetInstance();
//			if (UIMgr)
//			{
//				UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
//					BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
//					BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("GameServerAddressError")),
//					0.f,
//					true,
//					true,
//					EUIMsgPopupButtonGroup::Confirm,
//					FMsgPopupOnClick::CreateLambda([]()
//				{
//					exit(0);
//				}
//					)
//					);
//			}
//			else	exit(0);
//		}
//		);
//		data_trader::SessionClosedNotifyClass<int32>::GetInstance().Subscribe(
//			[this](int32 _reason)
//		{
//			this->SetSessionCloseReason(ESessionCloseReason::ESessionCloseReason_ClosedNotify);
//
//			FBladeIIBlockToSyncNetwork::GetInstance().SessionDisconnetFromServer(_reason);
//		}
//		);
//
//		data_trader::ServerTimeSyncEventClass<int64>::GetInstance().Subscribe(
//			[this](const int64 InServerTime)
//		{
//			UB2GameInstance::SetUTCNow(InServerTime);
//		}
//		);


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
	//if (WorldContextObject == nullptr)
	//	WorldContextObject = UB2UIManager::GetInstance();

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
// GDC ���Ŀ� �̰� ����� �κ� �����س��� ��� ���ش�. �� �츮�ڵ忡�� GDC �˻��ؼ� ������ �� �� üũ.
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
/** CBTVersionReady �� true �� ���·� ��ŷ �׽�Ʈ�� �� �������� ������ ��Ȳ������ ��Ȳ ���.
//* �̰� �����ϴ� �ܼ� Ŀ�ǵ尡 ����. */
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
	// ���⿡ ���� ���������� ����Ǵ� �°� ���� �׷��� �ɼǵ��� B2Scalability �� ����
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
	// ���⿡ ���� ���������� ����Ǵ� �°� ���� �׷��� �ɼǵ��� B2Scalability �� ����
	// EB2GraphicsLevel enum ���� �ٲٴ� ���߿�.. ���� ���� �ε��ϵ��� "GameGraphicLevel_N" ���� �ٲ�. ���� �ð� ������ ���� �ٽ� "GameGraphicLevel" �� �ǵ��� ��.
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameGraphicLevel")), OutValue, GB2GeneralSavedStateIni));
}
bool LoadGameSetting_Graphics_OrByDefault(int32& OutValue)
{
	//if (!LoadGameSetting_Graphics(OutValue)) {
	//	// ���� ����� ������ �����ٸ� DeviceProfile �� ���� detect �� ���� scalability �ý��� �⺻ ����������.
	//	OutValue = B2GraphicsLevelToInt(B2Scalability::GetGraphicsLevelByExpectedScalability());
	//	return false; // ���ϰ� ����� ����� ���ð��� �������� ������.
	//}
	return true;
}
bool LoadGameSetting_FrameLimit(int32& OutValue)
{
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameFrameLimitLevel")), OutValue, GB2GeneralSavedStateIni));
}
bool LoadGameSetting_FrameLimit_OrByDefault(int32& OutValue)
{
	if (!LoadGameSetting_FrameLimit(OutValue)) {
		// ���� ����� ���� ������ �����ٸ� DeviceProfile �� ���� �� ���� ���� ��..
		OutValue = B2FrameLevelToInt(B2Scalability::GetDefaultRelativeFrameLimitLevel());
		return false; // ���ϰ� ����� ����� ���ð��� �������� ������.
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
		// ���� ����� ���� ������ �����ٸ� DeviceProfile �� ���� �� ���� �⺻ ��.
		OutValue = B2ResolutionLevelToInt(B2Scalability::GetResolutionLevelOfRQType(GetDefaultGraphicsRQType()));
		return false; // ���ϰ� ����� ����� ���ð��� �������� ������.
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
			check(CastedSaveValue >= 0); // ������ Ŭ���� �� �ɷ� �ν��� ��.
			GConfig->SetInt(TEXT("GameSetting"), TEXT("ReservedResolutionLevel"), CastedSaveValue, GB2GeneralSavedStateIni);
		}
		else
		{ // �̰� Ŭ��� �ش�.
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
	return EB2ResolutionLevel::End; // �̰� reserve �� �Ǿ� �ִٴ� �ǹ̷�.
}

void InitializeAllGameSettingData(UObject* WorldContextObject, bool bExtraContentScaleFallback)
{
	//// High-level ���� ���� �����͵��� �о���̰ų�, ������ �ʱⰪ���� ���� �� ����.
	//// ���� ��ŸƮ��, ������ ù BladeIIGameMode ���� ������
	//bool bSavedSome = false;

	//int32 LoadedGraphicsLevel = B2GraphicsLevelToInt(EB2GraphicsLevel::GraphicsLevel_HIGH);
	//if (!LoadGameSetting_Graphics_OrByDefault(LoadedGraphicsLevel))
	//{
	//	// �̰� �ε� ���� �� �⺻���� ���⼭ ���� �ʿ�� ����. LoadGameSetting_Graphics_OrByDefault ���� ó���� ��.
	//	SaveGameSetting_Graphics(LoadedGraphicsLevel);
	//	bSavedSome = true;
	//}
	//int32 LoadedFrameLimitLevel = B2FrameLevelToInt(EB2FrameLimitLevel::Mid); // �������ϸ� �̰� High �� �⺻������ �� �ϴ� �� �������� ����
	//if (!LoadGameSetting_FrameLimit_OrByDefault(LoadedFrameLimitLevel))
	//{
	//	// �̰� �ε� ���� �� �⺻���� ���⼭ ���� �ʿ�� ����. LoadGameSetting_FrameLimit_OrByDefault ���� ó���� ��.
	//	SaveGameSetting_FrameLimit(LoadedFrameLimitLevel);
	//	bSavedSome = true;
	//}
	//int32 LoadedResolutionLevel = B2ResolutionLevelToInt(EB2ResolutionLevel::Mid);  // �������ϸ� �̰� High �� �⺻������ �� �ϴ� �� �������� ����
	//if (!LoadGameSetting_Resolution_OrByDefault(LoadedResolutionLevel))
	//{
	//	// �̰� �ε� ���� �� �⺻���� ���⼭ ���� �ʿ�� ����. LoadGameSetting_Resolution_OrByDefault ���� ó���� ��.
	//	SaveGameSetting_Resolution(LoadedResolutionLevel);
	//	bSavedSome = true;
	//}
	//// ���� ���࿡�� ��Ÿ�� ����Ī�� �ȵǴ� �ػ� ������ �� ���. ���� �ʵ忡 ����Ǿ� �ִ� �� ����.
	//EB2ResolutionLevel ReservedResLevel = LoadReservedResolutionLevelOfPrevAppRun();
	//if (ReservedResLevel != EB2ResolutionLevel::End && IntToB2ResolutionLevel(LoadedResolutionLevel) != ReservedResLevel)
	//{
	//	UE_LOG(LogBladeII, Log, TEXT("Applying reserved resolution level %d (Was loaded as %d)"), B2ResolutionLevelToInt(ReservedResLevel), LoadedResolutionLevel);
	//	LoadedResolutionLevel = B2ResolutionLevelToInt(ReservedResLevel);
	//	SaveGameSetting_Resolution(LoadedResolutionLevel);
	//	ClearResolutionLevelReserve();
	//	bSavedSome = true;
	//}

	//float LoadedBGMVolume = 1.0f;
	//if (!LoadGameSetting_BGMVolume(LoadedBGMVolume))
	//{
	//	LoadedBGMVolume = 0.7f; // �⺻��
	//	SaveGameSetting_BGMVolume(LoadedBGMVolume);
	//	bSavedSome = true;
	//}
	//float LoadedFxVolume = 1.0f;
	//if (!LoadGameSetting_EffectVolume(LoadedFxVolume))
	//{
	//	LoadedFxVolume = 0.7f; // �⺻��
	//	SaveGameSetting_EffectVolume(LoadedFxVolume);
	//	bSavedSome = true;
	//}
	//int32 LoadedVibrationScale = 1;
	//if (!LoadGameSetting_Vibration(LoadedVibrationScale))
	//{
	//	LoadedVibrationScale = 1; // �⺻��
	//	SaveGameSetting_Vibration(LoadedVibrationScale);
	//	bSavedSome = true;
	//}

	//int32 LoadedKeepScreenOn = 1;
	//if (!LoadGameSetting_KeepScreenOn(LoadedKeepScreenOn))
	//{
	//	LoadedKeepScreenOn = 1; // �⺻��
	//	SaveGameSetting_KeepScreenOn(LoadedKeepScreenOn);
	//	bSavedSome = true;
	//}

	//int32 LoadGamePush = 1;
	//if (!LoadGameSetting_GamePush(LoadGamePush))
	//{
	//	LoadedKeepScreenOn = 1; // �⺻��
	//	SaveGameSetting_GamePush(LoadGamePush);
	//	bSavedSome = true;
	//}

	//int32 LoadGameSoundLocType = B2SoundLocalizeType::Default_Type();
	//if (!LoadGameSetting_SoundLocType(LoadGameSoundLocType))
	//{
	//	SaveGameSetting_SoundLocType(LoadGameSoundLocType);
	//	bSavedSome = true;
	//}
	//FGenericPlatformMisc::SetSoundLocType(LoadGameSoundLocType);

	//// ���� �ʱ�ȭ ������ �Ҹ��� ���̱⵵ �ϰ� ���õ� �ڵ�⵵ �ϴ� �ٸ� ��⿡�� ����� ���� �Լ� ������ �Ҵ�. 
	//gLoadGameFxVolumeSettingFnPtr = LoadGameSetting_EffectVolume;
	//gLoadGameBGMVolumeSettingFnPtr = LoadGameSetting_BGMVolume;

	//if (bSavedSome)
	//{
	//	GConfig->Flush(false, GB2UserSavedStateIni);
	//	GConfig->Flush(false, GB2GeneralSavedStateIni);
	//}

	//// WorldContextObject �� ���Ӹ�� �� bias �� ���� �Ŷ� ��� �����̳� DLCFront ���� �Ҹ��� �Ÿ� �ʿ䰡 ����.
	//B2Scalability::AdjustScalabilityBySelectedLevel(WorldContextObject, LoadedGraphicsLevel, LoadedResolutionLevel, bExtraContentScaleFallback);
	//B2Scalability::SetRelativeFrameLimitLevel(IntToB2FrameLimitLevel(LoadedFrameLimitLevel));

	//if (GEngine)
	//{
	//	FAudioDevice* audioDevice = GEngine->GetMainAudioDevice();
	//	if (audioDevice)
	//	{
	//		GlobalSetGameBGMVolume(LoadedBGMVolume);
	//		GlobalSetGameEffectVolume(LoadedFxVolume);
	//	}
	//}

	//if (LoadedVibrationScale == 1)
	//{
	//	SetVibration(true);
	//}
	//else
	//{
	//	SetVibration(false);
	//}
}

void SaveGameSetting_AccessTerm(bool InValue)
{
	// �̰� ���߿� ������ �����ؾ��� [@AKI, 171220] ���Կ��� Ȯ�ΰ�� ������ ������ �ʿ� ����
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
{ // ���� ���� �ɼ� ������ ���. ���� ������ ������ �ʰ� �װ� SaveGameSetting_ ���� ����. ���� : Object iterate ��.
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
	// 4.13 ���� WrapBox �� PaperSprite �� ����� image widget batching �� Ȱ���ϱ� ���� �ӽ÷� ����.
	// �� �ӽóĸ� UImage �ʿ��� SetBrushFromMaterial �� �����ϴ� SetBrushFromPaperSprite ���� �� ������ �Ŷ� ����ϱ� ����.
	// ���� ������ UMG �ʿ� PaperSprite �ڵ带 ���� ���� ����..
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
	//	// MaterialInstance �� ������ UImage �� GetDynamicMaterial �� ���� MID �� �����ͼ� �Ķ���͸� ������ �� �ִµ� 
	//	// PaperSprite �� ����μ� (4.14) �׷� �� �ȸ���.
	//	// �� �÷� ���������� UImage �� �ִ� �ɷ� �� �� �ִ�. PaperSpriteComponent �� �÷� �귻�� ������ �����ϴ� ����.
	//	InImageWithPaperSprite->SetColorAndOpacity(InBlendColor);
	//	InImageWithPaperSprite->GetBrush().TintColor = InBlendColor; // SetColorAndOpacity �δ� �ȸ����� �ؼ�.. Tint �� ����.
	//	InImageWithPaperSprite->SetBrush(InImageWithPaperSprite->Brush);
	//}
}

/**
* MatineeActor �� ���ڷ� �޴� ���翡 ��������� ���� �ȴ�. �־��ִ� DataStore �� ���缭 �÷��̾� ĳ������ �ܰ��� �� SkeletalMeshActor spawn.
* StageEventDirector �ʿ��� �ϴ� �ϰ� ����� ���� �ϴµ� �װ� Ÿ�� �ʴ� ������ ���� ����� ����ϰ� �� ���� �־ �� �Լ��� ���� �������.
*/
//class ASkeletalMeshActor* SpawnPuppetActor(FString TrackName, AMatineeActor* MatineeActor, EPCClass CharClass, const class ICharacterDataStore* DataStore, class UAnimationAsset* ToPlayAnimation)
//{
////	if (!DataStore)
////		return NULL;
////
////	if (!MatineeActor)
////		return NULL;
////
////	if (!MatineeActor->bIsPlaying)
////		MatineeActor->InitInterp();
////
////	FTransform SpawnTransform;
////
////	for (auto* InterpGroup : MatineeActor->GroupInst)
////	{
////		if (InterpGroup->Group && InterpGroup->Group->GroupName == FName(*TrackName) && InterpGroup->GroupActor)
////		{
////			SpawnTransform = InterpGroup->GroupActor->GetTransform();
////		}
////	}
////
////	if (!MatineeActor->bIsPlaying)
////		MatineeActor->TermInterp();
////
////	auto* ClassInfoBox = StaticFindPCClassInfoBox();
////	auto* ClassInfo = ClassInfoBox ? ClassInfoBox->GetSingleClassInfo(CharClass) : nullptr;
////
////	if (ClassInfo)
////	{
////		auto* EntryActor = GWorld->SpawnActor<ASkeletalMeshActor>(ASkeletalMeshActor::StaticClass(), SpawnTransform);
////		check(EntryActor);
////
////		float fScale = ClassInfo->MeshScaleOverride;
////		EntryActor->SetActorScale3D(FVector(fScale, fScale, fScale));
////
////		if (!GWorld->HasBegunPlay())
////		{
////			UE_LOG(LogBladeII, Log, TEXT("Forcing call to 'BeginPlay' on newly spawned actor."));
////			EntryActor->DispatchBeginPlay();
////		}
////
////		auto* SkeletalMeshComponent = EntryActor->GetSkeletalMeshComponent();
////		if (SkeletalMeshComponent)
////		{
////			SkeletalMeshComponent->SetSkeletalMesh(ClassInfo->BaseMeshAsset);
////		}
////
////		if (ClassInfo && SkeletalMeshComponent)
////		{
////			USkeletalMesh* PrebuiltMeshOrResult = NULL;
////
////			TArray<FB2Item> EquippedItems;
////			DataStore->GetEquippedItems(CharClass, EquippedItems);
////			DataStore->GetEquippedCostumeItems(CharClass, EquippedItems);
////			FB2Wing WingData;
////			const bool bHasWing = DataStore->GetCharacterWing(CharClass, WingData);
////			ICharacterDataStore::GetRenderItem(DataStore, CharClass, EquippedItems);
////
////			if (UB2PCClassInfo::SetupSKCompForParts(CharClass, EntryActor, SkeletalMeshComponent, PrebuiltMeshOrResult, ClassInfo->BaseMeshAsset, ClassInfo->DefaultPartsInfo,
////				EquippedItems, bHasWing ? &WingData : NULL, NULL,
////				false, // �۽�, Ȥ�� ���� ���� ���� ����� ���ɼ��� �����Ѵٸ� ����� mesh �� ĳ�������� �ʴ� �ǿ� ���� ���ҽ��� �޸𸮿� ���� �θ� ���� �� ���Ƽ�..
////					   // �̰� �뵵�� �ΰ��� �÷��̾� ĳ���Ͱ� �ƴ� ����� EntryActor spawn.. ������ �ٸ� ����̶� ������ϲ��� �ش� �Լ��� �Ǻ�
////				UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(EntryActor)
////			))
////			{
////				// ���⼱ merge �� mesh �� ĳ���ϸ� ���� ID ���̰� �ؼ� ������ �� �� �����Ƿ� �׳� �Ѿ. �׷��� �� �ε� �ٽ� �ϴ� �� �ƴ϶�� �� �θ� ���� �ϴ�. ���� �����ϸ鼭 �ٽ� �� �ص� �Ǵ�.
////			}
////
////			ABladeIIPlayer::SetupLODInfoForLobbyRepresentative(EntryActor); // ����� LOD == �κ�� LOD
////
////			if (SkeletalMeshComponent)
////			{
////				SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
////				SkeletalMeshComponent->PlayAnimation(ToPlayAnimation, true);
////			}
////		}
////
////		return EntryActor;
////	}
////
//	return NULL;
//}

/** UE 4.15 ���� UNavigationSystem::GetRandomReachablePointInRadius static ������ �־ �װ� �ܻ��鼭 ���� �־��µ�
* 4.16 ���� deprecated �Ǹ鼭 �ᱹ �̷��� �Ȱ��� ������ ����. */
FVector GetRandomReachablePointInRadiusWithCurrNavSys(UWorld* InWorld, const FVector& Origin, float Radius, ANavigationData* NavData, TSubclassOf<UNavigationQueryFilter> FilterClass)
{
	FNavLocation RandomPoint;

	//UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(InWorld); // �� ���� GetRandomReachablePointInRadius ������ WorldContextObject �� �޾Ҵµ� ���⼱ �׳� World �� �޴´�.
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
		FString::Printf(TEXT("InfoAsset %s �� ��ϵ� TAsset %s �ε� ����. ��ǻ�Ͱ� �� �����Ѵ�."), *InfoAssetName, *WarningAssetName)
	));
#endif
}
#endif

#if !UE_BUILD_SHIPPING
void CheckResetAssumeNoWorldLoadingCondition(UObject* WorldContextObject)
{ // GameMode �� ��� ���͵鿡 �� ResetAssumeNoWorldLoading ����� ���� ��� ���� üũ. �ƹ������� ����� ������ ������ �߰��� ����� �ƴϹǷ�..
#if WITH_EDITOR
	if (GIsEditor) { // �����Ϳ����� �׽�Ʈ�� ���� �ٸ� ���Ӹ�忡���� ����� �㰡��.
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
	//�ӽ� �ϴ� �����..
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
		// �ϰ͵� ����. 
		break;
	case b2network::B2ContentsModeState::BLOCK_ENTER:
	case b2network::B2ContentsModeState::MODE_FINISH:
	case b2network::B2ContentsModeState::SETTLE_BEGIN:
	case b2network::B2ContentsModeState::SETTLE_END:
		UB2UIManager::GetInstance()->OpenMsgPopupFromErrorCode(811);
		return true;
		break;
	case b2network::B2ContentsModeState::SYSTEM_CHECK:
		// ������UI
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