#include "B2CharPartTouchComponent.h"
//#include "BladeII.h"


#include "B2LobbySkeletalMeshActor.h"
#include "B2LobbyGameMode.h"

UB2CharPartTouchComponent::UB2CharPartTouchComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetCollisionProfileName(TEXT("CharacterTouchComp"));

	PrimaryComponentTick.bCanEverTick = true;
}

void UB2CharPartTouchComponent::BeginPlay()
{
	Super::BeginPlay();

	AttachOwnerSkeletalMesh();
}

void UB2CharPartTouchComponent::SetEnable(bool bSet)
{
	if (bEnable == bSet)
		return;

	bEnable = bSet;
}

void UB2CharPartTouchComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bEnable)
		TraceBone();
}

void UB2CharPartTouchComponent::TraceBone()
{
	if (bUseBone)
	{
		auto* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetAttachParent());
		if (SkeletalMeshComponent)
		{
			this->SetRelativeLocationAndRotation(
				SkeletalMeshComponent->GetBoneLocation(BoneNameToAttach, EBoneSpaces::ComponentSpace),
				SkeletalMeshComponent->GetBoneQuaternion(BoneNameToAttach, EBoneSpaces::ComponentSpace));
		}
	}
}

void UB2CharPartTouchComponent::AttachOwnerSkeletalMesh(bool bIgnoreOldLocation)
{
	//auto* SkelMeshActor = Cast<AB2LobbySkeletalMeshActor>(GetOwner());
	//auto* ComponentToAttach = SkelMeshActor ? SkelMeshActor->GetSkeletalMeshComponent() : nullptr;

	//if (ComponentToAttach)
	//{
	//	if (bIgnoreOldLocation)
	//	{
	//		this->RelativeLocation = FVector::ZeroVector;
	//	}
	//	if (bUseBone)
	//	{
	//		AttachToComponent(ComponentToAttach, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//		TraceBone();
	//	}			
	//	else
	//		AttachToComponent(ComponentToAttach, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), SocketNameToAttach);
	//}
}

#if WITH_EDITOR
void UB2CharPartTouchComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const static FName SocketNameToAttachName("SocketNameToAttach");
	const static FName BoneNameToAttachName("BoneNameToAttach");

	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : FName();

	if (PropertyName == SocketNameToAttachName || PropertyName == BoneNameToAttachName)
	{
		AttachOwnerSkeletalMesh(true);
	}
}
#endif

class UAnimSequenceBase* UB2CharPartTouchComponent::GetInteractionAnim()
{
	int32 NumAnimInstances = TouchAnimInstances.Num();
	int32 RandIndex = NumAnimInstances > 0 ? FMath::Rand() % NumAnimInstances : INDEX_NONE;

	return TouchAnimInstances.IsValidIndex(RandIndex) ? TouchAnimInstances[RandIndex] : nullptr;
}