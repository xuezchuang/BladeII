// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "Patcher.h"
#include "B2UIPatch.h"
#include "IPlatformFilePak.h"
#include "B2UIManager.h"


#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
extern FString GExternalFilePath;
extern FString GFilePathBase;

#endif
FString m_sCdnUrl = "https://s3.ap-northeast-2.amazonaws.com/b2-cdn/blade2/patch/1.1/";
UCPatcher::UCPatcher()
{
	state_ = EDownloadState::None;
	is_download_ = false;
}

UCPatcher::~UCPatcher()
{

}

FString UCPatcher::GetPatchPath()
{
	FString path = "";

//#if 1	
//	// 만일 GamePersistentDownloadDir 로 커버 안되는 경우가 발생하면 FPaths::DLCInstallStageDir 혹은 FPaths::DLCInstallDir 이쪽도 같이 수정합니다..
//	path = FPaths::GamePersistentDownloadDir() + TEXT("/");
//#else
//
//	#if PLATFORM_ANDROID
//		path = GExternalFilePath;// FPaths::Combine(*GExternalFilePath, TEXT("Patches")); ;
//	#elif PLATFORM_IOS
//
//		path = TEXT("/");
//		//		path = FString( [NSSearchPathForDirectoriesInDomains( NSCachesDirectory, NSUserDomainMask, YES ) objectAtIndex:0] ) + TEXT( "/" );
//		//		path = FString([NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0]) + TEXT("/");
//		//		path = FString([[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains : NSUserDomainMask] lastObject]) + TEXT("/");
//	#else
//		path = FPaths::RootDir();
//	#endif
//
//#endif

	return path;
}

void UCPatcher::OnResponseReceived(UB2UIPatch* owner, FHttpResponsePtr response, FString sCdnUrl)
{
	//state_ = EDownloadState::None;
	//owner_ = owner;
	//m_sCdnUrl = sCdnUrl;

	//FString sTest = response->GetContentAsString();
	//UE_LOG(LogClass, Log, TEXT("#### CDN GetContentAsString == %s"), *sTest);

	//build_patch_services_ = &FModuleManager::LoadModuleChecked<IBuildPatchServicesModule>(TEXT("BuildPatchServices"));

	//remote_manifest_ = build_patch_services_->MakeManifestFromJSON(response->GetContentAsString());
	//if (!remote_manifest_.IsValid()) {
	////	UUtility::ShippingLog(FString(TEXT("[UCPatcher] Manifest was invalid")));
	////	return;
	//}

	////UAGWControl* AGWControl = GetMgr(ULogInMgr)->GetAGWControl();
	//UE_LOG(LogClass, Log, TEXT("#### CDN URL == %s"), *m_sCdnUrl);
	//build_patch_services_->SetCloudDirectory(m_sCdnUrl);//Set Download URL 

	//FString stage_dir = FPaths::DLCInstallStageDir(); //FPaths::Combine(*GetPatchPath(), TEXT("Staged"));
	//if (!IFileManager::Get().DirectoryExists(*stage_dir))
	//	IFileManager::Get().MakeDirectory(*stage_dir, true);

	//build_patch_services_->SetStagingDirectory(stage_dir);

	//install_dir_ = FPaths::DLCInstallDir(); //FPaths::Combine(*GetPatchPath(), TEXT("Install"));
	//if (!IFileManager::Get().DirectoryExists(*install_dir_))
	//	IFileManager::Get().MakeDirectory(*install_dir_, true);

	//FString last_manifest_path = FPaths::Combine(*GetPatchPath(), TEXT("last_patch.manifest"));

	//UE_LOG(LogClass, Log, TEXT("[OnResponseReceived] CDNUrl=%s, StageDir=%s, InstallDir=%s, LastManifestPath=%s"), *m_sCdnUrl, *stage_dir, *install_dir_, *last_manifest_path);


	//last_manifest_ = nullptr;
	//if (IFileManager::Get().FileExists(*last_manifest_path)) {
	//	last_manifest_ = build_patch_services_->LoadManifestFromFile(last_manifest_path);
	//	UE_LOG(LogClass, Log, TEXT("[OnResponseReceived] CDNUrl=%s, StageDir=%s, InstallDir=%s, LastManifestPath=%s"), *m_sCdnUrl, *stage_dir, *install_dir_, *last_manifest_path);
	//	if (last_manifest_.IsValid() && last_manifest_->GetVersionString() == remote_manifest_->GetVersionString()) {
	//		UE_LOG(LogClass, Log, TEXT("#### CDN ????????????? == %s"), *m_sCdnUrl);

	//		FString InstallDir = FPaths::DLCInstallDir(); //FPaths::Combine(*GetPatchPath(), TEXT("Install"));
	//		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	//		TArray<FString> PakFiles;
	//		FPakSearchVisitor Visitor(PakFiles);
	//		PlatformFile.IterateDirectoryRecursively(*InstallDir, Visitor);

	//		for (uint32 PakIndex = 0, PakCount = PakFiles.Num(); PakIndex < PakCount; ++PakIndex)
	//		{
	//			if (FCoreDelegates::OnMountPak.IsBound()) {
	//				FPakPlatformFile* PakPlatformFile = static_cast<FPakPlatformFile*>(FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile")));
	//				if (PakPlatformFile != nullptr) {
	//					PakPlatformFile->Mount(*PakFiles[PakIndex], 1);
	//				}
	//				else {
	//					//UUtility::ShippingLog(FString::Printf(TEXT("[OnOpenLevelLogin] PakPlatformFile not found! InstallDir=%s"), *InstallDir));
	//					return;
	//				}
	//			}
	//		}

	//		//if (IsAccessTerm())
	//		//{
	//		// 카카오걸로 대채
	//			owner_->GameStart();
	//	/*	}
	//		else
	//		{
	//			owner_->BottomLayerVisible(ELayerType::ELayerType_FILELOAD);
	//			owner_->GotoAccessTermUI();
	//		}*/
	//		return;
	//	}
	//}

	//state_ = EDownloadState::Download;

	//StartBuildInstall();
}

void UCPatcher::StartBuildInstall()
{
//	FString sBasePath = "";
//#if 0// PLATFORM_ANDROID
//	sBasePath = FPaths::Combine(*GFilePathBase, TEXT("BladeII/Install"));
//
//	if (!IFileManager::Get().DirectoryExists(*sBasePath))
//		IFileManager::Get().MakeDirectory(*sBasePath, true);
//
////	UE_LOG(LogClass, Log, TEXT("[OnResponseRecsBasePath!!!!!!!!!!!eived] StartBuildInstall=%s"), *sBasePath);
//#endif
//	/*build_installer_ = build_patch_services_->StartBuildInstall(last_manifest_,
//		remote_manifest_,
//		sBasePath,
//		FBuildPatchBoolManifestDelegate::CreateRaw(this, &UCPatcher::OnDownloadCompleted));
//		UE_LOG(LogClass, Log, TEXT("#### CDN StartBuildInstall == %s"), *m_sCdnUrl);*/
//	build_installer_ = build_patch_services_->StartBuildInstall(last_manifest_,
//		remote_manifest_,
//		install_dir_,
//		FBuildPatchBoolManifestDelegate::CreateRaw(this, &UCPatcher::OnDownloadCompleted));

}
void UCPatcher::OnDownloadCompleted(bool success, IBuildManifestRef build_manifest)
{
//	if (success)
//	{
//	//	UUtility::ShippingLog(FString(TEXT("[UCPatcher] OnDownloadCompleted Success!")));
//
//		FString last_manifest_path = FPaths::Combine(*GetPatchPath(), TEXT("last_patch.manifest"));
//		if (IFileManager::Get().FileExists(*last_manifest_path))
//			IFileManager::Get().Delete(*last_manifest_path);
//		build_patch_services_->SaveManifestToFile(last_manifest_path, build_manifest, false);
//
//		build_installer_ = nullptr;
//		build_patch_services_ = nullptr;
//	//	owner_->GetWorld()->ForceGarbageCollection(true);
//	//	GetMgr(ULogInMgr)->ChangeSequence(ULogInMgr::ESequence::LoginPageStart);
//		UE_LOG(LogClass, Log, TEXT("#### CDN success == %s"), *m_sCdnUrl);
//
//		FString InstallDir = FPaths::DLCInstallDir(); // FPaths::Combine(*GetPatchPath(), TEXT("Install"));
//		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
//
//		TArray<FString> PakFiles;
//		FPakSearchVisitor Visitor(PakFiles);
//		PlatformFile.IterateDirectoryRecursively(*InstallDir, Visitor);
//
//		for (uint32 PakIndex = 0, PakCount = PakFiles.Num(); PakIndex < PakCount; ++PakIndex)
//		{
//			if (FCoreDelegates::OnMountPak.IsBound()) {
//				FPakPlatformFile* PakPlatformFile = static_cast<FPakPlatformFile*>(FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile")));
//				if (PakPlatformFile != nullptr) {
//					PakPlatformFile->Mount(*PakFiles[PakIndex], 1);
//				}
//				else {
//					//UUtility::ShippingLog(FString::Printf(TEXT("[OnOpenLevelLogin] PakPlatformFile not found! InstallDir=%s"), *InstallDir));
//					return;
//				}
//			}
//		}
//		// savedata
//		//if (IsAccessTerm())
//		//{
//			owner_->GameStart();
//		//}
//		//else
//		/*{
//			owner_->BottomLayerVisible(ELayerType::ELayerType_FILELOAD);
//			owner_->GotoAccessTermUI();
//		}*/
//		
//	}
//	else
//	{
//		EBuildPatchInstallError err = build_installer_->GetBuildStatistics().FailureType;
//		FString message;
//		UE_LOG(LogClass, Log, TEXT("#### CDN success===Fail == %s"), *m_sCdnUrl);
//		if (err == EBuildPatchInstallError::OutOfDiskSpace
//			|| err == EBuildPatchInstallError::MoveFileToInstall
//			|| err == EBuildPatchInstallError::BuildVerifyFail)
//		{
////			message = UCppFunctionLibrary::GetString(501121);
//			state_ = EDownloadState::Shutdown;
//		}
//		else if (err != EBuildPatchInstallError::UserCanceled
//			&& err != EBuildPatchInstallError::ApplicationClosing
//			&& err != EBuildPatchInstallError::ApplicationError)
//		{
//			//message = UCppFunctionLibrary::GetString(501114);
//		}
//
//// 		if (!message.IsEmpty()
//// 			&& owner_ && owner_->GetSplashGameMode())
//// 		{
//// 		//	ACSplashGameMode* SplashGameMode = Cast<ACSplashGameMode>(owner_->GetSplashGameMode());
//// 		//	SplashGameMode->OpenMsgBox(message, EUIMsgBoxBtnType::Confirm, owner_, TEXT("OnTapMsgBoxOk"));
//// 		}
//
//		if (build_patch_services_)
//		{
//			build_patch_services_->CancelAllInstallers(false);
//		}
//
//		//UUtility::ShippingLog(FString::Printf(TEXT("[UCPatcher] OnDownloadCompleted Fail!!! %d"), (int32)err));
//	}
}


BuildPatchServices::EBuildPatchState UCPatcher::GetPatchState()
{
	BuildPatchServices::EBuildPatchState state = BuildPatchServices::EBuildPatchState::Initializing;

	if (build_installer_.Get() == nullptr)
		return state;

	state = build_installer_->GetState();

	return state;
}


