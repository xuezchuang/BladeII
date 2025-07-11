// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeIITestDummyPlayer.h"
#include "BladeIITestDummyNPC.h"
#include "BladeIIGameMode.h"
#include "B2Inventory.h"
#include "B2PCClassInfoBox.h"
#include "B2BuffModeEffectInfo.h"
#include "B2DamageEffectInfo.h"
#include "Event.h"

//
// ���Ƿ� ��Ÿ�� spawn ���Ѽ� ��ü�� ���� �׽�Ʈ ������ ���.
// �� Ȯ���Ű�� �ٸ� �뵵�� ����� ����������.
// 

ABladeIITestDummyPlayer::ABladeIITestDummyPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ThisTickFinalAttackState = EAttackState::ECS_None;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	PrimaryActorTick.bCanEverTick = true;

	DirectAttackState = EAttackState::ECS_None;
	FinalAttackStateMaintainCount = 0;
	bUseRandomAttackState = false;

	CooltimeBetweenAnim = 0.0f;

	SpawnFloatingUIInterval = 0.0f;
	NextTimeSpawnFloatingUIInterval = 0.0f;
	TimeSinceLastSpawnFloatingUITime = 0.0f;

	SpawnDamageFxInterval = 0.0f;
	NextTimeSpawnDamageFxInterval = 0.0f;
	TimeSinceLastSpawnDamageFxTime = 0.0f;
#endif
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2

void ABladeIITestDummyPlayer::BeginPlay()
{
	Super::BeginPlay();

	// ���̷ν� ������� �͵� ����.
	FormalSetPointLightOn(false);  // ������ IsHeadPointLightAllowed() �� false ����..
}

void ABladeIITestDummyPlayer::BeginDestroy()
{
	Super::BeginDestroy();
}

void ABladeIITestDummyPlayer::Destroyed()
{
	Super::Destroyed();
}

void ABladeIITestDummyPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// AnimBP State ó���� ���� ���������� ��ƾ� �ϴ� state �� �ִٸ� ó��. ��ü�� �޺� ������ ���
	if (FinalAttackStateMaintainCount <= 0)
	{
		if (IsOneOfComboAttackState(DirectAttackState))
		{
			if (ThisTickFinalAttackState < DirectAttackState) {
				ThisTickFinalAttackState = (EAttackState)((int32)ThisTickFinalAttackState + 1);
				FinalAttackStateMaintainCount = FINAL_ATTACK_STATE_MAINTAIN_MAX_COUNT; // ������ State transition ���� ���.
			}
			else {
				ThisTickFinalAttackState = DirectAttackState;
			}
		}
		else
		{ // �ٸ� �͵� ����� ó���� �ʿ��ϸ�..
			ThisTickFinalAttackState = DirectAttackState;
		}
	}
	else
	{
		FinalAttackStateMaintainCount--;
	}

	SetAttackState(ThisTickFinalAttackState);

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
}

bool ABladeIITestDummyPlayer::CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType)
{
	return false;
}

bool ABladeIITestDummyPlayer::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	return false;
}

void ABladeIITestDummyPlayer::ReceiveComboEnd(EAttackState InLastAttackState, bool IsAttackCanceled)
{
	// ������ ReceiveComboEnd �� ��� ���ϰ� �Ǹ� �ٸ� ��θ� ���ϵ���..
	AttackStateEndInternal(InLastAttackState);
}

void ABladeIITestDummyPlayer::AttackStateEndInternal(EAttackState InLastAttackState)
{
	// SkillRTManager �� ������ �� �ϴ� �� �־ �ܰ��� ������ ��ĥ ���� �͵� ���� �����.

	if (InLastAttackState >= EAttackState::ECS_GuardStart && InLastAttackState <= EAttackState::ECS_Evade)
	{
		::EmissiveNotifyEnd(GetMesh(), ChainBitField, GetChainMaterial(), true);
	}

	if (InLastAttackState == EAttackState::ECS_WeaponSkill)
	{
		SetupLODInfoForInGame(); // WeaponSkill ���� �� ����� LOD �� �¾��ϹǷ� �ǵ�����.
	}

	// ���� �ִϸ��̼� ����.. ������ �ΰų� �ٷ�
	UWorld* TheWorld = GetWorld();
	if (CooltimeBetweenAnim > 0.0f && TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(CooltimeBetweenAnimTH, 
			// RandomAttackState �� ��� BeginNextAnimCommon �� ������ �Ѱ��ֵ� ���� ���õ� �������� �ǹ̸� ��Ȯ��..
			FTimerDelegate::CreateUObject(this, &ABladeIITestDummyPlayer::BeginNextAnimCommon, (bUseRandomAttackState ? EAttackState::ECS_End : DirectAttackState)), 
			CooltimeBetweenAnim, false);
		SetDirectAttackState(EAttackState::ECS_None); // ���� �ִϸ��̼� �ߵ� ������ Idle ���·� �޽�.
	}
	else
	{
		BeginNextAnimCommon(); // �ٷ� ����. ���� DirectAttackState �״�� �̾.
	}
}

bool ABladeIITestDummyPlayer::IsFineToStartAnyNewInputHandling()
{
	return false;
}
bool ABladeIITestDummyPlayer::IsFineToStartAnyNewMoveInputHandling()
{
	return false;
}

void ABladeIITestDummyPlayer::SubscribeEvents()
{
	// ����� �Է� subscribe ������..
}
void ABladeIITestDummyPlayer::UnsubscribeEvents()
{

}

void ABladeIITestDummyPlayer::RandomSpawnFloatingUIs()
{
	TestRandomSpawnFloatingUIs(this, 0.5f);
}

void ABladeIITestDummyPlayer::RandomSpawnDamageFx()
{
	TestRandomSpawnDamageFx(this);
}

void ABladeIITestDummyPlayer::BeginNextAnimCommon(EAttackState SpecifiedNextAnim /* RandomAttackState �� �ƴϰ� EAttackState::ECS_End ���� ���� �� �� ����. */)
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

void ABladeIITestDummyPlayer::SetDirectAttackStateByKeyword(const FString& InAttackStateKeyword)
{
	// �ܼ� Ŀ�ǵ带 ���� AttackState �����ϴ� ����� ������.

	EAttackState ParsedAttackState = EAttackState::ECS_None;

	if (InAttackStateKeyword == TEXT("A1")) {
		ParsedAttackState = EAttackState::ECS_Combo01;
	}
	else if (InAttackStateKeyword == TEXT("A2")) {
		ParsedAttackState = EAttackState::ECS_Combo02;
	}
	else if (InAttackStateKeyword == TEXT("A3")) {
		ParsedAttackState = EAttackState::ECS_Combo03;
	}
	else if (InAttackStateKeyword == TEXT("A4")) {
		ParsedAttackState = EAttackState::ECS_Combo04;
	}
	else if (InAttackStateKeyword == TEXT("A5")) {
		ParsedAttackState = EAttackState::ECS_Combo05;
	}
	else if (InAttackStateKeyword == TEXT("A6")) {
		ParsedAttackState = EAttackState::ECS_Combo06;
	}
	// SkillAnimIdx ���� �� ����..
	else if (InAttackStateKeyword.StartsWith(TEXT("S1"))) {
		ParsedAttackState = EAttackState::ECS_Skill01;
		CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill01_01;
		if (InAttackStateKeyword == TEXT("S11")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill01_01;
		}
		else if (InAttackStateKeyword == TEXT("S12")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill01_02;
		}
		else if (InAttackStateKeyword == TEXT("S13")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill01_03;
		}
	}
	else if (InAttackStateKeyword.StartsWith(TEXT("S2"))) {
		ParsedAttackState = EAttackState::ECS_Skill02;
		CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill02_01;
	}
	else if (InAttackStateKeyword.StartsWith(TEXT("S3"))) {
		ParsedAttackState = EAttackState::ECS_Skill03;
		CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill03_01;
		if (InAttackStateKeyword == TEXT("S31")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill03_01;
		}
		else if (InAttackStateKeyword == TEXT("S32")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill03_02;
		}
		else if (InAttackStateKeyword == TEXT("S33")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill03_03;
		}
	}
	else if (InAttackStateKeyword.StartsWith(TEXT("S4"))) {
		ParsedAttackState = EAttackState::ECS_Skill04;
		CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill04_01;
	}
	else if (InAttackStateKeyword.StartsWith(TEXT("S5"))) {
		ParsedAttackState = EAttackState::ECS_Skill05;
		CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill05_01;
		if (InAttackStateKeyword == TEXT("S51")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill05_01;
		}
		else if (InAttackStateKeyword == TEXT("S52")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill05_02;
		}
		else if (InAttackStateKeyword == TEXT("S53")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill05_03;
		}
	}
	else if (InAttackStateKeyword.StartsWith(TEXT("S6"))) {
		ParsedAttackState = EAttackState::ECS_Skill06;
		CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill06_01;
	}
	else if (InAttackStateKeyword.StartsWith(TEXT("WP"))) {
		ParsedAttackState = EAttackState::ECS_WeaponSkill;
		CurrentSkillAnimIdx = ESkillAnimType::ESA_Weapon_01;

		// SkillLOD ���� 1 �� ���� �� ���� SkillAnimIdx LOD �ƴ� �ɷ� �־��ָ� ���� �ǳ�..
		if (InAttackStateKeyword == TEXT("WP1")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Weapon_01;
		}
		else if (InAttackStateKeyword == TEXT("WP2")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Weapon_02;
		}
		else if (InAttackStateKeyword == TEXT("WP3")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Weapon_03;
		}
		else if (InAttackStateKeyword == TEXT("WP4")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Weapon_04;
		}
		else if (InAttackStateKeyword == TEXT("WP5")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Weapon_05;
		}
		else if (InAttackStateKeyword == TEXT("WP1L")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Weapon_01_LOD;
		}
		else if (InAttackStateKeyword == TEXT("WP2L")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Weapon_02_LOD;
		}
		else if (InAttackStateKeyword == TEXT("WP3L")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Weapon_03_LOD;
		}
		else if (InAttackStateKeyword == TEXT("WP4L")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Weapon_04_LOD;
		}
		else if (InAttackStateKeyword == TEXT("WP5L")) {
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Weapon_05_LOD;
		}
	}

	// ��Ÿ �ʿ信 ����..

	SetDirectAttackState(ParsedAttackState);
}

void ABladeIITestDummyPlayer::SetDirectAttackState(EAttackState InState)
{
	DirectAttackState = InState;

	if (IsOneOfComboAttackState(DirectAttackState)){
		ThisTickFinalAttackState = EAttackState::ECS_Combo01; // Direct �ϰ� AnimBP �� �����Ѵ� �ص� State �ӽ��� ������� �ޱ� ���� ��¿ �� ���� Combo1 ���� �����ؾ�..
	}
	else {
		// �ٸ� �͵� ������� �ؾ� �ϴ� �� ���� �� �ִ�. �ʿ��ϸ� ����ϰ�..
		ThisTickFinalAttackState = DirectAttackState;
	}

	if (IsOneOfSkillAttackState(ThisTickFinalAttackState))
	{ // Skill ���� �� Ư���� �߰��ؾ� �ϴ� ��
		SetReservedSkill(ThisTickFinalAttackState);
	}

	FinalAttackStateMaintainCount = FINAL_ATTACK_STATE_MAINTAIN_MAX_COUNT; // AttackState �� ���������� �������Ѿ� �ϴ� ����� ī��Ʈ �ڵ鸵�� ����
}

void ABladeIITestDummyPlayer::SetUseRandomAttackState(bool bInRandomAttackState)
{
	bUseRandomAttackState = bInRandomAttackState;
	if (bUseRandomAttackState)
	{
		GoToNextRandomAttackState();
	}
}

void ABladeIITestDummyPlayer::SetCooltimeBetweenAnim(float InCooltime)
{
	CooltimeBetweenAnim = InCooltime;
}

void ABladeIITestDummyPlayer::GoToNextRandomAttackState()
{
	// ���������� WeaponSkill �ȳ���. Ȯ���� ����ϴ� state ���.

	const int32 RandStateInt = FMath::RandRange(0, 10);
	const int32 RandSkillVariInt = FMath::RandRange(0, 2);
	EAttackState RandAttackState = EAttackState::ECS_None;

	switch (RandStateInt)
	{
	case 0: RandAttackState = EAttackState::ECS_Combo01; break;
	case 1: RandAttackState = EAttackState::ECS_Combo02; break;
	case 2: RandAttackState = EAttackState::ECS_Combo03; break;
	case 3: RandAttackState = EAttackState::ECS_Combo04; break;
	case 4: RandAttackState = EAttackState::ECS_Combo05; break;
	case 5:
		{
			RandAttackState = EAttackState::ECS_Skill01;
			if (RandSkillVariInt == 0) {
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill01_01;
			}
			else if (RandSkillVariInt == 1) {
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill01_02;
			}
			else if (RandSkillVariInt == 2) {
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill01_03;
			}
			else{
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill01_01;
			}
		}
		break;
	case 6:
		{
			RandAttackState = EAttackState::ECS_Skill02;
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill02_01;
		}
		break;
	case 7:
		{
			RandAttackState = EAttackState::ECS_Skill03;
			if (RandSkillVariInt == 0) {
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill03_01;
			}
			else if (RandSkillVariInt == 1) {
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill03_02;
			}
			else if (RandSkillVariInt == 2) {
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill03_03;
			}
			else {
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill03_01;
			}
		}
		break;
	case 8: 
		{
			RandAttackState = EAttackState::ECS_Skill04;
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill04_01;
		}
		break;
	case 9: 
		{
			RandAttackState = EAttackState::ECS_Skill05;
			if (RandSkillVariInt == 0) {
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill05_01;
			}
			else if (RandSkillVariInt == 1) {
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill05_02;
			}
			else if (RandSkillVariInt == 2) {
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill05_03;
			}
			else {
				CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill05_01;
			}
		}		
		break;
	case 10:
		{
			RandAttackState = EAttackState::ECS_Skill06;
			CurrentSkillAnimIdx = ESkillAnimType::ESA_Skill06_01;
		}
		break;
	}

	// bUseRandomAttackState �� true �̸� �̹� ����� ������ ���� �ٽ� �̹� ���� ���� ������ ��ġ�� �� ���̴�.
	SetDirectAttackState(RandAttackState);
}

/** ����� ���� ���� ��ƿ */
ABladeIITestDummyPlayer* ABladeIITestDummyPlayer::SpawnWithStandardEquipment(UObject* WorldContextObject, EPCClass InClass, const FTransform& InSpawnTransform, 
	int32 InEquipGrade, float InSpawnFloatingUIInterval, float InSpawnDamageFxInterval, bool bShowWing, int32 InWingGrade, bool bPossessByAIController)
{
	// SpawnPlayerCharAsPuppet �ȿ����� ����ϰ� �β����� ���� �ϴµ� ���⼭��..
	// BladeIIGameMode �ȿ� ������ Spawn �Լ����� ����ϴ� Ŭ���� ��� ������ ��½ �ٲ���� ȣ��.
	// ���� �׳��� ���߿� ����̴� �� �����ε�
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	TSubclassOf<APawn> CachedGMDefaultPawnClass = nullptr;
	if (B2GM)
	{
		CachedGMDefaultPawnClass = B2GM->DefaultPawnClass;
		B2GM->DefaultPawnClass = ABladeIITestDummyPlayer::StaticClass();
	}

	ABladeIITestDummyPlayer* SpawnedDummy = Cast<ABladeIITestDummyPlayer>(ABladeIIGameMode::SpawnPlayerCharAsPuppet(WorldContextObject, InClass, InSpawnTransform, bPossessByAIController));

	if (B2GM && CachedGMDefaultPawnClass) // �β����� �� �� �ٽ� ��������
	{
		B2GM->DefaultPawnClass = CachedGMDefaultPawnClass;
	}
	
	if (SpawnedDummy)
	{
		TArray<FB2Item> EquipDummyItems;
		GetStandardDummyPCPartItemData(InClass, InEquipGrade, EquipDummyItems);

		FB2Wing DummyWing;
		if (bShowWing && InWingGrade >= MIN_WING_EVOLUTION_GRADE && InWingGrade <= MAX_WING_EVOLUTION_GRADE)
		{
			DummyWing.EnhanceLevel = 1;
			DummyWing.EvolutionGrade = InWingGrade;
		}

		UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox(WorldContextObject);
		UB2PCClassInfo* ThisClassPCInfo = PCClassInfoBox ? PCClassInfoBox->GetSingleClassInfo(InClass) : NULL;
		// SpawnPlayerCharAsPuppet ������ InGameOnlyInfo �� ���� �Ǵ��� InGameOnlyInfo �����ؼ� �ٽñ�.
		SpawnedDummy->OverrideByPCClassInfo(B2GM, ThisClassPCInfo , false);
		if (SpawnedDummy->CenterShadow)
		{
			SpawnedDummy->SetupDecalCompForCenterShadowCommon(SpawnedDummy->CenterShadow, SpawnedDummy->GetMesh());
			SpawnedDummy->CenterShadow->SetDecalMaterial(SpawnedDummy->ShadowMaterial);
		}

		// MergeSections �� ���⼭ ���� �ʿ���� �׽�Ʈ Ŀ�ǵ�� �����ϸ� �� ��.
		SpawnedDummy->SetupComponentsForPartsCommon(EquipDummyItems, bShowWing ? &DummyWing : nullptr, false, true);

		SpawnedDummy->SpawnFloatingUIInterval = InSpawnFloatingUIInterval;
		SpawnedDummy->NextTimeSpawnFloatingUIInterval = InSpawnFloatingUIInterval;
		SpawnedDummy->TimeSinceLastSpawnFloatingUITime = 0.0f;

		SpawnedDummy->SpawnDamageFxInterval = InSpawnDamageFxInterval;
		SpawnedDummy->NextTimeSpawnDamageFxInterval= InSpawnDamageFxInterval;
		SpawnedDummy->TimeSinceLastSpawnDamageFxTime = 0.0f;


		return SpawnedDummy;
	}
	return nullptr;
}

#endif