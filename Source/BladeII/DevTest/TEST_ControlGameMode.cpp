// Fill out your copyright notice in the Description page of Project Settings.

#include "TEST_ControlGameMode.h"
#include "Engine.h"
#include "GameFramework/SpectatorPawn.h"
#include "Camera/CameraActor.h"
#include "Particles/Emitter.h"

#include "B2UIManager.h"
#include "Event.h"
#include "FB2ErrorMessage.h"
#include "B2PCClassInfo.h"
#include "BladeIIPlayer.h"
#include "BladeIITestDummyPlayer.h"

ATEST_ControlGameMode::ATEST_ControlGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedMinotaur = nullptr;
	DefaultPCClassEnum = EPCClass::EPC_Assassin; // ����
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	bTestSessionBegun = false;
#endif
}

void ATEST_ControlGameMode::SetResourceCacheFromBP(class ASkeletalMeshActor* InMinotaur)
{
	CachedMinotaur = InMinotaur;
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // UFUNCTION �̿��� ������ �� ������.

float ATEST_ControlGameMode::TotalTestSessionTime = 20.0f;
int32 ATEST_ControlGameMode::TestComplexityLevel = 3; // �׽�Ʈ ����� �󸶳� ���������� ���ؼ�.. �뷫 1,2,3 ���� ����

float ATEST_ControlGameMode::DummyAnimPlayInterval = 1.0f;
int32 ATEST_ControlGameMode::DummyEquipGrade = 6;
float ATEST_ControlGameMode::DummyFloatingUIInterval = 1.0f;
float ATEST_ControlGameMode::DummyDamageFxInterval = 1.0f;
float ATEST_ControlGameMode::DummySpawnRadius = 800.0f;
bool ATEST_ControlGameMode::bDummyMobOn = true;
bool ATEST_ControlGameMode::bWingOn = true;

void ATEST_ControlGameMode::BeginDestroy()
{
	Super::BeginDestroy();


}

void ATEST_ControlGameMode::PreBeginPlay()
{
	IGameRule* LocalGameRule = new TestControlGameModeGameRule;
	SetGameRule(LocalGameRule);

	Super::PreBeginPlay();
	
	GConfig->GetFloat(TEXT("ControlGameProfiling"), TEXT("TotalTestSessionTime"), TotalTestSessionTime, GGameIni);

	GConfig->GetFloat(TEXT("ControlGameProfiling"), TEXT("DummyAnimPlayInterval"), DummyAnimPlayInterval, GGameIni);
	GConfig->GetInt(TEXT("ControlGameProfiling"), TEXT("DummyEquipGrade"), DummyEquipGrade, GGameIni);
	GConfig->GetFloat(TEXT("ControlGameProfiling"), TEXT("DummyFloatingUIInterval"), DummyFloatingUIInterval, GGameIni);
	GConfig->GetFloat(TEXT("ControlGameProfiling"), TEXT("DummyDamageFxInterval"), DummyDamageFxInterval, GGameIni);
	GConfig->GetFloat(TEXT("ControlGameProfiling"), TEXT("DummySpawnRadius"), DummySpawnRadius, GGameIni);

	GConfig->GetBool(TEXT("ControlGameProfiling"), TEXT("DummyMobOn"), bDummyMobOn, GGameIni);
	GConfig->GetBool(TEXT("ControlGameProfiling"), TEXT("WingOn"), bWingOn, GGameIni);

	// ���� �������Ʈ�� ���� �Ѱܹ��� ���ҽ��� ������ �� ��ȣ�� �޵���
	GEngine->HandleCeCommand(GetWorld(), TEXT("PreBeginPlay"), *GLog);
}

void ATEST_ControlGameMode::SubscribeEvents()
{
	Super::SubscribeEvents();
}

void ATEST_ControlGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsInPreRenderPhase()) // Pre-render �� ���ư��ٸ� ������ �� �ʰ�..
	{
		BeginTestSession();
	}
}

void ATEST_ControlGameMode::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	// ���� PreRender �� ���� �׽�Ʈ ������ �������ٸ� �ڴʰ� ����.
	if (!bTestSessionBegun && !IsInPreRenderPhase())
	{
		BeginTestSession();
	}
}

void ATEST_ControlGameMode::BeginTestSession()
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
			if (TheWorldContext->LastURL.HasOption(TEXT("CL"))) // ComplexityLevel
			{
				const TCHAR* CLStr = TheWorldContext->LastURL.GetOption(TEXT("CL="), TEXT("3"));
				TestComplexityLevel = FCString::Atoi(CLStr);
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
			if (TheWorldContext->LastURL.HasOption(TEXT("SpawnRad")))
			{
				const TCHAR* SRStr = TheWorldContext->LastURL.GetOption(TEXT("SpawnRad="), TEXT("800.0"));
				DummySpawnRadius = FCString::Atof(SRStr);
			}
			if (TheWorldContext->LastURL.HasOption(TEXT("Mob")))
			{
				const TCHAR* MobStr = TheWorldContext->LastURL.GetOption(TEXT("Mob="), TEXT("1"));
				bDummyMobOn = ((FCString::Atoi(MobStr) != 0) || (!(FString(MobStr) == TEXT("False")) && !(FString(MobStr) == TEXT("Off"))));
			}
			if (TheWorldContext->LastURL.HasOption(TEXT("Wing")))
			{
				const TCHAR* WingStr = TheWorldContext->LastURL.GetOption(TEXT("Wing="), TEXT("1"));
				bWingOn = ((FCString::Atoi(WingStr) != 0) || (!(FString(WingStr) == TEXT("False")) && !(FString(WingStr) == TEXT("Off"))));
			}
		}
	}

	UE_LOG(LogBladeII, Log, TEXT("Begin ControlGameMode performance test session with Time : %.2f s, ComplexityLevel: %d"), TotalTestSessionTime, TestComplexityLevel);
	BeginTestSessionWithOptions(TotalTestSessionTime, TestComplexityLevel);
}

void ATEST_ControlGameMode::BeginTestSessionWithOptions(float InTestSessionTime, int32 InComplexityLevel)
{
	// �������� ��� ����̹Ƿ� �÷��̾� ĳ���Ͱ� �ִ�. �׽�Ʈ�� ���ص��� �ʴ� �¾����� �ϵ���.
	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	if (LocalB2Player)
	{
		//LocalB2Player->bCanBeDamaged = true;
		// �ܼ� Ŀ�ǵ� �Է��Ϸ��µ� �Ǽ��� �±׹�ư ���� ���� ���Ƽ� ��
		LocalB2Player->SetTagEnable(false);

		LocalB2Player->SetActorHiddenInGame(true); // �� ������ �� ���� �׽�Ʈ�� ���ظ� �� ��
	}
	const FVector DummyCenterLocation = LocalB2Player ? LocalB2Player->GetActorLocation() : FVector(0.0f, 0.0f, 0.0f);

	// InComplexityLevel �� ���� spawn ��Ű�� �÷��̾� ���� AnimPlay, Effect ���� �ٸ���.
	const int32 TotalDummyPlayerNum = (InComplexityLevel <= 1) ? 4 : ((InComplexityLevel == 2) ? 6 : 8);
	for (int32 DPI = 0; DPI < TotalDummyPlayerNum; ++DPI)
	{		
		ABladeIITestDummyPlayer* ThisSpawned = SpawnSingleTestPlayerDummy(InComplexityLevel, DPI, DummyCenterLocation);
		if (ThisSpawned)
		{
			AllSpawnedDummy.Add(ThisSpawned);
		}
	}

	// ��� �׳� �ʿ� �ھƳ��� �����ͷδ�
	if (CachedMinotaur && CachedMinotaur->GetSkeletalMeshComponent())
	{
		if (bDummyMobOn)
		{
			CachedMinotaur->GetSkeletalMeshComponent()->GlobalAnimRateScale = 1.0f;
			CachedMinotaur->SetActorHiddenInGame(false);
		}
		else
		{
			CachedMinotaur->GetSkeletalMeshComponent()->GlobalAnimRateScale = 0.0f;
			CachedMinotaur->SetActorHiddenInGame(true);
		}
	}

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(TestSessionTH, this, &ATEST_ControlGameMode::EndTestSession, InTestSessionTime, false);
	}	

#if BII_STATS // ������ ������ ���� �̰� �⺻���� �Ҵ�. ���� ������ �����ߴ��� �ٽ� ����.
	FStatisticalPerfStatCollector::bDoStatCollecting = true;
	PerfStatCollector.OnBegin(this);
#endif

	bTestSessionBegun = true;
}

ABladeIITestDummyPlayer* ATEST_ControlGameMode::SpawnSingleTestPlayerDummy(int32 InComplexityLevel, int32 InMyIndex, const FVector& InDummyCenterLocation)
{
	// InComplexityLevel �� ���� �߰� ������ �ұ? �ϴ� Spawn ������ ������ �ִ� �ǵ�..

	// �ణ�� ������ ���ϴ� ���� ������ �ƴϸ� ������ �Ȱ��� ���� ������.. ������ �׽�Ʈ ���迡 ���ϸ� �ణ�� ������ �׽�Ʈ ����� ������ ��ġ���� ���� �� ����.


	FVector2D SpawnDir(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f));
	// Ȥ�ö� ��ģ���� ���� ���ڰ� ������ �ζǿ� ��÷�Ǹ� ����ϴ�
	if (FMath::Abs(SpawnDir.X) < KINDA_SMALL_NUMBER && FMath::Abs(SpawnDir.Y) < KINDA_SMALL_NUMBER)
	{
		SpawnDir.X = KINDA_SMALL_NUMBER;
		SpawnDir.Y = KINDA_SMALL_NUMBER;
	}
	SpawnDir.Normalize();
	float SpawnDist = FMath::FRandRange(50.0f, FMath::Max(DummySpawnRadius,100.0f));

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

void ATEST_ControlGameMode::EndTestSession()
{
#if BII_STATS
	PerfStatCollector.OnEnd(this); // �̹� ������ ����ǥ�� ����� ���ô�.
#endif

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TestSessionTH);
	}

	// ��� �׳� �ʿ� �ھƳ��� �����ͷδ�
	if (CachedMinotaur && CachedMinotaur->GetSkeletalMeshComponent())
	{
		CachedMinotaur->GetSkeletalMeshComponent()->GlobalAnimRateScale = 0.0f;
		CachedMinotaur->SetActorHiddenInGame(true);
	}

	for (ABladeIITestDummyPlayer* ThisDummy : AllSpawnedDummy)
	{
		if (ThisDummy && ThisDummy->IsValidObj())
		{
			ThisDummy->Destroy();
		}
	}
	AllSpawnedDummy.Empty();
}
#if BII_STATS
void ATEST_ControlGameMode::DrawAdditionalPerfStatInfo(float DrawXSoFar, float DrawYSoFar)
{
	if (!UB2PCMeshSectionMergeInfo::ShouldForceUseSectionMerge())
	{
		// �Ϲ������δ� ������ ȯ���� �״�� �����ٸ� section merge ��带 �����ؾ� �ϹǷ� �ƴ� ��� ȭ�鿡 ǥ��.
		// ���� �׽�Ʈ �������� section merge �� �� �ϰ� �ϴ� ��쵵 ���� ��.
		DrawDebugText(DrawXSoFar, DrawYSoFar, FString::Printf(TEXT("PC Section Merge NOT forced be ON!")), FLinearColor::Red, 24);
	}
}
#endif

#endif