// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeIITestDummyNPC.h"
#include "BladeIIGameMode.h"
#include "B2NPCClassInfoBox.h"
#include "B2BuffModeEffectInfo.h"
#include "B2DamageEffectInfo.h"
#include "B2CombatStatEvaluator.h"
#include "Event.h"

//
// 임의로 런타임 spawn 시켜서 대체로 성능 테스트 용으로 사용.
// ABladeIITestDummyPlayer 와 중복된 기능이 있을 수 있는데 정식 기능은 아닌 테스트 용이므로 웬만한 중복 코드는 넘어감.
// 

ABladeIITestDummyNPC::ABladeIITestDummyNPC(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	PrimaryActorTick.bCanEverTick = true;

	DirectAttackState = EAttackState::ECS_None;
	bUseRandomAttackState = false;

	bMaintainLocation = true;
	bMaintainRotation = true;

	CooltimeBetweenAnim = 0.0f;

	SpawnFloatingUIInterval = 0.0f;
	NextTimeSpawnFloatingUIInterval = 0.0f;
	TimeSinceLastSpawnFloatingUITime = 0.0f;

	SpawnDamageFxInterval = 0.0f;
	NextTimeSpawnDamageFxInterval = 0.0f;
	TimeSinceLastSpawnDamageFxTime = 0.0f;
#endif
}


void ABladeIITestDummyNPC::SetAttackState(EAttackState InAttackState)
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	const EAttackState LastStateBackup = GetAttackState();

	Super::SetAttackState(InAttackState);

	if (IsOneOfComboAttackState(LastStateBackup) && InAttackState == EAttackState::ECS_None)
	{ // BladeIICharacter 쪽은 AnimBPFnLib 을 통해 오는 ReceiveComboEnd 가 없어서 대략 이렇게.. 
		OnComboEnd(LastStateBackup);
	}
#endif
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2

void ABladeIITestDummyNPC::BeginPlay()
{
	Super::BeginPlay();

	// 더미로써 쓸모없는 것들 끄기.

}

void ABladeIITestDummyNPC::BeginDestroy()
{
	Super::BeginDestroy();
}

void ABladeIITestDummyNPC::Destroyed()
{
	Super::Destroyed();
}

void ABladeIITestDummyNPC::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
		
	// 사실상 여길 통해서 AttackState 로 전달되는 DirectAttackState 는 공격 상태가 되어야 할 듯.
	if (IsOneOfComboAttackState(DirectAttackState))
	{
		SetSignalAttack(true); // 기존 코드 구조 상 SetDirectAttackState 에서 단발성으로 SetSignalAttack 를 하는 걸로는 충분치 않은 듯 함.
		SetAttackState(DirectAttackState);
	}
	
	TimeSinceLastSpawnFloatingUITime += DeltaSeconds;
	// 실제로는 NextTimeSpawnFloatingUIInterval 을 본다. 매 턴마다 SpawnFloatingUIInterval 을 기준으로 조금씩 변동. 완전히 똑같은 interval 이면 여러마리 동시 spawn 시 일제히 나올 수도 있으므로
	if (SpawnFloatingUIInterval > 0.0f && TimeSinceLastSpawnFloatingUITime >= NextTimeSpawnFloatingUIInterval)
	{
		RandomSpawnFloatingUIs();
		TimeSinceLastSpawnFloatingUITime = 0.0f;
		NextTimeSpawnFloatingUIInterval = SpawnFloatingUIInterval * FMath::FRandRange(0.75f, 1.25f);
	}

	TimeSinceLastSpawnDamageFxTime += DeltaSeconds;
	// 여기도 마찬가지로 매 타임마다 랜덤으로 정해지는 NextTimeSpawnDamageFxInterval 을..
	if (SpawnDamageFxInterval > 0.0f && TimeSinceLastSpawnDamageFxTime >= NextTimeSpawnDamageFxInterval)
	{
		RandomSpawnDamageFx();
		TimeSinceLastSpawnDamageFxTime = 0.0f;
		NextTimeSpawnDamageFxInterval = SpawnDamageFxInterval * FMath::FRandRange(0.75f, 1.25f);
	}

	// Spawn 당시 지정한 위치에 고정. Z 값은 Spawn 당시에 막혀서 공중에 떠 있을 수 있으니 업데이트 된 값을 변경하지 않고 돔.
	if (bMaintainLocation)
	{
		SetActorLocation(FVector(InitialLocation.X, InitialLocation.Y, GetActorLocation().Z));
	}
	if (bMaintainRotation)
	{
		SetActorRotation(InitialRotation);
	}

}

bool ABladeIITestDummyNPC::CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType)
{
	return false;
}

bool ABladeIITestDummyNPC::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	return false;
}

void ABladeIITestDummyNPC::InitializeCombatStats()
{
	Super::InitializeCombatStats();

	// 공격 모션으로 Damage 입히지 못하게 함. 그렇지만 이것만 가지고는 플레이어가 공격에 맞았을 시 밀린다거나 하는 것까지 막진 못함.
	TotalAttack = 0.0f;
	AttackLevel = CombatStatEval::GetADLevel(TotalAttack, CombatStatEval::GetDefaultCombatCoefficient());
}

void ABladeIITestDummyNPC::OnComboEnd(EAttackState LastState)
{
	// 다음 애니메이션 시작.. 간격을 두거나 바로
	UWorld* TheWorld = GetWorld();
	if (CooltimeBetweenAnim > 0.0f && TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(CooltimeBetweenAnimTH,
			// RandomAttackState 인 경우 BeginNextAnimCommon 에 무엇을 넘겨주든 랜덤 선택될 것이지만 의미를 명확히..
			FTimerDelegate::CreateUObject(this, &ABladeIITestDummyNPC::BeginNextAnimCommon, (bUseRandomAttackState ? EAttackState::ECS_End : DirectAttackState)),
			CooltimeBetweenAnim, false);
		SetDirectAttackState(EAttackState::ECS_None); // 다음 애니메이션 발동 전까지 Idle 상태로 휴식.
	}
	else
	{
		BeginNextAnimCommon(); // 바로 시작. 현재 DirectAttackState 그대로 이어서.
	}
}

void ABladeIITestDummyNPC::RandomSpawnFloatingUIs()
{
	// NPC 에 HP 외에 FloatingUI 띄우는 게 있으려나.. 있더라도 그닥..?
	TestRandomSpawnFloatingUIs(this, 1.0f);
}

void ABladeIITestDummyNPC::RandomSpawnDamageFx()
{
	TestRandomSpawnDamageFx(this);
}

void ABladeIITestDummyNPC::BeginNextAnimCommon(EAttackState SpecifiedNextAnim /* RandomAttackState 가 아니고 EAttackState::ECS_End 외의 값을 줄 시 사용됨. */)
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(CooltimeBetweenAnimTH);
	}

	if (bUseRandomAttackState) // 랜덤 재생 시 다음 랜덤 애니메이션을 선택
	{
		GoToNextRandomAttackState();
	}
	else
	{ // 랜덤 애님 재생이 아니라면 지정된 걸로 다시 시작.
	  // 콤보 state 는 처음부터 다시 시작해야 해서 ThisTickFinalAttackState 까지 리셋이 된다.
		const EAttackState FinalNextAnim = (SpecifiedNextAnim != EAttackState::ECS_End) ? SpecifiedNextAnim : DirectAttackState;
		if (IsOneOfComboAttackState(FinalNextAnim))
		{
			SetDirectAttackState(FinalNextAnim);
		}
	}
}

void ABladeIITestDummyNPC::SetDirectAttackStateByKeyword(const FString& InAttackStateKeyword)
{
	// 콘솔 커맨드를 통해 AttackState 지정하는 기능을 쓰려고.

	if (InAttackStateKeyword == TEXT("None") || InAttackStateKeyword == TEXT("No"))
	{
		SetDirectAttackState(EAttackState::ECS_None);
	}
	else
	{
		int32 ParsedAttackIndex = FCString::Atoi(*InAttackStateKeyword);

		if (ParsedAttackIndex == 0) // Atoi 가 실패했든지 아님 0 을 지정했든지.
		{
			if (InAttackStateKeyword == TEXT("A1")) {
				ParsedAttackIndex = 1;
			}
			else if (InAttackStateKeyword == TEXT("A2")) {
				ParsedAttackIndex = 2;
			}
			else if (InAttackStateKeyword == TEXT("A3")) {
				ParsedAttackIndex = 3;
			}
			else if (InAttackStateKeyword == TEXT("A4")) {
				ParsedAttackIndex = 4;
			}
			else if (InAttackStateKeyword == TEXT("A5")) {
				ParsedAttackIndex = 5;
			}
			else if (InAttackStateKeyword == TEXT("A6")) {
				ParsedAttackIndex = 6;
			}
			else if (InAttackStateKeyword == TEXT("A7")) {
				ParsedAttackIndex = 7;
			}
			else if (InAttackStateKeyword == TEXT("A8")) {
				ParsedAttackIndex = 8;
			}
			else if (InAttackStateKeyword == TEXT("A9")) {
				ParsedAttackIndex = 9;
			}
		}

		// 실제 세팅된 AttackData 개수 이내로.
		AIAttackIndex = ParsedAttackIndex % AIAttackDataArray.Num();

		// NPC 들은 공격 상태는 하나만 사용.
		SetDirectAttackState(EAttackState::ECS_Combo01);
	}
}

void ABladeIITestDummyNPC::SetDirectAttackState(EAttackState InState)
{
	// 여기서 SetSignalAttack 을 부르는 것만으로 충분치는 않음..
	if (IsOneOfComboAttackState(DirectAttackState))
	{
		SetSignalAttack(true);
	}
	else
	{
		SetSignalAttack(false);
	}

	DirectAttackState = InState;
}

void ABladeIITestDummyNPC::SetUseRandomAttackState(bool bInRandomAttackState)
{
	bUseRandomAttackState = bInRandomAttackState;
	if (bUseRandomAttackState)
	{
		GoToNextRandomAttackState();
	}
}

void ABladeIITestDummyNPC::SetCooltimeBetweenAnim(float InCooltime)
{
	CooltimeBetweenAnim = InCooltime;
}
void ABladeIITestDummyNPC::SetMaintainLocation(bool bMaintain)
{
	bMaintainLocation = bMaintain;
	// InitialLocation 을 갱신하는 옵션이라도..?
}
void ABladeIITestDummyNPC::SetMaintainRotation(bool bMaintain)
{
	bMaintainRotation = bMaintain;
}

void ABladeIITestDummyNPC::GoToNextRandomAttackState()
{
	// AIAttackDataArray 에 등록된 개수만큼.
	AIAttackIndex = FMath::RandRange(0, AIAttackDataArray.Num() - 1);
	
	// NPC 들은 공격 상태는 하나만 사용.
	SetDirectAttackState(EAttackState::ECS_Combo01);
}

/** 사용을 위한 메인 유틸 */
ABladeIITestDummyNPC* ABladeIITestDummyNPC::SpawnUtil(UObject* WorldContextObject,
		ENPCClass InClass, ENPCClassVariation InVariation,
		const FTransform& InSpawnTransform,
		float InSpawnFloatingUIInterval, float InSpawnDamageFxInterval)
{
	if (!WorldContextObject) {
		return nullptr;
	}
	
	UB2NPCClassInfoBox* NPCInfoBox = StaticFindMobClassInfoBox(WorldContextObject);
	UWorld* TheWorld = WorldContextObject->GetWorld();
	if (TheWorld && NPCInfoBox)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = TheWorld->GetAuthGameMode();
		SpawnInfo.bDeferConstruction = true; // AutoPossessAI 설정 및 NPCClassInfo 적용을 위해 DeferConstruction 이 필요.
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.ObjectFlags |= RF_Transient;

		ABladeIITestDummyNPC* SpawnedDummy = NPCInfoBox ?
			NPCInfoBox->SpawnNPCClassInfoDummyCharacter(TheWorld, InClass, InVariation, InSpawnTransform, SpawnInfo) : nullptr;
		
		if (SpawnedDummy)
		{
			// AI possess 를 하지 않으니 결국 spawn 당시 상황에 따라 지면 snap 이 잘 안되는 것 때문에 AI possess 하고선 여타 제한을 가함.
			// 그리고 상황에 따라 플레이어를 따라오거나 회전 정도는 사용할 필요도 있을 수 있고 해서..
			SpawnedDummy->AutoPossessAI = EAutoPossessAI::Spawned;
			SpawnedDummy->FinishSpawning(InSpawnTransform);
			SpawnedDummy->bNeedSpawnCheck = false; // SpawnStay 가 있는 경우 넘기도록.
			SnapBladeIICharacterToGround(SpawnedDummy);
			// 현 위치에 고정시키기 위해 저장해 둠.
			SpawnedDummy->InitialLocation = SpawnedDummy->GetActorLocation();
			SpawnedDummy->InitialRotation = SpawnedDummy->GetActorRotation();

			if (SpawnedDummy->CenterShadow)
			{
				SpawnedDummy->SetupDecalCompForCenterShadowCommon(SpawnedDummy->CenterShadow, SpawnedDummy->GetMesh());
				SpawnedDummy->CenterShadow->SetDecalMaterial(SpawnedDummy->ShadowMaterial);
			}

			UCharacterMovementComponent* CharMoveComp = SpawnedDummy->GetCharacterMovement();
			if (CharMoveComp)
			{ // 움직일 여지를 최대한 제거.. 하려다가 Z 축은 움직일 필요가 있어서 다른 식으로.. ㅋ
				//CharMoveComp->SetUpdateRotationOnly(true);
			}

			SpawnedDummy->SpawnFloatingUIInterval = InSpawnFloatingUIInterval;
			SpawnedDummy->NextTimeSpawnFloatingUIInterval = InSpawnFloatingUIInterval;
			SpawnedDummy->TimeSinceLastSpawnFloatingUITime = 0.0f;

			SpawnedDummy->SpawnDamageFxInterval = InSpawnDamageFxInterval;
			SpawnedDummy->NextTimeSpawnDamageFxInterval = InSpawnDamageFxInterval;
			SpawnedDummy->TimeSinceLastSpawnDamageFxTime = 0.0f;


			return SpawnedDummy;
		}
	}
	return nullptr;
}

void TestRandomSpawnFloatingUIs(ABladeIICharacter* InOwnerDummy, float HPProbabilityAlpha)
{
	if (!InOwnerDummy)
	{
		return;
	}

	// HPProbabilityAlpha 가 0 이면 BuffEffect 만, 1 이면 HP 만.
	const bool bSpawnHP = (HPProbabilityAlpha <= 0.0f) ? false : (FMath::RandRange(0.0f, 1.0f) <= FMath::Clamp(HPProbabilityAlpha, 0.0f, 1.0f));
	if (bSpawnHP)
	{
		CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Signal(InOwnerDummy, FMath::FRandRange(-19999.0f, 19999.0f), FMath::RandBool());
	}
	else
	{
		UB2BuffModeEffectInfo* BuffEffectInfo = InOwnerDummy->GetBuffModeEffectInfo();
		if (BuffEffectInfo)
		{
			int32 RandBuffType = FMath::RandRange(0, 1);

			switch (RandBuffType)
			{
			case 0:
			{
				int32 RandBuffTypeInt = FMath::RandRange(1, int32(EBuffType::Buff_End) - 1);

				BuffEffectInfo->SpawnCharacterBuffTextEffect(EBuffType(RandBuffTypeInt), InOwnerDummy);
			}
			break;
			case 1: BuffEffectInfo->SpawnResurrectTextEffect((EResurrectBuffType)FMath::RandRange(1, 3), InOwnerDummy); break;
			}
		}
	}
}
void TestRandomSpawnDamageFx(ABladeIICharacter* InOwnerDummy)
{
	if (!InOwnerDummy)
	{
		return;
	}

	UB2DamageEffectInfo* DamageEffectInfo = InOwnerDummy->GetDamageEffectInfo();
	if (DamageEffectInfo)
	{
		DamageEffectInfo->PlayHitFlash(InOwnerDummy);

		DamageEffectInfo->SpawnDamageEffect(
			(EDamageWeapon)(FMath::RandRange(0,2)),
			(EDamageElement)(FMath::RandRange(1,4)),
			InOwnerDummy,
			EHitDirection::EHD_HitActor, 180.0f,
			InOwnerDummy->GetActorRotation()
		);

		DamageEffectInfo->PlayHitSound(InOwnerDummy, (ECharacterType)(FMath::RandRange(1,4)), InOwnerDummy->GetActorLocation());
	}
}

#endif