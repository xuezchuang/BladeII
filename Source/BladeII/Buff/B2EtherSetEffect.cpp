// Fill out your copyright notice in the Description page of Project Settings.


#include "B2EtherSetEffect.h"
#include "BladeIIUtil.h"
#include "Event.h"
#include "BladeIIGameMode.h"
//#include "B2UIDocHelper.h"
//#include "B2SkillRTManager.h"
#include "B2AssetLoader.h"
//#include "B2GeneralBuffs.h"
//#include "BladeIINetPlayer.h"
#include "BladeIIDamageType.h"


UB2EtherSetEffect::UB2EtherSetEffect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OwnerPlayer = nullptr;
	//EtherEffectAsset = nullptr;

	TriggerRate = 0.f;

	CoolDown = 0.f;
	CurrentCool = 0.0f;
	EnableCoolDown = true;
}

UB2EtherSetEffect::~UB2EtherSetEffect()
{

}

void UB2EtherSetEffect::PostInitProperties()
{
	Super::PostInitProperties();

	const uint32 DamageInfoHash = DamageInfo.CalculateDamageHash(this);
	FDamageInfoContainer::GetContainer().RegisterDamageInfo(DamageInfoHash, DamageInfo);
}

void UB2EtherSetEffect::BeginDestroy()
{
	Super::BeginDestroy();

}

uint32 UB2EtherSetEffect::GetHashValue()
{
	return DamageInfo.GetDamageHash();
}

bool UB2EtherSetEffect::GatherAsyncLoadAssets(const FString& RequestName, TArray<FSoftObjectPath>& OutGatheredAssets)
{
	//if (EtherEffectObj == nullptr && EtherEffectAsset.IsNull() == false)
	//	OutGatheredAssets.Add(EtherEffectAsset.GetSoftObjectPath());

	//if (TextTextureObj == nullptr && TextTexture.IsNull() == false)
	//	OutGatheredAssets.Add(TextTexture.GetSoftObjectPath());

	return OutGatheredAssets.Num() > 0 ? true : false;
}

void UB2EtherSetEffect::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& LoadedAssets)
{
	//EtherEffectObj = Cast<UParticleSystem>(EtherEffectAsset.GetSoftObjectPath().ResolveObject());
	//TextTextureObj = Cast<UTexture2D>(TextTexture.GetSoftObjectPath().ResolveObject());
}

void UB2EtherSetEffect::TryAsyncLoadEtherAssets()
{
	TArray<FSoftObjectPath> AsyncRequestAssets;
	GatherAsyncLoadAssets(TEXT(""), AsyncRequestAssets);

	TryAsyncLoad(GetEtherRequestName(), AsyncRequestAssets);
}

FString UB2EtherSetEffect::GetEtherRequestName()
{
	//static FString EtherFormat = ;
	return FString::Printf(TEXT("EtherSetEffect %d"), GetEtherSetID());
	//return FString("");
}

void UB2EtherSetEffect::Initialize(class ABladeIIPlayer* Owner, float EtherTriggerRate, float EtherCoolDown)
{
	TriggerRate = EtherTriggerRate;
	OwnerPlayer = Owner;
	CoolDown = EtherCoolDown;
	EnableCoolDown = true;

	TryAsyncLoadEtherAssets();
}

void UB2EtherSetEffect::StartCoolDown()
{
	EnableCoolDown = false;
	CurrentCool = 0.0f;
}

void UB2EtherSetEffect::EndCoolDown()
{
	CurrentCool = CoolDown;
	EnableCoolDown = true;
}

void UB2EtherSetEffect::ActivateEther(ABladeIICharacter* EtherCauser)
{
	BLADE2_SCOPE_CYCLE_COUNTER(B2EtherSetEffect_ActivateEther);

	//// Apply Ether to EtherOwner
	//ApplyEtherToOwner(GetOwner());

	//// Apply Ether to Main Causer
	//ApplyEtherToCauser(EtherCauser);

	//// Apply Ether to Influenced Others
	//TArray<ABladeIICharacter*> Others;
	//if (GetInfluencedCharacters(EtherCauser, Others))
	//{
	//	for (ABladeIICharacter* Other : Others)
	//		if (Other && Other != EtherCauser)
	//			ApplyEtherToOther(Other);
	//}

	//StartCoolDown();

	//OnEtherActivatedClass<int32, ABladeIIPlayer*, ABladeIICharacter*>::GetInstance().Signal(EtherSetID, GetOwner(), EtherCauser);
}

bool UB2EtherSetEffect::IsTriggering()
{
	return (FMath::FRand() <= TriggerRate) ? true : false;
}

bool UB2EtherSetEffect::IsCoolDown()
{
	return EnableCoolDown;
}

bool UB2EtherSetEffect::IsLocalEther()
{
	BII_CHECK(GetOwner());
	return GetOwner()->IsLocalPlayer() && GetOwner()->GetLocalRole() == ROLE_Authority;
}

void UB2EtherSetEffect::SpawnEtherFx(ABladeIICharacter* ApplyCharacter)
{
	//if (EtherEffectAsset.IsNull() == false)
	//{
	//	if (EtherEffectObj == nullptr)
	//		EtherEffectObj = LoadSynchronous(EtherEffectAsset);

	//	//[@AKI, 181206] Ether Effect 안나온다고 Crash 날 필요는 없을듯 함. //https://rink.hockeyapp.net/manage/apps/591832/app_versions/383/crash_reasons/251788769
	//	if (ApplyCharacter && ApplyCharacter->IsAlive())
	//	{
	//		UGameplayStatics::SpawnEmitterAtLocation(ApplyCharacter, EtherEffectObj, ApplyCharacter->GetActorLocation());
	//	}
	//}
}

void UB2EtherSetEffect::SpawnTextFx(ABladeIICharacter* ApplyCharacter)
{
	//if (UsingTextEffect)
	//{
	//	if (ApplyCharacter && TextTexture.IsNull() == false)
	//	{
	//		if (TextTextureObj == nullptr)
	//			TextTextureObj = LoadSynchronous(TextTexture);

	//		FLoadedFloatingAbnormalEffectSetupInfo DrawTextInfo;
	//		DrawTextInfo.LoadedTextEffectTexture = TextTextureObj;
	//		DrawTextInfo.ArrowType = ArrowType;
	//		DrawTextInfo.AdditionalZOffset = AdditionalZOffset;

	//		CharacterAbnormalEffectClass<ABladeIICharacter*, const FLoadedFloatingAbnormalEffectSetupInfo&>::GetInstance().Signal(ApplyCharacter, DrawTextInfo);
	//	}
	//}
}

void UB2EtherSetEffect::UpdateCoolDown(float DeltaSeconds)
{
	if (EnableCoolDown)
		return;

	if (!EnableCoolDown && CurrentCool < CoolDown)
	{
		CurrentCool += DeltaSeconds;
	}
	else
	{
		EndCoolDown();
	}
}


// ----------------------- Attack Ether Base ----------------------- //

bool UB2EtherSetOffense::GetInfluencedCharacters(ABladeIICharacter* EtherCauser, TArray<ABladeIICharacter*>& OutInfluenced)
{
	if (IsLocalEther() && GetOwner())
	{
		GetOwner()->GetEnemiesInRadius(Radius, OutInfluenced, FanAngle);
		return OutInfluenced.Num() > 0 ? true : false;
	}
	return false;
}

void UB2EtherSetOffense::ApplyEtherToOther(ABladeIICharacter* Other)
{
	ApplyEtherToCauser(Other);
}

bool UB2EtherSetOffense::IsActivatable(ABladeIICharacter* VictimCharacter, const FDamageInfo& CauserDamageInfo, float ActualDamage)
{
	if (CauserDamageInfo.bOffenseEtherActivatable && GetOwner()->GetLocalRole()  == ROLE_Authority && IsTriggering() && IsCoolDown())
		return true;

	return false;
}

void UB2EtherSetOffense::Initialize(class ABladeIIPlayer* Owner, float EtherTriggerRate, float EtherCoolDown)
{
	Super::Initialize(Owner, EtherTriggerRate, EtherCoolDown);

	//if (IsLocalEther())
	//{
	//	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	//	{
	//		DocBattle->SetOffenseEtherMaxCoolTime(CoolDown);
	//		DocBattle->SetIsOffenseEther(true);
	//	}
	//}
}

void UB2EtherSetOffense::SetEtherCondition(float EtherCoolTime)
{
	Super::SetEtherCondition(EtherCoolTime);

	//if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	//{
	//	bool bDuringCoolTime = EtherCoolTime > 0.0f && EtherCoolTime < CoolDown;
	//	EnableCoolDown = !bDuringCoolTime;

	//	DocBattle->SetOffenseEtherCoolTime(EtherCoolTime);
	//	DocBattle->SetEnableOffenseEther(EnableCoolDown);
	//}
}

void UB2EtherSetOffense::ProcessEffect(ABladeIICharacter* VictimCharacter)
{
	// 공격 Ether - 이펙트의 Default Spawn 대상은 피해자 ( == VictimCharacter )
	SpawnEtherFx(VictimCharacter);

	SpawnTextFx(VictimCharacter);
}

void UB2EtherSetOffense::StartCoolDown()
{
	Super::StartCoolDown();

	//if (IsLocalEther())
	//{
	//	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	//	{
	//		DocBattle->SetEnableOffenseEther(false);
	//	}
	//}
}

void UB2EtherSetOffense::EndCoolDown()
{
	Super::EndCoolDown();

	if (IsLocalEther())
	{
		//if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		//{
		//	DocBattle->SetEnableOffenseEther(true);

		//	if (auto* DocHero = UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass()))
		//	{
		//		//DocHero->SetCurOffenseEtherCoolTime(CurrentCool);
		//	}
		//}
	}
}


// ----------------------- Defense Ether Base ----------------------- //
bool UB2EtherSetDefense::IsActivatable(ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage)
{
	return CauserDamageInfo.bDefenseEtherActivatable && IsLocalEther() && IsTriggering() && IsCoolDown();
}

void UB2EtherSetDefense::Initialize(class ABladeIIPlayer* Owner, float EtherTriggerRate, float EtherCoolDown)
{
	Super::Initialize(Owner, EtherTriggerRate, EtherCoolDown);

	//if (IsLocalEther())
	//{
	//	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	//	{
	//		DocBattle->SetDefenseEtherMaxCoolTime(CoolDown);
	//		DocBattle->SetIsDefenseEther(true);
	//	}
	//}
}

void UB2EtherSetDefense::SetEtherCondition(float EtherCoolTime)
{
	Super::SetEtherCondition(EtherCoolTime);

	//if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	//{
	//	bool bDuringCoolTime = EtherCoolTime > 0.0f && EtherCoolTime < CoolDown;
	//	EnableCoolDown = !bDuringCoolTime;

	//	DocBattle->SetDefenseEtherCoolTime(EtherCoolTime);
	//	DocBattle->SetEnableDefenseEther(EnableCoolDown);
	//}
}

void UB2EtherSetDefense::ProcessEffect(ABladeIICharacter* EtherCauser)
{
	// 방어 Ether - 이펙트의 Default Spawn 대상은 EtherOwner
	SpawnEtherFx(GetOwner());

	SpawnTextFx(GetOwner());
}

void UB2EtherSetDefense::StartCoolDown()
{
	Super::StartCoolDown();

	//if (IsLocalEther())
	//{
	//	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	//	{
	//		DocBattle->SetEnableDefenseEther(false);
	//	}
	//}
}

void UB2EtherSetDefense::EndCoolDown()
{
	Super::EndCoolDown();

	if (IsLocalEther())
	{
		//if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		//{
		//	DocBattle->SetEnableDefenseEther(true);

		//	if (auto* DocHero = UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass()))
		//	{
		//		//DocHero->SetCurDefenseEtherCoolTime(CurrentCool);
		//	}
		//}
	}
}


/* ----- 폭뢰(공격) - 번개공격 [B2AetherSetOptionType::THUNDER_DAMAGE = 1001 ] ----- */
UB2Ether_Thunder::UB2Ether_Thunder() : Super(b2network::B2AetherSetOptionType::THUNDER_DAMAGE)
{
	ThunderDamageRate = 0.08f;
}

void UB2Ether_Thunder::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	if (EtherCauser && EtherCauser->IsAlive() && EtherCauser != GetOwner())
	{
		bool bValidDamageApplied = false;
		if (EtherCauser->GetLocalRole()  == ROLE_Authority)
		{
			const float AbsDamage = GetAbsThunderDamage(EtherCauser);
			bValidDamageApplied = EtherCauser->RequestDamage(AbsDamage, GetDamageInfo(), GetOwner());
		}

		if (bValidDamageApplied || EtherCauser->GetLocalRole()  == ROLE_SimulatedProxy)
		{
			if (HitEffectObj == nullptr)
				HitEffectObj = LoadSynchronous(HitEffect);

			UGameplayStatics::SpawnEmitterAtLocation(EtherCauser, HitEffectObj, EtherCauser->GetActorLocation());
			ProcessEffect(EtherCauser);
		}
	}
}

bool UB2Ether_Thunder::GatherAsyncLoadAssets(const FString& RequestName, TArray<FSoftObjectPath>& OutGatheredAssets)
{
	//if (HitEffectObj == nullptr && HitEffect.IsNull() == false)
	//	OutGatheredAssets.Add(HitEffect.GetSoftObjectPath());

	return Super::GatherAsyncLoadAssets(RequestName, OutGatheredAssets);
}

void UB2Ether_Thunder::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& LoadedAssets)
{
	Super::OnAsyncLoadComplete(CompleteRequest, LoadedAssets);

	//HitEffectObj = Cast<UParticleSystem>(HitEffect.GetSoftObjectPath().ResolveObject());
}

float UB2Ether_Thunder::GetAbsThunderDamage(ABladeIICharacter* EtherCauser)
{
	float ThunderDamage;
	// 최대체력에 비례한 데미지
	const float VictimHealthMax = (EtherCauser != nullptr) ? EtherCauser->GetMaxHealth() : 0.f;

	//보스몹일때 따로 계산
	if (EtherCauser->IsBossMob())
	{
		ThunderDamage = ThunderDamageRateToBoss * VictimHealthMax;
	}
	else
	{
		ThunderDamage = ThunderDamageRate * VictimHealthMax;
	}

	return ThunderDamage;
}

void UB2Ether_Thunder::GetThunderDamageRate(OUT float& OutThunderDamageRate, OUT float& OutThunderDamageRateToBoss)
{
	OutThunderDamageRate = ThunderDamageRate;
	OutThunderDamageRateToBoss = ThunderDamageRateToBoss;
}

/* ----- 속박(공격) - 이동불가 [B2AetherSetOptionType::IMMOBILIZE = 1002 ] ----- */
UB2Ether_Immobilize::UB2Ether_Immobilize() : Super(b2network::B2AetherSetOptionType::IMMOBILIZE) {}

void UB2Ether_Immobilize::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	//if (EtherCauser && EtherCauser->IsAlive() && EtherCauser->Role == ROLE_Authority)
	//	EtherCauser->AddBuff(EBuffType::Debuff_Immobilize, Duration, 0.f, GetOwner());
}


float UB2Ether_Immobilize::GetDuration()
{
	return Duration;
}

/* ----- 수호(방어) - 쉴드 [B2AetherSetOptionType::SHIELD = 1003 ] ----- */
UB2Ether_Shield::UB2Ether_Shield() : Super(b2network::B2AetherSetOptionType::SHIELD)
{
	ShieldRate = 0.2f;
	Duration = 10.f;
}

void UB2Ether_Shield::ApplyEtherToOwner(ABladeIIPlayer* EtherOwner)
{
	//if (EtherOwner && EtherOwner->IsAlive() && EtherOwner->Role == ROLE_Authority)
	//{
	//	const float ShieldAmount = EtherOwner->GetMaxHealth() * ShieldRate;
	//	EtherOwner->AddBuff(EBuffType::Buff_Shield, Duration, ShieldAmount, EtherOwner);
	//}
}


float UB2Ether_Shield::GetDuration()
{
	return Duration;
}

float UB2Ether_Shield::GetShieldRate()
{
	return ShieldRate;
}

/* ----- 열정(방어) - 주변 화염공격 [B2AetherSetOptionType::FIRE_RANGE = 1004 ] ----- */
UB2Ether_FireRange::UB2Ether_FireRange() : Super(b2network::B2AetherSetOptionType::FIRE_RANGE)
{
	Radius = 500.f;
}

void UB2Ether_FireRange::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	//if (EtherCauser && EtherCauser->Role == ROLE_Authority)
	//{
	//	if (EtherCauser->IsAlive())
	//	{
	//		UB2Buff_Base* BuffObject = EtherCauser->AddBuff(EBuffType::Debuff_Burning, DamageInfo.StateDamageDuration, DamageInfo.StateDamageAmount, GetOwner());
	//		if (UB2Buff_DotDamage* DotBurning = Cast<UB2Buff_DotDamage>(BuffObject))
	//		{
	//			DotBurning->OverrideDamageInfo(*GetDamageInfo());
	//			DotBurning->DotDamageLogicType = GetDamageInfo()->StateDamageLogicType;
	//		}
	//	}
	//}

	//ProcessEffect(GetOwner());
}

void UB2Ether_FireRange::ApplyEtherToOther(ABladeIICharacter* Other)
{
	ApplyEtherToCauser(Other);	// Causer와 동일한 효과를 적용
}

bool UB2Ether_FireRange::GetInfluencedCharacters(ABladeIICharacter* EtherCauser, TArray<ABladeIICharacter*>& OutInfluenced)
{
	if (IsLocalEther() && GetOwner())
	{
		GetOwner()->GetEnemiesInRadius(Radius, OutInfluenced);
		return OutInfluenced.Num() > 0 ? true : false;
	}

	return false;
}


float UB2Ether_FireRange::GetDuration()
{
	return GetDamageInfo()->StateDamageDuration;
}

/* ----- 파멸(공격) - 번개공격 [B2AetherSetOptionType::METEOR_DAMAGE = 1005 ] ----- */
UB2Ether_Meteor::UB2Ether_Meteor() : Super(b2network::B2AetherSetOptionType::METEOR_DAMAGE) {}

void UB2Ether_Meteor::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	//if (EtherCauser && EtherCauser->IsAlive() && EtherCauser->Role == ROLE_Authority)
	//	EtherCauser->RequestDamage(DamageAmount, GetDamageInfo(), GetOwner());

	//ProcessEffect(EtherCauser);
}

void UB2Ether_Meteor::ApplyEtherToOther(ABladeIICharacter* Other)
{
	ApplyEtherToCauser(Other);
}

bool UB2Ether_Meteor::GetInfluencedCharacters(ABladeIICharacter* EtherCauser, TArray<ABladeIICharacter*>& OutInfluenced)
{
	//ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetOwner()));
	//if (EtherCauser && GameMode)
	//{
	//	TArray<ABladeIICharacter*> Enemies;
	//	GameMode->GetAllWorldEnemyList(Enemies, GetOwner()->GetTeamNum());

	//	const float MeteorRadiusSq = MeteorRadius * MeteorRadius;
	//	for (ABladeIICharacter* Enemy : Enemies)
	//	{
	//		if (Enemy && Enemy->IsPendingKill() == false && Enemy->IsEnemy(GetOwner()))
	//		{
	//			// 원통형으로 Radius를 조사
	//			const float DistSq = FVector::DistSquaredXY(Enemy->GetActorLocation(), EtherCauser->GetActorLocation());
	//			if (DistSq <= MeteorRadiusSq)
	//				OutInfluenced.Add(Enemy);
	//		}
	//	}

	//	return OutInfluenced.Num() > 0 ? true : false;
	//}

	return false;
}



float UB2Ether_Meteor::GetDamageAmount()
{
	return DamageAmount;
}

/* ----- 통찰(공격) - 대상 스킬 재사용시간 max [B2AetherSetOptionType::SKILL_COOLTIME = 1006 ] ----- */
UB2Ether_IncreaseCooltime::UB2Ether_IncreaseCooltime() : Super(b2network::B2AetherSetOptionType::SKILL_COOLTIME) {}


void UB2Ether_IncreaseCooltime::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	//if (EtherCauser && EtherCauser->IsPlayerControlled() && EtherCauser->IsAlive() && EtherCauser->Role == ROLE_Authority)
	//{
	//	ABladeIIPlayer* VictimPlyaer = Cast<ABladeIIPlayer>(EtherCauser);
	//	if (UB2SkillRTManager* SkillManager = VictimPlyaer->GetSkillRTManager())
	//	{
	//		SkillManager->SetSkillCoolTimeTimer(EAttackState::ECS_Skill01);
	//		SkillManager->SetSkillCoolTimeTimer(EAttackState::ECS_Skill02);
	//		SkillManager->SetSkillCoolTimeTimer(EAttackState::ECS_Skill03);
	//		SkillManager->SetSkillCoolTimeTimer(EAttackState::ECS_Skill04);
	//		SkillManager->SetSkillCoolTimeTimer(EAttackState::ECS_Skill05);
	//		SkillManager->SetSkillCoolTimeTimer(EAttackState::ECS_Skill06);
	//	}
	//}


	if (IsLocalEther())
		ProcessEffect(EtherCauser);
}

void UB2Ether_IncreaseCooltime::ProcessEffect(ABladeIICharacter* VictimCharacter)
{
	Super::ProcessEffect(VictimCharacter);

	// Insight!! 단어 띄워줌 (Victim에)
}



/* ----- 분노(공격) - 결속스킬 쿨타임 감소 [B2AetherSetOptionType::SKILL_GAGE = 1007 ] ----- */
UB2Ether_DecreaseCooltime::UB2Ether_DecreaseCooltime() : Super(b2network::B2AetherSetOptionType::SKILL_GAGE)
{
	DecreaseCooltime = 1.f;	// 1초씩
}

void UB2Ether_DecreaseCooltime::ApplyEtherToOwner(ABladeIIPlayer* EtherOwner)
{
	//if (EtherOwner && EtherOwner->IsAlive() && EtherOwner->Role == ROLE_Authority)
	//{
	//	// Skill Cooltime
	//	//check mode..
	//	ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(EtherOwner));
	//	if (GameMode)
	//	{
	//		if (!IsAvailGameMode(GameMode->GetB2GameModeType()))
	//			return;

	//		UB2SkillRTManager* SkillRT = EtherOwner->GetSkillRTManager();
	//		if (SkillRT)
	//			SkillRT->SetZeroWeaponSkillCoolTime();

	//		const float CurrWeaponSkillPoint = EtherOwner->GetWeaponSkillPoint();
	//		EtherOwner->SetWeaponSkillPoint(CurrWeaponSkillPoint + AddSkillPoint);
	//	}
	//}
}

bool UB2Ether_DecreaseCooltime::IsActivatable(ABladeIICharacter * EtherCauser, const FDamageInfo & CauserDamageInfo, float ActualDamage)
{
	if (Super::IsActivatable(EtherCauser, CauserDamageInfo, ActualDamage))
	{
		ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetOwner()));
		if (GameMode && IsAvailGameMode(GameMode->GetB2GameModeType()))
			return true;
	}
	return false;
}

bool UB2Ether_DecreaseCooltime::IsAvailGameMode(EB2GameMode CurrentGameMode)
{
	if (CurrentGameMode == EB2GameMode::HeroTower ||
		CurrentGameMode == EB2GameMode::PVP_Tag ||
		CurrentGameMode == EB2GameMode::Raid ||
		CurrentGameMode == EB2GameMode::Scenario)
		return true;

	return false;
}

/* ----- 절망(공격) -  스턴 [B2AetherSetOptionType::STUN = 1008 ] ----- */
UB2Ether_Stun::UB2Ether_Stun() : Super(b2network::B2AetherSetOptionType::STUN) {}

void UB2Ether_Stun::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	// DamageInfo 자체를 Stun으로 셋팅
	if (EtherCauser && EtherCauser->IsAlive() && EtherCauser->GetLocalRole()  == ROLE_Authority)
	{
		EtherCauser->RequestDamage(1.f, GetDamageInfo(), GetOwner());
	}
	// Effect가 있다면...
}



float UB2Ether_Stun::GetDuration()
{
	return GetDamageInfo()->StateDamageDuration;
}

/* ----- 저항(방어) - 무적 [B2AetherSetOptionType::SUPER_ARMOR = 1009 ] ----- */
UB2Ether_invincible::UB2Ether_invincible() : Super(b2network::B2AetherSetOptionType::SUPER_ARMOR) {}

bool UB2Ether_invincible::IsActivatable(ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage)
{
	if (Super::IsActivatable(EtherCauser, CauserDamageInfo, ActualDamage) && GetOwner() && GetOwner()->IsAlive())
	{
		const float MaxHealth = GetOwner()->GetMaxHealth();

		// 최대체력의 ActivateDamageRate(비율) 이상의 데미지를 입으면
		if (MaxHealth > 0.f && (ActualDamage / MaxHealth) >= ActivateDamageRate)
			return true;
	}

	return false;
}

void UB2Ether_invincible::ApplyEtherToOwner(ABladeIIPlayer* EtherOwner)
{
	//if (EtherOwner && EtherOwner->IsAlive() && EtherOwner->Role == ROLE_Authority)
	//{
	//	EtherOwner->AddBuff(EBuffType::Buff_Unbeatable, Duration);
	//}
}

float UB2Ether_invincible::GetDuration()
{
	return Duration;
}

float UB2Ether_invincible::GetActivateDamageRate()
{
	return ActivateDamageRate;
}

/* ----- 복수(방어) - 데미지 반사 [B2AetherSetOptionType::DAMAGE_REFLECT = 1010 ] ----- */
UB2Ether_Reflect::UB2Ether_Reflect() : Super(b2network::B2AetherSetOptionType::DAMAGE_REFLECT)
{
	ReflectAmount = 400.f;
}


void UB2Ether_Reflect::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	// EtherCauser는 EtherOwner를 공격한( Owner의 DefenseEther를 발동시킨) 주체
	//if (EtherCauser && EtherCauser->Role == ROLE_Authority && EtherCauser->IsAlive())
	//{
	//	const float DamageAmount = GetReflectDamageAmount();
	//	if (DamageAmount > 0.f)
	//		EtherCauser->RequestDamage(DamageAmount, GetDamageInfo(), GetOwner());
	//}

	ProcessEffect(GetOwner());
}

float UB2Ether_Reflect::GetReflectDamageAmount()
{
	return ReflectAmount;
}

/* ----- 망각(공격) - N초간 방어/반격/구르기/태그 불능 상태 [B2AetherSetOptionType::PROHIBIT_BEHAVIOR = 1011] ----- */
UB2Ether_NotTagDefence::UB2Ether_NotTagDefence() : Super(b2network::B2AetherSetOptionType::PROHIBIT_BEHAVIOR) {}

void UB2Ether_NotTagDefence::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	//통찰과 같은 조건 (플레이어한테만 적용 되게)
	//if (EtherCauser && EtherCauser->IsPlayerControlled() && EtherCauser->IsAlive() && EtherCauser->Role == ROLE_Authority)
	//{
	//	ABladeIIPlayer* VictimPlyaer = Cast<ABladeIIPlayer>(EtherCauser);
	//	if (UB2SkillRTManager* SkillManager = VictimPlyaer->GetSkillRTManager())
	//	{
	//		SkillManager->SetSkillCoolTimeTimer(EAttackState::ECS_TagOtherChar);
	//	}

	//	VictimPlyaer->AddGuardCoolTime(Duration);
	//}

	//if (IsLocalEther())
	//{
	//	SpawnTextFx(EtherCauser);
	//}
}


float UB2Ether_NotTagDefence::GetDuration()
{
	return Duration;
}

/* ----- 혹한(방어) - 빙결 디버프 [B2AetherSetOptionType::FREEZE_RANGE = 1012] ----- */
UB2Ether_FreezeRange::UB2Ether_FreezeRange() :Super(b2network::B2AetherSetOptionType::FREEZE_RANGE)
{
	Radius = 500.0f;
}

void UB2Ether_FreezeRange::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	//if (EtherCauser && EtherCauser->Role == ROLE_Authority)
	//{
	//	if (EtherCauser->CanTakeEtherEffect(GetOwner()) == true && EtherCauser->GetAttackState() != EAttackState::ECS_WeaponSkill)
	//	{
	//		EtherCauser->AddBuff(EBuffType::Debuff_Freezing, Duration, 0, GetOwner());
	//	}
	//}
}

void UB2Ether_FreezeRange::ApplyEtherToOther(ABladeIICharacter* Other)
{
	//ABladeIIGameMode* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetOwner()));
	//if (GameMode && GameMode->GetB2GameModeType() == EB2GameMode::Control)
	//{
	//	if (Other->IsPlayerCharacter())
	//	{
	//		if (IsLocalEther() && GetOwner()->IsAlive())
	//		{
	//			if (ABladeIINetPlayer* NetPlayer = Cast<ABladeIINetPlayer>(GetOwner()))
	//			{
	//				NetPlayer->RequestEtherSetEffect(Other->GetNetId(), GetEtherSetID());
	//			}
	//		}
	//	}
	//	else
	//	{
	//		ApplyEtherToCauser(Other);
	//	}
	//}
	//else
	//{
	//	ApplyEtherToCauser(Other);	// Causer와 동일한 효과를 적용
	//}
}

bool UB2Ether_FreezeRange::GetInfluencedCharacters(ABladeIICharacter* EtherCauser, TArray<ABladeIICharacter*>& OutInfluenced)
{
	if (IsLocalEther() && GetOwner())
	{
		GetOwner()->GetEnemiesInRadius(Radius, OutInfluenced);
		return OutInfluenced.Num() > 0 ? true : false;
	}

	return false;
}

float UB2Ether_FreezeRange::GetDuration()
{
	return Duration;
}
