// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "B2UIEtherCharacterSlot.generated.h"

/**
 *
 */

DECLARE_DELEGATE_OneParam(FOnEtherTouchDelegate, const int64);
DECLARE_DELEGATE(FOnEtherEquipAnimElegate);

struct FEtherSlot
{
	FEtherSlot() : IsOn(false) {}
	TWeakObjectPtr<UImage>										IMG_BG;
	TWeakObjectPtr<UImage>										IMG_SetImage;
	TWeakObjectPtr<UImage>										IMG_Defence;
	TWeakObjectPtr<UImage>										IMG_Offense;
	TWeakObjectPtr<UB2Button>									BTN_Slot;
	TWeakObjectPtr<UB2RichTextBlock>							RTB_Enhance;
	TWeakObjectPtr<UB2RichTextBlock>							RTB_Star;
	TWeakObjectPtr<UPanelWidget>								PW_SlotInfo;
	int64														EtherID;
	int32														EtherType;
	int32														EtherTier;
	FOnEtherEquipAnimElegate									OnPlayAnim;
	bool														IsOn;

	void OnVisible();
	void OffVisible();
	void SetSetImage(int32 InType, int32 InTier);
	void SetTextInfo(int32 InStar, int32 InEnhanceLevel);
	void SetSetEffect(int32 SetState);
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIEtherCharacterSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayEquipAnimONE_BP();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayEquipAnimTWO_BP();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayEquipAnimTHREE_BP();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayEquipAnimFOUR_BP();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayEquipAnimFIVE_BP();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayEquipAnimSIX_BP();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayActiveOffense_BP();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayKeepOffense_BP();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayActiveDefense_BP();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayKeepDefense_BP();
	UFUNCTION(BlueprintCallable, Category = BladeII)
	void EndCombineAnim_BP();

	UFUNCTION(BlueprintCallable, Category = "BladeII B2UITutorial")
	void EquipAnimOneEnd_BP();

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void SetInfo(class UB2UIEther* InReceiver, EPCClass InPCClass, bool IsEquip);

	UPROPERTY(EditAnywhere)
	TMap<int32, float> ConvertComBineMap;
	UPROPERTY(EditAnywhere)
	TMap<int32, float> ConvertOffenseMap;
	UPROPERTY(EditAnywhere)
	TMap<int32, float> ConvertDefenseMap;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	UFUNCTION()
	void OnClickOneSlot();
	UFUNCTION()
	void OnClickTwoSlot();
	UFUNCTION()
	void OnClickThreeSlot();
	UFUNCTION()
	void OnClickFourSlot();
	UFUNCTION()
	void OnClickFiveSlot();
	UFUNCTION()
	void OnClickSixSlot();

	void SetEtherSetEffect(bool IsEquip);
	void SetEtherSetEffect(EPCClass InPCClass, int32 InDefenceSetID, int32 InOffenseSetID, bool IsEquip);
	void EquipEtherAnimation(int32 InPart);

	const int MAXOFFENSECOUNT = 4;
	const int MAXDEFENSECOUNT = 2;

	FOnEtherTouchDelegate														OnEtherSlotTouch;
	TMap<int32, FEtherSlot>														EtherSlots;
	uint32																		SelectEtherDefenseOptionEventID;
	EPCClass																	PCClass;
	TWeakObjectPtr<UPanelWidget>												X_PW_Combine;
	TArray<TWeakObjectPtr<UPanelWidget>>										ActiveOffense;
	TArray<TWeakObjectPtr<UPanelWidget>>										KeepOffense;
	TArray<TWeakObjectPtr<UPanelWidget>>										ActiveDefense;
	TArray<TWeakObjectPtr<UPanelWidget>>										keepDefense;
	int32																		DefenceSetID;
	int32																		OffenseSetID;
};
