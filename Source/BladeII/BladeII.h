// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//#include "EngineMinimal.h"
//#include "ModuleManager.h"
//[TO-DO] [@AKI, 170817] BladeII.h가 PCH로 의심 되는 상황이라 이 아래 해더와 관련 된 것들은 새로 만드는게 좋을듯.
//#include "B2ClientDataStore.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBladeII, Log, All);

//#define NONE_KAKAO_SDK_VERSION
//>> [BII_GAME] AKI
//>> DO NOT CHANGE
#ifndef B2_NATION_KOREA
#define B2_NATION_KOREA				1
#endif
#ifndef B2_NATION_TESTLANDING
#define B2_NATION_TESTLANDING		2	//ActionSquare Softlanding
#endif
#ifndef B2_NATION_SOFTLANDING
#define B2_NATION_SOFTLANDING		3	//KakaoGames Softlanding
#endif
#ifndef B2_NATION_GRANDLANDING
#define B2_NATION_GRANDLANDING		4	//KakaoGames GrandLading(정식 출시)
#endif
#ifndef B2_NATION_CHINA
#define B2_NATION_CHINA				5
#endif
#ifndef B2_NATION_JAPAN
#define B2_NATION_JAPAN				6
#endif
#ifndef B2_NATION_TAIWAN
#define B2_NATION_TAIWAN			7
#endif
#ifndef B2_NATION_THAILAND
#define B2_NATION_THAILAND			8
#endif
//<< DO NOT CHANGE


#ifndef B2_NATION_CODE
#define B2_NATION_CODE					B2_NATION_KOREA
//	#define B2_NATION_CODE					B2_NATION_TESTLANDING
//	#define B2_NATION_CODE					B2_NATION_SOFTLANDING
//	#define B2_NATION_CODE					B2_NATION_GRANDLANDING
//	#define B2_NATION_CODE					B2_NATION_CHINA
//	#define B2_NATION_CODE					B2_NATION_JAPAN
//	#define B2_NATION_CODE					B2_NATION_TAIWAN
//	#define B2_NATION_CODE					B2_NATION_THAILAND
#endif
//<< [BII_GAME] AKI

/** 
 * You might want to do something like this below to turn BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 on, instead of editing Build.h.
 * but you need to search for BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 in engine code, to actually enable some features.
 * Not really sure if it works as intended like this.. 
 */

#define B2_BUILD_LEVEL 0
#define B2_BUILD_LEVEL_LIVE 0
#define B2_BUILD_LEVEL_QA 0

#ifdef BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
#undef BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1

#if B2_BUILD_LEVEL == B2_BUILD_LEVEL_LIVE
#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 0
#else
#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 1
#endif
#endif

#ifdef BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 
#undef BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 
#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 1
#else
#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 1
#endif


/**
 * More development features for shipping test.
 * It must be with BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
 */
#if B2_BUILD_LEVEL == B2_BUILD_LEVEL_LIVE
	#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 0
#else
	#if UE_BUILD_SHIPPING
		#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 0
	#else
		#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
			#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 1
		#else
			#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 0
		#endif
	#endif
#endif

/**  
 * On-screen debug text feature (DrawDebugText, DrawTimedDebugText..) sometimes needed for shipping build
 * It can be a substitution of system log.
 */
#define WITH_BII_ON_SCREEN_DEBUG_TEXT BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2

/**
 * For BladeII own stat tracking, might or might not based on Unreal stat functionality.
 */
#define BII_STATS BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2

/** 
 * Temporary code to clean up some unintentionally saved reference due-to foolish mishandling of TAsset lazy-loading 
 */
#define TEMP_LAZYLOADING_MISHANDLING_CLEANUP WITH_EDITOR

/**
 * Enable read/write of event cut-scene (of StageEventDirector) played state to an .ini file.
 */
#define ENABLE_LOCAL_CUTSCENE_SAVED_STATE 0

/**
 * Testing version to send outside.
 */
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
#define BUILD_STANDALONE_TEST_APK 0 // Set this to 1 if you need standalone test build, that touching at title screen doesn't connect you to the server.
#else
#define BUILD_STANDALONE_TEST_APK 0
#endif

/** BII_CHECK is essentially for mobile debugging, where call-stacks are usually unavailable without check trace.
 * You may put it on various place without worrying about development editor crash.
 * In any other place that assert is desried for development editor too, use check() instead. */
#if WITH_EDITOR
#define BII_CHECK(expr) checkSlow((expr))
#else
#define BII_CHECK(expr) check((expr)) 
#endif

#define RemoveLevelSuffix TEXT("_BGRemove");

#if UE_BUILD_SHIPPING
#define DEBUG_LOG 0
#define B2_PROFILE_LEVEL 0
#else
#define DEBUG_LOG 0
#define B2_PROFILE_LEVEL 0//1
#endif //!UE_BUILD_SHIPPING

#if WITH_EDITOR
// For some more sophisticated progress handling.
typedef void(*SetB2ContentLoadingProgressSplashFnPtr)(float);
#endif

#define GLOBALRELICMANAGER BladeIIGameImpl::GetRelicManager()
#define GLOBALUNITYSKILLMANAGER BladeIIGameImpl::GetUnitySkillMissionManager()

#define FORCE_LOGGING_IN_ANDROID 0

#define TUTORIAL

extern bool GMinimalDLCFrontMode;

