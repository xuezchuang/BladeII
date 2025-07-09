#include "BladeII.h"
#include "B2BTService_CheckAggroByTaunt.h"

#include "AIController.h"
#include "BladeIICharacter.h"
#include "B2GeneralBuffs.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

void UB2BTService_CheckAggroByTaunt::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		TauntListKey.ResolveSelectedKey(*BBAsset);
	}

	if(TauntList == nullptr)
		TauntList = NewObject<UB2CharacterContainer>(this, *FString::Printf(TEXT("TauntList_%s"), *this->GetName()), RF_Transient);
}


void UB2BTService_CheckAggroByTaunt::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//AAIController* MyController = OwnerComp.GetAIOwner();
	//ABladeIICharacter* ControlledCharacter = MyController ? Cast<ABladeIICharacter>(MyController->GetPawn()) : nullptr;

	//if (ControlledCharacter == nullptr)
	//	return;

	//bool Result = false;
	//TArray<ABladeIICharacter*> TempTauntList;

	//for (TActorIterator<ABladeIICharacter> ActorItr(ControlledCharacter->GetWorld()); ActorItr; ++ActorItr)
	//{
	//	ABladeIICharacter *InWorldChracter = *ActorItr;
	//	if (InWorldChracter->IsValidObj() &&
	//		InWorldChracter->IsAlive() &&
	//		InWorldChracter != ControlledCharacter							&&
	//		InWorldChracter->GetTeamNum() != ControlledCharacter->GetTeamNum())
	//	{
	//		UB2Buff_Base* TauntBuff = InWorldChracter->GetBuff(EBuffType::Debuff_Taunt);
	//		if (TauntBuff && TauntBuff->IsActive() && TauntBuff->GetBuffCauser() == ControlledCharacter)
	//		{
	//			TempTauntList.Add(InWorldChracter);
	//			Result = true;
	//		}
	//	}
	//}
	//TauntList->Chracters = TempTauntList;

	//if(ControlledCharacter->bInPhaseAnimation)
	//{
	//	Result = false;
	//	TauntList->Chracters.Empty();
	//}

	//OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(BlackboardKey.SelectedKeyName, Result);
	//OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(TauntListKey.SelectedKeyName, TauntList);
}