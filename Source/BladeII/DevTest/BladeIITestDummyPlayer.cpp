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
// 烙狼肺 繁鸥烙 spawn 矫难辑 措眉肺 己瓷 抛胶飘 侩栏肺 荤侩.
// 粱 犬厘矫虐搁 促弗 侩档肺 荤侩捞 啊瓷且瘤档.
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

	// 歹固肺结 镜葛绝绰 巴甸 掺扁.
	FormalSetPointLightOn(false);  // 绢瞒乔 IsHeadPointLightAllowed() 啊 false 咯具..
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
	
	// AnimBP State 贸府甫 困秦 鉴瞒利栏肺 光酒具 窍绰 state 啊 乐促搁 贸府. 措眉肺 霓焊 绢琶狼 版快
	if (FinalAttackStateMaintainCount <= 0)
	{
		if (IsOneOfComboAttackState(DirectAttackState))
		{
			if (ThisTickFinalAttackState < DirectAttackState) {
				ThisTickFinalAttackState = (EAttackState)((int32)ThisTickFinalAttackState + 1);
				FinalAttackStateMaintainCount = FINAL_ATTACK_STATE_MAINTAIN_MAX_COUNT; // 促澜锅 State transition 鳖瘤 措扁.
			}
			else {
				ThisTickFinalAttackState = DirectAttackState;
			}
		}
		else
		{ // 促弗 巴档 厚搅茄 贸府啊 鞘夸窍搁..
			ThisTickFinalAttackState = DirectAttackState;
		}
	}
	else
	{
		FinalAttackStateMaintainCount--;
	}

	SetAttackState(ThisTickFinalAttackState);

	TimeSinceLastSpawnFloatingUITime += DeltaSeconds;
	// 角力肺绰 NextTimeSpawnFloatingUIInterval 阑 夯促. 概 畔付促 SpawnFloatingUIInterval 阑 扁霖栏肺 炼陛究 函悼. 肯傈洒 度鞍篮 interval 捞搁 咯矾付府 悼矫 spawn 矫 老力洒 唱棵 荐档 乐栏骨肺
	if (SpawnFloatingUIInterval > 0.0f && TimeSinceLastSpawnFloatingUITime >= NextTimeSpawnFloatingUIInterval)
	{
		RandomSpawnFloatingUIs();
		TimeSinceLastSpawnFloatingUITime = 0.0f;
		NextTimeSpawnFloatingUIInterval = SpawnFloatingUIInterval * FMath::FRandRange(0.75f, 1.25f);
	}

	TimeSinceLastSpawnDamageFxTime += DeltaSeconds;
	// 咯扁档 付蛮啊瘤肺 概 鸥烙付促 罚待栏肺 沥秦瘤绰 NextTimeSpawnDamageFxInterval 阑..
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
	// 攫哩啊 ReceiveComboEnd 甫 荤侩 给窍霸 登搁 促弗 版肺甫 烹窍档废..
	AttackStateEndInternal(InLastAttackState);
}

void ABladeIITestDummyPlayer::AttackStateEndInternal(EAttackState InLastAttackState)
{
	// SkillRTManager 甫 啊瘤绊 构 窍绰 霸 乐绢辑 寇包俊 康氢阑 固磨 父茄 巴甸 哗绊 厚况狄.

	if (InLastAttackState >= EAttackState::ECS_GuardStart && InLastAttackState <= EAttackState::ECS_Evade)
	{
		::EmissiveNotifyEnd(GetMesh(), ChainBitField, GetChainMaterial(), true);
	}

	if (InLastAttackState == EAttackState::ECS_WeaponSkill)
	{
		SetupLODInfoForInGame(); // WeaponSkill 矫累 矫 楷免侩 LOD 甫 悸诀窍骨肺 登倒妨淋.
	}

	// 促澜 局聪皋捞记 矫累.. 埃拜阑 滴芭唱 官肺
	UWorld* TheWorld = GetWorld();
	if (CooltimeBetweenAnim > 0.0f && TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(CooltimeBetweenAnimTH, 
			// RandomAttackState 牢 版快 BeginNextAnimCommon 俊 公均阑 逞败林电 罚待 急琶瞪 巴捞瘤父 狼固甫 疙犬洒..
			FTimerDelegate::CreateUObject(this, &ABladeIITestDummyPlayer::BeginNextAnimCommon, (bUseRandomAttackState ? EAttackState::ECS_End : DirectAttackState)), 
			CooltimeBetweenAnim, false);
		SetDirectAttackState(EAttackState::ECS_None); // 促澜 局聪皋捞记 惯悼 傈鳖瘤 Idle 惑怕肺 绒侥.
	}
	else
	{
		BeginNextAnimCommon(); // 官肺 矫累. 泅犁 DirectAttackState 弊措肺 捞绢辑.
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
	// 惑窜狼 涝仿 subscribe 绝档废..
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

void ABladeIITestDummyPlayer::BeginNextAnimCommon(EAttackState SpecifiedNextAnim /* RandomAttackState 啊 酒聪绊 EAttackState::ECS_End 寇狼 蔼阑 临 矫 荤侩凳. */)
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(CooltimeBetweenAnimTH);
	}

	if (bUseRandomAttackState) // 罚待 犁积 矫 促澜 罚待 局聪皋捞记阑 急琶
	{
		GoToNextRandomAttackState();
	}
	else
	{ // 罚待 局丛 犁积捞 酒聪扼搁 瘤沥等 吧肺 促矫 矫累.
	  // 霓焊 state 绰 贸澜何磐 促矫 矫累秦具 秦辑 ThisTickFinalAttackState 鳖瘤 府悸捞 等促.
		const EAttackState FinalNextAnim = (SpecifiedNextAnim != EAttackState::ECS_End) ? SpecifiedNextAnim : DirectAttackState;
		if (IsOneOfComboAttackState(FinalNextAnim))
		{
			SetDirectAttackState(FinalNextAnim);
		}
	}
}

void ABladeIITestDummyPlayer::SetDirectAttackStateByKeyword(const FString& InAttackStateKeyword)
{
	// 能贾 目盖靛甫 烹秦 AttackState 瘤沥窍绰 扁瓷阑 静妨绊.

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
	// SkillAnimIdx 鳖瘤 惑技 炼例..
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

		// SkillLOD 蔼篮 1 捞 甸绢啊霸 瞪 搏单 SkillAnimIdx LOD 酒囱 吧肺 持绢林搁 绢纳 登唱..
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

	// 扁鸥 鞘夸俊 蝶扼..

	SetDirectAttackState(ParsedAttackState);
}

void ABladeIITestDummyPlayer::SetDirectAttackState(EAttackState InState)
{
	DirectAttackState = InState;

	if (IsOneOfComboAttackState(DirectAttackState)){
		ThisTickFinalAttackState = EAttackState::ECS_Combo01; // Direct 窍霸 AnimBP 俊 技泼茄促 秦档 State 赣脚捞 鉴辑措肺 罐扁 东俊 绢驴 荐 绝捞 Combo1 何磐 矫累秦具..
	}
	else {
		// 促弗 巴档 鉴辑措肺 秦具 窍绰 霸 乐阑 荐 乐促. 鞘夸窍搁 厚搅窍霸..
		ThisTickFinalAttackState = DirectAttackState;
	}

	if (IsOneOfSkillAttackState(ThisTickFinalAttackState))
	{ // Skill 矫傈 矫 漂喊洒 眠啊秦具 窍绰 芭
		SetReservedSkill(ThisTickFinalAttackState);
	}

	FinalAttackStateMaintainCount = FINAL_ATTACK_STATE_MAINTAIN_MAX_COUNT; // AttackState 甫 鉴瞒利栏肺 刘啊矫难具 窍绰 版快狼 墨款飘 勤甸傅阑 困秦
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
	// 罚待俊辑绰 WeaponSkill 救唱咳. 犬角洒 荤侩窍绰 state 甸肺.

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

	// bUseRandomAttackState 啊 true 捞搁 捞锅 葛记捞 场唱绊 唱辑 促矫 捞锅 罚待 急琶 例瞒甫 芭摹霸 瞪 巴捞促.
	SetDirectAttackState(RandAttackState);
}

/** 荤侩阑 困茄 皋牢 蜡瓶 */
ABladeIITestDummyPlayer* ABladeIITestDummyPlayer::SpawnWithStandardEquipment(UObject* WorldContextObject, EPCClass InClass, const FTransform& InSpawnTransform, 
	int32 InEquipGrade, float InSpawnFloatingUIInterval, float InSpawnDamageFxInterval, bool bShowWing, int32 InWingGrade, bool bPossessByAIController)
{
	// SpawnPlayerCharAsPuppet 救俊辑档 厚搅窍霸 何掺矾款 窿阑 窍绰单 咯扁辑档..
	// BladeIIGameMode 救俊 备泅等 Spawn 窃荐俊辑 荤侩窍绰 努贰胶 糕滚 函荐甫 浇陆 官层初绊 龋免.
	// 咯变 弊唱付 俺惯侩 扁瓷捞聪 粱 促青牢淀
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	TSubclassOf<APawn> CachedGMDefaultPawnClass = nullptr;
	if (B2GM)
	{
		CachedGMDefaultPawnClass = B2GM->DefaultPawnClass;
		B2GM->DefaultPawnClass = ABladeIITestDummyPlayer::StaticClass();
	}

	ABladeIITestDummyPlayer* SpawnedDummy = Cast<ABladeIITestDummyPlayer>(ABladeIIGameMode::SpawnPlayerCharAsPuppet(WorldContextObject, InClass, InSpawnTransform, bPossessByAIController));

	if (B2GM && CachedGMDefaultPawnClass) // 何掺矾款 窿 交 促矫 倒妨初扁
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
		// SpawnPlayerCharAsPuppet 俊辑绰 InGameOnlyInfo 啊 绝捞 登蠢单 InGameOnlyInfo 器窃秦辑 促矫陛.
		SpawnedDummy->OverrideByPCClassInfo(B2GM, ThisClassPCInfo , false);
		if (SpawnedDummy->CenterShadow)
		{
			SpawnedDummy->SetupDecalCompForCenterShadowCommon(SpawnedDummy->CenterShadow, SpawnedDummy->GetMesh());
			SpawnedDummy->CenterShadow->SetDecalMaterial(SpawnedDummy->ShadowMaterial);
		}

		// MergeSections 绰 咯扁辑 焊尘 鞘夸绝捞 抛胶飘 目盖靛肺 力绢窍搁 瞪 淀.
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