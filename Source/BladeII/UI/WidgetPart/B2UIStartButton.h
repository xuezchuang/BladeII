// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIStartButton.generated.h"


DECLARE_DELEGATE(FStartButtonDelegate)
DECLARE_DELEGATE(FStartCheckDelegate)
DECLARE_DELEGATE(FClickDirectDelegate)

UCLASS()
class BLADEII_API UB2UIStartButton : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void OpenSetting();
	void SetStartButtonDelegate(FStartButtonDelegate Delegate);
	void SetGoodsNumber(int32 Number,bool bUsedYellowColor=false);
	void SetSecondGoodsNumber(int32 Number, bool bUsedYellowColor = false);
	void StartCloseAni();
	void SetButtonEnabled(bool bInIsEnabled);
	void SetStartButtonText(FText ButtonName);

	void HideAllIcon();

	FORCEINLINE void SetClickState() { ClickState = false; }
	
	template<typename FunctorType>
	inline void StartButtonDelegateBindLambda(FunctorType&& InFunctor)
	{
		ButtonDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}

	template<typename FunctorType>
	inline void StartButtonCheckDelegateBindLambda(FunctorType&& InFunctor)
	{
		StartCheckDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}

	template<typename FunctorType>
	inline void ClickDirectDelegateBindLambda(FunctorType&& InFunctor)
	{
		ClickDirectDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void OpenAnimation();
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void CloseAnimation();

	UFUNCTION(BlueprintCallable, Category = BladeII)
		void StartGame();

	UFUNCTION(BlueprintCallable, Category = BladeII)
		void SetStartAnimation(UWidgetAnimation* Inani);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		UMaterialInstance*				GoodsIMG;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		UMaterialInstance*				GoodsIMG_Second;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
		bool GoodsHide;

protected:
	UFUNCTION()
	void OnClickBTN_Start();

private:
	TWeakObjectPtr<UTextBlock> TB_BattleStart;
	TWeakObjectPtr<UTextBlock> TB_BattleStart_D;
	TWeakObjectPtr<UTextBlock> TB_GoodsNumber;
	TWeakObjectPtr<UTextBlock> TB_GoodsNumber_D;
	TWeakObjectPtr<UTextBlock> TB_GoodsNumber_Second;
	TWeakObjectPtr<UTextBlock> TB_GoodsNumber_D_Second;
	TWeakObjectPtr<UImage> IMG_Icon;
	TWeakObjectPtr<UImage> IMG_Icon_D;
	TWeakObjectPtr<UImage> IMG_Icon_Second;
	TWeakObjectPtr<UImage> IMG_Icon_D_Second;
	TWeakObjectPtr<UB2Button> BTN_Start;
	TWeakObjectPtr<UWidgetSwitcher> ButtonSwitcher;


	UWidgetAnimation*		StartAnimation;

	FStartButtonDelegate	ButtonDelegate;
	FStartCheckDelegate		StartCheckDelegate;
	FClickDirectDelegate	ClickDirectDelegate;

	bool ClickState;
	FTimerHandle StartGameTimemer;
};
