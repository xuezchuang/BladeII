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
���� ������ ���� �𸮾� Scalability �ý��� ��Ʈ�� �ϴ� ���� ��ƿ�� ��Ƴ���..
*/
/************************************************************************/
//
/** 
 * EB2ResolutionLevel �� ���ε� EB2GraphicsRQType �� �����ϴ� ����. DeviceProfile �� ���� �÷��� �� ��⺰�� ���� ����.
 * r.DefinedMobileFrameLimitQuality_H/M/L �� �������� ����. 
 * �� ��ü�� int ĳ���� �� EB2GraphicsRQType �� ������.
 * EB2GraphicsRQType �� �߰��Ǵ� ���� ���� �� ���⵵ ����.
 */
static TAutoConsoleVariable<int32> CVarAllowedGameRQType_H(
	TEXT("r.AllowedGameRQType_H"),
#if PLATFORM_WINDOWS
	7, /* PC �⺻���� ��� ������ �������� ������ �� �����. �ƴ� [Windows DeviceProfile] �� �־�ֵ� �Ǳ� ��. */
#else
	2,
#endif
	TEXT("Specifies which type of Resolution-Quality technique is allowed for BladeII game ResolutionLevel High:\n"),
	ECVF_ReadOnly /* ReadOnly �μ� DeviceProfile �� ���� �����ϵ���.. */
);
static TAutoConsoleVariable<int32> CVarAllowedGameRQType_M(
	TEXT("r.AllowedGameRQType_M"),
	2,
	TEXT("Specifies which type of Resolution-Quality technique is allowed for BladeII game ResolutionLevel Mid:\n"),
	ECVF_ReadOnly /* ReadOnly �μ� DeviceProfile �� ���� �����ϵ���.. */
);
static TAutoConsoleVariable<int32> CVarAllowedGameRQType_L(
	TEXT("r.AllowedGameRQType_L"),
	1,
	TEXT("Specifies which type of Resolution-Quality technique is allowed for BladeII game ResolutionLevel Low:\n"),
	ECVF_ReadOnly /* ReadOnly �μ� DeviceProfile �� ���� �����ϵ���.. */
);

/** �� ��ü�� EB2GraphicsRQType �̰� DeviceProfile �� ���� ����. ����� B2ResolutionLevel ���� ���� ���ε� RQType �� ����Ǳ� ������ ����. */
static TAutoConsoleVariable<int32> CVarDefaultB2GraphicsRQType(
	TEXT("r.DefaultB2GraphicsRQType"),
	2,
	TEXT("The default value of B2GraphicsRQType before user selection is loaded and applied. Supposed to be set by DeviceProfile."),
	ECVF_ReadOnly /* ReadOnly �μ� DeviceProfile �� ���� �����ϵ���.. */
);

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
// AllowedGameRQType �� �����ϴ� �׽�Ʈ Ŀ�ǵ� ��.
static TAutoConsoleVariable<int32> CVarB2DevForcedGraphicsRQType(
	TEXT("r.DevForcedGraphicsRQType"),
	-1,
	TEXT("Provided only for testing. In zero or positive value, it will take effect as casted EB2GraphicsRQType. It ignores AllowedGameRQType."),
	ECVF_Default
);
#endif

/** DeviceProfile �� ���� ���� �ɼ� �⺻�� ���� �� ������Ʈ ���� �� ���� ����� ���� �ѹ� �����ϱ� ���� ���. */ 
static TAutoConsoleVariable<int32> CVarResetScalabilityChance(
	TEXT("r.ScalabilityResetChance"),
	0, // �̰� 1�� �����ϴ� �� ���� ������ �ʿ�� ����. �ѵι��� ������Ʈ �� ����������?
	TEXT("It allows all saved BladeII scalability settings to default desired setting, only once by local ini state.\n"),
	ECVF_ReadOnly /* ReadOnly �μ� DeviceProfile �� ���� �����ϵ���.. */
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
{ // �ܼ��� r.DefaultB2GraphicsRQType ������ EB2GraphicsRQType enum ����
	return static_cast<EB2GraphicsRQType>(CVarDefaultB2GraphicsRQType.GetValueOnAnyThread());
}

FORCEINLINE IConsoleVariable* GetMCSFCVar() { return IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor")); }

namespace B2Scalability
{
#if !UE_BUILD_SHIPPING
/** BladeII ���Ӹ�峪 ���� �� ���� ��Ȳ�� ���� ���� �ɼ� bias ���ϴ� ��� On/Off 
 * �� ������ ���� �⺻ ���¿����� true ����� ��. */
bool bUseBladeIIScalabilityOverride = true; 
bool bOnModuleStartupCalled = false;
#endif

/** DeviceProfile �� ���� ������ ���� �ʱ� MobileContentScaleFactor. */
float CachedBaseMobileContentScaleFactor = 1.0f;

/** �� ���� ������ RQType �� ������ ���´�. Ư�� ��� ���� switching �� �̹� ���� �Ⱓ ���� ���� ������ �����ϱ� ����. */
EB2GraphicsRQType InitialRQTypeOfAppStart = EB2GraphicsRQType::End;

void OnModuleStartup()
{
//#if !UE_BUILD_SHIPPING 
//	if (GConfig)
//	{
//		// �׽�Ʈ ���� ���� ini �ɼǵ�.. �ʿ��� ���� ����.
//		GConfig->GetBool(TEXT("B2Scalability"), TEXT("UseBladeIIScalabilityOverride"), bUseBladeIIScalabilityOverride, GScalabilityIni);
//	}
//#endif
//
//	static IConsoleVariable* CVarMCSF = GetMCSFCVar();
//	check(CVarMCSF);
//	if (CVarMCSF)
//	{ // �� �������� �������� ���� �׳� �ʱⰪ�̰ų� DeviceProfile �� ���� ���õ� ���̾��.
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
//		/* H == M, M == L �ϼ��� ������ �� �� ��� ���� ���� ����. H �� L ���ٴ� ���ƾ�. */
//		CVarAllowedGameRQType_H.GetValueOnGameThread() > CVarAllowedGameRQType_L.GetValueOnGameThread()
//	);
//#if PLATFORM_IOS
//	// iOS ���� MobileContentScaleFactor �ǵ帮�� �ʴ� �� ���� �� ����.
//	check(
//		!IsRQTypeOneOfExtraContentScale(static_cast<EB2GraphicsRQType>(CVarAllowedGameRQType_H.GetValueOnGameThread())) &&
//		!IsRQTypeOneOfExtraContentScale(static_cast<EB2GraphicsRQType>(CVarAllowedGameRQType_M.GetValueOnGameThread())) &&
//		!IsRQTypeOneOfExtraContentScale(static_cast<EB2GraphicsRQType>(CVarAllowedGameRQType_L.GetValueOnGameThread()))
//	);
//#endif
//#endif
//
//	// �Ϻ� RQType ���� ��Ÿ�� switching �� �� ����� �ķ� �̷�� ���� ó�� ���� ������ ��.
//	{
//		int32 LoadedResolutionLevelInt = B2ResolutionLevelToInt(EB2ResolutionLevel::Mid);
//		LoadGameSetting_Resolution_OrByDefault(LoadedResolutionLevelInt);
//		const EB2ResolutionLevel ReservedResLevel = LoadReservedResolutionLevelOfPrevAppRun(); // ������ ����� �ػ� ������ �־��ٸ� ���� ������� ���� ������ ���̹Ƿ� �̰͵� �˻�.
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
	//// MobileContentScaleFactor ������ ���� �� �ȸ�����. (B2CLT-3374) ���� ���� �ذ� ������ �ӽ� ����. 
	//bool bHasContentScaleRQType = false;
	//for (int32 RLI = 0; RLI < B2ResolutionLevelToInt(EB2ResolutionLevel::End); ++RLI)
	//{
	//	EB2GraphicsRQType ThisMappedRQType = GetSafeMappedRQTypeOfResolutionLevel(IntToB2ResolutionLevel(RLI));
	//	if (IsRQTypeOneOfExtraContentScale(ThisMappedRQType))
	//	{
	//		bHasContentScaleRQType = true; // MCSF �� �����ϴ� ������ �ִ� ���. ���� �ȵ���̵� ���ķ����Ϳ��� ��.
	//		break;
	//	}
	//}
	//if (bHasContentScaleRQType)
	//{
	//	static IConsoleVariable* CVarMCSF = GetMCSFCVar();
	//	if (CVarMCSF)
	//	{
	//		const float WantedFactorValue = CVarMCSF->GetFloat();

	//		// �� Ȳ���� �ڵ�� ���̴µ� ��� ������Ʈ�� �ȵǴ� ���� �ִ��� DeviceProfile �� ó�� ������ ���ϰ� �ٸ��� �ε� ȭ�� �Ŀ� ȭ�� ũ�Ⱑ �ȸ´� ���� �߻��ؼ�..
	//		// ���� SceneRenderTarget ���� ũ�Ⱑ �� ó�� ������ ������ ���ư��� �Ǵ� ��찡 �ִ�.

	//		if (bForceInvalidateResCache)
	//		{ 
	//			// �׳� ���Ƿ� MCSF ���� �ٸ��� �־ ���� �ػ� ����Ī�� ���� ����Ÿ�� ���� ������Ʈ�� Ȯ���� �ǵ��� �ϱ� ����.
	//			// FAndroidWindow::GetScreenRect ����. MCSF �� �����ؼ� ���� ������ �����Ͱ� �����Ǵ� ��Ȳ������ �߰��� �׳� ������ �� ����. 
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
		// ���� ��ϵ� �ɼ� ���ð��� ����Ͽ� ���Ӹ�� ��ü������ Ư���� �߰� �ɼ� ����..
		StartingGM->ApplyGameModeSpecificScalabilitySetting();
	}
#if PLATFORM_ANDROID
	//AndroidMCSFUpdateHack(); ���� ���� �� �ص� �� �� ����..
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
/* EB2GraphicsLevel �� Scalability �������� ������ 
EB2FrameLimitLevel �̳� EB2ResolutionLevel �� ���� ������ ���� �����ϴ�.*/
/************************************************************************/
int32 GetScalabilityLevelOfGraphicsLevel(EB2GraphicsLevel InGraphicsLevel)
{
	// UI ���ÿ� ���� Scalability �ý��� ��ü���� ���� ���� ��.. ������ ������ �ִ� Ư�� �׸� �ܿ��� ���⼭ ���� ���� ����.
	int32 RetVal = 0;
	GConfig->GetInt(TEXT("ScalabilityLevelByUserSelection"), *GetScalabilityKeyNameOfGraphicsLevel(InGraphicsLevel), RetVal, GScalabilityIni);
	return RetVal;
}

int32 GetTextureLevelOfGraphicsLevel(EB2GraphicsLevel InGraphicsLevel)
{
	// UI ���ÿ� ���� TextureQuality ���� ���� ��.. ������ ������ �ִ� Ư�� �׸�.. �ؽ��Ķ� �ػ󵵴� ���� ���� ������ ������ ��찡 ���Ƽ� ���� ��� �ʿ䰡 �ִ�.
	int32 RetVal = 0;
	GConfig->GetInt(TEXT("TextureQualityLevelByUserSelection"), *GetScalabilityKeyNameOfGraphicsLevel(InGraphicsLevel), RetVal, GScalabilityIni);
	return RetVal;
}

float GetResolutionScaleOfGraphicsLevel(EB2GraphicsLevel InGraphicsLevel)
{
	// UI ���ÿ� ���� ResolutionQuality ���� ���� ��.. ������ ������ �ִ� Ư�� �׸�.. �ؽ��Ķ� �ػ󵵴� ���� ���� ������ ������ ��찡 ���Ƽ� ���� ��� �ʿ䰡 �ִ�.
	float RetVal = 100.0;
	GConfig->GetFloat(TEXT("ResolutionQualityByUserSelection"), *GetScalabilityKeyNameOfGraphicsLevel(InGraphicsLevel), RetVal, GScalabilityIni);
	return RetVal;
}
int32 GetGameplayOnlyLODQualityOfGraphicsLevel(EB2GraphicsLevel InGraphicsLevel)
{
	// UI ���ÿ� ���� Scalability �ý��� ��ü���� ���� ���� ��.. ������ ������ �ִ� Ư�� �׸�..
	int32 RetVal = 0;
	GConfig->GetInt(TEXT("GameplayOnlyLODQualityByUserSelection"), *GetScalabilityKeyNameOfGraphicsLevel(InGraphicsLevel), RetVal, GScalabilityIni);
	return RetVal;
}

/**
 * GetScalabilityLevelOfGraphicsLevel �̶� �ݴ���ε�, ���� �� �ִ� Scalability ���鿡 ����ؼ� ��ü�� ��Ī�Ǵ� EB2GraphicsLevel �� ����
 * ����� ������ ���� ���� ��Ȳ���� DeviceProfile �� �� Scalability �� ����ؼ� ����� ���� GraphicsLevel �� �����Ϸ��� ��.
 */
EB2GraphicsLevel GetGraphicsLevelByTotalScalability(const Scalability::FQualityLevels& InScalabilityLevel)
{
	//int32 TotalAdded =
	//	InScalabilityLevel.AntiAliasingQuality +
	//	InScalabilityLevel.EffectsQuality +
	//	InScalabilityLevel.PostProcessQuality +
	//	InScalabilityLevel.PostProcessExtraQuality +
	//	//InScalabilityLevel.ResolutionQuality + �̰� ��ġ�� �� �ٸ��� ����..
	//	InScalabilityLevel.TextureQuality +
	//	InScalabilityLevel.ShadowQuality +
	//	InScalabilityLevel.ViewDistanceQuality +
	//	InScalabilityLevel.GameplayOnlyLODQuality;
	//// Ȥ���� �߰��� Scalability �׸� ���� ó���� �ʿ��ϱ� �ѵ� �׷��� �����ϰԱ��� �� �ʿ�� ���� ��.
	//// �̰� int ĳ���ú��ٴ� Round �� ������ �� �ϴ�. ���� ����� ���� ã�� �� ����.
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
	// ���� �õ� �� �ϴ� ��� ���� �� �̿� ���� ����..
	// �̷� ó�� ���� �ܼ��� Scalability::GetQualityLevels �� �������� GameMode �� ���� override �� ����� �� ����ϰ� �� �� �����Ƿ� ����ġ �ʴ�.
	UDeviceProfileManager::InitializeCVarsForActiveDeviceProfile();

	return GetGraphicsLevelByTotalScalability(Scalability::GetQualityLevels());
}

/** GameplayOnlyLODQuality ���� ���� �ٷ� ����Ƿ��� �߰� �ڵ鸵�� �� �ʿ��ؼ�.
* ���� Scalability �ʿ��� ��� �� ������ ������ World �� �ʿ��ؼ� �׳� ���� ���� �����ϴ� �Ϳ� ���ؼ��� ���� ���Ѵ�.
* sg. �ܼ� ����� ���� ���游 �ٷ� �ڵ鸵�� �ȵ� �� ���� �޴��� ���� ������ �ٷ� �ڵ鸵 �� ��. */
void OnGameplayOnlyLODQualityChanged(UWorld* InWorld, int32 InNewValue)
{
	if (!InWorld) {
		return;
	}

	for (FActorIterator ItPlayer(InWorld); ItPlayer; ++ItPlayer)
	{
		ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(*ItPlayer);
		if (CastedPlayer)
		{ // Ȥ���� ���� �÷��� �����̶� �ϴ���.. ���� ���߿��� ��κ� BladeIIPlayer �� �ƴ� SkeletalMeshActor �� ���ǹǷ� �̰��� ȣ���ϴ� �� ������ ��ġ�� �ʴ´�. �ƴ� ���� �ϴ��� ���� ���߿� pause �޴��� �������� �ʰ�..
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
	// Extra/LowerScreenScale~~ RQType �� ���ε� ini �� ���ǵ� ���� ��ġ ������.
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
		check(RetScaleValue >= 1.0f); // �̰� �ƴϸ� �ǵ��� �ٸ��� ������ �� ��.
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
	// �׽�Ʈ �� CVar �� Ȱ��ȭ �� ��Ȳ.
	if (CVarB2DevForcedGraphicsRQType.GetValueOnGameThread() >= 0) {
		InRQType = static_cast<EB2GraphicsRQType>(CVarB2DevForcedGraphicsRQType.GetValueOnGameThread());
	}
#endif

	// EB2GraphicsRQType::ExtraScreenScale �� ���� ����
	if (IsRQTypeOneOfLowerScreenScale(InRQType) || IsRQTypeOneOfExtraScreenScale(InRQType))
	{
		float ScaleValue = GetScreenScaleRQTypeScaleValue(InRQType);
		InOutResolutionQuality *= ScaleValue;
	}
}

float GetContentScaleRQTypeScaleValue(EB2GraphicsRQType InRQType)
{ // ExtraContentScale~~ RQType �� ���ε� ini �� ���ǵ� ���� ��ġ ������.
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
//		// �׽�Ʈ �� CVar �� Ȱ��ȭ �� ��Ȳ.
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
//		// Ini �������� �׳� �����ϴ� �� �ƴ϶� ���� �ʱⰪ Ȥ�� DeviceProfile �� ���� ���ǵ� ���� �߰� �������� ���Ѵ�.
//		const float WantedValue = CachedBaseMobileContentScaleFactor * ScaleValue;
//		// �� �ٲ���� ���� Set �Ϸ��� ���� ���� �� ���ϴ� ��� �������� �ʴ� �� ���� ���� MCSF �� �����ϴ� RQType �� ���� �׻�
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
	// �⺻ Scalability �ý����� AntiAliasingQuality �� �ƿ� �����ϴ� �� �ƴ����� ���� ��Ʈ���� �� �ʿ�..

	static IConsoleVariable* _CVarMobileMSAA = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileMSAA"));
	check(_CVarMobileMSAA);

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	// �׽�Ʈ �� CVar �� Ȱ��ȭ �� ��Ȳ.
	if (CVarB2DevForcedGraphicsRQType.GetValueOnGameThread() >= 0) {
		InRQType = static_cast<EB2GraphicsRQType>(CVarB2DevForcedGraphicsRQType.GetValueOnGameThread());
	}
#endif

	//
	// r.MobileMSAA �� ���Ƿ� �����ؼ� �׽�Ʈ�� �ϰ� �ʹٸ� ���� �ڵ带 (�ӽ÷�) �����ϵ���, �ƴϸ� ���⿡ �߰� ��Ʈ���� �ִ���.
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
		// ���� PostProcessAA ���� �߰��Ǹ� �߰� CVar �� �����ϰ� �� ��.
		// �׷� AntiAliasingQuality �� �����ϰ� �� ���� �ְ�.

		if (_CVarMobileMSAA)
		{
			_CVarMobileMSAA->Set(1);
		}
	}
}

/**
* UI ���� ������ EB2GraphicsLevel �� ���� ���� Scalability �ý��� ���� ����.
* Scalability �� ���� �������� ��Ʈ���� Scalability.ini �� ���Ĵ� �����������..
* BladeII ���� �� Scalability ���� ����� ���� �Լ� ��. (100% Ŀ���� �ƴ�)
*/
void AdjustScalabilityBySelectedLevel(UObject* WorldContextObject, EB2GraphicsLevel InOverallGraphicsLevel, EB2ResolutionLevel InResolutionLevel, bool bExtraContentScaleFallback)
{
//	Scalability::FQualityLevels QualityLevel = Scalability::GetQualityLevels();
//
//	const int32 PrevGameplayOnlyLODQuality = QualityLevel.GameplayOnlyLODQuality;
//
//#if !UE_BUILD_SHIPPING
//	const Scalability::FQualityLevels ForLevelCountRef = Scalability::GetQualityLevelCounts();
//	// ini �� ������ �ֱ� ������ �ٲ�� �� ���� ���ϸ� ����ϴ�.
//	check(ForLevelCountRef.AntiAliasingQuality == 5 && ForLevelCountRef.EffectsQuality == 5 && ForLevelCountRef.PostProcessQuality == 5 && ForLevelCountRef.PostProcessExtraQuality == 5 && ForLevelCountRef.TextureQuality == 5 && ForLevelCountRef.ShadowQuality == 5 && ForLevelCountRef.GameplayOnlyLODQuality == 5);
//
//	check(bOnModuleStartupCalled); /* �Ǽ��� ������ �ٲ�� �� ���� ���� */
//#endif
//	
//	// �⺻�����δ� GetScalabilityLevelOfGraphicsLevel �� ���� ���ε� ���� ������ ���� ������ ���� ī�װ��� �ִ�.
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
//	// UI �� ����� �������̽��� EB2ResolutionLevel �� ���� ��⺰�� ���ε� ���� �ĺ����� EB2GraphicsRQType �� �����ͼ� ���
//	EB2GraphicsRQType MappedRQType = GetSafeMappedRQTypeOfResolutionLevel(InResolutionLevel);
//	if (bExtraContentScaleFallback)
//	{ // GetSafeMappedRQTypeOfResolutionLevel ������ ������ ��Ȳ�� ���� ContentScale �� ScreenScale �� �ٲٴ� ������ ������ �߰��� �ܺο��� �ƽθ� ������ �����ϴ� ��찡 �� ����.
//		MappedRQType = GetContentScaleRQTypeFallback(MappedRQType);
//	}
//
//	QualityLevel.AntiAliasingQuality = DesiredScalabilityLevel;
//	SetAntiAliasingByB2RQType(MappedRQType); // MSAA �� �ִٸ� ���� Scalability �ʹ� �������.
//
//	QualityLevel.ResolutionQuality = GetResolutionScaleOfGraphicsLevel(InOverallGraphicsLevel);
//	AdjustResolutionQualityByRQType(MappedRQType, QualityLevel.ResolutionQuality);
//	
//	AdjustMobileContentScaleFactorByRQType(MappedRQType); // ���� ���� Scalability �ʹ� �������.
//
//	Scalability::SetQualityLevels(QualityLevel, true /*���� ����� ���� ���ؼ��� Set ������ �Ͼ���� ���ڸ� �־���*/);
//	// Scalability ��ü�� ini ���嵵 �ؾ� �ϴµ� ������ ���ð� ���常���δ� ���� �ʱ�ȭ ������ ������ ���� �������� ���ϱ� �����̴�.
//	// �̷��� ������ ���� ������ �Ź� ���� �����ϸ鼭 ������ �Ȱ��� �ʱⰪ���� �����ߴٰ� �κ� �߰� ���� ����� ������ �����ϰ� �ϴ� ���� �ݺ��� ��.
//	Scalability::SaveState(GIsEditor ? GEditorSettingsIni : GGameUserSettingsIni);
//	
//	if (WorldContextObject)
//	{
//		// ���Ӹ�� ���� Ư���� ������.. ���� ��� � ���Ӹ��� dynamic shadow �� �� ���ٰų�, LOD �� ���� ���缭 ���ٰų� ��..
//		// �̰� ����ϴ� ���� ��ü�� �κ������ �ٸ� ���Ӹ�忡���� ���� �޴��� ������ ���� �ִ�.
//		ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
//		if (B2GM) 
//		{
//			B2GM->ApplyGameModeSpecificScalabilitySetting(InOverallGraphicsLevel, InResolutionLevel);
//
//			// GameplayOnlyLODQuality �ٲ� �Ϳ� ���ؼ��� ���� �ڵ鸵�� ��.. ���⼭��
//			const int32 CurrGameplayOnlyLODQuality = Scalability::GetQualityLevels_GameplayOnlyLOD();
//			if (CurrGameplayOnlyLODQuality != PrevGameplayOnlyLODQuality)
//			{
//				OnGameplayOnlyLODQualityChanged(B2GM->GetWorld(), CurrGameplayOnlyLODQuality);
//			}
//		}
//	}
}

void AdjustScalabilityBySavedUserSelection(UObject* WorldContextObject) // ���� �ʿ��Ѱ�..?
{
	/*int32 SavedGraphicsLevel = B2GraphicsLevelToInt(EB2GraphicsLevel::GraphicsLevel_HIGH);
	LoadGameSetting_Graphics_OrByDefault(SavedGraphicsLevel);
	int32 SavedResolutionLevel = B2ResolutionLevelToInt(EB2ResolutionLevel::Mid);
	LoadGameSetting_Resolution_OrByDefault(SavedResolutionLevel);
	AdjustScalabilityBySelectedLevel(WorldContextObject, SavedGraphicsLevel, SavedResolutionLevel);*/
}

/** ���Ӹ�� �� ���� ��Ȳ �� BladeII ���� �ý��ۿ� ���� scalability ��з� �� shadow override */
void ApplyBladeIIBiasedScalability_Shadow(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetScalabilityLevelOfGraphicsLevel(InBasicUserSelection); // ��ü�� GetScalabilityLevelOfGraphicsLevel �� �������� ������ ���� �ƴ� �� �־ ���ڷ� EB2GraphicsLevel �� �޴� ��.
	//int32 ShadowQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("ShadowQualityBias"), ShadowQualityBias, GScalabilityIni);

	//// �⺻���� ���� UI �� ���� ���ÿ� ������ Bias ��ŭ �����Ѵ�.
	//// ���� ini ���� �ش� �׸��� �� �о�鿴�ٰ� �ϴ��� 0 ������ �Ʒ� �ڵ带 �����ؼ� DesiredScalabilityLevel �� ����ǵ��� �Ѵ�.
	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - ShadowQualityBias, 0, QualityLevelCountRef.ShadowQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_Shadow() != BiasedNewLevel) {
	//	Scalability::SetQualityLevels_Shadow(BiasedNewLevel);
	//}
}
/** ���Ӹ�� �� ���� ��Ȳ �� BladeII ���� �ý��ۿ� ���� scalability ��з� �� effect override */
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
/** ���Ӹ�� �� ���� ��Ȳ �� BladeII ���� �ý��ۿ� ���� scalability ��з� �� ViewDistance override */
void ApplyBladeIIBiasedScalability_ViewDist(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName, 
	bool bApplyHigherOnly = false /* Ư���� override ��Ȳ������ ���. */
)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetScalabilityLevelOfGraphicsLevel(InBasicUserSelection);
	//int32 ViewDistanceQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("ViewDistanceBias"), ViewDistanceQualityBias, GScalabilityIni);

	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - ViewDistanceQualityBias, 0, QualityLevelCountRef.ViewDistanceQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_ViewDistance() != BiasedNewLevel
	//	&&
	//	(!bApplyHigherOnly || Scalability::GetQualityLevels_ViewDistance() < BiasedNewLevel) // ApplyHigherOnly ������ ���� scalability ������ ���� �ʴ´�.
	//	) {
	//	Scalability::SetQualityLevels_ViewDistance(BiasedNewLevel);
	//}
}
/** ���Ӹ�� �� ���� ��Ȳ �� BladeII ���� �ý��ۿ� ���� scalability ��з� �� PostProcess override */
void ApplyBladeIIBiasedScalability_PostProcess(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName,
	bool bApplyHigherOnly = false /* Ư���� override ��Ȳ������ ���. */
)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetScalabilityLevelOfGraphicsLevel(InBasicUserSelection);
	//int32 PostProcessQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("PostProcessQualityBias"), PostProcessQualityBias, GScalabilityIni);

	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - PostProcessQualityBias, 0, QualityLevelCountRef.PostProcessQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_PostProcess() != BiasedNewLevel
	//	&&
	//	(!bApplyHigherOnly || Scalability::GetQualityLevels_PostProcess() < BiasedNewLevel) // ApplyHigherOnly ������ ���� scalability ������ ���� �ʴ´�.
	//	) {
	//	Scalability::SetQualityLevels_PostProcess(BiasedNewLevel);
	//}
}
void ApplyBladeIIBiasedScalability_PostProcessExtra(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName,
	bool bApplyHigherOnly = false /* Ư���� override ��Ȳ������ ���. */
)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetScalabilityLevelOfGraphicsLevel(InBasicUserSelection);
	//int32 PostProcessExtraQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("PostProcessExtraQualityBias"), PostProcessExtraQualityBias, GScalabilityIni);

	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - PostProcessExtraQualityBias, 0, QualityLevelCountRef.PostProcessExtraQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_PostProcessExtra() != BiasedNewLevel
	//	&&
	//	(!bApplyHigherOnly || Scalability::GetQualityLevels_PostProcessExtra() < BiasedNewLevel) // ApplyHigherOnly ������ ���� scalability ������ ���� �ʴ´�.
	//	) {
	//	Scalability::SetQualityLevels_PostProcessExtra(BiasedNewLevel);
	//}
}
/** ���Ӹ�� �� ���� ��Ȳ �� BladeII ���� �ý��ۿ� ���� scalability ��з� �� Texture override */
void ApplyBladeIIBiasedScalability_Texture(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetTextureLevelOfGraphicsLevel(InBasicUserSelection); // GetScalabilityLevelOfGraphicsLevel �� �ƴ� ���� ��������
	//int32 TextureQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("TextureQualityBias"), TextureQualityBias, GScalabilityIni);

	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - TextureQualityBias, 0, QualityLevelCountRef.TextureQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_Texture() != BiasedNewLevel) {
	//	Scalability::SetQualityLevels_Texture(BiasedNewLevel);
	//}
}
/** ���Ӹ�� �� ���� ��Ȳ �� BladeII ���� �ý��ۿ� ���� scalability ��з� �� ResolutionQuality override */
void ApplyBladeIIBiasedScalability_Resolution(EB2GraphicsLevel InBasicUserSelection, EB2GraphicsRQType InRQType, const FString& InIniSectionName)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const float DesiredDefaultResolutionQuality = GetResolutionScaleOfGraphicsLevel(InBasicUserSelection);
	//float ResolutionQualityBias = 0; // ���� int �� �ƴ�
	//GConfig->GetFloat(*InIniSectionName, TEXT("ResolutionQualityBias"), ResolutionQualityBias, GScalabilityIni);

	//float BiasedNewQuality = FMath::Clamp(DesiredDefaultResolutionQuality - ResolutionQualityBias, 10.0f, 100.0f);
	//// ���� ExtraScreenScale �� ���Ѵٸ� Bias ���� ���İ� �Ǿ��.
	//AdjustResolutionQualityByRQType(InRQType, BiasedNewQuality);

	//if (Scalability::GetQualityLevels_Resolution() != BiasedNewQuality) {
	//	Scalability::SetQualityLevels_Resolution(BiasedNewQuality);
	//}
}
/** ���Ӹ�� �� ���� ��Ȳ �� BladeII ���� �ý��ۿ� ���� scalability ��з� �� GameplayOnlyLOD override */
void ApplyBladeIIBiasedScalability_GameplayOnlyLOD(EB2GraphicsLevel InBasicUserSelection, const FString& InIniSectionName)
{
	//const Scalability::FQualityLevels QualityLevelCountRef = Scalability::GetQualityLevelCounts();

	//const int32 DesiredScalabilityLevel = GetGameplayOnlyLODQualityOfGraphicsLevel(InBasicUserSelection); // GetScalabilityLevelOfGraphicsLevel �� �ƴ�.
	//int32 GameplayOnlyLODQualityBias = 0;
	//GConfig->GetInt(*InIniSectionName, TEXT("GameplayOnlyLODQualityBias"), GameplayOnlyLODQualityBias, GScalabilityIni);

	//int32 BiasedNewLevel = FMath::Clamp(DesiredScalabilityLevel - GameplayOnlyLODQualityBias, 0, QualityLevelCountRef.GameplayOnlyLODQuality - MaxPossibleQualityLevelSubtract);
	//if (Scalability::GetQualityLevels_GameplayOnlyLOD() != BiasedNewLevel) {
	//	Scalability::SetQualityLevels_GameplayOnlyLOD(BiasedNewLevel);
	//}
}

/** StageEventDirector �� BeginShow/FinishShow Ȥ�� �׿� �ش��ϴ� �̺�Ʈ����.. */
void ApplyStageEventSpecificScalabilitySetting(UObject* WorldContextObject, bool bIsShowOn)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("ApplyStageEventSpecificScalabilitySetting %d"), (int32)bIsShowOn));
//
//#if !UE_BUILD_SHIPPING
//	if (!bUseBladeIIScalabilityOverride) { // �׽�Ʈ ������ ���� ��Ȳ�� ���� override �� �� �ֵ���. ��� LOD ����� ������.
//		AdjustScalabilityBySavedUserSelection(WorldContextObject);
//		return;
//	}
//#endif
//
//	// ���⼭ WorldContextObject �� StageEventDirector �� �� ���ɼ��� ũ����.. �ʿ��ϸ� ĳ�����ؼ� �� ������..?
//	// ���� �������δ� ���Ӹ�忡 ���� ������ ��� ��ǻ� �߰����� PerGMSetting ���� ���� �� ��.
//	// ���Ӹ�� ���� ����� scalability ���� ���� ������ �� �����ϱ� �ϰ����� �װ� �ʿ��ұ� �ͱ⵵ �ϰ�.
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
//		// ���⼭ �߰��� ��Ʈ���ϴ� �׸���� �翬�� ��Ÿ�� ������ ������ ���̾�� �Ѵ�.
//		//
//
//		// Ư�� ���� ��Ȳ������ �� ���̴� �� ����� �� �ִ� ViewDistance �� PostProcess..?
//		// GameMode override ���� ���� ������ �ʵ��� ��.
//		ApplyBladeIIBiasedScalability_ViewDist(UserSelectedLevel, StageEventSettingSection, true);
//		ApplyBladeIIBiasedScalability_PostProcess(UserSelectedLevel, StageEventSettingSection, true);
//		ApplyBladeIIBiasedScalability_PostProcessExtra(UserSelectedLevel, StageEventSettingSection, true);
//
//		//
//		// GameplayOnlyLODQuality �� ���⼭ �߰� override �� ���� �ʰ� �ƿ� �װ� ��ü�� ����� ���� �׸��� �ִ�.
//		// LOD ������ ���� ���� ���� �� �� �پ��� �̺�Ʈ�� ���� ���� �ؼ�.
//		//
//	}
//	else
//	{ // ���� ������ �⺻ ��Ȳ���� �ǵ���. ���� �̷��� Ư���� scalability �� �����ϴ� ��찡 �� ����� �̰ɷ� �� �ǰ���.
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

/** ApplyGameModeSpecificScalabilitySetting �������̵� �� �Լ����� �ᱹ ��� �̰� ���Ѵ�.. ini �� �ڽſ� �ش��ϴ� �׸�� �Բ� */
void ApplyGameModeSpecificScalabilitySetting_OverrideCommon(ABladeIIGameMode* InB2GM, EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
//#if !UE_BUILD_SHIPPING
//	if (!B2Scalability::bUseBladeIIScalabilityOverride) { // �׽�Ʈ ������ ���Ӹ�忡 ���� override �� �� �ֵ���
//		return;
//	}
//#endif
//
//	if (!InB2GM) {
//		return;
//	}
//
//	EB2GraphicsLevel UsedGraphicsLevel = InSelectedGraphicsLevel;
//	if (UsedGraphicsLevel == EB2GraphicsLevel::GraphicsLevel_End) // �������� ���� ��� UI ������ ���� ini ����� �� ���.
//	{
//		int32 SavedGraphicsLevel = B2GraphicsLevelToInt(EB2GraphicsLevel::GraphicsLevel_HIGH);
//		// �� ������ GameSetting �� �ʱ�ȭ�� �� �Ǿ� ������ ���⼭ override �� scalability �� �ƴ� DeviceProfile �� ���� ������ scalability �� GraphicsLevel �� �����ϰ� �� ��.
//		LoadGameSetting_Graphics_OrByDefault(SavedGraphicsLevel);		
//		UsedGraphicsLevel = IntToB2GraphicsLevel(SavedGraphicsLevel);
//	}
//
//	EB2ResolutionLevel UsedResLevel = InSelectedResLevel;
//	if (UsedResLevel == EB2ResolutionLevel::End) // ���������� �������� ���� ���.. ini ����� ������
//	{
//		int32 SavedResLevel = B2ResolutionLevelToInt(EB2ResolutionLevel::Mid);
//		LoadGameSetting_Resolution_OrByDefault(SavedResLevel);
//		UsedResLevel = IntToB2ResolutionLevel(SavedResLevel);
//	}
//	
//	//
//	// ���⼭ �߰��� ��Ʈ���ϴ� �׸���� �翬�� ��Ÿ�� ������ ������ ���̾�� �Ѵ�.
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
/* EB2GraphicsLevel �� Scalability ���� ������ �� ����������
EB2FrameLimitLevel �̳� EB2ResolutionLevel �� ������ ���� �ܼ�.*/
/************************************************************************/
IConsoleVariable* GetMappedCVarOfFrameLimitLevel(EB2FrameLimitLevel InRelativeLevel)
{
	// EB2FrameLimitLevel �� �ش��ϴ� CVar �� ��������.
	IConsoleVariable* FoundCVar = IConsoleManager::Get().FindConsoleVariable
	(
		(InRelativeLevel == EB2FrameLimitLevel::High) ? TEXT("r.DefinedMobileFrameLimitQuality_H") : (
		(InRelativeLevel == EB2FrameLimitLevel::Low) ? TEXT("r.DefinedMobileFrameLimitQuality_L") :
			TEXT("r.DefinedMobileFrameLimitQuality_M")
			)
	);
	check(FoundCVar); // ���� �̸��� �ٲ������..
	return FoundCVar;
}

/** �����ӷ� ������ ��� ���� ������ ���� ������ �д�. 
 * DeviceProfile �� ���� ���� ��ġ ����ؼ� ���� ������ �������̽��� ����. 
 * ���� ��� ������S6 �� S8 �� ��⿡�� ������ EB2FrameLimitLevel ������ ������ ���� ������ ���� ��ġ�� �޶����� �Ǵ� ��. */
void SetRelativeFrameLimitLevel(EB2FrameLimitLevel InRelativeLevel)
{
	IConsoleVariable* CVarDefinedLevel = GetMappedCVarOfFrameLimitLevel(InRelativeLevel);
	IConsoleVariable* CVarCurrentLevel = IConsoleManager::Get().FindConsoleVariable(TEXT("r.CurrentMobileFrameLimitLevel"));

	check(CVarDefinedLevel && CVarCurrentLevel); // �� �� �ϳ��� ���� �̸��� �ٲ������..
	if (CVarDefinedLevel && CVarCurrentLevel)
	{
		// DeviceProfile �� ���� �������� ���� ��ġ�� ���� FrameLimit �� ���Ǵ� �ܼ� ������ ����
		CVarCurrentLevel->Set(CVarDefinedLevel->GetInt());
	}

}

EB2FrameLimitLevel GetRelativeFrameLimitLevelInternal(int32 CheckFrameLimitLevel)
{
	IConsoleVariable* CVarDefinedLevel = GetMappedCVarOfFrameLimitLevel(EB2FrameLimitLevel::Mid); // Mid �� ��������.
	check(CVarDefinedLevel);
	if (CVarDefinedLevel)
	{
		const int32 DefinedLevelInt = CVarDefinedLevel->GetInt();

		// ������ ���� DefinedMobileFrameLimitQuality �� L �̳� H �� M �� ������ ������ ���� �� ���� �ִµ� ���ϰ� Mid �� ���� �� ��.
		// �׷� ���� �ֽô��� �ش� ������ ���� ������ �������� �ʾƾ�..
		return (CheckFrameLimitLevel > DefinedLevelInt) ? EB2FrameLimitLevel::High
			: ((CheckFrameLimitLevel < DefinedLevelInt) ? EB2FrameLimitLevel::Low : EB2FrameLimitLevel::Mid);

	}
	return EB2FrameLimitLevel::Mid;
}
EB2FrameLimitLevel GetCurrentRelativeFrameLimitLevel()
{
	IConsoleVariable* CVarCurrentLevel = IConsoleManager::Get().FindConsoleVariable(TEXT("r.CurrentMobileFrameLimitLevel"));
	check(CVarCurrentLevel); // ���� �̸��� �ٲ������..
	if (CVarCurrentLevel)
	{
		return GetRelativeFrameLimitLevelInternal(CVarCurrentLevel->GetInt());
	}
	return EB2FrameLimitLevel::Mid;
}
EB2FrameLimitLevel GetDefaultRelativeFrameLimitLevel()
{
	IConsoleVariable* CVarDefaultLevel = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefinedMobileFrameLimitQuality_Default"));
	check(CVarDefaultLevel); // ���� �̸��� �ٲ������..
	if (CVarDefaultLevel)
	{
		return GetRelativeFrameLimitLevelInternal(CVarDefaultLevel->GetInt());
	}
	return EB2FrameLimitLevel::Mid;
}

bool IsRelativeFrameLimitLevelAvailable(EB2FrameLimitLevel InCheckLevel)
{
	// ���� �������� �� ���� EB2FrameLimitLevel �� Ư�� �������� ������ �������� üũ�ϴ� ��.
	// �Ұ����ϴٸ� �� ���� ���� SetRelativeFrameLimitLevel �� ��ǻ� �� ��������..

	if (InCheckLevel == EB2FrameLimitLevel::Mid) {
		return true; // ��Ģ������ Mid �� ���⼭ üũ�ϴ� ������ �ǰ� ���� ������ true.
	}

	IConsoleVariable* CVarMidLevel = GetMappedCVarOfFrameLimitLevel(EB2FrameLimitLevel::Mid);
	IConsoleVariable* CVarCheckLevel = GetMappedCVarOfFrameLimitLevel(InCheckLevel);
	check(CVarMidLevel && CVarCheckLevel); // ���� �̸��� �ٲ������..
	if (CVarMidLevel && CVarCheckLevel)
	{
		if (
			InCheckLevel == EB2FrameLimitLevel::High &&
			CVarCheckLevel->GetInt() > CVarMidLevel->GetInt() // High �� ���� ������ Mid ���� ���ƾ� ���������� ������ �����Ǵ� ������ ����.
			)
		{
			return true;
		}
		else if (
			InCheckLevel == EB2FrameLimitLevel::Low &&
			CVarCheckLevel->GetInt() < CVarMidLevel->GetInt() && // Low �� ���� ������ Mid ���� ���ƾ� ���������� ������ �����Ǵ� ������ ����.
			CVarCheckLevel->GetInt() >= 0 // ��, ���⼭�� ������ �ϳ� �� �ִµ� 0 ���� ���Ƽ��� �� �ǰ���.
			)
		{
			return true;
		}
	}
	return false;
}

IConsoleVariable* GetMappedCVarOfResolutionLevel(EB2ResolutionLevel InLevel)
{
	// EB2FrameLimitLevel �� �ش��ϴ� CVar �� ��������.
	IConsoleVariable* FoundCVar = IConsoleManager::Get().FindConsoleVariable
	(
		(InLevel == EB2ResolutionLevel::High) ? TEXT("r.AllowedGameRQType_H") : (
		(InLevel == EB2ResolutionLevel::Low) ? TEXT("r.AllowedGameRQType_L") :
			TEXT("r.AllowedGameRQType_M")
			)
	);
	check(FoundCVar); // ���� �̸��� �ٲ������..
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
	// ����̽� ȯ�� ������ ���� �� �ٸ��� ������ �� �־.. Safe ��⺸�ٴ� �������� �� ��ü ����.
	IConsoleVariable* AndroidWindowSizeXCvar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DetectedAndroidDeviceWindowSizeX"));
	IConsoleVariable* AndroidWindowSizeYCvar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DetectedAndroidDeviceWindowSizeY"));
	check(AndroidWindowSizeXCvar && AndroidWindowSizeYCvar);
	if (IsRQTypeOneOfExtraContentScale(SimpleMappedRQType) &&
		AndroidWindowSizeXCvar && AndroidWindowSizeYCvar &&
		AndroidWindowSizeXCvar->GetInt() > 0 && AndroidWindowSizeYCvar->GetInt() > 0)
	{
		// ǥ�� �ػ󵵸� ���� ���ϴ� ȭ���̸� MobileContentScaleFactor �� �Ἥ �̵��� �� �� ����.
		// ǥ�� �ػ󵵸� �Ѵ´ٰ� �ص� �� ���� ������ ���ó� �״���..

		// ���� ���ǵ� ������ ��.. �� ������ �̵��� �� ���� �ػ����� üũ
		// IOS �� �ƴ� �� ������ Android �� �� ���� �״�� MCSF �� ���ٰ� ���� ������ ��.
		const float DefinedContentScaleValue = GetContentScaleRQTypeScaleValue(SimpleMappedRQType);
		// ���� �ػ󵵿��� �� �����ŭ ũ�� ContentScale �� �����ϱ� ����� �������� �Ǵ��ϴ� ��.
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
	// ��Ȯ�� EB2ResolutionLevel �� �ϳ��� ���ε��� �ʴ� RQType �� ���� ���ɼ��� �����ؼ�.. 
	// ���⵵ FrameLimitLevel ó�� enum �׸� ���� ũ��� ���� ���� ������������ �Ǵ��Ѵ�.

	IConsoleVariable* CVarDefinedHighLevelRQ = GetMappedCVarOfResolutionLevel(EB2ResolutionLevel::High);
	IConsoleVariable* CVarDefinedMidLevelRQ = GetMappedCVarOfResolutionLevel(EB2ResolutionLevel::Mid);
	IConsoleVariable* CVarDefinedLowLevelRQ = GetMappedCVarOfResolutionLevel(EB2ResolutionLevel::Low);
	check(CVarDefinedHighLevelRQ && CVarDefinedMidLevelRQ && CVarDefinedLowLevelRQ);
	if (CVarDefinedHighLevelRQ && CVarDefinedMidLevelRQ && CVarDefinedLowLevelRQ && InRQType != EB2GraphicsRQType::End)
	{
		const int32 DefinedHighLevelRQInt = CVarDefinedHighLevelRQ->GetInt();
		const int32 DefinedMidLevelRQInt = CVarDefinedMidLevelRQ->GetInt();
		const int32 DefinedLowLevelRQInt = CVarDefinedLowLevelRQ->GetInt();
		// High == Mid, Mid == Low �� ���� ������ ��� High > Low ����.
		check(DefinedHighLevelRQInt > DefinedLowLevelRQInt);

		const int32 CheckRQTypeInt = static_cast<int32>(InRQType);

		// High == Mid �ų� Mid == Low �� ��� Mid �� �켱�����̹Ƿ� Mid ���� ������ ���� �˻�
		if (CheckRQTypeInt == DefinedMidLevelRQInt) {
			return EB2ResolutionLevel::Mid;
		}

		// High �̻��� ��ġ�� High ��, Low ������ ��ġ�� Low ��, �� �ܿ��� Mid �� ����
		return (CheckRQTypeInt >= DefinedHighLevelRQInt) ? EB2ResolutionLevel::High
			: ((CheckRQTypeInt <= DefinedLowLevelRQInt) ? EB2ResolutionLevel::Low : EB2ResolutionLevel::Mid);
	}
	return EB2ResolutionLevel::Mid;
}
bool IsResolutionLevelAvailable(EB2ResolutionLevel InCheckLevel)
{
	if (InCheckLevel == EB2ResolutionLevel::Mid) {
		return true; // ���ذ�. ������ true.
	}

	const int32 MidRQLevelInt = static_cast<int32>(GetMappedRQTypeOfResolutionLevel(EB2ResolutionLevel::Mid));
	const int32 CheckRQLevelInt = static_cast<int32>(GetMappedRQTypeOfResolutionLevel(InCheckLevel));

	// ���⵵ RQType �� ���� ũ�⿡ ���� ���� ���� Ȥ�� ���� �������� �����ϴ� ��.
	if (InCheckLevel == EB2ResolutionLevel::High)
	{
		return (CheckRQLevelInt > MidRQLevelInt);
	}
	else if (InCheckLevel == EB2ResolutionLevel::Low)
	{
		return (CheckRQLevelInt < MidRQLevelInt);
	}

	check(0); // �̿ܿ� EB2ResolutionLevel �� �߰��� �� �ִµ� ��ó�� ���ߴٸ� �ɸ�����.
	return false;
}

bool DoesRQTypeRequiresAppRestart(EB2GraphicsRQType InCheckRQType)
{
	check(InitialRQTypeOfAppStart != EB2GraphicsRQType::End); // �ƴϸ� ���� �ʱ�ȭ �� �� ���¿��� �Ҹ� ��.
	if (InitialRQTypeOfAppStart != EB2GraphicsRQType::End)
	{
		const bool bInitialRQTypeMCSF = IsRQTypeOneOfExtraContentScale(InitialRQTypeOfAppStart);
		const bool bCheckRQTypeMCSF = IsRQTypeOneOfExtraContentScale(InCheckRQType);
		// �� ������ ���� ù ���� �񱳽�..
		if (
			(bInitialRQTypeMCSF != bCheckRQTypeMCSF) || // MCSF �� �����ϴ� �Ͱ� �ƴ� �� ������ ��ȯ�̰ų�
			(bInitialRQTypeMCSF && bCheckRQTypeMCSF && InitialRQTypeOfAppStart != InCheckRQType) // �� �� MCSF �����ε� �ٸ��ų�
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
���� ��� �� Scalability ���� �������̽� ������ �����ϱ� ���� ���⿡ ���Ƴ���.
************************************************************************/

void ABladeIIGameMode::ApplyGameModeSpecificScalabilitySetting(EB2GraphicsLevel InSelectedGraphicsLevel, EB2ResolutionLevel InSelectedResLevel)
{
	
	// ���� Override �� ���� �ʾ��� ���� �⺻ �������� ���� �⺻������ ���ư� �� �ֵ��� �Ѵ�. 
	//

	// ApplyGameModeSpecificScalabilitySetting �� ���� Override ���� ������ AdjustScalabilityBySelectedLevel �� �״�� ����� �Ͱ� ���������� �ǵ���.

	//EB2GraphicsLevel UsedGraphicsLevel = InSelectedGraphicsLevel;
	//if (UsedGraphicsLevel == EB2GraphicsLevel::GraphicsLevel_End) // �������� ���� ��� UI ������ ���� ini ����� �� ���.
	//{
	//	int32 SavedGraphicsLevel = B2GraphicsLevelToInt(EB2GraphicsLevel::GraphicsLevel_HIGH);
	//	bool bSettingLoaded = LoadGameSetting_Graphics_OrByDefault(SavedGraphicsLevel);
	//	// �� ������ GameSetting �� �ʱ�ȭ�� �� �Ǿ� �ִٰ� �ص� ���⼱ �⺻ scalability �� ���������� ������ �������� �׷��� �ٶ����� ��Ȳ�� �ƴ���.
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
	//	// ��庰 override �� �ƴϴ��� RQType �� ���� �߰� ������ �ʿ�.
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
	return FString(TEXT("ABladeIIGameMode")); // ���Ӹ�� �̸� �״�� �����ϴµ� Class GetName �ϴϱ� �ᱹ Blueprint Ŭ���� �̸��� ���ͼ� �̷���.. �Ѥ�
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
��Ÿ Scalability �� �����ִ� ������ ��ƿ��..
************************************************************************/

static TAutoConsoleVariable<float> CVarB2CameraDistanceAlpha(
	TEXT("r.B2CameraDistanceAlpha"),
	1.0f,
	TEXT("Expected to be in 0.0 ~ 1.0 range, it adjusts the amount of extra camera distance being applied to CameraBoomArmLength.\n"),
	ECVF_Scalability /* �� Scalability ��Ʈ���� ���� �ʴ��� �׷� ������ ���ܵ�. */
);
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
float GDevTestCameraDistanceScale = -1.0f;
#endif
float GetExtraAppliedCameraDistance(float InBaseLength, float InExtraLength)
{ // ������ �ڵ������� �л�Ǿ� �ִ� ���õ��� ������ �� ���� ����ϰ� �����ν� ����� �ϰ����� �����Ѵ�.
	float ScaledDistance = (InBaseLength >= InExtraLength) ? InBaseLength : // ExtraLength �� BaseLength ���� Ŀ�߸� �����Ѵ�.
		FMath::Lerp(InBaseLength, InExtraLength, CVarB2CameraDistanceAlpha.GetValueOnGameThread());
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (GDevTestCameraDistanceScale > 0.0f)
	{ // ������ �׽�Ʈ �� �߰� ������. ExtraLength �� ���� �ȵ� ��Ȳ������ B2CameraDistanceAlpha �� ������� �׽�Ʈ�� �ȵǹǷ�.
		ScaledDistance *= GDevTestCameraDistanceScale;
	}
#endif
	return ScaledDistance;
}