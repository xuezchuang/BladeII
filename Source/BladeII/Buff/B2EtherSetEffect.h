// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "BladeII.h"
#include "B2FloatingAbnormalEffect.h"
#include "CommonStruct.h"
#include "B2AssetLoader.h"
#include "../BladeII/BladeIIDamageType.h"
#include "B2EtherSetEffect.generated.h"

/**
 *
 */

UCLASS()
class UB2EtherSetEffect : public UObject, public IB2AsyncInterface//, public FTickableGameObject
{
public:
	GENERATED_BODY()

	UB2EtherSetEffect()
		: TriggerRate(0.f), EtherEffectAsset(nullptr), Radius(0.0f), FanAngle(180.0f), EtherSetType(EEtherSetType::None), EtherSetID(INDEX_NONE)
	{
	}
	UB2EtherSetEffect(EEtherSetType SetType, int32 SetID)
		: TriggerRate(0.f)
		, EtherEffectAsset(nullptr)
		, Radius(0.0f)
		, FanAngle(180.0f)
		, EtherSetType(SetType)
		, EtherSetID(SetID)
	{
	}
	virtual ~UB2EtherSetEffect();

	UB2EtherSetEffect(const FObjectInitializer& ObjectInitializer);


	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;
	virtual uint32 GetHashValue();

	// Ether Logic Interface

	virtual void Initialize(class ABladeIIPlayer* Owner, float EtherTriggerRate, float EtherCoolDown);
	virtual void SetEtherCondition(float EtherCoolTime) { CurrentCool = EtherCoolTime; }
	virtual void ActivateEther(ABladeIICharacter* EtherCauser);

	virtual bool IsActivatable(ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage) { return false; }

	virtual void ApplyEtherToOwner(class ABladeIIPlayer* Owner) {}
	virtual void ApplyEtherToCauser(ABladeIICharacter* EtherCauser) {}
	virtual void ApplyEtherToOther(ABladeIICharacter* Other) {}	// Influenced by Cuaser or Owner
	virtual void StartCoolDown();
	virtual void EndCoolDown();

	bool IsCoolDown();
	bool IsTriggering();
	bool IsLocalEther();

	void UpdateCoolDown(float DeltaSeconds);
	void SpawnEtherFx(ABladeIICharacter* ApplyCharacter);
	void SpawnTextFx(ABladeIICharacter* ApplyCharacter);
	virtual void ProcessEffect(ABladeIICharacter* ApplyCharacter) {}

	// Get Interface

	class ABladeIIPlayer* GetOwner()		const { return OwnerPlayer; }

	const EEtherSetType GetEtherSetType()	const { return EtherSetType; }
	const int32			GetEtherSetID()		const { return EtherSetID; }

	const FDamageInfo* GetDamageInfo()		const { return &DamageInfo; }

	const float			GetCurrentCoolTime() const { return CurrentCool; }
	const float			GetMaxCoolTime() const { return CoolDown; }
	bool				GetEnableCoolTime() const { return EnableCoolDown; }

	virtual bool GetInfluencedCharacters(ABladeIICharacter* EtherCauser, TArray<ABladeIICharacter*>& OutInfluenced) { return false; }

protected:

	// AsyncLoading Interface

	virtual bool GatherAsyncLoadAssets(const FString& RequestName, TArray<FSoftObjectPath>& OutGatheredAssets);
	virtual void OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& LoadedAssets);

	// AsyncLoading Interface

	virtual void TryAsyncLoadEtherAssets();
	FString GetEtherRequestName();



	class ABladeIIPlayer* OwnerPlayer;
	float					TriggerRate;
	bool					EnableCoolDown;
	float					CoolDown;
	float					CurrentCool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	FDamageInfo				DamageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	TSoftObjectPtr<UParticleSystem>	EtherEffectAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	bool UsingTextEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	TSoftObjectPtr<class UTexture2D> TextTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	EFloatingAbnormalEffectArrowType ArrowType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float AdditionalZOffset;

	UPROPERTY(Transient)
	UParticleSystem* EtherEffectObj;

	UPROPERTY(Transient)
	UTexture2D* TextTextureObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float Radius;
	// 0 ~ 180
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether", meta = (ClampMin = "0.0", ClampMax = "180.0", UIMin = "0.0", UIMax = "180.0"))
	float FanAngle;

private:	// Do not change

	UPROPERTY(VisibleDefaultsOnly, Category = "Ether")
	EEtherSetType	EtherSetType;

	UPROPERTY(VisibleDefaultsOnly, Category = "Ether")
	int32			EtherSetID;
};

UCLASS()
class UB2EtherSetOffense : public UB2EtherSetEffect
{
public:
	GENERATED_BODY()

	UB2EtherSetOffense() : Super(EEtherSetType::None, INDEX_NONE) {}
	UB2EtherSetOffense(const int32 SetID) : Super(EEtherSetType::Offense, SetID) {}
	virtual bool IsActivatable(ABladeIICharacter* VictimCharacter, const FDamageInfo& CauserDamageInfo, float ActualDamage);
	virtual void Initialize(class ABladeIIPlayer* Owner, float EtherTriggerRate, float EtherCoolDown) override;
	virtual void SetEtherCondition(float EtherCoolTime) override;

	virtual void ProcessEffect(ABladeIICharacter* VictimCharacter);
	virtual void StartCoolDown() override;
	virtual void EndCoolDown() override;

	virtual void ApplyEtherToOther(ABladeIICharacter* Other) override;
	virtual bool GetInfluencedCharacters(ABladeIICharacter* EtherCauser, TArray<ABladeIICharacter*>& OutInfluenced) override;
};

UCLASS()
class UB2EtherSetDefense : public UB2EtherSetEffect
{
public:
	GENERATED_BODY()

	UB2EtherSetDefense() : Super(EEtherSetType::None, INDEX_NONE) {}
	UB2EtherSetDefense(const int32 SetID) : Super(EEtherSetType::Defense, SetID) {}
	virtual bool IsActivatable(ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage);
	virtual void Initialize(class ABladeIIPlayer* Owner, float EtherTriggerRate, float EtherCoolDown) override;
	virtual void SetEtherCondition(float EtherCoolTime) override;

	virtual void ProcessEffect(ABladeIICharacter* EtherCauser);
	virtual void StartCoolDown() override;
	virtual void EndCoolDown() override;
};



/*
	Offense Ethers...
*/
/* ----- 폭뢰(공격) - 번개공격 [B2AetherSetOptionType::THUNDER_DAMAGE = 1001 ] ----- */
UCLASS(Blueprintable)
class UB2Ether_Thunder : public UB2EtherSetOffense
{
public:
	GENERATED_BODY()
	UB2Ether_Thunder();

	virtual void ApplyEtherToCauser(ABladeIICharacter* EtherCauser);

	virtual bool GatherAsyncLoadAssets(const FString& RequestName, TArray<FSoftObjectPath>& OutGatheredAssets) override;
	virtual void OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& LoadedAssets) override;

	float GetAbsThunderDamage(ABladeIICharacter* EtherCauser);

	void GetThunderDamageRate(OUT float& OutThunderDamageRate, OUT float& OutThunderDamageRateToBoss);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float ThunderDamageRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float ThunderDamageRateToBoss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	TSoftObjectPtr<UParticleSystem>	HitEffect;

	UPROPERTY(Transient)
	UParticleSystem* HitEffectObj;
};

/* ----- 속박(공격) - 이동불가 [B2AetherSetOptionType::IMMOBILIZE = 1002 ] ----- */
UCLASS(Blueprintable)
class UB2Ether_Immobilize : public UB2EtherSetOffense
{
public:
	GENERATED_BODY()
	UB2Ether_Immobilize();

	virtual void ApplyEtherToCauser(ABladeIICharacter* EtherCauser);

	float GetDuration();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float Duration;
};

/* ----- 수호(방어) - 쉴드 [B2AetherSetOptionType::SHIELD = 1003 ] ----- */
UCLASS(Blueprintable)
class UB2Ether_Shield : public UB2EtherSetDefense
{
public:
	GENERATED_BODY()
	UB2Ether_Shield();

	virtual void ApplyEtherToOwner(class ABladeIIPlayer* EtherOwner);

	float GetDuration();
	float GetShieldRate();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float ShieldRate;
};


/* ----- 열정(방어) - 화염 디버프 [B2AetherSetOptionType::FIRE_RANGE = 1004 ] ----- */
UCLASS(Blueprintable)
class UB2Ether_FireRange : public UB2EtherSetDefense
{
public:
	GENERATED_BODY()
	UB2Ether_FireRange();

	virtual void ApplyEtherToCauser(ABladeIICharacter* EtherCauser);
	virtual void ApplyEtherToOther(ABladeIICharacter* Other);	// Influenced by Cuaser or Owner

	virtual bool GetInfluencedCharacters(ABladeIICharacter* EtherCauser, TArray<ABladeIICharacter*>& OutInfluenced);

	float GetDuration();
};



/* ----- 파멸(공격) - 운석공격 [B2AetherSetOptionType::METEOR_DAMAGE = 1005 ] ----- */
UCLASS(Blueprintable)
class UB2Ether_Meteor : public UB2EtherSetOffense
{
public:
	GENERATED_BODY()
	UB2Ether_Meteor();

	virtual void ApplyEtherToCauser(ABladeIICharacter* EtherCauser);
	virtual void ApplyEtherToOther(ABladeIICharacter* Other);	// Influenced by Cuaser or Owner

	virtual bool GetInfluencedCharacters(ABladeIICharacter* EtherCauser, TArray<ABladeIICharacter*>& OutInfluenced);

	float GetDamageAmount();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float MeteorRadius;
};

/* ----- 통찰(공격) - 대상 스킬 재사용시간 max [B2AetherSetOptionType::SKILL_COOLTIME = 1006 ] ----- */
UCLASS(Blueprintable)
class UB2Ether_IncreaseCooltime : public UB2EtherSetOffense
{
public:
	GENERATED_BODY()
	UB2Ether_IncreaseCooltime();

	virtual void ApplyEtherToCauser(ABladeIICharacter* EtherCauser);
	virtual void ProcessEffect(ABladeIICharacter* VictimCharacter) override;
};

/* ----- 분노(공격) - 결속스킬 쿨타임 감소 [B2AetherSetOptionType::SKILL_GAGE = 1007 ] ----- */
UCLASS(Blueprintable)
class UB2Ether_DecreaseCooltime : public UB2EtherSetOffense
{
public:
	GENERATED_BODY()
	UB2Ether_DecreaseCooltime();

	virtual void ApplyEtherToOwner(class ABladeIIPlayer* EtherOwner);
	virtual bool IsActivatable(ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage) override;
	bool IsAvailGameMode(EB2GameMode CurrentGameMode);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float DecreaseCooltime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float AddSkillPoint;
};

/* ----- 절망(공격) -  스턴 [B2AetherSetOptionType::STUN = 1008 ] ----- */
UCLASS(Blueprintable)
class UB2Ether_Stun : public UB2EtherSetOffense
{
public:
	GENERATED_BODY()
	UB2Ether_Stun();

	virtual void ApplyEtherToCauser(class ABladeIICharacter* EtherOwner);
	float GetDuration();
};


/* ----- 저항(방어) - 무적 [B2AetherSetOptionType::SUPER_ARMOR = 1009 ] ----- */
UCLASS(Blueprintable)
class UB2Ether_invincible : public UB2EtherSetDefense
{
public:
	GENERATED_BODY()
	UB2Ether_invincible();

	virtual bool IsActivatable(ABladeIICharacter* EtherCauser, const FDamageInfo& CauserDamageInfo, float ActualDamage) override;
	virtual void ApplyEtherToOwner(class ABladeIIPlayer* EtherOwner);

	float GetDuration();
	float GetActivateDamageRate();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float ActivateDamageRate;
};


/* ----- 복수(방어) - 데미지 반사 [B2AetherSetOptionType::DAMAGE_REFLECT = 1010 ] ----- */
UCLASS(Blueprintable)
class UB2Ether_Reflect : public UB2EtherSetDefense
{
public:
	GENERATED_BODY()
	UB2Ether_Reflect();

	virtual void ApplyEtherToCauser(ABladeIICharacter* EtherCauser);

	float GetReflectDamageAmount();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float ReflectAmount;
};

/* ----- 망각(공격) - N초간 방어/반격/구르기/태그 불능 상태 [B2AetherSetOptionType::PROHIBIT_BEHAVIOR = 1011] ----- */
UCLASS(Blueprintable)
class UB2Ether_NotTagDefence : public UB2EtherSetOffense
{
public:
	GENERATED_BODY()
	UB2Ether_NotTagDefence();

	virtual void ApplyEtherToCauser(ABladeIICharacter* EtherCauser) override;

	float GetDuration();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float Duration;
};

/* ----- 혹한(방어) - 빙결 디버프 [B2AetherSetOptionType::FREEZE_RANGE = 1012] ----- */
UCLASS(Blueprintable)
class UB2Ether_FreezeRange : public UB2EtherSetDefense
{
public:
	GENERATED_BODY()
	UB2Ether_FreezeRange();

	virtual void ApplyEtherToCauser(ABladeIICharacter* EtherCauser) override;
	virtual void ApplyEtherToOther(ABladeIICharacter* Other) override;	// Influenced by Cuaser or Owner
	virtual bool GetInfluencedCharacters(ABladeIICharacter* EtherCauser, TArray<ABladeIICharacter*>& OutInfluenced) override;

	float GetDuration();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ether")
	float Duration;
};