#include "B2LobbySkeletalMeshActorBase.h"
//#include "BladeII.h"
#include "Animation/AnimInstance.h"

#include "BladeIIUtil.h"

/************************************************************************/
/* AB2UnitedLobbySKMeshActorBase
 AB2LobbySkeletalMeshActorBase 이후에 만들어졌음.
 이후에 추가된 다른 클래스를 같이 묶기 위한 더미격의 부모 클래스
 뭔가 기능이 들어갈 수도 있고..
 */
/************************************************************************/

AB2UnitedLobbySKMeshActorBase::AB2UnitedLobbySKMeshActorBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

const float LookUpTime = 1.2f;

/************************************************************************/
/* AB2LobbySkeletalMeshActorBase
 처음 만들었을 땐 이름 그대로 Lobby 에서의 SkeletalMeshActor Base 였겠지만  
 이제는 플레이어 캐릭터를 위한 베이스가 됨.
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
	
	if (IsHidden() == bNewHidden) // 같은 조건으로 여러번 이게 불릴 때가 있어서 아래 추가 코드들은 확실히 변경이 되었을 때만 실행되도록
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

		// 특히 Hidden 이후에 다시 보이도록 했을 때 Particle 붙인 것들이 안보이는 경우가 있어서.. 이렇게 해 주니 잘 나옴.
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
	//Lobby Anim 특별 보정
	//Lobby animation은 일반 animation과의 root위치가 다른데,
	//Lobby animation은 Bip001-Pelvis가 root이고
	//일반 animation은 RootBone이 root임.
	//그 차이인지는 몰라도 GetBoneLocation의 위치가 실제 렌더링되는 위치랑 차이가 있음.

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
	////[@AKI, 171118] 암살자에서 Floating 에러가 나서 그냥 소수점 Floor(버림) 했음.
	//// 매 틱마다 캐스팅 해서 걱정이 되기는 하지만 속도보다 Visualization이라 이렇게 함
	//ActorToCameraRotationActorSpace.Pitch = static_cast<float>(FPlatformMath::FloorToInt(ActorToCameraRotationActorSpace.Pitch));
	//ActorToCameraRotationActorSpace.Roll = static_cast<float>(FPlatformMath::FloorToInt(ActorToCameraRotationActorSpace.Roll));
	//ActorToCameraRotationActorSpace.Yaw = static_cast<float>(FPlatformMath::FloorToInt(ActorToCameraRotationActorSpace.Yaw));
}
