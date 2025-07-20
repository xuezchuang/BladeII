// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIDLCFrontBase.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "B2GameEnum.h"
#include "Json.h"
#include "Interfaces/IBuildPatchServicesModule.h"
#include "Interfaces/IBuildManifest.h"
#include "BuildPatchState.h"
#include "Patcher.h"
#include "B2UIDLCFrontPopup.h"
#include "B2UICheckPermission.h"
#include "B2UIPatchBack.h"
#include "B2UIPatch.generated.h" 

#define  AGW_SERVICE	0

enum class GatewayType : uint8
{
	GT_NONE = 0,
	GT_LOCAL,
	GT_AGW,
	GT_KAKAO,
};

UENUM(BlueprintType)
enum class EPatchPlatformType : uint8
{
	Google = 0,
	GameCenter,
	None,
};

UENUM(BlueprintType)
enum class ELayerType : uint8
{
	ELayerType_AGW = 0,
	ELayerType_DOWNLOAD,
	ELayerType_FILELOAD,
	ELayerType_NONE,
};

class IServiceHelper
{
public:
	virtual ~IServiceHelper() {};

	virtual void    VersionRequest()    = 0;
	virtual void    VersionResponse(FHttpRequestPtr, FHttpResponsePtr, bool) = 0;
	virtual void	Request()			= 0;
	virtual FString GetCDNAddress()		= 0;
};

UCLASS()
class BLADEII_API UB2UIPatch : public UB2UIDLCFrontBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void GameStart();
	void BottomLayerVisible(ELayerType eType);
	void GotoAccessTermUI();

	void PlatformPermission();
	void PatchStart();
	
	void RetryPlatformPermission();
	void GameExit();

	void RetryRequest();

	UFUNCTION(BlueprintImplementableEvent, Category = DLCFront)
		void PlayBackgroundMovie();
	UFUNCTION(BlueprintImplementableEvent, Category = DLCFront)
		void StopBackgroundMovie();

	UFUNCTION(BlueprintImplementableEvent, Category = DLCFront)
		void PlayClassMovieBP();

	UFUNCTION(BlueprintCallable, Category = "DLCFront")
		EPCClass GetSelectPcClass();

	void PatchOnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void ResponseErrorCodePopup(int32 _RessponseCode, ESlateVisibility _visible = ESlateVisibility::Visible, EPopupType _popupType = EPopupType::EPopupType_ErrorRespones, bool _bExitPopup = true);

protected:
	virtual void CacheAssets() override;

private:
	bool HasEnoughStorageSize(const uint64 NeedSize);
	void ShowPopupNotEnoughStorageSize(int64 nVolume);

	bool GetFullDownloadSize(int64 &OutData);
	bool GetResumeDataSize(int64 &OutData);

	UFUNCTION()
	void OnClicked_BlockDownload();
private:
	TWeakObjectPtr<UTextBlock> text_desc;
	TWeakObjectPtr<UTextBlock> text_percent;
	TWeakObjectPtr<UProgressBar> pb_download;

	TWeakObjectPtr<UOverlay> O_Download;
	TWeakObjectPtr<UOverlay> O_AgwCheck;
	TWeakObjectPtr<UOverlay> O_FileLoading;

	TWeakObjectPtr<UB2Button> BTN_BlockDownload;

	TWeakObjectPtr<UB2UICheckPermission> UI_CheckPermission;
	TWeakObjectPtr<UB2UIDLCFrontPopup> UI_MsgPopup01;
	TWeakObjectPtr<UB2UIDLCFrontPopup> UI_MsgPopup02;
	TWeakObjectPtr<UB2UIPatchBack> UIP_PatchBack;

	EMarketType MarketType;
	EPatchPlatformType PlatformType;

	EServiceState m_ServiceState;
	
	FString Version;
	FString GameId;
	FString install_dir_;
	float progress_value_;
	FHttpResponsePtr ResponsePtr;
	bool m_bExitPopup;

	TSharedPtr<UCPatcher> Patcher;
	BuildPatchServices::EBuildPatchState bfPatchState;

	bool m_bPlayMovie;

	TSharedPtr<IServiceHelper>	ServiceHelper;

	void InitGameInfo();
	void OnTapUpdateStartOk();
	void OpenMsgBoxUpdateStart();
	
	int64 GetResumeDataSize(const FString& InStagingDir, const IBuildManifestRef& InBuildManifest);

	void OpenPermissionNotice();

#if PLATFORM_ANDROID
	FDelegateHandle OnCheckPermissionHandler;
	void CheckPermissionDelegate(JNIEnv *env, jobject thiz, jboolean bResult);
#endif
};
