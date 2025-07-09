// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "RelicManager.h"
#include "B2RelicUIP_EnchantInfo.generated.h"

/*
 * Wing enhancement menu on wing main management page. (Evolution menu can replace this menu at the same place).
 */
DECLARE_DELEGATE_OneParam(FButtonDelegate, FAncientRelic)

UCLASS()
class BLADEII_API UB2RelicUIP_EnchantInfo : public UB2UIWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	FButtonDelegate	ButtonDelegate;
private:
	TWeakObjectPtr<UProgressBar> PB_EnhancePoint;
	TWeakObjectPtr<UTextBlock> TB_EnhancePointProgress;

	TWeakObjectPtr<UTextBlock> TB_RelicName;
	TWeakObjectPtr<UTextBlock> TB_RelicName_Close;
	TWeakObjectPtr<UTextBlock> STB_RelicOption;
	TWeakObjectPtr<UImage> IMG_RelicIcon;
	TWeakObjectPtr<class UB2LobbyUI_RelicStarItem> GradeWidget;
	TWeakObjectPtr<UB2Button> BTN_Relic;
	TWeakObjectPtr<UPanelWidget> P_Disable;
	TWeakObjectPtr<UPanelWidget> P_Able;
	TWeakObjectPtr<UProgressBar> PB_Count;
	TWeakObjectPtr<UTextBlock> TB_Count;

	FAncientRelic CurrentRelicInfo;
	EPCClass CurrentPCClass;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII RelicUI")
	TArray<UMaterialInterface*> RelicIconsArray;

public:
	UFUNCTION()
	void OnClicked_RelicButton();

	void InitRelicOptionData(int RelicID, int32 ClientStageID, EPCClass ePC);
	void UpdateRelicData(FAncientRelic RelicInfo, EPCClass ePC);
	void ClearRelicData();
	bool RedDotCondition_RelicItem();
	FORCEINLINE const int32 GetCurrentRelicIndex() { return CurrentRelicInfo.nRelicId; }
	FORCEINLINE const FAncientRelic GetCurrentRelicInfo() { return CurrentRelicInfo; }
};