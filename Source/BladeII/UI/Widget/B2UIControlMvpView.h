// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIControlMvpView.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIControlMvpView : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	UB2UIControlMvpView(const FObjectInitializer& ObjectInitializer);

	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void AddPlayer(AActor* TargetActor, class ABladeIIPlayer* pPlayer, bool bMvp);
	
	void SetVisibilityPlayerUI(ESlateVisibility NewVisibility);

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationEvent_AnimWinRed();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationEvent_AnimWinBlue();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationEvent_AnimDraw();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION()
		void OnClickBTN_Next();

protected:
	TWeakObjectPtr<UB2Button> BTN_Next;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
		TSubclassOf<class UB2UIControlMvpViewPlayerInfo> MvpViewItemClass;
	
	TArray<class UB2UIControlMvpViewPlayerInfo*> m_arPlayerInfoUI;

	TArray<class ABladeIIPlayer*> m_ArMvpPlayers;
	
	bool m_bSendResult;

	uint32 DeliveryAssaultResultTicket;
};
