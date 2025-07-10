#pragma once
//#include "BladeII.h"
#include "EngineMinimal.h"
#include "CommonStruct.h"
#include "BladeIIPlayer.h"
#include "EventSubsystem.h"
#include "B2UIEnum.h"
#include "B2ModPlayerInfoData.h"
#include "B2LobbySceneManager.generated.h"


UENUM(BlueprintType)
enum class ELobbyScene : uint8
{
	ELobbyScene_Title,
	ELobbyScene_SelectLogin,
	ELobbyScene_CharacterIntro,
	ELobbyScene_LobbyMain,
	ELobbyScene_CharacterObserve,
	ELobbyScene_HeroManagement,
	ELobbyScene_Store,
	ELobbyScene_Mail,
	ELobbyScene_Chapter,
	ELobbyScene_StageInfo,
	ElobbyScene_ModSelect,
	ElobbyScene_PVPMain,
	ElobbyScene_TeamMatchMain,
	ElobbyScene_CounterAttackSelect,
	ELobbyScene_Friend,
	ELobbyScene_ContentsSubMain,
	ELobbyScene_AdventureModSelect,
	ELobbyScene_HeroTowerReady,
	ELobbyScene_Guild,
	ELobbyScene_GuildCreateAndJoin,
	ELobbyScene_RaidMain,
	ELobbyScene_CollectBookMain,
	ELobbyScene_SummonItem,
	ELobbyScene_SummonItemDisplay,
	ELobbyScene_GuildMapUI,
	ELobbyScene_GuildBattleReady,
	ELobbyScene_ControlMain,
	ELobbyScene_GeneralLotteryDisplay,
	ELobbyScene_CharacterPage,
	ELobbyScene_ControlCharacterPage,
	ELobbyScene_FindAccount,
	ELobbyScene_EnterDungeonMain,
	ELobbyScene_EnterShopMain,
	ELobbyScene_HeroTowerCharacterSelectPage,
	ELobbyScene_EnterBattleMain,
	ELobbyScene_PVPCharacterSelectPage,
	ELobbyScene_EtherMain,
	ELobbyScene_RestPoint,
	ELobbyScene_EventMain,
	ELobbyScene_SeasonEvent,
	ELobbyScene_PackageStore,
	ELobbyScene_HallOfFame,
	ELobbyScene_MagicStore,
	ELobbyScene_Mercenary,
	ELobbyScene_AdventureEnterBattleMain,
	ELobbyScene_FairyMain,
	ELobbyScene_Mission,
	ELobbyScene_DonationEvent,
	ELobbyScene_DimensionMain,
	ELobbyScene_DimensionSelectPage,
	ELobbyScene_ItemForge,
	ELobbyScene_SealBoxResult,
	ELobbyScene_None
};

UENUM(BlueprintType)
enum class ELobbyLevelType : uint8
{
	ELobbyLevelType_A, //CharacterIntro/HeroManagement/StageInfo
	ELobbyLevelType_B, //LobbyMain/CharacterObserve
	
	// Chapter. A single streaming level is suppose to take one chapter now..
	ELobbyLevelType_C_Act01,
	ELobbyLevelType_C_Act02,
	ELobbyLevelType_C_Act03,
	ELobbyLevelType_C_Act04,
	ELobbyLevelType_C_Act05,
	ELobbyLevelType_C_Act06,
	ELobbyLevelType_C_Act07,

	// ELobbyLevelType_C_Act* .. to be added for additional chapters..

	// Added ELobbyLevelType_C_Act* must be in successive number!!

	ELobbyLevelType_C_MOD, // Let's say this is also a kind of C type.
	ELobbyLevelType_D,		// Hero Tower Floor Select.
	ELobbyLevelType_E,		// Guild Map.
	//ELobbyLevelType_F,		// 더이상 사용 안함 (누군가 다음에 추가할 일이 있으면 이것을 사용하시길...)
	ELobbyLevelType_G,
	ELobbyLevelType_H,
	ELobbyLevelType_SummonItem,
	ELobbyLevelType_SummonItemDisplayScene,
	ELobbyLevelType_RestPoint,
	ELobbyLevelType_HallOfFame,
	ELobbyLevelType_Fairy,
	ELobbyLevelType_Empty,
	ELobbyLevelType_None
};

// Its manifestation is supposed to be AB2LobbyNPCSkeletalMeshActor
UENUM(BlueprintType)
enum class ELobbyNPCType : uint8
{
	ELobbyNPC_ShopGirl,
	ELobbyNPC_RelaxingDoumi,
	ELobbyNPC_Fairy_Niece,
	ELobbyNPC_Fairy_Pang,
	ELobbyNPC_Fairy_Rio,
	ELobbyNPC_FlyingFairy_Niece,
	ELobbyNPC_FlyingFairy_Pang,
	ELobbyNPC_FlyingFairy_Rio,
	ELobbyNPC_Test,	//봉인함 카드 임시 항목

	ELobbyNPC_None // End
};
/** Being used for sending information on which anim to play to lobby shop girl. */
UENUM(BlueprintType)
enum class ELobbyShopGirlState : uint8
{
	ELSG_IdleLoop,
	ELSG_Welcoming,
	ELSG_CheerNormal,
	ELSG_CheerMuch,
	ELSG_TouchReaction,

	ELSG_End
};

UENUM(BlueprintType)
enum class ELobbyRelaxingDoumiState : uint8
{
	ELRD_IdleLoop,
	ELRD_Welcoming,
	ELRD_RewardNormal,
	ELRD_RewardDouble,
	ELRD_TouchReaction,

	ELRD_End
};

UENUM(BlueprintType)
enum class ELobbyFairyDoumiState : uint8
{
	ELFD_IdleLoop,
	ELFD_Welcoming,
	ELFD_Bless,
	ELFD_TouchReaction01,
	ELFD_TouchReaction02,

	ELFD_End
};

FORCEINLINE ELobbyLevelType GetChapterSceneLevelTypeOfChapter(int32 InChapterNum)
{
	check(InChapterNum > 0);
	// ELobbyLevelType of each chapter (act) must be successive.
	return (ELobbyLevelType)((int32)ELobbyLevelType::ELobbyLevelType_C_Act01 + InChapterNum - 1);
}

class FLobbyLevelVisibleManager
{
private:
	FLobbyLevelVisibleManager(class FLobbySceneManager*);
	void AddLevelLobbyScene(ELobbyLevelType LobbyType, FName LevelName);
	void ChangeVisibility(ELobbyScene ToChangeScene, int32 OptionalPrevChapterNum = -1, int32 OptionalChapterNum = -1); // Optional**ChapterNum are required only for ELobbyScene_Chapter
	bool IsStreamingLevelLoaded(ELobbyScene CheckScene, int32 OptionalChapterNum = -1);
	void PrefetchStreamingLevel(ELobbyScene PrefetchScene);
	ELobbyLevelType GetLobbyLevelType(ELobbyScene LobbyScene, int32 OptionalChapterNum = -1); // OptionalChapterNum is required only for ELobbyScene_Chapter
	bool HasSameStreamingLevelToCurrentScene(ELobbyScene TestScene, int32 OptionalCurrChapterNum = -1, int32 OptionalTestChapterNum = -1); // Optional**ChapterNum are required only for ELobbyScene_Chapter

private:
	TMultiMap<ELobbyLevelType, FName> LevelsRequiredScene;
	class FLobbySceneManager* LobbySceneManager;	
	ELobbyScene CurrentLobbyScene;

	friend class FLobbySceneManager;
};

USTRUCT(BlueprintType)
struct FHallOfFameCharacterInfo
{
	GENERATED_USTRUCT_BODY()

	FHallOfFameCharacterInfo()
	{
		HallOfFameCharacter = nullptr;
		CharacterClass = EPCClass::EPC_End;
		Ranking = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HallOfFameCharacter)
	class ASkeletalMeshActor* HallOfFameCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HallOfFameCharacter)
	EPCClass CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HallOfFameCharacter)
	int32 Ranking;
};

USTRUCT(BlueprintType)
struct FLobbyCharacterInfo
{
	GENERATED_USTRUCT_BODY()

	FLobbyCharacterInfo()
	{
		//ActorForMain = NULL;
		//ActorForSub = NULL;
		//MatineeForMain = NULL;
		//MatineeForInven = NULL;
		//MatineeForWingEventScene = NULL;
		//IdleAnimInstance = NULL;
	}

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	//class AB2HeroMgntSkeletalMeshActor* ActorForMain;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	//class AB2HeroMgntSkeletalMeshActor* ActorForSub;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	//class AMatineeActor* MatineeForMain;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	//class AMatineeActor* MatineeForInven;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	//class AMatineeActor* MatineeForWingEventScene; // For any kind of special directed event scene
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	//class AMatineeActor* MatineeForCollectBook;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	//class UAnimationAsset* IdleAnimInstance;

	FVector CachedInitLocation;
	FRotator InitialRotation;
};

USTRUCT(BlueprintType)
struct FLobbyCharacter
{
	GENERATED_USTRUCT_BODY()

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	//class AB2LobbySkeletalMeshActor* LevelActor;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	//class AMatineeActor* GameBeginingMatinee;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	//class AMatineeActor* SelectingMatinee;
};

/** Added later, other FLobbyCharacter is implicitly for PlayerCharacter. */
USTRUCT(BlueprintType)
struct FLobbyNPCCharacterInfo
{
	GENERATED_USTRUCT_BODY()

	FLobbyNPCCharacterInfo()
	{
		//LevelActor = nullptr;
	}

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LobbyCharacter)
	//class AB2LobbyNPCSkeletalMeshActor* LevelActor;

};

USTRUCT(BlueprintType)
struct FLobbyMainScenePointLightInfo
{
	GENERATED_USTRUCT_BODY()

	FLobbyMainScenePointLightInfo()
	{
		PL_ObserveView_1 = nullptr;
		PL_ObserveView_2 = nullptr;
		PL_AllCharView_1 = nullptr;
		PL_AllCharView_2 = nullptr;
	}

	// Declare them in array format might not be that necessary.. or rather worse. The maximum number of movable point lights is limited.
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LobbyScene")
	class APointLight* PL_ObserveView_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyScene")
	class APointLight* PL_ObserveView_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyScene")
	class APointLight* PL_AllCharView_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyScene")
	class APointLight* PL_AllCharView_2;
};

class FLobbySceneBase
{
public:
	FLobbySceneBase(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbySceneBase();
	virtual void OpenScene();
	virtual void CloseScene();
	virtual void Tick(float DeltaSeconds){}
	virtual void ForceUpdate(){}
	virtual EUIScene GetUIScene() = 0;
	virtual ELobbyScene GetLobbyScene() = 0;

	/** Called when LobbyGM got NotifyActorCustomEvent. One of its usage is Matinee stop notification */
	virtual void OnLobbyGMActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject) {} 

	class APlayerController* GetLocalPlayerController();
	class AB2LobbyGameMode* GetLobbyGameMode();
	class UGameViewportClient* GetViewportClient();
	FORCEINLINE class FLobbySceneManager* GetSceneManager() { return SceneManager; }

protected:
	TMap<EPCClass, FLobbyCharacterInfo>&	GetLobbyCharactersForATypeLevel();
	TMap<EPCClass, FLobbyCharacter>&		GetLobbyCharactersForBTypeLevel();
	TMap<EPCClass, FLobbyCharacterInfo>&	GetLobbyCharactersForGTypeLevel();
	TArray<FHallOfFameCharacterInfo>&		GetLobbyCharactersForHallOfFame();
	//TMap<EPCClass, class UAnimationAsset*>& GetHallOfAnimation(bool IsBestCharacter = false);
	//class AMatineeActor* GetDefaultCameraMatinee();
	//class AMatineeActor* GetChapterMatinee();

	/** Use the designated KeyTiem of the first found camera track group of MatineeActor as current camera */
	//void SetCameraMatineeCam(class AMatineeActor* MatineeActor, float KeyTime = 0.f);

public:
	virtual void SubscribeEvents_OnConstruct();
	virtual void UnsubscribeEvents_OnDestruct();
protected:
	virtual void SubscribeEvents_OnOpen();
	virtual void UnsubscribeEvents_OnClose();

	// LobbyMatching
public:
	virtual void OnHostChanged(int32 RoomType, int32 NewHostId) {}
	virtual void NotifySuccessMatchMake(bool IsHost) {}
	virtual void HandleRequestMatchMaking() {}
	virtual void HandleNotifyLeave(uint32) {}
	virtual void SetMyPlayerInfoData(uint32, const FB2FindAccountInfoPtr&) {}
	virtual void SetRemotePlayerInfoData(uint32, const FB2FindAccountInfoPtr&) {}
	virtual void SetRemotePlayerClass(uint32, EPCClass, const FB2FindAccountInfoPtr&) {};
	virtual void LeaveRoomIfJoined() {}
	virtual void HandleNotifyKickUser() {}
	virtual void HandleNotifyRoomLeave() {}
	virtual void HandleWaitToMeetCondition() {}
	virtual void HandlePreparePlayers() {}
	virtual void HandleFinishLobbyMatch() {}
	
	virtual int32 GetMaxMatchPlayerNum() { return 1; }
	virtual void ResponseChangeRoomAccess(uint8 AccessKey) {}
	virtual int32 GetCurrentModeMapId() { return 0; }
	virtual int32 GetCurrentModeDifficulty() { return 1; }
	virtual ERoomCloseReasonType GetStartRoomCloseReason() { return ERoomCloseReasonType::NONE; }
	virtual ERoomCloseReasonType GetBreakRoomCloseReason() { return ERoomCloseReasonType::NONE; }
	virtual void ReceiveMatchPlayerTeamInfo() {}
	virtual void ReceiveNoResponsePrerequisiteToStart() {}
public:
	virtual void BattleStart() {};

private:
	class FLobbySceneManager* SceneManager;
};

USTRUCT()
struct FCameraZoomModule
{
	GENERATED_BODY()

	FCameraZoomModule();
	~FCameraZoomModule() {}
		

	float	CamNearDistanceLimit;
	float	CamFarDistanceLimit;
	FVector	OrgCameraLocation;

	float	TopOffsetZ;

	float	ZoomFixedOffsetZ;

	FLobbySceneBase*				Scene;
	ULocalPlayer*					LocalPlayer;

	//AB2HeroMgntSkeletalMeshActor*	ZoomTarget;
	ACameraActor*					CurrentCamera;

	void InitializeModule(FLobbySceneBase* OwnerScene);
	void ClearZoomModule();
	
	//void ChangeZoomTarget(AB2HeroMgntSkeletalMeshActor* Target);

	float GetPlaneIntersectHeight(const FVector2D& ScreenPos, const FVector& Orgin);
	FVector GetPinpointLocation(const FVector2D& ScreenFocus);

	void Zoom(const FVector2D& ScreeenFocus, float Delta);
	void Zoom(const FVector& PinpointLocation, float Delta);

	FORCEINLINE const float GetMinCamDistance() const { return CamNearDistanceLimit; }
	FORCEINLINE const float GetMaxCamDistance() const { return CamFarDistanceLimit; }

};

class FLobbySceneManager : public FTickableGameObject
{
public:
	FLobbySceneManager();
	virtual ~FLobbySceneManager();

	virtual void Tick(float DeltaSeconds);
	virtual bool IsTickable() const { return GameMode != nullptr; }

	virtual bool IsTickableWhenPaused() const { return true; }
	virtual bool IsTickableInEditor() const { return true; }

	virtual TStatId GetStatId() const;

public:
	class AB2LobbyGameMode* GetLobbyGameMode();
	void SetGameMode(class AB2LobbyGameMode*);
	/** bForceChange will set the Lobby scene again even if it tries to change to the same scene.
	 * OptionalToChangeChapterNum is required only for ELobbyScene_Chapter */
	void ChangeLobbyScene(ELobbyScene ToChangeScene, int32 OptionalToChangeChapterNum = -1, bool bForceChange = false);
	bool IsSceneStreamingLevelLoaded(ELobbyScene CheckScene, int32 OptionalChapterNum = -1);
	void PrefetchSceneStreamingLevel(ELobbyScene PrefetchScene);
	ELobbyScene GetLobbyScene(EUIScene UIScene);
	
	/* To use when map-transitioning. ex) Game -> Lobby. Never capture UObject-ptr/refs when using lambda*/
	static void DeferredRegistChangeLobbyScene(std::function<void(void)> Func);
	static bool IsExistDeferredRegistChangeLobbyScene();

	static bool HasDeferredChangeLobbyScenes();
	void CallDeferredChangeLobbyScenes();
	void ClearDeferredChangeLobbyScenes();

	void UpdateLobbyOnHeadDisplayWidget(EPCClass OptionalCreateOnlyClass = EPCClass::EPC_End);
	void ConditionalUpdateLobbyOnHeadDisplayWidget();
	void AddHallOfFrameCharacter(const TArray<FHallOfFameCharacterInfo>& HallOfFameCharacter);
	void AddHallOfCharacterAnimation(EPCClass PCClass, class UAnimationAsset* PCAnimation);
	void AddLevelLobbyScene(ELobbyLevelType LobbyType, FName LevelName);
	void AddLobbyCharacter(EPCClass PCClass, const FLobbyCharacter& LobbyCharacter);
	void AddLobbyATypeActorInfo(EPCClass PCClass, const FLobbyCharacterInfo& LobbyCharacter);
	void AddLobbyGTypeActorInfo(EPCClass PCClass, const FLobbyCharacterInfo& LobbyCharacter);	// only Collectbook
	void AddLobbyNPCCharacter(ELobbyNPCType InType, const FLobbyNPCCharacterInfo& InAddInfo);
	void AddLobbyFairyFlyingActor(EFairyType InType, class AB2LobbyFlyingFairyDoumi* fairyactor);
	void AddLobbyMainScenePointLights(const FLobbyMainScenePointLightInfo& InAddInfo);
	void AddSummonItemDisplayMatinee(bool bIsMultiple,  bool bIsOpen,bool bIsFriendShip, class AMatineeActor* Matinee);
	//void AddSummonItemDisplayMatineeResult(class AMatineeActor* Matinee);
	void HeroTransientEffectBegin(const int32 Index);
	/** Dynamically create a SkeletalMesh to represent equipped state of InPCClass and set it to designated SkeletalMeshActor. 
	 * @param bForceUpdate : If true, it will re-generate necessary stuff even cached object is available. */
	void SetupLobbyCharCompForParts(EPCClass InPCClass, class ASkeletalMeshActor* InSKActorToSet, bool bForceUpdate = false);
	/** Calls SetupLobbyCharCompForParts for all SkeletalMeshActors representing InPCClass. */
	void UpdateLobbyCharCompForParts(EPCClass InPCClass);
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	void DevForceUpdateLobbyCharCompForParts(EPCClass InPCClass);
//#endif
	bool SelectLobbyMainCharacter(EPCClass SelectedClass);

	EPCClass GetHeroMgmtModePCClass();
	void SetHeroMgmtModePCClass(EPCClass pcClass);

	FORCEINLINE ELobbyScene GetCurrentLobbyScene() const { return CurrentLobbyScene; }
	FORCEINLINE bool IsPlayingBeginAnim() const { return PlayBeginAnim; }
	
	void SetPlayingBeginAnim(bool InPlay) { PlayBeginAnim = InPlay; }
	void Zoom(float CamDistDelta, const FVector2D& Focus);
	void SwipeV(float Value);
	void SwipeH(float Value);
	void FlickV(bool bUpDirection);
	void FlickH(bool bLeftDirection);

	void SelectCharacterHeroMgmt(EPCClass);
	void SelectCharacterStageInfo(EPCClass, EPCClass);
	void SetGuildMapChangeMatinee(class AMatineeActor* Atype, class AMatineeActor* Btype);
	void GuildMapChangeMatineeEnd();	

	void ForceUpdate();
	void SetDefaultCameraMatinee(class AMatineeActor*);
	void SetChapterViewMatinee(class AMatineeActor*);

	//class AMatineeActor* GetDefaultCameraMatinee();
	//class AMatineeActor* GetChapterMatinee();

	void SetItemOpDirectingSceneObjects(class ACameraActor* InCamActor);
	void EnterItemOpDirectingView(ELobbyInvenItemOpMode InOpMode);
	void QuitItemOpDirectingView();
	bool IsInItemOpDirectingView();
	void OnLobbyGMActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject); // Called when LobbyGM got NotifyActorCustomEvent

	FORCEINLINE TMap<EPCClass, FLobbyCharacterInfo>&	GetLobbyCharactersForATypeLevel() { return LobbyCharactersForALobbyType; }
	FORCEINLINE TMap<EPCClass, FLobbyCharacter>&		GetLobbyCharactersForBTypeLevel() { return LobbyCharactersForBLobbyType; }
	FORCEINLINE TMap<EPCClass, FLobbyCharacterInfo>&	GetLobbyCharactersForGTypeLevel() { return LobbyCharactersForGLobbyType; }
	FORCEINLINE TMap<EPCClass, class UAnimationAsset*>& GetHallOfAnimation(bool IsBestCharacter = false) { return IsBestCharacter ? HallOfFameBestCharacterAnimation : HallOfFameCharacterAnimation; }
	FORCEINLINE TArray<FHallOfFameCharacterInfo>&	GetLobbyCharactersForHallOfFame() { return LobbyCharactersForHallOfFame; }

	//class AB2LobbyNPCSkeletalMeshActor* GetNPCSKActor(ELobbyNPCType InType);

	void GetPointLightsOfObserveView(TArray<class APointLight*>& OutPointLights);
	void GetPointLightsOfAllCharView(TArray<class APointLight*>& OutPointLights);
	void SetPointLightsOfLobbyMain(bool bForObserveScene /* True for Observe view, false for AllCharacter view */);

	void PlaySummonItemDisplayMatinee(bool bIsMultiple, bool bIsOpen, bool bIsFriendShip);
	void PlaySummonItemDisplayMatineeResult();
	void OnFinishedSummonItemDisplayResultMatinee();
	//void OnFinishedSummonItemDisplayMatinee(class AMatineeActor* Matinee);
	//void OnChangedToFxSummonItemDisplayMatinee(class AMatineeActor* Matinee);
	//void OnFinishedHallOfFameDisplayMatinee(class AMatineeActor* Matinee);

	// Hall Of Fame
	bool IsHallOfFameData(FB2ResponseGetHallOfFamePtr InHallOfFameInfo);
	void SetHallOfFameInfo(FB2ResponseGetHallOfFamePtr InHallOfFameInfo);
	void SetPraiseInfo(FB2ResponsePraiseTargetRankerPtr InPraiseInfo);
	void ExecutePendingHallOfFameScene();

	// Lobby Match
	void OnHostChanged(int32 RoomType, int32 NewHostId);
	void NotifySuccessMatchMake(bool IsHost);
	void HandleRequestMatchMaking();
	void HandleNotifyLeave(uint32);
	void SetMyPlayerInfoData(uint32, const FB2FindAccountInfoPtr&);
	void SetRemotePlayerInfoData(uint32, const FB2FindAccountInfoPtr&);
	void SetRemotePlayerClass(uint32, EPCClass, const FB2FindAccountInfoPtr&);
	void LeaveRoomIfJoined();
	void HandleNotifyKickUser();
	void HandleNotifyRoomLeave();
	void HandleWaitToMeetCondition();
	void HandlePreparePlayers();
	void HandleFinishLobbyMatch();
	void ReceiveNoResponsePrerequisiteToStart();

	int32 GetMaxMatchPlayerNum();
	void ResponseChangeRoomAccess(uint8 AccessKey);

	int32 GetMatchModeMapId();
	int32 GetMatchModeDifficulty();

	ERoomCloseReasonType GetStartRoomCloseReason();
	ERoomCloseReasonType GetBreakRoomCloseReason();

	void ReceiveMatchPlayerTeamInfo();

	/** Set if character wing mesh must be visible unconditionally, 
	 * with optional mesh composition update (bUpdateCharMeshImmediate), 
	 * and if so, designating the character class (MeshUpdateClass, and EPCClass::EPC_End for all) */
	void SetUnconditionalShowWing(bool bOn, bool bUpdateCharMeshImmediate = true, EPCClass MeshUpdateClass = EPCClass::EPC_End);
	
	void SetShopGirlReservedState(ELobbyShopGirlState InStateToReserve);
	FORCEINLINE void ResetShopGirlReservedState() { SetShopGirlReservedState(ELobbyShopGirlState::ELSG_IdleLoop); }
	FORCEINLINE ELobbyShopGirlState GetShopGirlReservedState() const { return ShopGirlReservedState; }

	FORCEINLINE void SetCanUseRemoteInfoForSKMeshSetup(bool InUse) { bCanUseRemoteInfoForSKMeshSetup = InUse; }

	float GetCharacterCameraRotateFactor() const { return CharacterCameraRotateFactor; }
	float GetCharacterCameraFreeRotateMaxSpeed() const { return CharacterCameraFreeRotateMaxSpeed; } // Speed must be absolute (positive) value
	float GetCharacterCameraRotateDeaccelerationFactor() const { return CharacterCameraRotateDeaccelerationFactor; }
	float GetCharacterFreeRotateAdditionalScale() const { return CharacterFreeRotateAdditionalScale; } // Only for character rotate, not for camera.
	float GetWorldMapSwipeDeaccelerationFactor() const { return WorldMapSwipeDeaccelerationFactor; }
	float GetWorldMapSpeedTriggeringAcceleratingSwipe() const { return WorldMapSpeedTriggeringAcceleratingSwipe; }
	int32 GetWorldMapDirectSwipeSpeedHistoryNum() const { return WorldMapDirectSwipeSpeedHistoryNum; }
	int32 GetWorldMapDirectSwipeSpeedFilterNum() const { return WorldMapDirectSwipeSpeedFilterNum; }

	void BattleStart();

	void OnLobbyFlyFairy_Visible(EFairyType InType);
	void OnLobbyFlyFairy_AttachFX(bool bValue);
	void OnLobbyFlyFairy_AllHidden();

private:
	void HiddenCharacter(const FLobbyCharacter& ToHiddenLobbyCharacter, bool bHidden);		
	void CreateLobbyScenes();
	void DeleteLobbyScenes();
	void OnChangeLobbySceneSetSound(ELobbyScene InPrevScene, ELobbyScene InChangedScene);
	
	class FLobbySceneBase* GetLobbyScene(ELobbyScene InLobbyScene);
	class FLobbyHallOfFame* GetHallOfFameScene();
private:
	class AB2LobbyGameMode* GameMode;
	FLobbyLevelVisibleManager LevelManager;
	ELobbyScene CurrentLobbyScene;
	int32 CurrentChapterViewNum; // Only for ELobbyScene_Chapter
	TMap<EPCClass, FLobbyCharacterInfo> LobbyCharactersForALobbyType;
	TMap<EPCClass, FLobbyCharacter>		LobbyCharactersForBLobbyType;
	TMap<EPCClass, FLobbyCharacterInfo> LobbyCharactersForGLobbyType;
	TMap<EPCClass, FVector>				GLobbyTypeActorLocation;
	TArray<FHallOfFameCharacterInfo>	LobbyCharactersForHallOfFame;
	TMap<EPCClass, class UAnimationAsset*> HallOfFameCharacterAnimation;
	TMap<EPCClass, class UAnimationAsset*> HallOfFameBestCharacterAnimation;
	TMap<ELobbyNPCType, FLobbyNPCCharacterInfo> LobbyNPCs;
	TMap<EFairyType, class AB2LobbyFlyingFairyDoumi*> LobbyFlyFairys;
	FLobbyMainScenePointLightInfo LobbyMainScenePointLights;
	void ResetObjectRefsOnSceneLevelStreaming();

	struct SummonItemDisplayMapKey
	{
		SummonItemDisplayMapKey(bool bInIsMultiple,bool bInIsOpen, bool bInIsFriendShip) : bIsMultiple(bInIsMultiple), bIsOpen(bInIsOpen), bIsFriendShip(bInIsFriendShip) {}
		bool bIsMultiple;
		bool bIsOpen;
		bool bIsFriendShip;

		bool operator==(const SummonItemDisplayMapKey& Other) const
		{
			return bIsMultiple == Other.bIsMultiple && bIsOpen == Other.bIsOpen && bIsFriendShip == Other.bIsFriendShip;
		}

		friend int32 GetTypeHash(const SummonItemDisplayMapKey& Key)
		{
			return  Key.bIsMultiple + (Key.bIsOpen << 1) + (Key.bIsFriendShip << 2);
		}
	};
	TMap<SummonItemDisplayMapKey, TWeakObjectPtr<AMatineeActor>>	SummonItemMatinees;
	TWeakObjectPtr<AMatineeActor>									SummonItemMatineesResult;
	class AMatineeActor* SummonItemMultipleResultMatinee;

	bool PlayBeginAnim;
	class AMatineeActor* DefaultCameraMatinee;
	class AMatineeActor* ChapterMatinee;

	/** If true, character wing is visible regardless of user wing visibility selection (FB2Wing.bShouldBeVisible)
	 * For wing evolution scene fow now. Collect book scene could use this but that seems a little different.. */
	uint32 bUnconditionalShowWing : 1;

	/** True for FindAcccount scene (i.e. other user info view) */
	uint32 bCanUseRemoteInfoForSKMeshSetup : 1;

	/** To check if shop girl is about to do crazy sexy dance. */
	ELobbyShopGirlState ShopGirlReservedState;

	UPROPERTY()
	class ACameraActor* ItemOpSceneCamera;

	EPCClass CurrentSubPCClass;
	TArray<FLobbySceneBase*> LobbyScenes;

	static TArray<std::function<void(void)>> DeferredChangeLobbyScenesFunc;
	static TArray<std::function<void(void)>> PendingEventSubSystems;

	float CharacterCameraRotateFactor;
	float CharacterCameraFreeRotateMaxSpeed;
	float CharacterCameraRotateDeaccelerationFactor;
	float CharacterFreeRotateAdditionalScale; // To scale character rotate speed, relative to the camera rotating case.

	float WorldMapSwipeDeaccelerationFactor;
	float WorldMapSpeedTriggeringAcceleratingSwipe;
	int32 WorldMapDirectSwipeSpeedHistoryNum; // How many history to keep.. and the total average will be the free swipe initial speed.
	int32 WorldMapDirectSwipeSpeedFilterNum; // This number of sample from DirectSwipeSpeedHistory will be used to get direct swipe speed.
	
	float LobbyCameraZoomSpeed;
	float LobbyCameraRotationX;
	float LobbyCameraRotationY;
};

class FLobbyEmptyScene : public FLobbySceneBase
{
public:
	FLobbyEmptyScene(class FLobbySceneManager* OwnerSceneManager, EUIScene UIScene, ELobbyScene LobbyScene)
		:FLobbySceneBase(OwnerSceneManager), RelatedUIScene(UIScene), RelatedLobbyScene(LobbyScene)
	{

	}

	virtual void OpenScene() final {}
	virtual void CloseScene() final {}
	virtual EUIScene GetUIScene() final { return RelatedUIScene; }
	virtual ELobbyScene GetLobbyScene() final { return RelatedLobbyScene; }

private:
	EUIScene RelatedUIScene;
	ELobbyScene RelatedLobbyScene;
};


class FLobbyCharacterIntroScene : public FLobbySceneBase
{
public:
	FLobbyCharacterIntroScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyCharacterIntroScene();
	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::CharacterIntro; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_CharacterIntro; }
private:
	void SelectCharacterIntro(EPCClass);	

private:
	EPCClass CurrentPCClass;
	uint32 LobbyCharIntroSelectTicket;
};

class FLobbyTitleScene : public FLobbySceneBase
{
public:
	FLobbyTitleScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyTitleScene();

	virtual EUIScene GetUIScene() final { return EUIScene::Title; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_Title; }
};

class FSelectLoginScene : public FLobbySceneBase
{
public:
	FSelectLoginScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FSelectLoginScene();

	virtual EUIScene GetUIScene() final { return EUIScene::SelectLogin; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_SelectLogin; }
};

class FLobbyMainScene : public FLobbySceneBase
{
public:
	FLobbyMainScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyMainScene();
	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::LobbyMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_LobbyMain; }

	virtual void Tick(float DeltaSeconds) override;

	virtual void SwipeScene(float Delta);

private:
	void SetHiddenLobbyMainUI(bool bHidden);
	float GetDefaultLobbyMatineeTimeStamp();

	float AutoHideInterval;
	float ElapsedOpenScene;
	float ElapsedTimeNoInput; // 일정시간 후에 마티니 재생속도를 1.0f 로 만들기 위한 변수
	bool bVisibleUI;

	float MatineePos;
	const float PlayRateRestoreTime;

	uint32 LobbyMainCameraMoveTicket;
};
	
class FLobbyCharObserveScene : public FLobbySceneBase
{
public:
	FLobbyCharObserveScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyCharObserveScene();
	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	void ConditionalForceRecacheResource();

	virtual EUIScene GetUIScene() final { return EUIScene::LobbyMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_CharacterObserve; }

	virtual void Tick(float DeltaSeconds) override;

	void SetPCClass(EPCClass);
	void ForcedViewCharacterChange(EPCClass SelectedClass);
	void PlayStartCharMatinee(float InStartMatineeDelay);

	void VerticalScreen(bool bVertical);

private:
	void PlaySelectMatinee(bool IsBegin = false, bool bPromptToClosedUpView = false);
	void PlayPoseAnim();

	EFairyType FindFlyFairyTypeByOpenStage();

private:
	struct FLobbyCharacterObserveCamera
	{
		FLobbyCharacterObserveCamera(FLobbyCharObserveScene* OwnerScene) : Scene(OwnerScene)
		{
			Initialize();
		}

	public:
		void ActivateObserveCamera(bool bInActivate);
		void BlockMoveCamera(bool bBlock);

		void Initialize();		
		void Tick(float DeltaSeconds);

		void SetData(EPCClass SelectedClass, bool IsBegin, bool bPromptToClosedUpView = false);
		void PlayCharacterMatinee(AMatineeActor* PlayMatinee, class AB2LobbySkeletalMeshActor* LevelActor);
		void MoveCamera(const FRotator& RotDelta, float CamDistDelta, bool bForceUpdate = false);
		void RotateCharacterYaw(bool bLeft);
		void UpdateViewLoc(const FVector2D& ScreenPos);

		void SetScreenState(bool bVerticalScreen);
		float GetCurrentCamDistance();

	private:
		float GetViewLocZ(const FVector2D& ScreenPos);

	public:
		class AB2LobbySkeletalMeshActor* CachedViewTargetActor;
		class AMatineeActor* CachedEntryMatinee;

		bool bActivate;

		FVector CachedViewTargetLocation;
		float MatineeEndCamDist;
		float DefaultCamDist;
		float CachedCamDist;
		
		float ElapsedSelectMatineeTime;
		float MaxSelectMatineeCameraTime;
		bool bMatineeCamFinished;

		float NearFOV;
		float FarFOV;
		float DefaultFOV;

		float FarLimitDistance;
		float NearLimitDistance;

		float FarUnderCamLimitRotPitch;
		float NearUnderCamLimitRotPitch;

		float AddtionalOffsetZ;

		FVector2D DestViewTargetScreenPos;
		FLobbyCharObserveScene* Scene;

		TArray<AActor*> HiddenActorsBetweenCameraAndActor;		

		bool bVerticalScreen;
		bool bBlockMoveCamera;
	public:
		TArray<AActor*> IgnoreHiddenActorsBetweenCameraAndActor;
	};

	

	bool IsPlayStartMatinee;
	float StartMatineeDelay;
	float CurrentStartMatineeDelay;

	FLobbyCharacterObserveCamera LobbyObserveCamera;

	EPCClass CurrentPCClass;

	uint32 LobbyCharObvSelectTicket;
	uint32 LobbyCharMoveCameraTicket;
	uint32 LobbyCharRotateCharacterTicket;
	uint32 LobbyCharCameraViewTargetTicket;
	uint32 LobbyCharObserveChangeScreenTicket;
	uint32 LobbyBlockMoveCameraTicket;
	uint32 LobbyCharPlayMatineeTicket;
	uint32 LobbyCharObvForcedChangeTicket;

public:
	FORCEINLINE FLobbyCharacterObserveCamera& GetObserveCamera() { return LobbyObserveCamera; }

	friend struct FLobbyCharacterObserveCamera;

private:
	class FLobbySceneManager* CahsedSceneManager;
};

class FLobbyHeroMgmtScene : public FLobbySceneBase
{
public:
	FLobbyHeroMgmtScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyHeroMgmtScene();
	virtual void OpenScene() override;
	virtual void CloseScene() override;
	
	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	/** Called when LobbyGM got NotifyActorCustomEvent. One of its usage is Matinee stop notification */
	virtual void OnLobbyGMActorCustomEvent(FName OptionalEventName, UObject* OptionalNotifyingObject) override;

	virtual EUIScene GetUIScene() final;
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_HeroManagement; }

	FORCEINLINE EPCClass GetCurrentPCClass() { return CurrentPCClass; };
	FORCEINLINE void SetCurrentPCClass(EPCClass pcClass) { CurrentPCClass = pcClass; };
private:

	void SelectCharacterHeroMgmt(EPCClass SelectedCharacter);
	void RotateCharacterYaw(float Value);
	void PlayEquippedAnimation(EPCClass CharClass, EItemEquipPlace EquippedPlace);
	void OnChangedLobbyFocusClass(class UB2UIDocBase* Sender, EPCClass CurrentFocusClass, EPCClass PrevFocusClass);

	EPCClass CurrentPCClass;
	uint32 DeliveryEnhanceRelic;

	uint32 LobbyHeroMgmtSelectTicket;
	uint32 LobbyCharRotateCharacterTicket;
	uint32 LobbyCharEquippedItemTicket;
	uint32 BeginWingEvolutionSceneTicket;
	uint32 EndWingEvolutionSceneTicket;
	uint32 LobbyCharZoomTicket;

	FVector InitialCamLocation;
	float	MinCamDistance;
	float	MaxCamDistance;

	FORCEINLINE float GetMinCamDistance() { return MinCamDistance; }
	FORCEINLINE float GetMaxCamDistance() { return MaxCamDistance; }

	//AB2HeroMgntSkeletalMeshActor* CachedViewTarget;

	FCameraZoomModule ZoomModule;

	//////////////////////////////////////////////////
	// For wing evolution scene, as a part of LobbyHeroMgmtScene

	bool bCurrentlyWingEvolutionScene;
	// Identical to CurrentPCClass in most expected cases, but just for a case that it is being different due to some sort of network delay..
	EPCClass WingEvolutionScenePCClass; 
	// If InEvolvedPCClass is EPC_End, it starts evolution scene using CurrentPCClass
	void BeginWingEvolutionScene(EPCClass InEvolvedPCClass = EPCClass::EPC_End);
	// Wing evolution matinee end does not mean the end of whole wing evolution scene.
	void OnWingEvolutionMatineeEnd();

	void EndWingEvolutionScene();

	//////////////////////////////////////////////////
};

class FLobbyStageInfoScene : public FLobbySceneBase
{
public:
	FLobbyStageInfoScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyStageInfoScene();
	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::BattleStageInfo; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_StageInfo; }
private:
	void SelectCharacterStageInfo(EPCClass MainClass, EPCClass SubClass);	

private:
	EPCClass CurrentMainClass;
	EPCClass CurrentSubClass;

	uint32 LobbyStageInfoSelectTicket;
};

// Common base for FLobbyChapterScene and FLobbyModSelectScene
class FLobbyBattleSelectSceneBase : public FLobbySceneBase 
{
public:
	FLobbyBattleSelectSceneBase(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyBattleSelectSceneBase();
	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual void Tick(float DeltaSeconds) override;
	virtual void ForceUpdate() override;
protected:
	void UpdateForFreeSwipe(float DeltaSeconds);
	virtual void InitializeUIPositionMarkers() {}
	virtual void SwipeScene(float Delta);
	virtual bool CheckActorLocationDirty() { return false; }
	virtual void UpdateUIPosition() {}

	void MoveCameraDirect(float Value, bool bNoCheckForFreeSwipe = false);

	float GetAvgDirectSwipeSpeed(int32 InMaxHistoryNum = -1, bool bRespectTheLastSign = true) const;

	// Get both 2D and 3D positions from InMarkerActors
	void GetUIMarkerPositions(const TArray<TWeakObjectPtr<AActor>>& InMarkerActors, TArray<FVector2D>& OutPos2D, TArray<FVector>& OutPos3D, FMatrix& OutViewProjMat);

	float MatineePos;
	float MatineeLength;
	float MatineeAutoScrollLeftEndPosition;
	float MatineeAutoScrollRightEndPosition;

	float SpeedSwipe; // Finally calculated swipe speed;
	
	// The maximum size of this array is WorldMapDirectSwipeSpeedHistoryNum of SceneManager.
	TArray<float> DirectSwipeSpeedHistory;
	/** Calculated as the average of DirectSwipeSpeedHistory when it start free swipe. */
	float FreeSwipeStartingSpeed;

	/** Both bMoveCameraDirectCalledThisFrame and bAcceleratingSwipe condition must met for free swipe.. */
	bool bMoveCameraDirectCalledThisFrame;
	bool bAcceleratingSwipe; // a.k.a Free swipe
	bool bBeginAcceleratingSwipe; // Transient flag for beginning moment.
		
	float SwipeAcceleration;
	float AcceleratingSwipeInitialSpeed;

	bool bAutoMoveDirection;
	float SpeedAutoScrolling;
	float ElapsedLastInput;
	
	uint32 LobbyChapterCameraMoveTicket;
};

class FLobbyChapterScene : public FLobbyBattleSelectSceneBase
{
public:
	FLobbyChapterScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyChapterScene();
	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::Chapter; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_Chapter; }

	virtual void Tick(float DeltaSeconds) override;

private:
	virtual void InitializeUIPositionMarkers() override;
	virtual void SwipeScene(float Delta) override;

	virtual bool CheckActorLocationDirty() override;
	virtual void UpdateUIPosition() override;

private:
	TArray<TWeakObjectPtr<AActor>> StageUIPositionActors;
	TArray<FVector> CachedUIActorLocation;
	FMatrix CachedViewProjectionMatrix;
};

UENUM(BlueprintType)
enum class ELobbyModSceneElem : uint8 // To use as array index, must be in strict order.
{
	LMODIDX_PVP = 0,
	LMODIDX_TeamMatch = 1,
	LMODIDX_HeroTop = 2,
	LMODIDX_GuildMatch = 3,
	LMODIDX_CounterDungeon = 4,
	LMODIDX_Raid = 5,
	LMODIDX_Control = 6,
	LMODIDX_Dimension = 7,

	LMODIDX_End = 8
};
FORCEINLINE int32 GetIndexOfModSceneElem(ELobbyModSceneElem InModSceneElem) { return static_cast<int32>(InModSceneElem); }

class FLobbyModSelectScene : public FLobbyBattleSelectSceneBase
{
public:
	FLobbyModSelectScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyModSelectScene();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::ModSelect; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ElobbyScene_ModSelect; }
	
	virtual void Tick(float DeltaSeconds) override;

private:
	virtual void InitializeUIPositionMarkers() override;
	virtual void SwipeScene(float Delta) override;

	virtual bool CheckActorLocationDirty() override;
	virtual void UpdateUIPosition() override;
	

	TArray<FName> UIPositionGroupName; // They are in certain order according to ELobbyModSceneElemIndex
	TArray<TWeakObjectPtr<AActor>> UIPositionActors; // Supposed to be in same order to UIPositionGroupName
	TArray<FVector> CachedUIActorLocation; // Supposed to be in same order to UIPositionGroupName
	FMatrix CachedViewProjectionMatrix;
};

class FLobbyTeamMatchScene : public FLobbySceneBase
{
public:
	FLobbyTeamMatchScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyTeamMatchScene();

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::TeamMatchUI; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ElobbyScene_TeamMatchMain; }

	void SetMyAllFormationData(const FB2TeamBattleStatus& TeamBattleStatus);
	void SetMyCurrentFormationEntry(FB2TeamBattleEntryPtr TeamEntry);

private:
	uint32 DeliveryGetTeamBattleStatusTicket;
	uint32 DeliveryUpdateTeamBattleEntryTicket;
	uint32 DeliveryEnhanceTeamFormationTicket;
};

class FLobbyTagMatchScene : public FLobbySceneBase
{
public:
	FLobbyTagMatchScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyTagMatchScene();

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::PVPOneOnOne; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ElobbyScene_PVPMain; }

	void SetUIData(const FB2TagMatchStatus& TagMatchStatus);

private:
	uint32 DeliveryGetMatchStatusTicket;
};

class FLobbyHeroTowerReadyScene : public FLobbySceneBase
{
public:
	FLobbyHeroTowerReadyScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyHeroTowerReadyScene();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::HeroTowerReady; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_HeroTowerReady; }	
	
	void SetUIData(const FB2ResponseGetHeroTowerPtr& MsgPtr);
private:
	int32 DeliveryGetHeroTowerTicket;
};

class FLobbyStoreScene : public FLobbySceneBase
{
public:
	FLobbyStoreScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyStoreScene();

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::Store; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_Store; }

	void SetGeneralShopData(const FB2ResponseGetGeneralShopPtr& ShopData);
private:
	TArray<issue_ptr> Issues;
	bool ReserveStoreScene;
};

class FLobbyMailScene : public FLobbySceneBase
{
public:
	FLobbyMailScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyMailScene();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::Mail; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_Mail; }
	
private:
	void SetUIData(const FB2MailList& MailList);

private:
	uint32 DeliveryGetMailListTicket;	
};

class FLobbyCounterAttackScene : public FLobbySceneBase
{
public:
	FLobbyCounterAttackScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyCounterAttackScene();

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::CounterAttackDGSelect; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ElobbyScene_CounterAttackSelect; }
private:
	void SetGetCounterDungeonInfo(const FB2GetCounterDungeon& DungeonInfo);

private:
	uint32 DeliveryGetCounterDundeonTicket;
};

class FLobbyFriendScene : public FLobbySceneBase
{
public:
	FLobbyFriendScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyFriendScene();

	virtual EUIScene GetUIScene() final { return EUIScene::Friend; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_Friend; }
};

class FContentsSubMainScene : public FLobbySceneBase
{
public:
	FContentsSubMainScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FContentsSubMainScene();

	virtual EUIScene GetUIScene() final { return EUIScene::ContentsSubMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_ContentsSubMain; }
};

class FLobbyAdventureModSelectScene : public FLobbySceneBase
{
public:
	FLobbyAdventureModSelectScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyAdventureModSelectScene();
	
	virtual EUIScene GetUIScene() final { return EUIScene::AdventureModSelect; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_AdventureModSelect; }
};

class FLobbyGuildScene : public FLobbySceneBase
{
public:
	FLobbyGuildScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyGuildScene();

	virtual EUIScene GetUIScene() final { return EUIScene::GuildMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_Guild; }

	virtual void Tick(float DeltaSeconds) override;
	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

private:
	void SetUIData();
	void SetGuildJoin();
	void ReceiveCreateGuild(const FB2ReceiveCreateGuild& CreateGuild);
	void ReceiveGuildDetailInfo(const FB2ReceiveGuildInfo& GuildInfoData);
	void SetGuildFormation(const TArray<b2network::B2BattleFormationPtr>& FormationPtr);
private:
	uint32 DeliveryGetCreateGuildIDTicket;
	uint32 DeliveryGetGuildInfoTicket;
	uint32 DeliveryLeaveGuildTicket;
	uint32 DeliveryAskJoinGuildTicket;
	uint32 DeliveryGuidJoinBattleTicket;
	uint32 DeliveryGuildAcceptGuildTicket;
	uint32 DeliveryGuildBattleHistoryTicket;
	uint32 DeliverytGuildBattleSeasonRewardTicket;



	float	MatineePos;
	float	MatineeLength;
	bool	MatineeReverse;
};
class FLobbyGuildCreateAndJoinScene : public FLobbySceneBase
{
public:
	FLobbyGuildCreateAndJoinScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyGuildCreateAndJoinScene();

	virtual EUIScene GetUIScene() final { return EUIScene::GuildJoinAndCreate; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_GuildCreateAndJoin; }

	virtual void OpenScene() override;
	virtual void CloseScene() override;

};

class FLobbyRaidMainScene : public FLobbySceneBase
{
public:
	FLobbyRaidMainScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyRaidMainScene();

public:
	static const uint8	MAX_PLAYERS = 4;

public:
	virtual EUIScene GetUIScene() final { return EUIScene::RaidMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_RaidMain; }

public:
	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	// matching
public:
	virtual void OnHostChanged(int32 RoomType, int32 NewHostId) override;
	virtual void NotifySuccessMatchMake(bool IsHost) override;
	virtual void HandleRequestMatchMaking() override;
	virtual void HandleNotifyLeave(uint32) override;
	virtual void SetMyPlayerInfoData(uint32, const FB2FindAccountInfoPtr&) override;
	virtual void SetRemotePlayerInfoData(uint32, const FB2FindAccountInfoPtr&) override;
	virtual void SetRemotePlayerClass(uint32, EPCClass, const FB2FindAccountInfoPtr&) override;
	virtual void LeaveRoomIfJoined() override;
	virtual void HandleNotifyKickUser() override;
	virtual void HandleNotifyRoomLeave() override;
	virtual void HandleWaitToMeetCondition() override;
	virtual void HandlePreparePlayers() override;
	virtual void HandleFinishLobbyMatch() override;
	virtual int32 GetMaxMatchPlayerNum() override;
	virtual void ResponseChangeRoomAccess(uint8 AccessKey) override;
	virtual int32 GetCurrentModeMapId() override;
	virtual int32 GetCurrentModeDifficulty() override;
	virtual ERoomCloseReasonType GetStartRoomCloseReason() override;
	virtual ERoomCloseReasonType GetBreakRoomCloseReason() override;
	virtual void ReceiveNoResponsePrerequisiteToStart() override;

	void OnDeliveryRaidMatchmaking(const FB2ResponseRaidMatchmakingPtr& Message);

private:
	void UpdateUIHost(bool IsHost);
	void AddUIData(const uint32 NetId, const FB2ModPlayerInfo& info, const int GuildID);
	void AddUIData(const uint32 NetId, const EPCClass PCClass, const FB2ModPlayerInfo& info, const int GuildID);
	void AddUIData(const uint32 NetId, const EPCClass PCClass, const int32 CharLevel, const FText UserNick, const int32 CombatPower, const bool IsMySlot, const int GuildID);
	void RemoveUIData(const uint32 NetId);

	void	RemoveSlotIndex();
	int32	RemoveSlotIndex(const int32 NetId);
	void	InsertSlotIndex(const int32 SlotIdx, const int32 NetId);
	int32	InsertNextSlotIndex(const int32 NetId);
	int32	GetSlotIndex(const int32 NetId);
	int32	GetNextInsertSlotIndex(const int32 NetId);
	bool	IsExist(const int32 NetId) const;

	FString GetStageDifficultyString(EStageDifficulty InDifficulty);
	void	SetUIData(const FB2ResponseGetRaidPtr& MsgPtr);
	void	ResponseGetChallengeModeInfo(const FB2ResponseGetChallengeModeInfoPtr& MsgPtr);

private:
	uint32 DeliveryRaidMatchmakingTicket;
	uint32 DeliveryGetRaidTicket;
	uint32 DeliveryResponseGetChallengeModeInfoTicket;

	TArray<uint32> SlotIndexs;
};


class FLobbyCollectBookMain : public FLobbySceneBase
{
public:
	FLobbyCollectBookMain(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyCollectBookMain();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::CollectBookMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_CollectBookMain; }
	
	void OnCollectItemEquip(EPCClass CharClass, EItemEquipPlace EquippedPlace);
	void SelectCollectBookCharacter(EPCClass SelectedCharacter);
	void RotateCharacterYaw(float Value);

private:
	class FLobbySceneManager* SceneManager;
	EPCClass CurrentSelectedCharacter;
	//class AB2HeroMgntSkeletalMeshActor* CurrentCharacter;

	FCameraZoomModule ZoomModule;

	uint32 LobbyCharRotateCharacterTicket;
	uint32 LobbyCharEquippedItemTicket;
	uint32 LobbyCharCollectBookSelectTicket;
	uint32 LobbyCharZoomTicket;
};

class FLobbySummonItemScene : public FLobbySceneBase
{
	ELobbyShopGirlState CachedShopGirlReservedState;

public:
	FLobbySummonItemScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbySummonItemScene();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::SummonItemStore; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_SummonItem; }

protected:
	void SetSummonItemUIData(const FB2ResponseGetLotteryShopPtr&);

	TWeakObjectPtr<AMatineeActor> CachedItemSceneMatinee; // Cached due to some stupid reason.

	uint32 DeliveryGetLotteryShopTicket;
	uint32 HandleServerError7237Ticket;
};

class FLobbyGachaDisplaySceneBase : public FLobbySceneBase
{
public:
	FLobbyGachaDisplaySceneBase(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyGachaDisplaySceneBase();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	void OnFinishedSummonItemDisplayMatinee(class AMatineeActor* Matinee);
	void OnChangedToFxSummonItemDisplayMatinee(class AMatineeActor* Matinee);

	void PlaySummonItemMatinee();
	void PlayOpenSummonItemMatinee(bool InIsMulti);

protected:
	FB2AddedItems CurrentSummonedItems;
	ESummonItemDifficulty SummonDisplayDifficulty;
};

class FLobbySummonItemDisplayScene : public FLobbyGachaDisplaySceneBase
{
public:
	FLobbySummonItemDisplayScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbySummonItemDisplayScene();

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::SummonItemResult; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_SummonItemDisplay; }	

protected:
	void SetSummonedItemResult(const FB2ResponseDrawShopLotteryPtr& SummonResult);

	uint32 SummonitemOpenCardMatineePlayTicket;
	uint32 DeliveryDrawShopLotteryTicket;
};

class FLobbyGeneralLotteryDisplayScene : public FLobbyGachaDisplaySceneBase
{
public:
	FLobbyGeneralLotteryDisplayScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyGeneralLotteryDisplayScene();

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::GeneralLotteryResult; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_GeneralLotteryDisplay; }

protected:
	void SetGeneralLotteryItemResult(const FB2Item& SummonItemUsingLottery, ESummonItemDifficulty InSummonItemDisplay);

	uint32 DeliveryGeneralLotteryItemResultTicket;
};


class FLobbyGuildMapUIScene : public FLobbySceneBase
{
public:
	FLobbyGuildMapUIScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyGuildMapUIScene();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual void Tick(float DeltaSeconds) override;

	void SetGuildBattleInfo(const FB2ResponseGetGuildBattlePtr& GuildBattleInfo);

	void OpenGuildBattleTurnResult(const FB2ResponseGuildBattleTurnResultPtr& GuildBattleResult);
	void SetGuildMapChangeMatinee(class AMatineeActor* Atype, class AMatineeActor* Btype);

	void GuildChangeMatineeEnd();

	virtual EUIScene GetUIScene() final { return EUIScene::GuildMapUI; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_GuildMapUI; }

private:
	uint32 DeliveryGetGuildBattleInfoTicket;
	uint32 DeliveryGuildBattleTurnResultTicket;
	uint32 GuildStateChangeEventTicket;
	uint32 DeliveryStartGuildBattleTicket;

	class AMatineeActor* GuildMapChangeA;
	class AMatineeActor* GuildMapChangeB;

	bool ChangeState;
	bool FirstMatineePlay;
};

class FLobbyGuildBattleReady : public FLobbySceneBase
{
public:
	FLobbyGuildBattleReady(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyGuildBattleReady();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::GuildBattleReady; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_GuildBattleReady;}

private:
	uint32 DeliverySetGuildBattleEntryTicket;
};

class FLobbyControlMainScene : public FLobbySceneBase
{
public:
	FLobbyControlMainScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyControlMainScene();

public:

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::ControlMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_ControlMain; }

	void SetUIData(const FB2ResponseGetAssaultBattleStatusPtr& MsgPtr);

private:
	uint32 DeliveryGetAssaultBattleStatusTicket;
};
class FLobbyCharacterSelectPage : public FLobbySceneBase
{
public:
	FLobbyCharacterSelectPage(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyCharacterSelectPage();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::CharacterSelectPage; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_CharacterPage; }

	virtual void BattleStart() final;

private:
	void SelectCharacterStageInfo(EPCClass MainClass, EPCClass SubClass);

private:
	EPCClass CurrentMainClass;
	EPCClass CurrentSubClass;

	uint32 LobbyStageInfoSelectTicket;
};
class FLobbyFindAccountInfo : public FLobbySceneBase
{
public:
	FLobbyFindAccountInfo(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyFindAccountInfo();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual void SubscribeEvents_OnOpen() override;
	virtual void UnsubscribeEvents_OnClose() override;

	virtual EUIScene GetUIScene() final { return EUIScene::FindAccountInfo; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_FindAccount; }

	void FindAccountUIOpen(const FB2FindAccountPtr& AccountInfo);
	void CharacterSelect(EPCClass PCClass);
	void RotateCharacterYaw(float Value);
private:
	uint32 DeliveryResponseFindAccountTicket;
	uint32 LobbyHeroFindAccountTicket;
	uint32 LobbyCharRotateCharacterTicket;
	uint32 DeliveryAskFriendTicket;
	uint32 HandleServerError5503Ticket;
	EPCClass CurrentPCClass;
};
class FLobbyControlCharacterSelectPage : public FLobbySceneBase
{
public:

	FLobbyControlCharacterSelectPage(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyControlCharacterSelectPage();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::ControlCharacterSelectPage; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_ControlCharacterPage; }

	virtual void BattleStart() final;

	// matching

	//virtual void OnHostChanged(int32 RoomType, int32 NewHostId) override;
	virtual void NotifySuccessMatchMake(bool IsHost) override;
	virtual void HandleRequestMatchMaking() override;
	virtual void HandleNotifyLeave(uint32) override;
	virtual void SetMyPlayerInfoData(uint32, const FB2FindAccountInfoPtr&) override;
	virtual void SetRemotePlayerInfoData(uint32, const FB2FindAccountInfoPtr&) override;
	virtual void SetRemotePlayerClass(uint32, EPCClass, const FB2FindAccountInfoPtr&) override;
	virtual void LeaveRoomIfJoined() override;
	//virtual void HandleNotifyKickUser() override;
	//virtual void HandleNotifyRoomLeave() override;
	//virtual void HandleWaitToMeetCondition() override;
	virtual void HandlePreparePlayers() override;
	virtual void HandleFinishLobbyMatch() override;
	virtual int32 GetMaxMatchPlayerNum() override;
	//virtual void ResponseChangeRoomAccess(uint8 AccessKey) override;
	//virtual int32 GetCurrentModeMapId() override;
	//virtual int32 GetCurrentModeDifficulty() override;
	virtual ERoomCloseReasonType GetStartRoomCloseReason() override;
	//virtual ERoomCloseReasonType GetBreakRoomCloseReason() override;
	virtual void ReceiveMatchPlayerTeamInfo() override;
	virtual void ReceiveNoResponsePrerequisiteToStart() override;

	void SendControlMatchUserInfo();

private:
	void SelectCharacterStageInfo(EPCClass MainClass);

	void RefreshFindMatchPopup();

	// matching

	//void UpdateUIHost(bool IsHost);
	//void AddUIData(const uint32 NetId, const FB2ModPlayerInfo& info);
	//void AddUIData(const uint32 NetId, const EPCClass PCClass, const FB2ModPlayerInfo& info);
	//void AddUIData(const uint32 NetId, const EPCClass PCClass, const int32 CharLevel, const FText UserNick, const int32 AttackPoint, const int32 DefensePoint, const bool IsMySlot);
	//void RemoveUIData(const uint32 NetId);
	//
	//void	RemoveSlotIndex();
	//int32	RemoveSlotIndex(const int32 NetId);
	//void	InsertSlotIndex(const int32 SlotIdx, const int32 NetId);
	//int32	InsertNextSlotIndex(const int32 NetId);
	//int32	GetSlotIndex(const int32 NetId);
	//int32	GetNextInsertSlotIndex(const int32 NetId);

private:
	EPCClass CurrentMainClass;

	uint32 LobbyStageInfoSelectTicket;
};

class FLobbyEnterDungeonMain : public FLobbySceneBase
{
public:
	FLobbyEnterDungeonMain(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyEnterDungeonMain();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::EnterDungeonMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_EnterDungeonMain; }

	void ResponseGetChallengeModeInfo(const FB2ResponseGetChallengeModeInfoPtr& MsgPtr);

private:
	uint8 DeliveryResponseGetChallengeModeInfoTicket;
};

class FLobbyEnterShopMain : public FLobbySceneBase
{
public:
	FLobbyEnterShopMain(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyEnterShopMain();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::EnterShopMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_EnterShopMain; }
};

class FLobbyHeroTowerCharacterSelectPage : public FLobbySceneBase
{
public:
	FLobbyHeroTowerCharacterSelectPage(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyHeroTowerCharacterSelectPage();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::HeroTowerCharacterSelectPage; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_HeroTowerCharacterSelectPage; }

private:
	void SelectCharacterStageInfo(EPCClass MainClass, EPCClass SubClass);

private:
	EPCClass CurrentMainClass;
	EPCClass CurrentSubClass;

	uint32 LobbyStageInfoSelectTicket;
};

class FLobbyEnterBattleMain : public FLobbySceneBase
{
public:
	FLobbyEnterBattleMain(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyEnterBattleMain();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	void ResponseGetDuelModeInfo(const FB2ResponseGetDuelModeInfoPtr& MsgPtr);

	virtual EUIScene GetUIScene() final { return EUIScene::EnterBattleMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_EnterBattleMain; }

private:
	uint8	DeliveryResponseGetDuelModeInfoTicket;
};

class FLobbyPVPCharacterSelectPage : public FLobbySceneBase
{
public:
	FLobbyPVPCharacterSelectPage(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyPVPCharacterSelectPage();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::PVPCharacterSelectPage; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_PVPCharacterSelectPage; }

private:
	void SelectCharacterStageInfo(EPCClass MainClass, EPCClass SubClass);

private:
	EPCClass CurrentMainClass;
	EPCClass CurrentSubClass;

	uint32 LobbyStageInfoSelectTicket;
};

class FLobbyMission : public FLobbySceneBase
{
public:
	FLobbyMission(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyMission();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::Mission; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_Mission; }
};

class FLobbyEtherMain : public FLobbySceneBase
{
public:
	FLobbyEtherMain(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyEtherMain();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::EtherMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_EtherMain; }
};

class FLobbyRestPointScene : public FLobbySceneBase
{
public:
	FLobbyRestPointScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyRestPointScene();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::RestPointMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_RestPoint; }

protected:
	virtual void SubscribeEvents_OnOpen() override;
	virtual void UnsubscribeEvents_OnClose() override;

	void OnResponseReward(const FB2ResponseRestRewardPtr& msgPtr);

private:
	int32 DeliveryRestRewardTicket;
};

class FLobbyEventMain : public FLobbySceneBase
{
public:
	FLobbyEventMain(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyEventMain();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::EventMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_EventMain; }
};

class FLobbySeasonEvent : public FLobbySceneBase
{
public:
	FLobbySeasonEvent(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbySeasonEvent();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::SeasonEvent; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_SeasonEvent; }
};


class FLobbyPackageStore : public FLobbySceneBase
{
public:
	FLobbyPackageStore(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyPackageStore();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::PackageStore; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_PackageStore; }

	void SetPackageShopData(const FB2ResponseGetPackageShopPtr& ShopData);
	uint32 DeliveryGetPackageShopTicket;
};

class FLobbyHallOfFame : public FLobbySceneBase
{
private:
	b2network::B2HallOfFameUserPtr GetUserPtr(FB2ResponseGetHallOfFamePtr HallOfInfo, int64 InAccountId);

public:
	FLobbyHallOfFame(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyHallOfFame();
	
	virtual void OpenScene() override;
	virtual void ForceUpdate() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::HallOfFame; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_HallOfFame; }

	bool IsBackingScene() const { return BackingScene;  }
	int32 GetCurrentEquipRanking() const { return CurrentEquipRanking; }
	EPCClass GetRankerClass(int32 Ranking);
	EHallOfFameMode GetHallOfFameMode() const;

	TArray<FRankerInfo>& GetRankers(EHallOfFameMode GameMode);
	FRankerInfo GetRankerToRanking(int32 Ranking, EHallOfFameMode GameMode = EHallOfFameMode::None);
	FRankerInfo GetRankerToAccountID(int32 AccountID, EHallOfFameMode GameMode = EHallOfFameMode::None);

	void SetCelebratePopup();
	void SetRankeInfo(FB2ResponseGetHallOfFamePtr HallOfInfo);
	void SetCurrentEquipRanking(int32 InCurrentEquipRanking) { CurrentEquipRanking = InCurrentEquipRanking;}
	void SetHallOfCharactersInfo();
	void SetHallOfFameInfo(FB2ResponseGetHallOfFamePtr InHallOfFameInfo);
	void SetPraiseInfo(FB2ResponsePraiseTargetRankerPtr InPraiseInfo);

	void PlayStartMatinee();
	void OnFinishedHallOfFameDisplayMatinee(class AMatineeActor* Matinee);
	void ForceEndMatinee();
	void UIVisibility(bool IsVisible);

	// for mesh
	TArray<FHallOfFameCharacterInfo> GetFirstRankCharacterMeshInfo();
	FHallOfFameCharacterInfo GetHallOfFameCharacter(int32 Ranking);
	//UB2PCClassInfo* GetSingleClassInfo(int32 Ranking);
	//class UAnimationAsset* GetAnimation(int32 Ranking);
	void SetPlayAnimationForEndMatinee();
	void GetEquipItems(TArray<FB2Item>& EquipItems, FB2Wing& InWing);
	void SetHallOfFameCharactersMesh();
	void SetFirstCharacterVisible();

	struct FPraiseCount
	{
		int32 CurrentPraise;
		int32 MaxPraise;
		int64 PraisedAccountID;
	};

	struct FHallOfFameReward
	{
		int32 Ranking;
		TArray<b2network::B2RewardPtr> RewardItem;
	};
private:
	bool BackingScene;
	uint32 EndHallOfFameMatineeTicket;
	int32 CurrentEquipRanking;

	TMap<EHallOfFameMode, bool> PlayedMatinee;
	TMap<EHallOfFameMode, TArray<FRankerInfo>> RankersInfo;
	TMap<EHallOfFameMode, FHallOfFameReward> RewardInfo;
	TMap<EHallOfFameMode, FPraiseCount> PraiseInfo;
};

class FLobbyMagicStore : public FLobbySceneBase
{
public:
	FLobbyMagicStore(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyMagicStore();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::MagicStore; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_MagicStore; }

private:
	void SetMagicStoreData(const FB2ResponseGetMagicShopPtr& MagicShopPtr);

private:
	uint32 DeliveryGetMagicShopTicket;

};

class FLobbyGuildMercenary : public FLobbySceneBase
{
public:
	FLobbyGuildMercenary(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyGuildMercenary();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::GuildMercenary; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_Mercenary; }

};

class FLobbyAdventureEnterBattleMain : public FLobbySceneBase
{
public:
	FLobbyAdventureEnterBattleMain(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyAdventureEnterBattleMain();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::AdventureEnterBattleMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_AdventureEnterBattleMain; }

};

class FLobbyFairyScene : public FLobbySceneBase
{
public:
	FLobbyFairyScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyFairyScene();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::FairyMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_FairyMain; }

protected:
	virtual void SubscribeEvents_OnOpen() override;
	virtual void UnsubscribeEvents_OnClose() override;

	void OnPlayFairyBlessAnim(EFairyType eFairyType);
	
	ELobbyNPCType ConvertFairyTypeToLobbyNPCType(EFairyType eFairyType);
	
	void OnChangeFairyActor(EFairyType eFairyType);

private:
	ELobbyNPCType CurrentFairyType;
	TArray<issue_ptr>	Issues;
	bool bEventsSubscribed;
};

class FLobbyItemForgeScene : public FLobbySceneBase
{
public:
	FLobbyItemForgeScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyItemForgeScene();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::ItemForge; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_ItemForge; }

private:
	uint32 DeliveryGetItemForgeListTicket;
};

class FLobbySealBoxResultScene : public FLobbySceneBase
{
public:
	FLobbySealBoxResultScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbySealBoxResultScene();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

	virtual EUIScene GetUIScene() final { return EUIScene::SealBoxResult; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_SealBoxResult; }

private:
	uint32 DeliverySealBoxResultTicket;
};

class FLobbyDonationScene : public FLobbySceneBase
{
public:
	FLobbyDonationScene(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyDonationScene();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::DonationEvent; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_DonationEvent; }

protected:
	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

private:
	void ResponseGetDonationEvent(const FB2GetDonationEventPtr& InData);
	void ResponseMakeDonation(const FB2MakeDonationPtr& InData);
	void ResponseDonationPointReward(const FB2DonationPointRewardPtr& InData);

private:
	TArray<issue_ptr>	Issues;
	bool IsOpenScene;
};

class FLobbyDimensionSelectPage : public FLobbySceneBase
{
public:
	FLobbyDimensionSelectPage(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyDimensionSelectPage();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::DimensionSelectPage; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_DimensionSelectPage; }

	void SelectCharacterStageInfo(EPCClass MainClass, EPCClass SubClass);

protected:
	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

private:
	TArray<issue_ptr>	Issues;
	bool IsOpenScene;

	EPCClass CurrentMainClass;
	EPCClass CurrentSubClass;

};

class FLobbyDimensionMain : public FLobbySceneBase
{
public:
	FLobbyDimensionMain(class FLobbySceneManager* OwnerSceneManager);
	virtual ~FLobbyDimensionMain();

	virtual void OpenScene() override;
	virtual void CloseScene() override;

	virtual EUIScene GetUIScene() final { return EUIScene::DimensionMain; }
	virtual ELobbyScene GetLobbyScene() final { return ELobbyScene::ELobbyScene_DimensionMain; }

protected:
	virtual void SubscribeEvents_OnConstruct() override;
	virtual void UnsubscribeEvents_OnDestruct() override;

private:
	TArray<issue_ptr>	Issues;
};