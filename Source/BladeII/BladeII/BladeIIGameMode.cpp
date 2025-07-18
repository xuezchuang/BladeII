// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#include "BladeIIGameMode.h"

//#include "Runtime/Engine/Public/Engine.h"
//#if WITH_EDITOR
//#include "Editor.h"
//#endif
 #include "B2PCClassInfo.h"
//#include "Camera/CameraComponent.h"
//#include "Particles/ParticleSystem.h"
//#include "Components/AudioComponent.h"
//#include "BehaviorTree/BehaviorTree.h"
#include "BladeIIPlayerController.h"
#include "BladeIIPlayer.h"
//#include "BladeIINetPlayer.h"
#include "B2NPCClassInfo.h"
#include "B2NPCClassInfoBox.h"
#include "B2PCClassInfoBox.h"
#include "B2InGameHUDInfo_Player.h"
#include "B2ItemInfo.h"
#include "B2ChapterInfo.h"
#include "B2StageInfo.h"
#include "B2CommonSoundInfo.h"
#include "B2ScenarioDialogInfo.h"
#include "B2CharacterTagDialogueInfo.h"
#include "B2AutoAIController.h"
#include "B2SkillRTManager.h"
//#include "ContentStreaming.h"
#include "B2MonsterSpawnPool.h"
#include "B2StageEventDirector.h"
#include "B2TreasureChestBase.h"
#include "B2DestructibleLevelObjBase.h"
#include "B2AutoWayPoint.h"
#include "B2UIManager_InGameCombat.h"
#include "B2DropItem.h"
#include "B2ItemInfo.h"
#include "B2ActiveCameraActor.h"
#include "Event.h"
#include "B2UIManager.h"
#include "B2UIDocManager.h"
#include "B2UIDocHelper.h"
#include "B2StageManager.h"
#include "BladeIIUtil.h"
#include "Retailer.h"
//#include "MyAssetStore.h"
//#include "MoviePlayer.h"
#include "B2PreLoadingScreen.h"
#include "B2UIChatting.h"
#include "QuestManager.h"
#include "B2UIQuestDialog.h"
#include "B2WingInfo.h"
#include "B2SomeInfo.h"
#include "B2LoadingImageInfo.h"
#include "B2UIMain.h"
#include "B2UIQuestNotice.h"
#include "B2HomePoint.h"
#include "B2Airport.h"
//#include "GameDelegates.h"
#include "B2DamageEffectInfo.h"
#include "B2BuffModeEffectInfo.h"
#include "B2SkillInfo.h"
#include "B2UIHeader.h"
#include "B2UIBroadcastSystemNotice.h"
//#include "RHIDefinitions.h"
#include "Scalability.h"
#include "BladeIIScalabilityControl.h"
#include "TutorialManager.h"
#include "B2CompositeMeshCache.h"
#include "B2AssetLoader.h"
#include "B2GameLoadingProgress.h"
#include "B2BGMManager.h"
#include "B2AndroidBackManager.h"
#include "Global.h"
#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"
#include "B2EtherContainer.h"
#include "B2DialogTriggerActorBase.h"
#include "B2GuildMarkInfo.h"
#include "B2InfluenceSkillInfo.h"
#include "B2GuildNPCInfo.h"
#include "B2GuildBuffInfo.h"
#include "B2CombatConstantInfo.h"
#include "B2CombatStatEvaluator.h"
#include "BladeIIBlockToSyncNetwork.h"
#if !UE_BUILD_SHIPPING
#include "Sound/SoundNodeWavePlayer.h"
#endif
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidMisc.h"
extern void AndroidThunkCpp_StageMoviePlay(const FString& MovieName);
#endif
#include "B2BTTask_PCAutoAttack.h"
//#include "Matinee/InterpTrack.h"
#include "B2GameInstance.h"
//
////#include "MissionManager.h"
////#include "InternalEvents.h"
#include "B2UITimerNotice.h"
#include "B2NetworkBridge.h"

#include "B2CodeTable.h"
#include "B2UIHallOfFame.h"

#include "Engine/PawnIterator.h"
#include "../../../../down/blade_dev_client/BladeII/Source/BladeII/Public/BladeIIUtil.h"

#if BII_STATS
bool FStatisticalPerfStatCollector::bDoStatCollecting = false; // 메인 작동 플래그. 바꾸게 되면 다음 레벨에서 적용.
bool FStatisticalPerfStatCollector::bSkipEventCinematics = false; // 일반적인 전투 장면만 측정하고 싶은 경우 true 로. 연출과 전투에서 사용하는 LOD 가 다르기도 하고..
bool FStatisticalPerfStatCollector::bIsCurrentlyInEventScene = false;
float FStatisticalPerfStatCollector::SkipFrameTimeLimit = 1000.0f; // 특정 한도 이상의 큰 FrameTime 이 감지된 건 뭔가 로딩 시간이 들어갔거나 한 걸로 간주하고 스킵
FStatisticalPerfStatCollector::FStatisticalPerfStatCollector()
	: bInitialized(false), bDataAvailableForDisplay(false) 
	, Result_GameThreadTime_Avg(0.0f), Result_RenderThreadTime_Avg(0.0f), Result_GPUTime_Avg(0.0f), Result_FrameTime_Avg(0.0f), Result_DrawCall_Avg(0), Result_PolyCount_Avg(0)
	, Result_GameThreadTime_SD(0.0f), Result_RenderThreadTime_SD(0.0f), Result_GPUTime_SD(0.0f), Result_FrameTime_SD(0.0f), Result_DrawCall_SD(0.0f), Result_PolyCount_SD(0.0f)
	, Result_GameThreadTime_Upper10(0.0f), Result_RenderThreadTime_Upper10(0.0f), Result_GPUTime_Upper10(0.0f), Result_FrameTime_Upper10(0.0f), Result_DrawCall_Upper10(0), Result_PolyCount_Upper10(0)
	, SkippedFrameNum(0), SkippedFrameTimeAvg(0.0)
{
}

void FStatisticalPerfStatCollector::LoadSettings()
{
	if (GConfig)
	{
		GConfig->GetBool(TEXT("StatisticalPerfStatCollector"), TEXT("DoStatCollecting"), bDoStatCollecting, GGameIni);
		GConfig->GetBool(TEXT("StatisticalPerfStatCollector"), TEXT("SkipEventCinematics"), bSkipEventCinematics, GGameIni);
		GConfig->GetFloat(TEXT("StatisticalPerfStatCollector"), TEXT("SkipFrameTimeLimit"), SkipFrameTimeLimit, GGameIni);
	}
}

void FStatisticalPerfStatCollector::OnBegin(ABladeIIGameMode* InB2GM)
{
	if (!bDoStatCollecting || !InB2GM) {
		return;
	}
	
	GameThreadTimes.Reset();
	RenderThreadTimes.Reset();
	GPUTimes.Reset();
	FrameTimes.Reset();
	DrawCalls.Reset();
	PolyCounts.Reset();

	bInitialized = true;
	bDataAvailableForDisplay = false;
	SkippedFrameNum = 0;
	SkippedFrameTimeAvg = 0.0;
}
void FStatisticalPerfStatCollector::OnTick(ABladeIIGameMode* InB2GM, float InDeltaTime)
{
	//if (!bDoStatCollecting || !InB2GM || !bInitialized) {
	//	return;
	//}

	//if (bDataAvailableForDisplay)
	//{ // 수집한 값들을 표시
	//	DisplayResultStats(InB2GM);
	//}
	//else
	//{ // 아직 데이터 수집 중		
	//	if (!bSkipEventCinematics || !bIsCurrentlyInEventScene)
	//	{
	//		double FrameDiffTime = FApp::GetCurrentTime() - FApp::GetLastTime();
	//		double FrameDiffTimeMs = FrameDiffTime * 1000.0;
	//		if (FrameDiffTimeMs >= (double)SkipFrameTimeLimit) // 너무 큰 프레임타임은 로딩 직후 첫 프레임일 수 있다..
	//		{
	//			++SkippedFrameNum;
	//			SkippedFrameTimeAvg += FrameDiffTimeMs; // 참고용으로 일단 더해놓고 최후에 평균 냄.
	//		}
	//		else
	//		{
	//			// FStatUnitData 에서.. 그쪽서는 기존 수치들에 0.9 가중치로 블렌딩을 해서 즉석 LPF 를 거는데 여기선 일단 모은 후 통계를 낸다.
	//			if (GGameThreadTime > 0) {
	//				GameThreadTimes.AddValue(FPlatformTime::ToMilliseconds(GGameThreadTime));
	//			}
	//			if (GRenderThreadTime > 0) {
	//				RenderThreadTimes.AddValue(FPlatformTime::ToMilliseconds(GRenderThreadTime));
	//			}
	//			uint32 GPUFrameCycle = RHIGetGPUFrameCycles();
	//			if (GPUFrameCycle > 0) {
	//				GPUTimes.AddValue(FPlatformTime::ToMilliseconds(GPUFrameCycle));
	//			}
	//			if (FrameDiffTime > 0.0) {
	//				FrameTimes.AddValue((float)(FrameDiffTimeMs));
	//			}
	//			if (GCachedLastDrawCalls > 0) {
	//				DrawCalls.AddValue(GCachedLastDrawCalls);
	//			}
	//			if (GCachedLastPrimitivesDrawn > 0) {
	//				PolyCounts.AddValue(GCachedLastPrimitivesDrawn);
	//			}
	//		}
	//	}
	//}
}
void FStatisticalPerfStatCollector::OnEnd(ABladeIIGameMode* InB2GM)
{
	if (!bDoStatCollecting || !InB2GM || !bInitialized || bDataAvailableForDisplay) {
		return;
	}

	// 그냥 평균
	//Result_GameThreadTime_Avg = GameThreadTimes.GetTotalAverage();
	//Result_RenderThreadTime_Avg = RenderThreadTimes.GetTotalAverage();
	//Result_GPUTime_Avg = GPUTimes.GetTotalAverage();
	//Result_FrameTime_Avg = FrameTimes.GetTotalAverage();
	//Result_DrawCall_Avg = DrawCalls.GetTotalAverage();
	//Result_PolyCount_Avg = PolyCounts.GetTotalAverage();

	// 표준편차
	Result_GameThreadTime_SD = GameThreadTimes.GetStandardDeviation();
	Result_RenderThreadTime_SD = RenderThreadTimes.GetStandardDeviation();
	Result_GPUTime_SD = GPUTimes.GetStandardDeviation();
	Result_FrameTime_SD = FrameTimes.GetStandardDeviation();
	Result_DrawCall_SD = DrawCalls.GetStandardDeviation();
	Result_PolyCount_SD = PolyCounts.GetStandardDeviation();

	// 상위 10% 평균
	//Result_GameThreadTime_Upper10 = GameThreadTimes.GetRangedAverage(true, 0.1f);
	//Result_RenderThreadTime_Upper10 = RenderThreadTimes.GetRangedAverage(true, 0.1f);
	//Result_GPUTime_Upper10 = GPUTimes.GetRangedAverage(true, 0.1f);
	//Result_FrameTime_Upper10 = FrameTimes.GetRangedAverage(true, 0.1f);
	//Result_DrawCall_Upper10 = DrawCalls.GetRangedAverage(true, 0.1f);
	//Result_PolyCount_Upper10 = PolyCounts.GetRangedAverage(true, 0.1f);

	if (SkippedFrameNum > 0 && SkippedFrameTimeAvg > 0.0)
	{ // 참고용으로 이것도 따로 평균.
		SkippedFrameTimeAvg /= (double)SkippedFrameNum;
	}

	bDataAvailableForDisplay = true;
}

void FStatisticalPerfStatCollector::DisplayResultStats(ABladeIIGameMode* InB2GM)
{
#if WITH_BII_ON_SCREEN_DEBUG_TEXT // Shipping 버전에서 PerfStatCollector 를 쓰게 되면 WITH_BII_ON_SCREEN_DEBUG_TEXT 도 같이..
	int32 FontSize = 18;
	int32 LineDist = 12;

	float DrawX = 10.0f;
	float DrawY = 10.0f;
	
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[Avg] Game T. : %.2f ms (%d)"), Result_GameThreadTime_Avg, GameThreadTimes.GetRecordedNum()), FLinearColor::Green, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[Avg] Render T. : %.2f ms (%d)"), Result_RenderThreadTime_Avg, RenderThreadTimes.GetRecordedNum()), FLinearColor::Green, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[Avg] GPU T. : %.2f ms (%d)"), Result_GPUTime_Avg, GPUTimes.GetRecordedNum()), FLinearColor::Green, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[Avg] Frame T. : %.2f ms (%d)"), Result_FrameTime_Avg, FrameTimes.GetRecordedNum()), FLinearColor::Green, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[Avg] DrawCall : %d (%d)"), Result_DrawCall_Avg, DrawCalls.GetRecordedNum()), FLinearColor::Green, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[Avg] Polys : %d (%d)"), Result_PolyCount_Avg, PolyCounts.GetRecordedNum()), FLinearColor::Green, FontSize);
	
	DrawX = 400.0f;
	DrawY = 10.0f;
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[StdDev] GameThread : %.4f"), Result_GameThreadTime_SD), FLinearColor::Yellow, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[StdDev] RenderThread : %.4f"), Result_RenderThreadTime_SD), FLinearColor::Yellow, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[StdDev] GPUTime : %.4f"), Result_GPUTime_SD), FLinearColor::Yellow, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[StdDev] FrameTime : %.4f"), Result_FrameTime_SD), FLinearColor::Yellow, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[StdDev] DrawCall : %.4f"), Result_DrawCall_SD), FLinearColor::Yellow, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[StdDev] Polygons : %.4f"), Result_PolyCount_SD), FLinearColor::Yellow, FontSize);

	DrawX = 800.0f;
	DrawY = 10.0f;
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[10%%] GameThread : %.2f ms"), Result_GameThreadTime_Upper10), FLinearColor::Red, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[10%%] RenderThread : %.2f ms"), Result_RenderThreadTime_Upper10), FLinearColor::Red, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[10%%] GPUTime : %.2f ms"), Result_GPUTime_Upper10), FLinearColor::Red, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[10%%] FrameTime : %.2f ms"), Result_FrameTime_Upper10), FLinearColor::Red, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[10%%] DrawCall : %d"), Result_DrawCall_Upper10), FLinearColor::Red, FontSize);
	DrawY += (float)(FontSize + LineDist);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[10%%] Polygons : %d"), Result_PolyCount_Upper10), FLinearColor::Red, FontSize);

	// 그 아래에 기타 추가 참고 정보. 바로 스샷 찍어서 기록하기 좋게
	DrawX = 10.0f;

	DrawY += (float)((FontSize + LineDist) * 1);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("Skipped Frames : %.2f ms (%d)"), SkippedFrameTimeAvg, SkippedFrameNum), FLinearColor::Green, FontSize);

	DrawY += (float)( (FontSize + LineDist) * 2 );

	UWorld* TheWorld = InB2GM ? InB2GM->GetWorld() : nullptr;
	ERHIFeatureLevel::Type RHIFeatureLevel = (TheWorld && TheWorld->Scene) ? TheWorld->Scene->GetFeatureLevel() : ERHIFeatureLevel::Num;
	// UEnum 이 아니라서 당장 문자열 변경이 안됨 ㅡㅡ;
	EShaderPlatform ShaderPlatform = GetFeatureLevelShaderPlatform(RHIFeatureLevel);
	
	FString PlatformAPIStr = FString::Printf(TEXT("Unknown.. ShaderPlatform %d"), (int32)ShaderPlatform);
#if PLATFORM_WINDOWS
	if (RHIFeatureLevel == ERHIFeatureLevel::SM5) {
		PlatformAPIStr = TEXT("PC SM5");
	}
	else if (RHIFeatureLevel == ERHIFeatureLevel::SM4) {
		PlatformAPIStr = TEXT("PC SM4");
	}
	else if (RHIFeatureLevel == ERHIFeatureLevel::ES3_1) {
		PlatformAPIStr = TEXT("PC ES31");
	}
	else if (RHIFeatureLevel == ERHIFeatureLevel::ES2) {
		PlatformAPIStr = TEXT("PC ES2");
	}
	else {
		PlatformAPIStr = TEXT("PC");
	}
#elif PLATFORM_ANDROID
	if (FAndroidMisc::ShouldUseVulkan())
	{
		PlatformAPIStr = TEXT("Android Vulkan");
	}
	else
	{
		PlatformAPIStr = FString::Printf(TEXT("Android %s"), (RHIFeatureLevel == ERHIFeatureLevel::ES2) ? TEXT("ES2") : (RHIFeatureLevel == ERHIFeatureLevel::ES3_1 ? TEXT("ES31") : TEXT("Unknown")));
	}		
#elif PLATFORM_IOS
	if (ShaderPlatform == EShaderPlatform::SP_METAL || ShaderPlatform == EShaderPlatform::SP_METAL_MRT){
		PlatformAPIStr = TEXT("iOS Metal"); // Metal 구분 이거 맞나 몰라.
	}
	else{
		PlatformAPIStr = TEXT("iOS");
	}
#endif

	// 결과에 영향을 미칠 수 있는 구동 환경 및 설정들을 표시.
	{
		Scalability::FQualityLevels CurrScalabilityLevel = Scalability::GetQualityLevels();
		int32 TotalAdded =
			CurrScalabilityLevel.AntiAliasingQuality +
			CurrScalabilityLevel.EffectsQuality +
			CurrScalabilityLevel.PostProcessQuality +
			//InScalabilityLevel.ResolutionQuality + 이건 수치가 좀 다르니 빼야..
			CurrScalabilityLevel.TextureQuality +
			CurrScalabilityLevel.ShadowQuality +
			CurrScalabilityLevel.ViewDistanceQuality +
			CurrScalabilityLevel.GameplayOnlyLODQuality;
		// 혹여나 추가될 Scalability 항목에 대한 처리가 필요하긴 한데 그렇게 정교하게까지 할 필요는 없을 듯.
		float AvergageScalabilityValue = (float)TotalAdded / 7.0f;

		UPackage* WorldOuterMost = TheWorld ? TheWorld->GetOutermost() : nullptr;
		FString WorldLevelName = WorldOuterMost ? WorldOuterMost->GetName() : TEXT("Unknown");

		DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[Platform] %s,  [AvgScaleLevel] %.2f,  [PlayMap] %s"), *PlatformAPIStr, AvergageScalabilityValue, *WorldLevelName), FLinearColor::Green, FontSize);
	}
	{
		//DrawY += (float)(FontSize + LineDist);
		//int32 SelectedResLevel = 0;
		//LoadGameSetting_Resolution_OrByDefault(SelectedResLevel);
		//const EB2GraphicsRQType CurrRQType = B2Scalability::GetSafeMappedRQTypeOfResolutionLevel(IntToB2ResolutionLevel(SelectedResLevel));
		//FString CurrRQTypeStr;
		//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EB2GraphicsRQType"), true);
		//if (EnumPtr)
		//{
		//	CurrRQTypeStr = EnumPtr->GetNameStringByValue((int64)CurrRQType);
		//}
		//DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[MobileFrameUpperLimit] %.2f,  [RQType] %s"), GEngine->GetCurrentMobileSmoothFrameRange().GetUpperBoundValue(), *CurrRQTypeStr), FLinearColor::Green, FontSize);
	}
	{
		DrawY += (float)(FontSize + LineDist);
		IConsoleVariable* CamDistAlphaCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.B2CameraDistanceAlpha"));
		extern float GDevTestCameraDistanceScale;
		DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[CamDistAlpha] %.2f,  [TestCamDistScale] %.2f"), CamDistAlphaCVar ? CamDistAlphaCVar->GetFloat() : -1.0f, GDevTestCameraDistanceScale), FLinearColor::Green, FontSize);
	}
	{
		DrawY += (float)(FontSize + LineDist);
		DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("[SkipEvent] %d,  [AutoComboOnly] %d"), (int32)bSkipEventCinematics, (int32)UB2BTTask_PCAutoAttack::bDevAllowComboAttackOnly), FLinearColor::Green, FontSize);
	}

	if (InB2GM)
	{ // 추가로 하위 게임모드 성격에 따라 표시할 거 있음.
		InB2GM->DrawAdditionalPerfStatInfo(DrawX, DrawY + (float)(FontSize + LineDist));
	}
#endif
}
#endif

#ifdef BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
// 로딩 시간 측정 용. 
// 로딩 시간이야 그냥 보면 나오는 거 아니냐고 할 수 있지만 이런저런 기능 테스트해 가면서 보려면 1~2 초 단축되는 거 체크할 필요가 있어서 넣음.
bool gDisplayLoadingTimeTaken = false;
static bool gLoadTimeCheckBeginMark = false;
static double gLastLoadStartTime = 0.0;
void DevCheckLoadingTimeBegin()
{
	if (gDisplayLoadingTimeTaken && !gLoadTimeCheckBeginMark)
	{
		gLoadTimeCheckBeginMark = true;
		gLastLoadStartTime = FPlatformTime::Seconds();
	}
}
void DevCheckLoadingTimeEnd()
{
	if (gDisplayLoadingTimeTaken && gLoadTimeCheckBeginMark)
	{
		// 게임 코드 상으로는 적절한 위치에서 로딩 시간 체크를 한다고 해도 렌더쓰레드와의 타이밍으로 인해 실제 체감 로딩시간은 다를 수 있다.
		//FlushRenderingCommands(); // 근데 이렇게 해도 소용이 없단 건 함정. 결국 이건 참고용으로만 쓰고 실제 로딩 시간 측정은 직접 스톱워치로...
		gLoadTimeCheckBeginMark = false;
		double LoadingTimeTaken = FPlatformTime::Seconds() - gLastLoadStartTime;
		UE_LOG(LogBladeII, Log, TEXT("[DevCheckLoadingTime] LastLoadingTime %f"), LoadingTimeTaken);

		// 이걸 렌더링 할 수 있는 시점이려면 UIManager 가 초기화 된 상태여야 함.
		BII_SCREEN_LOG(FString::Printf(TEXT("[DevCheckLoadingTime] LastLoadingTime %f"), LoadingTimeTaken),
			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 10.0f);
	}
}
#endif


ABladeIIGameMode::ABladeIIGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
//	// use our custom PlayerController class
//	//PlayerControllerClass = ABladeIIPlayerController::StaticClass();
//	//DefaultPawnClass = ABladeIIPlayer::StaticClass();
//
//	//// It will override DefaultPawnClass if being set to other than EPC_End.
//	//DefaultPCClassEnum = EPCClass::EPC_Assassin;
//	//CurrentPCClassEnum = EPCClass::EPC_End;
//
//	//LoadingImageIdx = INDEX_NONE;
//	//bLoadingImageUnloadedAndClear = false;
//	//bUseCustomWaitForLoadingScreen = false;
//	//
//	//if (!HasAnyFlags(RF_ClassDefaultObject))
//	//{
//	//	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ABladeIIGameMode::OnPreLoadMap);
//	//}	
//	//	
//	//CurrentActiveActiveCameraActor = NULL;
//	//TheActiveSpawnPool = NULL;
//
//	//bGlobalSceneBlurOn = false;
//	//CachedCameraAtBlurBegin = NULL;
//
//	//bEventsSubscribed = false;
//
//	//BGMManager = NULL;
//	//CurrentPlayingAMB = NULL;
//
//	//if (!HasAnyFlags(RF_ClassDefaultObject))
//	//{// 정식 게임에서 이 값이 처음에 true 여야 해서 기본값 true 로 세팅하다 보니 DLCFrontMap 을 거치지 않는 개발용 테스트를 위해 여기 들어오면 걍 바꿔줌.
//	//	GMinimalDLCFrontMode = false; // 그리고 ini 에 이걸 true 로 해서 시작했다 하더라도 false 로 바뀌어서 에디터 같은 경우 원래 의도와 다른 동작을 할 수 있을 테니 조심조심
//	//}
//	//	
//	//if (!GMinimalDLCFrontMode){ // DLC Front 모드 리소스로딩 최대한 제거
//	//	FString PreLoadingScreenWidgetClassPath;
//	//	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("PreLoadingScreenWidgetClass"), PreLoadingScreenWidgetClassPath, GGameIni);
//	//	static ConstructorHelpers::FClassFinder<UB2PreLoadingScreen> PreLoadingScreenClassFoundSet(*PreLoadingScreenWidgetClassPath);
//	//	PreLoadingScreenWidgetClass = PreLoadingScreenClassFoundSet.Class;
//	//	// 기본값 둘 다 똑같음..
//	//	PreLoadingScreenWidgetClass_PreRender = PreLoadingScreenClassFoundSet.Class;
//	//}
//
//	//CreatedPreLoadingScreen = NULL;
//	//CreatedPreLoadingScreen_PreRender = NULL;
//
//	//bAnyStageEventPlayedOnce = false;
//	//PlayerAutoBattleType = AutoBattleType::NONE;
//	//bDeferAutoBeginAuto = false;
//
//	//LastRequestedStageBuffType = EResurrectBuffType::RBT_None;
//
//	//PreRenderPhase = EPreRenderPhase::EPRP_None;
//	//PreRenderer = nullptr;
//
//	//CachedPreLoadingGameMode = EB2GameMode::Unknown;
//
//	//bBuffTick = true;
}

bool ABladeIIGameMode::NeedToRecordDamageLog(const float ActualDamage, class ABladeIICharacter* Attacker)
{
	//const bool IsMoreDamage = ActualDamage > DamageLogInfo.ActualDamage;
	//auto* AttackPlayer = Cast<ABladeIIPlayer>(Attacker);

	//if (IsMoreDamage && AttackPlayer)
	//	return AttackPlayer->IsLocalPlayer();

	return false;
}

void ABladeIIGameMode::RecordDamageLog(const float ActualDamage, class ABladeIICharacter* Victim, class ABladeIICharacter* Attacker)
{
	//if(NeedToRecordDamageLog(ActualDamage, Attacker))
	//{
	//	// 일반적인 Local실행 모드 (Stage, 영웅의 탑, 반격던전, 팀대전)은 Attacker가 Local일 때 기록

	//	DamageLogInfo.ActualDamage = ActualDamage;
	//	DamageLogInfo.AttackerAccountID = Cast<ABladeIIPlayer>(Attacker)->GetAccountId();
	//}
}

void ABladeIIGameMode::SendDamageLog()
{
	if (DamageLogInfo.ActualDamage > 0.f && DamageLogInfo.AttackerAccountID != 0)
	{
		//B2ActionLogSender Sender;
		//Sender.setCategory(B2ActionLogType<EB2GameMode>::GetName(GetB2GameModeType()))
		//	.setAction(TEXT("MaxTakeDamage"))
		//	.setLabel(FString::SanitizeFloat(DamageLogInfo.ActualDamage))
		//	.setActionAttr1(FString::FromInt(DamageLogInfo.AttackerAccountID))
		//	.LogSender(eLogSendToFlag::LOG_SENDTO_KOREA_KAKAOBIS);
	}

}

///////////////////////////////////////////////////////////////

TArray<EPCClass> ABladeIIGameMode::GetPCClassesToPreLoad()
{
	TArray<EPCClass> RetArray;
	// 하위 클래스, 특히 StageGameMode 에서 필요한 애들만 넣어서 리턴하도록 하기 위함..
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}
	return RetArray;
}
TArray<EPCClass> ABladeIIGameMode::GetPCClassesToMatineePreLoad()
{
	return GetPCClassesToPreLoad(); // 일부 게임모드에서 이게 아닌 경우가 있을 것.
}
TArray<FCombinedPCSkillAnimID> ABladeIIGameMode::GetPCSkillAnimsToPreLoad()
{
	TArray<FCombinedPCSkillAnimID> RetArray;

	//TArray<EPCClass> AllPCClassToPreLoad = GetPCClassesToPreLoad();

	//for (EPCClass ThisPCClass : AllPCClassToPreLoad)
	//{
	//	// 기본 구현은 매 PCClass 별로 가능한 스킬 애니메이션들을 몽땅 다 넣음. 
	//	// 하위 GameMode 클래스에서는 각 플레이어 캐릭터 별로 장착한 Skill 상태에 따라 필요한 것만 넣을 수 있도록 해야 함.
	//	// 이를 위해서는 CharacterDataStore 가 필요하므로 네트워크 게임이라면 캐릭터 별로 iteration 을 돌아서 장착한 skill 들을 모아와야지, PCClass 로 iteration 하면 안될 것임.
	//	// 그러나 일부 네트워크 기반 모드에서는 실제 매칭이 되기 전에는 CharacterDataStore 가 없을 것이므로 Preload 시점에 불가능할 수도 있다.

	//	TArray<FCombinedPCSkillAnimID> AllAnimIDOfThisClass = GetAllPCSkillAnimsOfClass(ThisPCClass);
	//	RetArray.Append(AllAnimIDOfThisClass);
	//}
	return RetArray;
}

TArray<FCombinedPCSkillAnimID> ABladeIIGameMode::GetAllPCSkillAnimsOfClass(EPCClass InCharClass)
{ // 유틸 수준. 꼭 여기에 있을 필요는 없다.
	TArray<FCombinedPCSkillAnimID> RetArray;
	for (int32 SAI = 0; SAI < static_cast<int32>(ESkillAnimType::ESA_End); ++SAI)
	{
		ESkillAnimType ThisAnimType = static_cast<ESkillAnimType>(SAI);
		if (ThisAnimType != ESkillAnimType::ESA_Weapon_Normal_End) // 이건 중간에 뭐 식별한답시고 넣은 거니 빼고
		{
			RetArray.Add(FCombinedPCSkillAnimID(InCharClass, ThisAnimType));
		}
	}
	return RetArray;
}

// 기본적으로 Main / Sub 두개를 로드하고 GameMode별로 관리한다. ex ) 팀대전 - 셋, 레이드, 점령전 - 하나
void ABladeIIGameMode::TryAsyncLoadSkillAnims(ICharacterDataStore* DataStore)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::TryAsyncLoadSkillAnims"));

	if (DataStore != nullptr)
	{
		TArray<FCombinedPCSkillAnimID> SkillAnimIDs;
		GatherEquippedSkillAnimIDs(DataStore, SkillAnimIDs);

		if (auto* AnimInfo = StaticFindSkillAnimInfo(this))
		{
			const FString RequestName = FString::Printf(TEXT("%s SkillAnims"), *DataStore->GetUserNickName());
			TArray<FB2AsyncRequestInfo> RequestAsyncLoadList = AnimInfo->GetAsyncRequestAssetList(SkillAnimIDs);

			AnimInfo->TryAsyncLoad(RequestName, RequestAsyncLoadList);
		}
	}
}

 //GatherEquippedSkillAnimIDs 함수만 재정의 하여 게임 모드별 필요 Class SkillAnim들을 AsyncLoad
void ABladeIIGameMode::GatherEquippedSkillAnimIDs(ICharacterDataStore* DataStore, TArray<FCombinedPCSkillAnimID>& OutEquippedSkills)
{
	//OutEquippedSkills.Append(GetEquippedSkillAnimIDs(DataStore->GetMainPlayerClass(), DataStore));
	//OutEquippedSkills.Append(GetEquippedSkillAnimIDs(DataStore->GetSubPlayerClass(), DataStore));
}

TArray<FCombinedPCSkillAnimID> ABladeIIGameMode::GetEquippedSkillAnimIDs(EPCClass PlayerClass, ICharacterDataStore* DataStore)
{
	TArray<ESkillAnimType> SkillAnimTypes;
	UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(this, PlayerClass, GetB2GameModeType(), SkillAnimTypes, DataStore);

	TArray<FCombinedPCSkillAnimID> SkillAnimIDs;
	for (auto SkillAnimType : SkillAnimTypes)
	{
		if (SkillAnimType != ESkillAnimType::ESA_End)
			SkillAnimIDs.AddUnique(FCombinedPCSkillAnimID(PlayerClass, SkillAnimType));
	}

	return SkillAnimIDs;
}

void ABladeIIGameMode::PreloadAnyNecessaryInfo(bool bAboutToPlayLoadingMovie)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::PreloadAnyNecessaryInfo"));

	//// TAsset 로딩하는 InfoAsset 들이 대상임. 반대는 ManualUnloadBeforeNewBeginning
	//
	//UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	//if (PCBox)
	//{
	//	B2_SCOPED_TIME_LOG(TEXT("PreloadAnyNecessaryInfo > PCClassInfo"));
	//	// 이건 별로 시간 안 먹을 것이고 PCClassInfo 중에서 따로 떼어낸 InGameOnlyInfo 가 많이 먹을 꺼. 그건 Async 로딩 대상이라 이거 이후에 함.
	//	TArray<EPCClass> ClassesToLoad = GetPCClassesToPreLoad();
	//	PCBox->PreloadClassAssets(ClassesToLoad);
	//}
	//
	//{
	//	// SpawnPool 에서 spawn 할 수 있는 모든 NPC class 들 preload 하는 건 아니고 여기선 게임 시작 이후 Async 대상 제외하고.
	//	B2_SCOPED_TIME_LOG(TEXT("PreloadAnyNecessaryInfo > MobClassInfo"));
	//	if (TheActiveSpawnPool)
	//		TheActiveSpawnPool->PreloadPoolMonsters();
	//}

	//B2GMLoadingProgCollector::StepCustom(bAboutToPlayLoadingMovie ? 0.4f : 0.1f);
}

void ABladeIIGameMode::PreloadInGameInfoAsset_SynchronousOnly(bool bAboutToPlayLoadingMovie)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::PreloadInGameInfoAsset_SynchronousOnly"));

	// 요 Synchronous 로딩이 FlushAsyncLoading 을 유발한다면 다른 async 요청 이전에 해야 하고,
	// 그게 아니라면 이걸 오히려 다른 async 요청 이후에 함으로서 다중 쓰레드로 로딩이 돌아가도록 할 수 있.. 을까?
		
	// TAsset 으로 바꾼 Matinee 레퍼런스 에셋 로딩.. 이건 원래 맵 로딩의 일부였던 것이다. 가능한 처음에 위치시킴.
	{
		int32 LazyLoadHandledNum = B2GamePreloadMatineeRefAssets();
		B2GMLoadingProgCollector::StepCustom((float)LazyLoadHandledNum * (bAboutToPlayLoadingMovie ? 0.02f : 0.01f));
	}

	{ // DamageEffectInfo 와 BuffModeEffectInfo 로딩에 의한 진행도 산정시 실제 로딩 필요가 있는지에 따라 다르게..
		if (!UB2DamageEffectInfo::AssetsAreRooted() && !UB2BuffModeEffectInfo::AssetsAreRooted())
		{ // 둘 다 로딩하게 되는 경우
			B2GMLoadingProgCollector::SetStepRate(bAboutToPlayLoadingMovie ? 0.4f : 0.1f);
		}
		else if (UB2DamageEffectInfo::AssetsAreRooted() && UB2BuffModeEffectInfo::AssetsAreRooted())
		{
			B2GMLoadingProgCollector::SetStepRate(0.0f);
		}
		else
		{ // 둘 중 하나만 로딩하게 되는 경우
			B2GMLoadingProgCollector::SetStepRate(bAboutToPlayLoadingMovie ? 0.2f : 0.07f);
		}
	}
	UB2DamageEffectInfo* DFXInfo = StaticFindDamageEffectInfo(this);
	if (DFXInfo)
	{ // 전투에서 자잘한 spike 요인이 될 수 있음.
		DFXInfo->PreloadAndCacheAllTAsset();
	}
	UB2BuffModeEffectInfo* BuffInfo = StaticFindBuffModeEffectInfo(this);
	if (BuffInfo)
	{
		BuffInfo->PreloadAndCacheAllTAsset();
	}
	B2GMLoadingProgCollector::Step(); // 앞서 산정한 StepRate 로 진행도 증가.

	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (PCBox)
	{ // 메인 캐릭터 데이터는 sync 로딩으로 처리함. RootSet 이면 그냥 넘어가게 될 꺼고.
		UB2PCClassInfo* ThisLoadedInfo = PCBox->GetSingleClassInfo(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
		if (ThisLoadedInfo)
		{
			// 루트셋 처리 여부와 관계없이 이 시점에서 루트셋이 아니면 로딩이 안되어 있어야 함.
			// 최악의 경우 다른 PCClass InGameOnly 데이터도 다 로딩하는 경우도 있겠지만.. 여기선 일단 확실한 곳이니 많은 Step 지분 배당.
			B2GMLoadingProgCollector::SetStepRate(
				ThisLoadedInfo->IsInGameOnlyInfoAtRootSet() ? 0.0f : 
				 (bAboutToPlayLoadingMovie ? 0.6f : 0.4f)
			);

			ThisLoadedInfo->GetInGameOnlyInfo(this);

			B2GMLoadingProgCollector::Step();
		}
	}
}

void ABladeIIGameMode::PreloadInGameInfoAsset_AsyncAllowed(bool bAboutToPlayLoadingMovie)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::PreloadInGameInfoAsset_AsyncAllowed"));
	B2_SCOPED_TIME_LOG(TEXT("ABladeIIGameMode::PreloadInGameInfoAsset_AsyncAllowed"));

	// 로비를 제외한 게임모드들에서는 실질적으로 메모리와 로딩타임을 소모하는 InGameOnlyInfo 를 로딩해 놓을 필요가 있다.
	// 게임모드 별 필요 및 구조에 따라 가능한 부분은 비동기 로딩으로 처리를 하도록 한다.
	// 여기서 바로 로딩을 하던 Async 요청만 하던 큰 덩어리에 대한 로딩이라 궁극적으로 많은 메모리가 올라감.
	// 단, RootSet 처리를 한 거라면 이미 메모리에 올라와 있을 테니 아무 변화 없겠지.

	// 여기서 StaticFindPCClassInfoBox 를 콜하는 것이 Async flush 를 일으킨다면 단순히 이전에 한번 콜해 놓으면 된다.
	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (PCBox)
	{
		TArray<EPCClass> ClassesToLoad = GetPCClassesToPreLoad();

#if !UE_BUILD_SHIPPING // PC 클래스 로딩의 영향 상세 파악
		extern int32 gPCDataPreLoadMaxNum;
		if (gPCDataPreLoadMaxNum >= 0)
		{
			if (gPCDataPreLoadMaxNum == 0)
			{
				ClassesToLoad.Empty();
			}
			else if (gPCDataPreLoadMaxNum == 1) // 하나만이면 처음 spawn 할 애만
			{
				ClassesToLoad.Empty();
				ClassesToLoad.Add(CurrentPCClassEnum);
			}
			else
			{
				if (ClassesToLoad.Num() > gPCDataPreLoadMaxNum)
				{
					ClassesToLoad.RemoveAt(gPCDataPreLoadMaxNum, ClassesToLoad.Num() - gPCDataPreLoadMaxNum);
					if (!ClassesToLoad.Find(CurrentPCClassEnum)) { // 여하간 처음 spawn 할 클래스는 pre-load 를 해야 테스트가 정확해 질 것.
						ClassesToLoad.RemoveAt(ClassesToLoad.Num() - 1);
						ClassesToLoad.Add(CurrentPCClassEnum);
					}
				}
			}
		}
#endif

		for (EPCClass ThisClassLoad : ClassesToLoad)
		{
			// 메인 캐릭터 클래스는 루트셋 혹은 로딩이 되더라도 PreloadInGameInfoAsset_SynchronousOnly 에서 로딩된 상태.. 
			// 거나 아니면 이 요청 직후에 Synchronous 로딩을 해서 다중으로 돌아가는 로딩의 이득을 얻을 수 있으면 좋겠지.
			UB2PCClassInfo* ThisLoadedInfo = PCBox->GetSingleClassInfo(ThisClassLoad);
			if (ThisLoadedInfo)
			{
				ThisLoadedInfo->ReqAsyncLoad(this);
			}
		}
	}

	PreloadLocalSkillAnimation();
}

void ABladeIIGameMode::PreloadLocalSkillAnimation()
{
	FLocalCharacterData& LocalCharacterData = BladeIIGameImpl::GetLocalCharacterData();
	TryAsyncLoadSkillAnims(&LocalCharacterData);
}

void ABladeIIGameMode::PreloadMatineeRefAssetsDefault()
{
	// 의도적으로 비워짐.

	// 여기서 원래 하는 GameMode 쪽 기본 기능은 제거하고
	// 필요한 정보들이 확실히 채워진 후에 StageEventDirector 단위로 필요한 것들을 걸러내면서 
	// AB2StageEventDirector::LoadLazyLoadedMatineeRefAssets 를 사용해서 pre-load 함. (B2GamePreloadMatineeRefAssets)
}
int32 ABladeIIGameMode::B2GamePreloadMatineeRefAssets()
{
	int32 LazyLoadedSEDNum = 0;
//#if _MATINEE_REF_ASSET_LAZYLOAD // 게임 코드 쪽에선 여기에만 위치시키면 관련 기능 컨트롤이 가능.
//	TArray<FString> StageEventRefLoadExcludeFilters;
//
//#if 0 // 일부 트랙을 제외시켜도 나중에 연출 초기화되면서 결국 로딩. 그리고 이걸로 제외되는 양은 그리 많지 않은 듯. 일단 여긴 보류.
//	TArray<EPCClass> PCClassesToLoad = GetPCClassesToMatineePreLoad();
//	if (PCClassesToLoad.Num() > 0)
//	{
//		// Preload 대상 PCClass 를 Exclude 에서 제외
//		// 리소스 셋업에도 상당히 의존하는 거라 이게 100% 정확하지 않을 수도 있는데
//		// 그런 식으로 의도치 않게 걸러져서 여기서 로딩이 안된다고 해도 큰일 나는 건 아니다. 필요할 땐 어차피 로딩이 된다. 
//		ABladeIIPlayer::GetAllTrimmedNameFromPCClassEnum(StageEventRefLoadExcludeFilters);
//		for (EPCClass ThisPCClass : PCClassesToLoad)
//		{
//			StageEventRefLoadExcludeFilters.Remove(UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(ThisPCClass));
//		}
//	}
//#if !UE_BUILD_SHIPPING
//	FString LogMsg = TEXT("Matinee ref lazy load exclude filter:");
//	for (FString& ThisFilter : StageEventRefLoadExcludeFilters)
//	{
//		LogMsg += FString::Printf(TEXT(" %s,"), *ThisFilter);
//	}
//	UE_LOG(LogBladeII, Log, TEXT("%s"), *LogMsg);
//#endif
//#endif
//
//	// 게임모드의 레벨 구성에 따라서 이걸 콜하는 기본 시점에 EventDirectors 배열이 안 채워져 있을 수도 있다.
//	// 그런 경우 그냥 놔둬도 기능 자체는 이상이 없을 것이나 만일 Matinee 참조 에셋이 로딩되면서 눈에 띄는 끊김을 유발하거나 async flush 를 일으킨다면
//	// 이걸 추가로 콜해야 할 수도.
//	for (AB2StageEventDirector* CurrSED : EventDirectors)
//	{
//		if (CurrSED && CurrSED->IsExpectedForGameMode(this))
//		{
//			CurrSED->LoadLazyLoadedMatineeRefAssets(StageEventRefLoadExcludeFilters);
//			++LazyLoadedSEDNum;
//		}
//	}
//#endif
	return LazyLoadedSEDNum; // Lazy-loading 처리가 된 StageEventDirector (거의 Matinee) 개수를 리턴.
}

void ABladeIIGameMode::GameContextAsyncFlush()
{ // 로딩 타임에 Preload 한 거 이외에 게임 플레이 되면서 Async 로딩 할 것들이 flush 될 수 있다거나 
	// 내지는 바람직하지 않은 상황이 있다면 이걸 사용해서 그런 것들을 미연에 처리하기 위해 제공
	// 이건 로딩 타임에 불려야 한다.
	if (TheActiveSpawnPool)
	{
		TheActiveSpawnPool->TryAsyncLoadReservedMonsters();
		FlushAsyncLoading();
	}
}

void ABladeIIGameMode::PrebuildAndCacheCharMesh(EPCClass InCharClass, const TArray<FB2Item>& InEquippedItems, const FB2Wing* InWingData, bool bMergeSections, int32 InOptionalCharEntryID /*= -1*/)
{
	// Preload 시점에 태그 캐릭터 mesh merge 를 미리 해 놓기 위한 목적인데.. 
	// 로컬 캐릭터 mesh 의 RootSet 처리를 하는 B2CompositeMeshCache 의 도입 이후로는 일반 게임모드에서는 사실상 필요 없는 기능이다.
	// 장비 장착이 다른 걸로 강제되는 튜토리얼 모드에서는 필요하고.. 
	// 내지는 PVP 의 상대방 태그 캐릭터를 미리 merge 해 놓게 되면 거기서 필요할 수도 있긴 한데.. 그럴려면 좀 수정이 필요.

	UWorld* TheWorld = GetWorld();
	UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox(this);
	UB2PCClassInfo* ThisCharInfo = PCInfoBox ? PCInfoBox->GetSingleClassInfo(InCharClass) : nullptr;
	if (TheWorld && ThisCharInfo)
	{
		// 일시적으로 owner 역할을 할 dummy 를 spawn 시킨다.
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.ObjectFlags |= RF_Transient;
		FTransform SpawnTransform(FVector(0.0f, 0.0f, 0.0f));
		ASkeletalMeshActor* DummyOwnerSKActor = TheWorld->SpawnActor<ASkeletalMeshActor>(ASkeletalMeshActor::StaticClass(), SpawnTransform, SpawnInfo);
		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
		if (DummyOwnerSKActor && MeshCacheManager)
		{
			// LocalPlayerChar 는 NULL 이어도 문제 없음. 여하간 CompositeMeshCache 쪽서 Local 용으로 처리할 것.
			ABladeIIPlayer* LocalPlayerChar = Cast<ABladeIIPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
			// 의도한 이 함수의 사용에서 BuiltResult 는 NULL 일 것이다. B2CompositeMeshCache 에서 로컬 캐릭터 용 메쉬를 RootSet 처리를 하는 일반 스테이지 모드에선 여기로 와선 안됨.
			// 이게 NULL 이 아닐 하나의 가능성은 테스트 중에 튜토리얼 게임모드에서 쓰는 것과 똑같은 장비를 입혀놓고 튜토리얼 게임모드로 들어가는 거.
			USkeletalMesh* BuiltResult = MeshCacheManager->GetCachedCompositeMesh(InCharClass, LocalPlayerChar, InEquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, bMergeSections);

			if (UB2PCClassInfo::SetupSKCompForParts(InCharClass, DummyOwnerSKActor, DummyOwnerSKActor->GetSkeletalMeshComponent(), BuiltResult, ThisCharInfo->BaseMeshAsset, ThisCharInfo->DefaultPartsInfo,
				InEquippedItems, InWingData, 
				nullptr, // 메쉬만 빌드하는 게 목적이므로 AnimBP 같은 건 전달할 필요 없지
				false, // 아직은 빌드한 걸 캐싱하는 목적을 살리려면 원본 파트들도 보존해야 해서..
				bMergeSections)) 
			{
				MeshCacheManager->SetCachedCompositeMesh(InCharClass, LocalPlayerChar, BuiltResult, InEquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, bMergeSections);
			}
			DummyOwnerSKActor->Destroy();
		}
	}
}

void ABladeIIGameMode::PrebuildLocalTagCharMesh()
{
	// 로컬 캐릭터의 태그가 허용되는 모드에서 필요한 기능. 즉 모험모드, 영웅의탑, PVP
	// PVP 의 경우는 로컬 캐릭터 뿐 아니라 리모트 캐릭터도 태그를 하므로 그것까지 고려하려면 확장된 기능이 필요하다.

	UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
	if (MeshCacheManager && MeshCacheManager->IsLocalPCMeshRootSetAllowed()) {
		// 로컬 캐릭터 메쉬의 RootSet 처리 이후에는 이건 필요 없을 것임. 
		// 테스트를 위해 당분간 남겨둠.
		return;
	}

	FLocalCharacterData& LocalCharData = BladeIIGameImpl::GetLocalCharacterData();
	TArray<FB2Item> TagCharEquipItems;
	LocalCharData.GetEquippedItems(LocalCharData.GetSubPlayerClass(), TagCharEquipItems);
	LocalCharData.GetEquippedCostumeItems(LocalCharData.GetSubPlayerClass(), TagCharEquipItems);
	FB2Wing TagCharWing;
	bool bHasWing = LocalCharData.GetCharacterWing(LocalCharData.GetSubPlayerClass(), TagCharWing);
	ICharacterDataStore::GetRenderItem(&LocalCharData, LocalCharData.GetSubPlayerClass(), TagCharEquipItems);

	PrebuildAndCacheCharMesh(LocalCharData.GetSubPlayerClass(), TagCharEquipItems, bHasWing ? &TagCharWing : nullptr,
		false // 일단 기본적으로 섹션 나눠진 일반 모델로.
	);

	// 태그 캐릭터를 사용하는 모드라면 섹션 병합된 모델을 사용할 가능성을 생각하기는 어렵지만.. 코드는 갖춰두고.
	if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
	{
		PrebuildAndCacheCharMesh(LocalCharData.GetSubPlayerClass(), TagCharEquipItems, bHasWing ? &TagCharWing : nullptr, true);
	}
}

bool ABladeIIGameMode::IsAllowTag()
{
	//IGameRule* pGgmeRule = GetGameRule();

	//if (pGgmeRule)
	//	return pGgmeRule->AllowToTag();

	return true;
}

bool ABladeIIGameMode::ShouldSetupPreRender()
{
//#if !UE_BUILD_SHIPPING // PreRender 셋업하면서 로딩하게 될 수도 있으니 테스트하려면 끈다 ㅋ
//	extern int32 gPCDataPreLoadMaxNum;
//#endif
//
//	return (UB2PreRenderer::MaxPreRenderCount > 0 // 비교 테스트를 위해 0 이면 생성을 하지 않는다. 플랫폼이나 기타 조건에 따라 사용을 안해야 할 수도 있고..
//#if WITH_EDITOR
//		&& !GIsEditor // 에디터 플레이에서 가끔 보기 안좋은 렌더링 문제를 일으키기도..
//#endif
//#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 는 메모리 소모가 더 큰 문제라 곤란하기도 하고 셰이더 캐싱을 어떻게 하는지 몰라도 안드로이드보다 덜 버벅임.
//		&& UB2PreRenderer::ShouldDoPreRenderForIOS() // SetupPreRenderObjects 도중 30 ~ 50MB 정도 메모리가 올라간다. 그리고 GPU 메모리에 상당히 위협이 되는 모양. 별도의 PreRenderGameMode 에서는 해 볼 수도 있다.
//#endif
//#if !UE_BUILD_SHIPPING // PreRender 셋업하면서 로딩하게 될 수도 있으니 테스트하려면 끈다 ㅋ
//		&& (gPCDataPreLoadMaxNum < 0 || gPCDataPreLoadMaxNum >= PCClassToInt(EPCClass::EPC_End))
//#endif
//		);
	return false;
}
////
void ABladeIIGameMode::SetupPreRenderObjects() 
{
	// 셰이더 컴파일을 미리 해 놓지 않는 모바일 플랫폼에서의 셰이더 컴파일을 위한 기능. 여기서 놓치는 게 있다고 뭐가 작동 안하는 건 아니고 걍 중간에 필요해 질 때 버벅거림.

	//check(!PreRenderer);
	//PreRenderer = NewObject<UB2PreRenderer>(this, NAME_None, RF_Transient);
	//check(PreRenderer);
	//if (PreRenderer)
	//{
	//	if (PreRenderer->SetupPreRenderObjects(this))
	//	{
	//		B2GMLoadingProgCollector::StepCustom(0.2f); // Pre-render 셋업을 했다면 좀 먹었을 듯.

	//		// 가린 동안 게임이나 특히 처음 연출이 거의 진행되지 않도록 매우 작은 TimeDilation 을 준다.
	//		SetWholeGameTimeScale(0.0001f); // 다른 곳에서의 TimeDilation 과 얽히지 않도록 일시적으로 WholeGameTimeScale 을 사용.

	//		PreRenderPhase = EPreRenderPhase::EPRP_Selected; // PreRender 진입 마크. 만에 하나 생성된 Dummy 가 없더라도 일단 Selected 로. All 로 진행은 이후 처리될 것.
	//	}
	//	else
	//	{
	//		// SetupPreRenderObjects 실패라기보다는.. 이미 Pre-render 를 거친 이후면 이렇게 되는 건데..
	//		// 매번 OnPreRenderComplete 라니 좀 낭비스럽군.
	//		OnPreRenderComplete(); 
	//	}
	//}
}

void ABladeIIGameMode::UpdateForPreRenderObjects()
{
	//checkSlow(IsInPreRenderPhase() && PreRenderer);
	
	//if (PreRenderer)
	//{
	//	PreRenderer->UpdateForPreRenderObjects();
	//}

	//if (PreRenderPhase == EPreRenderPhase::EPRP_Selected)
	//{
	//	if (PreRenderer && !PreRenderer->HasPreRenderDummyToProcess()) // 기본 Selected 단계 완료.
	//	{
	//		if (UB2PreRenderer::bPreRenderAllPrimitives) 
	//		{// 추가로 전체 primitive 들 대상으로..
	//			if (PreRenderer->SetupPreRenderAllPrimPhase()) {
	//				PreRenderPhase = EPreRenderPhase::EPRP_AllPrim;
	//			}
	//			else {
	//				// 이미 한번 PreRender 를 거친 레벨이거나 기타 사유로 특별히 AllPrim 처리를 안하도록 한 곳은 실패할 것. 종료 처리
	//				OnPreRenderComplete(); 
	//			}
	//		}
	//		else 
	//		{ // Pre-render 종료
	//			OnPreRenderComplete(); 
	//		}
	//	}
	//}
	//else if (PreRenderPhase == EPreRenderPhase::EPRP_AllPrim)
	//{
	//	if (PreRenderer && PreRenderer->CheckAndFinishPreRenderAllPrimPhase())
	//	{ // Pre-render 전체 싸이클 다 끝내고 종료
	//		OnPreRenderComplete();
	//	}
	//}
}

void ABladeIIGameMode::OnPreRenderComplete()
{
	check(PreRenderer);
	if (PreRenderer) {
		// AllPrim 여부에 상관없이 현재 월드가 기반하고 있는 레벨에서는 Pre-render 가 한번 완료된 것으로 기록.
		PreRenderer->MarkCurrentWorldLevelPreRendered();
		PreRenderer->OnPreRenderComplete();
		PreRenderer->ConditionalBeginDestroy(); // 뭔가 빨리 제거 마크라도 해 주어야 안심이 될 듯한 기분
	}
	PreRenderer = nullptr;
	
	PreRenderPhase = EPreRenderPhase::EPRP_Done;

	// Pre-render 걸린 상태에서 시작했던 연출에서 일부 비활성화 시켰던 거 있으면 복구.
	for (AB2StageEventDirector* ThisSED : EventDirectors)
	{
		if (ThisSED && ThisSED->IsPlayingShow())
		{
			ThisSED->RestoreTracksAfterPreRender();
		}
	}

	ClosePreLoadingScreen(EPreLoadingScreenType::PLST_PreRender_Default);
	RestoreWholeGameTimeScale(); // 화면 가린 동안 낮췄던 WholeGameTimeScale 복구.
	EnsureStageEventTimeDilation(); // EventDirector 쪽도 TimeDilation 안 주었을 테니 이 때 같이 복구를.

	// Pre-render 를 한다면 따로 타이머 설치 안했을 꺼고 이 시점에서 로딩 이미지 언로드. Pre-render 시 블록 스크린에 쓰게 될 수도 있고.
	UnloadAndClearLoadingImageState();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	// Pre-render 를 했다면 이 시점에서 로딩 시간 측정 종료
	DevCheckLoadingTimeEnd();
#endif
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
int32 gDetectedGarbageExcessiveRenderCount = 0;
#endif
void ABladeIIGameMode::OnPreLoadMap(const FString& InString)
{
//	//
//	// 이름은 PreLoadMap 이라 이번 게임모드 시작 직전으로 오해할 수 있지만 실제로는 자기 자신은 내려갈 준비를 하는 부분.
//	// 매 맵 전환 직전 아직 World GC 되기 이전 무언가 정리하기 좋은 스팟임.
//	// 각 UObject 들의 BeginDestroy 에서는 하면 잘 안되는 것들이 여기에서는 여전히 가능한 경우가 많음.
//	//
//
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::OnPreLoadMap"));
//
//#if !UE_BUILD_SHIPPING || BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	// ExcessiveRenderProcessing 은 Pre-render 도중에만 사용하는 특별한 기능으로, 자칫 일반적인 상황에까지 흘러나왔다가는 성능에 큰 쥐약이 된다.
//	{
//		int32 ExcessiveRenderModeCount = 0;
//		for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
//		{
//			UPrimitiveComponent* ThisPrimComp = *It;
//			if (ThisPrimComp && ThisPrimComp->IsInExcessiveRenderMode())
//			{
//				++ExcessiveRenderModeCount;
//			}
//		}
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 쉽핑 테스트 빌드에서도 표시..
//		gDetectedGarbageExcessiveRenderCount = ExcessiveRenderModeCount;
//#endif
//		checkf(ExcessiveRenderModeCount == 0, TEXT("Detected %d PrimitiveComponents in ExcessiveRenderProcessing at last level transition!"), ExcessiveRenderModeCount);
//	}
//#endif
//
//	// 기본값 차원.. 게임모드 쪽 변수(UseCustomWaitForLoadingScreen)는 여기서 세팅해봐야 소용이 없겠지만 MoviePlayer 쪽 변수는 리셋이 필요해서.
//	SetUseCustomWaitForLoadingScreen(false);
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	// 로딩 시간 측정.
//	DevCheckLoadingTimeBegin();
//#endif
//
//	UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//	if (MeshCacheManager)
//	{
//		MeshCacheManager->OnPreLoadMap(this);
//	}
//	
//	UnloadAndClearLoadingImageState(); // 웬만해서 이 시점에 다 내려가 있긴 해야 하는데 타임 딜레이를 주는 이상 모르는 일이니. PreLoadingScreen 열었다가 이걸로 Unload 하고 다시 LoadingScreen 때문에 로딩하는 건 문제없다.
//	ClosePreLoadingScreenAll(); // 예열(?) 성격으로 UI 로 제작된 PreLoadingScreen 을 띄우는 게 있어서 그걸 닫음.
//	SetupNextGameLoadingScreen();
//	
//	LobbyEndOnTouchImpulseRingClass<>::GetInstance().Signal(); // 처리를 안해주는 경우 만에 하나 남아 있을 수 있다. 로비모드 아니면 이걸 처리하지 않으므로 확실히 끝내줌.
//	
//	// 기본 코드에서는 레벨 종료시 불리지 않는 듯한데, VirtualJoystick 은 매번 새로 생성. 예전에 뭔가 버그가 있어서 넣어두었었는데 이제 괜찮을지도..?
//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
//	if (B2PC)
//	{
//		B2PC->DeactivateVitualJoystick();
//	}
//
//	// 결국 매 월드 내려갈 때마다 같이 없애주는 걸로. UIManager 는 리소스 오브젝트라 실질적인 GC 는 안되므로 내부 찌꺼기들을 직접 제거해야 한다.
//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//	if (UIMgrInst)
//	{
//		UIMgrInst->MasterDestroy(); // 그냥 Destroy 말고 MasterDestroy 로 해야 함.
//	}
//	if (UIManager_InGameCombat) // DJLegacy 의 전투씬 UIManager 
//	{
//		UIManager_InGameCombat->OnPreLoadMap();
//	}
//
//	StopBGM();
//
//	// Async Load request 같은 거 모두 완료 및 클리어.
//	// 바로 아래 ManualUnloadBeforeNewBeginning 과 같은 맥락임.
//	if (UB2AssetLoader* AssetLoader = UB2AssetLoader::Get()){
//		AssetLoader->FlushAll();
//	}
//	ManualUnloadBeforeNewBeginning(this); // InfoAsset 들 일부 예외 제외하고 모두 싹 클리어
//
//	SaveLocalIniStateOnPreLoadMap(); 
//	
//	ExtraOnPreLoadMap();
//
//	IStreamingManager::Get().StreamAllResources(5.0f);
}

/** OnPreLoadMap 시점의 이제 로딩이 시작되는 게임모드를 위한 로딩 스크린셋업. */
void ABladeIIGameMode::SetupNextGameLoadingScreen()
{
	//IB2GameLoadingScreenModule* const LoadingScreenModule = GetB2LoadingScreenModulePtr();
	//if (LoadingScreenModule != nullptr)
	//{
	//	const FString ClosingMovieName = GetClosingMovieName();
	//	if (ClosingMovieName.Len() > 0) 
	//	{
	//		// 특별히 종료 시 플레이할 동영상을 지정하는 경우.. Subtitle 이나 기타 인자는 필요에 따라 구현 추가되어야 함.
	//		// 일반적인 시나리오 동영상이 아님. 그건 다음 로딩된 게임모드의 StartPlay 에서 셋업
	//		LoadingScreenModule->StartGameLoadingScreen_Movie(*ClosingMovieName,
	//			// 그냥 FitToMin 으로 하면 다양한 동영상 비율에 대한 대처가 가능하나 AppleMovieStreamer 구조와 관련해서 IOS 일부 기기에서 일시적으로 화면 비율이 흐트러지는 문제가 발생할 수 있다.
	//			// DLC 타이틀에서 로비 로딩을 제외한 기타 로딩 동영상이 16:9 로 제작되는 한 여기는 StdHD 로 고정.
	//			ELoadingScreenMovieAlignMode::FitToMin_StdHD,
	//			true	// 튜토리얼 Skip 추가
	//		);
	//	}
	//	else 
	//	{ // 일반적인 이미지 로딩 스크린
	//		SetupLoadingScreenImageCommon(false, true, false);
	//	}
	//}
}
void ABladeIIGameMode::SetupLoadingScreenImageCommon(
	bool bSetImagePersistent, // 사용한 로딩 이미지를 (현 게임모드 도중) 다음번에도 사용하려면 true 로.
	bool bInDisplayProgress, bool bInDisplayIndicator,
	const TCHAR* InOptionalImageName, /*= nullptr*/
	EB2LoadingMessageType InLoadingMessageType /*= EB2LoadingMessageType::ELMT_GameLoading*/)
{
	IB2GameLoadingScreenModule* const LoadingScreenModule = GetB2LoadingScreenModulePtr();
	if (LoadingScreenModule != nullptr)
	{
		int32 ViewSizeX = 1280, ViewSizeY = 720;
		APlayerController* LocalPC = GetWorld()->GetFirstPlayerController();
		//APlayerController* LocalPC = Cast<APlayerController>(UGameplayStatics::GetLocalPlayerController(this));
		if (LocalPC)
		{
			LocalPC->GetViewportSize(ViewSizeX, ViewSizeY);
		}

		float OptimalAspectRatio = 2.055556f;
		GConfig->GetFloat(TEXT("/Script/BladeII"), TEXT("LoadingImageOptimalAspectRatio"), OptimalAspectRatio, GGameIni);

		LoadingScreenModule->StartGameLoadingScreen_Image(
			InOptionalImageName ? InOptionalImageName : *GetLoadingImageName(bSetImagePersistent), // 일반적인 경우, InOptionalImageName 을 넣지 않고 GameMode 자체적으로 정해진 루틴에 따른 이미지를 사용.
			bInDisplayProgress, bInDisplayIndicator,
			FVector2D((float)ViewSizeX, (float)ViewSizeY),
			false, OptimalAspectRatio, // 특정 화면비율을 최적으로 한 중앙정렬 사용.
			InLoadingMessageType
		);
	}
}

#if WITH_EDITOR
void ABladeIIGameMode::OnEditorEndPlayMap()
{
	// 에디터 플레이 중단 시에도 작동했으면 하는 OnPreLoadMap 동작들이 있으면 여기에.. UIManager Destroy 는 자체적으로 실행됨.
		
	SaveLocalSavedAutoType();
}
#endif

FString ABladeIIGameMode::LastUsedLoadingImageName(TEXT(""));

const FString ABladeIIGameMode::GetLoadingImageName(bool bPersistent)
{
	//UTexture2D* LoadingImageTexture = GetLoadingImageTexture(CachedPreLoadingGameMode, bPersistent);
	//if (LoadingImageTexture)
	//{
	//	return LoadingImageTexture->GetPathName();
	//}
	return FString(TEXT(""));
}

UTexture2D* ABladeIIGameMode::GetLoadingImageTexture(EB2GameMode InGameMode, bool bPersistent)
{
	UB2LoadingImageInfo* LIInfo = StaticFindLoadingImageInfo(this);
	if (LIInfo)
	{
		UTexture2D* LoadedLoadingImageTexture = NULL;
		// 기존에 무작위로 정해놓은 값이 없으면 새로 뽑아 쓰고
		// bPersistent 인 경우 무작위로 정해진 값을 저장해서 다음에 똑같이 사용할 수 있도록 함.

		if (LoadingImageIdx == INDEX_NONE)
		{
			int32 ThisRandIndex = INDEX_NONE;
			LoadedLoadingImageTexture = LIInfo->RandGetLoadingImage(InGameMode, ThisRandIndex);
			
			if (bPersistent && LoadedLoadingImageTexture && ThisRandIndex != INDEX_NONE) {
				LoadingImageIdx = ThisRandIndex;
			}
		}
		else
		{
			LoadedLoadingImageTexture = LIInfo->GetLoadingImageOfIndex(InGameMode, LoadingImageIdx);
		}

		// 이건 다른 상황에서 로딩 이미지를 연장해서 쓰기 위해..
		LastUsedLoadingImageName = LoadedLoadingImageTexture ? LoadedLoadingImageTexture->GetPathName() : TEXT("");

		return LoadedLoadingImageTexture;
	}
	return NULL;
}

void ABladeIIGameMode::OnLoadingImageUnloadTimerCB()
{
	//UWorld* TheWorld = GetWorld();
	//if (TheWorld)
	//{
	//	TheWorld->GetTimerManager().ClearTimer(LoadingImageUnloadTH);
	//	if (!IsInPreRenderPhase() && GetMoviePlayer()->IsLoadingFinished()) // 사실 WaitForMovieToFinish 후면 IsLoadingFinished 여야 한다. 그 시점에 Unload 를 해도 문제가 일어나는 이유를 알수가 없어서 이러는 건데..
	//	{
	//		//FlushRenderingCommands(); // IsLoadingFinished 이후에도 실질적으로 로딩 이미지를 쓸 가능성이 있는 거라면 렌더 쓰레드 플러쉬라도..
	//		UnloadAndClearLoadingImageState();
	//	}
	//	else
	//	{// 만일 아니면 또 대기. 실질적으로 이렇게 걸리는 일은 없을 듯..?
	//		TheWorld->GetTimerManager().SetTimer(LoadingImageUnloadTH, this, &ABladeIIGameMode::OnLoadingImageUnloadTimerCB, 1.0f, false);
	//	}
	//}
}
void ABladeIIGameMode::UnloadAndClearLoadingImageState()
{
	if (!bLoadingImageUnloadedAndClear)
	{
		UWorld* TheWorld = GetWorld();
		if (TheWorld)
		{
			TheWorld->GetTimerManager().ClearTimer(LoadingImageUnloadTH);
		}
		UB2LoadingImageInfo* LoadingImgInfo = StaticFindLoadingImageInfo(this);
		if (LoadingImgInfo)
		{
			LoadingImgInfo->UnloadAll();
		}
		bLoadingImageUnloadedAndClear = true;
	}
}

void ABladeIIGameMode::SetUseCustomWaitForLoadingScreen(bool bSet)
{
	//bUseCustomWaitForLoadingScreen = bSet;
	//IGameMoviePlayer* GMP = GetMoviePlayer();
	//if (GMP) { // GameMoviePlayer 쪽에도 같이 세팅을 해야 기본 엔진 루틴에 의한 종료를 막을 수 있다.
	//	GMP->SetUseCustomWait(bUseCustomWaitForLoadingScreen);
	//}
}
void ABladeIIGameMode::WaitForLoadingScreen()
{ 
//	GlobalMarkMapLoadingProgressDone(); // 100% 로 채우는 건데 사실 여기는 로딩 화면 끝나는 곳이라 이거 이전 적절한 시점에 100% 로 채워놓아야..
//
//	// 그냥 로딩 화면이면 중단이 될 꺼고 영상 플레이를 시작한 경우라면 끝날때까지 대기. 이렇게 대기 중 지정한 시간이 지나서 플레이 되고 있으면 Skip 버튼이 뜰 것
//	IGameMoviePlayer* GMP = GetMoviePlayer();
//	if (GMP) {
//		GMP->WaitForMovieToFinish();
//	}
//
//	B2Scalability::OnGameModeLoadingScreenEnd();
//
//	// 일반적인 맵 로딩이 아니면서 GameMoviePlayer 를 쓴다면 약간 얘기가 다르겠지만
//	// 일반적으로 이 타이밍은 이번 게임모드의 로딩이 완료되고 로딩 스크린까지 걷힌 시점이 되겠다.
//	OnLoadingScreenFinished();
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (!IsInPreRenderPhase())
//	{
//		// Pre-render 를 안 한다면 이 시점에서 로딩 시간 측정 종료
//		DevCheckLoadingTimeEnd();
//	}
//#endif
}
void ABladeIIGameMode::ConditionalWaitForLoadingScreen() // WaitForMovieToFinish 에서도 체크는 하므로 굳이 이렇게 안 해도 될 것 같긴 한데..
{
	IGameMoviePlayer* GMP = GetMoviePlayer();
	if (GMP && GMP->IsMovieCurrentlyPlaying()) {
		WaitForLoadingScreen();
	}
}
void ABladeIIGameMode::OnLoadingScreenFinished()
{
	/*
	 이번 게임모드의 로딩이 끝난 시점이 될 수 있는데
	 일반적으로는 이것보다 PreBeginPlay, BeginPlay 등의 다른 "친숙한" 시점을 사용하는 게 좋다.
	 
	 로딩 화면은 비동기로 돌아가면서 특히 동영상인 경우는 메인쓰레드가 준비된 이후에는 메인쓰레드를 막기 때문에 
	 일반적으로는 로딩 화면이 돌아가는 동안 원치 않는 동작이 백그라운드로 돌아가는 것에 대해 우려할 필요는 없다.

	 그러나 간혹 예외 상황이란 게 있기 때문에 (엔진 외부 플랫폼 쪽으로 신호를 보낸다거나 하는 등의)
	 그런 경우는 결국 여기서 로딩 스크린이 끝나는 타이밍을 알아와야 함.

	 게임모드 성격에 따라 override 해서 사용
	*/ 
}

void ABladeIIGameMode::OpenPreLoadingScreen(EPreLoadingScreenType InPreLoadType, EB2GameMode GameMode /*= EB2GameMode::Unknown*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::OpenPreLoadingScreen"));

	ClosePreLoadingScreen(InPreLoadType); // 만에 하나 중복해서 콜하게 될 때 이전꺼 처리 안해주면 좀 문제가..
	
	CachedPreLoadingGameMode = GameMode;

	// 원래 이거 목적이 그런 건 아닌데 OnPreLoadMap 에서 B2GameLoadingScreenModule 의 로딩 스크린 띄우기 전 로딩 이미지 로딩을 확실히 하는 역할이 있는 듯 함.

	// InPreLoadType 추후 추가되면 필요에 따라 사용
	//APlayerController* LocalPC = UGameplayStatics::GetLocalPlayerController(this);
	APlayerController* LocalPC = GetWorld()->GetFirstPlayerController();
	TSubclassOf<class UB2PreLoadingScreen>& UsedWidgetClass = IsOneOfPreRenderScreenType(InPreLoadType) ? PreLoadingScreenWidgetClass_PreRender : PreLoadingScreenWidgetClass;
	if (UsedWidgetClass && LocalPC)
	{
		UB2PreLoadingScreen* LocalCreated = CreateWidget<UB2PreLoadingScreen>(LocalPC, UsedWidgetClass);

		if (LocalCreated)
		{
			// PreRenderScreen 은 하더라도 별도로 처리.. 아니면 실제로 여기서 PreRender 가림막에 사용할 이미지를 로딩하게 될 수도 있고.
			UTexture2D* LoadedLoadingImage = nullptr;
			if (IsOneOfPreRenderScreenType(InPreLoadType))
			{
				if (InPreLoadType == EPreLoadingScreenType::PLST_PreRender_Default && !LastUsedLoadingImageName.IsEmpty())
				{
					LoadedLoadingImage = LoadObject<UTexture2D>(nullptr, *LastUsedLoadingImageName);
				}
				// PLST_PreRender_First 에서는 그냥 NULL.. UB2PreLoadingScreen 안에서 처리.
			}
			else
			{
				// 일반적인 맵 로딩 직전 케이스.
				LoadedLoadingImage = GetLoadingImageTexture(GameMode, true);
			}
			
			LocalCreated->AddToViewport(BII_WIDGET_ZORDER_PRELOADINGSCREEN);
			LocalCreated->SetPreLoadingType(InPreLoadType, LoadedLoadingImage);

			if (IsOneOfPreRenderScreenType(InPreLoadType)) {
				CreatedPreLoadingScreen_PreRender = LocalCreated; // 얘는 따로..
			}
			else {
				CreatedPreLoadingScreen = LocalCreated;
			}
		}
	}
}

void ABladeIIGameMode::ClosePreLoadingScreen(EPreLoadingScreenType InPreLoadType)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::ClosePreLoadingScreen"));

	if (IsOneOfPreRenderScreenType(InPreLoadType)) {
		if (CreatedPreLoadingScreen_PreRender) // 얘는 따로 관리를..
		{
			CreatedPreLoadingScreen_PreRender->DestroySelf();
			CreatedPreLoadingScreen_PreRender = NULL;
		}
	}
	else {
		if (CreatedPreLoadingScreen)
		{
			CreatedPreLoadingScreen->DestroySelf();
			CreatedPreLoadingScreen = NULL;
		}
	}
}

void ABladeIIGameMode::ClosePreLoadingScreenAll()
{
	for (int32 PLST = 0; PLST < static_cast<int32>(EPreLoadingScreenType::PLST_End); ++PLST)
	{
		ClosePreLoadingScreen(static_cast<EPreLoadingScreenType>(PLST));
	}
}

UB2PreLoadingScreen* ABladeIIGameMode::GetCurrCreatedPreLoadingScreen(EPreLoadingScreenType InType)
{
	return IsOneOfPreRenderScreenType(InType) ? CreatedPreLoadingScreen_PreRender : CreatedPreLoadingScreen;
}

void ABladeIIGameMode::BeginPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::BeginPlay"));
//	
//	Super::BeginPlay();
//
//#if WITH_EDITOR
//	if (GIsEditor && !FGameDelegates::Get().GetEndPlayMapDelegate().IsBoundToObject(this)) { //중첩 바인딩 안되게
//		FGameDelegates::Get().GetEndPlayMapDelegate().AddUObject(this, &ABladeIIGameMode::OnEditorEndPlayMap);
//	}
//#endif
//
//	if (!bDeferAutoBeginAuto) // 실제 Match Start 시점이 늦어지는 게임모드는 bDeferAutoBeginAuto 를 True 로 두고 적절한 시점에 TryAutoBeginAutoAtBeginning 을 직접 콜하도록.
//	{
//		TryAutoBeginAutoAtBeginning();
//	}
//
//	if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
//		DocBattle->ClearBattleDataForStageClear();
//
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 쉽핑 테스트 빌드에서도 표시..
//	if (gDetectedGarbageExcessiveRenderCount > 0)
//	{
//		BII_SCREEN_LOG(FString::Printf(TEXT("[WARNING] Found %d objects in ExcessiveRenderProcessing!"), gDetectedGarbageExcessiveRenderCount),
//			FLinearColor(1.0, 1.0f, 0.0f, 1.0f), 18, 10.0f);
//		gDetectedGarbageExcessiveRenderCount = 0;
//	}
//#endif
//
//	// UAnimNotifyState_B2CameraAnim::SetBGVisibility 에서 설정해논거 초기화
//	auto* ViewportClient = GetWorld()->GetGameViewport();
//	if (ViewportClient)
//	{
//		ViewportClient->EngineShowFlags.SetFog(true);
//		ViewportClient->EngineShowFlags.SetStaticMeshes(true);
//	}
}

void ABladeIIGameMode::BeginDestroy()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::BeginDestroy"));

	//DeleteGameRule();
	//
	//if (bEventsSubscribed)
	//{ // 실제 사용되지 않는 게임모드가 생성되었다가 제거될 수 있음. Subscribe 를 했던 게임모드일 경우만 Unsubscribe. Unsubscribe 시 GameMode 가 unique 하다고 가정하므로.
	//	UnsubscribeEvents();
	//}

	//ClosePreLoadingScreenAll();
	
	Super::BeginDestroy();
}

void ABladeIIGameMode::Destroyed()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::Destroyed"));

	//DeleteGameRule();

	//if (bEventsSubscribed)
	//{
	//	UnsubscribeEvents();
	//}

	//Super::Destroyed();
}

void ABladeIIGameMode::Tick(float dt)
{
	Super::Tick(dt);
//
//	if (BladeIIGameImpl::GetClientDataStore().ExistAccountInfo()
//#if WITH_EDITOR
//		|| 1  // 개발 버전 프리뷰 플레이 플리즈
//#endif
//		) 
//	{
//		RealtimeUpdateUIDoc();
//	}
//
//	if (IsInPreRenderPhase()) 
//	{
//		UpdateForPreRenderObjects(); // 이게 돌아가는 와중에는 아직 정식 시작 안 한 거..
//
//		// Pre-render 에서 메인 맵 로딩 화면 닫는 걸 지연시키게 되므로 여기서 처리. Pre-render 용 로딩 화면은 따로 있고 이건 단지 로딩 뺑글이 돌아가는 거 연장하기 위한 차원이다.
//		ConditionalWaitForLoadingScreen();
//	}
//
//#if BII_STATS
//	if (FStatisticalPerfStatCollector::bDoStatCollecting) {
//		PerfStatCollector.OnTick(this, dt);
//	}
//#endif
//
//#if !UE_BUILD_SHIPPING
//	// 모바일 셰이더 컴파일 로그 플러시
//	if (GDevCheckMobileShaderCompile)
//	{
//		// 아래 로그 표시하는 동안 렌더 쓰레드에서 GShaderCompileCheck_AllCompiledNotLogged 이거 추가할 가능성 최소화. 
//		// 기능 오동작에 민감하지는 않은 개발용 코드이니 lock 같은 거 안 걸고 적당히..
//		FlushRenderingCommands(); 
//
//		TArray<FString> ListCopy = GShaderCompileCheck_AllCompiledNotLogged;
//
//		if (ListCopy.Num() > 0)
//		{
//			for (int32 CI = 0; CI < ListCopy.Num(); ++CI)
//			{
//				const FString& CurrLogStr = ListCopy[CI];
//
//				UE_LOG(LogBladeII, Log, TEXT("[ShaderCompileCheck] %s"), *CurrLogStr);
//
//				BII_SCREEN_LOG(FString::Printf(TEXT("%s"), *CurrLogStr),
//					FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 10.0f);
//			}
//		}
//		GShaderCompileCheck_AllCompiledNotLogged.Empty();
//	}
//	// 또 다른 엔진 내부 동작 로그 플러시
//	if (GDevCheckSoundWaveLoading)
//	{
//		TArray<FString> ListCopy = GSoundWaveLoadingCheck_NotLoggedList;
//
//		if (ListCopy.Num() > 0)
//		{
//			for (int32 CI = 0; CI < ListCopy.Num(); ++CI)
//			{
//				const FString& CurrLogStr = ListCopy[CI];
//				UE_LOG(LogBladeII, Log, TEXT("[SoundLoadCheck] %s"), *CurrLogStr);
//				BII_SCREEN_LOG(FString::Printf(TEXT("%s"), *CurrLogStr),
//					FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 11, 10.0f);
//			}
//		}
//		GSoundWaveLoadingCheck_NotLoggedList.Empty();
//	}
//#endif
}

void ABladeIIGameMode::StartPlay()
{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StartPlay"));
//
//	if (HasAnyFlags(RF_ClassDefaultObject))
//	{
//		return;
//	}
//
//#if !UE_BUILD_SHIPPING
//	extern int32 gPCDataPreLoadMaxNum;
//	if (gPCDataPreLoadMaxNum == 0) {
//		// 아예 처음부터 spawn 을 하면서 하나는 로딩하게 되니 0 이면 기본 spawn 도 하지 않게 한다.
//		DefaultPawnClass = ASpectatorPawn::StaticClass();
//	}
//#endif
//
//	B2GMLoadingProgCollector::SetStepRate(0.1f);
//
//	auto AllowPrecomputedVisibilityVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AllowPrecomputedVisibility"));
//	if (AllowPrecomputedVisibilityVar != nullptr)
//	{
//		AllowPrecomputedVisibilityVar->Set(0); // Disable Precomputed Visibility
//	}
//
//	UWorld* World = GetWorld();
//	if (World)
//	{
//		B2_SCOPED_TRACK_LOG_L2(TEXT("FlushLevelStreaming/ForceGarbageCollection at ABladeIIGameMode::StartPlay"));
//		// Show all levels
//		World->bShouldForceVisibleStreamingLevels = true;
//		World->FlushLevelStreaming();
//		World->ForceGarbageCollection(); // 레벨 transition 하면서 당연히 될 걸로 생각했는데 실제로 보면 오브젝트가 좀 남아있고 GC 를 돌리고 나면 좀 제거된다.
//	}
	
#if WITH_EDITOR
	bDevCheckBladeIIGameModeEnteredOnce = true; // 딱 한번 진입시 마크해 둠. 단순 체크용도.
#endif

//	B2Scalability::OnGameModeStartPlay(this);
//
//	{
//		// 여기서 주요 InfoAsset 들에 대해 미리 StaticFind~~ 를 해 놓는데 
//		// 이들을 처음 호출하는 상황에서는 Async 로딩들이 flush 되기 때문에 미리 한번 불러주는 거.
//		// 이렇게 해 놓아도 다른 코드에서 직접 StatiFind~~ 를 WorldContextObject 없이 사용하면 도루묵.
//		// 대부분 껍데기만 가져오는 거라 그리 오래 안 걸림.
//		StaticFindPCClassInfoBox(this);
//		StaticFindMobClassInfoBox(this);
//		StaticFindCommonSoundInfo(this);
//		StaticFindDialogCharacterInfoTable(this);
//		StaticFindWingInfo(this);
//		StaticFindSomeInfo(this);
//		StaticFindItemMiscInfo(this);
//		StaticFindLoadingImageInfo(this);
//		StaticFindNoAnimSkillInfo(this);
//		StaticFindDamageEffectInfo(this);
//		StaticFindBuffModeEffectInfo(this);
//		StaticFindSkillAnimInfo(this);
//
//		// 여긴 BladeIiGameMode 쪽에 캐싱해 놓는 건 아닌데 여하간 이쪽도 아직 한번도 안 불린 상태에서는 마찬가지 문제를 야기할 수 있음.
//		StaticFindDialogInfoTable();
//		StaticFindDialogSoundInfoTable();
//		StaticFindQuestTable(); // 스테이지 클리어 시점에 QuestCompleteNotify 뜨면서 처음 불리면 flush 나는 듯.
//		StaticFindCharacterTagDialogueInfo();
//	}
//
//	BGMManager = NewObject<UB2BGMManager>(GetTransientPackage(), NAME_None, RF_Transient);
//
//	if(BGMManager)
//		BGMManager->Init(this);
//
//	B2GMLoadingProgCollector::Step();
//
//	// 다른 Actor 들의 BeginPlay 가 아래 Super::StartPlay 에서 시작됨.
//	PreBeginPlay(); // 로직 처리 타이밍 등으로 인해 문제가 발생하면 WaitForMovieToFinish 보다 뒤로 보내야 할 수도 있다.
//		
//	// 로딩 진행도 산정 땜에 미리 이쯤에 알아야 함. 동영상을 플레이하게 되면 로딩 진행도는 보다 일찍 채워 본다.
//	const FString PossibleOpeningMovieName = GetOpeningMovieName();
//	const bool bSupposedToPlayMove = (PossibleOpeningMovieName.Len() > 0);
//
//	// TSoftObjectPtr 등으로 인해 사전에 로딩이 안되어 있는 asset 들 중 이번 스테이지에서 필요할 것으로 예상되는 것들을 로딩한다. 
//	// Synchronous 로딩이 포함되므로 아래 거치면서 시간이 좀 걸림. 그러나 진짜 큰 건 Async 로딩을 함.
//	PreloadAnyNecessaryInfo(bSupposedToPlayMove);
//
//	// Preload 와 비슷하지만 다른 차원에서.. 모바일 셰이더 컴파일을 위한 Pre-render
//	if (ShouldSetupPreRender())
//	{
//		SetupPreRenderObjects();
//	}
//
//	IStreamingManager::Get().StreamAllResources(2.0f);
//	
//	////////////////////////////////////////////
//	bool bPlayingOpeningMovie = false; // 실제 플레이 여부..
//	{ // 스테이지 도입 영상이 있는 경우 동영상 플레이
//	  // 메인 에셋 로딩 (PreloadAnyNecessaryInfo) 전에 시작을 하면 좋은데 혹여나 에셋 로딩 다 되기 전에 영상이 끝나버릴 수 있다. 루핑을 하거나 아니면 그냥 까만 화면이 나오겠지.
//		// 이후 BeginPlay 를 거치면서도 이것저것 할 테니 이쯤서 틀어보자.		
//		if (bSupposedToPlayMove)
//		{
//			// 이 경우 PreloadAnyNecessaryInfo 를 거치면서 진행도도 다른 경우보다 많이 올라갔을 것.
//			// 끝까지 채우려니 아래 AsyncFlush 요소가 있어서.. 동영상 플레이도 로딩 스크린인 만큼 채워놓고 한참 기다리느니 약간 남겨두자.
//			B2GMLoadingProgCollector::StepCustom(0.1f, 0.9f); 
//
//			// 이게 Async 로딩 flush 를 일으키고.. 이걸 안쓴다 해도 LoadingScreenModule 안에서 내부적으로 로딩하는 게 또 있어서 .. ㅠㅠ 그냥 먼저 로딩해 둠..
//			FString LoadingImageName = GetLoadingImageName(false);
//
//			GetMoviePlayer()->WaitForMovieToFinish(); // 기존 로딩 스크린을 멈춘다.
//			
//			// AndroidThunkCpp_StageMoviePlay(MovieName); 수틀렸을 땐 플랫폼 자체 기능으로 이런 식으로 동영상 플레이를 하기도..
//			
//			IB2GameLoadingScreenModule* const LoadingScreenModule = GetB2LoadingScreenModulePtr();
//			if (LoadingScreenModule != nullptr)
//			{
//				auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
//				EStageDifficulty StageDifficulty = StageDataStore.GetStageDifficulty(StageDataStore.GetLastClearServerStageIdByLastPlayedStageDifficulty());
//
//				int32 LastClearClientStageId = StageDataStore.GetLastClearClientId(StageDifficulty);
//				int32 CurrentStageId = AB2StageManager::GetCacheStageKeepEssentialData().GetCurrentClientStageId();
//
//				// StartGameLoadingScreen_Movie 에 전체화면 여부, Skip버튼 표시 여부 등의 인자를 넣어줄 수 있음. 필요에 따라 사용.
//				GetMovieSubtitles(LoadingScreenModule->StartGameLoadingScreen_Movie(*PossibleOpeningMovieName, 
//					// 그냥 FitToMin 으로 하면 다양한 동영상 비율에 대한 대처가 가능하나 AppleMovieStreamer 구조와 관련해서 IOS 일부 기기에서 일시적으로 화면 비율이 흐트러지는 문제가 발생할 수 있다.
//					// DLC 타이틀에서 로비 로딩을 제외한 기타 로딩 동영상이 16:9 로 제작되는 한 여기는 StdHD 로 고정.
//					ELoadingScreenMovieAlignMode::FitToMin_StdHD,
//					true	// 최초 플레이부터 Skip 가능하도록 변경
//				));
//				bPlayingOpeningMovie = true;
//			}
//		}
	//}
	////////////////////////////////////////////

///*
//	if (GetGameRule() == nullptr)
//	{
//		auto* GameRule = new DefaultGameRule;
//		SetGameRule(GameRule);
//	}
//*/
//
//	Super::StartPlay();
//	
//	if (!IsInPreRenderPhase())
//	{
//		// 이 시점에서 LoadingImage 를 Unload 해 주면 적절할 거 같은데 크래쉬가 나는 경우가 여전히 있다. 완전히 렌더링 단까지 안 끝났을 가능성이.. 궁여지책으로 타이머를.
//		UWorld* TheWorld = GetWorld();
//		if (TheWorld)
//		{
//			TheWorld->GetTimerManager().SetTimer(LoadingImageUnloadTH, this, &ABladeIIGameMode::OnLoadingImageUnloadTimerCB, 1.0f, false);
//		}
//	}
//	else
//	{ 
//		if (PreRenderer && !PreRenderer->IsInPreRenderGameMode()) {
//			// 일반적인 레벨 로딩타임에서의 PreRender 상황.
//			// SetupPreRenderObjects 시에는 안먹힐 수 있어서 여기서 Light 환경을 셋업하는데, PreRenderGM 에서는 별도로 특별히 Light pass 를 돌릴 거라 여기서 하지 않는다.
//			PreRenderer->ConditionalSetupPreRenderLightEnvForDynPL();
//		}
//
//		if (!bPlayingOpeningMovie) {
//			// PreRender 에서는 기본 로딩 화면 닫는 루틴으로는 뺑글이 멈춘 후에 시간 지연이 좀 있어서 로딩 화면 닫는 걸 좀 늦춘다.
//			// 단 동영상 플레이하게 되면 이게 오히려 동영상의 끊김을 유발해서 어쩔 수 없지..
//			SetUseCustomWaitForLoadingScreen(true);
//		}
//	}
//		
//	// 로딩 화면을 닫는 WaitForLoadingScreen 을 대체로 여기서 하지만 아닌 경우도 종종 있겠지..
//	if (!UseCustomWaitForLoadingScreen())
//	{
//		WaitForLoadingScreen();
//	}
}

void ABladeIIGameMode::HandleMatchHasStarted()
{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::HandleMatchHasStarted"));
//
//	UWorld* World = GetWorld();
//	if (World)
//	{
//		// Hide levels that should be hidden 
//		World->bShouldForceVisibleStreamingLevels = false;
//	}
//
//	Super::HandleMatchHasStarted();
//	
//#if BII_STATS // PerfStat 은 MatchStart 시점부터 모으기 시작.
//	PerfStatCollector.OnBegin(this);
//#endif
//
//	StartEventScene_GameBegin();
}

AB2StageEventDirector* ABladeIIGameMode::GetCurrentPlayingStageEventDirector()
{
	for (AB2StageEventDirector* CurrSED : EventDirectors)
	{
		if (CurrSED && CurrSED->IsPlayingShow())
			return CurrSED;
	}

	return NULL;
}

int32 ABladeIIGameMode::GetLocalSavedAutoType() const
{
	int32 bRetSavedState = AutoBattleType::NONE;
	const FString& SavedKeyName = GetLocalSavedAutoStateKeyName(); // 게임모드 별 Auto 상태를 저장할 ini 키 값
	if (GConfig && SavedKeyName.Len() > 0) {
		GConfig->GetInt(TEXT("AutoState"), *SavedKeyName, bRetSavedState, GB2UserSavedStateIni);
	}
	return bRetSavedState;
}

void ABladeIIGameMode::SaveLocalSavedAutoType() const
{
	const FString& SavedKeyName = GetLocalSavedAutoStateKeyName(); // 게임모드 별 Auto 상태를 저장할 ini 키 값
	if (GConfig && SavedKeyName.Len() > 0)
	{
		GConfig->SetInt(TEXT("AutoState"), *SavedKeyName, PlayerAutoBattleType, GB2UserSavedStateIni);
	}
}

bool ABladeIIGameMode::ShouldStartAutoFromBeginning()
{
	bool Result = false;

	int32 LocalAutoType = GetLocalSavedAutoType();

	if (LocalAutoType == AutoBattleType::Attack || LocalAutoType == AutoBattleType::Skill)
	{
		Result = true;
	}

	return Result; // 이 조건은 하위 클래스에서 기본으로 포함할 필요가 있겠다.
}

//void ABladeIIGameMode::TryAutoBeginAutoAtBeginning()
//{ // 각 모드 별 사용자가 이전에 auto 를 사용했는지 여부에 따라 자동으로 auto 를 돌리기 위함. 네퉠겜모드의 AI 플레이어는 해당안됨.
//	ABladeIIPlayer* LocalPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	if (LocalPlayer && ShouldStartAutoFromBeginning())
//	{
//		// Auto 를 끄는 연출이 현재 진행중인가를 보고 아니면 당장 시작은 안한다. 어차피 연출이 끝나면 알아서 시작될 것임.
//		bool bAutoStopEventRunning = false;
//		for (AB2StageEventDirector* CurrSED : EventDirectors)
//		{
//			if (CurrSED && CurrSED->IsPlayingShow() && CurrSED->ShouldStopAutoDuringShow())
//			{
//				bAutoStopEventRunning = true;
//				break;
//			}
//		}
//
//		if (!bAutoStopEventRunning)
//		{
//			SetPlayerAutoType(GetLocalSavedAutoType());
//		}
//		else
//		{ // 당장 Auto 시작은 안 하더라도 UIDoc 에 세팅은 해야.. StageEventDirector 쪽에서 Auto 복구를 할 때에는 이건 처리 안 할 것이므로.
//			UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
//			if (DocBattle)
//			{
//				DocBattle->SetInAutoPlay(GetLocalSavedAutoType());
//			}
//		}
//	}
//	else 
//	{
//		SetPlayerAutoType(GetLocalSavedAutoType());
//	}
//}
//
//PVP쪽에서 AutoAIController를 로딩하는 현상이 있어 가상메소드로 돌려서 B2PVPGameMode에서 오버라이드 함
void ABladeIIGameMode::SetPlayerAutoType(int32 InAuto) // 일시적인 On/Off 가 아닌 현 게임모드에서 유지되는 auto 상태.
{
	//if (PlayerAutoBattleType != InAuto)
	//{
	//	PlayerAutoBattleType = InAuto;
	//	SaveLocalSavedAutoType();
	//}

	//// 이걸 콜 하는 상황에서는 연출 등에 의해 임시적으로 Auto 가 꺼진다거나 하는 건 무시한다. 그것까지 생각하려면 좀 복잡하고 그럴 필요까지는 없을 듯함.
	//ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//if (LocalB2Player) 
	//{
	//	if (PlayerAutoBattleType == AutoBattleType::Attack || PlayerAutoBattleType == AutoBattleType::Skill)
	//	{
	//		LocalB2Player->StartAutoPlay();
	//	}
	//	else 
	//	{
	//		if (LocalB2Player->IsInAutoPlay()) 
	//		{
	//			LocalB2Player->StopAutoPlay();
	//		}
	//	}
	//}

	//// UI 로 시그널은 기본적으로는 이걸로만 보냄. 일시적인 On/Off 에서는 안 보내고. 근데 하다보니 그렇게만 되지는 않는군 쩝..
	//UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	//if (DocBattle)
	//{
	//	DocBattle->SetInAutoPlay(PlayerAutoBattleType);
	//}
}

//void ABladeIIGameMode::ToggleLocalPlayerAutoState()
//{
//	SetPlayerAutoType(UpdateAutoBattleType(PlayerAutoBattleType));
//}
//
//bool ABladeIIGameMode::IsLocalPlayerAutoBattle()
//{
//	bool Result = false;
//
//	if (PlayerAutoBattleType == AutoBattleType::Attack || PlayerAutoBattleType == AutoBattleType::Skill)
//	{
//		Result = true;
//	}
//
//	return Result;
//}
//
//void ABladeIIGameMode::TemporaryStopLocalPlayerAuto()
//{// bMaintainedLocalPlayerAutoState 는 유지하고 이걸로는 UI Doc 세팅을 하지 않는다. 연출 등으로 일시적으로 off 할 때 사용.
//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	if (LocalB2Player && LocalB2Player->IsInAutoPlay()) 
//	{
//		LocalB2Player->StopAutoPlay();
//	}	
//}
//void ABladeIIGameMode::RestoreLocalPlayerAuto()
//{ // 기본 상태가 true 인 경우만 복구.
//	if (PlayerAutoBattleType != AutoBattleType::NONE)
//	{
//		ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//		if (LocalB2Player) 
//		{
//			LocalB2Player->StartAutoPlay();
//		}
//	}
//}

bool ABladeIIGameMode::IsGlobalTimeDilationEnabled(const AActor* RelativePawn /*= NULL*/)
{
	return GetGameRule()->IsGlobalTimeDilationEnabled();
}

#if WITH_EDITOR
bool ABladeIIGameMode::bDevCheckBladeIIGameModeEnteredOnce = false;
#endif

void ABladeIIGameMode::PreBeginPlay()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::PreBeginPlay"));
	
	bAnyStageEventPlayedOnce = false;
//
//	// 다른 Actor 들의 BeginPlay 가 불리기 전에 처리되어야 할 것들을 여기에 쑤셔넣어 보자.
//
//	BladeIIGameImpl::GetLocalCharacterData().SetCharClassNames(StaticFindPCClassInfoBox(this)); // 전체 모듈 차원에서 초기 한번만 필요한 필수정보. AccountInfo 를 받는 곳을 제외하면 딱히 적당한 곳이 없어서..
//
//	UpdateManagedActorList();
//
//	// It will be picked up by player's BeginPlay.
//	SetDesiredPCClassEnum();
//
//	// 웬만하면 DefaultPawnClass 는 네이티브 기본값으로 세팅한 그대로
//	// GetCurrentPCClassInfo 의 BaseBPClass 로 세팅해서는 안됨. 기본 spawn 은 네이티브 기반으로 하고 BaseBPClass 는 PCClassInfo overriding 전에 적용을 한다.
//#if WITH_EDITOR
//	checkSlow(!IsDefaultPawnTestBP()); // 정식 맵에서는 걸리면 안된다. 일부 테스트 맵에서는 걸릴 수 있음. 그런 경우는 그냥 넘어가도록.
//#endif
//
//#if WITH_EDITOR
//	if (GIsEditor)
//	{ // UIManager 를 맵을 새로 로딩할 때마다 새로 생성하는데 에디터에서는 안먹힐 때가 있는 거 같다.
//		UB2UIManager::EditorGetNewInstance();
//	}
//	else
//	{// UIManager 가 static 한 구조로 만들어졌는데 언리얼 구조에서 위험성이 있으므로 맵 전환 시 destroy 를 한다. 계획대로라면 이 시점에서 UIManager 는 새로 생성하는 것이어야 함.
//		BII_CHECK(UB2UIManager::IsInstanceNull());
//	}
//#else
//	BII_CHECK(UB2UIManager::IsInstanceNull());
//#endif
//	UB2UIManager* UIMgr = UB2UIManager::GetInstance();
//	if (UIMgr)
//	{
//		UIMgr->Init(FLocalPlayerContext(GetWorld()->GetFirstLocalPlayerFromController()));
//	}
//
//	//UI독매니저 초기화
//	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
//	if (UIDocManager){
//		UIDocManager->Init(this);
//	}
//
//	// B2UIManager_InGameCombat 은 DJLegacy 인데 여전히 사용. 이름만 InGameCombat 이고 실제로는 ABladeIIGameMode 를 사용하는 모든 곳에 존재하겠군. SetCenterMessage 유틸땜에 필요하긴 함.
//	FActorSpawnParameters SpawnInfo;
//	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	SpawnInfo.ObjectFlags |= RF_Transient;
//	UIManager_InGameCombat = GetWorld()->SpawnActor<AB2UIManager_InGameCombat>(AB2UIManager_InGameCombat::StaticClass(), SpawnInfo);
//	if (UIManager_InGameCombat)
//	{
//		UIManager_InGameCombat->PreBeginPlay();
//	}
//
//	// GameModes that have their own custom GameRule already have set MyGameRule to their rule
//	// if not, you should set this value to DefaultGameRule
//	if (MyGameRule == nullptr)
//	{
//		auto* GameRule = new DefaultGameRule;
//		SetGameRule(GameRule);
//	}

	SubscribeEvents();
}

//void ABladeIIGameMode::UpdateManagedActorList()
//{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::UpdateManagedActorList"));
//
//	// 레벨 스트리밍이라도 한다면 PreBeginPlay 이후라도 따로 불러줄 필요가 생길 것.
//
//	RemoveAllManagedActors();
//
//	for (FActorIterator It(GetWorld()); It; ++It)
//	{
//		if ((*It)->Tags.Num() > 0)
//		{
//			AddTaggedActor((*It));
//		}
//
//		// 얘들 사이에 서로 BeginPlay 시점의 의존 관계가 있을 수 있어서 BeginPlay 시작 전에 미리 등록해 둔다.
//		auto* CurrSpawnPool = Cast<AB2MonsterSpawnPool>(*It);
//		if (CurrSpawnPool)
//		{
//			AddSpawnPool(CurrSpawnPool);
//			continue;
//		}
//
//		auto* CurrSED = Cast<AB2StageEventDirector>(*It);
//		if (CurrSED)
//		{
//			AddStageEventDirector(CurrSED);
//			continue;
//		}
//
//		auto* CurrWayPoint = Cast<AB2AutoWayPoint>(*It);
//		if (CurrWayPoint)
//		{
//			AddAutoWayPoint(CurrWayPoint);
//			continue;
//		}
//
//		auto* CurrAC = Cast<AB2ActiveCameraActor>(*It);
//		if (CurrAC)
//		{
//			AddActiveCameraActor(CurrAC);
//			continue;
//		}
//
//		auto* CurrTC = Cast<AB2TreasureChestBase>(*It);
//		if (CurrTC)
//		{
//			AddTreasureChest(CurrTC);
//			continue;
//		}
//
//		auto* CurrDO = Cast<AB2DestructibleLevelObjBase>(*It);
//		if (CurrDO)
//		{
//			AddDestructibleObject(CurrDO);
//			continue;
//		}
//
//		auto* CurrHomePoint = Cast<AB2HomePoint>(*It);
//		if (CurrHomePoint)
//		{
//			AddHomePoint(CurrHomePoint);
//			continue;
//		}
//
//		auto* CastedDialogTrigger = Cast<AB2DialogTriggerActorBase>(*It);
//		if (CastedDialogTrigger)
//		{
//			AddDialogTriggerActor(CastedDialogTrigger);
//			continue;
//		}
//	}
//
//	if (AutoWayPoints.Num() >= 2)
//		AutoWayPoints.Sort([](const AB2AutoWayPoint& a, const AB2AutoWayPoint& b){ return a.PassOrder < b.PassOrder; });
//
//	DecideAndSetActiveSpawnPool(); // 리스트 다 차면 이번 레벨에서 사용될 SpawnPool 하나를 찾아서 세팅.
//}
//
//void ABladeIIGameMode::RemoveAllManagedActors()
//{
//	SpawnPools.Empty();
//	EventDirectors.Empty();
//	AutoWayPoints.Empty();
//	ActiveCameraActors.Empty();
//	TreasureChests.Empty();
//	DestructibleObjects.Empty();
//	HomePoints.Empty();
//	TaggedActors.Empty();
//	DialogTriggers.Empty();
//}

void ABladeIIGameMode::SetDesiredPCClassEnum()
{
//	if (Utilities::FConnectionState::GetInstance().IsConnected())
//	{
//		// Main 을 사용하는 것은 기본맵 플레이할 때이고 Extra 맵의 경우는 이전에 마지막으로 플레이한 current 클래스를 세팅해야 함. AB2StageGameMode::SetDesiredPCClassEnum 참고.
//		CurrentPCClassEnum = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
//	}
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	else
//	{
//		// 연결 안 된 상태여도 개발 테스트 용 단독 플레이 모드도 있고 LocalCharacterData 에서는 로컬에 저장된 값을 로딩해 놓았을 수도 있어서 그 값을 사용.
//		CurrentPCClassEnum = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
//	}
//#endif
//
//	// 첨엔 뭔가 값이 있어야 안전함.
//	if (CurrentPCClassEnum == EPCClass::EPC_End)
//		CurrentPCClassEnum = DefaultPCClassEnum;
}

void ABladeIIGameMode::SubscribeEvents()
{ 
	if (bEventsSubscribed)
		return;

	//// 대체로 UI 관련 이벤트 받기 위함.
	//Issues.Add(BeginGlobalSceneBlurClass<float>::GetInstance().Subscribe2(
	//	[this](float f)
	//	{
	//		this->BeginGlobalSceneBlur(f);
	//	}
	//));
	//Issues.Add(EndGlobalSceneBlurClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{ 
	//		this->EndGlobalSceneBlur(); 
	//	}
	//));
	//Issues.Add(RestartGameClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->RestartGame(); 
	//	}
	//));
	//Issues.Add(GoToTitleClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->GoToTitle();
	//	}
	//));
	//Issues.Add(GoToDLCMapClass<>::GetInstance().Subscribe2(
	//	[this]()
	//{
	//	OpenBladeIIDLCFrontCommon(this);
	//}
	//));
	//Issues.Add(GoToVillageClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->GoToVillage(); 
	//	}
	//));
	//Issues.Add(GoToVillageDefeatedClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->GoToVillageDefeated();
	//	}
	//));
	//Issues.Add(GoToMapClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->GoToMap(); 
	//	}
	//));
	//Issues.Add(GoToTutorialClass<bool>::GetInstance().Subscribe2(
	//	[this](bool bIsSkip)
	//	{
	//		this->GoToTutorial(bIsSkip);
	//	}
	//));
	//Issues.Add(GoToDeferredStageClass<int32>::GetInstance().Subscribe2(
	//	[this](int32 InNextClientStageId)
	//	{
	//		this->GoToDeferredStage(InNextClientStageId);
	//	}
	//));
	//Issues.Add(RequestResurrectClass<EResurrectBuffType>::GetInstance().Subscribe2(
	//	[this](EResurrectBuffType rb)
	//	{
	//		this->RequestResurrect(rb); 
	//	}
	//));
	//Issues.Add(DeliveryStartStageClass<FB2StartStageInfo>::GetInstance().Subscribe2(
	//	[this](const FB2StartStageInfo& Info)
	//	{
	//		AB2StageManager::GetCacheStageKeepEssentialData().ResponseStartStage(this, Info); 
	//	}
	//));
	//Issues.Add(DeliveryStartCounterDungeonClass<FB2ResponseStartCounterDungeon>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseStartCounterDungeon& Info)
	//	{
	//		AB2StageManager::GetCacheStageKeepEssentialData().ResponseStartCounterDungeon(this, Info->play_token);
	//		GuildBuffSetttingClass<const TArray<b2network::B2GuildSkillPtr>&>::GetInstance().Signal(Info->guild_skills);
	//	}
	//));
	//Issues.Add(DeliveryForceStartCounterDungeonClass<FB2ResponseForceStartCounterDungeonPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseForceStartCounterDungeonPtr& Info)
	//	{
	//		AB2StageManager::GetCacheStageKeepEssentialData().ResponseStartCounterDungeon(this, Info->play_token);
	//	}
	//));
	//Issues.Add(DeliveryStartHeroTowerClass<FB2ResponseStartHeroTowerPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseStartHeroTowerPtr& msgPtr)
	//	{
	//		AB2StageManager::GetCacheStageKeepEssentialData().ResponseStartHeroTower(this, msgPtr);
	//	}
	//));
	//Issues.Add(DeliveryStartDimensionClass<FB2ResponseStartDimensionPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseStartDimensionPtr& msgPtr)
	//{
	//	AB2StageManager::GetCacheStageKeepEssentialData().ResponseStartDimension(this, msgPtr);
	//}
	//));
	//Issues.Add(OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Subscribe2(
	//	[this](EPreLoadingScreenType PLType, EB2GameMode InGameMode)
	//	{
	//		this->OpenPreLoadingScreen(PLType, InGameMode);
	//	}
	//));
	//Issues.Add(ClosePreLoadingScreenClass<EPreLoadingScreenType>::GetInstance().Subscribe2(
	//	[this](EPreLoadingScreenType PLType)
	//	{
	//		this->ClosePreLoadingScreen(PLType);
	//	}
	//));
	//Issues.Add(ClosePreLoadingScreenAllClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->ClosePreLoadingScreenAll();
	//	}
	//));
	//Issues.Add(SpawnPoolMobSpawnClass<ABladeIICharacter*, const FMobSpawnedBirthplaceInfo&>::GetInstance().Subscribe2(
	//	[this](ABladeIICharacter* ch, const FMobSpawnedBirthplaceInfo& bpi)
	//	{
	//		this->NotifySpawnPoolMobSpawn(ch, bpi);
	//	}
	//));
	//Issues.Add(SpawnPoolCompleteClass<AB2MonsterSpawnPool*>::GetInstance().Subscribe2(
	//	[this](AB2MonsterSpawnPool* sp)
	//	{
	//		this->NotifySpawnPoolComplete(sp);
	//	}
	//));
	//Issues.Add(SpawnPoolSingleWaveStartClass<AB2MonsterSpawnPool*, int32>::GetInstance().Subscribe2(
	//	[this](AB2MonsterSpawnPool* sp, int32 i)
	//	{
	//		this->NotifySpawnPoolSingleWaveStart(sp, i);
	//	}
	//));
	//Issues.Add(SpawnPoolSingleWaveCompleteClass<AB2MonsterSpawnPool*, int32>::GetInstance().Subscribe2(
	//	[this](AB2MonsterSpawnPool* sp, int32 i)
	//	{
	//		this->NotifySpawnPoolSingleWaveComplete(sp, i);
	//	}
	//));
	//Issues.Add(SpawnPoolWaveMobDeadClass<AB2MonsterSpawnPool*, ABladeIICharacter*, int32, int32>::GetInstance().Subscribe2(
	//	[this](AB2MonsterSpawnPool* sp, ABladeIICharacter* ch, int32 i1, int32 i2)
	//	{
	//		this->NotifySpawnPoolWaveMobDead(sp, ch, i1, i2);
	//	}
	//));
	//Issues.Add(SpawnPoolWaveMobPhaseChangeClass<AB2MonsterSpawnPool*, ABladeIICharacter*, ABladeIICharacter*, int32, int32, int32>::GetInstance().Subscribe2(
	//	[this](AB2MonsterSpawnPool* sp, ABladeIICharacter* chnew, ABladeIICharacter* chold, int32 i1, int32 i2, int32 i3)
	//	{
	//		this->NotifySpawnPoolWaveMobPhaseChange(sp, chnew, chold, i1, i2, i3);
	//	}
	//));
	//Issues.Add(SpawnPoolSummonedMobDeadClass<AB2MonsterSpawnPool*, ABladeIICharacter*>::GetInstance().Subscribe2(
	//	[this](AB2MonsterSpawnPool* sp, ABladeIICharacter* ch)
	//	{
	//		this->NotifySpawnPoolSummonedMobDead(sp, ch);
	//	}
	//));
	//Issues.Add(PleaseForgetMeClass<ABladeIICharacter*>::GetInstance().Subscribe2(
	//	[this](ABladeIICharacter* CharAboutTobeDestroyed)
	//	{
	//		this->NotifyForgetACharacter(CharAboutTobeDestroyed);
	//	}
	//));
	//Issues.Add(StageEventSceneBeginClass<AB2StageEventDirector*, EStageEvent>::GetInstance().Subscribe2(
	//	[this](AB2StageEventDirector* ed, EStageEvent et)
	//	{
	//		this->NotifyStageEventSceneBegin(ed, et);
	//	}
	//));
	//Issues.Add(StageEventSceneEndClass<AB2StageEventDirector*>::GetInstance().Subscribe2(
	//	[this](AB2StageEventDirector* ed)
	//	{
	//		this->NotifyStageEventSceneEnd(ed);
	//	}
	//));
	//Issues.Add(UIWidgetOpenCompleteClass<FName>::GetInstance().Subscribe2(
	//	[this](FName un)
	//	{
	//		this->NotifyUIWidgetOpenComplete(un);
	//	}
	//));
	//Issues.Add(CancelOrStopRepeatBattleClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->CancelOrStopRepeatBattle();
	//	}
	//));
	//Issues.Add(SetPlayerAutoTypeClass<int32>::GetInstance().Subscribe2(
	//	[this](int32 InType)
	//{
	//	this->SetPlayerAutoType(InType);
	//}
	//));
	//Issues.Add(GiveUpGameClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->GiveUpGame();
	//	}
	//));
	//Issues.Add(UpdateChatUIClass<bool>::GetInstance().Subscribe2(
	//	[this](const bool IsScrollEnd)
	//	{
	//		this->UpdateChatUI(IsScrollEnd);
	//	}
	//));
	//Issues.Add(UpdateAdminMessageClass<const FString&, const int32>::GetInstance().Subscribe2(
	//	[this](const FString& InMessage, const int32 InDisplayTime)
	//	{
	//		this->UpdateAdminMessage(InMessage, InDisplayTime);
	//	}
	//));
	//Issues.Add(UpdateChatSinglePopupClass<const FB2ChatMessage&>::GetInstance().Subscribe2(
	//	[this](const FB2ChatMessage& cm)
	//	{
	//		this->TryOpenChatSinglePopup(cm);
	//	}
	//));
	//Issues.Add(EnterShopClass<int32>::GetInstance().Subscribe2(
	//	[this](int32 nWhereShop)
	//	{
	//		this->EnterShop(nWhereShop);
	//	}
	//));
	//Issues.Add(EnsureStageEventTimeDilationClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->EnsureStageEventTimeDilation();
	//	}
	//));
	//Issues.Add(OnQuestCompleteNotifyClass<int32>::GetInstance().Subscribe2(
	//	[this](int32 CompleteQuestSlot)
	//	{
	//		this->OnQuestCompleteNotify(CompleteQuestSlot);
	//	}
	//));
	//Issues.Add(TogglePlayerAutoStateClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->ToggleLocalPlayerAutoState();
	//	}
	//));
	//Issues.Add(ShowMissionDialogClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->ShowMissionDialog();
	//	}
	//));
	//Issues.Add(GuildBuffSetttingClass<const TArray<b2network::B2GuildSkillPtr>&>::GetInstance().Subscribe2(
	//	[this](const TArray<b2network::B2GuildSkillPtr>& SkillInfo)
	//	{
	//		this->SetGuildBuff(SkillInfo);
	//	}
	//));
	//Issues.Add(StageBeginClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->OnStageBegin();
	//	}
	//));
	//Issues.Add(data_trader::ContentsModeStateNotifyClass<int32, int32, int32>::GetInstance().Subscribe2(
	//	[this](int32 mode, int32 prev_state, int32 current_state)
	//{
	//	this->OnReceiveContentsModeStateNotify(mode, prev_state, current_state);
	//}
	//));
	//Issues.Add(SetupLoadingScreenImageClass<bool, bool, bool, const TCHAR*, EB2LoadingMessageType>::GetInstance().Subscribe2(
	//	[this](bool bSetImagePersistent, bool bInDisplayProgress, bool bInDisplayIndicator, const TCHAR* InOptionalImageName, EB2LoadingMessageType InLoadingMessageType)
	//	{
	//		this->SetupLoadingScreenImageCommon(bSetImagePersistent, bInDisplayProgress, bInDisplayIndicator, InOptionalImageName, InLoadingMessageType);
	//	}
	//));
	//Issues.Add(ConditionalWaitForLoadingScreenClass<>::GetInstance().Subscribe2(
	//	[this]()
	//	{
	//		this->ConditionalWaitForLoadingScreen();
	//	}
	//));

	bEventsSubscribed = true; // SubsResponseGetQuestRewardcribeEvents 를 했다면 어찌어찌 더미로 생성되는 GameMode 는 아닐 것.
}

//void ABladeIIGameMode::UnsubscribeEvents()
//{
//	Issues.Empty();
//
//	bEventsSubscribed = false;
//}

void ABladeIIGameMode::DecideAndSetActiveSpawnPool(bool bFallBackToLowerDifficulty)
{ // 특정 게임모드에서나 (StageGameMode 기반) 현재 상황에서의 의미있는 SpawnPool 로 세팅이 될 것.
	//TheActiveSpawnPool = (SpawnPools.Num() > 0) ? SpawnPools[0] : NULL;
}

//AActor* ABladeIIGameMode::FindPlayerStartForCurrStage(AController* InController)
//{
//	return FindPlayerStart(InController); // 거의 StageGameMode 용이다. 사정이 있어서 여기로 끌고 옴.
//}
//
void ABladeIIGameMode::RestartGame()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::RestartGame"));

//	// AB2StageGameMode::RestartGame 에서는 기능을 다르게 정의해서 사용하고 있으니 대부분의 경우는 그쪽을 확인.
//#if WITH_EDITOR
//	if (GIsEditor)
//	{
//		// 에디터에서는 단지 Play 세션을 끝낸다.
//		GEditor->RequestEndPlayMap();
//		return;
//	}
//#endif
	Super::RestartGame();
}

void ABladeIIGameMode::GoToTitle()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::GoToTitle"));

	//OpenBladeIILobbyCommon(this);

	FB2NetworkBridge::GetInstance()->Shutdown();
}

void ABladeIIGameMode::GoToTutorial(bool bIsSkip)
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2TutorialGameMode::GoToTutorial"));

	//TutorialManager::GetInstance().SetSkipNewTutorial(bIsSkip);

	// 반복되는 코드들이 있어서 한데 모아놓은 곳에 있는 거 사용.
	//StartBladeIIGameTutorial(this);
}

void ABladeIIGameMode::GoToVillage()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::GoToVillage"));

	//OpenPreLoadingScreen(EPreLoadingScreenType::PLST_GoToLobby, EB2GameMode::Lobby); // 로비로 돌아갈 때 로딩 스크린 관련해서 크래쉬가 좀 있는데 이걸 이 시점에서 미리 한번 로딩해주는 게 안전하다는 가설이 있어서 해 봄..

	//FLobbySceneManager::DeferredRegistChangeLobbyScene([](){
	//	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_LobbyMain);
	//});
	//	
	//OpenBladeIILobbyCommon(this); // 씬 전환 예약 후 본격 로비 맵 로딩
}

void ABladeIIGameMode::GoToVillageDefeated()
{
	GoToVillage(); // 하위 클래스에서 필요에 따라 다르게 쓰도록..
}

void ABladeIIGameMode::GoToMap()
{
	GoToVillage();
}


void ABladeIIGameMode::GoToDeferredStage(int32 InNextClientStageId)
{

}

void ABladeIIGameMode::GoToNextArea()
{
	//do nothing
}

void ABladeIIGameMode::EnterShop(int32 nWhereShop)
{
	// 일단 LobbyGameMode 에서.. 인게임에서도 상점 진입을 해야 할 수 있음.
}

float ABladeIIGameMode::GetPVEProportionalConstant()
{
	UB2CombatConstantInfo* CombatConstantInfo = StaticFindCombatConstantInfo(this);
	if (CombatConstantInfo)
		return CombatConstantInfo->GetPVEProportionalConstant(GetB2GameModeType());

	return CombatStatEval::GetDefaultPVEProportionalConstant();
}

float ABladeIIGameMode::GetHealthRateByGameModeType()
{
	UB2CombatConstantInfo* CombatConstantInfo = StaticFindCombatConstantInfo(this);
	if (CombatConstantInfo)
		return CombatConstantInfo->GetHPCoefficient(GetB2GameModeType());

	return CombatStatEval::GetDefaultCombatCoefficient();
}

float ABladeIIGameMode::GetPVPMaxDamageRatioByGameModeType()
{
	UB2CombatConstantInfo* CombatConstantInfo = StaticFindCombatConstantInfo(this);
	if (CombatConstantInfo)
		return CombatConstantInfo->GetPVPMaxDamageRatio(GetB2GameModeType());

	return CombatStatEval::GetDefaultPVPMaxDamageRatio();
}

float ABladeIIGameMode::GetPVPDamageScaleByGameModeType()
{
	UB2CombatConstantInfo* CombatConstantInfo = StaticFindCombatConstantInfo(this);
	if (CombatConstantInfo)
		return CombatConstantInfo->GetPVPDamageScale(GetB2GameModeType());

	return CombatStatEval::GetDefaultPVPDamageScale();
}

float ABladeIIGameMode::GetAttackCoefficient()
{
	UB2CombatConstantInfo* CombatConstantInfo = StaticFindCombatConstantInfo(this);
	if (CombatConstantInfo)
		return CombatConstantInfo->GetAttackCoefficient(GetB2GameModeType());

	return CombatStatEval::GetDefaultCombatCoefficient();
}
float ABladeIIGameMode::GetDefenseCoefficient()
{
	UB2CombatConstantInfo* CombatConstantInfo = StaticFindCombatConstantInfo(this);
	if (CombatConstantInfo)
		return CombatConstantInfo->GetDefenseCoefficient(GetB2GameModeType());

	return CombatStatEval::GetDefaultCombatCoefficient();
}

void ABladeIIGameMode::RecacheLevelResources()
{
	// 각 레벨 블루프린트 (ALevelScriptActor) 에서 리소스 캐싱 (바인딩) 동작을 특정 시점에 호출하기 위함.
	// 예를 들어 BeginPlay 시점에 주로 하는 걸 다른 타이밍에도 실행하기 위해.

	if (GEngine && GLog)
	{
		GEngine->HandleCeCommand(GetWorld(), *(RECACHE_LEVEL_RESOURCE_CENAME.ToString()), *GLog);
	}
}

UB2PCClassInfo* ABladeIIGameMode::GetCurrentPCClassInfo()
{
	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (CurrentPCClassEnum != EPCClass::EPC_End && PCBox)
	{
		return PCBox->GetSingleClassInfo(CurrentPCClassEnum);
	}
	return NULL;
}

ABladeIIPlayer* ABladeIIGameMode::SpawnPCClassInfoPlayer(class UWorld* SpawnWorld, EPCClass InClass,
	const FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters, bool bSpawnAsDummyPuppet, UClass* OverridePawnClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::SpawnPCClassInfoPlayer"));

	//FActorSpawnParameters FinalSpawnParameter = SpawnParameters;
	//FinalSpawnParameter.bDeferPostSpawnInitialize = true; // bDeferConstruction 관련 처리는 밖에서 하지만 bDeferPostSpawnInitialize 는 여기선 무조건 사용해야. 

	//UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	//if (SpawnWorld && InClass != EPCClass::EPC_End && PCBox) // Even for the EPC_End, we can still do some..
	//{
	//	UB2PCClassInfo* ClassInfo = PCBox->GetSingleClassInfo(InClass);
	//	if (ClassInfo)
	//	{
	//		// It will be picked up by player's BeginPlay.
	//		CurrentPCClassEnum = InClass; 

	//		// Main spawn operation. Other spawn handling (e.g. FinishSpawning) needs to be done outside of here depend on spawn parameters.
	//		UClass* PawnClass = DefaultPawnClass;
	//		if (OverridePawnClass != nullptr)
	//			PawnClass = OverridePawnClass;
	//		
	//		ABladeIIPlayer* SpawnedActor = Cast<ABladeIIPlayer>(SpawnWorld->SpawnActor(PawnClass, &SpawnTransform, FinalSpawnParameter));
	//		
	//		// Rest of overriding by PCClassInfo object are done by BladeIIPlayer's BeginPlay and OverrideByPCClassInfo

	//		if (SpawnedActor)
	//		{
	//			SpawnedActor->FinalizePCClassInfoSpawn(bSpawnAsDummyPuppet); // DummyPuppet spawn 시 InGameInfo 사용 없이 진행하도록 인자를 넘겨준다.
	//			SpawnWorld->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, FinalSpawnParameter); // bDeferPostSpawnInitialize 에 따른 처리.
	//			return SpawnedActor;
	//		}
	//	}
	//}
	BII_CHECK(0);
	return NULL;
}

ABladeIIPlayer* ABladeIIGameMode::StaticSpawnPCClassInfoPlayer(UObject* WorldContextObject, EPCClass InClass, const FTransform& SpawnTransform, 
	ESpawnActorCollisionHandlingMethod CollisionHandlingOverride /*= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn*/, bool bDeferConstruction /*= true*/, AActor* InOwner /*= nullptr*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StaticSpawnPCClassInfoPlayer"));

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = InOwner;
	SpawnParam.bDeferConstruction = bDeferConstruction; // 이걸 true 로 하면 이거 호출 이후에 FinishSpawning 을 따로 불러주어야 하는 번거로움이 있지만 일반적인 게임플레이 상황에서의 로컬 플레이어 캐릭터라면 BeginPlay 시점에 PlayerController 에 의해 Possess 된 상태여야 할 것이므로 필요함.
	SpawnParam.SpawnCollisionHandlingOverride = CollisionHandlingOverride;

	ABladeIIGameMode* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (B2GameMode && World)
	{
		return B2GameMode->SpawnPCClassInfoPlayer(World, InClass, SpawnTransform, SpawnParam);
	}
	BII_CHECK(0);
	return NULL;
}

void ABladeIIGameMode::HookOnSpawnDefaultPawn(APawn* InSpawned)
{
	// GameMode 시작 시 default pawn spawn 시점에만 불림. BeginPlay 및 컴포넌트들 register 이전.
//	ABladeIIPlayer* SpawnedB2Player = Cast<ABladeIIPlayer>(InSpawned);
//	if (SpawnedB2Player)
//	{
//		SpawnedB2Player->FinalizePCClassInfoSpawn();
//		
//		// 로딩 진행도 집계.. FinalizePCClassInfoSpawn 도중 BaseBP 로딩하면서 뭔가 앞서 요청한 것들이 flush 되는 듯 많은 시간이 소모..
//		B2GMLoadingProgCollector::StepCustom(0.4f);
//
//#if WITH_EDITOR // 아직 PCClass 가 없는 테스트 상황에서는.. CharacterType 을 임의로 준다. 이거라도 안 넣어주면 크래쉬 나는 곳이 너무 많음.
//		if (IsDefaultPawnTestBP() && CurrentPCClassEnum == EPCClass::EPC_End)
//		{
//			SpawnedB2Player->CharacterType = ECharacterType::ECT_Gladiator;
//		}
//#endif
//	}
}

ABladeIIPlayer* ABladeIIGameMode::SpawnPlayerCharAsPuppet(UObject* WorldContextObject, EPCClass InClass, const FTransform& InSpawnTransform, bool bPossessByAIController, UClass* OverridePawnClass)
{	
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::SpawnPlayerCharAsPuppet"));

	ABladeIIGameMode* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	UWorld* TheWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (InClass != EPCClass::EPC_End && B2GameMode && TheWorld)
	{		
		FActorSpawnParameters SpawnParam;
		SpawnParam.bDeferConstruction = false; // 여긴 DeferContruction 따위 할 필요가 없겠지..
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABladeIIPlayer* SpawnedPuppet = B2GameMode->SpawnPCClassInfoPlayer(TheWorld, InClass, InSpawnTransform, SpawnParam, true, OverridePawnClass); // SpawnAsDummyPuppet 인자를 주면 InGameInfo 사용 없이 spawn 을 진행하게 될 것.

		if (SpawnedPuppet)
		{
			if (bPossessByAIController)
			{
				// APawn::PostInitializeComponents 에서 조건에 따라 생성을 하는데, 
				// 기본 AutoPossessAI 를 Spawned 로 하기에는 PlayerController 에 possess 되는 경우를 생각하면 낭비이고 
				// Puppet spawn 시에는 여하간 중력을 받으려면 기본 AI controller 가 필요하니 별도로 불러준다.
				SpawnedPuppet->SpawnDefaultController();
			}
			return SpawnedPuppet;
		}
	}
	BII_CHECK(0);
	return NULL;
}

void ABladeIIGameMode::FinishSpawnByPCClassInfo()
{
	CurrentPCClassEnum = EPCClass::EPC_End;
}

bool ABladeIIGameMode::TagPlayerCharacter(AActor* Player)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::TagPlayerCharacter"));

	//BII_CHECK(Player);

	//auto* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//auto* B2Player = Cast<ABladeIIPlayer>(B2PC->GetPawn());
	//UB2SkillRTManager* SkillRTManager = B2Player ? B2Player->GetSkillRTManager() : NULL;
	//
	//BII_CHECK(SkillRTManager && B2Player);
	//return SkillRTManager->TagToOther((int32)B2Player->TagAttackMode, B2Player->TagAttackMode == ETagAttackMode::EAM_Disable ? EAttackState(EAttackState::ECS_None) : B2Player->GetAttackState());
	return true;
}

bool ABladeIIGameMode::RequestResurrect(EResurrectBuffType InResurrectBuffType)
{
	return false; // 하위 게임 모드 클래스에서 구현.
}

// 진동 로비에선 플레이어 없으니 이거쓰자. 다른데서도 써두됨.
void ABladeIIGameMode::ForceFeedback()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIGameMode_ForceFeedback);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::ForceFeedback"));

	//APlayerController* PC = UGameplayStatics::GetLocalPlayerController(this);
	
	//if (PC)
	//{
	//	UB2SomeInfo* SomeInfo = StaticFindSomeInfo(this);
	//	if (IsVibration() && SomeInfo && SomeInfo->GetForceFeedbackEffect())
	//	{
	//		PC->ClientPlayForceFeedback(SomeInfo->GetForceFeedbackEffect() , false, FName());
	//	}
	//}
}

void ABladeIIGameMode::NotifySpawnPoolMobSpawn(class ABladeIICharacter* SpawnedMob, const FMobSpawnedBirthplaceInfo& InBirthplaceInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolMobSpawn"));

	//checkSlow(!InBirthplaceInfo.Birthplace || InBirthplaceInfo.Birthplace == TheActiveSpawnPool);

	//if (InBirthplaceInfo.Birthplace && SpawnedMob)
	//{
	//	SpawnedMob->SetBirthPlace(InBirthplaceInfo);
	//}
}

void ABladeIIGameMode::NotifySpawnPoolComplete(class AB2MonsterSpawnPool* CompletedSpawnPool)
{
	//checkSlow(!CompletedSpawnPool || CompletedSpawnPool == TheActiveSpawnPool);
}

void ABladeIIGameMode::NotifySpawnPoolSingleWaveStart(AB2MonsterSpawnPool* StartedSpawnPool, int32 WaveNum)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolSingleWaveStart"));

	//checkSlow(!StartedSpawnPool || StartedSpawnPool == TheActiveSpawnPool);

	//for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
	//{
	//	AB2StageEventDirector* CurrDirector = EventDirectors[DI];
	//	if (CurrDirector)
	//	{
	//		CurrDirector->NotifySpawnPoolSingleWaveStart(WaveNum);
	//	}
	//}
}

void ABladeIIGameMode::NotifySpawnPoolSingleWaveComplete(AB2MonsterSpawnPool* CompletedSpawnPool, int32 WaveNum)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolSingleWaveComplete"));

	//checkSlow(!CompletedSpawnPool || CompletedSpawnPool == TheActiveSpawnPool);

	//for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
	//{
	//	AB2StageEventDirector* CurrDirector = EventDirectors[DI];
	//	if (CurrDirector)
	//	{
	//		CurrDirector->NotifySpawnPoolSingleWaveComplete(WaveNum);
	//	}
	//}
}

void ABladeIIGameMode::NotifySpawnPoolWaveMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob, int32 WaveNum, int32 MobIndex)
{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolWaveMobDead"));
//
//	BII_CHECK(DyingMob && DyingMob->IsValidObj());
//	checkSlow(!SpawnPool || SpawnPool == TheActiveSpawnPool);
//
//	for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
//	{
//		AB2StageEventDirector* CurrDirector = EventDirectors[DI];
//		if (CurrDirector)
//		{
//			CurrDirector->NotifySpawnPoolSingleMobDead(DyingMob, WaveNum, MobIndex);
//		}
//	}
//
//	// SpawnPool 보다 StageEventDirector 에 먼저 Notify 를 줄 필요가 있다. 
//	// 그래야 에서 MobDying 이벤트 처리 시 WaveNum 과 ObjIndex 로 DyingMonster 를 가져올 수가 있음.
//	// 물론 이 경우 delay 없이 바로 show 를 시작해야 함. 그게 아니라면 DyingMonstser 를 어떤 식으로든 캐싱해서 써야 함.
//
//	if (SpawnPool)
//	{
//		SpawnPool->OnSpawnedMonsterDead(DyingMob, WaveNum, MobIndex, false);
//	}
}

void ABladeIIGameMode::NotifySpawnPoolWaveMobPhaseChange(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* ChangedMob, class ABladeIICharacter* DestroyedPrevRef, int32 WaveNum, int32 MobObjIndex, int32 NewPhase)
{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolWaveMobPhaseChange"));
//
//	// DestroyedPrevRef 는 NULL 이 아닌 경우라도 IsValidObj 하지 않을 것임.
//
//	BII_CHECK(ChangedMob && ChangedMob->IsValidObj());
//	checkSlow(!SpawnPool || SpawnPool == TheActiveSpawnPool);
//
//	for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
//	{
//		AB2StageEventDirector* CurrDirector = EventDirectors[DI];
//		if (CurrDirector)
//		{
//			CurrDirector->NotifySpawnPoolSingleMobPhaseChange(ChangedMob, DestroyedPrevRef, WaveNum, MobObjIndex, NewPhase);
//		}
//	}
}

void ABladeIIGameMode::NotifySpawnPoolSummonedMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolSummonedMobDead"));
//
//	checkSlow(!SpawnPool || SpawnPool == TheActiveSpawnPool);
//
//	// 여기선 StageEventDirector 쪽 통지는 필요없겠음. 만일 필요해진다면 아마도 SpawnPool 보다는 먼저 통지.
//
//	if (SpawnPool)
//	{
//		SpawnPool->OnSpawnedMonsterDead(DyingMob, -1, -1, true); // 소환이면 WaveNum 등은 필요없음.
//	}
}

void ABladeIIGameMode::NotifyForgetACharacter(class ABladeIICharacter* CharacterAboutToBeDestroyed)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifyForgetACharacter"));

//	// Notify**Dead 가 게임 플레이상의 죽음을 의미한다면 이건 언리얼 오브젝트 차원의 제거 직전임. 넘어온 오브젝트를 참조하는 게 있다면 모두 NULL out.
//
//	BII_CHECK(CharacterAboutToBeDestroyed && CharacterAboutToBeDestroyed->IsValidObj()); // 아직은 Valid 해야겠지..
//
//	for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
//	{
//		AB2StageEventDirector* CurrDirector = EventDirectors[DI];
//		if (CurrDirector)
//		{
//			CurrDirector->NotifyForgetACharacter(CharacterAboutToBeDestroyed);
//		}
//	}
}

//void ABladeIIGameMode::StopAllStageEventDirector(class AB2StageEventDirector* skipDirector)
//{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StopAllStageEventDirector"));
//
//	for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
//	{
//		AB2StageEventDirector* CurrDirector = EventDirectors[DI];
//		if (CurrDirector && CurrDirector->IsPlayingShow() && CurrDirector != skipDirector)
//		{
//			CurrDirector->CustomFinishShow();
//		}
//	}
//}

void ABladeIIGameMode::NotifyStageEventSceneBegin(class AB2StageEventDirector* BegunDirector, EStageEvent EventType)
{
}
void ABladeIIGameMode::NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector)
{
}

void ABladeIIGameMode::NotifyUIWidgetOpenComplete(FName InOpenedUI)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifyUIWidgetOpenComplete"));

	//if (AB2StageEventDirector::IsStageEventRelatedUI(InOpenedUI))
	//{
	//	for (AB2StageEventDirector* CurrSED : EventDirectors)
	//	{
	//		if (CurrSED)
	//		{
	//			CurrSED->NotifyStageEventUIOpenComplete(InOpenedUI);
	//		}
	//	}
	//}
}

void ABladeIIGameMode::AddSpawnPool(class AB2MonsterSpawnPool* InNewSpawnPool)
{
	//if (InNewSpawnPool)
	//{
	//	SpawnPools.AddUnique(InNewSpawnPool);
	//	InNewSpawnPool->SetCurrentGameMode(this);
	//}
}

void ABladeIIGameMode::AddStageEventDirector(class AB2StageEventDirector* InNewEventDirector)
{
	//if (InNewEventDirector)
	//{
	//	EventDirectors.AddUnique(InNewEventDirector);
	//}
}

void ABladeIIGameMode::AddActiveCameraActor(class AB2ActiveCameraActor* InNewActiveCameraActor)
{
	//if (InNewActiveCameraActor)
	//{
	//	ActiveCameraActors.AddUnique(InNewActiveCameraActor);
	//}
}

class AB2ActiveCameraActor* ABladeIIGameMode::FindActiveCameraActorByTag(FName ACATagName)
{
	//for (AB2ActiveCameraActor* pACA : ActiveCameraActors)
	//{
	//	if (pACA->ActorHasTag(ACATagName))
	//	{
	//		return pACA;
	//	}
	//}

	return nullptr;
}

void ABladeIIGameMode::AddAutoWayPoint(AB2AutoWayPoint* InNewWayPoint)
{
	if (InNewWayPoint)
	{
		AutoWayPoints.AddUnique(InNewWayPoint);
	}
}

void ABladeIIGameMode::AddTreasureChest(class AB2TreasureChestBase* InNewTreasureChest)
{
	if (InNewTreasureChest)
	{
		TreasureChests.AddUnique(InNewTreasureChest);
	}
}

void ABladeIIGameMode::RemoveTreasureChest(class AB2TreasureChestBase* InRemoveTreasureChest)
{
	if (InRemoveTreasureChest)
	{
		TreasureChests.Remove(InRemoveTreasureChest);
	}
}

void ABladeIIGameMode::AddDestructibleObject(AB2DestructibleLevelObjBase* InNewDestructibleObject)
{
	//if (InNewDestructibleObject)
	//{
	//	DestructibleObjects.AddUnique(InNewDestructibleObject);
	//}
}

void ABladeIIGameMode::AddHomePoint(AB2HomePoint* InNewHomePoint)
{
	HomePoints.AddUnique(InNewHomePoint);
}

AB2HomePoint* ABladeIIGameMode::GetHomePoint() const
{
	if (HomePoints.Num() == 0)
		return nullptr;

	return HomePoints[0];
}

void ABladeIIGameMode::RemoveDestructibleObject(class AB2DestructibleLevelObjBase* InRemoveDestructibleObject)
{
//	if (InRemoveDestructibleObject)
//	{
//		DestructibleObjects.Remove(InRemoveDestructibleObject);
//
//}
}

void ABladeIIGameMode::AddDialogTriggerActor(class AB2DialogTriggerActorBase* InDlgTriggerActor)
{
	//if (InDlgTriggerActor)
	//{
	//	DialogTriggers.AddUnique(InDlgTriggerActor);
	//}
}

void ABladeIIGameMode::AddTaggedActor(class AActor* InNewTaggedActor)
{
	if (InNewTaggedActor)
	{
		TaggedActors.AddUnique(InNewTaggedActor);
	}
}

AActor* ABladeIIGameMode::GetTaggedActor(FName TagName)
{
	for (auto* TD : TaggedActors)
	{
		if (TD->ActorHasTag(TagName))
			return TD;
	}

	return NULL;
}

void ABladeIIGameMode::GetSingleWaveMobList(int32 WaveNum, TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /*= true*/)
{
	//if (TheActiveSpawnPool) // 전체 SpawnPool 을 iteration 할 필요가 없어야 한다.
	//{
	//	TArray<class ABladeIICharacter*> CurrList;
	//	TheActiveSpawnPool->GetWaveMobList(WaveNum, CurrList, bOnlyLiveMobs);
	//	OutList.Append(CurrList);
	//}
}

void ABladeIIGameMode::GetAllWaveMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /*= true*/)
{
//	if (TheActiveSpawnPool) // 전체 SpawnPool 을 iteration 할 필요가 없어야 한다.
//	{
//		TArray<class ABladeIICharacter*> CurrList;
//		TheActiveSpawnPool->GetAllWaveMobList(CurrList, bOnlyLiveMobs);
//		OutList.Append(CurrList);
//	}
}

void ABladeIIGameMode::GetAllSummonedMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /*= true*/)
{
	//if (TheActiveSpawnPool) // 전체 SpawnPool 을 iteration 할 필요가 없어야 한다.
	//{
	//	TArray<class ABladeIICharacter*> CurrList;
	//	TheActiveSpawnPool->GetAllSummonedMobList(CurrList, bOnlyLiveMobs);
	//	OutList.Append(CurrList);
	//}
}

void ABladeIIGameMode::GetAllCharacterList(TArray<class ABladeIICharacter*>& OutList)
{
	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	ABladeIICharacter* pPawn = Cast<ABladeIICharacter>(*Iterator);

	//	if (pPawn)
	//	{
	//		OutList.Add(pPawn);
	//	}
	//}
	for (TActorIterator<ABladeIICharacter> It(GetWorld()); It; ++It)
	{
		ABladeIICharacter* pPawn = *It;
		if (IsValid(pPawn))
		{
			OutList.Add(pPawn);
		}
	}

}

void ABladeIIGameMode::GetAllWorldMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /*= true*/)
{
//	GetAllWaveMobList(OutList, bOnlyLiveMobs);
//	GetAllSummonedMobList(OutList, bOnlyLiveMobs);
//
//#if WITH_EDITOR && !UE_BUILD_SHIPPING
//	// 에디터에선 테스트도 많이 하고 하니SpawnNPC 커맨드로 생성한 근본없는 몹들도 타겟 체크해 준다.
//	for (FActorIterator It(GetWorld()); It; ++It) // ActorIterator 이므로 테스트 외엔 무리.
//	{
//		ABladeIICharacter* Actor = Cast<ABladeIICharacter>(*It);
//		if (Actor && Actor->IsValidObj())
//		{
//			ABladeIIPlayer* Player = Cast<ABladeIIPlayer>(*It);
//			if (Player == NULL && (!bOnlyLiveMobs || Actor->IsAlive()) && Actor->CanBeTarget())
//			{
//				OutList.AddUnique(Actor);
//			}
//		}
//	}
//#endif
}

bool ABladeIIGameMode::ApplyActiveCameraIfAny(float BlendInTimeOverride /*= -1.0f*/)
{ // StageEventDirector 쪽에서 연출 장면 종료 후에 플레이어 캐릭터 텔레포트에 따라 필요한 기능.
	
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::ApplyActiveCameraIfAny"));

	//for (int32 ACI = 0; ACI < ActiveCameraActors.Num(); ++ACI)
	//{
	//	if (ActiveCameraActors[ACI]->ManualTestBeginActiveCamera(BlendInTimeOverride) == true)
	//	{
	//		return true; // 하나 적용되었으면 더 볼 필요 없음.
	//	}
	//}

	//// 원래 액티브된 카메라 있으면 그냥둠
	//if (CurrentActiveActiveCameraActor)
	//{
	//	return true;
	//}

	return false;
}

// 기본 액티브카메라 있으면 그거 활성화
void ABladeIIGameMode::SetBaseActiveCamera()
{
	//for (int32 ACI = 0; ACI < ActiveCameraActors.Num(); ++ACI)
	//{
	//	if (ActiveCameraActors[ACI]->IsBaseActiveCamera())
	//	{
	//		ActiveCameraActors[ACI]->ForceBeginActiveCamera();
	//		return;
	//	}
	//}
}

bool ABladeIIGameMode::SummonFromSpawnPool(class ABladeIICharacter* Summoner, int32 SummonIndex, float MaxRadius)
{
	//if (TheActiveSpawnPool) // 전체 SpawnPool 을 iteration 할 필요가 없어야 한다.
	//{
	//	return TheActiveSpawnPool->SummonMobs(Summoner, SummonIndex, MaxRadius);
	//}
	return false;
}

void ABladeIIGameMode::SetHUDCenterMessage(const FText& InMessageText, float DisplayTime, int32 AnimIndex)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::SetHUDCenterMessage"));

	//SetHUDCenterMessageClass<const FText, float, int32>::GetInstance().Signal(InMessageText, DisplayTime, AnimIndex);
}

void ABladeIIGameMode::HideHUDCenterMessage()
{
	//HideHUDCenterMessageClass<>::GetInstance().Signal();
}

void ABladeIIGameMode::GetObjectInLevel(ULevel* _level /* GetWorld()->GetCurrentLevel()*/, UClass* _classType/* ASkeletalMeshActor::StaticClass()*/, TArray<AActor*>& _OutWasVisibleSkeletalActorsPool /*Out*/)
{
	//if (_level == nullptr) return;
	//if (_classType == nullptr) return;

	////In Function
	//TArray<AActor*> InlevelActors = _level->Actors;
	//int count = 0;
	//for (AActor* InLevelActor : InlevelActors)
	//{
	//	if (InLevelActor && InLevelActor->IsValidObj() && InLevelActor->IsA(_classType))
	//	{
	//		count++;
	//		_OutWasVisibleSkeletalActorsPool.Add(InLevelActor);
	//	}
	//}

	//UE_LOG(LogBladeII, Warning, TEXT("Skeletal Mesh count : %d"), count);
	//UE_LOG(LogBladeII, Warning, TEXT("_OutWasVisibleSkeletalActorsPool Size : %d"), _OutWasVisibleSkeletalActorsPool.Num());
}

void ABladeIIGameMode::GetObjectHiddenInLevel(ULevel* _level /* GetWorld()->GetCurrentLevel()*/, UClass* _classType/* ASkeletalMeshActor::StaticClass()*/, TArray<AActor*>& _OutWasVisibleSkeletalActorsPool /*Out*/)
{
	//if (_level == nullptr) return;
	//if (_classType == nullptr) return;

	////In Function
	//TArray<AActor*> InlevelActors = _level->Actors;
	//int count = 0;
	//for (AActor* InLevelActor : InlevelActors)
	//{
	//	if (InLevelActor && InLevelActor->IsValidObj() && InLevelActor->IsA(_classType) && InLevelActor->bHidden == false)
	//	{
	//		count++;
	//		InLevelActor->SetActorHiddenInGame(true);
	//		_OutWasVisibleSkeletalActorsPool.Add(InLevelActor);
	//	}
	//}

	//UE_LOG(LogBladeII, Warning, TEXT("Skeletal Mesh count : %d"), count);
	//UE_LOG(LogBladeII, Warning, TEXT("_OutWasVisibleSkeletalActorsPool Size : %d"), _OutWasVisibleSkeletalActorsPool.Num());
}

void ABladeIIGameMode::GetObjectVisibleInLevel(const TArray<AActor*>& _OutWasVisibleSkeletalActorsPool)
{
	//for (AActor* SelectSKMesh : _OutWasVisibleSkeletalActorsPool)
	//{
	//	if (SelectSKMesh && SelectSKMesh->IsValidObj() && SelectSKMesh->bHidden == true)
	//		SelectSKMesh->SetActorHiddenInGame(false);
	//}
}

bool ABladeIIGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::SetPause"));

	return Super::SetPause(PC, CanUnpauseDelegate);
}

bool ABladeIIGameMode::ClearPause()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::ClearPause"));

	bool bSuperPauseCleared = Super::ClearPause();

	// Pause menu widget 해제
	//StopPauseMenuClass<>::GetInstance().Signal();

	return bSuperPauseCleared;
}

bool ABladeIIGameMode::BeginGlobalSceneBlur(float BlurScale)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::BeginGlobalSceneBlur"));
	//
	//if (bGlobalSceneBlurOn) {
	//	// Blur 되기 전의 카메라 설정을 캐싱해 놓기 때문에 이미 적용되어 있는 상태에서 두번 들어오면 곤란
	//	return false;
	//}
	//
	//CachedCameraAtBlurBegin = GetCurrentActiveCamComp(this);

	//if (CachedCameraAtBlurBegin)
	//{
	//	// GlobalSceneBlur 에서 복구하기 위해 설정들 캐싱.
	//	CachedPPBeforeBlur = CachedCameraAtBlurBegin->PostProcessSettings;
	//	CachedPPWeightBeforeBlur = CachedCameraAtBlurBegin->PostProcessBlendWeight;

	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldMethod = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldMethod = EDepthOfFieldMethod::DOFM_Gaussian;
	//	
	//	// PC 버전의 GaussianDOF 는 Near/FarBlurSize 를 조절하지만 Mobile 버전에서는 Scale 값으로 조절함. 둘이 걍 다름
	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldScale = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldScale = BlurScale; // 근데 어째 모바일에서도 안먹히는 듯? 걍 정해진 스케일이 들어가는 거 같다. 블러 커널 사이즈에 따라 성능이 좌우되는 식이면 충분히 말이 되겠지 낄낄
	//	
	//	// 즉, 여기 Near/FarBlurSize 세팅하는 건 PC 에서 적당히 보여주기 위함.
	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldNearBlurSize = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldFarBlurSize = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldNearBlurSize = BlurScale * 5.0f;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldFarBlurSize = BlurScale * 5.0f;

	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldFocalDistance = 100000.0f; // 이건 그냥 큰 수로 줘서 모두 블러링 되도록 함.

	//	// 한번 FSceneView::OverridePostProcessSettings 를 거친 후에나 실제 적용이 되는 듯. Pause 메뉴 같은 경우라면 실제 적용을 한 틱 늦춰야..

	//	bGlobalSceneBlurOn = true;

	//	return true;
	//}

	return false;
}

void ABladeIIGameMode::EndGlobalSceneBlur()
{
	if (bGlobalSceneBlurOn)
	{
		if (CachedCameraAtBlurBegin) // 없다고 해서 다시 가져오지 않는다. 
		{
			CachedCameraAtBlurBegin->PostProcessSettings = CachedPPBeforeBlur;
			CachedCameraAtBlurBegin->PostProcessBlendWeight = CachedPPWeightBeforeBlur;
		}

		CachedCameraAtBlurBegin = NULL;
		bGlobalSceneBlurOn = false;
	}
}

void ABladeIIGameMode::NotifyPlayerDeadEnd(ABladeIIPlayer* DeadPlayer)
{
}

void ABladeIIGameMode::NotifyCharacterDead(class ABladeIICharacter* TargetCharacter, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	if (TargetCharacter)
	{
		const TArray<ABladeIICharacter*>& Minions = TargetCharacter->GetMinions();
		for (int32 Index = 0; Index < Minions.Num(); Index++)
		{
			auto* Minion = Minions[Index];
			if (Minion && !IsValid(Minion))
				Minion->SetDieForce();
		}
	}
}

void ABladeIIGameMode::DeactivateWorldMovablePointLight() // 모든 MovablePointLight 는 아니고 필수적인 거 뺀.. 대략 연출 용 끄기
{
	// 특히 연출 전에 의도하지 않게 켜져 있는 movable point light 를 꺼서 모바일 렌더러의 movable point light 제한을 지키기 위한 의미로 사용하는 것.
	// 연출에서는 matinee 트랙이나 플레이어 캐릭터 light 를 이어받는 식으로 자체적인 라이트 세팅을 하게 된다.
	for (TActorIterator<APointLight> It(GetWorld()); It; ++It)
	{
		APointLight* ThisPLActor = Cast<APointLight>(*It);
		UPointLightComponent* ThisPLComp = ThisPLActor ? Cast<UPointLightComponent>(ThisPLActor->GetLightComponent()) : nullptr;
		// 별도로 설치한 PointLight 만 끄는 것이므로 플레이어 캐릭터 머리 위에 붙인 건 이걸로 꺼지지 않는다.
		if (ThisPLComp && ThisPLComp->Mobility == EComponentMobility::Type::Movable)
		{
			ThisPLComp->bAffectsWorld = false;
			ThisPLComp->MarkRenderStateDirty();
		}
	}
}

void ABladeIIGameMode::StartEventScene_GameBegin()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StartEventScene_GameBegin"));

	//for (AB2StageEventDirector* CurrSED : EventDirectors)
	//{
	//	if (CurrSED){
	//		CurrSED->NotifyGameStarted();
	//	}
	//}
}

void ABladeIIGameMode::StartEventScene_GameClear()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StartEventScene_GameClear"));
//	for (AB2StageEventDirector* CurrSED : EventDirectors)
//	{
//		if (CurrSED){
//			CurrSED->NotifyGameClear();
//		}
//	}
//#if BII_STATS
//	PerfStatCollector.OnEnd(this); // 대략 게임 클리어 시점이 좋은데 모드별로 조금씩 달라서.. OnEnd 는 여러 곳에서 호출
//#endif
//
//	SendDamageLog();
}

void ABladeIIGameMode::StartEventScene_MatchResult(bool HasLocalPlayerWon)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StartEventScene_MatchResult"));
	// 일차적으로, PVP 모드 결과 연출이 승리 패배 상관없이 승자를 보여주는 방식이 되어 StartEventScene_GameClear 와 구분이 되었음.
//	for (AB2StageEventDirector* CurrSED : EventDirectors)
//	{
//		if (CurrSED){ // 향후 다른 NetGameMode 결과 연출도 마찬가지 루트를 탈 수 있을지는 모르겠다.
//			CurrSED->NotifyMatchEnd(HasLocalPlayerWon);
//		}
//	}
//#if BII_STATS
//	PerfStatCollector.OnEnd(this); // 대략 게임 클리어 시점이 좋은데 모드별로 조금씩 달라서.. OnEnd 는 여러 곳에서 호출
//#endif
//
//	SendDamageLog();
}

void ABladeIIGameMode::PlayVictoryBGM()
{
	PlayBGMByCommonSoundID(ECommonSoundID::ECSID_BGM_CombatVictory);
}

void ABladeIIGameMode::PlayStageVictoryBGM(int32 IsGoldBonusStage, int32 IsBosStage)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ABladeIIGameMode::PlayStageVictoryBGM IsGoldBonusStage : %d, IsBosStage : %d"), IsGoldBonusStage, IsBosStage));

	ECommonSoundID eVictorySoundID = ECommonSoundID::ECSID_BGM_CombatVictory;

	if (IsBosStage)
	{
		eVictorySoundID = ECommonSoundID::ECSID_BGM_CombatBossStageVictory;
	}
	else if (IsGoldBonusStage)
	{
		eVictorySoundID = ECommonSoundID::ECSID_BGM_CombatGoldBonusStageVictory;
	}

	PlayBGMByCommonSoundID(eVictorySoundID);
}

void ABladeIIGameMode::KillAllEnemy()
{
	for (TActorIterator<ABladeIICharacter> It(GetWorld()); It; ++It)
	{
		ABladeIICharacter* pCharacter = *It;

		//if (pPlayer && pPlayer->IsLocalPlayer())
		//	continue;
		if (pCharacter)
		{
			pCharacter->SetDieForce();
		}
	}
}

void ABladeIIGameMode::PlayBGMByCommonSoundID(ECommonSoundID InID, bool bForceStopCurrent)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::PlayBGMByCommonSoundID"));
	//if (BGMManager)
	//	BGMManager->PlayBGMByCommonSoundID(InID, bForceStopCurrent);
}

void ABladeIIGameMode::PlayBGMByStageContext(int32 InClientStageId, bool bIsExtraMap, EStageCombatSoundContext InStageContext, bool bForceStopCurrent)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::PlayBGMByStageContext"));
	if (BGMManager)
		BGMManager->PlayBGMByStageContext(InClientStageId, bIsExtraMap, InStageContext, bForceStopCurrent);
}

void ABladeIIGameMode::PlayBGMByUIScene(EUIScene InUIScene)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::PlayBGMByUIScene"));
	if (BGMManager)
		BGMManager->PlayBGMByUIScene(InUIScene);
}

void ABladeIIGameMode::PlayBGMCustomize(const FName& InName)
{
	
}

void ABladeIIGameMode::StopBGM()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StopBGM"));

	if (BGMManager)
		BGMManager->StopBGM();
}
void ABladeIIGameMode::SetPauseBGM(bool bPause)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::SetPauseBGM"));
	if (BGMManager)
		BGMManager->SetPauseBGM(bPause);
}

void ABladeIIGameMode::PlayAMBByCommonSoundID(ECommonSoundID InID, bool bForceStopCurrentAMB, bool bStopCurrentBGM)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::PlayAMBByCommonSoundID"));

	if (bStopCurrentBGM)
	{ // 일반적으로 Ambient sound 트는데 BGM 을 멈출 이유는 없겠지..
		StopBGM();
	}

	if (bForceStopCurrentAMB)
	{
		StopAMB();
	}

	UB2CommonSoundInfo* CSInfo = StaticFindCommonSoundInfo(this);
	if (CSInfo)
	{
		USoundCue* WantedSound = CSInfo->GetSoundByID(InID);
		if (WantedSound)
		{
			USoundCue* CurrentPlayingSoundCue = (CurrentPlayingAMB && CurrentPlayingAMB->IsPlaying()) ? Cast<USoundCue>(CurrentPlayingAMB->Sound) : NULL;
			const bool bSameOneAlreadyPlaying = (WantedSound == CurrentPlayingSoundCue);

			if (!bSameOneAlreadyPlaying)
			{
				if (!bForceStopCurrentAMB){
					StopAMB(); // 처음에 멈추지 않았다면
				}
				CurrentPlayingAMB = UGameplayStatics::SpawnSound2D(this, WantedSound);
			}
		}
	}
}

void ABladeIIGameMode::StopAMB()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StopAMB"));

	//if (CurrentPlayingAMB)
	//{
	//	if (!CurrentPlayingAMB->IsPendingKill())
	//	{
	//		CurrentPlayingAMB->Stop();
	//		CurrentPlayingAMB->ConditionalBeginDestroy();
	//	}
	//	CurrentPlayingAMB = NULL;
	//}
}
void ABladeIIGameMode::SetPauseAMB(bool bPause)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::SetPauseAMB"));
	if (CurrentPlayingAMB)
	{
		CurrentPlayingAMB->SetPaused(bPause);
	}
}

FString EB2LOC_CAT_ToString(EB2LOC_CAT InB2LOCEnum)
{
	switch (InB2LOCEnum)
	{
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(GENERAL)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(WORLDNAME)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(CHARNAME)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(ITEMNAME)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(DIALOGTEXT)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(SKILLDATA)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(MAILCONTENT)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(BATTLE_MENT)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(QUESTTEXT)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(ERROR_CODE)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(MISSIONTEXT)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(WINGTEXT)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(STORE)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(ETHER)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(ANCIENTRELIC)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(UNITYSKILLMISSION)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(ITEMPREFIX)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(LOCALPUSH)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(CONFIGURATION)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(GUILDNPC)
		EB2LOC_CAT_ENUM_TO_STRING_CASE_MAPPING(GUILDNPCSKILL)
	}
	check(0); // 새로운 항목에 대해 case 구문을 제공하지 않으면 check 에 걸리게 될 것.
	return TEXT("");
}

////#include "Matinee/MatineeActor.h"
//#include "Matinee/InterpData.h"
//#include "Matinee/InterpGroup.h"
//#include "Matinee/InterpGroupInst.h"
//#include "Matinee/InterpTrack.h"
//#include "Matinee/InterpTrackEvent.h"
//#include "Matinee/InterpTrackMove.h"
//#include "Matinee/InterpTrackFloatProp.h"

float ABladeIIGameMode::GetMatineePositionByEventName(class ALevelSequenceActor * MatineeActor, FName EventName)
{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::GetMatineePositionByEventName"));
//
//	if (MatineeActor == NULL || MatineeActor->MatineeData == NULL)
//		return 0.f;
//
//	if (!MatineeActor->MatineeData->IsEventName(EventName))
//		return 0.f;
//
//	for (auto* InterpGroup : MatineeActor->MatineeData->InterpGroups)
//	{
//		if (InterpGroup == NULL)
//			continue;
//
//		for (auto* InterpTrack : InterpGroup->InterpTracks)
//		{
//			auto* EventTrack = Cast<UInterpTrackEvent>(InterpTrack);
//			if (EventTrack == NULL)
//				continue;
//
//			for (auto& EventKey : EventTrack->EventTrack)
//			{
//				if (EventName == EventKey.EventName)
//					return EventKey.Time;
//			}
//		}
//	}

	return 0.f;
}

void ABladeIIGameMode::B2GM_SetAllowCullDistance(bool bAllow, TArray<UPrimitiveComponent*> Components)
{
//	for (UPrimitiveComponent* pComponent : Components)
//	{
//		if (pComponent)
//		{
//			pComponent->bAllowCullDistanceVolume = bAllow;
//
//			if (!bAllow)
//			{
//				pComponent->CachedMaxDrawDistance = pComponent->LDMaxDrawDistance;
//			}
//			else if (UWorld* World = GetWorld())
//			{
//				World->UpdateCullDistanceVolumes(nullptr, pComponent);
//			}
//			
//			pComponent->MarkRenderStateDirty();			
//		}
//	}
}

void ABladeIIGameMode::SetCameraMatineeCam(class ALevelSequenceActor * MatineeActor, float KeyTime)
{
	//// 아마도 모험맵 자동 스크롤의 영향인지 가만히 있어도 로그가 많이 나와서.. 여긴 제거.
	////B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::SetCameraMatineeCam"));

	//if (!MatineeActor)
	//	return;

	//if (!MatineeActor->bIsPlaying)
	//	MatineeActor->InitInterp();

	//auto* LocalController = UGameplayStatics::GetLocalPlayerController(MatineeActor);
	//if (!LocalController)
	//	return;

	//for (auto* InterpGroup : MatineeActor->GroupInst)
	//{
	//	AActor* GroupActor = InterpGroup->GetGroupActor();
	//	if (GroupActor == NULL)
	//		continue;

	//	// 제일 먼저 발견한 카메라 트랙을 사용.. 이걸로 사용하려는 카메라 트랙을 제일 위에 배치해야 한다.
	//	if (GroupActor->IsA(ACameraActor::StaticClass()) && InterpGroup->Group)
	//	{
	//		LocalController->PlayerCameraManager->SetViewTarget(GroupActor);

	//		for (int32 i = 0; i < InterpGroup->Group->InterpTracks.Num(); ++i)
	//		{
	//			BII_CHECK(InterpGroup->Group->InterpTracks.Num() == InterpGroup->TrackInst.Num())
	//				auto* Track = InterpGroup->Group->InterpTracks[i];
	//			if (Track)
	//			{
	//				auto* FloatPropTrack = Cast<UInterpTrackFloatProp>(Track);
	//				if (FloatPropTrack&& !FloatPropTrack->IsDisabled() && FloatPropTrack->PropertyName == TEXT("FOVAngle"))
	//				{
	//					FloatPropTrack->UpdateTrack(KeyTime, InterpGroup->TrackInst[i], true);
	//				}

	//				auto* CameraMovementTrack = Cast<UInterpTrackMove>(Track);
	//				if (CameraMovementTrack && !CameraMovementTrack->IsDisabled())
	//				{
	//					FVector Location;
	//					FRotator Rotation;
	//					CameraMovementTrack->GetLocationAtTime(InterpGroup->TrackInst[i], KeyTime, Location, Rotation);

	//					GroupActor->SetActorLocationAndRotation(Location, Rotation);
	//				}
	//			}
	//		}

	//		LocalController->PlayerCameraManager->UpdateCamera(0.f);
	//		break;
	//	}
	//}

	//if (!MatineeActor->bIsPlaying)
	//	MatineeActor->TermInterp();
}

void ABladeIIGameMode::CustomEndCurrentActiveActiveCameraActor()
{
	//if (CurrentActiveActiveCameraActor)
	//{
	//	CurrentActiveActiveCameraActor->CustomEndActiveCamera();
	//}
}

void ABladeIIGameMode::GetAllWorldEnemyList(TArray<class ABladeIICharacter*>& OutList, int8 MyTeamNum, bool bOnlyLiveMobs /* = true */)
{
	OutList.Empty();
}

void ABladeIIGameMode::SetupUIScene()
{

}

void ABladeIIGameMode::OnStageBegin()
{
	//B2GMLoadingProgCollector::StepToMax(); // 대체로 SetupUIScene 직전에 로딩바 꽉 채우면 적절한 듯
	SetupUIScene();
}

void ABladeIIGameMode::OnReceiveContentsModeStateNotify(int32 ContentsMode, int32 PrevModeState, int32 CurrentModeState)
{
	//UB2UIDocMode* pDocMode = UB2UIDocHelper::GetDocMode();

	//if (pDocMode)
	//{
	//	if (CurrentModeState == b2network::B2ContentsModeState::SYSTEM_CHECK
	//		|| CurrentModeState == b2network::B2ContentsModeState::MODE_FINISH)
	//	{
	//		CheckOpenModeStateNoticeUI(ContentsMode, CurrentModeState);
	//	}

	//	pDocMode->ReceiveSingleGetContentsModeState(ContentsMode, CurrentModeState);
	//}
}

void ABladeIIGameMode::CheckOpenModeStateNoticeUI(int32 ContentsMode, int32 CurrentModeState)
{
//	UB2UITimerNotice* pUI = UB2UIManager::GetInstance()->GetUI<UB2UITimerNotice>(UIFName::TimerNotice);
//	
//	// 이미 오픈되있으면 체크ㄴㄴ
//	if (!pUI)
//	{
//		bool bNeedOpen = false;
//		EUIScene CurrentUIScene = UB2UIManager::GetInstance()->GetCurrUIScene();
//
//		// 체크할때 현재게임모드나 UI씬이 적합하면 로비로 팅김UI 보여준다.
//		// 정산이랑 점검이랑 처리가 다르다.
//		if (CurrentModeState == b2network::B2ContentsModeState::SYSTEM_CHECK)
//		{
//			// 점검 케이스
//			switch (ContentsMode)
//			{
//			case b2network::B2ContentsMode::PVP_DUEL:
//				bNeedOpen = GetB2GameModeType() == EB2GameMode::PVP_Tag || CurrentUIScene == EUIScene::PVPOneOnOneFindMatch;
//				bNeedOpen = bNeedOpen && TutorialManager::GetInstance().IsFinishTutorial(TutorialID_PvPOneAndOne);
//				break;
//			case b2network::B2ContentsMode::PVP_TEAM:
//				bNeedOpen = GetB2GameModeType() == EB2GameMode::PVP_Team || CurrentUIScene == EUIScene::TeamMatchFindMatch;
//				bNeedOpen = bNeedOpen && TutorialManager::GetInstance().IsFinishTutorial(TutorialID_PvPTeam);
//				break;
//			case b2network::B2ContentsMode::ASSAULT:
//				bNeedOpen = GetB2GameModeType() == EB2GameMode::Control;
//				bNeedOpen = bNeedOpen && TutorialManager::GetInstance().IsFinishTutorial(TutorialID_Occupy);
//				break;
//			case b2network::B2ContentsMode::COUNTER:
//				bNeedOpen = GetB2GameModeType() == EB2GameMode::CounterDungeon;
//				bNeedOpen = bNeedOpen && TutorialManager::GetInstance().IsFinishTutorial(TutorialID_CounterDungeon);
//				break;
//			case b2network::B2ContentsMode::HERO_TOWER:
//				bNeedOpen = GetB2GameModeType() == EB2GameMode::HeroTower;
//				bNeedOpen = bNeedOpen && TutorialManager::GetInstance().IsFinishTutorial(TutorialID_HeroTower);
//				break;
//			case b2network::B2ContentsMode::RAID:
//				bNeedOpen = GetB2GameModeType() == EB2GameMode::Raid;
//				bNeedOpen = bNeedOpen && TutorialManager::GetInstance().IsFinishTutorial(TutorialID_Raid);
//				break;
//			case b2network::B2ContentsMode::DIMENSION:
//				bNeedOpen = GetB2GameModeType() == EB2GameMode::Dimension;
//				break;
//			default:
//				break;
//			}
//		}
//		else if (CurrentModeState == b2network::B2ContentsModeState::MODE_FINISH)
//		{
//			// 정산 케이스
//			// 1:1 팀대전 점령전 균열만 정산있음.
//			switch (ContentsMode)
//			{
//			case b2network::B2ContentsMode::PVP_DUEL:
//				bNeedOpen = GetB2GameModeType() == EB2GameMode::PVP_Tag || CurrentUIScene == EUIScene::PVPOneOnOne
//					|| CurrentUIScene == EUIScene::PVPCharacterSelectPage || CurrentUIScene == EUIScene::PVPOneOnOneFindMatch;
//				bNeedOpen = bNeedOpen && TutorialManager::GetInstance().IsFinishTutorial(TutorialID_PvPOneAndOne);
//				break;
//			case b2network::B2ContentsMode::PVP_TEAM:
//				bNeedOpen = GetB2GameModeType() == EB2GameMode::PVP_Team || CurrentUIScene == EUIScene::TeamMatchUI
//					|| CurrentUIScene == EUIScene::TeamHeroChangeUI || CurrentUIScene == EUIScene::TeamMatchFindMatch;
//				bNeedOpen = bNeedOpen && TutorialManager::GetInstance().IsFinishTutorial(TutorialID_PvPTeam);
//				break;
//			case b2network::B2ContentsMode::ASSAULT:
//				bNeedOpen = GetB2GameModeType() == EB2GameMode::Control || CurrentUIScene == EUIScene::ControlMain
//					|| CurrentUIScene == EUIScene::ControlCharacterSelectPage;
//				bNeedOpen = bNeedOpen && TutorialManager::GetInstance().IsFinishTutorial(TutorialID_Occupy);
//				break;
//			case b2network::B2ContentsMode::DIMENSION:
//				bNeedOpen = GetB2GameModeType() == EB2GameMode::Dimension || CurrentUIScene == EUIScene::DimensionMain
//					|| CurrentUIScene == EUIScene::DimensionSelectPage;
//				break;
//			case b2network::B2ContentsMode::COUNTER:
//				break;
//			case b2network::B2ContentsMode::HERO_TOWER:
//				break;
//			case b2network::B2ContentsMode::RAID:
//				break;
//			default:
//				break;
//			}
//		}
//
//		if (CurrentUIScene == EUIScene::HallOfFame && CurrentModeState == b2network::B2ContentsModeState::MODE_FINISH)
//		{
//			switch (ContentsMode)
//			{
//			case b2network::B2ContentsMode::PVP_DUEL:
//			case b2network::B2ContentsMode::PVP_TEAM:
//			case b2network::B2ContentsMode::ASSAULT:
//			//case b2network::B2ContentsMode::DIMENSION: // 균열은 전당하고 관계없음
//			{
//
//				if (auto* UIHallOfFame = UB2UIManager::GetInstance()->GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
//				{
//					UIHallOfFame->SetSettleState();
//					bNeedOpen = false; // 이건 무조건 안떠야 됨.
//				}
//
//				break;
//			}
//			}
//		}
//
//		if (bNeedOpen)
//		{
//			pUI = UB2UIManager::GetInstance()->OpenUI<UB2UITimerNotice>(UIFName::TimerNotice);
//
//			if (pUI)
//			{
//				FString NoticeTextKey = CurrentModeState == b2network::B2ContentsModeState::SYSTEM_CHECK ? TEXT("ContentsCheck") : TEXT("RankingCheck");
//
//				pUI->SetContentText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, NoticeTextKey));
//				pUI->SetNoticeTimer(5.99, FMsgPopupOnClick::CreateLambda([this]()
//				{
//					GoToVillage();
//				}
//				));
//			}
//		}
//	}
}

void ABladeIIGameMode::CancelOrStopRepeatBattle()
{

}

void ABladeIIGameMode::GiveUpGame()
{

}

void ABladeIIGameMode::UpdateAdminMessage(const FString &InMessage, const int32 InDisplayTime)
{
//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//	if (UIMgrInst)
//	{
//		bool IsInvisible = UIMgrInst->GetCurrUIScene() == EUIScene::LogIn || UIMgrInst->GetCurrUIScene() == EUIScene::Title;
//		if (IsInvisible)
//			return;
//
//		UB2UIBroadcastSystemNotice* BroadcastUI = UIMgrInst->GetUI<UB2UIBroadcastSystemNotice>(UIFName::BroadcastSystemNotice);
//		if (BroadcastUI == nullptr)
//		{
//			BroadcastUI = UIMgrInst->OpenUI<UB2UIBroadcastSystemNotice>(UIFName::BroadcastSystemNotice);
//		}
//		if (BroadcastUI)
//		{
//			BroadcastUI->SetMessageInfo(InMessage, InDisplayTime);
//			BroadcastUI->StartBroadcast();
//		}
//	}
}

void ABladeIIGameMode::UpdateChatUI(bool bIsScrollEnd)
{


}

bool ABladeIIGameMode::CheckConditionBroadcastMessage()
{
//	// 헤더가 존재 할 경우와 길드씬 제외하고 모두 방송 가능
//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//	UB2UIChatting* MainChatPage = UIMgrInst ? UIMgrInst->GetUI<UB2UIChatting>(UIFName::Chatting) : NULL;
//
//	UB2UIHeader* HeaderUI = UIMgrInst ? UIMgrInst->GetUI<UB2UIHeader>(UIFName::Header) : NULL;
//	bool HasHeaderUI = (HeaderUI ? true : false) || B2AndroidBackManager::GetInstance()->HasHaderUI();
//	if (HeaderUI && HeaderUI->GetRestoreRotate())
//	{
//		return false;
//	}
//
//	EUIScene FillterScens[] = {
//		EUIScene::GuildJoinAndCreate,
//		EUIScene::GuildMain,
//		EUIScene::GuildMatchBattle,
//		EUIScene::GuildMapUI,
//		EUIScene::GuildResult,
//		EUIScene::GuildBattleReady,
//	};
	bool IsShowMessage = true;
//	for (auto &Each : FillterScens)
//	{
//		if (UIMgrInst->IsUIScene(Each))
//		{
//			IsShowMessage = false;
//			break;
//		}
//	}
//
	//return IsShowMessage ? HasHeaderUI : false;
	return false;

}

void ABladeIIGameMode::TryOpenChatSinglePopup(const FB2ChatMessage& InNewMessage)
{
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::TryOpenChatSinglePopup"));
//	if (InNewMessage.IsBroadcastMessage())
//	{
//		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//		UB2UIChatting* MainChatPage = UIMgrInst ? UIMgrInst->GetUI<UB2UIChatting>(UIFName::Chatting) : NULL;
//		bool ConditionShowMessage = CheckConditionBroadcastMessage();
//		bool HasWidget = UIMgrInst && !MainChatPage && UIMgrInst->GetMainWidget();
//
//		if (HasWidget && ConditionShowMessage) // MainChatPage 가 있다면 한줄채팅을 열 필요가 없다.
//		{
//			UIMgrInst->GetMainWidget()->OpenChatSinglePopupWithNewMessage(InNewMessage);
//		}
//	}
}

void ABladeIIGameMode::StartEventScene_FinishAttack(ABladeIICharacter* Inflictor)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StartEventScene_FinishAttack"));

/*	for (AB2StageEventDirector* CurrSED : EventDirectors)
	{
		if (CurrSED)
		{
			CurrSED->NotifyFinishAttack(Inflictor);
		}
	}

	auto* BladeIIPlayer = Cast<ABladeIIPlayer>(Inflictor);
	if (BladeIIPlayer)
		BladeIIPlayer->ForceFeedback()*/;
}

void ABladeIIGameMode::EnsureStageEventTimeDilation()
{ // 기본적으로 연출 시작 시 TimeDilation 처리를 하지만 TimeDilation 이 있는 AnimNotifyState 등과 간발의 차이로 인해 그게 무효화될 때에 다시 적용을 위함.
	//for (AB2StageEventDirector* CurrSED : EventDirectors)
	//{
	//	if (CurrSED){
	//		CurrSED->ApplyStageEventTimeDilationIfPlaying();
	//	}
	//}
}

void ABladeIIGameMode::RestoreWholeGameTimeScale()
{
	//SetWholeGameTimeScale(1.0f); // 기본 1.0 이고 특정 설정으로 인해 다르게 쓸 수 있음. SetGlobalTimeDilation 과 별도로 먹힘.
}

ABladeIICharacter* ABladeIIGameMode::SpawnCharacter(UB2NPCClassInfoBox* InfoBox, ENPCClass InClass, ENPCClassVariation InVariation, FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::SpawnCharacter"));

	BII_CHECK(InfoBox);

	return InfoBox->SpawnNPCClassInfoCharacter(GetWorld(), InClass, InVariation, SpawnTransform, SpawnParameters);
}

ACharacter* ABladeIIGameMode::GetNearestPlayerCharacter(FVector Location, bool bSkipByCustomLogic)
{
	//auto* PlayerCharacter = UGameplayStatics::GetNearestPlayerCharacter(this, Location, bSkipByCustomLogic);
	//return PlayerCharacter;
	return NULL;
}

ACharacter* ABladeIIGameMode::GetNearestPlayerCharacter(class ACharacter* pCharacter, bool bSkipByCustomLogic)
{
	return GetNearestPlayerCharacter(pCharacter->GetActorLocation(), bSkipByCustomLogic);
}

void ABladeIIGameMode::SetResurrectPlayerPosition(ABladeIIPlayer* pBladePlayer)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::SetResurrectPlayer"));
}

void ABladeIIGameMode::ShowCachedCompleteQuestNotify()
{
	//const TArray<QuestPtr>& CachedCompleteQuests = QuestManager::GetInstance().GetCachedCompleteSubQuests();
	//bool bShowNotifyWidget = CachedCompleteQuests.Num() > 0 ? true : false;

	//if (bShowNotifyWidget)
	//{
	//	//UB2UIQuestCompleteNotify* QuestNotify = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestCompleteNotify>(UIFName::QuestCompleteNotify);
	//	//for (QuestPtr CompleteQuest : CachedCompleteQuests)
	//	//{
	//	//	if (QuestNotify && CompleteQuest != nullptr && CompleteQuest->GetQuestSlot() != b2network::B2QuestSlotType::MAIN1)
	//	//	{
	//	//		QuestNotify->ShowCompleteQuestNotify(CompleteQuest->GetQuestSlot());
	//	//	}
	//	//}
	//}
}

void ABladeIIGameMode::OnQuestCompleteNotify(int32 QuestSlot)
{
	//UB2UIQuestCompleteNotify* QuestNotify = UB2UIManager::GetInstance()->OpenUI<UB2UIQuestCompleteNotify>(UIFName::QuestCompleteNotify);
	//if (QuestNotify)
	//{
	//	QuestNotify->ShowCompleteQuestNotify(QuestSlot);
	//}
}

void ABladeIIGameMode::RealtimeUpdateUIDoc()
{
	//ABladeIIPlayerController* const B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
	//ABladeIIPlayer* const B2Player = B2PC ? Cast<ABladeIIPlayer>(B2PC->GetPawn()) : nullptr;

	//RealtimeUpdateUIDocSkill(B2Player);
	//RealtimeUpdateUIDocHealth(B2Player);
}

void ABladeIIGameMode::OnPlatformPause()
{
	// Nothing
}

void ABladeIIGameMode::RealtimeUpdateUIDocSkill(ABladeIIPlayer* const B2Player)
{
//	//매 Tick마다 불리는 것임
//	// ABladeIIPlayer::SpawnOtherAsPuppet 같은 걸로 생성된 경우 update 에 끼어 들어가선 안됨.
//	if (B2Player && B2Player->IsLocalPlayer())
//	{
//		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
//		if (DocBattle)
//		{
//			DocBattle->SetCurrPCInCriticalCondition(B2Player->IsInCriticalCondition());
//			// TagPCHealth 는 SkillRTManager 쪽에서..
//			if (B2Player->CurrentComboNum > 0) // 0 보다 크다면 언제나 카운트 하고 있을 것임
//			{
//				DocBattle->SetComboExpireProgress((GetWorld()->GetTimeSeconds() - B2Player->LastAttackSuccessTime) / B2Player->ComboMaintainDuration);
//			}
//			else
//			{
//				DocBattle->SetComboExpireProgress(1.0f);
//			}
//
//			ABladeIICharacter* TargetActor = B2Player->QTEActor ? B2Player->QTEActor : B2Player->FindQTEEnableTarget();
//			if (B2Player->IsAlive() && TargetActor && TargetActor->IsAlive() && TargetActor->IsTimingForQTEPossession()
//				// QTE진입 가능할때만 보여주자.
//				&& B2Player->IsQTEStartAble()
//				&& TargetActor->GetDamagedNum() == 0
//				// 조건추가. 타겟몹막타친팀(QTEEnableTeam) 설정 안되었거나, 플레이어팀이랑 일치할때.
//				// 타겟액터가 넷플레이어 아니면 타겟몹막타친팀(QTEEnableTeam) 설정이 안되어있을거임
//				&& (TargetActor->QTEEnableTeam == ETeamType::ETT_End || ETeamType(B2Player->GetTeamNum()) == TargetActor->QTEEnableTeam))
//			{//QTE 1타 진입 가능 시점
//				if(TargetActor->QTEType == EQTEType::EQT_Mount)
//					DocBattle->SetQTEEnableState(EQTEUIAttackEnableTimingState::Mount);
//				else
//					DocBattle->SetQTEEnableState(EQTEUIAttackEnableTimingState::FirstAttack);
//			}
//			else if (B2Player->IsAlive() && TargetActor && TargetActor->IsAlive() && B2Player->IsTimingForSuccessiveQTE()
//				// QTE진입 가능할때만 보여주자.
//				&& B2Player->IsQTEStartAble()
//				&& TargetActor->GetDamagedNum() == 0)
//			{// QTE 2타 진입 시점
//				DocBattle->SetQTEEnableState(EQTEUIAttackEnableTimingState::SecondAttack);
//			}
//			else
//			{
//				DocBattle->SetQTEEnableState(EQTEUIAttackEnableTimingState::None);
//			}
//		}
//
//		if (auto* const CachedSkillRTManager = B2Player->GetSkillRTManager())
//		{
//			CachedSkillRTManager->RealtimeUpdateUIDoc();
//		}
//
//		if (auto* const EtherContainer = B2Player->GetEtherContainer())
//		{
//			EtherContainer->RealtimeUpdateUIDoc(DocBattle);
//		}
//	}
}

void ABladeIIGameMode::RealtimeUpdateUIDocHealth(ABladeIIPlayer* const B2Player)
{
//	//매 Tick마다 불리는 것임
//	if (B2Player && B2Player->IsLocalPlayer())
//	{
//		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
//		if (DocBattle)
//			DocBattle->SetCurrPCHealth(B2Player->Health);
//	}
//
//	TArray<ABladeIICharacter*> Moblist;
//	GetAllWorldMobList(Moblist);
//	for (ABladeIICharacter* mob : Moblist)
//	{
//		if (mob && mob->IsSpawnedAsBossMob())
//		{
//			UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
//			if (DocBattle)
//			{ // BossMob 관련 나머지 초기 수치들은 SpawnPool 에서 세팅
//				DocBattle->SetBossHPCurrent(mob->Health < 0 ? 0 : mob->Health);
//				if (mob->IsQTEBreakable()){
//					DocBattle->SetBossQTEBreakPointCurrent(mob->GetCurrentQTEBreakPoint());
//				}
//			}
//			// 리스트 중 Boss 는 하나만 있어야 한다. 여기서 넣고자 하는 수치가 단일 UI 에 영향을 미치게 되므로 만에 하나 여럿 있다고 모두 넣어봐야 문제만 됨.
//			break;
//		}
//	}
}

void ABladeIIGameMode::ShowMissionDialog()
{
//	//튜토리얼 진행 중에는 다이얼로그 창이 뜨지 않아야 한다.
//	if (TutorialManager::GetInstance().HasAnyTutorial())
//		return;
//
//	UB2UIManager* UIManager = UB2UIManager::GetInstance();
//	if (UIManager != nullptr)
//	{
//		// Dialog 띄워주기
//		TArray<MissionPtr> CompleteMissionList = MissionManager::GetInstance().GetCachedCompleteMission();
//		if (CompleteMissionList.IsValidIndex(0))
//		{
//			MissionPtr CompleteMission = CompleteMissionList[0];
//			auto MissionDialog = UIManager->OpenUI<UB2UIQuestDialog>(UIFName::QuestDialog);
//			if(MissionDialog)
//				MissionDialog->OpenMissionCompleteDialog(CompleteMission ? CompleteMission->MissionID : 0);
//		}
//	}
}

void ABladeIIGameMode::SetGuildBuff(const TArray<b2network::B2GuildSkillPtr>& GuildSkill)
{
//	auto GuildDoc = UB2UIDocHelper::GetDocGuild();
//
//	if (!GuildDoc)
//		return;
//	
//	GuildDoc->SetGuildBuff(GuildSkill);
}

void ABladeIIGameMode::MakeMobUndead()
{
}

void ABladeIIGameMode::MakeUndeadMobDeath()
{
//	//데미지 숫자 올라가는 것
//	CharacterTakeDamageEvent.Broadcast();
//	CharacterTakeDamageEvent.Clear();
//
//	//죽는 것(OnDeath에서 Undeath상태를 변경하기 때문에 맨 아래 있어야 함)
//	CharacterDeathEvent.Broadcast();
//	CharacterDeathEvent.Clear();//[fixme][@AKI, 170815]위험해 보이는데...Timer를 걸어서 제거 할까??
}

//float ABladeIIGameMode::GetTotalAttack(ICharacterDataStore* CharacterDataStore)
//{
//	//const auto	MainPCClass = CharacterDataStore->GetMainPlayerClass();
//	//const auto	TagPCClass = CharacterDataStore->GetSubPlayerClass();
//
//	//float		MainPCAttack = CombatStatEval::GetPCAttack(MainPCClass, CharacterDataStore);
//	//float		MainModSpecializeAttack = CombatStatEval::GetPCAttackByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);
//	//float		MainTotalAttack = MainPCAttack + MainModSpecializeAttack;
//	//MainTotalAttack = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(MainPCClass))), MainTotalAttack);
//
//	//float		TagPCAttack = CombatStatEval::GetPCAttack(TagPCClass, CharacterDataStore);
//	//float		TagModSpecializeAttack = CombatStatEval::GetPCAttackByMod(TagPCClass, GetB2GameModeType(), CharacterDataStore);
//	//float		TagTotalAttack = TagPCAttack + TagModSpecializeAttack;
//	//TagTotalAttack = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(TagPCClass))), TagTotalAttack);
//
//	//float		TotalAttack = MainTotalAttack + TagTotalAttack;
//
//	//return TotalAttack;
//}
//
//float ABladeIIGameMode::GetTotalDefense(ICharacterDataStore* CharacterDataStore)
//{
//	const auto	MainPCClass = CharacterDataStore->GetMainPlayerClass();
//	const auto	TagPCClass = CharacterDataStore->GetSubPlayerClass();
//
//	float		MainPCDefense = CombatStatEval::GetPCDefense(MainPCClass, CharacterDataStore);
//	float		MainModSpecializeDefense = CombatStatEval::GetPCDefenseByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);
//	float		MainTotalDefense = MainPCDefense + MainModSpecializeDefense;
//	MainTotalDefense = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(MainPCClass))), MainTotalDefense);
//
//	float		TagPCDefense = CombatStatEval::GetPCDefense(TagPCClass, CharacterDataStore);
//	float		TagModSpecializeDefense = CombatStatEval::GetPCDefenseByMod(TagPCClass, GetB2GameModeType(), CharacterDataStore);
//	float		TagTotalDefense = TagPCDefense + TagModSpecializeDefense;
//	TagTotalDefense = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(TagPCClass))), TagTotalDefense);
//
//	float		TotalDefense = MainTotalDefense + TagTotalDefense;
//
//	return TotalDefense;
//}

void ABladeIIGameMode::OverrideDamageInfo(FDamageInfo& OutDamage, const ABladeIICharacter* B2Char)
{
	BII_CHECK(B2Char);

	if (B2Char->StateAttackDuration > 0.f && B2Char->StateAttackType < EStateAttackType::EDT_End) // other state attack override handling
	{
		OutDamage.StateType				= B2Char->StateAttackType;
		OutDamage.StateDamageDuration	= B2Char->StateAttackDuration;
		OutDamage.StateDamageAmount		= B2Char->StateAttackAmount;
	}
}
void ABladeIIGameMode::OverridePauseForDuration(float pausetime, const ABladeIICharacter* B2Char)
{
	BII_CHECK(B2Char);
}
#if WITH_EDITOR
bool ABladeIIGameMode::IsDefaultPawnTestBP() const
{ // DefaultPCClassEnum 을 EPC_End 로 두고 BP 클래스를 테스트 용도로 spawn 하려는 의도. 정식 레벨에는 이렇게 하면 안됨. 아예 안먹힐 것.
	return (Cast<UBlueprintGeneratedClass>(*DefaultPawnClass) != NULL);
}
#endif
