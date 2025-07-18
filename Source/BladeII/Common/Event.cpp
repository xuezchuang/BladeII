
#include "Event.h"



DEFINITION_EVENT_CLASS(StageBegin)
DEFINITION_EVENT_CLASS(CharacterBeginPlay, ABladeIICharacter*)
DEFINITION_EVENT_CLASS(CancelCastAreaDamage, ABladeIICharacter*)
DEFINITION_EVENT_CLASS(TargetDamaged, ABladeIICharacter*, int32, float)
DEFINITION_EVENT_CLASS(CharacterTakeDamage, ABladeIICharacter*, float, bool)
DEFINITION_EVENT_CLASS(CharacterAbnormalEffect, ABladeIICharacter*, const FLoadedFloatingAbnormalEffectSetupInfo&)
DEFINITION_EVENT_CLASS(PlayerTagged, ABladeIIPlayer*, ABladeIIPlayer*)
DEFINITION_EVENT_CLASS(CharacterDeath, ABladeIICharacter*)
DEFINITION_EVENT_CLASS(PlayerSuckedGold, int32)
DEFINITION_EVENT_CLASS(SetHUDCenterMessage, const FText, float, int32)
DEFINITION_EVENT_CLASS(HideHUDCenterMessage)
DEFINITION_EVENT_CLASS(SetSpawnNotifyMessage, const FString&, const float)
DEFINITION_EVENT_CLASS(RemoveSpawnNotifyMessage)

DEFINITION_EVENT_CLASS(PleaseForgetMe, ABladeIICharacter*)
DEFINITION_EVENT_CLASS(OnClearStage)
DEFINITION_EVENT_CLASS(OnResetStartingLevelInfo)
DEFINITION_EVENT_CLASS(PlayerHideMobPointer, bool)
DEFINITION_EVENT_CLASS(BornAgainChangedPlayer, int32, ABladeIIPlayer*, ABladeIIPlayer*)

DEFINITION_EVENT_CLASS(PlayerStartAttack)
DEFINITION_EVENT_CLASS(PlayerStopAttack)
DEFINITION_EVENT_CLASS(PlayerStartSkill01)
DEFINITION_EVENT_CLASS(PlayerStartSkill02)
DEFINITION_EVENT_CLASS(PlayerStartSkill03)
DEFINITION_EVENT_CLASS(PlayerStartGuard)
DEFINITION_EVENT_CLASS(PlayerStopGuard)
DEFINITION_EVENT_CLASS(PlayerStartWeaponSkill)
DEFINITION_EVENT_CLASS(PlayerStartWeaponSkillByIndex, int32)
DEFINITION_EVENT_CLASS(PlayerStartTag)
DEFINITION_EVENT_CLASS(PlayerChanged, ABladeIIPlayer*)
DEFINITION_EVENT_CLASS(TagPlayerSpawned, ABladeIIPlayer*)
DEFINITION_EVENT_CLASS(PlayerGuardStarted)
DEFINITION_EVENT_CLASS(PlayerTakeAttack, float)
DEFINITION_EVENT_CLASS(PlayerCounterAttackSucceed)
DEFINITION_EVENT_CLASS(TogglePlayerAutoState)

DEFINITION_EVENT_CLASS(StartPauseMenu, EB2GameMode)
DEFINITION_EVENT_CLASS(StopPauseMenu)
DEFINITION_EVENT_CLASS(StartDefeatMenu, EB2GameMode)
DEFINITION_EVENT_CLASS(StopDefeatMenu)
DEFINITION_EVENT_CLASS(OpenPreLoadingScreen, EPreLoadingScreenType, EB2GameMode)
DEFINITION_EVENT_CLASS(ClosePreLoadingScreenAll)
DEFINITION_EVENT_CLASS(ClosePreLoadingScreen, EPreLoadingScreenType)
DEFINITION_EVENT_CLASS(LobbyConditionalBeginOnTouchImpulseRing, const FVector2D&)
DEFINITION_EVENT_CLASS(LobbyMoveOnTouchImpulseRing, const FVector2D&)
DEFINITION_EVENT_CLASS(LobbyEndOnTouchImpulseRing)
DEFINITION_EVENT_CLASS(LobbyShowNetworkLoadingIcon, bool)
DEFINITION_EVENT_CLASS(ShowMiscLoadingIcon, bool)

DEFINITION_EVENT_CLASS(StartVisibleChatPopup, EB2GameMode)
DEFINITION_EVENT_CLASS(StopVisibleChatPopup)

//DEFINITION_EVENT_CLASS(DeliveryAppUpdatePopUp)
DEFINITION_EVENT_CLASS(DJLegacy_ChangeLobbyUIPage, ELobbyUIPages)
DEFINITION_EVENT_CLASS(DJLegacy_OpenLobbySubPopup, ELobbySubPopups, FDJLegacySubpopupAddPanelInfo)
DEFINITION_EVENT_CLASS(DJLegacy_CloseLobbySubPopup, ELobbySubPopups)
DEFINITION_EVENT_CLASS(ChangeInvenItemOpMode, ELobbyInvenItemOpMode)
DEFINITION_EVENT_CLASS(LobbyPCSelectionChanged, EPCClass)
DEFINITION_EVENT_CLASS(StartGameStageFromLobby, int32, EStageDifficulty, const TArray<EStageBuffType>&, bool)
DEFINITION_EVENT_CLASS(CancelStartGameStage)
DEFINITION_EVENT_CLASS(EndPlayOfSingleRepeatBattleStage)
DEFINITION_EVENT_CLASS(StartRepeatBattleStage, int32, EStageDifficulty)
DEFINITION_EVENT_CLASS(CancelOrStopRepeatBattle)
DEFINITION_EVENT_CLASS(RefreshAutoSellState)
DEFINITION_EVENT_CLASS(SetPlayerAutoType, int32)
DEFINITION_EVENT_CLASS(GiveUpGame)
DEFINITION_EVENT_CLASS(LobbyLoginAndStartGame, const FString&, const FString&)
DEFINITION_EVENT_CLASS(LobbyCreateAccount, const FString&, const FString&)
DEFINITION_EVENT_CLASS(LobbySetPCSelection, EPCClass, EPCClass)
DEFINITION_EVENT_CLASS(LobbySetMainPCClass, EPCClass)
DEFINITION_EVENT_CLASS(LobbyCharacterIntroSetPCSelection, EPCClass)
DEFINITION_EVENT_CLASS(LobbyProceedOnCharSel)
DEFINITION_EVENT_CLASS(LobbyEnterHeroMgmtMode, EHeroMgmtSubMode)
DEFINITION_EVENT_CLASS(LobbyQuitHeroMgmtMode)
DEFINITION_EVENT_CLASS(LobbySetHeroMgmtModePCSelection, EPCClass)
DEFINITION_EVENT_CLASS(LobbyPopupCreateAccount)
DEFINITION_EVENT_CLASS(LobbyUpdateAllItemIcons)
DEFINITION_EVENT_CLASS(LobbyTryUpdateSingleItemIcon, int64)
DEFINITION_EVENT_CLASS(LobbyStartMatch, bool)
DEFINITION_EVENT_CLASS(DoMarkCombineRedDot)
DEFINITION_EVENT_CLASS(SelectForLevelUpGrade, int32);

DEFINITION_EVENT_CLASS(UpdateLobbyInventoryControl)

DEFINITION_EVENT_CLASS(UpdateItemPreset, EPCClass, EItemPresetSlotType);
DEFINITION_EVENT_CLASS(UpdateSkillSlot, EPCClass, EItemPresetSlotType)
DEFINITION_EVENT_CLASS(SelectPresetSlot, EItemPresetSlotType)
DEFINITION_EVENT_CLASS(SelectPreviewPresetSlot, EItemPresetSlotType)

DEFINITION_EVENT_CLASS(LobbyReqUnEquipItemsOtherPreset, int32, const TArray<int64>&)

DEFINITION_EVENT_CLASS(EnableInventoryTouch, bool)
DEFINITION_EVENT_CLASS(EnterItemLevelupMode, int64)
DEFINITION_EVENT_CLASS(EnterItemUpgradeMode, int64)
DEFINITION_EVENT_CLASS(EnterItemCompositionMode, int64)
DEFINITION_EVENT_CLASS(EnterItemSurpassMode, int64)
DEFINITION_EVENT_CLASS(EnterItemEnhanceMode, int64)
DEFINITION_EVENT_CLASS(EnterItemCostumeEnhanceMode, int64)
DEFINITION_EVENT_CLASS(LobbyInventoryViewJustOpen)
DEFINITION_EVENT_CLASS(LobbyInventoryViewJustClose)
DEFINITION_EVENT_CLASS(LobbyExternalSetInventoryTab, EItemInvenType, bool)
DEFINITION_EVENT_CLASS(QuitItemOpMode, bool)
DEFINITION_EVENT_CLASS(LobbyEnterItemOpDirectingView, ELobbyInvenItemOpMode)
DEFINITION_EVENT_CLASS(LobbyQuitItemOpDirectingView)
DEFINITION_EVENT_CLASS(SelectForItemLevelupIngred, int64)
DEFINITION_EVENT_CLASS(UnSelectForItemLevelupIngred, int64)
DEFINITION_EVENT_CLASS(SelectForComposeIngred, int64)
DEFINITION_EVENT_CLASS(UnSelectForComposeIngred, int64)
DEFINITION_EVENT_CLASS(SelectForSurpassIngred, int64)
DEFINITION_EVENT_CLASS(UnSelectForSurpassIngred, int64)
DEFINITION_EVENT_CLASS(SelectForEnhanceIngred, int64)
DEFINITION_EVENT_CLASS(UnSelectForEnhanceIngred, int64)
DEFINITION_EVENT_CLASS(SelectForEnhanceChangeIngred, int64)
DEFINITION_EVENT_CLASS(UnSelectForEnhanceChangeIngred, int64)
DEFINITION_EVENT_CLASS(SelectForEnhanceProtectionItem, int32)
DEFINITION_EVENT_CLASS(SelectForItemHammer, int64)
DEFINITION_EVENT_CLASS(UnSelectForItemHammer, int64)
DEFINITION_EVENT_CLASS(LobbyTryUpdateItemOPMainMenu)
DEFINITION_EVENT_CLASS(LobbySlotChangeEffect)
DEFINITION_EVENT_CLASS(LobbyReqEquipItems, const TArray<int64>&)
DEFINITION_EVENT_CLASS(LobbyReqUnEquipItems, const TArray<int64>&)
DEFINITION_EVENT_CLASS(LobbyReqSellItem, int64, bool)
DEFINITION_EVENT_CLASS(LobbyReqSellItems, const TArray<int64>&, bool)
DEFINITION_EVENT_CLASS(LobbyReqDecompositionItems, const TArray<int64>&, bool)
DEFINITION_EVENT_CLASS(LobbyReqSellConsumableItem, int64, int32)
DEFINITION_EVENT_CLASS(LobbyReqConvertData, int32)
DEFINITION_EVENT_CLASS(LobbyReqConvertItem, int32, int32)
DEFINITION_EVENT_CLASS(LobbyReqToggleItemLock, int64, bool)
DEFINITION_EVENT_CLASS(LobbyReqItemLevelupWithCurrentSelection)
DEFINITION_EVENT_CLASS(LobbyReqEnhanceItem)
DEFINITION_EVENT_CLASS(LobbyReqUpgradeItem)
DEFINITION_EVENT_CLASS(LobbyReqComposeItemWithCurrentSelection)
DEFINITION_EVENT_CLASS(LobbyReqSurpassItemWithCurrentSelection)
DEFINITION_EVENT_CLASS(LobbyReqAutoItemLevelup, const TArray<FB2Item>&, bool)
DEFINITION_EVENT_CLASS(LobbyAllItemsAdded)
DEFINITION_EVENT_CLASS(LobbyItemsAdded, const FB2AddedItems&)
DEFINITION_EVENT_CLASS(LobbySetCombatEntryChapterNum, int32, EStageDifficulty)
DEFINITION_EVENT_CLASS(GoGameStageInfoFromLobby, FServerStageID, FRepeatBattleStateSet)
DEFINITION_EVENT_CLASS(GoGameStageInfoLastReachedFromLobby)
DEFINITION_EVENT_CLASS(GameStageCheckForArinConsulting, FServerStageID)
DEFINITION_EVENT_CLASS(ChangeUserAccountInfo)
DEFINITION_EVENT_CLASS(ChangeStaminaChargeTime)
DEFINITION_EVENT_CLASS(ChangePvPMatchPointChargeTime)
DEFINITION_EVENT_CLASS(StartRankingUpAnimation, const EStateAfterGradeUp)
DEFINITION_EVENT_CLASS(ChangeTeamMatchPointChargeTime)
DEFINITION_EVENT_CLASS(LobbyInvenSetItemOpModeGuideText, const FText&)
DEFINITION_EVENT_CLASS(LobbyInvenHideItemOpModeGuideText)
DEFINITION_EVENT_CLASS(LobbyChangeScene, ELobbyScene)
DEFINITION_EVENT_CLASS(LobbyChangeSceneByUIScene, EUIScene)
DEFINITION_EVENT_CLASS(LobbyChangeToChapterSceneByUIScene, int32)
DEFINITION_EVENT_CLASS(LobbyUpdateCharacterPart, EPCClass)
DEFINITION_EVENT_CLASS(LobbyEnterPVPMainMenu)
DEFINITION_EVENT_CLASS(ReturnToFindAccount)
DEFINITION_EVENT_CLASS(ReturnToPVPMainMenu, bool)
DEFINITION_EVENT_CLASS(ReturnToPVPRematch)
DEFINITION_EVENT_CLASS(ReturnToHeroTowerMainMenu)
DEFINITION_EVENT_CLASS(ReturnToRaidMain)
DEFINITION_EVENT_CLASS(ReturnToGuildMain)
DEFINITION_EVENT_CLASS(LobbyEnterTeamMatchMenu)
DEFINITION_EVENT_CLASS(LobbyEnterControlMenu)
DEFINITION_EVENT_CLASS(EnterTMHeroChangeMenu)
DEFINITION_EVENT_CLASS(LobbyAnyOtherUpdateOnSetAccount)
DEFINITION_EVENT_CLASS(LobbyUpdateChapterData)
DEFINITION_EVENT_CLASS(UpdateChatUI, bool)
//DEFINITION_EVENT_CLASS(UpdateChatSinglePopup, const FB2ChatMessage&)
DEFINITION_EVENT_CLASS(UpdateAdminMessage, const FString&, const int32)
DEFINITION_EVENT_CLASS(LobbyPlayUIEquipEffect, EItemEquipPlace)
DEFINITION_EVENT_CLASS(LobbyReceivedRepeatClearReward, int32)
DEFINITION_EVENT_CLASS(EnterShop, int32)
DEFINITION_EVENT_CLASS(LobbyUpdateWingMainUI)
DEFINITION_EVENT_CLASS(BeginReqForLobbyInitWingData)
DEFINITION_EVENT_CLASS(SetLocalWingDataByAccountInfo, const TArray<b2network::B2CharacterServerInfoPtr>&, const TArray<b2network::B2WingOptionInfoPtr>&)
DEFINITION_EVENT_CLASS(UpdateWingCombatStatsOnMD)
DEFINITION_EVENT_CLASS(BeginWingEvolutionScene, EPCClass)
DEFINITION_EVENT_CLASS(EndWingEvolutionScene)
DEFINITION_EVENT_CLASS(LinkUICollectBook, EPCClass, EItemClass)
DEFINITION_EVENT_CLASS(FindAccountRequestEvent, int64, EPCClass, EPCClass, EPCClass)
DEFINITION_EVENT_CLASS(ChangeLastClearStage, EStageDifficulty, int32, int32)
DEFINITION_EVENT_CLASS(EndHallOfFameMatinee)

DEFINITION_EVENT_CLASS(ReserveStoreScene, bool)

//DEFINITION_EVENT_CLASS(SpawnPoolMobSpawn, ABladeIICharacter*, const FMobSpawnedBirthplaceInfo&)
DEFINITION_EVENT_CLASS(SpawnPoolComplete, AB2MonsterSpawnPool*)
DEFINITION_EVENT_CLASS(SpawnPoolSingleWaveStart, AB2MonsterSpawnPool*, int32)
DEFINITION_EVENT_CLASS(SpawnPoolSingleWaveComplete, AB2MonsterSpawnPool*, int32)
DEFINITION_EVENT_CLASS(SpawnPoolWaveMobDead, AB2MonsterSpawnPool*, ABladeIICharacter*, int32, int32)
DEFINITION_EVENT_CLASS(SpawnPoolWaveMobPhaseChange, AB2MonsterSpawnPool*, ABladeIICharacter*, ABladeIICharacter*, int32, int32, int32)
DEFINITION_EVENT_CLASS(SpawnPoolSummonedMobDead, AB2MonsterSpawnPool*, ABladeIICharacter*)
//DEFINITION_EVENT_CLASS(StageEventSceneBegin, AB2StageEventDirector*, EStageEvent)
DEFINITION_EVENT_CLASS(StageEventSceneEnd, AB2StageEventDirector*)
DEFINITION_EVENT_CLASS(PlayStageBGM)
DEFINITION_EVENT_CLASS(EnsureStageEventTimeDilation)

DEFINITION_EVENT_CLASS(StageEventTalkStart, int32, bool)
DEFINITION_EVENT_CLASS(StageEventTalkEnd, int32, bool)

DEFINITION_EVENT_CLASS(ShowStageClearWingEffect)
DEFINITION_EVENT_CLASS(ShowStageClearGemEffect)
DEFINITION_EVENT_CLASS(BeginStageClearScene_Phase1)
DEFINITION_EVENT_CLASS(BeginStageClearScene_Phase2)

DEFINITION_EVENT_CLASS(BeginGlobalSceneBlur, float)
DEFINITION_EVENT_CLASS(EndGlobalSceneBlur)
DEFINITION_EVENT_CLASS(RestartGame)
DEFINITION_EVENT_CLASS(GoToTitle)
DEFINITION_EVENT_CLASS(GoToVillage)
DEFINITION_EVENT_CLASS(GoToVillageDefeated)
DEFINITION_EVENT_CLASS(GoToMap)
DEFINITION_EVENT_CLASS(GoToDeferredStage, int32)
DEFINITION_EVENT_CLASS(GoToTutorial, bool)
DEFINITION_EVENT_CLASS(GoToDLCMap)
DEFINITION_EVENT_CLASS(FailResponseMasterData)

/* InGame flow Event*/
DEFINITION_EVENT_CLASS(FightStart, int32)

/* InGame flow Event*/

DEFINITION_EVENT_CLASS(RequestResurrect, EResurrectBuffType)

DEFINITION_EVENT_CLASS(LobbyMissionAlarm)
DEFINITION_EVENT_CLASS(ShowMissionDialog)

DEFINITION_EVENT_CLASS(OpenDialog, FName, bool)
DEFINITION_EVENT_CLASS(NextDialog)
DEFINITION_EVENT_CLASS(SkipDialog)
DEFINITION_EVENT_CLASS(OnUpdatedDialog, FName, int32)
DEFINITION_EVENT_CLASS(CloseDialog, FName)
DEFINITION_EVENT_CLASS(ToolTipOpen)
DEFINITION_EVENT_CLASS(ToolTipClose)
DEFINITION_EVENT_CLASS(ToolTipMove)
DEFINITION_EVENT_CLASS(ForcedCloseToolTipPopup)

DEFINITION_EVENT_CLASS(UIWidgetOpenComplete, FName)

DEFINITION_EVENT_CLASS(StageComplete)

DEFINITION_EVENT_CLASS(CounterFinishAttack)
DEFINITION_EVENT_CLASS(QTEAttack)

DEFINITION_EVENT_CLASS(SelectCharacterToSummonItem, int32, bool)
DEFINITION_EVENT_CLASS(SelectItemToBuy, int32)
DEFINITION_EVENT_CLASS(UpdateMarkRedDot, const FName&)
DEFINITION_EVENT_CLASS(ProcessLobbyTutorial)
DEFINITION_EVENT_CLASS(AutoMinoRidding)

DEFINITION_EVENT_CLASS(SetEquippedEther, const int32, const TArray<b2network::B2AetherServerInfoPtr>&)
DEFINITION_EVENT_CLASS(OpenEtherSetToolTip, int32, EPCClass)

DEFINITION_EVENT_CLASS(OnEtherActivated, int32, ABladeIIPlayer*, ABladeIICharacter*);

DEFINITION_EVENT_CLASS(MarkBeginAsyncLoad)

DEFINITION_EVENT_CLASS(OnUpdateCombineMenu, bool)

DEFINITION_EVENT_CLASS(RequestCurrentTabMailList)

DEFINITION_EVENT_CLASS(UpdateEventAttendanceParts)
DEFINITION_EVENT_CLASS(ReceiveEventAttendanceReward, int32, int32, b2network::B2RewardPtr)
DEFINITION_EVENT_CLASS(ReceiveAttendanceExitEnable, bool)
DEFINITION_EVENT_CLASS(ReceiveAttendanceExitVisibility, bool)
DEFINITION_EVENT_CLASS(ReceivePlayTimeReward, const TArray<b2network::B2RewardPtr>&)
DEFINITION_EVENT_CLASS(UpdateAccumulatedPlayTime, int32)
DEFINITION_EVENT_CLASS(LobbyNoticePopUp, int32)
DEFINITION_EVENT_CLASS(FinishConnectReward)
DEFINITION_EVENT_CLASS(SummonitemOpenCardMatineePlay, int32)

DEFINITION_EVENT_CLASS(RequestJoinRoomBridge, const FString&, int32, int32)

DEFINITION_EVENT_CLASS(LobbyBannerAllLoad)

DEFINITION_EVENT_CLASS(RefreshHotTimeUI)
DEFINITION_EVENT_CLASS(BuyHotTimeBuffSuccessful)
DEFINITION_EVENT_CLASS(GetEventStatusSuccessful)
DEFINITION_EVENT_CLASS(GetLatestEventInfoSuccessful)
DEFINITION_EVENT_CLASS(DiceCellFinishAnimation, int32)
DEFINITION_EVENT_CLASS(DiceCellFinishArriveAnimation, int32)

DEFINITION_EVENT_CLASS(SetupLoadingScreenImage, bool, bool, bool, const TCHAR*, EB2LoadingMessageType)
DEFINITION_EVENT_CLASS(ConditionalWaitForLoadingScreen)

DEFINITION_EVENT_CLASS(ErrorOpenWebEmptyURL)

DEFINITION_EVENT_CLASS(SkipTutorialRefreshUI)

DEFINITION_EVENT_CLASS(UseGuildNPCSkill, bool)
DEFINITION_EVENT_CLASS(ForceFocusViewTarget, ABladeIIPlayer*, float)
DEFINITION_EVENT_CLASS(GuildGameChangeCamera, bool, ABladeIIPlayer*)

DEFINITION_EVENT_CLASS(FirstRequestUpdateDailyPlayTime)
/*
	LobbyScene Events
*/
DEFINITION_EVENT_CLASS(LobbyCharIntroSelect, EPCClass)
DEFINITION_EVENT_CLASS(LobbyHeroMgmtSelect, EPCClass)
DEFINITION_EVENT_CLASS(LobbyStageInfoSelect, EPCClass, EPCClass)
DEFINITION_EVENT_CLASS(LobbyCharObvSelect, EPCClass)
DEFINITION_EVENT_CLASS(LobbyCharEquippedItem, EPCClass, EItemEquipPlace)
DEFINITION_EVENT_CLASS(LobbyCharCollectBookSelect, EPCClass)
DEFINITION_EVENT_CLASS(LobbyHeroFindAccount, EPCClass)
DEFINITION_EVENT_CLASS(LobbyCharObvForcedChange, EPCClass)

DEFINITION_EVENT_CLASS(LobbyCharPlayMatinee, float)
DEFINITION_EVENT_CLASS(LobbyCharMoveCamera, const FRotator&, float)
DEFINITION_EVENT_CLASS(LobbyCharRotateCharacter, float)
DEFINITION_EVENT_CLASS(LobbyCharCameraViewTarget, const FVector2D&)
DEFINITION_EVENT_CLASS(LobbyChapterCameraMove, float)
DEFINITION_EVENT_CLASS(LobbyBlockMoveCamera, bool)
DEFINITION_EVENT_CLASS(LobbyMainCameraMove, float)
DEFINITION_EVENT_CLASS(LobbyCharZoom, const FVector2D&, float)

DEFINITION_EVENT_CLASS(CreateLobbyCharOnHeadDisplay, EPCClass, ASkeletalMeshActor*, ELobbyOnHeadDisplayType)
DEFINITION_EVENT_CLASS(DestroyAllLobbyCharOnHeadDisplay)
DEFINITION_EVENT_CLASS(UpdateLobbyCharOnHeadDisplay)
DEFINITION_EVENT_CLASS(LobbyCharObservePopupNickEdit, EPCClass)
DEFINITION_EVENT_CLASS(LobbyCharObserveCloseNickEdit)

DEFINITION_EVENT_CLASS(LobbyCharObserveChangeScreen, bool)

DEFINITION_EVENT_CLASS(UpdateCombatPower, EPCClass, bool)
DEFINITION_EVENT_CLASS(ShowBattleScoreNotify, EPCClass)

DEFINITION_EVENT_CLASS(RequestCombatPower, EPCClass);

DEFINITION_EVENT_CLASS(DoneToGetAccount)
DEFINITION_EVENT_CLASS(LobbyHeroTransientEffectBegin, const int32);

DEFINITION_EVENT_CLASS(NotifyUIButtonClicked, UButton*);
DEFINITION_EVENT_CLASS(ShowRaidInviteNotice);

DEFINITION_EVENT_CLASS(SelectEventPageTab, int32);
DEFINITION_EVENT_CLASS(SelectEventTabShopLimited, int32)
DEFINITION_EVENT_CLASS(StratAnimEventMainTransparencyBlock)

DEFINITION_EVENT_CLASS(CloseCombineMenu)

DEFINITION_EVENT_CLASS(PlayLobbyBgm)
DEFINITION_EVENT_CLASS(OnChangUIScene, EUIScene)
DEFINITION_EVENT_CLASS(OnChangUISceneBack)

DEFINITION_EVENT_CLASS(OnChangeFairyCharacter, EFairyType)

DEFINITION_EVENT_CLASS(OpenCostume, bool)
DEFINITION_EVENT_CLASS(EnterCostumePage, ECostumePage)
DEFINITION_EVENT_CLASS(CostumeEquipSlotVisible, bool)
DEFINITION_EVENT_CLASS(SuccesGetAccountCostumeAll)

DEFINITION_EVENT_CLASS(SelectCostumePackage, int32)

DEFINITION_EVENT_CLASS(RefreshCostumeHeroMgmtRedDot)

DEFINITION_EVENT_CLASS(ForcedInvenTabOpen, EItemInvenType)
DEFINITION_EVENT_CLASS(AllClearUnityAwakenMission)

DEFINITION_EVENT_CLASS(ForcedClickCostumeMaterial, bool)
DEFINITION_EVENT_CLASS(OnCallBackEnhanceCostumeInvenPage, const FB2Item&)

DEFINITION_EVENT_CLASS(UpdateFairyGiftState)
DEFINITION_EVENT_CLASS(UpdateCombatStatScale, const float)

/*
	SkillList Events
*/
DEFINITION_EVENT_CLASS(ChangeSkillPoint)
DEFINITION_EVENT_CLASS(ChangeSkillLevel, int32)
DEFINITION_EVENT_CLASS(ChangeBuySkillPoint)
DEFINITION_EVENT_CLASS(CloseSkillTooltip, int32)
DEFINITION_EVENT_CLASS(RegistSkill)

/*
LinkManagerEvent
*/

DEFINITION_EVENT_CLASS(LinkChangeUIScene)

/*
EtherEvent
*/

DEFINITION_EVENT_CLASS(RefreshScrollUI)
DEFINITION_EVENT_CLASS(SelectEtherLotterySlot, int32)
DEFINITION_EVENT_CLASS(SellEtherItem, int64, int32)
DEFINITION_EVENT_CLASS(ResultAllSellEtherItem, int32)

/*
BackEvent
*/

DEFINITION_EVENT_CLASS(SetCloseBattleMainUI, bool)

/*
MailEvent
*/

DEFINITION_EVENT_CLASS(SetLotteryAllFilteringInfo, TArray<int64>&, EPCClass, int32)

/*
PlatformPauseEvent
*/

DEFINITION_EVENT_CLASS(PlatformPausePress)

/*
Mission&Quest Event
*/

DEFINITION_EVENT_CLASS(GetAllMIssionQuestReward)
DEFINITION_EVENT_CLASS(ChangeMissionTab, const EMissionType, const bool)
DEFINITION_EVENT_CLASS(ReqGetContetnsOpenStatus, const int32)
DEFINITION_EVENT_CLASS(SetAllRewardButtonDisable, const bool)
DEFINITION_EVENT_CLASS(UpdateMainQuestScrollByIndex, const int32)
DEFINITION_EVENT_CLASS(UpdateSubQuestScoll)

DEFINITION_EVENT_CLASS(UpdateDonationResultPopup)
DEFINITION_EVENT_CLASS(UpdateDonationEvent)
DEFINITION_EVENT_CLASS(OpenDonationPointStepRewardInfoPopup, int32)
DEFINITION_EVENT_CLASS(UpdateDonationPointRewardUI)

/*
	Network Events
*/
DEFINITION_EVENT_CLASS(NoAccount)
DEFINITION_EVENT_CLASS(RequestGiveNickname,				const FString&)
DEFINITION_EVENT_CLASS(RequestUpdateNickname,			const FString&, uint32)
DEFINITION_EVENT_CLASS(DeliveryGetServerVersion,		const FString&, const FString&)

DEFINITION_EVENT_CLASS(DeliveryAccountInfo,				FB2WholeInfo)
DEFINITION_EVENT_CLASS(DeliveryAccountItem,				FB2AccountItems)
DEFINITION_EVENT_CLASS(DeliveryMasterData,				FB2MasterDatas)
DEFINITION_EVENT_CLASS(DeliveryAddItem,					FB2AddedItems)
DEFINITION_EVENT_CLASS(DeliveryDismantleItems,			FB2DismantleItems)
DEFINITION_EVENT_CLASS(DeliveryDismantleItemsError)
DEFINITION_EVENT_CLASS(DeliverySellItem,				FB2SoldItems)
DEFINITION_EVENT_CLASS(DeliverySellItemError)
DEFINITION_EVENT_CLASS(DeliveryEquipItem,				FB2EquippedItems)
DEFINITION_EVENT_CLASS(DeliverySellConsumableItem,		FB2SellConsumableItem)
DEFINITION_EVENT_CLASS(DeliveryConvertData,				FB2ConvertData)
DEFINITION_EVENT_CLASS(DeliveryConvertItem,				FB2ConvertItem)
DEFINITION_EVENT_CLASS(DeliveryUnequipItem,				FB2UnequippedItems)
DEFINITION_EVENT_CLASS(DeliveryLockItem,				FB2LockedItems)
DEFINITION_EVENT_CLASS(DeliveryUnlockItem,				FB2UnlockedItems)
DEFINITION_EVENT_CLASS(DeliveryItemLevelup,				FB2LevelupItem)
DEFINITION_EVENT_CLASS(DeliveryEnhanceItem,				FB2EnhanceItem)
DEFINITION_EVENT_CLASS(DeliveryItemEnhanceLevel,		FB2ItemEnhanceLevel)
DEFINITION_EVENT_CLASS(DeliveryOpenSealSlotItem,		FB2ResponseOpenSealSlotItemPtr)
DEFINITION_EVENT_CLASS(DeliveryOpenSealSlotItemError)
DEFINITION_EVENT_CLASS(DeliveryCarveSealItem,			FB2ResponseCarveSealItemPtr)
DEFINITION_EVENT_CLASS(DeliveryCarveSealItemError)
DEFINITION_EVENT_CLASS(DeliveryGetItemSurpassCost,		FB2GetItemSurpassCost)
DEFINITION_EVENT_CLASS(DeliverySurpassItem,				FB2SurpassItem)
DEFINITION_EVENT_CLASS(DeliveryGetItemUpgradeCost,		FB2GetItemUpgradeCost)
DEFINITION_EVENT_CLASS(DeliveryUpgradeItem,				FB2UpgradeItem)
DEFINITION_EVENT_CLASS(DeliveryGetItemComposeCost,		FB2GetItemComposeCost)
DEFINITION_EVENT_CLASS(DeliveryComposeItem,				FB2ComposeItem)
DEFINITION_EVENT_CLASS(DeliveryExtendInventory,			FB2ExtendInventory)
DEFINITION_EVENT_CLASS(DeliveryStartStage,				FB2StartStageInfo)
DEFINITION_EVENT_CLASS(DeliveryStartStageError)
DEFINITION_EVENT_CLASS(DeliveryResurrection,			FB2Resurrection)
DEFINITION_EVENT_CLASS(DeliveryClearStage,				FB2ClearStageInfo)
DEFINITION_EVENT_CLASS(DeliveryFailStage)
DEFINITION_EVENT_CLASS(DeliverySweepStage,				FB2SweepStageInfo)
DEFINITION_EVENT_CLASS(DeliverySweepStageError, FB2SweepStageInfo, int32)
DEFINITION_EVENT_CLASS(DeliveryCheckChargePoints,		FB2CheckChargePointsPtr)
DEFINITION_EVENT_CLASS(DeliveryCheckBladePointCharge,	FB2BladePointCharge)
DEFINITION_EVENT_CLASS(DeliveryCheckPvpMatchPointCharge, FB2PvpMatchPointCharge)
DEFINITION_EVENT_CLASS(DeliveryCheckTeamMatchPointCharge, FB2TeamMatchPointCharge)
DEFINITION_EVENT_CLASS(DeliveryGetActInfo,				FB2OriginGetActInfoPtr)
DEFINITION_EVENT_CLASS(DeliveryGetStageInfo,			FB2OriginGetStageInfoPtr)
DEFINITION_EVENT_CLASS(DeliveryGetActClearReward,		FB2GetReceiveActClearReward)
DEFINITION_EVENT_CLASS(DeliveryGetActRepeatClearReward, FB2GetReceiveActRepeatClearReward)

DEFINITION_EVENT_CLASS(DeliveryTokenToTagMatch,			const FString&)
DEFINITION_EVENT_CLASS(DeliveryGetMatchStatus,			FB2TagMatchStatus)
DEFINITION_EVENT_CLASS(DeliveryCancelMatchmaking)
DEFINITION_EVENT_CLASS(DeliveryTagMatchResult,			FB2TagMatchResult)
DEFINITION_EVENT_CLASS(DeliveryGetTeamBattleStatus,		FB2TeamBattleStatus)
DEFINITION_EVENT_CLASS(DeliveryUpdateTeamBattleEntry,	FB2TeamBattleEntryPtr)
DEFINITION_EVENT_CLASS(DeliveryTeamMatchMaking,			FB2TeamMatchMaking)
DEFINITION_EVENT_CLASS(DeliveryTeamMatchResult,			FB2TeamMatchResult)
DEFINITION_EVENT_CLASS(DeliveryEnhanceTeamFormation,	FB2EnhanceTeamFormation)
DEFINITION_EVENT_CLASS(DeliveryReceiveGetWing,			const FB2Wing&)
DEFINITION_EVENT_CLASS(DeliveryReceiveEnhanceWing,		FB2EnhanceWing)
DEFINITION_EVENT_CLASS(DeliveryReceiveUpgradeWing,		FB2UpgradeWing)
DEFINITION_EVENT_CLASS(DeliveryReceiveEnhanceWingOption, FB2EnhanceWingOption)
DEFINITION_EVENT_CLASS(DeliveryReceiveSetWingVisible,	FB2SetWingVisible)
DEFINITION_EVENT_CLASS(DeliveryReceiveSetHelmetVisible, FB2SetHelmetVisible)
DEFINITION_EVENT_CLASS(DeliveryGetCollectionItem,		FB2CollectionItemPtr)
DEFINITION_EVENT_CLASS(DeliveryRewardCollectionItem,	FB2RewardCollectionItemPtr)
DEFINITION_EVENT_CLASS(DeliveryRewardCollectionSetItem, FB2RewardCollectionSetItemPtr)
DEFINITION_EVENT_CLASS(DeliveryGetDonationEvent,		FB2GetDonationEventPtr)
DEFINITION_EVENT_CLASS(DeliveryMakeDonation,			FB2MakeDonationPtr)
DEFINITION_EVENT_CLASS(DeliveryDonationPointReward,		FB2DonationPointRewardPtr)
DEFINITION_EVENT_CLASS(DeliveryGetFairy,				FB2GetFairyPtr)
DEFINITION_EVENT_CLASS(DeliveryLevelupFairy,			FB2LevelupFairyPtr)
DEFINITION_EVENT_CLASS(DeliveryLevelupFairyError,		FB2LevelupFairyPtr)
DEFINITION_EVENT_CLASS(DeliveryBlessFairy,				FB2BlessFairyPtr)
DEFINITION_EVENT_CLASS(DeliveryReceiveFairyGift,		FB2ReceiveFairyGiftPtr)

DEFINITION_EVENT_CLASS(HandleServerError4032)
DEFINITION_EVENT_CLASS(HandleServerError4015,			FB2StartStageInfo)
DEFINITION_EVENT_CLASS(HandleServerError4030,			FB2StartStageInfo)
DEFINITION_EVENT_CLASS(HandleServerError910)
DEFINITION_EVENT_CLASS(HandleServerError911)
DEFINITION_EVENT_CLASS(HandleServerError901)
DEFINITION_EVENT_CLASS(HandleServerError919)
DEFINITION_EVENT_CLASS(HandleServerError932)
DEFINITION_EVENT_CLASS(HandleServerError1012)
DEFINITION_EVENT_CLASS(HandleServerError101)
DEFINITION_EVENT_CLASS(HandleServerError113)
DEFINITION_EVENT_CLASS(HandleServerError4207)
DEFINITION_EVENT_CLASS(HandleServerError5503)
DEFINITION_EVENT_CLASS(HandleServerError5513)
DEFINITION_EVENT_CLASS(HandleServerError6516)
DEFINITION_EVENT_CLASS(HandleServerError7413)

DEFINITION_EVENT_CLASS(HandleServerError7237)

DEFINITION_EVENT_CLASS(HandleServerError7250			, FB2ResponseIAPPreparePurchasePtr)
DEFINITION_EVENT_CLASS(HandleServerError7253)
DEFINITION_EVENT_CLASS(HandleServerError7256)
DEFINITION_EVENT_CLASS(HandleServerInAppPurchaseError, int32)

DEFINITION_EVENT_CLASS(HandleServerError8005)

DEFINITION_EVENT_CLASS(DeliveryReceiveRenewalMagicShopError, FB2ResponseRenewalMagicShopPtr)
DEFINITION_EVENT_CLASS(DeliveryReceivePurchaseMagicShopError, FB2ResponsePurchaseMagicShopPtr)

DEFINITION_EVENT_CLASS(HandleServerErrorGoodsShortage, const uint32, const EGoodsButtonType)
DEFINITION_EVENT_CLASS(HandleServerError4018)

//DEFINITION_EVENT_CLASS(SuccessToMatchmake,				data_trader::ChannelPtr, uint32, const TArray<FB2RoomParticipantPtr>&)
DEFINITION_EVENT_CLASS(ReceivedMyNetId,					uint32, int64, const FB2FindAccountInfoPtr&)
DEFINITION_EVENT_CLASS(ReceivedExchangeInfomations,		uint32, EPCClass, EPCClass, uint32, uint32, uint32, const FGuid&, int64)
DEFINITION_EVENT_CLASS(ReceivedAckExchangeInformations)
DEFINITION_EVENT_CLASS(ReceivedOtherNetId,				uint32, int64, const FB2FindAccountInfoPtr&)
DEFINITION_EVENT_CLASS(ReceivedNotifyLeave,				uint32)
DEFINITION_EVENT_CLASS(ReceivedRoomLeaveResponse)
DEFINITION_EVENT_CLASS(ReceivedRoomCloseResponse,		const FString&, int32)

DEFINITION_EVENT_CLASS(ReceivedStandalonePlay, FB2TagMatchMakingPtr)

DEFINITION_EVENT_CLASS(BroadcastRoomMessage,			uint32, const FString&, bool)
DEFINITION_EVENT_CLASS(ReceivedNotifyChangeHost,		int32, int32)

DEFINITION_EVENT_CLASS(ReceivedNotifyInviteMessage,		const FString&, const FString&, const FString&, int32, int32)
DEFINITION_EVENT_CLASS(ReceivedNotifyUpdateRoomProperty, const FString&, const TArray<b2network::B2RoomPropertyPtr>&)

DEFINITION_EVENT_CLASS(MatchGameStartEventSceneEnd)
DEFINITION_EVENT_CLASS(StageEventDirectorFinishShowEvent, int32)

DEFINITION_EVENT_CLASS(ReceivedNetId,					uint32)
DEFINITION_EVENT_CLASS(ReceivedNotifyResult,			ENetMatchResult, uint8)
DEFINITION_EVENT_CLASS(ReceivedMovement,				uint32, FVector, FVector, FRotator, float)
DEFINITION_EVENT_CLASS(ReceivedRotation,				uint32, FRotator)
DEFINITION_EVENT_CLASS(ReceivedAttack,					uint32, FVector, FRotator)
DEFINITION_EVENT_CLASS(ReceivedStopAttack,				uint32, FVector, FRotator)
//DEFINITION_EVENT_CLASS(ReceivedGuard,					uint32, FVector, FRotator, uint32, uint32, uint8, uint8)
//DEFINITION_EVENT_CLASS(ReceivedStopGuard,				uint32, FVector, FRotator)
DEFINITION_EVENT_CLASS(ReceivedSkillAttack,				uint32, uint8, uint8, FVector, FRotator)
DEFINITION_EVENT_CLASS(ReceivedTagPlayer,				uint32, uint8, uint8, FVector, FRotator)
DEFINITION_EVENT_CLASS(ReceivedComboEnd,				uint32, uint8, bool)
DEFINITION_EVENT_CLASS(ReceivedTakeDamage,				uint32, uint32, float, uint32, int32, int32)
DEFINITION_EVENT_CLASS(ReceivedRequestDamage,			uint32, uint32, float, uint32)
DEFINITION_EVENT_CLASS(ReceivedHealth,					uint32, float, uint32)
DEFINITION_EVENT_CLASS(ReceivedUpdateTarget,			uint32, uint32, float)
//DEFINITION_EVENT_CLASS(ReceivedCounterAttack,			uint32, FVector, FRotator)
DEFINITION_EVENT_CLASS(ReceivedMobAttack,				uint32, FVector, FRotator, bool, uint32)
DEFINITION_EVENT_CLASS(ReceivedResurrect,				uint32, FVector, FRotator, TMap<EResurrectBuffType, uint8>)
DEFINITION_EVENT_CLASS(ReceivedRaidNotifyResult,		bool, uint32, TArray<TPair<int32, int32>> const &)
DEFINITION_EVENT_CLASS(ReceivedRaidResult,				FB2ResponseRaidResultPtr)
DEFINITION_EVENT_CLASS(HandleServerError4015InRaid)
DEFINITION_EVENT_CLASS(HandleServerError4015ByRoomJoin)
DEFINITION_EVENT_CLASS(HandleServerError4015ForSelectCharacter, TArray<b2network::B2InventoryCountServerInfoPtr>)
DEFINITION_EVENT_CLASS(ReceivedGetRaidCount,			uint8)
DEFINITION_EVENT_CLASS(ReceivedEtherActivated,			uint32, uint32, uint32)
DEFINITION_EVENT_CLASS(ReceivedDie,						uint32, uint32, int32)
DEFINITION_EVENT_CLASS(ReceivedBuffActivated,			uint32, uint32, uint8, float, float, bool, bool, TArray<float>*)
DEFINITION_EVENT_CLASS(ReceivedSetAnimState,			uint32, const FString&)
DEFINITION_EVENT_CLASS(DeliveryRaidResurrection,		FB2RaidResurrection)
DEFINITION_EVENT_CLASS(DeliveryGetRaid,					FB2ResponseGetRaidPtr)
DEFINITION_EVENT_CLASS(DeliveryStartRaid)
DEFINITION_EVENT_CLASS(DeliveryRaidMatchmaking,			FB2ResponseRaidMatchmakingPtr)
DEFINITION_EVENT_CLASS(DeliveryRaidMatchmakingError)
DEFINITION_EVENT_CLASS(DeliveryRoomJoin,				FB2ResponseRoomJoinPtr)
DEFINITION_EVENT_CLASS(DeliveryRoomJoinError)
DEFINITION_EVENT_CLASS(ReceivedConsumeControlSanctuary, int32)
DEFINITION_EVENT_CLASS(ReceivedRespawnControlSanctuary, int32)
DEFINITION_EVENT_CLASS(ReceivedDrinkControlSanctuary,	uint32, int32)
DEFINITION_EVENT_CLASS(ReceiveRoomPlayerTeamInfo,		TMap<uint32, uint8>&)
DEFINITION_EVENT_CLASS(ReceivedChangeDamageNum,			uint32, int32);
DEFINITION_EVENT_CLASS(ReceivedRaidNotifyTimeEnd,		uint32);
DEFINITION_EVENT_CLASS(ReceivedSyncAnimStateIndex, uint32, FNetSyncAnimInfo&)
DEFINITION_EVENT_CLASS(ReceivedQteNext, uint32)
DEFINITION_EVENT_CLASS(ReceivedSpawnEnd, uint32)

DEFINITION_EVENT_CLASS(ReceivedPrerequisiteToStart,		int32, const FString&, TArray<FB2NetMatchPeerInfo> const&)
DEFINITION_EVENT_CLASS(ReceivedAckPrerequisiteToStart,  uint32)
DEFINITION_EVENT_CLASS(ReceivedPrepareToStart,			bool)
DEFINITION_EVENT_CLASS(ReceivedChangeRoomAccess,		uint8)
DEFINITION_EVENT_CLASS(ReceivedKickUser)
DEFINITION_EVENT_CLASS(ReceivedNoResponsePrerequisiteToStart)
DEFINITION_EVENT_CLASS(ReceivedRoomPlayerClass,			uint32, TArray<FB2NetMatchPeerInfo> const&)
DEFINITION_EVENT_CLASS(ReceivedAckRoomPlayerClass,		uint32, EPCClass)

DEFINITION_EVENT_CLASS(ReceivedSyncReadyToStartGame,	uint32)
DEFINITION_EVENT_CLASS(ReceivedSyncStartGame)

DEFINITION_EVENT_CLASS(ReceivedConquestAreaState,		uint8, float, float)
DEFINITION_EVENT_CLASS(ReceivedControlState,			uint8, float, float, bool)
DEFINITION_EVENT_CLASS(ReceivedControlKillCount,		TMap<uint32, int32>*)
//DEFINITION_EVENT_CLASS(ReceivedControlResult,			ETeamType)
DEFINITION_EVENT_CLASS(ReceivedControlTimer,			uint8, float)
DEFINITION_EVENT_CLASS(ReceivedControlEnemyLeaveAll)

DEFINITION_EVENT_CLASS(ReceivedAskSpawnComplete)
DEFINITION_EVENT_CLASS(ReceivedSpawnComplete,			uint32, bool)

DEFINITION_EVENT_CLASS(ReceiveRequestQTE, uint32)
DEFINITION_EVENT_CLASS(ReceiveResponseQTE, uint32, bool)
DEFINITION_EVENT_CLASS(ReceiveNotifyEndQTE, uint32)
DEFINITION_EVENT_CLASS(ReceiveQteEnable, uint32, uint8)
DEFINITION_EVENT_CLASS(ReceiveQteEnableEnd)
DEFINITION_EVENT_CLASS(ReceiveSpawnControlNpc)

DEFINITION_EVENT_CLASS(ReceiveControlChat, uint32, uint8, int32)

DEFINITION_EVENT_CLASS(DeliveryLevelUpSkill,			FB2LevelUpSkillInfo)
DEFINITION_EVENT_CLASS(DeliveryLevelUpSkillError)
DEFINITION_EVENT_CLASS(DeliveryResetSkill,				FB2ResetSkillInfo)
DEFINITION_EVENT_CLASS(DeliveryBuySkillPoint,			FB2BuySkillPointInfo)
DEFINITION_EVENT_CLASS(DeliverySetUsingSkill,			FB2SetUsingSkillInfo)
DEFINITION_EVENT_CLASS(DeliveryCharacterNickName)
DEFINITION_EVENT_CLASS(DeliveryChangeNickName,			FB2UpdateNicknamePtr)
DEFINITION_EVENT_CLASS(DeliveryUserNickName)

DEFINITION_EVENT_CLASS(DeliveryEnhanceRankNode,			FB2EnhanceRankNode)

DEFINITION_EVENT_CLASS(DeliveryGetChatChannel,			FB2GetChatChannelResult)
DEFINITION_EVENT_CLASS(DeliveryChangeChatChannel)

DEFINITION_EVENT_CLASS(DeliveryGetMailList,				FB2MailList)
DEFINITION_EVENT_CLASS(DeliveryOpenMail,				int64, FB2ResponseOpenMailPtr)
DEFINITION_EVENT_CLASS(DeliveryOpenMailError,			int32, FB2ResponseOpenMailPtr)
DEFINITION_EVENT_CLASS(DeliveryOpenAllMail,				FB2OpenAllMail)
DEFINITION_EVENT_CLASS(DeliveryOpenAllMailError,		int32)

DEFINITION_EVENT_CLASS(DeliveryGeneralLotteryItemResult,  FB2Item, ESummonItemDifficulty)

DEFINITION_EVENT_CLASS(CacheSendAskFriendAccountId, int64)
DEFINITION_EVENT_CLASS(RemoveSendAskFriendAccountId, int64)
DEFINITION_EVENT_CLASS(DeliveryAskFriend)
DEFINITION_EVENT_CLASS(DeliveryAcceptAskFriend)
DEFINITION_EVENT_CLASS(DeliveryRejectAskFriend)
DEFINITION_EVENT_CLASS(DeliveryGetFriendList,			FB2FriendList)
DEFINITION_EVENT_CLASS(DeliveryCancelAskFriend)
DEFINITION_EVENT_CLASS(DeliveryDeleteFriend,			FB2DeleteFriend)
DEFINITION_EVENT_CLASS(DeliveryGetAllSendAskFriend,		FBAllSendAskFriendList)
DEFINITION_EVENT_CLASS(DeliveryGetAllReceiveAskFriend,	FB2AllReceiveAskFriendList)
DEFINITION_EVENT_CLASS(DeliverySendSocialPoint,			FB2SendSocialPoint)
DEFINITION_EVENT_CLASS(DeliveryGetRecommendFriend,		FB2RecommendFriendList)
DEFINITION_EVENT_CLASS(DeliveryFindUser,				FB2FindUser)

DEFINITION_EVENT_CLASS(OnFriendSideMenuSelect, int32)

DEFINITION_EVENT_CLASS(DeliveryKakaoInviteFriend,		EKakaoInviteResultCode)

DEFINITION_EVENT_CLASS(DeliveryGetQuest,				FB2GetQuest)
DEFINITION_EVENT_CLASS(DeliveryGetQuestUI,				FB2GetQuest)
DEFINITION_EVENT_CLASS(DeliveryAcceptQuest,				FB2AcceptQuest)
DEFINITION_EVENT_CLASS(DeliveryReceiveQuestReward,		FB2ReceiveQuestReward)
DEFINITION_EVENT_CLASS(DeliveryQuestReward,				Quest, TArray<b2network::B2RewardPtr>)
DEFINITION_EVENT_CLASS(DeliveryeGiveUpQuest,			FB2GiveUpQuestPtr)
DEFINITION_EVENT_CLASS(InformQuestFromYesterday,		TArray<int32>)
DEFINITION_EVENT_CLASS(OnQuestCompleteNotify,			int32)

DEFINITION_EVENT_CLASS(TeamMatchChangeCharacterCam,		bool, int32)

DEFINITION_EVENT_CLASS(DeliveryStartCounterDungeon,		FB2ResponseStartCounterDungeon)
DEFINITION_EVENT_CLASS(DeliveryForceStartCounterDungeon, FB2ResponseForceStartCounterDungeonPtr)
DEFINITION_EVENT_CLASS(DeliveryFinishCounterDungeon,	FB2FinishCounterDungeonInfo)
DEFINITION_EVENT_CLASS(DeliverySweepCounterDungeon,		FB2SweepCounterDungeonInfo)
DEFINITION_EVENT_CLASS(DeliveryGetCounterDundeon,		FB2GetCounterDungeon)
DEFINITION_EVENT_CLASS(DeliveryCounterDungeonWeeklyReward, FB2CounterDungeonWeeklyReward)
DEFINITION_EVENT_CLASS(DeliveryBuyTicketCounterDungeon, FB2CounterDungeonBuyTicket)

DEFINITION_EVENT_CLASS(DeliveryGetMission,				FB2GetMission)
DEFINITION_EVENT_CLASS(DeliveryReceiveMissionReward,	FB2ReceiveMissionReward)
DEFINITION_EVENT_CLASS(DeliveryReceiveMissionRewardError, const int32)
DEFINITION_EVENT_CLASS(DeliveryReceiveCompleteMissionReward, FB2ReceiveCompleteMissionReward)

DEFINITION_EVENT_CLASS(DeliveryReceiveCreateGuild,		FB2ReceiveCreateGuild)
DEFINITION_EVENT_CLASS(DeliveryReceiveGuildDetailInfo, FB2ReceiveGuildInfo)
DEFINITION_EVENT_CLASS(DeliveryReceiveLeaveGuild,		FB2ResponseLeaveGuildPtr)
DEFINITION_EVENT_CLASS(DeliveryReceiveGetRecommendGuild, FB2ReciveRecommenGuild)
DEFINITION_EVENT_CLASS(DeliveryReceiveAskJoinGuild)
DEFINITION_EVENT_CLASS(DeliveryReceiveAppointSubGuildMaster, FB2ReciveAppointSubGuildMasterInfo)
DEFINITION_EVENT_CLASS(DeliveryReceiveDismissSubGuildMaster, FB2ReciveDismissSubGuildMasterInfo)
DEFINITION_EVENT_CLASS(DeliveryReceiveKickGuildMember)
DEFINITION_EVENT_CLASS(DeliveryReceiveJoinGuildBattle)
DEFINITION_EVENT_CLASS(DeliveryGetGuildBattleInfo,		FB2ResponseGetGuildBattlePtr)
DEFINITION_EVENT_CLASS(DeliveryStartGuildBattle,		FB2ResponseStartGuildBattlePtr)
DEFINITION_EVENT_CLASS(DeliverySetGuildBattleEntry,		FB2ResponseSetGuildBattlePtr)
DEFINITION_EVENT_CLASS(DeliveryFinishGuildBattle,		FB2ResponseFinishGuildBattlePtr)
DEFINITION_EVENT_CLASS(DeliverGetGuildInviteList,		FB2ResponseGetGuildInviteListPtr)
DEFINITION_EVENT_CLASS(DeliveryGetGuildMemberCandidates, FB2ResponseGetGuildMemberCandidatesPtr)
DEFINITION_EVENT_CLASS(DeliveryInviteGuild)
DEFINITION_EVENT_CLASS(DeliveryRejectInviteGuild)
DEFINITION_EVENT_CLASS(DeliveryAcceptInviteGuild)
DEFINITION_EVENT_CLASS(DeliveryGuildBattleHistory,		FB2ResponseGuildBattleHistoryPtr)
DEFINITION_EVENT_CLASS(DeliveryUpdateGuild,				FB2ResponseUpdateGuildPtr)
DEFINITION_EVENT_CLASS(DeliveryUpdateGuildNotice,		FB2ResponseUpdateGuildNoticePtr)
DEFINITION_EVENT_CLASS(DeliveryFindGuild,				FB2ResponseFindGuildPtr)
DEFINITION_EVENT_CLASS(DeliverytGetGuildRankingList,	FB2ResponseGetGuildRankingListPtr)
DEFINITION_EVENT_CLASS(DeliverytGuildBattleSeasonReward,FB2ResponseGuildBattleSeasonRewardPtr)

DEFINITION_EVENT_CLASS(DeliverytLevelUpGuildSkill,		 FB2ResponseLevelUpGuildSkillPtr)
DEFINITION_EVENT_CLASS(DeliverytBuyGuildSkill,			FB2ResponseBuyGuildSkillPtr)
DEFINITION_EVENT_CLASS(DeliverytObtainGuildMaster, FB2ResponseObtainGuildMasterPtr)
DEFINITION_EVENT_CLASS(DeliverytHandOverGuildMaster, FB2ResponseHandOverGuildMasterPtr)

DEFINITION_EVENT_CLASS(DeliveryGuildBattleTurnResult,	FB2ResponseGuildBattleTurnResultPtr)
DEFINITION_EVENT_CLASS(DeliveryGuildBattleTurnReward,	FB2ResponseGuildBattleTurnRewardPtr)

DEFINITION_EVENT_CLASS(DeliveryGetFriendRanking,		FB2ResponseGetFriendRankingsPtr)
DEFINITION_EVENT_CLASS(GuildStateChangeEvent,			int32)
DEFINITION_EVENT_CLASS(GuildMatchChangeCharacterCam,	bool, int32)

DEFINITION_EVENT_CLASS(GuildBuffSettting,				const TArray<b2network::B2GuildSkillPtr>&)

DEFINITION_EVENT_CLASS(DeliveryuildDonation,			FB2ResponseGuildDonationPtr)
DEFINITION_EVENT_CLASS(DeliveryuildDonatioError)

DEFINITION_EVENT_CLASS(DeliveryCheckReachPower,			FB2ResponseCheckReachPowerPtr)
DEFINITION_EVENT_CLASS(DeliveryTrackingPowerDetail,		FB2ResponseTrackingPowerDetailPtr)

DEFINITION_EVENT_CLASS(DeliveryGetHeroTower,			FB2ResponseGetHeroTowerPtr)
DEFINITION_EVENT_CLASS(DeliveryStartHeroTower,			FB2ResponseStartHeroTowerPtr)
DEFINITION_EVENT_CLASS(DeliveryFinishHeroTower,			FB2ResponseFinishHeroTowerPtr)
DEFINITION_EVENT_CLASS(DeliveryAbandonHeroTower,		FB2ResponseAbandonHeroTowerPtr)
DEFINITION_EVENT_CLASS(DeliverySweepHeroTower,			FB2ResponseSweepHeroTowerPtr)
DEFINITION_EVENT_CLASS(DeliveryBuyHeroTowerTicket,		FB2ResponseBuyHeroTowerTicketPtr)

DEFINITION_EVENT_CLASS(DeliveryGetAssaultBattleStatus,	FB2ResponseGetAssaultBattleStatusPtr)
DEFINITION_EVENT_CLASS(DeliveryAssaultResult,			FB2ResponseAssaultResultPtr)
DEFINITION_EVENT_CLASS(ReceivedBattleGradeInfo,			uint32, int32, int32)
DEFINITION_EVENT_CLASS(ReceivedOtherToken,				uint32, const FString&)

DEFINITION_EVENT_CLASS(DeliveryGetLotteryShop,			FB2ResponseGetLotteryShopPtr)
DEFINITION_EVENT_CLASS(DeliveryDrawShopLottery,			FB2ResponseDrawShopLotteryPtr)
DEFINITION_EVENT_CLASS(DeliveryGetGeneralShop,			FB2ResponseGetGeneralShopPtr)
DEFINITION_EVENT_CLASS(DeliveryBuyShopProduct,			FB2ResponseBuyShopProductPtr)
DEFINITION_EVENT_CLASS(DeliveryRewardShopMileage,		FB2ResponseRewardShopMileagePtr)

DEFINITION_EVENT_CLASS(DeliveryGetMagicShop,			FB2ResponseGetMagicShopPtr)
DEFINITION_EVENT_CLASS(DeliveryRenewalMagicShop,		FB2ResponseRenewalMagicShopPtr)
DEFINITION_EVENT_CLASS(DeliveryPurchaseMagicShop,		FB2ResponsePurchaseMagicShopPtr)

DEFINITION_EVENT_CLASS(DeliveryGetLatestEventData,		FB2ResponseGetLatestEventDataPtr)

DEFINITION_EVENT_CLASS(DeliveryGetPackageShop,			FB2ResponseGetPackageShopPtr)
DEFINITION_EVENT_CLASS(DeliveryGetPeriodPackageState,	FB2ResponseGetPeriodPackageStatePtr)

DEFINITION_EVENT_CLASS(DeliveryResponseRewardLevelUpPackage, FB2ResponseRewardLevelUpPackagePtr)
DEFINITION_EVENT_CLASS(DeliveryResponseRewardReachLevelPackage, FB2ResponseRewardReachLevelPackagePtr)
DEFINITION_EVENT_CLASS(DeliveryResponseRewardFlatRatePackageBonus, FB2ResponseRewardFlatRatePackageBonusPtr)
DEFINITION_EVENT_CLASS(DeliveryResponseGetContentsModeState, FB2ResponseGetContentsModeStatePtr)

DEFINITION_EVENT_CLASS(DeliveryResponseGetDuelModeInfo, FB2ResponseGetDuelModeInfoPtr)
DEFINITION_EVENT_CLASS(DeliveryResponseGetChallengeModeInfo, FB2ResponseGetChallengeModeInfoPtr)

DEFINITION_EVENT_CLASS(DeliveryResponseGetHallOfFame,		FB2ResponseGetHallOfFamePtr)
DEFINITION_EVENT_CLASS(DeliveryResponsePraiseTargetRanker,	FB2ResponsePraiseTargetRankerPtr)

DEFINITION_EVENT_CLASS(DeliveryEnhanceRelic,			FB2ResponseEnhanceRelicPtr)
DEFINITION_EVENT_CLASS(DeliveryPromotionRelic,			FB2ResponsePromotionRelicPtr)
DEFINITION_EVENT_CLASS(DeliveryGetAccountRelic,			FB2ResponseGetAccountRelicPtr)
DEFINITION_EVENT_CLASS(DeliveryDailyAttendance,			FB2ReceiveDailyAttendance)

DEFINITION_EVENT_CLASS(StopRelicAutoEnchant)


DEFINITION_EVENT_CLASS(DeliveryGetItemRandomOption,			FB2ResponseGetItemRandomOptionPtr)
DEFINITION_EVENT_CLASS(DeliveryGenerateItemRandomOption,	FB2ResponseGenerateItemRandomOptionPtr)
DEFINITION_EVENT_CLASS(DeliveryApplyItemRandomOption,		FB2ResponseApplyItemRandomOptionPtr)
DEFINITION_EVENT_CLASS(DeliveryDiscardItemRandomOption,		FB2ResponseDiscardItemRandomOptionPtr)

DEFINITION_EVENT_CLASS(DeliveryGetItemRandomQuality,		FB2ResponseGetItemRandomQualityPtr)
DEFINITION_EVENT_CLASS(DeliveryGenerateItemRandomQuality,	FB2ResponseGenerateItemRandomQualityPtr)
DEFINITION_EVENT_CLASS(DeliveryApplyItemRandomQuality,		FB2ResponseApplyItemRandomQualityPtr)
DEFINITION_EVENT_CLASS(DeliveryDiscardItemRandomQuality,	FB2ResponseDiscardItemRandomQualityPtr)

DEFINITION_EVENT_CLASS(DeliveryCheckRedDot, FB2MessageInfoResponseCheckRedDotPtr)
DEFINITION_EVENT_CLASS(DeliveryModifyItemNewFlag)
DEFINITION_EVENT_CLASS(DeliveryModifyCostumeNewFlag)
DEFINITION_EVENT_CLASS(DeliveryResponseFindAccount, FB2FindAccountPtr)

DEFINITION_EVENT_CLASS(DeliveryMatchMaking, FB2TagMatchMakingPtr)

DEFINITION_EVENT_CLASS(NotifyInputTouchBeganInViewPort)

DEFINITION_EVENT_CLASS(DeliveryOpenDuelRewardBox, FB2OpenDuelRewardBoxPtr)
DEFINITION_EVENT_CLASS(DeliveryGetDuelRewardBoxInfo, FB2GetDuelRewardBoxInfoPtr)


DEFINITION_EVENT_CLASS(DeliveryEventAttendance, FB2ResponseEventAttendancePtr)
DEFINITION_EVENT_CLASS(DeliveryBeginnerAttendance, FB2ResponseBeginnerAttendancePtr)
DEFINITION_EVENT_CLASS(DeliveryUpdateDailyPlayTime, FB2ResponseUpdateDailyPlayTimePtr)
DEFINITION_EVENT_CLASS(DeliveryDailyPlayTimeReward, FB2ResponseReceiveDailyPlayTimeRewardPtr)
DEFINITION_EVENT_CLASS(DeliveryGetDuelRewardBoxGainableList, FB2GetDuelRewardBoxGainableListPtr)
DEFINITION_EVENT_CLASS(DeliveryAssaultMvpReward, FB2AssaultMvpRewardPtr)

DEFINITION_EVENT_CLASS(DeliveryGetTutorialProgress, FB2ResponseGetTutorialProgressPtr)
DEFINITION_EVENT_CLASS(DeliverySetTutorialProgress, FB2ResponseSetTutorialProgressPtr)
DEFINITION_EVENT_CLASS(DeliveryUpdateTutorialProgress, FB2ResponseUpdateTutorialProgressPtr)
DEFINITION_EVENT_CLASS(DeliverySkipTutorialProgress, FB2ResponseSkipTutorialProgressPtr)
DEFINITION_EVENT_CLASS(DeliveryDrawTutorialItem, FB2ResponseDrawTutorialItemPtr)
DEFINITION_EVENT_CLASS(DeliveryPrepareTutorial, FB2ResponsePrepareTutorialPtr)
DEFINITION_EVENT_CLASS(DeliveryGiveTutorialReward, FB2ResponseGiveTutorialRewardPtr)
DEFINITION_EVENT_CLASS(CahcedCheckAndProgressTutorial)

DEFINITION_EVENT_CLASS(DeliveryAskGuildSupport, FB2ResponseAskGuildSupportPtr)
DEFINITION_EVENT_CLASS(DeliveryGuildSupport, FB2ResponseGuildSupportPtr)
DEFINITION_EVENT_CLASS(DeliveryGetGuildChat, FB2ResponseGetGuildChatPtr)

DEFINITION_EVENT_CLASS(NotifyGuildChatBadge, int32)

DEFINITION_EVENT_CLASS(DeliveryGetUnitySkillMission, FB2ResponseGetUnitySkillMissionPtr)
DEFINITION_EVENT_CLASS(DeliveryReceiveUnitySkillReward, FB2ResponseUnitySkillRewardPtr)
DEFINITION_EVENT_CLASS(DeliveryAwakenUnitySkillMission, FB2ResponseAwakenUnitySkillMissionPtr)
DEFINITION_EVENT_CLASS(DeliveryAwakenUnitySkill, FB2ResponseAwakenUnitySkillPtr)

DEFINITION_EVENT_CLASS(DeliveryCheckModeOpen, FB2ResponseCheckModeOpenPtr)
DEFINITION_EVENT_CLASS(DeliveryRestReward, FB2ResponseRestRewardPtr)
DEFINITION_EVENT_CLASS(DeliveryGetGuildMember, FB2ResponseGuildMemberPtr)

DEFINITION_EVENT_CLASS(DeliveryGetEvent, FB2ResponseGetEventPtr)
DEFINITION_EVENT_CLASS(DeliveryLevelUpEventReward, FB2ResponseReceiveLevelUpEventRewardPtr)
DEFINITION_EVENT_CLASS(DeliveryStageClearEventReward, FB2ResponseReceiveStageClearEventRewardPtr)
DEFINITION_EVENT_CLASS(DeliveryRewardPointShopEvent, FB2ResponseRewardPointShopEventdPtr)
DEFINITION_EVENT_CLASS(DeliveryRewardTenLotteryEvent, FB2ResponseRewardTenLotteryEventPtr)
DEFINITION_EVENT_CLASS(DeliveryRewardGemSpendEvent, FB2ResponseRewardSpendGemEventPtr)
DEFINITION_EVENT_CLASS(DeliveryRewardGemPurchaseEvent, FB2ResponseReceiveGemPurchaseEventRewardPtr)
DEFINITION_EVENT_CLASS(DeliveryRewardTenLotterySavingsEvent, FB2ResponseRewardTenLotterySavingsEventPtr)
DEFINITION_EVENT_CLASS(DeliveryRollDiceEvent, FB2ResponseRollDiceEventPtr)
DEFINITION_EVENT_CLASS(DeliveryBuyDiceEventPoint, FB2ResponseBuyDiceEventPointPtr)
DEFINITION_EVENT_CLASS(DeliveryRewardDiceEventMileage, FB2ResponseRewardDiceEventMileagePtr)

DEFINITION_EVENT_CLASS(DeliveryPointEventReward, FB2ResponseReceivePointEventRewardPtr)

DEFINITION_EVENT_CLASS(DeliveryGetAccountEther, FB2GetAccountEtherPtr)
DEFINITION_EVENT_CLASS(DeliveryMountEther, FB2MountEtherPtr)
DEFINITION_EVENT_CLASS(DeliveryUnmountEther, FB2UnmountEtherPtr)
DEFINITION_EVENT_CLASS(DeliverySellEther, FB2SellEtherPtr)
DEFINITION_EVENT_CLASS(DeliverySellEtherError)
DEFINITION_EVENT_CLASS(DeliveryDecompositionEther, FB2DecompositionEtherPtr)
DEFINITION_EVENT_CLASS(DeliveryDecompositionEtherError)
DEFINITION_EVENT_CLASS(DeliveryEnhanceEther, FB2EnhanceEtherPtr)
DEFINITION_EVENT_CLASS(DeliveryEnhanceEther2630, FB2EnhanceEtherPtr)
DEFINITION_EVENT_CLASS(DeliveryLockEther, FB2LockEtherPtr)
DEFINITION_EVENT_CLASS(DeliveryUnlockEther, FB2UnlockEtherPtr)
DEFINITION_EVENT_CLASS(DeliverySelectEtherDefenseOption, FB2SelectEtherDefenseOptionPtr)
DEFINITION_EVENT_CLASS(DeliverySelectEtherUnitySkillOption, FB2SelectEtherUnitySkillOptionPtr)
DEFINITION_EVENT_CLASS(DeliveryModifyEtherNewFlag, FB2ModifyEtherNewFlagPtr)
DEFINITION_EVENT_CLASS(DeliveryExtendEtherInventory, FB2ExtendEtherInventoryPtr)

DEFINITION_EVENT_CLASS(DeliveryGetContentsOpenStatus, FB2ResponseGetContentsOpenStatusPtr)

DEFINITION_EVENT_CLASS(DeliveryKakaoMessageProcessing, FB2ResponseKakaoMessageProcessingPtr)

DEFINITION_EVENT_CLASS(DeliveryKakaoAccountConversionPrepare, FB2ReciveKakaoAccountConversionPrepare)
DEFINITION_EVENT_CLASS(DeliveryKakaoAccountConversionComplete, FB2ReciveKakaoAccountConversionComplete)

DEFINITION_EVENT_CLASS(DeliveryGetFeatureMission, FB2ReciveGetFeaturedMission)

DEFINITION_EVENT_CLASS(DeliveryPlatformFriendInviteInfo, FB2ResponseFriendInviteRewardInfoPtr)
DEFINITION_EVENT_CLASS(DeliveryFriendInviteCountUp, FB2ResponseFriendInviteCountUpPtr)
DEFINITION_EVENT_CLASS(DeliveryFriendJoinerCountUp, FB2ResponseFriendJoinerCountUpPtr)
DEFINITION_EVENT_CLASS(DeliveryFriendInviteReward, FB2ResponseFriendInviteRewardPtr)

DEFINITION_EVENT_CLASS(DeliveryIAPGetMarketProduct, FB2ResponseIAPGetMarketProductPtr)
DEFINITION_EVENT_CLASS(DeliveryIAPPreparePurchase, FB2ResponseIAPPreparePurchasePtr)
DEFINITION_EVENT_CLASS(DeliveryIAPVerifyPurchase, FB2ResponseIAPVerifyPurchasePtr)
DEFINITION_EVENT_CLASS(DeliveryIAPCancelPurchase, FB2ResponseIAPCancelPurchasePtr)
DEFINITION_EVENT_CLASS(DeliveryIAPFinishPurchase, FB2ResponseIAPFinishPurchasePtr)

DEFINITION_EVENT_CLASS(DeliveryGetSeasonMission, FB2ResponseGetSeasonMissionPtr)
DEFINITION_EVENT_CLASS(DeliveryRewardSeasonMission, FB2ResponseRewardSeasonMissionPtr)
DEFINITION_EVENT_CLASS(DeliveryAttendSeasonMission, FB2ResponseAttendSeasonMissionPtr)
DEFINITION_EVENT_CLASS(SelectSeasonEventTab, int32)
DEFINITION_EVENT_CLASS(SeasonEventGotoLinkScene, int32, int32)

DEFINITION_EVENT_CLASS(DeliveryBuyHotTimeBuff, FB2ResponseBuyHotTimeBuffPtr)

DEFINITION_EVENT_CLASS(DeliveryReportUser)

DEFINITION_EVENT_CLASS(DeliveryBlcokUser, FB2ResponseBlockChatPtr)
DEFINITION_EVENT_CLASS(DeliveryUnBlcokUser)
DEFINITION_EVENT_CLASS(DeliveryGetBlockChatList, FB2ResponseGetBlockChatListPtr)

DEFINITION_EVENT_CLASS(DeliveryClientStateReport, FB2ResponseClientStateReportPtr)

DEFINITION_EVENT_CLASS(DeliveryUnityAwakeFail)

DEFINITION_EVENT_CLASS(UpdateFairyStatus)
DEFINITION_EVENT_CLASS(UpdateFairyLevelUp, EFairyType, bool)
DEFINITION_EVENT_CLASS(UpdateFairyLevelUpError, int32)
DEFINITION_EVENT_CLASS(UpdateFairyBless, EFairyType)
DEFINITION_EVENT_CLASS(UpdateFairyAutoLevelUp, bool)

DEFINITION_EVENT_CLASS(DeliveryGetAccountCostume, FB2GetAccountCostumePtr)
DEFINITION_EVENT_CLASS(DeliveryEquipCostume, FB2EquipCostumePtr)
DEFINITION_EVENT_CLASS(DeliveryUnequipCostume, FB2UnequipCostumePtr)
DEFINITION_EVENT_CLASS(DeliveryEnhanceCostume, FB2EnhanceCostumePtr)
DEFINITION_EVENT_CLASS(DeliveryDismantleCostumes, FB2DismantleCostumesPtr)
DEFINITION_EVENT_CLASS(DeliveryLockCostumes, FB2LockCostumesPtr)
DEFINITION_EVENT_CLASS(DeliveryUnlockCostumes, FB2UnlockCostumesPtr)
DEFINITION_EVENT_CLASS(DeliveryGetCostumeShop, FB2GetCostumeShopPtr)
DEFINITION_EVENT_CLASS(DeliveryPurchaseCostume, FB2PurchaseCostumePtr)
DEFINITION_EVENT_CLASS(DeliverySetCostumeVisible, FB2SetCostumeVisiblePtr)
DEFINITION_EVENT_CLASS(DeliveryAddCostumeToInventory, FB2AddCostumeToInventoryPtr)

DEFINITION_EVENT_CLASS(DeliveryDismantleCostumesFail)
DEFINITION_EVENT_CLASS(DeliveryEnhanceCostumesFail)

DEFINITION_EVENT_CLASS(DeliveryGetChangeItemCharacterTypeCost, FB2ResponseGetChangeItemCharacterTypeCostPtr)
DEFINITION_EVENT_CLASS(DeliveryChangeItemCharacterType, FB2ResponseChangeItemCharacterTypePtr)

DEFINITION_EVENT_CLASS(DeliveryJackpotFairy, FB2ServerCmdPtr)

//dimension
DEFINITION_EVENT_CLASS(DeliveryStartDimension, FB2ResponseStartDimensionPtr)
DEFINITION_EVENT_CLASS(DeliveryFinishDimension, FB2ResponseFinishDimensionPtr)
DEFINITION_EVENT_CLASS(DeliveryGetDimension, FB2ResponseGetDimensionPtr)
DEFINITION_EVENT_CLASS(DimensionBuffCheck)

// Totem
DEFINITION_EVENT_CLASS(EnterTotemSmeltingMode, int64)
DEFINITION_EVENT_CLASS(OpenTotemSmeltingInfoPopup, int64)
DEFINITION_EVENT_CLASS(ReturnTotemInventory, int64)
DEFINITION_EVENT_CLASS(LobbyReqSmeltingTotem, int64, int32, int32, bool)
DEFINITION_EVENT_CLASS(DeliveryGetAccountTotem, FB2GetAccountTotemPtr)
DEFINITION_EVENT_CLASS(DeliveryDismantleTotems, FB2DismantleTotemsPtr)
DEFINITION_EVENT_CLASS(DeliveryEquipTotems, FB2EquipTotemsPtr)
DEFINITION_EVENT_CLASS(DeliveryUnequipTotems, FB2UnequipTotemsPtr)
DEFINITION_EVENT_CLASS(DeliveryLockTotems, FB2LockTotemsPtr)
DEFINITION_EVENT_CLASS(DeliveryUnlockTotems, FB2UnlockTotemsPtr)
DEFINITION_EVENT_CLASS(DeliveryRefineTotem, FB2RefineTotemPtr)
DEFINITION_EVENT_CLASS(DeliverySellTotems, FB2SellTotemsPtr)
DEFINITION_EVENT_CLASS(DeliveryModifyTotemNewFlag, FB2ModifyTotemNewFlagPtr)
DEFINITION_EVENT_CLASS(DeliveryAddTotemToInventory, FB2AddTotemToInventoryPtr)
DEFINITION_EVENT_CLASS(SuccesGetAccountTotem)
DEFINITION_EVENT_CLASS(LobbyReqToggleTotemLock, int64, bool)
DEFINITION_EVENT_CLASS(UpdateTotemInfoPopupControl, FB2Totem)
DEFINITION_EVENT_CLASS(UpdateTotemSmeltingResult, bool, FB2Totem)
DEFINITION_EVENT_CLASS(LobbyReqSmeltingCompleteTotem)

//ItemForge
DEFINITION_EVENT_CLASS(DeliveryGetItemForgeList, FB2ResponseGetItemForgeListPtr)
DEFINITION_EVENT_CLASS(DeliveryTryItemForge, FB2ResponseTryItemForgePtr)
DEFINITION_EVENT_CLASS(DeliverySelectItemForge, FB2Item&)

//SealBox
DEFINITION_EVENT_CLASS(DeliveryUnsealBox, FB2ResponseUnsealBoxPtr)

// Replay
DEFINITION_EVENT_CLASS(StartPVPReplay, FString, int32, int32)


/*
	Events for DLC front
*/

DEFINITION_EVENT_CLASS(DLCFrontLaunchBladeIIGame)

DEFINITION_EVENT_CLASS(SealBoxSelectChar,int32)
DEFINITION_EVENT_CLASS(SealBoxCardFlip, int32, int32)