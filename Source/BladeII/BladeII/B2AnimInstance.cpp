// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2AnimInstance.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"

#include "BladeIITestDummyPlayer.h"
#include "BladeIIPlayer.h"
#include "BladeIIGameMode.h"



void UB2AnimInstance::NativeInitializeAnimation()
{
	if (OwnerPlayer == nullptr)
	{
		bool bExpectedSituation = false;
		OwnerPlayer = Cast<ABladeIIPlayer>(GetOwningActor());

		if(OwnerPlayer)
		{
			UE_LOG(LogB2Animation, Log, TEXT("B2AnimInstance::NativeInitializeAnimation - AnimInstance Initialized"));
		}

		else
		{
			/* Pre-render �������� BladeIIPlayer �� �ƴ� ���� SkeletalMesh �� AnimBlueprint �� ���̴� ��찡 ������ �׷� ��� OwnerPlayer �� null �� �� ��.
			* Pre-render ������ �ƴ� �Ϲ� Pre-render �ÿ��� �׷��� �� �� �ִ� ��Ʈ�� �ֱ� ������ ���� �ǵ��ϴ� �������δ� ���ɼ��� ���� by djsong */
			
			auto* GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
			if (GameMode && GameMode->IsInPreRenderPhase())
				bExpectedSituation = true;

			if (GIsEditor)
				bExpectedSituation = true;

			// �� ��Ȳ�� OwnerPlayer�� nullptr�̾ �������� ��Ȳ���� ����
		}

		BII_CHECK(OwnerPlayer || bExpectedSituation);
	}

	if (OwnerPlayer)
	{
		OwnerPlayer->BP_ConfigMinDashDistance = cfg_DashMinDistance;
		OwnerPlayer->BP_ConfigMaxDashDistance = cfg_DashMaxDistance;

		static const FName DefaultMachineName(TEXT("New State Machine"));
		RegisterStateChangeDelegate(DefaultMachineName);

		/*
		if(IsOwnerAuthority() == false)
			RegisterNativeTransition(DefaultMachineName);
		*/
	}
}

void UB2AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerPlayer == nullptr)
	{
		UE_LOG(LogB2Animation, Warning, TEXT("B2AnimInstance::NativeUpdateAnimation - OwnerPlayer is null"));
		return;
	}

	/*if (OwnerPlayer->IsRemoteCharacter())
	{
		SetInQTEState(OwnerPlayer->IsInQTEState());
		return;
	}*/

	// step1 - Copying SpawnMotionState for state machine use
	SpawnMotionState = OwnerPlayer->GetSpawnMotionState();

	// step2 - �׾��ٸ� ���̻��� AnimUpdate �������� ���� - StateMachine���� �ڵ� Dead��Ʈ�� �귯��
	if (ProcessDying() == false)
	{
		AttackDir = static_cast<int32>(OwnerPlayer->TargetDirection);

		// step3 - AbnormalState ���� Ȯ��
		UpdateAbnormalState();

		// step4 - QTE ���� ����
		UpdateQTEState();

		// step5 - Counter ���� Ȯ��
		const bool IsCounter = ProcessCounterHandling();
		if (IsCounter)
			return;

		// step5_1 - (Counter ���� �� Guard���� �ƴ� �� - Update Damage) / (Guard�� DamageȮ�� X)
		const bool NeedToUpdateDamage = !IsGuarding();
		if (NeedToUpdateDamage)
			UpdateDamageState();

		// step6 - ComboEnd ���� Ȯ��
		const bool IsComboEnded = ProcessComboEnd();

		// step7 - ComboEnd�ʹ� �����ϰ� RunningState �� �׻� Update
		UpdateRunning();

		if (IsComboEnded) // ComboEnd�� ��� ������ Attack(Skill) & Dash/Warp & Guard�� Animation ������ �ʿ����
			return;

		// step8 - Skill ��������� Ȯ��
		const bool IsSkillPlay = ProcessSkill();
		if (IsSkillPlay) // Skill��� �� ���� Anim���� �ʿ����
			return;

		// step9 - Guard or Evade �� ���� ���� ����
		const bool IsGuardState = ProcessGuard();
		if (IsGuardState)
			return;

		// step9 - Combo ���� Ȯ��
		if (IsComboNext() && !bDamaged)
		{
			// step 9-1 Dash/warp �������� ����
			UpdateDashOrWarpEnable();

			// Final
			// step 9-2 Combo Handling
			UpdateComboNext();
		}

		else
		{
			UpdateBeginAttack();
		}
	}
}

void UB2AnimInstance::NativeUninitializeAnimation()
{

}

bool UB2AnimInstance::ProcessDying()
{
	SetDead(OwnerPlayer->bIsDying);
	if (IsDead())
	{
		OwnerPlayer->GetCharacterMovement()->StopMovementImmediately();
		ProcessComboEnd();

		return true;
	}

	return false;
}

bool UB2AnimInstance::ProcessComboEnd()
{
	const bool IsPlayerInputEnable = OwnerPlayer->GetHadInput() || OwnerPlayer->GetReceiveNetMotionCancleSignal();
	const bool IsPlayerOnQTEState = OwnerPlayer->IsQTEMounting();
	const bool IsMoveable = IsInputEnable() && (IsPlayerInputEnable && !IsPlayerOnQTEState);

	if (IsComboEnd() || IsMoveable || OwnerPlayer->ConsumeAndCheckStopAttackMotion())
	{
		SetComboEnabled(false);
		SetComboNext(false);
		SetInQTEState(false);
		SetQTEAttack(false);
		SetComboNextEnabled(false);
		SetComboReserved(false);
		SetComboEnd(false);
		SetComboSkill(false);
		SetWarpEnabled(false);
		SetDashEnabled(false);
		SetGuarding(false);
		SetInputEnable(false);
		SetSkillCanceled(false);
		SetSmash(false);
		SetComboNextThisFrame(false);
		SetHadRestartAttack(true);
		SetLastComboEndTime(GetGameTime());
		SetAnimAttackState(EAttackState::ECS_None);
		SetGuardBreakLogic(EGuardBreakLogicType::EGLT_AbleToGuard);

		const EAttackState LastAttackState = OwnerPlayer->GetAttackState();
		OwnerPlayer->ReceiveComboEnd(LastAttackState, IsMoveable);
		OwnerPlayer->OnProcessComboEnd();

		return true;
	}

	return false;
}

void UB2AnimInstance::SetDamageAnimIndex(EAttackType DamageAttackType)
{
	DamageAnimIndex = DamageAttackType;
}

void UB2AnimInstance::SetForceRotateInRootMotion(bool bForce)
{
	bForceRotateInRootMotion = bForce;
}

bool UB2AnimInstance::StartCustomAnim(UAnimSequenceBase * InAnimAsset)
{
	if (bStartCustomAnim && InAnimAsset == nullptr)
		return false;

	//custom animation ����� ������ Left State Event���� null�� �ʱ�ȭ ������
	CustomAnimSequence = InAnimAsset;
	bStartCustomAnim = true;

	return true;
}

bool UB2AnimInstance::IsOwnerAuthority()
{
//	if (OwnerPlayer && OwnerPlayer->Role == ROLE_Authority)
//		return true;

	return false;
}

void UB2AnimInstance::UpdateAbnormalState()
{
	const EAttackType CurrDmgAnimType = GetDamageAnimIndex();

	// HugeDamage �߿��� Stun������ �ȵ�
	// HugeDamageAnim�� ������ Stun �ð��� �����ִٸ� ����

	if (IsDamaged() && IsHugeDamage(CurrDmgAnimType))
	{
		SetStunned(false);
	}
	else
	{
		SetStunned(OwnerPlayer->IsStunned());
	}
}

void UB2AnimInstance::UpdateQTEState()
{
	SetInQTEState(OwnerPlayer->IsInQTEState());
	SetQTEStateNum(OwnerPlayer->QTEStateNum);
	SetQTEType(OwnerPlayer->QTEType);
	SetQTENPCClass(OwnerPlayer->QTENPCClass);
	SetQTERun(OwnerPlayer->bQTERun);

	if (IsInputEnable() && OwnerPlayer->GetHadInput() && OwnerPlayer->GetQTEInputEnableReady())
	{
		SetInputEnable(false);
		OwnerPlayer->OnQTEAttackEnd();
	}
}

void UB2AnimInstance::UpdateDamageState()
{
	const int32 OwnerDamageNum = OwnerPlayer->GetDamagedNum();
	const bool IsNewDamage = OwnerDamageNum > 0 && OwnerDamageNum != GetDamageNum();

	SetDamageNum(OwnerDamageNum);

	bool bIgnoreToggle = false;
	if (IsNewDamage)
	{
		EAttackType OrgDamageType = OwnerPlayer->LastDamageType;

		const EAttackType NewDamageType = OrgDamageType;
		const EAttackType CurrDamageType = GetDamageAnimIndex();

		const bool IsTinyNewDamage = IsTinyDamage(NewDamageType);
		const bool IsTinyCurrDamage = IsTinyDamage(CurrDamageType);

		const bool IsHugeNewDamage = IsHugeDamage(NewDamageType);
		const bool IsHugeCurrDamage = IsHugeDamage(CurrDamageType);

		const bool IsAbnormalState = OwnerPlayer->IsFrozen() || IsStunned(); 

		// -1. Huge Damage�� � ���¿����� �׻� ����
		if (IsHugeNewDamage)
		{
			SetDamageAnimIndex(NewDamageType);
			SetDamaged(true);

#if FORCE_LOGGING_IN_ANDROID
			if (OwnerPlayer->GetWorld())
			{
				float GameTime = OwnerPlayer->GetWorld()->GetTimeSeconds();
				FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLog HugeDamage On : %d / time : %f threadId : %d\n"), static_cast<int32>(NewDamageType), GameTime, FPlatformTLS::GetCurrentThreadId());
			}
#endif
		}

		// -2. Tiny Damage�� ��Ȳ�� ����
		else if (IsTinyNewDamage)
		{
			// 2-1. ����/���� or HugeDamage ���´� TinyDamage ����
			const bool IgnoreTinyDamage = IsAbnormalState || IsHugeCurrDamage;
			if(IgnoreTinyDamage == false)
			{
				// 2-2���� TinyDamage ���̸� ���� TinyDamage ��� - ex) KnockUp ��� �� KnockDown �¾Ƶ� Knockup ���
				if (IsTinyCurrDamage)
				{
					SetDamageAnimIndex(CurrDamageType);
					SetDamaged(true);
				}

				// 2-3 ó�� �´� Damage�� �ǰ� DamageAnim ���
				else if (IsDamaged() == false)	// �ʿ������ ��Ȯ �ϰ� �ϱ� ���� ���ǹ�
				{
					SetDamageAnimIndex(NewDamageType);
					SetDamaged(true);
				}
			}

			bIgnoreToggle = IgnoreTinyDamage;
		}

		else if (NewDamageType == EAttackType::EAT_Default)
			bIgnoreToggle = true;
	}

	if (IsNewDamage && IsDamaged() && bIgnoreToggle == false)
		SetDamageToggle(IsDamageToggle() ^ true);

	SetGuardBreakLogic(OwnerPlayer->LastGuardBreakType);

	const EAttackState OwnerAttackState = OwnerPlayer->GetAttackState();
	const bool IsNormalAttack = OwnerAttackState >= EAttackState::ECS_Combo01 && OwnerAttackState < EAttackState::ECS_Skill01;
	const bool IsEvade = OwnerAttackState == EAttackState::ECS_Evade;

	const bool NeedToCancelCombo = IsDamaged() && (IsNormalAttack || IsEvade);
	const bool bForceCancelCombo = OwnerPlayer->IsForceComboEnd();

	if (NeedToCancelCombo || bForceCancelCombo)
	{
		if (OwnerPlayer->IsFrozen() == false) // Freezing�� ClearBuff�� �ڵ� ȣ��
		{
			SetComboEnd(true);
			OwnerPlayer->ConsumeComboEnd();
		}
	}

	if (IsDamaged() == false)
	{
		const bool HitByNonAnimDamage = GetDamageNum() > 0 && GetDamageAnimIndex() == EAttackType::EAT_Default;
		if (HitByNonAnimDamage)
			UpdateDamageEnd();
	}
}

bool UB2AnimInstance::ProcessCounterHandling()
{
	const auto CurrentState = GetAnimAttackState();

	const bool IsGuardingState = (CurrentState == EAttackState::ECS_Guard) || (CurrentState == EAttackState::ECS_GuardStart);
	const bool IsCounterTime = (GetGameTime() - OwnerPlayer->GetLastGuardTime()) < OwnerPlayer->CounterCheckTime;
	const bool IsCountable = IsGuardingState && IsCounterTime && !OwnerPlayer->IsFrozen();

	const bool IsForceCounter = OwnerPlayer->GetForceCounterAttack();

	/*
	// ���� �������� ��츦 ���� Ư��ó��

	1. [  n-1  ] Tick::ProcessCounterHandling() ���� GuardStart �����̹Ƿ� AttackState�� ECS_Counter01�� ����
	2. [n-1 ~ n] Tick ���̿� GuardStart AnimSequence�� ComboNext Notify�޾� bComboNext = true�� ����
	3. [   n   ] Tick::UpdateComboNext���� ���� AttackState�� ECS_Counter01�̹Ƿ� ECS_Counter02�� IncreaseAttackState ��

	���� Guard_Start AnimSequence�� bComboNext Notify�� ���� ���� Counter�� �߻� �ߴٸ� CounterReserved�� �ɾ� ����
	���� ī���ʹ� [n] ��° Tick�� ����Ǿ� Guard_Start -> Guard_loop( EAttackState::ECS_Guard ) ���¿� �� �� �ߵ��ϵ��� �Ѵ�.
	*/

	if (IsCountable || IsForceCounter || IsCounterReserved())
	{
		SetGuarding(true);

		const int32 OwnerDamagedNum = OwnerPlayer->GetDamagedNum();
		const bool IsNewDamaged = OwnerDamagedNum > 0 && OwnerDamagedNum != GetDamageNum();
		if (IsForceCounter || IsNewDamaged || IsCounterReserved())
		{
			if (CurrentState == EAttackState::ECS_GuardStart)
			{
				SetCounterReserve(true);
				return false;
			}

			OwnerPlayer->OnCounterStart();

			EAttackState CounterAttackState = EAttackState::ECS_Counter01;
			if (bUseOneCounterAnim)
			{
				const int32 CounterMotionInc = static_cast<int32>(OwnerPlayer->GetCounterMotionIncrease());
				const int32 CastedCounterState = static_cast<int32>(EAttackState::ECS_Counter01);

				CounterAttackState = static_cast<EAttackState>(CounterMotionInc * 2 + CastedCounterState);
			}

			SetAnimAttackState(CounterAttackState);
			UpdateDamageEnd();

			if (IsCounterReserved())
				SetCounterReserve(false);

			return true;
		}
	}

	return false;
}

bool UB2AnimInstance::ProcessSkill()
{
//	//const bool IsSkillAttacking = OwnerPlayer->HasSkillAttackSignal();
//	const EAttackState OwnerSkillState = OwnerPlayer->GetReservedSkill();
//	
//	if (OwnerSkillState != EAttackState::ECS_None && IsComboSkill() == false)
//	{
//		SetSkillAnimType(OwnerPlayer->GetCurrentSKillAnimIdx());
//		SetComboSkill(true);
//		SetAnimAttackState(OwnerSkillState);
//
//		if (GetSpawnMotionState() == EPCSpawnMotionState::EPST_Normal)
//		{
//			// �̰����ָ� ��Ʈ������ �ٽ�ã�ư�. ��ų->��ų ������Ʈ ��ȯ�� �ʿ��ؼ�
//			GetProxyOnGameThread<FAnimInstanceProxy>().InitializeRootNode();
//		}
//		
//#if FORCE_LOGGING_IN_ANDROID
//		if (OwnerPlayer->GetWorld())
//		{
//			float GameTime = OwnerPlayer->GetWorld()->GetTimeSeconds();
//			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLog ProcessSkill On : %d / time : %f threadId : %d\n"), static_cast<int32>(OwnerSkillState), GameTime, FPlatformTLS::GetCurrentThreadId());
//		}
//#endif
//
//		return true;
//	}
//
//	//const EAttackState OwnerAttackState = OwnerPlayer->GetAttackState();
//	if (OwnerSkillState == EAttackState::ECS_TagOtherChar)
//	{
//		SetAnimAttackState(OwnerSkillState);
//
//		return true;
//	}

	return false;
}

bool UB2AnimInstance::ProcessGuard()
{
	// ------  ��ƽ�� ���� �������� ��Ȳ�� �߻�... �ش����� ����ó��
	static const FName GuardStart(TEXT("Guard_Start"));
	if (CurrentAnimState == GuardStart && GetAnimAttackState() == EAttackState::ECS_Combo01)
	{
		OwnerPlayer->StopRepeatingInput();
		OwnerPlayer->SetAttackState(EAttackState::ECS_GuardEnd);
		SetAnimAttackState(EAttackState::ECS_GuardEnd);
	}
	// ------  ��ƽ�� ���� �������� ��Ȳ�� �߻�... �ش����� ����ó��




	// �Ϲ� �����߿��� Guard Enable Timing�� ���� ����
	const bool InNormalAttack = GetAnimAttackState() >= EAttackState::ECS_Combo01 && GetAnimAttackState() <= EAttackState::ECS_Combo06;
	if (InNormalAttack && IsAllowGuardStart() == false)
		return false;

	const EAttackState OwnerState = OwnerPlayer->GetAttackState();
	const bool IsGuardStart = IsGuarding() == false && OwnerState == EAttackState::ECS_GuardStart;
	const bool IsGuardEnd = IsGuarding() && GetAnimAttackState() < EAttackState::ECS_Evade && OwnerState == EAttackState::ECS_GuardEnd;
	const bool IsEvade = GetAnimAttackState() < EAttackState::ECS_Evade && OwnerState == EAttackState::ECS_Evade;
	if (IsGuardStart)
	{
		const bool InAttackMotion = IsNormalAttackState(GetAnimAttackState());
		
		// ������ GuardEnable Noti ������ Cancel �Ұ���
		if (InAttackMotion && IsAllowGuardStart() == false)
			return false;

		SetGuarding(true);
		SetAnimAttackState(EAttackState::ECS_GuardStart);
		SetLastGuardTime(GetGameTime());
		OwnerPlayer->SetAttackState(EAttackState::ECS_GuardStart);
	}
	else if(IsGuardEnd)
	{
		SetGuarding(false);
		SetAnimAttackState(EAttackState::ECS_GuardEnd);
		OwnerPlayer->SetAttackState(EAttackState::ECS_GuardEnd);
	}
	else if (IsCounterDungeon() == false && IsEvade)	// Owner->IsAvailableEvade�� Ȯ���ؾ���
	{
		SetGuarding(false);
		SetAnimAttackState(EAttackState::ECS_Evade);
		OwnerPlayer->SetAttackState(EAttackState::ECS_Evade);
	}

	return IsGuardStart || IsEvade;
}

void UB2AnimInstance::SimulateAnimation()
{
	static const FName DefaultStateMachine(TEXT("New State Machine"));
	RegisterNativeTransition(DefaultStateMachine);
}

void UB2AnimInstance::SetReplicatedState(const FString& State)
{
	const FName ReceivedState = *State;
	ReplicatedState = ReceivedState;
}

void UB2AnimInstance::SetAnimAttackState(EAttackState State, bool bForce /*= false*/)
{
	// hakgu87 test - AttackState Simulating ���� ��
//	if (IsOwnerAuthority() || bForce)
	{
		AttackState = State;
		OwnerPlayer->SetAttackState(State);
	}
}

void UB2AnimInstance::UpdateDamageEnd()
{
	OwnerPlayer->OnDamageEnd();

	SetDamageNum(0);
	SetDamaged(false);
	SetDamageAnimIndex(EAttackType::EAT_Default);
}

void UB2AnimInstance::UpdateRunning()
{
	if (OwnerPlayer->CustomTimeDilation > 0.f)
	{
		if (OwnerPlayer->CanPlayerHaveMoveMotion())
		{
			const FVector& OwnerVelocity = OwnerPlayer->GetVelocity();
			const FVector& OwnerInputVector = OwnerPlayer->GetLastMovementInputVector();

			SetCurrentSpeed(OwnerVelocity.Size());

			const float InputVelocity = OwnerInputVector.Size();
			if (InputVelocity > 0.f)
				SetCurrentSpeed(InputVelocity);

			const bool IsNotAttack = (GetAnimAttackState() == EAttackState::ECS_None) && !IsComboSkill();
			if (IsNotAttack && OwnerVelocity.Size() > cfg_RunningSpeed)
				SetRunning(true);
		}
		else
		{
			SetCurrentSpeed(0.f);
		}
	}
}

void UB2AnimInstance::UpdateDashOrWarpEnable()
{
	if (IsCounterDungeon())
		return;

	const float TargetDist = OwnerPlayer->GetTargetDistance();
	const bool IsDashMinDist = TargetDist > cfg_DashMinDistance;
	const bool IsWarpMaxDist = TargetDist < cfg_WarpMaxDistance;
	
	if (IsDashMinDist && IsWarpMaxDist && bAllowedDash)
	{
		SetDashEnabled(true);
		SetWarpEnabled(TargetDist > cfg_DashMaxDistance);
	}
}

void UB2AnimInstance::UpdateComboNext()
{
	SetComboNext(false);

	const EAttackState OwnerState = OwnerPlayer->GetAttackState();

	const bool IsSkillAttacking = GetAnimAttackState() >= EAttackState::ECS_Skill01 && GetAnimAttackState() < EAttackState::ECS_GuardStart;
	if (IsSkillAttacking)
		return;

	if (GetDamageAnimIndex() == EAttackType::EAT_Default && GetDamageNum() > 0)
		UpdateDamageEnd();

	const bool IsInDash = GetAnimAttackState() == EAttackState::ECS_Dash;
	const bool IsAvailableNextCombo = IsComboNextEnabled() || GetAnimAttackState() == EAttackState::ECS_None || GetAnimAttackState() == EAttackState::ECS_GuardEnd;
	const bool IsAttack = OwnerPlayer->GetSignalAttack() || IsComboReserved();
	const bool HasComboReserved = IsAvailableNextCombo && IsAttack;

	// Combo ������ ������ ��Ȳ - �ణ�����ϴ�
	// case 1. Dash ��
	// case 2. Idle Ȥ�� GuardEnd ����
	// case 3. ���� ������ ���¿��� AttackSignal�� �޾��� ��
	// case 4. Guard ��

	const bool IsAvailableComboHandling = IsInDash || HasComboReserved || IsGuarding();
	if (IsAvailableComboHandling == false)
		return;

	if (IsDashEnabled())
	{
		if (GetAnimAttackState() == EAttackState::ECS_GuardEnd || GetAnimAttackState() == EAttackState::ECS_None)
			SetAnimAttackState(EAttackState::ECS_Dash);

		else
			IncreaseAttackState(1); // �Ѵܰ� AttackState ���
	}

	else
	{
		const int32 CastedAttackState = static_cast<int32>(GetAnimAttackState());

		const bool IsCounter = GetAnimAttackState() >= EAttackState::ECS_Counter01;
		const bool IsCounterWithoutDash = IsCounter && ((CastedAttackState % 2) == 0);

		if (GetAnimAttackState() == EAttackState::ECS_None || GetAnimAttackState() == EAttackState::ECS_GuardEnd)
			SetAnimAttackState(EAttackState::ECS_Combo01);

		else if (IsCounterWithoutDash)
			IncreaseAttackState(2); // Counter0xDash�� �ǳʶ��� �ϹǷ� 2�ܰ� ���

		else
			IncreaseAttackState(1); // ���� ��츦 �����ϰ� 1�ܰ� ���
	}

	// ------ ���� AnimAttackState�� ������ ���� ------ //

	const int32 CastedCounterAttack = static_cast<int32>(EAttackState::ECS_Counter01);
	const int32 OwnerMotionIncrease = OwnerPlayer->GetCounterMotionIncrease() * 2;

	const EAttackState OwnerIncreasedAttackState = static_cast<EAttackState>(CastedCounterAttack + OwnerMotionIncrease);
	if (GetAnimAttackState() > OwnerIncreasedAttackState)
		SetAnimAttackState(OwnerIncreasedAttackState);

	OwnerPlayer->bComboEnabled = false;
	SetComboEnabled(false);
	SetComboNextEnabled(false);
	SetComboReserved(false);
	SetDashEnabled(false);
	SetInputEnable(false);
	SetComboNextThisFrame(false);
	SetHadRestartAttack(false);

	// hakgu87 test - AttackState Simulating ���� ��
	//if (OwnerPlayer->Role == ROLE_Authority)
		OwnerPlayer->SetAttackState(GetAnimAttackState());
}

void UB2AnimInstance::UpdateBeginAttack()
{
	// DummyPlayer ����ó��
	if (auto* DummyPlayer = Cast<ABladeIITestDummyPlayer>(OwnerPlayer))
	{
		SetAnimAttackState(DummyPlayer->GetDirectAttackState());
		return;
	}

	const bool HasAttackSignal = OwnerPlayer->GetSignalAttack();
	const bool NoneAttackSignal = OwnerPlayer->GetSignalAttack() == false && IsComboNextEnabled() == false;

	SetHadRestartAttack(NoneAttackSignal || HasRestartAttack());

	const bool ReadyforAttack = IsComboReserved() || HasAttackSignal;
	const bool IsAvailableAttackState = OwnerPlayer->bComboEnabled || GetAnimAttackState() == EAttackState::ECS_None || GetAnimAttackState() == EAttackState::ECS_GuardEnd;

	const bool IsNormalAttack = ReadyforAttack && IsAvailableAttackState;
	if (IsNormalAttack && !IsInDamageAnimation())
	{
		const bool NewComboReserved = (IsComboReserved() || IsNormalAttack) && ((HasAttackSignal && IsComboNextEnabled()) || HasRestartAttack());
		SetComboReserved(NewComboReserved);
		SetSmash((IsComboNextEnabled() && !IsComboNextThisFrame()) || IsSmash());
		OwnerPlayer->SetSignalAttack(false);

		SetComboNext(true);
	}

	SetComboNextThisFrame(false);
}

void UB2AnimInstance::OnSpawnEnd()
{
	SetIsSpawnEnd(true);
	OwnerPlayer->EndSpawnMotionState();

	//if(OwnerPlayer->Role == ROLE_Authority)
	//	OwnerPlayer->ConsumeReservedSkill();

	SetComboEnd(true);
}

void UB2AnimInstance::OnDeadEnd()
{
	OwnerPlayer->NotifyPlayerDeadEnd();
}

void UB2AnimInstance::IncreaseAttackState(int32 IncreaseLevel/*= 1*/)
{
	const int32 CurrAttackState = static_cast<int32>(GetAnimAttackState());
	const int32 NewAttackState = CurrAttackState + IncreaseLevel;

	if (NewAttackState < static_cast<int32>(EAttackState::ECS_End))
		SetAnimAttackState(static_cast<EAttackState>(NewAttackState));

	else
		SetAnimAttackState(EAttackState::ECS_None);	// AttackState ������ ������ none���� �ʱ�ȭ
}

bool UB2AnimInstance::IsNormalAttackState(EAttackState InAttackState)
{
	return InAttackState >= EAttackState::ECS_Dash && InAttackState <= EAttackState::ECS_Combo06;
}

bool UB2AnimInstance::IsInDamageAnimation()
{
	if (IsDamaged() && GetDamageAnimIndex() > EAttackType::EAT_Default)
		return true;

	return false;
}

bool UB2AnimInstance::NativeTransitionFunc(const FName& PrevState, const FName& NextState)
{
	return ReplicatedState == NextState;
}

void UB2AnimInstance::OnStateChangeEntry(const struct FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex)
{
	FName PrevState(TEXT("None"));
	FName NextState(TEXT("None"));

	if(PrevStateIndex != INDEX_NONE)
		PrevState = Machine.GetStateInfo(PrevStateIndex).StateName;

	if (NextStateIndex != INDEX_NONE)
		NextState = Machine.GetStateInfo(NextStateIndex).StateName;

	OnAnimStateChanged(PrevState, NextState);
	
	// ���� �ǵ��� ����
	CurrentAnimState = NextState;
	// ���� �ǵ��� ����

	// Authority�� ReplicatedState ������� ����(NativeTransition ������� ����) - Console Ȯ��������
	if (IsOwnerAuthority())
		SetReplicatedState(NextState.ToString());
}

void UB2AnimInstance::OnAnimStateChanged(const FName& PrevState, const FName& NewState)
{
	SetAllowGuardStart(false);
	

	UE_LOG(LogB2Animation, Log, TEXT("AnimState Changed - %s -> %s"), *PrevState.ToString(), *NewState.ToString());

	OwnerPlayer->OnExitAnimState(PrevState);
	OwnerPlayer->OnEnterAnimState(NewState);

#if FORCE_LOGGING_IN_ANDROID
	if (OwnerPlayer->GetWorld())
	{
		float GameTime = OwnerPlayer->GetWorld()->GetTimeSeconds();
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("SHTestLog AnimStateChanged : %s / time : %f threadId : %d\n"), *NewState.ToString(), GameTime, FPlatformTLS::GetCurrentThreadId());
	}
#endif
}

void UB2AnimInstance::RegisterStateChangeDelegate(const FName& MachineName)
{
	const auto* MachineInst = GetStateMachineInstanceDesc(MachineName);
	if (MachineInst == nullptr || MachineInst->States.Num() == 0)
	{
		UE_LOG(LogB2Animation, Warning, TEXT("B2AnimInstance::RegisterStateChangeDelegate Failed"));
		return;
	}

	for (auto& BakedState : MachineInst->States)
	{
		const FName ValidStateName = BakedState.StateName;
		AddNativeStateEntryBinding(MachineName, ValidStateName, FOnGraphStateChanged::CreateUObject(this, &UB2AnimInstance::OnStateChangeEntry));
	}
}

void UB2AnimInstance::RegisterNativeTransition(const FName& MachineName)
{
	const auto* MachineInst = GetStateMachineInstanceDesc(MachineName);
	if (MachineInst == nullptr)
	{
		UE_LOG(LogB2Animation, Warning, TEXT("B2AnimInstance::RegisterNativeTransition Failed"));
		return;
	}

	if (MachineInst->States.Num() == 0 || MachineInst->Transitions.Num() == 0)
	{
		UE_LOG(LogB2Animation, Warning, TEXT("B2AnimInstance::RegisterNativeTransition has Invalid Attributes"));
		return;
	}

	TArray<FName> IgnoreStates = { TEXT("PreTag"), TEXT("Tag_Spawn") };

	const auto& States = MachineInst->States;
	for (auto& Transition : MachineInst->Transitions)
	{
		if (States.IsValidIndex(Transition.PreviousState) && States.IsValidIndex(Transition.NextState))
		{
			const FName& PrevStateName = States[Transition.PreviousState].StateName;
			const FName& NextStateName = States[Transition.NextState].StateName;

			if (IgnoreStates.Find(NextStateName))
				continue;

			//AddNativeTransitionBinding(MachineName, PrevStateName, NextStateName, FCanTakeTransition::CreateUObject(this, &UB2AnimInstance::NativeTransitionFunc));
		}
	}
}

const float UB2AnimInstance::GetGameTime() const
{
	if (auto* World = OwnerPlayer->GetWorld())
		return World->GetTimeSeconds();

	return 0.f;
}

bool UB2AnimInstance::IsCounterDungeon() const
{
	if (OwnerPlayer && OwnerPlayer->GetGameMode())
	{
		if (OwnerPlayer->GetGameMode()->GetB2GameModeType() == EB2GameMode::CounterDungeon)
			return true;
	}

	return false;
}
