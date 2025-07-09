#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UICounterDGResult.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICounterDGResult : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	UFUNCTION(BlueprintCallable, Category = BladeII)
	void CheckAndOpenLevelUpPart(); // Do this on completion timing.

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void OnOpenComplete() override;


	void SetCounterDungeonLv();

	UFUNCTION()
	void OnClickBtnOK();

	void OnFinishLevelUpFX();

	TWeakObjectPtr<UButton>					BTN_OK;

	TWeakObjectPtr<UTextBlock>				TB_MainRewardName;
	TWeakObjectPtr<UTextBlock>				TB_SendMail;

	TWeakObjectPtr<UTextBlock>				TB_CounterDungeonLv;

	TWeakObjectPtr<UHorizontalBox>			List_SubReward;

	TWeakObjectPtr<class UB2UIRewardIcon>		  UIP_Counter_MainRewardIcon;
	TWeakObjectPtr<class UB2UIStageResultLevelUp> UIP_LevelUp;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UB2UIRewardIcon> SubRewardIconClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UB2UIRewardIcon> MainRewardIconClass;
};