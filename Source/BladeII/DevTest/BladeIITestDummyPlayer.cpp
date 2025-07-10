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
// 임의로 런타임 spawn 시켜서 대체로 성능 테스트 용으로 사용.
// 좀 확장시키면 다른 용도로 사용이 가능할지도.
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

	// 더미로써 쓸모없는 것들 끄기.
	FormalSetPointLightOn(false);  // 어차피 IsHeadPointLightAllowed() 가 false 여야..
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
	
	// AnimBP State 처리를 위해 순차적으로 밟아야 하는 state 가 있다면 처리. 대체로 콤보 어택의 경우
	if (FinalAttackStateMaintainCount <= 0)
	{
		if (IsOneOfComboAttackState(DirectAttackState))
		{
			if (ThisTickFinalAttackState < DirectAttackState) {
				ThisTickFinalAttackState = (EAttackState)((int32)ThisTickFinalAttackState + 1);
				FinalAttackStateMaintainCount = FINAL_ATTACK_STATE_MAINTAIN_MAX_COUNT; // 다음번 State transition 까지 대기.
			}
			else {
				ThisTickFinalAttackState = DirectAttackState;
			}
		}
		else
		{ // 다른 것도 비슷한 처리가 필요하면..
			ThisTickFinalAttackState = DirectAttackState;
		}
	}
	else
	{
		FinalAttackStateMaintainCount--;
	}

	SetAttackState(ThisTickFinalAttackState);

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
	// 언젠가 ReceiveComboEnd 를 사용 못하게 되면 다른 경로를 통하도록..
	AttackStateEndInternal(InLastAttackState);
}

void ABladeIITestDummyPlayer::AttackStateEndInternal(EAttackState InLastAttackState)
{
	// SkillRTManager 를 가지고 뭐 하는 게 있어서 외관에 영향을 미칠 만한 것들 빼고 비워둠.

	if (InLastAttackState >= EAttackState::ECS_GuardStart && InLastAttackState <= EAttackState::ECS_Evade)
	{
		::EmissiveNotifyEnd(GetMesh(), ChainBitField, GetChainMaterial(), true);
	}

	if (InLastAttackState == EAttackState::ECS_WeaponSkill)
	{
		SetupLODInfoForInGame(); // WeaponSkill 시작 시 연출용 LOD 를 셋업하므로 되돌려줌.
	}

	// 다음 애니메이션 시작.. 간격을 두거나 바로
	UWorld* TheWorld = GetWorld();
	if (CooltimeBetweenAnim > 0.0f && TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(CooltimeBetweenAnimTH, 
			// RandomAttackState 인 경우 BeginNextAnimCommon 에 무엇을 넘겨주든 랜덤 선택될 것이지만 의미를 명확히..
			FTimerDelegate::CreateUObject(this, &ABladeIITestDummyPlayer::BeginNextAnimCommon, (bUseRandomAttackState ? EAttackState::ECS_End : DirectAttackState)), 
			CooltimeBetweenAnim, false);
		SetDirectAttackState(EAttackState::ECS_None); // 다음 애니메이션 발동 전까지 Idle 상태로 휴식.
	}
	else
	{
		BeginNextAnimCommon(); // 바로 시작. 현재 DirectAttackState 그대로 이어서.
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
	// 상단의 입력 subscribe 없도록..
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

void ABladeIITestDummyPlayer::BeginNextAnimCommon(EAttackState SpecifiedNextAnim /* RandomAttackState 가 아니고 EAttackState::ECS_End 외의 값을 줄 시 사용됨. */)
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

void ABladeIITestDummyPlayer::SetDirectAttackStateByKeyword(const FString& InAttackStateKeyword)
{
	// 콘솔 커맨드를 통해 AttackState 지정하는 기능을 쓰려고.

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
	// SkillAnimIdx 까지 상세 조절..
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

		// SkillLOD 값은 1 이 들어가게 될 껀데 SkillAnimIdx LOD 아닌 걸로 넣어주면 어케 되나..
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

	// 기타 필요에 따라..

	SetDirectAttackState(ParsedAttackState);
}

void ABladeIITestDummyPlayer::SetDirectAttackState(EAttackState InState)
{
	DirectAttackState = InState;

	if (IsOneOfComboAttackState(DirectAttackState)){
		ThisTickFinalAttackState = EAttackState::ECS_Combo01; // Direct 하게 AnimBP 에 세팅한다 해도 State 머신이 순서대로 받기 땜에 어쩔 수 없이 Combo1 부터 시작해야..
	}
	else {
		// 다른 것도 순서대로 해야 하는 게 있을 수 있다. 필요하면 비슷하게..
		ThisTickFinalAttackState = DirectAttackState;
	}

	if (IsOneOfSkillAttackState(ThisTickFinalAttackState))
	{ // Skill 시전 시 특별히 추가해야 하는 거
		SetReservedSkill(ThisTickFinalAttackState);
	}

	FinalAttackStateMaintainCount = FINAL_ATTACK_STATE_MAINTAIN_MAX_COUNT; // AttackState 를 순차적으로 증가시켜야 하는 경우의 카운트 핸들링을 위해
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
	// 랜덤에서는 WeaponSkill 안나옴. 확실히 사용하는 state 들로.

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

	// bUseRandomAttackState 가 true 이면 이번 모션이 끝나고 나서 다시 이번 랜덤 선택 절차를 거치게 될 것이다.
	SetDirectAttackState(RandAttackState);
}

/** 사용을 위한 메인 유틸 */
ABladeIITestDummyPlayer* ABladeIITestDummyPlayer::SpawnWithStandardEquipment(UObject* WorldContextObject, EPCClass InClass, const FTransform& InSpawnTransform, 
	int32 InEquipGrade, float InSpawnFloatingUIInterval, float InSpawnDamageFxInterval, bool bShowWing, int32 InWingGrade, bool bPossessByAIController)
{
	// SpawnPlayerCharAsPuppet 안에서도 비슷하게 부끄러운 짓을 하는데 여기서도..
	// BladeIIGameMode 안에 구현된 Spawn 함수에서 사용하는 클래스 멤버 변수를 슬쩍 바꿔놓고 호출.
	// 여긴 그나마 개발용 기능이니 좀 다행인듯
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	TSubclassOf<APawn> CachedGMDefaultPawnClass = nullptr;
	if (B2GM)
	{
		CachedGMDefaultPawnClass = B2GM->DefaultPawnClass;
		B2GM->DefaultPawnClass = ABladeIITestDummyPlayer::StaticClass();
	}

	ABladeIITestDummyPlayer* SpawnedDummy = Cast<ABladeIITestDummyPlayer>(ABladeIIGameMode::SpawnPlayerCharAsPuppet(WorldContextObject, InClass, InSpawnTransform, bPossessByAIController));

	if (B2GM && CachedGMDefaultPawnClass) // 부끄러운 짓 슥 다시 돌려놓기
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
		// SpawnPlayerCharAsPuppet 에서는 InGameOnlyInfo 가 없이 되느데 InGameOnlyInfo 포함해서 다시금.
		SpawnedDummy->OverrideByPCClassInfo(B2GM, ThisClassPCInfo , false);
		if (SpawnedDummy->CenterShadow)
		{
			SpawnedDummy->SetupDecalCompForCenterShadowCommon(SpawnedDummy->CenterShadow, SpawnedDummy->GetMesh());
			SpawnedDummy->CenterShadow->SetDecalMaterial(SpawnedDummy->ShadowMaterial);
		}

		// MergeSections 는 여기서 보낼 필요없이 테스트 커맨드로 제어하면 될 듯.
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