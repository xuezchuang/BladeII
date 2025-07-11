#pragma once
#include "UObject/Object.h"
#include "EngineMinimal.h"
#include <memory>
#include "../UI/B2UIEnum.h"
//#include "B2CommonSoundInfo.h"
#include "B2BGMManager.generated.h"

#define DEFINE_STATE_CLASS(CLASSNAME)												\
	class CLASSNAME##State : public BGMState										\
	{																				\
	public:																			\
		CLASSNAME##State(UB2BGMManager* InBGMManager) : BGMState(InBGMManager) {}	\
		virtual void Enter();														\
		virtual void Leave();														\
		virtual void PlayBGMCommon(USoundCue* InBGMSound, float FadeInDuraction);	\
		virtual void OnEndFadeInOut();												\
	};																				\

class ChangeBGMRule
{
public:
	virtual bool CheckChangeBGM(EUIScene UIScene) = 0;
};

class ChangeBGMRule_InInventory : public ChangeBGMRule
{
private:
	TArray<EUIScene> ExceptChangeUIScene;
public:
	ChangeBGMRule_InInventory();
	virtual ~ChangeBGMRule_InInventory() {}
	virtual bool CheckChangeBGM(EUIScene UIScene);
};

UCLASS(Transient)
class UB2BGMManager : public UObject
{
	GENERATED_BODY()
//
//private:
//	class BGMState
//	{
//	protected:
//		UB2BGMManager* BGMManager;
//	public:
//		BGMState(UB2BGMManager* InBGMManager) : BGMManager(InBGMManager)
//		{}
//		virtual ~BGMState() {}
//		virtual void Enter() = 0;
//		virtual void Leave() = 0;
//		virtual void PlayBGMCommon(USoundCue* InBGMSound, float FadeInDuraction) = 0;
//		virtual void OnEndFadeInOut() = 0;
//	};
//	DEFINE_STATE_CLASS(Default)
//	DEFINE_STATE_CLASS(FadeInOut)
//	
//	enum BGMPlayState
//	{
//		Default,
//		FadeInOut,
//	};
//
//public:
//	UB2BGMManager(const FObjectInitializer& ObjectInitializer);
//
//	void Init(TWeakObjectPtr<class ABladeIIGameMode> InGameMode);
//
//	//void PlayBGMByStageContext(int32 InClientStageId, bool bIsExtraMap, EStageCombatSoundContext InStageContext, bool bForceStopCurrent = false);
//	void PlayBGMByStageCustomize(const FName& InName);
//	//void PlayBGMByCommonSoundID(ECommonSoundID InID, bool bForceStopCurrent);
//	bool PlayBGMByUIScene(EUIScene InUIScene);
//	void PlayBGMByPrevUIScene();
//	void SetPauseBGM(bool bPause);
//	void StopBGM();
//
//private:
//	class UB2CommonSoundInfo* GetCommonSoundInfo();
//	
//	bool CheckChangeBGMRules(EUIScene InUIScene);
//
//	void PlayBGMCommonInternal(USoundCue* InBGMSound, float FadeInDuraction = 0.0f);
//	void ChangeState(BGMPlayState NextPlayState);
//	bool IsFadeInOutState();
//	void OnEndFadeInOut();
//	void DestroyBGM(UAudioComponent** Componet);
//	void DestroyCurrentBGM();
//	void DestroyFadeBGM();
//
//private:
//	TArray<TSharedPtr<ChangeBGMRule>> ChangeBGMRules;
//
//	std::shared_ptr<BGMState> DefaultPtr;
//	std::shared_ptr<BGMState> FadeInOutPtr;
//	std::shared_ptr<BGMState> CurrentState;
//	
//	FTimerHandle BGMFadeTimeHandler;
//	
//	TWeakObjectPtr<class ABladeIIGameMode> GameMode;
//
//	UPROPERTY(Transient)
//		class UAudioComponent* FadeOutBGM;
//	UPROPERTY(Transient)
//		class UAudioComponent* CurrentPlayingBGM;
};

