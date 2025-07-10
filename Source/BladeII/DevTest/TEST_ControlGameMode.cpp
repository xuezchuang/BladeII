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
	DefaultPCClassEnum = EPCClass::EPC_Assassin; // 내맘
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	bTestSessionBegun = false;
#endif
}

void ATEST_ControlGameMode::SetResourceCacheFromBP(class ASkeletalMeshActor* InMinotaur)
{
	CachedMinotaur = InMinotaur;
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // UFUNCTION 이외의 구현은 이 안으로.

float ATEST_ControlGameMode::TotalTestSessionTime = 20.0f;
int32 ATEST_ControlGameMode::TestComplexityLevel = 3; // 테스트 장면이 얼마나 복잡한지에 대해서.. 대략 1,2,3 정도 생각

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

	// 레벨 블루프린트를 통해 넘겨받을 리소스가 있으면 이 신호를 받도록
	GEngine->HandleCeCommand(GetWorld(), TEXT("PreBeginPlay"), *GLog);
}

void ATEST_ControlGameMode::SubscribeEvents()
{
	Super::SubscribeEvents();
}

void ATEST_ControlGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsInPreRenderPhase()) // Pre-render 가 돌아간다면 시작을 좀 늦게..
	{
		BeginTestSession();
	}
}

void ATEST_ControlGameMode::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	// 만일 PreRender 로 인해 테스트 세션이 늦춰졌다면 뒤늦게 시작.
	if (!bTestSessionBegun && !IsInPreRenderPhase())
	{
		BeginTestSession();
	}
}

void ATEST_ControlGameMode::BeginTestSession()
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
			if (TheWorldContext->LastURL.HasOption(TEXT("CL"))) // ComplexityLevel
			{
				const TCHAR* CLStr = TheWorldContext->LastURL.GetOption(TEXT("CL="), TEXT("3"));
				TestComplexityLevel = FCString::Atoi(CLStr);
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
	// 스테이지 모드 기반이므로 플레이어 캐릭터가 있다. 테스트에 방해되지 않는 셋업들을 하도록.
	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	if (LocalB2Player)
	{
		//LocalB2Player->bCanBeDamaged = true;
		// 콘솔 커맨드 입력하려는데 실수로 태그버튼 누를 때가 많아서 ㅋ
		LocalB2Player->SetTagEnable(false);

		LocalB2Player->SetActorHiddenInGame(true); // 걍 숨기자 ㅋ 괜히 테스트에 방해만 될 듯
	}
	const FVector DummyCenterLocation = LocalB2Player ? LocalB2Player->GetActorLocation() : FVector(0.0f, 0.0f, 0.0f);

	// InComplexityLevel 에 따라 spawn 시키는 플레이어 수나 AnimPlay, Effect 등이 다르다.
	const int32 TotalDummyPlayerNum = (InComplexityLevel <= 1) ? 4 : ((InComplexityLevel == 2) ? 6 : 8);
	for (int32 DPI = 0; DPI < TotalDummyPlayerNum; ++DPI)
	{		
		ABladeIITestDummyPlayer* ThisSpawned = SpawnSingleTestPlayerDummy(InComplexityLevel, DPI, DummyCenterLocation);
		if (ThisSpawned)
		{
			AllSpawnedDummy.Add(ThisSpawned);
		}
	}

	// 얘는 그냥 맵에 박아놓은 데이터로다
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

#if BII_STATS // 성능을 수집할 꺼니 이건 기본으로 켠다. 만일 이전에 시작했더라도 다시 리셋.
	FStatisticalPerfStatCollector::bDoStatCollecting = true;
	PerfStatCollector.OnBegin(this);
#endif

	bTestSessionBegun = true;
}

ABladeIITestDummyPlayer* ATEST_ControlGameMode::SpawnSingleTestPlayerDummy(int32 InComplexityLevel, int32 InMyIndex, const FVector& InDummyCenterLocation)
{
	// InComplexityLevel 에 따른 추가 보정을 할까나? 일단 Spawn 개수에 영향을 주는 건데..

	// 약간의 랜덤을 가하는 것이 좋을까 아니면 완전히 똑같은 것이 좋을까.. 팀대전 테스트 경험에 의하면 약간의 랜덤이 테스트 결과에 영향을 미치지는 않을 것 같다.


	FVector2D SpawnDir(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f));
	// 혹시라도 미친듯이 작은 숫자가 나오는 로또에 당첨되면 곤란하니
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
		bWingOn, MAX_WING_EVOLUTION_GRADE, true // 날개 등급까지는 정해놓고..
	);

	if (SpawnedDummy) // 애니메이션은 정해진 시간 간격으로 랜덤.
	{
		SpawnedDummy->SetUseRandomAttackState(true);
		SpawnedDummy->SetCooltimeBetweenAnim(DummyAnimPlayInterval);
	}

	return SpawnedDummy;
}

void ATEST_ControlGameMode::EndTestSession()
{
#if BII_STATS
	PerfStatCollector.OnEnd(this); // 이번 세션의 성적표를 출력해 봅시다.
#endif

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(TestSessionTH);
	}

	// 얘는 그냥 맵에 박아놓은 데이터로다
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
		// 일반적으로는 점령전 환경을 그대로 따른다면 section merge 모드를 강제해야 하므로 아닌 경우 화면에 표시.
		// 물론 테스트 목적으로 section merge 를 안 하고 하는 경우도 있을 것.
		DrawDebugText(DrawXSoFar, DrawYSoFar, FString::Printf(TEXT("PC Section Merge NOT forced be ON!")), FLinearColor::Red, 24);
	}
}
#endif

#endif