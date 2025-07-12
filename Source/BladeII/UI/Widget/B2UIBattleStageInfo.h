// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "CommonStruct.h"
#include "B2UIDocBindable.h"
#include "BladeIICharacter.h"
#include "B2ScrollBox.h"
#include "../B2RichTextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "../../DataStore/B2ClientDataStore.h"
#include "B2UIBattleStageInfo.generated.h"

/*
* Informing you before entering the battle is the UI. You can choose a hero to join the battle.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIBattleStageInfo : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	UB2UIBattleStageInfo(const FObjectInitializer& ObjectInitializer);
	virtual void BeginDestroy() override;

	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
private:
	void SubscribeEvent();
	void UnsubscribeEvent();
	void SetHeaderTitleAsChapterNum();
	void SetStageDiffulty();

public:
	//Get Binded ClientStageId
	int32 GetClientStageId()	const { return ClientStageId; }

	//BattleReward Info
	void AddRewardItem(const FB2RewardItemPreviewInfo& InItemData); // Must be scrolled when it exceeds four entry.
	void AddAetherItem(const FB2AetherItemPreviewInfo& InItemData); // Must be scrolled when it exceeds four entry.
	void AddMaterialItem(const FB2MaterialItemPreviewInfo& InItemData);
	void AddBossPieceRewardItem(const FStageFixedDropsInfo* _BossPieceData); // Must be scrolled when it exceeds four entry.
	void ClearRewardItems();
	void ArinCheck();
	FORCEINLINE void CancelStageGameStart() { StageGameStarted = false; }

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleStageInfo")
	class UB2UISlotItem* CreateRewardItem();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII BattleStageInfo")
	void RemoveAllRewardItems();

protected:
	virtual void OnOpenComplete() override;
	virtual void OnCloseComplete() override;

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	//Update the ability of the main character and the sub character. (The AttackPower and DefensePower values)
	void UpdateHerosPower();
	void SetStageGradeInfo(const TArray<bool>& Grade);
	void SetStageMainTexture(UTexture2D* MainTexture);
	void SetNPCTexture(UTexture2D* NPCTexture);
	void SetStageStory(FText StageStory);
	bool CheckAndInvokeArinConsulting(); // Invoked Arin consulting if returns true.
	void ShowRewardItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData);
	void ShowBossPieceInfo(const FB2RewardItemPreviewInfo& InRewardInfoData);
	void ShowEtherItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData);
	void ShowMaterialItemInfo(const FB2RewardItemPreviewInfo& InRewardInfoData);
	void HideRewardItemInfo();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterFrame")
	void BeginStarRewardAcquiredEvent_BP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterFrame")
	void PlayChangeChacterAnimBP();
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII ChapterFrame")
	void StageClearGrade(int32 Grade);

private:
	//===================================================Click
	UFUNCTION()
	void OnClickBtnChangeMainSub();
	UFUNCTION()
	void OnClickBtnBossPieceItem(class UB2UISlotItem* ClickedItem);
	UFUNCTION()
	void OnClickBtnRewardItem(class UB2UISlotItem* ClickedItem);
	UFUNCTION()
	void OnClickBtnEtherItem(class UB2UISlotItem* ClickedItem);
	UFUNCTION()
	void OnClickBtnMaterialItem(class UB2UISlotItem* ClickedItem);
	UFUNCTION()
	void OnClickBtnHideRewardItemInfoSet();
	UFUNCTION()
	void OnClickBattleReady();
	//====================================================Doc Delegate
	UFUNCTION()
	void OnChangedCurPCClass(class UB2UIDocBase* Sender, int32 CurPCClass, int32 PrevCurPCClass);
	UFUNCTION()
	void OnChangedTagPCClass(class UB2UIDocBase* Sender, int32 TagPCClass, int32 PrevTagPCClass);

protected:
	TWeakObjectPtr<UB2Button>						BTN_BattleReady;

	TWeakObjectPtr<UB2ScrollBox>					SB_RewardItems; // Where the RewardItems are created.
	TArray<class UB2UISlotItem*>					BattleRewardItems;
	TWeakObjectPtr<class UB2UIBattleStageInfoToolTip>		UIP_BattleStageInfoToolTip;

	TWeakObjectPtr<UB2Image>						IMG_MainTexture;
	TWeakObjectPtr<UB2Image>						IMG_NPC;
	TWeakObjectPtr<UB2RichTextBlock>				TB_StageStory;
	TWeakObjectPtr<UB2RichTextBlock>				TB_TitleName;
	TWeakObjectPtr<UWidgetSwitcher>					WS_ChangeDiffImage;
	TArray<TWeakObjectPtr<UWidgetSwitcher>>			WS_ClearResultStar;
	TArray<TWeakObjectPtr<UB2RichTextBlock>>		TB_ClearResult;

private:
	/** This UI is valid only for a stage, so cache the StageInfoData at the BindDoc time querying it. */
	struct FSingleStageInfoData* CachedCurrentStageInfoData;
	int32				ClientStageId;
	FServerStageID		ServerStageId;
	EStageDifficulty	 StageDifficulty;

	/* The currently active HeroDocument. */
	class UB2UIDocHero* MainHeroDoc;

	/* This document is a Hero waiting for a tag. */
	class UB2UIDocHero* SubHeroDoc;

	/* This value use for check to disable buttons for waiting to enter stage game */
	bool									StageGameStarted;
	void MarkStageGameStarted();
	// We need to reset StageGameStarted for some case that server does not allow enter the stage. Just a workaround
	FTimerHandle							StageGameStartedResetTH;
	void StageGameStartedResetCB();

	uint32									CancelStageGameStartTicket;
};