#include "B2LobbySkeletalMeshActorBase.h"
//#include "BladeII.h"
#include "Animation/AnimInstance.h"

#include "BladeIIUtil.h"

/************************************************************************/
/* AB2UnitedLobbySKMeshActorBase
 AB2LobbySkeletalMeshActorBase ���Ŀ� ���������.
 ���Ŀ� �߰��� �ٸ� Ŭ������ ���� ���� ���� ���̰��� �θ� Ŭ����
 ���� ����� �� ���� �ְ�..
 */
/************************************************************************/

AB2UnitedLobbySKMeshActorBase::AB2UnitedLobbySKMeshActorBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

const float LookUpTime = 1.2f;

/************************************************************************/
/* AB2LobbySkeletalMeshActorBase
 ó�� ������� �� �̸� �״�� Lobby ������ SkeletalMeshActor Base ��������  
 ������ �÷��̾� ĳ���͸� ���� ���̽��� ��.
*/
/************************************************************************/

AB2LobbySkeletalMeshActorBase::AB2LobbySkeletalMeshActorBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	LookUpAlphaBlend = RestoreFromPauseBlend = RemainPauseLookUpTime = 0.f;

	PrimaryActorTick.bCanEverTick = true;

	bEnableInputRotate = true;
}

void AB2LobbySkeletalMeshActorBase::BeginPlay()
{
	Super::BeginPlay();

	if (AnimClass)
	{
		auto* Mesh = GetSkeletalMeshComponent();
		check(Mesh);

		Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		Mesh->SetAnimInstanceClass(AnimClass);		

		PrevLookAtTargetLocation = this->GetActorLocation() + this->GetActorRotation().Vector().GetSafeNormal() * 400.f;
	}
}

void AB2LobbySkeletalMeshActorBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RemainPauseLookUpTime = FMath::Max(RemainPauseLookUpTime - DeltaSeconds, 0.0f);
	if (RemainPauseLookUpTime == 0.f)
		RestoreFromPauseBlend = FMath::Min(RestoreFromPauseBlend + DeltaSeconds, LookUpTime);
	
	if (CheckLookAtEnable())
		LookUpAlphaBlend = FMath::Min(LookUpAlphaBlend + DeltaSeconds, LookUpTime);
	else
		LookUpAlphaBlend = FMath::Max(LookUpAlphaBlend - DeltaSeconds, 0.0f);		

	UpdateLookAtParameters();
}

void AB2LobbySkeletalMeshActorBase::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
	
	if (IsHidden() == bNewHidden) // ���� �������� ������ �̰� �Ҹ� ���� �־ �Ʒ� �߰� �ڵ���� Ȯ���� ������ �Ǿ��� ���� ����ǵ���
		return;

	auto* Mesh = GetSkeletalMeshComponent();
	check(Mesh);

	if (bNewHidden == false)
	{
		if (AnimClass)
		{
			Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			Mesh->SetAnimInstanceClass(AnimClass);
		}

		// Ư�� Hidden ���Ŀ� �ٽ� ���̵��� ���� �� Particle ���� �͵��� �Ⱥ��̴� ��찡 �־.. �̷��� �� �ִ� �� ����.
		EnsureActivateStaticallyAttachedPSCs(this);
	}
	else
	{
		Mesh->SetAnimInstanceClass(nullptr);
		bPlayingRotateAnim = bPlayingActionAnim = bActionAnimTriggered = bRotateAnimTriggered = false;
		LookUpAlphaBlend = 0.f;
		CurrentAnimSequence = nullptr;
	}
}

UAnimSequenceBase* AB2LobbySkeletalMeshActorBase::GetCurrentAnimSequence()
{
	return CurrentAnimSequence.Get();
}

void AB2LobbySkeletalMeshActorBase::PlayAnimation(UAnimSequenceBase* Anim, bool bInSkipAnimNotifiers, bool bInSkipSoundNotifiersOnly)
{
	CurrentAnimSequence = Anim;

	LookUpAlphaBlend = 0.f;

	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	//if (SKComp && SKComp->AnimScriptInstance)
	//{
	//	SKComp->AnimScriptInstance->bSkipAnimNotifiers = bInSkipAnimNotifiers;
	//	SKComp->AnimScriptInstance->bSkipSoundNotifiersOnly = bInSkipSoundNotifiersOnly;
	//}
}

void AB2LobbySkeletalMeshActorBase::PlaySelectAnimation(ELobbyScene CurrentLobbyScene, bool bInSkipAnimNotifiers, bool bInSkipSoundNotifiersOnly)
{
	PlayAnimation(SelectedAnimAsset, bInSkipAnimNotifiers, bInSkipSoundNotifiersOnly);

	bPlayingActionAnim = bActionAnimTriggered = true;	
}

bool AB2LobbySkeletalMeshActorBase::IsPlayingSelectedAnim()
{
	return bPlayingActionAnim && GetCurrentAnimSequence() == SelectedAnimAsset;
}

void AB2LobbySkeletalMeshActorBase::OnEndedActionAnim()
{
	bPlayingActionAnim = false;
}

void AB2LobbySkeletalMeshActorBase::PlayTurnAnimation(bool bLeftTurn, bool bInSkipAnimNotifiers, bool bInSkipSoundNotifiersOnly)
{
	if (!bEnableInputRotate && (bPlayingRotateAnim || bPlayingActionAnim || (bLeftTurn ? GetLeftRotateAnimAsset() : GetRightRotateAnimAsset()) == nullptr))
		return;

	PlayAnimation(bLeftTurn ? GetLeftRotateAnimAsset() : GetRightRotateAnimAsset(), bInSkipAnimNotifiers, bInSkipSoundNotifiersOnly);
	
	bLeftRotation = bLeftTurn;
	bPlayingRotateAnim = bRotateAnimTriggered = true;
	bEnableInputRotate = false;
}

void AB2LobbySkeletalMeshActorBase::OnStartedRotatationAnim()
{
	AddActorWorldRotation(FRotator(0.f, bLeftRotation ? -60.f : 60.f, 0.f));
}

void AB2LobbySkeletalMeshActorBase::OnEndedRotatationAnim()
{	
	if (!bRotateAnimTriggered)
	{
		bPlayingRotateAnim = false;
		bEnableInputRotate = true;
	}	
}

#include "Animation/AnimInstanceProxy.h"
bool AB2LobbySkeletalMeshActorBase::CheckLookAtEnable()
{
	if (!UseLookAt())
		return false;

	if (bPlayingActionAnim || bPlayingRotateAnim)
		return false;

	if (bCameraOutLookAtArea)
		return false;
	
	return !bIgnoreLookAtState;
}

float AB2LobbySkeletalMeshActorBase::GetLookUpBlend()
{
	return FMath::CubicInterp(0.f, 0.f, 1.f, 0.f, LookUpAlphaBlend / LookUpTime);
}

void AB2LobbySkeletalMeshActorBase::SetPauseLookAt(float PauseTime)
{
	RemainPauseLookUpTime = PauseTime;
	RestoreFromPauseBlend = 0.f;
}

FVector AB2LobbySkeletalMeshActorBase::GetLobbyAnimAdjustOffset()
{
	//Lobby Anim Ư�� ����
	//Lobby animation�� �Ϲ� animation���� root��ġ�� �ٸ���,
	//Lobby animation�� Bip001-Pelvis�� root�̰�
	//�Ϲ� animation�� RootBone�� root��.
	//�� ���������� ���� GetBoneLocation�� ��ġ�� ���� �������Ǵ� ��ġ�� ���̰� ����.

	return (GetSkeletalMeshComponent()->GetBoneLocation(TEXT("Bip001-Pelvis")) - GetSkeletalMeshComponent()->GetBoneLocation(TEXT("RootBone")));
}

FRotator AB2LobbySkeletalMeshActorBase::GetActorToCameraRotationActorSpace()
{
	return ActorToCameraRotationActorSpace;
}

FVector AB2LobbySkeletalMeshActorBase::GetLookAtTargetLocation()
{
	return LookAtTargetLocation;
}

void AB2LobbySkeletalMeshActorBase::UpdateLookAtParameters()
{
	//if (this->bHidden || RemainPauseLookUpTime > 0.f)
	//	return;

	//auto* CameraManager = UGameplayStatics::GetLocalPlayerCameraManager(GetWorld());
	//if (CameraManager == nullptr)
	//	return;

	//FVector CameraWorldLocation = CameraManager->GetTransform().TransformPosition(LookAtOffset);
	//bCameraOutLookAtArea = ((CameraWorldLocation - GetActorLocation()) | GetActorRotation().Vector()) < 0;

	//if (!CheckLookAtEnable())
	//{
	//	RestoreFromPauseBlend = 0.f;
	//	return;
	//}		

	//FVector WorldToBoneOffset = GetLobbyAnimAdjustOffset();		
	//	
	////TargetLocation
	//float Blend = FMath::Lerp(0.f, 1.f, RestoreFromPauseBlend / LookUpTime);
	//if (Blend == 1.f)
	//{		
	//	LookAtTargetLocation = CameraWorldLocation;
	//}
	//else if (Blend != 0.f)
	//{
	//	LookAtTargetLocation = FMath::Lerp(PrevLookAtTargetLocation, CameraWorldLocation, Blend);
	//}

	////ActorToCamera
	//ActorToCameraRotationActorSpace = GetActorRotation().UnrotateVector(LookAtTargetLocation - (GetSkeletalMeshComponent()->GetBoneLocation(TEXT("Bip001-head")) - WorldToBoneOffset)).Rotation();
	//
	////[@AKI, 171118] �ϻ��ڿ��� Floating ������ ���� �׳� �Ҽ��� Floor(����) ����.
	//// �� ƽ���� ĳ���� �ؼ� ������ �Ǳ�� ������ �ӵ����� Visualization�̶� �̷��� ��
	//ActorToCameraRotationActorSpace.Pitch = static_cast<float>(FPlatformMath::FloorToInt(ActorToCameraRotationActorSpace.Pitch));
	//ActorToCameraRotationActorSpace.Roll = static_cast<float>(FPlatformMath::FloorToInt(ActorToCameraRotationActorSpace.Roll));
	//ActorToCameraRotationActorSpace.Yaw = static_cast<float>(FPlatformMath::FloorToInt(ActorToCameraRotationActorSpace.Yaw));
}
