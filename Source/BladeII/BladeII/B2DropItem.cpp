#include "B2DropItem.h"
#include "Components/AudioComponent.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
//#include "B2FloatingGoldAcq.h"
//#include "B2UIManager_InGameHUDPlayer.h"
#include "Event.h"
//#include "B2StageManager.h"


#include "BladeIIUtil.h"
#include "EngineMinimal.h"
// Sets default values
AB2DropBase::AB2DropBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereCollisionComp"));
	CollisionComp->InitSphereRadius(5.f);
	CollisionComp->AlwaysLoadOnClient = true;
	CollisionComp->AlwaysLoadOnServer = true;
	//CollisionComp->bCheckAsyncSceneOnMove = true;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap); // Not like to be blocked..
	RootComponent = CollisionComp;

	DropItemMoveComp = ObjectInitializer.CreateDefaultSubobject<UB2DropItemMovementComponent>(this, TEXT("DropItemMovementComp"));
	DropItemMoveComp->UpdatedComponent = CollisionComp;
	DropItemMoveComp->InitialSpeed = 0.0f;
	DropItemMoveComp->MaxSpeed = 1500.0f;
	DropItemMoveComp->bRotationFollowsVelocity = true;
	DropItemMoveComp->ProjectileGravityScale = 0.0f;

	// Those homing properties can be changed if subclass is not like auto-sucked.
	DropItemMoveComp->bIsHomingProjectile = true;
	DropItemMoveComp->HomingAccelerationMagnitude = 10000.0f;

	AudioComp = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("AudioComp"));
	AudioComp->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	AudioComp->bAutoActivate = false;

	CachedSuckingPSComp = NULL;

	MainBodyPS = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("MainBodyPS"));
	MainBodyPS->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	SetReplicatingMovement(true);

	PlaySoundDelay = 1.0f;
	PostSuckLifeSpan = 5.0f;
	bSoundPlayedAtLeastOnce = false;

	SpawnLocationOffsetZ = 0.0f;
	StayTime = 1.0f;
	SpawnMinRandomDistance = 0.f;
	SpawnMaxRandomDistance = 0.f;
	SpawningTime = 0.0f;

	SpawnHeight = 0.f;
	QTEStayTime = 0.f;
	bFindFloor = false;

	//m_Sucker = nullptr;
}

// Called when the game starts or when spawned
void AB2DropBase::BeginPlay()
{
	Super::BeginPlay();

	// 이거 이전에 OverrideByItemInfo 가 되어 있어야 할 건데 만일 안된다면 아래 부분을 좀 떼어서 직접 불러주거나 낄낄

	// It is assumed to be spawned right at the dead character's location. Give an offset to it.
	SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, SpawnLocationOffsetZ));

	// 스폰 연출이 끝나기 전까지는 먹지 못하게 컬리젼 막음.
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (SpawningTime > 0.f)
		GetWorldTimerManager().SetTimer(TimeToFinishSpawningHandle, this, &AB2DropBase::TimeToFinishSpawning, SpawningTime, false);
	else
		TimeToFinishSpawning();

	float RandomDist = FMath::FRandRange(SpawnMinRandomDistance, SpawnMaxRandomDistance);
	FRotationMatrix RotMat(FRotator(0.f, GetActorRotation().Yaw, 0.f));
	SetActorRotation(FRotator(0.f, 0.f, 0.f));
	FVector SpawnTargetLocation = GetActorLocation() + FVector(RotMat.TransformPosition(FVector(RandomDist, 0.f, 0.f)));
	GetMovementComp()->SetSpawningLocationInfo(SpawningTime, SpawnTargetLocation, SpawnHeight, bFindFloor);

	if (!FMath::IsNearlyZero(QTEStayTime))
	{
		StayTime = QTEStayTime;
	}

	if (StayTime + SpawningTime > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimeToGetSuckedTimerHandle, this, &AB2DropBase::TimeToGetSuckedCB, StayTime + SpawningTime, false);
	}
	else
	{
		TimeToGetSuckedCB(); // 아님 바로 빨리기 시작.
	}

	if (PlaySoundDelay > 0.0f)
	{
		GetWorldTimerManager().SetTimer(PlaySoundTimerHandle, this, &AB2DropBase::PlayDropItemSound, PlaySoundDelay, false);
	}
	else
	{
		PlayDropItemSound(); // 아님 바로 플레이
	}
}

// Called every frame
void AB2DropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPlayerSucksMe();
}

void AB2DropBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//if (ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor))
	//{
	//	PlayerSucksMe(B2Player);
	//}
}

// 원래 로컬플레이어 쫒는데 써커따로 셋팅해주면 개를 쫒음
//void AB2DropBase::SetSucker(ABladeIIPlayer* pSucker)
//{
//	m_Sucker = pSucker;
//}

void AB2DropBase::PlayDropItemSound()
{
	GetWorldTimerManager().ClearTimer(PlaySoundTimerHandle);

	if (AudioComp)
	{
		AudioComp->Play();
		bSoundPlayedAtLeastOnce = true; // 한번 플레이 되었음을 표시
	}
}

void AB2DropBase::TimeToGetSuckedCB()
{
	// Now, let's go get sucked by the wicked player!	

	//// 로컬플레이어가 아니어도 먹을수있을듯(점령전 샘에서 나오는구슬)
	//// +써커 외부에서 설정해줄수도 있음
	//if (!m_Sucker)
	//{
	//	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetNearestPlayerCharacter(this, GetActorLocation()));

	//	if (B2Player)
	//		m_Sucker = B2Player;
	//}

	//if (m_Sucker)
	//	GetMovementComp()->StartHommingToSucker(m_Sucker);

	GetWorldTimerManager().ClearTimer(TimeToGetSuckedTimerHandle); // Not sure if really required.
}

void AB2DropBase::TimeToFinishSpawning()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

//void AB2DropBase::PlayerSucksMe(ABladeIIPlayer* InSucker)
//{
//	// 들어온게 멤버변수 써커랑 다르면 리턴
//	// 점령전에서 타플레이어에도 충돌할수 있다.
//	if (InSucker == NULL || m_Sucker != InSucker)
//	{
//		return;
//	}
//
//	if (SuckingPS)
//	{
//		CachedSuckingPSComp = UGameplayStatics::SpawnEmitterAttached(SuckingPS, InSucker->GetRootComponent());
//
//		// 슬로우, 빙결때도 습득 이펙트 나오게
//		CachedSuckingPSComp->bUseOwnerCustomTimeDilation = false;
//	}
//
//	// Notify event both to myself and player.
//	OnPlayerSuckedMe(InSucker);
//	InSucker->SuckedDropItem(this);
//
//	if (AudioComp && bSoundPlayedAtLeastOnce == false && PlaySoundDelay > 0.0f)
//	{
//		// 뭔가 사운드 플레이가 되어야 하는데 플레이어와 너무 일찍 접촉한 경우일 것. 걍 바로 플레이
//		PlayDropItemSound();
//	}
//
//	if (PostSuckLifeSpan > 0.0f)
//	{
//		// 이렇게 잠시 살려두는 건 일단 끝나고도 빨리는 여운이 남아야 하는 사운드 때문.
//		this->SetActorHiddenInGame(true);
//		this->GetCollisionComp()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//		this->SetLifeSpan(PostSuckLifeSpan);
//	}
//	else
//	{
//		GetWorld()->DestroyActor(this);
//	}
//}

void AB2DropBase::CheckPlayerSucksMe()
{
	//if (m_Sucker && m_Sucker->IsPendingKill())
	//{
	//	Destroy();
	//}

	//if (m_Sucker && this->GetCollisionComp()->GetCollisionEnabled() != ECollisionEnabled::NoCollision)
	//{
	//	// 플레이어 출동체 off하는 케이스가 있어서 거리로도 체크
	//	if ((GetActorLocation() - m_Sucker->GetActorLocation()).Size() <= 100.0f)
	//	{
	//		PlayerSucksMe(m_Sucker);
	//	}
	//}
}

void AB2DropBase::OnPlayerSuckedMe(class ABladeIIPlayer* InSucker)
{

}

//void AB2DropItem::OnPlayerSuckedMe(class ABladeIIPlayer* InSucker)
//{
//	// Do anything necessary.
//	if (SignificantData.ItemRefID == FItemRefIDHelper::GetGoodsID_Gold())
//		AB2StageManager::GetStageRewardManager().AcquireGold(SignificantData.ConsumingAmount);
//	else
//		AB2StageManager::GetStageRewardManager().AcquireItem(SignificantData.ItemRefID);
//
//	//OnPlayerSuckedMeBP(InSucker); // And.. for blueprint too.
//}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	UB2DropItemMovementComponent
//

UB2DropItemMovementComponent::UB2DropItemMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Acceleration = CachedLocationTarget = FVector::ZeroVector;
}

void UB2DropItemMovementComponent::FinishSpawning()
{
	GetOwner()->SetActorLocation(CachedLocationTarget);
	Velocity = Acceleration = FVector::ZeroVector;

	GetOwner()->GetWorldTimerManager().ClearTimer(TimeToFinishSpawningTimerHandle); // Not sure if really required.
}

//void UB2DropItemMovementComponent::StartHommingToSucker(ABladeIIPlayer* Player)
//{
//	if (!Player)
//		return;
//
//	HomingTargetComponent = Player->GetRootComponent();
//	bIsHomingProjectile = true;
//}

FVector UB2DropItemMovementComponent::ComputeVelocity(FVector InitialVelocity, float DeltaTime) const
{
	if (bIsHomingProjectile && HomingTargetComponent.IsValid())
	{
		return ComputeHomingAcceleration(InitialVelocity, DeltaTime);
	}

	return InitialVelocity + Acceleration * DeltaTime;
}

void UB2DropItemMovementComponent::SetAcceleration(const FVector& InitialVelocity, float SpawningTime, const FVector& LocationTarget)
{

	if (!GetOwner())
		return;

	//s=vt+at^2/2
	//s-vt=at^2/2
	//2(s-vt)/t^2=a
	Acceleration = 2 * (LocationTarget - GetOwner()->GetActorLocation() - InitialVelocity * SpawningTime) / FMath::Square(SpawningTime);
}

void UB2DropItemMovementComponent::SetSpawningLocationInfo(float SpawningTime, const FVector& LocationTarget, float HighestHeight, bool bFindFloorLocation)
{//초기 속도와 목표 위치 설정.

	if (!GetOwner())
		return;

	CachedLocationTarget = LocationTarget;

	if (bFindFloorLocation)
	{
		FHitResult Hit;
		FVector Start = LocationTarget;
		FVector End = LocationTarget + FVector(0.f, 0.f, -1200.f);

		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		ABladeIIPlayer* LocalB2Player = PC ? Cast<ABladeIIPlayer>(PC->GetCharacter()) : nullptr;

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, FCollisionQueryParams(NAME_None, false, LocalB2Player));

		if (!Hit.bBlockingHit)
		{//바닥을 못찾았다면 지형 밑일 수도 있다.
			Start.Z += 1200.f;
			End.Z += 1200.f;

			GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, FCollisionQueryParams(NAME_None, false, LocalB2Player));
		}

		if (Hit.bBlockingHit)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor->IsA(ABladeIICharacter::StaticClass()))
				Hit.Location.Z -= 2.f * HitActor->GetSimpleCollisionHalfHeight();

			CachedLocationTarget = Hit.Location + FVector(0.f, 0.f, 10.f);
		}
	}

	Velocity = (CachedLocationTarget - GetOwner()->GetActorLocation()) / SpawningTime;
	Velocity.Z += 2.f * HighestHeight / SpawningTime;

	SetAcceleration(Velocity, SpawningTime, CachedLocationTarget);

	if (SpawningTime > 0.f)
		GetOwner()->GetWorldTimerManager().SetTimer(TimeToFinishSpawningTimerHandle, this, &UB2DropItemMovementComponent::FinishSpawning, SpawningTime, false);
	else
		FinishSpawning();
}