// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIPatch.h"
#include "B2UIManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "Patcher.h"
#include "IPlatformFilePak.h"
#include "B2DLCFrontGameMode.h"
#include "B2UICheckPermission.h"
#include "B2PlatformAPI.h"
#include "Global.h"
#include "B2Version.h"
#include "BladeIIGameImpl.h"
#if PLATFORM_ANDROID
#include "AndroidRuntimeSettings.h"
#include "Android/AndroidJNI.h"

#elif PLATFORM_IOS
#include "IOSRuntimeSettings.h"
#endif

#include "GenericPlatform/GenericPlatformMisc.h"


const TCHAR* DEV_AGW_URL = TEXT("http://13.124.57.145:9050/agw/index.php?version=%s");

GatewayType StringToGatewayTypeEnum(const FString& s)
{
	//Must Upper
	if (s == TEXT("LOCAL"))
		return GatewayType::GT_LOCAL;
	else if (s == TEXT("AGW"))
		return GatewayType::GT_AGW;
	else if (s == TEXT("KAKAO"))
		return GatewayType::GT_KAKAO;

	checkf(false, TEXT("Shouldn't reach here... Argument string is %s"), *s);

	return GatewayType::GT_NONE;
}

EOSCode GetOSCode()
{
#if PLATFORM_WINDOWS
	return EOSCode::PC;
#elif PLATFORM_ANDROID
	return EOSCode::AOS;
#elif PLATFORM_IOS
	return EOSCode::IOS;
#else
	return EOSCode::None;
#endif	
}

FString UnescapeURI(const FString& URLString)
{
	// @todo: support decoding of all escaped symbols in a better way

	FString DecodedString = URLString.Replace(TEXT("%3A"), TEXT(":"), ESearchCase::CaseSensitive);
	DecodedString = DecodedString.Replace(TEXT("%2F"), TEXT("/"), ESearchCase::CaseSensitive);

	return DecodedString;
}

FString ConcatenateURL(const FString& base, const FString sub)
{
	FString uri;

	auto index = base.Find(TEXT("/"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

	if (index == base.Len() - 1)
		uri = FString::Printf(TEXT("%s%s"), *base, *sub);
	else
		uri = FString::Printf(TEXT("%s/%s"), *base, *sub);

	return uri;
}

class AGWService : public IServiceHelper
{
public:
	AGWService(UB2UIPatch* patch) : ui_patch(patch)
	{
	}

	virtual void    VersionRequest() override
	{

	}

	virtual void    VersionResponse(FHttpRequestPtr, FHttpResponsePtr, bool) override
	{

	}

	virtual void    Request() override
	{
		FString Version = "0.13";

		UE_LOG(LogClass, Log, TEXT("####AGWService RequestHttpAGW!!!!!!!!!!!!!!"));
		TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
		//HttpRequest->SetURL(FString::Printf(DEV_AGW_URL, *Version));
		HttpRequest->SetVerb("GET");
		HttpRequest->OnProcessRequestComplete().BindRaw(this, &AGWService::Response);
		HttpRequest->ProcessRequest();
	}

	void Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (!bWasSuccessful)
		{
			if (ui_patch != nullptr)
				ui_patch->ResponseErrorCodePopup(Response->GetResponseCode());
			return;
		}

		TSharedPtr<FJsonObject> json_parse_obj;
		TSharedRef<TJsonReader<TCHAR>> json_reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
		if (FJsonSerializer::Deserialize(json_reader, json_parse_obj))
		{
			FString strServiceState = json_parse_obj->GetStringField("status");

			if (strServiceState.Equals("on"))
			{
				auto FGID			= json_parse_obj->GetIntegerField("fgid");
				cdn_url				= json_parse_obj->GetStringField("cdn");//"https://s3.ap-northeast-2.amazonaws.com/b2-cdn/blade2/patch/1.1/BladeII1.1.manifest";//"https://s3.ap-northeast-2.amazonaws.com/b2-cdn/blade2/live/0.2.2/1"; //FString::Printf(TEXT("%s/%d"), *cdnurl, FGID);
				resource_version	= json_parse_obj->GetStringField("fileversion");

				FString ManiFestUrl = FString::Printf(TEXT("%sBladeII%s.manifest"), *GetCDNAddress(), *resource_version);

				UE_LOG(LogClass, Log, TEXT("####PatchHtttpRequest !!!!!!!!!!!!!!"));
				TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
				HttpRequest->SetURL(ManiFestUrl);
				HttpRequest->SetVerb("GET");
				HttpRequest->OnProcessRequestComplete().BindUObject(ui_patch, &UB2UIPatch::PatchOnResponseReceived);
				HttpRequest->ProcessRequest();
			}
			else
			{
				ui_patch->BottomLayerVisible(ELayerType::ELayerType_FILELOAD);
				ui_patch->GameStart();
			}
		}
	}

	virtual FString GetCDNAddress() override
	{
		FString TextureFormat = "";

#if PLATFORM_ANDROID
		if (FAndroidMisc::SupportsASTC())
		{
			TextureFormat = "ASTC";
		}
		else
		{
			TextureFormat = "ETC1";
		}
#elif PLATFORM_IOS
		TextureFormat = "IOS";
#endif
		FString CDNURL = FString::Printf(TEXT("%s%s/%s/"), *cdn_url, *resource_version, *TextureFormat);

		return CDNURL;
	}

private:
	UB2UIPatch* ui_patch;
	FString		cdn_url;
	FString		resource_version;
};

class LocalService : public IServiceHelper
{
public:
	LocalService(UB2UIPatch* patch) : ui_patch(patch)
	{
	}

	virtual void    VersionRequest() override
	{
		FString GameInI;
		FConfigCacheIni::LoadGlobalIniFile(GameInI, TEXT("Game"));

		GConfig->GetString(TEXT("Local"), TEXT("CDNURL"), base_cdn_url, GameInI);

		FString VersionsURI = ConcatenateURL(base_cdn_url, TEXT("versions.json"));
#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================>Local versions.json URI : %s"), *VersionsURI);
#endif

		TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetURL(VersionsURI);
		HttpRequest->SetVerb("GET");
		HttpRequest->OnProcessRequestComplete().BindRaw(this, &LocalService::VersionResponse);
		HttpRequest->ProcessRequest();
	}

	virtual void    VersionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) override
	{
		if (!Response.IsValid() || !bWasSuccessful)
		{
#if FORCE_LOGGING_IN_ANDROID
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================> Failed to version request"));
#endif
			if (ui_patch != nullptr)
				ui_patch->ResponseErrorCodePopup(9999);
			return;
		}

		bool  bFound = false;
		const FString Version(TEXT(PREPROCESSOR_TO_STRING(VERSION)));

		TSharedPtr<FJsonObject> json_parse_obj;
		TSharedRef<TJsonReader<TCHAR>> json_reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(json_reader, json_parse_obj))
		{
			for (const auto& obj : json_parse_obj->GetArrayField(TEXT("versions")))
			{
				const auto& Object = obj->AsObject();

				const auto& platform = Object->GetStringField(TEXT("platform"));
				const auto& version = Object->GetStringField(TEXT("version"));
				const auto& cl = Object->GetStringField(TEXT("cl"));

#if PLATFORM_ANDROID
				if (TEXT("Android") != platform)
					continue;
#elif PLATFORM_IOS
				if (TEXT("IOS") != platform)
					continue;
#endif

				if (Version != version)
					continue;

				bFound = true;
				changelist_number = cl;

				break;
			}
		}
		else
		{
#if FORCE_LOGGING_IN_ANDROID
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================> versions.json is corrupted"));
#endif
		}

		if (!bFound)
		{
#if FORCE_LOGGING_IN_ANDROID
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================> Failed to find matching version..."));
#endif
			changelist_number = TEXT(PREPROCESSOR_TO_STRING(CL));
		}

		BladeIIGameImpl::GetClientDataStore().SetChangelistNum(changelist_number);

		this->Request();
	}

	virtual void    Request() override
	{
		FString ManifestURI = ConcatenateURL(GetCDNAddress(), TEXT("BladeII.manifest"));
#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================>Local Manifest URI : %s"), *ManifestURI);
#endif

		TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetURL(ManifestURI);
		HttpRequest->SetVerb("GET");
		HttpRequest->OnProcessRequestComplete().BindUObject(ui_patch, &UB2UIPatch::PatchOnResponseReceived);
		HttpRequest->ProcessRequest();
	}

	virtual FString GetCDNAddress() override
	{
		FString TCType;

#if PLATFORM_ANDROID
		if (FAndroidMisc::SupportsASTC())
		{
			TCType = TEXT("ASTC");
		}
		else
		{
			TCType = TEXT("ETC1");
		}
#elif PLATFORM_IOS
		TCType = TEXT("PVRTC");
#endif

		FString CDNURL = ConcatenateURL(ConcatenateURL(base_cdn_url, changelist_number), TCType);

#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================>Local CDNURL : %s"), *CDNURL);
#endif

		return CDNURL;
	}

private:
	UB2UIPatch* ui_patch;
	FString		base_cdn_url;
	FString     changelist_number;
};

class KakaoService : public IServiceHelper
{
public:
	KakaoService(UB2UIPatch* patch) : ui_patch(patch)
	{
	}

	virtual void    VersionRequest() override
	{
		base_cdn_url = B2P_GetCDNAddress();

		FString VersionsURI = ConcatenateURL(base_cdn_url, TEXT("versions.json"));
#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================> versions.json URI : %s"), *VersionsURI);
#endif

		TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetURL(VersionsURI);
		HttpRequest->SetVerb("GET");
		HttpRequest->OnProcessRequestComplete().BindRaw(this, &KakaoService::VersionResponse);
		HttpRequest->ProcessRequest();
	}

	virtual void    VersionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) override
	{
		if (!Response.IsValid() || !bWasSuccessful)
		{
#if FORCE_LOGGING_IN_ANDROID
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================> Failed to version request"));
#endif
			if(ui_patch != nullptr)
				ui_patch->ResponseErrorCodePopup(9999);
			return;
		}

		bool  bFound = false;
		const FString Version(TEXT(PREPROCESSOR_TO_STRING(VERSION)));

		TSharedPtr<FJsonObject> json_parse_obj;
		TSharedRef<TJsonReader<TCHAR>> json_reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(json_reader, json_parse_obj))
		{
			for (const auto& obj : json_parse_obj->GetArrayField(TEXT("versions")))
			{
				const auto& Object = obj->AsObject();

				const auto& platform = Object->GetStringField(TEXT("platform"));
				const auto& version = Object->GetStringField(TEXT("version"));
				const auto& cl = Object->GetStringField(TEXT("cl"));

#if PLATFORM_ANDROID
				if (TEXT("Android") != platform)
					continue;
#elif PLATFORM_IOS
				if (TEXT("IOS") != platform)
					continue;
#endif

				if (Version != version)
					continue;

				bFound = true;
				changelist_number = cl;

				break;
			}
		}
		else
		{
#if FORCE_LOGGING_IN_ANDROID
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================> versions.json is corrupted"));
#endif
		}

		if (!bFound)
		{
#if FORCE_LOGGING_IN_ANDROID
			FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================> Failed to find matching version..."));
#endif
			changelist_number = TEXT(PREPROCESSOR_TO_STRING(CL));
		}

		BladeIIGameImpl::GetClientDataStore().SetChangelistNum(changelist_number);

		this->Request();
	}

	virtual void    Request() override
	{
		FString ManifestURI = ConcatenateURL(GetCDNAddress(), TEXT("BladeII.manifest"));
#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================> Manifest URI : %s"), *ManifestURI);
#endif
		TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetURL(ManifestURI);
		HttpRequest->SetVerb("GET");
		HttpRequest->OnProcessRequestComplete().BindUObject(ui_patch, &UB2UIPatch::PatchOnResponseReceived);
		HttpRequest->ProcessRequest();

#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===============KakaoService HttpRequest"));
#endif
	}

	virtual FString GetCDNAddress() override
	{
		FString CdnURL;
		FString TCType;

#if PLATFORM_ANDROID
		if (FAndroidMisc::SupportsASTC())
		{
			TCType = TEXT("ASTC");
		}
		else
		{
			TCType = TEXT("ETC1");
		}
#elif PLATFORM_IOS 
		TCType = TEXT("PVRTC");
#endif 

		FString CDNURL;
		CDNURL = ConcatenateURL(ConcatenateURL(base_cdn_url, changelist_number), TCType);

		SetCDNUrl(CDNURL);

#if FORCE_LOGGING_IN_ANDROID
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("===================================> CDNURL : %s"), *CDNURL);
#endif

		return CDNURL;
	}

private:
	UB2UIPatch* ui_patch;
	FString		base_cdn_url;
	FString     changelist_number;
};

void UB2UIPatch::Init()
{
	Super::Init();
	PlatformType	= EPatchPlatformType::None;
	MarketType		= EMarketType::None;
	m_ServiceState	= EServiceState::UnknownError;
	progress_value_ = 0;
	m_bPlayMovie	= false;	
	m_bExitPopup	= false;
	ResponsePtr = nullptr;

	UI_CheckPermission->SetVisibility(ESlateVisibility::Hidden);
	UE_LOG(LogClass, Log, TEXT("####UB2UIPatch !!!!!!!!!!!!!!"));

	BottomLayerVisible(ELayerType::ELayerType_AGW);
	UB2UIPatch::InitGameInfo();

#if PLATFORM_ANDROID 
	FJavaWrapper::OnCheckPermissionResultDelegate.Remove(OnCheckPermissionHandler);
	OnCheckPermissionHandler = FJavaWrapper::OnCheckPermissionResultDelegate.AddUObject(this, &UB2UIPatch::CheckPermissionDelegate);
#elif PLATFORM_WINDOWS // PC 플랫폼에서 patch 가 될리 없으니 다음 스텝으로 넘어감.
	GameStart();
	return;
#endif

	FString ServiceType;

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA
	FString GameInI;
	FConfigCacheIni::LoadGlobalIniFile(GameInI, TEXT("Game"));

	GConfig->GetString(TEXT("CDN"), TEXT("ServiceType"), ServiceType, GameInI);
#else
	ServiceType = TEXT("KAKAO");
#endif
	
	if (isLocalConnectIniFileExist())
	{
		FString PreServiceType(ServiceType);
		ServiceType = TEXT("LOCAL");
		UE_LOG(LogClass, Log, TEXT("===================================>UIPatch isLocalConnectIniFileExist!! Pre Service Type : [%s] -> Changed Service Type : [%s]"), *PreServiceType, *ServiceType);
	}

	UE_LOG(LogClass, Log, TEXT("===================================> ServiceType : %s"), *ServiceType);

	auto GatewayToUse = StringToGatewayTypeEnum(ServiceType.ToUpper());

	switch (GatewayToUse)
	{
	case GatewayType::GT_KAKAO:
		ServiceHelper = MakeShared<KakaoService>(this);
		break;
	case GatewayType::GT_LOCAL:
		ServiceHelper = MakeShared<LocalService>(this);
		break;
	case GatewayType::GT_AGW:
		ServiceHelper = MakeShared<AGWService>(this);
		break;
	case GatewayType::GT_NONE:
	default:
		checkf(false, TEXT("GatewayToUse is invalid..."));
		break;
	}

	ServiceHelper->VersionRequest();

	PlayClassMovieBP();
}

void UB2UIPatch::InitGameInfo()
{
	switch (GetOSCode())
	{
	case EOSCode::AOS:
	{
		PlatformType = EPatchPlatformType::Google;
		MarketType = EMarketType::GoogleStore;
	}break;
	case EOSCode::IOS:
	{
		PlatformType = EPatchPlatformType::GameCenter;
		MarketType = EMarketType::AppStore;
	}break;
	default:
	{
		PlatformType = EPatchPlatformType::Google;
		MarketType = EMarketType::GoogleStore;
	}break;
	}

#if PLATFORM_ANDROID
	const UAndroidRuntimeSettings* Settings = GetDefault<UAndroidRuntimeSettings>();
	Version = Settings->VersionDisplayName;
	GameId = Settings->PackageName;
#elif PLATFORM_IOS
	const UIOSRuntimeSettings* Settings = GetDefault<UIOSRuntimeSettings>();
	Version = Settings->VersionInfo;
	GameId = Settings->BundleIdentifier;
#else
	//에디터에서 테스트 목적.
	Version = "0.2.2";
	GameId = "com.action2quare.blade2test";
#endif

#if AGW_SERVICE	
	Version = "0.13";
	GameId = "com.action2quare.blade2test";
	m_sAgwUrl = FString::Printf(DEV_AGW_URL, *Version);
#endif
}

void UB2UIPatch::OpenPermissionNotice()
{
	//UI_CheckPermission->SetVisibility(ESlateVisibility::Visible);
	//UI_CheckPermission->SetOwnerUI(this);
}

#if PLATFORM_ANDROID
void UB2UIPatch::CheckPermissionDelegate(JNIEnv *env, jobject thiz, jboolean bResult)
{
	if (bResult)
	{
		if(ServiceHelper.IsValid())
			ServiceHelper->Request();
		else
			checkf(false, TEXT("Illegal Service Type!!! Please Check Game Config..."));

		return;
	}

	UI_MsgPopup01->SetVisibility(ESlateVisibility::Visible);
	UI_MsgPopup01->SetOwnerUI(this);
	UI_MsgPopup01->SetPopupType(EPopupType::EPopupType_CheckPermission);
}
#endif

void UB2UIPatch::PlatformPermission()
{
	UI_CheckPermission->SetVisibility(ESlateVisibility::Hidden);
	B2P_InitPlatformPermission();
}

void UB2UIPatch::PatchOnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!Response.IsValid())
	{
		// respos 값이 아에 없을 시 처리 ( 999999 )
		UE_LOG(LogClass, Log, TEXT("####Respons is not vaild!!!!!!!!!!!!!!"));
		ResponseErrorCodePopup(999999);

		return;
	}

	if (!bWasSuccessful)
	{
		FString type = "";

		UE_LOG(LogClass, Log, TEXT("####agwCheck FALSE!!!!!!!!!!!!!!"));
		ResponseErrorCodePopup(Response->GetResponseCode());

		return;
	}

	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		ResponsePtr = Response;

		UE_LOG(LogClass, Log, TEXT("####agwCheck OK !!!!!!!!!!!!!!"));
		UB2UIPatch::OpenMsgBoxUpdateStart();
	}
	else
	{
		ResponseErrorCodePopup(Response->GetResponseCode());
	}
}

void UB2UIPatch::ResponseErrorCodePopup(int32 _RessponseCode, ESlateVisibility _visible, EPopupType _popupType, bool _bExitPopup)
{
#if B2_BUILD_LEVEL == B2_BUILD_LEVEL_LIVE
	_RessponseCode = _RessponseCode ^ 47880;
#endif

	UI_MsgPopup01->SetOwnerUI(this);
	UI_MsgPopup01->SetResponsCode(_RessponseCode); //404 -> 47772
	UI_MsgPopup01->SetVisibility(_visible);
	UI_MsgPopup01->SetPopupType(_popupType);
#if PLATFORM_IOS
	m_bExitPopup = false;
#else 
	m_bExitPopup = _bExitPopup;
#endif
}

bool UB2UIPatch::GetFullDownloadSize(int64 &OutData)
{
	//if (ResponsePtr.Get() == nullptr)
	//{
	//	ensure(!TEXT("Check ResponsePtr"));
	//	OutData = -1;
	//	return false;
	//}

	//IBuildPatchServicesModule* build_patch_services = &FModuleManager::LoadModuleChecked<IBuildPatchServicesModule>(TEXT("BuildPatchServices"));
	//IBuildManifestPtr remote_manifest = build_patch_services->MakeManifestFromJSON(ResponsePtr->GetContentAsString());
	//if (!remote_manifest.IsValid())
	//{
	//	ensure(!TEXT("####remote_manifest FALSE !!!!!!!!!!!!!!"));
	//	OutData = -1;
	//	return false;
	//}

	//IBuildManifestPtr last_manifest = nullptr;
	//FString last_manifest_path = FPaths::Combine(*UCPatcher::GetPatchPath(), TEXT("last_patch.manifest"));
	//last_manifest = build_patch_services->LoadManifestFromFile(last_manifest_path);

	//if (last_manifest.Get() == nullptr)
	//{
	//	OutData = remote_manifest->GetDownloadSize();
	//}
	//else
	//{
	//	TSet<FString> Tags;
	//	OutData = remote_manifest->GetDeltaDownloadSize(Tags, last_manifest.ToSharedRef());
	//}

	return true;
}

bool UB2UIPatch::GetResumeDataSize(int64 &OutData)
{
	//if (ResponsePtr.Get() == nullptr)
	//{
	//	ensure(!TEXT("Check ResponsePtr"));
	//	OutData = -1;
	//	return false;
	//}

	//IBuildPatchServicesModule* build_patch_services = &FModuleManager::LoadModuleChecked<IBuildPatchServicesModule>(TEXT("BuildPatchServices"));
	//IBuildManifestPtr remote_manifest = build_patch_services->MakeManifestFromJSON(ResponsePtr->GetContentAsString());

	//OutData = GetResumeDataSize(*UCPatcher::GetPatchPath(), remote_manifest.ToSharedRef());

	return true;
}

void UB2UIPatch::OpenMsgBoxUpdateStart()
{
	int64 downloadsize = 0;

	if (GetFullDownloadSize(downloadsize) == true)
	{
		int64 resumeDataSize = 0;
		GetResumeDataSize(resumeDataSize);

		// resumeDataSize가 큰 경우는 비정상으로 판단하고 용량체크를 안함
		if (downloadsize > resumeDataSize)
		{
			downloadsize -= resumeDataSize;

			if (HasEnoughStorageSize(downloadsize) == false)
			{
				if (BTN_BlockDownload.IsValid())
					BTN_BlockDownload->SetVisibility(ESlateVisibility::Visible);
				ShowPopupNotEnoughStorageSize(downloadsize / (1024 * 1024));
				return;
			}
		}

		if (BTN_BlockDownload.IsValid())
			BTN_BlockDownload->SetVisibility(ESlateVisibility::Hidden);

		downloadsize = downloadsize / (1024 * 1024);

		if (resumeDataSize > 0 || downloadsize <= 0)
		{
			UB2UIPatch::OnTapUpdateStartOk();
			UE_LOG(LogClass, Log, TEXT("#### resumeDataSize == %d"), resumeDataSize);
		}
		else if (resumeDataSize <= 0)
		{
			UI_MsgPopup02->SetVisibility(ESlateVisibility::Visible);
			UI_MsgPopup02->SetPopupType(EPopupType::EPopupType_DownloadVolume);
			UI_MsgPopup02->SetOwnerUI(this);
			UI_MsgPopup02->SetDownLoadVolume(downloadsize);
			UE_LOG(LogClass, Log, TEXT("####resumeDataSize == %d"), resumeDataSize);
			PlayClassMovieBP();
		}
	}
}

int64 UB2UIPatch::GetResumeDataSize(const FString& InStagingDir, const IBuildManifestRef& InBuildManifest)
{
	int64 size = 0;
	FString InstallPath = InStagingDir / TEXT("Staged") / TEXT("Install");
	FString ResumeDataInfoFile = InstallPath / TEXT("$resumeData");
	bool bHasResumeData = FPlatformFileManager::Get().GetPlatformFile().FileExists(*ResumeDataInfoFile);

	if (bHasResumeData)
	{
		FString PrevResumeData;
		FString PatchVersion = InBuildManifest->GetAppName() + InBuildManifest->GetVersionString();
		TArray< FString > ResumeDataLines;
		FFileHelper::LoadFileToString(PrevResumeData, *ResumeDataInfoFile);
		PrevResumeData.ParseIntoArray(ResumeDataLines, TEXT("\n"), true);
		FString PreviousVersion = (ResumeDataLines.Num() > 0) ? MoveTemp(ResumeDataLines[0]) : TEXT("");
		if (PreviousVersion == PatchVersion)
		{
			TArray<FString> fileList = InBuildManifest->GetBuildFileList();
			for (int32 i = 0; i < fileList.Num(); i++)
			{
				FString fileName = InstallPath / fileList[i];
				size += FPlatformFileManager::Get().GetPlatformFile().FileSize(*fileName);
			}
		}
	}
	return size;
}

bool UB2UIPatch::HasEnoughStorageSize(const uint64 NeedSize)
{
#if WITH_EDITOR || PLATFORM_WINDOWS
	return true;
#endif

	uint64 TotalNumberOfBytes = 0; 
	uint64 NumberOfFreeBytes = 0;

	FPlatformMisc::GetDiskTotalAndFreeSpace(TEXT(""), TotalNumberOfBytes, NumberOfFreeBytes);

#if PLATFORM_IOS
	UE_LOG(LogClass, Log, TEXT("#### HasEnoughStorageSize ] total:%ld free:%ld need:%ld"), TotalNumberOfBytes, NumberOfFreeBytes, NeedSize);
#endif
#if PLATFORM_ANDROID
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("#### HasEnoughStorageSize : total:%12lld   free : %12lld  need: %lld"), TotalNumberOfBytes, NumberOfFreeBytes, NeedSize);
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("#### HasEnoughStorageSize : total:%ld   free : %ld  need: %ld"), TotalNumberOfBytes, NumberOfFreeBytes, NeedSize);
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("#### HasEnoughStorageSize : total:%lld   free : %lld  need: %lld"), TotalNumberOfBytes, NumberOfFreeBytes, NeedSize);
#endif

	uint64 RealNeedSize = NeedSize * 1.05f;
	return RealNeedSize < NumberOfFreeBytes;
}

void UB2UIPatch::ShowPopupNotEnoughStorageSize(int64 nVolume)
{
	UI_MsgPopup01->SetVisibility(ESlateVisibility::Visible);
	UI_MsgPopup01->SetOwnerUI(this);
	UI_MsgPopup01->SetPopupType(EPopupType::EPopupType_NotEnoughDiskSpace);
	UI_MsgPopup01->SetNeedDiskSize(FString::Printf(TEXT("%dMB"), nVolume));
	m_bExitPopup = false;
}

void UB2UIPatch::OnTapUpdateStartOk()
{
	UE_LOG(LogClass, Log, TEXT("####OnTapUpdateStartOk ] !!!!!!!!!!!!!!"));
	BottomLayerVisible(ELayerType::ELayerType_DOWNLOAD);

	if (!Patcher.IsValid())
		Patcher = MakeShared<UCPatcher>();

	check(ServiceHelper.IsValid());
	Patcher->OnResponseReceived(this, ResponsePtr, ServiceHelper->GetCDNAddress());
}

void UB2UIPatch::CacheAssets()
{
	GET_SLOT(UProgressBar, pb_download)
	GET_SLOT(UTextBlock, text_desc)
	GET_SLOT(UTextBlock, text_percent)

	GET_SLOT(UOverlay, O_Download)
	GET_SLOT(UOverlay, O_AgwCheck)
	GET_SLOT(UOverlay, O_FileLoading)

	GET_SLOT(UB2UICheckPermission, UI_CheckPermission)
	
	if (UI_CheckPermission.IsValid()) 
	{
		UI_CheckPermission->Init();
	}

	GET_SLOT(UB2UIDLCFrontPopup, UI_MsgPopup01)
	if (UI_MsgPopup01.IsValid()) 
	{
		UI_MsgPopup01->Init();
		UI_MsgPopup01->SetVisibility(ESlateVisibility::Hidden);
	}

	GET_SLOT(UB2UIDLCFrontPopup, UI_MsgPopup02)
	if (UI_MsgPopup02.IsValid()) 
	{
		UI_MsgPopup02->Init();
		UI_MsgPopup02->SetVisibility(ESlateVisibility::Hidden);
	}

	GET_SLOT(UB2Button, BTN_BlockDownload);
	if(BTN_BlockDownload.IsValid())
		BTN_BlockDownload->SetVisibility(ESlateVisibility::Hidden);
	BIND_CLICK_FUNC_TO_BTN(BTN_BlockDownload, &UB2UIPatch::OnClicked_BlockDownload);

	GET_SLOT(UB2UIPatchBack, UIP_PatchBack);
	if (UIP_PatchBack.IsValid())
	{
		UIP_PatchBack->Init();
		UIP_PatchBack->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void UB2UIPatch::GameStart()
{
	StopBackgroundMovie();

	SaveGameData_DLCDownload(true);
	GConfig->Flush(false, GB2GeneralSavedStateIni);

	auto* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
	if (LoginGameMode)
	{
		LoginGameMode->GmaeModeGotoTitle();
	}
}

void UB2UIPatch::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!Patcher.IsValid() || m_bExitPopup)
		return;

	if (!Patcher->GetInstaller().Get())
		return;

	if (!m_bPlayMovie)
	{
		m_bPlayMovie = true;
		if (UIP_PatchBack.IsValid())
		{
			UIP_PatchBack->SetVisibility(ESlateVisibility::Visible);
			UIP_PatchBack->StartPlayMovie();
		}
	}

	float progress = Patcher->GetInstaller()->GetUpdateProgress();
	progress_value_ = FMath::Max(progress_value_, progress);
	FString percent = FString::Printf(TEXT("%d"), (int32)(progress_value_ * 100.f)) + "%";
	text_percent->SetText(FText::FromString(percent));
	pb_download->SetPercent(progress_value_);
	
	if (Patcher->GetInstaller()->GetDownloadHealth() == EBuildPatchDownloadHealth::Disconnected)
	{
#if PLATFORM_IOS
		if (UI_MsgPopup01->GetVisibility() == ESlateVisibility::Visible)
			return;
#endif
		UI_MsgPopup01->SetVisibility(ESlateVisibility::Visible);
		UI_MsgPopup01->SetOwnerUI(this);
		UI_MsgPopup01->SetPopupType(EPopupType::EPopupType_CheckPermission);
#if PLATFORM_ANDROID
		m_bExitPopup = true;		
#endif
	}
}

void UB2UIPatch::BottomLayerVisible(ELayerType eType)
{
	switch (eType)
	{
	case ELayerType::ELayerType_AGW:
		O_Download->SetVisibility(ESlateVisibility::Hidden);
		O_AgwCheck->SetVisibility(ESlateVisibility::Visible);
		O_FileLoading->SetVisibility(ESlateVisibility::Hidden);
		break;
	case ELayerType::ELayerType_DOWNLOAD:
		O_Download->SetVisibility(ESlateVisibility::Visible);
		O_AgwCheck->SetVisibility(ESlateVisibility::Hidden);
		O_FileLoading->SetVisibility(ESlateVisibility::Hidden);
		break;
	case ELayerType::ELayerType_FILELOAD:
		O_Download->SetVisibility(ESlateVisibility::Hidden);
		O_AgwCheck->SetVisibility(ESlateVisibility::Hidden);
		O_FileLoading->SetVisibility(ESlateVisibility::Visible);
		break;
	case ELayerType::ELayerType_NONE:
		break;
	default:
		break;
	}
	PlayClassMovieBP();
}

void UB2UIPatch::GotoAccessTermUI()
{
	if (UIP_PatchBack.IsValid())
	{
		UIP_PatchBack->SetVisibility(ESlateVisibility::Hidden);
		UIP_PatchBack->StopAllMovie();
	}
	BottomLayerVisible(ELayerType::ELayerType_FILELOAD);

	auto* LoginGameMode = Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(this));
	if (LoginGameMode)
	{
		LoginGameMode->OpenUIScene(EDLCFrontUIName::EDLCUI_AccessTerms);
	}
}

void UB2UIPatch::RetryPlatformPermission()
{
	UI_MsgPopup01->SetVisibility(ESlateVisibility::Hidden);
	//B2P_InitPlatformPermission();
	B2P_GameExit();
}

void UB2UIPatch::PatchStart()
{
	UI_MsgPopup02->SetVisibility(ESlateVisibility::Hidden);
	UB2UIPatch::OnTapUpdateStartOk();
}

void UB2UIPatch::GameExit()
{
	UI_MsgPopup01->SetVisibility(ESlateVisibility::Hidden);
	B2P_GameExit();
}

EPCClass UB2UIPatch::GetSelectPcClass()
{
	return AB2DLCFrontGameMode::GetSelectedFrontTitleMovieClass();
	PlayClassMovieBP();
}

void UB2UIPatch::OnClicked_BlockDownload()
{
	OpenMsgBoxUpdateStart();
}

void UB2UIPatch::RetryRequest()
{
	if (UI_MsgPopup01.IsValid())
	{
		UI_MsgPopup01->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ResponsePtr.Get() != nullptr)
		return;

	if (ServiceHelper.Get() != nullptr)
		ServiceHelper->VersionRequest();
}