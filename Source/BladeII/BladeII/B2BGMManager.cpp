#include "B2BGMManager.h"
#include "BladeIIGameMode.h"
//#include "BladeII.h"
//#include "B2UIManager.h"



ChangeBGMRule_InInventory::ChangeBGMRule_InInventory()
{
	ExceptChangeUIScene.Add(EUIScene::PVPCharacterSelectPage);
	ExceptChangeUIScene.Add(EUIScene::CharacterSelectPage);
	ExceptChangeUIScene.Add(EUIScene::HeroTowerCharacterSelectPage);
	ExceptChangeUIScene.Add(EUIScene::RaidMain);
	ExceptChangeUIScene.Add(EUIScene::CounterAttackDGSelect);
	ExceptChangeUIScene.Add(EUIScene::ControlCharacterSelectPage);
}
bool ChangeBGMRule_InInventory::CheckChangeBGM(EUIScene UIScene)
{
	//if (UB2UIManager::IsInstanceNull())
	//	return true;

	//if (UIScene == EUIScene::Inventory)
	//{
	//	for (auto& ExceptUIScene : ExceptChangeUIScene)
	//	{
	//		if(UB2UIManager::GetInstance()->HasUISceneHistory(ExceptUIScene))
	//			return false;
	//	}
	//}
	return true;
}
//
//UB2BGMManager::UB2BGMManager(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	//FadeOutBGM = nullptr;
//	//CurrentPlayingBGM = nullptr;
//
//	//DefaultPtr = std::make_shared<DefaultState>(this);
//	//FadeInOutPtr = std::make_shared<FadeInOutState>(this);
//
//	//ChangeBGMRules.Emplace(TSharedPtr<ChangeBGMRule>(new ChangeBGMRule_InInventory()));
//
//	//ChangeState(Default);
//}
////
////void UB2BGMManager::Init(TWeakObjectPtr<class ABladeIIGameMode> InGameMode)
////{
////	GameMode = InGameMode;
////}
////
////UB2CommonSoundInfo* UB2BGMManager::GetCommonSoundInfo()
////{
////	if (GameMode.IsValid())
////	{
////		return StaticFindCommonSoundInfo(GameMode.Get());
////	}
////
////	return nullptr;
////}
////
////void UB2BGMManager::PlayBGMByStageContext(int32 InClientStageId, bool bIsExtraMap, EStageCombatSoundContext InStageContext, bool bForceStopCurrent)
////{
////	if (bForceStopCurrent) 
////		StopBGM();
////
////	if (GetCommonSoundInfo())
////	{
////		float Duration = 0.0f;
////		USoundCue* WantedSound = GetCommonSoundInfo()->GetBGMSoundOfStageCombat(InClientStageId, bIsExtraMap, InStageContext, Duration);
////		PlayBGMCommonInternal(WantedSound, Duration);
////	}
////}
////
////void UB2BGMManager::PlayBGMByStageCustomize(const FName& InName)
////{
////
////	if (GetCommonSoundInfo())
////	{
////		float Duration = 0.0f;
////		USoundCue* SoundInfo = GetCommonSoundInfo()->GetCustomizeBossBGM(InName);
////		PlayBGMCommonInternal(SoundInfo, Duration);
////
////	}
////}
////
////void UB2BGMManager::PlayBGMByCommonSoundID(ECommonSoundID InID, bool bForceStopCurrent)
////{
////	if (bForceStopCurrent)
////		StopBGM();
////
////	if (GetCommonSoundInfo())
////	{
////		USoundCue* WantedSound = GetCommonSoundInfo()->GetSoundByID(InID);
////		PlayBGMCommonInternal(WantedSound, GetCommonSoundInfo()->GetFadeInOutDuractionByID(InID));
////	}
////}
////
////bool UB2BGMManager::PlayBGMByUIScene(EUIScene InUIScene)
////{
////	bool IsPlaySuccess = false;
////
////	if (!CheckChangeBGMRules(InUIScene))
////		return IsPlaySuccess;
////
////	if (GetCommonSoundInfo())
////	{
////		USoundCue* WantedSound = nullptr;
////		float Duration = 0.0f;
////		if (GetCommonSoundInfo()->GetSoundByUIScene(InUIScene, &WantedSound, Duration))
////		{
////			PlayBGMCommonInternal(WantedSound, Duration);
////			IsPlaySuccess = true;
////		}
////	}
////
////	return IsPlaySuccess;
////}
////
////void UB2BGMManager::PlayBGMByPrevUIScene()
////{
////	//const TArray<TPair<EUIScene, FB2UISceneOnOpen>>& UISceneHistory = UB2UIManager::GetInstance()->GetUISceneHistory();
////
////	//const int32 iMaxPrevUISceneCount = UISceneHistory.Num() - 2;
////	//for (int32 i = iMaxPrevUISceneCount; i >= 0; --i)
////	//{
////	//	if (UISceneHistory.IsValidIndex(i) == false)
////	//	{
////	//		continue;
////	//	}
////
////	//	if (PlayBGMByUIScene(UISceneHistory[i].Key) == true)
////	//	{
////	//		break;
////	//	}
////	//}
////}
////
////void UB2BGMManager::SetPauseBGM(bool bPause)
////{
////	if (FadeOutBGM)
////		FadeOutBGM->SetPaused(bPause);
////
////	if (CurrentPlayingBGM)
////		CurrentPlayingBGM->SetPaused(bPause);
////}
////void UB2BGMManager::StopBGM()
////{
////	DestroyCurrentBGM();
////	DestroyFadeBGM();
////}
////
////void UB2BGMManager::ChangeState(BGMPlayState NextPlayState)
////{
////	std::shared_ptr<BGMState> ChangeStatePtr;
////	switch (NextPlayState)
////	{
////	case Default:
////		ChangeStatePtr = DefaultPtr;
////		break;
////	case FadeInOut:
////		ChangeStatePtr = FadeInOutPtr;
////		break;
////	}
////
////	if (CurrentState == ChangeStatePtr)
////		return;
////
////	if(CurrentState)
////		CurrentState->Leave();
////
////	CurrentState = ChangeStatePtr;
////	CurrentState->Enter();
////}
////bool UB2BGMManager::IsFadeInOutState()
////{
////	return CurrentState == FadeInOutPtr;
////}
////
////void UB2BGMManager::OnEndFadeInOut()
////{
////	CurrentState->OnEndFadeInOut();
////}
////
////void UB2BGMManager::DestroyBGM(UAudioComponent** Componet)
////{
////	if (Componet && *Componet)
////	{
////		//if (!(*Componet)->IsPendingKill())
////		{
////			(*Componet)->Stop();
////			(*Componet)->ConditionalBeginDestroy();
////		}
////		(*Componet) = NULL;
////	}
////}
////
////void UB2BGMManager::DestroyCurrentBGM()
////{
////	if (CurrentPlayingBGM)
////	{
////		DestroyBGM(&CurrentPlayingBGM);
////	}
////}
////void UB2BGMManager::DestroyFadeBGM()
////{
////	if (FadeOutBGM)
////	{
////		DestroyBGM(&FadeOutBGM);
////	}
////}
////
////bool UB2BGMManager::CheckChangeBGMRules(EUIScene InUIScene)
////{
////	for (TSharedPtr<ChangeBGMRule>& Rule : ChangeBGMRules)
////	{
////		if (Rule.IsValid() && !Rule->CheckChangeBGM(InUIScene))
////			return false;
////	}
////	return true;
////}
////
////void UB2BGMManager::PlayBGMCommonInternal(USoundCue* InBGMSound, float FadeInDuraction)
////{
////	if (InBGMSound)
////	{
////		USoundCue* CurrentPlayingSoundCue = (CurrentPlayingBGM && CurrentPlayingBGM->IsPlaying()) ? Cast<USoundCue>(CurrentPlayingBGM->Sound) : NULL;
////		const bool bSameOneAlreadyPlaying = (InBGMSound == CurrentPlayingSoundCue);
////
////		if (!bSameOneAlreadyPlaying)
////		{
////			ChangeState(FadeInDuraction > 0.0f ? FadeInOut : Default);
////			CurrentState->PlayBGMCommon(InBGMSound, FadeInDuraction);
////		}
////	}
////}
////
////void UB2BGMManager::DefaultState::Enter()
////{
////
////}
////void UB2BGMManager::DefaultState::Leave()
////{
////
////}
////void UB2BGMManager::DefaultState::PlayBGMCommon(USoundCue* InBGMSound, float FadeInDuraction)
////{
////	if (BGMManager)
////	{
////		BGMManager->StopBGM();
////		BGMManager->CurrentPlayingBGM = UGameplayStatics::SpawnSound2D(BGMManager->GameMode.Get(), InBGMSound);
////	}
////}
////
////void UB2BGMManager::DefaultState::OnEndFadeInOut()
////{
////	
////}
////
////void UB2BGMManager::FadeInOutState::Enter()
////{
////
////}
////void UB2BGMManager::FadeInOutState::Leave()
////{
////	if (BGMManager)
////	{
////		if (BGMManager->BGMFadeTimeHandler.IsValid())
////		{
////			UWorld* TheWorld = BGMManager->GameMode.IsValid() ? BGMManager->GameMode->GetWorld() : nullptr;
////			if (TheWorld)
////				TheWorld->GetTimerManager().ClearTimer(BGMManager->BGMFadeTimeHandler);
////		}
////		BGMManager->DestroyFadeBGM();
////	}
////}
////void UB2BGMManager::FadeInOutState::PlayBGMCommon(USoundCue* InBGMSound, float FadeInDuraction)
////{
////	if (BGMManager)
////	{
////		BGMManager->DestroyFadeBGM();
////
////		BGMManager->FadeOutBGM = BGMManager->CurrentPlayingBGM;
////		BGMManager->CurrentPlayingBGM = UGameplayStatics::SpawnSound2D(BGMManager->GameMode.Get(), InBGMSound);
////
////		if (BGMManager->FadeOutBGM)
////			BGMManager->FadeOutBGM->FadeOut(FadeInDuraction, 0.0f);
////
////		if(BGMManager->CurrentPlayingBGM)
////			BGMManager->CurrentPlayingBGM->FadeIn(FadeInDuraction);
////
////		UWorld* TheWorld = BGMManager->GameMode.IsValid() ? BGMManager->GameMode->GetWorld() : nullptr;
////		if (TheWorld)
////		{
////			if (BGMManager->BGMFadeTimeHandler.IsValid())
////				TheWorld->GetTimerManager().ClearTimer(BGMManager->BGMFadeTimeHandler);
////
////			TheWorld->GetTimerManager().SetTimer(BGMManager->BGMFadeTimeHandler, BGMManager, &UB2BGMManager::OnEndFadeInOut, FadeInDuraction, false);
////		}
////	}
////	else
////	{
////		UE_LOG(LogBladeII, Log, TEXT("[UB2BGMManager::FadeInOutState::PlayBGMCommon] BGMManager NULL!"));
////	}
////}
////
////void UB2BGMManager::FadeInOutState::OnEndFadeInOut()
////{
////	if (BGMManager)
////	{
////		BGMManager->ChangeState(Default);
////	}
////}
