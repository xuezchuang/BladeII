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
	// ���Ӹ�� ���ؼ� ȣ��Ʈ���� ��û�ϱ� �㰡������ SuperStartQTE() �̰ɷ� ����
	if (IsAlive() && IsQTEStartAble() &&GetNetStatus() == NET_MASTER && !bRequestStartQTE)
	{
		GetControlGameMode()->NetSendRequestQTE();
		bRequestStartQTE = true;
	}
}

void AB2ControlModePlayer::StopQTEMount()
{
	// ��Ƽ���̷� �����ٰ� �˸�. ���� ��Ƽ�ް� ����
	// �̰� �����Է����� ȣ��Ǵ°� �ƴϴϱ�.. �ߺ� üũ ����
	if (GetNetStatus() == NET_MASTER)
	{
		GetControlGameMode()->NetSendNotifyEndQTE();
	}
}

float AB2ControlModePlayer::GetUnitySkillPointDeltaSecondsRate(float DeltaSeconds)
{
	// �������� ������.
	return 0.0f;
}

void AB2ControlModePlayer::AddWeaponSkillPoint(float ToAddPoint)
{
	//171206_YJ ������������ ��ų�̳� �ݰݽ����� ����Ʈ ä������ �ʵ��� ��.
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

	// AI ��ȯ �׽�Ʈ Off �Ұ�
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

	// �����÷��̾ Ż��Ÿ�������� ������
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
