//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "B2StageGameModeBase.h"
//#include "B2HeroTowerGameMode.generated.h"
//
///**
// * 
// */
//UCLASS()
//class BLADEII_API AB2HeroTowerGameMode : public AB2StageGameModeBase
//{
//	GENERATED_BODY()
////	
////public:
////	virtual void SetupUIScene() override;
////	virtual EB2GameMode GetB2GameModeType() override { return EB2GameMode::HeroTower; }
////	
////	virtual void SubscribeEvents() override; // Events of EventSubsystem
////	
////	void ReturnToHTMainMenu();
////
////	virtual void GiveUpGame() override;
////
////	virtual float GetCurrentStageDifficultyScale() override;
////
////	virtual void NotifyPlayerDeadEnd(class ABladeIIPlayer* DeadPlayer) override;
////
////	virtual bool IsQTEEnabled() { return true; }
////
////	virtual void PlayVictoryBGM() override;
////
////	virtual void Tick(float DeltaSeconds) override;
////	
////	virtual void  ResetAssumeNoWorldLoading() override;
////
////	// BladeIIGameMode interfaces...
////	virtual ABladeIICharacter* SpawnCharacter(UB2NPCClassInfoBox* InfoBox, ENPCClass InClass, ENPCClassVariation InVariation, FTransform& SpawnTransform, FActorSpawnParameters& SpawnParameters) override;//[@AKI, 171114] B2BUG-858 영웅의 탑 연속 전투 관련 오류 수정 Map 생성시에만 Mob Level을 Caching하는 것을 매 전투 시작 시로 변경
////	int32 GetHeroTowerFloor();
////	
////	//void CheckRemainTime();
////	FTimespan GetElapsedTime();
////	//FTimespan GetRemainTime();
////	float GetRemainTimeRate();
////
////	UFUNCTION(BlueprintCallable, Category = "BladeII")
////	void OnEndMatinee_MobDead();
////
////protected:
////	/** Called before any other actor's BeginPlay is called. */
////	virtual void PreBeginPlay() override;
////
////	virtual void BeginPlay() override;
////
////	void SetMobLevel();
////
////	virtual void NotifyUIWidgetOpenComplete(FName InOpenedUI) override;
////
////	virtual FString GetLocalSavedAutoStateKeyName() const override { return TEXT("AutoState_HeroTowerGameMode"); }
////
////private:
////	float m_fCachingMobLevel;
//};
