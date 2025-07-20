// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeIIMercenaryPlayer.h"
//#include "BladeII.h"
#include "B2GuildDataStore.h"
#include "B2ClientDataStore.h"
#include "B2CombatStatEvaluator.h"
#include "BladeIIGameImpl.h"
#include "B2GameInstance.h"
#include "B2GuildNPCInfo.h"
#include "B2CombatStatEvaluator.h"
//#include "B2GuildNPCAIController.h"
#include "B2SomeInfo.h"
#include "B2PassiveSkillFactory.h"
#include "../BladeII/BladeIIUtil.h"


bool FMercenarySkillData::IsRemainCooltime()
{
	return RemainSkillCoolTime > 0.0f;
}

void FMercenarySkillData::ResetSkillCoolTime()
{
	RemainSkillCoolTime = 0.0f;
}


ABladeIIMercenaryPlayer::ABladeIIMercenaryPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CacheAttackIndex = INDEX_NONE;
	bSkillControlledPlayer = false;
	ReadyWeaponSkillFX = nullptr;
	ReadyWeaponSkillPSC = nullptr;
	FloorRingPS = nullptr;
	bReservedUseSkill = false;
	IsInAttackState = false;
	CombatStatEval::InitCombatStatMap(CombatStats);
	SkillStats.Empty();
	PassiveSkills.Empty();
	CharacterType = ECharacterType::ECT_Mercenary;
}

void ABladeIIMercenaryPlayer::InitializeCombatStats()
{
	//Super::InitializeCombatStats();
	//Do Nothing
	//GetSkillRTManager();
}

void ABladeIIMercenaryPlayer::InitCombateStatsForParam(const FCombatStatParam & CombatStatParam)
{
	CharacterLevel = MercenaryInfo.MercenaryLevel;

	float SumAttack = 0, SumDefense = 0, SumHealth = 0;
#if !UE_EDITOR
	check(CombatStatParam.AddStatClasses.Num() != 0);
#endif
	float ClassCounter = CombatStatParam.AddStatClasses.Num();

	if (ClassCounter != 0)
	{
		for (auto& PCClass : CombatStatParam.AddStatClasses)
		{
			float ClassAttack = 0.0f;
			float ClassDefense = 0.0f;
			float ClassHealth = 0.0f;

			ClassAttack += CombatStatEval::GetPCAttack(PCClass, CombatStatParam.CharacterDataStore, -1, nullptr, true);
			ClassDefense += CombatStatEval::GetPCDefense(PCClass, CombatStatParam.CharacterDataStore, -1, nullptr, true);
			ClassHealth += CombatStatEval::GetPCHealth(PCClass, CombatStatParam.CharacterDataStore, -1, nullptr, true);

			if (GetGameMode())
			{
				ClassAttack += CombatStatEval::GetPCAttackByMod(PCClass, GetGameMode()->GetB2GameModeType(), CombatStatParam.CharacterDataStore);
				ClassDefense += CombatStatEval::GetPCDefenseByMod(PCClass, GetGameMode()->GetB2GameModeType(), CombatStatParam.CharacterDataStore);
				ClassHealth += CombatStatEval::GetPCHealthByMod(PCClass, GetGameMode()->GetB2GameModeType(), CombatStatParam.CharacterDataStore);
			}

			ClassAttack = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CombatStatParam.CharacterDataStore->GetCharacterLevel(PCClass))), ClassAttack);
			ClassDefense = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CombatStatParam.CharacterDataStore->GetCharacterLevel(PCClass))), ClassDefense);
			ClassHealth = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseHealth(CombatStatParam.CharacterDataStore->GetCharacterLevel(PCClass))), ClassHealth);

			SumAttack += ClassAttack;
			SumDefense += ClassDefense;
			SumHealth += ClassHealth;
		}

		TotalAttack = SumAttack * 2.0f;
		TotalDefense = SumDefense * 2.0f;
		MaxHealth = SumHealth / ClassCounter;
	}
	else
	{
		TotalAttack = CombatStatEval::GetMobBaseAttack(CharacterLevel);
		TotalDefense = CombatStatEval::GetMobBaseDefense(CharacterLevel);
		MaxHealth = CombatStatEval::GetMobBaseHealth(CharacterLevel);
	}

	float AttackCoefficientValue = CombatStatEval::GetDefaultCombatCoefficient();
	float DefenseCoefficientValue = CombatStatEval::GetDefaultCombatCoefficient();
	if (GetGameMode())
	{
		AttackCoefficientValue = GetGameMode()->GetAttackCoefficient();
		DefenseCoefficientValue = GetGameMode()->GetDefenseCoefficient();
	}

	AttackLevel = CombatStatEval::GetADLevel(TotalAttack, AttackCoefficientValue);
	DefenseLevel = CombatStatEval::GetADLevel(TotalDefense, DefenseCoefficientValue);

	/////////////////////////////////////////////
	//侩捍 饭骇喊 瓷仿摹 眠啊
	FB2GuildMercenaryInfo& Infos = BladeIIGameImpl::GetGuildDataStore().GetGuildMercenaryMasterData();
	b2network::B2mdGuildMercenaryMasteryPtr MasteryPtr = Infos.GetMercenaryMasteries(MercenaryInfo.MercenaryID, MercenaryInfo.MercenaryLevel);

	if (MasteryPtr)
	{
		AddMercenaryMastery(SvrToCliMercenarySkillOption(MasteryPtr->option_id_1), MasteryPtr->option_value_1);
		AddMercenaryMastery(SvrToCliMercenarySkillOption(MasteryPtr->option_id_2), MasteryPtr->option_value_2);
		AddMercenaryMastery(SvrToCliMercenarySkillOption(MasteryPtr->option_id_3), MasteryPtr->option_value_3);
	}
	
	InitializeCombatOptionCacheValues();
	
	b2network::B2mdGuildMercenaryPtr GuildMercenary = Infos.GetMercenaries(MercenaryInfo.MercenaryID);
	if (GuildMercenary)
		MaxArmor = GuildMercenary->max_armor_override;

	//PC客 促弗 侩捍傈侩甸 眠啊秦具 凳
	if(CombatStats.Contains(EUnitedCombatOptions::UCO_Health_IncMaxHP))
		MaxHealth *= (1.0f + CombatStats[EUnitedCombatOptions::UCO_Health_IncMaxHP]);

	Health = MaxHealth;
	Armor = MaxArmor;
	CurrentQTEBreakPoint = 0.f;

	///////////////////////////////////////////
	//GameMode俊 蝶弗 胶湃焊沥
	CharacterDamageRate *= CombatStatParam.ModeDamageRate;
	Health *= CombatStatParam.ModeHealthRate;
	MaxHealth *= CombatStatParam.ModeHealthRate;
}

void ABladeIIMercenaryPlayer::SetMercenaryInfo(const FGuildMercenaryInfo & InMercenaryInfo)
{
	MercenaryInfo = InMercenaryInfo;
	SetMercenarySkill(InMercenaryInfo);
}

void ABladeIIMercenaryPlayer::SetMercenaryUsingState(TSharedPtr<FNPCUsingSkillState> InUsingSkillState)
{
	UsingSkillState = InUsingSkillState;
}

const FGuildMercenaryInfo & ABladeIIMercenaryPlayer::GetMercenaryInfo()
{
	return MercenaryInfo;
}

bool ABladeIIMercenaryPlayer::IsSkillUsable()
{
	return GetAbleToControl();
}

bool ABladeIIMercenaryPlayer::IsComboAttacking() const
{
	return ABladeIICharacter::IsComboAttacking();
}

bool ABladeIIMercenaryPlayer::IsCounterAttacking() const
{
	return ABladeIICharacter::IsCounterAttacking();
}

bool ABladeIIMercenaryPlayer::HadHugeDamaged()
{
	return ABladeIICharacter::HadHugeDamaged();
}

bool ABladeIIMercenaryPlayer::IsGuarding()
{
	return ABladeIICharacter::IsGuarding();
}

ABladeIICharacter * ABladeIIMercenaryPlayer::GetNearestEnemyCharacter()
{
	return ABladeIICharacter::GetNearestEnemyCharacter();
}

void ABladeIIMercenaryPlayer::PostActorCreated()
{
	ABladeIICharacter::PostActorCreated();
}

//ENPCClass ABladeIIMercenaryPlayer::GetNPCClass() const
//{
//	return ABladeIICharacter::GetNPCClass();
//}

ENPCClassVariation ABladeIIMercenaryPlayer::GetNPCClassVariation() const
{
	return ABladeIICharacter::GetNPCClassVariation();
}

bool ABladeIIMercenaryPlayer::GetAbleToMove()
{
	return ABladeIICharacter::GetAbleToMove();
}

FVector ABladeIIMercenaryPlayer::GetVelocity() const
{
	return ABladeIICharacter::GetVelocity();
}

bool ABladeIIMercenaryPlayer::CanDie(float KillingDamage, FDamageEvent const & DamageEvent, AController * Killer, AActor * DamageCauser) const
{
	return ABladeIICharacter::CanDie(KillingDamage, DamageEvent, Killer, DamageCauser);
}

void ABladeIIMercenaryPlayer::OnDeath(float KillingDamage, FDamageEvent const & DamageEvent, APawn * InstigatingPawn, AActor * DamageCauser)
{
	ABladeIICharacter::OnDeath(KillingDamage, DamageEvent, InstigatingPawn, DamageCauser);
}

void ABladeIIMercenaryPlayer::InitUpdateUIDoc()
{
	ABladeIICharacter::InitUpdateUIDoc();
}

float ABladeIIMercenaryPlayer::GetBonusBuffDamageDecreaseAmount() const
{
	return ABladeIICharacter::GetBonusBuffDamageDecreaseAmount();
}

float ABladeIIMercenaryPlayer::GetBuffReflectionDamageRate() const
{
	return ABladeIICharacter::GetBuffReflectionDamageRate();
}

float ABladeIIMercenaryPlayer::GetBonusBuffHealHPByHit() const
{
	return SkillStats.Contains(EMercenarySkillOption::EMSO_HealHPOnHitOnBuff) ? SkillStats[EMercenarySkillOption::EMSO_HealHPOnHitOnBuff] : 0;
}

float ABladeIIMercenaryPlayer::GetBonusBuffHealHP() const
{
	return ABladeIICharacter::GetBonusBuffHealHP();
}

float ABladeIIMercenaryPlayer::GetBonusBuffIncMoveSpeed() const
{
	return ABladeIICharacter::GetBonusBuffIncMoveSpeed();
}

float ABladeIIMercenaryPlayer::GetBonusBuffReceiveDamageDecreaseRate() const
{
	return ABladeIICharacter::GetBonusBuffReceiveDamageDecreaseRate();
}

float ABladeIIMercenaryPlayer::GetBonusBuffAttackDamageIncreaseRate() const
{
	return ABladeIICharacter::GetBonusBuffAttackDamageIncreaseRate();
}

float ABladeIIMercenaryPlayer::GetAttackDamageIncreaseRateByClass(EPCClass TargetClass) const
{
	return ABladeIICharacter::GetAttackDamageIncreaseRateByClass(TargetClass);
}

float ABladeIIMercenaryPlayer::GetReceiveDamageDecreaseRateByClass(EPCClass AttackerClass) const
{
	return ABladeIICharacter::GetReceiveDamageDecreaseRateByClass(AttackerClass);
}

float ABladeIIMercenaryPlayer::GetDamageRadiusIncreaseRate() const
{
	return ABladeIICharacter::GetDamageRadiusIncreaseRate();
}

int32 ABladeIIMercenaryPlayer::GetBonusBuffAffectNum() const
{
	return SkillStats.Contains(EMercenarySkillOption::EMSO_IncMaxNumCharacterBuff) ? SkillStats[EMercenarySkillOption::EMSO_IncMaxNumCharacterBuff] : 0;
}

float ABladeIIMercenaryPlayer::GetBonusBuffDuration() const
{
	return SkillStats.Contains(EMercenarySkillOption::EMSO_BuffTime) ? SkillStats[EMercenarySkillOption::EMSO_BuffTime] : 0;
}

bool ABladeIIMercenaryPlayer::CanBeTarget() const
{
	//return ABladeIICharacter::CanBeTarget();
	return true;
}

float ABladeIIMercenaryPlayer::GetStaticOptionAppliedWalkSpeed() const
{
	return ABladeIICharacter::GetStaticOptionAppliedWalkSpeed();
}

bool ABladeIIMercenaryPlayer::ProcessEtherSetEffect(EEtherSetType EtherSetType, ABladeIICharacter * EtherCauser, const FDamageInfo & CauserDamageInfo, float ActualDamage)
{
	return ABladeIICharacter::ProcessEtherSetEffect(EtherSetType, EtherCauser, CauserDamageInfo, ActualDamage);
}

UMaterialInstanceDynamic * ABladeIIMercenaryPlayer::GetChainMaterial()
{
	return ABladeIICharacter::GetChainMaterial();
}

bool ABladeIIMercenaryPlayer::IsForceApplyHitAnimation()
{
	return ABladeIICharacter::IsForceApplyHitAnimation();
}

void ABladeIIMercenaryPlayer::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	ABladeIICharacter::FaceRotation(NewControlRotation, DeltaTime);
}

bool ABladeIIMercenaryPlayer::IsSkillAttacking() const
{
	return (GetAttackState() == EAttackState::ECS_Combo01) && IsCurrentAttackIndexSkill();
}

void ABladeIIMercenaryPlayer::BeginPlay()
{
	ABladeIICharacter::BeginPlay();

	//if (ActionCameraComponent)
	//{
	//	ActionCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	//}

	//// UpdateCamera** 阑 烹秦 技泼且 锭俊绰 Camera** 蔼甸捞 鞍捞 某教凳.
	//CameraDistance = CameraBoom->TargetArmLength;
	//CameraPitch = CameraBoom->RelativeRotation.Pitch;
	//CameraYaw = CameraBoom->RelativeRotation.Yaw;
	//PlayerTopDownCamInitialFOV = TopDownCameraComponent->FieldOfView;

	//CameraLengthValueForBlending.TargetArmLength = CameraDistance;
	//CameraFovValueForBlending.CameraFov = PlayerTopDownCamInitialFOV;

	//FormalSetPointLightOn(false);
}

void ABladeIIMercenaryPlayer::BeginDestroy()
{
	PassiveSkills.Empty();
	ABladeIICharacter::BeginDestroy();
}

void ABladeIIMercenaryPlayer::EnableInput(APlayerController * PlayerController)
{
	ABladeIICharacter::EnableInput(PlayerController);
}

void ABladeIIMercenaryPlayer::DisableInput(APlayerController * PlayerController)
{
	ABladeIICharacter::DisableInput(PlayerController);
}

void ABladeIIMercenaryPlayer::Destroyed()
{
	ABladeIICharacter::Destroyed();
}

void ABladeIIMercenaryPlayer::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
}

float ABladeIIMercenaryPlayer::GetMoveAcceptableRadius()
{
	return ABladeIICharacter::GetMoveAcceptableRadius();
}

float ABladeIIMercenaryPlayer::GetShield() const
{
	return ABladeIICharacter::GetShield();
}

void ABladeIIMercenaryPlayer::SetShield(const float NewShieldAmount)
{
	ABladeIICharacter::SetShield(NewShieldAmount);
}

bool ABladeIIMercenaryPlayer::CanRotateToTarget()
{
	if (IsStunned() || !IsAlive() || CustomTimeDilation <= 0.f || !GetAbleToMove())
		return false;

	return true;
}

bool ABladeIIMercenaryPlayer::IsCurrentAttackIndexSkill() const
{
	for (const TSharedPtr<FMercenarySkillData>& SkillData : MercenarySkillDatas)
	{
		if (CacheAttackIndex == SkillData->SkillAttackIndex)
			return true;
	}
	return false;
}

bool ABladeIIMercenaryPlayer::IsAnotherNPCUsingSkill()
{
	return (UsingSkillState.IsValid() && UsingSkillState->bUsingSkill);
}

bool ABladeIIMercenaryPlayer::StartUseSkill()
{
	if (!IsActiveUseSkill())
		return false;

	TSharedPtr<FMercenarySkillData> UseSkillData = GetUseSkillData();
	AIAttackIndex = UseSkillData->SkillAttackIndex;
	SetSignalAttack(true);

	return true;
}

void ABladeIIMercenaryPlayer::ReserveUseSkill()
{
	if (IsActiveUseSkill(true))
		StartUseSkill();
	else
		bReservedUseSkill = true;
}

bool ABladeIIMercenaryPlayer::IsActiveUseSkill(bool bCheckCharacterState)
{
	if (bCheckCharacterState && IsSkillAttacking())
		return false;

	if (bCheckCharacterState && !IsSkillUsable())
		return false;

	TSharedPtr<FMercenarySkillData> UseSkillData = GetUseSkillData();

	if (IsBuffActive(EBuffType::DeBuff_Silent))
		return false;

	if (IsAnotherNPCUsingSkill())
		return false;

	if (!UseSkillData.IsValid())
		return false;

	if (UseSkillData->IsRemainCooltime())
		return false;

	return true;
}

void ABladeIIMercenaryPlayer::SetPlayerControlSkill(bool bControl)
{
	bSkillControlledPlayer = bControl;
}

void ABladeIIMercenaryPlayer::Tick(float DeltaSeconds)
{
	ABladeIICharacter::Tick(DeltaSeconds);

	UpdateCameraBoom(DeltaSeconds);

	UpdateSkill(DeltaSeconds);
}

void ABladeIIMercenaryPlayer::SetTeamNum(int32 InTeamNum)
{
	ABladeIICharacter::SetTeamNum(InTeamNum);
}

void ABladeIIMercenaryPlayer::StopAttack()
{
	SetSignalAttack(false);
}

void ABladeIIMercenaryPlayer::SetAttackState(EAttackState InAttackState)
{
	ABladeIICharacter::SetAttackState(InAttackState);
	if (GetAttackState() == EAttackState::ECS_Combo01)
		CacheAttackIndex = AIAttackIndex;
	else
		CacheAttackIndex = INDEX_NONE;
	
	if(IsInAttackState)
		OnMercenaryStartSkill(CacheAttackIndex);
}

void ABladeIIMercenaryPlayer::OnAttackState()
{
	IsInAttackState = true;
	
	if (GetAttackState() == EAttackState::ECS_Combo01)
		CacheAttackIndex = AIAttackIndex;
	else
		CacheAttackIndex = INDEX_NONE;
	
	OnMercenaryStartSkill(CacheAttackIndex);
}

void ABladeIIMercenaryPlayer::OnEndAttackState()
{
	IsInAttackState = false;
}


void ABladeIIMercenaryPlayer::OnAnimMobDeadEnd(bool bInUseDeadRagDoll, float InRemainLifeSpan, bool bInUseDeadSink, float InDeadSinkDelayTime, float InDeadSinkZOffset, float InDeadSinkDuration)
{
	PreventMovementByStageEvent(); // 拱府甫 荤侩窍瘤 臼栏搁辑 movment stop 鳖瘤 窍搁 沥富 犁固绝澜.

	check(GetCapsuleComponent());
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

bool ABladeIIMercenaryPlayer::GetAreaDamageTargetLocation(float RandMaxDist, FVector & OutTargetLocation, const FVector & LocationOffset, bool bSelfTarget)
{
	//OutTargetLocation = FVector::ZeroVector;

	//ABladeIICharacter* Target = nullptr;
	//if (bSelfTarget)
	//{
	//	Target = this;
	//}
	//else
	//{
	//	//--固摹摆匙
	//	AB2GuildNPCAIController* NPCController = Cast<AB2GuildNPCAIController>(GetController());
	//	if (NPCController)
	//	{
	//		Target = NPCController->GetCurrentTarget();
	//		if (!Target)
	//		{
	//			NPCController->UpdateTarget();
	//			Target = NPCController->GetCurrentTarget();
	//		}
	//	}
	//}
	//	
	//if (Target)
	//{
	//	InnerGetAreaDamageTargetLocation(Target, RandMaxDist, OutTargetLocation, LocationOffset);
	//	return true;
	//}

	return false;
}

void ABladeIIMercenaryPlayer::PreDamageAnimation(const FDamageInfo& DamageInfo, bool IsArmorBreak)
{
	auto ArmorBreakResponse = GetSuitableDamageType(DamageInfo, IsArmorBreak);

	//酒赣 宏饭捞农 寸秦辑 AttackType捞 汲沥登菌阑锭父 甸绢啊档废..
	if (ArmorBreakResponse == EAttackType::EAT_Default)
		return;

	ApplyDamageAnimation(ArmorBreakResponse);
	UpdateDamageNum(DamageInfo);
}

bool ABladeIIMercenaryPlayer::CalcAIAttackIndex(float TargetDistance, int32 & OutAttackIndex, float & OutRadius)
{
	int32 NewAIAttackIndex = INDEX_NONE;
	int32 CandidateAttackIndex = INDEX_NONE;

	const TArray<FAIAttackData>& DataArray = GetAIAttackArray();
	const TArray<int32>& InvalidIndices = GetInvalidAttackIndices();

	// Use 0 index as default when no AIAttackArray available
	if (DataArray.Num() == 0)
	{
		NewAIAttackIndex = 0;
		CandidateAttackIndex = 0;
		// no return for logging case
	}
	else
	{
		TBitArray<> InvalidBitArray(false, DataArray.Num());
		for (int32 i = 0; i < InvalidIndices.Num(); ++i)
		{
			if (InvalidBitArray.IsValidIndex(InvalidIndices[i]))
				InvalidBitArray[InvalidIndices[i]] = true;
		}

		float TotalWeight = 0.f, TotalWeight2 = 0.f;
		TBitArray<> BitIndexArray;
		TBitArray<> BitIndexArray2;
		for (int32 i = 0; i < DataArray.Num(); ++i)
		{
			if (!InvalidBitArray[i] && TargetDistance >= DataArray[i].MinDistance)
			{
				BitIndexArray2.Add(true);
				TotalWeight2 += DataArray[i].PossibilityWeight;
				if (TargetDistance < DataArray[i].MaxDistance)
				{
					BitIndexArray.Add(true);
					TotalWeight += DataArray[i].PossibilityWeight;
				}
				else
				{
					BitIndexArray.Add(false);
				}
			}
			else
			{
				BitIndexArray.Add(false);
				BitIndexArray2.Add(false);
			}
		}

		float ConditionalTotalWeight = TotalWeight;
		float ConditionalTotalWeight2 = TotalWeight2;

		bool IsContain = BitIndexArray.IsValidIndex(AIAttackIndex) && BitIndexArray[AIAttackIndex] == true;
		if (IsContain)
		{
			ConditionalTotalWeight2 -= DataArray[AIAttackIndex].PossibilityWeight;
			if (TargetDistance < DataArray[AIAttackIndex].MaxDistance)
			{
				ConditionalTotalWeight -= DataArray[AIAttackIndex].PossibilityWeight;
			}
		}

		if (ConditionalTotalWeight > 0.f)
		{
			NewAIAttackIndex = GetAIAttackIndex(BitIndexArray, ConditionalTotalWeight, AIAttackIndex, DataArray);
		}
		else if (ConditionalTotalWeight2 > 0.f)
		{
			CandidateAttackIndex = GetAIAttackIndex(BitIndexArray2, ConditionalTotalWeight2, AIAttackIndex, DataArray);
		}
		else if (TotalWeight > 0.f)
		{
			float Weight = FMath::FRand() * TotalWeight;
			float CurrentWeight = 0.f;
			for (int32 i = 0; i < BitIndexArray.Num(); ++i)
			{
				if (BitIndexArray[i])
				{
					CurrentWeight += DataArray[i].PossibilityWeight;
					if (Weight <= CurrentWeight)
					{
						NewAIAttackIndex = i;
						break;
					}
				}
			}
		}
		else if (TotalWeight2 > 0.f)
		{
			float Weight = FMath::FRand() * TotalWeight2;
			float CurrentWeight = 0.f;
			for (int32 i = 0; i < BitIndexArray2.Num(); ++i)
			{
				if (BitIndexArray2[i])
				{
					CurrentWeight += DataArray[i].PossibilityWeight;
					if (Weight <= CurrentWeight)
					{
						CandidateAttackIndex = i;
						break;
					}
				}
			}
		}

		if (NewAIAttackIndex >= 0 && NewAIAttackIndex < DataArray.Num())
		{
			OutRadius = DataArray[NewAIAttackIndex].MaxDistance;
		}
		else if (CandidateAttackIndex >= 0 && CandidateAttackIndex < DataArray.Num())
		{
			OutRadius = DataArray[CandidateAttackIndex].MaxDistance;
		}
	}
	OutAttackIndex = NewAIAttackIndex;

	return NewAIAttackIndex != INDEX_NONE;
}

const FGuildPlayerInfo & ABladeIIMercenaryPlayer::GetGuildPlayerInfo()
{
	return MercenaryInfo;
}

bool ABladeIIMercenaryPlayer::PawnIsAlive()
{
	return IsAlive();
}

ABladeIICharacter * ABladeIIMercenaryPlayer::GetPawnInstance()
{
	return this;
}

FVector ABladeIIMercenaryPlayer::GetLocation() const
{
	return GetActorLocation();
}

AController * ABladeIIMercenaryPlayer::GetPawnController()
{
	return GetController();
}

FString ABladeIIMercenaryPlayer::GetPlayerStartName()
{
	return FString::Printf(TEXT("P%d_%d"), TeamNum, MercenaryInfo.Spot);
}

const TArray<int32>& ABladeIIMercenaryPlayer::GetInvalidAttackIndices()
{
	NPCInvalidIndices.Empty();
	NPCInvalidIndices.Append(ABladeIICharacter::GetInvalidAttackIndices());
	
	for (TSharedPtr<FMercenarySkillData>& SkillData : MercenarySkillDatas)
	{
		if (!SkillData->bAvailableSkill)
		{
			NPCInvalidIndices.Add(SkillData->SkillAttackIndex);
			continue;
		}
		if (SkillData->bUseSkill && bReservedUseSkill && IsActiveUseSkill())
		{
			for (int32 index = 0; index < GetAIAttackArray().Num(); index++)
			{
				if (index != SkillData->SkillAttackIndex)
					NPCInvalidIndices.Add(index);
			}
			break;
		}
		if (SkillData->bUseSkill && (bSkillControlledPlayer || IsAnotherNPCUsingSkill()))
		{
			NPCInvalidIndices.Add(SkillData->SkillAttackIndex);
			continue;
		}
		if (!IsSkillUsable() || SkillData->IsRemainCooltime())
		{
			NPCInvalidIndices.Add(SkillData->SkillAttackIndex);
			continue;
		}

	}

	return NPCInvalidIndices;
}

void ABladeIIMercenaryPlayer::OnMercenaryStartSkill(int32 InSkillAttackIndex)
{
	for (TSharedPtr<FMercenarySkillData>& SkillData : MercenarySkillDatas)
	{
		if (SkillData->bAvailableSkill && SkillData->SkillAttackIndex == InSkillAttackIndex && SkillData->RemainSkillCoolTime <= 0.0f)
		{
			SkillData->RemainSkillCoolTime = SkillData->MaxSkillCoolTime;
			bReservedUseSkill = false;
			return;
		}
	}
}

void ABladeIIMercenaryPlayer::UpdateSkill(float DeltaTime)
{
	const float NonCustomDelatedDeltaSec = GetDeltaTimeAsOtherWorldObj(DeltaTime, this);

	for (TSharedPtr<FMercenarySkillData>& SkillData : MercenarySkillDatas)
	{
		if (SkillData->bAvailableSkill && SkillData->IsRemainCooltime())
		{
			float RemainingCooltime = SkillData->RemainSkillCoolTime - NonCustomDelatedDeltaSec;
			SkillData->RemainSkillCoolTime = FMath::Max(RemainingCooltime, 0.0f);
		}
	}
}

void ABladeIIMercenaryPlayer::AddStatMercenaryBySkill(EMercenarySkillOption InOption, float InValue)
{
	if (InOption == EMercenarySkillOption::EMSO_End)
		return;
	
	EUnitedCombatOptions CombatOption = CombatStatEval::GetUnitedOptionOfMercenarySkillOption(InOption);
	if (CombatOption == EUnitedCombatOptions::UCO_End)
	{
		AddSkillStat(InOption, InValue);
		return;
	}

	TArray<FOptionValue> RawValues;
	RawValues.Add(FOptionValue(InValue));
	float ResultValue = GetEffectiveOptionValueTempl<EMercenarySkillOption>(InOption, RawValues, true);

	if (!CombatStats.Contains(CombatOption))
	{
		ECombatOptionApplyType ApplyType = GetCombatOptionApplyType(InOption);
		float DefaultValue = GetInitialValueOfCombatOptionApplyType(ApplyType);
		CombatStats.Add(CombatOption, DefaultValue);
	}

	CombatStats[CombatOption] = ResultValue;

	//Player傈侩栏肺 肚促弗 规侥栏肺 利侩登绢具窃 ぱぱ;
	//primarydata甸
	if (CombatOption == EUnitedCombatOptions::UCO_Health_PerSecRecoverHP)
		SetRegenerateHPRate(CombatStats[CombatOption], false);
}

void ABladeIIMercenaryPlayer::AddSkillStat(EMercenarySkillOption InOption, float InValue)
{
	if (InOption == EMercenarySkillOption::EMSO_End)
		return;

	//眠饶 矫胶袍拳 苞楷 攫力且瘤?
	if (InOption == EMercenarySkillOption::EMSO_BuffTime ||
		InOption == EMercenarySkillOption::EMSO_IncMaxNumCharacterBuff
		)
	{
		if (!SkillStats.Contains(InOption))
			SkillStats.Emplace(InOption, InValue);
		else
			SkillStats[InOption] = InValue;
	}
}

void ABladeIIMercenaryPlayer::AddPassiveSkill(EMercenarySkillOption InOption, float InValue)
{
	//UB2GuildNPCInfo* GuildNPCInfo = StaticFindGuildNPCInfo();
	//if (GuildNPCInfo == nullptr)
	//	return;

 //	const FMercenaryMasterySkillInfo* SkillInfo = GuildNPCInfo->GetMasterySkillInfo(InOption);
	//if (SkillInfo == nullptr)
	//	return;

	//UB2PassiveSkill* CreatedPassiveSkill = PassiveSkillFactory::CreateMasteryPassiveSkill(this, *SkillInfo, InValue);
	//if (CreatedPassiveSkill == nullptr)
	//	return;

	//PassiveSkills.Add(CreatedPassiveSkill);
}

void ABladeIIMercenaryPlayer::AddMercenaryMastery(EMercenarySkillOption InOption, float InValue)
{
	AddStatMercenaryBySkill(InOption, InValue);
	AddPassiveSkill(InOption, InValue);
}

void ABladeIIMercenaryPlayer::InitializeCombatOptionCacheValues()
{
	auto GetStat = [this](EUnitedCombatOptions InOption) -> float
	{
		return CombatStats.Contains(InOption) ? CombatStats[InOption] : 0.0f;
	};

	IncDamageValueByPassive			 = GetStat(EUnitedCombatOptions::UCO_Offense_IncDamage); 
	IncNormalDamageValueByPassive	 = GetStat(EUnitedCombatOptions::UCO_Offense_IncNormalDamage); 
	IncSkillDamageValueByPassive	 = GetStat(EUnitedCombatOptions::UCO_Skill_IncSkillDamage); 
	IncCriticalRateValueByPassive	 = GetStat(EUnitedCombatOptions::UCO_Offense_IncCriticalRate); 
	IncCriticalDamageValueByPassive  = GetStat(EUnitedCombatOptions::UCO_Offense_IncCriticalDamage);
	IncDamageToBossValueByPassive	 = GetStat(EUnitedCombatOptions::UCO_Offense_IncBossDamage);
}

bool ABladeIIMercenaryPlayer::CanTakeDamage(AActor * DamageCauser, FDamageEvent const & DamageEvent, const FDamageInfo * DamageType)
{
	return ABladeIICharacter::CanTakeDamage(DamageCauser, DamageEvent, DamageType);
}

void ABladeIIMercenaryPlayer::ProcessTakeDamageAnimation(const FDamageInfo & DamageInfo, AActor * DamageCauser)
{
	ABladeIICharacter::ProcessTakeDamageAnimation(DamageInfo, DamageCauser);
}

float ABladeIIMercenaryPlayer::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	return ABladeIICharacter::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

UAnimSequenceBase * ABladeIIMercenaryPlayer::GetAnimStateOverrideSequence()
{
	//return ABladeIICharacter::GetAnimStateOverrideSequence();
	return NULL;
}

void ABladeIIMercenaryPlayer::OverrideByBaseBP(TSubclassOf<class ABladeIICharacter> BPClassToOverride)
{
	//Player铂肺 檬扁拳 矫糯..
	UB2SomeInfo* SomeInfo = StaticFindSomeInfo(this);
	BPClassToOverride = SomeInfo ? SomeInfo->GetDefaultPCClassInfoBaseBPClass() : NULL;

	Super::OverrideByBaseBP(BPClassToOverride);

	//Character漂己栏肺 FaceRotation 荤侩窍霸登骨肺
	GetCharacterMovement()->RotationRate = FRotator(0.f, 4000.f, 0.f);
	bUseControllerRotationYaw = true;
}

void ABladeIIMercenaryPlayer::NotifyTargetDamaged(ABladeIICharacter * DamagedTarget, float DamageAmount)
{
	ABladeIICharacter::NotifyTargetDamaged(DamagedTarget, DamageAmount);
}

void ABladeIIMercenaryPlayer::CharacterBuffUpdateUIdoc()
{
	ABladeIICharacter::CharacterBuffUpdateUIdoc();
}

void ABladeIIMercenaryPlayer::SetMercenarySkill(const FGuildMercenaryInfo & InMercenaryInfo)
{
	b2network::B2mdGuildMercenaryPtr MercenaryData = BladeIIGameImpl::GetGuildDataStore().GetGuildMercenaryMasterData().GetMercenaries(InMercenaryInfo.MercenaryID);
	//
	//if (!MercenaryData)
	//	return;

	//TArray<int32> MercenarySkillList;
	//
	//MercenarySkillList.Add(MercenaryData->skill_id_1);
	//MercenarySkillList.Add(MercenaryData->skill_id_2);
	//MercenarySkillList.Add(MercenaryData->skill_id_3);

	//UB2GuildNPCInfo* GuildNPCInfo = StaticFindGuildNPCInfo();
	//check(GuildNPCInfo)

	//for (int32 SkillID : MercenarySkillList)
	//{
	//	if(SkillID <= 0)
	//		continue;

	//	b2network::B2mdGuildMercenarySkillPtr ServerSkillInfo = BladeIIGameImpl::GetGuildDataStore().GetGuildMercenaryMasterData().GetMercenarySkill(SkillID);
	//	const FGuildMercenarySkill* ClientSkillInfo = GuildNPCInfo->GetMercenarySkillOfID(SkillID);
	//	
	//	if(!ServerSkillInfo || !ClientSkillInfo)
	//		continue;

	//	TSharedPtr<FMercenarySkillData> MercenarySkillData = MakeShared<FMercenarySkillData>();
	//	MercenarySkillData->bUseSkill = ClientSkillInfo->bUseSkill;
	//	MercenarySkillData->RemainSkillCoolTime = 0.0f;
	//	MercenarySkillData->SkillID = SkillID;
	//	MercenarySkillData->MaxSkillCoolTime = ServerSkillInfo->cool_time_sec;
	//	MercenarySkillData->SkillAttackIndex = ClientSkillInfo->SkillAttackIndex;
	//	MercenarySkillData->bAvailableSkill = ServerSkillInfo->require_mercenary_level <= InMercenaryInfo.MercenaryLevel;

	//	MercenarySkillDatas.Add(MercenarySkillData);
	//}
}

FName FloorColorParamName(TEXT("ColorControl"));
FName FloorColorMultiplyParamName(TEXT("ColorMultiplier"));

void ABladeIIMercenaryPlayer::ChangeFloorRingPS(UParticleSystem * NewFloorRingPS)
{
	if (!NewFloorRingPS)
		return;

	FloorRingPSC->SetTemplate(NewFloorRingPS);
	FloorRingPSC->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	FloorRingPSC->ReregisterComponent();
	FloorRingPSC->Activate();
	
	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(FloorRingPSC->GetMaterial(0));
	if (DynamicMaterial == NULL)
		DynamicMaterial = FloorRingPSC->CreateAndSetMaterialInstanceDynamic(0);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(FloorColorParamName, FloorRingColor);
		DynamicMaterial->SetScalarParameterValue(FloorColorMultiplyParamName, FloorRingColorMultiplier);

		int32 MatNum = FloorRingPSC->GetNumMaterials();
		for (int32 i = 1; i < MatNum; ++i)
			FloorRingPSC->SetMaterial(i, DynamicMaterial);
	}
}

float ABladeIIMercenaryPlayer::GetDamageIncreaseRate() const
{
	float Ret = ABladeIICharacter::GetDamageIncreaseRate() + CombatStats[EUnitedCombatOptions::UCO_Offense_IncDamage];

	if (!IsCounterAttackingState(GetAttackState()) && !IsCurrentAttackIndexSkill())
		Ret += CombatStats[EUnitedCombatOptions::UCO_Offense_IncNormalDamage];

	return Ret;
}
float ABladeIIMercenaryPlayer::GetBossDamageIncreaseRate() const
{
	return CombatStats[EUnitedCombatOptions::UCO_Offense_IncBossDamage];
}
float ABladeIIMercenaryPlayer::GetCriticalRate() const
{
	float CriticalRate = ABladeIICharacter::GetCriticalRate();
	CriticalRate += CombatStats[EUnitedCombatOptions::UCO_Offense_IncCriticalRate];

	return CriticalRate;
}
float ABladeIIMercenaryPlayer::GetCriticalDamageRate() const
{
	float CriticalDamageRate = ABladeIICharacter::GetCriticalDamageRate();
	CriticalDamageRate += CombatStats[EUnitedCombatOptions::UCO_Offense_IncCriticalDamage];
	return CriticalDamageRate;
}
float ABladeIIMercenaryPlayer::GetDamageDecreaseRate() const
{
	return ABladeIICharacter::GetDamageDecreaseRate() * CombatStats[EUnitedCombatOptions::UCO_Defense_DecDamage];
}
float ABladeIIMercenaryPlayer::GetMeleeDamageDecreaseRate() const
{
	return CombatStats[EUnitedCombatOptions::UCO_Defense_DecMeleeDamage];
}
float ABladeIIMercenaryPlayer::GetRangeDamageDecreaseRate() const
{
	return CombatStats[EUnitedCombatOptions::UCO_Defense_DecRangedDamage];
}
float ABladeIIMercenaryPlayer::GetBossDamageDecreaseRate() const
{
	return CombatStats[EUnitedCombatOptions::UCO_Defense_DecBossDamage];  
}
float ABladeIIMercenaryPlayer::GetResistCriticalRate() const
{
	return CombatStats[EUnitedCombatOptions::UCO_Defense_ResistCritical]; 
}
float ABladeIIMercenaryPlayer::GetHealHPAmountRateOnKill() const
{
	return CombatStats[EUnitedCombatOptions::UCO_Health_ProbableRecoverHP]; 
}
float ABladeIIMercenaryPlayer::GetAbsorbHPAmountRateOnAttack() const
{
	return CombatStats[EUnitedCombatOptions::UCO_Health_ProbableHPAbsorption]; 
}
float ABladeIIMercenaryPlayer::GetDurationDecreaseRate() const
{
	return CombatStats[EUnitedCombatOptions::UCO_Defense_DecAbnormalStateTime];
}

void ABladeIIMercenaryPlayer::SetFloorRingInfo(UParticleSystem * InFloorRingPS, FLinearColor InFloorRingColor, float InFloorRingColorMultiplier)
{
	if (InFloorRingPS)
	{
		FloorRingPS = InFloorRingPS;
		FloorRingColor = InFloorRingColor;
		FloorRingColorMultiplier = FMath::Max(1.f, InFloorRingColorMultiplier);

		ChangeFloorRingPS(FloorRingPS);
	}
}

void ABladeIIMercenaryPlayer::ResetSkillCooltime()
{
	for (TSharedPtr<FMercenarySkillData>& SkillData : MercenarySkillDatas)
	{
		SkillData->ResetSkillCoolTime();
	}
}

TSharedPtr<FMercenarySkillData> ABladeIIMercenaryPlayer::GetUseSkillData()
{
	for (TSharedPtr<FMercenarySkillData>& SkillData : MercenarySkillDatas)
	{
		if (SkillData->bAvailableSkill && SkillData->bUseSkill)
			return SkillData;
	}
	return nullptr;
}

void ABladeIIMercenaryPlayer::FilterTeamBuffAffectMember(ETeamBuffType TeamBuffType, TArray<class ABladeIICharacter*>& InChracters)
{
	/*
		盔贰 捞繁扒 胶懦 漂己俊 乐绢具 窃... 
		窍瘤父 AnimationNotify俊 捞亥飘父 乐绰霸 酒聪扼 肺流鳖瘤 较促 冠囚 乐绢辑 
		胶懦漂己栏肺 贸府 且 荐 绝澜....
		弊府绊 局檬俊 Character啊 购啊 魄喊肺流阑 甸绊 乐绰霸 捞惑窃...胶懦捞 秦具瘤 
		评滚橇甫 林绰单 啊鳖款娟甫 临瘤 酒聪搁 眉仿 撤篮 娟甫 临瘤 绢恫 肺流捞 绢痘霸 甸绢哎临 舅绊 某腐磐俊辑 促 贸府窍绰瘤 葛福摆澜
	*/
	if (TeamBuffType == ETeamBuffType::ETBT_Enhance)
	{
		for (ABladeIICharacter* CheckCharacter : InChracters)
		{
			if (Cast<ABladeIIMercenaryPlayer>(CheckCharacter) == nullptr)
			{
				InChracters.Remove(CheckCharacter);
			}
		}
	}
}
