// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
//#include "LocalPushManager.h"
#include "B2PlatformAPI.h"
#include "CommonStruct.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif
#include "B2GameInstance.generated.h"

/**
 *
 */
UCLASS(config = Game)
class BLADEII_API UB2GameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()

private:
	class FB2Time
	{
	public:
		FB2Time()
		: RecieveServerTime(false)
		, RequestUpdateDailyPlayStep(ERequestUpdateDailyPlayStep::NotRequest)
		, m_TimeZone(0)
		{
			CalcTimezoneOffset();
		}
		
		/*  Get Time synchronized with server */
		void SetUTCNow(const int64& InTimeMillionSec);
		bool IsInitializeServerInfo() const { return RecieveServerTime; }
		void SetRequestUpdateDailyPlayStep(const ERequestUpdateDailyPlayStep InRequestUpdateDailyPlayStep) { RequestUpdateDailyPlayStep = InRequestUpdateDailyPlayStep; }
		ERequestUpdateDailyPlayStep GetRequestUpdateDailyPlayStep() const { return RequestUpdateDailyPlayStep; }
		FDateTime GetUTCNow();
		FDateTime GetLocalNow();
		FDateTime GetKoreaTimeNow();
		int32 GetTimeZone(ETimeZone InTimeZone);
		void CalcTimezoneOffset();
		FDateTime GetUtcToLocalTime(FDateTime InDateTime);
		int32 ConvertToSvrDayOfWeek();
		int32 ConvertToDayOfWeek(FDateTime InDateTime);

	protected:
	private:

	private:
		bool RecieveServerTime;
		ERequestUpdateDailyPlayStep RequestUpdateDailyPlayStep;
		FDateTime CachingServerTime;
		FDateTime CachingClientTime;

		FTimespan m_TimeZone;
	};

	struct InGamePlayTime
	{
		float GamePlayTime;
		FDateTime GamePlayTimeAfterRequest;
		InGamePlayTime();
	};

public:
	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	virtual void Init() override;
	virtual void Shutdown() override;

	bool Tick(float DeltaTime);
	int64 GetPauseResumeIntervalTime();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResumeDelegate);
	FResumeDelegate ResumeDelegate;
	void SendKakaoMessgaeInboxProcessing();

	static void SetUTCNow(const int64& InTimeMillionSec) { m_TimeToServer.SetUTCNow(InTimeMillionSec); };
	static FDateTime GetUTCNow() { return m_TimeToServer.GetUTCNow(); };
	static FDateTime GetLocalNow() { return m_TimeToServer.GetLocalNow(); };
	static int32 GetTimeZone(ETimeZone InTimeZone) { return m_TimeToServer.GetTimeZone(InTimeZone); }
	static FDateTime GetUtcToLocalTime(FDateTime InDateTime) { return m_TimeToServer.GetUtcToLocalTime(InDateTime); }
	static int32 ConvertToSvrDayOfWeek() { return m_TimeToServer.ConvertToSvrDayOfWeek(); }
	static int32 ConvertToDayOfWeek(FDateTime InDateTime) { return m_TimeToServer.ConvertToDayOfWeek(InDateTime); }

	static FDateTime GetKoreaTimeNow() { return m_TimeToServer.GetKoreaTimeNow(); };
	static FDateTime GetPlayTimeAfterRequest() { return GamePlayTimeInfo.GamePlayTimeAfterRequest; }
	static bool IncludeTimeNow(const int64 InStartTime, const int64 InEndTime);

	static void SetPlayTimeInfo(float InTime);
	static void RequestUpdateDailyPlayTime(bool IsForce = true);

	static float GetPlayTime();

	void EnableSavePlayTime(bool InEnable);
	void SendPlayTimeToServer();


#if PLATFORM_ANDROID
	void PlatformResumeDelegate(JNIEnv *env, jobject thiz);
	void PlatformPauseDelegate(JNIEnv *env, jobject thiz);

	void MessageBoxCompletedDelegate(JNIEnv *env, jobject thiz);

#else //PLATFORM_IOS || PLATFORM_WINDOWS
	void PlatformResumeDelegate();
	void PlatformPauseDelegate();

	void MessageBoxCompletedDelegate(bool bSuccess);
#endif

private:
	// AndroidLifeCycle Function
	/*void OnAndroid_Create();
	void OnAndroid_Start();
	void OnAndroid_Freeze();
	void OnAndroid_Stop();
	void OnAndroid_Destroy();
	void OnAndroid_Restart();*/

	static FB2Time m_TimeToServer;
	static FDateTime m_LastRequestPlayTime;
	static InGamePlayTime GamePlayTimeInfo;

	FDelegateHandle TickDelegateHandle;

	//LocalPushManager* PushManagerClass;
	FDateTime m_ResumeTime;
	FDateTime m_PauseTime;

	bool m_bSendOK;
	FThreadSafeBool m_MessageBoxCalled;
	FTimerHandle SendMessageInboxTimer;
	FTimerHandle SavePlayTimeHandler;
	void OnSendMessageInboxTimerCB();

	FDelegateHandle OnPlatformResumeHandler;
	FDelegateHandle OnKPlatformPauseHandler;
	FDelegateHandle OnMessageBoxCompletedHandler;


protected:
	//////////////////////////////////////////////////////////////////////////////
	 //Info Asset references. Other info assets based on DataTable are at BladeIIGameImpl
	UPROPERTY()
	class UB2NPCClassInfoBox* CachedMobClassInfoBox;
	UPROPERTY()
	class UB2PCClassInfoBox* CachedPCClassInfoBox;
	UPROPERTY()
	class UB2CommonSoundInfo* CachedCommonSoundInfo;
	UPROPERTY()
	class UB2DialogCharacterInfoSet* CachedDialogCharacterInfoTable;
	UPROPERTY()
	class UB2WingInfo* CachedWingInfo;
	UPROPERTY()
	class UB2SomeInfo* CachedSomeInfo;
	UPROPERTY()
	class UB2ItemMiscInfo* CachedItemMiscInfo;
	UPROPERTY()
	class UB2LoadingImageInfo* CachedLoadingImageInfo;
	UPROPERTY()
	class UB2NoAnimSkillInfo* CachedNoAnimSkillInfo;
	UPROPERTY()
	class UB2DamageEffectInfo* CachedDamageEffectInfo;
	UPROPERTY()
	class UB2BuffModeEffectInfo* CachedBuffModeEffectInfo;
	UPROPERTY()
	class UB2SkillAnimInfo* CachedSkillAnimInfo;
	UPROPERTY()
	class UB2GuildMarkInfo* CachedGuildMarkInfo;
	UPROPERTY()
	class UB2InfluenceSkillInfo* CachedInfluenceSkillInfo;
	//UPROPERTY()
	//class UB2GuildNPCInfo* CachedGuildNPCInfo;
	UPROPERTY()
	class UB2GuildSkillInfo* CachedGuildSkillInfo;
	UPROPERTY()
	class UB2CombatConstantInfo* CachedCombatConstantInfo;

public:
	/** Find or get cached asset reference. */
	class UB2PCClassInfoBox* GetPCClassInfoBox();
	class UB2InGameHUDInfo_Player* GetPlayerHUDInfo(EPCClass InCharClass);
	class UB2NPCClassInfoBox* GetMobClassInfoBox();
	class UB2CommonSoundInfo* GetCommonSoundInfo();
	class UB2DialogCharacterInfoSet* GetDialogCharacterInfoTable();
	class UB2WingInfo* GetWingInfo();
	class UB2SomeInfo* GetSomeInfo();
	class UB2ItemMiscInfo* GetItemMiscInfo();
	class UB2LoadingImageInfo* GetLoadingImageInfo();
	class UB2NoAnimSkillInfo* GetNoAnimSkillInfo();
	class UB2DamageEffectInfo* GetDamageEffectInfo();
	class UB2BuffModeEffectInfo* GetBuffModeEffectInfo();
	class UB2SkillAnimInfo* GetSkillAnimInfo();
	class UB2GuildMarkInfo* GetGuildMarkInfo();
	class UB2InfluenceSkillInfo* GetInfluenceSkillInfo();
//	class UB2GuildNPCInfo* GetGuildNPCInfo();
	class UB2GuildSkillInfo* GetGuildSkillInfo();
	class UB2CombatConstantInfo* GetCombatConstantInfo();
};
