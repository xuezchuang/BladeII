// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIEtherScrollSlot.generated.h"

DECLARE_DELEGATE_OneParam(FOnEtherSlotTouchDelegate, const int64);	

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEtherScrollSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII EtherScrollSlot")
		void PlayClickSoundEther_BP();

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetRowSlotInfo(class UB2UIEther* Class, struct FB2Ether* SlotEther);
	void SetInfo(const FB2Ether* SlotEther);
	void SetInfo(int32 InType, int32 InTier, int32 InStar);
	void UpdateLock(bool Value);
	void UpdateRedDot(bool Value);
	void UpdateEquipUI(bool Value);
	
	int64 GetEtherID() { return ID; }
	
protected:
	virtual void CacheAssets() override;
	virtual void OnClickedSenderBTNArea() override;

private:
	void SetEtherImageInfo(int32 InTier, int32 InType);
	void SetEtherStar(int32 InStar);
	void SetEtherName(int32 InTier, int32 InType);
	void SetEtherEnhaceLevel(int32 InEnhanceLevel);
	void SetPosImage(bool Value, int32 Part);

private:
	int64												ID;
	TWeakObjectPtr<UImage>								X_IMG_SetImage;
	TWeakObjectPtr<UImage>								X_IMG_Lock;
	TWeakObjectPtr<UImage>								X_IMG_PosImage;
	TWeakObjectPtr<UImage>								X_IMG_BG;
	FOnEtherSlotTouchDelegate							OnEtherSlotTouch;
	TWeakObjectPtr<class UB2UIWidgetBase>				UIP_RedDot;
	TWeakObjectPtr<UTextBlock>							X_TB_EtherName;
	TWeakObjectPtr<UTextBlock>							X_TB_EnhaceLevel;
	TWeakObjectPtr<UPanelWidget>						X_PW_EquipUI;
	TWeakObjectPtr<UB2Button>							BTN_Tutorial;
	TWeakObjectPtr<class UB2UIStarGradePart>		UIP_Star;
////////////ethertest
	TWeakObjectPtr<UB2RichTextBlock>					EtherID;
};
