#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UMG.h"
#include "B2NetGameMode.h"
#include "B2ClientDataStore.h"
#include "UObject/Class.h"
#include "B2ControlGameMode.generated.h"

UENUM()
enum class EControlMatchSceneState : uint8
{
	None,
	Intro,			// 牢飘肺
	Ready,			// 霖厚肯丰
	WaitActive,		// 芭痢 劝己拳 措扁
	InGame,			// 霸烙吝
	EndMatch		// 霸烙 辆丰
};

UENUM()
enum class EControlMatchSubmersionState : uint8
{
	None,
	Play,
	FirstWarning,
	Warning,
	Kickout,
};

USTRUCT(BlueprintType)
struct FControlMatchAnims
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimationAsset* Idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimationAsset* Mvp;
};

UENUM()
enum class EControlTutorialActionStep : uint8
{
	ViewCenterCamera,			// 吝居俊 墨皋扼初扁
	ViewSanctuaryBottomCamera,	// 窍窜 己家 墨皋扼初扁
	ViewSanctuaryTopCamera,		// 惑窜 己家 墨皋扼初扁
	ViewIngameCamera,			// 墨皋扼父 牢霸烙栏肺 汗备
	SetIngame,			// 场 汗备
};

USTRUCT(BlueprintType)
struct FControlResurrectInfo
{
	GENERATED_BODY()

	FControlResurrectInfo()
	{
		ResurrectCount = 1;
		ResurrectTime = 10;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ResurrectCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ResurrectTime;
};

class FControlMatchSceneManager
{
public:
	static const int32 BATTLE_TIME = 90;
public:
	FControlMatchSceneManager() :
		CurrentState(EControlMatchSceneState::None), GameMode(nullptr)
	{
		ControlGamePlayStateTime = -1.0f;
	}

	EControlMatchSceneState GetState() { return CurrentState; }

	void SetGameMode(class AB2ControlGameMode* InGameMode);
	void SetMatchLobbyResources(class ALevelSequenceActor * InResultMatinee,
		class AB2StageEventDirector* InIntroEventDirector,
		const FControlMatchAnims& Gladiator,
		const FControlMatchAnims& Assassin,
		const FControlMatchAnims& Wizard,
		const FControlMatchAnims& Fighter);
	void SetState(EControlMatchSceneState InState, bool bNoAction = false);

	void CheckIngameStateTime();

	void SetIntro();
	void SetReady();
	void SetWaitActive();
	void SetInGame();
	void SetEndMatch();
	class ASkeletalMeshActor* SpawnMvpPuppetActor(EPCClass CharClass, const class ICharacterDataStore* DataStore, int32 PlayerIndex);
	
	float GetIngamePlayStateElapseTime();
	void SetControlGamePlayStateTime(float fTimeSec);

	void MatchResultMatineeFinished();

private:
	EControlMatchSceneState CurrentState;

	class ALevelSequenceActor * ResultMatinee;
	TArray<FControlMatchAnims> AnimArray;
	TArray<class ASkeletalMeshActor*> SpawnedActors;

	class AB2ControlGameMode* GameMode;

	class AB2StageEventDirector* IntroEventDirector;

	float ControlGamePlayStateTime;
};

USTRUCT()
struct FControlPeerInfo
{
	GENERATED_USTRUCT_BODY()

	FControlPeerInfo()
	{
		PlayerStartIdx = 0;
		IsReady = false;
	}

	bool IsReady;
	uint8 PlayerStartIdx;
};

class ControlGameRule : public DefaultGameRule
{
public:
	virtual FString GetBehaviorTreeAsset() const override
	{ 
		return FString(TEXT("/Game/Blueprints/AI/ControlModePlayerBehaviorTree.ControlModePlayerBehaviorTree")); 
	}

	virtual bool IsQTEEnabled() const override
	{ 
		return true; 
	}

	virtual bool IsGlobalTimeDilationEnabled() const override { return false; }
	virtual bool AllowPlayerPointLight() const override { return false; } // Performance rule
	virtual bool UseSectionMergedPCModel() const override { return true; }  // Performance rule
	virtual bool AllowToTag() const override { return false; }
	virtual bool AllowOtherPlayerDamageNumNotify() const override { return false; }
	virtual bool IsEvadeCoolTimeCheck() const override { return true; }
	virtual bool ForceApplyHitAnimation() const { return true; }
	virtual bool IsHitFreezeEnabled() const override { return false; }
};

#define CONTROL_MODE_ORIGINAL 0
#define CONTROL_MODE_NOMINO 1

UCLASS()
class BLADEII_API AB2ControlGameMode : public AB2NetGameMode
{
	GENERATED_BODY()

public:
	static const uint8		START_PLAYERS = 2;

	AB2ControlGameMode(const FObjectInitializer& ObjectInitializer);

	// Actor interfaces...
	virtual void Tick(float) final;
	virtual void BeginPlay() final;
	virtual void BeginDestroy() final;
	virtual void Destroyed() final;

	// GameMode interfaces...
	virtual void SetMatchState(FName NewState) final;

	// BladeIIGameMode interfaces...
	/** Called before any other actor's BeginPlay is called. */
	virtual void PreBeginPlay()			final;
	virtual void SubscribeEvents()		final; // Events of EventSubsystem
	virtual void ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel = EB2GraphicsLevel::GraphicsLevel_End, EB2ResolutionLevel InSelectedResLevel = EB2ResolutionLevel::End) override;
	virtual FString GetGameModeScalabilitySettingSectionPostfix() const override;

	virtual void NotifyCharacterBeginPlay(ABladeIICharacter* TargetCharacter) final;
	virtual void NotifyPlayerDeadEnd(ABladeIIPlayer*) final;
	virtual void NotifyCharacterDead(ABladeIICharacter*, APawn*, AActor*) final;
	virtual void NotifyPlayerStartSkill(class ABladeIIPlayer* pPlayer, EAttackState SkillAttackState) final;

	virtual void GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 InMyTeamNum = INDEX_NONE, bool bOnlyLiveMobs = true) final;
	virtual EB2GameMode GetB2GameModeType() final { return EB2GameMode::Control; }

	virtual EB2SkillLODType GetSkillLODType() final { return EB2SkillLODType::ESLT_ONLY_FOR_SELF; }
	virtual EPCSpawnMotionState GetBornAgainSpawnMotionState() final { return EPCSpawnMotionState::EPST_BornAgain_ForControlGame; }

	virtual void NotifyTargetDamagedFromDamagedTarget(class ABladeIICharacter* DamageCauser, class ABladeIICharacter* DamagedTarget, float fDamageAmount) final;
	//virtual ABladeIICharacter* SpawnCharacter(UB2NPCClassInfoBox*, ENPCClass, ENPCClassVariation, FTransform&, FActorSpawnParameters&) override;

	virtual ACharacter* GetNearestPlayerCharacter(class ACharacter*, bool = false) final;

	virtual bool IsPVPGameMode() const { return true; }

	virtual float GetCurrentStageDifficultyScale()		final { return 1.f; }
		
	virtual bool IsQTEEnabled() final { return true; }

	virtual void NotifyAnimMobDeadEnd(class ABladeIICharacter* TargetCharacter) final;
	virtual void NotifyQTEMountBeginToIdle(class ABladeIIPlayer* pPlayer) final;

	virtual float GetTotalAttack(ICharacterDataStore*) final;
	virtual float GetTotalDefense(ICharacterDataStore*) final;

	virtual void GatherEquippedSkillAnimIDs(ICharacterDataStore* DataStore, TArray<FCombinedPCSkillAnimID>& OutEquippedSkills) override;

	virtual void GetAllMyTeamCharList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum, bool bOnlyLive = true) override;

	// NetGameMode interfaces...
	virtual void HandleRequestMatchMaking() final;
	virtual void HandleMatchHasStarted() final;
	virtual void HandleLobbyMatchToGameMode() final;
	virtual void SetResurrectPlayerPosition(ABladeIIPlayer* pBladePlayer = NULL) final;
	virtual void SetPlayerCollisionType(ABladeIINetPlayer* pPlayer) final;

	virtual void OnHostChanged(int32, int32) final;
	virtual void HandleWaitToMeetCondition() final;
	
	virtual void HandleNotifyLeave(uint32) final;

	virtual void CheckNetAIPlayerResurrect(class ABladeIINetPlayer* pPlayer) final;
	virtual bool CanTakeDamage() override;

	virtual bool IsAllowCameraWorking() override;

	virtual EResurrectGameModeType GetResurrectGameModeType() override { return EResurrectGameModeType::EResurrectType_OnlyClientMode; }

	virtual void ForceResurrectLocalPlayer();

	void PrepareHostPlayer();
	void PrepareRemotePlayer(uint32);
	void RestartRemotePlayer(uint32);
	void AddPeerInfo(uint32 NetId, uint32 StartIndex);

	void CheckControlBattleStart();

	void CheckKicked();

	void SetReady();

	UFUNCTION(BlueprintCallable, Category = "ControlObject")
	void SetControlObject(class ABladeIINetControlObject* InControlObject);

	FVector GetControlObjectLocation();
	float GetControlObjectViewLimitLength();

	UFUNCTION(BlueprintCallable, Category = "ControlObject")
	void AddSanctuaryControlGame(class AB2SanctuaryControlGame* InSanctuary);
	
	void SendChangeControlState(uint8 nNewState, float fControlPointGageRed, float fControlPointGageBlue, bool bResetConquestGage);
	void AckChangeControlState(uint8 nNewState, float fControlPointGageRed, float fControlPointGageBlue, bool bResetConquestGage);
	void SendChangeConquestAreaState(uint8 nNewState, float fConquestGageRed, float fConquestGageBlue);
	void AckChangeConquestAreaState(uint8 nNewState, float fConquestGageRed, float fConquestGageBlue);

	void AckControlStartGame();

	void SendLeavePlayer(uint32 NetId, int64 AccountId);
	void SendEndMatch(ETeamType WinTeam);
	void AckEndMatch(ETeamType WinTeam);

	TArray<class ABladeIINetPlayer*> GetPlayerArray();
	TArray<class ABladeIINetPlayer*> GetAllyPlayerArray();
	TArray<class ABladeIINetPlayer*> GetTeamPlayerArray(ETeamType TeamType);

	void SetControlKillInfos(TMap<uint32, int32>* ControlKillInfos);
	void NetSendControlKillInfos();

	void NetSendControlTimerInfos();
	void AckControlTimerInfos(uint8 SceneState, float ElapseTime);

	void NetSendAskSpawnComplete();
	void NetReceiveAskSpawnComplete();
	void NetSendSpawnComplete();
	void NetReceiveSpawnComplete(uint32 NetId, bool bForce);
	void NetSendSpawnCompleteForceAI(uint32 NetId);

	void NetReceiveQTEEnable(uint32 NetId, uint8 TeamNum);

	void NetSendQTEEnableEnd();
	void NetReceiveQTEEnableEnd();

	void NetSendConsumeControlSanctuary(int32 SancIndex);
	void NetReceiveConsumeControlSanctuary(int32 SancIndex);

	void NetSendRespawnControlSanctuary(int32 SancIndex);
	void NetReceiveRespawnControlSanctuary(int32 SancIndex);

	void NetSendDrinkControlSanctuary(uint32 NetId, int32 SancIndex);
	void NetReceiveDrinkControlSanctuary(uint32 NetId, int32 SancIndex);

	int32 GetMyTeamNum();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void FinishReadyToStartGame();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetMatchLobbyResources(class ALevelSequenceActor* InResultMatinee,
		class AB2StageEventDirector* InIntroEventDirector,
		const FControlMatchAnims& Gladiator,
		const FControlMatchAnims& Assassin,
		const FControlMatchAnims& Wizard,
		const FControlMatchAnims& Fighter,
		TArray<AEmitter*> EmitterRed,
		TArray<AEmitter*> EmitterBlue,
		AEmitter* WinEmitterRed,
		AEmitter* WinEmitterBlue);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetTutorialCamera(class AB2ActiveCameraActor* pCenterCamActor,
		class AB2ActiveCameraActor* pSanctuaryBottomCamActor,
		class AB2ActiveCameraActor* pSanctuaryTopCamActor);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetDeadWatchingCamera(class AB2ActiveCameraActor* pCamActor);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void MatchResultMatineeFinished();

	void ProcessTutorialAction(EControlTutorialActionStep ActionStep);	

	virtual ERoomCloseReasonType GetStartRoomCloseReason() final;

	virtual bool CanStartInGame() final;

	bool IsMyAssaultState();

	bool IsAssaultState();

	void ReturnToControlMainMenu();

	void SetFindingEnd();
	
	void SetMvpScene();

	void SetResultScene();

	void PlayEndBgm();

	TMap<uint32, FControlPeerInfo>& GetAllPeerInfos();
	
	void StartPlay();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetSceneManagerState(EControlMatchSceneState NewState);
	
	void ApplyCurrentStateToUI();
	
	void SetActiveControlObject(bool NewActive);

	void StopAIControllers();

	void SendGamePlayInfo();

	void ReserveSpawnControlNPC(bool bFirst);

	void NetSendSpawnControlNPC();
	void NetReceiveSpawnControlNPC();

	void ReserveRespawnControlSanctuary(int32 SancIndex = -1);

	void CheckControlSanctuaryRespawn(int32 SancIndex);

	ETeamType GetControlTeam(uint8 PlayerStsrtIndex);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool IsFinishedControlMatch() { return bEndMatch; }

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	float GetControlObjectBonusTimeGage();

	UFUNCTION(BlueprintImplementableEvent)
	void CreateControlNpcRemainTimeFloatingGageBar(AActor* Target, float RemainTime);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "BladeII")
	void DestroyControlNpcRemainTimeFloatingGageBar();

	UFUNCTION(BlueprintImplementableEvent)
	void CreateControlPlayerArrow(AActor* Target);

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyControlPlayerArrow();

	void ControlForceStart();

	void ControlWallCollisionOff();

	void NetSendRequestQTE();
	void NetReceiveRequestQTE(uint32 NetId);

	void NetSendResponseQTE(uint32 NetId, bool bAllow);
	void NetReceiveResponseQTE(uint32 NetId, bool bAllow);

	void NetSendNotifyEndQTE(uint32 NetId = 0);
	void NetReceiveNotifyEndQTE(uint32 NetId);	

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	float GetControlNpcRemainHpRate();

	void NetSendControlChat(EControlChatType ChatType, int32 MessageIndex);
	void NetReceiveControlChat(uint32 NetId, EControlChatType ChatType, int32 MessageIndex);

	void NetSendLeaveAllToPeer();
	float GetControlResurrectTime(uint32 NetId);

	void ActiveDeadWatchingCamera(bool bActive);

	void ActiveTutorialCenterCamera(bool bActive);
	void ActiveTutorialSanctuaryCameraBottom(bool bActive);
	void ActiveTutorialSanctuaryCameraTop(bool bActive);

	void GotoTutorialNextStep();

	int32 GetActualDamage();

	void TouchPressed();
	void SetWaitRevive(bool InValue);

protected:
	int32 GetStartPlayerNum();
	int32 GetMatchPlayerNum();
	int32 GetMaxMatchPlayerNum();
	bool IsMaxMatchPlayerNum();

	void CheckLeaveAll();
	void StartLeaveAllDirection();
	void EndLeaveAllGame();

	void UpdateTimerUI();

	void UpdateConquestEffect();

	uint8 GetNextPlayStartIdx(bool IsMyStartIdx);

	FString GetPlayerStartTagName(FControlPeerInfo& PeerInfo);

	void SetPlayerToBattleUI(class ABladeIINetPlayer* pPlayer);
	void SetKillInfoToBattleUI(class ABladeIINetPlayer* pKillerPlayer, class ABladeIINetPlayer* pDeadPlayer);
	void ViewPlayerJoinUI(class ABladeIINetPlayer* pPlayer);
	
private:
	void CheckSubmersion(float InDelta);
	void SubmersionOverTimePlayState();
	void SubmersionOverTimeWarningState();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ControlMob)
	ENPCClass ControlNPCClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ControlMob)
	ENPCClassVariation ControlNPCVariation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ControlPlayer)
	TSubclassOf<class UB2FloatingHPBar> FloatingHPBarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timer)
	float WaitInProgressTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timer)
	float ReadyFightActionTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timer)
	float ResultActionTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timer)
	float WaitFinalResultTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timer)
	float WaitActiveTime;		// 芭痢 劝己拳 措扁

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timer)
	float WaitInMatchMakingTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timer)
	float ForceStartTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ControlPlayer)
	TArray<FControlResurrectInfo> ControlResurrectInfos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timer)
	float NpcFirstSpawnWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timer)
	float NpcReSpawnWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timer)
	float SanctuaryReSpawnWaitTime;

	uint32									ReceivedRoomLeaveResponseTicket;
	uint32									ReceivedSyncStartGameTicket;
	uint32									ReceivedConquestAreaStateTicket;
	uint32									ReceivedControlStateTicket;
	uint32									ReceivedControlKillCountTicket;
	uint32									ReceivedControlResultTicket;
	uint32									ReceivedControlTimerTicket;
	uint32									ReceivedAskSpawnCompleteTicket;
	uint32									ReceivedSpawnCompleteTicket;
	uint32									ReceiveRequestQTETicket;
	uint32									ReceiveResponseQTETicket;
	uint32									ReceiveNotifyEndQTETicket;
	uint32									ReceiveQteEnableTicket;
	uint32									ReceiveQteEnableEndTicket;
	uint32									ReceiveControlChatTicket;
	uint32									ReceiveSpawnControlNpcTicket;
	uint32									ReceiveLeaveAllTicket;
	uint32									ReceivedConsumeControlSanctuaryTicket;
	uint32									ReceivedRespawnControlSanctuaryTicket;
	uint32									ReceivedDrinkControlSanctuaryTicket;

	TMap<uint32, FControlPeerInfo>			AllPeerInfos;
	TArray<uint32>							AIControlleredPeerIds;
	FControlMatchSceneManager				SceneManager;

	FTimerHandle							WaitInProgressTimeHandle;
	FTimerHandle							ResultActionTimeHandle;
	FTimerHandle							WaitFinalResultTimeHandle;
	FTimerHandle							RequestMatchMakingTimeHandle;
	FTimerHandle							ForceStartTimeHandle;
	FTimerHandle							NpcSpawnTimeHandle;
	FTimerHandle							TimeToLeaveEnemy;
	FTimerHandle							TutorialWaitHandle;

	ETeamType ResultWinTeam;
	ETeamType ResultLeaveAllTeam;

	bool bSendEndMatch;
	bool bEndMatch;
	bool bStartMatch;
	float m_fTotalPlayTimeSec;
	
	bool bIsKicked;
	bool bKickChecked;

	TMap<int32, float> ControlSanctuaryRespawnRemainTimeInfos;

protected:
	class ABladeIINetControlObject* m_pControlObject;
	TArray<AB2SanctuaryControlGame*> m_arSanctuary;

private:
	UPROPERTY(Transient)
	class ABladeIIControlNetCharacter*					ControlNPC;

	class AB2ActiveCameraActor* DeadWatchingCamera;

	class AB2ActiveCameraActor* TutorialCameraCenter;
	class AB2ActiveCameraActor* TutorialCameraSanctuaryBottom;
	class AB2ActiveCameraActor* TutorialCameraSanctuaryTopCamera;

	int32 LastActiveFxIndex;
	TArray<AEmitter*> ConquestAreaFxEmittersRed;
	TArray<AEmitter*> ConquestAreaFxEmittersBlue;
	AEmitter* WinFxEmittersRed;
	AEmitter* WinFxEmittersBlue;

	// 己家 付脚磊 沥焊. 虐绰 己家酒捞叼 蔼篮 冈篮磊 齿酒捞叼
	TMap<int32, uint32> SanctuaryDrunkInfos;

	bool m_bApplyedNpcSpawnTimer;

	uint32 ReservedQTEPlayerNetId;

	UPROPERTY(EditAnywhere, Category = Submersion)
	float KickoutSubmersionTime;
	UPROPERTY(EditAnywhere, Category = Submersion)
	float WarningSubmersionTime;

	float MaxSubmersionTime;
	float SubmersionTime;
	EControlMatchSubmersionState SubmersionState;
	int32 DeliveryAssaultResultTicket;
	bool IsWaitRevive;
};
