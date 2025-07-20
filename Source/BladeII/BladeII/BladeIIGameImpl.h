

#pragma once
//#include "BladeII.h"
#ifndef __BLADEIIGAMEIMPL_H__
#define __BLADEIIGAMEIMPL_H__


#include "B2StageDataStore.h"
#include "B2GuildDataStore.h"
#include "RelicManager.h"
#include "UnitySkillManager.h"
#include "BannerManager.h"
#include "B2Chatting.h"
#include "B2CostumeDataStore.h"
#include "B2TotemDataStore.h"
#include "B2ClientDataStore.h"
#include "B2EtherContainer.h"

class FClientDataStore;

/** To transfer some data for stage loading. It is DEVELOPMENT ONLY, but available for shipping build */
struct FDevOnlyStageLoadData
{
	FDevOnlyStageLoadData()
	{
		ResetAll();
	}

	/** Stage number specified by StartBladeIIGameStage function. To pass it without using URL option. */
	int32 SpecifiedClientStageId;


	// To be called at StageManager's BeginPlay timing after utilizing data here.
	void ResetAll()
	{
		SpecifiedClientStageId = -1;
	}
};

class BladeIIGameImpl : public FDefaultGameModuleImpl
{
	////////////////////////////////////////////////////////////////////////////////
	// DataTable based info asset objects. They are rootset for some reasons.
protected:
	UPROPERTY()
	class UB2StageInfo* StageInfoTable;
public:
	/** Create if not created yet. */
	class UB2StageInfo* GetStageInfoTable();
	void DestroyStageInfoTable();

protected:
	UPROPERTY()
	class UB2ChapterInfo* ChapterInfoTable;
public:
	class UB2ChapterInfo* GetChapterInfoTable();
	void DestroyChapterInfoTable();

protected:
	/** Client side item data and resources info. */
	UPROPERTY()
	class UB2ItemInfo* ItemInfoTable;
public:
	class UB2ItemInfo* GetItemInfoTable();
	void DestroyItemInfoTable();

protected:
	UPROPERTY()
	class UB2ScenarioDialogInfo* DialogInfoTable;
public:
	class UB2ScenarioDialogInfo* GetDialogInfoTable();
	void DestroyDialogInfoTable();

protected:
	UPROPERTY()
	class UB2DialogSoundInfoSet* DialogSoundInfoTable;
public:
	class UB2DialogSoundInfoSet* GetDialogSoundInfoTable();
	void DestroyDialogSoundInfoTable();

protected:
	UPROPERTY()
	class UB2NPCSensitiveInfo* NPCSensitiveInfoTable;
public:
	/** Create if not created yet. */
	class UB2NPCSensitiveInfo* GetNPCSensitiveInfoTable();
	void DestroyNPCSensitiveInfoTable();

protected:
	UPROPERTY()
	class UB2BrevetInfo* BrevetInfoTable;
public:
	/** Create if not created yet. */
	class UB2BrevetInfo* GetBrevetInfoTable();
	void DestroyBrevetInfoTable();

protected:
	UPROPERTY()
	class UB2ErrorCodeInfo* ErrorCodeInfoTable;
public:
	/** Create if not created yet. */
	class UB2ErrorCodeInfo* GetErrorCodeInfoTable();
	void DestroyErrorCodeInfoTable();
	////////////////////////////////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2QuestInfoTable* QuestTable;
public:
	/** Create if not created yet. */
	class UB2QuestInfoTable* GetQuestInfoTable();
	void DestroyQuestInfoTable();
	////////////////////////////////////////////////////////////////////////////////
protected:
	UPROPERTY()
	class UB2QuestNPCSoundInfo* QuestNPCSoundInfoTable;
public:
	/** Create if not created yet. */
	class UB2QuestNPCSoundInfo* GetQuestNPCSoundInfoTable();
	void DestroyQuestNPCSoundInfoTable();
	////////////////////////////////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2MissionInfo* MissionInfoTable;
public:
	/** Create if not created yet. */
	class UB2MissionInfo* GetMissionInfoTable();
	void DestroyMissionInfoTable();
	////////////////////////////////////////////////////////////////////////////////
protected:
	UPROPERTY()
	class UB2RewardInfo* RewardInfoTable;
public:
	/** Create if not created yet. */
	class UB2RewardInfo* GetRewardInfoTable();
	void DestroyRewardInfoTable();
	////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2RaidInfoTable* RaidInfoTable;
public:
	/** Create if not created yet. */
	class UB2RaidInfoTable* GetRaidInfoTable();
	void DestroyRaidInfoTable();
	////////////////////////////////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2TalkInfo* TalkInfoTable;
public:
	/** Create if not created yet. */
	class UB2TalkInfo* GetTalkInfoTable();
	void DestroyTalkInfoTable();
	////////////////////////////////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2DialogSoundInfoSet* TalkSoundInfoTable;
public:
	class UB2DialogSoundInfoSet* GetTalkSoundInfoTable();
	void DestroyTalkSoundInfoTable();
	////////////////////////////////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2FairyInfo* FairyInfoTable;
public:
	class UB2FairyInfo* GetFairyInfoTable();
	void DestroyFairyInfoTable();
	////////////////////////////////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2ChattingFilteringInfo* ChattingFilteringInfoTable;
public:
	/** Create if not created yet. */
	class UB2ChattingFilteringInfo* GetChattingFilteringInfoTable();
	void DestroyChattingFilteringInfoTable();
	////////////////////////////////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2GoodsTypeInfo* GoodsTypeInfoTable;
public:
	/** Create if not created yet. */
	class UB2GoodsTypeInfo* GetGoodsTypeInfoTable();
	void DestroyGoodsTypeInfoTable();
	////////////////////////////////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2GoodsShortcutInfo* GoodsShortcutInfo;
public:
	/** Create if not created yet. */
	class UB2GoodsShortcutInfo* GetGoodsShortcutInfo();
	void DestroyGoodsShortcutInfo();
	////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2EtherInfo* EtherInfoTable;
	UPROPERTY()
	class UB2EtherSetOptionInfo* EtherSetOptionInfoTable;
	UPROPERTY()
	class UB2EtherSetInfoTable* EtherSetEffectInfo;
	UPROPERTY()
	class UB2SelectiveEtherLottery* SelectiveEtherLotteryTable;

public:
	/** Create if not created yet. */
	class UB2EtherInfo* GetEtherInfoTable();
	void DestroyEtherInfoTable();

	class UB2SelectiveEtherLottery* GetSelectiveEtherLotteryTable();
	void DestroySelectiveEtherLotteryTable();

	class UB2EtherSetOptionInfo* GetEtherSetOptionInfoTable();
	void DestroyEtherSetOptionInfoTable();

	class UB2EtherSetInfoTable* GetEtherSetEffectInfo();
	void DestroyEtherSetEffectInfo();
	////////////////////////////////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2TotemRefineInfo* TotemRefineInfoTable;
public:
	/** Create if not created yet. */
	class UB2TotemRefineInfo* GetTotemRefineInfoTable();
	void DestroyTotemRefineInfoTable();
	////////////////////////////////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UB2ItemSortInfo* ItemSortInfoTable;
public:
	/** Create if not created yet. */
	class UB2ItemSortInfo* GetItemSortInfoTable();
	void DestroyItemSortInfoTable();
	////////////////////////////////////////////////////////////////////////////////

public:
	UPROPERTY()
	class UB2CharacterTagDialogueInfo* CharacterTagDialogueInfoTable;

	/** Create if not created yet. */
	class UB2CharacterTagDialogueInfo* GetCharacterTagDialogueInfoTable();
	void DestroyCharacterTagDialogueInfoTable();
	////////////////////////////////////////////////////////////////////////////////

public:
	UPROPERTY()
	class UB2SeasonEventInfo* SeasonEventInfoTable;

	/** Create if not created yet. */
	class UB2SeasonEventInfo* GetSeasonEventInfoTable();
	void DestroySeasonEventInfoTable();
	////////////////////////////////////////////////////////////////////////////////

protected:
	/** It's a UObject, but not one of InfoAsset, runtime created player character's composite mesh management system. */
	UPROPERTY()
	class UB2CompositeMeshCache* CompositeMeshCacher;
public:
	/** Create if not created yet. */
	class UB2CompositeMeshCache* GetCompositeMeshCacher();
	void DestroyCompositeMeshCacher();

	////////////////////////////////////////////////////////////////////////////////

protected:
	UPROPERTY()
	class UBannerManager* LobbyBannerCacher;

public:
	class UBannerManager* GetLobbyBannerCacher();
	void DestroyLobbyBannerCacher();

	////////////////////////////////////////////////////////////////////////////////
protected:
	/** Where some unordered various server originated client data are placed. */
	static FClientDataStore ClientDataStore;
	static FStageDataStore StageDataStore;
	static FB2ChatStore ChatStore;
	static RelicManager m_RelicManager;
	static UnitySkillManager m_UnitySkillMissionManager;
	static FGuildDataStore GuildDataStore;
	static FCostumeDataStore CostumeDataStore;
	static FTotemDataStore TotemDataStore;

public:

	/** Development only, but still valid in shipping build. Variables here to be reset at StageManager's BeginPlay */
	static FDevOnlyStageLoadData DevOnlyStageLoadData;

public:

	BladeIIGameImpl();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static void LoadIniSystemSettings();

	void HandleCorePreExit();

#if PLATFORM_ANDROID
	void OnAndroidDestroyWindow();
#endif
//
	static FClientDataStore& GetClientDataStore();
	static FStageDataStore& GetStageDataStore();
	static FLocalCharacterData& GetLocalCharacterData();
	static FTutorialCharacterData& GetTutorialCharacterData();
	static FB2RedDotManager& GetRedDotManager();
	static FB2ChatStore& GetChatStore();
	static RelicManager& GetRelicManager();
	static UnitySkillManager& GetUnitySkillMissionManager();
	static FGuildDataStore& GetGuildDataStore();
	static FCostumeDataStore& GetCostumeDataStore();
	static FTotemDataStore& GetTotemDataStore();
public:

	static FName GetModuleName() {
		return FName(TEXT("BladeII"));
	}

#if WITH_EDITOR
	void HandleEditorClosePIE(bool bIsSimulating);
#endif

private:
	bool IsNetFeaturekInitialized;
public:
	/** Could be just a test flag, but allow it being turn on/off until we sure that it works safely. */
	static bool bAllowInfoAssetAsyncLoading;
	/** Technically not to different from other InfoAsset async loading, but a little different by intended usage. */
	static bool bAllowUIAsyncLoading;
	/** Subset of AllowUIAsyncLoading, especially trying it on specific UI page open.. */
	static bool bAllowUIOnDemandAsyncLoading;
	/** Better not allow taking too much time */
	static float UIOnDemandAsyncLoadingFlushTimeLimit;
};


#endif