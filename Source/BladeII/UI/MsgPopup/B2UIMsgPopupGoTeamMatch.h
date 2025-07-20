// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/MsgPopup/B2UIMsgPopupSimple.h"
#include "B2UIMsgPopupGoTeamMatch.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIMsgPopupGoTeamMatch : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;

protected:
	TWeakObjectPtr<UB2Button> BTN_GoTeamMatch;
	TWeakObjectPtr<UB2Button> BTN_Close;
	
public:
	UFUNCTION()
	void OnGoTeamMatchBtnClick();

	UFUNCTION()
	void OnCloseBtnClick();	
};
