// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UICostumeItemSlot.generated.h"

UCLASS()
class BLADEII_API UB2UICostumeItemSlot : public UB2UIWidget
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	
protected:
	virtual void OnSetupManualScrollBoxSender(class UB2UnitedWidgetBase* InReceiver, class UB2ScrollBox* HandlingScrollBox) override;
	virtual void OnClickedSenderBTNArea() override;

private:
	UFUNCTION()
	void OnClickBTN_CostumeItemSlot();

public:
	void UpdateCostumeItemSlot_FromFB2Item(const FB2Item& InItem);

	void SetOpenItemInfoPopup(bool OpenInfoPopup);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlayClickSound();

private:
	void UpdateIMGItemIcon(class UB2ItemInfo* pAllItemInfo, struct FSingleItemInfoData* pItemInfo);
	void UpdateIMGItemGrade(const uint8& StarGrade);
	void UpdateRTBItemName(struct FSingleItemInfoData* pItemInfo, const uint8& StarGrade);
	void UpdateEquipMark(const FB2Item& InItem);
	void UpdateLockImage(const FB2Item& InItem);
	void UpdateEnhanceText(const FB2Item& InItem);
	void UpdateItemCountText(const FB2Item& InItem);
	void UpdateRedDot(const FB2Item& InItem);

private:
	bool IsNotNeedUpdateCostumeItemSlot(const FB2Item& InItem);

private:
	void OpenCostumeItemInfoPopup(const FB2Item& InItem, const bool& IsLeftPopup = false);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OpenItemInfoPopup;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* DefaultIconBGMtrl;

private:
	TWeakObjectPtr<UB2Image>			IMG_ItemIcon;
	TWeakObjectPtr<UB2Image>			IMG_ItemGrade;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_ItemName;

	TWeakObjectPtr<UB2Image>			IMG_ItemLock;

	TWeakObjectPtr<UB2Button>			BTN_CostumeItemSlot;

	TWeakObjectPtr<UOverlay>			O_EquipMark;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_EquipMark;
	TWeakObjectPtr<UTextBlock>			TB_ItemEnhanceLevel;
	TWeakObjectPtr<UTextBlock>			TB_ItemCount;

	TWeakObjectPtr<class UB2UIWidgetBase>	UIP_RedDot;

private:
	FB2Item CachedB2ItemInfo;
};
