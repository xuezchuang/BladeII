// Fill out your copyright notice in the Description page of Project Settings.
#include "B2HeroTowerGameMode.h"
#include "BladeII.h"

#include "B2UIManager.h"
#include "B2StageManager.h"
#include "Event.h"


#include "BladeIIGameImpl.h"


void AB2HeroTowerGameMode::SetupUIScene()
{
	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::HeroTowerBattle);
}

void AB2HeroTowerGameMode::SubscribeEvents()
{
	Super::SubscribeEvents();

	Issues.Add(ReturnToHeroTowerMainMenuClass<>::GetInstance().Subscribe2(
		[this]()
		{
			this->ReturnToHTMainMenu();
		}		
	));
}

ABladeIICharacter* AB2HeroTowerGameMode::SpawnCharacter(UB2NPCClassInfoBox* InfoBox, ENPCClass InClass, ENPCClassVariation InVariation, FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2HeroTowerGameMode::SpawnCharacter"));

	return Super::SpawnCharacter(InfoBox, InClass, InVariation, SpawnTransform, SpawnParameters);
}

void AB2HeroTowerGameMode::ReturnToHTMainMenu()
{
	//if (MatchState != AsyncMatchState::StartResult)
	//{
	//	data_trader::Retailer::GetInstance().RequestTeamMatchResult(PlayToken, false);
	//}

	FLobbySceneManager::DeferredRegistChangeLobbyScene([](){
		// ��ü �� ��ȯ�� �ϸ� �� �ε� �ð��� �þ�Ƿ� ���ϴ� ȭ�� �������� UI History �� ������ش�. 
		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst) {
			UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
			UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterDungeonMain);
		}
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_HeroTowerReady);
	});

	OpenBladeIILobbyCommon(this); // �� ��ȯ ���� �� ���� �κ� �� �ε�
}

void AB2HeroTowerGameMode::GiveUpGame()
{
	if (GetStageManager())
	{
		StageManager->RequestAbandonHeroTower();
	}
}

float AB2HeroTowerGameMode::GetCurrentStageDifficultyScale()
{
	UE_LOG(LogBladeII, Warning, TEXT("AB2HeroTowerGameMode::GetCurrentStageDifficultyScale() m_fCachingMobLevel : [%f]"), m_fCachingMobLevel);
	return m_fCachingMobLevel;
}

void AB2HeroTowerGameMode::NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadPlayer)
{
	bool bTestVar = StageManager->IsStageCleared();

	if (!(StageManager && StageManager->IsStageCleared())) // �й� �޴��� ���� �Ǵµ� �������� Ŭ���� ���Ŀ��� �۵����� �ʰ�..
	{
		PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_CombatDefeat); // ���� �״� �ٷ� �� �������� �÷��� �ؾ� �Ѵٸ� DeadEnd �� �ƴ� OnDeath ���� ������.. 

		StageManager->RequestHeroTowerClear(false);
	}
}

void AB2HeroTowerGameMode::PreBeginPlay()
{
	Super::PreBeginPlay();

	SetMobLevel();
}

void AB2HeroTowerGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	ABladeIIPlayer* B2Player = PC ? Cast<ABladeIIPlayer>(PC->GetCharacter()) : nullptr;

	if (B2Player)
	{
		B2Player->SetUnityTick(true);
	}
}

void AB2HeroTowerGameMode::SetMobLevel()
{
	// �ʷ��� �����س�
	int32 nFloor = GetStageManager()->GetCacheStageKeepEssentialData().GetHeroTowerFloor();

	m_fCachingMobLevel = BladeIIGameImpl::GetClientDataStore().GetHeroTowerLevel(nFloor);
	UE_LOG(LogBladeII, Warning, TEXT("AB2HeroTowerGameMode::SetMobLevel() nFloor :[%d], m_fCachingMobLevel : [%f]"), nFloor, m_fCachingMobLevel);
}

void AB2HeroTowerGameMode::PlayVictoryBGM()
{
	PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_HeroTWVictory);
}

void AB2HeroTowerGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AB2HeroTowerGameMode::ResetAssumeNoWorldLoading()
{
	Super::ResetAssumeNoWorldLoading();

	SetMobLevel();
}

FTimespan AB2HeroTowerGameMode::GetElapsedTime()
{
	float fPlayTime = GetStageManager()->GetStagePlayTime();

	return FTimespan::FromMilliseconds(double(fPlayTime * 1000));
}

int32 AB2HeroTowerGameMode::GetHeroTowerFloor()
{
	return GetStageManager()->GetCacheStageKeepEssentialData().GetHeroTowerFloor();
}



void AB2HeroTowerGameMode::NotifyUIWidgetOpenComplete(FName InOpenedUI)
{
	Super::NotifyUIWidgetOpenComplete(InOpenedUI);

	if (InOpenedUI == UIFName::HeroTowerResult)
		ShowCachedCompleteQuestNotify();
}


void AB2HeroTowerGameMode::OnEndMatinee_MobDead()
{
	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::HeroTowerResult);
}