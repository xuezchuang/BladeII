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

	// -1이면 시간 안잼. 외부에서 해제
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
	// 이 시점에서 Buff End Broadcasting

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
	// Authority / Remote 모두 호출
	BII_CHECK(BuffEffectStore && GetOwnerCharacter());

	if (!m_bUseEffect)
		return;

	// 기존거 이펙트 남아있을거 대비해서 호출
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
		// 여긴 루핑 들어오면 아니됨.
		BII_CHECK(!pPSC->Template->IsLooping());
		BII_CHECK(!pPSC->Template->IsImmortal());
	}
}

void UB2Buff_Base::ActivateBuffText()
{
	if (!m_bUseTextEffect)
		return;

	// Authority / Remote 모두 호출
	BII_CHECK(BuffEffectStore && GetOwnerCharacter());

	BuffEffectStore->SpawnCharacterBuffTextEffect(BuffType, GetOwnerCharacter());
}

// CC기 점감
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