
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

	// 점령지 인아웃 델리게이트(케릭터)
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

	// 걍 틱에서 체크함. 충돌 들어옴 나감으로 체크하니 안맞는경우가 생김.
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
	// 호스트 아니면 리턴!
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	check(pGM); /** 이것을 사용하는 게임모드가 정해져 있음 */
	if (pGM && pGM->GetPeerType() == PEER_SUPER)
	{
		// 보낸거 응답 안왔으면 리턴
		if (bWaitSendChangeControlState)
			return;

		bWaitSendChangeControlState = true;

		pGM->SendChangeControlState(NewStateKey, m_fControlPointGageRed, m_fControlPointGageBlue, true);
	}

	// 본인도 받아서 처리해보자.
	//ApplyChangeState(NewStateKey);
}

void ABladeIINetControlObject::ApplyChangeState(uint8 NewStateKey, float fControlPointGageRed, float fControlPointGageBlue, bool bResetConquestGage)
{
	bWaitSendChangeControlState = false;

	if (bResetConquestGage)
	{
		// 점령지 게이지 초기화(점령 완료후는 점령팀 게이지는 의미없음)
		m_fConquestGageRed = 0.0f;
		m_fConquestGageBlue = 0.0f;
	}

	IState** NewState = StateMaps.Find(NewStateKey);

	if (NewState)
	{
		ControledState = *NewState;

		m_fControlPointGageRed = fControlPointGageRed;
		m_fControlPointGageBlue = fControlPointGageBlue;

		// 이벤트 추가. 변한 상태 UI에 전달
		UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
		if (pUI)
		{
			pUI->SetControlObjectState((*NewState)->GetState(), m_fConquestGageRed, m_fConquestGageBlue);
			pUI->SetControlPoint(true, m_fControlPointGageRed);
			pUI->SetControlPoint(false, m_fControlPointGageBlue);
		}
	}

	// 추가시간 초기화
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
	// 활성화 전이면 리턴
	if (!bActiveControlObject)
		return;

	// 호스트 아니면 리턴!
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

	// QTE타면 충돌체가 없음.. 거리로 체크
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

	// 활성화 시켰으면 오버랩 한번 체크
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
	// 추가시간엔 점령지 게이지 깍이지 않는다.
	if (fAmount <= 0 && IsinBonusTime())
		return;

	if (bRed)
	{
		bool bUpdateUI = m_fConquestGageRed != 0.0f;
		m_fConquestGageRed += fAmount;
		m_fConquestGageRed = FMath::Clamp(m_fConquestGageRed, 0.0f, 1.0f);

		if (bUpdateUI)
		{
			// 이벤트 추가. 변한 상태 UI에 전달
			UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
			if (pUI)
				pUI->SetConquestGageInfo(bRed, m_fConquestGageRed);
		}

		//점령 성공
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
			// 이벤트 추가. 변한 상태 UI에 전달
			UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
			if (pUI)
				pUI->SetConquestGageInfo(bRed, m_fConquestGageBlue);
		}

		//점령 성공
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

		// 포인트가 추가시간 기준치보다 높을때
		if (m_fControlPointGageRed > LimitControlPointForBonusTimeRed)
		{
			if (GetConquestAreaState() == EConquestAreaState::Assault || GetConquestAreaState() == EConquestAreaState::OwnerBlue)
			{
				// 격돌중이면 추가시간!
				// 상대팀 점령중이어도 추가시간!
				SetBonusTime(ETeamType::ETT_Red);
				m_fControlPointGageRed = LimitControlPointForBonusTimeRed;
			}
			else
			{
				// 격돌중 아니면 그냥 올림. 추가시간 기준도 올림.
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

		// 포인트가 추가시간 기준치보다 높을때
		if (m_fControlPointGageBlue > LimitControlPointForBonusTimeBlue)
		{
			if (GetConquestAreaState() == EConquestAreaState::Assault || GetConquestAreaState() == EConquestAreaState::OwnerRed)
			{
				// 격돌중이면 추가시간!
				// 상대팀 점령중이어도 추가시간!
				SetBonusTime(ETeamType::ETT_Blue);
				m_fControlPointGageBlue = LimitControlPointForBonusTimeBlue;
			}
			else
			{
				// 격돌중 아니면 그냥 올림. 추가시간 기준도 올림.
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
			// 경기 종료 방송
			
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

	// 호스트는 게임모드 통해서 방송!
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	if (pGM->GetPeerType() == PEER_SUPER)
		pGM->SendChangeConquestAreaState(int32(NewState), m_fConquestGageRed, m_fConquestGageBlue);

	// 본인도 패킷받아서 처리해봄.
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

// 현재정보 한번 방송!
void ABladeIINetControlObject::BroadCastCurrentState()
{
	// 점령지 상태
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
	
	// dTime으로 게이지 빼줄때 0이하로 떨어진값은 무시한다.(따로 반대편 게이지에 더해주지 않음)

	//if (IsActiveControlTutorial())
	//{
	//	delta *= CO->GageRateForTutorial;
	//}

	//switch (CO->GetConquestAreaState())
	//{
	//case EConquestAreaState::Assault:
	//	// 격돌중이면 암것두 안함
	//	break;
	//case EConquestAreaState::None:
	//	// 아무도 없으면 둘다 빼버린다.
	//	CO->AddConquestGage(true, -delta / CO->ConquestGageMaxTime);
	//	CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
	//	break;
	//case EConquestAreaState::OwnerRed:
	//	// 레드쪽 점령이면 블루부터 다빼주고 레드증가.
	//	if (CO->GetConquestGage(false) > 0.0f)
	//		CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
	//	else
	//		CO->AddConquestGage(true, delta / CO->ConquestGageMaxTime);
	//	break;
	//case EConquestAreaState::OwnerBlue:
	//	// 블루쪽 점령이면 레드부터 다빼주고 블루증가.
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

// 레드팀 점령중 틱
void RedState::Tick(ABladeIINetControlObject* CO, float delta)
{
	if (!CO->IsObjectUpdateable())
		return;

	if (IsActiveControlTutorial())
	{
		delta *= CO->GageRateForTutorial;
	}

	CO->AddControlPointGage(true, delta / CO->ControlPointGageMaxTime);

	// 레드팀 점수 만땅이면 리턴(게임 이미종료)
	if (CO->GetControlPointGage(true) >= 1.0f)
		return;

	switch (CO->GetConquestAreaState())
	{
	case EConquestAreaState::Assault:
		// 격돌중이면 암것두 안함
		break;
	case EConquestAreaState::None:
		// 아무도 없으면 블루팀 게이지 감소
		CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerRed:
		// 레드쪽 점령이면 블루팀 게이지 감소
		CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerBlue:
		// 블루쪽 점령이면 블루팀 게이지 증가
		CO->AddConquestGage(false, delta / CO->ConquestGageMaxTime);
		break;
	}
}

// 블루팀 점령중 틱
void BlueState::Tick(ABladeIINetControlObject* CO, float delta)
{
	if (!CO->IsObjectUpdateable())
		return;

	if (IsActiveControlTutorial())
	{
		delta *= CO->GageRateForTutorial;
	}

	CO->AddControlPointGage(false, delta / CO->ControlPointGageMaxTime);

	// 블루팀 점수 만땅이면 리턴(게임 이미종료)
	if (CO->GetControlPointGage(false) >= 1.0f)
		return;

	switch (CO->GetConquestAreaState())
	{
	case EConquestAreaState::Assault:
		// 격돌중이면 암것두 안함
		break;
	case EConquestAreaState::None:
		// 아무도 없으면 레드팀 게이지 감소
		CO->AddConquestGage(true, -delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerRed:
		// 레드쪽 점령이면 레드팀 게이지 증가
		CO->AddConquestGage(true, delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerBlue:
		// 블루쪽 점령이면 레드팀 게이지 감소
		CO->AddConquestGage(true, -delta / CO->ConquestGageMaxTime);
		break;
	}
}