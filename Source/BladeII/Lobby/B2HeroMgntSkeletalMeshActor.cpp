
#include "B2HeroMgntSkeletalMeshActor.h"
//#include "BladeII.h"

AB2HeroMgntSkeletalMeshActor::AB2HeroMgntSkeletalMeshActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZoomOffsetZ = 0.f;
	CameraNearInventory = 200.f;
	CameraNearCollection = 310.f;
}

void AB2HeroMgntSkeletalMeshActor::BeginPlay()
{
	Super::BeginPlay();

	if (SelectedAnimAsset_HeroManagement == nullptr)
		SelectedAnimAsset_HeroManagement = SelectedAnimAsset;
}

void AB2HeroMgntSkeletalMeshActor::PlayEquipAnimation(EItemEquipPlace EquippedPlace)
{
	for (auto& EquipAnimInfo : EquippedAnimations)
	{
		if (EquipAnimInfo.EquipPlace == EquippedPlace)
		{
			PlayAnimation(EquipAnimInfo.EquippedAnimAsset);
			bPlayingActionAnim = bActionAnimTriggered = true;
			
			break;
		}
	}
}

void AB2HeroMgntSkeletalMeshActor::PlaySelectAnimation(ELobbyScene CurrentLobbyScene, bool bInSkipAnimNotifiers, bool bInSkipSoundNotifiersOnly)
{
	switch (CurrentLobbyScene)
	{
	case ELobbyScene::ELobbyScene_HeroManagement:
		PlayAnimation(SelectedAnimAsset_HeroManagement, bInSkipAnimNotifiers, bInSkipSoundNotifiersOnly);
		break;
	default:
		PlayAnimation(SelectedAnimAsset, bInSkipAnimNotifiers, bInSkipSoundNotifiersOnly);
	}	

	bPlayingActionAnim = bActionAnimTriggered = true;
}

bool AB2HeroMgntSkeletalMeshActor::IsPlayingSelectedAnim()
{
	return bPlayingActionAnim && 
		(GetCurrentAnimSequence() == SelectedAnimAsset_HeroManagement || GetCurrentAnimSequence() == SelectedAnimAsset);
}

void AB2HeroMgntSkeletalMeshActor::CreateEffect(const int32 Index)
{
	if (PCParticles.IsValidIndex(Index))
	{
		if(Index == 0)
			SuccessItemEnhancementEffect(Index);
	}
}

void AB2HeroMgntSkeletalMeshActor::SuccessItemEnhancementEffect(const int32 Index)
{
	//if (PCParticles.IsValidIndex(Index) && PCParticles[Index].PCParticle)
	//{
	//	const FVector EffectLocation = GetActorLocation() - PCParticles[Index].ParticleOffset;
	//	UGameplayStatics::SpawnEmitterAtLocation(this, PCParticles[Index].PCParticle, EffectLocation, GetActorRotation(), true, PCParticles[Index].ParticleSacle);
	//}
}
