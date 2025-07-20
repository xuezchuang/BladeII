// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIRepeatBattleInGameInfo.h"
#include "B2UIBackWidget.h"
#include "B2UIStageBegin.generated.h"

/**
* A widget to be displayed at stage begin event.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIStageBegin : public UB2UIWidget, public IB2UIDocBindable, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	/*void SetStageNum(int32 InValue);
	void SetAreaNameLocalized(const FText& InText);
	void SetAreaNameSub(const FText& InText);
	void SetHeroTowerInfo();*/
	
private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	//====================================================Doc Delegate
	//There is no change data, but in BindDocAuto Get.

protected:
	/*TWeakObjectPtr<UTextBlock>			TB_StageNum;
	TWeakObjectPtr<UTextBlock>			TB_TextStage;
	TWeakObjectPtr<UTextBlock>			TB_AreaNameLocalized;
	TWeakObjectPtr<UTextBlock>			TB_AreaNameSub;
	TWeakObjectPtr<UTextBlock>			TB_CounterDGStageName;

	TWeakObjectPtr<UOverlay>			OV_CounterDG;*/
	virtual void CloseWidgetDelegate() override;
	
	TWeakObjectPtr<UB2UIRepeatBattleInGameInfo> UIP_RepeatBattleInGameInfoP;
};
