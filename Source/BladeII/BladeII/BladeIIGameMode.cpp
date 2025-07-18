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
bool FStatisticalPerfStatCollector::bDoStatCollecting = false; // ���� �۵� �÷���. �ٲٰ� �Ǹ� ���� �������� ����.
bool FStatisticalPerfStatCollector::bSkipEventCinematics = false; // �Ϲ����� ���� ��鸸 �����ϰ� ���� ��� true ��. ����� �������� ����ϴ� LOD �� �ٸ��⵵ �ϰ�..
bool FStatisticalPerfStatCollector::bIsCurrentlyInEventScene = false;
float FStatisticalPerfStatCollector::SkipFrameTimeLimit = 1000.0f; // Ư�� �ѵ� �̻��� ū FrameTime �� ������ �� ���� �ε� �ð��� ���ų� �� �ɷ� �����ϰ� ��ŵ
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
	//{ // ������ ������ ǥ��
	//	DisplayResultStats(InB2GM);
	//}
	//else
	//{ // ���� ������ ���� ��		
	//	if (!bSkipEventCinematics || !bIsCurrentlyInEventScene)
	//	{
	//		double FrameDiffTime = FApp::GetCurrentTime() - FApp::GetLastTime();
	//		double FrameDiffTimeMs = FrameDiffTime * 1000.0;
	//		if (FrameDiffTimeMs >= (double)SkipFrameTimeLimit) // �ʹ� ū ������Ÿ���� �ε� ���� ù �������� �� �ִ�..
	//		{
	//			++SkippedFrameNum;
	//			SkippedFrameTimeAvg += FrameDiffTimeMs; // ��������� �ϴ� ���س��� ���Ŀ� ��� ��.
	//		}
	//		else
	//		{
	//			// FStatUnitData ����.. ���ʼ��� ���� ��ġ�鿡 0.9 ����ġ�� ������ �ؼ� �Ｎ LPF �� �Ŵµ� ���⼱ �ϴ� ���� �� ��踦 ����.
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

	// �׳� ���
	//Result_GameThreadTime_Avg = GameThreadTimes.GetTotalAverage();
	//Result_RenderThreadTime_Avg = RenderThreadTimes.GetTotalAverage();
	//Result_GPUTime_Avg = GPUTimes.GetTotalAverage();
	//Result_FrameTime_Avg = FrameTimes.GetTotalAverage();
	//Result_DrawCall_Avg = DrawCalls.GetTotalAverage();
	//Result_PolyCount_Avg = PolyCounts.GetTotalAverage();

	// ǥ������
	Result_GameThreadTime_SD = GameThreadTimes.GetStandardDeviation();
	Result_RenderThreadTime_SD = RenderThreadTimes.GetStandardDeviation();
	Result_GPUTime_SD = GPUTimes.GetStandardDeviation();
	Result_FrameTime_SD = FrameTimes.GetStandardDeviation();
	Result_DrawCall_SD = DrawCalls.GetStandardDeviation();
	Result_PolyCount_SD = PolyCounts.GetStandardDeviation();

	// ���� 10% ���
	//Result_GameThreadTime_Upper10 = GameThreadTimes.GetRangedAverage(true, 0.1f);
	//Result_RenderThreadTime_Upper10 = RenderThreadTimes.GetRangedAverage(true, 0.1f);
	//Result_GPUTime_Upper10 = GPUTimes.GetRangedAverage(true, 0.1f);
	//Result_FrameTime_Upper10 = FrameTimes.GetRangedAverage(true, 0.1f);
	//Result_DrawCall_Upper10 = DrawCalls.GetRangedAverage(true, 0.1f);
	//Result_PolyCount_Upper10 = PolyCounts.GetRangedAverage(true, 0.1f);

	if (SkippedFrameNum > 0 && SkippedFrameTimeAvg > 0.0)
	{ // ��������� �̰͵� ���� ���.
		SkippedFrameTimeAvg /= (double)SkippedFrameNum;
	}

	bDataAvailableForDisplay = true;
}

void FStatisticalPerfStatCollector::DisplayResultStats(ABladeIIGameMode* InB2GM)
{
#if WITH_BII_ON_SCREEN_DEBUG_TEXT // Shipping �������� PerfStatCollector �� ���� �Ǹ� WITH_BII_ON_SCREEN_DEBUG_TEXT �� ����..
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

	// �� �Ʒ��� ��Ÿ �߰� ���� ����. �ٷ� ���� �� ����ϱ� ����
	DrawX = 10.0f;

	DrawY += (float)((FontSize + LineDist) * 1);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("Skipped Frames : %.2f ms (%d)"), SkippedFrameTimeAvg, SkippedFrameNum), FLinearColor::Green, FontSize);

	DrawY += (float)( (FontSize + LineDist) * 2 );

	UWorld* TheWorld = InB2GM ? InB2GM->GetWorld() : nullptr;
	ERHIFeatureLevel::Type RHIFeatureLevel = (TheWorld && TheWorld->Scene) ? TheWorld->Scene->GetFeatureLevel() : ERHIFeatureLevel::Num;
	// UEnum �� �ƴ϶� ���� ���ڿ� ������ �ȵ� �Ѥ�;
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
		PlatformAPIStr = TEXT("iOS Metal"); // Metal ���� �̰� �³� ����.
	}
	else{
		PlatformAPIStr = TEXT("iOS");
	}
#endif

	// ����� ������ ��ĥ �� �ִ� ���� ȯ�� �� �������� ǥ��.
	{
		Scalability::FQualityLevels CurrScalabilityLevel = Scalability::GetQualityLevels();
		int32 TotalAdded =
			CurrScalabilityLevel.AntiAliasingQuality +
			CurrScalabilityLevel.EffectsQuality +
			CurrScalabilityLevel.PostProcessQuality +
			//InScalabilityLevel.ResolutionQuality + �̰� ��ġ�� �� �ٸ��� ����..
			CurrScalabilityLevel.TextureQuality +
			CurrScalabilityLevel.ShadowQuality +
			CurrScalabilityLevel.ViewDistanceQuality +
			CurrScalabilityLevel.GameplayOnlyLODQuality;
		// Ȥ���� �߰��� Scalability �׸� ���� ó���� �ʿ��ϱ� �ѵ� �׷��� �����ϰԱ��� �� �ʿ�� ���� ��.
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
	{ // �߰��� ���� ���Ӹ�� ���ݿ� ���� ǥ���� �� ����.
		InB2GM->DrawAdditionalPerfStatInfo(DrawX, DrawY + (float)(FontSize + LineDist));
	}
#endif
}
#endif

#ifdef BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
// �ε� �ð� ���� ��. 
// �ε� �ð��̾� �׳� ���� ������ �� �ƴϳİ� �� �� ������ �̷����� ��� �׽�Ʈ�� ���鼭 ������ 1~2 �� ����Ǵ� �� üũ�� �ʿ䰡 �־ ����.
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
		// ���� �ڵ� �����δ� ������ ��ġ���� �ε� �ð� üũ�� �Ѵٰ� �ص� ������������� Ÿ�̹����� ���� ���� ü�� �ε��ð��� �ٸ� �� �ִ�.
		//FlushRenderingCommands(); // �ٵ� �̷��� �ص� �ҿ��� ���� �� ����. �ᱹ �̰� ��������θ� ���� ���� �ε� �ð� ������ ���� �����ġ��...
		gLoadTimeCheckBeginMark = false;
		double LoadingTimeTaken = FPlatformTime::Seconds() - gLastLoadStartTime;
		UE_LOG(LogBladeII, Log, TEXT("[DevCheckLoadingTime] LastLoadingTime %f"), LoadingTimeTaken);

		// �̰� ������ �� �� �ִ� �����̷��� UIManager �� �ʱ�ȭ �� ���¿��� ��.
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
//	//{// ���� ���ӿ��� �� ���� ó���� true ���� �ؼ� �⺻�� true �� �����ϴ� ���� DLCFrontMap �� ��ġ�� �ʴ� ���߿� �׽�Ʈ�� ���� ���� ������ �� �ٲ���.
//	//	GMinimalDLCFrontMode = false; // �׸��� ini �� �̰� true �� �ؼ� �����ߴ� �ϴ��� false �� �ٲ� ������ ���� ��� ���� �ǵ��� �ٸ� ������ �� �� ���� �״� ��������
//	//}
//	//	
//	//if (!GMinimalDLCFrontMode){ // DLC Front ��� ���ҽ��ε� �ִ��� ����
//	//	FString PreLoadingScreenWidgetClassPath;
//	//	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("PreLoadingScreenWidgetClass"), PreLoadingScreenWidgetClassPath, GGameIni);
//	//	static ConstructorHelpers::FClassFinder<UB2PreLoadingScreen> PreLoadingScreenClassFoundSet(*PreLoadingScreenWidgetClassPath);
//	//	PreLoadingScreenWidgetClass = PreLoadingScreenClassFoundSet.Class;
//	//	// �⺻�� �� �� �Ȱ���..
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
	//	// �Ϲ����� Local���� ��� (Stage, ������ ž, �ݰݴ���, ������)�� Attacker�� Local�� �� ���

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
	// ���� Ŭ����, Ư�� StageGameMode ���� �ʿ��� �ֵ鸸 �־ �����ϵ��� �ϱ� ����..
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}
	return RetArray;
}
TArray<EPCClass> ABladeIIGameMode::GetPCClassesToMatineePreLoad()
{
	return GetPCClassesToPreLoad(); // �Ϻ� ���Ӹ�忡�� �̰� �ƴ� ��찡 ���� ��.
}
TArray<FCombinedPCSkillAnimID> ABladeIIGameMode::GetPCSkillAnimsToPreLoad()
{
	TArray<FCombinedPCSkillAnimID> RetArray;

	//TArray<EPCClass> AllPCClassToPreLoad = GetPCClassesToPreLoad();

	//for (EPCClass ThisPCClass : AllPCClassToPreLoad)
	//{
	//	// �⺻ ������ �� PCClass ���� ������ ��ų �ִϸ��̼ǵ��� ���� �� ����. 
	//	// ���� GameMode Ŭ���������� �� �÷��̾� ĳ���� ���� ������ Skill ���¿� ���� �ʿ��� �͸� ���� �� �ֵ��� �ؾ� ��.
	//	// �̸� ���ؼ��� CharacterDataStore �� �ʿ��ϹǷ� ��Ʈ��ũ �����̶�� ĳ���� ���� iteration �� ���Ƽ� ������ skill ���� ��ƿ;���, PCClass �� iteration �ϸ� �ȵ� ����.
	//	// �׷��� �Ϻ� ��Ʈ��ũ ��� ��忡���� ���� ��Ī�� �Ǳ� ������ CharacterDataStore �� ���� ���̹Ƿ� Preload ������ �Ұ����� ���� �ִ�.

	//	TArray<FCombinedPCSkillAnimID> AllAnimIDOfThisClass = GetAllPCSkillAnimsOfClass(ThisPCClass);
	//	RetArray.Append(AllAnimIDOfThisClass);
	//}
	return RetArray;
}

TArray<FCombinedPCSkillAnimID> ABladeIIGameMode::GetAllPCSkillAnimsOfClass(EPCClass InCharClass)
{ // ��ƿ ����. �� ���⿡ ���� �ʿ�� ����.
	TArray<FCombinedPCSkillAnimID> RetArray;
	for (int32 SAI = 0; SAI < static_cast<int32>(ESkillAnimType::ESA_End); ++SAI)
	{
		ESkillAnimType ThisAnimType = static_cast<ESkillAnimType>(SAI);
		if (ThisAnimType != ESkillAnimType::ESA_Weapon_Normal_End) // �̰� �߰��� �� �ĺ��Ѵ�ð� ���� �Ŵ� ����
		{
			RetArray.Add(FCombinedPCSkillAnimID(InCharClass, ThisAnimType));
		}
	}
	return RetArray;
}

// �⺻������ Main / Sub �ΰ��� �ε��ϰ� GameMode���� �����Ѵ�. ex ) ������ - ��, ���̵�, ������ - �ϳ�
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

 //GatherEquippedSkillAnimIDs �Լ��� ������ �Ͽ� ���� ��庰 �ʿ� Class SkillAnim���� AsyncLoad
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

	//// TAsset �ε��ϴ� InfoAsset ���� �����. �ݴ�� ManualUnloadBeforeNewBeginning
	//
	//UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	//if (PCBox)
	//{
	//	B2_SCOPED_TIME_LOG(TEXT("PreloadAnyNecessaryInfo > PCClassInfo"));
	//	// �̰� ���� �ð� �� ���� ���̰� PCClassInfo �߿��� ���� ��� InGameOnlyInfo �� ���� ���� ��. �װ� Async �ε� ����̶� �̰� ���Ŀ� ��.
	//	TArray<EPCClass> ClassesToLoad = GetPCClassesToPreLoad();
	//	PCBox->PreloadClassAssets(ClassesToLoad);
	//}
	//
	//{
	//	// SpawnPool ���� spawn �� �� �ִ� ��� NPC class �� preload �ϴ� �� �ƴϰ� ���⼱ ���� ���� ���� Async ��� �����ϰ�.
	//	B2_SCOPED_TIME_LOG(TEXT("PreloadAnyNecessaryInfo > MobClassInfo"));
	//	if (TheActiveSpawnPool)
	//		TheActiveSpawnPool->PreloadPoolMonsters();
	//}

	//B2GMLoadingProgCollector::StepCustom(bAboutToPlayLoadingMovie ? 0.4f : 0.1f);
}

void ABladeIIGameMode::PreloadInGameInfoAsset_SynchronousOnly(bool bAboutToPlayLoadingMovie)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::PreloadInGameInfoAsset_SynchronousOnly"));

	// �� Synchronous �ε��� FlushAsyncLoading �� �����Ѵٸ� �ٸ� async ��û ������ �ؾ� �ϰ�,
	// �װ� �ƴ϶�� �̰� ������ �ٸ� async ��û ���Ŀ� �����μ� ���� ������� �ε��� ���ư����� �� �� ��.. ����?
		
	// TAsset ���� �ٲ� Matinee ���۷��� ���� �ε�.. �̰� ���� �� �ε��� �Ϻο��� ���̴�. ������ ó���� ��ġ��Ŵ.
	{
		int32 LazyLoadHandledNum = B2GamePreloadMatineeRefAssets();
		B2GMLoadingProgCollector::StepCustom((float)LazyLoadHandledNum * (bAboutToPlayLoadingMovie ? 0.02f : 0.01f));
	}

	{ // DamageEffectInfo �� BuffModeEffectInfo �ε��� ���� ���൵ ������ ���� �ε� �ʿ䰡 �ִ����� ���� �ٸ���..
		if (!UB2DamageEffectInfo::AssetsAreRooted() && !UB2BuffModeEffectInfo::AssetsAreRooted())
		{ // �� �� �ε��ϰ� �Ǵ� ���
			B2GMLoadingProgCollector::SetStepRate(bAboutToPlayLoadingMovie ? 0.4f : 0.1f);
		}
		else if (UB2DamageEffectInfo::AssetsAreRooted() && UB2BuffModeEffectInfo::AssetsAreRooted())
		{
			B2GMLoadingProgCollector::SetStepRate(0.0f);
		}
		else
		{ // �� �� �ϳ��� �ε��ϰ� �Ǵ� ���
			B2GMLoadingProgCollector::SetStepRate(bAboutToPlayLoadingMovie ? 0.2f : 0.07f);
		}
	}
	UB2DamageEffectInfo* DFXInfo = StaticFindDamageEffectInfo(this);
	if (DFXInfo)
	{ // �������� ������ spike ������ �� �� ����.
		DFXInfo->PreloadAndCacheAllTAsset();
	}
	UB2BuffModeEffectInfo* BuffInfo = StaticFindBuffModeEffectInfo(this);
	if (BuffInfo)
	{
		BuffInfo->PreloadAndCacheAllTAsset();
	}
	B2GMLoadingProgCollector::Step(); // �ռ� ������ StepRate �� ���൵ ����.

	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (PCBox)
	{ // ���� ĳ���� �����ʹ� sync �ε����� ó����. RootSet �̸� �׳� �Ѿ�� �� ����.
		UB2PCClassInfo* ThisLoadedInfo = PCBox->GetSingleClassInfo(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
		if (ThisLoadedInfo)
		{
			// ��Ʈ�� ó�� ���ο� ������� �� �������� ��Ʈ���� �ƴϸ� �ε��� �ȵǾ� �־�� ��.
			// �־��� ��� �ٸ� PCClass InGameOnly �����͵� �� �ε��ϴ� ��쵵 �ְ�����.. ���⼱ �ϴ� Ȯ���� ���̴� ���� Step ���� ���.
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

	// �κ� ������ ���Ӹ��鿡���� ���������� �޸𸮿� �ε�Ÿ���� �Ҹ��ϴ� InGameOnlyInfo �� �ε��� ���� �ʿ䰡 �ִ�.
	// ���Ӹ�� �� �ʿ� �� ������ ���� ������ �κ��� �񵿱� �ε����� ó���� �ϵ��� �Ѵ�.
	// ���⼭ �ٷ� �ε��� �ϴ� Async ��û�� �ϴ� ū ����� ���� �ε��̶� �ñ������� ���� �޸𸮰� �ö�.
	// ��, RootSet ó���� �� �Ŷ�� �̹� �޸𸮿� �ö�� ���� �״� �ƹ� ��ȭ ������.

	// ���⼭ StaticFindPCClassInfoBox �� ���ϴ� ���� Async flush �� ����Ų�ٸ� �ܼ��� ������ �ѹ� ���� ������ �ȴ�.
	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (PCBox)
	{
		TArray<EPCClass> ClassesToLoad = GetPCClassesToPreLoad();

#if !UE_BUILD_SHIPPING // PC Ŭ���� �ε��� ���� �� �ľ�
		extern int32 gPCDataPreLoadMaxNum;
		if (gPCDataPreLoadMaxNum >= 0)
		{
			if (gPCDataPreLoadMaxNum == 0)
			{
				ClassesToLoad.Empty();
			}
			else if (gPCDataPreLoadMaxNum == 1) // �ϳ����̸� ó�� spawn �� �ָ�
			{
				ClassesToLoad.Empty();
				ClassesToLoad.Add(CurrentPCClassEnum);
			}
			else
			{
				if (ClassesToLoad.Num() > gPCDataPreLoadMaxNum)
				{
					ClassesToLoad.RemoveAt(gPCDataPreLoadMaxNum, ClassesToLoad.Num() - gPCDataPreLoadMaxNum);
					if (!ClassesToLoad.Find(CurrentPCClassEnum)) { // ���ϰ� ó�� spawn �� Ŭ������ pre-load �� �ؾ� �׽�Ʈ�� ��Ȯ�� �� ��.
						ClassesToLoad.RemoveAt(ClassesToLoad.Num() - 1);
						ClassesToLoad.Add(CurrentPCClassEnum);
					}
				}
			}
		}
#endif

		for (EPCClass ThisClassLoad : ClassesToLoad)
		{
			// ���� ĳ���� Ŭ������ ��Ʈ�� Ȥ�� �ε��� �Ǵ��� PreloadInGameInfoAsset_SynchronousOnly ���� �ε��� ����.. 
			// �ų� �ƴϸ� �� ��û ���Ŀ� Synchronous �ε��� �ؼ� �������� ���ư��� �ε��� �̵��� ���� �� ������ ������.
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
	// �ǵ������� �����.

	// ���⼭ ���� �ϴ� GameMode �� �⺻ ����� �����ϰ�
	// �ʿ��� �������� Ȯ���� ä���� �Ŀ� StageEventDirector ������ �ʿ��� �͵��� �ɷ����鼭 
	// AB2StageEventDirector::LoadLazyLoadedMatineeRefAssets �� ����ؼ� pre-load ��. (B2GamePreloadMatineeRefAssets)
}
int32 ABladeIIGameMode::B2GamePreloadMatineeRefAssets()
{
	int32 LazyLoadedSEDNum = 0;
//#if _MATINEE_REF_ASSET_LAZYLOAD // ���� �ڵ� �ʿ��� ���⿡�� ��ġ��Ű�� ���� ��� ��Ʈ���� ����.
//	TArray<FString> StageEventRefLoadExcludeFilters;
//
//#if 0 // �Ϻ� Ʈ���� ���ܽ��ѵ� ���߿� ���� �ʱ�ȭ�Ǹ鼭 �ᱹ �ε�. �׸��� �̰ɷ� ���ܵǴ� ���� �׸� ���� ���� ��. �ϴ� ���� ����.
//	TArray<EPCClass> PCClassesToLoad = GetPCClassesToMatineePreLoad();
//	if (PCClassesToLoad.Num() > 0)
//	{
//		// Preload ��� PCClass �� Exclude ���� ����
//		// ���ҽ� �¾����� ����� �����ϴ� �Ŷ� �̰� 100% ��Ȯ���� ���� ���� �ִµ�
//		// �׷� ������ �ǵ�ġ �ʰ� �ɷ����� ���⼭ �ε��� �ȵȴٰ� �ص� ū�� ���� �� �ƴϴ�. �ʿ��� �� ������ �ε��� �ȴ�. 
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
//	// ���Ӹ���� ���� ������ ���� �̰� ���ϴ� �⺻ ������ EventDirectors �迭�� �� ä���� ���� ���� �ִ�.
//	// �׷� ��� �׳� ���ֵ� ��� ��ü�� �̻��� ���� ���̳� ���� Matinee ���� ������ �ε��Ǹ鼭 ���� ��� ������ �����ϰų� async flush �� ����Ų�ٸ�
//	// �̰� �߰��� ���ؾ� �� ����.
//	for (AB2StageEventDirector* CurrSED : EventDirectors)
//	{
//		if (CurrSED && CurrSED->IsExpectedForGameMode(this))
//		{
//			CurrSED->LoadLazyLoadedMatineeRefAssets(StageEventRefLoadExcludeFilters);
//			++LazyLoadedSEDNum;
//		}
//	}
//#endif
	return LazyLoadedSEDNum; // Lazy-loading ó���� �� StageEventDirector (���� Matinee) ������ ����.
}

void ABladeIIGameMode::GameContextAsyncFlush()
{ // �ε� Ÿ�ӿ� Preload �� �� �̿ܿ� ���� �÷��� �Ǹ鼭 Async �ε� �� �͵��� flush �� �� �ִٰų� 
	// ������ �ٶ������� ���� ��Ȳ�� �ִٸ� �̰� ����ؼ� �׷� �͵��� �̿��� ó���ϱ� ���� ����
	// �̰� �ε� Ÿ�ӿ� �ҷ��� �Ѵ�.
	if (TheActiveSpawnPool)
	{
		TheActiveSpawnPool->TryAsyncLoadReservedMonsters();
		FlushAsyncLoading();
	}
}

void ABladeIIGameMode::PrebuildAndCacheCharMesh(EPCClass InCharClass, const TArray<FB2Item>& InEquippedItems, const FB2Wing* InWingData, bool bMergeSections, int32 InOptionalCharEntryID /*= -1*/)
{
	// Preload ������ �±� ĳ���� mesh merge �� �̸� �� ���� ���� �����ε�.. 
	// ���� ĳ���� mesh �� RootSet ó���� �ϴ� B2CompositeMeshCache �� ���� ���ķδ� �Ϲ� ���Ӹ�忡���� ��ǻ� �ʿ� ���� ����̴�.
	// ��� ������ �ٸ� �ɷ� �����Ǵ� Ʃ�丮�� ��忡���� �ʿ��ϰ�.. 
	// ������ PVP �� ���� �±� ĳ���͸� �̸� merge �� ���� �Ǹ� �ű⼭ �ʿ��� ���� �ֱ� �ѵ�.. �׷����� �� ������ �ʿ�.

	UWorld* TheWorld = GetWorld();
	UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox(this);
	UB2PCClassInfo* ThisCharInfo = PCInfoBox ? PCInfoBox->GetSingleClassInfo(InCharClass) : nullptr;
	if (TheWorld && ThisCharInfo)
	{
		// �Ͻ������� owner ������ �� dummy �� spawn ��Ų��.
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.ObjectFlags |= RF_Transient;
		FTransform SpawnTransform(FVector(0.0f, 0.0f, 0.0f));
		ASkeletalMeshActor* DummyOwnerSKActor = TheWorld->SpawnActor<ASkeletalMeshActor>(ASkeletalMeshActor::StaticClass(), SpawnTransform, SpawnInfo);
		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
		if (DummyOwnerSKActor && MeshCacheManager)
		{
			// LocalPlayerChar �� NULL �̾ ���� ����. ���ϰ� CompositeMeshCache �ʼ� Local ������ ó���� ��.
			ABladeIIPlayer* LocalPlayerChar = Cast<ABladeIIPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
			// �ǵ��� �� �Լ��� ��뿡�� BuiltResult �� NULL �� ���̴�. B2CompositeMeshCache ���� ���� ĳ���� �� �޽��� RootSet ó���� �ϴ� �Ϲ� �������� ��忡�� ����� �ͼ� �ȵ�.
			// �̰� NULL �� �ƴ� �ϳ��� ���ɼ��� �׽�Ʈ �߿� Ʃ�丮�� ���Ӹ�忡�� ���� �Ͱ� �Ȱ��� ��� �������� Ʃ�丮�� ���Ӹ��� ���� ��.
			USkeletalMesh* BuiltResult = MeshCacheManager->GetCachedCompositeMesh(InCharClass, LocalPlayerChar, InEquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, bMergeSections);

			if (UB2PCClassInfo::SetupSKCompForParts(InCharClass, DummyOwnerSKActor, DummyOwnerSKActor->GetSkeletalMeshComponent(), BuiltResult, ThisCharInfo->BaseMeshAsset, ThisCharInfo->DefaultPartsInfo,
				InEquippedItems, InWingData, 
				nullptr, // �޽��� �����ϴ� �� �����̹Ƿ� AnimBP ���� �� ������ �ʿ� ����
				false, // ������ ������ �� ĳ���ϴ� ������ �츮���� ���� ��Ʈ�鵵 �����ؾ� �ؼ�..
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
	// ���� ĳ������ �±װ� ���Ǵ� ��忡�� �ʿ��� ���. �� ������, ������ž, PVP
	// PVP �� ���� ���� ĳ���� �� �ƴ϶� ����Ʈ ĳ���͵� �±׸� �ϹǷ� �װͱ��� ����Ϸ��� Ȯ��� ����� �ʿ��ϴ�.

	UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
	if (MeshCacheManager && MeshCacheManager->IsLocalPCMeshRootSetAllowed()) {
		// ���� ĳ���� �޽��� RootSet ó�� ���Ŀ��� �̰� �ʿ� ���� ����. 
		// �׽�Ʈ�� ���� ��а� ���ܵ�.
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
		false // �ϴ� �⺻������ ���� ������ �Ϲ� �𵨷�.
	);

	// �±� ĳ���͸� ����ϴ� ����� ���� ���յ� ���� ����� ���ɼ��� �����ϱ�� �������.. �ڵ�� ����ΰ�.
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
//#if !UE_BUILD_SHIPPING // PreRender �¾��ϸ鼭 �ε��ϰ� �� ���� ������ �׽�Ʈ�Ϸ��� ���� ��
//	extern int32 gPCDataPreLoadMaxNum;
//#endif
//
//	return (UB2PreRenderer::MaxPreRenderCount > 0 // �� �׽�Ʈ�� ���� 0 �̸� ������ ���� �ʴ´�. �÷����̳� ��Ÿ ���ǿ� ���� ����� ���ؾ� �� ���� �ְ�..
//#if WITH_EDITOR
//		&& !GIsEditor // ������ �÷��̿��� ���� ���� ������ ������ ������ ����Ű�⵵..
//#endif
//#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS �� �޸� �Ҹ� �� ū ������ ����ϱ⵵ �ϰ� ���̴� ĳ���� ��� �ϴ��� ���� �ȵ���̵庸�� �� ������.
//		&& UB2PreRenderer::ShouldDoPreRenderForIOS() // SetupPreRenderObjects ���� 30 ~ 50MB ���� �޸𸮰� �ö󰣴�. �׸��� GPU �޸𸮿� ����� ������ �Ǵ� ���. ������ PreRenderGameMode ������ �� �� ���� �ִ�.
//#endif
//#if !UE_BUILD_SHIPPING // PreRender �¾��ϸ鼭 �ε��ϰ� �� ���� ������ �׽�Ʈ�Ϸ��� ���� ��
//		&& (gPCDataPreLoadMaxNum < 0 || gPCDataPreLoadMaxNum >= PCClassToInt(EPCClass::EPC_End))
//#endif
//		);
	return false;
}
////
void ABladeIIGameMode::SetupPreRenderObjects() 
{
	// ���̴� �������� �̸� �� ���� �ʴ� ����� �÷��������� ���̴� �������� ���� ���. ���⼭ ��ġ�� �� �ִٰ� ���� �۵� ���ϴ� �� �ƴϰ� �� �߰��� �ʿ��� �� �� �����Ÿ�.

	//check(!PreRenderer);
	//PreRenderer = NewObject<UB2PreRenderer>(this, NAME_None, RF_Transient);
	//check(PreRenderer);
	//if (PreRenderer)
	//{
	//	if (PreRenderer->SetupPreRenderObjects(this))
	//	{
	//		B2GMLoadingProgCollector::StepCustom(0.2f); // Pre-render �¾��� �ߴٸ� �� �Ծ��� ��.

	//		// ���� ���� �����̳� Ư�� ó�� ������ ���� ������� �ʵ��� �ſ� ���� TimeDilation �� �ش�.
	//		SetWholeGameTimeScale(0.0001f); // �ٸ� �������� TimeDilation �� ������ �ʵ��� �Ͻ������� WholeGameTimeScale �� ���.

	//		PreRenderPhase = EPreRenderPhase::EPRP_Selected; // PreRender ���� ��ũ. ���� �ϳ� ������ Dummy �� ������ �ϴ� Selected ��. All �� ������ ���� ó���� ��.
	//	}
	//	else
	//	{
	//		// SetupPreRenderObjects ���ж�⺸�ٴ�.. �̹� Pre-render �� ��ģ ���ĸ� �̷��� �Ǵ� �ǵ�..
	//		// �Ź� OnPreRenderComplete ��� �� ���񽺷���.
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
	//	if (PreRenderer && !PreRenderer->HasPreRenderDummyToProcess()) // �⺻ Selected �ܰ� �Ϸ�.
	//	{
	//		if (UB2PreRenderer::bPreRenderAllPrimitives) 
	//		{// �߰��� ��ü primitive �� �������..
	//			if (PreRenderer->SetupPreRenderAllPrimPhase()) {
	//				PreRenderPhase = EPreRenderPhase::EPRP_AllPrim;
	//			}
	//			else {
	//				// �̹� �ѹ� PreRender �� ��ģ �����̰ų� ��Ÿ ������ Ư���� AllPrim ó���� ���ϵ��� �� ���� ������ ��. ���� ó��
	//				OnPreRenderComplete(); 
	//			}
	//		}
	//		else 
	//		{ // Pre-render ����
	//			OnPreRenderComplete(); 
	//		}
	//	}
	//}
	//else if (PreRenderPhase == EPreRenderPhase::EPRP_AllPrim)
	//{
	//	if (PreRenderer && PreRenderer->CheckAndFinishPreRenderAllPrimPhase())
	//	{ // Pre-render ��ü ����Ŭ �� ������ ����
	//		OnPreRenderComplete();
	//	}
	//}
}

void ABladeIIGameMode::OnPreRenderComplete()
{
	check(PreRenderer);
	if (PreRenderer) {
		// AllPrim ���ο� ������� ���� ���尡 ����ϰ� �ִ� ���������� Pre-render �� �ѹ� �Ϸ�� ������ ���.
		PreRenderer->MarkCurrentWorldLevelPreRendered();
		PreRenderer->OnPreRenderComplete();
		PreRenderer->ConditionalBeginDestroy(); // ���� ���� ���� ��ũ�� �� �־�� �Ƚ��� �� ���� ���
	}
	PreRenderer = nullptr;
	
	PreRenderPhase = EPreRenderPhase::EPRP_Done;

	// Pre-render �ɸ� ���¿��� �����ߴ� ���⿡�� �Ϻ� ��Ȱ��ȭ ���״� �� ������ ����.
	for (AB2StageEventDirector* ThisSED : EventDirectors)
	{
		if (ThisSED && ThisSED->IsPlayingShow())
		{
			ThisSED->RestoreTracksAfterPreRender();
		}
	}

	ClosePreLoadingScreen(EPreLoadingScreenType::PLST_PreRender_Default);
	RestoreWholeGameTimeScale(); // ȭ�� ���� ���� ����� WholeGameTimeScale ����.
	EnsureStageEventTimeDilation(); // EventDirector �ʵ� TimeDilation �� �־��� �״� �� �� ���� ������.

	// Pre-render �� �Ѵٸ� ���� Ÿ�̸� ��ġ ������ ���� �� �������� �ε� �̹��� ��ε�. Pre-render �� ��� ��ũ���� ���� �� ���� �ְ�.
	UnloadAndClearLoadingImageState();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	// Pre-render �� �ߴٸ� �� �������� �ε� �ð� ���� ����
	DevCheckLoadingTimeEnd();
#endif
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
int32 gDetectedGarbageExcessiveRenderCount = 0;
#endif
void ABladeIIGameMode::OnPreLoadMap(const FString& InString)
{
//	//
//	// �̸��� PreLoadMap �̶� �̹� ���Ӹ�� ���� �������� ������ �� ������ �����δ� �ڱ� �ڽ��� ������ �غ� �ϴ� �κ�.
//	// �� �� ��ȯ ���� ���� World GC �Ǳ� ���� ���� �����ϱ� ���� ������.
//	// �� UObject ���� BeginDestroy ������ �ϸ� �� �ȵǴ� �͵��� ���⿡���� ������ ������ ��찡 ����.
//	//
//
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::OnPreLoadMap"));
//
//#if !UE_BUILD_SHIPPING || BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	// ExcessiveRenderProcessing �� Pre-render ���߿��� ����ϴ� Ư���� �������, ��ĩ �Ϲ����� ��Ȳ������ �귯���Դٰ��� ���ɿ� ū ����� �ȴ�.
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
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // ���� �׽�Ʈ ���忡���� ǥ��..
//		gDetectedGarbageExcessiveRenderCount = ExcessiveRenderModeCount;
//#endif
//		checkf(ExcessiveRenderModeCount == 0, TEXT("Detected %d PrimitiveComponents in ExcessiveRenderProcessing at last level transition!"), ExcessiveRenderModeCount);
//	}
//#endif
//
//	// �⺻�� ����.. ���Ӹ�� �� ����(UseCustomWaitForLoadingScreen)�� ���⼭ �����غ��� �ҿ��� �������� MoviePlayer �� ������ ������ �ʿ��ؼ�.
//	SetUseCustomWaitForLoadingScreen(false);
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	// �ε� �ð� ����.
//	DevCheckLoadingTimeBegin();
//#endif
//
//	UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//	if (MeshCacheManager)
//	{
//		MeshCacheManager->OnPreLoadMap(this);
//	}
//	
//	UnloadAndClearLoadingImageState(); // �����ؼ� �� ������ �� ������ �ֱ� �ؾ� �ϴµ� Ÿ�� �����̸� �ִ� �̻� �𸣴� ���̴�. PreLoadingScreen �����ٰ� �̰ɷ� Unload �ϰ� �ٽ� LoadingScreen ������ �ε��ϴ� �� ��������.
//	ClosePreLoadingScreenAll(); // ����(?) �������� UI �� ���۵� PreLoadingScreen �� ���� �� �־ �װ� ����.
//	SetupNextGameLoadingScreen();
//	
//	LobbyEndOnTouchImpulseRingClass<>::GetInstance().Signal(); // ó���� �����ִ� ��� ���� �ϳ� ���� ���� �� �ִ�. �κ��� �ƴϸ� �̰� ó������ �����Ƿ� Ȯ���� ������.
//	
//	// �⺻ �ڵ忡���� ���� ����� �Ҹ��� �ʴ� ���ѵ�, VirtualJoystick �� �Ź� ���� ����. ������ ���� ���װ� �־ �־�ξ����µ� ���� ����������..?
//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
//	if (B2PC)
//	{
//		B2PC->DeactivateVitualJoystick();
//	}
//
//	// �ᱹ �� ���� ������ ������ ���� �����ִ� �ɷ�. UIManager �� ���ҽ� ������Ʈ�� �������� GC �� �ȵǹǷ� ���� ������ ���� �����ؾ� �Ѵ�.
//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//	if (UIMgrInst)
//	{
//		UIMgrInst->MasterDestroy(); // �׳� Destroy ���� MasterDestroy �� �ؾ� ��.
//	}
//	if (UIManager_InGameCombat) // DJLegacy �� ������ UIManager 
//	{
//		UIManager_InGameCombat->OnPreLoadMap();
//	}
//
//	StopBGM();
//
//	// Async Load request ���� �� ��� �Ϸ� �� Ŭ����.
//	// �ٷ� �Ʒ� ManualUnloadBeforeNewBeginning �� ���� �ƶ���.
//	if (UB2AssetLoader* AssetLoader = UB2AssetLoader::Get()){
//		AssetLoader->FlushAll();
//	}
//	ManualUnloadBeforeNewBeginning(this); // InfoAsset �� �Ϻ� ���� �����ϰ� ��� �� Ŭ����
//
//	SaveLocalIniStateOnPreLoadMap(); 
//	
//	ExtraOnPreLoadMap();
//
//	IStreamingManager::Get().StreamAllResources(5.0f);
}

/** OnPreLoadMap ������ ���� �ε��� ���۵Ǵ� ���Ӹ�带 ���� �ε� ��ũ���¾�. */
void ABladeIIGameMode::SetupNextGameLoadingScreen()
{
	//IB2GameLoadingScreenModule* const LoadingScreenModule = GetB2LoadingScreenModulePtr();
	//if (LoadingScreenModule != nullptr)
	//{
	//	const FString ClosingMovieName = GetClosingMovieName();
	//	if (ClosingMovieName.Len() > 0) 
	//	{
	//		// Ư���� ���� �� �÷����� �������� �����ϴ� ���.. Subtitle �̳� ��Ÿ ���ڴ� �ʿ信 ���� ���� �߰��Ǿ�� ��.
	//		// �Ϲ����� �ó����� �������� �ƴ�. �װ� ���� �ε��� ���Ӹ���� StartPlay ���� �¾�
	//		LoadingScreenModule->StartGameLoadingScreen_Movie(*ClosingMovieName,
	//			// �׳� FitToMin ���� �ϸ� �پ��� ������ ������ ���� ��ó�� �����ϳ� AppleMovieStreamer ������ �����ؼ� IOS �Ϻ� ��⿡�� �Ͻ������� ȭ�� ������ ��Ʈ������ ������ �߻��� �� �ִ�.
	//			// DLC Ÿ��Ʋ���� �κ� �ε��� ������ ��Ÿ �ε� �������� 16:9 �� ���۵Ǵ� �� ����� StdHD �� ����.
	//			ELoadingScreenMovieAlignMode::FitToMin_StdHD,
	//			true	// Ʃ�丮�� Skip �߰�
	//		);
	//	}
	//	else 
	//	{ // �Ϲ����� �̹��� �ε� ��ũ��
	//		SetupLoadingScreenImageCommon(false, true, false);
	//	}
	//}
}
void ABladeIIGameMode::SetupLoadingScreenImageCommon(
	bool bSetImagePersistent, // ����� �ε� �̹����� (�� ���Ӹ�� ����) ���������� ����Ϸ��� true ��.
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
			InOptionalImageName ? InOptionalImageName : *GetLoadingImageName(bSetImagePersistent), // �Ϲ����� ���, InOptionalImageName �� ���� �ʰ� GameMode ��ü������ ������ ��ƾ�� ���� �̹����� ���.
			bInDisplayProgress, bInDisplayIndicator,
			FVector2D((float)ViewSizeX, (float)ViewSizeY),
			false, OptimalAspectRatio, // Ư�� ȭ������� �������� �� �߾����� ���.
			InLoadingMessageType
		);
	}
}

#if WITH_EDITOR
void ABladeIIGameMode::OnEditorEndPlayMap()
{
	// ������ �÷��� �ߴ� �ÿ��� �۵������� �ϴ� OnPreLoadMap ���۵��� ������ ���⿡.. UIManager Destroy �� ��ü������ �����.
		
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
		// ������ �������� ���س��� ���� ������ ���� �̾� ����
		// bPersistent �� ��� �������� ������ ���� �����ؼ� ������ �Ȱ��� ����� �� �ֵ��� ��.

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

		// �̰� �ٸ� ��Ȳ���� �ε� �̹����� �����ؼ� ���� ����..
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
	//	if (!IsInPreRenderPhase() && GetMoviePlayer()->IsLoadingFinished()) // ��� WaitForMovieToFinish �ĸ� IsLoadingFinished ���� �Ѵ�. �� ������ Unload �� �ص� ������ �Ͼ�� ������ �˼��� ��� �̷��� �ǵ�..
	//	{
	//		//FlushRenderingCommands(); // IsLoadingFinished ���Ŀ��� ���������� �ε� �̹����� �� ���ɼ��� �ִ� �Ŷ�� ���� ������ �÷�����..
	//		UnloadAndClearLoadingImageState();
	//	}
	//	else
	//	{// ���� �ƴϸ� �� ���. ���������� �̷��� �ɸ��� ���� ���� ��..?
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
	//if (GMP) { // GameMoviePlayer �ʿ��� ���� ������ �ؾ� �⺻ ���� ��ƾ�� ���� ���Ḧ ���� �� �ִ�.
	//	GMP->SetUseCustomWait(bUseCustomWaitForLoadingScreen);
	//}
}
void ABladeIIGameMode::WaitForLoadingScreen()
{ 
//	GlobalMarkMapLoadingProgressDone(); // 100% �� ä��� �ǵ� ��� ����� �ε� ȭ�� ������ ���̶� �̰� ���� ������ ������ 100% �� ä�����ƾ�..
//
//	// �׳� �ε� ȭ���̸� �ߴ��� �� ���� ���� �÷��̸� ������ ����� ���������� ���. �̷��� ��� �� ������ �ð��� ������ �÷��� �ǰ� ������ Skip ��ư�� �� ��
//	IGameMoviePlayer* GMP = GetMoviePlayer();
//	if (GMP) {
//		GMP->WaitForMovieToFinish();
//	}
//
//	B2Scalability::OnGameModeLoadingScreenEnd();
//
//	// �Ϲ����� �� �ε��� �ƴϸ鼭 GameMoviePlayer �� ���ٸ� �ణ ��Ⱑ �ٸ�������
//	// �Ϲ������� �� Ÿ�̹��� �̹� ���Ӹ���� �ε��� �Ϸ�ǰ� �ε� ��ũ������ ���� ������ �ǰڴ�.
//	OnLoadingScreenFinished();
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (!IsInPreRenderPhase())
//	{
//		// Pre-render �� �� �Ѵٸ� �� �������� �ε� �ð� ���� ����
//		DevCheckLoadingTimeEnd();
//	}
//#endif
}
void ABladeIIGameMode::ConditionalWaitForLoadingScreen() // WaitForMovieToFinish ������ üũ�� �ϹǷ� ���� �̷��� �� �ص� �� �� ���� �ѵ�..
{
	IGameMoviePlayer* GMP = GetMoviePlayer();
	if (GMP && GMP->IsMovieCurrentlyPlaying()) {
		WaitForLoadingScreen();
	}
}
void ABladeIIGameMode::OnLoadingScreenFinished()
{
	/*
	 �̹� ���Ӹ���� �ε��� ���� ������ �� �� �ִµ�
	 �Ϲ������δ� �̰ͺ��� PreBeginPlay, BeginPlay ���� �ٸ� "ģ����" ������ ����ϴ� �� ����.
	 
	 �ε� ȭ���� �񵿱�� ���ư��鼭 Ư�� �������� ���� ���ξ����尡 �غ�� ���Ŀ��� ���ξ����带 ���� ������ 
	 �Ϲ������δ� �ε� ȭ���� ���ư��� ���� ��ġ �ʴ� ������ ��׶���� ���ư��� �Ϳ� ���� ����� �ʿ�� ����.

	 �׷��� ��Ȥ ���� ��Ȳ�̶� �� �ֱ� ������ (���� �ܺ� �÷��� ������ ��ȣ�� �����ٰų� �ϴ� ����)
	 �׷� ���� �ᱹ ���⼭ �ε� ��ũ���� ������ Ÿ�̹��� �˾ƿ;� ��.

	 ���Ӹ�� ���ݿ� ���� override �ؼ� ���
	*/ 
}

void ABladeIIGameMode::OpenPreLoadingScreen(EPreLoadingScreenType InPreLoadType, EB2GameMode GameMode /*= EB2GameMode::Unknown*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::OpenPreLoadingScreen"));

	ClosePreLoadingScreen(InPreLoadType); // ���� �ϳ� �ߺ��ؼ� ���ϰ� �� �� ������ ó�� �����ָ� �� ������..
	
	CachedPreLoadingGameMode = GameMode;

	// ���� �̰� ������ �׷� �� �ƴѵ� OnPreLoadMap ���� B2GameLoadingScreenModule �� �ε� ��ũ�� ���� �� �ε� �̹��� �ε��� Ȯ���� �ϴ� ������ �ִ� �� ��.

	// InPreLoadType ���� �߰��Ǹ� �ʿ信 ���� ���
	//APlayerController* LocalPC = UGameplayStatics::GetLocalPlayerController(this);
	APlayerController* LocalPC = GetWorld()->GetFirstPlayerController();
	TSubclassOf<class UB2PreLoadingScreen>& UsedWidgetClass = IsOneOfPreRenderScreenType(InPreLoadType) ? PreLoadingScreenWidgetClass_PreRender : PreLoadingScreenWidgetClass;
	if (UsedWidgetClass && LocalPC)
	{
		UB2PreLoadingScreen* LocalCreated = CreateWidget<UB2PreLoadingScreen>(LocalPC, UsedWidgetClass);

		if (LocalCreated)
		{
			// PreRenderScreen �� �ϴ��� ������ ó��.. �ƴϸ� ������ ���⼭ PreRender �������� ����� �̹����� �ε��ϰ� �� ���� �ְ�.
			UTexture2D* LoadedLoadingImage = nullptr;
			if (IsOneOfPreRenderScreenType(InPreLoadType))
			{
				if (InPreLoadType == EPreLoadingScreenType::PLST_PreRender_Default && !LastUsedLoadingImageName.IsEmpty())
				{
					LoadedLoadingImage = LoadObject<UTexture2D>(nullptr, *LastUsedLoadingImageName);
				}
				// PLST_PreRender_First ������ �׳� NULL.. UB2PreLoadingScreen �ȿ��� ó��.
			}
			else
			{
				// �Ϲ����� �� �ε� ���� ���̽�.
				LoadedLoadingImage = GetLoadingImageTexture(GameMode, true);
			}
			
			LocalCreated->AddToViewport(BII_WIDGET_ZORDER_PRELOADINGSCREEN);
			LocalCreated->SetPreLoadingType(InPreLoadType, LoadedLoadingImage);

			if (IsOneOfPreRenderScreenType(InPreLoadType)) {
				CreatedPreLoadingScreen_PreRender = LocalCreated; // ��� ����..
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
		if (CreatedPreLoadingScreen_PreRender) // ��� ���� ������..
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
//	if (GIsEditor && !FGameDelegates::Get().GetEndPlayMapDelegate().IsBoundToObject(this)) { //��ø ���ε� �ȵǰ�
//		FGameDelegates::Get().GetEndPlayMapDelegate().AddUObject(this, &ABladeIIGameMode::OnEditorEndPlayMap);
//	}
//#endif
//
//	if (!bDeferAutoBeginAuto) // ���� Match Start ������ �ʾ����� ���Ӹ��� bDeferAutoBeginAuto �� True �� �ΰ� ������ ������ TryAutoBeginAutoAtBeginning �� ���� ���ϵ���.
//	{
//		TryAutoBeginAutoAtBeginning();
//	}
//
//	if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
//		DocBattle->ClearBattleDataForStageClear();
//
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // ���� �׽�Ʈ ���忡���� ǥ��..
//	if (gDetectedGarbageExcessiveRenderCount > 0)
//	{
//		BII_SCREEN_LOG(FString::Printf(TEXT("[WARNING] Found %d objects in ExcessiveRenderProcessing!"), gDetectedGarbageExcessiveRenderCount),
//			FLinearColor(1.0, 1.0f, 0.0f, 1.0f), 18, 10.0f);
//		gDetectedGarbageExcessiveRenderCount = 0;
//	}
//#endif
//
//	// UAnimNotifyState_B2CameraAnim::SetBGVisibility ���� �����س�� �ʱ�ȭ
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
	//{ // ���� ������ �ʴ� ���Ӹ�尡 �����Ǿ��ٰ� ���ŵ� �� ����. Subscribe �� �ߴ� ���Ӹ���� ��츸 Unsubscribe. Unsubscribe �� GameMode �� unique �ϴٰ� �����ϹǷ�.
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
//		|| 1  // ���� ���� ������ �÷��� �ø���
//#endif
//		) 
//	{
//		RealtimeUpdateUIDoc();
//	}
//
//	if (IsInPreRenderPhase()) 
//	{
//		UpdateForPreRenderObjects(); // �̰� ���ư��� ���߿��� ���� ���� ���� �� �� ��..
//
//		// Pre-render ���� ���� �� �ε� ȭ�� �ݴ� �� ������Ű�� �ǹǷ� ���⼭ ó��. Pre-render �� �ε� ȭ���� ���� �ְ� �̰� ���� �ε� ������ ���ư��� �� �����ϱ� ���� �����̴�.
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
//	// ����� ���̴� ������ �α� �÷���
//	if (GDevCheckMobileShaderCompile)
//	{
//		// �Ʒ� �α� ǥ���ϴ� ���� ���� �����忡�� GShaderCompileCheck_AllCompiledNotLogged �̰� �߰��� ���ɼ� �ּ�ȭ. 
//		// ��� �����ۿ� �ΰ������� ���� ���߿� �ڵ��̴� lock ���� �� �� �ɰ� ������..
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
//	// �� �ٸ� ���� ���� ���� �α� �÷���
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
//		// �ƿ� ó������ spawn �� �ϸ鼭 �ϳ��� �ε��ϰ� �Ǵ� 0 �̸� �⺻ spawn �� ���� �ʰ� �Ѵ�.
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
//		World->ForceGarbageCollection(); // ���� transition �ϸ鼭 �翬�� �� �ɷ� �����ߴµ� ������ ���� ������Ʈ�� �� �����ְ� GC �� ������ ���� �� ���ŵȴ�.
//	}
	
#if WITH_EDITOR
	bDevCheckBladeIIGameModeEnteredOnce = true; // �� �ѹ� ���Խ� ��ũ�� ��. �ܼ� üũ�뵵.
#endif

//	B2Scalability::OnGameModeStartPlay(this);
//
//	{
//		// ���⼭ �ֿ� InfoAsset �鿡 ���� �̸� StaticFind~~ �� �� ���µ� 
//		// �̵��� ó�� ȣ���ϴ� ��Ȳ������ Async �ε����� flush �Ǳ� ������ �̸� �ѹ� �ҷ��ִ� ��.
//		// �̷��� �� ���Ƶ� �ٸ� �ڵ忡�� ���� StatiFind~~ �� WorldContextObject ���� ����ϸ� ���繬.
//		// ��κ� �����⸸ �������� �Ŷ� �׸� ���� �� �ɸ�.
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
//		// ���� BladeIiGameMode �ʿ� ĳ���� ���� �� �ƴѵ� ���ϰ� ���ʵ� ���� �ѹ��� �� �Ҹ� ���¿����� �������� ������ �߱��� �� ����.
//		StaticFindDialogInfoTable();
//		StaticFindDialogSoundInfoTable();
//		StaticFindQuestTable(); // �������� Ŭ���� ������ QuestCompleteNotify �߸鼭 ó�� �Ҹ��� flush ���� ��.
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
//	// �ٸ� Actor ���� BeginPlay �� �Ʒ� Super::StartPlay ���� ���۵�.
//	PreBeginPlay(); // ���� ó�� Ÿ�̹� ������ ���� ������ �߻��ϸ� WaitForMovieToFinish ���� �ڷ� ������ �� ���� �ִ�.
//		
//	// �ε� ���൵ ���� ���� �̸� ���뿡 �˾ƾ� ��. �������� �÷����ϰ� �Ǹ� �ε� ���൵�� ���� ���� ä�� ����.
//	const FString PossibleOpeningMovieName = GetOpeningMovieName();
//	const bool bSupposedToPlayMove = (PossibleOpeningMovieName.Len() > 0);
//
//	// TSoftObjectPtr ������ ���� ������ �ε��� �ȵǾ� �ִ� asset �� �� �̹� ������������ �ʿ��� ������ ����Ǵ� �͵��� �ε��Ѵ�. 
//	// Synchronous �ε��� ���ԵǹǷ� �Ʒ� ��ġ�鼭 �ð��� �� �ɸ�. �׷��� ��¥ ū �� Async �ε��� ��.
//	PreloadAnyNecessaryInfo(bSupposedToPlayMove);
//
//	// Preload �� ��������� �ٸ� ��������.. ����� ���̴� �������� ���� Pre-render
//	if (ShouldSetupPreRender())
//	{
//		SetupPreRenderObjects();
//	}
//
//	IStreamingManager::Get().StreamAllResources(2.0f);
//	
//	////////////////////////////////////////////
//	bool bPlayingOpeningMovie = false; // ���� �÷��� ����..
//	{ // �������� ���� ������ �ִ� ��� ������ �÷���
//	  // ���� ���� �ε� (PreloadAnyNecessaryInfo) ���� ������ �ϸ� ������ Ȥ���� ���� �ε� �� �Ǳ� ���� ������ �������� �� �ִ�. ������ �ϰų� �ƴϸ� �׳� � ȭ���� ��������.
//		// ���� BeginPlay �� ��ġ�鼭�� �̰����� �� �״� ���뼭 Ʋ���.		
//		if (bSupposedToPlayMove)
//		{
//			// �� ��� PreloadAnyNecessaryInfo �� ��ġ�鼭 ���൵�� �ٸ� ��캸�� ���� �ö��� ��.
//			// ������ ä����� �Ʒ� AsyncFlush ��Ұ� �־.. ������ �÷��̵� �ε� ��ũ���� ��ŭ ä������ ���� ��ٸ����� �ణ ���ܵ���.
//			B2GMLoadingProgCollector::StepCustom(0.1f, 0.9f); 
//
//			// �̰� Async �ε� flush �� ����Ű��.. �̰� �Ⱦ��� �ص� LoadingScreenModule �ȿ��� ���������� �ε��ϴ� �� �� �־ .. �Ф� �׳� ���� �ε��� ��..
//			FString LoadingImageName = GetLoadingImageName(false);
//
//			GetMoviePlayer()->WaitForMovieToFinish(); // ���� �ε� ��ũ���� �����.
//			
//			// AndroidThunkCpp_StageMoviePlay(MovieName); ��Ʋ���� �� �÷��� ��ü ������� �̷� ������ ������ �÷��̸� �ϱ⵵..
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
//				// StartGameLoadingScreen_Movie �� ��üȭ�� ����, Skip��ư ǥ�� ���� ���� ���ڸ� �־��� �� ����. �ʿ信 ���� ���.
//				GetMovieSubtitles(LoadingScreenModule->StartGameLoadingScreen_Movie(*PossibleOpeningMovieName, 
//					// �׳� FitToMin ���� �ϸ� �پ��� ������ ������ ���� ��ó�� �����ϳ� AppleMovieStreamer ������ �����ؼ� IOS �Ϻ� ��⿡�� �Ͻ������� ȭ�� ������ ��Ʈ������ ������ �߻��� �� �ִ�.
//					// DLC Ÿ��Ʋ���� �κ� �ε��� ������ ��Ÿ �ε� �������� 16:9 �� ���۵Ǵ� �� ����� StdHD �� ����.
//					ELoadingScreenMovieAlignMode::FitToMin_StdHD,
//					true	// ���� �÷��̺��� Skip �����ϵ��� ����
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
//		// �� �������� LoadingImage �� Unload �� �ָ� ������ �� ������ ũ������ ���� ��찡 ������ �ִ�. ������ ������ �ܱ��� �� ������ ���ɼ���.. �ÿ���å���� Ÿ�̸Ӹ�.
//		UWorld* TheWorld = GetWorld();
//		if (TheWorld)
//		{
//			TheWorld->GetTimerManager().SetTimer(LoadingImageUnloadTH, this, &ABladeIIGameMode::OnLoadingImageUnloadTimerCB, 1.0f, false);
//		}
//	}
//	else
//	{ 
//		if (PreRenderer && !PreRenderer->IsInPreRenderGameMode()) {
//			// �Ϲ����� ���� �ε�Ÿ�ӿ����� PreRender ��Ȳ.
//			// SetupPreRenderObjects �ÿ��� �ȸ��� �� �־ ���⼭ Light ȯ���� �¾��ϴµ�, PreRenderGM ������ ������ Ư���� Light pass �� ���� �Ŷ� ���⼭ ���� �ʴ´�.
//			PreRenderer->ConditionalSetupPreRenderLightEnvForDynPL();
//		}
//
//		if (!bPlayingOpeningMovie) {
//			// PreRender ������ �⺻ �ε� ȭ�� �ݴ� ��ƾ���δ� ������ ���� �Ŀ� �ð� ������ �� �־ �ε� ȭ�� �ݴ� �� �� �����.
//			// �� ������ �÷����ϰ� �Ǹ� �̰� ������ �������� ������ �����ؼ� ��¿ �� ����..
//			SetUseCustomWaitForLoadingScreen(true);
//		}
//	}
//		
//	// �ε� ȭ���� �ݴ� WaitForLoadingScreen �� ��ü�� ���⼭ ������ �ƴ� ��쵵 ���� �ְ���..
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
//#if BII_STATS // PerfStat �� MatchStart �������� ������ ����.
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
	const FString& SavedKeyName = GetLocalSavedAutoStateKeyName(); // ���Ӹ�� �� Auto ���¸� ������ ini Ű ��
	if (GConfig && SavedKeyName.Len() > 0) {
		GConfig->GetInt(TEXT("AutoState"), *SavedKeyName, bRetSavedState, GB2UserSavedStateIni);
	}
	return bRetSavedState;
}

void ABladeIIGameMode::SaveLocalSavedAutoType() const
{
	const FString& SavedKeyName = GetLocalSavedAutoStateKeyName(); // ���Ӹ�� �� Auto ���¸� ������ ini Ű ��
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

	return Result; // �� ������ ���� Ŭ�������� �⺻���� ������ �ʿ䰡 �ְڴ�.
}

//void ABladeIIGameMode::TryAutoBeginAutoAtBeginning()
//{ // �� ��� �� ����ڰ� ������ auto �� ����ߴ��� ���ο� ���� �ڵ����� auto �� ������ ����. �׹s�׸���� AI �÷��̾�� �ش�ȵ�.
//	ABladeIIPlayer* LocalPlayer = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	if (LocalPlayer && ShouldStartAutoFromBeginning())
//	{
//		// Auto �� ���� ������ ���� �������ΰ��� ���� �ƴϸ� ���� ������ ���Ѵ�. ������ ������ ������ �˾Ƽ� ���۵� ����.
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
//		{ // ���� Auto ������ �� �ϴ��� UIDoc �� ������ �ؾ�.. StageEventDirector �ʿ��� Auto ������ �� ������ �̰� ó�� �� �� ���̹Ƿ�.
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
//PVP�ʿ��� AutoAIController�� �ε��ϴ� ������ �־� ����޼ҵ�� ������ B2PVPGameMode���� �������̵� ��
void ABladeIIGameMode::SetPlayerAutoType(int32 InAuto) // �Ͻ����� On/Off �� �ƴ� �� ���Ӹ�忡�� �����Ǵ� auto ����.
{
	//if (PlayerAutoBattleType != InAuto)
	//{
	//	PlayerAutoBattleType = InAuto;
	//	SaveLocalSavedAutoType();
	//}

	//// �̰� �� �ϴ� ��Ȳ������ ���� � ���� �ӽ������� Auto �� �����ٰų� �ϴ� �� �����Ѵ�. �װͱ��� �����Ϸ��� �� �����ϰ� �׷� �ʿ������ ���� ����.
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

	//// UI �� �ñ׳��� �⺻�����δ� �̰ɷθ� ����. �Ͻ����� On/Off ������ �� ������. �ٵ� �ϴٺ��� �׷��Ը� ������ �ʴ±� ��..
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
//{// bMaintainedLocalPlayerAutoState �� �����ϰ� �̰ɷδ� UI Doc ������ ���� �ʴ´�. ���� ������ �Ͻ������� off �� �� ���.
//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
//	if (LocalB2Player && LocalB2Player->IsInAutoPlay()) 
//	{
//		LocalB2Player->StopAutoPlay();
//	}	
//}
//void ABladeIIGameMode::RestoreLocalPlayerAuto()
//{ // �⺻ ���°� true �� ��츸 ����.
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
//	// �ٸ� Actor ���� BeginPlay �� �Ҹ��� ���� ó���Ǿ�� �� �͵��� ���⿡ ���ų־� ����.
//
//	BladeIIGameImpl::GetLocalCharacterData().SetCharClassNames(StaticFindPCClassInfoBox(this)); // ��ü ��� �������� �ʱ� �ѹ��� �ʿ��� �ʼ�����. AccountInfo �� �޴� ���� �����ϸ� ���� ������ ���� ���..
//
//	UpdateManagedActorList();
//
//	// It will be picked up by player's BeginPlay.
//	SetDesiredPCClassEnum();
//
//	// �����ϸ� DefaultPawnClass �� ����Ƽ�� �⺻������ ������ �״��
//	// GetCurrentPCClassInfo �� BaseBPClass �� �����ؼ��� �ȵ�. �⺻ spawn �� ����Ƽ�� ������� �ϰ� BaseBPClass �� PCClassInfo overriding ���� ������ �Ѵ�.
//#if WITH_EDITOR
//	checkSlow(!IsDefaultPawnTestBP()); // ���� �ʿ����� �ɸ��� �ȵȴ�. �Ϻ� �׽�Ʈ �ʿ����� �ɸ� �� ����. �׷� ���� �׳� �Ѿ����.
//#endif
//
//#if WITH_EDITOR
//	if (GIsEditor)
//	{ // UIManager �� ���� ���� �ε��� ������ ���� �����ϴµ� �����Ϳ����� �ȸ��� ���� �ִ� �� ����.
//		UB2UIManager::EditorGetNewInstance();
//	}
//	else
//	{// UIManager �� static �� ������ ��������µ� �𸮾� �������� ���輺�� �����Ƿ� �� ��ȯ �� destroy �� �Ѵ�. ��ȹ��ζ�� �� �������� UIManager �� ���� �����ϴ� ���̾�� ��.
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
//	//UI���Ŵ��� �ʱ�ȭ
//	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
//	if (UIDocManager){
//		UIDocManager->Init(this);
//	}
//
//	// B2UIManager_InGameCombat �� DJLegacy �ε� ������ ���. �̸��� InGameCombat �̰� �����δ� ABladeIIGameMode �� ����ϴ� ��� ���� �����ϰڱ�. SetCenterMessage ��ƿ���� �ʿ��ϱ� ��.
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
//	// ���� ��Ʈ�����̶� �Ѵٸ� PreBeginPlay ���Ķ� ���� �ҷ��� �ʿ䰡 ���� ��.
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
//		// ��� ���̿� ���� BeginPlay ������ ���� ���谡 ���� �� �־ BeginPlay ���� ���� �̸� ����� �д�.
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
//	DecideAndSetActiveSpawnPool(); // ����Ʈ �� ���� �̹� �������� ���� SpawnPool �ϳ��� ã�Ƽ� ����.
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
//		// Main �� ����ϴ� ���� �⺻�� �÷����� ���̰� Extra ���� ���� ������ ���������� �÷����� current Ŭ������ �����ؾ� ��. AB2StageGameMode::SetDesiredPCClassEnum ����.
//		CurrentPCClassEnum = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
//	}
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	else
//	{
//		// ���� �� �� ���¿��� ���� �׽�Ʈ �� �ܵ� �÷��� ��嵵 �ְ� LocalCharacterData ������ ���ÿ� ����� ���� �ε��� ������ ���� �־ �� ���� ���.
//		CurrentPCClassEnum = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
//	}
//#endif
//
//	// ÷�� ���� ���� �־�� ������.
//	if (CurrentPCClassEnum == EPCClass::EPC_End)
//		CurrentPCClassEnum = DefaultPCClassEnum;
}

void ABladeIIGameMode::SubscribeEvents()
{ 
	if (bEventsSubscribed)
		return;

	//// ��ü�� UI ���� �̺�Ʈ �ޱ� ����.
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

	bEventsSubscribed = true; // SubsResponseGetQuestRewardcribeEvents �� �ߴٸ� ������� ���̷� �����Ǵ� GameMode �� �ƴ� ��.
}

//void ABladeIIGameMode::UnsubscribeEvents()
//{
//	Issues.Empty();
//
//	bEventsSubscribed = false;
//}

void ABladeIIGameMode::DecideAndSetActiveSpawnPool(bool bFallBackToLowerDifficulty)
{ // Ư�� ���Ӹ�忡���� (StageGameMode ���) ���� ��Ȳ������ �ǹ��ִ� SpawnPool �� ������ �� ��.
	//TheActiveSpawnPool = (SpawnPools.Num() > 0) ? SpawnPools[0] : NULL;
}

//AActor* ABladeIIGameMode::FindPlayerStartForCurrStage(AController* InController)
//{
//	return FindPlayerStart(InController); // ���� StageGameMode ���̴�. ������ �־ ����� ���� ��.
//}
//
void ABladeIIGameMode::RestartGame()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::RestartGame"));

//	// AB2StageGameMode::RestartGame ������ ����� �ٸ��� �����ؼ� ����ϰ� ������ ��κ��� ���� ������ Ȯ��.
//#if WITH_EDITOR
//	if (GIsEditor)
//	{
//		// �����Ϳ����� ���� Play ������ ������.
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

	// �ݺ��Ǵ� �ڵ���� �־ �ѵ� ��Ƴ��� ���� �ִ� �� ���.
	//StartBladeIIGameTutorial(this);
}

void ABladeIIGameMode::GoToVillage()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::GoToVillage"));

	//OpenPreLoadingScreen(EPreLoadingScreenType::PLST_GoToLobby, EB2GameMode::Lobby); // �κ�� ���ư� �� �ε� ��ũ�� �����ؼ� ũ������ �� �ִµ� �̰� �� �������� �̸� �ѹ� �ε����ִ� �� �����ϴٴ� ������ �־ �� ��..

	//FLobbySceneManager::DeferredRegistChangeLobbyScene([](){
	//	LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_LobbyMain);
	//});
	//	
	//OpenBladeIILobbyCommon(this); // �� ��ȯ ���� �� ���� �κ� �� �ε�
}

void ABladeIIGameMode::GoToVillageDefeated()
{
	GoToVillage(); // ���� Ŭ�������� �ʿ信 ���� �ٸ��� ������..
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
	// �ϴ� LobbyGameMode ����.. �ΰ��ӿ����� ���� ������ �ؾ� �� �� ����.
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
	// �� ���� �������Ʈ (ALevelScriptActor) ���� ���ҽ� ĳ�� (���ε�) ������ Ư�� ������ ȣ���ϱ� ����.
	// ���� ��� BeginPlay ������ �ַ� �ϴ� �� �ٸ� Ÿ�ֿ̹��� �����ϱ� ����.

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
	//FinalSpawnParameter.bDeferPostSpawnInitialize = true; // bDeferConstruction ���� ó���� �ۿ��� ������ bDeferPostSpawnInitialize �� ���⼱ ������ ����ؾ�. 

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
	//			SpawnedActor->FinalizePCClassInfoSpawn(bSpawnAsDummyPuppet); // DummyPuppet spawn �� InGameInfo ��� ���� �����ϵ��� ���ڸ� �Ѱ��ش�.
	//			SpawnWorld->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, FinalSpawnParameter); // bDeferPostSpawnInitialize �� ���� ó��.
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
	SpawnParam.bDeferConstruction = bDeferConstruction; // �̰� true �� �ϸ� �̰� ȣ�� ���Ŀ� FinishSpawning �� ���� �ҷ��־�� �ϴ� ���ŷο��� ������ �Ϲ����� �����÷��� ��Ȳ������ ���� �÷��̾� ĳ���Ͷ�� BeginPlay ������ PlayerController �� ���� Possess �� ���¿��� �� ���̹Ƿ� �ʿ���.
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
	// GameMode ���� �� default pawn spawn �������� �Ҹ�. BeginPlay �� ������Ʈ�� register ����.
//	ABladeIIPlayer* SpawnedB2Player = Cast<ABladeIIPlayer>(InSpawned);
//	if (SpawnedB2Player)
//	{
//		SpawnedB2Player->FinalizePCClassInfoSpawn();
//		
//		// �ε� ���൵ ����.. FinalizePCClassInfoSpawn ���� BaseBP �ε��ϸ鼭 ���� �ռ� ��û�� �͵��� flush �Ǵ� �� ���� �ð��� �Ҹ�..
//		B2GMLoadingProgCollector::StepCustom(0.4f);
//
//#if WITH_EDITOR // ���� PCClass �� ���� �׽�Ʈ ��Ȳ������.. CharacterType �� ���Ƿ� �ش�. �̰Ŷ� �� �־��ָ� ũ���� ���� ���� �ʹ� ����.
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
		SpawnParam.bDeferConstruction = false; // ���� DeferContruction ���� �� �ʿ䰡 ������..
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABladeIIPlayer* SpawnedPuppet = B2GameMode->SpawnPCClassInfoPlayer(TheWorld, InClass, InSpawnTransform, SpawnParam, true, OverridePawnClass); // SpawnAsDummyPuppet ���ڸ� �ָ� InGameInfo ��� ���� spawn �� �����ϰ� �� ��.

		if (SpawnedPuppet)
		{
			if (bPossessByAIController)
			{
				// APawn::PostInitializeComponents ���� ���ǿ� ���� ������ �ϴµ�, 
				// �⺻ AutoPossessAI �� Spawned �� �ϱ⿡�� PlayerController �� possess �Ǵ� ��츦 �����ϸ� �����̰� 
				// Puppet spawn �ÿ��� ���ϰ� �߷��� �������� �⺻ AI controller �� �ʿ��ϴ� ������ �ҷ��ش�.
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
	return false; // ���� ���� ��� Ŭ�������� ����.
}

// ���� �κ񿡼� �÷��̾� ������ �̰ž���. �ٸ������� ��ε�.
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
//	// SpawnPool ���� StageEventDirector �� ���� Notify �� �� �ʿ䰡 �ִ�. 
//	// �׷��� ���� MobDying �̺�Ʈ ó�� �� WaveNum �� ObjIndex �� DyingMonster �� ������ ���� ����.
//	// ���� �� ��� delay ���� �ٷ� show �� �����ؾ� ��. �װ� �ƴ϶�� DyingMonstser �� � �����ε� ĳ���ؼ� ��� ��.
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
//	// DestroyedPrevRef �� NULL �� �ƴ� ���� IsValidObj ���� ���� ����.
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
//	// ���⼱ StageEventDirector �� ������ �ʿ������. ���� �ʿ������ٸ� �Ƹ��� SpawnPool ���ٴ� ���� ����.
//
//	if (SpawnPool)
//	{
//		SpawnPool->OnSpawnedMonsterDead(DyingMob, -1, -1, true); // ��ȯ�̸� WaveNum ���� �ʿ����.
//	}
}

void ABladeIIGameMode::NotifyForgetACharacter(class ABladeIICharacter* CharacterAboutToBeDestroyed)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifyForgetACharacter"));

//	// Notify**Dead �� ���� �÷��̻��� ������ �ǹ��Ѵٸ� �̰� �𸮾� ������Ʈ ������ ���� ������. �Ѿ�� ������Ʈ�� �����ϴ� �� �ִٸ� ��� NULL out.
//
//	BII_CHECK(CharacterAboutToBeDestroyed && CharacterAboutToBeDestroyed->IsValidObj()); // ������ Valid �ؾ߰���..
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
	//if (TheActiveSpawnPool) // ��ü SpawnPool �� iteration �� �ʿ䰡 ����� �Ѵ�.
	//{
	//	TArray<class ABladeIICharacter*> CurrList;
	//	TheActiveSpawnPool->GetWaveMobList(WaveNum, CurrList, bOnlyLiveMobs);
	//	OutList.Append(CurrList);
	//}
}

void ABladeIIGameMode::GetAllWaveMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /*= true*/)
{
//	if (TheActiveSpawnPool) // ��ü SpawnPool �� iteration �� �ʿ䰡 ����� �Ѵ�.
//	{
//		TArray<class ABladeIICharacter*> CurrList;
//		TheActiveSpawnPool->GetAllWaveMobList(CurrList, bOnlyLiveMobs);
//		OutList.Append(CurrList);
//	}
}

void ABladeIIGameMode::GetAllSummonedMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /*= true*/)
{
	//if (TheActiveSpawnPool) // ��ü SpawnPool �� iteration �� �ʿ䰡 ����� �Ѵ�.
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
//	// �����Ϳ��� �׽�Ʈ�� ���� �ϰ� �ϴ�SpawnNPC Ŀ�ǵ�� ������ �ٺ����� ���鵵 Ÿ�� üũ�� �ش�.
//	for (FActorIterator It(GetWorld()); It; ++It) // ActorIterator �̹Ƿ� �׽�Ʈ �ܿ� ����.
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
{ // StageEventDirector �ʿ��� ���� ��� ���� �Ŀ� �÷��̾� ĳ���� �ڷ���Ʈ�� ���� �ʿ��� ���.
	
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::ApplyActiveCameraIfAny"));

	//for (int32 ACI = 0; ACI < ActiveCameraActors.Num(); ++ACI)
	//{
	//	if (ActiveCameraActors[ACI]->ManualTestBeginActiveCamera(BlendInTimeOverride) == true)
	//	{
	//		return true; // �ϳ� ����Ǿ����� �� �� �ʿ� ����.
	//	}
	//}

	//// ���� ��Ƽ��� ī�޶� ������ �׳ɵ�
	//if (CurrentActiveActiveCameraActor)
	//{
	//	return true;
	//}

	return false;
}

// �⺻ ��Ƽ��ī�޶� ������ �װ� Ȱ��ȭ
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
	//if (TheActiveSpawnPool) // ��ü SpawnPool �� iteration �� �ʿ䰡 ����� �Ѵ�.
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

	// Pause menu widget ����
	//StopPauseMenuClass<>::GetInstance().Signal();

	return bSuperPauseCleared;
}

bool ABladeIIGameMode::BeginGlobalSceneBlur(float BlurScale)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::BeginGlobalSceneBlur"));
	//
	//if (bGlobalSceneBlurOn) {
	//	// Blur �Ǳ� ���� ī�޶� ������ ĳ���� ���� ������ �̹� ����Ǿ� �ִ� ���¿��� �ι� ������ ���
	//	return false;
	//}
	//
	//CachedCameraAtBlurBegin = GetCurrentActiveCamComp(this);

	//if (CachedCameraAtBlurBegin)
	//{
	//	// GlobalSceneBlur ���� �����ϱ� ���� ������ ĳ��.
	//	CachedPPBeforeBlur = CachedCameraAtBlurBegin->PostProcessSettings;
	//	CachedPPWeightBeforeBlur = CachedCameraAtBlurBegin->PostProcessBlendWeight;

	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldMethod = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldMethod = EDepthOfFieldMethod::DOFM_Gaussian;
	//	
	//	// PC ������ GaussianDOF �� Near/FarBlurSize �� ���������� Mobile ���������� Scale ������ ������. ���� �� �ٸ�
	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldScale = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldScale = BlurScale; // �ٵ� ��° ����Ͽ����� �ȸ����� ��? �� ������ �������� ���� �� ����. �� Ŀ�� ����� ���� ������ �¿�Ǵ� ���̸� ����� ���� �ǰ��� ����
	//	
	//	// ��, ���� Near/FarBlurSize �����ϴ� �� PC ���� ������ �����ֱ� ����.
	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldNearBlurSize = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldFarBlurSize = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldNearBlurSize = BlurScale * 5.0f;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldFarBlurSize = BlurScale * 5.0f;

	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldFocalDistance = 100000.0f; // �̰� �׳� ū ���� �༭ ��� ���� �ǵ��� ��.

	//	// �ѹ� FSceneView::OverridePostProcessSettings �� ��ģ �Ŀ��� ���� ������ �Ǵ� ��. Pause �޴� ���� ����� ���� ������ �� ƽ �����..

	//	bGlobalSceneBlurOn = true;

	//	return true;
	//}

	return false;
}

void ABladeIIGameMode::EndGlobalSceneBlur()
{
	if (bGlobalSceneBlurOn)
	{
		if (CachedCameraAtBlurBegin) // ���ٰ� �ؼ� �ٽ� �������� �ʴ´�. 
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

void ABladeIIGameMode::DeactivateWorldMovablePointLight() // ��� MovablePointLight �� �ƴϰ� �ʼ����� �� ��.. �뷫 ���� �� ����
{
	// Ư�� ���� ���� �ǵ����� �ʰ� ���� �ִ� movable point light �� ���� ����� �������� movable point light ������ ��Ű�� ���� �ǹ̷� ����ϴ� ��.
	// ���⿡���� matinee Ʈ���̳� �÷��̾� ĳ���� light �� �̾�޴� ������ ��ü���� ����Ʈ ������ �ϰ� �ȴ�.
	for (TActorIterator<APointLight> It(GetWorld()); It; ++It)
	{
		APointLight* ThisPLActor = Cast<APointLight>(*It);
		UPointLightComponent* ThisPLComp = ThisPLActor ? Cast<UPointLightComponent>(ThisPLActor->GetLightComponent()) : nullptr;
		// ������ ��ġ�� PointLight �� ���� ���̹Ƿ� �÷��̾� ĳ���� �Ӹ� ���� ���� �� �̰ɷ� ������ �ʴ´�.
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
//	PerfStatCollector.OnEnd(this); // �뷫 ���� Ŭ���� ������ ������ ��庰�� ���ݾ� �޶�.. OnEnd �� ���� ������ ȣ��
//#endif
//
//	SendDamageLog();
}

void ABladeIIGameMode::StartEventScene_MatchResult(bool HasLocalPlayerWon)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StartEventScene_MatchResult"));
	// ����������, PVP ��� ��� ������ �¸� �й� ������� ���ڸ� �����ִ� ����� �Ǿ� StartEventScene_GameClear �� ������ �Ǿ���.
//	for (AB2StageEventDirector* CurrSED : EventDirectors)
//	{
//		if (CurrSED){ // ���� �ٸ� NetGameMode ��� ���⵵ �������� ��Ʈ�� Ż �� �������� �𸣰ڴ�.
//			CurrSED->NotifyMatchEnd(HasLocalPlayerWon);
//		}
//	}
//#if BII_STATS
//	PerfStatCollector.OnEnd(this); // �뷫 ���� Ŭ���� ������ ������ ��庰�� ���ݾ� �޶�.. OnEnd �� ���� ������ ȣ��
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
	{ // �Ϲ������� Ambient sound Ʈ�µ� BGM �� ���� ������ ������..
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
					StopAMB(); // ó���� ������ �ʾҴٸ�
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
	check(0); // ���ο� �׸� ���� case ������ �������� ������ check �� �ɸ��� �� ��.
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
	//// �Ƹ��� ����� �ڵ� ��ũ���� �������� ������ �־ �αװ� ���� ���ͼ�.. ���� ����.
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

	//	// ���� ���� �߰��� ī�޶� Ʈ���� ���.. �̰ɷ� ����Ϸ��� ī�޶� Ʈ���� ���� ���� ��ġ�ؾ� �Ѵ�.
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
	//B2GMLoadingProgCollector::StepToMax(); // ��ü�� SetupUIScene ������ �ε��� �� ä��� ������ ��
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
//	// �̹� ���µ������� üũ����
//	if (!pUI)
//	{
//		bool bNeedOpen = false;
//		EUIScene CurrentUIScene = UB2UIManager::GetInstance()->GetCurrUIScene();
//
//		// üũ�Ҷ� ������Ӹ�峪 UI���� �����ϸ� �κ�� �ñ�UI �����ش�.
//		// �����̶� �����̶� ó���� �ٸ���.
//		if (CurrentModeState == b2network::B2ContentsModeState::SYSTEM_CHECK)
//		{
//			// ���� ���̽�
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
//			// ���� ���̽�
//			// 1:1 ������ ������ �տ��� ��������.
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
//			//case b2network::B2ContentsMode::DIMENSION: // �տ��� �����ϰ� �������
//			{
//
//				if (auto* UIHallOfFame = UB2UIManager::GetInstance()->GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
//				{
//					UIHallOfFame->SetSettleState();
//					bNeedOpen = false; // �̰� ������ �ȶ��� ��.
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
//	// ����� ���� �� ���� ���� �����ϰ� ��� ��� ����
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
//		if (HasWidget && ConditionShowMessage) // MainChatPage �� �ִٸ� ����ä���� �� �ʿ䰡 ����.
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
{ // �⺻������ ���� ���� �� TimeDilation ó���� ������ TimeDilation �� �ִ� AnimNotifyState ��� ������ ���̷� ���� �װ� ��ȿȭ�� ���� �ٽ� ������ ����.
	//for (AB2StageEventDirector* CurrSED : EventDirectors)
	//{
	//	if (CurrSED){
	//		CurrSED->ApplyStageEventTimeDilationIfPlaying();
	//	}
	//}
}

void ABladeIIGameMode::RestoreWholeGameTimeScale()
{
	//SetWholeGameTimeScale(1.0f); // �⺻ 1.0 �̰� Ư�� �������� ���� �ٸ��� �� �� ����. SetGlobalTimeDilation �� ������ ����.
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
//	//�� Tick���� �Ҹ��� ����
//	// ABladeIIPlayer::SpawnOtherAsPuppet ���� �ɷ� ������ ��� update �� ���� ���� �ȵ�.
//	if (B2Player && B2Player->IsLocalPlayer())
//	{
//		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
//		if (DocBattle)
//		{
//			DocBattle->SetCurrPCInCriticalCondition(B2Player->IsInCriticalCondition());
//			// TagPCHealth �� SkillRTManager �ʿ���..
//			if (B2Player->CurrentComboNum > 0) // 0 ���� ũ�ٸ� ������ ī��Ʈ �ϰ� ���� ����
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
//				// QTE���� �����Ҷ��� ��������.
//				&& B2Player->IsQTEStartAble()
//				&& TargetActor->GetDamagedNum() == 0
//				// �����߰�. Ÿ�ٸ���Ÿģ��(QTEEnableTeam) ���� �ȵǾ��ų�, �÷��̾����̶� ��ġ�Ҷ�.
//				// Ÿ�پ��Ͱ� ���÷��̾� �ƴϸ� Ÿ�ٸ���Ÿģ��(QTEEnableTeam) ������ �ȵǾ���������
//				&& (TargetActor->QTEEnableTeam == ETeamType::ETT_End || ETeamType(B2Player->GetTeamNum()) == TargetActor->QTEEnableTeam))
//			{//QTE 1Ÿ ���� ���� ����
//				if(TargetActor->QTEType == EQTEType::EQT_Mount)
//					DocBattle->SetQTEEnableState(EQTEUIAttackEnableTimingState::Mount);
//				else
//					DocBattle->SetQTEEnableState(EQTEUIAttackEnableTimingState::FirstAttack);
//			}
//			else if (B2Player->IsAlive() && TargetActor && TargetActor->IsAlive() && B2Player->IsTimingForSuccessiveQTE()
//				// QTE���� �����Ҷ��� ��������.
//				&& B2Player->IsQTEStartAble()
//				&& TargetActor->GetDamagedNum() == 0)
//			{// QTE 2Ÿ ���� ����
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
//	//�� Tick���� �Ҹ��� ����
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
//			{ // BossMob ���� ������ �ʱ� ��ġ���� SpawnPool ���� ����
//				DocBattle->SetBossHPCurrent(mob->Health < 0 ? 0 : mob->Health);
//				if (mob->IsQTEBreakable()){
//					DocBattle->SetBossQTEBreakPointCurrent(mob->GetCurrentQTEBreakPoint());
//				}
//			}
//			// ����Ʈ �� Boss �� �ϳ��� �־�� �Ѵ�. ���⼭ �ְ��� �ϴ� ��ġ�� ���� UI �� ������ ��ġ�� �ǹǷ� ���� �ϳ� ���� �ִٰ� ��� �־���� ������ ��.
//			break;
//		}
//	}
}

void ABladeIIGameMode::ShowMissionDialog()
{
//	//Ʃ�丮�� ���� �߿��� ���̾�α� â�� ���� �ʾƾ� �Ѵ�.
//	if (TutorialManager::GetInstance().HasAnyTutorial())
//		return;
//
//	UB2UIManager* UIManager = UB2UIManager::GetInstance();
//	if (UIManager != nullptr)
//	{
//		// Dialog ����ֱ�
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
//	//������ ���� �ö󰡴� ��
//	CharacterTakeDamageEvent.Broadcast();
//	CharacterTakeDamageEvent.Clear();
//
//	//�״� ��(OnDeath���� Undeath���¸� �����ϱ� ������ �� �Ʒ� �־�� ��)
//	CharacterDeathEvent.Broadcast();
//	CharacterDeathEvent.Clear();//[fixme][@AKI, 170815]������ ���̴µ�...Timer�� �ɾ ���� �ұ�??
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
{ // DefaultPCClassEnum �� EPC_End �� �ΰ� BP Ŭ������ �׽�Ʈ �뵵�� spawn �Ϸ��� �ǵ�. ���� �������� �̷��� �ϸ� �ȵ�. �ƿ� �ȸ��� ��.
	return (Cast<UBlueprintGeneratedClass>(*DefaultPawnClass) != NULL);
}
#endif
