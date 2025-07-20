// Fill out your copyright notice in the Description page of Project Settings.


#include "B2DimensionGameMode.h"
//#include "BladeII.h"
#include "B2UIManager.h"
#include "B2StageManager.h"
#include "B2StageInfo.h"
#include "B2MonsterSpawnPool.h"
#include "Event.h"
#include "B2UIDocBattle.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "B2GeneralBuffs.h"
#include "B2SkillRTManager.h"
#include "B2UIBattleMain.h"
#include "B2UIBattleSkill.h"
#include "B2CombatStatEvaluator.h"

#include "BladeIIGameImpl.h"

AB2DimensionGameMode::AB2DimensionGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void AB2DimensionGameMode::PreBeginPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DimensionGameMode::PreBeginPlay"));
	GameRule = new DimensionGameRule;
	SetGameRule(GameRule);
	CashedSpawnPool = nullptr;

	Super::PreBeginPlay();
}
 
void AB2DimensionGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		DocBattle->SetCurDimensionBuffAttackCount(0);
		DocBattle->SetCurDimensionBuffDefenseCount(0);
		//DocBattle->SetNeedResurrectBuffText(false);
	}

	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (B2Player)
	{
		B2Player->SetUnityTick(true);
	}
}

void AB2DimensionGameMode::SubscribeEvents()
{
	if (bEventsSubscribed)
		return;

	Issues.Add(DeliveryGetDimensionClass<FB2ResponseGetDimensionPtr>::GetInstance().Subscribe2(
			[this](const FB2ResponseGetDimensionPtr& msgPtr)
	{
		auto* pDoc = UB2UIDocHelper::GetDocDimension();

		if (pDoc)
			pDoc->SetDimensionPtr(msgPtr);

		this->GotoDimensionMain();
	}
	));
	Issues.Add(DimensionBuffCheckClass<>::GetInstance().Subscribe2(
		[this]()
	{
		this->CheckAura();
	}
	));

	Super::SubscribeEvents();
}

void AB2DimensionGameMode::SetupUIScene()
{
	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::DimensionBattle);
}

void AB2DimensionGameMode::BeginDestroy()
{
	// BeginDestroy 矫痢俊辑狼 UIWidget destroy 绰 林狼且 鞘夸啊 乐澜.
	CashedSpawnPool = nullptr;

	Super::BeginDestroy();

}

void AB2DimensionGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AB2DimensionGameMode::PlayVictoryBGM()
{
	PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_HeroTWVictory);
}

void AB2DimensionGameMode::GiveUpGame()
{
	bool bTestVar = StageManager->IsStageCleared();

	if (!(StageManager && StageManager->IsStageCleared()))
	{
		StageManager->RequestDimensionClear(EDimensionResult::GIVEUP);
	}
}

float AB2DimensionGameMode::GetCurrentStageDifficultyScale()
{
	int32 nDiff = 1;
	float m_fCachingMobLevel = 1;

	auto DocDimension = UB2UIDocHelper::GetDocDimension();
	if (DocDimension)
		nDiff = DocDimension->GetDifficulty();
	
	FB2DimensionInfo* CurrentInfo = BladeIIGameImpl::GetTotemDataStore().GetDimensionInfo(nDiff);
	if(CurrentInfo)
		m_fCachingMobLevel = CurrentInfo->Difficulty_scale / 100.0f;
	
	UE_LOG(LogBladeII, Warning, TEXT("AB2DimensionGameMode::SetMobLevel() nFloor :[%d], m_fCachingMobLevel : [%f]"), nDiff, m_fCachingMobLevel);

	return m_fCachingMobLevel;
}

void AB2DimensionGameMode::NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadPlayer)
{
	bool bTestVar = StageManager->IsStageCleared();

	if (!(StageManager && StageManager->IsStageCleared()))
	{
		StageManager->RequestDimensionClear(EDimensionResult::DEFEAT);
	}
}

void AB2DimensionGameMode::GoToVillage()
{	
	// 俊矾惑炔老锭 咯扁肺... 捞扒 肺厚肺 捞悼矫难淋
	FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
		// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst) {
			LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_LobbyMain);
		}
	});

	OpenBladeIILobbyCommon(this); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
}

void AB2DimensionGameMode::GoToVillageDefeated()
{
	GoToMap();
}

void AB2DimensionGameMode::GoToMap()
{
	// 弥辆利栏肺 叼膏记 沥焊 盎脚茄促澜俊 皋牢栏肺 柳涝
	data_trader::Retailer::GetInstance().RequestGetDimension();
}

void AB2DimensionGameMode::GotoDimensionMain()
{
	FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
		// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst) {
			UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
			UIMgrInst->ArtificialAddUIHistory(EUIScene::EnterDungeonMain);
		}
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_DimensionMain);
	});

	OpenBladeIILobbyCommon(this); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
}

void AB2DimensionGameMode::DecideAndSetActiveSpawnPool(bool bFallBackToLowerDifficulty)
{
	// 扁夯 悼累苞绰 崔府 StageId 客 Difficulty 甫 绊妨秦辑 嘎绰 吧 啊廉柯促.
	// 胶抛捞瘤 矫累 矫痢俊 茄锅 沥窍搁 官差瘤 臼绰促.
	if (StageManager)
	{
		auto DocDimension = UB2UIDocHelper::GetDocDimension();
		if (DocDimension)
			SetForcedPerStageSettings(SpawnPools, DocDimension->GetClientStageID(), static_cast<int32>(EStageDifficulty::ESD_Normal));

		TheActiveSpawnPool = GetDesiredSpawnPoolOfSettingFromList(SpawnPools,
			// 霸烙捞电, PlayInEditor 电 StageManager 狼 PreBeginPlay 捞饶扼搁 StageId 唱 DifficultyLevel 殿捞 荤侩 啊瓷秦具.
			StageManager->GetCurrentClientStageId(), StageManager->GetStageDifficultyLevel(), bFallBackToLowerDifficulty);
	}
	else
	{
		Super::DecideAndSetActiveSpawnPool(); // 惑困窜 扁瓷栏肺 弃归
	}
}

void AB2DimensionGameMode::NotifySpawnPoolMobSpawn(class ABladeIICharacter* SpawnedMob, const FMobSpawnedBirthplaceInfo& InBirthplaceInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DimensionGameMode::NotifySpawnPoolMobSpawn"));
	Super::NotifySpawnPoolMobSpawn(SpawnedMob, InBirthplaceInfo);
}

void AB2DimensionGameMode::NotifySpawnPoolSingleWaveStart(AB2MonsterSpawnPool* StartedSpawnPool, int32 WaveNum)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DimensionGameMode::NotifySpawnPoolSingleWaveStart"));
	Super::NotifySpawnPoolSingleWaveStart(StartedSpawnPool, WaveNum);
	
	CashedSpawnPool = StartedSpawnPool;

	TArray<ABladeIICharacter*> WaveModeList;
	if(CashedSpawnPool)	
		CashedSpawnPool->GetAllWaveMobList(WaveModeList);

	// 咯扁辑 各吝俊 胶沛捞 乐促搁 buff 
	for (auto Mob : WaveModeList)
	{
		if (Mob && IsValid(Mob) && Mob->IsAlive())
		{
			if (CheckBloodStone(Mob))
			{
				if (!IsBuffTargetMe(Mob->BloodStoneType))
				{
					SetttingAura_Mob(Mob->BloodStoneType, Mob->BloodStoneAmount);
				}
				else
				{
					SetttingAura_Me(Mob->BloodStoneType, Mob->BloodStoneBuffLogicType, Mob->BloodStoneAmount, Mob);
				}
				break;
			}
		}
	}
}

void AB2DimensionGameMode::NotifySpawnPoolSingleWaveComplete(class AB2MonsterSpawnPool* CompletedSpawnPool, int32 WaveNum)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DimensionGameMode::NotifySpawnPoolSingleWaveComplete"));
	Super::NotifySpawnPoolSingleWaveComplete(CompletedSpawnPool, WaveNum);
}

void AB2DimensionGameMode::NotifySpawnPoolWaveMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob, int32 WaveNum, int32 MobIndex)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2DimensionGameMode::NotifySpawnPoolWaveMobDead"));

	if (DyingMob->bBloodStoneAura)
	{
		if (!IsBuffTargetMe(DyingMob->BloodStoneType))
		{
			ReleaseAura_Mob(DyingMob->BloodStoneType);
		}
		else
		{
			ReleaseAura_Me(DyingMob->BloodStoneType);
		}
	}

	Super::NotifySpawnPoolWaveMobDead(SpawnPool, DyingMob, WaveNum, MobIndex);
}

void AB2DimensionGameMode::NotifyCharacterDead(ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	Super::NotifyCharacterDead(TargetCharacter, PawnInstigator, DamageCauser);
}

bool AB2DimensionGameMode::CheckBloodStoneAlive()
{
	TArray<ABladeIICharacter*> WaveModeList;
	if (CashedSpawnPool)
		CashedSpawnPool->GetAllWaveMobList(WaveModeList);

	bool BloodStoneIsAlive = false;

	for (auto Mob : WaveModeList)
	{
		if (Mob && IsValid(Mob) && Mob->IsAlive())
		{
			if (CheckBloodStone(Mob))
			{
				BloodStoneIsAlive = true;
				break;
			}
		}
	}

	return BloodStoneIsAlive;
}

bool AB2DimensionGameMode::CheckBloodStone(class ABladeIICharacter* SpawnedMob)
{
	return SpawnedMob ? SpawnedMob->bBloodStoneAura : false;
}

void AB2DimensionGameMode::SetttingAura_Mob(EBloodStoneType type, float amount)
{
	TArray<ABladeIICharacter*> WaveModeList;
	if (CashedSpawnPool)
		CashedSpawnPool->GetAllWaveMobList(WaveModeList);

	switch (type)
	{
	case EBloodStoneType::EBST_UnBeatable:
	{
		for (auto Mob : WaveModeList)
		{
			if (Mob && IsValid(Mob) && Mob->IsAlive())
			{
				if (!CheckBloodStone(Mob)) // 喉矾靛胶沛捞 酒囱 傈眉 各阑 公利栏肺.
				{
					Mob->AddBuff(EBuffType::Buff_Unbeatable, -1);
				}
			}
		}
	}
	break;
	case EBloodStoneType::EBST_AllKill: // 喉矾靛 胶沛捞 颇鲍瞪锭 傈何 懦.
		break;
	}
}

void AB2DimensionGameMode::SetttingAura_Me(EBloodStoneType type, EDamageLogicType damagetype, float amount, class ABladeIICharacter* TargetCharacter)
{
	ABladeIICharacter* MyChar = Cast<ABladeIICharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (MyChar)
	{
		switch (type)
		{
		case EBloodStoneType::EBST_OnFire:
		{
			UB2Buff_Base* BuffObject = MyChar->AddBuff(EBuffType::Debuff_Burning, -1, amount, TargetCharacter);
			if (UB2Buff_DotDamage* DotBurning = Cast<UB2Buff_DotDamage>(BuffObject))
			{
				DotBurning->DotDamageLogicType = damagetype;
			}
		}
		break;
		case EBloodStoneType::EBST_OnPoision:
			MyChar->AddBuff(EBuffType::Debuff_Poison, -1, amount);
			break;
		}
	}
}

void AB2DimensionGameMode::ReleaseAura_Mob(EBloodStoneType type)
{
	// 喉矾靛 胶沛篮 茄胶迄钱俊 悼老 粮犁父 硅摹等促绰巴捞 傈力炼扒
	// 喉矾靛 胶沛捞 茄付府扼档 鞍捞 混酒乐阑版快 滚橇 蜡瘤
	if (CheckBloodStoneAlive())
		return;

	TArray<ABladeIICharacter*> WaveModeList;
	if (CashedSpawnPool)
		CashedSpawnPool->GetAllWaveMobList(WaveModeList);

	TArray<ABladeIICharacter*> TempArray = WaveModeList;
	switch (type)
	{
	case EBloodStoneType::EBST_UnBeatable:
	{
		for (int32 i = 0; i < TempArray.Num(); i++)
		{
			if (!TempArray.IsValidIndex(i))
				continue;
			auto Mob = TempArray[i];
			//if (Mob && !Mob->IsPendingKill() && Mob->IsAlive() && !Mob->bBloodStoneAura)
			{
				Mob->ClearBuff(EBuffType::Buff_Unbeatable);
			}
		}
	}
		break;
	case EBloodStoneType::EBST_AllKill: // 喉矾靛 胶沛捞 颇鲍瞪锭 傈何 懦
	{
		for (int32 i = 0; i < TempArray.Num(); i++)
		{
			if (!TempArray.IsValidIndex(i))
				continue;
			auto Mob = TempArray[i];
			//if (Mob && !Mob->IsPendingKill() && Mob->IsAlive())
			{
				Mob->Suicide();
			}
		}
	}
		break;
	}

	RemoveSpawnNotifyMessageClass<>::GetInstance().Signal();
}

void AB2DimensionGameMode::ReleaseAura_Me(EBloodStoneType type)
{
	// 喉矾靛 胶沛篮 茄胶迄钱俊 悼老 粮犁父 硅摹等促绰巴捞 傈力炼扒
	// 喉矾靛 胶沛捞 茄付府扼档 鞍捞 混酒乐阑版快 滚橇 蜡瘤
	if (CheckBloodStoneAlive())
		return;

	ABladeIICharacter* MyChar = Cast<ABladeIICharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (MyChar)
	{
		switch (type)
		{
		case EBloodStoneType::EBST_OnFire:
			MyChar->ClearBuff(EBuffType::Debuff_Burning);
			break;
		case EBloodStoneType::EBST_OnPoision:
			MyChar->ClearBuff(EBuffType::Debuff_Poison);
			break;
		}
	}

	RemoveSpawnNotifyMessageClass<>::GetInstance().Signal();
}

FTimespan AB2DimensionGameMode::GetElapsedTime()
{
	float fPlayTime = GetStageManager()->GetStagePlayTime();

	return FTimespan::FromMilliseconds(double(fPlayTime * 1000));
}

float AB2DimensionGameMode::GetTotalAttack(ICharacterDataStore* CharacterDataStore)
{
	const auto	MainPCClass = CharacterDataStore->GetMainPlayerClass();

	float		Attack = CombatStatEval::GetPCAttack(MainPCClass, CharacterDataStore);
	float		ModSpecializeAttack = CombatStatEval::GetPCAttackByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);

	float		TotalAttack = Attack + ModSpecializeAttack;
	TotalAttack = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(MainPCClass))), TotalAttack);

	return TotalAttack;
}

float AB2DimensionGameMode::GetTotalDefense(ICharacterDataStore* CharacterDataStore)
{
	const auto	MainPCClass = CharacterDataStore->GetMainPlayerClass();

	float		Defense = CombatStatEval::GetPCDefense(MainPCClass, CharacterDataStore);
	float		ModSpecializeDefense = CombatStatEval::GetPCDefenseByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);

	float		TotalDefense = Defense + ModSpecializeDefense;
	TotalDefense = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(MainPCClass))), TotalDefense);

	return TotalDefense;
}

void AB2DimensionGameMode::CheckAura()
{
	if (!CashedSpawnPool)
		return;

	TArray<ABladeIICharacter*> WaveModeList;
	if (CashedSpawnPool)
		CashedSpawnPool->GetAllWaveMobList(WaveModeList);

	// 咯扁辑 各吝俊 胶沛捞 乐促搁 buff 
	for (auto Mob : WaveModeList)
	{
		//if (Mob && !Mob->IsPendingKill() && Mob->IsAlive())
		{
			if (CheckBloodStone(Mob))
			{
				if (IsBuffTargetMe(Mob->BloodStoneType))
				{
					if(!CheckBuff_Me(Mob->BloodStoneType))
						SetttingAura_Me(Mob->BloodStoneType, Mob->BloodStoneBuffLogicType, Mob->BloodStoneAmount, Mob);
				}
				break;
			}
		}
	}
}

bool AB2DimensionGameMode::CheckBuff_Me(EBloodStoneType type)
{
	EBuffType CurrentBuff = EBuffType::Buff_None;
	ABladeIICharacter* MyChar = Cast<ABladeIICharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (MyChar)
	{
		switch (type)
		{
		case EBloodStoneType::EBST_OnFire:
			CurrentBuff = EBuffType::Debuff_Burning;
			break;
		case EBloodStoneType::EBST_OnPoision:
			CurrentBuff = EBuffType::Debuff_Poison;
			break;
		}

		UB2Buff_Base* BuffObject = MyChar->GetBuff(CurrentBuff);
		if (!BuffObject->IsActive())
			return false;
	}

	return true;
}

bool AB2DimensionGameMode::IsBuffTargetMe(EBloodStoneType type)
{
	return (static_cast<int32>(type) >= 100);
}