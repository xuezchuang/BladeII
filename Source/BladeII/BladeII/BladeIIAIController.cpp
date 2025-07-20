// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeIIAIController.h"
#include "BladeIIGameMode.h"
//#include "B2NPCClassInfoBox.h"
#include "Event.h"
#include "B2MonsterSpawnPool.h"
#include "B2SomeInfo.h"
#include "BrainComponent.h"
#include "B2GameInstance.h"
#include "BladeIIUtil.h"

ABladeIIAIController::ABladeIIAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentPhaseNum = 0;
}

//void ABladeIIAIController::Possess(APawn* InPawn)
//{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIAIController::Possess"));
//
//	Super::Possess(InPawn);
//	
//	ABladeIICharacter* B2Char = Cast<ABladeIICharacter>(InPawn);
//	
//	UBehaviorTree* BTtoRun = NULL;
//	// NPCClassInfo 甫 烹秦 spawn 等 巴捞扼搁 SpawnNPCClassInfoCharacter 俊辑 绢恫 侥栏肺电 BehaviorTree 啊 技泼捞 登绢绰 乐阑 巴.
//	if (B2Char && B2Char->BehaviorTree)
//	{
//		BTtoRun = B2Char->BehaviorTree;
//	}
//	else
//	{
//		UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
//		UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : NULL;
//		BTtoRun = SomeInfo ? SomeInfo->GetDefaultNPCClassInfoMeleeBT() : NULL;
//	}
//
//	RunBehaviorTree(BTtoRun);
//
//	//
//	// 扁粮 BP 俊辑 窍带 BT 技泼 何盒篮 力芭.
//	//
//}

void ABladeIIAIController::ToNextPhase()
{
	ABladeIICharacter* B2Char = Cast<ABladeIICharacter>(GetPawn());
	//BII_CHECK(B2Char && B2Char->IsValidObj());

	if (B2Char && B2Char->PhaseDataArray.Num() > CurrentPhaseNum)
	{
		// 盔窍绰 Phase 锅龋 -1 捞 PhaseDataArray 牢郸胶. 0 俊辑绰 PhaseDataArray 甫 荤侩窍瘤 臼栏骨肺.
		const FAIPhaseData& ThisPhaseData = B2Char->PhaseDataArray[CurrentPhaseNum];
		++CurrentPhaseNum;

		PhaseChange(CurrentPhaseNum, ThisPhaseData);
	}
}

void ABladeIIAIController::PhaseChange(int32 ToPhaseNum, const FAIPhaseData& ToPhaseData)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIAIController::PhaseChange"));

	//// Phase 0 栏肺 啊绰 扒 绝澜.
	//BII_CHECK(ToPhaseNum >= 1);
	//ABladeIICharacter* CurrChar = Cast<ABladeIICharacter>(GetPawn());
	//BII_CHECK(CurrChar && CurrChar->IsValidObj());

	//ABladeIICharacter* PrevDestroyRef = NULL; // Re-spawn 阑 荐馆窍绰 版快 颇鲍等 CurrChar 甫 甸绊 乐霸 瞪 巴. 

	//CurrentPhaseNum = ToPhaseNum;

	//// 某腐磐 磊眉甫 官层摹扁窍绰 吧 刚历 贸府秦具 咯鸥 促弗 phase 瓤苞甸捞 鞍捞 利侩瞪 荐 乐促.
	//if (ToPhaseData.BladeIICharacterClass != ENPCClass::ENC_End)
	//{
	//	// Spawn new character...
	//	CurrChar->SetActorEnableCollision(false);

	//	FTransform SpawnTransform = CurrChar->GetTransform();

	//	FActorSpawnParameters SpawnInfo;
	//	SpawnInfo.Owner = NULL;
	//	SpawnInfo.bDeferConstruction = true; // Need deferred construction and manually call FinishSpawning)
	//	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//	SpawnInfo.ObjectFlags |= RF_Transient;	// We do not want to save them into the map.

	//	UB2NPCClassInfoBox* ClassInfoBox = StaticFindMobClassInfoBox();
	//	ABladeIICharacter* NewCharacter = NULL;
	//	if (ClassInfoBox)
	//	{
	//		// PhaseDataArray 鞍篮 扒 SpawnNPCClassInfoCharacter 俊辑 盲况临 巴. 扁鸥 鞘夸茄 繁鸥烙 惑怕绰 咯扁辑 盲款促.
	//		NewCharacter = ClassInfoBox->SpawnNPCClassInfoCharacter(GetWorld(), ToPhaseData.BladeIICharacterClass, ToPhaseData.CharacterClassVariation, SpawnTransform, SpawnInfo);
	//	}

	//	BII_CHECK(NewCharacter && NewCharacter->IsValidObj());

	//	if (NewCharacter != NULL)
	//	{
	//		// 扁粮俊 SpawnPool 俊 殿废登绢 乐带 各捞扼搁 官层摹扁 秦 霖促. SetBirthPlace 贸府档 瞪 巴.
	//		if (CurrChar->GetBirthPlace()) // StageManager 甫 芭摹瘤 臼绰 流立 贸府牢单, 老窜 弊成 滴绊 趣矫 鞘夸窍霸 登搁 背眉.
	//		{
	//			CurrChar->GetBirthPlace()->SwapMonsterForPhaseSpawn(CurrChar, NewCharacter);
	//		}

	//		NewCharacter->AutoPossessAI = EAutoPossessAI::Disabled; // 郴何利栏肺 AIController 甫 积己窍绊 possess 窍档废 郴滚妨滴绰 措脚 瘤陛 捞 AIController 肺 流立 促矫 possess.
	//		NewCharacter->FinishSpawning(SpawnTransform);

	//		////////////////////////////////////////////////////////////////////////////////
	//		// NPCClassInfo 唱 InitializeCombatStats 殿阑 烹秦 拌魂登绰 单捞磐甸 寇俊 泅犁 惑怕殿篮 汗荤秦 林绢具 窃.
	//		NewCharacter->Health = CurrChar->Health;
	//		NewCharacter->MaxHealth = CurrChar->MaxHealth;
	//		NewCharacter->Armor = CurrChar->Armor;
	//		NewCharacter->MaxArmor = CurrChar->MaxArmor;
	//		NewCharacter->PhaseDataArray = CurrChar->PhaseDataArray; // 捞扒 NPCClassInfo 俊辑 坷变 窍绰单.. 弥檬 PhaseData 甫 蜡瘤秦具 窍绰瘤, 酒丛 蝶肺 技泼茄 吧肺 窍绰 霸 嘎绰瘤.. CurrentPhaseNum 篮 拌加 捞绢罐栏骨肺 贸澜 吧 蜡瘤茄促 摹磊.

	//		NewCharacter->bHadChangedPhase = true; // 烙矫 敲贰辟单 咯扁辑 技泼秦 林磊.
	//		////////////////////////////////////////////////////////////////////////////////
	//		
	//		// 扁粮 扒 都绢郴绊 颇鲍.
	//		UnPossess();
	//		PrevDestroyRef = CurrChar; // 绢叼俊 傈崔阑 秦 临 扒单 傲 厚背父 秦具 窃. 荤侩窍瘤绰 富绊 せせ
	//		CurrChar->Destroy();
	//		CurrChar = NULL;

	//		// 货肺款 某腐磐甫 possess.

	//		Possess(NewCharacter);
	//		CurrChar = Cast<ABladeIICharacter>(GetPawn());
	//		check(CurrChar == NewCharacter);
	//	}
	//	else
	//	{
	//		CurrChar->SetActorEnableCollision(true);
	//	}
	//}

	//// Continue if new spawn with BP Class failed
	//if (ToPhaseData.MaxWalkSpeed > 0.f)
	//{
	//	CurrChar->GetCharacterMovement()->MaxWalkSpeed = ToPhaseData.MaxWalkSpeed;
	//}

	//if (ToPhaseData.BehaviorTree != NULL)
	//{
	//	CurrChar->SetBehaviorTree(ToPhaseData.BehaviorTree);
	//}

	//if (ToPhaseData.PSTemplate != NULL)
	//{
	//	if (CurrChar->PhasePSComp != NULL)
	//	{
	//		// Destroy old one
	//		CurrChar->PhasePSComp->DestroyComponent();
	//		CurrChar->PhasePSComp = NULL;
	//	}

	//	// Spawn New PS
	//	CurrChar->PhasePSComp =
	//		UGameplayStatics::SpawnEmitterAttached(ToPhaseData.PSTemplate, CurrChar->GetMesh(), ToPhaseData.SocketName, ToPhaseData.LocationOffset, ToPhaseData.RotationOffset,
	//		EAttachLocation::KeepRelativeOffset, true, ToPhaseData.bIgnoreCustomTimeDilation);
	//}

	//if (ToPhaseData.BuffModeOverride != EUniqueBuffType::EBT_End)
	//{
	//	CurrChar->SetBuffMode(ToPhaseData.BuffModeOverride, true, 0.f, -1.f, true, true);
	//}

	//if (ToPhaseData.DialogCodeNameOnPhaseChanged != NAME_None)
	//{
	//	OpenDialogClass<FName, bool>::GetInstance().Signal(ToPhaseData.DialogCodeNameOnPhaseChanged, false); // 鞘夸窍搁 MaintainBattleUI 汲沥阑 眠啊秦辑 逞败淋.
	//}

	//// SpawnPool wave mob 狼 版快 phase 函拳俊 措茄 捞亥飘 矫弊澄
	//if (CurrChar->GetBirthPlace() && !CurrChar->IsSummonedMob())
	//{
	//	SpawnPoolWaveMobPhaseChangeClass<AB2MonsterSpawnPool*, ABladeIICharacter*, ABladeIICharacter*, int32, int32, int32>::GetInstance().Signal(
	//		CurrChar->GetBirthPlace(), CurrChar, PrevDestroyRef, CurrChar->GetSpawnPoolWaveNum(), CurrChar->GetSpawnPoolWaveObjIndex(), CurrentPhaseNum
	//		);
	//}
}

void ABladeIIAIController::StopAutoPlay()
{
	if (BrainComponent)
		BrainComponent->StopLogic(TEXT("ForceStop"));

	auto* ControlledPawn = Cast<ABladeIICharacter>(GetPawn());
	if (ControlledPawn)
	{
		ControlledPawn->SetAttackState(EAttackState::ECS_None);
	}
}

void ABladeIIAIController::ApplyMountBehaviorTree()
{
	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : NULL;
	UBehaviorTree* BTtoRun = SomeInfo ? SomeInfo->GetMountAutoBT() : NULL;

	if(BTtoRun)
		RunBehaviorTree(BTtoRun);
}
