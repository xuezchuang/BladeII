// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UICostumeDecompositionPopup.generated.h"


UCLASS()
class BLADEII_API UB2UICostumeDecompositionPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;
	
	void UnsubscribeEvent();
	void SubscribeEvent();

	void UpdatePopupInfo(const FB2Item& InItem);
	void UpdateResultPopupInfo(const FB2ResponseDismantleCostumesPtr& msg);

protected:
	UFUNCTION()
	void OnClickBTN_Modal();
	UFUNCTION()
	void OnClickBTN_NO();
	UFUNCTION()
	void OnClickBTN_Yes();
	UFUNCTION()
	void OnClickBTN_Confirm();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayAnimation_IMP();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UB2UICostumeDecompositionItem>	UICostumeItemClass;

private:
	TWeakObjectPtr<UHorizontalBox> List_MainMaterials;
	TWeakObjectPtr<UHorizontalBox> List_ReturnMaterial;
	TWeakObjectPtr<UPanelWidget> P_MoreItems;

	TWeakObjectPtr<UWidgetSwitcher> WS_Buttons;

	TWeakObjectPtr<UB2Button> BTN_Modal;
	TWeakObjectPtr<UB2Button> BTN_NO;
	TWeakObjectPtr<UB2Button> BTN_Yes;
	TWeakObjectPtr<UB2Button> BTN_Confirm;

	TWeakObjectPtr<UB2RichTextBlock> RTB_ObtainDesc;

	TWeakObjectPtr<UTextBlock> STB_No;
	TWeakObjectPtr<UTextBlock> STB_Yes;
	TWeakObjectPtr<UTextBlock> STB_Confirm;
	TWeakObjectPtr<UTextBlock> STB_ReturnMaterial;
	
	TWeakObjectPtr<UTextBlock> STB_Title;

	int64 CurrentCostumeIndex;

	TArray<std::shared_ptr<class issue>> Issues;
};

UCLASS()
class BLADEII_API UB2UICostumeDecompositionItem : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetInfo(int32 ItemIndex, int32 Amount);
	void SetInfo(int32 ItemIndex, int32 CurrentCount, int32 NeedCount);
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayAnimation_IMP();

private:
	TWeakObjectPtr<UImage> IMG_Item;
	TWeakObjectPtr<UTextBlock> TB_Count;
	TWeakObjectPtr<UTextBlock> TB_NeedCount;
	TWeakObjectPtr<class UB2ButtonGoodInfoToolTip> BTN_StuffTip;
};