// Fill out your copyright notice in the Description page of Project Settings.
#include "B2TMGameMode.h"
//#include "BladeII.h"


#include "BladeIITMAIPlayer.h"
#include "BladeIIPlayerAIController.h"
#include "Event.h"

#include "BladeIIPlayerAIController.h"
#include "BladeIIPlayerController.h"
#include "B2ActiveCameraActor.h"

#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "BladeIIUtil.h"
#include "B2PCClassInfoBox.h"
////#include "Matinee/MatineeActor.h"
//#include "Matinee/InterpGroup.h"
//#include "Matinee/InterpGroupInst.h"
//#include "Matinee/InterpTrackMove.h"
#include "B2UIPVPModFindingMatch.h"
#include "B2UITeamMatchBattle.h"
#include "B2CombatStatEvaluator.h"
#include "B2DamageEffectInfo.h"
#include "B2BuffModeEffectInfo.h"
#include "B2GameLoadingProgress.h"
#include "Retailer.h"
#include "B2SkillInfo.h"
#include "B2SkillRTManager.h"

//#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "../Common/CommonStruct.h"
#include <variant>
#include "../../B2Network/Public/B2MessageInfo.h"
//#include "LevelSequence/Public/LevelSequenceActor.h"

int8 AB2TMGameMode::MyTeamNum = 1;
int8 AB2TMGameMode::OpponentTeamNum = 2;

int32 AB2TMGameMode::NumCharactersOfTeam = 3;
int8 AB2TMGameMode::GetMyTeamNum() 
{ 
	return MyTeamNum; 
}

int8 AB2TMGameMode::GetOpponentTeamNum()
{ 
	return OpponentTeamNum; 
}

AB2TMGameMode::AB2TMGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ASpectatorPawn::StaticClass(); // SpectatorPawn 을 기본으로
	PlayerControllerClass = ABladeIIPlayerController::StaticClass(); // 필요에 따라 따로 extend 해서 사용..

	DefaultPCClassEnum = EPCClass::EPC_End; // 일단 spawn 하지 않는다.

	MyTeamInfo.TeamNum = MyTeamNum;
	OpponentTeamInfo.TeamNum = OpponentTeamNum;

	CurrentActiveCameraPlayerIndex = 0;

	DefaultPlayerClass = ABladeIITMAIPlayer::StaticClass();
}

void AB2TMGameMode::PreBeginPlay()
{
	auto* GameRule = new TMGameModeRule;
	SetGameRule(GameRule);

	Super::PreBeginPlay();	

	PlayTMBGM(ETMBGMPlayContext::TMBGMCTX_Standby);

	SetStartTimeCount(false);
}

void AB2TMGameMode::BeginPlay()
{
	Super::BeginPlay();
	StageBeginClass<>::GetInstance().Signal(); // 이게 현재 하는 건 전투 UI 를 여는 건데 Stage 란 이름이 거슬리면 다른 이벤트로 바꿔서 쓸 수 있겠다. 아예 다른 이벤트가 필요할 수도 있고.
}

void AB2TMGameMode::BeginDestroy()
{
	Super::BeginDestroy();
}

void AB2TMGameMode::SubscribeEvents()
{
	if(bEventsSubscribed)
		return;

	Super::SubscribeEvents();

	Issues.Add(ReturnToPVPRematchClass<>::GetInstance().Subscribe2(
		[this]()
		{
			this->RequestRematch();
		}
	));
	Issues.Add(ReturnToPVPMainMenuClass<bool>::GetInstance().Subscribe2(
		[this](bool bSendMatchResult)
		{
			this->ReturnToTMMainMenu();
		}
	));
	Issues.Add(TeamMatchChangeCharacterCamClass<bool, int32>::GetInstance().Subscribe2(
		[this](bool bMyTeam, int32 SlotIdx)
		{
			this->ChangeCharacterCamera(bMyTeam, SlotIdx);
		}
	));

	Issues.Add(DeliveryTeamMatchMakingClass<FB2TeamMatchMaking>::GetInstance().Subscribe2(
		[this](const FB2TeamMatchMaking& MatchInfo)
		{
			//this->PlayToken = GET_TUPLE_DATA(FB2ResponseTeamMatchMaking::match_token_index, MatchInfo);
			//this->SetOpponentInfo(GET_TUPLE_DATA(FB2ResponseTeamMatchMaking::opponent_team_index, MatchInfo));

			//this->SetMatchState(AsyncMatchState::MatchOtherPlayer);
			//this->SetTeamMatchPointInfo(MatchInfo);
			//GuildBuffSetttingClass<const TArray<b2network::B2GuildSkillPtr>&>::GetInstance().Signal(GET_TUPLE_DATA(FB2ResponseTeamMatchMaking::guild_skills_index, MatchInfo));
		}
	));
	Issues.Add(DeliveryTeamMatchResultClass<FB2TeamMatchResult>::GetInstance().Subscribe2(
		[this](const FB2TeamMatchResult& ResultInfo)
		{
			this->SetMatchResultInfo(ResultInfo);
		}
	));
	Issues.Add(HandleServerError901Class<>::GetInstance().Subscribe2(
		[this]()
		{
			bool bUsedAdditionalPoint = false;
			auto* DocTeam = UB2UIDocHelper::GetDocTeamMatch();
			if (DocTeam)	bUsedAdditionalPoint = DocTeam->GetUsedAdditionalPoint();

			data_trader::Retailer::GetInstance().RequestTeamMatchmaking(bUsedAdditionalPoint);
		}
	));
	Issues.Add(HandleServerError6516Class<>::GetInstance().Subscribe2(
		[this]()
		{
			this->OpenPopupGotoTMMainMenu();
		}
	));
	Issues.Add(MatchGameStartEventSceneEndClass<>::GetInstance().Subscribe2(
		[this]()
		{
			this->MatchGameStartEventSceneEnd();
		}
	));
	Issues.Add(ReturnToFindAccountClass<>::GetInstance().Subscribe2(
		[this]()
		{
			this->ReturnToFindAccount();
		}
	));
	Issues.Add(PlayerStartWeaponSkillByIndexClass<int32>::GetInstance().Subscribe2(
		[this](int32 SkillInputindex)
		{
			this->OnPlayerStartWeaponSkillByIndex(SkillInputindex);
		}
	));
	Issues.Add(StageEventDirectorFinishShowEventClass<int32>::GetInstance().Subscribe2(
		[this](int32 EventType)
		{
			this->NotifyStageEventDirector_FinishShow(EventType);
		}
	));

}

void AB2TMGameMode::RequestRematch()
{
	//새롭게 시작하자.
	StartBladeIITeamMatchGame(Cast<AGameMode>(GetWorld()->GetAuthGameMode()));
}

void AB2TMGameMode::ReturnToFindAccount()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2TMGameMode::ReturnToFindAccount"));
	
	if (OpponentTeamCharacterDataStore.GetAccountInfoPtr())
	{
		BladeIIGameImpl::GetClientDataStore().SetOtherUserInfo(OpponentTeamCharacterDataStore.GetAccountInfoPtr());

		auto DocSome = UB2UIDocHelper::GetDocSome();

		if (DocSome)
			DocSome->SetFindUserAccountID(OpponentTeamCharacterDataStore.GetAccountInfoPtr()->account_id);

		FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
			// 전체 씬 전환을 하면 실 로딩 시간이 늘어나므로 원하는 화면 직전까지 UI History 만 만들어준다.
			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			if (UIMgrInst) {
				UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
				UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterBattleMain);
				UIMgrInst->ArtificialAddUIHistory(EUIScene::TeamMatchUI);
			}
			LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_FindAccount);
			data_trader::Retailer::GetInstance().RequestGetTeamBattleStatus();
		});

		ReturnToTMMainMenu();
	}
}

void AB2TMGameMode::ReturnToTMMainMenu()
{
	if (MatchState != AsyncMatchState::StartResult && PlayToken.Len() > 0)
	{
		data_trader::Retailer::GetInstance().RequestTeamMatchResult(PlayToken, false, true);
	}
		
	if (FLobbySceneManager::IsExistDeferredRegistChangeLobbyScene() == false)
	{
		FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
			// 전체 씬 전환을 하면 실 로딩 시간이 늘어나므로 원하는 화면 직전까지 UI History 만 만들어준다. 
			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			if (UIMgrInst) {
				UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
				UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterBattleMain);
			}
			LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ElobbyScene_TeamMatchMain);
			data_trader::Retailer::GetInstance().RequestGetTeamBattleStatus();
		});
	}	
	OpenBladeIILobbyCommon(this); // 씬 전환 예약 후 본격 로비 맵 로딩
}

void AB2TMGameMode::OpenPopupGotoTMMainMenu()
{
	FText txtComment = BladeIIGetLOCText(B2LOC_CAT_ERROR_CODE, FString::FromInt(6516));

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
		txtComment,
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm,
		FMsgPopupOnClick::CreateLambda([this]() {
		ReturnToTMMainMenu();
	})
	);
}

void AB2TMGameMode::EnterShop(int32 nWhereShop)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2TMGameMode::EnterShop"));

	FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
		// 전체 씬 전환을 하면 실 로딩 시간이 늘어나므로 원하는 화면 직전까지 UI History 만 만들어준다.
		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst) {
			UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
			UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterBattleMain);
			UIMgrInst->ArtificialAddUIHistory(EUIScene::TeamMatchUI);
		}
	});

	switch (static_cast<EStorePageWhere>(nWhereShop))
	{
	case EStorePageWhere::Store: 
		data_trader::Retailer::GetInstance().RequestGetGeneralShop();
		break;
	case EStorePageWhere::EquipStore:
		data_trader::Retailer::GetInstance().RequestGetLotteryShop();
		break;
	case EStorePageWhere::PackageStore:
		data_trader::Retailer::GetInstance().RequestGetPackageShop();
		break;
	case EStorePageWhere::MagicStore:
		data_trader::Retailer::GetInstance().RequestGetMagicShop();
		break;
	}

	ReturnToTMMainMenu();
}

void AB2TMGameMode::OnPlayerStartWeaponSkillByIndex(int32 nInputIndex)
{
	if (MyTeamInfo.GetNumAliveCharacters() == 0)
		return;

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

bool AB2TMGameMode::NeedToRecordDamageLog(const float ActualDamage, class ABladeIICharacter* Attacker)
{
	const bool IsMoreDamage = ActualDamage > DamageLogInfo.ActualDamage;
	auto* TMPlayer = Cast<ABladeIITMAIPlayer>(Attacker);

	if (IsMoreDamage && TMPlayer)
		return TMPlayer->GetIsLocalPlayerTeam();

	return false;
}

void AB2TMGameMode::OnMyTeamDead_AddUnityPoint(int32 TeamNum)
{
	TArray<ABladeIICharacter*> AllLivePlayers;

	if (TeamNum == GetMyTeamNum())
		GetAllMyTeamCharList(AllLivePlayers, GetMyTeamNum(), false);

	int32 playernumber = 0;
	for (auto thisplayer : AllLivePlayers)
	{
		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(thisplayer);
		if (pPlayer)
		{
			float BeforePoint = pPlayer->GetWeaponSkillPoint();
			float AfterPoint = FMath::Min(10.0f, BeforePoint + DeathUnityPointChargeAmount);

			pPlayer->SetWeaponSkillPoint(AfterPoint);
		}
	}
}

void AB2TMGameMode::OnOtherPlayerUnityPointClear(ABladeIITMAIPlayer* player)
{
	TArray<ABladeIICharacter*> AllLivePlayers;

	if(player->GetTeamNum() == GetMyTeamNum())
		GetAllMyTeamCharList(AllLivePlayers, GetMyTeamNum(), false);
	else
		GetAllMyTeamCharList(AllLivePlayers, GetOpponentTeamNum(), false);

	int32 playernumber = 0;
	for (auto thisplayer : AllLivePlayers)
	{
		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(thisplayer);
		if(pPlayer)
			pPlayer->SetWeaponSkillPoint(0.0f);
	}
}

void AB2TMGameMode::NotifyStageEventDirector_FinishShow(int32 EventType)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2TMGameMode::NotifyStageEventDirector_FinishShow"));

	EStageEvent eStageEvent = (EStageEvent)EventType;

	switch (eStageEvent)
	{
	case EStageEvent::ESEV_FinishAttack:
	{
		SetMatchState(AsyncMatchState::FinishBattle);
	}break;
	}
}

FTeamInfo& AB2TMGameMode::GetTeamInfo(int32 InTeamNum)
{
	return InTeamNum == MyTeamNum ? MyTeamInfo : OpponentTeamInfo;
}

APlayerController* AB2TMGameMode::GetSpectatorController()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		// It returns the first player controller having ULocalPlayer.
		// The implementation can be changed later..
		APlayerController* PlayerController = Iterator->Get();
		// It can be null when process is being terminated.
		if (PlayerController && PlayerController->GetLocalPlayer())
		{
			return PlayerController;
		}
	}

	return nullptr;
}

ICharacterDataStore* AB2TMGameMode::GetRemotePlayerDataStore(uint32 InTeamNum)
{	
	if (InTeamNum == GetMyTeamNum())
		return &BladeIIGameImpl::GetLocalCharacterData();
	else 
		return &OpponentTeamCharacterDataStore;
}

void AB2TMGameMode::AddEntryCharacterInfo(int8 TeamNum, int8 Spot, EPCClass CharClass, int32 InDocCharIndex, int32 AggroPriority, int8 FormationNum)
{
	check(TeamNum == GetMyTeamNum() || TeamNum == GetOpponentTeamNum());

	FCharInfo	Info(Spot, CharClass, InDocCharIndex, AggroPriority, FormationNum);

	if (TeamNum == GetMyTeamNum())
		MyTeamInfo.Characters.AddUnique(Info);
	else
		OpponentTeamInfo.Characters.AddUnique(Info);
}

void AB2TMGameMode::SetCharacterDoc()
{
	/*auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	auto* TeamMatchBattle = UB2UIManager::GetInstance()->GetUI<UB2UITeamMatchBattle>(UIFName::TeamMatchBattle);

	if (!TeamMatchDoc || !TeamMatchBattle)
		return;
	
	TeamMatchBattle->SetLeftPlayerInfo(BladeIIGameImpl::GetLocalCharacterData().GetUserNickName(), BladeIIGameImpl::GetLocalCharacterData().GetUserLevel(),
										TeamMatchDoc->GetPCGrade(), TeamMatchDoc->GetPCGradeStar());

	for (auto& Character : MyTeamInfo.Characters)
	{
		check(Character.AIController);
		auto* ControlledPawn = Cast<ABladeIITMAIPlayer>(Character.AIController->GetPawn());
		if (!ControlledPawn)
			continue;

		TeamMatchDoc->SetCharacterHP(true, Character.DocCharIndex, ControlledPawn->Health, ControlledPawn->MaxHealth);
		TeamMatchDoc->SetCharacterShield(true, Character.DocCharIndex, ControlledPawn->GetShield());
		TeamMatchBattle->SetReadyLeftSlot(Character.DocCharIndex, ControlledPawn->GetCurrentPlayerClass(), ControlledPawn->GetPlayerLevel(), Character.FormationNumber);
	}

	TeamMatchBattle->SetRightPlayerInfo(OpponentTeamCharacterDataStore.GetUserNickName(), OpponentTeamCharacterDataStore.GetUserLevel(),
										TeamMatchDoc->GetOpponentGrade(), TeamMatchDoc->GetOpponentGradeStar());
	for (auto& Character : OpponentTeamInfo.Characters)
	{
		check(Character.AIController);
		auto* ControlledPawn = Cast<ABladeIITMAIPlayer>(Character.AIController->GetPawn());
		if (!ControlledPawn)
			continue;

		TeamMatchDoc->SetCharacterHP(false, Character.DocCharIndex, ControlledPawn->Health, ControlledPawn->MaxHealth);
		TeamMatchDoc->SetCharacterShield(false, Character.DocCharIndex, ControlledPawn->GetShield());
		TeamMatchBattle->SetReadyRightSlot(Character.DocCharIndex, ControlledPawn->GetCurrentPlayerClass(), ControlledPawn->GetPlayerLevel());
	}

	this->TakeDamageUpdate();*/
}

int32 AB2TMGameMode::GetFormationSlotPosition(ETMFormation Formation, int32 SlotIdx)
{
	int32 FormationSlotNum = INDEX_NONE;

	//테이블화하기에는 좀 그렇고.. 하드코드함.
	switch (Formation)
	{	
	case ETMFormation::ETM_Attack:
		if (SlotIdx == 0)		FormationSlotNum = 2;
		else if (SlotIdx == 1)	FormationSlotNum = 4;
		else if (SlotIdx == 2)	FormationSlotNum = 5;
		break;
	case ETMFormation::ETM_Defense:
		if (SlotIdx == 0)		FormationSlotNum = 7;
		else if (SlotIdx == 1)	FormationSlotNum = 6;
		else if (SlotIdx == 2)	FormationSlotNum = 2;
		break;	
	default:
		FormationSlotNum = SlotIdx + 1;
		break;
	}

	return FormationSlotNum;
}

int32 AB2TMGameMode::GetAggroPriority(ETMFormation Formation, int32 SlotIdx)
{
	int32 AggroPriority = INDEX_NONE;

	//테이블화하기에는 좀 그렇고.. 하드코드함.
	switch (Formation)
	{
	case ETMFormation::ETM_Attack:
		if (SlotIdx == 0)		AggroPriority = 0;
		else if (SlotIdx == 1)	AggroPriority = 1;
		else if (SlotIdx == 2)	AggroPriority = 1;
		break;
	case ETMFormation::ETM_Defense:
		if (SlotIdx == 0)		AggroPriority = 0;
		else if (SlotIdx == 1)	AggroPriority = 0;
		else if (SlotIdx == 2)	AggroPriority = 1;
		break;
	default:
		AggroPriority = 0;
		break;
	}

	return AggroPriority;
}

int32 AB2TMGameMode::GetAttackDamageBonusPercentageByFormation(int32 InTeamNum)
{
	UB2UIDocTeamMatch* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (!TeamMatchDoc)
		return 0;

	if (GetMyTeamNum() == InTeamNum)
	{
		return BladeIIGameImpl::GetClientDataStore().GetAttackDamageBonusByTeamFormation(TeamMatchDoc->GetTMFormation(), TeamMatchDoc->GetCurrentPCFormationLevel());
	}
	else
	{
		return BladeIIGameImpl::GetClientDataStore().GetAttackDamageBonusByTeamFormation(TeamMatchDoc->GetOpponentTMFormation(), TeamMatchDoc->GetOpponentFormationLevel());
	}
}

int32 AB2TMGameMode::GetReduceDamageBonusPercentageByFormation(int32 InTeamNum)
{
	auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (!TeamMatchDoc)
		return 0;

	if (GetMyTeamNum() == InTeamNum)
	{
		return BladeIIGameImpl::GetClientDataStore().GetReduceDamageBonusByTeamFormation(TeamMatchDoc->GetTMFormation(), TeamMatchDoc->GetCurrentPCFormationLevel());
	}
	else
	{
		return BladeIIGameImpl::GetClientDataStore().GetReduceDamageBonusByTeamFormation(TeamMatchDoc->GetOpponentTMFormation(), TeamMatchDoc->GetOpponentFormationLevel());
	}
}

float AB2TMGameMode::GetOpponentHandicap(int32 InTeamNum)
{
	return GetMyTeamNum() == InTeamNum ? 1.f : ReduceOpponentStatForTutorial;
}

void AB2TMGameMode::AddMyTeamCharacterInfo()
{
	auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (!TeamMatchDoc)
		return;	

	for (int i = 0; i < AB2TMGameMode::GetNumCharactersOfTeam(); ++i)
	{
		AddEntryCharacterInfo(MyTeamNum, 
			GetFormationSlotPosition(TeamMatchDoc->GetTMFormation(), i), 
			TeamMatchDoc->GetCharacterClass(true, i), 
			i,
			GetAggroPriority(TeamMatchDoc->GetTMFormation(), i),
			TeamMatchDoc->GetCharacterFormationNum(i)
		);
	}
}

void AB2TMGameMode::AddOpponentTeamCharacterInfo()
{
	auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (!TeamMatchDoc)
		return;

	for (int i = 0; i < AB2TMGameMode::GetNumCharactersOfTeam(); ++i)
	{
		AddEntryCharacterInfo(OpponentTeamNum, 
			GetFormationSlotPosition(TeamMatchDoc->GetOpponentTMFormation(), i), 
			TeamMatchDoc->GetCharacterClass(false, i), 
			i,
			GetAggroPriority(TeamMatchDoc->GetOpponentTMFormation(), i));
	}
}

void AB2TMGameMode::TakeDamageUpdate()
{
	UB2UIDocTeamMatch* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (!TeamMatchDoc)
		return;

	for (auto& Character : MyTeamInfo.Characters)
	{
		check(Character.AIController);
		auto* ControlledPawn = Cast<ABladeIITMAIPlayer>(Character.AIController->GetPawn());
		if (!ControlledPawn)
			continue;

		TeamMatchDoc->SetCharacterHP(true, Character.DocCharIndex, ControlledPawn->Health);
		TeamMatchDoc->SetCharacterShield(true, Character.DocCharIndex, ControlledPawn->GetShield());
	}

	for (auto& Character : OpponentTeamInfo.Characters)
	{
		check(Character.AIController);
		auto* ControlledPawn = Cast<ABladeIITMAIPlayer>(Character.AIController->GetPawn());
		if (!ControlledPawn)
			continue;

		TeamMatchDoc->SetCharacterHP(false, Character.DocCharIndex, ControlledPawn->Health);
		TeamMatchDoc->SetCharacterShield(false, Character.DocCharIndex, ControlledPawn->GetShield());
	}
}

void FTeamInfo::SetAccumulateDamage(const class ABladeIICharacter* InPlayer, float InDamage)
{
	if (InPlayer && CharacterAccumulateDamage.Num() > static_cast<int32>(InPlayer->GetCurrentPlayerClass()))
	{
		CharacterAccumulateDamage[static_cast<int32>(InPlayer->GetCurrentPlayerClass())] += InDamage;
	}
}

void FTeamInfo::SpawnTeamCharacters(AB2TMGameMode* GameMode)
{
	check(GameMode);
	check(Characters.Num() == GameMode->GetNumCharactersOfTeam());

	//FActorSpawnParameters ControllerSpawnInfo;
	//ControllerSpawnInfo.Instigator = NULL;
	//ControllerSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//ControllerSpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save player controllers into a map
	//ControllerSpawnInfo.bDeferConstruction = true;

	//FActorSpawnParameters PawnSpawnInfo;
	//PawnSpawnInfo.bDeferPostSpawnInitialize = true;

	//FTransform SpawnTransform(FRotator::ZeroRotator, FVector::ZeroVector);
	//CharacterAccumulateDamage.SetNum(static_cast<int32>(EPCClass::EPC_End));

	//for (auto& Character : Characters)
	//{
	//	GameMode->CurrentPCClassEnum = Character.CharClass;
	//	auto* NewAC = GameMode->GetWorld()->SpawnActor<AB2TeamMatchPlayerAIController>(AB2TeamMatchPlayerAIController::StaticClass(), SpawnTransform, ControllerSpawnInfo);
	//	BII_CHECK(NewAC);
	//	UGameplayStatics::FinishSpawningActor(NewAC, SpawnTransform);

	//	auto* ControlledPawn = GameMode->GetWorld()->SpawnActor<ABladeIITMAIPlayer>(GameMode->GetDefaultPlayerClass(), SpawnTransform, PawnSpawnInfo);
	//	check(ControlledPawn);

	//	ControlledPawn->FinalizePCClassInfoSpawn();
	//	ControlledPawn->SetTeamCharClasses(Characters[0].CharClass, Characters[1].CharClass, Characters[2].CharClass);
	//	NewAC->SetTeamNum(TeamNum);
	//	NewAC->Possess(ControlledPawn);
	//	GameMode->GetWorld()->ManualPostSpawnInitialize(ControlledPawn, SpawnTransform, PawnSpawnInfo);

	//	// 최초스폰은 P1B_1 이동할곳은 P1_1 이런형식으로
	//	// 이동은 TeamPlayerMoveToStartPosition 함수에서
	//	FString SpotTagName = FString::Printf(TEXT("P%dB_%d"), TeamNum, Character.DocCharIndex);
	//	GameMode->RestartEntryPlayer(NewAC, SpotTagName);

	//	Character.AIController = NewAC;
	//}
}

void AB2TMGameMode::NotifyTargetDamagedFromDamagedTarget(class ABladeIICharacter* DamageCauser, class ABladeIICharacter* DamagedTarget, float fDamageAmount)
{
	//if (DamageCauser)
	//{
	//	GetTeamInfo(DamageCauser->GetTeamNum()).SetAccumulateDamage(DamageCauser, fDamageAmount);

	//	if (auto* TeamMatchBattle = UB2UIManager::GetInstance()->GetUI<UB2UITeamMatchBattle>(UIFName::TeamMatchBattle))
	//	{
	//		TeamMatchBattle->SetDamagePercent(DamageCauser->GetTeamNum() == MyTeamNum, GetTeamInfo(DamageCauser->GetTeamNum()).CharacterAccumulateDamage);
	//	}
	//}
}

bool AB2TMGameMode::HasMatchStarted() const
{
	return !(GetMatchState() == MatchState::EnteringMap
		|| GetMatchState() == MatchState::WaitingToStart
		|| GetMatchState() == AsyncMatchState::RequestMatchMaking
		|| GetMatchState() == AsyncMatchState::MatchOtherPlayer);
}

void AB2TMGameMode::OnMatchStateSet()
{
	if (MatchState == AsyncMatchState::RequestMatchMaking)
	{
		HandleRequestMatchMaking();
	}
	else if (MatchState == AsyncMatchState::MatchOtherPlayer)
	{
		HandleMatchOtherPlayer();
	}
	else if (MatchState == AsyncMatchState::StartResult)
	{
		HandleStartResult();
	}
	else if (MatchState == AsyncMatchState::FinishBattle)
	{
		HandleFinishBattle();
	}
	else
	{
		Super::OnMatchStateSet();
	}
}

void AB2TMGameMode::HandleMatchIsWaitingToStart()
{
	//auto* PlayerController = UGameplayStatics::GetLocalPlayerController(this);
	//if (PlayerController)
	//	PlayerController->ActivateTouchInterface(nullptr);

	//SetMatchState(AsyncMatchState::RequestMatchMaking);
}

void AB2TMGameMode::HandleRequestMatchMaking()
{
	AddMyTeamCharacterInfo();

	SceneManager.SetGameMode(this);
	SceneManager.SetState(ETeamMatchState::Finding);	
}

void AB2TMGameMode::SetOpponentInfo(FB2OpponentTeamMatchInfoDataPtr OpponentServerInfo)
{
	if (OpponentServerInfo && OpponentServerInfo->account)
	{
		OpponentTeamCharacterDataStore.SetModPlayerInfo(OpponentServerInfo->account);

		auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();

		if (TeamMatchDoc)
		{
			TeamMatchDoc->SetOpponentGrade(OpponentServerInfo->match_status->battle_grade);
			TeamMatchDoc->SetOpponentGradeStar(OpponentServerInfo->match_status->battle_star_count);
			TeamMatchDoc->SetOpponentContinuousWins(OpponentServerInfo->match_status->continue_win);

			TeamMatchDoc->SetOpponentTMFormation(SvrToCliFormationType(OpponentServerInfo->formation->formation_type));
			TeamMatchDoc->SetOpponentFormationLevel(OpponentServerInfo->formation->formation_level);

			TeamMatchDoc->SetCharacterClass(false, 0, SvrToCliPCClassType(OpponentServerInfo->entry->character_slot_1));
			TeamMatchDoc->SetCharacterClass(false, 1, SvrToCliPCClassType(OpponentServerInfo->entry->character_slot_2));
			TeamMatchDoc->SetCharacterClass(false, 2, SvrToCliPCClassType(OpponentServerInfo->entry->character_slot_3));

			ReduceOpponentStatForTutorial = OpponentServerInfo->debuff_value > 0 ? OpponentServerInfo->debuff_value * 0.01f : 1.f;
			
			auto IsSameTeam = OpponentServerInfo->account->account_id == BladeIIGameImpl::GetClientDataStore().GetAccountId();
			TeamMatchDoc->SetIsAIMe(IsSameTeam);
		}
	}
}

void AB2TMGameMode::GatherEquippedSkillAnimIDs(ICharacterDataStore* DataStore, TArray<FCombinedPCSkillAnimID>& OutEquippedSkills)
{
	//auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();

	//if (TeamMatchDoc && DataStore)
	//{
	//	const int32 PCTypeMax = 3;
	//	const bool IsLocalPlayer = DataStore->IsLocalCharacterData();

	//	for (int32 index = 0; index < PCTypeMax; index++)
	//	{
	//		const EPCClass TargetClass = TeamMatchDoc->GetCharacterClass(IsLocalPlayer, index);
	//		OutEquippedSkills.Append(GetEquippedSkillAnimIDs(TargetClass, DataStore));
	//	}
	//}
}

void AB2TMGameMode::HandleMatchOtherPlayer()
{
	AddOpponentTeamCharacterInfo();

	///////임시	
	//SceneManager.SetRightEntry();

	//auto* FindingMatchUI = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::TeamMatchFindMatch);
	//if (FindingMatchUI)
	//{	
	//	FindingMatchUI->FindMatch(true);
	//}

	//SceneManager.SetState(ETeamMatchState::Ready);

	//TryAsyncLoadSkillAnims(&BladeIIGameImpl::GetLocalCharacterData());
	//TryAsyncLoadSkillAnims(&OpponentTeamCharacterDataStore);
}

void AB2TMGameMode::HandleMatchHasStarted()
{
	MyTeamInfo.SpawnTeamCharacters(this);
	OpponentTeamInfo.SpawnTeamCharacters(this);

	CachedHighAggroCharacter.AddDefaulted(2);
	UpdateHighAggroCharacters();

	GEngine->BlockTillLevelStreamingCompleted(GetWorld());

	BII_CHECK(GetWorldSettings());

	// First fire BeginPlay, if we haven't already in waiting to start match
	GetWorldSettings()->NotifyBeginPlay();

	// Then fire off match started
	GetWorldSettings()->NotifyMatchStarted();

	// 연출 시작전에 기본카메라있으면 찾아서 활성화
	// 핸드폰에서는 컨트롤러의 부재로 액티브 카메라 액터의
	// BeginPlay()에서 활성화 안되는경우가 있는듯
	SetBaseActiveCamera();

#if BII_STATS // PerfStat 은 MatchStart 시점부터 모으기 시작.
	PerfStatCollector.OnBegin(this);
#endif

	// 연출시작
	StartEventScene_GameBegin();

	PlayTMBGM(ETMBGMPlayContext::TMBGMCTX_Combat);

	SetCharacterDoc();
}

void AB2TMGameMode::SetMatchState(FName NewState)
{
	Super::SetMatchState(NewState);
}

void AB2TMGameMode::MatchGameStartEventSceneEnd()
{
	AllPlayerBeginAI();
}

void AB2TMGameMode::HandleStartResult()
{
	ENetMatchResult result(CalcResult());
	
	AllPlayerStopAI();

	// 결과 정보를 UI 쪽에 세팅하는 부분은 구현 정리되면 한 곳으로
	auto* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome)
	{
		DocSome->SetLocalPlayerMatchResult(result);
	}

	GetWorldTimerManager().ClearTimer(TimeToChangeCameraTarget);
}

void AB2TMGameMode::StartMatchResult()
{
	ENetMatchResult result(CalcResult());
	bool bMyTeamWin = (result == ENetMatchResult::LocalLose ? false : true);

	StartEventScene_MatchResult(bMyTeamWin);
	data_trader::Retailer::GetInstance().RequestTeamMatchResult(PlayToken, bMyTeamWin);
}

void AB2TMGameMode::HandleFinishBattle()
{
	//[@AKI, 170607] 전투가 끝났으나 마지막 TakeDamageUpdate를 안해줌으로 여기에 추가함
	TakeDamageUpdate();

	SceneManager.SetState(ETeamMatchState::BattleEnd);
}

void AB2TMGameMode::AllPlayerStopAI()
{
	for (auto& Character : MyTeamInfo.Characters)
	{
		if (Character.AIController)
			Character.AIController->StopAutoPlay();
	}
	for (auto& Character : OpponentTeamInfo.Characters)
	{
		if (Character.AIController)
			Character.AIController->StopAutoPlay();
	}
}

void AB2TMGameMode::AllPlayerBeginAI()
{
	for (auto& Character : MyTeamInfo.Characters)
	{
		if (Character.AIController)
			Character.AIController->BeginAutoPlay();
	}
	for (auto& Character : OpponentTeamInfo.Characters)
	{
		if (Character.AIController)
			Character.AIController->BeginAutoPlay();
	}
}

void AB2TMGameMode::TeamPlayerMoveToStartPosition(bool bMyTeam)
{
	auto* teamInfo = bMyTeam ? &MyTeamInfo : &OpponentTeamInfo;

	for (auto& Character : teamInfo->Characters)
	{
		if (!Character.AIController)
			continue;

		// 이벤트 디렉터에서 숨긴거랑 이동 잠근거 풀어줌
		ABladeIICharacter* pCharacter = Cast<ABladeIICharacter>(Character.AIController->GetPawn());

		if (pCharacter)
		{
			pCharacter->SetActorHiddenInGame(false);
			pCharacter->AllowMovementByStageEvent();
		}

		// 정해진 곳으로 이동
		FString SpotTagName = FString::Printf(TEXT("P%d_%d"), teamInfo->TeamNum, Character.Spot);

		AActor* StartSpot = FindPlayerStart(Character.AIController, SpotTagName);
		if (StartSpot)
			Character.AIController->MoveToLocation(StartSpot->GetActorLocation());
	}
}

ENetMatchResult AB2TMGameMode::CalcResult()
{
	ENetMatchResult result(ENetMatchResult::LocalLose);

	bool bMyTeamWin = true;
	bool bDraw		= false;

	//잔존 캐릭터 HP합산
	auto MyTeamHPTotal			= MyTeamInfo.GetCharacterHPTotal();
	auto OpponentTeamHPTotal	= OpponentTeamInfo.GetCharacterHPTotal();

	if (MyTeamHPTotal > OpponentTeamHPTotal)
		result = ENetMatchResult::LocalWin;
	else if (MyTeamHPTotal < OpponentTeamHPTotal)
		result = ENetMatchResult::LocalLose;
	else
		result = ENetMatchResult::Draw;

	return result;
}

void AB2TMGameMode::SetMatchResultInfo(const FB2TeamMatchResult& MatchResult)
{
	//auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	//if (TeamMatchDoc)
	//{
	//	auto matchStatus = GET_TUPLE_DATA(FB2ResponseTeamMatchResult::match_status_index, MatchResult);
	//	auto RewardBoxInfo = GET_TUPLE_DATA(FB2ResponseTeamMatchResult::reward_box_status_index, MatchResult);

	//	if (matchStatus && RewardBoxInfo)
	//	{
	//		TeamMatchDoc->SetPCGrade(matchStatus->battle_grade);
	//		TeamMatchDoc->SetPCGradeStar(matchStatus->battle_star_count);
	//		TeamMatchDoc->SetPCPrevContinuousWins(TeamMatchDoc->GetPCCurrContinuousWins());
	//		TeamMatchDoc->SetPCCurrContinuousWins(matchStatus->continue_win);
	//		TeamMatchDoc->SetPCPrevTopContinuousWins(TeamMatchDoc->GetPCCurrTopContinuousWins());
	//		TeamMatchDoc->SetPCCurrTopContinuousWins(matchStatus->continue_win_max);

	//		// 박스 결과세팅하기전에. 원래꺼랑 비교
	//		if (TeamMatchDoc->GetRewardBoxStatus()
	//			&& !TeamMatchDoc->GetRewardBoxStatus()->is_reward_state
	//			&& RewardBoxInfo->is_reward_state)
	//		{
	//			TeamMatchDoc->SetCanViewModRewardBoxConfirm(true);
	//		}

	//		TeamMatchDoc->SetRewardBoxStatus(RewardBoxInfo);
	//	}

	//	auto* DocSome = UB2UIDocHelper::GetDocSome();
	//	if (DocSome)
	//	{
	//		auto MyRanking = GET_TUPLE_DATA(FB2ResponseTeamMatchResult::ranking_index, MatchResult);
	//		if (MyRanking.get())
	//		{
	//			DocSome->SetRankingChanged((TeamMatchDoc->GetPCRank() != 0 ? TeamMatchDoc->GetPCRank() : MyRanking->total_ranker_count) - MyRanking->ranking);
	//			TeamMatchDoc->SetPCRank(MyRanking->ranking);

	//			FModRankerInfo MyRankerInfo;
	//			MyRankerInfo.Grade = TeamMatchDoc->GetPCGrade();
	//			MyRankerInfo.GradeStar = TeamMatchDoc->GetPCGradeStar();
	//			MyRankerInfo.Rank = MyRanking->ranking;
	//			MyRankerInfo.UserName = BladeIIGameImpl::GetClientDataStore().GetUserNickName();

	//			MyRankerInfo.Entry.Add(TeamMatchDoc->GetPCCharacterClass1());
	//			MyRankerInfo.Entry.Add(TeamMatchDoc->GetPCCharacterClass2());
	//			MyRankerInfo.Entry.Add(TeamMatchDoc->GetPCCharacterClass3());

	//			auto* CharDoc1 = UB2UIDocHelper::GetDocHero((int32)TeamMatchDoc->GetPCCharacterClass1());
	//			auto* CharDoc2 = UB2UIDocHelper::GetDocHero((int32)TeamMatchDoc->GetPCCharacterClass2());
	//			auto* CharDoc3 = UB2UIDocHelper::GetDocHero((int32)TeamMatchDoc->GetPCCharacterClass3());

	//			if (CharDoc1 && CharDoc2 && CharDoc3)
	//			{
	//				MyRankerInfo.TotalCombatPower = CharDoc1->GetCombatPower() + CharDoc2->GetCombatPower() + CharDoc3->GetCombatPower();
	//	
	//				//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
	//				//MyRankerInfo.EntryLevels.Add(CharDoc1->GetCurrentLevel());
	//				//MyRankerInfo.EntryLevels.Add(CharDoc2->GetCurrentLevel());
	//				//MyRankerInfo.EntryLevels.Add(CharDoc3->GetCurrentLevel());
	//				FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	//				MyRankerInfo.EntryLevels.Add(CharacterDataStore.GetCharacterLevel(CharDoc1->GetHeroClass()));
	//				MyRankerInfo.EntryLevels.Add(CharacterDataStore.GetCharacterLevel(CharDoc2->GetHeroClass()));
	//				MyRankerInfo.EntryLevels.Add(CharacterDataStore.GetCharacterLevel(CharDoc3->GetHeroClass()));
	//			}

	//			DocSome->SetMyRankerInfo(MyRankerInfo);
	//		}

	//		auto OtherRanking = GET_TUPLE_DATA(FB2ResponseTeamMatchResult::prev_ranker_index, MatchResult);
	//		if (OtherRanking.get())
	//		{
	//			FModRankerInfo OtherRankerInfo;
	//			OtherRankerInfo.Grade = OtherRanking->battle_grade;
	//			OtherRankerInfo.GradeStar = OtherRanking->battle_star_count;
	//			OtherRankerInfo.Rank = MyRanking->ranking + 1;
	//			OtherRankerInfo.UserName = OtherRanking->nickname;

	//			for (auto& EntryChar : OtherRanking->entries)
	//			{
	//				OtherRankerInfo.Entry.Add(SvrToCliPCClassType(EntryChar.character_type));
	//				OtherRankerInfo.EntryLevels.Add(EntryChar.level);
	//			}

	//			OtherRankerInfo.TotalCombatPower = OtherRanking->total_power;		// jwyim power

	//			DocSome->SetOtherRankerInfo(OtherRankerInfo);
	//		}
	//	}

	//	auto MyRewards = GET_TUPLE_DATA(FB2ResponseTeamMatchResult::rewards_index, MatchResult);
	//	for (auto Reward : MyRewards)
	//	{
	//		BladeIIGameImpl::GetClientDataStore().ApplyRewardInfo(Reward);
	//	}
	//	DocSome->SetRewards(MyRewards);
	//}
	////[@AKI, 170613] 약 네트워크에서 결과를 서버에서 받지 않은 상태에서 결과화면 보여주지 않게 변경
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::PVPResult);
}

void AB2TMGameMode::Tick(float DeltaSeconds)
{
//	SceneManager.Tick(DeltaSeconds);
//
//	if (IsInPreRenderPhase()) {
//		UpdateForPreRenderObjects(); // 이게 돌아가는 와중에는 아직 정식 시작 안 한 거..
//		// Pre-render 에서 메인 맵 로딩 화면 닫는 걸 지연시키게 되므로 여기서 처리. Pre-render 용 로딩 화면은 따로 있고 이건 단지 로딩 뺑글이 돌아가는 거 연장하기 위한 차원이다.
//		ConditionalWaitForLoadingScreen();
//	}
//
//#if BII_STATS
//	if (FStatisticalPerfStatCollector::bDoStatCollecting) {
//		PerfStatCollector.OnTick(this, DeltaSeconds);
//	}
//#endif
}

void AB2TMGameMode::SetMatchLobbyResources(
	class ALevelSequenceActor* InMatinee,
	const FMatchingAnims& Gladiator,
	const FMatchingAnims& Assassin,
	const FMatchingAnims& Wizard,
	const FMatchingAnims& Fighter
)
{
	SceneManager.SetMatchLobbyResources(InMatinee, Gladiator, Assassin, Wizard, Fighter);
}

void AB2TMGameMode::SetMatchLobbyCharacterLocationZ(float GladiatorZ, float AssassinZ, float WizardZ, float FighterZ)
{
	SceneManager.SetMatchLobbyCharacterLocationZ(GladiatorZ, AssassinZ, WizardZ, FighterZ);
}

void AB2TMGameMode::StartPlay()
{	
	Super::StartPlay();
}

APawn* AB2TMGameMode::RestartEntryPlayer(class AController* NewPlayer, FString const& TagName)
{
	AActor* StartSpot = FindPlayerStart(NewPlayer, TagName);

	// if a start spot wasn't found,
	BII_CHECK(StartSpot != NULL);

	if (NewPlayer == NULL)
	{
		auto*	Pawn = SpawnDefaultPawnFor(NULL, StartSpot);
		BII_CHECK(Pawn);

		Pawn->FaceRotation(Pawn->GetActorRotation(), 0.f);
		Pawn->SetPlayerDefaults();

		return Pawn;
	}

	APawn*	created = NULL;
	// try to create a pawn to use of the default class for this player
	if (NewPlayer->GetPawn() == NULL && DefaultPawnClass != NULL)
	{
		created = SpawnDefaultPawnFor(NewPlayer, StartSpot);
		BII_CHECK(created);

		NewPlayer->SetPawn(created);
	}
	else if (NewPlayer->GetPawn())
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
	NewPlayer->Possess(NewPlayer->GetPawn());

	// set initial control rotation to player start's rotation
	NewPlayer->ClientSetRotation(NewPlayer->GetPawn()->GetActorRotation(), true);

	FRotator NewControllerRot = StartSpot->GetActorRotation();
	NewControllerRot.Roll = 0.f;
	NewPlayer->SetControlRotation(NewControllerRot);

	SetPlayerDefaults(NewPlayer->GetPawn());

	K2_OnRestartPlayer(NewPlayer);

	return created;
}

void AB2TMGameMode::GetAllMyTeamCharList(TArray<class ABladeIICharacter*>& OutList, int8 InMyTeamNum, bool bOnlyLive)
{
	BII_CHECK(InMyTeamNum == GetMyTeamNum() || InMyTeamNum == GetOpponentTeamNum());

	auto& TeamInfo = InMyTeamNum == GetMyTeamNum() ? MyTeamInfo : OpponentTeamInfo;

	TeamInfo.GetCharacterList(OutList, bOnlyLive);
}

void AB2TMGameMode::GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 InMyTeamNum, bool bOnlyLiveMobs)
{
	if (InMyTeamNum == INDEX_NONE)
	{
		GetAllWorldMobList(OutList, bOnlyLiveMobs);
	}
	else
	{
		OutList.Empty();

		auto& TeamInfo = InMyTeamNum != GetMyTeamNum() ? MyTeamInfo : OpponentTeamInfo;

		TeamInfo.GetCharacterList(OutList, bOnlyLiveMobs);
	}
}

void AB2TMGameMode::GetAllWorldMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs)
{
	OutList.Empty();

	MyTeamInfo.GetCharacterList(OutList, bOnlyLiveMobs);
	OpponentTeamInfo.GetCharacterList(OutList, bOnlyLiveMobs);
}

ABladeIIPlayer* AB2TMGameMode::GetTeamPlayer(bool bAllyTeam, int32 nPlayerIndex, bool bLiveOnly, bool bSpawnPuppetForDead /*= false*/)
{
	TArray<class ABladeIICharacter*> CharacterList;

	if (bAllyTeam)
		MyTeamInfo.GetCharacterList(CharacterList, bLiveOnly);
	else
		OpponentTeamInfo.GetCharacterList(CharacterList, bLiveOnly);

	// 현재 GetCharacterList 구현상 CharacterList 에 채워넣지 못하는 경우 인덱스가 틀려져 버리므로 인덱스로 보는 것 자체가 문제임.
	// 결국 여길 지나서 아래 SpawnTeamPlayerAsPuppet 으로 가는 게 여전히 문제 상황이 될 수 있음.
	if (CharacterList.IsValidIndex(nPlayerIndex))
	{
		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(CharacterList[nPlayerIndex]);
		if (pPlayer)
			return pPlayer;
	}

	// 이미 죽어서 못 가져오는 경우..
	if (!bLiveOnly && bSpawnPuppetForDead)
	{
		return SpawnTeamPlayerAsPuppet(bAllyTeam, nPlayerIndex, false);
	}

	return nullptr;
}

// 대체로 연출에 사용할 목적임. 연출에 사용하기 위해 spawn.
ABladeIIPlayer* AB2TMGameMode::SpawnTeamPlayerAsPuppet(bool bAllyTeam, int32 nPlayerIndex, bool bPossessByAIController /*= true*/)
{
	// 클래스 정보가 여기 말고 딴 데 없나..
	auto* TeamMatchDoc		= UB2UIDocHelper::GetDocTeamMatch();
	auto CharClassToSpawn	= EPCClass::EPC_End;

	if (TeamMatchDoc)
	{
		if (nPlayerIndex == 0) 
		{
			CharClassToSpawn = TeamMatchDoc->GetPCCharacterClass1();
		}
		else if (nPlayerIndex == 1) 
		{
			CharClassToSpawn = TeamMatchDoc->GetPCCharacterClass2();
		}
		else if (nPlayerIndex == 2) 
		{
			CharClassToSpawn = TeamMatchDoc->GetPCCharacterClass3();
		}
	}

	return SpawnTeamPlayerAsPuppetbyClass(bAllyTeam, CharClassToSpawn, bPossessByAIController);
}

ABladeIIPlayer* AB2TMGameMode::SpawnTeamPlayerAsPuppetbyClass(bool bAllyTeam, EPCClass InCharClass, bool bPossessByAIController /*= true*/)
{
	//if (InCharClass == EPCClass::EPC_End)
	//{
		return nullptr;
	//}

	//// 기준이 될 플레이어가 없을 수 있다.. 플레이어 start 라도 쓸까.. 
	//FTransform NewTransform(
	//	FRotator(0.0f, 0.0f, 0.0f),
	//	FVector(0.0f, 0.0f, 0.0f),
	//	FVector(1.0f, 1.0f, 1.0f)
	//);

	//// 딱히 이전 걸 없앤다든지 할 필요 없이 그대로 리턴. 받은 쪽에서 잘 처리. 
	//// PlayerController 에 possess 하지는 않으니 GetLocalPlayerCharacter 등이 의도와 달리 여기서 생성한 것을 리턴한다든지 하는 일은 없을 것.
	//ABladeIIPlayer* SpawnedPuppet = ABladeIIGameMode::SpawnPlayerCharAsPuppet(this, InCharClass, NewTransform, bPossessByAIController, ABladeIIPlayer::StaticClass());

	//// 기본적으로는 로컬 플레이어 캐릭터 장비로 맞춰져 있을테니 Ally 가 아니라면 장비 데이터를 직접 세팅해준다. 
	//if (!bAllyTeam)
	//{ // 단, 게임모드에 캐싱된 걸 가져와서 사용하려면 NetID 를 넣어주어야 하는데 현재 SetupComponentsForPartsCustomDataStore 구현 자체가 Custom 에 걸맞게 캐싱을 무시하도록 되어있다.
	//	SpawnedPuppet->SetupComponentsForPartsCustomDataStore(GetRemotePlayerDataStore(GetOpponentTeamNum()), 
	//		// 이거 거의 연출용 기능이라 SectionMerge 여부에 연출용 판별 함수 리턴값 전달.
	//		UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(this)
	//		);
	//}

	//return SpawnedPuppet;
}

void FTeamInfo::GetCharacterList(TArray<class ABladeIICharacter*>& CharacterList, bool bOnlyLive)
{
	for (auto& Character : Characters)
	{
		check(Character.AIController);
		auto* ControlledPawn = Cast<ABladeIIPlayer>(Character.AIController->GetPawn());
		if (ControlledPawn &&
			(!bOnlyLive || (ControlledPawn->IsAlive() && ControlledPawn->CanBeTarget()))
			)
		{
			CharacterList.Add(ControlledPawn);
		}
	}
}


int32 FTeamInfo::GetNumAliveCharacters()
{
	int32 rtnValue = 0;
	for (auto& Character : Characters)
	{
		check(Character.AIController);
		auto* ControlledPawn = Cast<ABladeIIPlayer>(Character.AIController->GetPawn());
		if (ControlledPawn && ControlledPawn->IsAlive())
			++rtnValue;
	}

	return rtnValue;
}

float FTeamInfo::GetCharacterHPTotal()
{
	float TeamHPTotal = 0.f;
	for (auto& Character : Characters)
	{
		auto* B2Player = Character.AIController ? Cast<ABladeIIPlayer>(Character.AIController->GetPawn()) : nullptr;
		if (!B2Player)
			continue;

		TeamHPTotal += B2Player->GetHealth();
	}

	return TeamHPTotal;
}

int32 FTeamInfo::GetCharacterCount()
{
	int32 CharacterCount = 0;

	for (auto& Character : Characters)
	{
		auto* B2Player = Character.AIController ? Cast<ABladeIIPlayer>(Character.AIController->GetPawn()) : nullptr;
		if (!B2Player)
			continue;

		if (B2Player->GetHealth() <= 0)
			continue;
			
		++CharacterCount;
	}

	return CharacterCount;
}


void AB2TMGameMode::NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadPlayer)
{
	if (0 == MyTeamInfo.GetNumAliveCharacters() || 0 == OpponentTeamInfo.GetNumAliveCharacters())
	{
		SetMatchState(AsyncMatchState::StartResult);
	}

	UpdateHighAggroCharacters();
}

void FTeamMatchSceneMagnager::SetState(ETeamMatchState InState)
{
	BII_CHECK(GameMode);

	CurrentState = InState;

	switch (InState)
	{
	case ETeamMatchState::Finding:
		SetFinding();
		break;
	case ETeamMatchState::Ready:
		SetReady();
		break;
	case ETeamMatchState::Battle:
		SetBattle();
		break;
	case ETeamMatchState::EndMatchByTime:
		SetEndMatchByTime();
		break;
	case ETeamMatchState::BattleEnd:
		SetBattleEnd();
		break;
	default:
		break;
	}
}

void FTeamMatchSceneMagnager::SetFinding()
{
	//if (LobbyMatinee)
	//{
	//	float KeyTime = ABladeIIGameMode::GetMatineePositionByEventName(LobbyMatinee, TEXT("pvpLobby"));
	//	LobbyMatinee->SetPosition(KeyTime);
	//	LobbyMatinee->Play();

	//	ABladeIIGameMode::SetCameraMatineeCam(LobbyMatinee, KeyTime);
	//	LobbyMatinee->Pause();
	//}

	//// 이건 어쩌다 SetupUIScene 이 아닌 여기서 ChangeUIScene 을..
	//B2GMLoadingProgCollector::StepToMax(); // 로딩 진행도 이쯤서 꽉 채워넣음.
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::TeamMatchFindMatch);

	//auto* TeamMatchFindWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::TeamMatchFindMatch);
	//if (TeamMatchFindWidget)
	//{
	//	TeamMatchFindWidget->SetEnableCancel(false);
	//}
	//	
	//SetLeftEntry();

	////// 임시
	//WaitingReadyToMatchTime = 2.f; 
	//ElapsedWaitingReadyToMatchTime = 0.f;
	//////

	//auto* LocalController = UGameplayStatics::GetLocalPlayerController(GameMode);
	//BII_CHECK(LocalController);
	//if (LocalController)
	//	LocalController->ClientSetCameraFade_Implementation(true, FColor::Black, FVector2D(1.f, 0.f), 1.f, false);
}

void FTeamMatchSceneMagnager::SetReady()
{
	WaitingReadyToMatchTime = 4.5f; //temporary
	ElapsedWaitingReadyToMatchTime = 0.f;

	auto* TeamMatchFindWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::TeamMatchFindMatch);
	if (TeamMatchFindWidget)
	{
		TeamMatchFindWidget->SetReady(WaitingReadyToMatchTime);
	}

	//음.. 따로 빼기는 귀찮은 작업이고 볼륨도 작으니 그냥 하드코드로..
	BII_CHECK(SpawnedActors.Num() == 6 && AnimArray.Num() == 4);
	if (SpawnedActors.Num() == 6 && AnimArray.Num() == 4)
	{
		auto* DocTM = UB2UIDocHelper::GetDocTeamMatch();
		if (DocTM)
		{
			SpawnedActors[0]->GetSkeletalMeshComponent()->PlayAnimation(AnimArray[PCClassToInt(DocTM->GetPCCharacterClass1())].Ready_L, false);
			SpawnedActors[1]->GetSkeletalMeshComponent()->PlayAnimation(AnimArray[PCClassToInt(DocTM->GetPCCharacterClass2())].Ready_L, false);
			SpawnedActors[2]->GetSkeletalMeshComponent()->PlayAnimation(AnimArray[PCClassToInt(DocTM->GetPCCharacterClass3())].Ready_L, false);

			SpawnedActors[3]->GetSkeletalMeshComponent()->PlayAnimation(AnimArray[PCClassToInt(DocTM->GetOpponentCharacterClass1())].Ready_R, false);
			SpawnedActors[4]->GetSkeletalMeshComponent()->PlayAnimation(AnimArray[PCClassToInt(DocTM->GetOpponentCharacterClass2())].Ready_R, false);
			SpawnedActors[5]->GetSkeletalMeshComponent()->PlayAnimation(AnimArray[PCClassToInt(DocTM->GetOpponentCharacterClass3())].Ready_R, false);
		}
	}
}

void FTeamMatchSceneMagnager::SetBattle()
{
	//if (LobbyMatinee)
	//{
	//	LobbyMatinee->Stop();
	//}

	for (auto* SpawnedActor : SpawnedActors)
	{
		SpawnedActor->Destroy();
	}
	SpawnedActors.Empty();

	//ChangeScene(true);

	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::TeamMatchBattle);
	GameMode->SetMatchState(MatchState::InProgress);
}

void FTeamMatchSceneMagnager::SetEndMatchByTime()
{
	check(GameMode);

	GameMode->SetMatchState(AsyncMatchState::FinishBattle);
	GameMode->SetMatchState(AsyncMatchState::StartResult);
}

void FTeamMatchSceneMagnager::SetBattleEnd()
{
	ShowResultUiImage();
}

void FTeamMatchSceneMagnager::ShowResultUiImage()
{
	if (auto* TeamMatchBattle = UB2UIManager::GetInstance()->GetUI<UB2UITeamMatchBattle>(UIFName::TeamMatchBattle))
	{
		ENetMatchResult result(ENetMatchResult::LocalLose);

		if (auto* DocSome = UB2UIDocHelper::GetDocSome())
			result = DocSome->GetLocalPlayerMatchResult();

		TeamMatchBattle->ShowResultImage(result);
	}
}

void FTeamMatchSceneMagnager::SetGameMode(class AB2TMGameMode* InGameMode)
{
	GameMode = InGameMode;

	if (GameMode)
	{ // 레벨 리소스 네이티브 레퍼런스 캐싱 타이밍.
		GameMode->RecacheLevelResources();
	}
}

void FTeamMatchSceneMagnager::SetMatchLobbyResources(
	class ALevelSequenceActor * InMatinee, 
	const FMatchingAnims& Gladiator,
	const FMatchingAnims& Assassin,
	const FMatchingAnims& Wizard,
	const FMatchingAnims& Fighter
)
{
	AnimArray.Empty(4);
	AnimArray.Add(Gladiator);
	AnimArray.Add(Assassin);
	AnimArray.Add(Wizard);
	AnimArray.Add(Fighter);

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	LobbyMatinee = InMatinee;

	if (LobbyMatinee)
		ABladeIIGameMode::SetCameraMatineeCam(LobbyMatinee, 0.f);
}

void FTeamMatchSceneMagnager::SetMatchLobbyCharacterLocationZ(float GladiatorZ, float AssassinZ, float WizardZ, float FighterZ)
{
	CharacterLocationZ.Empty(4);
	CharacterLocationZ.Add(GladiatorZ);
	CharacterLocationZ.Add(AssassinZ);
	CharacterLocationZ.Add(WizardZ);
	CharacterLocationZ.Add(FighterZ);

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
}

void FTeamMatchSceneMagnager::SetLeftEntry()
{
	//auto* TeamMatch = UB2UIDocHelper::GetDocTeamMatch();

	//if (!TeamMatch)
	//	return;

	//auto* CharacterDataStore = GameMode->GetRemotePlayerDataStore(GameMode->GetMyTeamNum());
	//check(CharacterDataStore);

	//int32 PowerValue = 0;
	//for (int32 idx = 0; idx < GameMode->GetNumCharactersOfTeam(); ++idx)
	//{
	//	EPCClass PCClass = TeamMatch->GetCharacterClass(true, idx);

	//	FString PositionName(TEXT("Left_"));

	//	if (idx == 0)
	//		PositionName += TEXT("main");
	//	else if (idx == 1)
	//		PositionName += TEXT("sub");
	//	else if (idx == 2)
	//		PositionName += TEXT("sub1");

	//	ASkeletalMeshActor* EntryActor = SpawnPuppetActor(PositionName, LobbyMatinee, PCClass, CharacterDataStore, AnimArray[PCClassToInt(PCClass)].Idle);

	//	FVector NewLocation = EntryActor->GetActorLocation();
	//	NewLocation.Z = CharacterLocationZ[PCClassToInt(PCClass)];
	//	EntryActor->SetActorLocation(NewLocation);

	//	SpawnedActors.Add(EntryActor);

	//	PowerValue += CombatStatEval::GetPCCombatPower(PCClass, CharacterDataStore);
	//}

	//auto* FindingMatchUI = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::TeamMatchFindMatch);
	//if (FindingMatchUI)
	//{
	//	FindingMatchUI->SetUserData(true, CharacterDataStore->GetUserNickName(), CharacterDataStore->GetUserLevel(), TeamMatch->GetPCGrade(), TeamMatch->GetPCGradeStar(), TeamMatch->GetPCCurrContinuousWins(), PowerValue,
	//		TEXT(""), 0, 0, 0, 0);
	//}
}

void FTeamMatchSceneMagnager::SetRightEntry()
{
	/*/// 전부 임시 세팅
	auto* TeamMatch = UB2UIDocHelper::GetDocTeamMatch();

	if (!TeamMatch)
		return;

	auto* CharacterDataStore = GameMode->GetRemotePlayerDataStore(GameMode->GetOpponentTeamNum());
	check(CharacterDataStore);

	int32 PowerValue = 0;
	for (int32 idx = 0; idx < GameMode->GetNumCharactersOfTeam(); ++idx)
	{
		EPCClass PCClass = TeamMatch->GetCharacterClass(false, idx);
		
		FString PositionName(TEXT("Right_"));

		if (idx == 0)
			PositionName += TEXT("main");
		else if (idx == 1)
			PositionName += TEXT("sub");
		else if (idx == 2)
			PositionName += TEXT("sub1");

		ASkeletalMeshActor* EntryActor = SpawnPuppetActor(PositionName, LobbyMatinee, PCClass, CharacterDataStore, AnimArray[PCClassToInt(PCClass)].Idle);

		FVector NewLocation = EntryActor->GetActorLocation();
		NewLocation.Z = CharacterLocationZ[PCClassToInt(PCClass)];
		EntryActor->SetActorLocation(NewLocation);

		SpawnedActors.Add(EntryActor);

		PowerValue += CombatStatEval::GetPCCombatPower(PCClass, CharacterDataStore);
	}

	auto* FindingMatchUI = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::TeamMatchFindMatch);
	if (FindingMatchUI)
	{
		FindingMatchUI->SetUserData(false, CharacterDataStore->GetUserNickName(), CharacterDataStore->GetUserLevel(), TeamMatch->GetOpponentGrade(), TeamMatch->GetOpponentGradeStar(), TeamMatch->GetOpponentContinuousWins(), PowerValue,
			TEXT(""), 0, 0, 0, 0);
	}*/
}

void FTeamMatchSceneMagnager::SetStartTimeCount(bool InStart)
{
	IsStartTimeCount = InStart;

	if (IsStartTimeCount)
		TimeLimitBattle = LeftTimeBattle = 90.f;

}

void FTeamMatchSceneMagnager::Tick(float DeltaSeconds)
{
	////// 임시
	//if (CurrentState == ETeamMatchState::Finding)
	//{
	//	ElapsedWaitingReadyToMatchTime += DeltaSeconds;
	//	if (ElapsedWaitingReadyToMatchTime > WaitingReadyToMatchTime)
	//	{
	//		bool bUsedAdditionalPoint = false;// 팀매칭 티켓이 0 개일때 추가 재화를 사용할 것인지 확인 170421YJ
	//		auto* DocTeam = UB2UIDocHelper::GetDocTeamMatch();
	//		if (DocTeam)	bUsedAdditionalPoint = DocTeam->GetUsedAdditionalPoint();

	//		data_trader::Retailer::GetInstance().RequestTeamMatchmaking(bUsedAdditionalPoint);
	//		WaitingReadyToMatchTime = FLT_MAX;
	//	}			
	//}
	//else
	///////
	//if (CurrentState == ETeamMatchState::Ready)
	//{
	//	ElapsedWaitingReadyToMatchTime += DeltaSeconds;

	//	auto* TeamMatchFindWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::TeamMatchFindMatch);
	//	if (TeamMatchFindWidget)
	//	{
	//		TeamMatchFindWidget->SetCount(FMath::Min(WaitingReadyToMatchTime - ElapsedWaitingReadyToMatchTime + 0.5f, 5.f));
	//	}

	//	if (ElapsedWaitingReadyToMatchTime > WaitingReadyToMatchTime)
	//		SetState(ETeamMatchState::Battle);
	//}
	//else if (CurrentState == ETeamMatchState::Battle)
	//{
	//	if (!IsStartTimeCount)
	//		return;

	//	auto* TeamMatchBattle = UB2UIManager::GetInstance()->GetUI<UB2UITeamMatchBattle>(UIFName::TeamMatchBattle);
	//	if (!TeamMatchBattle)
	//		return;

	//	GameMode->TakeDamageUpdate();

	//	LeftTimeBattle -= DeltaSeconds;
	//	
	//	if (LeftTimeBattle > 0.0f)
	//	{
	//		TeamMatchBattle->SetBattleTime(FMath::CeilToInt(LeftTimeBattle));
	//	}
	//	else
	//	{
	//		SetState(ETeamMatchState::EndMatchByTime);
	//	}
	//}
}

void AB2TMGameMode::SetTeamMatchActiveCameraActor(AB2ActiveCameraActor* pActiveCameraFixLong, AB2ActiveCameraActor* pActiveCameraFixShort, AB2ActiveCameraActor* pActiveCameraPlayer)
{
	TeamMatchFixLongActiveCameraActor = pActiveCameraFixLong;
	TeamMatchFixShortActiveCameraActor = pActiveCameraFixShort;
	TeamMatchPlayerActiveCameraActor = pActiveCameraPlayer;
}

AB2ActiveCameraActor* AB2TMGameMode::GetTeamPlayerActiveCameraActor()
{
	return TeamMatchPlayerActiveCameraActor;
}

void AB2TMGameMode::NotifyCharacterDead(ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	Super::NotifyCharacterDead(TargetCharacter, PawnInstigator, DamageCauser);

	//auto* B2PAC = Cast<ABladeIIPlayerAIController>(TargetCharacter->GetController());
	//
	//if (!B2PAC)
	//	return;

	//// 현재 액티브카메라 뷰타겟이 죽었을때 기본 액티브카메라로 돌림
	//// 팀대전 게임모드니 당연 팀대전에서만 작동하겠지
	//auto* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (B2PC && TargetCharacter)
	//{
	//	if (TargetCharacter == B2PC->GetViewTarget())
	//	{
	//		//뷰타겟이 죽었다!
	//		//초기카메라 찾아서 활성화
	//		SetBaseActiveCamera();
	//	}
	//}

	//FString TeamName;

	//auto nAllyCount = B2PAC->GetTeamNum() == MyTeamNum ? MyTeamInfo.GetNumAliveCharacters() : OpponentTeamInfo.GetNumAliveCharacters();

	//if (nAllyCount != 0)
	//	return;

	//ABladeIICharacter* Inflictor;

	//// 투사체일수도 있어서 둘다체크
	//if (PawnInstigator)
	//	Inflictor = Cast<ABladeIICharacter>(PawnInstigator);
	//else			
	//	Inflictor = Cast<ABladeIICharacter>(DamageCauser);
	//	
	//if(Inflictor == nullptr)
	//	Inflictor = Cast<ABladeIICharacter>(DamageCauser->GetOwner());

	//if (Inflictor)
	//	StartEventScene_FinishAttack(Inflictor);

	//auto* DocSome = UB2UIDocHelper::GetDocSome();
	//if (DocSome)
	//{
	//	DocSome->SetLocalPlayerMatchResult(CalcResult());
	//}

	////[@SameLine, 180508] 여기서 호출하면 Dead마티니 중에 response를 받음 Dead마티니 끝나고 호출
	////SetMatchState(AsyncMatchState::FinishBattle);
}

void AB2TMGameMode::ChangeCharacterCamera(bool bMyTeam, int32 SlotIdx)
{
	// Not For Enemy
	if (!bMyTeam)
		return;

	GetWorldTimerManager().ClearTimer(TimeToChangeCameraTarget);

	CurrentActiveCameraPlayerIndex = SlotIdx;

	auto* pPlayer = GetTeamPlayer(bMyTeam, SlotIdx);

	if (pPlayer && TeamMatchPlayerActiveCameraActor)
	{
		TeamMatchPlayerActiveCameraActor->ChangeCameraOwnerPlayer(pPlayer);
	}		

	// UI갱신
	UpdateUICharSlotCamera();
}

bool AB2TMGameMode::ChangeFixedCamera()
{
	//auto* pCurrentActiveCameraActor = GetCurrentActiveActiveCameraActor();

	//if (pCurrentActiveCameraActor == TeamMatchPlayerActiveCameraActor)
	//{
	//	// 원래 보던 멀리보기 카메라로 전환
	//	if (TeamMatchTempActiveCameraActor)
	//		ActiveTeamMatchActiveCameraActor(TeamMatchTempActiveCameraActor);

	//	// UI갱신
	//	UpdateUICharSlotCamera();

	//	// timer
	//	GetWorldTimerManager().SetTimer(TimeToChangeCameraTarget, this, &AB2TMGameMode::ActiveTeamMatchActiveCameraActorNextTarget, 10, true);

	//	return false;
	//}
	//else
	//{
	//	// 현재 보던 멀리보기 카메라랑 다른걸로 전환
	//	if (pCurrentActiveCameraActor == TeamMatchFixLongActiveCameraActor)
	//	{
	//		ActiveTeamMatchActiveCameraActor(TeamMatchFixShortActiveCameraActor);
	//		TeamMatchTempActiveCameraActor = TeamMatchFixShortActiveCameraActor;
	//	}
	//	else
	//	{
	//		ActiveTeamMatchActiveCameraActor(TeamMatchFixLongActiveCameraActor);
	//		TeamMatchTempActiveCameraActor = TeamMatchFixLongActiveCameraActor;
	//	}

		return true;
	//}

}

void AB2TMGameMode::ActiveTeamMatchActiveCameraActor(AB2ActiveCameraActor* ActiveCameraActor)
{
	if (MyTeamInfo.GetNumAliveCharacters() == 0)
		return;

	auto* pPlayer = GetTeamPlayer(true, CurrentActiveCameraPlayerIndex);

	if (!pPlayer)
	{
		TArray<ABladeIICharacter*> AllLiveMyPlayers;
		GetAllMyTeamCharList(AllLiveMyPlayers, GetMyTeamNum(), true);
		if (AllLiveMyPlayers.Num() > 0) // 아무도 없는데 NextTarget 하면 무한반복으로 stack overflow 걸림.
		{
			ActiveTeamMatchActiveCameraActorNextTarget();
		}
		return;
	}

	if (ActiveCameraActor)
	{
		ActiveCameraActor->ChangeCameraOwnerPlayer(pPlayer);
	}
}

// 현재 선택된 플레이어 빼고 랜덤
int32 AB2TMGameMode::GetNextCameraPlayer() const
{
	TArray<int32> OtherPlayer;

	for (int32 i = 0; i < GetNumCharactersOfTeam(); ++i)
	{
		if (i != CurrentActiveCameraPlayerIndex)
		{
			OtherPlayer.Add(i);
		}
	}

	auto NextPlayerIndex =  FMath::RandRange(0, OtherPlayer.Num() - 1);
	return OtherPlayer.IsValidIndex(NextPlayerIndex) ? OtherPlayer[NextPlayerIndex] : 0;
}

void AB2TMGameMode::ActiveTeamMatchActiveCameraActorNextTarget()
{
	/*CurrentActiveCameraPlayerIndex = GetNextCameraPlayer();

	if (CurrentActiveCameraPlayerIndex >= GetNumCharactersOfTeam())
		CurrentActiveCameraPlayerIndex = 0;

	ActiveTeamMatchActiveCameraActor(GetCurrentActiveActiveCameraActor());

	UpdateUICharSlotCamera();*/
}

void AB2TMGameMode::SetBaseActiveCamera()
{
	//Super::SetBaseActiveCamera();

	//// 초기 세팅
	//TeamMatchTempActiveCameraActor = GetCurrentActiveActiveCameraActor();

	//// test
	//ActiveTeamMatchActiveCameraActor(TeamMatchTempActiveCameraActor);
	//GetWorldTimerManager().SetTimer(TimeToChangeCameraTarget, this, &AB2TMGameMode::ActiveTeamMatchActiveCameraActorNextTarget, 10, true);
	UpdateUICharSlotCamera();
}

void AB2TMGameMode::UpdateUICharSlotCamera()
{
	/*auto* TeamMatchBattle = UB2UIManager::GetInstance()->GetUI<UB2UITeamMatchBattle>(UIFName::TeamMatchBattle);
	if (!TeamMatchBattle)
		return;

	auto* LocalController = UGameplayStatics::GetLocalPlayerController(this);
	auto* ViewTarget = LocalController ? LocalController->GetViewTarget() : nullptr;
		
	for (auto& Character : MyTeamInfo.Characters)
	{
		BII_CHECK(Character.AIController);
		if (Character.AIController && Character.AIController->GetPawn() == ViewTarget)
			TeamMatchBattle->SetSlotCamera(true, true, Character.DocCharIndex);
		else
			TeamMatchBattle->SetSlotCamera(false, true, Character.DocCharIndex);
	}
	
	for (auto& Character : OpponentTeamInfo.Characters)
	{
		BII_CHECK(Character.AIController);
		if (Character.AIController && Character.AIController->GetPawn() == ViewTarget)
			TeamMatchBattle->SetSlotCamera(true, false, Character.DocCharIndex);
		else
			TeamMatchBattle->SetSlotCamera(false, false, Character.DocCharIndex);
	}*/
}

void AB2TMGameMode::StartEventScene_MatchResult(bool HasLocalPlayerWon)
{
	PlayTMBGM(HasLocalPlayerWon ? ETMBGMPlayContext::TMBGMCTX_Victory : ETMBGMPlayContext::TMBGMCTX_Lose, true);

	Super::StartEventScene_MatchResult(HasLocalPlayerWon);
}

void AB2TMGameMode::SetStartTimeCount(bool InStartCount)
{
	SceneManager.SetStartTimeCount(InStartCount);

	if (InStartCount == true)
	{
		for (auto& Character : MyTeamInfo.Characters)
		{
			check(Character.AIController);
			ABladeIITMAIPlayer* ControlledPawn = Cast<ABladeIITMAIPlayer>(Character.AIController->GetPawn());
			if (ControlledPawn)
			{
				ControlledPawn->SetUnityTick(true);
			}
		}

		for (auto& Character : OpponentTeamInfo.Characters)
		{
			check(Character.AIController);
			ABladeIITMAIPlayer* ControlledPawn = Cast<ABladeIITMAIPlayer>(Character.AIController->GetPawn());
			if (ControlledPawn)
			{
				ControlledPawn->SetUnityTick(true);
			}
		}
	}
}

void AB2TMGameMode::PlayTMBGM(ETMBGMPlayContext InContext, bool bForceStopCurrent)
{
	/*if (bForceStopCurrent)
	{
		StopBGM();
	}

	if (!StaticFindCommonSoundInfo(this))
		return;

	ECommonSoundID MatchingSoundID = ECommonSoundID::ECSID_End;
	switch (InContext)
	{
	case ETMBGMPlayContext::TMBGMCTX_Standby: MatchingSoundID = ECommonSoundID::ECSID_BGM_TMStandby; break;
	case ETMBGMPlayContext::TMBGMCTX_Combat: MatchingSoundID = ECommonSoundID::ECSID_BGM_TMCombat; break;
	case ETMBGMPlayContext::TMBGMCTX_Victory: MatchingSoundID = ECommonSoundID::ECSID_BGM_TMVictory; break;
	case ETMBGMPlayContext::TMBGMCTX_Lose: MatchingSoundID = ECommonSoundID::ECSID_BGM_Lose; break;
	}

	PlayBGMByCommonSoundID(MatchingSoundID);*/
}

void AB2TMGameMode::UpdateHighAggroCharacters()
{
	const int32 MyTeamHighAggroIdx = 0;
	const int32 OpponentTeamHighAggroIdx = 1;

	CachedHighAggroCharacter[MyTeamHighAggroIdx] = UpdateHighAggroOpponentCharacter(AB2TMGameMode::OpponentTeamNum);
	CachedHighAggroCharacter[OpponentTeamHighAggroIdx] = UpdateHighAggroOpponentCharacter(AB2TMGameMode::MyTeamNum);
}

ABladeIICharacter* AB2TMGameMode::UpdateHighAggroOpponentCharacter(int32 InMyTeamNumber)
{
	//TArray<FCharInfo> ValidCharacters;
	//const FTeamInfo& TargetTeam = (InMyTeamNumber == GetMyTeamNum()) ? OpponentTeamInfo : MyTeamInfo;
	//for (auto& CharInfo : TargetTeam.Characters)
	//{
	//	check(CharInfo.AIController);
	//	auto* Pawn = Cast<ABladeIICharacter>(CharInfo.AIController->GetPawn());
	//	if (Pawn && Pawn->IsAlive() && Pawn->CanBeTarget())
	//		ValidCharacters.Add(CharInfo);
	//}

	//if (ValidCharacters.IsValidIndex(0))
	//{
	//	ValidCharacters.Sort([](const FCharInfo& Char1, const FCharInfo& Char2) { return Char1.AggroPriority < Char2.AggroPriority ? true : false; });

	//	int32 HighAggroSameCount = 0;
	//	const int32 HighAggroFormation = ValidCharacters[0].AggroPriority;
	//	for (auto& CharInfo : ValidCharacters)
	//	{
	//		if (CharInfo.AggroPriority == HighAggroFormation)
	//			HighAggroSameCount++;
	//	}

	//	int32 HighAggroIdx = 0;
	//	if (HighAggroSameCount >= 2)
	//		HighAggroIdx = FMath::RandRange(0, HighAggroSameCount - 1);

	//	return Cast<ABladeIICharacter>(ValidCharacters[HighAggroIdx].AIController->GetPawn());
	//}

	return nullptr;
}

ABladeIICharacter* AB2TMGameMode::GetHighAggroOpponentCharacter(int32 InMyTeamNumber)
{
	const int32 OpponentTeamNumber = (InMyTeamNumber == GetMyTeamNum()) ? AB2TMGameMode::OpponentTeamNum : AB2TMGameMode::MyTeamNum;
	const int32 FindTeamIndex = OpponentTeamNumber - 1;

	if (CachedHighAggroCharacter.IsValidIndex(FindTeamIndex))
	{
		ABladeIICharacter* HighAggroCharacter = CachedHighAggroCharacter[FindTeamIndex];
		if (HighAggroCharacter && HighAggroCharacter->IsAlive())
			return HighAggroCharacter;
	}

	return UpdateHighAggroOpponentCharacter(InMyTeamNumber);
}

void AB2TMGameMode::NotifyUIWidgetOpenComplete(FName InOpenedUI)
{
	Super::NotifyUIWidgetOpenComplete(InOpenedUI);

	if (InOpenedUI == UIFName::PVPResult)
		ShowCachedCompleteQuestNotify();
}

void AB2TMGameMode::SetTeamMatchPointInfo(const FB2TeamMatchMaking& MatchInfo)
{
	/*if (auto* DocTeamMatch = UB2UIDocHelper::GetDocTeamMatch())
	{
		int32 MatchPoint = GET_TUPLE_DATA(FB2ResponseTeamMatchMaking::match_point_index, MatchInfo);
		int32 AdditionalCount = GET_TUPLE_DATA(FB2ResponseTeamMatchMaking::addadditional_match_count_index, MatchInfo);

		DocTeamMatch->SetTeamMatchPoint(MatchPoint);
		DocTeamMatch->SetTeamAdditionalMatchCount(AdditionalCount);
	}

	int32 CurrentGem = GET_TUPLE_DATA(FB2ResponseTeamMatchMaking::current_gem_index, MatchInfo);
	BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gem, CurrentGem);*/
}

TArray<EPCClass> AB2TMGameMode::GetPCClassesToPreLoad()
{
	TArray<EPCClass> RetArray;

	UB2UIDocTeamMatch* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	// 팀 게임에서는 태그를 하지 않기 때문에 여기서 캐릭터 정보를 정확히 얻어올 필요는 없지만 
	// Pre-Render 에까지 쓰려면 다 있으면 좋긴 하다. 단, Pre-render 를 하려면 매칭 된 후에 pre-render 가 들어갈 수 있도록 바뀌어야.. 
	if (TeamMatchDoc)
	{
		for (int i = 0; i < GetNumCharactersOfTeam(); ++i)
		{ // 내 것만 있다.. 상대편 정보는 매칭 정보가 전달되고 SetOpponentInfo 가 불린 후에나 가능..
			RetArray.AddUnique(TeamMatchDoc->GetCharacterClass(true, i));
		}
	}
	return RetArray; 
}

TArray<FCombinedPCSkillAnimID> AB2TMGameMode::GetPCSkillAnimsToPreLoad()
{
	return TArray<FCombinedPCSkillAnimID> {};	// Preload 하지않음 Matching State로 넘어간 이후부터 AsyncLoading
}

void AB2TMGameMode::PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie)
{
	//// TAsset 로딩하는 InfoAsset 들이 대상임. 반대는 ManualUnloadBeforeNewBeginning

	////Synchronous 로딩의 Async 로딩 flush 여부에 따라.. 이걸 다른 async request 보다 먼저 혹은 이후에..
	//PreloadInGameInfoAsset_SynchronousOnly(bAboutToPlayLoadingMovie);

	//Super::PreloadAnyNecessaryInfo(bAboutToPlayLoadingMovie);

	//PreloadInGameInfoAsset_AsyncAllowed(bAboutToPlayLoadingMovie);
}
