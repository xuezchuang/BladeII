//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "BladeIIAIController.h"
//#include "BladeIIGameMode.h"
////#include "B2NPCClassInfoBox.h"
//#include "Event.h"
//#include "B2MonsterSpawnPool.h"
////#include "B2SomeInfo.h"
//#include "BrainComponent.h"
//#include "B2GameInstance.h"
//
//ABladeIIAIController::ABladeIIAIController(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	CurrentPhaseNum = 0;
//}
//
////void ABladeIIAIController::Possess(APawn* InPawn)
////{
////	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIAIController::Possess"));
////
////	Super::Possess(InPawn);
////	
////	ABladeIICharacter* B2Char = Cast<ABladeIICharacter>(InPawn);
////	
////	UBehaviorTree* BTtoRun = NULL;
////	// NPCClassInfo �� ���� spawn �� ���̶�� SpawnNPCClassInfoCharacter ���� � �����ε� BehaviorTree �� ������ �Ǿ�� ���� ��.
////	if (B2Char && B2Char->BehaviorTree)
////	{
////		BTtoRun = B2Char->BehaviorTree;
////	}
////	else
////	{
////		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
////		UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : NULL;
////		BTtoRun = SomeInfo ? SomeInfo->GetDefaultNPCClassInfoMeleeBT() : NULL;
////	}
////
////	RunBehaviorTree(BTtoRun);
////
////	//
////	// ���� BP ���� �ϴ� BT ���� �κ��� ����.
////	//
////}
//
//void ABladeIIAIController::ToNextPhase()
//{
//	ABladeIICharacter* B2Char = Cast<ABladeIICharacter>(GetPawn());
//	//BII_CHECK(B2Char && B2Char->IsValidObj());
//
//	if (B2Char && B2Char->PhaseDataArray.Num() > CurrentPhaseNum)
//	{
//		// ���ϴ� Phase ��ȣ -1 �� PhaseDataArray �ε���. 0 ������ PhaseDataArray �� ������� �����Ƿ�.
//		const FAIPhaseData& ThisPhaseData = B2Char->PhaseDataArray[CurrentPhaseNum];
//		++CurrentPhaseNum;
//
//		PhaseChange(CurrentPhaseNum, ThisPhaseData);
//	}
//}
//
//void ABladeIIAIController::PhaseChange(int32 ToPhaseNum, const FAIPhaseData& ToPhaseData)
//{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIAIController::PhaseChange"));
//
//	//// Phase 0 ���� ���� �� ����.
//	//BII_CHECK(ToPhaseNum >= 1);
//	//ABladeIICharacter* CurrChar = Cast<ABladeIICharacter>(GetPawn());
//	//BII_CHECK(CurrChar && CurrChar->IsValidObj());
//
//	//ABladeIICharacter* PrevDestroyRef = NULL; // Re-spawn �� �����ϴ� ��� �ı��� CurrChar �� ��� �ְ� �� ��. 
//
//	//CurrentPhaseNum = ToPhaseNum;
//
//	//// ĳ���� ��ü�� �ٲ�ġ���ϴ� �� ���� ó���ؾ� ��Ÿ �ٸ� phase ȿ������ ���� ����� �� �ִ�.
//	//if (ToPhaseData.BladeIICharacterClass != ENPCClass::ENC_End)
//	//{
//	//	// Spawn new character...
//	//	CurrChar->SetActorEnableCollision(false);
//
//	//	FTransform SpawnTransform = CurrChar->GetTransform();
//
//	//	FActorSpawnParameters SpawnInfo;
//	//	SpawnInfo.Owner = NULL;
//	//	SpawnInfo.bDeferConstruction = true; // Need deferred construction and manually call FinishSpawning)
//	//	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	//	SpawnInfo.ObjectFlags |= RF_Transient;	// We do not want to save them into the map.
//
//	//	UB2NPCClassInfoBox* ClassInfoBox = StaticFindMobClassInfoBox();
//	//	ABladeIICharacter* NewCharacter = NULL;
//	//	if (ClassInfoBox)
//	//	{
//	//		// PhaseDataArray ���� �� SpawnNPCClassInfoCharacter ���� ä���� ��. ��Ÿ �ʿ��� ��Ÿ�� ���´� ���⼭ ä���.
//	//		NewCharacter = ClassInfoBox->SpawnNPCClassInfoCharacter(GetWorld(), ToPhaseData.BladeIICharacterClass, ToPhaseData.CharacterClassVariation, SpawnTransform, SpawnInfo);
//	//	}
//
//	//	BII_CHECK(NewCharacter && NewCharacter->IsValidObj());
//
//	//	if (NewCharacter != NULL)
//	//	{
//	//		// ������ SpawnPool �� ��ϵǾ� �ִ� ���̶�� �ٲ�ġ�� �� �ش�. SetBirthPlace ó���� �� ��.
//	//		if (CurrChar->GetBirthPlace()) // StageManager �� ��ġ�� �ʴ� ���� ó���ε�, �ϴ� �׳� �ΰ� Ȥ�� �ʿ��ϰ� �Ǹ� ��ü.
//	//		{
//	//			CurrChar->GetBirthPlace()->SwapMonsterForPhaseSpawn(CurrChar, NewCharacter);
//	//		}
//
//	//		NewCharacter->AutoPossessAI = EAutoPossessAI::Disabled; // ���������� AIController �� �����ϰ� possess �ϵ��� �������δ� ��� ���� �� AIController �� ���� �ٽ� possess.
//	//		NewCharacter->FinishSpawning(SpawnTransform);
//
//	//		////////////////////////////////////////////////////////////////////////////////
//	//		// NPCClassInfo �� InitializeCombatStats ���� ���� ���Ǵ� �����͵� �ܿ� ���� ���µ��� ������ �־�� ��.
//	//		NewCharacter->Health = CurrChar->Health;
//	//		NewCharacter->MaxHealth = CurrChar->MaxHealth;
//	//		NewCharacter->Armor = CurrChar->Armor;
//	//		NewCharacter->MaxArmor = CurrChar->MaxArmor;
//	//		NewCharacter->PhaseDataArray = CurrChar->PhaseDataArray; // �̰� NPCClassInfo ���� ���� �ϴµ�.. ���� PhaseData �� �����ؾ� �ϴ���, �ƴ� ���� ������ �ɷ� �ϴ� �� �´���.. CurrentPhaseNum �� ��� �̾�����Ƿ� ó�� �� �����Ѵ� ġ��.
//
//	//		NewCharacter->bHadChangedPhase = true; // �ӽ� �÷��ٵ� ���⼭ ������ ����.
//	//		////////////////////////////////////////////////////////////////////////////////
//	//		
//	//		// ���� �� ����� �ı�.
//	//		UnPossess();
//	//		PrevDestroyRef = CurrChar; // ��� ������ �� �� �ǵ� �� �񱳸� �ؾ� ��. ��������� ���� ����
//	//		CurrChar->Destroy();
//	//		CurrChar = NULL;
//
//	//		// ���ο� ĳ���͸� possess.
//
//	//		Possess(NewCharacter);
//	//		CurrChar = Cast<ABladeIICharacter>(GetPawn());
//	//		check(CurrChar == NewCharacter);
//	//	}
//	//	else
//	//	{
//	//		CurrChar->SetActorEnableCollision(true);
//	//	}
//	//}
//
//	//// Continue if new spawn with BP Class failed
//	//if (ToPhaseData.MaxWalkSpeed > 0.f)
//	//{
//	//	CurrChar->GetCharacterMovement()->MaxWalkSpeed = ToPhaseData.MaxWalkSpeed;
//	//}
//
//	//if (ToPhaseData.BehaviorTree != NULL)
//	//{
//	//	CurrChar->SetBehaviorTree(ToPhaseData.BehaviorTree);
//	//}
//
//	//if (ToPhaseData.PSTemplate != NULL)
//	//{
//	//	if (CurrChar->PhasePSComp != NULL)
//	//	{
//	//		// Destroy old one
//	//		CurrChar->PhasePSComp->DestroyComponent();
//	//		CurrChar->PhasePSComp = NULL;
//	//	}
//
//	//	// Spawn New PS
//	//	CurrChar->PhasePSComp =
//	//		UGameplayStatics::SpawnEmitterAttached(ToPhaseData.PSTemplate, CurrChar->GetMesh(), ToPhaseData.SocketName, ToPhaseData.LocationOffset, ToPhaseData.RotationOffset,
//	//		EAttachLocation::KeepRelativeOffset, true, ToPhaseData.bIgnoreCustomTimeDilation);
//	//}
//
//	//if (ToPhaseData.BuffModeOverride != EUniqueBuffType::EBT_End)
//	//{
//	//	CurrChar->SetBuffMode(ToPhaseData.BuffModeOverride, true, 0.f, -1.f, true, true);
//	//}
//
//	//if (ToPhaseData.DialogCodeNameOnPhaseChanged != NAME_None)
//	//{
//	//	OpenDialogClass<FName, bool>::GetInstance().Signal(ToPhaseData.DialogCodeNameOnPhaseChanged, false); // �ʿ��ϸ� MaintainBattleUI ������ �߰��ؼ� �Ѱ���.
//	//}
//
//	//// SpawnPool wave mob �� ��� phase ��ȭ�� ���� �̺�Ʈ �ñ׳�
//	//if (CurrChar->GetBirthPlace() && !CurrChar->IsSummonedMob())
//	//{
//	//	SpawnPoolWaveMobPhaseChangeClass<AB2MonsterSpawnPool*, ABladeIICharacter*, ABladeIICharacter*, int32, int32, int32>::GetInstance().Signal(
//	//		CurrChar->GetBirthPlace(), CurrChar, PrevDestroyRef, CurrChar->GetSpawnPoolWaveNum(), CurrChar->GetSpawnPoolWaveObjIndex(), CurrentPhaseNum
//	//		);
//	//}
//}
//
//void ABladeIIAIController::StopAutoPlay()
//{
//	if (BrainComponent)
//		BrainComponent->StopLogic(TEXT("ForceStop"));
//
//	auto* ControlledPawn = Cast<ABladeIICharacter>(GetPawn());
//	if (ControlledPawn)
//	{
//		ControlledPawn->SetAttackState(EAttackState::ECS_None);
//	}
//}
//
//void ABladeIIAIController::ApplyMountBehaviorTree()
//{
//	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
//	UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : NULL;
//	UBehaviorTree* BTtoRun = SomeInfo ? SomeInfo->GetMountAutoBT() : NULL;
//
//	if(BTtoRun)
//		RunBehaviorTree(BTtoRun);
//}
