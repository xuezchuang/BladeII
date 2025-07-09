// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "B2UIDocBindable.h"
#include "UI/B2UIWidgetBase.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif

#include "B2UIFriendKakaoInvite.generated.h"

class UAsyncTaskDownloadImage;
class UB2UIKakaoFriendInviteRow;
class UB2UIKakaoFriendRewardRow;


enum class EKakaoRewardTabType : uint8
{
	None,
	InviteReward,
	JoinerReward,
	Max
};


class FB2KakaoProfileDownloadInfo
{
public:
	FB2KakaoProfileDownloadInfo()
		: SkipTarget(false)
		, Index(-1)
		, ImageUrl(TEXT(""))
		, TargetRow(nullptr)
	{}

	int32 GetIndex() { return Index; }
	void SetIndex(const int32 InIndex) { Index = InIndex; }

	FString GetImageUrl() { return ImageUrl; }
	void SetImageUrl(const FString InImageUrl) { ImageUrl = InImageUrl; }

	void SetDownloadImage(class UTexture2DDynamic *Tex);

	void SetTargetRow(UB2UIKakaoFriendInviteRow *Row) { TargetRow = Row; }
	int32 GetTargetIndex();

	void SetSkip(bool bSkip) { SkipTarget = bSkip; }
	bool GetSkip() { return SkipTarget; }
private:

	bool SkipTarget;
	int32 Index;
	FString ImageUrl;

	class UB2UIKakaoFriendInviteRow *TargetRow;
};


UCLASS()
class UB2KakaoProfileManager : public UObject
{
	GENERATED_BODY()
public:
	void Init();

	UFUNCTION()
	void OnDownloadSucess(class UTexture2DDynamic* Tex);

	UFUNCTION()
	void OnDownloadFailed(class UTexture2DDynamic* Tex);

	void Start();

	void NextDownload();
	bool HasMoreDownload();

	int32 GetNextDownloadIndex();

	void PushDownloadInfo(std::shared_ptr<FB2KakaoProfileDownloadInfo> DownloadInfo);

	void SetSkipDownload(int32 Index);

	void ClearAll();

	void RetryDownloads();
private:
	bool bRetryDownload;

	int32 CurDownloadIndex;
	UPROPERTY(Transient)
	class UAsyncTaskDownloadImage* ImageDownloader;
	TArray<std::shared_ptr<FB2KakaoProfileDownloadInfo>> DownloadInfoList;

	TArray<std::shared_ptr<FB2KakaoProfileDownloadInfo>> RetryInfoList;
};


/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIFriendKakaoInvite : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	UB2UIFriendKakaoInvite(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

	virtual void NativeConstruct() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void OnRefresh();

	void RefreshInfo();

	void LoadFriendList();

	void OnKakaoFriendInvite(const int32 RowIndex);
	void OnKakaoGetReward(const int32 RowIndex);

	void SubscribeEvent();
	void UnsubscribeEvent();
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	
	UFUNCTION()
	void OnChangeInviteRewardInfo(class UB2UIDocBase* Sender, bool CurrState, bool PrevState);

#if PLATFORM_ANDROID
	void OnKakaoInvitePlatformCallback(JNIEnv *env, jobject thiz, jint resultCode);
	void OnInviteFriendInfoCompletedCallback(JNIEnv *env, jobject thiz);
	void OnKakaoInvitationEventsCompletedCallback(JNIEnv *env, jobject thiz);	
#endif

	void OnKakaoInvitePlatformCallback(const int32 ResultCode);
	void OnDeliveryInviteReward(const FB2ResponseFriendInviteRewardPtr &RewardInfo);
private:

	void CreateJoinerRewardList();
	void CreateInviteRewardList();
	void CreateInviteFriendList();

	void ClearInviteFriendList();

	void OnRefreshInviteRewardList();
	void OnRefreshJoinerRewardList();

	void OnUpdateRewardList();

	void SwitchTab(EKakaoRewardTabType TabType);

	void SetInvisibleFriendRows();

	void OnScrolledOffset(float Offset);
	void OnEndScroll();

	void CreateFriendRows(int32 StartIndex, int32 EndIndex);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<UB2UIKakaoFriendInviteRow> RowContentClass;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	int32 KakaoInvitedRewardCount;
private:
	TArray<TWeakObjectPtr<UB2UIKakaoFriendRewardRow>> RewardRowList;
	TArray<TWeakObjectPtr<UB2UIKakaoFriendInviteRow>> FriendRowList;

	TWeakObjectPtr<UB2UICommonTab> Tab_InviteReward;
	TWeakObjectPtr<UB2UICommonTab> Tab_JoinerReward;

	TWeakObjectPtr<UTextBlock> STB_InviteFriendCount;
	TWeakObjectPtr<UTextBlock> TB_InviteFriendCount;

	TWeakObjectPtr<UTextBlock> STB_SuccessFriendCount;
	TWeakObjectPtr<UTextBlock> TB_SuccessFriendCount;

	TWeakObjectPtr<UTextBlock> TB_RightBottomTip;

	TWeakObjectPtr<UTextBlock> STB_TodayInviteCount;
	TWeakObjectPtr<UTextBlock> TB_TodayInviteCount;

	TWeakObjectPtr<UTextBlock> TB_KakaoFriendTip;
	
	TWeakObjectPtr<UB2ScrollBox> SB_FriendList;

	UPROPERTY(Transient)
	UB2KakaoProfileManager *ProfileManagerPtr;
	EKakaoRewardTabType CurRewardTabState;

	FDelegateHandle OnKakaoInviteHandler;
	FDelegateHandle OnKakaoFriendInfoHandler;
	FDelegateHandle OnKakaoInviteEventHandler;

	int32 DeliveryFriendInviteRewardTicket;

	bool bIsLoadingFriendList;

#if PLATFORM_IOS
	FDelegateHandle OnInvitionFriendListHandler;
	FDelegateHandle OnInvitionEventsHandler;
#endif
};
