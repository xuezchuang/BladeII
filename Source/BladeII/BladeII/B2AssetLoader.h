// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "Engine/StreamableManager.h"
#include "B2AssetLoader.generated.h"

/**
 * 
 */

UENUM()
enum class ELoadingState : uint8 
{
	Requested,
	InLoading,
	Complete,
	Canceled,
	NotRequested,

	Max
};

DECLARE_DELEGATE_OneParam(FB2AsyncLoadingComplete, const FString&);


USTRUCT()
struct FB2AsyncRequest
{
	GENERATED_BODY()

	FB2AsyncRequest() : AsyncHandle(nullptr), bBlockUI(false), bCompleteDelegateCalled(false) {}
	~FB2AsyncRequest() {}

	FString								RequestName;
	TSharedPtr<FStreamableHandle>		AsyncHandle;
	FB2AsyncLoadingComplete				CompleteDelgate;

	TMap<uint32, FSoftObjectPath>	RequestAssetHash;

	bool								bBlockUI;

	void	ConstructAssetHash(const TArray<FSoftObjectPath>& StringAssets);
	bool	GetStringAssets(TArray<FSoftObjectPath>& OutStringAssets) const;

	void	ExecuteCompleteCallback();

	bool	HasRequest(const FSoftObjectPath& StringAsset) const;
	bool	IsLoadingComplete() const;
	bool	IsCompleteNotify() const;
	bool	IsNowLoading() const;

	float	GetProgress() const; /* 0.f ~ 1.f */

private:

	bool bCompleteDelegateCalled;
};

UCLASS()
class BLADEII_API UB2AssetLoader : public UObject
{
public:

	GENERATED_BODY()

	static UB2AssetLoader* Get() 
	{
		static UB2AssetLoader* AssetLoader = nullptr;

		if (AssetLoader == nullptr)
		{
			if (UPackage* TransientPkg = GetTransientPackage())
			{
				AssetLoader = NewObject<UB2AssetLoader>(GetTransientPackage(), FName(TEXT("B2AssetLoader")), RF_MarkAsRootSet);
				if(AssetLoader)
					AssetLoader->GlobalFlushHandle = FCoreDelegates::OnAsyncLoadingFlush.AddUObject(AssetLoader, &UB2AssetLoader::OnAsyncLoadFlush);
			}
		}

		//BII_CHECK(AssetLoader);
		return AssetLoader;
	}

	// AsyncWaiting < 0 일땐 LoadSynchronous로
	template<class T=UObject>
	T* LoadSynchronous(const TSoftObjectPtr<T>& AssetReference, float AsyncWaiting = -1.f)
	{
		if (!AssetReference.IsNull())
			return Cast<T>(LoadSynchronous(AssetReference.GetSoftObjectPath(), AsyncWaiting));

		return nullptr;
	}

	template<class T=UObject>
	UClass* LoadSynchronous(const TSoftClassPtr<T>& ClassAsset, float AsyncWaiting = -1.f)
	{
		if (!ClassAsset.IsNull())
			return Cast<UClass>(LoadSynchronous(ClassAsset.ToSoftObjectPath(), AsyncWaiting));

		return nullptr;
	}

	UObject*		LoadSynchronous(const FSoftObjectPath& StringAsset, float AsyncWaiting = -1.f);

	template<class T=UObject>
	ELoadingState	RequestAsyncLoad(const FString& RequestName, TSoftObjectPtr<T> Asset, FB2AsyncLoadingComplete CompleteDelegate = FB2AsyncLoadingComplete(), bool bBlocking = false)
	{
		FSoftObjectPath StringAsset = Asset.GetSoftObjectPath();
		return RequestAsyncLoad(RequestName, StringAsset, CompleteDelegate, bBlocking);
	}

	template <class T=UObject>
	ELoadingState	RequestAsyncLoad(const FString& RequestName, TSoftClassPtr<T> ClassAsset, FB2AsyncLoadingComplete CompleteDelegate = FB2AsyncLoadingComplete(), bool bBlocking = false)
	{
		FSoftObjectPath StringClassAsset = ClassAsset.ToSoftObjectPath();
		return RequestAsyncLoad(RequestName, StringClassAsset, CompleteDelegate, bBlocking);
	}

	ELoadingState	RequestAsyncLoad(const FString& RequestName, const TArray<FSoftObjectPath>& StringAssets, FB2AsyncLoadingComplete CompleteDelegate = FB2AsyncLoadingComplete(), bool bBlocking = false);
	ELoadingState	RequestAsyncLoad(const FString& RequestName, const FSoftObjectPath& StringAsset, FB2AsyncLoadingComplete CompleteDelegate = FB2AsyncLoadingComplete(), bool bBlocking = false);


	const FB2AsyncRequest*	GetAsyncRequest(const FString& RequestName);
	ELoadingState			GetLoadingState(const FString& RequestName);

	bool					WaitUntilLoadComplete(const FString& RequestName);



	template <class T=UObject>
	void	UnloadAsset(TSoftClassPtr<T> ClassAsset)
	{
		UnloadAsset(ClassAsset.ToSoftObjectPath());
	}

	template <class T=UObject>
	void	UnloadAsset(TSoftObjectPtr<T> Asset)
	{
		UnloadAsset(Asset.GetSoftObjectPath());
	}


	void	UnloadAsset(const FSoftObjectPath& StringAsset);
	void	ReleaseRequest(const FString& RequestName);

	void	FlushAll();
	void	OnShutdownModule();

	FORCEINLINE static FStreamableManager& GetStreamableManager() 
	{ 
		auto* Loader = UB2AssetLoader::Get();
		//BII_CHECK(Loader);
		
		return Loader->StreamManager; 
	}

private:

	void OnCompleteAsyncLoad();
	TSharedPtr<FStreamableHandle>	GetAsyncHandle(const FString& RequestName);
	TSharedPtr<FStreamableHandle>	GetAsyncHandle(const FSoftObjectPath& StringAsset);

	UFUNCTION()
	void OnAsyncLoadFlush();

private:

	uint32							BlockingRequestCount;

	TMap<FString, FB2AsyncRequest>	AsyncLoadRequestMap;
	FStreamableManager				StreamManager;

	FDelegateHandle					GlobalFlushHandle;

//#if B2_BUILD_LEVEL == B2_BUILD_LEVEL_LIVE
//#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 0
//#else
//#if UE_BUILD_SHIPPING
//#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 0
//#else
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
//#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 1
//#else
//#define BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 0
//#endif
//#endif
//#endif

//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//public:
//	static bool bEnableDevLog; // Including on-screen log..
//#endif
};


class IB2LoadInterface
{
public:

	FORCEINLINE UB2AssetLoader*	GetAssetLoader() { return UB2AssetLoader::Get(); }

	/* Synchronous Load Interface */
	virtual UObject* LoadSynchronous(const FSoftObjectPath& StringAsset, float AsyncWaiting = -1.f) final;

	template<class T = UObject>
	T* LoadSynchronous(const TSoftObjectPtr<T>& AssetReference, float AsyncWaiting = -1.f)
	{
		if (AssetReference.IsNull() == false)
			return Cast<T>(LoadSynchronous(AssetReference, AsyncWaiting));

		return nullptr;
	}

	template<class T = UObject>
	UClass* LoadSynchronous(const TSoftClassPtr<T>& ClassAssetReference, float AsyncWaiting = -1.f)
	{
		if (ClassAssetReference.IsNull() == false)
			return Cast<UClass>(LoadSynchronous(ClassAssetReference, AsyncWaiting));

		return nullptr;
	}

	virtual void UnloadAsset(const FSoftObjectPath& StringAsset) final;


	FSoftObjectPath EstimateStringAsset(const UObject* Object);

};

USTRUCT()
struct FB2AsyncRequestInfo
{
	GENERATED_BODY()

	uint32					AssetKey;		/* not a StringAsset Hash */
	FSoftObjectPath	StringAsset;

	FB2AsyncRequestInfo() : AssetKey(0) {}
	FB2AsyncRequestInfo(uint32 InAssetKey, const FSoftObjectPath& InStringAsset) : AssetKey(InAssetKey), StringAsset(InStringAsset) {}

	bool operator == (FB2AsyncRequestInfo const& InOther) const
	{
		return (this->AssetKey == InOther.AssetKey) && (this->StringAsset == InOther.StringAsset);
	}

	template<typename T>
	FB2AsyncRequestInfo(const T& Key, const FSoftObjectPath& InStringAsset)
	{
		AssetKey = GetTypeHash(Key);
		StringAsset = InStringAsset;
	}
};



class IB2AsyncInterface : public IB2LoadInterface
{
public:

	virtual ~IB2AsyncInterface() {}

	virtual bool GatherAsyncLoadAssets(const FString& RequestName, TArray<FSoftObjectPath>& OutGatheredRequesList) { return false; }

	virtual const FB2AsyncRequest* GetAsyncRequest(const FString& RequestName) final;

	virtual void TryAsyncLoad(const FString& RequestName, bool bBlocking = false) final;
	virtual void TryAsyncLoad(const FString& RequestName, const TArray<FSoftObjectPath>& RequestList, bool bBlocking = false) final;
	virtual void TryAsyncLoad(const FString& RequestName, const TArray<FB2AsyncRequestInfo>& RequestList, bool bBlocking = false) final;

	virtual void ReleaseAsyncLoad(const FSoftObjectPath& StringAsset) final;
	virtual void ReleaseAsyncLoad(const FString& RequestName) final;

protected:

	virtual void OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FB2AsyncRequestInfo>& CompleteRequestList) {}
	virtual void OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FSoftObjectPath>& CompleteRequestList) {}

	/* FB2AsyncRequestInfo 를 사용해서 TryAsyncLoad 할때만 호출됨 */
	virtual void OnAsyncLoadComplete_EachObject(const uint32 AssetKey, UObject* LoadedAssetObject) {}


private:

	virtual void PrivateAsyncCompleteCallback(const FString& CompleteRequest) final;
	TMap<FString, TArray<FB2AsyncRequestInfo> > TemporaryRequestList;
};
