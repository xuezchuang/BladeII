
#include "BladeIITMAIPlayer.h"
//#include "BladeII.h"
#include "BladeIIPlayerAIController.h"
#include "B2SkillRTManager.h"
#include "B2CharacterBuffManager.h"
#include "FakeConfigure.h"

#include "B2TMGameMode.h"
#include "B2GuildGameMode.h"
#include "BladeIIProjectile.h"

#include "BladeIIGameImpl.h"
#include "B2ClientDataStore.h"

ABladeIITMAIPlayer::ABladeIITMAIPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamOtherCharClasses[0] = TeamOtherCharClasses[1] = EPCClass::EPC_End;
	MyTeamNumber = INDEX_NONE;
}

float ABladeIITMAIPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ABladeIITMAIPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

}

void ABladeIITMAIPlayer::BeginPlay()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_BeginPlay);
	Super::BeginPlay();

	FormalSetPointLightOn(false); // 어차피 IsHeadPointLightAllowed() 가 false 여야..
}

void ABladeIITMAIPlayer::PossessedBy(AController* InController)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_PossessedBy);
	BII_CHECK(InController);

	Super::PossessedBy(InController);

	auto* MyController = Cast<ABladeIIPlayerAIController>(GetController());
	if (MyController)
	{
		MyTeamNumber = MyController->GetTeamNum();
	}
}

int32 ABladeIITMAIPlayer::GetTeamNum() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_GetTeamNum);
	check(MyTeamNumber != INDEX_NONE);

	return MyTeamNumber;
}

bool ABladeIITMAIPlayer::GetIsLocalPlayerTeam()
{
	auto* TeamMatchGameMode = Cast<AB2AsyncGameMode>(UGameplayStatics::GetGameMode(this));
	bool bIsMine = false;

	if(TeamMatchGameMode)
		bIsMine = (GetTeamNum() == TeamMatchGameMode->GetMyTeamNum());

	return bIsMine;
}

bool ABladeIITMAIPlayer::IsFineToStartAnyNewInputHandling()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_IsFineToStartAnyNewInputHandling);
	return IsAlive();
}

bool ABladeIITMAIPlayer::IsFineToStartAnyNewMoveInputHandling()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_IsFineToStartAnyNewMoveInputHandling);
	if (!GetAbleToMove())
	{
		return false;
	}

	return IsAlive();
}

UB2SkillRTManager* ABladeIITMAIPlayer::GetSkillRTManager()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_GetSkillRTManager);
	if (CachedSkillRTManager == nullptr)
	{
		//태그가 불가능한 Player이기 때문에 이곳에서 SkillRTManager를 생성한다.
		CachedSkillRTManager = NewObject<UB2SkillRTManager>(this, *FString::Printf(TEXT("SkillRTManager_%s"), *this->GetName()), RF_Transient);
		CachedSkillRTManager->InitSkillRTManager(this);
	}

	return CachedSkillRTManager;
}

ICharacterDataStore* ABladeIITMAIPlayer::GetCharacterDataStore()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_GetCharacterDataStore);
	if (CharacterDataStore != nullptr)
		return CharacterDataStore;

	auto* TeamMatchGameMode = GetGameMode();
	if (TeamMatchGameMode)
	{
		auto* MyController = Cast<ABladeIIPlayerAIController>(GetController());
		check(MyController);
		CharacterDataStore = TeamMatchGameMode->GetRemotePlayerDataStore(MyController->GetTeamNum());
	}

	return CharacterDataStore;
}

void ABladeIITMAIPlayer::SetTeamCharClasses(EPCClass TeamMate1, EPCClass TeamMate2, EPCClass TeamMate3)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_SetTeamCharClasses);
	TeamOtherCharClasses[0] = GetCurrentPlayerClass() == TeamMate1 ? TeamMate2 : TeamMate1;
	TeamOtherCharClasses[1] = TeamOtherCharClasses[0] == TeamMate1 ? (GetCurrentPlayerClass() == TeamMate2 ? TeamMate3 : TeamMate2) : TeamMate3;
}

#include "B2CombatStatEvaluator.h"
#include "B2LobbyInventory.h"
void ABladeIITMAIPlayer::InitializeCombatStats()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_InitializeCombatStats);
	GetSkillRTManager();
	GetCharacterDataStore();

	const EPCClass CurrPCClassEnum = GetCurrentPlayerClass();

	check(CachedSkillRTManager != nullptr && CharacterDataStore != nullptr)

	TArray<FB2Item> MyEquippedItem, Other1EquippedItem, Other2EquippedItem;
	CharacterDataStore->GetEquippedItems(CurrPCClassEnum, MyEquippedItem);
	CharacterDataStore->GetEquippedItems(TeamOtherCharClasses[0], Other1EquippedItem);
	CharacterDataStore->GetEquippedItems(TeamOtherCharClasses[1], Other2EquippedItem);
		
	/*
	FB2Wing MyWingData; // bMyHasWing 이 true 여야 valid 함.
	const bool bMyHasWing = CharacterDataStore->GetCharacterWing(CurrPCClassEnum, MyWingData);
	FB2Wing Other1WingData; // Other1HasWing 이 true 여야 valid 함.
	const bool bOther1HasWing = CharacterDataStore->GetCharacterWing(GetTagPlayerClass(), Other1WingData);
	FB2Wing Other2WingData; // Other2HasWing 이 true 여야 valid 함.
	const bool bOther2HasWing = CharacterDataStore->GetCharacterWing(GetTagPlayerClass(), Other2WingData);
	*/

	CharacterLevel = CharacterDataStore->GetCharacterLevel(CurrPCClassEnum);

	float CurrPCAttack = CombatStatEval::GetPCAttack(CurrPCClassEnum, CharacterDataStore);
	float TeamCharAttack_0 = CombatStatEval::GetPCAttack(TeamOtherCharClasses[0], CharacterDataStore);
	float TeamCharAttack_1 = CombatStatEval::GetPCAttack(TeamOtherCharClasses[1], CharacterDataStore);

	auto* GameMode = Cast<AB2TMGameMode>(GetGameMode());
	if (GameMode)
	{
		CurrPCAttack += CombatStatEval::GetPCAttackByMod(CurrPCClassEnum, GameMode->GetB2GameModeType(), CharacterDataStore);
		TeamCharAttack_0 += CombatStatEval::GetPCAttackByMod(TeamOtherCharClasses[0], GameMode->GetB2GameModeType(), CharacterDataStore);
		TeamCharAttack_1 += CombatStatEval::GetPCAttackByMod(TeamOtherCharClasses[1], GameMode->GetB2GameModeType(), CharacterDataStore);
	}
	
	CurrPCAttack = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(CurrPCClassEnum))), CurrPCAttack);
	TeamCharAttack_0 = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(TeamOtherCharClasses[0]))), TeamCharAttack_0);
	TeamCharAttack_1 = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(TeamOtherCharClasses[1]))), TeamCharAttack_1);

	TotalAttack = CurrPCAttack + TeamCharAttack_0 + TeamCharAttack_1;

	float CurrPCDefense = CombatStatEval::GetPCDefense(CurrPCClassEnum, CharacterDataStore);
	float TeamCharDefense_0 = CombatStatEval::GetPCDefense(TeamOtherCharClasses[0], CharacterDataStore);
	float TeamCharDefense_1 = CombatStatEval::GetPCDefense(TeamOtherCharClasses[1], CharacterDataStore);

	if (GameMode)
	{
		CurrPCDefense += CombatStatEval::GetPCDefenseByMod(CurrPCClassEnum, GameMode->GetB2GameModeType(), CharacterDataStore);
		TeamCharDefense_0 += CombatStatEval::GetPCDefenseByMod(TeamOtherCharClasses[0], GameMode->GetB2GameModeType(), CharacterDataStore);
		TeamCharDefense_1 += CombatStatEval::GetPCDefenseByMod(TeamOtherCharClasses[1], GameMode->GetB2GameModeType(), CharacterDataStore);
	}

	CurrPCDefense = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(CurrPCClassEnum))), CurrPCDefense);
	TeamCharDefense_0 = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(TeamOtherCharClasses[0]))), TeamCharDefense_0);
	TeamCharDefense_1 = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(TeamOtherCharClasses[1]))), TeamCharDefense_1);

	TotalDefense = CurrPCDefense + TeamCharDefense_0 + TeamCharDefense_1;

	float AttackCoefficientValue = CombatStatEval::GetDefaultCombatCoefficient();
	float DefenseCoefficientValue = CombatStatEval::GetDefaultCombatCoefficient();
	if (GetGameMode())
	{
		AttackCoefficientValue = GetGameMode()->GetAttackCoefficient();
		DefenseCoefficientValue = GetGameMode()->GetDefenseCoefficient();
	}

	AttackLevel = CombatStatEval::GetADLevel(TotalAttack, AttackCoefficientValue);
	DefenseLevel = CombatStatEval::GetADLevel(TotalDefense / 3.f, DefenseCoefficientValue);

	// Need to be change to use formula, skill/item %, etc...
	MaxHealth = CombatStatEval::GetPCHealth(CurrPCClassEnum, CharacterDataStore);

	float ModSpecializeHealth = 0.0f;
	if (GameMode)
		ModSpecializeHealth = CombatStatEval::GetPCHealthByMod(CurrPCClassEnum, GameMode->GetB2GameModeType(), CharacterDataStore);

	MaxHealth = MaxHealth + ModSpecializeHealth;
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

	if (GameMode)
	{
		//진형에 따른 보정
		BaseDamageDecreaseRate	*= (100 - GameMode->GetReduceDamageBonusPercentageByFormation(GetTeamNum())) * 0.01f;
		CharacterDamageRate		+= (GameMode->GetAttackDamageBonusPercentageByFormation(GetTeamNum()));

		//GameMode에 따른 스탯보정
		CharacterDamageRate		*= GameMode->GetDamageRateByGameModeType() * GameMode->GetOpponentHandicap(GetTeamNum());
		Health					*= GameMode->GetHealthRateByGameModeType() * GameMode->GetOpponentHandicap(GetTeamNum());
		MaxHealth				*= GameMode->GetHealthRateByGameModeType() * GameMode->GetOpponentHandicap(GetTeamNum());
	}
}

void ABladeIITMAIPlayer::StartDash(float InDashDuration, float InMaxDistance)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_StartDash);
	Super::StartDash(InDashDuration, InMaxDistance);
}

void ABladeIITMAIPlayer::StopDash()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_StopDash);
	Super::StopDash();
}

int64 ABladeIITMAIPlayer::GetAccountId()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_GetAccountId);
	if(GetIsLocalPlayerTeam())
		return BladeIIGameImpl::GetClientDataStore().GetAccountId();

	return 0;
}

EPCClass ABladeIITMAIPlayer::GetTagPlayerClass()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_GetTagPlayerClass);
	return EPCClass::EPC_End;
}

bool ABladeIITMAIPlayer::IsVisibleFloorRing()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_IsVisibleFloorRing);
	
	auto* MyController = Cast<ABladeIIPlayerAIController>(GetController());
	auto* TeamMatchGameMode = Cast<AB2AsyncGameMode>(UGameplayStatics::GetGameMode(this));

	check(MyController && TeamMatchGameMode);

	return MyController->GetTeamNum() == TeamMatchGameMode->GetMyTeamNum();
}

void ABladeIITMAIPlayer::NotifyPlayerDeadEnd()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_NotifyPlayerDeadEnd);
	Super::NotifyPlayerDeadEnd();

	//[@AKI, 170629] [FIXME] Destroy 된 후에 노티를 하는 것 같음.....
	auto* MyController = Cast<ABladeIIPlayerAIController>(GetController());

	//check(MyController);

	if(MyController)
		MyController->StopAutoPlay();
}

float ABladeIITMAIPlayer::UpdateTarget(bool bInputGuided, bool bOnlyQTEEnabled /*= false*/)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_UpdateTarget);
	if (ABladeIIPlayerAIController* AIController = Cast<ABladeIIPlayerAIController>(GetController()))
	{
		ABladeIICharacter* TargetCharacter = AIController->PickOpponent();
		if (TargetCharacter != nullptr)
		{
			TargetActor = TargetCharacter;

			FVector DistanceToTarget = TargetCharacter->GetActorLocation() - GetActorLocation();
			return DistanceToTarget.Size2D();
		}
	}

	return Super::UpdateTarget(bInputGuided, bOnlyQTEEnabled);
}

int32 ABladeIITMAIPlayer::GetGameEntryID() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_GetGameEntryID);
	// 게임 레벨 안에서 본 캐릭터를 식별 가능하게 할 ID 가 됨. 팀 번호를 반영한다.
	return (GetTeamNum() * 10) + PCClassToInt(GetCurrentPlayerClass());
}
bool ABladeIITMAIPlayer::HasValidGameEntryID() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_HasValidGameEntryID);
	return (GetTeamNum() != INDEX_NONE); // 아직 TeamNumber 를 부여받지 못한 상황에서는 GameEntryID 를 쓰면 안됨.
}

int32 ABladeIITMAIPlayer::GetHighPrioritySkillIndex()
{
	for (int32 PriorityIdx = 0; PriorityIdx < MAX_SKILL_INPUT; PriorityIdx++)
	{
		if (AutoSkillPriority.IsValidIndex(PriorityIdx))
		{
			const int32 SkillIndex = AutoSkillPriority[PriorityIdx];

			// 내 캐릭터들일 경우에 결속스킬 자동으로 안나가도록.
			if (SkillIndex == WEAPON_6STAR_SKILL_INDEX && GetIsLocalPlayerTeam())
				continue;

			if (CachedSkillRTManager->GetStartableSkill(SkillIndex, false) != EAttackState::ECS_None)
				return SkillIndex;
		}
	}
	  
	return INDEX_NONE;
}

EAttackState ABladeIITMAIPlayer::StartSkillInner(int32 SkillInputIndex, bool IsPlayerInput)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_StartSkillInner);
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIITMAIPlayer::StartSkillInner, Current AttackState %d"), static_cast<int32>(GetAttackState())));

	const EAttackState FinalAttackState = Super::StartSkillInner(SkillInputIndex, IsPlayerInput);

	if (FinalAttackState == EAttackState::ECS_WeaponSkill)
	{
		// 팀대전의 경우 같은 편 다른 캐릭터들 스킬 포인트도 초기화 .
		auto* TeamMatchGameMode = Cast<AB2TMGameMode>(UGameplayStatics::GetGameMode(this));
		if (TeamMatchGameMode)
			TeamMatchGameMode->OnOtherPlayerUnityPointClear(this);
	}

	return FinalAttackState;
}

void ABladeIITMAIPlayer::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser)
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_OnDeath);
	
	auto* TeamMatchGameMode = Cast<AB2TMGameMode>(UGameplayStatics::GetGameMode(this));
	if (TeamMatchGameMode)
		TeamMatchGameMode->OnMyTeamDead_AddUnityPoint(this->GetTeamNum());

	this->SetWeaponSkillPoint(0.0f); // 죽은사람 결속스킬 포인트 초기화

	Super::OnDeath(KillingDamage, DamageEvent, InstigatingPawn, DamageCauser);
}