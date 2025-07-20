// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidgetBase.h"
#include "CommonStruct.h"
#include "B2UIFilterCheckBox.generated.h"

DECLARE_DELEGATE(FCheckButtonDelegate)

class UB2UIFilterCheckBox;
DECLARE_DELEGATE_OneParam(OnPreSelectFilter, UB2UIFilterCheckBox*)

/**
*
*/
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIFilterCheckBox : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIFilterCheckBox(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

	void SetIsChecked(bool InIsChecked, bool bBroadcast = true);
	bool IsChecked() const;
	void SetLabel(FText LabelName);

	void SetPreFilterCheck(OnPreSelectFilter InOnPreSelectFilter);

// 	UFUNCTION(BlueprintCallable, Category = "BladeII")
// 		CHECKBOXSTATES GetCheckBox() { return CB_Select; }

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void OnChangeState(bool State);

	UFUNCTION()
		void OnClickPreSelect();

public:
	FOnCheckBoxComponentStateChanged OnCheckStateChanged;

private:
	TWeakObjectPtr<UCheckBox>	CB_Select;
	TWeakObjectPtr<UTextBlock>	TB_SelectLabel;
	TWeakObjectPtr<UTextBlock>	TB_UnSelectLabel;
	TWeakObjectPtr<UB2Button>	BTN_PreSelect;

	OnPreSelectFilter PreSelectFilter;

public:
	template<typename FunctorType>
	inline void CheckButtonDelegateBindLambda(FunctorType&& InFunctor)
	{
		ButtonDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}

private:
	FCheckButtonDelegate	ButtonDelegate;

};
