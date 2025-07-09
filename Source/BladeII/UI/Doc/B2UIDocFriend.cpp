// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "Event.h"
#include "B2UIDocFriend.h"
#include "B2PlatformAPI.h"
#include "Json.h"
#include "Retailer.h"
#include "BladeIIGameImpl.h"
//#include "B2GameInstance.h"
#include "BladeIIUtil.h"

#if PLATFORM_IOS
#include "KakaoTalkPlugin.h"
#endif

#define DEFAULT_INVITE_EVENT_ID		134

UB2UIDocFriend::UB2UIDocFriend(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurSendInviteFriendRowIndex = 0;
}

UB2UIDocFriend::~UB2UIDocFriend()
{
}

void UB2UIDocFriend::Init()
{
	Super::Init();

	CAPTURE_UOBJECT(UB2UIDocFriend);

	DeliveryGetFriendListTicket = DeliveryGetFriendListClass<FB2FriendList>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetFriendList, const FB2FriendList& FriendList)
		Capture->OnDeliveryGetFriendList(FriendList);
	END_CAPTURE_OBJECT()
		);

	DeliveryGetAllReceiveAskFriendTicket = DeliveryGetAllReceiveAskFriendClass<FB2AllReceiveAskFriendList>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetAllReceiveAskFriend, const FB2AllReceiveAskFriendList& FriendList)
		Capture->OnDeliveryGetAllReceiveAskFriend(FriendList);
	END_CAPTURE_OBJECT()
		);

	DeliveryGetAllSendAskFriendTicket = DeliveryGetAllSendAskFriendClass<FBAllSendAskFriendList>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryGetAllSendAskFriend, const FBAllSendAskFriendList& FriendList)
		Capture->OnDeliveryGetAllSendAskFriend(FriendList);
	END_CAPTURE_OBJECT()
		);

	CacheSendAskFriendAccountIdTicket = CacheSendAskFriendAccountIdClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(CacheSendAskFriendAccountId, int64 InAccountId)
		Capture->AddFriendSendInfo(InAccountId, FB2UserPtr(new b2network::B2User));
	END_CAPTURE_OBJECT()
		);
	RemoveSendAskFriendAccountIdTicket = RemoveSendAskFriendAccountIdClass<int64>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(RemoveSendAskFriendAccountId, int64 InAccountId)
		Capture->RemoveFriendSendInfo(InAccountId);
	END_CAPTURE_OBJECT()
		);

	DeliveryPlatformFriendInviteInfoTicket = DeliveryPlatformFriendInviteInfoClass<FB2ResponseFriendInviteRewardInfoPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryPlatformFriendInviteInfo, const FB2ResponseFriendInviteRewardInfoPtr& RewardInfo)
		Capture->OnDeliveryPlatformFriendInviteInfo(RewardInfo->invite_reward_info);
	END_CAPTURE_OBJECT()
		);

	DeliveryKakaoInviteFriendTicket = DeliveryKakaoInviteFriendClass<EKakaoInviteResultCode>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryKakaoInviteFriend, const EKakaoInviteResultCode ResultCode)
		Capture->OnDeliveryInviteKakaoResult(ResultCode);
	END_CAPTURE_OBJECT()
		);
	DeliveryFriendInviteCountUpTicket = DeliveryFriendInviteCountUpClass<FB2ResponseFriendInviteCountUpPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryFriendInviteCountUp, const FB2ResponseFriendInviteCountUpPtr& RewardInfo)
		Capture->OnDeliveryPlatformFriendInviteInfo(RewardInfo->invite_reward_info);
	END_CAPTURE_OBJECT()
		);
	DeliveryFriendJoinerCountUpTicket = DeliveryFriendJoinerCountUpClass<FB2ResponseFriendJoinerCountUpPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryFriendJoinerCountUp, const FB2ResponseFriendJoinerCountUpPtr& RewardInfo)
		Capture->OnDeliveryPlatformFriendInviteInfo(RewardInfo->invite_reward_info);
	END_CAPTURE_OBJECT()
		);
	DeliveryFriendInviteRewardTicket = DeliveryFriendInviteRewardClass<FB2ResponseFriendInviteRewardPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryFriendInviteReward, const FB2ResponseFriendInviteRewardPtr& RewardInfo)
		Capture->OnDeliveryInviteReward(RewardInfo);
	END_CAPTURE_OBJECT()
		);

	SetKakaoFriendInfoData();
	SetKakaoFriendEventData();
}

void UB2UIDocFriend::Destroy()
{
	Super::Destroy();
	DeliveryPlatformFriendInviteInfoClass<FB2ResponseFriendInviteRewardInfoPtr>::GetInstance().Unsubscribe(DeliveryPlatformFriendInviteInfoTicket);
	DeliveryGetFriendListClass<FB2FriendList>::GetInstance().Unsubscribe(DeliveryGetFriendListTicket);
	DeliveryGetAllReceiveAskFriendClass<FB2AllReceiveAskFriendList>::GetInstance().Unsubscribe(DeliveryGetAllReceiveAskFriendTicket);
	DeliveryGetAllSendAskFriendClass<FBAllSendAskFriendList>::GetInstance().Unsubscribe(DeliveryGetAllSendAskFriendTicket);
	DeliveryFriendInviteCountUpClass<FB2ResponseFriendInviteCountUpPtr>::GetInstance().Unsubscribe(DeliveryFriendInviteCountUpTicket);
	DeliveryFriendJoinerCountUpClass<FB2ResponseFriendJoinerCountUpPtr>::GetInstance().Unsubscribe(DeliveryFriendJoinerCountUpTicket);
	DeliveryFriendInviteRewardClass<FB2ResponseFriendInviteRewardPtr>::GetInstance().Unsubscribe(DeliveryFriendInviteRewardTicket);
}

TMap<int64, FB2FriendPtr> UB2UIDocFriend::GetFriendList()
{
	return m_ptrFriendList;
}

FB2FriendPtr UB2UIDocFriend::GetFriendInfo(int64 AccountId)
{
	return m_ptrFriendList.Contains(AccountId) ? m_ptrFriendList[AccountId] : nullptr;
}

void UB2UIDocFriend::AddFriendInfo(int32 AccountId, FB2FriendPtr FriendInfo)
{
	m_ptrFriendList.Add(AccountId, FriendInfo);
}

TMap<int64, FB2UserPtr> UB2UIDocFriend::GetFriendSendListInfo()
{
	return m_ptrFriendSendList;
}

FB2UserPtr UB2UIDocFriend::GetFriendSendInfo(int64 AccountId)
{
	return m_ptrFriendSendList.Contains(AccountId) ? m_ptrFriendSendList[AccountId] : nullptr;
}

void UB2UIDocFriend::AddFriendSendInfo(int32 AccountId, FB2UserPtr FriendInfo)
{
	if (m_ptrFriendSendList.Contains(AccountId))
		m_ptrFriendSendList[AccountId] = FriendInfo;
	else
		m_ptrFriendSendList.Add(AccountId, FriendInfo);
}

void UB2UIDocFriend::RemoveFriendSendInfo(int32 AccountId)
{
	if (m_ptrFriendSendList.Contains(AccountId) == false)
		return;

	m_ptrFriendSendList.Remove(AccountId);
}

TMap<int64, FB2UserPtr> UB2UIDocFriend::GetAllReceiveAskFriendList()
{
	return m_ptrReceiveAskFriendList;
}

FB2UserPtr UB2UIDocFriend::GetReceiveAskFriendInfo(int64 AccountId)
{
	return m_ptrReceiveAskFriendList.Contains(AccountId) ? m_ptrReceiveAskFriendList[AccountId] : nullptr;
}

void UB2UIDocFriend::AddReceiveAskFriendInfo(int32 AccountId, FB2UserPtr FriendInfo)
{
	m_ptrReceiveAskFriendList.Add(AccountId, FriendInfo);
}

bool UB2UIDocFriend::IsFriendAskable(int64 AccountId)
{
	if (AccountId == BladeIIGameImpl::GetClientDataStore().GetAccountId())
		return false;

	// 내친구 목록에 없고, 보낸친구신청, 받은친구신청에 없어야함.
	return GetFriendInfo(AccountId) == NULL
		&& GetFriendSendInfo(AccountId) == NULL
		&& GetReceiveAskFriendInfo(AccountId) == NULL;
}

bool UB2UIDocFriend::IsMyFriend(int64 AccountId)
{
	if (AccountId == BladeIIGameImpl::GetClientDataStore().GetAccountId())
		return false;

	return GetFriendInfo(AccountId) == NULL;
}

void UB2UIDocFriend::OnDeliveryGetFriendList(const FB2FriendList& FriendList)
{
	//TArray<FB2FriendPtr> arFriendList = GET_TUPLE_DATA(FB2ResponseGetFriendList::friends_index, FriendList);
	//m_ptrFriendList.Empty();
	//for (auto El : arFriendList)
	//{
	//	int64 accountid = El->user->character_summary->account_id;
	//	m_ptrFriendList.Add(accountid, El);
	//}
}

void UB2UIDocFriend::OnDeliveryGetAllReceiveAskFriend(const FB2AllReceiveAskFriendList& FriendList)
{
	//TArray<FB2UserPtr> arFriendList = GET_TUPLE_DATA(FB2ResponseGetAllSendAskFriend::acceptors_index, FriendList);
	//m_ptrReceiveAskFriendList.Empty();
	//for (auto El : arFriendList)
	//{
	//	int64 accountid = El->character_summary->account_id;
	//	m_ptrReceiveAskFriendList.Add(accountid, El);
	//}
}

void UB2UIDocFriend::OnDeliveryGetAllSendAskFriend(const FBAllSendAskFriendList& FriendList)
{
	//TArray<FB2UserPtr> arFriendList = GET_TUPLE_DATA(FB2ResponseGetAllReceiveAskFriend::requestors_index, FriendList);

	//m_ptrFriendSendList.Empty();
	//for (auto El : arFriendList)
	//{
	//	int64 accountid = El->character_summary->account_id;
	//	m_ptrFriendSendList.Add(accountid, El);
	//}
}

bool UB2UIDocFriend::GetAbleToSendFriendPoint()
{
	//FDateTime ServerTimeNow = UB2GameInstance::GetUTCNow();
	//int32 CooltimeForSendSocialPoint = BladeIIGameImpl::GetClientDataStore().GetCooltimeForSendSocialPoint();

	//for (TMap<int64, FB2FriendPtr>::TIterator CII(m_ptrFriendList); CII; ++CII)
	//{
	//	FB2FriendPtr& FriendInfo = CII.Value();
	//	FDateTime timePointSend = FDateTime::FromUnixTimestamp(FriendInfo->last_social_point_send_time / 1000);

	//	FTimespan timeElapse = ServerTimeNow - timePointSend;

	//	if (timeElapse >= FTimespan::FromHours(CooltimeForSendSocialPoint))
	//		return true;
	//}
	return false;
}

bool UB2UIDocFriend::GetIsNewFriend()
{
	for (TMap<int64, FB2FriendPtr>::TIterator CII(m_ptrFriendList); CII; ++CII)
	{
		FB2FriendPtr& FriendInfo = CII.Value();
		if (FriendInfo->is_new)
			return true;
	}
	return false;
}

bool UB2UIDocFriend::GetIsAskFriend()
{
	return (m_ptrReceiveAskFriendList.Num() > 0);
}

void UB2UIDocFriend::OnForcedFriendSocialPointInfo(int64 AccountId, int64 LastSendPointTime)
{
	if (m_ptrFriendList.Contains(AccountId))
		m_ptrFriendList[AccountId]->last_social_point_send_time = LastSendPointTime*1000;
}

void UB2UIDocFriend::OnForcedAddFriendInfo(int64 AccountId)
{
	if (!m_ptrFriendList.Contains(AccountId))
	{
		auto FriendInfo = std::make_shared<b2network::B2Friend>();
		FriendInfo->is_new = true;
		AddFriendInfo(AccountId,FriendInfo);
	}
}

void UB2UIDocFriend::OnForcedDeleteFriendInfo(int64 AccountId)
{
	if (m_ptrFriendList.Contains(AccountId))
		m_ptrFriendList.Remove(AccountId);
}

void UB2UIDocFriend::OnForcedDeleteAskFriend(int64 AccountId)
{
	if (m_ptrReceiveAskFriendList.Contains(AccountId))
		m_ptrReceiveAskFriendList.Remove(AccountId);
}

void UB2UIDocFriend::OnForcedRemoveNewFriend(int64 AccountId)
{
	if (m_ptrFriendList.Contains(AccountId))
		m_ptrFriendList[AccountId]->is_new = false;
}

void UB2UIDocFriend::SetKakaoFriendInfoData()
{
	LoadLocalInvitedFriends();

#if WITH_EDITOR && BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	if (m_KakaoFriendInfoList.Num() > 1)
		m_KakaoFriendInfoList.Empty();
	// 더미 데이터
	for (int32 i = 0; i < 100; ++i)
	{
		FB2KakaoFriendInfo Row;
		Row.sKakaoUserID = FString::FromInt(i);
		Row.sKakaoNickName = FString::FromInt(i);

		if (FMath::RandBool() == true)
			Row.sThumbnailImageUrl = TEXT("http://issue.chosun.com/poll//data/image/solve/solve_30_20170728191124.jpg");

		Row.bInvited = m_KakaoInvitedSet.Contains(Row.sKakaoUserID);

		m_KakaoFriendInfoList.Add(Row);
	}
	return;
#endif

	if (m_KakaoFriendInfoList.Num() > 1)
		m_KakaoFriendInfoList.Empty();
	for (int i = 0; i < B2P_GetKakaoFriendCount(); i++)
	{
		FB2KakaoFriendInfo sKakaoTemp;

		TSharedPtr<FJsonObject> json_parse_obj;
		TSharedRef<TJsonReader<TCHAR>> json_reader = TJsonReaderFactory<TCHAR>::Create(B2P_GetKakaoFriendInfo(i));
		if (FJsonSerializer::Deserialize(json_reader, json_parse_obj))
		{
			sKakaoTemp.sKakaoUserID = json_parse_obj->GetStringField("serviceUserId");
#if PLATFORM_ANDROID
			sKakaoTemp.sKakaoNickName = B2P_GetKakaoFriendNickName(i);// 한글 깨짐 때문에 따로
#else
			sKakaoTemp.sKakaoNickName = json_parse_obj->GetStringField("nickname");
#endif
			sKakaoTemp.sThumbnailImageUrl = json_parse_obj->GetStringField("thumbnailImageUrl");
			sKakaoTemp.bInvited = m_KakaoInvitedSet.Contains(sKakaoTemp.sKakaoUserID);
			m_KakaoFriendInfoList.Add(sKakaoTemp);
		}
	}
}

void UB2UIDocFriend::SetKakaoFriendEventData()
{
	m_KakaoFriendEventInfoList.Empty();

	for (int i = 0; i < B2P_GetKakaoFriendEventCount(); i++)
	{
		
		TSharedPtr<FJsonObject> json_parse_obj;
		TSharedRef<TJsonReader<TCHAR>> json_reader = TJsonReaderFactory<TCHAR>::Create(B2P_GetKakaoFriendEventInfo(i));
		if (FJsonSerializer::Deserialize(json_reader, json_parse_obj))
		{
			FB2KakaoFriendEventInfo sKakaoTemp;
			
 			sKakaoTemp.EventID = json_parse_obj->GetIntegerField("eventId");
			sKakaoTemp.IsEventCardEnabled = json_parse_obj->GetBoolField("isEventCardEnabled");
 			sKakaoTemp.StartTime = static_cast<int64>(json_parse_obj->GetNumberField("startTime"));
 			sKakaoTemp.EndTime = static_cast<int64>(json_parse_obj->GetNumberField("finishTime"));
 			sKakaoTemp.MaxHostRewardCount = json_parse_obj->GetIntegerField("maxHostRewardCount");
 			sKakaoTemp.HostRewardCode = json_parse_obj->GetStringField("hostRewardCode");
 			sKakaoTemp.JoinerRewardCode = json_parse_obj->GetStringField("joinRewardCode");
 			sKakaoTemp.InvitationUrl = json_parse_obj->GetStringField("invitationUrl");
 			sKakaoTemp.TotalJoinerCount = json_parse_obj->GetIntegerField("totalJoinerCount");

			m_KakaoFriendEventInfoList.Add(sKakaoTemp);
		}
	}


	if (InviteRewardInfo.get() && HasKakaoEvent())
	{
		FB2KakaoFriendEventInfo sKakaoTemp = GetRecentlyKakaoEvent();

		if (sKakaoTemp.TotalJoinerCount > InviteRewardInfo->joiner_count)
		{
			data_trader::Retailer::GetInstance().RequestPlatformFriendJoinerCountUp(b2network::B2PlatformCode::FOR_KAKAO, sKakaoTemp.TotalJoinerCount);
		}		
	}
}

void UB2UIDocFriend::GetKakaoFriendList(TArray<FB2KakaoFriendInfo> &OutList)
{
	OutList.Empty();

	for (auto& Each : m_KakaoFriendInfoList)
	{
		OutList.Add(Each);
	}
}

void UB2UIDocFriend::GetKakaoFriendEventList(TArray<FB2KakaoFriendEventInfo> &OutList)
{
	OutList.Empty();
#if WITH_EDITOR && BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// 에디터용 더미 데이터
	FB2KakaoFriendEventInfo Info;
	// ios 로그
	// <KGKakaoInvitationEvent : eventId(153), isEventCardEnabled(NO), startTime(1517466098000), finishTime(2531646041000), maxHostRewardCount(50), hostRewardCode(10001), joinRewardCode(10001), invitationUrl(http://kko.to/PgOAWUH60), totalJoinerCount(0)>
	Info.EventID = DEFAULT_INVITE_EVENT_ID;
	Info.IsEventCardEnabled = false;
	Info.StartTime = 1517466098000;
	Info.EndTime = 2531646041000;
	Info.MaxHostRewardCount = 50;
	Info.HostRewardCode = TEXT("10001");
	Info.JoinerRewardCode = TEXT("10001");
	Info.InvitationUrl = TEXT("http://kko.to/PgOAWUH60");
	Info.TotalJoinerCount = 1;
	OutList.Add(Info);
	return;
#endif

	for (auto& Each : m_KakaoFriendEventInfoList)
	{
		OutList.Add(Each);
	}
}

FB2KakaoFriendEventInfo UB2UIDocFriend::GetRecentlyKakaoEvent()
{
	if (m_KakaoFriendEventInfoList.Num() < 1)
	{
		FB2KakaoFriendEventInfo Temp;
		Temp.EventID = DEFAULT_INVITE_EVENT_ID;
//		ensure(!"check event.");
		return Temp;
	}
	m_KakaoFriendEventInfoList.Sort([](const FB2KakaoFriendEventInfo& A, const FB2KakaoFriendEventInfo& B)
	{
		return A.EventID > B.EventID ? true : false;
	});

#if WITH_EDITOR || BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	for (auto Each : m_KakaoFriendEventInfoList)
	{
		UE_LOG(LogBladeII, Log, TEXT("@@@ Event ID : %d"), Each.EventID);
	}
#endif
	
	return m_KakaoFriendEventInfoList[0];
}

bool  UB2UIDocFriend::HasKakaoEvent()
{
#if WITH_EDITOR
	// 에디터에서는 테스트를 위해 있다고 퉁침
	return 1;
#endif
	return m_KakaoFriendEventInfoList.Num() > 0;
}

void UB2UIDocFriend::RemoveInviteFriendInfo(const int32 RowIndex)
{
	if (m_KakaoFriendInfoList.IsValidIndex(RowIndex))
	{
		m_KakaoFriendInfoList.RemoveAt(RowIndex);
	}
}

bool UB2UIDocFriend::IsGetInviteReward(const int32 RewardIndex)
{
	//if (InviteRewardInfo.get() != nullptr)
	//{
	//	if ((InviteRewardInfo->invite_reward_mask & (1 << RewardIndex)) != 0)
	//		return true;
	//}
	return false;
}

bool UB2UIDocFriend::IsGetJoinerReward(const int32 RewardIndex)
{
	//if (InviteRewardInfo.get() != nullptr)
	//{
	//	if ((InviteRewardInfo->joiner_reward_mask & (1 << RewardIndex)) != 0)
	//		return true;
	//}
	return false;
}

void UB2UIDocFriend::OnDeliveryPlatformFriendInviteInfo(const FB2FriendInviteRewardInfoPtr& RewardInfo)
{
	InviteRewardInfo = RewardInfo;

	SetIsUpdateFriendInviteRewardInfo(true);
}

b2network::B2FriendInviteRewardInfoPtr UB2UIDocFriend::GetFriendInviteRewardInfo()
{
	return InviteRewardInfo;
}

void UB2UIDocFriend::GetInviteFriendInfo(const int32 RowIndex, FB2KakaoFriendInfo &FriendInfo)
{
#if WITH_EDITOR && BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	TArray<FB2KakaoFriendInfo> KakaoFriendList;
	GetKakaoFriendList(KakaoFriendList);

	if (KakaoFriendList.IsValidIndex(RowIndex) == false)
		return;
	FriendInfo = KakaoFriendList[RowIndex];
	return;
#endif


	if (m_KakaoFriendInfoList.IsValidIndex(RowIndex) == false)
		return;
	FriendInfo = m_KakaoFriendInfoList[RowIndex];
}

void UB2UIDocFriend::OnDeliveryInviteKakaoResult(EKakaoInviteResultCode  Code)
{

}

void UB2UIDocFriend::OnDeliveryInviteReward(const FB2ResponseFriendInviteRewardPtr &RewardInfo)
{
	InviteRewardInfo = RewardInfo->invite_reward_info;

	SetIsUpdateFriendInviteRewardInfo(true);
}

void UB2UIDocFriend::AddInviteKakaoFriend(const int32 TargetIndex)
{
	//if (m_KakaoFriendInfoList.IsValidIndex(TargetIndex) == false)
	//	return;
	//FString UserID = m_KakaoFriendInfoList[TargetIndex].sKakaoUserID;
	//if (m_KakaoInvitedSet.Contains(UserID))
	//	return;

	//m_KakaoInvitedSet.Add(UserID);

	//FDateTime DateNow = UB2GameInstance::GetKoreaTimeNow();

	//FString SaveDataInfo = FString::Printf(TEXT("%s##%s"), *UserID, *DateNow.ToString());
	//m_KakaoInvietedInfoList.Add(SaveDataInfo);

	//// 캐싱 해 두고 씬 전환시 일괄적으로 저장을 하면
	//// 앱을 강제 종료할 경우 저장이 안되어 친구초대 마다 저장하도록 함.
	//SaveLocalInvitedFriends();
}

void UB2UIDocFriend::SaveLocalInvitedFriends()
{
	//TArray<FString> TempArray;

	//for (auto& Elem : m_KakaoInvietedInfoList)
	//{
	//	TempArray.Add(*Elem);
	//}

	//SaveGameData_KakaoInvitedFriends(TempArray);
}

int32 GetDistanceInvitedDay(const FString CurrentDateString, const FDateTime &TargetDate)
{
	FDateTime EachTime;
	if (FDateTime::Parse(CurrentDateString, EachTime))
	{
		FTimespan DistDays = TargetDate - EachTime;

		return DistDays.GetDays();
	}

	return INT32_MAX;
}

void UB2UIDocFriend::LoadLocalInvitedFriends()
{
	//if (m_KakaoInvitedSet.Num() > 0)
	//	m_KakaoInvitedSet.Empty();

	//if (m_KakaoInvietedInfoList.Num() > 0)
	//	m_KakaoInvietedInfoList.Empty();

	//TArray<FString> TempArray;
	//if (LoadGameData_KakaoInvitedFriends(TempArray))
	//{
	//	FString AVStr_Left;
	//	FString AVStr_Right;

	//	FDateTime DateNow = UB2GameInstance::GetKoreaTimeNow();
	//	//FDateTime::Parse(TEXT("2018.08.01-00.00.01"), DateNow); // 테스트용

	//	const int32 InviteDayCondition = 31;
	//	for (FString Each : TempArray)
	//	{
	//		if (Each.Split(TEXT("##"), &AVStr_Left, &AVStr_Right))
	//		{
	//			if (m_KakaoInvitedSet.Contains(AVStr_Left))
	//				continue;

	//			// 31일 이후는 조건문에서 안들어가 자동으로 빠짐
	//			if(GetDistanceInvitedDay(AVStr_Right, DateNow) < InviteDayCondition)
	//			{
	//				m_KakaoInvitedSet.Add(AVStr_Left);

	//				m_KakaoInvietedInfoList.Add(Each);
	//			}			
	//		}
	//	}
	//}
}