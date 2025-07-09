// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "UI/B2UIWidgetBase.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2LobbyUI_ItemDetailPostOp.h"
#include "B2UIBackWidget.h"
#include "B2UIDocBindable.h"
#include "B2SealCardFx.h"
#include "CommonStruct.h"
#include "B2UISealBoxResult.generated.h"


const int32 MAX_CARD_FX_PART_NUM = 5; // Max reward number itself might be different.
/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISealBoxResult : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient)
		FCardFXSetupLoadedRefHolder LoadedRewardPSHolder;

	/** Particle system set-up for reward Fx, per item star-grade */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		FCardFXSetup RewardPSSetup;

	/** Main stage of reward item Fx play. Only one will be loaded and instanced by total reward item number. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TArray<TSoftClassPtr<class UB2SealCardFx>> RewardFxPartWidgetClasses;

	/** Created widget part instance, by one among RewardFxPartWidgetClasses */
	UPROPERTY(Transient)
		class UB2SealCardFx* CreatedRewardFxPart;

	TArray<FB2Item> RewardItemData; // Gathered for displaying here.

public:
	UB2UISealBoxResult(const FObjectInitializer& ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif

	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void CloseWidgetDelegate() override;

	void SetRewardDataManually(const FB2ResponseUnsealBoxPtr& UnsealBoxPtr, int32 pcClass);

	UFUNCTION(BlueprintCallable, Category = "BladeII StageClear")
		void BeginRewardItemFx();

	/* 3d effects at this point are to be played. */
	UFUNCTION(BlueprintCallable, Category = "BladeII StageClear")
		void Show3DWingEffect();
	UFUNCTION(BlueprintCallable, Category = "BladeII StageClear")
		void Show3DGemEffect();


private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void GatherRewardItemData();
	bool IsMaterialFromAtlas(int32 ItemRefID);

	/** Override because this StageClearWidget is expected to be used at paused state. */
	virtual void SetupWidgetCustomTimer() override;
	/** Override because this StageClearWidget is expected to be used at paused state. */
	virtual void SetupWidgetCustomHideTimer() override;

	float CachedWidgetStartTime;
	void UpdateWidgetCustomTimer();

	UB2SealCardFx* GetRewardPartForRewardNum(int32 InRewardNum);

	//===================================================Click
	UFUNCTION()
		void OnClickBtnOpenAgain();
	UFUNCTION()
		void OnClickBtnCloseThis();

	//====================================================Doc Delegate

	UFUNCTION()
		void OnChangedRepeatBattleCurrentOn(class UB2UIDocBase* Sender, bool bCurrentOn, bool bPrevOn);

protected:
	TWeakObjectPtr<UCanvasPanel>			CP_OpenAgain;
	TWeakObjectPtr<UButton>					BTN_OpenAgain;

	TWeakObjectPtr<UCanvasPanel>			CP_CloseThis;
	TWeakObjectPtr<UButton>					BTN_CloseThis;

private:
	FB2ResponseUnsealBoxPtr UnsealData;
	int32 savedClass;
};
