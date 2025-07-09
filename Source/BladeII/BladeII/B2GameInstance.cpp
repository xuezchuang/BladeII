// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "Event.h"
#include "B2NetGameMode.h"
//#include "B2StageGameMode.h"
#include "B2GameInstance.h"
#include "Retailer.h"

#include "B2PlatformAPI.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#elif PLATFORM_IOS
#include "KakaoTalkPlugin.h"
#endif
#include "UnrealEngine.h"

#include "B2NetworkBridge.h"

////#include "B2IAPGenericPlatform.h"
#include "B2NetworkBridge.h"
#include "BladeIIBlockToSyncNetwork.h"

#include "BladeIIUtil.h"
#include "B2PCClassInfoBox.h"
//#include "B2NPCClassInfoBox.h"
//#include "B2CommonSoundInfo.h"
//#include "B2ScenarioDialogInfo.h"
//#include "B2CharacterTagDialogueInfo.h"
#include "B2WingInfo.h"
#include "B2SomeInfo.h"
#include "B2LoadingImageInfo.h"
#include "B2DamageEffectInfo.h"
#include "B2BuffModeEffectInfo.h"
#include "B2SkillInfo.h"
#include "B2GuildMarkInfo.h"
#include "B2InfluenceSkillInfo.h"
//#include "B2GuildNPCInfo.h"
//#include "B2CombatConstantInfo.h"
//#include "B2InGameHUDInfo_Player.h"
//#include "B2DLCFrontGameMode.h"

FDateTime UB2GameInstance::m_LastRequestPlayTime;
UB2GameInstance::FB2Time UB2GameInstance::m_TimeToServer;
UB2GameInstance::InGamePlayTime UB2GameInstance::GamePlayTimeInfo;

void UB2GameInstance::FB2Time::SetUTCNow(const int64& InTimeMillionSec)
{
	CachingServerTime = FDateTime::FromUnixTimestamp(InTimeMillionSec / 1000);
	CachingClientTime = FDateTime::UtcNow();

	if (!RecieveServerTime)
	{
		RecieveServerTime = true;
		UB2GameInstance::RequestUpdateDailyPlayTime();
	}
}

// 서버 시간을 매틱 받는게 아니라 약4초 주기로 받기 때문에 그 사이의 시간은 클라에서 채워준다.
FDateTime UB2GameInstance::FB2Time::GetUTCNow()
{
	if (!RecieveServerTime)
	{
		return FDateTime::UtcNow();
	}

	return CachingServerTime + (FDateTime::UtcNow() - CachingClientTime);
}

FDateTime UB2GameInstance::FB2Time::GetLocalNow()
{
	return GetUTCNow() + m_TimeZone;
}


FDateTime UB2GameInstance::FB2Time::GetKoreaTimeNow()
{
	return GetUTCNow() + FTimespan(9 * ETimespan::TicksPerHour);
}

void UB2GameInstance::FB2Time::CalcTimezoneOffset()
{
	const FDateTime LocalNow = FDateTime::Now();
	const FDateTime UTCNow = FDateTime::UtcNow();

	m_TimeZone = LocalNow - UTCNow;
}

FDateTime  UB2GameInstance::FB2Time::GetUtcToLocalTime(FDateTime InDateTime)
{
	return InDateTime + m_TimeZone;
}

int32 UB2GameInstance::FB2Time::ConvertToSvrDayOfWeek()
{
	FDateTime DateNow = GetKoreaTimeNow();//GetLocalNow();  //[TO-DO][@AKI 180603]일단 한국 빌드만 생각해서 KoreaTime으로 변경. 추후 글로벌 서비스때는 UTC로 변경해야됨.
	return ConvertToDayOfWeek(DateNow);
}

int32 UB2GameInstance::FB2Time::ConvertToDayOfWeek(FDateTime InDateTime)
{
	int32 nLocalDayofweek = static_cast<int32>(InDateTime.GetDayOfWeek());
	int32 nConvertDayOfWeek = nLocalDayofweek + 1; // Monday to Sunday
	int32 nConvertBaseInt = nConvertDayOfWeek % 7 + 1; // 서버는 1로 시작. Engine Enum은 0

	return nConvertBaseInt;
}

//Copy InternationalizationSettingsModel.CPP
int32 UB2GameInstance::FB2Time::GetTimeZone(ETimeZone InTimeZone)
{
	switch (InTimeZone)
	{
	case ETimeZone::InternationalDateLineWest:
		return -1200;
	case ETimeZone::CoordinatedUniversalTimeNeg11:
	case ETimeZone::Samoa:
		return -1100;
	case ETimeZone::Hawaii:
		return -1000;
	case ETimeZone::Alaska:
		return -900;
	case ETimeZone::PacificTime_USCAN:
	case ETimeZone::BajaCalifornia:
		return -800;
	case ETimeZone::MountainTime_USCAN:
	case ETimeZone::Chihuahua_LaPaz_Mazatlan:
	case ETimeZone::Arizona:
		return -700;
	case ETimeZone::Saskatchewan:
	case ETimeZone::CentralAmerica:
	case ETimeZone::CentralTime_USCAN:
	case ETimeZone::Guadalajara_MexicoCity_Monterrey:
		return -600;
	case ETimeZone::EasternTime_USCAN:
	case ETimeZone::Bogota_Lima_Quito:
	case ETimeZone::Indiana_US:
		return -500;
	case ETimeZone::Caracas:
		return -430;
	case ETimeZone::AtlanticTime_Canada:
	case ETimeZone::Cuiaba:
	case ETimeZone::Santiago:
	case ETimeZone::Georgetown_LaPaz_Manaus_SanJuan:
	case ETimeZone::Asuncion:
		return -400;
	case ETimeZone::Newfoundland:
		return -330;
	case ETimeZone::Brasilia:
	case ETimeZone::Greenland:
	case ETimeZone::Montevideo:
	case ETimeZone::Cayenne_Fortaleza:
	case ETimeZone::BuenosAires:
		return -300;
	case ETimeZone::MidAtlantic:
	case ETimeZone::CoordinatedUniversalTimeNeg02:
		return -200;
	case ETimeZone::Azores:
	case ETimeZone::CaboVerdeIs:
		return -100;
	case ETimeZone::Dublin_Edinburgh_Lisbon_London:
	case ETimeZone::Monrovia_Reykjavik:
	case ETimeZone::Casablanca:
	case ETimeZone::UTC:
		return 0;
	case ETimeZone::Belgrade_Bratislava_Budapest_Ljubljana_Prague:
	case ETimeZone::Sarajevo_Skopje_Warsaw_Zagreb:
	case ETimeZone::Brussels_Copenhagen_Madrid_Paris:
	case ETimeZone::WestCentralAfrica:
	case ETimeZone::Amsterdam_Berlin_Bern_Rome_Stockholm_Vienna:
	case ETimeZone::Windhoek:
		return 100;
	case ETimeZone::Minsk:
	case ETimeZone::Cairo:
	case ETimeZone::Helsinki_Kyiv_Riga_Sofia_Tallinn_Vilnius:
	case ETimeZone::Athens_Bucharest:
	case ETimeZone::Jerusalem:
	case ETimeZone::Amman:
	case ETimeZone::Beirut:
	case ETimeZone::Harare_Pretoria:
	case ETimeZone::Damascus:
	case ETimeZone::Istanbul:
		return 200;
	case ETimeZone::Kuwait_Riyadh:
	case ETimeZone::Baghdad:
	case ETimeZone::Nairobi:
	case ETimeZone::Kaliningrad:
		return 300;
	case ETimeZone::Tehran:
		return 330;
	case ETimeZone::Moscow_StPetersburg_Volgograd:
	case ETimeZone::AbuDhabi_Muscat:
	case ETimeZone::Baku:
	case ETimeZone::Yerevan:
	case ETimeZone::Tbilisi:
	case ETimeZone::PortLouis:
		return 400;
	case ETimeZone::Kabul:
		return 430;
	case ETimeZone::Tashkent:
	case ETimeZone::Islamabad_Karachi:
		return 500;
	case ETimeZone::Chennai_Kolkata_Mumbai_NewDelhi:
	case ETimeZone::SriJayawardenepura:
		return 530;
	case ETimeZone::Kathmandu:
		return 545;
	case ETimeZone::Ekaterinburg:
	case ETimeZone::Astana:
	case ETimeZone::Dhaka:
		return 600;
	case ETimeZone::Yangon_Rangoon:
		return 630;
	case ETimeZone::Novosibirsk:
	case ETimeZone::Bangkok_Hanoi_Jakarta:
		return 700;
	case ETimeZone::Krasnoyarsk:
	case ETimeZone::Beijing_Chongqing_HongKong_Urumqi:
	case ETimeZone::KualaLumpur_Singapore:
	case ETimeZone::Taipei:
	case ETimeZone::Perth:
	case ETimeZone::Ulaanbaatar:
		return 800;
	case ETimeZone::Irkutsk:
	case ETimeZone::Seoul:
	case ETimeZone::Osaka_Sapporo_Tokyo:
		return 900;
	case ETimeZone::Darwin:
	case ETimeZone::Adelaide:
		return 930;
	case ETimeZone::Yakutsk:
	case ETimeZone::Canberra_Melbourne_Sydney:
	case ETimeZone::Brisbane:
	case ETimeZone::Hobart:
	case ETimeZone::Guam_PortMoresby:
		return 1000;
	case ETimeZone::Vladivostok:
	case ETimeZone::SolomonIs_NewCaledonia:
		return 1100;
	case ETimeZone::Magadan:
	case ETimeZone::Fiji:
	case ETimeZone::Auckland_Wellington:
	case ETimeZone::CoordinatedUniversalTime12:
		return 1200;
	case ETimeZone::Nukualofa:
		return 1300;
	case ETimeZone::LocalTime:
	{
		// This is definitely a hack, but our FPlatformTime doesn't do timezones
		const FDateTime LocalNow = FDateTime::Now();
		const FDateTime UTCNow = FDateTime::UtcNow();

		const FTimespan Difference = LocalNow - UTCNow;

		const int32 MinutesDifference = FMath::RoundToInt(Difference.GetTotalMinutes());

		const int32 Hours = MinutesDifference / 60;
		const int32 Minutes = MinutesDifference % 60;

		const int32 Timezone = (Hours * 100) + Minutes;
		return Timezone;
	}

	default:
		return 0;
	}
}

UB2GameInstance::UB2GameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	PushManagerClass(nullptr)
{
	UE_LOG(LogBladeII, Log, TEXT("UB2GameInstance::UB2GameInstance()"));

	//CachedMobClassInfoBox = NULL;
	//CachedPCClassInfoBox = NULL;
	//CachedCommonSoundInfo = NULL;
	//CachedDialogCharacterInfoTable = NULL;
	//CachedWingInfo = NULL;
	//CachedSomeInfo = NULL;
	//CachedItemMiscInfo = NULL;
	//CachedLoadingImageInfo = NULL;
	//CachedNoAnimSkillInfo = NULL;
	//CachedDamageEffectInfo = NULL;
	//CachedBuffModeEffectInfo = NULL;
	//CachedSkillAnimInfo = NULL;
	//CachedGuildMarkInfo = NULL;
	//CachedInfluenceSkillInfo = NULL;
	//CachedGuildNPCInfo = NULL;
	//CachedGuildSkillInfo = NULL;
	//CachedCombatConstantInfo = NULL;
}

void UB2GameInstance::Init()
{
	Super::Init();

	UE_LOG(LogBladeII, Log, TEXT("UB2GameInstance::Init()"));
//
//	// 틱생성
//	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UB2GameInstance::Tick));
//	
//	if (PushManagerClass == nullptr)
//		PushManagerClass = new LocalPushManager();
//
//#if !PLATFORM_MAC
//	// 인앱 초기화
//	B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->SubscribeEvents();
//#endif
//
//#if PLATFORM_ANDROID // 로컬푸쉬용~
//	FJavaWrapper::OnPlatformResumeDelegate.Remove(OnPlatformResumeHandler);
//	OnPlatformResumeHandler = FJavaWrapper::OnPlatformResumeDelegate.AddUObject(this, &UB2GameInstance::PlatformResumeDelegate);
//
//	FJavaWrapper::OnPlatformPauseDelegate.Remove(OnKPlatformPauseHandler);
//	OnKPlatformPauseHandler = FJavaWrapper::OnPlatformPauseDelegate.AddUObject(this, &UB2GameInstance::PlatformPauseDelegate);
//
//	FJavaWrapper::OnnativeKakaoMessageBoxCompletedDelegate.Remove(OnMessageBoxCompletedHandler);
//	OnMessageBoxCompletedHandler = FJavaWrapper::OnnativeKakaoMessageBoxCompletedDelegate.AddUObject(this, &UB2GameInstance::MessageBoxCompletedDelegate);
//#elif PLATFORM_IOS
//	FKakaoTalkPlugin::OnResumeDelegate.Remove(OnPlatformResumeHandler);
//	OnPlatformResumeHandler = FKakaoTalkPlugin::OnResumeDelegate.AddUObject(this, &UB2GameInstance::PlatformResumeDelegate);
//
//	FCoreDelegates::ApplicationWillDeactivateDelegate.AddUObject(this, &UB2GameInstance::PlatformPauseDelegate);
//
//	FKakaoTalkPlugin::OnPauseDelegate.Remove(OnKPlatformPauseHandler);
//	OnKPlatformPauseHandler = FKakaoTalkPlugin::OnPauseDelegate.AddUObject(this, &UB2GameInstance::PlatformPauseDelegate);
//
//	FKakaoTalkPlugin::OnMessagestDelegate.Remove(OnMessageBoxCompletedHandler);
//	OnMessageBoxCompletedHandler = FKakaoTalkPlugin::OnMessagestDelegate.AddUObject(this, &UB2GameInstance::MessageBoxCompletedDelegate);
//#endif
//
//	m_MessageBoxCalled.AtomicSet(false);
//	m_bSendOK = true;

	m_TimeToServer.CalcTimezoneOffset();
}

void UB2GameInstance::Shutdown()
{
	Super::Shutdown();
//
//	// 시간 누적x
//	EnableSavePlayTime(false);
//
//#if !PLATFORM_MAC
//	// 인앱 해제
//	B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->UnsubscribeEvents();
//#endif
//
//	// Unregister ticker delegate
//	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
//	if (PushManagerClass)
//	{
//		delete PushManagerClass;
//		PushManagerClass = nullptr;
//	}
}

bool UB2GameInstance::Tick(float DeltaTime)
{
	if (m_MessageBoxCalled)
	{
		m_MessageBoxCalled.AtomicSet(false);
		GetWorld()->GetTimerManager().ClearTimer(SendMessageInboxTimer);
		GetWorld()->GetTimerManager().SetTimer(SendMessageInboxTimer, this, &UB2GameInstance::OnSendMessageInboxTimerCB, 60.0f, false);
		m_bSendOK = false;

		UE_LOG(LogLoad, Log, TEXT("Platform!! RequestKakaoMessageProcessing!!!!!!!!!!!!!!!! ~~~~~~~"));
		data_trader::Retailer::GetInstance().RequestKakaoMessageProcessing("inbox");
	}
	return true;
}


bool UB2GameInstance::IncludeTimeNow(const int64 InStartTime, const int64 InEndTime)
{
	//FDateTime DateNow = GetLocalNow();
	//FDateTime StartTime = GetUtcToLocalTime(GetServerTimeToDateTime(InStartTime));
	//FDateTime EndTime = GetUtcToLocalTime(GetServerTimeToDateTime(InEndTime));

	//return DateNow - StartTime > 0 && EndTime - DateNow > 0;
	return true;
}

UB2GameInstance::InGamePlayTime::InGamePlayTime()
{
	GamePlayTime = 0.0f;
	GamePlayTimeAfterRequest = FDateTime();
}

void UB2GameInstance::SetPlayTimeInfo(float InTime)
{
	//GamePlayTimeInfo.GamePlayTime = InTime;
	//GamePlayTimeInfo.GamePlayTimeAfterRequest = GetUTCNow();

	//if (m_TimeToServer.GetRequestUpdateDailyPlayStep() == ERequestUpdateDailyPlayStep::RequestUpdateDailyTime)
	//{
	//	FirstRequestUpdateDailyPlayTimeClass<>::GetInstance().Signal();
	//	m_TimeToServer.SetRequestUpdateDailyPlayStep(ERequestUpdateDailyPlayStep::ResponseUpdateDailyTime);
	//}
}

float UB2GameInstance::GetPlayTime()
{
	if ((GetUTCNow() - GamePlayTimeInfo.GamePlayTimeAfterRequest).GetTotalSeconds() < 0)
	{
		GamePlayTimeInfo.GamePlayTimeAfterRequest = GetUTCNow();
	}
	return GamePlayTimeInfo.GamePlayTime + (GetUTCNow() - GamePlayTimeInfo.GamePlayTimeAfterRequest).GetTotalSeconds();
}

void UB2GameInstance::EnableSavePlayTime(bool InEnable)
{
	auto* GameInstanceWorld = GetWorld();

	if (GameInstanceWorld)
	{
		GameInstanceWorld->GetTimerManager().ClearTimer(SavePlayTimeHandler);

		if (InEnable)
		{
			m_LastRequestPlayTime = GetUTCNow();
			GameInstanceWorld->GetTimerManager().SetTimer(SavePlayTimeHandler, this, &UB2GameInstance::SendPlayTimeToServer, 180.f, true);
		}
	}
}

void UB2GameInstance::SendPlayTimeToServer()
{
	RequestUpdateDailyPlayTime(false);
}

/*
   m_LastRequestPlayTime : Request가 자주 가지않게 Interval을 체크하는 용도
   RequestInterval       : Interval 간격
   GamePlayTimeInfo.GamePlayTimeAfterRequest : 지난번에 Request를 보낸 시각
*/
void UB2GameInstance::RequestUpdateDailyPlayTime(bool IsForce)
{
	CONSTEXPR auto RequestInterval = 300.0;
	const auto UTCNow = GetUTCNow();
	const auto CurrentRequestInterval = (UTCNow - m_LastRequestPlayTime).GetTotalSeconds();

	if (m_TimeToServer.IsInitializeServerInfo() // 네트워크 시간동기화를 한 상태인가? 
		&& (IsForce || CurrentRequestInterval > RequestInterval || m_TimeToServer.GetRequestUpdateDailyPlayStep() == ERequestUpdateDailyPlayStep::NotRequest)) // 강제가 아니라면 Interval이 지났거나 한번도 없데이트 한적 없는가
	{
		// 현재시간 - 최근 Request를 보낸 시간 = PendingTime
		int64 PendingTime = (UTCNow - GamePlayTimeInfo.GamePlayTimeAfterRequest).GetTicks() / 10000000;
		m_LastRequestPlayTime = UTCNow; // Interval 초기화

		// 음수거나, 10분보다 클 때 잘못된 값으로 간주하고 초기화
		if (PendingTime < 0 || PendingTime > 600)
		{
			GamePlayTimeInfo.GamePlayTimeAfterRequest = UTCNow;
			return;
		}
		else
		{
			// 가장 첫번째 업데이트 요청의 경우 PendingTime 이 0초일 경우도 있다.
			// 이 경우 1 이상 값을 보내기 위해 보정을 거치는데 보정된 PendingTime 을 담아 보내도 서버에서는 이 값을 사용하는게 아니라 직접 계산한 시간을 사용하기에 문제 없다.
			if (m_TimeToServer.GetRequestUpdateDailyPlayStep() == ERequestUpdateDailyPlayStep::NotRequest)
			{
				m_TimeToServer.SetRequestUpdateDailyPlayStep(ERequestUpdateDailyPlayStep::RequestUpdateDailyTime);
				PendingTime = FMath::Max<int64>(1, PendingTime);
			}

			// 1 >= 인 값을 보내달라고해서.
			if (PendingTime >= 1)
			{
				data_trader::Retailer::GetInstance().Retailer::RequestUpdateDailyPlayTime(static_cast<int32>(PendingTime));
			}
		}
	}
}

#if PLATFORM_ANDROID
void UB2GameInstance::PlatformResumeDelegate(JNIEnv *env, jobject thiz)
#else
void UB2GameInstance::PlatformResumeDelegate()
#endif
{
	UE_LOG(LogLoad, Log, TEXT("Platform!! PlatformResumeDelegate ~~~~~~~"));

	//// TODO:: 여기다가 ~~~
	//if (PushManagerClass != nullptr)
	//{
	//	PushManagerClass->CancelLocalPushSchedule_RestPoint();
	//}

	//m_ResumeTime = FDateTime::UtcNow();
	//ResumeDelegate.Broadcast();
	//m_TimeToServer.CalcTimezoneOffset();

	//// PlayTime에서 Pause 된 시간을 빼줌
	//FTimespan RemainTime = m_ResumeTime - m_PauseTime;
	//GamePlayTimeInfo.GamePlayTimeAfterRequest += RemainTime;

	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (B2GM == nullptr) //ABladeIIGameMode가 nullptr인 경우는 B2DLCFrontGameMode밖에 없어야 함
	//	return;

	////[@AKI, 180806] NetGameMode일 경우 Pause시 일시정지 화면 안나오게 변경
	//if (B2GM->IsNetGameMode() == false)
	//{
	//	// 네트워크 게임모드가 아닌 스탠드얼론 게임모드들 (로비 / 모험모드 / 영웅의탑 / 카운터던전 등)
	//	FFunctionGraphTask::CreateAndDispatchWhenReady([ B2GM ]()
	//	{
	//		// [모험모드] 중 "반복전투" 및 "연속전투" 일 경우 시스템(FJavaWrapper::OnPlatformResumeDelegate) 을 통해 들어온 멈춤메뉴의 경우 처리 하지 않는다
	//		if ( AB2StageGameMode* StageGameMode = Cast< AB2StageGameMode >( B2GM ) )
	//		{
	//			if ( StageGameMode->IsRepeatBattleOn() )
	//			{
	//				return;
	//			}
	//		}

	//		PlatformPausePressClass<>::GetInstance().Signal();

	//	}, TStatId(), NULL, ENamedThreads::GameThread);
	//}
	//else
	//{
	//	//PvP, Raid, Control 일때는 채널이 끊겼다는 팝업을 띄워 줌
	//	FBladeIIBlockToSyncNetwork::GetInstance().ChannelDisconnect(-1); 	//[@AKI, 180727] [B2CLT-3743] 일대일 모드 어뷰징 개선을 위해 클라에서 강제로 채널 서버 끊는 부분 제거
	//}
}

#if PLATFORM_ANDROID
void UB2GameInstance::PlatformPauseDelegate(JNIEnv *env, jobject thiz)
#else
void UB2GameInstance::PlatformPauseDelegate()
#endif
{
	UE_LOG(LogLoad, Log, TEXT("Platform!! PlatformPauseDelegate ~~~~~~~"));

	//// TODO:: 여기다가 ~~~
	//if (PushManagerClass != nullptr)
	//{
	//	PushManagerClass->SetLocalPushSchedule_RestPoint();
	//}
	//
	//m_PauseTime = FDateTime::UtcNow();

	////[@AKI, 180727] [B2CLT-3743] 일대일 모드 어뷰징 개선을 위해 클라에서 강제로 채널 서버 끊는 부분 제거
	////[@AKI, 180806] TD님과 논의 후 다시 살림
	//if (GetWorld() == nullptr || UGameplayStatics::GetGameMode(GetWorld()) == nullptr)
	//	return;

	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (B2GM == nullptr) //ABladeIIGameMode가 nullptr인 경우는 B2DLCFrontGameMode밖에 없어야 함
	//	return;

	//B2GM->OnPlatformPause();

	//if (B2GM->IsNetGameMode() == false)
	//{
	//	//Lobby, Stage, HeroTower, CounterAttack, TM, Guild, Tutorial 등등
	//}
	//else
	//{
	//	//PvP, Raid, Control 일때는 채널 소캣 강제로 Close 시킴
	//	FB2NetworkBridge::GetInstance()->CloseChannel("ByPause");
	//}
}


#if PLATFORM_ANDROID
void UB2GameInstance::MessageBoxCompletedDelegate(JNIEnv *env, jobject thiz)
{
	UE_LOG(LogLoad, Log, TEXT("Platform!! PlatformPauseDelegate ~~~~~~~"));

	m_MessageBoxCalled.AtomicSet(true);
}

#else
void UB2GameInstance::MessageBoxCompletedDelegate(bool bSuccess)
{
	UE_LOG(LogLoad, Log, TEXT("@@@@@@@@ Platform!! MessageBoxCompletedDelegate ~~~~~~~ %d"), bSuccess);
	if (bSuccess)
		m_MessageBoxCalled.AtomicSet(true);
}
#endif

int64 UB2GameInstance::GetPauseResumeIntervalTime()
{
	FTimespan RemainTime = m_PauseTime - m_ResumeTime;
	return RemainTime.GetTotalSeconds();
}

void UB2GameInstance::SendKakaoMessgaeInboxProcessing()
{
	if (m_bSendOK)
	{
		GetWorld()->GetTimerManager().ClearTimer(SendMessageInboxTimer);
		GetWorld()->GetTimerManager().SetTimer(SendMessageInboxTimer, this, &UB2GameInstance::OnSendMessageInboxTimerCB, 60.0f, false);
		m_bSendOK = false;
		B2P_KakaoLoadMessage(TEXT("inbox"));
		UE_LOG(LogLoad, Log, TEXT("Platform!! SendKakaoMessgaeInboxProcessing ~~~~~~~"));
	}
}

void UB2GameInstance::OnSendMessageInboxTimerCB()
{
	m_bSendOK = true;
}

/////////////////////////////////////////////////////////////
// Blueprint 기반 InfoAsset 들 게임모드 캐싱 버전
// StaticFind~~ 씨리즈보다 GetGameMode 한 담에 이렇게 얻어오는 게 훨 빠름
//
UB2PCClassInfoBox* UB2GameInstance::GetPCClassInfoBox()
{
	if (!CachedPCClassInfoBox)
	{
		CachedPCClassInfoBox = StaticFindPCClassInfoBox(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	}
	BII_CHECK(CachedPCClassInfoBox);
	return CachedPCClassInfoBox;
}

UB2InGameHUDInfo_Player* UB2GameInstance::GetPlayerHUDInfo(EPCClass InCharClass)
{
	if (GetPCClassInfoBox())
	{
		UB2PCClassInfo* SinglePCClassInfo = CachedPCClassInfoBox->GetSingleClassInfo(InCharClass);
		if (SinglePCClassInfo && SinglePCClassInfo->HUDInfoClassOverride)
		{
			return SinglePCClassInfo->HUDInfoClassOverride->GetDefaultObject<UB2InGameHUDInfo_Player>();
		}
	}
	return NULL;
}

UB2NPCClassInfoBox* UB2GameInstance::GetMobClassInfoBox()
{
	//if (!CachedMobClassInfoBox)
	//{
	//	CachedMobClassInfoBox = StaticFindMobClassInfoBox(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	//}
	//BII_CHECK(CachedMobClassInfoBox);
	//return CachedMobClassInfoBox;
	return NULL;
}

UB2CommonSoundInfo* UB2GameInstance::GetCommonSoundInfo()
{
	if (!CachedCommonSoundInfo)
	{
		CachedCommonSoundInfo = StaticFindCommonSoundInfo(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	}
	//BII_CHECK(CachedCommonSoundInfo || 
	//	// DLCFront Apk 에서 UI 버튼 사운드 때문에 필요로 할 때가 있음. 아직 DLC 다운로드 받지 않은 상황에서는 로딩을 못하니 어쩔 수 없지.
	//	(WorldContext && Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(WorldContext->World())))
	//);
	return CachedCommonSoundInfo;
}

UB2DialogCharacterInfoSet* UB2GameInstance::GetDialogCharacterInfoTable()
{
	if (!CachedDialogCharacterInfoTable)
	{
		CachedDialogCharacterInfoTable = StaticFindDialogCharacterInfoTable(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	}
	BII_CHECK(CachedDialogCharacterInfoTable);
	return CachedDialogCharacterInfoTable;
}

UB2WingInfo* UB2GameInstance::GetWingInfo()
{
	if (!CachedWingInfo)
	{
		CachedWingInfo = StaticFindWingInfo(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	}
	BII_CHECK(CachedWingInfo);
	return CachedWingInfo;
}

UB2SomeInfo* UB2GameInstance::GetSomeInfo()
{
	if (!CachedSomeInfo)
	{
		CachedSomeInfo = StaticFindSomeInfo(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	}
	BII_CHECK(CachedSomeInfo);
	return CachedSomeInfo;
}

UB2ItemMiscInfo* UB2GameInstance::GetItemMiscInfo()
{
	if (!CachedItemMiscInfo)
	{
		CachedItemMiscInfo = StaticFindItemMiscInfo(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	}
	BII_CHECK(CachedItemMiscInfo);
	return CachedItemMiscInfo;
}

UB2LoadingImageInfo* UB2GameInstance::GetLoadingImageInfo()
{
	if (!CachedLoadingImageInfo)
	{
		CachedLoadingImageInfo = StaticFindLoadingImageInfo(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	}
	BII_CHECK(CachedLoadingImageInfo);
	return CachedLoadingImageInfo;
}

class UB2NoAnimSkillInfo* UB2GameInstance::GetNoAnimSkillInfo()
{
	if (!CachedNoAnimSkillInfo)
	{
		CachedNoAnimSkillInfo = StaticFindNoAnimSkillInfo(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	}
	BII_CHECK(CachedNoAnimSkillInfo);
	return CachedNoAnimSkillInfo;
}

UB2DamageEffectInfo* UB2GameInstance::GetDamageEffectInfo()
{
	if (!CachedDamageEffectInfo)
	{
		CachedDamageEffectInfo = StaticFindDamageEffectInfo(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	}
	BII_CHECK(CachedDamageEffectInfo);
	return CachedDamageEffectInfo;
}

UB2BuffModeEffectInfo* UB2GameInstance::GetBuffModeEffectInfo()
{
	if (!CachedBuffModeEffectInfo)
	{
		CachedBuffModeEffectInfo = StaticFindBuffModeEffectInfo(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	}
	BII_CHECK(CachedBuffModeEffectInfo);
	return CachedBuffModeEffectInfo;
}

UB2SkillAnimInfo* UB2GameInstance::GetSkillAnimInfo()
{
	if (!CachedSkillAnimInfo)
	{
		CachedSkillAnimInfo = StaticFindSkillAnimInfo(nullptr); // 여기서 this 를 넘겨주면 무한 호출 루프
	}
	BII_CHECK(CachedSkillAnimInfo);
	return CachedSkillAnimInfo;
}
UB2GuildMarkInfo* UB2GameInstance::GetGuildMarkInfo()
{
	if (!CachedGuildMarkInfo)
	{
		CachedGuildMarkInfo = StaticFindGuildMarkInfo(nullptr);  // 여기서 this 를 넘겨주면 무한 호출 루프 
	}
	BII_CHECK(CachedGuildMarkInfo);
	return CachedGuildMarkInfo;
}
UB2InfluenceSkillInfo* UB2GameInstance::GetInfluenceSkillInfo()
{
	if (!CachedInfluenceSkillInfo)
	{
		CachedInfluenceSkillInfo = StaticFindInfluenceSkillInfo(nullptr);  // 여기서 this 를 넘겨주면 무한 호출 루프 
	}
	BII_CHECK(CachedInfluenceSkillInfo);
	return CachedInfluenceSkillInfo;
}
//UB2GuildNPCInfo* UB2GameInstance::GetGuildNPCInfo()
//{
//	if (!CachedGuildNPCInfo)
//	{
//		CachedGuildNPCInfo = StaticFindGuildNPCInfo(nullptr);  // 여기서 this 를 넘겨주면 무한 호출 루프 
//	}
//	BII_CHECK(CachedGuildNPCInfo);
//	return CachedGuildNPCInfo;
//}
UB2GuildSkillInfo* UB2GameInstance::GetGuildSkillInfo()
{
	if (!CachedGuildSkillInfo)
	{
		CachedGuildSkillInfo = StaticFindGuildSkillInfo(nullptr);  // 여기서 this 를 넘겨주면 무한 호출 루프 
	}
	BII_CHECK(CachedGuildSkillInfo);
	return CachedGuildSkillInfo;
}

UB2CombatConstantInfo * UB2GameInstance::GetCombatConstantInfo()
{
	if (!CachedCombatConstantInfo)
	{
		CachedCombatConstantInfo = StaticFindCombatConstantInfo(nullptr);  // 여기서 this 를 넘겨주면 무한 호출 루프 
	}
	BII_CHECK(CachedCombatConstantInfo);
	return CachedCombatConstantInfo;
}
