
#include "BladeIICounterPlayer.h"
#include "B2SkillRTManager.h"
#include "B2CounterAttackGameMode.h"
#include "B2UIDocBattle.h"
#include "B2UIDocHero.h"
#include "B2UIDocSkill.h"
#include "B2UIDocHelper.h"
#include "BladeIIUtil.h"

ABladeIICounterPlayer::ABladeIICounterPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


float ABladeIICounterPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	return 0.0f;
}


void ABladeIICounterPlayer::InitializeCombatStats()
{
	Super::InitializeCombatStats();

	// 馆拜带傈篮 5檬
	ComboMaintainDuration = 5.0f;
}

void ABladeIICounterPlayer::InitUpdateUIDoc()
{
	Super::InitUpdateUIDoc();

	// 扁粮 霓焊 檬扁拳
	ResetComboNumTimerCallback();
}

void ABladeIICounterPlayer::BeginPlay()
{
	Super::BeginPlay();

	OverrideSkillUIAsset(GetCurrentPlayerClass());

	SetCounterAttackPoint(0);
}

void ABladeIICounterPlayer::StartSkill01()
{
	if (StartSkillInner(0, true) == EAttackState::ECS_None)
		return;

	SetCounterAttackPoint(0);

	auto* GameMode = Cast<AB2CounterAttackGameMode>(GetGameMode());
	BII_CHECK(GameMode);
}

void ABladeIICounterPlayer::OnCounterStart()
{
	AddCounterAttackPoint(1);
	Super::OnCounterStart();
}

void ABladeIICounterPlayer::SetAttackState(EAttackState InAttackState)
{
	if (InAttackState == EAttackState::ECS_Evade || InAttackState == EAttackState::ECS_Dash)
		return;
	Super::SetAttackState(InAttackState);
}

void ABladeIICounterPlayer::PreDamageAnimation(const FDamageInfo& DamageInfo, bool IsArmorBreak)
{
	// 馆拜带傈篮 Guard吝 GuardBreak啊 绝促
	if (IsGuarding() && IsArmorBreak)
		IsArmorBreak = false;

	Super::PreDamageAnimation(DamageInfo, IsArmorBreak);
}

void ABladeIICounterPlayer::ResetComboNumTimerCallback()
{
	CurrentComboNum = 0;
	auto* DocBattle = GetDocBattle();
	if (DocBattle)
	{
		DocBattle->SetComboNum(CurrentComboNum);
		DocBattle->SetIncCounterDamageCombo(0.f);
	}
}

void ABladeIICounterPlayer::UpdateUIDocOnTargetDamaged()
{
	auto* DocBattle = GetDocBattle();
	if (DocBattle)
	{
		DocBattle->SetComboNum(CurrentComboNum);
	}
}

float ABladeIICounterPlayer::GetAdditionalCriticalRateByComboNum() const
{
	return 0.f;
}

float ABladeIICounterPlayer::GetDamageIncreaseRate() const
{
	auto Ret = Super::GetDamageIncreaseRate();

	auto* DocBattle = GetDocBattle();
	if (DocBattle)
		Ret += DocBattle->GetIncCounterDamageCombo();

	return Ret;
}

// 馆拜带傈 Skill篮 StateMachine狼 AnimSequence俊 碍力肺 冠囚乐澜 ( SkillDoc 悸泼苞 公包窍霸 沥秦柳 胶懦父 唱皑 ) - UI 胶懦酒捞能父 背眉 秦具窃
void ABladeIICounterPlayer::OverrideSkillUIAsset(EPCClass PCClass)
{
	// 绢瞒乔 Asset俊 冠囚乐绰 AnimSequence聪瘪 弊成 咯扁俊 窍靛内爹窃
	static const int32 BindingSkillIDs[4] = { 10000, 20002, 30000, 40002 };


	const int32 ClassIndex = static_cast<int32>(GetCurrentPlayerClass());
	if (UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(ClassIndex))
	{
		if (UB2UIDocSkill* DocSkill01 = DocHero->GetSkillDocByInputIndex(0))
			DocHero->AddSkillDoc(DocSkill01, BindingSkillIDs[ClassIndex], 0);
	}
}

void ABladeIICounterPlayer::EnableNormalAttack()
{
	SetEnableAttack(true);
	ModifyInputCommand(true);
}

void ABladeIICounterPlayer::DisableNormalAttack()
{
	StopAttackFromInput(); //碍力肺 辆丰.
	StopRepeatingInput();
	SetEnableAttack(false);
	ModifyInputCommand(false);
}

void ABladeIICounterPlayer::ModifyInputCommand(bool EnableNormalAttack)
{
	if (!InputComponent)
		return;

	InputComponent->ClearActionBindings();

	if (bEnableNormalAttack)
	{
		FInputActionBinding& FirePressed = InputComponent->BindAction("Attack", IE_Pressed, this, &ABladeIIPlayer::StartAttackFromInput);
		FInputActionBinding& FireReleased = InputComponent->BindAction("Attack", IE_Released, this, &ABladeIIPlayer::StopAttackFromInput);
	}

	FInputActionBinding& GuardPressed = InputComponent->BindAction("Guard", IE_Pressed, this, &ABladeIIPlayer::StartGuardFromInput);
	FInputActionBinding& GuardReleased = InputComponent->BindAction("Guard", IE_Released, this, &ABladeIIPlayer::StopGuardFromInput);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ABladeIIPlayer::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ABladeIIPlayer::TouchStopped);
}

void ABladeIICounterPlayer::AddCounterAttackPoint(int32 ToAddPoint)
{
	SetCounterAttackPoint(CounterAttackPoint + ToAddPoint);
}

void ABladeIICounterPlayer::SetCounterAttackPoint(int32 ToSetPoint)
{
	GetSkillRTManager();
	if (CachedSkillRTManager)
	{
		CounterAttackPoint = FMath::Clamp(ToSetPoint, 0, MAX_COUNTER_ATTACK_POINT);
		CachedSkillRTManager->UpdateCounterAttackPoint(CounterAttackPoint);
	}
}