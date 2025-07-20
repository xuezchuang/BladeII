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
	if (EtherEffectObj == nullptr && EtherEffectAsset.IsNull() == false)
		OutGatheredAssets.Add(EtherEffectAsset.ToSoftObjectPath());

	if (TextTextureObj == nullptr && TextTexture.IsNull() == false)
		OutGatheredAssets.Add(TextTexture.ToSoftObjectPath());

	return OutGatheredAssets.Num() > 0 ? true : false;
}

void UB2EtherSetEffect::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& LoadedAssets)
{
	EtherEffectObj = Cast<UParticleSystem>(EtherEffectAsset.ToSoftObjectPath().ResolveObject());
	TextTextureObj = Cast<UTexture2D>(TextTexture.ToSoftObjectPath().ResolveObject());
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

	//	//[@AKI, 181206] Ether Effect 救唱柯促绊 Crash 朝 鞘夸绰 绝阑淀 窃. //https://rink.hockeyapp.net/manage/apps/591832/app_versions/383/crash_reasons/251788769
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
	// 傍拜 Ether - 捞棋飘狼 Default Spawn 措惑篮 乔秦磊 ( == VictimCharacter )
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
	// 规绢 Ether - 捞棋飘狼 Default Spawn 措惑篮 EtherOwner
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


/* ----- 气汾(傍拜) - 锅俺傍拜 [B2AetherSetOptionType::THUNDER_DAMAGE = 1001 ] ----- */
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
	if (HitEffectObj == nullptr && HitEffect.IsNull() == false)
		OutGatheredAssets.Add(HitEffect.ToSoftObjectPath());

	return Super::GatherAsyncLoadAssets(RequestName, OutGatheredAssets);
}

void UB2Ether_Thunder::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& LoadedAssets)
{
	Super::OnAsyncLoadComplete(CompleteRequest, LoadedAssets);

	HitEffectObj = Cast<UParticleSystem>(HitEffect.ToSoftObjectPath().ResolveObject());
}

float UB2Ether_Thunder::GetAbsThunderDamage(ABladeIICharacter* EtherCauser)
{
	float ThunderDamage;
	// 弥措眉仿俊 厚肥茄 单固瘤
	const float VictimHealthMax = (EtherCauser != nullptr) ? EtherCauser->GetMaxHealth() : 0.f;

	//焊胶各老锭 蝶肺 拌魂
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

/* ----- 加冠(傍拜) - 捞悼阂啊 [B2AetherSetOptionType::IMMOBILIZE = 1002 ] ----- */
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

/* ----- 荐龋(规绢) - 蒋靛 [B2AetherSetOptionType::SHIELD = 1003 ] ----- */
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

/* ----- 凯沥(规绢) - 林函 拳堪傍拜 [B2AetherSetOptionType::FIRE_RANGE = 1004 ] ----- */
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
	ApplyEtherToCauser(Other);	// Causer客 悼老茄 瓤苞甫 利侩
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

/* ----- 颇戈(傍拜) - 锅俺傍拜 [B2AetherSetOptionType::METEOR_DAMAGE = 1005 ] ----- */
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
	//			// 盔烹屈栏肺 Radius甫 炼荤
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

/* ----- 烹蔓(傍拜) - 措惑 胶懦 犁荤侩矫埃 max [B2AetherSetOptionType::SKILL_COOLTIME = 1006 ] ----- */
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

	// Insight!! 窜绢 剁况淋 (Victim俊)
}



/* ----- 盒畴(傍拜) - 搬加胶懦 酿鸥烙 皑家 [B2AetherSetOptionType::SKILL_GAGE = 1007 ] ----- */
UB2Ether_DecreaseCooltime::UB2Ether_DecreaseCooltime() : Super(b2network::B2AetherSetOptionType::SKILL_GAGE)
{
	DecreaseCooltime = 1.f;	// 1檬究
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

/* ----- 例噶(傍拜) -  胶畔 [B2AetherSetOptionType::STUN = 1008 ] ----- */
UB2Ether_Stun::UB2Ether_Stun() : Super(b2network::B2AetherSetOptionType::STUN) {}

void UB2Ether_Stun::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	// DamageInfo 磊眉甫 Stun栏肺 悸泼
	if (EtherCauser && EtherCauser->IsAlive() && EtherCauser->GetLocalRole()  == ROLE_Authority)
	{
		EtherCauser->RequestDamage(1.f, GetDamageInfo(), GetOwner());
	}
	// Effect啊 乐促搁...
}



float UB2Ether_Stun::GetDuration()
{
	return GetDamageInfo()->StateDamageDuration;
}

/* ----- 历亲(规绢) - 公利 [B2AetherSetOptionType::SUPER_ARMOR = 1009 ] ----- */
UB2Ether_invincible::UB2Ether_invincible() : Super(b2network::B2AetherSetOptionType::SUPER_ARMOR) {}

bool UB2Ether_invincible::IsActivatable(ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage)
{
	if (Super::IsActivatable(EtherCauser, CauserDamageInfo, ActualDamage) && GetOwner() && GetOwner()->IsAlive())
	{
		const float MaxHealth = GetOwner()->GetMaxHealth();

		// 弥措眉仿狼 ActivateDamageRate(厚啦) 捞惑狼 单固瘤甫 涝栏搁
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

/* ----- 汗荐(规绢) - 单固瘤 馆荤 [B2AetherSetOptionType::DAMAGE_REFLECT = 1010 ] ----- */
UB2Ether_Reflect::UB2Ether_Reflect() : Super(b2network::B2AetherSetOptionType::DAMAGE_REFLECT)
{
	ReflectAmount = 400.f;
}


void UB2Ether_Reflect::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	// EtherCauser绰 EtherOwner甫 傍拜茄( Owner狼 DefenseEther甫 惯悼矫挪) 林眉
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

/* ----- 噶阿(傍拜) - N檬埃 规绢/馆拜/备福扁/怕弊 阂瓷 惑怕 [B2AetherSetOptionType::PROHIBIT_BEHAVIOR = 1011] ----- */
UB2Ether_NotTagDefence::UB2Ether_NotTagDefence() : Super(b2network::B2AetherSetOptionType::PROHIBIT_BEHAVIOR) {}

void UB2Ether_NotTagDefence::ApplyEtherToCauser(ABladeIICharacter* EtherCauser)
{
	//烹蔓苞 鞍篮 炼扒 (敲饭捞绢茄抛父 利侩 登霸)
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

/* ----- 趣茄(规绢) - 葫搬 叼滚橇 [B2AetherSetOptionType::FREEZE_RANGE = 1012] ----- */
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
	//	ApplyEtherToCauser(Other);	// Causer客 悼老茄 瓤苞甫 利侩
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
