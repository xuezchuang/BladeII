#include "BladeII.h"
#include "Global.h"
#include "BladeIIScalabilityControl.h"
#include "BladeIIGameMode.h"
#include "B2LobbyGameMode.h"
#include "B2StageGameModeBase.h"
#include "B2TutorialGameMode.h"
#include "B2CounterAttackGameMode.h"
#include "B2PVPGameMode.h"
#include "B2RaidGameMode.h"
#include "B2ControlGameMode.h"
#include "B2TMGameMode.h"
#include "B2GuildGameMode.h"
#include "B2PreRenderGameMode.h"
#include "B2DLCFrontGameMode.h"
#include "BladeIIPlayer.h"
#include "B2LobbySkeletalMeshActorBase.h"
#include "DeviceProfiles/DeviceProfileManager.h"

#include "B2GuildGameMode.h"

/************************************************************************/
/* 
게임 설정에 따라 언리얼 Scalability 시스템 컨트롤 하는 관련 유틸들 모아놓음..
*/
/************************************************************************/
//
/** 
 * EB2ResolutionLevel 에 맵핑될 EB2GraphicsRQType 을 연결하는 역할. DeviceProfile 을 통해 플랫폼 및 기기별로 설정 제어.
 * r.DefinedMobileFrameLimitQuality_H/M/L 과 마찬가지 식임. 
 * 값 자체는 int 캐스팅 된 EB2GraphicsRQType 에 대응됨.
 * EB2GraphicsRQType 에 추가되는 것이 있을 시 여기도 점검.
 */
static TAutoConsoleVariable<int32> CVarAllowedGameRQType_H(
	TEXT("r.AllowedGameRQType_H"),
#if PLATFORM_WINDOWS
	7, /* PC 기본값은 기능 프리뷰 목적으로 적당한 걸 열어둠. 아님 [Windows DeviceProfile] 에 넣어둬도 되긴 함. */
#else
	2,
#endif
	TEXT("Specifies which type of Resolution-Quality technique is allowed for BladeII game ResolutionLevel High:\n"),
	ECVF_ReadOnly /* ReadOnly 로서 DeviceProfile 을 통해 세팅하도록.. */
);
static TAutoConsoleVariable<int32> CVarAllowedGameRQType_M(
	TEXT("r.AllowedGameRQType_M"),
	2,
	TEXT("Specifies which type of Resolution-Quality technique is allowed for BladeII game ResolutionLevel Mid:\n"),
	ECVF_ReadOnly /* ReadOnly 로서 DeviceProfile 을 통해 세팅하도록.. */
);
static TAutoConsoleVariable<int32> CVarAllowedGameRQType_L(
	TEXT("r.AllowedGameRQType_L"),
	1,
	TEXT("Specifies which type of Resolution-Quality technique is allowed for BladeII game ResolutionLevel Low:\n"),
	ECVF_ReadOnly /* ReadOnly 로서 DeviceProfile 을 통해 세팅하도록.. */
);

/** 값 자체는 EB2GraphicsRQType 이고 DeviceProfile 에 의해 세팅. 저장된 B2ResolutionLevel 값에 따라 맵핑된 RQType 이 적용되기 전까지 적용. */
static TAutoConsoleVariable<int32> CVarDefaultB2GraphicsRQType(
	TEXT("r.DefaultB2GraphicsRQType"),
	2,
	TEXT("The default value of B2GraphicsRQType before user selection is loaded and applied. Supposed to be set by DeviceProfile."),
	ECVF_ReadOnly /* ReadOnly 로서 DeviceProfile 을 통해 세팅하도록.. */
);

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
// AllowedGameRQType 을 무시하는 테스트 커맨드 용.
static TAutoConsoleVariable<int32> CVarB2DevForcedGraphicsRQType(
	TEXT("r.DevForcedGraphicsRQType"),
	-1,
	TEXT("Provided only for testing. In zero or positive value, it will take effect as casted EB2GraphicsRQType. It ignores AllowedGameRQType."),
	ECVF_Default
);
#endif

/** DeviceProfile 을 통한 성능 옵션 기본값 변경 시 업데이트 배포 후 유저 저장된 값을 한번 리셋하기 위한 기능. */ 
static TAutoConsoleVariable<int32> CVarResetScalabilityChance(
	TEXT("r.ScalabilityResetChance"),
	0, // 이걸 1로 세팅하는 건 오래 지속할 필요는 없다. 한두번의 업데이트 텀 정도에서만?
	TEXT("It allows all saved BladeII scalability settings to default desired setting, only once by local ini state.\n"),
	ECVF_ReadOnly /* ReadOnly 로서 DeviceProfile 을 통해 세팅하도록.. */
);
bool HasScalabilityResetChance()
{	
	if (CVarResetScalabilityChance.GetValueOnGameThread() != 0)
	{
		if (GConfig)
		{
			bool bChanceUsed = false;
			GConfig->GetBool(TEXT("GameSetting"), TEXT("ScalabilityResetChanceUsed"), bChanceUsed, GB2GeneralSavedStateIni);
			return !bChanceUsed;
		}
	}
	return false;
}
void MarkScalabilityResetChanceUse()
{
	if (GConfig)
	{
		GConfig->SetBool(TEXT("GameSetting"), TEXT("ScalabilityResetChanceUsed"), true, GB2GeneralSavedStateIni);
	}
}

EB2GraphicsRQType GetDefaultGraphicsRQType()
{ // 단순히 r.DefaultB2GraphicsRQType 변수의 EB2GraphicsRQType enum 맵핑
	return static_cast<EB2GraphicsRQType>(CVarDefaultB2GraphicsRQType.GetValueOnAnyThread());
}

FORCEINLINE IConsoleVariable* GetMCSFCVar() { return IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor")); }

namespace B2Scalability
{
#if !UE_BUILD_SHIPPING
/** BladeII 게임모드나 연출 등 게임 상황에 따른 성능 옵션 bias 가하는 기능 On/Off 
 * 이 변수가 빠진 기본 상태에서는 true 취급인 거. */
bool bUseBladeIIScalabilityOverride = true; 
bool bOnModuleStartupCalled = false;
#endif

/** DeviceProfile 을 통해 지정된 실행 초기 MobileContentScaleFactor. */
float CachedBaseMobileContentScaleFactor = 1.0f;

/** 앱 시작 시점의 RQType 을 저장해 놓는다. 특정 방식 간의 switching 은 이번 실행 기간 동안 막고 다음에 적용하기 위해. */
EB2GraphicsRQType InitialRQTypeOfAppStart = EB2GraphicsRQType::End;

void OnModuleStartup()
{
//#if !UE_BUILD_SHIPPING 
//	if (GConfig)
//	{
//		// 테스트 목적 등의 ini 옵션들.. 필요할 때가 있음.
//		GConfig->GetBool(TEXT("B2Scalability"), TEXT("UseBladeIIScalabilityOverride"), bUseBladeIIScalabilityOverride, GScalabilityIni);
//	}
//#endif
//
//	static IConsoleVariable* CVarMCSF = GetMCSFCVar();
//	check(CVarMCSF);
//	if (CVarMCSF)
//	{ // 이 시점에서 가져오는 값은 그냥 초기값이거나 DeviceProfile 을 통해 세팅된 값이어야.
//		CachedBaseMobileContentScaleFactor = CVarMCSF->GetFloat();
//		check(CachedBaseMobileContentScaleFactor > 0.0f);
//		UE_LOG(LogBladeII, Log, TEXT("Cached MobileContentScaleFactor at OnModuleStartup %.2f"), CachedBaseMobileContentScaleFactor);
//	}
//
//	check(GetDefaultGraphicsRQType() != EB2GraphicsRQType::End);
//
//#if !UE_BUILD_SHIPPING 
//	check(
//		CVarDefaultB2GraphicsRQType.GetValueOnGameThread() == CVarAllowedGameRQType_H.GetValueOnGameThread() ||
//		CVarDefaultB2GraphicsRQType.GetValueOnGameThread() == CVarAllowedGameRQType_M.GetValueOnGameThread() ||
//		CVarDefaultB2GraphicsRQType.GetValueOnGameThread() == CVarAllowedGameRQType_L.GetValueOnGameThread()
//	);
//	check(
//		CVarAllowedGameRQType_H.GetValueOnGameThread() >= CVarAllowedGameRQType_M.GetValueOnGameThread() &&
//		CVarAllowedGameRQType_M.GetValueOnGameThread() >= CVarAllowedGameRQType_L.GetValueOnGameThread() &&
//		/* H == M, M == L 일수는 있지만 셋 다 모두 같을 수는 없다. H 는 L 보다는 높아야. */
//		CVarAllowedGameRQType_H.GetValueOnGameThread() > CVarAllowedGameRQType_L.GetValueOnGameThread()
//	);
//#if PLATFORM_IOS
//	// iOS 에선 MobileContentScaleFactor 건드리지 않는 게 좋을 것 같다.
//	check(
//		!IsRQTypeOneOfExtraContentScale(static_cast<EB2GraphicsRQType>(CVarAllowedGameRQType_H.GetValueOnGameThread())) &&
//		!IsRQTypeOneOfExtraContentScale(static_cast<EB2GraphicsRQType>(CVarAllowedGameRQType_M.GetValueOnGameThread())) &&
//		!IsRQTypeOneOfExtraContentScale(static_cast<EB2GraphicsRQType>(CVarAllowedGameRQType_L.GetValueOnGameThread()))
//	);
//#endif
//#endif
//
//	// 일부 RQType 간의 런타임 switching 을 앱 재시작 후로 미루기 위해 처음 값을 저장해 둠.
//	{
//		int32 LoadedResolutionLevelInt = B2ResolutionLevelToInt(EB2ResolutionLevel::Mid);
//		LoadGameSetting_Resolution_OrByDefault(LoadedResolutionLevelInt);
//		const EB2ResolutionLevel ReservedResLevel = LoadReservedResolutionLevelOfPrevAppRun(); // 이전에 예약된 해상도 변경이 있었다면 아직 적용되지 않은 상태일 것이므로 이것도 검사.
//		const EB2ResolutionLevel InitialResolutionLevel = (ReservedResLevel != EB2ResolutionLevel::End) ? ReservedResLevel : IntToB2ResolutionLevel(LoadedResolutionLevelInt);
//		InitialRQTypeOfAppStart = GetSafeMappedRQTypeOfResolutionLevel(InitialResolutionLevel);
//		check(InitialRQTypeOfAppStart != EB2GraphicsRQType::End);
//
//		UE_LOG(LogBladeII, Log, TEXT("InitialResolutionLevel %d, InitialRQType %d"), B2ResolutionLevelToInt(InitialResolutionLevel), (int32)InitialRQTypeOfAppStart);
//	}
//
//#if !UE_BUILD_SHIPPING
//	bOnModuleStartupCalled = true;
//#endif
}

#if PLATFORM_ANDROID
void AndroidMCSFUpdateHack(bool bForceInvalidateResCache )
{
	//// MobileContentScaleFactor 조절이 뭔가 잘 안먹혀서. (B2CLT-3374) 문제 정식 해결 이전의 임시 땜질. 
	//bool bHasContentScaleRQType = false;
	//for (int32 RLI = 0; RLI < B2ResolutionLevelToInt(EB2ResolutionLevel::End); ++RLI)
	//{
	//	EB2GraphicsRQType ThisMappedRQType = GetSafeMappedRQTypeOfResolutionLevel(IntToB2ResolutionLevel(RLI));
	//	if (IsRQTypeOneOfExtraContentScale(ThisMappedRQType))
	//	{
	//		bHasContentScaleRQType = true; // MCSF 를 조절하는 설정이 있는 경우. 거의 안드로이드 에뮬레이터용일 것.
	//		break;
	//	}
	//}
	//if (bHasContentScaleRQType)
	//{
	//	static IConsoleVariable* CVarMCSF = GetMCSFCVar();
	//	if (CVarMCSF)
	//	{
	//		const float WantedFactorValue = CVarMCSF->GetFloat();

	//		// 좀 황당한 코드로 보이는데 어디 업데이트가 안되는 값이 있는지 DeviceProfile 에 처음 세팅한 값하고 다르면 로딩 화면 후에 화면 크기가 안맞는 일이 발생해서..
	//		// 내부 SceneRenderTarget 등의 크기가 맨 처음 지정된 값으로 돌아가게 되는 경우가 있다.

	//		if (bForceInvalidateResCache)
	//		{ 
	//			// 그냥 임의로 MCSF 값을 다르게 주어서 내부 해상도 스위칭에 따른 렌더타겟 등의 업데이트가 확실히 되도록 하기 위함.
	//			// FAndroidWindow::GetScreenRect 참고. MCSF 를 포함해서 실제 사이즈 데이터가 유지되는 상황에서는 중간에 그냥 리턴을 해 버림. 
	//			CVarMCSF->Set(WantedFactorValue * 0.5f);
	//			FlushRenderingCommands();
	//		}
	//		
	//		CVarMCSF->Set(WantedFactorValue);
	//		FlushRenderingCommands();
	//	}
	//}
}
#endif

void OnGameModeStartPlay(ABladeIIGameMode* StartingGM)
{
	if (HasScalabilityResetChance())
	{
		UE_LOG(LogBladeII, Log, TEXT("User saved scalability settings are being reset.."));

		ResetAllScalabilitySettings();
		MarkScalabilityResetChanceUse();
		GConfig->Flush(false, GB2GeneralSavedStateIni);
	}

	if (StartingGM)
	{
		// 현재 기록된 옵션 선택값에 기반하여 게임모드 자체적으로 특수한 추가 옵션 설정..
		StartingGM->ApplyGameModeSpecificScalabilitySetting();
	}
#if PLATFORM_ANDROID
	//AndroidMCSFUpdateHack(); 여긴 이제 안 해도 될 것 같고..
#endif
}
void OnGameModeLoadingScreenEnd()
{
#if PLATFORM_ANDROID
	AndroidMCSFUpdateHack(true);
#endif
}

FString GetScalabilityKeyNameOfGraphicsLevel(EB2GraphicsLevel InGraphicsLevel)
{
	return (InGraphicsLevel == EB2GraphicsLevel::GraphicsLevel_HIGH) ? TEXT("High") :
		(InGraphicsLevel == EB2GraphicsLevel::GraphicsLevel_MID ? TEXT("Mid") : TEXT("Low"));
}

/************************************************************************/
/* EB2GraphicsLevel 과 Scalability 레벨간의 맵핑은 
EB2FrameLimitLevel 이나 EB2ResolutionLevel 에 비해 사정이 보다 복잡하다.*/
/************************************************************************/
int32 GetScalabilityLevelOfGraphicsLevel(EB2GraphicsLevel InGraphicsLevel)
{
	// UI 선택에 따른 Scalability 시스템 전체적인 조절 레벨 값.. 별도로 설정이 있는 특수 항목 외에는 여기서 나온 값을 쓴다.
	int32 RetVal = 0;
	GConfig->GetInt(TEXT("ScalabilityLevelByUserSelection"), *GetScalabilityKeyNameOfGraphicsLevel(InGraphicsLevel), RetVal, GScalabilityIni);
	return RetVal;
}

int32 GetTextureLevelOfGraphicsLevel(EB2GraphicsLevel InGraphicsLevel)
{
	// UI 선택에 따른 TextureQuality 조절 레벨 값.. 별도로 설정이 있는 특수 항목.. 텍스쳐랑 해상도는 보통 별도 선택이 가능한 경우가 많아서 따로 떼어낼 필요가 있다.
	int32 RetVal = 0;
	GConfig->GetInt(TEXT("TextureQualityLevelByUserSelection"), *GetScalabilityKeyNameOfGraphicsLevel(InGraphicsLevel), RetVal, GScalabilityIni);
	return RetVal;
}

float GetResolutionScaleOfGraphicsLevel(EB2GraphicsLevel InGraphicsLevel)
{
	// UI 선택에 따른 ResolutionQuality 조절 레벨 값.. 별도로 설정이 있는 특수 항목.. 텍스쳐랑 해상도는 보통 별도 선택이 가능한 경우가 많아서 따로 떼어낼 필요가 있다.
	float RetVal = 100.0;
	GConfig->GetFloat(TEXT("ResolutionQualityByUserSelection"), *GetScalabilityKeyNameOfGraphicsLevel(InGraphicsLevel), RetVal, GScalabilityIni);
	return RetVal;
}
int32 GetGameplayOnlyLODQualityOfGraphicsLevel(EB2GraphicsLevel InGraphicsLevel)
{
	// UI 선택에 따른 Scalability 시스템 전체적인 조절 레벨 값.. 별도로 설정이 있는 특수 항목..
	int32 RetVal = 0;
	GConfig->GetInt(TEXT("GameplayOnlyLODQualityByUserSelection"), *GetScalabilityKeyNameOfGraphicsLevel(InGraphicsLevel), RetVal, GScalabilityIni);
	return RetVal;
}

/**
 * GetScalabilityLevelOfGraphicsLevel 이랑 반대격인데, 현재 들어가 있는 Scalability 값들에 기반해서 대체로 매칭되는 EB2GraphicsLevel 을 리턴
 * 사용자 선택이 아직 없는 상황에서 DeviceProfile 로 들어간 Scalability 에 기반해서 사용자 선택 GraphicsLevel 을 산정하려는 것.
 */
EB2GraphicsLevel GetGraphicsLevelByTotalScalability(const Scalability::FQualityLevels& InScalabilityLevel)
{
	//int32 TotalAdded =
	//	InScalabilityLevel.AntiAliasingQuality +
	//	InScalabilityLevel.EffectsQuality +
	//	InScalabilityLevel.PostProcessQuality +
	//	InScalabilityLevel.PostProcessExtraQuality +
	//	//InScalabilityLevel.ResolutionQuality + 이건 수치가 좀 다르니 빼야..
	//	InScalabilityLevel.TextureQuality +
	//	InScalabilityLevel.ShadowQuality +
	//	InScalabilityLevel.ViewDistanceQuality +
	//	InScalabilityLevel.GameplayOnlyLODQuality;
	//// 혹여나 추가될 Scalability 항목에 대한 처리가 필요하긴 한데 그렇게 정교하게까지 할 필요는 없을 듯.
	//// 이건 int 캐스팅보다는 Round 가 적절할 듯 하다. 가장 가까운 값을 찾는 게 좋다.
	//int32 AvergageScalabilityValue = FMath::RoundToInt((float)TotalAdded / 8.0f);
	//
	//FString FoundString;
	//GConfig->GetString(TEXT("AssumedUserSelectionByScalabilityLevel"), *FString::Printf(TEXT("%d"), AvergageScalabilityValue), FoundString, GScalabilityIni);

	//UE_LOG(LogBladeII, Log, TEXT("GetGraphicsLevelByTotalScalability. TotalScaleAdded %d, AvgScaleValue %d, MatchingString %s"), TotalAdded, AvergageScalabilityValue, *FoundString);

	//if (FoundString == TEXT("High"))
	//{
	//	return EB2GraphicsLevel::GraphicsLevel_HIGH;
	//}
	//else if (FoundString == TEXT("Mid"))
	//{
	//	return EB2GraphicsLevel::GraphicsLevel_MID;
	//}
	//else if (FoundString == TEXT("Low")) 
	//{
	//	return EB2GraphicsLevel::GraphicsLevel_LOW;
	//}
	//
	//BII_SCREEN_LOG(FString::Printf(TEXT("Could not find matching GraphicsLevel string %s by scalability QualityLevel %d"), *FoundString, AvergageScalabilityValue),
	//	FLinearColor::Green, 12, 10.0f);

	return EB2GraphicsLevel::GraphicsLevel_HIGH;
}

EB2GraphicsLevel GetGraphicsLevelByExpectedScalability()
{
	// 엔진 시동 시 하는 기기 감지 및 이에 따른 세팅..
	// 이런 처리 없이 단순히 Scalability::GetQualityLevels 를 가져오면 GameMode 에 따른 override 가 적용된 걸 사용하게 될 수 있으므로 적절치 않다.
	UDeviceProfileManager::InitializeCVarsForActiveDeviceProfile();

	return GetGraphicsLevelByTotalScalability(Scalability::GetQualityLevels());
}

/** GameplayOnlyLODQuality 변경 직후 바로 적용되려면 추가 핸들링이 좀 필요해서.
* 엔진 Scalability 쪽에서 어떻게 해 보려니 어차피 World 가 필요해서 그냥 여길 통해 변경하는 것에 대해서만 직접 콜한다.
* sg. 콘솔 명령을 통한 변경만 바로 핸들링이 안될 뿐 정식 메뉴를 통한 변경은 바로 핸들링 될 것. */
void OnGameplayOnlyLODQualityChanged(UWorld* InWorld, int32 InNewValue)
{
	if (!InWorld) {
		return;
	}

	for (FActorIterator ItPlayer(InWorld); ItPlayer; ++ItPlayer)
	{
		ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(*ItPlayer);
		if (CastedPlayer)
		{ // 혹여나 영상 플레이 도중이라 하더라도.. 연출 도중에는 대부분 BladeIIPlayer 가 아닌 SkeletalMeshActor 가 사용되므로 이것을 호출하는 게 영향을 미치지 않는다. 아닌 경우라 하더라도 연출 도중에 pause 메뉴가 나오지도 않고..
			CastedPlayer->SetupLODInfoForInGame();
		}
		else
		{
			AB2LobbySkeletalMeshActorBase* LobbySKActor = Cast<AB2LobbySkeletalMeshActorBase>(*ItPlayer);
			if (LobbySKActor)
			{
				ABladeIIPlayer::SetupLODInfoForLobbyRepresentative(LobbySKActor);
			}
		}
	}
}


float GetScreenScaleRQTypeScaleValue(EB2GraphicsRQType InRQType)
{
	// Extra/LowerScreenScale~~ RQType 에 맵핑된 ini 에 정의된 실제 수치 얻어오기.
	check(IsRQTypeOneOfExtraScreenScale(InRQType) || IsRQTypeOneOfLowerScreenScale(InRQType));
	float RetScaleValue = 1.0f;
	FString IniItemString;
	switch (InRQType)
	{
	case EB2GraphicsRQType::LowerScreenScale_1:
		IniItemString = TEXT("LowerScreenScale_1");
		break;
	case EB2GraphicsRQType::LowerScreenScale_2:
		IniItemString = TEXT("LowerScreenScale_2");
		break;
	case EB2GraphicsRQType::ExtraScreenScale_1:
		IniItemString += TEXT("ExtraScreenScale_1");
		break;
	case EB2GraphicsRQType::ExtraScreenScale_2:
		IniItemString += TEXT("ExtraScreenScale_2");
		break;
	case EB2GraphicsRQType::ExtraScreenScale_3:
		IniItemString += TEXT("ExtraScreenScale_3");
		break;
	}
	GConfig->GetFloat(TEXT("ResolutionQualityByUserSelection"), *IniItemString, RetScaleValue, GScalabilityIni);
#if !UE_BUILD_SHIPPING
	if (IsRQTypeOneOfExtraScreenScale(InRQType))
	{
		check(RetScaleValue >= 1.0f); // 이게 아니면 의도와 다르게 설정을 한 거.
	}
	else
	{
		check(RetScaleValue > 0.0f && RetScaleValue < 1.0f);
	}
#endif
	return RetScaleValue;
}
void AdjustResolutionQualityByRQType(EB2GraphicsRQType InRQType, float& InOutResolutionQuality)
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	// 테스트 용 CVar 가 활성화 된 상황.
	if (CVarB2DevForcedGraphicsRQType.GetValueOnGameThread() >= 0) {
		InRQType = static_cast<EB2GraphicsRQType>(CVarB2DevForcedGraphicsRQType.GetValueOnGameThread());
	}
#endif

	// EB2GraphicsRQType::ExtraScreenScale 의 동작 정의
	if (IsRQTypeOneOfLowerScreenScale(InRQType) || IsRQTypeOneOfExtraScreenScale(InRQType))
	{
		float ScaleValue = GetScreenScaleRQTypeScaleValue(InRQType);
		InOutResolutionQuality *= ScaleValue;
	}
}

float GetContentScaleRQTypeScaleValue(EB2GraphicsRQType InRQType)
{ // ExtraContentScale~~ RQType 에 맵핑된 ini 에 정의된 실제 수치 얻어오기.
	check(IsRQTypeOneOfExtraContentScale(InRQType));

	float RetScaleValue = 1.0f;
	FString IniItemString;
	switch (InRQType)
	{
	case EB2GraphicsRQType::ExtraContentScale_1:
		IniItemString += TEXT("ExtraContentScale_1");
		break;
	case EB2GraphicsRQType::ExtraContentScale_2:
		IniItemString += TEXT("ExtraContentScale_2");
		break;
	}

	GConfig->GetFloat(TEXT("ResolutionQualityByUserSelection"), *IniItemString, RetScaleValue, GScalabilityIni);
	check(RetScaleValue > 1.0f);
	return RetScaleValue;
}
void AdjustMobileContentScaleFactorByRQType(EB2GraphicsRQType InRQType)
{
//	static IConsoleVariable* CVarMCSF = GetMCSFCVar();
//	check(CVarMCSF);
//	if (CVarMCSF)
//	{
//		const float CurrValue = CVarMCSF->GetFloat();
//		float ScaleValue = 1.0f;
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//		// 테스트 용 CVar 가 활성화 된 상황.
//		if (CVarB2DevForcedGraphicsRQType.GetValueOnGameThread() >= 0) {
//			InRQType = static_cast<EB2GraphicsRQType>(CVarB2DevForcedGraphicsRQType.GetValueOnGameThread());
//		}
//#endif
//
//		if (IsRQTypeOneOfExtraContentScale(InRQType))
//		{
//			ScaleValue = GetContentScaleRQTypeScaleValue(InRQType);
//		}
//
//		// Ini 설정값을 그냥 적용하는 게 아니라 엔진 초기값 혹은 DeviceProfile 을 통해 정의된 값에 추가 스케일을 가한다.
//		const float WantedValue = CachedBaseMobileContentScaleFactor * ScaleValue;
//		// 값 바뀌었을 때만 Set 하려다 보니 뭔가 썩 원하는 대로 동작하지 않는 것 같아 실제 MCSF 를 조절하는 RQType 인 경우는 항상
//		if (std::abs(WantedValue - CurrValue) > KINDA_SMALL_NUMBER || IsRQTypeOneOfExtraContentScale(InRQType))
//		{
//			UE_LOG(LogBladeII, Log, TEXT("Setting MobileContentScaleFactor to %.2f by B2GraphicsRQType %d"), WantedValue, (int32)InRQType);
//			CVarMCSF->Set(WantedValue);
//			FlushRenderingCommands();
//		}
//	}
}

void SetAntiAliasingByB2RQType(EB2GraphicsRQType InRQType)
{
	// 기본 Scalability 시스템의 AntiAliasingQuality 를 아예 무시하는 건 아니지만 별도 컨트롤이 좀 필요..

	static IConsoleVariable* _CVarMobileMSAA = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileMSAA"));
	check(_CVarMobileMSAA);

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	// 테스트 용 CVar 가 활성화 된 상황.
	if (CVarB2DevForcedGraphicsRQType.GetValueOnGameThread() >= 0) {
		InRQType = static_cast<EB2GraphicsRQType>(CVarB2DevForcedGraphicsRQType.GetValueOnGameThread());
	}
#endif

	//
	// r.MobileMSAA 를 임의로 변경해서 테스트를 하고 싶다면 이쪽 코드를 (임시로) 제거하든지, 아니면 여기에 추가 컨트롤을 넣던지.
	//

	if (IsRQTypeOneOfMSAA(InRQType))
	{
		if (_CVarMobileMSAA)
		{
			_CVarMobileMSAA->Set( (InRQType == EB2GraphicsRQType::MSAAx4) ? 4 : 2 );
		}
	}
	else
	{
		// 만일 PostProcessAA 등이 추가되면 추가 CVar 를 세팅하게 될 것.
		// 그럼 AntiAliasingQuality 는 무시하게 될 수도 있고.

		if (_CVarMobileMSAA)
		{
			_CVarMobileMSAA->Set(1);
		}
	}
}

/**
* UI 에서 선택한 EB2GraphicsLevel 에 따라 엔진 Scalability 시스템 레벨 설정.
* Scalability 에 따른 세부적인 컨트롤은 Scalability.ini 및 이후는 엔진기능으로..
* BladeII 게임 내 Scalability 적용 기능의 메인 함수 격. (100% 커버는 아님)
*/
void AdjustScalabilityBySelectedLevel(UObject* WorldContextObject, EB2GraphicsLevel InOverallGraphicsLevel, EB2ResolutionLevel InResolutionLevel, bool bExtraContentScaleFallback)
{
//	Scalability::FQualityLevels QualityLevel = Scalability::GetQualityLevels();
//
//	const int32 PrevGameplayOnlyLODQuality = QualityLevel.GameplayOnlyLODQuality;
//
//#if !UE_BUILD_SHIPPING
//	const Scalability::FQualityLevels ForLevelCountRef = Scalability::GetQualityLevelCounts();
//	// ini 에 설정을 넣긴 했지만 바뀌는 거 감지 못하면 곤란하니.
//	check(ForLevelCountRef.AntiAliasingQuality == 5 && ForLevelCountRef.EffectsQuality == 5 && ForLevelCountRef.PostProcessQuality == 5 && ForLevelCountRef.PostProcessExtraQuality == 5 && ForLevelCountRef.TextureQuality == 5 && ForLevelCountRef.ShadowQuality == 5 && ForLevelCountRef.GameplayOnlyLODQuality == 5);
//
//	check(bOnModuleStartupCalled); /* 실수로 순서가 바뀌는 거 방지 차원 */
//#endif
//	
//	// 기본적으로는 GetScalabilityLevelOfGraphicsLevel 에 의해 맵핑된 값을 쓰지만 별도 맵핑을 쓰는 카테고리도 있다.
//	int32 DesiredScalabilityLevel = GetScalabilityLevelOfGraphicsLevel(InOverallGraphicsLevel);
//	
//	QualityLevel.EffectsQuality = DesiredScalabilityLevel;
//	QualityLevel.PostProcessQuality = DesiredScalabilityLevel;
//	QualityLevel.PostProcessExtraQuality = DesiredScalabilityLevel;
//	QualityLevel.TextureQuality = GetTextureLevelOfGraphicsLevel(InOverallGraphicsLevel);
//	QualityLevel.ShadowQuality = DesiredScalabilityLevel;
//	QualityLevel.GameplayOnlyLODQuality = GetGameplayOnlyLODQualityOfGraphicsLevel(InOverallGraphicsLevel);
//	QualityLevel.ViewDistanceQuality = DesiredScalabilityLevel;
//
//	// UI 에 연결된 인터페이스인 EB2ResolutionLevel 에 따라 기기별로 맵핑된 내부 식별자인 EB2GraphicsRQType 을 가져와서 사용
//	EB2GraphicsRQType MappedRQType = GetSafeMappedRQTypeOfResolutionLevel(InResolutionLevel);
//	if (bExtraContentScaleFallback)
//	{ // GetSafeMappedRQTypeOfResolutionLevel 에서도 나름의 상황에 따라 ContentScale 을 ScreenScale 로 바꾸는 동작을 하지만 추가로 외부에서 아싸리 폴백을 강제하는 경우가 또 있음.
//		MappedRQType = GetContentScaleRQTypeFallback(MappedRQType);
//	}
//
//	QualityLevel.AntiAliasingQuality = DesiredScalabilityLevel;
//	SetAntiAliasingByB2RQType(MappedRQType); // MSAA 만 있다면 엔진 Scalability 와는 상관없다.
//
//	QualityLevel.ResolutionQuality = GetResolutionScaleOfGraphicsLevel(InOverallGraphicsLevel);
//	AdjustResolutionQualityByRQType(MappedRQType, QualityLevel.ResolutionQuality);
//	
//	AdjustMobileContentScaleFactorByRQType(MappedRQType); // 실은 엔진 Scalability 와는 상관없다.
//
//	Scalability::SetQualityLevels(QualityLevel, true /*실제 변경된 값에 대해서만 Set 동작이 일어나도록 인자를 넣어줌*/);
//	// Scalability 자체의 ini 저장도 해야 하는데 게임쪽 선택값 저장만으로는 엔진 초기화 시점에 선택한 값을 적용하지 못하기 때문이다.
//	// 이렇게 저장해 두지 않으면 매번 게임 시작하면서 언제나 똑같은 초기값만을 적용했다가 로비 뜨고 나서 사용자 선택을 적용하고 하는 일이 반복될 것.
//	Scalability::SaveState(GIsEditor ? GEditorSettingsIni : GGameUserSettingsIni);
//	
//	if (WorldContextObject)
//	{
//		// 게임모드 별로 특수한 설정들.. 예를 들어 어떤 게임모드는 dynamic shadow 를 안 쓴다거나, LOD 를 보다 낮춰서 쓴다거나 등..
//		// 이걸 사용하는 곳은 대체로 로비겠지만 다른 게임모드에서도 설정 메뉴에 접근할 수가 있다.
//		ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
//		if (B2GM) 
//		{
//			B2GM->ApplyGameModeSpecificScalabilitySetting(InOverallGraphicsLevel, InResolutionLevel);
//
//			// GameplayOnlyLODQuality 바뀐 것에 대해서는 따로 핸들링을 좀.. 여기서라도
//			const int32 CurrGameplayOnlyLODQuality = Scalability::GetQualityLevels_GameplayOnlyLOD();
//			if (CurrGameplayOnlyLODQuality != PrevGameplayOnlyLODQuality)
//			{
//				OnGameplayOnlyLODQualityChanged(B2GM->GetWorld(), CurrGameplayOnlyLODQuality);
//			}
//		}
//	}
}

void AdjustScalabilityBySavedUserSelection(UObject* WorldContextObject) // 따로 필요한가..?
{
	/*int32 SavedGraphicsLevel = B2GraphicsLevelToInt(EB2GraphicsLevel::GraphicsLevel_HIGH);
	LoadGameSetting_Graphics_OrByDefault(SavedGraphicsLevel);
	int32 SavedResolutionLevel = B2ResolutionLevelToInt(EB2ResolutionLevel::Mid);
	LoadGameSetting_Resolution_OrByDefault(SavedResolutionLevel);
	AdjustScalabilityBySelectedLevel(WorldContextObject, SavedGraphicsLevel, SavedResolutionLevel);*/
}

/** 게임모드 및 연출 상황 등 BladeII 게임 시스템에 의한 scalability 대분류 중 shadow override */
void ApplyBladeIIBiasedScalability_Shadow(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetScalabilityLevelOfGraphicsLevel(InBasicUserSelection); // 대체로 GetScalabilityLevelOfGraphicsLevel 을 쓰겠지만 종류에 따라 아닐 수 있어서 인자로 EB2GraphicsLevel 을 받는 거.
	//int32 ShadowQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("ShadowQualityBias"), ShadowQualityBias, GScalabilityIni);

	//// 기본적인 세팅 UI 를 통한 선택에 지정된 Bias 만큼 차감한다.
	//// 만일 ini 에서 해당 항목을 못 읽어들였다고 하더라도 0 값으로 아래 코드를 실행해서 DesiredScalabilityLevel 이 적용되도록 한다.
	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - ShadowQualityBias, 0, QualityLevelCountRef.ShadowQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_Shadow() != BiasedNewLevel) {
	//	Scalability::SetQualityLevels_Shadow(BiasedNewLevel);
	//}
}
/** 게임모드 및 연출 상황 등 BladeII 게임 시스템에 의한 scalability 대분류 중 effect override */
void ApplyBladeIIBiasedScalability_Effect(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName)
{
	/*const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	const int32 DesiredScalabilityLevel = GetScalabilityLevelOfGraphicsLevel(InBasicUserSelection);
	int32 EffectsQualityBias = 0;
	GConfig->GetInt(*InIniSectionName, TEXT("EffectsQualityBias"), EffectsQualityBias, GScalabilityIni);

	int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - EffectsQualityBias, 0, QualityLevelCountRef.EffectsQuality - MaxPossibleQualityLevelSubtract);
	if (Scalability::GetQualityLevels_Effects() != BiasedNewLevel) {
		Scalability::SetQualityLevels_Effects(BiasedNewLevel);
	}*/
}
/** 게임모드 및 연출 상황 등 BladeII 게임 시스템에 의한 scalability 대분류 중 ViewDistance override */
void ApplyBladeIIBiasedScalability_ViewDist(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName, 
	bool bApplyHigherOnly = false /* 특정한 override 상황에서만 사용. */
)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetScalabilityLevelOfGraphicsLevel(InBasicUserSelection);
	//int32 ViewDistanceQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("ViewDistanceBias"), ViewDistanceQualityBias, GScalabilityIni);

	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - ViewDistanceQualityBias, 0, QualityLevelCountRef.ViewDistanceQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_ViewDistance() != BiasedNewLevel
	//	&&
	//	(!bApplyHigherOnly || Scalability::GetQualityLevels_ViewDistance() < BiasedNewLevel) // ApplyHigherOnly 에서는 낮은 scalability 레벨로 가지 않는다.
	//	) {
	//	Scalability::SetQualityLevels_ViewDistance(BiasedNewLevel);
	//}
}
/** 게임모드 및 연출 상황 등 BladeII 게임 시스템에 의한 scalability 대분류 중 PostProcess override */
void ApplyBladeIIBiasedScalability_PostProcess(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName,
	bool bApplyHigherOnly = false /* 특정한 override 상황에서만 사용. */
)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetScalabilityLevelOfGraphicsLevel(InBasicUserSelection);
	//int32 PostProcessQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("PostProcessQualityBias"), PostProcessQualityBias, GScalabilityIni);

	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - PostProcessQualityBias, 0, QualityLevelCountRef.PostProcessQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_PostProcess() != BiasedNewLevel
	//	&&
	//	(!bApplyHigherOnly || Scalability::GetQualityLevels_PostProcess() < BiasedNewLevel) // ApplyHigherOnly 에서는 낮은 scalability 레벨로 가지 않는다.
	//	) {
	//	Scalability::SetQualityLevels_PostProcess(BiasedNewLevel);
	//}
}
void ApplyBladeIIBiasedScalability_PostProcessExtra(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName,
	bool bApplyHigherOnly = false /* 특정한 override 상황에서만 사용. */
)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetScalabilityLevelOfGraphicsLevel(InBasicUserSelection);
	//int32 PostProcessExtraQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("PostProcessExtraQualityBias"), PostProcessExtraQualityBias, GScalabilityIni);

	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - PostProcessExtraQualityBias, 0, QualityLevelCountRef.PostProcessExtraQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_PostProcessExtra() != BiasedNewLevel
	//	&&
	//	(!bApplyHigherOnly || Scalability::GetQualityLevels_PostProcessExtra() < BiasedNewLevel) // ApplyHigherOnly 에서는 낮은 scalability 레벨로 가지 않는다.
	//	) {
	//	Scalability::SetQualityLevels_PostProcessExtra(BiasedNewLevel);
	//}
}
/** 게임모드 및 연출 상황 등 BladeII 게임 시스템에 의한 scalability 대분류 중 Texture override */
void ApplyBladeIIBiasedScalability_Texture(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetTextureLevelOfGraphicsLevel(InBasicUserSelection); // GetScalabilityLevelOfGraphicsLevel 이 아닌 별도 맵핑으로
	//int32 TextureQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("TextureQualityBias"), TextureQualityBias, GScalabilityIni);

	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - TextureQualityBias, 0, QualityLevelCountRef.TextureQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_Texture() != BiasedNewLevel) {
	//	Scalability::SetQualityLevels_Texture(BiasedNewLevel);
	//}
}
/** 게임모드 및 연출 상황 등 BladeII 게임 시스템에 의한 scalability 대분류 중 ResolutionQuality override */
void ApplyBladeIIBiasedScalability_Resolution(EB2GraphicsLevel InBasicUserSelection, EB2GraphicsRQType InRQType, const FString& InIniSectionName)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const float DesiredDefaultResolutionQuality = GetResolutionScaleOfGraphicsLevel(InBasicUserSelection);
	//float ResolutionQualityBias = 0; // 여긴 int 는 아님
	//GConfig->GetFloat(*InIniSectionName, TEXT("ResolutionQualityBias"), ResolutionQualityBias, GScalabilityIni);

	//float BiasedNewQuality = FMath::Clamp(DesiredDefaultResolutionQuality - ResolutionQualityBias, 10.0f, 100.0f);
	//// 만일 ExtraScreenScale 을 가한다면 Bias 적용 이후가 되어야.
	//AdjustResolutionQualityByRQType(InRQType, BiasedNewQuality);

	//if (Scalability::GetQualityLevels_Resolution() != BiasedNewQuality) {
	//	Scalability::SetQualityLevels_Resolution(BiasedNewQuality);
	//}
}
/** 게임모드 및 연출 상황 등 BladeII 게임 시스템에 의한 scalability 대분류 중 GameplayOnlyLOD override */
void ApplyBladeIIBiasedScalability_GameplayOnlyLOD(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetGameplayOnlyLODQualityOfGraphicsLevel(InBasicUserSelection); // GetScalabilityLevelOfGraphicsLevel 이 아님.
	//int32 GameplayOnlyLODQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("GameplayOnlyLODQualityBias"), GameplayOnlyLODQualityBias, GScalabilityIni);

	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - GameplayOnlyLODQualityBias, 0, QualityLevelCountRef.GameplayOnlyLODQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_GameplayOnlyLOD() != BiasedNewLevel) {
	//	Scalability::SetQualityLevels_GameplayOnlyLOD(BiasedNewLevel);
	//}
}

/** StageEventDirector 의 BeginShow/FinishShow 혹은 그에 해당하는 이벤트에서.. */
void ApplyStageEventSpecificScalabilitySetting(UObject* WorldContextObject, bool bIsShowOn)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ApplyStageEventSpecificScalabilitySetting %d"), (int32)bIsShowOn));
//
//#if !UE_BUILD_SHIPPING
//	if (!bUseBladeIIScalabilityOverride) { // 테스트 용으로 연출 상황에 따른 override 끌 수 있도록. 대신 LOD 제어는 따로임.
//		AdjustScalabilityBySavedUserSelection(WorldContextObject);
//		return;
//	}
//#endif
//
//	// 여기서 WorldContextObject 는 StageEventDirector 가 될 가능성이 크겠지.. 필요하면 캐스팅해서 써 보든지..?
//	// 현재 구현으로는 게임모드에 따른 구분이 없어서 사실상 추가적인 PerGMSetting 으로 봐도 될 듯.
//	// 게임모드 별로 연출용 scalability 세팅 따로 있으면 더 유연하긴 하겠지만 그게 필요할까 싶기도 하고.
//
//	if (bIsShowOn)
//	{
//		int32 UserSelectedLevelInt = B2GraphicsLevelToInt(EB2GraphicsLevel::GraphicsLevel_HIGH);
//		LoadGameSetting_Graphics_OrByDefault(UserSelectedLevelInt);
//		EB2GraphicsLevel UserSelectedLevel = IntToB2GraphicsLevel(UserSelectedLevelInt);
//
//		const FString StageEventSettingSection(TEXT("StageEventCinematicsSetting"));
//		
//		//
//		// 여기서 추가로 컨트롤하는 항목들은 당연히 런타임 적용이 가능한 것이어야 한다.
//		//
//
//		// 특히 연출 상황에서는 좀 높이는 걸 고려할 수 있는 ViewDistance 랑 PostProcess..?
//		// GameMode override 보다 낮게 가지는 않도록 함.
//		ApplyBladeIIBiasedScalability_ViewDist(UserSelectedLevel, StageEventSettingSection, true);
//		ApplyBladeIIBiasedScalability_PostProcess(UserSelectedLevel, StageEventSettingSection, true);
//		ApplyBladeIIBiasedScalability_PostProcessExtra(UserSelectedLevel, StageEventSettingSection, true);
//
//		//
//		// GameplayOnlyLODQuality 는 여기서 추가 override 를 하지 않고 아예 그것 자체가 연출용 세부 항목이 있다.
//		// LOD 레벨은 게임 구현 구조 상 더 다양한 이벤트를 통해 들어가야 해서.
//		//
//	}
//	else
//	{ // 유저 선택의 기본 상황으로 되돌림. 만일 이렇게 특별히 scalability 를 조절하는 경우가 또 생기면 이걸로 안 되겠지.
//		AdjustScalabilityBySavedUserSelection(WorldContextObject);
//	}
}

FString GetScalabilitySectionNamePerGM(ABladeIIGameMode* InB2GM)
{
	if (InB2GM) {
		return FString(TEXT("PerGMSetting")) + TEXT("@") + InB2GM->GetGameModeScalabilitySettingSectionPostfix();
	}
	return TEXT("");
}

/** ApplyGameModeSpecificScalabilitySetting 오버라이드 한 함수들은 결국 모두 이걸 콜한다.. ini 에 자신에 해당하는 항목과 함께 */
void ApplyGameModeSpecificScalabilitySetting_OverrideCommon(ABladeIIGameMode* InB2GM, EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
//#if !UE_BUILD_SHIPPING
//	if (!B2Scalability::bUseBladeIIScalabilityOverride) { // 테스트 용으로 게임모드에 따른 override 끌 수 있도록
//		return;
//	}
//#endif
//
//	if (!InB2GM) {
//		return;
//	}
//
//	EB2GraphicsLevel UsedGraphicsLevel = InSelectedGraphicsLevel;
//	if (UsedGraphicsLevel == EB2GraphicsLevel::GraphicsLevel_End) // 지정하지 않은 경우 UI 설정에 따라 ini 저장된 값 사용.
//	{
//		int32 SavedGraphicsLevel = B2GraphicsLevelToInt(EB2GraphicsLevel::GraphicsLevel_HIGH);
//		// 이 이전에 GameSetting 값 초기화가 안 되어 있으면 여기서 override 한 scalability 가 아닌 DeviceProfile 에 의해 정해진 scalability 로 GraphicsLevel 을 세팅하게 될 것.
//		LoadGameSetting_Graphics_OrByDefault(SavedGraphicsLevel);		
//		UsedGraphicsLevel = IntToB2GraphicsLevel(SavedGraphicsLevel);
//	}
//
//	EB2ResolutionLevel UsedResLevel = InSelectedResLevel;
//	if (UsedResLevel == EB2ResolutionLevel::End) // 마찬가지로 지정하지 않은 경우.. ini 저장된 값으로
//	{
//		int32 SavedResLevel = B2ResolutionLevelToInt(EB2ResolutionLevel::Mid);
//		LoadGameSetting_Resolution_OrByDefault(SavedResLevel);
//		UsedResLevel = IntToB2ResolutionLevel(SavedResLevel);
//	}
//	
//	//
//	// 여기서 추가로 컨트롤하는 항목들은 당연히 런타임 적용이 가능한 것이어야 한다.
//	//
//		
//	const FString& DesiredSectionName = GetScalabilitySectionNamePerGM(InB2GM);
//	ApplyBladeIIBiasedScalability_Shadow(UsedGraphicsLevel, DesiredSectionName);
//	ApplyBladeIIBiasedScalability_Effect(UsedGraphicsLevel, DesiredSectionName);
//	ApplyBladeIIBiasedScalability_PostProcess(UsedGraphicsLevel, DesiredSectionName);
//	ApplyBladeIIBiasedScalability_PostProcessExtra(UsedGraphicsLevel, DesiredSectionName);
//	ApplyBladeIIBiasedScalability_Texture(UsedGraphicsLevel, DesiredSectionName);
//	ApplyBladeIIBiasedScalability_Resolution(UsedGraphicsLevel, GetSafeMappedRQTypeOfResolutionLevel(UsedResLevel), DesiredSectionName);
//	ApplyBladeIIBiasedScalability_GameplayOnlyLOD(UsedGraphicsLevel, DesiredSectionName);
//	ApplyBladeIIBiasedScalability_ViewDist(UsedGraphicsLevel, DesiredSectionName);

}


/************************************************************************/
/* EB2GraphicsLevel 과 Scalability 간의 맵핑은 좀 복잡하지만
EB2FrameLimitLevel 이나 EB2ResolutionLevel 의 맵핑은 보다 단순.*/
/************************************************************************/
IConsoleVariable* GetMappedCVarOfFrameLimitLevel(EB2FrameLimitLevel InRelativeLevel)
{
	// EB2FrameLimitLevel 에 해당하는 CVar 를 연결해줌.
	IConsoleVariable* FoundCVar = IConsoleManager::Get().FindConsoleVariable
	(
		(InRelativeLevel == EB2FrameLimitLevel::High) ? TEXT("r.DefinedMobileFrameLimitQuality_H") : (
		(InRelativeLevel == EB2FrameLimitLevel::Low) ? TEXT("r.DefinedMobileFrameLimitQuality_L") :
			TEXT("r.DefinedMobileFrameLimitQuality_M")
			)
	);
	check(FoundCVar); // 없음 이름이 바뀌었든지..
	return FoundCVar;
}

/** 프레임률 제한은 기기 성능 레벨에 따라 차등을 둔다. 
 * DeviceProfile 로 들어가는 기준 수치 대비해서 변경 가능한 인터페이스를 제공. 
 * 예를 들어 갤럭시S6 과 S8 두 기기에서 설정된 EB2FrameLimitLevel 변수가 같더라도 실제 프레임 제한 수치는 달라지게 되는 것. */
void SetRelativeFrameLimitLevel(EB2FrameLimitLevel InRelativeLevel)
{
	IConsoleVariable* CVarDefinedLevel = GetMappedCVarOfFrameLimitLevel(InRelativeLevel);
	IConsoleVariable* CVarCurrentLevel = IConsoleManager::Get().FindConsoleVariable(TEXT("r.CurrentMobileFrameLimitLevel"));

	check(CVarDefinedLevel && CVarCurrentLevel); // 둘 중 하나가 없음 이름이 바뀌었든지..
	if (CVarDefinedLevel && CVarCurrentLevel)
	{
		// DeviceProfile 에 의해 정해지는 기준 수치를 실제 FrameLimit 에 사용되는 콘솔 변수에 전달
		CVarCurrentLevel->Set(CVarDefinedLevel->GetInt());
	}

}

EB2FrameLimitLevel GetRelativeFrameLimitLevelInternal(int32 CheckFrameLimitLevel)
{
	IConsoleVariable* CVarDefinedLevel = GetMappedCVarOfFrameLimitLevel(EB2FrameLimitLevel::Mid); // Mid 를 기준으로.
	check(CVarDefinedLevel);
	if (CVarDefinedLevel)
	{
		const int32 DefinedLevelInt = CVarDefinedLevel->GetInt();

		// 설정에 따라 DefinedMobileFrameLimitQuality 의 L 이나 H 를 M 과 동일한 값으로 놓게 될 수도 있는데 여하간 Mid 로 봐야 할 것.
		// 그런 경우는 애시당초 해당 레벨에 대한 선택이 제공되지 않아야..
		return (CheckFrameLimitLevel > DefinedLevelInt) ? EB2FrameLimitLevel::High
			: ((CheckFrameLimitLevel < DefinedLevelInt) ? EB2FrameLimitLevel::Low : EB2FrameLimitLevel::Mid);

	}
	return EB2FrameLimitLevel::Mid;
}
EB2FrameLimitLevel GetCurrentRelativeFrameLimitLevel()
{
	IConsoleVariable* CVarCurrentLevel = IConsoleManager::Get().FindConsoleVariable(TEXT("r.CurrentMobileFrameLimitLevel"));
	check(CVarCurrentLevel); // 없음 이름이 바뀌었든지..
	if (CVarCurrentLevel)
	{
		return GetRelativeFrameLimitLevelInternal(CVarCurrentLevel->GetInt());
	}
	return EB2FrameLimitLevel::Mid;
}
EB2FrameLimitLevel GetDefaultRelativeFrameLimitLevel()
{
	IConsoleVariable* CVarDefaultLevel = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefinedMobileFrameLimitQuality_Default"));
	check(CVarDefaultLevel); // 없음 이름이 바뀌었든지..
	if (CVarDefaultLevel)
	{
		return GetRelativeFrameLimitLevelInternal(CVarDefaultLevel->GetInt());
	}
	return EB2FrameLimitLevel::Mid;
}

bool IsRelativeFrameLimitLevelAvailable(EB2FrameLimitLevel InCheckLevel)
{
	// 현재 설정으로 볼 때에 EB2FrameLimitLevel 의 특정 값으로의 세팅이 가능한지 체크하는 거.
	// 불가능하다면 그 값에 대한 SetRelativeFrameLimitLevel 이 사실상 안 먹히겠지..

	if (InCheckLevel == EB2FrameLimitLevel::Mid) {
		return true; // 원칙적으로 Mid 는 여기서 체크하는 기준이 되고 따라서 무조건 true.
	}

	IConsoleVariable* CVarMidLevel = GetMappedCVarOfFrameLimitLevel(EB2FrameLimitLevel::Mid);
	IConsoleVariable* CVarCheckLevel = GetMappedCVarOfFrameLimitLevel(InCheckLevel);
	check(CVarMidLevel && CVarCheckLevel); // 없음 이름이 바뀌었든지..
	if (CVarMidLevel && CVarCheckLevel)
	{
		if (
			InCheckLevel == EB2FrameLimitLevel::High &&
			CVarCheckLevel->GetInt() > CVarMidLevel->GetInt() // High 에 대한 세팅이 Mid 보다 높아야 그쪽으로의 세팅이 지원되는 것으로 본다.
			)
		{
			return true;
		}
		else if (
			InCheckLevel == EB2FrameLimitLevel::Low &&
			CVarCheckLevel->GetInt() < CVarMidLevel->GetInt() && // Low 에 대한 세팅이 Mid 보다 낮아야 그쪽으로의 세팅이 지원되는 것으로 본다.
			CVarCheckLevel->GetInt() >= 0 // 단, 여기서는 조건이 하나 더 있는데 0 보다 낮아서는 안 되겠지.
			)
		{
			return true;
		}
	}
	return false;
}

IConsoleVariable* GetMappedCVarOfResolutionLevel(EB2ResolutionLevel InLevel)
{
	// EB2FrameLimitLevel 에 해당하는 CVar 를 연결해줌.
	IConsoleVariable* FoundCVar = IConsoleManager::Get().FindConsoleVariable
	(
		(InLevel == EB2ResolutionLevel::High) ? TEXT("r.AllowedGameRQType_H") : (
		(InLevel == EB2ResolutionLevel::Low) ? TEXT("r.AllowedGameRQType_L") :
			TEXT("r.AllowedGameRQType_M")
			)
	);
	check(FoundCVar); // 없음 이름이 바뀌었든지..
	return FoundCVar;
}
EB2GraphicsRQType GetMappedRQTypeOfResolutionLevel(EB2ResolutionLevel InLevel)
{
	IConsoleVariable* MappedCVar = GetMappedCVarOfResolutionLevel(InLevel);
	if (MappedCVar)
	{
		return static_cast<EB2GraphicsRQType>(MappedCVar->GetInt());
	}
	return EB2GraphicsRQType::Default;
}
EB2GraphicsRQType GetSafeMappedRQTypeOfResolutionLevel(EB2ResolutionLevel InLevel)
{
	EB2GraphicsRQType SimpleMappedRQType = GetMappedRQTypeOfResolutionLevel(InLevel);

#if PLATFORM_ANDROID
	// 디바이스 환경 설정에 따라 좀 다르게 쓰려는 게 있어서.. Safe 라기보다는 쓸데없는 거 대체 차원.
	IConsoleVariable* AndroidWindowSizeXCvar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DetectedAndroidDeviceWindowSizeX"));
	IConsoleVariable* AndroidWindowSizeYCvar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DetectedAndroidDeviceWindowSizeY"));
	check(AndroidWindowSizeXCvar && AndroidWindowSizeYCvar);
	if (IsRQTypeOneOfExtraContentScale(SimpleMappedRQType) &&
		AndroidWindowSizeXCvar && AndroidWindowSizeYCvar &&
		AndroidWindowSizeXCvar->GetInt() > 0 && AndroidWindowSizeYCvar->GetInt() > 0)
	{
		// 표준 해상도를 넘지 못하는 화면이면 MobileContentScaleFactor 를 써서 이득이 될 게 없다.
		// 표준 해상도를 넘는다고 해도 썩 높지 않으면 역시나 그다지..

		// 원래 정의된 스케일 값.. 이 값으로 이득을 볼 만한 해상도인지 체크
		// IOS 면 아닐 수 있지만 Android 면 이 값이 그대로 MCSF 에 들어간다고 봐도 무방할 듯.
		const float DefinedContentScaleValue = GetContentScaleRQTypeScaleValue(SimpleMappedRQType);
		// 기준 해상도에서 이 배수만큼 크면 ContentScale 을 적용하기 충분한 사이즈라고 판단하는 거.
		const float BigEnoughScreenSizeScale = 1.0f + ((DefinedContentScaleValue - 1.0f) * 0.5f);
		const int32 BigEnoughScreenSizeX = (int32)(1280.0f * BigEnoughScreenSizeScale);
		const int32 BigEnoughScreenSizeY = (int32)(720.0f * BigEnoughScreenSizeScale);

		UE_LOG(LogBladeII, Log, TEXT("ScreenSize considered to be big enough for ExtraContentScale %.2f: Scale %.2f, Size %dx%d"),
			DefinedContentScaleValue, BigEnoughScreenSizeScale, BigEnoughScreenSizeX, BigEnoughScreenSizeY);

		if (AndroidWindowSizeXCvar->GetInt() < BigEnoughScreenSizeX && AndroidWindowSizeYCvar->GetInt() < BigEnoughScreenSizeY)
		{
			EB2GraphicsRQType FallbackRQType = GetContentScaleRQTypeFallback(SimpleMappedRQType);
#if !UE_BUILD_SHIPPING
			if (FallbackRQType != SimpleMappedRQType)
			{
				UE_LOG(LogBladeII, Log, TEXT("Falling back to RQType %d -> %d as physical device screen size is not big enough. %dx%d"),
					(int32)SimpleMappedRQType, (int32)FallbackRQType,
					AndroidWindowSizeXCvar->GetInt(), AndroidWindowSizeYCvar->GetInt()
				);
			}
#endif
			return FallbackRQType;
		}
	}
#endif
	return SimpleMappedRQType;
}
EB2ResolutionLevel GetResolutionLevelOfRQType(EB2GraphicsRQType InRQType)
{
	// 정확히 EB2ResolutionLevel 중 하나에 맵핑되지 않는 RQType 이 들어올 가능성을 생각해서.. 
	// 여기도 FrameLimitLevel 처럼 enum 항목 숫자 크기로 보다 높은 설정용인지를 판단한다.

	IConsoleVariable* CVarDefinedHighLevelRQ = GetMappedCVarOfResolutionLevel(EB2ResolutionLevel::High);
	IConsoleVariable* CVarDefinedMidLevelRQ = GetMappedCVarOfResolutionLevel(EB2ResolutionLevel::Mid);
	IConsoleVariable* CVarDefinedLowLevelRQ = GetMappedCVarOfResolutionLevel(EB2ResolutionLevel::Low);
	check(CVarDefinedHighLevelRQ && CVarDefinedMidLevelRQ && CVarDefinedLowLevelRQ);
	if (CVarDefinedHighLevelRQ && CVarDefinedMidLevelRQ && CVarDefinedLowLevelRQ && InRQType != EB2GraphicsRQType::End)
	{
		const int32 DefinedHighLevelRQInt = CVarDefinedHighLevelRQ->GetInt();
		const int32 DefinedMidLevelRQInt = CVarDefinedMidLevelRQ->GetInt();
		const int32 DefinedLowLevelRQInt = CVarDefinedLowLevelRQ->GetInt();
		// High == Mid, Mid == Low 일 수는 있지만 적어도 High > Low 여야.
		check(DefinedHighLevelRQInt > DefinedLowLevelRQInt);

		const int32 CheckRQTypeInt = static_cast<int32>(InRQType);

		// High == Mid 거나 Mid == Low 인 경우 Mid 가 우선순위이므로 Mid 값과 같은지 먼저 검사
		if (CheckRQTypeInt == DefinedMidLevelRQInt) {
			return EB2ResolutionLevel::Mid;
		}

		// High 이상의 수치는 High 로, Low 이하의 수치는 Low 로, 그 외에는 Mid 로 간주
		return (CheckRQTypeInt >= DefinedHighLevelRQInt) ? EB2ResolutionLevel::High
			: ((CheckRQTypeInt <= DefinedLowLevelRQInt) ? EB2ResolutionLevel::Low : EB2ResolutionLevel::Mid);
	}
	return EB2ResolutionLevel::Mid;
}
bool IsResolutionLevelAvailable(EB2ResolutionLevel InCheckLevel)
{
	if (InCheckLevel == EB2ResolutionLevel::Mid) {
		return true; // 기준값. 무조건 true.
	}

	const int32 MidRQLevelInt = static_cast<int32>(GetMappedRQTypeOfResolutionLevel(EB2ResolutionLevel::Mid));
	const int32 CheckRQLevelInt = static_cast<int32>(GetMappedRQTypeOfResolutionLevel(InCheckLevel));

	// 여기도 RQType 의 숫자 크기에 따라 높은 설정 혹은 낮은 설정으로 간주하는 것.
	if (InCheckLevel == EB2ResolutionLevel::High)
	{
		return (CheckRQLevelInt > MidRQLevelInt);
	}
	else if (InCheckLevel == EB2ResolutionLevel::Low)
	{
		return (CheckRQLevelInt < MidRQLevelInt);
	}

	check(0); // 이외에 EB2ResolutionLevel 에 추가된 게 있는데 대처를 안했다면 걸리겠지.
	return false;
}

bool DoesRQTypeRequiresAppRestart(EB2GraphicsRQType InCheckRQType)
{
	check(InitialRQTypeOfAppStart != EB2GraphicsRQType::End); // 아니면 아직 초기화 안 된 상태에서 불린 거.
	if (InitialRQTypeOfAppStart != EB2GraphicsRQType::End)
	{
		const bool bInitialRQTypeMCSF = IsRQTypeOneOfExtraContentScale(InitialRQTypeOfAppStart);
		const bool bCheckRQTypeMCSF = IsRQTypeOneOfExtraContentScale(InCheckRQType);
		// 앱 시작할 때의 첫 값과 비교시..
		if (
			(bInitialRQTypeMCSF != bCheckRQTypeMCSF) || // MCSF 를 조절하는 것과 아닌 것 사이의 변환이거나
			(bInitialRQTypeMCSF && bCheckRQTypeMCSF && InitialRQTypeOfAppStart != InCheckRQType) // 둘 다 MCSF 조절인데 다르거나
			)
		{
			return true;
		}
	}
	return false;
}
void ResetAllScalabilitySettings()
{
	/*SaveGameSetting_Graphics(B2GraphicsLevelToInt(B2Scalability::GetGraphicsLevelByExpectedScalability()));
	SaveGameSetting_FrameLimit(B2FrameLevelToInt(B2Scalability::GetDefaultRelativeFrameLimitLevel()));
	SaveGameSetting_Resolution(B2ResolutionLevelToInt(B2Scalability::GetResolutionLevelOfRQType(GetDefaultGraphicsRQType())));*/
}

}

/************************************************************************
게임 모드 별 Scalability 설정 인터페이스 구현을 관리하기 좋게 여기에 몰아넣음.
************************************************************************/

void ABladeIIGameMode::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	
	// 여긴 Override 를 하지 않았을 때의 기본 구현으로 가장 기본값으로 돌아갈 수 있도록 한다. 
	//

	// ApplyGameModeSpecificScalabilitySetting 을 따로 Override 하지 않으면 AdjustScalabilityBySelectedLevel 를 그대로 사용한 것과 마찬가지가 되도록.

	//EB2GraphicsLevel UsedGraphicsLevel = InSelectedGraphicsLevel;
	//if (UsedGraphicsLevel == EB2GraphicsLevel::GraphicsLevel_End) // 지정하지 않은 경우 UI 설정에 따라 ini 저장된 값 사용.
	//{
	//	int32 SavedGraphicsLevel = B2GraphicsLevelToInt(EB2GraphicsLevel::GraphicsLevel_HIGH);
	//	bool bSettingLoaded = LoadGameSetting_Graphics_OrByDefault(SavedGraphicsLevel);
	//	// 이 이전에 GameSetting 값 초기화가 안 되어 있다고 해도 여기선 기본 scalability 로 돌려놓으니 문제는 없겠지만 그래도 바람직한 상황은 아니지.
	//	check(bSettingLoaded);
	//	UsedGraphicsLevel = IntToB2GraphicsLevel(SavedGraphicsLevel);
	//}

	//{
	//	const int32 DesiredScalabilityLevel = B2Scalability::GetScalabilityLevelOfGraphicsLevel(UsedGraphicsLevel);
	//	if (Scalability::GetQualityLevels_Shadow() != DesiredScalabilityLevel) {
	//		Scalability::SetQualityLevels_Shadow(DesiredScalabilityLevel);
	//	}
	//}
	//{
	//	const int32 DesiredScalabilityLevel = B2Scalability::GetScalabilityLevelOfGraphicsLevel(UsedGraphicsLevel);
	//	if (Scalability::GetQualityLevels_Effects() != DesiredScalabilityLevel) {
	//		Scalability::SetQualityLevels_Effects(DesiredScalabilityLevel);
	//	}
	//}
	//{
	//	const int32 DesiredScalabilityLevel = B2Scalability::GetScalabilityLevelOfGraphicsLevel(UsedGraphicsLevel);
	//	if (Scalability::GetQualityLevels_PostProcess() != DesiredScalabilityLevel) {
	//		Scalability::SetQualityLevels_PostProcess(DesiredScalabilityLevel);
	//	}
	//}
	//{
	//	const int32 DesiredScalabilityLevel = B2Scalability::GetScalabilityLevelOfGraphicsLevel(UsedGraphicsLevel);
	//	if (Scalability::GetQualityLevels_PostProcessExtra() != DesiredScalabilityLevel) {
	//		Scalability::SetQualityLevels_PostProcessExtra(DesiredScalabilityLevel);
	//	}
	//}
	//{
	//	const int32 DesiredScalabilityLevel = B2Scalability::GetTextureLevelOfGraphicsLevel(UsedGraphicsLevel);
	//	if (Scalability::GetQualityLevels_Texture() != DesiredScalabilityLevel) {
	//		Scalability::SetQualityLevels_Texture(DesiredScalabilityLevel);
	//	}
	//}
	//{
	//	float DesiredScalabilityLevel = B2Scalability::GetResolutionScaleOfGraphicsLevel(UsedGraphicsLevel);
	//	// 모드별 override 가 아니더라도 RQType 에 따른 추가 조절은 필요.
	//	B2Scalability::AdjustResolutionQualityByRQType(B2Scalability::GetSafeMappedRQTypeOfResolutionLevel(InSelectedResLevel), DesiredScalabilityLevel);
	//	if (Scalability::GetQualityLevels_Resolution() != DesiredScalabilityLevel) {
	//		Scalability::SetQualityLevels_Resolution(DesiredScalabilityLevel);
	//	}
	//}
	//{
	//	const int32 DesiredScalabilityLevel = B2Scalability::GetGameplayOnlyLODQualityOfGraphicsLevel(UsedGraphicsLevel);
	//	if (Scalability::GetQualityLevels_GameplayOnlyLOD() != DesiredScalabilityLevel) {
	//		Scalability::SetQualityLevels_GameplayOnlyLOD(DesiredScalabilityLevel);
	//	}
	//}
	//{
	//	const int32 DesiredScalabilityLevel = B2Scalability::GetScalabilityLevelOfGraphicsLevel(UsedGraphicsLevel);
	//	if (Scalability::GetQualityLevels_ViewDistance() != DesiredScalabilityLevel) {
	//		Scalability::SetQualityLevels_ViewDistance(DesiredScalabilityLevel);
	//	}
	//}
}
FString ABladeIIGameMode::GetGameModeScalabilitySettingSectionPostfix() const
{
	return FString(TEXT("ABladeIIGameMode")); // 게임모드 이름 그대로 리턴하는데 Class GetName 하니까 결국 Blueprint 클래스 이름이 나와서 이렇게.. ㅡㅡ
}

void AB2LobbyGameMode::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	B2Scalability::ApplyGameModeSpecificScalabilitySetting_OverrideCommon(this, InSelectedGraphicsLevel, InSelectedResLevel);
}
FString AB2LobbyGameMode::GetGameModeScalabilitySettingSectionPostfix() const
{
	return FString(TEXT("AB2LobbyGameMode"));
}

void AB2StageGameModeBase::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	B2Scalability::ApplyGameModeSpecificScalabilitySetting_OverrideCommon(this, InSelectedGraphicsLevel, InSelectedResLevel);
}
FString AB2StageGameModeBase::GetGameModeScalabilitySettingSectionPostfix() const
{
	return FString(TEXT("AB2StageGameModeBase"));
}

void AB2TutorialGameMode::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	B2Scalability::ApplyGameModeSpecificScalabilitySetting_OverrideCommon(this, InSelectedGraphicsLevel, InSelectedResLevel);
}
FString AB2TutorialGameMode::GetGameModeScalabilitySettingSectionPostfix() const
{
	return FString(TEXT("AB2TutorialGameMode"));
}

void AB2CounterAttackGameMode::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	B2Scalability::ApplyGameModeSpecificScalabilitySetting_OverrideCommon(this, InSelectedGraphicsLevel, InSelectedResLevel);
}
FString AB2CounterAttackGameMode::GetGameModeScalabilitySettingSectionPostfix() const
{
	return FString(TEXT("AB2CounterAttackGameMode"));
}

void AB2PVPGameMode::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	B2Scalability::ApplyGameModeSpecificScalabilitySetting_OverrideCommon(this, InSelectedGraphicsLevel, InSelectedResLevel);
}
FString AB2PVPGameMode::GetGameModeScalabilitySettingSectionPostfix() const
{
	return FString(TEXT("AB2PVPGameMode"));
}

void AB2RaidGameMode::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	B2Scalability::ApplyGameModeSpecificScalabilitySetting_OverrideCommon(this, InSelectedGraphicsLevel, InSelectedResLevel);
}
FString AB2RaidGameMode::GetGameModeScalabilitySettingSectionPostfix() const
{
	return FString(TEXT("AB2RaidGameMode"));
}

void AB2ControlGameMode::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	B2Scalability::ApplyGameModeSpecificScalabilitySetting_OverrideCommon(this, InSelectedGraphicsLevel, InSelectedResLevel);
}
FString AB2ControlGameMode::GetGameModeScalabilitySettingSectionPostfix() const
{
	return FString(TEXT("AB2ControlGameMode"));
}

void AB2TMGameMode::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	B2Scalability::ApplyGameModeSpecificScalabilitySetting_OverrideCommon(this, InSelectedGraphicsLevel, InSelectedResLevel);
}
FString AB2TMGameMode::GetGameModeScalabilitySettingSectionPostfix() const
{
	return FString(TEXT("AB2TMGameMode"));
}

void AB2GuildGameMode::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	B2Scalability::ApplyGameModeSpecificScalabilitySetting_OverrideCommon(this, InSelectedGraphicsLevel, InSelectedResLevel);
}
FString AB2GuildGameMode::GetGameModeScalabilitySettingSectionPostfix() const
{
	return FString(TEXT("AB2GuildGameMode"));
}

void AB2PreRenderGameMode::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	B2Scalability::ApplyGameModeSpecificScalabilitySetting_OverrideCommon(this, InSelectedGraphicsLevel, InSelectedResLevel);
}
FString AB2PreRenderGameMode::GetGameModeScalabilitySettingSectionPostfix() const
{
	return FString(TEXT("AB2PreRenderGameMode"));
}


/************************************************************************
기타 Scalability 와 관련있는 변수나 유틸들..
************************************************************************/

static TAutoConsoleVariable<float> CVarB2CameraDistanceAlpha(
	TEXT("r.B2CameraDistanceAlpha"),
	1.0f,
	TEXT("Expected to be in 0.0 ~ 1.0 range, it adjusts the amount of extra camera distance being applied to CameraBoomArmLength.\n"),
	ECVF_Scalability /* 꼭 Scalability 컨트롤을 하지 않더라도 그럴 여지를 남겨둠. */
);
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
float GDevTestCameraDistanceScale = -1.0f;
#endif
float GetExtraAppliedCameraDistance(float InBaseLength, float InExtraLength)
{ // 간단한 코드이지만 분산되어 있는 세팅들을 적용할 때 여길 통과하게 함으로써 기능의 일관성을 보장한다.
	float ScaledDistance = (InBaseLength >= InExtraLength) ? InBaseLength : // ExtraLength 는 BaseLength 보다 커야만 동작한다.
		FMath::Lerp(InBaseLength, InExtraLength, CVarB2CameraDistanceAlpha.GetValueOnGameThread());
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (GDevTestCameraDistanceScale > 0.0f)
	{ // 순전히 테스트 용 추가 스케일. ExtraLength 가 세팅 안된 상황에서는 B2CameraDistanceAlpha 만 가지고는 테스트가 안되므로.
		ScaledDistance *= GDevTestCameraDistanceScale;
	}
#endif
	return ScaledDistance;
}