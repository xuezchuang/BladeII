#include "BladeII.h"
#include "B2BTService_CheckPlayerTag.h"

#include "BladeIIPlayer.h"
#include "B2SkillRTManager.h"
#include "B2UIDocHelper.h"
#include "BladeIIGameMode.h"
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
#include "B2BTTask_PCAutoAttack.h"
#endif

UB2BTService_CheckPlayerTag::UB2BTService_CheckPlayerTag(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "CheckPlayerTag";
	Interval = 0.5f;
	RandomDeviation = 0.0f;
}

void UB2BTService_CheckPlayerTag::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (UB2BTTask_PCAutoAttack::bDevAllowComboAttackOnly) // Auto 도중 콤보 공격만 허용할 시에는 태그도 막는다. 일관적인 플레이 패턴을 위해
	{
		return;
	}
#endif

	auto*	Owner = Cast<AController>(OwnerComp.GetOwner());
	auto*	PlayerPawn = Owner ? Cast<ABladeIIPlayer>(Owner->GetPawn()) : nullptr;
	auto*	SkillRTManager = PlayerPawn ? PlayerPawn->GetSkillRTManager() : nullptr;

	UB2UIDocBattle* DocBattle	= UB2UIDocHelper::GetDocBattle();
	//const bool bIsForTagAttack	= DocBattle ? DocBattle->GetIsTagAttackSuggested() : false;

	auto*		GameMode	= Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Owner));
	const bool	bAllowToTag = GameMode->GetGameRule()->AllowToTag();
	
	if (!(SkillRTManager && SkillRTManager->IsTagPossible() && bAllowToTag))
		return;

	if (/*bIsForTagAttack || */(PlayerPawn->Health / PlayerPawn->MaxHealth <= 0.2f && SkillRTManager->GetInactiveCharacterHealth() / SkillRTManager->GetInactiveCharacterMaxHealth() >= 0.3f))
	{
		PlayerPawn->StartTag();
	}
}