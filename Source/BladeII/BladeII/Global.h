
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
//#include "BladeII.h"
//GLOBAL VAR
extern FString GB2NetworkIni;
#if ENABLE_LOCAL_CUTSCENE_SAVED_STATE
extern FString GB2LocalCutSceneSavedStateIni; // Regarding event scene played state like it was played at least once.
#endif
extern FString GB2NewAcquireStateIni; // For anything that requires showing some "new item available mark".
extern FString GB2GeneralSavedStateIni; // For anything else that need to be saved at local device..
extern FString GB2PreRenderSavedStateIni; // BladeII game content-specific level shader caching state.
extern FString GB2UserSavedStateIni; // For anything else that need to be saved at local device..

extern bool GbLocalSavedStateCleared;

//GLOBAL FUNC
void LoadLocalIniStateOnStartup();
void LoadLocalIniStateOnNetworkInit();
void SaveLocalIniStateOnExit();
void SaveLocalIniStateOnPreLoadMap();
void SaveLocalIniStateOnAppBecomeInactive();

extern FText GetBuildInfo();
extern void SetCDNUrl(FString _CDNUrl);
extern void SetSessionServerUrl(FString _sessionServerUrl);

/** Clear locally saved (ini) data, meant to be used on some stuff like account clearing. */
void ClearLocalIniSavedState();

FString GetPreRenderSavedStateIniName();

void InitializeB2NetworkModule();

void InitB2GameSubsystemsOnStartupModule();
void InitB2GameSubsystemsOnInitB2Network();
void CloseB2GameSubsystemsOnShutdown();

FORCEINLINE bool CanLoadInfoAssetWhileStartupModule()
{
#if WITH_EDITOR
	return GIsEditor && !GMinimalDLCFrontMode;
#else
	return false; // We won't allow InfoAsset loading without game-mode loading, unless we are running editor.
#endif
}

#endif