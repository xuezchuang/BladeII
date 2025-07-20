
#include "B2AreaDamageActorBase.h"
//#include "BladeII.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "EngineGlobals.h"
//#include "BladeIICharacter.h"
#include "Engine/World.h"
//#include "Event.h"
#include "TimerManager.h"

FName AB2AreaDamageActorBase::OldFxParamCountTimeName = FName(TEXT("CountTime"));
FName AB2AreaDamageActorBase::OldFxParamRadiusName = FName(TEXT("Radius"));
FName AB2AreaDamageActorBase::OldFxParamFadeName = FName(TEXT("Fade"));

FName AB2AreaDamageActor::NewFxParamLifeTimeName = FName(TEXT("LifeTime"));
FName AB2AreaDamageActor::NewFxParamRadiusName = FName(TEXT("Radius"));
FName AB2AreaDamageActor::NewFxParamColorName = FName(TEXT("Color"));

// Sets default values
AB2AreaDamageActorBase::AB2AreaDamageActorBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerType = EInteractiveTriggeringType::InteractBySpawn;

	PSCComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent0"));
	RootComponent = PSCComponent;

	Damage = 0;
	DamageRadius = 500;
	InteractingTimeOverride = DamageDelay = 1.f;
	PeriodSeconds = DamageInterval = 0;
	LifeSpan = 0;
	FadeInOutTime = 0.5f;
	ElapsedTimeSinceActivate = 0.f;
	DamageFirstImpactTime = 0.f;
	bDamageFirstImpactTimeTriggeredTimer = false;

	WarningFxScale = FVector(1.f, 1.f, 1.f);

	bCancelCastAreaDamage = false;
}

//AB2AreaDamageActorBase* AB2AreaDamageActorBase::SpawnDamageAreaActor(
//	TSubclassOf<AB2AreaDamageActorBase> Class,
//	ABladeIICharacter* Owner,
//	const FDamageInfo& InDamageInfo,
//	bool bOverrideDamageInfo, const FVector& SpawnLocation, float DamageOfTick, float Radius, float DelayTime, float Interval, float Life, float FirstImpactTime,
//	class UParticleSystem* InWarningFX, class UParticleSystem* InActivateFX, const FLinearColor& Color, const FVector& InWarningFxScale)
//{
//	UWorld* World = Owner ? Owner->GetWorld() : NULL;
//	if (World)
//	{
//		FTransform SpawnTransform(Owner->GetActorRotation(), SpawnLocation);
//		FActorSpawnParameters SpawnParams;
//
//		SpawnParams.bDeferConstruction = true;
//
//		auto* DamageAreaActor = Cast<AB2AreaDamageActorBase>(World->SpawnActor(Class, &SpawnTransform, SpawnParams));
//
//		if (DamageAreaActor)
//		{
//			DamageAreaActor->SetOwner(Owner);
//			DamageAreaActor->SetDamageAreaData(InDamageInfo, Owner, bOverrideDamageInfo, DamageOfTick, Radius, DelayTime, Interval, Life, FirstImpactTime);
//			if (InWarningFX && InActivateFX)
//				DamageAreaActor->SetDamageAreaFXData(InWarningFX, InActivateFX, Color, InWarningFxScale);
//
//			DamageAreaActor->FinishSpawning(SpawnTransform);
//
//			return DamageAreaActor;
//		}
//	}
//
//	return NULL;
//}

//void AB2AreaDamageActorBase::SetDamageAreaData(const FDamageInfo& InDamageInfo, const ABladeIICharacter* B2Char, bool bOverrideDamageInfo, float DamageOfTick, float Radius, float DelayTime, float Interval, float Life, float FirstImpactTime)
//{
//	if (bOverrideDamageInfo)
//	{
//		DamageInfo.SetDamageInfo(&InDamageInfo, B2Char);
//	}
//	else
//	{
//		DamageInfo.SetDamageInfo(NULL, B2Char);
//	}
//
//	Damage = DamageOfTick;
//	DamageRadius = Radius;
//	InteractingTimeOverride = DamageDelay = DelayTime;
//	PeriodSeconds = DamageInterval = Interval;
//
//	if (Life == -1.0f)
//		LifeSpan = Life;
//	else
//		LifeSpan = FMath::Max<float>(FMath::Max<float>(1.f, FadeInOutTime * 2.f), Life);
//	if (FirstImpactTime >= 0.f)
//		DamageFirstImpactTime = FirstImpactTime;
//	bDamageFirstImpactTimeTriggeredTimer = DamageFirstImpactTime == 0.f;
//}

void AB2AreaDamageActorBase::SetDamageAreaFXData(class UParticleSystem* InWarningFX, class UParticleSystem* InActivateFX, const FLinearColor& Color, const FVector& InWarningFxScale)
{
	WarningFX = InWarningFX;
	ActivatingFX = InActivateFX;

	WarningFXColor = Color;
	WarningFxScale = InWarningFxScale;
}

void AB2AreaDamageActorBase::BeginPlay()
{
	Super::BeginPlay();

	Interact(NULL);
}

void AB2AreaDamageActorBase::BeginInteract(class AActor* InInteractingActor)
{
	if (InteractingTimeOverride > 0.f)
	{
		PSCComponent->SetTemplate(WarningFX);
		PSCComponent->SetFloatParameter(OldFxParamCountTimeName, InteractingTimeOverride);
		PSCComponent->SetVectorParameter(OldFxParamRadiusName, FVector(DamageRadius));
	}

	Super::BeginInteract(InInteractingActor);
}

void AB2AreaDamageActor::BeginInteract(class AActor* InInteractingActor)
{
	if (InteractingTimeOverride > 0.f)
	{
		PSCComponent->SetTemplate(WarningFX);

		PSCComponent->SetVectorParameter(NewFxParamColorName, FVector(WarningFXColor));
		PSCComponent->SetFloatParameter(NewFxParamLifeTimeName, InteractingTimeOverride);
		PSCComponent->SetVectorParameter(NewFxParamRadiusName, FVector(DamageRadius));

		PSCComponent->SetRelativeScale3D(WarningFxScale);
	}

	Super::BeginInteract(InInteractingActor);
}

void AB2AreaDamageActorBase::InteractAction()
{
	if (GetPhase() == EInteractivePhaseType::ReadyPhase && PSCComponent->Template != ActivatingFX)
	{
		PSCComponent->SetTemplate(ActivatingFX);
		PSCComponent->SetVectorParameter(OldFxParamRadiusName, FVector(DamageRadius));
		PSCComponent->SetRelativeScale3D(FVector(1.f, 1.f, 1.f)); // 이건 현재로선 스케일 파라미터가 없으니 1.0 으로 맞춰줌. 이걸 안해주면 앞서 WarningFxScale 넣은 게 유지될 것.
		ElapsedTimeSinceActivate = 0.f;
	}

	if (bDamageFirstImpactTimeTriggeredTimer)
	{
		TArray<AActor*> IgnoreActors;

		//UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), DamageRadius, UDamageType::StaticClass(), IgnoreActors, this, NULL, true, ECC_Visibility, 1.f, 0.f, false, 0.f);

		Super::InteractAction();
	}
	else
	{
		GetWorldTimerManager().SetTimer(DamageFirstImpactTimerHandle, this, &AB2AreaDamageActorBase::ToggleDamageImpact, DamageFirstImpactTime, false);
	}
}

void AB2AreaDamageActorBase::ToggleDamageImpact()
{
	bDamageFirstImpactTimeTriggeredTimer = true;
	InteractAction();
}

const void* AB2AreaDamageActorBase::GetCustomDamageInfo()
{
	return &DamageInfo;
}

void AB2AreaDamageActorBase::Tick(float DeltaSeconds)
{
	if (GetPhase() == EInteractivePhaseType::InteractingPhase && LifeSpan > 0.f && PSCComponent)
	{
		ElapsedTimeSinceActivate += DeltaSeconds;

		if (ElapsedTimeSinceActivate < FadeInOutTime)
		{//Fading in
			PSCComponent->SetFloatParameter(OldFxParamFadeName, ElapsedTimeSinceActivate / FadeInOutTime);
		}
		else if (ElapsedTimeSinceActivate < 0 || ElapsedTimeSinceActivate > LifeSpan)
		{
			PSCComponent->SetFloatParameter(OldFxParamFadeName, 0.f);

			// 취소시그널 왔구 시간 다되었으면 제거
			if (bCancelCastAreaDamage)
				EndInteract();
		}
		else if ((LifeSpan - ElapsedTimeSinceActivate) < FadeInOutTime)
		{//Fading out
			PSCComponent->SetFloatParameter(OldFxParamFadeName, (LifeSpan - ElapsedTimeSinceActivate) / FadeInOutTime);
		}
		else
		{
			PSCComponent->SetFloatParameter(OldFxParamFadeName, 1.f);
		}
	}
}

void AB2AreaDamageActorBase::EndInteract()
{
	Super::EndInteract();

	Destroy();
}

void AB2AreaDamageActorBase::BeginDestroy()
{
	//CancelCastAreaDamageClass<ABladeIICharacter*>::GetInstance().Unsubscribe(EventOwnerCancelAreaDamageSubscribeKey);
	Super::BeginDestroy();
}

void AB2AreaDamageActorBase::Destroyed()
{
	//CancelCastAreaDamageClass<ABladeIICharacter*>::GetInstance().Unsubscribe(EventOwnerCancelAreaDamageSubscribeKey);
	Super::Destroyed();
}

void AB2AreaDamageActorBase::CancelCastAreaDamage_CallBack()
{
	// LifeSpan이 -1이면 오너 케릭터가 죽는 경로로만 제거된다.
	if (LifeSpan == -1)
	{
		EndInteract();
		return;
	}

	bCancelCastAreaDamage = true;

	if (GetPhase() == EInteractivePhaseType::InteractingPhase)
	{
		Damage = 0;

		if (ElapsedTimeSinceActivate < FadeInOutTime)
			ElapsedTimeSinceActivate = LifeSpan - ElapsedTimeSinceActivate;
		else if (ElapsedTimeSinceActivate < LifeSpan - FadeInOutTime)
			ElapsedTimeSinceActivate = LifeSpan - FadeInOutTime;
	}
	else
	{
		EndInteract();
	}
}

