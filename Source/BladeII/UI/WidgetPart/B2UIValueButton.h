// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
//#include "BladeII.h"
#include "B2UIWidgetBase.h"
#include "B2UIValueButton.generated.h"

DECLARE_DELEGATE(FValueButtonDelegate)
DECLARE_DELEGATE(FValueCheckDelegate)

UCLASS()
class BLADEII_API UB2UIValueButton : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	//UFUNCTION(BlueprintCallable, Category = "Widget")
	virtual void SetIsEnabled(bool bInIsEnabled) override;

	UFUNCTION()
	void OnClickedButton();

	UFUNCTION(BlueprintCallable, Category = "BladeII UI")
	void OnClicked_ValueButton();


	UFUNCTION(BlueprintCallable, Category = "BladeII UI") // if valueAmount = 0, "TextField and IconImage" will be hidden.
	void SetAmount(FText ValueAmount, bool bIsRed = false);
	UFUNCTION(BlueprintCallable, Category = "BladeII UI")
	void SetButtonName(FText ButtonName);

	void SetButtonDelegate(FValueButtonDelegate Delegate);

	template<typename FunctorType>
	inline void ValueButtonDelegateBindLambda(FunctorType&& InFunctor)
	{
		ButtonDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}

	template<typename FunctorType>
	inline void ValueButtonCheckDelegateBindLambda(FunctorType&& InFunctor)
	{
		ButtonCheckDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII UI")
	FSlateColor NormalColor;
private:
	TWeakObjectPtr<UTextBlock> STB_Enhancebutton;
	TWeakObjectPtr<UImage> IMG_Icon;
	TWeakObjectPtr<UTextBlock> TB_EnhnceValue;
	TWeakObjectPtr<UB2Button> BTN_Enhance;

	FValueButtonDelegate	ButtonDelegate;
	FValueCheckDelegate		ButtonCheckDelegate;
};
