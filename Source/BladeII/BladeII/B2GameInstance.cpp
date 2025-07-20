// Fill out your copyright notice in the Description page of Project Settings.
#include "B2GameInstance.h"
//#include "BladeII.h"
#include "Event.h"
//#include "B2NetGameMode.h"
//#include "B2StageGameMode.h"

//#include "Retailer.h"

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
#include "../InfoAsset/B2PCClassInfoBox.h"
//#include "B2PCClassInfoBox.h"
//#include "B2NPCClassInfoBox.h"
//#include "B2CommonSoundInfo.h"
//#include "B2ScenarioDialogInfo.h"
//#include "B2CharacterTagDialogueInfo.h"
//#include "B2WingInfo.h"
//#include "B2SomeInfo.h"
//#include "B2LoadingImageInfo.h"
//#include "B2DamageEffectInfo.h"
//#include "B2BuffModeEffectInfo.h"
//#include "B2SkillInfo.h"
//#include "B2GuildMarkInfo.h"
//#include "B2InfluenceSkillInfo.h"
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

// 辑滚 矫埃阑 概平 罐绰霸 酒聪扼 距4檬 林扁肺 罐扁 锭巩俊 弊 荤捞狼 矫埃篮 努扼俊辑 盲况霖促.
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
	FDateTime DateNow = GetKoreaTimeNow();//GetLocalNow();  //[TO-DO][@AKI 180603]老窜 茄惫 呼靛父 积阿秦辑 KoreaTime栏肺 函版. 眠饶 臂肺国 辑厚胶锭绰 UTC肺 函版秦具凳.
	return ConvertToDayOfWeek(DateNow);
}

int32 UB2GameInstance::FB2Time::ConvertToDayOfWeek(FDateTime InDateTime)
{
	int32 nLocalDayofweek = static_cast<int32>(InDateTime.GetDayOfWeek());
	int32 nConvertDayOfWeek = nLocalDayofweek + 1; // Monday to Sunday
	int32 nConvertBaseInt = nConvertDayOfWeek % 7 + 1; // 辑滚绰 1肺 矫累. Engine Enum篮 0

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
	: Super(ObjectInitializer)
	//,PushManagerClass(nullptr)
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
//	// 平积己
//	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UB2GameInstance::Tick));
//	
//	if (PushManagerClass == nullptr)
//		PushManagerClass = new LocalPushManager();
//
//#if !PLATFORM_MAC
//	// 牢聚 檬扁拳
//	B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->SubscribeEvents();
//#endif
//
//#if PLATFORM_ANDROID // 肺拿仟浆侩~
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
//	// 矫埃 穿利x
//	EnableSavePlayTime(false);
//
//#if !PLATFORM_MAC
//	// 牢聚 秦力
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
	//if (m_MessageBoxCalled)
	//{
	//	m_MessageBoxCalled.AtomicSet(false);
	//	GetWorld()->GetTimerManager().ClearTimer(SendMessageInboxTimer);
	//	GetWorld()->GetTimerManager().SetTimer(SendMessageInboxTimer, this, &UB2GameInstance::OnSendMessageInboxTimerCB, 60.0f, false);
	//	m_bSendOK = false;

	//	UE_LOG(LogLoad, Log, TEXT("Platform!! RequestKakaoMessageProcessing!!!!!!!!!!!!!!!! ~~~~~~~"));
	//	data_trader::Retailer::GetInstance().RequestKakaoMessageProcessing("inbox");
	//}
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
   m_LastRequestPlayTime : Request啊 磊林 啊瘤臼霸 Interval阑 眉农窍绰 侩档
   RequestInterval       : Interval 埃拜
   GamePlayTimeInfo.GamePlayTimeAfterRequest : 瘤抄锅俊 Request甫 焊辰 矫阿
*/
void UB2GameInstance::RequestUpdateDailyPlayTime(bool IsForce)
{
	//CONSTEXPR auto RequestInterval = 300.0;
	//const auto UTCNow = GetUTCNow();
	//const auto CurrentRequestInterval = (UTCNow - m_LastRequestPlayTime).GetTotalSeconds();

	//if (m_TimeToServer.IsInitializeServerInfo() // 匙飘况农 矫埃悼扁拳甫 茄 惑怕牢啊? 
	//	&& (IsForce || CurrentRequestInterval > RequestInterval || m_TimeToServer.GetRequestUpdateDailyPlayStep() == ERequestUpdateDailyPlayStep::NotRequest)) // 碍力啊 酒聪扼搁 Interval捞 瘤车芭唱 茄锅档 绝单捞飘 茄利 绝绰啊
	//{
	//	// 泅犁矫埃 - 弥辟 Request甫 焊辰 矫埃 = PendingTime
	//	int64 PendingTime = (UTCNow - GamePlayTimeInfo.GamePlayTimeAfterRequest).GetTicks() / 10000000;
	//	m_LastRequestPlayTime = UTCNow; // Interval 檬扁拳

	//	// 澜荐芭唱, 10盒焊促 努 锭 肋给等 蔼栏肺 埃林窍绊 檬扁拳
	//	if (PendingTime < 0 || PendingTime > 600)
	//	{
	//		GamePlayTimeInfo.GamePlayTimeAfterRequest = UTCNow;
	//		return;
	//	}
	//	else
	//	{
	//		// 啊厘 霉锅掳 诀单捞飘 夸没狼 版快 PendingTime 捞 0檬老 版快档 乐促.
	//		// 捞 版快 1 捞惑 蔼阑 焊郴扁 困秦 焊沥阑 芭摹绰单 焊沥等 PendingTime 阑 淬酒 焊郴档 辑滚俊辑绰 捞 蔼阑 荤侩窍绰霸 酒聪扼 流立 拌魂茄 矫埃阑 荤侩窍扁俊 巩力 绝促.
	//		if (m_TimeToServer.GetRequestUpdateDailyPlayStep() == ERequestUpdateDailyPlayStep::NotRequest)
	//		{
	//			m_TimeToServer.SetRequestUpdateDailyPlayStep(ERequestUpdateDailyPlayStep::RequestUpdateDailyTime);
	//			PendingTime = FMath::Max<int64>(1, PendingTime);
	//		}

	//		// 1 >= 牢 蔼阑 焊郴崔扼绊秦辑.
	//		if (PendingTime >= 1)
	//		{
	//			data_trader::Retailer::GetInstance().Retailer::RequestUpdateDailyPlayTime(static_cast<int32>(PendingTime));
	//		}
	//	}
	//}
}

#if PLATFORM_ANDROID
void UB2GameInstance::PlatformResumeDelegate(JNIEnv *env, jobject thiz)
#else
void UB2GameInstance::PlatformResumeDelegate()
#endif
{
	UE_LOG(LogLoad, Log, TEXT("Platform!! PlatformResumeDelegate ~~~~~~~"));

	//// TODO:: 咯扁促啊 ~~~
	//if (PushManagerClass != nullptr)
	//{
	//	PushManagerClass->CancelLocalPushSchedule_RestPoint();
	//}

	//m_ResumeTime = FDateTime::UtcNow();
	//ResumeDelegate.Broadcast();
	//m_TimeToServer.CalcTimezoneOffset();

	//// PlayTime俊辑 Pause 等 矫埃阑 哗淋
	//FTimespan RemainTime = m_ResumeTime - m_PauseTime;
	//GamePlayTimeInfo.GamePlayTimeAfterRequest += RemainTime;

	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (B2GM == nullptr) //ABladeIIGameMode啊 nullptr牢 版快绰 B2DLCFrontGameMode观俊 绝绢具 窃
	//	return;

	////[@AKI, 180806] NetGameMode老 版快 Pause矫 老矫沥瘤 拳搁 救唱坷霸 函版
	//if (B2GM->IsNetGameMode() == false)
	//{
	//	// 匙飘况农 霸烙葛靛啊 酒囱 胶拍靛倔沸 霸烙葛靛甸 (肺厚 / 葛氰葛靛 / 康旷狼啪 / 墨款磐带傈 殿)
	//	FFunctionGraphTask::CreateAndDispatchWhenReady([ B2GM ]()
	//	{
	//		// [葛氰葛靛] 吝 "馆汗傈捧" 棺 "楷加傈捧" 老 版快 矫胶袍(FJavaWrapper::OnPlatformResumeDelegate) 阑 烹秦 甸绢柯 肛勉皋春狼 版快 贸府 窍瘤 臼绰促
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
	//	//PvP, Raid, Control 老锭绰 盲澄捞 谗板促绰 扑诀阑 剁况 淋
	//	FBladeIIBlockToSyncNetwork::GetInstance().ChannelDisconnect(-1); 	//[@AKI, 180727] [B2CLT-3743] 老措老 葛靛 绢轰隆 俺急阑 困秦 努扼俊辑 碍力肺 盲澄 辑滚 谗绰 何盒 力芭
	//}
}

#if PLATFORM_ANDROID
void UB2GameInstance::PlatformPauseDelegate(JNIEnv *env, jobject thiz)
#else
void UB2GameInstance::PlatformPauseDelegate()
#endif
{
	UE_LOG(LogLoad, Log, TEXT("Platform!! PlatformPauseDelegate ~~~~~~~"));

	//// TODO:: 咯扁促啊 ~~~
	//if (PushManagerClass != nullptr)
	//{
	//	PushManagerClass->SetLocalPushSchedule_RestPoint();
	//}
	//
	//m_PauseTime = FDateTime::UtcNow();

	////[@AKI, 180727] [B2CLT-3743] 老措老 葛靛 绢轰隆 俺急阑 困秦 努扼俊辑 碍力肺 盲澄 辑滚 谗绰 何盒 力芭
	////[@AKI, 180806] TD丛苞 稠狼 饶 促矫 混覆
	//if (GetWorld() == nullptr || UGameplayStatics::GetGameMode(GetWorld()) == nullptr)
	//	return;

	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//if (B2GM == nullptr) //ABladeIIGameMode啊 nullptr牢 版快绰 B2DLCFrontGameMode观俊 绝绢具 窃
	//	return;

	//B2GM->OnPlatformPause();

	//if (B2GM->IsNetGameMode() == false)
	//{
	//	//Lobby, Stage, HeroTower, CounterAttack, TM, Guild, Tutorial 殿殿
	//}
	//else
	//{
	//	//PvP, Raid, Control 老锭绰 盲澄 家墓 碍力肺 Close 矫糯
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
// Blueprint 扁馆 InfoAsset 甸 霸烙葛靛 某教 滚傈
// StaticFind~~ 揪府令焊促 GetGameMode 茄 淬俊 捞犯霸 掘绢坷绰 霸 任 狐抚
//
UB2PCClassInfoBox* UB2GameInstance::GetPCClassInfoBox()
{
	if (!CachedPCClassInfoBox)
	{
		CachedPCClassInfoBox = StaticFindPCClassInfoBox(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
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
	//	CachedMobClassInfoBox = StaticFindMobClassInfoBox(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
	//}
	//BII_CHECK(CachedMobClassInfoBox);
	//return CachedMobClassInfoBox;
	return NULL;
}

UB2CommonSoundInfo* UB2GameInstance::GetCommonSoundInfo()
{
	if (!CachedCommonSoundInfo)
	{
		CachedCommonSoundInfo = StaticFindCommonSoundInfo(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
	}
	//BII_CHECK(CachedCommonSoundInfo || 
	//	// DLCFront Apk 俊辑 UI 滚瓢 荤款靛 锭巩俊 鞘夸肺 且 锭啊 乐澜. 酒流 DLC 促款肺靛 罐瘤 臼篮 惑炔俊辑绰 肺爹阑 给窍聪 绢驴 荐 绝瘤.
	//	(WorldContext && Cast<AB2DLCFrontGameMode>(UGameplayStatics::GetGameMode(WorldContext->World())))
	//);
	return CachedCommonSoundInfo;
}

UB2DialogCharacterInfoSet* UB2GameInstance::GetDialogCharacterInfoTable()
{
	if (!CachedDialogCharacterInfoTable)
	{
		CachedDialogCharacterInfoTable = StaticFindDialogCharacterInfoTable(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
	}
	BII_CHECK(CachedDialogCharacterInfoTable);
	return CachedDialogCharacterInfoTable;
}

UB2WingInfo* UB2GameInstance::GetWingInfo()
{
	if (!CachedWingInfo)
	{
		CachedWingInfo = StaticFindWingInfo(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
	}
	BII_CHECK(CachedWingInfo);
	return CachedWingInfo;
}

UB2SomeInfo* UB2GameInstance::GetSomeInfo()
{
	if (!CachedSomeInfo)
	{
		CachedSomeInfo = StaticFindSomeInfo(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
	}
	BII_CHECK(CachedSomeInfo);
	return CachedSomeInfo;
}

UB2ItemMiscInfo* UB2GameInstance::GetItemMiscInfo()
{
	if (!CachedItemMiscInfo)
	{
		CachedItemMiscInfo = StaticFindItemMiscInfo(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
	}
	BII_CHECK(CachedItemMiscInfo);
	return CachedItemMiscInfo;
}

UB2LoadingImageInfo* UB2GameInstance::GetLoadingImageInfo()
{
	if (!CachedLoadingImageInfo)
	{
		CachedLoadingImageInfo = StaticFindLoadingImageInfo(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
	}
	BII_CHECK(CachedLoadingImageInfo);
	return CachedLoadingImageInfo;
}

class UB2NoAnimSkillInfo* UB2GameInstance::GetNoAnimSkillInfo()
{
	if (!CachedNoAnimSkillInfo)
	{
		CachedNoAnimSkillInfo = StaticFindNoAnimSkillInfo(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
	}
	BII_CHECK(CachedNoAnimSkillInfo);
	return CachedNoAnimSkillInfo;
}

UB2DamageEffectInfo* UB2GameInstance::GetDamageEffectInfo()
{
	if (!CachedDamageEffectInfo)
	{
		CachedDamageEffectInfo = StaticFindDamageEffectInfo(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
	}
	BII_CHECK(CachedDamageEffectInfo);
	return CachedDamageEffectInfo;
}

UB2BuffModeEffectInfo* UB2GameInstance::GetBuffModeEffectInfo()
{
	if (!CachedBuffModeEffectInfo)
	{
		CachedBuffModeEffectInfo = StaticFindBuffModeEffectInfo(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
	}
	BII_CHECK(CachedBuffModeEffectInfo);
	return CachedBuffModeEffectInfo;
}

UB2SkillAnimInfo* UB2GameInstance::GetSkillAnimInfo()
{
	if (!CachedSkillAnimInfo)
	{
		CachedSkillAnimInfo = StaticFindSkillAnimInfo(nullptr); // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇
	}
	BII_CHECK(CachedSkillAnimInfo);
	return CachedSkillAnimInfo;
}
UB2GuildMarkInfo* UB2GameInstance::GetGuildMarkInfo()
{
	if (!CachedGuildMarkInfo)
	{
		CachedGuildMarkInfo = StaticFindGuildMarkInfo(nullptr);  // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇 
	}
	BII_CHECK(CachedGuildMarkInfo);
	return CachedGuildMarkInfo;
}
UB2InfluenceSkillInfo* UB2GameInstance::GetInfluenceSkillInfo()
{
	if (!CachedInfluenceSkillInfo)
	{
		CachedInfluenceSkillInfo = StaticFindInfluenceSkillInfo(nullptr);  // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇 
	}
	BII_CHECK(CachedInfluenceSkillInfo);
	return CachedInfluenceSkillInfo;
}
//UB2GuildNPCInfo* UB2GameInstance::GetGuildNPCInfo()
//{
//	if (!CachedGuildNPCInfo)
//	{
//		CachedGuildNPCInfo = StaticFindGuildNPCInfo(nullptr);  // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇 
//	}
//	BII_CHECK(CachedGuildNPCInfo);
//	return CachedGuildNPCInfo;
//}
UB2GuildSkillInfo* UB2GameInstance::GetGuildSkillInfo()
{
	if (!CachedGuildSkillInfo)
	{
		CachedGuildSkillInfo = StaticFindGuildSkillInfo(nullptr);  // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇 
	}
	BII_CHECK(CachedGuildSkillInfo);
	return CachedGuildSkillInfo;
}

UB2CombatConstantInfo * UB2GameInstance::GetCombatConstantInfo()
{
	if (!CachedCombatConstantInfo)
	{
		CachedCombatConstantInfo = StaticFindCombatConstantInfo(nullptr);  // 咯扁辑 this 甫 逞败林搁 公茄 龋免 风橇 
	}
	BII_CHECK(CachedCombatConstantInfo);
	return CachedCombatConstantInfo;
}
