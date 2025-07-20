//// Fill out your copyright notice in the Description page of Project Settings.
//#include "TEST_RaidGameMode.h"
//
//#include "Engine.h"
//#include "GameFramework/SpectatorPawn.h"
//#include "Camera/CameraActor.h"
//#include "Particles/Emitter.h"
//
//#include "B2UIManager.h"
//#include "Event.h"
//#include "FB2ErrorMessage.h"
//#include "BladeIITestDummyPlayer.h"
//#include "BladeIITestDummyNPC.h"
//#include "BladeIIWorldSettings.h"
//#include "BladeIIPlayerController.h"
//
//ATEST_RaidGameMode::ATEST_RaidGameMode(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	DummyRaidTargetMob = nullptr;
//	DefaultPCClassEnum = EPCClass::EPC_Assassin; // 郴妇
//
//	DummyTargetMobClass = ENPCClass::ENC_Boss_Sargon;
//	DummyTargetMobVariation = ENPCClassVariation::ENCV_Normal;
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	bTestSessionBegun = false;
//
//	TotalTestSessionTime = 600.0f; // 老瞒利牢 侩档啊 单捞磐 惶绰 霸 酒聪扼 敲饭捞 焊搁辑 炼例窍绰 芭扼聪 扁夯 矫埃 辨霸..
//	DummyAnimPlayInterval = 0.2f;
//	DummyEquipGrade = 6;
//	DummyFloatingUIInterval = 1.0f;
//	DummyDamageFxInterval = 1.0f;
//	DummySpawnRandRadius = 100.0f; // 阿 歹固 咀磐付促 困摹甫 瘤沥且 荐 乐霸 且 芭扼 困摹 罚待 裹困绰 农瘤 臼霸 茄促.
//	CamDistScale = 1.0f;
//	bWingOn = true;
//	bGatherPerfStats = false;
//#endif
//}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // UFUNCTION 捞寇狼 备泅篮 捞 救栏肺.
//
//void ATEST_RaidGameMode::BeginDestroy()
//{
//	Super::BeginDestroy();
//
//}
//
//void ATEST_RaidGameMode::PreBeginPlay()
//{
//	IGameRule* LocalGameRule = new TestRaidGameModeGameRule;
//	SetGameRule(LocalGameRule);
//
//	Super::PreBeginPlay();
//	
//}
//
//void ATEST_RaidGameMode::SubscribeEvents()
//{
//	Super::SubscribeEvents();
//}
//
//void ATEST_RaidGameMode::BeginPlay()
//{
//	Super::BeginPlay();
//	
//	if (!IsInPreRenderPhase()) // Pre-render 啊 倒酒埃促搁 矫累阑 粱 词霸..
//	{
//		BeginTestSession();
//	}
//}
//
//void ATEST_RaidGameMode::Tick(float DeltaSecond)
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
//void ATEST_RaidGameMode::GatherDummyMarkers(ASkeletalMeshActor*& OutDummyTargetMobMarker, TArray<ASkeletalMeshActor*>& OutDummyPlayerMarkers)
//{
//	// 茫酒辰 巴甸狼 沥焊甫 Dummy Actor spawn 俊 荤侩且 芭.
//	UWorld* TheWorld = GetWorld();
//	if (TheWorld)
//	{
//		for (FActorIterator ActorIt(TheWorld); ActorIt; ++ActorIt)
//		{
//			ASkeletalMeshActor* ThisSKActor = Cast<ASkeletalMeshActor>(*ActorIt);
//			// 沥秦初篮 捞抚栏肺 硅摹茄 吧 荤侩.
//			if (ThisSKActor)
//			{
//				FString ActorName = ThisSKActor->GetName();
//				if (ActorName.Contains(TEXT("DummyTargetMob_Marker")))
//				{
//					OutDummyTargetMobMarker = ThisSKActor;
//				}
//				else if (ActorName.Contains(TEXT("DummyPlayer01_Marker"))
//					|| ActorName.Contains(TEXT("DummyPlayer02_Marker"))
//					|| ActorName.Contains(TEXT("DummyPlayer03_Marker"))
//					|| ActorName.Contains(TEXT("DummyPlayer04_Marker"))
//					)
//				{
//					OutDummyPlayerMarkers.Add(ThisSKActor);
//				}
//			}
//		}
//	}
//}
//
//void ATEST_RaidGameMode::BeginTestSession()
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
//
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
//			if (TheWorldContext->LastURL.HasOption(TEXT("CamDistScale")))
//			{
//				const TCHAR* CDSStr = TheWorldContext->LastURL.GetOption(TEXT("CamDistScale="), TEXT("1.0"));
//				CamDistScale = FCString::Atof(CDSStr);
//			}
//			else if (TheWorldContext->LastURL.HasOption(TEXT("CDS")))
//			{
//				const TCHAR* CDSStr = TheWorldContext->LastURL.GetOption(TEXT("CDS="), TEXT("1.0"));
//				CamDistScale = FCString::Atof(CDSStr);
//			}
//			if (TheWorldContext->LastURL.HasOption(TEXT("SpawnRad")))
//			{
//				const TCHAR* SRStr = TheWorldContext->LastURL.GetOption(TEXT("SpawnRad="), TEXT("800.0"));
//				DummySpawnRandRadius = FCString::Atof(SRStr);
//			}
//			if (TheWorldContext->LastURL.HasOption(TEXT("Wing")))
//			{
//				const TCHAR* WingStr = TheWorldContext->LastURL.GetOption(TEXT("Wing="), TEXT("1"));
//				bWingOn = ((FCString::Atoi(WingStr) != 0) || (!(FString(WingStr) == TEXT("False")) && !(FString(WingStr) == TEXT("Off"))));
//			}
//			if (TheWorldContext->LastURL.HasOption(TEXT("PerfStat")))
//			{
//				const TCHAR* PerfStatStr = TheWorldContext->LastURL.GetOption(TEXT("PerfStat="), TEXT("1"));
//				bGatherPerfStats = ((FCString::Atoi(PerfStatStr) != 0) || (!(FString(PerfStatStr) == TEXT("False")) && !(FString(PerfStatStr) == TEXT("Off"))));
//			}
//			else if (TheWorldContext->LastURL.HasOption(TEXT("PS")))
//			{
//				const TCHAR* PerfStatStr = TheWorldContext->LastURL.GetOption(TEXT("PS="), TEXT("1"));
//				bGatherPerfStats = ((FCString::Atoi(PerfStatStr) != 0) || (!(FString(PerfStatStr) == TEXT("False")) && !(FString(PerfStatStr) == TEXT("Off"))));
//			}
//		}
//	}
//
//	UE_LOG(LogBladeII, Log, TEXT("Begin RaidGameMode performance test session with Time : %.2f s"), TotalTestSessionTime);
//	BeginTestSessionWithOptions(TotalTestSessionTime);
//}
//
//void ATEST_RaidGameMode::BeginTestSessionWithOptions(float InTestSessionTime)
//{
////	// 胶抛捞瘤 葛靛 扁馆捞骨肺 敲饭捞绢 某腐磐啊 乐促. 抛胶飘俊 规秦登瘤 臼绰 悸诀甸阑 窍档废.
////	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
////	if (LocalB2Player)
////	{
////		//LocalB2Player->bCanBeDamaged = true;
////		// 能贾 目盖靛 涝仿窍妨绰单 角荐肺 怕弊滚瓢 穿甫 锭啊 腹酒辑 せ
////		LocalB2Player->SetTagEnable(false);
////
////		LocalB2Player->SetActorHiddenInGame(true); // 傲 见扁磊 せ 宝洒 抛胶飘俊 规秦父 瞪 淀
////	}
////	const FVector DefaultDummyCenterLocation = LocalB2Player ? LocalB2Player->GetActorLocation() : FVector(0.0f, 0.0f, 0.0f);
////		
////	// 甘俊 硅摹秦 初篮 SkeletalMeshActor 困摹甫 啊瘤绊 扁霖 困摹肺 伙澜. 捞霸 绝栏搁 肺拿 敲饭捞绢 某腐磐 困摹甫 扁霖栏肺.
////	ASkeletalMeshActor* TargetMobMarker = nullptr;
////	TArray<ASkeletalMeshActor*> DummyPlayerMarkers;
////	GatherDummyMarkers(TargetMobMarker, DummyPlayerMarkers);
////
////	// 痢飞傈 抛胶飘俊辑绰 敲饭捞绢 荐甫 函版矫懦 鞘夸啊 粱 乐菌瘤父 捞扒 漏绝捞 4付府.
////	const int32 TotalDummyPlayerNum = 4;
////	for (int32 DPI = 0; DPI < TotalDummyPlayerNum; ++DPI)
////	{		
////		ASkeletalMeshActor* ThisDummyMarker = DummyPlayerMarkers.IsValidIndex(DPI) ? DummyPlayerMarkers[DPI] : nullptr;
////		ABladeIITestDummyPlayer* ThisSpawned = SpawnSingleTestPlayerDummy(DPI, 
////			ThisDummyMarker ? ThisDummyMarker->GetActorLocation() + FVector(0.0f, 0.0f, 200.0f) : DefaultDummyCenterLocation);
////		if (ThisSpawned)
////		{
////			AllDummyPlayers.Add(ThisSpawned);
////		}
////	}
////
////	DummyRaidTargetMob = SpawnSingleTestMobDummy(
////		// 困摹甫 棵府绰 扒 窍窜 扁霖 SkeletalMeshActor 客狼 offset 焊沥牢单 沥犬窍霸 窍瘤绰 给窍绊.. 绢瞒乔 spawn 矫 顶官蹿俊 snap 凳.
////		TargetMobMarker ? TargetMobMarker->GetActorLocation() + FVector(0.0f,0.0f,500.0f) : DefaultDummyCenterLocation,
////		TargetMobMarker ? TargetMobMarker->GetActorRotation() : FRotator(0.f, 0.f, 0.f)
////	);
////
////	UWorld* TheWorld = GetWorld();
////	if (TheWorld)
////	{
////		ABladeIIWorldSettings* B2WS = Cast<ABladeIIWorldSettings>(TheWorld->GetWorldSettings());
////		if (B2WS)
////		{
////			// WorldSetting 狼 CameraSetting 阑 静绰 啊沥. ActiveCameraActor 啊 乐促搁 公侩瘤拱.
////			// 父老 甘捞 肺爹等 盲肺 捞霸 咯矾锅 阂府搁 胶纳老捞 拌加 穿利登摆瘤..
////			B2WS->DefaultPlayerCameraSettings.bOverrideCameraBoomArmLength = true; // DefaultPlayerCameraSettings 立辟窍妨绊 friend class 急攫窃. 沥侥 扁瓷篮 酒聪聪 利寸洒..
////			B2WS->DefaultPlayerCameraSettings.CameraBoomArmLength *= CamDistScale;
////			ApplyNormallyExpectedInGameCamera(this,
////				Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(TheWorld)),
////				0.0f);
////		}
////
////		TheWorld->GetTimerManager().SetTimer(TestSessionTH, this, &ATEST_RaidGameMode::EndTestSession, InTestSessionTime, false);
////	}	
////
////#if BII_STATS // 己瓷 荐笼 棺 钎矫.
////	FStatisticalPerfStatCollector::bDoStatCollecting = bGatherPerfStats;
////	if (bGatherPerfStats) {
////		PerfStatCollector.OnBegin(this);
////	}
////#endif
////
////	bTestSessionBegun = true;
//}
//
//ABladeIITestDummyPlayer* ATEST_RaidGameMode::SpawnSingleTestPlayerDummy(int32 InMyIndex, const FVector& InDummyCenterLocation)
//{
//	// 距埃狼 罚待阑 啊窍绰 巴捞 亮阑鳖 酒聪搁 肯傈洒 度鞍篮 巴捞 亮阑鳖.. 弊悼救狼 评措傈捞唱 痢飞傈 抛胶飘 版氰俊 狼窍搁 距埃狼 罚待捞 抛胶飘 搬苞俊 康氢阑 固摹瘤绰 臼阑 巴 鞍促.
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
//	float SpawnDist = FMath::FRandRange(50.0f, FMath::Max(DummySpawnRandRadius,50.0f));
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
//ABladeIITestDummyNPC* ATEST_RaidGameMode::SpawnSingleTestMobDummy(const FVector& InLocation, const FRotator& InRotation)
//{
//	// 咯扁浚 罚待 救 持澜. 瘤沥茄 谅钎俊 弊措肺.
//	FTransform SpawnTransform(InRotation, InLocation, FVector(1.0f, 1.0f, 1.0f));
//	
//	ABladeIITestDummyNPC* SpawnedDummy = ABladeIITestDummyNPC::SpawnUtil(GetWorld(), 
//		DummyTargetMobClass, DummyTargetMobVariation, SpawnTransform,
//		DummyFloatingUIInterval, DummyDamageFxInterval);
//
//	if (SpawnedDummy)
//	{
//		SpawnedDummy->SetUseRandomAttackState(true);
//		SpawnedDummy->SetCooltimeBetweenAnim(DummyAnimPlayInterval);
//
//		// 雀傈父 啊瓷窍霸 茄 扒 荤福帮阑 堪滴俊 敌.. 
//		SpawnedDummy->SetMaintainLocation(true);
//		SpawnedDummy->SetMaintainRotation(false);
//	}
//
//	return SpawnedDummy;
//}
//
//void ATEST_RaidGameMode::EndTestSession()
//{
//#if BII_STATS
//	if (bGatherPerfStats)
//	{
//		PerfStatCollector.OnEnd(this); // 捞锅 技记狼 己利钎甫 免仿秦 壕矫促.
//	}
//#endif
//
//	UWorld* TheWorld = GetWorld();
//	if (TheWorld)
//	{
//		TheWorld->GetTimerManager().ClearTimer(TestSessionTH);
//	}
//
//	// 娟绰 弊成 甘俊 冠酒初篮 单捞磐肺促
//	if (DummyRaidTargetMob && DummyRaidTargetMob->IsValidObj())
//	{
//		DummyRaidTargetMob->Destroy();
//		DummyRaidTargetMob = nullptr;
//	}
//
//	for (ABladeIITestDummyPlayer* ThisDummy : AllDummyPlayers)
//	{
//		if (ThisDummy && ThisDummy->IsValidObj())
//		{
//			ThisDummy->Destroy();
//		}
//	}
//	AllDummyPlayers.Empty();
//}
//
//#endif