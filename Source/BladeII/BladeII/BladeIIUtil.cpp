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
	// DLC 진입을 위한 코어 기능만 넣어놓음. 
	// 이것을 콜 하기 이전에 DeferredRegistChangeLobbyScene 을 넣어야 진행도 예측이 의도대로 될 것.

	//[@AKI, 180508] 앱이 재실행 되는 것 제외하고는 사용해서는 안됨!!!
	//데이터를 초기화 하지 않고 사용하고 있어서 안됨!! 사용시 데이터 초기화 하는 부분을 구현 해야 할 것

	FString DLCFrontMapPath;
	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIDLCMap"), DLCFrontMapPath, GGameIni);

	FB2NetworkBridge::GetInstance()->Shutdown();

	if (DLCFrontMapPath.Len() > 0)
	{
		// 순수 맵 로딩만의 예상 지분 세팅.
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
	// 로비 진입을 위한 코어 기능만 넣어놓음. 
	// 이것을 콜 하기 이전에 DeferredRegistChangeLobbyScene 을 넣어야 진행도 예측이 의도대로 될 것.

	FString LobbyMapPath;
	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIILobbyMap"), LobbyMapPath, GGameIni);

	if (LobbyMapPath.Len() > 0)
	{
		// 순수 맵 로딩만의 예상 지분 세팅.
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

	//// 다음 맵 로딩 진행도를 위해 순수 맵 로딩만의 예상 지분을 세팅
	//B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, ChangeGameMode);

	//// 주의: 다른 곳에서는 굳이 B2GameModule 에서 StageInfoTable 을 직접 가져다 쓸 필요가 없다.
	//// 여기는 StageManager 가 들고 있는 CurrentStageNumber 와 다른 StageInfoData 를 가져오기 위함.
	//// 여타 일반적인 사용에서는 가급적 AB2StageManager::GetCurrentStageInfoData 나 AB2StageGameMode::GetCurrentStageInfoData 를 사용

	//UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	//FSingleStageInfoData* StageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(InClientStageId) : NULL;
	//if (StageInfoData)
	//{
	//	// 클라이언트 테스트 용으로 StageNum 넘겨줌.
	//	BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = InClientStageId;

	//	// OnPreLoadMap 에서 Movie 시작 시점까지 잠깐 화면을 덮을 UI Screen.
	//	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, ChangeGameMode);

	//	UGameplayStatics::OpenLevel(InCurrentGameMode, StageInfoData->MainMap, true, OpenURLOption);
	//	return true;
	//}

	return false;
}

bool StartBladeIIPVPGame(class AGameMode* InCurrentGameMode, FString OpenURLOption)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIPVPGame"));

	// 다음 맵 로딩 진행도를 위해 순수 맵 로딩만의 예상 지분을 세팅
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::PVP_Tag);

	FString PVPMapName;

	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIPVPMap"), PVPMapName, GGameIni);

	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::PVP_Tag); // 로딩 스크린을 미리 한번 로딩해 주는 게 안전하다는 가설이 있어서 미리 한차례 해 준다..

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

	// 다음 맵 로딩 진행도를 위해 순수 맵 로딩만의 예상 지분을 세팅
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::PVP_Team);

	FString TeamMatchMapName;

	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIITeamMatchMap"), TeamMatchMapName, GGameIni);

	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::PVP_Team); // 로딩 스크린을 미리 한번 로딩해 주는 게 안전하다는 가설이 있어서 미리 한차례 해 준다..

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

	//// 다음 맵 로딩 진행도를 위해 순수 맵 로딩만의 예상 지분을 세팅
	//B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Control);

	//FString ControlMapName;

	//GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIControlMap"), ControlMapName, GGameIni);

	//OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::Control); // 로딩 스크린을 미리 한번 로딩해 주는 게 안전하다는 가설이 있어서 미리 한차례 해 준다..

	//UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*ControlMapName), true, OpenURLOption);
	return true;
}

bool StartBladeIIGuildMatchGame(class AGameMode* InCurrentGameMode, FString OpenURLOption /*= FString(TEXT(""))*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIGuildGame"));

	// 다음 맵 로딩 진행도를 위해 순수 맵 로딩만의 예상 지분을 세팅
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Guild);

	FString ControlMapName;

	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("BladeIIGuildMatchMap"), ControlMapName, GGameIni);

	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::Guild); // 로딩 스크린을 미리 한번 로딩해 주는 게 안전하다는 가설이 있어서 미리 한차례 해 준다..

	UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*ControlMapName), true, OpenURLOption);
	return true;

}

// 이거 대신 ABladeIIGameMode::GoToTutorial 이거 쓰는 듯..?
bool StartBladeIIGameTutorial(class AGameMode* InCurrentGameMode, FString OpenURLOption)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIGameTutorial"));

	// 다음 맵 로딩 진행도를 위해 순수 맵 로딩만의 예상 지분을 세팅
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

	// 다음 맵 로딩 진행도를 위해 순수 맵 로딩만의 예상 지분을 세팅
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::Dimension);

	UB2StageInfo* StageInfoTable = StaticFindStageInfo();
	FSingleStageInfoData* StageInfoData = StageInfoTable ? StageInfoTable->GetInfoData(MapLoadID) : NULL;
	if (StageInfoData)
	{
		// 클라이언트 테스트 용으로 StageNum 넘겨줌.
		BladeIIGameImpl::DevOnlyStageLoadData.SpecifiedClientStageId = MapLoadID;

		// OnPreLoadMap 에서 Movie 시작 시점까지 잠깐 화면을 덮을 UI Screen.
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
// 이 이하 StaticFind 들은 느릴 수 있으므로 가급적 BladeIIGameMode 에 캐싱된 걸 사용하고(WorldContextObject를 넘겨줌), 매 tick 사용하지 말 것.

UB2PCClassInfoBox* StaticFindPCClassInfoBox(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	//{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	//{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	{ // 가능한 경우 GameInstance 의 캐싱된 레퍼런스를 사용.
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
	//	// 실행 시간이 일정 기준 이상으로 오래 걸리면 경고로 표시.. 몰아서 찾아보기 좋게.. 대체로 이걸 설치하는 곳은 Tick 에서 불리는 건 아니어서 이벤트 성으로도 오래 걸린다 싶은 정도인 거를 걸러낸다.
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

	//	// 특별히 화면에 보기 쉽게 표시
	//	if (gDrawPerfMark && (bConsiderAsWarningLv1 || bConsiderAsWarningLv2))
	//	{
	//		BII_SCREEN_LOG(LogString, PerfMarkDrawTextColor, 12, 8.0f);
	//	}
	//}
}


// ScopedCallTracker 에도 시간 측정 기능이 들어갔는데 그렇다고 ScopedTimeLogger 를 쓸모없다고 할 필욘 없고 특별히 시간 측정이 필요한 스폿에 넣어서 쓰면 됨.
//
DEFINE_LOG_CATEGORY_STATIC(LogB2ScopedTimeLog, Log, All)
bool FB2ScopedTimeLogger::bIsOn = false; // 이건 기본값 false.. 필요할 때만 사용하려는 거
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
		// 실행 시간이 일정 기준 이상으로 오래 걸리면 경고로 표시.. 몰아서 찾아보기 좋게.. 대체로 이걸 설치하는 곳은 Tick 에서 불리는 건 아니어서 이벤트 성으로도 오래 걸린다 싶은 정도인 거를 걸러낸다.
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
		return; // 에디터에선 이런 짓을 해선 안된다. 모두 로딩된 상태 유지.
	}
#endif

	FlushAsyncLoading(); // 이걸 하는 게 안전할까?

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// TAsset 적용된 InfoAsset 들 중에서 별도의 레퍼런스 캐싱과 함께 FStreamableManager 를 쓰는 애들은 명시적으로 언로딩한다. 
	// FStreamableManager 의 문제점이기도 한데, 꼭 그거 아니더라도 Preload 의 필요성으로 레퍼런스 캐싱이 있다면 제거는 해 주어야 함.
	// 자잘한 애들은 FStreamableManager 대신 단순 LoadSynchronous 사용으로 전환하면 이거 안하게 될 것임.
	// 이거의 반대는 GameMode 의 PreloadAnyNecessaryInfo

	UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox(WorldContextObject); // 가능한 한 BladeIIGameMode 에 캐싱되어 있는 걸 가져오기 위해 WorldContextObject 를 넘겨줌.
	if (MobInfoBox)
	{
		MobInfoBox->UnloadExceptCurrentExplicit(B2GM);
	}

	UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox(WorldContextObject);
	if (PCClassInfoBox)
	{ // 설정과 플랫폼에 따라 unload 를 안하게 될 수도 있다.
		PCClassInfoBox->UnloadAllOrExceptSelectedIfAllowed();
	}
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{ // PCClassInfo 를 확실히 Unload 하려면 여기 데이터를 물고 있는 HeroDoc 도 안전하게 release 해 줄 필요가 있다.
		UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCI);
		if (DocHero)
		{
			DocHero->ReleaseResources();
		}
	}
	// 한때는 PCClassInfo 의 일부로 로딩되던 skill 애니메이션 데이터
	UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo(WorldContextObject);
	if (SkillAnimInfo)
	{
		SkillAnimInfo->UnloadAllOrExceptSelected(B2GM); // PCClassInfo 의 일부 격인 만큼 마찬가지로 UnloadAllOrExceptSelected 로
	}

	UB2UIDocScenarioDialog* DocScenarioDlg = UB2UIDocHelper::GetDocScenarioDialog();
	if (DocScenarioDlg)
	{
		DocScenarioDlg->ReleaseResources(); // 초상화 material 을 들고 있음.
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

	// UIManager 쪽은 여기서 Unload 안함. 괜히 그거 Destroy 시점하고 얽힐라..
	// UB2LoadingImageInfo 도 여기서 Unload 안 함. 그건 반대로 대략 이 시점에 필요한 걸 로딩하고 다음 레벨 로딩 후에 언로딩이 될 것.

	UE_LOG(LogBladeII, Log, TEXT("Unloaded lazy-loaded info assets"));

	// 기타 추가될 수 있음.
}

void ManualUnloadOnStageClear(UObject* WorldContextObject)
{
	B2_SCOPED_TRACK_LOG(TEXT("ManualUnloadOnStageClear"));

#if WITH_EDITOR
	if (GIsEditor)
	{
		return; // 에디터에선 이런 짓을 해선 안된다. 모두 로딩된 상태 유지.
	}
#endif

	FlushAsyncLoading(); // 이걸 하는 게 안전할까?

	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	// 스테이지 종료 연출 같은 거 나오면서 메모리 상승하기 직전 내릴 거 내리기.
	// 여기서 언로딩 하지 않은 것들도 ManualUnloadBeforeNewBeginning 에서 모두 내려갈 거

	// 스테이지 클리어 시점이라 MobInfo 는 더 이상 필요없을 것으로 간주. (설령 필요한 경우가 발생한다 하더라도 로딩 블로킹이 좀 생긴다 뿐)
	UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox(WorldContextObject); // 가능한 한 BladeIIGameMode 에 캐싱되어 있는 걸 가져오기 위해 WorldContextObject 를 넘겨줌.
	if (MobInfoBox)
	{
		MobInfoBox->UnloadExceptCurrentExplicit(B2GM);
	}

	UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox(WorldContextObject);
	if (PCInfoBox)
	{// 이 시점에서 태그는 물론 없어야 하고.. 승리 연출에서 태그캐릭터 puppet 을 spawn 할 수는 있는데 거기서 실질적인 PCClassInfo 데이터를 사용하지 않도록 해야 한다.
	 // ABladeIIGameMode::SpawnPlayerCharAsPuppet 에서부터 관련 인자를 넘겨주도록 되어 있음.
		PCInfoBox->UnloadAllOrExceptSelectedIfAllowed();
	}
	// 한때는 PCClassInfo 의 일부로 로딩되던 skill 애니메이션 데이터
	UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo(WorldContextObject);
	if (SkillAnimInfo)
	{
		SkillAnimInfo->UnloadAllOrExceptSelected(B2GM); // PCClassInfo 의 일부 격인 만큼 마찬가지로 UnloadAllOrExceptSelected 로
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

	// 기타 이젠 별 볼일 없어 보이는 것들 언로딩.


	// GC 도 한번 쿨하게 돌린다. 이걸 해야 실제로 메모리가 확보될 거.
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
	//	// 같은 클래스던지, 내지는 ObjCopyFrom 이 ObjCopyTo 의 하위 클래스던지.
	//	(ObjCopyTo->GetClass() != ObjCopyFrom->GetClass() && !ObjCopyFrom->IsA(ObjCopyTo->GetClass()))
	//	)
	//{
	//	return;
	//}

	// 참고 : FObjectInitializer::InitProperties
	// 이건 기본 serialize 후에 사용되는 것이므로 핵심 파트는 좀 단순화 되어 있음.

	//UClass* ObjClass = ObjCopyTo->GetClass();
	//BII_CHECK(ObjClass);
	//for (FProperty* P = ObjClass->PropertyLink; P; P = P->PropertyLinkNext)
	//{
	//	const bool bIsTransient = P->HasAnyPropertyFlags(CPF_Transient | CPF_DuplicateTransient | CPF_NonPIEDuplicateTransient);
	//	if (bIsTransient && bExcludeTransient) {
	//		continue;
	//	}

	//	// Actor 를 카피하는 경우 하위 Component 까지 카피하고 싶지는 않을 것이다. 웬만하면 따로 처리
	//	UObjectProperty* CastedObjProp = Cast<UObjectProperty>(P);
	//	if (bExcludeSubComponents && CastedObjProp && CastedObjProp->PropertyClass &&
	//		CastedObjProp->PropertyClass->GetDefaultObject()->IsA(UActorComponent::StaticClass())) {
	//		continue;
	//	}

	//	// 기타 특별히 제외하고 싶은 것들.
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
//	//UE4Game가 Root인듯 함.
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
////		//소문자 검사
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

// 아래 SetupNamedMIDForFxComp, GetFxNamedMIDCommon, SetupMIDAtlasParamFromUIMIC 를 사용해서 
// 일반적인 3D material 을 특정 UI material 과 마찬가지로 꾸미는 예시를 B2LobbyUI_ItemCUSProg 에서 참고..

void SetupNamedMIDForFxComp(UParticleSystemComponent* InPSComp)
{
	B2_SCOPED_TRACK_LOG(TEXT("SetupNamedMIDForFxComp"));

	if (!InPSComp || !InPSComp->Template) {
		return;
	}

	// FParticleEmitterInstance::Tick_MaterialOverrides 참고
	// NamedMaterialSlots 에 세팅한 것에 따라 같은 인덱스의 EmitterMaterials 가 있으면 그걸 사용하게 된다. 
	// Fx Component 생성하면서 순서대로 EmitterMaterials 에 MID 를 만들어서 넣어줌
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
			// FParticleEmitterInstance::Tick_MaterialOverrides 참고
			// NamedMaterialSlots 에 세팅한 것에 따라 같은 인덱스의 EmitterMaterials 가 있으면 그걸 사용하게 된다. 
			FNamedEmitterMaterial& CurrSlot = InPSComp->Template->NamedMaterialSlots[SI];
			if (CurrSlot.Name == MatSlotName && InPSComp->EmitterMaterials.IsValidIndex(SI))
			{
				// Fx 생성시 순서대로 EmitterMaterials 에 MID 생성해서 넣어놨어야 함. (SetupNamedMIDForFxComp)
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

// SetupMIDAtlasParamFromUIMIC 의 반복 파트
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
	// 물론 각각의 master material 에는 같은 이름의 파라미터가 같은 역할로 들어가 있어야겠다.
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleX);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleY);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleWidth);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasSampleHeight);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasWholeWidth);
	GetAndSetMIScalarParamCommon(MIDToSet, RefUIMIC, MtrlParamName_UIAtlasWholeHeight);
	// 마지막으로 텍스쳐 파라미터도.
	UTexture* ThisTextureParamValue = NULL;
	if (RefUIMIC->GetTextureParameterValue(MtrlParamName_UIAtlasTexture, ThisTextureParamValue)) {
		MIDToSet->SetTextureParameterValue(MtrlParamName_UIAtlasTexture, ThisTextureParamValue);
	}
}
// 위에 SetupMIDAtlasParamFromUIMIC 랑 동일한 마스터 머티리얼 가정에 마찬가지 기능인데 NoAtlas static switch 로 사용하는 거
void SetupMIDNonAtlasParamFromUIMIC(class UMaterialInstanceDynamic* MIDToSet, class UMaterialInstanceConstant* RefUIMIC)
{
	B2_SCOPED_TRACK_LOG(TEXT("SetupMIDNonAtlasParamFromUIMIC"));

	if (!MIDToSet || !RefUIMIC) {
		return;
	}
	// 텍스쳐 파라미터만 있으면 됨.
	UTexture* ThisTextureParamValue = NULL;
	if (RefUIMIC->GetTextureParameterValue(MtrlParamName_NoAtlasSingleTexture, ThisTextureParamValue)) {
		MIDToSet->SetTextureParameterValue(MtrlParamName_NoAtlasSingleTexture, ThisTextureParamValue);
		MIDToSet->SetTextureParameterValue(MtrlParamName_UIAtlasTexture, nullptr); // 어차피 표시되지는 않겠지만 확실히 날리는 게 좀 더 좋을 거 같아서 
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
	// 서버에서 사용하는 RoundDown 공식
	return FPlatformMath::FloorToInt(a * FMath::Pow(10.f, NumDigit)) / FMath::Pow(10.f, NumDigit);
}

////////////////////////////////////////////////////
// Other miscellaneous utilities.

void GetTimeFormatFromRawSecond(int32 InRawSecond, int32& OutHour, int32& OutMin, int32& OutSec)
{
	OutHour = InRawSecond / 3600;

	int32 RemainingSecond = InRawSecond - OutHour * 3600; // 1시간 미만의 나머지 초

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

	// bFixInAir플래그로 인해 무브먼트껐으면 패스
	if (!InB2Char->GetCharacterMovement()->bAutoActivate)
		return;

	// Refered to UEditorEngine::SnapObjectTo
	// It doesn't have to be highly precise. Could be rough snap at the moment of spawn. They will fall to the ground by gravity anyway.

	if (InB2Char)
	{
		FVector ActorBoundOrigin;
		FVector ActorBoundBoxExtent;
		// Sometimes it could be better to use RootComponent's Bounds. Character 에 설치된 snapping 을 방해할 만한 다른 것들의 영향을 줄이기 위해 루트인 Capsule component 에서 bound 값을 가져옴.
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
	// 이건 특히 플레이어 캐릭터에 붙은 20/30렙 및 6성 무기 particle effect 가 잘 안나오곤 하는 이슈에 대응하기 위해 준비되었음.
	// AB2StageEventDirector::ActivateMatineePuppetPSCs 와 마찬가지 맥락.
	// 단순히 Hidden 해제 이후 Activate 를 시키는데 정확히 무엇이 문제되었는지는 알지 못한다.

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
{ // 특정하게 사용하는 곳이 있음.
	if (InPSC) {
		InPSC->SetVisibility(true);
		InPSC->SetHiddenInGame(false);
		InPSC->SecondsBeforeInactive = BIG_NUMBER; // 이것 때문에 문제가 된 상황이 이전에 어떤 영상 씬에서 발견된 적은 있었는데 이건 걍 추가로 도움이 될까 싶어서..
		InPSC->Activate(true);
	}
}

bool IsStaticallyVisiblePSC(class UParticleSystemComponent* InPSC)
{ // 연출이나 기타의 경우에 visibility 핸들링 시 처리 목록에 넣을 애들. 이게 아니면 anim notify 를 통해 일시적으로 붙은 걸로 간주.
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

// GameplayStatics.cpp 에 있는 로컬 유틸 함수를 가져옴. 일반적인 경우에는 UGameplayStatics::SpawnEmitter 씨리즈를 쓰도록.
UParticleSystemComponent* CreateParticleSystemHelper(UParticleSystem* EmitterTemplate, UWorld* World, AActor* Actor, bool bAutoDestroy)
{
	UParticleSystemComponent* PSC = NewObject<UParticleSystemComponent>((Actor ? Actor : (UObject*)World));
	PSC->bAutoDestroy = bAutoDestroy;
	// 몇몇 기타 변수들은 임의로 채워넣는다. 대체로 인게임 dynamic spawn 에 쓰기 적절한 설정인건가..
	PSC->bAllowAnyoneToDestroyMe = true;
	PSC->SecondsBeforeInactive = 0.0f;
	PSC->bAutoActivate = false;
	PSC->SetTemplate(EmitterTemplate);
	PSC->bOverrideLODMethod = false;

	return PSC;
}

UCameraComponent* GetCurrentActiveCamComp(UObject* WorldContextObject)
{ // 현재 뷰 렌더링에 쓰이고 있는 카메라 쿼리. 예를 들어 일반적인 플레이어 전투 뷰라면 플레이어에 달려 있는 카메라.
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
	//// 인게임 전투 상황에서 현재 적절한 카메라 워크로 돌아갈 수 있도록 하는 기능.
	//// 일반적으로 이걸 콜해야 카메라가 적용되는 건 아니고 일시적으로 특수한 카메라를 썼다가 돌아가는 등의 상황에서 쓸 수 있다.

	//if (InB2GM && InLocalPC)
	//{
	//	check(InLocalPC == Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(InB2GM)));

	//	if (!InB2GM->ApplyActiveCameraIfAny(InBlendInTime)) // 특정 구역에 설치한 ActiveCamera 적용 시도.
	//	{
	//		// 딱히 현재 적용되는 ActiveCamera 가 없다면 월드 기본 카메라로.
	//		// 여기 블렌딩 설정은 따로 가는데 BlendFunction 도 필요하면 인자로 넘겨 주거나.. 이 상황에선 Cubic 이 존나 적절해 보여서 쳐넣어봄.
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
			ClampedStr += TEXT("..."); // 말줄임표 붙이는 옵션
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
	// 소수점 InMaxFracDigit 까지 허용하되 끝자리 0 은 유효숫자로 안 치고 생략.
	NumberDisplayFormat.SetMinimumFractionalDigits(0);
	NumberDisplayFormat.SetMaximumFractionalDigits(InMaxFracDigit);

	return FText::AsNumber(InNumber, &NumberDisplayFormat);
}

FText GetAutoFractionalFormattedText_FirstFracCountOnly(float InCountNumber, float InMaxNumber, int32 InMaxFracDigit)
{
	// 유틸이라기보다는 특정한 케이스에 사용하는 기능인데 사용할 곳이 여러 곳 있어서 여기에 빼 놓는다.
	// 최대 쿨타임이 소수점으로 조정된 경우를 표시하기 위해 첫 소수점을 카운트하는 동안에만 소수점을 표시하기위한 용도. 계속 표시하면 소수점이 나타났다 사라졌다 해서 정신없다 보니

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
	// OutAllFound 를 리셋하지 않고 사용..

	//if (!InSKComp || !InSKComp->GetAnimInstance()) {
	//	return;
	//}

	//B2_SCOPED_TRACK_LOG(TEXT("TryGatherAllBoundPSFromSKComp"));
	//B2_SCOPED_TIME_LOG(FString::Printf(TEXT("TryGatherAllBoundPSFromSKComp %s"), InSKComp->GetOwner() ? *(InSKComp->GetOwner()->GetName()) : *InSKComp->GetName()));

	//// 적절히 초기화가 된 AnimInstance 에서만 먹힐 것이다.
	//UAnimInstance* TheAnimInst = InSKComp->GetAnimInstance();

	//TArray<UAnimationAsset*> AllPlayableAnims;
	//TheAnimInst->GetAllAnimAssets(AllPlayableAnims);

	//for (UAnimationAsset* ThisAnim : AllPlayableAnims)
	//{
	//	// BlendSpace 를 넣어야 하는데., 일단 요 함수의 사용 목적 상 실질적으로 의미있는 ParticleSystem 들은 단일 AnimSequence 에 들어가 있을 듯.
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
		{ // 이미 추가된 거 아닌지 검사. TArray 로 AddUnique 하면 넘 느릴까봐 PathName 을 Key 로 한 맵으로..
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
	// ParticleSystem 을 사용하는 종류의 AnimNotify(State) 로.. 일단 아는 건 두가지..
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

/** 한번 호출에 한 프레임 그려짐. 필요한 기간 동안 매 프레임 호출 */
void DrawDebugText(float InStartX, float InStartY, const FString& InText, const FLinearColor& InFontColor, int32 InFontSize, bool bDrawShadow, const FLinearColor& InShadowColor)
{
#if WITH_BII_ON_SCREEN_DEBUG_TEXT
	if (UB2UIManager::IsInstanceNull()) { // 여기서 GetInstance 를 할 때에 생성이 되도록 하지 않는다.
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
/** 이건 한번만 콜하면 정해진 시간동안 디스플레이 된다. 대신 위치는 정해져 있음 */
void DrawTimedDebugText(const FString& InText, const FLinearColor& InFontColor, int32 InFontSize, float InDisplayTime, bool bDrawShadow, const FLinearColor& InShadowColor)
{
	//if (UB2UIManager::IsInstanceNull()) { // 여기서 GetInstance 를 할 때에 생성이 되도록 하지 않는다. 특히 FB2ScopedCallTracker 에 사용되기 때문에 UIManager 가 Unload 된 시점에도 불리게 될 수 있다.
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
//	// 여기 걸리면 모바일 프리뷰. 몇몇 게임상의 렌더링 프리뷰 등을 위한 거라 잘못된다고 해서 최종 기능에 영향을 미치는 건 아니다.
//	ERHIFeatureLevel::Type RHIFeatureLevel = (InWorld && InWorld->Scene) ? InWorld->Scene->GetFeatureLevel() : ERHIFeatureLevel::Num;
//	return (RHIFeatureLevel == ERHIFeatureLevel::ES2 || RHIFeatureLevel == ERHIFeatureLevel::ES3_1);
//#endif
}

void TurnOffMeshComponentsDyamicShadowForModulated(AActor* InOwnerActor)
{
	////////////////////////////////////////
	//// InteractiveActor 와 같이 BladeII 자체적인 하위에 movable MeshComponent 들이 있는 Actor 를 위한 dynamic shadow 속성 컨트롤 유틸.
	//// Modulated shadow 가 좀 문제가 있어서 딱히 필요도 없고 문제만 일으킬 소지가 있는 것들은 shadow 를 드리우지 않도록 하기 위함.
	//// Modulated shadow 를 대체하는 괜찮은 수단이 나오기 전까지..
	//// 근데 배경이 static lighting 에 기반하고 있는 한 어차피 modulated shadow 외에 적절한 대안을 찾기도 어려움 ㅋ
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
	// 디바이스에서 간혹 가다 보이는 렌더링 버그에 대처하는 차원에서..
	// 만든 시점에 이게 실제로 효과가 있는지는 모름. 재현이 되고 테스트해서 먹히면 적절한 타이밍에 사용하게 되는 거..

	IConsoleVariable* CVarSendSignal = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ReallocateRenderTargetsSignal"));
	if (CVarSendSignal)
	{ // 단순히 하나 증가시킨 걸 시그널로 씀 ㅡㅡ;
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
			//// 세션 서버와 접속이 끊김
			//this->SetState(CONNECTION_STATE_SERVICENOTAVAILABLE);

			//UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			//if (UIMgrInst)
			//{
			//	if (this->GetSessionCloseReason() == ESessionCloseReason::ESessionCloseReason_None)
			//	{
			//		data_trader::DisconnectReasonLogEventClass<int32>::GetInstance().Signal(error);
			//		auto* gameMode = GetBladeIIGameMode(UIMgrInst);

			//		if (gameMode && gameMode->GetB2GameModeType() == EB2GameMode::PVP_Tag) //PVP Matching일때만 로비로 보내기 위해 예외 처리. 함수 이름이 안어울리지만....
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
			// TODO: 채널 서버와 연결 끊김, PVP/Raid/Assault 등 관련 게임 모드 중단됨
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

			//[@AKI, 170619] 여기는 여러번 들어 올수 잇으므로 한번만 들어 오게 함
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
// GDC 이후엔 이거 사용한 부분 색출해내서 모두 없앤다. 걍 우리코드에서 GDC 검색해서 나오는 거 다 체크.
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
/** CBTVersionReady 가 true 인 상태로 쿠킹 테스트할 때 컨텐츠가 막혀서 당황스러운 상황 대비.
//* 이걸 변경하는 콘솔 커맨드가 있음. */
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
	// 여기에 따라 실질적으로 적용되는 온갖 엔진 그래픽 옵션들은 B2Scalability 를 참고
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
	// 여기에 따라 실질적으로 적용되는 온갖 엔진 그래픽 옵션들은 B2Scalability 를 참고
	// EB2GraphicsLevel enum 순서 바꾸는 와중에.. 설정 새로 로딩하도록 "GameGraphicLevel_N" 으로 바꿈. 일정 시간 지나고 나서 다시 "GameGraphicLevel" 로 되돌릴 것.
	return (GConfig && GConfig->GetInt(TEXT("GameSetting"), *FString::Printf(TEXT("GameGraphicLevel")), OutValue, GB2GeneralSavedStateIni));
}
bool LoadGameSetting_Graphics_OrByDefault(int32& OutValue)
{
	//if (!LoadGameSetting_Graphics(OutValue)) {
	//	// 아직 사용자 선택이 없었다면 DeviceProfile 에 의해 detect 된 엔진 scalability 시스템 기본 설정값으로.
	//	OutValue = B2GraphicsLevelToInt(B2Scalability::GetGraphicsLevelByExpectedScalability());
	//	return false; // 여하간 저장된 사용자 선택값은 없었음을 리턴함.
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
		// 아직 사용자 설정 저장이 없었다면 DeviceProfile 에 의해 들어가 있을 현재 값..
		OutValue = B2FrameLevelToInt(B2Scalability::GetDefaultRelativeFrameLimitLevel());
		return false; // 여하간 저장된 사용자 선택값은 없었음을 리턴함.
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
		// 아직 사용자 설정 저장이 없었다면 DeviceProfile 에 의해 들어가 있을 기본 값.
		OutValue = B2ResolutionLevelToInt(B2Scalability::GetResolutionLevelOfRQType(GetDefaultGraphicsRQType()));
		return false; // 여하간 저장된 사용자 선택값은 없었음을 리턴함.
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
			check(CastedSaveValue >= 0); // 음수는 클리어 된 걸로 인식할 것.
			GConfig->SetInt(TEXT("GameSetting"), TEXT("ReservedResolutionLevel"), CastedSaveValue, GB2GeneralSavedStateIni);
		}
		else
		{ // 이건 클리어에 해당.
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
	return EB2ResolutionLevel::End; // 이건 reserve 안 되어 있다는 의미로.
}

void InitializeAllGameSettingData(UObject* WorldContextObject, bool bExtraContentScaleFallback)
{
	//// High-level 게임 세팅 데이터들을 읽어들이거나, 없으면 초기값으로 저장 및 적용.
	//// 게임 스타트업, 내지는 첫 BladeIIGameMode 시작 시점에
	//bool bSavedSome = false;

	//int32 LoadedGraphicsLevel = B2GraphicsLevelToInt(EB2GraphicsLevel::GraphicsLevel_HIGH);
	//if (!LoadGameSetting_Graphics_OrByDefault(LoadedGraphicsLevel))
	//{
	//	// 이건 로딩 실패 시 기본값을 여기서 넣을 필요는 없다. LoadGameSetting_Graphics_OrByDefault 에서 처리될 것.
	//	SaveGameSetting_Graphics(LoadedGraphicsLevel);
	//	bSavedSome = true;
	//}
	//int32 LoadedFrameLimitLevel = B2FrameLevelToInt(EB2FrameLimitLevel::Mid); // 어지간하면 이건 High 를 기본값으로 안 하는 게 좋을꺼다 낄낄
	//if (!LoadGameSetting_FrameLimit_OrByDefault(LoadedFrameLimitLevel))
	//{
	//	// 이건 로딩 실패 시 기본값을 여기서 넣을 필요는 없다. LoadGameSetting_FrameLimit_OrByDefault 에서 처리될 것.
	//	SaveGameSetting_FrameLimit(LoadedFrameLimitLevel);
	//	bSavedSome = true;
	//}
	//int32 LoadedResolutionLevel = B2ResolutionLevelToInt(EB2ResolutionLevel::Mid);  // 어지간하면 이건 High 를 기본값으로 안 하는 게 좋을꺼다 히히
	//if (!LoadGameSetting_Resolution_OrByDefault(LoadedResolutionLevel))
	//{
	//	// 이건 로딩 실패 시 기본값을 여기서 넣을 필요는 없다. LoadGameSetting_Resolution_OrByDefault 에서 처리될 것.
	//	SaveGameSetting_Resolution(LoadedResolutionLevel);
	//	bSavedSome = true;
	//}
	//// 이전 실행에서 런타임 스위칭이 안되는 해상도 변경을 한 경우. 별도 필드에 저장되어 있는 걸 적용.
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
	//	LoadedBGMVolume = 0.7f; // 기본값
	//	SaveGameSetting_BGMVolume(LoadedBGMVolume);
	//	bSavedSome = true;
	//}
	//float LoadedFxVolume = 1.0f;
	//if (!LoadGameSetting_EffectVolume(LoadedFxVolume))
	//{
	//	LoadedFxVolume = 0.7f; // 기본값
	//	SaveGameSetting_EffectVolume(LoadedFxVolume);
	//	bSavedSome = true;
	//}
	//int32 LoadedVibrationScale = 1;
	//if (!LoadGameSetting_Vibration(LoadedVibrationScale))
	//{
	//	LoadedVibrationScale = 1; // 기본값
	//	SaveGameSetting_Vibration(LoadedVibrationScale);
	//	bSavedSome = true;
	//}

	//int32 LoadedKeepScreenOn = 1;
	//if (!LoadGameSetting_KeepScreenOn(LoadedKeepScreenOn))
	//{
	//	LoadedKeepScreenOn = 1; // 기본값
	//	SaveGameSetting_KeepScreenOn(LoadedKeepScreenOn);
	//	bSavedSome = true;
	//}

	//int32 LoadGamePush = 1;
	//if (!LoadGameSetting_GamePush(LoadGamePush))
	//{
	//	LoadedKeepScreenOn = 1; // 기본값
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

	//// 엔진 초기화 시점에 불리는 곳이기도 하고 관련된 코드기도 하니 다른 모듈에서 사용을 위해 함수 포인터 할당. 
	//gLoadGameFxVolumeSettingFnPtr = LoadGameSetting_EffectVolume;
	//gLoadGameBGMVolumeSettingFnPtr = LoadGameSetting_BGMVolume;

	//if (bSavedSome)
	//{
	//	GConfig->Flush(false, GB2UserSavedStateIni);
	//	GConfig->Flush(false, GB2GeneralSavedStateIni);
	//}

	//// WorldContextObject 는 게임모드 별 bias 를 위한 거라 모듈 시작이나 DLCFront 에서 불리는 거면 필요가 없다.
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
	// 이거 나중에 서버도 저장해야함 [@AKI, 171220] 용운님에게 확인결과 서버에 저장이 필요 없음
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
{ // 게임 사운드 옵션 조절에 사용. 실제 저장을 하지는 않고 그건 SaveGameSetting_ 으로 따로. 주의 : Object iterate 함.
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
	// 4.13 에서 WrapBox 와 PaperSprite 를 사용한 image widget batching 을 활용하기 위해 임시로 넣음.
	// 왜 임시냐면 UImage 쪽에서 SetBrushFromMaterial 에 대응하는 SetBrushFromPaperSprite 같은 걸 제공할 거라 기대하기 때문.
	// 지금 당장은 UMG 쪽에 PaperSprite 코드를 넣을 수가 없군..
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
	//	// MaterialInstance 를 세팅한 UImage 는 GetDynamicMaterial 을 통해 MID 를 가져와서 파라미터를 세팅할 수 있는데 
	//	// PaperSprite 는 현재로선 (4.14) 그런 게 안먹힘.
	//	// 단 컬러 블렌딩까지는 UImage 에 있는 걸로 할 수 있다. PaperSpriteComponent 도 컬러 브렌딩 정도만 지원하는 듯함.
	//	InImageWithPaperSprite->SetColorAndOpacity(InBlendColor);
	//	InImageWithPaperSprite->GetBrush().TintColor = InBlendColor; // SetColorAndOpacity 로는 안먹히곤 해서.. Tint 도 설정.
	//	InImageWithPaperSprite->SetBrush(InImageWithPaperSprite->Brush);
	//}
}

/**
* MatineeActor 도 인자로 받는 마당에 연출용으로 보면 된다. 넣어주는 DataStore 에 맞춰서 플레이어 캐릭터의 외관을 한 SkeletalMeshActor spawn.
* StageEventDirector 쪽에서 하는 일과 비슷한 일을 하는데 그걸 타지 않는 별도의 연출 기능을 사용하게 된 곳이 있어서 이 함수가 따로 만들어짐.
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
////				false, // 글쎄, 혹시 전투 시작 전에 사용할 가능성을 생각한다면 결과물 mesh 를 캐싱하지도 않는 판에 원본 리소스라도 메모리에 남겨 두면 좋을 것 같아서..
////					   // 이거 용도가 인게임 플레이어 캐릭터가 아닌 모드의 EntryActor spawn.. 내지는 다른 사용이라도 연출용일꺼라 해당 함수로 판별
////				UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(EntryActor)
////			))
////			{
////				// 여기선 merge 된 mesh 를 캐싱하면 괜히 ID 꼬이고 해서 문제만 될 수 있으므로 그냥 넘어감. 그래도 맵 로딩 다시 하는 게 아니라면 해 두면 좋긴 하다. 전투 진입하면서 다시 안 해도 되니.
////			}
////
////			ABladeIIPlayer::SetupLODInfoForLobbyRepresentative(EntryActor); // 연출용 LOD == 로비용 LOD
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

/** UE 4.15 까지 UNavigationSystem::GetRandomReachablePointInRadius static 버전이 있어서 그걸 꿀빨면서 쓰고 있었는데
* 4.16 부터 deprecated 되면서 결국 이렇게 똑같이 구현해 놓음. */
FVector GetRandomReachablePointInRadiusWithCurrNavSys(UWorld* InWorld, const FVector& Origin, float Radius, ANavigationData* NavData, TSubclassOf<UNavigationQueryFilter> FilterClass)
{
	FNavLocation RandomPoint;

	//UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(InWorld); // 단 원본 GetRandomReachablePointInRadius 에서는 WorldContextObject 를 받았는데 여기선 그냥 World 로 받는다.
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
		FString::Printf(TEXT("InfoAsset %s 에 등록된 TAsset %s 로딩 실패. 컴퓨터가 곧 폭발한다."), *InfoAssetName, *WarningAssetName)
	));
#endif
}
#endif

#if !UE_BUILD_SHIPPING
void CheckResetAssumeNoWorldLoadingCondition(UObject* WorldContextObject)
{ // GameMode 및 몇몇 액터들에 들어간 ResetAssumeNoWorldLoading 기능의 실제 사용 조건 체크. 아무데서나 사용이 가능할 정도로 견고한 기능은 아니므로..
#if WITH_EDITOR
	if (GIsEditor) { // 에디터에서만 테스트를 위해 다른 게임모드에서의 사용을 허가함.
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
	//임시 일단 서울로..
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
		// 암것도 안함. 
		break;
	case b2network::B2ContentsModeState::BLOCK_ENTER:
	case b2network::B2ContentsModeState::MODE_FINISH:
	case b2network::B2ContentsModeState::SETTLE_BEGIN:
	case b2network::B2ContentsModeState::SETTLE_END:
		UB2UIManager::GetInstance()->OpenMsgPopupFromErrorCode(811);
		return true;
		break;
	case b2network::B2ContentsModeState::SYSTEM_CHECK:
		// 점검중UI
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