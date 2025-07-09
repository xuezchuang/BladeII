// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDonationResultPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIDonationResultPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public	:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	UFUNCTION(BlueprintImplementableEvent)
		void PlayTenOpen();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayOneOpen();
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

private:
	UFUNCTION()
		void OnClickConfirm();
	UFUNCTION()
		void OnClickOneMore();
	UFUNCTION()
		void OnClickItem(UB2UISlotItem* ClickedItem);
	UFUNCTION()
		void CloseItemDetailPopup();

	void GetResultItems(OUT TArray<FB2Item>& OutItem);
	void InitCostIcon();
	void UpdateUIInfo();
	void UpdateTenUIInfo(TArray<FB2Item>& InResultItems);
	void UpdateOneUIInfo(FB2Item& InResultItem);

#if WITH_EDITOR
	void EditorOnPostLoad();
#endif
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DonationPointStepReward")
		TSoftObjectPtr<class UMaterialInterface> GemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DonationPointStepReward")
		TSoftObjectPtr<class UMaterialInterface> GoldIcon;

private:
	TArray<TWeakObjectPtr<class UB2UISlotItem>> UIP_TenDonationResultItems;
	TWeakObjectPtr<class UB2UISlotItem> UIP_FixedItem;
	TWeakObjectPtr<class UB2UISlotItem> UIP_OneDonationResultItem;
	TWeakObjectPtr<class UB2UIDonationProductCost> UIP_DonationProductCost;
	TWeakObjectPtr<class UB2LobbyUI_ItemDetailPostOp> UIP_ItemDetailPopup;
	TWeakObjectPtr<UB2Button> BTN_Confirm;
	TWeakObjectPtr<UB2Button> BTN_OneMore;
	TWeakObjectPtr<UTextBlock> TB_OneMore;
	TWeakObjectPtr<UTextBlock> TB_Confirm;
	TWeakObjectPtr<UTextBlock> TB_CostAmount;
	TWeakObjectPtr<UTextBlock> TB_DonationPointTitle;
	TWeakObjectPtr<UTextBlock> TB_DoantionPoint;
	TWeakObjectPtr<UTextBlock> TB_FixedItem;
	TWeakObjectPtr<UCanvasPanel> CP_DetailPopup;
	TWeakObjectPtr<UImage> IMG_Icon;
	TWeakObjectPtr<UWidgetAnimation> ANIM_FinalResult;
	TArray<issue_ptr>	Issues;
	bool IsTen;
};
