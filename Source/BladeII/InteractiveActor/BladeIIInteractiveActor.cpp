#include "BladeIIInteractiveActor.h"
#include "Animation/AnimSingleNodeInstance.h"
//#include "BladeIIPlayer.h"
//#include "BladeIIUtil.h"
#include "TimerManager.h"
#include "Engine/CollisionProfile.h"
// NotifyHit()/TakeDamage()/BeginOverlap() 
// ￠
// Interact() -> BeginInteract() -> InteractAction() -> EndInteract()

// Sets default values
ABladeIIInteractiveActor::ABladeIIInteractiveActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 捞 咀磐绰 蝶肺 平阑 罐阑 鞘夸啊 绝促. 促父, 捞亥飘 贸府 规侥俊 蝶扼 官层具且瘤档 葛抚(立盟窍绊 乐绰 悼救 瓤苞啊 惯积秦具茄促电瘤 且 锭 - 辟单 备炼何磐 官层具且 淀.)
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

	// 措眉肺 捞繁 辆幅 局甸 shadow 啊 modulated shadow 漂己窍绊 嘎拱妨辑 弊促瘤 焊扁 救亮澜.
	// Modulated shadow 甫 措眉窍绰 宝满篮 荐窜捞 唱坷扁 傈鳖瘤绰 波敌促.
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
// ACountinuousInAreaInteractiveActor 漂沥 杭俘 救俊辑 瘤加利牢 瓤苞甫 林绰 惑龋累侩 咀磐
//

// BeginOverlap() -------------------------------------> EndOverlap()
// ￠                                                    ￠
// Interact() -> BeginInteract() Θ> InteractAction() Δ EndInteract()
//                               Ζ<Α<Α<Α<Α<Α<Α<Ε


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
		SetPhase(EInteractivePhaseType::FinishPhase); //PeriodSeconds啊 0捞窍扼绰 巴篮 茄锅父 馆览窍绊 场郴绰 巴阑 狼固茄促.

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
	//	TriggerType = EInteractiveTriggeringType::InteractByTriggerVolume; //亲惑 碍力茄促.
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
