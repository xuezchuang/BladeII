// Fill out your copyright notice in the Description page of Project Settings.
#include "TEST_RaidGameMode.h"

#include "Engine.h"
#include "GameFramework/SpectatorPawn.h"
#include "Camera/CameraActor.h"
#include "Particles/Emitter.h"

#include "B2UIManager.h"
#include "Event.h"
#include "FB2ErrorMessage.h"
#include "BladeIITestDummyPlayer.h"
#include "BladeIITestDummyNPC.h"
#include "BladeIIWorldSettings.h"
#include "BladeIIPlayerController.h"

ATEST_RaidGameMode::ATEST_RaidGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DummyRaidTargetMob = nullptr;
	DefaultPCClassEnum = EPCClass::EPC_Assassin; // ����

	DummyTargetMobClass = ENPCClass::ENC_Boss_Sargon;
	DummyTargetMobVariation = ENPCClassVariation::ENCV_Normal;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	bTestSessionBegun = false;

	TotalTestSessionTime = 600.0f; // �������� �뵵�� ������ �̴� �� �ƴ϶� �÷��� ���鼭 �����ϴ� �Ŷ�� �⺻ �ð� ���..
	DummyAnimPlayInterval = 0.2f;
	DummyEquipGrade = 6;
	DummyFloatingUIInterval = 1.0f;
	DummyDamageFxInterval = 1.0f;
	DummySpawnRandRadius = 100.0f; // �� ���� ���͸��� ��ġ�� ������ �� �ְ� �� �Ŷ� ��ġ ���� ������ ũ�� �ʰ� �Ѵ�.
	CamDistScale = 1.0f;
	bWingOn = true;
	bGatherPerfStats = false;
#endif
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // UFUNCTION �̿��� ������ �� ������.

void ATEST_RaidGameMode::BeginDestroy()
{
	Super::BeginDestroy();

}

void ATEST_RaidGameMode::PreBeginPlay()
{
	IGameRule* LocalGameRule = new TestRaidGameModeGameRule;
	SetGameRule(LocalGameRule);

	Super::PreBeginPlay();
	
}

void ATEST_RaidGameMode::SubscribeEvents()
{
	Super::SubscribeEvents();
}

void ATEST_RaidGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsInPreRenderPhase()) // Pre-render �� ���ư��ٸ� ������ �� �ʰ�..
	{
		BeginTestSession();
	}
}

void ATEST_RaidGameMode::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	// ���� PreRender �� ���� �׽�Ʈ ������ �������ٸ� �ڴʰ� ����.
	if (!bTestSessionBegun && !IsInPreRenderPhase())
	{
		BeginTestSession();
	}
}

void ATEST_RaidGameMode::GatherDummyMarkers(ASkeletalMeshActor*& OutDummyTargetMobMarker, TArray<ASkeletalMeshActor*>& OutDummyPlayerMarkers)
{
	// ã�Ƴ� �͵��� ������ Dummy Actor spawn �� ����� ��.
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		for (FActorIterator ActorIt(TheWorld); ActorIt; ++ActorIt)
		{
			ASkeletalMeshActor* ThisSKActor = Cast<ASkeletalMeshActor>(*ActorIt);
			// ���س��� �̸����� ��ġ�� �� ���.
			if (ThisSKActor)
			{
				FString ActorName = ThisSKActor->GetName();
				if (ActorName.Contains(TEXT("DummyTargetMob_Marker")))
				{
					OutDummyTargetMobMarker = ThisSKActor;
				}
				else if (ActorName.Contains(TEXT("DummyPlayer01_Marker"))
					|| ActorName.Contains(TEXT("DummyPlayer02_Marker"))
					|| ActorName.Contains(TEXT("DummyPlayer03_Marker"))
					|| ActorName.Contains(TEXT("DummyPlayer04_Marker"))
					)
				{
					OutDummyPlayerMarkers.Add(ThisSKActor);
				}
			}
		}
	}
}

void ATEST_RaidGameMode::BeginTestSession()
{
	// �� ���� Open ������� ���� �� ���� �׽�Ʈ ���� ���ڸ� ? �� ���е� �ɼ����� �־��ش�.
	if (GEngine)
	{
		FWorldContext* TheWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
		if (TheWorldContext)
		{
			if (TheWorldContext->LastURL.HasOption(TEXT("Time")))
			{
				const TCHAR* TimeStr = TheWorldContext->LastURL.GetOption(TEXT("Time="), TEXT("20.0f"));
				TotalTestSessionTime = FCString::Atof(TimeStr);
			}

			// ���� �÷��̾� ���� �ɼǵ�
			if (TheWorldContext->LastURL.HasOption(TEXT("Equip"))) 
			{
				const TCHAR* EquipStr = TheWorldContext->LastURL.GetOption(TEXT("Equip="), TEXT("6"));
				DummyEquipGrade = FCString::Atoi(EquipStr);
			}
			if (TheWorldContext->LastURL.HasOption(TEXT("AT")))
			{
				const TCHAR* ATStr = TheWorldContext->LastURL.GetOption(TEXT("AT="), TEXT("1.0"));
				DummyAnimPlayInterval = FCString::Atof(ATStr);
			}
			if (TheWorldContext->LastURL.HasOption(TEXT("FUI")))
			{
				const TCHAR* FUIStr = TheWorldContext->LastURL.GetOption(TEXT("FUI="), TEXT("1.0"));
				DummyFloatingUIInterval = FCString::Atof(FUIStr);
			}
			if (TheWorldContext->LastURL.HasOption(TEXT("DFX")))
			{
				const TCHAR* DFXStr = TheWorldContext->LastURL.GetOption(TEXT("DFX="), TEXT("1.0"));
				DummyDamageFxInterval = FCString::Atof(DFXStr);
			}
			if (TheWorldContext->LastURL.HasOption(TEXT("CamDistScale")))
			{
				const TCHAR* CDSStr = TheWorldContext->LastURL.GetOption(TEXT("CamDistScale="), TEXT("1.0"));
				CamDistScale = FCString::Atof(CDSStr);
			}
			else if (TheWorldContext->LastURL.HasOption(TEXT("CDS")))
			{
				const TCHAR* CDSStr = TheWorldContext->LastURL.GetOption(TEXT("CDS="), TEXT("1.0"));
				CamDistScale = FCString::Atof(CDSStr);
			}
			if (TheWorldContext->LastURL.HasOption(TEXT("SpawnRad")))
			{
				const TCHAR* SRStr = TheWorldContext->LastURL.GetOption(TEXT("SpawnRad="), TEXT("800.0"));
				DummySpawnRandRadius = FCString::Atof(SRStr);
			}
			if (TheWorldContext->LastURL.HasOption(TEXT("Wing")))
			{
				const TCHAR* WingStr = TheWorldContext->LastURL.GetOption(TEXT("Wing="), TEXT("1"));
				bWingOn = ((FCString::Atoi(WingStr) != 0) || (!(FString(WingStr) == TEXT("False")) && !(FString(WingStr) == TEXT("Off"))));
			}
			if (TheWorldContext->LastURL.HasOption(TEXT("PerfStat")))
			{
				const TCHAR* PerfStatStr = TheWorldContext->LastURL.GetOption(TEXT("PerfStat="), TEXT("1"));
				bGatherPerfStats = ((FCString::Atoi(PerfStatStr) != 0) || (!(FString(PerfStatStr) == TEXT("False")) && !(FString(PerfStatStr) == TEXT("Off"))));
			}
			else if (TheWorldContext->LastURL.HasOption(TEXT("PS")))
			{
				const TCHAR* PerfStatStr = TheWorldContext->LastURL.GetOption(TEXT("PS="), TEXT("1"));
				bGatherPerfStats = ((FCString::Atoi(PerfStatStr) != 0) || (!(FString(PerfStatStr) == TEXT("False")) && !(FString(PerfStatStr) == TEXT("Off"))));
			}
		}
	}

	UE_LOG(LogBladeII, Log, TEXT("Begin RaidGameMode performance test session with Time : %.2f s"), TotalTestSessionTime);
	BeginTestSessionWithOptions(TotalTestSessionTime);
}

void ATEST_RaidGameMode::BeginTestSessionWithOptions(float InTestSessionTime)
{
//	// �������� ��� ����̹Ƿ� �÷��̾� ĳ���Ͱ� �ִ�. �׽�Ʈ�� ���ص��� �ʴ� �¾����� �ϵ���.
//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	if (LocalB2Player)
//	{
//		//LocalB2Player->bCanBeDamaged = true;
//		// �ܼ� Ŀ�ǵ� �Է��Ϸ��µ� �Ǽ��� �±׹�ư ���� ���� ���Ƽ� ��
//		LocalB2Player->SetTagEnable(false);
//
//		LocalB2Player->SetActorHiddenInGame(true); // �� ������ �� ���� �׽�Ʈ�� ���ظ� �� ��
//	}
//	const FVector DefaultDummyCenterLocation = LocalB2Player ? LocalB2Player->GetActorLocation() : FVector(0.0f, 0.0f, 0.0f);
//		
//	// �ʿ� ��ġ�� ���� SkeletalMeshActor ��ġ�� ������ ���� ��ġ�� ����. �̰� ������ ���� �÷��̾� ĳ���� ��ġ�� ��������.
//	ASkeletalMeshActor* TargetMobMarker = nullptr;
//	TArray<ASkeletalMeshActor*> DummyPlayerMarkers;
//	GatherDummyMarkers(TargetMobMarker, DummyPlayerMarkers);
//
//	// ������ �׽�Ʈ������ �÷��̾� ���� �����ų �ʿ䰡 �� �־����� �̰� ©���� 4����.
//	const int32 TotalDummyPlayerNum = 4;
//	for (int32 DPI = 0; DPI < TotalDummyPlayerNum; ++DPI)
//	{		
//		ASkeletalMeshActor* ThisDummyMarker = DummyPlayerMarkers.IsValidIndex(DPI) ? DummyPlayerMarkers[DPI] : nullptr;
//		ABladeIITestDummyPlayer* ThisSpawned = SpawnSingleTestPlayerDummy(DPI, 
//			ThisDummyMarker ? ThisDummyMarker->GetActorLocation() + FVector(0.0f, 0.0f, 200.0f) : DefaultDummyCenterLocation);
//		if (ThisSpawned)
//		{
//			AllDummyPlayers.Add(ThisSpawned);
//		}
//	}
//
//	DummyRaidTargetMob = SpawnSingleTestMobDummy(
//		// ��ġ�� �ø��� �� �ϴ� ���� SkeletalMeshActor ���� offset �����ε� ��Ȯ�ϰ� ������ ���ϰ�.. ������ spawn �� ���ٴڿ� snap ��.
//		TargetMobMarker ? TargetMobMarker->GetActorLocation() + FVector(0.0f,0.0f,500.0f) : DefaultDummyCenterLocation,
//		TargetMobMarker ? TargetMobMarker->GetActorRotation() : FRotator(0.f, 0.f, 0.f)
//	);
//
//	UWorld* TheWorld = GetWorld();
//	if (TheWorld)
//	{
//		ABladeIIWorldSettings* B2WS = Cast<ABladeIIWorldSettings>(TheWorld->GetWorldSettings());
//		if (B2WS)
//		{
//			// WorldSetting �� CameraSetting �� ���� ����. ActiveCameraActor �� �ִٸ� ��������.
//			// ���� ���� �ε��� ä�� �̰� ������ �Ҹ��� �������� ��� �����ǰ���..
//			B2WS->DefaultPlayerCameraSettings.bOverrideCameraBoomArmLength = true; // DefaultPlayerCameraSettings �����Ϸ��� friend class ������. ���� ����� �ƴϴ� ������..
//			B2WS->DefaultPlayerCameraSettings.CameraBoomArmLength *= CamDistScale;
//			ApplyNormallyExpectedInGameCamera(this,
//				Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(TheWorld)),
//				0.0f);
//		}
//
//		TheWorld->GetTimerManager().SetTimer(TestSessionTH, this, &ATEST_RaidGameMode::EndTestSession, InTestSessionTime, false);
//	}	
//
//#if BII_STATS // ���� ���� �� ǥ��.
//	FStatisticalPerfStatCollector::bDoStatCollecting = bGatherPerfStats;
//	if (bGatherPerfStats) {
//		PerfStatCollector.OnBegin(this);
//	}
//#endif
//
//	bTestSessionBegun = true;
}

ABladeIITestDummyPlayer* ATEST_RaidGameMode::SpawnSingleTestPlayerDummy(int32 InMyIndex, const FVector& InDummyCenterLocation)
{
	// �ణ�� ������ ���ϴ� ���� ������ �ƴϸ� ������ �Ȱ��� ���� ������.. �׵����� �������̳� ������ �׽�Ʈ ���迡 ���ϸ� �ణ�� ������ �׽�Ʈ ����� ������ ��ġ���� ���� �� ����.


	FVector2D SpawnDir(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f));
	// Ȥ�ö� ��ģ���� ���� ���ڰ� ������ �ζǿ� ��÷�Ǹ� ����ϴ�
	if (FMath::Abs(SpawnDir.X) < KINDA_SMALL_NUMBER && FMath::Abs(SpawnDir.Y) < KINDA_SMALL_NUMBER)
	{
		SpawnDir.X = KINDA_SMALL_NUMBER;
		SpawnDir.Y = KINDA_SMALL_NUMBER;
	}
	SpawnDir.Normalize();
	float SpawnDist = FMath::FRandRange(50.0f, FMath::Max(DummySpawnRandRadius,50.0f));

	FTransform SpawnTransform(
		FRotator(0.0f, FMath::FRandRange(-180.0f,180.0f), 0.0f),
		InDummyCenterLocation + FVector(SpawnDir.X * SpawnDist, SpawnDir.Y * SpawnDist, 0.0f),
		FVector(1.0f, 1.0f, 1.0f)
	);

	EPCClass SpawnClass = IntToPCClass(FMath::RandRange(0, 3));

	ABladeIITestDummyPlayer* SpawnedDummy = ABladeIITestDummyPlayer::SpawnWithStandardEquipment(
		GetWorld(), SpawnClass, SpawnTransform, DummyEquipGrade, DummyFloatingUIInterval, DummyDamageFxInterval,
		bWingOn, MAX_WING_EVOLUTION_GRADE, true // ���� ��ޱ����� ���س���..
	);

	if (SpawnedDummy) // �ִϸ��̼��� ������ �ð� �������� ����.
	{
		SpawnedDummy->SetUseRandomAttackState(true);
		SpawnedDummy->SetCooltimeBetweenAnim(DummyAnimPlayInterval);
	}

	return SpawnedDummy;
}

ABladeIITestDummyNPC* ATEST_RaidGameMode::SpawnSingleTestMobDummy(const FVector& InLocation, const FRotator& InRotation)
{
	// ���⿣ ���� �� ����. ������ ��ǥ�� �״��.
	FTransform SpawnTransform(InRotation, InLocation, FVector(1.0f, 1.0f, 1.0f));
	
	ABladeIITestDummyNPC* SpawnedDummy = ABladeIITestDummyNPC::SpawnUtil(GetWorld(), 
		DummyTargetMobClass, DummyTargetMobVariation, SpawnTransform,
		DummyFloatingUIInterval, DummyDamageFxInterval);

	if (SpawnedDummy)
	{
		SpawnedDummy->SetUseRandomAttackState(true);
		SpawnedDummy->SetCooltimeBetweenAnim(DummyAnimPlayInterval);

		// ȸ���� �����ϰ� �� �� �縣���� ���ο� ��.. 
		SpawnedDummy->SetMaintainLocation(true);
		SpawnedDummy->SetMaintainRotation(false);
	}

	return SpawnedDummy;
}

void ATEST_RaidGameMode::EndTestSession()
{
#if BII_STATS
	if (bGatherPerfStats)
	{
		PerfStatCollector.OnEnd(this); // �̹� ������ ����ǥ�� ����� ���ô�.
	}
#endif

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TestSessionTH);
	}

	// ��� �׳� �ʿ� �ھƳ��� �����ͷδ�
	if (DummyRaidTargetMob && DummyRaidTargetMob->IsValidObj())
	{
		DummyRaidTargetMob->Destroy();
		DummyRaidTargetMob = nullptr;
	}

	for (ABladeIITestDummyPlayer* ThisDummy : AllDummyPlayers)
	{
		if (ThisDummy && ThisDummy->IsValidObj())
		{
			ThisDummy->Destroy();
		}
	}
	AllDummyPlayers.Empty();
}

#endif