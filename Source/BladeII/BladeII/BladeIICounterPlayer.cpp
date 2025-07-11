//
//#include "BladeIICounterPlayer.h"
//
////#include "B2SkillRTManager.h"
////#include "B2CounterAttackGameMode.h"
////#include "B2UIDocBattle.h"
////#include "B2UIDocHero.h"
////#include "B2UIDocSkill.h"
////#include "B2UIDocHelper.h"
//
//ABladeIICounterPlayer::ABladeIICounterPlayer(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//
//}
//
///*
//float ABladeIICounterPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
//{
//
//}
//*/
//
//void ABladeIICounterPlayer::InitializeCombatStats()
//{
//	Super::InitializeCombatStats();
//
//	// 반격던전은 5초
//	ComboMaintainDuration = 5.0f;
//}
//
//void ABladeIICounterPlayer::InitUpdateUIDoc()
//{
//	Super::InitUpdateUIDoc();
//
//	// 기존 콤보 초기화
//	ResetComboNumTimerCallback();
//}
//
//void ABladeIICounterPlayer::BeginPlay()
//{
//	Super::BeginPlay();
//
//	OverrideSkillUIAsset(GetCurrentPlayerClass());
//
//	SetCounterAttackPoint(0);
//}
//
//void ABladeIICounterPlayer::StartSkill01()
//{
//	if (StartSkillInner(0, true) == EAttackState::ECS_None)
//		return;
//
//	SetCounterAttackPoint(0);
//
//	auto* GameMode = Cast<AB2CounterAttackGameMode>(GetGameMode());
//	BII_CHECK(GameMode);
//}
//
//void ABladeIICounterPlayer::OnCounterStart()
//{
//	AddCounterAttackPoint(1);
//	Super::OnCounterStart();
//}
//
//void ABladeIICounterPlayer::SetAttackState(EAttackState InAttackState)
//{
//	if (InAttackState == EAttackState::ECS_Evade || InAttackState == EAttackState::ECS_Dash)
//		return;
//	Super::SetAttackState(InAttackState);
//}
//
//void ABladeIICounterPlayer::PreDamageAnimation(const FDamageInfo& DamageInfo, bool IsArmorBreak)
//{
//	// 반격던전은 Guard중 GuardBreak가 없다
//	if (IsGuarding() && IsArmorBreak)
//		IsArmorBreak = false;
//
//	Super::PreDamageAnimation(DamageInfo, IsArmorBreak);
//}
//
//void ABladeIICounterPlayer::ResetComboNumTimerCallback()
//{
//	CurrentComboNum = 0;
//	auto* DocBattle = GetDocBattle();
//	if (DocBattle)
//	{
//		DocBattle->SetComboNum(CurrentComboNum);
//		DocBattle->SetIncCounterDamageCombo(0.f);
//	}
//}
//
//void ABladeIICounterPlayer::UpdateUIDocOnTargetDamaged()
//{
//	auto* DocBattle = GetDocBattle();
//	if (DocBattle)
//	{
//		DocBattle->SetComboNum(CurrentComboNum);
//	}
//}
//
//float ABladeIICounterPlayer::GetAdditionalCriticalRateByComboNum() const
//{
//	return 0.f;
//}
//
//float ABladeIICounterPlayer::GetDamageIncreaseRate() const
//{
//	auto Ret = Super::GetDamageIncreaseRate();
//
//	auto* DocBattle = GetDocBattle();
//	if (DocBattle)
//		Ret += DocBattle->GetIncCounterDamageCombo();
//
//	return Ret;
//}
//
//// 반격던전 Skill은 StateMachine의 AnimSequence에 강제로 박혀있음 ( SkillDoc 셋팅과 무관하게 정해진 스킬만 나감 ) - UI 스킬아이콘만 교체 해야함
//void ABladeIICounterPlayer::OverrideSkillUIAsset(EPCClass PCClass)
//{
//	// 어차피 Asset에 박혀있는 AnimSequence니깐 그냥 여기에 하드코딩함
//	static const int32 BindingSkillIDs[4] = { 10000, 20002, 30000, 40002 };
//
//
//	const int32 ClassIndex = static_cast<int32>(GetCurrentPlayerClass());
//	if (UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(ClassIndex))
//	{
//		if (UB2UIDocSkill* DocSkill01 = DocHero->GetSkillDocByInputIndex(0))
//			DocHero->AddSkillDoc(DocSkill01, BindingSkillIDs[ClassIndex], 0);
//	}
//}
//
//void ABladeIICounterPlayer::EnableNormalAttack()
//{
//	SetEnableAttack(true);
//	ModifyInputCommand(true);
//}
//
//void ABladeIICounterPlayer::DisableNormalAttack()
//{
//	StopAttackFromInput(); //강제로 종료.
//	StopRepeatingInput();
//	SetEnableAttack(false);
//	ModifyInputCommand(false);
//}
//
//void ABladeIICounterPlayer::ModifyInputCommand(bool EnableNormalAttack)
//{
//	if (!InputComponent)
//		return;
//
//	InputComponent->ClearActionBindings();
//
//	if (bEnableNormalAttack)
//	{
//		FInputActionBinding& FirePressed = InputComponent->BindAction("Attack", IE_Pressed, this, &ABladeIIPlayer::StartAttackFromInput);
//		FInputActionBinding& FireReleased = InputComponent->BindAction("Attack", IE_Released, this, &ABladeIIPlayer::StopAttackFromInput);
//	}
//
//	FInputActionBinding& GuardPressed = InputComponent->BindAction("Guard", IE_Pressed, this, &ABladeIIPlayer::StartGuardFromInput);
//	FInputActionBinding& GuardReleased = InputComponent->BindAction("Guard", IE_Released, this, &ABladeIIPlayer::StopGuardFromInput);
//
//	// handle touch devices
//	InputComponent->BindTouch(IE_Pressed, this, &ABladeIIPlayer::TouchStarted);
//	InputComponent->BindTouch(IE_Released, this, &ABladeIIPlayer::TouchStopped);
//}
//
//void ABladeIICounterPlayer::AddCounterAttackPoint(int32 ToAddPoint)
//{
//	SetCounterAttackPoint(CounterAttackPoint + ToAddPoint);
//}
//
//void ABladeIICounterPlayer::SetCounterAttackPoint(int32 ToSetPoint)
//{
//	//GetSkillRTManager();
//	//if (CachedSkillRTManager)
//	//{
//	//	CounterAttackPoint = FMath::Clamp(ToSetPoint, 0, MAX_COUNTER_ATTACK_POINT);
//	//	CachedSkillRTManager->UpdateCounterAttackPoint(CounterAttackPoint);
//	//}
//}