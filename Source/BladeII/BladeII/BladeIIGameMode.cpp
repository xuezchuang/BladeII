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
#include "BladeIIUtil.h"

#if BII_STATS
bool FStatisticalPerfStatCollector::bDoStatCollecting = false; // 皋牢 累悼 敲贰弊. 官操霸 登搁 促澜 饭骇俊辑 利侩.
bool FStatisticalPerfStatCollector::bSkipEventCinematics = false; // 老馆利牢 傈捧 厘搁父 螟沥窍绊 酵篮 版快 true 肺. 楷免苞 傈捧俊辑 荤侩窍绰 LOD 啊 促福扁档 窍绊..
bool FStatisticalPerfStatCollector::bIsCurrentlyInEventScene = false;
float FStatisticalPerfStatCollector::SkipFrameTimeLimit = 1000.0f; // 漂沥 茄档 捞惑狼 奴 FrameTime 捞 皑瘤等 扒 购啊 肺爹 矫埃捞 甸绢艾芭唱 茄 吧肺 埃林窍绊 胶诺
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
	//{ // 荐笼茄 蔼甸阑 钎矫
	//	DisplayResultStats(InB2GM);
	//}
	//else
	//{ // 酒流 单捞磐 荐笼 吝		
	//	if (!bSkipEventCinematics || !bIsCurrentlyInEventScene)
	//	{
	//		double FrameDiffTime = FApp::GetCurrentTime() - FApp::GetLastTime();
	//		double FrameDiffTimeMs = FrameDiffTime * 1000.0;
	//		if (FrameDiffTimeMs >= (double)SkipFrameTimeLimit) // 呈公 奴 橇饭烙鸥烙篮 肺爹 流饶 霉 橇饭烙老 荐 乐促..
	//		{
	//			++SkippedFrameNum;
	//			SkippedFrameTimeAvg += FrameDiffTimeMs; // 曼绊侩栏肺 老窜 歹秦初绊 弥饶俊 乞闭 晨.
	//		}
	//		else
	//		{
	//			// FStatUnitData 俊辑.. 弊率辑绰 扁粮 荐摹甸俊 0.9 啊吝摹肺 喉坊爹阑 秦辑 溜籍 LPF 甫 芭绰单 咯扁急 老窜 葛篮 饶 烹拌甫 辰促.
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

	// 弊成 乞闭
	//Result_GameThreadTime_Avg = GameThreadTimes.GetTotalAverage();
	//Result_RenderThreadTime_Avg = RenderThreadTimes.GetTotalAverage();
	//Result_GPUTime_Avg = GPUTimes.GetTotalAverage();
	//Result_FrameTime_Avg = FrameTimes.GetTotalAverage();
	//Result_DrawCall_Avg = DrawCalls.GetTotalAverage();
	//Result_PolyCount_Avg = PolyCounts.GetTotalAverage();

	// 钎霖祈瞒
	Result_GameThreadTime_SD = GameThreadTimes.GetStandardDeviation();
	Result_RenderThreadTime_SD = RenderThreadTimes.GetStandardDeviation();
	Result_GPUTime_SD = GPUTimes.GetStandardDeviation();
	Result_FrameTime_SD = FrameTimes.GetStandardDeviation();
	Result_DrawCall_SD = DrawCalls.GetStandardDeviation();
	Result_PolyCount_SD = PolyCounts.GetStandardDeviation();

	// 惑困 10% 乞闭
	//Result_GameThreadTime_Upper10 = GameThreadTimes.GetRangedAverage(true, 0.1f);
	//Result_RenderThreadTime_Upper10 = RenderThreadTimes.GetRangedAverage(true, 0.1f);
	//Result_GPUTime_Upper10 = GPUTimes.GetRangedAverage(true, 0.1f);
	//Result_FrameTime_Upper10 = FrameTimes.GetRangedAverage(true, 0.1f);
	//Result_DrawCall_Upper10 = DrawCalls.GetRangedAverage(true, 0.1f);
	//Result_PolyCount_Upper10 = PolyCounts.GetRangedAverage(true, 0.1f);

	if (SkippedFrameNum > 0 && SkippedFrameTimeAvg > 0.0)
	{ // 曼绊侩栏肺 捞巴档 蝶肺 乞闭.
		SkippedFrameTimeAvg /= (double)SkippedFrameNum;
	}

	bDataAvailableForDisplay = true;
}

void FStatisticalPerfStatCollector::DisplayResultStats(ABladeIIGameMode* InB2GM)
{
#if WITH_BII_ON_SCREEN_DEBUG_TEXT // Shipping 滚傈俊辑 PerfStatCollector 甫 静霸 登搁 WITH_BII_ON_SCREEN_DEBUG_TEXT 档 鞍捞..
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

	// 弊 酒贰俊 扁鸥 眠啊 曼绊 沥焊. 官肺 胶鸡 嘛绢辑 扁废窍扁 亮霸
	DrawX = 10.0f;

	DrawY += (float)((FontSize + LineDist) * 1);
	DrawDebugText(DrawX, DrawY, FString::Printf(TEXT("Skipped Frames : %.2f ms (%d)"), SkippedFrameTimeAvg, SkippedFrameNum), FLinearColor::Green, FontSize);

	DrawY += (float)( (FontSize + LineDist) * 2 );

	UWorld* TheWorld = InB2GM ? InB2GM->GetWorld() : nullptr;
	ERHIFeatureLevel::Type RHIFeatureLevel = (TheWorld && TheWorld->Scene) ? TheWorld->Scene->GetFeatureLevel() : ERHIFeatureLevel::Num;
	// UEnum 捞 酒聪扼辑 寸厘 巩磊凯 函版捞 救凳 ぱぱ;
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
		PlatformAPIStr = TEXT("iOS Metal"); // Metal 备盒 捞芭 嘎唱 隔扼.
	}
	else{
		PlatformAPIStr = TEXT("iOS");
	}
#endif

	// 搬苞俊 康氢阑 固磨 荐 乐绰 备悼 券版 棺 汲沥甸阑 钎矫.
	{
		Scalability::FQualityLevels CurrScalabilityLevel = Scalability::GetQualityLevels();
		int32 TotalAdded =
			CurrScalabilityLevel.AntiAliasingQuality +
			CurrScalabilityLevel.EffectsQuality +
			CurrScalabilityLevel.PostProcessQuality +
			//InScalabilityLevel.ResolutionQuality + 捞扒 荐摹啊 粱 促福聪 哗具..
			CurrScalabilityLevel.TextureQuality +
			CurrScalabilityLevel.ShadowQuality +
			CurrScalabilityLevel.ViewDistanceQuality +
			CurrScalabilityLevel.GameplayOnlyLODQuality;
		// 趣咯唱 眠啊瞪 Scalability 亲格俊 措茄 贸府啊 鞘夸窍变 茄单 弊犯霸 沥背窍霸鳖瘤 且 鞘夸绰 绝阑 淀.
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
	{ // 眠啊肺 窍困 霸烙葛靛 己拜俊 蝶扼 钎矫且 芭 乐澜.
		InB2GM->DrawAdditionalPerfStatInfo(DrawX, DrawY + (float)(FontSize + LineDist));
	}
#endif
}
#endif

#ifdef BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
// 肺爹 矫埃 螟沥 侩. 
// 肺爹 矫埃捞具 弊成 焊搁 唱坷绰 芭 酒聪衬绊 且 荐 乐瘤父 捞繁历繁 扁瓷 抛胶飘秦 啊搁辑 焊妨搁 1~2 檬 窜绵登绰 芭 眉农且 鞘夸啊 乐绢辑 持澜.
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
		// 霸烙 内靛 惑栏肺绰 利例茄 困摹俊辑 肺爹 矫埃 眉农甫 茄促绊 秦档 坊歹静饭靛客狼 鸥捞怪栏肺 牢秦 角力 眉皑 肺爹矫埃篮 促甫 荐 乐促.
		//FlushRenderingCommands(); // 辟单 捞犯霸 秦档 家侩捞 绝窜 扒 窃沥. 搬惫 捞扒 曼绊侩栏肺父 静绊 角力 肺爹 矫埃 螟沥篮 流立 胶砰况摹肺...
		gLoadTimeCheckBeginMark = false;
		double LoadingTimeTaken = FPlatformTime::Seconds() - gLastLoadStartTime;
		UE_LOG(LogBladeII, Log, TEXT("[DevCheckLoadingTime] LastLoadingTime %f"), LoadingTimeTaken);

		// 捞吧 坊歹傅 且 荐 乐绰 矫痢捞妨搁 UIManager 啊 檬扁拳 等 惑怕咯具 窃.
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
//	//{// 沥侥 霸烙俊辑 捞 蔼捞 贸澜俊 true 咯具 秦辑 扁夯蔼 true 肺 技泼窍促 焊聪 DLCFrontMap 阑 芭摹瘤 臼绰 俺惯侩 抛胶飘甫 困秦 咯扁 甸绢坷搁 傲 官层淋.
//	//	GMinimalDLCFrontMode = false; // 弊府绊 ini 俊 捞吧 true 肺 秦辑 矫累沁促 窍歹扼档 false 肺 官差绢辑 俊叼磐 鞍篮 版快 盔贰 狼档客 促弗 悼累阑 且 荐 乐阑 抛聪 炼缴炼缴
//	//}
//	//	
//	//if (!GMinimalDLCFrontMode){ // DLC Front 葛靛 府家胶肺爹 弥措茄 力芭
//	//	FString PreLoadingScreenWidgetClassPath;
//	//	GConfig->GetString(TEXT("/Script/BladeII"), TEXT("PreLoadingScreenWidgetClass"), PreLoadingScreenWidgetClassPath, GGameIni);
//	//	static ConstructorHelpers::FClassFinder<UB2PreLoadingScreen> PreLoadingScreenClassFoundSet(*PreLoadingScreenWidgetClassPath);
//	//	PreLoadingScreenWidgetClass = PreLoadingScreenClassFoundSet.Class;
//	//	// 扁夯蔼 笛 促 度鞍澜..
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
	//	// 老馆利牢 Local角青 葛靛 (Stage, 康旷狼 啪, 馆拜带傈, 评措傈)篮 Attacker啊 Local老 锭 扁废

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
	// 窍困 努贰胶, 漂洒 StageGameMode 俊辑 鞘夸茄 局甸父 持绢辑 府畔窍档废 窍扁 困窃..
	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}
	return RetArray;
}
TArray<EPCClass> ABladeIIGameMode::GetPCClassesToMatineePreLoad()
{
	return GetPCClassesToPreLoad(); // 老何 霸烙葛靛俊辑 捞霸 酒囱 版快啊 乐阑 巴.
}
TArray<FCombinedPCSkillAnimID> ABladeIIGameMode::GetPCSkillAnimsToPreLoad()
{
	TArray<FCombinedPCSkillAnimID> RetArray;

	//TArray<EPCClass> AllPCClassToPreLoad = GetPCClassesToPreLoad();

	//for (EPCClass ThisPCClass : AllPCClassToPreLoad)
	//{
	//	// 扁夯 备泅篮 概 PCClass 喊肺 啊瓷茄 胶懦 局聪皋捞记甸阑 根顶 促 持澜. 
	//	// 窍困 GameMode 努贰胶俊辑绰 阿 敲饭捞绢 某腐磐 喊肺 厘馒茄 Skill 惑怕俊 蝶扼 鞘夸茄 巴父 持阑 荐 乐档废 秦具 窃.
	//	// 捞甫 困秦辑绰 CharacterDataStore 啊 鞘夸窍骨肺 匙飘况农 霸烙捞扼搁 某腐磐 喊肺 iteration 阑 倒酒辑 厘馒茄 skill 甸阑 葛酒客具瘤, PCClass 肺 iteration 窍搁 救瞪 巴烙.
	//	// 弊矾唱 老何 匙飘况农 扁馆 葛靛俊辑绰 角力 概莫捞 登扁 傈俊绰 CharacterDataStore 啊 绝阑 巴捞骨肺 Preload 矫痢俊 阂啊瓷且 荐档 乐促.

	//	TArray<FCombinedPCSkillAnimID> AllAnimIDOfThisClass = GetAllPCSkillAnimsOfClass(ThisPCClass);
	//	RetArray.Append(AllAnimIDOfThisClass);
	//}
	return RetArray;
}

TArray<FCombinedPCSkillAnimID> ABladeIIGameMode::GetAllPCSkillAnimsOfClass(EPCClass InCharClass)
{ // 蜡瓶 荐霖. 怖 咯扁俊 乐阑 鞘夸绰 绝促.
	TArray<FCombinedPCSkillAnimID> RetArray;
	for (int32 SAI = 0; SAI < static_cast<int32>(ESkillAnimType::ESA_End); ++SAI)
	{
		ESkillAnimType ThisAnimType = static_cast<ESkillAnimType>(SAI);
		if (ThisAnimType != ESkillAnimType::ESA_Weapon_Normal_End) // 捞扒 吝埃俊 构 侥喊茄翠矫绊 持篮 芭聪 哗绊
		{
			RetArray.Add(FCombinedPCSkillAnimID(InCharClass, ThisAnimType));
		}
	}
	return RetArray;
}

// 扁夯利栏肺 Main / Sub 滴俺甫 肺靛窍绊 GameMode喊肺 包府茄促. ex ) 评措傈 - 悸, 饭捞靛, 痢飞傈 - 窍唱
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

 //GatherEquippedSkillAnimIDs 窃荐父 犁沥狼 窍咯 霸烙 葛靛喊 鞘夸 Class SkillAnim甸阑 AsyncLoad
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

	//// TAsset 肺爹窍绰 InfoAsset 甸捞 措惑烙. 馆措绰 ManualUnloadBeforeNewBeginning
	//
	//UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	//if (PCBox)
	//{
	//	B2_SCOPED_TIME_LOG(TEXT("PreloadAnyNecessaryInfo > PCClassInfo"));
	//	// 捞扒 喊肺 矫埃 救 冈阑 巴捞绊 PCClassInfo 吝俊辑 蝶肺 都绢辰 InGameOnlyInfo 啊 腹捞 冈阑 波. 弊扒 Async 肺爹 措惑捞扼 捞芭 捞饶俊 窃.
	//	TArray<EPCClass> ClassesToLoad = GetPCClassesToPreLoad();
	//	PCBox->PreloadClassAssets(ClassesToLoad);
	//}
	//
	//{
	//	// SpawnPool 俊辑 spawn 且 荐 乐绰 葛电 NPC class 甸 preload 窍绰 扒 酒聪绊 咯扁急 霸烙 矫累 捞饶 Async 措惑 力寇窍绊.
	//	B2_SCOPED_TIME_LOG(TEXT("PreloadAnyNecessaryInfo > MobClassInfo"));
	//	if (TheActiveSpawnPool)
	//		TheActiveSpawnPool->PreloadPoolMonsters();
	//}

	//B2GMLoadingProgCollector::StepCustom(bAboutToPlayLoadingMovie ? 0.4f : 0.1f);
}

void ABladeIIGameMode::PreloadInGameInfoAsset_SynchronousOnly(bool bAboutToPlayLoadingMovie)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::PreloadInGameInfoAsset_SynchronousOnly"));

	// 夸 Synchronous 肺爹捞 FlushAsyncLoading 阑 蜡惯茄促搁 促弗 async 夸没 捞傈俊 秦具 窍绊,
	// 弊霸 酒聪扼搁 捞吧 坷洒妨 促弗 async 夸没 捞饶俊 窃栏肺辑 促吝 静饭靛肺 肺爹捞 倒酒啊档废 且 荐 乐.. 阑鳖?
		
	// TAsset 栏肺 官槽 Matinee 饭欺繁胶 俊悸 肺爹.. 捞扒 盔贰 甘 肺爹狼 老何看带 巴捞促. 啊瓷茄 贸澜俊 困摹矫糯.
	{
		int32 LazyLoadHandledNum = B2GamePreloadMatineeRefAssets();
		B2GMLoadingProgCollector::StepCustom((float)LazyLoadHandledNum * (bAboutToPlayLoadingMovie ? 0.02f : 0.01f));
	}

	{ // DamageEffectInfo 客 BuffModeEffectInfo 肺爹俊 狼茄 柳青档 魂沥矫 角力 肺爹 鞘夸啊 乐绰瘤俊 蝶扼 促福霸..
		if (!UB2DamageEffectInfo::AssetsAreRooted() && !UB2BuffModeEffectInfo::AssetsAreRooted())
		{ // 笛 促 肺爹窍霸 登绰 版快
			B2GMLoadingProgCollector::SetStepRate(bAboutToPlayLoadingMovie ? 0.4f : 0.1f);
		}
		else if (UB2DamageEffectInfo::AssetsAreRooted() && UB2BuffModeEffectInfo::AssetsAreRooted())
		{
			B2GMLoadingProgCollector::SetStepRate(0.0f);
		}
		else
		{ // 笛 吝 窍唱父 肺爹窍霸 登绰 版快
			B2GMLoadingProgCollector::SetStepRate(bAboutToPlayLoadingMovie ? 0.2f : 0.07f);
		}
	}
	UB2DamageEffectInfo* DFXInfo = StaticFindDamageEffectInfo(this);
	if (DFXInfo)
	{ // 傈捧俊辑 磊肋茄 spike 夸牢捞 瞪 荐 乐澜.
		DFXInfo->PreloadAndCacheAllTAsset();
	}
	UB2BuffModeEffectInfo* BuffInfo = StaticFindBuffModeEffectInfo(this);
	if (BuffInfo)
	{
		BuffInfo->PreloadAndCacheAllTAsset();
	}
	B2GMLoadingProgCollector::Step(); // 菊辑 魂沥茄 StepRate 肺 柳青档 刘啊.

	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (PCBox)
	{ // 皋牢 某腐磐 单捞磐绰 sync 肺爹栏肺 贸府窃. RootSet 捞搁 弊成 逞绢啊霸 瞪 波绊.
		UB2PCClassInfo* ThisLoadedInfo = PCBox->GetSingleClassInfo(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass());
		if (ThisLoadedInfo)
		{
			// 风飘悸 贸府 咯何客 包拌绝捞 捞 矫痢俊辑 风飘悸捞 酒聪搁 肺爹捞 救登绢 乐绢具 窃.
			// 弥厩狼 版快 促弗 PCClass InGameOnly 单捞磐档 促 肺爹窍绰 版快档 乐摆瘤父.. 咯扁急 老窜 犬角茄 镑捞聪 腹篮 Step 瘤盒 硅寸.
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

	// 肺厚甫 力寇茄 霸烙葛靛甸俊辑绰 角龙利栏肺 皋葛府客 肺爹鸥烙阑 家葛窍绰 InGameOnlyInfo 甫 肺爹秦 初阑 鞘夸啊 乐促.
	// 霸烙葛靛 喊 鞘夸 棺 备炼俊 蝶扼 啊瓷茄 何盒篮 厚悼扁 肺爹栏肺 贸府甫 窍档废 茄促.
	// 咯扁辑 官肺 肺爹阑 窍带 Async 夸没父 窍带 奴 耽绢府俊 措茄 肺爹捞扼 泵必利栏肺 腹篮 皋葛府啊 棵扼皑.
	// 窜, RootSet 贸府甫 茄 芭扼搁 捞固 皋葛府俊 棵扼客 乐阑 抛聪 酒公 函拳 绝摆瘤.

	// 咯扁辑 StaticFindPCClassInfoBox 甫 妮窍绰 巴捞 Async flush 甫 老栏挪促搁 窜鉴洒 捞傈俊 茄锅 妮秦 初栏搁 等促.
	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (PCBox)
	{
		TArray<EPCClass> ClassesToLoad = GetPCClassesToPreLoad();

#if !UE_BUILD_SHIPPING // PC 努贰胶 肺爹狼 康氢 惑技 颇厩
		extern int32 gPCDataPreLoadMaxNum;
		if (gPCDataPreLoadMaxNum >= 0)
		{
			if (gPCDataPreLoadMaxNum == 0)
			{
				ClassesToLoad.Empty();
			}
			else if (gPCDataPreLoadMaxNum == 1) // 窍唱父捞搁 贸澜 spawn 且 局父
			{
				ClassesToLoad.Empty();
				ClassesToLoad.Add(CurrentPCClassEnum);
			}
			else
			{
				if (ClassesToLoad.Num() > gPCDataPreLoadMaxNum)
				{
					ClassesToLoad.RemoveAt(gPCDataPreLoadMaxNum, ClassesToLoad.Num() - gPCDataPreLoadMaxNum);
					if (!ClassesToLoad.Find(CurrentPCClassEnum)) { // 咯窍埃 贸澜 spawn 且 努贰胶绰 pre-load 甫 秦具 抛胶飘啊 沥犬秦 龙 巴.
						ClassesToLoad.RemoveAt(ClassesToLoad.Num() - 1);
						ClassesToLoad.Add(CurrentPCClassEnum);
					}
				}
			}
		}
#endif

		for (EPCClass ThisClassLoad : ClassesToLoad)
		{
			// 皋牢 某腐磐 努贰胶绰 风飘悸 趣篮 肺爹捞 登歹扼档 PreloadInGameInfoAsset_SynchronousOnly 俊辑 肺爹等 惑怕.. 
			// 芭唱 酒聪搁 捞 夸没 流饶俊 Synchronous 肺爹阑 秦辑 促吝栏肺 倒酒啊绰 肺爹狼 捞垫阑 掘阑 荐 乐栏搁 亮摆瘤.
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
	// 狼档利栏肺 厚况咙.

	// 咯扁辑 盔贰 窍绰 GameMode 率 扁夯 扁瓷篮 力芭窍绊
	// 鞘夸茄 沥焊甸捞 犬角洒 盲况柳 饶俊 StageEventDirector 窜困肺 鞘夸茄 巴甸阑 吧矾郴搁辑 
	// AB2StageEventDirector::LoadLazyLoadedMatineeRefAssets 甫 荤侩秦辑 pre-load 窃. (B2GamePreloadMatineeRefAssets)
}
int32 ABladeIIGameMode::B2GamePreloadMatineeRefAssets()
{
	int32 LazyLoadedSEDNum = 0;
//#if _MATINEE_REF_ASSET_LAZYLOAD // 霸烙 内靛 率俊急 咯扁俊父 困摹矫虐搁 包访 扁瓷 牧飘费捞 啊瓷.
//	TArray<FString> StageEventRefLoadExcludeFilters;
//
//#if 0 // 老何 飘发阑 力寇矫难档 唱吝俊 楷免 檬扁拳登搁辑 搬惫 肺爹. 弊府绊 捞吧肺 力寇登绰 剧篮 弊府 腹瘤 臼篮 淀. 老窜 咯变 焊幅.
//	TArray<EPCClass> PCClassesToLoad = GetPCClassesToMatineePreLoad();
//	if (PCClassesToLoad.Num() > 0)
//	{
//		// Preload 措惑 PCClass 甫 Exclude 俊辑 力寇
//		// 府家胶 悸诀俊档 惑寸洒 狼粮窍绰 芭扼 捞霸 100% 沥犬窍瘤 臼阑 荐档 乐绰单
//		// 弊繁 侥栏肺 狼档摹 臼霸 吧矾廉辑 咯扁辑 肺爹捞 救等促绊 秦档 奴老 唱绰 扒 酒聪促. 鞘夸且 订 绢瞒乔 肺爹捞 等促. 
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
//	// 霸烙葛靛狼 饭骇 备己俊 蝶扼辑 捞吧 妮窍绰 扁夯 矫痢俊 EventDirectors 硅凯捞 救 盲况廉 乐阑 荐档 乐促.
//	// 弊繁 版快 弊成 出抵档 扁瓷 磊眉绰 捞惑捞 绝阑 巴捞唱 父老 Matinee 曼炼 俊悸捞 肺爹登搁辑 传俊 剁绰 谗辫阑 蜡惯窍芭唱 async flush 甫 老栏挪促搁
//	// 捞吧 眠啊肺 妮秦具 且 荐档.
//	for (AB2StageEventDirector* CurrSED : EventDirectors)
//	{
//		if (CurrSED && CurrSED->IsExpectedForGameMode(this))
//		{
//			CurrSED->LoadLazyLoadedMatineeRefAssets(StageEventRefLoadExcludeFilters);
//			++LazyLoadedSEDNum;
//		}
//	}
//#endif
	return LazyLoadedSEDNum; // Lazy-loading 贸府啊 等 StageEventDirector (芭狼 Matinee) 俺荐甫 府畔.
}

void ABladeIIGameMode::GameContextAsyncFlush()
{ // 肺爹 鸥烙俊 Preload 茄 芭 捞寇俊 霸烙 敲饭捞 登搁辑 Async 肺爹 且 巴甸捞 flush 瞪 荐 乐促芭唱 
	// 郴瘤绰 官恩流窍瘤 臼篮 惑炔捞 乐促搁 捞吧 荤侩秦辑 弊繁 巴甸阑 固楷俊 贸府窍扁 困秦 力傍
	// 捞扒 肺爹 鸥烙俊 阂妨具 茄促.
	if (TheActiveSpawnPool)
	{
		TheActiveSpawnPool->TryAsyncLoadReservedMonsters();
		FlushAsyncLoading();
	}
}

void ABladeIIGameMode::PrebuildAndCacheCharMesh(EPCClass InCharClass, const TArray<FB2Item>& InEquippedItems, const FB2Wing* InWingData, bool bMergeSections, int32 InOptionalCharEntryID /*= -1*/)
{
	// Preload 矫痢俊 怕弊 某腐磐 mesh merge 甫 固府 秦 初扁 困茄 格利牢单.. 
	// 肺拿 某腐磐 mesh 狼 RootSet 贸府甫 窍绰 B2CompositeMeshCache 狼 档涝 捞饶肺绰 老馆 霸烙葛靛俊辑绰 荤角惑 鞘夸 绝绰 扁瓷捞促.
	// 厘厚 厘馒捞 促弗 吧肺 碍力登绰 譬配府倔 葛靛俊辑绰 鞘夸窍绊.. 
	// 郴瘤绰 PVP 狼 惑措规 怕弊 某腐磐甫 固府 merge 秦 初霸 登搁 芭扁辑 鞘夸且 荐档 乐变 茄单.. 弊凡妨搁 粱 荐沥捞 鞘夸.

	UWorld* TheWorld = GetWorld();
	UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox(this);
	UB2PCClassInfo* ThisCharInfo = PCInfoBox ? PCInfoBox->GetSingleClassInfo(InCharClass) : nullptr;
	if (TheWorld && ThisCharInfo)
	{
		// 老矫利栏肺 owner 开且阑 且 dummy 甫 spawn 矫挪促.
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.ObjectFlags |= RF_Transient;
		FTransform SpawnTransform(FVector(0.0f, 0.0f, 0.0f));
		ASkeletalMeshActor* DummyOwnerSKActor = TheWorld->SpawnActor<ASkeletalMeshActor>(ASkeletalMeshActor::StaticClass(), SpawnTransform, SpawnInfo);
		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
		if (DummyOwnerSKActor && MeshCacheManager)
		{
			// LocalPlayerChar 绰 NULL 捞绢档 巩力 绝澜. 咯窍埃 CompositeMeshCache 率辑 Local 侩栏肺 贸府且 巴.
			ABladeIIPlayer* LocalPlayerChar = Cast<ABladeIIPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
			// 狼档茄 捞 窃荐狼 荤侩俊辑 BuiltResult 绰 NULL 老 巴捞促. B2CompositeMeshCache 俊辑 肺拿 某腐磐 侩 皋浆甫 RootSet 贸府甫 窍绰 老馆 胶抛捞瘤 葛靛俊急 咯扁肺 客急 救凳.
			// 捞霸 NULL 捞 酒匆 窍唱狼 啊瓷己篮 抛胶飘 吝俊 譬配府倔 霸烙葛靛俊辑 静绰 巴苞 度鞍篮 厘厚甫 涝囚初绊 譬配府倔 霸烙葛靛肺 甸绢啊绰 芭.
			USkeletalMesh* BuiltResult = MeshCacheManager->GetCachedCompositeMesh(InCharClass, LocalPlayerChar, InEquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, bMergeSections);

			if (UB2PCClassInfo::SetupSKCompForParts(InCharClass, DummyOwnerSKActor, DummyOwnerSKActor->GetSkeletalMeshComponent(), BuiltResult, ThisCharInfo->BaseMeshAsset, ThisCharInfo->DefaultPartsInfo,
				InEquippedItems, InWingData, 
				nullptr, // 皋浆父 呼靛窍绰 霸 格利捞骨肺 AnimBP 鞍篮 扒 傈崔且 鞘夸 绝瘤
				false, // 酒流篮 呼靛茄 吧 某教窍绰 格利阑 混府妨搁 盔夯 颇飘甸档 焊粮秦具 秦辑..
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
	// 肺拿 某腐磐狼 怕弊啊 倾侩登绰 葛靛俊辑 鞘夸茄 扁瓷. 溜 葛氰葛靛, 康旷狼啪, PVP
	// PVP 狼 版快绰 肺拿 某腐磐 挥 酒聪扼 府葛飘 某腐磐档 怕弊甫 窍骨肺 弊巴鳖瘤 绊妨窍妨搁 犬厘等 扁瓷捞 鞘夸窍促.

	UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
	if (MeshCacheManager && MeshCacheManager->IsLocalPCMeshRootSetAllowed()) {
		// 肺拿 某腐磐 皋浆狼 RootSet 贸府 捞饶俊绰 捞扒 鞘夸 绝阑 巴烙. 
		// 抛胶飘甫 困秦 寸盒埃 巢败狄.
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
		false // 老窜 扁夯利栏肺 冀记 唱床柳 老馆 葛胆肺.
	);

	// 怕弊 某腐磐甫 荤侩窍绰 葛靛扼搁 冀记 捍钦等 葛胆阑 荤侩且 啊瓷己阑 积阿窍扁绰 绢菲瘤父.. 内靛绰 爱苗滴绊.
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
//#if !UE_BUILD_SHIPPING // PreRender 悸诀窍搁辑 肺爹窍霸 瞪 荐档 乐栏聪 抛胶飘窍妨搁 馋促 せ
//	extern int32 gPCDataPreLoadMaxNum;
//#endif
//
//	return (UB2PreRenderer::MaxPreRenderCount > 0 // 厚背 抛胶飘甫 困秦 0 捞搁 积己阑 窍瘤 臼绰促. 敲阀汽捞唱 扁鸥 炼扒俊 蝶扼 荤侩阑 救秦具 且 荐档 乐绊..
//#if WITH_EDITOR
//		&& !GIsEditor // 俊叼磐 敲饭捞俊辑 啊阐 焊扁 救亮篮 坊歹傅 巩力甫 老栏虐扁档..
//#endif
//#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 绰 皋葛府 家葛啊 歹 奴 巩力扼 帮鄂窍扁档 窍绊 嘉捞歹 某教阑 绢痘霸 窍绰瘤 隔扼档 救靛肺捞靛焊促 代 滚棍烙.
//		&& UB2PreRenderer::ShouldDoPreRenderForIOS() // SetupPreRenderObjects 档吝 30 ~ 50MB 沥档 皋葛府啊 棵扼埃促. 弊府绊 GPU 皋葛府俊 惑寸洒 困蛆捞 登绰 葛剧. 喊档狼 PreRenderGameMode 俊辑绰 秦 杭 荐档 乐促.
//#endif
//#if !UE_BUILD_SHIPPING // PreRender 悸诀窍搁辑 肺爹窍霸 瞪 荐档 乐栏聪 抛胶飘窍妨搁 馋促 せ
//		&& (gPCDataPreLoadMaxNum < 0 || gPCDataPreLoadMaxNum >= PCClassToInt(EPCClass::EPC_End))
//#endif
//		);
	return false;
}
////
void ABladeIIGameMode::SetupPreRenderObjects() 
{
	// 嘉捞歹 哪颇老阑 固府 秦 初瘤 臼绰 葛官老 敲阀汽俊辑狼 嘉捞歹 哪颇老阑 困茄 扁瓷. 咯扁辑 初摹绰 霸 乐促绊 构啊 累悼 救窍绰 扒 酒聪绊 傲 吝埃俊 鞘夸秦 龙 锭 滚棍芭覆.

	//check(!PreRenderer);
	//PreRenderer = NewObject<UB2PreRenderer>(this, NAME_None, RF_Transient);
	//check(PreRenderer);
	//if (PreRenderer)
	//{
	//	if (PreRenderer->SetupPreRenderObjects(this))
	//	{
	//		B2GMLoadingProgCollector::StepCustom(0.2f); // Pre-render 悸诀阑 沁促搁 粱 冈菌阑 淀.

	//		// 啊赴 悼救 霸烙捞唱 漂洒 贸澜 楷免捞 芭狼 柳青登瘤 臼档废 概快 累篮 TimeDilation 阑 霖促.
	//		SetWholeGameTimeScale(0.0001f); // 促弗 镑俊辑狼 TimeDilation 苞 爵洒瘤 臼档废 老矫利栏肺 WholeGameTimeScale 阑 荤侩.

	//		PreRenderPhase = EPreRenderPhase::EPRP_Selected; // PreRender 柳涝 付农. 父俊 窍唱 积己等 Dummy 啊 绝歹扼档 老窜 Selected 肺. All 肺 柳青篮 捞饶 贸府瞪 巴.
	//	}
	//	else
	//	{
	//		// SetupPreRenderObjects 角菩扼扁焊促绰.. 捞固 Pre-render 甫 芭模 捞饶搁 捞犯霸 登绰 扒单..
	//		// 概锅 OnPreRenderComplete 扼聪 粱 扯厚胶反焙.
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
	//	if (PreRenderer && !PreRenderer->HasPreRenderDummyToProcess()) // 扁夯 Selected 窜拌 肯丰.
	//	{
	//		if (UB2PreRenderer::bPreRenderAllPrimitives) 
	//		{// 眠啊肺 傈眉 primitive 甸 措惑栏肺..
	//			if (PreRenderer->SetupPreRenderAllPrimPhase()) {
	//				PreRenderPhase = EPreRenderPhase::EPRP_AllPrim;
	//			}
	//			else {
	//				// 捞固 茄锅 PreRender 甫 芭模 饭骇捞芭唱 扁鸥 荤蜡肺 漂喊洒 AllPrim 贸府甫 救窍档废 茄 镑篮 角菩且 巴. 辆丰 贸府
	//				OnPreRenderComplete(); 
	//			}
	//		}
	//		else 
	//		{ // Pre-render 辆丰
	//			OnPreRenderComplete(); 
	//		}
	//	}
	//}
	//else if (PreRenderPhase == EPreRenderPhase::EPRP_AllPrim)
	//{
	//	if (PreRenderer && PreRenderer->CheckAndFinishPreRenderAllPrimPhase())
	//	{ // Pre-render 傈眉 轿捞努 促 场郴绊 辆丰
	//		OnPreRenderComplete();
	//	}
	//}
}

void ABladeIIGameMode::OnPreRenderComplete()
{
	check(PreRenderer);
	if (PreRenderer) {
		// AllPrim 咯何俊 惑包绝捞 泅犁 岿靛啊 扁馆窍绊 乐绰 饭骇俊辑绰 Pre-render 啊 茄锅 肯丰等 巴栏肺 扁废.
		PreRenderer->MarkCurrentWorldLevelPreRendered();
		PreRenderer->OnPreRenderComplete();
		PreRenderer->ConditionalBeginDestroy(); // 购啊 弧府 力芭 付农扼档 秦 林绢具 救缴捞 瞪 淀茄 扁盒
	}
	PreRenderer = nullptr;
	
	PreRenderPhase = EPreRenderPhase::EPRP_Done;

	// Pre-render 吧赴 惑怕俊辑 矫累沁带 楷免俊辑 老何 厚劝己拳 矫淖带 芭 乐栏搁 汗备.
	for (AB2StageEventDirector* ThisSED : EventDirectors)
	{
		if (ThisSED && ThisSED->IsPlayingShow())
		{
			ThisSED->RestoreTracksAfterPreRender();
		}
	}

	ClosePreLoadingScreen(EPreLoadingScreenType::PLST_PreRender_Default);
	RestoreWholeGameTimeScale(); // 拳搁 啊赴 悼救 撤描带 WholeGameTimeScale 汗备.
	EnsureStageEventTimeDilation(); // EventDirector 率档 TimeDilation 救 林菌阑 抛聪 捞 锭 鞍捞 汗备甫.

	// Pre-render 甫 茄促搁 蝶肺 鸥捞赣 汲摹 救沁阑 波绊 捞 矫痢俊辑 肺爹 捞固瘤 攫肺靛. Pre-render 矫 喉废 胶农赴俊 静霸 瞪 荐档 乐绊.
	UnloadAndClearLoadingImageState();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	// Pre-render 甫 沁促搁 捞 矫痢俊辑 肺爹 矫埃 螟沥 辆丰
	DevCheckLoadingTimeEnd();
#endif
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
int32 gDetectedGarbageExcessiveRenderCount = 0;
#endif
void ABladeIIGameMode::OnPreLoadMap(const FString& InString)
{
//	//
//	// 捞抚篮 PreLoadMap 捞扼 捞锅 霸烙葛靛 矫累 流傈栏肺 坷秦且 荐 乐瘤父 角力肺绰 磊扁 磊脚篮 郴妨哎 霖厚甫 窍绰 何盒.
//	// 概 甘 傈券 流傈 酒流 World GC 登扁 捞傈 公攫啊 沥府窍扁 亮篮 胶铺烙.
//	// 阿 UObject 甸狼 BeginDestroy 俊辑绰 窍搁 肋 救登绰 巴甸捞 咯扁俊辑绰 咯傈洒 啊瓷茄 版快啊 腹澜.
//	//
//
//	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::OnPreLoadMap"));
//
//#if !UE_BUILD_SHIPPING || BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	// ExcessiveRenderProcessing 篮 Pre-render 档吝俊父 荤侩窍绰 漂喊茄 扁瓷栏肺, 磊末 老馆利牢 惑炔俊鳖瘤 汝矾唱吭促啊绰 己瓷俊 奴 零距捞 等促.
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
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 奖俏 抛胶飘 呼靛俊辑档 钎矫..
//		gDetectedGarbageExcessiveRenderCount = ExcessiveRenderModeCount;
//#endif
//		checkf(ExcessiveRenderModeCount == 0, TEXT("Detected %d PrimitiveComponents in ExcessiveRenderProcessing at last level transition!"), ExcessiveRenderModeCount);
//	}
//#endif
//
//	// 扁夯蔼 瞒盔.. 霸烙葛靛 率 函荐(UseCustomWaitForLoadingScreen)绰 咯扁辑 技泼秦毫具 家侩捞 绝摆瘤父 MoviePlayer 率 函荐绰 府悸捞 鞘夸秦辑.
//	SetUseCustomWaitForLoadingScreen(false);
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	// 肺爹 矫埃 螟沥.
//	DevCheckLoadingTimeBegin();
//#endif
//
//	UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//	if (MeshCacheManager)
//	{
//		MeshCacheManager->OnPreLoadMap(this);
//	}
//	
//	UnloadAndClearLoadingImageState(); // 愧父秦辑 捞 矫痢俊 促 郴妨啊 乐变 秦具 窍绰单 鸥烙 掉饭捞甫 林绰 捞惑 葛福绰 老捞聪. PreLoadingScreen 凯菌促啊 捞吧肺 Unload 窍绊 促矫 LoadingScreen 锭巩俊 肺爹窍绰 扒 巩力绝促.
//	ClosePreLoadingScreenAll(); // 抗凯(?) 己拜栏肺 UI 肺 力累等 PreLoadingScreen 阑 剁快绰 霸 乐绢辑 弊吧 摧澜.
//	SetupNextGameLoadingScreen();
//	
//	LobbyEndOnTouchImpulseRingClass<>::GetInstance().Signal(); // 贸府甫 救秦林绰 版快 父俊 窍唱 巢酒 乐阑 荐 乐促. 肺厚葛靛 酒聪搁 捞吧 贸府窍瘤 臼栏骨肺 犬角洒 场郴淋.
//	
//	// 扁夯 内靛俊辑绰 饭骇 辆丰矫 阂府瘤 臼绰 淀茄单, VirtualJoystick 篮 概锅 货肺 积己. 抗傈俊 购啊 滚弊啊 乐绢辑 持绢滴菌菌绰单 捞力 宝满阑瘤档..?
//	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(this));
//	if (B2PC)
//	{
//		B2PC->DeactivateVitualJoystick();
//	}
//
//	// 搬惫 概 岿靛 郴妨哎 锭付促 鞍捞 绝局林绰 吧肺. UIManager 绰 府家胶 坷宏璃飘扼 角龙利牢 GC 绰 救登骨肺 郴何 骂波扁甸阑 流立 力芭秦具 茄促.
//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//	if (UIMgrInst)
//	{
//		UIMgrInst->MasterDestroy(); // 弊成 Destroy 富绊 MasterDestroy 肺 秦具 窃.
//	}
//	if (UIManager_InGameCombat) // DJLegacy 狼 傈捧纠 UIManager 
//	{
//		UIManager_InGameCombat->OnPreLoadMap();
//	}
//
//	StopBGM();
//
//	// Async Load request 鞍篮 芭 葛滴 肯丰 棺 努府绢.
//	// 官肺 酒贰 ManualUnloadBeforeNewBeginning 苞 鞍篮 钙遏烙.
//	if (UB2AssetLoader* AssetLoader = UB2AssetLoader::Get()){
//		AssetLoader->FlushAll();
//	}
//	ManualUnloadBeforeNewBeginning(this); // InfoAsset 甸 老何 抗寇 力寇窍绊 葛滴 较 努府绢
//
//	SaveLocalIniStateOnPreLoadMap(); 
//	
//	ExtraOnPreLoadMap();
//
//	IStreamingManager::Get().StreamAllResources(5.0f);
}

/** OnPreLoadMap 矫痢狼 捞力 肺爹捞 矫累登绰 霸烙葛靛甫 困茄 肺爹 胶农赴悸诀. */
void ABladeIIGameMode::SetupNextGameLoadingScreen()
{
	//IB2GameLoadingScreenModule* const LoadingScreenModule = GetB2LoadingScreenModulePtr();
	//if (LoadingScreenModule != nullptr)
	//{
	//	const FString ClosingMovieName = GetClosingMovieName();
	//	if (ClosingMovieName.Len() > 0) 
	//	{
	//		// 漂喊洒 辆丰 矫 敲饭捞且 悼康惑阑 瘤沥窍绰 版快.. Subtitle 捞唱 扁鸥 牢磊绰 鞘夸俊 蝶扼 备泅 眠啊登绢具 窃.
	//		// 老馆利牢 矫唱府坷 悼康惑捞 酒丛. 弊扒 促澜 肺爹等 霸烙葛靛狼 StartPlay 俊辑 悸诀
	//		LoadingScreenModule->StartGameLoadingScreen_Movie(*ClosingMovieName,
	//			// 弊成 FitToMin 栏肺 窍搁 促剧茄 悼康惑 厚啦俊 措茄 措贸啊 啊瓷窍唱 AppleMovieStreamer 备炼客 包访秦辑 IOS 老何 扁扁俊辑 老矫利栏肺 拳搁 厚啦捞 儒飘矾瘤绰 巩力啊 惯积且 荐 乐促.
	//			// DLC 鸥捞撇俊辑 肺厚 肺爹阑 力寇茄 扁鸥 肺爹 悼康惑捞 16:9 肺 力累登绰 茄 咯扁绰 StdHD 肺 绊沥.
	//			ELoadingScreenMovieAlignMode::FitToMin_StdHD,
	//			true	// 譬配府倔 Skip 眠啊
	//		);
	//	}
	//	else 
	//	{ // 老馆利牢 捞固瘤 肺爹 胶农赴
	//		SetupLoadingScreenImageCommon(false, true, false);
	//	}
	//}
}
void ABladeIIGameMode::SetupLoadingScreenImageCommon(
	bool bSetImagePersistent, // 荤侩茄 肺爹 捞固瘤甫 (泅 霸烙葛靛 档吝) 促澜锅俊档 荤侩窍妨搁 true 肺.
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
			InOptionalImageName ? InOptionalImageName : *GetLoadingImageName(bSetImagePersistent), // 老馆利牢 版快, InOptionalImageName 阑 持瘤 臼绊 GameMode 磊眉利栏肺 沥秦柳 风凭俊 蝶弗 捞固瘤甫 荤侩.
			bInDisplayProgress, bInDisplayIndicator,
			FVector2D((float)ViewSizeX, (float)ViewSizeY),
			false, OptimalAspectRatio, // 漂沥 拳搁厚啦阑 弥利栏肺 茄 吝居沥纺 荤侩.
			InLoadingMessageType
		);
	}
}

#if WITH_EDITOR
void ABladeIIGameMode::OnEditorEndPlayMap()
{
	// 俊叼磐 敲饭捞 吝窜 矫俊档 累悼沁栏搁 窍绰 OnPreLoadMap 悼累甸捞 乐栏搁 咯扁俊.. UIManager Destroy 绰 磊眉利栏肺 角青凳.
		
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
		// 扁粮俊 公累困肺 沥秦初篮 蔼捞 绝栏搁 货肺 惶酒 静绊
		// bPersistent 牢 版快 公累困肺 沥秦柳 蔼阑 历厘秦辑 促澜俊 度鞍捞 荤侩且 荐 乐档废 窃.

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

		// 捞扒 促弗 惑炔俊辑 肺爹 捞固瘤甫 楷厘秦辑 静扁 困秦..
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
	//	if (!IsInPreRenderPhase() && GetMoviePlayer()->IsLoadingFinished()) // 荤角 WaitForMovieToFinish 饶搁 IsLoadingFinished 咯具 茄促. 弊 矫痢俊 Unload 甫 秦档 巩力啊 老绢唱绰 捞蜡甫 舅荐啊 绝绢辑 捞矾绰 扒单..
	//	{
	//		//FlushRenderingCommands(); // IsLoadingFinished 捞饶俊档 角龙利栏肺 肺爹 捞固瘤甫 镜 啊瓷己捞 乐绰 芭扼搁 坊歹 静饭靛 敲矾浆扼档..
	//		UnloadAndClearLoadingImageState();
	//	}
	//	else
	//	{// 父老 酒聪搁 肚 措扁. 角龙利栏肺 捞犯霸 吧府绰 老篮 绝阑 淀..?
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
	//if (GMP) { // GameMoviePlayer 率俊档 鞍捞 技泼阑 秦具 扁夯 浚柳 风凭俊 狼茄 辆丰甫 阜阑 荐 乐促.
	//	GMP->SetUseCustomWait(bUseCustomWaitForLoadingScreen);
	//}
}
void ABladeIIGameMode::WaitForLoadingScreen()
{ 
//	GlobalMarkMapLoadingProgressDone(); // 100% 肺 盲快绰 扒单 荤角 咯扁绰 肺爹 拳搁 场唱绰 镑捞扼 捞芭 捞傈 利例茄 矫痢俊 100% 肺 盲况初酒具..
//
//	// 弊成 肺爹 拳搁捞搁 吝窜捞 瞪 波绊 康惑 敲饭捞甫 矫累茄 版快扼搁 场朝锭鳖瘤 措扁. 捞犯霸 措扁 吝 瘤沥茄 矫埃捞 瘤唱辑 敲饭捞 登绊 乐栏搁 Skip 滚瓢捞 躲 巴
//	IGameMoviePlayer* GMP = GetMoviePlayer();
//	if (GMP) {
//		GMP->WaitForMovieToFinish();
//	}
//
//	B2Scalability::OnGameModeLoadingScreenEnd();
//
//	// 老馆利牢 甘 肺爹捞 酒聪搁辑 GameMoviePlayer 甫 敬促搁 距埃 娟扁啊 促福摆瘤父
//	// 老馆利栏肺 捞 鸥捞怪篮 捞锅 霸烙葛靛狼 肺爹捞 肯丰登绊 肺爹 胶农赴鳖瘤 叭腮 矫痢捞 登摆促.
//	OnLoadingScreenFinished();
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (!IsInPreRenderPhase())
//	{
//		// Pre-render 甫 救 茄促搁 捞 矫痢俊辑 肺爹 矫埃 螟沥 辆丰
//		DevCheckLoadingTimeEnd();
//	}
//#endif
}
void ABladeIIGameMode::ConditionalWaitForLoadingScreen() // WaitForMovieToFinish 俊辑档 眉农绰 窍骨肺 被捞 捞犯霸 救 秦档 瞪 巴 鞍变 茄单..
{
	IGameMoviePlayer* GMP = GetMoviePlayer();
	if (GMP && GMP->IsMovieCurrentlyPlaying()) {
		WaitForLoadingScreen();
	}
}
void ABladeIIGameMode::OnLoadingScreenFinished()
{
	/*
	 捞锅 霸烙葛靛狼 肺爹捞 场抄 矫痢捞 瞪 荐 乐绰单
	 老馆利栏肺绰 捞巴焊促 PreBeginPlay, BeginPlay 殿狼 促弗 "模槛茄" 矫痢阑 荤侩窍绰 霸 亮促.
	 
	 肺爹 拳搁篮 厚悼扁肺 倒酒啊搁辑 漂洒 悼康惑牢 版快绰 皋牢静饭靛啊 霖厚等 捞饶俊绰 皋牢静饭靛甫 阜扁 锭巩俊 
	 老馆利栏肺绰 肺爹 拳搁捞 倒酒啊绰 悼救 盔摹 臼绰 悼累捞 归弊扼款靛肺 倒酒啊绰 巴俊 措秦 快妨且 鞘夸绰 绝促.

	 弊矾唱 埃趣 抗寇 惑炔捞鄂 霸 乐扁 锭巩俊 (浚柳 寇何 敲阀汽 率栏肺 脚龋甫 焊辰促芭唱 窍绰 殿狼)
	 弊繁 版快绰 搬惫 咯扁辑 肺爹 胶农赴捞 场唱绰 鸥捞怪阑 舅酒客具 窃.

	 霸烙葛靛 己拜俊 蝶扼 override 秦辑 荤侩
	*/ 
}

void ABladeIIGameMode::OpenPreLoadingScreen(EPreLoadingScreenType InPreLoadType, EB2GameMode GameMode /*= EB2GameMode::Unknown*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::OpenPreLoadingScreen"));

	ClosePreLoadingScreen(InPreLoadType); // 父俊 窍唱 吝汗秦辑 妮窍霸 瞪 锭 捞傈波 贸府 救秦林搁 粱 巩力啊..
	
	CachedPreLoadingGameMode = GameMode;

	// 盔贰 捞芭 格利捞 弊繁 扒 酒囱单 OnPreLoadMap 俊辑 B2GameLoadingScreenModule 狼 肺爹 胶农赴 剁快扁 傈 肺爹 捞固瘤 肺爹阑 犬角洒 窍绰 开且捞 乐绰 淀 窃.

	// InPreLoadType 眠饶 眠啊登搁 鞘夸俊 蝶扼 荤侩
	//APlayerController* LocalPC = UGameplayStatics::GetLocalPlayerController(this);
	APlayerController* LocalPC = GetWorld()->GetFirstPlayerController();
	TSubclassOf<class UB2PreLoadingScreen>& UsedWidgetClass = IsOneOfPreRenderScreenType(InPreLoadType) ? PreLoadingScreenWidgetClass_PreRender : PreLoadingScreenWidgetClass;
	if (UsedWidgetClass && LocalPC)
	{
		UB2PreLoadingScreen* LocalCreated = CreateWidget<UB2PreLoadingScreen>(LocalPC, UsedWidgetClass);

		if (LocalCreated)
		{
			// PreRenderScreen 篮 窍歹扼档 喊档肺 贸府.. 酒聪搁 角力肺 咯扁辑 PreRender 啊覆阜俊 荤侩且 捞固瘤甫 肺爹窍霸 瞪 荐档 乐绊.
			UTexture2D* LoadedLoadingImage = nullptr;
			if (IsOneOfPreRenderScreenType(InPreLoadType))
			{
				if (InPreLoadType == EPreLoadingScreenType::PLST_PreRender_Default && !LastUsedLoadingImageName.IsEmpty())
				{
					LoadedLoadingImage = LoadObject<UTexture2D>(nullptr, *LastUsedLoadingImageName);
				}
				// PLST_PreRender_First 俊辑绰 弊成 NULL.. UB2PreLoadingScreen 救俊辑 贸府.
			}
			else
			{
				// 老馆利牢 甘 肺爹 流傈 纳捞胶.
				LoadedLoadingImage = GetLoadingImageTexture(GameMode, true);
			}
			
			LocalCreated->AddToViewport(BII_WIDGET_ZORDER_PRELOADINGSCREEN);
			LocalCreated->SetPreLoadingType(InPreLoadType, LoadedLoadingImage);

			if (IsOneOfPreRenderScreenType(InPreLoadType)) {
				CreatedPreLoadingScreen_PreRender = LocalCreated; // 娟绰 蝶肺..
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
		if (CreatedPreLoadingScreen_PreRender) // 娟绰 蝶肺 包府甫..
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
//	if (GIsEditor && !FGameDelegates::Get().GetEndPlayMapDelegate().IsBoundToObject(this)) { //吝酶 官牢爹 救登霸
//		FGameDelegates::Get().GetEndPlayMapDelegate().AddUObject(this, &ABladeIIGameMode::OnEditorEndPlayMap);
//	}
//#endif
//
//	if (!bDeferAutoBeginAuto) // 角力 Match Start 矫痢捞 词绢瘤绰 霸烙葛靛绰 bDeferAutoBeginAuto 甫 True 肺 滴绊 利例茄 矫痢俊 TryAutoBeginAutoAtBeginning 阑 流立 妮窍档废.
//	{
//		TryAutoBeginAutoAtBeginning();
//	}
//
//	if (UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle())
//		DocBattle->ClearBattleDataForStageClear();
//
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 奖俏 抛胶飘 呼靛俊辑档 钎矫..
//	if (gDetectedGarbageExcessiveRenderCount > 0)
//	{
//		BII_SCREEN_LOG(FString::Printf(TEXT("[WARNING] Found %d objects in ExcessiveRenderProcessing!"), gDetectedGarbageExcessiveRenderCount),
//			FLinearColor(1.0, 1.0f, 0.0f, 1.0f), 18, 10.0f);
//		gDetectedGarbageExcessiveRenderCount = 0;
//	}
//#endif
//
//	// UAnimNotifyState_B2CameraAnim::SetBGVisibility 俊辑 汲沥秦稠芭 檬扁拳
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
	//{ // 角力 荤侩登瘤 臼绰 霸烙葛靛啊 积己登菌促啊 力芭瞪 荐 乐澜. Subscribe 甫 沁带 霸烙葛靛老 版快父 Unsubscribe. Unsubscribe 矫 GameMode 啊 unique 窍促绊 啊沥窍骨肺.
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
//		|| 1  // 俺惯 滚傈 橇府轰 敲饭捞 敲府令
//#endif
//		) 
//	{
//		RealtimeUpdateUIDoc();
//	}
//
//	if (IsInPreRenderPhase()) 
//	{
//		UpdateForPreRenderObjects(); // 捞霸 倒酒啊绰 客吝俊绰 酒流 沥侥 矫累 救 茄 芭..
//
//		// Pre-render 俊辑 皋牢 甘 肺爹 拳搁 摧绰 吧 瘤楷矫虐霸 登骨肺 咯扁辑 贸府. Pre-render 侩 肺爹 拳搁篮 蝶肺 乐绊 捞扒 窜瘤 肺爹 槐臂捞 倒酒啊绰 芭 楷厘窍扁 困茄 瞒盔捞促.
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
//	// 葛官老 嘉捞歹 哪颇老 肺弊 敲矾矫
//	if (GDevCheckMobileShaderCompile)
//	{
//		// 酒贰 肺弊 钎矫窍绰 悼救 坊歹 静饭靛俊辑 GShaderCompileCheck_AllCompiledNotLogged 捞芭 眠啊且 啊瓷己 弥家拳. 
//		// 扁瓷 坷悼累俊 刮皑窍瘤绰 臼篮 俺惯侩 内靛捞聪 lock 鞍篮 芭 救 吧绊 利寸洒..
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
//	// 肚 促弗 浚柳 郴何 悼累 肺弊 敲矾矫
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
//		// 酒抗 贸澜何磐 spawn 阑 窍搁辑 窍唱绰 肺爹窍霸 登聪 0 捞搁 扁夯 spawn 档 窍瘤 臼霸 茄促.
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
//		World->ForceGarbageCollection(); // 饭骇 transition 窍搁辑 寸楷洒 瞪 吧肺 积阿沁绰单 角力肺 焊搁 坷宏璃飘啊 粱 巢酒乐绊 GC 甫 倒府绊 唱搁 粱 力芭等促.
//	}
	
#if WITH_EDITOR
	bDevCheckBladeIIGameModeEnteredOnce = true; // 迭 茄锅 柳涝矫 付农秦 狄. 窜鉴 眉农侩档.
#endif

//	B2Scalability::OnGameModeStartPlay(this);
//
//	{
//		// 咯扁辑 林夸 InfoAsset 甸俊 措秦 固府 StaticFind~~ 甫 秦 初绰单 
//		// 捞甸阑 贸澜 龋免窍绰 惑炔俊辑绰 Async 肺爹甸捞 flush 登扁 锭巩俊 固府 茄锅 阂矾林绰 芭.
//		// 捞犯霸 秦 初酒档 促弗 内靛俊辑 流立 StatiFind~~ 甫 WorldContextObject 绝捞 荤侩窍搁 档风宫.
//		// 措何盒 伯单扁父 啊廉坷绰 芭扼 弊府 坷贰 救 吧覆.
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
//		// 咯变 BladeIiGameMode 率俊 某教秦 初绰 扒 酒囱单 咯窍埃 捞率档 酒流 茄锅档 救 阂赴 惑怕俊辑绰 付蛮啊瘤 巩力甫 具扁且 荐 乐澜.
//		StaticFindDialogInfoTable();
//		StaticFindDialogSoundInfoTable();
//		StaticFindQuestTable(); // 胶抛捞瘤 努府绢 矫痢俊 QuestCompleteNotify 哆搁辑 贸澜 阂府搁 flush 唱绰 淀.
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
//	// 促弗 Actor 甸狼 BeginPlay 啊 酒贰 Super::StartPlay 俊辑 矫累凳.
//	PreBeginPlay(); // 肺流 贸府 鸥捞怪 殿栏肺 牢秦 巩力啊 惯积窍搁 WaitForMovieToFinish 焊促 第肺 焊郴具 且 荐档 乐促.
//		
//	// 肺爹 柳青档 魂沥 东俊 固府 捞码俊 舅酒具 窃. 悼康惑阑 敲饭捞窍霸 登搁 肺爹 柳青档绰 焊促 老嘛 盲况 夯促.
//	const FString PossibleOpeningMovieName = GetOpeningMovieName();
//	const bool bSupposedToPlayMove = (PossibleOpeningMovieName.Len() > 0);
//
//	// TSoftObjectPtr 殿栏肺 牢秦 荤傈俊 肺爹捞 救登绢 乐绰 asset 甸 吝 捞锅 胶抛捞瘤俊辑 鞘夸且 巴栏肺 抗惑登绰 巴甸阑 肺爹茄促. 
//	// Synchronous 肺爹捞 器窃登骨肺 酒贰 芭摹搁辑 矫埃捞 粱 吧覆. 弊矾唱 柳楼 奴 扒 Async 肺爹阑 窃.
//	PreloadAnyNecessaryInfo(bSupposedToPlayMove);
//
//	// Preload 客 厚搅窍瘤父 促弗 瞒盔俊辑.. 葛官老 嘉捞歹 哪颇老阑 困茄 Pre-render
//	if (ShouldSetupPreRender())
//	{
//		SetupPreRenderObjects();
//	}
//
//	IStreamingManager::Get().StreamAllResources(2.0f);
//	
//	////////////////////////////////////////////
//	bool bPlayingOpeningMovie = false; // 角力 敲饭捞 咯何..
//	{ // 胶抛捞瘤 档涝 康惑捞 乐绰 版快 悼康惑 敲饭捞
//	  // 皋牢 俊悸 肺爹 (PreloadAnyNecessaryInfo) 傈俊 矫累阑 窍搁 亮篮单 趣咯唱 俊悸 肺爹 促 登扁 傈俊 康惑捞 场唱滚副 荐 乐促. 风俏阑 窍芭唱 酒聪搁 弊成 鳖父 拳搁捞 唱坷摆瘤.
//		// 捞饶 BeginPlay 甫 芭摹搁辑档 捞巴历巴 且 抛聪 捞码辑 撇绢焊磊.		
//		if (bSupposedToPlayMove)
//		{
//			// 捞 版快 PreloadAnyNecessaryInfo 甫 芭摹搁辑 柳青档档 促弗 版快焊促 腹捞 棵扼艾阑 巴.
//			// 场鳖瘤 盲快妨聪 酒贰 AsyncFlush 夸家啊 乐绢辑.. 悼康惑 敲饭捞档 肺爹 胶农赴牢 父怒 盲况初绊 茄曼 扁促府蠢聪 距埃 巢败滴磊.
//			B2GMLoadingProgCollector::StepCustom(0.1f, 0.9f); 
//
//			// 捞霸 Async 肺爹 flush 甫 老栏虐绊.. 捞吧 救敬促 秦档 LoadingScreenModule 救俊辑 郴何利栏肺 肺爹窍绰 霸 肚 乐绢辑 .. ばば 弊成 刚历 肺爹秦 狄..
//			FString LoadingImageName = GetLoadingImageName(false);
//
//			GetMoviePlayer()->WaitForMovieToFinish(); // 扁粮 肺爹 胶农赴阑 肛冕促.
//			
//			// AndroidThunkCpp_StageMoviePlay(MovieName); 荐撇啡阑 订 敲阀汽 磊眉 扁瓷栏肺 捞繁 侥栏肺 悼康惑 敲饭捞甫 窍扁档..
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
//				// StartGameLoadingScreen_Movie 俊 傈眉拳搁 咯何, Skip滚瓢 钎矫 咯何 殿狼 牢磊甫 持绢临 荐 乐澜. 鞘夸俊 蝶扼 荤侩.
//				GetMovieSubtitles(LoadingScreenModule->StartGameLoadingScreen_Movie(*PossibleOpeningMovieName, 
//					// 弊成 FitToMin 栏肺 窍搁 促剧茄 悼康惑 厚啦俊 措茄 措贸啊 啊瓷窍唱 AppleMovieStreamer 备炼客 包访秦辑 IOS 老何 扁扁俊辑 老矫利栏肺 拳搁 厚啦捞 儒飘矾瘤绰 巩力啊 惯积且 荐 乐促.
//					// DLC 鸥捞撇俊辑 肺厚 肺爹阑 力寇茄 扁鸥 肺爹 悼康惑捞 16:9 肺 力累登绰 茄 咯扁绰 StdHD 肺 绊沥.
//					ELoadingScreenMovieAlignMode::FitToMin_StdHD,
//					true	// 弥檬 敲饭捞何磐 Skip 啊瓷窍档废 函版
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
//		// 捞 矫痢俊辑 LoadingImage 甫 Unload 秦 林搁 利例且 芭 鞍篮单 农贰浆啊 唱绰 版快啊 咯傈洒 乐促. 肯傈洒 坊歹傅 窜鳖瘤 救 场车阑 啊瓷己捞.. 泵咯瘤氓栏肺 鸥捞赣甫.
//		UWorld* TheWorld = GetWorld();
//		if (TheWorld)
//		{
//			TheWorld->GetTimerManager().SetTimer(LoadingImageUnloadTH, this, &ABladeIIGameMode::OnLoadingImageUnloadTimerCB, 1.0f, false);
//		}
//	}
//	else
//	{ 
//		if (PreRenderer && !PreRenderer->IsInPreRenderGameMode()) {
//			// 老馆利牢 饭骇 肺爹鸥烙俊辑狼 PreRender 惑炔.
//			// SetupPreRenderObjects 矫俊绰 救冈鳃 荐 乐绢辑 咯扁辑 Light 券版阑 悸诀窍绰单, PreRenderGM 俊辑绰 喊档肺 漂喊洒 Light pass 甫 倒副 芭扼 咯扁辑 窍瘤 臼绰促.
//			PreRenderer->ConditionalSetupPreRenderLightEnvForDynPL();
//		}
//
//		if (!bPlayingOpeningMovie) {
//			// PreRender 俊辑绰 扁夯 肺爹 拳搁 摧绰 风凭栏肺绰 槐臂捞 肛冕 饶俊 矫埃 瘤楷捞 粱 乐绢辑 肺爹 拳搁 摧绰 吧 粱 词冕促.
//			// 窜 悼康惑 敲饭捞窍霸 登搁 捞霸 坷洒妨 悼康惑狼 谗辫阑 蜡惯秦辑 绢驴 荐 绝瘤..
//			SetUseCustomWaitForLoadingScreen(true);
//		}
//	}
//		
//	// 肺爹 拳搁阑 摧绰 WaitForLoadingScreen 阑 措眉肺 咯扁辑 窍瘤父 酒囱 版快档 辆辆 乐摆瘤..
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
//#if BII_STATS // PerfStat 篮 MatchStart 矫痢何磐 葛栏扁 矫累.
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
	const FString& SavedKeyName = GetLocalSavedAutoStateKeyName(); // 霸烙葛靛 喊 Auto 惑怕甫 历厘且 ini 虐 蔼
	if (GConfig && SavedKeyName.Len() > 0) {
		GConfig->GetInt(TEXT("AutoState"), *SavedKeyName, bRetSavedState, GB2UserSavedStateIni);
	}
	return bRetSavedState;
}

void ABladeIIGameMode::SaveLocalSavedAutoType() const
{
	const FString& SavedKeyName = GetLocalSavedAutoStateKeyName(); // 霸烙葛靛 喊 Auto 惑怕甫 历厘且 ini 虐 蔼
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

	return Result; // 捞 炼扒篮 窍困 努贰胶俊辑 扁夯栏肺 器窃且 鞘夸啊 乐摆促.
}

void ABladeIIGameMode::TryAutoBeginAutoAtBeginning()
{ // 阿 葛靛 喊 荤侩磊啊 捞傈俊 auto 甫 荤侩沁绰瘤 咯何俊 蝶扼 磊悼栏肺 auto 甫 倒府扁 困窃. 匙箂白葛靛狼 AI 敲饭捞绢绰 秦寸救凳.

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	ABladeIIPlayer* LocalPlayer = PC ? Cast<ABladeIIPlayer>(PC->GetCharacter()) : nullptr;

	if (LocalPlayer && ShouldStartAutoFromBeginning())
	{
		// Auto 甫 掺绰 楷免捞 泅犁 柳青吝牢啊甫 焊绊 酒聪搁 寸厘 矫累篮 救茄促. 绢瞒乔 楷免捞 场唱搁 舅酒辑 矫累瞪 巴烙.
		bool bAutoStopEventRunning = false;
		for (AB2StageEventDirector* CurrSED : EventDirectors)
		{
			if (CurrSED && CurrSED->IsPlayingShow() && CurrSED->ShouldStopAutoDuringShow())
			{
				bAutoStopEventRunning = true;
				break;
			}
		}

		if (!bAutoStopEventRunning)
		{
			SetPlayerAutoType(GetLocalSavedAutoType());
		}
		else
		{ // 寸厘 Auto 矫累篮 救 窍歹扼档 UIDoc 俊 技泼篮 秦具.. StageEventDirector 率俊辑 Auto 汗备甫 且 锭俊绰 捞扒 贸府 救 且 巴捞骨肺.
			UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
			if (DocBattle)
			{
				DocBattle->SetInAutoPlay(GetLocalSavedAutoType());
			}
		}
	}
	else 
	{
		SetPlayerAutoType(GetLocalSavedAutoType());
	}
}

//PVP率俊辑 AutoAIController甫 肺爹窍绰 泅惑捞 乐绢 啊惑皋家靛肺 倒妨辑 B2PVPGameMode俊辑 坷滚扼捞靛 窃
void ABladeIIGameMode::SetPlayerAutoType(int32 InAuto) // 老矫利牢 On/Off 啊 酒囱 泅 霸烙葛靛俊辑 蜡瘤登绰 auto 惑怕.
{
	//if (PlayerAutoBattleType != InAuto)
	//{
	//	PlayerAutoBattleType = InAuto;
	//	SaveLocalSavedAutoType();
	//}

	//// 捞吧 妮 窍绰 惑炔俊辑绰 楷免 殿俊 狼秦 烙矫利栏肺 Auto 啊 波柳促芭唱 窍绰 扒 公矫茄促. 弊巴鳖瘤 积阿窍妨搁 粱 汗棱窍绊 弊凡 鞘夸鳖瘤绰 绝阑 淀窃.
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

	//// UI 肺 矫弊澄篮 扁夯利栏肺绰 捞吧肺父 焊晨. 老矫利牢 On/Off 俊辑绰 救 焊郴绊. 辟单 窍促焊聪 弊犯霸父 登瘤绰 臼绰焙 铝..
	//UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	//if (DocBattle)
	//{
	//	DocBattle->SetInAutoPlay(PlayerAutoBattleType);
	//}
}

void ABladeIIGameMode::ToggleLocalPlayerAutoState()
{
	SetPlayerAutoType(UpdateAutoBattleType(PlayerAutoBattleType));
}

bool ABladeIIGameMode::IsLocalPlayerAutoBattle()
{
	bool Result = false;

	if (PlayerAutoBattleType == AutoBattleType::Attack || PlayerAutoBattleType == AutoBattleType::Skill)
	{
		Result = true;
	}

	return Result;
}

void ABladeIIGameMode::TemporaryStopLocalPlayerAuto()
{// bMaintainedLocalPlayerAutoState 绰 蜡瘤窍绊 捞吧肺绰 UI Doc 技泼阑 窍瘤 臼绰促. 楷免 殿栏肺 老矫利栏肺 off 且 锭 荤侩.
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	ABladeIIPlayer* LocalB2Player = PC ? Cast<ABladeIIPlayer>(PC->GetCharacter()) : nullptr;

	if (LocalB2Player && LocalB2Player->IsInAutoPlay()) 
	{
		LocalB2Player->StopAutoPlay();
	}	
}
void ABladeIIGameMode::RestoreLocalPlayerAuto()
{ // 扁夯 惑怕啊 true 牢 版快父 汗备.
	if (PlayerAutoBattleType != AutoBattleType::NONE)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		ABladeIIPlayer* LocalB2Player = PC ? Cast<ABladeIIPlayer>(PC->GetCharacter()) : nullptr;
		if (LocalB2Player) 
		{
			LocalB2Player->StartAutoPlay();
		}
	}
}

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
//	// 促弗 Actor 甸狼 BeginPlay 啊 阂府扁 傈俊 贸府登绢具 且 巴甸阑 咯扁俊 茎寂持绢 焊磊.
//
//	BladeIIGameImpl::GetLocalCharacterData().SetCharClassNames(StaticFindPCClassInfoBox(this)); // 傈眉 葛碘 瞒盔俊辑 檬扁 茄锅父 鞘夸茄 鞘荐沥焊. AccountInfo 甫 罐绰 镑阑 力寇窍搁 迭洒 利寸茄 镑捞 绝绢辑..
//
//	UpdateManagedActorList();
//
//	// It will be picked up by player's BeginPlay.
//	SetDesiredPCClassEnum();
//
//	// 愧父窍搁 DefaultPawnClass 绰 匙捞萍宏 扁夯蔼栏肺 技泼茄 弊措肺
//	// GetCurrentPCClassInfo 狼 BaseBPClass 肺 技泼秦辑绰 救凳. 扁夯 spawn 篮 匙捞萍宏 扁馆栏肺 窍绊 BaseBPClass 绰 PCClassInfo overriding 傈俊 利侩阑 茄促.
//#if WITH_EDITOR
//	checkSlow(!IsDefaultPawnTestBP()); // 沥侥 甘俊辑绰 吧府搁 救等促. 老何 抛胶飘 甘俊辑绰 吧副 荐 乐澜. 弊繁 版快绰 弊成 逞绢啊档废.
//#endif
//
//#if WITH_EDITOR
//	if (GIsEditor)
//	{ // UIManager 甫 甘阑 货肺 肺爹且 锭付促 货肺 积己窍绰单 俊叼磐俊辑绰 救冈鳃 锭啊 乐绰 芭 鞍促.
//		UB2UIManager::EditorGetNewInstance();
//	}
//	else
//	{// UIManager 啊 static 茄 备炼肺 父甸绢脸绰单 攫府倔 备炼俊辑 困氰己捞 乐栏骨肺 甘 傈券 矫 destroy 甫 茄促. 拌裙措肺扼搁 捞 矫痢俊辑 UIManager 绰 货肺 积己窍绰 巴捞绢具 窃.
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
//	//UI刀概聪历 檬扁拳
//	UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
//	if (UIDocManager){
//		UIDocManager->Init(this);
//	}
//
//	// B2UIManager_InGameCombat 篮 DJLegacy 牢单 咯傈洒 荤侩. 捞抚父 InGameCombat 捞绊 角力肺绰 ABladeIIGameMode 甫 荤侩窍绰 葛电 镑俊 粮犁窍摆焙. SetCenterMessage 蜡瓶东俊 鞘夸窍变 窃.
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

void ABladeIIGameMode::UpdateManagedActorList()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::UpdateManagedActorList"));

	// 饭骇 胶飘府怪捞扼档 茄促搁 PreBeginPlay 捞饶扼档 蝶肺 阂矾临 鞘夸啊 积辨 巴.

	RemoveAllManagedActors();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		if ((*It)->Tags.Num() > 0)
		{
			AddTaggedActor((*It));
		}

		// 娟甸 荤捞俊 辑肺 BeginPlay 矫痢狼 狼粮 包拌啊 乐阑 荐 乐绢辑 BeginPlay 矫累 傈俊 固府 殿废秦 敌促.
		auto* CurrSpawnPool = Cast<AB2MonsterSpawnPool>(*It);
		if (CurrSpawnPool)
		{
			AddSpawnPool(CurrSpawnPool);
			continue;
		}

		auto* CurrSED = Cast<AB2StageEventDirector>(*It);
		if (CurrSED)
		{
			AddStageEventDirector(CurrSED);
			continue;
		}

		auto* CurrWayPoint = Cast<AB2AutoWayPoint>(*It);
		if (CurrWayPoint)
		{
			AddAutoWayPoint(CurrWayPoint);
			continue;
		}

		auto* CurrAC = Cast<AB2ActiveCameraActor>(*It);
		if (CurrAC)
		{
			AddActiveCameraActor(CurrAC);
			continue;
		}

		auto* CurrTC = Cast<AB2TreasureChestBase>(*It);
		if (CurrTC)
		{
			AddTreasureChest(CurrTC);
			continue;
		}

		auto* CurrDO = Cast<AB2DestructibleLevelObjBase>(*It);
		if (CurrDO)
		{
			AddDestructibleObject(CurrDO);
			continue;
		}

		auto* CurrHomePoint = Cast<AB2HomePoint>(*It);
		if (CurrHomePoint)
		{
			AddHomePoint(CurrHomePoint);
			continue;
		}

		auto* CastedDialogTrigger = Cast<AB2DialogTriggerActorBase>(*It);
		if (CastedDialogTrigger)
		{
			AddDialogTriggerActor(CastedDialogTrigger);
			continue;
		}
	}

	if (AutoWayPoints.Num() >= 2)
		AutoWayPoints.Sort([](const AB2AutoWayPoint& a, const AB2AutoWayPoint& b){ return a.PassOrder < b.PassOrder; });

	DecideAndSetActiveSpawnPool(); // 府胶飘 促 瞒搁 捞锅 饭骇俊辑 荤侩瞪 SpawnPool 窍唱甫 茫酒辑 技泼.
}

void ABladeIIGameMode::RemoveAllManagedActors()
{
	SpawnPools.Empty();
	EventDirectors.Empty();
	AutoWayPoints.Empty();
	ActiveCameraActors.Empty();
	TreasureChests.Empty();
	DestructibleObjects.Empty();
	HomePoints.Empty();
	TaggedActors.Empty();
	DialogTriggers.Empty();
}

void ABladeIIGameMode::SetDesiredPCClassEnum()
{
//	if (Utilities::FConnectionState::GetInstance().IsConnected())
//	{
//		// Main 阑 荤侩窍绰 巴篮 扁夯甘 敲饭捞且 锭捞绊 Extra 甘狼 版快绰 捞傈俊 付瘤阜栏肺 敲饭捞茄 current 努贰胶甫 技泼秦具 窃. AB2StageGameMode::SetDesiredPCClassEnum 曼绊.
//		CurrentPCClassEnum = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
//	}
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	else
//	{
//		// 楷搬 救 等 惑怕咯档 俺惯 抛胶飘 侩 窜刀 敲饭捞 葛靛档 乐绊 LocalCharacterData 俊辑绰 肺拿俊 历厘等 蔼阑 肺爹秦 初疽阑 荐档 乐绢辑 弊 蔼阑 荤侩.
//		CurrentPCClassEnum = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
//	}
//#endif
//
//	// 梅浚 购啊 蔼捞 乐绢具 救傈窃.
//	if (CurrentPCClassEnum == EPCClass::EPC_End)
//		CurrentPCClassEnum = DefaultPCClassEnum;
}

void ABladeIIGameMode::SubscribeEvents()
{ 
	if (bEventsSubscribed)
		return;

	//// 措眉肺 UI 包访 捞亥飘 罐扁 困窃.
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

	bEventsSubscribed = true; // SubsResponseGetQuestRewardcribeEvents 甫 沁促搁 绢骂绢骂 歹固肺 积己登绰 GameMode 绰 酒匆 巴.
}

void ABladeIIGameMode::UnsubscribeEvents()
{
	Issues.Empty();

	bEventsSubscribed = false;
}

void ABladeIIGameMode::DecideAndSetActiveSpawnPool(bool bFallBackToLowerDifficulty)
{ // 漂沥 霸烙葛靛俊辑唱 (StageGameMode 扁馆) 泅犁 惑炔俊辑狼 狼固乐绰 SpawnPool 肺 技泼捞 瞪 巴.
	//TheActiveSpawnPool = (SpawnPools.Num() > 0) ? SpawnPools[0] : NULL;
}

AActor* ABladeIIGameMode::FindPlayerStartForCurrStage(AController* InController)
{
	return FindPlayerStart(InController); // 芭狼 StageGameMode 侩捞促. 荤沥捞 乐绢辑 咯扁肺 缠绊 咳.
}

void ABladeIIGameMode::RestartGame()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::RestartGame"));

//	// AB2StageGameMode::RestartGame 俊辑绰 扁瓷阑 促福霸 沥狼秦辑 荤侩窍绊 乐栏聪 措何盒狼 版快绰 弊率阑 犬牢.
//#if WITH_EDITOR
//	if (GIsEditor)
//	{
//		// 俊叼磐俊辑绰 窜瘤 Play 技记阑 场辰促.
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

	// 馆汗登绰 内靛甸捞 乐绢辑 茄单 葛酒初篮 镑俊 乐绰 芭 荤侩.
	//StartBladeIIGameTutorial(this);
}

void ABladeIIGameMode::GoToVillage()
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::GoToVillage"));

	OpenPreLoadingScreen(EPreLoadingScreenType::PLST_GoToLobby, EB2GameMode::Lobby); // 肺厚肺 倒酒哎 锭 肺爹 胶农赴 包访秦辑 农贰浆啊 粱 乐绰单 捞吧 捞 矫痢俊辑 固府 茄锅 肺爹秦林绰 霸 救傈窍促绰 啊汲捞 乐绢辑 秦 航..

	FLobbySceneManager::DeferredRegistChangeLobbyScene([](){
		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_LobbyMain);
	});
		
	OpenBladeIILobbyCommon(this); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
}

void ABladeIIGameMode::GoToVillageDefeated()
{
	GoToVillage(); // 窍困 努贰胶俊辑 鞘夸俊 蝶扼 促福霸 静档废..
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
	// 老窜 LobbyGameMode 俊辑.. 牢霸烙俊辑档 惑痢 柳涝阑 秦具 且 荐 乐澜.
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
	// 阿 饭骇 喉风橇赴飘 (ALevelScriptActor) 俊辑 府家胶 某教 (官牢爹) 悼累阑 漂沥 矫痢俊 龋免窍扁 困窃.
	// 抗甫 甸绢 BeginPlay 矫痢俊 林肺 窍绰 吧 促弗 鸥捞怪俊档 角青窍扁 困秦.

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

ABladeIIPlayer* ABladeIIGameMode::SpawnPCClassInfoPlayer(
	class UWorld* SpawnWorld,
	EPCClass InClass,
	const FTransform& SpawnTransform,
	FActorSpawnParameters& SpawnParameters,
	bool bSpawnAsDummyPuppet,
	UClass* OverridePawnClass)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::SpawnPCClassInfoPlayer"));

	UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(this);
	if (SpawnWorld && InClass != EPCClass::EPC_End && PCBox)
	{
		UB2PCClassInfo* ClassInfo = PCBox->GetSingleClassInfo(InClass);
		if (ClassInfo)
		{
			// It will be picked up by player's BeginPlay.
			CurrentPCClassEnum = InClass;

			UClass* PawnClass = (OverridePawnClass != nullptr) ? OverridePawnClass : DefaultPawnClass;

			// ✅ 改用 SpawnActorDeferred
			ABladeIIPlayer* SpawnedActor = Cast<ABladeIIPlayer>(
				SpawnWorld->SpawnActorDeferred<ABladeIIPlayer>(
					PawnClass,
					SpawnTransform,
					nullptr, // Owner
					nullptr, // Instigator
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn
				)
			);

			if (SpawnedActor)
			{
				SpawnedActor->FinalizePCClassInfoSpawn(bSpawnAsDummyPuppet);

				// ✅ 必须手动 Finish
				UGameplayStatics::FinishSpawningActor(SpawnedActor, SpawnTransform);
				return SpawnedActor;
			}
		}
	}

	BII_CHECK(0);
	return nullptr;
}


ABladeIIPlayer* ABladeIIGameMode::StaticSpawnPCClassInfoPlayer(UObject* WorldContextObject, EPCClass InClass, const FTransform& SpawnTransform, 
	ESpawnActorCollisionHandlingMethod CollisionHandlingOverride /*= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn*/, bool bDeferConstruction /*= true*/, AActor* InOwner /*= nullptr*/)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StaticSpawnPCClassInfoPlayer"));

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = InOwner;
	SpawnParam.bDeferConstruction = bDeferConstruction; // 捞吧 true 肺 窍搁 捞芭 龋免 捞饶俊 FinishSpawning 阑 蝶肺 阂矾林绢具 窍绰 锅芭肺框捞 乐瘤父 老馆利牢 霸烙敲饭捞 惑炔俊辑狼 肺拿 敲饭捞绢 某腐磐扼搁 BeginPlay 矫痢俊 PlayerController 俊 狼秦 Possess 等 惑怕咯具 且 巴捞骨肺 鞘夸窃.
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
	// GameMode 矫累 矫 default pawn spawn 矫痢俊父 阂覆. BeginPlay 棺 哪器惩飘甸 register 捞傈.
	ABladeIIPlayer* SpawnedB2Player = Cast<ABladeIIPlayer>(InSpawned);
	if (SpawnedB2Player)
	{
		SpawnedB2Player->FinalizePCClassInfoSpawn();
		
		// 肺爹 柳青档 笼拌.. FinalizePCClassInfoSpawn 档吝 BaseBP 肺爹窍搁辑 购啊 菊辑 夸没茄 巴甸捞 flush 登绰 淀 腹篮 矫埃捞 家葛..
		B2GMLoadingProgCollector::StepCustom(0.4f);

#if WITH_EDITOR // 酒流 PCClass 啊 绝绰 抛胶飘 惑炔俊辑绰.. CharacterType 阑 烙狼肺 霖促. 捞芭扼档 救 持绢林搁 农贰浆 唱绰 镑捞 呈公 腹澜.
		if (IsDefaultPawnTestBP() && CurrentPCClassEnum == EPCClass::EPC_End)
		{
			SpawnedB2Player->CharacterType = ECharacterType::ECT_Gladiator;
		}
#endif
	}
}

ABladeIIPlayer* ABladeIIGameMode::SpawnPlayerCharAsPuppet(UObject* WorldContextObject, EPCClass InClass, const FTransform& InSpawnTransform, bool bPossessByAIController, UClass* OverridePawnClass)
{	
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::SpawnPlayerCharAsPuppet"));

	ABladeIIGameMode* B2GameMode = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	UWorld* TheWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (InClass != EPCClass::EPC_End && B2GameMode && TheWorld)
	{		
		FActorSpawnParameters SpawnParam;
		SpawnParam.bDeferConstruction = false; // 咯变 DeferContruction 蝶困 且 鞘夸啊 绝摆瘤..
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABladeIIPlayer* SpawnedPuppet = B2GameMode->SpawnPCClassInfoPlayer(TheWorld, InClass, InSpawnTransform, SpawnParam, true, OverridePawnClass); // SpawnAsDummyPuppet 牢磊甫 林搁 InGameInfo 荤侩 绝捞 spawn 阑 柳青窍霸 瞪 巴.

		if (SpawnedPuppet)
		{
			if (bPossessByAIController)
			{
				// APawn::PostInitializeComponents 俊辑 炼扒俊 蝶扼 积己阑 窍绰单, 
				// 扁夯 AutoPossessAI 甫 Spawned 肺 窍扁俊绰 PlayerController 俊 possess 登绰 版快甫 积阿窍搁 扯厚捞绊 
				// Puppet spawn 矫俊绰 咯窍埃 吝仿阑 罐栏妨搁 扁夯 AI controller 啊 鞘夸窍聪 喊档肺 阂矾霖促.
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

	BII_CHECK(Player);

	auto* B2PC = UGameplayStatics::GetPlayerController(this, 0);
	ABladeIIPlayer* B2Player = B2PC ? Cast<ABladeIIPlayer>(B2PC->GetCharacter()) : nullptr;
	UB2SkillRTManager* SkillRTManager = B2Player ? B2Player->GetSkillRTManager() : NULL;
	
	BII_CHECK(SkillRTManager && B2Player);
	return SkillRTManager->TagToOther((int32)B2Player->TagAttackMode, B2Player->TagAttackMode == ETagAttackMode::EAM_Disable ? EAttackState(EAttackState::ECS_None) : B2Player->GetAttackState());
	return true;
}

bool ABladeIIGameMode::RequestResurrect(EResurrectBuffType InResurrectBuffType)
{
	return false; // 窍困 霸烙 葛靛 努贰胶俊辑 备泅.
}

// 柳悼 肺厚俊急 敲饭捞绢 绝栏聪 捞芭静磊. 促弗单辑档 结滴凳.
void ABladeIIGameMode::ForceFeedback()
{
	BLADE2_SCOPE_CYCLE_COUNTER(ABladeIIGameMode_ForceFeedback);
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::ForceFeedback"));

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		UB2SomeInfo* SomeInfo = StaticFindSomeInfo(this);
		if (IsVibration() && SomeInfo && SomeInfo->GetForceFeedbackEffect())
		{
			PC->ClientPlayForceFeedback(SomeInfo->GetForceFeedbackEffect());
		}
	}
}

void ABladeIIGameMode::NotifySpawnPoolMobSpawn(class ABladeIICharacter* SpawnedMob, const FMobSpawnedBirthplaceInfo& InBirthplaceInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolMobSpawn"));

	checkSlow(!InBirthplaceInfo.Birthplace || InBirthplaceInfo.Birthplace == TheActiveSpawnPool);

	if (InBirthplaceInfo.Birthplace && SpawnedMob)
	{
		SpawnedMob->SetBirthPlace(InBirthplaceInfo);
	}
}

void ABladeIIGameMode::NotifySpawnPoolComplete(class AB2MonsterSpawnPool* CompletedSpawnPool)
{
	checkSlow(!CompletedSpawnPool || CompletedSpawnPool == TheActiveSpawnPool);
}

void ABladeIIGameMode::NotifySpawnPoolSingleWaveStart(AB2MonsterSpawnPool* StartedSpawnPool, int32 WaveNum)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolSingleWaveStart"));

	checkSlow(!StartedSpawnPool || StartedSpawnPool == TheActiveSpawnPool);

	for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
	{
		AB2StageEventDirector* CurrDirector = EventDirectors[DI];
		if (CurrDirector)
		{
			CurrDirector->NotifySpawnPoolSingleWaveStart(WaveNum);
		}
	}
}

void ABladeIIGameMode::NotifySpawnPoolSingleWaveComplete(AB2MonsterSpawnPool* CompletedSpawnPool, int32 WaveNum)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolSingleWaveComplete"));

	checkSlow(!CompletedSpawnPool || CompletedSpawnPool == TheActiveSpawnPool);

	for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
	{
		AB2StageEventDirector* CurrDirector = EventDirectors[DI];
		if (CurrDirector)
		{
			CurrDirector->NotifySpawnPoolSingleWaveComplete(WaveNum);
		}
	}
}

void ABladeIIGameMode::NotifySpawnPoolWaveMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob, int32 WaveNum, int32 MobIndex)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolWaveMobDead"));

	BII_CHECK(DyingMob && DyingMob->IsValidObj());
	checkSlow(!SpawnPool || SpawnPool == TheActiveSpawnPool);

	for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
	{
		AB2StageEventDirector* CurrDirector = EventDirectors[DI];
		if (CurrDirector)
		{
			CurrDirector->NotifySpawnPoolSingleMobDead(DyingMob, WaveNum, MobIndex);
		}
	}

	// SpawnPool 焊促 StageEventDirector 俊 刚历 Notify 甫 临 鞘夸啊 乐促. 
	// 弊贰具 俊辑 MobDying 捞亥飘 贸府 矫 WaveNum 苞 ObjIndex 肺 DyingMonster 甫 啊廉棵 荐啊 乐澜.
	// 拱沸 捞 版快 delay 绝捞 官肺 show 甫 矫累秦具 窃. 弊霸 酒聪扼搁 DyingMonstser 甫 绢恫 侥栏肺电 某教秦辑 结具 窃.

	if (SpawnPool)
	{
		SpawnPool->OnSpawnedMonsterDead(DyingMob, WaveNum, MobIndex, false);
	}
}

void ABladeIIGameMode::NotifySpawnPoolWaveMobPhaseChange(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* ChangedMob, class ABladeIICharacter* DestroyedPrevRef, int32 WaveNum, int32 MobObjIndex, int32 NewPhase)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolWaveMobPhaseChange"));

	// DestroyedPrevRef 绰 NULL 捞 酒囱 版快扼档 IsValidObj 窍瘤 臼阑 巴烙.

	BII_CHECK(ChangedMob && ChangedMob->IsValidObj());
	checkSlow(!SpawnPool || SpawnPool == TheActiveSpawnPool);

	for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
	{
		AB2StageEventDirector* CurrDirector = EventDirectors[DI];
		if (CurrDirector)
		{
			CurrDirector->NotifySpawnPoolSingleMobPhaseChange(ChangedMob, DestroyedPrevRef, WaveNum, MobObjIndex, NewPhase);
		}
	}
}

void ABladeIIGameMode::NotifySpawnPoolSummonedMobDead(class AB2MonsterSpawnPool* SpawnPool, class ABladeIICharacter* DyingMob)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifySpawnPoolSummonedMobDead"));

	checkSlow(!SpawnPool || SpawnPool == TheActiveSpawnPool);

	// 咯扁急 StageEventDirector 率 烹瘤绰 鞘夸绝摆澜. 父老 鞘夸秦柳促搁 酒付档 SpawnPool 焊促绰 刚历 烹瘤.

	if (SpawnPool)
	{
		SpawnPool->OnSpawnedMonsterDead(DyingMob, -1, -1, true); // 家券捞搁 WaveNum 殿篮 鞘夸绝澜.
	}
}

void ABladeIIGameMode::NotifyForgetACharacter(class ABladeIICharacter* CharacterAboutToBeDestroyed)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifyForgetACharacter"));

	// Notify**Dead 啊 霸烙 敲饭捞惑狼 磷澜阑 狼固茄促搁 捞扒 攫府倔 坷宏璃飘 瞒盔狼 力芭 流傈烙. 逞绢柯 坷宏璃飘甫 曼炼窍绰 霸 乐促搁 葛滴 NULL out.

	BII_CHECK(CharacterAboutToBeDestroyed && CharacterAboutToBeDestroyed->IsValidObj()); // 酒流篮 Valid 秦具摆瘤..

	for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
	{
		AB2StageEventDirector* CurrDirector = EventDirectors[DI];
		if (CurrDirector)
		{
			CurrDirector->NotifyForgetACharacter(CharacterAboutToBeDestroyed);
		}
	}
}

void ABladeIIGameMode::StopAllStageEventDirector(class AB2StageEventDirector* skipDirector)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StopAllStageEventDirector"));

	for (int32 DI = 0; DI < EventDirectors.Num(); ++DI)
	{
		AB2StageEventDirector* CurrDirector = EventDirectors[DI];
		if (CurrDirector && CurrDirector->IsPlayingShow() && CurrDirector != skipDirector)
		{
			CurrDirector->CustomFinishShow();
		}
	}
}

void ABladeIIGameMode::NotifyStageEventSceneBegin(class AB2StageEventDirector* BegunDirector, EStageEvent EventType)
{
}
void ABladeIIGameMode::NotifyStageEventSceneEnd(class AB2StageEventDirector* EndedDirector)
{
}

void ABladeIIGameMode::NotifyUIWidgetOpenComplete(FName InOpenedUI)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::NotifyUIWidgetOpenComplete"));

	if (AB2StageEventDirector::IsStageEventRelatedUI(InOpenedUI))
	{
		for (AB2StageEventDirector* CurrSED : EventDirectors)
		{
			if (CurrSED)
			{
				CurrSED->NotifyStageEventUIOpenComplete(InOpenedUI);
			}
		}
	}
}

void ABladeIIGameMode::AddSpawnPool(class AB2MonsterSpawnPool* InNewSpawnPool)
{
	if (InNewSpawnPool)
	{
		SpawnPools.AddUnique(InNewSpawnPool);
		InNewSpawnPool->SetCurrentGameMode(this);
	}
}

void ABladeIIGameMode::AddStageEventDirector(class AB2StageEventDirector* InNewEventDirector)
{
	if (InNewEventDirector)
	{
		EventDirectors.AddUnique(InNewEventDirector);
	}
}

void ABladeIIGameMode::AddActiveCameraActor(class AB2ActiveCameraActor* InNewActiveCameraActor)
{
	if (InNewActiveCameraActor)
	{
		ActiveCameraActors.AddUnique(InNewActiveCameraActor);
	}
}

class AB2ActiveCameraActor* ABladeIIGameMode::FindActiveCameraActorByTag(FName ACATagName)
{
	for (AB2ActiveCameraActor* pACA : ActiveCameraActors)
	{
		if (pACA->ActorHasTag(ACATagName))
		{
			return pACA;
		}
	}

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
	//if (TheActiveSpawnPool) // 傈眉 SpawnPool 阑 iteration 且 鞘夸啊 绝绢具 茄促.
	//{
	//	TArray<class ABladeIICharacter*> CurrList;
	//	TheActiveSpawnPool->GetWaveMobList(WaveNum, CurrList, bOnlyLiveMobs);
	//	OutList.Append(CurrList);
	//}
}

void ABladeIIGameMode::GetAllWaveMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /*= true*/)
{
//	if (TheActiveSpawnPool) // 傈眉 SpawnPool 阑 iteration 且 鞘夸啊 绝绢具 茄促.
//	{
//		TArray<class ABladeIICharacter*> CurrList;
//		TheActiveSpawnPool->GetAllWaveMobList(CurrList, bOnlyLiveMobs);
//		OutList.Append(CurrList);
//	}
}

void ABladeIIGameMode::GetAllSummonedMobList(TArray<class ABladeIICharacter*>& OutList, bool bOnlyLiveMobs /*= true*/)
{
	//if (TheActiveSpawnPool) // 傈眉 SpawnPool 阑 iteration 且 鞘夸啊 绝绢具 茄促.
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
//	// 俊叼磐俊急 抛胶飘档 腹捞 窍绊 窍聪SpawnNPC 目盖靛肺 积己茄 辟夯绝绰 各甸档 鸥百 眉农秦 霖促.
//	for (FActorIterator It(GetWorld()); It; ++It) // ActorIterator 捞骨肺 抛胶飘 寇浚 公府.
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
{ // StageEventDirector 率俊辑 楷免 厘搁 辆丰 饶俊 敲饭捞绢 某腐磐 炮饭器飘俊 蝶扼 鞘夸茄 扁瓷.
	
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::ApplyActiveCameraIfAny"));

	//for (int32 ACI = 0; ACI < ActiveCameraActors.Num(); ++ACI)
	//{
	//	if (ActiveCameraActors[ACI]->ManualTestBeginActiveCamera(BlendInTimeOverride) == true)
	//	{
	//		return true; // 窍唱 利侩登菌栏搁 歹 杭 鞘夸 绝澜.
	//	}
	//}

	//// 盔贰 咀萍宏等 墨皋扼 乐栏搁 弊成狄
	//if (CurrentActiveActiveCameraActor)
	//{
	//	return true;
	//}

	return false;
}

// 扁夯 咀萍宏墨皋扼 乐栏搁 弊芭 劝己拳
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
	//if (TheActiveSpawnPool) // 傈眉 SpawnPool 阑 iteration 且 鞘夸啊 绝绢具 茄促.
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

	// Pause menu widget 秦力
	//StopPauseMenuClass<>::GetInstance().Signal();

	return bSuperPauseCleared;
}

bool ABladeIIGameMode::BeginGlobalSceneBlur(float BlurScale)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::BeginGlobalSceneBlur"));
	//
	//if (bGlobalSceneBlurOn) {
	//	// Blur 登扁 傈狼 墨皋扼 汲沥阑 某教秦 初扁 锭巩俊 捞固 利侩登绢 乐绰 惑怕俊辑 滴锅 甸绢坷搁 帮鄂
	//	return false;
	//}
	//
	//CachedCameraAtBlurBegin = GetCurrentActiveCamComp(this);

	//if (CachedCameraAtBlurBegin)
	//{
	//	// GlobalSceneBlur 俊辑 汗备窍扁 困秦 汲沥甸 某教.
	//	CachedPPBeforeBlur = CachedCameraAtBlurBegin->PostProcessSettings;
	//	CachedPPWeightBeforeBlur = CachedCameraAtBlurBegin->PostProcessBlendWeight;

	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldMethod = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldMethod = EDepthOfFieldMethod::DOFM_Gaussian;
	//	
	//	// PC 滚傈狼 GaussianDOF 绰 Near/FarBlurSize 甫 炼例窍瘤父 Mobile 滚傈俊辑绰 Scale 蔼栏肺 炼例窃. 笛捞 傲 促抚
	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldScale = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldScale = BlurScale; // 辟单 绢掳 葛官老俊辑档 救冈洒绰 淀? 傲 沥秦柳 胶纳老捞 甸绢啊绰 芭 鞍促. 喉矾 目澄 荤捞令俊 蝶扼 己瓷捞 谅快登绰 侥捞搁 面盒洒 富捞 登摆瘤 偿偿
	//	
	//	// 溜, 咯扁 Near/FarBlurSize 技泼窍绰 扒 PC 俊辑 利寸洒 焊咯林扁 困窃.
	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldNearBlurSize = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldFarBlurSize = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldNearBlurSize = BlurScale * 5.0f;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldFarBlurSize = BlurScale * 5.0f;

	//	CachedCameraAtBlurBegin->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	//	CachedCameraAtBlurBegin->PostProcessSettings.DepthOfFieldFocalDistance = 100000.0f; // 捞扒 弊成 奴 荐肺 拎辑 葛滴 喉矾傅 登档废 窃.

	//	// 茄锅 FSceneView::OverridePostProcessSettings 甫 芭模 饶俊唱 角力 利侩捞 登绰 淀. Pause 皋春 鞍篮 版快扼搁 角力 利侩阑 茄 平 词苗具..

	//	bGlobalSceneBlurOn = true;

	//	return true;
	//}

	return false;
}

void ABladeIIGameMode::EndGlobalSceneBlur()
{
	if (bGlobalSceneBlurOn)
	{
		if (CachedCameraAtBlurBegin) // 绝促绊 秦辑 促矫 啊廉坷瘤 臼绰促. 
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

void ABladeIIGameMode::DeactivateWorldMovablePointLight() // 葛电 MovablePointLight 绰 酒聪绊 鞘荐利牢 芭 猾.. 措帆 楷免 侩 掺扁
{
	// 漂洒 楷免 傈俊 狼档窍瘤 臼霸 难廉 乐绰 movable point light 甫 波辑 葛官老 坊歹矾狼 movable point light 力茄阑 瘤虐扁 困茄 狼固肺 荤侩窍绰 巴.
	// 楷免俊辑绰 matinee 飘发捞唱 敲饭捞绢 某腐磐 light 甫 捞绢罐绰 侥栏肺 磊眉利牢 扼捞飘 技泼阑 窍霸 等促.
	for (TActorIterator<APointLight> It(GetWorld()); It; ++It)
	{
		APointLight* ThisPLActor = Cast<APointLight>(*It);
		UPointLightComponent* ThisPLComp = ThisPLActor ? Cast<UPointLightComponent>(ThisPLActor->GetLightComponent()) : nullptr;
		// 喊档肺 汲摹茄 PointLight 父 掺绰 巴捞骨肺 敲饭捞绢 某腐磐 赣府 困俊 嘿牢 扒 捞吧肺 波瘤瘤 臼绰促.
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
//	PerfStatCollector.OnEnd(this); // 措帆 霸烙 努府绢 矫痢捞 亮篮单 葛靛喊肺 炼陛究 崔扼辑.. OnEnd 绰 咯矾 镑俊辑 龋免
//#endif
//
//	SendDamageLog();
}

void ABladeIIGameMode::StartEventScene_MatchResult(bool HasLocalPlayerWon)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIGameMode::StartEventScene_MatchResult"));
	// 老瞒利栏肺, PVP 葛靛 搬苞 楷免捞 铰府 菩硅 惑包绝捞 铰磊甫 焊咯林绰 规侥捞 登绢 StartEventScene_GameClear 客 备盒捞 登菌澜.
//	for (AB2StageEventDirector* CurrSED : EventDirectors)
//	{
//		if (CurrSED){ // 氢饶 促弗 NetGameMode 搬苞 楷免档 付蛮啊瘤 风飘甫 呕 荐 乐阑瘤绰 葛福摆促.
//			CurrSED->NotifyMatchEnd(HasLocalPlayerWon);
//		}
//	}
//#if BII_STATS
//	PerfStatCollector.OnEnd(this); // 措帆 霸烙 努府绢 矫痢捞 亮篮单 葛靛喊肺 炼陛究 崔扼辑.. OnEnd 绰 咯矾 镑俊辑 龋免
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
	{ // 老馆利栏肺 Ambient sound 飘绰单 BGM 阑 肛免 捞蜡绰 绝摆瘤..
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
					StopAMB(); // 贸澜俊 肛眠瘤 臼疽促搁
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
	check(0); // 货肺款 亲格俊 措秦 case 备巩阑 力傍窍瘤 臼栏搁 check 俊 吧府霸 瞪 巴.
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
	//// 酒付档 葛氰甘 磊悼 胶农费狼 康氢牢瘤 啊父洒 乐绢档 肺弊啊 腹捞 唱客辑.. 咯变 力芭.
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

	//	// 力老 刚历 惯斑茄 墨皋扼 飘发阑 荤侩.. 捞吧肺 荤侩窍妨绰 墨皋扼 飘发阑 力老 困俊 硅摹秦具 茄促.
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
	//B2GMLoadingProgCollector::StepToMax(); // 措眉肺 SetupUIScene 流傈俊 肺爹官 菜 盲快搁 利例茄 淀
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
//	// 捞固 坷锹登乐栏搁 眉农いい
//	if (!pUI)
//	{
//		bool bNeedOpen = false;
//		EUIScene CurrentUIScene = UB2UIManager::GetInstance()->GetCurrUIScene();
//
//		// 眉农且锭 泅犁霸烙葛靛唱 UI纠捞 利钦窍搁 肺厚肺 泼辫UI 焊咯霖促.
//		// 沥魂捞尔 痢八捞尔 贸府啊 促福促.
//		if (CurrentModeState == b2network::B2ContentsModeState::SYSTEM_CHECK)
//		{
//			// 痢八 纳捞胶
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
//			// 沥魂 纳捞胶
//			// 1:1 评措傈 痢飞傈 闭凯父 沥魂乐澜.
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
//			//case b2network::B2ContentsMode::DIMENSION: // 闭凯篮 傈寸窍绊 包拌绝澜
//			{
//
//				if (auto* UIHallOfFame = UB2UIManager::GetInstance()->GetUI<UB2UIHallOfFame>(UIFName::HallOfFame))
//				{
//					UIHallOfFame->SetSettleState();
//					bNeedOpen = false; // 捞扒 公炼扒 救栋具 凳.
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
//	// 庆歹啊 粮犁 且 版快客 辨靛纠 力寇窍绊 葛滴 规价 啊瓷
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
//		if (HasWidget && ConditionShowMessage) // MainChatPage 啊 乐促搁 茄临盲泼阑 凯 鞘夸啊 绝促.
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
{ // 扁夯利栏肺 楷免 矫累 矫 TimeDilation 贸府甫 窍瘤父 TimeDilation 捞 乐绰 AnimNotifyState 殿苞 埃惯狼 瞒捞肺 牢秦 弊霸 公瓤拳瞪 锭俊 促矫 利侩阑 困窃.
	//for (AB2StageEventDirector* CurrSED : EventDirectors)
	//{
	//	if (CurrSED){
	//		CurrSED->ApplyStageEventTimeDilationIfPlaying();
	//	}
	//}
}

void ABladeIIGameMode::RestoreWholeGameTimeScale()
{
	//SetWholeGameTimeScale(1.0f); // 扁夯 1.0 捞绊 漂沥 汲沥栏肺 牢秦 促福霸 镜 荐 乐澜. SetGlobalTimeDilation 苞 喊档肺 冈塞.
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
//	//概 Tick付促 阂府绰 巴烙
//	// ABladeIIPlayer::SpawnOtherAsPuppet 鞍篮 吧肺 积己等 版快 update 俊 尝绢 甸绢啊急 救凳.
//	if (B2Player && B2Player->IsLocalPlayer())
//	{
//		UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
//		if (DocBattle)
//		{
//			DocBattle->SetCurrPCInCriticalCondition(B2Player->IsInCriticalCondition());
//			// TagPCHealth 绰 SkillRTManager 率俊辑..
//			if (B2Player->CurrentComboNum > 0) // 0 焊促 农促搁 攫力唱 墨款飘 窍绊 乐阑 巴烙
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
//				// QTE柳涝 啊瓷且锭父 焊咯林磊.
//				&& B2Player->IsQTEStartAble()
//				&& TargetActor->GetDamagedNum() == 0
//				// 炼扒眠啊. 鸥百各阜鸥模评(QTEEnableTeam) 汲沥 救登菌芭唱, 敲饭捞绢评捞尔 老摹且锭.
//				// 鸥百咀磐啊 齿敲饭捞绢 酒聪搁 鸥百各阜鸥模评(QTEEnableTeam) 汲沥捞 救登绢乐阑芭烙
//				&& (TargetActor->QTEEnableTeam == ETeamType::ETT_End || ETeamType(B2Player->GetTeamNum()) == TargetActor->QTEEnableTeam))
//			{//QTE 1鸥 柳涝 啊瓷 矫痢
//				if(TargetActor->QTEType == EQTEType::EQT_Mount)
//					DocBattle->SetQTEEnableState(EQTEUIAttackEnableTimingState::Mount);
//				else
//					DocBattle->SetQTEEnableState(EQTEUIAttackEnableTimingState::FirstAttack);
//			}
//			else if (B2Player->IsAlive() && TargetActor && TargetActor->IsAlive() && B2Player->IsTimingForSuccessiveQTE()
//				// QTE柳涝 啊瓷且锭父 焊咯林磊.
//				&& B2Player->IsQTEStartAble()
//				&& TargetActor->GetDamagedNum() == 0)
//			{// QTE 2鸥 柳涝 矫痢
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
//	//概 Tick付促 阂府绰 巴烙
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
//			{ // BossMob 包访 唱赣瘤 檬扁 荐摹甸篮 SpawnPool 俊辑 技泼
//				DocBattle->SetBossHPCurrent(mob->Health < 0 ? 0 : mob->Health);
//				if (mob->IsQTEBreakable()){
//					DocBattle->SetBossQTEBreakPointCurrent(mob->GetCurrentQTEBreakPoint());
//				}
//			}
//			// 府胶飘 吝 Boss 绰 窍唱父 乐绢具 茄促. 咯扁辑 持绊磊 窍绰 荐摹啊 窜老 UI 俊 康氢阑 固摹霸 登骨肺 父俊 窍唱 咯返 乐促绊 葛滴 持绢毫具 巩力父 凳.
//			break;
//		}
//	}
}

void ABladeIIGameMode::ShowMissionDialog()
{
//	//譬配府倔 柳青 吝俊绰 促捞倔肺弊 芒捞 哆瘤 臼酒具 茄促.
//	if (TutorialManager::GetInstance().HasAnyTutorial())
//		return;
//
//	UB2UIManager* UIManager = UB2UIManager::GetInstance();
//	if (UIManager != nullptr)
//	{
//		// Dialog 剁况林扁
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
//	//单固瘤 箭磊 棵扼啊绰 巴
//	CharacterTakeDamageEvent.Broadcast();
//	CharacterTakeDamageEvent.Clear();
//
//	//磷绰 巴(OnDeath俊辑 Undeath惑怕甫 函版窍扁 锭巩俊 盖 酒贰 乐绢具 窃)
//	CharacterDeathEvent.Broadcast();
//	CharacterDeathEvent.Clear();//[fixme][@AKI, 170815]困氰秦 焊捞绰单...Timer甫 吧绢辑 力芭 且鳖??
}

float ABladeIIGameMode::GetTotalAttack(ICharacterDataStore* CharacterDataStore)
{
	const auto	MainPCClass = CharacterDataStore->GetMainPlayerClass();
	const auto	TagPCClass = CharacterDataStore->GetSubPlayerClass();

	float		MainPCAttack = CombatStatEval::GetPCAttack(MainPCClass, CharacterDataStore);
	float		MainModSpecializeAttack = CombatStatEval::GetPCAttackByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);
	float		MainTotalAttack = MainPCAttack + MainModSpecializeAttack;
	MainTotalAttack = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(MainPCClass))), MainTotalAttack);

	float		TagPCAttack = CombatStatEval::GetPCAttack(TagPCClass, CharacterDataStore);
	float		TagModSpecializeAttack = CombatStatEval::GetPCAttackByMod(TagPCClass, GetB2GameModeType(), CharacterDataStore);
	float		TagTotalAttack = TagPCAttack + TagModSpecializeAttack;
	TagTotalAttack = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseAttack(CharacterDataStore->GetCharacterLevel(TagPCClass))), TagTotalAttack);

	float		TotalAttack = MainTotalAttack + TagTotalAttack;

	return TotalAttack;
}

float ABladeIIGameMode::GetTotalDefense(ICharacterDataStore* CharacterDataStore)
{
	const auto	MainPCClass = CharacterDataStore->GetMainPlayerClass();
	const auto	TagPCClass = CharacterDataStore->GetSubPlayerClass();

	float		MainPCDefense = CombatStatEval::GetPCDefense(MainPCClass, CharacterDataStore);
	float		MainModSpecializeDefense = CombatStatEval::GetPCDefenseByMod(MainPCClass, GetB2GameModeType(), CharacterDataStore);
	float		MainTotalDefense = MainPCDefense + MainModSpecializeDefense;
	MainTotalDefense = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(MainPCClass))), MainTotalDefense);

	float		TagPCDefense = CombatStatEval::GetPCDefense(TagPCClass, CharacterDataStore);
	float		TagModSpecializeDefense = CombatStatEval::GetPCDefenseByMod(TagPCClass, GetB2GameModeType(), CharacterDataStore);
	float		TagTotalDefense = TagPCDefense + TagModSpecializeDefense;
	TagTotalDefense = FMath::Max(static_cast<float>(CombatStatEval::GetPCBaseDefense(CharacterDataStore->GetCharacterLevel(TagPCClass))), TagTotalDefense);

	float		TotalDefense = MainTotalDefense + TagTotalDefense;

	return TotalDefense;
}

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
{ // DefaultPCClassEnum 阑 EPC_End 肺 滴绊 BP 努贰胶甫 抛胶飘 侩档肺 spawn 窍妨绰 狼档. 沥侥 饭骇俊绰 捞犯霸 窍搁 救凳. 酒抗 救冈鳃 巴.
	return (Cast<UBlueprintGeneratedClass>(*DefaultPawnClass) != NULL);
}
#endif
