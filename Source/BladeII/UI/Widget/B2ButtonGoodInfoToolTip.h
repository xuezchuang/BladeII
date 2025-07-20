// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UI/Widget/B2Button.h"
//#include "BladeII.h"

#include "B2ButtonGoodInfoToolTip.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2ButtonGoodInfoToolTip : public UB2Button
{
	GENERATED_BODY()

//Variable
private:
protected:
	EGoodsInfoPopupType GoodsPopupType;
	int32 m_ItemInfoidx;

	UB2UnitedWidgetBase* m_InOwnerUnitedWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Appearance")
	FSlateBrush AuraImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Appearance")
	FSlateBrush OutLineImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII Appearance")
	FSlateBrush ReflectImg;
	

public:

//Function
private:
protected:
	UMaterialInterface* GetIconMaterialFromItemTable(int32 _RewardID) const;

public:
	UB2ButtonGoodInfoToolTip(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void SetItemInfo(int32 _ItemInfoidx);
	UFUNCTION()
	void SetIcon(const FSlateBrush& _NormalSlate, const FSlateBrush& _PressedSlate, const FSlateBrush& _HoveredSlate);
	UFUNCTION()
	void SetDrawAs(const ESlateBrushDrawType::Type& _type);
	UFUNCTION()
	FVector2D GetPopupPosition(UB2UIGoodsShortcutTooltipPopup* const _pUI) const;
	
	//UFUNCTION()
	virtual void RegisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget) override;

	//UFUNCTION()
	virtual void UnregisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget) override;
	//UFUNCTION()
	virtual void ExtraOnClick() override;
	//UFUNCTION()
	virtual void ExtraOnPress() override;
	//UFUNCTION()
	virtual void ExtraOnRelease() override;
	//UFUNCTION()
	virtual void ExtraOnHovered() override;
	
	FORCEINLINE EGoodsInfoPopupType GetGoodsPopupType() const { return GoodsPopupType; };

};
