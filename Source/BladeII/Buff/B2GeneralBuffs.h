#pragma once
//#include "BladeII.h"
#include "B2Buff_Base.h"
#include "B2GeneralBuffs.generated.h"

static const TArray<EBuffType> DebuffGroup = {EBuffType::Debuff_Freezing , EBuffType::Debuff_Seal, EBuffType::Debuff_Stun };

void ClearDebuffGroup(EBuffType CurrentBuff, class ABladeIICharacter* Character);

/*
	�ӹ�
*/
UCLASS()
class BLADEII_API UB2Buff_Restraint : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_Restraint();
	virtual ~UB2Buff_Restraint() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnTick(const float DeltaSeconds);

private:
	FVector m_vRestraintPos;
};


/*
	����
*/
UCLASS()
class BLADEII_API UB2Buff_Shield : public UB2Buff_Base
{
	GENERATED_BODY()
public:

	UB2Buff_Shield();
	virtual ~UB2Buff_Shield() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnClearBuff(bool bTimeOut) override;

	void SetShieldAmount(const float Amount);
	FORCEINLINE float GetShieldAmount() { return ShieldAmount; }

protected:

	bool bShieldClear;
	float ShieldAmount;
};


/*
	DoT ������
*/
UCLASS()
class BLADEII_API UB2Buff_DotDamage : public UB2Buff_Base
{
	GENERATED_BODY()

public:

	UB2Buff_DotDamage();
	virtual ~UB2Buff_DotDamage() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnTick(const float DeltaSeconds);
	virtual void OnClearBuff(bool bTimeOut) override;

	virtual void OnDotActive(const int32 DotTickCount);

	virtual void PostInitProperties() override;
	
	void OverrideDamageInfo(const FDamageInfo& NewDamageInfo) 
	{
		DamageInfo.SetDamageInfo(&NewDamageInfo, GetOwnerCharacter()); 
	}
	
protected:	
	int32 CurrentDotCount;

	float DotTickTime;

	FDamageInfo DamageInfo;

public:
	EDamageLogicType DotDamageLogicType;
};


/*
	����
*/
UCLASS()
class BLADEII_API UB2Buff_Freezing : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_Freezing();
	virtual ~UB2Buff_Freezing() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnTick(const float DeltaSeconds);
	virtual void OnClearBuff(bool bTimeOut) override;

	virtual float GetCustomTimeDilation() override;
};


/*
	�ݻ�
*/
UCLASS()
class BLADEII_API UB2Buff_Reflect : public UB2Buff_Base
{
	GENERATED_BODY()

public:
	UB2Buff_Reflect();
	virtual ~UB2Buff_Reflect() {}
	virtual void OwnerTakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	virtual void PostInitProperties() override;

protected:
	FDamageInfo DamageInfo;
};


/*
	����
*/
UCLASS()
class BLADEII_API UB2Buff_Stun : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_Stun();
	virtual ~UB2Buff_Stun() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnTick(const float DeltaSeconds);
	virtual void OnClearBuff(bool bTimeOut) override;
};


/*
	�ߵ�
*/
UCLASS()
class BLADEII_API UB2Buff_Poison : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_Poison();
	virtual ~UB2Buff_Poison() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnTick(const float DeltaSeconds);
	virtual void OnClearBuff(bool bTimeOut) override;

	virtual float GetCustomTimeDilation() override;
};


/*
	����
*/
UCLASS()
class BLADEII_API UB2Buff_Hide : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_Hide();
	virtual ~UB2Buff_Hide() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnTick(const float DeltaSeconds);
	virtual void OnClearBuff(bool bTimeOut) override;
};


/*
	����
*/
UCLASS()
class BLADEII_API UB2Buff_HealHPByAttack : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_HealHPByAttack();
	virtual ~UB2Buff_HealHPByAttack() {}
	virtual void AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo) override;
};


/*
	����� �̹�
*/
UCLASS()
class BLADEII_API UB2Buff_AbnormalStateImmune : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_AbnormalStateImmune();
	virtual ~UB2Buff_AbnormalStateImmune() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnClearBuff(bool bTimeOut) override;
};


/*
	�̼�����
*/
UCLASS()
class BLADEII_API UB2Buff_IncreaseMoveSpeed : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_IncreaseMoveSpeed();
	virtual ~UB2Buff_IncreaseMoveSpeed() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnTick(const float DeltaSeconds);
	virtual void OnClearBuff(bool bTimeOut) override;
};


/*
	�����̻� ����
*/
UCLASS()
class BLADEII_API UB2Buff_DebuffAttack : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_DebuffAttack();
	virtual ~UB2Buff_DebuffAttack() {}
	float GetDebuffDuration();
	float GetDebuffTriggerRate();

	virtual void AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo);
};


UCLASS()
class BLADEII_API UB2Buff_HitFreeze : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_HitFreeze();
	virtual ~UB2Buff_HitFreeze() {}

	virtual float GetCustomTimeDilation() { return 0.f; }
};

/*
���� ��ȭ
*/
UCLASS()
class BLADEII_API UB2Buff_Enhance : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_Enhance();
	virtual ~UB2Buff_Enhance() {}
	virtual void OnBeginBuff(const float BuffDuration) override;
	virtual void OnTick(const float DeltaSeconds) override;
	virtual void OnClearBuff(bool bTimeOut) override;

private:
	static const float ChangeScaleSize;
	FVector RestoreScale;
	float ScaleDuration;
	bool NoAnimateScaleUP;
};

/*
���� ���
*/
UCLASS()
class BLADEII_API UB2Buff_IncreaseDefense : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_IncreaseDefense();
	virtual ~UB2Buff_IncreaseDefense() {}
	virtual void OnBeginBuff(const float BuffDuration) override;
	virtual void OnClearBuff(bool bTimeOut) override;
};

/*
����� ����
*/
UCLASS()
class BLADEII_API UB2Buff_Taunt : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_Taunt();
	virtual ~UB2Buff_Taunt() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnClearBuff(bool bTimeOut) override;
};

/*
����� ħ��(��ų ��� ����)
*/
UCLASS()
class BLADEII_API UB2Buff_Silent : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_Silent();
	virtual ~UB2Buff_Silent() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnClearBuff(bool bTimeOut) override;
};


/*
����� ����(�ൿ���� == ����¡ ȿ���� ���� ����Ʈ�� �ٸ�)
*/
UCLASS()
class BLADEII_API UB2Buff_Seal : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_Seal();
	virtual ~UB2Buff_Seal() {}
	virtual void OnBeginBuff(const float BuffDuration);
	virtual void OnClearBuff(bool bTimeOut) override;

	virtual float GetCustomTimeDilation() override;
};

UCLASS()
class BLADEII_API UB2Buff_InstanceBuff : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_InstanceBuff();
	virtual ~UB2Buff_InstanceBuff() {}

	virtual void BuffBegin(class UB2CharacterBuffManager* pBuffManager, float fDuration, AActor* InBuffCauser, bool bUseEffect = true, bool bUseTextEffect = true, TArray<float>* PtrAdditionalParams = nullptr) final;
	virtual void UpdateBuffTick(const float DeltaSeconds) final;
	virtual void OnTick(const float DeltaSeconds) final {}
	virtual void OnClearBuff(bool bTimeOut) final {}
};

/*
����
*/
UCLASS()
class BLADEII_API UB2Buff_Dispell : public UB2Buff_InstanceBuff
{
	GENERATED_BODY()
public:
	UB2Buff_Dispell();
	virtual ~UB2Buff_Dispell() {}
	virtual void OnBeginBuff(const float BuffDuration);

};


/*
���� ��
*/
UCLASS()
class BLADEII_API UB2Buff_Heal : public UB2Buff_Base
{
	GENERATED_BODY()
public:
	UB2Buff_Heal();
	virtual ~UB2Buff_Heal() {}
	virtual void OnBeginBuff(const float BuffDuration) override;
	virtual void OnTick(const float DeltaSeconds) override;
	virtual void OnClearBuff(bool bTimeOut) override;

protected:
	float HealTickTime;
	int32 CurrentHealCount;
	float RemainAmount;
};