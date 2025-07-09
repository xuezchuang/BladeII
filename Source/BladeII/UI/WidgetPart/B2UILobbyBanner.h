// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UILobbyBanner.generated.h"

enum class EErrorImageType : uint8
{
	NONE,
	IMG_INVALID,
	IMG_TOLARGE
};

enum class EBannerLobbyToGo : uint8
{
	Goto_Pakage = 1,
	Goto_EventMenu,
	Goto_AttendanceEvent,


	DoNotGoAnywhere,
};

struct FLobbyBannerData
{
	FLobbyBannerData(int32 index, UTexture2DDynamic* image, FString urlpath, int64 time_start, int64 time_end, float scrolltime, EBannerLobbyToGo goType, int32 goSubType, EErrorImageType error_type, FString web_url)
		: BannerIndex(index)
		, BannerImage(image)
		, BannerUrl(urlpath)
		, StartTime(time_start)
		, EndTime(time_end)
		, NextBannerScrollTime(scrolltime)
		, WhereToGo(goType)
		, WhereToGo_Sub(goSubType)
		, ErrorType(error_type)
		, WebURL(web_url)
	{
	}

	explicit FLobbyBannerData()
		: BannerIndex(0)
		, BannerImage(nullptr)
		, BannerUrl(TEXT(""))
		, StartTime(0)
		, EndTime(0)
		, NextBannerScrollTime(0.0)
		, WhereToGo(EBannerLobbyToGo::DoNotGoAnywhere)
		, WhereToGo_Sub(0)
		, ErrorType(EErrorImageType::NONE)
		, WebURL(TEXT(""))
	{
	}

	int32 BannerIndex;
	UTexture2DDynamic* BannerImage;
	FString BannerUrl;
	int64 StartTime;
	int64 EndTime;
	float NextBannerScrollTime;
	EBannerLobbyToGo WhereToGo;
	int32 WhereToGo_Sub;
	EErrorImageType ErrorType;
	FString WebURL;
};

UCLASS()
class BLADEII_API UB2UILobbyBanner : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	UB2UILobbyBanner(const FObjectInitializer& ObjectInitializer);
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	void ShowBanner(int32 ToShowIndex, bool bForce);

	void OnDownloadStart();
	void DownUrlBannerImagesStart();
	void OnDownloadNext(int32 idx);
	void CheckBannerAllLoad();
	void AddtoErrorData(bool bIsFail);
	bool CheckImageSize(class UTexture2DDynamic* Tex);
	void LoadErrorImage();

	UFUNCTION()
	void OnDownloadSucess(class UTexture2DDynamic* Tex);
	
	UFUNCTION()
	void OnDownloadFailed(class UTexture2DDynamic* Tex);

protected:
	UFUNCTION()
	void OnClickBTN_Banner_Back();
	UFUNCTION()
	void OnClickBTN_Banner_Current();
	UFUNCTION()
	void OnClickBTN_Banner_Forward();

	void ClickBanner(int32 idx);

	void AutoScroll(float InDeltaTime);
	void OnBeginScroll(float ScrolledDelta);
	void OnEndScroll();
	void ChangeBanner(int32 CenterBannerIndex, bool bForced = false);
	float GetBannerWidth();

	UPROPERTY(EditAnywhere, Category = "BladeIILobbyBanner")
	TSubclassOf<class UB2UIStoreBannerButton> BannerButtonClass;

	UPROPERTY(EditAnywhere, Category = "BladeIILobbyBanner")
	TSoftObjectPtr<UMaterialInstance> MAT_VeryLarge;
	UPROPERTY(EditAnywhere, Category = "BladeIILobbyBanner")
	TSoftObjectPtr<UMaterialInstance> MAT_ValidDownLoad;

	float AutoScrollTime;

	TArray<FLobbyBannerData> BannerImages;
private:
	TWeakObjectPtr<UB2Button> BTN_Banner_Back;
	TWeakObjectPtr<UB2Button> BTN_Banner_Current;
	TWeakObjectPtr<UB2Button> BTN_Banner_Forward;
	TWeakObjectPtr<UB2ScrollBox> BannerScrollBox;
	TWeakObjectPtr<UImage> IMG_BannerBackward;
	TWeakObjectPtr<UImage> IMG_BannerCurrent;
	TWeakObjectPtr<UImage> IMG_BannerForward;
	TWeakObjectPtr<UHorizontalBox> HB_ButtonBox;
	TWeakObjectPtr<UHorizontalBox> HB_BannerBox;

	TArray<TWeakObjectPtr<UB2UIStoreBannerButton>> BannerButtons;
	int32 CurrentViewBannerIndex;
	int32 DesiredPosition;

	float ElapsedNextBannerScrollTime;
	float BannerImageWidth;
	float WidgetScale;

	bool bInitSuccess;
	bool bAutoMove;

	UPROPERTY(Transient)
	UMaterialInstance* LoadedErrorImage_Valid;
	UPROPERTY(Transient)
	UMaterialInstance* LoadedErrorImage_Large;

	UPROPERTY(Transient)
	class UAsyncTaskDownloadImage* ImageDownloader;
	int32 CurrentDownloadIndex;
};
