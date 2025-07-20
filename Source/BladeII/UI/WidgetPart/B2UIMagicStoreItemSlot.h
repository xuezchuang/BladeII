// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIMagicStoreItemSlot.generated.h"


DECLARE_DELEGATE(FBuyButtonDelegate)

UCLASS()
class BLADEII_API UB2UIMagicStoreItemSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

public:
	void SetData(int32 ProductID, bool IsPurchased);
	void SetVisibilityItem(ESlateVisibility bVislble);
	void StartAnimation(int32 SlotIndex);
	void PlaySlotIndexAnimation();

	template<typename FunctorType>
	inline void BuyButtonDelegateBindLambda(FunctorType&& InFunctor)
	{
		ButtonDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}

	void UnbindDelegate();

private:
	void SetItemIconAndName(int32 ItemRefID);

protected:
	UFUNCTION()
		void OnClickBTN_Buy();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Store Item")
		float AniStartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic Store Item")
		float AniSpeed;

private:
	TWeakObjectPtr<UTextBlock> TB_ItemName;
	TWeakObjectPtr<UImage> IMG_ItemIcon;
	TWeakObjectPtr<UImage> IMG_BuyItemIcon;
	TWeakObjectPtr<UTextBlock> TB_ItemCount;
	TWeakObjectPtr<UWidgetSwitcher> WS_IsPurchased;

	TWeakObjectPtr<class UB2UIStoreProductCost> UIP_Cost;
	TWeakObjectPtr<UTextBlock> TB_BuyComplete;

	TWeakObjectPtr<UB2Button> BTN_BuyItem;
	TWeakObjectPtr<UB2Button> BTN_Buy;

	TWeakObjectPtr<UWidgetAnimation> Anim_Change;

private:
	FBuyButtonDelegate ButtonDelegate;

	FTimerHandle AniTimeHandle;

};
