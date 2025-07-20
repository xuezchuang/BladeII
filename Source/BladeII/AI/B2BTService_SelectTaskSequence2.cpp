
#include "B2BTService_SelectTaskSequence2.h"
//#include "BladeII.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "B2AI.h"
#include "BladeIIGameMode.h"
#include "BladeIIPlayer.h"
#include "BladeIIPlayerAIController.h"
#include "B2HomePoint.h"
#include "B2Lottery.h"

UB2BTService_SelectTaskSequence2::UB2BTService_SelectTaskSequence2(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Select Task Sequence II";
	Interval = 0.25f;
	RandomDeviation = 0.25f;
}

void UB2BTService_SelectTaskSequence2::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto*	Controller	= Cast<ABladeIIPlayerAIController>(OwnerComp.GetOwner());
	BII_CHECK(Controller);

	auto*	Player		= Cast<ABladeIIPlayer>(Controller->GetPawn());
	auto*	Blackboard	= OwnerComp.GetBlackboardComponent();
	auto*	GameMode	= Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Controller));
	BII_CHECK(GameMode);

	auto*	HomePoint	= GameMode->GetHomePoint();

	//Controller->CurrentTargetMob 焊促 困俊 乐绢具 窃. 咯扁辑 CurrentTargetMob阑 汲沥 窃
	Controller->UpdateTarget();
	ABladeIICharacter* pTarget = Controller->CurrentTargetMob;

	BII_CHECK(Player);
	BII_CHECK(Blackboard);

	const FString EnumKey(TEXT("SelectedTaskMode"));
	const FString TargetToBeat(TEXT("TargetToBeat"));
	
	// 鸥百 绝栏搁 归权
	if (!pTarget)
	{
		Blackboard->SetValueAsEnum(FName(*EnumKey), static_cast<uint8>(BTTASK_MODE::EBM_RETURNHOME));
		return;
	}

	// 鸥百各 汲沥
	Blackboard->SetValueAsObject(FName(*TargetToBeat), pTarget);

	// 鸥百苞狼 芭府
	float fTargetDistance = (pTarget->GetActorLocation() - Player->GetActorLocation()).Size();
		
	if (fTargetDistance < AttackTargetDistance)
	{
		// 傍拜 裹困 救捞搁 傍拜
		Blackboard->SetValueAsEnum(FName(*EnumKey), static_cast<uint8>(BTTASK_MODE::EBM_ATTACK));
	}
	else if (fTargetDistance < SearchTargetDistance)
	{
		// 牢侥 裹困 救捞搁 捞悼
		Blackboard->SetValueAsEnum(FName(*EnumKey), static_cast<uint8>(BTTASK_MODE::EBM_MOVE));
	}
	else
	{
		// 弊寇浚 归权
		Blackboard->SetValueAsEnum(FName(*EnumKey), static_cast<uint8>(BTTASK_MODE::EBM_RETURNHOME));
		Blackboard->SetValueAsObject(FName(*TargetToBeat), HomePoint);
	}

}
