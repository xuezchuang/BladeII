// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Widget/B2UIBattleMain.h"
#include "B2UITutorialBattleMain.generated.h"

/**
*
*/

UENUM(BlueprintType)
enum class ETutorialWidgetState : uint8
{
	Lock,
	Enable,
	Suggest,

	Max
};


struct FB2CachedWidgetStatus
{
	UWidget*			WidgetPtr;
	ESlateVisibility	OrgVisibility;

	FB2CachedWidgetStatus(UWidget* InWidgetPtr) : WidgetPtr(InWidgetPtr)
	{
		if (WidgetPtr) OrgVisibility = WidgetPtr->GetVisibility();
	}
};

struct FB2WidgetGroup
{
	FString							GroupName;
	TArray<FB2CachedWidgetStatus>	WidgetList;
};

UCLASS()
class BLADEII_API UB2UITutorialBattleMain : public UB2UIBattleMain
{
	GENERATED_BODY()

	virtual void Init() override;
	virtual void CacheAssets() override;
	virtual void DestroySelf(UB2UIManager* InUIManager) override;
	virtual void BindDelegates() override;

	virtual void BindDocAuto() override;

	virtual void RequestAsyncLoadWidgets() override;

public:

	virtual void TagCommon(EPCClass MainClass, EPCClass SubClass) override;
	virtual void SetLevelMainChar(int32 InValue) override;

	void HideTutorialWidgetGroup(const FString& WidgetGroupName);
	void RestoreTutorialWidgetGroup(const FString& WidgetGroupName);

	void SetGuardWidgetState(ETutorialWidgetState WidgetState);
	void SetQTEWidgetState(ETutorialWidgetState WidgetState);
	void SetTagWidgetState(ETutorialWidgetState WidgetState);
	void SetMoveWidgetState(ETutorialWidgetState WidgetState);
	void SetAttackWidgetState(ETutorialWidgetState WidgetState);
	void SetMountAttackWidgetState(ETutorialWidgetState WidgetState);
	void SetSkillWidgetSuggest(int32 SkillIndex, bool bSuggest);
	void SetSkillWidgetClickable(bool bClickable, bool bSkipWeapon = true);
	void SetUnitySkillWidgetClickable(bool bUnityClickable, bool bOtherClickable);

	void SetUIEnable(bool bEnable);

	void SubscribeEvents();
	void UnsubscribeEvents();

	void OnOpenScenarioDialog(FName OpenDialogName);
	void OnCloseScenarioDialog(FName CloseDialogName);

	UFUNCTION(BlueprintImplementableEvent)
		void EventGuardSuggestChanged(bool bVisible);

	UFUNCTION(BlueprintImplementableEvent)
		void EventAttackSuggestChanged(bool bVisible);

	UFUNCTION(BlueprintImplementableEvent)
		void EventMountAttackSuggestChanged(bool bVisible);

	UFUNCTION(BlueprintImplementableEvent)
		void EventMoveSuggestChanged(bool bVisible);

	UFUNCTION(BlueprintImplementableEvent)
		void EventSkillSuggestChanged(int32 SkillIndex, bool bVisible);

	UFUNCTION(BlueprintImplementableEvent)
		void EventQTESuggestChanged(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void SetVisibilityWidgetGroup(const FString& InGroupName, ESlateVisibility GroupVisibility);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void RestoreVisibilityWidgetGroup(const FString& InGroupName);

	//UFUNCTION()
		void OnChangedQTEEnableState(class UB2UIDocBase* Sender, EQTEUIAttackEnableTimingState CurrQTEState, EQTEUIAttackEnableTimingState PrevQTEState);

	UFUNCTION()
		void OnClickBtnSkip();

protected:

	virtual void HideHUDForQTE(bool bHide) override;

	virtual void CloseWidgetDelegate() override;
	virtual void CloseBattleMainUIEvent(bool Valuse) override;

	TArray<FB2WidgetGroup>	WidgetGroupList;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void AddWidgetToWidgetGroup(const FString& InGroupName, const TArray<UWidget*>& GroupWidgetList);

	FB2WidgetGroup& GetOrAddWidgetGroupRef(const FString& InGroupName);

private:

	void SetVisibilitySkipButton(bool bVisibility);

private:

	TWeakObjectPtr<UTextBlock>		TB_StageName;
	TWeakObjectPtr<UB2Button>		BTN_Skip;

	TWeakObjectPtr<UImage>			IMG_GuardLock;

	TWeakObjectPtr<UCanvasPanel>	CP_GuardSuggest;
	TWeakObjectPtr<UCanvasPanel>	CP_QTESuggest;
	TWeakObjectPtr<UCanvasPanel>	CP_AttackSuggest;
	TWeakObjectPtr<UCanvasPanel>	CP_MountAttackSuggest;
	TWeakObjectPtr<UCanvasPanel>	CP_SuggestPortraitSub;
	TWeakObjectPtr<UCanvasPanel>	CP_MoveSuggest;

	TWeakObjectPtr<UCanvasPanel>	CP_SuggestSkill0;
	TWeakObjectPtr<UCanvasPanel>	CP_SuggestSkill1;
	TWeakObjectPtr<UCanvasPanel>	CP_SuggestSkill2;
	TWeakObjectPtr<UCanvasPanel>	CP_SuggestSkill3;
	TArray<TWeakObjectPtr<UCanvasPanel>>	CP_SuggestSkills;

private:

	uint32 OpenDialogTicket;
	uint32 CloseDialogTicket;

	bool bAutoMinoRiddingSignal;

};
