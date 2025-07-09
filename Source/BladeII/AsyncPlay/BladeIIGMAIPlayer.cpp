#include "BladeII.h"
#include "BladeIIGMAIPlayer.h"

#include "BladeIIPlayerAIController.h"
#include "B2SkillRTManager.h"
#include "B2CombatStatEvaluator.h"
#include "B2LobbyInventory.h"

ABladeIIGMAIPlayer::ABladeIIGMAIPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ABladeIIGMAIPlayer::InitializeCombatStats()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIGMAIPlayer_InitializeCombatStats);
	GetSkillRTManager();
}

void ABladeIIGMAIPlayer::InitCombateStatsForParam(const FCombatStatParam& CombatStatParam)
{
	CharacterDataStore = CombatStatParam.CharacterDataStore;

	const EPCClass CurrPCClassEnum = GetCurrentPlayerClass();

	check(CharacterDataStore != nullptr)

	if(CurrPCClassEnum != EPCClass::EPC_End)
		CharacterLevel = CharacterDataStore->GetCharacterLevel(CurrPCClassEnum);

	TotalAttack = 0;
	TotalDefense = 0;
	for (auto& Class : CombatStatParam.AddStatClasses)
	{
		float ClassAttack = 0.0;
		float ClassDefense = 0.0f;

		ClassAttack += CombatStatEval::GetPCAttack(Class, CharacterDataStore, -1, nullptr, true);
		ClassDefense += CombatStatEval::GetPCDefense(Class, CharacterDataStore, -1, nullptr, true);
		if (GetGameMode())
		{
			ClassAttack += CombatStatEval::GetPCAttackByMod(Class, GetGameMode()->GetB2GameModeType(), CharacterDataStore);
			ClassDefense += CombatStatEval::GetPCDefenseByMod(Class, GetGameMode()->GetB2GameModeType(), CharacterDataStore);
		}
		ClassAttack = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(Class))), ClassAttack);
		ClassDefense = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(Class))), ClassDefense);

		TotalAttack += ClassAttack;
		TotalDefense += ClassDefense;
	}

	TotalAttack *= 2.0f;
	TotalDefense *= 2.0f;

	float AttackCoefficientValue = CombatStatEval::GetDefaultCombatCoefficient();
	float DefenseCoefficientValue = CombatStatEval::GetDefaultCombatCoefficient();
	if (GetGameMode())
	{
		AttackCoefficientValue = GetGameMode()->GetAttackCoefficient();
		DefenseCoefficientValue = GetGameMode()->GetDefenseCoefficient();
	}

	AttackLevel = CombatStatEval::GetADLevel(TotalAttack, AttackCoefficientValue);
	DefenseLevel = CombatStatEval::GetADLevel(TotalDefense, DefenseCoefficientValue);

	// Need to be change to use formula, skill/item %, etc...
	MaxHealth = CombatStatEval::GetPCHealth(CurrPCClassEnum, CharacterDataStore, -1, nullptr, true);
	if (GetGameMode())
		MaxHealth += CombatStatEval::GetPCHealthByMod(CurrPCClassEnum, GetGameMode()->GetB2GameModeType(), CharacterDataStore);
	MaxHealth = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseHealth(CharacterDataStore->GetCharacterLevel(CurrPCClassEnum))), MaxHealth);

	Health = MaxHealth;
	// No formula for armor for now
	Armor = MaxArmor;

	// 기본 공/방/체 이외의 전투 옵션 수치들 캐싱.
	InitializeCombatOptionCacheValues();

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{ // PCClassInfo 에서는 기본값만을 넣고 아이템이랑 스킬 옵션 등이 반영된 walk speed 적용
		MovementComp->MaxWalkSpeed = GetStaticOptionAppliedWalkSpeed();
	}

	//진형에 따른 보정
	BaseDamageDecreaseRate *= (100 - CombatStatParam.ReduceDamageBonusByFormation) * 0.01f;
	CharacterDamageRate += CombatStatParam.AttackBonusByFormation;

	//GameMode에 따른 스탯보정
	CharacterDamageRate *= CombatStatParam.ModeDamageRate;
	Health *= CombatStatParam.ModeHealthRate;
	MaxHealth *= CombatStatParam.ModeHealthRate;
}

ICharacterDataStore * ABladeIIGMAIPlayer::GetCharacterDataStore()
{
	return CharacterDataStore;
}


void ABladeIIGMAIPlayer::InitializePCClassInfoSpawn(ABladeIIGameMode* InGameMode, UB2PCClassInfo* PCClassInfo)
{
	if (InGameMode)
	{
		OverrideByPCClassInfo(InGameMode, PCClassInfo, false);
		InjectGameMode(InGameMode);
	}
}

void ABladeIIGMAIPlayer::SetPlayerInfo(const FGuildPlayerInfo& InPlayerInfo)
{
	CacheGuildPlayerInfo = InPlayerInfo;
}

const FGuildPlayerInfo & ABladeIIGMAIPlayer::GetPlayerInfo()
{
	return CacheGuildPlayerInfo;
}

const FGuildPlayerInfo & ABladeIIGMAIPlayer::GetGuildPlayerInfo()
{
	return CacheGuildPlayerInfo;
}

bool ABladeIIGMAIPlayer::PawnIsAlive()
{
	return IsAlive();
}

ABladeIICharacter * ABladeIIGMAIPlayer::GetPawnInstance()
{
	return this;
}

FVector ABladeIIGMAIPlayer::GetLocation() const
{
	return GetActorLocation();
}

AController * ABladeIIGMAIPlayer::GetPawnController()
{
	return GetController();
}

FString ABladeIIGMAIPlayer::GetPlayerStartName()
{
	return FString::Printf(TEXT("P%d_%d"), TeamNum, CacheGuildPlayerInfo.Spot);
}
