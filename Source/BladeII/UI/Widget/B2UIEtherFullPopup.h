// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIEtherFullPopup.generated.h"

DECLARE_DELEGATE(FMoveEther)
/**
 * 
 */
UCLASS()
class UB2UIEtherFullPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	void SetEtherFullPopupInfo(int InStoredCount, int MaxCount);

	template<typename FunctionType>
	void SetMoveEtherDelegate(FunctionType&& InFuctionType)
	{
		MoveEtherDelegate.Unbind();
		MoveEtherDelegate.BindLambda(Forward<FunctionType>(InFuctionType));
	}

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;
	
	UFUNCTION()
		void OnClickBtnExtensionEtherInven();
	UFUNCTION()
		void OnClickBtnMoveEther();
	UFUNCTION()
		void OnClickBtnClose();

private:
	TWeakObjectPtr<UButton> BTN_ExtensionInven;
	TWeakObjectPtr<UButton> BTN_MoveInven;
	TWeakObjectPtr<UButton> BTN_Close;
	TWeakObjectPtr<UTextBlock> TB_ExtensionInven;
	TWeakObjectPtr<UTextBlock> TB_MoveInven;
	TWeakObjectPtr<UTextBlock> TB_StoredItemCount;
	TWeakObjectPtr<UTextBlock> TB_ContainerItemCount;
	TWeakObjectPtr<UTextBlock> TB_Content;
	TWeakObjectPtr<UTextBlock> TB_Title;
	EPCClass CurrentPCClass;


	FMoveEther MoveEtherDelegate;
};
