#pragma once
//#include "BladeII.h"
#include "B2Buff_Base.generated.h"

UCLASS()
class BLADEII_API UB2Buff_Base : public UObject
{
	GENERATED_BODY()
public:
	UB2Buff_Base();
	virtual ~UB2Buff_Base();

	void SetBuffType(EBuffType Type);
	EBuffType GetBuffType() const;

	virtual void BuffBegin(class UB2CharacterBuffManager* pBuffManager, float fDuration, AActor* InBuffCauser, bool bUseEffect = true, bool bUseTextEffect = true, TArray<float>* PtrAdditionalParams = nullptr);
	virtual void UpdateBuffTick(const float DeltaSeconds);
	void BuffClear(bool bTimeOut = true);

	virtual void OnBeginBuff(const float BuffDuration) {}
	virtual void OnTick(const float DeltaSeconds) {}
	virtual void OnClearBuff(bool bTimeOut) {}

	virtual void OwnerTakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {}
	virtual void AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo) {}

	FORCEINLINE void Activate()						{ bActive = true; }
	FORCEINLINE void Deactivate()					{ bActive = false; }
	virtual void SetBuffAmount(const float Amount)	{ BuffAmount = Amount; }

	class ABladeIICharacter* GetOwnerCharacter();

	FORCEINLINE bool IsActive() const { return bActive; }
	FORCEINLINE float GetBuffBeginTime() { return BeginTime; }
	FORCEINLINE float GetRemainTime() { return Duration == -1.0f ? -1.0f : Duration - ElapsedSeconds;}

	FORCEINLINE UB2CharacterBuffManager* GetBuffManager() { return BuffManager; }
	FORCEINLINE float GetBuffAmount() const { return BuffAmount; }
	FORCEINLINE float GetElapsedTime() const { return ElapsedSeconds; }
	FORCEINLINE bool IsTickableBuff() const { return bTickableBuff; }
	FORCEINLINE bool IsOverDuration() { return Duration != -1.0f && ElapsedSeconds >= Duration; }
	FORCEINLINE AActor* GetBuffCauser() const { return BuffCauser.IsValid() ? BuffCauser.Get() : nullptr; }
	
	bool IsDebuff() { return bDebuff; }

	virtual float GetCustomTimeDilation() { return 1.f; }

// Network Interface

	bool IsLocalBuff();

// Effect Interface

	virtual void ActiveBuffEffect();
	virtual void DeactiveBuffEffect();

	virtual void ActiveBuffEndEffect();

	virtual void ActivateBuffText();

	void CheckCrowdControlCount();

	bool IsUseCrowdControlCount() { return bUseCrowdControlCount; }

protected:

	class UB2CharacterBuffManager* BuffManager;

// Buff Own Property
	
	EBuffType	BuffType;

	float		BeginTime;
	float		Duration;
	float		ElapsedSeconds;

	bool		bActive;

	TWeakObjectPtr<AActor>	BuffCauser;
	float					BuffAmount;

	bool		bTickableBuff;

	bool		bDebuff;

	// CC점감 사용여부
	bool		bUseCrowdControlCount;

// Effect

	class UB2BuffModeEffectInfo* BuffEffectStore;

	UPROPERTY(Transient)
	UParticleSystemComponent* BuffEffectParticle;

	bool m_bUseEffect;
	bool m_bUseTextEffect;

	TArray<float> m_arAdditionalParams;
};
