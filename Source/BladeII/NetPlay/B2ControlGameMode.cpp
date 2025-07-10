
#include "B2ControlGameMode.h"

#include "BehaviorTree/BehaviorTree.h"
#include "B2NPCClassInfoBox.h"
//#include "B2ControlModePlayer.h"
//#include "B2UIManager.h"
#include "BladeIINetCharacter.h"
#include "ChannelManager.h"
#include "Event.h"
#include "Packet.h"
#include "PacketMaker.h"
#include "Retailer.h"
#include "Serializer.h"
//#include "BladeIINetControlObject.h"
//#include "B2UIManager.h"
//#include "B2UIControlBattleSub.h"
#include "B2UIDocHelper.h"
//#include "B2UIWaitRevive.h"
////#include "B2FloatingHPBar.h"

#include "B2PCClassInfoBox.h"
////#include "Matinee/MatineeActor.h"
//#include "B2UIControlMvpView.h"
#include "BladeIIPlayerAIController.h"
#include "BladeIIControlNetCharacter.h"
//#include "B2UIControlReadyToBattle.h"
#include "B2MonsterSpawnPool.h"
#include "BladeIIAIController.h"
//#include "B2UIManager_InGameCombat.h"
#include "B2GameLoadingProgress.h"

#include "FB2ErrorMessage.h"
#include "FakeConfigure.h"
#include "BladeIIGameImpl.h"
//#include "B2SanctuaryControlGame.h"
#include "B2ActiveCameraActor.h"
#include "TutorialManager.h"
#include "BladeIIPlayerController.h"
#include "BladeIIScalabilityControl.h"
#include "B2GameDefine.h"
#include "BladeIINetPlayer.h"
#include "Engine/PawnIterator.h"

DEFINE_LOG_CATEGORY_STATIC(LogControlGameMode, Log, All);

namespace ControlMatchState
{
}

AB2ControlGameMode::AB2ControlGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	//DefaultPawnClass = AB2ControlModePlayer::StaticClass();
	//DefaultRemotePawnClass = AB2ControlModePlayer::StaticClass();
	AllPeerInfos.Empty();

	//m_pControlObject = NULL;
	//
	//if (!GMinimalDLCFrontMode)
	//{ // DLC Front 모드 리소스로딩 최대한 제거

	//	// 플레이어 케릭터용 체력바+구르기쿨타임용 UI클래스
	//	// BP에서 재설정할수도있음
	//	FString ControlGameModeFloatingHPBarWidgetClassPath;
	//	GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("ControlGameModeFloatingHPBarWidgetClass"), ControlGameModeFloatingHPBarWidgetClassPath, GGameIni);
	//	static ConstructorHelpers::FClassFinder<UB2FloatingHPBar> ControlGameModeFloatingHPBarWidgetClass(*ControlGameModeFloatingHPBarWidgetClassPath);
	//	if (ControlGameModeFloatingHPBarWidgetClass.Class != NULL)
	//	{
	//		FloatingHPBarWidgetClass = ControlGameModeFloatingHPBarWidgetClass.Class;
	//	}
	//}

	// 대기시간 초기값
	WaitInProgressTime = 3.0f;
	ResultActionTime = 4.0f;
	WaitFinalResultTime = 5.0f;

	WaitActiveTime = 7.0f;

	WaitInMatchMakingTime = 1.2f;

	ForceStartTime = 30.0f;
	
	ResultWinTeam	   = ETeamType::ETT_End;	
	ResultLeaveAllTeam = ETeamType::ETT_End;

	bSendEndMatch = false;
	bEndMatch = false;
	bStartMatch = false;

	m_fTotalPlayTimeSec = 0.0f;

//	ControlNPCClass = ENPCClass::ENC_Mob_Minotaur;
//	ControlNPCVariation = ENPCClassVariation::ENCV_Normal;

	LastActiveFxIndex = -1;
	
	ControlResurrectInfos.Empty();

	NpcFirstSpawnWaitTime = 10.0f;

	NpcReSpawnWaitTime = 10.0f;

	WinFxEmittersRed = NULL;
	WinFxEmittersBlue = NULL;

	ReadyFightActionTime = 5.0f;

	bIsKicked = false;
	bKickChecked = false;

	SanctuaryReSpawnWaitTime = 20.0f;

	ControlSanctuaryRespawnRemainTimeInfos.Empty();

	//m_arSanctuary.Empty();

	SanctuaryDrunkInfos.Empty();

	m_bApplyedNpcSpawnTimer = false;

	ReservedQTEPlayerNetId = 0;
	
	SubmersionState = EControlMatchSubmersionState::None;

	IsWaitRevive = false;
}

void AB2ControlGameMode::Tick(float delta)
{
	Super::Tick(delta);

	//SceneManager.CheckIngameStateTime();

	//UpdateTimerUI();

	//UpdateConquestEffect();

	//// 내가 점령기여하구있는지 체크(조건 그냥 점령지 밟고있는가)
	//if (m_pControlObject && m_pControlObject->IsOverlap(GetLocalController()->GetPawn()))
	//	UB2UIDocHelper::GetDocControl()->AddMyContributeTime(delta);

	//// 성소 리젠 타이머들 체크
	//for (TPair<int32, float>& TimeInfo : ControlSanctuaryRespawnRemainTimeInfos)
	//{
	//	if (TimeInfo.Value >= 0.0f)
	//	{
	//		TimeInfo.Value -= delta;

	//		if (TimeInfo.Value < 0.0f)
	//			CheckControlSanctuaryRespawn(TimeInfo.Key);
	//	}
	//}

	//// QTE가능상태 이전에 QTE시작패킷 받은케이스
	//if (ControlNPC && ControlNPC->IsTimingForQTEPossession() && ReservedQTEPlayerNetId != 0)
	//{
	//	NetReceiveResponseQTE(ReservedQTEPlayerNetId, true);
	//	ReservedQTEPlayerNetId = 0;
	//}

	//CheckSubmersion(delta);
}

void AB2ControlGameMode::BeginPlay()
{
	Super::BeginPlay();

	//StageBeginClass<>::GetInstance().Signal();

	//PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_ControlBattleCombat);

	//SubmersionState = EControlMatchSubmersionState::Play;
	//MaxSubmersionTime = WarningSubmersionTime;
}

void AB2ControlGameMode::BeginDestroy()
{
	Super::BeginDestroy();
}

void AB2ControlGameMode::Destroyed()
{
	Super::Destroyed();
}

void AB2ControlGameMode::SetMatchState(FName NewState)
{
	if (MatchState == NewState)
	{
		return;
	}

	Super::SetMatchState(NewState);
}

void AB2ControlGameMode::PreBeginPlay()
{
	//auto* GameRule = new ControlGameRule;
	//SetGameRule(GameRule);

	//Super::PreBeginPlay();

	//UB2UIDocHelper::GetDocControl()->ResetSerialKillCount();
	//UB2UIDocHelper::GetDocControl()->ResetMatchRecordInfos();
}

void AB2ControlGameMode::SubscribeEvents()
{
	//if (bEventsSubscribed)
	//	return;

	//Super::SubscribeEvents();

	//Issues.Add(ReceivedRoomLeaveResponseClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->ReturnToControlMainMenu();
	//	}
	//));
	//Issues.Add(ReceivedSyncStartGameClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->AckControlStartGame();
	//	}
	//));
	//Issues.Add(ReceivedConquestAreaStateClass<uint8, float, float>::GetInstance().Subscribe2(
	//	[this](uint8 InState, float RedRate, float BlueRate)
	//	{
	//		this->AckChangeConquestAreaState(InState, RedRate, BlueRate);
	//	}
	//));
	//Issues.Add(ReceivedControlStateClass<uint8, float, float, bool>::GetInstance().Subscribe2(
	//	[this](uint8 InState, float RedRate, float BlueRate, bool bResetConquestGage)
	//	{
	//		this->AckChangeControlState(InState, RedRate, BlueRate, bResetConquestGage);
	//	}
	//));
	////킬인포 받는 이벤트 티켓
	//Issues.Add(ReceivedControlKillCountClass<TMap<uint32, int32>*>::GetInstance().Subscribe2(
	//	[this](TMap<uint32, int32>* ControlKillInfos)
	//	{
	//		this->SetControlKillInfos(ControlKillInfos);
	//	}
	//));
	//Issues.Add(ReceivedControlResultClass<ETeamType>::GetInstance().Subscribe2(
	//	[this](ETeamType WinTeam)
	//	{
	//		this->AckEndMatch(WinTeam);
	//	}
	//));
	//Issues.Add(ReceivedControlTimerClass<uint8, float>::GetInstance().Subscribe2(
	//	[this](uint8 SceneState, float ElapseTime)
	//	{
	//		this->AckControlTimerInfos(SceneState, ElapseTime);
	//	}
	//));
	//Issues.Add(ReceivedAskSpawnCompleteClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->NetReceiveAskSpawnComplete();
	//	}
	//));
	//Issues.Add(ReceivedSpawnCompleteClass<uint32, bool>::GetInstance().Subscribe2(
	//	[this](uint32 NetId, bool bForce)
	//	{
	//		this->NetReceiveSpawnComplete(NetId, bForce);
	//	}
	//));
	//Issues.Add(ReceiveRequestQTEClass<uint32>::GetInstance().Subscribe2(
	//	[this](uint32 NetId)
	//	{
	//		this->NetReceiveRequestQTE(NetId);
	//	}
	//));
	//Issues.Add(ReceiveResponseQTEClass<uint32, bool>::GetInstance().Subscribe2(
	//	[this](uint32 NetId, bool bAllow)
	//	{
	//		this->NetReceiveResponseQTE(NetId, bAllow);
	//	}
	//));
	//Issues.Add(ReceiveNotifyEndQTEClass<uint32>::GetInstance().Subscribe2(
	//	[this](uint32 NetId)
	//	{
	//		this->NetReceiveNotifyEndQTE(NetId);
	//	}
	//));
	//Issues.Add(ReceiveQteEnableClass<uint32, uint8>::GetInstance().Subscribe2(
	//	[this](uint32 NetId, uint8 TeamNum)
	//	{
	//		this->NetReceiveQTEEnable(NetId, TeamNum);
	//	}
	//));
	//Issues.Add(ReceiveQteEnableEndClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->NetReceiveQTEEnableEnd();
	//	}
	//));
	//Issues.Add(ReceiveControlChatClass<uint32, uint8, int32>::GetInstance().Subscribe2(
	//	[this](uint32 NetId, uint8 ChatType, int32 ChatIndex)
	//	{
	//		this->NetReceiveControlChat(NetId, EControlChatType(ChatType), ChatIndex);
	//	}
	//));
	//Issues.Add(ReceiveSpawnControlNpcClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->NetReceiveSpawnControlNPC();
	//	}
	//));
	//Issues.Add(ReceivedControlEnemyLeaveAllClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->StartLeaveAllDirection();
	//	}
	//));
	//Issues.Add(ReceivedConsumeControlSanctuaryClass<int32>::GetInstance().Subscribe2(
	//	[this](int32 SancIndex)
	//	{
	//		this->NetReceiveConsumeControlSanctuary(SancIndex);
	//	}
	//));
	//Issues.Add(ReceivedRespawnControlSanctuaryClass<int32>::GetInstance().Subscribe2(
	//	[this](int32 SancIndex)
	//	{
	//		this->NetReceiveRespawnControlSanctuary(SancIndex);
	//	}
	//));
	//Issues.Add(ReceivedDrinkControlSanctuaryClass<uint32, int32>::GetInstance().Subscribe2(
	//	[this](uint32 NetId, int32 SancIndex)
	//	{
	//		this->NetReceiveDrinkControlSanctuary(NetId, SancIndex);
	//	}
	//));
	//
	//Issues.Add(PlayerStartSkill01Class<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->TouchPressed();
	//}
	//));
	//Issues.Add(PlayerStartSkill02Class<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->TouchPressed();
	//}
	//));
	//Issues.Add(PlayerStartSkill03Class<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->TouchPressed();
	//}
	//));
	//Issues.Add(PlayerStartWeaponSkillClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->TouchPressed();
	//}
	//));
	//Issues.Add(PlayerStartAttackClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->TouchPressed();
	//}
	//));
	//Issues.Add(PlayerStopAttackClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->TouchPressed();
	//}
	//));
	//Issues.Add(PlayerStartGuardClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->TouchPressed();
	//}
	//));
	//Issues.Add(PlayerStopGuardClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->TouchPressed();
	//}
	//));

	//bEventsSubscribed = true;
}

void AB2ControlGameMode::NotifyCharacterBeginPlay(ABladeIICharacter* TargetCharacter)
{
	Super::NotifyCharacterBeginPlay(TargetCharacter);		
}

void AB2ControlGameMode::NotifyPlayerDeadEnd(ABladeIIPlayer* DeadGuy)
{
	//// 전체 플레이어용(로컬, 봇, 리모트)
	//ABladeIINetPlayer* TargetNetPlayer = Cast<ABladeIINetPlayer>(DeadGuy);
	//if (!bEndMatch && TargetNetPlayer)
	//{
	//	// 죽은 카운트 올림
	//	UB2UIDocHelper::GetDocControl()->AddDeadCount(TargetNetPlayer->GetNetId());

	//	// 플레이어UI에 죽은횟수 적용(8명떠있는곳)
	//	UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//	if (pUI)
	//		pUI->SetPlayerResurrectTime(TargetNetPlayer->GetNetId(), GetControlResurrectTime(TargetNetPlayer->GetNetId()));
	//}

	//// 로컬플레이어용
	//ABladeIIPlayer* LocalPawn = Cast<ABladeIIPlayer>(GetLocalController()->GetPawn());
	//if (DeadGuy == LocalPawn)
	//{
	//	if (!bEndMatch)
	//	{
	//		// 부활 대기UI
	//		UB2UIWaitRevive* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIWaitRevive>(UIFName::WaitRevive);

	//		if (pUI)
	//			pUI->InitTimer(GetControlResurrectTime(GetMyNetId()));

	//		TouchPressed();
	//		SetWaitRevive(true);
	//	}

	//	// 플레이어 UI매니져 제거(컨트롤 게임모드만)
	//	DeadGuy->DestroyPlayerUIManager();		

	//	// 관전 카메라
	//	ActiveDeadWatchingCamera(true);
	//}
}

void AB2ControlGameMode::NotifyCharacterDead(ABladeIICharacter* TargetCharacter, APawn* PawnInstigator, AActor* DamageCauser)
{
//	Super::NotifyCharacterDead(TargetCharacter, PawnInstigator, DamageCauser);
//
////	if (m_pControlObject)
////		m_pControlObject->OnCharacterDead();
//
//	// 킬카운트
//	ABladeIINetPlayer* pPlayer = Cast<ABladeIINetPlayer>(DamageCauser);
//	ABladeIINetPlayer* pDeadPlayer = Cast<ABladeIINetPlayer>(TargetCharacter);
//	ABladeIIControlNetCharacter* pMinotaur = Cast<ABladeIIControlNetCharacter>(DamageCauser);
//
//	// 투사체나 바닥케이스
//	if (!pPlayer && DamageCauser)
//		pPlayer = Cast<ABladeIINetPlayer>(DamageCauser->GetOwner());
//
//	// 탑승한 미노가 죽인케이스
//	if (!pPlayer && pMinotaur && pMinotaur->IsControlledByQTEPlayer())
//	{
//		for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
//		{
//			ABladeIINetPlayer* pIteratorPlayer = Cast<ABladeIINetPlayer>(*Iterator);
//
//			if (pIteratorPlayer && pIteratorPlayer->QTEActor == pMinotaur)
//				pPlayer = pIteratorPlayer;
//		}
//	}
//	
//	// 로컬플레이어가 결속스킬 성소먹고 죽으면 채널내 알림
//	for (TPair<int32, uint32> DrunkInfo : SanctuaryDrunkInfos)
//	{
//		if (pDeadPlayer && pDeadPlayer->IsLocalPlayer() && pDeadPlayer->GetNetId() == DrunkInfo.Value)
//			NetSendConsumeControlSanctuary(DrunkInfo.Key);
//	}
//
//	if (pPlayer && pDeadPlayer)
//	{
//		SetKillInfoToBattleUI(pPlayer, pDeadPlayer);
//	}
//	else
//	{
//		// 자살이거나(정상루트는 없음) 플레이어가 타지않은 점령전 몹한테 죽은 케이스
//		return;
//	}
//
//	// 죽은놈이 로컬플레이어
//	if (pDeadPlayer == Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn()))
//	{
//		UB2UIDocHelper::GetDocControl()->ResetSerialKillCount();
//	}
//
//	// 죽인놈이 로컬플레이어
//	if (pPlayer == Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn()))
//	{
//		UB2UIDocHelper::GetDocControl()->AddSerialKillCount();
//	}
//
//	// 호스트는 킬저장, 알림
//	if (pPlayer && GetPeerType() == PEER_SUPER)
//	{		
//		// UIDoc에 저장
//		UB2UIDocControl* pUIDoc = UB2UIDocHelper::GetDocControl();
//
//		if (pUIDoc)
//		{
//			pUIDoc->AddKillCount(pPlayer->GetNetId());
//
//			NetSendControlKillInfos();			
//		}		
//	}	
}

void AB2ControlGameMode::NotifyPlayerStartSkill(class ABladeIIPlayer* pPlayer, EAttackState SkillAttackState)
{
	// 로컬플레이어가 결속스킬 쓰면 채널내 알림
	if (pPlayer->IsLocalPlayer() && SkillAttackState == EAttackState::ECS_WeaponSkill)
	{
		ABladeIINetPlayer* pNetPlayer = Cast<ABladeIINetPlayer>(pPlayer);
		
		if (pNetPlayer)
		{
			const int32* SancIndexPtr = SanctuaryDrunkInfos.FindKey(pNetPlayer->GetNetId());

			if(SancIndexPtr)
				NetSendConsumeControlSanctuary(*SancIndexPtr);
		}		
	}
}

void AB2ControlGameMode::GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 InMyTeamNum /*= INDEX_NONE*/, bool bOnlyLiveMobs /*= true*/)
{
	// 몹도 추가
	/*GetAllWorldMobList(OutList, bOnlyLiveMobs);

	if (ControlNPC && (ControlNPC->GetQTEEnabled() || InMyTeamNum == ControlNPC->GetTeamNum()))
	{
		OutList.Remove(ControlNPC);
	}

	if (InMyTeamNum != INDEX_NONE)
	{
		for (auto peerInfo : AllPeerInfos)
		{
			ABladeIINetPlayer* pPlayer = Cast<ABladeIINetPlayer>(FindPawnByNetId(peerInfo.Key));
			if (pPlayer && pPlayer->GetTeamNum() != InMyTeamNum)
				OutList.Add(pPlayer);
		}
	}*/
}

void AB2ControlGameMode::NotifyTargetDamagedFromDamagedTarget(class ABladeIICharacter* DamageCauser, class ABladeIICharacter* DamagedTarget, float fDamageAmount)
{
	if (GetLocalController()->GetPawn() == DamageCauser)
	{
		// 누적 데미지는 int32로한다. 별이유 없음. 소수점 표기 안하니깐..
		UB2UIDocHelper::GetDocControl()->AddMyTotalDamage(int32(fDamageAmount));
	}
}

//ABladeIICharacter* AB2ControlGameMode::SpawnCharacter(UB2NPCClassInfoBox* InfoBox, ENPCClass InClass, ENPCClassVariation InVariation, FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters)
//{
//	B2_SCOPED_TRACK_LOG(TEXT("AB2ControlGameMode::SpawnCharacter"));
//
//	BII_CHECK(InfoBox);
//	BII_CHECK(GetPeerType() != PEER_NONE);
//
//	ABladeIICharacter*	NewSpawned = nullptr;
//
//	//// 점령지 위치갖다 써봄
//	//SpawnTransform.SetLocation(m_pControlObject->GetActorLocation());
//
//	//NewSpawned = InfoBox->SpawnNPCClassInfoControlNetCharacter(GetWorld(), InClass, InVariation, SpawnTransform, SpawnParameters);
//	//auto* NetCharacter = Cast<ABladeIINetCharacter>(NewSpawned);
//	//if (NetCharacter)
//	//{
//	//	auto CharacterRole = GetPeerType() == PEER_RESTRICTED ? ABladeIINetCharacter::NET_SLAVE : ABladeIINetCharacter::NET_MASTER;
//
//	//	NetCharacter->SetNetId(0x00010000);
//	//	NetCharacter->SetNetStatus(CharacterRole);
//
//	//	NetCharacter->GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
//	//}
//
//	//if (NewSpawned)
//	//{
//	//	ControlNPC = Cast<ABladeIIControlNetCharacter>(NewSpawned);
//	//	//ControlNPC->SetActorHiddenInGame(true);
//	//}
//
//	return NewSpawned;
//}

ACharacter* AB2ControlGameMode::GetNearestPlayerCharacter(class ACharacter* pCharacter, bool bSkipByCustomLogic)
{
	//float fTestDistance = m_pControlObject ? m_pControlObject->GetAreaRadius() : 1000;
	//float fTestMinDistance = -1.0f;

	//ABladeIIControlNetCharacter* pControlNetChar = Cast<ABladeIIControlNetCharacter>(pCharacter);
	//
	//// pCharacter 본인이 점령전 미노면 특정 범위안에서 플레이어 찾음
	//if (pControlNetChar && m_pControlObject)
	//{
	//	ACharacter* pSelectedTargetCharacter = NULL;

	//	for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//	{
	//		ABladeIINetPlayer* Player = Cast<ABladeIINetPlayer>(*Iterator);

	//		if (Player && Player->IsAlive())
	//		{
	//			// 점령지와의 거리
	//			float fCurTargetDistanceCO = (Player->GetActorLocation() - m_pControlObject->GetActorLocation()).Size();

	//			// NPC와의 거리
	//			float fCurTargetDistanceCN = (Player->GetActorLocation() - pCharacter->GetActorLocation()).Size();

	//			if (fCurTargetDistanceCO < fTestDistance && (fTestMinDistance == -1.0f || fCurTargetDistanceCN < fTestMinDistance))
	//			{
	//				fTestMinDistance = fCurTargetDistanceCN;
	//				pSelectedTargetCharacter = Player;
	//			}
	//		}			
	//	}
	//	return pSelectedTargetCharacter;
	//}
	//else
		return Super::GetNearestPlayerCharacter(pCharacter, bSkipByCustomLogic);
}

void AB2ControlGameMode::NotifyAnimMobDeadEnd(class ABladeIICharacter* TargetCharacter)
{
	//// NPC죽었으면 널
	//if (ControlNPC == TargetCharacter)
	//{
	//	ControlNPC = NULL;

	//	ReserveSpawnControlNPC(false);
	//}
}

void AB2ControlGameMode::NotifyQTEMountBeginToIdle(class ABladeIIPlayer* pPlayer)
{
	Super::NotifyQTEMountBeginToIdle(pPlayer);

	//// UI에 알림
	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI && pPlayer)
	//{
	//	if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Red)
	//		pUI->SetControlNpcUIState(EControlNpcUiState::QTE_Red, ControlNPC->QTEMountTime);

	//	if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Blue)
	//		pUI->SetControlNpcUIState(EControlNpcUiState::QTE_Blue, ControlNPC->QTEMountTime);
	//}

	//// 요기서 플레이어 HP바 숨기구. 미노타우르스 남은시간 게이지 표기.
	//GetUIManager_InGameCombat()->HideFloatingHPBarForPlayer(pPlayer, true);

	//CreateControlNpcRemainTimeFloatingGageBar(ControlNPC, ControlNPC->QTEMountTime);
}

void AB2ControlGameMode::HandleRequestMatchMaking()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2ControlGameMode::HandleRequestMatchMaking"));

	SceneManager.SetGameMode(this);
	
	SetMatchState(NetMatchState::LobbyMatchToGameMode);
}

void AB2ControlGameMode::HandleMatchHasStarted()
{	
	Super::HandleMatchHasStarted();

	//TryAsyncLoadSkillAnims(&BladeIIGameImpl::GetLocalCharacterData());

	//for (auto& RemotePlayerData : RemotePlayerInfoData)
	//	TryAsyncLoadSkillAnims(&RemotePlayerData.Value);
}

void AB2ControlGameMode::HandleLobbyMatchToGameMode()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2ControlGameMode::HandleLobbyMatchToGameMode"));

	if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	{
		SetPeerType(DocLobbyMatch->GetPeerType());
		SetRoomId(DocLobbyMatch->GetRoomId());
		SetHostId(DocLobbyMatch->GetHostId());
		SetToken(DocLobbyMatch->GetRoomToken());

		// 자기자신 셋팅
		SetMyPlayerInfoData(DocLobbyMatch->GetMyNetId(), FB2FindAccountInfoPtr());
		AddPeerInfo(DocLobbyMatch->GetMyNetId(), DocLobbyMatch->GetMyStartIndex());

		// 유저셋팅
		for (int32 i = 0; i < DocLobbyMatch->GetRemotePlayerInfoDataCount(); ++i)
		{
			int32 NetId = DocLobbyMatch->GetRemoteNetIdByIndex(i);
			EPCClass ReservedPCClass = DocLobbyMatch->GetReservedRemotePCClassByIndex(i);
			
			SetRemotePlayerInfoData(NetId, DocLobbyMatch->GetRemoteAccountIdByIndex(i), DocLobbyMatch->GetRemoteAccountInfoByIndex(i));

			// 메인케릭터 클래스 셋팅
			if (RemotePlayerInfoData.Contains(NetId) && ReservedPCClass != EPCClass::EPC_End)
			{
				RemotePlayerInfoData[NetId].SetMainPlayerClass(ReservedPCClass);
			}

			AddPeerInfo(NetId, DocLobbyMatch->GetRemotePlayerStartIndexByIndex(i));
		}
	}

	SetMatchState(NetMatchState::WaitToMeetCondition);
}

void AB2ControlGameMode::SetResurrectPlayerPosition(ABladeIIPlayer* pBladePlayer)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2ControlGameMode::SetResurrectPlayer"));

	auto* PlayerController = GetLocalController();
	auto* pPlayer = Cast<ABladeIINetPlayer>(pBladePlayer);

	if (!pPlayer)
	{
		// 부활 위치 : 스폰위치		
		if (PlayerController == nullptr || PlayerController->GetPawn() == nullptr)
			return;

		pPlayer = Cast<ABladeIINetPlayer>(PlayerController->GetPawn());
	}

	if (!pPlayer)
		return;

	uint32 PlayerNetId = pPlayer->GetNetId();
	
	FString TagName = GetPlayerStartTagName(AllPeerInfos[PlayerNetId]);
	AActor* StartSpot = FindPlayerStart(PlayerController, TagName);

	FRotator StartRotation(ForceInit);
	StartRotation.Yaw = StartSpot->GetActorRotation().Yaw;
	FVector StartLocation = StartSpot->GetActorLocation();
	pPlayer->SetActorLocationAndRotation(StartLocation, StartRotation);	
}

void AB2ControlGameMode::SetPlayerCollisionType(ABladeIINetPlayer* pPlayer)
{
	// <-- 팀별 콜리전 타입
	if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Red)
		pPlayer->GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Blue)
		pPlayer->GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
	// --> 팀별 콜리전 타입
}
void AB2ControlGameMode::OnHostChanged(int32 RoomType, int32 NewHostId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2ControlGameMode::OnHostChanged"));

	//// 이전 호스트가 성소마시구 나간거면 알림(내가 새호스트일때만)
	//if (GetMyNetId() == NewHostId)
	//{
	//	const int32* SancIndexPtr = SanctuaryDrunkInfos.FindKey(GetHostId());

	//	if (SancIndexPtr)
	//		NetSendConsumeControlSanctuary(*SancIndexPtr);
	//}

	//// 로컬플레이어 QTE요청기록 제거
	//AB2ControlModePlayer* pLocalPlayer = Cast<AB2ControlModePlayer>(FindPawnByNetId(GetMyNetId()));

	//if (pLocalPlayer)
	//	pLocalPlayer->ResetRequestStartQTEFlag();

	//Super::OnHostChanged(RoomType, NewHostId);

	//if (GetMyNetId() != NewHostId)
	//	return;

	//if (MatchState != MatchState::InProgress)
	//	return;

	//// 게임종료시 나간플레이어 처리 안함
	//if (bEndMatch)
	//	return;

	//// 호스트 바뀌면 체크한번해준다.
	//CheckControlBattleStart();

	//for (auto NetId : AIControlleredPeerIds)
	//{
	//	AB2ControlModePlayer* Player = Cast<AB2ControlModePlayer>(FindPawnByNetId(NetId));

	//	BII_CHECK(Player);

	//	Player->PromoteNetStatus();

	//	if (AIControlleredPeerIds[AIControlleredPeerIds.Num() - 1] == NetId)
	//	{
	//		SendLeavePlayer(NetId, Player->GetAccountId());
	//	}
	//}

	//if (ControlNPC)
	//	ControlNPC->PromoteNetStatus();
	//else
	//{
	//	// 타이머가 적용되었는데 미노가 없다? 그럼 스폰체크
	//	// 내타이머는 이미 종료, 기존 호스트는 타이머 다돌았을때쯤 나간케이스일듯
	//	if (m_bApplyedNpcSpawnTimer)
	//		NetSendSpawnControlNPC();
	//}

	CheckLeaveAll();
}

void AB2ControlGameMode::HandleNotifyLeave(uint32 NetId)
{
	AIControlleredPeerIds.AddUnique(NetId);

	//// 스폰 안보내구 떠났으면 스폰시켜준다.
	//// 호스트가 나갔을경우. 호스트체인지보다 이게 먼저와야함.
	//AB2ControlModePlayer* Player = Cast<AB2ControlModePlayer>(FindPawnByNetId(NetId));

	//if (!Player)
	//{
	//	PrepareRemotePlayer(NetId);
	//	Player = Cast<AB2ControlModePlayer>(FindPawnByNetId(NetId));
	//}

	//if (bEndMatch || GetPeerType() == PEER_RESTRICTED)
	//	return;
	//
	//BII_CHECK(Player);

	//Player->PromoteNetStatus();

	//// 떠난놈이 마지막으로 성소마신놈이면 알림.(호스트 체크)
	//
	//const int32* SancIndexPtr = SanctuaryDrunkInfos.FindKey(NetId);

	//if (SancIndexPtr)
	//	NetSendConsumeControlSanctuary(SanctuaryDrunkInfos[*SancIndexPtr]);

	//// 점령전 튜토리얼에선 추방이나, 타유저 채널나감 처리안한다.
	//if (IsActiveControlTutorial())
	//	return;

	//SendLeavePlayer(NetId, Player->GetAccountId());
	//CheckLeaveAll();
}

void AB2ControlGameMode::CheckNetAIPlayerResurrect(class ABladeIINetPlayer* pPlayer)
{
	if (bEndMatch)
		return;
	
	if (pPlayer->GetNetStatus() == ABladeIINetPlayer::NET_AIMASTER && pPlayer->GetDeadTimeSec() != -1 && GetWorld()->GetTimeSeconds() - pPlayer->GetDeadTimeSec() > GetControlResurrectTime(pPlayer->GetNetId()))
	{
		//부활!
		ForceResurrectAIPlayer(pPlayer);

		pPlayer->SetDeadTimeSec(-1);
	}	
}

bool AB2ControlGameMode::CanTakeDamage()
{
	return bStartMatch && !bEndMatch;
}

bool AB2ControlGameMode::IsAllowCameraWorking()
{
	return !bEndMatch;
}

void AB2ControlGameMode::HandleWaitToMeetCondition()
{
	SceneManager.SetState(EControlMatchSceneState::Intro);

	SetMatchState(MatchState::InProgress);
}

void AB2ControlGameMode::CheckControlBattleStart()
{
	if (GetPeerType() != PEER_SUPER || SceneManager.GetState() != EControlMatchSceneState::Ready)
		return;

	int32 nPlayerCount = 0;

	for (TActorIterator<APawn> It(GetWorld()); It; ++It)
	{
		 ABladeIINetPlayer* Player = Cast<ABladeIINetPlayer>(*It);

		if (Player)
			nPlayerCount++;
	}

	if (nPlayerCount >= AllPeerInfos.Num() && !bStartMatch)
	{
		SendStartGamePacket();
		bStartMatch = true;
		return;
	}

	// 타이머 돌린다. 시간다되면 안들어온피어 AI로 스폰하고 시작한다.
	GetWorld()->GetTimerManager().SetTimer(ForceStartTimeHandle, this, &AB2ControlGameMode::ControlForceStart, ForceStartTime, false);
}

void AB2ControlGameMode::CheckKicked()
{
	//if (bIsKicked)
	//{
	//	// 로딩이 길어서 추방당했다 ㅜㅜ 팝업 띄워주자.
	//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Control_Ban")),
	//		0.f,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::Confirm,
	//		FMsgPopupOnClick::CreateLambda([this]() {			
	//			// 로비로.
	//			ReturnToControlMainMenu();
	//			})
	//		);
	//}

	bKickChecked = true;
}

void AB2ControlGameMode::SetReady()
{
	//UB2UIManager::GetInstance()->OpenUI(UIFName::ControlReadyToBattle);

	NetSendSpawnComplete();

	CheckControlBattleStart();

	CheckKicked();
}

void AB2ControlGameMode::ForceResurrectLocalPlayer()
{
	// 겜끝났으면 부활하지 말자.
	if (bEndMatch)
		return;

	Super::ForceResurrectLocalPlayer();
	
	ActiveDeadWatchingCamera(false);
}

void AB2ControlGameMode::PrepareHostPlayer()
{
	auto* PlayerController = GetLocalController();
	if (PlayerController->GetPawn() != NULL)
		return;

	BII_CHECK(AllPeerInfos.Contains(GetMyNetId()));

	FString TagName = GetPlayerStartTagName(AllPeerInfos[GetMyNetId()]);
		 
	RestartNetPlayer(PlayerController, TagName);

	auto MyPawn = Cast<ABladeIINetPlayer>(PlayerController->GetPawn());
	BII_CHECK(MyPawn)

	MyPawn->SetNetId(GetMyNetId());
	MyPawn->SetTagEnable(false);
	SetLocalController(PlayerController);

	MyPawn->SetTeamNum(int32(GetControlTeam(AllPeerInfos[GetMyNetId()].PlayerStartIdx)));
	
	SetPlayerCollisionType(MyPawn);
	
	auto  Asset = GetGameRule()->GetBehaviorTreeAsset();
	if (!Asset.IsEmpty())
		MyPawn->BehaviorTree = LoadObject<UBehaviorTree>(NULL, *Asset);

	// 로컬플레이어용 화살표 UI
	CreateControlPlayerArrow(MyPawn);
}

void AB2ControlGameMode::PrepareRemotePlayer(uint32 NetId)
{
	UE_LOG(LogControlGameMode, Log, TEXT("Called PrepareRemotePlayer Function. NetId : %d"), NetId);

	BII_CHECK(AllPeerInfos.Contains(NetId));

	if (FindPawnByNetId(NetId) != NULL)
		return;

	FString TagName = GetPlayerStartTagName(AllPeerInfos[NetId]);

	AActor* StartSpot = FindPlayerStart(NULL, TagName);
	BII_CHECK(StartSpot);

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = nullptr;
	SpawnParam.bDeferConstruction = true;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FTransform SpawnTransform = StartSpot->GetTransform();
	ABladeIINetPlayer* RemotePlayer = Cast<ABladeIINetPlayer>(AB2NetGameMode::SpawnRemotePlayer(GetWorld(), RemotePlayerInfoData[NetId].GetMainPlayerClass(), SpawnTransform, SpawnParam));
	BII_CHECK(RemotePlayer);

	RemotePlayer->SetTeamNum(int32(GetControlTeam(AllPeerInfos[NetId].PlayerStartIdx)));

	RemotePlayer->CurrentPlayingClass = RemotePlayer->MainPlayerClass = RemotePlayerInfoData[NetId].GetMainPlayerClass();
	RemotePlayer->SetNetStatus(ABladeIINetPlayer::NET_SLAVE);
	RemotePlayer->SetNetId(NetId);
	RemotePlayer->SetTagEnable(false);
	RemotePlayer->DisableComponentsSimulatePhysics();
	RemotePlayer->SetActorHiddenInGame(true);
	RemotePlayer->FinishSpawning(SpawnTransform);
		
	SetPlayerCollisionType(RemotePlayer);

	RestartRemotePlayer(NetId);
	
	// 인프로그래스에서 여기 들어왔으면 난입유저이니 UI띄워줌
	if (MatchState == MatchState::InProgress)
	{
		ViewPlayerJoinUI(RemotePlayer);
	}

	SetPlayerToBattleUI(RemotePlayer);

	CheckControlBattleStart();
}

void AB2ControlGameMode::RestartRemotePlayer(uint32 NetId)
{
	if (AllPeerInfos.Contains(NetId) == false)
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("RestartRemotePlayer - AllPeerInfos Not Exist (NetID).")));
#endif
		return;
	}

	BII_CHECK(AllPeerInfos[NetId].PlayerStartIdx != 0);

	auto*	OtherPlayer = FindPawnByNetId(NetId);
	auto	TagName = GetPlayerStartTagName(AllPeerInfos[NetId]);
	auto*	start = MyFindPlayerStart(TagName);
	BII_CHECK(start);
	
	if (start)
	{
		FRotator StartRotation(ForceInit);
		StartRotation.Yaw = start->GetActorRotation().Yaw;
		FVector StartLocation = start->GetActorLocation();

		OtherPlayer->TeleportTo(StartLocation, StartRotation);
	}

	OtherPlayer->SetActorHiddenInGame(false);
}

void AB2ControlGameMode::AddPeerInfo(uint32 NetId, uint32 StartIndex)
{
	if(!AllPeerInfos.Contains(NetId))
		AllPeerInfos.Emplace(NetId, FControlPeerInfo());

	AllPeerInfos[NetId].PlayerStartIdx = StartIndex;
}

//void AB2ControlGameMode::SetControlObject(class ABladeIINetControlObject* InControlObject)
//{
//	//m_pControlObject = InControlObject;
//}

FVector AB2ControlGameMode::GetControlObjectLocation()
{
	//if (m_pControlObject)
	//	return m_pControlObject->GetActorLocation();

	return FVector::ZeroVector;
}

float AB2ControlGameMode::GetControlObjectViewLimitLength()
{
	//if (m_pControlObject)
	//	return m_pControlObject->GetViewLimitLength();

	return 0.0f;
}

//void AB2ControlGameMode::AddSanctuaryControlGame(class AB2SanctuaryControlGame* InSanctuary)
//{
//	//if (InSanctuary)
//	//{
//	//	m_arSanctuary.Add(InSanctuary);
//	//	
//	//	ControlSanctuaryRespawnRemainTimeInfos.Add(InSanctuary->NetIndex, -1.0f);
//	//}
//}

void AB2ControlGameMode::SendChangeControlState(uint8 nNewState, float fControlPointGageRed, float fControlPointGageBlue, bool bResetConquestGage)
{
	//if (GetPeerType() == PEER_SUPER)
	//{
	//	packet::ByteStream	payload;
	//	payload << nNewState;
	//	payload << fControlPointGageRed;
	//	payload << fControlPointGageBlue;
	//	payload << bResetConquestGage;
	//	FString encoded_string = packet::FinalizePacket(packet::CONTROL_STATE, GetMyNetId(), 0, packet::ALL, payload);
	//	data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);
	//}
}

void AB2ControlGameMode::AckChangeControlState(uint8 nNewState, float fControlPointGageRed, float fControlPointGageBlue, bool bResetConquestGage)
{
	//if (m_pControlObject)
	//{
	//	m_pControlObject->ApplyChangeState(nNewState, fControlPointGageRed, fControlPointGageBlue, bResetConquestGage);
	//}
}

void AB2ControlGameMode::SendChangeConquestAreaState(uint8 nNewState, float fConquestGageRed, float fConquestGageBlue)
{
	//if (GetPeerType() == PEER_SUPER)
	//{
	//	packet::ByteStream	payload;
	//	payload << nNewState;
	//	payload << fConquestGageRed;
	//	payload << fConquestGageBlue;
	//	FString encoded_string = packet::FinalizePacket(packet::CONQUEST_AREA_STATE, GetMyNetId(), 0, packet::ALL, payload);
	//	data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);
	//}
}

void AB2ControlGameMode::AckChangeConquestAreaState(uint8 nNewState, float fConquestGageRed, float fConquestGageBlue)
{
	//if (m_pControlObject)
	//{
	//	m_pControlObject->ApplyConquestAreaState(nNewState, fConquestGageRed, fConquestGageBlue);
	//}
}

void AB2ControlGameMode::AckControlStartGame()
{
	//// 게임시작
	////SendGamePlayInfo();

	//// 준비UI에 전달
	//UB2UIControlReadyToBattle* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlReadyToBattle>(UIFName::ControlReadyToBattle);

	//if (pUI)
	//{
	//	pUI->PlayAnimationEvent_ReadyToStartGame();
	//}

	//bStartMatch = true;

	//// 3 2 1 파이트 뜰때 없애달라함. 안에서 타이머 건다.
	//DestroyControlPlayerArrow();
}

void AB2ControlGameMode::SendLeavePlayer(uint32 NetId, int64 AccountId)
{
	//// 점령전 튜토리얼에선 추방이나, 타유저 채널나감 처리안한다.
	//if (IsActiveControlTutorial())
	//	return;

	//if (GetPeerType() == PEER_SUPER)
	//{
	//	if (auto* DocLobbyMatch = UB2UIDocHelper::GetDocLobbyMatch())
	//	{
	//		const TMap<uint32, FString>& Tokens = DocLobbyMatch->GetMatchToken();

	//		if (Tokens.Contains(NetId))
	//		{
	//			data_trader::Retailer::GetInstance().RequestRoomLeaveResult(GetRoomId(), AccountId, Tokens[NetId]);
	//		}
	//	}
	//}
}

void AB2ControlGameMode::SendEndMatch(ETeamType WinTeam)
{
	//if (bSendEndMatch)
	//	return;

	//bSendEndMatch = true;
	//if (GetPeerType() == PEER_SUPER)
	//{
	//	packet::ByteStream	payload;
	//	payload << uint8(WinTeam);
	//	FString encoded_string = packet::FinalizePacket(packet::CONTROL_RESULT, GetMyNetId(), 0, packet::ALL, payload);
	//	data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);
	//}
}

void AB2ControlGameMode::AckEndMatch(ETeamType WinTeam)
{
	//ResultWinTeam = WinTeam;
	//bEndMatch = true;

	//UB2UIManager::GetInstance()->CloseUI(UIFName::WaitRevive);

	//m_pControlObject->SetActiveControlObject(false);

	//// 점령지 이펙트 켜줌
	//if (WinTeam == ETeamType::ETT_Red && WinFxEmittersRed)
	//	WinFxEmittersRed->GetParticleSystemComponent()->SetActive(true, true);;

	//if (WinTeam == ETeamType::ETT_Blue && WinFxEmittersBlue)
	//	WinFxEmittersBlue->GetParticleSystemComponent()->SetActive(true, true);;

	//StopAIControllers();

	//if (ControlNPC)
	//{
	//	// NPC 스탑	
	//	ABladeIIAIController* pAC = Cast<ABladeIIAIController>(ControlNPC->GetController());

	//	if (pAC)
	//		pAC->StopAutoPlay();
	//}
	//else
	//{
	//	GetWorldTimerManager().ClearTimer(NpcSpawnTimeHandle);
	//}


	//GetWorld()->GetTimerManager().SetTimer(ResultActionTimeHandle, this, &AB2ControlGameMode::SetMvpScene, ResultActionTime, false);

	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI)
	//{
	//	pUI->PlayResultAction(WinTeam, ETeamType(GetMyTeamNum()));
	//}

	//auto* PlayerController = GetLocalController();
	//auto MyPawn = Cast<ABladeIINetPlayer>(PlayerController->GetPawn());
	//MyPawn->DisableInput(PlayerController);

	//ABladeIIPlayerController* pPC = Cast<ABladeIIPlayerController>(PlayerController);
	//if (pPC)
	//{
	//	pPC->SetStageEndNonCombatState(true);
	//}
}

// 모든 유저갖고오기
TArray<class ABladeIINetPlayer*> AB2ControlGameMode::GetPlayerArray()
{
	TArray<class ABladeIINetPlayer*> arPlayers;

	for (auto peerInfo : AllPeerInfos)
	{
		ABladeIINetPlayer* pTargetPlayer = Cast<ABladeIINetPlayer>(FindPawnByNetId(peerInfo.Key));
		arPlayers.Add(pTargetPlayer);
	}

	return arPlayers;
}

// 우리편 유저갖고오기
TArray<class ABladeIINetPlayer*> AB2ControlGameMode::GetAllyPlayerArray()
{
	return GetTeamPlayerArray(ETeamType(GetMyTeamNum()));
}

TArray<class ABladeIINetPlayer*> AB2ControlGameMode::GetTeamPlayerArray(ETeamType TeamType)
{
	TArray<class ABladeIINetPlayer*> arPlayers;

	for (auto peerInfo : AllPeerInfos)
	{
		ABladeIINetPlayer* pTargetPlayer = Cast<ABladeIINetPlayer>(FindPawnByNetId(peerInfo.Key));

		if (pTargetPlayer && pTargetPlayer->GetTeamNum() == int32(TeamType))
			arPlayers.Add(pTargetPlayer);
	}

	return arPlayers;
}

void AB2ControlGameMode::SetControlKillInfos(TMap<uint32, int32>* ControlKillInfos)
{
	UB2UIDocControl* pUIDoc = UB2UIDocHelper::GetDocControl();

	//UE_LOG(LogBladeII, Log, TEXT("Receive [CONTROL_KILLCOUNT]"));

	if (pUIDoc)
	{
		for (TPair<uint32, int32>& KillInfo : *ControlKillInfos)
		{
			pUIDoc->SetKillCount(KillInfo.Key, KillInfo.Value);
			pUIDoc->RefreshKillInfoUI();
		}
	}
}

void AB2ControlGameMode::NetSendControlKillInfos()
{
	// UIDoc에 저장
	UB2UIDocControl* pUIDoc = UB2UIDocHelper::GetDocControl();

	if (pUIDoc)
	{
		TMap<uint32, int32> pUserKillInfos = pUIDoc->GetUserKillInfos();
		int32 nCnt = pUserKillInfos.Num();

		packet::ByteStream	payload;
		payload << nCnt;
		for (TPair<uint32, int32>& UserKillInfo : pUserKillInfos)
		{
			payload << UserKillInfo.Key << UserKillInfo.Value;
		}

		// 모두에게 알림. 나도 내가보낸걸로 세팅!
		FString encoded_string = packet::FinalizePacket(packet::CONTROL_KILLCOUNT, GetMyNetId(), 0, packet::ALL, payload);
		SendMessage2(encoded_string);
		//UE_LOG(LogBladeII, Log, TEXT("Send [CONTROL_KILLCOUNT]"));
	}
}

void AB2ControlGameMode::NetSendControlTimerInfos()
{
	if (SceneManager.GetState() != EControlMatchSceneState::WaitActive && SceneManager.GetState() != EControlMatchSceneState::InGame)
		return;
	
	uint8 SceneState = uint8(SceneManager.GetState());
	float CurrentElapseTime = SceneManager.GetIngamePlayStateElapseTime();

	packet::ByteStream	payload;
	
	payload << SceneState << CurrentElapseTime;

	FString encoded_string = packet::FinalizePacket(packet::CONTROL_TIMER, GetMyNetId(), 0, packet::ALLBUTME, payload);
	SendMessage2(encoded_string);
	//UE_LOG(LogBladeII, Log, TEXT("Send [CONTROL_TIMER]"));
}

void AB2ControlGameMode::AckControlTimerInfos(uint8 SceneState, float ElapseTime)
{
	SceneManager.SetState(EControlMatchSceneState(SceneState), true);
		
	SceneManager.SetControlGamePlayStateTime(GetWorld()->GetTimeSeconds() - ElapseTime);
}

void AB2ControlGameMode::NetSendAskSpawnComplete()
{
	// 다른 플레이어한테 물어봄 너 스폰했니?
	packet::ByteStream	payload;

	FString encoded_string = packet::FinalizePacket(packet::ASK_SPAWN_COMPLETE, GetMyNetId(), 0, packet::ALLBUTME, payload);
	SendMessage2(encoded_string);
	//UE_LOG(LogBladeII, Log, TEXT("Send [ASK_SPAWN_COMPLETE]"));
}

void AB2ControlGameMode::NetReceiveAskSpawnComplete()
{
	// 조건없이 보내봄..
	NetSendSpawnComplete();
}

void AB2ControlGameMode::NetSendSpawnComplete()
{
	// 스폰 완로되었다 알림. 다른 플레이어가 물어보면 또알려줌.
	packet::ByteStream	payload;
	
	// 강제 스폰인가.
	payload << false;

	FString encoded_string = packet::FinalizePacket(packet::SPAWN_COMPLETE, GetMyNetId(), 0, packet::ALLBUTME, payload);
	SendMessage2(encoded_string);

	//UE_LOG(LogBladeII, Log, TEXT("Send [SPAWN_COMPLETE]"));

	// 현재 케릭터 애니메이션 정보도 한번 보내줘야겠다.
	ABladeIINetPlayer* pPlayer = Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn());
	if (pPlayer)
	{
		pPlayer->SendAnimStateIndex();
		UE_LOG(LogTemp, Log, TEXT("AB2ControlGameMode::NetSendSpawnComplete,"));
	}
}

void AB2ControlGameMode::NetReceiveSpawnComplete(uint32 NetId, bool bForce)
{
	APawn* pPawn = FindPawnByNetId(NetId);

	// 이미 있으면 리턴
	if (pPawn)
	{
		return;
	}

	PrepareRemotePlayer(NetId);

	if (bForce)
	{
		if (NetId == GetMyNetId())
		{
			// Ai스폰 인데 그게 나이면 로비로 보내기 플래그 킨다.
			// Ready씬 쯤에서 팝업 띄워줄거
			// 추방당한거임
			bIsKicked = true;

			// 인트로 이후 준비단계에서 체크하는데 이단계가 이미 지났으면 또 체크해줌
			if (bKickChecked)
				CheckKicked();

			return;
		}
		// 떠남 로직을 따른다.
		HandleNotifyLeave(NetId);
	}
}

void AB2ControlGameMode::NetSendSpawnCompleteForceAI(uint32 NetId)
{
	packet::ByteStream	payload;
	
	// 강제 스폰인가.
	payload << true;

	FString encoded_string = packet::FinalizePacket(packet::SPAWN_COMPLETE, NetId, 0, packet::ALL, payload);
	SendMessage2(encoded_string);

	//UE_LOG(LogBladeII, Log, TEXT("Send [SPAWN_COMPLETE_AI]"));
}

void AB2ControlGameMode::NetReceiveQTEEnable(uint32 NetId, uint8 TeamNum)
{
	/*if (ControlNPC && ControlNPC->GetNetId() == NetId)
	{
		ControlNPC->ApplyQTEEnable(ETeamType(TeamNum));
	}*/
}

void AB2ControlGameMode::NetSendQTEEnableEnd()
{
	packet::ByteStream	payload;

	FString encoded_string = packet::FinalizePacket(packet::QTE_ENABLE_END, GetMyNetId(), 0, packet::ALL, payload);
	SendMessage2(encoded_string);
}

void AB2ControlGameMode::NetReceiveQTEEnableEnd()
{
	//if (ControlNPC)
	//	ControlNPC->OnNetReceiveQTEEnableEnd();
}

// 성소 소모됨(결속스킬을 쓰거나, 쓰지않고 죽거나, 쓰지않고 나가거나)
void AB2ControlGameMode::NetSendConsumeControlSanctuary(int32 SancIndex)
{
	packet::ByteStream	payload;

	payload << SancIndex;

	FString encoded_string = packet::FinalizePacket(packet::CONSUME_CONTROL_SANCTUARY, 0, 0, packet::ALL, payload);
	SendMessage2(encoded_string);
}

void AB2ControlGameMode::NetReceiveConsumeControlSanctuary(int32 SancIndex)
{
	// 유저 채널떠남처리, 호스트 변경처리 과정에서 꼬이면 중복으로 날라올수도 있을거같다.
	//check(SanctuaryDrunkInfos.Contains(SancIndex));
	if (!SanctuaryDrunkInfos.Contains(SancIndex))
		return;

	SanctuaryDrunkInfos.Remove(SancIndex);

	ReserveRespawnControlSanctuary(SancIndex);
}

// 성소 젠됨
void AB2ControlGameMode::NetSendRespawnControlSanctuary(int32 SancIndex)
{
	if (GetPeerType() != PEER_SUPER)
		return;

	packet::ByteStream	payload;

	payload << SancIndex;

	FString encoded_string = packet::FinalizePacket(packet::SPAWN_CONTROL_SANCTUARY, 0, 0, packet::ALL, payload);
	SendMessage2(encoded_string);
}

void AB2ControlGameMode::NetReceiveRespawnControlSanctuary(int32 SancIndex)
{
	// 혹시 시간 남아있을까봐
	//ControlSanctuaryRespawnRemainTimeInfos[SancIndex] = -1.0f;

	//// 성소 채우기
	//for (AB2SanctuaryControlGame* pSanc : m_arSanctuary)
	//{
	//	if (pSanc && pSanc->NetIndex == SancIndex)
	//		pSanc->RecycleInteractiveActor();
	//}

	//// UI노티스 띄움
	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI)
	//{
	//	pUI->OnRespawnControlSanctuary();
	//}

}

// 성소 먹음
void AB2ControlGameMode::NetSendDrinkControlSanctuary(uint32 NetId, int32 SancIndex)
{
	if (GetPeerType() != PEER_SUPER)
		return;

	packet::ByteStream	payload;

	payload << SancIndex;

	FString encoded_string = packet::FinalizePacket(packet::DRINK_CONTROL_SANCTUARY, NetId, 0, packet::ALL, payload);
	SendMessage2(encoded_string);
}

void AB2ControlGameMode::NetReceiveDrinkControlSanctuary(uint32 NetId, int32 SancIndex)
{
	// 성소 마시기
	//AActor* pTarget = FindPawnByNetId(NetId);

	//if (!pTarget)
	//	return;

	//SanctuaryDrunkInfos.Add(SancIndex, NetId);

	//float fHealPercent = 0.0f;;
	//
	//for (AB2SanctuaryControlGame* pSanc : m_arSanctuary)
	//{
	//	if (pSanc && pSanc->NetIndex == SancIndex)
	//	{
	//		pSanc->ReceiveInteract(pTarget);
	//		fHealPercent = pSanc->TotalHealPercentage;
	//	}
	//}	

	//ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(pTarget);

	//// 마심처리 or 소모처리는 로컬플레이어만 체크
	//if (pPlayer && pPlayer->IsLocalPlayer())
	//{
	//	// 스킬 안배웠으면 소모처리
	//	if(pPlayer->GetCharacterDataStore()->GetUnitySkillId(pPlayer->GetCurrentPlayerClass()) == SKILL_INVALID_ID)
	//		NetSendConsumeControlSanctuary(SancIndex);
	//	else
	//	{
	//		// 먹고 또먹으면 소모처리
	//		if (pPlayer->IsWeaponSkillPointMax())
	//			NetSendConsumeControlSanctuary(SancIndex);
	//		else
	//			pPlayer->SetWeaponSkillPointMax();
	//	}

	//	// 체력회복
	//	pPlayer->HealingHPByPercent(fHealPercent);
	//}	
}

int32 AB2ControlGameMode::GetMyTeamNum()
{
	ABladeIINetPlayer* pLocalPawn = Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn());

	if (pLocalPawn)
	{
		return pLocalPawn->GetTeamNum();
	}

	return 0;
}

// UI에서 전달받음
void AB2ControlGameMode::FinishReadyToStartGame()
{
}
//
//void AB2ControlGameMode::SetMatchLobbyResources(class AMatineeActor* InResultMatinee,
//	class AB2StageEventDirector* InIntroEventDirector,
//	const FControlMatchAnims& Gladiator,
//	const FControlMatchAnims& Assassin,
//	const FControlMatchAnims& Wizard,
//	const FControlMatchAnims& Fighter,
//	TArray<AEmitter*> EmitterRed,
//	TArray<AEmitter*> EmitterBlue,
//	AEmitter* WinEmitterRed,
//	AEmitter* WinEmitterBlue)
//{
//	SceneManager.SetMatchLobbyResources(InResultMatinee, InIntroEventDirector, Gladiator, Assassin, Wizard, Fighter);
//
//	ConquestAreaFxEmittersRed = EmitterRed;
//	ConquestAreaFxEmittersBlue = EmitterBlue;
//	WinFxEmittersRed = WinEmitterRed;
//	WinFxEmittersBlue = WinEmitterBlue;
//}

//void AB2ControlGameMode::SetTutorialCamera(class AB2ActiveCameraActor* pCenterCamActor, class AB2ActiveCameraActor* pSanctuaryBottomCamActor, class AB2ActiveCameraActor* pSanctuaryTopCamActor)
//{
//	//TutorialCameraCenter = pCenterCamActor;
//	//TutorialCameraSanctuaryBottom = pSanctuaryBottomCamActor;
//	//TutorialCameraSanctuaryTopCamera = pSanctuaryTopCamActor;
//}

//void AB2ControlGameMode::SetDeadWatchingCamera(class AB2ActiveCameraActor* pCamActor)
//{
//	//DeadWatchingCamera = pCamActor;
//}

void AB2ControlGameMode::MatchResultMatineeFinished()
{
	SceneManager.MatchResultMatineeFinished();
}

void AB2ControlGameMode::ProcessTutorialAction(EControlTutorialActionStep ActionStep)
{
	//switch (ActionStep)
	//{
	//case EControlTutorialActionStep::ViewCenterCamera:
	//	ActiveTutorialCenterCamera(true);
	//	break;
	//case EControlTutorialActionStep::ViewSanctuaryBottomCamera:
	//	ActiveTutorialSanctuaryCameraBottom(true);
	//	break;
	//case EControlTutorialActionStep::ViewSanctuaryTopCamera:
	//	ActiveTutorialSanctuaryCameraTop(true);
	//	break;
	//case EControlTutorialActionStep::ViewIngameCamera:
	//	CustomEndCurrentActiveActiveCameraActor();

	//	if (TutorialCameraSanctuaryTopCamera)
	//	{
	//		GetWorld()->GetTimerManager().SetTimer(TutorialWaitHandle, this, &AB2ControlGameMode::GotoTutorialNextStep, TutorialCameraSanctuaryTopCamera->GetBlendOutTime(), false);
	//	}
	//	break;
	//case EControlTutorialActionStep::SetIngame:
	//	
	//	// 튜토리얼땜에 멈췄던 스탭 진행한다.
	//	SetReady();

	//	// 강제시작 타이머 취소하구 함수호출.
	//	GetWorld()->GetTimerManager().ClearTimer(ForceStartTimeHandle);
	//	ControlForceStart();

	//	break;
	//default:
	//	break;
	//}
}

int32 AB2ControlGameMode::GetStartPlayerNum()
{
//#if UE_BUILD_SHIPPING
//	return AB2ControlGameMode::START_PLAYERS;
//#else
//	if (NetFakeConfigure::GetInstance().IsNumToPlayRaid())
//		return NetFakeConfigure::GetInstance().GetNumToPlayRaid();
//
	return AB2ControlGameMode::START_PLAYERS;
//#endif
}

int32 AB2ControlGameMode::GetMatchPlayerNum()
{
	return AllPeerInfos.Num();
}

int32 AB2ControlGameMode::GetMaxMatchPlayerNum()
{
//#if UE_BUILD_SHIPPING
//	return CONTROL_GAME_MAX_PLAYERS;
//#else
//	if (NetFakeConfigure::GetInstance().IsNumToPlayRaid())
		//return NetFakeConfigure::GetInstance().GetNumToPlayRaid();
//
	return CONTROL_GAME_MAX_PLAYERS;
//#endif
}

bool AB2ControlGameMode::IsMaxMatchPlayerNum()
{
	return GetMatchPlayerNum() == GetMaxMatchPlayerNum();
}

void AB2ControlGameMode::CheckLeaveAll()
{
	if (bEndMatch || bSendEndMatch || GetPeerType() != PEER_SUPER)
		return;

	TMap<ETeamType, int32> LeavePlayerCount;

	// 나간 플레이어 체크
	for (auto& PeerId : AIControlleredPeerIds)
	{
		if (AllPeerInfos.Contains(PeerId))
		{
			ETeamType TeamType = GetControlTeam(AllPeerInfos[PeerId].PlayerStartIdx);

			if (!LeavePlayerCount.Contains(TeamType))
				LeavePlayerCount.Add(TeamType, 0);
			++LeavePlayerCount[TeamType];
		}
	}

	// 무슨팀이 몇명 나갔는지 체크
	for (auto& LeavePlayer : LeavePlayerCount)
	{
		int NumToPlayer = GetMaxMatchPlayerNum();
		if (LeavePlayer.Value >= (NumToPlayer / 2))
		{
			ResultLeaveAllTeam = LeavePlayer.Key == ETeamType::ETT_Red ? ETeamType::ETT_Blue : ETeamType::ETT_Red;
			NetSendLeaveAllToPeer();
			return;
		}
	}
	return;
}

void AB2ControlGameMode::StartLeaveAllDirection()
{
	//if (auto* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub))
	//{
	//	const float DELAY_COUNT = 2.5f;
	//	pUI->SetLeaveEnemyText(ESlateVisibility::Visible);
	//	// todo fix
	//	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToLeaveEnemy);
	//	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(
	//		TimeToLeaveEnemy
	//		, this
	//		, &AB2ControlGameMode::EndLeaveAllGame
	//		, 0.1f
	//		, false
	//		, DELAY_COUNT);
	//}
	//else // ui 없으면 그냥 종료
	//{
	//	EndLeaveAllGame();
	//}
}

void AB2ControlGameMode::EndLeaveAllGame()
{
	SendEndMatch(ResultLeaveAllTeam);
}

void AB2ControlGameMode::UpdateTimerUI()
{
	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI)
	//{
	//	FText TextContent;// = FText::GetEmpty();
	//	FTimespan RemainTime;
	//	float fRemainTimeSec = 0.0f;

	//	switch (SceneManager.GetState())
	//	{
	//	case EControlMatchSceneState::WaitActive:
	//		pUI->SetHiddneTimerUI(false);
	//		TextContent = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlWaitActiveRemainTime")), FText::AsNumber(WaitActiveTime - int32(SceneManager.GetIngamePlayStateElapseTime())));
	//		break;
	//	default:
	//		// 이외상태엔 리턴
	//		pUI->SetHiddneTimerUI(true);
	//		return;
	//		break;
	//	}

	//	pUI->UpdateTimerUI(TextContent);
	//}
}

void AB2ControlGameMode::UpdateConquestEffect()
{
	if (!bStartMatch)
		return;
	//
	//float GageRed = m_pControlObject->GetConquestGage(true);
	//float GageBlue = m_pControlObject->GetConquestGage(false);
	//float GageRate = 0.0f;

	//// 점령하고 있는 팀 기준
	//TArray<AEmitter*> CurTeamEmitters;
	//TArray<AEmitter*> EnemyTeamEmitters;

	//if (GageRed != 0)
	//{
	//	GageRate = GageRed;

	//	CurTeamEmitters = ConquestAreaFxEmittersRed;
	//	EnemyTeamEmitters = ConquestAreaFxEmittersBlue;
	//}
	//else if (GageBlue != 0)
	//{
	//	GageRate = GageBlue;

	//	CurTeamEmitters = ConquestAreaFxEmittersBlue;
	//	EnemyTeamEmitters = ConquestAreaFxEmittersRed;
	//}
	//else
	//{
	//	return;
	//}

	//if (GageRate == 0.0f)
	//	LastActiveFxIndex = -1;

	//// 마지막꺼 따로관리할려면 -2 빼주기
	//int32 GegeToFxIndex = int32((CurTeamEmitters.Num() - 1) * GageRate);

	//// 현재 게이지에 맞는 인덱스의 이펙트 켜주고 상대팀껀 꺼줌
	//if (GegeToFxIndex > LastActiveFxIndex)
	//{
	//	CurTeamEmitters[GegeToFxIndex]->GetParticleSystemComponent()->SetActive(true, true);
	//	EnemyTeamEmitters[GegeToFxIndex]->GetParticleSystemComponent()->SetActive(false, true);
	//}
	//else
	//{
	//	// 인덱스보다 큰거 꺼줌
	//	for (int32 nCnt = 0; nCnt < CurTeamEmitters.Num(); ++nCnt)
	//	{
	//		if (nCnt > GegeToFxIndex)
	//		{
	//			UParticleSystemComponent* pComp = CurTeamEmitters[nCnt]->GetParticleSystemComponent();

	//			pComp->SetActive(false, true);
	//			
	//			// 중립 아니고 루핑일땐 상대편꺼 켜줌.
	//			if (!m_pControlObject->IsNeutralState() && pComp->Template->IsLooping()
	//				&& !EnemyTeamEmitters[nCnt]->GetParticleSystemComponent()->IsActive())
	//			{
	//				EnemyTeamEmitters[nCnt]->GetParticleSystemComponent()->SetActive(true, true);
	//			}
	//		}
	//	}		
	//}

	//LastActiveFxIndex = GegeToFxIndex;
}

uint8 AB2ControlGameMode::GetNextPlayStartIdx(bool IsMyStartIdx)
{
	// 주의 : 마티니의 PC Index는 1부터 시작!!
	// 자기자신은 무조건 1번 슬롯
	if (IsMyStartIdx)
		return 1;

	uint8 PCPlayStartIdx = 2; // 파티원은 2부터 시작
	for (int32 i = 0; i < AllPeerInfos.Num(); ++i)
	{
		uint8 checkidxOld = PCPlayStartIdx;
		for (auto peerInfo2 : AllPeerInfos)
		{
			if (PCPlayStartIdx == peerInfo2.Value.PlayerStartIdx)
			{
				PCPlayStartIdx += 1;
				break;
			}
		}

		// PCPlayStartIdx가 이전에 셋팅된적이 없으면 리턴
		if (PCPlayStartIdx <= GetMaxMatchPlayerNum() && PCPlayStartIdx == checkidxOld)
			return PCPlayStartIdx;
	}

	return 0;
}

bool AB2ControlGameMode::CanStartInGame()
{
	if (AllPeerInfos.Num() < GetStartPlayerNum())
		return false;

	for (auto PeerInfo : AllPeerInfos)
	{
		if (PeerInfo.Key == GetMyNetId())
			continue;

		if (PeerInfo.Value.IsReady == false)
			return false;
	}

	return true;
}

// 내가 안에 진입해있고 점령상태가 우리팀꺼인가
bool AB2ControlGameMode::IsMyAssaultState()
{
	/*if (m_pControlObject)
	{
		ABladeIICharacter* pPawn = Cast<ABladeIICharacter>(GetLocalController()->GetPawn());
		return m_pControlObject->IsOverlap(pPawn) && m_pControlObject->IsMatchingConquestAreaTeam(ETeamType(pPawn->GetTeamNum()));
	}*/

	return false;
}

bool AB2ControlGameMode::IsAssaultState()
{
	/*if (m_pControlObject)
	{
		return m_pControlObject->IsAssaultState();
	}*/

	return false;
}

void AB2ControlGameMode::ReturnToControlMainMenu()
{
	// 방나가기? 매칭완료후엔 나가기 기능이 없다함.
	// LeaveRoomIfJoined();

	//FLobbySceneManager::DeferredRegistChangeLobbyScene([](){
	//	// 전체 씬 전환을 하면 실 로딩 시간이 늘어나므로 원하는 화면 직전까지 UI History 만 만들어준다. 
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst) {
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterBattleMain);
	//	}
	//	//data_trader::Retailer::GetInstance().RequestGetTeamBattleStatus();
	//	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_ControlMain);
	//});

	//OpenBladeIILobbyCommon(this); // 씬 전환 예약 후 본격 로비 맵 로딩
}

void AB2ControlGameMode::SetFindingEnd()
{
	UB2UIDocControl* pDoc = UB2UIDocHelper::GetDocControl();

	if (pDoc)
		pDoc->SetFindingEnd();
}

TMap<uint32, FControlPeerInfo>& AB2ControlGameMode::GetAllPeerInfos()
{
	return AllPeerInfos;
}

void AB2ControlGameMode::StartPlay()
{
	Super::StartPlay();
}

void AB2ControlGameMode::SetSceneManagerState(EControlMatchSceneState NewState)
{
	SceneManager.SetState(NewState);
}

void AB2ControlGameMode::ApplyCurrentStateToUI()
{
	//if (m_pControlObject)
		//m_pControlObject->ApplyCurrentStateToUI();
}

void AB2ControlGameMode::SetActiveControlObject(bool NewActive)
{
	//m_pControlObject->SetActiveControlObject(NewActive);
}

void AB2ControlGameMode::StopAIControllers()
{
	for (TActorIterator<ABladeIIPlayerAIController> It(GetWorld()); It; ++It)
	{
		(*It)->StopAutoPlay();
	}
}

void AB2ControlGameMode::SendGamePlayInfo()
{
	//if (GetPeerType() == PEER_SUPER)
	//{
	//	// 호스트는 점령지 정보한번 전송
	//	if (m_pControlObject)
	//		m_pControlObject->BroadCastCurrentState();

	//	// 킬정보도 한번 전송.
	//	NetSendControlKillInfos();
	//	NetSendControlTimerInfos();
	//}
}

void AB2ControlGameMode::ReserveSpawnControlNPC(bool bFirst)
{
	//if (UB2UIDocHelper::GetDocControl()->GetControlType() != CONTROL_MODE_NOMINO || IsActiveControlTutorial())
	//{
	//	float fWaitTimeSec = bFirst ? NpcFirstSpawnWaitTime : NpcReSpawnWaitTime;

	//	GetWorldTimerManager().SetTimer(NpcSpawnTimeHandle, this, &AB2ControlGameMode::NetSendSpawnControlNPC, fWaitTimeSec, false);
	//	m_bApplyedNpcSpawnTimer = false;

	//	// 대기시간 UI에 설정
	//	UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//	if (pUI)
	//	{
	//		pUI->SetControlNpcUIState(EControlNpcUiState::WaitSpawn, fWaitTimeSec);
	//	}
	//}
}

void AB2ControlGameMode::NetSendSpawnControlNPC()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2ControlGameMode::NetSendSpawnControlNPC"));

	m_bApplyedNpcSpawnTimer = true;

	if (bEndMatch)
		return;

	// 함수는 다들어오는데 호스트만 실행
	if (GetPeerType() != PEER_SUPER)
		return;

	// 스폰알림 날려준다.
	packet::ByteStream	payload;
	FString encoded_string = packet::FinalizePacket(packet::SPAWN_CONTROL_NPC, 0, 0, packet::ALL, payload);
	SendMessage2(encoded_string);
}

// 타이밍 맞출 목적으로 호스트도 받고실행 
void AB2ControlGameMode::NetReceiveSpawnControlNPC()
{
	//if (UB2UIDocHelper::GetDocControl()->GetControlType() != CONTROL_MODE_NOMINO || IsActiveControlTutorial())
	//{
	//	// 혹여 남아있으면 제거후 스폰
	//	if (ControlNPC)
	//	{
	//		ControlNPC->Destroy();
	//		ControlNPC = nullptr;
	//	}

	//	// SpawnCharacter 요기타면서 ControlNPC 셋팅된다.
	//	GetActiveSpawnPool()->CustomSpawnWave(0);

	//	check(ControlNPC);

	//	if (GetPeerType() == PEER_SUPER)
	//	{
	//		ControlNPC->PromoteNetStatus();

	//		ControlNPC->StartPlay();
	//	}

	//	UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//	if (pUI)
	//	{
	//		pUI->SetControlNpcUIState(EControlNpcUiState::Live);
	//	}
	//}
}

void AB2ControlGameMode::ReserveRespawnControlSanctuary(int32 SancIndex)
{
	// 에디터에서 설정된 시간으로 예약
	// 아이디 -1이면 둘다 예약
	if (SancIndex == -1)
	{
		for (TPair<int32, float>& TimeInfo : ControlSanctuaryRespawnRemainTimeInfos)
		{
			TimeInfo.Value = SanctuaryReSpawnWaitTime;
		}
	}
	else
	{
		ControlSanctuaryRespawnRemainTimeInfos[SancIndex] = SanctuaryReSpawnWaitTime;
	}	
}

void AB2ControlGameMode::CheckControlSanctuaryRespawn(int32 SancIndex)
{
	if (GetPeerType() != PEER_SUPER || ControlSanctuaryRespawnRemainTimeInfos[SancIndex]  >= 0.0f)
		return;

	NetSendRespawnControlSanctuary(SancIndex);
}

ERoomCloseReasonType AB2ControlGameMode::GetStartRoomCloseReason()
{
	return ERoomCloseReasonType::START_ASSAULT;
}

FString AB2ControlGameMode::GetPlayerStartTagName(FControlPeerInfo& PeerInfo)
{
	switch (GetControlTeam(PeerInfo.PlayerStartIdx))
	{
	case ETeamType::ETT_Red:
		return FString::Printf(TEXT("R_%d"), int32((PeerInfo.PlayerStartIdx - 1) / 2) + 1);
		break;
	case ETeamType::ETT_Blue:
		return FString::Printf(TEXT("B_%d"), int32((PeerInfo.PlayerStartIdx - 1) / 2) + 1);
		break;
	default:
		return FString::Printf(TEXT("N_%d"), int32((PeerInfo.PlayerStartIdx - 1) / 2) + 1);
		break;
	}
}

ETeamType AB2ControlGameMode::GetControlTeam(uint8 PlayerStsrtIndex)
{
	return PlayerStsrtIndex % 2 == 1 ? ETeamType::ETT_Red : ETeamType::ETT_Blue;
}

float AB2ControlGameMode::GetControlObjectBonusTimeGage()
{
	/*if (m_pControlObject && !bEndMatch)
	{
		return m_pControlObject->GetBonusTimeGage();
	}*/

	return 0.0f;
}

void AB2ControlGameMode::ControlForceStart()
{
	//if (SceneManager.GetState() != EControlMatchSceneState::Ready || GetPeerType() != PEER_SUPER || bStartMatch)
	//	return;

	//for (TPair<uint32, FControlPeerInfo> PeerInfo : AllPeerInfos)
	//{
	//	APawn* pPlayer = FindPawnByNetId(PeerInfo.Key);

	//	if (!pPlayer)
	//	{
	//		NetSendSpawnCompleteForceAI(PeerInfo.Key);

	//		// 점령전 튜토리얼에선 추방이나, 타유저 채널나감 처리안한다.
	//		if (IsActiveControlTutorial())
	//			continue;

	//		// 서버로 추방 알림을 날린다. 있으면.
	//		if(RemotePlayerInfoData.Contains(PeerInfo.Key))
	//			data_trader::Retailer::GetInstance().RequestKickUser(GetRoomId(), RemotePlayerInfoData[PeerInfo.Key].GetAccountId());			
	//	}
	//}
}

void AB2ControlGameMode::ControlWallCollisionOff()
{
	if (GEngine && GLog)
	{
		GEngine->HandleCeCommand(GetWorld(), TEXT("ControlWallCollisionOff"), *GLog);
	}
}

void AB2ControlGameMode::NetSendRequestQTE()
{
	/*packet::ByteStream	payload;
	
	FString encoded_string = packet::FinalizePacket(packet::REQUEST_QTE, GetMyNetId(), 0, packet::ALL, payload);
	data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);*/
}

void AB2ControlGameMode::NetReceiveRequestQTE(uint32 NetId)
{
	// NetId는 요청 보낸애

	//if (GetPeerType() != PEER_SUPER)
	//	return;
	//
	//if (!ControlNPC)
	//	return;

	//ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(FindPawnByNetId(NetId));

	//if (!pPlayer)
	//	return;
	//
	//// 아직 허가한적 없으면 허가.
	//bool bAllow = !ControlNPC->bAllowedMount && pPlayer->IsQTEStartAble();
	//
	//// NPC제어 포기(허가 케이스만 포기..)
	//if(bAllow)
	//	ControlNPC->DemoteNetStatus();

	//// 전송
	//NetSendResponseQTE(NetId, bAllow);

	//// 허가했는가 플래그 온
	//if(bAllow)
	//	ControlNPC->bAllowedMount = true;
}

void AB2ControlGameMode::NetSendResponseQTE(uint32 NetId, bool bAllow)
{
	/*packet::ByteStream	payload;

	payload << NetId << bAllow;

	FString encoded_string = packet::FinalizePacket(packet::RESPONSE_QTE, GetMyNetId(), 0, packet::ALL, payload);
	data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);*/
}

void AB2ControlGameMode::NetReceiveResponseQTE(uint32 NetId, bool bAllow)
{
	//// NetId는 QTE실핼할애		
	//AB2ControlModePlayer* pPawn = Cast<AB2ControlModePlayer>(FindPawnByNetId(NetId));

	//if (!pPawn)
	//	return;

	//pPawn->ResetRequestStartQTEFlag();

	//if (!bAllow)
	//	return;
	//
	//// 호스트 이외에는 이패킷은 받은시점에 미노가 QTE가능상태가 아닐수도 있다.
	//// 가능상태때 실행하게 예약걸어놈
	//if (!ControlNPC->IsTimingForQTEPossession())
	//{
	//	ReservedQTEPlayerNetId = NetId;
	//	return;
	//}

	//// 내가 타는거면 제어권부터 획득
	//if (pPawn->IsLocalPlayer() && ControlNPC)
	//{
	//	ControlNPC->PromoteNetStatus();
	//}

	//// 요청하고 나서 cc걸렸을경우도 대비
	//pPawn->ClearDebuffs();

	//pPawn->SuperStartQTE();
}

void AB2ControlGameMode::NetSendNotifyEndQTE(uint32 NetId/* = 0*/)
{
	/*if (bEndMatch)
		return;

	if (NetId == 0)
		NetId = GetMyNetId();

	packet::ByteStream	payload;

	FString encoded_string = packet::FinalizePacket(packet::NOTIFY_END_QTE, NetId, 0, packet::ALL, payload);
	data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);*/
}

void AB2ControlGameMode::NetReceiveNotifyEndQTE(uint32 NetId)
{
	//AB2ControlModePlayer* pPawn = Cast<AB2ControlModePlayer>(FindPawnByNetId(NetId));

	//if (pPawn)
	//{
	//	// QTE내리면서 죽일수 있게.
	//	if (ControlNPC)
	//		ControlNPC->SetRemoteDyingSignal();

	//	pPawn->SuperStopQTEMount();

	//	// 요기서 플레이어 HP바 보이구. 미노타우르스 남은시간 게이지 제거.
	//	GetUIManager_InGameCombat()->HideFloatingHPBarForPlayer(pPawn, false);

	//	// BP에서 셀프 시간체크 & 디스트로이였는데 이제 패킷 받구도 제거해줌
	//	DestroyControlNpcRemainTimeFloatingGageBar();
	//}
}

float AB2ControlGameMode::GetControlNpcRemainHpRate()
{
	/*if (ControlNPC)
		return ControlNPC->GetHealth() / ControlNPC->GetMaxHealth();*/

	return 0.0f;
}

void AB2ControlGameMode::NetSendControlChat(EControlChatType ChatType, int32 MessageIndex)
{
	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
	//
	//if (pUI)
	//{
	//	// 채팅 창닫고 쿨타임 돌림
	//	pUI->NotifySendControlChat();
	//}

	////전송
	//packet::ByteStream	payload;
	//payload << uint8(ChatType) << MessageIndex;

	//FString encoded_string = packet::FinalizePacket(packet::CONTROL_CHAT, GetMyNetId(), 0, packet::ALL, payload);
	//data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);
}

void AB2ControlGameMode::NetReceiveControlChat(uint32 NetId, EControlChatType ChatType, int32 MessageIndex)
{
	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI)
	//{
	//	pUI->NotifyReceiveControlChat(NetId, ChatType, MessageIndex);
	//}
}

void AB2ControlGameMode::NetSendLeaveAllToPeer()
{
	/*if (GetPeerType() == PEER_SUPER)
	{
		packet::ByteStream	payload;
		FString encoded_string = packet::FinalizePacket(packet::CONTROL_ENEMY_LEAVE_ALL, GetMyNetId(), 0, packet::ALL, payload);
		data_trader::ChannelManager::GetInstance().SendMessage(0, encoded_string);
	}*/
}

float AB2ControlGameMode::GetControlResurrectTime(uint32 NetId)
{
	float fTimeMax = 1.0f;
	
	for (FControlResurrectInfo& ResurrectInfo : ControlResurrectInfos)
	{
		if (UB2UIDocHelper::GetDocControl()->GetDeadCount(NetId) == ResurrectInfo.ResurrectCount)
			return ResurrectInfo.ResurrectTime;

		if (fTimeMax < ResurrectInfo.ResurrectTime)
			fTimeMax = ResurrectInfo.ResurrectTime;
	}

	return fTimeMax;
}

void AB2ControlGameMode::ActiveDeadWatchingCamera(bool bActive)
{
	/*if (DeadWatchingCamera)
	{
		if (bActive)
			DeadWatchingCamera->ForceBeginActiveCamera();
		else
			DeadWatchingCamera->CustomEndActiveCamera();
	}*/
}

void AB2ControlGameMode::ActiveTutorialCenterCamera(bool bActive)
{
	/*if (TutorialCameraCenter)
	{
		if (bActive)
			TutorialCameraCenter->ForceBeginActiveCamera();
		else
			TutorialCameraCenter->CustomEndActiveCamera();
	}*/
}

void AB2ControlGameMode::ActiveTutorialSanctuaryCameraBottom(bool bActive)
{
	/*if (TutorialCameraSanctuaryBottom)
	{
		if (bActive)
			TutorialCameraSanctuaryBottom->ForceBeginActiveCamera();
		else
			TutorialCameraSanctuaryBottom->CustomEndActiveCamera();

		GetWorld()->GetTimerManager().SetTimer(TutorialWaitHandle, this, &AB2ControlGameMode::GotoTutorialNextStep, TutorialCameraSanctuaryBottom->GetBlendInTime(), false);
	}*/
}

void AB2ControlGameMode::ActiveTutorialSanctuaryCameraTop(bool bActive)
{
	/*if (TutorialCameraSanctuaryTopCamera)
	{
		if (bActive)
			TutorialCameraSanctuaryTopCamera->ForceBeginActiveCamera();
		else
			TutorialCameraSanctuaryTopCamera->CustomEndActiveCamera();

		GetWorld()->GetTimerManager().SetTimer(TutorialWaitHandle, this, &AB2ControlGameMode::GotoTutorialNextStep, TutorialCameraSanctuaryTopCamera->GetBlendInTime(), false);
	}*/
}

void AB2ControlGameMode::GotoTutorialNextStep()
{
	TutorialManager::GetInstance().GotoNextStep();
}

int32 AB2ControlGameMode::GetActualDamage()
{
	return FMath::FloorToInt(DamageLogInfo.ActualDamage);
}

void AB2ControlGameMode::SetPlayerToBattleUI(ABladeIINetPlayer* pPlayer)
{
	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI)
	//{
	//	//if (MatchState != MatchState::InProgress)
	//	//{
	//		pUI->AddTeamPlayerInfoUI(pPlayer);
	//	//}
	//	// 둘이 같이들어온다는 보장 없음
	//	// 난입 케이스
	//	//else
	//	//{
	//	//	// 난입시 같이들어온 상대팀 이름.
	//	//	int32 nPlayerStartIndex = AllPeerInfos[pPlayer->GetNetId()].PlayerStartIdx;
	//	//
	//	//	int32 nEnemyPlayerStartIndex = 0;
	//	//	uint32 nEnemyPlayerNetId = 0;
	//	//
	//	//	if (nPlayerStartIndex % 2 == 0)
	//	//		nEnemyPlayerStartIndex = nPlayerStartIndex - 1;
	//	//	else
	//	//		nEnemyPlayerStartIndex = nPlayerStartIndex + 1;
	//	//
	//	//	for (auto peerInfo : AllPeerInfos)
	//	//	{
	//	//		if (peerInfo.Value.PlayerStartIdx == nEnemyPlayerStartIndex)
	//	//		{
	//	//			nEnemyPlayerNetId = peerInfo.Key;
	//	//		}
	//	//	}
	//	//
	//	//	FString EnemyName = GetRemotePlayerInfoData(nEnemyPlayerNetId).GetUserNickName();
	//	//
	//	//	pUI->AddTeamPlayerInfoUIForIntrusion(pPlayer, EnemyName);
	//	//}
	//}
}

void AB2ControlGameMode::SetKillInfoToBattleUI(class ABladeIINetPlayer* pKillerPlayer, class ABladeIINetPlayer* pDeadPlayer)
{
	/*UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	if (pUI)
	{
		pUI->UpdateKillInfo(pKillerPlayer, pDeadPlayer);
	}*/
}

void AB2ControlGameMode::ViewPlayerJoinUI(class ABladeIINetPlayer* pPlayer)
{
	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI)
	//{
	//	pUI->ViewPlayerJoinUI(pPlayer);
	//}
}

void AB2ControlGameMode::TouchPressed()
{
	//SubmersionTime = 0;
	//if (SubmersionState == EControlMatchSubmersionState::FirstWarning)
	//{
	//	UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
	//	if (pUI)
	//	{
	//		pUI->SetMsgSubmersion(false);
	//	}
	//	SubmersionState = EControlMatchSubmersionState::Warning;
	//	MaxSubmersionTime = WarningSubmersionTime;
	//}
}

void AB2ControlGameMode::SetWaitRevive(bool InValue)
{
	IsWaitRevive = InValue;
}

void AB2ControlGameMode::CheckSubmersion(float InDelta)
{
	/*if (SceneManager.GetState() == EControlMatchSceneState::InGame && IsWaitRevive == false)
	{
		SubmersionTime += InDelta;

		if (SubmersionState == EControlMatchSubmersionState::FirstWarning)
		{
			UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
			if (pUI)
			{
				pUI->SetMsgSubmersionTime(MaxSubmersionTime - SubmersionTime);
			}
		}

		if (SubmersionTime > MaxSubmersionTime)
		{
			switch (SubmersionState)
			{
			case EControlMatchSubmersionState::Play:
				SubmersionOverTimePlayState();
				break;
			case EControlMatchSubmersionState::FirstWarning:
			case EControlMatchSubmersionState::Warning:
				SubmersionOverTimeWarningState();
				break;
			default:
				break;
			}
		}
	}*/
}

void AB2ControlGameMode::SubmersionOverTimePlayState()
{
	/*SubmersionTime = 0;
	MaxSubmersionTime = KickoutSubmersionTime;
	SubmersionState = EControlMatchSubmersionState::FirstWarning;
	UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
	if (pUI)
	{
		pUI->SetMsgSubmersion(true);
	}*/
}

void AB2ControlGameMode::SubmersionOverTimeWarningState()
{
	//SubmersionState = EControlMatchSubmersionState::Kickout;
	//TWeakObjectPtr<AB2ControlGameMode>	Capture(this);
	////이벤트바인드
	//DeliveryAssaultResultTicket = DeliveryAssaultResultClass<FB2ResponseAssaultResultPtr>::GetInstance().Subscribe([Capture](FB2ResponseAssaultResultPtr MsgPtr)
	//{
	//	DeliveryAssaultResultClass<FB2ResponseAssaultResultPtr>::GetInstance().Unsubscribe(Capture->DeliveryAssaultResultTicket);

	//	UB2UIDocControl* DocControl = UB2UIDocHelper::GetDocControl();
	//	if (DocControl)
	//	{
	//		DocControl->SetResultPtr(MsgPtr);
	//		DocControl->SetSubmersion(true);
	//	}

	//	if (!Capture.IsValid())
	//	{
	//		if (Capture.IsValid(true))
	//		{
	//			UE_LOG(LogEventSubsystem, Warning, TEXT("Called Event to capture invalid object - Event Name is %s"), TEXT("DeliveryAssaultResult"));
	//			return;
	//		}
	//		else
	//		{
	//			UE_LOG(LogEventSubsystem, Warning, TEXT("Called Event to capture already cleaned object - Event Name is %s"), TEXT("DeliveryAssaultResult"));
	//			return;
	//		}
	//	}
	//	Capture->LeaveRoomIfJoined();
	//});

	//auto battleInfo = std::make_shared<b2network::B2AssaultBattleInfo>();
	//battleInfo->kill_count = UB2UIDocHelper::GetDocControl()->GetKillCount(GetMyNetId());
	//battleInfo->death_count = UB2UIDocHelper::GetDocControl()->GetDeadCount(GetMyNetId());
	//battleInfo->damage = GetActualDamage();
	//data_trader::Retailer::GetInstance().RequestAssaultResult(GetToken(), b2::protocol::commondata::MatchResult::LOSE, false, true, battleInfo);
}

void AB2ControlGameMode::SetMvpScene()
{
	//// 점령전 튜토리얼에선 이타이밍에 로비로~ 로딩!
	//if (IsActiveControlTutorial())
	//{
	//	// 평소엔 점령전 정보(GetAssaultBattleStatus) 세션패킷 요청해서 받고 처리하는데 여기선 걍 로비로.
	//	ReturnToControlMainMenu();
	//	return;
	//}

	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::ControlMvpView);

	//SceneManager.SetState(EControlMatchSceneState::EndMatch);

	////GetWorld()->GetTimerManager().SetTimer(WaitFinalResultTimeHandle, this, &AB2ControlGameMode::SetResultScene, WaitFinalResultTime, false);

	////폰다숨김
	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	ABladeIICharacter* pPawn = Cast<ABladeIICharacter>(*Iterator);

	//	if (pPawn)
	//		pPawn->SetActorHiddenInGame(true);
	//}

	PlayEndBgm();
}

void AB2ControlGameMode::PlayEndBgm()
{
	ETeamType MyTeam = ETeamType(GetMyTeamNum());

	if (ResultWinTeam == ETeamType::ETT_End) // 무승부
	{
		PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_ControlBattleVictory);
	}
	else if (ResultWinTeam == MyTeam) //승리
	{
		PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_ControlBattleVictory);
	}
	else //패배
	{
		PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_Lose);
	}
}

void AB2ControlGameMode::SetResultScene()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::ControlResult);
}

#include "B2CombatStatEvaluator.h"

float AB2ControlGameMode::GetTotalAttack(ICharacterDataStore* CharacterDataStore)
{
	const auto MainPCClass = CharacterDataStore->GetMainPlayerClass();

	float Attack = CombatStatEval::GetPCAttack(MainPCClass, CharacterDataStore);
	float ModSpecializeAttack = CombatStatEval::GetPCAttackByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);

	float TotalAttack = Attack + ModSpecializeAttack;
	TotalAttack = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(MainPCClass))), TotalAttack);

	return TotalAttack;
}

float AB2ControlGameMode::GetTotalDefense(ICharacterDataStore* CharacterDataStore)
{
	const auto MainPCClass = CharacterDataStore->GetMainPlayerClass();

	float Defense				= CombatStatEval::GetPCDefense(MainPCClass, CharacterDataStore);
	float ModSpecializeDefense	= CombatStatEval::GetPCDefenseByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);

	float TotalDefense = Defense + ModSpecializeDefense;
	TotalDefense = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(MainPCClass))), TotalDefense);

	return TotalDefense;
}

void AB2ControlGameMode::GatherEquippedSkillAnimIDs(ICharacterDataStore* DataStore, TArray<FCombinedPCSkillAnimID>& OutEquippedSkills)
{
	// 점령전은 MainClass만 SkillAnim Load
	//OutEquippedSkills.Append(GetEquippedSkillAnimIDs(DataStore->GetMainPlayerClass(), DataStore));
}


void AB2ControlGameMode::GetAllMyTeamCharList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum, bool bOnlyLive /*= true*/)
{
	TArray<class ABladeIINetPlayer*> arPlayers = GetTeamPlayerArray(ETeamType(MyTeamNum));

	for (ABladeIINetPlayer* pPlayer : arPlayers)
	{
		if (!pPlayer)
			continue;

		//if (pPlayer->IsPendingKill())
		//	continue;

		if (bOnlyLive && !pPlayer->IsAlive())
			continue;

		OutList.Add(pPlayer);
	}
	
	// 팀버프 걸릴인원 선정시 동기화를위해 넷아이디순서로 정렬
	OutList.Sort([](const ABladeIICharacter& A, const ABladeIICharacter& B) { return B.GetNetId() > A.GetNetId(); });
}

// ============================== FControlMatchSceneManager
void FControlMatchSceneManager::SetGameMode(class AB2ControlGameMode* InGameMode)
{
	GameMode = InGameMode;

	if (GameMode)
	{ // 레벨 리소스 네이티브 레퍼런스 캐싱 타이밍.
		GameMode->RecacheLevelResources();
	}
}

void FControlMatchSceneManager::SetMatchLobbyResources(class AMatineeActor* InResultMatinee,
	class AB2StageEventDirector* InIntroEventDirector,
	const FControlMatchAnims& Gladiator,
	const FControlMatchAnims& Assassin,
	const FControlMatchAnims& Wizard,
	const FControlMatchAnims& Fighter)
{
	AnimArray.Empty(4);
	AnimArray.Add(Gladiator);
	AnimArray.Add(Assassin);
	AnimArray.Add(Wizard);
	AnimArray.Add(Fighter);

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	ResultMatinee = InResultMatinee;

	IntroEventDirector = InIntroEventDirector;
}

void FControlMatchSceneManager::SetState(EControlMatchSceneState InState, bool bNoAction)
{
	BII_CHECK(GameMode);
	CurrentState = InState;

	if (bNoAction)
		return;
	
	switch (InState)
	{
	case EControlMatchSceneState::Intro:
		SetIntro();
		break;
	case EControlMatchSceneState::Ready:
		SetReady();
		break;
	case EControlMatchSceneState::WaitActive:
		SetWaitActive();
		break;
	case EControlMatchSceneState::InGame:
		SetInGame();
		break;
	case EControlMatchSceneState::EndMatch:
		SetEndMatch();
	default:
		break;
	}

	//// 튜토리얼일때 래디, 대기씬에선 조이스틱처리 안한다.(튜토리얼액션에서 처리)
	//if (IsActiveControlTutorial() &&
	//	(InState == EControlMatchSceneState::Ready || InState == EControlMatchSceneState::WaitActive)
	//	)
	//{
	//	return;
	//}

	//GameMode->GetUIManager_InGameCombat()->HideVirtualJoystick(InState == EControlMatchSceneState::Intro || InState == EControlMatchSceneState::EndMatch);
}

void FControlMatchSceneManager::CheckIngameStateTime()
{
	if (ControlGamePlayStateTime == -1.0f || !GameMode)
		return;

	float fElapseTime = GameMode->GetWorld()->GetTimeSeconds() - ControlGamePlayStateTime;

	switch (CurrentState)
	{
	case EControlMatchSceneState::WaitActive:

		if (fElapseTime >= GameMode->WaitActiveTime)
		{
			ControlGamePlayStateTime = GameMode->GetWorld()->GetTimeSeconds();
			SetState(EControlMatchSceneState::InGame);
		}

		break;

	case EControlMatchSceneState::InGame:

		// 시간 체크안함. 시간 무제한
		//if (fElapseTime >= GameMode->InGameLimitedTime)
		//{
		//	if (GameMode->GetPeerType() != PEER_SUPER || GameMode->bSendEndMatch)
		//		return;
		//
		//	// 승패체크 필요하면 여기추가
		//	int32 nRedKillCount = UB2UIDocHelper::GetDocControl()->GetTeamKillCount(ETeamType::ETT_Red);
		//	int32 nBlueKillCount = UB2UIDocHelper::GetDocControl()->GetTeamKillCount(ETeamType::ETT_Blue);			
		//
		//	ETeamType WinTeam = ETeamType::ETT_End;
		//
		//	if (nRedKillCount > nBlueKillCount)
		//		WinTeam = ETeamType::ETT_Red;
		//
		//	if (nBlueKillCount > nRedKillCount)
		//		WinTeam = ETeamType::ETT_Blue;
		//
		//	GameMode->SendEndMatch(WinTeam);
		//}

		break;
	}
}

void FControlMatchSceneManager::SetIntro()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::None);

	//GameMode->PrepareHostPlayer();

	//// 레벨BP에서 셋팅 안했을수도 있음
	//check(IntroEventDirector);

	//IntroEventDirector->CustomBeginShow();
}

void FControlMatchSceneManager::SetReady()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::ControlBattle);

	//if (IsActiveControlTutorial())
	//{
	//	// 추가처리는 TutorialManager - ACTION_EXCEPTION에서
	//	return;
	//}

	//GameMode->SetReady();
}

void FControlMatchSceneManager::SetWaitActive()
{
	ControlGamePlayStateTime = GameMode->GetWorld()->GetTimeSeconds();
	
	GameMode->SendGamePlayInfo();

	GameMode->ControlWallCollisionOff();

	GameMode->ForceFeedback();
}

void FControlMatchSceneManager::SetInGame()
{
	// SetEndMatch 에서 StageEventSpecificScalability 를 직접 넣는 케이스가 있어서 당장 필요성이 보이지는 않지만 짝맞춤 용으로 넣어둠. 
	// 이쪽 연출이 StageEventDirector 없이 별도 구현이 되다 보면 이거 이전에 어떤 이유로든 별도의 연출 전용 scalability 가 적용된 채 지속될 수가 있어서 리셋을 넣어주는 게 좋기도 하고.
	//B2Scalability::ApplyStageEventSpecificScalabilitySetting(GameMode, false);

	//GameMode->SetActiveControlObject(true);

	//GameMode->ReserveSpawnControlNPC(true);

	//GameMode->ReserveRespawnControlSanctuary();

	//
	//B2GMLoadingProgCollector::StepToMax(); // 로딩 진행도 이쯤서 꽉 채워넣음.
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::ControlBattle);

	//ControlGamePlayStateTime = GameMode->GetWorld()->GetTimeSeconds();

	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI)
	//{
	//	pUI->PlayAnimationEvent_AnimOpenLock();
	//}

	//// 추가시간 위젯 생성
	//UB2UIManager::GetInstance()->OpenUI(UIFName::ControlAddTime);
}

void FControlMatchSceneManager::SetEndMatch()
{
	//if (ResultMatinee)
	//{
	//	ResultMatinee->SetPosition(0.0f);
	//	ResultMatinee->Play();

	//	ABladeIIGameMode::SetCameraMatineeCam(ResultMatinee, 0.0f);
	//}

	//if (!GameMode)
	//	return;

	//TArray<ABladeIINetPlayer*> ResultPlayers;
	//
	//// 이긴팀 or 비겼으면 우리팀 찾아서 퍼펫으로 생성해줘라
	//if(GameMode->ResultWinTeam == ETeamType::ETT_End)
	//	ResultPlayers = GameMode->GetTeamPlayerArray(ETeamType(GameMode->GetMyTeamNum()));
	//else
	//	ResultPlayers = GameMode->GetTeamPlayerArray(GameMode->ResultWinTeam);

	//int32 nPlayerIndex = 0;

	//UB2UIControlMvpView* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlMvpView>(UIFName::ControlMvpView);

	//if (pUI)
	//{
	//	pUI->SetVisibility(ESlateVisibility::Hidden);
	//}

	//// StageEventDirector 를 통한 장면이면 연출 전용 scalability adjust가 적용이 되는데 여긴 따로.
	//// 일반적으로 Scalability 적용 여부가 버그 상황을 만들어내지는 않지만 딴 것보다 ViewDistanceBias 상향이 적용되지 않으면 빵꾸뚫린 외관이 나올 수 있어서 특별히 넣어둠.
	//// 게임모드 나가기 전이라 되돌릴 필요는 없지만 할 수 있다면 짝은 맞춰 주는 게 좋지
	//B2Scalability::ApplyStageEventSpecificScalabilitySetting(GameMode, true);

	//int32 nBestKillCount = 1;

	//for (ABladeIINetPlayer* pPlayer : ResultPlayers)
	//{
	//	int32 nPlayerKillCount = UB2UIDocHelper::GetDocControl()->GetKillCount(pPlayer->GetNetId());
	//	if (nPlayerKillCount > nBestKillCount)
	//		nBestKillCount = nPlayerKillCount;
	//}

	//for (ABladeIINetPlayer* pPlayer : ResultPlayers)
	//{
	//	nPlayerIndex++;

	//	ICharacterDataStore* pDataStore = pPlayer->GetCharacterDataStore();

	//	ASkeletalMeshActor* pActor = SpawnMvpPuppetActor(pDataStore->GetMainPlayerClass(), pDataStore, nPlayerIndex);

	//	if (pActor)
	//	{
	//		bool bMvp = false;

	//		int32 nPlayerKillCount = UB2UIDocHelper::GetDocControl()->GetKillCount(pPlayer->GetNetId());

	//		if (nPlayerKillCount >= nBestKillCount)
	//		{
	//			bMvp = true;
	//			pActor->GetSkeletalMeshComponent()->PlayAnimation(AnimArray[PCClassToInt(pPlayer->GetCharacterDataStore()->GetMainPlayerClass())].Mvp, false);

	//		}

	//		if (pUI)
	//		{
	//			pUI->AddPlayer(pActor, pPlayer, bMvp);
	//		}
	//	}
	//}
}

ASkeletalMeshActor* FControlMatchSceneManager::SpawnMvpPuppetActor(EPCClass CharClass, const class ICharacterDataStore* DataStore, int32 PlayerIndex)
{
	/*FString PositionName(TEXT("p") + FString::FormatAsNumber(PlayerIndex));
	ASkeletalMeshActor* EntryActor = SpawnPuppetActor(PositionName, ResultMatinee, CharClass, DataStore, AnimArray[PCClassToInt(CharClass)].Idle);
	SpawnedActors.Add(EntryActor);
	return EntryActor;*/
	return NULL;

}

float FControlMatchSceneManager::GetIngamePlayStateElapseTime()
{
	return GameMode->GetWorld()->GetTimeSeconds() - ControlGamePlayStateTime;
}

void FControlMatchSceneManager::SetControlGamePlayStateTime(float fTimeSec)
{
	ControlGamePlayStateTime = fTimeSec;
}

void FControlMatchSceneManager::MatchResultMatineeFinished()
{
	//if (ResultMatinee)
	//{
	//	ResultMatinee->Pause();
	//}

	//UB2UIControlMvpView* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlMvpView>(UIFName::ControlMvpView);

	//if (pUI)
	//{
	//	pUI->SetVisibility(ESlateVisibility::Visible);
	//	pUI->SetVisibilityPlayerUI(ESlateVisibility::Visible);
	//	switch (GameMode->ResultWinTeam)
	//	{
	//	case ETeamType::ETT_Red:
	//		pUI->PlayAnimationEvent_AnimWinRed();
	//		break;
	//	case ETeamType::ETT_Blue:
	//		pUI->PlayAnimationEvent_AnimWinBlue();
	//		break;
	//	case ETeamType::ETT_End:
	//		pUI->PlayAnimationEvent_AnimDraw();
	//		break;
	//	}
	//}
}
