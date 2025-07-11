
#include "BladeIINetControlObject.h"
#include "B2UIManager.h"
#include "B2UIControlBattleSub.h"
#include "B2ControlGameMode.h"

ABladeIINetControlObject::ABladeIINetControlObject(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick	= true;
	//bReplicates						= false;
	//bCanBeDamaged					= false;

	bWaitSendChangeControlState = false;
	bWaitSendSetConquestAreaState = false;

	// This can check player overlap for EAWPPC_Overlap, as well as being the root component.
	DefaultCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DefaultCollisionComponent"));
	DefaultCollisionComponent->InitSphereRadius(200.0f);
	DefaultCollisionComponent->SetCollisionProfileName(CollisionProfileName_OverlapMostDynamic);

	ViewLimitLength = 1000.0f;
	ConquestGageMaxTime = 5.0f;
	ControlPointGageMaxTime = 100.0f;
	BonusTime = 5.0f;

	bActiveControlObject = false;

	LimitControlPointForBonusTimeRed = 0.99;
	LimitControlPointForBonusTimeBlue = 0.99;

	BonueTimeTeam = ETeamType::ETT_End;

	BonusTimeGage = 0.0f;

	GageRateForTutorial = 2.0f;

	NotifyControlPointTerm = 5.0f;

	m_fNotifyControlPointRemainTime = NotifyControlPointTerm;
}

void ABladeIINetControlObject::BeginPlay()
{
	Super::BeginPlay();

	StateMaps.Emplace(CONTROL_STATE_NEUTRAL, new NeutralState);
	StateMaps.Emplace(CONTROL_STATE_RED, new RedState);
	StateMaps.Emplace(CONTROL_STATE_BLUE, new BlueState);

	ControledState	= StateMaps[CONTROL_STATE_NEUTRAL];

	// ������ �ξƿ� ��������Ʈ(�ɸ���)
	//DefaultCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABladeIINetControlObject::OnInCheckConquestArea);
	//DefaultCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABladeIINetControlObject::OnOutCheckConquestArea);

	ConquestAreaState = EConquestAreaState::None;

	m_fConquestGageRed = 0.0f;
	m_fConquestGageBlue = 0.0f;

	bEndMatch = false;

	ChangeState(CONTROL_STATE_NEUTRAL);
}

void ABladeIINetControlObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	check(ControledState != nullptr);

	UpdateBonusTimeGage(DeltaSeconds);

	ControledState->Tick(this, DeltaSeconds);

//	if (AssultState)
//		AssultState->Tick(this, DeltaSeconds);

	//Log
	//UE_LOG(LogBladeII, Log, TEXT("Red Gage : %f"), m_fConquestGageRed);
	//UE_LOG(LogBladeII, Log, TEXT("Blue Gage : %f\n"), m_fConquestGageBlue);
	//
	//UE_LOG(LogBladeII, Log, TEXT("Red Point Gage : %f"), m_fControlPointGageRed);
	//UE_LOG(LogBladeII, Log, TEXT("Blue Point Gage : %f\n"), m_fControlPointGageBlue);

	// �� ƽ���� üũ��. �浹 ���� �������� üũ�ϴ� �ȸ´°�찡 ����.
	CheckOverlap();

	CheckToNotifyControlPoint(DeltaSeconds);
}

void ABladeIINetControlObject::BeginDestroy()
{
	DeleteInternal();	
	Super::BeginDestroy();
}

void ABladeIINetControlObject::Destroyed()
{
	DeleteInternal();
	Super::Destroyed();
}

void ABladeIINetControlObject::DeleteInternal()
{
	for (auto& Elem : StateMaps)
	{
		if (!StateMaps[Elem.Key])
			continue;

		delete StateMaps[Elem.Key];
	}

	StateMaps.Empty();
}

void ABladeIINetControlObject::ChangeState(uint8 NewStateKey)
{
	// ȣ��Ʈ �ƴϸ� ����!
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	check(pGM); /** �̰��� ����ϴ� ���Ӹ�尡 ������ ���� */
	if (pGM && pGM->GetPeerType() == PEER_SUPER)
	{
		// ������ ���� �ȿ����� ����
		if (bWaitSendChangeControlState)
			return;

		bWaitSendChangeControlState = true;

		pGM->SendChangeControlState(NewStateKey, m_fControlPointGageRed, m_fControlPointGageBlue, true);
	}

	// ���ε� �޾Ƽ� ó���غ���.
	//ApplyChangeState(NewStateKey);
}

void ABladeIINetControlObject::ApplyChangeState(uint8 NewStateKey, float fControlPointGageRed, float fControlPointGageBlue, bool bResetConquestGage)
{
	bWaitSendChangeControlState = false;

	if (bResetConquestGage)
	{
		// ������ ������ �ʱ�ȭ(���� �Ϸ��Ĵ� ������ �������� �ǹ̾���)
		m_fConquestGageRed = 0.0f;
		m_fConquestGageBlue = 0.0f;
	}

	IState** NewState = StateMaps.Find(NewStateKey);

	if (NewState)
	{
		ControledState = *NewState;

		m_fControlPointGageRed = fControlPointGageRed;
		m_fControlPointGageBlue = fControlPointGageBlue;

		// �̺�Ʈ �߰�. ���� ���� UI�� ����
		UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
		if (pUI)
		{
			pUI->SetControlObjectState((*NewState)->GetState(), m_fConquestGageRed, m_fConquestGageBlue);
			pUI->SetControlPoint(true, m_fControlPointGageRed);
			pUI->SetControlPoint(false, m_fControlPointGageBlue);
		}
	}

	// �߰��ð� �ʱ�ȭ
	SetBonusTime(ETeamType::ETT_End);
}

void ABladeIINetControlObject::ApplyCurrentStateToUI()
{
	UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
	if (pUI)
	{
		pUI->SetControlObjectState(ControledState ? ControledState->GetState() : CONTROL_STATE_NEUTRAL, m_fConquestGageRed, m_fConquestGageBlue);
		pUI->SetControlPoint(true, m_fControlPointGageRed);
		pUI->SetControlPoint(false, m_fControlPointGageBlue);
	}
}

void ABladeIINetControlObject::CheckOverlap()
{
	// Ȱ��ȭ ���̸� ����
	if (!bActiveControlObject)
		return;

	// ȣ��Ʈ �ƴϸ� ����!
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	if (pGM->GetPeerType() != PEER_SUPER)
		return;

	//TArray<AActor*> Overlaps;
	//DefaultCollisionComponent->GetOverlappingActors(Overlaps);
	
	bool bInRed = false;
	bool bInBlue = false;
	
	//for (AActor* pActor : Overlaps)
	//{
	//	ABladeIICharacter* pChar = Cast<ABladeIICharacter>(pActor);
	//	if (pChar && pChar->IsAlive())
	//	{
	//		if (ETeamType(pChar->GetTeamNum()) == ETeamType::ETT_Red)
	//			bInRed = true;
	//
	//		if (ETeamType(pChar->GetTeamNum()) == ETeamType::ETT_Blue)
	//			bInBlue = true;
	//
	//	}
	//}

	// QTEŸ�� �浹ü�� ����.. �Ÿ��� üũ
	/*for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	{
		ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(*Iterator);

		if (pPlayer && pPlayer->IsAlive())
		{
			FVector LengthVec = GetActorLocation() - pPlayer->GetActorLocation();

			if (LengthVec.Size() < DefaultCollisionComponent->GetScaledSphereRadius())
			{
				if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Red)
					bInRed = true;

				if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Blue)
					bInBlue = true;
			}
		}
	}*/

	if (bInRed && bInBlue)
	{
		SetConquestAreaState(EConquestAreaState::Assault);
		UE_LOG(LogBladeII, Log, TEXT("Controll Log : ASSAULT"));
	}
	else if (!bInRed && !bInBlue)
	{
		SetConquestAreaState(EConquestAreaState::None);
		//UE_LOG(LogBladeII, Log, TEXT("Controll Log : ZERO"));
	}
	else
	{
		if (bInRed)
		{
			SetConquestAreaState(EConquestAreaState::OwnerRed);
			UE_LOG(LogBladeII, Log, TEXT("Controll Log : Owner Red"));
		}

		if (bInBlue)
		{
			SetConquestAreaState(EConquestAreaState::OwnerBlue);
			UE_LOG(LogBladeII, Log, TEXT("Controll Log : Owner Blue"));
		}
	}
}

void ABladeIINetControlObject::SetBonusTime(ETeamType OwnerTeam)
{
	if(OwnerTeam == ETeamType::ETT_End)
		BonusTimeGage = 0.0f;
	else
		BonusTimeGage = 1.0f;

	BonueTimeTeam = OwnerTeam;
}

bool ABladeIINetControlObject::IsinBonusTime()
{
	return BonueTimeTeam != ETeamType::ETT_End;
}

float ABladeIINetControlObject::GetAreaRadius()
{
	if (DefaultCollisionComponent)
		return DefaultCollisionComponent->GetScaledSphereRadius();

	return 0.0f;
}

void ABladeIINetControlObject::UpdateBonusTimeGage(float DeltaSeconds)
{
	if (!IsinBonusTime())
		return;

	if (GetConquestAreaState() == EConquestAreaState::None ||
		(BonueTimeTeam == ETeamType::ETT_Red && GetConquestAreaState() == EConquestAreaState::OwnerRed) ||
		(BonueTimeTeam == ETeamType::ETT_Blue && GetConquestAreaState() == EConquestAreaState::OwnerBlue)
)
	{
		BonusTimeGage -= DeltaSeconds / BonusTime;

		if (BonusTimeGage <= 0)
		{
			BonusTimeGage = 0.0f;
			SetBonusTime(ETeamType::ETT_End);
		}
	}
	else
	{	
		BonusTimeGage = 1.0f;
	}
}

bool ABladeIINetControlObject::IsOverlap(AActor* pTarget)
{
	TArray<AActor*> Overlaps;
	DefaultCollisionComponent->GetOverlappingActors(Overlaps);

	bool bInRed = false;
	bool bInBlue = false;

	for (AActor* pActor : Overlaps)
	{
		if (pActor == pTarget)
			return true;
	}

	return false;
}



bool ABladeIINetControlObject::IsMatchingConquestAreaTeam(ETeamType teamType)
{
	if (teamType == ETeamType::ETT_Red && GetConquestAreaState() == EConquestAreaState::OwnerRed)
		return true;

	if (teamType == ETeamType::ETT_Blue && GetConquestAreaState() == EConquestAreaState::OwnerBlue)
		return true;

	return false;

}

bool ABladeIINetControlObject::IsAssaultState()
{
	return GetConquestAreaState() == EConquestAreaState::Assault;
}

void ABladeIINetControlObject::SetActiveControlObject(bool NewActive)
{
	bActiveControlObject = NewActive;

	// Ȱ��ȭ �������� ������ �ѹ� üũ
	//if (NewActive)
	//	CheckOverlap();
}

bool ABladeIINetControlObject::IsObjectUpdateable()
{
	return IsActiveControlObject() && !bEndMatch;
}

bool ABladeIINetControlObject::IsNeutralState()
{
	return ControledState == StateMaps[CONTROL_STATE_NEUTRAL];
}

float ABladeIINetControlObject::GetBonusTimeGage()
{
	return BonusTimeGage;
}

void ABladeIINetControlObject::AddConquestGage(bool bRed, float fAmount)
{
	// �߰��ð��� ������ ������ ������ �ʴ´�.
	if (fAmount <= 0 && IsinBonusTime())
		return;

	if (bRed)
	{
		bool bUpdateUI = m_fConquestGageRed != 0.0f;
		m_fConquestGageRed += fAmount;
		m_fConquestGageRed = FMath::Clamp(m_fConquestGageRed, 0.0f, 1.0f);

		if (bUpdateUI)
		{
			// �̺�Ʈ �߰�. ���� ���� UI�� ����
			UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
			if (pUI)
				pUI->SetConquestGageInfo(bRed, m_fConquestGageRed);
		}

		//���� ����
		if (m_fConquestGageRed >= 1.0f)
			ChangeState(CONTROL_STATE_RED);
	}
	else
	{
		bool bUpdateUI = m_fConquestGageBlue != 0.0f;
		m_fConquestGageBlue += fAmount;
		m_fConquestGageBlue = FMath::Clamp(m_fConquestGageBlue, 0.0f, 1.0f);

		if (bUpdateUI)
		{
			// �̺�Ʈ �߰�. ���� ���� UI�� ����
			UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
			if (pUI)
				pUI->SetConquestGageInfo(bRed, m_fConquestGageBlue);
		}

		//���� ����
		if (m_fConquestGageBlue >= 1.0f)
			ChangeState(CONTROL_STATE_BLUE);
	}
}

void ABladeIINetControlObject::AddControlPointGage(bool bRed, float fAmount)
{
	if (IsinBonusTime())
		return;

	UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
	
	if (bRed)
	{
		m_fControlPointGageRed += fAmount;
		m_fControlPointGageRed = FMath::Clamp(m_fControlPointGageRed, 0.0f, 1.0f);

		// ����Ʈ�� �߰��ð� ����ġ���� ������
		if (m_fControlPointGageRed > LimitControlPointForBonusTimeRed)
		{
			if (GetConquestAreaState() == EConquestAreaState::Assault || GetConquestAreaState() == EConquestAreaState::OwnerBlue)
			{
				// �ݵ����̸� �߰��ð�!
				// ����� �������̾ �߰��ð�!
				SetBonusTime(ETeamType::ETT_Red);
				m_fControlPointGageRed = LimitControlPointForBonusTimeRed;
			}
			else
			{
				// �ݵ��� �ƴϸ� �׳� �ø�. �߰��ð� ���ص� �ø�.
				LimitControlPointForBonusTimeRed = m_fControlPointGageRed;
			}
		}

		if (pUI)
			pUI->SetControlPoint(bRed, m_fControlPointGageRed);
	}
	else
	{
		m_fControlPointGageBlue += fAmount;
		m_fControlPointGageBlue = FMath::Clamp(m_fControlPointGageBlue, 0.0f, 1.0f);

		// ����Ʈ�� �߰��ð� ����ġ���� ������
		if (m_fControlPointGageBlue > LimitControlPointForBonusTimeBlue)
		{
			if (GetConquestAreaState() == EConquestAreaState::Assault || GetConquestAreaState() == EConquestAreaState::OwnerRed)
			{
				// �ݵ����̸� �߰��ð�!
				// ����� �������̾ �߰��ð�!
				SetBonusTime(ETeamType::ETT_Blue);
				m_fControlPointGageBlue = LimitControlPointForBonusTimeBlue;
			}
			else
			{
				// �ݵ��� �ƴϸ� �׳� �ø�. �߰��ð� ���ص� �ø�.
				LimitControlPointForBonusTimeBlue = m_fControlPointGageBlue;
			}
		}

		if (pUI)
			pUI->SetControlPoint(bRed, m_fControlPointGageBlue);
	}

	if (m_fControlPointGageRed >= 1.0f || m_fControlPointGageBlue >= 1.0f)
	{
		AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
		if (pGM->GetPeerType() == PEER_SUPER)
		{
			bEndMatch = true;
			// ��� ���� ���
			
			pGM->SendEndMatch(bRed ? ETeamType::ETT_Red : ETeamType::ETT_Blue);
		}
	}
}

void ABladeIINetControlObject::SetConquestAreaState(EConquestAreaState NewState)
{
	if (bWaitSendSetConquestAreaState)
		return;

	if (ConquestAreaState == NewState)
		return;

	// ȣ��Ʈ�� ���Ӹ�� ���ؼ� ���!
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	if (pGM->GetPeerType() == PEER_SUPER)
		pGM->SendChangeConquestAreaState(int32(NewState), m_fConquestGageRed, m_fConquestGageBlue);

	// ���ε� ��Ŷ�޾Ƽ� ó���غ�.
	//ApplyConquestAreaState();

	bWaitSendSetConquestAreaState = true;
}

void ABladeIINetControlObject::ApplyConquestAreaState(uint8 NewState, float fConquestGageRed, float fConquestGageBlue)
{
	bWaitSendSetConquestAreaState = false;

	m_fConquestGageRed = fConquestGageRed;
	m_fConquestGageBlue = fConquestGageBlue;
	ConquestAreaState = EConquestAreaState(NewState);
}

// �������� �ѹ� ���!
void ABladeIINetControlObject::BroadCastCurrentState()
{
	// ������ ����
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	if (pGM->GetPeerType() == PEER_SUPER)
	{
		pGM->SendChangeConquestAreaState(uint8(GetConquestAreaState()), m_fConquestGageRed, m_fConquestGageBlue);
		pGM->SendChangeControlState(ControledState->GetState(), m_fControlPointGageRed, m_fControlPointGageBlue, false);
	}
}

void ABladeIINetControlObject::CheckToNotifyControlPoint(float DeltaSeconds)
{
	if (!bActiveControlObject)
		return;

	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	if (pGM && pGM->GetPeerType() == PEER_SUPER)
	{
		m_fNotifyControlPointRemainTime -= DeltaSeconds;

		if (m_fNotifyControlPointRemainTime <= 0)
		{
			BroadCastCurrentState();
			m_fNotifyControlPointRemainTime = NotifyControlPointTerm;
		}
	}	
}

//void ABladeIINetControlObject::OnCharacterDead()
//{
//	CheckOverlap();
//}
//
//void ABladeIINetControlObject::OnInCheckConquestArea(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	CheckOverlap();
//}
//
//void ABladeIINetControlObject::OnOutCheckConquestArea(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	CheckOverlap();
//}

void NeutralState::Tick(ABladeIINetControlObject* CO, float delta)
{
	if (!CO->IsObjectUpdateable())
		return;
	
	// dTime���� ������ ���ٶ� 0���Ϸ� ���������� �����Ѵ�.(���� �ݴ��� �������� �������� ����)

	//if (IsActiveControlTutorial())
	//{
	//	delta *= CO->GageRateForTutorial;
	//}

	//switch (CO->GetConquestAreaState())
	//{
	//case EConquestAreaState::Assault:
	//	// �ݵ����̸� �ϰ͵� ����
	//	break;
	//case EConquestAreaState::None:
	//	// �ƹ��� ������ �Ѵ� ��������.
	//	CO->AddConquestGage(true, -delta / CO->ConquestGageMaxTime);
	//	CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
	//	break;
	//case EConquestAreaState::OwnerRed:
	//	// ������ �����̸� ������ �ٻ��ְ� ��������.
	//	if (CO->GetConquestGage(false) > 0.0f)
	//		CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
	//	else
	//		CO->AddConquestGage(true, delta / CO->ConquestGageMaxTime);
	//	break;
	//case EConquestAreaState::OwnerBlue:
	//	// ����� �����̸� ������� �ٻ��ְ� �������.
	//	if (CO->GetConquestGage(true) > 0.0f)
	//		CO->AddConquestGage(true, -delta / CO->ConquestGageMaxTime);
	//	else
	//		CO->AddConquestGage(false, delta / CO->ConquestGageMaxTime);
	//	break;
	//}	
}

//void RedAssultState::Tick(ABladeIINetControlObject* CO, float delta)
//{
//
//}
//
//void BlueAssultState::Tick(ABladeIINetControlObject* CO, float delta)
//{
//
//}

// ������ ������ ƽ
void RedState::Tick(ABladeIINetControlObject* CO, float delta)
{
	if (!CO->IsObjectUpdateable())
		return;

	if (IsActiveControlTutorial())
	{
		delta *= CO->GageRateForTutorial;
	}

	CO->AddControlPointGage(true, delta / CO->ControlPointGageMaxTime);

	// ������ ���� �����̸� ����(���� �̹�����)
	if (CO->GetControlPointGage(true) >= 1.0f)
		return;

	switch (CO->GetConquestAreaState())
	{
	case EConquestAreaState::Assault:
		// �ݵ����̸� �ϰ͵� ����
		break;
	case EConquestAreaState::None:
		// �ƹ��� ������ ����� ������ ����
		CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerRed:
		// ������ �����̸� ����� ������ ����
		CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerBlue:
		// ����� �����̸� ����� ������ ����
		CO->AddConquestGage(false, delta / CO->ConquestGageMaxTime);
		break;
	}
}

// ����� ������ ƽ
void BlueState::Tick(ABladeIINetControlObject* CO, float delta)
{
	if (!CO->IsObjectUpdateable())
		return;

	if (IsActiveControlTutorial())
	{
		delta *= CO->GageRateForTutorial;
	}

	CO->AddControlPointGage(false, delta / CO->ControlPointGageMaxTime);

	// ����� ���� �����̸� ����(���� �̹�����)
	if (CO->GetControlPointGage(false) >= 1.0f)
		return;

	switch (CO->GetConquestAreaState())
	{
	case EConquestAreaState::Assault:
		// �ݵ����̸� �ϰ͵� ����
		break;
	case EConquestAreaState::None:
		// �ƹ��� ������ ������ ������ ����
		CO->AddConquestGage(true, -delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerRed:
		// ������ �����̸� ������ ������ ����
		CO->AddConquestGage(true, delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerBlue:
		// ����� �����̸� ������ ������ ����
		CO->AddConquestGage(true, -delta / CO->ConquestGageMaxTime);
		break;
	}
}