// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2NetGameMode.h"
#include "BladeIIGameMode.h"
#include "InfoAsset/B2PCClassInfo.h"
//#include "B2LobbyUISwitcher.h"
#include "B2UIManager.h"
#include "B2LobbySceneManager.h"
#include "BladeIIPlayer.h"
#include "CommonStruct.h"
#include "../UI/DJLegacy/B2LobbyUISwitcher.h"
#include "../UI/DJLegacy/B2UIManager_Lobby.h"
#include "B2LobbyGameMode.generated.h"

/** For Lobby level blueprint custom event. */
#define UPDATE_NATIVE_LOBBY_MATINEE_REF_EVENT TEXT("UpdateNativeLobbyMatineeRef")

struct FLobbyMatchPeerInfo
{
	FLobbyMatchPeerInfo()
		: IsReady(false)
		, ReservedPCClass(EPCClass::EPC_End)
		, PlayerTeamType(ETeamType::ETT_End)
	{}

	bool		IsReady;
	EPCClass	ReservedPCClass;	// 선택된 캐릭터를 동기화
	ETeamType	PlayerTeamType;		// 필요하면 팀구분
};


/**
 * Game mode base for Lobby. It is separated from BladeIIGameMode, as it is not expected to have something to do with gameplay, 
 * but if needed for any reason, consider extending from BladeIIGameMode, or implement part of BladeIIGameMode feature.
 */
UCLASS()
class BLADEII_API AB2LobbyGameMode : public AB2MatchGameModeBase
{
	GENERATED_BODY()

	struct TouchData
	{
		//TouchData() :bIsPressed(false), bIsPinching(false),
		//	MainFingerPrevLocation(FVector::ZeroVector),
		//	SubFingerPrevLocation(FVector::ZeroVector)
		//{ }
		bool bIsPressed;
		ETouchIndex::Type MainFingerIndex;
		ETouchIndex::Type SubFingerIndex;
		FVector2D MainFingerPrevLocation;
		FVector2D SubFingerPrevLocation;

		bool bIsPinching;
	};	

#define MAX_INPUT_HISTORY_INDEX 10
	struct FInputHistory
	{
		struct FInputHistoryElem
		{
			FInputHistoryElem()
			{
				CheckPointTime = 0.0f;
				CheckPointPosition = FVector2D(0.0f, 0.0f);
				bInitialized = false;
			}
			float CheckPointTime;
			FVector2D CheckPointPosition;
			bool bInitialized;
		};
		
		FInputHistory()
		{
			InputHistoryLastIndex = INDEX_NONE;
			FMemory::Memzero(&InputPointHistory, sizeof(FInputHistoryElem) * MAX_INPUT_HISTORY_INDEX);
		}

		int32 InputHistoryLastIndex;
		FInputHistoryElem InputPointHistory[MAX_INPUT_HISTORY_INDEX];

		void InitializeHistory(
#if !UE_BUILD_SHIPPING
			const FString& OptionalLoggingHistoryName = TEXT("")
#endif
		);
		void AddCheckPoint(float Time, const FVector2D& Position);
		bool IsSpeedOverDeltaX(float DeltaX, float Time, float& OutDeltaX);
		/** See if the last input history odd in time and delta, compared to other older histories */
		bool IsLastHistoryOdd(float MinOddTimeInterval, float MinOddPosDeltaRatio);
	private:
		// Helper to get history array index in circular manner.
		FORCEINLINE int32 GetPrevHistoryIndexOf(int32 CurrHistoryIndex) const { return ((CurrHistoryIndex + MAX_INPUT_HISTORY_INDEX - 1) % MAX_INPUT_HISTORY_INDEX); }
		FORCEINLINE int32 GetNextHistoryIndexOf(int32 CurrHistoryIndex) const { return ((CurrHistoryIndex + MAX_INPUT_HISTORY_INDEX + 1) % MAX_INPUT_HISTORY_INDEX); }
	};
	
public:
	AB2LobbyGameMode(const FObjectInitializer& ObjectInitializer);
	virtual EB2GameMode GetB2GameModeType() override { return EB2GameMode::Lobby; }

	// GameMode override
public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorCustomEvent(FName OptionalEventName = NAME_None, UObject* OptionalNotifyingObject = NULL) /*override*/;
	virtual void BeginPlay() override;

	//UFUNCTION(BlueprintCallable, Category = "Game")
	virtual void StartPlay() override;

	virtual void OnLoadingScreenFinished() override;

	virtual void BeginDestroy() override;

	void BattleStart();
protected:
	virtual void PreBeginPlay() override;
	virtual void SetDesiredPCClassEnum() override;
	virtual void SubscribeEvents() override;
	virtual void UpdateChatUI(bool bIsScrollEnd) override;
	virtual void ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel = EB2ResolutionLevel::End);
	virtual FString GetGameModeScalabilitySettingSectionPostfix() const;
	virtual void ExtraOnPreLoadMap() override;
	virtual void OnPlatformPause() override;
public:
	// NetGameMode interfaces...
	virtual void ClearMatchRoom();
	virtual void ClearInvitationGameInfo(EB2GameMode GameModeType) override;

	virtual void RequestGameInvite(const FString& NaickName, EB2GameMode GameModeType) override;
	virtual void RequestDeclineInvitation(EB2GameMode GameModeType) override;
	virtual void PrevAcceptGameInvite(const FString& Inviter, EB2GameMode GameModeType) override;

	virtual bool CanReceiveGameInvite(const FString& Inviter, EB2GameMode GameModeType) const override;

	virtual void OnReceiveInvitation(const FString& Inviter, EB2GameMode GameModeType);
	virtual void OnInvitationSuccess(const FString& Invitee, EB2GameMode GameModeType, const FString& InvitationRef);
	virtual void OnInvitationDeclined(const FString& Invitee, EB2GameMode GameModeType);


public:
	void ChangeLobbyScene(ELobbyScene LobbyScene, int32 OptionalChapterNum = -1); // OptionalChapterNum is required only for ELobbyScene_Chapter
	void ChangeLobbyScene(EUIScene LobbyUIScene, int32 OptionalChapterNum = -1); // OptionalChapterNum is required only for ELobbyScene_Chapter

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	ELobbyScene GetCurrentLobbyScene() const;

	class UB2LobbyInventory*	GetLobbyInventory();
	class UB2Airport*			GetLobbyAirport();

	void ViewportPressMouseEventStart();
	void ViewportTouchEventStart(bool bInputTypeTouch, const FVector2D& InPressedPosition);
	void SetEnableClickEvents(bool bEnable);
	void BindTouchInput(bool bBind);
	void CheckAndDoSwipe(const FVector2D& CurrentPos, const FVector2D& PrevPos);
	void CheckAndDoFlick(const FVector2D& CurrentPos);
	/// Temporary Lobby Camera work
	void TouchStart(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchMove(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchEnd(const ETouchIndex::Type FingerIndex, const FVector Location);
	void SwipeH(float Value);
	void SwipeV(float Value);
	void FlickH(float Value);
	void FlickV(float Value);
	void MouseX(float Value);
	void MouseY(float Value);
	void LeftControlPressed();
	void LeftControlReleased();
	void MouseMoved();
	FVector2D GetTouchingInputCenter();
	void CameraZoomIn();
	void CameraZoomOut();
	void CameraZoom(float Value);
	FReply TouchMoveFromUI(const FGeometry& Geometry, const FPointerEvent& Event);
	FReply TouchStartFromUI(const FGeometry& Geometry, const FPointerEvent& Event);
	FReply TouchEndFromUI(const FGeometry& Geometry, const FPointerEvent& Event);

	FORCEINLINE void SetFirstLobbyMain(bool bIsFirst) { IsFirstLobbyMain = bIsFirst; };
	FORCEINLINE const bool GetFirstLobbyMain() const { return IsFirstLobbyMain; };

private:
	void BeginLobbySceneFreeRotate(float InLastSwipeValue); // Free rotate after user touch and swipe end, either camera or character depend on the scene type.
	
	void OnChangeUIScene(EUIScene UIScene);
	void OnChangeUISceneBack();
public:

	class UB2PCClassInfo* GetCurrentSelectedPCClassInfo();
	void SetPCSelection(EPCClass InMainPCClass, EPCClass InSubPCClass);
	bool SetMainPCClass(EPCClass InMainPCClass);

	UFUNCTION(BlueprintPure, Category = "BladeII")
	bool IsCurrentUIScene(EUIScene CheckScene);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool SetMainPCClassBP(EPCClass InMainPCClass);
	
	void SceneForceUpdate();
	/** ====================================================
	 * Add~~ series methods here, adding level-placed resources to LobbySceneManager
	 * It could be better.. but seems too late to change it.
	 */

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void AddHallOfCharacterAnimation(EPCClass PCClass, class UAnimationAsset* PCAnimation);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void AddHallOfFrameCharacter(const TArray<FHallOfFameCharacterInfo>& HallOfFameCharacter);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void AddLevelToLobbyScene(ELobbyLevelType LobbyType, class ULevelStreaming* StreamingLevel);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void AddLobbyCharacter(EPCClass PCClass, const FLobbyCharacter& LobbyCharacter);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void AddLobbyATypeActorInfo(EPCClass PCClass, const FLobbyCharacterInfo& LobbyCharacter);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void AddLobbyGTypeActorInfo(EPCClass PCClass, const FLobbyCharacterInfo& LobbyCharacter);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void AddLobbyNPCCharacter(ELobbyNPCType InType, const FLobbyNPCCharacterInfo& InAddInfo);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void AddLobbyMainScenePointLights(const FLobbyMainScenePointLightInfo& InAddInfo);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void AddLobbyFairyFlyingActor(EFairyType InType, class AB2LobbyFlyingFairyDoumi* fairyactor);

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	//void AddSummonItemDisplayMatinee(bool bIsMultiple, bool bIsOpen, bool bIsFriendShip,  class ALevelSequenceActor * Matinee);

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	//void AddSummonItemDisplayMatineeResult(class ALevelSequenceActor * Matinee);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void PlaySummonItemDisplayMatineeResult();

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	//void OnFinishedHallOfFameDisplayMatinee(class ALevelSequenceActor * Matinee);

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	//void OnFinishedSummonItemDisplayMatinee(class ALevelSequenceActor * Matinee);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnFinishedSummonItemOpenState(bool State);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnFinishedSummonItemDisplayResultMatinee();

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	//void OnChangedToFxSummonItemDisplayMatinee(ALevelSequenceActor * Matinee);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void NotifyOpenCard(int32 InIndex);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 OpenRemainCardCount();
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool SelectLobbyMainCharacter(EPCClass SelectedClass);

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	//void SetDefaultCameraMatinee(class ALevelSequenceActor * DefaultMatinee);

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	//void SetChapterViewMatinee(class ALevelSequenceActor * ChapterMatinee);
	void UpdateNativeLobbyMatineeRef(); // It sends signal to level blueprint to set MatineeActor reference to native.

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void HideActorHasTag(FName Tag, bool bHide);

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	//void SetGuildMapChangeMatinee(class ALevelSequenceActor * Atype, class ALevelSequenceActor * Btype);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void GuildMapChangeMatineeEnd();

	void ResponseHallOfFameInfo(FB2ResponseGetHallOfFamePtr HallOfFameInfo);
	void ResponsePraiseTargetRanker(FB2ResponsePraiseTargetRankerPtr PraiseInfo);

	void EnterCostumeUIScene(const ECostumePage& eCostumePageType);
	
	void ResponseCheckReachPower(FB2ResponseCheckReachPowerPtr Message);

	void ResponseReceiveFairyGift(const FB2ReceiveFairyGiftPtr& pReceiveFairyGift);
	void UpdateFairyGiftState();

	/** Main entry point of getting into stage game mode combat. It just request to the server and client loading will begin after getting response */
	bool RequestStartGameStageWithSelectedPC(int32 ClientStageId, EStageDifficulty StageDifficulty, const TArray<EStageBuffType>& StageBuffTypes, bool bSkipStoryCinematic);
	void SetCharacterIntroModePCSelection(EPCClass InPCClass);

	// FindAccount Request
	void RequsetFindAccount(int64 AccountId, EPCClass PCClass1, EPCClass PCClass2, EPCClass PCClass3);

	void HeroTransientEffectBegin(const int32 Index);
	// * LobbyMatching ===============================================================
	virtual bool HasMatchStarted() const final;
	virtual void SetMatchState(FName NewState) final;
	virtual void HandleSuccessToMatchmake(data_trader::ChannelPtr ch, uint32 peer_role, const TArray<FB2RoomParticipantPtr>& participants) final;
	virtual void HandleRequestMatchMaking() final;
	virtual void HandleNotifyLeave(uint32) final;
	virtual void SetMyPlayerInfoData(uint32, const FB2FindAccountInfoPtr&) final;
	virtual void SetRemotePlayerInfoData(uint32, int64, const FB2FindAccountInfoPtr&) final;
	virtual void SetRoomParticipant(const TArray<FB2RoomParticipantPtr>&) final;
	virtual void HandleNotifyRoomClose(const FString& RoomToken, int32 RoomType) final;
	virtual void OnHostChanged(int32, int32) final;
	virtual void ResponseChangeRoomAccess(uint8 AccessKey) final;
	virtual void HandleWaitToMeetCondition() final;
	virtual void HandlePreparePlayers() final;
	virtual void LeaveRoomIfJoined() final;
	virtual bool CanRequestRoomClose() final;
	virtual bool CanRequestRoomCloseReason(ERoomCloseReasonType RoomCloseReason) final;
	virtual ERoomCloseReasonType GetStartRoomCloseReason() final;
	virtual int32 GetMatchModeMapId() final;
	virtual int32 GetMatchModeDifficulty() final;
	virtual void HandleFinishLobbyMatch() final;
	virtual void CheckFullPeerRoom() final;

	void SendPrerequisiteToStart(int32 RoomType);

	void StartLobbyMatch(bool IsRequestMatchMaking);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void StartFinishLobbyMatch();

	void HandleNotifyNoResponsePrerequisiteToStart();

	void HandleNotifyKickUser();	
	void BreakRoom(int32 InMapId = 1, int32 InDifficulty = 1);
	void IncreasePeerPlayers(uint32);

	void NetSendBattleGradeInfo(int32 BattleGrade, int32 BattleStarCount);
	void NetSendMatchToken();
	void NetAckBattleGradeInfo(uint32 NetId, int32 BattleGrade, int32 BattleStarCount);
	void SetOtherToken(uint32 InNetId, const FString& MatchToken);

	void NetSendMatchPlayerTeamInfo();
	void NetReceiveMatchPlayerTeamInfo(TMap<uint32, uint8>& TeamInfos);

	virtual void OnDeliveryRoomJoin(FB2ResponseRoomJoinPtr Message) override;
	virtual void OnDeliveryRoomJoin_Raid(FB2ResponseRoomJoinPtr Message);

	void PlayMainLobbyBgm();

	virtual void SetRemotePlayerClass(uint32, EPCClass) final;
	virtual void ResponsePrerequisiteToStart(int32 RoomType, const FString& RoomToken, TArray<FB2NetMatchPeerInfo> const&) final;
	virtual void ResponseAckPrerequisiteToStart(uint32 NetId) final;
		
	void EmplacePeerPlayer(int32 InNetId);
	void EmplacePeerPlayer(int32 InNetId, bool IsReady);
	void EmplacePeerPlayer(int32 InNetId, EPCClass InReservedPCClass);
	void EmplacePeerPlayer(int32 InNetId, ETeamType InTeamTpye);
	void RemovePeerPlayer(int32 InNetId);

	virtual bool CanStartInGame() final;

	ERoomCloseReasonType GetBreakRoomCloseReason();
	int32 GetMatchPlayerNum();
	int32 GetMatchPlayerNum(ETeamType teamType);
	int32 GetMaxMatchPlayerNum();
	bool IsMatchMaxPlayerNum();
	bool IsMatchMaxPlayerNumOver();

	virtual bool IsForceStartMatch(ELobbyScene LobbySceneType);

	ETeamType GetMyTeamType();
	bool IsPlayBeginAction();
	void ResetPlayBeginActionState(); // Let it enable to play again..

	void SendLeavePlayerOnlyRaid(uint32 NetId);
	// ===============================================================================

	// * RepeatBattle ================================================================
	FORCEINLINE FRepeatBattleStateSet GetRepeatBattleRequestState() { return RepeatBattleRequestState; }
	/** Very first start request of repeat battle, not for each repeat battle stage. */
	bool RequestStartRepeatBattleWithSelectedPC(int32 ClientStageId, EStageDifficulty StageDifficulty);
	virtual void CancelOrStopRepeatBattle() override;
	/** Executed when back from RepeatBattle stage, and still got remaining count. */
	void SetupForNextRepeatBattleStage(FServerStageID ServerStageId);
	void SyncRepeatBattleRequestStateToUIDoc();
	//================================================================================

	// * DJLegacy ============================================================================
	/** Send signal to LobbyUISwitcher to lobby ui page transition. -> They are legacy now. */
	FORCEINLINE class UB2UIManager_Lobby* DJLegacy_GetLobbyUIManager() { return LobbyUIManager; }
	void DJLegacy_ChangeLobbyUIPage(ELobbyUIPages InPageEnum);
	void DJLegacy_OpenLobbySubPopup(ELobbySubPopups InPopupEnum, FDJLegacySubpopupAddPanelInfo InOptionalAddPanelInfo = FDJLegacySubpopupAddPanelInfo());
	void DJLegacy_CloseLobbySubPopup(ELobbySubPopups InPopupEnum);
	/** Find from current created pop-up pages contained in LobbyUISwitcher. */
	class UB2LobbyUserWidget* DJLegacy_FindLobbySubPopup(ELobbySubPopups InUserWidgetEnum);
	class UB2LobbyUserWidget* DJLegacy_GetCurrLobbyUIPage();

	void DJLegacy_AsyncLoadUIPage(ELobbyUIPages InLoadUIPage, bool bBlocking);

	//================================================================================

	// * HeroMgmtMode ========================================================================
	/** Enter hero management mode UI screen. Use this for switching hero management sub mode too. */
	void EnterHeroMgmtMode(EHeroMgmtSubMode InSubMode);
	void QuitHeroMgmtMode();
	void SetHeroMgmtModePCSelection(EPCClass InPCClass);

UFUNCTION(BlueprintCallable, Category = "BladeII")
bool IsHeroMgmtMode() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	EPCClass GetHeroMgmtModePCClass();

	FORCEINLINE EHeroMgmtSubMode GetHeroMgmtSubMode() { return HeroMgmtSubMode; }
	FORCEINLINE void SetTouchingRotateAreaInHeroMgnt(bool bToSet){ bToSet ? bIsTouchingRotateAreaInHeroMgnt = bToSet : bPendingSetTouchingRotateAreaInHeroMgnt = true; }
	FORCEINLINE bool IsTouchingRotateAreaInHeroMgnt() { return bIsTouchingRotateAreaInHeroMgnt || bDeacceleratingCharRotate; }
	//================================================================================

	bool IsIgnoreTouchEnd();
	void UpdateChapterData(); 

	/** Change current selected chapter view num and its relevant data, also send it to UI. */
	void SetCombatEntryChapterNum(int32 InChapterNum, EStageDifficulty InDifficulty);

	/* When you press a subchapter button, and must send the StageInfoUI */
	void GoGameStageInfo(FServerStageID InServerStageId, FRepeatBattleStateSet& RepeatBattleSet);
	/* When you press a button, and must send the last Reached stage's informationUI. */
	void GoGameStageInfoLastReached();	
	void GoRaidGameMode(const FString& InvitedRoomId, EPCClass ChangeMainPCClass, int32 RaidType, int32 Step);
	void GameStageCheckForArinConsulting(FServerStageID InServerStageId);

	/** Set some object deployed at level for native manipulation. */
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetItemOpDirectingSceneObjects(class ACameraActor* InCamActor);
	void QuitItemOpDirectingView();
	/** Signal for LobbyGM that start showing one of Item OP mode progress scene. */
	void EnterItemOpDirectingView(ELobbyInvenItemOpMode InOpMode);
	bool IsInItemOpDirectingView();
	void EnterPVPMainMenu();
	void EnterTeamMatchMenu();
	void EnterControlMenu();
	void EnterTMChageHeroMod();
	virtual void EnterShop(int32 nWhereShop) override;
	//[@AKI, 170613] 약 네트워크에서 로비에서만 Disconnect를 해달라고 요청해서 만든 가상 메소드
	//필요한 모드에서는 오버라이드 해서 사용하면 되고 아니면 BladeIIGameMode에서 false를 return 함

	// Server-bound functions mainly be called from one of LobbyUserWidgets.
	void LoginAndStartGame(const FString& InUserID, const FString& InUserPW); // For the very first log-in.
	void CreateAccount(const FString& InUserID, const FString& InUserPW);
	void ProceedOnCharacterSelection();

	/** Update all skeletal meshes representing InPCClass in lobby scene. */
	void UpdateCharacterParts(EPCClass InPClass);
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	void DevForceUpdateCharacterParts(EPCClass InPCClass);
#endif
	void AnyOtherUpdateOnSetAccount(); // Kinda tired of making separate SetAcount call back update..

UFUNCTION(BlueprintCallable, Category = "BladeII")
int32 GetSelectedChapterViewNum() const;

	FORCEINLINE void SetPressedInputInUI(bool bSet) { bPressedInputInUI = bSet; }
	void OnReleasedLeftMouseButton();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	float GetHeroTowerUITowerPosition() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	EHeroTowerReadyUIState GetHeroTowerUIState() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 GetHeroTowerStartFloor() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 GetHeroTowerMaxFloor() const;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool IsHeroTowerMaxFloorCleared() const;

	//========================================================Guild
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void GuildMapInitCarmeraPosSetting(const FVector& Camera1, const FVector& Camera2, const FVector& Camera3);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void GuildMapFinishMatinee();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void GuildMapMoveScroll(ACameraActor* CameraActor);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void GuildMapDoneMoveScroll(ACameraActor* CameraActor);

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void OnCharacterEntryEnd(class ASkeletalMeshActor* ActorToPlayIdleAnim);

	UFUNCTION(BlueprintCallable, Category = BladeII)
	void SetGuildSelectEffects(class AEmitter* Maker_01, class AEmitter* Maker_02, class AEmitter* Maker_03, class AEmitter* Maker_04);

	void SetGuildEffectsLocation(int32 Type, FVector EffectLocation);

	void SelectLobbyFocusPCClass(EPCClass SelectClass);
	EPCClass GetSuitableFocusPCClass();

	bool IsFirstTimePreRenderPending() const;

	bool bTestShowLobbyWing;
	void ToggleShowLobbyWing();

	FORCEINLINE void SetDefaultMatineePlayedReverse(bool _bDefaultMatineePlayedReverse) { bDefaultMatineePlayedReverse = _bDefaultMatineePlayedReverse; };
	FORCEINLINE const bool GetDefaultMatineePlayedReverse() const { return bDefaultMatineePlayedReverse; };

	void SetPeerTeam();

	uint8 GetPlayerStartIndexByNetId(uint32 NetId, ETeamType PlayerTeamType);

	bool IsEnableLobbyScreenRotated() { return bEnableLobbyScreenRotate; }

	void StartControlTutorial();

	void NoResponsePrerequisiteToStart();
	void ClearWaitPrerequisiteToStartTimeHandler();

	void ReleaseWaitInventorySync();
private:
	TArray<class AEmitter*>		GuildEffects;

	struct FB2PeerInfo
	{
		uint32		NetId;
		uint8		Class;
		uint8		PlayerStartIndex;
		uint8 MatchGrade;
		uint8 MatchStar;
		ETeamType PlayerTeamType;
	};

	void GetControlPlayerInfo(TArray<FB2PeerInfo>& InPeerInfos);
	void GetTutorialControlPlayerInfo(TArray<FB2PeerInfo>& InPeerInfos);
private:
	bool CreateLobbyInventory();
	bool CreateLobbyAirport();
	void SetTimeToIgnoreTouchEnd(float LimitTime);
	void TickTimeTouchAction(float DeltaSeconds);
	void OnQuitHeroMgmtSubModeCommon(EHeroMgmtSubMode SubModeToClose);
	virtual void RealtimeUpdateUIDoc() override;
	void OnUpdateCombatPower(EPCClass UpdatedClass, bool PopupShow = false);
	void OnShowBattleScoreNotify(EPCClass EPC);
	void NotifyRoomInviteMessage(const FString& Inviter, const FString& InviteKey, const FString& RoomId, int32, int32);
	void ErrorNotEnoughGems();
	void ErrorNotEnoughGolds();
	void ErrorWrongCharacterNickname();	
	void ErrorNotEnoughHeroPiece();
	void ErrorNotEnoughStageAutoClearTicket();
	void ErrorNotEnoughDarkCystal();
	void ErrorNotEnoughFamePoint();
	void ErrorUserInRaidPenalty();
	void ErrorInventoryIsFull_RoomJoin();
	void ErrorInventoryIsFull_RoomJoin(TArray<b2network::B2InventoryCountServerInfoPtr> InInvenInfo);
	void ErrorRoomJoin();
	void ErrorRaidMatchMaking();
	void ErrorOpenWebEmptyURL();
	void ErrorChannelRoomClose();
	void ModRewardOpenPopup(FB2OpenDuelRewardBoxPtr RewardBoxInfo);
	void ModRewardBoxListInfo(FB2GetDuelRewardBoxInfoPtr BoxInfo);
	void ModRewardBoxDetailInfo(FB2GetDuelRewardBoxGainableListPtr BoxInfo);

	void CheckAndPopupKakaoPromotion();

	void RequestCombatPower();
	bool CheckCombatPower();

#if !UE_BUILD_SHIPPING
	void DevOnlyGenerateDummyChat(float DeltaSeconds);
#endif

private:
	/** Containing, managing, and sending signals to Lobby UI widgets. */
	UPROPERTY(Transient)
	class UB2UIManager_Lobby* LobbyUIManager;

	/** Items are contained here. */
	UPROPERTY()
	class UB2LobbyInventory* LobbyInventory;

	/** Inventory for Wing, the Airport. */
	UPROPERTY()
	class UB2Airport* LobbyAirport;

protected:
	/** For later extension.. we might provide multiple game mode extended from BladeIIGameMode */
	//TSubclassOf<class ABladeIIGameMode> GameModeToPlay;

	/** Whether it is currently in hero management mode, which includes equipment, level-up and skill menu within. */
	uint32 bHeroMgmtMode : 1;
	EHeroMgmtSubMode HeroMgmtSubMode; // Valid only when bHeroMgmtMode is true.
	EPCClass CharacterIntroModePCSelection;

	/** For combat entry stage selection page.
	* The combination of chapter and sub-chapter num is a unique stage. Refer to UB2StageInfo::GetClientStageIdOfChapterNums and GetInfoDataByChapterNums */
	int32 SelectedChapterViewNum;
	EStageDifficulty SelectedStageDifficulty;
	FLobbySceneManager LobbySceneManager;
	FRepeatBattleStateSet RepeatBattleRequestState; // Lobby has its own RepeatBattleState to transfer to in-game struct.

	UPROPERTY(BlueprintReadWrite, Category = BladeII)
	bool bDefaultMatineePlayedReverse;
private:
	static bool IsFirstLobby;
	static bool IsFirstLobbyMain;
	bool bPressedInputInUI;
	bool bIsTouchingRotateAreaInHeroMgnt;
	bool bPendingSetTouchingRotateAreaInHeroMgnt;
	bool bIsInputTypeTouch;
	bool bIsInputLeftControl;
	bool bBindTouchInput;
	bool bDeacceleratingCharRotate;

	float TimeLimitTouchToClick;
	float ElapsedTimeTouchAction;
	float ElapsedTimeTouchFlickStart;
	float ConsiderFlickDeltaTime; // Time parameter for use of InputHistory_CheckFlick
	float ConsiderFlickScreenDeltaRatio; // Relative pos parameter for use of InputHistory_CheckFlick
	float DeaccelerationCharRotate;
	float RotationSpeedCharRotate;
	float OddInputMinTimeInterval; // Time parameter for use of InputHistory_IgnoreCheck
	float OddInputMinPosDeltaRatio; // Relative pos parameter for use of InputHistory_IgnoreCheck

	FVector2D ScreenZoomCenter;
	FVector2D PressedInputPosition;
	FVector2D PressedFlickStartPosition;
	FVector2D PrevMousePosition;

	TouchData CurrentTouchData;

	FInputHistory InputHistory_CheckFlick; // InputHistory to check beginning of free rotate
	FInputHistory InputHistory_IgnoreCheck; // InputHistory for some hacky workaround.. of missing TouchEnd 

	TMap<uint32, FLobbyMatchPeerInfo>	AllPeerInfos;

	/** For signaling to specially provided feature, usually not implemented by engine functionality. */
	bool bLobbyLoadingScreenCompleted;
	bool bEnableLobbyScreenRotate;

	FTimerHandle WaitPrerequisiteToStartTimeHandler;
	bool bWaitPrerequisiteToStartForNonHost;

	FTimerHandle WaitInventorySyncTimeHandler;
};

FORCEINLINE bool AB2LobbyGameMode::IsHeroMgmtMode() const { return bHeroMgmtMode; }

FORCEINLINE int32 AB2LobbyGameMode::GetSelectedChapterViewNum() const { return SelectedChapterViewNum; }

FORCEINLINE ELobbyScene AB2LobbyGameMode::GetCurrentLobbyScene() const { return LobbySceneManager.GetCurrentLobbyScene(); }