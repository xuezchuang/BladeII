#include "B2GuildGameMode.h"
//#include "BladeII.h"

#include "Event.h"

#include "B2GameInstance.h"

//#include "BladeIIPlayerAIController.h"
#include "BladeIIPlayerController.h"
//#include "B2GuildNPCAIController.h"
//#include "B2ActiveCameraActor.h"
//#include "B2NPCClassInfoBox.h"

#include "B2UIDocHelper.h"
//#include "B2UIManager.h"
#include "BladeIIUtil.h"
#include "B2PCClassInfoBox.h"
////#include "Matinee/MatineeActor.h"
//#include "Matinee/InterpGroup.h"
//#include "Matinee/InterpGroupInst.h"
//#include "Matinee/InterpTrackMove.h"

//#include "B2GuildNPCInfo.h"
#include "Retailer.h"
//#include "BladeIIGMAIPlayer.h"
//#include "B2UIGuildBattle.h"
//#include "B2GeneralBuffs.h"
//#include "BladeIIMercenaryPlayer.h"
//#include "B2UIGuildRound.h"


#include "BladeIIGameImpl.h"
#include "B2UIDocGuild.h"

#include "AIController.h"

namespace GuildMatchState
{
	const FName ReadyToRound = FName(TEXT("ReadyToRound"));
	const FName ProgressRound = FName(TEXT("ProgressRound"));
}

FMoveToCompleteInfo::FMoveToCompleteInfo()
{
	for (int8 Index = (int8)ETeamIndex::Player; Index < (int8)ETeamIndex::TeamIndex_Max; Index++)
	{
		ETeamIndex TeamIndex = (ETeamIndex)Index;
		MoveReqeustCount.Add(TeamIndex, 0);
		MoveRequestMap.Add(TeamIndex, TMap<uint32, AAIController*>());
	}
}

void FMoveToCompleteInfo::SetMoveReqeustCount(ETeamIndex InTeamIndex, int32 Count)
{
	MoveReqeustCount[InTeamIndex] = Count;
	MoveRequestMap[InTeamIndex].Empty();
}

void FMoveToCompleteInfo::AddMoveRequestID(ETeamIndex InTeamIndex, uint32 RequestID, AAIController* AIController)
{
	MoveRequestMap[InTeamIndex].Add(RequestID, AIController);
}

//bool FMoveToCompleteInfo::ReceiveRequestID(uint32 RequestID, EPathFollowingResult::Type Result, ETeamIndex & ComplateTeam, AAIController** OutController)
//{
//	ComplateTeam = ETeamIndex::TeamIndex_Max;
//	
//	for (int8 Index = (int8)ETeamIndex::Player; Index < (int8)ETeamIndex::TeamIndex_Max; Index++)
//	{
//		ETeamIndex TeamIndex = (ETeamIndex)Index;
//		if (AAIController** AIController = (MoveRequestMap[TeamIndex]).Find(RequestID))
//		{
//			if (OutController)
//				(*OutController) = *AIController;
//			MoveRequestMap[TeamIndex].Remove(RequestID);
//			if (--MoveReqeustCount[TeamIndex] == 0)
//			{
//				ComplateTeam = TeamIndex;
//				return true;
//			}
//		}
//	}
//	return false;
//}


#if WITH_EDITOR
void AB2GuildGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	SelectedMercenaryIDList.Empty();


	if (UGameplayStatics::HasOption(Options, TEXT("mercenary")))
	{
		FString RawOptionString = UGameplayStatics::ParseOption(Options, TEXT("mercenary"));
		TArray<FString> ParsedOptionStringArray;
		RawOptionString.ParseIntoArray(ParsedOptionStringArray, TEXT(","));
		for (FString MercenaryID : ParsedOptionStringArray)
			SelectedMercenaryIDList.Add(FCString::Atoi(*MercenaryID));
	}
	IsTestMode = UGameplayStatics::HasOption(Options, TEXT("test"));
}

void AB2GuildGameMode::CreateDummyGuildTeamInfo()
{
	//
	const int32 Dev_Mercenary_Min_ID = 10001;
	const int32 Dev_Mercenary_Max_ID = 10004;
	for (int8 TeamIndex = (int8)ETeamIndex::Player; TeamIndex < (int8)ETeamIndex::TeamIndex_Max; ++TeamIndex)
	{
		ETeamIndex CurrentTeamIndex = (ETeamIndex)TeamIndex;
		TArray<EPCClass> RandomSelectPC = { EPCClass::EPC_Fighter, EPCClass::EPC_Gladiator,EPCClass::EPC_Wizard,EPCClass::EPC_Assassin };
		MatchTeamInfos.Add(CurrentTeamIndex);
		//2扼款靛
		for (int32 i = 0; i < 2; i++)
		{
			int32 DocIndex = 0;
			FGuildTeamInfo GuildTeamInfo;
			GuildTeamInfo.TeamIndex = CurrentTeamIndex;
		
			//2某腐磐 究
			for (int32 j = 0; j < 2; j++)
			{
				int32 SelectIndex = 0;// FMath::CeilToInt(FMath::RandRange(0, RandomSelectPC.Num() - 1));
				EPCClass SelectPC = RandomSelectPC[SelectIndex];
				RandomSelectPC.RemoveAt(SelectIndex);

				FGuildPlayerInfo CharInfo(GetFormationSlotPosition(DocIndex), SelectPC, DocIndex, 1, (int8)GetForamtionNUM(DocIndex));
				++DocIndex;
				GuildTeamInfo.PlayersInfos.Add(CharInfo);
			}
			//侩捍 2某腐磐 究
			for (int32 j = 0; j < 2; j++)
			{
				int32 MercenaryID = Dev_Mercenary_Max_ID;
				if (SelectedMercenaryIDList.Num() == 0)
					MercenaryID = FMath::RandRange(Dev_Mercenary_Min_ID, Dev_Mercenary_Max_ID);
				else
					MercenaryID = SelectedMercenaryIDList[FMath::RandRange(0, SelectedMercenaryIDList.Num() - 1)];

				FGuildMercenaryInfo CharInfo(MercenaryID, 20,
					GetFormationSlotPosition(DocIndex), DocIndex, 1, (int8)GetForamtionNUM(DocIndex));
				++DocIndex;
				GuildTeamInfo.MercenaryInfos.Add(CharInfo);
			}
			MatchTeamInfos[CurrentTeamIndex].Add(GuildTeamInfo);
		}
	}
}
#endif

AB2GuildGameMode::AB2GuildGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	DefaultPawnClass			= ASpectatorPawn::StaticClass();
	PlayerControllerClass		= ABladeIIPlayerController::StaticClass();
	
	MatchTeamInfos.Empty();

	CurrentRound.RoundNumber = 0;
	for (int8 Index = (int8)ETeamIndex::Player; Index < (int8)ETeamIndex::TeamIndex_Max; Index++)
	{
		ETeamIndex TeamIndex = (ETeamIndex)Index;
		CurrentRound.TeamInfoIndexInRound.Add(TeamIndex, -1);
		CurrentRound.WinRoundCount.Add(TeamIndex, 0);
	}
	CurrentRound.WinnerTeam = ETeamIndex::TeamIndex_Max;

	RemainBattleTime = 0;
	CacheGuildDoc = nullptr;

	PCCache.ClearCache();
	IsNPCSkillUsing = MakeShared<FNPCUsingSkillState>();
	IsNPCSkillUsing->bUsingSkill = false;
	WaitReadyToRoundTime = 4.0f;
	ChangeCameraTime = 10.0f;
	bExcutedFinishBattle = false;
}

void AB2GuildGameMode::CreateGuildTeamInfo()
{
	UB2UIDocGuild* GuildDoc = GetGuildDoc();
	check(GuildDoc)

	for (int8 TeamIndex = (int8)ETeamIndex::Player; TeamIndex < (int8)ETeamIndex::TeamIndex_Max; ++TeamIndex)
	{
		ETeamIndex CurrentTeamIndex = (ETeamIndex)TeamIndex;

		b2network::B2GuildBattleEntryPtr BattleEntrys;
		if (CurrentTeamIndex == ETeamIndex::Player)
			BattleEntrys = GuildDoc->GetAllyGuildMemeberEntry();
		else
			BattleEntrys = GuildDoc->GetOtherGuildMemeberEntry();

		MatchTeamInfos.Add(CurrentTeamIndex);
		uint32 Count = 0;

		FGuildTeamInfo TeamInfo;
		TeamInfo.TeamIndex = CurrentTeamIndex;
		MatchTeamInfos[CurrentTeamIndex].Init(TeamInfo, BattleEntrys->slots.Num() / ROUND_PER_MAX_NUMBER);
		
		for (b2network::B2GuildBattleCharacterSlotPtr Slot : BattleEntrys->slots)
		{
			int32 FinalSlotNum = ((Slot->slot_num - 1) % ROUND_PER_MAX_NUMBER) + 1;
			int32 RoundIndex = ((Slot->slot_num - 1) / ROUND_PER_MAX_NUMBER);
			
			check(MatchTeamInfos[CurrentTeamIndex].IsValidIndex(RoundIndex));

			FGuildTeamInfo& GuildTeamInfo = MatchTeamInfos[CurrentTeamIndex][RoundIndex];

			if (Slot->entry_type == b2network::B2GuildBattleEntryType::CHARACTER)
			{
				FGuildPlayerInfo CharInfo(FinalSlotNum, SvrToCliPCClassType(Slot->entry_code), FinalSlotNum - 1, 1, (int8)GetForamtionNUM(FinalSlotNum - 1));
				GuildTeamInfo.PlayersInfos.Add(CharInfo);
			}
			else if(Slot->entry_type == b2network::B2GuildBattleEntryType::MERCENARY)
			{
				FGuildMercenaryInfo CharInfo(Slot->entry_code, Slot->level, FinalSlotNum, FinalSlotNum - 1, 1, (int8)GetForamtionNUM(FinalSlotNum - 1));
				GuildTeamInfo.MercenaryInfos.Add(CharInfo);
			}
			else
			{
				//Error Log
			}
		}
	}
}

void AB2GuildGameMode::BeginPlay()
{
	//Super::BeginPlay();

	//StageBeginClass<>::GetInstance().Signal();
	//
	//SetBaseActiveCamera();
}

void AB2GuildGameMode::StartPlay()
{
	auto GuildDoc = GetGuildDoc();
#if WITH_EDITOR
	if (!IsTestMode)
	{
#endif
		OpponentTeamCharacterDataStore.SetModPlayerInfo(GuildDoc->GetOtherGuildMemeberCharInfo());
		MyTeamCharacterDataStore.SetModPlayerInfo(GuildDoc->GetAllyGuildMemeberCharInfo());
#if WITH_EDITOR
	}
#endif


#if WITH_EDITOR
	if (IsTestMode)
		CreateDummyGuildTeamInfo();
	else
#endif
		CreateGuildTeamInfo();

	InitGuildDoc();

	Super::StartPlay();
}

void AB2GuildGameMode::PreBeginPlay()
{
	//auto* GameRule = new GuildGameRule;
	//SetGameRule(GameRule);

	//Super::PreBeginPlay();

	//SetMatchState(MatchState::WaitingToStart);
}

void AB2GuildGameMode::HandleMatchIsWaitingToStart()
{
	SetMatchState(MatchState::InProgress);
}

void AB2GuildGameMode::HandleMatchHasStarted()
{
//	GEngine->BlockTillLevelStreamingCompleted(GetWorld());
//
//	GetWorldSettings()->NotifyBeginPlay();
//
//	// Then fire off match started
//	GetWorldSettings()->NotifyMatchStarted();
//
//	auto* PlayerController = UGameplayStatics::GetLocalPlayerController(this);
//	if (PlayerController)
//		PlayerController->ActivateTouchInterface(nullptr);
//
//#if BII_STATS // PerfStat 篮 MatchStart 矫痢何磐 葛栏扁 矫累.
//	PerfStatCollector.OnBegin(this);
//#endif
//
//	//固府 Spawn 矫难初澜
//	SetNextRound();
//
//	// 楷免矫累
//	StartEventScene_GameBegin();
//	PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_GuildBattleCombat, true);
}

void AB2GuildGameMode::HandleStartResult()
{
	int32 MatchResult = CurrentRound.WinnerTeam == ETeamIndex::Player ? b2network::B2MatchResultType::BMRT_WIN
		   															  : b2network::B2MatchResultType::BMRT_LOSE;
	
	UE_LOG(LogBladeII, Log, TEXT("Guild Game End :HandleStartResult"));
	
	// 搬苞 沥焊甫 UI 率俊 技泼窍绰 何盒篮 备泅 沥府登搁 茄 镑栏肺
	auto* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome)
		DocSome->SetLocalPlayerMatchResult(SvrToCliNetMatchResult(MatchResult));
	
	if (!GetGuildDoc())
		return;
	
#if WITH_EDITOR
	if(!IsTestMode)
#endif
	data_trader::Retailer::GetInstance().RequestFinishGuildBattle(GetGuildDoc()->GetOtherGuildMemeberAccountId(),
																 GetGuildDoc()->GetBattleToken(), MatchResult, CurrentRound.WinRoundCount[ETeamIndex::Player]);

#if WITH_EDITOR
	if (IsTestMode)
	{
		GetGuildDoc()->SetBattleResultMedal(FMath::RandRange(0,4));
		StartEventScene_MatchResult(CurrentRound.WinnerTeam == ETeamIndex::Player);
	}
#endif
	
}

void AB2GuildGameMode::HandleFinishBattle()
{
	//SetBaseActiveCamera();
	////胶懦 给静霸 函版
	//IsNPCSkillUsing->bUsingSkill = true;
	////buff tick 救倒档废 荐沥
	//SetBuffTick(false);
	////檬扁拳
	//InitDilation();
	//ETeamIndex LoseTeam = CurrentRound.WinnerTeam == ETeamIndex::Player ? ETeamIndex::Enemy : ETeamIndex::Player;
	////Statstics 鞘夸且 淀 窃
	//
	//SweepingTeam(CurrentRound, CurrentRound.WinnerTeam);
	//SweepingTeam(CurrentRound, LoseTeam, true);

	////next 评捞 绝栏搁 场郴滚覆
	//if (!HaveNextTeam(LoseTeam))
	//{
	//	SetMatchState(AsyncMatchState::StartResult);
	//	return;
	//}

	//ClearAllDebuff(CurrentRound.WinnerTeam);
	//ResetSkillCoolTime(CurrentRound.WinnerTeam);
	//TeamPlayerMoveToBattlePosition(CurrentRound.WinnerTeam);
	//bExcutedFinishBattle = false;
}

void AB2GuildGameMode::HandleReadyToRound()
{
	if (CurrentRound.RoundNumber != 1)
	{
		SetBaseActiveCamera();
	
		ShowRoundUI(true, WaitReadyToRoundTime);

		//victory anim
		CallSpawnAnim(ETeamIndex::Player);
		CallSpawnAnim(ETeamIndex::Enemy);

		GetWorldTimerManager().ClearTimer(ReadyToRoundTimer);
		GetWorldTimerManager().SetTimer(ReadyToRoundTimer, this, &AB2GuildGameMode::OnReadyToRoundTime, WaitReadyToRoundTime);
	}
	else
	{
		SetBaseActiveCamera();
		SetMatchState(GuildMatchState::ProgressRound);
	}
}

void AB2GuildGameMode::HandleProgressRound()
{
	StartRound();
}



void AB2GuildGameMode::KillAllEnemy()
{
	KillAllTeam(ETeamIndex::Enemy);
}

void AB2GuildGameMode::KillAllAlly()
{
	KillAllTeam(ETeamIndex::Player);
}

void AB2GuildGameMode::KillAllTeam(ETeamIndex InTeamIndex)
{
	/*for (AAIController* Controller : CurrentRound.GetTeamControllerCache(InTeamIndex))
	{
		if (Controller && Controller->IsValidObj())
		{
			ABladeIICharacter* Enemy = Cast<ABladeIICharacter>(Controller->GetPawn());
			if (Enemy && Enemy->IsValidObj())
				Enemy->SetDieForce();
		}
	}*/
}

void AB2GuildGameMode::SetRemainBattleTime(float InTime)
{
	RemainBattleTime = InTime;
}

void AB2GuildGameMode::StartEventScene_MatchResult(bool HasLocalPlayerWon)
{
	////BGM 绝匙
	////捞亥飘 叼泛磐俊档 鸥涝捞 绝匙 搬苞芒阑 毫具窍聪鳖 老窜 持澜
	////UB2UIManager::GetInstance()->SetHUDHidingCinematicMode(EUIScene::GuildResult);
	//ECommonSoundID PlaySoundID = ECommonSoundID::ECSID_BGM_GuildBattleDefeat;
	//if (HasLocalPlayerWon)
	//	PlaySoundID = ECommonSoundID::ECSID_BGM_GuildBattleVictory;
	//
	//PlayBGMByCommonSoundID(PlaySoundID, true);
	//Super::StartEventScene_MatchResult(HasLocalPlayerWon);
}

void AB2GuildGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	if (MatchState == AsyncMatchState::StartResult)
	{
		HandleStartResult();
	}
	else if (MatchState == AsyncMatchState::FinishBattle)
	{
		HandleFinishBattle();
	}
	else if (MatchState == GuildMatchState::ReadyToRound)
	{
		HandleReadyToRound();
	}
	else if (MatchState == GuildMatchState::ProgressRound)
	{
		HandleProgressRound();
	}
}

void AB2GuildGameMode::NotifyCharacterDead(ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	//Super::NotifyCharacterDead(TargetCharacter, PawnInstigator, DamageCauser);

	//// 泅犁 咀萍宏墨皋扼 轰鸥百捞 磷菌阑锭 扁夯 咀萍宏墨皋扼肺 倒覆
	//auto* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (B2PC && TargetCharacter)
	//{
	//	if (TargetCharacter == B2PC->GetViewTarget())
	//	{
	//		//促澜 鸥百..
	//		OnChangeCamera();
	//	}
	//}

	//auto nAllyCount = NumberOfAliveCharacter((ETeamIndex)TargetCharacter->GetTeamNum());

	//if (nAllyCount != 0)
	//	return;

	//ABladeIICharacter* Inflictor = nullptr;
	//// 捧荤眉老荐档 乐绢辑 笛促眉农
	//if (PawnInstigator)
	//	Inflictor = Cast<ABladeIICharacter>(PawnInstigator);
	//else
	//	Inflictor = Cast<ABladeIICharacter>(DamageCauser);

	//if (Inflictor == nullptr && DamageCauser)
	//	Inflictor = Cast<ABladeIICharacter>(DamageCauser->GetOwner());


	//FinishBattlePresentation(false, Inflictor);
}

ABladeIICharacter * AB2GuildGameMode::GetHighAggroOpponentCharacter(ABladeIICharacter* OwnerPawn, int32 InMyTeamNumber, FVector CurrentPawnLocation)
{
	/*ETeamIndex OhterTeam = InMyTeamNumber == GetMyTeamNum() ? ETeamIndex::Enemy : ETeamIndex::Player;
	IGuildPawnInterface* BestAggroCharacter = nullptr;

	if (OwnerPawn)
	{
		UB2Buff_Base* Taunt_Debuff = OwnerPawn->GetBuff(EBuffType::Debuff_Taunt);
		if (Taunt_Debuff && Taunt_Debuff->IsActive())
		{
			ABladeIICharacter* TauntCharacter = Cast<ABladeIICharacter>(Taunt_Debuff->GetBuffCauser());
			if (TauntCharacter && TauntCharacter->IsValidObj() && TauntCharacter->IsAlive())
				return TauntCharacter;
		}
	}

	for (AAIController* AIController : CurrentRound.GetTeamControllerCache(OhterTeam))
	{
		if (AIController&& AIController->IsValidObj() && AIController->GetPawn())
		{
			IGuildPawnInterface* pawn = Cast<IGuildPawnInterface>(AIController->GetPawn());

			if(!pawn->PawnIsAlive())
				continue;

			if (BestAggroCharacter == nullptr)
			{
				BestAggroCharacter = pawn;
				continue;
			}

			if((CurrentPawnLocation - BestAggroCharacter->GetLocation()).SizeSquared() > (CurrentPawnLocation - pawn->GetLocation()).SizeSquared())
				BestAggroCharacter = pawn;
		}
	}*/
	return /*BestAggroCharacter ? BestAggroCharacter->GetPawnInstance() :*/ nullptr;
}

void AB2GuildGameMode::Tick(float DeltaSeconds)
{
	////this->TakeDamageUpdate();

	//if (IsInPreRenderPhase()) {
	//	UpdateForPreRenderObjects(); // 捞霸 倒酒啊绰 客吝俊绰 酒流 沥侥 矫累 救 茄 芭..
	//								 // Pre-render 俊辑 皋牢 甘 肺爹 拳搁 摧绰 吧 瘤楷矫虐霸 登骨肺 咯扁辑 贸府. Pre-render 侩 肺爹 拳搁篮 蝶肺 乐绊 捞扒 窜瘤 肺爹 槐臂捞 倒酒啊绰 芭 楷厘窍扁 困茄 瞒盔捞促.
	//	ConditionalWaitForLoadingScreen();
	//}

	//if (GetMatchState() == GuildMatchState::ProgressRound && !bExcutedFinishBattle)
	//{
	//	RemainBattleTime -= DeltaSeconds;

	//	if (RemainBattleTime < 0)
	//	{
	//		RemainBattleTime = 0.0f;
	//		FinishBattlePresentation(true);
	//	}
	//	if (GetGuildDoc())
	//		GetGuildDoc()->SetBattleTime(RemainBattleTime);
	//}
}

void AB2GuildGameMode::SubscribeEvents()
{
	//if (bEventsSubscribed)
	//	return;

	//Super::SubscribeEvents();

	//Issues.Add(GuildMatchChangeCharacterCamClass<bool, int32>::GetInstance().Subscribe2(
	//	[this](bool bMyTeam, int32 SlotIdx)
	//	{
	//		//TODO UI累诀
	//		//this->ChangeCharacterCamera(bMyTeam, SlotIdx);
	//	}
	//));
	//Issues.Add(DeliveryFinishGuildBattleClass<FB2ResponseFinishGuildBattlePtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseFinishGuildBattlePtr& GuildFinish)
	//	{
	//		auto* GuildDoc = GetGuildDoc();
	//		check(GuildDoc)

	//		GuildDoc->SetBattleResultMedal(GuildFinish->obtain_medal);
	//		GuildDoc->SetBattleResultReward(GuildFinish->rewards);

	//		bool BattleResult = GuildFinish->battle_result == b2network::B2MatchResultType::BMRT_WIN;
	//		this->StartEventScene_MatchResult(BattleResult);
	//	}
	//));
	//Issues.Add(ReturnToGuildMainClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->ReturnToGuildMainMenu();
	//	}
	//));
	//Issues.Add(MatchGameStartEventSceneEndClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	this->MatchGameStartEventSceneEnd();
	//}
	//));

	//Issues.Add(StageEventDirectorFinishShowEventClass<int32>::GetInstance().Subscribe2(
	//	[this](int32 EventType)
	//{
	//	this->NotifyStageEventDirector_FinishShow(EventType);
	//}
	//));

	//Issues.Add(UseGuildNPCSkillClass<bool>::GetInstance().Subscribe2(
	//	[this](bool bUsing)
	//{
	//	if (IsNPCSkillUsing.IsValid())
	//		IsNPCSkillUsing->bUsingSkill = bUsing;
	//}
	//));

	//Issues.Add(ForceFocusViewTargetClass<ABladeIIPlayer*, float>::GetInstance().Subscribe2(
	//	[this](ABladeIIPlayer* ForcusPlayer, float BlendTime)
	//{
	//	if (!ForcusPlayer)
	//	{
	//		SetBaseActiveCamera();
	//	}
	//	else if (ForcusPlayer && ForcusPlayer->IsValidObj())
	//	{
	//		ChangePlayerActiveCamera(false ,ForcusPlayer, BlendTime);
	//	}
	//}
	//));

	//Issues.Add(GuildGameChangeCameraClass<bool, ABladeIIPlayer*>::GetInstance().Subscribe2(
	//	[this](bool bShort, ABladeIIPlayer* ForcusPlayer)
	//{
	//	ChangePlayerActiveCamera(false, ForcusPlayer, -1.0f);
	//}
	//));
}

void AB2GuildGameMode::SetupUIScene()
{
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::GuildMatchBattle);
}

void AB2GuildGameMode::GetAllWorldMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs)
{
	GetAllMyTeamCharList(OutList, GetMyTeamNum(), bOnlyLiveMobs);
	GetAllMyTeamCharList(OutList, GetOpponentTeamNum(), bOnlyLiveMobs);
}

void AB2GuildGameMode::GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 InMyTeamNum, bool bOnlyLiveMobs)
{
	GetAllMyTeamCharList(OutList, GetOpponentTeamNum(), bOnlyLiveMobs);
}

void AB2GuildGameMode::GetAllMyTeamCharList(TArray<class ABladeIICharacter*>& OutList, int8 InMyTeamNum, bool bOnlyLive)
{
	ETeamIndex TeamIndex = (ETeamIndex)InMyTeamNum;
	for (AAIController* AIController : CurrentRound.GetTeamControllerCache(TeamIndex))
	{
		if (AIController /*&& AIController->IsValidObj()*/)
		{
			ABladeIICharacter* Character = Cast<ABladeIICharacter>(AIController->GetPawn());
			if (Character && (!bOnlyLive || Character->IsAlive()))
				OutList.Add(Character);
		}
	}
}

int32 AB2GuildGameMode::NumberOfAliveCharacter(ETeamIndex InTeamIndex)
{
	TArray<class ABladeIICharacter*> OutList;
	GetAllMyTeamCharList(OutList, (int8)InTeamIndex, true);

	return OutList.Num();
}

float AB2GuildGameMode::GetTotalRemainHPPercent(ETeamIndex InTeamIndex)
{
	TArray<class ABladeIICharacter*> OutList;
	GetAllMyTeamCharList(OutList, (int8)InTeamIndex, true);

	float TotalPercent = 0.0f;
	for (ABladeIICharacter* character : OutList)
		TotalPercent += (character->GetHealth() / character->GetMaxHealth());

	return TotalPercent;
}

void AB2GuildGameMode::SetBaseActiveCamera()
{
	/*auto* GuildDOc = GetGuildDoc();
	if (!GuildDOc)
		return;

	if (BaseFixActiveCameraActor && GetCurrentActiveActiveCameraActor() != BaseFixActiveCameraActor)
	{
		BaseFixActiveCameraActor->ForceBeginActiveCamera();
		GuildDOc->SetFixedCamera(true);
		GuildDOc->SetViewTarget(nullptr);
	}*/
}

void AB2GuildGameMode::ChangePlayerActiveCamera(bool bShort, ABladeIIPlayer * ViewTarget, float BlendTime)
{
	//auto* GuildDOc = GetGuildDoc();
	//if (!GuildDOc)
	//	return;

	//if (!ViewTarget)
	//{
	//	ABladeIIPlayer* CurrentViewTarget = GuildDOc->GetViewTarget();
	//	if (!CurrentViewTarget)
	//		return;

	//	if (bShort && GetCurrentActiveActiveCameraActor() != PlayerShortActiveCameraActor)
	//		PlayerShortActiveCameraActor->ChangeCameraOwnerPlayer(CurrentViewTarget);
	//	else if (!bShort && GetCurrentActiveActiveCameraActor() != PlayerLongActiveCameraActor)
	//		PlayerLongActiveCameraActor->ChangeCameraOwnerPlayer(CurrentViewTarget);
	//}
	//else
	//{
	//	if (!ViewTarget->IsAlive())
	//		return;

	//	if (bShort && PlayerShortActiveCameraActor)
	//		PlayerShortActiveCameraActor->ChangeCameraOwnerPlayer(ViewTarget, BlendTime);
	//	else if (!bShort && PlayerLongActiveCameraActor)
	//		PlayerLongActiveCameraActor->ChangeCameraOwnerPlayer(ViewTarget, BlendTime);

	//	GuildDOc->SetViewTarget(ViewTarget);
	//}

	//GuildDOc->SetFixedCamera(GetCurrentActiveActiveCameraActor() == BaseFixActiveCameraActor);
	//GuildDOc->SetShortCamera(GetCurrentActiveActiveCameraActor() == PlayerShortActiveCameraActor);
}

bool AB2GuildGameMode::IsShortActiveCamera()
{
	//return GetCurrentActiveActiveCameraActor() == PlayerShortActiveCameraActor;
	return true;
}

void AB2GuildGameMode::ReturnToGuildMap()
{	
	//FLobbySceneManager::DeferredRegistChangeLobbyScene(
	//	[]() 
	//	{
	//		// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
	//		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//		if (UIMgrInst) {
	//			UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//		}
	//		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_Guild);
	//		data_trader::Retailer::GetInstance().RequestGetGuildBattleInfo();
	//	}
	//);

	//OpenBladeIILobbyCommon(this); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
}

void AB2GuildGameMode::TeamPlayerMoveToBattlePosition(ETeamIndex InTeamIndex)
{
	//int32 AliveCount = NumberOfAliveCharacter(InTeamIndex);

	//if (AliveCount <= 0)
	//	return;

	//MoveToCompleteInfo.SetMoveReqeustCount(InTeamIndex, AliveCount);

	//for (AAIController* AIController : CurrentRound.GetTeamControllerCache(InTeamIndex))
	//{
	//	if (AIController && AIController->IsValidObj())
	//	{
	//		IGuildPawnInterface* pawn = Cast<IGuildPawnInterface>(AIController->GetPawn());
	//		if (pawn && pawn->PawnIsAlive())
	//		{
	//			FString SpotTagName = pawn->GetPlayerStartName();

	//			if (AActor* StartSpot = FindPlayerStart(AIController, SpotTagName))
	//			{
	//				AIController->StopMovement();

	//				FAIMoveRequest MoveReq(StartSpot->GetActorLocation());
	//				MoveReq.SetUsePathfinding(true);
	//				MoveReq.SetAllowPartialPath(true);
	//				MoveReq.SetAcceptanceRadius(20.0f);
	//				MoveReq.SetReachTestIncludesAgentRadius(false);
	//				MoveReq.SetNavigationFilter(AIController->GetDefaultNavigationFilterClass());
	//				MoveReq.SetCanStrafe(true);

	//				//MoveTo浚柳内靛俊辑 Character甫 啊廉客辑 促矫 AcceptableRadius甫 犁汲沥 窍绰瘤 捞蜡甫 葛福摆澜...
	//				//绢露电 MoveTo甫 荐沥窍霸登搁 绢恫 荤捞靛捞棋飘啊 磐龙瘤 隔扼 咯扁辑父 贸府..
	//				ABladeIICharacter* Character = Cast<ABladeIICharacter>(AIController->GetPawn());
	//				if (Character)
	//				{
	//					Character->AIAttackMoveAcceptableRadius = 0.0f;
	//					Character->SetMoveCollision(false);
	//				}

	//				AIController->ReceiveMoveCompleted.AddDynamic(this, &AB2GuildGameMode::OnMoveToComplete);
	//				FPathFollowingRequestResult MoveResult = AIController->MoveTo(MoveReq);
	//				MoveToCompleteInfo.AddMoveRequestID(InTeamIndex, MoveResult.MoveId.GetID(), AIController);
	//			}
	//		}
	//	}
	//}

}

void AB2GuildGameMode::ClearAllDebuff(ETeamIndex InTeamIndex)
{
	TArray<ABladeIICharacter*> AllCharacter;
	GetAllMyTeamCharList(AllCharacter, (int8)InTeamIndex, true);
	for (ABladeIICharacter* Character : AllCharacter)
		Character->ClearDebuffs();
}

void AB2GuildGameMode::ResetSkillCoolTime(ETeamIndex InTeamIndex)
{
	TArray<ABladeIICharacter*> AllCharacter;
	GetAllMyTeamCharList(AllCharacter, (int8)InTeamIndex, true);
	for (ABladeIICharacter* Character : AllCharacter)
	{
		ABladeIIPlayer* Player = Cast<ABladeIIPlayer>(Character);
		if (Player)
			Player->ResetSkillCooltime();
	}
}

void AB2GuildGameMode::TakeDamageUpdate()
{
	//auto* GuildDoc = UB2UIDocHelper::GetDocGuild();
	//if (!GuildDoc)
	//	return;

	//for (auto& Character : MyTeamInfo.Characters)
	//{
	//	check(Character.AIController);
	//	auto* ControlledPawn = Cast<ABladeIITMAIPlayer>(Character.AIController->GetPawn());
	//	if (!ControlledPawn)
	//		continue;

	//	GuildDoc->SetCharacterHP(true, Character.DocCharIndex, ControlledPawn->Health);
	//	GuildDoc->SetCharacterShield(true, Character.DocCharIndex, ControlledPawn->GetShield());
	//}

	//for (auto& Character : OpponentTeamInfo.Characters)
	//{
	//	check(Character.AIController);
	//	auto* ControlledPawn = Cast<ABladeIITMAIPlayer>(Character.AIController->GetPawn());
	//	if (!ControlledPawn)
	//		continue;

	//	GuildDoc->SetCharacterHP(false, Character.DocCharIndex, ControlledPawn->Health);
	//	GuildDoc->SetCharacterShield(false, Character.DocCharIndex, ControlledPawn->GetShield());
	//}
}

ABladeIIPlayer* AB2GuildGameMode::GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly /*= false*/, bool bSpawnPuppetForDead /*= false*/)
{
	//if (bSpawnPuppetForDead)
	//{
	//	EPCClass PCClass = IntToPCClass(nPlayerIndex);
	//	if (PCClass == EPCClass::EPC_End)
	//		return nullptr;

	//	TArray<ABladeIIPlayer*>* CurrentTeamCache = nullptr;
	//	if (bAllyTeam)
	//		CurrentTeamCache = &(PCCache.MyTeamPC);
	//	else
	//		CurrentTeamCache = &(PCCache.EnemyTeamPC);

	//	ABladeIIPlayer** CachePlayer = CurrentTeamCache->FindByPredicate([PCClass](const ABladeIIPlayer* Player)
	//	{
	//		if (Player && Player->IsValidObj())
	//			return Player->GetCurrentPlayerClass() == PCClass;
	//		return false;
	//	});

	//	if (CachePlayer)
	//	{
	//		(*CachePlayer)->SetActorHiddenInGame(false);
	//		return (*CachePlayer);
	//	}
	//	else
	//	{
	//		FTransform NewTransform(
	//			FRotator(0.0f, 0.0f, 0.0f),
	//			FVector(0.0f, 0.0f, 0.0f),
	//			FVector(1.0f, 1.0f, 1.0f)
	//		);

	//		ABladeIIPlayer* SpawnedPuppet = ABladeIIGameMode::SpawnPlayerCharAsPuppet(this, PCClass, NewTransform, false, ABladeIIPlayer::StaticClass());

	//		if (!bAllyTeam)
	//		{ // 窜, 霸烙葛靛俊 某教等 吧 啊廉客辑 荤侩窍妨搁 NetID 甫 持绢林绢具 窍绰单 泅犁 SetupComponentsForPartsCustomDataStore 备泅 磊眉啊 Custom 俊 吧嘎霸 某教阑 公矫窍档废 登绢乐促.
	//			SpawnedPuppet->SetupComponentsForPartsCustomDataStore(&OpponentTeamCharacterDataStore,
	//				UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(this)
	//			);
	//		}

	//		return SpawnedPuppet;
	//	}
	//}

	return Cast<ABladeIIPlayer>(GetTeamCharacter(bAllyTeam, nPlayerIndex, bLiveOnly, bSpawnPuppetForDead));
}

ABladeIICharacter * AB2GuildGameMode::GetTeamCharacter(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly, bool bSpawnPuppetForDead)
{
	//TArray<class ABladeIICharacter*> CharacterList;
	//GetAllMyTeamCharList(CharacterList, bAllyTeam ? GetMyTeamNum() : GetOpponentTeamNum(), bLiveOnly);

	//if (CharacterList.IsValidIndex(nPlayerIndex))
	//{
	//	if(CharacterList[nPlayerIndex] && CharacterList[nPlayerIndex]->IsValidObj())
	//		return CharacterList[nPlayerIndex];
	//}

	//// 捞固 磷绢辑 给 啊廉坷绰 版快..
	//if (!bLiveOnly && bSpawnPuppetForDead)
	//{
	//	return SpawnTeamCharacterAsPuppet(bAllyTeam, nPlayerIndex, false);
	//}

	return nullptr;
}

ABladeIICharacter * AB2GuildGameMode::SpawnTeamCharacterAsPuppet(bool bAllyTeam, int32 nPlayerIndex, bool bPossessByAIController)
{
	//// 努贰胶 沥焊啊 咯扁 富绊 谍 单 绝唱..
	//auto* GuildMatchDoc = UB2UIDocHelper::GetDocGuild();
	//auto CharClassToSpawn = EPCClass::EPC_End;

	////泅犁 惑怕俊急 咯扁 甸绢坷搁 救凳..
	////TODO 备泅

	return nullptr;

}

//ABladeIIMercenaryPlayer * AB2GuildGameMode::SpawnGuildNPCCharacter(UB2NPCClassInfoBox * InfoBox, ENPCClass InClass, ENPCClassVariation InVariation, FTransform & SpawnTransform, FActorSpawnParameters & SpawnParameters)
//{
//	BII_CHECK(InfoBox);
//	return Cast<ABladeIIMercenaryPlayer>(InfoBox->SpawnNPCClassInfoGuildCharacter(GetWorld(), InClass, InVariation, SpawnTransform, SpawnParameters));
//}

void AB2GuildGameMode::SpawnTeam(FGuildTeamInfo & InGuildTeamInfo)
{
	//FActorSpawnParameters ControllerSpawnInfo;
	//ControllerSpawnInfo.Instigator = NULL;
	//ControllerSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//ControllerSpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save player controllers into a map
	//ControllerSpawnInfo.bDeferConstruction = true;

	//FActorSpawnParameters PawnSpawnInfo;
	//PawnSpawnInfo.bDeferPostSpawnInitialize = true;

	//FTransform SpawnTransform(FRotator::ZeroRotator, FVector::ZeroVector);

	//CurrentRound.GetTeamControllerCache(InGuildTeamInfo.TeamIndex).Empty();

	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	BII_CHECK(B2GI)

		UB2PCClassInfoBox* PCInfoBox = B2GI->GetPCClassInfoBox();
	BII_CHECK(PCInfoBox)

	//for (auto& Player : InGuildTeamInfo.PlayersInfos)
	//{
	//	auto* NewAC = GetWorld()->SpawnActor<AB2GuildPlayerAIController>(AB2GuildPlayerAIController::StaticClass(), SpawnTransform, ControllerSpawnInfo);
	//	BII_CHECK(NewAC);
	//	UGameplayStatics::FinishSpawningActor(NewAC, SpawnTransform);

	//	auto* ControlledPawn = GetWorld()->SpawnActor<ABladeIIGMAIPlayer>(ABladeIIGMAIPlayer::StaticClass(), SpawnTransform, PawnSpawnInfo);
	//	check(ControlledPawn);

	//	ControlledPawn->InitializePCClassInfoSpawn(this, PCInfoBox->GetSingleClassInfo(Player.CharClass));
	//	ControlledPawn->SetPlayerInfo(Player);
	//	ControlledPawn->InitCombateStatsForParam(GetCombatStat(InGuildTeamInfo, Player));
	//	ControlledPawn->SetTeamNum((int32)InGuildTeamInfo.TeamIndex);
	//
	//	NewAC->SetTeamNum((int8)InGuildTeamInfo.TeamIndex);
	//	NewAC->Possess(ControlledPawn);
	//	GetWorld()->ManualPostSpawnInitialize(ControlledPawn, SpawnTransform, PawnSpawnInfo);
	//	CurrentRound.GetTeamControllerCache(InGuildTeamInfo.TeamIndex).Add(NewAC);
	//	PCCache.AddCache(InGuildTeamInfo.TeamIndex, ControlledPawn);
	//}
	//
	//UB2GuildNPCInfo* GuildNPCInfo = B2GI->GetGuildNPCInfo();
	//UB2NPCClassInfoBox* NPCInfoBox = B2GI->GetMobClassInfoBox();
	//BII_CHECK(NPCInfoBox && GuildNPCInfo)

	//FLinearColor FloorRingColor = FLinearColor(0, 0, 0, 0);
	//float FloorRingColorMultiplier = 1.0f;

	//for (FGuildMercenaryInfo& Mercenary : InGuildTeamInfo.MercenaryInfos)
	//{
	//	const FGuildNPCInfo* NPCInfo = GuildNPCInfo->GetGuildNPCInfoOfID(Mercenary.MercenaryID);
	//	check(NPCInfo)

	//	if (NPCInfo)
	//	{
	//		AB2GuildNPCAIController* NewAC = GetWorld()->SpawnActor<AB2GuildNPCAIController>(AB2GuildNPCAIController::StaticClass(), SpawnTransform, ControllerSpawnInfo);
	//		BII_CHECK(NewAC);
	//		UGameplayStatics::FinishSpawningActor(NewAC, SpawnTransform);

	//		ABladeIIMercenaryPlayer* NPCCharacter = SpawnGuildNPCCharacter(NPCInfoBox, NPCInfo->NPCClass, ENPCClassVariation::ENCV_GuildBattle, SpawnTransform, PawnSpawnInfo);
	//		BII_CHECK(NPCCharacter);

	//		int8 TeamNum = (int8)InGuildTeamInfo.TeamIndex;
	//		bool SetFloorRing = GetMyTeamNum() == TeamNum;

	//		NPCCharacter->SetMercenaryInfo(Mercenary);
	//		NPCCharacter->SetMercenaryUsingState(IsNPCSkillUsing);
	//		NPCCharacter->InitCombateStatsForParam(GetCombatStat(InGuildTeamInfo, Mercenary));
	//		NPCCharacter->InjectGameMode(this);
	//		NPCCharacter->AutoPossessAI = EAutoPossessAI::Disabled;
	//		NPCCharacter->FinishSpawning(SpawnTransform);
	//		
	//		if (GuildNPCFloorRing && SetFloorRing)
	//		{
	//			if (bOverrideFloorRingColor && FloorRingColors.Num() > TeamNum)
	//				FloorRingColor = FloorRingColors[TeamNum];

	//			if (bOverrideFloorRingColor && FloorRingColorMultipliers.Num() > TeamNum)
	//				FloorRingColorMultiplier = FloorRingColorMultipliers[TeamNum];
	//			NPCCharacter->SetFloorRingInfo(GuildNPCFloorRing, FloorRingColor, FloorRingColorMultiplier);
	//		}
	//			
	//		GetWorld()->ManualPostSpawnInitialize(NPCCharacter, SpawnTransform, PawnSpawnInfo);

	//		NewAC->SetTeamNum(TeamNum);
	//		NewAC->Possess(NPCCharacter);
	//		CurrentRound.GetTeamControllerCache(InGuildTeamInfo.TeamIndex).Add(NewAC);
	//	}
	//}
}

void AB2GuildGameMode::ResetPositionTeam(ETeamIndex InTeamIndex)
{
	// 弥檬胶迄篮 P1B_1 捞悼且镑篮 P1_1 捞繁屈侥栏肺
	// 捞悼篮 TeamPlayerMoveToStartPosition 窃荐俊辑
	FGuildTeamInfo& GuildTeamInfo = GetGuildTeamInfo(InTeamIndex);

	TArray<ABladeIICharacter*> AllLiveMyPlayers;
	GetAllMyTeamCharList(AllLiveMyPlayers, (int8)InTeamIndex, false);

	for (int index = 0; AllLiveMyPlayers.Num() > index; ++index)
	{
		IGuildPawnInterface* pawn = Cast<IGuildPawnInterface>(AllLiveMyPlayers[index]);
		if (pawn && pawn->PawnIsAlive())
		{
			FString SpotTagName = pawn->GetPlayerStartName();
			SetFormationPosition(pawn->GetPawnController(), SpotTagName);
		}
	}
}

EFormationNUM AB2GuildGameMode::GetForamtionNUM(int32 Index)
{
	switch (Index)
	{
	case 0:
		return EFormationNUM::Center;
	case 1:
		return EFormationNUM::Back;
	case 2:
		return EFormationNUM::Front;
	case 3:
		return EFormationNUM::Center;
	}
	return EFormationNUM::None;
}

void AB2GuildGameMode::OnPlayerStartWeaponSkillByIndex(int32 nInputIndex)
{
	TArray<ABladeIICharacter*> AllLiveMyPlayers;
	GetAllMyTeamCharList(AllLiveMyPlayers, GetMyTeamNum(), false);

	int32 playernumber = 0;
	for (auto player : AllLiveMyPlayers)
	{
		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(player);

		if (pPlayer)
		{
			if (nInputIndex == playernumber)
			{
				pPlayer->StartWeaponSkill();
				return;
			}
			playernumber++;
		}
	}
}

void AB2GuildGameMode::SetFormationPosition(class AController* NewPlayer, FString const& TagName)
{
	AActor* StartSpot = FindPlayerStart(NewPlayer, TagName);

	// if a start spot wasn't found,
	BII_CHECK(StartSpot != NULL);

	if (NewPlayer && NewPlayer->GetPawn())
	{
		auto* Pawn = NewPlayer->GetPawn();

		FRotator StartRotation(ForceInit);
		StartRotation.Yaw = StartSpot->GetActorRotation().Yaw;
		FVector StartLocation = StartSpot->GetActorLocation();
		Pawn->SetActorLocationAndRotation(StartLocation, StartRotation);
	}

	BII_CHECK(NewPlayer->GetPawn());

	// initialize and start it up
	InitStartSpot(StartSpot, NewPlayer);

	// set initial control rotation to player start's rotation
	NewPlayer->ClientSetRotation(NewPlayer->GetPawn()->GetActorRotation(), true);

	FRotator NewControllerRot = StartSpot->GetActorRotation();
	NewControllerRot.Roll = 0.f;
	NewPlayer->SetControlRotation(NewControllerRot);
}

void AB2GuildGameMode::AllControllerAutoTrigger(bool Start)
{
	//for (int8 TeamIndex = (int8)ETeamIndex::Player; TeamIndex < (int8)ETeamIndex::TeamIndex_Max; ++TeamIndex)
	//{
	//	TArray<ABladeIICharacter*> OutList;
	//	GetAllMyTeamCharList(OutList, TeamIndex, true);
	//	for (ABladeIICharacter* Character : OutList)
	//	{
	//		ABladeIIPlayerAIController* Controller = Cast<ABladeIIPlayerAIController>(Character->GetController());
	//		if (Controller)
	//		{
	//			Start ? Controller->BeginAutoPlay() : Controller->StopAutoPlay();
	//			continue;
	//		}
	//			
	//		AB2GuildNPCAIController* NPCController = Cast<AB2GuildNPCAIController>(Character->GetController());
	//		if (NPCController)
	//		{
	//			if (Start)
	//				NPCController->BeginAutoPlay();
	//			else
	//				NPCController->StopAutoPlay();
	//		}
	//			
	//	}
	//}
}

int32 AB2GuildGameMode::GetAttackDamageBonusPercentageByFormation(int32 InTeamNum)
{
	//return BladeIIGameImpl::GetClientDataStore().GetAttackDamageBonusByTeamFormation(ETMFormation::ETM_Normal, 1);
	return 0;
}

int32 AB2GuildGameMode::GetReduceDamageBonusPercentageByFormation(int32 InTeamNum)
{
	//return BladeIIGameImpl::GetClientDataStore().GetReduceDamageBonusByTeamFormation(ETMFormation::ETM_Normal, 1);
	return 0;
}
//
//void AB2GuildGameMode::SetGuildMatchActiveCameraActor(AB2ActiveCameraActor* pActiveCameraPlayerLong, AB2ActiveCameraActor* pActiveCameraPlayerShort, AB2ActiveCameraActor* pActiveCameraBaseFix)
//{
//	PlayerLongActiveCameraActor = pActiveCameraPlayerLong;
//	PlayerShortActiveCameraActor = pActiveCameraPlayerShort;
//	BaseFixActiveCameraActor = pActiveCameraBaseFix;
//}

void AB2GuildGameMode::ReturnToGuildMainMenu()
{
	//auto* GuildDOc = GetGuildDoc();

	//if (!GuildDOc)
	//	return;

	//if (MatchState != AsyncMatchState::StartResult && GuildDOc->GetBattleToken().Len() > 0)
	//{
	//	data_trader::Retailer::GetInstance().RequestFinishGuildBattle(GuildDOc->GetOtherGuildMemeberAccountId(),GuildDOc->GetBattleToken(), b2network::B2MatchResultType::BMRT_LOSE,0);
	//}

	//GoToVillage();

	//FLobbySceneManager::DeferredRegistChangeLobbyScene(
	//	[]() 
	//	{
	//		data_trader::Retailer::GetInstance().RequestGuildDetailInfo(0);
	//	}
	//);
}

void AB2GuildGameMode::MatchGameStartEventSceneEnd()
{
	SetMatchState(GuildMatchState::ReadyToRound);
}

TSharedPtr<FNPCUsingSkillState> AB2GuildGameMode::SharedUsingNPCSKillState()
{
	return IsNPCSkillUsing;
}

void AB2GuildGameMode::PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie)
{
	////肺爹官 救盲框 老窜..
	//UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	//if (PCBox)
	//{
	//	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	//	{
	//		UB2PCClassInfo* ThisLoadedInfo = PCBox->GetSingleClassInfo(IntToPCClass((PCI)));
	//		if (ThisLoadedInfo)
	//			ThisLoadedInfo->GetInGameOnlyInfo(this);
	//	}
	//}

	//PreloadInGameInfoAsset_SynchronousOnly(bAboutToPlayLoadingMovie);

	//if (UB2GuildNPCInfo* GuildNPCInfo = StaticFindGuildNPCInfo(this))
	//{
	//	TArray<FCombinedNPCClassID> PreloadGuildNPCIDs;
	//	for (int8 TeamIndex = (int8)ETeamIndex::Player; TeamIndex < (int8)ETeamIndex::TeamIndex_Max; ++TeamIndex)
	//	{
	//		ETeamIndex CurrentTeamIndex = (ETeamIndex)TeamIndex;
	//		for (const FGuildTeamInfo& GuildTeamInfo : MatchTeamInfos[CurrentTeamIndex])
	//		{
	//			for (const FGuildMercenaryInfo& MercenaryInfo : GuildTeamInfo.MercenaryInfos)
	//			{
	//				const FGuildNPCInfo* NPCInfo = GuildNPCInfo->GetGuildNPCInfoOfID(MercenaryInfo.MercenaryID);
	//				if(NPCInfo)
	//					PreloadGuildNPCIDs.AddUnique(FCombinedNPCClassID(NPCInfo->NPCClass, ENPCClassVariation::ENCV_GuildBattle));
	//			}
	//		}
	//	}
	//	TArray<FB2AsyncRequestInfo> PreloadAssets;
	//	if (UB2NPCClassInfoBox* NPCInfoBox = StaticFindMobClassInfoBox(this))
	//	{
	//		NPCInfoBox->GetAsyncRequestInfoFromNPCIds(PreloadGuildNPCIDs, PreloadAssets);
	//		//Aync? 傲 肺爹..捞 嘎阑波鞍篮单
	//		NPCInfoBox->TryAsyncLoad(TEXT("Guild Mercenary Preload"), PreloadAssets);
	//	}
	//}

	//Super::PreloadAnyNecessaryInfo(bAboutToPlayLoadingMovie);


	//PreloadInGameInfoAsset_AsyncAllowed(bAboutToPlayLoadingMovie);
}

TArray<EPCClass> AB2GuildGameMode::GetPCClassesToPreLoad()
{
	//Sync肺 肺爹且巴烙
	return TArray<EPCClass>();
}

void AB2GuildGameMode::CallSpawnAnim(ETeamIndex InTeamIndex)
{
	/*TArray<ABladeIICharacter*> TeamCharacters;
	GetAllMyTeamCharList(TeamCharacters, (int8)InTeamIndex == GetMyTeamNum(), true);

	for (ABladeIICharacter* Character : TeamCharacters)
	{
		if (ABladeIIGMAIPlayer* GMPlayer = Cast<ABladeIIGMAIPlayer>(Character))
		{
			if(GMPlayer->GetMesh())
				GMPlayer->StartCustomAnim(PCAnimMap[GMPlayer->GetCurrentPlayerClass()]);
		}
		if (ABladeIIMercenaryPlayer * MercenaryPlayer = Cast<ABladeIIMercenaryPlayer>(Character))
			MercenaryPlayer->bForceSpawAnim = true;
	}*/
}

void AB2GuildGameMode::FinishBattlePresentation(bool bTimeEnd, ABladeIICharacter * InInflictor)
{
	//if (bExcutedFinishBattle)
	//	return;

	//bExcutedFinishBattle = true;
	//AllControllerAutoTrigger(false);
	//SetAutoChangeCamera(false);

	////儡粮 某腐磐 HP钦魂
	//float MyTeamHPTotal = GetTotalRemainHPPercent((ETeamIndex)GetMyTeamNum());
	//float OpponentTeamHPTotal = GetTotalRemainHPPercent((ETeamIndex)GetOpponentTeamNum());

	//CurrentRound.WinnerTeam = MyTeamHPTotal >= OpponentTeamHPTotal ? ETeamIndex::Player : ETeamIndex::Enemy;
	//
	//CurrentRound.WinRoundCount[CurrentRound.WinnerTeam]++;

	//auto* GuildDoc = GetGuildDoc();
	//if (GuildDoc)
	//	GuildDoc->SetCurrentRoundWinner(CurrentRound.WinnerTeam);

	//if (bTimeEnd)
	//{
	//	//ui
	//	ShowRoundUI(false, WaitEndTimeFinishBattleTime);

	//	SetBaseActiveCamera();

	//	//Timer
	//	GetWorldTimerManager().ClearTimer(EndTimeFinishBattleTimer);
	//	GetWorldTimerManager().SetTimer(EndTimeFinishBattleTimer, this, &AB2GuildGameMode::OnEndTimeFinishBattle, WaitEndTimeFinishBattleTime);
	//}
	//else
	//{
	//	if (InInflictor)
	//		StartEventScene_FinishAttack(InInflictor);
	//	else
	//		SetMatchState(AsyncMatchState::FinishBattle);
	//}
}

void AB2GuildGameMode::ShowRoundUI(bool bStart, float InTime)
{
	/*auto* UI = UB2UIManager::GetInstance()->OpenUI<UB2UIGuildRound>(UIFName::GuildRound);
	if (UI)
		UI->StartRoundUI(InTime, bStart);*/
}

void AB2GuildGameMode::InitDilation()
{
	/*UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);

	for (TActorIterator<ABladeIICharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ABladeIICharacter *StopCharacter = *ActorItr;
		if (StopCharacter->IsValidObj() && StopCharacter->IsAlive())
			StopCharacter->SetCustomTimeDilation(1.0f);
	}*/
}

void AB2GuildGameMode::SetAutoChangeCamera(bool bSet)
{
	if (bSet)
	{
		GetWorldTimerManager().ClearTimer(ChangeCameraTimer);
		GetWorldTimerManager().SetTimer(ChangeCameraTimer, this, &AB2GuildGameMode::OnChangeCamera, ChangeCameraTime, true, 0.0f);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ChangeCameraTimer);
	}
}

UB2UIDocGuild * AB2GuildGameMode::GetGuildDoc()
{
	//if (!CacheGuildDoc)
	//{
	//	CacheGuildDoc = UB2UIDocHelper::GetDocGuild();
	//}
	return CacheGuildDoc;
}

void AB2GuildGameMode::InitGuildDoc()
{
	if (!GetGuildDoc())
		return;

	GetGuildDoc()->ClearGuildBattle();
	for (int8 TeamIndex = (int8)ETeamIndex::Player; TeamIndex < (int8)ETeamIndex::TeamIndex_Max; ++TeamIndex)
	{
		ETeamIndex CurrentTeamIndex = (ETeamIndex)TeamIndex;
		for (int32 index = 0; index < MatchTeamInfos[CurrentTeamIndex].Num(); index++)
			GetGuildDoc()->AddGuildTeam(CurrentTeamIndex, index);
	}
	GetGuildDoc()->SetAllyDisplayTeamNum(0);
	GetGuildDoc()->SetOtherDisplayTeamNum(0);
	GetGuildDoc()->SetViewTarget(nullptr);
	GetGuildDoc()->SetCurrentRound(1);
}

int32 AB2GuildGameMode::GetFormationSlotPosition(int32 SlotIdx)
{
	return SlotIdx + 1;
}

int32 AB2GuildGameMode::GetAggroPriority(int32 SlotIdx)
{
	return 1.0f;
}

void AB2GuildGameMode::SetDefaultTeamCombatStat(FGuildTeamInfo & TeamInfo, FCombatStatParam & CombatStat)
{
//	//CharacterDataStore
//	if (TeamInfo.TeamIndex == ETeamIndex::Player)
//		CombatStat.CharacterDataStore = &MyTeamCharacterDataStore;
//	else
//		CombatStat.CharacterDataStore = &OpponentTeamCharacterDataStore;
//
//#if WITH_EDITOR
//	if(IsTestMode)
//		CombatStat.CharacterDataStore = &BladeIIGameImpl::GetLocalCharacterData();
//#endif
//
//	//AddStatClasses
//	for (auto& Class : TeamInfo.PlayersInfos)
//		CombatStat.AddStatClasses.Add(Class.CharClass);
//
//	//柳屈俊 蝶弗 焊沥
//	CombatStat.ReduceDamageBonusByFormation = (100 - GetReduceDamageBonusPercentageByFormation((int32)TeamInfo.TeamIndex)) * 0.01f;
//	CombatStat.AttackBonusByFormation = (GetAttackDamageBonusPercentageByFormation((int32)TeamInfo.TeamIndex));
//
//	//GameMode俊 蝶弗 胶湃焊沥
//	CombatStat.ModeDamageRate = GetDamageRateByGameModeType();
//	CombatStat.ModeHealthRate = GetHealthRateByGameModeType();
}

FCombatStatParam AB2GuildGameMode::GetCombatStat(FGuildTeamInfo& TeamInfo, FGuildPlayerInfo& CharacterInfo)
{
	FCombatStatParam StatParam;
	
	SetDefaultTeamCombatStat(TeamInfo, StatParam);
	StatParam.SpawnPCClass = CharacterInfo.CharClass;

	return StatParam;
}

bool AB2GuildGameMode::HaveNextTeam(ETeamIndex InTeamIndex)
{
	return MatchTeamInfos[InTeamIndex].IsValidIndex(CurrentRound.TeamInfoIndexInRound[InTeamIndex] + 1);
}

void AB2GuildGameMode::SweepingTeam(FRoundInfo & InRound, ETeamIndex TeamIndex, bool bForceDie)
{
	/*for (AAIController* ControllerCache : InRound.GetTeamControllerCache(TeamIndex))
	{
		if (ControllerCache && ControllerCache->IsValidObj())
		{
			ABladeIICharacter* Pawn = Cast<ABladeIICharacter>(ControllerCache->GetPawn());
			if (Pawn && (!Pawn->IsAlive() || bForceDie))
			{
				Pawn->SetActorHiddenInGame(true);
				if(Pawn->IsAlive() && bForceDie)
					Pawn->SetDieForce();
			}
		}
	}*/
}

void AB2GuildGameMode::StartRound()
{
	//IsNPCSkillUsing->bUsingSkill = false;
	//
	//SetBuffTick(true);

	//AllControllerAutoTrigger(true);

	//RemainBattleTime = GUILD_BATTLE_TIME;
	//auto* GuildDoc = GetGuildDoc();
	//if (GuildDoc)
	//{
	//	GuildDoc->SetBattleTime(RemainBattleTime);
	//}

	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::GuildMatchBattle);
	//SetAutoChangeCamera(true);

	////gamestart
	//FightStartClass<int32>::GetInstance().Signal(CurrentRound.RoundNumber);
}

bool AB2GuildGameMode::SetNextRound()
{
	for (int8 TeamIndex = (int8)ETeamIndex::Player; TeamIndex < (int8)ETeamIndex::TeamIndex_Max; ++TeamIndex)
	{
		ETeamIndex CurrentTeamIndex = (ETeamIndex)TeamIndex;
		if (CurrentRound.WinnerTeam != CurrentTeamIndex)
		{
			
			if (HaveNextTeam(CurrentTeamIndex))
			{
				int32 TeamRoundNumber = ++CurrentRound.TeamInfoIndexInRound[CurrentTeamIndex];
				SpawnTeam(GetGuildTeamInfo(CurrentTeamIndex));
				ResetPositionTeam(CurrentTeamIndex);
				UB2UIDocGuild * GuildDoc = GetGuildDoc();
				if (GuildDoc)
				{
					TArray<ABladeIICharacter*> CharacterList;
					GetAllMyTeamCharList(CharacterList, TeamIndex, false);
					GuildDoc->AddGuildTeamMember(CurrentTeamIndex, TeamRoundNumber, CharacterList);

					if (TeamIndex == GetMyTeamNum())
						GuildDoc->SetAllyDisplayTeamNum(TeamRoundNumber);
					else
						GuildDoc->SetOtherDisplayTeamNum(TeamRoundNumber);
				}
			}
			else
			{
				return false;
			}
		}
	}

	CurrentRound.RoundNumber++;
	UB2UIDocGuild * GuildDoc = GetGuildDoc();
	if (GuildDoc)
		GuildDoc->SetCurrentRound(CurrentRound.RoundNumber);

	return true;
}

FGuildTeamInfo & AB2GuildGameMode::GetGuildTeamInfo(ETeamIndex InTeamIndex)
{
	return MatchTeamInfos[InTeamIndex][CurrentRound.GetRoundTeamNum(InTeamIndex)];
}

AAIController* AB2GuildGameMode::GetFindControllerInRound(ETeamIndex InTeamIndex, const FGuildPlayerInfo & InGuildPlayerInfo)
{
	/*for (AAIController* Controller : CurrentRound.GetTeamControllerCache(InTeamIndex))
	{
		if (Controller && Controller->IsValidObj())
		{
			IGuildPawnInterface* pawn = Cast<IGuildPawnInterface>(Controller->GetPawn());
			if (pawn && pawn->GetGuildPlayerInfo() == InGuildPlayerInfo)
				return Controller;
		}
	}*/
	return nullptr;
}

void AB2GuildGameMode::OnTeamPlayerMoveToBattlePosition(ETeamIndex InTeamIndex)
{
	if (!SetNextRound())
	{
		SetMatchState(AsyncMatchState::StartResult);
		return;
	}
	//力磊府肺 捞悼 肯丰 饶
	SetMatchState(GuildMatchState::ReadyToRound);
}

//void AB2GuildGameMode::OnMoveToComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result)
//{
//	ETeamIndex CompleteTeam;
//	AAIController* CurrentController = nullptr;
//	bool ReceiveResult = MoveToCompleteInfo.ReceiveRequestID(RequestID.GetID(), Result, CompleteTeam, &CurrentController);
//
//	if (CurrentController)
//	{
//		CurrentController->ReceiveMoveCompleted.RemoveDynamic(this, &AB2GuildGameMode::OnMoveToComplete);
//		IGuildPawnInterface* Pawn = Cast<IGuildPawnInterface>(CurrentController->GetPawn());
//		if (Pawn)
//		{
//			Pawn->GetPawnInstance()->SetMoveCollision(true);
//			AActor* PlayerStart = FindPlayerStart(CurrentController, Pawn->GetPlayerStartName());
//			if (PlayerStart)
//			{
//				Pawn->GetPawnInstance()->SetActorRotation(PlayerStart->GetActorRotation());
//				//捞悼 角菩矫 碍力肺 困摹 嘎苗 淋
//				if(Result != EPathFollowingResult::Success)
//					Pawn->GetPawnInstance()->SetActorLocation(PlayerStart->GetActorLocation());
//			}
//		}
//	}
//
//	if (ReceiveResult)
//		OnTeamPlayerMoveToBattlePosition(CompleteTeam);
//}

void AB2GuildGameMode::OnReadyToRoundTime()
{
	SetMatchState(GuildMatchState::ProgressRound);
}

void AB2GuildGameMode::OnEndTimeFinishBattle()
{
	SetMatchState(AsyncMatchState::FinishBattle);
}

void AB2GuildGameMode::OnChangeCamera()
{
	UB2UIDocGuild* GuildDoc = GetGuildDoc();

	TArray<ABladeIICharacter*> Characters;
	GetAllMyTeamCharList(Characters, GetMyTeamNum(), true);

	if (Characters.Num() <= 0)
		return;

	ABladeIICharacter* CurrentViewTarget = GuildDoc->GetViewTarget();
	Characters.Remove(CurrentViewTarget);

	if (Characters.Num() <= 0)
		return;

	int32 SelectIndex = FMath::RandRange(0, Characters.Num() - 1);

	ABladeIIPlayer* SelectViewTarget = Cast<ABladeIIPlayer>(Characters[SelectIndex]);
	if (SelectViewTarget == nullptr)
		return;

	ChangePlayerActiveCamera(false, SelectViewTarget, -1.0f);
}

void AB2GuildGameMode::NotifyStageEventDirector_FinishShow(int32 EventType)
{
	//EStageEvent eStageEvent = (EStageEvent)EventType;

	//switch (eStageEvent)
	//{
	//case EStageEvent::ESEV_FinishAttack:
	//	SetMatchState(AsyncMatchState::FinishBattle);
	//	break;
	//}
}

void FPCCache::ClearCache()
{
	MyTeamPC.Empty();
	EnemyTeamPC.Empty();
}

void FPCCache::AddCache(ETeamIndex team, ABladeIIPlayer * AddedCachePC)
{
	if (!AddedCachePC)
		return;

	if (team == ETeamIndex::Player)
		MyTeamPC.Add(AddedCachePC);
	else if (team == ETeamIndex::Enemy)
		EnemyTeamPC.Add(AddedCachePC);
}

