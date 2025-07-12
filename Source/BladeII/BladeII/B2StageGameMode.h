// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "B2StageGameModeBase.h"
#include "B2StageGameMode.generated.h"


class StageGameRule : public DefaultGameRule
{
public:
	virtual bool AllowToTag() const override { return true; }
	virtual bool IsQTEEnabled() const override { return true; }
	virtual bool ConditionalUseSectionMergedPCModel() const override { return true; }
};

/**
 * GameMode for the most basic stage or scenario based mode game. Combat through stages.
 */
UCLASS()
class BLADEII_API AB2StageGameMode : public AB2StageGameModeBase
{
	GENERATED_BODY()
	
protected:	
	virtual void BeginPlay() override;
	virtual void SetDesiredPCClassEnum() override;
	virtual void SubscribeEvents() override;
	virtual void PreBeginPlay() override;

	virtual void RestoreWholeGameTimeScale() override;

	virtual void SetMatchState(FName NewState) override;

public:
	//virtual EResurrectGameModeType GetResurrectGameModeType() final { return EResurrectGameModeType::EResurrectType_Stage; }
	virtual void Tick(float DeltaSeconds) override;
	virtual const FString GetOpeningMovieName() override;
	virtual void GetMovieSubtitles(TArray<B2MovieSubtitle>* Subtitles) override;

	/** Here, it loads the main map of current stage by StageInfo, instead of re-loading current loaded level. */
	virtual void RestartGame() override;
	virtual void RestartGameNext();
	virtual void GoToMap() override;
	virtual void GoToDeferredStage(int32 InNextClientStageId) override;
	virtual void GoToNextArea() override;
	virtual void GoToVillageDefeated() override;
	virtual void GiveUpGame() override;
	virtual void EnterShop(int32 nWhereShop) override;

	bool OpenEquipNumberNotice();

	/** Load the main map of current stage by StageInfo */
	void RestartStage();

	/** Overridden to handle StageInfo's PlayerStartTag */
	virtual void OverrideDefaultPawnStartLocation(AController* NewPlayerController, FVector& InOutStartLocation) /*override*/;

	/** Wrapper to StageManager's GetCurrentStageNumber for Blueprint access. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	int32 GetCurrentStageNumber() const;

	/** Wrapper to StageManager's GetCurrentStageNumber for Blueprint access. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	int32 GetCurrentServerStageNumber() const;

	/** Set level deployed objects to be used by stage clear event scene. */
	UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	void SetStageClearViewObjects(class ACameraActor* InCamActor, class UParticleSystemComponent* InPSWingComp, class UParticleSystemComponent* InPSGemComp, class UParticleSystemComponent* InPSBGComp);

	//UFUNCTION(BlueprintCallable, Category = "BladeIIGame")
	virtual void PlayBGMCustomize(const FName& InName)override;

	void EnterHeroMgmtMode(EHeroMgmtSubMode InSubMode);

	virtual void SetupUIScene() override;
	virtual void OnStageBegin() override;

	virtual EB2GameMode GetB2GameModeType() override { return EB2GameMode::Scenario; }
	virtual bool IsQTEEnabled() { return true; }

private:
	void DeferredGoToLobby();

	//================================================================================
	// RepeatBattle
private:
	void OnBeginPlayOfRepeatBattle();
	void OnEndPlayOfSingleRepeatBattleStage();
public:
	bool IsRepeatBattleOn();
	bool IsRepeatBattleLoopAll();
	int GetRepeatBattleRemainingCount();
	virtual void CancelOrStopRepeatBattle() override;
	//================================================================================

	bool HasStageBuff(EStageBuffType InType);

	virtual FString GetLocalSavedAutoStateKeyName() const override { return TEXT("AutoState_StageGameMode"); } 
	virtual bool ShouldStartAutoFromBeginning() override;

private:	
	bool bRegistredDeferredUISceneChapter;
	bool bRegistredDeferredUISceneStageInfo;
	bool bRequestedGetActInfo;
	bool bRequestedGetStageInfo;	
	bool bEnterShop;
	bool bEnterInventory;
	int32 nRequestGeneralShop;
	
	int32 DeferredClientStageId;
};
