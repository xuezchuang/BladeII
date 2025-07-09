// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "CommonStruct.h"
#include "B2UIAdventureModSlot.h"
#include "B2UIAdventureModSelect.generated.h"

/**
 * UB2UIAdventureModSelect
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIAdventureModSelect : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void BindDelegates() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	
private:
	void SetAdventureDifficulty(EStageDifficulty Difficulty);
	void SetAdventureModLock(bool IsUnLock, bool IsUnLockAnimation);
	void SetScenarioComplete(bool IsComplete);
	void UpdateAdventureChapterInfo();
	void UpdateScenarioChapterInfo();
	void UpdateChapterLock(EStageDifficulty InStageDifficulty = EStageDifficulty::ESD_None);
	void GetAdventureLockState(EStageDifficulty InLastDifficulty, bool& OutCompleteScenario, bool& OutAdventureUnLock, bool& OutAdventureUnLockAni);

	void InitAdventureModeSlot();
	bool InitAdventureModeSlot_CheckBluePrint();
	void InitAdventureModeSlot_SetCurAdventureSlot();

	void ClickAdventureBTNSelect(EAdventureModeType AdventureModeType);
	void ClickAdventureBTNSelectSub(EAdventureModeType AdventureModeType);
	void ClickAdventureBTNLock(EAdventureModeType AdventureModeType);
	FAdventureModeSetting GetAdventureModeSetting(EAdventureModeType AdventureModeType);
	

	// delegates
public:
	UFUNCTION()
	void OnChangeDifficulty(class UB2UIDocBase* Sender, EStageDifficulty diff, EStageDifficulty PrevDiff);
	UFUNCTION()
	void OnChangeChapterNum(class UB2UIDocBase* Sender, int32 SubChapterNum, int32 PreChapterNum);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII AdventureModSelect")
	void StartBeginAnimation_BP(int32 SlotCount);	

	// binded control
protected:
	TMap<EAdventureModeType, TWeakObjectPtr<class UB2UIAdventureModSlot>> CurAdventureSlotMap;

	TWeakObjectPtr<UCanvasPanel>							CP_TwoDummySlot;
	TWeakObjectPtr<UCanvasPanel>							CP_ThreeDummySlot;

	TArray<TWeakObjectPtr<class UB2UIAdventureModSlot>>		TwoDummyChildList;
	TArray<TWeakObjectPtr<class UB2UIAdventureModSlot>>		ThreeDummyChildList;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<FAdventureModeSetting>	AdventureModeTypeDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<EAdventureModeType>		ShowAdventureSlotTypes;
};

