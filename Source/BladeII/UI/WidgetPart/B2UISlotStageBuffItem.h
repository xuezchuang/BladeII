// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UISlotIcon.h"
#include "CommonStruct.h"
#include "B2UISlotStageBuffItem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSlotStageBuffItemClickDelgate, UB2UISlotStageBuffItem*, ClickedItem);

/*
* In the form of slots and indicates stage buff items shown at BattleStageInfo UI.
*/
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISlotStageBuffItem : public UB2UISlotIcon
{
	GENERATED_BODY()

	EStageBuffType MyBuffType;
	FServerStageID ServerStageID;

public:
	// UWidget interface
	virtual void SynchronizeProperties() override;

	UB2UISlotStageBuffItem(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

	void SetBuffSlotInfo(FServerStageID InServerStageNum, EStageBuffType InType);
	FORCEINLINE EStageBuffType GetBuffType() { return MyBuffType; }

	//select
	void SetSelected(bool InbSelected);
	FORCEINLINE bool GetSelected() const { return bSelected; }

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void SetSelected_BP(bool InSelected);

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	
	void UpdateBuffIconDisplay(int32 GoldCost, bool bHasTicket);

	//=================================================== Btn
	//UFUNCTION()
	virtual void OnClickBtnArea() override;

	//==================================================== Doc Delegate
	

public:

	FOnSlotStageBuffItemClickDelgate OnSlotStageBuffItemClickDelgate;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UMaterialInterface* MainIconImageMat_UnSelected;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UMaterialInterface* MainIconImageMat_Selected;

	TWeakObjectPtr<UTextBlock> TB_ItemName;
	TWeakObjectPtr<UImage> IMG_MainIcon;
	TWeakObjectPtr<UImage> IMG_CostTicketIcon;
	TWeakObjectPtr<UImage> IMG_CostGoldIcon;

	TWeakObjectPtr<UImage> IMG_Selected_BoostGameSpeed;
	TWeakObjectPtr<UImage> IMG_Selected_IncExp;
	TWeakObjectPtr<UImage> IMG_Selected_IncGold;
	TWeakObjectPtr<UImage> IMG_Selected_AutoSkill;

	TWeakObjectPtr<UTextBlock> TB_Cost;

	bool bSelected;
};
