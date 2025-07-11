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
////	// NPCClassInfo 를 통해 spawn 된 것이라면 SpawnNPCClassInfoCharacter 에서 어떤 식으로든 BehaviorTree 가 세팅이 되어는 있을 것.
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
////	// 기존 BP 에서 하던 BT 세팅 부분은 제거.
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
//		// 원하는 Phase 번호 -1 이 PhaseDataArray 인덱스. 0 에서는 PhaseDataArray 를 사용하지 않으므로.
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
//	//// Phase 0 으로 가는 건 없음.
//	//BII_CHECK(ToPhaseNum >= 1);
//	//ABladeIICharacter* CurrChar = Cast<ABladeIICharacter>(GetPawn());
//	//BII_CHECK(CurrChar && CurrChar->IsValidObj());
//
//	//ABladeIICharacter* PrevDestroyRef = NULL; // Re-spawn 을 수반하는 경우 파괴된 CurrChar 를 들고 있게 될 것. 
//
//	//CurrentPhaseNum = ToPhaseNum;
//
//	//// 캐릭터 자체를 바꿔치기하는 걸 먼저 처리해야 여타 다른 phase 효과들이 같이 적용될 수 있다.
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
//	//		// PhaseDataArray 같은 건 SpawnNPCClassInfoCharacter 에서 채워줄 것. 기타 필요한 런타임 상태는 여기서 채운다.
//	//		NewCharacter = ClassInfoBox->SpawnNPCClassInfoCharacter(GetWorld(), ToPhaseData.BladeIICharacterClass, ToPhaseData.CharacterClassVariation, SpawnTransform, SpawnInfo);
//	//	}
//
//	//	BII_CHECK(NewCharacter && NewCharacter->IsValidObj());
//
//	//	if (NewCharacter != NULL)
//	//	{
//	//		// 기존에 SpawnPool 에 등록되어 있던 몹이라면 바꿔치기 해 준다. SetBirthPlace 처리도 될 것.
//	//		if (CurrChar->GetBirthPlace()) // StageManager 를 거치지 않는 직접 처리인데, 일단 그냥 두고 혹시 필요하게 되면 교체.
//	//		{
//	//			CurrChar->GetBirthPlace()->SwapMonsterForPhaseSpawn(CurrChar, NewCharacter);
//	//		}
//
//	//		NewCharacter->AutoPossessAI = EAutoPossessAI::Disabled; // 내부적으로 AIController 를 생성하고 possess 하도록 내버려두는 대신 지금 이 AIController 로 직접 다시 possess.
//	//		NewCharacter->FinishSpawning(SpawnTransform);
//
//	//		////////////////////////////////////////////////////////////////////////////////
//	//		// NPCClassInfo 나 InitializeCombatStats 등을 통해 계산되는 데이터들 외에 현재 상태등은 복사해 주어야 함.
//	//		NewCharacter->Health = CurrChar->Health;
//	//		NewCharacter->MaxHealth = CurrChar->MaxHealth;
//	//		NewCharacter->Armor = CurrChar->Armor;
//	//		NewCharacter->MaxArmor = CurrChar->MaxArmor;
//	//		NewCharacter->PhaseDataArray = CurrChar->PhaseDataArray; // 이건 NPCClassInfo 에서 오긴 하는데.. 최초 PhaseData 를 유지해야 하는지, 아님 따로 세팅한 걸로 하는 게 맞는지.. CurrentPhaseNum 은 계속 이어받으므로 처음 걸 유지한다 치자.
//
//	//		NewCharacter->bHadChangedPhase = true; // 임시 플래근데 여기서 세팅해 주자.
//	//		////////////////////////////////////////////////////////////////////////////////
//	//		
//	//		// 기존 건 떼어내고 파괴.
//	//		UnPossess();
//	//		PrevDestroyRef = CurrChar; // 어디에 전달을 해 줄 건데 걍 비교만 해야 함. 사용하지는 말고 ㅋㅋ
//	//		CurrChar->Destroy();
//	//		CurrChar = NULL;
//
//	//		// 새로운 캐릭터를 possess.
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
//	//	OpenDialogClass<FName, bool>::GetInstance().Signal(ToPhaseData.DialogCodeNameOnPhaseChanged, false); // 필요하면 MaintainBattleUI 설정을 추가해서 넘겨줌.
//	//}
//
//	//// SpawnPool wave mob 의 경우 phase 변화에 대한 이벤트 시그널
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
