﻿
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

	// 痢飞瘤 牢酒眶 胆府霸捞飘(纳腐磐)
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

	// 傲 平俊辑 眉农窃. 面倒 甸绢咳 唱皑栏肺 眉农窍聪 救嘎绰版快啊 积辫.
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
	// 龋胶飘 酒聪搁 府畔!
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	check(pGM); /** 捞巴阑 荤侩窍绰 霸烙葛靛啊 沥秦廉 乐澜 */
	if (pGM && pGM->GetPeerType() == PEER_SUPER)
	{
		// 焊辰芭 览翠 救吭栏搁 府畔
		if (bWaitSendChangeControlState)
			return;

		bWaitSendChangeControlState = true;

		pGM->SendChangeControlState(NewStateKey, m_fControlPointGageRed, m_fControlPointGageBlue, true);
	}

	// 夯牢档 罐酒辑 贸府秦焊磊.
	//ApplyChangeState(NewStateKey);
}

void ABladeIINetControlObject::ApplyChangeState(uint8 NewStateKey, float fControlPointGageRed, float fControlPointGageBlue, bool bResetConquestGage)
{
	bWaitSendChangeControlState = false;

	if (bResetConquestGage)
	{
		// 痢飞瘤 霸捞瘤 檬扁拳(痢飞 肯丰饶绰 痢飞评 霸捞瘤绰 狼固绝澜)
		m_fConquestGageRed = 0.0f;
		m_fConquestGageBlue = 0.0f;
	}

	IState** NewState = StateMaps.Find(NewStateKey);

	if (NewState)
	{
		ControledState = *NewState;

		m_fControlPointGageRed = fControlPointGageRed;
		m_fControlPointGageBlue = fControlPointGageBlue;

		// 捞亥飘 眠啊. 函茄 惑怕 UI俊 傈崔
		UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
		if (pUI)
		{
			pUI->SetControlObjectState((*NewState)->GetState(), m_fConquestGageRed, m_fConquestGageBlue);
			pUI->SetControlPoint(true, m_fControlPointGageRed);
			pUI->SetControlPoint(false, m_fControlPointGageBlue);
		}
	}

	// 眠啊矫埃 檬扁拳
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
	// 劝己拳 傈捞搁 府畔
	if (!bActiveControlObject)
		return;

	// 龋胶飘 酒聪搁 府畔!
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

	// QTE鸥搁 面倒眉啊 绝澜.. 芭府肺 眉农
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

	// 劝己拳 矫淖栏搁 坷滚乏 茄锅 眉农
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
	// 眠啊矫埃浚 痢飞瘤 霸捞瘤 憋捞瘤 臼绰促.
	if (fAmount <= 0 && IsinBonusTime())
		return;

	if (bRed)
	{
		bool bUpdateUI = m_fConquestGageRed != 0.0f;
		m_fConquestGageRed += fAmount;
		m_fConquestGageRed = FMath::Clamp(m_fConquestGageRed, 0.0f, 1.0f);

		if (bUpdateUI)
		{
			// 捞亥飘 眠啊. 函茄 惑怕 UI俊 傈崔
			UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
			if (pUI)
				pUI->SetConquestGageInfo(bRed, m_fConquestGageRed);
		}

		//痢飞 己傍
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
			// 捞亥飘 眠啊. 函茄 惑怕 UI俊 傈崔
			UB2UIControlBattleSub* pUI = UB2UIManager::GetInstance()->GetUI<UB2UIControlBattleSub>(UIFName::ControlBattleSub);
			if (pUI)
				pUI->SetConquestGageInfo(bRed, m_fConquestGageBlue);
		}

		//痢飞 己傍
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

		// 器牢飘啊 眠啊矫埃 扁霖摹焊促 臭阑锭
		if (m_fControlPointGageRed > LimitControlPointForBonusTimeRed)
		{
			if (GetConquestAreaState() == EConquestAreaState::Assault || GetConquestAreaState() == EConquestAreaState::OwnerBlue)
			{
				// 拜倒吝捞搁 眠啊矫埃!
				// 惑措评 痢飞吝捞绢档 眠啊矫埃!
				SetBonusTime(ETeamType::ETT_Red);
				m_fControlPointGageRed = LimitControlPointForBonusTimeRed;
			}
			else
			{
				// 拜倒吝 酒聪搁 弊成 棵覆. 眠啊矫埃 扁霖档 棵覆.
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

		// 器牢飘啊 眠啊矫埃 扁霖摹焊促 臭阑锭
		if (m_fControlPointGageBlue > LimitControlPointForBonusTimeBlue)
		{
			if (GetConquestAreaState() == EConquestAreaState::Assault || GetConquestAreaState() == EConquestAreaState::OwnerRed)
			{
				// 拜倒吝捞搁 眠啊矫埃!
				// 惑措评 痢飞吝捞绢档 眠啊矫埃!
				SetBonusTime(ETeamType::ETT_Blue);
				m_fControlPointGageBlue = LimitControlPointForBonusTimeBlue;
			}
			else
			{
				// 拜倒吝 酒聪搁 弊成 棵覆. 眠啊矫埃 扁霖档 棵覆.
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
			// 版扁 辆丰 规价
			
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

	// 龋胶飘绰 霸烙葛靛 烹秦辑 规价!
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	if (pGM->GetPeerType() == PEER_SUPER)
		pGM->SendChangeConquestAreaState(int32(NewState), m_fConquestGageRed, m_fConquestGageBlue);

	// 夯牢档 菩哦罐酒辑 贸府秦航.
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

// 泅犁沥焊 茄锅 规价!
void ABladeIINetControlObject::BroadCastCurrentState()
{
	// 痢飞瘤 惑怕
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
	
	// dTime栏肺 霸捞瘤 哗临锭 0捞窍肺 冻绢柳蔼篮 公矫茄促.(蝶肺 馆措祈 霸捞瘤俊 歹秦林瘤 臼澜)

	//if (IsActiveControlTutorial())
	//{
	//	delta *= CO->GageRateForTutorial;
	//}

	//switch (CO->GetConquestAreaState())
	//{
	//case EConquestAreaState::Assault:
	//	// 拜倒吝捞搁 鞠巴滴 救窃
	//	break;
	//case EConquestAreaState::None:
	//	// 酒公档 绝栏搁 笛促 哗滚赴促.
	//	CO->AddConquestGage(true, -delta / CO->ConquestGageMaxTime);
	//	CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
	//	break;
	//case EConquestAreaState::OwnerRed:
	//	// 饭靛率 痢飞捞搁 喉风何磐 促哗林绊 饭靛刘啊.
	//	if (CO->GetConquestGage(false) > 0.0f)
	//		CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
	//	else
	//		CO->AddConquestGage(true, delta / CO->ConquestGageMaxTime);
	//	break;
	//case EConquestAreaState::OwnerBlue:
	//	// 喉风率 痢飞捞搁 饭靛何磐 促哗林绊 喉风刘啊.
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

// 饭靛评 痢飞吝 平
void RedState::Tick(ABladeIINetControlObject* CO, float delta)
{
	if (!CO->IsObjectUpdateable())
		return;

	if (IsActiveControlTutorial())
	{
		delta *= CO->GageRateForTutorial;
	}

	CO->AddControlPointGage(true, delta / CO->ControlPointGageMaxTime);

	// 饭靛评 痢荐 父顶捞搁 府畔(霸烙 捞固辆丰)
	if (CO->GetControlPointGage(true) >= 1.0f)
		return;

	switch (CO->GetConquestAreaState())
	{
	case EConquestAreaState::Assault:
		// 拜倒吝捞搁 鞠巴滴 救窃
		break;
	case EConquestAreaState::None:
		// 酒公档 绝栏搁 喉风评 霸捞瘤 皑家
		CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerRed:
		// 饭靛率 痢飞捞搁 喉风评 霸捞瘤 皑家
		CO->AddConquestGage(false, -delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerBlue:
		// 喉风率 痢飞捞搁 喉风评 霸捞瘤 刘啊
		CO->AddConquestGage(false, delta / CO->ConquestGageMaxTime);
		break;
	}
}

// 喉风评 痢飞吝 平
void BlueState::Tick(ABladeIINetControlObject* CO, float delta)
{
	if (!CO->IsObjectUpdateable())
		return;

	if (IsActiveControlTutorial())
	{
		delta *= CO->GageRateForTutorial;
	}

	CO->AddControlPointGage(false, delta / CO->ControlPointGageMaxTime);

	// 喉风评 痢荐 父顶捞搁 府畔(霸烙 捞固辆丰)
	if (CO->GetControlPointGage(false) >= 1.0f)
		return;

	switch (CO->GetConquestAreaState())
	{
	case EConquestAreaState::Assault:
		// 拜倒吝捞搁 鞠巴滴 救窃
		break;
	case EConquestAreaState::None:
		// 酒公档 绝栏搁 饭靛评 霸捞瘤 皑家
		CO->AddConquestGage(true, -delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerRed:
		// 饭靛率 痢飞捞搁 饭靛评 霸捞瘤 刘啊
		CO->AddConquestGage(true, delta / CO->ConquestGageMaxTime);
		break;
	case EConquestAreaState::OwnerBlue:
		// 喉风率 痢飞捞搁 饭靛评 霸捞瘤 皑家
		CO->AddConquestGage(true, -delta / CO->ConquestGageMaxTime);
		break;
	}
}