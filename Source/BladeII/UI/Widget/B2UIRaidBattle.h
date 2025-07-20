// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidget.h"
#include "B2UIBattleMain.h"
#include "B2UITeamMatchBattleSlot.h"
#include "B2UIRaidBattle.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIRaidBattle : public UB2UIBattleMain
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	void SetWidgetForRaidBattle();
	void SetEntry(int32 SlotIdx, EPCClass CharacterClass, int32 Lv);
	void SetContributionValue(int32 SlotIdx, int32 ContributionValue);		// 扁咯档 蔼 汲沥

	void OnSlotChangedHealthAndShieldCommon(bool bMyTeam, int32 SlotIdx);
	void OnSlotChangedOnClickCharacter(bool bMyTeam, int32 SlotIdx);

	void InitEntryData();
	void SetDisableSlot(int32 SlotIdx);

	void OnVisibleBtnForDefeat(class UB2UIDocBase* Sender, bool CurVisible, bool PrevVisible);
	void SetResurrectHelperNotice(FText userName);

	void ShowResultImage(const ENetMatchResult& result);
public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_AnimResultAction(ENetMatchResult InMatchResult);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowResurrectHelperNotice_IMP();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationNotEnoughTime_IMP();

	UFUNCTION()
	void OnChangedRaidBattleTime(class UB2UIDocBase* Sender, int32 CurrentTime, int32 PrevTime);

	UFUNCTION()
	void OnChangedMainPCContribution(class UB2UIDocBase* Sender, int32 CurContribution, int32 PrevContribution);

	UFUNCTION()
		void OnChangedCharName(class UB2UIDocBase* Sender, FText CurrentName, FText PrevName);

	UFUNCTION()
	void OnPressedRaidPauseBtn();

protected:
	TArray<TWeakObjectPtr<class UB2UITeamMatchBattleSlot>>		TeamCharSlots;
	TWeakObjectPtr<UTextBlock>									TB_BattleTime;
	TWeakObjectPtr<UTextBlock>									TB_MainContribution;
	TWeakObjectPtr<UTextBlock>									TB_ResurrectHelper;
	TWeakObjectPtr<UTextBlock>									TB_RaidStep;

	TWeakObjectPtr<UPanelWidget>								CP_SkillSet0;
	TWeakObjectPtr<UPanelWidget>								CP_SkillSet1;
	TWeakObjectPtr<UPanelWidget>								CP_SkillSet2;
	TWeakObjectPtr<UPanelWidget>								CP_SkillSet3;
	TWeakObjectPtr<UPanelWidget>								CP_Attack;
	TWeakObjectPtr<UPanelWidget>								CP_Guard;

	TWeakObjectPtr<UImage>									IMG_SkillSetBG;
	TWeakObjectPtr<UImage>									IMG_TouchRing_AttackBtn;
	TWeakObjectPtr<UImage>									IMG_TouchRing_GuardBtn;
};