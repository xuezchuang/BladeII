#pragma once

//#include "BladeII.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IBuildPatchServicesModule.h"
#include "Interfaces/IBuildManifest.h"
#include "BuildPatchState.h"
#include "Engine/CollisionProfile.h"
#include "HAL/PlatformFile.h"
#include "Misc/Paths.h"


class UB2UIPatch;

class FPakSearchVisitor : public IPlatformFile::FDirectoryVisitor
{
	TArray<FString>& FoundPakFiles;
public:
	FPakSearchVisitor(TArray<FString>& InFoundPakFiles)
		: FoundPakFiles(InFoundPakFiles)
	{}
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
	{
		if (bIsDirectory == false)
		{
			FString Filename(FilenameOrDirectory);
			if (FPaths::GetExtension(Filename) == TEXT("pak"))
			{
				FoundPakFiles.Add(Filename);
			}
		}
		return true;
	}
};

class UCPatcher
{
public:
	enum class EDownloadState : uint8
	{
		None,
		Download,
		Shutdown,
	};

	UCPatcher();
	~UCPatcher();

	void OnResponseReceived(UB2UIPatch* owner, FHttpResponsePtr Response, FString sCdnUrl);
	void OnDownloadCompleted(bool, IBuildManifestRef);
	bool SaveStringTextToFile(FString SaveDirectory, FString FileName, FString SaveText, bool AllowOverWriting);

	static FString GetPatchPath();
	static FString GetRootPath();
	
	void StartUpPatch();

	void StartBuildInstall();

	BuildPatchServices::EBuildPatchState GetPatchState();

	EDownloadState GetDownloadState() { return state_; }
	IBuildInstallerPtr GetInstaller() { return build_installer_; }

private:
	EDownloadState state_;
	bool is_download_;
	FString install_dir_;
	IBuildManifestPtr remote_manifest_;
	IBuildManifestPtr last_manifest_;
	IBuildPatchServicesModule* build_patch_services_;
	IBuildInstallerPtr build_installer_;
	UB2UIPatch* owner_;
};