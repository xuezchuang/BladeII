#include "BladeII.h"
#include "B2BTTask_SelectAttackMethod.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIIPlayerAIController.h"
#include "BladeIITMAIPlayer.h"
#include "B2SkillRTManager.h"
#include "B2AI.h"
#include "B2Lottery.h"


#include "BladeIIUtil.h"

UB2BTTask_SelectAttackMethod::UB2BTTask_SelectAttackMethod(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Select Attack Method";
}

EBTNodeResult::Type UB2BTTask_SelectAttackMethod::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EBTNodeResult::Type RetResult = PerformTask(OwnerComp, NodeMemory);

	return RetResult;
}

EBTNodeResult::Type UB2BTTask_SelectAttackMethod::PerformTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* Blackboard = OwnerComp.GetBlackboardComponent();
	auto* Controller = Cast<AAIController>(OwnerComp.GetAIOwner());

	BII_CHECK(Controller);
	BII_CHECK(Blackboard);

	auto* Player = Cast<ABladeIIPlayer>(Controller->GetPawn());
	BII_CHECK(Player);

	auto* CachedSkillRTManager = Player->GetSkillRTManager();
	BII_CHECK(CachedSkillRTManager);

	const FString	AttackModeKey(TEXT("SelectedAttack"));
	const FString	AvailableSkillNumKey(TEXT("AvailableSkillNum"));

	ATTACK_MODE SelectedAttack = ATTACK_MODE::EAM_NORMAL;

// 	if (bThinkTagAsSkill && CachedSkillRTManager->IsTagSuggestedPlayerCondition())
// 	{
// 		SelectedAttack = ATTACK_MODE::EAM_TAG;
// 	}
// 	else
// 	{
		TArray<int32> UsableSkills;

		for (int32 SkillIndex = 0; SkillIndex < MAX_ACTIVE_SKILL_INPUT; ++SkillIndex)
		{
			if (!CachedSkillRTManager->IsSkillCurrentlyUsable(SkillIndex, false))
				continue;

			UsableSkills.Add(SkillIndex);
		}

//		auto	AddTagToSkill = (bThinkTagAsSkill && CachedSkillRTManager->IsTagPossible()) ? 1 : 0;

		Lottery	Lotto;

// 		if (AddTagToSkill)
// 		{
// 			if (UsableSkills.Num())
// 			{
// 				Lotto.PushBall(static_cast<uint8>(ATTACK_MODE::EAM_SKILL), 0.15f);
// 				Lotto.PushBall(static_cast<uint8>(ATTACK_MODE::EAM_TAG), 0.15f);
// 				Lotto.PushBall(static_cast<uint8>(ATTACK_MODE::EAM_NORMAL), 0.7f);
// 			}
// 			else
// 			{
// 				Lotto.PushBall(static_cast<uint8>(ATTACK_MODE::EAM_TAG), 0.15f);
// 				Lotto.PushBall(static_cast<uint8>(ATTACK_MODE::EAM_NORMAL), 0.85f);
// 			}
// 		}
// 		else 
// 		{
			if (UsableSkills.Num())
			{
				Lotto.PushBall(static_cast<uint8>(ATTACK_MODE::EAM_SKILL), 0.25f);
				Lotto.PushBall(static_cast<uint8>(ATTACK_MODE::EAM_NORMAL), 0.75f);
			}
			else
				Lotto.PushBall(static_cast<uint8>(ATTACK_MODE::EAM_NORMAL), 1.f);
//		}

		auto	ball = static_cast<ATTACK_MODE>(Lotto.PopBall());

		Blackboard->SetValueAsInt(FName(*AvailableSkillNumKey), UsableSkills.Num());
		SelectedAttack = ball;
//	}

	Blackboard->SetValueAsEnum(FName(*AttackModeKey), static_cast<uint8>(SelectedAttack));

	return EBTNodeResult::Succeeded;
}