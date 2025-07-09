// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIRaidMvpView.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIRaidMvpView : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	UB2UIRaidMvpView(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void AddPlayer(AActor* TargetActor, class ABladeIIPlayer* pPlayer, bool bMvp, int32 InContribution);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION()
	void OnClickBTN_Next();

	void SetResultHudInfo();

protected:

	TWeakObjectPtr<UTextBlock> RTB_RaidName;
	TWeakObjectPtr<UB2RichTextBlock> RTB_ClearDifficulty;
	TWeakObjectPtr<UTextBlock> RTB_PlayTime;
	TWeakObjectPtr<UTextBlock> STB_GotoResult;

	TWeakObjectPtr<UB2Button> BTN_Next;	

	TWeakObjectPtr<UImage> IMG_Boss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<UTexture2D*>	BossImages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
	TSubclassOf<class UB2UIControlMvpViewPlayerInfo> MvpViewItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position")
	FVector2D AdjustPosition;

	TArray<class UB2UIControlMvpViewPlayerInfo*> m_arPlayerInfoUI;
	
	bool m_bSendResult;

	uint32 DeliveryAssaultResultTicket;
};
