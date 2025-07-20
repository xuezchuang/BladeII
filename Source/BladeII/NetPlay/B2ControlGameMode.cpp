
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
#include "../BladeII/BladeIIUtil.h"

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
	//{ // DLC Front 葛靛 府家胶肺爹 弥措茄 力芭

	//	// 敲饭捞绢 纳腐磐侩 眉仿官+备福扁酿鸥烙侩 UI努贰胶
	//	// BP俊辑 犁汲沥且荐档乐澜
	//	FString ControlGameModeFloatingHPBarWidgetClassPath;
	//	GConfig->GetString(TEXT("/Script/BladeII.BladeIICharacter"), TEXT("ControlGameModeFloatingHPBarWidgetClass"), ControlGameModeFloatingHPBarWidgetClassPath, GGameIni);
	//	static ConstructorHelpers::FClassFinder<UB2FloatingHPBar> ControlGameModeFloatingHPBarWidgetClass(*ControlGameModeFloatingHPBarWidgetClassPath);
	//	if (ControlGameModeFloatingHPBarWidgetClass.Class != NULL)
	//	{
	//		FloatingHPBarWidgetClass = ControlGameModeFloatingHPBarWidgetClass.Class;
	//	}
	//}

	// 措扁矫埃 檬扁蔼
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

	UpdateTimerUI();

	//UpdateConquestEffect();

	//// 郴啊 痢飞扁咯窍备乐绰瘤 眉农(炼扒 弊成 痢飞瘤 光绊乐绰啊)
	//if (m_pControlObject && m_pControlObject->IsOverlap(GetLocalController()->GetPawn()))
	//	UB2UIDocHelper::GetDocControl()->AddMyContributeTime(delta);

	//// 己家 府哩 鸥捞赣甸 眉农
	//for (TPair<int32, float>& TimeInfo : ControlSanctuaryRespawnRemainTimeInfos)
	//{
	//	if (TimeInfo.Value >= 0.0f)
	//	{
	//		TimeInfo.Value -= delta;

	//		if (TimeInfo.Value < 0.0f)
	//			CheckControlSanctuaryRespawn(TimeInfo.Key);
	//	}
	//}

	//// QTE啊瓷惑怕 捞傈俊 QTE矫累菩哦 罐篮纳捞胶
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
	////懦牢器 罐绰 捞亥飘 萍南
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
	//// 傈眉 敲饭捞绢侩(肺拿, 嚎, 府葛飘)
	//ABladeIINetPlayer* TargetNetPlayer = Cast<ABladeIINetPlayer>(DeadGuy);
	//if (!bEndMatch && TargetNetPlayer)
	//{
	//	// 磷篮 墨款飘 棵覆
	//	UB2UIDocHelper::GetDocControl()->AddDeadCount(TargetNetPlayer->GetNetId());

	//	// 敲饭捞绢UI俊 磷篮冉荐 利侩(8疙栋乐绰镑)
	//	UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//	if (pUI)
	//		pUI->SetPlayerResurrectTime(TargetNetPlayer->GetNetId(), GetControlResurrectTime(TargetNetPlayer->GetNetId()));
	//}

	//// 肺拿敲饭捞绢侩
	//ABladeIIPlayer* LocalPawn = Cast<ABladeIIPlayer>(GetLocalController()->GetPawn());
	//if (DeadGuy == LocalPawn)
	//{
	//	if (!bEndMatch)
	//	{
	//		// 何劝 措扁UI
	//		UB2UIWaitRevive* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIWaitRevive>(UIFName::WaitRevive);

	//		if (pUI)
	//			pUI->InitTimer(GetControlResurrectTime(GetMyNetId()));

	//		TouchPressed();
	//		SetWaitRevive(true);
	//	}

	//	// 敲饭捞绢 UI概聪廉 力芭(牧飘费 霸烙葛靛父)
	//	DeadGuy->DestroyPlayerUIManager();		

	//	// 包傈 墨皋扼
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
//	// 懦墨款飘
//	ABladeIINetPlayer* pPlayer = Cast<ABladeIINetPlayer>(DamageCauser);
//	ABladeIINetPlayer* pDeadPlayer = Cast<ABladeIINetPlayer>(TargetCharacter);
//	ABladeIIControlNetCharacter* pMinotaur = Cast<ABladeIIControlNetCharacter>(DamageCauser);
//
//	// 捧荤眉唱 官蹿纳捞胶
//	if (!pPlayer && DamageCauser)
//		pPlayer = Cast<ABladeIINetPlayer>(DamageCauser->GetOwner());
//
//	// 啪铰茄 固畴啊 磷牢纳捞胶
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
//	// 肺拿敲饭捞绢啊 搬加胶懦 己家冈绊 磷栏搁 盲澄郴 舅覆
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
//		// 磊混捞芭唱(沥惑风飘绰 绝澜) 敲饭捞绢啊 鸥瘤臼篮 痢飞傈 各茄抛 磷篮 纳捞胶
//		return;
//	}
//
//	// 磷篮仇捞 肺拿敲饭捞绢
//	if (pDeadPlayer == Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn()))
//	{
//		UB2UIDocHelper::GetDocControl()->ResetSerialKillCount();
//	}
//
//	// 磷牢仇捞 肺拿敲饭捞绢
//	if (pPlayer == Cast<ABladeIINetPlayer>(GetLocalController()->GetPawn()))
//	{
//		UB2UIDocHelper::GetDocControl()->AddSerialKillCount();
//	}
//
//	// 龋胶飘绰 懦历厘, 舅覆
//	if (pPlayer && GetPeerType() == PEER_SUPER)
//	{		
//		// UIDoc俊 历厘
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
	// 肺拿敲饭捞绢啊 搬加胶懦 静搁 盲澄郴 舅覆
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
	// 各档 眠啊
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
		// 穿利 单固瘤绰 int32肺茄促. 喊捞蜡 绝澜. 家荐痢 钎扁 救窍聪瘪..
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
//	//// 痢飞瘤 困摹爱促 结航
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
	//// pCharacter 夯牢捞 痢飞傈 固畴搁 漂沥 裹困救俊辑 敲饭捞绢 茫澜
	//if (pControlNetChar && m_pControlObject)
	//{
	//	ACharacter* pSelectedTargetCharacter = NULL;

	//	for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//	{
	//		ABladeIINetPlayer* Player = Cast<ABladeIINetPlayer>(*Iterator);

	//		if (Player && Player->IsAlive())
	//		{
	//			// 痢飞瘤客狼 芭府
	//			float fCurTargetDistanceCO = (Player->GetActorLocation() - m_pControlObject->GetActorLocation()).Size();

	//			// NPC客狼 芭府
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
	//// NPC磷菌栏搁 澄
	//if (ControlNPC == TargetCharacter)
	//{
	//	ControlNPC = NULL;

	//	ReserveSpawnControlNPC(false);
	//}
}

void AB2ControlGameMode::NotifyQTEMountBeginToIdle(class ABladeIIPlayer* pPlayer)
{
	Super::NotifyQTEMountBeginToIdle(pPlayer);

	//// UI俊 舅覆
	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI && pPlayer)
	//{
	//	if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Red)
	//		pUI->SetControlNpcUIState(EControlNpcUiState::QTE_Red, ControlNPC->QTEMountTime);

	//	if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Blue)
	//		pUI->SetControlNpcUIState(EControlNpcUiState::QTE_Blue, ControlNPC->QTEMountTime);
	//}

	//// 夸扁辑 敲饭捞绢 HP官 见扁备. 固畴鸥快福胶 巢篮矫埃 霸捞瘤 钎扁.
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

		// 磊扁磊脚 悸泼
		SetMyPlayerInfoData(DocLobbyMatch->GetMyNetId(), FB2FindAccountInfoPtr());
		AddPeerInfo(DocLobbyMatch->GetMyNetId(), DocLobbyMatch->GetMyStartIndex());

		// 蜡历悸泼
		for (int32 i = 0; i < DocLobbyMatch->GetRemotePlayerInfoDataCount(); ++i)
		{
			int32 NetId = DocLobbyMatch->GetRemoteNetIdByIndex(i);
			EPCClass ReservedPCClass = DocLobbyMatch->GetReservedRemotePCClassByIndex(i);
			
			SetRemotePlayerInfoData(NetId, DocLobbyMatch->GetRemoteAccountIdByIndex(i), DocLobbyMatch->GetRemoteAccountInfoByIndex(i));

			// 皋牢纳腐磐 努贰胶 悸泼
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
		// 何劝 困摹 : 胶迄困摹		
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
	// <-- 评喊 妮府傈 鸥涝
	if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Red)
		pPlayer->GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Blue)
		pPlayer->GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
	// --> 评喊 妮府傈 鸥涝
}
void AB2ControlGameMode::OnHostChanged(int32 RoomType, int32 NewHostId)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2ControlGameMode::OnHostChanged"));

	//// 捞傈 龋胶飘啊 己家付矫备 唱埃芭搁 舅覆(郴啊 货龋胶飘老锭父)
	//if (GetMyNetId() == NewHostId)
	//{
	//	const int32* SancIndexPtr = SanctuaryDrunkInfos.FindKey(GetHostId());

	//	if (SancIndexPtr)
	//		NetSendConsumeControlSanctuary(*SancIndexPtr);
	//}

	//// 肺拿敲饭捞绢 QTE夸没扁废 力芭
	//AB2ControlModePlayer* pLocalPlayer = Cast<AB2ControlModePlayer>(FindPawnByNetId(GetMyNetId()));

	//if (pLocalPlayer)
	//	pLocalPlayer->ResetRequestStartQTEFlag();

	//Super::OnHostChanged(RoomType, NewHostId);

	//if (GetMyNetId() != NewHostId)
	//	return;

	//if (MatchState != MatchState::InProgress)
	//	return;

	//// 霸烙辆丰矫 唱埃敲饭捞绢 贸府 救窃
	//if (bEndMatch)
	//	return;

	//// 龋胶飘 官差搁 眉农茄锅秦霖促.
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
	//	// 鸥捞赣啊 利侩登菌绰单 固畴啊 绝促? 弊烦 胶迄眉农
	//	// 郴鸥捞赣绰 捞固 辆丰, 扁粮 龋胶飘绰 鸥捞赣 促倒疽阑锭码 唱埃纳捞胶老淀
	//	if (m_bApplyedNpcSpawnTimer)
	//		NetSendSpawnControlNPC();
	//}

	CheckLeaveAll();
}

void AB2ControlGameMode::HandleNotifyLeave(uint32 NetId)
{
	AIControlleredPeerIds.AddUnique(NetId);

	//// 胶迄 救焊郴备 栋车栏搁 胶迄矫难霖促.
	//// 龋胶飘啊 唱艾阑版快. 龋胶飘眉牢瘤焊促 捞霸 刚历客具窃.
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

	//// 栋抄仇捞 付瘤阜栏肺 己家付脚仇捞搁 舅覆.(龋胶飘 眉农)
	//
	//const int32* SancIndexPtr = SanctuaryDrunkInfos.FindKey(NetId);

	//if (SancIndexPtr)
	//	NetSendConsumeControlSanctuary(SanctuaryDrunkInfos[*SancIndexPtr]);

	//// 痢飞傈 譬配府倔俊急 眠规捞唱, 鸥蜡历 盲澄唱皑 贸府救茄促.
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
		//何劝!
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

	// 鸥捞赣 倒赴促. 矫埃促登搁 救甸绢柯乔绢 AI肺 胶迄窍绊 矫累茄促.
	GetWorld()->GetTimerManager().SetTimer(ForceStartTimeHandle, this, &AB2ControlGameMode::ControlForceStart, ForceStartTime, false);
}

void AB2ControlGameMode::CheckKicked()
{
	//if (bIsKicked)
	//{
	//	// 肺爹捞 辨绢辑 眠规寸沁促 ぬぬ 扑诀 剁况林磊.
	//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Control_Ban")),
	//		0.f,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::Confirm,
	//		FMsgPopupOnClick::CreateLambda([this]() {			
	//			// 肺厚肺.
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
	// 白场车栏搁 何劝窍瘤 富磊.
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

	// 肺拿敲饭捞绢侩 拳混钎 UI
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
	
	// 牢橇肺弊贰胶俊辑 咯扁 甸绢吭栏搁 抄涝蜡历捞聪 UI剁况淋
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

void AB2ControlGameMode::SetControlObject(class ABladeIINetControlObject* InControlObject)
{
	//m_pControlObject = InControlObject;
}

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

void AB2ControlGameMode::AddSanctuaryControlGame(class AB2SanctuaryControlGame* InSanctuary)
{
	//if (InSanctuary)
	//{
	//	m_arSanctuary.Add(InSanctuary);
	//	
	//	ControlSanctuaryRespawnRemainTimeInfos.Add(InSanctuary->NetIndex, -1.0f);
	//}
}

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
	//// 霸烙矫累
	////SendGamePlayInfo();

	//// 霖厚UI俊 傈崔
	//UB2UIControlReadyToBattle* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlReadyToBattle>(UIFName::ControlReadyToBattle);

	//if (pUI)
	//{
	//	pUI->PlayAnimationEvent_ReadyToStartGame();
	//}

	//bStartMatch = true;

	//// 3 2 1 颇捞飘 躲锭 绝局崔扼窃. 救俊辑 鸥捞赣 扒促.
	//DestroyControlPlayerArrow();
}

void AB2ControlGameMode::SendLeavePlayer(uint32 NetId, int64 AccountId)
{
	//// 痢飞傈 譬配府倔俊急 眠规捞唱, 鸥蜡历 盲澄唱皑 贸府救茄促.
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

	//// 痢飞瘤 捞棋飘 难淋
	//if (WinTeam == ETeamType::ETT_Red && WinFxEmittersRed)
	//	WinFxEmittersRed->GetParticleSystemComponent()->SetActive(true, true);;

	//if (WinTeam == ETeamType::ETT_Blue && WinFxEmittersBlue)
	//	WinFxEmittersBlue->GetParticleSystemComponent()->SetActive(true, true);;

	//StopAIControllers();

	//if (ControlNPC)
	//{
	//	// NPC 胶啪	
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

// 葛电 蜡历爱绊坷扁
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

// 快府祈 蜡历爱绊坷扁
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
	// UIDoc俊 历厘
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

		// 葛滴俊霸 舅覆. 唱档 郴啊焊辰吧肺 技泼!
		FString encoded_string = packet::FinalizePacket(packet::CONTROL_KILLCOUNT, GetMyNetId(), 0, packet::ALL, payload);
		SendGameMessage(encoded_string);
		UE_LOG(LogBladeII, Log, TEXT("Send [CONTROL_KILLCOUNT]"));
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
	SendGameMessage(encoded_string);
	UE_LOG(LogBladeII, Log, TEXT("Send [CONTROL_TIMER]"));
}

void AB2ControlGameMode::AckControlTimerInfos(uint8 SceneState, float ElapseTime)
{
	SceneManager.SetState(EControlMatchSceneState(SceneState), true);
		
	SceneManager.SetControlGamePlayStateTime(GetWorld()->GetTimeSeconds() - ElapseTime);
}

void AB2ControlGameMode::NetSendAskSpawnComplete()
{
	// 促弗 敲饭捞绢茄抛 拱绢航 呈 胶迄沁聪?
	packet::ByteStream	payload;

	FString encoded_string = packet::FinalizePacket(packet::ASK_SPAWN_COMPLETE, GetMyNetId(), 0, packet::ALLBUTME, payload);
	SendGameMessage(encoded_string);
	UE_LOG(LogBladeII, Log, TEXT("Send [ASK_SPAWN_COMPLETE]"));
}

void AB2ControlGameMode::NetReceiveAskSpawnComplete()
{
	// 炼扒绝捞 焊郴航..
	NetSendSpawnComplete();
}

void AB2ControlGameMode::NetSendSpawnComplete()
{
	// 胶迄 肯肺登菌促 舅覆. 促弗 敲饭捞绢啊 拱绢焊搁 肚舅妨淋.
	packet::ByteStream	payload;
	
	// 碍力 胶迄牢啊.
	payload << false;

	FString encoded_string = packet::FinalizePacket(packet::SPAWN_COMPLETE, GetMyNetId(), 0, packet::ALLBUTME, payload);
	SendGameMessage(encoded_string);

	UE_LOG(LogBladeII, Log, TEXT("Send [SPAWN_COMPLETE]"));

	// 泅犁 纳腐磐 局聪皋捞记 沥焊档 茄锅 焊郴拎具摆促.
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

	// 捞固 乐栏搁 府畔
	if (pPawn)
	{
		return;
	}

	PrepareRemotePlayer(NetId);

	if (bForce)
	{
		if (NetId == GetMyNetId())
		{
			// Ai胶迄 牢单 弊霸 唱捞搁 肺厚肺 焊郴扁 敲贰弊 挪促.
			// Ready纠 码俊辑 扑诀 剁况临芭
			// 眠规寸茄芭烙
			bIsKicked = true;

			// 牢飘肺 捞饶 霖厚窜拌俊辑 眉农窍绰单 捞窜拌啊 捞固 瘤车栏搁 肚 眉农秦淋
			if (bKickChecked)
				CheckKicked();

			return;
		}
		// 栋巢 肺流阑 蝶弗促.
		HandleNotifyLeave(NetId);
	}
}

void AB2ControlGameMode::NetSendSpawnCompleteForceAI(uint32 NetId)
{
	packet::ByteStream	payload;
	
	// 碍力 胶迄牢啊.
	payload << true;

	FString encoded_string = packet::FinalizePacket(packet::SPAWN_COMPLETE, NetId, 0, packet::ALL, payload);
	SendGameMessage(encoded_string);

	UE_LOG(LogBladeII, Log, TEXT("Send [SPAWN_COMPLETE_AI]"));
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
	SendGameMessage(encoded_string);
}

void AB2ControlGameMode::NetReceiveQTEEnableEnd()
{
	//if (ControlNPC)
	//	ControlNPC->OnNetReceiveQTEEnableEnd();
}

// 己家 家葛凳(搬加胶懦阑 静芭唱, 静瘤臼绊 磷芭唱, 静瘤臼绊 唱啊芭唱)
void AB2ControlGameMode::NetSendConsumeControlSanctuary(int32 SancIndex)
{
	packet::ByteStream	payload;

	payload << SancIndex;

	FString encoded_string = packet::FinalizePacket(packet::CONSUME_CONTROL_SANCTUARY, 0, 0, packet::ALL, payload);
	SendGameMessage(encoded_string);
}

void AB2ControlGameMode::NetReceiveConsumeControlSanctuary(int32 SancIndex)
{
	// 蜡历 盲澄栋巢贸府, 龋胶飘 函版贸府 苞沥俊辑 部捞搁 吝汗栏肺 朝扼棵荐档 乐阑芭鞍促.
	//check(SanctuaryDrunkInfos.Contains(SancIndex));
	if (!SanctuaryDrunkInfos.Contains(SancIndex))
		return;

	SanctuaryDrunkInfos.Remove(SancIndex);

	ReserveRespawnControlSanctuary(SancIndex);
}

// 己家 哩凳
void AB2ControlGameMode::NetSendRespawnControlSanctuary(int32 SancIndex)
{
	if (GetPeerType() != PEER_SUPER)
		return;

	packet::ByteStream	payload;

	payload << SancIndex;

	FString encoded_string = packet::FinalizePacket(packet::SPAWN_CONTROL_SANCTUARY, 0, 0, packet::ALL, payload);
	SendGameMessage(encoded_string);
}

void AB2ControlGameMode::NetReceiveRespawnControlSanctuary(int32 SancIndex)
{
	// 趣矫 矫埃 巢酒乐阑鳖毫
	//ControlSanctuaryRespawnRemainTimeInfos[SancIndex] = -1.0f;

	//// 己家 盲快扁
	//for (AB2SanctuaryControlGame* pSanc : m_arSanctuary)
	//{
	//	if (pSanc && pSanc->NetIndex == SancIndex)
	//		pSanc->RecycleInteractiveActor();
	//}

	//// UI畴萍胶 剁框
	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI)
	//{
	//	pUI->OnRespawnControlSanctuary();
	//}

}

// 己家 冈澜
void AB2ControlGameMode::NetSendDrinkControlSanctuary(uint32 NetId, int32 SancIndex)
{
	if (GetPeerType() != PEER_SUPER)
		return;

	packet::ByteStream	payload;

	payload << SancIndex;

	FString encoded_string = packet::FinalizePacket(packet::DRINK_CONTROL_SANCTUARY, NetId, 0, packet::ALL, payload);
	SendGameMessage(encoded_string);
}

void AB2ControlGameMode::NetReceiveDrinkControlSanctuary(uint32 NetId, int32 SancIndex)
{
	// 己家 付矫扁
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

	//// 付缴贸府 or 家葛贸府绰 肺拿敲饭捞绢父 眉农
	//if (pPlayer && pPlayer->IsLocalPlayer())
	//{
	//	// 胶懦 救硅奎栏搁 家葛贸府
	//	if(pPlayer->GetCharacterDataStore()->GetUnitySkillId(pPlayer->GetCurrentPlayerClass()) == SKILL_INVALID_ID)
	//		NetSendConsumeControlSanctuary(SancIndex);
	//	else
	//	{
	//		// 冈绊 肚冈栏搁 家葛贸府
	//		if (pPlayer->IsWeaponSkillPointMax())
	//			NetSendConsumeControlSanctuary(SancIndex);
	//		else
	//			pPlayer->SetWeaponSkillPointMax();
	//	}

	//	// 眉仿雀汗
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

// UI俊辑 傈崔罐澜
void AB2ControlGameMode::FinishReadyToStartGame()
{
}

void AB2ControlGameMode::SetMatchLobbyResources(class ALevelSequenceActor* InResultMatinee,
	class AB2StageEventDirector* InIntroEventDirector,
	const FControlMatchAnims& Gladiator,
	const FControlMatchAnims& Assassin,
	const FControlMatchAnims& Wizard,
	const FControlMatchAnims& Fighter,
	TArray<AEmitter*> EmitterRed,
	TArray<AEmitter*> EmitterBlue,
	AEmitter* WinEmitterRed,
	AEmitter* WinEmitterBlue)
{
	//SceneManager.SetMatchLobbyResources(InResultMatinee, InIntroEventDirector, Gladiator, Assassin, Wizard, Fighter);

	//ConquestAreaFxEmittersRed = EmitterRed;
	//ConquestAreaFxEmittersBlue = EmitterBlue;
	//WinFxEmittersRed = WinEmitterRed;
	//WinFxEmittersBlue = WinEmitterBlue;
}

void AB2ControlGameMode::SetTutorialCamera(class AB2ActiveCameraActor* pCenterCamActor, class AB2ActiveCameraActor* pSanctuaryBottomCamActor, class AB2ActiveCameraActor* pSanctuaryTopCamActor)
{
	//TutorialCameraCenter = pCenterCamActor;
	//TutorialCameraSanctuaryBottom = pSanctuaryBottomCamActor;
	//TutorialCameraSanctuaryTopCamera = pSanctuaryTopCamActor;
}

void AB2ControlGameMode::SetDeadWatchingCamera(class AB2ActiveCameraActor* pCamActor)
{
	//DeadWatchingCamera = pCamActor;
}

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
	//	// 譬配府倔东俊 肛描带 胶徘 柳青茄促.
	//	SetReady();

	//	// 碍力矫累 鸥捞赣 秒家窍备 窃荐龋免.
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

	// 唱埃 敲饭捞绢 眉农
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

	// 公郊评捞 割疙 唱艾绰瘤 眉农
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
	//else // ui 绝栏搁 弊成 辆丰
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
	//		// 捞寇惑怕浚 府畔
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

	//// 痢飞窍绊 乐绰 评 扁霖
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

	//// 付瘤阜波 蝶肺包府且妨搁 -2 哗林扁
	//int32 GegeToFxIndex = int32((CurTeamEmitters.Num() - 1) * GageRate);

	//// 泅犁 霸捞瘤俊 嘎绰 牢郸胶狼 捞棋飘 难林绊 惑措评搏 波淋
	//if (GegeToFxIndex > LastActiveFxIndex)
	//{
	//	CurTeamEmitters[GegeToFxIndex]->GetParticleSystemComponent()->SetActive(true, true);
	//	EnemyTeamEmitters[GegeToFxIndex]->GetParticleSystemComponent()->SetActive(false, true);
	//}
	//else
	//{
	//	// 牢郸胶焊促 奴芭 波淋
	//	for (int32 nCnt = 0; nCnt < CurTeamEmitters.Num(); ++nCnt)
	//	{
	//		if (nCnt > GegeToFxIndex)
	//		{
	//			UParticleSystemComponent* pComp = CurTeamEmitters[nCnt]->GetParticleSystemComponent();

	//			pComp->SetActive(false, true);
	//			
	//			// 吝赋 酒聪绊 风俏老订 惑措祈波 难淋.
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
	// 林狼 : 付萍聪狼 PC Index绰 1何磐 矫累!!
	// 磊扁磊脚篮 公炼扒 1锅 浇吩
	if (IsMyStartIdx)
		return 1;

	uint8 PCPlayStartIdx = 2; // 颇萍盔篮 2何磐 矫累
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

		// PCPlayStartIdx啊 捞傈俊 悸泼等利捞 绝栏搁 府畔
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

// 郴啊 救俊 柳涝秦乐绊 痢飞惑怕啊 快府评波牢啊
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
	// 规唱啊扁? 概莫肯丰饶浚 唱啊扁 扁瓷捞 绝促窃.
	// LeaveRoomIfJoined();

	//FLobbySceneManager::DeferredRegistChangeLobbyScene([](){
	//	// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst) {
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterBattleMain);
	//	}
	//	//data_trader::Retailer::GetInstance().RequestGetTeamBattleStatus();
	//	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_ControlMain);
	//});

	//OpenBladeIILobbyCommon(this); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
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
	//	// 龋胶飘绰 痢飞瘤 沥焊茄锅 傈价
	//	if (m_pControlObject)
	//		m_pControlObject->BroadCastCurrentState();

	//	// 懦沥焊档 茄锅 傈价.
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

	//	// 措扁矫埃 UI俊 汲沥
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

	// 窃荐绰 促甸绢坷绰单 龋胶飘父 角青
	if (GetPeerType() != PEER_SUPER)
		return;

	// 胶迄舅覆 朝妨霖促.
	packet::ByteStream	payload;
	FString encoded_string = packet::FinalizePacket(packet::SPAWN_CONTROL_NPC, 0, 0, packet::ALL, payload);
	SendGameMessage(encoded_string);
}

// 鸥捞怪 嘎免 格利栏肺 龋胶飘档 罐绊角青 
void AB2ControlGameMode::NetReceiveSpawnControlNPC()
{
	//if (UB2UIDocHelper::GetDocControl()->GetControlType() != CONTROL_MODE_NOMINO || IsActiveControlTutorial())
	//{
	//	// 趣咯 巢酒乐栏搁 力芭饶 胶迄
	//	if (ControlNPC)
	//	{
	//		ControlNPC->Destroy();
	//		ControlNPC = nullptr;
	//	}

	//	// SpawnCharacter 夸扁鸥搁辑 ControlNPC 悸泼等促.
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
	// 俊叼磐俊辑 汲沥等 矫埃栏肺 抗距
	// 酒捞叼 -1捞搁 笛促 抗距
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

	//		// 痢飞傈 譬配府倔俊急 眠规捞唱, 鸥蜡历 盲澄唱皑 贸府救茄促.
	//		if (IsActiveControlTutorial())
	//			continue;

	//		// 辑滚肺 眠规 舅覆阑 朝赴促. 乐栏搁.
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
	// NetId绰 夸没 焊辰局

	//if (GetPeerType() != PEER_SUPER)
	//	return;
	//
	//if (!ControlNPC)
	//	return;

	//ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(FindPawnByNetId(NetId));

	//if (!pPlayer)
	//	return;
	//
	//// 酒流 倾啊茄利 绝栏搁 倾啊.
	//bool bAllow = !ControlNPC->bAllowedMount && pPlayer->IsQTEStartAble();
	//
	//// NPC力绢 器扁(倾啊 纳捞胶父 器扁..)
	//if(bAllow)
	//	ControlNPC->DemoteNetStatus();

	//// 傈价
	//NetSendResponseQTE(NetId, bAllow);

	//// 倾啊沁绰啊 敲贰弊 柯
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
	//// NetId绰 QTE角芹且局		
	//AB2ControlModePlayer* pPawn = Cast<AB2ControlModePlayer>(FindPawnByNetId(NetId));

	//if (!pPawn)
	//	return;

	//pPawn->ResetRequestStartQTEFlag();

	//if (!bAllow)
	//	return;
	//
	//// 龋胶飘 捞寇俊绰 捞菩哦篮 罐篮矫痢俊 固畴啊 QTE啊瓷惑怕啊 酒匆荐档 乐促.
	//// 啊瓷惑怕锭 角青窍霸 抗距吧绢仇
	//if (!ControlNPC->IsTimingForQTEPossession())
	//{
	//	ReservedQTEPlayerNetId = NetId;
	//	return;
	//}

	//// 郴啊 鸥绰芭搁 力绢鼻何磐 裙垫
	//if (pPawn->IsLocalPlayer() && ControlNPC)
	//{
	//	ControlNPC->PromoteNetStatus();
	//}

	//// 夸没窍绊 唱辑 cc吧啡阑版快档 措厚
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
	//	// QTE郴府搁辑 磷老荐 乐霸.
	//	if (ControlNPC)
	//		ControlNPC->SetRemoteDyingSignal();

	//	pPawn->SuperStopQTEMount();

	//	// 夸扁辑 敲饭捞绢 HP官 焊捞备. 固畴鸥快福胶 巢篮矫埃 霸捞瘤 力芭.
	//	GetUIManager_InGameCombat()->HideFloatingHPBarForPlayer(pPawn, false);

	//	// BP俊辑 伎橇 矫埃眉农 & 叼胶飘肺捞看绰单 捞力 菩哦 罐备档 力芭秦淋
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
	//	// 盲泼 芒摧绊 酿鸥烙 倒覆
	//	pUI->NotifySendControlChat();
	//}

	////傈价
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
	//	// 笛捞 鞍捞甸绢柯促绰 焊厘 绝澜
	//	// 抄涝 纳捞胶
	//	//else
	//	//{
	//	//	// 抄涝矫 鞍捞甸绢柯 惑措评 捞抚.
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
	////捞亥飘官牢靛
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
	//// 痢飞傈 譬配府倔俊急 捞鸥捞怪俊 肺厚肺~ 肺爹!
	//if (IsActiveControlTutorial())
	//{
	//	// 乞家浚 痢飞傈 沥焊(GetAssaultBattleStatus) 技记菩哦 夸没秦辑 罐绊 贸府窍绰单 咯扁急 傲 肺厚肺.
	//	ReturnToControlMainMenu();
	//	return;
	//}

	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::ControlMvpView);

	//SceneManager.SetState(EControlMatchSceneState::EndMatch);

	////GetWorld()->GetTimerManager().SetTimer(WaitFinalResultTimeHandle, this, &AB2ControlGameMode::SetResultScene, WaitFinalResultTime, false);

	////迄促见辫
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

	if (ResultWinTeam == ETeamType::ETT_End) // 公铰何
	{
		PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_ControlBattleVictory);
	}
	else if (ResultWinTeam == MyTeam) //铰府
	{
		PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_ControlBattleVictory);
	}
	else //菩硅
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
	// 痢飞傈篮 MainClass父 SkillAnim Load
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
	
	// 评滚橇 吧副牢盔 急沥矫 悼扁拳甫困秦 齿酒捞叼鉴辑肺 沥纺
	OutList.Sort([](const ABladeIICharacter& A, const ABladeIICharacter& B) { return B.GetNetId() > A.GetNetId(); });
}

// ============================== FControlMatchSceneManager
void FControlMatchSceneManager::SetGameMode(class AB2ControlGameMode* InGameMode)
{
	GameMode = InGameMode;

	if (GameMode)
	{ // 饭骇 府家胶 匙捞萍宏 饭欺繁胶 某教 鸥捞怪.
		GameMode->RecacheLevelResources();
	}
}

void FControlMatchSceneManager::SetMatchLobbyResources(class ALevelSequenceActor * InResultMatinee,
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

	//// 譬配府倔老锭 贰叼, 措扁纠俊急 炼捞胶平贸府 救茄促.(譬配府倔咀记俊辑 贸府)
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

		// 矫埃 眉农救窃. 矫埃 公力茄
		//if (fElapseTime >= GameMode->InGameLimitedTime)
		//{
		//	if (GameMode->GetPeerType() != PEER_SUPER || GameMode->bSendEndMatch)
		//		return;
		//
		//	// 铰菩眉农 鞘夸窍搁 咯扁眠啊
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

	//// 饭骇BP俊辑 悸泼 救沁阑荐档 乐澜
	//check(IntroEventDirector);

	//IntroEventDirector->CustomBeginShow();
}

void FControlMatchSceneManager::SetReady()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::ControlBattle);

	//if (IsActiveControlTutorial())
	//{
	//	// 眠啊贸府绰 TutorialManager - ACTION_EXCEPTION俊辑
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
	// SetEndMatch 俊辑 StageEventSpecificScalability 甫 流立 持绰 纳捞胶啊 乐绢辑 寸厘 鞘夸己捞 焊捞瘤绰 臼瘤父 娄嘎勉 侩栏肺 持绢狄. 
	// 捞率 楷免捞 StageEventDirector 绝捞 喊档 备泅捞 登促 焊搁 捞芭 捞傈俊 绢恫 捞蜡肺电 喊档狼 楷免 傈侩 scalability 啊 利侩等 盲 瘤加瞪 荐啊 乐绢辑 府悸阑 持绢林绰 霸 亮扁档 窍绊.
	//B2Scalability::ApplyStageEventSpecificScalabilitySetting(GameMode, false);

	//GameMode->SetActiveControlObject(true);

	//GameMode->ReserveSpawnControlNPC(true);

	//GameMode->ReserveRespawnControlSanctuary();

	//
	//B2GMLoadingProgCollector::StepToMax(); // 肺爹 柳青档 捞码辑 菜 盲况持澜.
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::ControlBattle);

	//ControlGamePlayStateTime = GameMode->GetWorld()->GetTimeSeconds();

	//UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);

	//if (pUI)
	//{
	//	pUI->PlayAnimationEvent_AnimOpenLock();
	//}

	//// 眠啊矫埃 困连 积己
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
	//// 捞变评 or 厚板栏搁 快府评 茫酒辑 欺脐栏肺 积己秦拎扼
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

	//// StageEventDirector 甫 烹茄 厘搁捞搁 楷免 傈侩 scalability adjust啊 利侩捞 登绰单 咯变 蝶肺.
	//// 老馆利栏肺 Scalability 利侩 咯何啊 滚弊 惑炔阑 父甸绢郴瘤绰 臼瘤父 谍 巴焊促 ViewDistanceBias 惑氢捞 利侩登瘤 臼栏搁 户操墩赴 寇包捞 唱棵 荐 乐绢辑 漂喊洒 持绢狄.
	//// 霸烙葛靛 唱啊扁 傈捞扼 登倒副 鞘夸绰 绝瘤父 且 荐 乐促搁 娄篮 嘎苗 林绰 霸 亮瘤
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
