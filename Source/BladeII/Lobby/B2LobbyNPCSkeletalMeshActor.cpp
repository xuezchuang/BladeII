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
	bool bPlayingCurrAnim, // True �� InCurrAnim �� �÷��̸� �����ϸ鼭 InNextAnim ���� �Ѿ�� �Ű�, False �� InCurrAnim �� ������ ��ġ�� ������ ä InNextAnim �� �÷��̵Ǹ鼭 �Ѿ�� ��
	float CurrAnimLastPos, // bPlayingCurrAnim �� False �� ���� �ʿ�.
	bool bFireAnimNotifyOfCurrAnim,
	bool bFireAnimNotifyOfNextAnim
)
{
	// Matinee AnimControl ������ Blending ����� ������ ���� ������ �������� ȯ�濡����..
	// ���⼭ AnimBlueprint ��� �̷� ������ anim ������ �ϴ� ������ 
	// AnimBlueprint (�� �ƴ� �𸮾� UObject ���ҽ���..) �� ���ҽ� ���۷����� ���� �ε� �ð� ������ ���� ���� ����.
	// Ȥ���� �� ����� �� ���� �������� ����� ���� ��� ��������..
	check(
		Cast<AB2LobbyNPCSkeletalMeshActor>(this) &&
		Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this))
	);
	// NextAnim �� ����Ǹ鼭 �����ϴ� ��쿡�� CurrAnim �� ������ ��ġ�� �־��־�� ��.
	check(bPlayingCurrAnim || CurrAnimLastPos >= 0.0f);

	if (!InCurrAnim || !InNextAnim) {
		return;
	}

	const float ClampedBlendingWeight = FMath::Clamp(BlendWeightForNextAnim, 0.0f, 1.0f); // �� ���� ó��..

	// ���� �÷����ϴ� �ִϸ��̼Ǻ��� �� �ð��� transition �� �� ���� ����.
	const float FinalTotalTransitionTime = bPlayingCurrAnim ?
		FMath::Min(InTotalTransitionTime, InCurrAnim->GetPlayLength()) :
		FMath::Min(InTotalTransitionTime, InNextAnim->GetPlayLength());

	// bPlayingCurrent ���ο� ���� CurrAnim �� NextAnim ������ �÷��� ��ġ ����. �ϳ��� ������ �ǰ� �ٸ� �ϳ��� ������ ��ġ.
	const float CurrAnimPlayPos = bPlayingCurrAnim ?
		(InCurrAnim->GetPlayLength() - FinalTotalTransitionTime + FinalTotalTransitionTime * ClampedBlendingWeight) :
		CurrAnimLastPos;
	const float NexAnimPlayPos = bPlayingCurrAnim ?
		0.0f :
		(FinalTotalTransitionTime * ClampedBlendingWeight);

	
	//// ����.. Matinee AnimControl Ʈ�������� Anim Blending �ٽ� �ڵ�..

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

	//	// Transition ���ȸ� �̰� ���� �� �ǵ��ϴ� �Ŷ� Looping ���� ���ڴ� ū �ǹ̴� ����.. FireNotifies ���� �ɼ��̳� RootMotion ���� �� �ٸ��� ��� �ϸ� ���� �Ű������� �ѱ⵵��.
	//	SetAnimPosition(NewInfoA.SlotName, 0, Cast<UAnimSequence>(InCurrAnim), CurrAnimPlayPos, bFireAnimNotifyOfCurrAnim, true);
	//	SetAnimPosition(NewInfoB.SlotName, 1, Cast<UAnimSequence>(InNextAnim), NexAnimPlayPos, bFireAnimNotifyOfNextAnim, true);

	//	SetAnimWeights(AnimWeightSlotInfo);
	//}
}

/************************************************************************/
/* 
 AB2LobbyDoumiGirlBase
 �����ҳ�� �޽ĵ���� ���� ��� �� ����� ���� �־ ���� �θ� ��.
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

	// PlayTemporaryAnim ���� ������ TempAnim ���� DefaultPose ���� ����.
	if (bInTempAnimEndTransition && CachedCurrPlayingTempAnim && DefaultPose)
	{		
		CurrEndTransitionTimeCounter += DeltaSeconds; // DefaultPose �� ���� weight �� ȯ���.

		TickForBlendedAnimTransition(Cast<UAnimSequence>(CachedCurrPlayingTempAnim), Cast<UAnimSequence>(DefaultPose),
			TempAnimEndTransitionTime,
			FMath::Clamp(CurrEndTransitionTimeCounter / TempAnimEndTransitionTime, 0.0f, 1.0f) 
		);
	}
	else if (bUseTouchReaction && bInTouchAnimTransition && CachedPreTouchAnim && CachedCurrPlayingTempAnim)
	{
		CurrTouchAnimTransitionTimeCounter += DeltaSeconds;

		// CachedCurrPlayingTempAnim �� ���⼱ NextAnim �� �ȴ�.
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
	{ // DefaultPose �� �������� ���ӽ�Ų��.
		SKComp->PlayAnimation(DefaultPose, true);

		CachedCurrPlayingTempAnim = nullptr;
		bInTempAnimEndTransition = false;
		CurrEndTransitionTimeCounter = 0.0f;
	}
}

void AB2LobbyDoumiGirlBase::PlayTemporaryAnim(class UAnimSequenceBase* InAnimToPlay)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("AB2LobbyDoumiGirlBase::PlayTemporaryAnim, AnimToPlay %s"), InAnimToPlay ? *InAnimToPlay->GetName() : TEXT("NULL")));

	check(!IsInAnimStateGuard()); // ���� Ŭ�������� ��Ծ��� �� �ɸ�.

	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	if (InAnimToPlay && InAnimToPlay->GetPlayLength() > 0.0f && SKComp)
	{
		// �ִϸ��̼� ���� ���ȸ� �÷����ϵ��� Ÿ�̸� ��ġ
		LastPlayTempAnimLength = InAnimToPlay->GetPlayLength();
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{
			FTimerManager& WTM = TheWorld->GetTimerManager();

			WTM.ClearTimer(TempAnimPlayTH); // ������ ������ �ʾҴµ� �ٷ� �ٸ� Anim �÷��̿� ���� ��û�� �� ���� �ְڴ�.
			WTM.SetTimer(TempAnimPlayTH, this, &AB2LobbyDoumiGirlBase::TempAnimPlayDoneCB, LastPlayTempAnimLength, false);

			WTM.ClearTimer(TempAnimPlayEndTransitionTH);
			WTM.SetTimer(TempAnimPlayEndTransitionTH, this, &AB2LobbyDoumiGirlBase::TempAnimEndTransitionCB,
				// �ִϸ��̼� ���̰� TempAnimEndTransitionTime ���� ª�� ���� �ְڴµ� �׷� ���ۺ��� DefaultPose �� ������ �ϰ���..
				FMath::Max(LastPlayTempAnimLength - TempAnimEndTransitionTime, KINDA_SMALL_NUMBER), false);
		}

		SKComp->PlayAnimation(InAnimToPlay, false);

		CachedCurrPlayingTempAnim = InAnimToPlay;
		bInTempAnimEndTransition = false;
	}
}

void AB2LobbyDoumiGirlBase::TempAnimEndTransitionCB()
{
	// TempAnimPlayDoneCB ������ �ǵ�.
	// �� �������� ���� (DefaultPose) �ִϸ��̼Ǳ��� ������ �Ǵٰ� TempAnimPlayDoneCB ���� ������ ��ȯ
	
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
		SKComp->Stop(); // �ʿ��Ѱ�..

	if (bCurrPlayTouchAnim)
		bCurrPlayTouchAnim = false;

	BeginDefaultPose();
}

void AB2LobbyDoumiGirlBase::QuitTempAnimEndTransitionState()
{
	// TempAnimEndTransition �� �����̳� BeginDefaultPose �� ���ư��� ���� ���� �� ���ִ� ��.
	// �̰� ���ϰ� �׳� �θ� ���� TempAnim �� ������ ���� �׳� ������ ���°� �� ����.
	// Ư���� ��� ��Ȳ�� ����.

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TempAnimPlayEndTransitionTH);
		TheWorld->GetTimerManager().ClearTimer(TempAnimPlayTH);
	}
	bInTempAnimEndTransition = false;
}

void AB2LobbyDoumiGirlBase::NotifyActorOnReleased(FKey ButtonReleased)
{ // PC ������ ��
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
	�⺻ ����� �׳� Touch ��ǿ� ���� PlayTemporaryAnim �ϳ��� ������ �ǵ�
	������ �÷����ϴ� �ִϸ��̼����κ����� ������ �ִ��� �� ��������.
	*/
	/************************************************************************/

	if (bCurrPlayTouchAnim || IsInAnimStateGuard())
		return;

	const bool bWasInTempAnimEndTransition = bInTempAnimEndTransition;
	QuitTempAnimEndTransitionState(); // ���� TempAnim �÷��� ���̸� �⺻ DefaultPose ���� transition �� ���.

	// TouchAnim ������ �÷����ϴ� �ִϸ��̼� ���¸� ĳ���� �д�. 
	if (CachedCurrPlayingTempAnim) {
		CachedPreTouchAnim = CachedCurrPlayingTempAnim;
	}
	else { // DefaultPose �� ���ư� �� CachedCurrPlayingTempAnim �� null-out �Ǿ�����.
		CachedPreTouchAnim = DefaultPose;
	}
	USkeletalMeshComponent* SKComp = GetSkeletalMeshComponent();
	if (CachedPreTouchAnim && SKComp)
	{
		if (bWasInTempAnimEndTransition)
		{
			// �� ������ ������ blending �� �ϴ� �ٶ��� SKComp->GetPosition() �� 0 �� �����Ƿ� LastPreTouchAnimPlayedPos �� ������..
			// TempAnimEndTransitionTime ���� ������ ��Ȯ�� position �� ����� �� �� �ֱ� �ѵ�.. TickForBlendedAnimTransition ������ �����;� ��.
			// �� �������� �� ������ ������ �߰� ������������ ����.. �� ������ ��κ��� ��Ȳ���� ũ�� ���� �� ��� �̷��� ����.
			LastPreTouchAnimPlayedPos = FMath::Max((CachedPreTouchAnim->GetPlayLength() - TempAnimEndTransitionTime * 0.5f), 0.0f);
		}
		else
		{
			LastPreTouchAnimPlayedPos = SKComp->GetPosition();
		}
	}

	OnPlayReactionOnTouch(); // ���⼭ ���� TouchAnim �� ���� Play �� ȣ���� �־�� �Ѵ�.
	
	UWorld* TheWorld = GetWorld();
	if (CachedPreTouchAnim && CachedCurrPlayingTempAnim && SKComp && TheWorld)
	{
		FTimerManager& WTM = TheWorld->GetTimerManager();

		WTM.ClearTimer(TouchAnimTransitionTH);
		WTM.SetTimer(TouchAnimTransitionTH, this, &AB2LobbyDoumiGirlBase::TouchAnimTransitionCB, TouchAnimTransitionTime, false);
		// TouchAnimTransitionTime �� �� ������ ���� ����.
		bInTouchAnimTransition = true;
		CurrTouchAnimTransitionTimeCounter = 0.0f;
	}
}

void AB2LobbyDoumiGirlBase::TouchAnimTransitionCB()
{
	check(!bInTempAnimEndTransition); // �ִϸ��̼� transition ������ ���� ���� ���� ���� ȥ�յ��� �ʵ���..

	bInTouchAnimTransition = false;
	CurrTouchAnimTransitionTimeCounter = 0.0f;
	CachedPreTouchAnim = nullptr;

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TouchAnimTransitionTH);
	}

	OnTouchAnimTransitionCB(); // TouchAnimTransitionTime ��ŭ�� ��ġ���� �ٽñ� Touch ����� �÷���.
}

void AB2LobbyDoumiGirlBase::QuitTouchAnimTransitionState()
{
	// �Ϲ����� TouchAnim �ڵ鸵 ���߿� ���� ������ �ƴϰ�, �ٸ� �ִϸ��̼� �÷��� ������ ���� Ŭ��� Ȯ���� �ϰ��� �ϴ� ��.

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TouchAnimTransitionTH);
	}
	bInTouchAnimTransition = false;
	CurrTouchAnimTransitionTimeCounter = 0.0f;
	//CachedPreTouchAnim = nullptr; ���⼭ null-out �ϱ� ������ �� ������ CachedPreTouchAnim �� ������ ���Ŀ� �̰� ���ϴ� ��Ȳ�� �־ �̰� ����.
}

void AB2LobbyDoumiGirlBase::OnPlayReactionOnTouch()
{
	// TouchAnim �� ����Ϸ��� Overriding �ʿ�.
	// ���� TouchAnim �� ���� Play �� ȣ���� �־�� �Ѵ�.
	bCurrPlayTouchAnim = true;
}

void AB2LobbyDoumiGirlBase::OnTouchAnimTransitionCB()
{
	// TouchAnim �� ����Ϸ��� Overriding �ʿ�.
	// TouchReaction ���� ���� �帥 �ð�(TouchAnimTransitionTime)��ŭ�� ��ġ���� �ٽñ� Touch ����� �÷��� �ϴ� �� �ʿ�.
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

			// ��δ� �ƴ����� ��κ��� �ִϸ��̼� �÷��� ȣ���� ���� ��.
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

/** ShopGirl �� RelaxingDoumi ���� �ݺ� �ڵ� �ּ�ȭ ���ø� ���� */
template<typename TStateEnum>
UAnimSequenceBase* GetLobbyNPCAnimSeqOfStateFromTMapTempl(TMap<TStateEnum, TSoftObjectPtr<UAnimSequenceBase>>& InStateAnimMap, TStateEnum InState)
{
	TSoftObjectPtr<UAnimSequenceBase>* FoundTSoftObjectPtrPtr = InStateAnimMap.Find(InState);
	if (FoundTSoftObjectPtrPtr)
	{
		TSoftObjectPtr<UAnimSequenceBase>& FoundTSoftObjectPtr = *FoundTSoftObjectPtrPtr;
		if (!FoundTSoftObjectPtr.IsNull())
		{ // �ܼ� LoadSynchronous �� ���.
			// �̰� ��ü�� Async �ε� ��ȹ�̳� �ٸ� asset �� async �ε��� ���� ������ ���ٴ� �Ǵ� �Ͽ� ����ϴ� ����.
			return FoundTSoftObjectPtr.LoadSynchronous();
		}
	}
	return nullptr;
}


/************************************************************************/
/* 
 AB2LobbyShopGirl
 �������� �ָӴϸ� �ʹ� �� �ϰ����� �ִ� ���� ���� �ҳ�
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
	{ // Idle �� �׳� DefaultPose.. ��� Idle �� �÷����ϴ� ���� �������ؼ� BeginDefaultPose �� �ٷ� ���..
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

	// CachedCurrPlayingTempAnim �� Touch ����� �Ǿ��..
	// CachedPreTouchAnim �̶� CachedCurrPlayingTempAnim �� ���� ���� �ִ�. ���޾� ��ġ�� �ϸ�
	check(CachedPreTouchAnim && (GetAnimSeqOfState(ELobbyShopGirlState::ELSG_TouchReaction) == CachedCurrPlayingTempAnim));
}
void AB2LobbyShopGirl::OnTouchAnimTransitionCB()
{
	// TouchAnim �� ���ݱ��� �÷����� ��ġ���� �����ؾ� �ؼ� �ٽ� �� �� ��..
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
 �������� �Ѵ� �� ƴ�� ���� �ʴ� �ʹ���ź �޽� �����
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
	{ // Idle �� �׳� DefaultPose.. ��� Idle �� �÷����ϴ� ���� �������ؼ� BeginDefaultPose �� �ٷ� ���..
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

	// CachedCurrPlayingTempAnim �� Touch ����� �Ǿ��..
	// CachedPreTouchAnim �̶� CachedCurrPlayingTempAnim �� ���� ���� �ִ�. ���޾� ��ġ�� �ϸ�
	check(CachedPreTouchAnim && (GetAnimSeqOfState(ELobbyRelaxingDoumiState::ELRD_TouchReaction) == CachedCurrPlayingTempAnim));
}
void AB2LobbyRelaxingDoumi::OnTouchAnimTransitionCB()
{
	// TouchAnim �� ���ݱ��� �÷����� ��ġ���� �����ؾ� �ؼ� �ٽ� �� �� ��..
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
	//	if (LobbyMainUIPage->GetCombineMenuIsVisible()) // ���ո޴��� ���ִ� ���¿����� ���ȵǰԲ�
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
//	������ ī�� �ӽ� �۾�
	//SealBoxCardFlipClass<int32, int32>::GetInstance().Signal(1,1);
}