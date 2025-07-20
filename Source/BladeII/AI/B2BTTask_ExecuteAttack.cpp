
#include "B2BTTask_ExecuteAttack.h"
//#include "BladeII.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BladeIIPlayer.h"
#include "BladeIITMAIPlayer.h"
#include "B2SkillRTManager.h"
#include "B2AI.h"

UB2BTTask_ExecuteAttack::UB2BTTask_ExecuteAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Execute Attack";
}

EBTNodeResult::Type UB2BTTask_ExecuteAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EBTNodeResult::Type RetResult = PerformTask(OwnerComp, NodeMemory);

	return RetResult;
}

EBTNodeResult::Type UB2BTTask_ExecuteAttack::PerformTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	RetResult	= EBTNodeResult::Succeeded;
	auto* Blackboard				= OwnerComp.GetBlackboardComponent();
	auto* Controller				= Cast<AAIController>(OwnerComp.GetAIOwner());

	BII_CHECK(Controller);
	BII_CHECK(Blackboard);

	auto* Player					= Cast<ABladeIIPlayer>(Controller->GetPawn());
	BII_CHECK(Player);

	auto*	CachedSkillRTManager	= Player->GetSkillRTManager();
	BII_CHECK(CachedSkillRTManager);

	const FString	AttackModeKey(TEXT("SelectedAttack"));
	const FString	AvailableSkillNumKey(TEXT("AvailableSkillNum"));

	auto	Attack		= static_cast<ATTACK_MODE>(Blackboard->GetValueAsEnum(FName(*AttackModeKey)));
	auto	SkillNum	= Blackboard->GetValueAsInt(FName(*AvailableSkillNumKey));

	if (Attack == ATTACK_MODE::EAM_NORMAL)
	{
		Player->StartAttack();
	}
	else if (Attack == ATTACK_MODE::EAM_SKILL)
	{
		//*
		Player->StartAnySkill();
		/*/
		if (Player->TargetActor == nullptr) return RetResult; //target捞 绝栏搁 酒贰甫 荐青且 鞘夸啊 绝澜

		TArray<int32> UsableSkills;

		for (int32 SkillIndex = 0; SkillIndex < MAX_ACTIVE_SKILL_INPUT; ++SkillIndex)
		{
			if (!CachedSkillRTManager->IsSkillCurrentlyUsable(SkillIndex))
				continue;

			UsableSkills.Add(SkillIndex);
		}

		auto Skill = FMath::RandRange(0, UsableSkills.Num() - 1);
		const float skillShootingRange = CachedSkillRTManager->GetSkillShootingRangeForAI(Player, Skill);
		const float fTargetDistance = (Player->GetActorLocation() - Player->TargetActor->GetActorLocation()).Size();

		if(fTargetDistance > skillShootingRange)
			Player->StartAttack();
		else
			Player->StartSkillInner(UsableSkills[Skill]);
		//*/
	}
	else if (Attack == ATTACK_MODE::EAM_TAG)
	{
		Player->StartTag();
	}
	else
	{
		RetResult = EBTNodeResult::Failed;
	}


	return RetResult;
}
