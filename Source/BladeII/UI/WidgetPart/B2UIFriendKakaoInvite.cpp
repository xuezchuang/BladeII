// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIFriendKakaoInvite.h"
#include "B2UIKakaoFriendInviteRow.h"
#include "B2UIKakaoFriendRewardRow.h"
#include "Blueprint/AsyncTaskDownloadImage.h"
#include "B2UIHeader.h"
#include "B2RedDotManager.h"
#include "B2UIDocHelper.h"
#include "B2UICommonTab.h"
#include "Retailer.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2CodeTable.h"
#include "B2UIManager.h"

#include "B2PlatformAPI.h"

#if PLATFORM_IOS
#include "KakaoTalkPlugin.h"
#endif

#include "Kismet/KismetSystemLibrary.h"

//*************************************
// FB2KakaoProfileDownloadInfo
//*************************************
void FB2KakaoProfileDownloadInfo::SetDownloadImage(class UTexture2DDynamic *Tex)
{
	if (TargetRow) 
		TargetRow->SetUserProfileImage(Tex); 
}

int32 FB2KakaoProfileDownloadInfo::GetTargetIndex() {
	if (TargetRow) return TargetRow->GetRowIndex();
	else return -1;
}

//*************************************
// UB2KakaoProfileManager
//*************************************
void UB2KakaoProfileManager::Init()
{
	bRetryDownload = false;
	ImageDownloader = NewObject<UAsyncTaskDownloadImage>(this, NAME_None, RF_Transient);

	if (ImageDownloader)
	{
		ImageDownloader->OnSuccess.AddDynamic(this, &UB2KakaoProfileManager::OnDownloadSucess);
		ImageDownloader->OnFail.AddDynamic(this, &UB2KakaoProfileManager::OnDownloadFailed);
	}
}

void UB2KakaoProfileManager::OnDownloadSucess(class UTexture2DDynamic* Tex)
{
	if (DownloadInfoList.IsValidIndex(CurDownloadIndex) && DownloadInfoList[CurDownloadIndex].get() != nullptr)
	{
		DownloadInfoList[CurDownloadIndex]->SetDownloadImage(Tex);
	}

	NextDownload();
}

void UB2KakaoProfileManager::OnDownloadFailed(class UTexture2DDynamic* Tex)
{
	if (bRetryDownload == false && DownloadInfoList.IsValidIndex(CurDownloadIndex) && DownloadInfoList[CurDownloadIndex].get() != nullptr)
	{
		RetryInfoList.Add(DownloadInfoList[CurDownloadIndex]);
	}
	
	NextDownload();
}

void UB2KakaoProfileManager::Start()
{
	if (RetryInfoList.Num() > 0)
		RetryInfoList.Empty();
	CurDownloadIndex = -1;
	NextDownload();
}

void UB2KakaoProfileManager::NextDownload()
{
	if (HasMoreDownload())
	{
		CurDownloadIndex = GetNextDownloadIndex();
		if (ImageDownloader && DownloadInfoList.IsValidIndex(CurDownloadIndex) && DownloadInfoList[CurDownloadIndex].get() != nullptr)
		{
			ImageDownloader->Start(DownloadInfoList[CurDownloadIndex]->GetImageUrl());
		}
	}
	else
	{
		if (bRetryDownload == false)
		{
			bRetryDownload = true;

			RetryDownloads();
		}
	}
}

int32 UB2KakaoProfileManager::GetNextDownloadIndex()
{
	int32 Result = CurDownloadIndex + 1;

	for (int32 Index = Result; Index < DownloadInfoList.Num(); ++Index)
	{
		if (DownloadInfoList[Index].get() != nullptr && DownloadInfoList[Index]->GetSkip())
		{
			++Result;
		}
		else
		{
			break;
		}
	}

	return Result;
}

bool UB2KakaoProfileManager::HasMoreDownload()
{
	if (DownloadInfoList.IsValidIndex(GetNextDownloadIndex()) == true)
		return true;
	return false;
}

void UB2KakaoProfileManager::PushDownloadInfo(std::shared_ptr<FB2KakaoProfileDownloadInfo> DownloadInfo)
{
	DownloadInfoList.Add(DownloadInfo);
}

void UB2KakaoProfileManager::SetSkipDownload(int32 Index)
{
	if (DownloadInfoList.IsValidIndex(Index) && DownloadInfoList[Index].get() != nullptr)
	{
		DownloadInfoList[Index]->SetSkip(true);
	}
}

void UB2KakaoProfileManager::ClearAll()
{
	if(DownloadInfoList.Num() > 0)
		DownloadInfoList.Empty();
}

void UB2KakaoProfileManager::RetryDownloads()
{
	if (RetryInfoList.Num() < 1)
		return;
	if (DownloadInfoList.Num() > 0)
		DownloadInfoList.Empty();

	for (auto &Each : RetryInfoList)
	{
		DownloadInfoList.Add(Each);
	}

	Start();
}


//*************************************
// UB2UIFriendKakaoInvite
//*************************************
UB2UIFriendKakaoInvite::UB2UIFriendKakaoInvite(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurRewardTabState = EKakaoRewardTabType::None;
	bIsLoadingFriendList = false;
	KakaoInvitedRewardCount = 0;
}

void UB2UIFriendKakaoInvite::Init()
{
	Super::Init();
	SubscribeEvent();

	ProfileManagerPtr = NewObject<UB2KakaoProfileManager>(this, NAME_None, RF_Transient);
	if (ProfileManagerPtr)
		ProfileManagerPtr->Init();

	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	{
		FriendDoc->RefreshKakaoFriendInfo();
		FriendDoc->RefreshKakaoEventInfo();
	}
}

void UB2UIFriendKakaoInvite::NativeConstruct()
{
	Super::NativeConstruct();
}

void UB2UIFriendKakaoInvite::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	UnsubscribeEvent();
	ClearInviteFriendList();
	if (ProfileManagerPtr)
		ProfileManagerPtr->ClearAll();
}

void UB2UIFriendKakaoInvite::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UICommonTab, Tab_InviteReward);
	GET_SLOT(UB2UICommonTab, Tab_JoinerReward);

	if (Tab_InviteReward.IsValid())
		Tab_InviteReward->Init();
	if (Tab_JoinerReward.IsValid())
		Tab_JoinerReward->Init();

	GET_SLOT(UTextBlock, STB_InviteFriendCount);
	GET_SLOT(UTextBlock, TB_InviteFriendCount);

	GET_SLOT(UTextBlock, STB_SuccessFriendCount);
	GET_SLOT(UTextBlock, TB_SuccessFriendCount);

	GET_SLOT(UTextBlock, STB_TodayInviteCount);
	GET_SLOT(UTextBlock, TB_TodayInviteCount);

	GET_SLOT(UTextBlock, TB_RightBottomTip);
	GET_SLOT(UTextBlock, TB_KakaoFriendTip);

	GET_SLOT(UB2ScrollBox, SB_FriendList);

	const int32 MaxRewardSlot = 5;
	RewardRowList.SetNum(MaxRewardSlot);
	GET_SLOT_BYNAME(UB2UIKakaoFriendRewardRow, RewardRowList[0], UIP_KakaoFriendReward_01);
	GET_SLOT_BYNAME(UB2UIKakaoFriendRewardRow, RewardRowList[1], UIP_KakaoFriendReward_02);
	GET_SLOT_BYNAME(UB2UIKakaoFriendRewardRow, RewardRowList[2], UIP_KakaoFriendReward_03);
	GET_SLOT_BYNAME(UB2UIKakaoFriendRewardRow, RewardRowList[3], UIP_KakaoFriendReward_04);
	GET_SLOT_BYNAME(UB2UIKakaoFriendRewardRow, RewardRowList[4], UIP_KakaoFriendReward_05);

	for (const auto &Each : RewardRowList)
	{
		if (Each.IsValid())
		{
			Each->Init();
			Each->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (SB_FriendList.IsValid())
	{
		SB_FriendList->SetOnUserScrolledEnd(FSimpleDelegate::CreateUObject(this, &UB2UIFriendKakaoInvite::OnEndScroll));
		SB_FriendList->SetOnUserScrolled(FOnUserScrolled::CreateUObject(this, &UB2UIFriendKakaoInvite::OnScrolledOffset));
	}	
}

void UB2UIFriendKakaoInvite::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (STB_InviteFriendCount.IsValid())
		STB_InviteFriendCount->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_KakaoFriendInviteNumber")));

	if (STB_SuccessFriendCount.IsValid())
		STB_SuccessFriendCount->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_KakaoFriendInviteSuccessNumber")));

	if (STB_TodayInviteCount.IsValid())
		STB_TodayInviteCount->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_SendList2")));

	if (TB_RightBottomTip.IsValid())
		TB_RightBottomTip->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_KakaoFriendInviteCondition")));

	if (TB_KakaoFriendTip.IsValid())
		TB_KakaoFriendTip->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_KakaoFriendInvite_FriendList")));
}

void UB2UIFriendKakaoInvite::BindDelegates()
{
	Super::BindDelegates();

	REGISTER_UITAB_BINDING(Tab_InviteReward.Get(), &UB2UIFriendKakaoInvite::OnRefreshInviteRewardList);
	REGISTER_UITAB_BINDING(Tab_JoinerReward.Get(), &UB2UIFriendKakaoInvite::OnRefreshJoinerRewardList);
}

void UB2UIFriendKakaoInvite::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UIFriendKakaoInvite);

#if PLATFORM_IOS
	FKakaoTalkPlugin::OnInviteFriendDelegate.Remove(OnKakaoInviteHandler);
	OnKakaoInviteHandler = FKakaoTalkPlugin::OnInviteFriendDelegate.AddUObject(this, &UB2UIFriendKakaoInvite::OnKakaoInvitePlatformCallback);

	FKakaoTalkPlugin::OnInvitationFriendInfoListDelegate.Remove(OnInvitionFriendListHandler);
	OnInvitionFriendListHandler = FKakaoTalkPlugin::OnInvitationFriendInfoListDelegate.AddUObject(this, &UB2UIFriendKakaoInvite::CreateInviteFriendList);

	FKakaoTalkPlugin::OnInvitationEventInfoDelegate.Remove(OnInvitionEventsHandler);
	OnInvitionEventsHandler = FKakaoTalkPlugin::OnInvitationEventInfoDelegate.AddUObject(this, &UB2UIFriendKakaoInvite::OnUpdateRewardList);
#endif
#if PLATFORM_ANDROID 
	FJavaWrapper::OnKakaoNewInviteCompletedDelegate.Remove(OnKakaoInviteHandler);
	OnKakaoInviteHandler = FJavaWrapper::OnKakaoNewInviteCompletedDelegate.AddUObject(this, &UB2UIFriendKakaoInvite::OnKakaoInvitePlatformCallback);

	// 여기에 처리 친구 목록
	FJavaWrapper::OnInviteKakaoFriendInfoCompletedDelegate.Remove(OnKakaoFriendInfoHandler);
	OnKakaoFriendInfoHandler = FJavaWrapper::OnInviteKakaoFriendInfoCompletedDelegate.AddUObject(this, &UB2UIFriendKakaoInvite::OnInviteFriendInfoCompletedCallback);

	FJavaWrapper::OnKakaoInvitationEventsCompletedDelegate.Remove(OnKakaoInviteEventHandler);
	OnKakaoInviteEventHandler = FJavaWrapper::OnKakaoInvitationEventsCompletedDelegate.AddUObject(this, &UB2UIFriendKakaoInvite::OnKakaoInvitationEventsCompletedCallback);
	
#endif

	UB2UIDocFriend* DocFriend = UB2UIDocHelper::GetDocFriend();
	if (DocFriend)
	{
		DocFriend->OnIsUpdateFriendInviteRewardInfoChanged.AddUObject(this, &UB2UIFriendKakaoInvite::OnChangeInviteRewardInfo);
	}

	DeliveryFriendInviteRewardTicket = DeliveryFriendInviteRewardClass<FB2ResponseFriendInviteRewardPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryFriendInviteReward, const FB2ResponseFriendInviteRewardPtr& RewardInfo)
		Capture->OnDeliveryInviteReward(RewardInfo);
	END_CAPTURE_OBJECT()
		);
}

void UB2UIFriendKakaoInvite::UnsubscribeEvent()
{
#if PLATFORM_IOS
	FKakaoTalkPlugin::OnInvitationFriendInfoListDelegate.Remove(OnInvitionFriendListHandler);
	FKakaoTalkPlugin::OnInviteFriendDelegate.Remove(OnKakaoInviteHandler);
	FKakaoTalkPlugin::OnInvitationEventInfoDelegate.Remove(OnInvitionEventsHandler);
#endif
#if PLATFORM_ANDROID // 진짜 kakao 로그인 delegate 등록
	FJavaWrapper::OnKakaoNewInviteCompletedDelegate.Remove(OnKakaoInviteHandler);
	FJavaWrapper::OnInviteKakaoFriendInfoCompletedDelegate.Remove(OnKakaoFriendInfoHandler);
	FJavaWrapper::OnKakaoInvitationEventsCompletedDelegate.Remove(OnKakaoInviteEventHandler);
	
#endif

	UB2UIDocFriend* DocFriend = UB2UIDocHelper::GetDocFriend();
	if (DocFriend)
	{
		DocFriend->OnIsUpdateFriendInviteRewardInfoChanged.RemoveAll(this);
	}

	DeliveryFriendInviteRewardClass<FB2ResponseFriendInviteRewardPtr>::GetInstance().Unsubscribe(DeliveryFriendInviteRewardTicket);
}	

void UB2UIFriendKakaoInvite::OnRefresh()
{
	if (Tab_InviteReward.IsValid())
		Tab_InviteReward->SetTabSelected(true);

	LoadFriendList();
	RefreshInfo();
}


void UB2UIFriendKakaoInvite::RefreshInfo()
{
	bool bSetText = false;
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	{
		const b2network::B2FriendInviteRewardInfoPtr RewardInfo = FriendDoc->GetFriendInviteRewardInfo();
		if (RewardInfo.get() != nullptr)
		{
			bSetText = true;
			if (TB_InviteFriendCount.IsValid())
				TB_InviteFriendCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ManCount")), FText::AsNumber(RewardInfo->invite_count)));
			if (TB_SuccessFriendCount.IsValid())
				TB_SuccessFriendCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ManCount")), FText::AsNumber(RewardInfo->joiner_count)));
		}
	}
	if (bSetText == false)
	{
		if (TB_InviteFriendCount.IsValid())
			TB_InviteFriendCount->SetText(FText::GetEmpty());
		if (TB_SuccessFriendCount.IsValid())
			TB_SuccessFriendCount->SetText(FText::GetEmpty());

	}
}

void UB2UIFriendKakaoInvite::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UB2UIFriendKakaoInvite::OnKakaoFriendInvite(const int32 RowIndex)
{
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		FText ReciverName;
		FString KakaoUserId;
		int32 EventID = 0;

		if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
		{
			FB2KakaoFriendInfo FriendInfo;
			FriendDoc->GetInviteFriendInfo(RowIndex, FriendInfo);

			ReciverName = FText::FromString(FriendInfo.sKakaoNickName);

			FriendDoc->SetCurSendInviteFriendRowIndex(RowIndex);
			FriendDoc->SetCurSendInviteFriendUUID(FriendInfo.sKakaoUserID);

			KakaoUserId = FriendInfo.sKakaoUserID;

			if (FriendDoc->HasKakaoEvent() == true)
			{
				FB2KakaoFriendEventInfo EventInfo = FriendDoc->GetRecentlyKakaoEvent();
				EventID = EventInfo.EventID;
			}
		}

		FText sPopupMessage = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Friend_KakaoFriendInvite_InviteMessage"))),
			ReciverName);

		UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
			sPopupMessage,
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::ConfirmOrCancel,
			FMsgPopupOnClick::CreateLambda([this, KakaoUserId, EventID]()
			{
				B2P_KakaoInviteNewMessage(KakaoUserId, EventID);

			// test code
#if WITH_EDITOR
				this->OnKakaoInvitePlatformCallback(200);
#endif

			})
		);
	}
}

void UB2UIFriendKakaoInvite::OnKakaoGetReward(const int32 RowIndex)
{
	TArray<b2network::B2mdKakaoFriendInviteRewardPtr> RewardList;
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	
	switch (CurRewardTabState)
	{
	case EKakaoRewardTabType::InviteReward:
		CliDataStore.GetKakaoFriendInviteRewardInfo(b2network::B2FriendInviteRewardType::INVITE_COUNT, RewardList);
		break;
	case EKakaoRewardTabType::JoinerReward:
		CliDataStore.GetKakaoFriendInviteRewardInfo(b2network::B2FriendInviteRewardType::JOINER_COUNT, RewardList);
		break;
	default:
		ensure(!TEXT("Check CurRewardTabState"));
		return;
	}

	if (RewardList.IsValidIndex(RowIndex))
	{
		const b2network::B2mdKakaoFriendInviteRewardPtr InviteRewardInfo = RewardList[RowIndex];

		data_trader::Retailer::GetInstance().RequestPlatformFriendInviteReward(b2network::B2PlatformCode::FOR_KAKAO, InviteRewardInfo->invite_reward_type, InviteRewardInfo->reward_index);
	}
}

void UB2UIFriendKakaoInvite::LoadFriendList()
{
	CurRewardTabState = EKakaoRewardTabType::InviteReward;
	SwitchTab(CurRewardTabState);

	CreateInviteRewardList();

	if(bIsLoadingFriendList == false)
	{
		bIsLoadingFriendList = true;
		B2P_RenewKakaoInviteFriend();
		B2p_RenewKakaoFriendEvent();
#if WITH_EDITOR
		CreateInviteFriendList();
#endif
	}
}

void UB2UIFriendKakaoInvite::OnUpdateRewardList()
{
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	{
		FriendDoc->RefreshKakaoEventInfo();
	}

	if (EKakaoRewardTabType::None == CurRewardTabState || EKakaoRewardTabType::Max == CurRewardTabState)
		CurRewardTabState = EKakaoRewardTabType::InviteReward;

	SwitchTab(CurRewardTabState);
	OnChangeInviteRewardInfo(nullptr, false, false);
}

void UB2UIFriendKakaoInvite::CreateJoinerRewardList()
{
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	{
		TArray<b2network::B2mdKakaoFriendInviteRewardPtr> JoinerList;
		FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
		CliDataStore.GetKakaoFriendInviteRewardInfo(b2network::B2FriendInviteRewardType::JOINER_COUNT, JoinerList);
		
		// 0행은 초대할때마다 보상받는 이벤트 정보임.
		const int32 EventIndex = 0;
		if (RewardRowList.IsValidIndex(EventIndex))
		{
			TArray<FB2KakaoFriendEventInfo> EventList;
			FriendDoc->GetKakaoFriendEventList(EventList);
			if (EventList.IsValidIndex(EventIndex))
			{
				const FB2KakaoFriendEventInfo &EventInfo = EventList[0];

				RewardRowList[EventIndex]->SetRewardInfoEveryJoin(FCString::Atoi(*EventInfo.HostRewardCode), KakaoInvitedRewardCount);
				RewardRowList[EventIndex]->SetRewardInfoEveryJoinTip(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("Friend_KakaoFriendRewardCondition")));
			}
		}

		const b2network::B2FriendInviteRewardInfoPtr RewardInfo = FriendDoc->GetFriendInviteRewardInfo();
		for (int32 Index = 0; Index < JoinerList.Num(); ++Index)
		{
			// 0행 건너 뜀
			int32 TargetRow = Index + 1;
			if (RewardRowList.IsValidIndex(TargetRow))
			{
				bool IsGetReward = FriendDoc->IsGetJoinerReward(Index);
				RewardRowList[TargetRow]->SetOwnerUI(this);
				RewardRowList[TargetRow]->SetRewardInfo(JoinerList[Index], false);
				RewardRowList[TargetRow]->SetRewardState(IsGetReward, RewardInfo->joiner_count, JoinerList[Index]->goal_count);
				RewardRowList[TargetRow]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				RewardRowList[TargetRow]->SetRowIndex(Index);
			}
		}

		// 나머지 는 가려줌.
		for (int32 Index = JoinerList.Num() + 1; Index < RewardRowList.Num(); ++Index)
		{
			if (RewardRowList.IsValidIndex(Index))
				RewardRowList[Index]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIFriendKakaoInvite::CreateInviteRewardList()
{
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	{
		const b2network::B2FriendInviteRewardInfoPtr RewardInfo = FriendDoc->GetFriendInviteRewardInfo();
		if (RewardInfo.get() == nullptr)
			return;

		TArray<b2network::B2mdKakaoFriendInviteRewardPtr> InviteList;
		FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
		CliDataStore.GetKakaoFriendInviteRewardInfo(b2network::B2FriendInviteRewardType::INVITE_COUNT, InviteList);
		
		for (int32 Index = 0; Index < InviteList.Num(); ++Index)
		{
			if (RewardRowList.IsValidIndex(Index))
			{
				bool IsGetReward = FriendDoc->IsGetInviteReward(Index);
				RewardRowList[Index]->SetOwnerUI(this);
				RewardRowList[Index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				RewardRowList[Index]->SetRewardInfo(InviteList[Index], true);
				RewardRowList[Index]->SetRewardState(IsGetReward, RewardInfo->invite_count, InviteList[Index]->goal_count);
				RewardRowList[Index]->SetRowIndex(Index);
			}
		}

		// 나머지 는 가려줌.
		for (int32 Index = InviteList.Num() + 1; Index < RewardRowList.Num(); ++Index)
		{
			if (RewardRowList.IsValidIndex(Index))
				RewardRowList[Index]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIFriendKakaoInvite::CreateInviteFriendList()
{
	ClearInviteFriendList();

	if (!SB_FriendList.IsValid())
		return;

	TArray<FB2KakaoFriendInfo> KakaoFriendList;
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	{
		FriendDoc->RefreshKakaoFriendInfo();
		FriendDoc->GetKakaoFriendList(KakaoFriendList);
	}

	ProfileManagerPtr->ClearAll();

	SB_FriendList->ScrollToStart();

	// 2000명이상 친구목록을 읽어보면 SDK상에서 잘못된 친구 목록을 리턴해서
	// 카카오측과 협의 후 2000이상은 안보이게 수정함..
	int32 MaxCount = FMath::Min(2000, KakaoFriendList.Num());
	CreateFriendRows(0, MaxCount);
	
	// 이미지 다운로드 시작~
	ProfileManagerPtr->Start();
}

void UB2UIFriendKakaoInvite::ClearInviteFriendList()
{
	if (SB_FriendList.IsValid())
		SB_FriendList->ClearChildren();
}

void UB2UIFriendKakaoInvite::SetInvisibleFriendRows()
{
	if (SB_FriendList.IsValid() == false)
		return;

	int32 MaxCount = SB_FriendList->GetChildrenCount();
	if (FMath::Max(0, MaxCount) < 1)
		return;

	float SizeCount = SB_FriendList->GetChildAt(0)->GetDesiredSize().Y;
	float Offset = SB_FriendList->GetScrollOffset();

	bool bScrollDown = SB_FriendList->GetScrollVelocity() > 0.0f;

	// 스크롤박스 상단
	int32 ScrolledIndex = FMath::RoundToInt(Offset / SizeCount);
	for (int32 i = ScrolledIndex - 1;  i > -1 ; --i)
	{
		if (FriendRowList[i].IsValid() == false)
			continue;

		if (FriendRowList[i]->GetVisibility() == ESlateVisibility::Hidden)
			break;

		if (FriendRowList[i]->GetVisibility() != ESlateVisibility::Hidden)
			FriendRowList[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	// 스크롤박스 클리핑 영역안 (7개는 보여줌)
	int32 VisibleMax = FMath::Min(MaxCount, ScrolledIndex + 7);
	for (int32 i = ScrolledIndex; i < VisibleMax; ++i)
	{
		if (FriendRowList[i].IsValid() == false)
			continue;
		if (FriendRowList[i]->GetVisibility() == ESlateVisibility::Hidden)
			FriendRowList[i]->SetVisibility(ESlateVisibility::Visible);
	}
	
	// 스크롤박스 하단
	int MaxBottomCount = FMath::Min(VisibleMax + 5, MaxCount);
	for (int32 i = VisibleMax; i < MaxBottomCount; ++i)
	{
		if (FriendRowList[i].IsValid() == false)
			continue;
		if (FriendRowList[i]->GetVisibility() == ESlateVisibility::Hidden)
			break;
		if (FriendRowList[i]->GetVisibility() != ESlateVisibility::Hidden)
			FriendRowList[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIFriendKakaoInvite::OnScrolledOffset(float Offset)
{
	SetInvisibleFriendRows();
}

void UB2UIFriendKakaoInvite::OnEndScroll()
{
}

void UB2UIFriendKakaoInvite::CreateFriendRows(int32 StartIndex, int32 EndIndex)
{
	TArray<FB2KakaoFriendInfo> KakaoFriendList;
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	{
		FriendDoc->GetKakaoFriendList(KakaoFriendList);
	}
	for (int32 Index = StartIndex; Index < EndIndex; ++Index)
	{
		UB2UIKakaoFriendInviteRow* Row = CreateWidget<UB2UIKakaoFriendInviteRow>(GetWorld(), RowContentClass);
		if (Row == nullptr)
			continue;

		const FB2KakaoFriendInfo &FriendInfo = KakaoFriendList[Index];
		Row->Init();
		Row->SetOwnerUI(this);
		Row->SetRowIndex(Index);
		Row->SetUserInfo(FriendInfo);

		UScrollBoxSlot* AddedRowSlot = Cast<UScrollBoxSlot>(SB_FriendList->AddChild(Row));
		FriendRowList.Add(Row);
		Row->SetupManualScrollBoxSender_byReceiver(this, SB_FriendList.Get());

		// 프로필 이미지 다운로드 정보 셋팅
		std::shared_ptr<FB2KakaoProfileDownloadInfo> DownloadInfo = std::make_shared<FB2KakaoProfileDownloadInfo>();
		DownloadInfo->SetIndex(Index);
		DownloadInfo->SetImageUrl(FriendInfo.sThumbnailImageUrl);
		DownloadInfo->SetTargetRow(Row);
		ProfileManagerPtr->PushDownloadInfo(DownloadInfo);
	}
}

void UB2UIFriendKakaoInvite::SwitchTab(EKakaoRewardTabType TabType)
{
	if (Tab_InviteReward.IsValid())
	{
		Tab_InviteReward->SetTabSelected(false);
		Tab_InviteReward->SetIsTabEnable(true);
	}
	if (Tab_JoinerReward.IsValid())
	{
		Tab_JoinerReward->SetTabSelected(false);
		Tab_JoinerReward->SetIsTabEnable(true);
	}

	CurRewardTabState = TabType;

	switch (CurRewardTabState)
	{
	case EKakaoRewardTabType::InviteReward:
		if (Tab_InviteReward.IsValid())
		{
			Tab_InviteReward->SetTabSelected(true);
			//Tab_InviteReward->SetIsTabEnable(false);
		}
		break;
	case EKakaoRewardTabType::JoinerReward:
		if (Tab_JoinerReward.IsValid())
		{
			Tab_JoinerReward->SetTabSelected(true);
			//Tab_JoinerReward->SetIsTabEnable(false);
		}
		break;
	default:
		break;
	}
}

void UB2UIFriendKakaoInvite::OnRefreshInviteRewardList()
{
	CurRewardTabState = EKakaoRewardTabType::InviteReward;
	CreateInviteRewardList();
}

void UB2UIFriendKakaoInvite::OnRefreshJoinerRewardList()
{
	CurRewardTabState = EKakaoRewardTabType::JoinerReward;
	CreateJoinerRewardList();
}

void UB2UIFriendKakaoInvite::OnChangeInviteRewardInfo(class UB2UIDocBase* Sender, bool CurrState, bool PrevState)
{
	switch (CurRewardTabState)
	{
	case EKakaoRewardTabType::InviteReward:
		OnRefreshInviteRewardList();
		break;
	case EKakaoRewardTabType::JoinerReward:
		OnRefreshJoinerRewardList();
		break;
	default:
		break;
	}

	RefreshInfo();
}

void ShowSimplePopup(FString MessageKey)
{
	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, MessageKey),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
		);
}

#if PLATFORM_ANDROID
void UB2UIFriendKakaoInvite::OnKakaoInvitePlatformCallback(JNIEnv *env, jobject thiz, jint resultCode)
{
	const int32 RealCode = resultCode;
	OnKakaoInvitePlatformCallback(RealCode);
}

void UB2UIFriendKakaoInvite::OnInviteFriendInfoCompletedCallback(JNIEnv *env, jobject thiz)
{
	CreateInviteFriendList();
}

void UB2UIFriendKakaoInvite::OnKakaoInvitationEventsCompletedCallback(JNIEnv *env, jobject thiz)
{
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("OnKakaoInvitationEventsCompletedCallback...  "));
	OnUpdateRewardList();
}

#endif

void UB2UIFriendKakaoInvite::OnKakaoInvitePlatformCallback(const int32 ResultCode)
{
	FText DialogMessage;

	EKakaoInviteResultCode Code = static_cast<EKakaoInviteResultCode>(ResultCode);
	if (Code == EKakaoInviteResultCode::Success)
	{
		if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
		{
			const int32 RowIndex = FriendDoc->GetCurSendInviteFriendRowIndex();
			if (FriendRowList.IsValidIndex(RowIndex) && FriendRowList[RowIndex].IsValid())
				FriendRowList[RowIndex]->SetKakaoInviteBtn(false);
			if (ProfileManagerPtr)
				ProfileManagerPtr->SetSkipDownload(RowIndex);
			
			FriendDoc->AddInviteKakaoFriend(RowIndex);
		}

		// 초대 숫자 갱신해줌
		data_trader::Retailer::GetInstance().RequestPlatformFriendInviteCountUp(b2network::B2PlatformCode::FOR_KAKAO);
		ShowSimplePopup(TEXT("FriendAdd"));
	}
	else
	{
		FString ErrorText = FString(TEXT("Friend_KakaoFriendInviteFailed"));
		switch (Code)
		{
		case EKakaoInviteResultCode::ExceedMonthlyUsage:	// 월간 허용 사용량 초과
			ErrorText = FString(TEXT("Friend_KakaoFriendInviteCondition2"));
			break;
		case EKakaoInviteResultCode::ExceedDailyUsage:		// 일간 허용 사용량 초과
			ErrorText = FString(TEXT("Friend_KakaoFriendInviteExceed"));
			break;
		case EKakaoInviteResultCode::SettingDisabled:		// 수신 차단 상태
			ErrorText = FString(TEXT("Friend_KakaoFriendInviteDenied"));
			break;
// 		case EKakaoInviteResultCode::NotKakaoTalkUser:		// 카카오톡 유저 아님 (이건안씀)
//		case EKakaoInviteResultCode::Error:
// 			break;
		default:// 모든 에러
			ErrorText = FString(TEXT("Friend_KakaoFriendInviteFailed"));
			break;
		}

		ShowSimplePopup(ErrorText);
	}
}

void UB2UIFriendKakaoInvite::OnDeliveryInviteReward(const FB2ResponseFriendInviteRewardPtr &RewardInfo)
{
	if (auto FriendDoc = UB2UIDocHelper::GetDocFriend())
	{
		DevShowNotReadyYetMessage(TEXT("General_GetMailReward"));
	}
}