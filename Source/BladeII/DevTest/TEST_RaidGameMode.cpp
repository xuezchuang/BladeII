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
	DefaultPCClassEnum = EPCClass::EPC_Assassin; // 내맘

	DummyTargetMobClass = ENPCClass::ENC_Boss_Sargon;
	DummyTargetMobVariation = ENPCClassVariation::ENCV_Normal;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	bTestSessionBegun = false;

	TotalTestSessionTime = 600.0f; // 일차적인 용도가 데이터 뽑는 게 아니라 플레이 보면서 조절하는 거라니 기본 시간 길게..
	DummyAnimPlayInterval = 0.2f;
	DummyEquipGrade = 6;
	DummyFloatingUIInterval = 1.0f;
	DummyDamageFxInterval = 1.0f;
	DummySpawnRandRadius = 100.0f; // 각 더미 액터마다 위치를 지정할 수 있게 할 거라 위치 랜덤 범위는 크지 않게 한다.
	CamDistScale = 1.0f;
	bWingOn = true;
	bGatherPerfStats = false;
#endif
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // UFUNCTION 이외의 구현은 이 안으로.

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
	
	if (!IsInPreRenderPhase()) // Pre-render 가 돌아간다면 시작을 좀 늦게..
	{
		BeginTestSession();
	}
}

void ATEST_RaidGameMode::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	// 만일 PreRender 로 인해 테스트 세션이 늦춰졌다면 뒤늦게 시작.
	if (!bTestSessionBegun && !IsInPreRenderPhase())
	{
		BeginTestSession();
	}
}

void ATEST_RaidGameMode::GatherDummyMarkers(ASkeletalMeshActor*& OutDummyTargetMobMarker, TArray<ASkeletalMeshActor*>& OutDummyPlayerMarkers)
{
	// 찾아낸 것들의 정보를 Dummy Actor spawn 에 사용할 거.
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		for (FActorIterator ActorIt(TheWorld); ActorIt; ++ActorIt)
		{
			ASkeletalMeshActor* ThisSKActor = Cast<ASkeletalMeshActor>(*ActorIt);
			// 정해놓은 이름으로 배치한 걸 사용.
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
	// 이 맵은 Open 명령으로 열게 될 껀데 테스트 관련 인자를 ? 로 구분된 옵션으로 넣어준다.
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

			// 더미 플레이어 세부 옵션들
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
//	// 스테이지 모드 기반이므로 플레이어 캐릭터가 있다. 테스트에 방해되지 않는 셋업들을 하도록.
//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	if (LocalB2Player)
//	{
//		//LocalB2Player->bCanBeDamaged = true;
//		// 콘솔 커맨드 입력하려는데 실수로 태그버튼 누를 때가 많아서 ㅋ
//		LocalB2Player->SetTagEnable(false);
//
//		LocalB2Player->SetActorHiddenInGame(true); // 걍 숨기자 ㅋ 괜히 테스트에 방해만 될 듯
//	}
//	const FVector DefaultDummyCenterLocation = LocalB2Player ? LocalB2Player->GetActorLocation() : FVector(0.0f, 0.0f, 0.0f);
//		
//	// 맵에 배치해 놓은 SkeletalMeshActor 위치를 가지고 기준 위치로 삼음. 이게 없으면 로컬 플레이어 캐릭터 위치를 기준으로.
//	ASkeletalMeshActor* TargetMobMarker = nullptr;
//	TArray<ASkeletalMeshActor*> DummyPlayerMarkers;
//	GatherDummyMarkers(TargetMobMarker, DummyPlayerMarkers);
//
//	// 점령전 테스트에서는 플레이어 수를 변경시킬 필요가 좀 있었지만 이건 짤없이 4마리.
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
//		// 위치를 올리는 건 하단 기준 SkeletalMeshActor 와의 offset 보정인데 정확하게 하지는 못하고.. 어차피 spawn 시 땅바닥에 snap 됨.
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
//			// WorldSetting 의 CameraSetting 을 쓰는 가정. ActiveCameraActor 가 있다면 무용지물.
//			// 만일 맵이 로딩된 채로 이게 여러번 불리면 스케일이 계속 누적되겠지..
//			B2WS->DefaultPlayerCameraSettings.bOverrideCameraBoomArmLength = true; // DefaultPlayerCameraSettings 접근하려고 friend class 선언함. 정식 기능은 아니니 적당히..
//			B2WS->DefaultPlayerCameraSettings.CameraBoomArmLength *= CamDistScale;
//			ApplyNormallyExpectedInGameCamera(this,
//				Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(TheWorld)),
//				0.0f);
//		}
//
//		TheWorld->GetTimerManager().SetTimer(TestSessionTH, this, &ATEST_RaidGameMode::EndTestSession, InTestSessionTime, false);
//	}	
//
//#if BII_STATS // 성능 수집 및 표시.
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
	// 약간의 랜덤을 가하는 것이 좋을까 아니면 완전히 똑같은 것이 좋을까.. 그동안의 팀대전이나 점령전 테스트 경험에 의하면 약간의 랜덤이 테스트 결과에 영향을 미치지는 않을 것 같다.


	FVector2D SpawnDir(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f));
	// 혹시라도 미친듯이 작은 숫자가 나오는 로또에 당첨되면 곤란하니
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
		bWingOn, MAX_WING_EVOLUTION_GRADE, true // 날개 등급까지는 정해놓고..
	);

	if (SpawnedDummy) // 애니메이션은 정해진 시간 간격으로 랜덤.
	{
		SpawnedDummy->SetUseRandomAttackState(true);
		SpawnedDummy->SetCooltimeBetweenAnim(DummyAnimPlayInterval);
	}

	return SpawnedDummy;
}

ABladeIITestDummyNPC* ATEST_RaidGameMode::SpawnSingleTestMobDummy(const FVector& InLocation, const FRotator& InRotation)
{
	// 여기엔 랜덤 안 넣음. 지정한 좌표에 그대로.
	FTransform SpawnTransform(InRotation, InLocation, FVector(1.0f, 1.0f, 1.0f));
	
	ABladeIITestDummyNPC* SpawnedDummy = ABladeIITestDummyNPC::SpawnUtil(GetWorld(), 
		DummyTargetMobClass, DummyTargetMobVariation, SpawnTransform,
		DummyFloatingUIInterval, DummyDamageFxInterval);

	if (SpawnedDummy)
	{
		SpawnedDummy->SetUseRandomAttackState(true);
		SpawnedDummy->SetCooltimeBetweenAnim(DummyAnimPlayInterval);

		// 회전만 가능하게 한 건 사르곤을 염두에 둔.. 
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
		PerfStatCollector.OnEnd(this); // 이번 세션의 성적표를 출력해 봅시다.
	}
#endif

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TestSessionTH);
	}

	// 얘는 그냥 맵에 박아놓은 데이터로다
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