#include "BladeII.h"
#include "B2ControlModePlayer.h"

#include "Event.h"
#include "Packet.h"
#include "PacketMaker.h"
#include "BladeIIPlayerAIController.h"
#include "B2ControlGameMode.h"

AB2ControlModePlayer::AB2ControlModePlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyAIController = nullptr;
	bRequestStartQTE = false;
}

void AB2ControlModePlayer::BeginPlay()
{
	Super::BeginPlay();	
}

void AB2ControlModePlayer::Tick(float delta)
{
	Super::Tick(delta);
}

void AB2ControlModePlayer::StartQTE(bool bForce/* = false*/)
{
	// 게임모드 통해서 호스트한테 요청하구 허가받으면 SuperStartQTE() 이걸로 실행
	if (IsAlive() && IsQTEStartAble() &&GetNetStatus() == NET_MASTER && !bRequestStartQTE)
	{
		GetControlGameMode()->NetSendRequestQTE();
		bRequestStartQTE = true;
	}
}

void AB2ControlModePlayer::StopQTEMount()
{
	// 노티파이로 내린다고 알림. 나두 노티받고 내림
	// 이건 유저입력으로 호출되는게 아니니깐.. 중복 체크 안함
	if (GetNetStatus() == NET_MASTER)
	{
		GetControlGameMode()->NetSendNotifyEndQTE();
	}
}

float AB2ControlModePlayer::GetUnitySkillPointDeltaSecondsRate(float DeltaSeconds)
{
	// 점령전은 안찬다.
	return 0.0f;
}

void AB2ControlModePlayer::AddWeaponSkillPoint(float ToAddPoint)
{
	//171206_YJ 점령전에서는 스킬이나 반격썼을때 포인트 채워지지 않도록 함.
}


bool AB2ControlModePlayer::CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType)
{
	return Super::CanTakeDamage(DamageCauser, DamageEvent, DamageType)
		&& !bRequestStartQTE;
}

void AB2ControlModePlayer::MoveForward(float InValue)
{
	if (FMath::IsNearlyZero(InValue) == false)
	{
		GetControlGameMode()->TouchPressed();
	}

	Super::MoveForward(InValue);
}

void AB2ControlModePlayer::MoveRight(float InValue)
{
	if (FMath::IsNearlyZero(InValue) == false)
	{
		GetControlGameMode()->TouchPressed();
	}

	Super::MoveRight(InValue);
}

void AB2ControlModePlayer::PromoteNetStatus()
{
	if (GetNetStatus() == NET_MASTER && MyAIController)
		return;

	UnsubscribeEvents();

	//Role = ROLE_Authority;
	SetNetStatus(NET_AIMASTER);

	// AI 전환 테스트 Off 불가
	FActorSpawnParameters SpawnInfo;
	//SpawnInfo.Instigator = Instigator;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save player controllers into a map
	SpawnInfo.bDeferConstruction = true;

	MyAIController = GetWorld()->SpawnActor<ABladeIIPlayerAIController>(ABladeIIPlayerAIController::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
	BII_CHECK(MyAIController);
	UGameplayStatics::FinishSpawningActor(MyAIController, FTransform());

	MyAIController->SetTeamNum(GetTeamNum());
	MyAIController->Possess(this);
	MyAIController->BeginAutoPlay();

	// 나간플레이어가 탈것타고있으면 내려줌
	if (IsQTEMounting())
	{
		GetControlGameMode()->NetSendNotifyEndQTE(GetNetId());
	}
}

class AB2ControlGameMode* AB2ControlModePlayer::GetControlGameMode()
{
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(GetGameMode());

	check(pGM);

	return pGM;
}
