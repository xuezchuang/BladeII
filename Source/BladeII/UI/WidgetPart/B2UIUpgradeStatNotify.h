// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
//#include "BladeII.h"
#include "B2UIWidget.h"
#include "B2UIUpgradeStatNotify.generated.h"


UCLASS()
class BLADEII_API UB2UIUpgradeStatNotify : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void AddListItem(FText LableText, FText ValueText, bool bIsUP);
	void OnDelayViewTimerHandle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatValueNotify)
	TSubclassOf<class UB2UIStatValueSlot> StaValueSlotClass;
	TWeakObjectPtr<UTextBlock> STB_Title;
	FTimerHandle DelayViewTimerHandle;
	float fDelayTime = 1.0f;

	void ClearList();
	void SetTitleText(FText _Text);
protected:

private:
	TWeakObjectPtr<UVerticalBox> List_Stat;
};
