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

	DefaultPawnClass = ASpectatorPawn::StaticClass(); // SpectatorPawn 阑 扁夯栏肺
	PlayerControllerClass = ABladeIIPlayerController::StaticClass(); // 鞘夸俊 蝶扼 蝶肺 extend 秦辑 荤侩..

	DefaultPCClassEnum = EPCClass::EPC_End; // 老窜 spawn 窍瘤 臼绰促.

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
	StageBeginClass<>::GetInstance().Signal(); // 捞霸 泅犁 窍绰 扒 傈捧 UI 甫 咯绰 扒单 Stage 鄂 捞抚捞 芭浇府搁 促弗 捞亥飘肺 官层辑 镜 荐 乐摆促. 酒抗 促弗 捞亥飘啊 鞘夸且 荐档 乐绊.
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
	//货酚霸 矫累窍磊.
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
			// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促.
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
			// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
			if (UIMgrInst) {
				UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
				UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterBattleMain);
			}
			LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ElobbyScene_TeamMatchMain);
			data_trader::Retailer::GetInstance().RequestGetTeamBattleStatus();
		});
	}	
	OpenBladeIILobbyCommon(this); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
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
		// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促.
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

	//抛捞喉拳窍扁俊绰 粱 弊犯绊.. 窍靛内靛窃.
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

	//抛捞喉拳窍扁俊绰 粱 弊犯绊.. 窍靛内靛窃.
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

	//	// 弥檬胶迄篮 P1B_1 捞悼且镑篮 P1_1 捞繁屈侥栏肺
	//	// 捞悼篮 TeamPlayerMoveToStartPosition 窃荐俊辑
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

	///////烙矫	
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

	// 楷免 矫累傈俊 扁夯墨皋扼乐栏搁 茫酒辑 劝己拳
	// 勤靛迄俊辑绰 牧飘费矾狼 何犁肺 咀萍宏 墨皋扼 咀磐狼
	// BeginPlay()俊辑 劝己拳 救登绰版快啊 乐绰淀
	SetBaseActiveCamera();

#if BII_STATS // PerfStat 篮 MatchStart 矫痢何磐 葛栏扁 矫累.
	PerfStatCollector.OnBegin(this);
#endif

	// 楷免矫累
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

	// 搬苞 沥焊甫 UI 率俊 技泼窍绰 何盒篮 备泅 沥府登搁 茄 镑栏肺
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
	//[@AKI, 170607] 傈捧啊 场车栏唱 付瘤阜 TakeDamageUpdate甫 救秦淋栏肺 咯扁俊 眠啊窃
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

		// 捞亥飘 叼泛磐俊辑 见变芭尔 捞悼 泪辟芭 钱绢淋
		ABladeIICharacter* pCharacter = Cast<ABladeIICharacter>(Character.AIController->GetPawn());

		if (pCharacter)
		{
			pCharacter->SetActorHiddenInGame(false);
			pCharacter->AllowMovementByStageEvent();
		}

		// 沥秦柳 镑栏肺 捞悼
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

	//儡粮 某腐磐 HP钦魂
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

	//		// 冠胶 搬苞技泼窍扁傈俊. 盔贰波尔 厚背
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
	//				//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 俺急
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
	////[@AKI, 170613] 距 匙飘况农俊辑 搬苞甫 辑滚俊辑 罐瘤 臼篮 惑怕俊辑 搬苞拳搁 焊咯林瘤 臼霸 函版
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::PVPResult);
}

void AB2TMGameMode::Tick(float DeltaSeconds)
{
//	SceneManager.Tick(DeltaSeconds);
//
//	if (IsInPreRenderPhase()) {
//		UpdateForPreRenderObjects(); // 捞霸 倒酒啊绰 客吝俊绰 酒流 沥侥 矫累 救 茄 芭..
//		// Pre-render 俊辑 皋牢 甘 肺爹 拳搁 摧绰 吧 瘤楷矫虐霸 登骨肺 咯扁辑 贸府. Pre-render 侩 肺爹 拳搁篮 蝶肺 乐绊 捞扒 窜瘤 肺爹 槐臂捞 倒酒啊绰 芭 楷厘窍扁 困茄 瞒盔捞促.
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

	// 泅犁 GetCharacterList 备泅惑 CharacterList 俊 盲况持瘤 给窍绰 版快 牢郸胶啊 撇妨廉 滚府骨肺 牢郸胶肺 焊绰 巴 磊眉啊 巩力烙.
	// 搬惫 咯辨 瘤唱辑 酒贰 SpawnTeamPlayerAsPuppet 栏肺 啊绰 霸 咯傈洒 巩力 惑炔捞 瞪 荐 乐澜.
	if (CharacterList.IsValidIndex(nPlayerIndex))
	{
		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(CharacterList[nPlayerIndex]);
		if (pPlayer)
			return pPlayer;
	}

	// 捞固 磷绢辑 给 啊廉坷绰 版快..
	if (!bLiveOnly && bSpawnPuppetForDead)
	{
		return SpawnTeamPlayerAsPuppet(bAllyTeam, nPlayerIndex, false);
	}

	return nullptr;
}

// 措眉肺 楷免俊 荤侩且 格利烙. 楷免俊 荤侩窍扁 困秦 spawn.
ABladeIIPlayer* AB2TMGameMode::SpawnTeamPlayerAsPuppet(bool bAllyTeam, int32 nPlayerIndex, bool bPossessByAIController /*= true*/)
{
	// 努贰胶 沥焊啊 咯扁 富绊 谍 单 绝唱..
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
	if (InCharClass == EPCClass::EPC_End)
	{
		return nullptr;
	}

	// 扁霖捞 瞪 敲饭捞绢啊 绝阑 荐 乐促.. 敲饭捞绢 start 扼档 镜鳖.. 
	FTransform NewTransform(
		FRotator(0.0f, 0.0f, 0.0f),
		FVector(0.0f, 0.0f, 0.0f),
		FVector(1.0f, 1.0f, 1.0f)
	);

	// 迭洒 捞傈 吧 绝矩促电瘤 且 鞘夸 绝捞 弊措肺 府畔. 罐篮 率俊辑 肋 贸府. 
	// PlayerController 俊 possess 窍瘤绰 臼栏聪 GetLocalPlayerCharacter 殿捞 狼档客 崔府 咯扁辑 积己茄 巴阑 府畔茄促电瘤 窍绰 老篮 绝阑 巴.
	ABladeIIPlayer* SpawnedPuppet = ABladeIIGameMode::SpawnPlayerCharAsPuppet(this, InCharClass, NewTransform, bPossessByAIController, ABladeIIPlayer::StaticClass());

	// 扁夯利栏肺绰 肺拿 敲饭捞绢 某腐磐 厘厚肺 嘎苗廉 乐阑抛聪 Ally 啊 酒聪扼搁 厘厚 单捞磐甫 流立 技泼秦霖促. 
	if (!bAllyTeam)
	{ // 窜, 霸烙葛靛俊 某教等 吧 啊廉客辑 荤侩窍妨搁 NetID 甫 持绢林绢具 窍绰单 泅犁 SetupComponentsForPartsCustomDataStore 备泅 磊眉啊 Custom 俊 吧嘎霸 某教阑 公矫窍档废 登绢乐促.
		SpawnedPuppet->SetupComponentsForPartsCustomDataStore(GetRemotePlayerDataStore(GetOpponentTeamNum()), 
			// 捞芭 芭狼 楷免侩 扁瓷捞扼 SectionMerge 咯何俊 楷免侩 魄喊 窃荐 府畔蔼 傈崔.
			UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(this)
			);
	}

	return SpawnedPuppet;
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

	//// 捞扒 绢录促 SetupUIScene 捞 酒囱 咯扁辑 ChangeUIScene 阑..
	//B2GMLoadingProgCollector::StepToMax(); // 肺爹 柳青档 捞码辑 菜 盲况持澜.
	//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::TeamMatchFindMatch);

	//auto* TeamMatchFindWidget = UB2UIManager::GetInstance()->GetUI<UB2UIPVPModFindingMatch>(UIFName::TeamMatchFindMatch);
	//if (TeamMatchFindWidget)
	//{
	//	TeamMatchFindWidget->SetEnableCancel(false);
	//}
	//	
	//SetLeftEntry();

	////// 烙矫
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

	//澜.. 蝶肺 哗扁绰 蓖满篮 累诀捞绊 杭俘档 累栏聪 弊成 窍靛内靛肺..
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
	{ // 饭骇 府家胶 匙捞萍宏 饭欺繁胶 某教 鸥捞怪.
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
	/*/// 傈何 烙矫 技泼
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
	////// 烙矫
	//if (CurrentState == ETeamMatchState::Finding)
	//{
	//	ElapsedWaitingReadyToMatchTime += DeltaSeconds;
	//	if (ElapsedWaitingReadyToMatchTime > WaitingReadyToMatchTime)
	//	{
	//		bool bUsedAdditionalPoint = false;// 评概莫 萍南捞 0 俺老锭 眠啊 犁拳甫 荤侩且 巴牢瘤 犬牢 170421YJ
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

	//// 泅犁 咀萍宏墨皋扼 轰鸥百捞 磷菌阑锭 扁夯 咀萍宏墨皋扼肺 倒覆
	//// 评措傈 霸烙葛靛聪 寸楷 评措傈俊辑父 累悼窍摆瘤
	//auto* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//if (B2PC && TargetCharacter)
	//{
	//	if (TargetCharacter == B2PC->GetViewTarget())
	//	{
	//		//轰鸥百捞 磷菌促!
	//		//檬扁墨皋扼 茫酒辑 劝己拳
	//		SetBaseActiveCamera();
	//	}
	//}

	//FString TeamName;

	//auto nAllyCount = B2PAC->GetTeamNum() == MyTeamNum ? MyTeamInfo.GetNumAliveCharacters() : OpponentTeamInfo.GetNumAliveCharacters();

	//if (nAllyCount != 0)
	//	return;

	//ABladeIICharacter* Inflictor;

	//// 捧荤眉老荐档 乐绢辑 笛促眉农
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

	////[@SameLine, 180508] 咯扁辑 龋免窍搁 Dead付萍聪 吝俊 response甫 罐澜 Dead付萍聪 场唱绊 龋免
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

	// UI盎脚
	UpdateUICharSlotCamera();
}

bool AB2TMGameMode::ChangeFixedCamera()
{
	//auto* pCurrentActiveCameraActor = GetCurrentActiveActiveCameraActor();

	//if (pCurrentActiveCameraActor == TeamMatchPlayerActiveCameraActor)
	//{
	//	// 盔贰 焊带 钢府焊扁 墨皋扼肺 傈券
	//	if (TeamMatchTempActiveCameraActor)
	//		ActiveTeamMatchActiveCameraActor(TeamMatchTempActiveCameraActor);

	//	// UI盎脚
	//	UpdateUICharSlotCamera();

	//	// timer
	//	GetWorldTimerManager().SetTimer(TimeToChangeCameraTarget, this, &AB2TMGameMode::ActiveTeamMatchActiveCameraActorNextTarget, 10, true);

	//	return false;
	//}
	//else
	//{
	//	// 泅犁 焊带 钢府焊扁 墨皋扼尔 促弗吧肺 傈券
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
		if (AllLiveMyPlayers.Num() > 0) // 酒公档 绝绰单 NextTarget 窍搁 公茄馆汗栏肺 stack overflow 吧覆.
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

// 泅犁 急琶等 敲饭捞绢 哗绊 罚待
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

	//// 檬扁 技泼
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
	// 评 霸烙俊辑绰 怕弊甫 窍瘤 臼扁 锭巩俊 咯扁辑 某腐磐 沥焊甫 沥犬洒 掘绢棵 鞘夸绰 绝瘤父 
	// Pre-Render 俊鳖瘤 静妨搁 促 乐栏搁 亮变 窍促. 窜, Pre-render 甫 窍妨搁 概莫 等 饶俊 pre-render 啊 甸绢哎 荐 乐档废 官差绢具.. 
	if (TeamMatchDoc)
	{
		for (int i = 0; i < GetNumCharactersOfTeam(); ++i)
		{ // 郴 巴父 乐促.. 惑措祈 沥焊绰 概莫 沥焊啊 傈崔登绊 SetOpponentInfo 啊 阂赴 饶俊唱 啊瓷..
			RetArray.AddUnique(TeamMatchDoc->GetCharacterClass(true, i));
		}
	}
	return RetArray; 
}

TArray<FCombinedPCSkillAnimID> AB2TMGameMode::GetPCSkillAnimsToPreLoad()
{
	return TArray<FCombinedPCSkillAnimID> {};	// Preload 窍瘤臼澜 Matching State肺 逞绢埃 捞饶何磐 AsyncLoading
}

void AB2TMGameMode::PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie)
{
	//// TAsset 肺爹窍绰 InfoAsset 甸捞 措惑烙. 馆措绰 ManualUnloadBeforeNewBeginning

	////Synchronous 肺爹狼 Async 肺爹 flush 咯何俊 蝶扼.. 捞吧 促弗 async request 焊促 刚历 趣篮 捞饶俊..
	//PreloadInGameInfoAsset_SynchronousOnly(bAboutToPlayLoadingMovie);

	//Super::PreloadAnyNecessaryInfo(bAboutToPlayLoadingMovie);

	//PreloadInGameInfoAsset_AsyncAllowed(bAboutToPlayLoadingMovie);
}
