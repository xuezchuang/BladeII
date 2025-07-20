#include "B2LobbySkeletalMeshActorBase.h"
//#include "BladeII.h"
#include "Animation/AnimInstance.h"

#include "BladeIIUtil.h"

/************************************************************************/
/* AB2UnitedLobbySKMeshActorBase
 AB2LobbySkeletalMeshActorBase 捞饶俊 父甸绢脸澜.
 捞饶俊 眠啊等 促弗 努贰胶甫 鞍捞 弓扁 困茄 歹固拜狼 何葛 努贰胶
 购啊 扁瓷捞 甸绢哎 荐档 乐绊..
 */
/************************************************************************/

AB2UnitedLobbySKMeshActorBase::AB2UnitedLobbySKMeshActorBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

const float LookUpTime = 1.2f;

/************************************************************************/
/* AB2LobbySkeletalMeshActorBase
 贸澜 父甸菌阑 订 捞抚 弊措肺 Lobby 俊辑狼 SkeletalMeshActor Base 看摆瘤父  
 捞力绰 敲饭捞绢 某腐磐甫 困茄 海捞胶啊 凳.
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
	
	if (IsHidden() == bNewHidden) // 鞍篮 炼扒栏肺 咯矾锅 捞霸 阂副 锭啊 乐绢辑 酒贰 眠啊 内靛甸篮 犬角洒 函版捞 登菌阑 锭父 角青登档废
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

		// 漂洒 Hidden 捞饶俊 促矫 焊捞档废 沁阑 锭 Particle 嘿牢 巴甸捞 救焊捞绰 版快啊 乐绢辑.. 捞犯霸 秦 林聪 肋 唱咳.
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
	//Lobby Anim 漂喊 焊沥
	//Lobby animation篮 老馆 animation苞狼 root困摹啊 促弗单,
	//Lobby animation篮 Bip001-Pelvis啊 root捞绊
	//老馆 animation篮 RootBone捞 root烙.
	//弊 瞒捞牢瘤绰 隔扼档 GetBoneLocation狼 困摹啊 角力 坊歹傅登绰 困摹尔 瞒捞啊 乐澜.

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
	////[@AKI, 171118] 鞠混磊俊辑 Floating 俊矾啊 唱辑 弊成 家荐痢 Floor(滚覆) 沁澜.
	//// 概 平付促 某胶泼 秦辑 捌沥捞 登扁绰 窍瘤父 加档焊促 Visualization捞扼 捞犯霸 窃
	//ActorToCameraRotationActorSpace.Pitch = static_cast<float>(FPlatformMath::FloorToInt(ActorToCameraRotationActorSpace.Pitch));
	//ActorToCameraRotationActorSpace.Roll = static_cast<float>(FPlatformMath::FloorToInt(ActorToCameraRotationActorSpace.Roll));
	//ActorToCameraRotationActorSpace.Yaw = static_cast<float>(FPlatformMath::FloorToInt(ActorToCameraRotationActorSpace.Yaw));
}
