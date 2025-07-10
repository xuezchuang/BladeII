
#include "B2BTTask_DynamicMoveTo.h"
//#include "BladeII.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "Tasks/AITask_MoveTo.h"

#include "BladeIIUtil.h"


UB2BTTask_DynamicMoveTo::UB2BTTask_DynamicMoveTo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Dynamic Move To";
}

void UB2BTTask_DynamicMoveTo::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		AcceptableRadiusKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}

UAITask_MoveTo * UB2BTTask_DynamicMoveTo::PrepareMoveTask(UBehaviorTreeComponent & OwnerComp, UAITask_MoveTo * ExistingTask, FAIMoveRequest & MoveRequest)
{
	UAITask_MoveTo* MoveTask = ExistingTask ? ExistingTask : NewBTAITask<UAITask_MoveTo>(OwnerComp);
	if (MoveTask)
	{
		if (OwnerComp.GetBlackboardComponent()->IsValidKey(AcceptableRadiusKey.GetSelectedKeyID()))
		{
			float Radius = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Float>(AcceptableRadiusKey.SelectedKeyName);
			MoveRequest.SetAcceptanceRadius(Radius >= 0.f ? Radius : AcceptableRadius);
		}
		MoveTask->SetUp(MoveTask->GetAIController(), MoveRequest);
	}
	return MoveTask;
}
