// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2LobbyUI_BrevetMain.h"
#include "B2UIBrevetAddStatus.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIBrevetAddStatus : public UB2UIWidget
{
	GENERATED_BODY()

public:
	void InitBrevetAddStatus(UB2LobbyUI_BrevetMain* pOwnerUI, const FBrevetNodeInfo* BrevetNodeInfo, UMaterialInterface* pGaugeBackMI, UMaterialInterface* pGaugeMI, UMaterialInterface* pIconMI, EPCClass OwnerClass);

protected:
	virtual void CacheAssets() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_BrevetAddStatusInitAnimation();

	UFUNCTION(BlueprintCallable, Category = "B2 Brevet")
	void OnFinishAnimationEvent_BrevetAddStatusInitAnimation();

protected:
	UB2LobbyUI_BrevetMain* m_pOwnerUI;

	TWeakObjectPtr<UImage> IMG_GaugeBg;
	TWeakObjectPtr<UImage> IMG_Gauge;
	TWeakObjectPtr<UImage> IMG_Icon;

	TWeakObjectPtr<UTextBlock> TB_StatusType;
	TWeakObjectPtr<UTextBlock> TB_StatusValue;

	EPCClass CurrentOwnerClass = EPCClass::EPC_End;
};

