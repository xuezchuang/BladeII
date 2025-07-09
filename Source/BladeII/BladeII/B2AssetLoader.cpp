// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2AssetLoader.h"
#include "BladeIIUtil.h"
#include "Event.h"


bool FB2AsyncRequest::IsLoadingComplete() const
{
	if (AsyncHandle.IsValid())
		return AsyncHandle->HasLoadCompleted();
	
	return false;
}

bool FB2AsyncRequest::IsCompleteNotify() const
{
	return bCompleteDelegateCalled;
}

bool FB2AsyncRequest::IsNowLoading() const
{
	if (AsyncHandle.IsValid())
		return AsyncHandle->IsLoadingInProgress();

	return false;
}

float FB2AsyncRequest::GetProgress() const
{
	if (AsyncHandle.IsValid())
		return AsyncHandle->GetProgress();

	return -1.f;
}

bool FB2AsyncRequest::HasRequest(const FSoftObjectPath& StringAsset) const
{
	const int32 AssetHash = GetTypeHash(StringAsset);
	if (RequestAssetHash.Find(AssetHash) != nullptr)
		return true;
	
	return false;
}


void FB2AsyncRequest::ConstructAssetHash(const TArray<FSoftObjectPath>& StringAssets)
{
	const int32 AssetCount = StringAssets.Num();
	RequestAssetHash.Empty(AssetCount);

	for (const auto& StringAsset : StringAssets)
	{
		const uint32 AssetHash = GetTypeHash(StringAsset);
		RequestAssetHash.Add(AssetHash, StringAsset);
	}
}

bool FB2AsyncRequest::GetStringAssets(TArray<FSoftObjectPath>& OutStringAssets) const
{
	RequestAssetHash.GenerateValueArray(OutStringAssets);
	return OutStringAssets.Num() > 0 ? true : false;
}

void FB2AsyncRequest::ExecuteCompleteCallback()
{
	if (CompleteDelgate.IsBound())
	{
		CompleteDelgate.Execute(RequestName);
		bCompleteDelegateCalled = true;
	}
}

UObject* UB2AssetLoader::LoadSynchronous(const FSoftObjectPath& StringAsset, float AsyncWaiting)
{
	if (StringAsset.IsValid() == false)
		return nullptr;

	if (UObject* ExistObj = StringAsset.ResolveObject())
		return ExistObj;

	// 이미 AsyncLoading중인 Asset이 있다면
	TSharedPtr<FStreamableHandle> AsyncHandle = GetAsyncHandle(StringAsset);
	if (AsyncHandle.IsValid() && AsyncWaiting >= 0.f)
	{
		if (EAsyncPackageState::Complete == AsyncHandle->WaitUntilComplete(AsyncWaiting))
			return StringAsset.ResolveObject();
}
	
	return StreamManager.LoadSynchronous(StringAsset);
}

ELoadingState UB2AssetLoader::RequestAsyncLoad(const FString& RequestName, const TArray<FSoftObjectPath>& StringAssets, FB2AsyncLoadingComplete CompleteDelegate, bool bBlocking/* = false*/)
{
	if (RequestName.Len() == 0) 
	{
		UE_LOG(LogBladeII, Error, TEXT("RequestAsyncLoad needs RequestName"));
		return ELoadingState::NotRequested;
	}

	TSharedPtr<FStreamableHandle> AsyncHandle = GetAsyncHandle(RequestName);
	if (AsyncHandle.IsValid())
	{
		if (AsyncHandle->IsLoadingInProgress())
			return ELoadingState::InLoading;

		else if (AsyncHandle->HasLoadCompleted())
			return ELoadingState::Complete;
	}

	FB2AsyncRequest Request;
	FStreamableDelegate PrivateCallback = FStreamableDelegate::CreateUObject(this, &UB2AssetLoader::OnCompleteAsyncLoad);

	Request.AsyncHandle = StreamManager.RequestAsyncLoad(StringAssets, PrivateCallback);
	Request.RequestName = RequestName;
	Request.CompleteDelgate = CompleteDelegate;
	Request.bBlockUI = bBlocking;
	Request.ConstructAssetHash(StringAssets);

	AsyncLoadRequestMap.Add(RequestName, Request);

	if (bBlocking)
	{ // 입력을 막고 로딩중임을 보여주는 UI. 메인쓰레드 Blocking 의 의미가 아닌 입력 Blocking 의 의미.
		ShowMiscLoadingIconClass<bool>::GetInstance().Signal(true);
		BlockingRequestCount++;
	}

	return ELoadingState::Requested;
}

ELoadingState UB2AssetLoader::RequestAsyncLoad(const FString& RequestName, const FSoftObjectPath& StringAsset, FB2AsyncLoadingComplete CompleteDelegate /*= FB2AsyncLoadingComplete()*/, bool bBlocking /*= false*/)
{
	return RequestAsyncLoad(RequestName, TArray<FSoftObjectPath> {StringAsset}, CompleteDelegate, bBlocking);
}

void UB2AssetLoader::FlushAll()
{
	if (AsyncLoadRequestMap.Num() > 0)
	{
		for (TMap<FString, FB2AsyncRequest>::TIterator It(AsyncLoadRequestMap); It; ++It)
		{
			FB2AsyncRequest& RequestInfo = It.Value();
			if (RequestInfo.AsyncHandle.IsValid())
				RequestInfo.AsyncHandle->CancelHandle();	// LoadComplete 시에는 ReleaseHandle 역할도
		}
	}

	AsyncLoadRequestMap.Empty();
}

void UB2AssetLoader::ReleaseRequest(const FString& RequestName)
{
	if (FB2AsyncRequest* FindedRequest = AsyncLoadRequestMap.Find(RequestName))
	{
		if (FindedRequest->AsyncHandle.IsValid())
			FindedRequest->AsyncHandle->CancelHandle();	// LoadComplete 시에는 ReleaseHandle 역할도

		AsyncLoadRequestMap.Remove(RequestName);
	}
}

void UB2AssetLoader::OnShutdownModule()
{
	FCoreDelegates::OnAsyncLoadingFlush.Remove(GlobalFlushHandle);
}

void UB2AssetLoader::UnloadAsset(const FSoftObjectPath& StringAsset)
{
	if (StringAsset.IsValid())
		StreamManager.Unload(StringAsset);	
}

void UB2AssetLoader::OnCompleteAsyncLoad()
{
	for (TMap<FString, FB2AsyncRequest>::TIterator It(AsyncLoadRequestMap); It; ++It)
	{
		FB2AsyncRequest& RequestInfo = It.Value();
		if (RequestInfo.IsLoadingComplete() && RequestInfo.IsCompleteNotify() == false)
		{
			if (RequestInfo.bBlockUI == true)
			{
				BlockingRequestCount--;
				if (BlockingRequestCount == 0)
					ShowMiscLoadingIconClass<bool>::GetInstance().Signal(false);
			}

			RequestInfo.ExecuteCompleteCallback();
		}
	}
}

TSharedPtr<FStreamableHandle> UB2AssetLoader::GetAsyncHandle(const FString& RequestName)
{
	if (const FB2AsyncRequest* AsyncRequest = GetAsyncRequest(RequestName))
		return AsyncRequest->AsyncHandle;

	return nullptr;
}

TSharedPtr<FStreamableHandle> UB2AssetLoader::GetAsyncHandle(const FSoftObjectPath& StringAsset)
{
	TSharedPtr<FStreamableHandle> AsyncHandle = GetAsyncHandle(StringAsset.ToString());
	if (AsyncHandle.IsValid())
	{
		return AsyncHandle;
	}

	else
	{
		for (TMap<FString, FB2AsyncRequest>::TIterator It(AsyncLoadRequestMap); It; ++It)
		{
			FB2AsyncRequest& RequestInfo = It.Value();
			if (RequestInfo.HasRequest(StringAsset))
				return RequestInfo.AsyncHandle;
		}
	}

	return nullptr;
}

void UB2AssetLoader::OnAsyncLoadFlush()
{
	int32 TotalAsyncRequestAssetNum = 0;
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	for (TMap<FString, FB2AsyncRequest>::TIterator It(AsyncLoadRequestMap); It; ++It)
	{
		FB2AsyncRequest& RequestInfo = It.Value();
		if (RequestInfo.IsNowLoading())
		{
			TotalAsyncRequestAssetNum += RequestInfo.RequestAssetHash.Num();
		}
	}
#endif

	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2AssetLoader::AsyncLoad Flush!! (while %d asset requests are ongoing)"), TotalAsyncRequestAssetNum));

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (bEnableDevLog)
	{
		// Flush 시점에 Async 로딩 중인 게 있으면 화면에 로그 띄움.
		if (TotalAsyncRequestAssetNum > 0)
		{
			BII_SCREEN_LOG(FString::Printf(TEXT("[B2AssetLoader] FlushAsyncLoading while %d asset requests are not completed."), TotalAsyncRequestAssetNum),
				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 8.0f);
		}
	}
#endif
}

const FB2AsyncRequest* UB2AssetLoader::GetAsyncRequest(const FString& RequestName)
{
	if (const FB2AsyncRequest* FindedRequest = AsyncLoadRequestMap.Find(RequestName))
		return FindedRequest;

	return nullptr;
}

ELoadingState UB2AssetLoader::GetLoadingState(const FString& RequestName)
{
	auto AsyncHandle = GetAsyncHandle(RequestName);
	if(AsyncHandle.IsValid())
	{
		if (AsyncHandle->HasLoadCompleted())			return ELoadingState::Complete;
		else if (AsyncHandle->IsLoadingInProgress())	return ELoadingState::InLoading;
		else if (AsyncHandle->WasCanceled())			return ELoadingState::Canceled;
		else											return ELoadingState::Requested;
	}

	return ELoadingState::NotRequested;
}

bool UB2AssetLoader::WaitUntilLoadComplete(const FString& RequestName)
{
	if (const auto* AsyncRequest = GetAsyncRequest(RequestName))
	{
		if (AsyncRequest->AsyncHandle.IsValid())
		{
			auto AsyncResult = AsyncRequest->AsyncHandle->WaitUntilComplete();
			return (AsyncResult == EAsyncPackageState::Complete) ? true : false;
		}
	}

	return false;
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
bool UB2AssetLoader::bEnableDevLog = false;
#endif


// ---------------------------- Load Interface ---------------------------- //

UObject* IB2LoadInterface::LoadSynchronous(const FSoftObjectPath& StringAsset, float AsyncWaiting /*= -1.f*/)
{
	UB2AssetLoader* AssetLoader = GetAssetLoader();
	BII_CHECK(AssetLoader);

	return AssetLoader->LoadSynchronous(StringAsset, AsyncWaiting);
}

void IB2LoadInterface::UnloadAsset(const FSoftObjectPath& StringAsset)
{
	UB2AssetLoader* AssetLoader = GetAssetLoader();
	BII_CHECK(AssetLoader);

	AssetLoader->UnloadAsset(StringAsset);
}

FSoftObjectPath IB2LoadInterface::EstimateStringAsset(const UObject* Object)
{
	FSoftObjectPath AssetString;

	if (Object && Object->IsAsset())
		AssetString = Object->GetPathName(nullptr);

	return AssetString;
}

// ---------------------------- Async Interface ---------------------------- //

void IB2AsyncInterface::TryAsyncLoad(const FString& RequestName, bool bBlocking/* = false*/)
{
	if (UB2AssetLoader* AssetLoader = GetAssetLoader())
	{
		TArray<FSoftObjectPath> AsyncLoadAssets;
		if (GatherAsyncLoadAssets(RequestName, AsyncLoadAssets))
			TryAsyncLoad(RequestName, AsyncLoadAssets, bBlocking);
	}
}

void IB2AsyncInterface::TryAsyncLoad(const FString& RequestName, const TArray<FB2AsyncRequestInfo>& RequestList, bool bBlocking/* = false*/)
{
	UB2AssetLoader* AssetLoader = GetAssetLoader();
	BII_CHECK(AssetLoader);

	if(GetAsyncRequest(RequestName) == nullptr && RequestList.Num() > 0)
	{
		// Request 정보 임시 Caching
		TemporaryRequestList.Add(RequestName, RequestList);
	
		TArray<FSoftObjectPath> StringAssets;
		for (const auto& RequestInfo : RequestList)
			StringAssets.Add(RequestInfo.StringAsset);

		TryAsyncLoad(RequestName, StringAssets, bBlocking);
	}
}

void IB2AsyncInterface::TryAsyncLoad(const FString& RequestName, const TArray<FSoftObjectPath>& RequestAssets, bool bBlocking/* = false*/)
{
	UB2AssetLoader* AssetLoader = GetAssetLoader();
	BII_CHECK(AssetLoader);

	if (GetAsyncRequest(RequestName) == nullptr && RequestAssets.Num() > 0)
	{
		FB2AsyncLoadingComplete CompleteCallback;
		CompleteCallback.BindRaw(this, &IB2AsyncInterface::PrivateAsyncCompleteCallback);

		AssetLoader->RequestAsyncLoad(RequestName, RequestAssets, CompleteCallback, bBlocking);
	}
}

void IB2AsyncInterface::ReleaseAsyncLoad(const FString& RequestName)
{
	UB2AssetLoader* AssetLoader = GetAssetLoader();
	BII_CHECK(AssetLoader);

	AssetLoader->ReleaseRequest(RequestName);
}

void IB2AsyncInterface::ReleaseAsyncLoad(const FSoftObjectPath& StringAsset)
{
	ReleaseAsyncLoad(StringAsset.ToString());
}

void IB2AsyncInterface::PrivateAsyncCompleteCallback(const FString& CompleteRequest)
{
	if (const auto* Request = GetAsyncRequest(CompleteRequest))
	{
		if (const TArray<FB2AsyncRequestInfo>* RequestListPtr = TemporaryRequestList.Find(CompleteRequest))
		{
			const auto& RequestList = *RequestListPtr;

			OnAsyncLoadComplete(CompleteRequest, RequestList);

			for (const auto& LoadedRequest : RequestList)
				OnAsyncLoadComplete_EachObject(LoadedRequest.AssetKey, LoadedRequest.StringAsset.ResolveObject());

			TemporaryRequestList.Remove(CompleteRequest);
		}
		else
		{
			TArray<FSoftObjectPath> LoadedStringAssets;
			Request->GetStringAssets(LoadedStringAssets);
			OnAsyncLoadComplete(CompleteRequest, LoadedStringAssets);
		}
	}
}

const FB2AsyncRequest* IB2AsyncInterface::GetAsyncRequest(const FString& RequestName)
{
	if (UB2AssetLoader* AssetLoader = GetAssetLoader())
	{
		if (const auto* Request = AssetLoader->GetAsyncRequest(RequestName))
			return Request;
	}

	return nullptr;
}

