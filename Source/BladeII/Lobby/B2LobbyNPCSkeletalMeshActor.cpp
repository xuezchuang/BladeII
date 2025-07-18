#include "B2LobbyNPCSkeletalMeshActor.h"
//#include "BladeII.h"
#include "Event.h"
#include "Animation/AnimInstance.h"

#include "BladeIIUtil.h"
#include "Animation/AnimSequence.h"
#include "B2LobbyGameMode.h"
#include "B2UIManager.h"
#include "B2UIMsgPopupSelectFairyGift.h"
#include "B2UILobbyMain.h"
#include "B2UIChatting.h"
#include "B2UIQuestDialog.h"
#include "B2UISetting.h"
#include "B2UIHotTimeBuffPopup.h"
#include "B2UIDailyNoticePopup.h"

#include "BladeIIGameImpl.h"

AB2LobbyNPCSkeletalMeshActor::AB2LobbyNPCSkeletalMeshActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyLobbySceneType = ELobbyNPCType::ELobbyNPC_None;
}

void AB2LobbyNPCSkeletalMeshActor::TickForBlendedAnimTransition(class UAnimSequence* InCurrAnim, class UAnimSequence* InNextAnim,
	float InTotalTransitionTime, float BlendWeightForNextAnim,
	bool bPlayingCurrAnim, // True 면 InCurrAnim 의 플레이를 유지하면서 InNextAnim 으로 넘어가는 거고, False 면 InCurrAnim 은 마지막 위치를 유지한 채 InNextAnim 이 플레이되면서 넘어가는 거
	float CurrAnimLastPos, // bPlayingCurrAnim 이 False 일 때만 필요.
	bool bFireAnimNotifyOfCurrAnim,
	bool bFireAnimNotifyOfNextAnim
)
{
	// Matinee AnimControl 에서의 Blending 기능을 가져다 쓰는 것으로 제한적인 환경에서만..
	// 여기서 AnimBlueprint 대신 이런 식으로 anim 블렌딩을 하는 이유는 
	// AnimBlueprint (뿐 아닌 언리얼 UObject 리소스들..) 의 리소스 레퍼런스로 인한 로딩 시간 증가에 데인 경험 때문.
	// 혹여나 이 기능을 딴 데로 가져가서 사용할 때의 경고 차원으로..
	check(
		Cast<AB2LobbyNPCSkeletalMeshActor>(this) &&
		Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this))
	);
	// NextAnim 이 진행되면서 블렌딩하는 경우에는 CurrAnim 의 마지막 위치를 넣어주어야 함.
	check(bPlayingCurrAnim || CurrAnimLastPos >= 0.0f);

	if (!InCurrAnim || !InNextAnim) {
		return;
	}

	const float ClampedBlendingWeight = FMath::Clamp(BlendWeightForNextAnim, 0.0f, 1.0f); // 걍 안전 처리..

	// 현재 플레이하는 애니메이션보다 긴 시간의 transition 을 둘 수는 없다.
	const float FinalTotalTransitionTime = bPlayingCurrAnim ?
		FMath::Min(InTotalTransitionTime, InCurrAnim->GetPlayLength()) :
		FMath::Min(InTotalTransitionTime, InNextAnim->GetPlayLength());

	// bPlayingCurrent 여부에 따라 CurrAnim 과 NextAnim 각각의 플레이 위치 산정. 하나만 진행이 되고 다른 하나는 정해진 위치.
	const float CurrAnimPlayPos = bPlayingCurrAnim ?
		(InCurrAnim->GetPlayLength() - FinalTotalTransitionTime + FinalTotalTransitionTime * ClampedBlendingWeight) :
		CurrAnimLastPos;
	const float NexAnimPlayPos = bPlayingCurrAnim ?
		0.0f :
		(FinalTotalTransitionTime * ClampedBlendingWeight);

	
	//// 이하.. Matinee AnimControl 트랙에서의 Anim Blending 핵심 코드..

	//TArray<FAnimSlotInfo> AnimWeightSlotInfo;
	//{
	//	FAnimSlotInfo NewInfoA;
	//	NewInfoA.SlotName = TEXT("AAA"); // DoesAnimTrackSlotSupportAnimBlending .. 
	//	NewInfoA.ChannelWeights.Add(1.0f - ClampedBlendingWeight);
	//	AnimWeightSlotInfo.Add(NewInfoA);
	//	FAnimSlotInfo NewInfoB;
	//	NewInfoB.SlotName = TEXT("BBB");
	//	NewInfoB.ChannelWeights.Add(ClampedBlendingWeight);
	//	AnimWeightSlotInfo.Add(NewInfoB);

	//	// Transition 동안만 이걸 쓰는 걸 의도하는 거라 Looping 같은 인자는 큰 의미는 없고.. FireNotifies 하위 옵션이나 RootMotion 같은 걸 다르게 써야 하면 여기 매개변수로 넘기도록.
	//	SetAnimPosition(NewInfoA.SlotName, 0, Cast<UAnimSequence>(InCurrAnim), CurrAnimPlayPos, bFireAnimNotifyOfCurrAnim, true);
	//	SetAnimPosition(NewInfoB.SlotName, 1, Cast<UAnimSequence>(InNextAnim), NexAnimPlayPos, bFireAnimNotifyOfNextAnim, true);

	//	SetAnimWeights(AnimWeightSlotInfo);
	//}
}

/************************************************************************/
/* 
 AB2LobbyDoumiGirlBase
 상점소녀와 휴식도우미 간에 기능 상 비슷한 면이 있어서 공통 부모를 둠.
*/
/************************************************************************/
AB2LobbyDoumiGirlBase::AB2LobbyDoumiGirlBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPose = nullptr;
	LastPlayTempAnimLength = 0.0f;

	TempAnimEndTransitionTime = 1.0f;
	CurrEndTransitionTimeCounter = 0.0f;
	bInTempAnimEndTransition = false;

	CachedCurrPlayingTempAnim = nullptr;

	bUseTouchReaction = false;
	TouchAnimTransitionTime = 0.2f;
	bInTouchAnimTransition = false;
	CachedPreTouchAnim = nullptr;
	LastPreTouchAnimPlayedPos = 0.0f;
	CurrTouchAnimTransitionTimeCounter = 0.0f;
}

void AB2LobbyDoumiGirlBase::BeginPlay()
{
	Super::BeginPlay();

	BeginDefaultPose();
}

void AB2LobbyDoumiGirlBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// PlayTemporaryAnim 으로 시작한 TempAnim 에서 DefaultPose 로의 블렌딩.
	if (bInTempAnimEndTransition && CachedCurrPlayingTempAnim && DefaultPose)
	{		
		CurrEndTransitionTimeCounter += DeltaSeconds; // DefaultPose 의 블렌딩 weight 로 환산됨.

		TickForBlendedAnimTransition(Cast<UAnimSequence>(CachedCurrPlayingTempAnim), Cast<UAnimSequence>(DefaultPose),
			TempAnimEndTransitionTime,
			FMath::Clamp(CurrEndTransitionTimeCounter / TempAnimEndTransitionTime, 0.0f, 1.0f) 
		);
	}
	else if (bUseTouchReaction && bInTouchAnimTransition && CachedPreTouchAnim && CachedCurrPlayingTempAnim)
	{
		CurrTouchAnimTransitionTimeCounter += DeltaSeconds;

		// CachedCurrPlayingTempAnim 은 여기선 NextAnim 이 된다.
		TickForBlendedAnimTransition(Cast<UAnimSequence>(CachedPreTouchAnim), Cast<UAnimSequence>(CachedCurrPlayingTempAnim),
			TouchAnimTransitionTime,
			FMath::Clamp(CurrTouchAnimTransitionTimeCounter / TouchAnimTransitionTime, 0.0f, 1.0f),
			false, LastPreTouchAnimPlayedPos
		);
	}
}

void AB2LobbyDoumiGirlBase::BeginDefaultPose()
{
	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	if (DefaultPose && SKComp)
	{ // DefaultPose 는 루핑으로 지속시킨다.
		SKComp->PlayAnimation(DefaultPose, true);

		CachedCurrPlayingTempAnim = nullptr;
		bInTempAnimEndTransition = false;
		CurrEndTransitionTimeCounter = 0.0f;
	}
}

void AB2LobbyDoumiGirlBase::PlayTemporaryAnim(class UAnimSequenceBase* InAnimToPlay)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("AB2LobbyDoumiGirlBase::PlayTemporaryAnim, AnimToPlay %s"), InAnimToPlay ? *InAnimToPlay->GetName() : TEXT("NULL")));

	check(!IsInAnimStateGuard()); // 하위 클래스에서 까먹었을 시 걸림.

	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	if (InAnimToPlay && InAnimToPlay->GetPlayLength() > 0.0f && SKComp)
	{
		// 애니메이션 길이 동안만 플레이하도록 타이머 설치
		LastPlayTempAnimLength = InAnimToPlay->GetPlayLength();
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{
			FTimerManager& WTM = TheWorld->GetTimerManager();

			WTM.ClearTimer(TempAnimPlayTH); // 이전꺼 끝나지 않았는데 바로 다른 Anim 플레이에 대한 요청이 올 수도 있겠다.
			WTM.SetTimer(TempAnimPlayTH, this, &AB2LobbyDoumiGirlBase::TempAnimPlayDoneCB, LastPlayTempAnimLength, false);

			WTM.ClearTimer(TempAnimPlayEndTransitionTH);
			WTM.SetTimer(TempAnimPlayEndTransitionTH, this, &AB2LobbyDoumiGirlBase::TempAnimEndTransitionCB,
				// 애니메이션 길이가 TempAnimEndTransitionTime 보다 짧을 수도 있겠는데 그럼 시작부터 DefaultPose 로 블렌딩을 하겠지..
				FMath::Max(LastPlayTempAnimLength - TempAnimEndTransitionTime, KINDA_SMALL_NUMBER), false);
		}

		SKComp->PlayAnimation(InAnimToPlay, false);

		CachedCurrPlayingTempAnim = InAnimToPlay;
		bInTempAnimEndTransition = false;
	}
}

void AB2LobbyDoumiGirlBase::TempAnimEndTransitionCB()
{
	// TempAnimPlayDoneCB 전으로 의도.
	// 이 시점부터 다음 (DefaultPose) 애니메이션까지 블렌딩이 되다가 TempAnimPlayDoneCB 에서 완전히 전환
	
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TempAnimPlayEndTransitionTH);
	}

	bInTempAnimEndTransition = true;
	CurrEndTransitionTimeCounter = 0.0f;
}

void AB2LobbyDoumiGirlBase::TempAnimPlayDoneCB()
{
	if (auto* TheWorld = GetWorld())
		TheWorld->GetTimerManager().ClearTimer(TempAnimPlayTH);

	if (auto* SKComp = GetSkeletalMeshComponent())
		SKComp->Stop(); // 필요한가..

	if (bCurrPlayTouchAnim)
		bCurrPlayTouchAnim = false;

	BeginDefaultPose();
}

void AB2LobbyDoumiGirlBase::QuitTempAnimEndTransitionState()
{
	// TempAnimEndTransition 의 블렌딩이나 BeginDefaultPose 로 돌아가는 예약 같은 거 없애는 거.
	// 이걸 콜하고 그냥 두면 현재 TempAnim 이 끝나고 나면 그냥 정지한 상태가 될 것임.
	// 특정한 사용 상황이 있음.

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TempAnimPlayEndTransitionTH);
		TheWorld->GetTimerManager().ClearTimer(TempAnimPlayTH);
	}
	bInTempAnimEndTransition = false;
}

void AB2LobbyDoumiGirlBase::NotifyActorOnReleased(FKey ButtonReleased)
{ // PC 프리뷰 용
	Super::NotifyActorOnReleased(ButtonReleased);

	if (bUseTouchReaction)
	{
		PlayReactionOnTouch();
	}
}
void AB2LobbyDoumiGirlBase::NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchEnd(FingerIndex);

	if (bUseTouchReaction)
	{
		PlayReactionOnTouch();
	}
}

void AB2LobbyDoumiGirlBase::PlayReactionOnTouch()
{
	/************************************************************************/
	/*
	기본 기능은 그냥 Touch 모션에 대해 PlayTemporaryAnim 하나로 끝나는 건데
	직전에 플레이하던 애니메이션으로부터의 블렌딩을 넣느라 좀 복잡해짐.
	*/
	/************************************************************************/

	if (bCurrPlayTouchAnim || IsInAnimStateGuard())
		return;

	const bool bWasInTempAnimEndTransition = bInTempAnimEndTransition;
	QuitTempAnimEndTransitionState(); // 만일 TempAnim 플레이 중이면 기본 DefaultPose 로의 transition 은 취소.

	// TouchAnim 이전에 플레이하던 애니메이션 상태를 캐싱해 둔다. 
	if (CachedCurrPlayingTempAnim) {
		CachedPreTouchAnim = CachedCurrPlayingTempAnim;
	}
	else { // DefaultPose 로 돌아갈 때 CachedCurrPlayingTempAnim 이 null-out 되었겠지.
		CachedPreTouchAnim = DefaultPose;
	}
	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	if (CachedPreTouchAnim && SKComp)
	{
		if (bWasInTempAnimEndTransition)
		{
			// 이 때에는 별도로 blending 을 하는 바람에 SKComp->GetPosition() 이 0 이 나오므로 LastPreTouchAnimPlayedPos 을 별도로..
			// TempAnimEndTransitionTime 등을 가지고 정확한 position 을 계산해 낼 수 있긴 한데.. TickForBlendedAnimTransition 내용을 가져와야 함.
			// 좀 복잡해질 것 같으니 적당히 중간 지점에서부터 시작.. 이 정도로 대부분의 상황에서 크게 눈에 안 띄면 이렇게 가고.
			LastPreTouchAnimPlayedPos = FMath::Max((CachedPreTouchAnim->GetPlayLength() - TempAnimEndTransitionTime * 0.5f), 0.0f);
		}
		else
		{
			LastPreTouchAnimPlayedPos = SKComp->GetPosition();
		}
	}

	OnPlayReactionOnTouch(); // 여기서 실제 TouchAnim 에 대한 Play 를 호출해 주어야 한다.
	
	UWorld* TheWorld = GetWorld();
	if (CachedPreTouchAnim && CachedCurrPlayingTempAnim && SKComp && TheWorld)
	{
		FTimerManager& WTM = TheWorld->GetTimerManager();

		WTM.ClearTimer(TouchAnimTransitionTH);
		WTM.SetTimer(TouchAnimTransitionTH, this, &AB2LobbyDoumiGirlBase::TouchAnimTransitionCB, TouchAnimTransitionTime, false);
		// TouchAnimTransitionTime 이 될 때까지 블렌딩 시작.
		bInTouchAnimTransition = true;
		CurrTouchAnimTransitionTimeCounter = 0.0f;
	}
}

void AB2LobbyDoumiGirlBase::TouchAnimTransitionCB()
{
	check(!bInTempAnimEndTransition); // 애니메이션 transition 블렌딩을 위한 상태 둘이 서로 혼합되지 않도록..

	bInTouchAnimTransition = false;
	CurrTouchAnimTransitionTimeCounter = 0.0f;
	CachedPreTouchAnim = nullptr;

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TouchAnimTransitionTH);
	}

	OnTouchAnimTransitionCB(); // TouchAnimTransitionTime 만큼의 위치에서 다시금 Touch 모션을 플레이.
}

void AB2LobbyDoumiGirlBase::QuitTouchAnimTransitionState()
{
	// 일반적인 TouchAnim 핸들링 와중에 사용될 목적은 아니고, 다른 애니메이션 플레이 이전에 상태 클리어를 확실히 하고자 하는 것.

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TouchAnimTransitionTH);
	}
	bInTouchAnimTransition = false;
	CurrTouchAnimTransitionTimeCounter = 0.0f;
	//CachedPreTouchAnim = nullptr; 여기서 null-out 하기 적절할 것 같지만 CachedPreTouchAnim 을 세팅한 직후에 이걸 콜하는 상황이 있어서 이건 제외.
}

void AB2LobbyDoumiGirlBase::OnPlayReactionOnTouch()
{
	// TouchAnim 을 사용하려면 Overriding 필요.
	// 실제 TouchAnim 에 대한 Play 를 호출해 주어야 한다.
	bCurrPlayTouchAnim = true;
}

void AB2LobbyDoumiGirlBase::OnTouchAnimTransitionCB()
{
	// TouchAnim 을 사용하려면 Overriding 필요.
	// TouchReaction 시작 이후 흐른 시간(TouchAnimTransitionTime)만큼의 위치에서 다시금 Touch 모션을 플레이 하는 게 필요.
}

void AB2LobbyDoumiGirlBase::SetAnimStateGuard(float InGuardDuration)
{
	if (InGuardDuration > 0.0f)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{
			FTimerManager& WTM = TheWorld->GetTimerManager();
			WTM.ClearTimer(AnimStateGuardTH);

			// 모두는 아니지만 대부분의 애니메이션 플레이 호출이 씹힐 것.
			bInAnimStateGuard = true;

			WTM.SetTimer(AnimStateGuardTH, this, &AB2LobbyDoumiGirlBase::AnimStateGuardExpireCB, InGuardDuration, false);
		}
	}
}
void AB2LobbyDoumiGirlBase::AnimStateGuardExpireCB()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(AnimStateGuardTH);
	}

	bInAnimStateGuard = false;
}

/** ShopGirl 과 RelaxingDoumi 간의 반복 코드 최소화 템플릿 낄낄 */
template<typename TStateEnum>
UAnimSequenceBase* GetLobbyNPCAnimSeqOfStateFromTMapTempl(TMap<TStateEnum, TSoftObjectPtr<UAnimSequenceBase>>& InStateAnimMap, TStateEnum InState)
{
	TSoftObjectPtr<UAnimSequenceBase>* FoundTSoftObjectPtrPtr = InStateAnimMap.Find(InState);
	if (FoundTSoftObjectPtrPtr)
	{
		TSoftObjectPtr<UAnimSequenceBase>& FoundTSoftObjectPtr = *FoundTSoftObjectPtrPtr;
		if (!FoundTSoftObjectPtr.IsNull())
		{ // 단순 LoadSynchronous 를 사용.
			// 이것 자체의 Async 로딩 계획이나 다른 asset 의 async 로딩과 얽힐 소지가 없다는 판단 하에 사용하는 것임.
			return FoundTSoftObjectPtr.LoadSynchronous();
		}
	}
	return nullptr;
}


/************************************************************************/
/* 
 AB2LobbyShopGirl
 유저들의 주머니를 터는 데 일가견이 있는 기요미 상점 소녀
*/
/************************************************************************/
AB2LobbyShopGirl::AB2LobbyShopGirl(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyLobbySceneType = ELobbyNPCType::ELobbyNPC_ShopGirl;
	bUseTouchReaction = true;
}

UAnimSequenceBase* AB2LobbyShopGirl::GetAnimSeqOfState(ELobbyShopGirlState InState)
{
	if (InState == ELobbyShopGirlState::ELSG_IdleLoop && DefaultPose)
	{ // Idle 은 그냥 DefaultPose.. 사실 Idle 을 플레이하는 경우면 어지간해선 BeginDefaultPose 를 바로 써야..
		return DefaultPose;
	}

	return GetLobbyNPCAnimSeqOfStateFromTMapTempl<ELobbyShopGirlState>(StateAnims, InState);
}

void AB2LobbyShopGirl::PlayTemporaryAnimOfState(ELobbyShopGirlState InState)
{
	if (!IsInAnimStateGuard())
	{
		QuitTouchAnimTransitionState();

		UAnimSequenceBase* FoundStateAnim = GetAnimSeqOfState(InState);
		if (FoundStateAnim)
		{
			PlayTemporaryAnim(FoundStateAnim);
		}
	}
}

void AB2LobbyShopGirl::OnPlayReactionOnTouch()
{
	Super::OnPlayReactionOnTouch();

	PlayTemporaryAnimOfState(ELobbyShopGirlState::ELSG_TouchReaction);

	// CachedCurrPlayingTempAnim 은 Touch 모션이 되어야..
	// CachedPreTouchAnim 이랑 CachedCurrPlayingTempAnim 은 같을 수도 있다. 연달아 터치를 하면
	check(CachedPreTouchAnim && (GetAnimSeqOfState(ELobbyShopGirlState::ELSG_TouchReaction) == CachedCurrPlayingTempAnim));
}
void AB2LobbyShopGirl::OnTouchAnimTransitionCB()
{
	// TouchAnim 을 지금까지 플레이한 위치에서 시작해야 해서 다시 한 번 더..
	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	if (SKComp)
	{
		SKComp->PlayAnimation(GetAnimSeqOfState(ELobbyShopGirlState::ELSG_TouchReaction), false);
		SKComp->SetPosition(TouchAnimTransitionTime);
	}
}

/************************************************************************/
/* 
 AB2LobbyRelaxingDoumi
 유저들이 한눈 팔 틈을 주지 않는 팜므파탄 휴식 도우미
*/
/************************************************************************/
AB2LobbyRelaxingDoumi::AB2LobbyRelaxingDoumi(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyLobbySceneType = ELobbyNPCType::ELobbyNPC_RelaxingDoumi;
	bUseTouchReaction = true;
}

UAnimSequenceBase* AB2LobbyRelaxingDoumi::GetAnimSeqOfState(ELobbyRelaxingDoumiState InState)
{
	if (InState == ELobbyRelaxingDoumiState::ELRD_IdleLoop && DefaultPose)
	{ // Idle 은 그냥 DefaultPose.. 사실 Idle 을 플레이하는 경우면 어지간해선 BeginDefaultPose 를 바로 써야..
		return DefaultPose;
	}

	return GetLobbyNPCAnimSeqOfStateFromTMapTempl<ELobbyRelaxingDoumiState>(StateAnims, InState);
}

void AB2LobbyRelaxingDoumi::PlayTemporaryAnimOfState(ELobbyRelaxingDoumiState InState)
{
	if (!IsInAnimStateGuard())
	{
		QuitTouchAnimTransitionState();

		UAnimSequenceBase* FoundStateAnim = GetAnimSeqOfState(InState);
		if (FoundStateAnim)
		{
			PlayTemporaryAnim(FoundStateAnim);
		}
	}
}

void AB2LobbyRelaxingDoumi::OnPlayReactionOnTouch()
{
	Super::OnPlayReactionOnTouch();

	PlayTemporaryAnimOfState(ELobbyRelaxingDoumiState::ELRD_TouchReaction);

	// CachedCurrPlayingTempAnim 은 Touch 모션이 되어야..
	// CachedPreTouchAnim 이랑 CachedCurrPlayingTempAnim 은 같을 수도 있다. 연달아 터치를 하면
	check(CachedPreTouchAnim && (GetAnimSeqOfState(ELobbyRelaxingDoumiState::ELRD_TouchReaction) == CachedCurrPlayingTempAnim));
}
void AB2LobbyRelaxingDoumi::OnTouchAnimTransitionCB()
{
	// TouchAnim 을 지금까지 플레이한 위치에서 시작해야 해서 다시 한 번 더..
	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	if (SKComp)
	{
		SKComp->PlayAnimation(GetAnimSeqOfState(ELobbyRelaxingDoumiState::ELRD_TouchReaction), false);
		SKComp->SetPosition(TouchAnimTransitionTime);
	}
}

/************************************************************************/
/*
AB2LobbyFairyDoumi
*/
/************************************************************************/
AB2LobbyFairyDoumi::AB2LobbyFairyDoumi(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyLobbySceneType = ELobbyNPCType::ELobbyNPC_Fairy_Niece;
	bUseTouchReaction = true;
}

UAnimSequenceBase* AB2LobbyFairyDoumi::GetAnimSeqOfState(ELobbyFairyDoumiState InState)
{
	if (InState == ELobbyFairyDoumiState::ELFD_IdleLoop && DefaultPose)
	{
		return DefaultPose;
	}

	return GetLobbyNPCAnimSeqOfStateFromTMapTempl<ELobbyFairyDoumiState>(StateAnims, InState);
}

void AB2LobbyFairyDoumi::PlayTemporaryAnimOfState(ELobbyFairyDoumiState InState)
{
	if (!IsInAnimStateGuard())
	{
		QuitTouchAnimTransitionState();

		UAnimSequenceBase* FoundStateAnim = GetAnimSeqOfState(InState);
		if (FoundStateAnim)
		{
			PlayTemporaryAnim(FoundStateAnim);
		}
	}
}

void AB2LobbyFairyDoumi::OnPlayReactionOnTouch()
{
	Super::OnPlayReactionOnTouch();

	RandTouchState = FMath::RandBool() ? ELobbyFairyDoumiState::ELFD_TouchReaction01 : ELobbyFairyDoumiState::ELFD_TouchReaction02;

	PlayTemporaryAnimOfState(RandTouchState);

	check(CachedPreTouchAnim && (GetAnimSeqOfState(RandTouchState) == CachedCurrPlayingTempAnim));
}

void AB2LobbyFairyDoumi::OnTouchAnimTransitionCB()
{
	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	if (SKComp)
	{
		SKComp->PlayAnimation(GetAnimSeqOfState(RandTouchState), false);
		SKComp->SetPosition(TouchAnimTransitionTime);
	}
}

void AB2LobbyFairyDoumi::ForcedRemoveAttachedFX()
{
	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	if (SKComp)
	{
		for (int32 i = 0; i < SKComp->GetNumChildrenComponents(); i++)
		{
			UParticleSystemComponent* SpawnedFxComp = Cast<UParticleSystemComponent>(SKComp->GetChildComponent(i));

			if (SpawnedFxComp)
			{
				SpawnedFxComp->Deactivate();
				SpawnedFxComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
				SpawnedFxComp->UnregisterComponent();
				SpawnedFxComp->DestroyComponent();
			}
		}
	}
}

void AB2LobbyFairyDoumi::BeginPlay()
{
	/*Super::BeginPlay();

	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	if (SKComp && FxParticle)
	{
		UParticleSystemComponent* pPSC = CreateParticleSystemHelper(FxParticle, GetWorld(), this, false);
		pPSC->RegisterComponent();
		pPSC->AttachToComponent(SKComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), AttachedBoneName);
		pPSC->Activate(true);
	}*/
}


void AB2LobbyFairyDoumi::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SetVisibleCharacter(true);
	ForcedRemoveAttachedFX();
}

void AB2LobbyFairyDoumi::SetVisibleCharacter(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
	SetActorEnableCollision(bVisible);
}

/************************************************************************/
/*
AB2LobbyFlyingFairyDoumi
*/
/************************************************************************/
AB2LobbyFlyingFairyDoumi::AB2LobbyFlyingFairyDoumi(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyLobbySceneType = ELobbyNPCType::ELobbyNPC_FlyingFairy_Niece;
	bUseTouchReaction = true;
}

void AB2LobbyFlyingFairyDoumi::BeginPlay()
{
	Super::BeginPlay();

	InitLobbySceneType();
	InitDynamicMaterial();

	//SetAttachFX(BladeIIGameImpl::GetClientDataStore().GetPossibleFairyGift());
}

void AB2LobbyFlyingFairyDoumi::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SetVisibleCharacter(true);
	SetAttachFX(false);
}

void AB2LobbyFlyingFairyDoumi::OnPlayReactionOnTouch()
{
	/*if (IsPossibleGetFairyGift == false)
	{
		return;
	}

	if (IsOpenedOtherPopup() == true)
	{
		return;
	}

	auto* UIManager = UB2UIManager::GetInstance();
	if (UIManager)
	{
		if (UIManager->GetUI<UB2UIMsgPopupSelectFairyGift>(UIFName::MsgPopupSelectFairyGift))
		{
			return;
		}

		UB2UIMsgPopupSelectFairyGift* pMsgPopupSelectFairyGift = UIManager->OpenUI<UB2UIMsgPopupSelectFairyGift>(UIFName::MsgPopupSelectFairyGift);
		if (pMsgPopupSelectFairyGift)
		{
			pMsgPopupSelectFairyGift->OnChangedFairy(GetFairyType());
		}
	}*/
}

void AB2LobbyFlyingFairyDoumi::SetAttachFX(bool bAttach)
{
	//IsPossibleGetFairyGift = bAttach;

	//USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	//if (SKComp == nullptr)
	//{
	//	return;
	//}

	//if (bAttach)
	//{
	//	if (FairyDynamicyMaterial)
	//	{
	//		FairyDynamicyMaterial->SetScalarParameterValue("Fresnel_intensity", EffectFresnelIntensityValue);
	//	}

	//	if (FxParticle)
	//	{
	//		UParticleSystemComponent* pPSC = CreateParticleSystemHelper(FxParticle, GetWorld(), this, false);
	//		if (pPSC)
	//		{
	//			pPSC->RegisterComponent();
	//			pPSC->AttachToComponent(SKComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), AttachedBoneName);
	//			pPSC->Activate(true);
	//		}
	//	}
	//}
	//else
	//{
	//	if (FairyDynamicyMaterial)
	//	{
	//		FairyDynamicyMaterial->SetScalarParameterValue("Fresnel_intensity", 0.0f);
	//	}

	//	for (int32 i = 0; i < SKComp->GetNumChildrenComponents(); i++)
	//	{
	//		UParticleSystemComponent* SpawnedFxComp = Cast<UParticleSystemComponent>(SKComp->GetChildComponent(i));
	//		if (SpawnedFxComp)
	//		{
	//			SpawnedFxComp->Deactivate();
	//			SpawnedFxComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	//			SpawnedFxComp->UnregisterComponent();
	//			SpawnedFxComp->DestroyComponent();
	//		}
	//	}
	//}
}

bool AB2LobbyFlyingFairyDoumi::IsOpenedOtherPopup()
{
	//UB2UILobbyMain* LobbyMainUIPage = UB2UIManager::GetInstance()->GetUI<UB2UILobbyMain>(UIFName::LobbyMain);
	//if (LobbyMainUIPage)
	//{
	//	if (LobbyMainUIPage->GetCombineMenuIsVisible()) // 통합메뉴가 떠있는 상태에서는 사용안되게끔
	//		return true;
	//}

	//UB2UIQuestDialog* QuestDialogUI = UB2UIManager::GetInstance()->GetUI<UB2UIQuestDialog>(UIFName::QuestDialog);
	//if (QuestDialogUI)
	//{
	//	return true;
	//}

	//UB2UIChatting* ChattingPopupUI = UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting);
	//if (ChattingPopupUI)
	//{
	//	return true;
	//}

	//UB2UISetting* SettingPopupUI = UB2UIManager::GetInstance()->GetUI<UB2UISetting>(UIFName::Setting);
	//if (SettingPopupUI)
	//{
	//	return true;
	//}

	//UB2UIHotTimeBuffPopup* HotTimeBuffPopupUI = UB2UIManager::GetInstance()->GetUI<UB2UIHotTimeBuffPopup>(UIFName::HotTimePopup);
	//if (HotTimeBuffPopupUI)
	//{
	//	return true;
	//}

	//UB2UIDailyNoticePopup* DailyNoticePopupUI = UB2UIManager::GetInstance()->GetUI<UB2UIDailyNoticePopup>(UIFName::DailyNoticePopup);
	//if (DailyNoticePopupUI)
	//{
	//	return true;
	//}

	return false;
}

void AB2LobbyFlyingFairyDoumi::InitLobbySceneType()
{
	switch (FairyType)
	{
	case EFairyType::FAIRY_NIECE:
		MyLobbySceneType = ELobbyNPCType::ELobbyNPC_FlyingFairy_Niece;
		break;
	case EFairyType::FAIRY_PANG:
		MyLobbySceneType = ELobbyNPCType::ELobbyNPC_FlyingFairy_Pang;
		break;
	case EFairyType::FAIRY_RIO:
		MyLobbySceneType = ELobbyNPCType::ELobbyNPC_FlyingFairy_Rio;
		break;
	default:
		MyLobbySceneType = ELobbyNPCType::ELobbyNPC_None;
		break;
	}
}

void AB2LobbyFlyingFairyDoumi::InitDynamicMaterial()
{
	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	if (SKComp && FairyDynamicyMaterial == nullptr)
	{
		const int32 FairyBodyMaterialIndex = 0;

		UMaterialInterface* FairyMaterial = SKComp->GetMaterial(FairyBodyMaterialIndex);
		if (FairyMaterial)
		{
			FairyDynamicyMaterial = UMaterialInstanceDynamic::Create(FairyMaterial, this);
			if (FairyDynamicyMaterial)
			{
				SKComp->SetMaterial(FairyBodyMaterialIndex, FairyDynamicyMaterial);
			}
		}
	}
}

void AB2LobbyFlyingFairyDoumi::SetVisibleCharacter(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
	SetActorEnableCollision(bVisible);
}


/************************************************************************/
/*
AB2SealCard
*/
/************************************************************************/
AB2SealCard::AB2SealCard(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyLobbySceneType = ELobbyNPCType::ELobbyNPC_Test;
	bUseTouchReaction = true;
}

void AB2SealCard::BeginPlay()
{
	Super::BeginPlay();
}

void AB2SealCard::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AB2SealCard::OnPlayReactionOnTouch()
{
//	봉인함 카드 임시 작업
	//SealBoxCardFlipClass<int32, int32>::GetInstance().Signal(1,1);
}