#include "BladeIIInteractiveActor.h"
#include "Animation/AnimSingleNodeInstance.h"
//#include "BladeIIPlayer.h"
//#include "BladeIIUtil.h"

// NotifyHit()/TakeDamage()/BeginOverlap() 
// ↓
// Interact() -> BeginInteract() -> InteractAction() -> EndInteract()

// Sets default values
ABladeIIInteractiveActor::ABladeIIInteractiveActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 이 액터는 따로 틱을 받을 필요가 없다. 다만, 이벤트 처리 방식에 따라 바꿔야할지도 모름(접촉하고 있는 동안 효과가 발생해야한다든지 할 때 - 근데 구조부터 바꿔야할 듯.)
	PrimaryActorTick.bCanEverTick = false; 

	TriggerType = EInteractiveTriggeringType::InteractByTriggerVolume;

	MaxHP = 10.0f;
	InteractingTimeOverride = 0.f;
	bOnlyPlayerCanInteract = true;
}

void ABladeIIInteractiveActor::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = TriggerType==EInteractiveTriggeringType::InteractByDamaged ? 0 : MaxHP;
	Phase = EInteractivePhaseType::ReadyPhase;

	// 대체로 이런 종류 애들 shadow 가 modulated shadow 특성하고 맞물려서 그다지 보기 안좋음.
	// Modulated shadow 를 대체하는 괜찮은 수단이 나오기 전까지는 꺼둔다.
	//TurnOffMeshComponentsDyamicShadowForModulated(this);
}

bool ABladeIIInteractiveActor::IsTriggering(AActor* Other, TEnumAsByte<EInteractiveTriggeringType::Type> InTriggerType)
{
	if (Phase != EInteractivePhaseType::ReadyPhase)
		return false;

	//if (bOnlyPlayerCanInteract && Cast<ABladeIIPlayer>(Other) == NULL)
	//	return false;

	return TriggerType == InTriggerType;
}

void ABladeIIInteractiveActor::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	if (IsTriggering(Other, EInteractiveTriggeringType::InteractByHit))
	{
		Interact(Other);
	}
}

float ABladeIIInteractiveActor::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	float ActualDamaged = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (IsTriggering(DamageCauser, EInteractiveTriggeringType::InteractByDamaged))
	{
		CurrentHP -= ActualDamaged;
		if (CurrentHP <= 0)
			Interact(DamageCauser);
	}

	return ActualDamaged;
}

void ABladeIIInteractiveActor::SetPhaseForce(TEnumAsByte<EInteractivePhaseType::Type> InPhase)
{
	Phase = InPhase;
}

void ABladeIIInteractiveActor::RecycleInteractiveActor()
{
	OnRecycleInteractiveActor();

	Phase = EInteractivePhaseType::ReadyPhase;

	switch (TriggerType)
	{
	case EInteractiveTriggeringType::InteractByDamaged:
		CurrentHP = MaxHP;
		break;
	case EInteractiveTriggeringType::InteractByTriggerVolume:
		ClearComponentOverlaps();
		UpdateOverlaps();
		break;
	}	
}

void ABladeIIInteractiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsTriggering(OtherActor, EInteractiveTriggeringType::InteractByTriggerVolume))
	{
		Interact(OtherActor);
	}
}

void ABladeIIInteractiveActor::Interact(class AActor* InInteractingActor)
{
	BeginInteract(InInteractingActor);
}

void ABladeIIInteractiveActor::BeginInteract(class AActor* InInteractingActor)
{
	OnBeginInteract();

	if (InteractingTimeOverride > 0.f)
		GetWorldTimerManager().SetTimer(EndInteractTimerHandle, this, &ABladeIIInteractiveActor::InteractAction, InteractingTimeOverride, false);
	else
		InteractAction();
}

void ABladeIIInteractiveActor::EndInteract()
{
	Phase = EInteractivePhaseType::FinishPhase;

	OnEndInteract();
}

void ABladeIIInteractiveActor::InteractAction()
{
	SetPhase(EInteractivePhaseType::InteractingPhase);

	EndInteract();
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// ACountinuousInAreaInteractiveActor 특정 볼륨 안에서 지속적인 효과를 주는 상호작용 액터
//

// BeginOverlap() -------------------------------------> EndOverlap()
// ↓                                                    ↓
// Interact() -> BeginInteract() ┬> InteractAction() ┐ EndInteract()
//                               └<─<─<─<─<─<─<┘


AContinuousInAreaInteractiveActor::AContinuousInAreaInteractiveActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TriggerType = EInteractiveTriggeringType::InteractByTriggerVolume;

	PrimaryActorTick.bCanEverTick = false;

	PeriodSeconds = 1.f;
}

void AContinuousInAreaInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
}

void AContinuousInAreaInteractiveActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (GetPhase() == EInteractivePhaseType::InteractingPhase && OtherActor == InteractingActor)
		EndInteract();
}

void AContinuousInAreaInteractiveActor::BeginInteract(class AActor* InInteractingActor)
{
	InteractingActor = InInteractingActor;
	
	Super::BeginInteract(InInteractingActor);
}

void AContinuousInAreaInteractiveActor::InteractAction()
{
	if (GetPhase() != EInteractivePhaseType::InteractingPhase)
	{
		if (PeriodSeconds > 0.f)
			GetWorldTimerManager().SetTimer(InteractPeriodTimerHandle, this, &AContinuousInAreaInteractiveActor::InteractAction, PeriodSeconds, true);

		SetPhase(EInteractivePhaseType::InteractingPhase);
		
		if (LifeSpan > 0)
			GetWorldTimerManager().SetTimer(InteractFinishTimerHandle, this, &AContinuousInAreaInteractiveActor::EndInteract, LifeSpan, true);
		else if(LifeSpan != -1.0f)
			EndInteract();
	}	
}

void AContinuousInAreaInteractiveActor::EndInteract()
{
	GetWorldTimerManager().ClearTimer(InteractPeriodTimerHandle);
	GetWorldTimerManager().ClearTimer(InteractFinishTimerHandle);

	if (PeriodSeconds > 0.f)
	{
		SetPhase(EInteractivePhaseType::ReadyPhase);

	}		
	else
		SetPhase(EInteractivePhaseType::FinishPhase); //PeriodSeconds가 0이하라는 것은 한번만 반응하고 끝내는 것을 의미한다.

	InteractingActor = NULL;
}

#if WITH_EDITOR
void AContinuousInAreaInteractiveActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_TriggerType = GET_MEMBER_NAME_CHECKED(AContinuousInAreaInteractiveActor, TriggerType);

	////FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	//FProperty* PropertyThatChanged = CastChecked<FProperty>(PropertyChangedEvent.Property);
	//FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	//if (PropertyName == Name_TriggerType)
	//{
	//	TriggerType = EInteractiveTriggeringType::InteractByTriggerVolume; //항상 강제한다.
	//}
}
#endif


ASkeletalMeshInteractiveActor::ASkeletalMeshInteractiveActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent0"));
	SkeletalMeshComponent->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	// check BaseEngine.ini for profile setup
	SkeletalMeshComponent->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = SkeletalMeshComponent;
}

void ASkeletalMeshInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASkeletalMeshInteractiveActor::BeginInteract(AActor* InInteractingActor)
{
	USkeletalMeshComponent* SKMeshComp = GetSkeletalMeshComponent();
	if (SKMeshComp && InteractingAnim)
	{
		SKMeshComp->PlayAnimation(InteractingAnim, false);
		SKMeshComp->SetPosition(0);
		UAnimSingleNodeInstance* AnimInstance = SKMeshComp->GetSingleNodeInstance();

		if (InteractingTimeOverride == 0.f)
		{
			InteractingTimeOverride = AnimInstance ? AnimInstance->GetLength() : 0.f;
		}
	}

	Super::BeginInteract(InInteractingActor);
}
