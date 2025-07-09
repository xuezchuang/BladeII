// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/MsgPopup/B2UIMsgPopupBase.h"
#include "B2UIMsgPopupSimple.h"
#include "B2UIMsgPopupNeedClearStage.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIMsgPopupNeedClearStage : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()
	
protected:
	virtual void CacheAssets() override;

protected:
	TWeakObjectPtr<UB2Button> BTN_GoStageSelect;
	TWeakObjectPtr<UB2Button> BTN_Close;

public:
	UFUNCTION()
		void OnGoStageSelectBtnClick();

	UFUNCTION()
		void OnCloseBtnClick();
	
	
};
