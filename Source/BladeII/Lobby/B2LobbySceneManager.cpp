
#include "B2LobbySceneManager.h"
//#include "BladeII.h"
#include "Engine/World.h"
#include "B2LobbyGameMode.h"
//
#include "B2LobbyInventory.h"
#include "B2PCClassInfoBox.h"
#include "BladeIIPlayer.h"
#include "Engine/World.h"
#include "B2LobbySkeletalMeshActor.h"
#include "B2HeroMgntSkeletalMeshActor.h"
#include "B2UIManager.h"
#include "B2UIManager_Lobby.h"
////#include "Matinee/MatineeActor.h"
//#include "Matinee/InterpGroup.h"
//#include "Matinee/InterpGroupInst.h"
//#include "Matinee/InterpTrackMove.h"
//#include "Matinee/InterpData.h"
#include "B2UnitedWidgetBase.h"
#include "B2UIModSelect.h"
#include "B2UIDocHelper.h"
#include "B2UIHeroTowerReady.h"
#include "Retailer.h"
#include "B2UIMsgPopupGuildPopup.h"
#include "B2UICollectBookMain.h"
#include "B2UIGuildMain.h"
#include "B2UIRaidMain.h"
#include "B2UIRaidJoin.h"
#include "B2UIGameInviteList.h"
#include "B2ModPlayerInfoData.h"
#include "B2CombatStatEvaluator.h"
#include "B2UILobbyMain.h"
#include "B2GuildGameMode.h"
#include "B2Airport.h"
#include "B2UIGuildBattleHistory.h"
#include "B2UIGuildMapMain.h"
#include "InternalEvents.h"
#include "BladeIIPointCharger.h"
#include "B2UIFindMatchPopup.h"
#include "B2UIOtherUserInfo.h"
#include "B2UISummonItem.h"
#include "B2UISummonItemDisplay.h"
#include "B2UILobbySelectCharacter.h"
#include "B2UIMailRewardPopUp.h"
#include "B2CompositeMeshCache.h"
#include "B2GameLoadingProgress.h"
#include "FakeConfigure.h"
#include "BladeIIGameImpl.h"
#include "B2LobbyNPCSkeletalMeshActor.h"
#include "B2UIRestPointMain.h"
#include "B2UIChatting.h"
#include "B2UIPackageStore.h"
#include "B2UIMagicStore.h"
#include "TutorialManager.h"
#include "B2RaidInfo.h"
#include "B2UIHallOfFame.h"
#include "B2UIHeader.h"
#include "B2UILinkManager.h"
#include "B2CostumeDataStore.h"
#include "BladeIIGameMode.h"
#include "B2GameDefine.h"
#include "B2UIMsgPopupSelectFairyGift.h"
#include "B2UIDocDonation.h"
#include "B2UIItemForge.h"
#include "B2UISealBoxResult.h"
#include "BladeIIUtil.h"
#include "FairyManager.h"
#include "DF.h"
#include "StereoRendering.h"
#include "../Fulfil/FairyManager.h"
#include "LevelSequence/Public/LevelSequenceActor.h"

#define SWIPE_NEXTSTAGE_POS 1.7
#define SWIPE_PREVSTAGE_POS 0.3

#define CHAR_ANIM_ROTATION 0
#define CHAR_ACTOR_ROTATION !CHAR_ANIM_ROTATION
#define CHAR_ROTATION_USE_DEACCEL 1

static void CharacterStopSound(ASkeletalMeshActor* Actor)
{
	if (!Actor)
		return;

	TArray<UAudioComponent*> AudioComponents;
	Actor->GetComponents<UAudioComponent>(AudioComponents);

	for (UAudioComponent* AudioComp : AudioComponents)
	{
		if (AudioComp && AudioComp->IsPlaying())
		{
			AudioComp->Stop();
		}
	}
}

static void CharacterHiddenAndStopSound(ASkeletalMeshActor* Actor, bool bHidden)
{
	if (!Actor)
		return;

	Actor->SetActorEnableCollision(!bHidden);
	Actor->SetActorHiddenInGame(bHidden);

	if (bHidden)
	{
		CharacterStopSound(Actor);
	}
}

void SetMatineeHiddenInGame(ALevelSequenceActor* Matinee, bool bHidden)
{
	//Matinee->SetActorHiddenInGame(bHidden);

	//for (auto& GroupActorInfo : Matinee->GroupActorInfos)
	//{
	//	for (auto* Actor : GroupActorInfo.Actors)
	//	{
	//		if (Actor)
	//			Actor->SetActorHiddenInGame(bHidden);
	//	}
	//}

	//if (!LevelSequenceActor)
	//	return;
	//
	//// Òþ²Ø LevelSequenceActor ±¾Éí
	//LevelSequenceActor->SetActorHiddenInGame(bHidden);

	//// »ñÈ¡°ó¶¨µÄ LevelSequence ×ÊÔ´
	//ULevelSequence* LevelSequence = Cast<ULevelSequence>(LevelSequenceActor->GetSequence());
	//if (!LevelSequence)
	//	return;

	//UMovieScene* MovieScene = LevelSequence->GetMovieScene();
	//if (!MovieScene)
	//	return;

	//// »ñÈ¡°ó¶¨ÐÅÏ¢ÁÐ±í
	//const TArray<FMovieSceneBinding>& Bindings = MovieScene->GetBindings();

	//for (const FMovieSceneBinding& Binding : Bindings)
	//{
	//	// ¹¹Ôì°ó¶¨ ID
	//	FMovieSceneObjectBindingID BindingID(Binding.GetObjectGuid(), MovieSceneSequenceID::Root, EMovieSceneObjectBindingSpace::Local);

	//	// »ñÈ¡°ó¶¨¶ÔÏó
	//	TArray<UObject*> BoundObjects;
	//	LevelSequenceActor->GetSequencePlayer()->FindBoundObjects(BindingID, BoundObjects);

	//	for (UObject* BoundObject : BoundObjects)
	//	{
	//		if (AActor* BoundActor = Cast<AActor>(BoundObject))
	//		{
	//			BoundActor->SetActorHiddenInGame(bHidden);
	//		}
	//	}
	//}
}

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
	/* ±×³É À§¿¡ ¾º¿©Áö´Â UIµé ¼³Á¤µéÀº Ãß°¡ÇÏÁö ¾Ê¾Æµµ µÊ.	
	case ELobbyScene::ELobbyScene_Mail:
	*/
		return ELobbyLevelType::ELobbyLevelType_A;

	case ELobbyScene::ELobbyScene_LobbyMain:
	case ELobbyScene::ELobbyScene_CharacterObserve:
		return ELobbyLevelType::ELobbyLevelType_B;

	case ELobbyScene::ELobbyScene_Chapter: // OptionalChapterNum ¿¡ µû¶ó ELobbyLevelType À» ±¸ºÐÇØ¼­ ¸®ÅÏ
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

	//	//SimpleDirectionalLight regist ¹®Á¦·Î RemoveFromWorld¸¦ ¸ÕÀú ºÒ·¯¾ßµÊ.
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
	//		if (CurrentLobbyScene != ELobbyScene::ELobbyScene_Title && // Å¸ÀÌÆ²¿¡¼­ °¥ ¶§´Â ¿ÀÈ÷·Á ½ß¶×¸ÂÀº ·Îµù È­¸éÀÌ ¶Ç ³ª¿Â´Ù ½Í¾î¼­ Á» ±×·¸´Ù..				
	//			(ToChangeScene == ELobbyScene::ELobbyScene_LobbyMain || ToChangeScene == ELobbyScene::ELobbyScene_CharacterObserve)
	//			&& !StreamingLevel->IsLevelLoaded())
	//		{ // ·Îºñ¿¡ ÀÌ¹Ì µé¾î¿Â ÈÄ¿¡ LobbyMain ¼­ºê·¹º§À» ·ÎµùÇØ¾ß ÇÏ´Â »óÈ²Àº Æ¯È÷ ±ä ½Ã°£À» ¿ä±¸ÇÏ¹Ç·Î ·Îµù È­¸éÀ» ¶ç¿î´Ù.
	//			// º¸Åë ÀüÅõ¿¡¼­ µ¹¾Æ¿Â ÈÄ¿¡ LobbyMain À¸·Î °¥¶§ ±×·¸°Ô µÈ´Ù.
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
	//		GameMode->SetupLoadingScreenImageCommon(false, false, false); // ÁøÇàµµ´Â ¾øÀÌ ·ÎµùÈ­¸é¸¸ ¶ç¿ò.
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

bool FLobbyLevelVisibleManager::IsStreamingLevelLoaded(ELobbyScene CheckScene, int32 OptionalChapterNum /*OptionalChapterNum Àº Ã©ÅÍ ¾ÀÀÎ °æ¿ì ÇÊ¿ä*/)
{
	TArray<FName> LevelNameList;
	LevelsRequiredScene.MultiFind(GetLobbyLevelType(CheckScene, OptionalChapterNum), LevelNameList);
	AB2LobbyGameMode* LobbyGM = LobbySceneManager ? LobbySceneManager->GetLobbyGameMode() : NULL;
	UWorld* TheWorld = LobbyGM ? LobbyGM->GetWorld() : nullptr;
	if (TheWorld)
	{
		for (auto& PrefetchLevelName : LevelNameList)
		{
			ULevelStreaming* StreamingLevel = TheWorld->GetLevelStreamingForPackageName(PrefetchLevelName);
			if (!StreamingLevel || !StreamingLevel->IsLevelLoaded()) {
				return false;
			}
		}
	}
	return true;
}
/* 
 * ÃÖÀûÈ­ ¸ñÀûÀ¸·Î ¹é±×¶ó¿îµå·Î ¹Ì¸® ·ÎµùÇØ µÎ´Â ±â´É. »ç¿ëÀº ÀûÀýÇÑ ½ÃÁ¡¿¡.. Àß¸ø »ç¿ëÇÏ¸é ¿ÀÈ÷·Á ºÎÀÛ¿ëÀÌ ¹ß»ýÇÒ ¼ö ÀÖ´Ù.
 */
void FLobbyLevelVisibleManager::PrefetchStreamingLevel(ELobbyScene PrefetchScene)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("FLobbyLevelVisibleManager::PrefetchStreamingLevel %d"), (int32)PrefetchScene));

	// Ã©ÅÍ ¾Àµµ ¹º°¡ ÇØ º¸·Á¸é ¹øÈ£ ¸Ô¿©ÁÖ¸é µÇ´Âµ¥ ÀÚÄ©ÇÏ¸é »ç¿ë ¾È ÇÏ´À´Ï¸¸ ¸øÇÑ ÀÌ·± ¹Î°¨ÇÑ ±â´ÉÀ» ±×·¸°Ô ³Î¸® »ç¿ëÇÒ °¡´É¼ºÀ» ÁÖ´À´Ï ±×³É Á¦°ø ¾ÈÇÏ°í ¸»Áö.
	check(PrefetchScene != ELobbyScene::ELobbyScene_Chapter);

	TArray<FName> LevelNameList;
	LevelsRequiredScene.MultiFind(GetLobbyLevelType(PrefetchScene), LevelNameList);

	auto* GameMode = LobbySceneManager ? LobbySceneManager->GetLobbyGameMode() : NULL;
	UWorld* TheWorld = GameMode ? GameMode->GetWorld() : nullptr;
	if (TheWorld)
	{
		for (auto& PrefetchLevelName : LevelNameList)
		{// bShouldBlokcOnLoad ¾È ÁÖ°í ·Îµù ÄÝ. ±×³É ÀÌ·¸°Ô ÇØ µÎ°í ³»ºñµÎ´Â °Å.
			UGameplayStatics::LoadStreamLevel(TheWorld, PrefetchLevelName, false, false, FLatentActionInfo());
		}
	}
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
		// LobbyScene ÀÌ Á¦°ÅµÉ ¶§±îÁö ³²¾Æ ÀÖÀ» event subscribe.
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
	//// OptionalToChangeChapterNum Àº ELobbyScene_Chapter ÀÇ °æ¿ì¸¸
	//if (CurrentLobbyScene == ToChangeScene && !bForceChange && (ToChangeScene != ELobbyScene::ELobbyScene_Chapter || CurrentChapterViewNum == OptionalToChangeChapterNum))
	//	return;

	////Close
	//uint8 CurLobbySceneIndex = static_cast<uint8>(CurrentLobbyScene);
	//if (LobbyScenes.IsValidIndex(CurLobbySceneIndex))
	//	LobbyScenes[CurLobbySceneIndex]->CloseScene();

	////¹®Á¦°¡ ÀÖ¾î¼­ UISceneÀº ¹Ì¸® º¯°æ.. ¿µ¿õ°ü¸®¾À (FLobbyHeroMgmtScene) DJLegacy UI ´Â ÀÌ°É·Î ¸ÔÈ÷Áö´Â ¾ÊÀ» °Å°í.. ±× °æ¿ì´Â UI º¯°æ Ä¿¸Çµå´Â µû·Î °¡µµ·Ï ÇØ¾ß (LobbyEnterHeroMgmtModeClass ¸¦ ÅëÇØ)
	//uint8 ToChangeSceneInt = static_cast<uint8>(ToChangeScene);
	//if (LobbyScenes.IsValidIndex(ToChangeSceneInt))
	//{
	//	EUIScene ToChangeUIScene = LobbyScenes[ToChangeSceneInt]->GetUIScene();
	//	if (UB2UIManager::GetInstance()->GetCurrUIScene() != ToChangeUIScene)
	//	{
	//		UB2UIManager::GetInstance()->ChangeUIScene(ToChangeUIScene);

	//		B2GMLoadingProgCollector::Step(); // ¸Ê ·Îµù Å¸ÀÓ¿¡ ·Îµù ÁøÇàµµ ³ÖÀ¸·Á´Â °Çµ¥ ¸Ê ·Îµù ½ÃÁ¡ ¾Æ´Ï¸é º° ÀÏ ¾øÀ» °Í
	//	}
	//}

	//const ELobbyLevelType CurrLevelType = LevelManager.GetLobbyLevelType(CurrentLobbyScene, CurrentChapterViewNum);
	//const ELobbyLevelType ToChangeLevelType = LevelManager.GetLobbyLevelType(ToChangeScene, OptionalToChangeChapterNum);

	//bool bToLoadNewLevels = (CurrLevelType != ToChangeLevelType && ToChangeLevelType != ELobbyLevelType::ELobbyLevelType_None)
	//	// ¿©±â¼­ LevelType ¸¸À¸·Î´Â ½ÇÁ¦ ¶È°°Àº ½ºÆ®¸®¹Ö ·¹º§À» Unload/Load ÇÒÁö ¿ÏÀüÈ÷ ¾Ë ¼ö°¡ ¾ø¾î¼­ Ãß°¡ °Ë»ç. °°Àº ½ºÆ®¸®¹Ö ·¹º§À» ³»·È´Ù ´Ù½Ã ¿Ã¸®´Â °Ô ¾Æ±î¿ö¼­ ±×·¯´Â °Ô ¾Æ´Ï¶ó Á» ¹®Á¦°¡ ÀÖ¾î º¸ÀÓ.
	//	&& !LevelManager.HasSameStreamingLevelToCurrentScene(ToChangeScene, CurrentChapterViewNum, OptionalToChangeChapterNum);

	//if (bToLoadNewLevels)
	//{
	//	ResetObjectRefsOnSceneLevelStreaming();

	//	LevelManager.ChangeVisibility(ToChangeScene, CurrentChapterViewNum, OptionalToChangeChapterNum);

	//	B2GMLoadingProgCollector::Step(); // ¸Ê ·Îµù Å¸ÀÓ¿¡ ·Îµù ÁøÇàµµ ³ÖÀ¸·Á´Â °Çµ¥ ¸Ê ·Îµù ½ÃÁ¡ ¾Æ´Ï¸é º° ÀÏ ¾øÀ» °Í
	//}

	//// »õ·Î¿î ·¹º§À» ·ÎµùÇÏ´Â °æ¿ì°¡ ¾Æ´Ï¶óµµ Ä³¸¯ÅÍ Á¶ÇÕÀ» ´Ù½Ã ÇØ¾ß ÇÏ´Â °æ¿ì.. ÇÊ¿ä¿¡ µû¶ó Ãß°¡. µµ°¨Àº Ä³¸¯ÅÍ Àåºñ Á¶ÇÕÀ» ÀÓÀÇ·Î ÇÏ¹Ç·Î ³ª°¥ ¶§ ¹«Á¶°Ç »õ·Î Á¶ÇÕÇÒ ÇÊ¿ä°¡ ÀÖ´Ù.
	//const bool bUnconditionalUpdatePart = (CurrentLobbyScene == ELobbyScene::ELobbyScene_CollectBookMain);

	////woong Ä³¸¯ÅÍ»ó¼¼º¸±â Ä³¸¯ÅÍ°¡ Ç×»ó 4°³°¡ ¾Æ´Ï¹Ç·Î ¿©±â¼­ Ä³¸¯ÅÍ ¾÷µ¥ÀÌÆ®¸¦ ¸øÇÏ°ÔÇØ¾ßÇÕ´Ï´Ù
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

	////[@AKI, 170718] <B2CLT-1789> ·Îºñ ¿¬Ãâ °³¼±À¸·Î ÁÖ¼® Ã³¸® ÇÔ.
	////ÃßÈÄ ÀüÃ¼ Ä³¸¯µéÀÌ ³ª¿ÍÀÖÀ»¶§ Ä³¸¯¸í µîÀ» º¸¿© ÁÖ°í ½ÍÀ¸¸é ÁÖ¼®À» ÇØÁ¦ ÇÏ¸é µÊ.
	////-> WidgetClass ·Îµù ¹× CreateWidget Àº ³¶ºñ°¡ µÇ¹Ç·Î ºñÈ°¼ºÈ­ ÇØ ³õÀº »óÅÂ.. º¹±¸ÇÏ·Á¸é RESTORE_BII_LOBBY_ONHEAD_DISPLAY µðÆÄÀÎÀ» ÄÑ¾ß.
	////UpdateLobbyOnHeadDisplayWidget();

	////Open
	//CurLobbySceneIndex = static_cast<uint8>(CurrentLobbyScene);
	//if (LobbyScenes.IsValidIndex(CurLobbySceneIndex))
	//	LobbyScenes[CurLobbySceneIndex]->OpenScene();
}

bool FLobbySceneManager::IsSceneStreamingLevelLoaded(ELobbyScene CheckScene, int32 OptionalChapterNum)
{ // ³Î¸® »ç¿ëÇÒ °Å ¾Æ´Ï¶óµµ ÀÌ°Ç ÇÔ¼ö ÀÇ¹Ì »ó ±â´ÉÀ» ¿ÏÀüÇÏ°Ô ¸¸µé ÇÊ¿ä´Â ÀÖ¾î¼­ Chapter ¾ÀÀÇ °æ¿ìµµ Ã³¸®..¤»
	return LevelManager.IsStreamingLevelLoaded(CheckScene, OptionalChapterNum);
}
void FLobbySceneManager::PrefetchSceneStreamingLevel(ELobbyScene PrefetchScene)
{
	// ·Îºñ ¼­ºê·¹º§ Áß µ¢Ä¡ Å« °É Àû´çÇÑ ½ÃÁ¡¿¡ ¹Ì¸® ·ÎµùÇÒ ¶§ »ç¿ë.
	// ´Ù¸¥ ¿¡¼Â ·Îµù°ú ¾ôÇô¼­ flush °¡ ½±°Ô À¯¹ßµÇ´Â »óÈ²ÀÌ¸é ºÎÀÛ¿ëÀÌ ´õ Å¬ ¼ö ÀÖÀ¸´Ï ÀûÀýÇÑ ½ÃÁ¡¿¡ Àß ¾Ë°í »ç¿ë.
	if (PrefetchScene != ELobbyScene::ELobbyScene_Chapter) // ¹øÈ£ ³Ñ°ÜÁÖ±â ½È¾î¼­ Ã©ÅÍ ¾À Á¦¿Ü. ±×´ÙÁö ³Î¸® »ç¿ëµÉ ±â´Éµµ ¾Æ´Ï°í.. ³Î¸® »ç¿ëÇØ¼­ ÁÁÀ» °Íµµ ¾ø°í.
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
	//		if (!bPrevForLobbyMainSceneAMB) // ÀÌÀü¿¡ ÇÃ·¹ÀÌÇÏÁö ¾ÊÀº °æ¿ì¸¸. ¾Æ¹«·¡µµ ÀÌ°Å ²÷±â¸é ¾ÈÁÁ¾ÆÇÒ °Å °°À½.
	//		{
	//			GameMode->PlayAMBByCommonSoundID(ECommonSoundID::ECSID_AMB_LobbyMainScene);
	//		}
	//	}
	//	else
	//	{
	//		GameMode->StopAMB(); // ¾ÆÁ÷Àº ´Ù¸¥ Àå¸é¿¡ AMB °¡ ¾øÀ½.
	//	}
	//}
}

void FLobbySceneManager::UpdateLobbyOnHeadDisplayWidget(EPCClass OptionalCreateOnlyClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::UpdateLobbyOnHeadDisplayWidget"));

	//// ÀÏ´Ü ±âÁ¸¿¡ ¸¸µç °Å Á¦°Å
	//DestroyAllLobbyCharOnHeadDisplayClass<>::GetInstance().Signal();

	//// Å¬·¡½ºº°·Î µ¹¸é¼­ Àç»ý¼º. ºÙÀÌ°íÀÚ ÇÏ´Â °Ç TypeB ¾ÖµéÀÌ´Ù.
	//if (LevelManager.GetLobbyLevelType(CurrentLobbyScene) == ELobbyLevelType::ELobbyLevelType_B)
	//{
	//	for (TMap<EPCClass, FLobbyCharacter>::TIterator It(LobbyCharactersForBLobbyType); It; ++It)
	//	{
	//		EPCClass ThisClass = It.Key();
	//		FLobbyCharacter& ThisCharInfo = It.Value();
	//		if (ThisClass != EPCClass::EPC_End && ThisCharInfo.LevelActor && ThisCharInfo.LevelActor->IsValidObj() && 
	//			// Observe Àå¸éÀÎ °æ¿ì OptionalCreateOnlyClass ¸¦ ÁöÁ¤ÇØ¼­ ÇöÀç Ä³¸¯ÅÍ ¾Æ´Ñ °ÍµéÀ» ¸í½ÃÀûÀ¸·Î °É·¯³»°í, ¼û°Ü³õÀº °Íµµ.
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
{ // ¾Æ½Î¸® ¿ÜºÎ¿¡¼­ °­Á¦ ¾÷µ¥ÀÌÆ® Á¤µµÀÇ ¿ëµµ.. ÀÖ¾î¾ß ÇÒ °æ¿ì¿¡¸¸ ¾÷µ¥ÀÌÆ®.
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

void FLobbySceneManager::SetGuildMapChangeMatinee(class ALevelSequenceActor * Atype, class ALevelSequenceActor * Btype)
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
	FLobbyNPCCharacterInfo& FoundOrAddElem = LobbyNPCs.FindOrAdd(InType); // Áßº¹ È£ÃâµÉÁö ¸ô¶ó¼­
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
 * bForceUpdate ´Â ÇÊ¼öÀûÀÎ »óÈ²ÀÌ ¾Æ´Ï¸é true ·Î ÁÖÁö ¸» °Í. bForceUpdate ¸¦ ÁÖ´Â °Ô µ¿ÀÛ¿¡ ÀÌ»óÀ» ÀÏÀ¸Å°Áö´Â ¾ÊÁö¸¸ ¼º´É¿¡ Áã¾àÀÌ µÉ ¼ö ÀÖÀ½.
 * ÇÊ¼öÀûÀÎ »óÈ²Àº µµ°¨°°ÀÌ ÀÏ½ÃÀûÀ¸·Î ´Ù¸¥ Àåºñ ÀåÂøÀ» º¸¿©ÁØ´Ù°Å³ª, Ä¡Æ®¸¦ ³¯¸®´Â »óÈ² µîÀÌ µÇ°Ú´Ù.
 * ±âº»ÀûÀ¸·Î °¢ Ä³¸¯ÅÍ Å¬·¡½º º°·Î merge ÇÑ ¸Þ½¬´Â ÀåÂø »óÅÂ, ³¯°³ ¿©ºÎ¿¡ µû¶ó refresh ¸¦ ÇØ¾ßÇÏ´ÂÁö ÆÇº°À» ÇÔ.
 */
void FLobbySceneManager::SetupLobbyCharCompForParts(EPCClass InPCClass, class ASkeletalMeshActor* InSKActorToSet, bool bForceUpdate /*= false*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::SetupLobbyCharCompForParts"));
//
//	// ABladeIIPlayer::SetupComponentsForParts ¿¡ ÇØ´ç. ¿©±â¼± ·Îºñ¸Ê¿¡ ¹Ú¾Æ³õÀº SkeletalMeshActor ´ë»óÀ¸·Î
//
//	if (InPCClass == EPCClass::EPC_End || !InSKActorToSet || !InSKActorToSet->IsValidObj() || !InSKActorToSet->GetWorld()){
//		return;
//	}
//
//	TArray<FB2Item> EquippedItems;
//	
//	AB2LobbyGameMode* B2LobbyGM = GetLobbyGameMode();
//	BII_CHECK(B2LobbyGM);
//	// ¸Ç Ã³À½ ³ª¿À´Â Ä³¸¯ÅÍ ¼Ò°³ È­¸é¿¡¼­´Â 6¼º Àåºñ ÀåÂøµÈ °ÍÀ» º¸¿©ÁØ´Ù.
//	const bool bUseFixedGradeEquip = (CurrentLobbyScene == ELobbyScene::ELobbyScene_CharacterIntro);
//		//|| (CurrentLobbyScene == ELobbyScene::ELobbyScene_Title) // Å¸ÀÌÆ² È­¸é¿¡¼­´Â ÀÌÁ¦ µ¿¿µ»óÀÌ ³ª¿Â´Ù..
//	bForceUpdate |= bUseFixedGradeEquip; // ¼Ò°³ È­¸é 6¼º Àåºñ ÀåÂøÀº ForceUpdate.
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	// Æ¯Á¤ º° ¼º ¼öÀÇ Àåºñ¸¦ ÀÏ°ý Âø¿ëÇÒ ¼ö ÀÖµµ·Ï ÇÏ´Â Ä¡Æ®ÀÎµ¥ SetupSKCompForParts ¾È¿¡¼­ Àåºñ¸¦ º¯°æÇÏ¹Ç·Î ForceUpdate ½ÅÈ£¸¦ º¸³»ÁÖ¾î¾ß ÇÑ´Ù.
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
//	// ÄÄÆ÷Áþ ¸Þ½¬ °á°ú¹°À» Ä³½ÌÇØ ³õÀ»Áö ¿©ºÎ. 
//	// ·Îºñ¿¡¼­´Â ÇöÀç Á¶ÇÕÀ» ÇÏ´Â °Ô ·ÎÄÃ Ä³¸¯ÅÍ ¿ëÀÎÁö ¾Æ´ÑÁö ±¸ºÐÀ» ÇÒ ¼ö ÀÖ´Â ´Ù¸¥ ÀûÀýÇÑ Á¤º¸°¡ ¾øÀ¸¹Ç·Î ¿©±â¼­ Àß ³Ñ°ÜÁÖ¾î¾ß ÇÔ.
//	bool bCanCacheCompositedMesh = true;
//
//	if (bUseFixedGradeEquip)
//	{
//		bCanCacheCompositedMesh = false; // ÀÓ½ÃÀûÀÎ °ÍÀ¸·Î °á°ú¹° Ä³½Ì ¾ÈÇÔ.
//		// ¸ðµÎ 6¼ºÀ¸·Î ÇÏ´Â ´ë½Å Ä³¸¯ÅÍº°·Î ¿äÃ»ÇÑ grade °¡ ÀÖ¾î¼­ ±×°É·Î..
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
//		// Æ¯º°È÷ ÁØºñµÈ ÇÔ¼öÀÓ. ¸ðÁ¾ÀÇ ±Ô¾à¿¡ µû¶ó RefID Á¶ÇÕÇÏ°í ÇÊ¿äÇÑ °ªµé¸¸ Ã¤¿ö¼­ ³Ö¾îÁÜ.
//		GetStandardDummyPCPartItemData(InPCClass, StandardDummyPCPartItemGrade, EquippedItems);
//	}
//	else if (CurrentLobbyScene == ELobbyScene::ELobbyScene_CollectBookMain)
//	{
//		bCanCacheCompositedMesh = false; // ÀÓ½ÃÀûÀÎ °ÍÀ¸·Î °á°ú¹° Ä³½Ì ¾ÈÇÔ.
//		UB2UICollectBookMain* CollectBookUI = UB2UIManager::GetInstance()->GetUI<UB2UICollectBookMain>(UIFName::CollectBookMain);
//		if (CollectBookUI)
//		{
//			bHasWing = false; // µµ°¨¿¡¼­´Â WingTab µé¾î°¡±â Àü±îÁö´Â ¾È ³ª¿È
//
//			EquippedItems = CollectBookUI->GetCollectBookCurrentEquipItems();
//			const int32 WingIndex = static_cast<int32>(EItemClass::EIC_Wing);
//			if (EquippedItems.IsValidIndex(WingIndex) && EquippedItems[WingIndex].ItemClass == EItemClass::EIC_Wing)
//			{
//				// WingTab¿¡ µé¾î°¡¸é ÀÚµ¿À¸·Î EquippedItems¿¡ µî·ÏµÇ°í ÀåÂø!
//				bHasWing = true;
//				// ³¯°³ ¿Ü°üÀº Å¬·¡½º¿Í ÁøÈ­ µî±Þ¿¡ µû¶ó¼­¸¸ °áÁ¤.
//				WingData.OwnerPCClass = PCClassToInt(InPCClass);
//				WingData.EvolutionGrade = EquippedItems[WingIndex].StarGrade;	// ÁøÈ­´Â StarGrade¿¡ ¹ÚÀÚ
//				
//				WingData.bShouldBeVisible = true; // µµ°¨ÀÌ¸é FLobbyCollectBookMain ¿¡¼­ SetUnconditionalShowWing À» ÇÏ´Â ´ë½Å ¿©±â¼­ ÀÌ°É Á÷Á¢ ³ÖÀÚ. ¸¶Âù°¡Áö±ä ÇÒ °Çµ¥ ¶æÀÌ Á» ´Ù¸¥ µí..
//			}
//		}
//	}
//	else if (CurrentLobbyScene == ELobbyScene::ELobbyScene_FindAccount
//		&& bCanUseRemoteInfoForSKMeshSetup // CurrentLobbyScene ¸¸ Ã¼Å©ÇÏ¸é È£Ãâ ½ºÅÃ »óÀÇ Å¸ÀÌ¹ÖÀ¸·Î ÀÎÇØ ½ÇÁ¦·Î´Â ·ÎÄÃ Ä³¸¯ÅÍ¸¦ º¸¿©ÁÖ´Â ºä·Î °¡´Â ½ÃÁ¡¿¡ ¿©±â¿¡ °É·Á¼­ Å©·¡½¬°¡ ¹ß»ýÇÒ ¼ö ÀÖÀ¸¹Ç·Î Ãß°¡ Ã¼Å©¸¦ ÇÑ´Ù.
//		)
//	{
//		bCanCacheCompositedMesh = false; // ÀÓ½ÃÀûÀÎ °ÍÀ¸·Î °á°ú¹° Ä³½Ì ¾ÈÇÔ. ´Ù¸¥ ÇÃ·¹ÀÌ¾î ¸Þ½¬´Â Ä³½ÌÇÏ´Â °æ¿ì ¿ªÈ¿°ú°¡ ³¯ ¼ö ÀÖ´Ù.
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
//				bCanCacheCompositedMesh = false; // ÀÓ½ÃÀûÀÎ °ÍÀ¸·Î °á°ú¹° Ä³½Ì ¾ÈÇÔ. ´Ù¸¥ ÇÃ·¹ÀÌ¾î ¸Þ½¬´Â Ä³½ÌÇÏ´Â °æ¿ì ¿ªÈ¿°ú°¡ ³¯ ¼ö ÀÖ´Ù.
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
//		// ÀÏ¹ÝÀûÀÎ ·ÎÄÃ ÇÃ·¹ÀÌ¾î Ä³¸¯ÅÍÀÇ °æ¿ì
//		UB2LobbyInventory::GetAllEquippedItem(EquippedItems, InPCClass);
//		UB2LobbyInventory::GetAllEquippedCostumeItem(EquippedItems, InPCClass);
//		bHasWing = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InPCClass, WingData);
//	}
//
//	if (bHasWing && bUnconditionalShowWing) // ³¯°³ ÁøÈ­ Àå¸éÀÌ³ª µµ°¨ µî¿¡¼­ µé¾î°¥ Á¶°Ç. ±âº»ÀûÀ¸·Î bShouldBeVisible Àº »ç¿ëÀÚ ¼±ÅÃ¿¡ µû¶ó ÀúÀåµÇ´Â °ª.
//	{
//		WingData.bShouldBeVisible = true;
//	}
//
//	// ¿¬ÃâOn½Ã ³¯°³ Á¦°Å
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_CharacterPage)
//	{
//		UB2UIDocUICondition* DocCon = UB2UIDocHelper::GetDocUICondition();
//
//		if(!DocCon->GetbUserSelectedSkipStory())
//			WingData.bShouldBeVisible = false;
//	}
//
//	//Mesh Render Item Filtering (¹Ì¸®º¸±â, visible ¿É¼Ç)
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
//	const bool bMergeSections = false; // ·Îºñ´Â Section Merge ¸¦ ¾µ °¡´É¼ºÀ» Á¤¸» »ý°¢ÇÏ±â ¾î·Á¿ì³ª °Ë»ö ÅÂ±× Á¤µµ´Â ´Þ¾ÆµÎÀÚ ¤» UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent
//
//	if (ThisClassInfo && BaseMeshComp)
//	{
//		USkeletalMesh* PrebuiltMeshOrResult = NULL;
//		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//		if (!bForceUpdate && bCanCacheCompositedMesh)
//		{ // Mesh merge ÇÏ´Â ¸ðµå ÇÑÁ¤, ÀÌÀü¿¡ ÀÌ¹Ì ¶È°°Àº Á¶°ÇÀ¸·Î ºôµåÇØ ³õÀº ¸Þ½¬°¡ ÀÖ´Ù¸é ±×°É ²¨³» ¾´´Ù. 
//			// ÇöÀç ·Îºñ¿¡¼­´Â PCClass ¸¸À» Å°·Î ÇØ¼­ ºôµåÇÑ ¸Þ½¬¸¦ Ä³½ÌÇÏ´Âµ¥
//			// ¸¸ÀÏ PCClass ´Â °°°í ¼­·Î ´Ù¸¥ SkeletalMeshActor °¡ ÇÑ¹ø¿¡ º¸¿©¾ß ÇÏ´Â »óÈ²ÀÌ ¿À¸é PCCompositeMeshCacher ÀÇ Å°°ªÀÌ ±×°É ¹Ý¿µÇÒ ¼ö ÀÖµµ·Ï ÇØ¾ß.
//			PrebuiltMeshOrResult = MeshCacheManager ? 
//				MeshCacheManager->GetCachedCompositeMesh(InPCClass, NULL, EquippedItems, (bHasWing && WingData.bShouldBeVisible), &WingData, bMergeSections)
//				: NULL; 
//		}
//
//		if (UB2PCClassInfo::SetupSKCompForParts(InPCClass, InSKActorToSet, BaseMeshComp, PrebuiltMeshOrResult, ThisClassInfo->BaseMeshAsset, ThisClassInfo->DefaultPartsInfo,
//			// Ã¹ È­¸é 6¼º Àåºñ Á¶ÇÕ º¸¿©ÁÙ ¶§´Â ³¯°³¸¦ »© º¸ÀÚ. °Å±â¼­ ¾îÂ÷ÇÇ º¸ÀÌÁö ¸»ÀÚ°í ÇÒ °Å °°´Ù.
//			EquippedItems, (bHasWing && !bUseFixedGradeEquip) ? &WingData : NULL, NULL,
//			bForceUpdate, // ForceUpdate ÀÇ °æ¿ì´Â ÀÏ½ÃÀûÀÎ preview ·Î º¸°íÀÚ ÇÔÀÌ Å©±â ¶§¹®¿¡ »ç¿ëÇÑ ÆÄÆ® ¸®¼Ò½º¸¦ ³¯¸®µµ·Ï ÇÔ.
//			bMergeSections
//			))
//		{
//			if (MeshCacheManager && bCanCacheCompositedMesh)
//			{ // MeshMerging ÀÌ ½ÇÇàµÇ´Â ¸ðµå¿¡¼­ÀÇ °á°ú°¡ ¼º°øÀûÀÌ¶ó¸é CompositeMeshCache ÂÊ¿¡ Ä³½ÌÇØ ³õÀ½. ³»ºÎÀûÀ¸·Î´Â ½ÇÁ¦ ¾÷µ¥ÀÌÆ®°¡ µÇ¾úÀ» ¶§¿¡¸¸ Ä³½ÌÀÌ µÉ °ÍÀÓ.
//				MeshCacheManager->SetCachedCompositeMesh(InPCClass, NULL, PrebuiltMeshOrResult, EquippedItems, (bHasWing && WingData.bShouldBeVisible), &WingData, bMergeSections);
//			}
//		}
//
//		ABladeIIPlayer::SetupLODInfoForLobbyRepresentative(InSKActorToSet);
//	}
}


/** 
 * ¼º´É¿¡ Áã¾àÀÌ µÉ ¼ö ÀÖ¾î¼­ °­Á¦ ¾÷µ¥ÀÌÆ® ÀÎÀÚ´Â Á¦°øÇÏÁö ¾Ê´Â´Ù.
 * ±âº»ÀûÀ¸·Î °¢ Ä³¸¯ÅÍ Å¬·¡½º º°·Î merge ÇÑ ¸Þ½¬´Â ÀåÂø »óÅÂ, ³¯°³ ¿©ºÎ¿¡ µû¶ó refresh ¸¦ ÇØ¾ßÇÏ´ÂÁö ÆÇº°À» ÇÔ.
 */
void FLobbySceneManager::UpdateLobbyCharCompForParts(EPCClass InPCClass)
{ // Àåºñ ÀåÂø »óÅÂ ¾÷µ¥ÀÌÆ® µîÀÇ °æ¿ì »ç¿ëÇÏ±â À§ÇÔ.
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
	//		// G ÄÅ Å¸ÀÔÀº ¿µ¿õ°ü¸®¸¸ ¾÷µ¥ÀÌÆ®.. µµ°¨Àº ÀÌ·¸°Ô ¾÷µ¥ÀÌÆ® ÇÏ¸é ¾ÈµÉ µí?
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
//	// Ä³¸¯ÅÍ Àåºñ Á¶ÇÕ °ü·Ã ÀÌ·± Àú·± Å×½ºÆ®¿¡ ¿ä±äÇÏ°Ô »ç¿ëÇÏ±â À§ÇØ.. 
//	// »ó´Ü UpdateLobbyCharCompForParts ¿Í °ÅÀÇ ºñ½ÁÇÏÁö¸¸ °ü¸®¸¦ ºý¼¼°Ô ÇÒ ÇÊ¿ä´Â ¾ø°Ú´Ù.
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

void FLobbySceneManager::AddSummonItemDisplayMatinee(bool bIsMultiple,bool bIsOpen, bool bIsFriendShip, class ALevelSequenceActor * Matinee)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::AddSummonItemDisplayMatinee"));

	//if (Matinee)
	//{
	//	SummonItemDisplayMapKey Key(bIsMultiple, bIsOpen, bIsFriendShip);
	//	SummonItemMatinees.Emplace(Key, TWeakObjectPtr<ALevelSequenceActor >(Matinee));

	//	Matinee->SetPosition(0.f);
	//	SetMatineeHiddenInGame(Matinee, true);
	//}
}

//void FLobbySceneManager::AddSummonItemDisplayMatineeResult(class ALevelSequenceActor * Matinee)
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
//		check(AllPLForObserveView.Num() <= MobileNumDynamicPointLights); // ·»´õ·¯ ÇÑµµ¸¦ ³Ñ±â´Â ÀÏ ¾øµµ·Ï.
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
//		check(AllPLForAllCharView.Num() <= MobileNumDynamicPointLights); // ·»´õ·¯ ÇÑµµ¸¦ ³Ñ±â´Â ÀÏ ¾øµµ·Ï.
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

	//	//¸¶Æ¼´Ï ÀÌº¥Æ®·Î Ã³¸®ÇÏ´Â °Íµµ ±¦ÂúÀ» µí.
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


//void FLobbySceneManager::OnFinishedSummonItemDisplayMatinee(ALevelSequenceActor * Matinee)
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
//void FLobbySceneManager::OnChangedToFxSummonItemDisplayMatinee(ALevelSequenceActor * Matinee)
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

	//			// RUN ³¡³ª´Â ½Ã°£
	//			BladeIIGameImpl::GetClientDataStore().GetDuelModeSettleSchedule(
	//				b2network::B2ContentsMode::PVP_DUEL,
	//				b2network::B2ContentsModeState::RUN,
	//				nStartDay,
	//				nStartHour,
	//				nStartMin);

	//			// SETTLE_END ³¡³ª´Â ½Ã°£
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

//void FLobbySceneManager::OnFinishedHallOfFameDisplayMatinee(class ALevelSequenceActor * Matinee)
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

//ALevelSequenceActor * FLobbySceneManager::GetDefaultCameraMatinee()
//{
//	return DefaultCameraMatinee;
//}

void FLobbySceneManager::SetDefaultCameraMatinee(class ALevelSequenceActor * DefaultMatinee)
{
	DefaultCameraMatinee = DefaultMatinee;
}

//ALevelSequenceActor * FLobbySceneManager::GetChapterMatinee()
//{
//	return ChapterMatinee;
//}

void FLobbySceneManager::SetChapterViewMatinee(class ALevelSequenceActor * InChapterMatinee)
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
	//			if (LobbyMainUIPage->GetCombineMenuIsVisible()) // ÅëÇÕ¸Þ´º°¡ ¶°ÀÖ´Â »óÅÂ¿¡¼­´Â »ç¿ë¾ÈµÇ°Ô²û
	//				return;
	//		}
	//		UB2UIMsgPopupSelectFairyGift* pMsgPopupSelectFairyGift = UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupSelectFairyGift>(UIFName::MsgPopupSelectFairyGift);
	//		if (pMsgPopupSelectFairyGift)
	//		{
	//			// Æä¾î¸® º¸»ó ÆË¾÷ÀÌ ¶°ÀÖ´Â »óÅÂ¿¡¼­µµ »ç¿ë ¾ÈµÇ°Ô²û
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
//				if (LobbyMainUIPage->GetCombineMenuIsVisible()) // ÅëÇÕ¸Þ´º°¡ ¶°ÀÖ´Â »óÅÂ¿¡¼­´Â »ç¿ë¾ÈµÇ°Ô²û
//					return;
//			}
//			UB2UIMsgPopupSelectFairyGift* pMsgPopupSelectFairyGift = UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupSelectFairyGift>(UIFName::MsgPopupSelectFairyGift);
//			if (pMsgPopupSelectFairyGift)
//			{
//				// Æä¾î¸® º¸»ó ÆË¾÷ÀÌ ¶°ÀÖ´Â »óÅÂ¿¡¼­µµ »ç¿ë ¾ÈµÇ°Ô²û
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
	{ // ÀÌ°Ç HeroManagement ¿¡¼­ HeroManagement ·ÎÀÇ (force) ¼¼ÆÃÀÓ. Å¸ÀÌ¹ÖÀ¸·Î ÀÎÇØ ´Ù¸¥ ¸ðµå·Î ³Ñ¾î°£ »óÅÂ¿¡¼­ ÀÌ°Ô ºÒ¸®¸é ¾ÈµÈ´Ù.
		ChangeLobbyScene(ELobbyScene::ELobbyScene_HeroManagement, -1, true);
	}
}

bool FLobbySceneManager::IsInItemOpDirectingView()
{ // È®ÀÎ Â÷¿ø¿¡¼­ ÇÊ¿äÇÑ ±â´É
	//if (ItemOpSceneCamera && ItemOpSceneCamera->IsValidObj() && ItemOpSceneCamera->GetCameraComponent())
	//{
	//	APlayerController* LocalController = UGameplayStatics::GetLocalPlayerController(ItemOpSceneCamera);
	//	return (LocalController && LocalController->GetViewTarget() == ItemOpSceneCamera);
	//}
	return false;
}

void FLobbySceneManager::OnLobbyGMActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject)
{
	// CurrentLobbyScene ¿¡¸¸ ActorCustomEvent ¸¦ Àü´Þ.
	uint8 CurLobbySceneIndex = static_cast<uint8>(CurrentLobbyScene);
	FLobbySceneBase* CurrentActiveLobbySceneRef = LobbyScenes.IsValidIndex(CurLobbySceneIndex) ? LobbyScenes[CurLobbySceneIndex] : NULL;

	if (CurrentActiveLobbySceneRef){
		CurrentActiveLobbySceneRef->OnLobbyGMActorCustomEvent(OptionalEventName, OptionalNotifyingObject);
	}
}

void FLobbySceneManager::ResetObjectRefsOnSceneLevelStreaming()
{
	// ÀÌ°Ô UPROPERTY ·Î ¹­ÀÎ °Ô ¾Æ´Ï¶ó¼­ TWeakObjectPtr ÀÌ ¾Æ´Ñ ÀÌ»ó GC ¶óµµ µÇ¸é ¾²·¹±â Æ÷ÀÎÅÍ°¡ µÉ °ÍÀÌ´Ù.
	// ±Ùµ¥ °¢°¢ÀÇ ±¸ÇöÀÌ ¾î¶² ½ÄÀÎÁö ¸ô¶ó¼­ ´Ù ¸®¼ÂÇØµµ µÇ´ÂÁö´Â ¸ð¸£°ÚÀ½.
	// LobbySceneManager °¡ LobbyGameMode Á¦°ÅµÉ ¶© °°ÀÌ ³¯¾Æ°¡±â ¶§¹®¿¡ Å©°Ô ¹®Á¦ÀÇ ¼ÒÁö°¡ ÀÖ´Â ¼öÁØÀº ¾Æ´Ï°í..
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
	// ÀüÃ¼ LobbyScene (LobbyGameMode) °ú ¶óÀÌÇÁ »çÀÌÅ¬À» °°ÀÌ ÇÒ event subscribe ¸¦ overriding À» ÅëÇØ Á¤ÀÇ
	// ½ÇÁ¦·Î ¿©±â¸¦ ÅëÇØ subscribe ÇÏ´Â °Ô ¹Ù¶÷Á÷ÇÑ ¸ð½ÀÀº ¾Æ´ÏÁö¸¸ ¿À·¨µ¿¾È ±×·± ±¸Á¶·Î ¸¸µé¾îÁ® ¿Ô¾î¼­ ¸í½ÃÀûÀ¸·Î Á¦°øÇÔ.
}
void FLobbySceneBase::UnsubscribeEvents_OnDestruct()
{
	// SubscribeEvents_OnConstruct ¿¡¼­ subscribe ÇÑ °ÍÀº ÀÌ°É override ÇØ¼­ unsubscribe ÇÑ´Ù.
}

void FLobbySceneBase::SubscribeEvents_OnOpen()
{
	// Æ¯Á¤ LobbyScene ÀÌ È°¼ºÈ­µÈ µ¿¾È¿¡¸¸ Àû¿ëµÉ event subscribe ¸¦ overriding À» ÅëÇØ Á¤ÀÇ
}
void FLobbySceneBase::UnsubscribeEvents_OnClose()
{
	// SubscribeEvents_OnOpen ¿¡¼­ subscribe ÇÑ °ÍÀº ÀÌ°É override ÇØ¼­ unsubscribe ÇÑ´Ù.
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

//ALevelSequenceActor * FLobbySceneBase::GetDefaultCameraMatinee()
//{
//	check(SceneManager);
//	return SceneManager->GetDefaultCameraMatinee();
//}
//
//ALevelSequenceActor * FLobbySceneBase::GetChapterMatinee()
//{
//	check(SceneManager);
//	return SceneManager->GetChapterMatinee();
//}

//void FLobbySceneBase::SetCameraMatineeCam(class ALevelSequenceActor * MatineeActor, float KeyTime)
//{
//	// MatineeActor ¿¡ ¹èÄ¡ÇÑ ¸¶Æ¼´Ï Æ®·¢ ±×·ì¿¡¼­ Á¦ÀÏ ¸ÕÀú Ã£´Â Ä«¸Þ¶ó Æ®·¢ ±×·ìÀÇ KeyTime À» ÇöÀç Ä«¸Þ¶ó·Î ¼³Á¤.
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
	//{ // Á¤»óÀûÀÎ »óÈ²ÀÌ¸é »óÁ¡»Ì±â Á÷ÈÄ »óÁ¡ µ¹¾Æ¿Í¼­ ¸®¼ÂµÉ °ÍÀÎµ¥ È¤½Ã ¸ð¸£´Â »óÈ²ÀÌ ÀÖÀ»Áö ¸ð¸£´Ï È®ÀÎ»ç»ì Â÷¿ø. 
	//	// ¿ä°Ô Àû¾îµµ LobbyMain ¾ÀÀ» ³Ñ³ªµé¸ç À¯ÁöµÉ ÀÌÀ¯´Â ¾øÀ» °Í.
	//	LobbySM->ResetShopGirlReservedState();
	//}

	//FLobbySceneBase::OpenScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (!DefaultCameraMatinee) // ¹ö±× »óÈ²ÀÌ ÀÖ¾î¼­ BeginPlay Àü¿¡ ÀÌ°Ô ºÒ¸± ¶§°¡ ÀÖÀ½. °­Á¦ Ä³½Ì
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
	//		// ¿©±ä ¾Ö´Ï¸ÞÀÌ¼ÇÀ» ¸¶Æ¼´Ï ÇÃ·¹ÀÌ·Î ±¸µ¿½ÃÅ³ °ÍÀÌ¹Ç·Î Àá½Ã ¹Ù²ãµÒ..
	//		charinfo.Value.LevelActor->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//	}
	//}

	//ElapsedTimeNoInput = ElapsedOpenScene = 0.0f;
	//bVisibleUI = true;
	//
	//// UB2UILobbyMain À» ¼û±â´Â ¼³Á¤À» UB2UILobbyMain ¿¡¼­ °¡Á®¿Í¼­ Á¤ÀÛ Å¸ÀÌ¸Ó´Â ¿©±â¼­ µ¹¸®´Â ½ÄÀÎµ¥ ±âÁ¸ ±¸Çö¿¡ ¸ÂÃç ¼³Á¤Àº ³ªÁß¿¡ ³Ö´Ùº¸´Ï ±×·¸°Ô µÈ °Å.
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
	//{ // ¹Ù·Î ¾Æ·¡¿¡ ObserverScene À¸·Î ÀüÈ¯ÇÏ±â Àü¿¡ MainScene ¿¡¼­ÀÇ ¶óÀÌÆÃ ¼Â¾÷À» ÇØ¾ß ÇÑ´Ù.
	//	// ÀÌ°Ô ±× ´ÙÀ½¿¡ ½ÇÇàµÇ¸é ¾Æ·¡¿¡¼­ ObserverScene À¸·Î ÀüÈ¯ÇÏ¸é¼­ ±×ÂÊ ¶óÀÌÆÃ ¼Â¾÷ÀÌ µÈ °É µ¤¾î¾²°Ô µÊ.
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
	//if (ElapsedTimeNoInput >= PlayRateRestoreTime) // ÀÏÁ¤ ½Ã°£ ÀÎÇ²ÀÌ ¾øÀ» ½Ã ¸¶Æ¼´Ï Àç»ý ½Ã°£À» ¿ø·¡´ë·Î µ¹·Á ³õ´Â´Ù
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
	//if (ElapsedTimeNoInput >= AutoHideInterval) // ÀÏÁ¤ ½Ã°£ ÀÎÇ²ÀÌ ¾øÀ» ½Ã ¸ÞÀÎ UI ¸¦ žA ¿·À¸·Î ¼û±ä´Ù.
	//{
	//	SetHiddenLobbyMainUI(true);
	//}
	//*/

	////µÚ·Î°¡±â ¹öÆ°ÀÌ Push event·Î ¹Ù²î¸é¼­ ´Ù¸¥ Scene¿¡¼­ µ¹¾Æ¿Ã ¶§, Release event°¡ ¸ÔÇô Ä³¸¯ÅÍ°¡ ¼±ÅÃµÇ¾î ¹ö¸®´Â ¹ö±× »çÇ×ÀÌ ³ª¿Ã ¼ö ÀÖ´Ù. 
	////ÀÌ ½Å¿¡¼­ Press event¸¦ Ä³Ä¡ÇÑ ÈÄ, Release event°¡ ºÒ¸±¶§¸¸ ½ÇÇàÇÏ°Ô ÇØ¾ßÇÏ´Âµ¥, ½Ã°£»ó ±¸ÇöÀÌ Èûµé¾î ¿ì¼± ÀÌ·¸°Ô ¸·¾Æ³õÀ½ - 20160728
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

	//// Æä¾î¸® ¾Èº¸ÀÌµµ·Ï.
	//FLobbySceneManager* LobbySM = GetSceneManager();
	//if (!bVisibleUI)
	//{
	//	LobbySM->OnLobbyFlyFairy_AllHidden();
	//}
}

void FLobbyMainScene::SwipeScene(float Delta)
{
	//ALevelSequenceActor * DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee && DefaultCameraMatinee->IsValidObj()) // ·¹º§ ½ºÆ®¸®¹Ö°ú ¾ôÇô¼­ Invalid ÇÑ °æ¿ì°¡ ÀÖ´Â °Å °°´Ù..
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

	//// ÇÏÀ§ Å¬·¡½º¿¡¼­ UpdateUIPosition À» È£ÃâÇÏµµ·Ï..
}

float FLobbyMainScene::GetDefaultLobbyMatineeTimeStamp()
{
	float EventKeyTime = 0.0f;
	//ALevelSequenceActor * DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee && DefaultCameraMatinee->IsValidObj()) // ·¹º§ ½ºÆ®¸®¹Ö°ú ¾ôÇô¼­ Invalid ÇÑ °æ¿ì°¡ ÀÖ´Â °Å °°´Ù..
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

	//// Ã¹ ·Îºñ ÁøÀÔ¶§´Â ¿ÜºÎ¿¡¼­ Ã³¸®
	//if (!GetSceneManager()->IsPlayingBeginAnim()) {
	//	PlaySelectMatinee();
	//}
	//else{
	//	// -> ·Îºñ ÁøÀÔ ÈÄ Ãâ¼® Ã¢ ¶ã¶§±îÁö Áö¿¬ÀÌ ½ÉÇÒ ¶§ º¸ÀÌ´Â Àå¸éÀÌ ³Ñ »¹ÂâÇØ¼­ ÀÏ´Ü ¹¹¶óµµ º¸¿©ÁÜ.
	//	PlaySelectMatinee(
	//		false, // ´ë·« Begin ½ÃÁ¡ÀÌ±ä ÇÑµ¥ ¿©±â¼­ ¿øÇÏ´Â Begin Àº Ãâ¼®Ã¢ ´ÝÀº Á÷ÈÄ¶ó false ·Î ³Ñ°ÜÁÖ¾î¾ß
	//		true // Closeup °úÁ¤À» º¸¿©ÁÖ°Ô µÇ¸é ±×°Ô Ãâ¼®Ã¢ ´Ý°í ³ª¼­ ÀçÅÁµÇ´Â ´À³¦ÀÌ µÉ ¼ö ÀÖÀ¸´Ï true ·Î ³Ñ°ÜÁÖ¾î¼­ ¹Ù·Î closeup ºä Á÷Çà
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
	//	// CharacterObserve UI ¸¦ ¿­±â Àü¿¡ ÀÌ°Ô ¼¼ÆÃÀÌ µÇ¾î ÀÖ¾î¾ß ÇÔ.
	//	// ±âÁ¸ÀÇ ¼­ºê Å¬·¡½º¿Í ¼³Á¤ÇÏ·Á´Â ¸ÞÀÎ Å¬·¡½º°¡ °°´Ù¸é ¸ÞÀÎ->¼­ºê
	//	if (DocBattle->GetTagPCClass() == PCClassToInt(CurrentPCClass))
	//	{
	//		DocBattle->SetTagPCClass(DocBattle->GetSavePCClass());
	//	}

	//	DocBattle->SetCurPCClass(PCClassToInt(CurrentPCClass));
	//	DocSome->SetIsCharacterObserveScene(true);
	//	
	//	// 180104_YJ HeroMgmt¸¦ ÀÔÀåÇÒ¶§ ±âº»°ªÀ¸·Î º¸¿©Áö´Â Ä³¸¯ÅÍ´Â ·Îºñ ¿ÉÀú¹ö Ä³¸¯ÅÍÀÌ¹Ç·Î, 
	//	// ¿ÉÀú¹ö Ä³¸¯ÅÍ°¡ º¯°æµÉ¶§ ÇÔ²² º¯°æ½ÃÅ´
	//	if (CahsedSceneManager)
	//		CahsedSceneManager->SetHeroMgmtModePCClass(CurrentPCClass);
	//}
	//
	//// ¸ðµÎ Á¦°ÅÇÏ±â·Î ÇÔ.
	//DestroyAllLobbyCharOnHeadDisplayClass<>::GetInstance().Signal();
	//// ÇöÀç Å¬·ÎÁî¾÷ÇÑ Ä³¸¯ÅÍ Á¤º¸¸¸ ¶ç¿ì´Â °Ô ´Ù½Ã ÇÊ¿äÇØÁö¸é FLobbySceneManager::UpdateLobbyOnHeadDisplayWidget À» »ç¿ë. UB2FloatingLobbyOnHeadDisplay ÂÊ¿¡¼­µµ ¼û±ä °Å º¹±¸


	////ºí·çÇÁ¸°Æ®¿¡¼­ ¼¼ÆÃÇÏ´Â °ÍÀ¸·Î º¯°æ¿¹Á¤ - LVCH_Lobby Ã¼Å©¾Æ¿ô¹®Á¦·Î ÇÏµåÄÚµù..
	//if (LobbyGameMode)
	//{
	//	//180220_YJ °è¼Ó ¹è¿­ÀÌ ½×ÀÌ´ø Çö»óÀÌ ÀÖ¾úÀ½.
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

	//	// Æä¾î¸® º¸ÀÌµµ·Ï.
	//	EFairyType ViewFairyType = FindFlyFairyTypeByOpenStage();
	//	if(ViewFairyType != EFairyType::FAIRY_NONE || ViewFairyType != EFairyType::FAIRY_MAX)
	//		LobbySM->OnLobbyFlyFairy_Visible(ViewFairyType);
	//}
}

EFairyType FLobbyCharObserveScene::FindFlyFairyTypeByOpenStage()
{
	// ÇöÀç ¿ÀÇÂµÇ¾îÀÖ´Â Æä¾î¸® Áß¿¡ ·£´ýÀ¸·Î ÇÏ³ª ¼±ÅÃ.
	EFairyType ViewFairyType = EFairyType::FAIRY_NONE; // NONE ÀÌ¶ó¸é ¾Æ¹«°Íµµ Æä¾î¸®°¡ ¿ÀÇÂµÇ¾îÀÖÁö ¾ÊÀ½.

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
	//// ¸®¼Ò½º°¡ Ä³½ÌµÇÁö ¾ÊÀº °æ¿ì¿¡ ´ëÇØ °­Á¦·Î ¸®Ä³½Ì ½Ã±×³ÎÀ» ³¯¸².. °¡²û ¹®Á¦°¡ º¸¿©¼­..
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
	//	// CharacterObserve UI ¸¦ ¿­±â Àü¿¡ ÀÌ°Ô ¼¼ÆÃÀÌ µÇ¾î ÀÖ¾î¾ß ÇÔ.
	//	DocBattle->SetCurPCClass(PCClassToInt(CurrentPCClass));
	//	DocSome->SetIsCharacterObserveScene(true);

	//	// 180104_YJ HeroMgmt¸¦ ÀÔÀåÇÒ¶§ ±âº»°ªÀ¸·Î º¸¿©Áö´Â Ä³¸¯ÅÍ´Â ·Îºñ ¿ÉÀú¹ö Ä³¸¯ÅÍÀÌ¹Ç·Î, 
	//	// ¿ÉÀú¹ö Ä³¸¯ÅÍ°¡ º¯°æµÉ¶§ ÇÔ²² º¯°æ½ÃÅ´
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
	//				{// ÀÌ ³ðÀº ¹Ù´ÚÀÌ´Ù.
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
	//				if (DesireCamDist - DefaultCamDist <= 0.f) // Defaultº¸´Ù ¾ÈÂÊ¿¡ Camera°¡ À§Ä¡(ÁÜÀÎ»óÅÂ)¶ó¸é
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
	//SetScreenState(false); //±âº» °¡·Î½ºÅ©¸°

	//CachedViewTargetActor->SetCloseUpState(true, 
	//	!bPromptToClosedUpView, // CloseUpView ·Î ¹Ù·Î Á÷ÇàÇÏ´Â °æ¿ì¶ó¸é ¼±ÅÃ Anim ÇÃ·¹ÀÌ¸¦ ÇÏÁö ¾Ê´Â´Ù.
	//	IsBegin); // °ÔÀÓ Á¢¼ÓÇÏ°í ¸Ç Ã³À½.. ÀÌ°É µÎ ¹ø Å¸°Ô µÇ´Ï °­Á¦·Î ´Ù½Ã state ¼¼ÆÃÀ» °ÅÄ¡µµ·Ï.

	//BII_CHECK(Scene->GetSceneManager());

	//ALevelSequenceActor * PlayMatinee = IsBegin ? SelectedCharacterInfo.GameBeginingMatinee : SelectedCharacterInfo.SelectingMatinee;
	//PlayCharacterMatinee(PlayMatinee, SelectedCharacterInfo.LevelActor);

	//if (bPromptToClosedUpView)
	//{
	//	// PlayCharacterMatinee ¿¡¼­´Â ½ÃÀÛ À§Ä¡¿¡ ³õ¾ÒÀ» °Í. ¹Ù·Î ³¡ À§Ä¡·Î °¨.
	//	if (PlayMatinee && PlayMatinee->MatineeData) {
	//		PlayMatinee->SetPosition(PlayMatinee->MatineeData->InterpLength);
	//	}
	//}

	//Scene->GetSceneManager()->SetPlayingBeginAnim(false);
}

//void FLobbyCharObserveScene::FLobbyCharacterObserveCamera::PlayCharacterMatinee(ALevelSequenceActor * PlayMatinee,AB2LobbySkeletalMeshActor* LevelActor)
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
	CurrentPCClass = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
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
	//if (GEngine->ActiveMatinee.IsValid() && GEngine->ActiveMatinee.Get()->GetWorld())//¸¶Æ¼³×°¡ ²¿ÀÌ°íÀÖ½À´Ï´Ù... µ¹°íÀÖ´Â ¸¶Æ¼³× ¹«Á¶°Ç ¸ØÃçÁÖ°í µ¹·ÁÁÝ½Ã´Ù.
	//	GEngine->ActiveMatinee.Get()->Stop();

	//SelectCharacterHeroMgmt(CurrentPCClass);
}

void FLobbyHeroMgmtScene::CloseScene()
{
	FLobbySceneBase::CloseScene();

	//auto& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();

	//auto* LobbyCharInfo = AllCharactersOfDesiredType.Find(CurrentPCClass);
	//if (LobbyCharInfo)
	//{
	//	LobbyCharInfo->MatineeForInven->Stop();
	//	if (LobbyCharInfo->MatineeForWingEventScene){
	//		LobbyCharInfo->MatineeForWingEventScene->Stop();
	//	}
	//	LobbyCharInfo->ActorForMain->SetActorRotation(LobbyCharInfo->InitialRotation);
	//}

/*
	if (UB2UIDocSome* DocSome = UB2UIDocHelper::GetDocSome())
	{
		if (!bCurrentlyWingEvolutionScene)
			CurrentPCClass = DocSome->GetLobbyCharObserverClass();
	}*/

	ZoomModule.ClearZoomModule();
}

void FLobbyHeroMgmtScene::SubscribeEvents_OnConstruct()
{
	LobbyHeroMgmtSelectTicket = LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Subscribe([this](EPCClass SelectedClass) { this->SelectCharacterHeroMgmt(SelectedClass); });
	LobbyCharRotateCharacterTicket = LobbyCharRotateCharacterClass<float>::GetInstance().Subscribe([this](float Value) { this->RotateCharacterYaw(Value); });
	LobbyCharEquippedItemTicket = LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Subscribe([this](EPCClass CharClass, EItemEquipPlace EquippedPlace) { this->PlayEquippedAnimation(CharClass, EquippedPlace); });
	BeginWingEvolutionSceneTicket = BeginWingEvolutionSceneClass<EPCClass>::GetInstance().Subscribe([this](EPCClass EvolvedClass) { this->BeginWingEvolutionScene(EvolvedClass); });
	EndWingEvolutionSceneTicket = EndWingEvolutionSceneClass<>::GetInstance().Subscribe([this]() { this->EndWingEvolutionScene(); });
	LobbyCharZoomTicket = LobbyCharZoomClass<const FVector2D&, float>::GetInstance().Subscribe([this](const FVector2D& Pinpoint, float Delta) { this->ZoomModule.Zoom(Pinpoint, Delta); });
}
void FLobbyHeroMgmtScene::UnsubscribeEvents_OnDestruct()
{
	LobbyHeroMgmtSelectClass<EPCClass>::GetInstance().Unsubscribe(LobbyHeroMgmtSelectTicket);
	LobbyCharRotateCharacterClass<float>::GetInstance().Unsubscribe(LobbyCharRotateCharacterTicket);
	LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Unsubscribe(LobbyCharEquippedItemTicket);
	BeginWingEvolutionSceneClass<EPCClass>::GetInstance().Unsubscribe(BeginWingEvolutionSceneTicket);
	EndWingEvolutionSceneClass<>::GetInstance().Unsubscribe(EndWingEvolutionSceneTicket);
	LobbyCharZoomClass<const FVector2D&, float>::GetInstance().Unsubscribe(LobbyCharZoomTicket);
}

void FLobbyHeroMgmtScene::OnLobbyGMActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject)
{
	//ALevelSequenceActor * CastedNotifyingMatinee = Cast<ALevelSequenceActor >(OptionalNotifyingObject);

	//// ³¯°³ÁøÈ­¿¬Ãâ¿¡¼­ MatineeForWingEventScene ÀÇ Stop notification ÀÎ °æ¿ì ¿¬Ãâ Á¾·á Ã³¸®.
	//// MATINEE_DONE_STAY_ACTOR_CUSTOM_EVENT ¸¦ ¹Þ¾Æ¾ß ÇÒ ¼öµµ ÀÖ´Ù..
	//if (bCurrentlyWingEvolutionScene && OptionalEventName == MATINEE_STOP_ACTOR_CUSTOM_EVENT && CastedNotifyingMatinee && CastedNotifyingMatinee->IsValidObj())
	//{
	//	TMap<EPCClass, FLobbyCharacterInfo>& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();
	//	FLobbyCharacterInfo* EvolvedCharInfo = AllCharactersOfDesiredType.Find(WingEvolutionScenePCClass);
	//	if (EvolvedCharInfo && EvolvedCharInfo->MatineeForWingEventScene == CastedNotifyingMatinee)
	//	{
	//		// ¹Ù·Î ³¡³»Áö ¾Ê°í °á°ú UI ¸¦ º¸¿©ÁØ ´ÙÀ½ EndWingEvolutionScene ´Â °á°ú UI ¿¡¼­ È®ÀÎ¹öÆ° ´©¸£¸é ºÒ¸®µµ·Ï.
	//		OnWingEvolutionMatineeEnd();
	//	}
	//}
}

EUIScene FLobbyHeroMgmtScene::GetUIScene()
{
	AB2LobbyGameMode* LobbyGM = GetSceneManager() ? GetSceneManager()->GetLobbyGameMode() : NULL;
	if (LobbyGM)
	{
		return DJLegacyHeroMgmtPageToUIScene(LobbyGM->GetHeroMgmtSubMode());
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
	//		//Ä³¸¯ÅÍÀÇ À§Ä¡¸¦ ¸ÂÃß±â À§ÇÔ.
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
	//	WingEvolutionScenePCClass = InEvolvedPCClass; // À¢¸¸ÇÑ ÁøÈ­ »óÈ²¿¡¼­¶ó¸é CurrentPCClass ¿Í °°¾Æ¾ß ÇÑ´Ù. ´Ü, È¤½Ã¶óµµ ³×Æ®¿÷ µô·¹ÀÌ·Î ÀÎÇØ ÀÀ´äÀÌ ´Ê°í ±× »õ Ä³¸¯ÅÍ°¡ ¹Ù²ð °¡´É¼ºÀ» »ý°¢ÇØ¼­..
	//}
	//else{
	//	WingEvolutionScenePCClass = CurrentPCClass;
	//}

	//if (WingEvolutionScenePCClass != EPCClass::EPC_End)
	//{		
	//	bCurrentlyWingEvolutionScene = true;
	//	
	//	if (GetSceneManager())
	//	{ // »ç¿ëÀÚ°¡ ³¯°³¸¦ ¼û°å´õ¶óµµ ¿©±â¼± º¸¿©ÁÖÀÚ.
	//		GetSceneManager()->SetUnconditionalShowWing(true, true, WingEvolutionScenePCClass);
	//	}

	//	// ÁøÈ­ ¿¬Ãâ Ã³À½¿¡´Â ³¯°³°¡ ¼û°ÜÁ® ÀÖ´Âµ¥ (¾Ö´Ï¸ÞÀÌ¼Ç ÀÚÃ¼°¡ ±×·³) ¼ø°£ÀûÀ¸·Î ³¯°³°¡ º¸ÀÌ´Â °Ô Æ¢¾î¼­.. È¤½Ã ÀÌ·¸°Ô¶óµµ ÇÏ¸é Á¶±ÝÀÌ³ª¸¶ µµ¿òÀÌ µÉ±î ½ÍÀ½
	//	FlushRenderingCommands(); // ±Ùµ¥ ÀÌ°Å¶û »ó°ü¾ø´Â µí..

	//	TMap<EPCClass, FLobbyCharacterInfo>& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();
	//	
	//	for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(AllCharactersOfDesiredType); It; ++It)
	//	{ // ÀÏ´Ü ´Ù ¼û°Ü³õ°í
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
	//				// ¿©±ä ¾Ö´Ï¸ÞÀÌ¼ÇÀ» ¸¶Æ¼´Ï ÇÃ·¹ÀÌ·Î ±¸µ¿½ÃÅ³ °ÍÀÌ¹Ç·Î Àá½Ã ¹Ù²ãµÒ..
	//				EvolvedCharInfo->ActorForMain->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//			}
	//		}

	//		if (EvolvedCharInfo->MatineeForInven){
	//			EvolvedCharInfo->MatineeForInven->Stop(); // ¿µ¿õ°ü¸® ¸ÞÀÎ ¸¶Æ¼´Ï¿Í °ãÄ¡Áö ¾Ê°Ô Á¤Áö½ÃÅ´.
	//		}
	//		if (EvolvedCharInfo->MatineeForWingEventScene){
	//			EvolvedCharInfo->MatineeForWingEventScene->Stop();

	//			EvolvedCharInfo->MatineeForWingEventScene->SetPosition(0.0f);
	//			EvolvedCharInfo->MatineeForWingEventScene->AddCustomNotifyActor(GetLobbyGameMode()); // Matinee Stop Notification À» ¹Þ±â À§ÇØ GameMode ¸¦ µî·ÏÇØ ³ÖÀ½.
	//			EvolvedCharInfo->MatineeForWingEventScene->Play();
	//		}
	//	}

	//	// ¿¬Ãâ ¿ë UI ÆäÀÌÁö·Î. ±×´ÙÁö º¸¿©ÁÖ´Â °Å ¾ø´Â °ÅÀÇ ºó È­¸éÀÏ µí..
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_WingEvolutionProgress);
	//}
}

void FLobbyHeroMgmtScene::OnWingEvolutionMatineeEnd()
{
	//// °á°ú UI º¸¿©ÁÖ°í..
	//DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_WingEvolutionResult);

	//// ¾Æ¸¶µµ ¸¶Æ¼´Ï ¸¶Áö¸· Ä«¸Þ¶ó Àå¸é¿¡¼­ Idle ÇÃ·¹ÀÌ¸¦ ¿ä±¸ÇÒ °Å °°Àºµ¥.. ±×°Ô ¾î·Á¿ì¸é Æò¼Ò ¿µ¿õ°ü¸® È­¸é¿¡¼­ÀÇ Ä«¸Þ¶ó¶óµµ..

	//TMap<EPCClass, FLobbyCharacterInfo>& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();
	//FLobbyCharacterInfo* EvolvedCharInfo = AllCharactersOfDesiredType.Find(WingEvolutionScenePCClass);
	//if (EvolvedCharInfo)
	//{
	//	if (EvolvedCharInfo->MatineeForWingEventScene)
	//	{
	//		// MATINEE_STOP_ACTOR_CUSTOM_EVENT ¸¦ ¹Þ°í °á°ú UI °¡ ³ª¿À´Â »óÈ²ÀÌ¶ó¸é ¿©±â¼­ Matinee Stop À» ÇØ¼­´Â ¾ÈµÇ°í
	//		// MATINEE_DONE_STAY_ACTOR_CUSTOM_EVENT ¸¦ ¹Þ°í °á°ú UI °¡ ³ª¿À´Â »óÈ²ÀÌ¶ó¸é ¿©±â¼­ Matinee Stop À» ÇÑ´Ù.
	//		//EvolvedCharInfo->MatineeForWingEventScene->Stop();
	//		EvolvedCharInfo->MatineeForWingEventScene->SetPosition(0.0f);

	//		// ³¯°³ ¿¬Ãâ ³¡³­ ¹Ù·Î ±× Ä«¸Þ¶ó ½ÃÁ¡À¸·Î..
	//		const float FinalKeyTime = EvolvedCharInfo->MatineeForWingEventScene->MatineeData ? // MatineeData °¡ ¾øÀ¸¸é ¸»ÀÌ ¾ÈµÇÁö¸¸..
	//			EvolvedCharInfo->MatineeForWingEventScene->MatineeData->InterpLength : 0.0f;
	//		EvolvedCharInfo->MatineeForWingEventScene->Play();
	//		SetCameraMatineeCam(EvolvedCharInfo->MatineeForWingEventScene, FinalKeyTime);
	//		EvolvedCharInfo->MatineeForWingEventScene->SetPosition(FinalKeyTime);
	//	}
	//}
	//// Ä«¸Þ¶ó´Â ³¡³­ ½ÃÁ¡¿¡.. ¾Ö´Ï¸ÞÀÌ¼ÇÀº µû·Î Idle
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(AllCharactersOfDesiredType); It; ++It)
	//{
	//	AB2HeroMgntSkeletalMeshActor* ThisSKActor = It.Value().ActorForMain;
	//	if (ThisSKActor && ThisSKActor->GetSkeletalMeshComponent()){
	//		// È¤ Ä³¸¯ÅÍ ¼±ÅÃÀÌ ¾î±ß³µÀ»Áöµµ ¸ð¸£´Ï ¸ðµÎ AnimBP ¸ðµå·Î È®½ÇÇÏ°Ô ¹Ù²ãÁÜ.
	//		ThisSKActor->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//	}
	//}
}

void FLobbyHeroMgmtScene::EndWingEvolutionScene()
{
//	if (GetSceneManager())
//	{ // »ç¿ëÀÚ°¡ ³¯°³¸¦ ¼û°å´Ù¸é ´Ù½Ã ¼û±ä´Ù.
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
//		WingAirport->MarkPreEvolveDataInvalid(); // ÁøÈ­ °á°úÃ¢¿¡¼­ ½á¸ÔÀº ÀÌÈÄ¿¡´Â ÇÊ¿ä¾ø´Ù.
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

	//if (GEngine->ActiveMatinee.IsValid() && GEngine->ActiveMatinee.Get()->GetWorld())//¸¶Æ¼³×°¡ ²¿ÀÌ°íÀÖ½À´Ï´Ù... µ¹°íÀÖ´Â ¸¶Æ¼³× ¹«Á¶°Ç ¸ØÃçÁÖ°í µ¹·ÁÁÝ½Ã´Ù.
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
	//		//¿µ¿õ°ü¸®¿¡¼­ ¾²ÀÌ´Â Actor¶ó¼­ ÃÊ±â À§Ä¡·Î µ¹¾Æ°¡¾ß¸¸ ÇÑ´Ù.
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
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_StageInfo, false, true);// Sub Ä³¸¯ÅÍÀÇ Sound notify ¸¦ ²ö´Ù.
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

	//ALevelSequenceActor * MainViewMatinee = GetChapterMatinee();
	//
	//if (MainViewMatinee && MainViewMatinee->IsValidObj()) // ·¹º§ ½ºÆ®¸®¹Ö°ú ¾ôÇô¼­ Invalid ÇÑ °æ¿ì°¡ ÀÖ´Â °Å °°´Ù..
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

	//ALevelSequenceActor * DefaultCameraMatinee = GetDefaultCameraMatinee();
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

	//ALevelSequenceActor * DefaultCameraMatinee = GetDefaultCameraMatinee();
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
	// ¸¶Áö¸· ÀÔ·Â ÀÌÈÄ ÀÏÁ¤ ½Ã°£ Èå¸¥ ÈÄ ÀÚµ¿ ½ºÅ©·Ñ.. ¾Æ´Ô Free swipe ÀÌ Áö³ª°í ³ª¸é..?
	if (ElapsedLastInput > 2.f && !bAcceleratingSwipe) 
	{
		if (MatineePos <= MatineeAutoScrollLeftEndPosition)
			bAutoMoveDirection = true;
		if (MatineePos >= MatineeAutoScrollRightEndPosition)
			bAutoMoveDirection = false;

		MoveCameraDirect(MatineeLength * DeltaSeconds * (bAutoMoveDirection ? SpeedAutoScrolling : -SpeedAutoScrolling),
			true // ¿©±â¼­ MoveCameraDirect ÄÝ ÇÒ ¶§¿¡´Â FreeSwipe À¸·Î ÁøÇàÇÒ ¿©Áö°¡ ¹ß»ýÇÏÁö ¾Êµµ·Ï ÇÔ.
		);
		// Ãß°¡·Î ÀÌ »óÈ²¿¡¼­´Â history µµ ³²Áö ¾Ê°Ô..
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
	// FreeSwipe À» À§ÇÑ Á¶°Ç
	check(!bMoveCameraDirectCalledThisFrame && bAcceleratingSwipe);
	FLobbySceneManager* SceneMgr = GetSceneManager();
	check(SceneMgr);

	if (bBeginAcceleratingSwipe) // Free swipe À» Ã³À½ ½ÃÀÛÇÒ ¶§¸¸.
	{
		SpeedSwipe = FreeSwipeStartingSpeed;
		bBeginAcceleratingSwipe = false;
		DirectSwipeSpeedHistory.Empty(); // ½ÃÀÛÇÑ ÀÌ»ó history ¸¦ ³¯¸².
	}

	if (MatineePos == 0.f || MatineePos == MatineeLength)
	{ // Free swipe µµÁß ¾ç ³¡¿¡ µµ´ÞÇßÀ» ¶§ÀÎµ¥.. ¼Óµµ¿Í °¡¼Óµµ ¹æÇâÀ» ¹ÝÀü½ÃÅ°´Âµ¥.. ¾à°£Àº °¨¼è¸¦ ÁÖ°í..
		SpeedSwipe *= -0.8f;
		SwipeAcceleration *= -0.8f;
	}

	if (SceneMgr)
	{
		// µî°¡¼Óµµ°¡ µÉ ¼öµµ ÀÖ´Âµ¥ ¼Óµµ ¸¸Å­ÀÇ ÀúÇ×·ÂÀ» ¹Ý¿µÇÏ´Â °¡¼Óµµ¸¦ »êÃâÇØ º»´Ù..
		SwipeAcceleration = -1.0f * SpeedSwipe * SceneMgr->GetWorldMapSwipeDeaccelerationFactor();

		const float PrevSpeedSwipe = SpeedSwipe;
		SpeedSwipe += (SwipeAcceleration * DeltaSeconds); // dv = at

		// °¨¼ÓÀ¸·Î ÀÎÇØ ¼Ó·ÂÀÌ ÁÙ°Å³ª ¼Óµµ°¡ ¹Ý´ë·Î µÇ´Â »óÈ²ÀÌ¸é ±×¸¸ Áß´Ü
		if (FMath::Abs(SpeedSwipe) < SceneMgr->GetWorldMapSpeedTriggeringAcceleratingSwipe() || SpeedSwipe * PrevSpeedSwipe < 0)
		{
			SpeedSwipe = 0.f;
			bAcceleratingSwipe = false;
		}

		// AcceleratingSwipeInitialSpeed ÀÌ°Å ¿ªÇÒÀ» È®½ÇÈ÷ ¸ð¸£°ÚÀ½. ¹º°¡ ÆÇº°ÇÏ´Â ±âÁØÀ¸·Î ¾²±ä ÇØ¼­ ³öµÒ.
		AcceleratingSwipeInitialSpeed = AcceleratingSwipeInitialSpeed - SwipeAcceleration * DeltaSeconds;

		//UE_LOG(LogBladeII, Log, TEXT("ChapterTick bAccel %d, Speed %.4f -> %.4f, Acc %.4f, ElapedLastInput %.2f, MatineePos %.2f"),
		//	(int32)bAcceleratingSwipe, PrevSpeedSwipe, SpeedSwipe, SwipeAcceleration, ElapsedLastInput, MatineePos);
	}
}

// Á¤ÇØÁø ¼Óµµ·Î ¿òÁ÷ÀÏ ¶§ ¾´´Ù. ÀÔ·ÂÀ¸·Î ½ºÅ©·ÑÇÑ´Ù°Å³ª ÀÚµ¿ ½ºÅ©·Ñ ½Ã
void FLobbyBattleSelectSceneBase::MoveCameraDirect(float Value, bool bNoCheckForFreeSwipe)
{
	FLobbySceneManager* SceneMgr = GetSceneManager();
	check(SceneMgr);
	if (SceneMgr)
	{
		// »ç¿ëÀÚ ÀÔ·ÂÀ¸·Î ÀÎÇÑ MoveCameraDirect °¡ ¸¶Áö¸·À¸·Î È£ÃâµÈ ½ÃÁ¡¿¡ Á¤ÇØÁø SpeedSwipe °ªÀ¸·Î °¨¼ÓÀ» ÇÏ¸é¼­ free swipe À» ÇÏ´Â°Çµ¥
		// Æ¯È÷ ¸ð¹ÙÀÏ ÀÔ·ÂÈ¯°æ¿¡¼­ ¸¶Áö¸·¿¡ ÀÛÀº °ªÀÌ ³ª¿À°ï ÇØ¼­ free ½ºÅ©·ÑÀÌ ¿À·¡ ¸ø°¡°Å³ª ¾Æ¿¹ ½ÃÀÛÀ» Á¦´ë·Î ¸øÇÒ ¶§°¡ ÀÖÀ½
		// µû¶ó¼­ ¸¶Áö¸· ¸î ¹øÀÇ Æò±ÕÀ» ³» º»´Ù..
		// ÅÍÄ¡·Î Á÷Á¢ swipe ½Ã¿¡µµ ¼ÒÁ¤ÀÇ Æò±ÕÀ» ³½´Ù.

		DirectSwipeSpeedHistory.Add(Value);
		if (DirectSwipeSpeedHistory.Num() > SceneMgr->GetWorldMapDirectSwipeSpeedHistoryNum())
		{
			DirectSwipeSpeedHistory.RemoveAt(0);
		}
		
		// AcceleratingSwipeInitialSpeed ÀÌ°Å ¿ªÇÒÀ» È®½ÇÈ÷ ¸ð¸£°ÚÀ½. ¹º°¡ ÆÇº°ÇÏ´Â ±âÁØÀ¸·Î ¾²±ä ÇØ¼­ ³öµÒ.
		AcceleratingSwipeInitialSpeed = Value;
		// ±âº»ÀûÀÎ SpeedSwipe Àº ÀÏÁ¤ »ùÇÃÀ» °¡Áö°í Æò±Õ³½ °ªÀ¸·Î
		SpeedSwipe = GetAvgDirectSwipeSpeed(SceneMgr->GetWorldMapDirectSwipeSpeedFilterNum());

		if (DirectSwipeSpeedHistory.Num() > 0)
		{ 			
			float TotalAvgSwipeSpeed = GetAvgDirectSwipeSpeed(); 

			// @.@
			if (!bNoCheckForFreeSwipe) // °£È¤ ÀÇµµ¿Í ´Ù¸¥ »óÈ²¿¡¼­ÀÇ MoveCameraDirect ÄÝÀÌ AcceleratingSwipe À» ¹ß»ý½ÃÅ°´Â µí..
			{
				bAcceleratingSwipe = (
					(SceneMgr->GetWorldMapSwipeDeaccelerationFactor() != 0.f && FMath::Abs(TotalAvgSwipeSpeed) > SceneMgr->GetWorldMapSpeedTriggeringAcceleratingSwipe())
					||
					(FMath::Abs(AcceleratingSwipeInitialSpeed * 0.4f + TotalAvgSwipeSpeed * 0.6f) > SceneMgr->GetWorldMapSpeedTriggeringAcceleratingSwipe())
					);
			}

			if (bAcceleratingSwipe)
			{
				// Free swipe À» ÇÏ°Ô µÉ °Å °°À¸¸é ½ÃÀÛ ¼Óµµ´Â ¼öÁýÇÑ ÀüÃ¼ »ùÇÃ Æò±ÕÀ» °¡Áö°í..
				FreeSwipeStartingSpeed = TotalAvgSwipeSpeed;
				bBeginAcceleratingSwipe = true;

				//UE_LOG(LogBladeII, Log, TEXT("MoveCameraDirect About to begin FreeSwip with Avgspeed %.4f from %d samples"),
				//	FreeSwipeStartingSpeed, DirectSwipeSpeedHistory.Num());

				// ½ÇÁ¦·Î´Â ´ÙÀ½ Æ½¿¡µµ MoveCameraDirect °¡ ºÒ¸®Áö ¾Ê¾Æ¾ß Free swipe À» ÇÏ°Ô µÉ °Í.
			}			
		}		
		bMoveCameraDirectCalledThisFrame = true;
	}

}

float FLobbyBattleSelectSceneBase::GetAvgDirectSwipeSpeed(int32 InMaxHistoryNum, bool bRespectTheLastSign) const
{
	const int32 FinalSampleNum = InMaxHistoryNum < 0 ? DirectSwipeSpeedHistory.Num() : FMath::Min(InMaxHistoryNum, DirectSwipeSpeedHistory.Num());
	if (FinalSampleNum <= 0) { // ¸ðÀÎ »ùÇÃÀÌ ¾ø´Â »óÈ²ÀÏ µí.
		return 0.0f;
	}

	// µÚÂÊÀÏ¼ö·Ï ÃÖ½Å »ùÇÃ·Î °£ÁÖÇÏ°í µÚ¿¡¼­ºÎÅÍ..
	const int32 AvgFirstIndex = DirectSwipeSpeedHistory.Num() - 1;
	const int32 AvgLastIndex = DirectSwipeSpeedHistory.Num() - FinalSampleNum;
	check(AvgLastIndex >= 0);
	float RetAvg = 0.0f;
	for (int32 SI = AvgFirstIndex; SI >= AvgLastIndex; --SI)
	{
		// bRespectTheLastSign ÀÎ °æ¿ì Àý´ë°ªÀ¸·Î Æò±Õ³»¼­ ºÎÈ£´Â ¸¶Áö¸· »ùÇÃ °É·Î´Ù°¡ »ç¿ë.
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
//	ALevelSequenceActor * DefaultCameraMatinee = GetDefaultCameraMatinee();
//	if (DefaultCameraMatinee && DefaultCameraMatinee->bIsPlaying){
//		return;
//	}
//
//	ALevelSequenceActor * MainViewMatinee = GetChapterMatinee();
//	if (MainViewMatinee && MainViewMatinee->IsValidObj() && MainViewMatinee->GetWorld()) // ·¹º§ ½ºÆ®¸®¹Ö°ú ¾ôÇô¼­ Invalid ÇÑ °æ¿ì°¡ ÀÖ´Â °Å °°´Ù..
//	{
//		MatineePos = FMath::Clamp<float>(MatineePos + Delta, 0.f, MatineeLength);
////#if UE_BUILD_DEBUG // ¾Æ¸¶µµ ¸ðÇè¸Ê ÀÚµ¿ ½ºÅ©·ÑÀÇ ¿µÇâÀÎÁö °¡¸¸È÷ ÀÖ¾îµµ ·Î±×°¡ ¸¹ÀÌ ³ª¿Í¼­..
////		UE_LOG(LogStreaming, Warning, TEXT("MatineePos ====== '%f' "), MatineePos);
////#endif
//		MainViewMatinee->Play();
//		MainViewMatinee->SetPosition(MatineePos);
//		MainViewMatinee->Pause();
//
//		SetCameraMatineeCam(MainViewMatinee, MatineePos);
//	}

	// ÇÏÀ§ Å¬·¡½º¿¡¼­ UpdateUIPosition À» È£ÃâÇÏµµ·Ï..
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
		// UMG ¿¡¼­ ¾µ UIPosition °è»ê ½Ã¿¡ ÇöÀç ÇØ»óµµ°¡ ¾Æ´Ñ ±âÁØ ÇØ»óµµ ±âÁØÀ¸·Î ³Ö¾îÁÖ¾î¾ß ÇÏ¹Ç·Î ºä ½ºÄÉÀÏÀÌ ÇÊ¿äÇÔ.
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
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // ·¹º§ ½ºÆ®¸®¹ÖÀÌ »õ·Î µÇÁö ¾Ê´õ¶óµµ ¸Å Ã©ÅÍ¸¶´Ù Matinee ´Â »õ·Î ¼¼ÆÃÇØ¾ß ÇÏ¹Ç·Î °­Á¦ update
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
	//{ // ¿ä ³×ÀÌ¹Ö¿¡ ºÎÇÕÇÏ´Â ¾Öµé¸¸ UI Æ÷Áö¼Ç ¼¼ÆÃ ¸¶Ä¿·Î °£ÁÖ.
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
	//				InterpGroup->GetGroupActor()->SetActorHiddenInGame(true);	//¸Ê¿¡¼­ ±× À§Ä¡ Å¥ºê ¾Èº¸ÀÌ°Ô ÇÏ´Â ±¸°£.

	//				for (int32 idx = 0; idx < InterpGroup->Group->InterpTracks.Num(); ++idx)
	//				{
	//					UInterpTrackMove* MoveTrack = Cast<UInterpTrackMove>(InterpGroup->Group->InterpTracks[idx]);
	//					if (MoveTrack && !MoveTrack->IsDisabled()) // MoveTrack ÀÌ ¾µµ¥¾øÀÌ ¿©·¯°³ µî·ÏµÈ °æ¿ì°¡ ÀÖÀ½. ÇÏ³ª¸¸ °É·¯³»¾ß..
	//					{
	//						CachedUIActorLocation.Add(MoveTrack->EvalPositionAtTime(InterpGroup->TrackInst[idx], 0.f));
	//						break;
	//					}
	//				}
	//			}					
	//		}
	//	}		
	//}

	//// ¿©±â Ã¼Å©¿¡ °É¸®¸é ÀÇµµ¿Í ´Ù¸£°Ô ¼¼ÆÃÇÑ ¸¶Æ¼´Ï Æ®·¢ÀÌ ÀÖ´Ù°Å³ª ÇÑ °æ¿ì. ¾û¶×ÇÏ°Ô ³×ÀÌ¹Ö¿¡ °É·È´Ù°Å³ª..?
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

	// ¾Æ·¡ ÀÌ¸§Àº Matinee ±×·ì ÀÌ¸§À¸·Î »ç¿ëÇÏ°Ô µÉ °Å.
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
	//	LobbyGM->UpdateNativeLobbyMatineeRef(); // Bundle_A ¿Í µ¿ÀÏÇÑ ½ºÆ®¸®¹Ö ·¹º§¿¡ ¼¼ÆÃÀÌ µÇ¾ú´Ù¸é ÀÌ ½ÃÁ¡¿¡ ÀûÀýÇÑ °ªÀ» µé°í ÀÖÁö ¾ÊÀ» °ÍÀÌ´Ï ÇÑ¹ø ´õ ¾÷µ¥ÀÌÆ®.
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

	UpdateUIPosition(); // UI Position ÀÌ »ìÂ¦ ²¿ÀÏ ¶§°¡ ÀÖ¾î¼­ °á±¹ tick ¿¡¼­ ¸Å¹ø Ã³¸®.. ¤Ñ.¤Ñ
}

void FLobbyModSelectScene::InitializeUIPositionMarkers()
{
	//ALevelSequenceActor * MainViewMatinee = GetChapterMatinee();
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
		//	{ // ÀÌ¹ø Elem ÀÇ Á¤ÇØÁø ±×·ì ÀÌ¸§°ú ÀÏÄ¡ÇÏ´Â ±×·ì.
		//		if (ThisGroupInst->GetGroupActor())
		//		{
		//			UIPositionActors[MEI] = ThisGroupInst->GetGroupActor();
		//			ThisGroupInst->GetGroupActor()->SetActorHiddenInGame(true); // UI À§Ä¡ ¸¶Å· ¿ªÇÒÀÏ »Ó. ¼û°Ü³õÀ½.

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
//		// 2D À§Ä¡µé ¾ò¾î¿Í¼­ ÃÖÁ¾ÀûÀ¸·Î UIMod ÂÊÀ¸·Î ³Ö¾îÁÜ.
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

	////ÀÓ½Ã Todo : Doc delegate·Î »©¾ßµÊ
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
	////¿ì¼± DocBattle¿¡ ´ã´Âµ¥, µ¥ÀÌÅÍ°¡ ¸¹¾ÆÁö¸é Doc¸¦ »õ·Î ¸¸µå´Â °ÍÀ¸·Î..
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
	//	//todo UI¿­±â
	//	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::Mail);

	//	//todo Categoryº°
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

	//ALevelSequenceActor * DefaultCameraMatinee = GetDefaultCameraMatinee();
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
//	//ÀÚ Á¤·ÄÇÕ½Ã´Ù ³ôÀº¼ø¼­ºÎÅÍ
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
//		// ·¹ÀÌµå ÃÊ´ë½Ã¿¡ ±æµå¿ø ¹Þ´Â°Í ¶§¹®¿¡.. ·¹ÀÌµå´Â ºÐ¸®ÇØµÎ¾ú´Âµ¥,
//		// ´Ù¸¥ ÆÐ³ÎÂÊ¿¡µµ GuildDetailInfo À» »ç¿ëÇÑ´Ù¸é ¼­¹öÂÊ¿¡ ÇÁ·ÎÅäÄÝ ºÐ¸® ¿äÃ» È¤Àº ¿¹¿ÜÃ³¸®¸¦ ½ÅÁßÈ÷ ÇØ¾ßÇÒµí..
//	}
//	else if (UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting))
//	{
//		// Ã¤ÆÃ UI°¡ ³ëÃâ ÁßÀÏ°æ¿ì ¹«½Ã.
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

	//if(GuildDoc->GetGuildOutState())			//±æµå¿¡¼­ Ãß¹æ´çÇßÀ»°æ¿ì ÆË¾÷À» ¶ç¿öÁÖÀÚ~~
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

	//// UI Init() ÀÌ ºÒ¸° ÈÄ¿¡ OpenSceneÀÌ µÇ¹Ç·Î Ã³À½ ÁøÀÔ ÇÑ¹øÀº ·ÎºñÄ³¸¯ÅÍ ¼ÂÆÃÀ» ÇØ ÁÖ¾î¾ß ÇÔ
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
	//// ÀÌÀü Character´Â ¼û±èÃ³¸® ÈÄ ½ÃÀÛ
	//auto& LobbyCharactersForGLobbyType = GetLobbyCharactersForGTypeLevel();
	//auto* LobbyCharInfo = LobbyCharactersForGLobbyType.Find(CurrentSelectedCharacter);
	//if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
	//{
	//	CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, true);
	//}

	//// »õ·Î¿î Character Visible Ã³¸® ½ÃÀÛ
	//LobbyCharInfo = LobbyCharactersForGLobbyType.Find(SelectedCharacter);
	//if (LobbyCharInfo)
	//{
	//	CurrentSelectedCharacter = SelectedCharacter;
	//	CurrentCharacter = LobbyCharInfo->ActorForMain;

	//	ALevelSequenceActor * CollectBookMatinee = nullptr;
	//	float EventKeyTime = 0.f; //µµ°¨¿ë Matinee EventKeyTime µû·Î ¾øÀ½
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
	//{ // °¡²û¾¿.. ¾îµð¼±°¡ ½ÅÈ£ º¸³½ °Ô µÚ´Ê°Ô ¿À±â¶óµµ ÇÏ´ÂÁö ½Ã°£ Áö³ª°í ³ª¼­ Ä«¸Þ¶ó°¡ È× ¹Ù²î´Â °æ¿ì°¡ ÀÖ¾î¼­ Á» »¹Áþ.
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
	//{ // LobbySceneManager ÀÇ ShopGirlReservedState À» Àì½Î°Ô Ä³½ÌÇØ ³õ°í ¸®¼Â
	//	CachedShopGirlReservedState = LobbySM->GetShopGirlReservedState();

	//	// OpenScene À» ÅëÇØ µé¾î¿Ã ¶§ Cheer °¡ ¾Æ´Ï¸é Ã³À½¿¡´Â È¯¿µ ¾Ö´Ï¸ÞÀÌ¼ÇÀÌ µÇ¾î¾ß ÇÔ.
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
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // ¾ÈÀü»§.

	//	// »óÁ¡ ¼Ò³àµµ ÅÍÄ¡ ¾×¼ÇÀÌ ÀÖ¾î¼­ ÀÔ·Â ¹ÙÀÎµù
	//	LobbyGameMode->BindTouchInput(true);
	//	LobbyGameMode->SetEnableClickEvents(true);
	//}

	//ALevelSequenceActor * DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee)
	//{
	//	CachedItemSceneMatinee = DefaultCameraMatinee;

	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}
	//// ¹èÄ¡ÇØ ³õÀº »óÁ¡ ¼Ò³à ¾×ÅÍ
	//AB2LobbyShopGirl* ShopGirActor = LobbySM ? Cast<AB2LobbyShopGirl>(LobbySM->GetNPCSKActor(ELobbyNPCType::ELobbyNPC_ShopGirl)) : nullptr;
	//if (ShopGirActor && CachedShopGirlReservedState != ELobbyShopGirlState::ELSG_End)
	//{	
	//	if (CachedShopGirlReservedState == ELobbyShopGirlState::ELSG_IdleLoop)
	//	{ // »Ì±â °á°ú¿¡ ´ëÇÑ ¾¤°¡ ¾Æ´Ñ ´ÙÀ½¿¡¾ß Welcoming ÀÌ µÉ Å×´Ï ¿©±â·Î µé¾î¿Ã ÀÏÀÌ ÀÖÁö´Â ¾Ê°Ú´Ù.
	//		ShopGirActor->BeginDefaultPose(); // Idle ÀÌ¸é PlayTemporary °¡ ¾Æ´Ï°í ·çÇÎÀ¸·Î °è¼Ó µ¹·Á¾ß µÇ´Â °Å.
	//	}
	//	else
	//	{ // ¼îÅ¸ÀÓ
	//		ShopGirActor->PlayTemporaryAnimOfState(CachedShopGirlReservedState);
	//	}
	//}

	//if (LobbySM && !LobbySM->IsSceneStreamingLevelLoaded(ELobbyScene::ELobbyScene_SummonItemDisplay))
	//{ // »Ì±â Àå¸éÀ» ¹Ì¸® ·ÎµùÇØ µÎ´Âµ¥ flush ·Î ÀÎÇÑ ºÎÀÛ¿ë °¡´É¼ºÀ» ³·Ãß±â À§ÇØ AnimStateGuard ¸¦ µÐ´Ù. 
	//	// ÀÌ°É ¾È ÇÏ¸é Æ¯È÷³ª ÅÍÄ¡ ½Ã ´«¿¡ ¶ç´Â ÇÁ¸®Â¡ÀÌ ¹ß»ýÇÒ ¼ö ÀÖ´Ù.
	//	// ±×¸®°í Ã³À½ ÀÏÁ¤ ½Ã°£Àº Welcoming ¸ð¼ÇÀÌ È®½ÇÇÏ°Ô º¸ÀÌµµ·Ï ÇÏ´Â ÀÇ¹Ìµµ µÉ ¼ö ÀÖ°í.
	//	ShopGirActor->SetAnimStateGuard(1.5f);
	//	LobbySM->PrefetchSceneStreamingLevel(ELobbyScene::ELobbyScene_SummonItemDisplay);
	//}
}
void FLobbySummonItemScene::CloseScene()
{
	//CachedItemSceneMatinee = nullptr;

	//// »óÁ¡ ¼Ò³àµµ ÅÍÄ¡ ¾×¼ÇÀÌ ÀÖ¾î¼­ ÀÔ·Â ¹ÙÀÎµù
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
	////Ä«µå µÚÁýÀ»¶§´Â ¾²´Â ÇÔ¼ö
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
//void FLobbyGachaDisplaySceneBase::OnFinishedSummonItemDisplayMatinee(ALevelSequenceActor * Matinee)
//{
//	UB2UISummonItemDisplay* DisplayUI = UB2UIManager::GetInstance()->GetUI<UB2UISummonItemDisplay>(UIFName::SummonItemResult);
//	if (DisplayUI)
//	{
//		DisplayUI->UpdateLocationSummonItemUIs();
//	}
//}
//
//void FLobbyGachaDisplaySceneBase::OnChangedToFxSummonItemDisplayMatinee(ALevelSequenceActor * Matinee)
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
	//if (GEngine->ActiveMatinee.IsValid() && GEngine->ActiveMatinee.Get()->GetWorld())	//¸¶Æ¼³×°¡ ²¿ÀÌ°íÀÖ½À´Ï´Ù... µ¹°íÀÖ´Â ¸¶Æ¼³× ¹«Á¶°Ç ¸ØÃçÁÖ°í µ¹·ÁÁÝ½Ã´Ù.
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
	////ÀÎº¥¿¡ ³Ö±â
	//auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	//FLobbySceneManager* LobbySM = GetSceneManager();
	//if (LobbySM)
	//{
	//	LobbySM->SetShopGirlReservedState(ELobbyShopGirlState::ELSG_CheerNormal); // ¸øÇØµµ ±âº»»§À¸·Î ÇÏ³ª´Â º¸¿©ÁØ´Ù
	//}
	//for (auto& SvrItem : SummonResult->items)
	//{
	//	int32 AddedIndex = CurrentSummonedItems.AddedItems.AddDefaulted();
	//	check(AddedIndex != INDEX_NONE);

	//	FB2Item& CurrAddedRef = CurrentSummonedItems.AddedItems[AddedIndex];
	//	CurrAddedRef = SvrItem;
	//	if (CurrAddedRef.StarGrade >= ITEM_SUMMON_JACKPOT_GRADE_LV1 && LobbySM)
	//	{
	//		LobbySM->SetShopGirlReservedState(ELobbyShopGirlState::ELSG_CheerMuch); // ¼½½Ã´í½º ¿¹¾à.
	//	}
	//}

	//ClientDataStore.AddNewAcquiredItems(CurrentSummonedItems.AddedItems);

	//auto StoreDoc = UB2UIDocHelper::GetDocStore();

	///*UB2UISummonItem* SummonItemUI = UB2UIManager::GetInstance()->GetUI<UB2UISummonItem>(UIFName::SummonItemStore);
	//BII_CHECK(SummonItemUI);*/
	//if (!StoreDoc)
	//	return;

	//// ¿ìÁ¤ Æ÷ÀÎÆ®·Î »Ì±â
	//if (SummonResult->social_point != ClientDataStore.GetSocialPoint())
	//{
	//	ClientDataStore.UseSocialPoint();
	//}

	////¸¶ÀÏ¸®Áö ¹× ÀçÈ­µî Update
	//if (SummonResult->free_lottery) //[@AKI, 171124] SummonResult->free_lottery == trueÀÏ¶§´Â ¼­¹ö¿¡¼­  DBÁ¶È¸¸¦ ÇÒ ÇÊ¿ä°¡ ¾ø±â ¶§¹®¿¡ next_free_timeÁ¦¿ÜÇÏ°í´Â °ª °»½ÅÀ» ¾ÈÇÔ.±×·¡¼­ Å¬¶óµµ ±×¿Í µ¿ÀÏÇÏ°Ô ±¸ÇöÇÔ. ex> gem = 0, social_point = 0
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

	//// ÇÑ¹ø ´õ »Ì±â¸¦ À§ÇÑ °ËÁõµÈ Å°
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

	//ÀÎº¥¿¡ ³Ö±â
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
	//	ALevelSequenceActor * ChangeMatinee;

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



	//	//°á°úÃ¢ PowrIndex ¼øÀ¸·Î Á¤·ÄÇÏÀÚ
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

void FLobbyGuildMapUIScene::SetGuildMapChangeMatinee(class ALevelSequenceActor * Atype, class ALevelSequenceActor * Btype)
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

	//	// 1. ÃÊ´ëÇÏ±â
	//	if (UIDocRaid->GetIsReceiveInvite()) // ÃÊ´ëÇÏ±â·Î ÀÔÀå.
	//	{
	//		JoinType = ERaidJoinType::MakeRoom;
	//		RaidUI->ForcedStartGame(RaidType, RaidStep, JoinType, PcClass, false);
	//	}
	//	// 2. ÀÚµ¿ ¸ÅÄª
	//	else if(IsRematch)
	//	{
	//		RaidUI->ForcedStartGame(RaidType, RaidStep, JoinType, PcClass, true);

	//	}
	//	// 3. ÀÏ¹ÝÀûÀÎ ÀÔÀå.
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

	//	// ÆÀ¿øµé°ú ´Ù½ÃÇÏ±â ´©¸¥°Ô ¾Æ´Ï¸é Raid MatchMaking ¶§ ¹ÞÀº Á¤º¸µéÀº ¹«½ÃÇÑ´Ù.
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
	//	// ·¹ÀÌµå ´Ù½ÃÇÏ±â - ÀÌÀü°ÔÀÓ ChannelEndpoint ³Ñ°ÜÁÖ¸é °°Àº ÆÀ¿øµé°ú ·ë Á¶ÀÎ
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
	//	// ±èÀºÃ¶ : 2018-08-24 ((UPDATE+3) ·¹ÀÌµå °³¼± - È¥ÀÚ ÇÏ±â/´Ù½ÃÇÏ±â) B2RaidRoomType REJOIN Ãß°¡
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
	// ÀÌ¶§´Â UI¸¦ ¼ÂÆÃÇØÁÖÁö ¸»ÀÚ.
}

void FLobbyRaidMainScene::SetRemotePlayerClass(uint32 NetId, EPCClass MainPCClass, const FB2FindAccountInfoPtr& Info)
{
	AddUIData(NetId, MainPCClass, FB2ModPlayerInfo(Info), Info->guild_id);
}

void FLobbyRaidMainScene::LeaveRoomIfJoined()
{
	RemoveSlotIndex();

	// Á¶ÀÎÃ¢ÀÌ ¿­·ÁÀÖÀ»¼öÀÖÀ¸´Ï ´Ý¾ÆÁØ´Ù.
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
	//// RaidLevel º¯°æ
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

	//	// roomLocation ¼³Á¤ ÈÄ
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

	//// Raid UI °»½ÅÀ» À§ÇØ Á¤º¸¸¦ ³Ö¾îÁØ´Ù.
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
	// ±âÁ¸¿¡ ÀûÀçµÇ¾îÀÖ´ÂÁö È®ÀÎ
	for (int i = 0; i < SlotIndexs.Num(); ++i)
	{
		if (SlotIndexs[i] == NetId)
			return i;
	}

	return -1;
}

bool FLobbyRaidMainScene::IsExist(const int32 NetId) const
{
	// ±âÁ¸¿¡ ÀûÀçµÇ¾îÀÖ´ÂÁö È®ÀÎ
	for (int i = 0; i < SlotIndexs.Num(); ++i)
	{
		if (SlotIndexs[i] == NetId)
			return true;
	}

	return false;
}

int32 FLobbyRaidMainScene::GetNextInsertSlotIndex(const int32 NetId)
{
	// ±âÁ¸¿¡ ÀûÀçµÇ¾îÀÖ´ÂÁö È®ÀÎ
	int32 ExistSlotIdx = GetSlotIndex(NetId);
	if (ExistSlotIdx != -1)
		return ExistSlotIdx;

	// ¾ÈµÇ¾îÀÖÀ¸¸é Ã¹¹øÂ°·Î ºñ¾îÀÖ´Â ½½·Ô
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
	{ // Áï¼® ¸Þ½¬ ¾÷µ¥ÀÌÆ®. ¾÷µ¥ÀÌÆ® ÇÏ¸é¼­ bUnconditionalShowWing À» ÂüÁ¶ÇÏ°Ô µÉ °Å.
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
	ShopGirlReservedState = InStateToReserve; // °ÅÀÇ ÀÌ°Ç ¼¼ÆÃ ÈÄ ¿À·¡ Áö³ªÁö ¾Ê¾Æ ¹Ù·Î ½á¸Ô°í ¸®¼ÂµÇ¾î¾ß ÇÒ ¼º°ÝÀÓ.
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
	//		//¿µ¿õ°ü¸®¿¡¼­ ¾²ÀÌ´Â Actor¶ó¼­ ÃÊ±â À§Ä¡·Î µ¹¾Æ°¡¾ß¸¸ ÇÑ´Ù.
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
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_CharacterPage, false, true);// Sub Ä³¸¯ÅÍÀÇ Sound notify ¸¦ ²ö´Ù.
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
	{ // Ä³¸¯ÅÍ ¸Þ½¬ Á¶ÇÕ ½Ã ¸®¸ðÆ® Ä³¸¯ÅÍ Çã¿ë.
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
	//{ // Ä³¸¯ÅÍ ¸Þ½¬ Á¶ÇÕ ½Ã ¸®¸ðÆ® Ä³¸¯ÅÍ Á¤º¸ »ç¿ë ±ÝÁö. ÀÌ°É ¾È ÇÒ ½Ã Àß¸øµÈ Á¤º¸ Á¢±ÙÀ¸·Î Å©·¡½¬ À§ÇèÀÌ ÀÖÀ½.
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
	//		//Ä³¸¯ÅÍÀÇ À§Ä¡¸¦ ¸ÂÃß±â À§ÇÔ.
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
	//		//¿µ¿õ°ü¸®¿¡¼­ ¾²ÀÌ´Â Actor¶ó¼­ ÃÊ±â À§Ä¡·Î µ¹¾Æ°¡¾ß¸¸ ÇÑ´Ù.
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
	// À¯Àú ³ª°¥¶§µµ À¯ÀúÄ«¿îÆÃUI °»½Å
	RefreshFindMatchPopup();
}

void FLobbyControlCharacterSelectPage::SetMyPlayerInfoData(uint32 NetId, const FB2FindAccountInfoPtr& info)
{
	//GetLobbyGameMode()->SetPeerTeam();
}

void FLobbyControlCharacterSelectPage::SetRemotePlayerInfoData(uint32, const FB2FindAccountInfoPtr&)
{
	// È£½ºÆ®. ÆÀÁ¤º¸ °»½Å. ¾Ë¸².
	//GetLobbyGameMode()->SetPeerTeam();
}

void FLobbyControlCharacterSelectPage::SetRemotePlayerClass(uint32, EPCClass, const FB2FindAccountInfoPtr&)
{
	// À¯Àú µé¾î¿À¸é À¯ÀúÄ«¿îÆÃUI °»½Å
	// ÆÀÁ¤º¸ ¹Þ°í °»½Å
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

	//	// Áøµ¿
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
	// ÆÀÁ¤º¸ ¹Þ±¸ °»½Å. È£½ºÆ®µµ ¹Þ±¸°»½Å.
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
	// ³»Á¤º¸(³Ý¾ÆÀÌµð) ¹Þ±¸³ª¼­ µî±ÞÁ¤º¸ º¸³¿
	//UB2UIDocControl* ControlDoc = UB2UIDocHelper::GetDocControl();
	//if (!ControlDoc)
	//	return;

	//GetLobbyGameMode()->NetSendBattleGradeInfo(ControlDoc->GetBattleGrade(), ControlDoc->GetBattleStarCount());
	//GetLobbyGameMode()->NetSendMatchToken();
}

void FLobbyControlCharacterSelectPage::BattleStart()
{
	//// Á¡°Ë Á¤»ê Ã¼Å©
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
	//// ±×³É °°Àº°Å¿¡ ¸ÞÀÎ¸¸ ½áº»´Ù.
	//// ½Ì±Û¿¡ »ç¿ëÇÒ ¾×ÅÍ ¹èÄ¡ÇÏ¸é ¹Ù²ã¾ßÇÔ
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

	//	// Ç®¹æÀ¸·Î º¸¿©ÁÜ
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
	// Å« °Å ÇÏ³ª ¹Ì¸® ¹é±×¶ó¿îµå ·ÎµùÇØ µÒ.
	// ÀÌ Àå¸é¿¡¼­ ¹Ý°ÝÀÌ³ª ·¹ÀÌµå Àå¸éÀ¸·Î ³Ñ¾î°¡¸é »ç¿ëÇÏÁöµµ ¾ÊÀ» ¿µ¿õÀÇ Å¾ ½ºÆ®¸®¹Ö ·¹º§ÀÌ flush µÇ¾î¹ö¸®´Â ºÎÀÛ¿ëÀÌ ÀÖ´Âµ¥ 
	// ±×·³¿¡µµ ºÒ±¸ÇÏ°í ÀÌÂë¼­ ÇØ µÑ °¡Ä¡°¡ ÀÖ´Ù°í ÆÇ´Ü.
	FLobbySceneManager* LobbySM = GetSceneManager();
	if (LobbySM && !LobbySM->IsSceneStreamingLevelLoaded(ELobbyScene::ELobbyScene_HeroTowerReady))
	{
		LobbySM->PrefetchSceneStreamingLevel(ELobbyScene::ELobbyScene_HeroTowerReady);
	}
}

void FLobbyEnterDungeonMain::ResponseGetChallengeModeInfo(const FB2ResponseGetChallengeModeInfoPtr& MsgPtr)
{
	//// ¸ðµå »óÅÂÁ¤º¸ ÀúÀå
	//UB2UIDocMode* pDocMode = UB2UIDocHelper::GetDocMode();
	//if (pDocMode)
	//	pDocMode->ReceiveGetContentsModeState(MsgPtr->mode_infos);

	//UB2UIDocRaid* pDocRaid = UB2UIDocHelper::GetDocRaid();
	//if (pDocRaid)
	//	pDocRaid->SetPeriodInfos(MsgPtr->raid_open_period_infos);

	//// ¿µ¿õÀÇÅ¾ ³²ÀºÆ¼ÄÏ ¼ÂÆÃ
	//UB2UIDocHeroTower * pDocHeroTower = UB2UIDocHelper::GetDocHeroTower();
	//if (pDocHeroTower)
	//	pDocHeroTower->SetDailyLeftFreeTryCount(MsgPtr->hero_tower_ticket_count);

	//// ¹Ý°Ý´øÀü ³²ÀºÆ¼ÄÏ ¼ÂÆÃ
	//UB2UIDocCounterAttack * pDocCounterAttack = UB2UIDocHelper::GetDocCounterAttack();
	//if (pDocCounterAttack)
	//	pDocCounterAttack->SetLeftTicket(MsgPtr->counter_dungeon_left_ticket_count);

	//// ±Õ¿­ ³²ÀºÆ¼ÄÏ ¼ÂÆÃ
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

	//// ¸µÄ¿¸Å´ÏÀú¸¦ Å¸°í µé¾î¿ÔÀ» °æ¿ì. response¸¦ ¹Þ°í Ä³½ÌµÈ ÆäÀÌÁö·Î ÀÌµ¿½ÃÅ´.
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
	//		//¿µ¿õ°ü¸®¿¡¼­ ¾²ÀÌ´Â Actor¶ó¼­ ÃÊ±â À§Ä¡·Î µ¹¾Æ°¡¾ß¸¸ ÇÑ´Ù.
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
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage, false, true);// Sub Ä³¸¯ÅÍÀÇ Sound notify ¸¦ ²ö´Ù.
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
	//// ¸ðµå »óÅÂÁ¤º¸ ÀúÀå
	//UB2UIDocMode* pDocMode = UB2UIDocHelper::GetDocMode();
	//if (pDocMode)
	//	pDocMode->ReceiveGetContentsModeState(MsgPtr->mode_infos);

	//// Á¡·ÉÀü ¿ÀÇÂ½Ã°£ Á¤º¸ ÀúÀå
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

	//	// ¸µÄ¿¸Å´ÏÀú¸¦ Å¸°í µé¾î¿ÔÀ» °æ¿ì. response¸¦ ¹Þ°í Ä³½ÌµÈ ÆäÀÌÁö·Î ÀÌµ¿½ÃÅ´.
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
	//		//¿µ¿õ°ü¸®¿¡¼­ ¾²ÀÌ´Â Actor¶ó¼­ ÃÊ±â À§Ä¡·Î µ¹¾Æ°¡¾ß¸¸ ÇÑ´Ù.
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
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage, false, true);// Sub Ä³¸¯ÅÍÀÇ Sound notify ¸¦ ²ö´Ù.
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
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // ¾ÈÀü»§.

	//	// ÅÍÄ¡ ¾×¼ÇÀ» À§ÇÑ ÀÔ·Â ¹ÙÀÎµù
	//	LobbyGameMode->BindTouchInput(true);
	//	LobbyGameMode->SetEnableClickEvents(true);
	//}

	//ALevelSequenceActor * DefaultCameraMatinee = GetDefaultCameraMatinee();
	//if (DefaultCameraMatinee)
	//{
	//	DefaultCameraMatinee->SetPosition(0.f);
	//	DefaultCameraMatinee->Play();

	//	SetCameraMatineeCam(DefaultCameraMatinee);
	//}

	//FLobbySceneManager* LobbySM = GetSceneManager();
	//AB2LobbyRelaxingDoumi* RelaxingDoumiActor = LobbySM ? Cast<AB2LobbyRelaxingDoumi>(LobbySM->GetNPCSKActor(ELobbyNPCType::ELobbyNPC_RelaxingDoumi)) : nullptr;
	//if (RelaxingDoumiActor)
	//{ // ½É½ÅÀÇ ÇÇ·Î°¡ Ç®¸®´Â ¶ó¹Ì¿¤ÀÇ È¯¿µ ¾¤.
	//	RelaxingDoumiActor->PlayTemporaryAnimOfState(ELobbyRelaxingDoumiState::ELRD_Welcoming);
	//}
}
void FLobbyRestPointScene::CloseScene()
{
	// ÅÍÄ¡ ¾×¼ÇÀ» À§ÇÑ ÀÔ·Â ¹ÙÀÎµù Á¦°Å.
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
//{ // µÑ »çÀÌ º¯È¯ÀÌ È¤¿©³ª ³Î¸® ¾²ÀÌ°Ô µÇ¸é À§Ä¡´Â ¿Å°Ü¼­..
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
//	// ¹èÄ¡ÇØ ³õÀº ÈÞ½Ä µµ¿ì¹Ì ¶ó¹Ì¿¤ ¾×ÅÍ
//	FLobbySceneManager* LobbySM = GetSceneManager();
//	AB2LobbyRelaxingDoumi* RelaxingDoumiActor = LobbySM ? Cast<AB2LobbyRelaxingDoumi>(LobbySM->GetNPCSKActor(ELobbyNPCType::ELobbyNPC_RelaxingDoumi)) : nullptr;
//	if (RelaxingDoumiActor)
//	{
//		// ÀÏ¹Ý¹Þ±âÀÎÁö, µÎ¹è¹Þ±âÀÎÁö ¿©±â·Î ³Ñ¾î¿Â µ¥ÀÌÅÍ¸¸ °¡Áö°í º¸·Á¸é used_cash °¡ ÀÖ´ÂÁö·Î º¼ ¼öµµ ÀÖ±ä ÇÏÁö¸¸
//		// ±×°Ô ±Ùº»ÀûÀ¸·Î ÀÏ¹Ý¹Þ±â, µÎ¹è¹Þ±â¸¦ Á¤ÀÇÇÏ´Â °Ç ¾Æ´Ï´Ï ÇÒ ÁþÀÌ ¾Æ´Ï°í.. 
//		// ÀÌ »óÈ²¿¡¼­ UIRestPointMain Àº ¿­·Á ÀÖ¾î¾ß ÇÒ Å×´Ï ±×ÂÊ¼­ ¿äÃ»ÇÒ ¶§ ÀúÀåÇØ ³õÀº °ªÀ» ¾ò¾î´Ù ¾´´Ù.
//
//		UB2UIRestPointMain* RestPointUI = UB2UIManager::GetInstance()->GetUI<UB2UIRestPointMain>(UIFName::RestPointMain);
//		if (RestPointUI)
//		{
//			ERestPointRewardRequestState RewardReqState = RestPointUI->GetLastRewardRequestState();
//
//			// ÀÌ check ¿¡ °É¸®´Â °Ç.. ¹º°¡.. ÈÞ½Äº¸»ó µé¾î°¡¼­ ¿äÃ»À» ´­·¶À¸³ª ÀÀ´äÀÌ ¾ø¾î¼­ ³ª°¬´Ù°¡ ´Ù½Ã ÈÞ½Ä º¸»ó Ã¢À¸·Î µé¾î¿Ô´Âµ¥ ±×Á¦¼­¾ß ÀÀ´äÀÌ ¿Ô´Ù°Å³ª?
//			// ¸¸¾à¿¡ Á¤¸» ±×·± ÀÏÀÌ ÀÏ¾î³ª¸é ±×³É ¹«½Ã.
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

	//// ±¸¸Å È½¼ö
	//for (auto Restrict : ShopData->shop_buy_manages)
	//{
	//	DocStore->SetReceivedRestrictCount(Restrict->product_id, Restrict->buy_count);
	//}

	//// ±â°£Á¦ ³²Àº ½Ã°£
	//for(auto PeridicItem: ShopData->periodic_package_states)
	//{
	//	DocStore->SetReceivedPeriodicPackageLeftTime(PeridicItem->package_id, PeridicItem->end_pay_time);
	//}

	//// ·¹º§¾÷ ÆÐÅ°Áö
	//for (auto LevelUpMissionItem : ShopData->level_up_package_missions)
	//{
	//	DocStore->SetReceivedLevelupPacakgeMissions(LevelUpMissionItem->package_id, LevelUpMissionItem);
	//}

	//// ·¹º§ ´Þ¼º °è¾à ÆÐÅ°Áö
	//for (auto ReachLevelMissionItem : ShopData->reach_level_package_missions)
	//{
	//	DocStore->SetReceivedReachLevelPacakgeMissions(ReachLevelMissionItem->package_id, ReachLevelMissionItem);
	//}

	//// ¿ùÁ¤¾×
	//if (ShopData->flat_rate_package_bonus_state)
	//{
	//	DocStore->SetReceivedFlatratePackageBonusState(ShopData->flat_rate_package_bonus_state);
	//}

	//// ÆÐÅ°Áö ÀÌº¥Æ® ¸¶Å©
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
	//	// ÇÑ¹ø Àç»ý ÇßÀ¸¸é
	//	if (PlayedMatinee[CurrentHallOfFameMode])
	//	{
	//		ForceEndMatinee();
	//	}
	//	// Àç»ý ¾ÈÇßÀ¸¸é
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

void FLobbyHallOfFame::OnFinishedHallOfFameDisplayMatinee(class ALevelSequenceActor * Matinee)
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
	//		LobbyGM->SetupLoadingScreenImageCommon(false, false, false); // ÁøÇàµµ´Â ¾øÀÌ ·ÎµùÈ­¸é¸¸ ¶ç¿ò.
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

// 1µî Ä³¸¯Àº ¸Þ½¬°¡ 4°³
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
	//		// ÀÚµ¿ °»½Å
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

	//// ´çÀÏ °»½Å È½¼ö
	//DocStore->SetRenewalCount(MagicShopPtr->renewal_count);

	//// ´ÙÀ½ ÀÚµ¿ °»½Å ½Ã°£
	//DocStore->SetNextFreeRenewalTime(MagicShopPtr->next_free_renewal_time);

	//// ¹«·á °»½Å ¿©ºÎ
	//DocStore->SetIsFreeRenewal(MagicShopPtr->is_free_renewal);

	//// ¸¶¹ý »óÁ¡ »óÇ° ¸®½ºÆ®
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
	//const float PinpointWeight = FMath::Clamp(ZWeight, -0.3f, 1.f);	// 0¿¡ °¡±î¿ï¼ö·Ï Ä³¸¯ÅÍÀÇ Áß½É¿¡ Pinpoint

	//const float WeightConstant = 0.5f;	// »ó ~ ÇÏ´Ü¿¡ ÇÉÆ÷ÀÎÆ®°¡ °¡±î¿ï¼ö·Ï 50% ÀÇ ¿©ºÐ Distance¸¦ ÁÜ
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

	//	// Camera ÀÌµ¿ Direction
	//	const bool IsZoomIn = Delta > 0 ? true : false;
	//	const FVector CameraMoveVector = IsZoomIn ? (PinpointLocation - CurrCamLocation) : (OrgCameraLocation - CurrCamLocation);
	//	if (CameraMoveVector.IsNearlyZero(1.f))
	//		return;

	//	const FVector CameraMoveDir = CameraMoveVector.GetSafeNormal();

	//	// Camera ÀÌµ¿ ¿¹»ó À§Ä¡
	//	const FVector ExpectCameraLocation = CurrCamLocation + (CameraMoveDir * Delta);

	//	// ÇöÀç Ä«¸Þ¶ó¿Í Ä³¸¯ÅÍ °Å¸®
	//	const float CurrCamDistance = (CurrCamLocation - PinpointLocation).Size();

	//	// ÀÌµ¿ ¿¹»ó À§Ä¡¿¡¼­ÀÇ Ä«¸Þ¶ó ¿Í Ä³¸¯ÅÍ °Å¸®
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
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // ¾ÈÀü»§.

	//												  // ÅÍÄ¡ ¾×¼ÇÀ» À§ÇÑ ÀÔ·Â ¹ÙÀÎµù
	//	LobbyGameMode->BindTouchInput(true);
	//	LobbyGameMode->SetEnableClickEvents(true);
	//}

	//ALevelSequenceActor * DefaultCameraMatinee = GetDefaultCameraMatinee();
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

	//// ÀÌÀü Æä¾î¸® ¾Èº¸ÀÌµµ·Ï.
	//AB2LobbyFairyDoumi* FairyBeforeActor = LobbySM ? Cast<AB2LobbyFairyDoumi>(LobbySM->GetNPCSKActor(BeforeType)) : nullptr;
	//if (FairyBeforeActor)
	//{
	//	FairyBeforeActor->SetVisibleCharacter(false);
	//}

	//// ´ÙÀ½ Æä¾î¸®´Â º¸ÀÌ°ÔÇÏ°í, ¾Ö´Ï¸ÞÀÌ¼Ç Àç»ý
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
//	// ÅÍÄ¡ ¾×¼ÇÀ» À§ÇÑ ÀÔ·Â ¹ÙÀÎµù Á¦°Å.
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
	//		//¿µ¿õ°ü¸®¿¡¼­ ¾²ÀÌ´Â Actor¶ó¼­ ÃÊ±â À§Ä¡·Î µ¹¾Æ°¡¾ß¸¸ ÇÑ´Ù.
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