#pragma once
#include "B2GameEnum.h"
#include "EngineMinimal.h"


DECLARE_LOG_CATEGORY_EXTERN(LogB2ActionLog, Log, All);

bool B2P_CheckPermission();

void B2P_InitPlatformPermission();

void B2P_GameExit();

FString B2P_GetExternalFilePath();
bool B2P_SupportTypeASTC();

// kakaoGame
void B2P_KakaoLogin();
void B2P_KakaoLogout();
void B2P_KakaoUnregister();
void B2P_KakaoAccountLink();
void B2P_KakaoCommunityView();

bool B2P_IsNetworkConnected();

bool B2P_IsKakaoLogin();

void B2P_KG_GoogleLogin();
void B2P_KG_GoogleLogOut();
void B2P_AchievementUnlock(FString sID);
void B2P_ShowAchivementView();
FString B2P_GetKakaoUserID();
void B2P_ShowKakaoCoupon();
void B2P_KakaoLoadMessage(FString sBoxID);

EPlatformIDPCode B2P_GetKakaoIDPCode();
void B2P_SetClipboardString(FString sUserID);


FString B2P_GetGameServerAddress();
FString B2P_GetCDNAddress();
bool B2P_IsGoogleLogin();

void B2P_LevelTarcking(const int32 Level, const FString CharacterType);
void B2P_GameExitPopUp(FString sMessage);

void B2P_ShowKakaoNoticeView(bool bOneDay, FString sNoticeKey);
void B2P_ShowKakaoEventView(FString sEventKey);

void B2P_ShowKakaoPromotionView();
void B2P_SetKakaoPushEnable(bool bDay, bool bSet);
bool B2P_IsEnableKakaoPush(bool bDay);

void B2P_ShowOpenURL(FString sUrl);

FString B2P_GetKakaoFriendInfo(int32 nIndex);
FString B2P_GetKakaoFriendNickName(int32 nIndex);

int32 B2P_GetKakaoFriendCount();

FString B2P_GetKakaoFriendEventInfo(int32 nIndex);
int32 B2P_GetKakaoFriendEventCount();

void B2P_KakaoNewGameMessage(FString sKakaoUserID);

void B2P_KakaoInviteNewMessage(FString sKakaoUserID, int32 EventID = 0);
void B2P_SendKakaoActionLog(FString sCategory, FString sAction, FString sLabel= FString(), FString sActionAttr1 = FString());

FString B2P_GetKakaoLanguageCode();
FString B2P_GetKakaoCountryCode();
FString B2P_GetKakaoOsName();
FString B2P_GetKakaoDeviceModel();
int32 B2P_GetMarKetType();
void B2P_KeepScreenOn(bool bEnable);
void B2P_ShowCsView();

void B2P_SetOrientation(bool bSet);
bool B2P_GetOrientationState();
void B2P_ScreenCapture();

void B2P_ShowKakaoGachaOddsView();

bool B2P_IsInReview();
void B2P_RenewKakaoInviteFriend();
void B2p_RenewKakaoFriendEvent();

bool B2P_IsViewPromotion();
FString B2P_GetKakaoCustomData(FString sKey);
void B2P_ShowEndingPromotion();

#if PLATFORM_IOS
void B2P_UpdateStatusbarOrientationState();
bool B2P_GetDeviceOrientationState();
bool B2P_GetStatusbarOrientationState();
void B2P_SetAutoRoateOrientaion(bool bTurnON);
#endif
////////////////////////////////////////////////////////////////////////////////
