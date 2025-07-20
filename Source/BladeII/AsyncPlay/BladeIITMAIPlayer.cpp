
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
#include "../BladeII/BladeIIUtil.h"

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

	FormalSetPointLightOn(false); // 绢瞒乔 IsHeadPointLightAllowed() 啊 false 咯具..
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
		//怕弊啊 阂啊瓷茄 Player捞扁 锭巩俊 捞镑俊辑 SkillRTManager甫 积己茄促.
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
	FB2Wing MyWingData; // bMyHasWing 捞 true 咯具 valid 窃.
	const bool bMyHasWing = CharacterDataStore->GetCharacterWing(CurrPCClassEnum, MyWingData);
	FB2Wing Other1WingData; // Other1HasWing 捞 true 咯具 valid 窃.
	const bool bOther1HasWing = CharacterDataStore->GetCharacterWing(GetTagPlayerClass(), Other1WingData);
	FB2Wing Other2WingData; // Other2HasWing 捞 true 咯具 valid 窃.
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
	
	// 扁夯 傍/规/眉 捞寇狼 傈捧 可记 荐摹甸 某教.
	InitializeCombatOptionCacheValues();

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{ // PCClassInfo 俊辑绰 扁夯蔼父阑 持绊 酒捞袍捞尔 胶懦 可记 殿捞 馆康等 walk speed 利侩
		MovementComp->MaxWalkSpeed = GetStaticOptionAppliedWalkSpeed();
	}

	if (GameMode)
	{
		//柳屈俊 蝶弗 焊沥
		BaseDamageDecreaseRate	*= (100 - GameMode->GetReduceDamageBonusPercentageByFormation(GetTeamNum())) * 0.01f;
		CharacterDamageRate		+= (GameMode->GetAttackDamageBonusPercentageByFormation(GetTeamNum()));

		//GameMode俊 蝶弗 胶湃焊沥
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

	//[@AKI, 170629] [FIXME] Destroy 等 饶俊 畴萍甫 窍绰 巴 鞍澜.....
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
	// 霸烙 饭骇 救俊辑 夯 某腐磐甫 侥喊 啊瓷窍霸 且 ID 啊 凳. 评 锅龋甫 馆康茄促.
	return (GetTeamNum() * 10) + PCClassToInt(GetCurrentPlayerClass());
}
bool ABladeIITMAIPlayer::HasValidGameEntryID() const
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIITMAIPlayer_HasValidGameEntryID);
	return (GetTeamNum() != INDEX_NONE); // 酒流 TeamNumber 甫 何咯罐瘤 给茄 惑炔俊辑绰 GameEntryID 甫 静搁 救凳.
}

int32 ABladeIITMAIPlayer::GetHighPrioritySkillIndex()
{
	for (int32 PriorityIdx = 0; PriorityIdx < MAX_SKILL_INPUT; PriorityIdx++)
	{
		if (AutoSkillPriority.IsValidIndex(PriorityIdx))
		{
			const int32 SkillIndex = AutoSkillPriority[PriorityIdx];

			// 郴 某腐磐甸老 版快俊 搬加胶懦 磊悼栏肺 救唱啊档废.
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
		// 评措傈狼 版快 鞍篮 祈 促弗 某腐磐甸 胶懦 器牢飘档 檬扁拳 .
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

	this->SetWeaponSkillPoint(0.0f); // 磷篮荤恩 搬加胶懦 器牢飘 檬扁拳

	Super::OnDeath(KillingDamage, DamageEvent, InstigatingPawn, DamageCauser);
}