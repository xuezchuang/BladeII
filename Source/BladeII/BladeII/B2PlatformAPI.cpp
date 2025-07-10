#include "B2PlatformAPI.h"
#include "Event.h"

#if PLATFORM_ANDROID
#include "AndroidRuntimeSettings.h"
#include "Android/AndroidJNI.h"

#elif PLATFORM_IOS
#include "IOSRuntimeSettings.h"
#include "KakaoTalkPlugin.h"
#include "IOSAppDelegate.h"
#endif



#if PLATFORM_ANDROID
extern bool AndroidThunkCpp_CheckPermission();
extern void AndroidThunkCpp_PaltformPermission();
extern void AndroidThunkCpp_GameExit();
extern bool AndroidThunkCpp_SupportTypeASTC();
extern void AndroidThunkCpp_KakaoLogin();
extern void AndroidThunkCpp_KakaoLogOut();
extern void AndroidThunkCpp_GoogleClientConnect();
extern void AndroidThunkCpp_GoogleClientDisconnect();
extern void AndroidThunkCpp_GoogleAchievement(const FString& sID);
extern void AndroidThunkCpp_ShowAchievementView();
extern void AndroidThunkCpp_KakaoCommunityView();
extern FString AndroidThunkCpp_GetKakaoUserID();
extern void AndroidThunkCpp__KakaoAccountLink();
extern int32 AndroidThunkCpp_GetKakaoIDPCode();
extern void AndroidThunkCpp_ShowKakaoCoupon();
extern void AndroidThunkCpp_LoadMessage(const FString& sBoxID);
extern void AndroidThunkCpp_SetClipboardString(const FString& sString);
extern FString AndroidThunkCpp_GetGameServerAddress();
extern FString AndroidThunkCpp_GetCDNAddress();
extern void AndroidThunkCpp_LevelTarcking(int32 nLevel, const FString& sCharacterType);
extern bool AndroidThunkCpp_IsGoogleLogin();
extern void AndroidThunkCpp_ExitPopup(const FString& sMessage);
extern void AndroidThunkCpp_KakaoUnregister();
extern void AndroidThunkCpp_ShowKakaoNoticeView(bool bOneDay, const FString& sNoticKey);
extern void AndroidThunkCpp_ShowKakaoEventView(const FString& sEventKey);
extern void AndroidThunkCpp_ShowKakaoPormotionView();
extern void AndroidThunkCpp_SetKakaoPushEnable(bool bDay, bool bSet);
extern bool AndroidThunkCpp_IsEnableKakaoPush(bool bDay);

extern FString AndroidThunkCpp_GetKakaoFriendInfo(int32 nIndex);
extern FString AndroidThunkCpp_GetKakaoFriendNickName(int32 nIndex);

extern int32 AndroidThunkCpp_GetKakaoFriendCount();

extern void AndroidThunkCpp_LaunchURL(const FString& URL);

extern void AndroidThunkCpp_KakaoSendNewInviteMessage(const FString& sKakaoUserID, int32 nEventID);
extern void AndroidThunkCpp_KakaoSendNewGameMessage(const FString& sKakaoUserID);
extern void AndroidThunkCpp_KakaoActionLog(const FString& sCategory, const FString& sAction, const FString& sLabel, const FString& sActionAttr1);


extern FString AndroidThunkCpp_GetKakaoLanguageCode();
extern FString AndroidThunkCpp_GetKakaoCountryCode();
extern FString AndroidThunkCpp_GetKakaoOsName();
extern FString AndroidThunkCpp_GetKakaoDeviceModel();

extern int32 AndroidThunkCpp_GetMarKetType();
extern void AndroidThunkCpp_KeepScreenOn(bool Enable);
extern void AndroidThunkCpp_ShowCsView();

extern void AndroidThunkCpp_SetOrientation(bool bSet);
extern bool AndroidThunkCpp_GetOrientationState();
extern void AndroidThunkCpp_ScreenCapture();

extern FString AndroidThunkCpp_GetKakaoInvitationEventInfo(int32 nIndex);
extern int32 AndroidThunkCpp_GetKakaoInvitationEventCount();

extern void AndroidThunkCpp_KakaoShowGachaView();

extern bool AndroidThunkCpp_IsinReview();
extern void AndroidThunkCpp_RenewKakaoInviteFriend();
extern void AndroidThunkCpp_RenewKakaoLoadInvitationEvents();

extern bool AndroidThunkCpp_IsViewPromotion();
extern FString AndroidThunkCpp_GetKakaoCustomData(const FString& sKey);

extern void AndroidThunkCpp_ShowEndingPormotion();

extern FString GExternalFilePath;

#elif PLATFORM_IOS

#endif

DEFINE_LOG_CATEGORY(LogB2ActionLog)

// android permission
bool B2P_CheckPermission()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_CheckPermission();
#else
	return true;
#endif
}

void B2P_InitPlatformPermission()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_PaltformPermission();
#endif
}

void B2P_GameExit()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_GameExit();
#elif PLATFORM_IOS
	exit(0);
#endif
}

FString B2P_GetExternalFilePath()
{
#if PLATFORM_ANDROID
	return GExternalFilePath;
#elif PLATFORM_IOS
	return TEXT("/");
#endif
	return FPaths::RootDir();
}

bool B2P_SupportTypeASTC()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_SupportTypeASTC();
#else
	return false;
#endif
}

// kakaoGame
void B2P_KakaoLogin()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_KakaoLogin();
#elif PLATFORM_IOS
	KakaotalkAPI::Login();
#endif
}

void B2P_KakaoLogout()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_KakaoLogOut();
#elif PLATFORM_IOS
	KakaotalkAPI::Logout();
#endif
}

void B2P_KakaoUnregister()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_KakaoUnregister();
#elif PLATFORM_IOS
	KakaotalkAPI::Unregister();
#endif
}

void B2P_KakaoAccountLink()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp__KakaoAccountLink();
#elif PLATFORM_IOS
	KakaotalkAPI::Connect();
#endif
}

EPlatformIDPCode B2P_GetKakaoIDPCode()
{
#if PLATFORM_ANDROID
	return (EPlatformIDPCode)AndroidThunkCpp_GetKakaoIDPCode();
#elif PLATFORM_IOS
	return static_cast<EPlatformIDPCode>(KakaotalkAPI::GetKakaoIDPCode());
#endif
	return EPlatformIDPCode::Guest;
}

void B2P_KakaoCommunityView()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_KakaoCommunityView();
#elif PLATFORM_IOS
	KakaotalkAPI::ShowKakaoCommunityView(TEXT(""));
#endif
}


bool B2P_IsNetworkConnected()
{
#if PLATFORM_ANDROID

#elif PLATFORM_IOS
		return KakaotalkAPI::IsNetworkConnected();
#endif
	return true;
}

bool B2P_IsKakaoLogin()
{
#if PLATFORM_ANDROID

#elif PLATFORM_IOS
	return KakaotalkAPI::IsLogin();
#endif
	// 타 플랫폼은 테스트라도 하게 true로 반환
	return true;
}

void B2P_KG_GoogleLogin()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_GoogleClientConnect();
#elif PLATFORM_IOS
	// IOS는 없음
#endif
}

void B2P_KG_GoogleLogOut()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_GoogleClientDisconnect();
#elif PLATFORM_IOS
	// IOS는 없음
#endif
}

void B2P_AchievementUnlock(FString sID)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_GoogleAchievement(sID);
#elif PLATFORM_IOS

#endif
}

void B2P_ShowAchivementView()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_ShowAchievementView();
#elif PLATFORM_IOS

#endif
}

FString B2P_GetKakaoUserID()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetKakaoUserID();
#elif PLATFORM_IOS
	return KakaotalkAPI::GetKakaoUserID();
#endif
	return "12121212";
}

void B2P_ShowKakaoCoupon()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_ShowKakaoCoupon();
#elif PLATFORM_IOS

#endif
}

void B2P_KakaoLoadMessage(FString sBoxID)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_LoadMessage(sBoxID);
#elif PLATFORM_IOS
	int32 RequestMessageCount = 5;
	KakaotalkAPI::LoadMessages(sBoxID, RequestMessageCount);
#endif
}

void B2P_SetClipboardString(FString sUserID)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_SetClipboardString(sUserID);
#elif PLATFORM_IOS
	KakaotalkAPI::SetPasteBoard(sUserID);
#endif
}

FString B2P_GetGameServerAddress()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetGameServerAddress();
#elif PLATFORM_IOS
	return KakaotalkAPI::GetKakaoServerAddress();
#endif
	return "172.168.2.104";
}

FString B2P_GetCDNAddress()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetCDNAddress();
#elif PLATFORM_IOS
	return  KakaotalkAPI::GetKakaoCDNAddress();
#endif
	return "12121212";
}

bool B2P_IsGoogleLogin()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_IsGoogleLogin();
#elif PLATFORM_IOS
	return false;
#endif
	return false;
	
}

void B2P_LevelTarcking(const int32 Level, const FString CharacterType)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_LevelTarcking(Level, CharacterType);
#elif PLATFORM_IOS
	KakaotalkAPI::LevelTracking(Level, CharacterType);
#endif	
}

void B2P_GameExitPopUp(FString sMessage)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_ExitPopup(sMessage);
#elif PLATFORM_IOS

#endif	
}

void B2P_ShowKakaoNoticeView(bool bOneDay, FString sNoticeKey)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_ShowKakaoNoticeView(bOneDay, sNoticeKey);
#elif PLATFORM_IOS
	KakaotalkAPI::ShowNoticeView(bOneDay ? sNoticeKey : TEXT(""));
#endif	

}

void B2P_ShowKakaoEventView(FString sEventKey)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_ShowKakaoEventView(sEventKey);
#elif PLATFORM_IOS
	KakaotalkAPI::ShowEventWinningResultView(sEventKey);
#endif	
}

// kakao promotion
void B2P_ShowKakaoPromotionView()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_ShowKakaoPormotionView();
#elif PLATFORM_IOS
	KakaotalkAPI::ShowStartingPromotionPopups();
#endif	
}

// kakao push set
void B2P_SetKakaoPushEnable(bool bDay, bool bSet)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_SetKakaoPushEnable(bDay, bSet);
#elif PLATFORM_IOS
	if (bDay)
		KakaotalkAPI::SetEnablePushOption(bSet);
	else
		KakaotalkAPI::SetEnablePushOptionNight(bSet);
#endif	
}

// is kakaopush
bool B2P_IsEnableKakaoPush(bool bDay)
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_IsEnableKakaoPush(bDay);
#elif PLATFORM_IOS
	if (bDay)
		return KakaotalkAPI::GetPushOption();
	else
		return KakaotalkAPI::GetPushOptionNight();
#endif	
	return false;
}

void B2P_ShowOpenURL(FString sUrl)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_LaunchURL(sUrl);
#elif PLATFORM_IOS
	KakaotalkAPI::ShowOpenURL(sUrl);
#else
	// 아.. ios도 이걸로 쓸까...
	FPlatformProcess::LaunchURL(*sUrl, nullptr, nullptr);
#endif	
}

FString B2P_GetKakaoFriendInfo(int32 nIndex)
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetKakaoFriendInfo(nIndex);
#elif PLATFORM_IOS
	return KakaotalkAPI::GetKakaoFriendInfo(nIndex);
#endif	
	return "";
}

FString B2P_GetKakaoFriendNickName(int32 nIndex)
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetKakaoFriendNickName(nIndex);
#elif PLATFORM_IOS
	return "";
#endif	
	return "";
}


int32 B2P_GetKakaoFriendCount()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetKakaoFriendCount();
#elif PLATFORM_IOS
	return KakaotalkAPI::GetKakaoFriendCount();
#endif	
	return 0;
}

FString B2P_GetKakaoFriendEventInfo(int32 nIndex)
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetKakaoInvitationEventInfo(nIndex);
#elif PLATFORM_IOS
	return KakaotalkAPI::GetKakaoInvitationEventInfo(nIndex);
#endif	
	return "";
}

int32 B2P_GetKakaoFriendEventCount()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetKakaoInvitationEventCount();
#elif PLATFORM_IOS
	return KakaotalkAPI::GetKakaoInvitationEventCount();
#endif	
	return 0;
}

void B2P_KakaoInviteNewMessage(FString sKakaoUserID, int32 EventID)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_KakaoSendNewInviteMessage(sKakaoUserID,EventID);
#elif PLATFORM_IOS
	KakaotalkAPI::SendNewInviteMessage(sKakaoUserID, EventID);
#endif	
	
}

void B2P_KakaoNewGameMessage(FString sKakaoUserID)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_KakaoSendNewGameMessage(sKakaoUserID);
#elif PLATFORM_IOS
	FString TempleteID = TEXT("7314");	// 따로 받아오는 곳이 없어서 여기서 셋팅.
	KakaotalkAPI::SendNewGameMessage(sKakaoUserID, TempleteID);
#endif	
}

void B2P_SendKakaoActionLog(FString sCategory, FString sAction, FString sLabel, FString sActionAttr1)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_KakaoActionLog(sCategory, sAction, sLabel, sActionAttr1); 
#elif PLATFORM_IOS
	KakaotalkAPI::WriteActionLog(sCategory, sAction, sLabel, sActionAttr1);
#elif PLATFORM_WINDOWS
	UE_LOG(LogB2ActionLog, Log, TEXT("B2P_SendKakaoActionLog sCategory : %s, sAction : %s, sLabel : %s, sActionAttr1 : %s"), *sCategory, *sAction, *sLabel, *sActionAttr1);
#endif	
}


FString B2P_GetKakaoLanguageCode()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetKakaoLanguageCode();
#elif PLATFORM_IOS
	return KakaotalkAPI::GetLanguageCode();
#endif
	return TEXT("");
}

FString B2P_GetKakaoCountryCode()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetKakaoCountryCode();
#elif PLATFORM_IOS
	return KakaotalkAPI::GetCountryCode();
#endif
	return TEXT("");
}
FString B2P_GetKakaoOsName()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetKakaoOsName();
#elif PLATFORM_IOS
	return KakaotalkAPI::GetOSName();
#endif
	return TEXT("");
}
FString B2P_GetKakaoDeviceModel()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetKakaoDeviceModel();
#elif PLATFORM_IOS
	return KakaotalkAPI::GetDeviceModel();
#endif
	return TEXT("");
}

int32 B2P_GetMarKetType()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetMarKetType();
#elif PLATFORM_IOS
	return 11;
#endif
	return 12;
}

void B2P_KeepScreenOn(bool bEnable)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_KeepScreenOn(bEnable);
#elif PLATFORM_IOS
	KakaotalkAPI::KeepScreenOn(bEnable);
#endif	

}

void B2P_ShowCsView()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_ShowCsView();
#elif PLATFORM_IOS
	KakaotalkAPI::ShowCSView();
#endif	
}

void B2P_SetOrientation(bool bSet)
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_SetOrientation(bSet);

#elif PLATFORM_IOS
	if(bSet)
		[[IOSAppDelegate GetDelegate] performSelectorOnMainThread:@selector(RotateOrientationPortrait) withObject:nil waitUntilDone : NO]; 
	else
		[[IOSAppDelegate GetDelegate] performSelectorOnMainThread:@selector(RotateOrientationLandscape) withObject:nil waitUntilDone : NO];
#endif	
}

bool B2P_GetOrientationState()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_GetOrientationState();

#elif PLATFORM_IOS 
	return B2P_GetStatusbarOrientationState();
#endif	
	return false;
}

void B2P_ScreenCapture()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_ScreenCapture();

#elif PLATFORM_IOS
	
#endif	

}

void B2P_ShowKakaoGachaOddsView()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_KakaoShowGachaView();
#elif PLATFORM_IOS
	KakaotalkAPI::ShowGachaOddsView();
#endif	
}

bool B2P_IsInReview()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_IsinReview();
#elif PLATFORM_IOS
	return KakaotalkAPI::IsInReview();
#endif	
	return false;
}

void B2P_RenewKakaoInviteFriend()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_RenewKakaoInviteFriend();
#elif PLATFORM_IOS
	KakaotalkAPI::LoadInvitableFriendProfiles(0, 500);
#endif		
}

void B2p_RenewKakaoFriendEvent()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_RenewKakaoLoadInvitationEvents();
#elif PLATFORM_IOS
	KakaotalkAPI::LoadInvitationEvents();
#endif		
}

bool B2P_IsViewPromotion()
{
#if PLATFORM_ANDROID
	return AndroidThunkCpp_IsViewPromotion();
#elif PLATFORM_IOS
	return KakaotalkAPI::IsViewPromotion();
#endif	
	return false;	
}

FString B2P_GetKakaoCustomData(FString sKey)
{
	FString KeyDebugString = FString(TEXT(""));

#if PLATFORM_ANDROID
	KeyDebugString = AndroidThunkCpp_GetKakaoCustomData(sKey);
#elif PLATFORM_IOS
	// IOS 처리 필요
	KeyDebugString = KakaotalkAPI::GetCustomData(sKey);
#else	
	KeyDebugString = TEXT("https://cs.kakao.com/helps?category=356&device=1081&locale=ko&service=4&articleId=1073191067&page=1");
#endif

	if (KeyDebugString.IsEmpty())
	{
		ErrorOpenWebEmptyURLClass<>::GetInstance().Signal();
	}

#if FORCE_LOGGING_IN_ANDROID
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("@@@@@ YJTestLOG @@@@@ B2P_GetKakaoCustomData @@@@@ KeyString : %s\n"), *KeyDebugString);
#endif

	return KeyDebugString;
}

void B2P_ShowEndingPromotion()
{
#if PLATFORM_ANDROID
	AndroidThunkCpp_ShowEndingPormotion();
#elif PLATFORM_IOS
	// IOS 처리 필요 없음
#endif		
}

#if PLATFORM_IOS
void B2P_UpdateStatusbarOrientationState()
{
	[[IOSAppDelegate GetDelegate] performSelectorOnMainThread:@selector(UpdateStatusbarOrientationState) withObject:nil waitUntilDone : NO];
}

/**
* @return true:가로, false:세로
*/
bool B2P_GetDeviceOrientationState()
{
	return [[IOSAppDelegate GetDelegate] GetDeviceOrientaionState];
}
/** 
* @return true:가로, false:세로
*/
bool B2P_GetStatusbarOrientationState()
{
	return [[IOSAppDelegate GetDelegate] GetStatusbarOrientaionState];
}

void B2P_SetAutoRoateOrientaion(bool bTurnON)
{
	if (bTurnON)
		[[IOSAppDelegate GetDelegate] performSelectorOnMainThread:@selector(SetAutoRotationOn) withObject:nil waitUntilDone : NO];
	else
		[[IOSAppDelegate GetDelegate] performSelectorOnMainThread:@selector(SetAutoRotationOff) withObject:nil waitUntilDone : NO];
}
#endif

