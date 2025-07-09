#pragma once

#include "Components/CapsuleComponent.h"
#include "B2CharPartTouchComponent.generated.h"

UCLASS(ClassGroup = "BladeII", editinlinenew, hidecategories = (Object, LOD, Lighting, TextureStreaming), meta = (DisplayName = "Socket Touch Bounds", BlueprintSpawnableComponent))
class UB2CharPartTouchComponent : public UCapsuleComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName SocketNameToAttach;

	UPROPERTY(EditAnywhere)
	bool bUseBone;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseBone"))
	FName BoneNameToAttach;

	UPROPERTY(EditAnywhere)
	int32 TouchPriority;

	UPROPERTY(EditAnywhere)
	TArray<class UAnimSequenceBase*> TouchAnimInstances;

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void SetEnable(bool bSet);
	class UAnimSequenceBase* GetInteractionAnim();

private:
	void AttachOwnerSkeletalMesh(bool bIgnoreOldLocation = false);
	void TraceBone();

	bool bEnable;
};