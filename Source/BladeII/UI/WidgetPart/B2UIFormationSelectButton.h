// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIFormationSelectButtonSlot.h"
#include "../B2RichTextBlock.h"
#include "B2UIFormationSelectButton.generated.h"


UCLASS()
class BLADEII_API UB2UIFormationSelectButton : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetFormationSetting(ETMFormation Formation, int32 Lv, int32 Param1, int32 Param2, FFormationSlotStateDelegate Delegate);
	void SetOnSlotState();
	void SetOffSlotState();

	UFUNCTION(BlueprintImplementableEvent)
	void OnClickState();

	UFUNCTION(BlueprintImplementableEvent)
	void OffClickState();

private:
	TWeakObjectPtr<class UB2UIFormationSelectButtonSlot> UIP_FormationSelectButton;
	TWeakObjectPtr<UB2RichTextBlock> RTB_Ability1;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<UMaterialInterface*>		FormationOnIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TArray<UMaterialInterface*>		FormationOffIcon;
};
