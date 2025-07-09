#include "BladeII.h"
#include "B2CharacterBuffManager.h"
#include "B2BuffModeEffectInfo.h"
#include "B2Buff_Base.h"
#include "B2GeneralBuffs.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"

UB2CharacterBuffManager::UB2CharacterBuffManager()
{
	CrowdControlCount = 0;
	SavedWorldTimeSeconds = 0;

	m_arBuffMaterialMark.Empty();
}

UB2CharacterBuffManager::~UB2CharacterBuffManager()
{
}

void UB2CharacterBuffManager::InitBuffManager(class ABladeIICharacter* OwnerCharacter)
{
	if(OwnerCharacter)
		m_pOwner = OwnerCharacter;

	SavedWorldTimeSeconds = GWorld->GetTimeSeconds();
}

UB2Buff_Base* UB2CharacterBuffManager::AddBuff(EBuffType BuffType, float fDuration, float Amount, AActor* InBuffCauser, bool bUseEffect/* = true*/, bool bUseTextEffect/* = true*/, TArray<float>* PtrAdditionalParams)
{
	if (BuffType == EBuffType::Buff_End || BuffType == EBuffType::Buff_None)
		return nullptr;

	UB2Buff_Base* PrevBuffInstance = FindBuff(BuffType);

	if (PrevBuffInstance && fDuration != -1)
	{
		if (NotAllowDuplicatedBuff(BuffType) && PrevBuffInstance->IsActive())
			return PrevBuffInstance;

		float fCompareDuration = fDuration;

		// ������� �ð����� �����ؼ� ��
		if (PrevBuffInstance->IsDebuff())
			fCompareDuration *= GetOwnerCharacter()->GetDurationDecreaseRate();

		// ���� ����ϴ� ������ �����ؼ� ��
		if(PrevBuffInstance->IsUseCrowdControlCount())
			fCompareDuration *= FMath::Pow(0.5, CrowdControlCount);
			
		// �������� �����ð�
		float fPrevBuffDuration = PrevBuffInstance->GetRemainTime();

		const bool IsBetterThanPrev = fPrevBuffDuration != -1 && fPrevBuffDuration < fCompareDuration ? true : false;
		
		// ���� Buff�� �ɷ��ִ� ���� && �����ź��� ª���� �밻��
		if (PrevBuffInstance->IsActive() && !IsBetterThanPrev)
			return PrevBuffInstance;
	}

	/* 
		BuffManager ���ο��� Buff�� ������ Pool �������� �����ȴ�
		�ѹ� �����Ǿ��� Buff�� �ٽ� ������ ���ɼ��� �����Ƿ� NewObject�� �ּ�ȭ�� ���ؼ� ������ �ʰ� Deactive�� ��Ŵ
	*/
	UB2Buff_Base* BuffInstance = FindOrCreateBuff(BuffType);
	if (BuffInstance != nullptr)
	{
		BuffInstance->SetBuffAmount(Amount);
		BuffInstance->BuffBegin(this, fDuration, InBuffCauser, bUseEffect, bUseTextEffect, PtrAdditionalParams);
	}
		
	OnPlayerBuffChanged();

	AddBuffMaterialMark(BuffType);

	return BuffInstance;
}

class UB2Buff_Base* UB2CharacterBuffManager::FindBuff(EBuffType BuffType) const
{
	for (UB2Buff_Base* pBuff : m_arBuff)
	{
		if (pBuff && pBuff->GetBuffType() == BuffType)
			return pBuff;
	}

	return nullptr;
}

void UB2CharacterBuffManager::ClearBuff(EBuffType BuffType)
{
	UB2Buff_Base* pBuff = FindBuff(BuffType);

	if (pBuff && pBuff->IsActive())
	{
		pBuff->BuffClear(false);		
	}
}

bool UB2CharacterBuffManager::IsBuffActive(EBuffType BuffType)
{
	return GetOwnerCharacter()->IsBuffActive(BuffType);
}

void UB2CharacterBuffManager::BuffTick()
{	
	const float fDeltaSec = GWorld->GetTimeSeconds() - SavedWorldTimeSeconds;
	SavedWorldTimeSeconds = GWorld->GetTimeSeconds();
	
	for (int32 BuffIdx = 0; BuffIdx < m_arBuff.Num(); BuffIdx++)
	{
		auto* BuffInstance = m_arBuff[BuffIdx];
		if (BuffInstance && BuffInstance->IsValidLowLevel() && BuffInstance->IsActive())
			BuffInstance->UpdateBuffTick(fDeltaSec);
	}
}

void UB2CharacterBuffManager::ClearAllBuffs()
{
	for (int32 BuffIdx = 0; BuffIdx < m_arBuff.Num(); BuffIdx++)
	{
		auto* BuffInstance = m_arBuff[BuffIdx];
		if (BuffInstance && BuffInstance->IsValidLowLevel() && BuffInstance->IsActive())
		{
			BuffInstance->BuffClear(false);
		}
	}
	m_arBuff.Empty();

	// ���������� �ɸ��� �ִϸ��̼� ����ӵ� üũ
	// Ŀ���� �����̼� �����ϴ� ������ �ȿ��� ��������
	//CheckCustomTimeDilation();
}

void UB2CharacterBuffManager::ClearDebuffs()
{
	for (UB2Buff_Base* pBuff : m_arBuff)
	{
		if (!pBuff->IsActive())
			continue;

		if (pBuff->IsDebuff())
			pBuff->BuffClear(false);
	}
}

// �ɸ��� CustomTimeDilation �����ϴ°� ������ ���Ѵ�. �߰��� ���� �ʿ��ϸ� Ŀ���� ���� �߰��ؼ� ����
void UB2CharacterBuffManager::CheckCustomTimeDilation()
{
	float AccumulatedDilation = 1.f;
	for (UB2Buff_Base* pBuff : m_arBuff)
	{
		if (pBuff && pBuff->IsActive())
			AccumulatedDilation *= pBuff->GetCustomTimeDilation();
	}

	GetOwnerCharacter()->SetCustomTimeDilation(AccumulatedDilation);
}

void UB2CharacterBuffManager::OwnerTakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	for (UB2Buff_Base* BuffInstance : m_arBuff)
	{
		if (BuffInstance && BuffInstance->IsActive())
		{
			BuffInstance->OwnerTakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		}
	}
}

void UB2CharacterBuffManager::AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo)
{
	for (UB2Buff_Base* BuffInstance : m_arBuff)
	{
		if (BuffInstance && BuffInstance->IsActive())
		{
			BuffInstance->AffectToAttacker(Victim, ActualDamage, DamageInfo);
		}
	}
}

void UB2CharacterBuffManager::AddBuffMaterialMark(EBuffType BuffType)
{
	if (!m_pOwner)
		return;

	bool bExist = GetOwnerCharacter()->GetBuffModeEffectInfo()->ApplyBuffMaterial(BuffType, m_pOwner);
	
	if(bExist)
		m_arBuffMaterialMark.AddUnique(BuffType);
}

void UB2CharacterBuffManager::RemoveBuffMaterialMark(EBuffType BuffType)
{
	if (!m_pOwner)
		return;

	if (m_arBuffMaterialMark.Contains(BuffType))
	{		
		GetOwnerCharacter()->GetBuffModeEffectInfo()->ResetBuffMaterial(m_pOwner);

		m_arBuffMaterialMark.Remove(BuffType);

		if (m_arBuffMaterialMark.Num() > 0)
		{
			EBuffType RestoreMatBuffType = m_arBuffMaterialMark[m_arBuffMaterialMark.Num() - 1];

			GetOwnerCharacter()->GetBuffModeEffectInfo()->ApplyBuffMaterial(RestoreMatBuffType, m_pOwner);
		}
	}
}

class UB2Buff_Base* UB2CharacterBuffManager::FindOrCreateBuff(EBuffType BuffType)
{
	UB2Buff_Base* PrevGeneratedBuff = FindBuff(BuffType);
	if (PrevGeneratedBuff)
	{		
		return PrevGeneratedBuff;
	}

	UB2Buff_Base* NewBuff = nullptr;
	switch (BuffType)
	{
	case EBuffType::Debuff_Immobilize:			NewBuff = CreateBuff<UB2Buff_Restraint>(BuffType); break;
	case EBuffType::Debuff_Burning:				NewBuff = CreateBuff<UB2Buff_DotDamage>(BuffType); break;
	case EBuffType::Buff_Shield:				NewBuff = CreateBuff<UB2Buff_Shield>(BuffType); break;
	case EBuffType::Debuff_Freezing:			NewBuff = CreateBuff<UB2Buff_Freezing>(BuffType); break;
	case EBuffType::Buff_ReflectDamage:			NewBuff = CreateBuff<UB2Buff_Reflect>(BuffType); break;
	case EBuffType::Debuff_Stun:				NewBuff = CreateBuff<UB2Buff_Stun>(BuffType); break;
	case EBuffType::Debuff_Poison:				NewBuff = CreateBuff<UB2Buff_Poison>(BuffType); break;
	case EBuffType::Buff_Hide:					NewBuff = CreateBuff<UB2Buff_Hide>(BuffType); break;
	case EBuffType::Buff_HealHPByAttack:		NewBuff = CreateBuff<UB2Buff_HealHPByAttack>(BuffType); break;
	case EBuffType::Buff_AbnormalStateImmune:	NewBuff = CreateBuff<UB2Buff_AbnormalStateImmune>(BuffType); break;
	case EBuffType::Buff_IncreaseMoveSpeed:		NewBuff = CreateBuff<UB2Buff_IncreaseMoveSpeed>(BuffType); break;
	case EBuffType::Buff_HitFreeze:				NewBuff = CreateBuff<UB2Buff_HitFreeze>(BuffType); break;
	case EBuffType::Buff_Enhance:				NewBuff = CreateBuff<UB2Buff_Enhance>(BuffType); break;
	case EBuffType::Buff_IncreaseDefense:		NewBuff = CreateBuff<UB2Buff_IncreaseDefense>(BuffType); break;
	case EBuffType::Debuff_Taunt:				NewBuff = CreateBuff<UB2Buff_Taunt>(BuffType); break;
	case EBuffType::Debuff_Seal:				NewBuff = CreateBuff<UB2Buff_Seal>(BuffType); break;
	case EBuffType::Buff_Dispell:				NewBuff = CreateBuff<UB2Buff_Dispell>(BuffType); break;
	case EBuffType::Buff_Heal:					NewBuff = CreateBuff<UB2Buff_Heal>(BuffType); break;
	case EBuffType::Buff_AllFreezeAttack:
	case EBuffType::Buff_AllStunAttack:
	case EBuffType::Buff_AllBurningAttack:
	case EBuffType::Buff_AllPoisonAttack:
		NewBuff = CreateBuff<UB2Buff_DebuffAttack>(BuffType);
		break;

	// �ܼ� ���������� ����� �����⵵�Ѵ�.
	default:									NewBuff = CreateBuff<UB2Buff_Base>(BuffType); break;
	}
	
	NewBuff->SetBuffType(BuffType);

	m_arBuff.Add(NewBuff);
	return NewBuff;
}

class ABladeIICharacter* UB2CharacterBuffManager::GetOwnerCharacter()
{
	return m_pOwner;
}

TArray<UB2Buff_Base*>* UB2CharacterBuffManager::GetBuffList()
{
	return &m_arBuff;
}

// ���� �ɸ��ų� �����Ǹ� ������;���.
void UB2CharacterBuffManager::OnPlayerBuffChanged()
{
	CheckCustomTimeDilation();
}

bool UB2CharacterBuffManager::NotAllowDuplicatedBuff(EBuffType InBuffType)
{
	if(InBuffType == EBuffType::Buff_Enhance)
		return true;

	return false;
}
