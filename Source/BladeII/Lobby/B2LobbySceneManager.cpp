
#include "B2LobbySceneManager.h"
//#include "BladeII.h"
//#include "Engine/World.h"
//#include "B2LobbyGameMode.h"
//
//#include "B2LobbyInventory.h"
#include "B2PCClassInfoBox.h"
#include "BladeIIPlayer.h"
//#include "Engine/World.h"
//#include "B2LobbySkeletalMeshActor.h"
//#include "B2HeroMgntSkeletalMeshActor.h"
//#include "B2UIManager.h"
//#include "B2UIManager_Lobby.h"
////#include "Matinee/MatineeActor.h"
//#include "Matinee/InterpGroup.h"
//#include "Matinee/InterpGroupInst.h"
//#include "Matinee/InterpTrackMove.h"
//#include "Matinee/InterpData.h"
//#include "B2UnitedWidgetBase.h"
//#include "B2UIModSelect.h"
//#include "B2UIDocHelper.h"
//#include "B2UIHeroTowerReady.h"
#include "Retailer.h"
//#include "B2UIMsgPopupGuildPopup.h"
//#include "B2UICollectBookMain.h"
//#include "B2UIGuildMain.h"
//#include "B2UIRaidMain.h"
//#include "B2UIRaidJoin.h"
//#include "B2UIGameInviteList.h"
//#include "B2ModPlayerInfoData.h"
//#include "B2CombatStatEvaluator.h"
//#include "B2UILobbyMain.h"
//#include "B2GuildGameMode.h"
#include "B2Airport.h"
//#include "B2UIGuildBattleHistory.h"
//#include "B2UIGuildMapMain.h"
#include "InternalEvents.h"
//#include "BladeIIPointCharger.h"
//#include "B2UIFindMatchPopup.h"
//#include "B2UIOtherUserInfo.h"
//#include "B2UISummonItem.h"
//#include "B2UISummonItemDisplay.h"
//#include "B2UILobbySelectCharacter.h"
//#include "B2UIMailRewardPopUp.h"
//#include "B2CompositeMeshCache.h"
//#include "B2GameLoadingProgress.h"
//#include "FakeConfigure.h"
#include "BladeIIGameImpl.h"
//#include "B2LobbyNPCSkeletalMeshActor.h"
//#include "B2UIRestPointMain.h"
//#include "B2UIChatting.h"
//#include "B2UIPackageStore.h"
//#include "B2UIMagicStore.h"
//#include "TutorialManager.h"
//#include "B2RaidInfo.h"
//#include "B2UIHallOfFame.h"
//#include "B2UIHeader.h"
//#include "B2UILinkManager.h"
//#include "B2CostumeDataStore.h"
#include "BladeIIGameMode.h"
#include "B2GameDefine.h"
//#include "B2UIMsgPopupSelectFairyGift.h"
//#include "B2UIDocDonation.h"
//#include "B2UIItemForge.h"
//#include "B2UISealBoxResult.h"
#include "BladeIIUtil.h"
//#include "FairyManager.h"
#include "DF.h"
#include "StereoRendering.h"
#include "../Fulfil/FairyManager.h"

#define SWIPE_NEXTSTAGE_POS 1.7
#define SWIPE_PREVSTAGE_POS 0.3

#define CHAR_ANIM_ROTATION 0
#define CHAR_ACTOR_ROTATION !CHAR_ANIM_ROTATION
#define CHAR_ROTATION_USE_DEACCEL 1

static void CharacterStopSound(ASkeletalMeshActor* Actor)
{
	//if (!Actor)
	//	return;

	//auto* SKMeshComp = Actor->GetSkeletalMeshComponent();
	//if (SKMeshComp)
	//{
	//	for (auto* AttachedComp : SKMeshComp->GetAttachChildren())
	//	{
	//		auto* AudioComp = Cast<UAudioComponent>(AttachedComp);
	//		if (AudioComp)
	//		{
	//			AudioComp->Stop();
	//		}
	//	}
	//}
}

static void CharacterHiddenAndStopSound(ASkeletalMeshActor* Actor, bool bHidden)
{
	//if (!Actor)
	//	return;

	//Actor->SetActorEnableCollision(!bHidden);
	//Actor->SetActorHiddenInGame(bHidden);

	//if (bHidden)
	//{
	//	CharacterStopSound(Actor);
	//}
}

//void SetMatineeHiddenInGame(AMatineeActor* Matinee, bool bHidden)
//{
//	Matinee->SetActorHiddenInGame(bHidden);
//
//	for (auto& GroupActorInfo : Matinee->GroupActorInfos)
//	{
//		for (auto* Actor : GroupActorInfo.Actors)
//		{
//			if (Actor)
//				Actor->SetActorHiddenInGame(bHidden);
//		}			
//	}
//}

FLobbyLevelVisibleManager::FLobbyLevelVisibleManager(FLobbySceneManager* InSceneManager) : LobbySceneManager(InSceneManager), CurrentLobbyScene(ELobbyScene::ELobbyScene_Title)
{

}

void FLobbyLevelVisibleManager::AddLevelLobbyScene(ELobbyLevelType LobbyType, FName LevelName)
{
	LevelsRequiredScene.AddUnique(LobbyType, LevelName);
}

ELobbyLevelType FLobbyLevelVisibleManager::GetLobbyLevelType(ELobbyScene LobbyScene, int32 OptionalChapterNum)
{
	switch (LobbyScene)
	{
	case ELobbyScene::ELobbyScene_Title:
	case ELobbyScene::ELobbyScene_SelectLogin:
	case ELobbyScene::ELobbyScene_CharacterIntro:
	//case ELobbyScene::ELobbyScene_StageInfo:
	case ELobbyScene::ELobbyScene_ContentsSubMain:
	case ELobbyScene::ELobbyScene_EtherMain:
	case ELobbyScene::ELobbyScene_CharacterPage:
	case ELobbyScene::ELobbyScene_ControlCharacterPage:
	case ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage:
	case ELobbyScene::ELobbyScene_PVPCharacterSelectPage:
	case ELobbyScene::ELobbyScene_DimensionSelectPage:
	/* 그냥 위에 씌여지는 UI들 설정들은 추가하지 않아도 됨.	
	case ELobbyScene::ELobbyScene_Mail:
	*/
		return ELobbyLevelType::ELobbyLevelType_A;

	case ELobbyScene::ELobbyScene_LobbyMain:
	case ELobbyScene::ELobbyScene_CharacterObserve:
		return ELobbyLevelType::ELobbyLevelType_B;

	case ELobbyScene::ELobbyScene_Chapter: // OptionalChapterNum 에 따라 ELobbyLevelType 을 구분해서 리턴
		if (OptionalChapterNum > 0) {
			return GetChapterSceneLevelTypeOfChapter(OptionalChapterNum);
		}
		break;
	case ELobbyScene::ElobbyScene_ModSelect:
		return ELobbyLevelType::ELobbyLevelType_C_MOD;
	case ELobbyScene::ELobbyScene_HeroTowerReady:
		return ELobbyLevelType::ELobbyLevelType_D;
	case ELobbyScene::ELobbyScene_GuildMapUI:
		return ELobbyLevelType::ELobbyLevelType_E;
	case ELobbyScene::ELobbyScene_HeroManagement :
	case ELobbyScene::ELobbyScene_CollectBookMain :
	case ELobbyScene::ELobbyScene_FindAccount:
		return ELobbyLevelType::ELobbyLevelType_G;
	case ELobbyScene::ELobbyScene_Guild:
	case ELobbyScene::ELobbyScene_GuildCreateAndJoin:
		return ELobbyLevelType::ELobbyLevelType_H;
	case ELobbyScene::ELobbyScene_SummonItemDisplay:
	case ELobbyScene::ELobbyScene_GeneralLotteryDisplay:
		return ELobbyLevelType::ELobbyLevelType_SummonItemDisplayScene;
	case ELobbyScene::ELobbyScene_SummonItem:
		return ELobbyLevelType::ELobbyLevelType_SummonItem;
	case ELobbyScene::ELobbyScene_RestPoint:
		return ELobbyLevelType::ELobbyLevelType_RestPoint;
	case ELobbyScene::ELobbyScene_HallOfFame:
		return ELobbyLevelType::ELobbyLevelType_HallOfFame;
	case ELobbyScene::ELobbyScene_FairyMain:
		return ELobbyLevelType::ELobbyLevelType_Fairy;
	case ELobbyScene::ELobbyScene_ItemForge:
	case ELobbyScene::ELobbyScene_SealBoxResult:
		return ELobbyLevelType::ELobbyLevelType_Empty;
	}

	return ELobbyLevelType::ELobbyLevelType_None;
}

void FLobbyLevelVisibleManager::ChangeVisibility(ELobbyScene ToChangeScene, int32 OptionalPrevChapterNum, int32 OptionalChapterNum)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("FLobbyLevelVisibleManager::ChangeVisibility, ToChangeScene %d, PrevCN %d, CN %d"), (int32)ToChangeScene, OptionalPrevChapterNum, OptionalChapterNum));
	BII_CHECK(LobbySceneManager);
	BII_CHECK(LevelsRequiredScene.Num() > 0);
	//
	//auto* GameMode = LobbySceneManager ? LobbySceneManager->GetLobbyGameMode() : NULL;
	//if (GameMode)
	//{
	//	auto* World = GameMode->GetWorld();
	//	if (!World)
	//		return;

	//	bool OldMatchStarted = World->bMatchStarted;
	//	World->bMatchStarted = false;
	//	bool OldForceVisibleStreamingLevels = World->bShouldForceVisibleStreamingLevels;
	//	World->bShouldForceVisibleStreamingLevels = false;

	//	TArray<FName> ToOff;
	//			
	//	LevelsRequiredScene.MultiFind(GetLobbyLevelType(CurrentLobbyScene, OptionalPrevChapterNum), ToOff);
	//	for (auto& ToVisibleOffLevelName : ToOff)
	//	{
	//		ULevelStreaming* StreamingLevel = World->GetLevelStreamingForPackageName(ToVisibleOffLevelName);

	//		if (!StreamingLevel)
	//			continue;

	//		StreamingLevel->bShouldBeVisible = StreamingLevel->bShouldBeVisibleInEditor = false;
	//	}

	//	//SimpleDirectionalLight regist 문제로 RemoveFromWorld를 먼저 불러야됨.
	//	World->FlushLevelStreaming();

	//	TArray<FName> ToOn;
	//	LevelsRequiredScene.MultiFind(GetLobbyLevelType(ToChangeScene, OptionalChapterNum), ToOn);
	//			
	//	bool bShouldShowLoadingScreen = false;

	//	for (auto& ToVisibleOnLevelName : ToOn)
	//	{
	//		ULevelStreaming* StreamingLevel = World->GetLevelStreamingForPackageName(ToVisibleOnLevelName);

	//		if (!StreamingLevel)
	//			continue;
	//		
	//		StreamingLevel->bShouldBeVisible = StreamingLevel->bShouldBeVisibleInEditor = StreamingLevel->bShouldBeLoaded = true;
	//		
	//		if (CurrentLobbyScene != ELobbyScene::ELobbyScene_Title && // 타이틀에서 갈 때는 오히려 쌩뚱맞은 로딩 화면이 또 나온다 싶어서 좀 그렇다..				
	//			(ToChangeScene == ELobbyScene::ELobbyScene_LobbyMain || ToChangeScene == ELobbyScene::ELobbyScene_CharacterObserve)
	//			&& !StreamingLevel->IsLevelLoaded())
	//		{ // 로비에 이미 들어온 후에 LobbyMain 서브레벨을 로딩해야 하는 상황은 특히 긴 시간을 요구하므로 로딩 화면을 띄운다.
	//			// 보통 전투에서 돌아온 후에 LobbyMain 으로 갈때 그렇게 된다.
	//			bShouldShowLoadingScreen = true;
	//		}

	//		if (!StreamingLevel->GetLoadedLevel())
	//		{				
	//			StreamingLevel->bShouldBlockOnLoad = true;
	//		}
	//	}
	//
	//	if (bShouldShowLoadingScreen)
	//	{
	//		GameMode->SetupLoadingScreenImageCommon(false, false, false); // 진행도는 없이 로딩화면만 띄움.
	//	}

	//	World->FlushLevelStreaming();

	//	World->bMatchStarted = OldMatchStarted;
	//	World->bShouldForceVisibleStreamingLevels = OldForceVisibleStreamingLevels;

	//	FlushRenderingCommands();

	//	if (bShouldShowLoadingScreen)
	//	{
	//		GameMode->ConditionalWaitForLoadingScreen();
	//	}
	//}
	//
	//CurrentLobbyScene = ToChangeScene;
}

bool FLobbyLevelVisibleManager::IsStreamingLevelLoaded(ELobbyScene CheckScene, int32 OptionalChapterNum /*OptionalChapterNum 은 챕터 씬인 경우 필요*/)
{
	//TArray<FName> LevelNameList;
	//LevelsRequiredScene.MultiFind(GetLobbyLevelType(CheckScene, OptionalChapterNum), LevelNameList);
	//AB2LobbyGameMode* LobbyGM = LobbySceneManager ? LobbySceneManager->GetLobbyGameMode() : NULL;
	//UWorld* TheWorld = LobbyGM ? LobbyGM->GetWorld() : nullptr;
	//if (TheWorld)
	//{
	//	for (auto& PrefetchLevelName : LevelNameList)
	//	{
	//		ULevelStreaming* StreamingLevel = TheWorld->GetLevelStreamingForPackageName(PrefetchLevelName);
	//		if (!StreamingLevel || !StreamingLevel->IsLevelLoaded()) {
	//			return false;
	//		}
	//	}
	//}
	return true;
}
/* 
 * 최적화 목적으로 백그라운드로 미리 로딩해 두는 기능. 사용은 적절한 시점에.. 잘못 사용하면 오히려 부작용이 발생할 수 있다.
 */
void FLobbyLevelVisibleManager::PrefetchStreamingLevel(ELobbyScene PrefetchScene)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("FLobbyLevelVisibleManager::PrefetchStreamingLevel %d"), (int32)PrefetchScene));

	//// 챕터 씬도 뭔가 해 보려면 번호 먹여주면 되는데 자칫하면 사용 안 하느니만 못한 이런 민감한 기능을 그렇게 널리 사용할 가능성을 주느니 그냥 제공 안하고 말지.
	//check(PrefetchScene != ELobbyScene::ELobbyScene_Chapter);

	//TArray<FName> LevelNameList;
	//LevelsRequiredScene.MultiFind(GetLobbyLevelType(PrefetchScene), LevelNameList);

	//auto* GameMode = LobbySceneManager ? LobbySceneManager->GetLobbyGameMode() : NULL;
	//UWorld* TheWorld = GameMode ? GameMode->GetWorld() : nullptr;
	//if (TheWorld)
	//{
	//	for (auto& PrefetchLevelName : LevelNameList)
	//	{// bShouldBlokcOnLoad 안 주고 로딩 콜. 그냥 이렇게 해 두고 내비두는 거.
	//		UGameplayStatics::LoadStreamLevel(TheWorld, PrefetchLevelName, false, false, FLatentActionInfo());
	//	}
	//}
}

bool FLobbyLevelVisibleManager::HasSameStreamingLevelToCurrentScene(ELobbyScene TestScene, int32 OptionalCurrChapterNum, int32 OptionalTestChapterNum)
{
	TArray<FName> CurrentStreamingLevels;
	LevelsRequiredScene.MultiFind(GetLobbyLevelType(CurrentLobbyScene, OptionalCurrChapterNum), CurrentStreamingLevels);

	TArray<FName> TestStreamingLevels;
	LevelsRequiredScene.MultiFind(GetLobbyLevelType(TestScene, OptionalTestChapterNum), TestStreamingLevels);

	return (CurrentStreamingLevels == TestStreamingLevels);	
}

TArray<std::function<void(void)>> FLobbySceneManager::DeferredChangeLobbyScenesFunc;
TArray<std::function<void(void)>> FLobbySceneManager::PendingEventSubSystems;

FLobbySceneManager::FLobbySceneManager():  GameMode(NULL), LevelManager(this),CurrentLobbyScene(ELobbyScene::ELobbyScene_None), PlayBeginAnim(false)
{
	ItemOpSceneCamera = NULL;
	CurrentChapterViewNum = 0;
	bUnconditionalShowWing = false;
	bCanUseRemoteInfoForSKMeshSetup = false;
	ShopGirlReservedState = ELobbyShopGirlState::ELSG_IdleLoop;
	GLobbyTypeActorLocation.Empty();

	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("CharacterCameraRotateFactor"), CharacterCameraRotateFactor, GGameIni);
	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("CharacterCameraFreeRotateMaxSpeed"), CharacterCameraFreeRotateMaxSpeed, GGameIni);
	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("CharacterCameraRotateDeaccelerationFactor"), CharacterCameraRotateDeaccelerationFactor, GGameIni);
	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("CharacterFreeRotateAdditionalScale"), CharacterFreeRotateAdditionalScale, GGameIni);

	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("WorldMapSwipeDeaccelerationFactor"), WorldMapSwipeDeaccelerationFactor, GGameIni);
	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("WorldMapSpeedTriggeringAcceleratingSwipe"), WorldMapSpeedTriggeringAcceleratingSwipe, GGameIni);
	GConfig->GetInt(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("WorldMapDirectSwipeSpeedHistoryNum"), WorldMapDirectSwipeSpeedHistoryNum, GGameIni);
	GConfig->GetInt(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("WorldMapDirectSwipeSpeedFilterNum"), WorldMapDirectSwipeSpeedFilterNum, GGameIni);
	
	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("LobbyCameraZoomSpeed"), LobbyCameraZoomSpeed, GGameIni);
	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("LobbyCameraRotationX"), LobbyCameraRotationX, GGameIni);
	GConfig->GetFloat(TEXT("/Script/BladeII.BladeIILobbySceneManager"), TEXT("LobbyCameraRotationY"), LobbyCameraRotationY, GGameIni);
}

FLobbySceneManager::~FLobbySceneManager()
{
	DeleteLobbyScenes();
}

void FLobbySceneManager::CreateLobbyScenes()
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::CreateLobbyScenes"));
	DeleteLobbyScenes();

	int32 idx = INDEX_NONE;

	idx = LobbyScenes.Add(new FLobbyTitleScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_Title));
	idx = LobbyScenes.Add(new FSelectLoginScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_SelectLogin));
	idx = LobbyScenes.Add(new FLobbyCharacterIntroScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_CharacterIntro));
	idx = LobbyScenes.Add(new FLobbyMainScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_LobbyMain));
	idx = LobbyScenes.Add(new FLobbyCharObserveScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_CharacterObserve));
	idx = LobbyScenes.Add(new FLobbyHeroMgmtScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_HeroManagement));
	idx = LobbyScenes.Add(new FLobbyStoreScene(this)); 
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_Store));
	idx = LobbyScenes.Add(new FLobbyMailScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_Mail));
	idx = LobbyScenes.Add(new FLobbyChapterScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_Chapter));
	idx = LobbyScenes.Add(new FLobbyStageInfoScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_StageInfo));
	idx = LobbyScenes.Add(new FLobbyModSelectScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ElobbyScene_ModSelect));
	idx = LobbyScenes.Add(new FLobbyTagMatchScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ElobbyScene_PVPMain));
	idx = LobbyScenes.Add(new FLobbyTeamMatchScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ElobbyScene_TeamMatchMain));
	idx = LobbyScenes.Add(new FLobbyCounterAttackScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ElobbyScene_CounterAttackSelect));
	idx = LobbyScenes.Add(new FLobbyFriendScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_Friend));
	idx = LobbyScenes.Add(new FContentsSubMainScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_ContentsSubMain));
	idx = LobbyScenes.Add(new FLobbyAdventureModSelectScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_AdventureModSelect));
	idx = LobbyScenes.Add(new FLobbyHeroTowerReadyScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_HeroTowerReady));
	idx = LobbyScenes.Add(new FLobbyGuildScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_Guild));
	idx = LobbyScenes.Add(new FLobbyGuildCreateAndJoinScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_GuildCreateAndJoin));
	idx = LobbyScenes.Add(new FLobbyRaidMainScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_RaidMain));
	idx = LobbyScenes.Add(new FLobbyCollectBookMain(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_CollectBookMain));
	idx = LobbyScenes.Add(new FLobbySummonItemScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_SummonItem));
	idx = LobbyScenes.Add(new FLobbySummonItemDisplayScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_SummonItemDisplay));
	idx = LobbyScenes.Add(new FLobbyGuildMapUIScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_GuildMapUI));
	idx = LobbyScenes.Add(new FLobbyGuildBattleReady(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_GuildBattleReady));
	idx = LobbyScenes.Add(new FLobbyControlMainScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_ControlMain));
	idx = LobbyScenes.Add(new FLobbyGeneralLotteryDisplayScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_GeneralLotteryDisplay));
	idx = LobbyScenes.Add(new FLobbyCharacterSelectPage(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_CharacterPage));
	idx = LobbyScenes.Add(new FLobbyControlCharacterSelectPage(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_ControlCharacterPage));
	idx = LobbyScenes.Add(new FLobbyFindAccountInfo(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_FindAccount));
	idx = LobbyScenes.Add(new FLobbyEnterDungeonMain(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_EnterDungeonMain));
	idx = LobbyScenes.Add(new FLobbyEnterShopMain(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_EnterShopMain));
	idx = LobbyScenes.Add(new FLobbyHeroTowerCharacterSelectPage(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage));
	idx = LobbyScenes.Add(new FLobbyEnterBattleMain(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_EnterBattleMain));
	idx = LobbyScenes.Add(new FLobbyPVPCharacterSelectPage(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_PVPCharacterSelectPage));
	idx = LobbyScenes.Add(new FLobbyEtherMain(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_EtherMain));
	idx = LobbyScenes.Add(new FLobbyRestPointScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_RestPoint));
	idx = LobbyScenes.Add(new FLobbyEventMain(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_EventMain));
	idx = LobbyScenes.Add(new FLobbySeasonEvent(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_SeasonEvent));
	idx = LobbyScenes.Add(new FLobbyPackageStore(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_PackageStore));
	idx = LobbyScenes.Add(new FLobbyHallOfFame(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_HallOfFame));
	idx = LobbyScenes.Add(new FLobbyMagicStore(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_MagicStore));
	idx = LobbyScenes.Add(new FLobbyGuildMercenary(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_Mercenary));
	idx = LobbyScenes.Add(new FLobbyAdventureEnterBattleMain(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_AdventureEnterBattleMain));
	idx = LobbyScenes.Add(new FLobbyFairyScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_FairyMain));
	idx = LobbyScenes.Add(new FLobbyMission(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_Mission));
	idx = LobbyScenes.Add(new FLobbyDonationScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_DonationEvent));
	idx = LobbyScenes.Add(new FLobbyDimensionMain(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_DimensionMain));
	idx = LobbyScenes.Add(new FLobbyDimensionSelectPage(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_DimensionSelectPage));
	idx = LobbyScenes.Add(new FLobbyItemForgeScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_ItemForge));
	idx = LobbyScenes.Add(new FLobbySealBoxResultScene(this));
	check(idx == static_cast<uint8>(ELobbyScene::ELobbyScene_SealBoxResult));

	for (FLobbySceneBase* JustCreatedLobbyScene : LobbyScenes)
	{
		check(JustCreatedLobbyScene);
		// LobbyScene 이 제거될 때까지 남아 있을 event subscribe.
		JustCreatedLobbyScene->SubscribeEvents_OnConstruct();
	}

	CurrentLobbyScene = ELobbyScene::ELobbyScene_None;
	CurrentChapterViewNum = 0;
}

void FLobbySceneManager::DeleteLobbyScenes()
{
	for (FLobbySceneBase* LobbySceneTobeDeleted : LobbyScenes)
	{
		check(LobbySceneTobeDeleted);
		LobbySceneTobeDeleted->UnsubscribeEvents_OnDestruct();
		delete LobbySceneTobeDeleted;
	}

	LobbyScenes.Empty();
}

void FLobbySceneManager::SetGameMode(class AB2LobbyGameMode* InGameMode)
{
	BII_CHECK(InGameMode);

	//GameMode = InGameMode;

	//auto* LocalController = GameMode ? UGameplayStatics::GetLocalPlayerController(GameMode) : NULL;
	//if (LocalController)
	//{
	//	LocalController->SetIgnoreLookInput(true);
	//	LocalController->SetIgnoreMoveInput(true);
	//}

	//CreateLobbyScenes();
}

AB2LobbyGameMode* FLobbySceneManager::GetLobbyGameMode()
{
	return GameMode;
}

ELobbyScene FLobbySceneManager::GetLobbyScene(EUIScene UIScene)
{
	for (auto* LobbyScene : LobbyScenes)
	{
		if (LobbyScene->GetUIScene() == UIScene)
			return LobbyScene->GetLobbyScene();
	}

	return ELobbyScene::ELobbyScene_None;
}

FLobbySceneBase* FLobbySceneManager::GetLobbyScene(ELobbyScene InLobbyScene)
{
	uint8 Index = static_cast<uint8>(InLobbyScene);
	return LobbyScenes.IsValidIndex(Index) ? LobbyScenes[Index] : nullptr;
}

FLobbyHallOfFame* FLobbySceneManager::GetHallOfFameScene()
{
	return static_cast<FLobbyHallOfFame*>(GetLobbyScene(ELobbyScene::ELobbyScene_HallOfFame));
}

void FLobbySceneManager::ChangeLobbyScene(ELobbyScene ToChangeScene, int32 OptionalToChangeChapterNum /*= -1*/, bool bForceChange /*= false*/)
{ 
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::ChangeLobbyScene"));
	//if (ToChangeScene != ELobbyScene::ELobbyScene_HeroManagement)
	//	BladeIIGameImpl::GetCostumeDataStore().ClearPreview();
	//// OptionalToChangeChapterNum 은 ELobbyScene_Chapter 의 경우만
	//if (CurrentLobbyScene == ToChangeScene && !bForceChange && (ToChangeScene != ELobbyScene::ELobbyScene_Chapter || CurrentChapterViewNum == OptionalToChangeChapterNum))
	//	return;

	////Close
	//uint8 CurLobbySceneIndex = static_cast<uint8>(CurrentLobbyScene);
	//if (LobbyScenes.IsValidIndex(CurLobbySceneIndex))
	//	LobbyScenes[CurLobbySceneIndex]->CloseScene();

	////문제가 있어서 UIScene은 미리 변경.. 영웅관리씬 (FLobbyHeroMgmtScene) DJLegacy UI 는 이걸로 먹히지는 않을 거고.. 그 경우는 UI 변경 커맨드는 따로 가도록 해야 (LobbyEnterHeroMgmtModeClass 를 통해)
	//uint8 ToChangeSceneInt = static_cast<uint8>(ToChangeScene);
	//if (LobbyScenes.IsValidIndex(ToChangeSceneInt))
	//{
	//	EUIScene ToChangeUIScene = LobbyScenes[ToChangeSceneInt]->GetUIScene();
	//	if (UB2UIManager::GetInstance()->GetCurrUIScene() != ToChangeUIScene)
	//	{
	//		UB2UIManager::GetInstance()->ChangeUIScene(ToChangeUIScene);

	//		B2GMLoadingProgCollector::Step(); // 맵 로딩 타임에 로딩 진행도 넣으려는 건데 맵 로딩 시점 아니면 별 일 없을 것
	//	}
	//}

	//const ELobbyLevelType CurrLevelType = LevelManager.GetLobbyLevelType(CurrentLobbyScene, CurrentChapterViewNum);
	//const ELobbyLevelType ToChangeLevelType = LevelManager.GetLobbyLevelType(ToChangeScene, OptionalToChangeChapterNum);

	//bool bToLoadNewLevels = (CurrLevelType != ToChangeLevelType && ToChangeLevelType != ELobbyLevelType::ELobbyLevelType_None)
	//	// 여기서 LevelType 만으로는 실제 똑같은 스트리밍 레벨을 Unload/Load 할지 완전히 알 수가 없어서 추가 검사. 같은 스트리밍 레벨을 내렸다 다시 올리는 게 아까워서 그러는 게 아니라 좀 문제가 있어 보임.
	//	&& !LevelManager.HasSameStreamingLevelToCurrentScene(ToChangeScene, CurrentChapterViewNum, OptionalToChangeChapterNum);

	//if (bToLoadNewLevels)
	//{
	//	ResetObjectRefsOnSceneLevelStreaming();

	//	LevelManager.ChangeVisibility(ToChangeScene, CurrentChapterViewNum, OptionalToChangeChapterNum);

	//	B2GMLoadingProgCollector::Step(); // 맵 로딩 타임에 로딩 진행도 넣으려는 건데 맵 로딩 시점 아니면 별 일 없을 것
	//}

	//// 새로운 레벨을 로딩하는 경우가 아니라도 캐릭터 조합을 다시 해야 하는 경우.. 필요에 따라 추가. 도감은 캐릭터 장비 조합을 임의로 하므로 나갈 때 무조건 새로 조합할 필요가 있다.
	//const bool bUnconditionalUpdatePart = (CurrentLobbyScene == ELobbyScene::ELobbyScene_CollectBookMain);

	////woong 캐릭터상세보기 캐릭터가 항상 4개가 아니므로 여기서 캐릭터 업데이트를 못하게해야합니다
	//const bool bFindAccountUpdate = (ToChangeScene == ELobbyScene::ELobbyScene_FindAccount) ? false : true;

	//OnChangeLobbySceneSetSound(CurrentLobbyScene, ToChangeScene);
	//CurrentLobbyScene = ToChangeScene;
	//if (CurrentLobbyScene == ELobbyScene::ELobbyScene_Chapter && OptionalToChangeChapterNum > 0)
	//{
	//	CurrentChapterViewNum = OptionalToChangeChapterNum;
	//}

	//if (bToLoadNewLevels || bUnconditionalUpdatePart)
	//{
	//	if (bFindAccountUpdate)
	//	{
	//		for (int32 Class = 0; Class < GetMaxPCClassNum(); ++Class)
	//		{
	//			UpdateLobbyCharCompForParts(IntToPCClass(Class));
	//		}
	//	}
	//}

	////[@AKI, 170718] <B2CLT-1789> 로비 연출 개선으로 주석 처리 함.
	////추후 전체 캐릭들이 나와있을때 캐릭명 등을 보여 주고 싶으면 주석을 해제 하면 됨.
	////-> WidgetClass 로딩 및 CreateWidget 은 낭비가 되므로 비활성화 해 놓은 상태.. 복구하려면 RESTORE_BII_LOBBY_ONHEAD_DISPLAY 디파인을 켜야.
	////UpdateLobbyOnHeadDisplayWidget();

	////Open
	//CurLobbySceneIndex = static_cast<uint8>(CurrentLobbyScene);
	//if (LobbyScenes.IsValidIndex(CurLobbySceneIndex))
	//	LobbyScenes[CurLobbySceneIndex]->OpenScene();
}

bool FLobbySceneManager::IsSceneStreamingLevelLoaded(ELobbyScene CheckScene, int32 OptionalChapterNum)
{ // 널리 사용할 거 아니라도 이건 함수 의미 상 기능을 완전하게 만들 필요는 있어서 Chapter 씬의 경우도 처리..ㅋ
	return LevelManager.IsStreamingLevelLoaded(CheckScene, OptionalChapterNum);
}
void FLobbySceneManager::PrefetchSceneStreamingLevel(ELobbyScene PrefetchScene)
{
	// 로비 서브레벨 중 덩치 큰 걸 적당한 시점에 미리 로딩할 때 사용.
	// 다른 에셋 로딩과 얽혀서 flush 가 쉽게 유발되는 상황이면 부작용이 더 클 수 있으니 적절한 시점에 잘 알고 사용.
	if (PrefetchScene != ELobbyScene::ELobbyScene_Chapter) // 번호 넘겨주기 싫어서 챕터 씬 제외. 그다지 널리 사용될 기능도 아니고.. 널리 사용해서 좋을 것도 없고.
	{
		LevelManager.PrefetchStreamingLevel(PrefetchScene);
	}
}

void FLobbySceneManager::DeferredRegistChangeLobbyScene(std::function<void(void)> Func)
{
	DeferredChangeLobbyScenesFunc.Add(Func);
}

bool FLobbySceneManager::IsExistDeferredRegistChangeLobbyScene()
{
	return DeferredChangeLobbyScenesFunc.Num() > 0;
}

bool FLobbySceneManager::HasDeferredChangeLobbyScenes() 
{
	return (DeferredChangeLobbyScenesFunc.Num() > 0);
}

void FLobbySceneManager::CallDeferredChangeLobbyScenes()
{
	for (auto& Func : DeferredChangeLobbyScenesFunc)
	{
		Func();
	}

	ClearDeferredChangeLobbyScenes();
}

void FLobbySceneManager::ClearDeferredChangeLobbyScenes()
{
	DeferredChangeLobbyScenesFunc.Empty();
}

bool IsLobbySceneForLobbyMainSceneAMB(ELobbyScene InLobbyScene)
{
	return (InLobbyScene == ELobbyScene::ELobbyScene_LobbyMain || InLobbyScene == ELobbyScene::ELobbyScene_CharacterObserve);
}

void FLobbySceneManager::OnChangeLobbySceneSetSound(ELobbyScene InPrevScene, ELobbyScene InChangedScene)
{
	//if (GameMode)
	//{
	//	const bool bPrevForLobbyMainSceneAMB = IsLobbySceneForLobbyMainSceneAMB(InPrevScene);
	//	const bool bNewForLobbyMainSceneAMB = IsLobbySceneForLobbyMainSceneAMB(InChangedScene);

	//	if (bNewForLobbyMainSceneAMB)
	//	{
	//		if (!bPrevForLobbyMainSceneAMB) // 이전에 플레이하지 않은 경우만. 아무래도 이거 끊기면 안좋아할 거 같음.
	//		{
	//			GameMode->PlayAMBByCommonSoundID(ECommonSoundID::ECSID_AMB_LobbyMainScene);
	//		}
	//	}
	//	else
	//	{
	//		GameMode->StopAMB(); // 아직은 다른 장면에 AMB 가 없음.
	//	}
	//}
}

void FLobbySceneManager::UpdateLobbyOnHeadDisplayWidget(EPCClass OptionalCreateOnlyClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::UpdateLobbyOnHeadDisplayWidget"));

	//// 일단 기존에 만든 거 제거
	//DestroyAllLobbyCharOnHeadDisplayClass<>::GetInstance().Signal();

	//// 클래스별로 돌면서 재생성. 붙이고자 하는 건 TypeB 애들이다.
	//if (LevelManager.GetLobbyLevelType(CurrentLobbyScene) == ELobbyLevelType::ELobbyLevelType_B)
	//{
	//	for (TMap<EPCClass, FLobbyCharacter>::TIterator It(LobbyCharactersForBLobbyType); It; ++It)
	//	{
	//		EPCClass ThisClass = It.Key();
	//		FLobbyCharacter& ThisCharInfo = It.Value();
	//		if (ThisClass != EPCClass::EPC_End && ThisCharInfo.LevelActor && ThisCharInfo.LevelActor->IsValidObj() && 
	//			// Observe 장면인 경우 OptionalCreateOnlyClass 를 지정해서 현재 캐릭터 아닌 것들을 명시적으로 걸러내고, 숨겨놓은 것도.
	//			!ThisCharInfo.LevelActor->bHidden && (OptionalCreateOnlyClass == EPCClass::EPC_End || OptionalCreateOnlyClass == ThisClass)) 
	//		{
	//			CreateLobbyCharOnHeadDisplayClass<EPCClass, ASkeletalMeshActor*, ELobbyOnHeadDisplayType>::GetInstance().Signal(ThisClass, ThisCharInfo.LevelActor,
	//				(CurrentLobbyScene == ELobbyScene::ELobbyScene_LobbyMain ? ELobbyOnHeadDisplayType::LOHDT_LobbyMain : ELobbyOnHeadDisplayType::LOHDT_CharObserve)
	//				);
	//		}
	//	}
	//}
}

void FLobbySceneManager::ConditionalUpdateLobbyOnHeadDisplayWidget()
{ // 아싸리 외부에서 강제 업데이트 정도의 용도.. 있어야 할 경우에만 업데이트.
	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_LobbyMain)
	{
		UpdateLobbyOnHeadDisplayWidget();
	}
}

void FLobbySceneManager::AddHallOfFrameCharacter(const TArray<FHallOfFameCharacterInfo>& HallOfFameCharacter)
{
	LobbyCharactersForHallOfFame.Append(HallOfFameCharacter);
}

void FLobbySceneManager::AddHallOfCharacterAnimation(EPCClass PCClass, class UAnimationAsset* PCAnimation)
{
	if (HallOfFameCharacterAnimation.Contains(PCClass))
	{
		HallOfFameBestCharacterAnimation.Add(PCClass, PCAnimation);
	}
	else
	{
		HallOfFameCharacterAnimation.Add(PCClass, PCAnimation);
	}
}

bool FLobbySceneManager::SelectLobbyMainCharacter(EPCClass SelectedClass)
{
	//switch (CurrentLobbyScene)
	//{
	//case ELobbyScene::ELobbyScene_LobbyMain:
	//{
	//	LobbyCharObvSelectClass<EPCClass>::GetInstance().Signal(SelectedClass);
	//	ChangeLobbyScene(ELobbyScene::ELobbyScene_CharacterObserve);
	//	return true;
	//}
	//break;
	//case ELobbyScene::ELobbyScene_CharacterObserve:
	//	if (SelectedClass == EPCClass::EPC_End)
	//	{
	//		ChangeLobbyScene(ELobbyScene::ELobbyScene_LobbyMain);
	//		return true;
	//	}
	//	break;
	//}
		
	return false;
}

#include "Animation/SkeletalMeshActor.h"

void FLobbySceneManager::SetGuildMapChangeMatinee(class AMatineeActor* Atype, class AMatineeActor* Btype)
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		FLobbyGuildMapUIScene* GuilScene = static_cast<FLobbyGuildMapUIScene*>(LobbyScenes[static_cast<uint8>(ELobbyScene::ELobbyScene_GuildMapUI)]);
		check(GuilScene)

		GuilScene->SetGuildMapChangeMatinee(Atype, Btype);
	}

}

void FLobbySceneManager::GuildMapChangeMatineeEnd()
{
	if(LobbyScenes.IsValidIndex(static_cast<uint8>(ELobbyScene::ELobbyScene_GuildMapUI)))
	{
		FLobbyGuildMapUIScene* GuilScene = static_cast<FLobbyGuildMapUIScene*>(LobbyScenes[static_cast<uint8>(ELobbyScene::ELobbyScene_GuildMapUI)]);
		check(GuilScene)

		GuilScene->GuildChangeMatineeEnd();
	}
}

void FLobbySceneManager::Tick(float DeltaSeconds)
{
	uint8 CurrentLobbySceneIdx = static_cast<uint8>(CurrentLobbyScene);
	if (LobbyScenes.IsValidIndex(CurrentLobbySceneIdx))
		LobbyScenes[CurrentLobbySceneIdx]->Tick(DeltaSeconds);
}

TStatId FLobbySceneManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FLobbySceneManager, STATGROUP_Tickables);
}

void FLobbySceneManager::ForceUpdate()
{
	uint8 CurrentLobbySceneIdx = static_cast<uint8>(CurrentLobbyScene);
	if (LobbyScenes.IsValidIndex(CurrentLobbySceneIdx))
		LobbyScenes[CurrentLobbySceneIdx]->ForceUpdate();
}

void FLobbySceneManager::AddLevelLobbyScene(ELobbyLevelType LobbyType, FName LevelName)
{
	LevelManager.AddLevelLobbyScene(LobbyType, LevelName);
}

void FLobbySceneManager::AddLobbyCharacter(EPCClass PCClass, const FLobbyCharacter& LobbyCharacter)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::AddLobbyCharacter"));
	//LobbyCharactersForBLobbyType.Add(PCClass, LobbyCharacter);

	//SetupLobbyCharCompForParts(PCClass, LobbyCharacter.LevelActor);
}

void FLobbySceneManager::HiddenCharacter(const FLobbyCharacter& ToHiddenLobbyCharacter, bool bHidden)
{
	//CharacterHiddenAndStopSound(ToHiddenLobbyCharacter.LevelActor, bHidden);
}

void FLobbySceneManager::AddLobbyATypeActorInfo(EPCClass PCClass, const FLobbyCharacterInfo& LobbyCharacter)
{	
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::AddLobbyATypeActorInfo"));
	/*auto& InsertedElem = LobbyCharactersForALobbyType.Add(PCClass, LobbyCharacter);

	InsertedElem.CachedInitLocation = InsertedElem.ActorForMain ? InsertedElem.ActorForMain->GetActorLocation() : FVector::ZeroVector;
	InsertedElem.InitialRotation = InsertedElem.ActorForMain->GetActorRotation();

	SetupLobbyCharCompForParts(PCClass, LobbyCharacter.ActorForMain);
	SetupLobbyCharCompForParts(PCClass, LobbyCharacter.ActorForSub);*/
}

void FLobbySceneManager::AddLobbyGTypeActorInfo(EPCClass PCClass, const FLobbyCharacterInfo& LobbyCharacter)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::AddLobbyATypeActorInfo"));
	//auto& InsertedElem = LobbyCharactersForGLobbyType.Add(PCClass, LobbyCharacter);

	//if (!GLobbyTypeActorLocation.Contains(PCClass))
	//	GLobbyTypeActorLocation.Add(PCClass, InsertedElem.ActorForMain ? InsertedElem.ActorForMain->GetActorLocation() : FVector::ZeroVector);

	////InsertedElem.CachedInitLocation = InsertedElem.ActorForMain ? InsertedElem.ActorForMain->GetActorLocation() : FVector::ZeroVector;
	//InsertedElem.CachedInitLocation = GLobbyTypeActorLocation.Contains(PCClass) ? *GLobbyTypeActorLocation.Find(PCClass) : FVector::ZeroVector;
	//InsertedElem.InitialRotation = InsertedElem.ActorForMain->GetActorRotation();

	//SetupLobbyCharCompForParts(PCClass, LobbyCharacter.ActorForMain);
}

void FLobbySceneManager::AddLobbyNPCCharacter(ELobbyNPCType InType, const FLobbyNPCCharacterInfo& InAddInfo)
{
	FLobbyNPCCharacterInfo& FoundOrAddElem = LobbyNPCs.FindOrAdd(InType); // 중복 호출될지 몰라서
	FoundOrAddElem = InAddInfo;
}

void FLobbySceneManager::AddLobbyFairyFlyingActor(EFairyType InType, class AB2LobbyFlyingFairyDoumi* fairyactor)
{
	if (!LobbyFlyFairys.Contains(InType))
		LobbyFlyFairys.Add(InType, fairyactor);
}

void FLobbySceneManager::AddLobbyMainScenePointLights(const FLobbyMainScenePointLightInfo& InAddInfo)
{
	LobbyMainScenePointLights = InAddInfo;
}

/** 
 * bForceUpdate 는 필수적인 상황이 아니면 true 로 주지 말 것. bForceUpdate 를 주는 게 동작에 이상을 일으키지는 않지만 성능에 쥐약이 될 수 있음.
 * 필수적인 상황은 도감같이 일시적으로 다른 장비 장착을 보여준다거나, 치트를 날리는 상황 등이 되겠다.
 * 기본적으로 각 캐릭터 클래스 별로 merge 한 메쉬는 장착 상태, 날개 여부에 따라 refresh 를 해야하는지 판별을 함.
 */
void FLobbySceneManager::SetupLobbyCharCompForParts(EPCClass InPCClass, class ASkeletalMeshActor* InSKActorToSet, bool bForceUpdate /*= false*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::SetupLobbyCharCompForParts"));
//
//	// ABladeIIPlayer::SetupComponentsForParts 에 해당. 여기선 로비맵에 박아놓은 SkeletalMeshActor 대상으로
//
//	if (InPCClass == EPCClass::EPC_End || !InSKActorToSet || !InSKActorToSet->IsValidObj() || !InSKActorToSet->GetWorld()){
//		return;
//	}
//
//	TArray<FB2Item> EquippedItems;
//	
//	AB2LobbyGameMode* B2LobbyGM = GetLobbyGameMode();
//	BII_CHECK(B2LobbyGM);
//	// 맨 처음 나오는 캐릭터 소개 화면에서는 6성 장비 장착된 것을 보여준다.
//	const bool bUseFixedGradeEquip = (CurrentLobbyScene == ELobbyScene::ELobbyScene_CharacterIntro);
//		//|| (CurrentLobbyScene == ELobbyScene::ELobbyScene_Title) // 타이틀 화면에서는 이제 동영상이 나온다..
//	bForceUpdate |= bUseFixedGradeEquip; // 소개 화면 6성 장비 장착은 ForceUpdate.
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	// 특정 별 성 수의 장비를 일괄 착용할 수 있도록 하는 치트인데 SetupSKCompForParts 안에서 장비를 변경하므로 ForceUpdate 신호를 보내주어야 한다.
//	extern int32 gForcedPlayerDummyPartGrade;
//	if (gForcedPlayerDummyPartGrade >= 1 && gForcedPlayerDummyPartGrade <= FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE)
//	{
//		bForceUpdate = true;
//	}
//#endif
//
//	FB2Wing WingData;
//	bool bHasWing = false;
//	
//	// 컴포짓 메쉬 결과물을 캐싱해 놓을지 여부. 
//	// 로비에서는 현재 조합을 하는 게 로컬 캐릭터 용인지 아닌지 구분을 할 수 있는 다른 적절한 정보가 없으므로 여기서 잘 넘겨주어야 함.
//	bool bCanCacheCompositedMesh = true;
//
//	if (bUseFixedGradeEquip)
//	{
//		bCanCacheCompositedMesh = false; // 임시적인 것으로 결과물 캐싱 안함.
//		// 모두 6성으로 하는 대신 캐릭터별로 요청한 grade 가 있어서 그걸로..
//		int32 StandardDummyPCPartItemGrade = FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE;
//		
//		if (GConfig)
//		{
//			FString IntroEquipGradeSection(TEXT("/Script/BladeII.B2Item"));
//			switch (InPCClass)
//			{
//			case EPCClass::EPC_Gladiator: GConfig->GetInt(*IntroEquipGradeSection, TEXT("IntroEquipGrade_Gladiator"), StandardDummyPCPartItemGrade, GGameIni); break;
//			case EPCClass::EPC_Assassin: GConfig->GetInt(*IntroEquipGradeSection, TEXT("IntroEquipGrade_Assassin"), StandardDummyPCPartItemGrade, GGameIni); break;
//			case EPCClass::EPC_Wizard: GConfig->GetInt(*IntroEquipGradeSection, TEXT("IntroEquipGrade_Wizard"), StandardDummyPCPartItemGrade, GGameIni); break;
//			case EPCClass::EPC_Fighter: GConfig->GetInt(*IntroEquipGradeSection, TEXT("IntroEquipGrade_Fighter"), StandardDummyPCPartItemGrade, GGameIni); break;
//			}
//			DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
//		}
//
//		// 특별히 준비된 함수임. 모종의 규약에 따라 RefID 조합하고 필요한 값들만 채워서 넣어줌.
//		GetStandardDummyPCPartItemData(InPCClass, StandardDummyPCPartItemGrade, EquippedItems);
//	}
//	else if (CurrentLobbyScene == ELobbyScene::ELobbyScene_CollectBookMain)
//	{
//		bCanCacheCompositedMesh = false; // 임시적인 것으로 결과물 캐싱 안함.
//		UB2UICollectBookMain* CollectBookUI = UB2UIManager::GetInstance()->GetUI<UB2UICollectBookMain>(UIFName::CollectBookMain);
//		if (CollectBookUI)
//		{
//			bHasWing = false; // 도감에서는 WingTab 들어가기 전까지는 안 나옴
//
//			EquippedItems = CollectBookUI->GetCollectBookCurrentEquipItems();
//			const int32 WingIndex = static_cast<int32>(EItemClass::EIC_Wing);
//			if (EquippedItems.IsValidIndex(WingIndex) && EquippedItems[WingIndex].ItemClass == EItemClass::EIC_Wing)
//			{
//				// WingTab에 들어가면 자동으로 EquippedItems에 등록되고 장착!
//				bHasWing = true;
//				// 날개 외관은 클래스와 진화 등급에 따라서만 결정.
//				WingData.OwnerPCClass = PCClassToInt(InPCClass);
//				WingData.EvolutionGrade = EquippedItems[WingIndex].StarGrade;	// 진화는 StarGrade에 박자
//				
//				WingData.bShouldBeVisible = true; // 도감이면 FLobbyCollectBookMain 에서 SetUnconditionalShowWing 을 하는 대신 여기서 이걸 직접 넣자. 마찬가지긴 할 건데 뜻이 좀 다른 듯..
//			}
//		}
//	}
//	else if (CurrentLobbyScene == ELobbyScene::ELobbyScene_FindAccount
//		&& bCanUseRemoteInfoForSKMeshSetup // CurrentLobbyScene 만 체크하면 호출 스택 상의 타이밍으로 인해 실제로는 로컬 캐릭터를 보여주는 뷰로 가는 시점에 여기에 걸려서 크래쉬가 발생할 수 있으므로 추가 체크를 한다.
//		)
//	{
//		bCanCacheCompositedMesh = false; // 임시적인 것으로 결과물 캐싱 안함. 다른 플레이어 메쉬는 캐싱하는 경우 역효과가 날 수 있다.
//		const FB2ModPlayerInfo& PlayerInfo = BladeIIGameImpl::GetClientDataStore().GetOtherUserInfo();
//		PlayerInfo.GetEquippedItems(InPCClass, EquippedItems);
//		PlayerInfo.GetEquippedCostumeItems(InPCClass, EquippedItems);
//		bHasWing = PlayerInfo.GetCharacterWing(InPCClass, WingData);
//	}
//	else if (CurrentLobbyScene == ELobbyScene::ELobbyScene_HallOfFame)
//	{
//		if (auto* HallOfFameScene = GetHallOfFameScene())
//		{
//			if (!HallOfFameScene->IsBackingScene())
//			{
//				HallOfFameScene->GetEquipItems(EquippedItems, WingData);
//				bHasWing = WingData.bShouldBeVisible;
//				bCanCacheCompositedMesh = false; // 임시적인 것으로 결과물 캐싱 안함. 다른 플레이어 메쉬는 캐싱하는 경우 역효과가 날 수 있다.
//			}
//			else
//			{
//				UB2LobbyInventory::GetAllEquippedItem(EquippedItems, InPCClass);
//				bHasWing = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InPCClass, WingData);
//			}
//		}
//	}
//	else
//	{
//		// 일반적인 로컬 플레이어 캐릭터의 경우
//		UB2LobbyInventory::GetAllEquippedItem(EquippedItems, InPCClass);
//		UB2LobbyInventory::GetAllEquippedCostumeItem(EquippedItems, InPCClass);
//		bHasWing = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InPCClass, WingData);
//	}
//
//	if (bHasWing && bUnconditionalShowWing) // 날개 진화 장면이나 도감 등에서 들어갈 조건. 기본적으로 bShouldBeVisible 은 사용자 선택에 따라 저장되는 값.
//	{
//		WingData.bShouldBeVisible = true;
//	}
//
//	// 연출On시 날개 제거
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_CharacterPage)
//	{
//		UB2UIDocUICondition* DocCon = UB2UIDocHelper::GetDocUICondition();
//
//		if(!DocCon->GetbUserSelectedSkipStory())
//			WingData.bShouldBeVisible = false;
//	}
//
//	//Mesh Render Item Filtering (미리보기, visible 옵션)
//	bool IgnoreFilter = CurrentLobbyScene == ELobbyScene::ELobbyScene_HallOfFame;
//	bool UseModCharacterDataStore = CurrentLobbyScene == ELobbyScene::ELobbyScene_FindAccount && bCanUseRemoteInfoForSKMeshSetup;
//
//	ICharacterDataStore* CurrentDataStore = nullptr;
//
//	if(UseModCharacterDataStore)
//		CurrentDataStore = &(BladeIIGameImpl::GetClientDataStore().GetOtherUserInfo());
//	else
//		CurrentDataStore = &BladeIIGameImpl::GetLocalCharacterData();
//
//	ICharacterDataStore::GetRenderItem(CurrentDataStore, InPCClass, EquippedItems, IgnoreFilter);
//	
//	
//	UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox(B2LobbyGM);
//	UB2PCClassInfo* ThisClassInfo = PCInfoBox ? PCInfoBox->GetSingleClassInfo(InPCClass) : NULL;
//	BII_CHECK(ThisClassInfo);
//
//	USkeletalMeshComponent* BaseMeshComp = InSKActorToSet->GetSkeletalMeshComponent();
//	BII_CHECK(BaseMeshComp);
//
//	const bool bMergeSections = false; // 로비는 Section Merge 를 쓸 가능성을 정말 생각하기 어려우나 검색 태그 정도는 달아두자 ㅋ UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent
//
//	if (ThisClassInfo && BaseMeshComp)
//	{
//		USkeletalMesh* PrebuiltMeshOrResult = NULL;
//		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//		if (!bForceUpdate && bCanCacheCompositedMesh)
//		{ // Mesh merge 하는 모드 한정, 이전에 이미 똑같은 조건으로 빌드해 놓은 메쉬가 있다면 그걸 꺼내 쓴다. 
//			// 현재 로비에서는 PCClass 만을 키로 해서 빌드한 메쉬를 캐싱하는데
//			// 만일 PCClass 는 같고 서로 다른 SkeletalMeshActor 가 한번에 보여야 하는 상황이 오면 PCCompositeMeshCacher 의 키값이 그걸 반영할 수 있도록 해야.
//			PrebuiltMeshOrResult = MeshCacheManager ? 
//				MeshCacheManager->GetCachedCompositeMesh(InPCClass, NULL, EquippedItems, (bHasWing && WingData.bShouldBeVisible), &WingData, bMergeSections)
//				: NULL; 
//		}
//
//		if (UB2PCClassInfo::SetupSKCompForParts(InPCClass, InSKActorToSet, BaseMeshComp, PrebuiltMeshOrResult, ThisClassInfo->BaseMeshAsset, ThisClassInfo->DefaultPartsInfo,
//			// 첫 화면 6성 장비 조합 보여줄 때는 날개를 빼 보자. 거기서 어차피 보이지 말자고 할 거 같다.
//			EquippedItems, (bHasWing && !bUseFixedGradeEquip) ? &WingData : NULL, NULL,
//			bForceUpdate, // ForceUpdate 의 경우는 일시적인 preview 로 보고자 함이 크기 때문에 사용한 파트 리소스를 날리도록 함.
//			bMergeSections
//			))
//		{
//			if (MeshCacheManager && bCanCacheCompositedMesh)
//			{ // MeshMerging 이 실행되는 모드에서의 결과가 성공적이라면 CompositeMeshCache 쪽에 캐싱해 놓음. 내부적으로는 실제 업데이트가 되었을 때에만 캐싱이 될 것임.
//				MeshCacheManager->SetCachedCompositeMesh(InPCClass, NULL, PrebuiltMeshOrResult, EquippedItems, (bHasWing && WingData.bShouldBeVisible), &WingData, bMergeSections);
//			}
//		}
//
//		ABladeIIPlayer::SetupLODInfoForLobbyRepresentative(InSKActorToSet);
//	}
}


/** 
 * 성능에 쥐약이 될 수 있어서 강제 업데이트 인자는 제공하지 않는다.
 * 기본적으로 각 캐릭터 클래스 별로 merge 한 메쉬는 장착 상태, 날개 여부에 따라 refresh 를 해야하는지 판별을 함.
 */
void FLobbySceneManager::UpdateLobbyCharCompForParts(EPCClass InPCClass)
{ // 장비 장착 상태 업데이트 등의 경우 사용하기 위함.
	//if (InPCClass != EPCClass::EPC_End)
	//{
	//	const ELobbyLevelType CurrLobbyLevelType = LevelManager.GetLobbyLevelType(CurrentLobbyScene);

	//	if (CurrLobbyLevelType == ELobbyLevelType::ELobbyLevelType_A)
	//	{
	//		FLobbyCharacterInfo* CharInfoA = LobbyCharactersForALobbyType.Find(InPCClass);
	//		if (CharInfoA)
	//		{
	//			SetupLobbyCharCompForParts(InPCClass, CharInfoA->ActorForMain, false);
	//			SetupLobbyCharCompForParts(InPCClass, CharInfoA->ActorForSub, false);
	//		}
	//	}
	//	else if (CurrLobbyLevelType == ELobbyLevelType::ELobbyLevelType_B)
	//	{
	//		FLobbyCharacter* CharInfoB = LobbyCharactersForBLobbyType.Find(InPCClass);
	//		if (CharInfoB)
	//		{
	//			SetupLobbyCharCompForParts(InPCClass, CharInfoB->LevelActor, false);
	//		}
	//	}
	//	else if (CurrLobbyLevelType == ELobbyLevelType::ELobbyLevelType_G)
	//	{
	//		// G 컵 타입은 영웅관리만 업데이트.. 도감은 이렇게 업데이트 하면 안될 듯?
	//		if (CurrentLobbyScene == ELobbyScene::ELobbyScene_HeroManagement || CurrentLobbyScene == ELobbyScene::ELobbyScene_FindAccount)
	//		{
	//			FLobbyCharacterInfo* CharInfoG = LobbyCharactersForGLobbyType.Find(InPCClass);
	//			if (CharInfoG)
	//			{
	//				SetupLobbyCharCompForParts(InPCClass, CharInfoG->ActorForMain, false);
	//				SetupLobbyCharCompForParts(InPCClass, CharInfoG->ActorForSub, false);
	//			}
	//		}
	//	}
	//}
}
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 
//void FLobbySceneManager::DevForceUpdateLobbyCharCompForParts(EPCClass InPCClass)
//{
//	/////
//	// 캐릭터 장비 조합 관련 이런 저런 테스트에 요긴하게 사용하기 위해.. 
//	// 상단 UpdateLobbyCharCompForParts 와 거의 비슷하지만 관리를 빡세게 할 필요는 없겠다.
//	/////
//
//	if (InPCClass != EPCClass::EPC_End)
//	{
//		const ELobbyLevelType CurrLobbyLevelType = LevelManager.GetLobbyLevelType(CurrentLobbyScene);
//
//		if (CurrLobbyLevelType == ELobbyLevelType::ELobbyLevelType_A)
//		{
//			FLobbyCharacterInfo* CharInfoA = LobbyCharactersForALobbyType.Find(InPCClass);
//			if (CharInfoA)
//			{
//				//SetupLobbyCharCompForParts(InPCClass, CharInfoA->ActorForMain, true);
//				//SetupLobbyCharCompForParts(InPCClass, CharInfoA->ActorForSub, true);
//			}
//		}
//		else if (CurrLobbyLevelType == ELobbyLevelType::ELobbyLevelType_B)
//		{
//			FLobbyCharacter* CharInfoB = LobbyCharactersForBLobbyType.Find(InPCClass);
//			if (CharInfoB)
//			{
//				//SetupLobbyCharCompForParts(InPCClass, CharInfoB->LevelActor, true);
//			}
//		}
//		else if (CurrLobbyLevelType == ELobbyLevelType::ELobbyLevelType_G)
//		{
//			FLobbyCharacterInfo* CharInfoG = LobbyCharactersForGLobbyType.Find(InPCClass);
//			if (CharInfoG)
//			{
//				//SetupLobbyCharCompForParts(InPCClass, CharInfoG->ActorForMain, true);
//				//SetupLobbyCharCompForParts(InPCClass, CharInfoG->ActorForSub, true);
//			}
//		}
//	}
//}
//#endif

void FLobbySceneManager::AddSummonItemDisplayMatinee(bool bIsMultiple,bool bIsOpen, bool bIsFriendShip, class AMatineeActor* Matinee)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::AddSummonItemDisplayMatinee"));

	//if (Matinee)
	//{
	//	SummonItemDisplayMapKey Key(bIsMultiple, bIsOpen, bIsFriendShip);
	//	SummonItemMatinees.Emplace(Key, TWeakObjectPtr<AMatineeActor>(Matinee));

	//	Matinee->SetPosition(0.f);
	//	SetMatineeHiddenInGame(Matinee, true);
	//}
}

//void FLobbySceneManager::AddSummonItemDisplayMatineeResult(class AMatineeActor* Matinee)
//{
//	SummonItemMatineesResult = Matinee;
//}

void FLobbySceneManager::HeroTransientEffectBegin(const int32 Index)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::HeroTransientEffectBegin"));

	//const auto LobbyPcClass = GetHeroMgmtModePCClass();
	//const auto CurrLobbyLevelType = LevelManager.GetLobbyLevelType(CurrentLobbyScene);

	//if (EPCClass::EPC_End == LobbyPcClass || ELobbyLevelType::ELobbyLevelType_None == CurrLobbyLevelType)
	//{
	//	BII_CHECK(false);
	//	return;
	//}

	//FLobbyCharacterInfo* CharInfo = NULL;

	//if (CurrLobbyLevelType == ELobbyLevelType::ELobbyLevelType_A)
	//{
	//	CharInfo = LobbyCharactersForALobbyType.Find(LobbyPcClass);
	//}
	//else if (CurrLobbyLevelType == ELobbyLevelType::ELobbyLevelType_G)
	//{
	//	CharInfo = LobbyCharactersForGLobbyType.Find(LobbyPcClass);
	//}

	//BII_CHECK(CharInfo);
	//if (CharInfo)
	//	CharInfo->ActorForMain->CreateEffect(Index);
}

//AB2LobbyNPCSkeletalMeshActor* FLobbySceneManager::GetNPCSKActor(ELobbyNPCType InType)
//{
//	FLobbyNPCCharacterInfo* FoundInfo = LobbyNPCs.Find(InType);
//	return FoundInfo ? FoundInfo->LevelActor : nullptr;
//}

void FLobbySceneManager::GetPointLightsOfObserveView(TArray<APointLight*>& OutPointLights)
{
	OutPointLights.Add(LobbyMainScenePointLights.PL_ObserveView_1);
	OutPointLights.Add(LobbyMainScenePointLights.PL_ObserveView_2);
}
void FLobbySceneManager::GetPointLightsOfAllCharView(TArray<APointLight*>& OutPointLights)
{
	OutPointLights.Add(LobbyMainScenePointLights.PL_AllCharView_1);
	OutPointLights.Add(LobbyMainScenePointLights.PL_AllCharView_2);
}
void FLobbySceneManager::SetPointLightsOfLobbyMain(bool bForObserveScene /* True for Observe view, false for AllCharacter view */)
{
//#if !UE_BUILD_SHIPPING
//	TConsoleVariableData<int32>* MobileNumDynamicPointLightsCVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.MobileNumDynamicPointLights"));
//	const int32 MobileNumDynamicPointLights = MobileNumDynamicPointLightsCVar ? MobileNumDynamicPointLightsCVar->GetValueOnGameThread() : 2;
//#endif
//
//	{
//		TArray<APointLight*> AllPLForObserveView;
//		GetPointLightsOfObserveView(AllPLForObserveView);
//#if !UE_BUILD_SHIPPING
//		check(AllPLForObserveView.Num() <= MobileNumDynamicPointLights); // 렌더러 한도를 넘기는 일 없도록.
//#endif
//		for (APointLight* ThisPL : AllPLForObserveView)
//		{
//			if (ThisPL && ThisPL->GetLightComponent())
//			{
//				ThisPL->GetLightComponent()->bAffectsWorld = bForObserveScene ? true : false;
//				ThisPL->GetLightComponent()->MarkRenderStateDirty();
//			}
//		}
//	}
//
//	{
//		TArray<APointLight*> AllPLForAllCharView;
//		GetPointLightsOfAllCharView(AllPLForAllCharView);
//#if !UE_BUILD_SHIPPING
//		check(AllPLForAllCharView.Num() <= MobileNumDynamicPointLights); // 렌더러 한도를 넘기는 일 없도록.
//#endif
//		for (APointLight* ThisPL : AllPLForAllCharView)
//		{
//			if (ThisPL && ThisPL->GetLightComponent())
//			{
//				ThisPL->GetLightComponent()->bAffectsWorld = bForObserveScene ? false : true;
//				ThisPL->GetLightComponent()->MarkRenderStateDirty();
//			}
//		}
//	}
}

void FLobbySceneManager::PlaySummonItemDisplayMatinee(bool bIsMultiple, bool bIsOpen, bool bIsFriendShip)
{

	SummonItemDisplayMapKey Key(bIsMultiple, bIsOpen, bIsFriendShip);


	auto MatineeWeakPtr = SummonItemMatinees.FindRef(Key);

	/*if (GEngine->ActiveMatinee.IsValid() && GEngine->ActiveMatinee.Get() != MatineeWeakPtr.Get() && GEngine->ActiveMatinee.Get()->GetWorld())
		GEngine->ActiveMatinee.Get()->Stop();*/

	//if (MatineeWeakPtr.IsValid())
	//{
	//	SetMatineeHiddenInGame(MatineeWeakPtr.Get(), false);
	//	MatineeWeakPtr->Play();
	//	MatineeWeakPtr->SetPosition(0.f);

	//	//마티니 이벤트로 처리하는 것도 괜찮을 듯.
	//	if (bIsOpen == false)
	//	{
	//		FTimerHandle TimeHandle;
	//		GetLobbyGameMode()->GetWorld()->GetTimerManager().SetTimer(
	//			TimeHandle,
	//			FTimerDelegate::CreateLambda([this, MatineeWeakPtr]() {/*MatineeWeakPtr->SetPosition(MatineeWeakPtr->MatineeData->InterpLength);*/
	//			this->OnChangedToFxSummonItemDisplayMatinee(MatineeWeakPtr.Get()); }),
	//			MatineeWeakPtr->MatineeData->InterpLength,
	//			false);
	//	}
	//	
	//}
}

void FLobbySceneManager::PlaySummonItemDisplayMatineeResult()
{
//	if (SummonItemMatineesResult.IsValid())
//	{
//
//		auto MatineeWeak = SummonItemMatineesResult;
//		MatineeWeak->SetPosition(0.f);
//		SummonItemMatineesResult->Play();
//
//		OnChangedToFxSummonItemDisplayMatinee(MatineeWeak.Get());
//
//	}
}

void FLobbySceneManager::OnFinishedSummonItemDisplayResultMatinee()
{
	//auto* DisplayUI = UB2UIManager::GetInstance()->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);
	//if (DisplayUI)
	//{
	//	DisplayUI->OnFinishedSummonItemDisplayResultMatinee();
	//}
}


//void FLobbySceneManager::OnFinishedSummonItemDisplayMatinee(AMatineeActor* Matinee)
//{
//	check(CurrentLobbyScene == ELobbyScene::ELobbyScene_SummonItemDisplay || CurrentLobbyScene == ELobbyScene::ELobbyScene_GeneralLotteryDisplay)
//
//	//OnChangedToFxSummonItemDisplayMatinee(Matinee);
//
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_SummonItemDisplay || CurrentLobbyScene == ELobbyScene::ELobbyScene_GeneralLotteryDisplay)
//	{
//		Matinee->Play();
//		Matinee->SetPosition(Matinee->InterpPosition);
//		Matinee->Pause();
//		OnChangedToFxSummonItemDisplayMatinee(Matinee);
//		// static_cast ..?
//		auto* DisplayScene = static_cast<FLobbyGachaDisplaySceneBase*>(LobbyScenes[static_cast<int32>(CurrentLobbyScene)]);
//		if (DisplayScene)
//		{
//			DisplayScene->OnFinishedSummonItemDisplayMatinee(Matinee);
//		}
//	}
//}
//
//void FLobbySceneManager::OnChangedToFxSummonItemDisplayMatinee(AMatineeActor* Matinee)
//{
//	check(CurrentLobbyScene == ELobbyScene::ELobbyScene_SummonItemDisplay || CurrentLobbyScene == ELobbyScene::ELobbyScene_GeneralLotteryDisplay);
//
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_SummonItemDisplay || CurrentLobbyScene == ELobbyScene::ELobbyScene_GeneralLotteryDisplay)
//	{
//		// static_cast ..?
//		auto* DisplayScene = static_cast<FLobbyGachaDisplaySceneBase*>(LobbyScenes[static_cast<int32>(CurrentLobbyScene)]);
//		if (DisplayScene)
//		{
//			DisplayScene->OnChangedToFxSummonItemDisplayMatinee(Matinee);
//		}
//	}
//	//SetMatineeHiddenInGame(Matinee, true);
//}

void FLobbySceneManager::SetHallOfFameInfo(FB2ResponseGetHallOfFamePtr InHallOfFameInfo)
{
	if (auto* HallOfScene = GetHallOfFameScene())
	{
		HallOfScene->SetHallOfFameInfo(InHallOfFameInfo);
	}
}

void FLobbySceneManager::SetPraiseInfo(FB2ResponsePraiseTargetRankerPtr InPraiseInfo)
{
	if (auto* HallOfScene = GetHallOfFameScene())
	{
		HallOfScene->SetPraiseInfo(InPraiseInfo);
	}
}

void FLobbySceneManager::ExecutePendingHallOfFameScene()
{
	//if (auto* DocMode = UB2UIDocHelper::GetDocMode())
	//{
	//	if (DocMode->GetContentsModeState(b2network::B2ContentsMode::PVP_TEAM) != b2network::B2ContentsModeState::SETTLE_BEGIN
	//		&& DocMode->GetContentsModeState(b2network::B2ContentsMode::PVP_DUEL) != b2network::B2ContentsModeState::SETTLE_BEGIN
	//		&& DocMode->GetContentsModeState(b2network::B2ContentsMode::ASSAULT) != b2network::B2ContentsModeState::SETTLE_BEGIN
	//		&& DocMode->GetContentsModeState(b2network::B2ContentsMode::PVP_TEAM) != b2network::B2ContentsModeState::SETTLE_END
	//		&& DocMode->GetContentsModeState(b2network::B2ContentsMode::PVP_DUEL) != b2network::B2ContentsModeState::SETTLE_END
	//		&& DocMode->GetContentsModeState(b2network::B2ContentsMode::ASSAULT) != b2network::B2ContentsModeState::SETTLE_END
	//		&& DocMode->GetContentsModeState(b2network::B2ContentsMode::PVP_TEAM) != b2network::B2ContentsModeState::MODE_FINISH
	//		&& DocMode->GetContentsModeState(b2network::B2ContentsMode::PVP_DUEL) != b2network::B2ContentsModeState::MODE_FINISH
	//		&& DocMode->GetContentsModeState(b2network::B2ContentsMode::ASSAULT) != b2network::B2ContentsModeState::MODE_FINISH
	//		&& DocMode->GetContentsModeState(b2network::B2ContentsMode::PVP_TEAM) != b2network::B2ContentsModeState::BLOCK_ENTER
	//		&& DocMode->GetContentsModeState(b2network::B2ContentsMode::PVP_DUEL) != b2network::B2ContentsModeState::BLOCK_ENTER
	//		&& DocMode->GetContentsModeState(b2network::B2ContentsMode::ASSAULT) != b2network::B2ContentsModeState::BLOCK_ENTER
	//		)
	//	{
	//		data_trader::RequestGetHallOfFameClass<int32, bool>::GetInstance().Signal(0, true);
	//	}
	//	else
	//	{
	//		if (auto* UIManager = UB2UIManager::GetInstance())
	//		{
	//			int32 nStartDay = -1;
	//			int32 nStartHour = -1;
	//			int32 nStartMin = -1;
	//			int32 nEndDay = -1;
	//			int32 nEndHour = -1;
	//			int32 nEndMin = -1;

	//			FNumberFormattingOptions FormattingOption;
	//			FormattingOption.MinimumIntegralDigits = 2;

	//			// RUN 끝나는 시간
	//			BladeIIGameImpl::GetClientDataStore().GetDuelModeSettleSchedule(
	//				b2network::B2ContentsMode::PVP_DUEL,
	//				b2network::B2ContentsModeState::RUN,
	//				nStartDay,
	//				nStartHour,
	//				nStartMin);

	//			// SETTLE_END 끝나는 시간
	//			BladeIIGameImpl::GetClientDataStore().GetDuelModeSettleSchedule(
	//				b2network::B2ContentsMode::PVP_DUEL,
	//				b2network::B2ContentsModeState::SETTLE_END,
	//				nEndDay,
	//				nEndHour,
	//				nEndMin);

	//			FText HallOfFameText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HallOfFrame_CalculatePopupMessage"));
	//			FString HallOfFameString = HallOfFameText.ToString();
	//			HallOfFameString.Append(TEXT("\n\n"));
	//			HallOfFameString.Append(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HallOfFrame_CalculteTime")), FText::AsNumber(nStartHour, &FormattingOption), FText::AsNumber(nStartMin, &FormattingOption), FText::AsNumber(nEndHour, &FormattingOption), FText::AsNumber(nEndMin, &FormattingOption)).ToString());

	//			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//				FText::FromString(HallOfFameString),
	//				0.f,
	//				true,
	//				true,
	//				EUIMsgPopupButtonGroup::Confirm
	//			);
	//		}
	//	}
	//}
}

//void FLobbySceneManager::OnFinishedHallOfFameDisplayMatinee(class AMatineeActor* Matinee)
//{
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_HallOfFame)
//	{
//		if (auto* HallOfFameScene = GetHallOfFameScene())
//		{
//			HallOfFameScene->OnFinishedHallOfFameDisplayMatinee(Matinee);
//		}
//	}
//}

bool FLobbySceneManager::IsHallOfFameData(FB2ResponseGetHallOfFamePtr InHallOfFameInfo)
{
	//if (InHallOfFameInfo && InHallOfFameInfo->basics.Num() >= 9 && InHallOfFameInfo->left_praise_counts.Num() >= 3)
	//{
	//	return true;
	//}

	//UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//	BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//	BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HallOfFame_FailRankingInfo")),
	//	0.f,
	//	true,
	//	true,
	//	EUIMsgPopupButtonGroup::Confirm
	//);
	
	return false;
}

//AMatineeActor* FLobbySceneManager::GetDefaultCameraMatinee()
//{
//	return DefaultCameraMatinee;
//}

void FLobbySceneManager::SetDefaultCameraMatinee(class AMatineeActor* DefaultMatinee)
{
	DefaultCameraMatinee = DefaultMatinee;
}

//AMatineeActor* FLobbySceneManager::GetChapterMatinee()
//{
//	return ChapterMatinee;
//}

void FLobbySceneManager::SetChapterViewMatinee(class AMatineeActor* InChapterMatinee)
{
	ChapterMatinee = InChapterMatinee;
}

EPCClass FLobbySceneManager::GetHeroMgmtModePCClass()
{
	return static_cast<FLobbyHeroMgmtScene*>(LobbyScenes[static_cast<uint8>(ELobbyScene::ELobbyScene_HeroManagement)])->GetCurrentPCClass();
}

void FLobbySceneManager::SetHeroMgmtModePCClass(EPCClass pcClass)
{
	return static_cast<FLobbyHeroMgmtScene*>(LobbyScenes[static_cast<uint8>(ELobbyScene::ELobbyScene_HeroManagement)])->SetCurrentPCClass(pcClass);
}

void FLobbySceneManager::Zoom(float CamDistDelta, const FVector2D& Focus)
{
	//if (CurrentLobbyScene == ELobbyScene::ELobbyScene_CharacterObserve)
	//{
	//	if (CamDistDelta < 0.f)
	//		LobbyCharCameraViewTargetClass<const FVector2D&>::GetInstance().Signal(Focus);
	//	LobbyCharMoveCameraClass<const FRotator&, float>::GetInstance().Signal(FRotator::ZeroRotator, CamDistDelta * LobbyCameraZoomSpeed);
	//}

	//else if (CurrentLobbyScene == ELobbyScene::ELobbyScene_HeroManagement ||
	//	CurrentLobbyScene == ELobbyScene::ELobbyScene_CollectBookMain)
	//{
	//	LobbyCharZoomClass<const FVector2D&, float>::GetInstance().Signal(Focus, CamDistDelta);
	//}
}

void FLobbySceneManager::SwipeV(float Value)
{
	//if (CurrentLobbyScene == ELobbyScene::ELobbyScene_CharacterObserve)
	//{
	//	if (UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::LobbyMain)
	//	{
	//		UB2UILobbyMain* LobbyMainUIPage = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	//		if (LobbyMainUIPage)
	//		{
	//			if (LobbyMainUIPage->GetCombineMenuIsVisible()) // 통합메뉴가 떠있는 상태에서는 사용안되게끔
	//				return;
	//		}
	//		UB2UIMsgPopupSelectFairyGift* pMsgPopupSelectFairyGift = UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupSelectFairyGift>(UIFName::MsgPopupSelectFairyGift);
	//		if (pMsgPopupSelectFairyGift)
	//		{
	//			// 페어리 보상 팝업이 떠있는 상태에서도 사용 안되게끔
	//			return;
	//		}
	//	}

	//	LobbyCharMoveCameraClass<const FRotator&, float>::GetInstance().Signal(FRotator(Value * 45.f * LobbyCameraRotationY, 0.f, 0.f), 0.f);
	//}
}

void FLobbySceneManager::SwipeH(float Value)
{
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_Chapter || CurrentLobbyScene == ELobbyScene::ElobbyScene_ModSelect)
//		LobbyChapterCameraMoveClass<float>::GetInstance().Signal(Value * CharacterCameraRotateFactor * 0.005f);
//#if CHAR_ACTOR_ROTATION
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_HeroManagement && GetLobbyGameMode()->IsTouchingRotateAreaInHeroMgnt())
//		LobbyCharRotateCharacterClass<float>::GetInstance().Signal(Value * CharacterCameraRotateFactor);
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_CharacterObserve)
//	{
//		if (UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::LobbyMain)
//		{
//			UB2UILobbyMain* LobbyMainUIPage = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
//			if (LobbyMainUIPage)
//			{
//				if (LobbyMainUIPage->GetCombineMenuIsVisible()) // 통합메뉴가 떠있는 상태에서는 사용안되게끔
//					return;
//			}
//			UB2UIMsgPopupSelectFairyGift* pMsgPopupSelectFairyGift = UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupSelectFairyGift>(UIFName::MsgPopupSelectFairyGift);
//			if (pMsgPopupSelectFairyGift)
//			{
//				// 페어리 보상 팝업이 떠있는 상태에서도 사용 안되게끔
//				return;
//			}
//		}
//		LobbyCharMoveCameraClass<const FRotator&, float>::GetInstance().Signal(FRotator(0.f, -Value * LobbyCameraRotationX, 0.f), 0.f);
//	}
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_CollectBookMain)
//		LobbyCharRotateCharacterClass<float>::GetInstance().Signal(Value * CharacterCameraRotateFactor);
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_FindAccount)
//		LobbyCharRotateCharacterClass<float>::GetInstance().Signal(Value * CharacterCameraRotateFactor);
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_LobbyMain)
//		LobbyMainCameraMoveClass<float>::GetInstance().Signal(Value * CharacterCameraRotateFactor * 0.5f);
//#endif
}

void FLobbySceneManager::FlickV(bool bUpDirection)
{

}

void FLobbySceneManager::FlickH(bool bLeftDirection)
{
#if CHAR_ANIM_ROTATION
	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_HeroManagement && GetLobbyGameMode()->IsTouchingRotateAreaInHeroMgnt())
		LobbyCharRotateCharacterClass<float>::GetInstance().Signal(!bLeftDirection ? 1.f : -1.f);
	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_CharacterObserve)
		LobbyCharRotateCharacterClass<float>::GetInstance().Signal(!bLeftDirection ? 1.f : -1.f);
#endif
}

void FLobbySceneManager::SetItemOpDirectingSceneObjects(class ACameraActor* InCamActor)
{
	ItemOpSceneCamera = InCamActor;
}

void FLobbySceneManager::EnterItemOpDirectingView(ELobbyInvenItemOpMode InOpMode)
{
	/*if (ItemOpSceneCamera && ItemOpSceneCamera->IsValidObj() && ItemOpSceneCamera->GetCameraComponent())
	{
		APlayerController* LocalController = UGameplayStatics::GetLocalPlayerController(ItemOpSceneCamera);
		if (LocalController)
		{
			ItemOpSceneCamera->GetCameraComponent()->Activate(true);
			LocalController->SetViewTargetWithBlend(ItemOpSceneCamera, 0.0f);
		}
	}*/
}

void FLobbySceneManager::QuitItemOpDirectingView()
{
	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_HeroManagement)
	{ // 이건 HeroManagement 에서 HeroManagement 로의 (force) 세팅임. 타이밍으로 인해 다른 모드로 넘어간 상태에서 이게 불리면 안된다.
		ChangeLobbyScene(ELobbyScene::ELobbyScene_HeroManagement, -1, true);
	}
}

bool FLobbySceneManager::IsInItemOpDirectingView()
{ // 확인 차원에서 필요한 기능
	//if (ItemOpSceneCamera && ItemOpSceneCamera->IsValidObj() && ItemOpSceneCamera->GetCameraComponent())
	//{
	//	APlayerController* LocalController = UGameplayStatics::GetLocalPlayerController(ItemOpSceneCamera);
	//	return (LocalController && LocalController->GetViewTarget() == ItemOpSceneCamera);
	//}
	return false;
}

void FLobbySceneManager::OnLobbyGMActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject)
{
	// CurrentLobbyScene 에만 ActorCustomEvent 를 전달.
	uint8 CurLobbySceneIndex = static_cast<uint8>(CurrentLobbyScene);
	FLobbySceneBase* CurrentActiveLobbySceneRef = LobbyScenes.IsValidIndex(CurLobbySceneIndex) ? LobbyScenes[CurLobbySceneIndex] : NULL;

	if (CurrentActiveLobbySceneRef){
		CurrentActiveLobbySceneRef->OnLobbyGMActorCustomEvent(OptionalEventName, OptionalNotifyingObject);
	}
}

void FLobbySceneManager::ResetObjectRefsOnSceneLevelStreaming()
{
	// 이게 UPROPERTY 로 묶인 게 아니라서 TWeakObjectPtr 이 아닌 이상 GC 라도 되면 쓰레기 포인터가 될 것이다.
	// 근데 각각의 구현이 어떤 식인지 몰라서 다 리셋해도 되는지는 모르겠음.
	// LobbySceneManager 가 LobbyGameMode 제거될 땐 같이 날아가기 때문에 크게 문제의 소지가 있는 수준은 아니고..
	LobbyCharactersForALobbyType.Empty();
	LobbyCharactersForBLobbyType.Empty();
	LobbyCharactersForGLobbyType.Empty();
	LobbyCharactersForHallOfFame.Empty();
	HallOfFameCharacterAnimation.Empty();
	LobbyNPCs.Empty();
	LobbyFlyFairys.Empty();
	DefaultCameraMatinee = NULL;
	ChapterMatinee = NULL;
}

//=================================================================================================
//
//	Lobby Scene
//
//=================================================================================================
FLobbySceneBase::FLobbySceneBase(class FLobbySceneManager* OwnerSceneManager)
{
	SceneManager = OwnerSceneManager;
	check(SceneManager);	
}

FLobbySceneBase::~FLobbySceneBase()
{
	SceneManager = NULL;
}

void FLobbySceneBase::OpenScene()
{
	SubscribeEvents_OnOpen();
}
void FLobbySceneBase::CloseScene()
{
	UnsubscribeEvents_OnClose();
}

void FLobbySceneBase::SubscribeEvents_OnConstruct()
{
	// 전체 LobbyScene (LobbyGameMode) 과 라이프 사이클을 같이 할 event subscribe 를 overriding 을 통해 정의
	// 실제로 여기를 통해 subscribe 하는 게 바람직한 모습은 아니지만 오랬동안 그런 구조로 만들어져 왔어서 명시적으로 제공함.
}
void FLobbySceneBase::UnsubscribeEvents_OnDestruct()
{
	// SubscribeEvents_OnConstruct 에서 subscribe 한 것은 이걸 override 해서 unsubscribe 한다.
}

void FLobbySceneBase::SubscribeEvents_OnOpen()
{
	// 특정 LobbyScene 이 활성화된 동안에만 적용될 event subscribe 를 overriding 을 통해 정의
}
void FLobbySceneBase::UnsubscribeEvents_OnClose()
{
	// SubscribeEvents_OnOpen 에서 subscribe 한 것은 이걸 override 해서 unsubscribe 한다.
}

TMap<EPCClass, FLobbyCharacterInfo>&	FLobbySceneBase::GetLobbyCharactersForATypeLevel()
{
	check(SceneManager);
	return SceneManager->GetLobbyCharactersForATypeLevel();
}

TMap<EPCClass, FLobbyCharacter>&		FLobbySceneBase::GetLobbyCharactersForBTypeLevel()
{
	check(SceneManager);
	return SceneManager->GetLobbyCharactersForBTypeLevel();
}

TMap<EPCClass, FLobbyCharacterInfo>& FLobbySceneBase::GetLobbyCharactersForGTypeLevel()
{
	check(SceneManager);
	return SceneManager->GetLobbyCharactersForGTypeLevel();
}

FORCEINLINE TArray<FHallOfFameCharacterInfo>& FLobbySceneBase::GetLobbyCharactersForHallOfFame()
{
	check(SceneManager);
	return SceneManager->GetLobbyCharactersForHallOfFame();
}

//TMap<EPCClass, class UAnimationAsset*>& FLobbySceneBase::GetHallOfAnimation(bool IsBestCharacter)
//{
//	check(SceneManager);
//	return SceneManager->GetHallOfAnimation(IsBestCharacter);
//}

//AMatineeActor* FLobbySceneBase::GetDefaultCameraMatinee()
//{
//	check(SceneManager);
//	return SceneManager->GetDefaultCameraMatinee();
//}
//
//AMatineeActor* FLobbySceneBase::GetChapterMatinee()
//{
//	check(SceneManager);
//	return SceneManager->GetChapterMatinee();
//}

//void FLobbySceneBase::SetCameraMatineeCam(class AMatineeActor* MatineeActor, float KeyTime)
//{
//	// MatineeActor 에 배치한 마티니 트랙 그룹에서 제일 먼저 찾는 카메라 트랙 그룹의 KeyTime 을 현재 카메라로 설정.
//	ABladeIIGameMode::SetCameraMatineeCam(MatineeActor, KeyTime);
//}

APlayerController* FLobbySceneBase::GetLocalPlayerController()
{
	auto* LobbyGameMode = GetLobbyGameMode();
	if (LobbyGameMode)
	{
		//return UGameplayStatics::GetLocalPlayerController(LobbyGameMode);
	}

	return NULL;
}

AB2LobbyGameMode* FLobbySceneBase::GetLobbyGameMode()
{
	return SceneManager ? SceneManager->GetLobbyGameMode() : NULL;
}

class UGameViewportClient* FLobbySceneBase::GetViewportClient()
{
	if (GetLocalPlayerController() && GetLocalPlayerController()->GetLocalPlayer())
	{
		if (const ULocalPlayer* LocalPlayer = GetLocalPlayerController()->GetLocalPlayer())
		{
			return LocalPlayer->ViewportClient;
		}
	}

	return nullptr;
}

//=================================================================================================
//
//	Character Intro
//
//=================================================================================================
FLobbyCharacterIntroScene::FLobbyCharacterIntroScene(class FLobbySceneManager* OwnerSceneManager) : FLobbySceneBase(OwnerSceneManager)
{
	CurrentPCClass = EPCClass::EPC_End;
}

FLobbyCharacterIntroScene::~FLobbyCharacterIntroScene()
{
}

void FLobbyCharacterIntroScene::OpenScene()
{
	FLobbySceneBase::OpenScene();

	//SelectCharacterIntro(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
}

void FLobbyCharacterIntroScene::CloseScene()
{
	//FLobbySceneBase::CloseScene();

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();

	//auto* LobbyCharInfo = LobbyCharactersForALobbyType.Find(CurrentPCClass);
	//if (LobbyCharInfo)
	//{
	//	LobbyCharInfo->MatineeForMain->Stop();
	//}

	//for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//{
	//	GetSceneManager()->UpdateLobbyCharCompForParts(IntToPCClass(PCI));
	//}
}

void FLobbyCharacterIntroScene::SubscribeEvents_OnConstruct()
{
	//LobbyCharIntroSelectTicket = LobbyCharIntroSelectClass<EPCClass>::GetInstance().Subscribe([this](EPCClass SelectedClass) { this->SelectCharacterIntro(SelectedClass); });
}
void FLobbyCharacterIntroScene::UnsubscribeEvents_OnDestruct()
{
	//LobbyCharIntroSelectClass<EPCClass>::GetInstance().Unsubscribe(LobbyCharIntroSelectTicket);
}

void FLobbyCharacterIntroScene::SelectCharacterIntro(EPCClass SelectedCharacter)
{
	//if (!(this->GetSceneManager() && this->GetSceneManager()->GetCurrentLobbyScene() == this->GetLobbyScene()))
	//{
	//	return;
	//}

	//if (SelectedCharacter == CurrentPCClass)
	//	return; 

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();

	//auto* LobbyCharInfo = LobbyCharactersForALobbyType.Find(CurrentPCClass);
	//if (LobbyCharInfo)
	//{
	//	LobbyCharInfo->MatineeForMain->Stop();
	//}
	//CurrentPCClass = SelectedCharacter;
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//		CharacterHiddenAndStopSound(It.Value().ActorForMain, true);
	//	if (It.Value().ActorForSub)
	//		CharacterHiddenAndStopSound(It.Value().ActorForSub, true);
	//}

	//LobbyCharInfo = LobbyCharactersForALobbyType.Find(CurrentPCClass);
	//if (LobbyCharInfo)
	//{
	//	CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, false);

	//	LobbyCharInfo->MatineeForMain->SetPosition(0);
	//	LobbyCharInfo->MatineeForMain->Play();

	//	SetCameraMatineeCam(LobbyCharInfo->MatineeForMain);

	//	if (LobbyCharInfo->ActorForMain)
	//		LobbyCharInfo->ActorForMain->PlaySelectAnimation();
	//}
}

//=================================================================================================
//
//	Lobby Main
//
//=================================================================================================
FLobbyMainScene::FLobbyMainScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
	, MatineePos(0.0f)
	, PlayRateRestoreTime(1.0f)
{
	AutoHideInterval = 20.0f;

}

FLobbyMainScene::~FLobbyMainScene()
{
	
}

void FLobbyMainScene::OpenScene()
{	
	//FLobbySceneManager* LobbySM = GetSceneManager();
	//if (LobbySM)
	//{ // 정상적인 상황이면 상점뽑기 직후 상점 돌아와서 리셋될 것인데 혹시 모르는 상황이 있을지 모르니 확인사살 차원. 
	//	// 요게 적어도 LobbyMain 씬을 넘나들며 유지될 이유는 없을 것.
	//	LobbySM->ResetShopGirlReservedState();
	//}

	//FLobbySceneBase::OpenScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (!DefaultCameraMatinee) // 버그 상황이 있어서 BeginPlay 전에 이게 불릴 때가 있음. 강제 캐싱
	//{
	//	AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
	//	if (LobbyGameMode)
	//	{
	//		LobbyGameMode->UpdateNativeLobbyMatineeRef();
	//		UE_LOG(LogBladeII, Warning, TEXT("Call UpdateNativeLobbyMatineeRef at FLobbyMainScene::OpenScene to force re-cache lobby resources.. This is a workaround for a bug, not a solid solution."));
	//	}
	//	DefaultCameraMatinee = GetDefaultCameraMatinee();
	//}

	//if (DefaultCameraMatinee)
	//{
	//	MatineePos = ABladeIIGameMode::GetMatineePositionByEventName(DefaultCameraMatinee, TEXT("Start"));

	//	DefaultCameraMatinee->SetPosition(GetDefaultLobbyMatineeTimeStamp());
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);

	//	TMap<EPCClass, FLobbyCharacter> Charinfos = GetLobbyCharactersForBTypeLevel();
	//	for (auto charinfo : Charinfos)
	//	{
	//		// 여긴 애니메이션을 마티니 플레이로 구동시킬 것이므로 잠시 바꿔둠..
	//		charinfo.Value.LevelActor->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//	}
	//}

	//ElapsedTimeNoInput = ElapsedOpenScene = 0.0f;
	//bVisibleUI = true;
	//
	//// UB2UILobbyMain 을 숨기는 설정을 UB2UILobbyMain 에서 가져와서 정작 타이머는 여기서 돌리는 식인데 기존 구현에 맞춰 설정은 나중에 넣다보니 그렇게 된 거.
	//UB2UILobbyMain* LobbyMainUIPage = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	//if (LobbyMainUIPage)
	//{
	//	AutoHideInterval = LobbyMainUIPage->GetAutoHideInterval();
	//	LobbyMainUIPage->SetVisibility(ESlateVisibility::Collapsed);
	//}

	//if(UB2UIManager::GetInstance())
	//	UB2UIManager::GetInstance()->OpenUI<UB2UILobbySelectCharacter>(UIFName::LobbySelectCharacter);
	//
	//if (LobbySM)
	//{ // 바로 아래에 ObserverScene 으로 전환하기 전에 MainScene 에서의 라이팅 셋업을 해야 한다.
	//	// 이게 그 다음에 실행되면 아래에서 ObserverScene 으로 전환하면서 그쪽 라이팅 셋업이 된 걸 덮어쓰게 됨.
	//	LobbySM->SetPointLightsOfLobbyMain(false);
	//}

	//static bool bFirstLobbyEnter = true;
	//auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	//if (DocBattle && GetLobbyGameMode())
	//{ 
	//	EPCClass ObserveClass = IntToPCClass(DocBattle->GetCurPCClass());
	//	if (bFirstLobbyEnter)
	//	{
	//		bFirstLobbyEnter = false;
	//	}

	//	if (ObserveClass != EPCClass::EPC_End)
	//		GetLobbyGameMode()->SelectLobbyMainCharacter(ObserveClass);
	//}
}

void FLobbyMainScene::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//auto* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->SetEnableClickEvents(false);
	//}

	//if(UB2UIManager::GetInstance())
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::LobbySelectCharacter);
}

void FLobbyMainScene::SubscribeEvents_OnConstruct()
{
	//LobbyMainCameraMoveTicket = LobbyMainCameraMoveClass<float>::GetInstance().Subscribe(
	//	[this](float Value)
	//{
	//	ElapsedTimeNoInput = 0.0f;
	//	this->SwipeScene(Value);
	//});
}
void FLobbyMainScene::UnsubscribeEvents_OnDestruct()
{
	//LobbyMainCameraMoveClass<float>::GetInstance().Unsubscribe(LobbyMainCameraMoveTicket);
}

void FLobbyMainScene::Tick(float DeltaSeconds)
{
	//if (!UB2UIManager::GetInstance()->IsUIScene(EUIScene::LobbyMain))
	//	return;

	//if(bVisibleUI)
	//	SetHiddenLobbyMainUI(true);
	//
	//ElapsedTimeNoInput += DeltaSeconds;
	//auto* PC = GetLocalPlayerController();
	//if (PC && (PC->WasInputKeyJustPressed(EKeys::LeftMouseButton) || PC->WasInputKeyJustPressed(EKeys::TouchKeys[0])))
	//{
	//	GetDefaultCameraMatinee()->PlayRate = 0.0f;
	//	ElapsedTimeNoInput = 0.0f;
	//}
	//if (ElapsedTimeNoInput >= PlayRateRestoreTime) // 일정 시간 인풋이 없을 시 마티니 재생 시간을 원래대로 돌려 놓는다
	//{
	//	GetDefaultCameraMatinee()->PlayRate = 1.0f;
	//}
	///*
	//auto* PC = GetLocalPlayerController();
	//if (PC && (PC->WasInputKeyJustPressed(EKeys::LeftMouseButton) || PC->WasInputKeyJustPressed(EKeys::TouchKeys[0])))
	//{
	//	SetHiddenLobbyMainUI(true);
	//	ElapsedTimeNoInput = 0.f;
	//}
	//if (ElapsedTimeNoInput >= AutoHideInterval) // 일정 시간 인풋이 없을 시 메인 UI 를 얖 옆으로 숨긴다.
	//{
	//	SetHiddenLobbyMainUI(true);
	//}
	//*/

	////뒤로가기 버튼이 Push event로 바뀌면서 다른 Scene에서 돌아올 때, Release event가 먹혀 캐릭터가 선택되어 버리는 버그 사항이 나올 수 있다. 
	////이 신에서 Press event를 캐치한 후, Release event가 불릴때만 실행하게 해야하는데, 시간상 구현이 힘들어 우선 이렇게 막아놓음 - 20160728
	//ElapsedOpenScene += DeltaSeconds;
	//if (ElapsedOpenScene > 0.25f)
	//{
	//	auto* LobbyGameMode = GetLobbyGameMode();
	//	if (LobbyGameMode)
	//	{
	//		LobbyGameMode->BindTouchInput(true);
	//		LobbyGameMode->SetEnableClickEvents(true);
	//	}
	//}
}

void FLobbyMainScene::SetHiddenLobbyMainUI(bool bNewHidden)
{
	if (bVisibleUI == !bNewHidden)
		return;

	//auto* DocCondition = UB2UIDocHelper::GetDocUICondition();
	//if (DocCondition && DocCondition->GetHideAllLobbyMainUIs() != bNewHidden)
	//	DocCondition->SetHideAllLobbyMainUIs(bNewHidden);

	//bVisibleUI = !bNewHidden;

	//// 페어리 안보이도록.
	//FLobbySceneManager* LobbySM = GetSceneManager();
	//if (!bVisibleUI)
	//{
	//	LobbySM->OnLobbyFlyFairy_AllHidden();
	//}
}

void FLobbyMainScene::SwipeScene(float Delta)
{
	//AMatineeActor* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee && DefaultCameraMatinee->IsValidObj()) // 레벨 스트리밍과 얽혀서 Invalid 한 경우가 있는 거 같다..
	//{
	//	MatineePos = DefaultCameraMatinee->InterpPosition;
	//	DefaultCameraMatinee->SetPosition(MatineePos);
	//	DefaultCameraMatinee->PlayRate += Delta > 0.0f ? Delta : -Delta;//2.0f;
	//	DefaultCameraMatinee->PlayRate = FMath::Clamp(DefaultCameraMatinee->PlayRate, 0.0f, 2.0f);
	//	if (Delta > 0.0f)
	//	{
	//		DefaultCameraMatinee->Play();
	//	}
	//	else if (Delta < 0.0f)
	//	{
	//		DefaultCameraMatinee->Reverse();
	//	}

	//	SetCameraMatineeCam(DefaultCameraMatinee, MatineePos);
	//}

	//// 하위 클래스에서 UpdateUIPosition 을 호출하도록..
}

float FLobbyMainScene::GetDefaultLobbyMatineeTimeStamp()
{
	float EventKeyTime = 0.0f;
	//AMatineeActor* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee && DefaultCameraMatinee->IsValidObj()) // 레벨 스트리밍과 얽혀서 Invalid 한 경우가 있는 거 같다..
	//{
	//	EPCClass MainPlayerClass = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
	//	switch (MainPlayerClass)
	//	{
	//		case EPCClass::EPC_Gladiator:
	//			EventKeyTime = ABladeIIGameMode::GetMatineePositionByEventName(DefaultCameraMatinee, TEXT("Gladiator"));
	//			break;
	//		case EPCClass::EPC_Assassin:
	//			EventKeyTime = ABladeIIGameMode::GetMatineePositionByEventName(DefaultCameraMatinee, TEXT("Assassin"));
	//			break;
	//		case EPCClass::EPC_Wizard:
	//			EventKeyTime = ABladeIIGameMode::GetMatineePositionByEventName(DefaultCameraMatinee, TEXT("Wizard"));
	//			break;
	//		case EPCClass::EPC_Fighter:
	//			EventKeyTime = ABladeIIGameMode::GetMatineePositionByEventName(DefaultCameraMatinee, TEXT("Fighter"));
	//			break;
	//		case EPCClass::EPC_End:
	//		default:
	//			EventKeyTime = ABladeIIGameMode::GetMatineePositionByEventName(DefaultCameraMatinee, TEXT("Start"));
	//			break;
	//	}
	//}
	return EventKeyTime;
}

//=================================================================================================
//
//	Character Observe
//
//=================================================================================================
FLobbyCharObserveScene::FLobbyCharObserveScene(class FLobbySceneManager* OwnerSceneManager) 
	: FLobbySceneBase(OwnerSceneManager), LobbyObserveCamera(this)
{

	CahsedSceneManager = OwnerSceneManager;
	IsPlayStartMatinee = false;
	StartMatineeDelay = 0.0f;
	CurrentStartMatineeDelay = 0.0f;
}

FLobbyCharObserveScene::~FLobbyCharObserveScene()
{

}

void FLobbyCharObserveScene::OpenScene()
{
	FLobbySceneBase::OpenScene();

	//ConditionalForceRecacheResource(); // Hack..


	//auto& LobbyCharactersForBLobbyType = GetLobbyCharactersForBTypeLevel();
	//for (TMap<EPCClass, FLobbyCharacter>::TIterator It(LobbyCharactersForBLobbyType); It; ++It)
	//{
	//	auto LobbyCharacter = It.Value();

	//	if (LobbyCharacter.LevelActor)
	//	{
	//		if (CurrentPCClass != It.Key())
	//		{
	//			CharacterHiddenAndStopSound(LobbyCharacter.LevelActor, true);
	//		}
	//		else
	//		{
	//			LobbyCharacter.LevelActor->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//		}
	//	}
	//}

	//// 첫 로비 진입때는 외부에서 처리
	//if (!GetSceneManager()->IsPlayingBeginAnim()) {
	//	PlaySelectMatinee();
	//}
	//else{
	//	// -> 로비 진입 후 출석 창 뜰때까지 지연이 심할 때 보이는 장면이 넘 뻘쭘해서 일단 뭐라도 보여줌.
	//	PlaySelectMatinee(
	//		false, // 대략 Begin 시점이긴 한데 여기서 원하는 Begin 은 출석창 닫은 직후라 false 로 넘겨주어야
	//		true // Closeup 과정을 보여주게 되면 그게 출석창 닫고 나서 재탕되는 느낌이 될 수 있으니 true 로 넘겨주어서 바로 closeup 뷰 직행
	//	);
	//}

	//auto* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->BindTouchInput(true);
	//	LobbyGameMode->SetEnableClickEvents(true);
	//}

	//auto* DocSome = UB2UIDocHelper::GetDocSome();
	//auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	//if (DocSome && DocBattle)
	//{
	//	// CharacterObserve UI 를 열기 전에 이게 세팅이 되어 있어야 함.
	//	// 기존의 서브 클래스와 설정하려는 메인 클래스가 같다면 메인->서브
	//	if (DocBattle->GetTagPCClass() == PCClassToInt(CurrentPCClass))
	//	{
	//		DocBattle->SetTagPCClass(DocBattle->GetSavePCClass());
	//	}

	//	DocBattle->SetCurPCClass(PCClassToInt(CurrentPCClass));
	//	DocSome->SetIsCharacterObserveScene(true);
	//	
	//	// 180104_YJ HeroMgmt를 입장할때 기본값으로 보여지는 캐릭터는 로비 옵저버 캐릭터이므로, 
	//	// 옵저버 캐릭터가 변경될때 함께 변경시킴
	//	if (CahsedSceneManager)
	//		CahsedSceneManager->SetHeroMgmtModePCClass(CurrentPCClass);
	//}
	//
	//// 모두 제거하기로 함.
	//DestroyAllLobbyCharOnHeadDisplayClass<>::GetInstance().Signal();
	//// 현재 클로즈업한 캐릭터 정보만 띄우는 게 다시 필요해지면 FLobbySceneManager::UpdateLobbyOnHeadDisplayWidget 을 사용. UB2FloatingLobbyOnHeadDisplay 쪽에서도 숨긴 거 복구


	////블루프린트에서 세팅하는 것으로 변경예정 - LVCH_Lobby 체크아웃문제로 하드코딩..
	//if (LobbyGameMode)
	//{
	//	//180220_YJ 계속 배열이 쌓이던 현상이 있었음.
	//	LobbyObserveCamera.IgnoreHiddenActorsBetweenCameraAndActor.Empty();

	//	for (TActorIterator<AStaticMeshActor> It(LobbyGameMode->GetWorld()); It; ++It)
	//	{
	//		auto* Actor = (*It);
	//		if (Actor && Actor->GetName().Find(TEXT("BoossFoor")) != INDEX_NONE)
	//			LobbyObserveCamera.IgnoreHiddenActorsBetweenCameraAndActor.Add(Actor);
	//	}
	//	for (TActorIterator<ABlockingVolume> It(LobbyGameMode->GetWorld()); It; ++It)
	//	{
	//		LobbyObserveCamera.IgnoreHiddenActorsBetweenCameraAndActor.Add(*It);
	//	}
	//}

	//UB2UILobbyMain* LobbyMainUIPage = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	//if (LobbyMainUIPage)
	//{
	//	LobbyMainUIPage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//}

	//auto* DocCondition = UB2UIDocHelper::GetDocUICondition();
	//if (DocCondition)
	//	DocCondition->SetHideAllLobbyMainUIs(false);

	//LobbyObserveCamera.ActivateObserveCamera(true);

	//FLobbySceneManager* LobbySM = GetSceneManager();
	//if (LobbySM)
	//{
	//	LobbySM->SetPointLightsOfLobbyMain(true);

	//	// 페어리 보이도록.
	//	EFairyType ViewFairyType = FindFlyFairyTypeByOpenStage();
	//	if(ViewFairyType != EFairyType::FAIRY_NONE || ViewFairyType != EFairyType::FAIRY_MAX)
	//		LobbySM->OnLobbyFlyFairy_Visible(ViewFairyType);
	//}
}

EFairyType FLobbyCharObserveScene::FindFlyFairyTypeByOpenStage()
{
	// 현재 오픈되어있는 페어리 중에 랜덤으로 하나 선택.
	EFairyType ViewFairyType = EFairyType::FAIRY_NONE; // NONE 이라면 아무것도 페어리가 오픈되어있지 않음.

	int32 FairyOpenCount = 0;
	FB2FairyStatusInfo StatusInfo;
	for (int32 i = CliToSvrFairyType(EFairyType::FAIRY_NIECE); i < CliToSvrFairyType(EFairyType::FAIRY_MAX); ++i)
	{
		if (FairyManager::GetInstance().GetFairyInfo(SvrToCliFairyType(i), StatusInfo))
		{
			if (StatusInfo.bClearStage)
				FairyOpenCount += 1;
		}
	}
	if (FairyOpenCount != 0)
		ViewFairyType = static_cast<EFairyType>(FMath::RandRange(1, FairyOpenCount));

	return ViewFairyType;
}

void FLobbyCharObserveScene::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto& LobbyCharactersForBLobbyType = GetLobbyCharactersForBTypeLevel();

	//for (TMap<EPCClass, FLobbyCharacter>::TIterator It(LobbyCharactersForBLobbyType); It; ++It)
	//{
	//	auto LobbyCharacter = It.Value();
	//	
	//	if (LobbyCharacter.LevelActor)
	//		LobbyCharacter.LevelActor->SetCloseUpState(false);

	//	if (CurrentPCClass != It.Key())
	//	{
	//		CharacterHiddenAndStopSound(LobbyCharacter.LevelActor, false);
	//		continue;
	//	}		
	//	
	//	if (LobbyCharacter.SelectingMatinee)
	//	{
	//		if (LobbyCharacter.SelectingMatinee->bIsPlaying) {
	//			LobbyCharacter.SelectingMatinee->Stop();
	//		}
	//		auto* PlayerController = UGameplayStatics::GetLocalPlayerController(LobbyCharacter.SelectingMatinee);
	//		if (PlayerController && PlayerController->PlayerCameraManager)
	//			PlayerController->PlayerCameraManager->UnlockFOV();
	//	}
	//}

	//auto* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->BindTouchInput(false);
	//	LobbyGameMode->SetEnableClickEvents(false);
	//}
	//FLobbySceneManager* LobbySceneManager = GetSceneManager();
	//if (LobbySceneManager)
	//{
	//	LobbySceneManager->UpdateLobbyOnHeadDisplayWidget();
	//}

	//UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	//if (DocSome)
	//{
	//	DocSome->SetIsCharacterObserveScene(false);
	//}

	//LobbyObserveCamera.ActivateObserveCamera(false);
}

void FLobbyCharObserveScene::PlayStartCharMatinee(float InStartMatineeDelay)
{
	if (!GetSceneManager()->IsPlayingBeginAnim())
		return;

	IsPlayStartMatinee = true;
	StartMatineeDelay = InStartMatineeDelay;
	CurrentStartMatineeDelay = 0.0f;


	//static FTimerHandle				TimeToLeaveEnemy;

	//if (AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode())
	//{
	//	LobbyGameMode->GetWorld()->GetTimerManager().ClearTimer(TimeToLeaveEnemy);
	//	LobbyGameMode->GetWorld()->GetTimerManager().SetTimer(
	//		TimeToLeaveEnemy
	//		, this
	//		, &FLobbyCharObserveScene::SubscribeEvents_OnConstruct
	//		, 0.1f
	//		, false
	//		, 2.5f);
	//}
}

void FLobbyCharObserveScene::VerticalScreen(bool bVertical)
{
//	this->GetObserveCamera().SetScreenState(bVertical);
//
//	FVector2D ScreenResolution(1280.f, 720.f);
//	if (auto* ViewportClient = GetViewportClient())
//		ViewportClient->GetViewportSize(ScreenResolution);
//
//	if (bVertical == false)
//	{
//		const float MatineeEndDelta = this->GetObserveCamera().MatineeEndCamDist - this->GetObserveCamera().GetCurrentCamDistance();
//		this->GetObserveCamera().MoveCamera(FRotator::ZeroRotator, MatineeEndDelta, true);
//		Swap(ScreenResolution.X, ScreenResolution.Y);
//	}
//	else
//	{
//		this->GetObserveCamera().MoveCamera(FRotator::ZeroRotator, 0.f, true);
//		Swap(ScreenResolution.X, ScreenResolution.Y);
//	}
//
//	UB2UILobbyMain* LobbyMainUIPage = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
//	if (LobbyMainUIPage)
//		LobbyMainUIPage->SetVisibility(bVertical ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
//
//#if WITH_EDITOR
//	FSystemResolution::RequestResolutionChange(ScreenResolution.X, ScreenResolution.Y, EWindowMode::Windowed);
//#endif
}

void FLobbyCharObserveScene::SubscribeEvents_OnConstruct()
{
	//LobbyCharObvSelectTicket = LobbyCharObvSelectClass<EPCClass>::GetInstance().Subscribe([this](EPCClass SelectedClass) { this->SetPCClass(SelectedClass); });
	//LobbyCharMoveCameraTicket = LobbyCharMoveCameraClass<const FRotator&, float>::GetInstance().Subscribe([this](const FRotator& RotDelta, float CamDistDelta) { this->GetObserveCamera().MoveCamera(RotDelta, CamDistDelta); });
	//LobbyCharRotateCharacterTicket = LobbyCharRotateCharacterClass<float>::GetInstance().Subscribe([this](float Value) { this->GetObserveCamera().RotateCharacterYaw(Value < 0); });
	//LobbyCharCameraViewTargetTicket = LobbyCharCameraViewTargetClass<const FVector2D&>::GetInstance().Subscribe([this](const FVector2D& ScreenPos) {this->GetObserveCamera().UpdateViewLoc(ScreenPos); });
	//LobbyCharPlayMatineeTicket = LobbyCharPlayMatineeClass<float>::GetInstance().Subscribe([this](float InStartMatineePlayTime) {this->PlayStartCharMatinee(InStartMatineePlayTime); });
	//LobbyCharObserveChangeScreenTicket = LobbyCharObserveChangeScreenClass<bool>::GetInstance().Subscribe([this](bool bVerticalScreen) { this->VerticalScreen(bVerticalScreen); });
	//LobbyBlockMoveCameraTicket = LobbyBlockMoveCameraClass<bool>::GetInstance().Subscribe([this](bool bBlockCamera) { this->GetObserveCamera().BlockMoveCamera(bBlockCamera); });
	//LobbyCharObvForcedChangeTicket = LobbyCharObvForcedChangeClass<EPCClass>::GetInstance().Subscribe([this](EPCClass SelectedClass) { this->ForcedViewCharacterChange(SelectedClass); });
}
void FLobbyCharObserveScene::UnsubscribeEvents_OnDestruct()
{
	//LobbyCharObvSelectClass<EPCClass>::GetInstance().Unsubscribe(LobbyCharObvSelectTicket);
	//LobbyCharMoveCameraClass<const FRotator&, float>::GetInstance().Unsubscribe(LobbyCharMoveCameraTicket);
	//LobbyCharRotateCharacterClass<float>::GetInstance().Unsubscribe(LobbyCharRotateCharacterTicket);
	//LobbyCharCameraViewTargetClass<const FVector2D&>::GetInstance().Unsubscribe(LobbyCharCameraViewTargetTicket);
	//LobbyCharObserveChangeScreenClass<bool>::GetInstance().Unsubscribe(LobbyCharObserveChangeScreenTicket);
	//LobbyBlockMoveCameraClass<bool>::GetInstance().Unsubscribe(LobbyBlockMoveCameraTicket);
	//LobbyCharPlayMatineeClass<float>::GetInstance().Unsubscribe(LobbyCharPlayMatineeTicket);
	//LobbyCharObvForcedChangeClass<EPCClass>::GetInstance().Unsubscribe(LobbyCharObvForcedChangeTicket);
}

void FLobbyCharObserveScene::ConditionalForceRecacheResource()
{
	//bool bHasInvalidResouceRef = false;
	//// 리소스가 캐싱되지 않은 경우에 대해 강제로 리캐싱 시그널을 날림.. 가끔 문제가 보여서..
	//TMap<EPCClass, FLobbyCharacter>& LobbyCharactersForBLobbyType = GetLobbyCharactersForBTypeLevel();
	//for (TMap<EPCClass, FLobbyCharacter>::TIterator It(LobbyCharactersForBLobbyType); It; ++It)
	//{
	//	FLobbyCharacter& LobbyCharacter = It.Value();
	//	if (!LobbyCharacter.LevelActor || !LobbyCharacter.SelectingMatinee) {
	//		bHasInvalidResouceRef = true;
	//	}
	//}
	//if (bHasInvalidResouceRef)
	//{
	//	AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
	//	if (LobbyGameMode)
	//	{
	//		LobbyGameMode->UpdateNativeLobbyMatineeRef();
	//		UE_LOG(LogBladeII, Warning, TEXT("Call UpdateNativeLobbyMatineeRef from FLobbyCharObserveScene to force re-cache lobby resources.. This is a workaround for a bug, not a solid solution."));
	//	}
	//}
}

void FLobbyCharObserveScene::SetPCClass(EPCClass SelectedClass)
{
	CurrentPCClass = SelectedClass;
}

void FLobbyCharObserveScene::ForcedViewCharacterChange(EPCClass SelectedClass)
{
	//SetPCClass(SelectedClass);

	//auto& LobbyCharactersForBLobbyType = GetLobbyCharactersForBTypeLevel();
	//for (TMap<EPCClass, FLobbyCharacter>::TIterator It(LobbyCharactersForBLobbyType); It; ++It)
	//{
	//	auto LobbyCharacter = It.Value();

	//	if (LobbyCharacter.LevelActor)
	//	{
	//		bool bIsCurrentClass = (CurrentPCClass == It.Key());

	//		CharacterHiddenAndStopSound(LobbyCharacter.LevelActor, !bIsCurrentClass);

	//		if (bIsCurrentClass)
	//			LobbyCharacter.LevelActor->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//	}
	//}

	//PlaySelectMatinee();

	//auto* DocSome = UB2UIDocHelper::GetDocSome();
	//auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	//if (DocSome && DocBattle)
	//{
	//	// CharacterObserve UI 를 열기 전에 이게 세팅이 되어 있어야 함.
	//	DocBattle->SetCurPCClass(PCClassToInt(CurrentPCClass));
	//	DocSome->SetIsCharacterObserveScene(true);

	//	// 180104_YJ HeroMgmt를 입장할때 기본값으로 보여지는 캐릭터는 로비 옵저버 캐릭터이므로, 
	//	// 옵저버 캐릭터가 변경될때 함께 변경시킴
	//	if (CahsedSceneManager)
	//		CahsedSceneManager->SetHeroMgmtModePCClass(CurrentPCClass);
	//}
}

void FLobbyCharObserveScene::PlaySelectMatinee(bool IsBegin, bool bPromptToClosedUpView)
{		
	LobbyObserveCamera.SetData(CurrentPCClass, IsBegin, bPromptToClosedUpView);
}

void FLobbyCharObserveScene::Tick(float DeltaSeconds)
{
	LobbyObserveCamera.Tick(DeltaSeconds);

	if (IsPlayStartMatinee)
	{
		if (CurrentStartMatineeDelay >= StartMatineeDelay)
		{
			PlaySelectMatinee(true);

			CurrentStartMatineeDelay = 0.0f;
			IsPlayStartMatinee = false;
		}
		else
		{
			CurrentStartMatineeDelay += DeltaSeconds;
		}
	}
}

void FLobbyCharObserveScene::FLobbyCharacterObserveCamera::SetScreenState(bool bToSet)
{
	bVerticalScreen = bToSet;
/*
	if (CachedViewTargetActor)
	{
		NearFOV = bVerticalScreen ? CachedViewTargetActor->VerticalCameraProperty.ZoomCamTargetNearFOV : CachedViewTargetActor->HorizontalCameraProperty.ZoomCamTargetNearFOV;
		FarFOV = bVerticalScreen ? CachedViewTargetActor->VerticalCameraProperty .ZoomCamTargetFarFOV : CachedViewTargetActor->HorizontalCameraProperty.ZoomCamTargetFarFOV;

		FarLimitDistance = bVerticalScreen ? CachedViewTargetActor->VerticalCameraProperty.ZoomCamFarLimitDistance : CachedViewTargetActor->HorizontalCameraProperty.ZoomCamFarLimitDistance;
		NearLimitDistance = bVerticalScreen ? CachedViewTargetActor->VerticalCameraProperty.ZoomCamNearLimitDistance : CachedViewTargetActor->HorizontalCameraProperty.ZoomCamNearLimitDistance;

		FarUnderCamLimitRotPitch = bVerticalScreen ? CachedViewTargetActor->VerticalCameraProperty.ZoomCamFarUnderCamLimitRotPitch : CachedViewTargetActor->HorizontalCameraProperty.ZoomCamFarUnderCamLimitRotPitch;
		NearUnderCamLimitRotPitch = bVerticalScreen ? CachedViewTargetActor->VerticalCameraProperty.ZoomCamNearUnderCamLimitRotPitch : CachedViewTargetActor->HorizontalCameraProperty.ZoomCamNearUnderCamLimitRotPitch;

		AddtionalOffsetZ = bVerticalScreen ? CachedViewTargetActor->VerticalCameraProperty.AddtionalZOffset : CachedViewTargetActor->HorizontalCameraProperty.AddtionalZOffset;
	}*/	
}

float FLobbyCharObserveScene::FLobbyCharacterObserveCamera::GetCurrentCamDistance()
{
	/*if (auto* LocalController = UGameplayStatics::GetLocalPlayerController(CachedEntryMatinee))
	{
		if(APlayerCameraManager* PlayerCameraManager = LocalController->PlayerCameraManager)
		{
			FVector FinalTargetVector = CachedViewTargetLocation + FVector(0, 0, GetViewLocZ(DestViewTargetScreenPos));
			return FVector(PlayerCameraManager->GetCameraLocation() - FinalTargetVector).Size();
		}
	}*/

	return 0.f;
}

void FLobbyCharObserveScene::FLobbyCharacterObserveCamera::UpdateViewLoc(const FVector2D& ScreenPos)
{
	DestViewTargetScreenPos = ScreenPos;	

	SetScreenState(bVerticalScreen);
}

float FLobbyCharObserveScene::FLobbyCharacterObserveCamera::GetViewLocZ(const FVector2D& ScreenPos)
{
	float rtnValue = 0.f;

	ULocalPlayer* const LP = (Scene && Scene->GetLocalPlayerController()) ? Scene->GetLocalPlayerController()->GetLocalPlayer() : nullptr;
	if (LP && LP->ViewportClient)
	{
		// get the projection data
		FSceneViewProjectionData ProjectionData;
		if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
		{
			auto InvViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix().Inverse();
			FVector WorldLoc, WorldDir;
			FSceneView::DeprojectScreenToWorld(ScreenPos, ProjectionData.GetConstrainedViewRect(), InvViewProjectionMatrix, WorldLoc, WorldDir);

			FVector InvCamDirPlaneNormalOrthXY(FVector2D(-WorldDir), 0);
			rtnValue = (FMath::LinePlaneIntersection(WorldLoc, WorldLoc + 1000.f * WorldDir, CachedViewTargetLocation, InvCamDirPlaneNormalOrthXY.GetSafeNormal())).Z - CachedViewTargetLocation.Z;

			//if (CachedViewTargetActor)
			//	rtnValue = FMath::Clamp(rtnValue, 
			//	-CachedViewTargetActor->GetSimpleCollisionHalfHeight() * CachedViewTargetActor->ZoomCamTargetHeightOffset, 
			//	CachedViewTargetActor->GetSimpleCollisionHalfHeight()* CachedViewTargetActor->ZoomCamTargetHeightOffset);
		}
	}

	return rtnValue;
}

void FLobbyCharObserveScene::FLobbyCharacterObserveCamera::MoveCamera(const FRotator& RotDelta, float CamDistDelta, bool bForceUpdate)
{
	if (!bMatineeCamFinished || bBlockMoveCamera)
		return;

	//if (RotDelta.IsNearlyZero() && CamDistDelta == 0.f && !bForceUpdate)
	//	return;
	//
	//if (RotDelta.Yaw != 0.f && CachedViewTargetActor)
	//	CachedViewTargetActor->SetPauseLookAt(0.1f);

	//bool bZoomIn = CamDistDelta < 0.f;

	//UE_LOG(LogBladeII, Log, TEXT("RotDelta Pitch : %f, Roll : %f, Yaw : %f "), RotDelta.Pitch, RotDelta.Roll, RotDelta.Yaw);

	//FVector CamLoc;
	//FRotator CamRot;
	//auto* PlayerController = CachedEntryMatinee ? UGameplayStatics::GetLocalPlayerController(CachedEntryMatinee) : NULL;
	//if (PlayerController)
	//{
	//	PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
	//	if (PlayerController->PlayerCameraManager)
	//	{
	//		float CalculatedFOV = DefaultFOV;

	//		auto* CamActor = Cast<ACameraActor>(PlayerController->PlayerCameraManager->ViewTarget.Target);
	//		if (CamActor)
	//		{
	//			float NewCamDist = FMath::Clamp(CachedCamDist + CamDistDelta, NearLimitDistance, FarLimitDistance);
	//			float RealCamDistDelta = NewCamDist - CachedCamDist;
	//			
	//			FRotator NewCamRot = CamRot + RotDelta;

	//			float UnderCamPitch = NearUnderCamLimitRotPitch + (FarUnderCamLimitRotPitch - NearUnderCamLimitRotPitch) * (NewCamDist - NearLimitDistance) / (FarLimitDistance - NearLimitDistance);
	//			NewCamRot.Pitch = FMath::Clamp(NewCamRot.Pitch, -40.f, UnderCamPitch);

	//			float OffsetZ = 0.f;

	//			FVector2D ViewportCenter(0.f, 0.f);
	//			auto* ViewportClient = PlayerController->GetWorld()->GetGameViewport();
	//			if (ViewportClient)
	//			{
	//				ViewportClient->GetViewportSize(ViewportCenter);
	//				if (bZoomIn)
	//				{
	//					//OffsetZ += -RealCamDistDelta * (GetViewLocZ(DestViewTargetScreenPos) - GetViewLocZ(ViewportCenter * 0.5f)) / (CachedCamDist - NearLimitDistance + 10.f);
	//				}
	//			}

	//			CachedCamDist = NewCamDist;

	//			const float TouchOffsetZ = GetViewLocZ(DestViewTargetScreenPos);
	//			FVector CurrCamViewTarget(CachedViewTargetLocation);

	//			if (bZoomIn)
	//			{
	//				float DestPitch = (CachedViewTargetLocation + FVector(0.f, 0.f, TouchOffsetZ) - CamLoc).Rotation().Pitch;
	//				NewCamRot.Pitch += RealCamDistDelta * DestPitch / (CachedCamDist - NearLimitDistance + 10.f);
	//			}			

	//			FVector CamInvDirection = -NewCamRot.Vector();				
	//			FVector CameraLocationToMove = CurrCamViewTarget + (CachedCamDist * CamInvDirection);


	//			const float TouchZDelta = TouchOffsetZ / (CachedCamDist - NearLimitDistance + 10.f);

	//			const FVector AddtionalZ(0.f, 0.f, AddtionalOffsetZ + TouchZDelta);
	//			CameraLocationToMove += AddtionalZ;

	//			HiddenActorsBetweenCameraAndActor.Empty();

	//			FHitResult Hit;
	//			UWorld* World = CamActor->GetWorld();
	//			FCollisionQueryParams Params(TEXT("ActorToCamLineTrace"), true, CachedViewTargetActor);
	//			Params.AddIgnoredActor(CamActor);
	//			
	//			World->LineTraceSingleByChannel(Hit, CurrCamViewTarget, CameraLocationToMove, ECC_Visibility, Params);

	//			float DesireCamDist = CachedCamDist;
	//			FRotator DesireCamRot = NewCamRot;

	//			if (Hit.bBlockingHit && Hit.Actor.IsValid())
	//			{
	//				if (IgnoreHiddenActorsBetweenCameraAndActor.Find(Hit.Actor.Get()) != INDEX_NONE)
	//				{// 이 놈은 바닥이다.
	//					CameraLocationToMove = Hit.Location;

	//					if (!bZoomIn)
	//						DesireCamRot = (CurrCamViewTarget - CameraLocationToMove).ToOrientationRotator();
	//					DesireCamDist = (CameraLocationToMove - CurrCamViewTarget).Size();
	//				}
	//			}

	//			TArray<FOverlapResult> Overlaps;
	//			FCollisionShape CameraBounds;
	//			CameraBounds.SetCapsule(100.f, DesireCamDist * 0.5f);
	//			
	//			Params.AddIgnoredActors(IgnoreHiddenActorsBetweenCameraAndActor);
	//			World->OverlapMultiByChannel(Overlaps, (CameraLocationToMove + CurrCamViewTarget) * 0.5f, (FRotationMatrix(FRotator(90.f, 0.f, 0.f)) * FRotationMatrix(DesireCamRot)).ToQuat(), ECC_Visibility, CameraBounds, Params);
	//			
	//			for (auto& Overlap : Overlaps)
	//			{
	//				HiddenActorsBetweenCameraAndActor.Add(Overlap.GetActor());
	//			}

	//			if (DefaultFOV != 0.f)
	//			{
	//				if (DesireCamDist - DefaultCamDist <= 0.f) // Default보다 안쪽에 Camera가 위치(줌인상태)라면
	//				{
	//					const float DistRatio = (DesireCamDist - NearLimitDistance) / (DefaultCamDist - NearLimitDistance);
	//					CalculatedFOV = FMath::Lerp(NearFOV, DefaultFOV, DistRatio);
	//				}

	//				else
	//				{
	//					const float DistRatio = (FarLimitDistance - DesireCamDist) / (FarLimitDistance - DefaultCamDist);
	//					CalculatedFOV = FMath::Lerp(DefaultFOV, FarFOV, FMath::Clamp(DistRatio, 0.f, 1.f));
	//				}
	//			}

	//			CamActor->SetActorLocationAndRotation(CameraLocationToMove, DesireCamRot);
	//		}

	//		PlayerController->PlayerCameraManager->SetFOV(CalculatedFOV);
	//		PlayerController->PlayerCameraManager->UpdateCamera(0.f);
	//	}		
	//}
}

void FLobbyCharObserveScene::FLobbyCharacterObserveCamera::RotateCharacterYaw(bool bLeft)
{
	if (!bMatineeCamFinished)
		return;

	if (CachedViewTargetActor == NULL)
		return;
#if CHAR_ANIM_ROTATION
	CachedViewTargetActor->PlayTurnAnimation(bLeft);
#endif
}

void FLobbyCharObserveScene::FLobbyCharacterObserveCamera::ActivateObserveCamera(bool bInActivate)
{
	bActivate = bInActivate;
}

void FLobbyCharObserveScene::FLobbyCharacterObserveCamera::BlockMoveCamera(bool bBlock)
{
	bBlockMoveCamera = bBlock;
}

void FLobbyCharObserveScene::FLobbyCharacterObserveCamera::Initialize()
{
	CachedViewTargetActor = NULL;
	CachedEntryMatinee = NULL;

	CachedViewTargetLocation = FVector(0.f);
	DestViewTargetScreenPos = FVector2D::ZeroVector;
	DefaultCamDist = 0.f;
	CachedCamDist = 0.f;
	ElapsedSelectMatineeTime = 0.f;
	MaxSelectMatineeCameraTime = 0.f;
	bMatineeCamFinished = false;
	NearFOV = 0.f;
	FarFOV = 0.f;
	DefaultFOV = 0.f;
	bBlockMoveCamera = false;
}

void FLobbyCharObserveScene::FLobbyCharacterObserveCamera::Tick(float DeltaSeconds)
{
	//if (!bActivate)
	//{
	//	Initialize();
	//}
	//else
	//{
	//	ElapsedSelectMatineeTime += DeltaSeconds;
	//	
	//	if (!bMatineeCamFinished && ElapsedSelectMatineeTime > MaxSelectMatineeCameraTime)
	//	{
	//		bMatineeCamFinished = true;
	//		if (CachedEntryMatinee && !CachedEntryMatinee->bPaused)
	//		{
	//			auto* LocalController = UGameplayStatics::GetLocalPlayerController(CachedEntryMatinee);
	//			if (!LocalController)
	//				return;

	//			APlayerCameraManager* PlayerCameraManager = LocalController->PlayerCameraManager;
	//			if (NearFOV != 0.f || FarFOV != 0.f)
	//			{
	//				DefaultFOV = PlayerCameraManager ? PlayerCameraManager->GetFOVAngle() : 0.f;
	//			}

	//			CachedEntryMatinee->Pause();

	//			MatineeEndCamDist = GetCurrentCamDistance();
	//		}		
	//	}

	//	for (auto* Actor : HiddenActorsBetweenCameraAndActor)
	//	{
	//		TArray<UActorComponent*> ActorComponents;
	//		Actor->GetComponents(ActorComponents);

	//		for (auto* Component : ActorComponents)
	//		{
	//			auto* SceneComp = Cast<UPrimitiveComponent>(Component);
	//			if (SceneComp && SceneComp->bVisible && !SceneComp->bHiddenInGame /*&& SceneComp->DrawInGameCustomDuration >= 0.f*/)
	//			{
	//				SceneComp->PushDrawInGameToProxy(false);
	//			}
	//		}
	//	}		
	//}

}

void FLobbyCharObserveScene::FLobbyCharacterObserveCamera::SetData(EPCClass SelectedClass, bool IsBegin, bool bPromptToClosedUpView)
{
	BII_CHECK(Scene);

	//Initialize();

	//auto& LobbyCharactersForBLobbyType = Scene->GetLobbyCharactersForBTypeLevel();

	//const auto& SelectedCharacterInfo = LobbyCharactersForBLobbyType.FindRef(SelectedClass);
	//
	//if (!SelectedCharacterInfo.GameBeginingMatinee || !SelectedCharacterInfo.GameBeginingMatinee->MatineeData || !SelectedCharacterInfo.LevelActor)
	//	return;

	//ElapsedSelectMatineeTime = 0;
	//CachedViewTargetActor = SelectedCharacterInfo.LevelActor;
	//CachedEntryMatinee = SelectedCharacterInfo.SelectingMatinee;
	//
	//SetScreenState(false); //기본 가로스크린

	//CachedViewTargetActor->SetCloseUpState(true, 
	//	!bPromptToClosedUpView, // CloseUpView 로 바로 직행하는 경우라면 선택 Anim 플레이를 하지 않는다.
	//	IsBegin); // 게임 접속하고 맨 처음.. 이걸 두 번 타게 되니 강제로 다시 state 세팅을 거치도록.

	//BII_CHECK(Scene->GetSceneManager());

	//AMatineeActor* PlayMatinee = IsBegin ? SelectedCharacterInfo.GameBeginingMatinee : SelectedCharacterInfo.SelectingMatinee;
	//PlayCharacterMatinee(PlayMatinee, SelectedCharacterInfo.LevelActor);

	//if (bPromptToClosedUpView)
	//{
	//	// PlayCharacterMatinee 에서는 시작 위치에 놓았을 것. 바로 끝 위치로 감.
	//	if (PlayMatinee && PlayMatinee->MatineeData) {
	//		PlayMatinee->SetPosition(PlayMatinee->MatineeData->InterpLength);
	//	}
	//}

	//Scene->GetSceneManager()->SetPlayingBeginAnim(false);
}

//void FLobbyCharObserveScene::FLobbyCharacterObserveCamera::PlayCharacterMatinee(AMatineeActor* PlayMatinee,AB2LobbySkeletalMeshActor* LevelActor)
//{
//	if (PlayMatinee == nullptr || LevelActor == nullptr)
//		return;
//
//	PlayMatinee->InitInterp();
//	PlayMatinee->SetPosition(0.f);
//	PlayMatinee->Play();
//
//	FVector CamMatineePosition;
//	FVector CamMatineeDirection;
//	FVector ActorMatineePosition;
//
//	for (auto* InterpGroup : PlayMatinee->GroupInst)
//	{
//		AActor* GroupActor = InterpGroup->GetGroupActor();
//		if (GroupActor == NULL)
//			continue;
//
//		if (GroupActor->IsA(ACameraActor::StaticClass()) && InterpGroup->Group)
//		{
//			for (int32 i = 0; i < InterpGroup->Group->InterpTracks.Num(); ++i)
//			{
//				auto* MoveTrack = Cast<UInterpTrackMove>(InterpGroup->Group->InterpTracks[i]);
//				if (MoveTrack)
//				{
//					int32 LastKey = MoveTrack->GetNumKeyframes() - 1;
//					if (LastKey >= 0)
//					{
//						MoveTrack->ExternalGetKeyframePosition(InterpGroup->TrackInst[i], LastKey, MaxSelectMatineeCameraTime, CamMatineePosition);
//						FVector EulerRotation;
//						MoveTrack->ExternalGetKeyframeRotation(InterpGroup->TrackInst[i], LastKey, MaxSelectMatineeCameraTime, EulerRotation);
//
//						CamMatineeDirection = FRotator::MakeFromEuler(EulerRotation).Vector();
//					}
//					continue;
//				}
//			}
//		}
//
//		if (GroupActor == LevelActor)
//		{
//			for (int32 i = 0; i < InterpGroup->Group->InterpTracks.Num(); ++i)
//			{
//				auto* MoveTrack = Cast<UInterpTrackMove>(InterpGroup->Group->InterpTracks[i]);
//				if (MoveTrack)
//				{
//					int32 LastKey = MoveTrack->GetNumKeyframes() - 1;
//					if (LastKey >= 0)
//					{
//						float KeyframeTime;
//						MoveTrack->ExternalGetKeyframePosition(InterpGroup->TrackInst[i], LastKey, KeyframeTime, ActorMatineePosition);
//					}
//					continue;
//				}
//			}
//		}
//	}
//
//	FVector InvCamDirPlaneNormalOrthXY(FVector2D(-CamMatineeDirection), 0);
//
//	CachedViewTargetLocation = FMath::LinePlaneIntersection(CamMatineePosition, CamMatineePosition + 1000.f * CamMatineeDirection, ActorMatineePosition, InvCamDirPlaneNormalOrthXY.GetSafeNormal());
//
//	DefaultCamDist = CachedCamDist = (CachedViewTargetLocation - CamMatineePosition).Size();
//
//	Scene->SetCameraMatineeCam(PlayMatinee);
//}

//=================================================================================================
//
//	Hero Management
//
//=================================================================================================
FLobbyHeroMgmtScene::FLobbyHeroMgmtScene(class FLobbySceneManager* OwnerSceneManager) : FLobbySceneBase(OwnerSceneManager)
{
	//CurrentPCClass = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
	bCurrentlyWingEvolutionScene = false;
	WingEvolutionScenePCClass = EPCClass::EPC_End;

}

FLobbyHeroMgmtScene::~FLobbyHeroMgmtScene()
{

}

void FLobbyHeroMgmtScene::OpenScene()
{
	FLobbySceneBase::OpenScene();

	//auto* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->BindTouchInput(true);
	//	LobbyGameMode->SetEnableClickEvents(true);
	//}
	//
	//
	//if (GEngine->ActiveMatinee.IsValid() && GEngine->ActiveMatinee.Get()->GetWorld())//마티네가 꼬이고있습니다... 돌고있는 마티네 무조건 멈춰주고 돌려줍시다.
	//	GEngine->ActiveMatinee.Get()->Stop();

	SelectCharacterHeroMgmt(CurrentPCClass);
}

void FLobbyHeroMgmtScene::CloseScene()
{
	FLobbySceneBase::CloseScene();
//
//	auto& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();
//
//	auto* LobbyCharInfo = AllCharactersOfDesiredType.Find(CurrentPCClass);
//	if (LobbyCharInfo)
//	{
//		LobbyCharInfo->MatineeForInven->Stop();
//		if (LobbyCharInfo->MatineeForWingEventScene){
//			LobbyCharInfo->MatineeForWingEventScene->Stop();
//		}
//		LobbyCharInfo->ActorForMain->SetActorRotation(LobbyCharInfo->InitialRotation);
//	}
//
///*
//	if (UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome())
//	{
//		if (!bCurrentlyWingEvolutionScene)
//			CurrentPCClass = DocSome->GetLobbyCharObserverClass();
//	}*/

	ZoomModule.ClearZoomModule();
}

void FLobbyHeroMgmtScene::SubscribeEvents_OnConstruct()
{
	//LobbyHeroMgmtSelectTicket = LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Subscribe([this](EPCClass SelectedClass) { this->SelectCharacterHeroMgmt(SelectedClass); });
	//LobbyCharRotateCharacterTicket = LobbyCharRotateCharacterClass<float>::GetInstance().Subscribe([this](float Value) { this->RotateCharacterYaw(Value); });
	//LobbyCharEquippedItemTicket = LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Subscribe([this](EPCClass CharClass, EItemEquipPlace EquippedPlace) {this->PlayEquippedAnimation(CharClass, EquippedPlace); });
	//BeginWingEvolutionSceneTicket = BeginWingEvolutionSceneClass<EPCClass>::GetInstance().Subscribe([this](EPCClass EvolvedClass) { this->BeginWingEvolutionScene(EvolvedClass); });
	//EndWingEvolutionSceneTicket = EndWingEvolutionSceneClass<>::GetInstance().Subscribe([this]() { this->EndWingEvolutionScene(); });
	//LobbyCharZoomTicket = LobbyCharZoomClass<const FVector2D&, float>::GetInstance().Subscribe([this](const FVector2D& Pinpoint, float Delta) { this->ZoomModule.Zoom(Pinpoint, Delta); });
}
void FLobbyHeroMgmtScene::UnsubscribeEvents_OnDestruct()
{
	//LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Unsubscribe(LobbyHeroMgmtSelectTicket);
	//LobbyCharRotateCharacterClass<float>::GetInstance().Unsubscribe(LobbyCharRotateCharacterTicket);
	//LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Unsubscribe(LobbyCharEquippedItemTicket);
	//BeginWingEvolutionSceneClass<EPCClass>::GetInstance().Unsubscribe(BeginWingEvolutionSceneTicket);
	//EndWingEvolutionSceneClass<>::GetInstance().Unsubscribe(EndWingEvolutionSceneTicket);
	//LobbyCharZoomClass<const FVector2D&, float>::GetInstance().Unsubscribe(LobbyCharZoomTicket);
}

void FLobbyHeroMgmtScene::OnLobbyGMActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject)
{
	//AMatineeActor* CastedNotifyingMatinee = Cast<AMatineeActor>(OptionalNotifyingObject);

	//// 날개진화연출에서 MatineeForWingEventScene 의 Stop notification 인 경우 연출 종료 처리.
	//// MATINEE_DONE_STAY_ACTOR_CUSTOM_EVENT 를 받아야 할 수도 있다..
	//if (bCurrentlyWingEvolutionScene && OptionalEventName == MATINEE_STOP_ACTOR_CUSTOM_EVENT && CastedNotifyingMatinee && CastedNotifyingMatinee->IsValidObj())
	//{
	//	TMap<EPCClass, FLobbyCharacterInfo>& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();
	//	FLobbyCharacterInfo* EvolvedCharInfo = AllCharactersOfDesiredType.Find(WingEvolutionScenePCClass);
	//	if (EvolvedCharInfo && EvolvedCharInfo->MatineeForWingEventScene == CastedNotifyingMatinee)
	//	{
	//		// 바로 끝내지 않고 결과 UI 를 보여준 다음 EndWingEvolutionScene 는 결과 UI 에서 확인버튼 누르면 불리도록.
	//		OnWingEvolutionMatineeEnd();
	//	}
	//}
}

EUIScene FLobbyHeroMgmtScene::GetUIScene()
{
	AB2LobbyGameMode* LobbyGM = GetSceneManager() ? GetSceneManager()->GetLobbyGameMode() : NULL;
	if (LobbyGM)
	{
		//return DJLegacyHeroMgmtPageToUIScene(LobbyGM->GetHeroMgmtSubMode());
	}
	return EUIScene::Inventory;
}

void FLobbyHeroMgmtScene::SelectCharacterHeroMgmt(EPCClass SelectedCharacter)
{
	//if (!(this->GetSceneManager() && this->GetSceneManager()->GetCurrentLobbyScene() == this->GetLobbyScene()))
	//{
	//	CurrentPCClass = SelectedCharacter;
	//	return;
	//}

	//auto& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();

	//auto* LobbyCharInfo = AllCharactersOfDesiredType.Find(CurrentPCClass);
	//if (LobbyCharInfo)
	//{
	//	LobbyCharInfo->MatineeForInven->Stop();
	//	if (LobbyCharInfo->MatineeForWingEventScene){
	//		LobbyCharInfo->MatineeForWingEventScene->Stop();
	//	}
	//	LobbyCharInfo->ActorForMain->SetActorRelativeRotation(LobbyCharInfo->InitialRotation);
	//}

	//CurrentPCClass = SelectedCharacter;
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(AllCharactersOfDesiredType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//		CharacterHiddenAndStopSound(It.Value().ActorForMain, true);
	//	if (It.Value().ActorForSub)
	//		CharacterHiddenAndStopSound(It.Value().ActorForSub, true);
	//}

	//LobbyCharInfo = AllCharactersOfDesiredType.Find(CurrentPCClass);
	//if (LobbyCharInfo)
	//{
	//	CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, false);

	//	if (LobbyCharInfo->ActorForMain && !LobbyCharInfo->ActorForMain->bHidden)
	//	{
	//		//캐릭터의 위치를 맞추기 위함.
	//		if (!LobbyCharInfo->CachedInitLocation.IsNearlyZero())				
	//			LobbyCharInfo->ActorForMain->SetActorLocation(LobbyCharInfo->CachedInitLocation);

	//		LobbyCharInfo->ActorForMain->PlaySelectAnimation(ELobbyScene::ELobbyScene_HeroManagement);
	//	}

	//	float EventKeyTime = ABladeIIGameMode::GetMatineePositionByEventName(LobbyCharInfo->MatineeForInven, TEXT("Inven"));

	//	LobbyCharInfo->MatineeForInven->Play();		
	//	SetCameraMatineeCam(LobbyCharInfo->MatineeForInven, EventKeyTime);
	//	LobbyCharInfo->MatineeForInven->SetPosition(EventKeyTime);
	//	LobbyCharInfo->MatineeForInven->Pause();

	//	ZoomModule.InitializeModule(this);
	//	ZoomModule.ChangeZoomTarget(LobbyCharInfo->ActorForMain);
	//}
}

void FLobbyHeroMgmtScene::RotateCharacterYaw(float Value)
{
	auto& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();
//
//	auto* LobbyCharInfo = AllCharactersOfDesiredType.Find(CurrentPCClass);
//	if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
//	{
//#if CHAR_ANIM_ROTATION
//		LobbyCharInfo->ActorForMain->PlayTurnAnimation(Value < 0.f);
//#elif CHAR_ACTOR_ROTATION
//		if (!LobbyCharInfo->ActorForMain->IsPlayingSelectedAnim())
//		{
//			FRotator Rotation = LobbyCharInfo->ActorForMain->GetActorRotation();
//			Rotation += FRotator(0.f, Value * 2.f, 0.f);
//			LobbyCharInfo->ActorForMain->SetActorRotation(Rotation);
//		}
//#endif
//	}
}

void  FLobbyHeroMgmtScene::PlayEquippedAnimation(EPCClass CharClass, EItemEquipPlace EquippedPlace)
{
	if (CharClass != CurrentPCClass)
		return;

	//auto& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();

	//auto* LobbyCharInfo = AllCharactersOfDesiredType.Find(CurrentPCClass);
	//if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
	//{
	//	LobbyCharInfo->ActorForMain->PlayEquipAnimation(EquippedPlace);
	//}
}

void FLobbyHeroMgmtScene::BeginWingEvolutionScene(EPCClass InEvolvedPCClass)
{
	//if (InEvolvedPCClass != EPCClass::EPC_End){
	//	WingEvolutionScenePCClass = InEvolvedPCClass; // 웬만한 진화 상황에서라면 CurrentPCClass 와 같아야 한다. 단, 혹시라도 네트웍 딜레이로 인해 응답이 늦고 그 새 캐릭터가 바뀔 가능성을 생각해서..
	//}
	//else{
	//	WingEvolutionScenePCClass = CurrentPCClass;
	//}

	//if (WingEvolutionScenePCClass != EPCClass::EPC_End)
	//{		
	//	bCurrentlyWingEvolutionScene = true;
	//	
	//	if (GetSceneManager())
	//	{ // 사용자가 날개를 숨겼더라도 여기선 보여주자.
	//		GetSceneManager()->SetUnconditionalShowWing(true, true, WingEvolutionScenePCClass);
	//	}

	//	// 진화 연출 처음에는 날개가 숨겨져 있는데 (애니메이션 자체가 그럼) 순간적으로 날개가 보이는 게 튀어서.. 혹시 이렇게라도 하면 조금이나마 도움이 될까 싶음
	//	FlushRenderingCommands(); // 근데 이거랑 상관없는 듯..

	//	TMap<EPCClass, FLobbyCharacterInfo>& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();
	//	
	//	for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(AllCharactersOfDesiredType); It; ++It)
	//	{ // 일단 다 숨겨놓고
	//		if (It.Value().ActorForMain){
	//			CharacterHiddenAndStopSound(It.Value().ActorForMain, true);
	//		}
	//		if (It.Value().ActorForSub){
	//			CharacterHiddenAndStopSound(It.Value().ActorForSub, true);
	//		}
	//	}

	//	FLobbyCharacterInfo* EvolvedCharInfo = AllCharactersOfDesiredType.Find(WingEvolutionScenePCClass);
	//	if (EvolvedCharInfo)
	//	{
	//		if (EvolvedCharInfo->ActorForMain)
	//		{
	//			CharacterHiddenAndStopSound(EvolvedCharInfo->ActorForMain, false);
	//			if (EvolvedCharInfo->ActorForMain->GetSkeletalMeshComponent()){
	//				// 여긴 애니메이션을 마티니 플레이로 구동시킬 것이므로 잠시 바꿔둠..
	//				EvolvedCharInfo->ActorForMain->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//			}
	//		}

	//		if (EvolvedCharInfo->MatineeForInven){
	//			EvolvedCharInfo->MatineeForInven->Stop(); // 영웅관리 메인 마티니와 겹치지 않게 정지시킴.
	//		}
	//		if (EvolvedCharInfo->MatineeForWingEventScene){
	//			EvolvedCharInfo->MatineeForWingEventScene->Stop();

	//			EvolvedCharInfo->MatineeForWingEventScene->SetPosition(0.0f);
	//			EvolvedCharInfo->MatineeForWingEventScene->AddCustomNotifyActor(GetLobbyGameMode()); // Matinee Stop Notification 을 받기 위해 GameMode 를 등록해 넣음.
	//			EvolvedCharInfo->MatineeForWingEventScene->Play();
	//		}
	//	}

	//	// 연출 용 UI 페이지로. 그다지 보여주는 거 없는 거의 빈 화면일 듯..
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_WingEvolutionProgress);
	//}
}

void FLobbyHeroMgmtScene::OnWingEvolutionMatineeEnd()
{
	//// 결과 UI 보여주고..
	//DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_WingEvolutionResult);

	//// 아마도 마티니 마지막 카메라 장면에서 Idle 플레이를 요구할 거 같은데.. 그게 어려우면 평소 영웅관리 화면에서의 카메라라도..

	//TMap<EPCClass, FLobbyCharacterInfo>& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();
	//FLobbyCharacterInfo* EvolvedCharInfo = AllCharactersOfDesiredType.Find(WingEvolutionScenePCClass);
	//if (EvolvedCharInfo)
	//{
	//	if (EvolvedCharInfo->MatineeForWingEventScene)
	//	{
	//		// MATINEE_STOP_ACTOR_CUSTOM_EVENT 를 받고 결과 UI 가 나오는 상황이라면 여기서 Matinee Stop 을 해서는 안되고
	//		// MATINEE_DONE_STAY_ACTOR_CUSTOM_EVENT 를 받고 결과 UI 가 나오는 상황이라면 여기서 Matinee Stop 을 한다.
	//		//EvolvedCharInfo->MatineeForWingEventScene->Stop();
	//		EvolvedCharInfo->MatineeForWingEventScene->SetPosition(0.0f);

	//		// 날개 연출 끝난 바로 그 카메라 시점으로..
	//		const float FinalKeyTime = EvolvedCharInfo->MatineeForWingEventScene->MatineeData ? // MatineeData 가 없으면 말이 안되지만..
	//			EvolvedCharInfo->MatineeForWingEventScene->MatineeData->InterpLength : 0.0f;
	//		EvolvedCharInfo->MatineeForWingEventScene->Play();
	//		SetCameraMatineeCam(EvolvedCharInfo->MatineeForWingEventScene, FinalKeyTime);
	//		EvolvedCharInfo->MatineeForWingEventScene->SetPosition(FinalKeyTime);
	//	}
	//}
	//// 카메라는 끝난 시점에.. 애니메이션은 따로 Idle
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(AllCharactersOfDesiredType); It; ++It)
	//{
	//	AB2HeroMgntSkeletalMeshActor* ThisSKActor = It.Value().ActorForMain;
	//	if (ThisSKActor && ThisSKActor->GetSkeletalMeshComponent()){
	//		// 혹 캐릭터 선택이 어긋났을지도 모르니 모두 AnimBP 모드로 확실하게 바꿔줌.
	//		ThisSKActor->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//	}
	//}
}

void FLobbyHeroMgmtScene::EndWingEvolutionScene()
{
//	if (GetSceneManager())
//	{ // 사용자가 날개를 숨겼다면 다시 숨긴다.
//		GetSceneManager()->SetUnconditionalShowWing(false, true, WingEvolutionScenePCClass);
//	}
//
////	CurrentPCClass = WingEvolutionScenePCClass;
//	WingEvolutionScenePCClass = EPCClass::EPC_End;
//	bCurrentlyWingEvolutionScene = false;
//	
//	AB2LobbyGameMode* LobbyGM = GetLobbyGameMode();
//	UB2Airport* WingAirport = LobbyGM ? LobbyGM->GetLobbyAirport() : NULL;
//	if (WingAirport)
//	{
//		WingAirport->MarkPreEvolveDataInvalid(); // 진화 결과창에서 써먹은 이후에는 필요없다.
//	}
//
//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_Wing);

	OpenScene();
}


//=================================================================================================
//
//	Stage Info
//
//=================================================================================================
FLobbyStageInfoScene::FLobbyStageInfoScene(class FLobbySceneManager* OwnerSceneManager) : FLobbySceneBase(OwnerSceneManager)
{
	
}

FLobbyStageInfoScene::~FLobbyStageInfoScene()
{
	
}

void FLobbyStageInfoScene::OpenScene()
{
	FLobbySceneBase::OpenScene();

	//SelectCharacterStageInfo(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());

	//if (GEngine->ActiveMatinee.IsValid() && GEngine->ActiveMatinee.Get()->GetWorld())//마티네가 꼬이고있습니다... 돌고있는 마티네 무조건 멈춰주고 돌려줍시다.
	//	GEngine->ActiveMatinee.Get()->Stop();
}

void FLobbyStageInfoScene::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//	{
	//		//영웅관리에서 쓰이는 Actor라서 초기 위치로 돌아가야만 한다.
	//		It.Value().ActorForMain->SetActorLocation(It.Value().CachedInitLocation);
	//	}
	//}
}

void FLobbyStageInfoScene::SubscribeEvents_OnConstruct()
{
	//LobbyStageInfoSelectTicket = LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Subscribe([this](EPCClass SelectedMain, EPCClass SelectedSub) { this->SelectCharacterStageInfo(SelectedMain, SelectedSub); });
}
void FLobbyStageInfoScene::UnsubscribeEvents_OnDestruct()
{
	//LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Unsubscribe(LobbyStageInfoSelectTicket);
}

void FLobbyStageInfoScene::SelectCharacterStageInfo(EPCClass MainClass, EPCClass SubClass)
{
	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//CurrentMainClass = MainClass;
	//CurrentSubClass = SubClass;

	//if (!(this->GetSceneManager() && this->GetSceneManager()->GetCurrentLobbyScene() == this->GetLobbyScene()))
	//{
	//	return;
	//}

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//		CharacterHiddenAndStopSound(It.Value().ActorForMain, true);
	//	if (It.Value().ActorForSub)
	//		CharacterHiddenAndStopSound(It.Value().ActorForSub, true);
	//}
	//if (CurrentMainClass != EPCClass::EPC_End)
	//{
	//	auto* LobbyCharInfo = LobbyCharactersForALobbyType.Find(CurrentMainClass);
	//	if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
	//	{
	//		CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, false);
	//		LobbyCharInfo->ActorForMain->PlaySelectAnimation(ELobbyScene::ELobbyScene_StageInfo, true, true);
	//	}
	//}

	//if (CurrentSubClass != EPCClass::EPC_End)
	//{
	//	auto* LobbySubCharInfo = LobbyCharactersForALobbyType.Find(CurrentSubClass);
	//	if (LobbySubCharInfo && LobbySubCharInfo->ActorForSub)
	//	{
	//		CharacterHiddenAndStopSound(LobbySubCharInfo->ActorForSub, false);
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_StageInfo, false, true);// Sub 캐릭터의 Sound notify 를 끈다.
	//	}
	//}

	//if (GEngine && GLog)
	//{
	//	GEngine->HandleCeCommand(GetLobbyGameMode()->GetWorld(), TEXT("SetDefaultCamActorForCharacterSelect"), *GLog);
	//}

	//DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();
	//	
	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}
}

//=================================================================================================
//
//	BattleSelectSceneBase - base for Chapter and Mod scene
//
//=================================================================================================

FLobbyBattleSelectSceneBase::FLobbyBattleSelectSceneBase(FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
	, MatineePos(0.0f), SpeedSwipe(0.f), FreeSwipeStartingSpeed(0.0f)
	, bMoveCameraDirectCalledThisFrame(false)
	, bAcceleratingSwipe(false), bBeginAcceleratingSwipe(false), AcceleratingSwipeInitialSpeed(0.0f)
{
	SwipeAcceleration = 0.03f;
	MatineeLength = 0.f;
	SpeedAutoScrolling = 0.f;
	ElapsedLastInput = 0.f;
	MatineeAutoScrollLeftEndPosition = MatineeAutoScrollRightEndPosition = 0.f;
	bAutoMoveDirection = false;
}

FLobbyBattleSelectSceneBase::~FLobbyBattleSelectSceneBase()
{
	
}

void FLobbyBattleSelectSceneBase::OpenScene()
{
	FLobbySceneBase::OpenScene();

	//AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->BindTouchInput(true);
	//}

	//AMatineeActor* MainViewMatinee = GetChapterMatinee();
	//
	//if (MainViewMatinee && MainViewMatinee->IsValidObj()) // 레벨 스트리밍과 얽혀서 Invalid 한 경우가 있는 거 같다..
	//{
	//	MatineePos = ABladeIIGameMode::GetMatineePositionByEventName(MainViewMatinee, TEXT("Start"));
	//	
	//	MainViewMatinee->Play();
	//	MainViewMatinee->SetPosition(MatineePos);
	//	MainViewMatinee->Pause();		

	//	MatineeLength = MainViewMatinee->MatineeData->InterpLength;

	//	MatineeAutoScrollLeftEndPosition = FMath::Max(0.f, ABladeIIGameMode::GetMatineePositionByEventName(MainViewMatinee, TEXT("LE")));
	//	MatineeAutoScrollRightEndPosition = FMath::Min(MatineeLength, ABladeIIGameMode::GetMatineePositionByEventName(MainViewMatinee, TEXT("RE")));
	//	bAutoMoveDirection = false;
	//	SetCameraMatineeCam(MainViewMatinee, MatineePos);
	//}

	//AMatineeActor* DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee && DefaultCameraMatinee->IsValidObj())
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}

	//InitializeUIPositionMarkers();

	//UpdateUIPosition();

	//DirectSwipeSpeedHistory.Empty();
	//ElapsedLastInput = 0.f;	
	//bAcceleratingSwipe = false;
	//bBeginAcceleratingSwipe = false;
	//SpeedSwipe = 0.0f;
}

void FLobbyBattleSelectSceneBase::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//AMatineeActor* DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee && DefaultCameraMatinee->IsValidObj())
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Stop();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}

	//AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//	LobbyGameMode->BindTouchInput(false);
}

void FLobbyBattleSelectSceneBase::SubscribeEvents_OnConstruct()
{
	//LobbyChapterCameraMoveTicket = LobbyChapterCameraMoveClass<float>::GetInstance().Subscribe(
	//	[this](float Value)
	//{
	//	ElapsedLastInput = 0.f;
	//	this->MoveCameraDirect(Value);
	//}
	//);
}
void FLobbyBattleSelectSceneBase::UnsubscribeEvents_OnDestruct()
{
	//LobbyChapterCameraMoveClass<float>::GetInstance().Unsubscribe(LobbyChapterCameraMoveTicket);
}

void FLobbyBattleSelectSceneBase::Tick(float DeltaSeconds)
{
	ElapsedLastInput += DeltaSeconds;
	// 마지막 입력 이후 일정 시간 흐른 후 자동 스크롤.. 아님 Free swipe 이 지나고 나면..?
	if (ElapsedLastInput > 2.f && !bAcceleratingSwipe) 
	{
		if (MatineePos <= MatineeAutoScrollLeftEndPosition)
			bAutoMoveDirection = true;
		if (MatineePos >= MatineeAutoScrollRightEndPosition)
			bAutoMoveDirection = false;

		MoveCameraDirect(MatineeLength * DeltaSeconds * (bAutoMoveDirection ? SpeedAutoScrolling : -SpeedAutoScrolling),
			true // 여기서 MoveCameraDirect 콜 할 때에는 FreeSwipe 으로 진행할 여지가 발생하지 않도록 함.
		);
		// 추가로 이 상황에서는 history 도 남지 않게..
		DirectSwipeSpeedHistory.Empty();
	}

	if (!bMoveCameraDirectCalledThisFrame && bAcceleratingSwipe)
	{
		UpdateForFreeSwipe(DeltaSeconds);
	}

	if (!FMath::IsNearlyZero(SpeedSwipe))
	{
		SwipeScene(SpeedSwipe);
	}
	else if (CheckActorLocationDirty())
	{
		UpdateUIPosition();
	}

	bMoveCameraDirectCalledThisFrame = false;
}

void FLobbyBattleSelectSceneBase::ForceUpdate()
{
	UpdateUIPosition();
}

void FLobbyBattleSelectSceneBase::UpdateForFreeSwipe(float DeltaSeconds)
{
	// FreeSwipe 을 위한 조건
	check(!bMoveCameraDirectCalledThisFrame && bAcceleratingSwipe);
	FLobbySceneManager* SceneMgr = GetSceneManager();
	check(SceneMgr);

	if (bBeginAcceleratingSwipe) // Free swipe 을 처음 시작할 때만.
	{
		SpeedSwipe = FreeSwipeStartingSpeed;
		bBeginAcceleratingSwipe = false;
		DirectSwipeSpeedHistory.Empty(); // 시작한 이상 history 를 날림.
	}

	if (MatineePos == 0.f || MatineePos == MatineeLength)
	{ // Free swipe 도중 양 끝에 도달했을 때인데.. 속도와 가속도 방향을 반전시키는데.. 약간은 감쇠를 주고..
		SpeedSwipe *= -0.8f;
		SwipeAcceleration *= -0.8f;
	}

	if (SceneMgr)
	{
		// 등가속도가 될 수도 있는데 속도 만큼의 저항력을 반영하는 가속도를 산출해 본다..
		SwipeAcceleration = -1.0f * SpeedSwipe * SceneMgr->GetWorldMapSwipeDeaccelerationFactor();

		const float PrevSpeedSwipe = SpeedSwipe;
		SpeedSwipe += (SwipeAcceleration * DeltaSeconds); // dv = at

		// 감속으로 인해 속력이 줄거나 속도가 반대로 되는 상황이면 그만 중단
		if (FMath::Abs(SpeedSwipe) < SceneMgr->GetWorldMapSpeedTriggeringAcceleratingSwipe() || SpeedSwipe * PrevSpeedSwipe < 0)
		{
			SpeedSwipe = 0.f;
			bAcceleratingSwipe = false;
		}

		// AcceleratingSwipeInitialSpeed 이거 역할을 확실히 모르겠음. 뭔가 판별하는 기준으로 쓰긴 해서 놔둠.
		AcceleratingSwipeInitialSpeed = AcceleratingSwipeInitialSpeed - SwipeAcceleration * DeltaSeconds;

		//UE_LOG(LogBladeII, Log, TEXT("ChapterTick bAccel %d, Speed %.4f -> %.4f, Acc %.4f, ElapedLastInput %.2f, MatineePos %.2f"),
		//	(int32)bAcceleratingSwipe, PrevSpeedSwipe, SpeedSwipe, SwipeAcceleration, ElapsedLastInput, MatineePos);
	}
}

// 정해진 속도로 움직일 때 쓴다. 입력으로 스크롤한다거나 자동 스크롤 시
void FLobbyBattleSelectSceneBase::MoveCameraDirect(float Value, bool bNoCheckForFreeSwipe)
{
	FLobbySceneManager* SceneMgr = GetSceneManager();
	check(SceneMgr);
	if (SceneMgr)
	{
		// 사용자 입력으로 인한 MoveCameraDirect 가 마지막으로 호출된 시점에 정해진 SpeedSwipe 값으로 감속을 하면서 free swipe 을 하는건데
		// 특히 모바일 입력환경에서 마지막에 작은 값이 나오곤 해서 free 스크롤이 오래 못가거나 아예 시작을 제대로 못할 때가 있음
		// 따라서 마지막 몇 번의 평균을 내 본다..
		// 터치로 직접 swipe 시에도 소정의 평균을 낸다.

		DirectSwipeSpeedHistory.Add(Value);
		if (DirectSwipeSpeedHistory.Num() > SceneMgr->GetWorldMapDirectSwipeSpeedHistoryNum())
		{
			DirectSwipeSpeedHistory.RemoveAt(0);
		}
		
		// AcceleratingSwipeInitialSpeed 이거 역할을 확실히 모르겠음. 뭔가 판별하는 기준으로 쓰긴 해서 놔둠.
		AcceleratingSwipeInitialSpeed = Value;
		// 기본적인 SpeedSwipe 은 일정 샘플을 가지고 평균낸 값으로
		SpeedSwipe = GetAvgDirectSwipeSpeed(SceneMgr->GetWorldMapDirectSwipeSpeedFilterNum());

		if (DirectSwipeSpeedHistory.Num() > 0)
		{ 			
			float TotalAvgSwipeSpeed = GetAvgDirectSwipeSpeed(); 

			// @.@
			if (!bNoCheckForFreeSwipe) // 간혹 의도와 다른 상황에서의 MoveCameraDirect 콜이 AcceleratingSwipe 을 발생시키는 듯..
			{
				bAcceleratingSwipe = (
					(SceneMgr->GetWorldMapSwipeDeaccelerationFactor() != 0.f && FMath::Abs(TotalAvgSwipeSpeed) > SceneMgr->GetWorldMapSpeedTriggeringAcceleratingSwipe())
					||
					(FMath::Abs(AcceleratingSwipeInitialSpeed * 0.4f + TotalAvgSwipeSpeed * 0.6f) > SceneMgr->GetWorldMapSpeedTriggeringAcceleratingSwipe())
					);
			}

			if (bAcceleratingSwipe)
			{
				// Free swipe 을 하게 될 거 같으면 시작 속도는 수집한 전체 샘플 평균을 가지고..
				FreeSwipeStartingSpeed = TotalAvgSwipeSpeed;
				bBeginAcceleratingSwipe = true;

				//UE_LOG(LogBladeII, Log, TEXT("MoveCameraDirect About to begin FreeSwip with Avgspeed %.4f from %d samples"),
				//	FreeSwipeStartingSpeed, DirectSwipeSpeedHistory.Num());

				// 실제로는 다음 틱에도 MoveCameraDirect 가 불리지 않아야 Free swipe 을 하게 될 것.
			}			
		}		
		bMoveCameraDirectCalledThisFrame = true;
	}

}

float FLobbyBattleSelectSceneBase::GetAvgDirectSwipeSpeed(int32 InMaxHistoryNum, bool bRespectTheLastSign) const
{
	const int32 FinalSampleNum = InMaxHistoryNum < 0 ? DirectSwipeSpeedHistory.Num() : FMath::Min(InMaxHistoryNum, DirectSwipeSpeedHistory.Num());
	if (FinalSampleNum <= 0) { // 모인 샘플이 없는 상황일 듯.
		return 0.0f;
	}

	// 뒤쪽일수록 최신 샘플로 간주하고 뒤에서부터..
	const int32 AvgFirstIndex = DirectSwipeSpeedHistory.Num() - 1;
	const int32 AvgLastIndex = DirectSwipeSpeedHistory.Num() - FinalSampleNum;
	check(AvgLastIndex >= 0);
	float RetAvg = 0.0f;
	for (int32 SI = AvgFirstIndex; SI >= AvgLastIndex; --SI)
	{
		// bRespectTheLastSign 인 경우 절대값으로 평균내서 부호는 마지막 샘플 걸로다가 사용.
		RetAvg += bRespectTheLastSign ? FMath::Abs(DirectSwipeSpeedHistory[SI]) : DirectSwipeSpeedHistory[SI];
	}
	RetAvg /= (float)FinalSampleNum;
	if (bRespectTheLastSign)
	{
		float TheLastSample = DirectSwipeSpeedHistory[DirectSwipeSpeedHistory.Num() - 1];
		if (RetAvg * TheLastSample < 0.0f)
		{
			RetAvg *= -1.0f;
		}
	}
	return RetAvg;
}

void FLobbyBattleSelectSceneBase::SwipeScene(float Delta)
{
//	AMatineeActor* DefaultCameraMatinee = GetDefaultCameraMatinee();
//	if (DefaultCameraMatinee && DefaultCameraMatinee->bIsPlaying){
//		return;
//	}
//
//	AMatineeActor* MainViewMatinee = GetChapterMatinee();
//	if (MainViewMatinee && MainViewMatinee->IsValidObj() && MainViewMatinee->GetWorld()) // 레벨 스트리밍과 얽혀서 Invalid 한 경우가 있는 거 같다..
//	{
//		MatineePos = FMath::Clamp<float>(MatineePos + Delta, 0.f, MatineeLength);
////#if UE_BUILD_DEBUG // 아마도 모험맵 자동 스크롤의 영향인지 가만히 있어도 로그가 많이 나와서..
////		UE_LOG(LogStreaming, Warning, TEXT("MatineePos ====== '%f' "), MatineePos);
////#endif
//		MainViewMatinee->Play();
//		MainViewMatinee->SetPosition(MatineePos);
//		MainViewMatinee->Pause();
//
//		SetCameraMatineeCam(MainViewMatinee, MatineePos);
//	}

	// 하위 클래스에서 UpdateUIPosition 을 호출하도록..
}

//#include "SceneView.h"
#include "Blueprint/WidgetLayoutLibrary.h"
void FLobbyBattleSelectSceneBase::GetUIMarkerPositions(const TArray<TWeakObjectPtr<AActor>>& InMarkerActors, TArray<FVector2D>& OutPos2D, TArray<FVector>& OutPos3D, FMatrix& OutViewProjMat)
{
	int32 ArrayNumDiff = InMarkerActors.Num() - OutPos2D.Num();
	if (ArrayNumDiff > 0){
		OutPos2D.AddZeroed(ArrayNumDiff);
	}
	ArrayNumDiff = InMarkerActors.Num() - OutPos3D.Num();
	if (ArrayNumDiff > 0){
		OutPos3D.AddZeroed(ArrayNumDiff);
	}

	APlayerController* LocalPlayerController = GetLocalPlayerController();
	if (LocalPlayerController)
	{
		// UMG 에서 쓸 UIPosition 계산 시에 현재 해상도가 아닌 기준 해상도 기준으로 넣어주어야 하므로 뷰 스케일이 필요함.
		float DPIScale = UWidgetLayoutLibrary::GetViewportScale(LocalPlayerController);

		ULocalPlayer* const LP = LocalPlayerController ? LocalPlayerController->GetLocalPlayer() : nullptr;
		if (LP && LP->ViewportClient)
		{
			 //get the projection data
			FSceneViewProjectionData ProjectionData;
			if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
			{
				OutViewProjMat = ProjectionData.ComputeViewProjectionMatrix();

				for (int32 AI = 0; AI < InMarkerActors.Num(); ++AI)
				{
					const TWeakObjectPtr<AActor>& ThisActor = InMarkerActors[AI];
					if (ThisActor.IsValid())
					{
						OutPos3D[AI] = ThisActor->GetActorLocation();
						FSceneView::ProjectWorldToScreen(OutPos3D[AI], ProjectionData.GetConstrainedViewRect(), OutViewProjMat, OutPos2D[AI]);
						OutPos2D[AI] += FVector2D(ProjectionData.GetConstrainedViewRect().Min.X, ProjectionData.GetConstrainedViewRect().Min.Y);
						OutPos2D[AI] /= DPIScale;
					}
				}
			}
		}
	}
}

//=================================================================================================
//
//	Chapter
//
//=================================================================================================
//#include "B2UIChapter.h"
//#include "B2UIChapterFrame.h"

FLobbyChapterScene::FLobbyChapterScene(class FLobbySceneManager* OwnerSceneManager) 
	: FLobbyBattleSelectSceneBase(OwnerSceneManager)
{	

}

FLobbyChapterScene::~FLobbyChapterScene()
{
	StageUIPositionActors.Empty();
}

void FLobbyChapterScene::OpenScene()
{
	//AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // 레벨 스트리밍이 새로 되지 않더라도 매 챕터마다 Matinee 는 새로 세팅해야 하므로 강제 update
	//	LobbyGameMode->UpdateChapterData();
	//}

	//FLobbyBattleSelectSceneBase::OpenScene();

	//auto* UIManager = UB2UIManager::GetInstance();
	//auto* UI = UIManager ? UIManager->GetUI<UB2UIChapterFrame>(UIFName::Chapter) : nullptr;
	//if (UI)
	//	SpeedAutoScrolling = UI->TimeAutoScrolling != 0.f ? 1.f / UI->TimeAutoScrolling : 0.f;
}

void FLobbyChapterScene::CloseScene()
{
	FLobbyBattleSelectSceneBase::CloseScene();

	StageUIPositionActors.Empty();
	CachedUIActorLocation.Empty();
}

void FLobbyChapterScene::SwipeScene(float Delta)
{
	FLobbyBattleSelectSceneBase::SwipeScene(Delta);

	//auto* ChapterMatinee = GetChapterMatinee();
	//if (ChapterMatinee)
	//{
	//	if (MatineePos > SWIPE_NEXTSTAGE_POS || MatineePos < SWIPE_PREVSTAGE_POS)
	//	{
	//		/*
	//		auto* UIChapterFrame = UB2UIManager::GetInstance()->GetUI<UB2UIChapterFrame>(UIFName::Chapter);
	//		if (MatineePos > SWIPE_NEXTSTAGE_POS)
	//			UIChapterFrame->ChangeChapter(EUISearchType::Forward);
	//		else
	//			UIChapterFrame->ChangeChapter(EUISearchType::Reverse);
	//		*/
	//	}
	//}

	UpdateUIPosition();	
}

void FLobbyChapterScene::InitializeUIPositionMarkers()
{
	//StageUIPositionActors.Empty();
	//CachedUIActorLocation.Empty();

	//auto* ChapterMatinee = GetChapterMatinee();
	//if (!ChapterMatinee)
	//	return;

	//if (!ChapterMatinee->bIsPlaying)
	//	ChapterMatinee->InitInterp();

	//TArray<FString> StageActorGroupNames;
	//for (int32 idx = 0; idx < 10; ++idx)
	//{ // 요 네이밍에 부합하는 애들만 UI 포지션 세팅 마커로 간주.
	//	StageActorGroupNames.Add(FString::Printf(TEXT("Stage%02d"), idx + 1));
	//}

	//for (auto* InterpGroup : ChapterMatinee->GroupInst)
	//{
	//	if (InterpGroup->Group)
	//	{
	//		FString GroupName = InterpGroup->Group->GroupName.ToString();
	//		if (StageActorGroupNames.Find(GroupName) != INDEX_NONE)
	//		{
	//			AActor* Actor = InterpGroup->GetGroupActor();
	//			if (Actor)
	//			{
	//				StageUIPositionActors.Add(Actor);
	//				InterpGroup->GetGroupActor()->SetActorHiddenInGame(true);	//맵에서 그 위치 큐브 안보이게 하는 구간.

	//				for (int32 idx = 0; idx < InterpGroup->Group->InterpTracks.Num(); ++idx)
	//				{
	//					UInterpTrackMove* MoveTrack = Cast<UInterpTrackMove>(InterpGroup->Group->InterpTracks[idx]);
	//					if (MoveTrack && !MoveTrack->IsDisabled()) // MoveTrack 이 쓸데없이 여러개 등록된 경우가 있음. 하나만 걸러내야..
	//					{
	//						CachedUIActorLocation.Add(MoveTrack->EvalPositionAtTime(InterpGroup->TrackInst[idx], 0.f));
	//						break;
	//					}
	//				}
	//			}					
	//		}
	//	}		
	//}

	//// 여기 체크에 걸리면 의도와 다르게 세팅한 마티니 트랙이 있다거나 한 경우. 엉뚱하게 네이밍에 걸렸다거나..?
	//BII_CHECK(StageUIPositionActors.Num() == CachedUIActorLocation.Num());

	//if (!ChapterMatinee->bIsPlaying)
	//	ChapterMatinee->TermInterp();
}

void FLobbyChapterScene::UpdateUIPosition()
{
	//auto* UIChapterFrame = UB2UIManager::GetInstance()->GetUI<UB2UIChapterFrame>(UIFName::Chapter);
	//auto* UIChapter = UIChapterFrame ? UIChapterFrame->GetCurChapter() : NULL;
	//if (UIChapter)
	//{
	//	TArray<FVector2D> UIPositions;
	//	GetUIMarkerPositions(StageUIPositionActors, UIPositions, CachedUIActorLocation, CachedViewProjectionMatrix);

	//	UIChapter->UpdateStageUIPosition(UIPositions);
	//}
}

bool FLobbyChapterScene::CheckActorLocationDirty()
{
	BII_CHECK(StageUIPositionActors.Num() == CachedUIActorLocation.Num());

	for (int32 idx = 0; idx < StageUIPositionActors.Num(); ++idx)
	{
		if (StageUIPositionActors[idx].IsValid())
		{
			if (!StageUIPositionActors[idx]->GetActorLocation().Equals(CachedUIActorLocation[idx]))
				return true;
		}
	}

	auto* LocalPlayerController = GetLocalPlayerController();
	if (LocalPlayerController)
	{
		ULocalPlayer* const LP = LocalPlayerController ? LocalPlayerController->GetLocalPlayer() : nullptr;
		if (LP && LP->ViewportClient)
		{
			// get the projection data
			FSceneViewProjectionData ProjectionData;
			if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
			{
				if (!ProjectionData.ComputeViewProjectionMatrix().Equals(CachedViewProjectionMatrix))
					return true;
			}
		}
	}

	return false;
}

void FLobbyChapterScene::Tick(float DeltaSeconds)
{
	FLobbyBattleSelectSceneBase::Tick(DeltaSeconds);
}

//=================================================================================================
//
//	ModSelect
//
//=================================================================================================
FLobbyModSelectScene::FLobbyModSelectScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbyBattleSelectSceneBase(OwnerSceneManager)
{
	const int32 AllArrayNum = static_cast<int32>(ELobbyModSceneElem::LMODIDX_End);
	UIPositionGroupName.AddZeroed(AllArrayNum);
	UIPositionActors.AddZeroed(AllArrayNum);
	CachedUIActorLocation.AddZeroed(AllArrayNum);

	// 아래 이름은 Matinee 그룹 이름으로 사용하게 될 거.
	UIPositionGroupName[GetIndexOfModSceneElem(ELobbyModSceneElem::LMODIDX_PVP)] = FName(TEXT("Marker_TagMatch"));
	UIPositionGroupName[GetIndexOfModSceneElem(ELobbyModSceneElem::LMODIDX_TeamMatch)] = FName(TEXT("Marker_TeamMatch"));
	UIPositionGroupName[GetIndexOfModSceneElem(ELobbyModSceneElem::LMODIDX_HeroTop)] = FName(TEXT("Marker_HeroTop"));
	UIPositionGroupName[GetIndexOfModSceneElem(ELobbyModSceneElem::LMODIDX_GuildMatch)] = FName(TEXT("Marker_GuildMatch"));
	UIPositionGroupName[GetIndexOfModSceneElem(ELobbyModSceneElem::LMODIDX_CounterDungeon)] = FName(TEXT("Marker_CounterDungeon"));
	UIPositionGroupName[GetIndexOfModSceneElem(ELobbyModSceneElem::LMODIDX_Raid)] = FName(TEXT("Marker_Raid"));
	UIPositionGroupName[GetIndexOfModSceneElem(ELobbyModSceneElem::LMODIDX_Control)] = FName(TEXT("Marker_Control"));
}

FLobbyModSelectScene::~FLobbyModSelectScene()
{

}

void FLobbyModSelectScene::OpenScene()
{
	//AB2LobbyGameMode* LobbyGM = GetLobbyGameMode();
	//if (LobbyGM)
	//{
	//	LobbyGM->UpdateNativeLobbyMatineeRef(); // Bundle_A 와 동일한 스트리밍 레벨에 세팅이 되었다면 이 시점에 적절한 값을 들고 있지 않을 것이니 한번 더 업데이트.
	//}

	//FLobbyBattleSelectSceneBase::OpenScene();

	//auto* UIManager = UB2UIManager::GetInstance();
	//auto* UI = UIManager ? UIManager->GetUI<UB2UIModSelect>(UIFName::ModSelect) : nullptr;
	//if (UI)
	//	SpeedAutoScrolling = UI->TimeAutoScrolling != 0.f ? 1.f / UI->TimeAutoScrolling : 0.f;
}

void FLobbyModSelectScene::CloseScene()
{
	FLobbyBattleSelectSceneBase::CloseScene();
}

void FLobbyModSelectScene::Tick(float DeltaSeconds)
{
	FLobbyBattleSelectSceneBase::Tick(DeltaSeconds);

	UpdateUIPosition(); // UI Position 이 살짝 꼬일 때가 있어서 결국 tick 에서 매번 처리.. ㅡ.ㅡ
}

void FLobbyModSelectScene::InitializeUIPositionMarkers()
{
	//AMatineeActor* MainViewMatinee = GetChapterMatinee();
	//if (!MainViewMatinee || !MainViewMatinee->IsValidObj()){
	//	return;
	//}

	//if (!MainViewMatinee->bIsPlaying){
	//	MainViewMatinee->InitInterp();
	//}

	checkSlow(UIPositionGroupName.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);
	checkSlow(UIPositionActors.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);
	checkSlow(CachedUIActorLocation.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);

	for (int32 MEI = 0; MEI < UIPositionGroupName.Num(); ++MEI)
	{
		const FName& ThisGroupName = UIPositionGroupName[MEI];

		//for (UInterpGroupInst* ThisGroupInst : MainViewMatinee->GroupInst)
		//{
		//	if (ThisGroupInst && ThisGroupInst->Group && ThisGroupInst->Group->GroupName == ThisGroupName)
		//	{ // 이번 Elem 의 정해진 그룹 이름과 일치하는 그룹.
		//		if (ThisGroupInst->GetGroupActor())
		//		{
		//			UIPositionActors[MEI] = ThisGroupInst->GetGroupActor();
		//			ThisGroupInst->GetGroupActor()->SetActorHiddenInGame(true); // UI 위치 마킹 역할일 뿐. 숨겨놓음.

		//			for (int32 TI = 0; TI < ThisGroupInst->Group->InterpTracks.Num(); ++TI)
		//			{
		//				UInterpTrackMove* MoveTrack = Cast<UInterpTrackMove>(ThisGroupInst->Group->InterpTracks[TI]);
		//				if (MoveTrack)
		//				{
		//					CachedUIActorLocation[MEI] = MoveTrack->EvalPositionAtTime(ThisGroupInst->TrackInst[TI], 0.f);
		//				}
		//			}
		//		}
		//		break;
		//	}
		//}
	}

	/*if (!MainViewMatinee->bIsPlaying){
		MainViewMatinee->TermInterp();
	}*/
}

void FLobbyModSelectScene::SwipeScene(float Delta)
{
	FLobbyBattleSelectSceneBase::SwipeScene(Delta);

	UpdateUIPosition();
}

bool FLobbyModSelectScene::CheckActorLocationDirty()
{
	checkSlow(UIPositionGroupName.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);
	checkSlow(UIPositionActors.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);
	checkSlow(CachedUIActorLocation.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);

	for (int32 idx = 0; idx < UIPositionActors.Num(); ++idx)
	{
		if (UIPositionActors[idx].IsValid())
		{
			if (!UIPositionActors[idx]->GetActorLocation().Equals(CachedUIActorLocation[idx]))
				return true;
		}
	}

	APlayerController* LocalPlayerController = GetLocalPlayerController();
	if (LocalPlayerController)
	{
		ULocalPlayer* const LP = LocalPlayerController ? LocalPlayerController->GetLocalPlayer() : nullptr;
		if (LP && LP->ViewportClient)
		{
			// get the projection data
			FSceneViewProjectionData ProjectionData;
			if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
			{
				if (!ProjectionData.ComputeViewProjectionMatrix().Equals(CachedViewProjectionMatrix))
					return true;
			}
		}
	}

	return false;
}

void FLobbyModSelectScene::UpdateUIPosition()
{
//	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
//	UB2UIModSelect* UIMod = UIMgr ? UIMgr->GetUI<UB2UIModSelect>(UIFName::ModSelect) : NULL;
//	
//	if (UIMod)
//	{
//		checkSlow(UIPositionGroupName.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);
//		checkSlow(UIPositionActors.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);
//		checkSlow(CachedUIActorLocation.Num() == (int32)ELobbyModSceneElem::LMODIDX_End);
//
//		// 2D 위치들 얻어와서 최종적으로 UIMod 쪽으로 넣어줌.
//		TArray<FVector2D> UIPositions;
//		GetUIMarkerPositions(UIPositionActors, UIPositions, CachedUIActorLocation, CachedViewProjectionMatrix);
//		
//		UIMod->UpdateModUIPosition(UIPositions);
//	}
}

//=================================================================================================
//
//	TeamMatch
//
//=================================================================================================
FLobbyTeamMatchScene::FLobbyTeamMatchScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyTeamMatchScene::~FLobbyTeamMatchScene()
{

}

void FLobbyTeamMatchScene::SubscribeEvents_OnConstruct()
{
	//DeliveryGetTeamBattleStatusTicket = DeliveryGetTeamBattleStatusClass<FB2TeamBattleStatus>::GetInstance().Subscribe([this](const FB2TeamBattleStatus& TeamBattleStatus)
	//{
	//	this->SetMyAllFormationData(TeamBattleStatus);
	//});

	//DeliveryUpdateTeamBattleEntryTicket = DeliveryUpdateTeamBattleEntryClass<FB2TeamBattleEntryPtr>::GetInstance().Subscribe([this](FB2TeamBattleEntryPtr Entry)
	//{
	//	this->SetMyCurrentFormationEntry(Entry);
	//});

	//DeliveryEnhanceTeamFormationTicket = DeliveryEnhanceTeamFormationClass<FB2EnhanceTeamFormation>::GetInstance().Subscribe([](const FB2EnhanceTeamFormation& EnhanceResult)
	//{
	//	//auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	//	//if (TeamMatchDoc)
	//	//{
	//	//	auto Formation = GET_TUPLE_DATA(FB2ResponseEnhanceTeamFormation::formation_index, EnhanceResult);

	//	//	switch (SvrToCliFormationType(Formation->formation_type))
	//	//	{
	//	//	case ETMFormation::ETM_Normal:
	//	//		TeamMatchDoc->SetFormationNormalLevel(Formation->formation_level);
	//	//		break;
	//	//	case ETMFormation::ETM_Attack:
	//	//		TeamMatchDoc->SetFormationAttLevel(Formation->formation_level);
	//	//		break;
	//	//	case ETMFormation::ETM_Defense:
	//	//		TeamMatchDoc->SetFormationDefLevel(Formation->formation_level);
	//	//		break;
	//	//	}
	//	//}
	//});
}
void FLobbyTeamMatchScene::UnsubscribeEvents_OnDestruct()
{
	//DeliveryGetTeamBattleStatusClass<FB2TeamBattleStatus>::GetInstance().Unsubscribe(DeliveryGetTeamBattleStatusTicket);
	//DeliveryUpdateTeamBattleEntryClass<FB2TeamBattleEntryPtr>::GetInstance().Unsubscribe(DeliveryUpdateTeamBattleEntryTicket);
	//DeliveryEnhanceTeamFormationClass<FB2EnhanceTeamFormation>::GetInstance().Unsubscribe(DeliveryEnhanceTeamFormationTicket);
}

void FLobbyTeamMatchScene::SetMyAllFormationData(const FB2TeamBattleStatus& TeamBattleStatus)
{
	/*UB2UIDocTeamMatch* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (!TeamMatchDoc)
		return;

	auto MyRanking = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::ranking_index, TeamBattleStatus);
	
	TeamMatchDoc->SetPCRankPercentage(MyRanking->total_ranker_count > 0 ? 100.f * MyRanking->ranking / (float)MyRanking->total_ranker_count : 100);
	TeamMatchDoc->SetPCRank(MyRanking->ranking);	

	auto matchStatus = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::match_status_index, TeamBattleStatus);
	TeamMatchDoc->SetPCGrade(matchStatus->battle_grade);
	TeamMatchDoc->SetPCBeforeGrade(matchStatus->battle_grade);
	TeamMatchDoc->SetPCGradeStar(matchStatus->battle_star_count);
	TeamMatchDoc->SetPCBeforeGradeStar(matchStatus->battle_star_count);
	TeamMatchDoc->SetPCCurrContinuousWins(matchStatus->continue_win);
	TeamMatchDoc->SetPCCurrTopContinuousWins(matchStatus->continue_win_max);

	auto Rewardboxstatus = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::reward_box_status_index, TeamBattleStatus);
	TeamMatchDoc->SetRewardBoxStatus(Rewardboxstatus);

	auto FormationInfo = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::formations_index, TeamBattleStatus);
	for (auto Formation : FormationInfo)
	{
		switch (SvrToCliFormationType(Formation->formation_type))
		{
		case ETMFormation::ETM_Normal:
			TeamMatchDoc->SetFormationNormalLevel(Formation->formation_level);
			break;
		case ETMFormation::ETM_Attack:
			TeamMatchDoc->SetFormationAttLevel(Formation->formation_level);
			break;
		case ETMFormation::ETM_Defense:
			TeamMatchDoc->SetFormationDefLevel(Formation->formation_level);
			break;
		}
	}

	auto FormationEntryData = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::entry_index, TeamBattleStatus);
	SetMyCurrentFormationEntry(FormationEntryData);


	auto RankingRewardData = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::ranking_reward_index, TeamBattleStatus);

	if (RankingRewardData)
	{
		TeamMatchDoc->SetIsTeamReward(true);
		TeamMatchDoc->SetTeamReward(RankingRewardData);

		auto HallOfFameData = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::hall_of_fames_index, TeamBattleStatus);


	}
	else
		TeamMatchDoc->SetIsTeamReward(false);

	auto RankingList = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::rankers_index, TeamBattleStatus);
	auto BeginRank = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::from_ranking_index, TeamBattleStatus);

	TeamMatchDoc->RankerInfoContainer.SetRankerList(RankingList, BeginRank);

	auto RewardRemainTime = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::settlement_time_index, TeamBattleStatus);
	TeamMatchDoc->SetTeamRewardTime(RewardRemainTime);

	auto TodayReward = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::today_reward_index, TeamBattleStatus);
	TeamMatchDoc->SetTeamTodayReward(TodayReward);

	auto LastReward = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::last_reward_index, TeamBattleStatus);
	TeamMatchDoc->SetTeamLastReward(LastReward);

	auto ParticipationCount = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::today_participation_count_index, TeamBattleStatus);
	TeamMatchDoc->SetTeamParticipationCount(ParticipationCount);

	auto CurrentGold = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::current_gold_index, TeamBattleStatus);
	TeamMatchDoc->SetTeamCurrentGold(CurrentGold);

	auto CurrentBladepoint = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::current_bladepoint_index, TeamBattleStatus);
	TeamMatchDoc->SetTeamCurrentBladepoint(CurrentBladepoint);

	auto CurrentGem = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::current_gem_index, TeamBattleStatus);
	TeamMatchDoc->SetTeamCurrentGem(CurrentGem);

	auto CurrentMatchPoint = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::match_point_index, TeamBattleStatus);
	TeamMatchDoc->SetTeamMatchPoint(CurrentMatchPoint);

	auto CurrentAdditionalMatchCount = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::additional_match_count_index, TeamBattleStatus);
	TeamMatchDoc->SetTeamAdditionalMatchCount(CurrentAdditionalMatchCount);

	auto NextChargeSecond = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::match_point_next_charge_second_index, TeamBattleStatus);
	TeamMatchDoc->SetNextChargeSecond(NextChargeSecond);*/
}

//#include "B2UITeamMatchMain.h"
void FLobbyTeamMatchScene::SetMyCurrentFormationEntry(FB2TeamBattleEntryPtr TeamEntry)
{
	//UB2UIDocTeamMatch* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();

	//if (!TeamMatchDoc)
	//	return;

	//TeamMatchDoc->SetTMFormation(SvrToCliFormationType(TeamEntry->formation_type));
	//TeamMatchDoc->SetCharacterClass(true, 0, SvrToCliPCClassType(TeamEntry->character_slot_1));
	//TeamMatchDoc->SetCharacterClass(true, 1, SvrToCliPCClassType(TeamEntry->character_slot_2));
	//TeamMatchDoc->SetCharacterClass(true, 2, SvrToCliPCClassType(TeamEntry->character_slot_3));

	////임시 Todo : Doc delegate로 빼야됨
	//auto* TeamMatchLobby = UB2UIManager::GetInstance()->GetUI<UB2UITeamMatchMain>(UIFName::TeamMatchUI);
	//if (TeamMatchLobby)
	//{
	//	TeamMatchLobby->SetEntryInfo();
	//}
}

//#include "B2UIPVPMain.h"
//=================================================================================================
//
//	TagMatch
//
//=================================================================================================
FLobbyTagMatchScene::FLobbyTagMatchScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyTagMatchScene::~FLobbyTagMatchScene()
{
	
}

void FLobbyTagMatchScene::SubscribeEvents_OnConstruct()
{
	//DeliveryGetMatchStatusTicket = DeliveryGetMatchStatusClass<FB2TagMatchStatus>::GetInstance().Subscribe([this](const FB2TagMatchStatus& TagMatchStatus)
	//{
	//	SetUIData(TagMatchStatus);
	//});
}
void FLobbyTagMatchScene::UnsubscribeEvents_OnDestruct()
{
	//DeliveryGetMatchStatusClass<FB2TagMatchStatus>::GetInstance().Unsubscribe(DeliveryGetMatchStatusTicket);
}

void FLobbyTagMatchScene::SetUIData(const FB2TagMatchStatus& TagMatchStatus)
{
	////우선 DocBattle에 담는데, 데이터가 많아지면 Doc를 새로 만드는 것으로..
	//UB2UIDocBattle*			BattleDoc = UB2UIDocHelper::GetDocBattle();
	//UB2UIDocPVP1on1Rival*	PvPDoc = UB2UIDocHelper::GetDocPVP1on1Rival();

	//if (!PvPDoc || !BattleDoc)
	//	return;

	//auto RewardBoxStatus = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::reward_box_status_index, TagMatchStatus);

	//PvPDoc->SetRewardBoxStatus(RewardBoxStatus);

	//auto MyRanking = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::ranking_index, TagMatchStatus);
	//BattleDoc->SetTagMatchRankPercentage(MyRanking->total_ranker_count > 0 ? 100.f * MyRanking->ranking / (float)MyRanking->total_ranker_count : 100);
	//BattleDoc->SetTagMatchRank(MyRanking->ranking);

	//auto MatchStatus = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::match_status_index, TagMatchStatus);
	//
	//BattleDoc->SetTagMatchGrade(MatchStatus->battle_grade);
	//BattleDoc->SetTagMatchBeforeGrade(MatchStatus->battle_grade);
	//BattleDoc->SetTagMatchGradeStar(MatchStatus->battle_star_count);
	//BattleDoc->SetTagMatchBeforeGradeStar(MatchStatus->battle_star_count);
	//BattleDoc->SetTagMatchCurrContinuousWins(MatchStatus->continue_win);
	//BattleDoc->SetTagMatchCurrTopContinuousWins(MatchStatus->continue_win_max);

	//auto RankingReward = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::ranking_reward_index, TagMatchStatus);
	//auto HallofFame = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::hall_of_fames_index, TagMatchStatus);

	//if (RankingReward)
	//{
	//	PvPDoc->SetIsTagReward(true);
	//	PvPDoc->SetTagReward(RankingReward);
	//}
	//else
	//	PvPDoc->SetIsTagReward(false);
	//
	//auto RankingList = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::rankers_index, TagMatchStatus);
	//auto BeginRank = GET_TUPLE_DATA(FB2ResponseTeamBattleStatus::from_ranking_index, TagMatchStatus);

	//PvPDoc->RankerInfoContainer.SetRankerList(RankingList, BeginRank);

	//auto RewardRemainTime = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::settlement_time_index, TagMatchStatus);
	//PvPDoc->SetTagRewardTime(RewardRemainTime);

	//auto TodayReward = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::today_reward_index, TagMatchStatus);
	//PvPDoc->SetTagTodayReward(TodayReward);

	//auto LastReward = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::last_reward_index, TagMatchStatus);
	//PvPDoc->SetTagLastReward(LastReward);

	//auto ParticipationCount = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::today_participation_count_index, TagMatchStatus);
	//PvPDoc->SetTagParticipationCount(ParticipationCount);

	//auto CurrentMatchPoint = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::match_point_index, TagMatchStatus);
	//PvPDoc->SetMatchPoint(CurrentMatchPoint);

	//auto CurrentAdditionalMatchCount = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::additional_match_count_index, TagMatchStatus);
	//PvPDoc->SetAdditionalMatchCount(CurrentAdditionalMatchCount);

	//auto NextChargeSecond = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::match_point_next_charge_second_index, TagMatchStatus);
	//PvPDoc->SetNextChargeSecond(NextChargeSecond);

	//auto CurrentGold = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::current_gold_index, TagMatchStatus);
	//BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, CurrentGold);

	//auto CurrentGem = GET_TUPLE_DATA(FB2ResponseGetMatchStatus::current_gem_index, TagMatchStatus);
	//BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, CurrentGem);
}

//=================================================================================================
//
//	Title
//
//=================================================================================================

FLobbyTitleScene::FLobbyTitleScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
	if(OwnerSceneManager)
		OwnerSceneManager->SetPlayingBeginAnim(true);
}

FLobbyTitleScene::~FLobbyTitleScene()
{

}

//=================================================================================================
//
//	Select Login
//
//=================================================================================================

FSelectLoginScene::FSelectLoginScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FSelectLoginScene::~FSelectLoginScene()
{

}

//=================================================================================================
//
//	Lobby Store
//
//=================================================================================================
FLobbyStoreScene::FLobbyStoreScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager), ReserveStoreScene(true)
{

}

FLobbyStoreScene::~FLobbyStoreScene()
{
	
}

void FLobbyStoreScene::SubscribeEvents_OnConstruct()
{
	UnsubscribeEvents_OnDestruct();

	//Issues.Add(ReserveStoreSceneClass<bool>::GetInstance().Subscribe2(
	//	[this](bool bReserveScene)
	//{
	//	this->ReserveStoreScene = bReserveScene;
	//}
	//));

	//Issues.Add(DeliveryGetGeneralShopClass<FB2ResponseGetGeneralShopPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseGetGeneralShopPtr& ShopData)
	//{
	//	this->SetGeneralShopData(ShopData);
	//	if (this->ReserveStoreScene)
	//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::Store);
	//}
	//));
}
void FLobbyStoreScene::UnsubscribeEvents_OnDestruct()
{
	Issues.Empty();
}

void FLobbyStoreScene::SetGeneralShopData(const FB2ResponseGetGeneralShopPtr& ShopData)
{
	//auto* DocStore = UB2UIDocHelper::GetDocStore();
	//if (DocStore == nullptr)
	//	return;

	//for (auto Mileage : ShopData->shop_mileages)
	//{
	//	DocStore->SetMileagePoint(Mileage->shop_mileage_type - b2network::B2ShopMileageType::LOTTERY, Mileage->mileage);
	//	DocStore->SetMileageReceivedIndex(Mileage->shop_mileage_type - b2network::B2ShopMileageType::LOTTERY, Mileage-> reward_index - 1);
	//}

	//for (auto Restrict : ShopData->shop_buy_manages)
	//{
	//	DocStore->SetReceivedRestrictCount(Restrict->product_id, Restrict->buy_count);
	//}
	//
	//for (auto ProductMark : ShopData->shop_product_marks)
	//{
	//	DocStore->SetProductMark(ProductMark->product_id, ProductMark->mark_type);
	//	DocStore->SetProductSalePeriod(ProductMark->product_id, ProductMark->sales_start_time, ProductMark->sales_end_time);
	//}

	//DocStore->OnGeneralShopChanged.Broadcast();
}

//=================================================================================================
//
//	Mail List
//
//=================================================================================================
FLobbyMailScene::FLobbyMailScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyMailScene::~FLobbyMailScene()
{

}

void FLobbyMailScene::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbyMailScene::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto* DocSome = UB2UIDocHelper::GetDocSome();
	//if (DocSome)
	//	DocSome->SetOpenedMailTab(MailTabIndex::EMailTab_Gift);
}

void FLobbyMailScene::SubscribeEvents_OnConstruct()
{
	//DeliveryGetMailListTicket = DeliveryGetMailListClass<FB2MailList>::GetInstance().Subscribe([this](const FB2MailList& MailList)
	//{
	//	//todo UI열기
	//	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::Mail);

	//	//todo Category별
	//	SetUIData(MailList);
	//});
}
void FLobbyMailScene::UnsubscribeEvents_OnDestruct()
{
	//DeliveryGetMailListClass<FB2MailList>::GetInstance().Unsubscribe(DeliveryGetMailListTicket);
}

//#include "B2UIMail.h"
void FLobbyMailScene::SetUIData(const FB2MailList& MailList)
{
	//auto* MailSceneUI = UB2UIManager::GetInstance()->GetUI<UB2UIMail>(UIFName::Mail);
	//if (MailSceneUI)
	//{
	//	MailSceneUI->SetMailList(MailList);
	//}
}

//=================================================================================================
//
//	Counter Attack MOD - Daily dungeon
//
//=================================================================================================
FLobbyCounterAttackScene::FLobbyCounterAttackScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyCounterAttackScene::~FLobbyCounterAttackScene()
{
	
}

void FLobbyCounterAttackScene::SubscribeEvents_OnConstruct()
{
	//DeliveryGetCounterDundeonTicket = DeliveryGetCounterDundeonClass<FB2GetCounterDungeon>::GetInstance().Subscribe([this](const FB2GetCounterDungeon& msg)
	//{
	//	this->SetGetCounterDungeonInfo(msg);
	//});
}
void FLobbyCounterAttackScene::UnsubscribeEvents_OnDestruct()
{
	//DeliveryGetCounterDundeonClass<FB2GetCounterDungeon>::GetInstance().Unsubscribe(DeliveryGetCounterDundeonTicket);
}

void FLobbyCounterAttackScene::SetGetCounterDungeonInfo(const FB2GetCounterDungeon& DungeonInfo)
{
	//auto* CounterDoc = UB2UIDocHelper::GetDocCounterAttack();

	//if (!CounterDoc)
	//	return;

	//auto OpenedDungeonType = GET_TUPLE_DATA(FB2ResponseGetCounterDungeonStatus::opened_dungeon_types_index, DungeonInfo);
	//CounterDoc->SetOpenedDungeonType(OpenedDungeonType);

	//auto LeftTicket = GET_TUPLE_DATA(FB2ResponseGetCounterDungeonStatus::left_ticket_index, DungeonInfo);
	//CounterDoc->SetLeftTicket(LeftTicket);

	//auto WeeklyClearCount = GET_TUPLE_DATA(FB2ResponseGetCounterDungeonStatus::weekly_clear_count_index, DungeonInfo);
	//CounterDoc->SetWeeklyClearCount(WeeklyClearCount);

	//auto ReceivedWeeklyReward = GET_TUPLE_DATA(FB2ResponseGetCounterDungeonStatus::received_weekly_reward_index, DungeonInfo);
	//CounterDoc->SetReceivedWeeklyReward(ReceivedWeeklyReward);

	//auto NextBuyTicketGem = GET_TUPLE_DATA(FB2ResponseGetCounterDungeonStatus::next_buy_ticket_gem_cost_index, DungeonInfo);
	//CounterDoc->SetNextBuyTicketGemCost(NextBuyTicketGem);

	//auto HighestLevel = GET_TUPLE_DATA(FB2ResponseGetCounterDungeonStatus::highest_difficulty_level_index, DungeonInfo);
	//CounterDoc->SetHighestLevel(HighestLevel);
}

FLobbyFriendScene::FLobbyFriendScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyFriendScene::~FLobbyFriendScene()
{

}

FContentsSubMainScene::FContentsSubMainScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FContentsSubMainScene::~FContentsSubMainScene()
{

}

FLobbyAdventureModSelectScene::FLobbyAdventureModSelectScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyAdventureModSelectScene::~FLobbyAdventureModSelectScene()
{

}

FLobbyHeroTowerReadyScene::FLobbyHeroTowerReadyScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyHeroTowerReadyScene::~FLobbyHeroTowerReadyScene()
{
	
}

void FLobbyHeroTowerReadyScene::OpenScene()
{
	FLobbySceneBase::OpenScene();

	//AMatineeActor* DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee && DefaultCameraMatinee->IsValidObj())
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}
}

void FLobbyHeroTowerReadyScene::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

void FLobbyHeroTowerReadyScene::SubscribeEvents_OnConstruct()
{
	DeliveryGetHeroTowerTicket = DeliveryGetHeroTowerClass<FB2ResponseGetHeroTowerPtr>::GetInstance().Subscribe([this](const FB2ResponseGetHeroTowerPtr& msgPtr)
	{
		this->SetUIData(msgPtr);

		//UB2UIDocHeroTower*	HeroTowerDoc = UB2UIDocHelper::GetDocHeroTower();
		//if (!HeroTowerDoc)
		//	return;
	});
}
void FLobbyHeroTowerReadyScene::UnsubscribeEvents_OnDestruct()
{
	//DeliveryGetHeroTowerClass<FB2ResponseGetHeroTowerPtr>::GetInstance().Unsubscribe(DeliveryGetHeroTowerTicket);
}

void FLobbyHeroTowerReadyScene::SetUIData(const FB2ResponseGetHeroTowerPtr& MsgPtr)
{
	//UB2UIDocHeroTower*	HeroTowerDoc = UB2UIDocHelper::GetDocHeroTower();
	//if (!HeroTowerDoc)
	//	return;

	//HeroTowerDoc->SetGetHeroTowerPtr(MsgPtr);
	//HeroTowerDoc->SetRankerList(MsgPtr->rankers, MsgPtr->from_ranking);

	//BladeIIGameImpl::GetClientDataStore().SetModRankerList(MsgPtr->rankers, MsgPtr->from_ranking,EB2GameMode::HeroTower);
}

FLobbyGuildScene::FLobbyGuildScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
	SetUIData();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}
}

FLobbyGuildScene::~FLobbyGuildScene()
{

}

void FLobbyGuildScene::Tick(float DeltaSeconds)
{
	/*MatineePos += MatineeReverse ? -DeltaSeconds : DeltaSeconds;

	if (MatineePos >= MatineeLength)
	{
		MatineeReverse = true;
		MatineePos = MatineeLength;
	}

	if (MatineePos <= 0.f )
	{
		MatineeReverse = false;
		MatineePos = 0.f;
	}

	auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	if (DefaultCameraMatinee)
		DefaultCameraMatinee->SetPosition(MatineePos);*/

}

void FLobbyGuildScene::OpenScene()
{
	FLobbySceneBase::OpenScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	MatineePos = 0;
	//	MatineeLength = DefaultCameraMatinee->MatineeData->InterpLength;

	//	MatineeReverse = false;
	//}
}

void FLobbyGuildScene::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

void FLobbyGuildScene::SubscribeEvents_OnConstruct()
{
	//DeliveryGetCreateGuildIDTicket = DeliveryReceiveCreateGuildClass<FB2ReceiveCreateGuild>::GetInstance().Subscribe([this](const FB2ReceiveCreateGuild& CreateGuild)
	//{
	//	ReceiveCreateGuild(CreateGuild);
	//});

	//DeliveryGetGuildInfoTicket = DeliveryReceiveGuildDetailInfoClass<FB2ReceiveGuildInfo>::GetInstance().Subscribe([this](const FB2ReceiveGuildInfo& GuildInfoData)
	//{
	//	ReceiveGuildDetailInfo(GuildInfoData);
	//});

	//DeliveryLeaveGuildTicket = DeliveryReceiveLeaveGuildClass<FB2ResponseLeaveGuildPtr>::GetInstance().Subscribe([this](const FB2ResponseLeaveGuildPtr& LeaveGuildInfo)
	//{
	//	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	//	check(GuildDoc)

	//		GuildDoc->SetLeaveGuildTime(LeaveGuildInfo->leave_guild_time);

	//	BladeIIGameImpl::GetClientDataStore().SetGuildAccountID(0);
	//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")), FText::Format(
	//			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Reject")), FText::FromString(BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->guild_name)), 0, true, true, EUIMsgPopupButtonGroup::Confirm
	//	);
	//	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::LobbyMain);

	//});

	//DeliveryAskJoinGuildTicket = DeliveryReceiveAskJoinGuildClass<>::GetInstance().Subscribe([this]() {
	//	SetGuildJoin();
	//});

	//DeliveryGuidJoinBattleTicket = DeliveryReceiveJoinGuildBattleClass<>::GetInstance().Subscribe([]() {
	//	BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->join_guild_battle = true;
	//	DevShowNotReadyYetMessage(TEXT("Guild_BattleRequestCompleteStatePopupMessage"));
	//});

	//DeliveryGuildAcceptGuildTicket = DeliveryAcceptInviteGuildClass<>::GetInstance().Subscribe([this]() {
	//	SetGuildJoin();
	//});

	//DeliveryGuildBattleHistoryTicket = DeliveryGuildBattleHistoryClass<FB2ResponseGuildBattleHistoryPtr>::GetInstance().Subscribe([this](const FB2ResponseGuildBattleHistoryPtr& HistoryData)
	//{
	//	auto* GuildHistory = UB2UIManager::GetInstance()->OpenUI<UB2UIGuildBattleHistory>(UIFName::GuildBattleHistory);

	//	check(GuildHistory);

	//	GuildHistory->OpenSetting(HistoryData);
	//	//HistoryData->battle_histories
	//});
}
void FLobbyGuildScene::UnsubscribeEvents_OnDestruct()
{
	DeliveryReceiveCreateGuildClass<FB2ReceiveCreateGuild>::GetInstance().Unsubscribe(DeliveryGetCreateGuildIDTicket);
	DeliveryReceiveGuildDetailInfoClass<FB2ReceiveGuildInfo>::GetInstance().Unsubscribe(DeliveryGetGuildInfoTicket);
	DeliveryReceiveLeaveGuildClass<FB2ResponseLeaveGuildPtr>::GetInstance().Unsubscribe(DeliveryLeaveGuildTicket);
	DeliveryReceiveAskJoinGuildClass<>::GetInstance().Unsubscribe(DeliveryAskJoinGuildTicket);
	DeliveryReceiveJoinGuildBattleClass<>::GetInstance().Unsubscribe(DeliveryGuidJoinBattleTicket);
	DeliveryAcceptInviteGuildClass<>::GetInstance().Unsubscribe(DeliveryGuildAcceptGuildTicket);
	DeliveryGuildBattleHistoryClass<FB2ResponseGuildBattleHistoryPtr>::GetInstance().Unsubscribe(DeliveryGuildBattleHistoryTicket);	
}

void FLobbyGuildScene::SetUIData()
{

}

void FLobbyGuildScene::SetGuildJoin()
{
	//UB2UIManager::GetInstance()->RemoveUISceneHistory(EUIScene::GuildJoinAndCreate);
	//BladeIIGameImpl::GetClientDataStore().SetGuildAccountID(BladeIIGameImpl::GetClientDataStore().GetOtherGuildInfo().GuildInfo->guild_id);
	////data_trader::Retailer::GetInstance().RequestGuildDetailInfo(BladeIIGameImpl::GetClientDataStore().GetOtherGuildInfo().GuildInfo->guild_id);
	//UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//	BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")), FText::Format(
	//	BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_JoinResultMsg")), FText::FromString(BladeIIGameImpl::GetClientDataStore().GetOtherGuildInfo().GuildInfo->guild_name)), 0, true, true, EUIMsgPopupButtonGroup::Confirm
	//	, FMsgPopupOnClick::CreateLambda([this]() {
	//	data_trader::Retailer::GetInstance().RequestGuildDetailInfo(0);
	//}));
}

void FLobbyGuildScene::SetGuildFormation(const TArray<b2network::B2BattleFormationPtr>& FormationPtr)
{
	//auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	//check(GuildDoc)

	//for (auto El : FormationPtr)
	//{
	//	//if (El->formation_type == b2network::B2BattleFormationType::BTBFT_Normal)
	//	//	GuildDoc->SetFormationNormalLevel(El->formation_level);
	//	//else if (El->formation_type == b2network::B2BattleFormationType::BTBFT_Offensive)
	//	//	GuildDoc->SetFormationAttLevel(El->formation_level);
	//	//else if (El->formation_type == b2network::B2BattleFormationType::BTBFT_Defensive)
	//	//	GuildDoc->SetFormationDefLevel(El->formation_level);

	//}
}

void FLobbyGuildScene::ReceiveCreateGuild(const FB2ReceiveCreateGuild& CreateGuild)
{
	//auto GuildInfo = GET_TUPLE_DATA(FB2ResponseReciveCreateGuild::guild_info_index, CreateGuild);

	//FGuildInfo	ClientGuildInfo;
	//ClientGuildInfo.GuildInfo = GuildInfo;

	//BladeIIGameImpl::GetClientDataStore().SetGuildAccountID(GuildInfo->guild_id);
	//BladeIIGameImpl::GetClientDataStore().SetMyGuildInfo(ClientGuildInfo);

	//auto UserDoc = UB2UIDocHelper::GetDocUser();

	//check(UserDoc)
	//BladeIIGameImpl::GetClientDataStore().AddGold(-BladeIIGameImpl::GetClientDataStore().GetGuildSetupGoldCost());
	//UserDoc->UpdateUserData();

	//UB2UIManager::GetInstance()->OpenUI(UIFName::MsgPopupGuildCreate);
	//UB2UIMsgPopupGuildPopup* GuildPopup = UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupGuildPopup>(UIFName::MsgPopupGuildCreate);

	//check(GuildPopup)

	//GuildPopup->SetGuildMark(GuildInfo->mark_index, GuildInfo->mark_color, GuildInfo->mark_bg_index, GuildInfo->mark_bg_color);
	//GuildPopup->SetGuildName(GuildInfo->guild_name);
	//GuildPopup->AddHandler(FMsgPopupOnClick::CreateLambda([](){
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::GuildCreate);
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::MsgPopupGuildCreate);
	//	UB2UIManager::GetInstance()->RemoveUISceneHistory(EUIScene::GuildJoinAndCreate);
	//	data_trader::Retailer::GetInstance().RequestGuildDetailInfo(/*BladeIIGameImpl::GetClientDataStore().GetUserGuildID()*/0);
	//}));
}

void FLobbyGuildScene::ReceiveGuildDetailInfo(const FB2ReceiveGuildInfo& GuildInfoData)
{
//	auto GuildInfo = GET_TUPLE_DATA(FB2ResponseGuildDetailInfo::guild_index, GuildInfoData);
//	auto GuildMemberArr = GET_TUPLE_DATA(FB2ResponseGuildDetailInfo::guild_members_index, GuildInfoData);
//	b2network::B2GuildBattleEntryPtr Entry = GET_TUPLE_DATA(FB2ResponseGuildDetailInfo::entry_index, GuildInfoData);
//
//	bool TunrReward = GET_TUPLE_DATA(FB2ResponseGuildDetailInfo::has_turn_reward_index, GuildInfoData);
//
//	auto GuildCharacterPower = GET_TUPLE_DATA(FB2ResponseGuildDetailInfo::character_powers_index, GuildInfoData);
//	
//	auto GuildDoc = UB2UIDocHelper::GetDocGuild();
//
//	if (!GuildDoc)
//		return;
//	GuildDoc->SetJoinGuildBattle(GuildInfo->join_guild_battle);
//	GuildDoc->SetTurnReward(TunrReward);
//	GuildDoc->SetGuildLv(GuildInfo->level);
//	GuildDoc->SetGuildExp(GuildInfo->exp);
//	GuildDoc->SetObtainMedal(GuildInfo->turn_obtain_medal);
//	GuildDoc->SetJoinGuildBattlememeber(GET_TUPLE_DATA(FB2ResponseGuildDetailInfo::joined_guild_battle_member_index, GuildInfoData));
//	GuildDoc->SetCharacterPower(GuildCharacterPower);
//
//	if (Entry)
//	{
//		GuildDoc->SetGuildBattleEntry(Entry->slots);
//	}
//
//	GuildDoc->SetGuildbattleStateNew(GET_TUPLE_DATA(FB2ResponseGuildDetailInfo::guild_battle_state_index, GuildInfoData));
//
//	//자 정렬합시다 높은순서부터
//	GuildMemberArr.Sort([](b2network::B2GuildMemberPtr Ele1, b2network::B2GuildMemberPtr Ele2)->bool{
//		if (Ele1->contribution < Ele2->contribution) 
//			return false;
//		else 
//			return true;
//	});
//
//
//	EGuildInfoType GuildType;
//
//	FGuildInfo	ClientGuildInfo;
//	ClientGuildInfo.GuildInfo = GuildInfo;
//	ClientGuildInfo.GuildMemberInfo = GuildMemberArr;
//
//	if (BladeIIGameImpl::GetClientDataStore().GetUserGuildID() == GuildInfo->guild_id)
//	{
//		int32 GuildRank = 0;
//		
//		// jwyim GuildFormation
//		/*const TArray<b2network::B2BattleFormationPtr>&  BattleFormation = GET_TUPLE_DATA(FB2ResponseGuildDetailInfo::battle_formations_index, GuildInfoData);
//		SetGuildFormation(BattleFormation);*/
//
//		for (auto Ele : GuildMemberArr)
//		{
//			if (Ele->character_summary->account_id == BladeIIGameImpl::GetClientDataStore().GetAccountId())
//			{
//				GuildRank = Ele->rank;
//				BladeIIGameImpl::GetGuildDataStore().SetCurrentGuildMemberInfo(Ele);
//				break;
//			}
//		}
//		BladeIIGameImpl::GetClientDataStore().SetMyGuildInfo(ClientGuildInfo);
//		BladeIIGameImpl::GetClientDataStore().SetUserGuildInfo(FUserGuildInfo(GuildInfo->guild_name, GuildInfo->mark_index, GuildInfo->mark_color, GuildInfo->mark_bg_index, GuildInfo->mark_bg_color, GuildRank));
//		auto GuildSkillArr = GET_TUPLE_DATA(FB2ResponseGuildDetailInfo::guild_skills_index, GuildInfoData);
//
//		for (auto GuildSkill : GuildSkillArr)
//		{
//			BladeIIGameImpl::GetClientDataStore().SetGuildSkillInfo(GuildSkill);
//		}
//
//		auto GuildMercenary = GET_TUPLE_DATA(FB2ResponseGuildDetailInfo::mercenaries_index, GuildInfoData);
//
//		BladeIIGameImpl::GetGuildDataStore().SetGuildMercenaryData(GuildMercenary);
//
//
//		GuildType = EGuildInfoType::MyGuild;
//		
//	}
//	else
//	{
//		UB2UIManager::GetInstance()->CloseUI(UIFName::RecommendGuildInfo);
//		BladeIIGameImpl::GetClientDataStore().SetOtherGuildInfo(ClientGuildInfo);
//		GuildType = EGuildInfoType::OtherGuild;
//	}
//
//	GuildDoc->SetGuildMainType(int32(GuildType));
//
//	if (UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::GuildMain)
//	{
//		auto* GuildMain = UB2UIManager::GetInstance()->GetUI<UB2UIGuildMain>(UIFName::GuildMain);
//		check(GuildMain);
//
//		GuildMain->GuildMainPageUpdate();
//	}
//	else if (UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::RaidMain) 
//	{
//		// [todo]
//		// 레이드 초대시에 길드원 받는것 때문에.. 레이드는 분리해두었는데,
//		// 다른 패널쪽에도 GuildDetailInfo 을 사용한다면 서버쪽에 프로토콜 분리 요청 혹은 예외처리를 신중히 해야할듯..
//	}
//	else if (UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting))
//	{
//		// 채팅 UI가 노출 중일경우 무시.
//	}
//	else
//	{
//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::GuildMain);
//	}
}

FLobbyGuildCreateAndJoinScene::FLobbyGuildCreateAndJoinScene(class FLobbySceneManager* OwnerSceneManager)
	:FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyGuildCreateAndJoinScene::~FLobbyGuildCreateAndJoinScene()
{

}

void FLobbyGuildCreateAndJoinScene::OpenScene()
{
	FLobbySceneBase::OpenScene();

	//auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	//check(GuildDoc)

	//if(GuildDoc->GetGuildOutState())			//길드에서 추방당했을경우 팝업을 띄워주자~~
	//{
	//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")), FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_DeportationMessage")),
	//			FText::FromString(BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->guild_name)),
	//		0, true, true, EUIMsgPopupButtonGroup::Confirm
	//		);

	//	GuildDoc->SetGuildOutState(false);			
	//}
}

void FLobbyGuildCreateAndJoinScene::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

FLobbyCollectBookMain::FLobbyCollectBookMain(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
	SceneManager = OwnerSceneManager;
	check(SceneManager);
	//CurrentCharacter = nullptr;
	
}

FLobbyCollectBookMain::~FLobbyCollectBookMain()
{

}

void FLobbyCollectBookMain::OpenScene()
{
	FLobbySceneBase::OpenScene();

	//// UI Init() 이 불린 후에 OpenScene이 되므로 처음 진입 한번은 로비캐릭터 셋팅을 해 주어야 함
	//UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome();
	//if (DocSome != nullptr)
	//{
	//	EPCClass CurrentCollectBookClass = DocSome->GetCollectBookSelectClass();
	//
	//	auto& CollectBookCharacters = GetLobbyCharactersForGTypeLevel();
	//	for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(CollectBookCharacters); It; ++It)
	//	{	
	//		if (It.Value().ActorForMain)
	//		{
	//			CharacterHiddenAndStopSound(It.Value().ActorForMain, true);
	//			It.Value().ActorForMain->SetActorRelativeRotation(It.Value().InitialRotation);
	//		}
	//	}


	//	SelectCollectBookCharacter(CurrentCollectBookClass);

	//	ZoomModule.InitializeModule(this);
	//	ZoomModule.ChangeZoomTarget(CurrentCharacter);

	//	FLobbyCharacterInfo* CurrentLobbyCharacter = CollectBookCharacters.Find(CurrentCollectBookClass);
	//	if (CurrentLobbyCharacter)
	//	{
	//		GetSceneManager()->SetupLobbyCharCompForParts(CurrentSelectedCharacter, CurrentLobbyCharacter->ActorForMain, true);
	//	}
	//}

	//auto* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->BindTouchInput(true);
	//	LobbyGameMode->SetEnableClickEvents(true);
	//}

}

void FLobbyCollectBookMain::CloseScene()
{
	//FLobbySceneBase::CloseScene();

	//auto& CollectBookCharacters = GetLobbyCharactersForGTypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(CollectBookCharacters); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//	{
	//		It.Value().ActorForMain->SetActorRelativeRotation(It.Value().InitialRotation);
	//	}
	//}
	//ZoomModule.ClearZoomModule();
}

void FLobbyCollectBookMain::SubscribeEvents_OnConstruct()
{
	//LobbyCharRotateCharacterTicket = LobbyCharRotateCharacterClass<float>::GetInstance().Subscribe([this](float Value) { this->RotateCharacterYaw(Value); });
	//LobbyCharEquippedItemTicket = LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Subscribe([this](EPCClass CharClass, EItemEquipPlace EquippedPlace) { this->OnCollectItemEquip(CharClass, EquippedPlace); });
	//LobbyCharCollectBookSelectTicket = LobbyCharCollectBookSelectClass<EPCClass>::GetInstance().Subscribe([this](EPCClass CharClass) { this->SelectCollectBookCharacter(CharClass); });
	//LobbyCharZoomTicket = LobbyCharZoomClass<const FVector2D&, float>::GetInstance().Subscribe([this](const FVector2D& Pinpoint, float Delta) { this->ZoomModule.Zoom(Pinpoint, Delta); });
}
void FLobbyCollectBookMain::UnsubscribeEvents_OnDestruct()
{
	//LobbyCharRotateCharacterClass<float>::GetInstance().Unsubscribe(LobbyCharRotateCharacterTicket);
	//LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Unsubscribe(LobbyCharEquippedItemTicket);
	//LobbyCharCollectBookSelectClass<EPCClass>::GetInstance().Unsubscribe(LobbyCharCollectBookSelectTicket);
	//LobbyCharZoomClass<const FVector2D&, float>::GetInstance().Unsubscribe(LobbyCharZoomTicket);
}

void FLobbyCollectBookMain::OnCollectItemEquip(EPCClass CharClass, EItemEquipPlace EquippedPlace)
{
	//if (CurrentSelectedCharacter != CharClass)
	//	return;

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForGTypeLevel();

	//auto* LobbyCharInfo = LobbyCharactersForALobbyType.Find(CurrentSelectedCharacter);
	//if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
	//{
	//	LobbyCharInfo->ActorForMain->PlayEquipAnimation(EquippedPlace);
	//	GetSceneManager()->SetupLobbyCharCompForParts(CurrentSelectedCharacter, LobbyCharInfo->ActorForMain, true);
	//}
}

void FLobbyCollectBookMain::SelectCollectBookCharacter(EPCClass SelectedCharacter)
{
	//// 이전 Character는 숨김처리 후 시작
	//auto& LobbyCharactersForGLobbyType = GetLobbyCharactersForGTypeLevel();
	//auto* LobbyCharInfo = LobbyCharactersForGLobbyType.Find(CurrentSelectedCharacter);
	//if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
	//{
	//	CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, true);
	//}

	//// 새로운 Character Visible 처리 시작
	//LobbyCharInfo = LobbyCharactersForGLobbyType.Find(SelectedCharacter);
	//if (LobbyCharInfo)
	//{
	//	CurrentSelectedCharacter = SelectedCharacter;
	//	CurrentCharacter = LobbyCharInfo->ActorForMain;

	//	AMatineeActor* CollectBookMatinee = nullptr;
	//	float EventKeyTime = 0.f; //도감용 Matinee EventKeyTime 따로 없음
	//	if (LobbyCharInfo->MatineeForMain)
	//		CollectBookMatinee = LobbyCharInfo->MatineeForCollectBook;

	//	if (CollectBookMatinee && CollectBookMatinee->IsValidObj())
	//	{
	//		CollectBookMatinee->Stop();
	//		CollectBookMatinee->Play();
	//		SetCameraMatineeCam(CollectBookMatinee, EventKeyTime);
	//		CollectBookMatinee->SetPosition(EventKeyTime);
	//	}

	//	CurrentCharacter->SetActorRelativeRotation(LobbyCharInfo->InitialRotation);
	//	CharacterHiddenAndStopSound(CurrentCharacter, false);
	//	if (CurrentCharacter && !CurrentCharacter->bHidden)
	//	{
	//		CurrentCharacter->PlaySelectAnimation(ELobbyScene::ELobbyScene_HeroManagement);
	//	}
	//}
}
void FLobbyCollectBookMain::RotateCharacterYaw(float Value)
{
//	if (CurrentCharacter)
//	{
//#if CHAR_ANIM_ROTATION
//		CurrentCharacter->PlayTurnAnimation(Value < 0.f);
//#elif CHAR_ACTOR_ROTATION
//		
//	FRotator Rotation = CurrentCharacter->GetActorRotation();
//	Rotation += FRotator(0.f, Value * 2.0f, 0.f);
//	CurrentCharacter->SetActorRotation(Rotation);
//
//#endif
//	}
}


FLobbySummonItemScene::FLobbySummonItemScene(class FLobbySceneManager* OwnerSceneManager)
	:FLobbySceneBase(OwnerSceneManager)
{
	CachedShopGirlReservedState = ELobbyShopGirlState::ELSG_IdleLoop;
}

FLobbySummonItemScene::~FLobbySummonItemScene()
{
	
}

void FLobbySummonItemScene::Tick(float DeltaSeconds)
{
	//if (CachedItemSceneMatinee.IsValid())
	//{ // 가끔씩.. 어디선가 신호 보낸 게 뒤늦게 오기라도 하는지 시간 지나고 나서 카메라가 휙 바뀌는 경우가 있어서 좀 뻘짓.
	//	SetCameraMatineeCam(CachedItemSceneMatinee.Get());
	//}
}

void FLobbySummonItemScene::SubscribeEvents_OnConstruct()
{
	/*DeliveryGetLotteryShopTicket = DeliveryGetLotteryShopClass<FB2ResponseGetLotteryShopPtr>::GetInstance().Subscribe([this](const FB2ResponseGetLotteryShopPtr& LotteryShop)
	{
		this->SetSummonItemUIData(LotteryShop);

		if (GetSceneManager()->GetCurrentLobbyScene() != GetLobbyScene())
		{
			LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::SummonItemStore);
		}
		else
		{
			UB2UISummonItem* SummonItemUI = UB2UIManager::GetInstance()->GetUI<UB2UISummonItem>(UIFName::SummonItemStore);
			if (SummonItemUI)
			{
				SummonItemUI->SetTab();
			}
		}
	});

	HandleServerError7237Ticket = HandleServerError7237Class<>::GetInstance().Subscribe(
		[this]()
	{
		data_trader::Retailer::GetInstance().RequestGetLotteryShop();
	});*/
}

void FLobbySummonItemScene::UnsubscribeEvents_OnDestruct()
{
	//DeliveryGetLotteryShopClass<FB2ResponseGetLotteryShopPtr>::GetInstance().Unsubscribe(DeliveryGetLotteryShopTicket);
	//HandleServerError7237Class<>::GetInstance().Unsubscribe(HandleServerError7237Ticket);
}

void FLobbySummonItemScene::OpenScene()
{
	//CachedShopGirlReservedState = ELobbyShopGirlState::ELSG_IdleLoop;
	//FLobbySceneManager* LobbySM = GetSceneManager();
	//if (LobbySM)
	//{ // LobbySceneManager 의 ShopGirlReservedState 을 잽싸게 캐싱해 놓고 리셋
	//	CachedShopGirlReservedState = LobbySM->GetShopGirlReservedState();

	//	// OpenScene 을 통해 들어올 때 Cheer 가 아니면 처음에는 환영 애니메이션이 되어야 함.
	//	if (CachedShopGirlReservedState != ELobbyShopGirlState::ELSG_CheerNormal && CachedShopGirlReservedState != ELobbyShopGirlState::ELSG_CheerMuch)
	//	{
	//		CachedShopGirlReservedState = ELobbyShopGirlState::ELSG_Welcoming;
	//	}

	//	LobbySM->ResetShopGirlReservedState();
	//}

	//FLobbySceneBase::OpenScene();

	//AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // 안전빵.

	//	// 상점 소녀도 터치 액션이 있어서 입력 바인딩
	//	LobbyGameMode->BindTouchInput(true);
	//	LobbyGameMode->SetEnableClickEvents(true);
	//}

	//AMatineeActor* DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee)
	//{
	//	CachedItemSceneMatinee = DefaultCameraMatinee;

	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}
	//// 배치해 놓은 상점 소녀 액터
	//AB2LobbyShopGirl* ShopGirActor = LobbySM ? Cast<AB2LobbyShopGirl>(LobbySM->GetNPCSKActor(ELobbyNPCType::ELobbyNPC_ShopGirl)) : nullptr;
	//if (ShopGirActor && CachedShopGirlReservedState != ELobbyShopGirlState::ELSG_End)
	//{	
	//	if (CachedShopGirlReservedState == ELobbyShopGirlState::ELSG_IdleLoop)
	//	{ // 뽑기 결과에 대한 쑈가 아닌 다음에야 Welcoming 이 될 테니 여기로 들어올 일이 있지는 않겠다.
	//		ShopGirActor->BeginDefaultPose(); // Idle 이면 PlayTemporary 가 아니고 루핑으로 계속 돌려야 되는 거.
	//	}
	//	else
	//	{ // 쇼타임
	//		ShopGirActor->PlayTemporaryAnimOfState(CachedShopGirlReservedState);
	//	}
	//}

	//if (LobbySM && !LobbySM->IsSceneStreamingLevelLoaded(ELobbyScene::ELobbyScene_SummonItemDisplay))
	//{ // 뽑기 장면을 미리 로딩해 두는데 flush 로 인한 부작용 가능성을 낮추기 위해 AnimStateGuard 를 둔다. 
	//	// 이걸 안 하면 특히나 터치 시 눈에 띄는 프리징이 발생할 수 있다.
	//	// 그리고 처음 일정 시간은 Welcoming 모션이 확실하게 보이도록 하는 의미도 될 수 있고.
	//	ShopGirActor->SetAnimStateGuard(1.5f);
	//	LobbySM->PrefetchSceneStreamingLevel(ELobbyScene::ELobbyScene_SummonItemDisplay);
	//}
}
void FLobbySummonItemScene::CloseScene()
{
	//CachedItemSceneMatinee = nullptr;

	//// 상점 소녀도 터치 액션이 있어서 입력 바인딩
	//AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->BindTouchInput(false);
	//	LobbyGameMode->SetEnableClickEvents(false);
	//}

	//FLobbySceneBase::CloseScene();

	//CachedShopGirlReservedState = ELobbyShopGirlState::ELSG_IdleLoop;
}

void FLobbySummonItemScene::SetSummonItemUIData(const FB2ResponseGetLotteryShopPtr& ShopDataPtr)
{
	//for (auto TimeData : ShopDataPtr->shop_lotteries)
	//{
	//	int32 CostType = TimeData->draw_price_type - b2network::B2ShopDrawPriceType::GEM;
	//	int32 HashKey = FSummonItemSlotData::GetHashKey(CostType, TimeData->is_continuous, false);

	//	BladeIIGameImpl::GetClientDataStore().SetStoreSummonItemFreeTime(HashKey, TimeData->next_free_time);
	//	BladeIIGameImpl::GetClientDataStore().SetStoreSummonItemDailyCount(HashKey, TimeData->lottery_count);

	//	HashKey = FSummonItemSlotData::GetHashKey(CostType, TimeData->is_continuous, true);

	//	BladeIIGameImpl::GetClientDataStore().SetStoreSummonItemFreeTime(HashKey, TimeData->next_free_time);
	//	BladeIIGameImpl::GetClientDataStore().SetStoreSummonItemDailyCount(HashKey, TimeData->lottery_count);
	//}

	//auto* DocStore = UB2UIDocHelper::GetDocStore();
	//if (DocStore)
	//{
	//	DocStore->SetSummonItemMileage(ShopDataPtr->mileage);
	//	DocStore->SetSummonItemMileageReceivedIndex(ShopDataPtr->reward_index - 1);
	//}
}

FLobbyGachaDisplaySceneBase::FLobbyGachaDisplaySceneBase(class FLobbySceneManager* OwnerSceneManager)
	:FLobbySceneBase(OwnerSceneManager)
{
}

FLobbyGachaDisplaySceneBase::~FLobbyGachaDisplaySceneBase()
{
}

void FLobbyGachaDisplaySceneBase::OpenScene()
{
	FLobbySceneBase::OpenScene();

	PlaySummonItemMatinee();
}

void FLobbyGachaDisplaySceneBase::PlaySummonItemMatinee()
{
	//if (GetSceneManager())
	//{
	//	auto StoreDoc = UB2UIDocHelper::GetDocStore();

	//	if (!StoreDoc)
	//		return;

	//	bool bUseSocial = false;
	//	FSummonItemSlotData TabMenu = FSummonItemSlotData(StoreDoc->GetSelectBoxHashKey(), 0);

	//	if (TabMenu.GetCostType() == int32(ESummonItemTabMenu::FriendShipBox))
	//		bUseSocial = true;

	//	GetSceneManager()->PlaySummonItemDisplayMatinee(CurrentSummonedItems.AddedItems.Num() > 1, false, bUseSocial);
	//}
}

void FLobbyGachaDisplaySceneBase::PlayOpenSummonItemMatinee(bool InIsMulti)
{
	////카드 뒤집을때는 쓰는 함수
	//if (GetSceneManager())
	//{
	//	auto StoreDoc = UB2UIDocHelper::GetDocStore();

	//	if (!StoreDoc)
	//		return;

	//	bool bUseSocial = false;
	//	FSummonItemSlotData TabMenu = FSummonItemSlotData(StoreDoc->GetSelectBoxHashKey(), 0);
	//	if (TabMenu.GetCostType() == int32(ESummonItemTabMenu::FriendShipBox))
	//		bUseSocial = true;

	//	GetSceneManager()->PlaySummonItemDisplayMatinee(InIsMulti, true, bUseSocial);
	//}
}

//#include "B2UISummonItem.h"

//
//void FLobbyGachaDisplaySceneBase::OnFinishedSummonItemDisplayMatinee(AMatineeActor* Matinee)
//{
//	UB2UISummonItemDisplay* DisplayUI = UB2UIManager::GetInstance()->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);
//	if (DisplayUI)
//	{
//		DisplayUI->UpdateLocationSummonItemUIs();
//	}
//}
//
//void FLobbyGachaDisplaySceneBase::OnChangedToFxSummonItemDisplayMatinee(AMatineeActor* Matinee)
//{
//	UB2UISummonItemDisplay* DisplayUI = UB2UIManager::GetInstance()->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);
//	if (DisplayUI)
//	{
//		check(CurrentSummonedItems.AddedItems.Num() > 0);
//		DisplayUI->ChangeSummonItemCardMatineeActorToFX(CurrentSummonedItems.AddedItems, Matinee);
//	}
//}

void FLobbyGachaDisplaySceneBase::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//CurrentSummonedItems.AddedItems.Empty();
	//if (GEngine->ActiveMatinee.IsValid() && GEngine->ActiveMatinee.Get()->GetWorld())	//마티네가 꼬이고있습니다... 돌고있는 마티네 무조건 멈춰주고 돌려줍시다.
	//	GEngine->ActiveMatinee.Get()->Stop();
}

FLobbySummonItemDisplayScene::FLobbySummonItemDisplayScene(class FLobbySceneManager* OwnerSceneManager)
	:FLobbyGachaDisplaySceneBase(OwnerSceneManager)
{

}

FLobbySummonItemDisplayScene::~FLobbySummonItemDisplayScene()
{

}

void FLobbySummonItemDisplayScene::SubscribeEvents_OnConstruct()
{
	//SummonitemOpenCardMatineePlayTicket = SummonitemOpenCardMatineePlayClass<int32>::GetInstance().Subscribe([this](int32 Count)
	//{
	//	auto StoreDoc = UB2UIDocHelper::GetDocStore();

	//	if (!StoreDoc)
	//		return;

	//	bool bUseSocial = false;

	//	FSummonItemSlotData TabMenu = FSummonItemSlotData(StoreDoc->GetSelectBoxHashKey(), 0);
	//	if (TabMenu.GetCostType() == int32(ESummonItemTabMenu::FriendShipBox))
	//		bUseSocial = true;


	//	if (this->GetSceneManager())
	//		this->GetSceneManager()->PlaySummonItemDisplayMatinee(Count == 1 ? false : true, true , bUseSocial);
	//});

	////DeliveryDrawShopLotteryTicket = DeliveryDrawShopLotteryClass<FB2ResponseDrawShopLotteryPtr>::GetInstance().Subscribe([this](const FB2ResponseDrawShopLotteryPtr& SummonItemResult)
	//{
	//	this->SetSummonedItemResult(SummonItemResult);
	//});
}
void FLobbySummonItemDisplayScene::UnsubscribeEvents_OnDestruct()
{
	SummonitemOpenCardMatineePlayClass<int32>::GetInstance().Unsubscribe(SummonitemOpenCardMatineePlayTicket);
	DeliveryDrawShopLotteryClass<FB2ResponseDrawShopLotteryPtr>::GetInstance().Unsubscribe(DeliveryDrawShopLotteryTicket);	
}

void FLobbySummonItemDisplayScene::SetSummonedItemResult(const FB2ResponseDrawShopLotteryPtr& SummonResult)
{
	//CurrentSummonedItems.AddedItems.Empty(SummonResult->items.Num());
	//
	////인벤에 넣기
	//auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	//FLobbySceneManager* LobbySM = GetSceneManager();
	//if (LobbySM)
	//{
	//	LobbySM->SetShopGirlReservedState(ELobbyShopGirlState::ELSG_CheerNormal); // 못해도 기본빵으로 하나는 보여준다
	//}
	//for (auto& SvrItem : SummonResult->items)
	//{
	//	int32 AddedIndex = CurrentSummonedItems.AddedItems.AddDefaulted();
	//	check(AddedIndex != INDEX_NONE);

	//	FB2Item& CurrAddedRef = CurrentSummonedItems.AddedItems[AddedIndex];
	//	CurrAddedRef = SvrItem;
	//	if (CurrAddedRef.StarGrade >= ITEM_SUMMON_JACKPOT_GRADE_LV1 && LobbySM)
	//	{
	//		LobbySM->SetShopGirlReservedState(ELobbyShopGirlState::ELSG_CheerMuch); // 섹시댄스 예약.
	//	}
	//}

	//ClientDataStore.AddNewAcquiredItems(CurrentSummonedItems.AddedItems);

	//auto StoreDoc = UB2UIDocHelper::GetDocStore();

	///*UB2UISummonItem* SummonItemUI = UB2UIManager::GetInstance()->GetUI<UB2UISummonItem>(UIFName::SummonItemStore);
	//BII_CHECK(SummonItemUI);*/
	//if (!StoreDoc)
	//	return;

	//// 우정 포인트로 뽑기
	//if (SummonResult->social_point != ClientDataStore.GetSocialPoint())
	//{
	//	ClientDataStore.UseSocialPoint();
	//}

	////마일리지 및 재화등 Update
	//if (SummonResult->free_lottery) //[@AKI, 171124] SummonResult->free_lottery == true일때는 서버에서  DB조회를 할 필요가 없기 때문에 next_free_time제외하고는 값 갱신을 안함.그래서 클라도 그와 동일하게 구현함. ex> gem = 0, social_point = 0
	//{
	//	if (SummonResult->next_free_time != 0)
	//		ClientDataStore.SetStoreSummonItemFreeTime(StoreDoc->GetSelectBoxHashKey(), SummonResult->next_free_time);
	//}
	//else 
	//{
	//	ClientDataStore.SetShopTenLotteryTicket(SummonResult->shop_ten_lottery_ticket);
	//	ClientDataStore.SetGemAmount(SummonResult->gem);
	//	ClientDataStore.SetSocialPoint(SummonResult->social_point);
	//}

	//ClientDataStore.SetStoreSummonItemDailyCount(StoreDoc->GetSelectBoxHashKey(), SummonResult->daily_lottery_count);

	//// 한번 더 뽑기를 위한 검증된 키
	//StoreDoc->SetLastLotteryKey(SummonResult->last_lottery_key);

	//ChangeUserAccountInfoClass<>::GetInstance().Signal();

	//auto* DocStore = UB2UIDocHelper::GetDocStore();
	//if (DocStore && SummonResult->mileage != 0)
	//{
	//	DocStore->SetMileagePoint(DocStore->GetCurrentMileageType(), SummonResult->mileage);
	//}

	//SummonDisplayDifficulty = UB2UISummonItem::GetCurrentStageDifficulty();
	//	
	//if (GetSceneManager())
	//{

	//	UB2UISummonItemDisplay* DisplayUI = UB2UIManager::GetInstance()->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);

	//	if (GetSceneManager()->GetCurrentLobbyScene() != GetLobbyScene())
	//	{
	//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::SummonItemResult);
	//		if (DisplayUI)
	//			DisplayUI->SetSummonedItemFrom(ESummonedItemFrom::SummonItemShop);
	//	}
	//	else
	//	{
	//		if (DisplayUI)
	//			DisplayUI->Init();

	//		PlaySummonItemMatinee();
	//	}
 //		DisplayUI = UB2UIManager::GetInstance()->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);

	//	if (DisplayUI)
	//	{
	//		const int32 AddedItems = CurrentSummonedItems.AddedItems.Num();
	//		DisplayUI->SetItemMaxNumber(AddedItems);
	//		DisplayUI->OnStartDrawShopLottery(AddedItems);
	//	}
	//}
}

FLobbyGeneralLotteryDisplayScene::FLobbyGeneralLotteryDisplayScene(class FLobbySceneManager* OwnerSceneManager)
	:FLobbyGachaDisplaySceneBase(OwnerSceneManager)
{

}

FLobbyGeneralLotteryDisplayScene::~FLobbyGeneralLotteryDisplayScene()
{
	
}

void FLobbyGeneralLotteryDisplayScene::SubscribeEvents_OnConstruct()
{
	//DeliveryGeneralLotteryItemResultTicket = DeliveryGeneralLotteryItemResultClass<FB2Item, ESummonItemDifficulty>::GetInstance().Subscribe([this](const FB2Item& SummonItem, ESummonItemDifficulty Difficulty)
	//{
	//	this->SetGeneralLotteryItemResult(SummonItem, Difficulty);
	//});
}
void FLobbyGeneralLotteryDisplayScene::UnsubscribeEvents_OnDestruct()
{
	//DeliveryGeneralLotteryItemResultClass<FB2Item, ESummonItemDifficulty>::GetInstance().Unsubscribe(DeliveryGeneralLotteryItemResultTicket);
}

void FLobbyGeneralLotteryDisplayScene::SetGeneralLotteryItemResult(const FB2Item& SummonItemUsingLottery, ESummonItemDifficulty InSummonItemDifficulty)
{
	CurrentSummonedItems.AddedItems.Empty();

	//인벤에 넣기
	//CurrentSummonedItems.AddedItems.Add(SummonItemUsingLottery);
	//BladeIIGameImpl::GetClientDataStore().AddNewAcquiredItems(CurrentSummonedItems.AddedItems);

	//SummonDisplayDifficulty = InSummonItemDifficulty;

	//if (GetSceneManager())
	//{
	//	if (GetSceneManager()->GetCurrentLobbyScene() != GetLobbyScene())
	//	{
	//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::GeneralLotteryResult);
	//		UB2UISummonItemDisplay* DisplayUI = UB2UIManager::GetInstance()->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);
	//		if (DisplayUI)
	//		{
	//			DisplayUI->SetSummonedItemFrom(ESummonedItemFrom::LotteryMail);
	//		}
	//	}
	//	else
	//		PlaySummonItemMatinee();
	//}
}

FLobbyGuildMapUIScene::FLobbyGuildMapUIScene(class FLobbySceneManager* OwnerSceneManager)
	:FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyGuildMapUIScene::~FLobbyGuildMapUIScene()
{

}

void FLobbyGuildMapUIScene::OpenScene()
{
	FLobbySceneBase::OpenScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	FirstMatineePlay = true;
	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}

	//auto* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->SetEnableClickEvents(true);
	//}

}

void FLobbyGuildMapUIScene::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->SetEnableClickEvents(false);
	//}
}

void FLobbyGuildMapUIScene::SubscribeEvents_OnConstruct()
{
	//DeliveryGetGuildBattleInfoTicket = DeliveryGetGuildBattleInfoClass<FB2ResponseGetGuildBattlePtr>::GetInstance().Subscribe([this](const FB2ResponseGetGuildBattlePtr GuildBattleInfo) {
	//	this->SetGuildBattleInfo(GuildBattleInfo);
	//});

	//DeliveryGuildBattleTurnResultTicket = DeliveryGuildBattleTurnResultClass<FB2ResponseGuildBattleTurnResultPtr>::GetInstance().Subscribe([this](const FB2ResponseGuildBattleTurnResultPtr& Result) {
	//	this->OpenGuildBattleTurnResult(Result);
	//});

	//GuildStateChangeEventTicket = GuildStateChangeEventClass<int32>::GetInstance().Subscribe([this](int32 Lv)
	//{
	//	AMatineeActor* ChangeMatinee;

	//	if (Lv % 2)
	//	{
	//		ChangeMatinee = GuildMapChangeB;
	//	}
	//	else
	//	{
	//		ChangeMatinee = GuildMapChangeA;
	//	}

	//	if (ChangeMatinee && ChangeMatinee->IsValidObj())
	//	{
	//		ChangeMatinee->SetPosition(ChangeMatinee->MatineeData->InterpLength);
	//		ChangeMatinee->Reverse();
	//		SetCameraMatineeCam(ChangeMatinee);
	//		ChangeState = true;
	//	}
	//});

	//DeliveryStartGuildBattleTicket = DeliveryStartGuildBattleClass<FB2ResponseStartGuildBattlePtr>::GetInstance().Subscribe([this](const FB2ResponseStartGuildBattlePtr& GuildBattleStartInfo) {

	//	auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	//	if (!GuildDoc)
	//		return;

	//	GuildDoc->SetOtherGuildMemeberCharInfo(GuildBattleStartInfo->target);
	//	GuildDoc->SetAllyGuildMemeberCharInfo(GuildBattleStartInfo->my_info);
	//	GuildDoc->SetBattleToken(GuildBattleStartInfo->battle_token);
	//	//GuildBuffSetttingClass<const TArray<b2network::B2GuildSkillPtr>&>::GetInstance().Signal(GuildBattleStartInfo->guild_skills);

	//	StartBladeIIGuildMatchGame(this->GetLobbyGameMode());

	//});
}
void FLobbyGuildMapUIScene::UnsubscribeEvents_OnDestruct()
{
	DeliveryGetGuildBattleInfoClass<FB2ResponseGetGuildBattlePtr>::GetInstance().Unsubscribe(DeliveryGetGuildBattleInfoTicket);
	DeliveryGuildBattleTurnResultClass<FB2ResponseGuildBattleTurnResultPtr>::GetInstance().Unsubscribe(DeliveryGuildBattleTurnResultTicket);
	GuildStateChangeEventClass<int32>::GetInstance().Unsubscribe(GuildStateChangeEventTicket);
	DeliveryStartGuildBattleClass<FB2ResponseStartGuildBattlePtr>::GetInstance().Unsubscribe(DeliveryStartGuildBattleTicket);
}

void FLobbyGuildMapUIScene::Tick(float DeltaSeconds)
{

}

void FLobbyGuildMapUIScene::SetGuildBattleInfo(const FB2ResponseGetGuildBattlePtr& GuildBattleInfo)
{
	//auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	//check(GuildDoc);
	//GuildDoc->SetGuildBattleEndTime(GuildBattleInfo->battle_end_time);
	//
	//auto& ClientData = BladeIIGameImpl::GetClientDataStore();

	//ClientData.SetMyGuildBattleInfo(GuildBattleInfo->my_guild);
	//ClientData.SetOtherGuildBattleInfo(GuildBattleInfo->target_guild);


	//auto GuildMapUIInfo = UB2UIManager::GetInstance()->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

	//if (GuildMapUIInfo)
	//{
	//	GuildMapUIInfo->RefreshInfo();
	//}


	//LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::GuildMapUI);
}

//#include "B2UIGuildBattleStatistics.h"
void FLobbyGuildMapUIScene::OpenGuildBattleTurnResult(const FB2ResponseGuildBattleTurnResultPtr& GuildBattleResult)
{
	//auto* UIManager = UB2UIManager::GetInstance();
	//if (UIManager)
	//{
	//	auto* GuildBattleStatisticUI = UIManager->OpenUI<UB2UIGuildStatistics_Main>(UIFName::GuildBattleStatistics, true);
	//	check(GuildBattleStatisticUI);



	//	//결과창 PowrIndex 순으로 정렬하자
	//	GuildBattleResult->my_guild->members.Sort([](b2network::B2GuildBattleMemberPtr Ele1 , b2network::B2GuildBattleMemberPtr Ele2)->bool {
	//		if (Ele1->power_index > Ele2->power_index)
	//			return false;
	//		else
	//			return true;
	//	});

	//	GuildBattleResult->target_guild->members.Sort([](b2network::B2GuildBattleMemberPtr Ele1, b2network::B2GuildBattleMemberPtr Ele2)->bool {
	//		if (Ele1->power_index > Ele2->power_index)
	//			return false;
	//		else
	//			return true;
	//	});
	//	
	//	GuildBattleStatisticUI->SetData(GuildBattleResult);
	//}
}

void FLobbyGuildMapUIScene::SetGuildMapChangeMatinee(class AMatineeActor* Atype, class AMatineeActor* Btype)
{
	GuildMapChangeA = Atype;
	GuildMapChangeB = Btype;
}

void FLobbyGuildMapUIScene::GuildChangeMatineeEnd()
{
	//auto* UIManager = UB2UIManager::GetInstance();
	//if (!UIManager)
	//	return;

	//auto* GuildMapUI = UIManager->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);
	//if (!GuildMapUI)
	//	return;


	//if (FirstMatineePlay)
	//{
	//	FirstMatineePlay = false;
	//	ChangeState = false;
	//	GuildMapUI->MedalMoveUI();
	//	GuildMapUI->UIHidden(false);
	//	return;
	//}

	//if (ChangeState)
	//{
	//	GuildMapUI->EnterMatineePlay(GuildMapChangeA, GuildMapChangeB);
	//	ChangeState = false;
	//}
	//else
	//{
	//	GuildMapUI->UIHidden(false);
	//	GuildMapUI->MedalMoveUI();
	//}
}

FLobbyRaidMainScene::FLobbyRaidMainScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
	SlotIndexs.AddZeroed(4);

}

FLobbyRaidMainScene::~FLobbyRaidMainScene()
{

}

void FLobbyRaidMainScene::OpenScene()
{
	FLobbySceneBase::OpenScene();
	

	//auto* RaidUI = UB2UIManager::GetInstance()->GetUI<UB2UIRaidMain>(UIFName::Raid);
	//auto* UIDocRaid = UB2UIDocHelper::GetDocRaid();
	//auto* DocBattle = UB2UIDocHelper::GetDocBattle();

	//if (RaidUI && UIDocRaid && DocBattle)
	//{
	//	const ERaidGotoWhere MatchType = UIDocRaid->GetReturnToPageType();
	//	const bool IsRematch = (MatchType == ERaidGotoWhere::Rematch) || (MatchType == ERaidGotoWhere::PartyRematch);
	//	int32 RaidType = UIDocRaid->GetLastRaidType();
	//	int32 RaidStep = UIDocRaid->GetLastRaidStep();
	//	ERaidJoinType JoinType = UIDocRaid->GetRaidJoinType();
	//	EPCClass PcClass = IntToPCClass(DocBattle->GetCurPCClass());

	//	// 1. 초대하기
	//	if (UIDocRaid->GetIsReceiveInvite()) // 초대하기로 입장.
	//	{
	//		JoinType = ERaidJoinType::MakeRoom;
	//		RaidUI->ForcedStartGame(RaidType, RaidStep, JoinType, PcClass, false);
	//	}
	//	// 2. 자동 매칭
	//	else if(IsRematch)
	//	{
	//		RaidUI->ForcedStartGame(RaidType, RaidStep, JoinType, PcClass, true);

	//	}
	//	// 3. 일반적인 입장.
	//	else
	//	{
	//		RaidUI->SetButtonEnable(true);
	//	}

	//	UIDocRaid->SetRaidJoinType(JoinType);
	//	UIDocRaid->SetReturnToPageType(ERaidGotoWhere::Lobby);
	//}
}

void FLobbyRaidMainScene::CloseScene()
{
	FLobbySceneBase::CloseScene();

	RemoveSlotIndex();

	//if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
	//{
	//	DocRaid->SetForceStartRaid(false);

	//	// 팀원들과 다시하기 누른게 아니면 Raid MatchMaking 때 받은 정보들은 무시한다.
	//	if(DocRaid->GetIsRequestPartyRematch() == false)
	//		DocRaid->ClearPartyRetryInfo();
	//}
}

void FLobbyRaidMainScene::SubscribeEvents_OnConstruct()
{
	//DeliveryRaidMatchmakingTicket = DeliveryRaidMatchmakingClass<FB2ResponseRaidMatchmakingPtr>::GetInstance().Subscribe([this](const FB2ResponseRaidMatchmakingPtr& Message) 
	//{ 
	//	this->OnDeliveryRaidMatchmaking(Message); 
	//});

	//DeliveryGetRaidTicket = DeliveryGetRaidClass<FB2ResponseGetRaidPtr>::GetInstance().Subscribe([this](const FB2ResponseGetRaidPtr& msgPtr)
	//{
	//	this->SetUIData(msgPtr);
	//});
}
void FLobbyRaidMainScene::UnsubscribeEvents_OnDestruct()
{
	//DeliveryRaidMatchmakingClass<FB2ResponseRaidMatchmakingPtr>::GetInstance().Unsubscribe(DeliveryRaidMatchmakingTicket);
	//DeliveryGetRaidClass<FB2ResponseGetRaidPtr>::GetInstance().Unsubscribe(DeliveryGetRaidTicket);
}

void FLobbyRaidMainScene::OnHostChanged(int32 RoomType, int32 NewHostId)
{
	if (GetLobbyGameMode() == nullptr)
		return;

	//UpdateUIHost(GetLobbyGameMode()->GetMyNetId() == NewHostId);
}

void FLobbyRaidMainScene::NotifySuccessMatchMake(bool IsHost)
{
	UpdateUIHost(IsHost);
}

void FLobbyRaidMainScene::HandleRequestMatchMaking()
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbyRaidMainScene::HandleRequestMatchMaking"));

	//if (UB2UIManager::GetInstance()->IsUIScene(EUIScene::RaidMain) == false)
	//	return;

	//RemoveSlotIndex();
	//
	//if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//{
	//	bool IsQuickJoin = DocRaid->GetOpenedRaidJoinType() == ERaidJoinType::QuickStart;
	//	int32 ClassIndex = CliToSvrPCClassType(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());

	//	b2network::B2EndpointPtr RetryChannelEndpoint = nullptr;


	//	bool IsRejoin = false;
	//	// 레이드 다시하기 - 이전게임 ChannelEndpoint 넘겨주면 같은 팀원들과 룸 조인
	//	if (DocRaid->GetIsRequestPartyRematch())
	//	{
	//		RetryChannelEndpoint = DocRaid->GetPartyRetryChannelPoint();
	//		IsRejoin = true;
	//	}

	//	int32 RaidRoomType = b2network::B2RaidRoomType::CREATE_PRIVATE_ROOM;
	//	if (IsRejoin)
	//		RaidRoomType = b2network::B2RaidRoomType::REJOIN;

	//	else if(IsQuickJoin)
	//		RaidRoomType = b2network::B2RaidRoomType::QUICK_JOIN;
	//	
	//	// 김은철 : 2018-08-24 ((UPDATE+3) 레이드 개선 - 혼자 하기/다시하기) B2RaidRoomType REJOIN 추가
	//	int32 RaidStep = DocRaid->GetLastRaidStep();
	//	int32 RaidType = DocRaid->GetLastRaidType();

	//	data_trader::Retailer::GetInstance().RequestRaidMatchmaking(RaidRoomType, ClassIndex, RaidType, RaidStep, RetryChannelEndpoint);
	//}
}

void FLobbyRaidMainScene::HandleNotifyLeave(uint32 NetId)
{
	if (GetLobbyGameMode() == nullptr)
		return;

	//FName MatchState = GetLobbyGameMode()->GetMatchState();
	//if (MatchState == NetMatchState::StartResult ||
	//	MatchState == MatchState::LeavingMap ||
	//	MatchState == MatchState::Aborted || 
	//	MatchState == MatchState::InProgress)
	//	return;

	//RemoveUIData(NetId);
}

void FLobbyRaidMainScene::SetMyPlayerInfoData(uint32 NetId, const FB2FindAccountInfoPtr& info)
{
	AddUIData(NetId, FB2ModPlayerInfo(info), info->guild_id);
}

void FLobbyRaidMainScene::SetRemotePlayerInfoData(uint32 NetId, const FB2FindAccountInfoPtr& info)
{
	// 이때는 UI를 셋팅해주지 말자.
}

void FLobbyRaidMainScene::SetRemotePlayerClass(uint32 NetId, EPCClass MainPCClass, const FB2FindAccountInfoPtr& Info)
{
	AddUIData(NetId, MainPCClass, FB2ModPlayerInfo(Info), Info->guild_id);
}

void FLobbyRaidMainScene::LeaveRoomIfJoined()
{
	RemoveSlotIndex();

	// 조인창이 열려있을수있으니 닫아준다.
	//UB2UIManager::GetInstance()->CloseUI(UIFName::RaidJoin);
}

void FLobbyRaidMainScene::HandleNotifyKickUser()
{
}

void FLobbyRaidMainScene::HandleNotifyRoomLeave()
{}

void FLobbyRaidMainScene::HandleWaitToMeetCondition()
{}

void FLobbyRaidMainScene::HandlePreparePlayers()
{
	/*if (auto* RaidJoinUI = UB2UIManager::GetInstance()->GetUI<UB2UIRaidJoin>(UIFName::RaidJoin))
	{
		const float CountDownTime = 3.0f;
		RaidJoinUI->StartCountDownAnimation(CountDownTime);
	}*/
}

void FLobbyRaidMainScene::HandleFinishLobbyMatch()
{
	//int32 RaidStageID = 0;
	//// RaidLevel 변경
	//if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//{
	//	if (auto* RaidInfoTable = StaticFindRaidInfo())
	//		RaidStageID =  RaidInfoTable->GetClientStageIdByRaidMapId(DocRaid->GetLastRaidType(), DocRaid->GetLastRaidStep());

	//	DocRaid->SetIsForceStartGame(DocRaid->GetForceStartRaid());
	//	DocRaid->SetForceStartRaid(false);
	//}

	//StartBladeIIRaidGame(Cast<AGameMode>(GetLobbyGameMode()->GetWorld()->GetAuthGameMode()), RaidStageID);
}

int32 FLobbyRaidMainScene::GetMaxMatchPlayerNum()
{
	return FLobbyRaidMainScene::MAX_PLAYERS;
}

void FLobbyRaidMainScene::ResponseChangeRoomAccess(uint8 AccessKey)
{
	//if (auto* RaidJoinUI = UB2UIManager::GetInstance()->GetUI<UB2UIRaidJoin>(UIFName::RaidJoin))
	//	RaidJoinUI->ChangeQuickStartRoom();
}

int32 FLobbyRaidMainScene::GetCurrentModeMapId()
{
	//if (UB2UIDocRaid* UIDocRaid = UB2UIDocHelper::GetDocRaid())
	//	return UIDocRaid->GetLastRaidType();

	return 0;
}

int32 FLobbyRaidMainScene::GetCurrentModeDifficulty()
{
	//if (UB2UIDocRaid* UIDocRaid = UB2UIDocHelper::GetDocRaid())
	//{
	//	return UIDocRaid->GetLastRaidStep();
	//	//return CliToSvrRaidDifficulty(UIDocRaid->GetLastRaidDifficulty());
	//}

	return 1;
}

ERoomCloseReasonType FLobbyRaidMainScene::GetStartRoomCloseReason()
{
	return ERoomCloseReasonType::START_RAID;
}

ERoomCloseReasonType FLobbyRaidMainScene::GetBreakRoomCloseReason()
{
	return ERoomCloseReasonType::BREAK_RAID;
}

void FLobbyRaidMainScene::ReceiveNoResponsePrerequisiteToStart()
{
	//UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//	BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//	BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_MatchImpossible")),
	//	0.f,
	//	true,
	//	true,
	//	EUIMsgPopupButtonGroup::Confirm
	//);
}

void FLobbyRaidMainScene::OnDeliveryRaidMatchmaking(const FB2ResponseRaidMatchmakingPtr& Message)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbyRaidMainScene::OnDeliveryRaidMatchmaking"));

	//auto& LocalCharData = BladeIIGameImpl::GetLocalCharacterData();

	//TArray<FB2RoomPropertyPtr> properties;
	//if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//{
	//	auto b2RoomProperty = std::make_shared<b2network::B2RoomProperty>();
	//	b2RoomProperty->key = "RaidStep";
	//	b2RoomProperty->value = FString::FromInt(DocRaid->GetLastRaidStep());
	//	properties.Add(b2RoomProperty);

	//	auto b2RoomProperty2 = std::make_shared<b2network::B2RoomProperty>();
	//	b2RoomProperty2->key = "RaidType";
	//	b2RoomProperty2->value = FString::FromInt(DocRaid->GetLastRaidType());
	//	properties.Add(b2RoomProperty2);
	//}

	//int32 roomType		= 0;
	//int32 roomAccess	= 0;
	//FString roomId		= TEXT("");
	//b2network::B2RoomLocationPtr roomLocation = nullptr;

	//if (Message->req_type == b2::protocol::session::CREATE_PRIVATE_ROOM)
	//{
	//	roomType = Message->room_type;
	//	roomAccess = Message->room_access;
	//	roomLocation = Message->room_location;

	//	// roomLocation 설정 후
	//	if (roomLocation)
	//		roomId = roomLocation->room_id;
	//	
	//
	//	UE_LOG(LogBladeII, Log, TEXT("%s:%d:%s:%d"), *roomLocation->channel_endpoint->host, roomLocation->channel_endpoint->port,
	//		*roomLocation->room_id, roomType);
	//}
	//else if (Message->req_type == b2::protocol::session::REJOIN)
	//{
	//	roomType = Message->room_type;
	//	roomAccess = Message->room_access;

	//	if (auto* DocRaid = UB2UIDocHelper::GetDocRaid())
	//		roomId = DocRaid->GetNextRoomId();
	//}
	//else
	//{
	//	roomType = Message->room_type;
	//	roomAccess = Message->room_access;
	//}

	//TArray<b2network::B2RoomCharacterPtr> characters;
	//TArray<b2network::B2PresetNumPtr> presetNums;

	//// temporary test code
	//for (int i = (int)EPCClass::EPC_Gladiator; i < GetMaxPCClassNum(); ++i)
	//{
	//	const EPCClass PCClass = static_cast<EPCClass>(i);

	//	auto b2PresetNum = std::make_shared<b2network::B2PresetNum>();
	//	b2PresetNum->character_type = CliToSvrPCClassType(IntToPCClass(i));
	//	b2PresetNum->item_preset_num = CliToSvrPresetID(LocalCharData.GetPCClassItemPresetSlotID(PCClass));
	//	b2PresetNum->skill_preset_num = CliToSvrPresetID(LocalCharData.GetPCClassSkillSlotID(PCClass));
	//	presetNums.Emplace(b2PresetNum);
	//}

	//data_trader::RequestJoinRoomClass<const FString&, int32, int32, TArray<FB2RoomPropertyPtr>&, TArray<b2network::B2RoomCharacterPtr>&,
	//	TArray<b2network::B2PresetNumPtr>&>::GetInstance().Signal(roomId, roomType, roomAccess, properties, characters, presetNums);

	//GuildBuffSetttingClass<const TArray<b2network::B2GuildSkillPtr>&>::GetInstance().Signal(Message->guild_skills);
}

void FLobbyRaidMainScene::UpdateUIHost(bool IsHost)
{
	//if (auto* RaidJoinUI = UB2UIManager::GetInstance()->GetUI<UB2UIRaidJoin>(UIFName::RaidJoin))
	//	RaidJoinUI->SetHost(IsHost);
}

void FLobbyRaidMainScene::AddUIData(const uint32 NetId, const FB2ModPlayerInfo& info, const int GuildID)
{
	//EPCClass InPCClass = info.GetMainPlayerClass();

	//if (NetId == GetLobbyGameMode()->GetMyNetId())
	//{
	//	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	//	{
	//		InPCClass = IntToPCClass(DocBattle->GetCurPCClass());
	//	}
	//}

	//AddUIData(NetId, InPCClass, info, GuildID);
}

void FLobbyRaidMainScene::AddUIData(const uint32 NetId, const EPCClass InPCClass, const FB2ModPlayerInfo& info, const int GuildID)
{
	FB2ModPlayerInfo* PlayerInfo = const_cast<FB2ModPlayerInfo*>(&info);

	//EPCClass MainPCClass = InPCClass;
	//int32 CharLevel = (MainPCClass != EPCClass::EPC_End) ? info.GetCharacterLevel(MainPCClass) : 1;
	//FText UserNick = FText::FromString(info.GetUserNickName());
	//
	//int32 CombatPoint = CombatStatEval::GetPCCombatPower(MainPCClass, PlayerInfo);

	//// Setting Raid Player JoinSlot.
	//AddUIData(NetId, MainPCClass, CharLevel, UserNick, CombatPoint, NetId == GetLobbyGameMode()->GetMyNetId(), GuildID);
}

void FLobbyRaidMainScene::AddUIData(const uint32 NetId, const EPCClass PCClass, const int32 CharLevel, const FText UserNick,  const int32 CombatPower, const bool IsMySlot, const int GuildID)
{
	if (IsExist(NetId))
		return;

	//// Raid UI 갱신을 위해 정보를 넣어준다.
	//if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	//{
	//	int32 NextSlotIdx = InsertNextSlotIndex(NetId);
	//	DocRaid->SetJoinSlotData(NextSlotIdx, NetId, UserNick, CharLevel, PCClass, CombatPower, GuildID);
	//	DocRaid->SetJoinSlotStateByIndex(NextSlotIdx, ERaidJoinSlotState::JoinCharacter);
	//	if (IsMySlot)
	//		DocRaid->SetMySlotIdx(NextSlotIdx);
	//}

	//if (auto* RaidInviteUI = UB2UIManager::GetInstance()->GetUI<UB2UIGameInviteList>(UIFName::RaidInviteList))
	//	RaidInviteUI->SetJoinEvent(UserNick);
}

void FLobbyRaidMainScene::RemoveUIData(const uint32 NetId)
{
	/*if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		int32 SlotIdx = RemoveSlotIndex(NetId);

		if (SlotIdx != -1)
		{
			DocRaid->ResetJoinSlotData(SlotIdx);
			DocRaid->SetJoinSlotStateByIndex(SlotIdx, DocRaid->GetDefaultSlotState(DocRaid->GetOpenedRaidJoinType()));
		}
	}*/
}

void FLobbyRaidMainScene::InsertSlotIndex(const int32 SlotIdx, const int32 NetId)
{
	SlotIndexs[SlotIdx] = NetId;
}

int32 FLobbyRaidMainScene::InsertNextSlotIndex(const int32 NetId)
{
	int32 NextInsertIdx = GetNextInsertSlotIndex(NetId);
	InsertSlotIndex(NextInsertIdx, NetId);
	return NextInsertIdx;
}

int32 FLobbyRaidMainScene::RemoveSlotIndex(const int32 NetId)
{
	int32 SlotIdx = GetSlotIndex(NetId);
	if (SlotIdx == -1)
		return -1;

	InsertSlotIndex(SlotIdx, 0);
	return SlotIdx;
}

void FLobbyRaidMainScene::RemoveSlotIndex()
{
	for (int32 i = 0; i < 4; ++i)
		InsertSlotIndex(i, 0);
}

int32 FLobbyRaidMainScene::GetSlotIndex(const int32 NetId)
{
	// 기존에 적재되어있는지 확인
	for (int i = 0; i < SlotIndexs.Num(); ++i)
	{
		if (SlotIndexs[i] == NetId)
			return i;
	}

	return -1;
}

bool FLobbyRaidMainScene::IsExist(const int32 NetId) const
{
	// 기존에 적재되어있는지 확인
	for (int i = 0; i < SlotIndexs.Num(); ++i)
	{
		if (SlotIndexs[i] == NetId)
			return true;
	}

	return false;
}

int32 FLobbyRaidMainScene::GetNextInsertSlotIndex(const int32 NetId)
{
	// 기존에 적재되어있는지 확인
	int32 ExistSlotIdx = GetSlotIndex(NetId);
	if (ExistSlotIdx != -1)
		return ExistSlotIdx;

	// 안되어있으면 첫번째로 비어있는 슬롯
	for (int i = 0; i < SlotIndexs.Num(); ++i)
	{
		if (SlotIndexs[i] == 0)
			return i;
	}

	return 0;
}

FString FLobbyRaidMainScene::GetStageDifficultyString(EStageDifficulty InDifficulty)
{
	//static UEnum* ePtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStageDifficulty"), true);
	//return ePtr->GetNameStringByIndex((int32)InDifficulty);
	return FString("");
}

void FLobbyRaidMainScene::SetUIData(const FB2ResponseGetRaidPtr& MsgPtr)
{
	/*UB2UIDocRaid* RaidDoc = UB2UIDocHelper::GetDocRaid();
	if (!RaidDoc)
		return;

	RaidDoc->SetGetRaidPtr(MsgPtr);*/
}

void FLobbySceneManager::OnHostChanged(int32 RoomType, int32 NewHostId)
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->OnHostChanged(RoomType, NewHostId);
	}
}

void FLobbySceneManager::NotifySuccessMatchMake(bool IsHost)
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->NotifySuccessMatchMake(IsHost);
	}
}


void FLobbySceneManager::HandleRequestMatchMaking()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->HandleRequestMatchMaking();
	}
}

void FLobbySceneManager::HandleNotifyLeave(uint32 NetId)
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->HandleNotifyLeave(NetId);
	}
}

void FLobbySceneManager::SetMyPlayerInfoData(uint32 NetId, const FB2FindAccountInfoPtr& info)
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->SetMyPlayerInfoData(NetId, info);
	}
}

void FLobbySceneManager::SetRemotePlayerInfoData(uint32 NetId, const FB2FindAccountInfoPtr& info)
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->SetRemotePlayerInfoData(NetId, info);
	}
}

void FLobbySceneManager::SetRemotePlayerClass(uint32 NetId, EPCClass MainPCClass, const FB2FindAccountInfoPtr& Info)
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->SetRemotePlayerClass(NetId, MainPCClass, Info);
	}
}

void FLobbySceneManager::LeaveRoomIfJoined()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->LeaveRoomIfJoined();
	}
}

void FLobbySceneManager::HandleNotifyKickUser()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->HandleNotifyKickUser();
	}
}

void FLobbySceneManager::HandleNotifyRoomLeave()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->HandleNotifyRoomLeave();
	}
}

void FLobbySceneManager::HandleWaitToMeetCondition()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->HandleWaitToMeetCondition();
	}
}

void FLobbySceneManager::HandlePreparePlayers()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->HandlePreparePlayers();
	}
}

void FLobbySceneManager::HandleFinishLobbyMatch()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->HandleFinishLobbyMatch();
	}
}

void FLobbySceneManager::ReceiveNoResponsePrerequisiteToStart()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->ReceiveNoResponsePrerequisiteToStart();
	}
}

int32 FLobbySceneManager::GetMaxMatchPlayerNum()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			return MatchSceneBase->GetMaxMatchPlayerNum();
	}

	return 1;
}

void FLobbySceneManager::ResponseChangeRoomAccess(uint8 AccessKey)
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->ResponseChangeRoomAccess(AccessKey);
	}
}

int32 FLobbySceneManager::GetMatchModeMapId()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			return MatchSceneBase->GetCurrentModeMapId();
	}

	return 1;
}

int32 FLobbySceneManager::GetMatchModeDifficulty()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			return MatchSceneBase->GetCurrentModeDifficulty();
	}

	return 1;
}

ERoomCloseReasonType FLobbySceneManager::GetStartRoomCloseReason()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			return MatchSceneBase->GetStartRoomCloseReason();
	}

	return ERoomCloseReasonType::NONE;
}

ERoomCloseReasonType FLobbySceneManager::GetBreakRoomCloseReason()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			return MatchSceneBase->GetBreakRoomCloseReason();
	}

	return ERoomCloseReasonType::NONE;
}

void FLobbySceneManager::ReceiveMatchPlayerTeamInfo()
{
	if (LobbyScenes.IsValidIndex(static_cast<uint8>(CurrentLobbyScene)))
	{
		if (auto* MatchSceneBase = LobbyScenes[static_cast<uint8>(CurrentLobbyScene)])
			MatchSceneBase->ReceiveMatchPlayerTeamInfo();
	}
}

void FLobbySceneManager::SetUnconditionalShowWing(bool bOn, bool bUpdateCharMeshImmediate, EPCClass MeshUpdateClass)
{
	bUnconditionalShowWing = bOn;

	if (bUpdateCharMeshImmediate)
	{ // 즉석 메쉬 업데이트. 업데이트 하면서 bUnconditionalShowWing 을 참조하게 될 거.
		if (MeshUpdateClass == EPCClass::EPC_End)
		{
			for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
			{
				UpdateLobbyCharCompForParts(IntToPCClass(PCI));
			}
		}
		else
		{
			UpdateLobbyCharCompForParts(MeshUpdateClass);
		}
	}
}

void FLobbySceneManager::SetShopGirlReservedState(ELobbyShopGirlState InStateToReserve)
{
	ShopGirlReservedState = InStateToReserve; // 거의 이건 세팅 후 오래 지나지 않아 바로 써먹고 리셋되어야 할 성격임.
}

void FLobbySceneManager::BattleStart()
{
	uint8 CurLobbySceneIndex = static_cast<uint8>(CurrentLobbyScene);
	if (LobbyScenes.IsValidIndex(CurLobbySceneIndex))
		LobbyScenes[CurLobbySceneIndex]->BattleStart();
}

void FLobbySceneManager::OnLobbyFlyFairy_Visible(EFairyType InType)
{
	/*for (int32 i = CliToSvrFairyType(EFairyType::FAIRY_NIECE); i < CliToSvrFairyType(EFairyType::FAIRY_MAX); ++i)
	{
		if (LobbyFlyFairys.Contains(InType))
		{
			AB2LobbyFlyingFairyDoumi* FairyActor = LobbyFlyFairys[SvrToCliFairyType(i)];
			if (FairyActor)
			{
				if (i == CliToSvrFairyType(InType))
					FairyActor->SetVisibleCharacter(true);
				else
					FairyActor->SetVisibleCharacter(false);
			}
		}
	}*/
}

void FLobbySceneManager::OnLobbyFlyFairy_AttachFX(bool bValue)
{
	/*for (int32 i = CliToSvrFairyType(EFairyType::FAIRY_NIECE); i < CliToSvrFairyType(EFairyType::FAIRY_MAX); ++i)
	{
		EFairyType CurrentFairyType = SvrToCliFairyType(i);
		if (LobbyFlyFairys.Contains(CurrentFairyType))
		{
			AB2LobbyFlyingFairyDoumi* FairyActor = LobbyFlyFairys[SvrToCliFairyType(i)];
			if (FairyActor)
			{
				FairyActor->SetAttachFX(bValue);
			}
		}
	}*/
}

void FLobbySceneManager::OnLobbyFlyFairy_AllHidden()
{
	/*for (int32 i = CliToSvrFairyType(EFairyType::FAIRY_NIECE); i < CliToSvrFairyType(EFairyType::FAIRY_MAX); ++i)
	{
		EFairyType CurrentFairyType = SvrToCliFairyType(i);
		if (LobbyFlyFairys.Contains(CurrentFairyType))
		{
			AB2LobbyFlyingFairyDoumi* FairyActor = LobbyFlyFairys[CurrentFairyType];
			if (FairyActor)
			{
				FairyActor->SetVisibleCharacter(false);
			}
		}
	}*/
}

FLobbyGuildBattleReady::FLobbyGuildBattleReady(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyGuildBattleReady::~FLobbyGuildBattleReady()
{
	
}

void FLobbyGuildBattleReady::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbyGuildBattleReady::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

void FLobbyGuildBattleReady::SubscribeEvents_OnConstruct()
{
	//DeliverySetGuildBattleEntryTicket = DeliverySetGuildBattleEntryClass<FB2ResponseSetGuildBattlePtr>::GetInstance().Subscribe([](const FB2ResponseSetGuildBattlePtr& Entry)
	//{
	//	DevShowNotReadyYetMessage(TEXT("Guild_BattleReady_SaveStatComplete"));
	//	auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	//	if (!GuildDoc)
	//		return;

	//	GuildDoc->SetGuildBattleEntry(Entry->entry->slots);

	//	// jwyim GuildEntry
	//	/*GuildDoc->SetGuildFormation(SvrToCliFormationType(Entry->formation_type));
	//	GuildDoc->SetPCCharacterClass1(SvrToCliPCClassType(Entry->character_slot_1));
	//	GuildDoc->SetPCCharacterClass2(SvrToCliPCClassType(Entry->character_slot_2));
	//	GuildDoc->SetPCCharacterClass3(SvrToCliPCClassType(Entry->character_slot_3));
	//	GuildDoc->SetPCCharacterClass4(SvrToCliPCClassType(Entry->character_slot_4));*/

	//	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::GuildMain);
	//	data_trader::Retailer::GetInstance().RequestGuildDetailInfo(0);

	//});
}
void FLobbyGuildBattleReady::UnsubscribeEvents_OnDestruct()
{
	DeliverySetGuildBattleEntryClass<FB2ResponseSetGuildBattlePtr>::GetInstance().Unsubscribe(DeliverySetGuildBattleEntryTicket);
}

FLobbyControlMainScene::FLobbyControlMainScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyControlMainScene::~FLobbyControlMainScene()
{
	
}

void FLobbyControlMainScene::SubscribeEvents_OnConstruct()
{
	DeliveryGetAssaultBattleStatusTicket = DeliveryGetAssaultBattleStatusClass<FB2ResponseGetAssaultBattleStatusPtr>::GetInstance().Subscribe([this](const FB2ResponseGetAssaultBattleStatusPtr& msgPtr)
	{
		this->SetUIData(msgPtr);
	});
}
void FLobbyControlMainScene::UnsubscribeEvents_OnDestruct()
{
	DeliveryGetAssaultBattleStatusClass<FB2ResponseGetAssaultBattleStatusPtr>::GetInstance().Unsubscribe(DeliveryGetAssaultBattleStatusTicket);
}

void FLobbyControlMainScene::SetUIData(const FB2ResponseGetAssaultBattleStatusPtr& MsgPtr)
{
	/*UB2UIDocControl* ControlDoc = UB2UIDocHelper::GetDocControl();
	if (!ControlDoc)
		return;

	ControlDoc->SetGetAssaultPtr(MsgPtr);*/
}

FLobbyCharacterSelectPage::FLobbyCharacterSelectPage(class FLobbySceneManager* OwnerSceneManager)
	:FLobbySceneBase(OwnerSceneManager)
{
	
}

FLobbyCharacterSelectPage::~FLobbyCharacterSelectPage()
{
	
}

void FLobbyCharacterSelectPage::OpenScene()
{
	FLobbySceneBase::OpenScene();

	SelectCharacterStageInfo(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());
}

void FLobbyCharacterSelectPage::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//	{
	//		//영웅관리에서 쓰이는 Actor라서 초기 위치로 돌아가야만 한다.
	//		It.Value().ActorForMain->SetActorLocation(It.Value().CachedInitLocation);
	//	}
	//}
}

void FLobbyCharacterSelectPage::SubscribeEvents_OnConstruct()
{
	LobbyStageInfoSelectTicket = LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Subscribe([this](EPCClass SelectedMain, EPCClass SelectedSub) 
	{
		this->SelectCharacterStageInfo(SelectedMain, SelectedSub); 
	});
}
void FLobbyCharacterSelectPage::UnsubscribeEvents_OnDestruct()
{
	LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Unsubscribe(LobbyStageInfoSelectTicket);
}

void FLobbyCharacterSelectPage::BattleStart()
{
	/*UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	if (DocBS)
	{
		if (DocBS->GetRepeatBattleLoopAll() || DocBS->GetRepeatBattleLoopOne())
		{
			StartRepeatBattleStageClass<int32, EStageDifficulty>::GetInstance().Signal(DocBS->GetClientStageId(), DocBS->GetStageDifficulty());
		}
		else
		{
			UB2UIDocUICondition* DocCon = UB2UIDocHelper::GetDocUICondition();
			StartGameStageFromLobbyClass<int32, EStageDifficulty, const TArray<EStageBuffType>&, bool>::GetInstance().Signal(DocBS->GetClientStageId(), DocBS->GetStageDifficulty(), DocBS->GetSelectedStageBuffs(), (DocCon ? DocCon->GetbUserSelectedSkipStory() : false));
		}
	}*/
}

void FLobbyCharacterSelectPage::SelectCharacterStageInfo(EPCClass MainClass, EPCClass SubClass)
{
	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//CurrentMainClass = MainClass;
	//CurrentSubClass = SubClass;

	//if (!(this->GetSceneManager() && this->GetSceneManager()->GetCurrentLobbyScene() == this->GetLobbyScene()))
	//{
	//	return;
	//}

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//		CharacterHiddenAndStopSound(It.Value().ActorForMain, true);
	//	if (It.Value().ActorForSub)
	//		CharacterHiddenAndStopSound(It.Value().ActorForSub, true);
	//}
	//if (CurrentMainClass != EPCClass::EPC_End)
	//{
	//	auto* LobbyCharInfo = LobbyCharactersForALobbyType.Find(CurrentMainClass);
	//	if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
	//	{
	//		CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, false);
	//		LobbyCharInfo->ActorForMain->PlaySelectAnimation(ELobbyScene::ELobbyScene_CharacterPage);
	//	}
	//}

	//if (CurrentSubClass != EPCClass::EPC_End)
	//{
	//	auto* LobbySubCharInfo = LobbyCharactersForALobbyType.Find(CurrentSubClass);
	//	if (LobbySubCharInfo && LobbySubCharInfo->ActorForSub)
	//	{
	//		CharacterHiddenAndStopSound(LobbySubCharInfo->ActorForSub, false);
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_CharacterPage, false, true);// Sub 캐릭터의 Sound notify 를 끈다.
	//	}
	//}

	//if (GEngine && GLog)
	//{
	//	GEngine->HandleCeCommand(GetLobbyGameMode()->GetWorld(), TEXT("SetDefaultCamActorForCharacterSelect"), *GLog);
	//}

	//DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}
}

FLobbyFindAccountInfo::FLobbyFindAccountInfo(class FLobbySceneManager* OwnerSceneManager)
	:FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyFindAccountInfo::~FLobbyFindAccountInfo()
{
}

void FLobbyFindAccountInfo::OpenScene()
{
	if (GetSceneManager())
	{ // 캐릭터 메쉬 조합 시 리모트 캐릭터 허용.
		GetSceneManager()->SetCanUseRemoteInfoForSKMeshSetup(true);
	}

	//FLobbySceneBase::OpenScene();

	//auto& AccountInfo = BladeIIGameImpl::GetClientDataStore().GetOtherUserInfo();

	//auto* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIOtherUserInfo>(UIFName::FindAccountInfo);
	//BII_CHECK(pUI);

	//EPCClass  aaaa = EPCClass::EPC_Fighter;

	//for (auto El : AccountInfo.GetAccountInfoPtr()->characters)
	//{
	//	LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(SvrToCliPCClassType(El->character_type));
	//	pUI->PCClassButtonSlotSettingEvent(SvrToCliPCClassType(El->character_type));
	//}

	//auto* DocSome = UB2UIDocHelper::GetDocSome();

	//if (DocSome && DocSome->GetOnClickUserToHallOf())
	//{
	//	pUI->CharSelectUpdateEvent(DocSome->GetCurrentBestCharacter());
	//	pUI->TotalUpdate(DocSome->GetCurrentBestCharacter());
	//	DocSome->SetOnClickUserToHallOf(false);
	//	
	//}
	//else
	//{
	//	pUI->CharSelectUpdateEvent(SvrToCliPCClassType(AccountInfo.GetAccountInfoPtr()->characters[0]->character_type));
	//	pUI->TotalUpdate(SvrToCliPCClassType(AccountInfo.GetAccountInfoPtr()->characters[0]->character_type));
	//}

	//AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//	LobbyGameMode->BindTouchInput(true);
}

void FLobbyFindAccountInfo::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();

	//auto* LobbyCharInfo = AllCharactersOfDesiredType.Find(CurrentPCClass);
	//if (LobbyCharInfo)
	//{
	//	LobbyCharInfo->MatineeForInven->Stop();
	//	if (LobbyCharInfo->MatineeForWingEventScene) {
	//		LobbyCharInfo->MatineeForWingEventScene->Stop();
	//	}
	//	LobbyCharInfo->ActorForMain->SetActorRotation(LobbyCharInfo->InitialRotation);
	//}

	//CurrentPCClass = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();

	//if (GetSceneManager())
	//{ // 캐릭터 메쉬 조합 시 리모트 캐릭터 정보 사용 금지. 이걸 안 할 시 잘못된 정보 접근으로 크래쉬 위험이 있음.
	//	GetSceneManager()->SetCanUseRemoteInfoForSKMeshSetup(false);
	//}
}

void FLobbyFindAccountInfo::SubscribeEvents_OnConstruct()
{
	DeliveryResponseFindAccountTicket = DeliveryResponseFindAccountClass<FB2FindAccountPtr>::GetInstance().Subscribe([this](FB2FindAccountPtr Account) {
		this->FindAccountUIOpen(Account);
	});

	//LobbyHeroFindAccountTicket = LobbyHeroFindAccountClass<EPCClass>::GetInstance().Subscribe([this](EPCClass PCClass)
	//{
	//	this->CharacterSelect(PCClass);
	//});
}
void FLobbyFindAccountInfo::UnsubscribeEvents_OnDestruct()
{
	DeliveryResponseFindAccountClass<FB2FindAccountPtr>::GetInstance().Unsubscribe(DeliveryResponseFindAccountTicket);
	//LobbyHeroFindAccountClass<EPCClass>::GetInstance().Unsubscribe(LobbyHeroFindAccountTicket);
}
void FLobbyFindAccountInfo::SubscribeEvents_OnOpen()
{
	LobbyCharRotateCharacterTicket = LobbyCharRotateCharacterClass<float>::GetInstance().Subscribe([this](float Value) { this->RotateCharacterYaw(Value); });
	//DeliveryAskFriendTicket = DeliveryAskFriendClass<>::GetInstance().Subscribe([this]() {DevShowNotReadyYetMessage("FriendAdd");  });
	//HandleServerError5503Ticket = HandleServerError5503Class<>::GetInstance().Subscribe([this]() {DevShowError("5502"); });
}
void FLobbyFindAccountInfo::UnsubscribeEvents_OnClose()
{
	LobbyCharRotateCharacterClass<float>::GetInstance().Unsubscribe(LobbyCharRotateCharacterTicket);
	//DeliveryAskFriendClass<>::GetInstance().Unsubscribe(DeliveryAskFriendTicket);
	//HandleServerError5503Class<>::GetInstance().Unsubscribe(HandleServerError5503Ticket);
}

void FLobbyFindAccountInfo::FindAccountUIOpen(const FB2FindAccountPtr& AccountInfo)
{
	//BladeIIGameImpl::GetClientDataStore().SetOtherUserInfo(AccountInfo->account_infos[0]);
	//UB2UIManager::GetInstance()->CloseAllStandaloneUIs();
	//LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_FindAccount);
}

void FLobbyFindAccountInfo::RotateCharacterYaw(float Value)
{
	/*auto& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();

	auto* LobbyCharInfo = AllCharactersOfDesiredType.Find(CurrentPCClass);
	if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
	{
#if CHAR_ANIM_ROTATION
		LobbyCharInfo->ActorForMain->PlayTurnAnimation(Value < 0.f);
#elif CHAR_ACTOR_ROTATION
		if (!LobbyCharInfo->ActorForMain->IsPlayingSelectedAnim())
		{
			FRotator Rotation = LobbyCharInfo->ActorForMain->GetActorRotation();
			Rotation += FRotator(0.f, Value * 2.0f, 0.f);
			LobbyCharInfo->ActorForMain->SetActorRotation(Rotation);
		}
#endif
	}*/
}


void FLobbyFindAccountInfo::CharacterSelect(EPCClass PCClass)
{
	/*if (!(this->GetSceneManager() && this->GetSceneManager()->GetCurrentLobbyScene() == this->GetLobbyScene()))
	{
	//CurrentPCClass = SelectedCharacter;
	return;
	}*/

	//CurrentPCClass = PCClass;

	//auto& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();

	//auto* LobbyCharInfo = AllCharactersOfDesiredType.Find(PCClass);
	//if (LobbyCharInfo)
	//{
	//	LobbyCharInfo->MatineeForInven->Stop();
	//	if (LobbyCharInfo->MatineeForWingEventScene) {
	//		LobbyCharInfo->MatineeForWingEventScene->Stop();
	//	}
	//	LobbyCharInfo->ActorForMain->SetActorRelativeRotation(LobbyCharInfo->InitialRotation);
	//}

	////CurrentPCClass = SelectedCharacter;
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(AllCharactersOfDesiredType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//		CharacterHiddenAndStopSound(It.Value().ActorForMain, true);
	//	if (It.Value().ActorForSub)
	//		CharacterHiddenAndStopSound(It.Value().ActorForSub, true);
	//}

	//LobbyCharInfo = AllCharactersOfDesiredType.Find(PCClass);
	//if (LobbyCharInfo)
	//{
	//	CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, false);

	//	if (LobbyCharInfo->ActorForMain && !LobbyCharInfo->ActorForMain->bHidden)
	//	{
	//		//캐릭터의 위치를 맞추기 위함.
	//		if (!LobbyCharInfo->CachedInitLocation.IsNearlyZero())
	//			LobbyCharInfo->ActorForMain->SetActorLocation(LobbyCharInfo->CachedInitLocation);

	//		LobbyCharInfo->ActorForMain->PlaySelectAnimation(ELobbyScene::ELobbyScene_HeroManagement);
	//	}

	//	float EventKeyTime = ABladeIIGameMode::GetMatineePositionByEventName(LobbyCharInfo->MatineeForInven, TEXT("Inven"));

	//	LobbyCharInfo->MatineeForInven->Play();
	//	SetCameraMatineeCam(LobbyCharInfo->MatineeForInven, EventKeyTime);
	//	LobbyCharInfo->MatineeForInven->SetPosition(EventKeyTime);
	//	LobbyCharInfo->MatineeForInven->Pause();
	//}
}

FLobbyControlCharacterSelectPage::FLobbyControlCharacterSelectPage(class FLobbySceneManager* OwnerSceneManager)
	:FLobbySceneBase(OwnerSceneManager)
{
	
}

FLobbyControlCharacterSelectPage::~FLobbyControlCharacterSelectPage()
{
	
}

void FLobbyControlCharacterSelectPage::OpenScene()
{
	FLobbySceneBase::OpenScene();

	//if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	//	SelectCharacterStageInfo(IntToPCClass(DocBattle->GetCurPCClass()));
}

void FLobbyControlCharacterSelectPage::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//	{
	//		//영웅관리에서 쓰이는 Actor라서 초기 위치로 돌아가야만 한다.
	//		It.Value().ActorForMain->SetActorLocation(It.Value().CachedInitLocation);
	//	}
	//}
}

void FLobbyControlCharacterSelectPage::SubscribeEvents_OnConstruct()
{
	LobbyStageInfoSelectTicket = LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Subscribe([this](EPCClass SelectedMain, EPCClass SelectedSub) { this->SelectCharacterStageInfo(SelectedMain); });
}
void FLobbyControlCharacterSelectPage::UnsubscribeEvents_OnDestruct()
{
	LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Unsubscribe(LobbyStageInfoSelectTicket);
}

void FLobbyControlCharacterSelectPage::NotifySuccessMatchMake(bool IsHost)
{	

}

void FLobbyControlCharacterSelectPage::HandleRequestMatchMaking()
{
	//if (UB2UIManager::GetInstance()->IsUIScene(EUIScene::ControlCharacterSelectPage) == false)
	//	return;

	//auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	//EPCClass CurPcClass = DocBattle ? IntToPCClass(DocBattle->GetCurPCClass()) : EPCClass::EPC_Gladiator;

	//data_trader::Retailer::GetInstance().RequestAssaultMatchmaking(CliToSvrPCClassType(CurPcClass));
}

void FLobbyControlCharacterSelectPage::HandleNotifyLeave(uint32 NetId)
{
	// 유저 나갈때도 유저카운팅UI 갱신
	RefreshFindMatchPopup();
}

void FLobbyControlCharacterSelectPage::SetMyPlayerInfoData(uint32 NetId, const FB2FindAccountInfoPtr& info)
{
	//GetLobbyGameMode()->SetPeerTeam();
}

void FLobbyControlCharacterSelectPage::SetRemotePlayerInfoData(uint32, const FB2FindAccountInfoPtr&)
{
	// 호스트. 팀정보 갱신. 알림.
	//GetLobbyGameMode()->SetPeerTeam();
}

void FLobbyControlCharacterSelectPage::SetRemotePlayerClass(uint32, EPCClass, const FB2FindAccountInfoPtr&)
{
	// 유저 들어오면 유저카운팅UI 갱신
	// 팀정보 받고 갱신
	//RefreshFindMatchPopup();	
}

void FLobbyControlCharacterSelectPage::LeaveRoomIfJoined()
{
	/*UB2UIFindMatchPopup* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIFindMatchPopup>(UIFName::FindMatchPopup);

	if (pUI)
	{
		pUI->OnCancleBtnClickInner();
	}*/
}

void FLobbyControlCharacterSelectPage::HandlePreparePlayers()
{
	//UB2UIFindMatchPopup* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIFindMatchPopup>(UIFName::FindMatchPopup);

	//if (pUI)
	//{
	//	pUI->SetClose(false);
	//	pUI->PlayAnimationEvent_Anim_Completed();

	//	// 진동
	//	AB2LobbyGameMode* pGM = GetLobbyGameMode();
	//	if(pGM)
	//		pGM->ForceFeedback();
	//}
	////GetLobbyGameMode()->StartFinishLobbyMatch();

	SendControlMatchUserInfo();
}

void FLobbyControlCharacterSelectPage::HandleFinishLobbyMatch()
{
	//StartBladeIIControlGame(Cast<AGameMode>(GetLobbyGameMode()->GetWorld()->GetAuthGameMode()));
}

int32 FLobbyControlCharacterSelectPage::GetMaxMatchPlayerNum()
{
//#if !UE_BUILD_SHIPPING
//	if (NetFakeConfigure::GetInstance().IsNumToPlayRaid())
//		return NetFakeConfigure::GetInstance().GetNumToPlayRaid();
//#endif
	return CONTROL_GAME_MAX_PLAYERS;
}

ERoomCloseReasonType FLobbyControlCharacterSelectPage::GetStartRoomCloseReason()
{
	return ERoomCloseReasonType::START_ASSAULT;
}

void FLobbyControlCharacterSelectPage::ReceiveMatchPlayerTeamInfo()
{
	// 팀정보 받구 갱신. 호스트도 받구갱신.
	RefreshFindMatchPopup();
}

void FLobbyControlCharacterSelectPage::ReceiveNoResponsePrerequisiteToStart()
{
	//UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_MatchImpossible")),
	//		0.f,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::Confirm
	//	);
}

void FLobbyControlCharacterSelectPage::SendControlMatchUserInfo()
{
	// 내정보(넷아이디) 받구나서 등급정보 보냄
	//UB2UIDocControl* ControlDoc = UB2UIDocHelper::GetDocControl();
	//if (!ControlDoc)
	//	return;

	//GetLobbyGameMode()->NetSendBattleGradeInfo(ControlDoc->GetBattleGrade(), ControlDoc->GetBattleStarCount());
	//GetLobbyGameMode()->NetSendMatchToken();
}

void FLobbyControlCharacterSelectPage::BattleStart()
{
	//// 점검 정산 체크
	//if (TutorialManager::GetInstance().IsFinishTutorial(TutorialID_Occupy) && CheckContentsModeState(b2network::B2ContentsMode::ASSAULT))
	//	return;

	//if (!IsActiveControlTutorial())
	//	LobbyStartMatchClass<bool>::GetInstance().Signal(true);

	//UB2UIManager::GetInstance()->OpenUI(UIFName::FindMatchPopup);

	//if (IsActiveControlTutorial())
	//{
	//	RefreshFindMatchPopup();
	//	GetLobbyGameMode()->StartControlTutorial();		
	//}
}

void FLobbyControlCharacterSelectPage::SelectCharacterStageInfo(EPCClass MainClass)
{
	//// 그냥 같은거에 메인만 써본다.
	//// 싱글에 사용할 액터 배치하면 바꿔야함
	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//CurrentMainClass = MainClass;

	//if (!(this->GetSceneManager() && this->GetSceneManager()->GetCurrentLobbyScene() == this->GetLobbyScene()))
	//{
	//	return;
	//}

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//		CharacterHiddenAndStopSound(It.Value().ActorForMain, true);
	//	if (It.Value().ActorForSub)
	//		CharacterHiddenAndStopSound(It.Value().ActorForSub, true);
	//}
	//if (CurrentMainClass != EPCClass::EPC_End)
	//{
	//	auto* LobbyCharInfo = LobbyCharactersForALobbyType.Find(CurrentMainClass);
	//	if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
	//	{
	//		CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, false);
	//		LobbyCharInfo->ActorForMain->PlaySelectAnimation(ELobbyScene::ELobbyScene_StageInfo);
	//	}
	//}
	//
	//if (GEngine && GLog)
	//{
	//	GEngine->HandleCeCommand(GetLobbyGameMode()->GetWorld(), TEXT("SetDefaultCamActorForSingleCharacterSelect"), *GLog);
	//}

	//DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}
}

void FLobbyControlCharacterSelectPage::RefreshFindMatchPopup()
{
	//UB2UIFindMatchPopup* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIFindMatchPopup>(UIFName::FindMatchPopup);

	//if (pUI)
	//{
	//	pUI->SetPlayerCount(GetLobbyGameMode()->GetMatchPlayerNum());

	//	// 풀방으로 보여줌
	//	if (IsActiveControlTutorial())
	//	{
	//		pUI->SetPlayerCount(GetLobbyGameMode()->GetMaxMatchPlayerNum());
	//	}
	//}
}

FLobbyEnterDungeonMain::FLobbyEnterDungeonMain(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
	DeliveryResponseGetChallengeModeInfoTicket = DeliveryResponseGetChallengeModeInfoClass<FB2ResponseGetChallengeModeInfoPtr>::GetInstance().Subscribe([this](const FB2ResponseGetChallengeModeInfoPtr& MsgPtr)
	{
		this->ResponseGetChallengeModeInfo(MsgPtr);
	});
}

FLobbyEnterDungeonMain::~FLobbyEnterDungeonMain()
{
	DeliveryResponseGetChallengeModeInfoClass<FB2ResponseGetChallengeModeInfoPtr>::GetInstance().Unsubscribe(DeliveryResponseGetChallengeModeInfoTicket);
}

void FLobbyEnterDungeonMain::OpenScene()
{
	FLobbySceneBase::OpenScene();
	// 큰 거 하나 미리 백그라운드 로딩해 둠.
	// 이 장면에서 반격이나 레이드 장면으로 넘어가면 사용하지도 않을 영웅의 탑 스트리밍 레벨이 flush 되어버리는 부작용이 있는데 
	// 그럼에도 불구하고 이쯤서 해 둘 가치가 있다고 판단.
	FLobbySceneManager* LobbySM = GetSceneManager();
	if (LobbySM && !LobbySM->IsSceneStreamingLevelLoaded(ELobbyScene::ELobbyScene_HeroTowerReady))
	{
		LobbySM->PrefetchSceneStreamingLevel(ELobbyScene::ELobbyScene_HeroTowerReady);
	}
}

void FLobbyEnterDungeonMain::ResponseGetChallengeModeInfo(const FB2ResponseGetChallengeModeInfoPtr& MsgPtr)
{
	//// 모드 상태정보 저장
	//UB2UIDocMode* pDocMode = UB2UIDocHelper::GetDocMode();
	//if (pDocMode)
	//	pDocMode->ReceiveGetContentsModeState(MsgPtr->mode_infos);

	//UB2UIDocRaid* pDocRaid = UB2UIDocHelper::GetDocRaid();
	//if (pDocRaid)
	//	pDocRaid->SetPeriodInfos(MsgPtr->raid_open_period_infos);

	//// 영웅의탑 남은티켓 셋팅
	//UB2UIDocHeroTower * pDocHeroTower = UB2UIDocHelper::GetDocHeroTower();
	//if (pDocHeroTower)
	//	pDocHeroTower->SetDailyLeftFreeTryCount(MsgPtr->hero_tower_ticket_count);

	//// 반격던전 남은티켓 셋팅
	//UB2UIDocCounterAttack * pDocCounterAttack = UB2UIDocHelper::GetDocCounterAttack();
	//if (pDocCounterAttack)
	//	pDocCounterAttack->SetLeftTicket(MsgPtr->counter_dungeon_left_ticket_count);

	//// 균열 남은티켓 셋팅
	//UB2UIDocDimension * pDocDimension = UB2UIDocHelper::GetDocDimension();
	//if (pDocDimension)
	//{
	//	int32 RemainTicket = FMath::Max(pDocDimension->GetDailyTryCount() - MsgPtr->dimension_daily_clear_count, 0);
	//	pDocDimension->SetDimensionTicket(RemainTicket);
	//}

	//if (UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::LobbyMain)
	//{
	//	UB2UILobbyMain* LobbyMainUIPage = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	//	if (LobbyMainUIPage)
	//	{
	//		LobbyMainUIPage->SetReceiveGetChallengeMode(true);
	//		LobbyMainUIPage->ExecutePendingDungeonSceneInner();
	//	}
	//}

	//// 링커매니저를 타고 들어왔을 경우. response를 받고 캐싱된 페이지로 이동시킴.
	//FB2UILinkManager::ExcuteUIScene();
}

void FLobbyEnterDungeonMain::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

FLobbyEnterShopMain::FLobbyEnterShopMain(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyEnterShopMain::~FLobbyEnterShopMain()
{

}

void FLobbyEnterShopMain::OpenScene()
{
	FLobbySceneBase::OpenScene();

}

void FLobbyEnterShopMain::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

FLobbyHeroTowerCharacterSelectPage::FLobbyHeroTowerCharacterSelectPage(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
	
}

FLobbyHeroTowerCharacterSelectPage::~FLobbyHeroTowerCharacterSelectPage()
{
	
}

void FLobbyHeroTowerCharacterSelectPage::OpenScene()
{
	FLobbySceneBase::OpenScene();

	SelectCharacterStageInfo(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());
}


void FLobbyHeroTowerCharacterSelectPage::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//	{
	//		//영웅관리에서 쓰이는 Actor라서 초기 위치로 돌아가야만 한다.
	//		It.Value().ActorForMain->SetActorLocation(It.Value().CachedInitLocation);
	//	}
	//}
}

void FLobbyHeroTowerCharacterSelectPage::SubscribeEvents_OnConstruct()
{
	LobbyStageInfoSelectTicket = LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Subscribe([this](EPCClass SelectedMain, EPCClass SelectedSub) { this->SelectCharacterStageInfo(SelectedMain, SelectedSub); });
}
void FLobbyHeroTowerCharacterSelectPage::UnsubscribeEvents_OnDestruct()
{
	LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Unsubscribe(LobbyStageInfoSelectTicket);
}

void FLobbyHeroTowerCharacterSelectPage::SelectCharacterStageInfo(EPCClass MainClass, EPCClass SubClass)
{
	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//CurrentMainClass = MainClass;
	//CurrentSubClass = SubClass;

	//if (!(this->GetSceneManager() && this->GetSceneManager()->GetCurrentLobbyScene() == this->GetLobbyScene()))
	//{
	//	return;
	//}

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//		CharacterHiddenAndStopSound(It.Value().ActorForMain, true);
	//	if (It.Value().ActorForSub)
	//		CharacterHiddenAndStopSound(It.Value().ActorForSub, true);
	//}
	//if (CurrentMainClass != EPCClass::EPC_End)
	//{
	//	auto* LobbyCharInfo = LobbyCharactersForALobbyType.Find(CurrentMainClass);
	//	if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
	//	{
	//		CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, false);
	//		LobbyCharInfo->ActorForMain->PlaySelectAnimation(ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage);
	//	}
	//}

	//if (CurrentSubClass != EPCClass::EPC_End)
	//{
	//	auto* LobbySubCharInfo = LobbyCharactersForALobbyType.Find(CurrentSubClass);
	//	if (LobbySubCharInfo && LobbySubCharInfo->ActorForSub)
	//	{
	//		CharacterHiddenAndStopSound(LobbySubCharInfo->ActorForSub, false);
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage, false, true);// Sub 캐릭터의 Sound notify 를 끈다.
	//	}
	//}

	//if (GEngine && GLog)
	//{
	//	GEngine->HandleCeCommand(GetLobbyGameMode()->GetWorld(), TEXT("SetDefaultCamActorForCharacterSelect"), *GLog);
	//}

	//DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}
}

FLobbyEnterBattleMain::FLobbyEnterBattleMain(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
	DeliveryResponseGetDuelModeInfoTicket = DeliveryResponseGetDuelModeInfoClass<FB2ResponseGetDuelModeInfoPtr>::GetInstance().Subscribe([this](const FB2ResponseGetDuelModeInfoPtr& MsgPtr)
	{
		this->ResponseGetDuelModeInfo(MsgPtr);
	});
}

FLobbyEnterBattleMain::~FLobbyEnterBattleMain()
{
	DeliveryResponseGetDuelModeInfoClass<FB2ResponseGetDuelModeInfoPtr>::GetInstance().Unsubscribe(DeliveryResponseGetDuelModeInfoTicket);
}

void FLobbyEnterBattleMain::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbyEnterBattleMain::CloseScene()
{
	FLobbySceneBase::CloseScene();
}


void FLobbyEnterBattleMain::ResponseGetDuelModeInfo(const FB2ResponseGetDuelModeInfoPtr& MsgPtr)
{
	//// 모드 상태정보 저장
	//UB2UIDocMode* pDocMode = UB2UIDocHelper::GetDocMode();
	//if (pDocMode)
	//	pDocMode->ReceiveGetContentsModeState(MsgPtr->mode_infos);

	//// 점령전 오픈시간 정보 저장
	//UB2UIDocControl* pDocControl = UB2UIDocHelper::GetDocControl();
	//if (pDocControl)
	//	pDocControl->SetPeriodInfos(MsgPtr->assault_open_period_infos);

	//auto* DocSome = UB2UIDocHelper::GetDocSome();
	//if (DocSome && DocSome->GetOnClickHallOfFame())
	//{
	//	DocSome->SetOnClickHallOfFame(false);
	//	GetSceneManager()->ExecutePendingHallOfFameScene();
	//}
	//else
	//{
	//	if (UB2UIManager::GetInstance()->GetCurrUIScene() == EUIScene::LobbyMain)
	//	{
	//		UB2UILobbyMain* LobbyMainUIPage = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	//		if (LobbyMainUIPage)
	//		{
	//			LobbyMainUIPage->SetReceiveGetDuelMode(true);
	//			LobbyMainUIPage->ExecutePendingPVPSceneInner();
	//		}
	//	}

	//	// 링커매니저를 타고 들어왔을 경우. response를 받고 캐싱된 페이지로 이동시킴.
	//	FB2UILinkManager::ExcuteUIScene();
	//}
}

FLobbyPVPCharacterSelectPage::FLobbyPVPCharacterSelectPage(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
	
}

FLobbyPVPCharacterSelectPage::~FLobbyPVPCharacterSelectPage()
{
	
}

void FLobbyPVPCharacterSelectPage::OpenScene()
{
	FLobbySceneBase::OpenScene();

	SelectCharacterStageInfo(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());
}


void FLobbyPVPCharacterSelectPage::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//	{
	//		//영웅관리에서 쓰이는 Actor라서 초기 위치로 돌아가야만 한다.
	//		It.Value().ActorForMain->SetActorLocation(It.Value().CachedInitLocation);
	//	}
	//}
}

void FLobbyPVPCharacterSelectPage::SubscribeEvents_OnConstruct()
{
	LobbyStageInfoSelectTicket = LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Subscribe([this](EPCClass SelectedMain, EPCClass SelectedSub) { this->SelectCharacterStageInfo(SelectedMain, SelectedSub); });
}
void FLobbyPVPCharacterSelectPage::UnsubscribeEvents_OnDestruct()
{
	LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Unsubscribe(LobbyStageInfoSelectTicket);
}

void FLobbyPVPCharacterSelectPage::SelectCharacterStageInfo(EPCClass MainClass, EPCClass SubClass)
{
	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//CurrentMainClass = MainClass;
	//CurrentSubClass = SubClass;

	//if (!(this->GetSceneManager() && this->GetSceneManager()->GetCurrentLobbyScene() == this->GetLobbyScene()))
	//{
	//	return;
	//}

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//		CharacterHiddenAndStopSound(It.Value().ActorForMain, true);
	//	if (It.Value().ActorForSub)
	//		CharacterHiddenAndStopSound(It.Value().ActorForSub, true);
	//}
	//if (CurrentMainClass != EPCClass::EPC_End)
	//{
	//	auto* LobbyCharInfo = LobbyCharactersForALobbyType.Find(CurrentMainClass);
	//	if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
	//	{
	//		CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, false);
	//		LobbyCharInfo->ActorForMain->PlaySelectAnimation(ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage);
	//	}
	//}

	//if (CurrentSubClass != EPCClass::EPC_End)
	//{
	//	auto* LobbySubCharInfo = LobbyCharactersForALobbyType.Find(CurrentSubClass);
	//	if (LobbySubCharInfo && LobbySubCharInfo->ActorForSub)
	//	{
	//		CharacterHiddenAndStopSound(LobbySubCharInfo->ActorForSub, false);
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage, false, true);// Sub 캐릭터의 Sound notify 를 끈다.
	//	}
	//}

	//if (GEngine && GLog)
	//{
	//	GEngine->HandleCeCommand(GetLobbyGameMode()->GetWorld(), TEXT("SetDefaultCamActorForCharacterSelect"), *GLog);
	//}

	//DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}
}

FLobbyEtherMain::FLobbyEtherMain(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyEtherMain::~FLobbyEtherMain()
{

}

void FLobbyEtherMain::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbyEtherMain::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

FLobbyRestPointScene::FLobbyRestPointScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
	DeliveryRestRewardTicket = INDEX_NONE;
}
FLobbyRestPointScene::~FLobbyRestPointScene()
{
}

void FLobbyRestPointScene::OpenScene()
{
	//FLobbySceneBase::OpenScene();

	//AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // 안전빵.

	//	// 터치 액션을 위한 입력 바인딩
	//	LobbyGameMode->BindTouchInput(true);
	//	LobbyGameMode->SetEnableClickEvents(true);
	//}

	//AMatineeActor* DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}

	//FLobbySceneManager* LobbySM = GetSceneManager();
	//AB2LobbyRelaxingDoumi* RelaxingDoumiActor = LobbySM ? Cast<AB2LobbyRelaxingDoumi>(LobbySM->GetNPCSKActor(ELobbyNPCType::ELobbyNPC_RelaxingDoumi)) : nullptr;
	//if (RelaxingDoumiActor)
	//{ // 심신의 피로가 풀리는 라미엘의 환영 쑈.
	//	RelaxingDoumiActor->PlayTemporaryAnimOfState(ELobbyRelaxingDoumiState::ELRD_Welcoming);
	//}
}
void FLobbyRestPointScene::CloseScene()
{
	// 터치 액션을 위한 입력 바인딩 제거.
	/*AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
	if (LobbyGameMode)
	{
		LobbyGameMode->BindTouchInput(false);
		LobbyGameMode->SetEnableClickEvents(false);
	}

	auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	if (DefaultCameraMatinee)
	{
		DefaultCameraMatinee->Stop();
	}*/

	FLobbySceneBase::CloseScene();
	
}

void FLobbyRestPointScene::SubscribeEvents_OnOpen()
{
	DeliveryRestRewardTicket = DeliveryRestRewardClass<FB2ResponseRestRewardPtr>::GetInstance().Subscribe([this](const FB2ResponseRestRewardPtr& msgPtr)
	{
		this->OnResponseReward(msgPtr);
	});
}
void FLobbyRestPointScene::UnsubscribeEvents_OnClose()
{
	DeliveryRestRewardClass<FB2ResponseRestRewardPtr>::GetInstance().Unsubscribe(DeliveryRestRewardTicket);
}
//
//static ELobbyRelaxingDoumiState RestPointRewardReqStateToDoumiState(ERestPointRewardRequestState InReqState)
//{ // 둘 사이 변환이 혹여나 널리 쓰이게 되면 위치는 옮겨서..
////	if (InReqState == ERestPointRewardRequestState::ERPRR_Normal) {
////		return ELobbyRelaxingDoumiState::ELRD_RewardNormal;
////	}
////	else if (InReqState == ERestPointRewardRequestState::ERPRR_Double) {
////		return ELobbyRelaxingDoumiState::ELRD_RewardDouble;
////	}
//	return ELobbyRelaxingDoumiState::ELRD_IdleLoop;
//}

void FLobbyRestPointScene::OnResponseReward(const FB2ResponseRestRewardPtr& msgPtr)
{
//	// 배치해 놓은 휴식 도우미 라미엘 액터
//	FLobbySceneManager* LobbySM = GetSceneManager();
//	AB2LobbyRelaxingDoumi* RelaxingDoumiActor = LobbySM ? Cast<AB2LobbyRelaxingDoumi>(LobbySM->GetNPCSKActor(ELobbyNPCType::ELobbyNPC_RelaxingDoumi)) : nullptr;
//	if (RelaxingDoumiActor)
//	{
//		// 일반받기인지, 두배받기인지 여기로 넘어온 데이터만 가지고 보려면 used_cash 가 있는지로 볼 수도 있긴 하지만
//		// 그게 근본적으로 일반받기, 두배받기를 정의하는 건 아니니 할 짓이 아니고.. 
//		// 이 상황에서 UIRestPointMain 은 열려 있어야 할 테니 그쪽서 요청할 때 저장해 놓은 값을 얻어다 쓴다.
//
//		UB2UIRestPointMain* RestPointUI = UB2UIManager::GetInstance()->GetUI<UB2UIRestPointMain>(UIFName::RestPointMain);
//		if (RestPointUI)
//		{
//			ERestPointRewardRequestState RewardReqState = RestPointUI->GetLastRewardRequestState();
//
//			// 이 check 에 걸리는 건.. 뭔가.. 휴식보상 들어가서 요청을 눌렀으나 응답이 없어서 나갔다가 다시 휴식 보상 창으로 들어왔는데 그제서야 응답이 왔다거나?
//			// 만약에 정말 그런 일이 일어나면 그냥 무시.
//			checkSlow(RewardReqState == ERestPointRewardRequestState::ERPRR_Normal || RewardReqState == ERestPointRewardRequestState::ERPRR_Double);
//
//			ELobbyRelaxingDoumiState DoumiAnimState = RestPointRewardReqStateToDoumiState(RewardReqState);
//			if (DoumiAnimState != ELobbyRelaxingDoumiState::ELRD_IdleLoop || DoumiAnimState != ELobbyRelaxingDoumiState::ELRD_End)
//			{
//				RelaxingDoumiActor->PlayTemporaryAnimOfState(DoumiAnimState);
//			}
//		}
//	}
}

FLobbyEventMain::FLobbyEventMain(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyEventMain::~FLobbyEventMain()
{

}

void FLobbyEventMain::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbyEventMain::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

FLobbySeasonEvent::FLobbySeasonEvent(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbySeasonEvent::~FLobbySeasonEvent()
{

}

void FLobbySeasonEvent::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbySeasonEvent::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

FLobbyPackageStore::FLobbyPackageStore(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyPackageStore::~FLobbyPackageStore()
{

}

void FLobbyPackageStore::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbyPackageStore::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

void FLobbyPackageStore::SubscribeEvents_OnConstruct()
{/*
	DeliveryGetPackageShopTicket = DeliveryGetPackageShopClass<FB2ResponseGetPackageShopPtr>::GetInstance().Subscribe([this](const FB2ResponseGetPackageShopPtr& ShopData)
	{
		this->SetPackageShopData(ShopData);

		if (GetSceneManager()->GetCurrentLobbyScene() != GetLobbyScene())
		{
			LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::PackageStore);
		}
		else
		{
			UB2UIPackageStore* PackageStoreUI = UB2UIManager::GetInstance()->GetUI<UB2UIPackageStore>(UIFName::PackageStore);
			if (PackageStoreUI)
			{
				PackageStoreUI->UpdatePanelWidget();
			}
		}
	});*/
}

void FLobbyPackageStore::UnsubscribeEvents_OnDestruct()
{
	DeliveryGetPackageShopClass<FB2ResponseGetPackageShopPtr>::GetInstance().Unsubscribe(DeliveryGetPackageShopTicket);
}

void FLobbyPackageStore::SetPackageShopData(const FB2ResponseGetPackageShopPtr& ShopData)
{
	//auto* DocStore = UB2UIDocHelper::GetDocStore();
	//if (DocStore == nullptr)
	//	return;

	//// 구매 횟수
	//for (auto Restrict : ShopData->shop_buy_manages)
	//{
	//	DocStore->SetReceivedRestrictCount(Restrict->product_id, Restrict->buy_count);
	//}

	//// 기간제 남은 시간
	//for(auto PeridicItem: ShopData->periodic_package_states)
	//{
	//	DocStore->SetReceivedPeriodicPackageLeftTime(PeridicItem->package_id, PeridicItem->end_pay_time);
	//}

	//// 레벨업 패키지
	//for (auto LevelUpMissionItem : ShopData->level_up_package_missions)
	//{
	//	DocStore->SetReceivedLevelupPacakgeMissions(LevelUpMissionItem->package_id, LevelUpMissionItem);
	//}

	//// 레벨 달성 계약 패키지
	//for (auto ReachLevelMissionItem : ShopData->reach_level_package_missions)
	//{
	//	DocStore->SetReceivedReachLevelPacakgeMissions(ReachLevelMissionItem->package_id, ReachLevelMissionItem);
	//}

	//// 월정액
	//if (ShopData->flat_rate_package_bonus_state)
	//{
	//	DocStore->SetReceivedFlatratePackageBonusState(ShopData->flat_rate_package_bonus_state);
	//}

	//// 패키지 이벤트 마크
	//for (auto ProductMark : ShopData->shop_product_marks)
	//{
	//	DocStore->SetPackageProductMark(ProductMark->product_id, ProductMark->mark_type);
	//	DocStore->SetProductSalePeriod(ProductMark->product_id, ProductMark->sales_start_time, ProductMark->sales_end_time);
	//}

	//FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_SHOP, RedDotHint::RED_DOT_SHOP_PACKAGE });
}

FLobbyHallOfFame::FLobbyHallOfFame(FLobbySceneManager * OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
	BackingScene = false;
	CurrentEquipRanking = 1;
}

FLobbyHallOfFame::~FLobbyHallOfFame()
{

}

void FLobbyHallOfFame::OpenScene()
{
	FLobbySceneBase::OpenScene();

	BackingScene = false;
	UIVisibility(false);
	SetHallOfFameCharactersMesh();

	//if (auto* UIHallOfFame = UB2UIManager::GetInstance()->GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
	//{
	//	for (int i = 1; i < 4; ++i)
	//	{

	//		UIHallOfFame->AddPlayer(GetHallOfFameCharacter(i), false);
	//	}
	//}

	//AB2LobbyGameMode* LobbyGM = GetSceneManager() ? GetSceneManager()->GetLobbyGameMode() : nullptr;
	//if (LobbyGM)
	//{
	//	LobbyGM->ConditionalWaitForLoadingScreen();
	//}

	PlayStartMatinee();
}

void FLobbyHallOfFame::ForceUpdate()
{
	UIVisibility(false);
	SetHallOfFameCharactersMesh();

	//AB2LobbyGameMode* LobbyGM = GetSceneManager() ? GetSceneManager()->GetLobbyGameMode() : nullptr;
	//if (LobbyGM)
	//{
	//	LobbyGM->ConditionalWaitForLoadingScreen();
	//}

	PlayStartMatinee();
}

void FLobbyHallOfFame::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//for (auto& ModeInfo : RankersInfo)
	//{
	//	ModeInfo.Value.Empty();
	//}
	//RankersInfo.Empty();
	//PraiseInfo.Empty();
	BackingScene = true;
}

void FLobbyHallOfFame::SubscribeEvents_OnConstruct()
{
	//EndHallOfFameMatineeTicket = EndHallOfFameMatineeClass<>::GetInstance().Subscribe([this]()
	//{
	//	this->ForceEndMatinee();
	//});
}

void FLobbyHallOfFame::UnsubscribeEvents_OnDestruct()
{
	//EndHallOfFameMatineeClass<>::GetInstance().Unsubscribe(EndHallOfFameMatineeTicket);
}

void FLobbyHallOfFame::PlayStartMatinee()
{
	EHallOfFameMode CurrentHallOfFameMode = GetHallOfFameMode();

	//if (PlayedMatinee.Contains(CurrentHallOfFameMode))
	//{
	//	// 한번 재생 했으면
	//	if (PlayedMatinee[CurrentHallOfFameMode])
	//	{
	//		ForceEndMatinee();
	//	}
	//	// 재생 안했으면
	//	else
	//	{
	//		if (auto* DefaultCameraMatinee = GetDefaultCameraMatinee())
	//		{
	//			DefaultCameraMatinee->SetPosition(0.f);
	//			DefaultCameraMatinee->Play();

	//			SetCameraMatineeCam(DefaultCameraMatinee);

	//			PlayedMatinee[CurrentHallOfFameMode] = true;
	//		}
	//	}
	//}
}


void FLobbyHallOfFame::ForceEndMatinee()
{
	//if (auto* DefaultCameraMatinee = GetDefaultCameraMatinee())
	//{
	//	if (DefaultCameraMatinee && DefaultCameraMatinee->MatineeData)
	//	{
	//		DefaultCameraMatinee->SetPosition(DefaultCameraMatinee->MatineeData->InterpLength);
	//		DefaultCameraMatinee->Play();
	//		SetCameraMatineeCam(DefaultCameraMatinee);
	//	}
	//}
}

void FLobbyHallOfFame::OnFinishedHallOfFameDisplayMatinee(class AMatineeActor* Matinee)
{
	UIVisibility(true);
	SetHallOfCharactersInfo();
	SetPlayAnimationForEndMatinee();
	SetCelebratePopup();

}

void FLobbyHallOfFame::SetRankeInfo(FB2ResponseGetHallOfFamePtr HallOfInfo)
{
	//if (HallOfInfo)
	//{
	//	for (auto& BaseInfo : HallOfInfo->basics)
	//	{
	//		if (BaseInfo)
	//		{
	//			b2network::B2HallOfFameUserPtr UserPtr = GetUserPtr(HallOfInfo, BaseInfo->account_id);

	//			if (UserPtr->user && UserPtr->user->character_summary)
	//			{
	//				FRankerInfo UserInfo;

	//				UserInfo.AccountID = UserPtr->user->character_summary->account_id;
	//				UserInfo.AttackPower = UserPtr->user->character_summary->power;
	//				UserInfo.CharacterClass = SvrToCliPCClassType(UserPtr->user->character_summary->character_type);
	//				UserInfo.MatchGrade = BaseInfo->match_grade;
	//				UserInfo.MatchStarCount = BaseInfo->match_start_count;
	//				UserInfo.Mode = static_cast<EHallOfFameMode>(BaseInfo->mode);
	//				UserInfo.MvpCount = BaseInfo->mvp_count;
	//				UserInfo.PraiseCount = BaseInfo->praised_count;
	//				UserInfo.Ranking = BaseInfo->ranking;
	//				UserInfo.UserName = UserPtr->user->character_summary->account_name;
	//				UserInfo.Wing_Grade = UserPtr->wing_grade;
	//				UserInfo.Wing_Level = UserPtr->wing_level;
	//				UserInfo.WingVisible = UserPtr->wing_visible;
	//				UserInfo.HelmetVisible = UserPtr->helmet_visible;
	//				UserInfo.CostumeWeaponVisible = UserPtr->costume_weapon_visible;
	//				UserInfo.CostumeHelmetVisible =	UserPtr->costume_hat_visible;
	//				UserInfo.CostumeBodyVisible   = UserPtr->costume_armor_visible;

	//				if (UserPtr->equip_items.Num() > 0)
	//				{
	//					UserInfo.EquipItems = UserPtr->equip_items;
	//				}

	//				if (UserPtr->equip_costumes.Num() > 0)
	//				{
	//					UserInfo.EquipICostumeItems = UserPtr->equip_costumes;
	//				}

	//				if (!RankersInfo.Contains(UserInfo.Mode))
	//				{
	//					RankersInfo.Add(UserInfo.Mode, TArray<FRankerInfo>());
	//				}

	//				RankersInfo[UserInfo.Mode].Add(UserInfo);
	//			}
	//		}
	//	}
	//}
}

void FLobbyHallOfFame::SetHallOfFameInfo(FB2ResponseGetHallOfFamePtr InHallOfFameInfo)
{
	//if (InHallOfFameInfo)
	//{
	//	for (auto& ModeInfo : RankersInfo)
	//	{
	//		ModeInfo.Value.Empty();
	//	}
	//	RankersInfo.Empty();
	//	PraiseInfo.Empty();
	//	RewardInfo.Empty();
	//	PlayedMatinee.Empty();
	//	BackingScene = true;

	//	AB2LobbyGameMode* LobbyGM = GetSceneManager() ? GetSceneManager()->GetLobbyGameMode() : nullptr;
	//	if (LobbyGM)
	//	{
	//		LobbyGM->SetupLoadingScreenImageCommon(false, false, false); // 진행도는 없이 로딩화면만 띄움.
	//	}

	//	SetRankeInfo(InHallOfFameInfo);

	//	if (InHallOfFameInfo->left_praise_counts.Num() > 0)
	//	{
	//		TArray<b2network::B2HallOfFamePraisePtr> ServerPraiseInfo = InHallOfFameInfo->left_praise_counts;

	//		for (auto& Praise : ServerPraiseInfo)
	//		{
	//			FPraiseCount PraiseCount;
	//			PraiseCount.CurrentPraise = Praise->praise_count;
	//			PraiseCount.MaxPraise = Praise->max_count;
	//			PraiseCount.PraisedAccountID = Praise->target_account_id;
	//			PraiseInfo.Add(static_cast<EHallOfFameMode>(Praise->mode), PraiseCount);
	//		}
	//	}

	//	if (InHallOfFameInfo->ranker_rewards.Num() > 0)
	//	{
	//		TArray<b2network::B2HallOfFameRewardPtr> ServerRewardInfo = InHallOfFameInfo->ranker_rewards;

	//		for (auto& InReward : ServerRewardInfo)
	//		{
	//			FHallOfFameReward InRewardInfo;
	//			InRewardInfo.RewardItem = InReward->rewards;
	//			InRewardInfo.Ranking = InReward->ranking;
	//			RewardInfo.Add(static_cast<EHallOfFameMode>(InReward->mode), InRewardInfo);
	//		}
	//	}

	//	if (auto* DocSome = UB2UIDocHelper::GetDocSome())
	//	{
	//		DocSome->SetPraiseInfo(InHallOfFameInfo->left_praise_counts);
	//	}

	//	PlayedMatinee.Add(EHallOfFameMode::PVP, false);
	//	PlayedMatinee.Add(EHallOfFameMode::TEAM_MATCH, false);
	//	PlayedMatinee.Add(EHallOfFameMode::CONTROL, false);
	//}
}

void FLobbyHallOfFame::SetPraiseInfo(FB2ResponsePraiseTargetRankerPtr InPraiseInfo)
{
	/*if (InPraiseInfo)
	{
		TArray<b2network::B2HallOfFamePtr> InBasics = InPraiseInfo->basics;
		TArray<b2network::B2RewardPtr> InRewards = InPraiseInfo->rewards;
		b2network::B2HallOfFamePraisePtr InHallOfPraiseInfo = InPraiseInfo->left_praise_count;

		for (auto& InBasic : InBasics)
		{
			EHallOfFameMode InMode = static_cast<EHallOfFameMode>(InBasic->mode);
			int32 InRanking = InBasic->ranking;

			if (RankersInfo.Contains(InMode) && RankersInfo[InMode].IsValidIndex(InRanking - 1))
			{
				RankersInfo[InMode][InRanking - 1].PraiseCount = InBasic->praised_count;
			}
		}

		if (InHallOfPraiseInfo)
		{
			EHallOfFameMode InMode = static_cast<EHallOfFameMode>(InHallOfPraiseInfo->mode);

			if (PraiseInfo.Contains(InMode))
			{
				PraiseInfo[InMode].CurrentPraise = InHallOfPraiseInfo->praise_count;
				PraiseInfo[InMode].MaxPraise = InHallOfPraiseInfo->max_count;
				PraiseInfo[InMode].PraisedAccountID = InHallOfPraiseInfo->target_account_id;
			}

		}

		if (auto* DocSome = UB2UIDocHelper::GetDocSome())
		{
			DocSome->SetPraiseInfo(InHallOfPraiseInfo);
		}

		if (auto* UIHallOfFame = UB2UIManager::GetInstance()->GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
		{
			UIHallOfFame->SetVisiblePlayerInfo(false);
		}

		SetHallOfCharactersInfo();

		if (InRewards.IsValidIndex(0) && InRewards[0]->raw_reward && InRewards[0]->raw_reward->count)
		{
			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HallOfFame_GetReward")), InRewards[0]->raw_reward->count),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm,
				FMsgPopupOnClick::CreateLambda([]()
			{
				if (auto* UIHallOfFame = UB2UIManager::GetInstance()->GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
				{
					UIHallOfFame->SetVisiblePlayerInfo(true);
				}
			})
			);
		}
	}*/
}

void FLobbyHallOfFame::UIVisibility(bool IsVisible)
{
	//if (auto* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header))
	//{
	//	HeaderUI->SetVisibility(IsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	//	HeaderUI->SetClose(IsVisible);
	//}

	//if (auto* UIHallOfFame = UB2UIManager::GetInstance()->GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
	//{
	//	UIHallOfFame->SetVisibleForMatinee(!IsVisible);
	//}
}

b2network::B2HallOfFameUserPtr FLobbyHallOfFame::GetUserPtr(FB2ResponseGetHallOfFamePtr HallOfInfo, int64 InAccountId)
{
	if (HallOfInfo)
	{
		auto RankerPtrs = HallOfInfo->rankers;
		
		for (auto& Ranker : RankerPtrs)
		{
			if (Ranker->user && Ranker->user->character_summary
				&& Ranker->user->character_summary->account_id == InAccountId)
			{
				return Ranker;
			}
		}
	}
	return nullptr;
}

EHallOfFameMode FLobbyHallOfFame::GetHallOfFameMode() const
{
	//if (auto* UIHallOfFame = UB2UIManager::GetInstance()->GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
	//{
	//	return UIHallOfFame->GetHallOfFameMode();
	//}
	return EHallOfFameMode::None;
}

TArray<FRankerInfo>& FLobbyHallOfFame::GetRankers(EHallOfFameMode GameMode)
{
	if (!RankersInfo.Contains(GameMode))
	{
		RankersInfo.Add(GameMode, TArray<FRankerInfo>());
	}
	return RankersInfo[GameMode];
}

FRankerInfo FLobbyHallOfFame::GetRankerToRanking(int32 Ranking, EHallOfFameMode GameMode)
{
	EHallOfFameMode FindGameMode = GameMode == EHallOfFameMode::None ? GetHallOfFameMode() : GameMode;
	TArray<FRankerInfo>& Rankers = GetRankers(FindGameMode);

	if (Rankers.IsValidIndex(Ranking - 1))
	{
		return Rankers[Ranking - 1];
	}

	return FRankerInfo();
}

FRankerInfo FLobbyHallOfFame::GetRankerToAccountID(int32 AccountID, EHallOfFameMode GameMode)
{
	EHallOfFameMode FindGameMode = GameMode == EHallOfFameMode::None ? GetHallOfFameMode() : GameMode;
	TArray<FRankerInfo>& Rankers = GetRankers(FindGameMode);

	for (auto& RankerInfo : Rankers)
	{
		if (RankerInfo.AccountID == AccountID)
		{
			return RankerInfo;
		}
	}
	return FRankerInfo();
}

void FLobbyHallOfFame::SetCelebratePopup()
{
	//EHallOfFameMode CurrentGameMode = GetHallOfFameMode();

	//if (RewardInfo.Contains(CurrentGameMode))
	//{
	//	if (auto* UIHallOfFame = UB2UIManager::GetInstance()->GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
	//	{
	//		UIHallOfFame->SetCelebratePopup(CurrentGameMode
	//			, RewardInfo[CurrentGameMode].Ranking, RewardInfo[CurrentGameMode].RewardItem);
	//	}
	//	RewardInfo.Remove(CurrentGameMode);
	//}
}

EPCClass FLobbyHallOfFame::GetRankerClass(int32 Ranking)
{
	TArray<FRankerInfo> Rankers = GetRankers(GetHallOfFameMode());

	if (Rankers.IsValidIndex(Ranking - 1))
	{
		return Rankers[Ranking - 1].CharacterClass;
	}

	return EPCClass::EPC_End;
}

TArray<FHallOfFameCharacterInfo> FLobbyHallOfFame::GetFirstRankCharacterMeshInfo()
{
	TArray<FHallOfFameCharacterInfo> ReturnArray;

	auto& LobbyCharactersForHallOfFame = GetLobbyCharactersForHallOfFame();

	for (int32 i = 0; i < LobbyCharactersForHallOfFame.Num(); ++i)
	{
		if (LobbyCharactersForHallOfFame[i].Ranking == 1)
		{
			ReturnArray.Add(LobbyCharactersForHallOfFame[i]);
		}
	}
	return ReturnArray;
}

FHallOfFameCharacterInfo FLobbyHallOfFame::GetHallOfFameCharacter(int32 Ranking)
{
	auto LobbyCharactersForHallOfFame = GetLobbyCharactersForHallOfFame();
	TArray<FRankerInfo> RankingUser = GetRankers(GetHallOfFameMode());

	for (int32 i = 0; i < LobbyCharactersForHallOfFame.Num(); ++i)
	{
		if (LobbyCharactersForHallOfFame[i].Ranking == Ranking)
		{
			switch (Ranking)
			{
			case 1:
			{
				if (RankingUser.IsValidIndex(0) && LobbyCharactersForHallOfFame[i].CharacterClass == RankingUser[0].CharacterClass)
				{
					LobbyCharactersForHallOfFame[i].Ranking = RankingUser[0].Ranking;
					return LobbyCharactersForHallOfFame[i];
				}
			}
			break;
			case 2:
			{
				if (RankingUser.IsValidIndex(1))
				{
					LobbyCharactersForHallOfFame[i].CharacterClass = RankingUser[1].CharacterClass;
					LobbyCharactersForHallOfFame[i].Ranking = RankingUser[1].Ranking;
				}
				return LobbyCharactersForHallOfFame[i];
			}
			break;
			case 3:
			{
				if (RankingUser.IsValidIndex(2))
				{
					LobbyCharactersForHallOfFame[i].CharacterClass = RankingUser[2].CharacterClass;
					LobbyCharactersForHallOfFame[i].Ranking = RankingUser[2].Ranking;
				}
				return LobbyCharactersForHallOfFame[i];
			}
			break;
			default:
				break;
			}
		}
	}
	return FHallOfFameCharacterInfo();
}

void FLobbyHallOfFame::GetEquipItems(TArray<FB2Item>& EquipItems, FB2Wing& InWing)
{
	FRankerInfo RankerInfo = GetRankerToRanking(CurrentEquipRanking);

	TArray<b2network::B2ItemServerInfoPtr> B2ServerItems = RankerInfo.EquipItems;

	for (auto& ServerItem : B2ServerItems)
	{
		FB2Item ConvertItem;
		ConvertItem = ServerItem;

		if (ConvertItem.ItemClass == EItemClass::EIC_Helmet && RankerInfo.HelmetVisible == false)
			continue;
		
		EquipItems.Add(ConvertItem);
	}

	//Costume
	for (auto& ServerCostumeItem : RankerInfo.EquipICostumeItems)
	{
		if(ServerCostumeItem == nullptr)
			continue;

		FB2Item ConvertItem;
		ConvertItem = *ServerCostumeItem;

		if (ConvertItem.ItemClass == EItemClass::EIC_CostumeWeapon && RankerInfo.CostumeWeaponVisible == false)
			continue;
		if (ConvertItem.ItemClass == EItemClass::EIC_CostumeHat && RankerInfo.CostumeHelmetVisible == false)
			continue;
		if (ConvertItem.ItemClass == EItemClass::EIC_CostumeArmor && RankerInfo.CostumeBodyVisible == false)
			continue;

		EquipItems.Add(ConvertItem);
	}

	InWing.bShouldBeVisible = RankerInfo.WingVisible;

	if (RankerInfo.WingVisible)
	{
		InWing.EnhanceLevel = RankerInfo.Wing_Level;
		InWing.EvolutionGrade = RankerInfo.Wing_Grade;
		InWing.OwnerPCClass = static_cast<int32>(RankerInfo.CharacterClass);
	}
}

void FLobbyHallOfFame::SetHallOfCharactersInfo()
{
	/*if (auto* UIHallOfFame = UB2UIManager::GetInstance()->GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
	{
		EHallOfFameMode CurrentMode = GetHallOfFameMode();

		UIHallOfFame->SetPlayerInfo(GetRankers(CurrentMode));

		if (PraiseInfo.Contains(CurrentMode))
		{
			FPraiseCount CurrentPraiseInfo = PraiseInfo[CurrentMode];
			FRankerInfo InRankerInfo = GetRankerToAccountID(CurrentPraiseInfo.PraisedAccountID, CurrentMode);

			UIHallOfFame->SetHallOfFamePraiseInfo(CurrentPraiseInfo.MaxPraise, CurrentPraiseInfo.CurrentPraise, InRankerInfo.Ranking);
		}
	}*/
}

//UB2PCClassInfo* FLobbyHallOfFame::GetSingleClassInfo(int32 Ranking)
//{
//	return StaticFindPCClassInfoBox() ? StaticFindPCClassInfoBox()->GetSingleClassInfo(GetRankerToRanking(Ranking).CharacterClass) : nullptr;
//}
//
//class UAnimationAsset* FLobbyHallOfFame::GetAnimation(int32 Ranking)
//{
//	TMap<EPCClass, class UAnimationAsset*>& HallOfFameAnim = GetHallOfAnimation(Ranking == 1);
//	EPCClass RankerClass = GetRankerClass(Ranking);
//
//	if (HallOfFameAnim.Contains(RankerClass))
//	{
//		return HallOfFameAnim[RankerClass];
//	}
//	return nullptr;
//}

void FLobbyHallOfFame::SetPlayAnimationForEndMatinee()
{
	//for (int32 i = 0; i < 3; ++i)
	//{
	//	FHallOfFameCharacterInfo CharacterInfo = GetHallOfFameCharacter(i + 1);

	//	if (CharacterInfo.HallOfFameCharacter && CharacterInfo.HallOfFameCharacter->GetSkeletalMeshComponent())
	//	{
	//		class UAnimationAsset* RankingAnim = GetAnimation(i + 1);
	//		CharacterInfo.HallOfFameCharacter->GetSkeletalMeshComponent()->PlayAnimation(RankingAnim, true);
	//	}
	//}
}

// 1등 캐릭은 메쉬가 4개
void FLobbyHallOfFame::SetFirstCharacterVisible()
{
	TArray<FHallOfFameCharacterInfo> FirstCharacters = GetFirstRankCharacterMeshInfo();
	EPCClass BestCharacterClass = GetRankerClass(1);

	for (auto& FirstCharacterInfo : FirstCharacters)
	{
		if (FirstCharacterInfo.HallOfFameCharacter)
		{
			bool IsFristCharacter = FirstCharacterInfo.CharacterClass == BestCharacterClass;
			FirstCharacterInfo.HallOfFameCharacter->SetActorHiddenInGame(!IsFristCharacter);

			if (IsFristCharacter)
			{
				SetCurrentEquipRanking(1);
				GetSceneManager()->SetupLobbyCharCompForParts(BestCharacterClass, FirstCharacterInfo.HallOfFameCharacter, false);
			}
		}
	}
}

void FLobbyHallOfFame::SetHallOfFameCharactersMesh()
{
	SetFirstCharacterVisible();

	//TMap<EPCClass, class UAnimationAsset*>& HallOfFameAnim = GetHallOfAnimation();

	//FHallOfFameCharacterInfo SecondCharacter = GetHallOfFameCharacter(2);
	//FHallOfFameCharacterInfo ThirdCharacter = GetHallOfFameCharacter(3);

	//if (SecondCharacter.HallOfFameCharacter)
	//{
	//	if (SecondCharacter.Ranking == 2 && SecondCharacter.HallOfFameCharacter->GetSkeletalMeshComponent())
	//	{
	//		EPCClass RankerClass = GetRankerClass(2);
	//		SetCurrentEquipRanking(2);
	//		GetSceneManager()->SetupLobbyCharCompForParts(RankerClass, SecondCharacter.HallOfFameCharacter, false);
	//		SecondCharacter.HallOfFameCharacter->GetSkeletalMeshComponent()->PlayAnimation(HallOfFameAnim[RankerClass], true);
	//	}
	//	else
	//	{
	//		SecondCharacter.HallOfFameCharacter->SetActorHiddenInGame(true);
	//	}
	//}

	//if (ThirdCharacter.HallOfFameCharacter)
	//{
	//	if (ThirdCharacter.Ranking == 3 && ThirdCharacter.HallOfFameCharacter->GetSkeletalMeshComponent())
	//	{
	//		EPCClass RankerClass = GetRankerClass(3);
	//		SetCurrentEquipRanking(3);
	//		GetSceneManager()->SetupLobbyCharCompForParts(RankerClass, ThirdCharacter.HallOfFameCharacter, false);
	//		ThirdCharacter.HallOfFameCharacter->GetSkeletalMeshComponent()->PlayAnimation(HallOfFameAnim[RankerClass], true);
	//	}
	//	else
	//	{
	//		ThirdCharacter.HallOfFameCharacter->SetActorHiddenInGame(true);
	//	}
	//}
}

FLobbyMagicStore::FLobbyMagicStore(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyMagicStore::~FLobbyMagicStore()
{

}

void FLobbyMagicStore::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbyMagicStore::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

void FLobbyMagicStore::SubscribeEvents_OnConstruct()
{
	//DeliveryGetMagicShopTicket = DeliveryGetMagicShopClass<FB2ResponseGetMagicShopPtr>::GetInstance().Subscribe(
	//	[this](const FB2ResponseGetMagicShopPtr& MagicShopPtr)
	//{
	//	this->SetMagicStoreData(MagicShopPtr);

	//	if (GetSceneManager()->GetCurrentLobbyScene() != GetLobbyScene())
	//	{
	//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::MagicStore);
	//	}
	//	else
	//	{
	//		// 자동 갱신
	//		UB2UIMagicStore* MagicStoreUI = UB2UIManager::GetInstance()->GetUI<UB2UIMagicStore>(UIFName::MagicStore);
	//		if (MagicStoreUI)
	//		{
	//			bool bIsWaitingRequest = MagicStoreUI->GetIsWaitingRequest();
	//			MagicStoreUI->SetItemList(!bIsWaitingRequest);
	//			MagicStoreUI->SetRemainTime();
	//		}
	//	}
	//});
}

void FLobbyMagicStore::UnsubscribeEvents_OnDestruct()
{
	DeliveryGetMagicShopClass<FB2ResponseGetMagicShopPtr>::GetInstance().Unsubscribe(DeliveryGetMagicShopTicket);
}

void FLobbyMagicStore::SetMagicStoreData(const FB2ResponseGetMagicShopPtr& MagicShopPtr)
{
	//auto* DocStore = UB2UIDocHelper::GetDocStore();
	//if (!DocStore)
	//	return;

	//// 당일 갱신 횟수
	//DocStore->SetRenewalCount(MagicShopPtr->renewal_count);

	//// 다음 자동 갱신 시간
	//DocStore->SetNextFreeRenewalTime(MagicShopPtr->next_free_renewal_time);

	//// 무료 갱신 여부
	//DocStore->SetIsFreeRenewal(MagicShopPtr->is_free_renewal);

	//// 마법 상점 상품 리스트
	//DocStore->ResetMagicShopList();
	//for (auto MagicShopItem : MagicShopPtr->magic_shops)
	//{
	//	DocStore->SetMagicShopList(MagicShopItem);
	//}
}

/************************************************************************/
/* FCameraZoomModule                                                    */
/************************************************************************/
FCameraZoomModule::FCameraZoomModule() :
	CamNearDistanceLimit(0.f), 
	CamFarDistanceLimit(0.f), 
	OrgCameraLocation(FVector::ZeroVector), 
	TopOffsetZ(0.f), 
	Scene(nullptr),
	LocalPlayer(nullptr),
	//ZoomTarget(nullptr),
	CurrentCamera(nullptr)
{

}

void FCameraZoomModule::InitializeModule(FLobbySceneBase* OwnerScene)
{
	Scene = OwnerScene;

	//if (auto* PlayerController = Scene->GetLocalPlayerController())
	//	LocalPlayer = PlayerController->GetLocalPlayer();

	//CamNearDistanceLimit = 0.f;
	//CamFarDistanceLimit = 0.f;
	//OrgCameraLocation = FVector::ZeroVector;

	//ZoomTarget = nullptr;
	//CurrentCamera = nullptr;
}

void FCameraZoomModule::ClearZoomModule()
{
	//if (CurrentCamera)
	//{
	//	CurrentCamera->SetActorLocation(OrgCameraLocation);
	//	CurrentCamera = nullptr;
	//}
}

//void FCameraZoomModule::ChangeZoomTarget(AB2HeroMgntSkeletalMeshActor* Target)
//{
//	
//	ZoomTarget = Target;
//	if (auto* PlayerController = UGameplayStatics::GetLocalPlayerController(Target))
//	{
//		if (auto* CameraManager = PlayerController->PlayerCameraManager)
//			CurrentCamera = Cast<ACameraActor>(CameraManager->ViewTarget.Target);
//	}
//
//	if (ZoomTarget && CurrentCamera)
//	{
//		OrgCameraLocation = CurrentCamera->GetActorLocation();
//
//		const FVector TargetLocation = ZoomTarget->GetActorLocation();
//
//		ELobbyScene LobbySceneType = Scene ? Scene->GetLobbyScene() : ELobbyScene::ELobbyScene_None;
//		if (LobbySceneType == ELobbyScene::ELobbyScene_CollectBookMain)
//			CamNearDistanceLimit = ZoomTarget->CameraNearCollection;
//		else if (LobbySceneType == ELobbyScene::ELobbyScene_HeroManagement)
//			CamNearDistanceLimit = ZoomTarget->CameraNearInventory;
//
//		CamFarDistanceLimit = (OrgCameraLocation - TargetLocation).Size();
//
//		ZoomFixedOffsetZ = ZoomTarget->ZoomOffsetZ;
//	}
//}

float FCameraZoomModule::GetPlaneIntersectHeight(const FVector2D& ScreenPos, const FVector& Orgin)
{
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		// get the projection data
		FSceneViewProjectionData ProjectionData;
		if (LocalPlayer->GetProjectionData(LocalPlayer->ViewportClient->Viewport, /*out*/ ProjectionData))
		{
			auto InvViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix().Inverse();
			FVector WorldLoc, WorldDir;
			FSceneView::DeprojectScreenToWorld(ScreenPos, ProjectionData.GetConstrainedViewRect(), InvViewProjectionMatrix, WorldLoc, WorldDir);

			FVector InvCamDirPlaneNormalOrthXY(FVector2D(-WorldDir), 0);
			return (FMath::LinePlaneIntersection(WorldLoc, WorldLoc + 1000.f * WorldDir, Orgin, InvCamDirPlaneNormalOrthXY.GetSafeNormal())).Z - Orgin.Z;
		}
	}

	return 0.f;
}

FVector FCameraZoomModule::GetPinpointLocation(const FVector2D& ScreenFocus)
{
	//if (ZoomTarget == nullptr)
		return FVector::ZeroVector;
	///*
	//const FVector TargetLocation = ZoomTarget->GetActorLocation();
	//float OffsetZ = GetPlaneIntersectHeight(ScreenFocus, TargetLocation);

	//const float HalfHeight = TopOffsetZ * 0.5f;

	//const float MiddleOffsetZ = OffsetZ - HalfHeight;
	//const float ZWeight = MiddleOffsetZ / HalfHeight;
	//const float PinpointWeight = FMath::Clamp(ZWeight, -0.3f, 1.f);	// 0에 가까울수록 캐릭터의 중심에 Pinpoint

	//const float WeightConstant = 0.5f;	// 상 ~ 하단에 핀포인트가 가까울수록 50% 의 여분 Distance를 줌
	//const float WeightedOffsetZ = (WeightConstant * PinpointWeight * FMath::Abs(MiddleOffsetZ));

	//return TargetLocation + FVector(0.f, 0.f, OffsetZ + WeightedOffsetZ);
	//*/

	//const FVector TargetLocation = ZoomTarget->GetActorLocation();
	//return TargetLocation + FVector(0.f, 0.f, ZoomFixedOffsetZ);
}

void FCameraZoomModule::Zoom(const FVector& PinpointLocation, float Delta)
{
	//if (ZoomTarget && CurrentCamera)
	//{
	//	const FVector CurrCamLocation = CurrentCamera->GetActorLocation();

	//	// Camera 이동 Direction
	//	const bool IsZoomIn = Delta > 0 ? true : false;
	//	const FVector CameraMoveVector = IsZoomIn ? (PinpointLocation - CurrCamLocation) : (OrgCameraLocation - CurrCamLocation);
	//	if (CameraMoveVector.IsNearlyZero(1.f))
	//		return;

	//	const FVector CameraMoveDir = CameraMoveVector.GetSafeNormal();

	//	// Camera 이동 예상 위치
	//	const FVector ExpectCameraLocation = CurrCamLocation + (CameraMoveDir * Delta);

	//	// 현재 카메라와 캐릭터 거리
	//	const float CurrCamDistance = (CurrCamLocation - PinpointLocation).Size();

	//	// 이동 예상 위치에서의 카메라 와 캐릭터 거리
	//	const float ExpectMovedDistance = (ExpectCameraLocation - PinpointLocation).Size();

	//	float NewMoveDistance = 0.f;
	//	bool bCanMove = false;
	//	if (IsZoomIn)
	//	{
	//		NewMoveDistance = FMath::Clamp(ExpectMovedDistance, GetMinCamDistance(), ExpectMovedDistance);
	//		bCanMove = !FMath::IsNearlyEqual(NewMoveDistance, GetMinCamDistance());
	//	}
	//	else // ZoomOut
	//	{
	//		NewMoveDistance = FMath::Clamp(ExpectMovedDistance, ExpectMovedDistance, GetMaxCamDistance());
	//		bCanMove = !FMath::IsNearlyEqual(NewMoveDistance, GetMaxCamDistance());
	//	}

	//	if (bCanMove)
	//	{
	//		const float MoveableDelta = FMath::Abs(NewMoveDistance - CurrCamDistance);
	//		const FVector NewCameraLocation = CurrCamLocation + (CameraMoveDir * MoveableDelta);

	//		CurrentCamera->SetActorLocation(NewCameraLocation);
	//	}
	//}
}

void FCameraZoomModule::Zoom(const FVector2D& ScreeenFocus, float Delta)
{
	Zoom(GetPinpointLocation(ScreeenFocus), -Delta * 30.f);
}

FLobbyGuildMercenary::FLobbyGuildMercenary(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyGuildMercenary::~FLobbyGuildMercenary()
{

}

void FLobbyGuildMercenary::OpenScene()
{
	
}

void FLobbyGuildMercenary::CloseScene()
{

}

void FLobbyGuildMercenary::SubscribeEvents_OnConstruct()
{

}

void FLobbyGuildMercenary::UnsubscribeEvents_OnDestruct()
{

}

FLobbyAdventureEnterBattleMain::FLobbyAdventureEnterBattleMain(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyAdventureEnterBattleMain::~FLobbyAdventureEnterBattleMain()
{

}

void FLobbyAdventureEnterBattleMain::OpenScene()
{

}

void FLobbyAdventureEnterBattleMain::CloseScene()
{

}

void FLobbyAdventureEnterBattleMain::SubscribeEvents_OnConstruct()
{

}

void FLobbyAdventureEnterBattleMain::UnsubscribeEvents_OnDestruct()
{

}

FLobbyFairyScene::FLobbyFairyScene(class FLobbySceneManager* OwnerSceneManager)
	: 
      FLobbySceneBase(OwnerSceneManager),
	  CurrentFairyType(ELobbyNPCType::ELobbyNPC_Fairy_Niece),
	bEventsSubscribed(false)
	
{

}

FLobbyFairyScene::~FLobbyFairyScene()
{

}

void FLobbyFairyScene::OpenScene()
{
	//FLobbySceneBase::OpenScene();
	//
	//AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
	//if (LobbyGameMode)
	//{
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // 안전빵.

	//												  // 터치 액션을 위한 입력 바인딩
	//	LobbyGameMode->BindTouchInput(true);
	//	LobbyGameMode->SetEnableClickEvents(true);
	//}

	//AMatineeActor* DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}

	//CurrentFairyType = ELobbyNPCType::ELobbyNPC_Fairy_Niece;
	//FLobbySceneManager* LobbySM = GetSceneManager();
	//AB2LobbyFairyDoumi* FairyDoumiActor = LobbySM ? Cast<AB2LobbyFairyDoumi>(LobbySM->GetNPCSKActor(ELobbyNPCType::ELobbyNPC_Fairy_Niece)) : nullptr;
	//if (FairyDoumiActor)
	//{ 
	//	FairyDoumiActor->PlayTemporaryAnimOfState(ELobbyFairyDoumiState::ELFD_Welcoming);
	//	FairyDoumiActor->SetVisibleCharacter(true);
	//}
}

void FLobbyFairyScene::OnChangeFairyActor(EFairyType eFairyType)
{
	//FLobbySceneManager* LobbySM = GetSceneManager();
	//ELobbyNPCType BeforeType = CurrentFairyType;
	//CurrentFairyType = ConvertFairyTypeToLobbyNPCType(eFairyType);

	//if (BeforeType == CurrentFairyType)
	//	return;

	//// 이전 페어리 안보이도록.
	//AB2LobbyFairyDoumi* FairyBeforeActor = LobbySM ? Cast<AB2LobbyFairyDoumi>(LobbySM->GetNPCSKActor(BeforeType)) : nullptr;
	//if (FairyBeforeActor)
	//{
	//	FairyBeforeActor->SetVisibleCharacter(false);
	//}

	//// 다음 페어리는 보이게하고, 애니메이션 재생
	//AB2LobbyFairyDoumi* FairyDoumiActor = LobbySM ? Cast<AB2LobbyFairyDoumi>(LobbySM->GetNPCSKActor(CurrentFairyType)) : nullptr;
	//if (FairyDoumiActor)
	//{
	//	FairyDoumiActor->PlayTemporaryAnimOfState(ELobbyFairyDoumiState::ELFD_Welcoming);
	//	FairyDoumiActor->SetVisibleCharacter(true);
	//}
}

ELobbyNPCType FLobbyFairyScene::ConvertFairyTypeToLobbyNPCType(EFairyType eFairyType)
{
	ELobbyNPCType FairyNpcType = ELobbyNPCType::ELobbyNPC_None;
	
	switch (eFairyType)
	{
	case EFairyType::FAIRY_NIECE:
		FairyNpcType = ELobbyNPCType::ELobbyNPC_Fairy_Niece;
		break;
	case EFairyType::FAIRY_PANG:
		FairyNpcType = ELobbyNPCType::ELobbyNPC_Fairy_Pang;
		break;
	case EFairyType::FAIRY_RIO:
		FairyNpcType = ELobbyNPCType::ELobbyNPC_Fairy_Rio;
		break;
	}

	return FairyNpcType;
}

void FLobbyFairyScene::OnPlayFairyBlessAnim(EFairyType eFairyType)
{
	//FLobbySceneManager* LobbySM = GetSceneManager();
	//AB2LobbyFairyDoumi* FairyDoumiActor = LobbySM ? Cast<AB2LobbyFairyDoumi>(LobbySM->GetNPCSKActor(ConvertFairyTypeToLobbyNPCType(eFairyType))) : nullptr;
	//if (FairyDoumiActor)
	//{
	//	FairyDoumiActor->PlayTemporaryAnimOfState(ELobbyFairyDoumiState::ELFD_Bless);
	//}
}

void FLobbyFairyScene::CloseScene()
{
//	// 터치 액션을 위한 입력 바인딩 제거.
//	AB2LobbyGameMode* LobbyGameMode = GetLobbyGameMode();
//	if (LobbyGameMode)
//	{
//		LobbyGameMode->BindTouchInput(false);
//		LobbyGameMode->SetEnableClickEvents(false);
//	}
//
//	auto* DefaultCameraMatinee = GetDefaultCameraMatinee();
//
//	if (DefaultCameraMatinee)
//	{
//		DefaultCameraMatinee->Stop();
//	}
//
//	FLobbySceneBase::CloseScene();

}

void FLobbyFairyScene::SubscribeEvents_OnOpen()
{
	if (bEventsSubscribed)
		return;

	Issues.Add(OnChangeFairyCharacterClass<EFairyType>::GetInstance().Subscribe2(
		[this](EFairyType eType)
	{
		this->OnChangeFairyActor(eType);
	}
	));
	Issues.Add(UpdateFairyBlessClass<EFairyType>::GetInstance().Subscribe2(
		[this](EFairyType eType)
	{
		this->OnPlayFairyBlessAnim(eType);
	}
	));

	bEventsSubscribed = true;
}

void FLobbyFairyScene::UnsubscribeEvents_OnClose()
{
	Issues.Empty();
	bEventsSubscribed = false;
}

FLobbyItemForgeScene::FLobbyItemForgeScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyItemForgeScene::~FLobbyItemForgeScene()
{

}

void FLobbyItemForgeScene::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbyItemForgeScene::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

void FLobbyItemForgeScene::SubscribeEvents_OnConstruct()
{
	//DeliveryGetItemForgeListTicket = DeliveryGetItemForgeListClass<FB2ResponseGetItemForgeListPtr>::GetInstance().Subscribe(
	//	[this](const FB2ResponseGetItemForgeListPtr& ItemForgeListPtr)
	//{
	//	/*
	//	if (GetSceneManager()->GetCurrentLobbyScene() != GetLobbyScene())
	//	{
	//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::ItemForge);
	//	}
	//	else*/
	//	{
	//		UB2UIItemForge* ItemForgeUI = UB2UIManager::GetInstance()->GetUI<UB2UIItemForge>(UIFName::ItemForge);
	//		if (ItemForgeUI)
	//		{
	//			ItemForgeUI->ForgeInit();
	//			ItemForgeUI->ResponseItemForgeList(ItemForgeListPtr);
	//		}
	//	}
	//});
}

void FLobbyItemForgeScene::UnsubscribeEvents_OnDestruct()
{
	//DeliveryGetPackageShopClass<FB2ResponseGetPackageShopPtr>::GetInstance().Unsubscribe(DeliveryGetPackageShopTicket);
}

FLobbySealBoxResultScene::FLobbySealBoxResultScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbySealBoxResultScene::~FLobbySealBoxResultScene()
{

}

void FLobbySealBoxResultScene::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbySealBoxResultScene::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

void FLobbySealBoxResultScene::SubscribeEvents_OnConstruct()
{
	//DeliverySealBoxResultTicket = DeliveryUnsealBoxClass<FB2ResponseUnsealBoxPtr>::GetInstance().Subscribe(
	//	[this](const FB2ResponseUnsealBoxPtr& SealBoxResultPtr)
	//{
	//	UB2UISealBoxResult* SealBoxUI = UB2UIManager::GetInstance()->GetUI<UB2UISealBoxResult>(UIFName::SealBoxResult);
	//		if (SealBoxUI)
	//		{
	//			SealBoxUI->Init();
	//			//SealBoxUI->ResponseItemForgeList(ItemForgeListPtr);
	//		}
	//	
	//});
}

void FLobbySealBoxResultScene::UnsubscribeEvents_OnDestruct()
{
	//DeliveryGetPackageShopClass<FB2ResponseGetPackageShopPtr>::GetInstance().Unsubscribe(DeliveryGetPackageShopTicket);
}

FLobbyMission::FLobbyMission(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyMission::~FLobbyMission()
{

}

void FLobbyMission::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbyMission::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

FLobbyDonationScene::FLobbyDonationScene(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
	IsOpenScene = false;
}

FLobbyDonationScene::~FLobbyDonationScene()
{
	//IsOpenScene = false;
	//UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	//if (DocDonation)
	//{
	//	DocDonation->SetIsResultPopup(false);
	//}
}

void FLobbyDonationScene::OpenScene()
{
	FLobbySceneBase::OpenScene();
	IsOpenScene = true;
}

void FLobbyDonationScene::CloseScene()
{
	FLobbySceneBase::CloseScene();
	IsOpenScene = false;
}

void FLobbyDonationScene::SubscribeEvents_OnConstruct()
{
	FLobbySceneBase::SubscribeEvents_OnConstruct();
	Issues.Empty();

	Issues.Add(DeliveryGetDonationEventClass<FB2GetDonationEventPtr>::GetInstance().Subscribe2(
		[this](const FB2GetDonationEventPtr& InData)
	{
		this->ResponseGetDonationEvent(InData);
	}
	));

	Issues.Add(DeliveryMakeDonationClass<FB2MakeDonationPtr>::GetInstance().Subscribe2(
		[this](const FB2MakeDonationPtr& InData)
	{
		this->ResponseMakeDonation(InData);
	}
	));

	Issues.Add(DeliveryDonationPointRewardClass<FB2DonationPointRewardPtr>::GetInstance().Subscribe2(
		[this](const FB2DonationPointRewardPtr& InData)
	{
		this->ResponseDonationPointReward(InData);
	}
	));
}

void FLobbyDonationScene::UnsubscribeEvents_OnDestruct()
{
	FLobbySceneBase::UnsubscribeEvents_OnDestruct();
	Issues.Empty();
}

void FLobbyDonationScene::ResponseGetDonationEvent(const FB2GetDonationEventPtr& InData)
{
	/*if (InData->donation_id > 0)
	{
		UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
		if (DocDonation)
		{
			DocDonation->SetDonationEventData(InData);

			if (DocDonation->GetIsResultPopup() == true)
			{
				UB2UIManager::GetInstance()->CloseUI(UIFName::DonationResultPopup);
			}
		}

		if (IsOpenScene == true)
		{
			UpdateDonationEventClass<>::GetInstance().Signal();
		}
		else
		{
			LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::DonationEvent);
		}
	}
	else
	{
		UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, FString::FromInt(6813)),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm,
			FMsgPopupOnClick::CreateLambda([this]() {
			if (this->GetSceneManager() && this->GetSceneManager()->GetCurrentLobbyScene() == ELobbyScene::ELobbyScene_DonationEvent)
			{
				GoToVillageClass<>::GetInstance().Signal();
			}
		})
		);
	}*/
}

void FLobbyDonationScene::ResponseMakeDonation(const FB2MakeDonationPtr& InData)
{
	//UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	//if (DocDonation)
	//{
	//	DocDonation->SetMakeDonationData(InData);
	//	if (DocDonation->GetIsResultPopup() == false)
	//	{
	//		UB2UIManager::GetInstance()->OpenUI(UIFName::DonationResultPopup);
	//	}

	//	UpdateDonationResultPopupClass<>::GetInstance().Signal();
	//}
}

void FLobbyDonationScene::ResponseDonationPointReward(const FB2DonationPointRewardPtr& InData)
{
	//UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	//if (DocDonation)
	//{
	//	DocDonation->SetDonationPointRewardData(InData);

	//	UpdateDonationPointRewardUIClass<>::GetInstance().Signal();
	//}
}

FLobbyDimensionSelectPage::FLobbyDimensionSelectPage(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{

}

FLobbyDimensionSelectPage::~FLobbyDimensionSelectPage()
{

}

void FLobbyDimensionSelectPage::OpenScene()
{
	FLobbySceneBase::OpenScene();

	SelectCharacterStageInfo(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass());
}

void FLobbyDimensionSelectPage::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->Stop();
	//}

	//auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	//{
	//	if (It.Value().ActorForMain)
	//	{
	//		//영웅관리에서 쓰이는 Actor라서 초기 위치로 돌아가야만 한다.
	//		It.Value().ActorForMain->SetActorLocation(It.Value().CachedInitLocation);
	//	}
	//}
}

void FLobbyDimensionSelectPage::SubscribeEvents_OnConstruct()
{
	Issues.Empty();

	Issues.Add(LobbyStageInfoSelectClass<EPCClass, EPCClass>::GetInstance().Subscribe2(
		[this](EPCClass SelectedMain, EPCClass SelectedSub)
	{
		this->SelectCharacterStageInfo(SelectedMain, SelectedSub);
	}
	));
}

void FLobbyDimensionSelectPage::UnsubscribeEvents_OnDestruct()
{
	Issues.Empty();
}

void FLobbyDimensionSelectPage::SelectCharacterStageInfo(EPCClass MainClass, EPCClass SubClass)
{
	/*auto* DefaultCameraMatinee = GetDefaultCameraMatinee();
	if (DefaultCameraMatinee)
	{
		DefaultCameraMatinee->Stop();
	}

	CurrentMainClass = MainClass;

	if (!(this->GetSceneManager() && this->GetSceneManager()->GetCurrentLobbyScene() == this->GetLobbyScene()))
	{
		return;
	}

	auto& LobbyCharactersForALobbyType = GetLobbyCharactersForATypeLevel();
	for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(LobbyCharactersForALobbyType); It; ++It)
	{
		if (It.Value().ActorForMain)
			CharacterHiddenAndStopSound(It.Value().ActorForMain, true);
		if (It.Value().ActorForSub)
			CharacterHiddenAndStopSound(It.Value().ActorForSub, true);
	}
	if (CurrentMainClass != EPCClass::EPC_End)
	{
		auto* LobbyCharInfo = LobbyCharactersForALobbyType.Find(CurrentMainClass);
		if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
		{
			CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, false);
			LobbyCharInfo->ActorForMain->PlaySelectAnimation(ELobbyScene::ELobbyScene_StageInfo);
		}
	}

	if (GEngine && GLog)
	{
		GEngine->HandleCeCommand(GetLobbyGameMode()->GetWorld(), TEXT("SetDefaultCamActorForSingleCharacterSelect"), *GLog);
	}

	DefaultCameraMatinee = GetDefaultCameraMatinee();

	if (DefaultCameraMatinee)
	{
		DefaultCameraMatinee->SetPosition(0.f);
		DefaultCameraMatinee->Play();

		SetCameraMatineeCam(DefaultCameraMatinee);
	}*/
}

FLobbyDimensionMain::FLobbyDimensionMain(class FLobbySceneManager* OwnerSceneManager)
	: FLobbySceneBase(OwnerSceneManager)
{
}

FLobbyDimensionMain::~FLobbyDimensionMain()
{
}

void FLobbyDimensionMain::OpenScene()
{
	FLobbySceneBase::OpenScene();
}

void FLobbyDimensionMain::CloseScene()
{
	FLobbySceneBase::CloseScene();
}

void FLobbyDimensionMain::SubscribeEvents_OnConstruct()
{
	Issues.Empty();
}

void FLobbyDimensionMain::UnsubscribeEvents_OnDestruct()
{
	Issues.Empty();
}