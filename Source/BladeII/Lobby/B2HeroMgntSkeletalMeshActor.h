#pragma once

#include "B2LobbySkeletalMeshActorBase.h"
#include "B2HeroMgntSkeletalMeshActor.generated.h"

USTRUCT(BlueprintType)
struct FEquippedItemAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EItemEquipPlace				EquipPlace;
	UPROPERTY(EditAnywhere)
	class UAnimSequenceBase*	EquippedAnimAsset;
};

USTRUCT(BlueprintType)
struct FPCParticles
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UParticleSystem* PCParticle;

	UPROPERTY(EditAnywhere)
	FVector ParticleOffset;

	UPROPERTY(EditAnywhere)
	FVector ParticleSacle;

	FPCParticles() : PCParticle(nullptr), ParticleOffset(FVector::ZeroVector), ParticleSacle(FVector(1.0f, 1.0f, 1.0f)) {}
};


UCLASS()
class AB2HeroMgntSkeletalMeshActor : public AB2LobbySkeletalMeshActorBase
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TArray<FEquippedItemAnimation> EquippedAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	TArray<FPCParticles> PCParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	class UAnimSequenceBase* SelectedAnimAsset_HeroManagement;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	class UAnimSequenceBase* SelectedAnimAssetInStageInfo;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraNearInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraNearCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float ZoomOffsetZ;

	virtual void BeginPlay() override;

	void PlayEquipAnimation(EItemEquipPlace EquippedPlace);
	virtual void PlaySelectAnimation(ELobbyScene CurrentLobbyScene = ELobbyScene::ELobbyScene_CharacterIntro, bool bInSkipAnimNotifiers = false, bool bInSkipSoundNotifiersOnly = false) override;

	virtual bool IsPlayingSelectedAnim() override;

	void CreateEffect(const int32 Index);

private:
	void SuccessItemEnhancementEffect(const int32 Index);
};