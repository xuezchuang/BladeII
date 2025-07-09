#include "BladeII.h"
#include "B2Buff_Base.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
#include "B2BuffModeEffectInfo.h"
#include "B2CharacterBuffManager.h"

UB2Buff_Base::UB2Buff_Base()
{
	BuffManager = nullptr;

	BuffType = EBuffType::Buff_None;
	BeginTime = 0.f;
	Duration = 0.f;
	ElapsedSeconds = 0.f;
	bActive= false;
	BuffCauser.Reset();
	bDebuff = false;

	BuffEffectStore = nullptr;
	BuffEffectParticle = nullptr;

	m_arAdditionalParams.Empty();

	bUseCrowdControlCount = false;
}

UB2Buff_Base::~UB2Buff_Base()
{
}

void UB2Buff_Base::SetBuffType(EBuffType Type)
{
	BuffType = Type;
}

EBuffType UB2Buff_Base::GetBuffType() const
{
	return BuffType;
}

void UB2Buff_Base::BuffBegin(class UB2CharacterBuffManager* pBuffManager, float fDuration, AActor* InBuffCauser, bool bUseEffect/* = true*/, bool bUseTextEffect/* = true*/, TArray<float>* PtrAdditionalParams/* = nullptr*/)
{
	BuffManager = pBuffManager;
	
	BeginTime = GWorld->TimeSeconds;

	// -1�̸� �ð� ����. �ܺο��� ����
	Duration = fDuration;

	ElapsedSeconds = 0.f;

	BuffCauser = InBuffCauser;

	m_bUseEffect = bUseEffect;
	m_bUseTextEffect = bUseTextEffect;

	if (PtrAdditionalParams)
	{
		m_arAdditionalParams = *PtrAdditionalParams;
	}

	Activate();
	//
	//if (GetOwnerCharacter())
	//	BuffEffectStore = GetOwnerCharacter()->GetBuffModeEffectInfo();
	
	OnBeginBuff(fDuration);

	if(bUseCrowdControlCount)
		CheckCrowdControlCount();

	ActiveBuffEffect();
	ActivateBuffText();	
}

void UB2Buff_Base::UpdateBuffTick(const float DeltaSeconds)
{
	if (IsActive())
	{
		ElapsedSeconds += DeltaSeconds;

		OnTick(DeltaSeconds);

		if (IsOverDuration())
			BuffClear(true);
	}
}

void UB2Buff_Base::BuffClear(bool bTimeOut/* = true*/)
{
	if (!IsActive())
		return;

	OnClearBuff(bTimeOut);

	DeactiveBuffEffect();
	ActiveBuffEndEffect();
	// �� �������� Buff End Broadcasting

	Deactivate();
	Duration = 0.f;
	BeginTime = 0.f;

	BuffCauser.Reset();
	
	BuffManager->OnPlayerBuffChanged();
	BuffManager->RemoveBuffMaterialMark(BuffType);
}

bool UB2Buff_Base::IsLocalBuff()
{
	if (GetOwnerCharacter() && GetOwnerCharacter()->IsLocallyControlled())
	{
		return GetOwnerCharacter()->GetLocalRole() == ROLE_Authority ? true : false;
	}

	return false;
}

void UB2Buff_Base::ActiveBuffEffect()
{
	// Authority / Remote ��� ȣ��
	BII_CHECK(BuffEffectStore && GetOwnerCharacter());

	if (!m_bUseEffect)
		return;

	// ������ ����Ʈ ���������� ����ؼ� ȣ��
	DeactiveBuffEffect();

	BuffEffectParticle = BuffEffectStore->CharacterBuffEffect(BuffType, GetOwnerCharacter());
	
	if (BuffEffectParticle)
	{
		BuffEffectParticle->bAutoDestroy = false;
		BuffEffectParticle->ActivateSystem(true);
	}
}

void UB2Buff_Base::DeactiveBuffEffect()
{
//	if (BuffEffectParticle && !BuffEffectParticle->IsPendingKill())
//	{
//		BuffEffectParticle->Deactivate();
//		BuffEffectParticle->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
//		BuffEffectParticle->UnregisterComponent();
//		BuffEffectParticle->DestroyComponent();
//	}

	BuffEffectParticle = nullptr;
}

void UB2Buff_Base::ActiveBuffEndEffect()
{
	if (!m_bUseEffect)
		return;

	UParticleSystemComponent* pPSC = BuffEffectStore->CharacterBuffEffect(BuffType, GetOwnerCharacter(), true);

	if (pPSC && pPSC->Template)
	{
		// ���� ���� ������ �ƴϵ�.
		BII_CHECK(!pPSC->Template->IsLooping());
		BII_CHECK(!pPSC->Template->IsImmortal());
	}
}

void UB2Buff_Base::ActivateBuffText()
{
	if (!m_bUseTextEffect)
		return;

	// Authority / Remote ��� ȣ��
	BII_CHECK(BuffEffectStore && GetOwnerCharacter());

	BuffEffectStore->SpawnCharacterBuffTextEffect(BuffType, GetOwnerCharacter());
}

// CC�� ����
void UB2Buff_Base::CheckCrowdControlCount()
{
	if (!BuffManager || Duration <= 0)
		return;

	Duration = Duration * FMath::Pow(0.5, BuffManager->CrowdControlCount);

	BuffManager->CrowdControlCount++;
}

class ABladeIICharacter* UB2Buff_Base::GetOwnerCharacter()
{
	return GetBuffManager()->GetOwnerCharacter();
}