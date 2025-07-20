// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIModRewardBoxDetailInfo.generated.h"


UCLASS()
class BLADEII_API UB2UIModRewardBoxDetailInfo : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetRewardCount(int32 Count);
	void SetPanelClear();
	void SetRewardIcon(int32 ItemId, int32 MinCount, int32 MaxCount, int32 PanelIndex);

	virtual void UpdateStaticText() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void GradeEvent(int32 Grade);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void GuildGradeEvent(int32 Grade);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void MVPEvent();

protected:
	UFUNCTION()
		void OnClickBTN_Close();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
		TSubclassOf<class UB2UIRewardIcon> RewardIconClass;
		

private:
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UTextBlock>	TB_RewardCount;
	TWeakObjectPtr<UUniformGridPanel> X_UniformPanel;

	TWeakObjectPtr<UTextBlock> TB_Desc;
};
