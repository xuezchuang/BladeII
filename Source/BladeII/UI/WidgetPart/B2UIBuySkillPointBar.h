#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIBuySkillPointBar.generated.h"

/*
* ProgressBar the use of the original version is more easily made.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIBuySkillPointBar : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	void SetMaxNum(int32 InMaxNum);
	
	void SetNum(int32 InNum);

	void SetCharacterName(const FText& Name);

	FORCEINLINE int32 GetNum() { return CurrentNum; }

	UFUNCTION()
	void SetSlider(float InValue);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void	OnClickBtnDecrease();

	UFUNCTION()
	void	OnClickBtnIncrease();

	UFUNCTION()
	void	OnClickBtnSetMax();

	void UpdateAmountPerMaxText();
private:
	TWeakObjectPtr<UTextBlock> TB_CharacterName;
	TWeakObjectPtr<UButton> BTN_Decrease;
	TWeakObjectPtr<UButton> BTN_Increase;
	TWeakObjectPtr<UButton> BTN_SetMax;
	TWeakObjectPtr<USlider> S_SkillBuySlider;

	int32 CurrentNum;
	int32 MaxNum;
};