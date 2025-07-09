// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "BladeIITestDummyNPC.h"
#include "BladeIIGameMode.h"
#include "B2NPCClassInfoBox.h"
#include "B2BuffModeEffectInfo.h"
#include "B2DamageEffectInfo.h"
#include "B2CombatStatEvaluator.h"
#include "Event.h"

//
// ���Ƿ� ��Ÿ�� spawn ���Ѽ� ��ü�� ���� �׽�Ʈ ������ ���.
// ABladeIITestDummyPlayer �� �ߺ��� ����� ���� �� �ִµ� ���� ����� �ƴ� �׽�Ʈ ���̹Ƿ� ������ �ߺ� �ڵ�� �Ѿ.
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
	{ // BladeIICharacter ���� AnimBPFnLib �� ���� ���� ReceiveComboEnd �� ��� �뷫 �̷���.. 
		OnComboEnd(LastStateBackup);
	}
#endif
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2

void ABladeIITestDummyNPC::BeginPlay()
{
	Super::BeginPlay();

	// ���̷ν� ������� �͵� ����.

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
		
	// ��ǻ� ���� ���ؼ� AttackState �� ���޵Ǵ� DirectAttackState �� ���� ���°� �Ǿ�� �� ��.
	if (IsOneOfComboAttackState(DirectAttackState))
	{
		SetSignalAttack(true); // ���� �ڵ� ���� �� SetDirectAttackState ���� �ܹ߼����� SetSignalAttack �� �ϴ� �ɷδ� ���ġ ���� �� ��.
		SetAttackState(DirectAttackState);
	}
	
	TimeSinceLastSpawnFloatingUITime += DeltaSeconds;
	// �����δ� NextTimeSpawnFloatingUIInterval �� ����. �� �ϸ��� SpawnFloatingUIInterval �� �������� ���ݾ� ����. ������ �Ȱ��� interval �̸� �������� ���� spawn �� ������ ���� ���� �����Ƿ�
	if (SpawnFloatingUIInterval > 0.0f && TimeSinceLastSpawnFloatingUITime >= NextTimeSpawnFloatingUIInterval)
	{
		RandomSpawnFloatingUIs();
		TimeSinceLastSpawnFloatingUITime = 0.0f;
		NextTimeSpawnFloatingUIInterval = SpawnFloatingUIInterval * FMath::FRandRange(0.75f, 1.25f);
	}

	TimeSinceLastSpawnDamageFxTime += DeltaSeconds;
	// ���⵵ ���������� �� Ÿ�Ӹ��� �������� �������� NextTimeSpawnDamageFxInterval ��..
	if (SpawnDamageFxInterval > 0.0f && TimeSinceLastSpawnDamageFxTime >= NextTimeSpawnDamageFxInterval)
	{
		RandomSpawnDamageFx();
		TimeSinceLastSpawnDamageFxTime = 0.0f;
		NextTimeSpawnDamageFxInterval = SpawnDamageFxInterval * FMath::FRandRange(0.75f, 1.25f);
	}

	// Spawn ��� ������ ��ġ�� ����. Z ���� Spawn ��ÿ� ������ ���߿� �� ���� �� ������ ������Ʈ �� ���� �������� �ʰ� ��.
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

	// ���� ������� Damage ������ ���ϰ� ��. �׷����� �̰͸� ������� �÷��̾ ���ݿ� �¾��� �� �и��ٰų� �ϴ� �ͱ��� ���� ����.
	TotalAttack = 0.0f;
	AttackLevel = CombatStatEval::GetADLevel(TotalAttack, CombatStatEval::GetDefaultCombatCoefficient());
}

void ABladeIITestDummyNPC::OnComboEnd(EAttackState LastState)
{
	// ���� �ִϸ��̼� ����.. ������ �ΰų� �ٷ�
	UWorld* TheWorld = GetWorld();
	if (CooltimeBetweenAnim > 0.0f && TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(CooltimeBetweenAnimTH,
			// RandomAttackState �� ��� BeginNextAnimCommon �� ������ �Ѱ��ֵ� ���� ���õ� �������� �ǹ̸� ��Ȯ��..
			FTimerDelegate::CreateUObject(this, &ABladeIITestDummyNPC::BeginNextAnimCommon, (bUseRandomAttackState ? EAttackState::ECS_End : DirectAttackState)),
			CooltimeBetweenAnim, false);
		SetDirectAttackState(EAttackState::ECS_None); // ���� �ִϸ��̼� �ߵ� ������ Idle ���·� �޽�.
	}
	else
	{
		BeginNextAnimCommon(); // �ٷ� ����. ���� DirectAttackState �״�� �̾.
	}
}

void ABladeIITestDummyNPC::RandomSpawnFloatingUIs()
{
	// NPC �� HP �ܿ� FloatingUI ���� �� ��������.. �ִ��� �״�..?
	TestRandomSpawnFloatingUIs(this, 1.0f);
}

void ABladeIITestDummyNPC::RandomSpawnDamageFx()
{
	TestRandomSpawnDamageFx(this);
}

void ABladeIITestDummyNPC::BeginNextAnimCommon(EAttackState SpecifiedNextAnim /* RandomAttackState �� �ƴϰ� EAttackState::ECS_End ���� ���� �� �� ����. */)
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(CooltimeBetweenAnimTH);
	}

	if (bUseRandomAttackState) // ���� ��� �� ���� ���� �ִϸ��̼��� ����
	{
		GoToNextRandomAttackState();
	}
	else
	{ // ���� �ִ� ����� �ƴ϶�� ������ �ɷ� �ٽ� ����.
	  // �޺� state �� ó������ �ٽ� �����ؾ� �ؼ� ThisTickFinalAttackState ���� ������ �ȴ�.
		const EAttackState FinalNextAnim = (SpecifiedNextAnim != EAttackState::ECS_End) ? SpecifiedNextAnim : DirectAttackState;
		if (IsOneOfComboAttackState(FinalNextAnim))
		{
			SetDirectAttackState(FinalNextAnim);
		}
	}
}

void ABladeIITestDummyNPC::SetDirectAttackStateByKeyword(const FString& InAttackStateKeyword)
{
	// �ܼ� Ŀ�ǵ带 ���� AttackState �����ϴ� ����� ������.

	if (InAttackStateKeyword == TEXT("None") || InAttackStateKeyword == TEXT("No"))
	{
		SetDirectAttackState(EAttackState::ECS_None);
	}
	else
	{
		int32 ParsedAttackIndex = FCString::Atoi(*InAttackStateKeyword);

		if (ParsedAttackIndex == 0) // Atoi �� �����ߵ��� �ƴ� 0 �� �����ߵ���.
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

		// ���� ���õ� AttackData ���� �̳���.
		AIAttackIndex = ParsedAttackIndex % AIAttackDataArray.Num();

		// NPC ���� ���� ���´� �ϳ��� ���.
		SetDirectAttackState(EAttackState::ECS_Combo01);
	}
}

void ABladeIITestDummyNPC::SetDirectAttackState(EAttackState InState)
{
	// ���⼭ SetSignalAttack �� �θ��� �͸����� ���ġ�� ����..
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
	// InitialLocation �� �����ϴ� �ɼ��̶�..?
}
void ABladeIITestDummyNPC::SetMaintainRotation(bool bMaintain)
{
	bMaintainRotation = bMaintain;
}

void ABladeIITestDummyNPC::GoToNextRandomAttackState()
{
	// AIAttackDataArray �� ��ϵ� ������ŭ.
	AIAttackIndex = FMath::RandRange(0, AIAttackDataArray.Num() - 1);
	
	// NPC ���� ���� ���´� �ϳ��� ���.
	SetDirectAttackState(EAttackState::ECS_Combo01);
}

/** ����� ���� ���� ��ƿ */
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
		SpawnInfo.bDeferConstruction = true; // AutoPossessAI ���� �� NPCClassInfo ������ ���� DeferConstruction �� �ʿ�.
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.ObjectFlags |= RF_Transient;

		ABladeIITestDummyNPC* SpawnedDummy = NPCInfoBox ?
			NPCInfoBox->SpawnNPCClassInfoDummyCharacter(TheWorld, InClass, InVariation, InSpawnTransform, SpawnInfo) : nullptr;
		
		if (SpawnedDummy)
		{
			// AI possess �� ���� ������ �ᱹ spawn ��� ��Ȳ�� ���� ���� snap �� �� �ȵǴ� �� ������ AI possess �ϰ� ��Ÿ ������ ����.
			// �׸��� ��Ȳ�� ���� �÷��̾ ������ų� ȸ�� ������ ����� �ʿ䵵 ���� �� �ְ� �ؼ�..
			SpawnedDummy->AutoPossessAI = EAutoPossessAI::Spawned;
			SpawnedDummy->FinishSpawning(InSpawnTransform);
			SpawnedDummy->bNeedSpawnCheck = false; // SpawnStay �� �ִ� ��� �ѱ⵵��.
			SnapBladeIICharacterToGround(SpawnedDummy);
			// �� ��ġ�� ������Ű�� ���� ������ ��.
			SpawnedDummy->InitialLocation = SpawnedDummy->GetActorLocation();
			SpawnedDummy->InitialRotation = SpawnedDummy->GetActorRotation();

			if (SpawnedDummy->CenterShadow)
			{
				SpawnedDummy->SetupDecalCompForCenterShadowCommon(SpawnedDummy->CenterShadow, SpawnedDummy->GetMesh());
				SpawnedDummy->CenterShadow->SetDecalMaterial(SpawnedDummy->ShadowMaterial);
			}

			UCharacterMovementComponent* CharMoveComp = SpawnedDummy->GetCharacterMovement();
			if (CharMoveComp)
			{ // ������ ������ �ִ��� ����.. �Ϸ��ٰ� Z ���� ������ �ʿ䰡 �־ �ٸ� ������.. ��
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

	// HPProbabilityAlpha �� 0 �̸� BuffEffect ��, 1 �̸� HP ��.
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