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

// ���� �ð��� ��ƽ �޴°� �ƴ϶� ��4�� �ֱ�� �ޱ� ������ �� ������ �ð��� Ŭ�󿡼� ä���ش�.
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
	FDateTime DateNow = GetKoreaTimeNow();//GetLocalNow();  //[TO-DO][@AKI 180603]�ϴ� �ѱ� ���常 �����ؼ� KoreaTime���� ����. ���� �۷ι� ���񽺶��� UTC�� �����ؾߵ�.
	return ConvertToDayOfWeek(DateNow);
}

int32 UB2GameInstance::FB2Time::ConvertToDayOfWeek(FDateTime InDateTime)
{
	int32 nLocalDayofweek = static_cast<int32>(InDateTime.GetDayOfWeek());
	int32 nConvertDayOfWeek = nLocalDayofweek + 1; // Monday to Sunday
	int32 nConvertBaseInt = nConvertDayOfWeek % 7 + 1; // ������ 1�� ����. Engine Enum�� 0

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
//	// ƽ����
//	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UB2GameInstance::Tick));
//	
//	if (PushManagerClass == nullptr)
//		PushManagerClass = new LocalPushManager();
//
//#if !PLATFORM_MAC
//	// �ξ� �ʱ�ȭ
//	B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->SubscribeEvents();
//#endif
//
//#if PLATFORM_ANDROID // ����Ǫ����~
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
//	// �ð� ����x
//	EnableSavePlayTime(false);
//
//#if !PLATFORM_MAC
//	// �ξ� ����
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
   m_LastRequestPlayTime : Request�� ���� �����ʰ� Interval�� üũ�ϴ� �뵵
   RequestInterval       : Interval ����
   GamePlayTimeInfo.GamePlayTimeAfterRequest : �������� Request�� ���� �ð�
*/
void UB2GameInstance::RequestUpdateDailyPlayTime(bool IsForce)
{
	CONSTEXPR auto RequestInterval = 300.0;
	const auto UTCNow = GetUTCNow();
	const auto CurrentRequestInterval = (UTCNow - m_LastRequestPlayTime).GetTotalSeconds();

	if (m_TimeToServer.IsInitializeServerInfo() // ��Ʈ��ũ �ð�����ȭ�� �� �����ΰ�? 
		&& (IsForce || CurrentRequestInterval > RequestInterval || m_TimeToServer.GetRequestUpdateDailyPlayStep() == ERequestUpdateDailyPlayStep::NotRequest)) // ������ �ƴ϶�� Interval�� �����ų� �ѹ��� ������Ʈ ���� ���°�
	{
		// ����ð� - �ֱ� Request�� ���� �ð� = PendingTime
		int64 PendingTime = (UTCNow - GamePlayTimeInfo.GamePlayTimeAfterRequest).GetTicks() / 10000000;
		m_LastRequestPlayTime = UTCNow; // Interval �ʱ�ȭ

		// �����ų�, 10�к��� Ŭ �� �߸��� ������ �����ϰ� �ʱ�ȭ
		if (PendingTime < 0 || PendingTime > 600)
		{
			GamePlayTimeInfo.GamePlayTimeAfterRequest = UTCNow;
			return;
		}
		else
		{
			// ���� ù��° ������Ʈ ��û�� ��� PendingTime �� 0���� ��쵵 �ִ�.
			// �� ��� 1 �̻� ���� ������ ���� ������ ��ġ�µ� ������ PendingTime �� ��� ������ ���������� �� ���� ����ϴ°� �ƴ϶� ���� ����� �ð��� ����ϱ⿡ ���� ����.
			if (m_TimeToServer.GetRequestUpdateDailyPlayStep() == ERequestUpdateDailyPlayStep::NotRequest)
			{
				m_TimeToServer.SetRequestUpdateDailyPlayStep(ERequestUpdateDailyPlayStep::RequestUpdateDailyTime);
				PendingTime = FMath::Max<int64>(1, PendingTime);
			}

			// 1 >= �� ���� �����޶���ؼ�.
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

	//// TODO:: ����ٰ� ~~~
	//if (PushManagerClass != nullptr)
	//{
	//	PushManagerClass->CancelLocalPushSchedule_RestPoint();
	//}

	//m_ResumeTime = FDateTime::UtcNow();
	//ResumeDelegate.Broadcast();
	//m_TimeToServer.CalcTimezoneOffset();

	//// PlayTime���� Pause �� �ð��� ����
	//FTimespan RemainTime = m_ResumeTime - m_PauseTime;
	//GamePlayTimeInfo.GamePlayTimeAfterRequest += RemainTime;

	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (B2GM == nullptr) //ABladeIIGameMode�� nullptr�� ���� B2DLCFrontGameMode�ۿ� ����� ��
	//	return;

	////[@AKI, 180806] NetGameMode�� ��� Pause�� �Ͻ����� ȭ�� �ȳ����� ����
	//if (B2GM->IsNetGameMode() == false)
	//{
	//	// ��Ʈ��ũ ���Ӹ�尡 �ƴ� ���ĵ��� ���Ӹ��� (�κ� / ������ / ������ž / ī���ʹ��� ��)
	//	FFunctionGraphTask::CreateAndDispatchWhenReady([ B2GM ]()
	//	{
	//		// [������] �� "�ݺ�����" �� "��������" �� ��� �ý���(FJavaWrapper::OnPlatformResumeDelegate) �� ���� ���� ����޴��� ��� ó�� ���� �ʴ´�
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
	//	//PvP, Raid, Control �϶��� ä���� ����ٴ� �˾��� ��� ��
	//	FBladeIIBlockToSyncNetwork::GetInstance().ChannelDisconnect(-1); 	//[@AKI, 180727] [B2CLT-3743] �ϴ��� ��� ���¡ ������ ���� Ŭ�󿡼� ������ ä�� ���� ���� �κ� ����
	//}
}

#if PLATFORM_ANDROID
void UB2GameInstance::PlatformPauseDelegate(JNIEnv *env, jobject thiz)
#else
void UB2GameInstance::PlatformPauseDelegate()
#endif
{
	UE_LOG(LogLoad, Log, TEXT("Platform!! PlatformPauseDelegate ~~~~~~~"));

	//// TODO:: ����ٰ� ~~~
	//if (PushManagerClass != nullptr)
	//{
	//	PushManagerClass->SetLocalPushSchedule_RestPoint();
	//}
	//
	//m_PauseTime = FDateTime::UtcNow();

	////[@AKI, 180727] [B2CLT-3743] �ϴ��� ��� ���¡ ������ ���� Ŭ�󿡼� ������ ä�� ���� ���� �κ� ����
	////[@AKI, 180806] TD�԰� ���� �� �ٽ� �츲
	//if (GetWorld() == nullptr || UGameplayStatics::GetGameMode(GetWorld()) == nullptr)
	//	return;

	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (B2GM == nullptr) //ABladeIIGameMode�� nullptr�� ���� B2DLCFrontGameMode�ۿ� ����� ��
	//	return;

	//B2GM->OnPlatformPause();

	//if (B2GM->IsNetGameMode() == false)
	//{
	//	//Lobby, Stage, HeroTower, CounterAttack, TM, Guild, Tutorial ���
	//}
	//else
	//{
	//	//PvP, Raid, Control �϶��� ä�� ��Ĺ ������ Close ��Ŵ
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
// Blueprint ��� InfoAsset �� ���Ӹ�� ĳ�� ����
// StaticFind~~ ������� GetGameMode �� �㿡 �̷��� ������ �� �� ����
//
UB2PCClassInfoBox* UB2GameInstance::GetPCClassInfoBox()
{
	if (!CachedPCClassInfoBox)
	{
		CachedPCClassInfoBox = StaticFindPCClassInfoBox(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
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
	//	CachedMobClassInfoBox = StaticFindMobClassInfoBox(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
	//}
	//BII_CHECK(CachedMobClassInfoBox);
	//return CachedMobClassInfoBox;
	return NULL;
}

UB2CommonSoundInfo* UB2GameInstance::GetCommonSoundInfo()
{
	if (!CachedCommonSoundInfo)
	{
		CachedCommonSoundInfo = StaticFindCommonSoundInfo(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
	}
	//BII_CHECK(CachedCommonSoundInfo || 
	//	// DLCFront Apk ���� UI ��ư ���� ������ �ʿ�� �� ���� ����. ���� DLC �ٿ�ε� ���� ���� ��Ȳ������ �ε��� ���ϴ� ��¿ �� ����.
	//	(WorldContext && Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(WorldContext->World())))
	//);
	return CachedCommonSoundInfo;
}

UB2DialogCharacterInfoSet* UB2GameInstance::GetDialogCharacterInfoTable()
{
	if (!CachedDialogCharacterInfoTable)
	{
		CachedDialogCharacterInfoTable = StaticFindDialogCharacterInfoTable(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
	}
	BII_CHECK(CachedDialogCharacterInfoTable);
	return CachedDialogCharacterInfoTable;
}

UB2WingInfo* UB2GameInstance::GetWingInfo()
{
	if (!CachedWingInfo)
	{
		CachedWingInfo = StaticFindWingInfo(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
	}
	BII_CHECK(CachedWingInfo);
	return CachedWingInfo;
}

UB2SomeInfo* UB2GameInstance::GetSomeInfo()
{
	if (!CachedSomeInfo)
	{
		CachedSomeInfo = StaticFindSomeInfo(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
	}
	BII_CHECK(CachedSomeInfo);
	return CachedSomeInfo;
}

UB2ItemMiscInfo* UB2GameInstance::GetItemMiscInfo()
{
	if (!CachedItemMiscInfo)
	{
		CachedItemMiscInfo = StaticFindItemMiscInfo(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
	}
	BII_CHECK(CachedItemMiscInfo);
	return CachedItemMiscInfo;
}

UB2LoadingImageInfo* UB2GameInstance::GetLoadingImageInfo()
{
	if (!CachedLoadingImageInfo)
	{
		CachedLoadingImageInfo = StaticFindLoadingImageInfo(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
	}
	BII_CHECK(CachedLoadingImageInfo);
	return CachedLoadingImageInfo;
}

class UB2NoAnimSkillInfo* UB2GameInstance::GetNoAnimSkillInfo()
{
	if (!CachedNoAnimSkillInfo)
	{
		CachedNoAnimSkillInfo = StaticFindNoAnimSkillInfo(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
	}
	BII_CHECK(CachedNoAnimSkillInfo);
	return CachedNoAnimSkillInfo;
}

UB2DamageEffectInfo* UB2GameInstance::GetDamageEffectInfo()
{
	if (!CachedDamageEffectInfo)
	{
		CachedDamageEffectInfo = StaticFindDamageEffectInfo(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
	}
	BII_CHECK(CachedDamageEffectInfo);
	return CachedDamageEffectInfo;
}

UB2BuffModeEffectInfo* UB2GameInstance::GetBuffModeEffectInfo()
{
	if (!CachedBuffModeEffectInfo)
	{
		CachedBuffModeEffectInfo = StaticFindBuffModeEffectInfo(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
	}
	BII_CHECK(CachedBuffModeEffectInfo);
	return CachedBuffModeEffectInfo;
}

UB2SkillAnimInfo* UB2GameInstance::GetSkillAnimInfo()
{
	if (!CachedSkillAnimInfo)
	{
		CachedSkillAnimInfo = StaticFindSkillAnimInfo(nullptr); // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ����
	}
	BII_CHECK(CachedSkillAnimInfo);
	return CachedSkillAnimInfo;
}
UB2GuildMarkInfo* UB2GameInstance::GetGuildMarkInfo()
{
	if (!CachedGuildMarkInfo)
	{
		CachedGuildMarkInfo = StaticFindGuildMarkInfo(nullptr);  // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ���� 
	}
	BII_CHECK(CachedGuildMarkInfo);
	return CachedGuildMarkInfo;
}
UB2InfluenceSkillInfo* UB2GameInstance::GetInfluenceSkillInfo()
{
	if (!CachedInfluenceSkillInfo)
	{
		CachedInfluenceSkillInfo = StaticFindInfluenceSkillInfo(nullptr);  // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ���� 
	}
	BII_CHECK(CachedInfluenceSkillInfo);
	return CachedInfluenceSkillInfo;
}
//UB2GuildNPCInfo* UB2GameInstance::GetGuildNPCInfo()
//{
//	if (!CachedGuildNPCInfo)
//	{
//		CachedGuildNPCInfo = StaticFindGuildNPCInfo(nullptr);  // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ���� 
//	}
//	BII_CHECK(CachedGuildNPCInfo);
//	return CachedGuildNPCInfo;
//}
UB2GuildSkillInfo* UB2GameInstance::GetGuildSkillInfo()
{
	if (!CachedGuildSkillInfo)
	{
		CachedGuildSkillInfo = StaticFindGuildSkillInfo(nullptr);  // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ���� 
	}
	BII_CHECK(CachedGuildSkillInfo);
	return CachedGuildSkillInfo;
}

UB2CombatConstantInfo * UB2GameInstance::GetCombatConstantInfo()
{
	if (!CachedCombatConstantInfo)
	{
		CachedCombatConstantInfo = StaticFindCombatConstantInfo(nullptr);  // ���⼭ this �� �Ѱ��ָ� ���� ȣ�� ���� 
	}
	BII_CHECK(CachedCombatConstantInfo);
	return CachedCombatConstantInfo;
}
