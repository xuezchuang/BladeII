//// Fill out your copyright notice in the Description page of Project Settings.
//
//#include "TEST_ControlGameMode.h"
//#include "Engine.h"
//#include "GameFramework/SpectatorPawn.h"
//#include "Camera/CameraActor.h"
//#include "Particles/Emitter.h"
//
//#include "B2UIManager.h"
//#include "Event.h"
//#include "FB2ErrorMessage.h"
//#include "B2PCClassInfo.h"
//#include "BladeIIPlayer.h"
//#include "BladeIITestDummyPlayer.h"
//
//ATEST_ControlGameMode::ATEST_ControlGameMode(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	CachedMinotaur = nullptr;
//	DefaultPCClassEnum = EPCClass::EPC_Assassin; // 郴妇
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	bTestSessionBegun = false;
//#endif
//}
//
//void ATEST_ControlGameMode::SetResourceCacheFromBP(class ASkeletalMeshActor* InMinotaur)
//{
//	CachedMinotaur = InMinotaur;
//}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // UFUNCTION 捞寇狼 备泅篮 捞 救栏肺.
//
//float ATEST_ControlGameMode::TotalTestSessionTime = 20.0f;
//int32 ATEST_ControlGameMode::TestComplexityLevel = 3; // 抛胶飘 厘搁捞 倔付唱 汗棱茄瘤俊 措秦辑.. 措帆 1,2,3 沥档 积阿
//
//float ATEST_ControlGameMode::DummyAnimPlayInterval = 1.0f;
//int32 ATEST_ControlGameMode::DummyEquipGrade = 6;
//float ATEST_ControlGameMode::DummyFloatingUIInterval = 1.0f;
//float ATEST_ControlGameMode::DummyDamageFxInterval = 1.0f;
//float ATEST_ControlGameMode::DummySpawnRadius = 800.0f;
//bool ATEST_ControlGameMode::bDummyMobOn = true;
//bool ATEST_ControlGameMode::bWingOn = true;
//
//void ATEST_ControlGameMode::BeginDestroy()
//{
//	Super::BeginDestroy();
//
//
//}
//
//void ATEST_ControlGameMode::PreBeginPlay()
//{
//	IGameRule* LocalGameRule = new TestControlGameModeGameRule;
//	SetGameRule(LocalGameRule);
//
//	Super::PreBeginPlay();
//	
//	GConfig->GetFloat(TEXT("ControlGameProfiling"), TEXT("TotalTestSessionTime"), TotalTestSessionTime, GGameIni);
//
//	GConfig->GetFloat(TEXT("ControlGameProfiling"), TEXT("DummyAnimPlayInterval"), DummyAnimPlayInterval, GGameIni);
//	GConfig->GetInt(TEXT("ControlGameProfiling"), TEXT("DummyEquipGrade"), DummyEquipGrade, GGameIni);
//	GConfig->GetFloat(TEXT("ControlGameProfiling"), TEXT("DummyFloatingUIInterval"), DummyFloatingUIInterval, GGameIni);
//	GConfig->GetFloat(TEXT("ControlGameProfiling"), TEXT("DummyDamageFxInterval"), DummyDamageFxInterval, GGameIni);
//	GConfig->GetFloat(TEXT("ControlGameProfiling"), TEXT("DummySpawnRadius"), DummySpawnRadius, GGameIni);
//
//	GConfig->GetBool(TEXT("ControlGameProfiling"), TEXT("DummyMobOn"), bDummyMobOn, GGameIni);
//	GConfig->GetBool(TEXT("ControlGameProfiling"), TEXT("WingOn"), bWingOn, GGameIni);
//
//	// 饭骇 喉风橇赴飘甫 烹秦 逞败罐阑 府家胶啊 乐栏搁 捞 脚龋甫 罐档废
//	GEngine->HandleCeCommand(GetWorld(), TEXT("PreBeginPlay"), *GLog);
//}
//
//void ATEST_ControlGameMode::SubscribeEvents()
//{
//	Super::SubscribeEvents();
//}
//
//void ATEST_ControlGameMode::BeginPlay()
//{
//	Super::BeginPlay();
//	
//	if (!IsInPreRenderPhase()) // Pre-render 啊 倒酒埃促搁 矫累阑 粱 词霸..
//	{
//		BeginTestSession();
//	}
//}
//
//void ATEST_ControlGameMode::Tick(float DeltaSecond)
//{
//	Super::Tick(DeltaSecond);
//
//	// 父老 PreRender 肺 牢秦 抛胶飘 技记捞 词苗脸促搁 第词霸 矫累.
//	if (!bTestSessionBegun && !IsInPreRenderPhase())
//	{
//		BeginTestSession();
//	}
//}
//
//void ATEST_ControlGameMode::BeginTestSession()
//{
//	// 捞 甘篮 Open 疙飞栏肺 凯霸 瞪 搏单 抛胶飘 包访 牢磊甫 ? 肺 备盒等 可记栏肺 持绢霖促.
//	if (GEngine)
//	{
//		FWorldContext* TheWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
//		if (TheWorldContext)
//		{
//			if (TheWorldContext->LastURL.HasOption(TEXT("Time")))
//			{
//				const TCHAR* TimeStr = TheWorldContext->LastURL.GetOption(TEXT("Time="), TEXT("20.0f"));
//				TotalTestSessionTime = FCString::Atof(TimeStr);
//			}
//			if (TheWorldContext->LastURL.HasOption(TEXT("CL"))) // ComplexityLevel
//			{
//				const TCHAR* CLStr = TheWorldContext->LastURL.GetOption(TEXT("CL="), TEXT("3"));
//				TestComplexityLevel = FCString::Atoi(CLStr);
//			}
//			// 歹固 敲饭捞绢 技何 可记甸
//			if (TheWorldContext->LastURL.HasOption(TEXT("Equip"))) 
//			{
//				const TCHAR* EquipStr = TheWorldContext->LastURL.GetOption(TEXT("Equip="), TEXT("6"));
//				DummyEquipGrade = FCString::Atoi(EquipStr);
//			}
//			if (TheWorldContext->LastURL.HasOption(TEXT("AT")))
//			{
//				const TCHAR* ATStr = TheWorldContext->LastURL.GetOption(TEXT("AT="), TEXT("1.0"));
//				DummyAnimPlayInterval = FCString::Atof(ATStr);
//			}
//			if (TheWorldContext->LastURL.HasOption(TEXT("FUI")))
//			{
//				const TCHAR* FUIStr = TheWorldContext->LastURL.GetOption(TEXT("FUI="), TEXT("1.0"));
//				DummyFloatingUIInterval = FCString::Atof(FUIStr);
//			}
//			if (TheWorldContext->LastURL.HasOption(TEXT("DFX")))
//			{
//				const TCHAR* DFXStr = TheWorldContext->LastURL.GetOption(TEXT("DFX="), TEXT("1.0"));
//				DummyDamageFxInterval = FCString::Atof(DFXStr);
//			}
//			if (TheWorldContext->LastURL.HasOption(TEXT("SpawnRad")))
//			{
//				const TCHAR* SRStr = TheWorldContext->LastURL.GetOption(TEXT("SpawnRad="), TEXT("800.0"));
//				DummySpawnRadius = FCString::Atof(SRStr);
//			}
//			if (TheWorldContext->LastURL.HasOption(TEXT("Mob")))
//			{
//				const TCHAR* MobStr = TheWorldContext->LastURL.GetOption(TEXT("Mob="), TEXT("1"));
//				bDummyMobOn = ((FCString::Atoi(MobStr) != 0) || (!(FString(MobStr) == TEXT("False")) && !(FString(MobStr) == TEXT("Off"))));
//			}
//			if (TheWorldContext->LastURL.HasOption(TEXT("Wing")))
//			{
//				const TCHAR* WingStr = TheWorldContext->LastURL.GetOption(TEXT("Wing="), TEXT("1"));
//				bWingOn = ((FCString::Atoi(WingStr) != 0) || (!(FString(WingStr) == TEXT("False")) && !(FString(WingStr) == TEXT("Off"))));
//			}
//		}
//	}
//
//	UE_LOG(LogBladeII, Log, TEXT("Begin ControlGameMode performance test session with Time : %.2f s, ComplexityLevel: %d"), TotalTestSessionTime, TestComplexityLevel);
//	BeginTestSessionWithOptions(TotalTestSessionTime, TestComplexityLevel);
//}
//
//void ATEST_ControlGameMode::BeginTestSessionWithOptions(float InTestSessionTime, int32 InComplexityLevel)
//{
//	// 胶抛捞瘤 葛靛 扁馆捞骨肺 敲饭捞绢 某腐磐啊 乐促. 抛胶飘俊 规秦登瘤 臼绰 悸诀甸阑 窍档废.
//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	if (LocalB2Player)
//	{
//		//LocalB2Player->bCanBeDamaged = true;
//		// 能贾 目盖靛 涝仿窍妨绰单 角荐肺 怕弊滚瓢 穿甫 锭啊 腹酒辑 せ
//		LocalB2Player->SetTagEnable(false);
//
//		LocalB2Player->SetActorHiddenInGame(true); // 傲 见扁磊 せ 宝洒 抛胶飘俊 规秦父 瞪 淀
//	}
//	const FVector DummyCenterLocation = LocalB2Player ? LocalB2Player->GetActorLocation() : FVector(0.0f, 0.0f, 0.0f);
//
//	// InComplexityLevel 俊 蝶扼 spawn 矫虐绰 敲饭捞绢 荐唱 AnimPlay, Effect 殿捞 促福促.
//	const int32 TotalDummyPlayerNum = (InComplexityLevel <= 1) ? 4 : ((InComplexityLevel == 2) ? 6 : 8);
//	for (int32 DPI = 0; DPI < TotalDummyPlayerNum; ++DPI)
//	{		
//		ABladeIITestDummyPlayer* ThisSpawned = SpawnSingleTestPlayerDummy(InComplexityLevel, DPI, DummyCenterLocation);
//		if (ThisSpawned)
//		{
//			AllSpawnedDummy.Add(ThisSpawned);
//		}
//	}
//
//	// 娟绰 弊成 甘俊 冠酒初篮 单捞磐肺促
//	if (CachedMinotaur && CachedMinotaur->GetSkeletalMeshComponent())
//	{
//		if (bDummyMobOn)
//		{
//			CachedMinotaur->GetSkeletalMeshComponent()->GlobalAnimRateScale = 1.0f;
//			CachedMinotaur->SetActorHiddenInGame(false);
//		}
//		else
//		{
//			CachedMinotaur->GetSkeletalMeshComponent()->GlobalAnimRateScale = 0.0f;
//			CachedMinotaur->SetActorHiddenInGame(true);
//		}
//	}
//
//	UWorld* TheWorld = GetWorld();
//	if (TheWorld)
//	{
//		TheWorld->GetTimerManager().SetTimer(TestSessionTH, this, &ATEST_ControlGameMode::EndTestSession, InTestSessionTime, false);
//	}	
//
//#if BII_STATS // 己瓷阑 荐笼且 波聪 捞扒 扁夯栏肺 囊促. 父老 捞傈俊 矫累沁歹扼档 促矫 府悸.
//	FStatisticalPerfStatCollector::bDoStatCollecting = true;
//	PerfStatCollector.OnBegin(this);
//#endif
//
//	bTestSessionBegun = true;
//}
//
//ABladeIITestDummyPlayer* ATEST_ControlGameMode::SpawnSingleTestPlayerDummy(int32 InComplexityLevel, int32 InMyIndex, const FVector& InDummyCenterLocation)
//{
//	// InComplexityLevel 俊 蝶弗 眠啊 焊沥阑 且鳖唱? 老窜 Spawn 俺荐俊 康氢阑 林绰 扒单..
//
//	// 距埃狼 罚待阑 啊窍绰 巴捞 亮阑鳖 酒聪搁 肯傈洒 度鞍篮 巴捞 亮阑鳖.. 评措傈 抛胶飘 版氰俊 狼窍搁 距埃狼 罚待捞 抛胶飘 搬苞俊 康氢阑 固摹瘤绰 臼阑 巴 鞍促.
//
//
//	FVector2D SpawnDir(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f));
//	// 趣矫扼档 固模淀捞 累篮 箭磊啊 唱坷绰 肺肚俊 寸梅登搁 帮鄂窍聪
//	if (FMath::Abs(SpawnDir.X) < KINDA_SMALL_NUMBER && FMath::Abs(SpawnDir.Y) < KINDA_SMALL_NUMBER)
//	{
//		SpawnDir.X = KINDA_SMALL_NUMBER;
//		SpawnDir.Y = KINDA_SMALL_NUMBER;
//	}
//	SpawnDir.Normalize();
//	float SpawnDist = FMath::FRandRange(50.0f, FMath::Max(DummySpawnRadius,100.0f));
//
//	FTransform SpawnTransform(
//		FRotator(0.0f, FMath::FRandRange(-180.0f,180.0f), 0.0f),
//		InDummyCenterLocation + FVector(SpawnDir.X * SpawnDist, SpawnDir.Y * SpawnDist, 0.0f),
//		FVector(1.0f, 1.0f, 1.0f)
//	);
//
//	EPCClass SpawnClass = IntToPCClass(FMath::RandRange(0, 3));
//
//	ABladeIITestDummyPlayer* SpawnedDummy = ABladeIITestDummyPlayer::SpawnWithStandardEquipment(
//		GetWorld(), SpawnClass, SpawnTransform, DummyEquipGrade, DummyFloatingUIInterval, DummyDamageFxInterval,
//		bWingOn, MAX_WING_EVOLUTION_GRADE, true // 朝俺 殿鞭鳖瘤绰 沥秦初绊..
//	);
//
//	if (SpawnedDummy) // 局聪皋捞记篮 沥秦柳 矫埃 埃拜栏肺 罚待.
//	{
//		SpawnedDummy->SetUseRandomAttackState(true);
//		SpawnedDummy->SetCooltimeBetweenAnim(DummyAnimPlayInterval);
//	}
//
//	return SpawnedDummy;
//}
//
//void ATEST_ControlGameMode::EndTestSession()
//{
//#if BII_STATS
//	PerfStatCollector.OnEnd(this); // 捞锅 技记狼 己利钎甫 免仿秦 壕矫促.
//#endif
//
//	UWorld* TheWorld = GetWorld();
//	if (TheWorld)
//	{
//		TheWorld->GetTimerManager().ClearTimer(TestSessionTH);
//	}
//
//	// 娟绰 弊成 甘俊 冠酒初篮 单捞磐肺促
//	if (CachedMinotaur && CachedMinotaur->GetSkeletalMeshComponent())
//	{
//		CachedMinotaur->GetSkeletalMeshComponent()->GlobalAnimRateScale = 0.0f;
//		CachedMinotaur->SetActorHiddenInGame(true);
//	}
//
//	for (ABladeIITestDummyPlayer* ThisDummy : AllSpawnedDummy)
//	{
//		if (ThisDummy && ThisDummy->IsValidObj())
//		{
//			ThisDummy->Destroy();
//		}
//	}
//	AllSpawnedDummy.Empty();
//}
//#if BII_STATS
//void ATEST_ControlGameMode::DrawAdditionalPerfStatInfo(float DrawXSoFar, float DrawYSoFar)
//{
//	if (!UB2PCMeshSectionMergeInfo::ShouldForceUseSectionMerge())
//	{
//		// 老馆利栏肺绰 痢飞傈 券版阑 弊措肺 蝶弗促搁 section merge 葛靛甫 碍力秦具 窍骨肺 酒囱 版快 拳搁俊 钎矫.
//		// 拱沸 抛胶飘 格利栏肺 section merge 甫 救 窍绊 窍绰 版快档 乐阑 巴.
//		DrawDebugText(DrawXSoFar, DrawYSoFar, FString::Printf(TEXT("PC Section Merge NOT forced be ON!")), FLinearColor::Red, 24);
//	}
//}
//#endif
//
//#endif