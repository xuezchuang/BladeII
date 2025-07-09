// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIDocBindable.h"
#include "B2UIDocBattle.h"
#include "BladeIIUtil.h"
#include "B2AssetLoader.h"
#include "B2UIBattleQTE.generated.h"

/*
* This represents each of the object skill. In battle.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIBattleQTE : public UB2UIWidgetBase, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	UB2UIBattleQTE(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
private:
	bool bEventsSubscribed;
	void SubscribeEvents();
	void UnsubscribeEvents();

	uint32 StartPauseMenuTicket;
	uint32 StopPauseMenuTicket;

	uint32 StartVisibleChatPopupTicket;
	uint32 StopVisibleChatPopupTicket;

	void OnPause(bool bPaused);
	uint32 bHiddenByPause : 1;
	uint32 bHiddenByBattleMain : 1;
	uint32 bHiddenByTutorialBattleMain : 1;

public:
	void UpdateByQTEEnableState(EQTEUIAttackEnableTimingState InEnableTimingState);

	void SetVisibilityFromBattleMainUI(ESlateVisibility InVisibility);
	void SetVisibilityFromTutorialBattleMainUI(ESlateVisibility InVisibility);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void PlayEnableAnim(EQTEUIAttackEnableTimingState InEnableTimingState);

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void PlayPressedAnim();
		
	/** Intended to be called before calling other Play**Anim */
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void TurnOffLoopAnims();

	virtual void BindDelegates();

public:
	void BindDoc();
	virtual void UnbindDoc() override;

	//=================================================== Btn
	UFUNCTION()
	void OnPressedBtnArea();

private:
	virtual void CacheAssets() override;

	void UpdateOverallVisibility();
	
	UFUNCTION()
	void OnChangedRemainingQTEMountTime(class UB2UIDocBase* Sender, float CurrRemainingTime, float PrevRemainingTime);

	void UpdateMountTimeProgress(float InRemainingTime);

protected:
	TWeakObjectPtr<UB2Button>			BTN_Area;
	TWeakObjectPtr<UB2Button>			BTN_Area2;
	
	// The same kind to the skill cooltime material
	UPROPERTY(Transient)
	class UMaterialInstanceDynamic* MountTimeMID;
	// The image widget Where the MountTimeMID comes from.
	TWeakObjectPtr<UImage> IMG_MountTimeProgress;
	TWeakObjectPtr<UTextBlock> TB_MountTimeNum;

	// Cached specific anim playing state.
	bool bPressedAnimPlaying;
	
	UFUNCTION(BlueprintCallable, Category = BladeII)
	void SetIsPressedAnimPlaying(bool bPlaying);
	UFUNCTION(BlueprintCallable, Category = BladeII)
	bool IsPressedAnimPlaying() const;
	
	EQTEUIAttackEnableTimingState CachedEnableTimingState;

	UFUNCTION(BlueprintCallable, Category = BladeII)
	EQTEUIAttackEnableTimingState GetCachedEnableTimingState() const;
	
	bool bIsQTEMounted;
	float MaxQTEMountTime; // Being cached at the moment of QTE mount start.
public:
	void SetQTEMounted(bool bMounted, float InMaxMountTime, bool bVisibleTimeNum);
protected:
	UFUNCTION(BlueprintCallable, Category = BladeII)
	bool GetIsQTEMounted() const;
};

FORCEINLINE bool UB2UIBattleQTE::IsPressedAnimPlaying() const { return bPressedAnimPlaying; }
FORCEINLINE EQTEUIAttackEnableTimingState UB2UIBattleQTE::GetCachedEnableTimingState() const { return CachedEnableTimingState; }
FORCEINLINE bool UB2UIBattleQTE::GetIsQTEMounted() const { return bIsQTEMounted; }


/** Common util to use UB2UIBattleQTE as intended. */
template< class OwnerWidgetClass >
UB2UIBattleQTE* LoadAndBeginTAssetQTEUIP(OwnerWidgetClass* InOwnerWidget, TSoftClassPtr<UB2UIBattleQTE>& InUIPTAssetToLoad, TSubclassOf<UB2UIBattleQTE>* OutLoadedPtrCache,
	int32 ViewportZOrder = BII_WIDGET_ZORDER_SEPARATE_ABOVE_UIMAIN)
{
	UB2UIBattleQTE* CreatedQTEUIP = NULL;
	if (InOwnerWidget && OutLoadedPtrCache)
	{
		GetSomeLazyLoadedClassAssetCommon<UB2UIBattleQTE>(UB2AssetLoader::GetStreamableManager(), InUIPTAssetToLoad, OutLoadedPtrCache);

		if (OutLoadedPtrCache)
		{
			CreatedQTEUIP = CreateWidget<UB2UIBattleQTE>(InOwnerWidget->GetOwningPlayer(), *OutLoadedPtrCache);
			if (CreatedQTEUIP)
			{
				// It won't be hidden if all other UIs are hidden by cheat, cinematic or anything else.. because it is not a child of other managed UI widget. 
				// That is required for QTE UI.
				CreatedQTEUIP->AddToViewport(ViewportZOrder);
				CreatedQTEUIP->Init();
				//CreatedQTEUIP->BindDoc(); Don't call it here, to call it explicitly at using side.

				CreatedQTEUIP->SetVisibility(ESlateVisibility::Collapsed); // Will be visible afterward, by using side.
			}
		}
	}
	return CreatedQTEUIP;
}