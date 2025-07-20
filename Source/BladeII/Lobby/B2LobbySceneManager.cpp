
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
	//// 隐藏 LevelSequenceActor 本身
	//LevelSequenceActor->SetActorHiddenInGame(bHidden);

	//// 获取绑定的 LevelSequence 资源
	//ULevelSequence* LevelSequence = Cast<ULevelSequence>(LevelSequenceActor->GetSequence());
	//if (!LevelSequence)
	//	return;

	//UMovieScene* MovieScene = LevelSequence->GetMovieScene();
	//if (!MovieScene)
	//	return;

	//// 获取绑定信息列表
	//const TArray<FMovieSceneBinding>& Bindings = MovieScene->GetBindings();

	//for (const FMovieSceneBinding& Binding : Bindings)
	//{
	//	// 构造绑定 ID
	//	FMovieSceneObjectBindingID BindingID(Binding.GetObjectGuid(), MovieSceneSequenceID::Root, EMovieSceneObjectBindingSpace::Local);

	//	// 获取绑定对象
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
	/* 弊成 困俊 竞咯瘤绰 UI甸 汲沥甸篮 眠啊窍瘤 臼酒档 凳.	
	case ELobbyScene::ELobbyScene_Mail:
	*/
		return ELobbyLevelType::ELobbyLevelType_A;

	case ELobbyScene::ELobbyScene_LobbyMain:
	case ELobbyScene::ELobbyScene_CharacterObserve:
		return ELobbyLevelType::ELobbyLevelType_B;

	case ELobbyScene::ELobbyScene_Chapter: // OptionalChapterNum 俊 蝶扼 ELobbyLevelType 阑 备盒秦辑 府畔
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

	//	//SimpleDirectionalLight regist 巩力肺 RemoveFromWorld甫 刚历 阂矾具凳.
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
	//		if (CurrentLobbyScene != ELobbyScene::ELobbyScene_Title && // 鸥捞撇俊辑 哎 锭绰 坷洒妨 竭蹲嘎篮 肺爹 拳搁捞 肚 唱柯促 酵绢辑 粱 弊犯促..				
	//			(ToChangeScene == ELobbyScene::ELobbyScene_LobbyMain || ToChangeScene == ELobbyScene::ELobbyScene_CharacterObserve)
	//			&& !StreamingLevel->IsLevelLoaded())
	//		{ // 肺厚俊 捞固 甸绢柯 饶俊 LobbyMain 辑宏饭骇阑 肺爹秦具 窍绰 惑炔篮 漂洒 变 矫埃阑 夸备窍骨肺 肺爹 拳搁阑 剁款促.
	//			// 焊烹 傈捧俊辑 倒酒柯 饶俊 LobbyMain 栏肺 哎锭 弊犯霸 等促.
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
	//		GameMode->SetupLoadingScreenImageCommon(false, false, false); // 柳青档绰 绝捞 肺爹拳搁父 剁框.
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

bool FLobbyLevelVisibleManager::IsStreamingLevelLoaded(ELobbyScene CheckScene, int32 OptionalChapterNum /*OptionalChapterNum 篮 茅磐 纠牢 版快 鞘夸*/)
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
 * 弥利拳 格利栏肺 归弊扼款靛肺 固府 肺爹秦 滴绰 扁瓷. 荤侩篮 利例茄 矫痢俊.. 肋给 荤侩窍搁 坷洒妨 何累侩捞 惯积且 荐 乐促.
 */
void FLobbyLevelVisibleManager::PrefetchStreamingLevel(ELobbyScene PrefetchScene)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("FLobbyLevelVisibleManager::PrefetchStreamingLevel %d"), (int32)PrefetchScene));

	// 茅磐 纠档 购啊 秦 焊妨搁 锅龋 冈咯林搁 登绰单 磊末窍搁 荤侩 救 窍蠢聪父 给茄 捞繁 刮皑茄 扁瓷阑 弊犯霸 澄府 荤侩且 啊瓷己阑 林蠢聪 弊成 力傍 救窍绊 富瘤.
	check(PrefetchScene != ELobbyScene::ELobbyScene_Chapter);

	TArray<FName> LevelNameList;
	LevelsRequiredScene.MultiFind(GetLobbyLevelType(PrefetchScene), LevelNameList);

	auto* GameMode = LobbySceneManager ? LobbySceneManager->GetLobbyGameMode() : NULL;
	UWorld* TheWorld = GameMode ? GameMode->GetWorld() : nullptr;
	if (TheWorld)
	{
		for (auto& PrefetchLevelName : LevelNameList)
		{// bShouldBlokcOnLoad 救 林绊 肺爹 妮. 弊成 捞犯霸 秦 滴绊 郴厚滴绰 芭.
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
		// LobbyScene 捞 力芭瞪 锭鳖瘤 巢酒 乐阑 event subscribe.
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
	//// OptionalToChangeChapterNum 篮 ELobbyScene_Chapter 狼 版快父
	//if (CurrentLobbyScene == ToChangeScene && !bForceChange && (ToChangeScene != ELobbyScene::ELobbyScene_Chapter || CurrentChapterViewNum == OptionalToChangeChapterNum))
	//	return;

	////Close
	//uint8 CurLobbySceneIndex = static_cast<uint8>(CurrentLobbyScene);
	//if (LobbyScenes.IsValidIndex(CurLobbySceneIndex))
	//	LobbyScenes[CurLobbySceneIndex]->CloseScene();

	////巩力啊 乐绢辑 UIScene篮 固府 函版.. 康旷包府纠 (FLobbyHeroMgmtScene) DJLegacy UI 绰 捞吧肺 冈洒瘤绰 臼阑 芭绊.. 弊 版快绰 UI 函版 目盖靛绰 蝶肺 啊档废 秦具 (LobbyEnterHeroMgmtModeClass 甫 烹秦)
	//uint8 ToChangeSceneInt = static_cast<uint8>(ToChangeScene);
	//if (LobbyScenes.IsValidIndex(ToChangeSceneInt))
	//{
	//	EUIScene ToChangeUIScene = LobbyScenes[ToChangeSceneInt]->GetUIScene();
	//	if (UB2UIManager::GetInstance()->GetCurrUIScene() != ToChangeUIScene)
	//	{
	//		UB2UIManager::GetInstance()->ChangeUIScene(ToChangeUIScene);

	//		B2GMLoadingProgCollector::Step(); // 甘 肺爹 鸥烙俊 肺爹 柳青档 持栏妨绰 扒单 甘 肺爹 矫痢 酒聪搁 喊 老 绝阑 巴
	//	}
	//}

	//const ELobbyLevelType CurrLevelType = LevelManager.GetLobbyLevelType(CurrentLobbyScene, CurrentChapterViewNum);
	//const ELobbyLevelType ToChangeLevelType = LevelManager.GetLobbyLevelType(ToChangeScene, OptionalToChangeChapterNum);

	//bool bToLoadNewLevels = (CurrLevelType != ToChangeLevelType && ToChangeLevelType != ELobbyLevelType::ELobbyLevelType_None)
	//	// 咯扁辑 LevelType 父栏肺绰 角力 度鞍篮 胶飘府怪 饭骇阑 Unload/Load 且瘤 肯傈洒 舅 荐啊 绝绢辑 眠啊 八荤. 鞍篮 胶飘府怪 饭骇阑 郴啡促 促矫 棵府绰 霸 酒鳖况辑 弊矾绰 霸 酒聪扼 粱 巩力啊 乐绢 焊烙.
	//	&& !LevelManager.HasSameStreamingLevelToCurrentScene(ToChangeScene, CurrentChapterViewNum, OptionalToChangeChapterNum);

	//if (bToLoadNewLevels)
	//{
	//	ResetObjectRefsOnSceneLevelStreaming();

	//	LevelManager.ChangeVisibility(ToChangeScene, CurrentChapterViewNum, OptionalToChangeChapterNum);

	//	B2GMLoadingProgCollector::Step(); // 甘 肺爹 鸥烙俊 肺爹 柳青档 持栏妨绰 扒单 甘 肺爹 矫痢 酒聪搁 喊 老 绝阑 巴
	//}

	//// 货肺款 饭骇阑 肺爹窍绰 版快啊 酒聪扼档 某腐磐 炼钦阑 促矫 秦具 窍绰 版快.. 鞘夸俊 蝶扼 眠啊. 档皑篮 某腐磐 厘厚 炼钦阑 烙狼肺 窍骨肺 唱哎 锭 公炼扒 货肺 炼钦且 鞘夸啊 乐促.
	//const bool bUnconditionalUpdatePart = (CurrentLobbyScene == ELobbyScene::ELobbyScene_CollectBookMain);

	////woong 某腐磐惑技焊扁 某腐磐啊 亲惑 4俺啊 酒聪骨肺 咯扁辑 某腐磐 诀单捞飘甫 给窍霸秦具钦聪促
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

	////[@AKI, 170718] <B2CLT-1789> 肺厚 楷免 俺急栏肺 林籍 贸府 窃.
	////眠饶 傈眉 某腐甸捞 唱客乐阑锭 某腐疙 殿阑 焊咯 林绊 酵栏搁 林籍阑 秦力 窍搁 凳.
	////-> WidgetClass 肺爹 棺 CreateWidget 篮 扯厚啊 登骨肺 厚劝己拳 秦 初篮 惑怕.. 汗备窍妨搁 RESTORE_BII_LOBBY_ONHEAD_DISPLAY 叼颇牢阑 难具.
	////UpdateLobbyOnHeadDisplayWidget();

	////Open
	//CurLobbySceneIndex = static_cast<uint8>(CurrentLobbyScene);
	//if (LobbyScenes.IsValidIndex(CurLobbySceneIndex))
	//	LobbyScenes[CurLobbySceneIndex]->OpenScene();
}

bool FLobbySceneManager::IsSceneStreamingLevelLoaded(ELobbyScene CheckScene, int32 OptionalChapterNum)
{ // 澄府 荤侩且 芭 酒聪扼档 捞扒 窃荐 狼固 惑 扁瓷阑 肯傈窍霸 父甸 鞘夸绰 乐绢辑 Chapter 纠狼 版快档 贸府..せ
	return LevelManager.IsStreamingLevelLoaded(CheckScene, OptionalChapterNum);
}
void FLobbySceneManager::PrefetchSceneStreamingLevel(ELobbyScene PrefetchScene)
{
	// 肺厚 辑宏饭骇 吝 耽摹 奴 吧 利寸茄 矫痢俊 固府 肺爹且 锭 荤侩.
	// 促弗 俊悸 肺爹苞 爵囚辑 flush 啊 奖霸 蜡惯登绰 惑炔捞搁 何累侩捞 歹 努 荐 乐栏聪 利例茄 矫痢俊 肋 舅绊 荤侩.
	if (PrefetchScene != ELobbyScene::ELobbyScene_Chapter) // 锅龋 逞败林扁 饺绢辑 茅磐 纠 力寇. 弊促瘤 澄府 荤侩瞪 扁瓷档 酒聪绊.. 澄府 荤侩秦辑 亮阑 巴档 绝绊.
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
	//		if (!bPrevForLobbyMainSceneAMB) // 捞傈俊 敲饭捞窍瘤 臼篮 版快父. 酒公贰档 捞芭 谗扁搁 救亮酒且 芭 鞍澜.
	//		{
	//			GameMode->PlayAMBByCommonSoundID(ECommonSoundID::ECSID_AMB_LobbyMainScene);
	//		}
	//	}
	//	else
	//	{
	//		GameMode->StopAMB(); // 酒流篮 促弗 厘搁俊 AMB 啊 绝澜.
	//	}
	//}
}

void FLobbySceneManager::UpdateLobbyOnHeadDisplayWidget(EPCClass OptionalCreateOnlyClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::UpdateLobbyOnHeadDisplayWidget"));

	//// 老窜 扁粮俊 父电 芭 力芭
	//DestroyAllLobbyCharOnHeadDisplayClass<>::GetInstance().Signal();

	//// 努贰胶喊肺 倒搁辑 犁积己. 嘿捞绊磊 窍绰 扒 TypeB 局甸捞促.
	//if (LevelManager.GetLobbyLevelType(CurrentLobbyScene) == ELobbyLevelType::ELobbyLevelType_B)
	//{
	//	for (TMap<EPCClass, FLobbyCharacter>::TIterator It(LobbyCharactersForBLobbyType); It; ++It)
	//	{
	//		EPCClass ThisClass = It.Key();
	//		FLobbyCharacter& ThisCharInfo = It.Value();
	//		if (ThisClass != EPCClass::EPC_End && ThisCharInfo.LevelActor && ThisCharInfo.LevelActor->IsValidObj() && 
	//			// Observe 厘搁牢 版快 OptionalCreateOnlyClass 甫 瘤沥秦辑 泅犁 某腐磐 酒囱 巴甸阑 疙矫利栏肺 吧矾郴绊, 见败初篮 巴档.
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
{ // 酒轿府 寇何俊辑 碍力 诀单捞飘 沥档狼 侩档.. 乐绢具 且 版快俊父 诀单捞飘.
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
	FLobbyNPCCharacterInfo& FoundOrAddElem = LobbyNPCs.FindOrAdd(InType); // 吝汗 龋免瞪瘤 隔扼辑
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
 * bForceUpdate 绰 鞘荐利牢 惑炔捞 酒聪搁 true 肺 林瘤 富 巴. bForceUpdate 甫 林绰 霸 悼累俊 捞惑阑 老栏虐瘤绰 臼瘤父 己瓷俊 零距捞 瞪 荐 乐澜.
 * 鞘荐利牢 惑炔篮 档皑鞍捞 老矫利栏肺 促弗 厘厚 厘馒阑 焊咯霖促芭唱, 摹飘甫 朝府绰 惑炔 殿捞 登摆促.
 * 扁夯利栏肺 阿 某腐磐 努贰胶 喊肺 merge 茄 皋浆绰 厘馒 惑怕, 朝俺 咯何俊 蝶扼 refresh 甫 秦具窍绰瘤 魄喊阑 窃.
 */
void FLobbySceneManager::SetupLobbyCharCompForParts(EPCClass InPCClass, class ASkeletalMeshActor* InSKActorToSet, bool bForceUpdate /*= false*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("FLobbySceneManager::SetupLobbyCharCompForParts"));
//
//	// ABladeIIPlayer::SetupComponentsForParts 俊 秦寸. 咯扁急 肺厚甘俊 冠酒初篮 SkeletalMeshActor 措惑栏肺
//
//	if (InPCClass == EPCClass::EPC_End || !InSKActorToSet || !InSKActorToSet->IsValidObj() || !InSKActorToSet->GetWorld()){
//		return;
//	}
//
//	TArray<FB2Item> EquippedItems;
//	
//	AB2LobbyGameMode* B2LobbyGM = GetLobbyGameMode();
//	BII_CHECK(B2LobbyGM);
//	// 盖 贸澜 唱坷绰 某腐磐 家俺 拳搁俊辑绰 6己 厘厚 厘馒等 巴阑 焊咯霖促.
//	const bool bUseFixedGradeEquip = (CurrentLobbyScene == ELobbyScene::ELobbyScene_CharacterIntro);
//		//|| (CurrentLobbyScene == ELobbyScene::ELobbyScene_Title) // 鸥捞撇 拳搁俊辑绰 捞力 悼康惑捞 唱柯促..
//	bForceUpdate |= bUseFixedGradeEquip; // 家俺 拳搁 6己 厘厚 厘馒篮 ForceUpdate.
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	// 漂沥 喊 己 荐狼 厘厚甫 老褒 馒侩且 荐 乐档废 窍绰 摹飘牢单 SetupSKCompForParts 救俊辑 厘厚甫 函版窍骨肺 ForceUpdate 脚龋甫 焊郴林绢具 茄促.
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
//	// 哪器窿 皋浆 搬苞拱阑 某教秦 初阑瘤 咯何. 
//	// 肺厚俊辑绰 泅犁 炼钦阑 窍绰 霸 肺拿 某腐磐 侩牢瘤 酒囱瘤 备盒阑 且 荐 乐绰 促弗 利例茄 沥焊啊 绝栏骨肺 咯扁辑 肋 逞败林绢具 窃.
//	bool bCanCacheCompositedMesh = true;
//
//	if (bUseFixedGradeEquip)
//	{
//		bCanCacheCompositedMesh = false; // 烙矫利牢 巴栏肺 搬苞拱 某教 救窃.
//		// 葛滴 6己栏肺 窍绰 措脚 某腐磐喊肺 夸没茄 grade 啊 乐绢辑 弊吧肺..
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
//		// 漂喊洒 霖厚等 窃荐烙. 葛辆狼 痹距俊 蝶扼 RefID 炼钦窍绊 鞘夸茄 蔼甸父 盲况辑 持绢淋.
//		GetStandardDummyPCPartItemData(InPCClass, StandardDummyPCPartItemGrade, EquippedItems);
//	}
//	else if (CurrentLobbyScene == ELobbyScene::ELobbyScene_CollectBookMain)
//	{
//		bCanCacheCompositedMesh = false; // 烙矫利牢 巴栏肺 搬苞拱 某教 救窃.
//		UB2UICollectBookMain* CollectBookUI = UB2UIManager::GetInstance()->GetUI<UB2UICollectBookMain>(UIFName::CollectBookMain);
//		if (CollectBookUI)
//		{
//			bHasWing = false; // 档皑俊辑绰 WingTab 甸绢啊扁 傈鳖瘤绰 救 唱咳
//
//			EquippedItems = CollectBookUI->GetCollectBookCurrentEquipItems();
//			const int32 WingIndex = static_cast<int32>(EItemClass::EIC_Wing);
//			if (EquippedItems.IsValidIndex(WingIndex) && EquippedItems[WingIndex].ItemClass == EItemClass::EIC_Wing)
//			{
//				// WingTab俊 甸绢啊搁 磊悼栏肺 EquippedItems俊 殿废登绊 厘馒!
//				bHasWing = true;
//				// 朝俺 寇包篮 努贰胶客 柳拳 殿鞭俊 蝶扼辑父 搬沥.
//				WingData.OwnerPCClass = PCClassToInt(InPCClass);
//				WingData.EvolutionGrade = EquippedItems[WingIndex].StarGrade;	// 柳拳绰 StarGrade俊 冠磊
//				
//				WingData.bShouldBeVisible = true; // 档皑捞搁 FLobbyCollectBookMain 俊辑 SetUnconditionalShowWing 阑 窍绰 措脚 咯扁辑 捞吧 流立 持磊. 付蛮啊瘤变 且 扒单 舵捞 粱 促弗 淀..
//			}
//		}
//	}
//	else if (CurrentLobbyScene == ELobbyScene::ELobbyScene_FindAccount
//		&& bCanUseRemoteInfoForSKMeshSetup // CurrentLobbyScene 父 眉农窍搁 龋免 胶琶 惑狼 鸥捞怪栏肺 牢秦 角力肺绰 肺拿 某腐磐甫 焊咯林绰 轰肺 啊绰 矫痢俊 咯扁俊 吧妨辑 农贰浆啊 惯积且 荐 乐栏骨肺 眠啊 眉农甫 茄促.
//		)
//	{
//		bCanCacheCompositedMesh = false; // 烙矫利牢 巴栏肺 搬苞拱 某教 救窃. 促弗 敲饭捞绢 皋浆绰 某教窍绰 版快 开瓤苞啊 朝 荐 乐促.
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
//				bCanCacheCompositedMesh = false; // 烙矫利牢 巴栏肺 搬苞拱 某教 救窃. 促弗 敲饭捞绢 皋浆绰 某教窍绰 版快 开瓤苞啊 朝 荐 乐促.
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
//		// 老馆利牢 肺拿 敲饭捞绢 某腐磐狼 版快
//		UB2LobbyInventory::GetAllEquippedItem(EquippedItems, InPCClass);
//		UB2LobbyInventory::GetAllEquippedCostumeItem(EquippedItems, InPCClass);
//		bHasWing = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InPCClass, WingData);
//	}
//
//	if (bHasWing && bUnconditionalShowWing) // 朝俺 柳拳 厘搁捞唱 档皑 殿俊辑 甸绢哎 炼扒. 扁夯利栏肺 bShouldBeVisible 篮 荤侩磊 急琶俊 蝶扼 历厘登绰 蔼.
//	{
//		WingData.bShouldBeVisible = true;
//	}
//
//	// 楷免On矫 朝俺 力芭
//	if (CurrentLobbyScene == ELobbyScene::ELobbyScene_CharacterPage)
//	{
//		UB2UIDocUICondition* DocCon = UB2UIDocHelper::GetDocUICondition();
//
//		if(!DocCon->GetbUserSelectedSkipStory())
//			WingData.bShouldBeVisible = false;
//	}
//
//	//Mesh Render Item Filtering (固府焊扁, visible 可记)
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
//	const bool bMergeSections = false; // 肺厚绰 Section Merge 甫 镜 啊瓷己阑 沥富 积阿窍扁 绢妨快唱 八祸 怕弊 沥档绰 崔酒滴磊 せ UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent
//
//	if (ThisClassInfo && BaseMeshComp)
//	{
//		USkeletalMesh* PrebuiltMeshOrResult = NULL;
//		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//		if (!bForceUpdate && bCanCacheCompositedMesh)
//		{ // Mesh merge 窍绰 葛靛 茄沥, 捞傈俊 捞固 度鞍篮 炼扒栏肺 呼靛秦 初篮 皋浆啊 乐促搁 弊吧 波郴 敬促. 
//			// 泅犁 肺厚俊辑绰 PCClass 父阑 虐肺 秦辑 呼靛茄 皋浆甫 某教窍绰单
//			// 父老 PCClass 绰 鞍绊 辑肺 促弗 SkeletalMeshActor 啊 茄锅俊 焊咯具 窍绰 惑炔捞 坷搁 PCCompositeMeshCacher 狼 虐蔼捞 弊吧 馆康且 荐 乐档废 秦具.
//			PrebuiltMeshOrResult = MeshCacheManager ? 
//				MeshCacheManager->GetCachedCompositeMesh(InPCClass, NULL, EquippedItems, (bHasWing && WingData.bShouldBeVisible), &WingData, bMergeSections)
//				: NULL; 
//		}
//
//		if (UB2PCClassInfo::SetupSKCompForParts(InPCClass, InSKActorToSet, BaseMeshComp, PrebuiltMeshOrResult, ThisClassInfo->BaseMeshAsset, ThisClassInfo->DefaultPartsInfo,
//			// 霉 拳搁 6己 厘厚 炼钦 焊咯临 锭绰 朝俺甫 哗 焊磊. 芭扁辑 绢瞒乔 焊捞瘤 富磊绊 且 芭 鞍促.
//			EquippedItems, (bHasWing && !bUseFixedGradeEquip) ? &WingData : NULL, NULL,
//			bForceUpdate, // ForceUpdate 狼 版快绰 老矫利牢 preview 肺 焊绊磊 窃捞 农扁 锭巩俊 荤侩茄 颇飘 府家胶甫 朝府档废 窃.
//			bMergeSections
//			))
//		{
//			if (MeshCacheManager && bCanCacheCompositedMesh)
//			{ // MeshMerging 捞 角青登绰 葛靛俊辑狼 搬苞啊 己傍利捞扼搁 CompositeMeshCache 率俊 某教秦 初澜. 郴何利栏肺绰 角力 诀单捞飘啊 登菌阑 锭俊父 某教捞 瞪 巴烙.
//				MeshCacheManager->SetCachedCompositeMesh(InPCClass, NULL, PrebuiltMeshOrResult, EquippedItems, (bHasWing && WingData.bShouldBeVisible), &WingData, bMergeSections);
//			}
//		}
//
//		ABladeIIPlayer::SetupLODInfoForLobbyRepresentative(InSKActorToSet);
//	}
}


/** 
 * 己瓷俊 零距捞 瞪 荐 乐绢辑 碍力 诀单捞飘 牢磊绰 力傍窍瘤 臼绰促.
 * 扁夯利栏肺 阿 某腐磐 努贰胶 喊肺 merge 茄 皋浆绰 厘馒 惑怕, 朝俺 咯何俊 蝶扼 refresh 甫 秦具窍绰瘤 魄喊阑 窃.
 */
void FLobbySceneManager::UpdateLobbyCharCompForParts(EPCClass InPCClass)
{ // 厘厚 厘馒 惑怕 诀单捞飘 殿狼 版快 荤侩窍扁 困窃.
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
	//		// G 呐 鸥涝篮 康旷包府父 诀单捞飘.. 档皑篮 捞犯霸 诀单捞飘 窍搁 救瞪 淀?
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
//	// 某腐磐 厘厚 炼钦 包访 捞繁 历繁 抛胶飘俊 夸变窍霸 荤侩窍扁 困秦.. 
//	// 惑窜 UpdateLobbyCharCompForParts 客 芭狼 厚搅窍瘤父 包府甫 糊技霸 且 鞘夸绰 绝摆促.
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
//		check(AllPLForObserveView.Num() <= MobileNumDynamicPointLights); // 坊歹矾 茄档甫 逞扁绰 老 绝档废.
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
//		check(AllPLForAllCharView.Num() <= MobileNumDynamicPointLights); // 坊歹矾 茄档甫 逞扁绰 老 绝档废.
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

	//	//付萍聪 捞亥飘肺 贸府窍绰 巴档 宝满阑 淀.
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

	//			// RUN 场唱绰 矫埃
	//			BladeIIGameImpl::GetClientDataStore().GetDuelModeSettleSchedule(
	//				b2network::B2ContentsMode::PVP_DUEL,
	//				b2network::B2ContentsModeState::RUN,
	//				nStartDay,
	//				nStartHour,
	//				nStartMin);

	//			// SETTLE_END 场唱绰 矫埃
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
	//			if (LobbyMainUIPage->GetCombineMenuIsVisible()) // 烹钦皋春啊 栋乐绰 惑怕俊辑绰 荤侩救登霸阐
	//				return;
	//		}
	//		UB2UIMsgPopupSelectFairyGift* pMsgPopupSelectFairyGift = UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupSelectFairyGift>(UIFName::MsgPopupSelectFairyGift);
	//		if (pMsgPopupSelectFairyGift)
	//		{
	//			// 其绢府 焊惑 扑诀捞 栋乐绰 惑怕俊辑档 荤侩 救登霸阐
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
//				if (LobbyMainUIPage->GetCombineMenuIsVisible()) // 烹钦皋春啊 栋乐绰 惑怕俊辑绰 荤侩救登霸阐
//					return;
//			}
//			UB2UIMsgPopupSelectFairyGift* pMsgPopupSelectFairyGift = UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupSelectFairyGift>(UIFName::MsgPopupSelectFairyGift);
//			if (pMsgPopupSelectFairyGift)
//			{
//				// 其绢府 焊惑 扑诀捞 栋乐绰 惑怕俊辑档 荤侩 救登霸阐
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
	{ // 捞扒 HeroManagement 俊辑 HeroManagement 肺狼 (force) 技泼烙. 鸥捞怪栏肺 牢秦 促弗 葛靛肺 逞绢埃 惑怕俊辑 捞霸 阂府搁 救等促.
		ChangeLobbyScene(ELobbyScene::ELobbyScene_HeroManagement, -1, true);
	}
}

bool FLobbySceneManager::IsInItemOpDirectingView()
{ // 犬牢 瞒盔俊辑 鞘夸茄 扁瓷
	//if (ItemOpSceneCamera && ItemOpSceneCamera->IsValidObj() && ItemOpSceneCamera->GetCameraComponent())
	//{
	//	APlayerController* LocalController = UGameplayStatics::GetLocalPlayerController(ItemOpSceneCamera);
	//	return (LocalController && LocalController->GetViewTarget() == ItemOpSceneCamera);
	//}
	return false;
}

void FLobbySceneManager::OnLobbyGMActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject)
{
	// CurrentLobbyScene 俊父 ActorCustomEvent 甫 傈崔.
	uint8 CurLobbySceneIndex = static_cast<uint8>(CurrentLobbyScene);
	FLobbySceneBase* CurrentActiveLobbySceneRef = LobbyScenes.IsValidIndex(CurLobbySceneIndex) ? LobbyScenes[CurLobbySceneIndex] : NULL;

	if (CurrentActiveLobbySceneRef){
		CurrentActiveLobbySceneRef->OnLobbyGMActorCustomEvent(OptionalEventName, OptionalNotifyingObject);
	}
}

void FLobbySceneManager::ResetObjectRefsOnSceneLevelStreaming()
{
	// 捞霸 UPROPERTY 肺 弓牢 霸 酒聪扼辑 TWeakObjectPtr 捞 酒囱 捞惑 GC 扼档 登搁 静饭扁 器牢磐啊 瞪 巴捞促.
	// 辟单 阿阿狼 备泅捞 绢恫 侥牢瘤 隔扼辑 促 府悸秦档 登绰瘤绰 葛福摆澜.
	// LobbySceneManager 啊 LobbyGameMode 力芭瞪 订 鞍捞 朝酒啊扁 锭巩俊 农霸 巩力狼 家瘤啊 乐绰 荐霖篮 酒聪绊..
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
	// 傈眉 LobbyScene (LobbyGameMode) 苞 扼捞橇 荤捞努阑 鞍捞 且 event subscribe 甫 overriding 阑 烹秦 沥狼
	// 角力肺 咯扁甫 烹秦 subscribe 窍绰 霸 官恩流茄 葛嚼篮 酒聪瘤父 坷法悼救 弊繁 备炼肺 父甸绢廉 吭绢辑 疙矫利栏肺 力傍窃.
}
void FLobbySceneBase::UnsubscribeEvents_OnDestruct()
{
	// SubscribeEvents_OnConstruct 俊辑 subscribe 茄 巴篮 捞吧 override 秦辑 unsubscribe 茄促.
}

void FLobbySceneBase::SubscribeEvents_OnOpen()
{
	// 漂沥 LobbyScene 捞 劝己拳等 悼救俊父 利侩瞪 event subscribe 甫 overriding 阑 烹秦 沥狼
}
void FLobbySceneBase::UnsubscribeEvents_OnClose()
{
	// SubscribeEvents_OnOpen 俊辑 subscribe 茄 巴篮 捞吧 override 秦辑 unsubscribe 茄促.
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
//	// MatineeActor 俊 硅摹茄 付萍聪 飘发 弊缝俊辑 力老 刚历 茫绰 墨皋扼 飘发 弊缝狼 KeyTime 阑 泅犁 墨皋扼肺 汲沥.
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
	//{ // 沥惑利牢 惑炔捞搁 惑痢惶扁 流饶 惑痢 倒酒客辑 府悸瞪 巴牢单 趣矫 葛福绰 惑炔捞 乐阑瘤 葛福聪 犬牢荤混 瞒盔. 
	//	// 夸霸 利绢档 LobbyMain 纠阑 逞唱甸哥 蜡瘤瞪 捞蜡绰 绝阑 巴.
	//	LobbySM->ResetShopGirlReservedState();
	//}

	//FLobbySceneBase::OpenScene();

	//auto* DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (!DefaultCameraMatinee) // 滚弊 惑炔捞 乐绢辑 BeginPlay 傈俊 捞霸 阂副 锭啊 乐澜. 碍力 某教
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
	//		// 咯变 局聪皋捞记阑 付萍聪 敲饭捞肺 备悼矫懦 巴捞骨肺 泪矫 官层狄..
	//		charinfo.Value.LevelActor->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//	}
	//}

	//ElapsedTimeNoInput = ElapsedOpenScene = 0.0f;
	//bVisibleUI = true;
	//
	//// UB2UILobbyMain 阑 见扁绰 汲沥阑 UB2UILobbyMain 俊辑 啊廉客辑 沥累 鸥捞赣绰 咯扁辑 倒府绰 侥牢单 扁粮 备泅俊 嘎苗 汲沥篮 唱吝俊 持促焊聪 弊犯霸 等 芭.
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
	//{ // 官肺 酒贰俊 ObserverScene 栏肺 傈券窍扁 傈俊 MainScene 俊辑狼 扼捞泼 悸诀阑 秦具 茄促.
	//	// 捞霸 弊 促澜俊 角青登搁 酒贰俊辑 ObserverScene 栏肺 傈券窍搁辑 弊率 扼捞泼 悸诀捞 等 吧 丹绢静霸 凳.
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
	//if (ElapsedTimeNoInput >= PlayRateRestoreTime) // 老沥 矫埃 牢遣捞 绝阑 矫 付萍聪 犁积 矫埃阑 盔贰措肺 倒妨 初绰促
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
	//if (ElapsedTimeNoInput >= AutoHideInterval) // 老沥 矫埃 牢遣捞 绝阑 矫 皋牢 UI 甫 濧 糠栏肺 见变促.
	//{
	//	SetHiddenLobbyMainUI(true);
	//}
	//*/

	////第肺啊扁 滚瓢捞 Push event肺 官差搁辑 促弗 Scene俊辑 倒酒棵 锭, Release event啊 冈囚 某腐磐啊 急琶登绢 滚府绰 滚弊 荤亲捞 唱棵 荐 乐促. 
	////捞 脚俊辑 Press event甫 某摹茄 饶, Release event啊 阂副锭父 角青窍霸 秦具窍绰单, 矫埃惑 备泅捞 塞甸绢 快急 捞犯霸 阜酒初澜 - 20160728
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

	//// 其绢府 救焊捞档废.
	//FLobbySceneManager* LobbySM = GetSceneManager();
	//if (!bVisibleUI)
	//{
	//	LobbySM->OnLobbyFlyFairy_AllHidden();
	//}
}

void FLobbyMainScene::SwipeScene(float Delta)
{
	//ALevelSequenceActor * DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee && DefaultCameraMatinee->IsValidObj()) // 饭骇 胶飘府怪苞 爵囚辑 Invalid 茄 版快啊 乐绰 芭 鞍促..
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

	//// 窍困 努贰胶俊辑 UpdateUIPosition 阑 龋免窍档废..
}

float FLobbyMainScene::GetDefaultLobbyMatineeTimeStamp()
{
	float EventKeyTime = 0.0f;
	//ALevelSequenceActor * DefaultCameraMatinee = GetDefaultCameraMatinee();

	//if (DefaultCameraMatinee && DefaultCameraMatinee->IsValidObj()) // 饭骇 胶飘府怪苞 爵囚辑 Invalid 茄 版快啊 乐绰 芭 鞍促..
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

	//// 霉 肺厚 柳涝锭绰 寇何俊辑 贸府
	//if (!GetSceneManager()->IsPlayingBeginAnim()) {
	//	PlaySelectMatinee();
	//}
	//else{
	//	// -> 肺厚 柳涝 饶 免籍 芒 躲锭鳖瘤 瘤楷捞 缴且 锭 焊捞绰 厘搁捞 逞 还骡秦辑 老窜 构扼档 焊咯淋.
	//	PlaySelectMatinee(
	//		false, // 措帆 Begin 矫痢捞变 茄单 咯扁辑 盔窍绰 Begin 篮 免籍芒 摧篮 流饶扼 false 肺 逞败林绢具
	//		true // Closeup 苞沥阑 焊咯林霸 登搁 弊霸 免籍芒 摧绊 唱辑 犁帕登绰 蠢肠捞 瞪 荐 乐栏聪 true 肺 逞败林绢辑 官肺 closeup 轰 流青
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
	//	// CharacterObserve UI 甫 凯扁 傈俊 捞霸 技泼捞 登绢 乐绢具 窃.
	//	// 扁粮狼 辑宏 努贰胶客 汲沥窍妨绰 皋牢 努贰胶啊 鞍促搁 皋牢->辑宏
	//	if (DocBattle->GetTagPCClass() == PCClassToInt(CurrentPCClass))
	//	{
	//		DocBattle->SetTagPCClass(DocBattle->GetSavePCClass());
	//	}

	//	DocBattle->SetCurPCClass(PCClassToInt(CurrentPCClass));
	//	DocSome->SetIsCharacterObserveScene(true);
	//	
	//	// 180104_YJ HeroMgmt甫 涝厘且锭 扁夯蔼栏肺 焊咯瘤绰 某腐磐绰 肺厚 可历滚 某腐磐捞骨肺, 
	//	// 可历滚 某腐磐啊 函版瞪锭 窃膊 函版矫糯
	//	if (CahsedSceneManager)
	//		CahsedSceneManager->SetHeroMgmtModePCClass(CurrentPCClass);
	//}
	//
	//// 葛滴 力芭窍扁肺 窃.
	//DestroyAllLobbyCharOnHeadDisplayClass<>::GetInstance().Signal();
	//// 泅犁 努肺令诀茄 某腐磐 沥焊父 剁快绰 霸 促矫 鞘夸秦瘤搁 FLobbySceneManager::UpdateLobbyOnHeadDisplayWidget 阑 荤侩. UB2FloatingLobbyOnHeadDisplay 率俊辑档 见变 芭 汗备


	////喉风橇赴飘俊辑 技泼窍绰 巴栏肺 函版抗沥 - LVCH_Lobby 眉农酒眶巩力肺 窍靛内爹..
	//if (LobbyGameMode)
	//{
	//	//180220_YJ 拌加 硅凯捞 阶捞带 泅惑捞 乐菌澜.
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

	//	// 其绢府 焊捞档废.
	//	EFairyType ViewFairyType = FindFlyFairyTypeByOpenStage();
	//	if(ViewFairyType != EFairyType::FAIRY_NONE || ViewFairyType != EFairyType::FAIRY_MAX)
	//		LobbySM->OnLobbyFlyFairy_Visible(ViewFairyType);
	//}
}

EFairyType FLobbyCharObserveScene::FindFlyFairyTypeByOpenStage()
{
	// 泅犁 坷锹登绢乐绰 其绢府 吝俊 罚待栏肺 窍唱 急琶.
	EFairyType ViewFairyType = EFairyType::FAIRY_NONE; // NONE 捞扼搁 酒公巴档 其绢府啊 坷锹登绢乐瘤 臼澜.

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
	//// 府家胶啊 某教登瘤 臼篮 版快俊 措秦 碍力肺 府某教 矫弊澄阑 朝覆.. 啊阐 巩力啊 焊咯辑..
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
	//	// CharacterObserve UI 甫 凯扁 傈俊 捞霸 技泼捞 登绢 乐绢具 窃.
	//	DocBattle->SetCurPCClass(PCClassToInt(CurrentPCClass));
	//	DocSome->SetIsCharacterObserveScene(true);

	//	// 180104_YJ HeroMgmt甫 涝厘且锭 扁夯蔼栏肺 焊咯瘤绰 某腐磐绰 肺厚 可历滚 某腐磐捞骨肺, 
	//	// 可历滚 某腐磐啊 函版瞪锭 窃膊 函版矫糯
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
	//				{// 捞 仇篮 官蹿捞促.
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
	//				if (DesireCamDist - DefaultCamDist <= 0.f) // Default焊促 救率俊 Camera啊 困摹(淋牢惑怕)扼搁
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
	//SetScreenState(false); //扁夯 啊肺胶农赴

	//CachedViewTargetActor->SetCloseUpState(true, 
	//	!bPromptToClosedUpView, // CloseUpView 肺 官肺 流青窍绰 版快扼搁 急琶 Anim 敲饭捞甫 窍瘤 臼绰促.
	//	IsBegin); // 霸烙 立加窍绊 盖 贸澜.. 捞吧 滴 锅 鸥霸 登聪 碍力肺 促矫 state 技泼阑 芭摹档废.

	//BII_CHECK(Scene->GetSceneManager());

	//ALevelSequenceActor * PlayMatinee = IsBegin ? SelectedCharacterInfo.GameBeginingMatinee : SelectedCharacterInfo.SelectingMatinee;
	//PlayCharacterMatinee(PlayMatinee, SelectedCharacterInfo.LevelActor);

	//if (bPromptToClosedUpView)
	//{
	//	// PlayCharacterMatinee 俊辑绰 矫累 困摹俊 初疽阑 巴. 官肺 场 困摹肺 皑.
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
	//if (GEngine->ActiveMatinee.IsValid() && GEngine->ActiveMatinee.Get()->GetWorld())//付萍匙啊 部捞绊乐嚼聪促... 倒绊乐绰 付萍匙 公炼扒 肛苗林绊 倒妨凛矫促.
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

	//// 朝俺柳拳楷免俊辑 MatineeForWingEventScene 狼 Stop notification 牢 版快 楷免 辆丰 贸府.
	//// MATINEE_DONE_STAY_ACTOR_CUSTOM_EVENT 甫 罐酒具 且 荐档 乐促..
	//if (bCurrentlyWingEvolutionScene && OptionalEventName == MATINEE_STOP_ACTOR_CUSTOM_EVENT && CastedNotifyingMatinee && CastedNotifyingMatinee->IsValidObj())
	//{
	//	TMap<EPCClass, FLobbyCharacterInfo>& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();
	//	FLobbyCharacterInfo* EvolvedCharInfo = AllCharactersOfDesiredType.Find(WingEvolutionScenePCClass);
	//	if (EvolvedCharInfo && EvolvedCharInfo->MatineeForWingEventScene == CastedNotifyingMatinee)
	//	{
	//		// 官肺 场郴瘤 臼绊 搬苞 UI 甫 焊咯霖 促澜 EndWingEvolutionScene 绰 搬苞 UI 俊辑 犬牢滚瓢 穿福搁 阂府档废.
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
	//		//某腐磐狼 困摹甫 嘎眠扁 困窃.
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
	//	WingEvolutionScenePCClass = InEvolvedPCClass; // 愧父茄 柳拳 惑炔俊辑扼搁 CurrentPCClass 客 鞍酒具 茄促. 窜, 趣矫扼档 匙飘亏 掉饭捞肺 牢秦 览翠捞 词绊 弊 货 某腐磐啊 官拆 啊瓷己阑 积阿秦辑..
	//}
	//else{
	//	WingEvolutionScenePCClass = CurrentPCClass;
	//}

	//if (WingEvolutionScenePCClass != EPCClass::EPC_End)
	//{		
	//	bCurrentlyWingEvolutionScene = true;
	//	
	//	if (GetSceneManager())
	//	{ // 荤侩磊啊 朝俺甫 见板歹扼档 咯扁急 焊咯林磊.
	//		GetSceneManager()->SetUnconditionalShowWing(true, true, WingEvolutionScenePCClass);
	//	}

	//	// 柳拳 楷免 贸澜俊绰 朝俺啊 见败廉 乐绰单 (局聪皋捞记 磊眉啊 弊烦) 鉴埃利栏肺 朝俺啊 焊捞绰 霸 脾绢辑.. 趣矫 捞犯霸扼档 窍搁 炼陛捞唱付 档框捞 瞪鳖 酵澜
	//	FlushRenderingCommands(); // 辟单 捞芭尔 惑包绝绰 淀..

	//	TMap<EPCClass, FLobbyCharacterInfo>& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();
	//	
	//	for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(AllCharactersOfDesiredType); It; ++It)
	//	{ // 老窜 促 见败初绊
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
	//				// 咯变 局聪皋捞记阑 付萍聪 敲饭捞肺 备悼矫懦 巴捞骨肺 泪矫 官层狄..
	//				EvolvedCharInfo->ActorForMain->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//			}
	//		}

	//		if (EvolvedCharInfo->MatineeForInven){
	//			EvolvedCharInfo->MatineeForInven->Stop(); // 康旷包府 皋牢 付萍聪客 般摹瘤 臼霸 沥瘤矫糯.
	//		}
	//		if (EvolvedCharInfo->MatineeForWingEventScene){
	//			EvolvedCharInfo->MatineeForWingEventScene->Stop();

	//			EvolvedCharInfo->MatineeForWingEventScene->SetPosition(0.0f);
	//			EvolvedCharInfo->MatineeForWingEventScene->AddCustomNotifyActor(GetLobbyGameMode()); // Matinee Stop Notification 阑 罐扁 困秦 GameMode 甫 殿废秦 持澜.
	//			EvolvedCharInfo->MatineeForWingEventScene->Play();
	//		}
	//	}

	//	// 楷免 侩 UI 其捞瘤肺. 弊促瘤 焊咯林绰 芭 绝绰 芭狼 后 拳搁老 淀..
	//	DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_WingEvolutionProgress);
	//}
}

void FLobbyHeroMgmtScene::OnWingEvolutionMatineeEnd()
{
	//// 搬苞 UI 焊咯林绊..
	//DJLegacy_ChangeLobbyUIPageClass<ELobbyUIPages>::GetInstance().Signal(ELobbyUIPages::ELUP_WingEvolutionResult);

	//// 酒付档 付萍聪 付瘤阜 墨皋扼 厘搁俊辑 Idle 敲饭捞甫 夸备且 芭 鞍篮单.. 弊霸 绢妨快搁 乞家 康旷包府 拳搁俊辑狼 墨皋扼扼档..

	//TMap<EPCClass, FLobbyCharacterInfo>& AllCharactersOfDesiredType = GetLobbyCharactersForGTypeLevel();
	//FLobbyCharacterInfo* EvolvedCharInfo = AllCharactersOfDesiredType.Find(WingEvolutionScenePCClass);
	//if (EvolvedCharInfo)
	//{
	//	if (EvolvedCharInfo->MatineeForWingEventScene)
	//	{
	//		// MATINEE_STOP_ACTOR_CUSTOM_EVENT 甫 罐绊 搬苞 UI 啊 唱坷绰 惑炔捞扼搁 咯扁辑 Matinee Stop 阑 秦辑绰 救登绊
	//		// MATINEE_DONE_STAY_ACTOR_CUSTOM_EVENT 甫 罐绊 搬苞 UI 啊 唱坷绰 惑炔捞扼搁 咯扁辑 Matinee Stop 阑 茄促.
	//		//EvolvedCharInfo->MatineeForWingEventScene->Stop();
	//		EvolvedCharInfo->MatineeForWingEventScene->SetPosition(0.0f);

	//		// 朝俺 楷免 场抄 官肺 弊 墨皋扼 矫痢栏肺..
	//		const float FinalKeyTime = EvolvedCharInfo->MatineeForWingEventScene->MatineeData ? // MatineeData 啊 绝栏搁 富捞 救登瘤父..
	//			EvolvedCharInfo->MatineeForWingEventScene->MatineeData->InterpLength : 0.0f;
	//		EvolvedCharInfo->MatineeForWingEventScene->Play();
	//		SetCameraMatineeCam(EvolvedCharInfo->MatineeForWingEventScene, FinalKeyTime);
	//		EvolvedCharInfo->MatineeForWingEventScene->SetPosition(FinalKeyTime);
	//	}
	//}
	//// 墨皋扼绰 场抄 矫痢俊.. 局聪皋捞记篮 蝶肺 Idle
	//for (TMap<EPCClass, FLobbyCharacterInfo>::TIterator It(AllCharactersOfDesiredType); It; ++It)
	//{
	//	AB2HeroMgntSkeletalMeshActor* ThisSKActor = It.Value().ActorForMain;
	//	if (ThisSKActor && ThisSKActor->GetSkeletalMeshComponent()){
	//		// 趣 某腐磐 急琶捞 绢边车阑瘤档 葛福聪 葛滴 AnimBP 葛靛肺 犬角窍霸 官层淋.
	//		ThisSKActor->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//	}
	//}
}

void FLobbyHeroMgmtScene::EndWingEvolutionScene()
{
//	if (GetSceneManager())
//	{ // 荤侩磊啊 朝俺甫 见板促搁 促矫 见变促.
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
//		WingAirport->MarkPreEvolveDataInvalid(); // 柳拳 搬苞芒俊辑 结冈篮 捞饶俊绰 鞘夸绝促.
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

	//if (GEngine->ActiveMatinee.IsValid() && GEngine->ActiveMatinee.Get()->GetWorld())//付萍匙啊 部捞绊乐嚼聪促... 倒绊乐绰 付萍匙 公炼扒 肛苗林绊 倒妨凛矫促.
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
	//		//康旷包府俊辑 静捞绰 Actor扼辑 檬扁 困摹肺 倒酒啊具父 茄促.
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
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_StageInfo, false, true);// Sub 某腐磐狼 Sound notify 甫 馋促.
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
	//if (MainViewMatinee && MainViewMatinee->IsValidObj()) // 饭骇 胶飘府怪苞 爵囚辑 Invalid 茄 版快啊 乐绰 芭 鞍促..
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
	// 付瘤阜 涝仿 捞饶 老沥 矫埃 儒弗 饶 磊悼 胶农费.. 酒丛 Free swipe 捞 瘤唱绊 唱搁..?
	if (ElapsedLastInput > 2.f && !bAcceleratingSwipe) 
	{
		if (MatineePos <= MatineeAutoScrollLeftEndPosition)
			bAutoMoveDirection = true;
		if (MatineePos >= MatineeAutoScrollRightEndPosition)
			bAutoMoveDirection = false;

		MoveCameraDirect(MatineeLength * DeltaSeconds * (bAutoMoveDirection ? SpeedAutoScrolling : -SpeedAutoScrolling),
			true // 咯扁辑 MoveCameraDirect 妮 且 锭俊绰 FreeSwipe 栏肺 柳青且 咯瘤啊 惯积窍瘤 臼档废 窃.
		);
		// 眠啊肺 捞 惑炔俊辑绰 history 档 巢瘤 臼霸..
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
	// FreeSwipe 阑 困茄 炼扒
	check(!bMoveCameraDirectCalledThisFrame && bAcceleratingSwipe);
	FLobbySceneManager* SceneMgr = GetSceneManager();
	check(SceneMgr);

	if (bBeginAcceleratingSwipe) // Free swipe 阑 贸澜 矫累且 锭父.
	{
		SpeedSwipe = FreeSwipeStartingSpeed;
		bBeginAcceleratingSwipe = false;
		DirectSwipeSpeedHistory.Empty(); // 矫累茄 捞惑 history 甫 朝覆.
	}

	if (MatineePos == 0.f || MatineePos == MatineeLength)
	{ // Free swipe 档吝 剧 场俊 档崔沁阑 锭牢单.. 加档客 啊加档 规氢阑 馆傈矫虐绰单.. 距埃篮 皑艰甫 林绊..
		SpeedSwipe *= -0.8f;
		SwipeAcceleration *= -0.8f;
	}

	if (SceneMgr)
	{
		// 殿啊加档啊 瞪 荐档 乐绰单 加档 父怒狼 历亲仿阑 馆康窍绰 啊加档甫 魂免秦 夯促..
		SwipeAcceleration = -1.0f * SpeedSwipe * SceneMgr->GetWorldMapSwipeDeaccelerationFactor();

		const float PrevSpeedSwipe = SpeedSwipe;
		SpeedSwipe += (SwipeAcceleration * DeltaSeconds); // dv = at

		// 皑加栏肺 牢秦 加仿捞 临芭唱 加档啊 馆措肺 登绰 惑炔捞搁 弊父 吝窜
		if (FMath::Abs(SpeedSwipe) < SceneMgr->GetWorldMapSpeedTriggeringAcceleratingSwipe() || SpeedSwipe * PrevSpeedSwipe < 0)
		{
			SpeedSwipe = 0.f;
			bAcceleratingSwipe = false;
		}

		// AcceleratingSwipeInitialSpeed 捞芭 开且阑 犬角洒 葛福摆澜. 购啊 魄喊窍绰 扁霖栏肺 静变 秦辑 出狄.
		AcceleratingSwipeInitialSpeed = AcceleratingSwipeInitialSpeed - SwipeAcceleration * DeltaSeconds;

		//UE_LOG(LogBladeII, Log, TEXT("ChapterTick bAccel %d, Speed %.4f -> %.4f, Acc %.4f, ElapedLastInput %.2f, MatineePos %.2f"),
		//	(int32)bAcceleratingSwipe, PrevSpeedSwipe, SpeedSwipe, SwipeAcceleration, ElapsedLastInput, MatineePos);
	}
}

// 沥秦柳 加档肺 框流老 锭 敬促. 涝仿栏肺 胶农费茄促芭唱 磊悼 胶农费 矫
void FLobbyBattleSelectSceneBase::MoveCameraDirect(float Value, bool bNoCheckForFreeSwipe)
{
	FLobbySceneManager* SceneMgr = GetSceneManager();
	check(SceneMgr);
	if (SceneMgr)
	{
		// 荤侩磊 涝仿栏肺 牢茄 MoveCameraDirect 啊 付瘤阜栏肺 龋免等 矫痢俊 沥秦柳 SpeedSwipe 蔼栏肺 皑加阑 窍搁辑 free swipe 阑 窍绰扒单
		// 漂洒 葛官老 涝仿券版俊辑 付瘤阜俊 累篮 蔼捞 唱坷帮 秦辑 free 胶农费捞 坷贰 给啊芭唱 酒抗 矫累阑 力措肺 给且 锭啊 乐澜
		// 蝶扼辑 付瘤阜 割 锅狼 乞闭阑 郴 夯促..
		// 磐摹肺 流立 swipe 矫俊档 家沥狼 乞闭阑 辰促.

		DirectSwipeSpeedHistory.Add(Value);
		if (DirectSwipeSpeedHistory.Num() > SceneMgr->GetWorldMapDirectSwipeSpeedHistoryNum())
		{
			DirectSwipeSpeedHistory.RemoveAt(0);
		}
		
		// AcceleratingSwipeInitialSpeed 捞芭 开且阑 犬角洒 葛福摆澜. 购啊 魄喊窍绰 扁霖栏肺 静变 秦辑 出狄.
		AcceleratingSwipeInitialSpeed = Value;
		// 扁夯利牢 SpeedSwipe 篮 老沥 基敲阑 啊瘤绊 乞闭辰 蔼栏肺
		SpeedSwipe = GetAvgDirectSwipeSpeed(SceneMgr->GetWorldMapDirectSwipeSpeedFilterNum());

		if (DirectSwipeSpeedHistory.Num() > 0)
		{ 			
			float TotalAvgSwipeSpeed = GetAvgDirectSwipeSpeed(); 

			// @.@
			if (!bNoCheckForFreeSwipe) // 埃趣 狼档客 促弗 惑炔俊辑狼 MoveCameraDirect 妮捞 AcceleratingSwipe 阑 惯积矫虐绰 淀..
			{
				bAcceleratingSwipe = (
					(SceneMgr->GetWorldMapSwipeDeaccelerationFactor() != 0.f && FMath::Abs(TotalAvgSwipeSpeed) > SceneMgr->GetWorldMapSpeedTriggeringAcceleratingSwipe())
					||
					(FMath::Abs(AcceleratingSwipeInitialSpeed * 0.4f + TotalAvgSwipeSpeed * 0.6f) > SceneMgr->GetWorldMapSpeedTriggeringAcceleratingSwipe())
					);
			}

			if (bAcceleratingSwipe)
			{
				// Free swipe 阑 窍霸 瞪 芭 鞍栏搁 矫累 加档绰 荐笼茄 傈眉 基敲 乞闭阑 啊瘤绊..
				FreeSwipeStartingSpeed = TotalAvgSwipeSpeed;
				bBeginAcceleratingSwipe = true;

				//UE_LOG(LogBladeII, Log, TEXT("MoveCameraDirect About to begin FreeSwip with Avgspeed %.4f from %d samples"),
				//	FreeSwipeStartingSpeed, DirectSwipeSpeedHistory.Num());

				// 角力肺绰 促澜 平俊档 MoveCameraDirect 啊 阂府瘤 臼酒具 Free swipe 阑 窍霸 瞪 巴.
			}			
		}		
		bMoveCameraDirectCalledThisFrame = true;
	}

}

float FLobbyBattleSelectSceneBase::GetAvgDirectSwipeSpeed(int32 InMaxHistoryNum, bool bRespectTheLastSign) const
{
	const int32 FinalSampleNum = InMaxHistoryNum < 0 ? DirectSwipeSpeedHistory.Num() : FMath::Min(InMaxHistoryNum, DirectSwipeSpeedHistory.Num());
	if (FinalSampleNum <= 0) { // 葛牢 基敲捞 绝绰 惑炔老 淀.
		return 0.0f;
	}

	// 第率老荐废 弥脚 基敲肺 埃林窍绊 第俊辑何磐..
	const int32 AvgFirstIndex = DirectSwipeSpeedHistory.Num() - 1;
	const int32 AvgLastIndex = DirectSwipeSpeedHistory.Num() - FinalSampleNum;
	check(AvgLastIndex >= 0);
	float RetAvg = 0.0f;
	for (int32 SI = AvgFirstIndex; SI >= AvgLastIndex; --SI)
	{
		// bRespectTheLastSign 牢 版快 例措蔼栏肺 乞闭郴辑 何龋绰 付瘤阜 基敲 吧肺促啊 荤侩.
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
//	if (MainViewMatinee && MainViewMatinee->IsValidObj() && MainViewMatinee->GetWorld()) // 饭骇 胶飘府怪苞 爵囚辑 Invalid 茄 版快啊 乐绰 芭 鞍促..
//	{
//		MatineePos = FMath::Clamp<float>(MatineePos + Delta, 0.f, MatineeLength);
////#if UE_BUILD_DEBUG // 酒付档 葛氰甘 磊悼 胶农费狼 康氢牢瘤 啊父洒 乐绢档 肺弊啊 腹捞 唱客辑..
////		UE_LOG(LogStreaming, Warning, TEXT("MatineePos ====== '%f' "), MatineePos);
////#endif
//		MainViewMatinee->Play();
//		MainViewMatinee->SetPosition(MatineePos);
//		MainViewMatinee->Pause();
//
//		SetCameraMatineeCam(MainViewMatinee, MatineePos);
//	}

	// 窍困 努贰胶俊辑 UpdateUIPosition 阑 龋免窍档废..
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
		// UMG 俊辑 镜 UIPosition 拌魂 矫俊 泅犁 秦惑档啊 酒囱 扁霖 秦惑档 扁霖栏肺 持绢林绢具 窍骨肺 轰 胶纳老捞 鞘夸窃.
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
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // 饭骇 胶飘府怪捞 货肺 登瘤 臼歹扼档 概 茅磐付促 Matinee 绰 货肺 技泼秦具 窍骨肺 碍力 update
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
	//{ // 夸 匙捞怪俊 何钦窍绰 局甸父 UI 器瘤记 技泼 付目肺 埃林.
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
	//				InterpGroup->GetGroupActor()->SetActorHiddenInGame(true);	//甘俊辑 弊 困摹 钮宏 救焊捞霸 窍绰 备埃.

	//				for (int32 idx = 0; idx < InterpGroup->Group->InterpTracks.Num(); ++idx)
	//				{
	//					UInterpTrackMove* MoveTrack = Cast<UInterpTrackMove>(InterpGroup->Group->InterpTracks[idx]);
	//					if (MoveTrack && !MoveTrack->IsDisabled()) // MoveTrack 捞 镜单绝捞 咯矾俺 殿废等 版快啊 乐澜. 窍唱父 吧矾郴具..
	//					{
	//						CachedUIActorLocation.Add(MoveTrack->EvalPositionAtTime(InterpGroup->TrackInst[idx], 0.f));
	//						break;
	//					}
	//				}
	//			}					
	//		}
	//	}		
	//}

	//// 咯扁 眉农俊 吧府搁 狼档客 促福霸 技泼茄 付萍聪 飘发捞 乐促芭唱 茄 版快. 钧蹲窍霸 匙捞怪俊 吧啡促芭唱..?
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

	// 酒贰 捞抚篮 Matinee 弊缝 捞抚栏肺 荤侩窍霸 瞪 芭.
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
	//	LobbyGM->UpdateNativeLobbyMatineeRef(); // Bundle_A 客 悼老茄 胶飘府怪 饭骇俊 技泼捞 登菌促搁 捞 矫痢俊 利例茄 蔼阑 甸绊 乐瘤 臼阑 巴捞聪 茄锅 歹 诀单捞飘.
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

	UpdateUIPosition(); // UI Position 捞 混娄 部老 锭啊 乐绢辑 搬惫 tick 俊辑 概锅 贸府.. ぱ.ぱ
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
		//	{ // 捞锅 Elem 狼 沥秦柳 弊缝 捞抚苞 老摹窍绰 弊缝.
		//		if (ThisGroupInst->GetGroupActor())
		//		{
		//			UIPositionActors[MEI] = ThisGroupInst->GetGroupActor();
		//			ThisGroupInst->GetGroupActor()->SetActorHiddenInGame(true); // UI 困摹 付欧 开且老 挥. 见败初澜.

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
//		// 2D 困摹甸 掘绢客辑 弥辆利栏肺 UIMod 率栏肺 持绢淋.
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

	////烙矫 Todo : Doc delegate肺 哗具凳
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
	////快急 DocBattle俊 淬绰单, 单捞磐啊 腹酒瘤搁 Doc甫 货肺 父靛绰 巴栏肺..
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
	//	//todo UI凯扁
	//	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::Mail);

	//	//todo Category喊
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
//	//磊 沥纺钦矫促 臭篮鉴辑何磐
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
//		// 饭捞靛 檬措矫俊 辨靛盔 罐绰巴 锭巩俊.. 饭捞靛绰 盒府秦滴菌绰单,
//		// 促弗 菩澄率俊档 GuildDetailInfo 阑 荤侩茄促搁 辑滚率俊 橇肺配妮 盒府 夸没 趣篮 抗寇贸府甫 脚吝洒 秦具且淀..
//	}
//	else if (UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting))
//	{
//		// 盲泼 UI啊 畴免 吝老版快 公矫.
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

	//if(GuildDoc->GetGuildOutState())			//辨靛俊辑 眠规寸沁阑版快 扑诀阑 剁况林磊~~
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

	//// UI Init() 捞 阂赴 饶俊 OpenScene捞 登骨肺 贸澜 柳涝 茄锅篮 肺厚某腐磐 悸泼阑 秦 林绢具 窃
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
	//// 捞傈 Character绰 见辫贸府 饶 矫累
	//auto& LobbyCharactersForGLobbyType = GetLobbyCharactersForGTypeLevel();
	//auto* LobbyCharInfo = LobbyCharactersForGLobbyType.Find(CurrentSelectedCharacter);
	//if (LobbyCharInfo && LobbyCharInfo->ActorForMain)
	//{
	//	CharacterHiddenAndStopSound(LobbyCharInfo->ActorForMain, true);
	//}

	//// 货肺款 Character Visible 贸府 矫累
	//LobbyCharInfo = LobbyCharactersForGLobbyType.Find(SelectedCharacter);
	//if (LobbyCharInfo)
	//{
	//	CurrentSelectedCharacter = SelectedCharacter;
	//	CurrentCharacter = LobbyCharInfo->ActorForMain;

	//	ALevelSequenceActor * CollectBookMatinee = nullptr;
	//	float EventKeyTime = 0.f; //档皑侩 Matinee EventKeyTime 蝶肺 绝澜
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
	//{ // 啊阐究.. 绢叼急啊 脚龋 焊辰 霸 第词霸 坷扁扼档 窍绰瘤 矫埃 瘤唱绊 唱辑 墨皋扼啊 茸 官差绰 版快啊 乐绢辑 粱 还窿.
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
	//{ // LobbySceneManager 狼 ShopGirlReservedState 阑 漓轿霸 某教秦 初绊 府悸
	//	CachedShopGirlReservedState = LobbySM->GetShopGirlReservedState();

	//	// OpenScene 阑 烹秦 甸绢棵 锭 Cheer 啊 酒聪搁 贸澜俊绰 券康 局聪皋捞记捞 登绢具 窃.
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
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // 救傈户.

	//	// 惑痢 家赤档 磐摹 咀记捞 乐绢辑 涝仿 官牢爹
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
	//// 硅摹秦 初篮 惑痢 家赤 咀磐
	//AB2LobbyShopGirl* ShopGirActor = LobbySM ? Cast<AB2LobbyShopGirl>(LobbySM->GetNPCSKActor(ELobbyNPCType::ELobbyNPC_ShopGirl)) : nullptr;
	//if (ShopGirActor && CachedShopGirlReservedState != ELobbyShopGirlState::ELSG_End)
	//{	
	//	if (CachedShopGirlReservedState == ELobbyShopGirlState::ELSG_IdleLoop)
	//	{ // 惶扁 搬苞俊 措茄 兢啊 酒囱 促澜俊具 Welcoming 捞 瞪 抛聪 咯扁肺 甸绢棵 老捞 乐瘤绰 臼摆促.
	//		ShopGirActor->BeginDefaultPose(); // Idle 捞搁 PlayTemporary 啊 酒聪绊 风俏栏肺 拌加 倒妨具 登绰 芭.
	//	}
	//	else
	//	{ // 碱鸥烙
	//		ShopGirActor->PlayTemporaryAnimOfState(CachedShopGirlReservedState);
	//	}
	//}

	//if (LobbySM && !LobbySM->IsSceneStreamingLevelLoaded(ELobbyScene::ELobbyScene_SummonItemDisplay))
	//{ // 惶扁 厘搁阑 固府 肺爹秦 滴绰单 flush 肺 牢茄 何累侩 啊瓷己阑 撤眠扁 困秦 AnimStateGuard 甫 敌促. 
	//	// 捞吧 救 窍搁 漂洒唱 磐摹 矫 传俊 剁绰 橇府隆捞 惯积且 荐 乐促.
	//	// 弊府绊 贸澜 老沥 矫埃篮 Welcoming 葛记捞 犬角窍霸 焊捞档废 窍绰 狼固档 瞪 荐 乐绊.
	//	ShopGirActor->SetAnimStateGuard(1.5f);
	//	LobbySM->PrefetchSceneStreamingLevel(ELobbyScene::ELobbyScene_SummonItemDisplay);
	//}
}
void FLobbySummonItemScene::CloseScene()
{
	//CachedItemSceneMatinee = nullptr;

	//// 惑痢 家赤档 磐摹 咀记捞 乐绢辑 涝仿 官牢爹
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
	////墨靛 第笼阑锭绰 静绰 窃荐
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
	//if (GEngine->ActiveMatinee.IsValid() && GEngine->ActiveMatinee.Get()->GetWorld())	//付萍匙啊 部捞绊乐嚼聪促... 倒绊乐绰 付萍匙 公炼扒 肛苗林绊 倒妨凛矫促.
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
	////牢亥俊 持扁
	//auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	//FLobbySceneManager* LobbySM = GetSceneManager();
	//if (LobbySM)
	//{
	//	LobbySM->SetShopGirlReservedState(ELobbyShopGirlState::ELSG_CheerNormal); // 给秦档 扁夯户栏肺 窍唱绰 焊咯霖促
	//}
	//for (auto& SvrItem : SummonResult->items)
	//{
	//	int32 AddedIndex = CurrentSummonedItems.AddedItems.AddDefaulted();
	//	check(AddedIndex != INDEX_NONE);

	//	FB2Item& CurrAddedRef = CurrentSummonedItems.AddedItems[AddedIndex];
	//	CurrAddedRef = SvrItem;
	//	if (CurrAddedRef.StarGrade >= ITEM_SUMMON_JACKPOT_GRADE_LV1 && LobbySM)
	//	{
	//		LobbySM->SetShopGirlReservedState(ELobbyShopGirlState::ELSG_CheerMuch); // 冀矫错胶 抗距.
	//	}
	//}

	//ClientDataStore.AddNewAcquiredItems(CurrentSummonedItems.AddedItems);

	//auto StoreDoc = UB2UIDocHelper::GetDocStore();

	///*UB2UISummonItem* SummonItemUI = UB2UIManager::GetInstance()->GetUI<UB2UISummonItem>(UIFName::SummonItemStore);
	//BII_CHECK(SummonItemUI);*/
	//if (!StoreDoc)
	//	return;

	//// 快沥 器牢飘肺 惶扁
	//if (SummonResult->social_point != ClientDataStore.GetSocialPoint())
	//{
	//	ClientDataStore.UseSocialPoint();
	//}

	////付老府瘤 棺 犁拳殿 Update
	//if (SummonResult->free_lottery) //[@AKI, 171124] SummonResult->free_lottery == true老锭绰 辑滚俊辑  DB炼雀甫 且 鞘夸啊 绝扁 锭巩俊 next_free_time力寇窍绊绰 蔼 盎脚阑 救窃.弊贰辑 努扼档 弊客 悼老窍霸 备泅窃. ex> gem = 0, social_point = 0
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

	//// 茄锅 歹 惶扁甫 困茄 八刘等 虐
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

	//牢亥俊 持扁
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



	//	//搬苞芒 PowrIndex 鉴栏肺 沥纺窍磊
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

	//	// 1. 檬措窍扁
	//	if (UIDocRaid->GetIsReceiveInvite()) // 檬措窍扁肺 涝厘.
	//	{
	//		JoinType = ERaidJoinType::MakeRoom;
	//		RaidUI->ForcedStartGame(RaidType, RaidStep, JoinType, PcClass, false);
	//	}
	//	// 2. 磊悼 概莫
	//	else if(IsRematch)
	//	{
	//		RaidUI->ForcedStartGame(RaidType, RaidStep, JoinType, PcClass, true);

	//	}
	//	// 3. 老馆利牢 涝厘.
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

	//	// 评盔甸苞 促矫窍扁 穿弗霸 酒聪搁 Raid MatchMaking 锭 罐篮 沥焊甸篮 公矫茄促.
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
	//	// 饭捞靛 促矫窍扁 - 捞傈霸烙 ChannelEndpoint 逞败林搁 鞍篮 评盔甸苞 冯 炼牢
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
	//	// 辫篮枚 : 2018-08-24 ((UPDATE+3) 饭捞靛 俺急 - 去磊 窍扁/促矫窍扁) B2RaidRoomType REJOIN 眠啊
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
	// 捞锭绰 UI甫 悸泼秦林瘤 富磊.
}

void FLobbyRaidMainScene::SetRemotePlayerClass(uint32 NetId, EPCClass MainPCClass, const FB2FindAccountInfoPtr& Info)
{
	AddUIData(NetId, MainPCClass, FB2ModPlayerInfo(Info), Info->guild_id);
}

void FLobbyRaidMainScene::LeaveRoomIfJoined()
{
	RemoveSlotIndex();

	// 炼牢芒捞 凯妨乐阑荐乐栏聪 摧酒霖促.
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
	//// RaidLevel 函版
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

	//	// roomLocation 汲沥 饶
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

	//// Raid UI 盎脚阑 困秦 沥焊甫 持绢霖促.
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
	// 扁粮俊 利犁登绢乐绰瘤 犬牢
	for (int i = 0; i < SlotIndexs.Num(); ++i)
	{
		if (SlotIndexs[i] == NetId)
			return i;
	}

	return -1;
}

bool FLobbyRaidMainScene::IsExist(const int32 NetId) const
{
	// 扁粮俊 利犁登绢乐绰瘤 犬牢
	for (int i = 0; i < SlotIndexs.Num(); ++i)
	{
		if (SlotIndexs[i] == NetId)
			return true;
	}

	return false;
}

int32 FLobbyRaidMainScene::GetNextInsertSlotIndex(const int32 NetId)
{
	// 扁粮俊 利犁登绢乐绰瘤 犬牢
	int32 ExistSlotIdx = GetSlotIndex(NetId);
	if (ExistSlotIdx != -1)
		return ExistSlotIdx;

	// 救登绢乐栏搁 霉锅掳肺 厚绢乐绰 浇吩
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
	{ // 溜籍 皋浆 诀单捞飘. 诀单捞飘 窍搁辑 bUnconditionalShowWing 阑 曼炼窍霸 瞪 芭.
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
	ShopGirlReservedState = InStateToReserve; // 芭狼 捞扒 技泼 饶 坷贰 瘤唱瘤 臼酒 官肺 结冈绊 府悸登绢具 且 己拜烙.
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
	//		//康旷包府俊辑 静捞绰 Actor扼辑 檬扁 困摹肺 倒酒啊具父 茄促.
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
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_CharacterPage, false, true);// Sub 某腐磐狼 Sound notify 甫 馋促.
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
	{ // 某腐磐 皋浆 炼钦 矫 府葛飘 某腐磐 倾侩.
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
	//{ // 某腐磐 皋浆 炼钦 矫 府葛飘 某腐磐 沥焊 荤侩 陛瘤. 捞吧 救 且 矫 肋给等 沥焊 立辟栏肺 农贰浆 困氰捞 乐澜.
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
	//		//某腐磐狼 困摹甫 嘎眠扁 困窃.
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
	//		//康旷包府俊辑 静捞绰 Actor扼辑 檬扁 困摹肺 倒酒啊具父 茄促.
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
	// 蜡历 唱哎锭档 蜡历墨款泼UI 盎脚
	RefreshFindMatchPopup();
}

void FLobbyControlCharacterSelectPage::SetMyPlayerInfoData(uint32 NetId, const FB2FindAccountInfoPtr& info)
{
	//GetLobbyGameMode()->SetPeerTeam();
}

void FLobbyControlCharacterSelectPage::SetRemotePlayerInfoData(uint32, const FB2FindAccountInfoPtr&)
{
	// 龋胶飘. 评沥焊 盎脚. 舅覆.
	//GetLobbyGameMode()->SetPeerTeam();
}

void FLobbyControlCharacterSelectPage::SetRemotePlayerClass(uint32, EPCClass, const FB2FindAccountInfoPtr&)
{
	// 蜡历 甸绢坷搁 蜡历墨款泼UI 盎脚
	// 评沥焊 罐绊 盎脚
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

	//	// 柳悼
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
	// 评沥焊 罐备 盎脚. 龋胶飘档 罐备盎脚.
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
	// 郴沥焊(齿酒捞叼) 罐备唱辑 殿鞭沥焊 焊晨
	//UB2UIDocControl* ControlDoc = UB2UIDocHelper::GetDocControl();
	//if (!ControlDoc)
	//	return;

	//GetLobbyGameMode()->NetSendBattleGradeInfo(ControlDoc->GetBattleGrade(), ControlDoc->GetBattleStarCount());
	//GetLobbyGameMode()->NetSendMatchToken();
}

void FLobbyControlCharacterSelectPage::BattleStart()
{
	//// 痢八 沥魂 眉农
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
	//// 弊成 鞍篮芭俊 皋牢父 结夯促.
	//// 教臂俊 荤侩且 咀磐 硅摹窍搁 官层具窃
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

	//	// 钱规栏肺 焊咯淋
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
	// 奴 芭 窍唱 固府 归弊扼款靛 肺爹秦 狄.
	// 捞 厘搁俊辑 馆拜捞唱 饭捞靛 厘搁栏肺 逞绢啊搁 荤侩窍瘤档 臼阑 康旷狼 啪 胶飘府怪 饭骇捞 flush 登绢滚府绰 何累侩捞 乐绰单 
	// 弊烦俊档 阂备窍绊 捞码辑 秦 笛 啊摹啊 乐促绊 魄窜.
	FLobbySceneManager* LobbySM = GetSceneManager();
	if (LobbySM && !LobbySM->IsSceneStreamingLevelLoaded(ELobbyScene::ELobbyScene_HeroTowerReady))
	{
		LobbySM->PrefetchSceneStreamingLevel(ELobbyScene::ELobbyScene_HeroTowerReady);
	}
}

void FLobbyEnterDungeonMain::ResponseGetChallengeModeInfo(const FB2ResponseGetChallengeModeInfoPtr& MsgPtr)
{
	//// 葛靛 惑怕沥焊 历厘
	//UB2UIDocMode* pDocMode = UB2UIDocHelper::GetDocMode();
	//if (pDocMode)
	//	pDocMode->ReceiveGetContentsModeState(MsgPtr->mode_infos);

	//UB2UIDocRaid* pDocRaid = UB2UIDocHelper::GetDocRaid();
	//if (pDocRaid)
	//	pDocRaid->SetPeriodInfos(MsgPtr->raid_open_period_infos);

	//// 康旷狼啪 巢篮萍南 悸泼
	//UB2UIDocHeroTower * pDocHeroTower = UB2UIDocHelper::GetDocHeroTower();
	//if (pDocHeroTower)
	//	pDocHeroTower->SetDailyLeftFreeTryCount(MsgPtr->hero_tower_ticket_count);

	//// 馆拜带傈 巢篮萍南 悸泼
	//UB2UIDocCounterAttack * pDocCounterAttack = UB2UIDocHelper::GetDocCounterAttack();
	//if (pDocCounterAttack)
	//	pDocCounterAttack->SetLeftTicket(MsgPtr->counter_dungeon_left_ticket_count);

	//// 闭凯 巢篮萍南 悸泼
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

	//// 傅目概聪历甫 鸥绊 甸绢吭阑 版快. response甫 罐绊 某教等 其捞瘤肺 捞悼矫糯.
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
	//		//康旷包府俊辑 静捞绰 Actor扼辑 檬扁 困摹肺 倒酒啊具父 茄促.
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
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage, false, true);// Sub 某腐磐狼 Sound notify 甫 馋促.
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
	//// 葛靛 惑怕沥焊 历厘
	//UB2UIDocMode* pDocMode = UB2UIDocHelper::GetDocMode();
	//if (pDocMode)
	//	pDocMode->ReceiveGetContentsModeState(MsgPtr->mode_infos);

	//// 痢飞傈 坷锹矫埃 沥焊 历厘
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

	//	// 傅目概聪历甫 鸥绊 甸绢吭阑 版快. response甫 罐绊 某教等 其捞瘤肺 捞悼矫糯.
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
	//		//康旷包府俊辑 静捞绰 Actor扼辑 檬扁 困摹肺 倒酒啊具父 茄促.
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
	//		LobbySubCharInfo->ActorForSub->PlaySelectAnimation(ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage, false, true);// Sub 某腐磐狼 Sound notify 甫 馋促.
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
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // 救傈户.

	//	// 磐摹 咀记阑 困茄 涝仿 官牢爹
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
	//{ // 缴脚狼 乔肺啊 钱府绰 扼固郡狼 券康 兢.
	//	RelaxingDoumiActor->PlayTemporaryAnimOfState(ELobbyRelaxingDoumiState::ELRD_Welcoming);
	//}
}
void FLobbyRestPointScene::CloseScene()
{
	// 磐摹 咀记阑 困茄 涝仿 官牢爹 力芭.
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
//{ // 笛 荤捞 函券捞 趣咯唱 澄府 静捞霸 登搁 困摹绰 颗败辑..
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
//	// 硅摹秦 初篮 绒侥 档快固 扼固郡 咀磐
//	FLobbySceneManager* LobbySM = GetSceneManager();
//	AB2LobbyRelaxingDoumi* RelaxingDoumiActor = LobbySM ? Cast<AB2LobbyRelaxingDoumi>(LobbySM->GetNPCSKActor(ELobbyNPCType::ELobbyNPC_RelaxingDoumi)) : nullptr;
//	if (RelaxingDoumiActor)
//	{
//		// 老馆罐扁牢瘤, 滴硅罐扁牢瘤 咯扁肺 逞绢柯 单捞磐父 啊瘤绊 焊妨搁 used_cash 啊 乐绰瘤肺 杭 荐档 乐变 窍瘤父
//		// 弊霸 辟夯利栏肺 老馆罐扁, 滴硅罐扁甫 沥狼窍绰 扒 酒聪聪 且 窿捞 酒聪绊.. 
//		// 捞 惑炔俊辑 UIRestPointMain 篮 凯妨 乐绢具 且 抛聪 弊率辑 夸没且 锭 历厘秦 初篮 蔼阑 掘绢促 敬促.
//
//		UB2UIRestPointMain* RestPointUI = UB2UIManager::GetInstance()->GetUI<UB2UIRestPointMain>(UIFName::RestPointMain);
//		if (RestPointUI)
//		{
//			ERestPointRewardRequestState RewardReqState = RestPointUI->GetLastRewardRequestState();
//
//			// 捞 check 俊 吧府绰 扒.. 购啊.. 绒侥焊惑 甸绢啊辑 夸没阑 喘范栏唱 览翠捞 绝绢辑 唱艾促啊 促矫 绒侥 焊惑 芒栏肺 甸绢吭绰单 弊力辑具 览翠捞 吭促芭唱?
//			// 父距俊 沥富 弊繁 老捞 老绢唱搁 弊成 公矫.
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

	//// 备概 冉荐
	//for (auto Restrict : ShopData->shop_buy_manages)
	//{
	//	DocStore->SetReceivedRestrictCount(Restrict->product_id, Restrict->buy_count);
	//}

	//// 扁埃力 巢篮 矫埃
	//for(auto PeridicItem: ShopData->periodic_package_states)
	//{
	//	DocStore->SetReceivedPeriodicPackageLeftTime(PeridicItem->package_id, PeridicItem->end_pay_time);
	//}

	//// 饭骇诀 菩虐瘤
	//for (auto LevelUpMissionItem : ShopData->level_up_package_missions)
	//{
	//	DocStore->SetReceivedLevelupPacakgeMissions(LevelUpMissionItem->package_id, LevelUpMissionItem);
	//}

	//// 饭骇 崔己 拌距 菩虐瘤
	//for (auto ReachLevelMissionItem : ShopData->reach_level_package_missions)
	//{
	//	DocStore->SetReceivedReachLevelPacakgeMissions(ReachLevelMissionItem->package_id, ReachLevelMissionItem);
	//}

	//// 岿沥咀
	//if (ShopData->flat_rate_package_bonus_state)
	//{
	//	DocStore->SetReceivedFlatratePackageBonusState(ShopData->flat_rate_package_bonus_state);
	//}

	//// 菩虐瘤 捞亥飘 付农
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
	//	// 茄锅 犁积 沁栏搁
	//	if (PlayedMatinee[CurrentHallOfFameMode])
	//	{
	//		ForceEndMatinee();
	//	}
	//	// 犁积 救沁栏搁
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
	//		LobbyGM->SetupLoadingScreenImageCommon(false, false, false); // 柳青档绰 绝捞 肺爹拳搁父 剁框.
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

// 1殿 某腐篮 皋浆啊 4俺
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
	//		// 磊悼 盎脚
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

	//// 寸老 盎脚 冉荐
	//DocStore->SetRenewalCount(MagicShopPtr->renewal_count);

	//// 促澜 磊悼 盎脚 矫埃
	//DocStore->SetNextFreeRenewalTime(MagicShopPtr->next_free_renewal_time);

	//// 公丰 盎脚 咯何
	//DocStore->SetIsFreeRenewal(MagicShopPtr->is_free_renewal);

	//// 付过 惑痢 惑前 府胶飘
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
	//const float PinpointWeight = FMath::Clamp(ZWeight, -0.3f, 1.f);	// 0俊 啊鳖匡荐废 某腐磐狼 吝缴俊 Pinpoint

	//const float WeightConstant = 0.5f;	// 惑 ~ 窍窜俊 巧器牢飘啊 啊鳖匡荐废 50% 狼 咯盒 Distance甫 淋
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

	//	// Camera 捞悼 Direction
	//	const bool IsZoomIn = Delta > 0 ? true : false;
	//	const FVector CameraMoveVector = IsZoomIn ? (PinpointLocation - CurrCamLocation) : (OrgCameraLocation - CurrCamLocation);
	//	if (CameraMoveVector.IsNearlyZero(1.f))
	//		return;

	//	const FVector CameraMoveDir = CameraMoveVector.GetSafeNormal();

	//	// Camera 捞悼 抗惑 困摹
	//	const FVector ExpectCameraLocation = CurrCamLocation + (CameraMoveDir * Delta);

	//	// 泅犁 墨皋扼客 某腐磐 芭府
	//	const float CurrCamDistance = (CurrCamLocation - PinpointLocation).Size();

	//	// 捞悼 抗惑 困摹俊辑狼 墨皋扼 客 某腐磐 芭府
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
	//	LobbyGameMode->UpdateNativeLobbyMatineeRef(); // 救傈户.

	//												  // 磐摹 咀记阑 困茄 涝仿 官牢爹
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

	//// 捞傈 其绢府 救焊捞档废.
	//AB2LobbyFairyDoumi* FairyBeforeActor = LobbySM ? Cast<AB2LobbyFairyDoumi>(LobbySM->GetNPCSKActor(BeforeType)) : nullptr;
	//if (FairyBeforeActor)
	//{
	//	FairyBeforeActor->SetVisibleCharacter(false);
	//}

	//// 促澜 其绢府绰 焊捞霸窍绊, 局聪皋捞记 犁积
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
//	// 磐摹 咀记阑 困茄 涝仿 官牢爹 力芭.
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
	//		//康旷包府俊辑 静捞绰 Actor扼辑 檬扁 困摹肺 倒酒啊具父 茄促.
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