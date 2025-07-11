#pragma once
//#include "BladeII.h"
//
#include "EventSubsystem.h"
//#include "Animation/SkeletalMeshActor.h"
#include "BladeIIPlayer.h"
//#include "B2LobbyUISwitcher.h"
#include "B2StageEventDirector.h"
#include "B2UIEnum.h"
//#include "B2LobbySceneManager.h"
//#include "B2UIManager_Lobby.h"
#include "CommonStruct.h"
#include "Engine/Channel.h"
#include "B2Chatting.h"
#include "B2GameLoadingScreen.h"
#include "UMG/Public/Components/Button.h"
#include "../UI/Widget/B2Button.h"
//#include "../UI/DJLegacy/B2FloatingAbnormalEffect.h"

class ABladeIICharacter;
class ABladeIIPlayer;
class AB2MonsterSpawnPool;
class AB2StageEventDirector;
class ASkeletalMeshActor;
class Quest;
enum class EStateAfterGradeUp : uint32;
enum class EMissionType : uint8;

//
//DECLARE_EVENT_CLASS(StageBegin)
//DECLARE_EVENT_CLASS(CharacterBeginPlay, ABladeIICharacter*)
//DECLARE_EVENT_CLASS(CancelCastAreaDamage, ABladeIICharacter*)
//DECLARE_EVENT_CLASS(TargetDamaged, ABladeIICharacter*, int32, float)
//DECLARE_EVENT_CLASS(CharacterTakeDamage, ABladeIICharacter*, float, bool)
////DECLARE_EVENT_CLASS(CharacterAbnormalEffect, ABladeIICharacter*, const FLoadedFloatingAbnormalEffectSetupInfo&)
//DECLARE_EVENT_CLASS(PlayerTagged, ABladeIIPlayer*, ABladeIIPlayer*)
//DECLARE_EVENT_CLASS(CharacterDeath, ABladeIICharacter*)
//DECLARE_EVENT_CLASS(PlayerSuckedGold, int32)
//DECLARE_EVENT_CLASS(SetHUDCenterMessage, const FText, float, int32)
//DECLARE_EVENT_CLASS(HideHUDCenterMessage)
//DECLARE_EVENT_CLASS(SetSpawnNotifyMessage, const FString&, const float)
//DECLARE_EVENT_CLASS(RemoveSpawnNotifyMessage)
//
//DECLARE_EVENT_CLASS(PleaseForgetMe, ABladeIICharacter*)
//DECLARE_EVENT_CLASS(OnClearStage)
//DECLARE_EVENT_CLASS(OnResetStartingLevelInfo)
//DECLARE_EVENT_CLASS(PlayerHideMobPointer, bool)
//DECLARE_EVENT_CLASS(BornAgainChangedPlayer, int32, ABladeIIPlayer*, ABladeIIPlayer*)
//
//DECLARE_EVENT_CLASS(PlayerStartAttack)
//DECLARE_EVENT_CLASS(PlayerStopAttack)
//DECLARE_EVENT_CLASS(PlayerStartSkill01)
//DECLARE_EVENT_CLASS(PlayerStartSkill02)
//DECLARE_EVENT_CLASS(PlayerStartSkill03)
//DECLARE_EVENT_CLASS(PlayerStartGuard)
//DECLARE_EVENT_CLASS(PlayerStopGuard)
//DECLARE_EVENT_CLASS(PlayerStartWeaponSkill)
//DECLARE_EVENT_CLASS(PlayerStartWeaponSkillByIndex, int32)
//DECLARE_EVENT_CLASS(PlayerStartTag)
//DECLARE_EVENT_CLASS(PlayerChanged, ABladeIIPlayer*)
//DECLARE_EVENT_CLASS(TagPlayerSpawned, ABladeIIPlayer*)
//DECLARE_EVENT_CLASS(PlayerGuardStarted)
//DECLARE_EVENT_CLASS(PlayerTakeAttack, float)
//DECLARE_EVENT_CLASS(PlayerCounterAttackSucceed)
//
//DECLARE_EVENT_CLASS(TogglePlayerAutoState)
//DECLARE_EVENT_CLASS(StartPauseMenu, EB2GameMode)
//DECLARE_EVENT_CLASS(StopPauseMenu)
//DECLARE_EVENT_CLASS(StartDefeatMenu, EB2GameMode)
//DECLARE_EVENT_CLASS(StopDefeatMenu)
//DECLARE_EVENT_CLASS(OpenPreLoadingScreen, EPreLoadingScreenType, EB2GameMode)
//DECLARE_EVENT_CLASS(ClosePreLoadingScreenAll)
//DECLARE_EVENT_CLASS(ClosePreLoadingScreen, EPreLoadingScreenType)
//DECLARE_EVENT_CLASS(LobbyConditionalBeginOnTouchImpulseRing, const FVector2D&)
//DECLARE_EVENT_CLASS(LobbyMoveOnTouchImpulseRing, const FVector2D&)
//DECLARE_EVENT_CLASS(LobbyEndOnTouchImpulseRing)
//DECLARE_EVENT_CLASS(LobbyShowNetworkLoadingIcon, bool)
//DECLARE_EVENT_CLASS(ShowMiscLoadingIcon, bool)
//
//DECLARE_EVENT_CLASS(StartVisibleChatPopup, EB2GameMode)
//DECLARE_EVENT_CLASS(StopVisibleChatPopup)
//
////DECLARE_EVENT_CLASS(DJLegacy_ChangeLobbyUIPage, ELobbyUIPages)
////DECLARE_EVENT_CLASS(DJLegacy_OpenLobbySubPopup, ELobbySubPopups, FDJLegacySubpopupAddPanelInfo)
////DECLARE_EVENT_CLASS(DJLegacy_CloseLobbySubPopup, ELobbySubPopups)
//DECLARE_EVENT_CLASS(ChangeInvenItemOpMode, ELobbyInvenItemOpMode)
//DECLARE_EVENT_CLASS(LobbyPCSelectionChanged, EPCClass)
//DECLARE_EVENT_CLASS(StartGameStageFromLobby, int32, EStageDifficulty, const TArray<EStageBuffType>&, bool)
//DECLARE_EVENT_CLASS(CancelStartGameStage)
//DECLARE_EVENT_CLASS(StartRepeatBattleStage, int32, EStageDifficulty)
//DECLARE_EVENT_CLASS(CancelOrStopRepeatBattle)
//DECLARE_EVENT_CLASS(RefreshAutoSellState)
//DECLARE_EVENT_CLASS(SetPlayerAutoType, int32)
//DECLARE_EVENT_CLASS(GiveUpGame)
//DECLARE_EVENT_CLASS(EndPlayOfSingleRepeatBattleStage)
//DECLARE_EVENT_CLASS(LobbyLoginAndStartGame, const FString&, const FString&)
//DECLARE_EVENT_CLASS(LobbyCreateAccount, const FString&, const FString&)
//DECLARE_EVENT_CLASS(LobbySetPCSelection, EPCClass, EPCClass)
//DECLARE_EVENT_CLASS(LobbySetMainPCClass, EPCClass)
//DECLARE_EVENT_CLASS(LobbyCharacterIntroSetPCSelection, EPCClass)
//DECLARE_EVENT_CLASS(LobbyProceedOnCharSel)
//DECLARE_EVENT_CLASS(LobbyEnterHeroMgmtMode, EHeroMgmtSubMode)
//DECLARE_EVENT_CLASS(LobbyQuitHeroMgmtMode)
//DECLARE_EVENT_CLASS(LobbySetHeroMgmtModePCSelection, EPCClass)
//DECLARE_EVENT_CLASS(LobbyPopupCreateAccount)
//DECLARE_EVENT_CLASS(LobbyUpdateAllItemIcons)
//DECLARE_EVENT_CLASS(LobbyTryUpdateSingleItemIcon, int64)
//DECLARE_EVENT_CLASS(LobbyStartMatch, bool)
//DECLARE_EVENT_CLASS(DoMarkCombineRedDot)
//DECLARE_EVENT_CLASS(EnterItemLevelupMode, int64)
//DECLARE_EVENT_CLASS(EnterItemUpgradeMode, int64)
//DECLARE_EVENT_CLASS(EnterItemCompositionMode, int64)
//DECLARE_EVENT_CLASS(EnterItemSurpassMode, int64)
//DECLARE_EVENT_CLASS(EnterItemEnhanceMode, int64)
//DECLARE_EVENT_CLASS(EnterItemCostumeEnhanceMode, int64)
//
//DECLARE_EVENT_CLASS(UpdateLobbyInventoryControl) 
//
//DECLARE_EVENT_CLASS(LobbyReqUnEquipItemsOtherPreset, int32, const TArray<int64>&)
//DECLARE_EVENT_CLASS(UpdateItemPreset, EPCClass, EItemPresetSlotType)
//DECLARE_EVENT_CLASS(UpdateSkillSlot, EPCClass, EItemPresetSlotType)
//DECLARE_EVENT_CLASS(SelectPresetSlot, EItemPresetSlotType)
//DECLARE_EVENT_CLASS(SelectPreviewPresetSlot, EItemPresetSlotType)
//
//DECLARE_EVENT_CLASS(LobbyInventoryViewJustOpen)
//DECLARE_EVENT_CLASS(LobbyInventoryViewJustClose)
//DECLARE_EVENT_CLASS(LobbyExternalSetInventoryTab, EItemInvenType, bool)
//DECLARE_EVENT_CLASS(QuitItemOpMode, bool)
//DECLARE_EVENT_CLASS(LobbyEnterItemOpDirectingView, ELobbyInvenItemOpMode)
//DECLARE_EVENT_CLASS(LobbyQuitItemOpDirectingView)
//DECLARE_EVENT_CLASS(SelectForLevelUpGrade, int32);
//
//DECLARE_EVENT_CLASS(EnableInventoryTouch, bool)
//DECLARE_EVENT_CLASS(DeliveryAppUpdatePopUp)
//DECLARE_EVENT_CLASS(SelectForItemLevelupIngred, int64)
//DECLARE_EVENT_CLASS(UnSelectForItemLevelupIngred, int64)
//DECLARE_EVENT_CLASS(SelectForComposeIngred, int64)
//DECLARE_EVENT_CLASS(UnSelectForComposeIngred, int64)
//DECLARE_EVENT_CLASS(SelectForSurpassIngred, int64)
//DECLARE_EVENT_CLASS(UnSelectForSurpassIngred, int64)
//DECLARE_EVENT_CLASS(SelectForEnhanceIngred, int64)
//DECLARE_EVENT_CLASS(UnSelectForEnhanceIngred, int64)
//DECLARE_EVENT_CLASS(SelectForEnhanceChangeIngred, int64)
//DECLARE_EVENT_CLASS(UnSelectForEnhanceChangeIngred, int64)
//DECLARE_EVENT_CLASS(SelectForEnhanceProtectionItem, int32)
//DECLARE_EVENT_CLASS(SelectForItemHammer, int64)
//DECLARE_EVENT_CLASS(UnSelectForItemHammer, int64)
//DECLARE_EVENT_CLASS(LobbyTryUpdateItemOPMainMenu)
//DECLARE_EVENT_CLASS(LobbySlotChangeEffect)
//DECLARE_EVENT_CLASS(LobbyReqEquipItems, const TArray<int64>&)
//DECLARE_EVENT_CLASS(LobbyReqUnEquipItems, const TArray<int64>&)
//DECLARE_EVENT_CLASS(LobbyReqSellItem, int64, bool)
//DECLARE_EVENT_CLASS(LobbyReqSellItems, const TArray<int64>&, bool)
//DECLARE_EVENT_CLASS(LobbyReqDecompositionItems, const TArray<int64>&, bool)
//DECLARE_EVENT_CLASS(LobbyReqSellConsumableItem, int64, int32)
//DECLARE_EVENT_CLASS(LobbyReqConvertData, int32)
//DECLARE_EVENT_CLASS(LobbyReqConvertItem, int32, int32)
//DECLARE_EVENT_CLASS(LobbyReqToggleItemLock, int64, bool)
//DECLARE_EVENT_CLASS(LobbyReqItemLevelupWithCurrentSelection)
//DECLARE_EVENT_CLASS(LobbyReqEnhanceItem)
//DECLARE_EVENT_CLASS(LobbyReqUpgradeItem)
//DECLARE_EVENT_CLASS(LobbyReqComposeItemWithCurrentSelection)
//DECLARE_EVENT_CLASS(LobbyReqSurpassItemWithCurrentSelection)
//DECLARE_EVENT_CLASS(LobbyReqAutoItemLevelup, const TArray<FB2Item>&, bool)
//DECLARE_EVENT_CLASS(LobbyAllItemsAdded)
//DECLARE_EVENT_CLASS(LobbyItemsAdded, const FB2AddedItems&)
//DECLARE_EVENT_CLASS(LobbySetCombatEntryChapterNum, int32, EStageDifficulty)
//DECLARE_EVENT_CLASS(GoGameStageInfoFromLobby, FServerStageID, FRepeatBattleStateSet)
//DECLARE_EVENT_CLASS(GoGameStageInfoLastReachedFromLobby)
//DECLARE_EVENT_CLASS(GameStageCheckForArinConsulting, FServerStageID)
//DECLARE_EVENT_CLASS(ChangeUserAccountInfo)
//DECLARE_EVENT_CLASS(ChangeStaminaChargeTime)
//DECLARE_EVENT_CLASS(ChangePvPMatchPointChargeTime)
//DECLARE_EVENT_CLASS(StartRankingUpAnimation, const EStateAfterGradeUp)
//DECLARE_EVENT_CLASS(ChangeTeamMatchPointChargeTime)
//DECLARE_EVENT_CLASS(LobbyInvenSetItemOpModeGuideText, const FText&)
//DECLARE_EVENT_CLASS(LobbyInvenHideItemOpModeGuideText)
////DECLARE_EVENT_CLASS(LobbyChangeScene, ELobbyScene)
//DECLARE_EVENT_CLASS(LobbyChangeSceneByUIScene, EUIScene)
//DECLARE_EVENT_CLASS(LobbyChangeToChapterSceneByUIScene, int32)
//DECLARE_EVENT_CLASS(LobbyUpdateCharacterPart, EPCClass)
//DECLARE_EVENT_CLASS(LobbyEnterPVPMainMenu)
//DECLARE_EVENT_CLASS(ReturnToFindAccount)
//DECLARE_EVENT_CLASS(ReturnToPVPMainMenu, bool)
//DECLARE_EVENT_CLASS(ReturnToPVPRematch)
//DECLARE_EVENT_CLASS(ReturnToHeroTowerMainMenu)
//DECLARE_EVENT_CLASS(ReturnToRaidMain)
//DECLARE_EVENT_CLASS(ReturnToGuildMain)
//DECLARE_EVENT_CLASS(LobbyEnterTeamMatchMenu)
//DECLARE_EVENT_CLASS(LobbyEnterControlMenu)
//DECLARE_EVENT_CLASS(EnterTMHeroChangeMenu)
//DECLARE_EVENT_CLASS(LobbyAnyOtherUpdateOnSetAccount)
//DECLARE_EVENT_CLASS(LobbyUpdateChapterData)
//DECLARE_EVENT_CLASS(UpdateChatUI, bool)
////DECLARE_EVENT_CLASS(UpdateChatSinglePopup, const FB2ChatMessage&)
//DECLARE_EVENT_CLASS(UpdateAdminMessage, const FString&, const int32)
//DECLARE_EVENT_CLASS(LobbyPlayUIEquipEffect, EItemEquipPlace)
//DECLARE_EVENT_CLASS(LobbyReceivedRepeatClearReward, int32)
//DECLARE_EVENT_CLASS(EnterShop, int32)
//DECLARE_EVENT_CLASS(LobbyUpdateWingMainUI)
//DECLARE_EVENT_CLASS(BeginReqForLobbyInitWingData)
//DECLARE_EVENT_CLASS(SetLocalWingDataByAccountInfo, const TArray<b2network::B2CharacterServerInfoPtr>&, const TArray<b2network::B2WingOptionInfoPtr>&)
//DECLARE_EVENT_CLASS(UpdateWingCombatStatsOnMD)
//DECLARE_EVENT_CLASS(BeginWingEvolutionScene, EPCClass)
//DECLARE_EVENT_CLASS(EndWingEvolutionScene)
//DECLARE_EVENT_CLASS(LinkUICollectBook, EPCClass, EItemClass)
//DECLARE_EVENT_CLASS(FindAccountRequestEvent, int64, EPCClass, EPCClass, EPCClass)
//DECLARE_EVENT_CLASS(ChangeLastClearStage, EStageDifficulty, int32, int32)
//DECLARE_EVENT_CLASS(EndHallOfFameMatinee)
//
//DECLARE_EVENT_CLASS(ReserveStoreScene, bool)
//
////DECLARE_EVENT_CLASS(SpawnPoolMobSpawn, ABladeIICharacter*, const FMobSpawnedBirthplaceInfo&)
//DECLARE_EVENT_CLASS(SpawnPoolComplete, AB2MonsterSpawnPool*)
//DECLARE_EVENT_CLASS(SpawnPoolSingleWaveStart, AB2MonsterSpawnPool*, int32)
//DECLARE_EVENT_CLASS(SpawnPoolSingleWaveComplete, AB2MonsterSpawnPool*, int32)
//DECLARE_EVENT_CLASS(SpawnPoolWaveMobDead, AB2MonsterSpawnPool*, ABladeIICharacter*, int32, int32)
//DECLARE_EVENT_CLASS(SpawnPoolWaveMobPhaseChange, AB2MonsterSpawnPool*, ABladeIICharacter*, ABladeIICharacter*, int32, int32, int32)
//DECLARE_EVENT_CLASS(SpawnPoolSummonedMobDead, AB2MonsterSpawnPool*, ABladeIICharacter*)
////DECLARE_EVENT_CLASS(StageEventSceneBegin, AB2StageEventDirector*, EStageEvent)
//DECLARE_EVENT_CLASS(StageEventSceneEnd, AB2StageEventDirector*)
//DECLARE_EVENT_CLASS(PlayStageBGM)
//DECLARE_EVENT_CLASS(EnsureStageEventTimeDilation)
//
//DECLARE_EVENT_CLASS(StageEventTalkStart, int32, bool)
//DECLARE_EVENT_CLASS(StageEventTalkEnd, int32, bool)
//
//DECLARE_EVENT_CLASS(ShowStageClearWingEffect)
//DECLARE_EVENT_CLASS(ShowStageClearGemEffect)
//DECLARE_EVENT_CLASS(BeginStageClearScene_Phase1)
//DECLARE_EVENT_CLASS(BeginStageClearScene_Phase2)
//
//DECLARE_EVENT_CLASS(BeginGlobalSceneBlur, float)
//DECLARE_EVENT_CLASS(EndGlobalSceneBlur)
//DECLARE_EVENT_CLASS(RestartGame)
//DECLARE_EVENT_CLASS(GoToTitle)
//DECLARE_EVENT_CLASS(GoToVillage)
//DECLARE_EVENT_CLASS(GoToVillageDefeated)
//DECLARE_EVENT_CLASS(GoToMap)
//DECLARE_EVENT_CLASS(GoToDeferredStage, int32)
//DECLARE_EVENT_CLASS(GoToTutorial , bool)
//DECLARE_EVENT_CLASS(GoToDLCMap)
//DECLARE_EVENT_CLASS(FailResponseMasterData)
//
///* InGame flow Event*/
//DECLARE_EVENT_CLASS(FightStart, int32)
//
///* InGame flow Event*/
//
//DECLARE_EVENT_CLASS(RequestResurrect, EResurrectBuffType)
//
//DECLARE_EVENT_CLASS(LobbyMissionAlarm)
//DECLARE_EVENT_CLASS(ShowMissionDialog)
//
//DECLARE_EVENT_CLASS(OpenDialog, FName, bool)
//DECLARE_EVENT_CLASS(NextDialog)
//DECLARE_EVENT_CLASS(SkipDialog)
//DECLARE_EVENT_CLASS(OnUpdatedDialog, FName, int32)
//DECLARE_EVENT_CLASS(CloseDialog, FName)
//DECLARE_EVENT_CLASS(ToolTipOpen)
//DECLARE_EVENT_CLASS(ToolTipClose)
//DECLARE_EVENT_CLASS(ToolTipMove)
//DECLARE_EVENT_CLASS(ForcedCloseToolTipPopup)
//
//DECLARE_EVENT_CLASS(UIWidgetOpenComplete, FName)
//
//DECLARE_EVENT_CLASS(StageComplete)
//
//DECLARE_EVENT_CLASS(CounterFinishAttack)
//DECLARE_EVENT_CLASS(QTEAttack)
//
//DECLARE_EVENT_CLASS(SelectCharacterToSummonItem, int32, bool)
//DECLARE_EVENT_CLASS(SelectItemToBuy, int32)
//DECLARE_EVENT_CLASS(UpdateMarkRedDot, const FName&)
//DECLARE_EVENT_CLASS(ProcessLobbyTutorial)
//DECLARE_EVENT_CLASS(AutoMinoRidding)
//
//DECLARE_EVENT_CLASS(SetEquippedEther, const int32, const TArray<b2network::B2AetherServerInfoPtr>&)
//DECLARE_EVENT_CLASS(OpenEtherSetToolTip, int32, EPCClass)
//
//DECLARE_EVENT_CLASS(OnEtherActivated, int32, ABladeIIPlayer*, ABladeIICharacter*);
//
//DECLARE_EVENT_CLASS(MarkBeginAsyncLoad)
//
//DECLARE_EVENT_CLASS(OnUpdateCombineMenu, bool)
//
//DECLARE_EVENT_CLASS(RequestCurrentTabMailList)
//
//DECLARE_EVENT_CLASS(RequestJoinRoomBridge, const FString&, int32, int32)
//
//DECLARE_EVENT_CLASS(LobbyBannerAllLoad)
//
//DECLARE_EVENT_CLASS(RefreshHotTimeUI)
//DECLARE_EVENT_CLASS(BuyHotTimeBuffSuccessful)
//DECLARE_EVENT_CLASS(GetEventStatusSuccessful)
//DECLARE_EVENT_CLASS(GetLatestEventInfoSuccessful)
//DECLARE_EVENT_CLASS(DiceCellFinishAnimation, int32)
//DECLARE_EVENT_CLASS(DiceCellFinishArriveAnimation, int32)
//
//DECLARE_EVENT_CLASS(SetupLoadingScreenImage, bool, bool, bool, const TCHAR*, EB2LoadingMessageType)
//DECLARE_EVENT_CLASS(ConditionalWaitForLoadingScreen)
//
//DECLARE_EVENT_CLASS(ErrorOpenWebEmptyURL)
//
//DECLARE_EVENT_CLASS(SkipTutorialRefreshUI)
//
//DECLARE_EVENT_CLASS(UseGuildNPCSkill, bool)
//DECLARE_EVENT_CLASS(ForceFocusViewTarget, ABladeIIPlayer*, float)
//
//DECLARE_EVENT_CLASS(GuildGameChangeCamera, bool, ABladeIIPlayer*)
//
//DECLARE_EVENT_CLASS(FirstRequestUpdateDailyPlayTime)
///* 
//	LobbyScene Events
//*/
//DECLARE_EVENT_CLASS(LobbyCharIntroSelect, EPCClass)
//DECLARE_EVENT_CLASS(LobbyHeroMgmtSelect, EPCClass)
//DECLARE_EVENT_CLASS(LobbyStageInfoSelect, EPCClass, EPCClass)
//DECLARE_EVENT_CLASS(LobbyCharObvSelect, EPCClass)
//DECLARE_EVENT_CLASS(LobbyCharEquippedItem, EPCClass, EItemEquipPlace)
//DECLARE_EVENT_CLASS(LobbyCharCollectBookSelect, EPCClass)
//DECLARE_EVENT_CLASS(LobbyCharObvForcedChange, EPCClass)
//
//DECLARE_EVENT_CLASS(LobbyCharPlayMatinee, float)
//DECLARE_EVENT_CLASS(LobbyCharMoveCamera, const FRotator&, float)
//DECLARE_EVENT_CLASS(LobbyCharRotateCharacter, float)
//DECLARE_EVENT_CLASS(LobbyCharCameraViewTarget, const FVector2D&)
//DECLARE_EVENT_CLASS(LobbyChapterCameraMove, float)
//DECLARE_EVENT_CLASS(LobbyBlockMoveCamera, bool)
//DECLARE_EVENT_CLASS(LobbyMainCameraMove, float)
//DECLARE_EVENT_CLASS(LobbyCharZoom, const FVector2D&, float)
//
////DECLARE_EVENT_CLASS(CreateLobbyCharOnHeadDisplay, EPCClass, ASkeletalMeshActor*, ELobbyOnHeadDisplayType)
//DECLARE_EVENT_CLASS(DestroyAllLobbyCharOnHeadDisplay)
//DECLARE_EVENT_CLASS(UpdateLobbyCharOnHeadDisplay)
//DECLARE_EVENT_CLASS(LobbyCharObservePopupNickEdit, EPCClass)
//DECLARE_EVENT_CLASS(LobbyCharObserveCloseNickEdit)
//
//DECLARE_EVENT_CLASS(LobbyCharObserveChangeScreen, bool)
//
//DECLARE_EVENT_CLASS(UpdateCombatPower, EPCClass, bool);
//DECLARE_EVENT_CLASS(ShowBattleScoreNotify, EPCClass);
//
//DECLARE_EVENT_CLASS(RequestCombatPower, EPCClass);
//
//DECLARE_EVENT_CLASS(DoneToGetAccount)
//DECLARE_EVENT_CLASS(LobbyHeroFindAccount, EPCClass);
//
//DECLARE_EVENT_CLASS(LobbyHeroTransientEffectBegin, const int32);
//
//DECLARE_EVENT_CLASS(NotifyUIButtonClicked, UButton*);
//
//DECLARE_EVENT_CLASS(UpdateEventAttendanceParts)
//DECLARE_EVENT_CLASS(ReceiveEventAttendanceReward, int32, int32, b2network::B2RewardPtr)
//DECLARE_EVENT_CLASS(ReceiveAttendanceExitEnable, bool)
//DECLARE_EVENT_CLASS(ReceiveAttendanceExitVisibility, bool)
//DECLARE_EVENT_CLASS(ReceivePlayTimeReward, const TArray<b2network::B2RewardPtr>&)
//DECLARE_EVENT_CLASS(UpdateAccumulatedPlayTime, int32)
//DECLARE_EVENT_CLASS(LobbyNoticePopUp, int32)
//
//DECLARE_EVENT_CLASS(FinishConnectReward)
//DECLARE_EVENT_CLASS(SummonitemOpenCardMatineePlay, int32)
//
//DECLARE_EVENT_CLASS(ShowRaidInviteNotice);
//
//DECLARE_EVENT_CLASS(SelectEventPageTab , int32)
//DECLARE_EVENT_CLASS(SelectEventTabShopLimited, int32)
//DECLARE_EVENT_CLASS(StratAnimEventMainTransparencyBlock)
//
//DECLARE_EVENT_CLASS(CloseCombineMenu)
//
//DECLARE_EVENT_CLASS(PlayLobbyBgm)
//
//DECLARE_EVENT_CLASS(OnChangUIScene, EUIScene)
//DECLARE_EVENT_CLASS(OnChangUISceneBack)
//
//DECLARE_EVENT_CLASS(OnChangeFairyCharacter, EFairyType)
//
//DECLARE_EVENT_CLASS(OpenCostume, bool)
//DECLARE_EVENT_CLASS(EnterCostumePage, ECostumePage)
//DECLARE_EVENT_CLASS(CostumeEquipSlotVisible, bool)
//DECLARE_EVENT_CLASS(SuccesGetAccountCostumeAll)
//
//DECLARE_EVENT_CLASS(SelectCostumePackage, int32)
//
//DECLARE_EVENT_CLASS(RefreshCostumeHeroMgmtRedDot)
//
//DECLARE_EVENT_CLASS(RefreshCostumeRefresh, EItemInvenType)
//
//DECLARE_EVENT_CLASS(ForcedInvenTabOpen, EItemInvenType)
//
//DECLARE_EVENT_CLASS(ForcedClickCostumeMaterial, bool)
//DECLARE_EVENT_CLASS(OnCallBackEnhanceCostumeInvenPage, const FB2Item&)
//
//DECLARE_EVENT_CLASS(AllClearUnityAwakenMission)
//
//DECLARE_EVENT_CLASS(UpdateFairyGiftState)
//DECLARE_EVENT_CLASS(UpdateCombatStatScale, const float)
//
///*
//	SkillList Events
//*/
//DECLARE_EVENT_CLASS(ChangeSkillPoint)
//DECLARE_EVENT_CLASS(ChangeSkillLevel, int32)
//DECLARE_EVENT_CLASS(ChangeBuySkillPoint)
//DECLARE_EVENT_CLASS(CloseSkillTooltip, int32)
//DECLARE_EVENT_CLASS(RegistSkill)
//
///*
//	LinkManagerEvent
//*/
//
//DECLARE_EVENT_CLASS(LinkChangeUIScene)
//
///*
//	EtherEvent
//*/
//
//DECLARE_EVENT_CLASS(RefreshScrollUI)
//DECLARE_EVENT_CLASS(SelectEtherLotterySlot, int32)
//DECLARE_EVENT_CLASS(SellEtherItem, int64, int32)
//DECLARE_EVENT_CLASS(ResultAllSellEtherItem, int32)
//
///*
//	BackEvent
//*/
//
//DECLARE_EVENT_CLASS(SetCloseBattleMainUI, bool)
//
///*
//	MailEvent
//*/
//
//DECLARE_EVENT_CLASS(SetLotteryAllFilteringInfo, TArray<int64>&, EPCClass, int32)
//
//
///*
//	PlatformPauseEvent
//*/
//
//DECLARE_EVENT_CLASS(PlatformPausePress)
//
///*
//	MissionEvent
//*/
//
//DECLARE_EVENT_CLASS(GetAllMIssionQuestReward)
//DECLARE_EVENT_CLASS(ChangeMissionTab, const EMissionType, const bool)
//DECLARE_EVENT_CLASS(ReqGetContetnsOpenStatus, const int32)
//DECLARE_EVENT_CLASS(SetAllRewardButtonDisable, const bool)
//DECLARE_EVENT_CLASS(UpdateMainQuestScrollByIndex, const int32)
//DECLARE_EVENT_CLASS(UpdateSubQuestScoll)
//
//DECLARE_EVENT_CLASS(UpdateDonationResultPopup)
//DECLARE_EVENT_CLASS(UpdateDonationEvent)
//DECLARE_EVENT_CLASS(OpenDonationPointStepRewardInfoPopup, int32)
//DECLARE_EVENT_CLASS(UpdateDonationPointRewardUI)
//
///*
//	Network Events
//
//	-> Let's think about event naming convetion kkk
//*/
//DECLARE_EVENT_CLASS(NoAccount)
//DECLARE_EVENT_CLASS(RequestGiveNickname,				const FString&)
//DECLARE_EVENT_CLASS(RequestUpdateNickname,				const FString&, uint32)
//DECLARE_EVENT_CLASS(DeliveryGetServerVersion,			const FString&, const FString&)
//
//DECLARE_EVENT_CLASS(DeliveryAccountInfo,				FB2WholeInfo)
//DECLARE_EVENT_CLASS(DeliveryAccountItem,				FB2AccountItems)
//DECLARE_EVENT_CLASS(DeliveryMasterData,					FB2MasterDatas)
//DECLARE_EVENT_CLASS(DeliveryAddItem,					FB2AddedItems)
//DECLARE_EVENT_CLASS(DeliveryDismantleItems,				FB2DismantleItems)
//DECLARE_EVENT_CLASS(DeliveryDismantleItemsError)
//DECLARE_EVENT_CLASS(DeliverySellItem,					FB2SoldItems)
//DECLARE_EVENT_CLASS(DeliverySellItemError)
//DECLARE_EVENT_CLASS(DeliverySellConsumableItem,			FB2SellConsumableItem)
//DECLARE_EVENT_CLASS(DeliveryConvertData,				FB2ConvertData)
//DECLARE_EVENT_CLASS(DeliveryConvertItem,				FB2ConvertItem)
//DECLARE_EVENT_CLASS(DeliveryEquipItem,					FB2EquippedItems)
//DECLARE_EVENT_CLASS(DeliveryUnequipItem,				FB2UnequippedItems)
//DECLARE_EVENT_CLASS(DeliveryLockItem,					FB2LockedItems)
//DECLARE_EVENT_CLASS(DeliveryUnlockItem,					FB2UnlockedItems)
//DECLARE_EVENT_CLASS(DeliveryItemLevelup,				FB2LevelupItem)
//DECLARE_EVENT_CLASS(DeliveryEnhanceItem,				FB2EnhanceItem)
//DECLARE_EVENT_CLASS(DeliveryItemEnhanceLevel,			FB2ItemEnhanceLevel)
//DECLARE_EVENT_CLASS(DeliveryOpenSealSlotItem,			FB2ResponseOpenSealSlotItemPtr)
//DECLARE_EVENT_CLASS(DeliveryOpenSealSlotItemError)
//DECLARE_EVENT_CLASS(DeliveryCarveSealItem,				FB2ResponseCarveSealItemPtr)
//DECLARE_EVENT_CLASS(DeliveryCarveSealItemError)
//DECLARE_EVENT_CLASS(DeliveryGetItemSurpassCost,			FB2GetItemSurpassCost)
//DECLARE_EVENT_CLASS(DeliverySurpassItem,				FB2SurpassItem)	
//DECLARE_EVENT_CLASS(DeliveryGetItemUpgradeCost,			FB2GetItemUpgradeCost)
//DECLARE_EVENT_CLASS(DeliveryUpgradeItem,				FB2UpgradeItem)	
//DECLARE_EVENT_CLASS(DeliveryGetItemComposeCost,			FB2GetItemComposeCost)
//DECLARE_EVENT_CLASS(DeliveryComposeItem,				FB2ComposeItem)
//DECLARE_EVENT_CLASS(DeliveryExtendInventory,			FB2ExtendInventory)
//DECLARE_EVENT_CLASS(DeliveryStartStage,					FB2StartStageInfo)
//DECLARE_EVENT_CLASS(DeliveryStartStageError)
//DECLARE_EVENT_CLASS(DeliveryResurrection,				FB2Resurrection)	
//DECLARE_EVENT_CLASS(DeliveryClearStage,					FB2ClearStageInfo)
//DECLARE_EVENT_CLASS(DeliveryFailStage)
//DECLARE_EVENT_CLASS(DeliverySweepStage,					FB2SweepStageInfo)
//DECLARE_EVENT_CLASS(DeliverySweepStageError, FB2SweepStageInfo, int32)
//
//DECLARE_EVENT_CLASS(DeliveryCheckChargePoints,			FB2CheckChargePointsPtr)
//DECLARE_EVENT_CLASS(DeliveryCheckBladePointCharge,		FB2BladePointCharge)
//DECLARE_EVENT_CLASS(DeliveryCheckPvpMatchPointCharge,	FB2PvpMatchPointCharge)
//DECLARE_EVENT_CLASS(DeliveryCheckTeamMatchPointCharge,	FB2TeamMatchPointCharge)
//DECLARE_EVENT_CLASS(DeliveryGetActInfo,					FB2OriginGetActInfoPtr)
//DECLARE_EVENT_CLASS(DeliveryGetStageInfo,				FB2OriginGetStageInfoPtr)
//DECLARE_EVENT_CLASS(DeliveryGetActClearReward,			FB2GetReceiveActClearReward)
//DECLARE_EVENT_CLASS(DeliveryGetActRepeatClearReward,	FB2GetReceiveActRepeatClearReward)
//DECLARE_EVENT_CLASS(DeliveryTokenToTagMatch,			const FString&)
//DECLARE_EVENT_CLASS(DeliveryGetMatchStatus,				FB2TagMatchStatus)
//DECLARE_EVENT_CLASS(DeliveryCancelMatchmaking)
//DECLARE_EVENT_CLASS(DeliveryTagMatchResult,				FB2TagMatchResult)
//DECLARE_EVENT_CLASS(DeliveryGetTeamBattleStatus,		FB2TeamBattleStatus)
//DECLARE_EVENT_CLASS(DeliveryUpdateTeamBattleEntry,		FB2TeamBattleEntryPtr)
//DECLARE_EVENT_CLASS(DeliveryTeamMatchMaking,			FB2TeamMatchMaking)
//DECLARE_EVENT_CLASS(DeliveryTeamMatchResult,			FB2TeamMatchResult)
//DECLARE_EVENT_CLASS(DeliveryEnhanceTeamFormation,		FB2EnhanceTeamFormation)
//DECLARE_EVENT_CLASS(DeliveryReceiveGetWing,				const FB2Wing&)
//DECLARE_EVENT_CLASS(DeliveryReceiveEnhanceWing,			FB2EnhanceWing)
//DECLARE_EVENT_CLASS(DeliveryReceiveUpgradeWing,			FB2UpgradeWing)
//DECLARE_EVENT_CLASS(DeliveryReceiveEnhanceWingOption,	FB2EnhanceWingOption)
//DECLARE_EVENT_CLASS(DeliveryReceiveSetWingVisible,		FB2SetWingVisible)
//DECLARE_EVENT_CLASS(DeliveryReceiveSetHelmetVisible,	FB2SetHelmetVisible)
//DECLARE_EVENT_CLASS(DeliveryGetDonationEvent,			FB2GetDonationEventPtr)
//DECLARE_EVENT_CLASS(DeliveryMakeDonation,				FB2MakeDonationPtr)
//DECLARE_EVENT_CLASS(DeliveryDonationPointReward,		FB2DonationPointRewardPtr)
//DECLARE_EVENT_CLASS(DeliveryGetFairy,					FB2GetFairyPtr)
//DECLARE_EVENT_CLASS(DeliveryLevelupFairy,				FB2LevelupFairyPtr)
//DECLARE_EVENT_CLASS(DeliveryLevelupFairyError,			FB2LevelupFairyPtr)
//DECLARE_EVENT_CLASS(DeliveryBlessFairy,					FB2BlessFairyPtr)
//DECLARE_EVENT_CLASS(DeliveryReceiveFairyGift,			FB2ReceiveFairyGiftPtr)
//DECLARE_EVENT_CLASS(DeliveryGetCollectionItem,			FB2CollectionItemPtr)
//DECLARE_EVENT_CLASS(DeliveryRewardCollectionItem,		FB2RewardCollectionItemPtr)
//DECLARE_EVENT_CLASS(DeliveryRewardCollectionSetItem,	FB2RewardCollectionSetItemPtr)
//DECLARE_EVENT_CLASS(DeliveryResponseFindAccount,		FB2FindAccountPtr)
//
//DECLARE_EVENT_CLASS(HandleServerError4032)
//DECLARE_EVENT_CLASS(HandleServerError4015,				FB2StartStageInfo)	
//DECLARE_EVENT_CLASS(HandleServerError4030,				FB2StartStageInfo)
//DECLARE_EVENT_CLASS(HandleServerError910)
//DECLARE_EVENT_CLASS(HandleServerError911)
//DECLARE_EVENT_CLASS(HandleServerError901)
//DECLARE_EVENT_CLASS(HandleServerError1012)
//DECLARE_EVENT_CLASS(HandleServerError101)
//DECLARE_EVENT_CLASS(HandleServerError113)
//DECLARE_EVENT_CLASS(HandleServerError4207)
//DECLARE_EVENT_CLASS(HandleServerError919)
//DECLARE_EVENT_CLASS(HandleServerError932)
//DECLARE_EVENT_CLASS(HandleServerError5503)
//DECLARE_EVENT_CLASS(HandleServerError5513)
//DECLARE_EVENT_CLASS(HandleServerError6516)
//DECLARE_EVENT_CLASS(HandleServerError7413)
//
//DECLARE_EVENT_CLASS(HandleServerError7237)
//
//DECLARE_EVENT_CLASS(HandleServerError7250, FB2ResponseIAPPreparePurchasePtr)
//DECLARE_EVENT_CLASS(HandleServerError7253)
//DECLARE_EVENT_CLASS(HandleServerError7256)
//DECLARE_EVENT_CLASS(HandleServerInAppPurchaseError,		int32)
//
//DECLARE_EVENT_CLASS(HandleServerError8005)
//
//DECLARE_EVENT_CLASS(DeliveryReceiveRenewalMagicShopError, FB2ResponseRenewalMagicShopPtr)
//DECLARE_EVENT_CLASS(DeliveryReceivePurchaseMagicShopError, FB2ResponsePurchaseMagicShopPtr)
//
//DECLARE_EVENT_CLASS(HandleServerErrorGoodsShortage,		const uint32, const EGoodsButtonType)
//DECLARE_EVENT_CLASS(HandleServerError4018)
//
//
////DECLARE_EVENT_CLASS(SuccessToMatchmake,					data_trader::ChannelPtr, uint32, const TArray<FB2RoomParticipantPtr>&)
//DECLARE_EVENT_CLASS(ReceivedMyNetId,					uint32, int64, const FB2FindAccountInfoPtr&)
//DECLARE_EVENT_CLASS(ReceivedExchangeInfomations,		uint32, EPCClass, EPCClass, uint32, uint32, uint32, const FGuid&, int64)
//DECLARE_EVENT_CLASS(ReceivedAckExchangeInformations)
//DECLARE_EVENT_CLASS(ReceivedOtherNetId,					uint32, int64, const FB2FindAccountInfoPtr&)
//DECLARE_EVENT_CLASS(ReceivedNotifyLeave,				uint32)
//DECLARE_EVENT_CLASS(ReceivedRoomLeaveResponse)
//DECLARE_EVENT_CLASS(ReceivedRoomCloseResponse,			const FString&, int32)
//
//DECLARE_EVENT_CLASS(ReceivedStandalonePlay, FB2TagMatchMakingPtr)
//
//DECLARE_EVENT_CLASS(BroadcastRoomMessage,				uint32, const FString&, bool)
//DECLARE_EVENT_CLASS(ReceivedNotifyChangeHost,			int32, int32)
//
//DECLARE_EVENT_CLASS(ReceivedNotifyInviteMessage,		const FString&, const FString&, const FString&, int32, int32)
//
//DECLARE_EVENT_CLASS(ReceivedNotifyUpdateRoomProperty,	const FString&, const TArray<b2network::B2RoomPropertyPtr>&)
//
//DECLARE_EVENT_CLASS(MatchGameStartEventSceneEnd)
//
//DECLARE_EVENT_CLASS(StageEventDirectorFinishShowEvent, int32)
//
//
//DECLARE_EVENT_CLASS(ReceivedNetId,						uint32)
//DECLARE_EVENT_CLASS(ReceivedNotifyResult, ENetMatchResult, uint8)
//DECLARE_EVENT_CLASS(ReceivedMovement,					uint32, FVector,FVector,FRotator, float)
//DECLARE_EVENT_CLASS(ReceivedRotation,					uint32, FRotator)
//DECLARE_EVENT_CLASS(ReceivedAttack,						uint32, FVector,FRotator)
//DECLARE_EVENT_CLASS(ReceivedStopAttack,					uint32, FVector,FRotator)
//DECLARE_EVENT_CLASS(ReceivedGuard,						uint32, FVector,FRotator,uint32,uint32,uint8,uint8)
//DECLARE_EVENT_CLASS(ReceivedStopGuard,					uint32, FVector,FRotator)
//DECLARE_EVENT_CLASS(ReceivedSkillAttack,				uint32, uint8,uint8,FVector,FRotator)
//DECLARE_EVENT_CLASS(ReceivedTagPlayer,					uint32, uint8,uint8,FVector,FRotator)
//DECLARE_EVENT_CLASS(ReceivedComboEnd,					uint32, uint8, bool)
//DECLARE_EVENT_CLASS(ReceivedTakeDamage,					uint32, uint32, float, uint32, int32, int32)
//DECLARE_EVENT_CLASS(ReceivedRequestDamage,				uint32, uint32, float, uint32)
//DECLARE_EVENT_CLASS(ReceivedHealth,						uint32, float, uint32)
//DECLARE_EVENT_CLASS(ReceivedUpdateTarget,				uint32, uint32, float)
//DECLARE_EVENT_CLASS(ReceivedCounterAttack,				uint32, FVector, FRotator)
//DECLARE_EVENT_CLASS(ReceivedMobAttack,					uint32, FVector, FRotator, bool, uint32)
//DECLARE_EVENT_CLASS(ReceivedResurrect,					uint32, FVector, FRotator, TMap<EResurrectBuffType, uint8>)
//DECLARE_EVENT_CLASS(ReceivedRaidNotifyResult,			bool, uint32, TArray<TPair<int32, int32>> const &)
//DECLARE_EVENT_CLASS(ReceivedRaidResult,					FB2ResponseRaidResultPtr)
//DECLARE_EVENT_CLASS(HandleServerError4015InRaid)
//DECLARE_EVENT_CLASS(HandleServerError4015ByRoomJoin)
//DECLARE_EVENT_CLASS(HandleServerError4015ForSelectCharacter, TArray<b2network::B2InventoryCountServerInfoPtr>)
//DECLARE_EVENT_CLASS(ReceivedGetRaidCount,				uint8)
//DECLARE_EVENT_CLASS(ReceivedEtherActivated,				uint32, uint32, uint32)
//DECLARE_EVENT_CLASS(ReceivedDie,						uint32, uint32, int32)
//DECLARE_EVENT_CLASS(ReceivedBuffActivated,				uint32, uint32, uint8, float, float, bool, bool, TArray<float>*)
//DECLARE_EVENT_CLASS(ReceivedSetAnimState,				uint32, const FString&)
//DECLARE_EVENT_CLASS(DeliveryRaidResurrection,			FB2RaidResurrection)
//DECLARE_EVENT_CLASS(DeliveryGetRaid,					FB2ResponseGetRaidPtr)
//DECLARE_EVENT_CLASS(DeliveryStartRaid)
//DECLARE_EVENT_CLASS(DeliveryRaidMatchmaking,			FB2ResponseRaidMatchmakingPtr)
//DECLARE_EVENT_CLASS(DeliveryRaidMatchmakingError)
//DECLARE_EVENT_CLASS(DeliveryRoomJoin,					FB2ResponseRoomJoinPtr)
//DECLARE_EVENT_CLASS(DeliveryRoomJoinError)
//DECLARE_EVENT_CLASS(ReceivedConsumeControlSanctuary,	int32)
//DECLARE_EVENT_CLASS(ReceivedRespawnControlSanctuary,	int32)
//DECLARE_EVENT_CLASS(ReceivedDrinkControlSanctuary,		uint32, int32)
//DECLARE_EVENT_CLASS(ReceiveRoomPlayerTeamInfo,			TMap<uint32, uint8>&)
//DECLARE_EVENT_CLASS(ReceivedChangeDamageNum,			uint32, int32)
//DECLARE_EVENT_CLASS(ReceivedRaidNotifyTimeEnd,			uint32)
//DECLARE_EVENT_CLASS(ReceivedSyncAnimStateIndex, uint32, FNetSyncAnimInfo&)
//DECLARE_EVENT_CLASS(ReceivedQteNext, uint32)
//DECLARE_EVENT_CLASS(ReceivedSpawnEnd, uint32)
//
//
//
//DECLARE_EVENT_CLASS(ReceivedPrerequisiteToStart,		int32, const FString&, TArray<FB2NetMatchPeerInfo> const&)
//DECLARE_EVENT_CLASS(ReceivedAckPrerequisiteToStart,		uint32)
//DECLARE_EVENT_CLASS(ReceivedPrepareToStart,				bool)
//DECLARE_EVENT_CLASS(ReceivedChangeRoomAccess,			uint8)
//DECLARE_EVENT_CLASS(ReceivedKickUser)
//DECLARE_EVENT_CLASS(ReceivedNoResponsePrerequisiteToStart)
//DECLARE_EVENT_CLASS(ReceivedRoomPlayerClass,			uint32, TArray<FB2NetMatchPeerInfo> const&)
//DECLARE_EVENT_CLASS(ReceivedAckRoomPlayerClass,			uint32, EPCClass)
//
//DECLARE_EVENT_CLASS(ReceivedSyncReadyToStartGame, uint32)
//DECLARE_EVENT_CLASS(ReceivedSyncStartGame)
//
//DECLARE_EVENT_CLASS(ReceivedConquestAreaState,			uint8, float, float)
//DECLARE_EVENT_CLASS(ReceivedControlState,				uint8, float, float, bool)
//DECLARE_EVENT_CLASS(ReceivedControlKillCount,			TMap<uint32, int32>*)
//DECLARE_EVENT_CLASS(ReceivedControlResult,				ETeamType)
//DECLARE_EVENT_CLASS(ReceivedControlTimer,				uint8, float)
//DECLARE_EVENT_CLASS(ReceivedControlEnemyLeaveAll)
//
//DECLARE_EVENT_CLASS(ReceivedAskSpawnComplete)
//DECLARE_EVENT_CLASS(ReceivedSpawnComplete,				uint32, bool)
//
//DECLARE_EVENT_CLASS(ReceiveRequestQTE, uint32)
//DECLARE_EVENT_CLASS(ReceiveResponseQTE, uint32, bool)
//DECLARE_EVENT_CLASS(ReceiveNotifyEndQTE, uint32)
//DECLARE_EVENT_CLASS(ReceiveQteEnable, uint32, uint8)
//DECLARE_EVENT_CLASS(ReceiveQteEnableEnd)
//DECLARE_EVENT_CLASS(ReceiveSpawnControlNpc)
//
//DECLARE_EVENT_CLASS(ReceiveControlChat, uint32, uint8, int32)
//
//DECLARE_EVENT_CLASS(DeliveryLevelUpSkill,				FB2LevelUpSkillInfo)
//DECLARE_EVENT_CLASS(DeliveryLevelUpSkillError)
//DECLARE_EVENT_CLASS(DeliveryResetSkill,					FB2ResetSkillInfo)
//DECLARE_EVENT_CLASS(DeliveryBuySkillPoint,				FB2BuySkillPointInfo)
//DECLARE_EVENT_CLASS(DeliverySetUsingSkill,				FB2SetUsingSkillInfo)
//DECLARE_EVENT_CLASS(DeliveryCharacterNickName)
//DECLARE_EVENT_CLASS(DeliveryChangeNickName,				FB2UpdateNicknamePtr)
//DECLARE_EVENT_CLASS(DeliveryUserNickName)
//
//DECLARE_EVENT_CLASS(DeliveryEnhanceRankNode,			FB2EnhanceRankNode)
//
//DECLARE_EVENT_CLASS(DeliveryGetChatChannel,				FB2GetChatChannelResult)
//DECLARE_EVENT_CLASS(DeliveryChangeChatChannel)
//
//DECLARE_EVENT_CLASS(DeliveryGetMailList,				FB2MailList)
//DECLARE_EVENT_CLASS(DeliveryOpenMail,					int64, FB2ResponseOpenMailPtr)
//DECLARE_EVENT_CLASS(DeliveryOpenMailError,				int32, FB2ResponseOpenMailPtr)
//DECLARE_EVENT_CLASS(DeliveryOpenAllMail,				FB2OpenAllMail)		
//DECLARE_EVENT_CLASS(DeliveryOpenAllMailError,			int32)
//
//DECLARE_EVENT_CLASS(DeliveryGeneralLotteryItemResult,		FB2Item, ESummonItemDifficulty)
//
//DECLARE_EVENT_CLASS(CacheSendAskFriendAccountId,		int64)
//DECLARE_EVENT_CLASS(RemoveSendAskFriendAccountId,		int64)
//DECLARE_EVENT_CLASS(DeliveryAskFriend)
//DECLARE_EVENT_CLASS(DeliveryAcceptAskFriend)
//DECLARE_EVENT_CLASS(DeliveryRejectAskFriend)
//DECLARE_EVENT_CLASS(DeliveryGetFriendList,				FB2FriendList)
//DECLARE_EVENT_CLASS(DeliveryCancelAskFriend)
//DECLARE_EVENT_CLASS(DeliveryDeleteFriend,				FB2DeleteFriend)
//DECLARE_EVENT_CLASS(DeliveryGetAllSendAskFriend,		FBAllSendAskFriendList)
//DECLARE_EVENT_CLASS(DeliveryGetAllReceiveAskFriend,		FB2AllReceiveAskFriendList)
//DECLARE_EVENT_CLASS(DeliverySendSocialPoint,			FB2SendSocialPoint)
//DECLARE_EVENT_CLASS(DeliveryGetRecommendFriend,			FB2RecommendFriendList)
//DECLARE_EVENT_CLASS(DeliveryFindUser,					FB2FindUser)
//
//DECLARE_EVENT_CLASS(OnFriendSideMenuSelect, int32)
//
//DECLARE_EVENT_CLASS(DeliveryKakaoInviteFriend,			EKakaoInviteResultCode)
//
//DECLARE_EVENT_CLASS(DeliveryGetQuest,					FB2GetQuest)	
//DECLARE_EVENT_CLASS(DeliveryGetQuestUI,					FB2GetQuest)
//DECLARE_EVENT_CLASS(DeliveryAcceptQuest,				FB2AcceptQuest)
//DECLARE_EVENT_CLASS(DeliveryReceiveQuestReward,			FB2ReceiveQuestReward)
//DECLARE_EVENT_CLASS(DeliveryQuestReward,				Quest, TArray<b2network::B2RewardPtr>)
//DECLARE_EVENT_CLASS(DeliveryeGiveUpQuest,				FB2GiveUpQuestPtr)
//DECLARE_EVENT_CLASS(InformQuestFromYesterday,			TArray<int32>)
//DECLARE_EVENT_CLASS(OnQuestCompleteNotify,				int32)
//
//DECLARE_EVENT_CLASS(DeliveryGetCounterDundeon,			FB2GetCounterDungeon)
//
//DECLARE_EVENT_CLASS(TeamMatchChangeCharacterCam,		bool, int32)
//
//DECLARE_EVENT_CLASS(DeliveryStartCounterDungeon,		FB2ResponseStartCounterDungeon)
//DECLARE_EVENT_CLASS(DeliveryForceStartCounterDungeon,	FB2ResponseForceStartCounterDungeonPtr)
//DECLARE_EVENT_CLASS(DeliveryFinishCounterDungeon,		FB2FinishCounterDungeonInfo)
//DECLARE_EVENT_CLASS(DeliverySweepCounterDungeon,		FB2SweepCounterDungeonInfo)
//DECLARE_EVENT_CLASS(DeliveryCounterDungeonWeeklyReward, FB2CounterDungeonWeeklyReward)
//DECLARE_EVENT_CLASS(DeliveryBuyTicketCounterDungeon,	FB2CounterDungeonBuyTicket)
//
//DECLARE_EVENT_CLASS(DeliveryGetMission,					FB2GetMission)
//DECLARE_EVENT_CLASS(DeliveryReceiveMissionReward,		FB2ReceiveMissionReward)
//DECLARE_EVENT_CLASS(DeliveryReceiveMissionRewardError, const int32)
//DECLARE_EVENT_CLASS(DeliveryReceiveCompleteMissionReward, FB2ReceiveCompleteMissionReward)
//
//DECLARE_EVENT_CLASS(DeliveryReceiveCreateGuild,			FB2ReceiveCreateGuild)
//DECLARE_EVENT_CLASS(DeliveryReceiveGuildDetailInfo,		FB2ReceiveGuildInfo)
//DECLARE_EVENT_CLASS(DeliveryReceiveLeaveGuild,			FB2ResponseLeaveGuildPtr)
//DECLARE_EVENT_CLASS(DeliveryReceiveGetRecommendGuild,	FB2ReciveRecommenGuild)
//DECLARE_EVENT_CLASS(DeliveryReceiveAskJoinGuild)
//DECLARE_EVENT_CLASS(DeliveryReceiveAppointSubGuildMaster, FB2ReciveAppointSubGuildMasterInfo)
//DECLARE_EVENT_CLASS(DeliveryReceiveDismissSubGuildMaster, FB2ReciveDismissSubGuildMasterInfo)
//DECLARE_EVENT_CLASS(DeliveryReceiveKickGuildMember)
//DECLARE_EVENT_CLASS(DeliveryReceiveJoinGuildBattle)
//DECLARE_EVENT_CLASS(GuildMatchChangeCharacterCam,		bool, int32)
//DECLARE_EVENT_CLASS(DeliveryGetGuildBattleInfo,			FB2ResponseGetGuildBattlePtr)
//DECLARE_EVENT_CLASS(DeliveryStartGuildBattle,			FB2ResponseStartGuildBattlePtr)
//DECLARE_EVENT_CLASS(DeliverySetGuildBattleEntry,		FB2ResponseSetGuildBattlePtr)
//DECLARE_EVENT_CLASS(DeliveryFinishGuildBattle,			FB2ResponseFinishGuildBattlePtr)
//DECLARE_EVENT_CLASS(DeliverGetGuildInviteList,			FB2ResponseGetGuildInviteListPtr)
//DECLARE_EVENT_CLASS(DeliveryGetGuildMemberCandidates,	FB2ResponseGetGuildMemberCandidatesPtr)
//DECLARE_EVENT_CLASS(DeliveryInviteGuild)
//DECLARE_EVENT_CLASS(DeliveryRejectInviteGuild)
//DECLARE_EVENT_CLASS(DeliveryAcceptInviteGuild)
//DECLARE_EVENT_CLASS(DeliveryGuildBattleHistory,			FB2ResponseGuildBattleHistoryPtr)
//DECLARE_EVENT_CLASS(DeliveryUpdateGuild,				FB2ResponseUpdateGuildPtr)
//DECLARE_EVENT_CLASS(DeliveryUpdateGuildNotice,			FB2ResponseUpdateGuildNoticePtr)
//DECLARE_EVENT_CLASS(DeliveryFindGuild,					FB2ResponseFindGuildPtr)
//DECLARE_EVENT_CLASS(DeliverytGetGuildRankingList,		FB2ResponseGetGuildRankingListPtr)
//DECLARE_EVENT_CLASS(DeliverytGuildBattleSeasonReward,	FB2ResponseGuildBattleSeasonRewardPtr)
//
//DECLARE_EVENT_CLASS(DeliverytLevelUpGuildSkill,			FB2ResponseLevelUpGuildSkillPtr)
//DECLARE_EVENT_CLASS(DeliverytBuyGuildSkill,				FB2ResponseBuyGuildSkillPtr)
//DECLARE_EVENT_CLASS(DeliverytObtainGuildMaster, FB2ResponseObtainGuildMasterPtr)
//DECLARE_EVENT_CLASS(DeliverytHandOverGuildMaster, FB2ResponseHandOverGuildMasterPtr)
//
//DECLARE_EVENT_CLASS(DeliveryGuildBattleTurnResult,		FB2ResponseGuildBattleTurnResultPtr)
//DECLARE_EVENT_CLASS(DeliveryGuildBattleTurnReward,		FB2ResponseGuildBattleTurnRewardPtr)
//DECLARE_EVENT_CLASS(GuildStateChangeEvent,				int32)
//DECLARE_EVENT_CLASS(GuildBuffSettting,					const TArray<b2network::B2GuildSkillPtr>&)
//DECLARE_EVENT_CLASS(DeliveryuildDonation,				FB2ResponseGuildDonationPtr)
//DECLARE_EVENT_CLASS(DeliveryuildDonatioError)
//
//DECLARE_EVENT_CLASS(DeliveryCheckReachPower,			FB2ResponseCheckReachPowerPtr)
//DECLARE_EVENT_CLASS(DeliveryTrackingPowerDetail,		FB2ResponseTrackingPowerDetailPtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetFriendRanking,			FB2ResponseGetFriendRankingsPtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetHeroTower,				FB2ResponseGetHeroTowerPtr)
//DECLARE_EVENT_CLASS(DeliveryStartHeroTower,				FB2ResponseStartHeroTowerPtr)
//DECLARE_EVENT_CLASS(DeliveryFinishHeroTower,			FB2ResponseFinishHeroTowerPtr)
//DECLARE_EVENT_CLASS(DeliveryAbandonHeroTower,			FB2ResponseAbandonHeroTowerPtr)
//DECLARE_EVENT_CLASS(DeliverySweepHeroTower,				FB2ResponseSweepHeroTowerPtr)
//DECLARE_EVENT_CLASS(DeliveryBuyHeroTowerTicket,			FB2ResponseBuyHeroTowerTicketPtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetAssaultBattleStatus,		FB2ResponseGetAssaultBattleStatusPtr)
//DECLARE_EVENT_CLASS(DeliveryAssaultResult,				FB2ResponseAssaultResultPtr)
//DECLARE_EVENT_CLASS(ReceivedBattleGradeInfo,			uint32, int32, int32)
//DECLARE_EVENT_CLASS(ReceivedOtherToken,					uint32, const FString&)
//
//DECLARE_EVENT_CLASS(DeliveryGetLotteryShop,				FB2ResponseGetLotteryShopPtr)
//DECLARE_EVENT_CLASS(DeliveryDrawShopLottery,			FB2ResponseDrawShopLotteryPtr)	
//DECLARE_EVENT_CLASS(DeliveryGetGeneralShop,				FB2ResponseGetGeneralShopPtr)
//DECLARE_EVENT_CLASS(DeliveryBuyShopProduct,				FB2ResponseBuyShopProductPtr)
//DECLARE_EVENT_CLASS(DeliveryRewardShopMileage,			FB2ResponseRewardShopMileagePtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetMagicShop,				FB2ResponseGetMagicShopPtr)
//DECLARE_EVENT_CLASS(DeliveryRenewalMagicShop,			FB2ResponseRenewalMagicShopPtr)
//DECLARE_EVENT_CLASS(DeliveryPurchaseMagicShop,			FB2ResponsePurchaseMagicShopPtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetLatestEventData,			FB2ResponseGetLatestEventDataPtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetPackageShop,						FB2ResponseGetPackageShopPtr)
//DECLARE_EVENT_CLASS(DeliveryGetPeriodPackageState,				FB2ResponseGetPeriodPackageStatePtr)
//DECLARE_EVENT_CLASS(DeliveryResponseRewardLevelUpPackage,		FB2ResponseRewardLevelUpPackagePtr)
//DECLARE_EVENT_CLASS(DeliveryResponseRewardReachLevelPackage,	FB2ResponseRewardReachLevelPackagePtr)
//DECLARE_EVENT_CLASS(DeliveryResponseRewardFlatRatePackageBonus,	FB2ResponseRewardFlatRatePackageBonusPtr)
//DECLARE_EVENT_CLASS(DeliveryResponseGetContentsModeState,		FB2ResponseGetContentsModeStatePtr)
//
//DECLARE_EVENT_CLASS(DeliveryResponseGetDuelModeInfo,		FB2ResponseGetDuelModeInfoPtr)
//DECLARE_EVENT_CLASS(DeliveryResponseGetChallengeModeInfo,	FB2ResponseGetChallengeModeInfoPtr)
//
//DECLARE_EVENT_CLASS(DeliveryResponseGetHallOfFame,			FB2ResponseGetHallOfFamePtr)
//DECLARE_EVENT_CLASS(DeliveryResponsePraiseTargetRanker,		FB2ResponsePraiseTargetRankerPtr)
//
//DECLARE_EVENT_CLASS(DeliveryEnhanceRelic,				FB2ResponseEnhanceRelicPtr)
//DECLARE_EVENT_CLASS(DeliveryPromotionRelic,				FB2ResponsePromotionRelicPtr)
//DECLARE_EVENT_CLASS(DeliveryGetAccountRelic,			FB2ResponseGetAccountRelicPtr)
//DECLARE_EVENT_CLASS(DeliveryDailyAttendance,			FB2ReceiveDailyAttendance)
//
//DECLARE_EVENT_CLASS(DeliveryGetItemRandomOption,		FB2ResponseGetItemRandomOptionPtr)
//DECLARE_EVENT_CLASS(DeliveryGenerateItemRandomOption,	FB2ResponseGenerateItemRandomOptionPtr)
//DECLARE_EVENT_CLASS(DeliveryApplyItemRandomOption,		FB2ResponseApplyItemRandomOptionPtr)
//DECLARE_EVENT_CLASS(DeliveryDiscardItemRandomOption,	FB2ResponseDiscardItemRandomOptionPtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetItemRandomQuality, FB2ResponseGetItemRandomQualityPtr)
//DECLARE_EVENT_CLASS(DeliveryGenerateItemRandomQuality, FB2ResponseGenerateItemRandomQualityPtr)
//DECLARE_EVENT_CLASS(DeliveryApplyItemRandomQuality, FB2ResponseApplyItemRandomQualityPtr)
//DECLARE_EVENT_CLASS(DeliveryDiscardItemRandomQuality, FB2ResponseDiscardItemRandomQualityPtr)
//
//DECLARE_EVENT_CLASS(DeliveryCheckRedDot, FB2MessageInfoResponseCheckRedDotPtr)
//DECLARE_EVENT_CLASS(DeliveryModifyItemNewFlag)
//DECLARE_EVENT_CLASS(DeliveryModifyCostumeNewFlag)
//
//DECLARE_EVENT_CLASS(StopRelicAutoEnchant)
//
//DECLARE_EVENT_CLASS(DeliveryMatchMaking,				FB2TagMatchMakingPtr)
//
//DECLARE_EVENT_CLASS(NotifyInputTouchBeganInViewPort)
//
//DECLARE_EVENT_CLASS(DeliveryOpenDuelRewardBox,			FB2OpenDuelRewardBoxPtr)
//DECLARE_EVENT_CLASS(DeliveryGetDuelRewardBoxInfo,		FB2GetDuelRewardBoxInfoPtr)
//
//
//DECLARE_EVENT_CLASS(DeliveryEventAttendance, FB2ResponseEventAttendancePtr)
//DECLARE_EVENT_CLASS(DeliveryBeginnerAttendance, FB2ResponseBeginnerAttendancePtr)
//DECLARE_EVENT_CLASS(DeliveryUpdateDailyPlayTime, FB2ResponseUpdateDailyPlayTimePtr)
//DECLARE_EVENT_CLASS(DeliveryDailyPlayTimeReward, FB2ResponseReceiveDailyPlayTimeRewardPtr)
//
//
//DECLARE_EVENT_CLASS(DeliveryGetDuelRewardBoxGainableList,		FB2GetDuelRewardBoxGainableListPtr)
//DECLARE_EVENT_CLASS(DeliveryAssaultMvpReward,					FB2AssaultMvpRewardPtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetTutorialProgress, FB2ResponseGetTutorialProgressPtr)
//DECLARE_EVENT_CLASS(DeliverySetTutorialProgress, FB2ResponseSetTutorialProgressPtr)
//DECLARE_EVENT_CLASS(DeliveryUpdateTutorialProgress, FB2ResponseUpdateTutorialProgressPtr)
//DECLARE_EVENT_CLASS(DeliverySkipTutorialProgress, FB2ResponseSkipTutorialProgressPtr)
//DECLARE_EVENT_CLASS(DeliveryDrawTutorialItem, FB2ResponseDrawTutorialItemPtr)
//DECLARE_EVENT_CLASS(DeliveryPrepareTutorial, FB2ResponsePrepareTutorialPtr)
//DECLARE_EVENT_CLASS(DeliveryGiveTutorialReward, FB2ResponseGiveTutorialRewardPtr)
//DECLARE_EVENT_CLASS(CahcedCheckAndProgressTutorial)
//
//DECLARE_EVENT_CLASS(DeliveryAskGuildSupport, FB2ResponseAskGuildSupportPtr)
//DECLARE_EVENT_CLASS(DeliveryGuildSupport, FB2ResponseGuildSupportPtr)
//DECLARE_EVENT_CLASS(DeliveryGetGuildChat, FB2ResponseGetGuildChatPtr)
//
//DECLARE_EVENT_CLASS(NotifyGuildChatBadge, int32)
//
//DECLARE_EVENT_CLASS(DeliveryGetUnitySkillMission, FB2ResponseGetUnitySkillMissionPtr)
//DECLARE_EVENT_CLASS(DeliveryReceiveUnitySkillReward, FB2ResponseUnitySkillRewardPtr)
//DECLARE_EVENT_CLASS(DeliveryAwakenUnitySkillMission, FB2ResponseAwakenUnitySkillMissionPtr)
//DECLARE_EVENT_CLASS(DeliveryAwakenUnitySkill, FB2ResponseAwakenUnitySkillPtr)
//
//DECLARE_EVENT_CLASS(DeliveryCheckModeOpen, FB2ResponseCheckModeOpenPtr)
//DECLARE_EVENT_CLASS(DeliveryRestReward, FB2ResponseRestRewardPtr)
//DECLARE_EVENT_CLASS(DeliveryGetGuildMember, FB2ResponseGuildMemberPtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetEvent, FB2ResponseGetEventPtr)
//DECLARE_EVENT_CLASS(DeliveryLevelUpEventReward, FB2ResponseReceiveLevelUpEventRewardPtr)
//DECLARE_EVENT_CLASS(DeliveryStageClearEventReward, FB2ResponseReceiveStageClearEventRewardPtr)
//DECLARE_EVENT_CLASS(DeliveryRewardPointShopEvent, FB2ResponseRewardPointShopEventdPtr)
//DECLARE_EVENT_CLASS(DeliveryRewardTenLotteryEvent, FB2ResponseRewardTenLotteryEventPtr)
//DECLARE_EVENT_CLASS(DeliveryRewardGemSpendEvent, FB2ResponseRewardSpendGemEventPtr)
//DECLARE_EVENT_CLASS(DeliveryRewardGemPurchaseEvent, FB2ResponseReceiveGemPurchaseEventRewardPtr)
//DECLARE_EVENT_CLASS(DeliveryRewardTenLotterySavingsEvent, FB2ResponseRewardTenLotterySavingsEventPtr)
//DECLARE_EVENT_CLASS(DeliveryRollDiceEvent, FB2ResponseRollDiceEventPtr)
//DECLARE_EVENT_CLASS(DeliveryBuyDiceEventPoint, FB2ResponseBuyDiceEventPointPtr)
//DECLARE_EVENT_CLASS(DeliveryRewardDiceEventMileage, FB2ResponseRewardDiceEventMileagePtr)
//
//DECLARE_EVENT_CLASS(DeliveryPointEventReward, FB2ResponseReceivePointEventRewardPtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetAccountCostume, FB2GetAccountCostumePtr)
//DECLARE_EVENT_CLASS(DeliveryEquipCostume, FB2EquipCostumePtr)
//DECLARE_EVENT_CLASS(DeliveryUnequipCostume, FB2UnequipCostumePtr)
//DECLARE_EVENT_CLASS(DeliveryEnhanceCostume, FB2EnhanceCostumePtr)
//DECLARE_EVENT_CLASS(DeliveryDismantleCostumes, FB2DismantleCostumesPtr)
//DECLARE_EVENT_CLASS(DeliveryLockCostumes, FB2LockCostumesPtr)
//DECLARE_EVENT_CLASS(DeliveryUnlockCostumes, FB2UnlockCostumesPtr)
//DECLARE_EVENT_CLASS(DeliveryGetCostumeShop, FB2GetCostumeShopPtr)
//DECLARE_EVENT_CLASS(DeliveryPurchaseCostume, FB2PurchaseCostumePtr)
//DECLARE_EVENT_CLASS(DeliverySetCostumeVisible, FB2SetCostumeVisiblePtr)
//DECLARE_EVENT_CLASS(DeliveryAddCostumeToInventory, FB2AddCostumeToInventoryPtr)
//
//DECLARE_EVENT_CLASS(DeliveryDismantleCostumesFail)
//DECLARE_EVENT_CLASS(DeliveryEnhanceCostumesFail)
//
//// aether
//DECLARE_EVENT_CLASS(DeliveryGetAccountEther, FB2GetAccountEtherPtr)
//DECLARE_EVENT_CLASS(DeliveryMountEther, FB2MountEtherPtr)
//DECLARE_EVENT_CLASS(DeliveryUnmountEther, FB2UnmountEtherPtr)
//DECLARE_EVENT_CLASS(DeliverySellEther, FB2SellEtherPtr)
//DECLARE_EVENT_CLASS(DeliverySellEtherError)
//DECLARE_EVENT_CLASS(DeliveryDecompositionEther, FB2DecompositionEtherPtr)
//DECLARE_EVENT_CLASS(DeliveryDecompositionEtherError)
//DECLARE_EVENT_CLASS(DeliveryEnhanceEther, FB2EnhanceEtherPtr)
//DECLARE_EVENT_CLASS(DeliveryEnhanceEther2630, FB2EnhanceEtherPtr)
//DECLARE_EVENT_CLASS(DeliveryLockEther, FB2LockEtherPtr)
//DECLARE_EVENT_CLASS(DeliveryUnlockEther, FB2UnlockEtherPtr)
//DECLARE_EVENT_CLASS(DeliverySelectEtherDefenseOption, FB2SelectEtherDefenseOptionPtr)
//DECLARE_EVENT_CLASS(DeliverySelectEtherUnitySkillOption, FB2SelectEtherUnitySkillOptionPtr)
//DECLARE_EVENT_CLASS(DeliveryModifyEtherNewFlag, FB2ModifyEtherNewFlagPtr)
//DECLARE_EVENT_CLASS(DeliveryExtendEtherInventory, FB2ExtendEtherInventoryPtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetContentsOpenStatus, FB2ResponseGetContentsOpenStatusPtr)
//
//DECLARE_EVENT_CLASS(DeliveryKakaoMessageProcessing , FB2ResponseKakaoMessageProcessingPtr)
//DECLARE_EVENT_CLASS(DeliveryKakaoAccountConversionPrepare, FB2ReciveKakaoAccountConversionPrepare)
//DECLARE_EVENT_CLASS(DeliveryKakaoAccountConversionComplete, FB2ReciveKakaoAccountConversionComplete)
//DECLARE_EVENT_CLASS(DeliveryGetFeatureMission, FB2ReciveGetFeaturedMission)
//
//DECLARE_EVENT_CLASS(DeliveryPlatformFriendInviteInfo, FB2ResponseFriendInviteRewardInfoPtr)
//DECLARE_EVENT_CLASS(DeliveryFriendInviteCountUp, FB2ResponseFriendInviteCountUpPtr)
//DECLARE_EVENT_CLASS(DeliveryFriendJoinerCountUp, FB2ResponseFriendJoinerCountUpPtr)
//DECLARE_EVENT_CLASS(DeliveryFriendInviteReward, FB2ResponseFriendInviteRewardPtr)
//
//DECLARE_EVENT_CLASS(DeliveryIAPGetMarketProduct, FB2ResponseIAPGetMarketProductPtr)
//DECLARE_EVENT_CLASS(DeliveryIAPPreparePurchase, FB2ResponseIAPPreparePurchasePtr)
//DECLARE_EVENT_CLASS(DeliveryIAPVerifyPurchase, FB2ResponseIAPVerifyPurchasePtr)
//DECLARE_EVENT_CLASS(DeliveryIAPCancelPurchase, FB2ResponseIAPCancelPurchasePtr)
//DECLARE_EVENT_CLASS(DeliveryIAPFinishPurchase, FB2ResponseIAPFinishPurchasePtr)
//
//DECLARE_EVENT_CLASS(DeliveryGetSeasonMission, FB2ResponseGetSeasonMissionPtr)
//DECLARE_EVENT_CLASS(DeliveryRewardSeasonMission, FB2ResponseRewardSeasonMissionPtr)
//DECLARE_EVENT_CLASS(DeliveryAttendSeasonMission, FB2ResponseAttendSeasonMissionPtr)
//DECLARE_EVENT_CLASS(SelectSeasonEventTab, int32)
//DECLARE_EVENT_CLASS(SeasonEventGotoLinkScene, int32, int32)
//
//DECLARE_EVENT_CLASS(DeliveryBuyHotTimeBuff, FB2ResponseBuyHotTimeBuffPtr)
//DECLARE_EVENT_CLASS(DeliveryReportUser)
//
//DECLARE_EVENT_CLASS(DeliveryBlcokUser, FB2ResponseBlockChatPtr)
//DECLARE_EVENT_CLASS(DeliveryUnBlcokUser)
//DECLARE_EVENT_CLASS(DeliveryGetBlockChatList, FB2ResponseGetBlockChatListPtr)
//
//DECLARE_EVENT_CLASS(DeliveryClientStateReport, FB2ResponseClientStateReportPtr)
//
//DECLARE_EVENT_CLASS(DeliveryUnityAwakeFail)
//DECLARE_EVENT_CLASS(DeliveryGetChangeItemCharacterTypeCost, FB2ResponseGetChangeItemCharacterTypeCostPtr)
//DECLARE_EVENT_CLASS(DeliveryChangeItemCharacterType, FB2ResponseChangeItemCharacterTypePtr)
//
//DECLARE_EVENT_CLASS(UpdateFairyStatus)
//DECLARE_EVENT_CLASS(UpdateFairyLevelUp, EFairyType, bool)
//DECLARE_EVENT_CLASS(UpdateFairyLevelUpError, int32)
//DECLARE_EVENT_CLASS(UpdateFairyBless, EFairyType)
//DECLARE_EVENT_CLASS(UpdateFairyAutoLevelUp, bool)
//
//DECLARE_EVENT_CLASS(DeliveryJackpotFairy, FB2ServerCmdPtr)
//
////dimension
//DECLARE_EVENT_CLASS(DeliveryGetDimension, FB2ResponseGetDimensionPtr)
//DECLARE_EVENT_CLASS(DeliveryStartDimension, FB2ResponseStartDimensionPtr)
//DECLARE_EVENT_CLASS(DeliveryFinishDimension, FB2ResponseFinishDimensionPtr)
//DECLARE_EVENT_CLASS(DimensionBuffCheck)
//
//// Totem
//DECLARE_EVENT_CLASS(EnterTotemSmeltingMode, int64)
//DECLARE_EVENT_CLASS(OpenTotemSmeltingInfoPopup, int64)
//DECLARE_EVENT_CLASS(ReturnTotemInventory, int64)
//DECLARE_EVENT_CLASS(LobbyReqSmeltingTotem, int64, int32, int32, bool)
//DECLARE_EVENT_CLASS(DeliveryGetAccountTotem, FB2GetAccountTotemPtr)
//DECLARE_EVENT_CLASS(DeliveryDismantleTotems, FB2DismantleTotemsPtr)
//DECLARE_EVENT_CLASS(DeliveryEquipTotems, FB2EquipTotemsPtr)
//DECLARE_EVENT_CLASS(DeliveryUnequipTotems, FB2UnequipTotemsPtr)
//DECLARE_EVENT_CLASS(DeliveryLockTotems, FB2LockTotemsPtr)
//DECLARE_EVENT_CLASS(DeliveryUnlockTotems, FB2UnlockTotemsPtr)
//DECLARE_EVENT_CLASS(DeliveryRefineTotem, FB2RefineTotemPtr)
//DECLARE_EVENT_CLASS(DeliverySellTotems, FB2SellTotemsPtr)
//DECLARE_EVENT_CLASS(DeliveryModifyTotemNewFlag, FB2ModifyTotemNewFlagPtr)
//DECLARE_EVENT_CLASS(DeliveryAddTotemToInventory, FB2AddTotemToInventoryPtr)
//DECLARE_EVENT_CLASS(SuccesGetAccountTotem)
//DECLARE_EVENT_CLASS(LobbyReqToggleTotemLock, int64, bool)
////DECLARE_EVENT_CLASS(UpdateTotemInfoPopupControl, FB2Totem)
////DECLARE_EVENT_CLASS(UpdateTotemSmeltingResult, bool, FB2Totem)
////DECLARE_EVENT_CLASS(LobbyReqSmeltingCompleteTotem)
//
////ItemForge
//DECLARE_EVENT_CLASS(DeliveryGetItemForgeList, FB2ResponseGetItemForgeListPtr)
//DECLARE_EVENT_CLASS(DeliveryTryItemForge, FB2ResponseTryItemForgePtr)
//DECLARE_EVENT_CLASS(DeliverySelectItemForge, FB2Item&)
//
////Sealbox
//DECLARE_EVENT_CLASS(DeliveryUnsealBox, FB2ResponseUnsealBoxPtr)
//DECLARE_EVENT_CLASS(SealBoxSelectChar, int32)
//DECLARE_EVENT_CLASS(SealBoxCardFlip, int32, int32)
//
//// Replay
//DECLARE_EVENT_CLASS(StartPVPReplay, FString, int32, int32)
//
///*
//	Events for DLC front
//*/
//
//DECLARE_EVENT_CLASS(DLCFrontLaunchBladeIIGame)