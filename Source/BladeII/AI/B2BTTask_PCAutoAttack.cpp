﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "B2BTTask_PCAutoAttack.h"
//#include "BladeII.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "EngineUtils.h"
#include "AIController.h"
#include "BladeIIPlayerController.h"
#include "BladeIICharacter.h"
#include "BladeIIPlayer.h"
#include "B2SkillRTManager.h"
#include "B2StageGameMode.h"
#include "B2StageManager.h"

UB2BTTask_PCAutoAttack::UB2BTTask_PCAutoAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName							= "PC Auto Attack";
	bNotifyTick							= true;

	SkillWorthyMonsterLevelScale		= 1.0f;
	SkillWorthyMobDistance				= 700.0f;

	CounterProbability					= 0.05f;
	CounterWorthyMobDistance			= 400.0f;
	CounterWorthyAttackingMonsterCount	= 2;
	CounterWorthyMonsterLevelScale		= 0.3f;

	bInAttackCooling					= false;
	MinAttackInterval					= 0.1f;

	bTryingCounter						= false;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (HasAnyFlags(RF_ClassDefaultObject) && GConfig)
	{	// 贸澜 茄锅 汲沥阑 佬绢甸烙
		GConfig->GetBool(TEXT("DevCheat"), TEXT("AllowAutoComboOnly"), bDevAllowComboAttackOnly, GGameIni);
	}
#endif
}

EBTNodeResult::Type UB2BTTask_PCAutoAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type RetResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	// Result from Super will be overwritten.
	RetResult = PerformAttackTask(OwnerComp, NodeMemory, 0.0f);

	return RetResult;
}

EBTNodeResult::Type UB2BTTask_PCAutoAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type RetResult = Super::AbortTask(OwnerComp, NodeMemory);
		
	return RetResult;
}

void UB2BTTask_PCAutoAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	EBTNodeResult::Type TaskResult = PerformAttackTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (TaskResult == EBTNodeResult::InProgress)
		return;
	
	FinishLatentTask(OwnerComp, TaskResult);
		
	// Any other place..?
	OnQuitTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UB2BTTask_PCAutoAttack::PerformAttackObjectTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EBTNodeResult::Type			RetResult = EBTNodeResult::Failed;
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	auto*						AIController = Cast<AAIController>(OwnerComp.GetAIOwner());

	if (!AIController)
		return RetResult;

	BII_CHECK(MyBlackboard);

	AActor* BlackboardTarget = NULL;

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		BlackboardTarget = Cast<AActor>(KeyValue);
	}

	if (BlackboardTarget == NULL)
		// No more target. It will go to the next task for Selector.
		return RetResult;

	RetResult = EBTNodeResult::InProgress;

	if (ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(AIController->GetPawn()))
	{
		B2Player->TargetActor = NULL;
		// Target is set by PlayerController, but we might check if target is properly updated in auto play mode.
		if (B2Player->IsSkillAttacking() == false)
		{
			B2Player->UpdateTargetObject();
			B2Player->StartAttack();
			PostAttackSetAttackCooling(B2Player);
		}

		if ((B2Player->TargetObject && B2Player->TargetObject != BlackboardTarget) || B2Player->TargetObject == NULL)
		{
			RetResult = EBTNodeResult::Succeeded;
		}
	}

	return RetResult;

}

EBTNodeResult::Type UB2BTTask_PCAutoAttack::PerformAttackTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EBTNodeResult::Type			RetResult			= EBTNodeResult::Failed;
	const UBlackboardComponent* MyBlackboard		= OwnerComp.GetBlackboardComponent();
	auto*						AIController		= Cast<AAIController>(OwnerComp.GetAIOwner());

	if (!AIController)
		return RetResult;

	BII_CHECK(MyBlackboard);

	ABladeIICharacter* BlackboardTargetCharacter	= NULL;

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue			= MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		BlackboardTargetCharacter	= Cast<ABladeIICharacter>(KeyValue);
	}

	if (BlackboardTargetCharacter == NULL)
		// No more target. It will go to the next task for Selector.
		return PerformAttackObjectTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (BlackboardTargetCharacter->IsAlive() == false)
	{
		// Report Succeeded, then with a Selector, it will first try AutoAttack again with other target.
		// If no target is found at next try, it will go to Failed, and do the next task.
		RetResult = EBTNodeResult::Succeeded;
	}
	else
	{
		RetResult = EBTNodeResult::InProgress;

		if (ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(AIController->GetPawn()))
		{
			// Target is set by PlayerController, but we might check if target is properly updated in auto play mode.

			if (B2Player->IsQTEMounting())
			{
				B2Player->StartAttack();
				return EBTNodeResult::Succeeded;
			}
			// Don't try other attack action if we are keep guard action for counter.
			else if (bInAttackCooling == false && bTryingCounter == false)
			{
				EPCAutoAttackType::Type AttackTypeThis = ChooseAttackType(OwnerComp, NodeMemory);

				switch (AttackTypeThis)
				{
				case EPCAutoAttackType::EPCAAT_Combo:
					{
						if ( B2Player->IsSkillAttacking() == false )
						{
							// 磊悼傍拜 弥措芭府俊 鸥百捞 器窃瞪锭父 傍拜贸府
							float TargetDistance = B2Player->GetTargetLocationVectorFromPlayer( BlackboardTargetCharacter ).Size2D();
							const float MaxDistanceToAllowAutoAttack = B2Player->MaxDistanceToAllowAutoAttack;
							if ( TargetDistance < MaxDistanceToAllowAutoAttack )
							{
								B2Player->StartAttack();
								PostAttackSetAttackCooling( B2Player );
							}
							else
							{
								// 磊悼傍拜 弥措芭府焊促 努版快 怕胶农 搬苞甫 [己傍] 贸府甫 窍咯 伎泛磐甫 狐廉唱埃饶, 厚庆捞滚 飘府狼 [捞悼] 怕胶农啊 角青瞪 荐 乐档废 茄促
								// [林狼] 磊悼傍拜 弥措芭府绰 亲惑 [捞悼] 怕胶农狼 Acceptable Radius 焊促 目具茄促
								return EBTNodeResult::Succeeded;
							}
						}
					}
					break;
				case EPCAutoAttackType::EPCAAT_Skill:
					{
						// TODO : Need skill number
						B2Player->StartAnySkill();
						PostAttackSetAttackCooling(B2Player);
					}
					break;
				case EPCAutoAttackType::EPCAAT_Counter:
					{
						//B2Player->StartGuard(); // Not right at this moment..
						bTryingCounter = true;

						// Start the guard motion after a little bit while, and keep guard motion for possible time (CounterTime).
						const float GuardStartDelay = 0.3f; // Magic number. Cannot be too long..
							
						B2Player->GetWorldTimerManager().SetTimer(GuardStartTimerHandle, FTimerDelegate::CreateUObject(this, &UB2BTTask_PCAutoAttack::TryingCounterStartTimerCallback, B2Player), GuardStartDelay, false);
						B2Player->GetWorldTimerManager().SetTimer(GuardFinishTimerHandle, FTimerDelegate::CreateUObject(this, &UB2BTTask_PCAutoAttack::TryingCounterFinishTimerCallback, B2Player), GuardStartDelay + B2Player->CounterCheckTime, false);
					}
					break;
				default:					
					return EBTNodeResult::Succeeded;
				}
			}
									
			// The default controller of player should be the player controller, although when it is possessed by auto AI controller too.
			//if (ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(B2Player->GetController()))
			{
				// We can do the attack even when the AIController's target and PC's target are different,
				// but the auto player needs to move to the target mob anyway. 
				// So, give a chance to update the blackboard target.
				// For any reason, if PC's TargetActor is NULL, better consider give a chance to move..
				if ((B2Player->TargetActor && B2Player->TargetActor != BlackboardTargetCharacter) || B2Player->TargetActor == NULL)
				{
					RetResult = EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			// 迄捞 绝阑版快 肺拿敲饭捞绢 荤侩
			ABladeIIPlayer* B2LocalPlayer = Cast<ABladeIIPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());

			if (B2LocalPlayer->IsQTEMounting())
			{
				B2LocalPlayer->StartAttack();
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return RetResult;
}

EPCAutoAttackType::Type UB2BTTask_PCAutoAttack::ChooseAttackType(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EPCAutoAttackType::Type RetType = EPCAutoAttackType::EPCAAT_Combo;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (bDevAllowComboAttackOnly)
	{
		return EPCAutoAttackType::EPCAAT_Combo;
	}
#endif

	if (CanDoSkillAttack(OwnerComp, NodeMemory) && IsWorthDoSkillAttack(OwnerComp, NodeMemory))
	{
		// Check for "WorthDo" later, it will be slower than "CanDo"
		RetType = EPCAutoAttackType::EPCAAT_Skill;
	}

	if (RetType == EPCAutoAttackType::EPCAAT_Combo && CanDoCounterAttack(OwnerComp, NodeMemory) && IsWorthDoCounterAttack(OwnerComp, NodeMemory))
	{
		// Try counter.
		RetType = EPCAutoAttackType::EPCAAT_Counter;
	}

	return RetType;
}

bool UB2BTTask_PCAutoAttack::CanDoSkillAttack(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto*					AIController	= Cast<AAIController>(OwnerComp.GetAIOwner());
	ABladeIIPlayer*			B2Player		= AIController ? Cast<ABladeIIPlayer>(AIController->GetPawn()) : NULL;

	if (!B2Player || !B2Player->GetSkillRTManager())
		return false;

	/*
	// Stage Buff 昏力 抗沥
	AB2StageGameMode* SGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(B2Player));
	if (SGM && !SGM->HasStageBuff(EStageBuffType::SBT_AutoSkill)){ // StageGameMode 啊 酒匆 矫 扁夯 啊瓷窍霸 且瘤 绢冻瘤绰..
		return false;
	}
	*/

	TArray<EAttackState> AvailableSkillAttackStates; 
	bool AbleSkill = B2Player->AutoAttack_AbleSkill(); //  EAttackState::ECS_None or  EAttackState::ECS_Combo

	return (AbleSkill && B2Player->GetSkillRTManager()->GetCurrentlyAvailableSkillAttackState(AvailableSkillAttackStates) == true);
}

bool UB2BTTask_PCAutoAttack::IsWorthDoSkillAttack(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto*					AIController	= Cast<AAIController>(OwnerComp.GetAIOwner());
	ABladeIIPlayer*			B2Player		= AIController ? Cast<ABladeIIPlayer>(AIController->GetPawn()) : NULL;

	if (!B2Player)
		return false;


	const float PlayerCombatGauge				= GetCharacterRoughCombatGauge(B2Player);
	float		MobCombatGaugeTotal				= 0.0f;

	TArray<ABladeIICharacter*> AllMobList;
	GetWorldMobList(AllMobList);

	int32 SkillWorthyWithinMobCount = 0;
	for (ABladeIICharacter* CurrChar : AllMobList)
	{
		if (!(CurrChar && CurrChar->bIsDying == false))
			continue;

//		auto* CheckPlayer			= Cast<ABladeIIPlayer>(CurrChar);
//		auto* CheckOpponentPlayer	= Cast<ABladeIIRemotePlayer>(CurrChar);
//		if (CheckPlayer && !CheckOpponentPlayer)
//			continue; // Not for player.. or filtering for other classes too..?

		if (B2Player->GetTargetLocationVectorFromPlayer(CurrChar).Size() > SkillWorthyMobDistance)
			continue;

		++SkillWorthyWithinMobCount;
		// Now it is close enough, let's consider it for skill attack.
		MobCombatGaugeTotal += GetCharacterRoughCombatGauge(CurrChar);
	}

	// Surrounded by mobs strong enough.. 弊府绊 给秦档 裹困 救俊 茄付府绰 乐绢具 胶懦阑 朝府绰 吧肺. SkillWorthyMonsterLevelScale 捞 0 捞扼绊 倾傍俊 措绊 惯荤秦 搭 荐绰 绝垒酒?
	return (SkillWorthyWithinMobCount > 0) && (MobCombatGaugeTotal / PlayerCombatGauge >= SkillWorthyMonsterLevelScale);
}

float UB2BTTask_PCAutoAttack::GetCharacterRoughCombatGauge(class ABladeIICharacter* InCharacter)
{
	// Still respect MaxHealth, because it is likely to reflect general level..
	// 绢瞒乔 惑措利牢 厚背捞骨肺 咯扁辑 酒林 缴坷茄 拌魂阑 且 鞘夸鳖柳 绝摆促.
	float HPSum = InCharacter->GetHealth() + InCharacter->GetMaxHealth();
	float ADSum = InCharacter->AttackLevel + InCharacter->DefenseLevel; // ABladeIICharacter::InitializeCombatStats 俊辑 诀单捞飘 登绰 蔼甸

	if (ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(InCharacter))
	{
		// Might need some player specific handling..
	}
	
	return HPSum + ADSum;
}

bool UB2BTTask_PCAutoAttack::CanDoCounterAttack(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto*					AIController	= Cast<AAIController>(OwnerComp.GetAIOwner());
	ABladeIIPlayer*			B2Player		= AIController ? Cast<ABladeIIPlayer>(AIController->GetPawn()) : NULL;

	if (B2Player && B2Player->IsAttacking())
	{
		// Guard can cancel attack without comboend notify. Don't try while attacking.
		return false;
	}

	// This will be decided by other condition like user's ticket, etc.. It's temporary now.
	if (FMath::FRandRange(0.0f, 1.0f) < CounterProbability)
	{
		return true;
	}

	return false;
}

bool UB2BTTask_PCAutoAttack::IsWorthDoCounterAttack(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto*					AIController	= Cast<AAIController>(OwnerComp.GetAIOwner());
	ABladeIIPlayer*			B2Player		= AIController ? Cast<ABladeIIPlayer>(AIController->GetPawn()) : NULL;;

	if (!B2Player)
		return false;

	const float				PlayerCombatGauge	= GetCharacterRoughCombatGauge(B2Player);
	float					MobCombatGaugeTotal = 0.0f;
	int32					AttackingMobCount	= 0;

	TArray<ABladeIICharacter*> AllMobList;
	GetWorldMobList(AllMobList);

	for (ABladeIICharacter* CurrChar : AllMobList)
	{
		if (CurrChar && CurrChar->bIsDying == false)
		{
			ABladeIIPlayer* CheckPlayer = Cast<ABladeIIPlayer>(CurrChar);
			if (CheckPlayer)
				continue; // Not for player.. or filtering for other classes too..?

			if (!((CurrChar->GetActorLocation() - B2Player->GetActorLocation()).Size() <= CounterWorthyMobDistance))
				continue;

			if (CurrChar->IsAttacking())
			{
				++AttackingMobCount;
			}
			MobCombatGaugeTotal += GetCharacterRoughCombatGauge(CurrChar);
		}
	}

	// See both how strong and how many.
	return (MobCombatGaugeTotal / PlayerCombatGauge >= CounterWorthyMonsterLevelScale && AttackingMobCount >= CounterWorthyAttackingMonsterCount);
}

void UB2BTTask_PCAutoAttack::OnQuitTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto*					AIController	= Cast<AAIController>(OwnerComp.GetAIOwner());
	ABladeIIPlayer*			B2Player		= AIController ? Cast<ABladeIIPlayer>(AIController->GetPawn()) : NULL;

	if (!B2Player)
		return;

	// Ensure quit guard action..       
	if (B2Player->IsGuarding())
	{
		B2Player->StopGuard();
	}
	else if (B2Player->IsSkillAttacking() == false)
	{
		B2Player->StopAttack();
	}

	if (bTryingCounter == true)
	{
		TryingCounterStartTimerCallback(B2Player);
		TryingCounterFinishTimerCallback(B2Player);
	}
	if (bInAttackCooling == true)
	{
		AttackIntervalTimerCB(B2Player);
	}
}

void UB2BTTask_PCAutoAttack::TryingCounterStartTimerCallback(class ABladeIIPlayer* InPlayer)
{
	// Could be set to false already by other condition..?
	if (!(bTryingCounter && InPlayer))
		return;
	
	InPlayer->StartGuard();
	InPlayer->GetWorldTimerManager().ClearTimer(GuardStartTimerHandle);
}

void UB2BTTask_PCAutoAttack::TryingCounterFinishTimerCallback(class ABladeIIPlayer* InPlayer)
{
	// Could be set to false already by other condition..?
	bTryingCounter = false;

	if (!InPlayer)
		return;

	InPlayer->StopGuard();
	InPlayer->GetWorldTimerManager().ClearTimer(GuardFinishTimerHandle);
}

void UB2BTTask_PCAutoAttack::GetWorldMobList(TArray<ABladeIICharacter*>& OutList)
{
	auto* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	BII_CHECK(B2GM);
	if (B2GM)
	{
		B2GM->GetAllWorldMobList(OutList);
	}
}

void UB2BTTask_PCAutoAttack::PostAttackSetAttackCooling(class ABladeIIPlayer* InPlayer)
{
	if (!(InPlayer && MinAttackInterval > 0.0f))
		return;

	// MinAttackInterval 悼救 泪矫 attack 疙飞阑 吝窜窍档废
	bInAttackCooling = true;
	InPlayer->GetWorldTimerManager().SetTimer(AttackIntervalTimer, FTimerDelegate::CreateUObject(this, &UB2BTTask_PCAutoAttack::AttackIntervalTimerCB, InPlayer), MinAttackInterval, false);
}

void UB2BTTask_PCAutoAttack::AttackIntervalTimerCB(class ABladeIIPlayer* InPlayer)
{
	bInAttackCooling = false;
	BII_CHECK(InPlayer);
	if (!InPlayer)
		return;

	InPlayer->GetWorldTimerManager().ClearTimer(AttackIntervalTimer);
}

void UB2BTTask_PCAutoAttack::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	// Add any necessary information to Values..
}

FString UB2BTTask_PCAutoAttack::GetStaticDescription() const
{
	FString StaticDescString = Super::GetStaticDescription();
	
	// Add any supporting string if necessary.

	return StaticDescString;
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
bool UB2BTTask_PCAutoAttack::bDevAllowComboAttackOnly = false; // 措眉肺 auto 倒府搁辑 己瓷 抛胶飘 侩.
#endif