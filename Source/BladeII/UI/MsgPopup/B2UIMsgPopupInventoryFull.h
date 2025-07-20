// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIMsgPopupBase.h"
#include "B2UIBackWidget.h"
#include "B2UIMsgPopupInventoryFull.generated.h"


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupInventoryFull : public UB2UIMsgPopupBase
{
	GENERATED_BODY()
	
public:
	UB2UIMsgPopupInventoryFull(const FObjectInitializer& ObjectInitializer);

	void SetInventoryItemCount(int32 CurrentItemCount, int32 MaxItemCount);
	void SetPCClass(const EPCClass PCClass);
	void OverrideMoveInventoryLogic(const FMsgPopupOnClick & OnBtnPosClick);
	void ShowExtensionBtn(bool Show);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	void UpdateWidget();

	virtual void CloseWidgetDelegate() override;

	UFUNCTION()
	void OnClickBtnExtensionInven();
	UFUNCTION()
	void OnClickBtnMoveInven();
	UFUNCTION()
	void OnClickBtnClose();

protected:

	TWeakObjectPtr<UButton>				BTN_ExtensionInven;
	TWeakObjectPtr<UButton>				BTN_MoveInven;
	TWeakObjectPtr<UButton>				BTN_Close;

	TWeakObjectPtr<UTextBlock>			TB_ExtensionInven;
	TWeakObjectPtr<UTextBlock>			TB_MoveInven;

	TWeakObjectPtr<UTextBlock>			TB_StoredItemCount;
	TWeakObjectPtr<UTextBlock>			TB_ContainerItemCount;

private:
	EPCClass					CurrentPCClass;
	FMsgPopupOnClick			OverrideClickMoveInven;
};
